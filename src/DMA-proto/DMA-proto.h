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

class DMA_proto : public QObject
{
    Q_OBJECT
public:
    comm_device_interface **devComm = nullptr;
    explicit DMA_proto(comm_device_interface **devComm = nullptr);
    virtual ~DMA_proto();

public slots:
    virtual bool connect(uint baudRate = 0) = 0;

    virtual QByteArray indirectDMAread(quint32 addr, int lenght) = 0;
    virtual QByteArray directDMAread(quint32 addr, int len) = 0;
    virtual void directDMAwrite(quint32 addr, char *buf, int lenght) = 0;

    void setBaudRate(uint baudRate);

private slots:

private:
    //Protocol protocol;
    //ConnectFlag ConnectFlag;
    //uint baudRate;

signals:
    void Log(QString);

};

#endif // ECU_INTERFACE_H
