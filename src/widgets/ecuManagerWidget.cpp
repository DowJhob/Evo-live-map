#include "ecuManagerWidget.h"

ecuManagerWidget::ecuManagerWidget(MainWindow *parent, ecu *ECU, commDevicesController *commDevCtrl) : QToolBar(parent), ECU(ECU)
{
    //    ECU->test();

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

    // addWidget(&wbWgt);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    setIconSize(QSize(200, 200));

    commDevsMngrWgt.deviceLostState();

    connect(this, &ecuManagerWidget::deviceEventLog, parent, &MainWindow::deviceEventLog);
    connect(this, &ecuManagerWidget::Log,            parent, &MainWindow::Log);

    // connect(this, &ecuManagerWidget::ECU_deviceHasLeft, commDevCtrl, &commDevicesController::ECU_DeviceHasLeft);
    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_deviceHasLeft, ECU, &ecu::ECU_DeviceHasLeft);
    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_deviceSelected, commDevCtrl, &commDevicesController::setSelectedECUcommDevice);
    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_SetBaudRate, commDevCtrl, &commDevicesController::ECU_SetBaudRate);

    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_deviceSelected, this, &ecuManagerWidget::deviceSelected);

    // ================================================================================================
    connect(ECU, &ecu::getAvailProtos, this, &ecuManagerWidget::fillAvailECU_Protos);
    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_ModelSelected, ECU, &ecu::setECUmodelType);
    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_ProtoSelected, ECU, &ecu::setDMAproto);

    connect(&commDevsMngrWgt, &commDevicesWidget::logRateChanged, ECU, &ecu::setLogRate);

    QObject::connect(this, &ecuManagerWidget::ecuConnect,          ECU, &ecu::connectDMA, Qt::QueuedConnection);
    QObject::connect(ECU, &ecu::ecuConnected, this, &ecuManagerWidget::ECUconnected, Qt::QueuedConnection);

    // connect(&commDevsMngrWgt, &commDevicesWidget::logReady, &wbWgt, &gaugeWidget::display);

    // Заполняем после подключения, тогда при добавлении буду сигналы
    fillECU_Models(ECU->getAvailModels());
    commDevsMngrWgt.getPresentCommDevices();

    commDevsMngrWgt.fillWB_Serial();
    commDevsMngrWgt.fillWB_Proto();
}

ecuManagerWidget::~ecuManagerWidget()
{
    qDebug() << "~ecuManager";
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

void ecuManagerWidget::deviceSelected(device devComm)
{
    if(devComm.type != deviceType::undef)
    {
        commDevsMngrWgt.devicePresentState();
        a_start_action->setDisabled(false);
    }
    else
    {
        commDevsMngrWgt.deviceLostState();
        a_start_action->setDisabled(true);
    }

    emit deviceEventLog(devComm.DeviceDesc + " / " + devComm.DeviceUniqueID, 0);
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

void ecuManagerWidget::fillECU_Models(QMap<ecuModelType, QString> *availECUmodels)
{
    commDevsMngrWgt.fillECU_Models(availECUmodels);
}

void ecuManagerWidget::fillAvailECU_Protos(QMap<DMA_ProtoType, QString> *availProtos)
{
    commDevsMngrWgt.fillAvailProtos(availProtos);
}
