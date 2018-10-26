/*
 * main.c
 * 
 * WinSecCheck.
 *
 * - thesnoom 2018
 * 
 * Credit: MSDN/Google/StackOverflow/MWR Incognito
 */


#include <Windows.h>
#include <stdio.h>

#include "sysinfo.h"
#include "token.h"
#include "local.h"


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
	DisplayWinVerInfo();
	DisplayCoreInfo();
	// ------------------------------------------------
	// ------------------------------------------------


	// ------------------------------------------------
	// Current User Information
	// ------------------------------------------------
	printf("\n[+] Listing current user information:\n");
	
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
	LoopTokens();
	// ------------------------------------------------
	// ------------------------------------------------


	// ------------------------------------------------
	// Running Processes
	// ------------------------------------------------
	printf("\n[+] Listing running processes:\n");
	DisplayProcesses();
	// ------------------------------------------------
	// ------------------------------------------------


	// ------------------------------------------------
	// Local Users & Groups
	// ------------------------------------------------
	printf("\n[+] Listing local user accounts:\n");
	LocalInformation(_LOCAL_USERS);

	printf("\n[+] Listing local groups:\n");
	LocalInformation(_LOCAL_GROUPS);
	// ------------------------------------------------
	// ------------------------------------------------
	
	getch();

	return 0;
}