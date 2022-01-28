#ifndef ECU_H
#define ECU_H

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QtEndian>
#include <QList>
#include <QStack>
#include <QChar>
#include <QMap>
#include <QtXml/QDomDocument>

//#include "../types.h"
#include "ecu-definition.h"
//#include "mutparam.h"
//#include "../map-decl/map.h"
#include "../abstract-memory.h"
#include "../DMA-proto/DMA-proto.h"

class ecu : public QObject
{
    Q_OBJECT
public:
    ecuDefinition ecuDef;

    comm_device_interface *devComm = nullptr;
    DMA_proto *ECUproto = nullptr;

    //quint32 DEAD_var;
    //quint32 RAM_MUT_addr;
    //quint16 RAM_MUT_size;
    //QVector<mutParam> RAM_MUT;
    //QHash<QString, mutParam> RAM_MUTh;

    QHash<QString, Map*> RAMtables;

    ecu();
    ~ecu();

public slots:
    void setComDev(comm_device_interface *_devComm);
    void setDMAproto(DMA_proto *_ECUproto);

    bool connectECU();

    void disConnectECU();

    void RAMreset();

    void updateRAM(abstractMemoryScaled memory);

    mapDefinition *getMap(Map *declMap);

    virtual void startLog();
    //virtual void stopLog();

private slots:
    void poll();

private:
    QVector<float> scaledRAM_MUTvalue;
    int readSize = 0;
    QTimer *pollTimer;

    QHash<QString, Scaling> scaling_qmap;                     //контейнер скалингов

signals:
    void ecuConnected();
    void ecuDisconnected();

    void createMap(mapDefinition*);

    void Log(QString);

    void logReady(QVector<float>);

};

#endif // ECU_H
