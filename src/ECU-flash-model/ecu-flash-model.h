#ifndef ECU_FLASH_MODEL_H
#define ECU_FLASH_MODEL_H


#include "src/DMA-proto/DMA-proto.h"
#include "src/comm-device-interface/comm-device-interface.h"
#include <QObject>


class ECU_FLASH_model : public QObject
{
    Q_OBJECT
public:
    comm_device_interface **devComm = nullptr;
    DMA_proto *ECUproto = nullptr;
    ECU_FLASH_model();

    QString lastError;

public slots:
    virtual int writeFLASH(quint32 addr, int lenght) = 0;
    virtual QByteArray readFLASH(quint32 addr, int len) = 0;



    virtual bool connect(quint32 romIDaddr, int length);
    virtual void disconnect() = 0;

    void setCommDev(comm_device_interface **devComm);


private:

};

#endif // ECU_FLASH_MODEL_H
