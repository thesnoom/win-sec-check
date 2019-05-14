/*
 * services.c
 *
 * WinSecCheck.
 *
 * - thesnoom 2019
 * TODO 
 *  - Fix the TODO's
 *  - Make sure we are free() on all maloc(). At the moment we dont on err()
 *  - Loaded .sys Drivers are enumerated this way as well, do we want to check for version no's?
 *  - Check ACL's assoc with the Service + Exe /
 *  - Also vars that look like files? I.E C:\WINDOWS\System32\DriverStore\FileRepository\nvaci.inf_amd64_00289c834e61b094\Display.NvContainer\NVDisplay.Container.exe -s NVDisplay.ContainerLocalSystem -f C:\ProgramData\NVDisplay.ContainerLocalSystem.log -l 3 -d C:\WINDOWS\System32\DriverStore\FileRepository\nvaci.inf_amd64_00289c834e61b094\Display.NvContainer\plugins\LocalSystem -r -p 30000 -cfg NVDisplay.ContainerLocalSystem\LocalSystem
 *  - Check more than just the loaded services!
 */


#include <Windows.h>
#include <stdio.h>
#include <winsvc.h>

#include "descriptors.h"
#include "services.h"

void GetServices(void) {
	HANDLE SvcmgrHandle;
	HANDLE SvcHandle;
	BOOL returnValue;
	UINT32 sizeServices = 0, hEnumHandle = 0, moreBytes = 0, sCount = 0;
	void* SvcOptConfBytes = NULL; 
	void* SvcConfBytes = NULL;
	void* DACLBytes = NULL;
	void* pServiceList = NULL;
	int err;
	char* ptr;

	// Open the default service Database
	// As we can say "Which" Does that mean windows has antther service store?
	SvcmgrHandle = OpenSCManagerA(NULL, NULL, GENERIC_READ);

	// First call tells us how much size is needed for the Buffer Holding the Service Array, Will allways Err
	// Should be checking the BOOL value returned by EnumServicesStatusEx
	EnumServicesStatusEx(
		SvcmgrHandle, 	SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_ACTIVE,
		(LPBYTE)pServiceList, sizeServices, (LPDWORD)& moreBytes,
		(LPDWORD)& sCount, (LPDWORD)& hEnumHandle,NULL);


	// Functionise this
	err = GetLastError();
	if (ERROR_MORE_DATA != err) {
		// Catch other Error other than no memory
		free(pServiceList);
		perror("Error in getting services ");
		exit(0);
	}

	sizeServices = moreBytes;
	free(pServiceList);
	pServiceList = malloc(sizeServices);
	
	EnumServicesStatusEx(
		SvcmgrHandle, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_ACTIVE,
		(LPBYTE)pServiceList, sizeServices, (LPDWORD)& moreBytes,
		(LPDWORD)& sCount, (LPDWORD)& hEnumHandle, NULL);

	ENUM_SERVICE_STATUS_PROCESS* services = (ENUM_SERVICE_STATUS_PROCESS*)pServiceList;
	// Is this a function?
	printf("[+] Listing Active Services \n");
	printf("------------------------------------------------------------------------------- \n");
	for (DWORD i = 0; i < sCount; ++i) {
		SvcHandle = OpenServiceA(SvcmgrHandle, services[i].lpServiceName, SERVICE_QUERY_CONFIG);
		//printf("Opening %s\n", services[i].lpServiceName);

		// Core Config, Maybe Abstract to a function?
		// Causing a Fail and then doing the alloc seems stupid?
		QueryServiceConfigA(SvcHandle, SvcConfBytes, 0, (LPDWORD)& moreBytes);

		// TODO : This should be a function not C&P
		err = GetLastError();
		sizeServices = moreBytes;
		free(SvcConfBytes);
		SvcConfBytes = malloc(sizeServices);

		QueryServiceConfigA(SvcHandle, SvcConfBytes, sizeServices, (LPDWORD)& moreBytes);
		QUERY_SERVICE_CONFIGA* SvcConf = (QUERY_SERVICE_CONFIGA*)SvcConfBytes;

		// Optional Config : Maybe Abstract to a function? Its Stupid how it works! This Really needs a function
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_DESCRIPTION, SvcOptConfBytes, 0, (LPDWORD)& moreBytes);
		sizeServices = moreBytes;
		free(SvcOptConfBytes);
		SvcOptConfBytes = malloc(sizeServices);
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_DESCRIPTION, SvcOptConfBytes, sizeServices, (LPDWORD)& moreBytes);
		SERVICE_DESCRIPTION* SvcConfDesc = (SERVICE_DESCRIPTION*)SvcOptConfBytes;
		SvcOptConfBytes = NULL;

		// What Happens on service Faliure : Some services can be forced to fail which calls a Vuln Program
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_FAILURE_ACTIONS, SvcOptConfBytes, 0, (LPDWORD)& moreBytes);
		sizeServices = moreBytes;
		free(SvcOptConfBytes);
		SvcOptConfBytes = malloc(sizeServices);
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_FAILURE_ACTIONS, SvcOptConfBytes, sizeServices, (LPDWORD)& moreBytes);
		SERVICE_FAILURE_ACTIONS* SvcConfFail = (SERVICE_FAILURE_ACTIONS*)SvcOptConfBytes;
		SvcOptConfBytes = NULL;
		// What Privs do it have 
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_REQUIRED_PRIVILEGES_INFO, SvcOptConfBytes, 0, (LPDWORD)& moreBytes);
		sizeServices = moreBytes;
		free(SvcOptConfBytes);
		SvcOptConfBytes = malloc(sizeServices);
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_REQUIRED_PRIVILEGES_INFO, SvcOptConfBytes, sizeServices, (LPDWORD)& moreBytes);
		SERVICE_REQUIRED_PRIVILEGES_INFO* SvcConfPriv = (SERVICE_REQUIRED_PRIVILEGES_INFO*)SvcOptConfBytes;
		SvcOptConfBytes = NULL;
		// Whos it running as 
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_SERVICE_SID_INFO, SvcOptConfBytes, 0, (LPDWORD)& moreBytes);
		sizeServices = moreBytes;
		free(SvcOptConfBytes);
		SvcOptConfBytes = malloc(sizeServices);
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_SERVICE_SID_INFO, SvcOptConfBytes, sizeServices, (LPDWORD)& moreBytes);
		SERVICE_SID_INFO* SvcConfSids = (SERVICE_SID_INFO*)SvcOptConfBytes;
		SvcOptConfBytes = NULL;
		// What starts it
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_TRIGGER_INFO, SvcOptConfBytes, 0, (LPDWORD)& moreBytes);
		sizeServices = moreBytes;
		free(SvcOptConfBytes);
		SvcOptConfBytes = malloc(sizeServices);
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_TRIGGER_INFO, SvcOptConfBytes, sizeServices, (LPDWORD)& moreBytes);
		SERVICE_TRIGGER_INFO* SvcConfTrig = (SERVICE_TRIGGER_INFO*)SvcOptConfBytes;
		SvcOptConfBytes = NULL;
		/*
		// This is 8.1> , Need to Try Catch?
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_LAUNCH_PROTECTED, SvcOptConfBytes, 0, (LPDWORD)& moreBytes);
		sizeServices = moreBytes;
		free(SvcOptConfBytes);
		SvcOptConfBytes = malloc(sizeServices);
		QueryServiceConfig2A(SvcHandle, SERVICE_CONFIG_LAUNCH_PROTECTED, SvcOptConfBytes, sizeServices, (LPDWORD)& moreBytes);
		SERVICE_DESCRIPTION* SvcConfDesc = (SERVICE_DESCRIPTION*)SvcOptConfBytes;
		*/

		printf("\t[-] Service : %s \n \t [>] BinPath : %s \n", SvcConf->lpDisplayName, SvcConf->lpBinaryPathName);

		ptr = strtok(SvcConf->lpBinaryPathName, " ");
		while (ptr != NULL)
		{
			if (ptr[1] == ':' ){
				if (ptr[2] == '\\'){
					printf("\t [>]Perms for file : %s \n ",ptr);
					HANDLE hPathDir = CreateFileA(ptr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
					SECURITY_DESCRIPTOR* pDesc;
					ACL* pDacl = GetObjectDacl(hPathDir, SE_FILE_OBJECT, &pDesc);
					BOOL bIsPresent = FALSE;
					GetSecurityDescriptorDacl(&pDesc, &bIsPresent, &pDacl, NULL);
					if (bIsPresent && pDacl == NULL)
						printf("NULL dacl! Everyone - F...");
					else
						// Seems to be printing both F(ull) and R W for directories?
						// Rather than printing Everythin (Meh) Highlight E+F / User + F ?
						PrintDacl(pDacl);
					LocalFree(pDesc);
					CloseHandle(hPathDir);
				}
			}
			ptr = strtok(NULL, " ");
		}

		if (SvcConfDesc->lpDescription) {
			printf("\t [>] Description :  %s \n", SvcConfDesc->lpDescription);
		} else {
			printf("\t [!] This service does not have a Desc, XXX Susp \n ");
			//getch();
		}
		if (SvcConf->lpLoadOrderGroup[0] != 0x00) {
			printf("\t [>] LoadOrder : %s \n", SvcConf->lpLoadOrderGroup);
		}
		if (SvcConf->lpDependencies[0] != 0x00) {
			printf("\t [>] Depends :  %s \n", SvcConf->lpDependencies);
		}
		printf("\t [>] Startup : %d \n", SvcConf->dwStartType);
		if (SvcConf->dwErrorControl) {
			printf("\t [>] ErrCtl : %d \n", SvcConf->dwErrorControl);
		}
		printf("\t [>] Type :  %d \n", SvcConf->dwServiceType);
		if (SvcConfFail->lpRebootMsg) {
			printf("\t [>] Fail Msg :  %s \n", SvcConfFail->lpRebootMsg);
		}
		if (SvcConfFail->lpCommand) {
			printf("\t [>] Fail Cmd :  %s \n", SvcConfFail->lpCommand);
		}
		if (SvcConf->lpServiceStartName[0] != 0x00) {
			printf("\t [>] User :  %s \n", SvcConf->lpServiceStartName);
		}
		if (SvcConfPriv->pmszRequiredPrivileges) {
			printf("\t [>] Privs :  %s \n", SvcConfPriv->pmszRequiredPrivileges);
		}
		if (SvcConfSids->dwServiceSidType) {
			printf("\t [>] Type :  %d \n", SvcConfSids->dwServiceSidType);
		}
		// printf("\t [>] Fail Act :  %d \n", SvcConfFail->lpsaActions); // SC_ACTION Struct?
		// printf("\t [>] Type :  %d \n", SvcConfTrig->); SERVICE_TRIGGER  Struct   */
		printf("------------------------------------------------------------------------------- \n");
		// Clear the handle
		SvcHandle = NULL;
	}

}
	