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

#include "ecu.h"

typedef struct
{
    unsigned length		: 7;
    unsigned mark4		: 1;

    unsigned length_msb : 1;
    unsigned mark3		: 1;
    unsigned reserved1	: 1;
    unsigned log_caps	: 1;
    unsigned is_data	: 1;
    unsigned mark2		: 1;
    unsigned is_rec		: 1;
    unsigned mark1		: 1;
} inno_v2_mts_hdr;

typedef struct
{
    unsigned lambda		: 7;
    unsigned mark5		: 1;

    unsigned lambda_hi  : 6;
    unsigned mark4		: 2;

    unsigned afr		: 7;
    unsigned mark3		: 1;

    unsigned afr_msb	: 1;
    unsigned mark2		: 1;
    unsigned func		: 3;
    unsigned mark1		: 3;
} inno_v2_lc1_pkt;

typedef struct
{
    unsigned aux_lsbs	: 7;
    unsigned mark2		: 1;

    unsigned aux_msbs	: 6;
    unsigned mark1		: 2;
} inno_v2_aux_pkt;

typedef struct
{
    unsigned afr		: 7;
    unsigned mark4		: 1;

    unsigned afr_msb	: 1;
    unsigned mark3		: 1;
    unsigned func		: 3;
    unsigned mark2		: 1;
    unsigned is_rec		: 1;
    unsigned mark1		: 1;
} inno_v1_mts_hdr;

typedef struct
{
    unsigned bv_lo		: 7;
    unsigned mark4		: 1;

    unsigned bv_hi		: 3;
    unsigned mb			: 3;
    unsigned mark3		: 2;

    unsigned lambda		: 7;
    unsigned mark2		: 1;

    unsigned lambda_hi  : 6;
    unsigned mark1		: 2;
} inno_v1_lm1_pkt;

typedef struct
{
    unsigned aux_lsbs	: 7;
    unsigned mark2		: 1;

    unsigned aux_msbs	: 3;
    unsigned mark1		: 5;
} inno_v1_aux_pkt;


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

    J2534 *j2534;
    ftdi *OP13;
    int VechicleInterfaceType = 0;
    uchar MUT_Out_buffer[4128];

    // J2534
    unsigned int baudRate = 15625;
    unsigned long magic_adder_readTimeout = 7;          // Длинна первого сообщения со статусом прием???
    unsigned long writeTimeout = 0;
    byte delay_after_command = 4;
    PASSTHRU_MSG  tx_msg = {};
    PASSTHRU_MSG  rx_msg[2] = {};
    PASSTHRU_MSG  inno_rx_msg[2] = {};
    unsigned long devID;
    unsigned long chanID;
    unsigned long chanID_INNO;
    unsigned long NumMsgs ;
    unsigned long protocol_inno = ISO9141_INNO;
    unsigned long protocol = ISO9141_K;
    unsigned long ConnectFlag = ISO9141_NO_CHECKSUM;  //        || ISO9141_K_LINE_ONLY ;
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
    bool init_inno()
    {
        if (j2534->PassThruConnect(devID, ISO9141_INNO, ISO9141_NO_CHECKSUM, 19200, &chanID_INNO ))
        {
            emit Log( "inno PassThruConnect: not ok  -  " + reportJ2534Error() );
            return false;
        }

        // all J2534 channels need filters in order to receive anything at all
        //
        // in this case, we simply create a "pass all" filter so that we can see
        // everything unfiltered in the raw stream

        PASSTHRU_MSG rxmsg,txmsg;
        PASSTHRU_MSG msgMask,msgPattern;
        unsigned long msgId;
        unsigned long numRxMsg;

        txmsg.ProtocolID = ISO9141_INNO;
        txmsg.RxStatus = 0;
        txmsg.TxFlags = 0;
        txmsg.Timestamp = 0;
        txmsg.DataSize = 1;
        txmsg.ExtraDataIndex = 0;
        msgMask = msgPattern  = txmsg;
        msgMask.Data[0] = 0; // mask the first byte to 0
        msgPattern.Data[0] = 0; // match it with 0 (i.e. pass everything)
        if (j2534->PassThruStartMsgFilter(chanID_INNO,PASS_FILTER,&msgMask,&msgPattern,NULL,&msgId))
        {
            emit Log( "inno PassThruStartMsgFilter: not ok" + reportJ2534Error() );
            return false;
        }
        emit Log( "inno init : OK" );
        return true;
    }
    void dump_inno(PASSTHRU_MSG* msg)
    {
        if (msg->RxStatus & START_OF_MESSAGE)
            return; // skip
        if (msg->DataSize < 2)
            return;
        inno_v1_mts_hdr hdrv1;
        inno_v2_mts_hdr hdrv2;
        *((int*)&hdrv1) = msg->Data[0]*256 + msg->Data[1];
        *((int*)&hdrv2) = msg->Data[0]*256 + msg->Data[1];
        unsigned int payload;
        unsigned char* msgptr;
        unsigned int auxcnt = 1;
        bool validV1Hdr = hdrv1.mark1 && !hdrv1.mark2 && !hdrv1.mark3 && !hdrv1.mark4;
        bool validV2Hdr = hdrv2.mark1 && hdrv2.mark2 && hdrv2.mark3 && hdrv2.mark4;
        bool is_lm1_packet = false;
        if (validV1Hdr || validV2Hdr)
        {
            if (validV2Hdr)
            {
                printf("-- V2 header --\n");
                payload = 2 * (hdrv2.length_msb * 128 + hdrv2.length);
            }
            else
            {
                printf("-- LM-1 headerless --\n");
                payload = 14; // LM-1 V1 payload is a fixed size
            }
            if (payload + 2 != msg->DataSize)
                return;
            msgptr = msg->Data + 2;
            // work our way through the payload bytes
            while (payload)
            {
                if (validV2Hdr && !is_lm1_packet)
                {
                    if ((*msgptr & 0xE2) == 0x42)
                    {
                        printf("-- LC-1 packet --\n");
                        // LC-1 packet
                        inno_v2_lc1_pkt pkt;
                        *((int*)&pkt) = (msgptr[0]<<24) + (msgptr[1]<<16) + (msgptr[2]<<8) + msgptr[3];
                        msgptr += 4;
                        payload -= 4;

                        if (pkt.func == 0)
                        {
                            double lambda,afr;
                            lambda = 0.5 + 0.001 * (pkt.lambda_hi * 128 + pkt.lambda);
                            afr = lambda * 0.1 * (pkt.afr_msb * 128 + pkt.afr);
                            printf("lambda: %lf  afr: %lf\n",lambda,afr);
                        }
                        else if (pkt.func == 1)
                        {
                            double pct_o2;
                            pct_o2 = 0.1 * (pkt.lambda_hi * 128 + pkt.lambda);
                            printf("O2: %lf%%%\n",pct_o2);
                        }
                        else
                            printf("func: %d\n",pkt.func);
                    }
                    else if ((*msgptr & 0xA2) == 0x80)
                    {
                        // LM-1 packet (within v2 header)
                        *((int*)&hdrv1) = msgptr[0]*256 + msgptr[1];
                        is_lm1_packet = true;
                        msgptr += 2;
                        payload -= 2;
                    }
                    else
                    {
                        // must be AUX packet
                        inno_v2_aux_pkt pkt;
                        *((int*)&pkt) = (msgptr[0]<<8) + msgptr[1];
                        msgptr += 2;
                        payload -= 2;
                        printf("aux%d: %d\n",auxcnt++,(pkt.aux_msbs << 7) + pkt.aux_lsbs);
                    }
                }
                else
                {
                    printf("-- LM-1 packet --\n");
                    // LM-1 packet
                    inno_v1_lm1_pkt pkt;
                    *((int*)&pkt) = (msgptr[0]<<24) + (msgptr[1]<<16) + (msgptr[2]<<8) + msgptr[3];
                    msgptr += 4;
                    payload -= 4;

                    if (hdrv1.func == 0)
                    {
                        double lambda,afr;
                        lambda = 0.5 + 0.001 * (pkt.lambda_hi * 128 + pkt.lambda);
                        afr = lambda * 0.1 * (hdrv1.afr_msb * 128 + hdrv1.afr);
                        printf("lambda: %lf  afr: %lf\n",lambda,afr);
                    }
                    else if (hdrv1.func == 1)
                    {
                        double pct_o2;
                        pct_o2 = 0.1 * (pkt.lambda_hi * 128 + pkt.lambda);
                        printf("O2: %lf%%%\n",pct_o2);
                    }
                    else
                        printf("func: %d\n",hdrv1.func);

                    // get 5 AUX packets
                    for (int i = 0; i < 5; i++)
                    {
                        inno_v1_aux_pkt pkt;
                        *((int*)&pkt) = (msgptr[0]<<8) + msgptr[1];
                        msgptr += 2;
                        payload -= 2;
                        printf("aux%d: %d\n",auxcnt++,(pkt.aux_msbs << 7) + pkt.aux_lsbs);
                    }
                    is_lm1_packet = false;
                }
            }
        }
    }

    void dll_connect(int VechicleInterfaceType)                //по сигналу перечислителя
    {
        emit timer_lock();
        this->VechicleInterfaceType = VechicleInterfaceType;
        if (VechicleInterfaceType == 13 && OP13 == nullptr  )
        {
            OP13 = new ftdi ;
            qDebug() << " OP13 created!!";
            init_FTDI();
            qDebug() << " OP13 dll inited!!";
            delay_after_command = 0;
        }
        if (VechicleInterfaceType == 20 && j2534 == nullptr  )
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
        return QString::number( qFromBigEndian<quint32>(MUT_Out_buffer), 16 );
    }

    void read_indirect(quint32 addr, int count)
    {
        emit timer_lock();
        if (VechicleInterfaceType == 13 && OP13 != nullptr  )
            read_FTDI(0xE0, addr, count);
        if (VechicleInterfaceType == 20  && j2534 != nullptr  )
            read_J2534(0xE0, addr, count);
        emit timer_unlock();
    }
    void read_direct(quint32 addr, int count)
    {
        emit timer_lock();
        if (VechicleInterfaceType == 13 && OP13 != nullptr  )
            read_FTDI(0xE1, addr, count);
        if (VechicleInterfaceType == 20 && j2534 != nullptr  )
            read_J2534(0xE1, addr, count);
        emit timer_unlock();
    }
    void write_direct(quint32 addr, int count)
    {
        emit timer_lock();
        if (VechicleInterfaceType == 13 && OP13 != nullptr  )
        {
            write_direct_FTDI(addr, count);
            qDebug() << " write_direct_FTDI";
        }
        if (VechicleInterfaceType == 20 && j2534 != nullptr  )
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
        {
            j2534->PassThruReadMsgs(chanID, &rx_msg[0], &NumMsgs, count*10000/baudRate + magic_adder_readTimeout);
            NumMsgs = 1;
//            j2534->PassThruReadMsgs(chanID_INNO, &inno_rx_msg[0], &NumMsgs, 10);
//            if (NumMsgs)
//                dump_inno(inno_rx_msg);
        }

        while(rx_msg[0].RxStatus == START_OF_MESSAGE);

        memcpy(&MUT_Out_buffer, &rx_msg[0].Data, count);
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
        tx_msg.ProtocolID = protocol;
        for (int r = 0; r < 2; r++)
        {
            rx_msg[r] = tx_msg;
        }
        if (!j2534->init())
        {
            emit Log( "can't connect to J2534 DLL." );
            return false;
        }
        //получаем дескриптор
        if (j2534->PassThruOpen(nullptr, &devID))
        {// читаем и выводим ошибку
            emit Log( "PassThruOpen: not ok" + reportJ2534Error() );
            return false;
        }
        emit Log( "PassThruOpen:  devID = " + QString::number(devID) );
        //читаем версию и прочюю требуху
        char strApiVersion[256];
        char strDllVersion[256];
        char strFirmwareVersion[256];
        char strSerial[256];
        if ( j2534->PassThruReadVersion(strApiVersion, strDllVersion, strFirmwareVersion, devID) )
        {
            emit Log( "PassThruReadVersion: not ok" + reportJ2534Error() );
        }
        else
        {
            emit Log( "J2534 API Version: " + QString(strApiVersion) );
            emit Log( "J2534 DLL Version: " +  QString(strDllVersion));
            emit Log( "Device Firmware Version: " +  QString(strFirmwareVersion) );
        }
        if (get_serial_num(devID, strSerial))
            emit Log( "Device Serial Number: " +  QString(strSerial) );
        else
            emit Log( "get_serial_num: not ok" + reportJ2534Error() );

        if (j2534->PassThruConnect(devID, protocol, ConnectFlag, baudRate, &chanID))
        {
            emit Log( "PassThruConnect: not ok" + reportJ2534Error() );
            return false;
        }
        emit Log( "PassThruConnect: OK  -  chanel ID: " + QString::number(chanID));

        emit Log( "common_init_j2534 OK" );

        init_inno();
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
    QByteArray read_FTDI(uchar command, quint32 addr, int count)
    {
        tx_msg.Data[0] = command;
        comand_write(addr, count);
        //Get bytes waiting to be read
        do
            OP13->FT_GetQueueStatus(OP13->ftHandle, &FT_RxQ_Bytes);
        while(FT_RxQ_Bytes <= count);
        OP13->FT_Read(OP13->ftHandle, &rx_msg[1].Data, count, &Reads);
        return QByteArray::fromRawData( (char*)rx_msg[1].Data, count);
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
        //-======================================== SET CONFIG ===========================================
        // set timing
        SCONFIG_LIST scl;
        SCONFIG scp[6] = { {DATA_RATE, baudRate}, {P1_MAX, 0}, {P2_MAX, 0}, {P3_MIN, 0}, {P4_MIN, 0}, {LOOPBACK, 0} };

        scl.NumOfParams = 6;
        scl.ConfigPtr = scp;
        if (j2534->PassThruIoctl(chanID, SET_CONFIG, &scl, nullptr))
        {
            emit Log( "PassThruIoctl - SET_CONFIG : not ok  " + reportJ2534Error() );
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

        if (j2534->PassThruIoctl(chanID, FIVE_BAUD_INIT, (void *)(&inputMsg), (void *)(&outputMsg)))
        {
            emit Log( "PassThruIoctl - FIVE_BAUD_INIT : not ok" + reportJ2534Error() );
            return false;
        }
        Log( "PassThruIoctl - FIVE_BAUD_INIT : OK " + QString::number(KeyWord[0], 16) + " " + QString::number(KeyWord[1], 16) + " " + QString::number(KeyWord[2], 16));

        j2534->PassThruIoctl(chanID, GET_CONFIG, &scl, nullptr)   ;
        baudRate = scl.ConfigPtr[0].Value;                                //
        Log( "PassThruIoctl - GET_CONFIG : baudRate = " + QString::number( baudRate ));

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

        if (j2534->PassThruStartMsgFilter(chanID, PASS_FILTER, &msgMask, &msgPattern, nullptr, &msgId))
        {
            emit Log( "PassThruIoctl - PassThruStartMsgFilter : not ok  " + reportJ2534Error() );
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

    QString reportJ2534Error()
    {
        char err[512];
        int result = j2534->PassThruGetLastError(err);
        return QString::fromLocal8Bit(err) + "  PassThruGetLastError.return = " + QString::number(result) + "\r\n";
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
    void Log(QString);
};
#endif // DMA_H
