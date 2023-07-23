#include "evo7-ecu-flash-model.h"
#include "qendian.h"
#include <QDebug>

evo7_ECUmodel::evo7_ECUmodel()
{

}

bool evo7_ECUmodel::connectFLASH()
{
    return ECU_FLASH_model::connect(0xF52, 4);
}

int evo7_ECUmodel::writeFLASH(quint32 addr, int lenght)
{

}

QByteArray evo7_ECUmodel::readFLASH(quint32 addr, int len)
{

}

