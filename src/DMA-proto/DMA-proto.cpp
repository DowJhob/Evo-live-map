#include "DMA-proto.h"

//DMA_proto::DMA_proto()
//{
//    qDebug() << "ECU_interface";
//}

//DMA_proto::DMA_proto(comm_device_interface **devComm) : devComm(devComm)
//{
//    qDebug() << "ECU_interface";
//}

//DMA_proto::~DMA_proto()
//{
//    qDebug() << "~ECU_interface";
//}

void DMA_proto::setCommDev(comm_device_interface **devComm)
{
    this->devComm = devComm;
}
