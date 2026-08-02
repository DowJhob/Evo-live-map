#ifndef FILTERHELPER_H
#define FILTERHELPER_H
#include <windows.h>
#include <wchar.h>
#include <vector>

#include <dbt.h>
#include <setupapi.h>
#include <QObject>
// #include <QVector>

#include <QUuid>
// #include <QAbstractNativeEventFilter>
// #include <QDebug>

#include "types.h"
// #include <QSettings>

#define TACTRIXOP20_DEVICEINSTANCEID   "VID_0403&PID_CC4C"
#define TACTRIXOP20_DEVICEINSTANCEID2  "VID_0403&PID_CC4D"
#define TACTRIX_OPENPORT_13_UNIVERSAL  "VID_0403&PID_CC4A"
#define TACTRIX_OPENPORT_13_MITSUBISHI "VID_0403&PID_CC48"
#define TACTRIX_OPENPORT_13_SUBARU     "VID_0403&PID_CC49"
#define TACTRIX_OPENPORT_13_ID2        "VID_0403&PID_6001"

typedef struct device
{
    device(){}
    device(QString Mfg,
           QString DeviceDesc,
           QString FunctionLibrary,
           QString DeviceInstanceId,
           QString DeviceUniqueID,
           GUID classDev,
           QString PortName):
        Mfg(Mfg),
        DeviceDesc(DeviceDesc),
        FunctionLibrary(FunctionLibrary),
        DeviceInstanceId(DeviceInstanceId),
        DeviceUniqueID(DeviceUniqueID),
        classDev(classDev),
        PortName(PortName)
    {

    }
    QString Mfg{};
    QString DeviceDesc = "No interface";
    QString FunctionLibrary{};
    QString DeviceInstanceId{};
    QString DeviceUniqueID{};
    deviceType type = deviceType::undef;
    dir direction;
    GUID classDev;
    QString PortName;
    uint baudRate;
}device;

// Q_DECLARE_METATYPE( device )

// class deviceNativeFilter: public QObject, public QAbstractNativeEventFilter
// {
    // Q_OBJECT

// public:
    // deviceNativeFilter();
    // ~deviceNativeFilter();
    extern void notifyRegister(HWND hwnd);
    //заглушка для прогона по всему вектору гуидов, нужна для запуска с подключенным устройством
    extern void getPresentCommDevices2();

// private:
    /// \brief Получаем производителя и описание при событиях
    /// \param pDevInf
    /// \return
    ///
    extern device getDevProp(PDEV_BROADCAST_DEVICEINTERFACE pDevInf);
    ///
    /// \brief Получаем производителя и описание при начальном перечислении
    /// \param pDevInf
    /// \return
    ///
    extern device getDevProp(HDEVINFO hDevInfo, SP_DEVINFO_DATA DeviceInfoData);

    extern QString getDLLpath(QString Mfg , QString reg);

    extern device handleEvent(MSG *pWindowsMessage);
    ///=========================================================

    extern QString reg;
    extern QString reg64;

    extern std::vector<GUID> subscribeInterfaces;
   // const std::vector<GUID> presentInterfaces{
   //     { 0xa5dcbf10, 0x6530, 0x11d2, {0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed}},
   //     { 0x4d36e978, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}}
   // };

   // GUID SERIAL{ 0x4d36e978, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}};
   // GUID J2534;
   // GUID FTDI{ 0x219d0508, 0x57a8, 0x4ff5, {0x97, 0xa1, 0xbd, 0x86, 0x58, 0x7c, 0x6c, 0x7e}};

    extern HDEVNOTIFY NotificationHandle;

    extern void checkType(device *dev);

    extern QByteArray getDeviceDesc(HDEVINFO hDevInfo, SP_DEVINFO_DATA DeviceInfoData, uint SPDRP);

// signals:
//     void deviceEvent(device);
//     void Log(QString);
// };

    Q_DECLARE_METATYPE( device );

#endif // FILTERHELPER_H
