#ifndef ECU_COMM_H
#define ECU_COMM_H

#include <QObject>

class ECU_Comm:public QObject
{
    Q_OBJECT
public:
    explicit ECU_Comm()
    {

    }
    ~ECU_Comm()
    {

    }

    virtual QString connect(unsigned long protocol, //ConnectFlag,
                            unsigned int baudRate) = 0;
    virtual bool five_baud_init() = 0;
    virtual void read() = 0;
    virtual void write(char* buf, uint count) = 0;

    void sendDMAcomand(char command, unsigned long addr, unsigned long count, char* buf = nullptr)
    {
        char out[6];
        write(&command, 1);
//        QThread::msleep(delay_after_command);
        out[0] = (addr & 0xFF000000) >> 24;
        out[1] = (addr & 0xFF0000) >> 16;
        out[2] = (addr & 0xFF00) >> 8;
        out[3] = (addr & 0xFF);
        out[4] = (count & 0xFF00) >> 8;
        out[5] = (count & 0xFF);
        write(out, 6);
        if ( buf != nullptr)
            write(buf, count);
    }
private:
    virtual bool init() = 0;

signals:
    void readyRead(QByteArray);
    void Log(QString);
};

#endif // ECU_COMM_H
