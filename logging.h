/*
 * logging.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#ifndef __WSC_LOG_H
#define __WSC_LOG_H

#define LOG_PLAIN		0x00000010
#define LOG_XML			0x00000020
#define LOG_GREP		0x00000040

extern unsigned long dwLogFlags;

#endif