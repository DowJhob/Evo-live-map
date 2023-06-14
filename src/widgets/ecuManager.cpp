#include "ecuManager.h"

ecuManager::ecuManager(QWidget *parent) : QToolBar(parent)
{
    qRegisterMetaType<Map>("Map");
    qRegisterMetaType<mapDefinition>("mapDefinition");
    qRegisterMetaType<offsetMemory>("offsetMemory");
    qRegisterMetaType<QVector<float>>("QVector<float>");

    //qRegisterMetaType<char *>("char *");

    ECU = new ecu();
    connect(ECU, &ecu::ecuConnected,          this, &ecuManager::ecuConnected,  Qt::QueuedConnection);       // Провайдим сигнал наружу
    connect(ECU, &ecu::ecuConnected,          this, &ecuManager::_ecuConnected, Qt::QueuedConnection);
//    connect(ECU, &ecu::createMap,             this, &ecuManager::createMap,     Qt::QueuedConnection);

    //connect(ECU, &ecu::logReady,              this, &ecuManager::logReady//,      //Qt::QueuedConnection
    //        );

    connect(this, &ecuManager::updateRAM,      ECU, &ecu::updateRAM,  Qt::QueuedConnection);
    connect(this, &ecuManager::logRateChanged, ECU, &ecu::setLogRate, Qt::QueuedConnection);
    //connect(this, &ecuManager::log,       ECU, &ecu::log,  Qt::QueuedConnection);

    //=============================================================================
    a_start_action = addAction( QIcon( ":ico/connect.png" ), "Start", this, &ecuManager::start_stop_Action);
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

void ecuManager::setProto(DMA_proto *_ECUproto)
{
    qDebug() << "=========== ecuManager::setProto ================";
    ECUproto = _ECUproto;
    ECU->setDMAproto(_ECUproto);
    connect(_ECUproto, &DMA_proto::logReady, this, &ecuManager::logReady, Qt::QueuedConnection);
}

void ecuManager::_ecuConnected()
{
    qDebug() << "=========== ecuManager::connectECU ================" << devComm;
    lockReset( false);
    a_start_action->setText("Stop");

    // переберем все описания таблиц
    for ( Map *tab : qAsConst(ECU->ecuDef.RAMtables) )
    {
        emit createMap( ECU->getMap(tab) );
    }


    // переберем все патчи
    emit addPatches( &ECU->ecuDef.patches );











    ECU->startLog();



    //emit ecuConnected_();
}

void ecuManager::start_stop_Action()
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

void ecuManager::applyPatch(bloblistPatch *patch)
{
// не тут, надоже это все в файле делать!
}

void ecuManager::applyOriginal(bloblistPatch *patch)
{

}
