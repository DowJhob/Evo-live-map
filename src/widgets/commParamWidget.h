#ifndef COMMPARAMWIDGET_H
#define COMMPARAMWIDGET_H

#include <QGroupBox>

#include "DMA-ProtoManager/proto-manager.h"
#include "src/widgets/ecuModelManager/ecumodelmanager.h"
#include "src/widgets/wb-manager/wb-manager.h"
#include "src/widgets/commDeviceManager/devicemanager.h"

class commParamWidget : public QGroupBox
{
    Q_OBJECT
public:
    uint baudRate;
    commDeviceManagerWidget devManager;
    wbManagerWidget _wbManager;
    ecuModelManager _ecuModelManager;
    protoManager _protoManager;

    explicit commParamWidget(QWidget *parent = nullptr, uint defaultBaudRate = 62500, uint defaultLogRate = 10);
    virtual ~commParamWidget();

public slots:
    void setEnabledWBcomm(bool state);

    void connectedState();

    void devicePresentState();

    void deviceLostState();

private:
    QGridLayout commonGrpBxLayout;

signals:

};

#endif // COMMPARAMWIDGET_H
