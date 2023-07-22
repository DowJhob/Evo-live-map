#include "jcsbanksDMA.h"

jcsbanksDMA::jcsbanksDMA()
{
    qDebug() << "jcsbanksDMA";
    poller = new pollHelper(this);
}

jcsbanksDMA::jcsbanksDMA(comm_device_interface **devComm)
{
    this->devComm = devComm;
    poller = new pollHelper(this);
    qDebug() << "jcsbanksDMA";
}

jcsbanksDMA::~jcsbanksDMA()
{
    poller->deleteLater();
    qDebug() << "~jcsbanksDMA";
}

bool jcsbanksDMA::connect()
{
    qDebug() << "=========== jcsbanksDMA::connect ================ baudRate" << (*devComm)->getBaudRate();
    if((*devComm)->open(Protocol::ISO9141, ConnectFlag::ISO9141NoChecksum , (*devComm)->getBaudRate()))
    {
        qDebug() << "=========== jcsbanksDMA::connect ================ open" << (*devComm)->getBaudRate();
        if ((*devComm)->connect())
        {
            qDebug() << "=========== jcsbanksDMA::connect ================ connect" << (*devComm)->getBaudRate();
            if ( (*devComm)->five_baud_init() )
            {
                qDebug() << "=========== jcsbanksDMA::connect ================ 5 baud init" << (*devComm)->getBaudRate();
                return true;
            }
        }
    }
    (*devComm)->close();
    return false;
}

QByteArray jcsbanksDMA::indirectDMAread(quint32 addr, int lenght)
{
    //qDebug() << "jcsbanksDMA::directDMAread";
    sendDMAcomand(0xE0, addr, lenght, nullptr);
    //QByteArray a = (*devComm)->read(lenght);
    //qDebug() << "jcsbanksDMA::directDMAread" << a.toHex(':');
    //return a;
    return (*devComm)->read(lenght);
}

QByteArray jcsbanksDMA::directDMAread(quint32 addr, int lenght)
{
    //qDebug() << "jcsbanksDMA::directDMAread";
    sendDMAcomand(0xE1, addr, lenght, nullptr);
    //qDebug() << "jcsbanksDMA::directDMAread2";
    return (*devComm)->read(lenght);
}

void jcsbanksDMA::directDMAwrite(quint32 addr, char* buf, int lenght)
{
    qDebug()<<"=========== jcsbanksDMA::directDMAwrite ================" << QString::number(addr, 16) << QByteArray(buf, 1).toHex(':');
    sendDMAcomand(0xE2, addr, lenght, buf);
}

void jcsbanksDMA::startLog(ramMUT *_ramMut)
{
    qDebug()<<"=========== jcsbanksDMA::startLog ================" << QThread::currentThread();
    DMA_proto::startLog(_ramMut);
    poller->startLog();
    //pollTimer->start();
}

void jcsbanksDMA::stopLog()
{
    qDebug()<<"=========== jcsbanksDMA::stopLog ================";
    poller->stopLog();
    //pollTimer->stop();
}

//void jcsbanksDMA::poll()
//{
//    //qDebug() << "jcsbanksDMA::poll" << QThread::currentThread() << ramMut->byteSize;
//    offsetMemory a = indirectDMAread(ramMut->addr, ramMut->byteSize);
//    for(int i = 0; i < ramMut->size(); i++)
//    {
//        ramMut->scaledValue[i] = a.toFloatOffset( &(*ramMut)[i].scaling, ramMut->at(i).offset );
//    }

//    emit logReady(ramMut->scaledValue);
//}

void jcsbanksDMA::sendDMAcomand(char command, unsigned long addr, unsigned long count, char *buf)
{
    //int subcount =0;
    (*devComm)->p_out_buff[0] = command;
    (*devComm)->write( 1 );
    QThread::msleep(delay_after_command);
    (*devComm)->p_out_buff[0] = (addr & 0xFF000000) >> 24;
    (*devComm)->p_out_buff[1] = (addr & 0xFF0000) >> 16;
    (*devComm)->p_out_buff[2] = (addr & 0xFF00) >> 8;
    (*devComm)->p_out_buff[3] = (addr & 0xFF);
    (*devComm)->p_out_buff[4] = (count & 0xFF00) >> 8;
    (*devComm)->p_out_buff[5] = (count & 0xFF);
    (*devComm)->write( 6 );
    //QThread::msleep(1);
    if ( buf != nullptr)
    {
        memcpy((*devComm)->p_out_buff, buf, count);
        //qDebug() << "jcsbanksDMA::sendDMAcomand payload data" << count;
        (*devComm)->write( count);
    }

    //(*devComm)->write( 6+subcount);
}
