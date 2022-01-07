#ifndef _WIDEBAND_INPUT_DEVICE_H
#define _WIDEBAND_INPUT_DEVICE_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QElapsedTimer>
enum wb_dev_type {
    undef,
    TACTRIX,
    SERIAL_INTERFACE
};
enum wb_iface_type {
    inno,
    plx
};
typedef struct
{
    unsigned length		: 7;
    unsigned mark4		: 1;

    unsigned length_msb : 1;
    unsigned mark3		: 1;
    unsigned reserved1	: 1;
    unsigned log_caps	: 1;
    unsigned is_data	: 1;
    unsigned mark2		: 1;
    unsigned is_rec		: 1;
    unsigned mark1		: 1;
} inno_v2_mts_hdr;
typedef struct
{
    unsigned lambda		: 7;
    unsigned mark5		: 1;

    unsigned lambda_hi  : 6;
    unsigned mark4		: 2;

    unsigned afr		: 7;
    unsigned mark3		: 1;

    unsigned afr_msb	: 1;
    unsigned mark2		: 1;
    unsigned func		: 3;
    unsigned mark1		: 3;
} inno_v2_lc1_pkt;
typedef struct
{
    unsigned aux_lsbs	: 7;
    unsigned mark2		: 1;

    unsigned aux_msbs	: 6;
    unsigned mark1		: 2;
} inno_v2_aux_pkt;
typedef struct
{
    unsigned afr		: 7;
    unsigned mark4		: 1;

    unsigned afr_msb	: 1;
    unsigned mark3		: 1;
    unsigned func		: 3;
    unsigned mark2		: 1;
    unsigned is_rec		: 1;
    unsigned mark1		: 1;
} inno_v1_mts_hdr;
typedef struct
{
    unsigned bv_lo		: 7;
    unsigned mark4		: 1;

    unsigned bv_hi		: 3;
    unsigned mb			: 3;
    unsigned mark3		: 2;

    unsigned lambda		: 7;
    unsigned mark2		: 1;

    unsigned lambda_hi  : 6;
    unsigned mark1		: 2;
} inno_v1_lm1_pkt;
typedef struct
{
    unsigned aux_lsbs	: 7;
    unsigned mark2		: 1;

    unsigned aux_msbs	: 3;
    unsigned mark1		: 5;
} inno_v1_aux_pkt;
enum ParserState {
    EXPECTING_START,
    EXPECTING_FIRST_HALF_OF_SENSOR_TYPE,
    EXPECTING_SECOND_HALF_OF_SENSOR_TYPE,
    EXPECTING_INSTANCE,
    EXPECTING_FIRST_HALF_OF_VALUE,
    EXPECTING_SECOND_HALF_OF_VALUE,
};

class wideband_input_device: public QObject
{
    Q_OBJECT
public:
    wb_iface_type _wb_iface_type;
    uchar* data;
    ulong *DataSize;

    void (wideband_input_device::*dump)();
    wideband_input_device(){}

    void common_destructor()
    {
        wb_polling_timer->stop();
        wb_polling_timer->deleteLater();
    }
    void set_type(wb_iface_type _wb_iface_type)
    {
        this->_wb_iface_type = _wb_iface_type;
        switch (_wb_iface_type)
        {
        case inno: dump = &wideband_input_device::_dump_inno; break;
        case plx: dump = &wideband_input_device::_dump_plx; break;
        }
    }

public slots:
    void _start()
    {
        _connect();
        wb_polling_timer = new QTimer(this);
        wb_polling_timer->setInterval(20);
        connect(wb_polling_timer, &QTimer::timeout, this, [=](){_read();});
        wb_polling_timer->start();

    }
    void _stop()
    {
        wb_polling_timer->stop();
        deleteLater();
    }

private slots:
    virtual void _connect() = 0;
    virtual bool _read() = 0;

    void _dump_inno()
    {

        //qDebug() << "-- dump --";
        inno_v1_mts_hdr hdrv1;
        inno_v2_mts_hdr hdrv2;

        *((int*)&hdrv1) = data[0]*256 + data[1];
        *((int*)&hdrv2) = data[0]*256 + data[1];
        unsigned int payload;
        unsigned char* msgptr;
        unsigned int auxcnt = 1;

        bool validV1Hdr = hdrv1.mark1 && !hdrv1.mark2 && !hdrv1.mark3 && !hdrv1.mark4;
        bool validV2Hdr = hdrv2.mark1 && hdrv2.mark2 && hdrv2.mark3 && hdrv2.mark4;
        bool is_lm1_packet = false;

        if (validV1Hdr || validV2Hdr)
        {
            if (validV2Hdr)
            {
                //qDebug() << "-- V2 header --";
                payload = 2 * (hdrv2.length_msb * 128 + hdrv2.length);
            }
            else
            {
                qDebug() << "-- LM-1 headerless --";
                payload = 14; // LM-1 V1 payload is a fixed size
            }
            if (payload + 2 != *DataSize)
                return;
            msgptr = data + 2;
            // work our way through the payload bytes
            while (payload)
            {
                if (validV2Hdr && !is_lm1_packet)
                {
                    if ((*msgptr & 0xE2) == 0x42)// LC-1 packet
                    {
                        //qDebug() << "-- LC-1 packet --";
                        inno_v2_lc1_pkt pkt;
                        *((int*)&pkt) = (msgptr[0]<<24) + (msgptr[1]<<16) + (msgptr[2]<<8) + msgptr[3];
                        msgptr += 4;
                        payload -= 4;
                        func_check(pkt.func, pkt.afr_msb * 128 + pkt.afr, pkt.lambda_hi * 128 + pkt.lambda);
                    }
                    else if ((*msgptr & 0xA2) == 0x80) // LM-1 packet (within v2 header)
                    {
                        // LM-1 packet (within v2 header)
                        *((int*)&hdrv1) = msgptr[0]*256 + msgptr[1];
                        is_lm1_packet = true;
                        msgptr += 2;
                        payload -= 2;
                        func_check(hdrv1.func, hdrv1.afr_msb * 128 + hdrv1.afr, 1);
                    }
                    else  // must be AUX packet
                    {
                        // must be AUX packet
                        inno_v2_aux_pkt pkt;
                        *((int*)&pkt) = (msgptr[0]<<8) + msgptr[1];
                        msgptr += 2;
                        payload -= 2;
                        qDebug() << "aux" << auxcnt++ << ": " << (pkt.aux_msbs << 7) + pkt.aux_lsbs;
                    }
                }
                else  // -- LM-1 packet --
                {
                    //qDebug() << "-- LM-1 packet --";
                    // LM-1 packet
                    inno_v1_lm1_pkt pkt;
                    *((int*)&pkt) = (msgptr[0]<<24) + (msgptr[1]<<16) + (msgptr[2]<<8) + msgptr[3];
                    msgptr += 4;
                    payload -= 4;

                    func_check(hdrv1.func, hdrv1.afr_msb * 128 + hdrv1.afr, pkt.lambda_hi * 128 + pkt.lambda);

                    // get 5 AUX packets
                    for (int i = 0; i < 5; i++)
                    {
                        inno_v1_aux_pkt pkt;
                        *((int*)&pkt) = (msgptr[0]<<8) + msgptr[1];
                        msgptr += 2;
                        payload -= 2;
                        printf("aux%d: %d\n",auxcnt++,(pkt.aux_msbs << 7) + pkt.aux_lsbs);
                    }
                    is_lm1_packet = false;
                }
            }
        }
    }
    void _dump_plx()
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

            emit AFR( (partialValue << 6) | b);
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
    QTimer *wb_polling_timer;

    float result;
    void func_check(int func, uint _afr, uint _lambda)
    {
        double lambda,afr;
        //result.clear();
        switch (func) {
        case 0b000: {
            lambda = 0.5 + 0.001 * _lambda;
            afr = lambda * 0.1 * _afr;
            result = afr;} break;
        case 0b001: {
            double pct_o2;
            pct_o2 = 0.1 * _lambda;
            result = pct_o2;
            //         qDebug() << "O2: " << pct_o2;
        } break;
        case 0b010: result =func; break;
        case 0b011: result = func; break;
        case 0b100: result = ((_lambda/10) <<3) + func; break;
        case 0b101: result = func; break;
        case 0b110: result = (_lambda << 3) + func; break;

        }
        emit AFR(result);
        //        010 Free air calibration in progress, Lambda data not valid
        //        011 Need Free air Calibration Request, Lambda data not valid
        //        100 Warming up, Lambda value is temp in 1/10% of operating temp.
        //        101 Heater Calibration, Lambda value contains calibration countdown.
        //        110 Error code in Lambda value

    }

    ParserState state = EXPECTING_START;
    int partialValue;
    uchar instance;

signals:
    void AFR(float);
};

#endif // _WIDEBAND_INPUT_DEVICE_H
