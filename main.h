/*
 * main.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#ifndef __WSC_MAIN_H
#define __WSC_MAIN_H

#define WSC_DOMAINJOINED	0x00000010
#define WSC_DOMAINDC		0x00000020

#define OPTS_USER			0x00000010
#define OPTS_PATH			0x00000020
#define OPTS_TOKENS			0x00000040
#define OPTS_PROCS			0x00000080
#define OPTS_APPS			0x00000100
#define OPTS_RECENT			0x00000200
#define OPTS_DOM			0x00000400
#define OPTS_LOC			0x00000800
#define	OPTS_ADAPTS			0x00001000

#define OPTS_ALL			0x00100000

extern unsigned long dwDomainFlags;

void help( );

#endif