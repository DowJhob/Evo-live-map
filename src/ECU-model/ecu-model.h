#ifndef ECU_MODEL_H
#define ECU_MODEL_H

#include <QObject>
#include "qdebug.h"
#include "qendian.h"
#include "src/DMA-proto/DMA-proto.h"
#include "src/ecu/ecu-definition.h"

enum class ecuModelType
{
    EVO7_9_ECU_Model,
    EVO_X_ECU_Model
};

Q_DECLARE_METATYPE(ecuModelType);

class ECU_model : public QObject
{
    // Q_OBJECT
public:
    ecuModelType type;
    QString name;

    QMap<DMA_ProtoType, QString> availProtos;
    quint32 ptr_calID;
    quint16 calIDsize;

    explicit ECU_model(QObject* parent = nullptr/*, QMap<DMA_ProtoType, DMA_proto*> *availProtos = nullptr*/);
    // virtual ~ECU_model() = 0;

    // QList<DMA_ProtoType>* getAvailProtos();



    // DMA_proto *DMAproto = nullptr;

    ecuDefinition ecuDef;

    bool getECU_def(QByteArray romIDbytes)
    {
        // = DMAproto->directDMAread( ecu_model->ptr_calID, ecu_model->calIDsize);                        //читаем номер калибровки
        if ( !romIDbytes.isEmpty() )
        {
            QString romID = QString::number( qFromBigEndian<quint32>(romIDbytes.data()), 16 );
            qDebug() << "=========== ecu::connectDMA ================ romID:" << romID;
            qDebug() << "romID: " + romID;
            if (ecuDef.fromROMID(romID))
            {
                return true;
            }
            else
            {
                qDebug() << "XML NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!";
            }
        }
        else
            qDebug() << "failure get ECU rom id";
    }


private:

};

Q_DECLARE_METATYPE(ECU_model*);
#endif // ECU_MODEL_H
