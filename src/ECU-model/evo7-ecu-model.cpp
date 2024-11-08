#include "evo7-ecu-model.h"

evo7_ECUmodel::evo7_ECUmodel(QObject *parent)
{
    availProtos.append(DMA_ProtoType::jcsbanks);
    availProtos.append(DMA_ProtoType::nanner55);

    name = "EVO7-9 ECU model";
    type = ecuModelType::EVO7_9_ECU_Model;
    ptr_calID = 0x0F52;
    calIDsize = 4;
}

evo7_ECUmodel::~evo7_ECUmodel()
{

}
