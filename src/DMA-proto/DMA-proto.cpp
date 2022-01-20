#include "DMA-proto.h"

DMA_proto::DMA_proto()
{
//    qDebug() << "DMA_proto";
}

DMA_proto::~DMA_proto()
{
//    qDebug() << "~ECU_interface";
}

bool DMA_proto::getECU(QString romID)
{
    if (!_ecu_definition.fromFile(_ecu_definition.SearchFiles(QApplication::applicationDirPath() + "/xml/", romID)))
    {
        //delete _ecu_definition;
        qDebug() << "XML NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!";
        return false;
    }
    return true;
}

void DMA_proto::setCommDev(comm_device_interface **devComm)
{
    qDebug() << "=========== DMA_proto::setCommDev ================" << devComm;

    this->devComm = devComm;
}
