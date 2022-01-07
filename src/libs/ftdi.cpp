#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <QDebug>
#include "ftdi.h"

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
#else
#include <dlfcn.h>
#include <CoreFoundation/CFBundle.h>
#include <unistd.h>
#endif

ftdi::ftdi(QString _dllName)
{
    hDLL = NULL;
    debugMode = false;
    isLibraryInitialized = false;
    // default to the Openport 1.3 ftdi DLL
#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
   _dllName.toWCharArray(dllName);

   dllName[_dllName.size()] = 0;
    //memcpy(dllName, _dllName, 512);

#else
    strcpy(dllName,"ftd2xx.lib");
#endif
}

void ftdi::setDllName(const char* name)
{
    //strcpy(dllName,name);
}

char* ftdi::getLastError()
{
    return lastError;
}

bool ftdi::valid()
{
    return hDLL != NULL;
}

ftdi::~ftdi(void)
{
#if defined(OP20PT32_USE_LIB)
    if (hDLL)
        ::OP20PT32_Stop();
#else

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    if (hDLL)
        FreeLibrary(hDLL);
#else
    if (hDLL)
        dlclose(hDLL);
#endif
#endif
}

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
#if defined(OP20PT32_USE_LIB)
    #define getPTfn(name) pf##name = ::name;
#else
    #define getPTfn(name) if (!(pf##name = (PF_##name*)GetProcAddress(hDLL,"" #name))) return false
#endif
#else
#if defined(OP20PT32_USE_LIB)
    #define getPTfn(name) pf##name = ::name;
#else
    #define getPTfn(name) if (!(pf##name = (PF_##name*)dlsym(hDLL,"" #name))) return false
#endif
#endif

#define DBGPRINT_BUFSIZE 10240

bool ftdi::getPTfns()     //тут получаем указатели на все нужные нам функции в dll
{
    if (!hDLL)
        return false;
    getPTfn(FT_Open);

//    if (!(pfFT_Open = (PF_FT_Open*)GetProcAddress(hDLL,"FT_Open")))
 //       return false;

    getPTfn(FT_OpenEx);
    getPTfn(FT_CreateDeviceInfoList);
    getPTfn(FT_GetDeviceInfoList);
    getPTfn(FT_ListDevices);
    getPTfn(FT_Close);
    getPTfn(FT_Read);
    getPTfn(FT_Write);
    getPTfn(FT_IoCtl);
    getPTfn(FT_SetBaudRate);
    getPTfn(FT_SetDivisor);
    getPTfn(FT_SetDataCharacteristics);
    getPTfn(FT_SetFlowControl);
    getPTfn(FT_ResetDevice);
    getPTfn(FT_SetDtr);
    getPTfn(FT_ClrDtr);
    getPTfn(FT_SetRts);
    getPTfn(FT_ClrRts);
    getPTfn(FT_GetModemStatus);
    getPTfn(FT_SetChars);
    getPTfn(FT_Purge);
    getPTfn(FT_SetTimeouts);
    getPTfn(FT_GetQueueStatus);
    getPTfn(FT_SetEventNotification);
    getPTfn(FT_GetStatus);
    getPTfn(FT_SetBreakOn);
    getPTfn(FT_SetBreakOff);
    getPTfn(FT_SetWaitMask);
    getPTfn(FT_WaitOnMask);
    getPTfn(FT_GetEventStatus);
    getPTfn(FT_ReadEE);
    getPTfn(FT_WriteEE);
    getPTfn(FT_EraseEE);
    //----------------
    getPTfn(FT_SetLatencyTimer);
    getPTfn(FT_GetLatencyTimer);
    getPTfn(FT_SetBitMode);
    getPTfn(FT_GetBitMode);
    getPTfn(FT_SetUSBParameters);
    getPTfn(FT_SetDeadmanTimeout);
    //-----------------------

    return true;
}

//void ftdi::dbgprint(const char* Format, ...)
//{
//    va_list arglist;
//    int cb;
//    char buffer[DBGPRINT_BUFSIZE];
//    char *pbuf = buffer;

//    va_start(arglist, Format);

//    cb = vsprintf(pbuf, Format, arglist);

//    if (cb == -1)
//    {
//        buffer[sizeof(buffer) - 2] = '\n';
//        buffer[sizeof(buffer) - 1] = '\0';
//    }

//#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
//    OutputDebugStringA(buffer);
//#else
//    printf("%s",buffer);
//#endif
//    va_end(arglist);
//}

#define MSG_READ 1
#define MSG_WRITE 2

//void ftdi::dbgdump(const unsigned char *data,unsigned int datalen,int kind)
//{
//    unsigned int i;
//    char buf[DBGPRINT_BUFSIZE];
//    char *pbuf = buf;

//    if (kind == MSG_READ)
//    {
//        *pbuf++ = 'R';
//        *pbuf++ = ' ';
//    }
//    else if (kind == MSG_WRITE)
//    {
//        *pbuf++ = 'W';
//        *pbuf++ = ' ';
//    }

//    for (i = 0; i < datalen; i++)
//    {
//        if (i > 15 && (i & 0xF) == 0)
//            pbuf += sprintf(pbuf,"\n   ");
//        pbuf += sprintf(pbuf,"%02X ",data[i]);
//    }
//    pbuf += sprintf(pbuf,"\n");
//    dbgprint(buf);
//}


//void ftdi::dbgprintptmsg(const PASSTHRU_MSG *pMsg,int kind)
//{
//    if (!pMsg)
//    {
//        dbgprint("(null message)\n");
//        return;
//    }
//    dbgprint("ProtocolID=%u,RxStatus=%u,TxFlags=%u,Timestamp=%u,DataSize=%u,ExtraDataIndex=%u\n",pMsg->ProtocolID,pMsg->RxStatus,pMsg->TxFlags,pMsg->Timestamp,pMsg->DataSize,pMsg->ExtraDataIndex);
//    dbgdump(pMsg->Data,pMsg->DataSize,kind);
//}

long ftdi::LoadftdiDLL(const TCHAR* szDLL)
{
#if defined(OP20PT32_USE_LIB)
    szDLL; // unused
    if (!hDLL)
        ::OP20PT32_Start();
#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    hDLL = (HINSTANCE)1;
#else
    hDLL = (void*)1;
#endif
    getPTfns();
#else

    if (szDLL == NULL)
    {
        qDebug() << "NULL string pointer to ftdi DLL location.\n";
        return(1);
    }

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    if (!(hDLL = LoadLibrary(szDLL)))
    {
        strcpy(lastError,"error loading ftdi DLL");
        return false;
    }
    else if (!getPTfns())
    {
        // assume unusable if we don't have everything we need
        FreeLibrary(hDLL);
        hDLL = NULL;
        strcpy(lastError,"error loading ftdi DLL function pointers");
        return false;
    }
#else
    CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef,
                                        kCFURLPOSIXPathStyle);
    const char *pathPtr = CFStringGetCStringPtr(macPath,
                                        CFStringGetSystemEncoding());

    char libPath[1024];
    char oldPath[1024];

    getcwd(oldPath,1024);
    strcpy(libPath,pathPtr);
    strcat(libPath,"/Contents/Frameworks");
    chdir(libPath); // change to this dir so ftdi .dylib can find any other needed dylibs in the same dir
    strcat(libPath,"/");
    strcat(libPath,szDLL);

    CFRelease(appUrlRef);
    CFRelease(macPath);

    if (!(hDLL = dlopen(libPath, RTLD_LOCAL|RTLD_LAZY)))
    {
        strcpy(lastError,"error loading ");
        strcat(lastError,libPath);
        chdir(oldPath);
        return false;
    }
    else if (!getPTfns())
    {
        // assume unusable if we don't have everything we need
        dlclose(hDLL);
        hDLL = NULL;
        strcpy(lastError,"error loading ftdi dylib function pointers");
        chdir(oldPath);
        return false;
    }
    chdir(oldPath);
#endif
qDebug() << "DLL loaded successfully\n";
    //DBGPRINT(("DLL loaded successfully\n"));
#endif
    return true;
}

bool ftdi::checkDLL()
{
    if (!hDLL)
        LoadftdiDLL(dllName);
    return (hDLL != NULL);
}
FT_STATUS ftdi::FT_Open (int iDevice, FT_HANDLE *ftHandle)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_Open error";}
    result = (*pfFT_Open)(iDevice, ftHandle);

//qDebug() << "FT_Open returned result iDevice and ftHandle, result" << iDevice << ftHandle << result;
    return result;
}
FT_STATUS ftdi::FT_OpenEx(PVOID pArg1,DWORD Flags,FT_HANDLE *pHandle)
{
    long result = (*pfFT_OpenEx)(pArg1, Flags, pHandle);
    return result;
}

 FT_STATUS ftdi:: FT_CreateDeviceInfoList(LPDWORD numDevs)
 {
     long result = FT_OK;
     result = (*pfFT_CreateDeviceInfoList)( numDevs);
     return result;
 }

FT_STATUS ftdi::FT_GetDeviceInfoList (FT_DEVICE_LIST_INFO_NODE *pDest, LPDWORD lpdwNumDevs)
{
    long result = FT_OK;
    result = (*pfFT_GetDeviceInfoList)( pDest, lpdwNumDevs);
    return result;
}


FT_STATUS ftdi::FT_ListDevices(PVOID pArg1,PVOID pArg2,DWORD Flags)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_ListDevices error";}
    result = (*pfFT_ListDevices)( pArg1, pArg2, Flags);
    qDebug() << "FT_ListDevices returned result " << result;
    return result;
}
FT_STATUS ftdi::FT_Close(FT_HANDLE ftHandle)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_Close error";}
    result = (*pfFT_Close)( ftHandle);
    qDebug() << "FT_Close returned result " << result;
    return result;
}
FT_STATUS ftdi::FT_Read(FT_HANDLE ftHandle,LPVOID lpBuffer,DWORD dwBytesToRead,LPDWORD lpBytesReturned){
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_Read error";}
    result = (*pfFT_Read)( ftHandle, lpBuffer, dwBytesToRead, lpBytesReturned);
    qDebug() << "FT_Read returned result " << result;
    return result;
}
FT_STATUS ftdi::FT_Write(FT_HANDLE ftHandle,LPVOID lpBuffer,DWORD dwBytesToWrite,LPDWORD lpBytesWritten){
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_Read error";}
    result = (*pfFT_Write)( ftHandle, lpBuffer, dwBytesToWrite, lpBytesWritten);
 //   qDebug() << "FT_Write returned result " << result;
    return result;
}
FT_STATUS ftdi::FT_IoCtl(FT_HANDLE ftHandle,DWORD dwIoControlCode,LPVOID lpInBuf,DWORD nInBufSize,LPVOID lpOutBuf,DWORD nOutBufSize,LPDWORD lpBytesReturned,LPOVERLAPPED lpOverlapped)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_IoCtl error";}
    result = (*pfFT_IoCtl)(ftHandle, dwIoControlCode, lpInBuf, nInBufSize, lpOutBuf, nOutBufSize, lpBytesReturned, lpOverlapped);
 //   qDebug() << "FT_IoCtl returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetBaudRate(FT_HANDLE ftHandle,ULONG BaudRate)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetBaudRate error";}
    result = (*pfFT_SetBaudRate)( ftHandle, BaudRate);
 //   qDebug() << "FT_SetBaudRate returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetDivisor(FT_HANDLE ftHandle,USHORT Divisor)
{

    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetDivisor error";}
    result = (*pfFT_SetDivisor)( ftHandle, Divisor);
//    qDebug() << "FT_SetDivisor returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetDataCharacteristics(FT_HANDLE ftHandle,UCHAR WordLength,UCHAR StopBits,UCHAR Parity)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetDataCharacteristics error";}
    result = (*pfFT_SetDataCharacteristics)( ftHandle, WordLength, StopBits, Parity);
//    qDebug() << "FT_SetDataCharacteristics returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetFlowControl(FT_HANDLE ftHandle,USHORT FlowControl,UCHAR XonChar,UCHAR XoffChar)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetFlowControl error";}
    result = (*pfFT_SetFlowControl)( ftHandle, FlowControl, XonChar, XoffChar);
//    qDebug() << "FT_SetFlowControl returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_ResetDevice(FT_HANDLE ftHandle)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_ResetDevice error";}
    result = (*pfFT_ResetDevice)( ftHandle);
//    qDebug() << "FT_ResetDevice returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetDtr(FT_HANDLE ftHandle)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetDtr error";}
    result = (*pfFT_SetDtr)( ftHandle);
 //   qDebug() << "FT_SetDtr returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_ClrDtr(FT_HANDLE ftHandle)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_ClrDtr error";}
    result = (*pfFT_ClrDtr)( ftHandle);
 //   qDebug() << "FT_ClrDtr returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetRts(FT_HANDLE ftHandle)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetRts error";}
    result = (*pfFT_SetRts)( ftHandle);
 //   qDebug() << "FT_SetRts returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_ClrRts(FT_HANDLE ftHandle)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_ClrRts error";}
    result = (*pfFT_ClrRts)( ftHandle);
 //   qDebug() << "FT_ClrRts returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_GetModemStatus(FT_HANDLE ftHandle,ULONG *pModemStatus)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_GetModemStatus error";}
    result = (*pfFT_GetModemStatus)( ftHandle, pModemStatus);
 //   qDebug() << "FT_GetModemStatus returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetChars(FT_HANDLE ftHandle,UCHAR EventChar,UCHAR EventCharEnabled,UCHAR ErrorChar,UCHAR ErrorCharEnabled)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetChars error";}
    result = (*pfFT_SetChars)( ftHandle, EventChar, EventCharEnabled, ErrorChar, ErrorCharEnabled);
 //   qDebug() << "FT_SetChars returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_Purge(FT_HANDLE ftHandle,ULONG Mask){
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_Purge error";}
    result = (*pfFT_Purge)( ftHandle, Mask);
 //   qDebug() << "FT_Purge returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetTimeouts(FT_HANDLE ftHandle,ULONG ReadTimeout,ULONG WriteTimeout){
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetTimeouts error";}
    result = (*pfFT_SetTimeouts)( ftHandle, ReadTimeout, WriteTimeout);
 //   qDebug() << "FT_SetTimeouts returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_GetQueueStatus(FT_HANDLE ftHandle,DWORD *dwRxBytes){
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_GetQueueStatus error";}
    result = (*pfFT_GetQueueStatus)( ftHandle, dwRxBytes);
 //   qDebug() << "FT_GetQueueStatus returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetEventNotification(FT_HANDLE ftHandle,DWORD Mask,PVOID Param)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetEventNotification error";}
    result = (*pfFT_SetEventNotification)( ftHandle, Mask, Param);
 //   qDebug() << "FT_SetEventNotification returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_GetStatus(FT_HANDLE ftHandle,DWORD *dwRxBytes,DWORD *dwTxBytes,DWORD *dwEventDWord)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_GetStatus error";}
    result = (*pfFT_GetStatus)( ftHandle, dwRxBytes, dwTxBytes, dwEventDWord);
 //   qDebug() << "FT_GetStatus returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetBreakOn(FT_HANDLE ftHandle){
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetBreakOn error";}
    result = (*pfFT_SetBreakOn)( ftHandle);
 //   qDebug() << "FT_SetBreakOn returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetBreakOff(FT_HANDLE ftHandle){
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetBreakOff error";}
    result = (*pfFT_SetBreakOff)( ftHandle);
 //   qDebug() << "FT_SetBreakOff returned result" << result;
    return result;
}
FT_STATUS ftdi::FT_SetWaitMask(FT_HANDLE ftHandle,DWORD Mask)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetWaitMask error";}
    result = (*pfFT_SetWaitMask)( ftHandle, Mask);
  //  qDebug() << "FT_SetWaitMask returned result"   << result;
    return result;
}
FT_STATUS ftdi::FT_WaitOnMask(FT_HANDLE ftHandle,DWORD *Mask)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_WaitOnMask error";}
    result = (*pfFT_WaitOnMask)( ftHandle, Mask);
  //  qDebug() << "FT_WaitOnMask returned result"   << result;
    return result;
}
FT_STATUS ftdi::FT_GetEventStatus(FT_HANDLE ftHandle,DWORD *dwEventDWord)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_GetEventStatus error";}
    result = (*pfFT_GetEventStatus)( ftHandle, dwEventDWord);
  //  qDebug() << "FT_GetEventStatus returned result"   << result;
    return result;
}
FT_STATUS ftdi::FT_ReadEE(FT_HANDLE ftHandle,DWORD dwWordOffset,LPWORD lpwValue){
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_ReadEE error";}
    result = (*pfFT_ReadEE)( ftHandle, dwWordOffset, lpwValue);
 //   qDebug() << "FT_ReadEE returned result, lpwValue"  << result << lpwValue;
    return result;
}
FT_STATUS ftdi::FT_WriteEE(FT_HANDLE ftHandle,DWORD dwWordOffset,WORD wValue)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_WriteEE error";}
    result = (*pfFT_WriteEE)( ftHandle, dwWordOffset, wValue);
  //  qDebug() << "FT_WriteEE returned result"   << result;
    return result;
}
FT_STATUS ftdi::FT_EraseEE(FT_HANDLE ftHandle)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_EraseEE error";}
    result = (*pfFT_EraseEE)( ftHandle);
  //  qDebug() << "FT_EraseEE returned result"   << result;
    return result;
}
//-------------------------
FT_STATUS ftdi::FT_SetLatencyTimer(FT_HANDLE ftHandle, UCHAR ucLatency ){
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetLatencyTimer error";}
    result = (*pfFT_SetLatencyTimer)( ftHandle, ucLatency);
  //  qDebug() << "FT_SetLatencyTimer returned result"   << result;
    return result;
}
FT_STATUS ftdi::FT_GetLatencyTimer(FT_HANDLE ftHandle, PUCHAR pucLatency )
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_GetLatencyTimer error";}
    result = (*pfFT_GetLatencyTimer)( ftHandle, pucLatency);
  //  qDebug() << "FT_GetLatencyTimer returned result"   << result;
    return result;
}
FT_STATUS ftdi::FT_SetBitMode( FT_HANDLE ftHandle, UCHAR ucMask, UCHAR ucEnable )
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetBitMode error";}
    result = (*pfFT_SetBitMode)( ftHandle, ucMask, ucEnable);
  //  qDebug() << "FT_SetBitMode returned result"   << result;
    return result;
}
FT_STATUS ftdi::FT_GetBitMode( FT_HANDLE ftHandle, PUCHAR pucMode )
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_GetBitMode error";}
    result = (*pfFT_GetBitMode)( ftHandle, pucMode);
  //  qDebug() << "FT_GetBitMode returned result"   << result;
    return result;
}
FT_STATUS ftdi::FT_SetUSBParameters(FT_HANDLE ftHandle, ULONG ulInTransferSize, ULONG ulOutTransferSize )
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetUSBParameters error";}
    result = (*pfFT_SetUSBParameters)( ftHandle, ulInTransferSize,  ulOutTransferSize);
  //  qDebug() << "FT_SetUSBParameters returned result"   << result;
    return result;
}
FT_STATUS ftdi::FT_SetDeadmanTimeout( FT_HANDLE ftHandle, ULONG ulDeadmanTimeout)
{
    long result = FT_OK;
    if (!checkDLL()){
        return FT_DEVICE_NOT_FOUND;
    qDebug() << "FT_SetDeadmanTimeout error";}
    result = (*pfFT_SetDeadmanTimeout)( ftHandle, ulDeadmanTimeout);
  //  qDebug() << "FT_SetDeadmanTimeout returned result"   << result;
    return result;
}

