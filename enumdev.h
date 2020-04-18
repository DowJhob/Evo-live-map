#ifndef ENUMDEV_H
#define ENUMDEV_H
#include <windows.h>
#include <dbt.h>
#include <setupapi.h>
#include <QObject>
#include <QVector>

#include <QUuid>
#include <QString>
#include <QDebug>
//#include <QSettings>

#define SERIAL_INTERFACE 13;
#define J2534_INTERFACE 20;

class enumerator:public QObject
{
    Q_OBJECT

public:

    enumerator()
    {}
    ~enumerator()
    {
        if (NotificationHandle != nullptr)
            UnregisterDeviceNotification(NotificationHandle);
    }

    QString result;
    QString dllName;
    bool VechicleInterfaceState;
    int VechicleInterfaceType;
    TCHAR DllLibraryPath[256];
    void NotifyRegister(HWND hwnd)
    {
        //Подписываемся на события

        DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
        ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );   //???
        NotificationFilter.dbcc_size = sizeof(NotificationFilter);
        NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
        for (int i = 0; i < J2543_interfaces.size(); i++)
        {
            NotificationFilter.dbcc_classguid = J2543_interfaces[i]; // подпишемся на все наши интерфейсы
            NotificationFilter.dbcc_name[0] = '\0';
            NotificationHandle = RegisterDeviceNotification( hwnd,
                                                             &NotificationFilter,
                                                             //DEVICE_NOTifY_ALL_INTERFACE_CLASSES
                                                             DEVICE_NOTIFY_WINDOW_HANDLE
                                                             );
            if ( NotificationHandle == nullptr )
            {
                qDebug() << " event not register!!";
            }
            else
                qDebug() << J2543_interfaces[i] << " event registered!!";
        }
        for (int i = 0; i < serial_interfaces.size(); i++)
        {
            NotificationFilter.dbcc_classguid = serial_interfaces[i]; // подпишемся на все наши интерфейсы
            NotificationFilter.dbcc_name[0] = '\0';
            NotificationHandle = RegisterDeviceNotification( hwnd,
                                                             &NotificationFilter,
                                                             //DEVICE_NOTifY_ALL_INTERFACE_CLASSES
                                                             DEVICE_NOTIFY_WINDOW_HANDLE
                                                             );
            if ( NotificationHandle == nullptr )
            {
                qDebug() << " event not register!!";
            }
            else
                qDebug() << serial_interfaces[i] << " event registered!!";
        }
    }

    //заглушка для прогона по всему вектору гуидовб нужна для запуска с подключенным устройством
    bool enumerateUSB_Device_by_guid()
    {



        for (int i = 0; i < J2543_interfaces.size(); i++)
        {
            //            if (enumerateUSB_Device_by_VID_PID(J2543_interfaces[i]))
            if (checkGUID(J2543_interfaces[i]))
            {
                VechicleInterfaceType = J2534_INTERFACE;
                //     get_dll_path2("Tactrix");
                emit InterfaceActive(VechicleInterfaceType);
                VechicleInterfaceState = true;

                return true;
            }
        }
        for (int i = 0; i < serial_interfaces.size(); i++)
        {
            //            if (enumerateUSB_Device_by_VID_PID(J2543_interfaces[i]))
            if (checkGUID(serial_interfaces[i]))
            {
                VechicleInterfaceType = SERIAL_INTERFACE;
                emit InterfaceActive(VechicleInterfaceType);
                VechicleInterfaceState = true;
                return true;
            }
        }
        SetupDiDestroyDeviceInfoList(hDevInfo);
        result = "No vechicle interface";
        VechicleInterfaceState = false;
        emit disconnectInterface();
        return false;
    }

signals:
    void InterfaceActive(int );
    void disconnectInterface();

private:
    QString VID_tactrix = "VID_0403";
    QString PID_OP13 = "PID_CC4A";
    QString PID_OP20 = "PID_CC4C";
    QVector<GUID> J2543_interfaces = {//массив гуидов
                                      { 0xfb1cf0c4, 0xb412, 0x451f, {0x9f, 0x04, 0xdf, 0x75, 0x37, 0xa5, 0x00, 0x3c}},  // VehiclePassThru j2534 class adapter?
                                      { 0x5a929f4c, 0x6f07, 0x426d, {0xa9, 0x70, 0x90, 0x3d, 0x25, 0xd4, 0x45, 0xb3}}   //Scanmatic
                                     };
    QVector<GUID> serial_interfaces = {//массив гуидов
                                       { 0x4d36e978, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}},   //Serial and parralel ports change or add OP1.3
                                      };
    HDEVNOTIFY NotificationHandle;
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;

    bool get_dll_path2(QString vendor_name)
    {
        qDebug() << "input vendor_name: " << vendor_name;

        HKEY  childKEY, sub_child;
        DWORD KeyType, KeySize = 300;

        DWORD lpcnumOfValues;
        DWORD lpcmaxValueNameLen;
        DWORD lpcnumOfSubKeys;
        DWORD lpcmaxSubKeyLen;
        DWORD lpcmaxValueLen;
        //================= прокрутим до нужного раздела ====================
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\PassThruSupport.04.04", 0, KEY_READ , &childKEY) == ERROR_SUCCESS)
        {        //=========== найдем подразделы ====================
            RegQueryInfoKey(childKEY, NULL, NULL, NULL, &lpcnumOfSubKeys, &lpcmaxSubKeyLen, NULL, &lpcnumOfValues, &lpcmaxValueNameLen, &lpcmaxValueLen, NULL, NULL);
            qDebug() << " lpcnumOfSubKeys: " << lpcnumOfSubKeys << " lpcmaxSubKeyLen: " << lpcmaxSubKeyLen << " lpcnumOfValues: " << lpcnumOfValues << " lpcmaxValueNameLen: " << lpcmaxValueNameLen;
            TCHAR *SKName = new TCHAR[lpcmaxSubKeyLen];
            // check SubKey's
            for (int i = 0; i < (int)lpcnumOfSubKeys; i++)
            {
                DWORD keyNameLen = lpcmaxSubKeyLen + 1;
                RegEnumKeyEx(childKEY, i, SKName, &keyNameLen, NULL, NULL, NULL, NULL);
                qDebug() << "subgroup: " << QString::fromWCharArray(SKName);
                {
                    if (RegOpenKeyEx(childKEY, SKName, 0, KEY_READ, &sub_child) == ERROR_SUCCESS)           //открыли ключ подраздел с каким то вендором
                    {
                        KeySize = 300;
                        if (RegQueryValueEx(sub_child, L"Vendor", 0, &KeyType, (uchar*)DllLibraryPath, &KeySize) == ERROR_SUCCESS)
                        {
                            qDebug() << "Vendor: " << QString::fromWCharArray(DllLibraryPath) ;             //в DllLibraryPath имя вендора!
                            if ( QString::fromWCharArray(DllLibraryPath).indexOf( vendor_name ) >= 0 )
                            {
                                KeySize = 256;                                                              //MAGICK
                                if (RegQueryValueEx(sub_child, L"FunctionLibrary", 0, &KeyType, (uchar*)DllLibraryPath, &KeySize) == ERROR_SUCCESS)
                                {
                                    qDebug() << "DllLibraryPath: " << QString::fromWCharArray(DllLibraryPath);
                                    return true;
                                }
                                else
                                    qDebug() << "not open: ";
                            }
                        }
                        RegCloseKey(sub_child);
                    }
                    else
                    {
                        RegCloseKey(childKEY);
                        return false;
                    }
                }
            }
        }
        return false;
    }

    bool checkGUID(GUID guid)                               //Следим за нашими интерфейсами
    {
        // Получаем указатель на множество устройств, присутствующих в системе
        hDevInfo = SetupDiGetClassDevs(&guid, nullptr, nullptr,  DIGCF_PRESENT );
        if ( hDevInfo == INVALID_HANDLE_VALUE )
        {
            qDebug() << "err create list dev";
        }
        ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
        DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

        // Перебор всех устройств из набора
        for (uint i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
        {
            //DeviceDesc(SPDRP_HARDWAREID);
            //DeviceDesc(SPDRP_CLASS);
            DeviceDesc(SPDRP_MFG);   //vendor
            //qDebug() << "SPDRP_: " << result;
            if ( !get_dll_path2(result) )
                return false;


            DeviceDesc(SPDRP_DEVICEDESC);
            SetupDiDestroyDeviceInfoList(hDevInfo);
            if (!result.isEmpty())
                return true;
        }
        return false;
    }

    bool enumerateUSB_Device_by_VID_PID(GUID guid)                               //Следим за нашими интерфейсами
    {
        // Получаем указатель на множество устройств, присутствующих в системе

        hDevInfo = SetupDiGetClassDevs(&guid, nullptr, nullptr,  DIGCF_PRESENT );

        if ( hDevInfo == INVALID_HANDLE_VALUE )
        {
            //    return "err create list dev";
        }
        ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
        DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        // Перебор всех устройств из набора
        for (uint i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
        {
            //получим vid pid
            DeviceDesc(SPDRP_HARDWAREID);  //vid pid
            DeviceDesc(SPDRP_MFG);   //vendor

            DeviceDesc(SPDRP_CLASS);
            qDebug() << "SPDRP_CLASS: " << result;

            if (result.indexOf(VID_tactrix, 0, Qt::CaseInsensitive) >= 0)
            {
                if (result.indexOf(PID_OP13, 0, Qt::CaseInsensitive) >= 0)
                    //если FTDI
                {
                    DeviceDesc(SPDRP_DEVICEDESC);
                    SetupDiDestroyDeviceInfoList(hDevInfo);
                    VechicleInterfaceType = SERIAL_INTERFACE;
                    emit InterfaceActive(VechicleInterfaceType);
                    return true;
                }
                if (result.indexOf(PID_OP20, 0, Qt::CaseInsensitive) >= 0)
                {     //или OP2.0..
                    DeviceDesc(SPDRP_DEVICEDESC);
                    SetupDiDestroyDeviceInfoList(hDevInfo);
                    VechicleInterfaceType = J2534_INTERFACE;
                    emit InterfaceActive(VechicleInterfaceType);
                    return true;
                }
            }
        }
        SetupDiDestroyDeviceInfoList(hDevInfo);
        result = "No vechicle interface";
        emit disconnectInterface();
        return false;
    }

    void DeviceDesc(uint SPDRP)
    {
        QChar propertyBuffer[128];       // массив для данных
        DWORD requiredPropertySize = 0; // необходимый размер массива для свойства (указывается функцией)
        //  memset(propertyBuffer, 0, 256);
        SetupDiGetDeviceRegistryProperty( hDevInfo,&DeviceInfoData, SPDRP, nullptr, nullptr, 0, &requiredPropertySize );
        SetupDiGetDeviceRegistryProperty( hDevInfo,&DeviceInfoData, SPDRP, nullptr, (UCHAR*)propertyBuffer, requiredPropertySize, nullptr );
        result = result.fromWCharArray((wchar_t*)propertyBuffer) ;//.left(128);
    }
};


#endif // ENUMDEV_H
