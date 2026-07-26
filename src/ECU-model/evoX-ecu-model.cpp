#include "evoX-ecu-model.h"

evoX_ECUmodel::evoX_ECUmodel(QObject* parent)
{
    availProtos.insert(DMA_ProtoType::tephraX, "cap for tephra x");

    name = "EVO X ECU model";
    type = ecuModelType::EVO_X_ECU_Model;
    ptr_calID = 0x5002a;
    calIDsize = 4;
}

evoX_ECUmodel::~evoX_ECUmodel()
{

}
