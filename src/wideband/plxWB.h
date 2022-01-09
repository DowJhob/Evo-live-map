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

    plxWB(commDeviceWB **cdWB):wbInterface(cdWB)
    {

    }
    void handleWB()
    {
        uchar b;
        if (b == (uchar) 0x80) {
            state = EXPECTING_FIRST_HALF_OF_SENSOR_TYPE;
            //          return null;
        }

        if (b == 0x40) {
            state = EXPECTING_START;
            //            return null;
        }

        switch (state) {
        case EXPECTING_START:
            break;
        case EXPECTING_FIRST_HALF_OF_SENSOR_TYPE:
            state = EXPECTING_SECOND_HALF_OF_SENSOR_TYPE;
            partialValue = b;
            break;

        case EXPECTING_SECOND_HALF_OF_SENSOR_TYPE:
            state = EXPECTING_INSTANCE;

            //emit AFR( (partialValue << 6) | b);
            //              sensorType = valueOf(value);
            //               if (PlxSensorType.UNKNOWN == sensorType)
            //          {
            //   emit Log( "PLX sensor address: %d, unknown sensor type");
            //               }
            break;

        case EXPECTING_INSTANCE:
            state = EXPECTING_FIRST_HALF_OF_VALUE;
            instance = b;
            break;

        case EXPECTING_FIRST_HALF_OF_VALUE:
            state = EXPECTING_SECOND_HALF_OF_VALUE;
            partialValue = b;
            break;

        case EXPECTING_SECOND_HALF_OF_VALUE:
            state = EXPECTING_FIRST_HALF_OF_SENSOR_TYPE;
            int rawValue = (partialValue << 6) | b;
            //emit Log( "PLX sensor: %s instance: %d, value: %d"
            //,
            //                   sensorType, instance, rawValue));
            //         return new PlxResponse(sensorType, instance, rawValue
            //        );
            break;
        }
        //        return null;
    }

private:

    float result;

    ParserState state = EXPECTING_START;
    int partialValue;
    uchar instance;

};

#endif // PLXWB_H
