/*
 * local.c
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */

#pragma comment(lib, "netapi32.lib")


#include <Windows.h>
#include <LM.h>
#include <stdio.h>

#include "local.h"


// Query local host for information regarding Users and Groups.
void LocalInformation(DWORD dwFlag)
{
	DWORD dwRet = 0, dwRec = 0, i = 0;

	if(dwFlag == _LOCAL_USERS) 
	{
		PNET_DISPLAY_USER pUser = NULL, p = NULL;

		do
		{
			dwRet = NetQueryDisplayInformation(L"localhost", dwFlag, i, 100, MAX_PREFERRED_LENGTH, &dwRec, &pUser);

			if(dwRet == ERROR_SUCCESS || dwRet == ERROR_MORE_DATA)
			{
				p = pUser;

				for(DWORD n = 0; n < dwRec && p; n++, i = p->usri1_next_index, p++)
				{
					if(p->usri1_full_name[0] != L'\0')
						printf("- %-30ws (%ws) - %-.40ws...\n", p->usri1_name, p->usri1_full_name, p->usri1_comment);
					else 
						printf("- %-30ws - %-.40ws...\n", p->usri1_name, (p->usri1_comment[0] != L'\0' ? p->usri1_comment : L"N/A"));
				}

				NetApiBufferFree(pUser);

			} else
				printf("[-] NetQueryDisplayInformation :: Error retrieving users (%d)", GetLastError());

		} while(dwRet == ERROR_MORE_DATA);

	} else {
		DWORD dwTotal = 0, *ptrRes = NULL;
		LOCALGROUP_INFO_1 *pGroup = NULL, *p = NULL;

		do
		{
			dwRet = NetLocalGroupEnum(L"localhost", 1, (LPBYTE *)&pGroup, MAX_PREFERRED_LENGTH, &dwRec, &dwTotal, ptrRes);

			if(dwRet == ERROR_SUCCESS || dwRet == ERROR_MORE_DATA)
			{
				p = pGroup;

				for(DWORD n = 0; n < dwRec && p; n++, p++)
					printf("- %-35ws - %-.60ws...\n", p->lgrpi1_name, p->lgrpi1_comment);

			} else
				printf("[-] NetQueryDisplayInformation :: Error retrieving groups (%d)", GetLastError());

			NetApiBufferFree(pGroup);

		} while(dwRet == ERROR_MORE_DATA);
	}
}
