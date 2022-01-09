#ifndef PLXWB_H
#define PLXWB_H

#include <QObject>
#include <QDebug>

#include <QElapsedTimer>

#include "wb-interface.h"

enum ParserState {
    EXPECTING_START,
    EXPECTING_FIRST_HALF_OF_SENSOR_TYPE,
    EXPECTING_SECOND_HALF_OF_SENSOR_TYPE,
    EXPECTING_INSTANCE,
    EXPECTING_FIRST_HALF_OF_VALUE,
    EXPECTING_SECOND_HALF_OF_VALUE,
};

class plxWB: public wbInterface
{
public:
    uchar* data;
    ulong *DataSize;

    plxWB(commDeviceWB **cdWB);
    void handleWB();

private:
    float result;

    ParserState state = EXPECTING_START;
    int partialValue;
    uchar instance;

};

#endif // PLXWB_H
