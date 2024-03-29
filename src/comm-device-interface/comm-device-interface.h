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

class comm_device_interface //: public QObject
{
    //Q_OBJECT
public:
    //deviceType devType = deviceType::undef;
    QString dllName;
    QString DeviceUniqueID;
    QString DeviceDesc;
    char* p_in_buff;
    char* p_out_buff;

    Protocol protocol;
    ConnectFlag ConnectFlag;
    unsigned int  baudRate = 0;

    unsigned long _readTimeout = 2000;
    unsigned long writeTimeout = 0;

    explicit comm_device_interface(QString dllName = nullptr, QString DeviceDesc = "", QString DeviceUniqueID = "");
    virtual ~comm_device_interface();

    virtual bool open(Protocol protocol, enum ConnectFlag ConnectFlag, uint baudRate) = 0;
    virtual bool close() = 0;
    virtual bool info() = 0;

    virtual bool five_baud_init() = 0;

    bool connect();

//public slots:
    virtual QByteArray read(uint lenght = 0) = 0;
    virtual void write(int lenght ) = 0;
    void setBaudRate(unsigned int BaudRate);

    uint getBaudRate();

//private slots:

private:
    virtual bool ISO9141() = 0;
    virtual bool ISO15765() = 0;
    virtual bool ISO14230() = 0;

//signals:
    //void readyInterface(bool);
    //void readyRead(QByteArray);

    //void Log(QString);
};

#endif // COMM_DEVICE_INTERFACE_H
