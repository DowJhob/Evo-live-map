#ifndef ECU_FLASH_MODEL_H
#define ECU_FLASH_MODEL_H

//

//#include "src/DMA-proto/DMA-proto.h"
#include "src/comm-device-interface/comm-device-interface.h"
#include <QObject>


class ECU_model : public QObject
{
    Q_OBJECT
public:
    quint32 ptr_calID;
    quint16 calIDsize;

    comm_device_interface **devComm = nullptr;
    //DMA_proto *ECUproto = nullptr;
    ECU_model();

    QString lastError;

public slots:
    virtual bool connect() = 0;
    virtual bool MUTconnect() = 0;
    /*virtual*/ void disconnect();

    /*virtual*/ int writeFLASH(quint32 addr, int lenght);
    /*virtual*/ QByteArray readFLASH(quint32 addr, int len);




    void setCommDev(comm_device_interface **devComm);

private slots:

private:

};

#endif // ECU_FLASH_MODEL_H
