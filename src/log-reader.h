#ifndef LOGREADER_H
#define LOGREADER_H

#include <QThread>
#include <QObject>
#include <QCoreApplication>


#include "comm-device-interface/comm-device-interface.h"
#include "ecu-proto/ECU-interface.h"

class j2534listener : public QThread
{
    Q_OBJECT
public:
    explicit j2534listener(QObject *parent = nullptr, comm_device_interface **devComm = nullptr, ECU_interface **ECUproto = nullptr);

private:
    comm_device_interface **devComm = nullptr;
    ECU_interface **ECUproto = nullptr;

    QElapsedTimer tt;

protected:
    void run();

private slots:
    void read();

signals:
    void logReady(abstractMemoryScaled);
    //void loop();
};

#endif // LOGREADER_H
