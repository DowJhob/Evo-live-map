#include "commDevicesController.h"
#include "src/comm-device-interface/ftdi-comm.h"
#include "src/comm-device-interface/op20.h"
#include "src/comm-device-interface/serial-comm.h"
#include "src/wideband/WB_OP20.h"

commDevicesController::commDevicesController(ecu *ECU)
{

    connect(this, &commDevicesController::sigCreatedECU_DevComm, ECU, &ecu::setSelectedECUcommDevice );
    // connect(ECU, &ecu::sigECU_deviceHasStopped, ECU, &ecu::setSelectedECUcommDevice );
}

void commDevicesController::setSelectedECUcommDevice(device dev)
{
    qDebug()<< Q_FUNC_INFO << dev.DeviceDesc;


    if(selectedECU_DevComm != nullptr)
    {
        // selectedDevComm->close();
        // selectedDevComm->disconnect();
        // selectedDevComm->deleteLater();
    }

    switch (dev.type)
    {
    case deviceType::SERIAL : selectedECU_DevComm = new serial_comm(nullptr, dev.PortName); break;
    case deviceType::FTDI   : selectedECU_DevComm = new FTDI_comm(nullptr, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID); break;
    case deviceType::J2534  : selectedECU_DevComm = new j2534_comm(nullptr, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID); break;
    case deviceType::OP20   : selectedECU_DevComm = new OP20(nullptr, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID);
        ((OP20*)selectedECU_DevComm)->tactrixWBinstance = new op20wb(static_cast<OP20*>(selectedECU_DevComm));
        emit tactrixArrived(((OP20*)selectedECU_DevComm)->tactrixWBinstance);
        break;

    default                : return;                                            //  но поскольку тут вылетим без добавления то вроде и не важно
    }

    selectedECU_DevComm->setBaudRate(dev.baudRate);

    emit sigCreatedECU_DevComm(selectedECU_DevComm);
}

void commDevicesController::ECU_DeviceHasLeft()
{
    selectedECU_DevComm->close();
    // selectedDMAproto->stopLog();

    delete selectedECU_DevComm;
}

void commDevicesController::ecustopped()
{

}

void commDevicesController::ECU_SetBaudRate(uint baudRate)
{    if(selectedECU_DevComm != nullptr)
    {
        selectedECU_DevComm->setBaudRate(baudRate);
    }
}
