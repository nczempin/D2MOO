// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Source
// Source File : ..\Source\D2Client\UNIT\Object.cpp
// .text Range : 0x6FAE9D40..0x6FAEB9D0 (42 functions)
//               - 0x6FAE9D40..0x6FAE9D60 (from tail of UI_useitem.asm)
//               - 0x6FAE9D60..0x6FAEB410 (from UNIT_Object.asm, 35 functions)
//               - 0x6FAEB410..0x6FAEB9D0 (from head of UI_automap.asm, 5 functions)
// __FILE__ VA : 0x6FB85DC8 ("..\\Source\\D2Client\\UNIT\\Object.cpp")
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <string.h>

#include <D2BasicTypes.h>
#include <D2Constants.h>
#include <D2Seed.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <D2StatList.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2Environment.h>
#include <D2QuestRecord.h>
#include <Units/Units.h>
#include <Units/Object.h>
#include <Units/Player.h>
#include <Fog.h>
#include <Storm.h>

// ============================================================================
// Struct Definitions (Client Object & Shrine Dispatch)
// ============================================================================

#pragma pack(push, 1)

typedef uint8_t (__fastcall *D2ObjectClientFunc_t)(D2UnitStrc* pObject);
typedef void (__fastcall *D2ShrineInitFunc_t)(D2UnitStrc* pObject, D2ShrinesTxt* pShrineTxt);
typedef void (__fastcall *D2ShrineEffectFunc_t)(D2UnitStrc* pObject, D2UnitStrc* pPlayer, D2ShrinesTxt* pShrineTxt);

// 1.12a: 0x6FB83240 (20 bytes per entry, 23 entries)
struct D2ShrineClientTableStrc
{
    D2ShrineInitFunc_t pfnInit;         // +0x00
    D2ShrineEffectFunc_t pfnEffect;     // +0x04
    int32_t nOverlay0;                  // +0x08
    int32_t nOverlay1;                  // +0x0C
    int32_t nSoundId;                   // +0x10
};

// Mode change descriptor passed to D2Client_ChangeObjectMode (0x6FAEB0D0)
struct D2ObjectModeChangeStrc
{
    int32_t bTargetable;                // +0x00
    int32_t nTargetMode;                // +0x04
};

// Client missile creation parameters passed to 0x6FAD4B20
struct D2ClientMissileParamStrc
{
    uint32_t dwFlags;                   // +0x00 (e.g. 0x520 or 0x03)
    D2UnitStrc* pOwner;                 // +0x04
    D2UnitStrc* pOrigin;                // +0x08
    int32_t nSubMissile0;               // +0x0C
    int32_t nMissileId;                 // +0x10 (e.g. 0x30, 0x2D, 0x3E)
    int32_t nX;                         // +0x14
    int32_t nY;                         // +0x18
    int32_t nTargetX;                   // +0x1C
    int32_t nTargetY;                   // +0x20
    int32_t nLevel;                     // +0x24
    int32_t unk0x28[13];                // +0x28..0x5B
};

#pragma pack(pop)

// ============================================================================
// External Globals (1.12a VAs)
// ============================================================================

// 1.12a: 0x6FBCA960 - Server-side unit hash tables (6 * 128 D2UnitStrc*)
extern D2UnitStrc* gapServerSideUnitHashTables[6][128];

// 1.12a: 0x6FBC9D60 - Client-side unit hash tables (6 * 128 D2UnitStrc*)
extern D2UnitStrc* gapClientSideUnitHashTables[6][128];

// 1.12a: 0x6FBCC01C - Current client Act pointer
extern D2ActStrc* gpClientAct;

// 1.12a: 0x6FBCC3D0 - Local player unit pointer
extern D2UnitStrc* gpPlayerUnit;

// 1.12a: 0x6FBCB643 - Local player quest flags bitbuffer
extern D2BitBufferStrc* gpLocalQuestFlags;

// 1.12a: 0x6FBCD515 - Tower Tome sound preloaded flag
extern uint8_t gbTowerTomeSoundPreloaded;

// 1.12a: 0x6FBCD516 - Mephisto/Act3 sound preloaded flag
extern uint8_t gbAct3ObjectSoundPreloaded;

// 1.12a: 0x6FBCD517 - Environment sound position update enabled flag
extern uint8_t gbEnvSoundPositionEnabled;

// 1.12a: 0x6FBA3C50 - Shrine count (23)
extern int32_t gnShrineClientTableCount;

// 1.12a: 0x6FBA3C58 - Object client function dispatch table (19 entries)
extern D2ObjectClientFunc_t gapfnObjectClientFunctions[19];

// 1.12a: 0x6FB83240 - Shrine client table (23 entries)
extern const D2ShrineClientTableStrc gShrineClientTable[23];

// ============================================================================
// External Helper Declarations
// ============================================================================

// 1.12a: 0x6FAB10C0
extern void __fastcall D2Client_ToggleUnitFlag_6FAB10C0(D2UnitStrc* pUnit, uint32_t dwFlag, BOOL bSet);

// 1.12a: 0x6FAB1590
extern int32_t __fastcall D2Client_GetUnitX_6FAB1590(D2UnitStrc* pUnit);

// 1.12a: 0x6FAB15C0
extern int32_t __fastcall D2Client_GetUnitY_6FAB15C0(D2UnitStrc* pUnit);

// 1.12a: 0x6FAB2E60 (custom: ecx = pSeed, edi = nMin, eax = nMax)
extern int32_t __fastcall D2Client_RollRandomRange_6FAB2E60(D2SeedStrc* pSeed, int32_t nMin, int32_t nMax);

// 1.12a: 0x6FAC5E60
extern void __stdcall D2Client_ShakeScreen_6FAC5E60(int32_t nType, int32_t nDuration);

// 1.12a: 0x6FAD0620
extern void __fastcall D2Client_RemoveUnitFromHashTable_6FAD0620(D2UnitStrc** ppTable, int32_t nBucket, int32_t nUnitId, int32_t nUnitType);

// 1.12a: 0x6FAD4B20
extern D2UnitStrc* __fastcall D2Client_CreateClientMissile_6FAD4B20(D2ClientMissileParamStrc* pParam);

// 1.12a: 0x6FAE1FA0 (custom: ebx = nSoundId, stdcall 4 args)
extern void __stdcall D2Client_PlayUnitSound_6FAE1FA0(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nArg1, int32_t nArg2, int32_t nArg3);

// 1.12a: 0x6FAE8170
extern void __stdcall D2Client_PlayObjectOperateSound_6FAE8170(D2UnitStrc* pObject);

// 1.12a: 0x6FAF0C30 (custom: edi = dwObjectUnitId)
extern int32_t __fastcall D2Client_CheckPlayerCanOperateObject_6FAF0C30(int32_t dwObjectUnitId);

// 1.12a: 0x6FAF1450
extern int32_t __fastcall D2Client_DispatchUnitModeChange_6FAF1450(int32_t nMode, D2UnitStrc* pUnit, void* pModeArgs, int32_t bFlag);

// 1.12a: 0x6FB0E500 (custom: esi = pLightMap)
extern void __fastcall D2Client_FreeLightMap_6FB0E500(void* pLightMap);

// 1.12a: 0x6FB0EAB0 (custom: ecx = pLightMap, eax = nRadius)
extern void __fastcall D2Client_SetLightMapRadius_6FB0EAB0(void* pLightMap, int32_t nRadius);

// 1.12a: 0x6FB0EAF0 (custom: ebx = nType)
extern void* __stdcall D2Client_AllocLightMap_6FB0EAF0(int32_t nRadius, int32_t nIntensity, int32_t nRed, int32_t nGreen, int32_t nBlue);

// 1.12a: 0x6FB40ED0 (custom: eax = pUnit)
extern void __fastcall D2Client_FreeUnitOverlays_6FB40ED0(D2UnitStrc* pUnit);

// 1.12a: 0x6FB41600 (custom: edi = pUnit, edx = nOverlayId)
extern void __fastcall D2Client_RemoveUnitOverlay_6FB41600(D2UnitStrc* pUnit, int32_t nOverlayId);

// 1.12a: 0x6FB41F40 (custom: ebx = nSoundId, stdcall 1 arg)
extern void __stdcall D2Client_PreloadSound_6FB41F40(int32_t nSoundId, int32_t bFlag);

// 1.12a: 0x6FB42000 (custom: eax = pUnit, stdcall 1 arg)
extern void __stdcall D2Client_UpdateUnitAnimGraphics_6FB42000(D2UnitStrc* pUnit, int32_t nArg);

// 1.12a: 0x6FB430E0 (custom: eax = nOverlayId, stdcall 7 args)
extern void __stdcall D2Client_SetUnitOverlay_6FB430E0(int32_t nOverlayId, D2UnitStrc* pUnit, int32_t nArg1, int32_t nArg2, int32_t nArg3, int32_t nArg4, int32_t nArg5, int32_t nArg6);

// 1.12a: 0x6FB70190
extern uint8_t __stdcall D2Client_IsSoundPaused_6FB70190();

// 1.12a: 0x6FB71700
extern void __stdcall D2Client_UpdateEnvironmentSoundCoords_6FB71700(int32_t nX, int32_t nY);

// ============================================================================
// Forward Declarations
// ============================================================================

void __fastcall D2Client_ApplyShrineInitialEffect(D2UnitStrc* pObject);
void __fastcall D2Client_UpdateObjectLightRadius(D2UnitStrc* pObject, uint16_t wLightRadius, int32_t nRed, int32_t nGreen, int32_t nBlue);
void __fastcall D2Client_FinishDoorModeTransition(D2UnitStrc* pObject);
BOOL __fastcall D2Client_UpdateObjectPeriodOfDay(D2UnitStrc* pObject, BOOL bSkipAnimGraphicsUpdate);

// ============================================================================
// Reconstructed Functions
// ============================================================================

// 1.12a: 0x6FAE9D40
// Calling convention: custom (__fastcall-like: ecx = pObject, eax = szOwner)
void __fastcall D2Client_SetObjectOwnerName(D2UnitStrc* pObject, const char* szOwner)
{
    SStrNCpy(pObject->pObjectData->szOwner, szOwner, 16);
}

// 1.12a: 0x6FAE9D50
// Calling convention: custom (eax = pObject)
char* __fastcall D2Client_GetObjectOwnerName(D2UnitStrc* pObject)
{
    return pObject->pObjectData->szOwner;
}

// 1.12a: 0x6FAE9D60
// Calling convention: custom (esi = pObject)
uint8_t __fastcall D2Client_ExecuteObjectClientFn(D2UnitStrc* pObject)
{
    D2ObjectsTxt* pObjectsTxt = DATATBLS_GetObjectsTxtRecord(pObject->dwClassId);
    uint8_t nClientFn = pObjectsTxt->nClientFn;
    if (nClientFn >= 19)
    {
        FOG_DisplayHalt("nClientFn < NUM_OBJECT_CLIENT_FNS", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x546);
        exit(-1);
    }

    if (nClientFn != 0)
    {
        return gapfnObjectClientFunctions[nClientFn](pObject);
    }
    return 1;
}

// 1.12a: 0x6FAE9DB0
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn01_NoOp(D2UnitStrc* pObject)
{
    return 1;
}

// 1.12a: 0x6FAE9DC0
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn11_NoOp(D2UnitStrc* pObject)
{
    return 1;
}

// 1.12a: 0x6FAE9DD0
// Calling convention: custom (edi = pObject)
void __fastcall D2Client_ApplyShrineInitialEffect(D2UnitStrc* pObject)
{
    D2ShrinesTxt* pShrineTxt = UNITS_GetShrineTxtRecordFromObject(pObject);
    if (!pShrineTxt)
    {
        FOG_DisplayHalt("ptShrineData", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x37A);
        exit(-1);
    }

    int32_t nShrineCode = pShrineTxt->nCode;
    if (nShrineCode >= gnShrineClientTableCount)
    {
        FOG_DisplayHalt("nShrineCode < gnShrineClientTableCount", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x37B);
        exit(-1);
    }

    D2ShrineInitFunc_t pfnInit = gShrineClientTable[nShrineCode].pfnInit;
    if (pfnInit)
    {
        pfnInit(pObject, pShrineTxt);
    }
}

// 1.12a: 0x6FAE9E30
// Calling convention: __fastcall (ecx = pObject, edx = pPlayer, [esp+4] = pShrineTxt)
void __fastcall D2Client_ShrineFn16_Enirhs(D2UnitStrc* pObject, D2UnitStrc* pPlayer, D2ShrinesTxt* pShrineTxt)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    if (pPlayerData)
    {
        strncpy(pPlayerData->szName, _strrev(pPlayerData->szName), 16);
    }
}

// 1.12a: 0x6FAE9E60
// Calling convention: custom (edi = pPacket)
void __fastcall D2Client_UpdatePortalFlagsFromPacket(const uint8_t* pPacket)
{
    if (!pPacket)
    {
        FOG_DisplayHalt("pPacket", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x556);
        exit(-1);
    }

    uint32_t dwObjectId = *(const uint32_t*)(pPacket + 3);
    D2UnitStrc* pObject = gapServerSideUnitHashTables[UNIT_OBJECT][dwObjectId & 0x7F];
    while (pObject)
    {
        if (pObject->dwUnitId == dwObjectId)
        {
            break;
        }
        pObject = pObject->pListNext;
    }

    if (!pObject)
    {
        return;
    }

    if (pObject->dwUnitType != UNIT_OBJECT)
    {
        FOG_DisplayHalt("pObject->eType == UNIT_OBJECT", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x5A);
        exit(-1);
    }

    if (pObject->dwClassId != OBJECT_TOWN_PORTAL && pObject->dwClassId != OBJECT_PERMANENT_TOWN_PORTAL)
    {
        FOG_DisplayHalt("bValidPortalClass", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x560);
        exit(-1);
    }

    uint8_t nDestLevel = pPacket[2];
    pObject->pObjectData->nShrineNo = nDestLevel;

    uint8_t nPortalFlags = UNITS_GetObjectPortalFlags(pObject);
    uint8_t nPacketFlags = pPacket[1];
    if (nPacketFlags & 2)
    {
        nPortalFlags |= 2;
    }
    if (nPacketFlags & 1)
    {
        nPortalFlags |= 1;
    }
    UNITS_SetObjectPortalFlags(pObject, nPortalFlags);
}

// 1.12a: 0x6FAE9F30
// Calling convention: custom (eax = pObject)
void __fastcall D2Client_InitShrineObject(D2UnitStrc* pObject)
{
    uint8_t nShrineId = pObject->pObjectData->nShrineNo;
    D2ShrinesTxt* pShrineTxt = DATATBLS_GetShrinesTxtRecord(nShrineId);
    UNITS_SetShrineTxtRecordInObjectData(pObject, pShrineTxt);
    D2Client_ApplyShrineInitialEffect(pObject);
}

// 1.12a: 0x6FAE9F50
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn02_PeriodicTorchAnim(D2UnitStrc* pObject)
{
    if (!pObject->dwFlagsEx)
    {
        uint64_t ullSeed = (uint64_t)pObject->pSeed.nLowSeed * 0x6AC690C5ULL + pObject->pSeed.nHighSeed;
        pObject->pSeed.nLowSeed = (uint32_t)ullSeed;
        pObject->pSeed.nHighSeed = (uint32_t)(ullSeed >> 32);
        pObject->dwClientInteracted = (uint32_t)ullSeed & 0x3F;
        pObject->dwFlagsEx = 1;
    }

    if ((pObject->dwClientInteracted & 0x3F) == 0 && pObject->dwAnimMode != OBJMODE_OPERATING)
    {
        pObject->dwAnimMode = OBJMODE_OPERATING;
        UNITS_SetAnimStartFrame(pObject);
    }

    pObject->dwClientInteracted++;
    if (pObject->dwAnimMode == OBJMODE_OPENED)
    {
        uint64_t ullSeed = (uint64_t)pObject->pSeed.nLowSeed * 0x6AC690C5ULL + pObject->pSeed.nHighSeed;
        pObject->pSeed.nLowSeed = (uint32_t)ullSeed;
        pObject->pSeed.nHighSeed = (uint32_t)(ullSeed >> 32);
        pObject->dwClientInteracted += ((uint32_t)ullSeed & 0x3F);
        pObject->dwAnimMode = OBJMODE_NEUTRAL;
        UNITS_SetAnimStartFrame(pObject);
    }

    return 1;
}

// 1.12a: 0x6FAEA000
// Calling convention: custom (edi = pObject)
void __fastcall D2Client_FreeObjectData(D2UnitStrc* pObject)
{
    D2Client_FreeUnitOverlays_6FB40ED0(pObject);
    if (gpClientAct)
    {
        UNITS_FreeCollisionPath(pObject);
        UNITROOM_RemoveUnitFromRoom(pObject);
        if (pObject->pLight)
        {
            D2Client_FreeLightMap_6FB0E500(pObject->pLight);
            pObject->pLight = nullptr;
        }
    }
    FOG_FreeClientMemory(pObject->pObjectData, "..\\Source\\D2Client\\UNIT\\Object.cpp", 0xFE, 0);
}

// 1.12a: 0x6FAEA050
// Calling convention: custom (__fastcall-like: ecx = pObject, ax = wLightRadius, stack: nRed, nGreen, nBlue)
void __fastcall D2Client_UpdateObjectLightRadius(D2UnitStrc* pObject, uint16_t wLightRadius, int32_t nRed, int32_t nGreen, int32_t nBlue)
{
    void* pLight = pObject->pLight;
    if (!wLightRadius)
    {
        if (pLight)
        {
            D2Client_FreeLightMap_6FB0E500(pLight);
            pObject->pLight = nullptr;
        }
        return;
    }

    if (!pLight)
    {
        pLight = D2Client_AllocLightMap_6FB0EAF0(wLightRadius >> 1, 0xFF, nRed, nGreen, nBlue);
        pObject->pLight = pLight;
        if (!pLight)
        {
            return;
        }
    }

    D2Client_SetLightMapRadius_6FB0EAB0(pLight, wLightRadius >> 1);
}

// 1.12a: 0x6FAEA0C0
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn18_AmbientRandomSound(D2UnitStrc* pObject)
{
    if (GetTickCount() > pObject->dwClientInteracted)
    {
        uint64_t ullSeed1 = (uint64_t)pObject->pSeed.nLowSeed * 0x6AC690C5ULL + pObject->pSeed.nHighSeed;
        pObject->pSeed.nLowSeed = (uint32_t)ullSeed1;
        pObject->pSeed.nHighSeed = (uint32_t)(ullSeed1 >> 32);

        if (((uint32_t)ullSeed1 % 100) < 10)
        {
            D2Client_PlayUnitSound_6FAE1FA0(0x9C9, pObject, 0, 0, 0);
        }

        uint64_t ullSeed2 = (uint64_t)pObject->pSeed.nLowSeed * 0x6AC690C5ULL + pObject->pSeed.nHighSeed;
        pObject->pSeed.nLowSeed = (uint32_t)ullSeed2;
        pObject->pSeed.nHighSeed = (uint32_t)(ullSeed2 >> 32);

        uint32_t dwDelaySec = (uint32_t)ullSeed2 % 60;
        pObject->dwClientInteracted = GetTickCount() + dwDelaySec * 1000;
    }
    return 1;
}

// 1.12a: 0x6FAEA150
// Calling convention: custom (eax = pObject, [esp+4] = pPlayer)
void __stdcall D2Client_PlayShrineSound(D2UnitStrc* pObject, D2UnitStrc* pPlayer)
{
    D2ShrinesTxt* pShrineTxt = UNITS_GetShrineTxtRecordFromObject(pObject);
    if (!pShrineTxt)
    {
        FOG_DisplayHalt("ptShrineData", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x34D);
        exit(-1);
    }

    int32_t nShrineCode = pShrineTxt->nCode;
    if (nShrineCode >= gnShrineClientTableCount)
    {
        FOG_DisplayHalt("nShrineCode < gnShrineClientTableCount", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x34E);
        exit(-1);
    }

    int32_t nSoundId = gShrineClientTable[nShrineCode].nSoundId;
    if (nSoundId)
    {
        D2Client_PlayUnitSound_6FAE1FA0(nSoundId, pPlayer, 0, 0, 0);
    }
}

// 1.12a: 0x6FAEA1C0
// Calling convention: custom (edi = pObject, [esp+4] = pTargetUnitIdPtr)
void __stdcall D2Client_InteractWithShrine(D2UnitStrc* pObject, const uint32_t* pPlayerUnitId)
{
    D2ShrinesTxt* pShrineTxt = nullptr;
    int32_t nShrineCode = 0;

    if (UNITS_IsShrine(pObject))
    {
        pShrineTxt = UNITS_GetShrineTxtRecordFromObject(pObject);
        nShrineCode = pShrineTxt->nCode;
    }
    else
    {
        int32_t nClassId = pObject ? pObject->dwClassId : -1;
        D2ObjectsTxt* pObjectsTxt = DATATBLS_GetObjectsTxtRecord(nClassId);
        nShrineCode = pObjectsTxt->nShrineFunction;
    }

    uint32_t dwPlayerId = *pPlayerUnitId;
    D2UnitStrc* pPlayer = gapServerSideUnitHashTables[UNIT_PLAYER][dwPlayerId & 0x7F];
    while (pPlayer)
    {
        if (pPlayer->dwUnitId == dwPlayerId)
        {
            break;
        }
        pPlayer = pPlayer->pListNext;
    }

    if (!pPlayer)
    {
        return;
    }

    if (pPlayer->dwUnitType != UNIT_PLAYER)
    {
        FOG_DisplayHalt("pPlayer->eType == UNIT_PLAYER", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x5A);
        exit(-1);
    }

    if (nShrineCode > 0 && nShrineCode < gnShrineClientTableCount)
    {
        D2ShrineEffectFunc_t pfnEffect = gShrineClientTable[nShrineCode].pfnEffect;
        if (pfnEffect)
        {
            pfnEffect(pObject, pPlayer, pShrineTxt);
        }
    }

    D2Client_PlayShrineSound(pObject, pPlayer);
}

// 1.12a: 0x6FAEA280
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn03_PeriodicAnimWithSound(D2UnitStrc* pObject)
{
    if (!pObject->dwFlagsEx)
    {
        uint64_t ullSeed = (uint64_t)pObject->pSeed.nLowSeed * 0x6AC690C5ULL + pObject->pSeed.nHighSeed;
        pObject->pSeed.nLowSeed = (uint32_t)ullSeed;
        pObject->pSeed.nHighSeed = (uint32_t)(ullSeed >> 32);
        pObject->dwClientInteracted = (uint32_t)ullSeed & 0x3F;
        pObject->dwFlagsEx = 1;
    }

    if ((pObject->dwClientInteracted & 0x3F) == 0 && pObject->dwAnimMode != OBJMODE_OPERATING)
    {
        pObject->dwAnimMode = OBJMODE_OPERATING;
        UNITS_SetAnimStartFrame(pObject);
    }

    pObject->dwClientInteracted++;
    if (pObject->dwAnimMode == OBJMODE_OPENED)
    {
        uint64_t ullSeed = (uint64_t)pObject->pSeed.nLowSeed * 0x6AC690C5ULL + pObject->pSeed.nHighSeed;
        pObject->pSeed.nLowSeed = (uint32_t)ullSeed;
        pObject->pSeed.nHighSeed = (uint32_t)(ullSeed >> 32);
        pObject->dwClientInteracted += ((uint32_t)ullSeed & 0x3F);
        pObject->dwAnimMode = OBJMODE_NEUTRAL;
        UNITS_SetAnimStartFrame(pObject);
    }

    D2Client_PlayObjectOperateSound_6FAE8170(pObject);
    return 1;
}

// 1.12a: 0x6FAEA330
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn16_AdvanceOperatingToOpened(D2UnitStrc* pObject)
{
    if (pObject->dwAnimMode == OBJMODE_OPERATING)
    {
        D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
        int32_t nTargetFrame = (int32_t)pObjectsTxt->nStart[OBJMODE_OPERATING] << 8;
        if (pObject->nActionFrame == nTargetFrame)
        {
            pObject->dwAnimMode = OBJMODE_OPENED;
            pObject->nActionFrame = 0;
            D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
            UNITS_SetAnimStartFrame(pObject);
        }
    }
    return 1;
}

// 1.12a: 0x6FAEA370
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn08_PreloadTowerTomeSound(D2UnitStrc* pObject)
{
    if (pObject && pObject->dwAnimMode != OBJMODE_NEUTRAL && !gbTowerTomeSoundPreloaded)
    {
        gbTowerTomeSoundPreloaded = 1;
        D2Client_PreloadSound_6FB41F40(0xD3, 1);
    }
    return 1;
}

// 1.12a: 0x6FAEA3A0
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn12_PreloadAct3ObjectSounds(D2UnitStrc* pObject)
{
    if (pObject && pObject->dwAnimMode != OBJMODE_NEUTRAL && !gbAct3ObjectSoundPreloaded)
    {
        gbAct3ObjectSoundPreloaded = 1;
        D2Client_PreloadSound_6FB41F40(0x219, 0);
        D2Client_PreloadSound_6FB41F40(0x21A, 0);
        D2Client_PreloadSound_6FB41F40(0x21B, 0);
    }
    return 1;
}

// 1.12a: 0x6FAEA3F0
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn10_TimedToggleOperatingShort(D2UnitStrc* pObject)
{
    uint32_t dwTick = GetTickCount();
    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        if (pObject->dwClientInteracted < dwTick)
        {
            pObject->dwClientInteracted = dwTick + 3000;
            D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
            pObject->nActionFrame = pObjectsTxt->nStart[OBJMODE_OPERATING];
            UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
            D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
        }
    }
    else if (pObject->dwAnimMode >= OBJMODE_OPERATING && pObject->dwClientInteracted < dwTick)
    {
        int32_t nRandomDelay = D2Client_RollRandomRange_6FAB2E60(&pObject->pSeed, 2500, 7500);
        pObject->dwClientInteracted = dwTick + nRandomDelay;
        UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
        D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
    }
    return 1;
}

// 1.12a: 0x6FAEA480
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn07_TimedToggleOperatingLong(D2UnitStrc* pObject)
{
    uint32_t dwTick = GetTickCount();
    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        if (pObject->dwClientInteracted < dwTick)
        {
            pObject->dwClientInteracted = dwTick + 10000;
            D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
            pObject->nActionFrame = pObjectsTxt->nStart[OBJMODE_OPERATING];
            UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
            D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
        }
    }
    else if (pObject->dwAnimMode >= OBJMODE_OPERATING && pObject->dwClientInteracted < dwTick)
    {
        int32_t nRandomDelay = D2Client_RollRandomRange_6FAB2E60(&pObject->pSeed, 4500, 7500);
        pObject->dwClientInteracted = dwTick + nRandomDelay;
        UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
        D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
    }
    return 1;
}

// 1.12a: 0x6FAEA510
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn06_TimedSpecial1Slow(D2UnitStrc* pObject)
{
    uint32_t dwTick = GetTickCount();
    if (pObject->dwClientInteracted < dwTick)
    {
        int32_t nRandomDelay = D2Client_RollRandomRange_6FAB2E60(&pObject->pSeed, 20000, 30000);
        pObject->dwClientInteracted = dwTick + nRandomDelay;
        D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
        pObject->nActionFrame = pObjectsTxt->nStart[OBJMODE_SPECIAL3];
        UNITS_ChangeAnimMode(pObject, OBJMODE_SPECIAL1);
        D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
    }

    uint32_t dwMode = pObject->dwAnimMode;
    if (dwMode >= OBJMODE_SPECIAL1)
    {
        D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
        if ((uint32_t)pObject->nActionFrame == pObjectsTxt->dwFrameCnt[dwMode] - 0x100)
        {
            pObject->nActionFrame = pObjectsTxt->nStart[OBJMODE_NEUTRAL];
            UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
            D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
        }
    }

    D2Client_PlayObjectOperateSound_6FAE8170(pObject);
    return 0;
}

// 1.12a: 0x6FAEA5B0
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn05_TimedSpecial1Or2(D2UnitStrc* pObject)
{
    uint32_t dwTick = GetTickCount();
    if (pObject->dwClientInteracted < dwTick)
    {
        int32_t nRandomDelay = D2Client_RollRandomRange_6FAB2E60(&pObject->pSeed, 23000, 33000);
        pObject->dwClientInteracted = dwTick + nRandomDelay;
        D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
        pObject->nActionFrame = pObjectsTxt->nStart[OBJMODE_SPECIAL3];

        uint64_t ullSeed = (uint64_t)pObject->pSeed.nLowSeed * 0x6AC690C5ULL + pObject->pSeed.nHighSeed;
        pObject->pSeed.nLowSeed = (uint32_t)ullSeed;
        pObject->pSeed.nHighSeed = (uint32_t)(ullSeed >> 32);

        int32_t nNewMode = ((uint32_t)ullSeed & 1) ? OBJMODE_SPECIAL1 : OBJMODE_SPECIAL2;
        UNITS_ChangeAnimMode(pObject, nNewMode);
        D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
    }

    uint32_t dwMode = pObject->dwAnimMode;
    if (dwMode == OBJMODE_SPECIAL1 || dwMode == OBJMODE_SPECIAL2)
    {
        D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
        if (pObject->nActionFrame >= (int32_t)(pObjectsTxt->dwFrameCnt[dwMode] - 0x100))
        {
            pObject->nActionFrame = pObjectsTxt->nStart[OBJMODE_NEUTRAL];
            UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
            D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
        }
    }

    D2Client_PlayObjectOperateSound_6FAE8170(pObject);
    return 0;
}

// 1.12a: 0x6FAEA680
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn04_TimedSpecial1Fast(D2UnitStrc* pObject)
{
    uint32_t dwTick = GetTickCount();
    if (pObject->dwClientInteracted < dwTick)
    {
        int32_t nRandomDelay = D2Client_RollRandomRange_6FAB2E60(&pObject->pSeed, 7000, 12000);
        pObject->dwClientInteracted = dwTick + nRandomDelay;
        D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
        pObject->nActionFrame = pObjectsTxt->nStart[OBJMODE_SPECIAL3];
        UNITS_ChangeAnimMode(pObject, OBJMODE_SPECIAL1);
        D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
    }

    if (pObject->dwAnimMode == OBJMODE_SPECIAL1)
    {
        D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
        if (pObject->nActionFrame >= (int32_t)(pObjectsTxt->dwFrameCnt[OBJMODE_SPECIAL1] - 0x100))
        {
            pObject->nActionFrame = pObjectsTxt->nStart[OBJMODE_NEUTRAL];
            UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
            D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
        }
    }

    D2Client_PlayObjectOperateSound_6FAE8170(pObject);
    return 0;
}

// 1.12a: 0x6FAEA720
// Calling convention: custom (esi = pObject, edi = nNewMode, [esp+4] = bTargetable)
void __stdcall D2Client_ChangeDoorMode(D2UnitStrc* pObject, int32_t nNewMode, BOOL bTargetable)
{
    D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
    if (nNewMode >= 8)
    {
        FOG_DisplayHalt("nMode < NUM_OBJ_MODES", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x210);
        exit(-1);
    }

    uint32_t dwOperatingFrames = pObjectsTxt->dwFrameCnt[OBJMODE_OPERATING];
    if (nNewMode == OBJMODE_NEUTRAL)
    {
        if (pObject->dwAnimMode == OBJMODE_OPENED && pObjectsTxt->dwFrameCnt[OBJMODE_SPECIAL1] != 0x100)
        {
            pObject->nActionFrame = pObjectsTxt->nStart[OBJMODE_NEUTRAL];
            UNITS_ChangeAnimMode(pObject, OBJMODE_SPECIAL1);
            D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
            pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
            int32_t nY = D2Client_GetUnitY_6FAB15C0(pObject);
            int32_t nX = D2Client_GetUnitX_6FAB1590(pObject);
            D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
            UNITS_BlockCollisionPath(pObject, pRoom, nX, nY);
            goto UPDATE_BLOCKS_LIGHT;
        }
    }
    else if (nNewMode == OBJMODE_OPENED)
    {
        if (pObject->dwAnimMode == OBJMODE_NEUTRAL && dwOperatingFrames != 0x100)
        {
            UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
            D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
            pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
            pObject->nActionFrame = pObject->pObjectData->pObjectTxt->nStart[OBJMODE_OPENED];
            goto UPDATE_BLOCKS_LIGHT;
        }
    }

    if (bTargetable)
    {
        pObject->dwFlags |= UNITFLAG_TARGETABLE;
    }
    else
    {
        pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    {
        uint32_t dwCurMode = pObject->dwAnimMode;
        if (nNewMode != (int32_t)dwCurMode)
        {
            if (nNewMode == OBJMODE_SPECIAL2 || nNewMode == OBJMODE_OPENED || nNewMode == OBJMODE_SPECIAL3)
            {
                if (dwCurMode != OBJMODE_SPECIAL3 && dwCurMode != OBJMODE_OPENED && dwCurMode != OBJMODE_SPECIAL2)
                {
                    UNITS_FreeCollisionPath(pObject);
                }
            }
            else if (nNewMode == OBJMODE_NEUTRAL && dwCurMode != OBJMODE_NEUTRAL)
            {
                int32_t nY = D2Client_GetUnitY_6FAB15C0(pObject);
                int32_t nX = D2Client_GetUnitX_6FAB1590(pObject);
                D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
                UNITS_BlockCollisionPath(pObject, pRoom, nX, nY);
            }

            UNITS_ChangeAnimMode(pObject, nNewMode);
            D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
        }
        pObject->nActionFrame = pObject->pObjectData->pObjectTxt->nStart[nNewMode];
    }

UPDATE_BLOCKS_LIGHT:
    if (!pObjectsTxt->nBlocksLight[pObject->dwAnimMode])
    {
        pObject->dwFlags |= UNITFLAG_DOESNOTBLOCKLIGHT;
    }
    else
    {
        pObject->dwFlags &= ~UNITFLAG_DOESNOTBLOCKLIGHT;
    }
}

// 1.12a: 0x6FAEA8C0
// Calling convention: custom (esi = pObject)
void __fastcall D2Client_FinishDoorModeTransition(D2UnitStrc* pObject)
{
    uint32_t dwMode = pObject->dwAnimMode;
    int32_t nNextActionFrame = 0;

    if (dwMode == OBJMODE_SPECIAL1)
    {
        pObject->dwAnimMode = OBJMODE_NEUTRAL;
        nNextActionFrame = pObject->pObjectData->pObjectTxt->nStart[OBJMODE_NEUTRAL];
    }
    else if (dwMode == OBJMODE_OPERATING)
    {
        UNITS_FreeCollisionPath(pObject);
        pObject->dwAnimMode = OBJMODE_OPENED;
        nNextActionFrame = pObject->pObjectData->pObjectTxt->nStart[OBJMODE_OPENED];
    }
    else
    {
        FOG_DisplayHalt("Invalid door mode transition", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x11F);
        exit(-1);
    }

    pObject->nActionFrame = nNextActionFrame;
    D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
    UNITS_SetAnimStartFrame(pObject);

    D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
    if (pObjectsTxt->nSelectable[pObject->dwAnimMode])
    {
        pObject->dwFlags |= UNITFLAG_TARGETABLE;
    }
    else
    {
        pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
    }
}

// 1.12a: 0x6FAEA960
// Calling convention: custom (esi = pObject, ebx = bSkipAnimGraphicsUpdate)
BOOL __fastcall D2Client_UpdateObjectPeriodOfDay(D2UnitStrc* pObject, BOOL bSkipAnimGraphicsUpdate)
{
    D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
    if (!pObjectsTxt->nEnvEffect)
    {
        return FALSE;
    }

    int32_t nPeriodOfDay = ENVIRONMENT_GetPeriodOfDayFromAct(gpClientAct, nullptr);
    if (nPeriodOfDay != 0)
    {
        if (nPeriodOfDay < 0 || nPeriodOfDay > 3)
        {
            FOG_DisplayHalt("Invalid period of day", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x66);
            exit(-1);
        }

        if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
        {
            pObject->dwAnimMode = OBJMODE_OPERATING;
            if (!bSkipAnimGraphicsUpdate)
            {
                D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
            }
            UNITS_SetAnimStartFrame(pObject);
            D2Client_ToggleUnitFlag_6FAB10C0(pObject, UNITFLAG_TARGETABLE, pObjectsTxt->nSelectable[pObject->dwAnimMode]);
        }

        D2Client_UpdateObjectLightRadius(
            pObject,
            pObjectsTxt->nLit[pObject->dwAnimMode],
            pObjectsTxt->nRed,
            pObjectsTxt->nGreen,
            pObjectsTxt->nBlue);
        return TRUE;
    }

    if (pObject->dwAnimMode != OBJMODE_OPENED && pObject->dwAnimMode != OBJMODE_OPERATING)
    {
        return FALSE;
    }

    pObject->dwAnimMode = OBJMODE_NEUTRAL;
    if (!bSkipAnimGraphicsUpdate)
    {
        D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
    }
    UNITS_SetAnimStartFrame(pObject);

    D2Client_UpdateObjectLightRadius(
        pObject,
        pObjectsTxt->nLit[pObject->dwAnimMode],
        pObjectsTxt->nRed,
        pObjectsTxt->nGreen,
        pObjectsTxt->nBlue);

    if (pObjectsTxt->nSelectable[pObject->dwAnimMode])
    {
        pObject->dwFlags |= UNITFLAG_TARGETABLE;
    }
    else
    {
        pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
    }
    return TRUE;
}

// 1.12a: 0x6FAEAAB0
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn14_PeriodicEnvEffectUpdate(D2UnitStrc* pObject)
{
    if (GetTickCount() > pObject->dwClientInteracted)
    {
        D2Client_UpdateObjectPeriodOfDay(pObject, FALSE);
        pObject->dwClientInteracted = GetTickCount() + 500;
    }
    return 1;
}

// 1.12a: 0x6FAEAAE0
// Calling convention: custom (eax = pObject)
void __fastcall D2Client_StepDoorAnimation(D2UnitStrc* pObject)
{
    D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
    uint32_t dwMode = pObject->dwAnimMode;
    int32_t nFrameCnt = pObjectsTxt->dwFrameCnt[dwMode];

    if (dwMode == OBJMODE_OPERATING)
    {
        int32_t nMaxFrame = nFrameCnt - 0x100;
        int32_t nCurFrame = pObject->nActionFrame;
        if (nCurFrame == nMaxFrame)
        {
            D2Client_FinishDoorModeTransition(pObject);
            return;
        }

        nCurFrame += pObject->wFrameRate;
        if (nCurFrame > nMaxFrame)
        {
            nCurFrame = nMaxFrame;
        }
        pObject->nActionFrame = nCurFrame;
        return;
    }

    if (dwMode == OBJMODE_SPECIAL1)
    {
        int32_t nCurFrame = pObject->nActionFrame;
        if (nCurFrame <= 0)
        {
            D2Client_FinishDoorModeTransition(pObject);
            return;
        }

        int32_t nDelta = pObject->wFrameRate;
        if (nDelta > nCurFrame)
        {
            pObject->nActionFrame = 0;
        }
        else
        {
            pObject->nActionFrame = nCurFrame - nDelta;
        }
        return;
    }

    FOG_DisplayHalt("Invalid door animation mode", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x155);
    exit(-1);
}

// 1.12a: 0x6FAEAB70
// Calling convention: custom (edx = pObject, ecx = nY, al = nMode, stack: pRoom, nX, nClassId)
void __stdcall D2Client_InitObjectUnit(D2UnitStrc* pObject, int32_t nY, uint8_t nMode, D2RoomStrc* pRoom, int32_t nX, int32_t nClassId)
{
    pObject->pLight = nullptr;
    pObject->dwAnimMode = nMode;

    D2ObjectDataStrc* pObjectData = (D2ObjectDataStrc*)FOG_AllocClientMemory(
        sizeof(D2ObjectDataStrc),
        "..\\Source\\D2Client\\UNIT\\Object.cpp",
        0x76,
        0);
    memset(pObjectData, 0, sizeof(D2ObjectDataStrc));
    pObject->pObjectData = pObjectData;

    D2ObjectsTxt* pObjectsTxt = DATATBLS_GetObjectsTxtRecord(nClassId);
    pObjectData->pObjectTxt = pObjectsTxt;

    if (pObjectsTxt->nSelectable[pObject->dwAnimMode])
    {
        pObject->dwFlags |= UNITFLAG_TARGETABLE;
    }
    else
    {
        pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    UNITS_InitializeStaticPath(pObject, pRoom, nX, nY);
    if (pObjectsTxt->nHasCollision[pObject->dwAnimMode])
    {
        UNITS_BlockCollisionPath(pObject, pRoom, nX, nY);
    }

    if (pObjectsTxt->nIsAttackable0)
    {
        pObject->dwFlags |= UNITFLAG_CANBEATTACKED;
    }
    else
    {
        pObject->dwFlags &= ~UNITFLAG_CANBEATTACKED;
    }

    UNITROOM_AddUnitToRoom(pObject, pRoom);
    pObject->dwFlags &= ~UNITFLAG_ISVALIDTARGET;
    UNITS_SetAnimStartFrame(pObject);

    if (!pObjectsTxt->nBlocksLight[pObject->dwAnimMode])
    {
        pObject->dwFlags |= UNITFLAG_DOESNOTBLOCKLIGHT;
    }
    else
    {
        pObject->dwFlags &= ~UNITFLAG_DOESNOTBLOCKLIGHT;
    }

    if (pObjectsTxt->nOrderFlag[pObject->dwAnimMode] == 1)
    {
        pObject->dwFlags |= UNITFLAG_FLOOR;
    }

    if (pObject->dwClassId == OBJECT_TOWN_PORTAL)
    {
        int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
        uint8_t nAct = DRLG_GetActNoFromLevelId(nLevelId);
        int32_t nTownLevelId = DUNGEON_GetTownLevelIdFromActNo(nAct);
        if (nTownLevelId > 0xFF)
        {
            FOG_DisplayHalt("nTownLevelId <= 0xFF", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x95);
            exit(-1);
        }
        pObject->pObjectData->nShrineNo = (uint8_t)nTownLevelId;
    }
    else if (pObject->dwClassId == OBJECT_PERMANENT_TOWN_PORTAL)
    {
        int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
        if (nLevelId == 0x26) // LEVEL_TRISTRAM -> LEVEL_STONYFIELD (4)
        {
            pObject->pObjectData->nShrineNo = 4;
        }
        else if (nLevelId == 4) // LEVEL_STONYFIELD -> LEVEL_TRISTRAM (0x26)
        {
            pObject->pObjectData->nShrineNo = 0x26;
        }
    }

    if (!D2Client_UpdateObjectPeriodOfDay(pObject, TRUE))
    {
        D2Client_UpdateObjectLightRadius(
            pObject,
            pObjectsTxt->nLit[pObject->dwAnimMode],
            pObjectsTxt->nRed,
            pObjectsTxt->nGreen,
            pObjectsTxt->nBlue);
    }
}

// 1.12a: 0x6FAEAD40
// Calling convention: custom (eax = pObject)
void __fastcall D2Client_StepObjectAnimation(D2UnitStrc* pObject)
{
    D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
    uint32_t dwMode = pObject->dwAnimMode;
    int32_t nFrameCnt = pObjectsTxt->dwFrameCnt[dwMode];

    if (nFrameCnt == 0x100)
    {
        return;
    }

    uint8_t bCycleAnim = pObjectsTxt->nCycleAnim[dwMode];
    if (!bCycleAnim)
    {
        if (pObjectsTxt->nIsDoor)
        {
            D2Client_StepDoorAnimation(pObject);
            return;
        }

        if (pObject->nActionFrame >= nFrameCnt - 0x100)
        {
            if (pObject->dwClassId == OBJECT_CAINPORTAL && (dwMode == OBJMODE_OPENED || dwMode == OBJMODE_SPECIAL1))
            {
                uint32_t dwNextMode = dwMode + 1;
                pObject->dwAnimMode = dwNextMode;
                pObject->nActionFrame = (int32_t)pObjectsTxt->nStart[dwNextMode] << 8;
                D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
                UNITS_SetAnimStartFrame(pObject);
                return;
            }

            if (dwMode == OBJMODE_OPERATING)
            {
                pObject->dwAnimMode = OBJMODE_OPENED;
                if (pObjectsTxt->nOrderFlag[OBJMODE_OPENED] == 1)
                {
                    pObject->dwFlags |= UNITFLAG_FLOOR;
                }

                uint32_t dwShakeParam = pObjectsTxt->dwParm[7]; // +0x194
                if (dwShakeParam != 0)
                {
                    D2Client_ShakeScreen_6FAC5E60((dwShakeParam == 0xFF) ? 0x153 : 0x97, 4);
                }

                pObject->nActionFrame = (int32_t)pObjectsTxt->nStart[OBJMODE_OPENED] << 8;
                D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
                UNITS_SetAnimStartFrame(pObject);

                if (pObjectsTxt->nOverlay)
                {
                    switch (pObject->dwClassId)
                    {
                    case OBJECT_HORADRIC_CUBE_CHEST:
                    case OBJECT_HORADRIC_SCROLL_CHEST:
                    case OBJECT_STAFF_OF_KINGS_CHEST:
                    case OBJECT_SPARKLY_CHEST:
                    case OBJECT_KHALIM_CHEST1:
                    case OBJECT_KHALIM_CHEST2:
                    case OBJECT_KHALIM_CHEST3:
                        D2Client_RemoveUnitOverlay_6FB41600(pObject, 0x47);
                        break;
                    default:
                        break;
                    }
                }

                D2Client_ToggleUnitFlag_6FAB10C0(pObject, UNITFLAG_TARGETABLE, pObjectsTxt->nSelectable[pObject->dwAnimMode]);
                D2Client_UpdateObjectLightRadius(
                    pObject,
                    pObjectsTxt->nLit[OBJMODE_OPENED],
                    pObjectsTxt->nRed,
                    pObjectsTxt->nGreen,
                    pObjectsTxt->nBlue);

                if (!pObjectsTxt->nHasCollision[pObject->dwAnimMode] && pObjectsTxt->nHasCollision[OBJMODE_OPERATING])
                {
                    UNITS_FreeCollisionPath(pObject);
                }
            }
            return;
        }
    }

    if (pObject->dwClassId == OBJECT_ROGUEFOUNTAIN)
    {
        pObject->nActionFrame -= pObject->wFrameRate;
        if (pObject->nActionFrame < 0)
        {
            pObject->nActionFrame += nFrameCnt;
        }
        return;
    }

    if (pObject->dwClassId == OBJECT_CAINPORTAL && dwMode == OBJMODE_SPECIAL1)
    {
        pObject->nActionFrame -= pObject->wFrameRate;
        if (pObject->nActionFrame < 0)
        {
            pObject->dwAnimMode = OBJMODE_SPECIAL2;
            pObject->nActionFrame = (int32_t)pObjectsTxt->nStart[OBJMODE_SPECIAL2] << 8;
            D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
            UNITS_SetAnimStartFrame(pObject);
        }
        return;
    }

    pObject->nActionFrame += pObject->wFrameRate;
    if (pObject->nActionFrame >= nFrameCnt)
    {
        if (!bCycleAnim)
        {
            pObject->nActionFrame = nFrameCnt - 0x100;
        }
        else
        {
            int32_t nStartFrame = (int32_t)pObjectsTxt->nStart[dwMode] << 8;
            pObject->nActionFrame = nStartFrame - nFrameCnt + pObject->nActionFrame;
        }
    }
}

// 1.12a: 0x6FAEAF70
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn15_ToggleOverlay48(D2UnitStrc* pObject)
{
    if (GetTickCount() > pObject->dwClientInteracted)
    {
        if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
        {
            D2Client_SetUnitOverlay_6FB430E0(0x48, pObject, 3, 0, 0, 0, 0, 0);
        }
        else
        {
            D2Client_RemoveUnitOverlay_6FB41600(pObject, 0x48);
        }
        pObject->dwClientInteracted = GetTickCount() + 500;
    }
    return 1;
}

// 1.12a: 0x6FAEAFD0
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn09_DespawnWhenFarFromPlayer(D2UnitStrc* pObject)
{
    uint32_t dwTick = GetTickCount();
    if (!pObject->dwClientInteracted)
    {
        pObject->dwClientInteracted = dwTick;
        return 1;
    }

    if (dwTick - pObject->dwClientInteracted > 1000)
    {
        if (UNITS_GetDistanceToOtherUnit(pObject, gpPlayerUnit) > 25)
        {
            D2Client_RemoveUnitFromHashTable_6FAD0620(
                gapClientSideUnitHashTables[UNIT_OBJECT],
                pObject->dwUnitId & 0x7F,
                pObject->dwUnitId,
                UNIT_OBJECT);
            return 0;
        }
        pObject->dwClientInteracted = dwTick;
    }
    return 1;
}

// 1.12a: 0x6FAEB030
// Calling convention: __fastcall (ecx = pObject, edx = pShrineTxt)
void __fastcall D2Client_ShrineInit_SetShrineOverlays(D2UnitStrc* pObject, D2ShrinesTxt* pShrineTxt)
{
    int32_t nShrineCode = pShrineTxt->nCode;
    if (nShrineCode >= gnShrineClientTableCount)
    {
        FOG_DisplayHalt("nShrineCode < gnShrineClientTableCount", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x337);
        exit(-1);
    }

    const D2ShrineClientTableStrc* pShrineRecord = &gShrineClientTable[nShrineCode];
    if (pShrineRecord->nOverlay0 != -1)
    {
        D2Client_RemoveUnitOverlay_6FB41600(pObject, pShrineRecord->nOverlay0);
        if (!pObject || pObject->dwAnimMode == OBJMODE_NEUTRAL)
        {
            D2Client_SetUnitOverlay_6FB430E0(pShrineRecord->nOverlay0, pObject, 3, 0, 0, 0, 0, 0);
        }
    }

    if (pShrineRecord->nOverlay1 != -1)
    {
        D2Client_RemoveUnitOverlay_6FB41600(pObject, pShrineRecord->nOverlay1);
        if (!pObject || pObject->dwAnimMode == OBJMODE_NEUTRAL)
        {
            D2Client_SetUnitOverlay_6FB430E0(pShrineRecord->nOverlay1, pObject, 3, 0, 0, 0, 0, 0);
        }
    }
}

// 1.12a: 0x6FAEB0D0
// Calling convention: custom (ecx = pObject, eax = pModeChange)
void __fastcall D2Client_ChangeObjectMode(D2UnitStrc* pObject, const D2ObjectModeChangeStrc* pModeChange)
{
    D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
    if (pObjectsTxt->nIsDoor)
    {
        D2Client_ChangeDoorMode(pObject, pModeChange->nTargetMode, pModeChange->bTargetable);
    }
    else
    {
        UNITS_ChangeAnimMode(pObject, pModeChange->nTargetMode);
        D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);

        if (pObjectsTxt->nOverlay)
        {
            switch (pObject->dwClassId)
            {
            case OBJECT_HORADRIC_CUBE_CHEST:
            case OBJECT_HORADRIC_SCROLL_CHEST:
            case OBJECT_STAFF_OF_KINGS_CHEST:
            case OBJECT_SPARKLY_CHEST:
            case OBJECT_KHALIM_CHEST1:
            case OBJECT_KHALIM_CHEST2:
            case OBJECT_KHALIM_CHEST3:
                if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
                {
                    D2Client_SetUnitOverlay_6FB430E0(0x47, pObject, 3, 0, 0, 0, 0, 0);
                }
                break;
            default:
                break;
            }
        }

        if (pObjectsTxt->nSelectable[pObject->dwAnimMode])
        {
            pObject->dwFlags |= UNITFLAG_TARGETABLE;
        }
        else
        {
            pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
        }

        if (!pObjectsTxt->nHasCollision[pObject->dwAnimMode])
        {
            UNITS_FreeCollisionPath(pObject);
        }

        if (pObjectsTxt->nOrderFlag[pObject->dwAnimMode] == 1)
        {
            pObject->dwFlags |= UNITFLAG_FLOOR;
        }

        D2Client_UpdateObjectLightRadius(
            pObject,
            pObjectsTxt->nLit[pModeChange->nTargetMode],
            pObjectsTxt->nRed,
            pObjectsTxt->nGreen,
            pObjectsTxt->nBlue);
    }

    D2Client_PlayObjectOperateSound_6FAE8170(pObject);
    UNITS_SetAnimStartFrame(pObject);

    if (!pObjectsTxt->nBlocksLight[pObject->dwAnimMode])
    {
        pObject->dwFlags |= UNITFLAG_DOESNOTBLOCKLIGHT;
    }
    else
    {
        pObject->dwFlags &= ~UNITFLAG_DOESNOTBLOCKLIGHT;
    }
}

// 1.12a: 0x6FAEB220
// Calling convention: custom (eax = pObject)
void __fastcall D2Client_UpdateObjectUnitPerTick(D2UnitStrc* pObject)
{
    D2Client_StepObjectAnimation(pObject);
    D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
    if (pObjectsTxt->nClientFn <= 3 || D2Client_ExecuteObjectClientFn(pObject))
    {
        D2Client_PlayObjectOperateSound_6FAE8170(pObject);
    }
}

// 1.12a: 0x6FAEB250
// Calling convention: __fastcall (ecx = pObject)
void __fastcall D2Client_ApplyInitialObjectOverlays(D2UnitStrc* pObject)
{
    D2ObjectsTxt* pObjectsTxt = pObject->pObjectData->pObjectTxt;
    if (!pObjectsTxt->nOverlay)
    {
        return;
    }

    switch (pObject->dwClassId)
    {
    case OBJECT_TAINTEDSUNALTAR: // 149 (0x95)
        if (pObject->dwAnimMode != OBJMODE_OPERATING)
        {
            return;
        }
        // Fallthrough to overlay 0xB6
    case OBJECT_ORIFICE: // 152 (0x98)
        D2Client_SetUnitOverlay_6FB430E0(0xB6, pObject, 3, 0, 0, 0, 0, 0);
        break;

    case OBJECT_HORADRIC_CUBE_CHEST:   // 354 (0x162)
    case OBJECT_HORADRIC_SCROLL_CHEST: // 355 (0x163)
    case OBJECT_STAFF_OF_KINGS_CHEST:  // 356 (0x164)
    case OBJECT_SPARKLY_CHEST:         // 397 (0x18D)
    case OBJECT_KHALIM_CHEST1:         // 405 (0x195)
    case OBJECT_KHALIM_CHEST2:         // 406 (0x196)
    case OBJECT_KHALIM_CHEST3:         // 407 (0x197)
    case 455:                          // 455 (0x1C7)
        if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
        {
            D2Client_SetUnitOverlay_6FB430E0(0x47, pObject, 3, 0, 0, 0, 0, 0);
        }
        break;

    default:
        break;
    }
}

// 1.12a: 0x6FAEB3C0
// Calling convention: custom (__fastcall-like: ecx = nEventType, edx = pObject, eax = pArg)
void __fastcall D2Client_DispatchObjectModeOrShrine(int32_t nEventType, D2UnitStrc* pObject, const void* pArg)
{
    if (nEventType == 3)
    {
        D2Client_ChangeObjectMode(pObject, (const D2ObjectModeChangeStrc*)pArg);
        if (UNITS_IsShrine(pObject))
        {
            D2Client_ApplyShrineInitialEffect(pObject);
        }
    }
    else if (nEventType == 0x15)
    {
        D2Client_InteractWithShrine(pObject, (const uint32_t*)pArg);
    }
    else
    {
        FOG_DisplayHalt("Invalid object dispatch event", "..\\Source\\D2Client\\UNIT\\Object.cpp", 0x39C);
        exit(-1);
    }
}

// 1.12a: 0x6FAEB410
// Calling convention: __fastcall (ecx = pObject, edx = pPlayer, [esp+4] = pShrineTxt)
void __fastcall D2Client_ShrineFn22_Poison(D2UnitStrc* pObject, D2UnitStrc* pPlayer, D2ShrinesTxt* pShrineTxt)
{
    static const int32_t gnOffsetsX[6] = { -6, -6,  0,  0,  6,  6 };
    static const int32_t gnOffsetsY[6] = {  6, -6,  6, -6,  6, -6 };

    for (int32_t i = 0; i < 6; ++i)
    {
        D2ClientMissileParamStrc tParam;
        memset(&tParam, 0, sizeof(tParam));
        tParam.dwFlags = 0x520;
        tParam.pOwner = pPlayer;
        tParam.pOrigin = pObject;
        tParam.nMissileId = 0x30; // Poison Nova / Poison Shrine cloud

        int32_t nX = D2Client_GetUnitX_6FAB1590(pObject);
        int32_t nY = D2Client_GetUnitY_6FAB15C0(pObject);
        tParam.nX = nX;
        tParam.nY = nY;
        tParam.nTargetX = nX + gnOffsetsX[i];
        tParam.nTargetY = nY + gnOffsetsY[i];

        int32_t nLevel = STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0) / 5;
        if (nLevel < 1)
        {
            nLevel = 1;
        }
        else if (nLevel > 8)
        {
            nLevel = 8;
        }
        tParam.nLevel = nLevel;

        D2Client_CreateClientMissile_6FAD4B20(&tParam);
    }
}

// 1.12a: 0x6FAEB560
// Calling convention: __fastcall (ecx = pObject, edx = pPlayer, [esp+4] = pShrineTxt)
void __fastcall D2Client_ShrineFn21_Explosion(D2UnitStrc* pObject, D2UnitStrc* pPlayer, D2ShrinesTxt* pShrineTxt)
{
    static const int32_t gnOffsetsX[6] = { -6, -6,  0,  0,  6,  6 };
    static const int32_t gnOffsetsY[6] = {  6, -6,  6, -6,  6, -6 };

    for (int32_t i = 0; i < 6; ++i)
    {
        D2ClientMissileParamStrc tParam;
        memset(&tParam, 0, sizeof(tParam));
        tParam.dwFlags = 0x520;
        tParam.pOwner = pPlayer;
        tParam.pOrigin = pObject;
        tParam.nMissileId = 0x2D; //Exploding Shrine fireball

        int32_t nX = D2Client_GetUnitX_6FAB1590(pObject);
        int32_t nY = D2Client_GetUnitY_6FAB15C0(pObject);
        tParam.nX = nX;
        tParam.nY = nY;
        tParam.nTargetX = nX + gnOffsetsX[i];
        tParam.nTargetY = nY + gnOffsetsY[i];

        int32_t nLevel = STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0) / 5;
        if (nLevel < 1)
        {
            nLevel = 1;
        }
        else if (nLevel > 8)
        {
            nLevel = 8;
        }
        tParam.nLevel = nLevel;

        D2Client_CreateClientMissile_6FAD4B20(&tParam);
    }
}

// 1.12a: 0x6FAEB6B0
// Calling convention: __fastcall (ecx = pObject, edx = pPlayer, [esp+4] = pShrineTxt)
void __fastcall D2Client_ShrineFn19_Storm(D2UnitStrc* pObject, D2UnitStrc* pPlayer, D2ShrinesTxt* pShrineTxt)
{
    int32_t nPosX = 5;
    int32_t nNegX = -5;
    for (int32_t nStepX = 1; nPosX < 25; ++nStepX, nPosX += 5, nNegX -= 5)
    {
        int32_t nPosY = 5;
        int32_t nNegY = -5;
        for (int32_t nStepY = 1; nPosY < 25; ++nStepY, nPosY += 5, nNegY -= 5)
        {
            D2ClientMissileParamStrc tParam;
            memset(&tParam, 0, sizeof(tParam));
            tParam.dwFlags = 3;
            tParam.pOwner = pPlayer;
            tParam.nMissileId = 0x3E;

            tParam.nX = D2Client_GetUnitX_6FAB1590(pObject);
            tParam.nY = D2Client_GetUnitY_6FAB15C0(pObject);
            tParam.nTargetX = (nStepX & 1) ? nPosX : nNegX;
            tParam.nTargetY = (nStepY & 1) ? nPosY : nNegY;

            int32_t nLevel = STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0) / 5;
            if (nLevel < 1)
            {
                nLevel = 1;
            }
            else if (nLevel > 8)
            {
                nLevel = 8;
            }
            tParam.nLevel = nLevel;

            D2Client_CreateClientMissile_6FAD4B20(&tParam);
        }
    }
}

// 1.12a: 0x6FAEB840
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn17_UpdateEnvSoundPosition(D2UnitStrc* pObject)
{
    if (gbEnvSoundPositionEnabled && !D2Client_IsSoundPaused_6FB70190())
    {
        int32_t nY = D2Client_GetUnitY_6FAB15C0(pObject);
        int32_t nX = D2Client_GetUnitX_6FAB1590(pObject);
        D2Client_UpdateEnvironmentSoundCoords_6FB71700(nX, nY);
    }
    return 1;
}

// 1.12a: 0x6FAEB8D0
// Calling convention: __fastcall (ecx = pObject)
uint8_t __fastcall D2Client_ObjClientFn13_FrozenAnyaQuestCheck(D2UnitStrc* pObject)
{
    if (pObject && pObject->dwAnimMode != OBJMODE_NEUTRAL)
    {
        return 1;
    }

    D2UnitStrc* pPlayer = gpPlayerUnit;
    if (!pPlayer)
    {
        return 1;
    }

    if (!QUESTRECORD_GetQuestState(gpLocalQuestFlags, 0x27, 0))
    {
        return 1;
    }

    if (QUESTRECORD_GetQuestState(gpLocalQuestFlags, 0x27, 4))
    {
        return 1;
    }

    if (UNITS_GetDistanceToOtherUnit(pObject, pPlayer) >= 25)
    {
        return 1;
    }

    if (pPlayer->pDynamicPath)
    {
        D2Common_10153(pPlayer->pDynamicPath);
    }

    if (D2Client_CheckPlayerCanOperateObject_6FAF0C30(pObject->dwUnitId))
    {
        int32_t aModeArgs[7] = { 0 };
        aModeArgs[0] = UNIT_OBJECT;
        aModeArgs[1] = pObject->dwUnitId;
        D2Client_DispatchUnitModeChange_6FAF1450(2, pPlayer, aModeArgs, 0);

        if (pPlayer->dwUnitType == UNIT_PLAYER && pPlayer->pPlayerData)
        {
            uint32_t* pPlayerClientFields = (uint32_t*)((uint8_t*)pPlayer->pPlayerData + 0x150);
            pPlayerClientFields[0] = 0;
            pPlayerClientFields[1] = 0;
            pPlayerClientFields[2] = 0;
            pPlayerClientFields[3] = 0;
        }
    }

    UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
    D2Client_UpdateUnitAnimGraphics_6FB42000(pObject, 0);
    return 1;
}
