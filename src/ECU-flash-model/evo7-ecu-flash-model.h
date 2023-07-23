#ifndef EVO7_ECU_FLASH_MODEL_H
#define EVO7_ECU_FLASH_MODEL_H


#include <QObject>
#include "src/ECU-flash-model/ecu-flash-model.h"


class evo7_ECUmodel : public ECU_FLASH_model
{
    Q_OBJECT
public:
    evo7_ECUmodel();

public slots:
    bool connectFLASH();
    void disconnect() = 0;


    int writeFLASH(quint32 addr, int lenght);
    QByteArray readFLASH(quint32 addr, int len);

private:

};

#endif // EVO7_ECU_FLASH_MODEL_H
