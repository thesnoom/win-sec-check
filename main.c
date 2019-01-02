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



	printf("\n[+] Office recent files:\n");
	printf("-----------------------------------\n");
	ZeroMemory(szFilePath, 256);
	snprintf(szFilePath, 256, "%s\\AppData\\Roaming\\Microsoft\\Office\\Recent", szProfileDir);

	if(FolderExists(szFilePath))
	{
		// Loop folder....
		printf("Office recent exists\n");
	}


	printf("\n[+] Start-menu recent files:\n");
	printf("-----------------------------------\n");
	ZeroMemory(szFilePath, 256);
	snprintf(szFilePath, 256, "%s\\AppData\\Roaming\\Microsoft\\Windows\\Recent", szProfileDir);
	
	if(FolderExists(szFilePath))
	{
		// Loop folder...
		printf("Windows recent exists\n");
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

	getch();

	return 0;
}