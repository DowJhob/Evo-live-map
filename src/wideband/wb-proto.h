#ifndef WBPROTO_H
#define WBPROTO_H

#include "innoProto.h"
#include "plxProto.h"

class wbProto : public plxProto, public innoProto
{
public:
    void handleWB();

private:
};

#endif // WBPROTO_H
