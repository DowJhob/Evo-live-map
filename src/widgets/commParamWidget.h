#ifndef COMMPARAMWIDGET_H
#define COMMPARAMWIDGET_H

#include <QGroupBox>

#include "DMA-ProtoManager/proto-manager.h"
#include "src/widgets/wb-manager/wb-manager.h"
#include "src/widgets/commDeviceManager/devicemanager.h"

class commParamWidget : public QGroupBox
{
    Q_OBJECT
public:
    uint baudRate;
    commDeviceManager devManager;
    protoManager _protoManager;
    wbManager _wbManager;

    explicit commParamWidget(QWidget *parent = nullptr, uint defaultBaudRate = 62500, uint defaultLogRate = 10);
    virtual ~commParamWidget();

public slots:

private:
    QGroupBox commonGrpBx{"Communication parameters"};
    QGridLayout commonGrpBxLayout;


signals:

};

#endif // COMMPARAMWIDGET_H
