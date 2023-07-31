#ifndef DMAPROTO_H
#define DMAPROTO_H

#include <QApplication>
#include <QObject>
#include <QDebug>
#include <QElapsedTimer>

#include "../ecu/rammut.h"
#include "../abstract-memory.h"
#include "../comm-device-interface/comm-device-interface.h"
#include "src/ECU-flash-model/ecu-flash-model.h"

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
    ECU_model **ecu_model = nullptr;
    ramMUT *ramMut;

    DMA_proto();
    //explicit DMA_proto(comm_device_interface **devComm = nullptr);
    virtual ~DMA_proto();

    void setCommDev(comm_device_interface **devComm = nullptr);

    virtual bool connect_() = 0;

    virtual QByteArray indirectDMAread(quint32 addr, int lenght) = 0;
    virtual QByteArray directDMAread(quint32 addr, int len) = 0;
    virtual void setLogRate(int freqRate) = 0;

public slots:
    virtual void directDMAwrite(quint32 addr, char *buf, int lenght) = 0;
//    void updateRAM(offsetMemory memory);

    virtual void startLog(ramMUT *_ramMut);
    virtual void startLog() = 0;
    virtual void stopLog() = 0;

private slots:

private:

signals:
    void logReady(QVector<float>);

};

#endif // DMAPROTO_H
