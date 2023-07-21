#include "pollhelper.h"

pollHelper::pollHelper() : DMA_proto()
{

}

void pollHelper::setLogRate(int rate)
{
pollTimer->setInterval(rate);
}

void pollHelper::init()
{
    pollTimer = new QTimer();
    pollTimer->setInterval(50);
    QObject::connect(pollTimer, &QTimer::timeout, this, &pollHelper::poll, Qt::DirectConnection); // Тут Qt::DirectConnection важно, что бы это выполнялось в потоке ecu
}

void pollHelper::startLog(ramMUT *ramMut)
{
    qDebug()<<"=========== pollHelper::startLog ================" << QThread::currentThread();
    DMA_proto::startLog(ramMut);
    if(pollTimer == nullptr)
        init();
    pollTimer->start();
}

void pollHelper::stopLog()
{
    pollTimer->stop();
}
