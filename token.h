/*
 * token.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#ifndef __WSC_TOKE_H
#define __WSC_TOKE_H

void CurrentUserFromToken( void );
void UserFromPID( DWORD dwProcID, char *szUserOut );
void ListTokenUserGroups( void );
void LoopTokens( void );

#endif