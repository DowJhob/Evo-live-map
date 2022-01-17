#ifndef PLXPROTO_H
#define PLXPROTO_H

#include <QDebug>

#include "wb-proto.h"

enum ParserState {
    EXPECTING_START,
    EXPECTING_FIRST_HALF_OF_SENSOR_TYPE,
    EXPECTING_SECOND_HALF_OF_SENSOR_TYPE,
    EXPECTING_INSTANCE,
    EXPECTING_FIRST_HALF_OF_VALUE,
    EXPECTING_SECOND_HALF_OF_VALUE,
};

class plxProto : public wbProto
{
public:
    uchar* data;
    ulong *DataSize;

    plxProto();
    float handleWB(QByteArray a);

private:
    float result;

    ParserState state = EXPECTING_START;
    int partialValue;
    uchar instance;

};

#endif // PLXPROTO_H
