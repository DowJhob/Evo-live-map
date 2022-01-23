#include "ecuManager.h"

ecuManager::ecuManager(QWidget *parent) : QToolBar(parent)
{
    qRegisterMetaType<Map>("Map");
    qRegisterMetaType<mapDefinition>("mapDefinition");
    qRegisterMetaType<abstractMemoryScaled>("abstractMemoryScaled");
    qRegisterMetaType<QVector<float>>("QVector<float>");

    //qRegisterMetaType<char *>("char *");

    ECUdef = new ecuDefinition();
    connect(ECUdef, &ecuDefinition::ecu_connected, this, &ecuManager::ecu_connected);
    connect(ECUdef, &ecuDefinition::ecu_connected, this, &ecuManager::ecuConnected);
    connect(ECUdef, &ecuDefinition::create_table, this, &ecuManager::create_table);

    connect(ECUdef, &ecuDefinition::logReady, this, &ecuManager::logReady);

    connect(this, &ecuManager::updateRAM, ECUdef, &ecuDefinition::updateRAM, Qt::QueuedConnection);

    //=============================================================================
    a_start_action = addAction( QIcon( ":ico/connect.png" ), "Start", this, &ecuManager::startAction);
    a_start_action->setDisabled(true);
    addSeparator();
    a_ramReset = addAction(QIcon( ":ico/Memory-Freer-icon.png" ), "RAM refresh", ECUdef, &ecuDefinition::RAMreset);
    a_ramReset->setDisabled(true);

    addSeparator();

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    setIconSize(QSize(200, 200));
}

ecuManager::~ecuManager()
{
    //    getECUdisconnect();

    //    if (ECUproto != nullptr)
    //        ECUproto->deleteLater();


    //    if (devComm != nullptr)
    //        devComm->deleteLater();
    qDebug() << "~ecuManager";
}

void ecuManager::setCommDevice(comm_device_interface *dev)
{
    qDebug() << "=========== ecuManager::setCommDevice ================" << dev << ECUproto;
    devComm = dev;
    ECUproto->setCommDev(&dev);
    ECUdef->setComDev(dev);
}

void ecuManager::setProto(DMA_proto *ECUproto)
{
    qDebug() << "=========== ecuManager::setProto ================";
    this->ECUproto = ECUproto;
    ECUdef->setDMAproto(ECUproto);
    connect(ECUproto, &DMA_proto::logReady, this, &ecuManager::logReady, Qt::QueuedConnection);
}

void ecuManager::setLogRate(uint logRate)
{
    //_dataLogger->setLogRate(logRate);
}

void ecuManager::ecuConnected()
{
    qDebug() << "=========== ecuManager::connectECU ================" << devComm;
    lockReset( false);
    a_start_action->setText("Stop");
    emit ecu_connected();
}

void ecuManager::startAction()
{
    if (a_start_action->text() == "Start")
    {
        qDebug() << "=========== ecuManager::startAction Start";
        QMetaObject::invokeMethod(ECUdef, "connectECU");
        qDebug() << "=========== ecuManager::startAction2 Start";
    }
    else
    {
        qDebug() << "=========== ecuManager::startAction Stop ================";
        QMetaObject::invokeMethod(ECUdef, "disConnectECU");
        a_start_action->setText("Start");
        lockReset( true);
        emit disConnectECUaction();
    }
}

//void ecuManager::updateRAM(abstractMemoryScaled memory)
//{
//    qDebug()<< "ecuManager::updateRAM" << memory.toHex(':');
//    QMetaObject::invokeMethod(&ECUdef, "updateRAM", Qt::QueuedConnection,
//                              Q_ARG(abstractMemoryScaled, memory));
//    //    ECUproto->directDMAwrite(memory);
//}

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
