#ifndef EVO7_ECUMODEL_H
#define EVO7_ECUMODEL_H


#include <QObject>
#include "src/ECU-model/ecumodel.h"


class evo7_ECUmodel : public ECUmodel
{
    Q_OBJECT
public:
    evo7_ECUmodel();

public slots:
    bool connectECU();
    virtual void disConnectECU() = 0;

private:

};

#endif // EVO7_ECUMODEL_H
