#include "op13.h"
#include <QCoreApplication>

OP13::OP13(QString dllName, QString DeviceUniqueID) :  comm_device_interface( dllName, DeviceUniqueID)
{
    devType = deviceType::OP13;
    _ftdi = new ftdi(QCoreApplication::applicationDirPath() + "\\ftd2xx.dll");
    p_in_buff = in_buf;
    p_out_buff = out_buf;
    qDebug() << "OP13" << DeviceUniqueID;
    info();
}

OP13::~OP13()
{
    //common_destructor();
    //close();
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
            qDebug() << "Number of devices is" << numDevs;
        }

        // allocate storage for list based on numDevs
        devInfo = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
        // get the device information list
        _ftdi->ftStatus = _ftdi->FT_GetDeviceInfoList(devInfo,&numDevs);
        qDebug() << "ftStatus: " << _ftdi->ftStatus;


    if (_ftdi->ftStatus == FT_OK)
    {
        for (uint i = 0; i < numDevs; i++) {
            QString info = "Dev: " + QString::number( i ) +
                    "  SerialNumber: " + QString::fromLatin1( devInfo[i].SerialNumber) +
                    "  Description: " + QString::fromLatin1(devInfo[i].Description);

            //qDebug() << "info: " << info;
            emit Log(info);
        }
    }
    }
    close();
    emit readyInterface(true);
    return true;
}

bool OP13::open(Protocol protocol, enum ConnectFlag ConnectFlag, uint baudRate)
{
    this->baudRate = baudRate;

    p_in_buff = in_buf;
    p_out_buff = out_buf;

    ulong s;_ftdi->ftHandle=0;
    if ( (s = _ftdi->FT_Open(0, &_ftdi->ftHandle) == FT_OK))
    {
        //   error_out = "FT_Open OK";
        // FT_Open OK, use ftHandle to access device
        s = _ftdi->ftStatus = _ftdi->FT_ResetDevice(_ftdi->ftHandle);
        s = _ftdi->ftStatus = _ftdi->FT_Purge(_ftdi->ftHandle, FT_PURGE_RX | FT_PURGE_TX);
        s = _ftdi->ftStatus = _ftdi->FT_SetTimeouts(_ftdi->ftHandle, 250, 2500);
        s = _ftdi->ftStatus = _ftdi->FT_SetLatencyTimer(_ftdi->ftHandle, 1);
        s = _ftdi->ftStatus = _ftdi->FT_SetBaudRate(_ftdi->ftHandle, baudRate);
        qDebug() << " FT_Open 5" <<  s;
        //     emit readyInterface(true);

        return true;
    }
    qDebug() << " FT_Open failed" << s << QString::fromLatin1( _ftdi->getLastError());

    return false;
}

bool OP13::close()
{
    ulong s = _ftdi->FT_Close( _ftdi->ftHandle);
    qDebug() << " close" << s;
    return true;
}

bool OP13::five_baud_init()
{
    ftdi_low_baud_sender(5, 0x00);                                 //5 baud, 0x00 ecu addr, 0x05 TCU?
    QThread::msleep(400);
    //Get bytes waiting to be read
    _ftdi->ftStatus = _ftdi->FT_GetQueueStatus(_ftdi->ftHandle, &FT_RxQ_Bytes);
    _ftdi->ftStatus = _ftdi->FT_Read(_ftdi->ftHandle, in_buf, FT_RxQ_Bytes, &Reads);
    if ((_ftdi->ftStatus != FT_OK) || (FT_RxQ_Bytes < 1))
    {
        qDebug() << " FT_Read failed" << Reads << FT_RxQ_Bytes;
        return false;
    }
    qDebug() << " FT_five_baud_OK";
    return true;
}

QByteArray OP13::read()
{
    //Get bytes waiting to be read
    int numAttemptcount = numAttempt;
    do
    {
        _ftdi->FT_GetQueueStatus(_ftdi->ftHandle, &FT_RxQ_Bytes);
        numAttemptcount--;
    }
    while(FT_RxQ_Bytes < 1 && numAttemptcount > 0);
    _ftdi->FT_Read(_ftdi->ftHandle, in_buf, FT_RxQ_Bytes, &Reads);
    //       emit readyRead(QByteArray::fromRawData( buf, Reads));
    return QByteArray( (char*)in_buf, Reads);
}

void OP13::write(int lenght)
{
    _ftdi->FT_Write(_ftdi->ftHandle, p_out_buff, lenght, &Reads );
    //        qDebug() << "Writed bytes " << Reads;
    _ftdi->FT_Read(_ftdi->ftHandle, p_in_buff, lenght, &Reads);    //читаем эхо
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
