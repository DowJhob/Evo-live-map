#include "commDevicesWidget.h"
#include <QGroupBox>

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

commDevicesWidget::commDevicesWidget(QWidget *parent, uint defaultBaudRate, uint defaultLogRate) : QGroupBox(parent)
{
    setTitle("Communication devices");
    baudRate = defaultBaudRate;
    setLayout(&commonGrpBxLayout);

    commonGrpBxLayout.addWidget(&ECUcommDevManager,       0, 0);
    commonGrpBxLayout.addWidget(&_ecuModelManager, 1, 0);
    commonGrpBxLayout.addWidget(&_logParamManager, 2, 0);
    commonGrpBxLayout.addWidget(&_wbManager,       3, 0);
    
    connect(&ECUcommDevManager,   &ECUcommDeviceManagerWidget::tactrixArrived,   &_wbManager,  &wbManagerWidget::addTactrix);
    connect(&ECUcommDevManager,   &ECUcommDeviceManagerWidget::tactrixRemoved,   &_wbManager,  &wbManagerWidget::removeTactrix);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *si = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    commonGrpBxLayout.addItem(si, 4, 0);


    HWND hwnd = reinterpret_cast<HWND>(winId());

    notifyRegister(hwnd);
}

commDevicesWidget::~commDevicesWidget(){}

void commDevicesWidget::setEnabledWBcomm(bool state)
{
    _wbManager.setEnabled(state);
}

void commDevicesWidget::connectedState()
{
    ECUcommDevManager.setEnabled(false);
    _logParamManager.setEnabled(false);
    _ecuModelManager.setEnabled(false);
}

void commDevicesWidget::devicePresentState()
{
    ECUcommDevManager.setEnabled(true);
    _logParamManager.setEnabled(true);
    _ecuModelManager.setEnabled(true);
}

void commDevicesWidget::deviceLostState()
{
    ECUcommDevManager.setEnabled(false);
    _logParamManager.setEnabled(false);
    _ecuModelManager.setEnabled(true);
    //        _wbManager.setEnabled(state);
}
