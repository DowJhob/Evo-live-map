#ifndef ECU_COMM_H
#define ECU_COMM_H

#include <QObject>
#include <QThread>
#include <QtEndian>
#include <QDebug>
#include "libs/J2534.h"

class ECU_Comm:public QObject
{
    Q_OBJECT
public:
    uchar* in_buff = rx_msg[0].Data;
    uchar* out_buff = tx_msg.Data;
    PASSTHRU_MSG  tx_msg = {};
    PASSTHRU_MSG  rx_msg[2] = {};
    PASSTHRU_MSG  inno_rx_msg[2] = {};

    char delay_after_command = 4;
    explicit ECU_Comm()
    {

    }
    ~ECU_Comm()
    {

    }
    virtual bool init() = 0;
    virtual void _connect(unsigned long protocol, unsigned long ConnectFlag, unsigned int baudRate) = 0;
    virtual bool five_baud_init() = 0;
    virtual void read() = 0;
    virtual void write( uint count) = 0;
    virtual void start_tactrix_inno() = 0;
virtual void close() = 0;
    void sendDMAcomand(char command, unsigned long addr, unsigned long count, char* buf = nullptr)
    {
        out_buff[0] = command;
        write( 1 );
        QThread::msleep(delay_after_command);
        out_buff[0] = (addr & 0xFF000000) >> 24;
        out_buff[1] = (addr & 0xFF0000) >> 16;
        out_buff[2] = (addr & 0xFF00) >> 8;
        out_buff[3] = (addr & 0xFF);
        out_buff[4] = (count & 0xFF00) >> 8;
        out_buff[5] = (count & 0xFF);
        write( 6 );
        if ( buf != nullptr)
        {
            memcpy(out_buff, buf, count);
            write( count);
        }
    }
private:

signals:
    void readyRead(QByteArray);
    void AFR(QString);
    void Log(QString);
};

#endif // ECU_COMM_H
