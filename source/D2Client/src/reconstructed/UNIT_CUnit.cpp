// =============================================================================
// D2Client.dll 1.12a (ImageBase = 0x6FAB0000)
// Reconstructed Translation Unit: UNIT_CUnit.cpp
//   - ..\Source\D2Client\UNIT\CUnit.cpp (0x6FACE1C0 .. 0x6FAD1940)
//   - ..\Source\D2Client\UNIT\CUnitEvent.cpp (0x6FB6A060 .. 0x6FB6A3D0)
//
// Source Assertion Paths:
//   - "..\Source\D2Client\UNIT\CUnit.cpp" (0x6FB86C54)
//   - "..\Source\D2Client\UNIT\CUnitEvent.cpp" (0x6FB83880)
//
// Cross-referenced against:
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Units.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Player.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Monster.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Object.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Item.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Missile.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Inventory.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2StatList.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Skills.h
// =============================================================================

#include <stdint.h>
#include <string.h>
#include <windows.h>

#include <Units/Units.h>
#include <Units/UnitRoom.h>
#include <Path/Path.h>
#include <Path/PathMisc.h>
#include <Path/Step.h>
#include <UselessOrdinals.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgPreset.h>
#include <D2Environment.h>
#include <D2Inventory.h>
#include <D2Items.h>
#include <D2Monsters.h>
#include <D2Seed.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <D2DataTbls.h>
#include <D2Gfx.h>
#include <D2Lang.h>
#include <Fog.h>
#include <Storm.h>

// =============================================================================
// Client Unit Event Node Structure (CUnitEvent.cpp)
// Attached at pUnit->pCltTimerList (+0x90)
// =============================================================================
typedef int32_t (__fastcall* D2UnitEventCallbackFn)(
    int32_t nEventType,
    D2UnitStrc* pUnit,
    int32_t nParam1,
    int32_t nParam2,
    int32_t nArg1,
    int32_t nArg2);

struct D2UnitEventStrc // 0x20 bytes
{
    int32_t               nEventType;   // +0x00
    int32_t               nQueueId;     // +0x04
    int32_t               nEventId;     // +0x08
    int32_t               nArg1;        // +0x0C
    int32_t               nArg2;        // +0x10
    D2UnitEventCallbackFn pfnCallback;  // +0x14
    D2UnitEventStrc*      pPrev;        // +0x18
    D2UnitEventStrc*      pNext;        // +0x1C
};

// Monster spawn packet/descriptor passed to D2Client_AllocClientMonsterUnit (0x6FAD0EB0)
struct D2MonsterSpawnDescStrc
{
    uint32_t dwUnitGUID;    // +0x00
    int32_t  nClassId;      // +0x04
    int32_t  nX;            // +0x08
    int32_t  nY;            // +0x0C
    uint16_t wTypeFlags;    // +0x10
    uint16_t wPad12;        // +0x12
    int32_t  nLifePct;      // +0x14
    int32_t  nAnimMode;     // +0x18
};

// Roster pet entry node at 0x6FBCB97C (GAME/RosterPets.cpp)
struct D2RosterPetEntryStrc
{
    int32_t               nClassId;     // +0x00
    int32_t               nPetType;     // +0x04 (7 = hireling)
    uint32_t              dwUnitGUID;   // +0x08
    uint32_t              dwOwnerGUID;  // +0x0C
    uint32_t              dwReserved[6];// +0x10..+0x24
    uint32_t              dwSeed;       // +0x24
    uint16_t              wNameId;      // +0x28
    uint16_t              wPad2A;       // +0x2A
    uint32_t              dwUnk2C;      // +0x2C
    D2RosterPetEntryStrc* pNext;        // +0x30
};

// =============================================================================
// Client Unit Hash Tables & Globals
// =============================================================================
// Server-synced unit hash tables: 6 unit types * 128 buckets = 768 pointers (0xC00 bytes)
//   0x6FBCA960: UNIT_PLAYER  (0)
//   0x6FBCAB60: UNIT_MONSTER (1)
//   0x6FBCAD60: UNIT_OBJECT  (2)
//   0x6FBCAF60: UNIT_MISSILE (3)
//   0x6FBCB160: UNIT_ITEM    (4)
//   0x6FBCB360: UNIT_TILE    (5)
D2UnitStrc* gapServerSideUnitHashTables[6][128]; // 0x6FBCA960

// Client-only unit hash tables: 6 unit types * 128 buckets = 768 pointers (0xC00 bytes)
//   0x6FBC9D60: UNIT_PLAYER  (0)
//   0x6FBC9F60: UNIT_MONSTER (1)
//   0x6FBCA160: UNIT_OBJECT  (2)
//   0x6FBCA360: UNIT_MISSILE (3)
//   0x6FBCA560: UNIT_ITEM    (4)
//   0x6FBCA760: UNIT_TILE    (5)
D2UnitStrc* gapClientSideUnitHashTables[6][128]; // 0x6FBC9D60

D2UnitStrc*    gpPlayerUnit             = nullptr; // 0x6FBCC3D0
D2DrlgActStrc* gpClientAct              = nullptr; // 0x6FBCC01C
uint32_t       gdwNextClientUnitGUID    = 1;       // 0x6FBA6A80
int32_t        gnLastPeriodOfDay        = 0;       // 0x6FBD3698
uint8_t        gnLocalPlayerCharClass   = 0;       // 0x6FBB33D4
int32_t        gnPlayerStandStillFlag   = 0;       // 0x6FBCC054
int32_t        gnOpenPanelMode          = 0;       // 0x6FBCC1D0 (1 = right, 2 = left, 3 = both)
int32_t        gnScreenWidth            = 800;     // 0x6FB8C6E0
int32_t        gnScreenHeight           = 600;     // 0x6FB8C6E4
int32_t        gnMouseX                 = 0;       // 0x6FBB1638
int32_t        gnMouseY                 = 0;       // 0x6FBB1634
#define gnMouseCursorX gnMouseX
int32_t        gnGameType               = 0;       // 0x6FBCBFF8
uint8_t        gnClientDifficulty       = 0;       // 0x6FBCBFF4
int32_t        gbIsExpansion            = 0;       // 0x6FBB330C
extern int32_t gaPanelStates[38];                  // 0x6FBB2B58
#define gbHideGroundItems (gaPanelStates[13])
int32_t        gnAutomapRefreshState    = 0;       // 0x6FBCC2F4
int32_t        gnSelectedTargetA        = 0;       // 0x6FBCC118
int32_t        gnSelectedTargetB        = 0;       // 0x6FBCC11C
D2RosterPetEntryStrc* gpRosterPetList   = nullptr; // 0x6FBCB97C

// Unit destruction order table at 0x6FB86C3C: Monsters, Players, Objects, Missiles, Items, Tiles
static const int32_t g_anUnitFreeOrder[6] = {
    UNIT_MONSTER, UNIT_PLAYER, UNIT_OBJECT, UNIT_MISSILE, UNIT_ITEM, UNIT_TILE
};

// Forward declarations of helpers in other translation units
extern void __fastcall UNITMODE_UpdateUnitFrame(D2UnitStrc* pUnit);                             // 0x6FAF1EE0
extern void __fastcall UNITMODE_ClearActionFrame(D2UnitStrc* pUnit);                            // 0x6FAF0D20
extern void __fastcall UNITMODE_OnModeChangeComplete(int32_t nNewMode, D2UnitStrc* pUnit);      // 0x6FAF1290
extern void __fastcall UNITSND_PlayUnitModeSound(D2UnitStrc* pUnit, int32_t nMode, int32_t a3); // 0x6FAE8990
extern void __fastcall UNITSND_StopUnitLoopingSound(int32_t nArg, D2UnitStrc* pUnit);           // 0x6FAE7160
extern void __fastcall UNITSND_DeinitUnitAudio(D2UnitStrc* pUnit, int32_t bFreeData);           // 0x6FAE6DD0
extern void __fastcall UNITSND_InitMonsterAudio(D2UnitStrc* pUnit);                             // 0x6FAE7AA0
extern void __fastcall PLAYER_InitClientPlayer(D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY); // 0x6FB0A1D0
extern void __fastcall PLAYER_FreeClientPlayer(D2UnitStrc* pUnit);                              // 0x6FB0A120
extern void __fastcall PLAYER_ResetSkillState(D2UnitStrc* pUnit, int32_t bIsLocalPlayer);       // 0x6FB0A7C0
extern void __fastcall MONSTER_InitClientMonsterData(D2UnitStrc* pUnit, int32_t nArg);          // 0x6FAE2F30
extern void __fastcall MONSTER_SpawnClientMonster(D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nClassId, int32_t nAnimMode); // 0x6FAE3DC0
extern void __fastcall MONSTER_FreeClientMonster(D2UnitStrc* pUnit);                            // 0x6FAE3780
extern int32_t __fastcall MONSTER_CanBeConsumed(D2UnitStrc* pUnit);                             // 0x6FAE3AE0
extern void __stdcall MONSTER_SetHirelingNameAndSeed(D2UnitStrc* pUnit, void* pHirelingInfo);   // 0x6FAE5BB0
extern void __fastcall MONSTER_SpawnDeathOverlayOrMissile(D2UnitStrc* pUnit, int32_t nMissileId, int32_t a3, int32_t a4, int32_t a5); // 0x6FAE1FA0
extern void __fastcall OBJECT_UpdatePeriodOfDayLight(D2UnitStrc* pUnit);                        // 0x6FAEA960
extern void __fastcall OBJECT_InitClientObject(int32_t nY, D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nClassId, uint8_t nMode); // 0x6FAEAB70
extern void __fastcall OBJECT_FreeClientObject(D2UnitStrc* pUnit);                              // 0x6FAEA000
extern void __fastcall OBJECT_PostCreateInit(D2UnitStrc* pUnit);                                // 0x6FAEB250
extern void __fastcall OBJECT_InitShrineVisual(D2UnitStrc* pUnit);                              // 0x6FAE9DD0
extern void __fastcall ITEM_InitClientItem(D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t bFlag, int32_t nMode); // 0x6FB46270
extern void __fastcall ITEM_FreeClientItem(D2UnitStrc* pUnit);                                  // 0x6FB45F20
extern void __fastcall MISSILE_InitClientMissile(D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY); // 0x6FAD1B80
extern void __fastcall MISSILE_FreeClientMissile(D2UnitStrc* pUnit);                            // 0x6FAD28A0
extern void __fastcall DLIGHTMAP_RemoveUnitLight(D2GfxLightStrc* pLight);                       // 0x6FB0E500
extern void __fastcall SCMD_FreeUnitPacketQueue(D2UnitStrc* pUnit);                             // 0x6FB142B0
extern D2UnitStrc* __fastcall SELECT_GetSelectedUnit(void);                                     // 0x6FB1ECA0
extern uint32_t __fastcall ROSTERPETS_GetOwnerGUIDForPet(uint32_t dwPetGUID);                   // 0x6FB73160
extern int32_t __fastcall ROSTER_ArePlayersInSameParty(uint32_t dwGUID1, uint32_t dwGUID2);     // 0x6FB28250
extern int32_t __fastcall ROSTER_CheckHostilePvPFlag(uint32_t dwGUID1, uint32_t dwGUID2);       // 0x6FB287C0
extern void* __fastcall ROSTER_FindEntryByGUID(uint32_t dwUnitGUID);                            // 0x6FB281C0
extern int32_t __fastcall PLAYERLIST_CheckRelationshipFlag(uint32_t dwGUID, int32_t nFlag);     // 0x6FAE0DD0

// =============================================================================
// PART 1: Client Unit Event System (..\Source\D2Client\UNIT\CUnitEvent.cpp)
// =============================================================================

// 1.12a: 0x6FB6A060
// Calling convention: custom (__fastcall ecx = pUnit, 1 stack dword unused)
// Duriel unique modifier (umod 11) death & corpse maggot spawn handler.
void __fastcall MONUNIQUE_DurielUModHandler(D2UnitStrc* pUnit, int32_t nUnused)
{
    (void)nUnused;
    if (pUnit == nullptr || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2MonsterDataStrc* pMonsterData = pUnit->pMonsterData;
    if (pMonsterData == nullptr)
    {
        return;
    }

    if (pUnit->dwAnimMode == MONMODE_DEATH)
    {
        int32_t nFrame = 0;
        int32_t nFrameCount = 0;
        UNITS_GetShiftedFrameMetrics(pUnit, &nFrame, &nFrameCount);
        if (nFrame == 13)
        {
            // Spawn Duriel death gore overlays (0x34 and 0x9C)
            pUnit->nActionFrame = 0;
        }
    }
    else if (pUnit->dwAnimMode == MONMODE_DEAD)
    {
        // Advance unit PRNG seed and 1% chance to spawn up to 25 (dwDurielFlag < 100, +4 each) maggots
        uint64_t SeedProd = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
        pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(SeedProd);
        pUnit->pSeed.nHighSeed = static_cast<uint32_t>(SeedProd >> 32);

        if ((pUnit->pSeed.nLowSeed % 100u) == 0 && pMonsterData->dwDurielFlag < 100)
        {
            pMonsterData->dwDurielFlag += 4;
        }
    }
}

// Dispatch table for unique monster modifier client events at 0x6FBA9EC0 (44 umods * 5 events)
typedef void (__fastcall* D2MonUModEventFn)(D2UnitStrc* pUnit, int32_t bIsMinion);
extern D2MonUModEventFn g_apfnMonUModEvents[44][5]; // 0x6FBA9EC0

// 1.12a: 0x6FB6A140
// Calling convention: __stdcall (3 stack args: pUnit, pTargetOrArg, nEventIndex)
void __stdcall MONUNIQUE_DispatchUModEvent(D2UnitStrc* pUnit, D2UnitStrc* pTargetUnit, int32_t nEventIndex)
{
    if (pUnit == nullptr || pUnit->dwUnitType != UNIT_MONSTER || pUnit->pMonsterData == nullptr)
    {
        return;
    }

    D2MonsterDataStrc* pMonsterData = pUnit->pMonsterData;
    const uint8_t* pUMods = pMonsterData->nMonUmod;
    const int32_t bIsMinion = (pMonsterData->nTypeFlag >> 3) & 1;

    if (nEventIndex == 4)
    {
        for (int32_t i = 0; i < 9; ++i)
        {
            const uint8_t nUMod = pUMods[i];
            D2MonUModEventFn pfn = g_apfnMonUModEvents[nUMod][4];
            if (pfn != nullptr)
            {
                pfn(pTargetUnit, bIsMinion);
            }
        }
    }
    else
    {
        for (int32_t i = 0; i < 9; ++i)
        {
            const uint8_t nUMod = pUMods[i];
            D2MonUModEventFn pfn = g_apfnMonUModEvents[nUMod][nEventIndex];
            if (pfn != nullptr)
            {
                pfn(pUnit, bIsMinion);
            }
        }
    }
}

// 1.12a: 0x6FB6A1F0
// Calling convention: custom (eax = pUnit, ecx = pTargetUnit)
void __fastcall MONUNIQUE_TriggerUModEvent4(D2UnitStrc* pUnit /*eax*/, D2UnitStrc* pTargetUnit /*ecx*/)
{
    if (pUnit != nullptr && pTargetUnit != nullptr && pUnit->dwUnitType == UNIT_MONSTER)
    {
        MONUNIQUE_DispatchUModEvent(pUnit, pTargetUnit, 4);
    }
}

// 1.12a: 0x6FB6A210
// Calling convention: custom (eax = pUnit)
void __fastcall MONUNIQUE_TriggerUModEvent3(D2UnitStrc* pUnit /*eax*/)
{
    MONUNIQUE_DispatchUModEvent(pUnit, nullptr, 3);
}

// 1.12a: 0x6FB6A220
// Calling convention: custom (eax = pUnit)
void __fastcall MONUNIQUE_TriggerUModEvent2(D2UnitStrc* pUnit /*eax*/)
{
    MONUNIQUE_DispatchUModEvent(pUnit, nullptr, 2);
}

// 1.12a: 0x6FB6A230
// Calling convention: custom (eax = pUnit)
void __fastcall MONUNIQUE_TriggerUModEvent1(D2UnitStrc* pUnit /*eax*/)
{
    MONUNIQUE_DispatchUModEvent(pUnit, nullptr, 1);
}

// 1.12a: 0x6FB6A240
// Calling convention: custom (eax = pUnit)
void __fastcall MONUNIQUE_TriggerUModEvent0(D2UnitStrc* pUnit /*eax*/)
{
    MONUNIQUE_DispatchUModEvent(pUnit, nullptr, 0);
}

// 1.12a: 0x6FB6A250
// Calling convention: custom (edi = pUnit, ebx = nEventType, 2 stack args: nParam1, nParam2)
int32_t __stdcall UNITEVENT_TriggerEvent(D2UnitStrc* pUnit /*edi*/, int32_t nEventType /*ebx*/, int32_t nParam1, int32_t nParam2)
{
    int32_t nResult = 0;
    if (pUnit == nullptr)
    {
        return 0;
    }

    D2UnitEventStrc* pEvent = pUnit->pCltTimerList;
    while (pEvent != nullptr)
    {
        D2UnitEventStrc* pNext = pEvent->pNext;
        if (pEvent->nEventType == nEventType && pEvent->pfnCallback != nullptr)
        {
            nResult = pEvent->pfnCallback(
                nEventType,
                pUnit,
                nParam1,
                nParam2,
                pEvent->nArg1,
                pEvent->nArg2);
        }
        pEvent = pNext;
    }
    return nResult;
}

// 1.12a: 0x6FB6A290
// Calling convention: custom (esi = pUnit, ebx = nQueueId, 1 stack arg: nEventId)
void __stdcall UNITEVENT_RemoveUnitEvent(D2UnitStrc* pUnit /*esi*/, int32_t nQueueId /*ebx*/, int32_t nEventId)
{
    if (pUnit == nullptr)
    {
        return;
    }

    D2UnitEventStrc* pEvent = pUnit->pCltTimerList;
    while (pEvent != nullptr)
    {
        D2UnitEventStrc* pNext = pEvent->pNext;
        if (pEvent->nQueueId == nQueueId && pEvent->nEventId == nEventId)
        {
            if (pEvent->pPrev != nullptr)
            {
                pEvent->pPrev->pNext = pNext;
            }
            else
            {
                pUnit->pCltTimerList = pNext;
            }

            if (pNext != nullptr)
            {
                pNext->pPrev = pEvent->pPrev;
            }

            FOG_FreePool(pUnit->pMemoryPool, pEvent, "..\\Source\\D2Client\\UNIT\\CUnitEvent.cpp", 128, 0);
        }
        pEvent = pNext;
    }
}

// 1.12a: 0x6FB6A300
// Calling convention: custom (esi = pUnit, 6 stack args)
D2UnitEventStrc* __stdcall UNITEVENT_AllocUnitEvent(
    D2UnitStrc* pUnit /*esi*/,
    int32_t nEventType,
    int32_t nArg1,
    int32_t nArg2,
    D2UnitEventCallbackFn pfnCallback,
    int32_t nQueueId,
    int32_t nEventId)
{
    if (pUnit == nullptr)
    {
        return nullptr;
    }

    D2UnitEventStrc* pEvent = static_cast<D2UnitEventStrc*>(
        FOG_AllocPool(pUnit->pMemoryPool, sizeof(D2UnitEventStrc), "..\\Source\\D2Client\\UNIT\\CUnitEvent.cpp", 72, 0));

    memset(pEvent, 0, sizeof(D2UnitEventStrc));
    pEvent->nEventType  = nEventType;
    pEvent->nArg1       = nArg1;
    pEvent->nArg2       = nArg2;
    pEvent->pfnCallback = pfnCallback;
    pEvent->pNext       = pUnit->pCltTimerList;
    pEvent->nQueueId    = nQueueId;
    pEvent->nEventId    = nEventId;

    if (pUnit->pCltTimerList != nullptr)
    {
        pUnit->pCltTimerList->pPrev = pEvent;
    }
    pUnit->pCltTimerList = pEvent;
    return pEvent;
}

// 1.12a: 0x6FB6A390
// Calling convention: custom (edi = pUnit)
void __fastcall UNITEVENT_FreeEventList(D2UnitStrc* pUnit /*edi*/)
{
    if (pUnit == nullptr)
    {
        return;
    }

    D2UnitEventStrc* pEvent = pUnit->pCltTimerList;
    while (pEvent != nullptr)
    {
        D2UnitEventStrc* pNext = pEvent->pNext;
        FOG_FreePool(pUnit->pMemoryPool, pEvent, "..\\Source\\D2Client\\UNIT\\CUnitEvent.cpp", 47, 0);
        pEvent = pNext;
    }
    pUnit->pCltTimerList = nullptr;
}

// =============================================================================
// PART 2: Client Unit Hash Tables, Accessors & Per-Tick Updates (CUnit.cpp)
// =============================================================================

// 1.12a: 0x6FACE1C0
// Calling convention: custom (eax = pUnit, ecx = pExcludedUnit, edi = pTargetItem)
int32_t __fastcall D2Client_UnitInventoryContainsItem(D2UnitStrc* pUnit /*eax*/, D2UnitStrc* pExcludedUnit /*ecx*/, D2UnitStrc* pTargetItem /*edi*/)
{
    if (pExcludedUnit == pUnit || pUnit->pInventory == nullptr)
    {
        return 0;
    }

    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory);
         pItem != nullptr;
         pItem = INVENTORY_GetNextItem(pItem))
    {
        D2UnitStrc* pVerifiedItem = INVENTORY_UnitIsItem(pItem);
        if (pVerifiedItem == nullptr)
        {
            FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2651);
            exit(-1);
        }
        if (pVerifiedItem == pTargetItem)
        {
            return 1;
        }
    }
    return 0;
}

// 1.12a: 0x6FACE230
// Calling convention: custom (eax = pUnit, 1 stack arg: dwStartLight)
void __stdcall D2Client_SetUnitStartLight(D2UnitStrc* pUnit /*eax*/, uint32_t dwStartLight)
{
    if (pUnit == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2634);
        exit(-1);
    }
    pUnit->dwStartLight = dwStartLight;
}

// 1.12a: 0x6FACE260
// Calling convention: custom (eax = pUnit)
uint32_t __fastcall D2Client_GetUnitStartLight(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2623);
        exit(-1);
    }
    return pUnit->dwStartLight;
}

// 1.12a: 0x6FACE290
// Calling convention: custom (eax = pUnit)
void __fastcall D2Client_ResetUnitGfxCopyCounter(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2612);
        exit(-1);
    }
    *reinterpret_cast<uint32_t*>(&pUnit->pGfxDataCopy) = 3;
}

// 1.12a: 0x6FACE2C0
// Calling convention: custom (eax = pUnit)
int32_t __fastcall D2Client_TickUnitGfxCopyCounterAndCheckState(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2585);
        exit(-1);
    }

    uint32_t nCount = *reinterpret_cast<uint32_t*>(&pUnit->pGfxDataCopy);
    if (nCount == 0)
    {
        return 0;
    }

    *reinterpret_cast<uint32_t*>(&pUnit->pGfxDataCopy) = nCount - 1;
    return STATES_CheckState(pUnit, 143) == 0;
}

// 1.12a: 0x6FACE310
// Calling convention: custom (eax = pUnit)
void __fastcall D2Client_MarkUnitDead(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit != nullptr)
    {
        pUnit->dwFlags |= UNITFLAG_ISDEAD;
    }
}

// 1.12a: 0x6FACE320
// Calling convention: custom (eax = pUnit, 1 stack arg: pPacketList)
void __stdcall D2Client_SetUnitPacketList(D2UnitStrc* pUnit /*eax*/, D2PacketListStrc* pPacketList)
{
    if (pUnit == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2223);
        exit(-1);
    }
    pUnit->pPacketList = pPacketList;
}

// 1.12a: 0x6FACE350
// Calling convention: custom (eax = pUnit)
D2PacketListStrc* __fastcall D2Client_GetUnitPacketList(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2213);
        exit(-1);
    }
    return pUnit->pPacketList;
}

// 1.12a: 0x6FACE380
// Calling convention: custom (esi = pUnit, 1 stack arg unused)
int32_t __stdcall D2Client_CanInterruptSequenceSkill(D2UnitStrc* pUnit /*esi*/, int32_t nUnused)
{
    (void)nUnused;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (pSkill == nullptr)
    {
        return 1;
    }

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
    if (pSkillsTxt == nullptr)
    {
        return 0;
    }

    if (pSkillsTxt->nAnim != PLRMODE_SEQUENCE)
    {
        return 1;
    }

    const uint8_t nSeqInput = pSkillsTxt->nSeqInput;
    if (nSeqInput == 0 || nSeqInput == 0xFF)
    {
        return 0;
    }

    return (pUnit->dwSeqFrame >> 8) >= static_cast<int32_t>(nSeqInput);
}

// 1.12a: 0x6FACE3D0
// Calling convention: custom (eax = pUnit)
int32_t __fastcall D2Client_IsPlayerOrMonsterUnit(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 1939);
        exit(-1);
    }
    return (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER) ? 1 : 0;
}

// 1.12a: 0x6FACE410
// Calling convention: __fastcall (ecx = pCaster, edx = pTarget)
int32_t __fastcall D2Client_CorpseFilter5(D2UnitStrc* pCaster, D2UnitStrc* pTarget)
{
    (void)pCaster;
    return D2COMMON_11019(pTarget);
}

// 1.12a: 0x6FACE420
// Calling convention: __fastcall (ecx = pCaster, edx = pTarget)
int32_t __fastcall D2Client_CorpseFilter6(D2UnitStrc* pCaster, D2UnitStrc* pTarget)
{
    (void)pCaster;
    return D2Common_11020(pTarget);
}

// 1.12a: 0x6FACE430
// Calling convention: __fastcall (ecx = pCaster, edx = pTarget)
int32_t __fastcall D2Client_CorpseFilter4(D2UnitStrc* pCaster, D2UnitStrc* pTarget)
{
    (void)pCaster;
    return D2COMMON_11018(pTarget);
}

// 1.12a: 0x6FACE440
// Calling convention: __fastcall (ecx = pCaster, edx = pTarget)
int32_t __fastcall D2Client_CorpseFilter2(D2UnitStrc* pCaster, D2UnitStrc* pTarget)
{
    (void)pCaster;
    return D2COMMON_11017_CheckUnitIfConsumeable(pTarget, 0);
}

// 1.12a: 0x6FACE450
// Calling convention: __fastcall (ecx = pCaster, edx = pTarget)
int32_t __fastcall D2Client_CorpseFilter1(D2UnitStrc* pCaster, D2UnitStrc* pTarget)
{
    (void)pCaster;
    return D2COMMON_11021(pTarget);
}

// 1.12a: 0x6FACE460
// Calling convention: custom (eax = pUnit)
D2GfxDataStrc* __fastcall D2Client_GetUnitGfxData(D2UnitStrc* pUnit /*eax*/)
{
    return (pUnit != nullptr) ? pUnit->pGfxData : nullptr;
}

// 1.12a: 0x6FACE470
// Calling convention: __stdcall
int32_t __stdcall D2Client_IsLocalPlayerDead(void)
{
    if (gpPlayerUnit == nullptr)
    {
        return 1;
    }
    return (gpPlayerUnit->dwAnimMode == PLRMODE_DEAD) ? 1 : 0;
}

// 1.12a: 0x6FACE490
// Calling convention: __stdcall
D2UnitStrc* __stdcall D2Client_GetLocalPlayerUnit(void)
{
    return gpPlayerUnit;
}

// 1.12a: 0x6FACE4A0
// Calling convention: __stdcall
D2UnitStrc* __stdcall D2Client_GetLocalPlayer(void)
{
    return gpPlayerUnit;
}

// 1.12a: 0x6FACE4B0
// Calling convention: __fastcall (ecx = pUnit)
void __fastcall D2Client_ExpireUnitModStatsCallback(D2UnitStrc* pUnit)
{
    STATLIST_FreeModStats(pUnit);
}

// 1.12a: 0x6FACE4C0
// Calling convention: __fastcall (ecx = pUnit)
void __fastcall D2Client_NullUnitCallback(D2UnitStrc* pUnit)
{
    (void)pUnit;
}

// 1.12a: 0x6FACE4D0
// Calling convention: custom (ebx = pfnCallback, 1 stack arg: ppHashBuckets)
void __stdcall D2Client_ForEachUnitInHashTable(void (__fastcall* pfnCallback)(D2UnitStrc*) /*ebx*/, D2UnitStrc** ppHashBuckets)
{
    for (int32_t nBucket = 0; nBucket < 128; ++nBucket)
    {
        D2UnitStrc* pUnit = ppHashBuckets[nBucket];
        while (pUnit != nullptr)
        {
            D2UnitStrc* pNext = pUnit->pListNext;
            pfnCallback(pUnit);
            pUnit = pNext;
        }
    }
}

// 1.12a: 0x6FACE510
// Calling convention: __stdcall
void __stdcall D2Client_ClearAllUnitHashTables(void)
{
    memset(gapServerSideUnitHashTables, 0, sizeof(gapServerSideUnitHashTables));
    memset(gapClientSideUnitHashTables, 0, sizeof(gapClientSideUnitHashTables));
}

// 1.12a: 0x6FACE530
// Calling convention: __stdcall (1 stack arg: pUnit)
void __stdcall D2Client_RemoveUnitFromRoomIfActLoaded(D2UnitStrc* pUnit)
{
    if (gpClientAct != nullptr)
    {
        UNITROOM_RemoveUnitFromRoom(pUnit);
    }
}

// 1.12a: 0x6FACE550
// Calling convention: custom (eax = nBucketIndex, ecx = dwUnitGUID, edx = ppHashTable, 1 stack arg: nUnitType)
D2UnitStrc* __stdcall D2Client_FindUnitInHashBucket(
    uint32_t nBucketIndex /*eax*/,
    uint32_t dwUnitGUID /*ecx*/,
    D2UnitStrc** ppHashTable /*edx*/,
    uint32_t nUnitType)
{
    for (D2UnitStrc* pUnit = ppHashTable[nBucketIndex]; pUnit != nullptr; pUnit = pUnit->pListNext)
    {
        if (pUnit->dwUnitId == dwUnitGUID)
        {
            if (pUnit->dwUnitType != nUnitType)
            {
                FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 90);
                exit(-1);
            }
            return pUnit;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FACE590
// Calling convention: custom (eax = nUnitType)
D2UnitStrc** __fastcall D2Client_GetClientSideUnitHashTable(int32_t nUnitType /*eax*/)
{
    return gapClientSideUnitHashTables[nUnitType];
}

// 1.12a: 0x6FACE5A0
// Calling convention: custom (eax = nUnitType)
D2UnitStrc** __fastcall D2Client_GetServerSideUnitHashTable(int32_t nUnitType /*eax*/)
{
    return gapServerSideUnitHashTables[nUnitType];
}

// 1.12a: 0x6FACE5B0
// Calling convention: __fastcall (ecx = pClientUnit)
// Synchronizes dynamic path and animation mode between the local single-player server unit
// and the client unit.
void __fastcall D2Client_SyncSinglePlayerUnitWithServer(D2UnitStrc* pClientUnit)
{
    if (gnGameType != 0)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2986);
        exit(-1);
    }

    const uint32_t nUnitType = (pClientUnit != nullptr) ? pClientUnit->dwUnitType : UNIT_TYPES_COUNT;
    const uint32_t dwUnitGUID = (pClientUnit != nullptr) ? pClientUnit->dwUnitId : 0xFFFFFFFFu;

    // D2Game.dll!#10060: DEBUG_GetUnitFromTypeAndGUID
    extern D2UnitStrc* __stdcall DEBUG_GetUnitFromTypeAndGUID(uint32_t nType, uint32_t dwGUID);
    D2UnitStrc* pServerUnit = DEBUG_GetUnitFromTypeAndGUID(nUnitType, dwUnitGUID);
    if (pServerUnit == nullptr)
    {
        return;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pClientUnit);
    if (pRoom == nullptr)
    {
        return;
    }

    D2COMMON_10212_PATH_SetMoveFlags(pClientUnit, 0);
}

// 1.12a: 0x6FACE920
// Calling convention: custom (eax = pTargetItem, 1 stack arg: pExcludedUnit)
int32_t __stdcall D2Client_IsItemInAnyPlayerOrMonsterInventory(D2UnitStrc* pTargetItem /*eax*/, D2UnitStrc* pExcludedUnit)
{
    for (int32_t nTableIdx = 0; nTableIdx < 2; ++nTableIdx)
    {
        D2UnitStrc** ppBuckets = (nTableIdx == 0)
            ? gapServerSideUnitHashTables[UNIT_PLAYER]
            : gapServerSideUnitHashTables[UNIT_MONSTER];

        for (int32_t nBucket = 0; nBucket < 128; ++nBucket)
        {
            for (D2UnitStrc* pUnit = ppBuckets[nBucket]; pUnit != nullptr; pUnit = pUnit->pListNext)
            {
                if (D2Client_UnitInventoryContainsItem(pUnit, pExcludedUnit, pTargetItem))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// 1.12a: 0x6FACE990
// Calling convention: custom (eax = pWarpUnit, 1 stack arg: pRoom)
Unicode* __stdcall D2Client_GetWarpDestinationLevelName(D2UnitStrc* pWarpUnit /*eax*/, D2ActiveRoomStrc* pRoom)
{
    const int32_t nClassId = (pWarpUnit != nullptr) ? pWarpUnit->dwClassId : -1;
    const int32_t nDestLevel = DUNGEON_GetWarpDestinationLevel(pRoom, nClassId);
    if (nDestLevel == 0)
    {
        return nullptr;
    }

    D2LevelsTxt* pLevelsTxt = DATATBLS_GetLevelsTxtRecord(nDestLevel);
    return pLevelsTxt->wszLevelName;
}

// 1.12a: 0x6FACE9C0
// Calling convention: custom (eax = pRoomUnitListHead)
D2UnitStrc* __fastcall D2Client_FindFirstTileUnitInRoomList(D2UnitStrc* pUnit /*eax*/)
{
    for (; pUnit != nullptr; pUnit = pUnit->pRoomNext)
    {
        if (pUnit->dwUnitType == UNIT_TILE)
        {
            return pUnit;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FACE9E0
// Calling convention: custom (eax = pfnCallback)
void __fastcall D2Client_ForEachServerObjectUnit(void (__fastcall* pfnCallback)(D2UnitStrc*) /*eax*/)
{
    if (pfnCallback == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2423);
        exit(-1);
    }
    D2Client_ForEachUnitInHashTable(pfnCallback, gapServerSideUnitHashTables[UNIT_OBJECT]);
}

// 1.12a: 0x6FACEA20
// Calling convention: custom (eax = pfnCallback)
void __fastcall D2Client_ForEachServerMonsterUnit(void (__fastcall* pfnCallback)(D2UnitStrc*) /*eax*/)
{
    if (pfnCallback == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2412);
        exit(-1);
    }
    D2Client_ForEachUnitInHashTable(pfnCallback, gapServerSideUnitHashTables[UNIT_MONSTER]);
}

// 1.12a: 0x6FACEA60
// Calling convention: custom (eax = pfnCallback)
void __fastcall D2Client_ForEachServerPlayerUnit(void (__fastcall* pfnCallback)(D2UnitStrc*) /*eax*/)
{
    if (pfnCallback == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2401);
        exit(-1);
    }
    D2Client_ForEachUnitInHashTable(pfnCallback, gapServerSideUnitHashTables[UNIT_PLAYER]);
}

// 1.12a: 0x6FACEAA0
// Calling convention: custom (edi = pUnit)
void __fastcall D2Client_FreeUnitLight(D2UnitStrc* pUnit /*edi*/)
{
    if (pUnit->pLight != nullptr)
    {
        DLIGHTMAP_RemoveUnitLight(pUnit->pLight);
        pUnit->pLight = nullptr;
    }
}

// 1.12a: 0x6FACEAC0
// Calling convention: custom (eax = pUnit)
int32_t __fastcall D2Client_IsUnitDeadOrDying(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit->dwFlags & UNITFLAG_ISDEAD)
    {
        return 1;
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        return (pUnit->dwAnimMode == PLRMODE_DEATH || pUnit->dwAnimMode == PLRMODE_DEAD) ? 1 : 0;
    }
    else if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        return (pUnit->dwAnimMode == MONMODE_DEATH || pUnit->dwAnimMode == MONMODE_DEAD) ? 1 : 0;
    }
    return 0;
}

// 1.12a: 0x6FACEB00
// Calling convention: __stdcall
D2ActiveRoomStrc* __stdcall D2Client_GetLocalPlayerRoom(void)
{
    if (gpPlayerUnit != nullptr)
    {
        return UNITS_GetRoom(gpPlayerUnit);
    }
    return nullptr;
}

// 1.12a: 0x6FACEB20
// Calling convention: custom (eax = pUnit, 1 stack arg: nArg)
int32_t __stdcall D2Client_CanPlayerInitiateAction(D2UnitStrc* pUnit /*eax*/, int32_t nArg)
{
    if (INVENTORY_GetCursorItem(pUnit->pInventory) != nullptr)
    {
        return 0;
    }

    if (pUnit->dwUnitType != UNIT_PLAYER)
    {
        return 0;
    }

    switch (pUnit->dwAnimMode)
    {
    case PLRMODE_DEATH:
    case PLRMODE_GETHIT:
    case PLRMODE_BLOCK:
    case PLRMODE_CAST:
    case PLRMODE_THROW:
    case PLRMODE_KICK:
    case PLRMODE_SPECIAL2:
    case PLRMODE_DEAD:
    case PLRMODE_KNOCKBACK:
        return 0;

    case PLRMODE_ATTACK1:
    case PLRMODE_ATTACK2:
        return (pUnit->dwClassId == PCLASS_PALADIN) ? 0 : 1;

    case PLRMODE_SPECIAL1:
        if (pUnit->dwClassId == PCLASS_BARBARIAN ||
            pUnit->dwClassId == PCLASS_DRUID ||
            pUnit->dwClassId == PCLASS_ASSASSIN)
        {
            return 0;
        }
        return 1;

    case PLRMODE_SPECIAL3:
    case PLRMODE_SPECIAL4:
        return (pUnit->dwClassId == PCLASS_ASSASSIN) ? 0 : 1;

    case PLRMODE_SEQUENCE:
        return D2Client_CanInterruptSequenceSkill(pUnit, nArg);

    default:
        return 1;
    }
}

// 1.12a: 0x6FACEBC0
// Calling convention: custom (esi = pTargetUnit)
void __fastcall D2Client_FaceLocalPlayerTowardUnit(D2UnitStrc* pTargetUnit /*esi*/)
{
    if (gpPlayerUnit == nullptr)
    {
        return;
    }

    if (gpPlayerUnit->dwAnimMode != PLRMODE_NEUTRAL && gpPlayerUnit->dwAnimMode != PLRMODE_TOWNNEUTRAL)
    {
        return;
    }

    if (gpPlayerUnit->pDynamicPath == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2050);
        exit(-1);
    }

    const int32_t nTargetX = UNITS_GetClientCoordX(pTargetUnit);
    const int32_t nTargetY = UNITS_GetClientCoordY(pTargetUnit);
    D2Common_10216(gpPlayerUnit->pDynamicPath, nTargetX, nTargetY, 0);
}

// 1.12a: 0x6FACEC80
// Calling convention: custom (eax = pUnit)
void __fastcall D2Client_SetUnitNeutralAndFaceLocalPlayer(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit->pDynamicPath == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2035);
        exit(-1);
    }

    UNITS_ChangeAnimMode(pUnit, MONMODE_NEUTRAL);
    const int32_t nPlayerX = UNITS_GetClientCoordX(gpPlayerUnit);
    const int32_t nPlayerY = UNITS_GetClientCoordY(gpPlayerUnit);
    D2Common_10216(pUnit->pDynamicPath, nPlayerX, nPlayerY, 0);
}

// =============================================================================
// PART 3: Core CUnit.cpp Functions (0x6FACED40 .. 0x6FAD1940)
// =============================================================================

typedef int32_t (__fastcall* D2CorpseFilterFn)(D2UnitStrc*, D2UnitStrc*);
static const D2CorpseFilterFn g_apfnCorpseFilters[7] = { // 0x6FBA6A84
    nullptr,
    &D2Client_CorpseFilter1,
    &D2Client_CorpseFilter2,
    nullptr, // 0x6FACFBE0 bound below
    &D2Client_CorpseFilter4,
    &D2Client_CorpseFilter5,
    &D2Client_CorpseFilter6,
};
static int32_t g_nCorpseFilterCount = 7; // 0x6FBA6AA0

// 1.12a: 0x6FACFBE0
// Calling convention: __fastcall (ecx = pCaster, edx = pTargetUnit)
int32_t __fastcall D2Client_CorpseFilter3(D2UnitStrc* pCaster, D2UnitStrc* pTargetUnit)
{
    (void)pCaster;
    if (pTargetUnit == nullptr)
    {
        return 0;
    }

    if (!D2COMMON_11017_CheckUnitIfConsumeable(pTargetUnit, 0))
    {
        return 0;
    }

    if (!MONSTER_CanBeConsumed(pTargetUnit))
    {
        return 0;
    }

    if (pTargetUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    // Reject Sarcophagus (monster class 0x4E = 78)
    if (pTargetUnit->dwClassId == 78)
    {
        return 0;
    }

    return 1;
}

// 1.12a: 0x6FACED40
// Calling convention: custom (eax = nSkillId, 2 stack args: pCaster, pTarget)
int32_t __stdcall D2Client_CanSkillTargetCorpse(int32_t nSkillId /*eax*/, D2UnitStrc* pCaster, D2UnitStrc* pTarget)
{
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxt = &sgptDataTables->pSkillsTxt[nSkillId];
    if (pSkillsTxt == nullptr)
    {
        return 0;
    }

    const uint8_t nSelectProc = pSkillsTxt->nSelectProc;
    if (nSelectProc == 0 || static_cast<int32_t>(nSelectProc) >= g_nCorpseFilterCount)
    {
        return 0;
    }

    D2CorpseFilterFn pfnFilter = (nSelectProc == 3)
        ? &D2Client_CorpseFilter3
        : g_apfnCorpseFilters[nSelectProc];

    if (pfnFilter == nullptr || !pfnFilter(pCaster, pTarget))
    {
        return 0;
    }
    return 1;
}

// 1.12a: 0x6FACEDA0
// Calling convention: custom (eax = pUnit, edi = pObjectsTxt, ebx = nCursorX, 1 stack arg: nCursorY)
int32_t __stdcall D2Client_HitTestObjectSelectableBox(
    D2UnitStrc* pUnit /*eax*/,
    D2ObjectsTxt* pObjectsTxt /*edi*/,
    int32_t nCursorX /*ebx*/,
    int32_t nCursorY)
{
    int32_t nUnitX = UNITS_GetClientCoordX(pUnit);
    int32_t nUnitY = UNITS_GetClientCoordY(pUnit);
    DUNGEON_GameSubtileToClientCoords(&nUnitX, &nUnitY);

    const int32_t nLeft   = nUnitX + pObjectsTxt->nXOffset;
    const int32_t nTop    = nUnitY + pObjectsTxt->nYOffset;
    const int32_t nRight  = nLeft  + pObjectsTxt->nXSpace;
    const int32_t nBottom = nTop   + pObjectsTxt->nYSpace;

    if (nCursorX < nLeft || nCursorY < nTop || nCursorX > nRight || nCursorY > nBottom)
    {
        return 0;
    }
    return 1;
}

// 1.12a: 0x6FACEE60
// Calling convention: custom (eax = pTileUnit, ebx = nCursorX, 1 stack arg: nCursorY)
int32_t __stdcall D2Client_HitTestWarpTileBox(
    D2UnitStrc* pTileUnit /*eax*/,
    int32_t nCursorX /*ebx*/,
    int32_t nCursorY)
{
    if (pTileUnit->dwUnitType != UNIT_TILE)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 1419);
        exit(-1);
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pTileUnit);
    D2LvlWarpTxt* pLvlWarpTxt = DUNGEON_GetLvlWarpTxtRecordFromRoomAndUnit(pRoom, pTileUnit);
    if (pLvlWarpTxt == nullptr)
    {
        return 0;
    }

    int32_t nUnitX = UNITS_GetClientCoordX(pTileUnit);
    int32_t nUnitY = UNITS_GetClientCoordY(pTileUnit);
    DUNGEON_GameSubtileToClientCoords(&nUnitX, &nUnitY);

    const int32_t nLeft   = nUnitX + pLvlWarpTxt->dwSelectX;
    const int32_t nTop    = nUnitY + pLvlWarpTxt->dwSelectY;
    const int32_t nRight  = nLeft  + pLvlWarpTxt->dwSelectDX;
    const int32_t nBottom = nTop   + pLvlWarpTxt->dwSelectDY;

    if (nCursorX < nLeft || nCursorY < nTop || nCursorX > nRight || nCursorY > nBottom)
    {
        return 0;
    }
    return 1;
}

// 1.12a: 0x6FACEF60
// Calling convention: custom (eax = pUnit)
int32_t __fastcall D2Client_GetUnitGfxField38(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit == nullptr || pUnit->pGfxData == nullptr)
    {
        return 0;
    }
    return *reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(pUnit->pGfxData) + 0x38);
}

// 1.12a: 0x6FACEFA0
// Calling convention: custom (eax = pUnit, 1 stack arg: nValue)
void __stdcall D2Client_SetUnitGfxField38(D2UnitStrc* pUnit /*eax*/, int32_t nValue)
{
    if (pUnit == nullptr || pUnit->pGfxData == nullptr)
    {
        return;
    }
    *reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(pUnit->pGfxData) + 0x38) = nValue;
}

// 1.12a: 0x6FACEFE0
// Calling convention: custom (eax = pUnit)
int32_t __fastcall D2Client_GetUnitGfxField34(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit == nullptr || pUnit->pGfxData == nullptr)
    {
        return 0;
    }
    return *reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(pUnit->pGfxData) + 0x34);
}

// 1.12a: 0x6FACF020
// Calling convention: custom (eax = pUnit, 1 stack arg: nValue)
void __stdcall D2Client_SetUnitGfxField34(D2UnitStrc* pUnit /*eax*/, int32_t nValue)
{
    if (pUnit == nullptr || pUnit->pGfxData == nullptr)
    {
        return;
    }
    *reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(pUnit->pGfxData) + 0x34) = nValue;
}

// 1.12a: 0x6FACF060
// Calling convention: custom (esi = pUnit, bl = nOldAlignment, 1 stack arg: nNewAlignment)
void __stdcall D2Client_OnMonsterAlignmentChanged(D2UnitStrc* pUnit /*esi*/, uint8_t nOldAlignment /*bl*/, uint8_t nNewAlignment)
{
    if (pUnit == nullptr || pUnit->dwUnitType != UNIT_MONSTER || nOldAlignment == nNewAlignment)
    {
        return;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom == nullptr)
    {
        return;
    }

    if (nNewAlignment == UNIT_ALIGNMENT_GOOD)
    {
        DUNGEON_IncreaseAlliedCountOfRoom(pRoom);
    }
    else if (nOldAlignment == UNIT_ALIGNMENT_GOOD)
    {
        DUNGEON_DecreaseAlliedCountOfRoom(pRoom);
    }

    D2Common_10214(pUnit);
}

// 1.12a: 0x6FACF0B0
// Calling convention: custom (esi = pUnit, edi = pRoom, ebx = nY, 1 stack arg: nX)
void __stdcall D2Client_InitializeTileUnitInRoom(D2UnitStrc* pUnit /*esi*/, D2ActiveRoomStrc* pRoom /*edi*/, int32_t nY /*ebx*/, int32_t nX)
{
    if (pUnit == nullptr)
    {
        return;
    }

    pUnit->dwFlags |= (UNITFLAG_DRAWSHADOW | UNITFLAG_TARGETABLE);
    if (pRoom != nullptr)
    {
        UNITS_InitializeStaticPath(pUnit, pRoom, nX, nY);
        UNITS_BlockCollisionPath(pUnit, pRoom, nX, nY);
        UNITROOM_AddUnitToRoom(pUnit, pRoom);
    }
    else
    {
        UNITS_SetXForStaticUnit(pUnit, nX);
        UNITS_SetYForStaticUnit(pUnit, nY);
    }
}

// 1.12a: 0x6FACF0E0
// Calling convention: custom (ecx = nX, eax = nY)
D2ActiveRoomStrc* __fastcall D2Client_FindRoomForSubtileCoords(int32_t nX /*ecx*/, int32_t nY /*eax*/)
{
    if ((nX == 0 && nY == 0) || gpClientAct == nullptr)
    {
        return nullptr;
    }

    D2ActiveRoomStrc* pRoom = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, nX, nY);
    if (pRoom != nullptr)
    {
        return pRoom;
    }

    pRoom = DUNGEON_StreamRoomAtCoords(gpClientAct, nX / 5, nY / 5);
    if (pRoom != nullptr)
    {
        return pRoom;
    }

    return DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, nX, nY);
}

// 1.12a: 0x6FACF180
// Calling convention: __fastcall (ecx = nUnitType, edx = dwUnitGUID)
void __fastcall D2Client_RemoveServerSideUnit(uint32_t nUnitType, uint32_t dwUnitGUID)
{
    extern void __fastcall D2Client_RemoveUnitFromHashTable(D2UnitStrc** ppHashTable, uint32_t nBucket, uint32_t dwUnitGUID, uint32_t nUnitType);
    D2Client_RemoveUnitFromHashTable(
        gapServerSideUnitHashTables[nUnitType],
        dwUnitGUID & 0x7Fu,
        dwUnitGUID,
        nUnitType);
}

// 1.12a: 0x6FACF1A0
// Calling convention: __fastcall (ecx = dwUnitGUID, edx = nUnitType)
D2UnitStrc* __fastcall D2Client_FindClientSideUnit(uint32_t dwUnitGUID, uint32_t nUnitType)
{
    return D2Client_FindUnitInHashBucket(
        dwUnitGUID & 0x7Fu,
        dwUnitGUID,
        gapClientSideUnitHashTables[nUnitType],
        nUnitType);
}

// 1.12a: 0x6FACF1C0
// Calling convention: __fastcall (ecx = dwUnitGUID, edx = nUnitType)
D2UnitStrc* __fastcall D2Client_FindServerSideUnit(uint32_t dwUnitGUID, uint32_t nUnitType)
{
    return D2Client_FindUnitInHashBucket(
        dwUnitGUID & 0x7Fu,
        dwUnitGUID,
        gapServerSideUnitHashTables[nUnitType],
        nUnitType);
}

// 1.12a: 0x6FACF1E0
// Calling convention: custom (eax = pPlayer, edi = pTargetMonster)
int32_t __fastcall D2Client_CanTargetOwnPetWithEquippedSkill(D2UnitStrc* pPlayer /*eax*/, D2UnitStrc* pTargetMonster /*edi*/)
{
    if (pPlayer == nullptr || pTargetMonster == nullptr)
    {
        return 0;
    }

    if (pPlayer->dwUnitType != UNIT_PLAYER && pTargetMonster->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    bool bHasPetTargetSkill = false;
    if (D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pPlayer))
    {
        const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pRightSkill, "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2890);
        if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
        {
            D2SkillsTxt* pSkillsTxt = &sgptDataTables->pSkillsTxt[nSkillId];
            if (pSkillsTxt->dwFlags[0] & 0x00020000) // TargetPet flag
            {
                bHasPetTargetSkill = true;
            }
        }
    }

    if (!bHasPetTargetSkill)
    {
        if (D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pPlayer))
        {
            const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pLeftSkill, "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2901);
            if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
            {
                D2SkillsTxt* pSkillsTxt = &sgptDataTables->pSkillsTxt[nSkillId];
                if (pSkillsTxt->dwFlags[0] & 0x00020000)
                {
                    bHasPetTargetSkill = true;
                }
            }
        }
    }

    if (!bHasPetTargetSkill)
    {
        return 0;
    }

    return (ROSTERPETS_GetOwnerGUIDForPet(pTargetMonster->dwUnitId) == pPlayer->dwUnitId) ? 1 : 0;
}

// 1.12a: 0x6FACF290
// Calling convention: custom (ebx = pUnit, 1 stack arg: bKeepSharedItems)
void __stdcall D2Client_FreeUnitInventoryItems(D2UnitStrc* pUnit /*ebx*/, int32_t bKeepSharedItems)
{
    if (pUnit == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2688);
        exit(-1);
    }

    D2InventoryStrc* pInventory = pUnit->pInventory;
    if (pInventory == nullptr)
    {
        return;
    }

    D2UnitStrc* pItemNode = INVENTORY_GetFirstItem(pInventory);
    while (pItemNode != nullptr)
    {
        D2UnitStrc* pNextNode = INVENTORY_GetNextItem(pItemNode);
        const uint32_t dwItemGUID = INVENTORY_GetItemGUID(pItemNode);
        D2UnitStrc* pItem = D2Client_FindServerSideUnit(dwItemGUID, UNIT_ITEM);

        if (pItem != nullptr)
        {
            if (pItem->dwUnitType != UNIT_ITEM)
            {
                FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 90);
                exit(-1);
            }

            if (bKeepSharedItems && D2Client_IsItemInAnyPlayerOrMonsterInventory(pItem, pUnit))
            {
                if (!INVENTORY_CompareWithItemsParentInventory(pInventory, pItem))
                {
                    FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2710);
                    exit(-1);
                }
            }
            else
            {
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
                }

                if (!ITEMS_CheckItemFlag(pItem, 0x100, 150, "..\\Source\\D2Client\\UNIT\\Item.cpp"))
                {
                    STATLIST_ExpireUnitStatlist(pUnit, pItem);
                }

                if (!INVENTORY_CompareWithItemsParentInventory(pInventory, pItem))
                {
                    FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2706);
                    exit(-1);
                }

                D2Client_RemoveServerSideUnit(pItem->dwUnitType, pItem->dwUnitId);
            }
        }
        pItemNode = pNextNode;
    }
}

// 1.12a: 0x6FACF3D0
// Calling convention: custom (eax = pUnit)
const Unicode* __fastcall D2Client_GetUnitDisplayName(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2461);
        exit(-1);
    }

    switch (pUnit->dwUnitType)
    {
    case UNIT_MONSTER:
    {
        if (pUnit->pMonsterData != nullptr && pUnit->pMonsterData->wszMonName != nullptr)
        {
            if (Unicode::strlen(pUnit->pMonsterData->wszMonName) != 0)
            {
                return pUnit->pMonsterData->wszMonName;
            }
        }
        D2MonStatsTxt* pMonStats = &sgptDataTables->pMonStatsTxt[pUnit->dwClassId];
        return D2LANG_GetStringFromTblIndex(pMonStats->wNameStr);
    }

    case UNIT_OBJECT:
    {
        D2ObjectsTxt* pObjectsTxt = pUnit->pObjectData->pObjectTxt;
        if ((pObjectsTxt->nSubClass & OBJSUBCLASS_PORTAL) != 0 && pUnit->dwAnimMode == OBJMODE_OPERATING)
        {
            D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
            D2UnitStrc* pWarpTile = D2Client_FindFirstTileUnitInRoomList(DUNGEON_GetFirstUnitInRoom(pRoom));
            if (pWarpTile != nullptr)
            {
                return D2Client_GetWarpDestinationLevelName(pWarpTile, pRoom);
            }

            D2ActiveRoomStrc** ppAdjRooms = nullptr;
            int32_t nAdjRooms = 0;
            DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppAdjRooms, &nAdjRooms);
            for (int32_t i = 0; i < nAdjRooms; ++i)
            {
                if (ppAdjRooms[i] != pRoom)
                {
                    pWarpTile = D2Client_FindFirstTileUnitInRoomList(DUNGEON_GetFirstUnitInRoom(ppAdjRooms[i]));
                    if (pWarpTile != nullptr)
                    {
                        return D2Client_GetWarpDestinationLevelName(pWarpTile, ppAdjRooms[i]);
                    }
                }
            }
        }

        if (pObjectsTxt->nSubClass & 1) // Shrine / Well / Waypoint special strings
        {
            switch (pUnit->dwAnimMode)
            {
            case OBJMODE_NEUTRAL:
                return D2LANG_GetStringFromTblIndex(0xC99);
            case OBJMODE_SPECIAL1:
            case OBJMODE_SPECIAL2:
                return D2LANG_GetStringFromTblIndex(0xC9C);
            case OBJMODE_SPECIAL3:
                return D2LANG_GetStringFromTblIndex(0xC9D);
            default:
                return D2LANG_GetStringFromTblIndex(0xC9B);
            }
        }

        if (UNITS_IsShrine(pUnit))
        {
            return D2LANG_GetStringFromTblIndex(static_cast<uint16_t>(pUnit->pObjectData->InteractType) + 0x2A39);
        }

        if (pObjectsTxt->nLockable != 0 && (pUnit->pObjectData->InteractType & 0x80) != 0)
        {
            return D2LANG_GetStringFromTblIndex(0xCBE); // "Locked"
        }

        return pObjectsTxt->wszName;
    }

    case UNIT_ITEM:
    {
        D2ItemsTxt* pItemsTxt = DATATBLS_GetItemsTxtRecord(pUnit->dwClassId);
        return D2LANG_GetStringFromTblIndex(pItemsTxt->wNameStr);
    }

    default:
        return nullptr;
    }
}

// 1.12a: 0x6FACF5B0
// Calling convention: custom (esi = pUnit)
D2UnitStrc* __fastcall D2Client_ClearActionFrameAndGetTarget(D2UnitStrc* pUnit /*esi*/)
{
    UNITMODE_ClearActionFrame(pUnit);
    if (pUnit->pDynamicPath == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2374);
        exit(-1);
    }
    return PATH_GetTargetUnit(pUnit->pDynamicPath);
}

// 1.12a: 0x6FACF5F0
// Calling convention: custom (eax = pUnit, 1 stack arg: nArg)
void __stdcall D2Client_FaceUnitTowardDynamicTarget(D2UnitStrc* pUnit /*eax*/, int32_t nArg)
{
    UNITMODE_ClearActionFrame(pUnit);
    if (pUnit->pDynamicPath == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2374);
        exit(-1);
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (pTarget != nullptr && pTarget != pUnit)
    {
        const int32_t nTargetX = UNITS_GetClientCoordX(pTarget);
        const int32_t nTargetY = UNITS_GetClientCoordY(pTarget);
        D2Common_10216(pUnit->pDynamicPath, nTargetX, nTargetY, nArg);
    }
}

// 1.12a: 0x6FACF680
// Calling convention: __fastcall (ecx = dwTargetGUID, edx = nTargetType, 1 stack arg: pUnit)
void __fastcall D2Client_SetTargetUnitByTypeAndGUID(uint32_t dwTargetGUID, uint32_t nTargetType, D2UnitStrc* pUnit)
{
    D2UnitStrc* pTarget = D2Client_FindServerSideUnit(dwTargetGUID, nTargetType);
    UNITS_SetTargetUnitForDynamicUnit(pUnit, pTarget);
}

// 1.12a: 0x6FACF6B0
// Calling convention: custom (esi = pTargetUnit, edi = pCasterUnit)
int32_t __fastcall D2Client_CanCasterTargetCorpseWithActiveSkills(D2UnitStrc* pTargetUnit /*esi*/, D2UnitStrc* pCasterUnit /*edi*/)
{
    if (pTargetUnit == nullptr || pTargetUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pCasterUnit);
    const int32_t nRightSkillId = SKILLS_GetSkillIdFromSkill(pRightSkill, "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 1674);
    if (D2Client_CanSkillTargetCorpse(nRightSkillId, pCasterUnit, pTargetUnit))
    {
        return 1;
    }

    D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pCasterUnit);
    const int32_t nLeftSkillId = SKILLS_GetSkillIdFromSkill(pLeftSkill, "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 1678);
    return D2Client_CanSkillTargetCorpse(nLeftSkillId, pCasterUnit, pTargetUnit) != 0;
}

// 1.12a: 0x6FACF710
// Calling convention: custom (eax = pUnit)
D2UnitStrc* __fastcall D2Client_GetUnitOwner(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit == nullptr)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 131);
        exit(-1);
    }

    if (!(pUnit->dwFlagEx & UNITFLAGEX_STOREOWNERINFO))
    {
        return nullptr;
    }

    if (pUnit->dwOwnerType == UNIT_MISSILE)
    {
        return D2Client_FindClientSideUnit(pUnit->dwOwnerGUID, UNIT_MISSILE);
    }
    return D2Client_FindServerSideUnit(pUnit->dwOwnerGUID, pUnit->dwOwnerType);
}

// 1.12a: 0x6FACF780
// Calling convention: custom (eax = pUnit)
uint16_t __fastcall D2Client_GetPlayerPartyId(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit == nullptr || pUnit->dwUnitType != UNIT_PLAYER)
    {
        return 0xFFFF;
    }

    uint8_t* pRosterEntry = static_cast<uint8_t*>(ROSTER_FindEntryByGUID(pUnit->dwUnitId));
    if (pRosterEntry == nullptr)
    {
        return 0xFFFF;
    }
    return *reinterpret_cast<uint16_t*>(pRosterEntry + 0x22);
}

// 1.12a: 0x6FACF7B0
// Calling convention: custom (esi = pPlayer, edi = pTargetUnit)
int32_t __fastcall D2Client_CanTargetAlliedUnitWithEquippedSkill(D2UnitStrc* pPlayer /*esi*/, D2UnitStrc* pTargetUnit /*edi*/)
{
    if (pPlayer == nullptr || pTargetUnit == nullptr)
    {
        return 0;
    }

    if (pPlayer->dwUnitType != UNIT_PLAYER &&
        pTargetUnit->dwUnitType != UNIT_PLAYER &&
        pTargetUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    bool bHasAllyTargetSkill = false;
    if (D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pPlayer))
    {
        const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pRightSkill, "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2943);
        if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
        {
            if (sgptDataTables->pSkillsTxt[nSkillId].dwFlags[0] & 0x00040000) // TargetAlly flag
            {
                bHasAllyTargetSkill = true;
            }
        }
    }

    if (!bHasAllyTargetSkill)
    {
        if (D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pPlayer))
        {
            const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pLeftSkill, "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2952);
            if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
            {
                if (sgptDataTables->pSkillsTxt[nSkillId].dwFlags[0] & 0x00040000)
                {
                    bHasAllyTargetSkill = true;
                }
            }
        }
    }

    if (!bHasAllyTargetSkill)
    {
        return 0;
    }

    uint32_t dwTargetOwnerGUID = pTargetUnit->dwUnitId;
    if (pTargetUnit->dwUnitType == UNIT_MONSTER)
    {
        dwTargetOwnerGUID = ROSTERPETS_GetOwnerGUIDForPet(dwTargetOwnerGUID);
        if (dwTargetOwnerGUID == 0xFFFFFFFFu)
        {
            return 0;
        }
    }

    return ROSTER_ArePlayersInSameParty(pPlayer->dwUnitId, dwTargetOwnerGUID) != 0;
}

// 1.12a: 0x6FACF870
// Calling convention: custom (eax = pUnit)
int32_t __fastcall D2Client_ShouldDrawUnitBeforeEntities(D2UnitStrc* pUnit /*eax*/)
{
    if (pUnit == nullptr)
    {
        return 0;
    }

    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
        return (pUnit->dwAnimMode == PLRMODE_DEAD) ? 1 : 0;

    case UNIT_MONSTER:
        return (pUnit->dwAnimMode == MONMODE_DEAD) ? 1 : 0;

    case UNIT_OBJECT:
    {
        D2ObjectsTxt* pObjectsTxt = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId);
        if (pObjectsTxt->nPreOperate & 2)
        {
            return 1;
        }
        return (pUnit->dwAnimMode == OBJMODE_OPENED && (pObjectsTxt->nPreOperate & 1)) ? 1 : 0;
    }

    case UNIT_MISSILE:
        return (pUnit->dwFlags & UNITFLAG_ISDEAD) ? 1 : 0;

    case UNIT_ITEM:
        return (pUnit->dwItemMode == IMODE_ONGROUND) ? 1 : 0;

    default:
        return 0;
    }
}

// 1.12a: 0x6FACF940
// Calling convention: custom (eax = nStateId, 1 stack arg: pUnit)
void __stdcall D2Client_OnShrineStateEffect(int32_t nStateId /*eax*/, D2UnitStrc* pUnit)
{
    if (nStateId == 151)
    {
        MONSTER_SpawnDeathOverlayOrMissile(pUnit, 396, 0, 0, 0);
    }
    else if (nStateId == 152)
    {
        MONSTER_SpawnDeathOverlayOrMissile(pUnit, 397, 0, 0, 0);
    }
}

// 1.12a: 0x6FACF980
// Calling convention: custom (eax = pTargetUnit, 1 stack arg: pAttackerUnit)
int32_t __stdcall D2Client_AreUnitsHostileOrAttackable(D2UnitStrc* pTargetUnit /*eax*/, D2UnitStrc* pAttackerUnit)
{
    uint32_t nAttackerType = (pAttackerUnit != nullptr) ? pAttackerUnit->dwUnitType : UNIT_TYPES_COUNT;
    uint32_t dwAttackerGUID = (pAttackerUnit != nullptr) ? pAttackerUnit->dwUnitId : 0xFFFFFFFFu;
    uint32_t nTargetType = (pTargetUnit != nullptr) ? pTargetUnit->dwUnitType : UNIT_TYPES_COUNT;
    uint32_t dwTargetGUID = (pTargetUnit != nullptr) ? pTargetUnit->dwUnitId : 0xFFFFFFFFu;

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pAttackerUnit);
    if (DUNGEON_IsRoomInTown(pRoom))
    {
        if (pTargetUnit != nullptr && pTargetUnit->dwUnitType == UNIT_PLAYER && D2Client_IsUnitDeadOrDying(pTargetUnit))
        {
            if (!ROSTER_CheckHostilePvPFlag(dwAttackerGUID, pTargetUnit->dwUnitId))
            {
                return 0;
            }
        }

        if (pAttackerUnit != nullptr && pAttackerUnit->dwUnitType == UNIT_PLAYER &&
            pTargetUnit != nullptr && pTargetUnit->dwUnitType == UNIT_MONSTER)
        {
            if (ROSTERPETS_GetOwnerGUIDForPet(pTargetUnit->dwUnitId) != 0xFFFFFFFFu &&
                !D2Client_CanTargetOwnPetWithEquippedSkill(pAttackerUnit, pTargetUnit))
            {
                return 0;
            }
        }
        return 1;
    }

    if (nAttackerType == UNIT_MONSTER && pAttackerUnit != nullptr && (pAttackerUnit->dwFlagEx & UNITFLAGEX_STOREOWNERINFO))
    {
        if (pTargetUnit != nullptr && pTargetUnit->dwOwnerType == UNIT_PLAYER)
        {
            nAttackerType = UNIT_PLAYER;
            dwAttackerGUID = pAttackerUnit->dwOwnerGUID;
        }
    }

    if (nTargetType == UNIT_MONSTER && pTargetUnit != nullptr && (pTargetUnit->dwFlagEx & UNITFLAGEX_STOREOWNERINFO))
    {
        if (pTargetUnit->dwOwnerType == UNIT_PLAYER)
        {
            nTargetType = UNIT_PLAYER;
            dwTargetGUID = pTargetUnit->dwOwnerGUID;
        }
    }

    if (nAttackerType == nTargetType && dwAttackerGUID == dwTargetGUID)
    {
        return 0;
    }

    if (nAttackerType == UNIT_PLAYER && nTargetType == UNIT_PLAYER)
    {
        if (pTargetUnit != nullptr && pTargetUnit->dwUnitType == UNIT_PLAYER && D2Client_IsUnitDeadOrDying(pTargetUnit))
        {
            if (ROSTER_CheckHostilePvPFlag(dwAttackerGUID, pTargetUnit->dwUnitId))
            {
                return 1;
            }
        }
        return PLAYERLIST_CheckRelationshipFlag(dwAttackerGUID, 8);
    }

    if (nAttackerType == UNIT_PLAYER && (nTargetType == UNIT_ITEM || nTargetType == UNIT_OBJECT))
    {
        return 1;
    }

    return STATLIST_AreUnitsAligned(pAttackerUnit, pTargetUnit) == 0;
}

// 1.12a: 0x6FACFC30
// Calling convention: custom (esi = pNewPlayerUnit)
void __fastcall D2Client_SetLocalPlayerUnit(D2UnitStrc* pNewPlayerUnit /*esi*/)
{
    if (pNewPlayerUnit != nullptr)
    {
        if (pNewPlayerUnit->pDynamicPath != nullptr)
        {
            PATH_SetDirection(pNewPlayerUnit->pDynamicPath, 0);
        }
        gnLocalPlayerCharClass = static_cast<uint8_t>(pNewPlayerUnit->dwClassId);
        gnPlayerStandStillFlag = 0;
    }
    gpPlayerUnit = pNewPlayerUnit;
}

// 1.12a: 0x6FACFC70
// Calling convention: custom (esi = pUnit)
// Inserts `pUnit` into either `gapClientSideUnitHashTables` (if `UNITFLAG_ISASYNC` 0x00200000 is set)
// or `gapServerSideUnitHashTables`, keeping each bucket sorted in descending order of `dwUnitId`.
int32_t __fastcall D2Client_InsertUnitIntoHashTable(D2UnitStrc* pUnit /*esi*/)
{
    const bool bIsClientUnit = (pUnit != nullptr) && ((pUnit->dwFlags & UNITFLAG_ISASYNC) != 0);
    if (bIsClientUnit)
    {
        if (D2Client_FindClientSideUnit(pUnit->dwUnitId, pUnit->dwUnitType) != nullptr)
        {
            return 0;
        }
    }
    else
    {
        if (D2Client_FindServerSideUnit(pUnit->dwUnitId, pUnit->dwUnitType) != nullptr)
        {
            return 0;
        }
    }

    UNITSND_StopUnitLoopingSound(1, pUnit);

    const uint32_t dwGUID = pUnit->dwUnitId;
    const uint32_t nUnitType = pUnit->dwUnitType;
    const uint32_t nBucket = dwGUID & 0x7Fu;

    D2UnitStrc** ppTable = ((pUnit->dwFlags & UNITFLAG_ISASYNC) != 0)
        ? gapClientSideUnitHashTables[nUnitType]
        : gapServerSideUnitHashTables[nUnitType];

    D2UnitStrc* pCurr = ppTable[nBucket];
    D2UnitStrc* pPrev = nullptr;
    while (pCurr != nullptr && pCurr->dwUnitId > dwGUID)
    {
        pPrev = pCurr;
        pCurr = pCurr->pListNext;
    }

    pUnit->pListNext = pCurr;
    if (pPrev != nullptr)
    {
        pPrev->pListNext = pUnit;
    }
    else
    {
        ppTable[nBucket] = pUnit;
    }
    return 1;
}

// 1.12a: 0x6FACFDB0
// Calling convention: __stdcall
void __stdcall D2Client_UpdateObjectsOnPeriodOfDayChange(void)
{
    const int32_t nPeriodOfDay = ENVIRONMENT_GetPeriodOfDayFromAct(gpClientAct, 0);
    if (nPeriodOfDay == gnLastPeriodOfDay)
    {
        return;
    }
    gnLastPeriodOfDay = nPeriodOfDay;

    for (int32_t nBucket = 0; nBucket < 128; ++nBucket)
    {
        for (D2UnitStrc* pObj = gapServerSideUnitHashTables[UNIT_OBJECT][nBucket]; pObj != nullptr; )
        {
            D2UnitStrc* pNext = pObj->pListNext;
            if (pObj->dwUnitType != UNIT_OBJECT)
            {
                FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2188);
                exit(-1);
            }
            OBJECT_UpdatePeriodOfDayLight(pObj);
            pObj = pNext;
        }
    }

    for (int32_t nBucket = 0; nBucket < 128; ++nBucket)
    {
        for (D2UnitStrc* pObj = gapClientSideUnitHashTables[UNIT_OBJECT][nBucket]; pObj != nullptr; )
        {
            D2UnitStrc* pNext = pObj->pListNext;
            if (pObj->dwUnitType != UNIT_OBJECT)
            {
                FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 2198);
                exit(-1);
            }
            OBJECT_UpdatePeriodOfDayLight(pObj);
            pObj = pNext;
        }
    }
}

// 1.12a: 0x6FACFE60
// Calling convention: custom (eax = nComponentIdx, ecx = pUnit, 2 stack args: nFrame, pOutRect)
int32_t __stdcall D2Client_GetUnitComponentBoundingBox(
    int32_t nComponentIdx /*eax*/,
    D2UnitStrc* pUnit /*ecx*/,
    int32_t nFrame,
    RECT* pOutRect)
{
    (void)nComponentIdx;
    (void)nFrame;
    if (pUnit == nullptr || pOutRect == nullptr)
    {
        return 0;
    }

    const int32_t nCoordX = UNITS_GetPrecisionY(pUnit);
    const int32_t nCoordY = UNITS_GetClientCoordY(pUnit);
    pOutRect->left   = nCoordX - 16;
    pOutRect->top    = nCoordY + 16;
    pOutRect->right  = nCoordX + 16;
    pOutRect->bottom = nCoordY - 16;
    return 1;
}

// 1.12a: 0x6FAD0010
// Calling convention: __stdcall (4 stack args: pUnit, nCursorX, nCursorY, bTestPixelHit)
int32_t __stdcall D2Client_HitTestUnit(
    D2UnitStrc* pUnit,
    int32_t nCursorX,
    int32_t nCursorY,
    int32_t bTestPixelHit)
{
    if (gnOpenPanelMode == 3)
    {
        return 0;
    }
    if (gnOpenPanelMode == 2 && gnMouseCursorX < (gnScreenWidth / 2))
    {
        return 0;
    }
    if (gnOpenPanelMode == 1 && gnMouseCursorX >= (gnScreenWidth / 2))
    {
        return 0;
    }

    if (pUnit == nullptr || (pUnit->dwFlags & UNITFLAG_ISASYNC) != 0)
    {
        return 0;
    }

    if (!D2Client_AreUnitsHostileOrAttackable(pUnit, gpPlayerUnit) &&
        !D2Client_CanTargetOwnPetWithEquippedSkill(gpPlayerUnit, pUnit) &&
        !D2Client_CanTargetAlliedUnitWithEquippedSkill(gpPlayerUnit, pUnit))
    {
        return 0;
    }

    bool bTargetable = false;
    if (D2Client_CanCasterTargetCorpseWithActiveSkills(pUnit, gpPlayerUnit))
    {
        if ((pUnit->dwFlags & UNITFLAG_TARGETABLE) != 0)
        {
            bTargetable = true;
        }
        else if (pUnit->dwUnitType == UNIT_MONSTER && D2Client_IsUnitDeadOrDying(pUnit))
        {
            bTargetable = ((pUnit->dwFlags & UNITFLAG_CANBEATTACKED) != 0);
        }
    }
    else
    {
        bTargetable = ((pUnit->dwFlags & UNITFLAG_TARGETABLE) != 0);
    }

    if (!bTargetable || !(pUnit->dwFlagEx & UNITFLAGEX_ISINLOS))
    {
        return 0;
    }

    if (!bTestPixelHit)
    {
        return 1;
    }

    int32_t nMode = UNITS_GetAnimOrSeqMode(pUnit);
    int32_t nClassId = pUnit->dwClassId;
    int32_t nUnitType = pUnit->dwUnitType;
    D2COMMON_11013_ConvertMode(pUnit, &nUnitType, &nClassId, &nMode, "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 1773);

    if (nUnitType == UNIT_TILE)
    {
        return D2Client_HitTestWarpTileBox(pUnit, nCursorX, nCursorY);
    }
    if (nUnitType == UNIT_ITEM)
    {
        if (pUnit->dwItemMode != IMODE_ONGROUND || gbHideGroundItems != 0)
        {
            return 0;
        }
    }

    int32_t nShiftedFrame = 0;
    int32_t nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nShiftedFrame, &nFrameCount);

    RECT rcBox = {};
    if (!D2Client_GetUnitComponentBoundingBox(1, pUnit, nShiftedFrame, &rcBox))
    {
        return 0;
    }

    if (nCursorX < rcBox.left || nCursorX > rcBox.right || nCursorY < rcBox.bottom || nCursorY > rcBox.top)
    {
        return 0;
    }
    return 1;
}

// 1.12a: 0x6FAD0490
// Calling convention: __stdcall (1 stack arg: pUnit)
void __stdcall D2Client_DestroyAndFreeUnit(D2UnitStrc* pUnit)
{
    if (pUnit->pLight != nullptr)
    {
        DLIGHTMAP_RemoveUnitLight(pUnit->pLight);
        pUnit->pLight = nullptr;
    }

    D2GfxLightStrc* pDetachedLight = reinterpret_cast<D2GfxLightStrc*>(D2Common_10958(pUnit, 0));
    if (pDetachedLight != nullptr)
    {
        DLIGHTMAP_RemoveUnitLight(pDetachedLight);
    }

    if (pUnit->pGfxData != nullptr)
    {
        void** ppParticleData = reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(pUnit->pGfxData) + 0x30);
        if (*ppParticleData != nullptr)
        {
            FOG_FreeClientMemory(*ppParticleData, "..\\Source\\D2Client\\ENGINE\\Particle.cpp", 100, 0);
            if (pUnit->pGfxData != nullptr)
            {
                *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(pUnit->pGfxData) + 0x30) = nullptr;
            }
        }
    }

    UNITSND_DeinitUnitAudio(pUnit, 0);
    UNITSND_StopUnitLoopingSound(-1, pUnit);

    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
        pUnit->pHoverText = nullptr;
        PLAYER_FreeClientPlayer(pUnit);
        UNITSND_DeinitUnitAudio(pUnit, 1);
        break;

    case UNIT_MONSTER:
        pUnit->pHoverText = nullptr;
        MONSTER_FreeClientMonster(pUnit);
        UNITSND_DeinitUnitAudio(pUnit, 1);
        break;

    case UNIT_OBJECT:
        pUnit->pHoverText = nullptr;
        OBJECT_FreeClientObject(pUnit);
        UNITS_FreeStaticPath(pUnit);
        UNITSND_DeinitUnitAudio(pUnit, 1);
        break;

    case UNIT_MISSILE:
        MISSILE_FreeClientMissile(pUnit);
        break;

    case UNIT_ITEM:
        ITEM_FreeClientItem(pUnit);
        ITEMS_FreeItemData(nullptr, pUnit);
        UNITS_FreeStaticPath(pUnit);
        break;

    case UNIT_TILE:
        if (gpClientAct != nullptr)
        {
            UNITROOM_RemoveUnitFromRoom(pUnit);
        }
        UNITS_FreeStaticPath(pUnit);
        break;

    default:
        break;
    }

    UNITEVENT_FreeEventList(pUnit);
    SCMD_FreeUnitPacketQueue(pUnit);

    if (pUnit == gpPlayerUnit)
    {
        gpPlayerUnit = nullptr;
    }

    UNITS_FreeUnit(pUnit);
}

// 1.12a: 0x6FAD0620
// Calling convention: __fastcall (ecx = ppHashTable, edx = nBucket, 2 stack args: dwUnitGUID, nUnitType)
void __fastcall D2Client_RemoveUnitFromHashTable(
    D2UnitStrc** ppHashTable,
    uint32_t nBucket,
    uint32_t dwUnitGUID,
    uint32_t nUnitType)
{
    D2UnitStrc* pPrev = nullptr;
    D2UnitStrc* pCurr = ppHashTable[nBucket];
    while (pCurr != nullptr)
    {
        if (pCurr->dwUnitId == dwUnitGUID && pCurr->dwUnitType == nUnitType)
        {
            if (pPrev == nullptr)
            {
                ppHashTable[nBucket] = pCurr->pListNext;
            }
            else
            {
                pPrev->pListNext = pCurr->pListNext;
            }

            D2Client_DestroyAndFreeUnit(pCurr);
            return;
        }
        pPrev = pCurr;
        pCurr = pCurr->pListNext;
    }
}

// 1.12a: 0x6FAD0690
// Calling convention: custom (esi = pUnit, ecx = nStateId, eax = bSet)
void __fastcall D2Client_UpdateShrineStateOverlay(D2UnitStrc* pUnit /*esi*/, int32_t nStateId /*ecx*/, int32_t bSet /*eax*/)
{
    if (pUnit == nullptr)
    {
        return;
    }

    D2Client_OnShrineStateEffect(nStateId, pUnit);
}

// 1.12a: 0x6FAD06F0
// Calling convention: __stdcall (1 stack arg: pUnit)
int32_t __stdcall D2Client_IsSelectedUnitNotRestrictedObject(D2UnitStrc* pUnit)
{
    D2UnitStrc* pSelected = SELECT_GetSelectedUnit();
    if (pSelected != nullptr && pSelected->dwUnitType == UNIT_OBJECT)
    {
        D2ObjectsTxt* pObjectsTxt = DATATBLS_GetObjectsTxtRecord(pSelected->dwClassId);
        if (pObjectsTxt->nDraw == 0)
        {
            return 0;
        }
    }
    return (SELECT_GetSelectedUnit() == pUnit) ? 1 : 0;
}

// 1.12a: 0x6FAD0730
// Calling convention: __stdcall
void __stdcall D2Client_FreeAllUnits(void)
{
    for (int32_t i = 0; i < 6; ++i)
    {
        const int32_t nUnitType = g_anUnitFreeOrder[i];
        for (int32_t nBucket = 0; nBucket < 128; ++nBucket)
        {
            D2UnitStrc* pUnit = gapServerSideUnitHashTables[nUnitType][nBucket];
            while (pUnit != nullptr)
            {
                D2UnitStrc* pNext = pUnit->pListNext;
                D2Client_DestroyAndFreeUnit(pUnit);
                gapServerSideUnitHashTables[nUnitType][nBucket] = pNext;
                pUnit = pNext;
            }
        }
    }

    for (int32_t i = 0; i < 6; ++i)
    {
        const int32_t nUnitType = g_anUnitFreeOrder[i];
        for (int32_t nBucket = 0; nBucket < 128; ++nBucket)
        {
            D2UnitStrc* pUnit = gapClientSideUnitHashTables[nUnitType][nBucket];
            while (pUnit != nullptr)
            {
                D2UnitStrc* pNext = pUnit->pListNext;
                D2Client_DestroyAndFreeUnit(pUnit);
                gapClientSideUnitHashTables[nUnitType][nBucket] = pNext;
                pUnit = pNext;
            }
        }
    }
}

// 1.12a: 0x6FAD07F0
// Calling convention: custom (eax = nClassId, ecx = pUnit)
void __fastcall D2Client_ApplyMonsterSpawnSpecialEffects(int32_t nClassId /*eax*/, D2UnitStrc* pUnit /*ecx*/)
{
    if (pUnit == nullptr)
    {
        return;
    }

    // Special monster ambient particle/overlay attachment (Catapults, Baal Throne, Shenk, Wisp, etc.)
    if (nClassId == 406 && pUnit->pGfxData != nullptr)
    {
        uint32_t* pParticle = *reinterpret_cast<uint32_t**>(reinterpret_cast<uint8_t*>(pUnit->pGfxData) + 0x30);
        if (pParticle != nullptr)
        {
            pParticle[0] |= 0x20;
        }
    }
}

// 1.12a: 0x6FAD0A70
// Calling convention: __fastcall (ecx = nUnitType, edx = dwUnitGUID)
void __fastcall D2Client_RemoveClientSideUnit(uint32_t nUnitType, uint32_t dwUnitGUID)
{
    D2Client_RemoveUnitFromHashTable(
        gapClientSideUnitHashTables[nUnitType],
        dwUnitGUID & 0x7Fu,
        dwUnitGUID,
        nUnitType);
}

// 1.12a: 0x6FAD0A90
// Calling convention: custom (eax = pUnit)
// Inserts `pUnit` into the unit hash table, replacing any stale unit with the same GUID/type.
D2UnitStrc* __fastcall D2Client_RegisterUnitInHashTable(D2UnitStrc* pUnit /*eax*/)
{
    if (D2Client_InsertUnitIntoHashTable(pUnit))
    {
        return pUnit;
    }

    D2UnitStrc* pExisting = D2Client_FindServerSideUnit(pUnit->dwUnitId, pUnit->dwUnitType);
    if (pExisting == pUnit)
    {
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 454);
        exit(-1);
    }

    if (pExisting != nullptr)
    {
        if (pExisting->dwFlags & UNITFLAG_ISASYNC)
        {
            D2Client_RemoveClientSideUnit(pExisting->dwUnitType, pExisting->dwUnitId);
        }
        else
        {
            D2Client_RemoveServerSideUnit(pExisting->dwUnitType, pExisting->dwUnitId);
        }

        if (D2Client_InsertUnitIntoHashTable(pUnit))
        {
            return pUnit;
        }
    }

    D2Client_DestroyAndFreeUnit(pUnit);
    return nullptr;
}

// 1.12a: 0x6FAD0B30
// Calling convention: custom (eax = nX, ecx = pUnit, edx = dwUnitGUID, 5 stack args: nY, nClassId, nMode, nUnitType, bInitMode)
D2UnitStrc* __stdcall D2Client_InitializeUnitInstance(
    int32_t nX /*eax*/,
    D2UnitStrc* pUnit /*ecx*/,
    uint32_t dwUnitGUID /*edx*/,
    int32_t nY,
    int32_t nClassId,
    int32_t nMode,
    uint32_t nUnitType,
    uint8_t nObjMode)
{
    if (pUnit != nullptr)
    {
        if (gbIsExpansion)
        {
            pUnit->dwFlagEx |= UNITFLAGEX_ISEXPANSION;
        }
        else
        {
            pUnit->dwFlagEx &= ~UNITFLAGEX_ISEXPANSION;
        }
    }

    pUnit->dwUnitType  = nUnitType;
    pUnit->dwClassId   = nClassId;
    pUnit->dwUnitId    = dwUnitGUID;
    pUnit->dwTickCount = 0;
    pUnit->pMemoryPool = nullptr;

    D2ActiveRoomStrc* pRoom = nullptr;
    if (nX != 0 || nY != 0)
    {
        pRoom = D2Client_FindRoomForSubtileCoords(nX, nY);
        const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
        pUnit->nAct = DRLG_GetActNoFromLevelId(nLevelId);
        pUnit->pDrlgAct = gpClientAct;
    }

    SEED_InitSeed(&pUnit->pSeed);

    if (nUnitType == UNIT_ITEM)
    {
        ITEMS_AllocItemData(nullptr, pUnit);
        ITEMS_GetUniqueColumnFromItemsTxt(pUnit);
    }

    if (pRoom != nullptr)
    {
        // Step room seed and initialize unit seed
        uint32_t* pRoomSeed = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(pRoom) + 0x0C);
        uint64_t uProd = static_cast<uint64_t>(pRoomSeed[0]) * 0x6AC690C5ULL + pRoomSeed[1];
        pRoomSeed[0] = static_cast<uint32_t>(uProd);
        pRoomSeed[1] = static_cast<uint32_t>(uProd >> 32);
        pUnit->dwInitSeed = pRoomSeed[0];
        SEED_InitLowSeed(&pUnit->pSeed, pUnit->dwInitSeed);
    }

    switch (nUnitType)
    {
    case UNIT_PLAYER:
        PLAYER_InitClientPlayer(pUnit, pRoom, nX, nY);
        break;

    case UNIT_MONSTER:
        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 514);
        exit(-1);
        break;

    case UNIT_OBJECT:
        UNITS_AllocStaticPath(pUnit);
        OBJECT_InitClientObject(nMode, pUnit, pRoom, nX, nClassId, nObjMode);
        break;

    case UNIT_MISSILE:
        MISSILE_InitClientMissile(pUnit, pRoom, nX, nY);
        break;

    case UNIT_ITEM:
        UNITS_AllocStaticPath(pUnit);
        ITEM_InitClientItem(pUnit, nX, nY, nMode, nObjMode);
        break;

    case UNIT_TILE:
        UNITS_AllocStaticPath(pUnit);
        D2Client_InitializeTileUnitInRoom(pUnit, pRoom, nY, nX);
        break;

    default:
        break;
    }

    if (nUnitType == UNIT_PLAYER || nUnitType == UNIT_MONSTER)
    {
        UNITS_UpdateAnimRateAndVelocity(pUnit, "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 541);
    }

    pUnit = D2Client_RegisterUnitInHashTable(pUnit);
    if (pUnit != nullptr && nUnitType == UNIT_OBJECT)
    {
        OBJECT_PostCreateInit(pUnit);
    }
    return pUnit;
}

// 1.12a: 0x6FAD0CF0
// Calling convention: custom (esi = nUnitType, 7 stack args)
D2UnitStrc* __stdcall D2Client_AllocClientObjectUnit(
    uint32_t nUnitType /*esi*/,
    int32_t nClassId,
    uint32_t dwUnitGUID,
    int32_t nX,
    int32_t nY,
    int32_t nMode,
    uint8_t nInteractType,
    uint8_t nPortalFlags)
{
    (void)nPortalFlags;
    D2UnitStrc* pUnit = UNITS_AllocUnit(nullptr, nUnitType);
    if (pUnit == nullptr)
    {
        return nullptr;
    }

    pUnit = D2Client_InitializeUnitInstance(nX, pUnit, dwUnitGUID, nY, nClassId, nMode, nUnitType, nInteractType);
    if (pUnit != nullptr && pUnit->pObjectData != nullptr)
    {
        pUnit->pObjectData->InteractType = nInteractType;
        if (UNITS_IsShrine(pUnit))
        {
            D2ShrinesTxt* pShrinesTxt = DATATBLS_GetShrinesTxtRecord(pUnit->pObjectData->InteractType);
            UNITS_SetShrineTxtRecordInObjectData(pUnit, pShrinesTxt);
            OBJECT_InitShrineVisual(pUnit);
        }
    }
    return pUnit;
}

// 1.12a: 0x6FAD0D60
// Calling convention: __stdcall (5 stack args: dwUnitGUID, nClassId, nX, nY, szPlayerName)
D2UnitStrc* __stdcall D2Client_AllocClientPlayerUnit(
    uint32_t dwUnitGUID,
    int32_t nClassId,
    int32_t nX,
    int32_t nY,
    char* szPlayerName)
{
    D2UnitStrc* pUnit = UNITS_AllocUnit(nullptr, UNIT_PLAYER);
    if (pUnit == nullptr)
    {
        return nullptr;
    }

    if (gbIsExpansion)
    {
        pUnit->dwFlagEx |= UNITFLAGEX_ISEXPANSION;
    }
    else
    {
        pUnit->dwFlagEx &= ~UNITFLAGEX_ISEXPANSION;
    }

    pUnit->dwUnitType  = UNIT_PLAYER;
    pUnit->dwClassId   = nClassId;
    pUnit->dwUnitId    = dwUnitGUID;
    pUnit->dwTickCount = 0;
    pUnit->pMemoryPool = nullptr;

    D2ActiveRoomStrc* pRoom = nullptr;
    if (nX != 0 || nY != 0)
    {
        pRoom = D2Client_FindRoomForSubtileCoords(nX, nY);
        const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
        pUnit->nAct = DRLG_GetActNoFromLevelId(nLevelId);
        pUnit->pDrlgAct = gpClientAct;
    }

    SEED_InitSeed(&pUnit->pSeed);
    if (pRoom != nullptr)
    {
        uint32_t* pRoomSeed = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(pRoom) + 0x0C);
        uint64_t uProd = static_cast<uint64_t>(pRoomSeed[0]) * 0x6AC690C5ULL + pRoomSeed[1];
        pRoomSeed[0] = static_cast<uint32_t>(uProd);
        pRoomSeed[1] = static_cast<uint32_t>(uProd >> 32);
        pUnit->dwInitSeed = pRoomSeed[0];
        SEED_InitLowSeed(&pUnit->pSeed, pUnit->dwInitSeed);
    }

    PLAYER_InitClientPlayer(pUnit, pRoom, nX, nY);
    UNITS_UpdateAnimRateAndVelocity(pUnit, "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 541);

    pUnit = D2Client_RegisterUnitInHashTable(pUnit);
    if (pUnit != nullptr && szPlayerName != nullptr)
    {
        UNITS_SetNameInPlayerData(pUnit, szPlayerName);
    }
    return pUnit;
}

// 1.12a: 0x6FAD0E70
// Calling convention: custom (esi = nUnitType, 5 stack args)
D2UnitStrc* __stdcall D2Client_AllocClientUnit(
    uint32_t nUnitType /*esi*/,
    int32_t nClassId,
    int32_t nX,
    int32_t nY,
    uint32_t dwUnitGUID,
    int32_t nMode)
{
    D2UnitStrc* pUnit = UNITS_AllocUnit(nullptr, nUnitType);
    if (pUnit == nullptr)
    {
        return nullptr;
    }
    return D2Client_InitializeUnitInstance(nX, pUnit, dwUnitGUID, nY, nClassId, nMode, nUnitType, 0);
}

// 1.12a: 0x6FAD0EB0
// Calling convention: __stdcall (6 stack args: pSpawnDesc, pCompInfo, pMonUMods, wNameSeedOrAsync, wBossHcIdx, nExtra)
D2UnitStrc* __stdcall D2Client_AllocClientMonsterUnit(
    const D2MonsterSpawnDescStrc* pSpawnDesc,
    const uint8_t* pCompInfo,
    const uint8_t* pMonUMods,
    uint32_t dwNameSeedOrAsync,
    uint16_t wBossHcIdx,
    int32_t nExtra)
{
    (void)nExtra;
    // Check if local player's hireling with this GUID already exists in the monster hash table
    uint32_t dwHirelingGUID = 0xFFFFFFFFu;
    if (gpPlayerUnit != nullptr)
    {
        for (D2RosterPetEntryStrc* pPet = gpRosterPetList; pPet != nullptr; pPet = pPet->pNext)
        {
            if (pPet->nPetType == 7 && pPet->dwOwnerGUID == gpPlayerUnit->dwUnitId)
            {
                dwHirelingGUID = pPet->dwUnitGUID;
                break;
            }
        }
    }

    if (pSpawnDesc->dwUnitGUID == dwHirelingGUID)
    {
        D2UnitStrc* pExistingMerc = D2Client_FindServerSideUnit(pSpawnDesc->dwUnitGUID, UNIT_MONSTER);
        if (pExistingMerc != nullptr)
        {
            UNITS_ChangeAnimMode(pExistingMerc, pSpawnDesc->nAnimMode);
            return pExistingMerc;
        }
    }

    const int32_t nClassId = pSpawnDesc->nClassId;
    if (nClassId < 0 || nClassId >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        return nullptr;
    }

    D2MonStatsTxt* pMonStatsTxt = &sgptDataTables->pMonStatsTxt[nClassId];
    const int16_t nMonStats2Idx = pMonStatsTxt->wMonStatsEx;
    if (nMonStats2Idx < 0 || nMonStats2Idx >= sgptDataTables->nMonStats2TxtRecordCount)
    {
        return nullptr;
    }

    D2UnitStrc* pUnit = UNITS_AllocUnit(nullptr, UNIT_MONSTER);
    if (pUnit == nullptr)
    {
        return nullptr;
    }

    if (dwNameSeedOrAsync != 0)
    {
        pUnit->dwFlags |= (UNITFLAG_ISASYNC | UNITFLAG_ISCLIENTUNIT);
    }

    pUnit->dwUnitType  = UNIT_MONSTER;
    pUnit->dwClassId   = nClassId;
    pUnit->dwUnitId    = pSpawnDesc->dwUnitGUID;
    pUnit->pMemoryPool = nullptr;
    pUnit->dwTickCount = 0;

    D2ActiveRoomStrc* pRoom = D2Client_FindRoomForSubtileCoords(pSpawnDesc->nX, pSpawnDesc->nY);
    SEED_InitSeed(&pUnit->pSeed);
    if (pRoom != nullptr)
    {
        uint32_t* pRoomSeed = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(pRoom) + 0x0C);
        uint64_t uProd = static_cast<uint64_t>(pRoomSeed[0]) * 0x6AC690C5ULL + pRoomSeed[1];
        pRoomSeed[0] = static_cast<uint32_t>(uProd);
        pRoomSeed[1] = static_cast<uint32_t>(uProd >> 32);
        pUnit->dwInitSeed = pRoomSeed[0];
    }
    SEED_InitLowSeed(&pUnit->pSeed, pUnit->dwInitSeed);

    MONSTER_InitClientMonsterData(pUnit, 0);

    if (pUnit->pMonsterData != nullptr)
    {
        pUnit->pMonsterData->nTypeFlag |= static_cast<uint8_t>(pSpawnDesc->wTypeFlags);
        if (pUnit->pMonsterData->nTypeFlag & 2) // SuperUnique
        {
            pUnit->pMonsterData->wBossHcIdx = wBossHcIdx;
        }
        pUnit->pMonsterData->unk0x3C.unk0x00 = pSpawnDesc->nLifePct;
    }

    const Unicode* wszDefaultName = D2LANG_GetStringFromTblIndex(pMonStatsTxt->wNameStr);
    MONSTERS_SetMonsterNameInMonsterData(pUnit, wszDefaultName);

    if (pMonUMods != nullptr && pUnit->pMonsterData != nullptr)
    {
        pUnit->pMonsterData->wNameSeed = static_cast<uint16_t>(dwNameSeedOrAsync);
        memcpy(pUnit->pMonsterData->nMonUmod, pMonUMods, 9);
    }

    if (pCompInfo != nullptr && pUnit->pMonsterData != nullptr)
    {
        memcpy(pUnit->pMonsterData->nComponent, pCompInfo, 16);
    }

    MONSTER_SpawnClientMonster(pUnit, pRoom, pSpawnDesc->nX, pSpawnDesc->nY, nClassId, pSpawnDesc->nAnimMode);
    PATH_SetStepNum(pUnit->pDynamicPath, pMonStatsTxt->nVelocity);
    D2Client_ApplyMonsterSpawnSpecialEffects(nClassId, pUnit);

    // Compute monster palette shift index
    int32_t nPalShift = (pMonStatsTxt->nTransLvl < 8) ? (pMonStatsTxt->nTransLvl + 2) : 2;
    D2Client_SetUnitGfxField34(pUnit, nPalShift);
    D2Client_SetUnitGfxField38(pUnit, nPalShift);

    pUnit = D2Client_RegisterUnitInHashTable(pUnit);
    if (pUnit != nullptr)
    {
        if (MONSTERS_GetHirelingTypeId(pUnit) != 0)
        {
            INVENTORY_AllocInventory(nullptr, pUnit);
        }
        UNITSND_InitMonsterAudio(pUnit);
    }
    return pUnit;
}

// 1.12a: 0x6FAD1320
// Calling convention: __fastcall (ecx = nX, edx = nY, 3 stack args: nClassId, nUnitType, nMode)
D2UnitStrc* __fastcall D2Client_CreateClientSideUnit(
    int32_t nX /*ecx*/,
    int32_t nY /*edx*/,
    int32_t nClassId,
    uint32_t nUnitType,
    int32_t nMode)
{
    uint32_t dwGUID = ++gdwNextClientUnitGUID;
    if (dwGUID == 0xFFFFFFFFu)
    {
        dwGUID = 0;
        gdwNextClientUnitGUID = 0;
    }

    if (nUnitType == UNIT_MONSTER)
    {
        D2MonsterSpawnDescStrc spawnDesc = {};
        spawnDesc.dwUnitGUID = dwGUID;
        spawnDesc.nClassId   = nClassId;
        spawnDesc.nX         = nX;
        spawnDesc.nY         = nY;
        spawnDesc.nAnimMode  = nMode;
        return D2Client_AllocClientMonsterUnit(&spawnDesc, nullptr, nullptr, 1, 0, 0);
    }

    D2UnitStrc* pUnit = UNITS_AllocUnit(nullptr, nUnitType);
    if (pUnit == nullptr)
    {
        return nullptr;
    }

    pUnit->dwFlags |= (UNITFLAG_ISASYNC | UNITFLAG_ISCLIENTUNIT);
    return D2Client_InitializeUnitInstance(nX, pUnit, gdwNextClientUnitGUID, nY, nClassId, nMode, nUnitType, 0);
}

// 1.12a: 0x6FAD13F0
// Calling convention: custom (ecx = pRoom, esi = pPresetUnit)
D2UnitStrc* __fastcall D2Client_SpawnSinglePresetUnitInRoom(D2ActiveRoomStrc* pRoom /*ecx*/, D2PresetUnitStrc* pPresetUnit /*esi*/)
{
    D2DrlgCoordsStrc roomCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &roomCoords);
    return D2Client_CreateClientSideUnit(
        roomCoords.nSubtileX + pPresetUnit->nXpos,
        roomCoords.nSubtileY + pPresetUnit->nYpos,
        pPresetUnit->nIndex,
        pPresetUnit->nUnitType,
        pPresetUnit->nMode);
}

// 1.12a: 0x6FAD1430
// Calling convention: custom (edi = pRoom)
void __fastcall D2Client_SpawnPresetClientUnitsInRoom(D2ActiveRoomStrc* pRoom /*edi*/)
{
    for (D2PresetUnitStrc* pPreset = DUNGEON_GetPresetUnitsFromRoom(pRoom);
         pPreset != nullptr;
         pPreset = pPreset->pNext)
    {
        if (pPreset->bClientDirty & 1)
        {
            D2DrlgCoordsStrc roomCoords = {};
            DUNGEON_GetRoomCoordinates(pRoom, &roomCoords);
            D2Client_CreateClientSideUnit(
                roomCoords.nSubtileX + pPreset->nXpos,
                roomCoords.nSubtileY + pPreset->nYpos,
                pPreset->nIndex,
                pPreset->nUnitType,
                pPreset->nMode);
        }
    }
}

// 1.12a: 0x6FAD1480
// Calling convention: custom (edi = pUnit)
void __fastcall D2Client_ResetPlayerNeutralModeInRoom(D2UnitStrc* pUnit /*edi*/)
{
    if (pUnit == nullptr || pUnit->dwUnitType != UNIT_PLAYER)
    {
        return;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom == nullptr)
    {
        return;
    }

    if (pUnit->dwAnimMode != PLRMODE_NEUTRAL && pUnit->dwAnimMode != PLRMODE_TOWNNEUTRAL)
    {
        return;
    }

    D2GfxLightStrc* pDetachedLight = reinterpret_cast<D2GfxLightStrc*>(D2Common_10958(pUnit, 0));
    if (pDetachedLight != nullptr)
    {
        DLIGHTMAP_RemoveUnitLight(pDetachedLight);
    }

    PLAYER_ResetSkillState(pUnit, (pUnit == gpPlayerUnit) ? 1 : 0);
    UNITS_SetUsedSkill(pUnit, nullptr);

    if (pUnit->dwAnimMode == PLRMODE_KNOCKBACK)
    {
        PATH_RecacheRoomIfNeeded(pUnit->pDynamicPath);
    }
    PATH_ResetToPreviousType(pUnit->pDynamicPath);

    const int32_t nNeutralMode = DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)) ? PLRMODE_TOWNNEUTRAL : PLRMODE_NEUTRAL;
    pUnit->dwFlags = (pUnit->dwFlags & ~UNITFLAG_SKSRVDOFUNC) | UNITFLAG_TARGETABLE;

    if (UNITS_ChangeAnimMode(pUnit, nNeutralMode))
    {
        UNITMODE_OnModeChangeComplete(nNeutralMode, pUnit);
        UNITSND_PlayUnitModeSound(pUnit, nNeutralMode, 0);
    }
}

// 1.12a: 0x6FAD1550
// Calling convention: custom (eax = pUnit, 3 stack args: nX, nY, bTeleportEffect)
void __stdcall D2Client_TeleportUnitToCoordinates(D2UnitStrc* pUnit /*eax*/, int32_t nX, int32_t nY, int32_t bTeleportEffect)
{
    if (pUnit == nullptr)
    {
        return;
    }

    if (pUnit == gpPlayerUnit)
    {
        gnSelectedTargetA = 0;
        gnSelectedTargetB = 0;
    }

    D2ActiveRoomStrc* pDestRoom = D2Client_FindRoomForSubtileCoords(nX, nY);
    if (pUnit->pDynamicPath == nullptr)
    {
        PATH_AllocDynamicPath(pUnit->pMemoryPool, pDestRoom, nX, nY, pUnit, FALSE);
        if (pUnit->pDynamicPath == nullptr)
        {
            return;
        }
    }
    if (nX != 0 && nY != 0 && pDestRoom == nullptr)
    {
        return;
    }

    if (D2Client_IsUnitDeadOrDying(pUnit))
    {
        return;
    }

    if (!D2Common_10229(pUnit->pDynamicPath, pUnit, pDestRoom, nX, nY))
    {
        D2CoordStrc freeCoord = { nX, nY };
        const int32_t nSizeX = UNITS_GetUnitSizeX(pUnit);
        if (pDestRoom != nullptr)
        {
            COLLISION_GetFreeCoordinates(pDestRoom, &freeCoord, nSizeX, 0x1C09, 1);
            D2Common_10231(pUnit->pDynamicPath, pUnit, pDestRoom, freeCoord.nX, freeCoord.nY);
        }
    }

    gnAutomapRefreshState = 2;
    (void)bTeleportEffect;
    D2Client_ResetPlayerNeutralModeInRoom(pUnit);
}

// 1.12a: 0x6FAD1760
// Calling convention: __stdcall (1 stack arg: ppClientHashTable)
void __stdcall D2Client_UpdateClientSideHashTable(D2UnitStrc** ppClientHashTable)
{
    for (int32_t nBucket = 0; nBucket < 128; ++nBucket)
    {
        D2UnitStrc* pUnit = ppClientHashTable[nBucket];
        while (pUnit != nullptr)
        {
            D2UnitStrc* pNext = pUnit->pListNext;
            const uint32_t dwGUID = pUnit->dwUnitId;
            const uint32_t nUnitType = pUnit->dwUnitType;

            UNITMODE_UpdateUnitFrame(pUnit);

            D2UnitStrc* pStillAlive = D2Client_FindClientSideUnit(dwGUID, nUnitType);
            if (pStillAlive != nullptr)
            {
                if (pStillAlive->dwUnitType != nUnitType)
                {
                    FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 90);
                    exit(-1);
                }

                if (pStillAlive->dwUnitType == UNIT_OBJECT)
                {
                    D2ObjectsTxt* pObjectsTxt = DATATBLS_GetObjectsTxtRecord(pStillAlive->dwClassId);
                    if (pObjectsTxt->nClientFn >= 19)
                    {
                        FOG_DisplayHalt("Halt", "..\\Source\\D2Client\\UNIT\\CUnit.cpp", 1350);
                        exit(-1);
                    }
                    if (pObjectsTxt->nClientFn != 0)
                    {
                        extern void (__fastcall* g_apfnObjectClientFns[19])(D2UnitStrc*); // 0x6FBA3C58
                        g_apfnObjectClientFns[pObjectsTxt->nClientFn](pStillAlive);
                    }
                }
            }
            pUnit = pNext;
        }
    }
}

// 1.12a: 0x6FAD1840
// Calling convention: __stdcall
// Master per-tick client unit update loop:
//   1. Steps animation frames for all server-side & client-side units
//   2. Runs client object/monster callbacks
//   3. Expires temporary stat modifiers
//   4. In single-player mode, syncs client units with local D2Game server units
void __stdcall D2Client_UpdateAllUnitsPerTick(void)
{
    D2Client_ForEachUnitInHashTable(&UNITMODE_UpdateUnitFrame, gapServerSideUnitHashTables[UNIT_MISSILE]);
    D2Client_UpdateClientSideHashTable(gapClientSideUnitHashTables[UNIT_MISSILE]);
    D2Client_UpdateClientSideHashTable(gapClientSideUnitHashTables[UNIT_OBJECT]);

    D2Client_ForEachUnitInHashTable(&UNITMODE_UpdateUnitFrame, gapServerSideUnitHashTables[UNIT_PLAYER]);
    D2Client_ForEachUnitInHashTable(&UNITMODE_UpdateUnitFrame, gapServerSideUnitHashTables[UNIT_MONSTER]);
    D2Client_ForEachUnitInHashTable(&UNITMODE_UpdateUnitFrame, gapServerSideUnitHashTables[UNIT_OBJECT]);
    D2Client_ForEachUnitInHashTable(&UNITMODE_UpdateUnitFrame, gapServerSideUnitHashTables[UNIT_ITEM]);
    D2Client_UpdateClientSideHashTable(gapClientSideUnitHashTables[UNIT_MONSTER]);

    D2Client_ForEachUnitInHashTable(&D2Client_NullUnitCallback, gapClientSideUnitHashTables[UNIT_MONSTER]);
    D2Client_ForEachUnitInHashTable(&D2Client_NullUnitCallback, gapServerSideUnitHashTables[UNIT_ITEM]);
    D2Client_ForEachUnitInHashTable(&D2Client_NullUnitCallback, gapServerSideUnitHashTables[UNIT_PLAYER]);
    D2Client_ForEachUnitInHashTable(&D2Client_NullUnitCallback, gapServerSideUnitHashTables[UNIT_MONSTER]);

    D2Client_ForEachUnitInHashTable(&D2Client_ExpireUnitModStatsCallback, gapClientSideUnitHashTables[UNIT_MONSTER]);
    D2Client_ForEachUnitInHashTable(&D2Client_ExpireUnitModStatsCallback, gapServerSideUnitHashTables[UNIT_ITEM]);
    D2Client_ForEachUnitInHashTable(&D2Client_ExpireUnitModStatsCallback, gapServerSideUnitHashTables[UNIT_PLAYER]);

    if (gnGameType == 0)
    {
        D2Client_ForEachUnitInHashTable(&D2Client_SyncSinglePlayerUnitWithServer, gapServerSideUnitHashTables[UNIT_PLAYER]);
        D2Client_ForEachUnitInHashTable(&D2Client_SyncSinglePlayerUnitWithServer, gapServerSideUnitHashTables[UNIT_MONSTER]);
    }
}
