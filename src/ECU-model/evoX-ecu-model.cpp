#include "evoX-ecu-model.h"

evoX_ECUmodel::evoX_ECUmodel(QObject* parent)
{
    availProtos.append(DMA_ProtoType::tephraX);
    name = "EVO X ECU model";
    type = ecuModelType::EVO_X_ECU_Model;
    ptr_calID = 0x5002a;
    calIDsize = 4;
}

evoX_ECUmodel::~evoX_ECUmodel()
{

}
