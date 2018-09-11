/*
 * main.c
 * 
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#include <Windows.h>
#include <stdio.h>

#include "getopt.h"
#include "log.h"


void help()
{
	//
}


int main(int argc, char **argv)
{
	printf( "+-----------------------+\n"
			"+      WinSecCheck      +\n"
			"+-----------------------+\n"
			"+      - thesnoom       +\n"
			"+        @GitHub        +\n"
			"+-----------------------+\n\n");


	static struct option long_opts[] =
	{
		{"help",		no_argument, 0, 'h'},
		{"verbose",		no_argument, 0, 'v'},
		{0, 0, 0, 0}
	};

	int opt_idx = 0, c = 0, verbosity = 0;

	while((c = getopt_long(argc, argv, "v", long_opts, &opt_idx)) != -1)
	{
		switch(c)
		{
			case 'h':
			{
				help();
				return 0;
			}

			case 'v':
			{
				verbosity++;
			}
		}
	}

	return 0;
}


