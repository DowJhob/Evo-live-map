#include "proto-manager.h"
#include "ui_proto-manager.h"

logRateManagerWidget::logRateManagerWidget(QWidget *parent) : QGroupBox(parent), ui(new Ui::protoManager)
{
    ui->setupUi(this);
}

logRateManagerWidget::~logRateManagerWidget()
{
    delete ui;
}

void logRateManagerWidget::_logRateChanged()   // Обновляем скорость обмена
{
    //baudRate = el_lograte.text().toUInt();
    emit logRateChanged(ui->el_lograte->text().toUInt());
}
