#ifndef WBLOGGER_H
#define WBLOGGER_H

#include <QObject>

#include "commdevicewb-interface.h"
#include "wb-proto.h"

class wbLogger:public QObject, public wbProto
{
    Q_OBJECT
public:
    wbLogger(commDeviceWB **cdWB);

private:
    commDeviceWB **cdWB;

};

#endif // WBLOGGER_H
