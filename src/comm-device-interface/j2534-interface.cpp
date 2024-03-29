#include "j2534-interface.h"

j2534_interface::j2534_interface(QString dllName, QString DeviceDesc, QString DeviceUniqueID) : comm_device_interface( dllName, DeviceDesc, DeviceUniqueID)
{
    //devType = deviceType::J2534;
    p_in_buff = rx_msg.m_data;
    p_out_buff = tx_msg.m_data;

    j2534 = new PassThru(dllName);
    ////connect(this, &j2534_interface::_loop, this, &j2534_interface::loop, Qt::QueuedConnection);            //
    //init();
    //qDebug() << "j2534_interface" << DeviceUniqueID;
}

j2534_interface::~j2534_interface()
{
    j2534->deleteLater();
    //qDebug() << "~j2534_interface";
}

bool j2534_interface::info()
{
    if (devID == 0)
        if (j2534->PassThruOpen(nullptr, &devID))         // Get devID
        {
            qDebug() << "==================== j2534_interface:info open ==================================" << j2534->lastErrorString();
            //emit Log("PassThruOpen error: " + reportJ2534Error());
            return false;
        }
    setUse();
    qDebug() << "==================== j2534_interface:info open2 ==================================" << j2534->lastErrorString();
    //emit Log("PassThruOpen deviceID: " + QString::number(devID) + " /opened");

    //    //читаем версию и прочюю требуху
    //    char strApiVersion[256];
    //    char strDllVersion[256];
    //    char strFirmwareVersion[256];
    //    char strSerial[256];
    //    if ( j2534->PassThruReadVersion(strApiVersion, strDllVersion, strFirmwareVersion, devID) )
    //    {
    //        emit Log("PassThruReadVersion: " + reportJ2534Error());
    //    }
    //    else
    //    {
    //        emit Log("J2534 API Version: " + QString(strApiVersion) + "\r\n"
    //                 + "J2534 DLL Version: " +  QString(strDllVersion) + "\r\n"
    //                 + "Device Firmware Version: " +  QString(strFirmwareVersion));
    //    }
    //    if (get_serial_num(devID, strSerial))
    //        emit Log("Device Serial Number: " +  QString(strSerial));
    //    else
    //        emit Log("get_serial_num: " + reportJ2534Error());

    if (j2534->PassThruClose(devID) != PassThru::Status::NoError)
    {
        qDebug()<<"PassThruClose error: ";
        return false;
    }

    resetUse();
    return true;
}

bool j2534_interface::open(Protocol protocol, enum ConnectFlag ConnectFlag, uint baudRate)
{
    this->protocol = protocol;
    this->baudRate = baudRate;

    rx_msg.setProtocolId(protocol);
    tx_msg.setProtocolId(protocol);

    if (isNotUse())
    {
        if (j2534->PassThruOpen(nullptr, &devID))         // Get devID
        {
            qDebug() << "==================== j2534_interface::open::PassThruOpen ==================================" << j2534->lastErrorString();
            return false;
        }
        setUse();;
    }
    if (chanID == 0)
        if (j2534->PassThruConnect(devID, protocol, ConnectFlag, baudRate, &chanID))
        {
            qDebug() << "==================== j2534_interface::open::PassThruConnect ==================================" << j2534->lastErrorString() << "chanID" << chanID;
            chanID = 0;
            return false;
        }
    return true;
}

bool j2534_interface::close()
{
    //    if (j2534->PassThruStopMsgFilter(chanID, msgId) != PassThru::Status::NoError )
    //    {
    //        qDebug() << "==================== j2534_interface::close::PassThruStopMsgFilter ==================================" << j2534->lastErrorString();
    //        //emit Log("PassThruStopMsgFilter error: " + reportJ2534Error() );
    //        reportJ2534Error();
    //        return false;
    //    }
    //emit Log("PassThruStopMsgFilter channel: " + QString::number(chanID) + " /stopped" );

    // shut down the channel
    if (chanID != 0)
        if (j2534->PassThruDisconnect(chanID) != PassThru::Status::NoError )
        {
            qDebug() << "==================== j2534_interface::close::PassThruDisconnect ==================================" << j2534->lastErrorString();
            chanID = 0;
            return false;
        }
    chanID = 0;

    if (resetUse())
        if (j2534->PassThruClose(devID) != PassThru::Status::NoError)
        {
            qDebug() << "==================== j2534_interface::close::PassThruClose ==================================" << j2534->lastErrorString();
            chanID = 0;
            return false;
        }
    return true;
}

QByteArray j2534_interface::read(uint lenght)
{
    QByteArray a;
    QElapsedTimer tt;
    NumMsgs = 1;
    rx_msg.m_rxStatus = 0;
    tt.start();
    do
    {
        j2534->PassThruReadMsgs(chanID, &rx_msg, &NumMsgs, _readTimeout);
        a.append(QByteArray((char*)rx_msg.m_data, rx_msg.m_dataSize));
        //        qDebug()<< "j2534_interface::read " << "rx_msg.m_rxStatus" << rx_msg.m_rxStatus
        //                << "rx_msg.m_dataSize" << rx_msg.m_dataSize
        //                << "time" << QString::number( tt.nsecsElapsed()/1000000.0)
        //                << "NumMsgs=" << NumMsgs
        //                   //<< "rx_msg[0].data" << QByteArray((char*)rx_msg[0].m_data, 4).toHex(':')
        //                   ;
    }
    while(rx_msg.m_rxStatus == Message::RxStatusBit::InStartOfMessage);

    qDebug() << "j2534_interface::read: total" << QString::number( tt.nsecsElapsed()/1000000.0) << "\n\n";

    return a;
}

void j2534_interface::write(int lenght)
{
    NumMsgs = 1;
    tx_msg.m_dataSize = lenght;
    j2534->PassThruWriteMsgs(chanID, &tx_msg, &NumMsgs, writeTimeout);
}

bool j2534_interface::five_baud_init()
{
    //==========================================   5 baud init  ================================
    uchar EcuAddr[1]; /* ECU target address array */
    uchar KeyWord[3]; /* Keyword identifier array */

    SArray<uchar> inputMsg(1, EcuAddr), outputMsg(0, KeyWord);

    EcuAddr[0] = 0x00;
    //EcuAddr[0] = 0x33; /* Initialization address used to activate all ECUs */
    //inputMsg.NumOfBytes = 1; /* ECU target address array contains one address. */
    //inputMsg.BytePtr = &EcuAddr[0]; /* Assign pointer to ECU target address array. */
    //outputMsg.NumOfBytes = 0; /* KeyWord array is empty. */
    //outputMsg.BytePtr = &KeyWord[0]; /* Assign pointer to KeyWord array. */

    if (j2534->PassThruIoctl(chanID, PassThru::FIVE_BAUD_INIT, (void*)(&inputMsg), (void*)(&outputMsg)))
    {
        qDebug() << "jPassThruIoctl - FIVE_BAUD_INIT FAIL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        //emit Log( "PassThruIoctl - FIVE_BAUD_INIT : not ok  | " + reportJ2534Error()  + QByteArray((char*)KeyWord, 3).toHex(':'));
        return false;
    }

    //emit Log( "PassThruIoctl - FIVE_BAUD_INIT : OK  |  " + QByteArray((char*)KeyWord, 3).toHex(':'));
    return true;
}

bool j2534_interface::ISO9141()
{
    Config scp[10] = { Config{Config::Parameter::DataRate, baudRate},
                       //  Config{Config::Parameter::W0, 800},
                       //   Config{Config::Parameter::W1, 300},
                       //   Config{Config::Parameter::W2, 3000},
                       //     Config{Config::Parameter::W3, 300},
                       //    Config{Config::Parameter::W4, 1000},
                       Config{Config::Parameter::P1Min, 0},
                       Config{Config::Parameter::P1Max, 1},           // сколько ждать на реальном чтении
                       Config{Config::Parameter::P2Min, 25},
                       Config{Config::Parameter::P2Max, 30},
                       Config{Config::Parameter::P3Min, 0},           // уменьшает в 4 раза отклик контроллера!!!
                       Config{Config::Parameter::P3Max, 1},
                       Config{Config::Parameter::P4Min, 0},           // уменьшает в 30 раз отклик контроллера!!!
                       Config{Config::Parameter::P4Max, 1},
                       Config{Config::Parameter::Loopback, 0}
                     };        // set timing

    const SArray<const Config> configList{10, scp};
    if (j2534->PassThruIoctl(chanID, PassThru::SET_CONFIG, &configList, nullptr))
    {
        //emit Log( "PassThruIoctl - SET_CONFIG : fail  " + reportJ2534Error() );
        return false;
    }
    // ============================ setup filter(s) =========================
    Message msgMask, msgPattern;
    msgMask.m_protocolId = ulong(protocol);
    msgMask.m_rxStatus = 0;
    msgMask.m_txFlags = 0;
    msgMask.m_timestamp = 0;
    msgMask.m_dataSize = 1;
    msgMask.m_extraDataIndex = 0;

    msgPattern  = msgMask;
    memset(msgMask.m_data,0,1); // mask the first 4 byte to 0
    memset(msgPattern.m_data,0,1);// match it with 0 (i.e. pass everything)

    if ((msgId = j2534->PassThruStartMsgFilter(chanID, PassThru::PassFilter, &msgMask, &msgPattern, nullptr, &msgId)))
    {
        //emit Log( "PassThruIoctl - PassThruStartMsgFilter : not ok  " + reportJ2534Error() );
        return -1;
    }

    //emit Log( "PassThruIoctl - PassThruStartMsgFilter : OK" );

    j2534->PassThruIoctl(chanID, PassThru::IoctlID::CLEAR_RX_BUFFER, nullptr, nullptr);
    j2534->PassThruIoctl(chanID, PassThru::IoctlID::CLEAR_TX_BUFFER, nullptr, nullptr);

    return true;
}

bool j2534_interface::ISO15765()
{
    j2534->PassThruIoctl(chanID, PassThru::IoctlID::CLEAR_RX_BUFFER, nullptr, nullptr);
    j2534->PassThruIoctl(chanID, PassThru::IoctlID::CLEAR_TX_BUFFER, nullptr, nullptr);
    j2534->PassThruIoctl(chanID, PassThru::IoctlID::CLEAR_MSG_FILTERS, nullptr, nullptr);

    //-======================================== SET CONFIG ===========================================
    Config scp[4] = { Config{Config::Parameter::DataRate, baudRate},
                      Config{Config::Parameter::ISO15765BS, 0x20},
                      Config{Config::Parameter::ISO15765STmin, 0},
                      Config{Config::Parameter::Loopback, 0}
                    };        // set timing

    const SArray<const Config> configList{4, scp};
    if (j2534->PassThruIoctl(chanID, PassThru::SET_CONFIG, &configList, nullptr))
    {
        //emit Log( "PassThruIoctl - SET_CONFIG : fail  " + reportJ2534Error() );
        return false;
    }
    // ============================ setup the filter(s) =========================
    unsigned long msgId;

    Message msgMask, msgPattern, msgFlowControl;

    msgMask.m_protocolId = ulong(protocol);
    msgMask.m_rxStatus   = 0;
    msgMask.m_txFlags    = Message::TxFlag::OutISO15765FramePad;
    msgMask.m_timestamp  = 0;
    msgMask.m_dataSize   = 4;
    msgMask.m_extraDataIndex = 0;
    msgMask.m_data[0] = 0xFF;
    msgMask.m_data[1] = 0xFF;
    msgMask.m_data[2] = 0xFF;
    msgMask.m_data[3] = 0xFF;

    msgPattern.m_protocolId = ulong(protocol);
    msgPattern.m_rxStatus = 0;
    msgPattern.m_txFlags = Message::TxFlag::OutISO15765FramePad;
    msgPattern.m_timestamp = 0;
    msgPattern.m_dataSize = 4;
    msgPattern.m_extraDataIndex = 0;
    msgPattern.m_data[0] = 0;
    msgPattern.m_data[1] = 0;
    msgPattern.m_data[2] = 7;
    msgPattern.m_data[3] = 0xE8;

    msgFlowControl.m_protocolId = ulong(protocol);
    msgFlowControl.m_rxStatus = 0;
    msgFlowControl.m_txFlags = Message::TxFlag::OutISO15765FramePad;
    msgFlowControl.m_timestamp = 0;
    msgFlowControl.m_dataSize = 4;
    msgFlowControl.m_extraDataIndex = 0;
    msgFlowControl.m_data[0] = 0;
    msgFlowControl.m_data[1] = 0;
    msgFlowControl.m_data[2] = 7;
    msgFlowControl.m_data[3] = 0xE0;

    if ((msgId = j2534->PassThruStartMsgFilter(chanID, PassThru::FlowControlFilter, &msgMask, &msgPattern, &msgFlowControl, &msgId)))
    {
        //emit Log( "PassThruIoctl - PassThruStartMsgFilter : not ok  " + reportJ2534Error() );
        return -1;
    }
    //emit Log( "PassThruIoctl - PassThruStartMsgFilter : OK" );
    return msgId;

    QThread::msleep(100);
}

bool j2534_interface::ISO14230()
{
    return true;
}

bool j2534_interface::get_serial_num(unsigned long devID, char *serial)
{
    inbuf inbuf;
    outbuf outbuf;
    inbuf.length = 2;
    inbuf.svcid = 5; // info
    inbuf.infosvcid = 1; // serial

    outbuf.length = sizeof( outbuf.data);

    if (j2534->PassThruIoctl(devID, PassThru::TX_IOCTL_APP_SERVICE, &inbuf, &outbuf))
    {
        serial[0] = 0;
        return false;
    }

    memcpy(serial,  outbuf.data,  outbuf.length);
    serial[ outbuf.length] = 0;
    return true;
}
