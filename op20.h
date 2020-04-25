#ifndef OP20_H
#define OP20_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include "tactrix_inno.h"
#include "libs/J2534.h"
#include "ecu_interface.h"


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

class OP20: public ECU_interface
{
    Q_OBJECT
public:
    unsigned long chanID;
    OP20(TCHAR *dllName = nullptr)
    {
        this->dllName = dllName;
    }
    ~OP20()
    {
        stop_tactrix_inno();
        close();
        delete j2534;
    }
    void _connect(unsigned long protocol, unsigned long ConnectFlag, unsigned int baudRate)     //get  chanID
    {

        //получаем дескриптор
        if (j2534->PassThruOpen(nullptr, &devID))         // Get devID
        {// читаем и выводим ошибку
            emit Log( "PassThruOpen: not ok  | " + reportJ2534Error() );
            //  return false;
        }
        emit Log( "PassThruOpen:  devID = " + QString::number(devID) );


        start_tactrix_inno();



        this->protocol = protocol;
        this->ConnectFlag = ConnectFlag;
        this->baudRate = baudRate;

        tx_msg.ProtocolID = protocol;
        tx_msg.TxFlags = ConnectFlag;
        rx_msg[0].ProtocolID = protocol;
        rx_msg[1].ProtocolID = protocol;

        if (j2534->PassThruConnect(devID, protocol, ConnectFlag, baudRate, &chanID))
        {
            emit Log( "PassThruConnect: not ok  | " + reportJ2534Error() );
            return;
        }
        emit Log( "PassThruConnect: OK  -  chanel ID: " + QString::number(chanID));
    }
    void set_config(SCONFIG_LIST *scl)
    {
        if (j2534->PassThruIoctl(chanID, SET_CONFIG, scl, nullptr))
        {
            emit Log( "PassThruIoctl - SET_CONFIG : not ok  " + reportJ2534Error() );
            //         return false;
        }
    }
    void e7_connect()
    {
        protocol = ISO9141_K;
        ConnectFlag = ISO9141_NO_CHECKSUM;
        baudRate = 15625;
        //==========================================   get  chanID  ================================
        _connect(protocol, ConnectFlag, baudRate);
        //-======================================== SET CONFIG ===========================================
        SCONFIG_LIST scl;
        SCONFIG scp[6] = { {DATA_RATE, baudRate}, {P1_MAX, 0}, {P2_MAX, 0}, {P3_MIN, 0}, {P4_MIN, 0}, {LOOPBACK, 0} };        // set timing
        scl.NumOfParams = 6;
        scl.ConfigPtr = scp;
        set_config(&scl);
        //==================================   5 baud init  ========================================
        five_baud_init();
        // ============================ setup filter(s) =========================
        PASSTHRU_MSG msgMask, msgPattern;
        unsigned long msgId;
        msgMask.ProtocolID = protocol;
        msgMask.DataSize = 1;
        msgPattern.ProtocolID = protocol;
        msgPattern.DataSize = 1;
        msgMask.Data[0] = 0; // mask the first byte to 0
        msgPattern.Data[0] = 0; // match it with 0 (i.e. pass everything)
        msgId = set_filter(PASS_FILTER, &msgMask, &msgPattern, nullptr);

    }
    void e10_connect()
    {
        protocol = ISO15765;
        ConnectFlag = 0;
        baudRate = 500000;
        //==========================================   get  chanID  ================================
        _connect(protocol, ConnectFlag, baudRate);


        j2534->PassThruIoctl(chanID, CLEAR_TX_BUFFER, nullptr, nullptr);
        j2534->PassThruIoctl(chanID, CLEAR_RX_BUFFER, nullptr, nullptr);
        j2534->PassThruIoctl(chanID, CLEAR_MSG_FILTERS, nullptr, nullptr);

        //-======================================== SET CONFIG ===========================================
        SCONFIG_LIST scl;
        SCONFIG scp[2] = { {ISO15765_BS   , 0x20},     //For protocol ID of ISO 15765, this sets the block size the interface should report to the vehicle for receiving segmented transfers.
                           {ISO15765_STMIN, 0x00} }; //For protocol ID of ISO 15765, this sets the separation time the interface should report to the vehicle for receiving segmented transfers.
        scl.NumOfParams = 2;
        scl.ConfigPtr = scp;
        set_config(&scl);
        // ============================ setup the filter(s) =========================
        unsigned long msgId;

        PASSTHRU_MSG msgMask, msgPattern, msgFlowControl;

        msgMask.ProtocolID = ISO15765;
        msgMask.RxStatus   = 0;
        msgMask.TxFlags    = ISO15765_FRAME_PAD;
        msgMask.Timestamp  = 0;
        msgMask.DataSize   = 4;
        msgMask.ExtraDataIndex = 0;
        msgMask.Data[0] = 0xFF;
        msgMask.Data[1] = 0xFF;
        msgMask.Data[2] = 0xFF;
        msgMask.Data[3] = 0xFF;

        msgPattern.ProtocolID = ISO15765;
        msgPattern.RxStatus = 0;
        msgPattern.TxFlags = ISO15765_FRAME_PAD;
        msgPattern.Timestamp = 0;
        msgPattern.DataSize = 4;
        msgPattern.ExtraDataIndex = 0;
        msgPattern.Data[0] = 0;
        msgPattern.Data[1] = 0;
        msgPattern.Data[2] = 7;
        msgPattern.Data[3] = 0xE8;

        msgFlowControl.ProtocolID = ISO15765;
        msgFlowControl.RxStatus = 0;
        msgFlowControl.TxFlags = ISO15765_FRAME_PAD;
        msgFlowControl.Timestamp = 0;
        msgFlowControl.DataSize = 4;
        msgFlowControl.ExtraDataIndex = 0;
        msgFlowControl.Data[0] = 0;
        msgFlowControl.Data[1] = 0;
        msgFlowControl.Data[2] = 7;
        msgFlowControl.Data[3] = 0xE0;

        msgId = set_filter( FLOW_CONTROL_FILTER, &msgMask, &msgPattern, &msgFlowControl);

        QThread::msleep(100);
    }

    ulong set_filter(ulong type, PASSTHRU_MSG *msgMask, PASSTHRU_MSG *msgPattern, PASSTHRU_MSG *msgFlowcontrol)
    {
        ulong msgId;
        if (j2534->PassThruStartMsgFilter(chanID, type, msgMask, msgPattern, msgFlowcontrol, &msgId))
        {
            emit Log( "PassThruIoctl - PassThruStartMsgFilter : not ok  " + reportJ2534Error() );
            //           return false;
        }
        return msgId;
    }

    bool five_baud_init()
    {
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
            emit Log( "PassThruIoctl - FIVE_BAUD_INIT : not ok  | " + reportJ2534Error() );
            return false;
        }
        Log( "PassThruIoctl - FIVE_BAUD_INIT : OK " + QString::number(KeyWord[0], 16) + " " + QString::number(KeyWord[1], 16) + " " + QString::number(KeyWord[2], 16));

        //        j2534->PassThruIoctl(chanID, GET_CONFIG, &scl, nullptr)   ;
        //        baudRate = scl.ConfigPtr[0].Value;                                //
        Log( "PassThruIoctl - GET_CONFIG : baudRate = " + QString::number( baudRate ));
        return true;
    }

    void read()
    {
        NumMsgs = 1;
        rx_msg[0].RxStatus = 0;
        do
            j2534->PassThruReadMsgs(chanID, &rx_msg[0], &NumMsgs, _readTimeout);
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

    void close()
    {
        // shut down the channel
        if (j2534->PassThruDisconnect(chanID))
        {
            reportJ2534Error();
        }

        // close the device
        if (j2534->PassThruClose(devID))
        {
            reportJ2534Error();
        }
    }
public slots:
    bool init()
    {
        j2534 = new J2534(dllName) ;
        delay_after_command = 4;
        if (!j2534->init())
        {
            emit Log( "can't connect to J2534 DLL." );
            return false;
        }
        //получаем дескриптор
        if (j2534->PassThruOpen(nullptr, &devID))         // Get devID
        {// читаем и выводим ошибку
            emit Log( "PassThruOpen: not ok  | " + reportJ2534Error() );
            //  return false;
        }
        emit Log( "PassThruOpen:  devID = " + QString::number(devID) );
        //читаем версию и прочюю требуху
        char strApiVersion[256];
        char strDllVersion[256];
        char strFirmwareVersion[256];
        char strSerial[256];
        if ( j2534->PassThruReadVersion(strApiVersion, strDllVersion, strFirmwareVersion, devID) )
        {
            emit Log( "PassThruReadVersion: not ok  | " + reportJ2534Error() );
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
            emit Log( "get_serial_num: not ok  | " + reportJ2534Error() );
        emit Log( "init_j2534 OK" );
        // close the device
        if (j2534->PassThruClose(devID))
        {
            reportJ2534Error();
        }
        emit interfaceReady();

        return true;
    }

    void start_tactrix_inno()
    {
        inno_thread = new QThread();
        _inno_interface = new tactrix_inno(j2534, devID);
        connect(_inno_interface, SIGNAL(AFR(QString)), SIGNAL(AFR(QString)));
        connect(inno_thread, &QThread::started, _inno_interface, &inno_interface::start);
        connect(inno_thread, &QThread::finished, [=](){inno_thread->deleteLater();});

        _inno_interface->moveToThread(inno_thread);
        inno_thread->start();
    }

    void stop_tactrix_inno()
    {
        if (_inno_interface != nullptr )
        {
            _inno_interface->stop();
            _inno_interface->AFR("----");
            _inno_interface->deleteLater();
            inno_thread->quit();
            inno_thread->wait(100);
        }
    }
private:

    // J2534
    J2534 *j2534;

    unsigned int baudRate = 15625;
    unsigned long _readTimeout = 225;
    unsigned long writeTimeout = 0;

    unsigned long devID;

    unsigned long chanID_INNO;
    unsigned long NumMsgs;
    unsigned long protocol_inno = ISO9141_INNO;
    unsigned long protocol = ISO9141_K;
    unsigned long ConnectFlag = ISO9141_NO_CHECKSUM;  //        || ISO9141_K_LINE_ONLY ;
    inno_interface *_inno_interface = nullptr;
    QThread *inno_thread;

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
