#include "ecuManager.h"

ecuManagerWidget::ecuManagerWidget(QWidget *parent, ecu *ECU) : QToolBar(parent), ECU(ECU)
{
//    ECU->test();
    connect(this, &ecuManagerWidget::ecuConnect,     ECU,  &ecu::connectDMA,             Qt::QueuedConnection);
    connect(ECU,  &ecu::ecuConnected,          this, &ecuManagerWidget::interfaceLock, Qt::QueuedConnection);

    //=============================================================================
    a_start_action = addAction( QIcon( ":ico/connect.png" ), "Start", this, &ecuManagerWidget::start_stop_Action);
    a_start_action->setDisabled(true);
    addSeparator();
    a_ramReset = addAction(QIcon( ":ico/Memory-Freer-icon.png" ), "RAM refresh", ECU, &ecu::RAMreset);
    a_ramReset->setDisabled(true);

    addSeparator();

    addWidget(&cpW);


    cpW._protoManager.ecu_thread = ECU->this_thread;                                             //
    setConectionParamWidget();

    addSeparator();

    addWidget(&wbWgt);

    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
    setIconSize(QSize(200, 200));
}

ecuManagerWidget::~ecuManagerWidget()
{
    //qDebug() << "~ecuManager";
}

void ecuManagerWidget::setUSBfilter(deviceNativeFilter *usbFilter)
{
    connect(usbFilter, &deviceNativeFilter::deviceEvent, &cpW.devManager, &commDeviceManager::deviceEvent);
}

void ecuManagerWidget::interfaceLock(bool state)
{
    cpW.setDisabled(true);
    //qDebug() << "=========== ecuManager::interfaceLock ================ state:" << state;
    lockReset( false);
    a_start_action->setText("Stop");
}

void ecuManagerWidget::deviceEvent(comm_device_interface *devComm)
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

void ecuManagerWidget::start_stop_Action()
{
    if (a_start_action->text() == "Start")
    {
        //qDebug() << "=========== ecuManager::startAction Start";
//        QMetaObject::invokeMethod(ECU, "connectECU");
        emit ecuConnect(true);
        //qDebug() << "=========== ecuManager::startAction2 Start";
    }
    else
    {
        //qDebug() << "=========== ecuManager::startAction Stop ================";
//        QMetaObject::invokeMethod(ECU, "disConnectECU");
        emit ecuConnect(false);
        a_start_action->setText("Start");
        lockReset( true);
        cpW.setDisabled(false);
    }
}

void ecuManagerWidget::lockConnect(bool lockFlag)
{
    a_start_action->setDisabled(lockFlag);
    if(lockFlag)
    {
        a_ramReset->setDisabled(lockFlag);
    }
}

void ecuManagerWidget::lockReset(bool lockFlag)
{
    a_ramReset->setDisabled(lockFlag);
}

void ecuManagerWidget::setConectionParamWidget()
{
    connect(&cpW.devManager,       &commDeviceManager::deviceSelected, ECU,   &ecu::setComDev);
    connect(&cpW._ecuModelManager, &ecuModelManager::modelSelected,    ECU,   &ecu::setECUmodel);

    connect(&cpW._protoManager, &protoManager::protoSelected,       ECU,   &ecu::setDMAproto);
    connect(&cpW._protoManager, &protoManager::logRateChanged,      ECU,   &ecu::setLogRate);

    connect(&cpW.devManager,    &commDeviceManager::deviceSelected, this,   &ecuManagerWidget::deviceEvent);
    connect(&cpW._wbManager,    &wbManager::logReady,               &wbWgt, &gaugeWidget::display);

    // Заполняем после подключения, тогда при добавлении буду сигналы
    cpW._protoManager.addProtos();
    cpW._ecuModelManager.addModels();
    cpW._wbManager.fillSerial();
    cpW._wbManager.fillProto();
}
