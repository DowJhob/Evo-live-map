#include "ecuManager.h"

ecuManager::ecuManager(QWidget *parent) : QToolBar(parent)
{
    qRegisterMetaType<Map>("Map");
    qRegisterMetaType<mapDefinition>("mapDefinition");
    qRegisterMetaType<offsetMemory>("abstractMemoryScaled");
    qRegisterMetaType<QVector<float>>("QVector<float>");

    //qRegisterMetaType<char *>("char *");

    ECU = new ecu();
    connect(ECU, &ecu::ecuConnected, this, &ecuManager::ecuConnected,  Qt::QueuedConnection);
    connect(ECU, &ecu::ecuConnected, this, &ecuManager::_ecuConnected, Qt::QueuedConnection);
    connect(ECU, &ecu::createMap,    this, &ecuManager::createMap,     Qt::QueuedConnection);

    connect(ECU, &ecu::logReady,     this, &ecuManager::logReady,      Qt::QueuedConnection);

    connect(this, &ecuManager::updateRAM,       ECU, &ecu::updateRAM,  Qt::QueuedConnection);

    //=============================================================================
    a_start_action = addAction( QIcon( ":ico/connect.png" ), "Start", this, &ecuManager::startAction);
    a_start_action->setDisabled(true);
    addSeparator();
    a_ramReset = addAction(QIcon( ":ico/Memory-Freer-icon.png" ), "RAM refresh", ECU, &ecu::RAMreset);
    a_ramReset->setDisabled(true);

    addSeparator();

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    setIconSize(QSize(200, 200));
}

ecuManager::~ecuManager()
{
    qDebug() << "~ecuManager";
}

void ecuManager::setCommDevice(comm_device_interface *dev)
{
    qDebug() << "=========== ecuManager::setCommDevice ================" << dev << ECUproto;
    devComm = dev;
    ECUproto->setCommDev(&dev);
    ECU->setComDev(dev);
}

void ecuManager::setProto(DMA_proto *ECUproto)
{
    qDebug() << "=========== ecuManager::setProto ================";
    this->ECUproto = ECUproto;
    ECU->setDMAproto(ECUproto);
    connect(ECUproto, &DMA_proto::logReady, this, &ecuManager::logReady, Qt::QueuedConnection);
}

void ecuManager::setLogRate(uint logRate)
{
    //_dataLogger->setLogRate(logRate);
}

void ecuManager::_ecuConnected()
{
    qDebug() << "=========== ecuManager::connectECU ================" << devComm;
    lockReset( false);
    a_start_action->setText("Stop");
    //emit ecuConnected_();
}

void ecuManager::startAction()
{
    if (a_start_action->text() == "Start")
    {
        qDebug() << "=========== ecuManager::startAction Start";
        QMetaObject::invokeMethod(ECU, "connectECU");
        qDebug() << "=========== ecuManager::startAction2 Start";
    }
    else
    {
        qDebug() << "=========== ecuManager::startAction Stop ================";
        QMetaObject::invokeMethod(ECU, "disConnectECU");
        a_start_action->setText("Start");
        lockReset( true);
        emit ecuDisconnect();
    }
}

void ecuManager::lockConnect(bool lockFlag)
{
    a_start_action->setDisabled(lockFlag);
    if(lockFlag)
    {
        a_ramReset->setDisabled(lockFlag);
    }
}

void ecuManager::lockReset(bool lockFlag)
{
    a_ramReset->setDisabled(lockFlag);
}
