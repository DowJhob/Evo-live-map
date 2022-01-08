#ifndef J2534_INTERFACE_H
#define J2534_INTERFACE_H

//#include <QObject>
#include <QDebug>

//#include "src/libs/J2534.h"
#include "src/libs/j2534passthru.h"

#include "comm-device-interface.h"

typedef struct
{
    unsigned int length;
    unsigned int svcid;
    unsigned short infosvcid;
} inbuf;
typedef struct
{
    unsigned int length;
    unsigned char data[256];
} outbuf;

using namespace J2534;
class j2534_interface : public comm_device_interface
{
    Q_OBJECT
public:
    // J2534
    //J2534 *j2534;
    PassThru *j2534;


    unsigned long devID;
    unsigned long chanID;

    j2534_interface( QString dllName = nullptr, QString DeviceUniqueID = "");
    virtual ~j2534_interface();

    bool init();
    bool open();
    bool close();

    bool connect(Protocol protocol, enum ConnectFlag ConnectFlag);

    void info();

    bool five_baud_init();

    QByteArray read();

    void write(int lenght);

    Message  tx_msg = {};
    Message  rx_msg = {};

    unsigned long NumMsgs;

    QString reportJ2534Error();

public slots:

private slots:

private:
    unsigned long msgId = 0;
    bool setFilter(Protocol protocol);
    bool get_channel(Protocol protocol, enum ConnectFlag ConnectFlag, unsigned int baudRate);
    bool set_config(const SArray<const Config> *scl);
    long set_filter(PassThru::FilterType type, Message *msgMask, Message *msgPattern, Message *msgFlowcontrol);
    bool get_serial_num(unsigned long devID, char* serial);

signals:

};
//    void e10_connect()
//    {
//        protocol = ISO15765;
//        ConnectFlag = 0;
//        baudRate = 500000;
//        //==========================================   get  chanID  ================================
//        get_channel(protocol, ConnectFlag, baudRate);


//        j2534->PassThruIoctl(chanID, CLEAR_TX_BUFFER, nullptr, nullptr);
//        j2534->PassThruIoctl(chanID, CLEAR_RX_BUFFER, nullptr, nullptr);
//        j2534->PassThruIoctl(chanID, CLEAR_MSG_FILTERS, nullptr, nullptr);

//        //-======================================== SET CONFIG ===========================================
//        SCONFIG_LIST scl;
//        SCONFIG scp[2] = { {ISO15765_BS   , 0x20},     //For protocol ID of ISO 15765, this sets the block size the interface should report to the vehicle for receiving segmented transfers.
//                           {ISO15765_STMIN, 0x00} }; //For protocol ID of ISO 15765, this sets the separation time the interface should report to the vehicle for receiving segmented transfers.
//        scl.NumOfParams = 2;
//        scl.ConfigPtr = scp;
//        set_config(&scl);
//        // ============================ setup the filter(s) =========================
//        unsigned long msgId;

//        PASSTHRU_MSG msgMask, msgPattern, msgFlowControl;

//        msgMask.ProtocolID = ISO15765;
//        msgMask.RxStatus   = 0;
//        msgMask.TxFlags    = ISO15765_FRAME_PAD;
//        msgMask.Timestamp  = 0;
//        msgMask.DataSize   = 4;
//        msgMask.ExtraDataIndex = 0;
//        msgMask.Data[0] = 0xFF;
//        msgMask.Data[1] = 0xFF;
//        msgMask.Data[2] = 0xFF;
//        msgMask.Data[3] = 0xFF;

//        msgPattern.ProtocolID = ISO15765;
//        msgPattern.RxStatus = 0;
//        msgPattern.TxFlags = ISO15765_FRAME_PAD;
//        msgPattern.Timestamp = 0;
//        msgPattern.DataSize = 4;
//        msgPattern.ExtraDataIndex = 0;
//        msgPattern.Data[0] = 0;
//        msgPattern.Data[1] = 0;
//        msgPattern.Data[2] = 7;
//        msgPattern.Data[3] = 0xE8;

//        msgFlowControl.ProtocolID = ISO15765;
//        msgFlowControl.RxStatus = 0;
//        msgFlowControl.TxFlags = ISO15765_FRAME_PAD;
//        msgFlowControl.Timestamp = 0;
//        msgFlowControl.DataSize = 4;
//        msgFlowControl.ExtraDataIndex = 0;
//        msgFlowControl.Data[0] = 0;
//        msgFlowControl.Data[1] = 0;
//        msgFlowControl.Data[2] = 7;
//        msgFlowControl.Data[3] = 0xE0;

//        msgId = set_filter( FLOW_CONTROL_FILTER, &msgMask, &msgPattern, &msgFlowControl);

//        QThread::msleep(100);
//    }
#endif // J2534_INTERFACE_H
