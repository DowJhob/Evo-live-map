#ifndef ENUMDEV_H
#define ENUMDEV_H
#include <windows.h>
#include <wchar.h>

#include <dbt.h>
#include <setupapi.h>
#include <QObject>
#include <QVector>
//#include <Cfgmgr32.h>
#include <QUuid>
#include <QAbstractNativeEventFilter>
#include <QDebug>

#include "types.h"

//#define SERIAL_INTERFACE 13;
//#define J2534_INTERFACE 20;

#include <QSettings>

enum class dir
{
    arrive,
    remove
};

typedef struct device
{
    device(){}
    device(QString Mfg, QString DeviceDesc, QString FunctionLibrary, QString DeviceUniqueID):Mfg(Mfg), DeviceDesc(DeviceDesc),
        FunctionLibrary(FunctionLibrary), DeviceUniqueID(DeviceUniqueID)
    {

    }
    QString Mfg{};
    QString DeviceDesc = "No interface";
    QString FunctionLibrary{};
    QString DeviceInstanceId{};
    QString DeviceUniqueID{};
    dev_type type = dev_type::undef;
    dir direction;
}device;

class enumerator: public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:

    enumerator();
    ~enumerator();

//    QString interfaceName;
    bool isTactrix = false;
//    dev_type VechicleInterfaceType;
//    TCHAR DllLibraryPath[256];
//    WCHAR Vendor[256];
//    WCHAR deviceDesc[256];
    void NotifyRegister(HWND hwnd);

    //заглушка для прогона по всему вектору гуидов, нужна для запуска с подключенным устройством
    void getPresentCommDevices();

    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result ) override;


private:
    QString reg64 = "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\PassThruSupport.04.04";
    ///
    /// \brief Получаем производителя и описание при событиях
    /// \param pDevInf
    /// \return
    ///
    device getDevProp(PDEV_BROADCAST_DEVICEINTERFACE pDevInf);
    ///
    /// \brief Получаем производителя и описание при начальном перечислении
    /// \param pDevInf
    /// \return
    ///
    device getDevProp(HDEVINFO hDevInfo, SP_DEVINFO_DATA DeviceInfoData);

    device getJ2534DLLpath(device dev, QString reg );

    void handleEvent(long wParam, PDEV_BROADCAST_DEVICEINTERFACE pDevInf);
    ///=========================================================

    QString tactrixOP20_DeviceInstanceId = "VID_0403&PID_CC4C";
    QString tactrixOP13_DeviceInstanceId = "VID_0403&PID_CC4A";
#define Jsize 1
#define Ssize 1
    const GUID J2543_interfaces[Jsize] = {
        //       { 0x6d1781b7, 0xc987, 0x4f6c, {0x8d, 0x4f, 0x1e, 0xfc, 0x09, 0x8b, 0xea, 0x67}}  // кажется это загрузчик тактрикса для смены ПО tactrix VehiclePassThru j2534 ???????
        //{ 0x5a929f4c, 0x6f07, 0x426d, {0xa9, 0x70, 0x90, 0x3d, 0x25, 0xd4, 0x45, 0xb3}},   //raw usb device for Scanmatic??
        //      ,
        { 0xa5dcbf10, 0x6530, 0x11d2, {0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed}}   //raw usb device for Scanmatic
    };
    const GUID serial_interfaces[Ssize] = {
        //        { 0x4d36e978, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}}   //Serial and parralel ports change or add OP1.3
        //,
        {0x219d0508, 0x57a8, 0x4ff5, {0x97, 0xa1, 0xbd, 0x86, 0x58, 0x7c, 0x6c, 0x7e}}        // FTDI_D2XX_Device Class GUID
        //        ,{0x86e0d1e0L, 0x8089, 0x11d0, {0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}}           // FTDI_VCP_Device Class GUID
    };
    HDEVNOTIFY NotificationHandle = nullptr;
//    HDEVINFO hDevInfo;
//    SP_DEVINFO_DATA DeviceInfoData;
//    TCHAR propertyBuffer[512];       // массив для данных

    void getPresentCommDevices(GUID guid);

    void checkTactrix(QString DeviceInstanceId);

    QByteArray getDeviceDesc(HDEVINFO hDevInfo, SP_DEVINFO_DATA DeviceInfoData, uint SPDRP);

signals:
    void commDeviceEvent(device);


//    void InterfaceActive( dev_type, TCHAR*, bool isTactrix );
//    void disconnectInterface();
//    void Log(QString, int);

//    void _serial_arrived();
//    void _serial_removed();

};


#endif // ENUMDEV_H
