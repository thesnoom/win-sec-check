/*
 * sysinfo.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#ifndef __WSC_SINFO_H
#define __WSC_SINFO_H


typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;

#ifdef _WIN64 //http://www.exploit-monday.com/2013/06/undocumented-ntquerysysteminformation.html
typedef struct _SYSTEM_PROCESS_INFORMATION // Size=184
{
	ULONG NextEntryOffset; // Size=4 Offset=0
	ULONG NumberOfThreads; // Size=4 Offset=4
	LARGE_INTEGER WorkingSetPrivateSize; // Size=8 Offset=8
	ULONG HardFaultCount; // Size=4 Offset=16
	ULONG NumberOfThreadsHighWatermark; // Size=4 Offset=20
	ULONGLONG CycleTime; // Size=8 Offset=24
	LARGE_INTEGER CreateTime; // Size=8 Offset=32
	LARGE_INTEGER UserTime; // Size=8 Offset=40
	LARGE_INTEGER KernelTime; // Size=8 Offset=48
	UNICODE_STRING ImageName; // Size=8 Offset=56
	LONG BasePriority; // Size=4 Offset=64
	PVOID UniqueProcessId; // Size=4 Offset=68
	PVOID InheritedFromUniqueProcessId; // Size=4 Offset=72
	ULONG HandleCount; // Size=4 Offset=76
	ULONG SessionId; // Size=4 Offset=80
	ULONG UniqueProcessKey; // Size=4 Offset=84
	ULONG PeakVirtualSize; // Size=4 Offset=88
	ULONG VirtualSize; // Size=4 Offset=92
	ULONG PageFaultCount; // Size=4 Offset=96
	ULONG PeakWorkingSetSize; // Size=4 Offset=100
	ULONG WorkingSetSize; // Size=4 Offset=104
	ULONG QuotaPeakPagedPoolUsage; // Size=4 Offset=108
	ULONG QuotaPagedPoolUsage; // Size=4 Offset=112
	ULONG QuotaPeakNonPagedPoolUsage; // Size=4 Offset=116
	ULONG QuotaNonPagedPoolUsage; // Size=4 Offset=120
	ULONG PagefileUsage; // Size=4 Offset=124
	ULONG PeakPagefileUsage; // Size=4 Offset=128
	ULONG PrivatePageCount; // Size=4 Offset=132
	LARGE_INTEGER ReadOperationCount; // Size=8 Offset=136
	LARGE_INTEGER WriteOperationCount; // Size=8 Offset=144
	LARGE_INTEGER OtherOperationCount; // Size=8 Offset=152
	LARGE_INTEGER ReadTransferCount; // Size=8 Offset=160
	LARGE_INTEGER WriteTransferCount; // Size=8 Offset=168
	LARGE_INTEGER OtherTransferCount; // Size=8 Offset=176
} SYSTEM_PROCESS_INFORMATION;
#else
typedef struct _SYSTEM_PROCESS_INFORMATION {
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	BYTE Reserved1[48];
	UNICODE_STRING ImageName;
	LONG BasePriority;
	HANDLE UniqueProcessId;
	PVOID Reserved2;
	ULONG HandleCount;
	ULONG SessionId;
	PVOID Reserved3;
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG Reserved4;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	PVOID Reserved5;
	SIZE_T QuotaPagedPoolUsage;
	PVOID Reserved6;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivatePageCount;
	LARGE_INTEGER Reserved7[6];
} SYSTEM_PROCESS_INFORMATION;
#endif

void DisplayCoreInfo( void );
void DisplayWinVerInfo( void );
void DisplayProcesses( void );
void DisplayPATH( void );

#endif