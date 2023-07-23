#ifndef ECU_H
#define ECU_H

#include <QObject>
#include <QDebug>

#include "ecu-definition.h"
#include "../abstract-memory.h"
#include "../DMA-proto/DMA-proto.h"
#include "src/ECU-model/ecumodel.h"
#include "src/ecu/mapDefinition.h"


class ecu : public QObject
{
    Q_OBJECT
public:
    ecuDefinition ecuDef;

    comm_device_interface *devComm = nullptr;
    ECUmodel *_ECUmodel = nullptr;
    DMA_proto *DMAproto = nullptr;

    ecu();
    ~ecu();

public slots:
    void setComDev(comm_device_interface *_devComm);
    void setECUmodel(ECUmodel *_ECUmodel);
    void setDMAproto(DMA_proto *_ECUproto);

    void connectDMA(bool state);

    void startLog();
    void stopLog();

    void updateRAM(offsetMemory memory);
    void RAMreset();

    mapDefinition *getMap(Map *declMap);

    void setLogRate(int freqRate);

    void test();

private slots:

private:
    bool _connectDMA();
    void disconnectDMA();

signals:
    void ecuConnected(bool);
    void ecuDisconnected();

//    void createMap(mapDefinition*);

    void Log(QString);

    void logReady(QVector<float>);

    void s_test();

};

#endif // ECU_H
