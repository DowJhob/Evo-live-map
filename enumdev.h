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
//#include <QtCore>

#define ELM_OPEN_PORT_13_INTERFACE 13;
#define ELM_OPEN_PORT_20_INTERFACE 20;

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

    bool VechicleInterfaceState;
    int VechicleInterfaceType;

    void NotifyRegister(HWND hwnd)
    {
        //Подписываемся на события

        DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
        ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );   //???
        NotificationFilter.dbcc_size = sizeof(NotificationFilter);
        NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
           for (int i = 0; i < interfaces.size(); i++)
        {
            NotificationFilter.dbcc_classguid = interfaces[i]; // подпишемся на все наши интерфейсы
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
                qDebug() << interfaces[i] << " event registered!!";
        }
    }

    //заглушка для прогона по всему вектору гуидов
    bool enumerateUSB_Device_by_guid()
    {
        for (int i = 0; i < interfaces.size(); i++)
        {
            if (enumerateUSB_Device_by_VID_PID(interfaces[i]))
            {
                VechicleInterfaceState = true;
                return true;
            }
        }
        VechicleInterfaceState = false;

        return false;
    }

signals:
    void InterfaceActive(int );
    void disconnectInterface();
private:
    QString VID = "VID_0403";//"VID_0403"
    QString PID_OP13 = "PID_CC4A";
    QString PID_OP20 = "PID_CC4C";
    QVector<GUID> interfaces = {//массив гуидов
                                //  { 0x36fc9e60, 0xc465, 0x11cf, {0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}}, //FTD Bus!!!  так тоже работает
                                { 0x4d36e978, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}},   //Serial and parralel ports!!!
                                { 0xfb1cf0c4, 0xb412, 0x451f, {0x9f, 0x04, 0xdf, 0x75, 0x37, 0xa5, 0x00, 0x3c}},  //OpenPorts20

                                //   { 0xa5dcbf10, 0x6530, 0x11d2, {0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed}},// USB Raw Device Interface Class GUID


                                //  { 0x745a17a0, 0x74d3, 0x11d0, {0xb6, 0xfe, 0x00, 0xa0, 0xc9, 0x0f, 0x57, 0xda}},  //hid from msdn

                                //  { 0xeec5ad98, 0x8080, 0x425f, {0x92, 0x2a, 0xda, 0xbf, 0x3d, 0xe3, 0xf6, 0x9a}},   //storage

                                //   { 0x378de44c, 0x56ef, 0x11d1, {0xbc, 0x8c, 0x00, 0xa0, 0xc9, 0x14, 0x05, 0xdd}},
                                //   { 0x4d1e55b2, 0xf16f, 0x11cf, {0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30}}
                               };
    HDEVNOTIFY NotificationHandle;
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;

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
            DeviceDesc(SPDRP_HARDWAREID);
            if (result.indexOf(VID, 0, Qt::CaseInsensitive) >= 0)
            {
                if (result.indexOf(PID_OP13, 0, Qt::CaseInsensitive) >= 0)
                    //если FTDI
                {
                    DeviceDesc(SPDRP_DEVICEDESC);
                    SetupDiDestroyDeviceInfoList(hDevInfo);
                    VechicleInterfaceType = ELM_OPEN_PORT_13_INTERFACE;
                    emit InterfaceActive(VechicleInterfaceType);
                    return true;
                }
                if (result.indexOf(PID_OP20, 0, Qt::CaseInsensitive) >= 0)
                {     //или OP2.0..
                    DeviceDesc(SPDRP_DEVICEDESC);
                    SetupDiDestroyDeviceInfoList(hDevInfo);
                    VechicleInterfaceType = ELM_OPEN_PORT_20_INTERFACE;
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
        result = result.fromRawData(propertyBuffer, 128) + "";//.left(128);
    }
};


#endif // ENUMDEV_H
