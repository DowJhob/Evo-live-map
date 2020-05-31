#ifndef WIDEBAND_INPUT_DEVICE_H
#define WIDEBAND_INPUT_DEVICE_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QElapsedTimer>
enum wb_dev_type {
    TACTRIX,
    SERIAL_INTERFACE
};
class wideband_input_device: public QObject
{
    Q_OBJECT
public:
    wideband_input_device()
    {

    }

public slots:
    void _start()
    {

        _connect();
 //       qDebug() << "wb _connect: " ;
//        _timer = new QTimer(this);
//        _timer->setInterval(200);
//        connect(_timer, &QTimer::timeout, this, [=](){_read();});

        while (true)
        {
            //qDebug() << "wb start: " ;
            if (!flag)
                return;
            if ( _read() )
            {
    //            _timer->start();
            }
            QCoreApplication::processEvents() ;
            QThread::msleep(10);
        }
        //emit AFR("----");
    }
    void _stop()
    {
        flag = false;
        _timer->deleteLater();
    }
private slots:

    virtual void _connect() = 0;
    virtual bool _read() = 0;

private:
    QElapsedTimer t;
    QTimer* _timer;
    QString result;
    bool flag = true;

signals:
    void data(uchar*, ulong);
};

#endif // WIDEBAND_INPUT_DEVICE_H
