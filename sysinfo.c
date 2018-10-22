/*
 * winver.c
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#pragma comment(lib, "netapi32.lib")

#include <Windows.h>
#include <LM.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

#include "sysinfo.h"


typedef LONG (WINAPI * RtlGetVer)(OSVERSIONINFOEXW *);
#define STATUS_SUCCESS 0x00000000


// Detect HT and use GetSystemInfo to display phy/log cores.
void DisplayCoreInfo()
{
	int nHyper = 0, nArch = 0, corePhys, coreLog;
	char szCpuInfo[64] = { 0 };
	unsigned regs[4] = { 0 };

	__asm
	{
		pushad

		mov eax, 1
		cpuid

		test edx, 0x10000000
		jz Nope
		mov nHyper, 1

		Nope:
		popad
	}

	// Query extended identifiers
	__cpuid(regs, 0x80000000);

	if(regs[0] >= 0x80000002) // 0x8000000[2-4] -> CPU String
	{
		__cpuid(regs, 0x80000002);
		memcpy(szCpuInfo, regs, sizeof(regs));

		__cpuid(regs, 0x80000003);
		memcpy(szCpuInfo + 16, regs, sizeof(regs));

		__cpuid(regs, 0x80000004);
		memcpy(szCpuInfo + 32, regs, sizeof(regs));
	}

	SYSTEM_INFO sysInfo = { 0 };
	GetNativeSystemInfo(&sysInfo);

	switch(sysInfo.wProcessorArchitecture)
	{
		case PROCESSOR_ARCHITECTURE_AMD64:
		{
			nArch = 64;
			break;
		}

		case PROCESSOR_ARCHITECTURE_INTEL:
		{
			nArch = 86;
			break;
		}
	}

	corePhys = coreLog = (int)sysInfo.dwNumberOfProcessors;

	if(nHyper)
		corePhys /= 2;

	printf("\tCPU Info:\t\tx%d - %dC(%dT) - %s\n", nArch, corePhys, coreLog, szCpuInfo);

}


// RtlGetVersion() :: https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/wdm/nf-wdm-rtlgetversion
// Since microsoft broke GetVersionEx >= W8.1, this is the best method.
void DisplayWinVerInfo( void )
{
	OSVERSIONINFOEXW osVer	= { 0 };
	HMODULE hNtDLL			= NULL;
	RtlGetVer RtlGetVersion = NULL;

	hNtDLL = LoadLibraryA("ntdll.dll");
	if(!hNtDLL)
	{
		printf("[!] LoadLibrary :: Error getting handle to ntdll.\n");
		return;
	} else {
		RtlGetVersion = (RtlGetVer)GetProcAddress(hNtDLL, "RtlGetVersion");
		if(RtlGetVersion)
		{
			ZeroMemory(&osVer, sizeof(OSVERSIONINFOEXW));
			osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);

			if(RtlGetVersion(&osVer) == STATUS_SUCCESS)
			{
				char szServPack[128]	= { 0 };
				char szProdName[128]	= { 0 };
				char szNtType[24]		= { 0 };
				DWORD dwBuffSize		= 128;

				ZeroMemory(szServPack, 128);
				ZeroMemory(szProdName, 128);
				ZeroMemory(szNtType,	24);

				switch(osVer.wProductType)
				{
					case VER_NT_WORKSTATION:
					{
						memcpy(szNtType, "Desktop", strlen("Desktop"));
						break;
					}

					case VER_NT_SERVER:
					{
						memcpy(szNtType, "Server", strlen("Server"));
						break;
					}

					case VER_NT_DOMAIN_CONTROLLER:
					{
						memcpy(szNtType, "Domain Controller", strlen("Domain Controller"));
						break;
					}
				}

				if(osVer.szCSDVersion[0] == L'\0')
					memcpy(szServPack, "N/A", strlen("N/A"));
				else {
					WCHAR *src	= osVer.szCSDVersion;
					char *dst	= szServPack;

					while(*src)
						*dst++ = (char)*src++;
				}

				RegGetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName", RRF_RT_ANY, NULL, (void *)&szProdName, &dwBuffSize);

				printf("\tSystem Version:\t\t%s - %d.%d.%d (%s)\n", szProdName, osVer.dwMajorVersion, osVer.dwMinorVersion, osVer.dwBuildNumber, szNtType);
				printf("\tService Pack:\t\t%d.%d (%s)\n", osVer.wServicePackMajor, osVer.wServicePackMinor, szServPack);
			}

		} else
			printf("[!] GetProcAddress :: RtlGetVersion API address.\n");

		FreeLibrary(hNtDLL);
	}
}

