#include "ecu-model.h"

ECU_model::ECU_model()
{

}

void ECU_model::disconnect()
{

}

int ECU_model::writeFLASH(quint32 addr, int lenght)
{
    return 1;
}

QByteArray ECU_model::readFLASH(quint32 addr, int len)
{

    return QByteArray();
}

void ECU_model::setCommDev(comm_device_interface **devComm)
{
    qDebug() << "=========== ECUmodel::setCommDev ================" << devComm;

    this->devComm = devComm;
}

