#include "serialwb.h"

serialWB::serialWB(QSerialPortInfo portInfo, QObject *parent):QSerialPort(portInfo, parent)
{

}

bool serialWB::isClosed()
{
    qDebug() << "=========== serialWB::isClosed ================";
    return !isOpen();
}

bool serialWB::openWB(uint baudRate)
{
    qDebug() << "=========== serialWB::openWB ================";
    setBaudRate(baudRate, QSerialPort::AllDirections);
    qDebug() << "=========== serialWB::openWB2 ================";
    open(QIODevice::ReadWrite);
    qDebug() << "=========== serialWB::openWB3 ================";
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
    return readAll();
}
