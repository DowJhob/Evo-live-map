#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QGroupBox>


#include "../../deviceNativeFilter.h"
#include "../../comm-device-interface/comm-device-interface.h"
#include "../../comm-device-interface/j2534-comm.h"
#include "../../comm-device-interface/op20.h"
#include "../../comm-device-interface/ftdi-comm.h"
#include "../../comm-device-interface/serial-comm.h"

#include "../../wideband/op20wb.h"

namespace Ui {
class commDeviceManagerWidget;
}

class commDeviceManagerWidget : public QGroupBox
{
    Q_OBJECT

public:
    uint baudRate;
    commDeviceManagerWidget(QWidget *parent = nullptr);
    ~commDeviceManagerWidget();

public slots:
    void deviceEvent(device dev);
    void _removeDevice(comm_device_interface*devComm);

private:
    Ui::commDeviceManagerWidget *ui;

    void addDevice(device dev);
    void removeDevice(device dev);

private slots:
    void _deviceSelected(int index);
    void _baudRateChanged();

signals:
    void deviceSelected(comm_device_interface*);

    void tactrixArrived(commDeviceWB *);
    void tactrixRemoved(comm_device_interface*);

};

#endif // DEVICEMANAGER_H
