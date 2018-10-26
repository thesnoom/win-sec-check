/*
 * token.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#ifndef __WSC_TOKE_H
#define __WSC_TOKE_H

BOOL UserFromProc( HANDLE hProc, char *szUserOut, char *szDomOut );
void UserFromPID(DWORD dwProcID, char *szUserOut, char *szDomOut );

BOOL ListGroupsFromProc( HANDLE hProc );

void LoopTokens( void );

#endif