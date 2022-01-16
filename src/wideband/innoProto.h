#ifndef INNOPROTO_H
#define INNOPROTO_H


#include <QDebug>

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

class innoProto
{
public:
    float handleWB(QByteArray a);

private:
    //uchar* data;
    //ulong *DataSize;
    float result;
    float func_check(int func, uint _afr, uint _lambda);

};

#endif // INNOPROTO_H
