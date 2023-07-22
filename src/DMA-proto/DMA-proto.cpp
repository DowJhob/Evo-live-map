#include "DMA-proto.h"

DMA_proto::DMA_proto()
{
//    qDebug() << "DMA_proto";
}

DMA_proto::~DMA_proto()
{
//    qDebug() << "~ECU_interface";
}

//void DMA_proto::updateRAM(offsetMemory memory)
//{
//    qDebug()<< "DMA_proto::directDMAwrite" << memory.toHex(':');z
//    //        QMetaObject::invokeMethod(this, "directDMAwrite", Qt::QueuedConnection,
//    //                                  Q_ARG(quint32, memory.addr),
//    //                                  Q_ARG(char *, memory.data()),
//    //                                  Q_ARG(int, memory.size()));
//    directDMAwrite(memory.addr, memory.data(), memory.size());
//}

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
