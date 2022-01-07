#ifndef COMMON_SERIAL_INTERFACE_H
#define COMMON_SERIAL_INTERFACE_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QThread>

class common_serial_interface : public QObject
{
    Q_OBJECT
public:
    explicit common_serial_interface();
    virtual ~common_serial_interface();

    void start();
    void _start();

private:
    QThread *this_thread = nullptr;
    QTimer *serial_input_polling_timer = nullptr;
    void tick();
    virtual void open() = 0;

private slots:
    virtual QByteArray read() = 0;
    //void _start();

signals:
    void __start();
    void AFR(float);

};

#endif // COMMON_SERIAL_INTERFACE_H
