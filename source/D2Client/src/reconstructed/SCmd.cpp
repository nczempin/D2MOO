// =============================================================================
// D2Client.dll 1.12a (ImageBase = 0x6FAB0000)
// Reconstructed Translation Unit: GAME/SCmd.cpp (0x6FB14190 .. 0x6FB18030)
//
// Source Assertion Path: "..\Source\D2Client\GAME\SCmd.cpp" (0x6FB84B64)
// Dispatch Table Address: 0x6FB93D20 (175 entries, opcodes 0x00 .. 0xAE)
//
// Cross-referenced against:
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Dungeon.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Units.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Path/Path.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2StatList.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2BitManip.h
//   - H:/git/diablo2-web/src/netproto/d2gs/tables/s2c-sizes.ts
// =============================================================================

#include <stdint.h>
#include <string.h>
#include <windows.h>

#include <Fog.h>
#include <D2Dungeon.h>
#include <Drlg/D2DrlgDrlg.h>
#include <DataTbls/MonsterTbls.h>
#include <Units/Units.h>
#include <Path/Path.h>
#include <D2Collision.h>
#include <D2StatList.h>
#include <D2States.h>
#include <D2Skills.h>
#include <D2Items.h>
#include <D2DataTbls.h>
#include <D2BitManip.h>
#include <D2QuestRecord.h>

#include "D2ClientReconstructed.h"

typedef D2ActiveRoomStrc D2RoomStrc;

// =============================================================================
// External D2Net / Fog / D2Common Imports
// =============================================================================
// D2Net.#10002: int32_t __fastcall SERVER_GetServerPacketSize(const uint8_t* pBuffer, int32_t nBufferSize, int32_t* pOutVarHeader)
extern "C" int32_t __fastcall SERVER_GetServerPacketSize(const uint8_t* pBuffer, int32_t nBufferSize, int32_t* pOutVarHeader);
// D2Net.#10036: int32_t __stdcall CLIENT_Send(int32_t nSize, int32_t nQueue, const void* pPacket)
extern "C" int32_t __stdcall CLIENT_Send(int32_t nSize, int32_t nQueue, const void* pPacket);

// =============================================================================
// S->C Dispatch Table Entry & Unit Packet Queue Structs
// =============================================================================
typedef void (__fastcall* SCMD_PacketHandlerFn)(uint8_t* pPacket);
typedef void (__fastcall* SCMD_UnitPacketHandlerFn)(D2UnitStrc* pUnit, uint8_t* pPacket);

struct D2SCmdTableStrc // 12 bytes per opcode at 0x6FB93D20 (175 entries: 0x00 .. 0xAE)
{
    SCMD_PacketHandlerFn     pfHandler;      // +0x00: Direct packet handler (or NULL if unit-queued / unhandled)
    int32_t                  nPacketSize;    // +0x04: Fixed packet byte size, or -1 (0xFFFFFFFF) for variable-length
    SCMD_UnitPacketHandlerFn pfUnitHandler;  // +0x08: Deferred per-unit packet handler drained via pUnit->pPacketQueue (+0xD8)
};

// Attached to D2UnitStrc at offset +0xD8 (`pUnit->pPacketQueue`)
struct D2UnitPacketQueueStrc // 16 bytes
{
    int32_t  nAllocatedCount; // +0x00: Capacity in 32-byte packet slots
    int32_t  nQueuedCount;    // +0x04: Current number of queued packets
    int32_t  nReadIndex;      // +0x08: Current drain cursor
    uint8_t* pBuffer;         // +0x0C: Contiguous array of 32-byte packet slots
};

// =============================================================================
// Client Globals Referenced by SCmd.cpp
// =============================================================================
extern D2UnitStrc*    gpClientPlayer;        // 0x6FBCC3D0
extern D2DrlgActStrc* gpClientAct;           // 0x6FBCC02C
extern D2UnitStrc*    gapServerSideUnitHashTables[6][128]; // 0x6FBCA960
#define gapMonsterHashTable (gapServerSideUnitHashTables[1])
extern uint8_t        gnClientDifficulty;    // 0x6FBCBFF4
extern int32_t        gbIsExpansionClient;   // 0x6FBB330C
extern int32_t        gbIsLadderClient;      // 0x6FBB3310
extern uint32_t       gdwArenaFlags;         // 0x6FBA675C
extern int32_t        gbActLoadedFlag;       // 0x6FBCC004
extern int32_t        gbActUnloadedFlag;     // 0x6FBCC00C
extern uint32_t       gdwReassignGraceTick;  // 0x6FBB32D4
extern uint32_t       g_dwBytesReceived;     // 0x6FBCBE50
extern uint32_t       g_dwPacketsReceived;   // 0x6FBCBC90

// Forward declarations for internal D2Client helpers
D2UnitStrc* __fastcall UNIT_GetClientPlayer(void);                                              // 0x6FB0B3E0
D2UnitStrc* __fastcall UNIT_FindClientUnit(int32_t nUnitGUID, int32_t nUnitType);               // 0x6FACE550
void __fastcall GAME_LoadAct(uint8_t nAct, uint32_t dwMapSeed, uint16_t wLevelId, uint32_t dwFlags); // 0x6FB2C320
int32_t __fastcall UNITMODE_DispatchClientEvent(D2UnitStrc* pUnit, int32_t nEvent, int32_t nX, int32_t nY, D2UnitStrc* pTarget, int32_t a6, int32_t a7); // 0x6FAF1450
int32_t __fastcall UNITMODE_CheckPositionDrift(D2UnitStrc* pUnit, int32_t nServerX, int32_t nServerY); // 0x6FAF0EB0
void __fastcall UNIT_SetClientUnitPosition(D2UnitStrc* pUnit, int32_t nX, int32_t nY);          // 0x6FAD1550
void __fastcall UNIT_AssignClientWarp(D2UnitStrc* pWarpUnit, uint16_t wWarpId, int32_t nX, int32_t nY); // 0x6FAD0B30
void __fastcall UNIT_AssignClientObject(uint8_t* pPacket);                                      // 0x6FAD0CF0
void __fastcall UNIT_AssignClientPlayer(uint8_t* pPacket);                                      // 0x6FAD0D60
D2UnitStrc* __fastcall UNIT_AssignClientMonster(
    uint32_t dwGUID,
    uint16_t wClassId,
    int32_t nX,
    int32_t nY,
    uint8_t nAnimMode,
    const uint8_t* pCompInfo,
    int32_t nFlags,
    const uint8_t* pBossMods,
    const char* szMonName,
    int32_t nNameFlags);                                                                        // 0x6FAD0EB0
void __fastcall UNIT_RemoveClientUnit(uint8_t nUnitType, uint32_t dwUnitGUID);                  // 0x6FAD0620

// Cross-module reconstructed helpers invoked by S->C handlers
struct D2RosterPetCreateDataStrc
{
    uint32_t dwMonSeed;
    uint32_t dwNameIdx;
    uint32_t dwSpecialFlag;
};

void __stdcall  sub_6FB23EE0(uint8_t* pPacket);                                                 // UI_text.cpp (0x26 GameChat)
void __fastcall sub_6FB21D70(uint8_t* pPacket);                                                 // UI_text.cpp (0x27 NpcInfo / speech)
int32_t __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t bMoveMouse);     // UI_ui.cpp (0x6FB3EF00: 0=Open, 1=Close, 2=Toggle)
void __fastcall D2Client_ProcessItemPacket9C(uint8_t* pPacket);                                 // UNIT_Item.cpp (0x9C)
void __fastcall D2Client_ProcessItemPacket9D(uint8_t* pPacket);                                 // UNIT_Item.cpp (0x9D)
void __fastcall D2Client_OnNpcInfoPacket(uint8_t* pPacket);                                     // UI_npcmenu.cpp / bridge (0x27)
void __fastcall D2Client_OnWeaponSwitchPacket(void);                                            // D2ClientLinkBridge.cpp (0x97)
void __fastcall D2Client_PlayUnitSound(int32_t nUnitType, uint32_t dwUnitId, uint16_t wSound);  // Sound_SoundHdr.cpp / bridge (0x2C)
void __fastcall D2Client_AssignSkillHotkey(int32_t nSlot, int32_t nSkillId, int32_t bLeft, uint32_t dwItemGUID); // UNIT_PlrSkills.cpp / bridge (0x7B)

// GAME_Roster.cpp exports
void __fastcall ROSTER_AddOrUpdatePlayer(
    uint32_t dwUnitId,
    const char* szCharName,
    uint32_t dwClassId,
    uint16_t wLevel,
    uint16_t wPartyId,
    uint32_t dwPartyFlags,
    uint16_t wGuildEmblem,
    const char* szAccountName,
    const char* szGuildTag);
void __fastcall ROSTER_RemovePlayerFromRoster(uint32_t dwUnitId);
void __fastcall ROSTER_AddPlayerCorpse(uint32_t dwPlayerUnitId, uint32_t dwCorpseUnitId);
void __fastcall ROSTER_RemovePlayerCorpse(uint32_t dwPlayerUnitId, uint32_t dwCorpseUnitId);
void __fastcall ROSTER_UpdatePlayerPartyAndLevel(uint32_t dwUnitId, uint16_t wPartyId, uint16_t wLevel, uint32_t dwPartyFlags);
void __fastcall ROSTER_UpdatePlayerLifeAndLevelId(uint32_t dwUnitId, uint32_t dwPartyLife, uint16_t wLevelId);
void __fastcall ROSTER_UpdatePlayerPartyFlags(uint32_t dwUnitId, uint8_t nPartyFlags);
void __fastcall ROSTER_OnRelationPacketReceived(uint32_t dwSourceUnitId, uint32_t dwTargetUnitId, uint16_t wRelationState);
void __fastcall ROSTER_UpdatePlayerPartyId(uint32_t dwUnitId, uint16_t wPartyId);
void __fastcall ROSTER_SetPlayerCoordsByUnitId(uint32_t dwUnitId, int32_t nXpos, int32_t nYpos);
void __fastcall ROSTER_SetPlayerArenaScore(uint32_t dwUnitId, int16_t nArenaScore);
void __fastcall ROSTERPETS_RemoveOrMarkPetDead(uint32_t dwPetUnitId);
void __fastcall ROSTERPETS_AddOrUpdateBasicPet(int32_t nPetType, uint32_t dwPetUnitId, uint32_t dwOwnerId, int32_t nClassId);
void __fastcall ROSTERPETS_AddOrUpdateFullPet(int32_t nPetType, uint32_t dwPetUnitId, const D2RosterPetCreateDataStrc* pCreateData, uint32_t dwOwnerId, int32_t nClassId);

// =============================================================================
// SECTION 1: Deferred Per-Unit Packet Queue Management (0x6FB14190 .. 0x6FB14AE0)
// =============================================================================

// 0x6FB14330
// Allocates or grows `pUnit->pPacketQueue` (+0xD8) in increments of 4 slots (32 bytes/slot).
void __fastcall SCMD_GrowUnitPacketQueue(D2UnitStrc* pUnit)
{
    D2UnitPacketQueueStrc** ppQueue = reinterpret_cast<D2UnitPacketQueueStrc**>(reinterpret_cast<uint8_t*>(pUnit) + 0xD8);
    D2UnitPacketQueueStrc* pQueue = *ppQueue;

    if (pQueue == nullptr)
    {
        pQueue = static_cast<D2UnitPacketQueueStrc*>(
            FOG_AllocClientMemory(sizeof(D2UnitPacketQueueStrc), "..\\Source\\D2Client\\GAME\\SCmd.cpp", 674, 0));
        pQueue->nAllocatedCount = 0;
        pQueue->nQueuedCount = 0;
        pQueue->nReadIndex = 0;
        pQueue->pBuffer = nullptr;
        *ppQueue = pQueue;
    }

    const int32_t nNewCapacity = pQueue->nAllocatedCount + 4;
    uint8_t* pNewBuffer = static_cast<uint8_t*>(
        FOG_AllocClientMemory(static_cast<size_t>(nNewCapacity) * 32, "..\\Source\\D2Client\\GAME\\SCmd.cpp", 684, 0));

    if (pQueue->pBuffer != nullptr)
    {
        memcpy(pNewBuffer, pQueue->pBuffer, static_cast<size_t>(pQueue->nAllocatedCount) * 32);
        FOG_FreeClientMemory(pQueue->pBuffer, "..\\Source\\D2Client\\GAME\\SCmd.cpp", 689, 0);
    }

    pQueue->nAllocatedCount = nNewCapacity;
    pQueue->pBuffer = pNewBuffer;
}

// 0x6FB142B0
// Frees `pUnit->pPacketQueue` (+0xD8) when a unit is destroyed or recycled.
void __fastcall SCMD_FreeUnitPacketQueue(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr)
    {
        return;
    }

    D2UnitPacketQueueStrc** ppQueue = reinterpret_cast<D2UnitPacketQueueStrc**>(reinterpret_cast<uint8_t*>(pUnit) + 0xD8);
    D2UnitPacketQueueStrc* pQueue = *ppQueue;
    if (pQueue != nullptr)
    {
        if (pQueue->pBuffer != nullptr)
        {
            FOG_FreeClientMemory(pQueue->pBuffer, "..\\Source\\D2Client\\GAME\\SCmd.cpp", 648, 0);
        }
        FOG_FreeClientMemory(pQueue, "..\\Source\\D2Client\\GAME\\SCmd.cpp", 650, 0);
        *ppQueue = nullptr;
    }
}

// 0x6FB14AE0
// Enqueues a 32-byte unit movement/event packet (0x0C..0x10, 0x67..0x6D) onto `pUnit->pPacketQueue`.
void __fastcall SCMD_QueueUnitPacket(D2UnitStrc* pUnit, const uint8_t* pPacket, size_t nPacketSize)
{
    if (pUnit == nullptr || nPacketSize > 32)
    {
        return;
    }

    D2UnitPacketQueueStrc* pQueue = *reinterpret_cast<D2UnitPacketQueueStrc**>(reinterpret_cast<uint8_t*>(pUnit) + 0xD8);
    if (pQueue == nullptr || pQueue->nQueuedCount >= pQueue->nAllocatedCount)
    {
        SCMD_GrowUnitPacketQueue(pUnit);
        pQueue = *reinterpret_cast<D2UnitPacketQueueStrc**>(reinterpret_cast<uint8_t*>(pUnit) + 0xD8);
    }

    uint8_t* pSlot = &pQueue->pBuffer[pQueue->nQueuedCount * 32];
    memcpy(pSlot, pPacket, nPacketSize);
    pQueue->nQueuedCount += 1;
}

// 0x6FB14190
// Drains `pUnit->pPacketQueue` at the end of `UNITMODE_UpdateClientUnit` (0x6FAF1EE0 at 0x6FAF2033)
// and invokes `sgptSCmdTable[bOpcode].pfUnitHandler(pUnit, pSlot)` for each queued packet.
extern const D2SCmdTableStrc sgptSCmdTable[0xAF];

void __fastcall SCMD_DrainUnitPacketQueue(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr)
    {
        return;
    }

    D2UnitPacketQueueStrc* pQueue = *reinterpret_cast<D2UnitPacketQueueStrc**>(reinterpret_cast<uint8_t*>(pUnit) + 0xD8);
    if (pQueue == nullptr || pQueue->nQueuedCount == 0)
    {
        return;
    }

    while (pQueue->nReadIndex < pQueue->nQueuedCount)
    {
        uint8_t* pSlot = &pQueue->pBuffer[pQueue->nReadIndex * 32];
        pQueue->nReadIndex += 1;

        const uint8_t bOpcode = pSlot[0];
        if (bOpcode < 0xAF && sgptSCmdTable[bOpcode].pfUnitHandler != nullptr)
        {
            sgptSCmdTable[bOpcode].pfUnitHandler(pUnit, pSlot);
        }
    }

    pQueue->nQueuedCount = 0;
    pQueue->nReadIndex = 0;
}

// =============================================================================
// SECTION 2: Core S->C Packet Handlers (0x01 .. 0xAE)
// =============================================================================

// 0x6FB18020 (0x00: S2C_GAMELOADING - 1 byte)
void __fastcall SCMD_Handler00_GameLoading(uint8_t* pPacket)
{
    (void)pPacket;
}

// 0x6FB18000 (0x01: S2C_GAMELOGONRECEIPT - 8 bytes)
// Layout: [0x01, uint8_t nDifficulty, uint16_t wUnknown, uint8_t bHardcore, uint8_t bExpansion, uint8_t bLadder]
void __fastcall SCMD_Handler01_GameLogonReceipt(uint8_t* pPacket)
{
    gnClientDifficulty  = pPacket[1];
    gdwArenaFlags      |= (*reinterpret_cast<uint16_t*>(&pPacket[2])) | (pPacket[4] != 0 ? 0x800 : 0);
    gbIsExpansionClient = (pPacket[6] != 0) ? 1 : 0;
    gbIsLadderClient    = (pPacket[7] != 0) ? 1 : 0;
}

// 0x6FB14FF0 (0x02: S2C_LOADDONE - 1 byte)
// Replies with C->S 0x6B (C2S_JOINACTCOMPLETE) so the server begins streaming S->C 0x03/0x07/0x15.
void __fastcall SCMD_Handler02_LoadDone(uint8_t* pPacket)
{
    (void)pPacket;
    uint8_t bReply = 0x6B;
    CLIENT_Send(1, 0, &bReply);
}

// 0x6FB160F0 (0x03: S2C_LOADACT - 12 bytes)
// Layout: [0x03, uint8_t nAct, uint32_t dwMapSeed, uint16_t wTownLevelId, uint32_t dwUnkFlags]
void __fastcall SCMD_Handler03_LoadAct(uint8_t* pPacket)
{
    const uint8_t  nAct      = pPacket[1];
    const uint32_t dwMapSeed = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const uint16_t wLevelId  = *reinterpret_cast<uint16_t*>(&pPacket[6]);
    const uint32_t dwFlags   = *reinterpret_cast<uint32_t*>(&pPacket[8]);

    GAME_LoadAct(nAct, dwMapSeed, wLevelId, dwFlags);
}

// 0x6FB16550 (0x04: S2C_LOADCOMPLETE - 1 byte)
void __fastcall SCMD_Handler04_LoadComplete(uint8_t* pPacket)
{
    (void)pPacket;
    gbActLoadedFlag = 1;
    gbActUnloadedFlag = 0;

    D2UnitStrc* pPlayer = UNIT_GetClientPlayer();
    if (pPlayer != nullptr && pPlayer->pPlayerData != nullptr)
    {
        // Clear pending movement/action slots at pPlayerData+0x150..+0x15C
        uint32_t* pPending = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(pPlayer->pPlayerData) + 0x150);
        pPending[0] = 0;
        pPending[1] = 0;
        pPending[2] = 0;
        pPending[3] = 0;
    }
}

// 0x6FB15550 (0x05: S2C_UNLOADCOMPLETE - 1 byte)
void __fastcall SCMD_Handler05_UnloadComplete(uint8_t* pPacket)
{
    (void)pPacket;
    gbActLoadedFlag = 0;
    gbActUnloadedFlag = 1;
}

// 0x6FB157A0 (0x06: S2C_GAMELOGOUTSUCCESS - 1 byte)
void __fastcall SCMD_Handler06_GameLogoutSuccess(uint8_t* pPacket)
{
    (void)pPacket;
}

// 0x6FB14920 (0x07: S2C_ADDROOM - 6 bytes)
// Layout: [0x07, uint16_t wRoomTileX, uint16_t wRoomTileY, uint8_t nLevelId]
// Marks the room at (wRoomTileX, wRoomTileY) in level nLevelId as in-sight for the client DRLG
// via D2Common.#10184 (DUNGEON_SetClientIsInSight).
void __fastcall SCMD_Handler07_AddRoom(uint8_t* pPacket)
{
    if (gpClientAct == nullptr)
    {
        return;
    }

    D2UnitStrc* pPlayer = UNIT_GetClientPlayer();
    D2RoomStrc* pPlayerRoom = (pPlayer != nullptr) ? UNITS_GetRoom(pPlayer) : nullptr;

    const int32_t nTileX   = *reinterpret_cast<uint16_t*>(&pPacket[1]);
    const int32_t nTileY   = *reinterpret_cast<uint16_t*>(&pPacket[3]);
    const int32_t nLevelId = pPacket[5];

    if (D2DrlgStrc* pDrlg = DUNGEON_GetDrlgFromAct(gpClientAct))
    {
        D2DrlgLevelStrc* pLevel = DRLG_GetLevel(pDrlg, nLevelId);
        if (pLevel != nullptr && pLevel->pFirstRoomEx == nullptr)
        {
            DRLG_InitLevel(pLevel);
        }
    }

    // D2Common.#10063: DUNGEON_SetClientIsInSight + ensure DT1 tiles & active room are streamed
    DUNGEON_SetClientIsInSight(gpClientAct, nLevelId, nTileX, nTileY, pPlayerRoom);
    DUNGEON_StreamRoomAtCoords(gpClientAct, nTileX, nTileY);
}

// 0x6FB148B0 (0x08: S2C_REMOVEROOM - 6 bytes)
// Layout: [0x08, uint16_t wRoomTileX, uint16_t wRoomTileY, uint8_t nLevelId]
// Unsets the client-in-sight flag on the room via D2Common.#11009 (DUNGEON_UnsetClientIsInSight).
// Inactive rooms are subsequently freed every 13th tick by DUNGEON_UpdateAndFreeInactiveRooms.
void __fastcall SCMD_Handler08_RemoveRoom(uint8_t* pPacket)
{
    if (gpClientAct == nullptr)
    {
        return;
    }

    D2UnitStrc* pPlayer = UNIT_GetClientPlayer();
    D2RoomStrc* pPlayerRoom = (pPlayer != nullptr) ? UNITS_GetRoom(pPlayer) : nullptr;

    const int32_t nTileX   = *reinterpret_cast<uint16_t*>(&pPacket[1]);
    const int32_t nTileY   = *reinterpret_cast<uint16_t*>(&pPacket[3]);
    const int32_t nLevelId = pPacket[5];

    // D2Common.#11009: DUNGEON_UnsetClientIsInSight
    DUNGEON_UnsetClientIsInSight(gpClientAct, nLevelId, nTileX, nTileY, pPlayerRoom);
}

// 0x6FB164E0 (0x09: S2C_ASSIGNWARP - 11 bytes)
// Layout: [0x09, uint8_t nUnitType(5), uint32_t dwUnitGUID, uint8_t nWarpClassId, uint16_t wX, uint16_t wY]
// Allocates a UNIT_TILE (5) via D2Common.#11157 (UNITS_AllocUnit) and registers it in the room.
void __fastcall SCMD_Handler09_AssignWarp(uint8_t* pPacket)
{
    const uint8_t  nUnitType = pPacket[1];
    const uint32_t dwGUID    = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const uint16_t wWarpId   = pPacket[6];
    const int32_t  nX        = *reinterpret_cast<uint16_t*>(&pPacket[7]);
    const int32_t  nY        = *reinterpret_cast<uint16_t*>(&pPacket[9]);

    if (nUnitType != UNIT_TILE)
    {
        return;
    }

    D2UnitStrc* pWarpUnit = UNITS_AllocUnit(nullptr, UNIT_TILE);
    if (pWarpUnit != nullptr)
    {
        pWarpUnit->dwUnitId = dwGUID;
        UNIT_AssignClientWarp(pWarpUnit, wWarpId, nX, nY);
    }
}

// 0x6FB154F0 (0x0A: S2C_REMOVEUNIT - 6 bytes)
// Layout: [0x0A, uint8_t nUnitType, uint32_t dwUnitGUID]
void __fastcall SCMD_Handler0A_RemoveUnit(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);

    UNIT_RemoveClientUnit(nUnitType, dwUnitGUID);
}

// 0x6FB17B60 .. 0x6FB17A30 (0x0C .. 0x10: Deferred Unit Event Handlers)
void __fastcall SCMD_UnitHandler0C_GameHandshake(D2UnitStrc* pUnit, uint8_t* pPacket)
{
    const int32_t nTargetX = *reinterpret_cast<uint16_t*>(&pPacket[6]);
    const int32_t nTargetY = 0;
    UNITMODE_DispatchClientEvent(pUnit, pPacket[8], nTargetX, nTargetY, nullptr, 0, 0);
}

void __fastcall SCMD_UnitHandler0D_PlayerStop(D2UnitStrc* pUnit, uint8_t* pPacket)
{
    // D2GSPacketSrv0D (13B): [0]=0x0D, [1]=nUnitType, [2..5]=dwGUID, [6]=nHitClass, [7..8]=wX, [9..10]=wY, [11]=nHitType, [12]=nLifePct
    const int32_t nX = *reinterpret_cast<uint16_t*>(&pPacket[7]);
    const int32_t nY = *reinterpret_cast<uint16_t*>(&pPacket[9]);

    UNITMODE_CheckPositionDrift(pUnit, nX, nY);
    UNITMODE_DispatchClientEvent(pUnit, 0x0D, nX, nY, nullptr, 0, 0);
}

void __fastcall SCMD_UnitHandler0F_PlayerMove(D2UnitStrc* pUnit, uint8_t* pPacket)
{
    // D2GSPacketSrv0F (16B): [0]=0x0F, [1]=nUnitType, [2..5]=dwGUID, [6]=nMoveType, [7..8]=wTargetX, [9..10]=wTargetY, [11]=unk, [12..13]=wCurX, [14..15]=wCurY
    const int32_t nTargetX = *reinterpret_cast<uint16_t*>(&pPacket[7]);
    const int32_t nTargetY = *reinterpret_cast<uint16_t*>(&pPacket[9]);
    const int32_t nServerX = *reinterpret_cast<uint16_t*>(&pPacket[12]);
    const int32_t nServerY = *reinterpret_cast<uint16_t*>(&pPacket[14]);

    UNITMODE_CheckPositionDrift(pUnit, nServerX, nServerY);
    UNITMODE_DispatchClientEvent(pUnit, pPacket[6], nTargetX, nTargetY, nullptr, 0, 0);
}

void __fastcall SCMD_UnitHandler10_PlayerToTarget(D2UnitStrc* pUnit, uint8_t* pPacket)
{
    // D2GSPacketSrv10 (16B): [0]=0x10, [1]=nUnitType, [2..5]=dwGUID, [6]=nMoveType, [7]=nTargetType, [8..11]=dwTargetGUID, [12..13]=wCurX, [14..15]=wCurY
    const uint32_t dwTargetType = pPacket[7];
    const uint32_t dwTargetGUID = *reinterpret_cast<uint32_t*>(&pPacket[8]);
    const int32_t  nServerX     = *reinterpret_cast<uint16_t*>(&pPacket[12]);
    const int32_t  nServerY     = *reinterpret_cast<uint16_t*>(&pPacket[14]);

    D2UnitStrc* pTarget = UNIT_FindClientUnit(dwTargetGUID, dwTargetType);
    UNITMODE_CheckPositionDrift(pUnit, nServerX, nServerY);
    UNITMODE_DispatchClientEvent(pUnit, pPacket[6], 0, 0, pTarget, 0, 0);
}

// 0x6FB17BB0 (0x15: S2C_REASSIGNPLAYER - 11 bytes)
// Layout: [0x15, uint8_t nUnitType, uint32_t dwUnitGUID, uint16_t wX, uint16_t wY, uint8_t bSnap]
// Hard-teleports or rubberbands a unit to (wX, wY) and resets its collision mask:
//   - Players: D2Common.#10209 (PATH_SetMoveTestCollisionMask(pPath, 0x1C09))
//   - Alive Monsters: D2Common.#10144 (PATH_SetFootprintCollisionMask(pPath, 0x0100))
void __fastcall SCMD_Handler15_ReassignPlayer(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const int32_t  nX         = *reinterpret_cast<uint16_t*>(&pPacket[6]);
    const int32_t  nY         = *reinterpret_cast<uint16_t*>(&pPacket[8]);
    const uint8_t  bSnap      = pPacket[10];

    D2UnitStrc* pUnit = UNIT_FindClientUnit(dwUnitGUID, nUnitType);
    if (pUnit == nullptr)
    {
        return;
    }

    if (bSnap != 0 || pUnit == UNIT_GetClientPlayer())
    {
        UNIT_SetClientUnitPosition(pUnit, nX, nY);
    }

    if (pUnit->pDynamicPath != nullptr)
    {
        if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            // D2Common.#10209: PATH_SetMoveTestCollisionMask
            PATH_SetMoveTestCollisionMask(pUnit->pDynamicPath, 0x1C09);
        }
        else if (pUnit->dwUnitType == UNIT_MONSTER &&
                 pUnit->dwAnimMode != MONMODE_DEAD &&
                 pUnit->dwAnimMode != MONMODE_DEATH)
        {
            // D2Common.#10144: PATH_SetFootprintCollisionMask
            PATH_SetFootprintCollisionMask(pUnit->pDynamicPath, 0x0100);
        }
    }

    if (pUnit == UNIT_GetClientPlayer())
    {
        gdwReassignGraceTick = GetTickCount() + 3000;
    }
}

// 0x6FB172E0 / 0x6FB17220 / 0x6FB15EC0 (0x18, 0x95, 0x96: Bitstream HP/Mana/Stamina/Position Sync)
// Wire format verified against D2Game/src/GAME/SCmd.cpp (lines 905-960):
//   - 0x18 (15 bytes): 8b hdr, 15b HP, 15b Mana, 15b Stamina, 7b HPRegen, 7b ManaRegen, 16b X, 16b Y, 8b vX, 8b vY
//   - 0x95 (13 bytes): 8b hdr, 15b HP, 15b Mana, 15b Stamina, 16b X, 16b Y, 8b vX, 8b vY
//   - 0x96 (9 bytes):  8b hdr, 15b Stamina, 16b X, 16b Y, 8b vX, 8b vY
void __fastcall SCMD_Handler18_PlayerHPMP(uint8_t* pPacket)
{
    D2UnitStrc* pPlayer = UNIT_GetClientPlayer();
    if (pPlayer == nullptr)
    {
        return;
    }

    D2BitBufferStrc bitBuf = {};
    BITMANIP_Initialize(&bitBuf, pPacket, 15);
    BITMANIP_Read(&bitBuf, 8);
    const int32_t nHP      = BITMANIP_Read(&bitBuf, 15) << 8;
    const int32_t nMana    = BITMANIP_Read(&bitBuf, 15) << 8;
    const int32_t nStamina = BITMANIP_Read(&bitBuf, 15) << 8;
    BITMANIP_Read(&bitBuf, 7);
    BITMANIP_Read(&bitBuf, 7);
    const int32_t nServerX = BITMANIP_Read(&bitBuf, 16);
    const int32_t nServerY = BITMANIP_Read(&bitBuf, 16);

    STATLIST_SetUnitStat(pPlayer, STAT_HITPOINTS, nHP, 0);
    STATLIST_SetUnitStat(pPlayer, STAT_MANA, nMana, 0);
    STATLIST_SetUnitStat(pPlayer, STAT_STAMINA, nStamina, 0);
    if (nServerX > 0 && nServerY > 0)
    {
        UNITMODE_CheckPositionDrift(pPlayer, nServerX, nServerY);
    }
}

void __fastcall SCMD_Handler95_PlayerLifeManaChange(uint8_t* pPacket)
{
    D2UnitStrc* pPlayer = UNIT_GetClientPlayer();
    if (pPlayer == nullptr)
    {
        return;
    }

    D2BitBufferStrc bitBuf = {};
    BITMANIP_Initialize(&bitBuf, pPacket, 13);
    BITMANIP_Read(&bitBuf, 8);
    const int32_t nHP      = BITMANIP_Read(&bitBuf, 15) << 8;
    const int32_t nMana    = BITMANIP_Read(&bitBuf, 15) << 8;
    const int32_t nStamina = BITMANIP_Read(&bitBuf, 15) << 8;
    const int32_t nServerX = BITMANIP_Read(&bitBuf, 16);
    const int32_t nServerY = BITMANIP_Read(&bitBuf, 16);

    STATLIST_SetUnitStat(pPlayer, STAT_HITPOINTS, nHP, 0);
    STATLIST_SetUnitStat(pPlayer, STAT_MANA, nMana, 0);
    STATLIST_SetUnitStat(pPlayer, STAT_STAMINA, nStamina, 0);
    if (nServerX > 0 && nServerY > 0)
    {
        UNITMODE_CheckPositionDrift(pPlayer, nServerX, nServerY);
    }
}

void __fastcall SCMD_Handler96_WalkVerify(uint8_t* pPacket)
{
    D2UnitStrc* pPlayer = UNIT_GetClientPlayer();
    if (pPlayer == nullptr)
    {
        return;
    }

    D2BitBufferStrc bitBuf = {};
    BITMANIP_Initialize(&bitBuf, pPacket, 9);
    BITMANIP_Read(&bitBuf, 8);
    const int32_t nStamina = BITMANIP_Read(&bitBuf, 15) << 8;
    const int32_t nServerX = BITMANIP_Read(&bitBuf, 16);
    const int32_t nServerY = BITMANIP_Read(&bitBuf, 16);

    STATLIST_SetUnitStat(pPlayer, STAT_STAMINA, nStamina, 0);
    if (nServerX > 0 && nServerY > 0)
    {
        UNITMODE_CheckPositionDrift(pPlayer, nServerX, nServerY);
    }
}

// 0x6FB170C0
// Post-stat update hook invoked whenever STAT_HITPOINTS (6), STAT_MAXHP (7), or STAT_LEVEL (12) changes.
void __fastcall SCMD_OnPlayerStatUpdated(D2UnitStrc* pPlayer, int32_t nStatId, int32_t nNewValue)
{
    (void)pPlayer;
    (void)nStatId;
    (void)nNewValue;
}

// 0x6FB17430 (0x19 .. 0x1F: Unified Stat Update Handler)
// Handles all 7 stat update opcodes:
//   - 0x19 (2 bytes):  Add uint8_t  to STAT_GOLD (14)
//   - 0x1A (2 bytes):  Add uint8_t  to STAT_EXPERIENCE (13)
//   - 0x1B (3 bytes):  Add uint16_t to STAT_EXPERIENCE (13)
//   - 0x1C (5 bytes):  Set uint32_t on STAT_EXPERIENCE (13)
//   - 0x1D (3 bytes):  Set uint8_t  on stat `pPacket[1]` (shifted << 8 for fixed-point stats 6..11)
//   - 0x1E (4 bytes):  Set uint16_t on stat `pPacket[1]` (shifted << 8 for fixed-point stats 6..11)
//   - 0x1F (6 bytes):  Set int32_t  on stat `pPacket[1]` (shifted << 8 for fixed-point stats 6..11)
void __fastcall SCMD_Handler19_1F_StatUpdate(uint8_t* pPacket)
{
    D2UnitStrc* pPlayer = UNIT_GetClientPlayer();
    if (pPlayer == nullptr)
    {
        return;
    }

    const uint8_t bOpcode = pPacket[0];
    int32_t nStatId = 0;
    int32_t nValue  = 0;

    switch (bOpcode)
    {
    case 0x19: // S2C_GOLDTOINV_BYTE (2 bytes)
        STATLIST_AddUnitStat(pPlayer, STAT_GOLD, pPacket[1], 0);
        return;

    case 0x1A: // S2C_ADDEXP_BYTE (2 bytes)
        STATLIST_AddUnitStat(pPlayer, STAT_EXPERIENCE, pPacket[1], 0);
        return;

    case 0x1B: // S2C_ADDEXP_WORD (3 bytes)
        STATLIST_AddUnitStat(pPlayer, STAT_EXPERIENCE, *reinterpret_cast<uint16_t*>(&pPacket[1]), 0);
        return;

    case 0x1C: // S2C_ADDEXP_DWORD (5 bytes)
        STATLIST_SetUnitStat(pPlayer, STAT_EXPERIENCE, *reinterpret_cast<int32_t*>(&pPacket[1]), 0);
        return;

    case 0x1D: // S2C_SETATTR_BYTE (3 bytes: [0x1D, nStatId, uint8_t nVal])
        nStatId = pPacket[1];
        nValue  = pPacket[2];
        break;

    case 0x1E: // S2C_SETATTR_WORD (4 bytes: [0x1E, nStatId, uint16_t wVal])
        nStatId = pPacket[1];
        nValue  = *reinterpret_cast<uint16_t*>(&pPacket[2]);
        break;

    case 0x1F: // S2C_SETATTR_DWORD (6 bytes: [0x1F, nStatId, int32_t dwVal])
        nStatId = pPacket[1];
        nValue  = *reinterpret_cast<int32_t*>(&pPacket[2]);
        break;

    default:
        return;
    }

    // Fixed-point 24.8 stats: STAT_HITPOINTS(6), STAT_MAXHP(7), STAT_MANA(8),
    // STAT_MAXMANA(9), STAT_STAMINA(10), STAT_MAXSTAMINA(11)
    if (nStatId >= STAT_HITPOINTS && nStatId <= STAT_MAXSTAMINA)
    {
        nValue <<= 8;
    }

    STATLIST_SetUnitStat(pPlayer, nStatId, nValue, 0);
    SCMD_OnPlayerStatUpdated(pPlayer, nStatId, nValue);
}

// 0x6FB173C0 (0x20: S2C_PLAYERATTRUPDATE - 10 bytes)
// Layout: [0x20, uint32_t dwUnitGUID, uint8_t nStatId, int32_t nValue]
void __fastcall SCMD_Handler20_PlayerAttrUpdate(uint8_t* pPacket)
{
    const uint32_t dwGUID  = *reinterpret_cast<uint32_t*>(&pPacket[1]);
    const int32_t  nStatId = pPacket[5];
    int32_t        nValue  = *reinterpret_cast<int32_t*>(&pPacket[6]);

    D2UnitStrc* pUnit = UNIT_FindClientUnit(dwGUID, UNIT_PLAYER);
    if (pUnit == nullptr)
    {
        return;
    }

    if (nStatId >= STAT_HITPOINTS && nStatId <= STAT_MAXSTAMINA)
    {
        nValue <<= 8;
    }

    STATLIST_SetUnitStat(pUnit, nStatId, nValue, 0);
    if (pUnit == UNIT_GetClientPlayer())
    {
        SCMD_OnPlayerStatUpdated(pUnit, nStatId, nValue);
    }
}

// 0x6FB164B0 (0x51: S2C_ASSIGNOBJECT - 14 bytes)
// Layout: [0x51, uint8_t nUnitType(2), uint32_t dwGUID, uint16_t wObjectClassId,
//          uint16_t wX, uint16_t wY, uint8_t nAnimMode, uint8_t nInteractType]
void __fastcall SCMD_Handler51_AssignObject(uint8_t* pPacket)
{
    UNIT_AssignClientObject(pPacket);
}

// 0x6FB16470 (0x59: S2C_ASSIGNPLAYER - 26 bytes)
// Layout: [0x59, uint32_t dwPlayerGUID, uint8_t nCharClass, char szName[16], uint16_t wX, uint16_t wY]
void __fastcall SCMD_Handler59_AssignPlayer(uint8_t* pPacket)
{
    UNIT_AssignClientPlayer(pPacket);
}

// 0x6FB179D0 .. 0x6FB17820 (0x67 .. 0x6D: Deferred Monster Movement & Attack Unit Handlers)
// All 7 opcodes are queued onto the target monster's `pUnit->pPacketQueue` (+0xD8)
// and drained during `UNITMODE_UpdateClientUnit` via `SCMD_DrainUnitPacketQueue`.
//
//   - 0x67 (16 bytes): S2C_MONSTERMOVE
//     [0x67, uint32_t dwMonGUID, uint8_t nMoveType(0x01=Walk, 0x17=Run), uint16_t wTargetX, uint16_t wTargetY, ...]
//   - 0x68 (21 bytes): S2C_MONSTERMOVETOTARGET
//     [0x68, uint32_t dwMonGUID, uint8_t nMoveType, uint16_t wCurX, uint16_t wCurY, uint8_t nTargetType, uint32_t dwTargetGUID, ...]
//   - 0x69 (12 bytes): S2C_MONSTERSTATE
//     [0x69, uint32_t dwMonGUID, uint8_t nStateMode, uint16_t wX, uint16_t wY, ...]
//   - 0x6A (12 bytes): S2C_MONSTERUNK6A
//   - 0x6B (16 bytes): S2C_MONSTERACTION (Attack / Skill / Cast)
//   - 0x6C (16 bytes): S2C_MONSTERATTACKONUNIT
//   - 0x6D (10 bytes): S2C_MONSTERSTOP
//     [0x6D, uint32_t dwMonGUID, uint16_t wX, uint16_t wY, uint8_t nLifePct]
void __fastcall SCMD_UnitHandler67_MonsterMove(D2UnitStrc* pMonster, uint8_t* pPacket)
{
    const uint8_t nEventType = pPacket[5];
    const int32_t nTargetX   = *reinterpret_cast<uint16_t*>(&pPacket[6]);
    const int32_t nTargetY   = *reinterpret_cast<uint16_t*>(&pPacket[8]);

    UNITMODE_DispatchClientEvent(pMonster, nEventType, nTargetX, nTargetY, nullptr, 0, 0);
}

void __fastcall SCMD_UnitHandler68_MonsterMoveToTarget(D2UnitStrc* pMonster, uint8_t* pPacket)
{
    const uint8_t  nEventType    = pPacket[5];
    const int32_t  nCurX         = *reinterpret_cast<uint16_t*>(&pPacket[6]);
    const int32_t  nCurY         = *reinterpret_cast<uint16_t*>(&pPacket[8]);
    const uint8_t  nTargetType   = pPacket[10];
    const uint32_t dwTargetGUID  = *reinterpret_cast<uint32_t*>(&pPacket[11]);

    UNITMODE_CheckPositionDrift(pMonster, nCurX, nCurY);
    D2UnitStrc* pTarget = UNIT_FindClientUnit(dwTargetGUID, nTargetType);
    UNITMODE_DispatchClientEvent(pMonster, nEventType, nCurX, nCurY, pTarget, 0, 0);
}

void __fastcall SCMD_UnitHandler69_MonsterState(D2UnitStrc* pMonster, uint8_t* pPacket)
{
    const uint8_t nMode = pPacket[5];
    const int32_t nX    = *reinterpret_cast<uint16_t*>(&pPacket[6]);
    const int32_t nY    = *reinterpret_cast<uint16_t*>(&pPacket[8]);

    UNIT_SetClientUnitPosition(pMonster, nX, nY);
    UNITMODE_DispatchClientEvent(pMonster, nMode, nX, nY, nullptr, 0, 0);
}

void __fastcall SCMD_UnitHandler6A_MonsterUnk6A(D2UnitStrc* pMonster, uint8_t* pPacket)
{
    const uint8_t  nMode        = pPacket[5];
    const uint8_t  nTargetType  = pPacket[6];
    const uint32_t dwTargetGUID = *reinterpret_cast<uint32_t*>(&pPacket[7]);

    D2UnitStrc* pTarget = UNIT_FindClientUnit(dwTargetGUID, nTargetType);
    UNITMODE_DispatchClientEvent(pMonster, nMode, 0, 0, pTarget, 0, 0);
}

void __fastcall SCMD_UnitHandler6B_MonsterAction(D2UnitStrc* pMonster, uint8_t* pPacket)
{
    const uint8_t nAction  = pPacket[5];
    const int32_t nTargetX = *reinterpret_cast<uint16_t*>(&pPacket[12]);
    const int32_t nTargetY = *reinterpret_cast<uint16_t*>(&pPacket[14]);

    UNITMODE_DispatchClientEvent(pMonster, nAction, nTargetX, nTargetY, nullptr, 0, 0);
}

void __fastcall SCMD_UnitHandler6C_MonsterAttackUnit(D2UnitStrc* pMonster, uint8_t* pPacket)
{
    const uint8_t  nAction      = pPacket[5];
    const uint8_t  nTargetType  = pPacket[7];
    const uint32_t dwTargetGUID = *reinterpret_cast<uint32_t*>(&pPacket[8]);
    const int32_t  nCurX        = *reinterpret_cast<uint16_t*>(&pPacket[12]);
    const int32_t  nCurY        = *reinterpret_cast<uint16_t*>(&pPacket[14]);

    UNITMODE_CheckPositionDrift(pMonster, nCurX, nCurY);
    D2UnitStrc* pTarget = UNIT_FindClientUnit(dwTargetGUID, nTargetType);
    UNITMODE_DispatchClientEvent(pMonster, nAction, nCurX, nCurY, pTarget, 0, 0);
}

void __fastcall SCMD_UnitHandler6D_MonsterStop(D2UnitStrc* pMonster, uint8_t* pPacket)
{
    const int32_t nX       = *reinterpret_cast<uint16_t*>(&pPacket[5]);
    const int32_t nY       = *reinterpret_cast<uint16_t*>(&pPacket[7]);
    const uint8_t nLifePct = pPacket[9];

    UNITMODE_CheckPositionDrift(pMonster, nX, nY);
    STATLIST_SetUnitStat(pMonster, STAT_HITPOINTS, nLifePct, 0);
    UNITMODE_DispatchClientEvent(pMonster, 0x09, nX, nY, nullptr, 0, 0);
}

// 0x6FB165F0 (0x9C: S2C_ITEMACTIONWORLD - variable length)
// 0x6FB16DE0 (0x9D: S2C_ITEMACTIONOWNED - variable length)
// Decodes the item bitstream via D2Common.#10903 (ITEMS_DecodeItemFromPacket) and executes
// a 24-way action switch (0x00 .. 0x17) for ground/inventory/buffer/cursor/equipped/socket/shop placement:
//   0x00: AddToGround          0x01: GroundToCursor      0x02: DropToGround
//   0x03: OnGround             0x04: PutInContainer      0x05: RemoveFromContainer (0x9D)
//   0x06: Equip (0x9D)         0x07: IndirectlySwapBody  0x08: Unequip (0x9D)
//   0x09: SwapBodyItem (0x9D)  0x0A: AddQuantity         0x0B:AddToShop
//   0x0C: RemoveFromShop       0x0D: SwapInContainer     0x0E: PutInBelt
//   0x0F: RemoveFromBelt       0x10: SwapInBelt          0x11: AutoUnequip (0x9D)
//   0x12: RemoveFromCursor     0x13: ItemInSocket        0x14: UpdateStats
//   0x15: UpdateStatsEx        0x16: WeaponSwitch        0x17: SwapCursorBuffer
void __fastcall SCMD_Handler9C_ItemActionWorld(uint8_t* pPacket)
{
    D2Client_ProcessItemPacket9C(pPacket);
}

void __fastcall SCMD_Handler9D_ItemActionOwned(uint8_t* pPacket)
{
    D2Client_ProcessItemPacket9D(pPacket);
}

// 0x6FB16990 (0xAC: S2C_ASSIGNNPC - variable length)
// Fixed header + bitstream payload:
//   +0x00: uint8_t  bOpcode (0xAC)
//   +0x01: uint32_t dwUnitGUID
//   +0x05: uint16_t wMonsterClassId (MonStats.txt HCIdx)
//   +0x07: uint16_t wX
//   +0x09: uint16_t wY
//   +0x0B: uint8_t  nLifePercentage (0..128)
//   +0x0C: uint8_t  nTotalPacketLength
//   +0x0D: Bitstream payload (D2BitBufferStrc initialized at &pPacket[13]):
//          1. 4 bits: Animation mode (MONMODE_*)
//          2. 1 bit:  Has composite equipment layers (`MonStats2.txt` component counts read via `bsr` bit width)
//          3. 1 bit:  Has Monster flags / Boss modifiers:
//             - 5 single-bit flags (Champion, Unique, SuperUnique, Minion, Ghostly)
//             - If SuperUnique: 16-bit SuperUnique ID
//             - Loop reading 8-bit `MONUMOD_*` IDs until 0 terminator
//          4. Optional UTF-8/localized monster name string
void __fastcall SCMD_HandlerAC_AssignNPC(uint8_t* pPacket)
{
    const uint32_t dwGUID      = *reinterpret_cast<uint32_t*>(&pPacket[1]);
    const uint16_t wClassId    = *reinterpret_cast<uint16_t*>(&pPacket[5]);
    const int32_t  nX          = *reinterpret_cast<uint16_t*>(&pPacket[7]);
    const int32_t  nY          = *reinterpret_cast<uint16_t*>(&pPacket[9]);
    const uint8_t  nLifePct    = pPacket[11];
    const uint8_t  nPacketSize = pPacket[12];

    uint8_t nAnimMode       = MONMODE_NEUTRAL;
    uint8_t abCompInfo[16]  = { 0 };
    uint8_t abBossMods[10]  = { 0 };
    int32_t nMonFlags       = 0;
    uint16_t wBossHcIdx     = 0;

    if (nPacketSize > 13)
    {
        D2BitBufferStrc bitStream = { 0 };
        BITMANIP_Initialize(&bitStream, &pPacket[13], static_cast<int32_t>(nPacketSize) - 13);

        nAnimMode = static_cast<uint8_t>(BITMANIP_Read(&bitStream, 4));
        if (BITMANIP_Read(&bitStream, 1) != 0)
        {
            D2MonStats2Txt* pMonStats2 = UNITS_GetMonStats2TxtRecordFromMonsterId(wClassId);
            if (pMonStats2 != nullptr)
            {
                for (int32_t c = 0; c < 16; ++c)
                {
                    uint8_t nChoices = pMonStats2->nComponentChoiceCounts[c];
                    if (nChoices <= 2)
                    {
                        abCompInfo[c] = (nChoices > 0) ? 0 : 0;
                    }
                    else
                    {
                        int32_t nBits = 1;
                        uint32_t v = static_cast<uint32_t>(nChoices - 1);
                        while ((v >>= 1) != 0) { ++nBits; }
                        abCompInfo[c] = static_cast<uint8_t>(BITMANIP_Read(&bitStream, nBits));
                    }
                }
            }
        }

        if (BITMANIP_Read(&bitStream, 1) != 0)
        {
            if (BITMANIP_Read(&bitStream, 1)) nMonFlags |= 0x04; // Champion
            if (BITMANIP_Read(&bitStream, 1)) nMonFlags |= 0x08; // Unique
            if (BITMANIP_Read(&bitStream, 1)) nMonFlags |= 0x02; // SuperUnique
            if (BITMANIP_Read(&bitStream, 1)) nMonFlags |= 0x10; // Minion
            if (BITMANIP_Read(&bitStream, 1)) nMonFlags |= 0x20; // Ghostly/Possessed
            if (nMonFlags & 0x02)
            {
                wBossHcIdx = static_cast<uint16_t>(BITMANIP_Read(&bitStream, 16));
            }
            for (int32_t m = 0; m < 9; ++m)
            {
                uint8_t nMod = static_cast<uint8_t>(BITMANIP_Read(&bitStream, 8));
                abBossMods[m] = nMod;
                if (nMod == 0)
                {
                    break;
                }
            }
        }
    }

    D2UnitStrc* pMonster = UNIT_AssignClientMonster(
        dwGUID,
        wClassId,
        nX,
        nY,
        nAnimMode,
        abCompInfo,
        nMonFlags,
        abBossMods,
        nullptr,
        static_cast<int32_t>(wBossHcIdx));

    if (pMonster != nullptr)
    {
        STATLIST_SetUnitStat(pMonster, STAT_MAXHP, 128 << 8, 0);
        STATLIST_SetUnitStat(pMonster, STAT_HITPOINTS, static_cast<int32_t>(nLifePct) << 8, 0);
    }
}

// Additional Reconstructed S->C Packet Handlers (0x0B .. 0xAE)

static void __fastcall SCMD_Handler0B_GameHandshake(uint8_t* pPacket)
{
    (void)pPacket;
}

static void __fastcall SCMD_Handler0E_ObjectState(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwObjGUID  = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const int32_t  nAnimMode  = *reinterpret_cast<int32_t*>(&pPacket[8]);

    D2UnitStrc* pObj = UNIT_FindClientUnit(static_cast<int32_t>(dwObjGUID), nUnitType);
    if (pObj != nullptr)
    {
        pObj->dwAnimMode = nAnimMode;
        UNITS_ChangeAnimMode(pObj, nAnimMode);
    }
}

static void __fastcall SCMD_Handler11_ReportKill(uint8_t* pPacket)
{
    const uint8_t  nUnitType = pPacket[1];
    const uint32_t dwGUID    = *reinterpret_cast<uint32_t*>(&pPacket[2]);

    D2UnitStrc* pUnit = UNIT_FindClientUnit(static_cast<int32_t>(dwGUID), nUnitType);
    if (pUnit != nullptr && pUnit->dwUnitType == UNIT_MONSTER)
    {
        UNITS_ChangeAnimMode(pUnit, MONMODE_DEAD);
    }
}

static void __fastcall SCMD_Handler16_MultiUnitUpdate(uint8_t* pPacket)
{
    const uint8_t nCount = pPacket[3];
    const uint8_t* pEntry = &pPacket[4];
    for (uint8_t i = 0; i < nCount; ++i, pEntry += 9)
    {
        const uint8_t  nUnitType = pEntry[0];
        const uint32_t dwGUID    = *reinterpret_cast<const uint32_t*>(&pEntry[1]);
        const int32_t  nX        = *reinterpret_cast<const uint16_t*>(&pEntry[5]);
        const int32_t  nY        = *reinterpret_cast<const uint16_t*>(&pEntry[7]);
        D2UnitStrc* pUnit = UNIT_FindClientUnit(static_cast<int32_t>(dwGUID), nUnitType);
        if (pUnit != nullptr)
        {
            UNIT_SetClientUnitPosition(pUnit, nX, nY);
        }
    }
}

static void __fastcall SCMD_Handler_NoOp(uint8_t* pPacket)
{
    (void)pPacket;
}

static void __fastcall SCMD_Handler21_UpdateSkill(uint8_t* pPacket)
{
    // D2GSPacketSrv21 (12B): [0]=0x21, [1]=nUnitType, [2]=unk, [3..6]=dwGUID, [7..8]=wSkillId, [9]=nBaseLvl, [10]=nBonusLvl
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[3]);
    const uint16_t wSkillId   = *reinterpret_cast<uint16_t*>(&pPacket[7]);
    const uint8_t  nBaseLevel = pPacket[9];

    D2UnitStrc* pUnit = UNIT_FindClientUnit(static_cast<int32_t>(dwUnitGUID), nUnitType);
    if (pUnit == nullptr)
    {
        pUnit = UNIT_GetClientPlayer();
    }
    if (pUnit != nullptr)
    {
        SKILLS_AssignSkill(pUnit, wSkillId, nBaseLevel, FALSE, __FILE__, __LINE__);
    }
}

static void __fastcall SCMD_Handler22_UpdateItemSkill(uint8_t* pPacket)
{
    // D2GSPacketSrv22 (12B): [0]=0x22, [1]=nUnitType, [2..5]=dwGUID, [6..7]=wSkillId, [8]=nAmount
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const uint16_t wSkillId   = *reinterpret_cast<uint16_t*>(&pPacket[6]);
    const uint8_t  nAmount    = pPacket[8];

    D2UnitStrc* pUnit = UNIT_FindClientUnit(static_cast<int32_t>(dwUnitGUID), nUnitType);
    if (pUnit != nullptr)
    {
        D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, wSkillId);
        if (pSkill != nullptr)
        {
            SKILLS_SetQuantity(pSkill, nAmount);
        }
    }
}

static void __fastcall SCMD_Handler23_SetSkill(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const uint8_t  nHand      = pPacket[6];
    const uint16_t wSkillId   = *reinterpret_cast<uint16_t*>(&pPacket[7]);
    const int32_t  nItemGUID  = *reinterpret_cast<int32_t*>(&pPacket[9]);

    D2UnitStrc* pUnit = UNIT_FindClientUnit(static_cast<int32_t>(dwUnitGUID), nUnitType);
    if (pUnit == nullptr)
    {
        pUnit = UNIT_GetClientPlayer();
    }
    if (pUnit != nullptr)
    {
        if (SKILLS_GetSkillById(pUnit, wSkillId, nItemGUID) == nullptr)
        {
            SKILLS_AddSkill(pUnit, wSkillId);
        }
        if (nHand != 0)
        {
            SKILLS_SetLeftActiveSkill(pUnit, wSkillId, nItemGUID);
        }
        else
        {
            SKILLS_SetRightActiveSkill(pUnit, wSkillId, nItemGUID);
        }
    }
}

static void __fastcall SCMD_Handler26_GameChat(uint8_t* pPacket)
{
    sub_6FB23EE0(pPacket);
}

static void __fastcall SCMD_Handler27_NpcInfo(uint8_t* pPacket)
{
    D2Client_OnNpcInfoPacket(pPacket);
    sub_6FB21D70(pPacket);
}

static void __fastcall SCMD_Handler28_PlayerQuestInfo(uint8_t* pPacket)
{
    if (g_pPlayerQuestRecord != nullptr)
    {
        QUESTRECORD_CopyBufferToRecord(g_pPlayerQuestRecord, &pPacket[7], 96, FALSE);
    }
}

static void __fastcall SCMD_Handler29_GameQuestInfo(uint8_t* pPacket)
{
    if (g_pGameQuestRecord != nullptr)
    {
        QUESTRECORD_CopyBufferToRecord(g_pGameQuestRecord, &pPacket[1], 96, FALSE);
    }
}

static void __fastcall SCMD_Handler2C_PlaySound(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const uint16_t wSoundId   = *reinterpret_cast<uint16_t*>(&pPacket[6]);
    D2Client_PlayUnitSound(nUnitType, dwUnitGUID, wSoundId);
}

static void __fastcall SCMD_Handler42_ClearCursor(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    D2UnitStrc* pUnit = UNIT_FindClientUnit(static_cast<int32_t>(dwUnitGUID), nUnitType);
    if (pUnit != nullptr && pUnit->pInventory != nullptr)
    {
        INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);
    }
}

static void __fastcall SCMD_Handler4C_UnitSkillOnTarget(uint8_t* pPacket)
{
    const uint8_t  nUnitType    = pPacket[1];
    const uint32_t dwUnitGUID   = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const uint8_t  nTargetType  = pPacket[10];
    const uint32_t dwTargetGUID = *reinterpret_cast<uint32_t*>(&pPacket[11]);

    D2UnitStrc* pUnit   = UNIT_FindClientUnit(static_cast<int32_t>(dwUnitGUID), nUnitType);
    D2UnitStrc* pTarget = UNIT_FindClientUnit(static_cast<int32_t>(dwTargetGUID), nTargetType);
    if (pUnit != nullptr)
    {
        UNITMODE_DispatchClientEvent(pUnit, 0x03, 0, 0, pTarget, 0, 0);
    }
}

static void __fastcall SCMD_Handler4D_UnitCastSkill(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const int32_t  nX         = *reinterpret_cast<uint16_t*>(&pPacket[11]);
    const int32_t  nY         = *reinterpret_cast<uint16_t*>(&pPacket[13]);

    D2UnitStrc* pUnit = UNIT_FindClientUnit(static_cast<int32_t>(dwUnitGUID), nUnitType);
    if (pUnit != nullptr)
    {
        UNITMODE_DispatchClientEvent(pUnit, 0x02, nX, nY, nullptr, 0, 0);
    }
}

static void __fastcall SCMD_Handler58_OpenUIDialog(uint8_t* pPacket)
{
    const uint8_t nUIType = pPacket[5];
    switch (nUIType)
    {
    case 2: // Stash
        UI_SetPanelState(D2PANEL_STASH, 0, 0);
        UI_SetPanelState(D2PANEL_INVENTORY, 0, 0);
        break;
    case 3: // Horadric Cube
        UI_SetPanelState(D2PANEL_CUBE, 0, 0);
        UI_SetPanelState(D2PANEL_INVENTORY, 0, 0);
        break;
    default:
        break;
    }
}

static void __fastcall SCMD_Handler5B_PlayerInGame(uint8_t* pPacket)
{
    const uint32_t dwUnitId   = *reinterpret_cast<uint32_t*>(&pPacket[3]);
    const uint8_t  nClassId   = pPacket[7];
    char szName[17] = { 0 };
    memcpy(szName, &pPacket[8], 16);
    const uint16_t wLevel     = *reinterpret_cast<uint16_t*>(&pPacket[24]);
    const uint16_t wPartyId   = *reinterpret_cast<uint16_t*>(&pPacket[26]);

    ROSTER_AddOrUpdatePlayer(dwUnitId, szName, nClassId, wLevel, wPartyId, 1, 0, "", "");
}

static void __fastcall SCMD_Handler5C_PlayerLeftGame(uint8_t* pPacket)
{
    const uint32_t dwUnitId = *reinterpret_cast<uint32_t*>(&pPacket[1]);
    ROSTER_RemovePlayerFromRoster(dwUnitId);
}

static void __fastcall SCMD_Handler63_WaypointMenu(uint8_t* pPacket)
{
    (void)pPacket;
    UI_SetPanelState(D2PANEL_WAYPOINT, 0, 0);
}

static void __fastcall SCMD_Handler65_PlayerKillCount(uint8_t* pPacket)
{
    const uint32_t dwPlayerId = *reinterpret_cast<uint32_t*>(&pPacket[1]);
    const int16_t  nCount     = *reinterpret_cast<int16_t*>(&pPacket[5]);
    ROSTER_SetPlayerArenaScore(dwPlayerId, nCount);
}

static void __fastcall SCMD_Handler74_PlayerCorpseAssign(uint8_t* pPacket)
{
    const uint8_t  bAssign      = pPacket[1];
    const uint32_t dwPlayerGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const uint32_t dwCorpseGUID = *reinterpret_cast<uint32_t*>(&pPacket[6]);
    if (bAssign != 0)
    {
        ROSTER_AddPlayerCorpse(dwPlayerGUID, dwCorpseGUID);
    }
    else
    {
        ROSTER_RemovePlayerCorpse(dwPlayerGUID, dwCorpseGUID);
    }
}

static void __fastcall SCMD_Handler75_PlayerPartyInfo(uint8_t* pPacket)
{
    const uint32_t dwUnitGUID   = *reinterpret_cast<uint32_t*>(&pPacket[1]);
    const uint16_t wPartyId     = *reinterpret_cast<uint16_t*>(&pPacket[5]);
    const uint16_t wLevel       = *reinterpret_cast<uint16_t*>(&pPacket[7]);
    const uint8_t  bInYourParty = pPacket[11];
    ROSTER_UpdatePlayerPartyAndLevel(dwUnitGUID, wPartyId, wLevel, bInYourParty ? 3u : 1u);
}

static void __fastcall SCMD_Handler7A_PetAction(uint8_t* pPacket)
{
    // D2GSPacketSrv7A (13B): [0]=0x7A, [1]=bAdd, [2]=nPetType, [3..4]=wClassId, [5..8]=dwPetGUID, [9..12]=dwOwnerGUID
    const uint8_t  bAdd        = pPacket[1];
    const uint8_t  nPetType    = pPacket[2];
    const uint16_t wClassId    = *reinterpret_cast<uint16_t*>(&pPacket[3]);
    const uint32_t dwPetGUID   = *reinterpret_cast<uint32_t*>(&pPacket[5]);
    const uint32_t dwOwnerGUID = *reinterpret_cast<uint32_t*>(&pPacket[9]);

    if (bAdd != 0)
    {
        ROSTERPETS_AddOrUpdateBasicPet(nPetType, dwPetGUID, dwOwnerGUID, wClassId);
    }
    else
    {
        ROSTERPETS_RemoveOrMarkPetDead(dwPetGUID);
    }
}

static void __fastcall SCMD_Handler7B_AssignSkillHotkey(uint8_t* pPacket)
{
    const uint8_t  nSlot      = pPacket[1];
    const uint8_t  nSkillId   = pPacket[2];
    const uint8_t  bLeft      = pPacket[3];
    const uint32_t dwItemGUID = *reinterpret_cast<uint32_t*>(&pPacket[4]);
    D2Client_AssignSkillHotkey(nSlot, nSkillId, (bLeft != 0) ? 1 : 0, dwItemGUID);
}

static void __fastcall SCMD_Handler7F_AllyPartyInfo(uint8_t* pPacket)
{
    // D2GSPacketSrv7F (10B): [1]=bIsPlayer (1=player, 0=pet), [2..3]=wLifePct, [4..7]=dwUnitId, [8..9]=wLevelId
    const uint8_t  bIsPlayer = pPacket[1];
    const uint16_t wLifePct  = *reinterpret_cast<uint16_t*>(&pPacket[2]);
    const uint32_t dwUnitId  = *reinterpret_cast<uint32_t*>(&pPacket[4]);
    const uint16_t wLevelId  = *reinterpret_cast<uint16_t*>(&pPacket[8]);
    if (bIsPlayer != 0)
    {
        ROSTER_UpdatePlayerLifeAndLevelId(dwUnitId, wLifePct, wLevelId);
    }
}

static void __fastcall SCMD_Handler81_AssignMerc(uint8_t* pPacket)
{
    const uint16_t wPetType    = *reinterpret_cast<uint16_t*>(&pPacket[2]);
    const uint32_t dwOwnerGUID = *reinterpret_cast<uint32_t*>(&pPacket[4]);
    const uint32_t dwMercGUID  = *reinterpret_cast<uint32_t*>(&pPacket[8]);
    const uint32_t dwMonSeed   = *reinterpret_cast<uint32_t*>(&pPacket[12]);
    const uint16_t wNameIdx    = *reinterpret_cast<uint16_t*>(&pPacket[16]);

    D2RosterPetCreateDataStrc tCreate = { dwMonSeed, wNameIdx, 0 };
    ROSTERPETS_AddOrUpdateFullPet(wPetType, dwMercGUID, &tCreate, dwOwnerGUID, 7);
}

static void __fastcall SCMD_Handler8B_PlayerPartyRelationship(uint8_t* pPacket)
{
    const uint32_t dwUnitId    = *reinterpret_cast<uint32_t*>(&pPacket[1]);
    const uint8_t  nPartyFlags = pPacket[5];
    ROSTER_UpdatePlayerPartyFlags(dwUnitId, nPartyFlags);
}

static void __fastcall SCMD_Handler8C_PlayerRelationshipUpdate(uint8_t* pPacket)
{
    const uint32_t dwSourceId = *reinterpret_cast<uint32_t*>(&pPacket[1]);
    const uint32_t dwTargetId = *reinterpret_cast<uint32_t*>(&pPacket[5]);
    const uint16_t wState     = *reinterpret_cast<uint16_t*>(&pPacket[9]);
    ROSTER_OnRelationPacketReceived(dwSourceId, dwTargetId, wState);
}

static void __fastcall SCMD_Handler8D_AssignPlayerToParty(uint8_t* pPacket)
{
    const uint32_t dwPlayerId = *reinterpret_cast<uint32_t*>(&pPacket[1]);
    const uint16_t wPartyId   = *reinterpret_cast<uint16_t*>(&pPacket[5]);
    ROSTER_UpdatePlayerPartyId(dwPlayerId, wPartyId);
}

static void __fastcall SCMD_Handler8F_Pong(uint8_t* pPacket)
{
    (void)pPacket;
}

static void __fastcall SCMD_Handler90_PartyAutomapCoords(uint8_t* pPacket)
{
    const uint32_t dwPlayerId = *reinterpret_cast<uint32_t*>(&pPacket[1]);
    const int32_t  nX         = *reinterpret_cast<int32_t*>(&pPacket[5]);
    const int32_t  nY         = *reinterpret_cast<int32_t*>(&pPacket[9]);
    ROSTER_SetPlayerCoordsByUnitId(dwPlayerId, nX, nY);
}

static void __fastcall SCMD_Handler92_RemoveUnitDisplay(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    UNIT_RemoveClientUnit(nUnitType, dwUnitGUID);
}

static void __fastcall SCMD_Handler94_BaseSkillLevels(uint8_t* pPacket)
{
    const uint8_t  nCount       = pPacket[1];
    const uint32_t dwPlayerGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    D2UnitStrc* pPlayer = UNIT_FindClientUnit(static_cast<int32_t>(dwPlayerGUID), UNIT_PLAYER);
    if (pPlayer == nullptr)
    {
        pPlayer = UNIT_GetClientPlayer();
    }
    if (pPlayer == nullptr)
    {
        return;
    }

    const uint8_t* pCursor = &pPacket[6];
    for (uint8_t i = 0; i < nCount; ++i, pCursor += 3)
    {
        const uint16_t wSkillId = *reinterpret_cast<const uint16_t*>(&pCursor[0]);
        const uint8_t  nLevel   = pCursor[2];
        SKILLS_AssignSkill(pPlayer, wSkillId, nLevel, FALSE, __FILE__, __LINE__);
    }
}

static void __fastcall SCMD_Handler97_WeaponSwitch(uint8_t* pPacket)
{
    (void)pPacket;
    D2Client_OnWeaponSwitchPacket();
}

static void __fastcall SCMD_Handler9E_A2_MercAttr(uint8_t* pPacket)
{
    const uint8_t  bOpcode    = pPacket[0];
    const uint8_t  nStatId    = pPacket[1];
    const uint32_t dwMercGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    D2UnitStrc* pMerc = UNIT_FindClientUnit(static_cast<int32_t>(dwMercGUID), UNIT_MONSTER);
    if (pMerc == nullptr)
    {
        return;
    }

    int32_t nValue = 0;
    switch (bOpcode)
    {
    case 0x9E:
    case 0xA1:
        nValue = pPacket[6];
        break;
    case 0x9F:
    case 0xA2:
        nValue = *reinterpret_cast<uint16_t*>(&pPacket[6]);
        break;
    case 0xA0:
        nValue = *reinterpret_cast<int32_t*>(&pPacket[6]);
        break;
    default:
        return;
    }

    if (bOpcode >= 0xA1)
    {
        STATLIST_AddUnitStat(pMerc, nStatId, nValue, 0);
    }
    else
    {
        STATLIST_SetUnitStat(pMerc, nStatId, nValue, 0);
    }
}

static void __fastcall SCMD_HandlerA7_DelayedState(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const uint8_t  nState     = pPacket[6];
    D2UnitStrc* pUnit = UNIT_FindClientUnit(static_cast<int32_t>(dwUnitGUID), nUnitType);
    if (pUnit != nullptr)
    {
        STATES_ToggleState(pUnit, nState, TRUE);
    }
}

static void __fastcall SCMD_HandlerA8_SetState(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const uint8_t  nState     = pPacket[7];
    D2UnitStrc* pUnit = UNIT_FindClientUnit(static_cast<int32_t>(dwUnitGUID), nUnitType);
    if (pUnit != nullptr)
    {
        STATES_ToggleState(pUnit, nState, TRUE);
    }
}

static void __fastcall SCMD_HandlerA9_EndState(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const uint8_t  nState     = pPacket[6];
    D2UnitStrc* pUnit = UNIT_FindClientUnit(static_cast<int32_t>(dwUnitGUID), nUnitType);
    if (pUnit != nullptr)
    {
        STATES_ToggleState(pUnit, nState, FALSE);
    }
}

static void __fastcall SCMD_HandlerAB_NpcHeal(uint8_t* pPacket)
{
    const uint8_t  nUnitType  = pPacket[1];
    const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pPacket[2]);
    const uint8_t  nLifePct   = pPacket[6];
    D2UnitStrc* pUnit = UNIT_FindClientUnit(static_cast<int32_t>(dwUnitGUID), nUnitType);
    if (pUnit != nullptr)
    {
        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nLifePct, 0);
    }
}

// =============================================================================
// SECTION 3: Complete 175-Opcode S->C Dispatch Table (0x6FB93D20: 0x00 .. 0xAE)
// =============================================================================
// Every entry below documents:
//   { pfHandler, nPacketSize, pfUnitHandler }
// along with the exact 1.12a handler VA in comments.
const D2SCmdTableStrc sgptSCmdTable[0xAF] =
{
    /* 0x00 */ { SCMD_Handler00_GameLoading,             1,  nullptr }, // 0x6FB18020
    /* 0x01 */ { SCMD_Handler01_GameLogonReceipt,        8,  nullptr }, // 0x6FB18000
    /* 0x02 */ { SCMD_Handler02_LoadDone,                1,  nullptr }, // 0x6FB14FF0
    /* 0x03 */ { SCMD_Handler03_LoadAct,                12,  nullptr }, // 0x6FB160F0
    /* 0x04 */ { SCMD_Handler04_LoadComplete,            1,  nullptr }, // 0x6FB16550
    /* 0x05 */ { SCMD_Handler05_UnloadComplete,          1,  nullptr }, // 0x6FB15550
    /* 0x06 */ { SCMD_Handler06_GameLogoutSuccess,       1,  nullptr }, // 0x6FB157A0
    /* 0x07 */ { SCMD_Handler07_AddRoom,                 6,  nullptr }, // 0x6FB14920
    /* 0x08 */ { SCMD_Handler08_RemoveRoom,              6,  nullptr }, // 0x6FB148B0
    /* 0x09 */ { SCMD_Handler09_AssignWarp,             11,  nullptr }, // 0x6FB164E0
    /* 0x0A */ { SCMD_Handler0A_RemoveUnit,              6,  nullptr }, // 0x6FB154F0
    /* 0x0B */ { SCMD_Handler0B_GameHandshake,           6,  nullptr }, // 0x6FB14B70: GameHandshake
    /* 0x0C */ { nullptr,                                9,  SCMD_UnitHandler0C_GameHandshake }, // 0x6FB17B60
    /* 0x0D */ { nullptr,                               13,  SCMD_UnitHandler0D_PlayerStop },    // 0x6FB17B00
    /* 0x0E */ { SCMD_Handler0E_ObjectState,            12,  nullptr }, // 0x6FB17E80: ObjectState
    /* 0x0F */ { nullptr,                               16,  SCMD_UnitHandler0F_PlayerMove },    // 0x6FB17A80
    /* 0x10 */ { nullptr,                               16,  SCMD_UnitHandler10_PlayerToTarget },// 0x6FB17A30
    /* 0x11 */ { SCMD_Handler11_ReportKill,              8,  nullptr }, // 0x6FB149D0: ReportKill
    /* 0x12 */ { nullptr,                               26,  nullptr }, // 0x12: Unused
    /* 0x13 */ { nullptr,                               14,  nullptr }, // 0x13: Unused
    /* 0x14 */ { nullptr,                               18,  nullptr }, // 0x14: Unused
    /* 0x15 */ { SCMD_Handler15_ReassignPlayer,         11,  nullptr }, // 0x6FB17BB0
    /* 0x16 */ { SCMD_Handler16_MultiUnitUpdate,        -1,  nullptr }, // 0x6FB17DA0: MultiUnitUpdate
    /* 0x17 */ { SCMD_Handler_NoOp,                     -1,  nullptr }, // 0x6FB156B0: UnusedVar
    /* 0x18 */ { SCMD_Handler18_PlayerHPMP,             15,  nullptr }, // 0x6FB172E0
    /* 0x19 */ { SCMD_Handler19_1F_StatUpdate,           2,  nullptr }, // 0x6FB17430: GoldToInvByte
    /* 0x1A */ { SCMD_Handler19_1F_StatUpdate,           2,  nullptr }, // 0x6FB17430: AddExpByte
    /* 0x1B */ { SCMD_Handler19_1F_StatUpdate,           3,  nullptr }, // 0x6FB17430: AddExpWord
    /* 0x1C */ { SCMD_Handler19_1F_StatUpdate,           5,  nullptr }, // 0x6FB17430: SetExpDword
    /* 0x1D */ { SCMD_Handler19_1F_StatUpdate,           3,  nullptr }, // 0x6FB17430: SetAttrByte
    /* 0x1E */ { SCMD_Handler19_1F_StatUpdate,           4,  nullptr }, // 0x6FB17430: SetAttrWord
    /* 0x1F */ { SCMD_Handler19_1F_StatUpdate,           6,  nullptr }, // 0x6FB17430: SetAttrDword
    /* 0x20 */ { SCMD_Handler20_PlayerAttrUpdate,       10,  nullptr }, // 0x6FB173C0
    /* 0x21 */ { SCMD_Handler21_UpdateSkill,            12,  nullptr }, // 0x6FB17540: UpdateSkill
    /* 0x22 */ { SCMD_Handler22_UpdateItemSkill,        12,  nullptr }, // 0x6FB17600: UpdateItemSkill
    /* 0x23 */ { SCMD_Handler23_SetSkill,               13,  nullptr }, // 0x6FB176F0: SetSkill
    /* 0x24 */ { nullptr,                               90,  nullptr }, // 0x24: Unused
    /* 0x25 */ { nullptr,                               90,  nullptr }, // 0x25: Unused
    /* 0x26 */ { SCMD_Handler26_GameChat,               -1,  nullptr }, // 0x6FB15AC0: GameChat
    /* 0x27 */ { SCMD_Handler27_NpcInfo,                40,  nullptr }, // 0x6FB14C50: NpcInfo
    /* 0x28 */ { SCMD_Handler28_PlayerQuestInfo,       103,  nullptr }, // 0x6FB14CE0: PlayerQuestInfo
    /* 0x29 */ { SCMD_Handler29_GameQuestInfo,          97,  nullptr }, // 0x6FB14D10: GameQuestInfo
    /* 0x2A */ { SCMD_Handler_NoOp,                     15,  nullptr }, // 0x6FB14F70: NpcTransaction
    /* 0x2B */ { nullptr,                                0,  nullptr }, // 0x2B: Unused
    /* 0x2C */ { SCMD_Handler2C_PlaySound,               8,  nullptr }, // 0x6FB14E10: PlaySound
    /* 0x2D */ { nullptr,                                0,  nullptr }, // 0x2D: Unused
    /* 0x2E */ { nullptr,                                0,  nullptr }, // 0x2E: Unused
    /* 0x2F */ { nullptr,                                0,  nullptr }, // 0x2F: Unused
    /* 0x30 */ { nullptr,                                0,  nullptr }, // 0x30: Unused
    /* 0x31 */ { nullptr,                                0,  nullptr }, // 0x31: Unused
    /* 0x32 */ { nullptr,                                0,  nullptr }, // 0x32: Unused
    /* 0x33 */ { nullptr,                                0,  nullptr }, // 0x33: Unused
    /* 0x34 */ { nullptr,                                0,  nullptr }, // 0x34: Unused
    /* 0x35 */ { nullptr,                                0,  nullptr }, // 0x35: Unused
    /* 0x36 */ { nullptr,                                0,  nullptr }, // 0x36: Unused
    /* 0x37 */ { nullptr,                                0,  nullptr }, // 0x37: Unused
    /* 0x38 */ { nullptr,                                0,  nullptr }, // 0x38: Unused
    /* 0x39 */ { nullptr,                                0,  nullptr }, // 0x39: Unused
    /* 0x3A */ { nullptr,                                0,  nullptr }, // 0x3A: Unused
    /* 0x3B */ { nullptr,                                0,  nullptr }, // 0x3B: Unused
    /* 0x3C */ { nullptr,                                0,  nullptr }, // 0x3C: Unused
    /* 0x3D */ { nullptr,                                0,  nullptr }, // 0x3D: Unused
    /* 0x3E */ { SCMD_Handler_NoOp,                     -1,  nullptr }, // 0x6FB16D80: UpdateItemStats
    /* 0x3F */ { SCMD_Handler_NoOp,                      8,  nullptr }, // 0x6FB17070: UseStackableItem
    /* 0x40 */ { SCMD_Handler_NoOp,                     13,  nullptr }, // 0x6FB16D40: ItemTrigger
    /* 0x41 */ { nullptr,                                0,  nullptr }, // 0x41: Unused
    /* 0x42 */ { SCMD_Handler42_ClearCursor,             6,  nullptr }, // 0x6FB157F0: ClearCursor
    /* 0x43 */ { nullptr,                                0,  nullptr }, // 0x43: Unused
    /* 0x44 */ { nullptr,                                0,  nullptr }, // 0x44: Unused
    /* 0x45 */ { nullptr,                               13,  nullptr }, // 0x45: Unused
    /* 0x46 */ { nullptr,                                0,  nullptr }, // 0x46: Unused
    /* 0x47 */ { SCMD_Handler_NoOp,                     11,  nullptr }, // 0x6FB17F30: Relator1
    /* 0x48 */ { SCMD_Handler_NoOp,                     11,  nullptr }, // 0x6FB17F80: Relator2
    /* 0x49 */ { nullptr,                                0,  nullptr }, // 0x49: Unused
    /* 0x4A */ { nullptr,                                0,  nullptr }, // 0x4A: Unused
    /* 0x4B */ { nullptr,                                0,  nullptr }, // 0x4B: Unused
    /* 0x4C */ { SCMD_Handler4C_UnitSkillOnTarget,      16,  nullptr }, // 0x6FB153F0: UnitSkillOnTarget
    /* 0x4D */ { SCMD_Handler4D_UnitCastSkill,          17,  nullptr }, // 0x6FB15380: UnitCastSkill
    /* 0x4E */ { SCMD_Handler_NoOp,                      7,  nullptr }, // 0x6FB14C30: MercForHire
    /* 0x4F */ { SCMD_Handler_NoOp,                      1,  nullptr }, // 0x6FB14C40: ClearMercList
    /* 0x50 */ { SCMD_Handler_NoOp,                     15,  nullptr }, // 0x6FB14BF0: QuestSpecial
    /* 0x51 */ { SCMD_Handler51_AssignObject,           14,  nullptr }, // 0x6FB164B0
    /* 0x52 */ { SCMD_Handler_NoOp,                     42,  nullptr }, // 0x6FB14D30: QuestLogInfo
    /* 0x53 */ { SCMD_Handler_NoOp,                     10,  nullptr }, // 0x6FB14840: PlayerSlotRefresh
    /* 0x54 */ { nullptr,                               10,  nullptr }, // 0x54: Unused
    /* 0x55 */ { nullptr,                                3,  nullptr }, // 0x55: Unused
    /* 0x56 */ { nullptr,                                0,  nullptr }, // 0x56: Unused
    /* 0x57 */ { SCMD_Handler_NoOp,                     14,  nullptr }, // 0x6FB16530: MonsterEnchants
    /* 0x58 */ { SCMD_Handler58_OpenUIDialog,            7,  nullptr }, // 0x6FB14FB0: OpenUIDialog
    /* 0x59 */ { SCMD_Handler59_AssignPlayer,           26,  nullptr }, // 0x6FB16470
    /* 0x5A */ { SCMD_Handler_NoOp,                     40,  nullptr }, // 0x6FB15D60: EventMessages
    /* 0x5B */ { SCMD_Handler5B_PlayerInGame,           36,  nullptr }, // 0x6FB16030: PlayerInGame
    /* 0x5C */ { SCMD_Handler5C_PlayerLeftGame,          5,  nullptr }, // 0x6FB16010: PlayerLeftGame
    /* 0x5D */ { SCMD_Handler_NoOp,                      6,  nullptr }, // 0x6FB14D50: QuestItemState
    /* 0x5E */ { SCMD_Handler_NoOp,                     38,  nullptr }, // 0x6FB14DA0: GameQuestAvailability
    /* 0x5F */ { SCMD_Handler_NoOp,                      5,  nullptr }, // 0x6FB14DC0: Unknown5F
    /* 0x60 */ { SCMD_Handler_NoOp,                      7,  nullptr }, // 0x6FB15320: TownportalState
    /* 0x61 */ { SCMD_Handler_NoOp,                      2,  nullptr }, // 0x6FB14DE0: CanGoToAct
    /* 0x62 */ { SCMD_Handler_NoOp,                      7,  nullptr }, // 0x6FB154B0: MakeUnitTargetable
    /* 0x63 */ { SCMD_Handler63_WaypointMenu,           21,  nullptr }, // 0x6FB15F30: WaypointMenu
    /* 0x64 */ { nullptr,                                0,  nullptr }, // 0x64: Unused
    /* 0x65 */ { SCMD_Handler65_PlayerKillCount,         7,  nullptr }, // 0x6FB15F10: PlayerKillCount
    /* 0x66 */ { nullptr,                                7,  nullptr }, // 0x66: Unused
    /* 0x67 */ { nullptr,                               16,  SCMD_UnitHandler67_MonsterMove },        // 0x6FB179D0
    /* 0x68 */ { nullptr,                               21,  SCMD_UnitHandler68_MonsterMoveToTarget },// 0x6FB17960
    /* 0x69 */ { nullptr,                               12,  SCMD_UnitHandler69_MonsterState },       // 0x6FB17900
    /* 0x6A */ { nullptr,                               12,  SCMD_UnitHandler6A_MonsterUnk6A },       // 0x6FB178B0
    /* 0x6B */ { nullptr,                               16,  SCMD_UnitHandler6B_MonsterAction },      // 0x6FB17D30
    /* 0x6C */ { nullptr,                               16,  SCMD_UnitHandler6C_MonsterAttackUnit },  // 0x6FB17CC0
    /* 0x6D */ { nullptr,                               10,  SCMD_UnitHandler6D_MonsterStop },        // 0x6FB17820
    /* 0x6E */ { nullptr,                                1,  nullptr }, // 0x6E..0x72: Unused
    /* 0x6F */ { nullptr,                                1,  nullptr },
    /* 0x70 */ { nullptr,                                1,  nullptr },
    /* 0x71 */ { nullptr,                                1,  nullptr },
    /* 0x72 */ { nullptr,                                1,  nullptr },
    /* 0x73 */ { SCMD_Handler_NoOp,                     32,  nullptr }, // 0x6FB15460: MissileData
    /* 0x74 */ { SCMD_Handler74_PlayerCorpseAssign,     10,  nullptr }, // 0x6FB152E0: PlayerCorpseAssign
    /* 0x75 */ { SCMD_Handler75_PlayerPartyInfo,        13,  nullptr }, // 0x6FB15FE0: PlayerPartyInfo
    /* 0x76 */ { SCMD_Handler_NoOp,                      6,  nullptr }, // 0x6FB17C70: PlayerInSight
    /* 0x77 */ { SCMD_Handler_NoOp,                      2,  nullptr }, // 0x6FB14FA0: TradeAction
    /* 0x78 */ { SCMD_Handler_NoOp,                     21,  nullptr }, // 0x6FB15060: TradeAccepted
    /* 0x79 */ { SCMD_Handler_NoOp,                      6,  nullptr }, // 0x6FB150A0: GoldInTrade
    /* 0x7A */ { SCMD_Handler7A_PetAction,              13,  nullptr }, // 0x6FB15270: PetAction
    /* 0x7B */ { SCMD_Handler7B_AssignSkillHotkey,       8,  nullptr }, // 0x6FB177D0: AssignSkillHotkey
    /* 0x7C */ { SCMD_Handler_NoOp,                      6,  nullptr }, // 0x6FB15000: UseScroll
    /* 0x7D */ { SCMD_Handler_NoOp,                     18,  nullptr }, // 0x6FB15220: SetItemFlags
    /* 0x7E */ { SCMD_Handler_NoOp,                      5,  nullptr }, // 0x6FB150C0: CMNCOF
    /* 0x7F */ { SCMD_Handler7F_AllyPartyInfo,          10,  nullptr }, // 0x6FB15FA0: AllyPartyInfo
    /* 0x80 */ { nullptr,                                4,  nullptr }, // 0x80: Unused
    /* 0x81 */ { SCMD_Handler81_AssignMerc,             20,  nullptr }, // 0x6FB151D0: AssignMerc
    /* 0x82 */ { SCMD_Handler_NoOp,                     29,  nullptr }, // 0x6FB15FD0: PortalOwnership
    /* 0x83 */ { nullptr,                                0,  nullptr }, // 0x83..0x88: Unused
    /* 0x84 */ { nullptr,                                0,  nullptr },
    /* 0x85 */ { nullptr,                                0,  nullptr },
    /* 0x86 */ { nullptr,                                0,  nullptr },
    /* 0x87 */ { nullptr,                                0,  nullptr },
    /* 0x88 */ { nullptr,                                0,  nullptr },
    /* 0x89 */ { SCMD_Handler_NoOp,                      2,  nullptr }, // 0x6FB14DF0: SpecialQuestEvent
    /* 0x8A */ { SCMD_Handler_NoOp,                      6,  nullptr }, // 0x6FB15160: NpcWantsInteract
    /* 0x8B */ { SCMD_Handler8B_PlayerPartyRelationship, 6,  nullptr }, // 0x6FB15F80: PlayerPartyRelationship
    /* 0x8C */ { SCMD_Handler8C_PlayerRelationshipUpdate,11, nullptr }, // 0x6FB15F50: PlayerRelationshipUpdate
    /* 0x8D */ { SCMD_Handler8D_AssignPlayerToParty,     7,  nullptr }, // 0x6FB15F40: AssignPlayerToParty
    /* 0x8E */ { SCMD_Handler74_PlayerCorpseAssign,     10,  nullptr }, // 0x6FB152B0: CorpseAssign
    /* 0x8F */ { SCMD_Handler8F_Pong,                   33,  nullptr }, // 0x6FB15780: Pong
    /* 0x90 */ { SCMD_Handler90_PartyAutomapCoords,     13,  nullptr }, // 0x6FB15EF0: PartyAutomapCoords
    /* 0x91 */ { SCMD_Handler_NoOp,                     26,  nullptr }, // 0x6FB14B90: NpcGossipFlags
    /* 0x92 */ { SCMD_Handler92_RemoveUnitDisplay,       6,  nullptr }, // 0x6FB16C90: RemoveUnitDisplay
    /* 0x93 */ { SCMD_Handler_NoOp,                      8,  nullptr }, // 0x6FB14FD0: Unknown93
    /* 0x94 */ { SCMD_Handler94_BaseSkillLevels,        -1,  nullptr }, // 0x6FB17590: BaseSkillLevels
    /* 0x95 */ { SCMD_Handler95_PlayerLifeManaChange,   13,  nullptr }, // 0x6FB17220
    /* 0x96 */ { SCMD_Handler96_WalkVerify,              9,  nullptr }, // 0x6FB15EC0
    /* 0x97 */ { SCMD_Handler97_WeaponSwitch,            1,  nullptr }, // 0x6FB16DB0: WeaponSwitch
    /* 0x98 */ { SCMD_Handler_NoOp,                      7,  nullptr }, // 0x6FB14BB0: EvilUrn
    /* 0x99 */ { SCMD_Handler4C_UnitSkillOnTarget,      16,  nullptr }, // 0x6FB153F0: SkillTriggeredAtUnit
    /* 0x9A */ { SCMD_Handler4D_UnitCastSkill,          17,  nullptr }, // 0x6FB15380: SkillTriggeredAtCoord
    /* 0x9B */ { SCMD_Handler_NoOp,                      7,  nullptr }, // 0x6FB14FC0: MercReviveCost
    /* 0x9C */ { SCMD_Handler9C_ItemActionWorld,        -1,  nullptr }, // 0x6FB165F0
    /* 0x9D */ { SCMD_Handler9D_ItemActionOwned,        -1,  nullptr }, // 0x6FB16DE0
    /* 0x9E */ { SCMD_Handler9E_A2_MercAttr,             7,  nullptr }, // 0x6FB17330: MercAttrByte
    /* 0x9F */ { SCMD_Handler9E_A2_MercAttr,             8,  nullptr }, // 0x6FB17330: MercAttrWord
    /* 0xA0 */ { SCMD_Handler9E_A2_MercAttr,            10,  nullptr }, // 0x6FB17330: MercAttrDword
    /* 0xA1 */ { SCMD_Handler9E_A2_MercAttr,             7,  nullptr }, // 0x6FB17330: MercAddExpByte
    /* 0xA2 */ { SCMD_Handler9E_A2_MercAttr,             8,  nullptr }, // 0x6FB17330: MercAddExpWord
    /* 0xA3 */ { SCMD_Handler_NoOp,                     24,  nullptr }, // 0x6FB15590: AuraStatSkill
    /* 0xA4 */ { SCMD_Handler_NoOp,                      3,  nullptr }, // 0x6FB14E90: BaalWaveClassId
    /* 0xA5 */ { SCMD_Handler_NoOp,                      8,  nullptr }, // 0x6FB17500: StateSkillEffect
    /* 0xA6 */ { nullptr,                               -1,  nullptr }, // 0xA6: UnusedVar
    /* 0xA7 */ { SCMD_HandlerA7_DelayedState,            7,  nullptr }, // 0x6FB162B0: DelayedState
    /* 0xA8 */ { SCMD_HandlerA8_SetState,               -1,  nullptr }, // 0x6FB162F0: SetState (Bitstream)
    /* 0xA9 */ { SCMD_HandlerA9_EndState,                7,  nullptr }, // 0x6FB16430: EndState
    /* 0xAA */ { SCMD_Handler_NoOp,                     -1,  nullptr }, // 0x6FB16110: AddUnit / StateList
    /* 0xAB */ { SCMD_HandlerAB_NpcHeal,                 7,  nullptr }, // 0x6FB14980: NpcHeal
    /* 0xAC */ { SCMD_HandlerAC_AssignNPC,              -1,  nullptr }, // 0x6FB16990
    /* 0xAD */ { nullptr,                                9,  nullptr }, // 0xAD: Unused
    /* 0xAE */ { SCMD_Handler_NoOp,                     -1,  nullptr }, // 0x6FB14EB0: WardenRequest
};

// =============================================================================
// SECTION 4: Top-Level S->C Stream Dispatcher (0x6FB150D0)
// =============================================================================

// 0x6FB150D0
// Main S->C packet stream parser invoked every frame when D2Net delivers server packets:
//   1. Calls D2Net.#10002 (SERVER_GetServerPacketSize) to determine the next packet's byte size.
//   2. If nSize == -1, delegates to SCMD_ProcessSystemPacket (0x6FB15A70) for system opcodes 0xAF..0xB4.
//   3. Validates `bOpcode < 0xAF` and checks `sgptSCmdTable[bOpcode].nPacketSize` (if != -1).
//   4. If `sgptSCmdTable[bOpcode].pfUnitHandler != nullptr`:
//      - For monster packets `0x67..0x6D`, looks up `dwMonsterGUID = *(uint32_t*)(pCursor + 1)`
//        in `gapMonsterHashTable[dwMonsterGUID & 0x7F]` (`0x6FBCAB60`).
//      - For generic unit packets `0x0C..0x10`, looks up `(nType = pCursor[1], dwGUID = *(uint32_t*)(pCursor + 2))`
//        via `UNIT_FindClientUnit` (`0x6FACE550`), pre-decoding packed bitstreams for `0x0D..0x10` (`0x6FB14720`).
//      - Enqueues the packet onto `pUnit->pPacketQueue` (`+0xD8`) via `SCMD_QueueUnitPacket` (`0x6FB14AE0`).
//   5. Otherwise pre-decodes packed bitstreams for `0x18`, `0x95`, `0x96` (`0x6FB14690`, `0x6FB14620`)
//      and invokes `sgptSCmdTable[bOpcode].pfHandler(pCursor)`.
void __fastcall SCMD_ProcessPacketStream(uint8_t* pBuffer, int32_t nBufferSize)
{
    uint8_t* pCursor    = pBuffer;
    int32_t  nRemaining = nBufferSize;

    while (nRemaining > 0)
    {
        int32_t nVarHeader = 0;
        const int32_t nPacketSize = SERVER_GetServerPacketSize(pCursor, nRemaining, &nVarHeader);
        if (nPacketSize <= 0)
        {
            break;
        }

        const uint8_t bOpcode = pCursor[0];
        if (bOpcode >= 0xAF)
        {
            break;
        }

        const D2SCmdTableStrc* pEntry = &sgptSCmdTable[bOpcode];
        if (pEntry->nPacketSize != -1 && pEntry->nPacketSize != nPacketSize)
        {
            FOG_DisplayAssert("tPacketSize == sgptSCmdTable[bCmd].nSize", "..\\Source\\D2Client\\GAME\\SCmd.cpp", 1040);
            exit(-1);
        }

        g_dwBytesReceived   += static_cast<uint32_t>(nPacketSize);
        g_dwPacketsReceived += 1;

        if (pEntry->pfUnitHandler != nullptr)
        {
            D2UnitStrc* pTargetUnit = nullptr;
            if (bOpcode >= 0x67 && bOpcode <= 0x6D)
            {
                const uint32_t dwMonGUID = *reinterpret_cast<uint32_t*>(&pCursor[1]);
                for (D2UnitStrc* pNode = gapMonsterHashTable[dwMonGUID & 0x7F]; pNode != nullptr; pNode = pNode->pListNext)
                {
                    if (pNode->dwUnitId == dwMonGUID)
                    {
                        pTargetUnit = pNode;
                        break;
                    }
                }
            }
            else
            {
                const uint8_t  nUnitType  = pCursor[1];
                const uint32_t dwUnitGUID = *reinterpret_cast<uint32_t*>(&pCursor[2]);
                pTargetUnit = UNIT_FindClientUnit(dwUnitGUID, nUnitType);
            }

            if (pTargetUnit != nullptr)
            {
                SCMD_QueueUnitPacket(pTargetUnit, pCursor, static_cast<size_t>(nPacketSize));
            }
        }
        else if (pEntry->pfHandler != nullptr)
        {
            pEntry->pfHandler(pCursor);
        }

        pCursor    += nPacketSize;
        nRemaining -= nPacketSize;
    }
}
