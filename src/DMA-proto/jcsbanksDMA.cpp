#include "jcsbanksDMA.h"

jcsbanksDMA::jcsbanksDMA(p_comm_device_interface *p_devComm) : /*jcsbanksDMA()*/ DMA_proto(p_devComm)
{
    name = "Custom DMA proto by jcsbanks";
    type = DMA_ProtoType::jcsbanks;
    poller = new pollHelper(this);
    // qDebug() << "jcsbanksDMA(p_comm_device_interface *p_devComm)" << this->p_devComm;
}

jcsbanksDMA::~jcsbanksDMA()
{
    poller->deleteLater();
    qDebug() << "~jcsbanksDMA";
}




QByteArray jcsbanksDMA::indirectDMAread(quint32 addr, int lenght)
{
    //qDebug() << "jcsbanksDMA::directDMAread";
    sendDMAcomand(0xE0, addr, lenght, nullptr);
    //QByteArray a = (*devComm)->read(lenght);
    //qDebug() << "jcsbanksDMA::directDMAread" << a.toHex(':');
    //return a;
    return (*p_devComm)->read(lenght);
}

QByteArray jcsbanksDMA::directDMAread(quint32 addr, int lenght)
{
    //qDebug() << "jcsbanksDMA::directDMAread";
    sendDMAcomand(0xE1, addr, lenght, nullptr);
    //qDebug() << "jcsbanksDMA::directDMAread2";
    return (*p_devComm)->read(lenght);
}

void jcsbanksDMA::directDMAwrite(quint32 addr, char* buf, int lenght)
{
    //qDebug()<<"=========== jcsbanksDMA::directDMAwrite ================" << QString::number(addr, 16) << QByteArray(buf, 1).toHex(':');
    sendDMAcomand(0xE2, addr, lenght, buf);
}

void jcsbanksDMA::startLog(ramMUT *_ramMut)
{
//    connect(this, &jcsbanksDMA::_poll, this, &jcsbanksDMA::poll/*, Qt::DirectConnection*/);
//    qDebug()<<"=========== jcsbanksDMA::startLog ================" << thread();
    DMA_proto::startLog(_ramMut);

//    int curBaudRate = (*p_devComm)->baudRate ;

//    int timeForSendCommand = (1000 * 7)/(curBaudRate/bitsInLineByte) + delay_after_command;

//    int timesNeededForByteBlock = (1000 *ramMut->byteSize)/(curBaudRate/bitsInLineByte);

//    int minPollTime = timeForSendCommand + timesNeededForByteBlock + mcuResponseTime + 1;

//    qDebug()<<"=========== jcsbanksDMA::startLog ================ minPollTime" << minPollTime;

//    (*p_devComm)->_readTimeout = minPollTime;

//    poller->startLog(minPollTime);
    poller->startLog2();
}

void jcsbanksDMA::startLog()
{
    poller->startLog2();
}

void jcsbanksDMA::stopLog()
{
    //qDebug()<<"=========== jcsbanksDMA::stopLog ================";
    poller->stopLog();
    //pollTimer->stop();
}

void jcsbanksDMA::RAMreset(quint32 var1, quint16 var2)
{
//    stopLog();                                      // Запоминать состояние логгера не нужно, на этом этапе он всегда работает иначе эку вывалиться из мут
    //    qDebug() << "jcsbanksDMA::RAMreset(addr::" << parent->ecuDef.ramMut.DEAD_var << ");";
    //        quint16 r = 0x0000;
    //        directDMAwrite(ecuDef.ramMut.DEAD_var, (char*)&r, 2);
    directDMAwrite(var1, (char*)&var2, 2);
//    startLog();
}

void jcsbanksDMA::updateRAM(offsetMemory memory)
{
//    stopLog();
    qDebug()<< "jcsbanksDMA::updateRAM" << memory.toHex(':');
    directDMAwrite(memory.addr, memory.data(), memory.size());
//    startLog();
}

void jcsbanksDMA::setLogRate(int freqRate)
{
    poller->setLogRate(1/freqRate);
}

void jcsbanksDMA::poll()
{
    offsetMemory a = indirectDMAread(ramMut->addr, ramMut->byteSize);
    for(int i = 0; i < ramMut->size(); i++)
    {
        ramMut->scaledValue[i] = a.toFloatOffset( &(*ramMut)[i].scaling, ramMut->at(i).offset );
    }

    emit logReady(ramMut->scaledValue);

    //qDebug() << "jcsbanksDMA::poll" << ramMut->byteSize << QString::number(ramMut->addr, 16) << a;
    emit _poll();
}

void jcsbanksDMA::sendDMAcomand(char command, unsigned long addr, unsigned long count, char *buf)
{
    //int subcount =0;
    (*p_devComm)->p_out_buff[0] = command;
    (*p_devComm)->write( 1 );
    QThread::msleep(delay_after_command);
    (*p_devComm)->p_out_buff[0] = (addr & 0xFF000000) >> 24;
    (*p_devComm)->p_out_buff[1] = (addr & 0xFF0000) >> 16;
    (*p_devComm)->p_out_buff[2] = (addr & 0xFF00) >> 8;
    (*p_devComm)->p_out_buff[3] = (addr & 0xFF);
    (*p_devComm)->p_out_buff[4] = (count & 0xFF00) >> 8;
    (*p_devComm)->p_out_buff[5] = (count & 0xFF);
    (*p_devComm)->write( 6 );
    if ( buf != nullptr)
    {
        //QThread::msleep(10);
        memcpy((*p_devComm)->p_out_buff, buf, count);
//        qDebug() << "jcsbanksDMA::sendDMAcomand" << QString::number(command, 16) << "addr" << QString::number(addr, 16) << "size" << count;
        (*p_devComm)->write( count);
    }

    //(*devComm)->write( 6+subcount);
}
