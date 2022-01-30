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

#include "../ecu/ecu.h"

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
    void setProto(DMA_proto *_ECUproto);

    void lockConnect(bool lockFlag);
    void lockReset(bool lockFlag);

private:
    ecu *ECU;

    QAction *a_start_action;
    QAction *a_ramReset;

    comm_device_interface *devComm = nullptr;
    DMA_proto *ECUproto = nullptr;

    char* p_in_buff;
    char* p_out_buff;

private slots:
    void startAction();
    void _ecuConnected();

signals:
    void ecuConnected();
    void ecuDisconnect();
    void createMap(mapDefinition*);

    void updateRAM(offsetMemory memory);

    void logRateChanged(int);

    void Log(QString);
    void logReady(QVector<float>);

};

#endif // ECUMANAGER_H
