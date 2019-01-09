/*
 * applications.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#ifndef __WSC_IAPPS_H
#define __WSC_IAPPS_H

BOOL FileExists( char *szPath );
BOOL FolderExists( char *szPath );
BOOL ResolveLink( char *szPath, char *szFile, wchar_t **wszOut );
void ListInstalledApps( int b32BitApps );

#endif