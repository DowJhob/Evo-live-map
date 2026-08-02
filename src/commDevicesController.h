#ifndef COMMDEVICESCONTROLLER_H
#define COMMDEVICESCONTROLLER_H

#include "src/comm-device-interface/comm-device-interface.h"
// #include "src/deviceNativeFilter.h"
#include "src/filterHelper.h"
#include "src/ecu/ecu.h"
#include <QObject>

class commDevicesController : public QObject
{
    Q_OBJECT
public:
    commDevicesController(ecu *ECU);


public slots:
    void setSelectedECUcommDevice(device dev);
    void ECU_DeviceHasLeft();
    void ecustopped();
    void ECU_SetBaudRate(uint baudRate);

private:
    comm_device_interface *selectedECU_DevComm = nullptr;

signals:
    void sigCreatedECU_DevComm(comm_device_interface *);
    void sigECU_DeviceHasLeft(); // Посылаем в эку и ждем пока он стопорнет лог


    void tactrixArrived(commDeviceWB *);
    void tactrixRemoved(commDeviceWB*);


};

#endif // COMMDEVICESCONTROLLER_H
