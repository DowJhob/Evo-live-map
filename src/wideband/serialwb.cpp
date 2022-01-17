#include "serialwb.h"

serialWB::serialWB(QSerialPortInfo portInfo, QObject *parent)//:QSerialPort(portInfo, parent)
{
    port.setPort(portInfo);
    connect(&port, &QSerialPort::readyRead, this, [this](){
        QByteArray a = readWB();
        if (a.size() > 0)
            emit readyRead(a);
    });
   // &commDeviceWB::readyRead);
}

bool serialWB::isClosed()
{
    //qDebug() << "=========== serialWB::isClosed ================";
    return !port.isOpen();
}

bool serialWB::openWB(uint baudRate)
{
    qDebug() << "=========== serialWB::openWB ================";
    port.setBaudRate(baudRate, QSerialPort::AllDirections);
    //qDebug() << "=========== serialWB::openWB2 ================";
    port.open(QIODevice::ReadWrite);
    //qDebug() << "=========== serialWB::openWB3 ================";
    return true;
}

bool serialWB::connectWB(unsigned int baudRate)
{

    return true;
}

bool serialWB::closeWB()
{

    return true;
}

QByteArray serialWB::readWB()
{
    return port.readAll();
}
