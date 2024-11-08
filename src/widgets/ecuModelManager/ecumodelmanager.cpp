#include "ecumodelmanager.h"

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
void ecuModelManager::fillModels(QMap<ecuModelType, ECU_model*>* availECUmodels, QMap<DMA_ProtoType, DMA_proto *> *availProto)
{
    this->availECUmodels = availECUmodels;
    this->availProtos = availProto;

    for(auto _ECUmodel : *availECUmodels)
    {
        ui->availECUmodel->addItem(_ECUmodel->name, QVariant::fromValue<ecuModelType>(_ECUmodel->type));
    }
}

void ecuModelManager::fillAvailProtos(ECU_model* _ECUmodel)
{
    ui->availProto_comboBox->blockSignals(true);
    ui->availProto_comboBox->clear();
    ui->availProto_comboBox->blockSignals(false);

    if(_ECUmodel == nullptr)
        return;

    for(auto protoType : *_ECUmodel->getAvailProtos())
    {
        auto proto = availProtos->value(protoType);
        ui->availProto_comboBox->addItem(proto->name, QVariant::fromValue<DMA_ProtoType>(proto->type));
    }
}

void ecuModelManager::_modelSelected(int index)
{
    ecuModelType modelType = qvariant_cast<ecuModelType>(ui->availECUmodel->itemData(index));

    ECU_model* model = availECUmodels->value(modelType, nullptr);

    if(model == nullptr)
        return;

    qDebug()<< "ecuModelManager::_modelSelected  /  index:" << index << "   /  model:" << model << "   /  model->thread():" << model->thread();

    emit modelSelected(model);

    fillAvailProtos(model);
}

void ecuModelManager::_protoSelected(int index)
{
    DMA_ProtoType protoType = qvariant_cast<DMA_ProtoType>(ui->availProto_comboBox->itemData(index));

    DMA_proto* proto = availProtos->value(protoType);

    if(proto == nullptr)
        return;

    qDebug()<< "ecuModelManager::_protoSelected  /  index:" << index << "   /  proto:" << proto << "   /  proto->thread():" << proto->thread();

    emit protoSelected(proto);
}
