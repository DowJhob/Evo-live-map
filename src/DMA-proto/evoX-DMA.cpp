#include "evoX-DMA.h"

evoX_DMA::evoX_DMA()
{
    qDebug() << "evoX_DMA";
}

evoX_DMA::evoX_DMA(comm_device_interface **devComm)
{
    this->devComm = devComm;

    qDebug() << "evoX_DMA";
}

evoX_DMA::~evoX_DMA()
{
    qDebug() << "~evoX_DMA";
}

bool evoX_DMA::connect_()
{
    qDebug() << "=========== evoX_DMA::connect ================ baudRate" << (*devComm)->getBaudRate();
    (*devComm)->open();

    if (!(*devComm)->connect())
    {
        (*devComm)->close();
        return false;
    }

    //==================================   5 baud init  ========================================
    if ( !(*devComm)->five_baud_init() )
    {
        (*devComm)->close();
        return false;
    }
    return true;
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

void evoX_DMA::startLog(ramMUT *ramMut)
{

}

void evoX_DMA::startLog()
{

}

void evoX_DMA::stopLog()
{

}

void evoX_DMA::setLogRate(int freqRate)
{

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
