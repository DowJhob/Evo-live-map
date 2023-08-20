#include "pollhelper.h"

pollHelper::pollHelper(DMA_proto *parent_proto) : QObject(parent_proto), parent_proto(parent_proto)
{
    //        pollTimer = new QTimer();
    //        pollTimer->setInterval(50);
    //        connect(pollTimer, &QTimer::timeout, this, &pollHelper::poll/*, Qt::DirectConnection*/); // Тут Qt::DirectConnection важно, что бы это выполнялось в потоке ecu
}

void pollHelper::setLogRate(int rate)
{
    pollTimer->setInterval(rate);
}

void pollHelper::init()
{
    qDebug()<<"=========== pollHelper::init ================ QThread::currentThread()" << thread();
    pollTimer = new QTimer();
    pollTimer->setInterval(50);
    pollTimer->setSingleShot(true);
    connect(pollTimer, &QTimer::timeout, this, &pollHelper::poll2/*, Qt::DirectConnection*/);
}

void pollHelper::startLog2()
{
    //    qDebug()<<"=========== pollHelper::startLog2 ================" << thread();
    if(pollTimer == nullptr)
        init();
    pollTimer->start();
}

void pollHelper::stopLog()
{
    if(pollTimer != nullptr)
        pollTimer->stop();
}

void pollHelper::poll2()
{
    //    qDebug() << "pollHelper::poll" << parent_proto->ramMut->byteSize << parent_proto->ramMut->addr;
    offsetMemory a = parent_proto->indirectDMAread(parent_proto->ramMut->addr, parent_proto->ramMut->byteSize);
    for(int i = 0; i < parent_proto->ramMut->size(); i++)
    {
        parent_proto->ramMut->scaledValue[i] = a.toFloatOffset( &(*parent_proto->ramMut)[i].scaling, parent_proto->ramMut->at(i).offset );
    }

    emit parent_proto->logReady(parent_proto->ramMut->scaledValue);
    //        pollTimer->singleShot(10, this, &pollHelper::poll2);
    pollTimer->start();
}
