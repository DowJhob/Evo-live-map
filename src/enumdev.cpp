#include "enumdev.h"
#include <QSettings>

enumerator::enumerator()
{
    qRegisterMetaType<device>("device");
}

enumerator::~enumerator()
{
    if (NotificationHandle != nullptr)
        UnregisterDeviceNotification(NotificationHandle);
}

void enumerator::NotifyRegister(HWND hwnd)
{
    //Подписываемся на события
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );   //???
    NotificationFilter.dbcc_size = sizeof(NotificationFilter);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    for (uint i = 0; i < Jsize; i++)
    {
        //         NotificationFilter.dbcc_classguid = {0xa5dcbf10, 0x6530, 0x11d2, {0x90, 0x1f, 0x0, 0xc0, 0x4f, 0xb9, 0x51, 0xed}} ; // подпишемся на все наши интерфейсы
        NotificationFilter.dbcc_classguid = J2543_interfaces[i];
        NotificationFilter.dbcc_name[0] = '\0';
        NotificationHandle = RegisterDeviceNotification( hwnd,
                                                         &NotificationFilter,
                                                         //DEVICE_NOTIFY_ALL_INTERFACE_CLASSES
                                                         DEVICE_NOTIFY_WINDOW_HANDLE
                                                         );
        if ( NotificationHandle == nullptr )
        {
            qDebug() << QString::fromWCharArray( NotificationFilter.dbcc_name) << "event not register!!";
        }
        //else
        //qDebug() << QString::fromWCharArray( NotificationFilter.dbcc_name) << "event registered!!";
    }
    for (int i = 0; i < Ssize; i++)
    {
        NotificationFilter.dbcc_classguid = serial_interfaces[i]; // подпишемся на все наши интерфейсы
        NotificationFilter.dbcc_name[0] = '\0';
        NotificationHandle = RegisterDeviceNotification( hwnd,
                                                         &NotificationFilter,
                                                         //DEVICE_NOTIFY_ALL_INTERFACE_CLASSES
                                                         DEVICE_NOTIFY_WINDOW_HANDLE
                                                         );
        if ( NotificationHandle == nullptr )
        {
            qDebug() << QString::fromWCharArray( NotificationFilter.dbcc_name) << "event not register!!";
        }
        //else
        //qDebug() << QString::fromWCharArray( NotificationFilter.dbcc_name) << "event registered!!";
    }
}

void enumerator::getPresentCommDevices()
{
    for (uint i = 0; i < Jsize; i++)
        //  if (
        getPresentCommDevices(J2543_interfaces[i]);
    //         )
    //        {
    //            VechicleInterfaceType = dev_type::J2534;
    //            emit InterfaceActive(VechicleInterfaceType, DllLibraryPath, isTactrix);
    //            emit Log(interfaceName, 0);
    //            return true;
    //        }
    for (int i = 0; i < Ssize; i++)
        // if (
        getPresentCommDevices(serial_interfaces[i]);
    //                )
    //        {
    //            VechicleInterfaceType = dev_type::OP13;
    //            emit InterfaceActive(VechicleInterfaceType, DllLibraryPath, isTactrix);
    //            emit Log(interfaceName, 0);
    //            return true;
    //        }

    // emit Log("No vechicle interface", 0);

    //emit disconnectInterface();
    //  return false;
}

void enumerator::getPresentCommDevices(GUID guid)
{
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    // Получаем указатель на множество устройств, присутствующих в системе
    hDevInfo = SetupDiGetClassDevs(&guid, nullptr, nullptr,  DIGCF_PRESENT|DIGCF_DEVICEINTERFACE );

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
        // SPDRP_HARDWAREID
        // SPDRP_CLASS
        // SPDRP_MFG   //vendor

        device dev = getJ2534DLLpath( getDevProp(hDevInfo, DeviceInfoData), reg64);
        if(!dev.FunctionLibrary.isEmpty())
        {
            //checkTactrix( QString::fromWCharArray( (wchar_t*)hwId.data() ).mid(4, 17));
            if(checkTactrix(dev.DeviceInstanceId))
                dev.type = deviceType::OP20;
            else
                dev.type = deviceType::J2534; //тут надо проверить вид пид на соответствие тактриксу, не забудь!!
            dev.direction = dir::arrive;
            emit commDeviceEvent(dev);
        }
    }
    SetupDiDestroyDeviceInfoList(hDevInfo);
}

bool enumerator::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED( result )
    Q_UNUSED( eventType )
    auto pWindowsMessage = static_cast<MSG*>(message);
    if(pWindowsMessage->message == WM_DEVICECHANGE)
    {
        auto wParam = pWindowsMessage->wParam;
        auto lParam = pWindowsMessage->lParam;
        if (lParam == 0 )
            return false;
        PDEV_BROADCAST_DEVICEINTERFACE pDevInf = reinterpret_cast<PDEV_BROADCAST_DEVICEINTERFACE>(lParam);
        if ( pDevInf->dbcc_devicetype == DBT_DEVTYP_DEVICEINTERFACE )
        {
            handleEvent(wParam, pDevInf);
            return true;
        }
    }
    return false;
}

void enumerator::handleEvent(long wParam, PDEV_BROADCAST_DEVICEINTERFACE pDevInf)
{
    switch(wParam)
    {
    case DBT_DEVICEREMOVECOMPLETE:{
        {
            device dev = getDevProp(pDevInf);
            dev.direction = dir::remove;
            emit commDeviceEvent(dev);
        }
    }break;
    case DBT_DEVICEARRIVAL:{
        device dev = getJ2534DLLpath( getDevProp(pDevInf), reg64);
        if(!dev.FunctionLibrary.isEmpty())
        {
            if(checkTactrix(dev.DeviceInstanceId))
                dev.type = deviceType::OP20;
            else
                dev.type = deviceType::J2534; //тут надо проверить вид пид на соответствие тактриксу, не забудь!!
            dev.direction = dir::arrive;
            emit commDeviceEvent(dev);
        }
    }break;
    }
}

device enumerator::getDevProp(PDEV_BROADCAST_DEVICEINTERFACE pDevInf)
{
    QStringList qDevInf = QString::fromWCharArray((wchar_t*)pDevInf->dbcc_name).split('#');
    //qDebug()<< "qDevInf" <<qDevInf;
    if (qDevInf.length() >= 3)
    {
        QString DevType = qDevInf[0].mid(qDevInf[0].indexOf("?\\") + 2 );
        QString DeviceInstanceId = qDevInf[1];
        QString DeviceUniqueID = qDevInf[2];
        QString reg = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Enum\\" + DevType + "\\" + DeviceInstanceId + "\\" + DeviceUniqueID;
        QSettings m(reg, QSettings::NativeFormat);
        return device
        {
            m.value("Mfg", "").toString().split(';').at(1),
                    m.value("DeviceDesc", "").toString().split(';').at(1),
                    "",
                    DeviceUniqueID
        };
    }
    return device{};
}

device enumerator::getDevProp(HDEVINFO hDevInfo, SP_DEVINFO_DATA DeviceInfoData)
{
    device dev;
    QByteArray Mfg = getDeviceDesc(hDevInfo, DeviceInfoData, SPDRP_MFG);
    QByteArray DeviceInstanceId = getDeviceDesc(hDevInfo, DeviceInfoData, SPDRP_HARDWAREID);
    QByteArray DeviceDesc = getDeviceDesc(hDevInfo, DeviceInfoData, SPDRP_DEVICEDESC);

    //QByteArray DeviceUniqueID = getDeviceDesc(hDevInfo, DeviceInfoData, 26);

    PTSTR buf = NULL;
    DWORD bufSize = 0;
    DWORD reqSize = 0;

    SetupDiGetDeviceInstanceId(hDevInfo, &DeviceInfoData, buf, bufSize, &reqSize);
    buf = new TCHAR[reqSize];
    bufSize = reqSize;
    SetupDiGetDeviceInstanceId(hDevInfo, &DeviceInfoData, buf, bufSize, &reqSize);

    dev.DeviceUniqueID = QString::fromWCharArray( (wchar_t*)buf ).split("\\").at(2);

    delete[] buf;

    dev.Mfg = QString::fromWCharArray( (wchar_t*)Mfg.data() );
    dev.DeviceDesc = QString::fromWCharArray( (wchar_t*)DeviceDesc.data() );
    dev.DeviceInstanceId = QString::fromWCharArray( (wchar_t*)DeviceInstanceId.data() );
    //QString::fromWCharArray( (wchar_t*)DeviceUniqueID.data() );
    dev.direction = dir::arrive;
    //qDebug()<< dev.DeviceInstanceId;

    //qDebug()<< "Mfg"<<  dev.Mfg;
    //qDebug()<< "DeviceDesc"<<   dev.DeviceDesc;
    //qDebug()<< "DeviceUniqueID"<<   dev.DeviceUniqueID;
    //qDebug()<< "DeviceInstanceId"<<  dev.DeviceInstanceId;
    //qDebug()<< "FunctionLibrary"<<   dev.FunctionLibrary << endl;
    return dev;
    return dev;
}

device enumerator::getJ2534DLLpath(device dev, QString reg)
{
    device _dev = dev;
    //qDebug() <<  "Mfg" << dev.Mfg;
    //qDebug() <<  "DeviceDesc" << dev.DeviceDesc;
    QSettings m(QString(reg), QSettings::NativeFormat);
    const QStringList ak = m.childGroups();
    for(const QString &group: ak)
    {
        m.beginGroup(group);
        QString vendor = m.value("Vendor", "").toString();
        if( vendor == dev.Mfg )
        {
            dev.FunctionLibrary = m.value("FunctionLibrary", "").toString();
        }
        m.endGroup();
    }
    return dev;
}

QByteArray enumerator::getDeviceDesc(HDEVINFO hDevInfo, SP_DEVINFO_DATA DeviceInfoData, uint SPDRP)
{
    DWORD requiredPropertySize = 0; // необходимый размер массива для свойства (указывается функцией)
    //  memset(propertyBuffer, 0, 256);
    SetupDiGetDeviceRegistryProperty( hDevInfo, &DeviceInfoData, SPDRP, nullptr, nullptr, 0, &requiredPropertySize );
    QByteArray a("\x00", requiredPropertySize);
    SetupDiGetDeviceRegistryProperty( hDevInfo, &DeviceInfoData, SPDRP, nullptr, (UCHAR*)a.data(), requiredPropertySize, nullptr );
    return a;
    //        qDebug() << "DeviceDesc: " << QString::fromWCharArray( propertyBuffer );
}

bool enumerator::checkTactrix(QString DeviceInstanceId)
{
    //qDebug() << "HardwareID: " << DeviceInstanceId;
    return DeviceInstanceId.contains(tactrixOP20_DeviceInstanceId2);
}
