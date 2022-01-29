#ifndef RAMMUT_H
#define RAMMUT_H

#include "mutparam.h"

class ramMUT : public QVector<mutParam>
{
public:
    quint32 DEAD_var;
    quint32 RAM_MUT_addr;
    quint16 RAM_MUT_size;
    QVector<mutParam> RAM_MUT;
    QHash<QString, mutParam> RAM_MUTh;

    ramMUT();

    //ramMUT& operator [](int i);

};

#endif // RAMMUT_H
