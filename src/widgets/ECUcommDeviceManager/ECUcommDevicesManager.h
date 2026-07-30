#ifndef ECUCOMMDEVICESMANAGER_H
#define ECUCOMMDEVICESMANAGER_H

#include <QGroupBox>


#include "../../deviceNativeFilter.h"
#include "../../comm-device-interface/comm-device-interface.h"
#include "../../comm-device-interface/j2534-comm.h"
#include "../../comm-device-interface/op20.h"
#include "../../comm-device-interface/ftdi-comm.h"
#include "../../comm-device-interface/serial-comm.h"

#include "../../wideband/WB_OP20.h"

namespace Ui {
class commDeviceManagerWidget;
}

class ECUcommDeviceManagerWidget : public QGroupBox
{
    Q_OBJECT

public:
    uint baudRate;
    ECUcommDeviceManagerWidget(QWidget *parent = nullptr);
    ~ECUcommDeviceManagerWidget();

public slots:
    void deviceEvent(device dev);

private:
    comm_device_interface *selected_devComm = nullptr;
    Ui::commDeviceManagerWidget *ui;

    void addDevice(device dev);
    void removeDevice(device dev);

private slots:
    void _deviceSelected(int index);
    void _baudRateChanged();

signals:
    void deviceSelected(device);

    void deviceHasLeft(comm_device_interface*);

    void tactrixArrived(commDeviceWB *);
    void tactrixRemoved(commDeviceWB*);

};

#endif // ECUCOMMDEVICESMANAGER_H
