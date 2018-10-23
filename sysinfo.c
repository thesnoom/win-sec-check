/*
 * winver.c
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#pragma comment(lib, "netapi32.lib")
#pragma warning(disable : 4731)


#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "sysinfo.h"


typedef LONG (WINAPI * RtlGetVer)(OSVERSIONINFOEXW *);
#define STATUS_SUCCESS 0x00000000


// Detect HT and use GetSystemInfo to display phy/log cores.
void DisplayCoreInfo( void )
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

	char finalDisp[256] = { 0 };

	if(szCpuInfo[0] != '\0')
		snprintf(finalDisp, sizeof(finalDisp), "x%d - %dC(%dT) - %s\n", nArch, corePhys, coreLog, szCpuInfo);
	else
		snprintf(finalDisp, sizeof(finalDisp), "x%d - %dC(%dT)\n", nArch, corePhys, coreLog);

	printf("- %-20s %s", "CPU Info:", finalDisp);
}


// RtlGetVersion() :: https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/wdm/nf-wdm-rtlgetversion
// Since Microsoft broke GetVersionEx >= W8.1, this is the best method.
void DisplayWinVerInfo( void )
{
	OSVERSIONINFOEXW osVer	= { 0 };
	HMODULE hNtDLL			= NULL;
	RtlGetVer RtlGetVersion = NULL;

	hNtDLL = LoadLibraryA("ntdll.dll");
	if(!hNtDLL)
	{
		printf("[!] LoadLibrary (%d) :: Error getting handle to ntdll.\n", GetLastError());
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
				char szProdVer[24]		= { 0 };
				char szNtType[24]		= { 0 };
				DWORD dwBuffSize		= 128;
				DWORD dwProdSize		= 24;

				ZeroMemory(szServPack, 128);
				ZeroMemory(szProdName, 128);
				ZeroMemory(szProdVer,	24);
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
				RegGetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ReleaseId", RRF_RT_ANY, NULL, (void *)&szProdVer, &dwProdSize);

				char finalDisp[256] = { 0 };
				if(szProdVer[0] != '\0')
					snprintf(finalDisp, sizeof(finalDisp), "%s %s - %d.%d.%d (%s)\n", szProdName, szProdVer, osVer.dwMajorVersion, osVer.dwMinorVersion, osVer.dwBuildNumber, szNtType);
				else
					snprintf(finalDisp, sizeof(finalDisp), "%s - %d.%d.%d (%s)\n", szProdName, osVer.dwMajorVersion, osVer.dwMinorVersion, osVer.dwBuildNumber, szNtType);

				printf("- %-20s %s", "System Version:", finalDisp);

				ZeroMemory(finalDisp, sizeof(finalDisp));
				snprintf(finalDisp, sizeof(finalDisp), "%d.%d (%s)\n", osVer.wServicePackMajor, osVer.wServicePackMinor, szServPack);

				printf("- %-20s %s", "Service Pack:", finalDisp);
			}

		} else
			printf("[!] GetProcAddress (%d) :: RtlGetVersion API address.\n", GetLastError());

		FreeLibrary(hNtDLL);
	}
}

