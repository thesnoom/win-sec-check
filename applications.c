/*
 * applications.c
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#include <Windows.h>
#include <stdio.h>
#include <shobjidl.h>

#include "applications.h"


BOOL FileExists(char *szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


BOOL FolderExists(char *szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

// https://stackoverflow.com/questions/35112942/using-ishelllink-bloats-my-executable-to-20kb
// https://docs.microsoft.com/en-us/windows/desktop/shell/links#resolving-a-shortcut
BOOL ResolveLink(char *szPath, char *szFile, wchar_t **wszOut)
{
	GUID IID_IShellLinkW = { 0x000214f9, 0, 0, {0xc0, 0, 0, 0, 0, 0, 0, 0x46} };
	GUID IID_IPersistFile = { 0x0000010b, 0, 0, {0xc0, 0, 0, 0, 0, 0, 0, 0x46} };

	BOOL bRet = FALSE;

	char buff[256] = { 0 };
	char *path = _strdup(szPath);

	if(!path)
		return bRet;

	if(path[strlen(path) - 1] == '*')
		path[strlen(path) - 1] = '\0';

	snprintf(buff, 256, "%s%s", path, szFile);

	free(path);


	HRESULT hRes; 
	IShellLink *ps1;
	IPersistFile *ppf;

	WCHAR szResolvePath[MAX_PATH] = { 0 };

	hRes = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLinkW, (void **)&ps1);

	if(hRes == S_OK)
	{
		hRes = ps1->lpVtbl->QueryInterface(ps1, &IID_IPersistFile, (void **)&ppf);

		if(hRes == S_OK)
		{
			WCHAR wszConvert[MAX_PATH] = { 0 };

			MultiByteToWideChar(CP_ACP, 0, buff, -1, wszConvert, MAX_PATH);

			hRes = ppf->lpVtbl->Load(ppf, wszConvert, STGM_READ);

			if(hRes == S_OK)
			{
				hRes = ps1->lpVtbl->Resolve(ps1, NULL, SLR_NO_UI);

				if(hRes == S_OK)
				{
					hRes = ps1->lpVtbl->GetPath(ps1, szResolvePath, MAX_PATH, NULL, SLGP_RAWPATH);

					if(hRes == S_OK)
					{
						*wszOut = _wcsdup(szResolvePath);

						bRet = TRUE;
					}
				}
			}
		}

		ppf->lpVtbl->Release(ppf);
	}

	ps1->lpVtbl->Release(ps1);

	return bRet;
}

/*
BOOL ResolveLink(char *szPath, char *szFile)
{
	char buff[256] = { 0 };
	char *path = _strdup(szPath); 

	if(!path)
		return FALSE;

	if(path[strlen(path) - 1] == '*')
		path[strlen(path) - 1] = '\0';
		
	snprintf(buff, 256, "%s%s", path, szFile);

	free(path);

	printf("T: %s\n", buff);

	if(!strstr(szFile, ".lnk"))
		return FALSE;


	TCHAR Path[MAX_PATH];

	HANDLE hFile = CreateFile(buff, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	GetFinalPathNameByHandle(hFile, Path, MAX_PATH, VOLUME_NAME_NT);

	printf("Test: %s\n", Path);

	
	HANDLE hFile = CreateFile(buff, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(!hFile)
		return FALSE;

	DWORD dwBytesRead, dwSize = GetFileSize(hFile, NULL);

	if(!dwSize)
		return FALSE;

	BYTE *szData = (BYTE *)malloc(dwSize);

	ReadFile(hFile, szData, dwSize, &dwBytesRead, NULL);
	
	CloseHandle(hFile);

	ZeroMemory(buff, 256);
	DWORD i;

	for(DWORD n = 0; n < dwSize; n++)
	{
		if(szData[n] != '\0')
			buff[i++] = szData[n];
		else {
			if(buff[0] == 0xA0)
				printf("PATH: %s\n", buff);

			i = 0;
			ZeroMemory(buff, 256);
		}
	}

	return TRUE;
}*/


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