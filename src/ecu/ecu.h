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
// #include "src/deviceNativeFilter.h"
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

    QMap<ecuModelType, QString> *getAvailModels();

public slots:
    void setSelectedECUcommDevice(comm_device_interface *DevComm);

    void setECUmodelType(ecuModelType _ECUmodelType);
    void setDMAproto(DMA_ProtoType _DMAprotoType);

    void ECU_DeviceHasLeft();

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

    QMap<ecuModelType, QString> AvailModels
        {
            {ecuModelType::EVO7_9_ECU_Model, "EVO7-9 ECU model"},
            {ecuModelType::EVO_X_ECU_Model, "EVO X ECU model"}
        };

signals:

    void sigAvailECU_Protos(QMap<DMA_ProtoType, QString>*);
    void sigECU_deviceHasStopped();
    void ecuConnected(bool);

    void Log(QString);

    void logReady(QVector<float>);

    void s_test();

};

#endif // ECU_H
