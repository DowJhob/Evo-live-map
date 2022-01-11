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
    OP13( QString dllName = nullptr, QString DeviceUniqueID = "");
    virtual ~OP13();
    bool open(Protocol protocol, enum ConnectFlag ConnectFlag, uint baudRate);
    bool close();
    bool info();
    bool five_baud_init();
    QByteArray read(uint lenght);
    void write( int lenght );

public slots:

private:
    ftdi *_ftdi;

    HANDLE hEvent;

    char in_buf[4096];
    char out_buf[4096];
    int numAttempt = 10;      //количество попыток чтения
    DWORD FT_RxQ_Bytes;
    ulong Reads;
    void ftdi_low_baud_sender(uint baudRate, byte value);

    bool ISO9141();
    bool ISO15765();
    bool ISO14230();

    void getDevList();
private slots:

signals:

};

#endif // OP13_H
