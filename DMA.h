#ifndef DMA_H
#define DMA_H
#include <QObject>
#include <QDebug>
#include "libs/ftdi.h"
#include "libs/J2534.h"
#include "QTimer"
#include <QStack>
#include <QTime>
#include <QThread>
#include <QtEndian>
#include <QQueue>

class dma:public QObject
{
    Q_OBJECT
public:
    dma()
    {

    }
    ~dma()
    {
        if (OP13 != nullptr)
        {
            delete OP13;
            OP13 = nullptr;
        }
        if (j2534 != nullptr)
        {
            delete j2534;
            j2534 = nullptr;
        }
    }

    void get_arr(QByteArray comm) // first 4 byte addr, next 2 byte count, and name..
    {
        command.enqueue(comm);
    }
    QQueue<QByteArray> command{};
    J2534 *j2534;
    ftdi *OP13;
    int VechicleInterfaceType = 0;
    byte MUT_Out_buffer[4128];
    // J2534
    unsigned int baudRate = 15625;              //предопределенная скорость соединения
    unsigned long magic_adder_readTimeout = 7;          // Длинна первого сообщения со статусом прием???
    unsigned long writeTimeout = 0;
    byte delay_after_command = 4;
    PASSTHRU_MSG  tx_msg = {};
    PASSTHRU_MSG  rx_msg[2] = {};
    unsigned long devID;
    unsigned long chanID;
    unsigned long chanID_INNO;
    unsigned long NumMsgs;
    unsigned long protocol = ISO9141_INNO;            // соответственно протокол
    unsigned long ConnectFlag = ISO9141_NO_CHECKSUM;
    //        || ISO9141_K_LINE_ONLY ;
    struct
    {
        unsigned int length;
        unsigned int svcid;
        unsigned short infosvcid;
    } inbuf;
    struct
    {
        unsigned int length;
        unsigned char data[256];
    } outbuf;


public slots:
    void dll_connect(int VechicleInterfaceType)                //по сигналу перечислителя
    {
        emit timer_lock();
        this->VechicleInterfaceType = VechicleInterfaceType;
        if (VechicleInterfaceType == 13)
            if ( OP13 == nullptr  )
            {
                OP13 = new ftdi ;
                qDebug() << " OP13 created!!";
                init_FTDI();
                qDebug() << " OP13 dll inited!!";
                delay_after_command = 0;
            }
        if (VechicleInterfaceType == 20)
            if ( j2534 == nullptr  )
            {
                j2534 = new J2534 ;
                qDebug() << " j2534 created!!";
                init_j2534();
                qDebug() << " j2534 dll inited!!";
                delay_after_command = 4;
            }
        emit timer_unlock();
    }
    void dll_disconnect()
    {
        if (OP13 != nullptr)
        {
            close_FTDI();
            delete OP13;
            OP13 = nullptr;
            qDebug() << "OP13 delete";
        }
        if (j2534 != nullptr)
        {
            close_j2534();
            delete j2534;
            j2534 = nullptr;
            qDebug() << "j2534 delete";
        }

    }

    QString connect()
    {
        if (!common_five_baud_init())
            return "";
        read_direct(0xF52, 4); //читаем номер калибровки
        return QString::number( qFromBigEndian<quint32>(rx_msg[1].Data), 16 );
    }

    void read_indirect(quint32 addr, int count)
    {
        emit timer_lock();
        if (VechicleInterfaceType == 13)
            if ( OP13 != nullptr  )
                read_FTDI(0xE0, addr, count);
        if (VechicleInterfaceType == 20)
            if ( j2534 != nullptr  )
                read_J2534(0xE0, addr, count);
        emit timer_unlock();
    }
    void read_direct(quint32 addr, int count)
    {
        emit timer_lock();
        if (VechicleInterfaceType == 13)
            if ( OP13 != nullptr  )
                read_FTDI(0xE1, addr, count);
        if (VechicleInterfaceType == 20)
            if ( j2534 != nullptr  )
                read_J2534(0xE1, addr, count);
        emit timer_unlock();
    }
    void write_direct(quint32 addr, int count)
    {
        emit timer_lock();
        if (VechicleInterfaceType == 13)
            if ( OP13 != nullptr  )
            {
                write_direct_FTDI(addr, count);
                qDebug() << " write_direct_FTDI";
            }
        if (VechicleInterfaceType == 20)
            if ( j2534 != nullptr  )
            {
                write_direct_J2534(addr, count);
                qDebug() << " write_direct_J2534";
            }
        emit timer_unlock();
    }

private:
    DWORD Reads;
    DWORD FT_RxQ_Bytes;
    bool common_five_baud_init()
    {
        if (VechicleInterfaceType == 13)
        {
            return ftdi_five_baud_init() ;
        }
        if (VechicleInterfaceType == 20)
        {
            return j2534_five_baud_init() ;
        }
        return false;
    }

    void ftdi_low_baud_sender(uint baudRate, byte value)
    { byte p;
        uint t = 1000/baudRate;
        OP13->ftStatus = OP13->FT_SetBreakOn(OP13->ftHandle);
        Sleep(t);    //старт бит
        for (int i = 0; i <= 7; i++)
        {
            p = value; //??
            p = p & 0x01;
            if (p == 1)
            {
                OP13->ftStatus = OP13->FT_SetBreakOff(OP13->ftHandle);  //1
                Sleep(t);
            }
            else
            {
                OP13->ftStatus = OP13->FT_SetBreakOn(OP13->ftHandle);   //0
                Sleep(t);
            }
            value = value >> 1;
        }
        OP13->ftStatus = OP13->FT_SetBreakOff(OP13->ftHandle);
    }
    //общая часть процедур доступа к памяти контроллера
    void comand_write(unsigned long addr, unsigned long count)  {
        NumMsgs = 1;
        tx_msg.DataSize = 1;
        if (VechicleInterfaceType == 13)
            //            if ( OP13 != nullptr  )
            OP13->FT_Write( OP13->ftHandle, &tx_msg.Data, 1, &Reads );
        if (VechicleInterfaceType == 20)
            //            if ( j2534 != nullptr  )
            j2534->PassThruWriteMsgs(chanID, &tx_msg, &NumMsgs, writeTimeout);
        QThread::msleep(delay_after_command);
        tx_msg.DataSize = 6;
        tx_msg.Data[0] = (addr & 0xFF000000) >> 24;
        tx_msg.Data[1] = (addr & 0xFF0000) >> 16;
        tx_msg.Data[2] = (addr & 0xFF00) >> 8;
        tx_msg.Data[3] = (addr & 0xFF);
        tx_msg.Data[4] = (count & 0xFF00) >> 8;
        tx_msg.Data[5] = (count & 0xFF);
        NumMsgs = 1;
        if (VechicleInterfaceType == 13)
            //            if ( OP13 != nullptr  )
        {
            OP13->FT_Write( OP13->ftHandle, &tx_msg.Data, 6, &Reads );
            OP13->FT_Read(OP13->ftHandle, &rx_msg[0].Data, 7, &Reads);    //читаем эхо оптом
        }
        if (VechicleInterfaceType == 20)
            //            if ( j2534 != nullptr  )
            j2534->PassThruWriteMsgs(chanID, &tx_msg, &NumMsgs, writeTimeout);
    }
    void read_J2534(uchar command, unsigned long addr, unsigned long count)
    {
        tx_msg.Data[0] = command;
        comand_write(addr, count);
        NumMsgs = 1;
        rx_msg[0] = tx_msg;
        rx_msg[0].RxStatus = 0;
        do
            j2534->PassThruReadMsgs(chanID, &rx_msg[0], &NumMsgs, count*10000/baudRate + magic_adder_readTimeout);
        while(rx_msg[0].RxStatus == START_OF_MESSAGE);
        emit recieve(QByteArray::fromRawData( (char*)rx_msg[1].Data, count) );
        //        memcpy(&MUT_In_buffer, &rx_msg[1].Data, count);
    }
    void write_direct_J2534(unsigned long addr, unsigned long count)
    {
        tx_msg.Data[0] = 0xE2;
        comand_write(addr, count);
        NumMsgs = 1;
        tx_msg.DataSize = count;
        memcpy(&tx_msg.Data, &MUT_Out_buffer, count);
        j2534->PassThruWriteMsgs(chanID, &tx_msg, &NumMsgs, writeTimeout);
    }
    bool init_j2534()
    {
        QString sf;
        tx_msg.ProtocolID = protocol;

        for (int r = 0; r < 2; r++)
        {
            rx_msg[r] = tx_msg;
        }

        if (!j2534->init())
        {
            //     error_out = "can't connect to J2534 DLL.\r\n";
            qDebug() << "can't connect to J2534 DLL.";
            return false;
        }

        //получаем дескриптор
        if (j2534->PassThruOpen(nullptr, &devID))
        {// читаем и выводим ошибку
            reportJ2534Error(sf);
            //       error_out = "PassThruOpen: " + sf;
            qDebug() << "PassThruOpen: " + sf;
            return false;
        }
        //       error_out = "PassThruOpen: devID = " + QString::number(devID) + "\r\n";
        //читаем версию и прочюю требуху
        char strApiVersion[256];
        char strDllVersion[256];
        char strFirmwareVersion[256];
        char strSerial[256];

        if (j2534->PassThruReadVersion(strApiVersion, strDllVersion, strFirmwareVersion, devID))
        {
            reportJ2534Error(sf);
            //  error_out += "PassThruReadVersion: " + sf;
            qDebug() << "PassThruReadVersion: " + sf;
            return false;
        }

        if (!get_serial_num(devID, strSerial))
        {
            reportJ2534Error(sf);
            //      error_out += "get_serial_num: " + sf;
            qDebug() << "get_serial_num: " + sf;
            return false;
        }

        //   error_out += "J2534 API Version: " + QString(strApiVersion) + "\r\n";
        //   error_out += "J2534 DLL Version: " +  QString(strDllVersion) + "\r\n";
        //   error_out += "Device Firmware Version: " +  QString(strFirmwareVersion) + "\r\n";
        //    error_out += "Device Serial Number: " +  QString(strSerial) + "\r\n";

        if (j2534->PassThruConnect(devID, protocol, ConnectFlag, baudRate, &chanID))

        {
            reportJ2534Error(sf);
            //error_out += "PassThruConnect: \n" + sf;
            qDebug() << "PassThruConnect: \n" + sf;
            return false;
        }


        // all J2534 channels need filters in order to receive anything at all
        //
        // in this case, we simply create a "pass all" filter so that we can see
        // everything unfiltered in the raw stream

        PASSTHRU_MSG txmsg;
        PASSTHRU_MSG msgMask,msgPattern;
        unsigned long msgId;

        txmsg.ProtocolID = ISO9141_INNO;
        txmsg.RxStatus = 0;
        txmsg.TxFlags = 0;
        txmsg.Timestamp = 0;
        txmsg.DataSize = 1;
        txmsg.ExtraDataIndex = 0;
        msgMask = msgPattern  = txmsg;
        msgMask.Data[0] = 0; // mask the first byte to 0
        msgPattern.Data[0] = 0; // match it with 0 (i.e. pass everything)
        if (j2534->PassThruStartMsgFilter(chanID, PASS_FILTER, &msgMask, &msgPattern, nullptr, &msgId))
        {
            reportJ2534Error(sf);
            return false;
        }
        qDebug() << "common_init_j2534 OK";
        return true;
    }
    bool init_FTDI()
    {
        if (OP13->FT_Open(0, &OP13->ftHandle) == FT_OK)
        {
            //   error_out = "FT_Open OK";
            // FT_Open OK, use ftHandle to access device
            OP13->ftStatus = OP13->FT_ResetDevice(OP13->ftHandle);
            OP13->ftStatus = OP13->FT_Purge(OP13->ftHandle, FT_PURGE_RX | FT_PURGE_TX);
            OP13->ftStatus = OP13->FT_SetTimeouts(OP13->ftHandle, 0, 0);
            OP13->ftStatus = OP13->FT_SetLatencyTimer(OP13->ftHandle, 1);
            OP13->ftStatus = OP13->FT_SetBaudRate(OP13->ftHandle, baudRate);
            return true;
        }
        qDebug() << " FT_Open failed";
        return false;
    }
    void read_FTDI(uchar command, quint32 addr, int count)
    {
        tx_msg.Data[0] = command;
        comand_write(addr, count);
        //Get bytes waiting to be read
        do
            OP13->FT_GetQueueStatus(OP13->ftHandle, &FT_RxQ_Bytes);
        while(FT_RxQ_Bytes <= count);
        OP13->FT_Read(OP13->ftHandle, &rx_msg[1].Data, count, &Reads);
    }

    void write_direct_FTDI(unsigned long addr, unsigned long count)
    {
        tx_msg.Data[0] = 0xE2;
        comand_write(addr, count);
        //   memcpy(&FT_Out_Buffer, &MUT_Out_buffer, count);
        //    Sleep(400);
        OP13->FT_Write(OP13->ftHandle, &MUT_Out_buffer, count, &Reads );
        qDebug() << "Writed bytes " << Reads;
        OP13->FT_Read(OP13->ftHandle, &rx_msg[0].Data, count, &Reads);    //читаем эхо
    }

    void close_FTDI()
    {}
    void close_j2534()
    {
        j2534->PassThruDisconnect(chanID);
        j2534->PassThruClose(devID);
    }

    bool j2534_five_baud_init()
    {
        QString sf;
        //-======================================== SET CONFIG ===========================================
        // set timing
        SCONFIG_LIST scl;
        SCONFIG scp[6] = { {DATA_RATE, baudRate}, {P1_MAX, 0}, {P2_MAX, 0}, {P3_MIN, 0}, {P4_MIN, 0}, {LOOPBACK, 0} };

        scl.NumOfParams = 6;
        scl.ConfigPtr = scp;
        if (j2534->PassThruIoctl(chanID, SET_CONFIG, &scl, nullptr))
        {
            reportJ2534Error(sf);
            return false;
        }

        //==========================================   5 baud init  ================================
        SBYTE_ARRAY inputMsg, outputMsg;
        unsigned char EcuAddr[1]; /* ECU target address array */
        unsigned char KeyWord[3]; /* Keyword identifier array */
        EcuAddr[0] = 0x00; /* Initialization address used to activate all ECUs */
        inputMsg.NumOfBytes = 1; /* ECU target address array contains one address. */
        inputMsg.BytePtr = &EcuAddr[0]; /* Assign pointer to ECU target address array. */
        outputMsg.NumOfBytes = 0; /* KeyWord array is empty. */
        outputMsg.BytePtr = &KeyWord[0]; /* Assign pointer to KeyWord array. */

        if (j2534->PassThruIoctl(chanID, FIVE_BAUD_INIT, reinterpret_cast<void *>(&inputMsg), reinterpret_cast<void *>(&outputMsg)))
        {
            reportJ2534Error(sf);
            qDebug() << "PassThruIoctl - FIVE_BAUD_INIT : not ok" + sf;
            return false;
        }
        qDebug() << "PassThruIoctl - FIVE_BAUD_INIT : OK " << QString::number(KeyWord[0], 16) << QString::number(KeyWord[1], 16);

        j2534->PassThruIoctl(chanID, GET_CONFIG, &scl, nullptr)   ;
        baudRate = scl.ConfigPtr[0].Value;                                //

        // ============================ now setup the filter(s) =========================
        //   PASSTHRU_MSG  txmsg;
        PASSTHRU_MSG msgMask, msgPattern;
        unsigned long msgId;

        // simply create a "pass all" filter so that we can see
        // everything unfiltered in the raw stream
        tx_msg.ProtocolID = protocol;

        tx_msg.DataSize = 1;

        msgMask = msgPattern = tx_msg;

        msgMask.Data[0] = 0; // mask the first byte to 0
        msgPattern.Data[0] = 0; // match it with 0 (i.e. pass everything)
        memset(&msgMask.Data, 0, 4128);
        memset(&msgPattern.Data, 0, 4128);
        if (j2534->PassThruStartMsgFilter(chanID, PASS_FILTER, &msgMask, &msgPattern, nullptr, &msgId))
        {
            reportJ2534Error(sf);
            qDebug() << "PassThruStartMsgFilter : OK"+sf;
            return false;
        }
        return true;
    }
    bool ftdi_five_baud_init()
    {
        ftdi_low_baud_sender(5, 0x00);                                 //5 baud, 0x00 ecu addr, 0x05 TCU?
        Sleep(400);
        //Get bytes waiting to be read
        OP13->ftStatus = OP13->FT_GetQueueStatus(OP13->ftHandle, &FT_RxQ_Bytes);
        OP13->ftStatus = OP13->FT_Read(OP13->ftHandle, rx_msg[0].Data, FT_RxQ_Bytes, &Reads);
        if ((OP13->ftStatus != FT_OK) || (FT_RxQ_Bytes < 1))
        {
            qDebug() << " FT_Read failed" << Reads << FT_RxQ_Bytes;
            return false;
        }
        qDebug() << " FT_five_baud_OK";
        return true;
    }

    void reportJ2534Error(QString &s)
    {
        char err[512];
        j2534->PassThruGetLastError(err);
        s += QString(err) + "\r\n";
    }
    bool get_serial_num(unsigned long devID, char* serial)
    {
        inbuf.length = 2;
        inbuf.svcid = 5; // info
        inbuf.infosvcid = 1; // serial

        outbuf.length = sizeof( outbuf.data);

        if (j2534->PassThruIoctl(devID, TX_IOCTL_APP_SERVICE, &inbuf, &outbuf))
        {
            serial[0] = 0;
            return false;
        }

        memcpy(serial,  outbuf.data,  outbuf.length);
        serial[ outbuf.length] = 0;
        return true;
    }

signals:
    void timer_lock();
    void timer_unlock();
    void recieve(QByteArray);
};

class Timer:public QTimer
{
    Q_OBJECT

public:
    Timer(QObject *parent = nullptr)
    {
        setParent(parent);
    }
    ~Timer()
    {

    }
private:
    QStack<bool> stack = {};//переменная для хранения предыдущего состояния таймера

public slots:
    void timer_lock()
    {
        stack.push(this->isActive());
        this->stop();
    }
    void timer_unlock()
    {
        if (stack.pop())
            this->start();
    }

};

#endif // DMA_H
