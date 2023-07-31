#include "DMA-proto.h"

DMA_proto::DMA_proto()
{
//    qDebug() << "DMA_proto";
}

DMA_proto::~DMA_proto()
{
//    qDebug() << "~ECU_interface";
}

void DMA_proto::startLog(ramMUT *_ramMut)
{
    qDebug()<<"=========== DMA_proto::startLog ================ _ramMut->size()" << _ramMut->size();
    ramMut = _ramMut;
    ramMut->scaledValue.resize(_ramMut->size());
}

void DMA_proto::setCommDev(comm_device_interface **devComm)
{
    qDebug() << "=========== DMA_proto::setCommDev ================" << devComm;

    this->devComm = devComm;
}
