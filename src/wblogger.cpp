#include "wblogger.h"

wbLogger::wbLogger()//:cdWB(cdWB)
{

    pollTimer = new QTimer(this);
    connect(pollTimer, &QTimer::timeout, this, &wbLogger::poll);

    QThread *this_thread = new QThread();
    QObject::connect(this_thread, &QThread::started, this, [this](){
        pollTimer = new QTimer(this);
        connect(pollTimer, &QTimer::timeout, this, &wbLogger::poll);
    }
    );
    //connect(this_thread, &QThread::started, this, &controller::loop, Qt::QueuedConnection);
    QObject::connect(this, &wbLogger::destroyed, this_thread, &QThread::quit);            // Когда удалим объект остановим поток
    QObject::connect(this_thread, &QThread::finished, this_thread, &QThread::deleteLater);  // Когда остановим поток удалим его
    //moveToThread(this_thread);
    //this_thread->start();
}

void wbLogger::setWB(commDeviceWB *cdWB)
{
    qDebug() << "=========== wbLogger::setWB ================" << cdWB;
    // сначала закроем предыдущий если он не отключен, а просто сменился
    if(this->cdWB != nullptr)
    {
        this->cdWB->closeWB();
    }

    this->cdWB = cdWB;
    if (cdWB != nullptr  )
    {
        connect(cdWB, &commDeviceWB::readyRead, this, [this](QByteArray a){
            emit logReady(_wbProto->handleWB(a));
            qDebug() << "=========== wbLogger::readyRead ================";
        });
    }
    else
    {
        // все интерфесы отключены, сделай что нибудь!!!!

        stop();
    }
}

void wbLogger::setProto(wbProto *proto)
{
    qDebug() << "=========== wbLogger::setProto ================" << proto;
    this->_wbProto = proto;
}

void wbLogger::start_stop(bool start)
{
    qDebug() << "=========== wbLogger::start_stop ================" << cdWB;
    if(start)
    {
        //pollTimer->start(50);
//        if(cdWB->isClosed())
//            cdWB->openWB(_wbProto->baudRate);
    }
    else
        //pollTimer->stop()
//        if(!cdWB->isClosed())
//            cdWB->closeWB()
        ;
}

void wbLogger::start()
{
    pollTimer->start();
}

void wbLogger::stop()
{
    pollTimer->stop();
}

void wbLogger::poll()
{
    //qDebug() << "=========== wbLogger::poll ================";
    QByteArray a = cdWB->readWB();
    if(a.size() > 0)
    {
        emit logReady(_wbProto->handleWB(a));
    }
    else
        emit logReady("ERR");
}
