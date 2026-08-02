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

    commonGrpBxLayout.addWidget(&ECUcommDevManagerWidget,       0, 0);
    commonGrpBxLayout.addWidget(&_ecuModelManagerWidget, 1, 0);
    commonGrpBxLayout.addWidget(&logRateManagerWidget, 2, 0);
    commonGrpBxLayout.addWidget(&_wbManagerWidget,       3, 0);
    
    connect(&ECUcommDevManagerWidget,   &ECUcommDeviceManagerWidget::tactrixArrived,   &_wbManagerWidget,  &wbManagerWidget::addTactrix);
    connect(&ECUcommDevManagerWidget,   &ECUcommDeviceManagerWidget::tactrixRemoved,   &_wbManagerWidget,  &wbManagerWidget::removeTactrix);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *si = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    commonGrpBxLayout.addItem(si, 4, 0);

    makeConnection();

    HWND hwnd = reinterpret_cast<HWND>(winId());

    notifyRegister(hwnd);
}

commDevicesWidget::~commDevicesWidget(){}

bool commDevicesWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED( result )
    Q_UNUSED( eventType )

    device dev = handleEvent(static_cast<MSG*>(message));

    if(dev.type != deviceType::undef)
        if(dev.direction == dir::arrive)
            ECUcommDevManagerWidget.insertECU_Device(dev);
        else if(dev.direction == dir::remove)
            ECUcommDevManagerWidget.removeECU_Device(dev);


    return false;
    return QWidget::nativeEvent(eventType, message, result);
}

void commDevicesWidget::getPresentCommDevices()
{
    for (auto classGUID: subscribeInterfaces)
    {
        //qDebug()<< "enumerator::getPresentCommDevices";
        HDEVINFO hDevInfo;
        SP_DEVINFO_DATA DeviceInfoData;
        // Получаем указатель на множество устройств, присутствующих в системе
        hDevInfo = SetupDiGetClassDevs(&classGUID, nullptr, nullptr,
                                       DIGCF_PRESENT
                                           |DIGCF_DEVICEINTERFACE
                                       //|DIGCF_ALLCLASSES
                                       //|DIGCF_PROFILE
                                       );

        if ( hDevInfo == INVALID_HANDLE_VALUE )
        {
            qDebug() << "err create list dev";
            //     return false;
        }
        ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
        DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        // Перебор всех устройств из набора
        for (uint i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
        {
            device dev = getDevProp(hDevInfo, DeviceInfoData);
            //qDebug() << "enumerator::getPresentCommDevices" << dev.classDev << dev.DeviceInstanceId + "/" + dev.DeviceDesc + "/" + dev.Mfg;
            dev.direction = dir::arrive;
            checkType(&dev);
            //qDebug() << "enumerator::checkType dev.type" << (int)dev.type << dev.DeviceUniqueID << dev.DeviceInstanceId + "/" + dev.DeviceDesc + "/" + dev.Mfg;
            // emit deviceEvent(dev);
            if(dev.type != deviceType::undef)
                ECUcommDevManagerWidget.insertECU_Device(dev);
        }
        SetupDiDestroyDeviceInfoList(hDevInfo);
    }
}

void commDevicesWidget::fillECU_Models(QMap<ecuModelType, QString> *availECUmodels)
{
    _ecuModelManagerWidget.fillECU_Models(availECUmodels);
}

void commDevicesWidget::fillAvailProtos(QMap<DMA_ProtoType, QString> *availProtos)
{
    _ecuModelManagerWidget.fillAvailProtos(availProtos);
}

void commDevicesWidget::addTactrix(commDeviceWB *cdWB)
{
    _wbManagerWidget.addTactrix(cdWB);
}

void commDevicesWidget::removeTactrix(commDeviceWB *cdWB)
{
    _wbManagerWidget.removeTactrix(cdWB);
}

void commDevicesWidget::fillWB_Serial()
{
    _wbManagerWidget.fillSerial();
}

void commDevicesWidget::fillWB_Proto()
{
    _wbManagerWidget.fillProto();
}

void commDevicesWidget::devicePresentState()
{
    ECUcommDevManagerWidget.setEnabled(true);
    _ecuModelManagerWidget.setEnabled(true);
    logRateManagerWidget.setEnabled(true);
}

void commDevicesWidget::deviceLostState()
{
    ECUcommDevManagerWidget.setEnabled(false);
    _ecuModelManagerWidget.setEnabled(true);
    logRateManagerWidget.setEnabled(true);
    //        _wbManager.setEnabled(state);
}

void commDevicesWidget::connectedState()
{
    ECUcommDevManagerWidget.setEnabled(false);
    _ecuModelManagerWidget.setEnabled(false);
    logRateManagerWidget.setEnabled(true);
}

void commDevicesWidget::setEnabledWBcomm(bool state)
{
    _wbManagerWidget.setEnabled(state);
}

void commDevicesWidget::makeConnection()
{
    // from ECU comm Devices Widget
    connect(&ECUcommDevManagerWidget, &ECUcommDeviceManagerWidget::deviceSelected, this, &commDevicesWidget::ECU_deviceSelected);
    connect(&ECUcommDevManagerWidget, &ECUcommDeviceManagerWidget::ECU_deviceHasLeft, this, &commDevicesWidget::ECU_deviceHasLeft);
    connect(&ECUcommDevManagerWidget, &ECUcommDeviceManagerWidget::ECU_SetBaudRate, this, &commDevicesWidget::ECU_SetBaudRate);

    // from ecuModelManagerWidget
    connect(&_ecuModelManagerWidget, &ecuModelManagerWidget::modelSelected, this, &commDevicesWidget::ECU_ModelSelected);
    connect(&_ecuModelManagerWidget, &ecuModelManagerWidget::protoSelected, this, &commDevicesWidget::ECU_ProtoSelected);

    // from logRateManagerWidget
    connect(&logRateManagerWidget, &logRateManagerWidget::logRateChanged, this, &commDevicesWidget::logRateChanged);

    // from wbManagerWidget
    connect(&_wbManagerWidget,   &wbManagerWidget::wbSelected, this,  &commDevicesWidget::wbSelected);
    connect(&_wbManagerWidget,   &wbManagerWidget::protoSelected, this,  &commDevicesWidget::WB_ProtoSelected);
    connect(&_wbManagerWidget,   &wbManagerWidget::wbStart, this,  &commDevicesWidget::wbStart);
    // connect(&_wbManagerWidget,   &wbManagerWidget::logReady, this,  &commDevicesWidget::logReady);

}
