#ifndef DMAPROTO_H
#define DMAPROTO_H

#include <QApplication>
#include <QObject>
#include <QDebug>
//#include <QTimer>
#include <QThread>
#include <QtEndian>
#include <QElapsedTimer>

#include "../ecu/rammut.h"
#include "../abstract-memory.h"
#include "../comm-device-interface/comm-device-interface.h"

//#include "pollhelper.h"

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
ramMUT *ramMut;
    DMA_proto();
    //explicit DMA_proto(comm_device_interface **devComm = nullptr);
    virtual ~DMA_proto();

    void setCommDev(comm_device_interface **devComm = nullptr);

    virtual bool connect() = 0;

    virtual QByteArray indirectDMAread(quint32 addr, int lenght) = 0;
    virtual QByteArray directDMAread(quint32 addr, int len) = 0;

public slots:
    virtual void directDMAwrite(quint32 addr, char *buf, int lenght) = 0;
    void updateRAM(offsetMemory memory);

    virtual void startLog(ramMUT *_ramMut);
    virtual void stopLog() = 0;

    //virtual void poll() = 0;

private slots:

private:

    //Protocol protocol;
    //ConnectFlag ConnectFlag;
    //uint baudRate;


signals:
    void logReady(QVector<float>);

};

#endif // DMAPROTO_H
