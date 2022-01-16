#include "innoProto.h"

float innoProto::handleWB(QByteArray a)
{
//    if (msg->RxStatus & START_OF_MESSAGE)
//        return; // skip

//    printf("[%u] ",msg->Timestamp);
//    for (unsigned int i = 0; i < msg->DataSize; i++)
//        printf("%02X ",msg->Data[i]);
//    printf("\n");

    if (a.size() < 2)
        return 0;

    inno_v1_mts_hdr hdrv1;
    inno_v2_mts_hdr hdrv2;

    *((int*)&hdrv1) = a.data()[0]*256 + a.data()[1];
    *((int*)&hdrv2) = a.data()[0]*256 + a.data()[1];
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
        if (payload + 2 != a.size())
            return -1;
        msgptr = (uchar*)a.data() + 2;
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
                    return func_check(pkt.func, pkt.afr_msb * 128 + pkt.afr, pkt.lambda_hi * 128 + pkt.lambda);
                }
                else if ((*msgptr & 0xA2) == 0x80) // LM-1 packet (within v2 header)
                {
                    // LM-1 packet (within v2 header)
                    *((int*)&hdrv1) = msgptr[0]*256 + msgptr[1];
                    is_lm1_packet = true;
                    msgptr += 2;
                    payload -= 2;
                    return func_check(hdrv1.func, hdrv1.afr_msb * 128 + hdrv1.afr, 1);
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

                return func_check(hdrv1.func, hdrv1.afr_msb * 128 + hdrv1.afr, pkt.lambda_hi * 128 + pkt.lambda);

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
    return 0;
}

float innoProto::func_check(int func, uint _afr, uint _lambda)
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
    case 0b010: result = func; break;
    case 0b011: result = func; break;
    case 0b100: result = ((_lambda/10) <<3) + func; break;
    case 0b101: result = func; break;
    case 0b110: result = (_lambda << 3) + func; break;

    }
    return result;
    //      emit AFR(result);
    //        010 Free air calibration in progress, Lambda data not valid
    //        011 Need Free air Calibration Request, Lambda data not valid
    //        100 Warming up, Lambda value is temp in 1/10% of operating temp.
    //        101 Heater Calibration, Lambda value contains calibration countdown.
    //        110 Error code in Lambda value

}
