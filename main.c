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


int main(int argc, char **argv)
{
	printf( "+-----------------------+\n"
			"+      WinSecCheck      +\n"
			"+-----------------------+\n"
			"+      - thesnoom       +\n"
			"+        @GitHub        +\n"
			"+-----------------------+\n\n");

	printf("[+] Outputting system information:\n");
	DisplayWinVerInfo();
	DisplayCoreInfo();

	getch();

	return 0;
}


