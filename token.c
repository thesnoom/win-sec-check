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


// Grab username from token. Cred: Incognito
void UserFromToken( void )
{
	HANDLE hCurrProc;
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &hCurrProc))
	{
		void *tokenUser[1024] = { 0 };
		char szUser[64] = { 0 }, szDomain[256] = { 0 };
		DWORD dwUserLen, dwDomLen, dwTokeLen, dwSidType = 0;

		if(GetTokenInformation(hCurrProc, TokenUser, tokenUser, 1024, &dwTokeLen))
		{
			LookupAccountSidA(NULL, ((TOKEN_USER *)tokenUser)->User.Sid, szUser, &dwUserLen, szDomain, &dwDomLen, (PSID_NAME_USE)&dwSidType);

			printf("- %-16s %s\\\\%s\n", "Token User:", szDomain, szUser);

			for(size_t i = 0; i < (21 + strlen(szDomain) + strlen(szUser)); i++) 
				printf("-");
			printf("\n");

		} else
			printf("[!] GetTokenInformation (%d) :: Error querying token user.\n", GetLastError());

		CloseHandle(hCurrProc);
	} else 
		printf("[!] OpenProcessToken (%d) :: Error opening process token for reading username.\n", GetLastError());
}


// Iterate through all groups attached to the user token. Cred: Incognito
void ListTokenUserGroups( void )
{
	HANDLE hCurrProc;
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &hCurrProc))
	{
		void *tokenGroups[1024] = { 0 };
		char szGroup[64] = { 0 }, szDomain[256] = { 0 }, finalDisplay[384] = { 0 };
		DWORD dwGroupLen, dwDomLen, dwTokeLen, dwSidType = 0, i;

		if(GetTokenInformation(hCurrProc, TokenGroups, tokenGroups, 1024, &dwTokeLen))
		{
			for(i = 0; i < ((TOKEN_GROUPS *)tokenGroups)->GroupCount; i++)
			{
				if((((TOKEN_GROUPS *)tokenGroups)->Groups[i].Attributes & SE_GROUP_ENABLED) != 0)
				{
					dwGroupLen = 64, dwDomLen = 256;

					LookupAccountSidA(NULL, ((TOKEN_GROUPS *)tokenGroups)->Groups[i].Sid, szGroup, &dwGroupLen, szDomain, &dwDomLen, (PSID_NAME_USE)&dwSidType);

					printf("- %-16s %s\\\\%s\n", "Token Group:", szDomain, szGroup);
				}
			}
		} else
			printf("[!] GetTokenInformation (%d) :: Error querying token user.\n", GetLastError());

		CloseHandle(hCurrProc);
	} else
		printf("[!] OpenProcessToken (%d) :: Error opening process token for reading username.\n", GetLastError());
}

// Iterate through all tokens in current process. Cred: Incognito
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

				printf("- %-45s %-12s %s\n", privName, (privSet & SE_PRIVILEGE_ENABLED ? "Enabled" : "Disabled"), privDisp);
			}

		} else 
			printf("[!] GetTokenInformation (%d) :: Error querying token privileges.\n", GetLastError());

		CloseHandle(hCurrProc);
	} else
		printf("[!] OpenProcessToken (%d) :: Error opening process token for reading privileges.\n", GetLastError());
}