/*
 * applications.c
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#include <Windows.h>
#include <stdio.h>

#include "applications.h"


BOOL FileExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


BOOL FolderExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


void ListInstalledApps( int b32BitApps )
{
	HKEY hkApps;

	TCHAR *pAppPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

#if _WIN64
	DWORD flagVal = (b32BitApps ? KEY_WOW64_32KEY : KEY_WOW64_64KEY);

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, pAppPath, 0, (KEY_QUERY_VALUE | flagVal | KEY_ENUMERATE_SUB_KEYS), &hkApps) == ERROR_SUCCESS)
	{
		DWORD dwKeys;
		RegQueryInfoKey(hkApps, NULL, NULL, NULL, &dwKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

		if(dwKeys > 0)
		{
			for(DWORD dwIndex = 0; dwIndex < dwKeys; dwIndex++)
			{
				TCHAR lpName[256] = { 0 }, lpFullPath[1024] = { 0 };
				BYTE pbAppName[1024] = { 0 };
				DWORD dwBuffLen = 256, dwAppLen = 1024;

				RegEnumKeyEx(hkApps, dwIndex, lpName, &dwBuffLen, NULL, NULL, NULL, NULL);


				snprintf(lpFullPath, 1024, "%s\\%s", pAppPath, lpName);


				HKEY hAppName;
				if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpFullPath, 0, (KEY_QUERY_VALUE | flagVal), &hAppName) == ERROR_SUCCESS)
				{
					if(RegQueryValueEx(hAppName, TEXT("DisplayName"), 0, NULL, (BYTE *)pbAppName, &dwAppLen) == ERROR_SUCCESS)
						printf("- %s\n", pbAppName);

					RegCloseKey(hAppName);
				}
			}
		}

		RegCloseKey(hkApps);
	}
#else
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, pAppPath, 0, (KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS), &hkApps) == ERROR_SUCCESS)
	{
		DWORD dwKeys;
		RegQueryInfoKey(hkApps, NULL, NULL, NULL, &dwKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

		if(dwKeys > 0)
		{
			for(DWORD dwIndex = 0; dwIndex < dwKeys; dwIndex++)
			{
				TCHAR lpName[256] = { 0 }, lpFullPath[1024] = { 0 };
				BYTE pbAppName[1024] = { 0 };
				DWORD dwBuffLen = 256, dwAppLen = 1024;

				RegEnumKeyEx(hkApps, dwIndex, lpName, &dwBuffLen, NULL, NULL, NULL, NULL);


				snprintf(lpFullPath, 1024, "%s\\%s", pAppPath, lpName);


				HKEY hAppName;
				if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpFullPath, 0, KEY_QUERY_VALUE, &hAppName) == ERROR_SUCCESS)
				{
					if(RegQueryValueEx(hAppName, TEXT("DisplayName"), 0, NULL, (BYTE *)pbAppName, &dwAppLen) == ERROR_SUCCESS)
						printf("- %s\n", pbAppName);

					RegCloseKey(hAppName);
				}
			}
		}

		RegCloseKey(hkApps);
	}
#endif
}