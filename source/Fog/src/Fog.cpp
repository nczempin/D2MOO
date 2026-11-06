#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <Windows.h>
#include <cstring>
#include <deque>
#include <unordered_map>
#include <vector>

#include <Fog.h>

#pragma comment(lib, "ws2_32.lib")

struct QServerPacket
{
	int32_t nClientId = 0;
	std::vector<uint8_t> data;
};

using FnValidateClientPacket = int32_t(__fastcall*)(const void* pPacketBuffer, uint32_t nBufferSize, int32_t* pOutSize, int32_t* pOutA4, int32_t* pOutQueueIdx, int32_t* pOutA6, int32_t nUnused1, int32_t nUnused2);
using FnOnConnectCallback = int32_t(__fastcall*)(int32_t a1, int32_t nClientId, int32_t a3, int32_t a4);
using FnOnDisconnectCallback = int32_t(__fastcall*)(int32_t a1, int32_t nClientId, int32_t a3, int32_t a4);
using FnReadPacketFromBuffer = int32_t(__fastcall*)(QServer* pServer, const void* pPacketBuffer, int32_t nBufferSize);

struct QServer
{
	CRITICAL_SECTION cs = {};
	int32_t nServerType = 0;
	int32_t nNumQueues = 3;
	int32_t nPort = 0;
	int32_t nMaxClients = 8;
	BOOL bHackListEnabled = FALSE;
	FnValidateClientPacket pfValidate = nullptr;
	FnOnConnectCallback pfOnConnect = nullptr;
	FnOnDisconnectCallback pfOnDisconnect = nullptr;
	FnReadPacketFromBuffer pfReadPacket = nullptr;
	void* pfCallback10171 = nullptr;
	std::deque<QServerPacket> queues[3];
	std::unordered_map<int32_t, int32_t> clientGameGuids;
	SOCKET listenSock = INVALID_SOCKET;
	HANDLE hNetThread = nullptr;
	volatile BOOL bStopNetThread = FALSE;
	int32_t nNextClientId = 1;
	std::unordered_map<int32_t, SOCKET> clientSockets;
	std::unordered_map<int32_t, uint32_t> clientIps;
	std::unordered_map<int32_t, std::vector<uint8_t>> clientRecvBuffers;
};

static DWORD WINAPI QServer_NetworkThreadProc(LPVOID lpParam)
{
	QServer* pServer = static_cast<QServer*>(lpParam);
	if (!pServer)
	{
		return 0;
	}

	while (!pServer->bStopNetThread)
	{
		fd_set readSet;
		FD_ZERO(&readSet);

		if (pServer->listenSock != INVALID_SOCKET)
		{
			FD_SET(pServer->listenSock, &readSet);
		}

		std::vector<std::pair<int32_t, SOCKET>> activeClients;
		EnterCriticalSection(&pServer->cs);
		activeClients.reserve(pServer->clientSockets.size());
		for (const auto& kv : pServer->clientSockets)
		{
			if (kv.second != INVALID_SOCKET && readSet.fd_count < FD_SETSIZE)
			{
				FD_SET(kv.second, &readSet);
				activeClients.push_back(kv);
			}
		}
		LeaveCriticalSection(&pServer->cs);

		if (readSet.fd_count == 0)
		{
			Sleep(10);
			continue;
		}

		timeval tv = {};
		tv.tv_sec = 0;
		tv.tv_usec = 10000; // 10ms
		const int nReady = select(0, &readSet, nullptr, nullptr, &tv);
		if (nReady <= 0)
		{
			continue;
		}

		if (pServer->listenSock != INVALID_SOCKET && FD_ISSET(pServer->listenSock, &readSet))
		{
			sockaddr_in clientAddr = {};
			int addrLen = sizeof(clientAddr);
			SOCKET clientSock = accept(pServer->listenSock, reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
			if (clientSock != INVALID_SOCKET)
			{
				BOOL bNoDelay = TRUE;
				setsockopt(clientSock, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&bNoDelay), sizeof(bNoDelay));

				int32_t nNewClientId = 0;
				EnterCriticalSection(&pServer->cs);
				nNewClientId = pServer->nNextClientId++;
				pServer->clientSockets[nNewClientId] = clientSock;
				pServer->clientIps[nNewClientId] = clientAddr.sin_addr.S_un.S_addr;
				pServer->clientRecvBuffers[nNewClientId].clear();
				LeaveCriticalSection(&pServer->cs);

				if (pServer->pfOnConnect)
				{
					pServer->pfOnConnect(0, nNewClientId, 0, 0);
				}
			}
		}

		for (const auto& clientPair : activeClients)
		{
			const int32_t nClientId = clientPair.first;
			const SOCKET sock = clientPair.second;
			if (!FD_ISSET(sock, &readSet))
			{
				continue;
			}

			uint8_t aRecv[4096];
			const int nRead = recv(sock, reinterpret_cast<char*>(aRecv), sizeof(aRecv), 0);
			if (nRead <= 0)
			{
				EnterCriticalSection(&pServer->cs);
				auto it = pServer->clientSockets.find(nClientId);
				if (it != pServer->clientSockets.end() && it->second == sock)
				{
					closesocket(sock);
					pServer->clientSockets.erase(it);
				}
				pServer->clientRecvBuffers.erase(nClientId);
				LeaveCriticalSection(&pServer->cs);

				if (pServer->pfOnDisconnect)
				{
					pServer->pfOnDisconnect(0, nClientId, 0, 0);
				}
				continue;
			}

			std::vector<std::vector<uint8_t>> completePackets;
			EnterCriticalSection(&pServer->cs);
			auto& buf = pServer->clientRecvBuffers[nClientId];
			buf.insert(buf.end(), aRecv, aRecv + nRead);

			while (!buf.empty())
			{
				int32_t nPktSize = static_cast<int32_t>(buf.size());
				int32_t a4 = 0;
				int32_t nQueueIdx = 1;
				int32_t a6 = 100;
				if (pServer->pfValidate)
				{
					const int32_t res = pServer->pfValidate(
						buf.data(),
						static_cast<uint32_t>(buf.size()),
						&nPktSize,
						&a4,
						&nQueueIdx,
						&a6,
						0,
						0);
					if (res == 3)
					{
						if (buf.size() > 2048)
						{
							buf.clear();
						}
						break;
					}
					if (res == 4 || nPktSize <= 0 || static_cast<size_t>(nPktSize) > buf.size())
					{
						buf.clear();
						break;
					}
				}
				completePackets.emplace_back(buf.begin(), buf.begin() + nPktSize);
				buf.erase(buf.begin(), buf.begin() + nPktSize);
			}
			LeaveCriticalSection(&pServer->cs);

			for (const auto& pkt : completePackets)
			{
				FOG_10175(pServer, pkt.data(), static_cast<int>(pkt.size()), nClientId);
			}
		}
	}

	return 0;
}

FOG_DLL_DECL QServer* __stdcall FOG_InitializeServer(int nServerType, int nNumQueues, int nPort, int a4, void* pfValidate, void* pfOnConnect, void* pfOnDisconnect, void* pfReadPacket)
{
	(void)a4;
	FOG_10218();

	QServer* pServer = new QServer();
	InitializeCriticalSection(&pServer->cs);
	pServer->nServerType = nServerType;
	pServer->nNumQueues = (nNumQueues > 0 && nNumQueues <= 3) ? nNumQueues : 3;
	pServer->nPort = nPort ? nPort : 4000;
	pServer->pfValidate = reinterpret_cast<FnValidateClientPacket>(pfValidate);
	pServer->pfOnConnect = reinterpret_cast<FnOnConnectCallback>(pfOnConnect);
	pServer->pfOnDisconnect = reinterpret_cast<FnOnDisconnectCallback>(pfOnDisconnect);
	pServer->pfReadPacket = reinterpret_cast<FnReadPacketFromBuffer>(pfReadPacket);

	WSADATA wsaData = {};
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0)
	{
		SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listenSock != INVALID_SOCKET)
		{
			BOOL bReuse = TRUE;
			setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&bReuse), sizeof(bReuse));

			sockaddr_in bindAddr = {};
			bindAddr.sin_family = AF_INET;
			bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			bindAddr.sin_port = htons(static_cast<u_short>(pServer->nPort));

			if (bind(listenSock, reinterpret_cast<sockaddr*>(&bindAddr), sizeof(bindAddr)) == 0 &&
				listen(listenSock, SOMAXCONN) == 0)
			{
				pServer->listenSock = listenSock;
				pServer->bStopNetThread = FALSE;
				pServer->hNetThread = CreateThread(nullptr, 0, QServer_NetworkThreadProc, pServer, 0, nullptr);
			}
			else
			{
				closesocket(listenSock);
			}
		}
	}

	return pServer;
}

FOG_DLL_DECL int __stdcall FOG_SetMaxClientsPerGame(QServer* pServer, int nMaxClients)
{
	if (pServer)
	{
		pServer->nMaxClients = nMaxClients;
	}
	return nMaxClients;
}

FOG_DLL_DECL int __stdcall FOG_10152(void* pServerVoid, const uint8_t* pData, int nSize)
{
	(void)pData;
	(void)nSize;
	QServer* pServer = static_cast<QServer*>(pServerVoid);
	if (pServer)
	{
		pServer->bStopNetThread = TRUE;
		if (pServer->listenSock != INVALID_SOCKET)
		{
			closesocket(pServer->listenSock);
			pServer->listenSock = INVALID_SOCKET;
		}
		if (pServer->hNetThread)
		{
			WaitForSingleObject(pServer->hNetThread, 2000);
			CloseHandle(pServer->hNetThread);
			pServer->hNetThread = nullptr;
		}
		EnterCriticalSection(&pServer->cs);
		for (auto& kv : pServer->clientSockets)
		{
			if (kv.second != INVALID_SOCKET)
			{
				closesocket(kv.second);
			}
		}
		pServer->clientSockets.clear();
		LeaveCriticalSection(&pServer->cs);
		DeleteCriticalSection(&pServer->cs);
		delete pServer;
	}
	return 1;
}

FOG_DLL_DECL int __stdcall FOG_WaitForSingleObject(void* pServer, int dwMilliseconds)
{
	(void)pServer;
	if (dwMilliseconds > 0)
	{
		Sleep(static_cast<DWORD>(dwMilliseconds));
	}
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10156(void* pServerVoid, int nQueueIdx, uint8_t* pBuffer, int nBufferSize)
{
	QServer* pServer = static_cast<QServer*>(pServerVoid);
	if (!pServer || nQueueIdx < 0 || nQueueIdx >= 3 || !pBuffer || nBufferSize < 4)
	{
		return -1;
	}

	EnterCriticalSection(&pServer->cs);
	auto& q = pServer->queues[nQueueIdx];
	if (q.empty())
	{
		LeaveCriticalSection(&pServer->cs);
		return -1;
	}

	QServerPacket pkt = std::move(q.front());
	q.pop_front();
	LeaveCriticalSection(&pServer->cs);

	const int nCopySize = static_cast<int>(pkt.data.size());
	if (nCopySize + 4 > nBufferSize)
	{
		return -1;
	}

	*reinterpret_cast<int32_t*>(pBuffer) = pkt.nClientId;
	if (nCopySize > 0)
	{
		std::memcpy(pBuffer + 4, pkt.data.data(), static_cast<size_t>(nCopySize));
	}
	return nCopySize;
}

static void QServer_CloseClientSocket(QServer* pServer, int nClientId)
{
	if (!pServer || nClientId <= 0)
	{
		return;
	}
	EnterCriticalSection(&pServer->cs);
	auto it = pServer->clientSockets.find(nClientId);
	if (it != pServer->clientSockets.end())
	{
		if (it->second != INVALID_SOCKET)
		{
			closesocket(it->second);
		}
		pServer->clientSockets.erase(it);
	}
	pServer->clientRecvBuffers.erase(nClientId);
	LeaveCriticalSection(&pServer->cs);
}

FOG_DLL_DECL int __stdcall FOG_10157(void* pServerVoid, int nClientId, const uint8_t* pBuffer, int nBufferSize)
{
	QServer* pServer = static_cast<QServer*>(pServerVoid);
	if (!pServer || !pBuffer || nBufferSize <= 0)
	{
		return 0;
	}
	if (nClientId > 0)
	{
		SOCKET sock = INVALID_SOCKET;
		EnterCriticalSection(&pServer->cs);
		auto it = pServer->clientSockets.find(nClientId);
		if (it != pServer->clientSockets.end())
		{
			sock = it->second;
		}
		LeaveCriticalSection(&pServer->cs);
		if (sock != INVALID_SOCKET)
		{
			int nSentTotal = 0;
			while (nSentTotal < nBufferSize)
			{
				const int nSent = send(sock, reinterpret_cast<const char*>(pBuffer + nSentTotal), nBufferSize - nSentTotal, 0);
				if (nSent <= 0)
				{
					return 0;
				}
				nSentTotal += nSent;
			}
			return 1;
		}
		return 0;
	}
	if (pServer->pfReadPacket)
	{
		pServer->pfReadPacket(pServer, pBuffer, nBufferSize);
	}
	return 1;
}

FOG_DLL_DECL int __stdcall FOG_10158(void* pServerVoid, int nClientId)
{
	QServer* pServer = static_cast<QServer*>(pServerVoid);
	if (pServer && nClientId > 0)
	{
		EnterCriticalSection(&pServer->cs);
		auto it = pServer->clientIps.find(nClientId);
		const uint32_t ip = (it != pServer->clientIps.end()) ? it->second : 0x0100007F;
		LeaveCriticalSection(&pServer->cs);
		return static_cast<int>(ip);
	}
	return 0x0100007F; // 127.0.0.1
}

FOG_DLL_DECL int __stdcall FOG_10159(void* pServer, int nClientId, char* szBuffer, int nBufferSize)
{
	(void)pServer;
	(void)nClientId;
	if (szBuffer && nBufferSize > 0)
	{
		strncpy_s(szBuffer, static_cast<size_t>(nBufferSize), "127.0.0.1", _TRUNCATE);
	}
	return 1;
}

FOG_DLL_DECL int __stdcall FOG_10161(void* pServerVoid, int nClientId)
{
	QServer* pServer = static_cast<QServer*>(pServerVoid);
	if (pServer && nClientId > 0)
	{
		EnterCriticalSection(&pServer->cs);
		auto it = pServer->clientSockets.find(nClientId);
		const SOCKET sock = (it != pServer->clientSockets.end()) ? it->second : 0;
		LeaveCriticalSection(&pServer->cs);
		return static_cast<int>(sock);
	}
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10162(void* pServerVoid, int nClientId, const char* szFile, int nLine)
{
	(void)szFile;
	(void)nLine;
	QServer_CloseClientSocket(static_cast<QServer*>(pServerVoid), nClientId);
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10163(void* pServerVoid, int nClientId, const char* szFile, int nLine)
{
	(void)szFile;
	(void)nLine;
	QServer_CloseClientSocket(static_cast<QServer*>(pServerVoid), nClientId);
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10164(void* pServer, int a2, int a3, int a4)
{
	(void)pServer;
	(void)a2;
	(void)a3;
	(void)a4;
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10165(void* pServerVoid, int nClientId, const char* szFile, int nLine)
{
	(void)szFile;
	(void)nLine;
	QServer_CloseClientSocket(static_cast<QServer*>(pServerVoid), nClientId);
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10166(void* pServer, int nClientId, int a3, int a4)
{
	(void)pServer;
	(void)nClientId;
	(void)a3;
	(void)a4;
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10170(void* pServer, int a2)
{
	(void)pServer;
	(void)a2;
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10171(void* pServerVoid, void* pfCallback)
{
	QServer* pServer = static_cast<QServer*>(pServerVoid);
	if (pServer)
	{
		pServer->pfCallback10171 = pfCallback;
	}
	return 1;
}

FOG_DLL_DECL int __stdcall FOG_10172(void* pServerVoid, int nClientId, int dwGameGuid)
{
	QServer* pServer = static_cast<QServer*>(pServerVoid);
	if (pServer)
	{
		EnterCriticalSection(&pServer->cs);
		pServer->clientGameGuids[nClientId] = dwGameGuid;
		LeaveCriticalSection(&pServer->cs);
	}
	return dwGameGuid;
}

FOG_DLL_DECL int __stdcall FOG_10173(void* pServerVoid, int nClientId)
{
	QServer* pServer = static_cast<QServer*>(pServerVoid);
	if (!pServer)
	{
		return 0;
	}
	EnterCriticalSection(&pServer->cs);
	auto it = pServer->clientGameGuids.find(nClientId);
	int32_t guid = (it != pServer->clientGameGuids.end()) ? it->second : 0;
	LeaveCriticalSection(&pServer->cs);
	return guid;
}

FOG_DLL_DECL int __stdcall FOG_10175(void* pServerVoid, const uint8_t* pBuffer, int nBufferSize, int nClientId)
{
	QServer* pServer = static_cast<QServer*>(pServerVoid);
	if (!pServer || !pBuffer || nBufferSize <= 0)
	{
		return 0;
	}

	int32_t nSize = nBufferSize;
	int32_t a4 = 0;
	int32_t nQueueIdx = 1;
	int32_t a6 = 100;
	if (pServer->pfValidate)
	{
		const int32_t res = pServer->pfValidate(pBuffer, static_cast<uint32_t>(nBufferSize), &nSize, &a4, &nQueueIdx, &a6, 0, 0);
		if (res == 3 || res == 4)
		{
			return 0;
		}
	}
	else
	{
		const uint8_t hdr = pBuffer[0];
		if (hdr >= 0x67u && hdr < 0x71u)
		{
			nQueueIdx = 0;
		}
		else if (hdr < 0x67u)
		{
			nQueueIdx = 1;
		}
		else
		{
			nQueueIdx = 2;
		}
	}

	if (nQueueIdx < 0 || nQueueIdx >= 3)
	{
		nQueueIdx = 1;
	}

	QServerPacket pkt;
	pkt.nClientId = nClientId;
	pkt.data.assign(pBuffer, pBuffer + nBufferSize);

	EnterCriticalSection(&pServer->cs);
	pServer->queues[nQueueIdx].push_back(std::move(pkt));
	LeaveCriticalSection(&pServer->cs);
	return 1;
}

FOG_DLL_DECL int __stdcall FOG_10177(void* pServer, int dwMilliseconds)
{
	(void)pServer;
	(void)dwMilliseconds;
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_SetHackListEnabled(QServer* pServer, BOOL bHacklistEnabled)
{
	if (pServer)
	{
		pServer->bHackListEnabled = bHacklistEnabled;
	}
	return bHacklistEnabled;
}

FOG_DLL_DECL int __stdcall FOG_10180(void* pServer)
{
	(void)pServer;
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10181(void* pServer, const uint8_t* pBuffer, int nBufferSize, int a4)
{
	(void)pServer;
	(void)pBuffer;
	(void)nBufferSize;
	(void)a4;
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10186(void* pServer, int nIndex, int nValue)
{
	(void)pServer;
	(void)nIndex;
	return nValue;
}

FOG_DLL_DECL int __fastcall FOG_10187(void* pServer, int a2, int a3)
{
	(void)pServer;
	(void)a2;
	(void)a3;
	return 0;
}
