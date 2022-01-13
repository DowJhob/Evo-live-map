#include "commParamWidget.h"
#include <QGroupBox>

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

commParamWidget::commParamWidget(QWidget *parent, uint defaultBaudRate, uint defaultLogRate) : QWidget(parent)
{
    //общая панель
    setLayout(&commonGrpBxLayout);

    QGroupBox *grBx //= new QGroupBox("ECU proto", this)
            ;
    QGridLayout *ll //= new QGridLayout()
            ;

    //панель лямбды
    grBx = new QGroupBox("Select wideband", this);
    ll = new QGridLayout();
    grBx->setLayout(ll);
    commonGrpBxLayout.addWidget(grBx, 2, 0);

    QGroupBox *grBxAv = new QGroupBox("Available wideband", this);
    ll->addWidget(grBxAv, 0, 0);
    QGroupBox *grBxWBprt = new QGroupBox("Wideband proto", this);
    ll->addWidget(grBxWBprt, 0, 1);

    grBxAv->setLayout(&widebandLayout);

//    availWB = new QComboBox(this);
//    availWB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//    widebandLayout.addWidget(availWB);

    ll = new QGridLayout();
    grBxWBprt->setLayout(ll);

    protoWB = new QComboBox(this);
    //protoWB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    widebandLayout.addWidget(protoWB);

    protoWB->addItem("Innovate");
    protoWB->addItem("PLX");
    protoWB->addItem("AEM");

    //connect(availWB,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commParamWidget::WBSelected);
    connect(protoWB,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commParamWidget::WBprotoSelected);

    qDebug() << " hop";

    baudRate = defaultBaudRate;
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);


    QSpacerItem *si = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    commonGrpBxLayout.addItem(si, 3, 0);
}

void commParamWidget::setDeviceManager(deviceManager *devManager)
{
    commonGrpBxLayout.addWidget(devManager, 0, 0);
    //devManager->baudRateUpdate();
}

void commParamWidget::setProtoManager(protoManager *protoManager)
{
    commonGrpBxLayout.addWidget(protoManager, 1, 0);
    protoManager->addProtos();
}

void commParamWidget::setWBManager(wbManager *wbManager)
{
    widebandLayout.addWidget(wbManager, 2, 0);
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
