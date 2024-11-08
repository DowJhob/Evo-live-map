#include "ecuManager.h"

ecuManagerWidget::ecuManagerWidget(QWidget *parent, ecu *ECU) : QToolBar(parent), ECU(ECU)
{
    //    ECU->test();


    connect(this, &ecuManagerWidget::ecuConnect,     ECU, &ecu::connectDMA,                Qt::QueuedConnection);
    connect(ECU,  &ecu::ecuConnected,               this, &ecuManagerWidget::ECUconnected, Qt::QueuedConnection);

    //=============================================================================
    a_start_action = addAction( QIcon( ":ico/connect.png" ), "Start", this, &ecuManagerWidget::start_stop_Action);
    a_start_action->setDisabled(true);
    addSeparator();
    a_ramReset = addAction(QIcon( ":ico/Memory-Freer-icon.png" ), "RAM refresh", ECU, &ecu::RAMreset);
    a_ramReset->setDisabled(true);

    addSeparator();

    addWidget(&cpW);

    // cpW._ecuModelManager.ecu_thread = ECU->readThread;

    addSeparator();

    addWidget(&wbWgt);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    setIconSize(QSize(200, 200));

    cpW.deviceLostState();
}

ecuManagerWidget::~ecuManagerWidget()
{
    //qDebug() << "~ecuManager";
}

void ecuManagerWidget::setUSBfilter(deviceNativeFilter *usbFilter)
{
    connect(usbFilter, &deviceNativeFilter::deviceEvent, &cpW.devManager, &commDeviceManagerWidget::deviceEvent);
}

void ecuManagerWidget::ECUconnected(bool state)
{
    //qDebug() << "=========== ecuManager::interfaceLock ================ state:" << state;
    //    cpW.setEnabledECUcomm(!state);
    a_ramReset->setDisabled(!state);
    if(state)
    {
        a_start_action->setText("Stop");
        cpW.connectedState();
    }
    else
    {
        a_start_action->setText("Start");
        cpW.devicePresentState();
    }
}

void ecuManagerWidget::setComDev(comm_device_interface *devComm)
{
    if(devComm == nullptr)
    {
        //        cpW.setEnabledECUcomm(false);
        cpW.deviceLostState();
        a_start_action->setDisabled(true);
        a_ramReset->setDisabled(true);
        emit deviceEventLog("No interface", 0);
        return;
    }
    emit deviceEventLog(devComm->DeviceDesc + " / " + devComm->DeviceUniqueID, 0);

    cpW.devicePresentState();
    a_start_action->setDisabled(false);
}

void ecuManagerWidget::start_stop_Action()
{
    if (a_start_action->text() == "Start")
    {
        emit ecuConnect(true);
    }
    else
    {
        emit ecuConnect(false);
    }
}

void ecuManagerWidget::setConectionParamWidget()
{
    connect(&cpW.devManager,       &commDeviceManagerWidget::deviceSelected, ECU,   &ecu::setComDev);
    connect(&cpW.devManager,       &commDeviceManagerWidget::deviceSelected, this,  &ecuManagerWidget::setComDev);
    connect(&cpW.devManager,       &commDeviceManagerWidget::deviceHasLeft,  ECU,   &ecu::deviceHasLeft);









    connect(&cpW._ecuModelManager, &ecuModelManager::modelSelected,    ECU,   &ecu::setECUmodel);

    connect(&cpW._ecuModelManager, &ecuModelManager::protoSelected,    ECU,   &ecu::setDMAproto);
    // set pointer to pointer to commDEv for proto obj
    // connect(&cpW._ecuModelManager, &ecuModelManager::protoSelected,    this,   [&](DMA_proto* proto)
    //         {
    //             proto->setCommDev(&selectedCommDev);
    //             ECU->setDMAproto(proto);}                  // &ecu::setDMAproto выполнится в потоке менеджера,иначе в потоке ecu, и не сможет переместить в поток
    //         );






    connect(&cpW._logParamManager, &protoManager::logRateChanged,      ECU,   &ecu::setLogRate);

    connect(&cpW._wbManager,    &wbManagerWidget::logReady,               &wbWgt, &gaugeWidget::display);

    // Заполняем после подключения, тогда при добавлении буду сигналы
    cpW._ecuModelManager.fillModels(ECU->getAvailModels(), ECU->getAvailProtos());
    cpW._wbManager.fillSerial();
    cpW._wbManager.fillProto();
}
