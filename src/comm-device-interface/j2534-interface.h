#ifndef J2534_INTERFACE_H
#define J2534_INTERFACE_H

#include <QDebug>
#include <QMutex>

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
    friend class OP20;
public:
    // J2534
    volatile
    uint countUSE = 0; // if higher zero device in use
    unsigned long devID = 0;   // использую как индикатор открытости, если ноль не опен!

    j2534_interface( QString dllName = nullptr, QString DeviceDesc = "", QString DeviceUniqueID = "");
    virtual ~j2534_interface();

    QMutex mu;

    void setUse()
    {
        mu.lock();
        countUSE++;
        mu.unlock();
    }
    bool resetUse()
    {
        mu.lock();
        bool b = (--countUSE <= 0);
        if (b)
            countUSE = 0;
        mu.unlock();
        return b;
    }
    bool isNotUse()
    {
        mu.lock();
        bool b = (countUSE == 0);
        mu.unlock();
        return b;
    }

    bool info();
    bool open(Protocol protocol, enum ConnectFlag ConnectFlag, uint baudRate);
    bool close();
    bool five_baud_init();
    QByteArray read(uint lenght = 0);
    void write(int lenght);

private:
    PassThru *j2534;
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

#endif // J2534_INTERFACE_H
