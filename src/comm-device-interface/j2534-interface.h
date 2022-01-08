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
    unsigned long NumMsgs;


    j2534_interface( QString dllName = nullptr, QString DeviceUniqueID = "");
    virtual ~j2534_interface();

    bool info();
    bool open(Protocol protocol, enum ConnectFlag ConnectFlag);
    bool close();

    bool connect();

    bool five_baud_init();

    QByteArray read();

    void write(int lenght);



    QString reportJ2534Error();

public slots:

private slots:

private:

    unsigned long msgId = 0;
    Message  tx_msg = {};
    Message  rx_msg = {};
    bool setFilter(Protocol protocol);
    long set_filter(PassThru::FilterType type, Message *msgMask, Message *msgPattern, Message *msgFlowcontrol);
    bool get_serial_num(unsigned long devID, char* serial);

signals:

};

#endif // J2534_INTERFACE_H
