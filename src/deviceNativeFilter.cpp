#include "deviceNativeFilter.h"
#include <QSettings>

deviceNativeFilter::deviceNativeFilter()
{
    qRegisterMetaType<device>("device");
}

deviceNativeFilter::~deviceNativeFilter()
{
    if (NotificationHandle != nullptr)
        UnregisterDeviceNotification(NotificationHandle);
}

//Подписываемся на события
void deviceNativeFilter::notifyRegister(HWND hwnd)
{
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );   //???
    NotificationFilter.dbcc_size = sizeof(NotificationFilter);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

    for (auto classGUID: subscribeInterfaces)
    {
        NotificationFilter.dbcc_classguid = classGUID;
        NotificationFilter.dbcc_name[0] = '\0';
        NotificationHandle = RegisterDeviceNotification( hwnd,
                                                         &NotificationFilter,
                                                         //DEVICE_NOTIFY_ALL_INTERFACE_CLASSES
                                                         //|
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

bool deviceNativeFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
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

void deviceNativeFilter::handleEvent(long wParam, PDEV_BROADCAST_DEVICEINTERFACE pDevInf)
{
    switch(wParam)
    {
    case DBT_DEVICEREMOVECOMPLETE:{
        device dev = getDevProp(pDevInf);
        dev.direction = dir::remove;
        emit deviceEvent(dev);
    }break;
    case DBT_DEVICEARRIVAL:{
        device dev = getDevProp(pDevInf);
        dev.direction = dir::arrive;
        checkType(dev);
    }break;
    }
}

void deviceNativeFilter::getPresentCommDevices()
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
            checkType(dev);
        }
        SetupDiDestroyDeviceInfoList(hDevInfo);
    }
}

device deviceNativeFilter::getDevProp(HDEVINFO hDevInfo, SP_DEVINFO_DATA DeviceInfoData)
{
    // SPDRP_HARDWAREID
    // SPDRP_CLASS
    // SPDRP_MFG   //vendor

    device dev;
    QByteArray Mfg = getDeviceDesc(hDevInfo, DeviceInfoData, SPDRP_MFG);
    QByteArray DeviceInstanceId = getDeviceDesc(hDevInfo, DeviceInfoData, SPDRP_HARDWAREID);
    QByteArray DeviceDesc = getDeviceDesc(hDevInfo, DeviceInfoData, SPDRP_DEVICEDESC);
    QByteArray classDev2 = getDeviceDesc(hDevInfo, DeviceInfoData, SPDRP_CLASSGUID);
    QString classDev = QString::fromWCharArray( (wchar_t*)classDev2.data() );
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
    dev.classDev = DeviceInfoData.ClassGuid;

    // qDebug()<< "classDev"<<  classDev//.toHex(':')
    //        << "dev.classDev"<< dev.classDev;
    //<< "DeviceUniqueID"<<   dev.DeviceUniqueID
    //<< "DeviceInstanceId"<<  dev.DeviceInstanceId << "FunctionLibrary"<<   dev.FunctionLibrary << endl;
    return dev;
}

device deviceNativeFilter::getDevProp(PDEV_BROADCAST_DEVICEINTERFACE pDevInf)
{
    if(pDevInf == NULL)
        return device();
    QStringList qDevInf = QString::fromWCharArray((wchar_t*)pDevInf->dbcc_name).split('#');
    qDebug() << "deviceNativeFilter::getDevProp pDevInf->dbcc_name" << qDevInf << "pDevInf->dbcc_classguid" << pDevInf->dbcc_classguid << endl;
    if (qDevInf.length() >= 3)
    {
        QString DevType = qDevInf[0].mid(qDevInf[0].indexOf("?\\") + 2 );
//        if (qDevInf[0].contains("USB"))
        {
            QString DeviceInstanceId = qDevInf[1];
            QString DeviceUniqueID = qDevInf[2];
            QString reg = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Enum\\" + DevType + "\\" + DeviceInstanceId + "\\" + DeviceUniqueID;
            QSettings regKey(reg, QSettings::NativeFormat);



            QSettings paramKey(reg + "\\" + "Device Parameters", QSettings::NativeFormat);


            //qDebug()<< "DeviceInstanceId" << DeviceInstanceId;
            return device
            {
                regKey.value("Mfg", "").toString().split(';').at(1),
                        regKey.value("DeviceDesc", "").toString().split(';').at(1),
                        "",
                        DeviceInstanceId,
                        DeviceUniqueID,
                        pDevInf->dbcc_classguid,
                        paramKey.value("PortName", "").toString().split(';').at(1)
            };
        }
    }
    return device{};
}

QString deviceNativeFilter::getDLLpath(QString Mfg, QString reg)
{
    QString FunctionLibrary;
    QSettings m(QString(reg), QSettings::NativeFormat);
    const QStringList ak = m.childGroups();
    for(const QString &group: ak)
    {
        m.beginGroup(group);
        QString vendor = m.value("Vendor", "").toString();
        if( vendor == Mfg )
        {
            FunctionLibrary = m.value("FunctionLibrary", "").toString();
        }
        m.endGroup();
    }
    return FunctionLibrary;
}

QByteArray deviceNativeFilter::getDeviceDesc(HDEVINFO hDevInfo, SP_DEVINFO_DATA DeviceInfoData, uint SPDRP)
{
    DWORD requiredPropertySize = 0; // необходимый размер массива для свойства (указывается функцией)
    //  memset(propertyBuffer, 0, 256);
    SetupDiGetDeviceRegistryProperty( hDevInfo, &DeviceInfoData, SPDRP, nullptr, nullptr, 0, &requiredPropertySize );
    QByteArray a("\x00", requiredPropertySize);
    SetupDiGetDeviceRegistryProperty( hDevInfo, &DeviceInfoData, SPDRP, nullptr, (UCHAR*)a.data(), requiredPropertySize, nullptr );
    return a;
    //        qDebug() << "DeviceDesc: " << QString::fromWCharArray( propertyBuffer );
}

void deviceNativeFilter::checkType(device dev)
{
    //qDebug() << "enumerator::checkType start dev.classDev" << dev.classDev;
    //    if (pDevInf->dbcc_classguid == GUID({ 0x219d0508, 0x57a8, 0x4ff5, {0x97, 0xa1, 0xbd, 0x86, 0x58, 0x7c, 0x6c, 0x7e}})               // FTDI_D2XX_Device Class GUID
    //            || pDevInf->dbcc_classguid == GUID{ 0x6d1781b7, 0xc987, 0x4f6c, {0x8d, 0x4f, 0x1e, 0xfc, 0x09, 0x8b, 0xea, 0x67}} )  // проверим на соответствие тактриксу оп20
    dev.FunctionLibrary.clear();
    dev.FunctionLibrary = getDLLpath( dev.Mfg, reg64);
    if(!dev.FunctionLibrary.isEmpty())
    {
        if(dev.DeviceInstanceId.contains(tactrixOP20_DeviceInstanceId2) )
        { //
            dev.type =  deviceType::OP20;
        }
        else
        {
            dev.type =  deviceType::J2534;
        }
    }
    else if (dev.classDev == GUID{ 0x4d36e978, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}})       // Serial and parralel ports standart Windows class (seeng Openport1.3 (as serial port))
    {
        dev.type =  deviceType::SERIAL;
    }
    else if (dev.classDev == GUID{0x219d0508, 0x57a8, 0x4ff5, {0x97, 0xa1, 0xbd, 0x86, 0x58, 0x7c, 0x6c, 0x7e}}) //||      // FTDI_D2XX_Device Class GUID
        //(dev.classDev == GUID{ 0x86e0d1e0, 0x8089, 0x11d0, {0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73}}) ||            // FTDI_VCP_Device Class GUID
    {
        dev.type =  deviceType::FTDI;
    }
    else
        return;
    //qDebug() << "enumerator::checkType dev.type" << (int)dev.type << dev.DeviceUniqueID << dev.DeviceInstanceId + "/" + dev.DeviceDesc + "/" + dev.Mfg;
    emit deviceEvent(dev);

}
