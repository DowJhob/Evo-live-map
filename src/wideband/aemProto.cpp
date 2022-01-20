#include "aemProto.h"

aemProto::aemProto()
{
    baudRate = 9200;
}

QString aemProto::handleWB(QByteArray a)
{
    a.chop(2);
    emit logReady(a);
    //emit logReady("test\n\n");
    return a;
}
