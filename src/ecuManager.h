#ifndef ECUMANAGER_H
#define ECUMANAGER_H

#include <QApplication>
#include <QObject>
#include <QToolBar>

#include <QFileDialog>

//#include "comm-device-interface/op13.h"
//#include "comm-device-interface/op20.h"

#include "DMA-proto/jcsbanksDMA.h"
#include "DMA-proto/stockDMA.h"
#include "DMA-proto/evoX-DMA.h"

#include "ecu/ecu-definition.h"
#include "logger.h"
#include <src/abstract-memory.h>
//#include "read-request.h"
#include "deviceNativeFilter.h"

class ecuManager : public QToolBar
{
    Q_OBJECT
public:
    explicit ecuManager(QWidget *parent = nullptr);
    ~ecuManager();

public slots:
    void setCommDevice(comm_device_interface *dev);
    void setProto(DMA_proto *ECUproto);
    void setLogRate(uint logRate);

    void connectECU();
    void disConnectECU();

//    void startLogger();
//    void stopLogger();

    void updateRAM(abstractMemoryScaled memory);
    void RAMreset();

    void lockConnect(bool lockFlag);
    void lockReset(bool lockFlag);

private:
    void createUI();
    QAction *a_start_action;
    QAction *a_ramReset;

    //QVector<float> scaledRAM_MUTvalue;
    //int readSize = 0;

    ecu_definition *_ecu_definition = nullptr;
    comm_device_interface *devComm = nullptr;
    DMA_proto *ECUproto = nullptr;

    dataLogger *_dataLogger;

    char* p_in_buff;
    char* p_out_buff;

    mapDefinition *getMap(Map *declMap);

private slots:
    void startAction();

signals:
    void ecu_connected();
    void disConnectECUaction();
    void create_table(mapDefinition*);


    void Log(QString);
    void logReady(QVector<float>);


};

#endif // ECUMANAGER_H
