#ifndef jcsbanksDMA_H
#define jcsbanksDMA_H

#include <QObject>
#include <QDebug>

#include "../comm-device-interface/j2534-comm.h"
//#include "DMA-proto.h"
#include "pollhelper.h"

class jcsbanksDMA : public pollHelper
{
    Q_OBJECT
public:
    jcsbanksDMA();
    explicit jcsbanksDMA(comm_device_interface **devComm);
    ~jcsbanksDMA();

    //public slots:
    bool connect();
    QByteArray indirectDMAread(quint32 addr, int lenght);
    QByteArray directDMAread(quint32 addr, int lenght);
    void directDMAwrite(quint32 addr, char *buf, int lenght);

public slots:
    void startLog(ramMUT *_ramMut);
    void stopLog();

private slots:
    void poll();

private:
    int delay_after_command = 5;
    bool OP13_connect();
    bool j2534_connect();

    void sendDMAcomand(char command, unsigned long addr, unsigned long count, char* buf = nullptr);

};

#endif // jcsbanksDMA_H
