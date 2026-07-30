#include "commDevicesController.h"
#include "src/comm-device-interface/ftdi-comm.h"
#include "src/comm-device-interface/op20.h"
#include "src/comm-device-interface/serial-comm.h"
#include "src/wideband/WB_OP20.h"

commDevicesController::commDevicesController(ecu *ECU)
{

    connect(this, &commDevicesController::createdDevComm, ECU, &ecu::setSelectedECUcommDevice );
}

void commDevicesController::setSelectedECUcommDevice(device dev)
{
    // qDebug()<< "deviceManager::addDevice start" << dev.DeviceDesc;


    if(selectedDevComm != nullptr)
    {
        // selectedDevComm->close();
        // selectedDevComm->disconnect();
        // selectedDevComm->deleteLater();
    }

    switch (dev.type)
    {
    case deviceType::SERIAL : selectedDevComm = new serial_comm(nullptr, dev.PortName); break;
    case deviceType::FTDI   : selectedDevComm = new FTDI_comm(nullptr, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID); break;
    case deviceType::J2534  : selectedDevComm = new j2534_comm(nullptr, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID); break;
    case deviceType::OP20   : selectedDevComm = new OP20(nullptr, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID);
        ((OP20*)selectedDevComm)->tactrixWBinstance = new op20wb(static_cast<OP20*>(selectedDevComm));
        // emit tactrixArrived(((OP20*)selectedDevComm)->tactrixWBinstance);
        break;

    default                : return;                                            //  но поскольку тут вылетим без добавления то вроде и не важно
    }

    selectedDevComm->setBaudRate(dev.baudRate);

    emit createdDevComm(selectedDevComm);
}

void commDevicesController::deviceHasLeft(comm_device_interface *_devComm)
{
    // selectedDMAproto->stopLog();

    delete _devComm;
}
