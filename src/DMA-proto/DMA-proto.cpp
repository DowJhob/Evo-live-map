#include "DMA-proto.h"

DMA_proto::DMA_proto()
{
//    qDebug() << "DMA_proto";
}

DMA_proto::~DMA_proto()
{
//    qDebug() << "~ECU_interface";
}

void DMA_proto::directDMAwrite(abstractMemoryScaled memory)
{
    qDebug()<< "DMA_proto::directDMAwrite" << memory.toHex(':');
    //        QMetaObject::invokeMethod(this, "directDMAwrite", Qt::QueuedConnection,
    //                                  Q_ARG(quint32, memory.addr),
    //                                  Q_ARG(char *, memory.data()),
    //                                  Q_ARG(int, memory.size()));
    directDMAwrite(memory.addr, memory.data(), memory.size());
}

void DMA_proto::setCommDev(comm_device_interface **devComm)
{
    qDebug() << "=========== DMA_proto::setCommDev ================" << devComm;

    this->devComm = devComm;
}
