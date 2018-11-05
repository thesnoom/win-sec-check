/*
 * domain.c
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#include <Windows.h>
#include <DsGetDC.h>
#include <LM.h>
#include <stdio.h>

#include "main.h"
#include "domain.h"


unsigned long dwDomainFlags;


// Query all domain trusts, return information if needed (to loop users and groups)
void ListDomainInfo( void )
{
	DWORD dwDcCount = 0;
	DS_DOMAIN_TRUSTS *pDcTrusts = NULL;

	DWORD dwFlags	= (DS_DOMAIN_DIRECT_INBOUND | DS_DOMAIN_PRIMARY | DS_DOMAIN_DIRECT_OUTBOUND | DS_DOMAIN_NATIVE_MODE | DS_DOMAIN_TREE_ROOT);
	DWORD dwRet		= DsEnumerateDomainTrustsA(NULL, dwFlags, &pDcTrusts, &dwDcCount);

	if(dwRet != ERROR_SUCCESS)
	{
		printf("[-] DsEnumerateDomainTrustsA :: Error retrieving DC information. (%d).\n", GetLastError());
		return;
	} else {

		for(DWORD n = 0; n < dwDcCount; n++)
		{
			DOMAIN_CONTROLLER_INFO *pDcInfo;

			DWORD dwDcRet = DsGetDcNameA(NULL, pDcTrusts[n].DnsDomainName, NULL, NULL, 0, &pDcInfo);

			printf("- %-25s %s\n- %-25s %s\n- %-25s %s\n", "Domain Name DNS: ", pDcTrusts[n].DnsDomainName, "Domain DC DNS", pDcInfo->DomainControllerName, "Domain DC IP: ", pDcInfo->DomainControllerAddress);
			for(size_t i = 0; i < 28 + strlen(pDcInfo->DomainControllerAddress); i++, printf("-")); puts("");

			if(!(dwDomainFlags & WSC_DOMAINDC))
			{
				printf("- Listing domain accounts:\n");
				printf("--------------------------\n");

				PNET_DISPLAY_USER pUser = NULL, pU = NULL;
				DWORD dwRec = 0, i = 0;

				TCHAR szDC[255] = TEXT("");
				MultiByteToWideChar(CP_ACP, 0, pDcInfo->DomainControllerName, -1, (LPWSTR)szDC, 255);

				do
				{
					dwRet = NetQueryDisplayInformation((LPCWSTR)szDC, 1, i, 100, MAX_PREFERRED_LENGTH, &dwRec, &pUser);

					if(dwRet == ERROR_SUCCESS || dwRet == ERROR_MORE_DATA)
					{
						pU = pUser;

						for(DWORD n = 0; n < dwRec && pU; n++, i = pU->usri1_next_index, pU++)
						{
							if(pU->usri1_full_name[0] != L'\0')
								printf("- %-30ws (%ws) - %-.40ws...\n", pU->usri1_name, pU->usri1_full_name, pU->usri1_comment);
							else
								printf("- %-30ws - %-.40ws...\n", pU->usri1_name, (pU->usri1_comment[0] != L'\0' ? pU->usri1_comment : L"N/A"));
						}

						NetApiBufferFree(pUser);

					} else
						printf("[-] NetQueryDisplayInformation :: Error retrieving users (%d)", GetLastError());

				} while(dwRet == ERROR_MORE_DATA);

				printf("\n- Listing domain groups:\n");
				printf("------------------------\n");

				PNET_DISPLAY_GROUP pGroup = NULL, pG = 0;
				dwRec = 0, i = 0;
				
				do
				{
					dwRet = NetQueryDisplayInformation((LPCWSTR)szDC, 3, i, 100, MAX_PREFERRED_LENGTH, &dwRec, &pGroup);

					if(dwRet == ERROR_SUCCESS || dwRet == ERROR_MORE_DATA)
					{
						pG = pGroup;

						for(DWORD n = 0; n < dwRec && pG; n++, i = pG->grpi3_next_index, pG++)
							printf("- %-30ws - %-.40ws...\n", pG->grpi3_name, (pG->grpi3_comment[0] != L'\0' ? pG->grpi3_comment : L"N/A"));

						NetApiBufferFree(pGroup);

					} else
						printf("[-] NetQueryDisplayInformation :: Error retrieving groups (%d)", GetLastError());
				} while(dwRet == ERROR_MORE_DATA);

			}
		}

		NetApiBufferFree(pDcTrusts);
	}
}