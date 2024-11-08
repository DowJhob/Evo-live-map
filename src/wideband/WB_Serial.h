#ifndef WB_SERIAL_H
#define WB_SERIAL_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "WB_I_ComDev.h"

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

    void startLog(int baudRate)
    {
        qDebug() << "==================== serialWB::startLog ================================== baudRate" << baudRate;
    }

    void stopLog()
    {
        qDebug() << "==================== serialWB::stopLog ==================================";
    }


private:
    QSerialPortInfo portInfo;
    QSerialPort port;

};

#endif // WB_SERIAL_H
