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
    qDebug()<<"=========== DMA_proto::startLog ================" << QThread::currentThread();
    ramMut->scaledValue.resize(_ramMut->size());
    ramMut = _ramMut;
}

void DMA_proto::setCommDev(comm_device_interface **devComm)
{
    qDebug() << "=========== DMA_proto::setCommDev ================" << devComm;

    this->devComm = devComm;
}
