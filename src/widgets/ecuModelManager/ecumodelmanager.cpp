#include "ecumodelmanager.h"

#include "src/ECU-model/evo7-ecu-model.h"
#include "src/ECU-model/evoX-ecu-model.h"

#include "../../DMA-proto/jcsbanksDMA.h"
#include "../../DMA-proto/stockDMA.h"
#include "../../DMA-proto/evoX-DMA.h"

#include "ui_ecumodelmanager.h"

ecuModelManager::ecuModelManager(QWidget *parent) : QGroupBox(parent), ui(new Ui::ecuModelManager)
{
    ui->setupUi(this);

    connect(ui->availECUmodel,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ecuModelManager::_modelSelected);

    connect(ui->availProto_comboBox,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ecuModelManager::_protoSelected);
}

ecuModelManager::~ecuModelManager()
{
    delete ui;
}

// Заполняем после подключения, тогда при добавлении буду сигналы
void ecuModelManager::fillModels()
{
    ui->availECUmodel->addItem("EVO7-9 ECU model", QVariant::fromValue<ecuModelType>(ecuModelType::EVO7_9_ECU_Model));
    ui->availECUmodel->addItem("EVO X ECU model", QVariant::fromValue<ecuModelType>(ecuModelType::EVO_X_ECU_Model));
}

void ecuModelManager::_modelSelected(int index)
{
    if(model != nullptr)
        model->deleteLater();

    model = nullptr;

    ecuModelType modelType = qvariant_cast<ecuModelType>(ui->availECUmodel->itemData(index));

    switch (modelType) {
    case ecuModelType::EVO7_9_ECU_Model:
        model = new evo7_ECUmodel();
        break;
    case ecuModelType::EVO_X_ECU_Model:
        model = new evoX_ECUmodel();
        break;
    default:
        return;
        break;
    }

    emit modelSelected(model);

    fillAvailModelProtos();

    qDebug()<< "ecuModelManager::_modelSelected  /  index:" << index << "   /  model:" << model;
}

void ecuModelManager::_protoSelected(int index)
{
    if(proto != nullptr)
        proto->deleteLater();

    proto = nullptr;

    DMA_ProtoType protoType = qvariant_cast<DMA_ProtoType>(ui->availProto_comboBox->itemData(index));

    switch (protoType) {
    case DMA_ProtoType::jcsbanks:
        proto = new jcsbanksDMA();
        break;
    case DMA_ProtoType::nanner55:
        proto = new stockDMA();
        break;
    case DMA_ProtoType::tephraX:
        proto = new evoX_DMA();
        break;
    default:
        return;
        break;
    }

    if(model == nullptr)
        return;

    proto->ecu_model = &model;

    // qDebug()<< "protoManager::_protoSelected  thread():" << thread() << "   /  proto->thread():" << proto->thread();
    // proto->moveToThread(ecu_thread);

    emit protoSelected(proto);

    // qDebug()<< "protoManager::_protoSelected  /  index:" << index << "   /  proto:" << proto << "   /  ecu_thread:" << ecu_thread << "   /  proto->thread():" << proto->thread();
}

void ecuModelManager::fillAvailModelProtos()
{
    ui->availProto_comboBox->blockSignals(true);
    ui->availProto_comboBox->clear();
    ui->availProto_comboBox->blockSignals(false);

    if(model == nullptr)
        return;

    for(auto var : model->getAvailProto())
    {
        switch (var) {
        case DMA_ProtoType::jcsbanks:
            ui->availProto_comboBox->addItem("Custom DMA proto by jcsbanks", QVariant::fromValue<DMA_ProtoType>(DMA_ProtoType::jcsbanks));
            break;
        case DMA_ProtoType::nanner55:
            ui->availProto_comboBox->addItem("Stock DMA proto by nanner55", QVariant::fromValue<DMA_ProtoType>(DMA_ProtoType::nanner55));
            break;
        case DMA_ProtoType::tephraX:
            ui->availProto_comboBox->addItem("evoX_DMA by tephra", QVariant::fromValue<DMA_ProtoType>(DMA_ProtoType::tephraX));
            break;
        default:
            // return;
            break;
        }
    }
}
