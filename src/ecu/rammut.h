#ifndef RAMMUT_H
#define RAMMUT_H

#include "mutparam.h"

class ramMUT : public QVector<mutParam>
{
public:
    quint32 DEAD_var;
    quint32 addr;
    quint16 byteSize;
    //QVector<mutParam> RAM_MUT;
    ////QHash<QString, mutParam> RAM_MUTh;

    QVector<float> scaledValue;
    ////int readSize = 0;

    ramMUT();

    //ramMUT& operator [](int i);

};

#endif // RAMMUT_H
