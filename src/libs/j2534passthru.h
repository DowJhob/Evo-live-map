/****************************************************************************
**
** Copyright (C) 2017 Ford Motor Company.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialBus module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef PASSTHRUCAN_J2534PASSTHRU_H
#define PASSTHRUCAN_J2534PASSTHRU_H

#include <QByteArray>
#include <QLibrary>
#include <QLoggingCategory>
#include <QObject>
#include <QString>

#include "../types.h"

#ifdef Q_OS_WIN32
# define J2534_API __stdcall
#else
# define J2534_API
#endif

Q_DECLARE_LOGGING_CATEGORY(QT_CANBUS_PLUGINS_PASSTHRU)

namespace J2534 {

class Message;

extern "C" {

typedef long (J2534_API *PassThruOpenFunc)(const void *pName, ulong *pDeviceId);
typedef long (J2534_API *PassThruCloseFunc)(ulong deviceId);
typedef long (J2534_API *PassThruConnectFunc)(ulong deviceId, ulong protocolId, ulong flags, ulong baudRate, ulong *pChannelId);
typedef long (J2534_API *PassThruDisconnectFunc)(ulong channelId);
typedef long (J2534_API *PassThruReadMsgsFunc)(ulong channelId, Message *pMsg, ulong *pNumMsgs, ulong timeout);
typedef long (J2534_API *PassThruWriteMsgsFunc)(ulong channelId, const Message *pMsg, ulong *pNumMsgs, ulong timeout);
typedef long (J2534_API *PassThruStartMsgFilterFunc)(ulong channelID, ulong filterType, const Message *pMaskMsg, const Message *pPatternMsg, const Message *pFlowControlMsg, ulong *pFilterId);
typedef long (J2534_API *PassThruStopMsgFilterFunc)(ulong channelID, ulong MsgID);


typedef long (J2534_API *PassThruGetLastErrorFunc)(char *pErrorDescription);
typedef long (J2534_API *PassThruIoctlFunc)(ulong channelId, ulong ioctlId, const void *pInput, void *pOutput);
} // extern "C"

#define QFLAGS_H

class Message
{
public:
    static const ulong maxSize = 4128;

    enum RxStatusBit {
        InTxMsgType = 1 << 0,
        InStartOfMessage = 1 << 1,
        InRxBreak = 1 << 2,
        InTxIndication = 1 << 3,
        InISO15765PaddingError = 1 << 4,
        InISO15765AddrType = 1 << 7,
        InCAN29BitID = 1 << 8
    };
    Q_DECLARE_FLAGS(RxStatus, RxStatusBit)

    enum TxFlag {
        OutISO15765FramePad = 1 << 6,
        OutISO15765AddrType = 1 << 7,
        OutCAN29BitID = 1 << 8,
        OutWaitP3MinOnly = 1 << 9
    };
    Q_DECLARE_FLAGS(TxFlags, TxFlag)

    Message();
    explicit Message(Protocol proto);

    Protocol protocolId() const { return Protocol(m_protocolId); }
    void setProtocolId(Protocol proto) { m_protocolId = uint(proto); }

    RxStatus rxStatus() const { return RxStatus(uint(m_rxStatus)); }
    void setRxStatus(RxStatus status) { m_rxStatus = uint(status); }

    TxFlags txFlags() const { return TxFlags(uint(m_txFlags)); }
    void setTxFlags(TxFlags flags) { m_txFlags = uint(flags); }

    ulong timestamp() const { return m_timestamp; }
    void setTimestamp(ulong stamp) { m_timestamp = stamp; }

    ulong size() const { return m_dataSize; }
    void setSize(ulong dataSize) { m_dataSize = dataSize; }

    ulong extraDataIndex() const { return m_extraDataIndex; }
    void setExtraDataIndex(ulong index) { m_extraDataIndex = index; }

    char *data() { return m_data; }
    const char *data() const { return m_data; }

//private:
    ulong m_protocolId = 0;
    ulong m_rxStatus = 0;
    ulong m_txFlags = 0;
    ulong m_timestamp = 0;
    ulong m_dataSize = 0;
    ulong m_extraDataIndex = 0;
    char  m_data[maxSize];
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Message::RxStatus)
Q_DECLARE_OPERATORS_FOR_FLAGS(Message::TxFlags)

class Config
{
public:
    enum Parameter {
        DataRate = 1,

        Loopback = 3,
        NodeAddress,
        NetworkLine,
        P1Min,
        P1Max,
        P2Min,
        P2Max,
        P3Min,
        P3Max,
        P4Min,
        P4Max,
        W1,
        W2,
        W3,
        W4,
        W5,
        Tidle,
        Tinil,
        Twup,
        Parity,
        BitSamplePoint,
        SyncJumpWidth,
        W0,
        T1Max,
        T2Max,
        T4Max,
        T5Max,
        ISO15765BS,
        ISO15765STmin,
        DataBits,
        FiveBaudMod,
        BSTx,
        STminTx,
        T3Max,
        ISO15765WFTMax,

        CanMixedFormat = 0x8000,
        J1962Pins,

        SWCANHSDataRate = 0x8010,
        SWCANSpeedchangeEnable,
        SWCANResSwitch,

        ActiveChannels = 0x8020,
        SampleRate,
        SamplesPerReading,
        ReadingsPerMsg,
        AveragingMethod,
        SampleResolution,
        InputRangeLow,
        InputRangeHigh
    };

    Config() : m_parameter(0), m_value(0) {}
    explicit Config(Parameter param, ulong val = 0) : m_parameter(param), m_value(val) {}

    Parameter parameter() const { return Parameter(m_parameter); }
    ulong value() const { return m_value; }

//private:
    ulong m_parameter;
    ulong m_value;
};

// Template to model the structs SCONFIG_LIST, SBYTE_ARRAY etc as defined
// in the J2534 spec.
template <typename T>
struct SArray
{
    SArray(ulong n, T *p) : num(n), ptr(p) {}
    // On Windows x64, ulong is 32 bit wide and thus the value would normally
    // be padded so that the pointer begins on a 64-bit boundary. It is not
    // clear from the J2534 spec whether structs should be packed or not on
    // x64. Most vendors still only provide a 32-bit DLL, but there is at
    // least one x64 implementation (from Hatteland Display) out there which
    // does not pack this struct.
    ulong num;
    T *ptr;
};

/**
 * @brief J2534 pass-through interface, version 04.04.
 * @internal
 * @see http://www.drewtech.com/support/passthru.html
 */
class PassThru : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(PassThru)
public:
    typedef ulong Handle;

    enum IoctlID
    {
        // J2534-1
        GET_CONFIG                              =   0x01,
        SET_CONFIG,
        READ_VBATT,
        FIVE_BAUD_INIT,
        FAST_INIT,
        CLEAR_TX_BUFFER                         =   0x07,
        CLEAR_RX_BUFFER,
        CLEAR_PERIODIC_MSGS	,
        CLEAR_MSG_FILTERS,
        CLEAR_FUNCT_MSG_LOOKUP_TABLE,                       // OP2.0: Not yet supported
        ADD_TO_FUNCT_MSG_LOOKUP_TABLE,                      // OP2.0: Not yet supported
        DELETE_FROM_FUNCT_MSG_LOOKUP_TABLE,                 // OP2.0: Not yet supported
        READ_PROG_VOLTAGE,                                  // OP2.0: Several pins are supported

        // J2534-2
        SW_CAN_NS                               =   0x8000, // OP2.0: Not supported
        SW_CAN_HS                               =	0x8001, // OP2.0: Not supported

        // Tactrix specific IOCTLs
        TX_IOCTL_BASE                           =	0x70000,
        // OP2.0: The IOCTL below supports application-specific functions
        // that can be built into the hardware
        TX_IOCTL_APP_SERVICE                    =	(TX_IOCTL_BASE+0),
        TX_IOCTL_SET_DLL_DEBUG_FLAGS            =	(TX_IOCTL_BASE+1),
        TX_IOCTL_DLL_DEBUG_FLAG_J2534_CALLS     =   0x00000001,
        TX_IOCTL_DLL_DEBUG_FLAG_ALL_DEV_COMMS	=   0x00000002,
        TX_IOCTL_SET_DEV_DEBUG_FLAGS			=   (TX_IOCTL_BASE+2),
        TX_IOCTL_DEV_DEBUG_FLAG_USB_COMMS		=   0x00000001,
        TX_IOCTL_SET_DLL_STATUS_CALLBACK		=   (TX_IOCTL_BASE+3),
        TX_IOCTL_GET_DEVICE_INSTANCES    		=   (TX_IOCTL_BASE+4)
    };

    enum Status {
        LoadFailed = -1,
        NoError = 0,
        NotSupported,
        InvalidChannelID,
        InvalidProtocolID,
        NullParameter,
        InvalidIoctlValue,
        InvalidFlags,
        Failed,
        DeviceNotConnected,
        Timeout,
        InvalidMsg,
        InvalidTimeInterval,
        ExceededLimit,
        InvalidMsgID,
        DeviceInUse,
        InvalidIoctlID,
        BufferEmpty,
        BufferFull,
        BufferOverflow,
        PinInvalid,
        ChannelInUse,
        MsgProtocolID,
        InvalidFilterID,
        NoFlowControl,
        NotUnique,
        InvalidBaudrate,
        InvalidDeviceID
    };

    Q_DECLARE_FLAGS(ConnectFlags, ConnectFlag)

    enum FilterType {
        PassFilter = 1,
        BlockFilter,
        FlowControlFilter
    };

    explicit PassThru(const QString &libraryPath, QObject *parent = nullptr);
    virtual ~PassThru();

    Status PassThruOpen(const QByteArray &name, Handle *deviceId);
    Status PassThruClose(Handle deviceId);
    Status PassThruConnect(Handle deviceId, Protocol protocolId, ConnectFlags flags, uint baudRate, Handle *channelId);
    Status PassThruDisconnect(Handle channelId);
    Status PassThruReadMsgs(Handle channelId, Message *msgs, ulong *numMsgs, uint timeout = 0);
    Status PassThruWriteMsgs(Handle channelId, const Message *msgs, ulong *numMsgs, uint timeout = 0);

    Status PassThruStartMsgFilter(Handle channelId, FilterType filterType, const Message *maskMsg, const Message *patternMsg, const Message *pFlowControlMsg, Handle *filterId);
    Status PassThruStopMsgFilter(Handle channelId, unsigned long MsgID);


// не забудь реализовать для полноты обертки
//
//    long PassThruSetProgrammingVoltage(unsigned long DeviceID, unsigned long Pin, unsigned long Voltage);
//    long PassThruReadVersion(char *pApiVersion,char *pDllVersion,char *pFirmwareVersion,unsigned long DeviceID);
//    long PassThruGetLastError(char *pErrorDescription);



    Status PassThruIoctl(Handle channelId, IoctlID IoctlID, const void *pInput, void *pOutput);
//    Status clear(Handle channelId, ClearTarget target);

    Status lastError() const { return m_lastError; }
    QString lastErrorString() const;

private:
    Status handleResult(long statusCode);

    template <typename Func>
    Func resolveApiFunction(Func *funcPtr, const char *name) {
        *funcPtr = reinterpret_cast<Func>(m_libJ2534.resolve(name));
        return *funcPtr;
    }

    QLibrary                    m_libJ2534;
    PassThruOpenFunc            m_ptOpen            = nullptr;
    PassThruCloseFunc           m_ptClose           = nullptr;
    PassThruConnectFunc         m_ptConnect         = nullptr;
    PassThruDisconnectFunc      m_ptDisconnect      = nullptr;
    PassThruReadMsgsFunc        m_ptReadMsgs        = nullptr;
    PassThruWriteMsgsFunc       m_ptWriteMsgs       = nullptr;
    PassThruStartMsgFilterFunc  m_ptStartMsgFilter  = nullptr;
    PassThruStopMsgFilterFunc   m_ptStopMsgFilter   = nullptr;
    PassThruGetLastErrorFunc    m_ptGetLastError    = nullptr;
    PassThruIoctlFunc           m_ptIoctl           = nullptr;
    QString                     m_lastErrorString;
    Status                      m_lastError         = NoError;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PassThru::ConnectFlags)

} // namespace J2534

#endif // PASSTHRUCAN_J2534PASSTHRU_H
