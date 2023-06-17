#include "ecuManager.h"

ecuManager::ecuManager(QWidget *parent) : QToolBar(parent)
{
    qRegisterMetaType<Map>("Map");
    qRegisterMetaType<mapDefinition>("mapDefinition");
    qRegisterMetaType<offsetMemory>("offsetMemory");
    qRegisterMetaType<QVector<float>>("QVector<float>");

    ECU = new ecu();
    connect(ECU, &ecu::ecuConnected,          this, &ecuManager::ecuConnected,  Qt::QueuedConnection);       // Провайдим сигнал наружу
    connect(ECU, &ecu::ecuConnected,          this, &ecuManager::interfaceLock, Qt::QueuedConnection);

    connect(this, &ecuManager::updateRAM,      ECU, &ecu::updateRAM,  Qt::QueuedConnection);
    connect(this, &ecuManager::logRateChanged, ECU, &ecu::setLogRate, Qt::QueuedConnection);

    connect(ECU, &ecu::s_test, this, &ecuManager::s_test, Qt::QueuedConnection);

    ECU->test();

    //=============================================================================
    a_start_action = addAction( QIcon( ":ico/connect.png" ), "Start", this, &ecuManager::start_stop_Action);
    a_start_action->setDisabled(true);
    addSeparator();
    a_ramReset = addAction(QIcon( ":ico/Memory-Freer-icon.png" ), "RAM refresh", ECU, &ecu::RAMreset);
    a_ramReset->setDisabled(true);

    addSeparator();

    addWidget(&cpW);

    setConectionParamWidget();

    addSeparator();

    addWidget(&wbWgt);

    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
    setIconSize(QSize(200, 200));
}

ecuManager::~ecuManager()
{
    qDebug() << "~ecuManager";
}

void ecuManager::setUSBfilter(deviceNativeFilter *usbFilter)
{
    connect(usbFilter, &deviceNativeFilter::deviceEvent, &cpW.devManager, &commDeviceManager::deviceEvent);
}

void ecuManager::startLog()
{
    ECU->startLog();
}

void ecuManager::stopLog()
{
    ECU->stopLog();
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

void ecuManager::interfaceLock()
{
    cpW.setDisabled(true);
    qDebug() << "=========== ecuManager::connectECU ================" << devComm;
    lockReset( false);
    a_start_action->setText("Stop");
}

void ecuManager::deviceEvent(comm_device_interface *devComm)
{
    if(devComm == nullptr)
    {
        lockConnect(true);
        emit deviceEventLog("No interface", 0);
        return;
    }
    emit deviceEventLog(devComm->DeviceDesc + " / " + devComm->DeviceUniqueID, 0);
    if( devComm->info() )
        lockConnect(false);         // Показываем кнопки старт и сброс памяти
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
        cpW.setDisabled(false);
        emit ecuDisconnected();
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

void ecuManager::setConectionParamWidget()
{
    connect(&cpW._protoManager, &protoManager::protoSelected,       this,   &ecuManager::setProto);
    connect(&cpW._protoManager, &protoManager::logRateChanged,      this,   &ecuManager::logRateChanged);

    connect(&cpW.devManager,    &commDeviceManager::deviceSelected, this,   &ecuManager::setCommDevice);

    connect(&cpW.devManager,    &commDeviceManager::deviceSelected, this,   &ecuManager::deviceEvent);
    connect(&cpW._wbManager,    &wbManager::logReady,               &wbWgt, &gaugeWidget::display);

    cpW._protoManager.addProtos();   // костыль пока
    cpW._wbManager.fillSerial();
    cpW._wbManager.fillProto();
}
