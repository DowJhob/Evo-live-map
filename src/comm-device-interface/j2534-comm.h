#ifndef J2534_COMM_H
#define J2534_COMM_H

#include <QDebug>
#include <QMutex>

#include "src/libs/j2534passthru.h"
#include "comm-device-interface.h"

//#include "pollhelper.h"

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
class j2534_comm : public comm_device_interface//,
        //public pollHelper
{
    friend class OP20;
public:
    // J2534
    volatile
    uint countUSE = 0; // if higher zero device in use
    unsigned long devID = 0;   // использую как индикатор открытости, если ноль не опен!

    j2534_comm(QObject *parent, QString dllName = nullptr, QString DeviceDesc = "", QString DeviceUniqueID = "");
    virtual ~j2534_comm();

    QMutex mu;

    void setUse();
    bool resetUse();
    bool isNotUse();

    bool info();
    bool open(Protocol protocol, enum ConnectFlag ConnectFlag, uint baudRate);
    bool close();
    bool five_baud_init();
    QByteArray read(uint lenght = 0);
    void write(int lenght);

    PassThru *j2534;
private:
    unsigned long chanID = 0;
    unsigned long NumMsgs;

    unsigned long msgId = 0;
    Message tx_msg = {};
    Message rx_msg = {};
    bool ISO9141();
    bool ISO15765();
    bool ISO14230();

    bool get_serial_num(unsigned long devID, char* serial);

};

#endif // J2534_COMM_H
