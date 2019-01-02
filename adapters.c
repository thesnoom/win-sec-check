/*
 * adapters.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <iphlpapi.h>

#include "adapters.h"


// Loop all local adapters for IPv4, DNS entries and Gateways.
void DisplayLocalAdapters( void )
{
	DWORD dwSize = 15000, dwRet = 0, dwFlags = (GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS);

	IP_ADAPTER_ADDRESSES *pAddr = NULL, *pCurrAddr = NULL;
	IP_ADAPTER_UNICAST_ADDRESS *pUniAddr = NULL;
	IP_ADAPTER_DNS_SERVER_ADDRESS *pDnsAddr = NULL;
	IP_ADAPTER_GATEWAY_ADDRESS *pGwAddr = NULL;
	IP_ADAPTER_PREFIX *pPrefix = NULL;

	do 
	{
		pAddr = (IP_ADAPTER_ADDRESSES *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
		if(!pAddr)
		{
			printf("[!] HeapAlloc (%d) :: Error allocating adapter information.\n", GetLastError());
			return;
		}

		dwRet = GetAdaptersAddresses(AF_INET, dwFlags, NULL, pAddr, &dwSize);

	} while(dwRet == ERROR_BUFFER_OVERFLOW);

	if(dwRet == NO_ERROR)
	{
		pCurrAddr = pAddr;

		while(pCurrAddr)
		{			
			pUniAddr = pCurrAddr->FirstUnicastAddress;
			if(pUniAddr)
			{
				while(pUniAddr)
				{
					if(pUniAddr->Address.lpSockaddr->sa_family == AF_INET)
					{
						char ipv4addr[16] = { 0 };

						struct sockaddr_in *sa_addr = (struct sockaddr_in *)pUniAddr->Address.lpSockaddr;
						inet_ntop(AF_INET, &(sa_addr->sin_addr), ipv4addr, 16);

						printf("- Address:   %-16s | %ws\n", ipv4addr, pCurrAddr->FriendlyName);

						for(size_t i = 0; i < (32 + wcslen(pCurrAddr->FriendlyName)); printf("-"), i++); puts("");

					}

					pUniAddr = pUniAddr->Next;
				}
			}

			pDnsAddr = pCurrAddr->FirstDnsServerAddress;
			if(pDnsAddr)
			{
				while(pDnsAddr)
				{
					if(pDnsAddr->Address.lpSockaddr->sa_family == AF_INET)
					{
						char ipv4addr[16] = { 0 };

						struct sockaddr_in *sa_addr = (struct sockaddr_in *)pDnsAddr->Address.lpSockaddr;
						inet_ntop(AF_INET, &(sa_addr->sin_addr), ipv4addr, 16);

						printf("- DNS:       %-16s\n", ipv4addr);
					}

					pDnsAddr = pDnsAddr->Next;
				}
			}

			pGwAddr = pCurrAddr->FirstGatewayAddress;
			if(pGwAddr)
			{
				size_t nlen = 0; 

				while(pGwAddr)
				{
					if(pGwAddr->Address.lpSockaddr->sa_family == AF_INET)
					{
						char ipv4addr[16] = { 0 };

						struct sockaddr_in *sa_addr = (struct sockaddr_in *)pGwAddr->Address.lpSockaddr;
						inet_ntop(AF_INET, &(sa_addr->sin_addr), ipv4addr, 16);

						printf("- Gateway:   %-16s\n", ipv4addr);
						nlen = (strlen(ipv4addr) + 13);
					}

					pGwAddr = pGwAddr->Next;
				}

				for(size_t i = 0; i < nlen; printf("-"), i++); puts("");
			}

			pCurrAddr = pCurrAddr->Next;
		}
	}

	HeapFree(GetProcessHeap(), 0, pAddr);
}