#include "evo7-ecu-model.h"

evo7_ECUmodel::evo7_ECUmodel()
{
    ptr_calID = 0x0F52;
    calIDsize = 4;
}

evo7_ECUmodel::~evo7_ECUmodel()
{

}

QList<DMA_ProtoType> evo7_ECUmodel::getAvailProto()
{
    return QList<DMA_ProtoType>{DMA_ProtoType::jcsbanks, DMA_ProtoType::nanner55};
}
