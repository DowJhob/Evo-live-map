#include "commParamWidget.h"
#include <QGroupBox>

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

commParamWidget::commParamWidget(QWidget *parent, uint defaultBaudRate, uint defaultLogRate) : QWidget(parent)
{
    QLayout *l = new QGridLayout(this);
    setLayout(l);
    //общая панель
    QGroupBox *grBx = new QGroupBox("Communication parameters", this);
    grBx->setFlat(false);
    l->addWidget(grBx);
    l = new QGridLayout(this);
    grBx->setLayout(l);

    //панель для интерфейса
    grBx = new QGroupBox("Communication devices", this);
    l->addWidget(grBx);
    QGridLayout *ll = new QGridLayout(this);
    grBx->setLayout(&communicationLayout);
    QLabel *lb = new QLabel("Baud rate, Baud:", this);
    lb->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    communicationLayout.addWidget(lb, 0, 1);
    el_baudRate = new QLineEdit(QString::number(defaultBaudRate), this);
    el_baudRate->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    communicationLayout.addWidget(el_baudRate, 0, 2);

    //панель для протокола
    grBx = new QGroupBox("ECU proto", this);
    ll = new QGridLayout(this);
    grBx->setLayout(ll);
    l->addWidget(grBx);
    protoListBox = new QComboBox(this);
    protoListBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ll->addWidget(protoListBox);
    lb = new QLabel("Logging rate, Hz:", this);
    lb->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ll->addWidget(lb, 0, 1);
    el_logRate = new QLineEdit(QString::number(defaultLogRate), this);
    el_logRate->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ll->addWidget(el_logRate, 0, 2);


    //панель лямбды
    grBx = new QGroupBox("Select wideband", this);

    grBx->setLayout(&widebandLayout);
    l->addWidget(grBx);

    QGroupBox *grBxAv = new QGroupBox("Available wideband", this);
    widebandLayout.addWidget(grBxAv, 0, 0);
    QGroupBox *grBxWBprt = new QGroupBox("Wideband proto", this);
    widebandLayout.addWidget(grBxWBprt, 0, 1);

    ll = new QGridLayout(this);
    grBxAv->setLayout(ll);

    availWB = new QComboBox(this);
    availWB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ll->addWidget(availWB);



    ll = new QGridLayout(this);
    grBxWBprt->setLayout(ll);

    protoWB = new QComboBox(this);
    protoWB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ll->addWidget(protoWB);

    protoWB->addItem("Innovate");
    protoWB->addItem("PLX");
    protoWB->addItem("AEM");

    connect(availWB,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commParamWidget::WBSelected);
    connect(protoWB,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commParamWidget::WBprotoSelected);


    QSpacerItem *si = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    l->addItem(si);

    protoListBox->addItem("Custom DMA proto by jcsbanks");
    protoListBox->addItem("Stock DMA proto by nanner55");
    protoListBox->addItem("evoX_DMA by tephra");

    connect(protoListBox,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commParamWidget::protoSelected);


    connect(el_baudRate,  &QLineEdit::editingFinished, this, &commParamWidget::baudChng);
    connect(el_logRate,  &QLineEdit::editingFinished, this, &commParamWidget::logchng);

    baudRate = defaultBaudRate;
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void commParamWidget::addWB(device dev)
{

}

void commParamWidget::removeWB(device dev)
{

}

void commParamWidget::WBSelected(int index)
{

}

void commParamWidget::WBprotoSelected(int index)
{

}

void commParamWidget::baudChng()   // Обновляем скорость обмена
{
    baudRate = el_baudRate->text().toUInt();
    //emit baudChanged(el_baudRate->text().toUInt());
}

void commParamWidget::logchng()
{
    emit logChanged(1000/el_logRate->text().toUInt());
}
