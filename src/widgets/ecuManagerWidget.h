#ifndef ECUMANAGERWIDGET_H
#define ECUMANAGERWIDGET_H

#include <QApplication>
#include <QObject>
#include <QToolBar>

#include <QFileDialog>

#include "../ecu/ecu.h"

// #include "../deviceNativeFilter.h"
#include "src/commDevicesController.h"
#include "src/mainwindow.h"
#include "src/widgets/commDevicesWidget.h"
#include "src/widgets/gauge_widget.h"

// enum class command
// {
//     commandECU_deviceHasLeft,
//     commandECU_DeviceSelected,
//     commandECU_ModelSelected,
//     commandECU_ProtoSelected,

//     // // from logRateManagerWidget
//     commandlogRateChanged,


//     // // from wbManagerWidget
//     commandwbSelected,
//     commandWB_ProtoSelected,
//     commandwbStart,
//     commandlogReady,
//     // void WB_DeviceSelected(device);
//     // void WB_ProtoSelected(DMA_ProtoType);

//     commandecuConnect,

//     commanddeviceEventLog,

//     commandLog
// };

class ecuManagerWidget : public QToolBar
{
    Q_OBJECT
public:
    ecu *ECU;

    explicit ecuManagerWidget(MainWindow *parent = nullptr, ecu *ECU = nullptr, commDevicesController *commDevCtrl = nullptr);
    ~ecuManagerWidget();

public slots:
    void fillECU_Models(QMap<ecuModelType, QString> *availECUmodels);
    void fillAvailECU_Protos(QMap<DMA_ProtoType, QString>*availProtos);
    
    void ECUconnected(bool state);

    void addTactrix(commDeviceWB *cdWB)
    {
        commDevsMngrWgt.addTactrix(cdWB);
    }
    void removeTactrix(commDeviceWB *cdWB)
    {
        commDevsMngrWgt.removeTactrix(cdWB);
    }

private:
    QAction *a_start_action;
    QAction *a_ramReset;
    commDevicesWidget commDevsMngrWgt;

private slots:
    void start_stop_Action();
    void deviceSelected(device devComm);

signals:
    // void command(command, QObject);

    // from ECU comm Devices Widget
    // void deviceSelected(device);
    void ECU_deviceHasLeft(comm_device_interface*);
    void ECU_DeviceSelected(device);
    void ECU_SetBaudRate(qint32);
    void ECU_ModelSelected(ecuModelType);
    void ECU_ProtoSelected(DMA_ProtoType);

    // // from logRateManagerWidget
    void logRateChanged(uint);


    // // from wbManagerWidget
    void wbSelected(commDeviceWB*);
    void WB_ProtoSelected(wbProto*);
    void wbStart(bool);
    void logReady(QString);
    // void WB_DeviceSelected(device);
    // void WB_ProtoSelected(DMA_ProtoType);

    void ecuConnect(bool);

    void deviceEventLog(QString, int);

    void Log(QString);

};

#endif // ECUMANAGERWIDGET_H


// // from ecuModelManagerWidget
// void modelSelected(ecuModelType);
// void ECU_ProtoSelected(DMA_ProtoType);

