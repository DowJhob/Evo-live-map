#ifndef WBLOGGER_H
#define WBLOGGER_H

#include <QObject>
#include <QThread>

#include <QTimer>


#include "wideband/commdevicewb-interface.h"
#include "wideband/serialwb.h"
#include "wideband/wb-proto.h"

class wbLogger:public QObject, public wbProto
{
    Q_OBJECT
public:
    wbLogger();

public slots:
    void setWB(commDeviceWB *cdWB);
    void setProto(int proto);
    void start_stop(bool start);
    void start();

    void stop();

private:
    commDeviceWB *cdWB = nullptr;
    QTimer *pollTimer;
    int pollInt = 20;
    int _wbProto = 0;

    void poll();

signals:
    void logReady(float);
};

#endif // WBLOGGER_H
