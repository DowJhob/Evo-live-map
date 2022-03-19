#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "src/libs/ftdi.h"
#include "comm-device-interface.h"

//#include "pollhelper.h"

class serial_comm: public comm_device_interface
{
    Q_OBJECT
public:
    serial_comm(QObject *parent, QString portName);
    virtual ~serial_comm();
    bool open(Protocol protocol, enum ConnectFlag ConnectFlag, uint baudRate);
    bool close();
    bool info();
    bool five_baud_init();
    QByteArray read(uint lenght);
    void write( int lenght );

private:
    QSerialPort _s_port;
    //ftdi *_ftdi;

    char in_buf[4096];
    char out_buf[4096];
    void low_baud_sender(uint baudRate, byte value);

    bool ISO9141();
    bool ISO15765();
    bool ISO14230();

    void getDevList();

};

#endif // SERIAL_COMM_H
