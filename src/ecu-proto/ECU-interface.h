#ifndef ECU_INTERFACE_H
#define ECU_INTERFACE_H

#include <QObject>
#include <QDebug>
//#include <QTimer>
#include <QThread>
#include <QtEndian>
#include <QElapsedTimer>

#include <src/ecu/ecu-definition.h>

#include "../comm-device-interface/comm-device-interface.h"

enum class DMAcomand
{
    stockWrite,
    stockRead,
    stockInfo,
    directWrite = 0x04,
    directRead,
};

class ECU_interface : public QObject
{
    Q_OBJECT
public:
    comm_device_interface **devComm = nullptr;

    explicit ECU_interface(comm_device_interface **devComm = nullptr);
    virtual ~ECU_interface();

public slots:
    virtual bool connect() = 0;

    virtual QByteArray indirectDMAread(quint32 addr, int lenght) = 0;
    virtual QByteArray directDMAread(quint32 addr, int len) = 0;
    virtual void directDMAwrite(quint32 addr, char *buf, int lenght) = 0;

    void DMApoll();
    void updateRAM(abstractMemoryScaled memory);
    mapDefinition *getMap(Map *declMap);
    void RAMreset(quint32 addr);

private slots:

private:

signals:
    void readyInterface(bool);
    void readyRead(QByteArray);

    //void gettedMap(mapDefinition);

    void ECUready();
    void AFR(float);
    void Log(QString);
};

#endif // ECU_INTERFACE_H
