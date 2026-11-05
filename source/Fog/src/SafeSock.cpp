#include "Safesock.h"

#include <cstdio>
#include <cstring>
#include <Storm.h>

static char gszInetNtoaBuffer[36] = {};
static char gszHostIPBuffer[36] = {};

//1.10f: Fog.dll 0x6FF63280 (#10000)
//1.12a: Fog.dll 0x6FF5D090 (#10000)
int __fastcall FOG_SOCKET_CloseSocket(SOCKET nSocket)
{
	return closesocket(nSocket);
}

//1.10f: Fog.dll 0x6FF63290 (#10001)
//1.12a: Fog.dll 0x6FF5D070 (#10001)
BOOL __fastcall FOG_SOCKET_WSAStartup()
{
	WSADATA wsaData{};
	return WSAStartup(MAKEWORD(1, 1), &wsaData) == 0;
}

//1.10f: Fog.dll 0x6FF632C0 (#10002)
//1.12a: Fog.dll 0x6FF5D060 (#10002)
int __fastcall FOG_SOCKET_WSACleanup()
{
	return WSACleanup();
}

//1.10f: Fog.dll 0x6FF632D0 (#10003)
//1.12a: Fog.dll 0x6FF5D540 (#10003)
FSafeSock* __fastcall FOG_SOCKET_Connect(char* szHostAddress, uint16_t nPortNumber, LPTHREAD_START_ROUTINE pThreadProc, LPVOID lpParameter, const char* szName)
{
	return FOG_SOCKET_ConnectEx(szHostAddress, nPortNumber, pThreadProc, lpParameter, szName, FALSE);
}

//1.10f: Fog.dll 0x6FF632F0 (#10004)
//1.12a: Fog.dll 0x6FF5D280 (#10004)
FSafeSock* __fastcall FOG_SOCKET_ConnectEx(char* szHostAddress, uint16_t nPortNumber, LPTHREAD_START_ROUTINE pThreadProc, LPVOID lpParameter, const char* szName, BOOL bNonBlocking)
{
	FSafeSock* pSafeSock = D2_CALLOC_STRC(FSafeSock);
	if (!pSafeSock)
	{
		return nullptr;
	}

	SStrCopy(pSafeSock->szName, szName ? szName : "UNNAMED", sizeof(pSafeSock->szName));
	InitializeCriticalSection(&pSafeSock->tCriticalSection);

	WSADATA wsaData{};
	const int nWsaInitErr = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (nWsaInitErr != 0)
	{
		pSafeSock->nFogErrorCode = 1;
		pSafeSock->nWSALastError = static_cast<DWORD>(nWsaInitErr);
		DeleteCriticalSection(&pSafeSock->tCriticalSection);
		D2_FREE(pSafeSock);
		return nullptr;
	}

	const SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	pSafeSock->nSocket = s;
	if (s == INVALID_SOCKET)
	{
		pSafeSock->nWSALastError = static_cast<DWORD>(WSAGetLastError());
		pSafeSock->nFogErrorCode = 2;
		DeleteCriticalSection(&pSafeSock->tCriticalSection);
		D2_FREE(pSafeSock);
		return nullptr;
	}

	BOOL bOptOne = TRUE;
	setsockopt(s, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&bOptOne), sizeof(bOptOne));
	setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, reinterpret_cast<const char*>(&bOptOne), sizeof(bOptOne));

	int nSndTimeo = 3000;
	setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&nSndTimeo), sizeof(nSndTimeo));

	const BOOL bAsync = (bNonBlocking & 1) != 0;
	if (bAsync)
	{
		u_long ulNonBlock = 1;
		ioctlsocket(s, FIONBIO, &ulNonBlock);
	}

	pSafeSock->bConnecting = TRUE;

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPortNumber);
	addr.sin_addr.s_addr = inet_addr(szHostAddress);

	if (connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
	{
		const int nWsaErr = WSAGetLastError();
		if (!bAsync || nWsaErr != WSAEWOULDBLOCK)
		{
			pSafeSock->nWSALastError = static_cast<DWORD>(nWsaErr);
			pSafeSock->nFogErrorCode = 3;
			DeleteCriticalSection(&pSafeSock->tCriticalSection);
			D2_FREE(pSafeSock);
			return nullptr;
		}
	}

	if (!bAsync)
	{
		pSafeSock->bConnected = TRUE;
		pSafeSock->bConnecting = FALSE;
	}

	if (gszHostIPBuffer[0] == '\0' || inet_addr(gszHostIPBuffer) == 0)
	{
		sockaddr_in localAddr{};
		int nAddrLen = sizeof(localAddr);
		if (getsockname(s, reinterpret_cast<sockaddr*>(&localAddr), &nAddrLen) != SOCKET_ERROR)
		{
			const char* szIp = ::inet_ntoa(localAddr.sin_addr);
			if (szIp)
			{
				SStrCopy(gszHostIPBuffer, szIp, sizeof(gszHostIPBuffer));
			}
		}
	}

	if (pThreadProc)
	{
		pSafeSock->hThread = CreateThread(nullptr, 0, pThreadProc, lpParameter, 0, &pSafeSock->nThreadId);
	}

	return pSafeSock;
}

//1.10f: Fog.dll 0x6FF63590 (#10005)
//1.12a: Fog.dll 0x6FF5D790 (#10005)
void __fastcall FOG_SOCKET_Destroy(FSafeSock* pSafeSock)
{
	if (!pSafeSock)
	{
		return;
	}

	D2_LOCK(&pSafeSock->tCriticalSection);
	pSafeSock->bStopping = TRUE;
	D2_UNLOCK(&pSafeSock->tCriticalSection);

	if (pSafeSock->hThread != nullptr)
	{
		WaitForSingleObject(pSafeSock->hThread, 5000);
	}

	D2_LOCK(&pSafeSock->tCriticalSection);
	if (pSafeSock->hThread != nullptr)
	{
		CloseHandle(pSafeSock->hThread);
		pSafeSock->hThread = nullptr;
	}

	const SOCKET s = pSafeSock->nSocket;
	pSafeSock->nSocket = 0;
	pSafeSock->bConnected = FALSE;

	if (closesocket(s) == SOCKET_ERROR)
	{
		pSafeSock->nWSALastError = static_cast<DWORD>(WSAGetLastError());
		pSafeSock->nFogErrorCode = 5;
	}

	D2_UNLOCK(&pSafeSock->tCriticalSection);
	DeleteCriticalSection(&pSafeSock->tCriticalSection);
	D2_FREE(pSafeSock);
}

//1.10f: Fog.dll 0x6FF63630 (#10006)
//1.12a: Fog.dll 0x6FF5D720 (#10006)
int __fastcall FOG_SOCKET_Receive(FSafeSock* pSafeSock, char* pBuffer, int nBufferLength)
{
	if (!pSafeSock || pSafeSock->bStopping)
	{
		return 0;
	}

	D2_LOCK(&pSafeSock->tCriticalSection);
	const int nRead = recv(pSafeSock->nSocket, pBuffer, nBufferLength, 0);
	D2_UNLOCK(&pSafeSock->tCriticalSection);

	if (nRead <= 0)
	{
		pSafeSock->nWSALastError = static_cast<DWORD>(WSAGetLastError());
		pSafeSock->nFogErrorCode = 4;
	}
	return nRead;
}

//1.10f: Fog.dll 0x6FF63690 (#10007)
//1.12a: Fog.dll 0x6FF5D670 (#10007)
int __fastcall FOG_SOCKET_Send(FSafeSock* pSafeSock, char* pBuffer, int nBufferLength)
{
	if (!pSafeSock || pSafeSock->bStopping)
	{
		return 0;
	}

	D2_LOCK(&pSafeSock->tCriticalSection);
	const int nSent = send(pSafeSock->nSocket, pBuffer, nBufferLength, 0);
	if (nSent == nBufferLength)
	{
		D2_UNLOCK(&pSafeSock->tCriticalSection);
		return nSent;
	}

	const int nWsaErr = WSAGetLastError();
	pSafeSock->nWSALastError = static_cast<DWORD>(nWsaErr);
	if (nSent == SOCKET_ERROR)
	{
		if (nWsaErr != WSAEWOULDBLOCK)
		{
			pSafeSock->nFogErrorCode = 4;
			D2_UNLOCK(&pSafeSock->tCriticalSection);
			return -1;
		}
	}
	else
	{
		pSafeSock->nFogErrorCode = 4;
	}

	D2_UNLOCK(&pSafeSock->tCriticalSection);
	return 0;
}

//1.10f: Fog.dll 0x6FF63770 (#10008)
//1.12a: Fog.dll 0x6FF5CF70 (#10008)
int __fastcall FOG_SOCKET_Select(FSafeSock* pSafeSock)
{
	if (!pSafeSock || pSafeSock->bStopping || !pSafeSock->bConnected)
	{
		return 0;
	}

	fd_set readSet{};
	fd_set exceptSet{};
	FD_SET(pSafeSock->nSocket, &readSet);
	FD_SET(pSafeSock->nSocket, &exceptSet);

	timeval tv{};
	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	const int nRes = select(0, &readSet, nullptr, &exceptSet, &tv);
	if (pSafeSock->bStopping || !pSafeSock->bConnected || nRes == 0)
	{
		return 0;
	}

	if (nRes == SOCKET_ERROR)
	{
		pSafeSock->nWSALastError = static_cast<DWORD>(WSAGetLastError());
		pSafeSock->nFogErrorCode = 4;
		return 1;
	}

	if (FD_ISSET(pSafeSock->nSocket, &exceptSet))
	{
		return 0;
	}

	return FD_ISSET(pSafeSock->nSocket, &readSet) ? 1 : 0;
}

//1.10f: Fog.dll 0x6FF63700 (#10009)
//1.12a: Fog.dll 0x6FF5D560 (#10009)
int __fastcall FOG_SOCKET_SendNoLock(FSafeSock* pSafeSock, char* pBuffer, int nBufferLength)
{
	int nSent = send(pSafeSock->nSocket, pBuffer, nBufferLength, 0);
	if (WSAGetLastError() == WSAETIMEDOUT)
	{
		FOG_Trace("Socket send timed out");
		nSent = 0;
		closesocket(pSafeSock->nSocket);
	}
	return nSent;
}

//1.10f: Fog.dll 0x6FF63750 (#10010)
//1.12a: Fog.dll 0x6FF5D650 (#10010)
void __fastcall FOG_SOCKET_Lock(FSafeSock* pSafeSock)
{
	D2_LOCK(&pSafeSock->tCriticalSection);
}

//1.10f: Fog.dll 0x6FF63760 (#10011)
//1.12a: Fog.dll 0x6FF5D050 (#10011)
void __fastcall FOG_SOCKET_Unlock(FSafeSock* pSafeSock)
{
	LeaveCriticalSection(&pSafeSock->tCriticalSection);
}

//1.10f: Fog.dll 0x6FF63860 (#10012)
//1.12a: Fog.dll 0x6FF5D600 (#10012)
BOOL __fastcall FOG_SOCKET_GetPeerName(FSafeSock* pSafeSock, struct sockaddr* name, int* namelen)
{
	D2_LOCK(&pSafeSock->tCriticalSection);
	const int nRes = getpeername(pSafeSock->nSocket, name, namelen);
	D2_UNLOCK(&pSafeSock->tCriticalSection);
	return nRes != SOCKET_ERROR;
}

//1.10f: Fog.dll 0x6FF638E0 (#10013)
//1.12a: Fog.dll 0x6FF5D180 (#10013)
const char* __fastcall FOG_GetHostIPAddress()
{
	WSADATA wsaData{};
	WSAStartup(MAKEWORD(1, 1), &wsaData);

	uint32_t dwAddr = 0;
	char szHostName[256] = {};
	if (gethostname(szHostName, sizeof(szHostName)) != SOCKET_ERROR)
	{
		hostent* pHost = gethostbyname(szHostName);
		if (pHost && pHost->h_addr_list && pHost->h_addr_list[0])
		{
			dwAddr = *reinterpret_cast<uint32_t*>(pHost->h_addr_list[0]);
		}
	}

	if (dwAddr == 0)
	{
		const SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
		if (s != INVALID_SOCKET)
		{
			sockaddr_in remoteAddr{};
			remoteAddr.sin_family = AF_INET;
			remoteAddr.sin_port = htons(7);
			remoteAddr.sin_addr.s_addr = inet_addr("128.0.0.1");

			sockaddr_in localAddr{};
			int nAddrLen = sizeof(localAddr);
			if (connect(s, reinterpret_cast<sockaddr*>(&remoteAddr), sizeof(remoteAddr)) != SOCKET_ERROR
				&& getsockname(s, reinterpret_cast<sockaddr*>(&localAddr), &nAddrLen) != SOCKET_ERROR)
			{
				dwAddr = localAddr.sin_addr.s_addr;
			}
			closesocket(s);
		}
	}

	const uint8_t* pBytes = reinterpret_cast<const uint8_t*>(&dwAddr);
	std::sprintf(gszHostIPBuffer, "%u.%u.%u.%u", pBytes[0], pBytes[1], pBytes[2], pBytes[3]);
	WSACleanup();
	return gszHostIPBuffer;
}

//1.10f: Fog.dll 0x6FF63A00 (#10014)
//1.12a: Fog.dll 0x6FF5CF50 (#10014)
const char* __fastcall FOG_inet_ntoa(ULONG inaddr_S_addr)
{
	in_addr addr{};
	addr.S_un.S_addr = inaddr_S_addr;
	const char* szIp = ::inet_ntoa(addr);
	if (szIp)
	{
		std::strcpy(gszInetNtoaBuffer, szIp);
	}
	else
	{
		gszInetNtoaBuffer[0] = '\0';
	}
	return gszInetNtoaBuffer;
}

//1.10f: Fog.dll 0x6FF63A40 (#10015)
//1.12a: Fog.dll 0x6FF5D0A0 (#10015)
const char* __fastcall FOG_ResolveHostIPAddress(const char* szHostName)
{
	if (!szHostName)
	{
		return nullptr;
	}

	unsigned int a = 0, b = 0, c = 0, d = 0;
	if (std::sscanf(szHostName, "%u.%u.%u.%u", &a, &b, &c, &d) == 4)
	{
		return szHostName;
	}

	BOOL bStartedWsa = FALSE;
	hostent* pHost = gethostbyname(szHostName);
	if (!pHost && WSAGetLastError() == WSANOTINITIALISED)
	{
		WSADATA wsaData{};
		if (WSAStartup(MAKEWORD(1, 1), &wsaData) == 0)
		{
			bStartedWsa = TRUE;
			pHost = gethostbyname(szHostName);
		}
	}

	if (!pHost || !pHost->h_addr_list || !pHost->h_addr_list[0])
	{
		if (bStartedWsa)
		{
			WSACleanup();
		}
		return nullptr;
	}

	const in_addr addr = *reinterpret_cast<in_addr*>(pHost->h_addr_list[0]);
	const char* szIp = ::inet_ntoa(addr);
	if (szIp)
	{
		std::strcpy(gszHostIPBuffer, szIp);
	}
	else
	{
		gszHostIPBuffer[0] = '\0';
	}

	if (bStartedWsa)
	{
		WSACleanup();
	}
	return gszHostIPBuffer;
}

//1.10f: Fog.dll 0x6FF63A20 (#10016)
//1.12a: Fog.dll 0x6FF5CF30 (#10016)
BOOL __fastcall FOG_SOCKET_GetError(FSafeSock* pSafeSock, DWORD* pFogError, int* pWSALastError)
{
	*pFogError = pSafeSock->nFogErrorCode;
	*pWSALastError = static_cast<int>(pSafeSock->nWSALastError);
	return TRUE;
}

//1.10f: Fog.dll 0x6FF63B20 (#10017)
//1.12a: Fog.dll 0x6FF5CF10 (#10017)
BOOL __fastcall FOG_SOCKET_SetBlockingMode(FSafeSock* pSafeSock, BOOL bNonBlocking)
{
	u_long ulMode = static_cast<u_long>(bNonBlocking);
	ioctlsocket(pSafeSock->nSocket, FIONBIO, &ulMode);
	return TRUE;
}
