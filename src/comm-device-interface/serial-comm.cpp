#include "serial-comm.h"
#include <QCoreApplication>

serial_comm::serial_comm(QObject *parent, QString portName) :  comm_device_interface(parent)
{
    _s_port.setPortName(portName);
    //_ftdi = new ftdi(QCoreApplication::applicationDirPath() + "\\ftd2xx.dll");
    p_in_buff = in_buf;
    p_out_buff = out_buf;
    //qDebug() << "OP13" << DeviceUniqueID;
    //info();
}

serial_comm::~serial_comm()
{
    //delete _ftdi;
}

bool serial_comm::info()
{
    QSerialPortInfo info{_s_port.portName()};



    QString _info = "SerialNumber: " + info.serialNumber() +
            "  Description: " + info.description();
    qDebug() << "info: " << _info;
    //emit Log(info);

    return true;
}

bool serial_comm::open()
{
    this->protocol = protocol;
    //this->baudRate = baudRate;

    p_in_buff = in_buf;
    p_out_buff = out_buf;

    _s_port.setBaudRate(baudRate, QSerialPort::AllDirections);

    if ( _s_port.open(QIODevice::ReadWrite))
    {
        qDebug() << "serial_comm Openened";
        //emit Log(" FT_Open status " + QString::number(s));
        return true;
    }
    //emit Log(" FT_Open failed status " + QString::number(s));
    //qDebug() << " FT_Open failed" << s << QString::fromLatin1( _ftdi->getLastError());
    return false;
}

bool serial_comm::close()
{
    _s_port.close();
    //qDebug() << "OP13::close" << s;
    return true;
}

bool serial_comm::five_baud_init()
{
    qDebug() << "Five baud init";
    low_baud_sender(5, 0x00);                                 //5 baud, 0x00 ecu addr, 0x05 TCU?

    //qDebug() << "Five baud init sended";
    //QThread::msleep(300);                             // W1 60 - 300ms
    //Get bytes waiting to be read
    QByteArray a = read(4);

    QString aa = a.toHex(':');

    qDebug() << "FT_five_baud_ response size" << a.size() << "response" << aa;
    //emit Log("FT_five_baud_ response size " + QString::number(a.size())  + " / response " + aa);

    if (a.size() < 4)
    {
        qDebug() << "FT five_baud failed";
        //emit Log("FT_ five_baud failed: ");
        return false;
    }



    qDebug() << "FT_five_baud_OK" << aa;
    //emit Log("FT_five_baud_OK: " + aa);
    return true;
}

bool serial_comm::setProgLine()
{
return true;
}

QByteArray serial_comm::read(uint lenght)
{
    _s_port.read(in_buf, lenght);
    QByteArray a = QByteArray( (char*)in_buf, lenght);
    //    qDebug() << "Readed bytes " << a.toHex(':') << Reads << endl;
    return a;
}

void serial_comm::write(int lenght)
{
    _s_port.write(p_out_buff, lenght);
    _s_port.read(in_buf, lenght);    //читаем эхо
    //qDebug() << "Echo readed bytes " << QByteArray(p_in_buff, lenght).toHex(':') << Reads << endl;
}

void serial_comm::low_baud_sender(uint baudRate, byte value)
{
    byte p;
    uint t = 1000/baudRate;

    _s_port.setBreakEnabled(true);
    Sleep(t);    //старт бит
    for (int i = 0; i <= 7; i++)
    {
        p = value; //??
        p = p & 0x01;
        if (p == 1)
        {
            _s_port.setBreakEnabled(false);  //1
            Sleep(t);
        }
        else
        {
            _s_port.setBreakEnabled(true);   //0
            Sleep(t);
        }
        value = value >> 1;
    }
    _s_port.setBreakEnabled(false);
}

bool serial_comm::ISO9141()
{
    return true;
}

bool serial_comm::ISO15765()
{
    return true;
}

bool serial_comm::ISO14230()
{
    return true;
}
