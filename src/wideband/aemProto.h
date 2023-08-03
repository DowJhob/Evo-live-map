#ifndef AEMPROTO_H
#define AEMPROTO_H

#include <QDebug>

#include "wb-proto.h"

class aemProto : public wbProto
{
public:
    uchar* data;
    ulong *DataSize;

    aemProto();
    QString handleWB(QByteArray a);

private:


};

#endif // AEMPROTO_H
