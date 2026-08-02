#ifndef COMMDEVICESWIDGET_H
#define COMMDEVICESWIDGET_H

#include <QGroupBox>

#include "DMA-ProtoManager/proto-manager.h"
#include "src/widgets/ecuModelManager/ecumodelmanager.h"
#include "src/widgets/wb-manager/wb-managerWidget.h"
#include "src/widgets/ECUcommDeviceManager/ECUcommDevicesManager.h"
#include "src/filterHelper.h"

class commDevicesWidget : public QGroupBox
{
    Q_OBJECT
public:
    uint baudRate;

    explicit commDevicesWidget(QWidget *parent = nullptr, uint defaultBaudRate = 62500, uint defaultLogRate = 10);
    virtual ~commDevicesWidget();

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    void getPresentCommDevices();

public slots:
    void fillECU_Models(QMap<ecuModelType, QString> *availECUmodels);
    void fillAvailProtos(QMap<DMA_ProtoType, QString> *availProtos);



    void addTactrix(commDeviceWB *cdWB);
    void removeTactrix(commDeviceWB *cdWB);
    void fillWB_Serial();
    void fillWB_Proto();
    void setEnabledWBcomm(bool state);
    void connectedState();
    void devicePresentState();
    void deviceLostState();

private:
    ECUcommDeviceManagerWidget ECUcommDevManagerWidget;
    ecuModelManagerWidget _ecuModelManagerWidget;
    logRateManagerWidget logRateManagerWidget;
    wbManagerWidget _wbManagerWidget;

    void makeConnection();
    QGridLayout commonGrpBxLayout;

signals:
    // from ECU comm Devices Widget
    void ECU_deviceSelected(device);
    void ECU_deviceHasLeft();

    // from ecuModelManagerWidget
    void ECU_ModelSelected(ecuModelType);
    void ECU_ProtoSelected(DMA_ProtoType);
    void ECU_SetBaudRate(quint32);

    // from logRateManagerWidget
    void logRateChanged(uint);

    // from wbManagerWidget
    void wbSelected(commDeviceWB*);
    void WB_ProtoSelected(wbProto*);
    void wbStart(bool);
    void logReady(QString);

    // void deviceEvent(device);
    void Log(QString);

};

#endif // COMMDEVICESWIDGET_H
