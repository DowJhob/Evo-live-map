#include "op13.h"
#include <QCoreApplication>

OP13::OP13(QString dllName, QString DeviceDesc, QString DeviceUniqueID) :  comm_device_interface( dllName, DeviceDesc, DeviceUniqueID)
{
    //devType = deviceType::OP13;
    _ftdi = new ftdi(QCoreApplication::applicationDirPath() + "\\ftd2xx.dll");
    p_in_buff = in_buf;
    p_out_buff = out_buf;
    //qDebug() << "OP13" << DeviceUniqueID;
    info();
}

OP13::~OP13()
{
    delete _ftdi;
}

bool OP13::info()
{
    DWORD numDevs =  1;
    FT_DEVICE_LIST_INFO_NODE *devInfo;
    if (_ftdi->FT_Open(0, &_ftdi->ftHandle) == FT_OK)
    {
        //   error_out = "FT_Open OK";
        // FT_Open OK, use ftHandle to access device

        // create the device information list
        _ftdi->ftStatus = _ftdi->FT_CreateDeviceInfoList(&numDevs);
        if (_ftdi->ftStatus == FT_OK) {
            //         qDebug() << "Number of devices is" << numDevs;
        }

        // allocate storage for list based on numDevs
        devInfo = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
        // get the device information list
        _ftdi->ftStatus = _ftdi->FT_GetDeviceInfoList(devInfo,&numDevs);
        // qDebug() << "ftStatus: " << _ftdi->ftStatus;
        if (_ftdi->ftStatus == FT_OK)
        {
            for (uint i = 0; i < numDevs; i++) {
                QString info = //"Dev: " + QString::number( i ) +
                        "  SerialNumber: " + QString::fromLatin1( devInfo[i].SerialNumber) +
                        "  Description: " + QString::fromLatin1(devInfo[i].Description);
                // qDebug() << "info: " << info;
                //emit Log(info);
            }
        }
    }
    close();
    return true;
}

bool OP13::open(Protocol protocol, enum ConnectFlag ConnectFlag, uint baudRate)
{
    this->protocol = protocol;
    //this->baudRate = baudRate;

    p_in_buff = in_buf;
    p_out_buff = out_buf;

    ulong s;
    _ftdi->ftHandle=0;
    if ( (s = _ftdi->FT_Open(0, &_ftdi->ftHandle) == FT_OK))
    {
        //   error_out = "FT_Open OK";
        // FT_Open OK, use ftHandle to access device
        s = _ftdi->ftStatus = _ftdi->FT_ResetDevice(_ftdi->ftHandle);
        s = _ftdi->ftStatus = _ftdi->FT_Purge(_ftdi->ftHandle, FT_PURGE_RX | FT_PURGE_TX);
        s = _ftdi->ftStatus = _ftdi->FT_SetTimeouts(_ftdi->ftHandle, 20000, 2500);
        s = _ftdi->ftStatus = _ftdi->FT_SetLatencyTimer(_ftdi->ftHandle, 1);
        s = _ftdi->ftStatus = _ftdi->FT_SetBaudRate(_ftdi->ftHandle, baudRate);
        qDebug() << " FT_Open" <<  s;
        //emit Log(" FT_Open status " + QString::number(s));
        return true;
    }
    //emit Log(" FT_Open failed status " + QString::number(s));
    qDebug() << " FT_Open failed" << s << QString::fromLatin1( _ftdi->getLastError());



    return false;
}

bool OP13::close()
{
    ulong s = _ftdi->FT_Close( _ftdi->ftHandle);
    //   qDebug() << " close" << s;
    //emit Log(" FT_Close status " + QString::number(s));
    return true;
}

bool OP13::five_baud_init()
{
    qDebug() << "Five baud init";
    ftdi_low_baud_sender(5, 0x00);                                 //5 baud, 0x00 ecu addr, 0x05 TCU?
    //QThread::msleep(300);                             // W1 60 - 300ms
    //Get bytes waiting to be read
    QByteArray a = read(4);

    QString aa = a.toHex(':');

    qDebug() << "FT_five_baud_ response size" << a.size() << "response" << aa;
    //emit Log("FT_five_baud_ response size " + QString::number(a.size())  + " / response " + aa);

    if (a.size() < 4)
    {
        qDebug() << "FT five_baud failed";
        //emit Log("FT_ five_baud failed: ");
        return false;
    }



    qDebug() << "FT_five_baud_OK" << aa;
    //emit Log("FT_five_baud_OK: " + aa);
    return true;
}

QByteArray OP13::read(uint lenght)
{
    //Get bytes waiting to be read
    DWORD FT_RxQ_Bytes;
    do
    {
        _ftdi->FT_GetQueueStatus(_ftdi->ftHandle, &FT_RxQ_Bytes);
        //qDebug() << "OP13::read2" << FT_RxQ_Bytes;
    }
    while (FT_RxQ_Bytes < lenght);

    ulong Reads;
    _ftdi->FT_Read(_ftdi->ftHandle, in_buf, FT_RxQ_Bytes, &Reads);
    QByteArray a = QByteArray( (char*)in_buf, Reads);
//    qDebug() << "Readed bytes " << a.toHex(':') << Reads << endl;
    return a;
}

void OP13::write(int lenght)
{
    ulong Reads;
    _ftdi->FT_Write(_ftdi->ftHandle, p_out_buff, lenght, &Reads );
    //qDebug() << "Writed bytes " << QByteArray(p_out_buff, lenght).toHex(':') << Reads;
    _ftdi->FT_Read(_ftdi->ftHandle, p_in_buff, lenght, &Reads);    //читаем эхо
    //qDebug() << "Echo readed bytes " << QByteArray(p_in_buff, lenght).toHex(':') << Reads << endl;
}

void OP13::ftdi_low_baud_sender(uint baudRate, byte value)
{
    byte p;
    uint t = 1000/baudRate;
    _ftdi->ftStatus = _ftdi->FT_SetBreakOn(_ftdi->ftHandle);
    Sleep(t);    //старт бит
    for (int i = 0; i <= 7; i++)
    {
        p = value; //??
        p = p & 0x01;
        if (p == 1)
        {
            _ftdi->ftStatus = _ftdi->FT_SetBreakOff(_ftdi->ftHandle);  //1
            Sleep(t);
        }
        else
        {
            _ftdi->ftStatus = _ftdi->FT_SetBreakOn(_ftdi->ftHandle);   //0
            Sleep(t);
        }
        value = value >> 1;
    }
    _ftdi->ftStatus = _ftdi->FT_SetBreakOff(_ftdi->ftHandle);
}

bool OP13::ISO9141()
{
    return true;
}

bool OP13::ISO15765()
{
    return true;
}

bool OP13::ISO14230()
{
    return true;
}

void OP13::getDevList()
{

    FT_DEVICE_LIST_INFO_NODE *devInfo;
    DWORD numDevs =  1;
    // create the device information list
    _ftdi->ftStatus = _ftdi->FT_CreateDeviceInfoList(&numDevs);
    //if (_ftdi->ftStatus == FT_OK) {
    //printf("Number of devices is %d\n",numDevs);
    //}

    // allocate storage for list based on numDevs
    devInfo = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
    // get the device information list
    _ftdi->ftStatus = _ftdi->FT_GetDeviceInfoList(devInfo,&numDevs);
    qDebug() << "ftStatus: " << _ftdi->ftStatus;



}
