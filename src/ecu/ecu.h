#ifndef ECU_H
#define ECU_H

#include <QObject>
#include <QDebug>

#include "ecu-definition.h"
#include "../abstract-memory.h"
#include "../DMA-proto/DMA-proto.h"
#include "src/DMA-proto/evoX-DMA.h"
#include "src/DMA-proto/jcsbanksDMA.h"
#include "src/DMA-proto/stockDMA.h"
#include "src/ECU-model/ecu-model.h"
#include "src/ECU-model/evo7-ecu-model.h"
#include "src/ECU-model/evoX-ecu-model.h"
#include "src/ecu/mapDefinition.h"

class ecu : public QObject
{
    Q_OBJECT
public:
    ecuDefinition ecuDef;

    QThread *readThread;
    DMA_proto *selectedDMAproto = nullptr;

    ecu();
    ~ecu();

    QMap<ecuModelType, ECU_model*>* getAvailModels();

    QMap<DMA_ProtoType, DMA_proto*>* getAvailProtos();


public slots:
    void setComDev(comm_device_interface *_devComm);
    void setECUmodel(ECU_model *_ECUmodel);
    void setDMAproto(DMA_proto *_ECUproto);

    void deviceHasLeft(comm_device_interface *_devComm);

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
    comm_device_interface *selectedDevComm = nullptr;
    ECU_model *selectedECUmodel = nullptr;

    QMap<ecuModelType, ECU_model*> AvailModels
        {
            {ecuModelType::EVO7_9_ECU_Model, new evo7_ECUmodel(this)},
            {ecuModelType::EVO_X_ECU_Model, new evoX_ECUmodel(this)}
        };

    QMap<DMA_ProtoType, DMA_proto*> availProtos
        {
            {DMA_ProtoType::jcsbanks, new jcsbanksDMA(&selectedDevComm)},
            {DMA_ProtoType::nanner55, new stockDMA(&selectedDevComm)},
            {DMA_ProtoType::tephraX, new evoX_DMA(&selectedDevComm)}
        };

signals:
    void ecuConnected(bool);

    void Log(QString);

    void logReady(QVector<float>);

    void s_test();

};

#endif // ECU_H
