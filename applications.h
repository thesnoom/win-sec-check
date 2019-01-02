/*
 * sysinfo.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#ifndef __WSC_IAPPS_H
#define __WSC_IAPPS_H

BOOL FileExists( LPCTSTR szPath );
BOOL FolderExists( LPCTSTR szPath );
void ListInstalledApps( int b32BitApps );

#endif