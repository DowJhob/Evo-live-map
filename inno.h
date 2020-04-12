#ifndef INNO_H
#define INNO_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "libs/J2534.h"


class inno: public QObject
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
    inno(J2534 *j2534, unsigned long devID)
    {
        this->j2534 = j2534;
        this->devID = devID;
        connect(this, SIGNAL(_tick()), this, SLOT(inno_read()));
    }
    void start()
    {
        emit _tick();
    }
    void _connect()
    {
        // try to connect to the specific channel we would like to use
        //
        // in this case, it is the 2.5mm jack on the Openport 2.0 which can be used as
        // a RS-232 voltage level receive only input. the Innovate MTS bus
        // can be used this way, as it is 19200 baud, N,8,1 serial and transmits continuously
        // without any polling needed.
        //
        // note that the ISO9141_NO_CHECKSUM connection flag is used to avoid requiring the data
        // to have valid ISO9141 checksums (it doesn't)

        if (j2534->PassThruConnect(devID,ISO9141_INNO,ISO9141_NO_CHECKSUM,19200,&chanID_INNO))
        {
   //         reportJ2534Error();
   //         return 0;
        }

        // all J2534 channels need filters in order to receive anything at all
        //
        // in this case, we simply create a "pass all" filter so that we can see
        // everything unfiltered in the raw stream

        txmsg.ProtocolID = ISO9141_INNO;
        txmsg.RxStatus = 0;
        txmsg.TxFlags = 0;
        txmsg.Timestamp = 0;
        txmsg.DataSize = 1;
        txmsg.ExtraDataIndex = 0;
        msgMask = msgPattern  = txmsg;
        msgMask.Data[0] = 0; // mask the first byte to 0
        msgPattern.Data[0] = 0; // match it with 0 (i.e. pass everything)
        if (j2534->PassThruStartMsgFilter(chanID_INNO, PASS_FILTER,&msgMask,&msgPattern,NULL,&msgId))
        {
//            reportJ2534Error();
//            return 0;
        }
    }
public slots:
    void inno_read()
    {
        numRxMsg = -1;
        j2534->PassThruReadMsgs(chanID_INNO,&rxmsg,&numRxMsg,1000);
        if (numRxMsg)
            dump_msg(&rxmsg);
        QThread::msleep(10);
        emit _tick();
    }
private:
    J2534 *j2534;
    unsigned long devID;
    unsigned long chanID_INNO;
    PASSTHRU_MSG rxmsg,txmsg;
    PASSTHRU_MSG msgMask,msgPattern;
    unsigned long msgId;
    unsigned long numRxMsg;
    unsigned long protocol_inno = ISO9141_INNO;


    void dump_msg(PASSTHRU_MSG* msg)
    {
        if (msg->RxStatus & START_OF_MESSAGE)
            return; // skip

        if (msg->DataSize < 2)
            return;

        qDebug() << "-- dump --";
        inno_v1_mts_hdr hdrv1;
        inno_v2_mts_hdr hdrv2;

        *((int*)&hdrv1) = msg->Data[0]*256 + msg->Data[1];
        *((int*)&hdrv2) = msg->Data[0]*256 + msg->Data[1];
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
                qDebug() << "-- V2 header --";
                payload = 2 * (hdrv2.length_msb * 128 + hdrv2.length);
            }
            else
            {
                qDebug() << "-- LM-1 headerless --";
                payload = 14; // LM-1 V1 payload is a fixed size
            }
            if (payload + 2 != msg->DataSize)
                return;
            msgptr = msg->Data + 2;
            // work our way through the payload bytes
            while (payload)
            {
                if (validV2Hdr && !is_lm1_packet)
                {
                    if ((*msgptr & 0xE2) == 0x42)
                    {
                        qDebug() << "-- LC-1 packet --";
                        // LC-1 packet
                        inno_v2_lc1_pkt pkt;
                        *((int*)&pkt) = (msgptr[0]<<24) + (msgptr[1]<<16) + (msgptr[2]<<8) + msgptr[3];
                        msgptr += 4;
                        payload -= 4;

                        if (pkt.func == 0)
                        {
                            double lambda,afr;
                            lambda = 0.5 + 0.001 * (pkt.lambda_hi * 128 + pkt.lambda);
                            afr = lambda * 0.1 * (pkt.afr_msb * 128 + pkt.afr);
                            qDebug() << "lambda: " << lambda << "  afr: "<<  afr;
                        }
                        else if (pkt.func == 1)
                        {
                            double pct_o2;
                            pct_o2 = 0.1 * (pkt.lambda_hi * 128 + pkt.lambda);
                            qDebug() << "O2: " << pct_o2;
                        }
                        else
                            qDebug() << "func: " << pkt.func;
                    }
                    else if ((*msgptr & 0xA2) == 0x80) // LM-1 packet (within v2 header)
                    {
                        // LM-1 packet (within v2 header)
                        *((int*)&hdrv1) = msgptr[0]*256 + msgptr[1];
                        is_lm1_packet = true;
                        msgptr += 2;
                        payload -= 2;
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
                    qDebug() << "-- LM-1 packet --";
                    // LM-1 packet
                    inno_v1_lm1_pkt pkt;
                    *((int*)&pkt) = (msgptr[0]<<24) + (msgptr[1]<<16) + (msgptr[2]<<8) + msgptr[3];
                    msgptr += 4;
                    payload -= 4;

                    if (hdrv1.func == 0)
                    {
                        double lambda,afr;
                        lambda = 0.5 + 0.001 * (pkt.lambda_hi * 128 + pkt.lambda);
                        afr = lambda * 0.1 * (hdrv1.afr_msb * 128 + hdrv1.afr);
                        qDebug() << "lambda: " << lambda << "  afr: "<<  afr;
                    }
                    else if (hdrv1.func == 1)
                    {
                        double pct_o2;
                        pct_o2 = 0.1 * (pkt.lambda_hi * 128 + pkt.lambda);
                        printf("O2: %lf%%%\n",pct_o2);
                    }
                    else
                        printf("func: %d\n",hdrv1.func);

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

signals:
    void _tick();
    void AFRpayload(quint16, quint16);

};


#endif // INNO_H
