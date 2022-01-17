#ifndef ABSTRACT_MEMORY_H
#define ABSTRACT_MEMORY_H

#include <QFile>
#include <QtEndian>
#include "map-decl/scaling.h"

class abstractMemory: public QByteArray
{
public:
    quint64 addr;

    abstractMemory()
    {}

    abstractMemory(QByteArray a):QByteArray(a)
    {}

private:

};

class abstractMemoryScaled: public abstractMemory
{
public:
    abstractMemoryScaled();

    abstractMemoryScaled(QByteArray a);

    abstractMemoryScaled(Scaling *scaling, float value2);

    abstractMemoryScaled& operator  = (const QByteArray &mem);

    QByteArray fromFloat(Scaling *scaling, float value2);

    float toFloat(Scaling *scaling);

    float toFloat(Scaling *scaling, int index);

    float toFloatOffset(Scaling *scaling, int index);

    QVector<float> fromMemoryA(Scaling *scaling, int count);

private:

};

#endif // ABSTRACT-MEMORY_H
