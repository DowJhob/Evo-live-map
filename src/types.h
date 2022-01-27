#ifndef TYPES_H
#define TYPES_H

#include <QtGlobal>

enum class dir
{
    arrive,
    remove
};

enum class Protocol : uint {
    //// J2534-1
    J1850VPW        = 1,
    J1850PWM,
    ISO9141,
    ISO14230,
    CAN,
    ISO15765,
    SCIAEngine,     // OP2.0: Not supported
    SCIATrans,      // OP2.0: Not supported
    SCIBEngine,     // OP2.0: Not supported
    SCIBTrans,     // OP2.0: Not supported
    //// J2534-2
     CAN_CH1        =   0x00009000,
     J1850VPW_CH1   =   0x00009080,
     J1850PWM_CH1   =   0x00009160,
     ISO9141_CH1    =   0x00009240,
     ISO9141_CH2    =   0x00009241,
     ISO9141_CH3    =   0x00009242,
     ISO9141_K      =   ISO9141_CH1,
     ISO9141_L      =   ISO9141_CH2,	// OP2.0: Support for ISO9141 communications over the L line
     ISO9141_INNO   =   ISO9141_CH3,		// OP2.0: Support for RS-232 receive-only communications via the 2.5mm jack
     ISO14230_CH1   =   0x00009320,
     ISO14230_CH2   =   0x00009321,
     ISO14230_K     =   ISO14230_CH1,
     ISO14230_L     =   ISO14230_CH2,	// OP2.0: Support for ISO14230 communications over the L line
     ISO15765_CH1   =   0x00009400
};

enum class ConnectFlag {
    CAN29BitID = 1 << 8,
    ISO9141NoChecksum = 1 << 9,
    CANIDBoth = 1 << 11,
    ISO9141KLineOnly = 1 << 12
};

enum class eLogMsgType : quint8
{
    LOG_24x2B = 0x81,
    LOG_24x1B,
    LOG_5x2B,
    LOG_24x2B_LT,
    LOG_2x2B_6x1B_LT,
    LOG_5x2B_LT,
    LOG_COPY,
    LOG_INV,
    LOG_LT
};

enum class deviceType{
    undef = 0,
    OP13 = 13,
    OP20 = 20,
    J2534 = 2534
};

#endif // TYPES_H
