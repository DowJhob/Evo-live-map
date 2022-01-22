#include "ecuManager.h"

ecuManager::ecuManager(QWidget *parent) : QToolBar(parent)
{
    qRegisterMetaType<Map>("Map");
    qRegisterMetaType<mapDefinition>("mapDefinition");
    qRegisterMetaType<abstractMemoryScaled>("abstractMemoryScaled");
    qRegisterMetaType<QVector<float>>("QVector<float>");

    //qRegisterMetaType<char *>("char *");

    connect(&ECUdef, &ecuDefinition::ecu_connected, this, &ecuManager::ecu_connected);
    connect(&ECUdef, &ecuDefinition::ecu_connected, this, &ecuManager::ecuConnected);
    connect(&ECUdef, &ecuDefinition::create_table, this, &ecuManager::create_table);

    connect(&ECUdef, &ecuDefinition::logReady, this, &ecuManager::logReady);

    //setProto(0);

    //=============================================================================
    a_start_action = addAction( QIcon( ":ico/connect.png" ), "Start", this, &ecuManager::startAction);
    a_start_action->setDisabled(true);
    addSeparator();
    a_ramReset = addAction(QIcon( ":ico/Memory-Freer-icon.png" ), "RAM refresh", &ECUdef, &ecuDefinition::RAMreset);
    a_ramReset->setDisabled(true);
    //a_logger = addAction( "Start", this, &MainWindow::logger_slot);
    //a_logger->setDisabled(true);

    addSeparator();
    //QWidget* empty = new QWidget(this);
    //empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    //addWidget(empty);
    //debug_action = ui->toolBar->addAction(QIcon( ":ico/screwdriver.png" ), "Debug", this, &MainWindow::debugButton_slot);

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
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
    ECUdef.setComDev(dev);
}

void ecuManager::setProto(DMA_proto *ECUproto)
{
    qDebug() << "=========== ecuManager::setProto ================";
    this->ECUproto = ECUproto;
    ECUdef.setDMAproto(ECUproto);
    connect(ECUproto, &DMA_proto::logReady, this, &ecuManager::logReady);
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
    emit setRamMut(ECUdef.RAM_MUT);
}

void ecuManager::startAction()
{
    if (a_start_action->text() == "Start")
    {
        qDebug() << "=========== ecuManager::startAction Start";
        QMetaObject::invokeMethod(&ECUdef, "connectECU");
        qDebug() << "=========== ecuManager::startAction2 Start";
    }
    else
    {
        qDebug() << "=========== ecuManager::startAction Stop ================";
        QMetaObject::invokeMethod(&ECUdef, "disConnectECU");
        a_start_action->setText("Start");
        lockReset( true);
        emit disConnectECUaction();
    }
}

mapDefinition *ecuManager::getMap(Map *declMap)
{
    //qDebug()<<"ECU_interface::getMap"<<declMap->Name;
    //if(declMap->rom_scaling._storagetype == Storagetype::undef || declMap->rom_scaling._storagetype == Storagetype::bloblist)
    //    return &mapDefinition();
    mapDefinition *defMap = new mapDefinition;
    defMap->declMap = declMap;
    if(declMap->X_axis.addr != 0)
        defMap->X_axis = ECUproto->directDMAread(declMap->X_axis.addr, declMap->X_axis.byteSize());   // читаем оси
    if(declMap->Y_axis.addr != 0)
        defMap->Y_axis = ECUproto->directDMAread(declMap->Y_axis.addr, declMap->Y_axis.byteSize());
    defMap->Map = ECUproto->directDMAread(declMap->addr, declMap->byteSize());
    //emit gettedMap(defMap);
    return defMap;
}

void ecuManager::updateRAM(abstractMemoryScaled memory)
{
    qDebug()<< "ecuManager::updateRAM" << memory.toHex(':');
    QMetaObject::invokeMethod(ECUproto, "directDMAwrite", Qt::QueuedConnection,
                              Q_ARG(abstractMemoryScaled, memory));
    //    ECUproto->directDMAwrite(memory);
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
