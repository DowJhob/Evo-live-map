#include "evoX-DMA.h"

evoX_DMA::evoX_DMA(p_comm_device_interface *p_devComm) : DMA_proto(p_devComm)
{
    name = "evoX DMA proto by tephra";
    type = DMA_ProtoType::tephraX;
    // qDebug() << "evoX_DMA::evoX_DMA(p_comm_device_interface *p_devComm)" << this->p_devComm;
}

evoX_DMA::~evoX_DMA()
{
    qDebug() << "~evoX_DMA";
}

QByteArray evoX_DMA::indirectDMAread(quint32 addr, int lenght)
{
return QByteArray();
}

QByteArray evoX_DMA::directDMAread(quint32 addr, int lenght)
{

}

void evoX_DMA::directDMAwrite(quint32 addr, char *buf, int lenght)
{

}

void evoX_DMA::startLog(ramMUT *ramMut)
{

}

void evoX_DMA::startLog()
{

}

void evoX_DMA::stopLog()
{

}

void evoX_DMA::setLogRate(int freqRate)
{

}

void evoX_DMA::RAMreset(quint32 var1, quint16 var2)
{

}

void evoX_DMA::updateRAM(offsetMemory memory)
{

}
