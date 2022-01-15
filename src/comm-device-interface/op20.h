#ifndef OP20_H
#define OP20_H

#include "j2534-interface.h"
#include "src/wideband/commdevicewb-interface.h"

class OP20 : public j2534_interface, public commDeviceWB
{
    Q_OBJECT
public:
    OP20(QString dllName = nullptr, QString DeviceDesc = "", QString DeviceUniqueID = "");
    ~OP20();

    bool openWB();

    bool connectWB(uint baudRate);

    bool closeWB();

    bool readWB();

public slots:

private slots:

private:
    unsigned long chanID_INNO;

    Message rxmsg;
    Message msgMask, msgPattern;
    unsigned long msgId;
    unsigned long numRxMsg = 1;

signals:
};


#endif // OP20_H
