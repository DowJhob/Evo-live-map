#include "op20wb.h"

op20wb::op20wb(OP20 *op20): op20(op20)// : j2534_interface(dllName, DeviceDesc, DeviceUniqueID)
{
    DeviceDesc = op20->DeviceDesc + " / " + op20->DeviceUniqueID;
    //pollHelper();
}

op20wb::~op20wb()
{
}

bool op20wb::openWB(uint baudRate)
{
    if ( PassThru::Status status = op20->j2534->PassThruOpen(nullptr, &op20->devID); status == PassThru::Status::NoError || status == PassThru::Status::DeviceInUse )
    {
        op20->WBinUse = true;
        // try to connect to the specific channel we would like to use
        //
        // in this case, it is the 2.5mm jack on the Openport 2.0 which can be used as
        // a RS-232 voltage level receive only input. the Innovate MTS bus
        // can be used this way, as it is 19200 baud, N,8,1 serial and transmits continuously
        // without any polling needed.
        //
        // note that the ISO9141_NO_CHECKSUM connection flag is used to avoid requiring the data
        // to have valid ISO9141 checksums (it doesn't)
        if (op20->j2534->PassThruConnect(op20->devID, Protocol::ISO9141_INNO, ConnectFlag::ISO9141NoChecksum, baudRate, &chanID_INNO) == PassThru::Status::NoError )
        {
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
            if (op20->j2534->PassThruStartMsgFilter(chanID_INNO, PassThru::PassFilter, &msgMask, &msgPattern, NULL, &msgId) == PassThru::Status::NoError)
            {
                //QMetaObject::invokeMethod(pollTimer, "start");
                qDebug() << "==================== op20wb:openWB::PassThruConnected ==================================" << "devID" << op20->devID << "chanID_INNO" << chanID_INNO;
                return true;
            }
        }
    }
    qDebug() << "====================op20wb::openWB ================================== error" << op20->j2534->lastErrorString();

    return false;
}

bool op20wb::connectWB(uint baudRate)
{
    return true;
}

bool op20wb::closeWB()
{
    QThread::msleep(10*200);                   // это что бы таймер последний раз отработал чтение и не порушил при дальнейшем дисконнекте ( птздец костыль))
    // shut down the channel
    if (op20->j2534->PassThruDisconnect(chanID_INNO) == PassThru::Status::NoError )
    {
        op20->chanID_INNO = 0;
        op20->WBinUse = false;
        if(!op20->inUse)
            if (op20->j2534->PassThruClose(op20->devID) == PassThru::Status::NoError)
            {
                qDebug() << "==================== op20wb::close::PassThruClose closed succefull ==================================" << op20->j2534->lastErrorString();
                return true;
            }

        qDebug() << "==================== op20wb::close::PassThruClose only disconnect succefull ==================================" << op20->j2534->lastErrorString();
        return true;
    }
    qDebug() << "==================== op20wb::close::PassThruDisconnect ==================================" << op20->j2534->lastErrorString();
    return false;
}

QByteArray op20wb::readWB()
{
    QByteArray a;
    numRxMsg = 1;
    do
    {
        op20->j2534->PassThruReadMsgs(chanID_INNO, &rxmsg, &numRxMsg, 500);
        qDebug() << "==================== op20wb:readWB ==================================" << op20->j2534->lastErrorString() << chanID_INNO;
        a.append(QByteArray((char*)rxmsg.m_data, rxmsg.m_dataSize));
    }
    while(rxmsg.m_rxStatus == Message::RxStatusBit::InStartOfMessage);
    //qDebug() << "OP20::read: readWB" << a << "\n\n";

    return a;
}

void op20wb::poll()
{
    qDebug() << "op20wb::poll" ;
    QByteArray a = readWB();
    if (a.size() > 0)
        emit readyRead(a);
}
