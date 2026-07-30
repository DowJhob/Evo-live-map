#ifndef COMMDEVICESWIDGET_H
#define COMMDEVICESWIDGET_H

#include <QGroupBox>

#include "DMA-ProtoManager/proto-manager.h"
#include "src/widgets/ecuModelManager/ecumodelmanager.h"
#include "src/widgets/wb-manager/wb-managerWidget.h"
#include "src/widgets/ECUcommDeviceManager/ECUcommDevicesManager.h"

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

    //Подписываемся на события
    void notifyRegister(HWND hwnd);

    QGridLayout commonGrpBxLayout;

    QString reg = "HKEY_LOCAL_MACHINE\\SOFTWARE\\PassThruSupport.04.04";
    QString reg64 = "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\PassThruSupport.04.04";

    const QVector<GUID> subscribeInterfaces{
        { 0xa5dcbf10, 0x6530, 0x11d2, {0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed}},        // raw usb device work for all
        //{ 0x219d0508, 0x57a8, 0x4ff5, {0x97, 0xa1, 0xbd, 0x86, 0x58, 0x7c, 0x6c, 0x7e}},        // FTDI_D2XX_Device Class GUID
        //{ 0x86e0d1e0, 0x8089, 0x11d0, {0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}},        // FTDI_VCP_Device Class GUID
        { 0x4d36e978, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}},       // Serial and parralel ports standart Windows class (seeng Openport1.3 (as serial port))
        //{ 0x36fc9e60, 0xc465, 0x11cf, {0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}},          // Устройства шины USB (концентраторы и контроллеры узлов)
        //{ 0x5a929f4c, 0x6f07, 0x426d, {0xa9, 0x70, 0x90, 0x3d, 0x25, 0xd4, 0x45, 0xb3}},       // it s from  Scanmatic dev manager propertis, dont worked (not interface)
        // hardly for use
        //{ 0xfb1cf0c4, 0xb412, 0x451f, {0x9f, 0x04, 0xdf, 0x75, 0x37, 0xa5, 0x00, 0x3c}},          // it s from  tactrix dev manager propertis, worked if disable DIGCF_DEVICEINTERFACE
        // seeng both openport 13 and 20 but op13 seeng twice
        //{ 0x6d1781b7, 0xc987, 0x4f6c, {0x8d, 0x4f, 0x1e, 0xfc, 0x09, 0x8b, 0xea, 0x67}},       // кажется это загрузчик тактрикса для смены ПО tactrix VehiclePassThru j2534 ??????? https://www.evoxforums.com/threads/bricked-my-ecu.236402/
    };
    //    const QVector<GUID> presentInterfaces{
    //        { 0xa5dcbf10, 0x6530, 0x11d2, {0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed}},
    //        { 0x4d36e978, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}}
    //    };

    //GUID SERIAL{ 0x4d36e978, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}};
    //GUID J2534;
    //GUID FTDI{ 0x219d0508, 0x57a8, 0x4ff5, {0x97, 0xa1, 0xbd, 0x86, 0x58, 0x7c, 0x6c, 0x7e}};

    void handleEvent(long wParam, PDEV_BROADCAST_DEVICEINTERFACE pDevInf);

    device getDevProp(HDEVINFO hDevInfo, SP_DEVINFO_DATA DeviceInfoData);

    device getDevProp(PDEV_BROADCAST_DEVICEINTERFACE pDevInf);

    QString getDLLpath(QString Mfg, QString reg);

    QByteArray getDeviceDesc(HDEVINFO hDevInfo, SP_DEVINFO_DATA DeviceInfoData, uint SPDRP);

    void checkType(device dev);

signals:
    // from ECU comm Devices Widget
    void ECU_deviceSelected(device);
    void ECU_deviceHasLeft(comm_device_interface*);

    // from ecuModelManagerWidget
    void ECU_ModelSelected(ecuModelType);
    void ECU_ProtoSelected(DMA_ProtoType);

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
