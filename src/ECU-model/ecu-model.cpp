#include "ecu-model.h"

ECU_model::ECU_model(QObject* parent/*, QMap<DMA_ProtoType, DMA_proto *> *availProtos*/)/* : availProtos(availProtos)*/
{

}

QList<DMA_ProtoType> *ECU_model::getAvailProtos()
{
    return &availProtos;
}

// ECU_model::~ECU_model()
// {

// }
