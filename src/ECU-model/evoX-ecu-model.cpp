#include "evoX-ecu-model.h"

evoX_ECUmodel::evoX_ECUmodel()
{

}

evoX_ECUmodel::~evoX_ECUmodel()
{

}

QList<DMA_ProtoType> evoX_ECUmodel::getAvailProto()
{
    return QList<DMA_ProtoType>{DMA_ProtoType::tephraX};
}
