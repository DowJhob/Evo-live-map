#include "ecuManagerWidget.h"

ecuManagerWidget::ecuManagerWidget(QWidget *parent, ecu *ECU, commDevicesController *commDevCtrl) : QToolBar(parent), ECU(ECU)
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

    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_deviceHasLeft, this, &ecuManagerWidget::ECU_deviceHasLeft);
    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_deviceSelected, this, &ecuManagerWidget::deviceSelected);
    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_deviceSelected, this, &ecuManagerWidget::ECU_DeviceSelected);
    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_ModelSelected, this, &ecuManagerWidget::ECU_ModelSelected);
    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_ProtoSelected, this, &ecuManagerWidget::ECU_ProtoSelected);



    connect(&commDevsMngrWgt, &commDevicesWidget::logRateChanged, this, &ecuManagerWidget::logRateChanged);
    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_ProtoSelected, this, &ecuManagerWidget::ECU_ProtoSelected);
    connect(&commDevsMngrWgt, &commDevicesWidget::ECU_ProtoSelected, this, &ecuManagerWidget::ECU_ProtoSelected);


    // connect(&commDevsMngrWgt, &commDevicesWidget::ECU_deviceHasLeft, ECU, &ecu::deviceHasLeft);
    connect(&commDevsMngrWgt, &commDevicesWidget::logRateChanged, ECU, &ecu::setLogRate);
    connect(&commDevsMngrWgt, &commDevicesWidget::logReady, &wbWgt, &gaugeWidget::display);






    // Заполняем после подключения, тогда при добавлении буду сигналы
    commDevsMngrWgt.fillWB_Serial();
    commDevsMngrWgt.fillWB_Proto();



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

void ecuManagerWidget::deviceSelected(device devComm)
{
    // if(devComm == nullptr)
    // {
    //     //        cpW.setEnabledECUcomm(false);
    //     commDevsMngrWgt.deviceLostState();
    //     a_start_action->setDisabled(true);
    //     a_ramReset->setDisabled(true);
    //     emit deviceEventLog("No interface", 0);
    //     return;
    // }
    emit deviceEventLog(devComm.DeviceDesc + " / " + devComm.DeviceUniqueID, 0);

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

void ecuManagerWidget::fillECU_Models(QMap<ecuModelType, QString> *availECUmodels)
{
    commDevsMngrWgt.fillECU_Models(availECUmodels);
}

void ecuManagerWidget::fillAvailECU_Protos(QMap<DMA_ProtoType, QString> *availProtos)
{
    commDevsMngrWgt.fillAvailProtos(availProtos);
}
