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

	printf("[+] Listing system information:\n");
	DisplayWinVerInfo();
	DisplayCoreInfo();

	printf("\n[+] Listing current user information:\n");
	CurrentUserFromToken();
	ListTokenUserGroups();
	DisplayPATH();

	printf("\n[+] Listing current process privileges:\n");
	LoopTokens();

	printf("\n[+] Listing running processes:\n");
	DisplayProcesses();

	printf("\n[+] Listing local user accounts:\n");
	LocalInformation(_LOCAL_USERS);

	printf("\n[+] Listing local groups:\n");
	LocalInformation(_LOCAL_GROUPS);

	getch();

	return 0;
}