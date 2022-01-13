#include "proto-manager.h"

protoManager::protoManager(QWidget *parent):QGroupBox(parent)
{
    setTitle("ECU proto");
    setLayout(&layout);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    availProto.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    lgrt.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    el_lograte.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    layout.addWidget(&availProto, 0, 0);
    layout.addWidget(&lgrt, 0, 1);
    layout.addWidget(&el_lograte, 0, 2);

    connect(&availProto,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &protoManager::_protoSelected);
    connect(&el_lograte,  &QLineEdit::editingFinished, this, &protoManager::_logRateChanged);

    //addProtos();
}


void protoManager::addProtos()
{
//    if ( ECUproto != nullptr )
//        ECUproto->deleteLater();
    //qDebug()<< "deviceManager::addDevice start" << dev.DeviceDesc;
    proto = new jcsbanksDMA();
    availProto.addItem("Custom DMA proto by jcsbanks", QVariant::fromValue<DMA_proto*>(proto));
    proto = new stockDMA();
    availProto.addItem("Stock DMA proto by nanner55", QVariant::fromValue<DMA_proto*>(proto));
    proto = new evoX_DMA();
    availProto.addItem("evoX_DMA by tephra", QVariant::fromValue<DMA_proto*>(proto));


    //connect(this,  &protoManager::_logRateChanged, proto, &DMA_proto::setLogRate);   // я хз зачм так, потом исправлю

}

void protoManager::_protoSelected(int index)
{
    qDebug()<< "protoManager::_protoSelected start";
    //qDebug()<< "deviceManager::_deviceSelected start" << itemData(index);
    DMA_proto *proto = qvariant_cast<DMA_proto*>(availProto.itemData(index));
    qDebug()<< "protoManager::_protoSelected finish" << proto;
    //qDebug()<< "deviceManager::_deviceSelected finish" << devComm->DeviceUniqueID; // Не делай так!!! devComm может быть нулл!!!
    //if( devComm != nullptr)
    emit protoSelected(proto);
}

void protoManager::_logRateChanged()   // Обновляем скорость обмена
{
    //baudRate = el_lograte.text().toUInt();
    emit logRateChanged(el_lograte.text().toUInt());
}
