#ifndef OP20_H
#define OP20_H

#include <QTimer>

#include "j2534-comm.h"
//#include "src/wideband/commdevicewb-interface.h"

class OP20 : public j2534_comm//, public commDeviceWB
{
    //Q_OBJECT
public:
    OP20(QObject *parent, QString dllName = nullptr, QString DeviceDesc = "", QString DeviceUniqueID = "");
    ~OP20();

//    bool isClosed();

//    bool openWB(uint baudRate);

//    bool connectWB(uint baudRate);

//    bool closeWB();

//    QByteArray readWB();

//public slots:

//private slots:

    unsigned long chanID_INNO = 0;


private:

//signals:

};


#endif // OP20_H
