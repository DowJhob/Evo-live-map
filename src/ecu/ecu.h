#ifndef ECU_H
#define ECU_H

#include <QObject>
#include <QDebug>

#include "ecu-definition.h"
#include "../abstract-memory.h"
#include "../DMA-proto/DMA-proto.h"
// #include "src/DMA-proto/evoX-DMA.h"
#include "src/DMA-proto/jcsbanksDMA.h"
#include "src/DMA-proto/stockDMA.h"
#include "src/ECU-model/ecu-model.h"
#include "src/ECU-model/evo7-ecu-model.h"
#include "src/ECU-model/evoX-ecu-model.h"
#include "src/comm-device-interface/ftdi-comm.h"
#include "src/comm-device-interface/op20.h"
#include "src/comm-device-interface/serial-comm.h"
#include "src/deviceNativeFilter.h"
#include "src/ecu/mapDefinition.h"
#include "src/wideband/WB_OP20.h"

class ecu : public QObject
{
    Q_OBJECT
public:
    ecuDefinition ecuDef;

    QThread *readThread;
    DMA_proto *selectedDMAproto = nullptr;

    ecu();
    ~ecu();

    // QMap<ecuModelType, ECU_model*>* getAvailModels();

    QMap<ecuModelType, QString> *getAvailModels();


    // QMap<DMA_ProtoType, DMA_proto*>* getAvailProtos();


public slots:


    void setSelectedECUcommDevice(comm_device_interface *DevComm);
    void setECUmodel(ECU_model *_ECUmodel);

    void setECUmodelType(ecuModelType _ECUmodelType);
    void setDMAproto(DMA_ProtoType _DMAprotoType);

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
    ecuModelType selectedECUmodelType;
    DMA_ProtoType selectedDMAprotoType;

    // QMap<ecuModelType, ECU_model*> AvailModels
    //     {
    //         {ecuModelType::EVO7_9_ECU_Model, new evo7_ECUmodel(this)},
    //         {ecuModelType::EVO_X_ECU_Model, new evoX_ECUmodel(this)}
    //     };

    QMap<ecuModelType, QString> AvailModels2
        {
            {ecuModelType::EVO7_9_ECU_Model, "EVO7-9 ECU model"},
            {ecuModelType::EVO_X_ECU_Model, "EVO X ECU model"}
        };

    // QMap<DMA_ProtoType, DMA_proto*> availProtos
    //     {
    //         {DMA_ProtoType::jcsbanks, new jcsbanksDMA(&selectedDevComm)},
    //         {DMA_ProtoType::nanner55, new stockDMA(&selectedDevComm)},
    //         // {DMA_ProtoType::tephraX, new evoX_DMA(&selectedDevComm)}
    //         {DMA_ProtoType::tephraX, new stockDMA(&selectedDevComm)}
    //     };
    // QMap<DMA_ProtoType, QString> availProtos2
    //     {
    //         {DMA_ProtoType::jcsbanks, "Custom DMA proto by jcsbanks"},
    //         {DMA_ProtoType::nanner55, "stock DMA proto by nanner55"},
    //         {DMA_ProtoType::tephraX, "cap for x"}
    //     };

signals:

    void getAvailProtos(QMap<DMA_ProtoType, QString>*);

    void ecuConnected(bool);

    void Log(QString);

    void logReady(QVector<float>);

    void s_test();

};

#endif // ECU_H
