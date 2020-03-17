#pragma once


#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef LPDWORD
typedef DWORD *LPDWORD;
#endif

#ifndef PVOID
typedef void *PVOID;
#endif

#ifndef LPVOID
typedef void *LPVOID;
#endif

#ifndef ULONG
typedef unsigned long ULONG;
#endif

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef WINAPI
#define WINAPI __stdcall
#endif

//#ifndef LPOVERLAPPED
//typedef struct _OVERLAPPED1 {
//	ULONG_PTR Internal;
//	ULONG_PTR InternalHigh;
//	DWORD Offset;
//	DWORD OffsetHigh;
//	HANDLE hEvent;
//} OVERLAPPED1, *LPOVERLAPPED;
//#endif
