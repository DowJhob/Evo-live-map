#ifndef WBPROTO_H
#define WBPROTO_H

#include <QByteArray>
//#include "innoProto.h"
//#include "plxProto.h"

class wbProto //: public plxProto, public innoProto
{
public:
    int baudRate = 0;
    virtual float handleWB(QByteArray a) = 0;

private:

};

#endif // WBPROTO_H
