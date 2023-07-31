#ifndef POLLHELPER_H
#define POLLHELPER_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <QTimer>

#include "DMA-proto.h"

class pollHelper : public QObject
{
    Q_OBJECT
public:
    explicit pollHelper(DMA_proto *parent_proto);

//    virtual bool connect() = 0;

//    virtual QByteArray indirectDMAread(quint32 addr, int lenght) = 0;
//    virtual QByteArray directDMAread(quint32 addr, int len) = 0;

//    virtual void directDMAwrite(quint32 addr, char *buf, int lenght) = 0;

public slots:
    void setLogRate(int rate);
    virtual void startLog(int minPollTime);
    virtual void stopLog();

private slots:
    void poll();

private:
    DMA_proto *parent_proto = nullptr;
    QTimer *pollTimer = nullptr;
    void init();

private slots:


};

#endif // POLLHELPER_H
