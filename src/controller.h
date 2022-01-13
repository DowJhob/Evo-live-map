#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QApplication>
#include <QObject>

#include <QFileDialog>

#include "comm-device-interface/op13.h"
#include "comm-device-interface/op20.h"

#include "DMA-proto/jcsbanksDMA.h"
#include "DMA-proto/stockDMA.h"
#include "DMA-proto/evoX-DMA.h"

#include "ecu/ecu-definition.h"
#include "logger.h"
#include <src/abstract-memory.h>
//#include "read-request.h"
#include "deviceNativeFilter.h"

class controller : public QObject
{
    Q_OBJECT
public:
    explicit controller(QObject *parent = nullptr);
    ~controller();
    void start();

public slots:
    void setCommDevice(comm_device_interface *dev);
    void setProto(DMA_proto *ECUproto);
    void setLogRate(uint logRate);

    void getECUconnect();
    void getECUdisconnect();

    void startLogger();
    void stopLogger();

    void updateRAM(abstractMemoryScaled memory);
    void RAMreset();


private:
    QThread *this_thread = nullptr;
    ecu_definition *_ecu_definition = nullptr;
    comm_device_interface *devComm = nullptr;
    DMA_proto *ECUproto = nullptr;

    dataLogger *_dataLogger;

    char* p_in_buff;
    char* p_out_buff;

    mapDefinition *getMap(Map *declMap);

    QString SearchFiles(QString path, QString CalID);

private slots:
    void init();

signals:
    void baudChanged(int);
    void logChanged(int);
    //void interfaceReady(bool);

    void ecu_connected();
    void create_table(mapDefinition*);
    void getWB(commDeviceWB*);

    void Log(QString);
    void logReady(QVector<float>);

};

#endif // CONTROLLER_H
