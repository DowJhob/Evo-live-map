#ifndef ECU_H
#define ECU_H

#include <QObject>
#include <QDebug>

#include <QTimer>

#include "ecu-definition.h"
#include "../abstract-memory.h"
#include "../DMA-proto/DMA-proto.h"

typedef struct                                       // Содержимое таблицы
{
    Map *declMap;
    offsetMemory Map;
    offsetMemory X_axis;
    offsetMemory Y_axis;
} mapDefinition;

class ecu : public QObject
{
    Q_OBJECT
public:
    ecuDefinition ecuDef;

    comm_device_interface *devComm = nullptr;
    DMA_proto *ECUproto = nullptr;

    ecu();
    ~ecu();

public slots:
    void setComDev(comm_device_interface *_devComm);
    void setDMAproto(DMA_proto *_ECUproto);

    bool connectECU();
    void disConnectECU();

    void updateRAM(offsetMemory memory);
    void RAMreset();

    mapDefinition *getMap(Map *declMap);

    void startLog();
    //virtual void stopLog();

    void setLogRate(int freqRate);

private slots:
//    void poll();

private:
    QVector<float> scaledRAM_MUTvalue;
    int readSize = 0;
    //QTimer *pollTimer;

    QHash<QString, Scaling> scaling_qmap;                     //контейнер скалингов

signals:
    void ecuConnected();
    void ecuDisconnected();

    //void updateRAM(offsetMemory);

    void createMap(mapDefinition*);

    void Log(QString);

    void logReady(QVector<float>);

};

#endif // ECU_H
