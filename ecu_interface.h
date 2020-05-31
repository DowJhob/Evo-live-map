#ifndef ECU_COMM_H
#define ECU_COMM_H

#include <QObject>
#include <QThread>
#include <QtEndian>
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>
#include "libs/J2534.h"
#include "wideband/wideband_input_device.h"

class ECU_interface:public QObject
{
    Q_OBJECT
public:
    uchar* in_buff;
    uchar* out_buff;

    char delay_after_command;

    explicit ECU_interface(TCHAR *dllName = nullptr)
    {
        this->dllName = dllName;
    }
    ~ECU_interface()
    {

    }

    virtual bool _connect(unsigned long protocol, unsigned long ConnectFlag, unsigned int baudRate) = 0;
    virtual bool disconnect() = 0;
    virtual bool e7_connect() = 0;
    virtual bool five_baud_init() = 0;
    virtual void read() = 0;
    virtual void write( uint count) = 0;
    TCHAR *dllName;
    virtual void close() = 0;
    void sendDMAcomand(char command, quint32 addr, quint16 count, char* buf = nullptr)
    {
        out_buff[0] = command;
        write( 1 );
        QThread::msleep(delay_after_command);
        uchar * const p_addr = reinterpret_cast<uchar * const>(&addr);
        uchar * const p_count = reinterpret_cast<uchar * const>(&count);
//        qDebug() << p_addr[0] << p_addr[1] << p_addr[2] << p_addr[3];

        out_buff[0] = p_addr[3];
        out_buff[1] = p_addr[2];
        out_buff[2] = p_addr[1];
        out_buff[3] = p_addr[0];

        out_buff[4] = p_count[1];
        out_buff[5] = p_count[0];



//        out_buff[0] = (addr & 0xFF000000) >> 24;
//        out_buff[1] = (addr & 0xFF0000) >> 16;
//        out_buff[2] = (addr & 0xFF00) >> 8;
//        out_buff[3] = (addr & 0xFF);
//        out_buff[4] = (count & 0xFF00) >> 8;
//        out_buff[5] = (count & 0xFF);
//        qDebug() << out_buff[0] << out_buff[1] << out_buff[2] << out_buff[3];
        write( 6 );
        if ( buf != nullptr)
        {
            memcpy(out_buff, buf, count);
            write( count);
        }
    }

public slots:

    bool init()
    {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &ECU_interface::timerTick);
        timer->setInterval(50);
        return _init();
    }

    void setLoggingInterval(int im)
    {
        timer->setInterval(im);
    }

    void startLogger(quint32 addr, quint16 len)
    {
        this->addr = addr;
        this->len = len;

        timer->start();
    }

    void stopLogger()
    {
        timer->stop();
    }

    void timerTick()
    {
        QElapsedTimer t;
        t.start();
        timer->stop();
        sendDMAcomand(0xE0, addr, len);
        read();
        timer->start();
        emit readyRead(a.fromRawData((char*)in_buff, len));
    }

    virtual void start_tactrix_wb() = 0;
    virtual void stop_tactrix_wb() = 0;

private slots:
    virtual bool _init() = 0;

private:
    QTimer* timer;
    quint32 addr;
    quint16 len;
    QByteArray a;

signals:
    void readyRead(QByteArray);
    void interfaceReady();
    void DMA_Ready();
    void AFR(QString);
    void Log(QString);
};

#endif // ECU_COMM_H
