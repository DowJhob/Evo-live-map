#ifndef EVO7_ECU_MODEL_H
#define EVO7_ECU_MODEL_H


#include <QObject>
#include "src/ECU-flash-model/ecu-model.h"


class evo7_ECUmodel : public ECU_model
{
    Q_OBJECT
public:
    evo7_ECUmodel();

public slots:
    bool connect();
    bool MUTconnect();
    //void disconnect();


    //int writeFLASH(quint32 addr, int lenght);
    //QByteArray readFLASH(quint32 addr, int len);

private:

};

#endif // EVO7_ECU_MODEL_H
