#include "commParamWidget.h"
#include <QGroupBox>

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

commParamWidget::commParamWidget(QWidget *parent, uint defaultBaudRate, uint defaultLogRate) : QWidget(parent)
{
    setLayout(&commonGrpBxLayout);

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
    commonGrpBxLayout.addWidget(wbManager, 2, 0);
}
