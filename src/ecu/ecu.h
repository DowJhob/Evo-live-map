#ifndef ECU_H
#define ECU_H

#include <QObject>
#include <QDebug>

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
    void ecuConnected(QHash<QString, Map*>*);
    void ecuDisconnected();

    void createMap(mapDefinition*);

    void Log(QString);

    void logReady(QVector<float>);

    void s_test(QHash<QString, Map*>*);

};

#endif // ECU_H
