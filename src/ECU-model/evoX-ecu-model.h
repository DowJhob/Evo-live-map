#ifndef EVOX_ECU_MODEL_H
#define EVOX_ECU_MODEL_H

#include "src/ECU-model/ecu-model.h"

class evoX_ECUmodel : public ECU_model
{
public:
    evoX_ECUmodel();
    ~evoX_ECUmodel();

    QList<DMA_ProtoType> getAvailProto();

private:

};

#endif // EVOX_ECU_MODEL_H
