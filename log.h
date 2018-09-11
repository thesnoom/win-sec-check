/*
 * log.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */

#ifndef __WSC_LOG_H
#define __WSC_LOG_H

extern int verbosity;

#define VERBOSE(n, x)	{ if(verbosity >= n) x; }

#endif