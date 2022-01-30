#ifndef POLLHELPER_H
#define POLLHELPER_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <QTimer>

#include "DMA-proto.h"

class pollHelper : public DMA_proto
{
    Q_OBJECT
public:
    explicit pollHelper();

    QTimer *pollTimer = nullptr;

    virtual bool connect() = 0;

    virtual QByteArray indirectDMAread(quint32 addr, int lenght) = 0;
    virtual QByteArray directDMAread(quint32 addr, int len) = 0;

public slots:
    virtual void directDMAwrite(quint32 addr, char *buf, int lenght) = 0;

public slots:
    virtual void startLog(ramMUT *ramMut);
    virtual void stopLog();

    virtual void poll() = 0;

private:
    void init();

private slots:


};

#endif // POLLHELPER_H
