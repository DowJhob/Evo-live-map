#ifndef SERIALWB_H
#define SERIALWB_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "commdevicewb-interface.h"

class serialWB: public commDeviceWB//, public QObject//
        //, public QSerialPort
{
    //Q_OBJECT
public:
    QString DeviceDesc;// + " / " + cdWB->DeviceUniqueID
    explicit serialWB(QSerialPortInfo portInfo, QObject *parent);

    virtual bool isClosed();
    virtual bool openWB(uint baudRate);
    virtual bool connectWB(unsigned int baudRate);
    virtual bool closeWB();
    virtual QByteArray readWB();

private:
    QSerialPortInfo portInfo;
    QSerialPort port;

};

#endif // SERIALWB_H
