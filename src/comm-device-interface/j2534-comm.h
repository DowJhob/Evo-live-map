#ifndef J2534_COMM_H
#define J2534_COMM_H

#include <QDebug>

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
class j2534_comm : public comm_device_interface//,
        //public pollHelper
{
    //friend class OP20;
public:
    // J2534

    unsigned long devID = 0;
    unsigned long chanID = 0;

    j2534_comm(QObject *parent, QString dllName = nullptr, QString DeviceDesc = "", QString DeviceUniqueID = "");
    virtual ~j2534_comm();

    bool info();
    bool open();
    bool close();
    bool five_baud_init();

    bool setProgLine();

    QByteArray read(uint readTimeout = 0);
    void write(int lenght);

    PassThru *j2534;

    bool ISO9141();
    bool ISO15765();
    bool ISO14230();

    Message tx_msg = {};
    Message rx_msg = {};
    unsigned long msgId = 0;

private:
    unsigned long NumMsgs;


    bool commonConnect();

    bool get_serial_num(unsigned long devID, char* serial);

};

#endif // J2534_COMM_H
