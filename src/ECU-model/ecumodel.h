#ifndef ECUMODEL_H
#define ECUMODEL_H


#include "src/DMA-proto/DMA-proto.h"
#include "src/comm-device-interface/comm-device-interface.h"
#include <QObject>


class ECUmodel : public QObject
{
    Q_OBJECT
public:
    comm_device_interface **devComm = nullptr;
    DMA_proto *ECUproto = nullptr;
    ECUmodel();

    QString lastError;

public slots:
    virtual bool connectDMA(quint32 romIDaddr, int length);
    virtual void disConnectECU() = 0;

    void setCommDev(comm_device_interface **devComm);


private:

};

#endif // ECUMODEL_H
