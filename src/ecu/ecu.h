#ifndef ECU_H
#define ECU_H

#include <QObject>
#include <QDebug>

#include "ecu-definition.h"
#include "../abstract-memory.h"
#include "../DMA-proto/DMA-proto.h"
#include "src/ECU-flash-model/ecu-model.h"
#include "src/ecu/mapDefinition.h"


class ecu : public QObject
{
    Q_OBJECT
public:
    ecuDefinition ecuDef;

    comm_device_interface *devComm = nullptr;
    ECU_model *ecu_model = nullptr;
    DMA_proto *DMAproto = nullptr;

    QThread *this_thread;

    ecu();
    ~ecu();

public slots:
    void setComDev(comm_device_interface *_devComm);
    void setECUmodel(ECU_model *_ECUmodel);
    void setDMAproto(DMA_proto *_ECUproto);

    bool connectDMA(bool state);

    void startLog();
    void stopLog();

    void updateRAM(offsetMemory memory);
    void RAMreset();

    mapDefinition *getMap(Map *declMap);

    void setLogRate(int freqRate);

    void test();

private slots:

private:

signals:
    void ecuConnected(bool);

    void removeDevice(comm_device_interface*);

    void Log(QString);

    void logReady(QVector<float>);

    void s_test();

};

#endif // ECU_H
