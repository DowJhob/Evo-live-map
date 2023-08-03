#include "wb.h"

WB::WB()
{
    thread = new QThread();
    //connect(this_thread, &QThread::started, this, &controller::loop, Qt::QueuedConnection);
    connect(this, &WB::destroyed, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    moveToThread(thread);
    thread->start();
    qDebug() << "=========== WB:: ================ QThread:" << thread;

//    connect(this, &WB::_poll, this, &WB::poll);
}

void WB::setWBDev(commDeviceWB *_wbdevComm)
{
    wbdevComm = _wbdevComm;
    if (wbdevComm == nullptr  )
    {
        // все интерфесы отключены, сделай что нибудь!!!!

//        stopFlag = true;
    }
}

void WB::setWBproto(wbProto *_protoWB)
{
    protoWB = _protoWB;
    if (protoWB == nullptr  )
    {
        // это не должно происходить вообще
    }
}

void WB::start(bool state)
{
    qDebug() << "==================== WB::start ==================================" << state;
    if(state)
        startLog();
    else
        stopLog();
}

void WB::startLog()
{
    qDebug() << "==================== WB::startLog ==================================" << wbdevComm << protoWB->baudRate;
    if(wbdevComm->openWB(protoWB->baudRate))
    {
        wbdevComm->startLog(protoWB->baudRate);
    }
}

void WB::stopLog()
{
    qDebug() << "==================== WB::stopLog ==================================" << wbdevComm << protoWB->baudRate;

    wbdevComm->stopLog();
}

//void WB::init()
//{
//    pollTimer = new QTimer(this);
//    pollTimer->setInterval(50);
//    QObject::connect(pollTimer, &QTimer::timeout, this, &WB::poll, Qt::DirectConnection);
//}

//void WB::poll()
//{
//    qDebug() << "WB::poll" ;
//    QByteArray a = wbdevComm->readWB();
//    if (a.size() > 0)
//        emit wbdevComm->readyRead(a);
//    if(!stopFlag)
//        emit _poll();
//    else
//        stopFlag = false;
//}
