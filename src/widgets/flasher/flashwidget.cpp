#include "flashwidget.h"
#include "src/ECU-model/evo7-ecu-model.h"
#include "ui_flashwidget.h"

flashWidget::flashWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::flashWidget)
{
    ui->setupUi(this);
}

flashWidget::~flashWidget()
{
    delete ui;
}

void flashWidget::_readECU()
{

}

void flashWidget::_writeECU()
{
    //emit writeECU(hexEdit.data());
}

void flashWidget::addFlashModel()
{
//    if ( ECUproto != nullptr )
//        ECUproto->deleteLater();
    //qDebug()<< "deviceManager::addDevice start" << dev.DeviceDesc;

    ECU_model *flashModel = new evo7_ECUmodel();
    ui->availFlashModel->addItem("Custom DMA proto by jcsbanks", QVariant::fromValue<ECU_model*>(flashModel));
    //proto = new stockDMA();
    //ui->availProto->addItem("Stock DMA proto by nanner55", QVariant::fromValue<DMA_proto*>(proto));
    //proto = new evoX_DMA();
    //ui->availProto->addItem("evoX_DMA by tephra", QVariant::fromValue<DMA_proto*>(proto));


    //connect(this,  &protoManager::_logRateChanged, proto, &DMA_proto::setLogRate);   // я хз зачм так, потом исправлю

}
