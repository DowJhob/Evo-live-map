#include "ecumodelmanager.h"
#include "src/ECU-model/evo7-ecu-model.h"
#include "ui_ecumodelmanager.h"

ecuModelManager::ecuModelManager(QWidget *parent) : QGroupBox(parent), ui(new Ui::ecuModelManager)
{
    ui->setupUi(this);
    connect(ui->availECUmodel,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ecuModelManager::_modelSelected);
}

ecuModelManager::~ecuModelManager()
{
    delete ui;
}

void ecuModelManager::addModels()
{
    ECU_model *model = new evo7_ECUmodel();
    ui->availECUmodel->addItem("EVO7-9 ECU model", QVariant::fromValue<ECU_model*>(model));
}

void ecuModelManager::_modelSelected(int index)
{
    qDebug()<< "ecuModelManager::_modelSelected";
    ECU_model *model = qvariant_cast<ECU_model*>(ui->availECUmodel->itemData(index));
    if(model ==nullptr)
        return;
    emit modelSelected(model);
}
