#include "proto-manager.h"
#include "ui_proto-manager.h"

protoManager::protoManager(QWidget *parent) : QGroupBox(parent), ui(new Ui::protoManager)
{
    ui->setupUi(this);
}

protoManager::~protoManager()
{
    delete ui;
}

void protoManager::_logRateChanged()   // Обновляем скорость обмена
{
    //baudRate = el_lograte.text().toUInt();
    emit logRateChanged(ui->el_lograte->text().toUInt());
}
