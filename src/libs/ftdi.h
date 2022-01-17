#pragma once

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    #include <windows.h>
#endif

#include "ftdi_types.h"
#include <QString>

#define PTfn(name) PF_##name* pf##name
#define PText(name) PT_API PF_##name name

#define DBGPRINT(args_in_parens)                                \
    {                                               \
        if (debugMode)	\
            dbgprint args_in_parens; \
    }

#define DBGDUMP(args_in_parens)                                \
    {                                               \
        if (debugMode)	\
            dbgdump args_in_parens; \
    }

#define DBGPRINTPT(args_in_parens)                                \
    {                                               \
        if (debugMode)	\
            dbgprintptmsg args_in_parens; \
    }

class ftdi
{
public:
    ftdi(QString _dllName);
    ~ftdi(void);
    bool init() { return checkDLL(); }
    void setDllName(const char* name);
    bool valid();
    void debug(bool enable) { debugMode = enable; }
    char* getLastError();

    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;

    FT_STATUS FT_Open (int iDevice, FT_HANDLE *ftHandle);
    FT_STATUS FT_OpenEx(PVOID pArg1,DWORD Flags,FT_HANDLE *pHandle);
    FT_STATUS FT_CreateDeviceInfoList(LPDWORD numDevs);
    FT_STATUS FT_GetDeviceInfoList(FT_DEVICE_LIST_INFO_NODE *pDest, LPDWORD lpdwNumDevs);
    FT_STATUS FT_ListDevices(PVOID pArg1,PVOID pArg2,DWORD Flags);
    FT_STATUS FT_Close(FT_HANDLE ftHandle);
    FT_STATUS FT_Read(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToRead, LPDWORD lpBytesReturned);
    FT_STATUS FT_Write(FT_HANDLE ftHandle,LPVOID lpBuffer,DWORD dwBytesToWrite,LPDWORD lpBytesWritten);
    FT_STATUS FT_IoCtl(FT_HANDLE ftHandle,DWORD dwIoControlCode,LPVOID lpInBuf,DWORD nInBufSize,LPVOID lpOutBuf,DWORD nOutBufSize,LPDWORD lpBytesReturned,LPOVERLAPPED lpOverlapped);
    FT_STATUS FT_SetBaudRate(FT_HANDLE ftHandle,ULONG BaudRate);
    FT_STATUS FT_SetDivisor(FT_HANDLE ftHandle,USHORT Divisor);
    FT_STATUS FT_SetDataCharacteristics(FT_HANDLE ftHandle,UCHAR WordLength,UCHAR StopBits,UCHAR Parity);
    FT_STATUS FT_SetFlowControl(FT_HANDLE ftHandle,USHORT FlowControl,UCHAR XonChar,UCHAR XoffChar);
    FT_STATUS FT_ResetDevice(FT_HANDLE ftHandle);
    FT_STATUS FT_SetDtr(FT_HANDLE ftHandle);
    FT_STATUS FT_ClrDtr(FT_HANDLE ftHandle);
    FT_STATUS FT_SetRts(FT_HANDLE ftHandle);
    FT_STATUS FT_ClrRts(FT_HANDLE ftHandle);
    FT_STATUS FT_GetModemStatus(FT_HANDLE ftHandle,ULONG *pModemStatus);
    FT_STATUS FT_SetChars(FT_HANDLE ftHandle,UCHAR EventChar,UCHAR EventCharEnabled,UCHAR ErrorChar,UCHAR ErrorCharEnabled);
    FT_STATUS FT_Purge(FT_HANDLE ftHandle,ULONG Mask);
    FT_STATUS FT_SetTimeouts(FT_HANDLE ftHandle,ULONG ReadTimeout,ULONG WriteTimeout);
    FT_STATUS FT_GetQueueStatus(FT_HANDLE ftHandle,DWORD *dwRxBytes);
    FT_STATUS FT_SetEventNotification(FT_HANDLE ftHandle,DWORD Mask,PVOID Param);
    FT_STATUS FT_GetStatus(FT_HANDLE ftHandle,DWORD *dwRxBytes,DWORD *dwTxBytes,DWORD *dwEventDWord);
    FT_STATUS FT_SetBreakOn(FT_HANDLE ftHandle);
    FT_STATUS FT_SetBreakOff(FT_HANDLE ftHandle);
    FT_STATUS FT_SetWaitMask(FT_HANDLE ftHandle,DWORD Mask);
    FT_STATUS FT_WaitOnMask(FT_HANDLE ftHandle,DWORD *Mask);
    FT_STATUS FT_GetEventStatus(FT_HANDLE ftHandle,DWORD *dwEventDWord);
    FT_STATUS FT_ReadEE(FT_HANDLE ftHandle,DWORD dwWordOffset,LPWORD lpwValue);
    FT_STATUS FT_WriteEE(FT_HANDLE ftHandle,DWORD dwWordOffset,WORD wValue);
    FT_STATUS FT_EraseEE(FT_HANDLE ftHandle);
//----------------------------------------------
    FT_STATUS FT_SetLatencyTimer(FT_HANDLE ftHandle, UCHAR ucLatency );
    FT_STATUS FT_GetLatencyTimer(FT_HANDLE ftHandle, PUCHAR pucLatency );
    FT_STATUS FT_SetBitMode( FT_HANDLE ftHandle, UCHAR ucMask, UCHAR ucEnable );
    FT_STATUS FT_GetBitMode( FT_HANDLE ftHandle, PUCHAR pucMode );
    FT_STATUS FT_SetUSBParameters(FT_HANDLE ftHandle, ULONG ulInTransferSize, ULONG ulOutTransferSize );
    FT_STATUS FT_SetDeadmanTimeout( FT_HANDLE ftHandle, ULONG ulDeadmanTimeout);
    //-------------------------------------------------





private:
    bool getPTfns();
    long LoadftdiDLL(const TCHAR *szDLL);
    bool checkDLL();
//    void dbgprint(const char* Format, ...);
//    void dbgdump(const unsigned char *data,unsigned int datalen,int kind);
//    void dbgprintptmsg(const PASSTHRU_MSG *pMsg,int kind);
//    int is_valid_sconfig_param(SCONFIG s);
//    void dump_sbyte_array(const SBYTE_ARRAY* s);
//    void dump_sconfig_param(SCONFIG s);

    char lastError[256];
    TCHAR dllName[256];
    bool debugMode;
    bool isLibraryInitialized;


#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    HINSTANCE hDLL;               // Handle to DLL
#else
    void* hDLL;
#endif

    /* ftdi Interface API function pointers */
    PTfn(FT_Open);
    PTfn(FT_OpenEx);
    PTfn(FT_CreateDeviceInfoList);
    PTfn(FT_GetDeviceInfoList);
    PTfn(FT_ListDevices);
    PTfn(FT_Close);
    PTfn(FT_Read);
    PTfn(FT_Write);
    PTfn(FT_IoCtl);
    PTfn(FT_SetBaudRate);
    PTfn(FT_SetDivisor);
    PTfn(FT_SetDataCharacteristics);
    PTfn(FT_SetFlowControl);
    PTfn(FT_ResetDevice);
    PTfn(FT_SetDtr);
    PTfn(FT_ClrDtr);
    PTfn(FT_SetRts);
    PTfn(FT_ClrRts);
    PTfn(FT_GetModemStatus);
    PTfn(FT_SetChars);
    PTfn(FT_Purge);
    PTfn(FT_SetTimeouts);
    PTfn(FT_GetQueueStatus);
    PTfn(FT_SetEventNotification);
    PTfn(FT_GetStatus);
    PTfn(FT_SetBreakOn);
    PTfn(FT_SetBreakOff);
    PTfn(FT_SetWaitMask);
    PTfn(FT_WaitOnMask);
    PTfn(FT_GetEventStatus);
    PTfn(FT_ReadEE);
    PTfn(FT_WriteEE);
    PTfn(FT_EraseEE);
    //----------------
    PTfn(FT_SetLatencyTimer);
    PTfn(FT_GetLatencyTimer);
    PTfn(FT_SetBitMode);
    PTfn(FT_GetBitMode);
    PTfn(FT_SetUSBParameters);
    PTfn(FT_SetDeadmanTimeout);
    //-----------------------
};

#if defined(FTD2XX_LIB)
    PT_API void OP20PT32_Start();
    PT_API void OP20PT32_Stop();
    PText(FT_Open);
    PText(FT_OpenEx);
    PText(FT_CreateDeviceInfoList);
    PText(FT_GetDeviceInfoList);
    PText(FT_ListDevices);
    PText(FT_Close);
    PText(FT_Read);
    PText(FT_Write);
    PText(FT_IoCtl);
    PText(FT_SetBaudRate);
    PText(FT_SetDivisor);
    PText(FT_SetDataCharacteristics);
    PText(FT_SetFlowControl);
    PText(FT_ResetDevice);
    PText(FT_SetDtr);
    PText(FT_ClrDtr);
    PText(FT_SetRts);
    PText(FT_ClrRts);
    PText(FT_GetModemStatus);
    PText(FT_SetChars);
    PText(FT_Purge);
    PText(FT_SetTimeouts);
    PText(FT_GetQueueStatus);
    PText(FT_SetEventNotification);
    PText(FT_GetStatus);
    PText(FT_SetBreakOn);
    PText(FT_SetBreakOff);
    PText(FT_SetWaitMask);
    PText(FT_WaitOnMask);
    PText(FT_GetEventStatus);
    PText(FT_ReadEE);
    PText(FT_WriteEE);
    PText(FT_EraseEE);

#endif

