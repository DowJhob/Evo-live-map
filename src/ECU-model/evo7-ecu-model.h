#ifndef EVO7_ECU_MODEL_H
#define EVO7_ECU_MODEL_H

#include "src/ECU-model/ecu-model.h"


class evo7_ECUmodel : public ECU_model
{
    Q_OBJECT
public:
    evo7_ECUmodel();
    ~evo7_ECUmodel();

public slots:
    QList<DMA_ProtoType> getAvailProto();

private:

};

#endif // EVO7_ECU_MODEL_H
