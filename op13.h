#ifndef OP13_H
#define OP13_H

#include <QObject>
#include <QDebug>
#include "libs/ftdi.h"
#include "ecu_interface.h"

class OP13: public ECU_interface
{
    Q_OBJECT
public:

    OP13(TCHAR *dllName = nullptr)
    {
        this->dllName = dllName;
    }
    ~OP13()
    {
        close();
    }

    bool _connect(unsigned long protocol, unsigned long ConnectFlag, unsigned int baudRate)
    {
        ConnectFlag = 0;
        this->protocol = protocol;
        this->baudRate = baudRate;
        return true;
    }
    bool disconnect()
    {
return true;
    }
    bool e7_connect()
    {
        return _connect( ISO9141_K, ISO9141_NO_CHECKSUM, 15625);
    }
    bool five_baud_init()
    {
        ftdi_low_baud_sender(5, 0x00);                                 //5 baud, 0x00 ecu addr, 0x05 TCU?
        QThread::msleep(400);
        //Get bytes waiting to be read
        op13->ftStatus = op13->FT_GetQueueStatus(op13->ftHandle, &FT_RxQ_Bytes);
        op13->ftStatus = op13->FT_Read(op13->ftHandle, in_buff, FT_RxQ_Bytes, &Reads);
        if ((op13->ftStatus != FT_OK) || (FT_RxQ_Bytes < 1))
        {
            qDebug() << " FT_Read failed" << Reads << FT_RxQ_Bytes;
            return false;
        }
        qDebug() << " FT_five_baud_OK";
        return true;
    }
    void read()
    {
        //Get bytes waiting to be read
        do
            op13->FT_GetQueueStatus(op13->ftHandle, &FT_RxQ_Bytes);
        while(FT_RxQ_Bytes < 1);
        op13->FT_Read(op13->ftHandle, in_buff, FT_RxQ_Bytes, &Reads);
        //       emit readyRead(QByteArray::fromRawData( buf, Reads));
        //return QByteArray::fromRawData( buf, Reads);
    }
    void write( uint count )
    {
        op13->FT_Write(op13->ftHandle, out_buff, count, &Reads );
        qDebug() << "Writed bytes " << Reads;
        op13->FT_Read(op13->ftHandle, in_buff, count, &Reads);    //читаем эхо
    }

    void close()
    {

    }

public slots:

    void start_tactrix_inno()
    {}
    void stop_tactrix_inno()
    {}
private slots:
    bool _init()
    {
        op13 = new ftdi ;

        delay_after_command = 0;
        if (op13->FT_Open(0, &op13->ftHandle) == FT_OK)
        {
            //   error_out = "FT_Open OK";
            // FT_Open OK, use ftHandle to access device
            op13->ftStatus = op13->FT_ResetDevice(op13->ftHandle);
            op13->ftStatus = op13->FT_Purge(op13->ftHandle, FT_PURGE_RX | FT_PURGE_TX);
            op13->ftStatus = op13->FT_SetTimeouts(op13->ftHandle, 0, 0);
            op13->ftStatus = op13->FT_SetLatencyTimer(op13->ftHandle, 1);
            op13->ftStatus = op13->FT_SetBaudRate(op13->ftHandle, baudRate);
            return true;
        }
        qDebug() << " FT_Open failed";
        return false;
    }


private:
    ftdi *op13;

    DWORD FT_RxQ_Bytes;
    ulong Reads;
    unsigned int baudRate = 15625;
    unsigned long protocol;
    void ftdi_low_baud_sender(uint baudRate, byte value)
    {
        byte p;
        uint t = 1000/baudRate;
        op13->ftStatus = op13->FT_SetBreakOn(op13->ftHandle);
        Sleep(t);    //старт бит
        for (int i = 0; i <= 7; i++)
        {
            p = value; //??
            p = p & 0x01;
            if (p == 1)
            {
                op13->ftStatus = op13->FT_SetBreakOff(op13->ftHandle);  //1
                Sleep(t);
            }
            else
            {
                op13->ftStatus = op13->FT_SetBreakOn(op13->ftHandle);   //0
                Sleep(t);
            }
            value = value >> 1;
        }
        op13->ftStatus = op13->FT_SetBreakOff(op13->ftHandle);
    }

signals:

};

#endif // OP13_H
