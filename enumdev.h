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


#define SERIAL_INTERFACE 13;
#define J2534_INTERFACE 20;

class enumerator: public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:

    enumerator()
    {

    }
    ~enumerator()
    {
        if (NotificationHandle != nullptr)
            UnregisterDeviceNotification(NotificationHandle);
    }

    QString interfaceName;
    bool isTactrix = false;
    int VechicleInterfaceType;
    TCHAR DllLibraryPath[256];
    WCHAR Vendor[256];
    WCHAR deviceDesc[256];
    void NotifyRegister(HWND hwnd)
    {
        //Подписываемся на события
        DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
        ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );   //???
        NotificationFilter.dbcc_size = sizeof(NotificationFilter);
        NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
        for (int i = 0; i < J2543_interfaces.size(); i++)
        {
            //         NotificationFilter.dbcc_classguid = {0xa5dcbf10, 0x6530, 0x11d2, {0x90, 0x1f, 0x0, 0xc0, 0x4f, 0xb9, 0x51, 0xed}} ; // подпишемся на все наши интерфейсы
            NotificationFilter.dbcc_classguid = J2543_interfaces.at(i);
            NotificationFilter.dbcc_name[0] = '\0';
            NotificationHandle = RegisterDeviceNotification( hwnd,
                                                             &NotificationFilter,
                                                             //DEVICE_NOTIFY_ALL_INTERFACE_CLASSES
                                                             DEVICE_NOTIFY_WINDOW_HANDLE
                                                             );
            if ( NotificationHandle == nullptr )
            {
                qDebug() << " event not register!!";
            }
            else
                qDebug() <<  " event registered!!";
        }
        //        for (int i = 0; i < serial_interfaces.size(); i++)
        //        {
        //            NotificationFilter.dbcc_classguid = serial_interfaces[i]; // подпишемся на все наши интерфейсы
        //            NotificationFilter.dbcc_name[0] = '\0';
        //            NotificationHandle = RegisterDeviceNotification( hwnd,
        //                                                             &NotificationFilter,
        //                                                             //DEVICE_NOTIFY_ALL_INTERFACE_CLASSES
        //                                                             DEVICE_NOTIFY_WINDOW_HANDLE
        //                                                             );
        //            if ( NotificationHandle == nullptr )
        //            {
        //                qDebug() << " event not register!!";
        //            }
        //            else
        //                qDebug() << &serial_interfaces[i] << " event registered!!";
        //        }
    }

    //заглушка для прогона по всему вектору гуидов, нужна для запуска с подключенным устройством
    bool enumerateUSB_Device_by_guid()
    {
        for (int i = 0; i < J2543_interfaces.size(); i++)
            if (checkGUID(J2543_interfaces[i]))
            {
                VechicleInterfaceType = J2534_INTERFACE;
                emit InterfaceActive(VechicleInterfaceType, DllLibraryPath, isTactrix);
                emit Log(interfaceName);
                return true;
            }
        for (int i = 0; i < serial_interfaces.size(); i++)
            if (checkGUID(serial_interfaces[i]))
            {
                VechicleInterfaceType = SERIAL_INTERFACE;
                emit InterfaceActive(VechicleInterfaceType, DllLibraryPath, isTactrix);
                emit Log(interfaceName);
                return true;
            }

        emit Log("No vechicle interface");

        emit disconnectInterface();
        return false;
    }

    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result ) Q_DECL_OVERRIDE
    {
        Q_UNUSED( result )
        Q_UNUSED( eventType )

        auto pWindowsMessage = static_cast<MSG*>(message);
        if(pWindowsMessage->message == WM_DEVICECHANGE)
        {
            auto wParam = pWindowsMessage->wParam;
            auto lParam = pWindowsMessage->lParam;
            PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;
            switch(wParam)
            {
            case DBT_DEVICEREMOVECOMPLETE:{
                if ( pDevInf->dbcc_devicetype == DBT_DEVTYP_DEVICEINTERFACE )
                {
                    //                    qDebug() << "removal: " << QString::fromWCharArray((wchar_t*)pDevInf->dbcc_name) ;
                    if ( checkDev(pDevInf) )
                    {
                        emit disconnectInterface();
                        emit Log("No vechicle interface");
                        return false;
                    }
                }
            }break;

            case DBT_DEVICEARRIVAL:{
                if ( pDevInf->dbcc_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                {
                    //                    qDebug() << "arrival: " << QString::fromWCharArray((wchar_t*)pDevInf->dbcc_name) ;
                    if ( checkDev(pDevInf) )
                    {
                        emit Log(interfaceName);
                        emit InterfaceActive(VechicleInterfaceType, DllLibraryPath, isTactrix);
                    }
                }
            }break;
            }
        }
        return false;
    }

signals:
    void InterfaceActive( int, TCHAR*, bool isTactrix );
    void disconnectInterface();
    void Log(QString);

private:
    QString tactrixOP20_DeviceInstanceId = "VID_0403&PID_CC4C";
    QString tactrixOP13_DeviceInstanceId = "VID_0403&PID_CC4A";
    QVector<GUID> J2543_interfaces = {
        //       { 0xfb1cf0c4, 0xb412, 0x451f, {0x9f, 0x04, 0xdf, 0x75, 0x37, 0xa5, 0x00, 0x3c}},  // VehiclePassThru j2534 class adapter?
        { 0x6d1781b7, 0xc987, 0x4f6c, {0x8d, 0x4f, 0x1e, 0xfc, 0x09, 0x8b, 0xea, 0x67}}  // tactrix VehiclePassThru j2534 ???????
        //        { 0x5a929f4c, 0x6f07, 0x426d, {0xa9, 0x70, 0x90, 0x3d, 0x25, 0xd4, 0x45, 0xb3}},   //raw usb device for Scanmatic??
        ,{ 0xa5dcbf10, 0x6530, 0x11d2, {0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed}}   //raw usb device for Scanmatic
    };
    QVector<GUID> serial_interfaces = {
        { 0x4d36e978, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}}   //Serial and parralel ports change or add OP1.3
        //        ,{0x219d0508, 0x57a8, 0x4ff5, {0x97, 0xa1, 0xbd, 0x86, 0x58, 0x7c, 0x6c, 0x7e}}        // FTDI_D2XX_Device Class GUID
        //        ,{0x86e0d1e0L, 0x8089, 0x11d0, {0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}}           // FTDI_VCP_Device Class GUID
    };
    HDEVNOTIFY NotificationHandle;
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    TCHAR propertyBuffer[512];       // массив для данных

    bool checkGUID(GUID guid)
    {
        // Получаем указатель на множество устройств, присутствующих в системе
        hDevInfo = SetupDiGetClassDevs(&guid, nullptr, nullptr,  DIGCF_PRESENT|DIGCF_DEVICEINTERFACE );
        if ( hDevInfo == INVALID_HANDLE_VALUE )
        {
            qDebug() << "err create list dev";
        }
        ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
        DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        // Перебор всех устройств из набора
        for (uint i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
        {
            // SPDRP_HARDWAREID
            // SPDRP_CLASS
            // SPDRP_MFG   //vendor
            DeviceDesc(SPDRP_MFG);
            if ( get_dll_path_j2534(propertyBuffer) )
            {
                DeviceDesc(SPDRP_HARDWAREID);
                checkTactrix( QString::fromWCharArray( propertyBuffer ).mid(4, 17));
                DeviceDesc(SPDRP_DEVICEDESC);
                interfaceName = interfaceName.fromWCharArray(propertyBuffer);
                return true;
            }
        }
        SetupDiDestroyDeviceInfoList(hDevInfo);
        return false;
    }

    bool checkDev(PDEV_BROADCAST_DEVICEINTERFACE pDevInf)
    {

        WCHAR* t = getVendorDesc_frRegistry(pDevInf);

        //pDevInf->dbcc_classguid

        if ( pDevInf->dbcc_classguid == J2543_interfaces.at(0) )
        {
            VechicleInterfaceType = 20;    //J2534_INTERFACE
            if ( get_dll_path_j2534( t ) )
            {

                return true;
            }
        }
        else
        {
            VechicleInterfaceType = 13;   //SERIAL_INTERFACE
            if ( get_dll_path_ftdi( t ) )
            {

                return true;
            }
        }
        return false;
    }

    bool get_dll_path_j2534(TCHAR *vendor_name)
    {
        //        qDebug() << "input vendor_name: " << vendor_name;

        HKEY  childKEY, sub_child;
        DWORD KeyType, KeySize = 256;

        DWORD lpcnumOfValues;
        DWORD lpcmaxValueNameLen;
        DWORD lpcnumOfSubKeys;
        DWORD lpcmaxSubKeyLen;
        DWORD lpcmaxValueLen;
        //================= прокрутим до нужного раздела ====================
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\PassThruSupport.04.04", 0, KEY_READ , &childKEY) == ERROR_SUCCESS)
        {        //=========== найдем подразделы ====================
            RegQueryInfoKey(childKEY, NULL, NULL, NULL, &lpcnumOfSubKeys, &lpcmaxSubKeyLen, NULL, &lpcnumOfValues, &lpcmaxValueNameLen, &lpcmaxValueLen, NULL, NULL);
            //qDebug() << " lpcnumOfSubKeys: " << lpcnumOfSubKeys << " lpcmaxSubKeyLen: " << lpcmaxSubKeyLen << " lpcnumOfValues: " << lpcnumOfValues << " lpcmaxValueNameLen: " << lpcmaxValueNameLen;
            TCHAR *SKName = new TCHAR[lpcmaxSubKeyLen];
            // check SubKey's
            for (int i = 0; i < (int)lpcnumOfSubKeys; i++)
            {
                DWORD keyNameLen = lpcmaxSubKeyLen + 1;
                RegEnumKeyEx(childKEY, i, SKName, &keyNameLen, NULL, NULL, NULL, NULL);
                //qDebug() << "subgroup: " << QString::fromWCharArray(SKName);
                {
                    if (RegOpenKeyEx(childKEY, SKName, 0, KEY_READ, &sub_child) == ERROR_SUCCESS)           //открыли ключ подраздел с каким то вендором
                    {
                        KeySize = 256;
                        if (RegQueryValueEx(sub_child, L"Vendor", 0, &KeyType, (uchar*)DllLibraryPath, &KeySize) == ERROR_SUCCESS)
                        {
                            if ( wcsstr ( DllLibraryPath, vendor_name ) != 0 )
                            {
                                KeySize = 256;                                                              //MAGICK
                                if (RegQueryValueEx(sub_child, L"FunctionLibrary", 0, &KeyType, (uchar*)DllLibraryPath, &KeySize) == ERROR_SUCCESS)
                                {
                                    //                                    qDebug() << "DllLibraryPath: " << QString::fromWCharArray(DllLibraryPath);  //в DllLibraryPath собственно путь до драйвера!
                                    RegCloseKey(childKEY);
                                    RegCloseKey(sub_child);
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

    bool get_dll_path_ftdi(TCHAR *vendor_name)
    {
        qDebug() << "ffff: ";
        memcpy(DllLibraryPath, L"ftd2xx.dll", 512);

        return true;
    }

    void checkTactrix(QString DeviceInstanceId)
    {
        isTactrix = false;
        if ( DeviceInstanceId.mid(0, 17)  == tactrixOP20_DeviceInstanceId )
            isTactrix = true;
        else
            isTactrix = false;
        qDebug() << "HardwareID: " << DeviceInstanceId << isTactrix;
    }

    void DeviceDesc(uint SPDRP)
    {
        DWORD requiredPropertySize = 0; // необходимый размер массива для свойства (указывается функцией)
        //  memset(propertyBuffer, 0, 256);
        SetupDiGetDeviceRegistryProperty( hDevInfo,&DeviceInfoData, SPDRP, nullptr, nullptr, 0, &requiredPropertySize );
        SetupDiGetDeviceRegistryProperty( hDevInfo,&DeviceInfoData, SPDRP, nullptr, (UCHAR*)propertyBuffer, requiredPropertySize, nullptr );
        //        qDebug() << "DeviceDesc: " << QString::fromWCharArray( propertyBuffer );
    }





    WCHAR* getVendorDesc_frRegistry(PDEV_BROADCAST_DEVICEINTERFACE pDevInf)
    {
        DWORD KeyType, KeySize = 256;
        QStringList Parts = QString::fromWCharArray((wchar_t*)pDevInf->dbcc_name).split('#');
        if (Parts.length() >= 3)
        {
            QString DevType = Parts[0].mid(Parts[0].indexOf("?\\") + 2 );
            QString DeviceInstanceId = Parts[1];

            checkTactrix( DeviceInstanceId);

            QString DeviceUniqueID = Parts[2];
            QString RegPath = "SYSTEM\\CurrentControlSet\\Enum\\" + DevType + "\\" + DeviceInstanceId + "\\" + DeviceUniqueID;
            HKEY key;
            //qDebug() << "RegPath: " << RegPath;
            if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, (WCHAR*)RegPath.utf16(), 0, KEY_READ , &key) == ERROR_SUCCESS)
            {
                KeySize = 256;
                if ( RegQueryValueEx(key, L"Mfg", 0, &KeyType, (uchar*)Vendor, &KeySize) == ERROR_SUCCESS)
                {
                    WCHAR* vendor = wcstok (Vendor, L";");
                    vendor = wcstok (NULL, L";");
                    qDebug() << "Vendor: " << QString::fromWCharArray(vendor) ;

                    KeySize = 512;
                    if ( RegQueryValueEx(key, L"DeviceDesc", 0, &KeyType, (uchar*)deviceDesc, &KeySize) == ERROR_SUCCESS)
                    {
                        WCHAR *t = wcstok (deviceDesc, L";");
                        t = wcstok (NULL, L";");
                        interfaceName = interfaceName.fromWCharArray(t) ;
                        qDebug() << "DeviceDesc: " <<    interfaceName;
                    }
                    return vendor;
                }
            }
        }
        return nullptr;
    }







};


#endif // ENUMDEV_H
