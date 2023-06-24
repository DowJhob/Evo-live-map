#include "ecuManager.h"

ecuManager::ecuManager(QWidget *parent, ecu *ECU) : QToolBar(parent), ECU(ECU)
{
//    ECU->test();
    connect(this, &ecuManager::ecuConnect,     ECU,  &ecu::_connect,             Qt::QueuedConnection);
    connect(ECU,  &ecu::ecuConnected,          this, &ecuManager::interfaceLock, Qt::QueuedConnection);

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

void ecuManager::interfaceLock(bool state)
{
    cpW.setDisabled(true);
    qDebug() << "=========== ecuManager::interfaceLock ================ state:" << state;
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
//        QMetaObject::invokeMethod(ECU, "connectECU");
        emit ecuConnect(true);
        qDebug() << "=========== ecuManager::startAction2 Start";
    }
    else
    {
        qDebug() << "=========== ecuManager::startAction Stop ================";
//        QMetaObject::invokeMethod(ECU, "disConnectECU");
        emit ecuConnect(false);
        a_start_action->setText("Start");
        lockReset( true);
        cpW.setDisabled(false);
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
    connect(&cpW.devManager,    &commDeviceManager::deviceSelected, ECU,   &ecu::setComDev);
    connect(&cpW._protoManager, &protoManager::protoSelected,       ECU,   &ecu::setDMAproto);
    connect(&cpW._protoManager, &protoManager::logRateChanged,      ECU,   &ecu::setLogRate);


    connect(&cpW.devManager,    &commDeviceManager::deviceSelected, this,   &ecuManager::deviceEvent);
    connect(&cpW._wbManager,    &wbManager::logReady,               &wbWgt, &gaugeWidget::display);

    cpW._protoManager.addProtos();   // костыль пока
    cpW._wbManager.fillSerial();
    cpW._wbManager.fillProto();
}
