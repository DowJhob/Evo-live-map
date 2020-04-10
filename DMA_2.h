#ifndef DMA_2_H
#define DMA_2_H
#include <QObject>
#include <QDebug>
#include "op13.h"
#include "op20.h"

#include "QTimer"
#include <QStack>
#include <QTime>
#include <QThread>
#include <QtEndian>
#include <QQueue>

class dma_2:public QObject
{
    Q_OBJECT
public:
    dma_2()
    {

    }
    ~dma_2()
    {

    }


public slots:

    void read_indirect(quint32 addr, int count)
    {

    }
    void read_direct(quint32 addr, int count)
    {

    }
    void write_direct(quint32 addr, int count)
    {

    }

private:

signals:
    void timer_lock();
    void timer_unlock();
    void Log(QString);
};
#endif // DMA_H
