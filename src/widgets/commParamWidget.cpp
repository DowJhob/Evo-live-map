#include "commParamWidget.h"
#include <QGroupBox>

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

commParamWidget::commParamWidget(QWidget *parent, uint defaultBaudRate, uint defaultLogRate) : QGroupBox(parent)
{
    baudRate = defaultBaudRate;
    setLayout(&commonGrpBxLayout);
//setStyleSheet("border-style: sunken; border: 1px solid black;") ;


    commonGrpBxLayout.addWidget(&_protoManager, 1, 0);
    commonGrpBxLayout.addWidget(&_wbManager, 2, 0);

    commonGrpBxLayout.addWidget(&devManager, 0, 0);

///    _protoManager.addProtos();
    connect(&devManager,   &deviceManager::tactrixArrived,   &_wbManager,  &wbManager::addTactrix);




    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QSpacerItem *si = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    commonGrpBxLayout.addItem(si, 3, 0);
}

commParamWidget::~commParamWidget(){}
