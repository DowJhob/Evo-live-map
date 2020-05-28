#ifndef INNO_INTERFACE_H
#define INNO_INTERFACE_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QElapsedTimer>

class inno_interface:public QObject
{
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

    Q_OBJECT
public:
    bool flag = true;
    inno_interface()
    {
        //this->polling_interval = polling_interval;
    }
    ~inno_interface()
    {
        emit AFR("----");
    }


    virtual void _connect() = 0;

    void dump_inno(uchar* data, uint DataSize)
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
            if (payload + 2 != DataSize)
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

public slots:
    void start()
    {
        _connect();
        _timer = new QTimer(this);
        _timer->setInterval(200);
        connect(_timer, &QTimer::timeout, this, [=](){emit AFR("----");});

        while (true)
        {
            if (!flag)
                return;
            if ( inno_read() )
                _timer->start();
            QCoreApplication::processEvents() ;
            QThread::msleep(10);
        }
        //emit AFR("----");
    }

    void stop()
    {
        flag = false;
        _timer->deleteLater();
    }
private slots:

    virtual bool inno_read() = 0;

private:
    QElapsedTimer t;
    QTimer* _timer;
    QString result;

    void func_check(int func, uint _afr, uint _lambda)
    {
        double lambda,afr;
        result.clear();
        switch (func) {
        case 0b000: {
            lambda = 0.5 + 0.001 * _lambda;
            afr = lambda * 0.1 * _afr;
            result = QString::number(afr, '.', 1);} break;
        case 0b001: {
            double pct_o2;
            pct_o2 = 0.1 * _lambda;
            result = QString::number(pct_o2);
            //         qDebug() << "O2: " << pct_o2;
        } break;
        case 0b010: result =" CAl"; break;
        case 0b011: result = "SCAl"; break;
        case 0b100: result = "H" + QString::number(_lambda/10); break;
        case 0b101: result = "hCAl"; break;
        case 0b110: result = "E" + QString::number(_lambda, 16); break;

        }
        emit AFR(result);
        //        010 Free air calibration in progress, Lambda data not valid
        //        011 Need Free air Calibration Request, Lambda data not valid
        //        100 Warming up, Lambda value is temp in 1/10% of operating temp.
        //        101 Heater Calibration, Lambda value contains calibration countdown.
        //        110 Error code in Lambda value

    }

signals:
    void AFR(QString);
};

#endif // INNO_INTERFACE_H
