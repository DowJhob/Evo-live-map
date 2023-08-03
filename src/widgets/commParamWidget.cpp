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

    commonGrpBxLayout.addWidget(&devManager,       0, 0);
    commonGrpBxLayout.addWidget(&_protoManager,    1, 0);
    commonGrpBxLayout.addWidget(&_wbManager,       2, 0);
    commonGrpBxLayout.addWidget(&_ecuModelManager, 3, 0);
    
    connect(&devManager,   &commDeviceManagerWidget::tactrixArrived,   &_wbManager,  &wbManagerWidget::addTactrix);
    connect(&devManager,   &commDeviceManagerWidget::tactrixRemoved,   &_wbManager,  &wbManagerWidget::removeTactrix);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *si = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    commonGrpBxLayout.addItem(si, 4, 0);
}

commParamWidget::~commParamWidget(){}



void commParamWidget::setEnabledWBcomm(bool state)
{
    _wbManager.setEnabled(state);
}

void commParamWidget::connectedState()
{
    devManager.setEnabled(false);
    _protoManager.setEnabled(false);
    _ecuModelManager.setEnabled(false);
}

void commParamWidget::devicePresentState()
{
    devManager.setEnabled(true);
    _protoManager.setEnabled(true);
    _ecuModelManager.setEnabled(true);
}

void commParamWidget::deviceLostState()
{
    devManager.setEnabled(false);
    _protoManager.setEnabled(false);
    _ecuModelManager.setEnabled(true);
    //        _wbManager.setEnabled(state);
}
