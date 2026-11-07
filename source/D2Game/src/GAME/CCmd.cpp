#include "GAME/CCmd.h"

#include <cstdio>
#include <Storm.h>
#include <Fog.h>

// D2Net
#include <Server.h>

#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "ITEMS/ItemMode.h"
#include "PLAYER/PlrMsg.h"


#pragma warning(disable: 28159)

constexpr int32_t CHARACTER_SAVE_SIZE = 0x2000;


//D2Game.0x6FC317F0
int32_t __fastcall CCMD_IsStringZeroTerminated(const char* pData, int32_t nDataSize)
{
    for (int32_t i = 0; i < nDataSize; ++i)
    {
        if (pData[i] == '\0')
        {
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FC31810
uint32_t __fastcall CCMD_CanClientJoinGame(int32_t a1, int32_t a2, char* szClientName)
{
    // TODO: Names
    if (a1 < 1 || a1 > 1024)
    {
        return 6;
    }

    const D2GameGUID nGUID = GAME_GetGameGUIDFromGameId(static_cast<uint16_t>(a1));
    if (!nGUID)
    {
        return 6;
    }

    D2GameStrc* pGame = GAME_LockGame(nGUID);
    if (!pGame)
    {
        return 6;
    }

    if (a2 < 10 || a2 > 14)
    {
        GAME_LeaveGamesCriticalSection(pGame);
        return 16;
    }

    if (pGame->nClients >= 8)
    {
        GAME_LeaveGamesCriticalSection(pGame);
        return 15;
    }

    if (!szClientName[0] || !szClientName[1] || !CCMD_IsStringZeroTerminated(szClientName, 16))
    {
        GAME_LeaveGamesCriticalSection(pGame);
        return 6;
    }
  
    for (D2ClientStrc* pClient = pGame->pClientList; pClient; pClient = CLIENTS_GetNext(pClient))
    {
        const char* szName = CLIENTS_GetName(pClient);
        if (szName && !SStrCmpI(szName, szClientName, 16))
        {
            GAME_LeaveGamesCriticalSection(pGame);
            return 6;
        }
    }

    GAME_LeaveGamesCriticalSection(pGame);
    return 0;
}

//D2Game.0x6FC31910
void __fastcall CCMD_ProcessClientSystemMessage(void* pData, int32_t nSize)
{
    // TODO: Names
    const int32_t nClientId = *(int32_t*)pData;
    int32_t pPacket = (int32_t)((char*)pData + 4);

    switch (*((uint8_t*)pPacket))
    {
    case D2CLTSYS_NEWGAME:
    {
        D2GSPacketClt66* pPacket66 = (D2GSPacketClt66*)pPacket;
        if (GAME_VerifyCreateNewGame(nClientId, pPacket66))
        {
            GAME_SendGameInit(nClientId, pPacket66->szGameName, pPacket66->nGameType, pPacket66->nPlayerClass, pPacket66->szClientName, pPacket66->wArena, pPacket66->nGameFlags, pPacket66->nTemplate, pPacket66->unk0x2B, pPacket66->unk0x2C, pPacket66->nDifficulty, pPacket66->nLocale, 0, 0);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_NEWGAME)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_JOINGAME:
    {
        D2GSPacketClt67* pPacket67 = (D2GSPacketClt67*)pPacket;
        const int32_t nExistingClientId = CLIENTS_GetClientIdByName(pPacket67->szClientName);
        if (nExistingClientId && nExistingClientId != nClientId)
        {
            GAME_DisconnectClientById(nExistingClientId, EVENTTYPE_DISCONNECT);
        }

        // Purge any empty games (nClients == 0) and locate an active populated game if one exists
        uint16_t nActiveGameId = 0;
        for (uint16_t id = 1; id <= 1024; ++id)
        {
            const D2GameGUID nGUID = GAME_GetGameGUIDFromGameId(id);
            if (!nGUID || nGUID == static_cast<D2GameGUID>(-1))
            {
                continue;
            }
            if (D2GameStrc* pExistingGame = GAME_LockGame(nGUID))
            {
                const int32_t nClientsInGame = pExistingGame->nClients;
                D2_UNLOCK(pExistingGame->lpCriticalSection);
                if (nClientsInGame <= 0)
                {
                    GAME_CloseGame(nGUID);
                }
                else if (!nActiveGameId || id == pPacket67->nGameId)
                {
                    nActiveGameId = id;
                }
            }
        }

        if (!GAME_GetGameGUIDFromGameId(pPacket67->nGameId) && nActiveGameId != 0)
        {
            pPacket67->nGameId = nActiveGameId;
        }

        if (!GAME_GetGameGUIDFromGameId(pPacket67->nGameId))
        {
            uint32_t nGameFlags = 0x100004u;
            uint8_t nPlayerClass = pPacket67->nPlayerClass < 7 ? pPacket67->nPlayerClass : 0;

            char szSaveDir[1024] = {};
            FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
            char szSaveFile[1024] = {};
            std::snprintf(szSaveFile, sizeof(szSaveFile), "%s%s.d2s", szSaveDir, pPacket67->szClientName);
            FILE* pSaveFp = std::fopen(szSaveFile, "rb");
            if (!pSaveFp)
            {
                static const char* const kFallbackDirs[] = { "Debug\\save\\", "Release\\save\\" };
                for (const char* szRel : kFallbackDirs)
                {
                    std::snprintf(szSaveFile, sizeof(szSaveFile), "%s..\\..\\%s%s.d2s", szSaveDir, szRel, pPacket67->szClientName);
                    pSaveFp = std::fopen(szSaveFile, "rb");
                    if (pSaveFp)
                    {
                        break;
                    }
                }
            }
            if (pSaveFp)
            {
                uint8_t aHdr[64] = {};
                const size_t nRead = std::fread(aHdr, 1, sizeof(aHdr), pSaveFp);
                std::fclose(pSaveFp);
                if (nRead >= 44 && *reinterpret_cast<const uint32_t*>(&aHdr[0]) == 0xAA55AA55u)
                {
                    const uint32_t nVersion = *reinterpret_cast<const uint32_t*>(&aHdr[4]);
                    const uint32_t dwSaveFlags = *reinterpret_cast<const uint32_t*>(nVersion > 91u ? &aHdr[0x24] : &aHdr[24]);
                    nGameFlags = 0x000004u;
                    if (dwSaveFlags & CLIENTSAVEFLAG_EXPANSION)
                    {
                        nGameFlags |= 0x100000u;
                    }
                    if (dwSaveFlags & CLIENTSAVEFLAG_LADDER)
                    {
                        nGameFlags |= 0x200000u;
                    }
                    if (dwSaveFlags & CLIENTSAVEFLAG_HARDCORE)
                    {
                        nGameFlags |= 0x000800u;
                    }
                    const uint8_t nHdrClass = (nVersion > 91u) ? aHdr[0x28] : aHdr[34];
                    if (nHdrClass < 7u)
                    {
                        nPlayerClass = nHdrClass;
                    }
                }
            }

            char szGameName[16] = "D2MOO";
            GAME_SendGameInit(
                nClientId,
                szGameName,
                3,
                nPlayerClass,
                pPacket67->szClientName,
                0,
                nGameFlags,
                0,
                0,
                0,
                0,
                pPacket67->nLocale,
                0,
                0);
            break;
        }
        if (!gpD2EventCallbackTable_6FD45830)
        {
            const uint32_t nErrorCode = CCMD_CanClientJoinGame(pPacket67->nGameId, pPacket67->unk0x08, pPacket67->szClientName);
            if (nErrorCode)
            {
                sub_6FC3C6D0(nClientId, nErrorCode);
                D2NET_10015(nClientId, __FILE__, __LINE__);
                break;
            }
        }

        char szAccountName[16] = {};
        int32_t nCharSaveTransactionToken = 0;
        int32_t v18 = 0;
        int32_t v19 = 0;
        if (GAME_VerifyJoinGame(nClientId, pPacket67->nGameId, pPacket67->nPlayerClass, pPacket67->szClientName, pPacket67->nTokenId, szAccountName, &nCharSaveTransactionToken, pPacket67->nLocale, &v19, &v18))
        {
            GAME_JoinGame(nClientId, pPacket67->nGameId, pPacket67->nPlayerClass, pPacket67->szClientName, szAccountName, nCharSaveTransactionToken, pPacket67->nLocale, v19, v18);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_JOINGAME)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_ENDGAME:
    {
        if (GAME_VerifyEndGame(nClientId))
        {
            GAME_EndGame(nClientId, 1);
        }
        break;
    }
    case D2CLTSYS_JOINLIST:
    {
        if (D2GAME_Return1_6FC41900())
        {
            sub_6FC39870(nClientId);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_JOINLIST)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_JOINACT:
    {
        if (GAME_VerifyJoinAct(nClientId))
        {
            GAME_SendActInit(nClientId);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_JOINACT)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_OPENCHAR:
    {
        D2GSPacketClt6B* pPacket6B = (D2GSPacketClt6B*)pPacket;
        D2_ASSERT(pPacket6B->nTotalSize < CHARACTER_SAVE_SIZE);

        if (D2GAME_Return1_6FC41900())
        {
            sub_6FC399A0(nClientId, pPacket6B);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_OPENCHAR)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_0x6C:
    {
        D2GSPacketClt6C* pPacket6C = (D2GSPacketClt6C*)pPacket;
        CLIENTS_UpdatePing(nClientId, pPacket6C->unk0x01, pPacket6C->unk0x05);
        break;
    }
    case D2CLTSYS_DISCONNECT:
    {
        if (GAME_VerifyDisconnect(nClientId))
        {
            GAME_DisconnectClientById(nClientId, EVENTTYPE_DISCONNECT);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_FORCEDISCONNECT)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_REMOVEFROMGAME:
    {
        D2Game_10024_RemoveClientFromGame(nClientId);
        break;
    }
    default:
        return;
    }
}

//D2Game.0x6FC31C00
void __fastcall CCMD_ProcessClientMessage(void* pData, int32_t nPacketSize)
{
    const int32_t nClientId = *(int32_t*)pData;
    uint8_t* pPacket = (uint8_t*)pData + 4;
    D2GameStrc* pGame = GAME_GetGameByClientId(nClientId);
    if (!pGame)
    {
        return;
    }

    D2ClientStrc* pClient = CLIENTS_GetClientFromClientId(pGame, nClientId);
    D2_ASSERT(pClient);

    pClient->dwLastPacketTick = GetTickCount();

    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER && D2GAME_PACKET_Handler_6FC89320(pGame, pPlayer, pPacket, nPacketSize) >= 3 && pGame->nGameType != 3)
    {
        GAME_LogMessage(3, "[HACKLIST]  ProcessClientMessage: Client %d '%s' believed to be cheating. Command was %d", nClientId, pClient->szName, *pPacket);
        sub_6FC36C20(pGame, nClientId, __FILE__, __LINE__);
    }

    GAME_LeaveGamesCriticalSection(pGame);
}
