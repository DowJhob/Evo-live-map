#include "wb-proto.h"

float wbProto::handleWB(int proto, QByteArray a)
{
    switch (proto)
    {
    case 1: return innoProto::handleWB(a); break;
    case 2:break;
    case 3: return plxProto::handleWB(a); break;
    default:break;
    }
}
