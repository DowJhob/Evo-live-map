#ifndef OP20_H
#define OP20_H

#include <QObject>
#include <QDebug>
#include "libs/J2534.h"
#include "ecu_comm.h"

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
class OP20: public ECU_Comm
{
    Q_OBJECT
public:
    OP20()
    {
        j2534 = new J2534 ;
        delay_after_command = 4;
    }
    bool init()
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


        emit Log( "common_init_j2534 OK" );

        //        init_inno();
        return true;
    }

    void connect(unsigned long protocol, //ConnectFlag,
                    unsigned int baudRate)
    {
        this->protocol = protocol; this->baudRate = baudRate;
        if (j2534->PassThruConnect(devID, protocol, ConnectFlag, baudRate, &chanID))
        {
            emit Log( "PassThruConnect: not ok" + reportJ2534Error() );
            return;
        }
        emit Log( "PassThruConnect: OK  -  chanel ID: " + QString::number(chanID));
    }
    bool five_baud_init()
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

    void read()
    {
        NumMsgs = 1;
        rx_msg[0].RxStatus = 0;
        do
            j2534->PassThruReadMsgs(chanID, &rx_msg[0], &NumMsgs, magic_adder_readTimeout+200);
        while(rx_msg[0].RxStatus == START_OF_MESSAGE);
//        memcpy(buffer, rx_msg[0].Data, rx_msg[0].DataSize);
//        qDebug()<< rx_msg[0].DataSize;

        //emit readyRead(QByteArray::fromRawData( (char*)rx_msg[0].Data, rx_msg[0].DataSize));
        //return QByteArray::fromRawData( (char*)rx_msg[0].Data, rx_msg[0].DataSize);
    }

    void write( uint count )
    {
        NumMsgs = 1;
        tx_msg.DataSize = count;
//        memcpy(tx_msg.Data, buf, count);

        j2534->PassThruWriteMsgs(chanID, &tx_msg, &NumMsgs, writeTimeout);
    }

private:

    // J2534
    J2534 *j2534;
    unsigned int baudRate = 15625;
    unsigned long magic_adder_readTimeout = 25;
    unsigned long writeTimeout = 0;


    unsigned long devID;
    unsigned long chanID;
    unsigned long chanID_INNO;
    unsigned long NumMsgs ;
    unsigned long protocol_inno = ISO9141_INNO;
    unsigned long protocol = ISO9141_K;
    unsigned long ConnectFlag = ISO9141_NO_CHECKSUM;  //        || ISO9141_K_LINE_ONLY ;
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

};

#endif // OP20_H
