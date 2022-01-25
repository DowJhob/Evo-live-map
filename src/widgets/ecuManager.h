#ifndef ECUMANAGER_H
#define ECUMANAGER_H

#include <QApplication>
#include <QObject>
#include <QToolBar>

#include <QFileDialog>

//#include "comm-device-interface/op13.h"
//#include "comm-device-interface/op20.h"

#include "../DMA-proto/jcsbanksDMA.h"
#include "../DMA-proto/stockDMA.h"
#include "../DMA-proto/evoX-DMA.h"

#include "../ecu/ecu-definition.h"

#include "src/abstract-memory.h"
//#include "read-request.h"
#include "../deviceNativeFilter.h"

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

    void ecuConnected();


    void lockConnect(bool lockFlag);
    void lockReset(bool lockFlag);

private:
    ecuDefinition *ECUdef;

    QAction *a_start_action;
    QAction *a_ramReset;

    comm_device_interface *devComm = nullptr;
    DMA_proto *ECUproto = nullptr;

    char* p_in_buff;
    char* p_out_buff;


private slots:
    void startAction();

signals:
    void ecuConnected_();
    void ecuDisconnect();
    void create_table(mapDefinition*);

    void updateRAM(abstractMemoryScaled memory);

    void Log(QString);
    void logReady(QVector<float>);


};

#endif // ECUMANAGER_H
