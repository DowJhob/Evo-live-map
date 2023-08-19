#ifndef ECU_H
#define ECU_H

#include <QObject>
#include <QDebug>

#include "ecu-definition.h"
#include "../abstract-memory.h"
#include "../DMA-proto/DMA-proto.h"
#include "src/ECU-model/ecu-model.h"
#include "src/ecu/mapDefinition.h"

class ecu : public QObject
{
    Q_OBJECT
public:
    ecuDefinition ecuDef;

    comm_device_interface *devComm = nullptr;
    ECU_model *ecu_model = nullptr;
    DMA_proto *DMAproto = nullptr;

    QThread *readThread;

    ecu();
    ~ecu();

public slots:
    void setComDev(comm_device_interface *_devComm);
    void setECUmodel(ECU_model *_ECUmodel);
    void setDMAproto(DMA_proto *_ECUproto);

    bool connectDMA(bool state);

    void startLog();
    void stopLog();

    mapDefinition *getMap(Map *declMap);

    void setLogRate(int freqRate);

    void test();

private slots:

private:

signals:
    void ecuConnected(bool);

    void removeDevice(comm_device_interface*);

    void updateRAM(offsetMemory);

    void RAMreset();

    void Log(QString);

    void logReady(QVector<float>);

    void s_test();

};

class writer : public QObject
{
    Q_OBJECT
public:
    writer(ecu *parent = nullptr);
    ~writer();
    QThread *writeThread;

public slots:
    void updateRAM(offsetMemory memory);
    void RAMreset();

private:
    ecu *parent = nullptr;

};

#endif // ECU_H
