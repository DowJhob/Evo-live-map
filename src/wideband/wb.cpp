#include "wb.h"


WB::WB()
{
    QThread *this_thread = new QThread();
    //connect(this_thread, &QThread::started, this, &controller::loop, Qt::QueuedConnection);
    connect(this, &WB::destroyed, this_thread, &QThread::quit);
    connect(this_thread, &QThread::finished, this_thread, &QThread::deleteLater);
    moveToThread(this_thread);
    this_thread->start();
    qDebug() << "=========== WB:: ================ QThread:" << this_thread;


    connect(this, &WB::_poll, this, &WB::poll);
}

void WB::setWBDev(commDeviceWB *_wbdevComm)
{
    wbdevComm = _wbdevComm;
    if (wbdevComm == nullptr  )
    {
        // все интерфесы отключены, сделай что нибудь!!!!
        stopLog();
    }
}

void WB::setWBproto(wbProto *_protoWB)
{
    protoWB = _protoWB;
    if (protoWB == nullptr  )
    {
        // все интерфесы отключены, сделай что нибудь!!!!
        // stopLog();
    }
}

void WB::start(bool state)
{
    if(state)
        startLog();
    else
        stopLog();
}

void WB::startLog()
{
    if(wbdevComm->openWB(protoWB->baudRate))
    {
        poll();
    }
}

void WB::stopLog()
{
    wbdevComm->closeWB();
}

void WB::init()
{
    pollTimer = new QTimer(this);
    pollTimer->setInterval(50);
    QObject::connect(pollTimer, &QTimer::timeout, this, &WB::poll, Qt::DirectConnection);

}

void WB::poll()
{
    qDebug() << "WB::poll" ;
    QByteArray a = wbdevComm->readWB();
    if (a.size() > 0)
        emit wbdevComm->readyRead(a);
    emit _poll();
}
