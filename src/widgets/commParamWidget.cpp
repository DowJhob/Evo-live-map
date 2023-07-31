#include "commParamWidget.h"
#include <QGroupBox>

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

commParamWidget::commParamWidget(QWidget *parent, uint defaultBaudRate, uint defaultLogRate) : QGroupBox(parent)
{
    setTitle("Communication devices");
    baudRate = defaultBaudRate;
    setLayout(&commonGrpBxLayout);

    commonGrpBxLayout.addWidget(&devManager, 0, 0);
    commonGrpBxLayout.addWidget(&_wbManager, 1, 0);
    commonGrpBxLayout.addWidget(&_ecuModelManager, 2, 0);
    commonGrpBxLayout.addWidget(&_protoManager, 3, 0);

    connect(&devManager,   &commDeviceManager::tactrixArrived,   &_wbManager,  &wbManager::addTactrix);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *si = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    commonGrpBxLayout.addItem(si, 4, 0);
}

commParamWidget::~commParamWidget(){}
