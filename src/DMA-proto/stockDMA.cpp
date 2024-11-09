#include "stockDMA.h"

stockDMA::stockDMA(p_comm_device_interface *p_devComm) : DMA_proto(p_devComm)
{
    name = "stock DMA proto by nanner55";
    type = DMA_ProtoType::nanner55;
// qDebug() << "stockDMA::stockDMA(p_comm_device_interface *p_devComm)" << this->p_devComm;
}

stockDMA::~stockDMA()
{
    qDebug() << "~stockDMA";
}

bool stockDMA::connect_()
{
    qDebug() << "=========== stockDMA::connect ================ baudRate" << (*p_devComm)->getBaudRate();
    if((*p_devComm)->open())
    {
        qDebug() << "=========== stockDMA::connect ================ open" << (*p_devComm)->getBaudRate();
        if ((*p_devComm)->connect(Protocol::ISO9141))
        {
            qDebug() << "=========== stockDMA::connect ================ connect" << (*p_devComm)->getBaudRate();
            return true;
        }
    }
    (*p_devComm)->close();
    return false;
}



QByteArray stockDMA::indirectDMAread(quint32 addr, int lenght)
{
    return QByteArray();
}

QByteArray stockDMA::directDMAread(quint32 addr, int lenght)
{
    //qDebug()<<"=========== stockDMA::directDMAread ================";
    QByteArray a;
    uchar packetSize = 0x33;
    uchar packetBodySize = 0x2C;
    uchar recPacketCount = (lenght + packetBodySize - 1) / packetBodySize; //сколько 44байтных пакетов нужно принять
    //getRead( recPacketCount, addr);
    setHeader(DMAcomand::directRead, recPacketCount, addr);
    // checksum
    getChckSmm();
    (*p_devComm)->write( 0x33 );
    QByteArray b = (*p_devComm)->read();

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

void stockDMA::directDMAwrite(quint32 addr, char *buf, int lenght)
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
        memcpy((*p_devComm)->p_out_buff + 5, buf + count*packetBodySize, currentPacketBodySize);
        getChckSmm();
        //QElapsedTimer t;
        //t.restart();
        (*p_devComm)->write(packetSize);
        //qDebug()  << "directDMAwrite write time " << t.nsecsElapsed();
        //t.restart();
        //auto a = read()
        //        ; //read ECU response
        //qDebug()  << "directDMAwrite read size"<<a.size()<<"time" << t.nsecsElapsed()<<a.toHex(':')
        ;
        addr += packetBodySize;
    }
}

void stockDMA::startLog(ramMUT *ramMut)
{

}

void stockDMA::startLog()
{

}

void stockDMA::stopLog()
{

}

void stockDMA::setLogRate(int freqRate)
{
    poller->setLogRate(1/freqRate);
}

void stockDMA::RAMreset(quint32 var1, quint16 var2)
{

}

void stockDMA::updateRAM(offsetMemory memory)
{

}

void stockDMA::setHeader(DMAcomand command, uchar count, quint32 addr)
{
    //uchar packetBodySize = 0x2C;
    //clear body
    for (uint i = 5; i < DS - 2; i++)
        (*p_devComm)->p_out_buff[i]=0;
    //============================================
    (*p_devComm)->p_out_buff[0] = 0x87;
    (*p_devComm)->p_out_buff[1] = count;
    (*p_devComm)->p_out_buff[2] = (uchar)command;
    if(command == DMAcomand::directRead)
        qToBigEndian<quint32>(addr, (char*)(*p_devComm)->p_out_buff + 3);
    if(command == DMAcomand::directWrite)
    {
        quint16 a = 0xffff&addr;
        qToBigEndian<quint16>(a, (char*)(*p_devComm)->p_out_buff + 3);
        // qDebug() << "setHeader"<< QString::number(a, 16);
    }
}

void stockDMA::getChckSmm()
{
    // checksum
    uchar chckSum = 0;
    for (uint i = 0; i < DS - 2; i++)
        chckSum += (*p_devComm)->p_out_buff[i];
    (*p_devComm)->p_out_buff[DS - 2] = chckSum;

    // trailer
    (*p_devComm)->p_out_buff[DS - 1] = 0x0D;
}
