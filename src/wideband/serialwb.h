#ifndef SERIALWB_H
#define SERIALWB_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "commdevicewb-interface.h"

class serialWB: public commDeviceWB//
        //, public QSerialPort
{
    //Q_OBJECT
public:
    QString DeviceDesc;// + " / " + cdWB->DeviceUniqueID
    explicit serialWB(QSerialPortInfo portInfo, QObject *parent);

    bool isClosed();
    bool openWB(uint baudRate);
    bool connectWB(unsigned int baudRate);
    bool closeWB();
    QByteArray readWB();

private:
    QSerialPortInfo portInfo;
    QSerialPort port;

};

#endif // SERIALWB_H
