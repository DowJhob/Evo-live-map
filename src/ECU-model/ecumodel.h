#ifndef ECUMODEL_H
#define ECUMODEL_H


#include <QObject>


class ECUmodel
{
    Q_OBJECT
public:
    ECUmodel();

public slots:
    virtual bool connectECU() = 0;
    virtual void disConnectECU() = 0;

private:

};

#endif // ECUMODEL_H
