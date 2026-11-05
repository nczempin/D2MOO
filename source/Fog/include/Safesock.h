#pragma once

#pragma warning(push)
#pragma warning(disable:4820 4121)
#include <Winsock2.h>
#pragma warning(pop)

#include <D2BasicTypes.h>
#include <Fog.h>

#pragma pack(1)

struct FSafeSock
{
	BOOL bConnected;
	BOOL bConnecting;
	SOCKET nSocket;
	CRITICAL_SECTION tCriticalSection;
	HANDLE hThread;
	DWORD nThreadId;
	BOOL bStopping;
	DWORD nFogErrorCode;
	DWORD nWSALastError;
	char szName[64];
};

#pragma pack()

FOG_DLL_DECL int __fastcall FOG_SOCKET_CloseSocket(SOCKET nSocket);														//Fog.#10000
FOG_DLL_DECL BOOL __fastcall FOG_SOCKET_WSAStartup();																	//Fog.#10001
FOG_DLL_DECL int __fastcall FOG_SOCKET_WSACleanup();																	//Fog.#10002
FOG_DLL_DECL FSafeSock* __fastcall FOG_SOCKET_Connect(char* szHostAddress, uint16_t nPortNumber, LPTHREAD_START_ROUTINE pThreadProc, LPVOID lpParameter, const char* szName);						//Fog.#10003
FOG_DLL_DECL FSafeSock* __fastcall FOG_SOCKET_ConnectEx(char* szHostAddress, uint16_t nPortNumber, LPTHREAD_START_ROUTINE pThreadProc, LPVOID lpParameter, const char* szName, BOOL bNonBlocking);	//Fog.#10004
FOG_DLL_DECL void __fastcall FOG_SOCKET_Destroy(FSafeSock* pSafeSock);													//Fog.#10005
FOG_DLL_DECL int __fastcall FOG_SOCKET_Receive(FSafeSock* pSafeSock, char* pBuffer, int nBufferLength);					//Fog.#10006
FOG_DLL_DECL int __fastcall FOG_SOCKET_Send(FSafeSock* pSafeSock, char* pBuffer, int nBufferLength);					//Fog.#10007
FOG_DLL_DECL int __fastcall FOG_SOCKET_Select(FSafeSock* pSafeSock);													//Fog.#10008
FOG_DLL_DECL int __fastcall FOG_SOCKET_SendNoLock(FSafeSock* pSafeSock, char* pBuffer, int nBufferLength);				//Fog.#10009
FOG_DLL_DECL void __fastcall FOG_SOCKET_Lock(FSafeSock* pSafeSock);														//Fog.#10010
FOG_DLL_DECL void __fastcall FOG_SOCKET_Unlock(FSafeSock* pSafeSock);													//Fog.#10011
FOG_DLL_DECL BOOL __fastcall FOG_SOCKET_GetPeerName(FSafeSock* pSafeSock, struct sockaddr* name, int* namelen);			//Fog.#10012
FOG_DLL_DECL const char* __fastcall FOG_GetHostIPAddress();																//Fog.#10013
FOG_DLL_DECL const char* __fastcall FOG_inet_ntoa(ULONG inaddr_S_addr);													//Fog.#10014
FOG_DLL_DECL const char* __fastcall FOG_ResolveHostIPAddress(const char* szHostName);									//Fog.#10015
FOG_DLL_DECL BOOL __fastcall FOG_SOCKET_GetError(FSafeSock* pSafeSock, DWORD* pFogError, int* pWSALastError);			//Fog.#10016
FOG_DLL_DECL BOOL __fastcall FOG_SOCKET_SetBlockingMode(FSafeSock* pSafeSock, BOOL bNonBlocking);						//Fog.#10017
