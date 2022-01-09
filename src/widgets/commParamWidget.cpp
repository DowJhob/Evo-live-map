#include "commParamWidget.h"
#include <QGroupBox>

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

commParamWidget::commParamWidget(QWidget *parent, uint defaultBaudRate) : QWidget(parent)
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
    grBx->setLayout(ll);
    commListBox = new QComboBox(this);
    commListBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ll->addWidget(commListBox, 0, 0);
    QLabel *lb = new QLabel("Baud rate, Baud:", this);
    lb->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ll->addWidget(lb, 0, 1);
    el_baudRate = new QLineEdit(QString::number(defaultBaudRate), this);
    el_baudRate->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ll->addWidget(el_baudRate, 0, 2);

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
    el_logRate = new QLineEdit("50", this);
    el_logRate->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ll->addWidget(el_logRate, 0, 2);


    //панель лямбды

    grBx = new QGroupBox("Select wideband", this);
    ll = new QGridLayout(this);
    grBx->setLayout(ll);
    l->addWidget(grBx);

    QGroupBox *grBxAv = new QGroupBox("Available wideband", this);
    ll->addWidget(grBxAv, 0, 0);
    QGroupBox *grBxWBprt = new QGroupBox("Wideband proto", this);
    ll->addWidget(grBxWBprt, 0, 1);

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

    protoListBox->addItem("Stock DMA proto by nanner55");
    protoListBox->addItem("Custom DMA proto by jcsbanks");
    protoListBox->addItem("evoX_DMA by tephra");


    connect(commListBox,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commParamWidget::deviceSelected);
    connect(protoListBox,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commParamWidget::protoSelected);


    connect(el_baudRate,  &QLineEdit::editingFinished, this, &commParamWidget::baudChng);
    connect(el_logRate,  &QLineEdit::editingFinished, this, &commParamWidget::logchng);

    baudRate = defaultBaudRate;
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void commParamWidget::addDevice(device dev)
{
    //if(commListBox->currentIndex() < 0)

    //qDebug()<< "commParamWidget::addDevice"<< dev.DeviceUniqueID << dev.DeviceInstanceId;
    this->dev.insert(dev.DeviceUniqueID, dev);
    commListBox->addItem(dev.DeviceDesc, dev.DeviceUniqueID);
    //qDebug()<< "commListBox->currentIndex()"<< commListBox->currentIndex();
}

void commParamWidget::removeDevice(device dev)
{
    int index = commListBox->findData(dev.DeviceUniqueID);
    //qDebug()<< " dev.DeviceUniqueID"<< dev.DeviceUniqueID<< "index"<<index<<" count"<<commListBox->count()<<commListBox->currentIndex();

    if( index < commListBox->count())
        commListBox->removeItem(index);
    else
        qDebug() << "Error deleting item";
    this->dev.remove(dev.DeviceUniqueID);
}

void commParamWidget::deviceSelected(int index)
{
    qDebug()<< "deviceSelected"<< commListBox->currentIndex();
    QString DeviceUniqueID = commListBox->itemData(index).toString();
    emit interfaceSelected(dev.value(DeviceUniqueID, device()));
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
