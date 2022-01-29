#ifndef ABSTRACT_MEMORY_H
#define ABSTRACT_MEMORY_H

#include <QFile>
#include <QtEndian>
#include "map-decl/scaling.h"

class abstractMemory: public QByteArray
{
public:

    abstractMemory()
    {}

    abstractMemory(QByteArray a):QByteArray(a)
    {}

private:

};

///
/// \brief The offsetMemory class stored raw memory from ecu with offset adrress
///
///
class offsetMemory: public //abstractMemory
        QByteArray
{
public:
    quint64 addr;

    offsetMemory();

    offsetMemory(QByteArray a);

    offsetMemory(Scaling *scaling, float value2);

    //offsetMemory& operator = (const QByteArray &mem);

    QByteArray fromFloat(Scaling *scaling, float value2);

    float toFloat(Scaling *scaling);

    float toFloat(Scaling *scaling, int index);

    float toFloatOffset(Scaling *scaling, int index);

    QVector<float> fromMemoryA(Scaling *scaling, int count);

private:

};

#endif // ABSTRACT-MEMORY_H
