#ifndef OP20_H
#define OP20_H

#include "j2534-interface.h"
#include "src/wideband/commdevicewb-interface.h"

class OP20 : public commDeviceWB, public j2534_interface
{
    //Q_OBJECT
public:
    OP20(QString dllName = nullptr, QString DeviceDesc = "", QString DeviceUniqueID = "");
    ~OP20();

    bool isClosed();

    bool openWB(uint baudRate);

    bool connectWB(uint baudRate);

    bool closeWB();

    QByteArray readWB();

//public slots:

//private slots:

private:
    QTimer *pollTimer;
    unsigned long chanID_INNO = 0;

    Message rxmsg;
    Message msgMask, msgPattern;
    unsigned long msgId;
    unsigned long numRxMsg = 1;

//signals:

};


#endif // OP20_H
