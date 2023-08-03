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
    void setConectionParamWidget();

    gaugeWidget wbWgt{"           = Wideband =           ", 4};

public slots:
    
    void ECUconnected(bool state);

private:
    QAction *a_start_action;
    QAction *a_ramReset;

private slots:
    void start_stop_Action();
    void deviceEvent(comm_device_interface *devComm);

signals:
    void ecuConnect(bool);

    void deviceEventLog(QString, int);

    void Log(QString);

};

#endif // ECUMANAGER_H
