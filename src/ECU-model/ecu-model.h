#ifndef ECU_MODEL_H
#define ECU_MODEL_H

#include <QObject>

enum class ecuModelType
{
    EVO7_9_ECU_Model,
    EVO_X_ECU_Model
};

enum class DMA_ProtoType
{
    jcsbanks,
    nanner55,
    tephraX
};

Q_DECLARE_METATYPE(ecuModelType);

Q_DECLARE_METATYPE(DMA_ProtoType);


class ECU_model : public QObject
{
 // Q_OBJECT
public:
    quint32 ptr_calID;
    quint16 calIDsize;

    ECU_model();
    // virtual ~ECU_model() = 0;

    virtual QList<DMA_ProtoType> getAvailProto() = 0;

private:

};

Q_DECLARE_METATYPE(ECU_model*);
#endif // ECU_MODEL_H
