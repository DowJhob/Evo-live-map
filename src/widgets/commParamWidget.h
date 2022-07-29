#ifndef COMMPARAMWIDGET_H
#define COMMPARAMWIDGET_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QLayout>
#include <QLineEdit>

#include "../comm-device-interface/devicemanager.h"
#include "../DMA-proto/proto-manager.h"
#include "../wideband/wb-manager.h"

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
