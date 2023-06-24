#ifndef FTDI_COMM_H
#define FTDI_COMM_H

#include <QDebug>
#include <QTimer>
#include "src/libs/ftdi.h"
#include "comm-device-interface.h"

//#include "pollhelper.h"

class FTDI_comm: public comm_device_interface
{
    Q_OBJECT
public:
    FTDI_comm(QObject *parent, QString dllName = nullptr, QString DeviceDesc = "", QString DeviceUniqueID = "");
    virtual ~FTDI_comm();
    bool open(Protocol protocol, enum ConnectFlag ConnectFlag, uint baudRate);
    bool close();
    bool info();
    bool five_baud_init();
    bool setProgLine();
    QByteArray read(uint lenght);
    void write( int lenght );

private:
    ftdi *_ftdi;

    char in_buf[4096];
    char out_buf[4096];
    void low_baud_sender(uint baudRate, byte value);

    bool ISO9141();
    bool ISO15765();
    bool ISO14230();

    void getDevList();

};

#endif // FTDI_COMM_H
