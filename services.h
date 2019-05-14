/*
 * services.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#ifndef __WSC_SVCS_H
#define __WSC_SVCS_H

void GetServices(void);

/* Structs Used but provided by windows for reference

typedef struct _ENUM_SERVICE_STATUS_PROCESSA {
	LPSTR                  lpServiceName;
	LPSTR                  lpDisplayName;
	SERVICE_STATUS_PROCESS ServiceStatusProcess;
} ENUM_SERVICE_STATUS_PROCESSA, * LPENUM_SERVICE_STATUS_PROCESSA;

typedef struct _QUERY_SERVICE_CONFIGA {
  DWORD dwServiceType;
  DWORD dwStartType;
  DWORD dwErrorControl;
  LPSTR lpBinaryPathName;
  LPSTR lpLoadOrderGroup;
  DWORD dwTagId;
  LPSTR lpDependencies;
  LPSTR lpServiceStartName;
  LPSTR lpDisplayName;
} QUERY_SERVICE_CONFIGA, *LPQUERY_SERVICE_CONFIGA;
*/

#endif