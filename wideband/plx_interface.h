#ifndef PLX_INTERFACE_H
#define PLX_INTERFACE_H

#include <QObject>
#include <QDebug>
#include <wideband/wideband_interface.h>

enum ParserState {
    EXPECTING_START,
    EXPECTING_FIRST_HALF_OF_SENSOR_TYPE,
    EXPECTING_SECOND_HALF_OF_SENSOR_TYPE,
    EXPECTING_INSTANCE,
    EXPECTING_FIRST_HALF_OF_VALUE,
    EXPECTING_SECOND_HALF_OF_VALUE,
};

class plx_interface:public wideband_interface
{
    Q_OBJECT
public:
    bool flag = true;
    plx_interface()
    {
        //this->polling_interval = polling_interval;
    }
    ~plx_interface()
    {
        emit AFR("----");
    }
    void pushByte(uchar b)
    {
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

            emit AFR( QString::number((partialValue << 6) | b));
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

    void _dump(uchar* data, uint DataSize)
    {
    }

public slots:

private:

    //    static final Logger LOGGER = getLogger(PlxParserImpl.class);
    ParserState state = EXPECTING_START;
    //        PlxSensorType sensorType;
    int partialValue;
    uchar instance;

    QString result;

};

























#endif // PLX_INTERFACE_H
