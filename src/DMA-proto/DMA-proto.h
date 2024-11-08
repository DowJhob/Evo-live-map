#ifndef DMAPROTO_H
#define DMAPROTO_H

#include <QApplication>
#include <QObject>
#include <QDebug>
#include <QElapsedTimer>

#include "../ecu/rammut.h"
#include "../abstract-memory.h"
#include "../comm-device-interface/comm-device-interface.h"
#include "src/ecu/ecu-definition.h"

enum class DMA_ProtoType
{
    jcsbanks,
    nanner55,
    tephraX
};

Q_DECLARE_METATYPE(DMA_ProtoType);

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
    DMA_ProtoType type;
    QString name;
    p_comm_device_interface *p_devComm = nullptr;
    // ECU_model **ecu_model = nullptr;

    ecuDefinition* ecuDef = nullptr;

    ramMUT *ramMut;

    explicit DMA_proto(p_comm_device_interface *p_devComm);
    virtual ~DMA_proto();;

    virtual bool connect_() = 0;
    virtual bool disconnect_();

    virtual QByteArray indirectDMAread(quint32 addr, int lenght) = 0;
    virtual QByteArray directDMAread(quint32 addr, int len) = 0;
    virtual void setLogRate(int freqRate) = 0;
    virtual void RAMreset(quint32 var1, quint16 var2) = 0;

    virtual void updateRAM(offsetMemory memory) = 0;

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

typedef DMA_proto* p_DMA_proto;

// Q_DECLARE_METATYPE( DMA_proto* )
#endif // DMAPROTO_H
