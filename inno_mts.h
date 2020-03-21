#ifndef INNO_MTS_H
#define INNO_MTS_H

#include <QObject>
#include "libs/J2534.h"
class inno_mts
{
public:
    inno_mts()
    {

    }

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

    void dump_msg(PASSTHRU_MSG* msg)
    {
        if (msg->RxStatus & START_OF_MESSAGE)
            return; // skip
        if (msg->DataSize < 2)
            return;
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
                printf("-- V2 header --\n");
                payload = 2 * (hdrv2.length_msb * 128 + hdrv2.length);
            }
            else
            {
                printf("-- LM-1 headerless --\n");
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
                        printf("-- LC-1 packet --\n");
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
                            printf("lambda: %lf  afr: %lf\n",lambda,afr);
                        }
                        else if (pkt.func == 1)
                        {
                            double pct_o2;
                            pct_o2 = 0.1 * (pkt.lambda_hi * 128 + pkt.lambda);
                            printf("O2: %lf%%%\n",pct_o2);
                        }
                        else
                            printf("func: %d\n",pkt.func);
                    }
                    else if ((*msgptr & 0xA2) == 0x80)
                    {
                        // LM-1 packet (within v2 header)
                        *((int*)&hdrv1) = msgptr[0]*256 + msgptr[1];
                        is_lm1_packet = true;
                        msgptr += 2;
                        payload -= 2;
                    }
                    else
                    {
                        // must be AUX packet
                        inno_v2_aux_pkt pkt;
                        *((int*)&pkt) = (msgptr[0]<<8) + msgptr[1];
                        msgptr += 2;
                        payload -= 2;
                        printf("aux%d: %d\n",auxcnt++,(pkt.aux_msbs << 7) + pkt.aux_lsbs);
                    }
                }
                else
                {
                    printf("-- LM-1 packet --\n");
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
                        printf("lambda: %lf  afr: %lf\n",lambda,afr);
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

};

#endif // INNO_MTS_H
