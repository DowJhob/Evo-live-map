#ifndef COMM_DEVICE_INTERFACE_H
#define COMM_DEVICE_INTERFACE_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QtEndian>
#include <QElapsedTimer>
//#include "src/libs/J2534.h"

#include "src/types.h"


class comm_device_interface : public QObject
{
    Q_OBJECT
public:
    QString dllName;
    QString DeviceUniqueID;
    char* p_in_buff;
    char* p_out_buff;

    Protocol protocol_inno = Protocol::ISO9141_INNO;
    Protocol protocol = Protocol::ISO9141;
    ConnectFlag ConnectFlag = ConnectFlag::ISO9141NoChecksum;  //        || ISO9141_K_LINE_ONLY ;

    unsigned int  baudRate = 62500;
    unsigned long _readTimeout = 2000;
    unsigned long writeTimeout = 0;

    explicit comm_device_interface(QString dllName = nullptr, QString DeviceUniqueID = "");
    virtual ~comm_device_interface();

    virtual bool init() = 0;
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool connect(Protocol protocol, enum ConnectFlag ConnectFlag) = 0;
    virtual bool five_baud_init() = 0;

public slots:
    virtual QByteArray read() = 0;
    virtual void write(int lenght ) = 0;
    void setBaudRate(int baudRate)
    {
        this->baudRate = baudRate;
    }

private slots:

private:

signals:
    void readyInterface(bool);
    void readyRead(QByteArray);

    void Log(QString);
};

#endif // COMM_DEVICE_INTERFACE_H
