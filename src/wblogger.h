#ifndef WBLOGGER_H
#define WBLOGGER_H

#include <QObject>
#include <QThread>

#include <QTimer>


#include "wideband/commdevicewb-interface.h"
#include "wideband/serialwb.h"
#include "wideband/wb-proto.h"

class wbLogger:public QObject//, public wbProto
{
    Q_OBJECT
public:
    wbLogger();

public slots:
    void setWB(commDeviceWB *cdWB);
    void setProto(wbProto *proto);
    void start_stop(bool start);
    void start();

    void stop();

private:
    commDeviceWB *cdWB = nullptr;
    QTimer *pollTimer;
    int pollInt = 20;
    wbProto *_wbProto = nullptr;

    void poll();

signals:
    void logReady(QString);
};

#endif // WBLOGGER_H
