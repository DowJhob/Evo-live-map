#include "wb-proto.h"

float wbProto::handleWB(int proto, QByteArray a)
{
    //qDebug() << "=========== wbProto::handleWB ================" << a.toHex(':');
    switch (proto)
    {
    case 0: return innoProto::handleWB(a); break;
    case 1: break;
    case 2: return plxProto::handleWB(a); break;
    default:break;
    }
}
