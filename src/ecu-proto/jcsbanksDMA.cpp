#include "jcsbanksDMA.h"

jcsbanksDMA::jcsbanksDMA(comm_device_interface **devComm)
{
    this->devComm = devComm;

    qDebug() << "jcsbanksDMA";
}

jcsbanksDMA::~jcsbanksDMA()
{
    qDebug() << "~jcsbanksDMA";
}

bool jcsbanksDMA::connect()
{
    qDebug() << "=========== ECU connect ================";
     ///if (!devComm->connect( ISO9141_K, ISO9141_NO_CHECKSUM))
     if (!(*devComm)->connect( Protocol::ISO9141, ConnectFlag(
                                (uint)ConnectFlag::ISO9141NoChecksum //| (uint)ConnectFlag::ISO9141KLineOnly
                                )
                            ))
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

QByteArray jcsbanksDMA::indirectDMAread(quint32 addr, int lenght)
{
    //qDebug() << "jcsbanksDMA::directDMAread";
    sendDMAcomand(0xE0, addr, lenght, nullptr);
    //qDebug() << "jcsbanksDMA::directDMAread2";
    return (*devComm)->read();
}

QByteArray jcsbanksDMA::directDMAread(quint32 addr, int lenght)
{
    //qDebug() << "jcsbanksDMA::directDMAread";
    sendDMAcomand(0xE1, addr, lenght, nullptr);
    //qDebug() << "jcsbanksDMA::directDMAread2";
    return (*devComm)->read();
}

void jcsbanksDMA::directDMAwrite(quint32 addr, char *buf, int lenght)
{
    sendDMAcomand(0xE2, addr, lenght, buf);
}

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
    //QThread::msleep(15);
    if ( buf != nullptr)
    {
        memcpy((*devComm)->p_out_buff, buf, count);
        //subcount = count;
        (*devComm)->write( count);
    }

    //(*devComm)->write( 6+subcount);
}
