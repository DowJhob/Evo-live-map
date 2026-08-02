#ifndef ECUCOMMDEVICESMANAGER_H
#define ECUCOMMDEVICESMANAGER_H

#include <QGroupBox>

#include "src/filterHelper.h"
#include "../../comm-device-interface/comm-device-interface.h"
// #include "../../comm-device-interface/j2534-comm.h"
// #include "../../comm-device-interface/op20.h"
// #include "../../comm-device-interface/ftdi-comm.h"
// #include "../../comm-device-interface/serial-comm.h"

#include "../../wideband/WB_OP20.h"
#include "ui_ECUcommDevicesManager.h"

// Q_DECLARE_METATYPE( device )

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

    void insertECU_Device(device dev);
    void removeECU_Device(device dev);

public slots:
    void deviceEvent(device dev)
    {
        switch (dev.direction)
        {
        case dir::arrive :
            // devComm->setBaudRate(ui->el_baudRate->text().toUInt());

            ui->availCommDev->addItem(dev.PortName /*+ " / " + dev.DeviceUniqueID*/, QVariant::fromValue<device>(dev));

            break;
        case dir::remove : removeECU_Device(dev); break;
        }
    }

private:
    // comm_device_interface *selected_devComm = nullptr;
    Ui::commDeviceManagerWidget *ui;

private slots:
    void _deviceSelected(int index);
    void _baudRateChanged();

signals:
    void ECU_deviceHasLeft();
    void deviceSelected(device);
    void ECU_SetBaudRate(quint32);

    void tactrixArrived(commDeviceWB *);
    void tactrixRemoved(commDeviceWB*);

};

#endif // ECUCOMMDEVICESMANAGER_H
