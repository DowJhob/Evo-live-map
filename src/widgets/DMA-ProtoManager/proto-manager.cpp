#include "proto-manager.h"
#include "ui_proto-manager.h"

protoManager::protoManager(QWidget *parent) : QGroupBox(parent), ui(new Ui::protoManager)
{
    ui->setupUi(this);
    connect(ui->availProto,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &protoManager::_protoSelected);
    connect(ui->el_lograte,  &QLineEdit::editingFinished, this, &protoManager::_logRateChanged);
}

protoManager::~protoManager()
{
    delete ui;
}

void protoManager::addProtos()
{
//    if ( ECUproto != nullptr )
//        ECUproto->deleteLater();
    //qDebug()<< "deviceManager::addDevice start" << dev.DeviceDesc;

    DMA_proto *proto = new jcsbanksDMA();
    ui->availProto->addItem("Custom DMA proto by jcsbanks", QVariant::fromValue<DMA_proto*>(proto));
    proto = new stockDMA();
    ui->availProto->addItem("Stock DMA proto by nanner55", QVariant::fromValue<DMA_proto*>(proto));
    proto = new evoX_DMA();
    ui->availProto->addItem("evoX_DMA by tephra", QVariant::fromValue<DMA_proto*>(proto));


    //connect(this,  &protoManager::_logRateChanged, proto, &DMA_proto::setLogRate);   // я хз зачм так, потом исправлю

}

void protoManager::_protoSelected(int index)
{
    qDebug()<< "protoManager::_protoSelected start";
    //qDebug()<< "deviceManager::_deviceSelected start" << itemData(index);
    DMA_proto *proto = qvariant_cast<DMA_proto*>(ui->availProto->itemData(index));
    qDebug()<< "protoManager::_protoSelected finish" << proto;
    //qDebug()<< "deviceManager::_deviceSelected finish" << devComm->DeviceUniqueID; // Не делай так!!! devComm может быть нулл!!!
    //if( devComm != nullptr)
    emit protoSelected(proto);
}

void protoManager::_logRateChanged()   // Обновляем скорость обмена
{
    //baudRate = el_lograte.text().toUInt();
    emit logRateChanged(ui->el_lograte->text().toUInt());
}
