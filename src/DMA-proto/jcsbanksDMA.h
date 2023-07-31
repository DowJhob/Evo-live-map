#ifndef jcsbanksDMA_H
#define jcsbanksDMA_H

#include <QObject>
#include <QDebug>

#include "../comm-device-interface/j2534-comm.h"
//#include "DMA-proto.h"
#include "pollhelper.h"

class jcsbanksDMA : public DMA_proto
{
    Q_OBJECT
public:
    jcsbanksDMA();
    explicit jcsbanksDMA(comm_device_interface **devComm);
    ~jcsbanksDMA();

    //public slots:
    bool connect_();
    QByteArray indirectDMAread(quint32 addr, int lenght);
    QByteArray directDMAread(quint32 addr, int lenght);
    void directDMAwrite(quint32 addr, char *buf, int lenght);

public slots:
    void startLog(ramMUT *_ramMut);
    void stopLog();

    void setLogRate(int freqRate);

    void poll();


private slots:
//    void poll();

private:
    pollHelper *poller;
    int delay_after_command = 5;
    int mcuResponseTime = 5;
    int bitsInLineByte = 10;

    void sendDMAcomand(char command, unsigned long addr, unsigned long count, char* buf = nullptr);

signals:
    void _poll();

};

#endif // jcsbanksDMA_H
