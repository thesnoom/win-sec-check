/*
 * main.c
 * 
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#include <Windows.h>
#include <stdio.h>

#include "sysinfo.h"
#include "token.h"


int main(int argc, char **argv)
{
	printf( "+-----------------------+\n"
			"+      WinSecCheck      +\n"
			"+-----------------------+\n"
			"+      - thesnoom       +\n"
			"+        @GitHub        +\n"
			"+-----------------------+\n\n");

	printf("[+] Listing system information:\n");
	DisplayWinVerInfo();
	DisplayCoreInfo();

	printf("\n[+] Listing current process privileges:\n");
	LoopTokens();

	getch();

	return 0;
}