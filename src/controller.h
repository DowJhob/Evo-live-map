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
#include "enumdev.h"

class controller : public QObject
{
    Q_OBJECT
public:
    bool connected = false;
    explicit controller(QObject *parent = nullptr);
    ~controller();
    void start();

public slots:
    void commDeviceSelected(device dev);
    void commDeviceRemoved(device dev);
    void setProto(int proto);

    void getECUconnect(uint baudRate);
    void getECUdisconnect();

    void startLogger();
    void stopLogger();

    void updateRAM(abstractMemoryScaled memory);
    void RAMreset();

    void setBaudRate(int baudRate);

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
    void interfaceReady(bool);

    void ecu_connected();
    void create_table(mapDefinition*);
    void getWB(commDeviceWB*);

    void Log(QString);
    void logReady(QVector<float>);

};

#endif // CONTROLLER_H
