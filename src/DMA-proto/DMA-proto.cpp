#include "DMA-proto.h"

DMA_proto::DMA_proto(p_comm_device_interface *p_devComm):p_devComm(p_devComm)
{
       // qDebug() << "DMA_proto::DMA_proto(p_comm_device_interface *p_devComm)";
}

DMA_proto::~DMA_proto()
{}

void DMA_proto::startLog(ramMUT *_ramMut)
{
    qDebug()<<"=========== DMA_proto::startLog ================ _ramMut->size()" << _ramMut->size();
    ramMut = _ramMut;
    ramMut->scaledValue.resize(_ramMut->size());
}

bool DMA_proto::disconnect_()
{
    if(*p_devComm != nullptr)
        (*p_devComm)->close();
    return false;
}
