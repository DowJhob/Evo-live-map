#include "commParamWidget.h"
#include <QGroupBox>

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

commParamWidget::commParamWidget(QWidget *parent) : QWidget(parent)
{
    QLayout *l = new QGridLayout(this);
    setLayout(l);
    //общая панель
    QGroupBox *grBx = new QGroupBox(this);
    grBx->setTitle("Communication parameters");
    l->addWidget(grBx);
    l = new QGridLayout(this);
    grBx->setLayout(l);

    //панель для интерфейса
    grBx = new QGroupBox(this);
    grBx->setTitle("Communication devices");
    l->addWidget(grBx);
    QGridLayout *ll = new QGridLayout(this);
    grBx->setLayout(ll);
    commListBox = new QComboBox(this);
    commListBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ll->addWidget(commListBox, 0, 0);
    QLabel *lb = new QLabel("Baud rate, Baud:", this);
    lb->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ll->addWidget(lb, 0, 1);
    el_baudRate = new QLineEdit("62500", this);
    el_baudRate->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ll->addWidget(el_baudRate, 0, 2);

    //панель для протокола
    grBx = new QGroupBox(this);
    grBx->setTitle("ECU proto");
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


    QSpacerItem *si = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    l->addItem(si);

    protoListBox->addItem("Stock DMA proto by nanner55");
    protoListBox->addItem("Custom DMA proto by jcsbanks");
    protoListBox->addItem("evoX_DMA by tephra");


    connect(commListBox,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commParamWidget::deviceSelected);
    connect(protoListBox,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commParamWidget::protoSelected);


    connect(el_baudRate,  &QLineEdit::editingFinished, this, &commParamWidget::baudChng);
    connect(el_logRate,  &QLineEdit::editingFinished, this, &commParamWidget::logchng);

}

void commParamWidget::addDevice(device dev)
{
    //if(commListBox->currentIndex() < 0)

    //qDebug()<< "commListBox->currentIndex()"<< commListBox->currentIndex();
    this->dev.insert(dev.DeviceUniqueID, dev);
    commListBox->addItem(dev.DeviceDesc, dev.DeviceUniqueID);
    //qDebug()<< "commListBox->currentIndex()"<< commListBox->currentIndex();
}

void commParamWidget::removeDevice(device dev)
{
    int index = commListBox->findData(dev.DeviceUniqueID);
    qDebug()<< " dev.DeviceUniqueID"<< dev.DeviceUniqueID<< "index"<<index<<" count"<<commListBox->count()<<commListBox->currentIndex();

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

void commParamWidget::baudChng()   // Обновляем скорость обмена
{
    //baudRate = el_baudRate->text().toUInt();
    //emit baudChanged(el_baudRate->text().toUInt());
}

void commParamWidget::logchng()
{
    emit logChanged(1000/el_logRate->text().toUInt());
}
