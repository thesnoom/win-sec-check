/*
 * token.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#include <Windows.h>
#include <stdio.h>
#include <AclAPI.h>

#include "descriptors.h"


ACL *GetObjectDacl( HANDLE hObject, SE_OBJECT_TYPE ObjectType, SECURITY_DESCRIPTOR **pDescOut )
{
	if(!hObject)
	{
		printf("[-] PrintObjectDacl :: NULL handle passed?\n");
		return NULL;
	}

	SECURITY_DESCRIPTOR *pSecDesc = NULL;
	ACL *pDacl = NULL;

	DWORD dwRet = GetSecurityInfo(hObject, ObjectType, DACL_SECURITY_INFORMATION, NULL, NULL, &pDacl, NULL, &pSecDesc);

	if(dwRet != ERROR_SUCCESS)
	{
		printf("[-] GetSecurityInfo :: Error retrieving SECURITY_INFORMATION (%d)\n", GetLastError());
		return NULL;
	}

	if(!IsValidAcl(pDacl))
	{
		printf("[-] IsValidAcl :: DACL is invalid? (%d).\n", GetLastError());
		return NULL;
	}

	*pDescOut = pSecDesc;
	return pDacl;
}


void PrintPathDacl( ACL *pDacl )
{
	for(int i = 0; i < pDacl->AceCount; i++)
	{
		ACCESS_ALLOWED_ACE *pAce = NULL;

		if(GetAce(pDacl, i, (PVOID *)&pAce))
		{
			if(pAce->Header.AceType == ACCESS_ALLOWED_ACE_TYPE)
			{
				char szAceUser[64] = { 0 }, szAceDom[256] = { 0 };
				DWORD dwUserLen = 64, dwDomLen = 256;
				SID_NAME_USE pName;

				LookupAccountSidA(NULL, (SID *)&pAce->SidStart, szAceUser, &dwUserLen, szAceDom, &dwDomLen, &pName);

				printf("%s\\%s -", szAceDom, szAceUser);

				if(pAce->Mask & GENERIC_ALL || pAce->Mask & FILE_ALL_ACCESS) 
				{
					printf(" F\n");
					continue;
				}

				if(pAce->Mask & GENERIC_READ || pAce->Mask & FILE_GENERIC_READ)
					printf(" R");

				if(pAce->Mask & GENERIC_WRITE || pAce->Mask & FILE_GENERIC_WRITE)
					printf(" W");

				if(pAce->Mask & GENERIC_EXECUTE || pAce->Mask & FILE_GENERIC_EXECUTE)
					printf(" X");

				puts("");
			}
		}
	}
}