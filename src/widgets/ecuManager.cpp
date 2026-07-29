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

    addWidget(&commDevsMngrWgt);

    // cpW._ecuModelManager.ecu_thread = ECU->readThread;

    addSeparator();

    addWidget(&wbWgt);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    setIconSize(QSize(200, 200));

    commDevsMngrWgt.deviceLostState();
    connect(&commDevsMngrWgt.ECUcommDevManager,       &ECUcommDeviceManagerWidget::deviceSelected, this,  &ecuManagerWidget::setComDev);
    commDevsMngrWgt.getPresentCommDevices();
}

ecuManagerWidget::~ecuManagerWidget()
{
    //qDebug() << "~ecuManager";
}

void ecuManagerWidget::ECUconnected(bool state)
{
    //qDebug() << "=========== ecuManager::interfaceLock ================ state:" << state;
    //    cpW.setEnabledECUcomm(!state);
    a_ramReset->setDisabled(!state);
    if(state)
    {
        a_start_action->setText("Stop");
        commDevsMngrWgt.connectedState();
    }
    else
    {
        a_start_action->setText("Start");
        commDevsMngrWgt.devicePresentState();
    }
}

void ecuManagerWidget::setComDev(comm_device_interface *devComm)
{
    if(devComm == nullptr)
    {
        //        cpW.setEnabledECUcomm(false);
        commDevsMngrWgt.deviceLostState();
        a_start_action->setDisabled(true);
        a_ramReset->setDisabled(true);
        emit deviceEventLog("No interface", 0);
        return;
    }
    emit deviceEventLog(devComm->DeviceDesc + " / " + devComm->DeviceUniqueID, 0);

    commDevsMngrWgt.devicePresentState();
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
    connect(&commDevsMngrWgt.ECUcommDevManager,       &ECUcommDeviceManagerWidget::deviceSelected, ECU,   &ecu::setComDev);
    // connect(&commDevsMngrWgt.ECUcommDevManager,       &ECUcommDeviceManagerWidget::deviceSelected, this,  &ecuManagerWidget::setComDev);
    connect(&commDevsMngrWgt.ECUcommDevManager,       &ECUcommDeviceManagerWidget::deviceHasLeft,  ECU,   &ecu::deviceHasLeft);



    connect(&commDevsMngrWgt._ecuModelManager, &ecuModelManager::modelSelected,    ECU,   &ecu::setECUmodelType);
    connect(ECU, &ecu::getAvailProtos,    &commDevsMngrWgt._ecuModelManager,   &ecuModelManager::fillAvailProtos);
    connect(&commDevsMngrWgt._ecuModelManager, &ecuModelManager::protoSelected,    ECU,   &ecu::setDMAproto);


    connect(&commDevsMngrWgt._logParamManager, &protoManager::logRateChanged,      ECU,   &ecu::setLogRate);

    connect(&commDevsMngrWgt._wbManager,    &wbManagerWidget::logReady,               &wbWgt, &gaugeWidget::display);

    // Заполняем после подключения, тогда при добавлении буду сигналы
    // cpW._ecuModelManager.fillModels(ECU->getAvailModels(), ECU->getAvailProtos());
    commDevsMngrWgt._ecuModelManager.fillModels(ECU->getAvailModels2());
    commDevsMngrWgt._wbManager.fillSerial();
    commDevsMngrWgt._wbManager.fillProto();
}
