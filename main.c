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
unsigned long dwOptFlags = 0x00;

static disp_once = 0;

int main(int argc, char **argv)
{
	static struct option long_opts[] =
	{
		{"help",		no_argument, 0, 'h'},	// Help
		{"all",			no_argument, 0, 'A'},	// All Functions
		{"user",		no_argument, 0, 'u'},	// User Information
		{"path",		no_argument, 0, 'P'},	// Path Expansion
		{"tokens",		no_argument, 0, 't'},	// Tokens
		{"processes",	no_argument, 0, 'p'},	// Processes
		{"apps",		no_argument, 0, 'a'},	// Installed Applications
		{"recentfiles",	no_argument, 0, 'r'},	// Recently Used Files
		{"dominfo",		no_argument, 0, 'd'},	// Domain User/Group Information
		{"localinfo",	no_argument, 0, 'l'},	// Local User/Group Information
		{"adapters",	no_argument, 0, 'n'},	// Adapter Information
		{0, 0, 0, 0}
	};

	int opt_idx = 0, c = 0;

	while((c = getopt_long(argc, argv, "hAuPtpardln", long_opts, &opt_idx)) != -1)
	{
		switch(c)
		{
			case 'h':
			{
				help();
				break;
			}

			case 'A':
			{
				dwOptFlags |= OPTS_ALL;
				break;
			}

			case 'u':
			{
				dwOptFlags |= OPTS_USER;
				break;
			}

			case 'P':
			{
				dwOptFlags |= OPTS_PATH;
				break;
			}

			case 't':
			{
				dwOptFlags |= OPTS_TOKENS;
				break;
			}

			case 'p':
			{
				dwOptFlags |= OPTS_PROCS;
				break;
			}

			case 'a':
			{
				dwOptFlags |= OPTS_APPS;
				break;
			}

			case 'r':
			{
				dwOptFlags |= OPTS_RECENT;
				break;
			}

			case 'd':
			{
				dwOptFlags |= OPTS_DOM;
				break;
			}

			case 'l':
			{
				dwOptFlags |= OPTS_LOC;
				break;
			}

			case 'n':
			{
				dwOptFlags |= OPTS_ADAPTS;
				break;
			}

			default:
			case '?':
			{
				if(!disp_once)
				{
					printf("Invalid option(s): ");
					disp_once++;
				}

				if(optopt)
					printf("%c", optopt);
				else
					printf("%s ", argv[optind - 1]);

				break;
			}
		}
	}

	if(disp_once)
		puts("");

	for(int n = optind; optind < argc; n++)
		if(argv[n])
			printf("Invalid argument: %s\n", argv[n]);

	if(optind <= 1 || disp_once)
		help();


	CoInitializeEx(NULL, COINIT_MULTITHREADED);


	printf("+-----------------------+\n"
		   "+      WinSecCheck      +\n"
		   "+-----------------------+\n"
		   "+      - thesnoom       +\n"
		   "+        @GitHub        +\n"
		   "+-----------------------+\n\n");


	// ------------------------------------------------
	// System Information  ( ALWAYS SHOWN )
	// ------------------------------------------------
	printf("[+] Listing system information:\n");
	printf("-------------------------------\n");

	DisplayWinVerInfo();
	DisplayCoreInfo();

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
	// Network Adapter Information
	// ------------------------------------------------
	if((dwOptFlags & OPTS_ALL) || (dwOptFlags & OPTS_ADAPTS))
	{
		printf("\n[+] Listing network adapter information:\n");
		printf("----------------------------------------\n");
		DisplayLocalAdapters();
	}
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Current User Information
	// ------------------------------------------------
	if((dwOptFlags & OPTS_ALL) || (dwOptFlags & OPTS_USER))
	{
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

		HeapFree(GetProcessHeap(), 0, szUser);
		HeapFree(GetProcessHeap(), 0, szDomain);

		CloseHandle(hCurrProc);
	}

	if((dwOptFlags & OPTS_ALL) || (dwOptFlags & OPTS_PATH))
		DisplayPATH();
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Current Process Privileges
	// ------------------------------------------------
	if((dwOptFlags & OPTS_ALL) || (dwOptFlags & OPTS_TOKENS))
	{
		printf("\n[+] Listing current process privileges:\n");
		printf("---------------------------------------\n");

		LoopTokens();
	}
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Running Processes
	// ------------------------------------------------
	if((dwOptFlags & OPTS_ALL) || (dwOptFlags & OPTS_PROCS))
	{
		printf("\n[+] Listing running processes:\n");
		printf("------------------------------\n");

		DisplayProcesses();
	}
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Installed Applications
	// ------------------------------------------------
	if((dwOptFlags & OPTS_ALL) || (dwOptFlags & OPTS_APPS))
	{
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
	}
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Application Specific Findings
	// ------------------------------------------------
	if((dwOptFlags & OPTS_ALL) || (dwOptFlags & OPTS_RECENT))
	{
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
	}
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Domain Controllers, Users & Groups.
	// ------------------------------------------------
	if((dwOptFlags & OPTS_ALL) || (dwOptFlags & OPTS_DOM))
	{
		if((dwDomainFlags & WSC_DOMAINJOINED) && !(dwDomainFlags & WSC_DOMAINDC))
		{
			printf("\n[+] Listing domain information:\n");
			printf("-------------------------------\n");

			ListDomainInfo();
		}

		// Ran on domain DC asking domain info, apply LOC flag instead. 
		if((dwDomainFlags & WSC_DOMAINJOINED) && !(dwDomainFlags & WSC_DOMAINDC) && !(dwOptFlags & OPTS_LOC))
			dwOptFlags |= OPTS_LOC;
	}
	// ------------------------------------------------
	// ------------------------------------------------




	// ------------------------------------------------
	// Local Users & Groups ( If DC it lists domain users... )
	// ------------------------------------------------
	if((dwOptFlags & OPTS_ALL) || (dwOptFlags & OPTS_LOC))
	{
		char *szType;
		if(dwDomainFlags & WSC_DOMAINDC)
			szType = "domain";
		else
			szType = "local";

		printf("\n[+] Listing %s user accounts:\n", szType);
		printf("---------------------------------\n");

		LocalInformation(_LOCAL_USERS);

		printf("\n[+] Listing %s groups:\n", szType);
		printf("--------------------------\n");

		LocalInformation(_LOCAL_GROUPS);
	}
	// ------------------------------------------------
	// ------------------------------------------------


	CoUninitialize();

	getch();

	return 0;
}


// Display the help function for teh new guys.
void help()
{
	printf(	"usage: WinSecCheck [-A] | [-uPtpardln]\n\n"
			"Arguments:\n"
			"-h, --help:\t\tDisplay(this);\n"
			"-A, --all:\t\tDisplay everything.\n"
			"-u, --user:\t\tUser information (PC/Domain & Groups).\n"
			"-P, --path:\t\tPath variable + DACL information.\n"
			"-t, --tokens:\t\tCurrent user token privileges.\n"
			"-p, --processes:\tCurrent running processes (PID/User/ImageName).\n"
			"-a, --apps:\t\tInstalled applications (x86/x64).\n"
			"-r, --recentfiles:\tRecently used files - office, start-menu, PS history.\n"
			"-d, --dominfo:\t\tDomain information (Users & Groups).\n"
			"-l, --localinfo:\tLocal information (User & Groups).\n"
			"-n, --adapters:\t\tNetwork Adapter information (Gateway/IP/DNS).\n\n" );

	exit(0);
}