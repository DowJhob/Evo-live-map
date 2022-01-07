#include "common-serial-interface.h"

common_serial_interface::common_serial_interface() : QObject()
{
    connect(this, &common_serial_interface::__start, this, &common_serial_interface::_start, Qt::QueuedConnection);
    this_thread = new QThread();
    connect(this, &QObject::destroyed, this_thread, &QThread::quit);            // Когда удалим объект остановим поток
    connect(this_thread, &QThread::finished, this_thread, &QThread::deleteLater);  // Когда остановим поток удалим его
    moveToThread(this_thread);
}

common_serial_interface::~common_serial_interface()
{
    serial_input_polling_timer->deleteLater();
}

void common_serial_interface::start()
{
    this_thread->start();  // запустим поток,
    emit __start();
}

void common_serial_interface::_start()
{
    open();
    //connect(this, &QThread::finished, this, &QThread::deleteLater);
    serial_input_polling_timer = new QTimer();
    connect(serial_input_polling_timer, &QTimer::timeout, this, &common_serial_interface::tick);
    serial_input_polling_timer->setSingleShot(true);
    serial_input_polling_timer->setInterval(20);
    serial_input_polling_timer->start();
}

void common_serial_interface::tick()
{
    read();
    serial_input_polling_timer->start();
}
