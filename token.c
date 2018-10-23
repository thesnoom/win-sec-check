/*
 * token.c
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */

#include <Windows.h>
#include <winnt.h>

#include <stdio.h>


// Iterate through all tokens in current process
void LoopTokens( void )
{
	HANDLE hCurrProc;
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &hCurrProc))
	{
		void *tokenPrivs[1024] = { 0 };
		char privName[64] = { 0 }, privDisp[256] = { 0 };

		DWORD dwPrivLen, dwNameLen = 64, dwDispLen = 256, i, dwNull, privSet;

		// Get current privileges
		if(GetTokenInformation(hCurrProc, TokenPrivileges, tokenPrivs, 1024, &dwPrivLen))
		{
			for(i = 0; i < ((TOKEN_PRIVILEGES *)tokenPrivs)->PrivilegeCount; i++)
			{
				dwNameLen = 64, dwDispLen = 256, privSet = ((TOKEN_PRIVILEGES *)tokenPrivs)->Privileges[i].Attributes;

				LookupPrivilegeNameA(NULL, &(((TOKEN_PRIVILEGES *)tokenPrivs)->Privileges[i].Luid), privName, &dwNameLen);
				LookupPrivilegeDisplayNameA(NULL, privName, privDisp, &dwDispLen, &dwNull);

				char finalDisplay[512] = { 0 };
				snprintf(finalDisplay, sizeof(finalDisplay), "%-45s %-12s %s", privName, (privSet & SE_PRIVILEGE_ENABLED ? "Enabled" : "Disabled"), privDisp);

				printf("- %s\n", finalDisplay);
			}

		} else {
			printf("[!] GetTokenInformation (%d) :: Error querying token privileges.\n", GetLastError());
		}

		CloseHandle(hCurrProc);
	} else
		printf("[!] OpenProcessToken (%d) :: Error opening process token for reading.\n", GetLastError());
}