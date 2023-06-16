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
#include "src/widgets/commParamWidget.h"
#include "src/widgets/gauge_widget.h"

class ecuManager : public QToolBar
{
    Q_OBJECT
public:
    ecu *ECU;
    commParamWidget cpW;

    explicit ecuManager(QWidget *parent = nullptr);
    ~ecuManager();

    void setUSBfilter(deviceNativeFilter *usbFilter);


public slots:

    void startLog();

    void stopLog();


private:

    QAction *a_start_action;
    QAction *a_ramReset;

    comm_device_interface *devComm = nullptr;
    DMA_proto *ECUproto = nullptr;

    char* p_in_buff;
    char* p_out_buff;


    gaugeWidget wbWgt{"           = Wideband =           ", 4};

    void setConectionParamWidget();


private slots:
    void lockConnect(bool lockFlag);
    void lockReset(bool lockFlag);
    void setCommDevice(comm_device_interface *dev);
    void setProto(DMA_proto *_ECUproto);
    void start_stop_Action();
    void interfaceLock();

    void deviceEvent(comm_device_interface *devComm);


signals:
    void deviceEventLog(QString, int);
    void ecuConnected(QHash<QString, Map*>*);
    void ecuDisconnected();
    void createMap(mapDefinition*);
    void addPatches(QHash<QString, bloblistPatch*> *);

    void updateRAM(offsetMemory memory);

    void logRateChanged(int);

    void Log(QString);
    void logReady(QVector<float>);

    void s_test(QHash<QString, Map*>*);

};

#endif // ECUMANAGER_H
