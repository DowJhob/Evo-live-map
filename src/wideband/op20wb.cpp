#include "op20wb.h"

op20wb::op20wb(OP20 *j2534): op20(j2534)// : j2534_interface(dllName, DeviceDesc, DeviceUniqueID)
{
    //devType = deviceType::OP20;
    //qDebug() << "OP20" << DeviceUniqueID;

    DeviceDesc = j2534->DeviceDesc + " / " + j2534->DeviceUniqueID;
    connect(j2534, &OP20::destroyed, this, &OP20::deleteLater);
}

op20wb::~op20wb()
{
}

bool op20wb::isClosed()
{
    if (op20->chanID_INNO == 0)
        return true;
    else
        return false;
}

bool op20wb::openWB(uint baudRate)
{
    //qDebug() << "==================== OP20:openWB ==================================" << j2534->lastErrorString();
    if (op20->isNotUse())
        if (op20->j2534->PassThruOpen(nullptr, &op20->devID))         // Get devID
        {
            qDebug() << "==================== OP20:openWB::PassThruOpen ==================================" << op20->j2534->lastErrorString();
            return false;
        }
    op20->setUse();

    //qDebug() << "==================== OP20:openWB3 ==================================" << j2534->lastErrorString();

    // try to connect to the specific channel we would like to use
    //
    // in this case, it is the 2.5mm jack on the Openport 2.0 which can be used as
    // a RS-232 voltage level receive only input. the Innovate MTS bus
    // can be used this way, as it is 19200 baud, N,8,1 serial and transmits continuously
    // without any polling needed.
    //
    // note that the ISO9141_NO_CHECKSUM connection flag is used to avoid requiring the data
    // to have valid ISO9141 checksums (it doesn't)
    qDebug() << "==================== OP20:openWB::PassThruConnect before ==================================" << op20->j2534->lastErrorString() << "devID" << op20->devID << "chanID_INNO" << op20->chanID_INNO;
    if (op20->chanID_INNO == 0)
        if (op20->j2534->PassThruConnect(op20->devID, Protocol::ISO9141_INNO, ConnectFlag::ISO9141NoChecksum, baudRate, &op20->chanID_INNO))
        {
            qDebug() << "==================== OP20:openWB::PassThruConnect after ==================================" << op20->j2534->lastErrorString() << "devID" << op20->devID << "chanID_INNO" << op20->chanID_INNO;

            return false;
        }
    qDebug() << "==================== OP20:openWB::PassThruConnect2 ==================================" << "devID" << op20->devID << "chanID_INNO" << op20->chanID_INNO;
    // all J2534 channels need filters in order to receive anything at all
    //
    // in this case, we simply create a "pass all" filter so that we can see
    // everything unfiltered in the raw stream

    rxmsg.setProtocolId(Protocol::ISO9141_INNO);
    rxmsg.m_rxStatus = 0;
    rxmsg.m_txFlags = 0;
    rxmsg.m_timestamp = 0;
    rxmsg.m_dataSize = 1;
    rxmsg.m_extraDataIndex = 0;
    msgMask = msgPattern  = rxmsg;
    msgMask.m_data[0] = 0; // mask the first byte to 0
    msgPattern.m_data[0] = 0; // match it with 0 (i.e. pass everything)
    if (op20->j2534->PassThruStartMsgFilter(op20->chanID_INNO, PassThru::PassFilter, &msgMask, &msgPattern, NULL, &msgId))
    {
        qDebug() << "==================== OP20:openWB::PassThruStartMsgFilter ==================================" << op20->j2534->lastErrorString();
        return false;
    }

    return true;
}

bool op20wb::connectWB(uint baudRate)
{
    return true;
}

bool op20wb::closeWB()
{
    qDebug() << "\n==================== OP20::closeWB ==================================" << "chanID_INNO" << op20->chanID_INNO;
    if (op20->j2534->PassThruDisconnect(op20->chanID_INNO))
    {
        qDebug() << "==================== OP20:closeWB::PassThruDisconnect ==================================" << op20->j2534->lastErrorString();
    }

    op20->resetUse();
    if(op20->countUSE <= 0)  // если последний то закрываем
    {
        op20->countUSE = 0;   // на всякий случай
        op20->j2534->PassThruClose(op20->devID);
        op20->chanID_INNO = 0;
    }

    op20->chanID_INNO = 0;
    return true;
}

QByteArray op20wb::readWB()
{
    QByteArray a;
    numRxMsg = 1;
    do
    {
        op20->j2534->PassThruReadMsgs(op20->chanID_INNO, &rxmsg, &numRxMsg, 140);
        qDebug() << "==================== OP20:readWB ==================================" << op20->j2534->lastErrorString() << op20->chanID_INNO;
        a.append(QByteArray((char*)rxmsg.m_data, rxmsg.m_dataSize));
        //        qDebug()<< "j2534_interface::read " << "rx_msg.m_rxStatus" << rx_msg.m_rxStatus
        //                << "rx_msg.m_dataSize" << rx_msg.m_dataSize
        //                << "time" << QString::number( tt.nsecsElapsed()/1000000.0)
        //                << "NumMsgs=" << NumMsgs
        //                   //<< "rx_msg[0].data" << QByteArray((char*)rx_msg[0].m_data, 4).toHex(':')
        //                   ;
    }
    while(rxmsg.m_rxStatus == Message::RxStatusBit::InStartOfMessage);

    //qDebug() << "OP20::read: readWB" << a << "\n\n";

    //
    return a;
}
