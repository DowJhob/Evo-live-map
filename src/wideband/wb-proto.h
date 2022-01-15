#ifndef WBPROTO_H
#define WBPROTO_H

#include "innoProto.h"
#include "plxProto.h"

class wbProto : public plxProto, public innoProto
{
public:
    float handleWB(int proto, QByteArray a);

private:
};

#endif // WBPROTO_H
