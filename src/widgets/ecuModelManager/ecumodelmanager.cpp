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
void ecuModelManager::fillModels(QMap<ecuModelType, QString> *availECUmodels)
{
    // this->availECUmodels = availECUmodels;
    // this->availProtos = availProto;
    for (auto it = availECUmodels->keyValueBegin(); it != availECUmodels->keyValueEnd(); ++it)
    {
        auto _ECUmodelType = it->first; auto _ECUmodelName = it->second;
        ui->availECUmodel->addItem(_ECUmodelName, QVariant::fromValue<ecuModelType>(_ECUmodelType));
    }

}

void ecuModelManager::fillAvailProtos(QMap<DMA_ProtoType, QString> *availProtos)
{
    ui->availProto_comboBox->blockSignals(true);
    ui->availProto_comboBox->clear();
    ui->availProto_comboBox->blockSignals(false);

    // if(_ECUmodel == nullptr)
    //     return;
    for (auto it = availProtos->keyValueBegin(); it != availProtos->keyValueEnd(); ++it)
    {
        auto protoType = it->first; auto protoName = it->second;
        ui->availProto_comboBox->addItem(protoName, QVariant::fromValue<DMA_ProtoType>(protoType));
    }
}

void ecuModelManager::_modelSelected(int index)
{
    ecuModelType modelType = qvariant_cast<ecuModelType>(ui->availECUmodel->itemData(index));

    // ECU_model* model = availECUmodels->value(modelType, nullptr);

    // if(model == nullptr)
        // return;

    // qDebug()<< "ecuModelManager::_modelSelected  /  index:" << index << "   /  model:" << model << "   /  model->thread():" << model->thread();

    // emit modelSelected(model);
    emit modelSelected(modelType);

    // fillAvailProtos(model);
}

void ecuModelManager::_protoSelected(int index)
{
    DMA_ProtoType protoType = qvariant_cast<DMA_ProtoType>(ui->availProto_comboBox->itemData(index));

    // DMA_proto* proto = availProtos->value(protoType);

    // if(proto == nullptr)
    //     return;

    // qDebug()<< "ecuModelManager::_protoSelected  /  index:" << index << "   /  proto:" << proto << "   /  proto->thread():" << proto->thread();

    emit protoSelected(protoType);
}
