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

class commParamWidget : public QWidget
{
    Q_OBJECT
public:
    uint baudRate;
    explicit commParamWidget(QWidget *parent = nullptr, uint defaultBaudRate = 62500, uint defaultLogRate = 10);

public slots:
    void setDeviceManager(deviceManager *devManager);
    void setProtoManager(protoManager *protoManager);
    void setWBManager(wbManager *wbManager);

    void addWB(device dev);
    void removeWB(device dev);
    void WBSelected(int index);
    void WBprotoSelected(int index);
    //void protoSelected(int index);

private:
    QGroupBox commonGrpBx{"Communication parameters"};
    QGridLayout commonGrpBxLayout;

    QGridLayout widebandLayout;

    QHash<QString, device> dev;
    QComboBox *protoListBox;
    QComboBox *availWB;
    QComboBox *protoWB;

    QLineEdit *el_baudRate;
    QLineEdit *el_logRate;

    void baudChng();

    void logchng();

signals:
    void protoSelected(int);
    void wbSelected(int);
    void wbProtoSelected(int);

    void baudChanged(int);
    void logChanged(int);

};

#endif // COMMPARAMWIDGET_H
