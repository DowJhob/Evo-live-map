#include "evoX-DMA.h"

evoX_DMA::evoX_DMA(comm_device_interface **devComm)
{
    protocol = Protocol::ISO15765;
    //ConnectFlag = 0;
    baudRate = 500000;
    this->devComm = reinterpret_cast<j2534_interface**>(devComm);

    qDebug() << "stockDMA";
}

evoX_DMA::~evoX_DMA()
{
    qDebug() << "~stockDMA";
}

bool evoX_DMA::connect()
{
    (*devComm)->open(Protocol::ISO15765, //ConnectFlag((uint)
                     ConnectFlag::ISO9141NoChecksum //| (uint)ConnectFlag::ISO9141KLineOnly
                    //     )
                     );


    (*devComm)->j2534->PassThruIoctl((*devComm)->chanID, PassThru::IoctlID::CLEAR_RX_BUFFER, nullptr, nullptr);
    (*devComm)->j2534->PassThruIoctl((*devComm)->chanID, PassThru::IoctlID::CLEAR_TX_BUFFER, nullptr, nullptr);
    (*devComm)->j2534->PassThruIoctl((*devComm)->chanID, PassThru::IoctlID::CLEAR_MSG_FILTERS, nullptr, nullptr);

    //-======================================== SET CONFIG ===========================================
    Config scp[4] = { Config{Config::Parameter::DataRate, baudRate},
                       Config{Config::Parameter::ISO15765BS, 0x20},
                       Config{Config::Parameter::ISO15765STmin, 0},
                       Config{Config::Parameter::Loopback, 0}
                     };        // set timing

    const SArray<const Config> configList{4, scp};
    if ((*devComm)->j2534->PassThruIoctl((*devComm)->chanID, PassThru::SET_CONFIG, &configList, nullptr))
    {
        emit Log( "PassThruIoctl - SET_CONFIG : fail  " + (*devComm)->reportJ2534Error() );
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

    if ((msgId = (*devComm)->j2534->PassThruStartMsgFilter((*devComm)->chanID, PassThru::FlowControlFilter, &msgMask, &msgPattern, &msgFlowControl, &msgId)))
    {
        emit Log( "PassThruIoctl - PassThruStartMsgFilter : not ok  " + (*devComm)->reportJ2534Error() );
        return -1;
    }
    emit Log( "PassThruIoctl - PassThruStartMsgFilter : OK" );
    return msgId;

    QThread::msleep(100);

}

QByteArray evoX_DMA::indirectDMAread(quint32 addr, int lenght)
{
return QByteArray();
}

QByteArray evoX_DMA::directDMAread(quint32 addr, int lenght)
{
    //qDebug()<<"=========== directDMAread ================";
    QByteArray a;
    uchar packetSize = 0x33;
    uchar packetBodySize = 0x2C;
    uchar recPacketCount = (lenght + packetBodySize - 1) / packetBodySize; //сколько 44байтных пакетов нужно принять
    //getRead( recPacketCount, addr);
    setHeader(DMAcomand::directRead, recPacketCount, addr);
    // checksum
    getChckSmm();
    (*devComm)->write( 0x33 );
    QByteArray b = (*devComm)->read();

    int lastPacketBodySize = lenght - packetBodySize*(recPacketCount-1);
    //qDebug() << "directDMAread " << b.toHex(':');
    for (int i = 0; i < recPacketCount; i++)
    {
        if (i == recPacketCount-1)
            packetBodySize = lastPacketBodySize;
        a.append(QByteArray(b.data() + 5 + i*packetSize, packetBodySize));
    }
    return a;
}

void evoX_DMA::directDMAwrite(quint32 addr, char *buf, int lenght)
{
    uchar packetSize = 0x33;
    uchar packetBodySize = 0x2C;
    uchar currentPacketBodySize = packetBodySize;
    uchar recPacketCount = (lenght + packetBodySize - 1) / packetBodySize; //сколько 44байтных пакетов нужно передать
    uchar lastPacketBodySize = lenght - packetBodySize*(recPacketCount-1);

    for (int count = 0; count < recPacketCount; count++)
    {
        if (count == recPacketCount-1)
            currentPacketBodySize = lastPacketBodySize;
        setHeader( DMAcomand::directWrite, currentPacketBodySize, addr);
        memcpy((*devComm)->p_out_buff + 5, buf + count*packetBodySize, currentPacketBodySize);
        getChckSmm();
        //QElapsedTimer t;
        //t.restart();
        (*devComm)->write(packetSize);
        //qDebug()  << "directDMAwrite write time " << t.nsecsElapsed();
        //t.restart();
        //auto a = read()
        //        ; //read ECU response
        //qDebug()  << "directDMAwrite read size"<<a.size()<<"time" << t.nsecsElapsed()<<a.toHex(':')
        ;
        addr += packetBodySize;
    }
}

void evoX_DMA::setHeader(DMAcomand command, uchar count, quint32 addr)
{
    //uchar packetBodySize = 0x2C;
    //clear body
    for (uint i = 5; i < DS - 2; i++)
        (*devComm)->p_out_buff[i]=0;
    //============================================
    (*devComm)->p_out_buff[0] = 0x87;
    (*devComm)->p_out_buff[1] = count;
    (*devComm)->p_out_buff[2] = (uchar)command;
    if(command == DMAcomand::directRead)
        qToBigEndian<quint32>(addr, (char*)(*devComm)->p_out_buff + 3);
    if(command == DMAcomand::directWrite)
    {
        quint16 a = 0xffff&addr;
        qToBigEndian<quint16>(a, (char*)(*devComm)->p_out_buff + 3);
        // qDebug() << "setHeader"<< QString::number(a, 16);
    }
}

void evoX_DMA::getChckSmm()
{
    // checksum
    uchar chckSum = 0;
    for (uint i = 0; i < DS - 2; i++)
        chckSum += (*devComm)->p_out_buff[i];
    (*devComm)->p_out_buff[DS - 2] = chckSum;

    // trailer
    (*devComm)->p_out_buff[DS - 1] = 0x0D;
}
