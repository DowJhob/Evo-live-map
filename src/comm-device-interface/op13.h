#ifndef OP13_H
#define OP13_H

#include <QObject>
#include <QDebug>
#include "src/libs/ftdi.h"
#include "comm-device-interface.h"

class OP13: public comm_device_interface
{
    Q_OBJECT
public:

    OP13( QString dllName = nullptr);
    ~OP13();

    bool info();

    bool open(Protocol protocol, enum ConnectFlag ConnectFlag);

    bool close();

    bool five_baud_init();
    QByteArray read();
    void write( int lenght );

public slots:

private slots:

private:
    ftdi *_ftdi;
    char in_buf[4096];
    char out_buf[4096];
    int numAttempt = 2;      //количество попыток чтения
    DWORD FT_RxQ_Bytes;
    ulong Reads;

    void ftdi_low_baud_sender(uint baudRate, byte value);

signals:

};

#endif // OP13_H
