#ifndef WB_H
#define WB_H

#include "qdebug.h"
#include "qthread.h"
#include "qtimer.h"
#include "src/wideband/commdevicewb-interface.h"
#include "src/wideband/wb-proto.h"
#include <QObject>

class WB : public QObject
{
    Q_OBJECT
public:
    QThread *thread;
    WB();

public slots:
    void setWBDev(commDeviceWB *_wbdevComm);

    void setWBproto(wbProto *_protoWB);

    void start(bool state);

    void startLog();

    void stopLog();

//    void init();

//    void poll();

private:
//    QTimer *pollTimer = nullptr;
    commDeviceWB *wbdevComm;
    wbProto *protoWB;
    bool stopFlag = false;

    void handleWB(QByteArray a);

signals:
    void _poll();
    void removeDevice(commDeviceWB*);

    void lambdaValue(QString);

};

#endif // WB_H
