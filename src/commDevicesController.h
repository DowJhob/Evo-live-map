#ifndef COMMDEVICESCONTROLLER_H
#define COMMDEVICESCONTROLLER_H

#include "src/comm-device-interface/comm-device-interface.h"
#include "src/deviceNativeFilter.h"
#include "src/ecu/ecu.h"
#include <QObject>

class commDevicesController : public QObject
{
    Q_OBJECT
public:
    commDevicesController(ecu *ECU);

    void setSelectedECUcommDevice(device dev);
    void deviceHasLeft(comm_device_interface *_devComm);
    void ecustopped();

public slots:

private:
    comm_device_interface *selectedDevComm = nullptr;

signals:
    void createdDevComm(comm_device_interface *);
    void deviceHasLeft(); // Посылаем в эку и ждем пока он стопорнет лог


    void tactrixArrived(commDeviceWB *);
    void tactrixRemoved(commDeviceWB*);


};

#endif // COMMDEVICESCONTROLLER_H
