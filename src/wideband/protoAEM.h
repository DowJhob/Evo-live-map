#ifndef PROTOAEM_H
#define PROTOAEM_H

#include <QDebug>

#include "protoWB.h"

class aemProto : public wbProto
{
public:
    uchar* data;
    ulong *DataSize;

    aemProto();
    QString handleWB(QByteArray a);

private:


};

#endif // PROTOAEM_H
