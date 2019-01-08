/*
 * main.c
 * 
 * WinSecCheck.
 *
 * - thesnoom 2018
 * 
 * Credit: MSDN/Google/StackOverflow/MWR Incognito
 */

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "userenv.lib")


#include <Windows.h>
#include <UserEnv.h>
#include <stdio.h>

#include "getopt.h"

#include "main.h"
#include "sysinfo.h"
#include "token.h"
#include "local.h"
#include "adapters.h"
#include "domain.h"
#include "services.h"
#include "descriptors.h"
#include "applications.h"


unsigned long dwDomainFlags;


int main(int argc, char **argv)
{
	printf( "+-----------------------+\n"
			"+      WinSecCheck      +\n"
			"+-----------------------+\n"
			"+      - thesnoom       +\n"
			"+        @GitHub        +\n"
			"+-----------------------+\n\n" );

	// Default option will display system information.
	static struct option long_opts[] =
	{
		{"help",		no_argument, 0, 'h'},	// Help
		{"all",			no_argument, 0, 'a'},	// All Functions
		{"user",		no_argument, 0, 'u'},	// User Information
		{"path",		no_argument, 0, 'p'},	// Path Expansion
		{"tokens",		no_argument, 0, 't'},	// Tokens
		{"processes",	no_argument, 0, 'r'},	// Processes
		{"apps",		no_argument, 0, 'i'},	// Installed Applications
		{"recentfiles",	no_argument, 0, 'f'},	// Recently Used Files.
		{"dominfo",		no_argument, 0, 'd'},	// Recently Used Files.
		{"localinfo",	no_argument, 0, 'l'},	// Recently Used Files.
		{0, 0, 0, 0}
	};

	int opt_idx = 0, c = 0;

	while((c = getopt_long(argc, argv, "v", long_opts, &opt_idx)) != -1)
	{
		switch(c)
		{
			case 'h':
			{
				return 0;
			}

			case 'a':
			{
				return 0;
			}

			case 'u':
			{
				return 0;
			}

			case 'p':
			{
				return 0;
			}

			case 't':
			{
				return 0;
			}

			case 'r':
			{
				return 0;
			}

			case 'i':
			{
				return 0;
			}

			case 'f':
			{
				return 0;
			}

			case 'd':
			{
				return 0;
			}

			case 'l':
			{
				return 0;
			}

			default:
			{
				return 0;
			}
		}
	}

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// ------------------------------------------------
	// System Information
	// ------------------------------------------------
	printf("[+] Listing system information:\n");
	printf("-------------------------------\n");

	DisplayWinVerInfo();
	DisplayCoreInfo();
	DisplayLocalAdapters();

	char szDomTest[256] = { 0 }, szDomFQ[256] = { 0 };
	DWORD dwDomLen = 256;
	GetComputerNameEx(ComputerNamePhysicalDnsDomain, szDomTest, &dwDomLen);

	dwDomLen = 256;
	GetComputerNameEx(ComputerNameDnsFullyQualified, szDomFQ, &dwDomLen);

	if(dwDomainFlags & WSC_DOMAINDC)
	{
		printf("- *** THIS IS A DOMAIN CONTROLLER ***\n");
		printf("-------------------------------------\n");
	}

	if(strlen(szDomTest))
	{
		printf("- %-20s %s\n- %-20s %s\n", "Domain joined PC: ", szDomTest, "PC Domain Name: ", szDomFQ);
		for(size_t i = 0; i < (23 + strlen(szDomFQ)); printf("-"), i++); puts("");

		dwDomainFlags |= WSC_DOMAINJOINED;
	}
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Current User Information
	// ------------------------------------------------
	printf("\n[+] Listing current user information:\n");
	printf("-------------------------------------\n");
	
	char *szUser = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 64);
	if(!szUser)
	{
		printf("[-] HeapAlloc :: Error allocating space (%d).\n", GetLastError());
		exit(-1);
	}

	char *szDomain = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 256);
	if(!szDomain)
	{
		printf("[-] HeapAlloc :: Error allocating space (%d).\n", GetLastError());
		HeapFree(GetProcessHeap(), 0, szUser);
		exit(-1);
	}

	HANDLE hCurrProc = GetCurrentProcess();

	if(!UserFromProc(hCurrProc, szUser, szDomain))
		printf("[-] UserFromToken :: Error retrieving username from token (%d).\n", GetLastError());

	printf("- %-16s %s\\\\%s\n", "Token User:", szDomain, szUser);
	for(size_t i = 0; i < (21 + strlen(szDomain) + strlen(szUser)); printf("-"), i++); puts("");

	ListGroupsFromProc(hCurrProc);

	CloseHandle(hCurrProc);

	DisplayPATH();
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Current Process Privileges
	// ------------------------------------------------
	printf("\n[+] Listing current process privileges:\n");
	printf("---------------------------------------\n");

	LoopTokens();
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Running Processes
	// ------------------------------------------------
	printf("\n[+] Listing running processes:\n");
	printf("------------------------------\n");

	DisplayProcesses();
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Installed Applications
	// ------------------------------------------------
	printf("\n[+] Listing installed applications:\n");
	printf("-----------------------------------\n");

#if _WIN64
	printf("[+] 32BIT Keys:\n");
	printf("---------------\n");

	ListInstalledApps(1);

	printf("\n[+] 64BIT Keys:\n");
	printf("---------------\n");

	ListInstalledApps(0);
#else
	ListInstalledApps(0);
#endif
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Application Specific Findings
	// ------------------------------------------------
	char szProfileDir[128] = { 0 }, szFilePath[256] = { 0 };
	DWORD dwProfileLen = 128;
	GetUserProfileDirectoryA(GetCurrentProcessToken(), szProfileDir, &dwProfileLen);

	printf("\n[+] PowerShell history:\n");
	printf("-----------------------------------\n");

	snprintf(szFilePath, 256, "%s\\AppData\\Roaming\\Microsoft\\Windows\\PowerShell\\PSReadline\\ConsoleHost_history.txt", szProfileDir);
	
	if(FileExists(szFilePath))
		printf("- PowerShell history found: %s\n", szFilePath);
	else
		printf("- PowerShell history not found\n");


	printf("\n[+] Active office recent files:\n");
	printf("-----------------------------------\n");
	ZeroMemory(szFilePath, 256);
	snprintf(szFilePath, 256, "%s\\AppData\\Roaming\\Microsoft\\Office\\Recent", szProfileDir);

	HANDLE hFind;
	WIN32_FIND_DATA w32Find;

	if(FolderExists(szFilePath))
	{
		strncat_s(szFilePath, 256, "\\*", 3);

		hFind = FindFirstFile(szFilePath, &w32Find);

		if(hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if(!strcmp(w32Find.cFileName, ".") || !strcmp(w32Find.cFileName, ".."))
					continue;

				wchar_t *wszResolved;

				if(ResolveLink(szFilePath, w32Find.cFileName, &wszResolved))
				{
					printf("- %ws\n", wszResolved);
					free(wszResolved);
				}

			} while(FindNextFile(hFind, &w32Find));
		}

		FindClose(hFind);
	}


	printf("\n[+] Active start-menu recent files:\n");
	printf("-----------------------------------\n");
	ZeroMemory(szFilePath, 256);
	snprintf(szFilePath, 256, "%s\\AppData\\Roaming\\Microsoft\\Windows\\Recent", szProfileDir);
	
	if(FolderExists(szFilePath))
	{
		strncat_s(szFilePath, 256, "\\*", 3);

		hFind = FindFirstFile(szFilePath, &w32Find);

		if(hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if(!strcmp(w32Find.cFileName, ".") || !strcmp(w32Find.cFileName, ".."))
					continue;
				
				wchar_t *wszResolved;

				if(ResolveLink(szFilePath, w32Find.cFileName, &wszResolved))
				{
					printf("- %ws\n", wszResolved);
					free(wszResolved);
				}

			} while(FindNextFile(hFind, &w32Find));
		}

		FindClose(hFind);
	}
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Local Users & Groups ( If DC it lists domain users... )
	// ------------------------------------------------
	if(dwDomainFlags & WSC_DOMAINDC)
	{
		printf("\n[+] Listing domain user accounts:\n");
		printf("---------------------------------\n");

		LocalInformation(_LOCAL_USERS);

		printf("\n[+] Listing domain groups:\n");
		printf("--------------------------\n");

		LocalInformation(_LOCAL_GROUPS);
	} else {
		printf("\n[+] Listing local user accounts:\n");
		printf("--------------------------------\n");

		LocalInformation(_LOCAL_USERS);

		printf("\n[+] Listing local groups:\n");
		printf("-------------------------\n");

		LocalInformation(_LOCAL_GROUPS);
	}
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Domain Controllers, Users & Groups.
	// ------------------------------------------------
	if((dwDomainFlags & WSC_DOMAINJOINED) && !(dwDomainFlags & WSC_DOMAINDC))
	{
		printf("\n[+] Listing domain information:\n");
		printf("-------------------------------\n");

		ListDomainInfo();
	}
	// ------------------------------------------------
	// ------------------------------------------------

	CoUninitialize();

	getch();

	return 0;
}