#include "ecuManager.h"

ecuManager::ecuManager(QWidget *parent) : QToolBar(parent)
{
    qRegisterMetaType<Map>("Map");
    qRegisterMetaType<mapDefinition>("mapDefinition");
    qRegisterMetaType<abstractMemoryScaled>("abstractMemoryScaled");
    qRegisterMetaType<QVector<float>>("QVector<float>");

    qRegisterMetaType<char *>("char *");

    setProto(0);

    createUI();
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
    qDebug() << "=========== ecuManager::setCommDevice ================" << dev << this->ECUproto;
    devComm = dev;
    //connect((pollHelper*)devComm, &pollHelper::readyRead, this, &ecuManager::poll);
    this->ECUproto->setCommDev(&devComm);
    if (devComm != nullptr  )
    {
        p_in_buff = devComm->p_in_buff;
    }
    else
    {
        // все интерфесы отключены, сделай что нибудь!!!!

        //_dataLogger->stop();
    }
}

void ecuManager::setProto(DMA_proto *ECUproto)
{
    qDebug() << "=========== ecuManager::setProto ================"// << ECUproto
                ;
    this->ECUproto = ECUproto;
    connect(ECUproto, &DMA_proto::logReady, this, &ecuManager::logReady);
    //this->ECUproto->setCommDev(&devComm);
}

void ecuManager::setLogRate(uint logRate)
{
    _dataLogger->setLogRate(logRate);
}

void ecuManager::connectECU()
{
    qDebug() << "=========== ecuManager::getECUconnect ================" << devComm;
    if (!ECUproto->connect())
    {
        emit Log("failure get ECU connect " + QString::number( devComm->getBaudRate()));
        return ;
    }

    QByteArray a = ECUproto->directDMAread( 0xF52, 4);                        //читаем номер калибровки

    if ( a.isEmpty() )
    {
        emit Log("failure get ECU rom id");
        return;
    }
    QString romID = QString::number( qFromBigEndian<quint32>(a.data()), 16 );
    emit Log("romID: " + romID);

    //emit Log("CurrDir: " + QApplication::applicationDirPath());

    _ecu_definition = new ecu_definition;
    if (!ECUproto->getECU(romID))
    {
        (*ECUproto->devComm)->close();
        emit Log("xml not found");
        return;
    }

    lockReset( false);

a_start_action->setText("Stop");
    emit ecu_connected();
    // переберем все описания таблиц
    for ( Map *tab : qAsConst(ECUproto->_ecu_definition.RAMtables) )
    {
        emit create_table( getMap(tab) );
    }

    QMetaObject::invokeMethod(ECUproto, "startLog");
}

void ecuManager::disConnectECU()
{
    qDebug() << "=========== ecuManager::disConnectECU ================";
    QMetaObject::invokeMethod(ECUproto, "stopLog");
    QThread::msleep(1000);
    devComm->close();
}

void ecuManager::RAMreset()
{
    qDebug() << "ecuManager::RAMreset(addr::" << _ecu_definition->DEAD_var << ");";
    quint16 r = 0x0000;
    ECUproto->directDMAwrite(_ecu_definition->DEAD_var, (char*)&r, 2);
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

void ecuManager::startAction()
{
    if (a_start_action->text() == "Start")
    {
        qDebug() << "ecuManager::startButton_slot Start";
        connectECU();
    }
    else
    {
        disConnectECU();
        a_start_action->setText("Start");
        lockReset( true);

        emit disConnectECUaction();
    }
}

void ecuManager::updateRAM(abstractMemoryScaled memory)
{
    qDebug()<< "ecuManager::updateRAM" << memory.toHex(':');
    QMetaObject::invokeMethod(ECUproto, "directDMAwrite", Qt::QueuedConnection,
                              Q_ARG(quint32, memory.addr),
                              Q_ARG(char *, memory.data()),
                              Q_ARG(int, memory.size()));
    //ECUproto->directDMAwrite(memory.addr, memory.data(), memory.size());
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

void ecuManager::createUI()
{
    //=============================================================================
    a_start_action = addAction( QIcon( ":ico/connect.png" ), "Start", this, &ecuManager::startAction);
    a_start_action->setDisabled(true);
    addSeparator();
    a_ramReset = addAction(QIcon( ":ico/Memory-Freer-icon.png" ), "RAM refresh", this, &ecuManager::RAMreset);
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
