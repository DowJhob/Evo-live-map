#include "wblogger.h"

wbLogger::wbLogger()//:cdWB(cdWB)
{


    QThread *this_thread = new QThread();
    QObject::connect(this_thread, &QThread::started, this, [this](){
        pollTimer = new QTimer(this);
        connect(pollTimer, &QTimer::timeout, this, &wbLogger::poll);
    }
    );
    //connect(this_thread, &QThread::started, this, &controller::loop, Qt::QueuedConnection);
    QObject::connect(this, &wbLogger::destroyed, this_thread, &QThread::quit);            // Когда удалим объект остановим поток
    QObject::connect(this_thread, &QThread::finished, this_thread, &QThread::deleteLater);  // Когда остановим поток удалим его
    moveToThread(this_thread);
    this_thread->start();
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
        qDebug() << "=========== wbLogger::cdWB->openWB ================" << cdWB;
        //        cdWB->openWB(19600);
        //cdWB->connectWB(19600);
        //        start();
    }
    else
    {
        // все интерфесы отключены, сделай что нибудь!!!!

        stop();
    }
}

void wbLogger::setProto(int proto)
{
    this->_wbProto = proto;
}

void wbLogger::start_stop(bool start)
{
    qDebug() << "=========== wbLogger::start_stop ================" << cdWB;
    if(cdWB->isClosed())
        cdWB->openWB(19200);
    if(start)
        pollTimer->start(50);
    else
        pollTimer->stop();
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
    QByteArray a = cdWB->readWB();
    if(a.size() > 0)
    {
        logReady(handleWB(_wbProto, a));
    }
}
