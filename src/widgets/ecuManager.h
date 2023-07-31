#ifndef ECUMANAGER_H
#define ECUMANAGER_H

#include <QApplication>
#include <QObject>
#include <QToolBar>

#include <QFileDialog>

#include "../ecu/ecu.h"

#include "../deviceNativeFilter.h"
#include "src/widgets/commParamWidget.h"
#include "src/widgets/gauge_widget.h"

class ecuManagerWidget : public QToolBar
{
    Q_OBJECT
public:
    ecu *ECU;
    commParamWidget cpW;

    explicit ecuManagerWidget(QWidget *parent = nullptr, ecu *ECU = nullptr);
    ~ecuManagerWidget();

    void setUSBfilter(deviceNativeFilter *usbFilter);


public slots:

    void interfaceLock(bool state);

private:
    QAction *a_start_action;
    QAction *a_ramReset;

    gaugeWidget wbWgt{"           = Wideband =           ", 4};

    void setConectionParamWidget();


private slots:
    void lockConnect(bool lockFlag);
    void lockReset(bool lockFlag);
//    void setCommDevice(comm_device_interface *dev);
//    void setProto(DMA_proto *_ECUproto);
    void start_stop_Action();

    void deviceEvent(comm_device_interface *devComm);


signals:
    void ecuConnect(bool);

    void deviceEventLog(QString, int);
    void createMap(mapDefinition*);

//    void updateRAM(offsetMemory memory);

    void logRateChanged(int);

    void Log(QString);
    void logReady(QVector<float>);

    void s_test(QHash<QString, Map*>*);

};

#endif // ECUMANAGER_H
