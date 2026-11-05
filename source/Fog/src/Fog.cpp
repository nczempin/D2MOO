#include <Windows.h>
#include <cstring>
#include <deque>
#include <unordered_map>
#include <vector>

#include <Fog.h>

struct QServerPacket
{
	int32_t nClientId = 0;
	std::vector<uint8_t> data;
};

using FnValidateClientPacket = int32_t(__fastcall*)(const void* pPacketBuffer, uint32_t nBufferSize, int32_t* pOutSize, int32_t* pOutA4, int32_t* pOutQueueIdx, int32_t* pOutA6, int32_t nUnused1, int32_t nUnused2);
using FnReadPacketFromBuffer = int32_t(__fastcall*)(const void* pPacketBuffer, int32_t nBufferSize);

struct QServer
{
	CRITICAL_SECTION cs = {};
	int32_t nServerType = 0;
	int32_t nNumQueues = 3;
	int32_t nPort = 0;
	int32_t nMaxClients = 8;
	BOOL bHackListEnabled = FALSE;
	FnValidateClientPacket pfValidate = nullptr;
	void* pfOnConnect = nullptr;
	void* pfOnDisconnect = nullptr;
	FnReadPacketFromBuffer pfReadPacket = nullptr;
	void* pfCallback10171 = nullptr;
	std::deque<QServerPacket> queues[3];
	std::unordered_map<int32_t, int32_t> clientGameGuids;
};

FOG_DLL_DECL QServer* __stdcall FOG_InitializeServer(int nServerType, int nNumQueues, int nPort, int a4, void* pfValidate, void* pfOnConnect, void* pfOnDisconnect, void* pfReadPacket)
{
	(void)a4;
	QServer* pServer = new QServer();
	InitializeCriticalSection(&pServer->cs);
	pServer->nServerType = nServerType;
	pServer->nNumQueues = (nNumQueues > 0 && nNumQueues <= 3) ? nNumQueues : 3;
	pServer->nPort = nPort;
	pServer->pfValidate = reinterpret_cast<FnValidateClientPacket>(pfValidate);
	pServer->pfOnConnect = pfOnConnect;
	pServer->pfOnDisconnect = pfOnDisconnect;
	pServer->pfReadPacket = reinterpret_cast<FnReadPacketFromBuffer>(pfReadPacket);
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

FOG_DLL_DECL int __stdcall FOG_10157(void* pServerVoid, int nClientId, const uint8_t* pBuffer, int nBufferSize)
{
	(void)nClientId;
	QServer* pServer = static_cast<QServer*>(pServerVoid);
	if (!pServer || !pBuffer || nBufferSize <= 0)
	{
		return 0;
	}
	if (pServer->pfReadPacket)
	{
		pServer->pfReadPacket(pBuffer, nBufferSize);
	}
	return 1;
}

FOG_DLL_DECL int __stdcall FOG_10158(void* pServer, int nClientId)
{
	(void)pServer;
	(void)nClientId;
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

FOG_DLL_DECL int __stdcall FOG_10161(void* pServer, int nClientId)
{
	(void)pServer;
	(void)nClientId;
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10162(void* pServer, int nClientId, const char* szFile, int nLine)
{
	(void)pServer;
	(void)nClientId;
	(void)szFile;
	(void)nLine;
	return 0;
}

FOG_DLL_DECL int __stdcall FOG_10163(void* pServer, int nClientId, const char* szFile, int nLine)
{
	(void)pServer;
	(void)nClientId;
	(void)szFile;
	(void)nLine;
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

FOG_DLL_DECL int __stdcall FOG_10165(void* pServer, int nClientId, const char* szFile, int nLine)
{
	(void)pServer;
	(void)nClientId;
	(void)szFile;
	(void)nLine;
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
