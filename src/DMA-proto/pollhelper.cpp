#include "pollhelper.h"

pollHelper::pollHelper() //: QObject(parent)
{
    QThread *this_thread = new QThread();
    QObject::connect(this_thread, &QThread::started, this, [this](){
        pollTimer = new QTimer(this);
        pollTimer->setInterval(50);
        QObject::connect(pollTimer, &QTimer::timeout, this, &pollHelper::poll);
    }
    );
    //    //connect(this_thread, &QThread::started, this, &controller::loop, Qt::QueuedConnection);
    QObject::connect(this, &QObject::destroyed, this_thread, &QThread::quit);            // Когда удалим объект остановим поток
    QObject::connect(this_thread, &QThread::finished, this_thread, &QThread::deleteLater);  // Когда остановим поток удалим его
    moveToThread(this_thread);
    this_thread->start();
}

void pollHelper::startLog()
{
    //   qDebug()<<"=========== pollHelper::startPoll ================";
    int rateHz = 50;
    pollTimer->start(rateHz);
}

void pollHelper::stopLog()
{
    pollTimer->stop();
}
