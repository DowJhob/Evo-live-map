#include "op20.h"

OP20::OP20(QString dllName, QString DeviceDesc, QString DeviceUniqueID) : j2534_interface(dllName, DeviceDesc, DeviceUniqueID)
{
    //devType = deviceType::OP20;
    //qDebug() << "OP20" << DeviceUniqueID;
}

OP20::~OP20()
{
}

bool OP20::openWB()
{
    // nothing to open, opening in j2534_interface constructor
    return true;
}

bool OP20::connectWB(uint baudRate)
{
    // try to connect to the specific channel we would like to use
    //
    // in this case, it is the 2.5mm jack on the Openport 2.0 which can be used as
    // a RS-232 voltage level receive only input. the Innovate MTS bus
    // can be used this way, as it is 19200 baud, N,8,1 serial and transmits continuously
    // without any polling needed.
    //
    // note that the ISO9141_NO_CHECKSUM connection flag is used to avoid requiring the data
    // to have valid ISO9141 checksums (it doesn't)
    if (j2534->PassThruConnect(devID, Protocol::ISO9141_INNO, ConnectFlag::ISO9141NoChecksum, baudRate, &chanID_INNO))
    {
        qDebug() << "PassThruConnect: tactrix wb error " ;
        return false;
    }

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
    if (j2534->PassThruStartMsgFilter(chanID_INNO, PassThru::PassFilter, &msgMask, &msgPattern, NULL, &msgId))
    {
        qDebug() << "PassThruStartMsgFilter : tactrix wb error:" << reportJ2534Error();
        return false;
    }
    return true;
}

bool OP20::closeWB()
{
    if (j2534->PassThruDisconnect(chanID_INNO))
    {
        //   reportJ2534Error();
    }
    return true;
}

bool OP20::readWB()
{
    //qDebug() << "hop: " ;
    numRxMsg = 1;
    j2534->PassThruReadMsgs(chanID_INNO, &rxmsg, &numRxMsg, 40);
    // qDebug() << "hop22: " ;
    //if (numRxMsg)
    //            return QByteArray::fromRawData((char*)rxmsg.Data, rxmsg.DataSize);
    //return false;
    return true;
}
