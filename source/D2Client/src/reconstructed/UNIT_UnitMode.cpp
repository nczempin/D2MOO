// ============================================================================
// UNIT_UnitMode.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll
// Address ranges:
//   - 0x6FAE6360 .. 0x6FAE8B60 (asm/UNIT_UnitSnd.asm)
//   - 0x6FAF0B70 .. 0x6FAF2060 (asm/UNIT_UnitMode.asm)
// ============================================================================

#include <windows.h>
#include <stdint.h>

struct D2UnitStrc;
struct D2RoomStrc;
struct D2DynamicPathStrc;
struct D2StaticPathStrc;
struct D2SkillStrc;
struct D2StatListStrc;
struct D2MonSoundsTxt;
struct D2SoundNodeStrc;

// ============================================================================
// External Globals (1.12a D2Client.dll)
// ============================================================================

extern D2UnitStrc* gpPlayerUnit;                         // 0x6FBCC3D0
extern uint32_t gdwClientPing_6FBB32BC;                  // 0x6FBB32BC
extern uint32_t gdwClientTickCount;                      // 0x6FBB32B0
extern uint32_t gdwLastCombatSoundTick_6FBCC35C;         // 0x6FBCC35C
extern uint32_t gdwLastPlayerSpeechTick_6FBA2090;        // 0x6FBA2090
extern uint32_t gnPendingQuestSpeechSoundId_6FBA2094;    // 0x6FBA2094
extern int32_t  gnForceTileSoundType_6FBCC360;           // 0x6FBCC360

struct D2CharVoiceEntryStrc
{
    uint32_t dwDeathSound;          // +0x00
    uint32_t dwGetHitSound;         // +0x04
    uint32_t dwSpeechSounds[40];    // +0x08
};
extern D2CharVoiceEntryStrc gCharVoiceTable_6FBA1BC0[7]; // 0x6FBA1BC0 (0xA8 bytes per class)
extern int32_t gCharBarkSoundIndices_6FBA2068[10];       // 0x6FBA2068

// ============================================================================
// PART 1: UnitSnd.cpp (0x6FAE6360 .. 0x6FAE8B60)
// ============================================================================

// 1.12a: 0x6FAE6360 (custom: eax = nMonSoundId)
D2MonSoundsTxt* __fastcall D2Client_GetMonSoundsTxtRecord(int32_t nMonSoundId)
{
    if (nMonSoundId < 0 || nMonSoundId >= sgptDataTables->nMonSoundsTxtRecordCount)
    {
        return nullptr;
    }
    return &sgptDataTables->pMonSoundsTxt[nMonSoundId];
}

// 1.12a: 0x6FAE6380 (custom: esi = nTileSoundIndex -> eax = nSoundId)
int32_t __fastcall D2Client_GetLightFootstepSoundByMaterial(int32_t nTileSoundIndex)
{
    static const int32_t kLightFootstepSounds[22] = {
        0xBA, 0xCA, 0xCE, 0xD2, 0xC6, 0xBE, 0xFA, 0xE6, 0xEA, 0xDE, 0xEE,
        0xDA, 0xD6, 0xC2, 0xF2, 0xF6, 0xE2, 0xFE, 0xB6, 0xCA, 0xC6, 0xE2
    };
    if (nTileSoundIndex >= 0 && nTileSoundIndex <= 21)
    {
        return kLightFootstepSounds[nTileSoundIndex];
    }
    return 0xBA;
}

// 1.12a: 0x6FAE63D0 (custom: esi = nTileSoundIndex -> eax = nSoundId)
int32_t __fastcall D2Client_GetHeavyFootstepSoundByMaterial(int32_t nTileSoundIndex)
{
    static const int32_t kHeavyFootstepSounds[22] = {
        0x102, 0x112, 0x116, 0x11A, 0x10E, 0x106, 0x142, 0x12E, 0x132, 0x126, 0x136,
        0x122, 0x11E, 0x10A, 0x13A, 0x13E, 0x12A, 0x146, 0x0FE, 0x112, 0x10E, 0x12A
    };
    if (nTileSoundIndex >= 0 && nTileSoundIndex <= 21)
    {
        return kHeavyFootstepSounds[nTileSoundIndex];
    }
    return 0x102;
}

// 1.12a: 0x6FAE6420 (custom: eax = nQuestSpeechCase -> eax = nSpeechSlotIndex)
int32_t __fastcall D2Client_MapQuestSpeechTypeToSlotIndex(int32_t nQuestSpeechCase)
{
    if (nQuestSpeechCase < 57 || nQuestSpeechCase > 89)
    {
        return 0;
    }
    return (nQuestSpeechCase - 57) + 25;
}

// 1.12a: 0x6FAE66C0 (custom: eax = nHitClassLowNibble -> eax = nWeaponSwingSoundId)
int32_t __fastcall D2Client_GetWeaponSwingSoundByHitClass(int32_t nHitClassLowNibble)
{
    switch (nHitClassLowNibble)
    {
    case 1:  return 24;
    case 2:  return 31;
    case 3:  return 35;
    case 4:  return 39;
    case 5:  return 47;
    case 6:  return 43;
    case 7:  return 55;
    case 8:  return 51;
    case 9:  return 59;
    case 10: return 23;
    case 11: return 22;
    case 12: return 2331;
    case 13: return 4675;
    default: return 0;
    }
}

// 1.12a: 0x6FAE6720 (custom: eax = nHitClassLowNibble -> eax = nWeaponImpactSoundId)
int32_t __fastcall D2Client_GetWeaponImpactSoundByHitClass(int32_t nHitClassLowNibble)
{
    switch (nHitClassLowNibble)
    {
    case 1:  return 28;
    case 2:  case 5:  return 67;
    case 3:  case 6:  return 63;
    case 4:  case 8:  return 71;
    case 7:  return 75;
    case 10: return 79;
    case 13: return 4679;
    default: return 0;
    }
}

// 1.12a: 0x6FAE6780 (custom: eax = nHitClassHighNibble -> eax = nElementalImpactSoundId)
int32_t __fastcall D2Client_GetElementalImpactSoundByHitClass(int32_t nHitClassHighNibble)
{
    switch (nHitClassHighNibble)
    {
    case 0x10: return 109;
    case 0x20: return 83;
    case 0x30: return 87;
    case 0x40: return 91;
    case 0x70: return 99;
    case 0x80: return 102;
    case 0xA0: return 182;
    default:   return 0;
    }
}

// 1.12a: 0x6FAE67C0 (custom: eax = nPlayerClass -> eax = pCharVoiceEntry)
D2CharVoiceEntryStrc* __fastcall D2Client_GetCharVoiceTableEntry(int32_t nPlayerClass)
{
    if (nPlayerClass < 0 || nPlayerClass >= 7)
    {
        return nullptr;
    }
    return &gCharVoiceTable_6FBA1BC0[nPlayerClass];
}

// 1.12a: 0x6FAE67E0 (__fastcall)
void __fastcall D2Client_SetUnitSoundPlayedFlag(D2UnitStrc* pUnit)
{
    if (pUnit)
    {
        pUnit->bSoundPlayed = 1;
    }
}

// 1.12a: 0x6FAE67F0 (__fastcall)
void __fastcall D2Client_ClearUnitSoundPlayedFlag(D2UnitStrc* pUnit)
{
    if (pUnit)
    {
        pUnit->bSoundPlayed = 0;
    }
}

// 1.12a: 0x6FAE6800 (__fastcall)
BOOL __fastcall D2Client_GetUnitSoundPlayedFlag(D2UnitStrc* pUnit)
{
    return pUnit ? pUnit->bSoundPlayed : FALSE;
}

// 1.12a: 0x6FAE6810 (__fastcall)
void __fastcall D2Client_SetUnitLastSoundMode(D2UnitStrc* pUnit, int32_t nMode)
{
    if (pUnit)
    {
        pUnit->dwLastSoundMode = nMode;
    }
}

// 1.12a: 0x6FAE6820 (__fastcall)
int32_t __fastcall D2Client_GetUnitLastSoundMode(D2UnitStrc* pUnit)
{
    return pUnit ? pUnit->dwLastSoundMode : 0;
}

// 1.12a: 0x6FAE6830 (__fastcall)
void __fastcall D2Client_SetUnitSoundList(D2UnitStrc* pUnit, D2SoundNodeStrc* pSoundList)
{
    if (pUnit)
    {
        pUnit->pSoundList = pSoundList;
    }
}

// 1.12a: 0x6FAE6840 (__fastcall)
D2SoundNodeStrc* __fastcall D2Client_GetUnitSoundList(D2UnitStrc* pUnit)
{
    return pUnit ? pUnit->pSoundList : nullptr;
}

// 1.12a: 0x6FAE6850 (__fastcall)
void __fastcall D2Client_SetUnitLastGenericSoundTick(D2UnitStrc* pUnit, uint32_t dwTick)
{
    if (pUnit)
    {
        pUnit->dwLastGenericSoundTick = dwTick;
    }
}

// 1.12a: 0x6FAE6860 (__fastcall)
uint32_t __fastcall D2Client_GetUnitLastGenericSoundTick(D2UnitStrc* pUnit)
{
    return pUnit ? pUnit->dwLastGenericSoundTick : 0;
}

// 1.12a: 0x6FAE6870 (__fastcall)
void __fastcall D2Client_SetUnitLastNeutralSoundTick(D2UnitStrc* pUnit, uint32_t dwTick)
{
    if (pUnit)
    {
        pUnit->dwLastNeutralSoundTick = dwTick;
    }
}

// 1.12a: 0x6FAE6880 (__fastcall)
uint32_t __fastcall D2Client_GetUnitLastNeutralSoundTick(D2UnitStrc* pUnit)
{
    return pUnit ? pUnit->dwLastNeutralSoundTick : 0;
}

// 1.12a: 0x6FAE6890 (__fastcall)
void __fastcall D2Client_SetUnitLastFootstepTick(D2UnitStrc* pUnit, uint32_t dwTick)
{
    if (pUnit)
    {
        pUnit->dwLastFootstepTick = dwTick;
    }
}

// 1.12a: 0x6FAE68A0 (__fastcall)
uint32_t __fastcall D2Client_GetUnitLastFootstepTick(D2UnitStrc* pUnit)
{
    return pUnit ? pUnit->dwLastFootstepTick : 0;
}

// 1.12a: 0x6FAE68B0 (__fastcall)
void __fastcall D2Client_SetUnitSoundsPreloadedFlag(D2UnitStrc* pUnit)
{
    if (pUnit)
    {
        pUnit->bSoundsPreloaded = 1;
    }
}

// 1.12a: 0x6FAE68C0 (__fastcall)
void __fastcall D2Client_ClearUnitSoundsPreloadedFlag(D2UnitStrc* pUnit)
{
    if (pUnit)
    {
        pUnit->bSoundsPreloaded = 0;
    }
}

// 1.12a: 0x6FAE68D0 (__fastcall)
BOOL __fastcall D2Client_GetUnitSoundsPreloadedFlag(D2UnitStrc* pUnit)
{
    return pUnit ? pUnit->bSoundsPreloaded : FALSE;
}

// 1.12a: 0x6FAE68E0 (custom: esi = pUnit)
int32_t __fastcall D2Client_GetUnitAnimMode(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        FOG_DisplayAssert("hUnit", "..\\Source\\D2Client\\Unit\\UnitSnd.cpp", 679);
        exit(-1);
    }
    return pUnit->dwAnimMode;
}

// 1.12a: 0x6FAE6910 (custom: esi = pUnit)
int32_t __fastcall D2Client_GetUnitX(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }
    if (pUnit->dwUnitType <= UNIT_OBJECT || pUnit->dwUnitType == UNIT_ITEM || pUnit->dwUnitType == UNIT_TILE)
    {
        return pUnit->pStaticPath ? D2COMMON_10175_PathGetFirstPointX(pUnit->pStaticPath) : 0;
    }
    return pUnit->pDynamicPath ? PATH_GetXPosition(pUnit->pDynamicPath) : 0;
}

// 1.12a: 0x6FAE6920 (custom: esi = pUnit)
int32_t __fastcall D2Client_GetUnitY(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }
    if (pUnit->dwUnitType <= UNIT_OBJECT || pUnit->dwUnitType == UNIT_ITEM || pUnit->dwUnitType == UNIT_TILE)
    {
        return pUnit->pStaticPath ? D2COMMON_10176_PathGetFirstPointY(pUnit->pStaticPath) : 0;
    }
    return pUnit->pDynamicPath ? PATH_GetYPosition(pUnit->pDynamicPath) : 0;
}

// 1.12a: 0x6FAE6930 (custom: esi = pUnit -> eax = dwUnitType)
uint32_t __fastcall D2Client_GetUnitTypeChecked(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        FOG_DisplayAssert("hUnit", "..\\Source\\D2Client\\Unit\\UnitSnd.cpp", 709);
        exit(-1);
    }
    return pUnit->dwUnitType;
}

// 1.12a: 0x6FAE6960 (custom: esi = pUnit -> eax = dwUnitId)
uint32_t __fastcall D2Client_GetUnitGUIDChecked(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        FOG_DisplayAssert("hUnit", "..\\Source\\D2Client\\Unit\\UnitSnd.cpp", 722);
        exit(-1);
    }
    return pUnit->dwUnitId;
}

// 1.12a: 0x6FAE6990 (custom: esi = pUnit -> eax = dwClassId)
uint32_t __fastcall D2Client_GetUnitClassIdChecked(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        FOG_DisplayAssert("hUnit", "..\\Source\\D2Client\\Unit\\UnitSnd.cpp", 735);
        exit(-1);
    }
    return pUnit->dwClassId;
}

// 1.12a: 0x6FAE69C0 (custom: esi = pUnit -> eax = pRoom)
D2RoomStrc* __fastcall D2Client_GetUnitRoomChecked(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        FOG_DisplayAssert("hUnit", "..\\Source\\D2Client\\Unit\\UnitSnd.cpp", 748);
        exit(-1);
    }
    return UNITS_GetRoom(pUnit);
}

// 1.12a: 0x6FAE69F0 (custom: esi = pUnit -> eax = nSoundId)
int32_t __fastcall D2Client_GetObjectOperateSoundId(D2UnitStrc* pUnit)
{
    int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
    switch (nClassId)
    {
    case 1:
    case 5:
    case 6:
    case 8:
    case 119:
        return 1185;
    case 2:
    case 9:
    case 28:
        return 0;
    default:
        return 1185;
    }
}

// 1.12a: 0x6FAE6A70 (custom: esi = pUnit -> eax = nSoundId)
int32_t __fastcall D2Client_GetObjectOpenedSoundId(D2UnitStrc* pUnit)
{
    int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
    switch (nClassId)
    {
    case 3:  return 1182;
    case 4:  return 1181;
    case 7:  return 1180;
    case 13:
    case 14: return 1173;
    case 15:
    case 16: return 1175;
    default: return 0;
    }
}

// 1.12a: 0x6FAE6AD0 (custom: eax = pUnit -> eax = nTileSoundIndex)
int32_t __fastcall D2Client_GetUnitTileSoundMaterial(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }
    if (gnForceTileSoundType_6FBCC360 != -1)
    {
        return gnForceTileSoundType_6FBCC360;
    }

    D2RoomStrc* pRoom = D2Client_GetUnitRoomChecked(pUnit);
    int32_t nX = D2Client_GetUnitX(pUnit);
    int32_t nY = D2Client_GetUnitY(pUnit);
    uint32_t dwTileFlags = D2Common_10920(pRoom, nX, nY);

    if (dwTileFlags & 0x00010000) return 1;
    if (dwTileFlags & 0x00020000) return 2;
    if (dwTileFlags & 0x00040000) return 3;
    if (dwTileFlags & 0x00080000) return 4;
    if (dwTileFlags & 0x00100000) return 5;
    if (dwTileFlags & 0x00200000) return 6;
    if (dwTileFlags & 0x00400000) return 7;
    if (dwTileFlags & 0x00800000) return 8;
    if (dwTileFlags & 0x01000000) return 9;
    if (dwTileFlags & 0x02000000) return 10;
    if (dwTileFlags & 0x04000000) return 11;
    if (dwTileFlags & 0x08000000) return 12;
    if (dwTileFlags & 0x10000000) return 13;
    if (dwTileFlags & 0x20000000) return 14;
    if (dwTileFlags & 0x40000000) return 15;
    if ((int32_t)dwTileFlags < 0) return 16;

    switch (LEVEL_GetActFromRoom(pRoom))
    {
    case 0:  return 18;
    case 1:  return 19;
    case 2:  return 20;
    case 3:  return 21;
    default: return 0;
    }
}

// 1.12a: 0x6FAE6BD0 (custom: ecx = pUnit -> eax = nWeaponSwingSoundId)
int32_t __fastcall D2Client_GetUnitActiveWeaponSwingSound(D2UnitStrc* pUnit)
{
    uint32_t dwUnitType = D2Client_GetUnitTypeChecked(pUnit);
    if (dwUnitType != UNIT_PLAYER && dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }
    if (!pUnit->pInventory)
    {
        return 0;
    }

    D2UnitStrc* pWeapon = nullptr;
    uint8_t nWeapSwitch = (uint8_t)D2Common_11076(pUnit, pUnit->dwAnimMode);
    if (nWeapSwitch == 11) // BULD/Left
    {
        pWeapon = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
    }
    else if (nWeapSwitch == 12) // Right
    {
        pWeapon = D2Common_10306(pUnit->pInventory);
    }
    else
    {
        pWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
        if (!pWeapon || !ITEMS_CanBeEquipped(pWeapon) || ITEMS_CheckWeaponIfTwoHanded(pWeapon))
        {
            pWeapon = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
        }
    }

    int32_t nHitClass = 0;
    if (pWeapon)
    {
        if (ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_WEAPON))
        {
            nHitClass = ITEMS_GetHitClass(pWeapon);
        }
        else
        {
            nHitClass = 1;
        }
    }
    else if (dwUnitType == UNIT_PLAYER)
    {
        nHitClass = 1;
    }

    if (pUnit->dwAnimMode == PLRMODE_KICK)
    {
        if (pUnit->dwClassId == PCLASS_ASSASSIN)
        {
            return 2327;
        }
        nHitClass = 1;
    }
    else if (pUnit->dwAnimMode == PLRMODE_THROW)
    {
        nHitClass = 10;
    }

    return D2Client_GetWeaponSwingSoundByHitClass(nHitClass);
}

// 1.12a: 0x6FAE6CC0 (custom: esi = pUnit)
void __fastcall D2Client_PlayPlayerLevelUpSound(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        FOG_DisplayAssert("hUnit", "..\\Source\\D2Client\\Unit\\UnitSnd.cpp", 996);
        exit(-1);
    }
    if (D2Client_GetUnitTypeChecked(pUnit) != UNIT_PLAYER)
    {
        FOG_DisplayAssert("UnitGetType(hUnit) == UNIT_PLAYER", "..\\Source\\D2Client\\Unit\\UnitSnd.cpp", 997);
        exit(-1);
    }
    D2Client_PlayUnitPositionalSound(pUnit, 7, 0, 0, 0);
}

// 1.12a: 0x6FAE6D30 (custom: esi = pUnit -> eax = nSoundId)
int32_t __fastcall D2Client_GetUnitDefaultWeaponImpactSound(D2UnitStrc* pUnit)
{
    uint32_t dwUnitType = D2Client_GetUnitTypeChecked(pUnit);
    if (dwUnitType == UNIT_PLAYER)
    {
        if (!pUnit->pInventory)
        {
            return 0;
        }
        D2UnitStrc* pWeapon = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
        int32_t nHitClass = pWeapon ? ITEMS_GetHitClass(pWeapon) : 1;
        return D2Client_GetWeaponImpactSoundByHitClass(nHitClass);
    }
    else if (dwUnitType == UNIT_MONSTER)
    {
        int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
        int32_t nHitClass = sgptDataTables->pMonStatsTxt[nClassId].nA1TH; // +0x154
        return D2Client_GetWeaponImpactSoundByHitClass(nHitClass);
    }
    return 0;
}

// 1.12a: 0x6FAE6DA0 (custom: ecx = pUnit, eax = nExpectedMode)
void __fastcall D2Client_UpdateSoundPlayedFlagForMode(D2UnitStrc* pUnit, int32_t nExpectedMode)
{
    if (!pUnit)
    {
        return;
    }

    int32_t nAnimMode = D2Client_GetUnitAnimMode(pUnit);
    uint32_t dwUnitType = D2Client_GetUnitTypeChecked(pUnit);
    if (dwUnitType == UNIT_PLAYER)
    {
        if (nAnimMode == PLRMODE_TOWNWALK) nAnimMode = PLRMODE_WALK;
        if (nAnimMode == PLRMODE_TOWNNEUTRAL) nAnimMode = PLRMODE_NEUTRAL;
    }

    if (nAnimMode != nExpectedMode)
    {
        return;
    }

    int32_t nFrame = UNITS_GetCurrentFrame(pUnit) >> 8;
    if (nFrame == 0)
    {
        if (!pUnit->bSoundPlayed)
        {
            pUnit->bSoundPlayed = 1;
        }
    }
    else if (pUnit->bSoundPlayed == 1)
    {
        pUnit->bSoundPlayed = 0;
    }
}

// 1.12a: 0x6FAE6E20 (custom: eax = pUnit, edi = pOutPhysSoundId, stack: nHitClass, nAttackerType, pOutElemSoundId)
void __stdcall D2Client_ResolveHitClassImpactSounds(D2UnitStrc* pUnit, int32_t* pOutPhysSoundId, uint32_t dwHitClass, uint32_t dwAttackerType, int32_t* pOutElemSoundId)
{
    uint32_t dwLowNibble = dwHitClass & 0x0F;
    if (!dwLowNibble)
    {
        if (dwAttackerType == 1)
        {
            *pOutPhysSoundId = D2Client_GetWeaponImpactSoundByHitClass(10);
        }
        else if (dwAttackerType == 2)
        {
            *pOutPhysSoundId = 0;
        }
        else
        {
            *pOutPhysSoundId = D2Client_GetWeaponImpactSoundByHitClass(1);
        }
    }
    else
    {
        *pOutPhysSoundId = D2Client_GetWeaponImpactSoundByHitClass(dwLowNibble);
    }

    uint32_t dwHighNibble = dwHitClass & 0xF0;
    if (dwHighNibble)
    {
        *pOutElemSoundId = D2Client_GetElementalImpactSoundByHitClass(dwHighNibble);
    }
    else
    {
        *pOutElemSoundId = 0;
    }

    if (D2Client_GetUnitTypeChecked(pUnit) == UNIT_MONSTER)
    {
        int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
        uint8_t nHitClassOverride = sgptDataTables->pMonStatsTxt[nClassId].nHitClass; // +0x93
        switch (nHitClassOverride)
        {
        case 0x0B: *pOutPhysSoundId = 113; break;
        case 0x0C: *pOutPhysSoundId = 132; break;
        case 0x10: *pOutPhysSoundId = 148; break;
        case 0x20: *pOutPhysSoundId = 128; break;
        case 0x30: *pOutPhysSoundId = 140; break;
        case 0x40: *pOutPhysSoundId = 124; break;
        case 0x50: *pOutPhysSoundId = 152; break;
        case 0x80: *pOutPhysSoundId = 136; break;
        case 0xB0: *pOutPhysSoundId = 157; break;
        default:   break;
        }
    }
}

// 1.12a: 0x6FAE70B0 (custom: eax = pUnit -> eax = pMonSoundsTxt)
D2MonSoundsTxt* __fastcall D2Client_GetUnitMonSoundsTxt(D2UnitStrc* pUnit)
{
    if (!pUnit || D2Client_GetUnitTypeChecked(pUnit) != UNIT_MONSTER)
    {
        return nullptr;
    }
    int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
    D2MonStatsTxt* pMonStatsTxt = &sgptDataTables->pMonStatsTxt[nClassId];
    if (!pMonStatsTxt)
    {
        return nullptr;
    }
    if (STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_GOOD)
    {
        return D2Client_GetMonSoundsTxtRecord(pMonStatsTxt->wPetSound);
    }
    if (MONSTER_IsBoss(nullptr, pUnit))
    {
        return D2Client_GetMonSoundsTxtRecord(pMonStatsTxt->wUMonSound);
    }
    return D2Client_GetMonSoundsTxtRecord(pMonStatsTxt->wMonSound);
}

// 1.12a: 0x6FAE7160 (custom: eax = nSkipSoundId, ecx = pUnit)
void __fastcall D2Client_StopAllUnitLoopingSoundsExcept(D2UnitStrc* pUnit, int32_t nSkipSoundId)
{
    if (!pUnit)
    {
        return;
    }
    D2SoundNodeStrc* pNode = pUnit->pSoundList;
    if (!pNode)
    {
        return;
    }
    while (pNode)
    {
        D2SoundNodeStrc* pNext = pNode->pNext;
        if (pNode->nSoundId != nSkipSoundId)
        {
            D2Client_RemoveUnitSoundNode_6FAE19F0(pUnit, pNode);
        }
        pNode = pNext;
    }
    if (nSkipSoundId == -1)
    {
        pUnit->pSoundList = nullptr;
    }
}

// 1.12a: 0x6FAE71A0 (custom: ecx = pUnit -> eax = bOutOfRange)
BOOL __fastcall D2Client_IsUnitSoundTooFarFromPlayer(D2UnitStrc* pUnit)
{
    if (!gpPlayerUnit)
    {
        return FALSE;
    }
    int32_t nPlayerX = D2Client_GetUnitX(gpPlayerUnit);
    int32_t nPlayerY = D2Client_GetUnitY(gpPlayerUnit);
    int32_t nUnitX = D2Client_GetUnitX(pUnit);
    int32_t nUnitY = D2Client_GetUnitY(pUnit);

    int32_t nDiffX = (nPlayerX - nUnitX) * (nPlayerX - nUnitX);
    int32_t nDiffY = (nPlayerY - nUnitY) * (nPlayerY - nUnitY);
    return (nDiffX >= 2500 || nDiffY >= 2500);
}

// 1.12a: 0x6FAE7250 (__fastcall)
void __fastcall D2Client_PlayUnitSpeechOrVoiceSound(D2UnitStrc* pUnit, int32_t nSpeechType)
{
    if (!pUnit || D2Client_GetUnitTypeChecked(pUnit) != UNIT_PLAYER)
    {
        return;
    }

    int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
    D2CharVoiceEntryStrc* pVoice = D2Client_GetCharVoiceTableEntry(nClassId);
    if (!pVoice)
    {
        return;
    }

    if (nSpeechType >= 25 && nSpeechType <= 32)
    {
        int32_t nSoundId = gCharBarkSoundIndices_6FBA2068[nSpeechType - 25] + nClassId * 18;
        if (pUnit == gpPlayerUnit)
        {
            D2Client_PlayGlobalSound_6FAE1470(nSoundId, 0xFF);
        }
        else if (!D2Client_IsUnitSoundTooFarFromPlayer(pUnit))
        {
            int32_t nUnitX = D2Client_GetUnitX(pUnit);
            int32_t nUnitY = D2Client_GetUnitY(pUnit);
            D2Client_PlayPositionalSoundAtCoords_6FAE12B0(nSoundId, nUnitX, nUnitY, 0, 0, 0);
        }
        return;
    }

    int32_t nSlot = 0;
    if (nSpeechType >= 38 && nSpeechType <= 56)
    {
        switch (nSpeechType)
        {
        case 39:
        case 45:
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
            if (pUnit == gpPlayerUnit)
            {
                D2Client_PlayGlobalSound_6FAE1470(4666, 0xFF);
            }
            return;
        default:
            if (pUnit == gpPlayerUnit)
            {
                D2Client_PlayGlobalSound_6FAE1470(4667, 0xFF);
            }
            return;
        }
    }

    switch (nSpeechType)
    {
    case 1:  nSlot = 0;  break;
    case 2:  nSlot = 1;  break;
    case 3:  nSlot = 2;  break;
    case 4:  nSlot = 3;  break;
    case 5:  nSlot = 4;  break;
    case 6:  nSlot = 5;  break;
    case 7:  nSlot = 6;  break;
    case 8:  nSlot = 7;  break;
    case 9:  nSlot = 8;  break;
    case 11: nSlot = 10; break;
    case 19: nSlot = 9;  break;
    case 20: nSlot = 23; break;
    case 21: nSlot = 19; break;
    case 22: nSlot = 20; break;
    case 23: nSlot = 21; break;
    case 24: nSlot = 24; break;
    default:
        nSlot = D2Client_MapQuestSpeechTypeToSlotIndex(nSpeechType);
        break;
    }

    uint32_t dwSoundId = pVoice->dwSpeechSounds[nSlot];
    if (dwSoundId && pUnit == gpPlayerUnit)
    {
        D2Client_PlayGlobalSound_6FAE1470(dwSoundId, 0xFF);
    }
}

// 1.12a: 0x6FAE7680 (custom: eax = pUnit)
void __fastcall D2Client_PlayMonsterAttack1Sound(D2UnitStrc* pUnit)
{
    D2MonSoundsTxt* pMonSoundsTxt = D2Client_GetUnitMonSoundsTxt(pUnit);
    if (!pMonSoundsTxt)
    {
        return;
    }

    int32_t nWeaponSound = pMonSoundsTxt->dwWeapon1;
    if (nWeaponSound <= 0)
    {
        nWeaponSound = D2Client_GetUnitActiveWeaponSwingSound(pUnit);
    }
    if (nWeaponSound > 0)
    {
        D2Client_PlayUnitPositionalSound(pUnit, nWeaponSound, pMonSoundsTxt->dwWea1Del, 0, pMonSoundsTxt->dwWea1Vol);
    }
    if (pMonSoundsTxt->dwAttack1 > 0)
    {
        D2Client_PlayUnitPositionalSound(pUnit, pMonSoundsTxt->dwAttack1, pMonSoundsTxt->dwAtt1Del, pMonSoundsTxt->dwAtt1Prb, 0);
    }
}

// 1.12a: 0x6FAE7710 (custom: eax = pUnit)
void __fastcall D2Client_PlayMonsterAttack2Sound(D2UnitStrc* pUnit)
{
    D2MonSoundsTxt* pMonSoundsTxt = D2Client_GetUnitMonSoundsTxt(pUnit);
    if (!pMonSoundsTxt)
    {
        return;
    }

    int32_t nWeaponSound = pMonSoundsTxt->dwWeapon2;
    if (nWeaponSound <= 0)
    {
        nWeaponSound = D2Client_GetUnitActiveWeaponSwingSound(pUnit);
    }
    if (nWeaponSound > 0)
    {
        D2Client_PlayUnitPositionalSound(pUnit, nWeaponSound, pMonSoundsTxt->dwWea2Del, 0, pMonSoundsTxt->dwWea2Vol);
    }
    if (pMonSoundsTxt->dwAttack2 > 0)
    {
        int32_t nAttack2Sound = pMonSoundsTxt->dwAttack2;
        if (D2Client_GetUnitTypeChecked(pUnit) == UNIT_MONSTER
            && D2Client_GetUnitClassIdChecked(pUnit) == 243 // Diablo
            && UNITS_GetCurrentLifePercentage(pUnit) > 50
            && nAttack2Sound == 1043)
        {
            nAttack2Sound = 1042;
        }
        D2Client_PlayUnitPositionalSound(pUnit, nAttack2Sound, pMonSoundsTxt->dwAtt2Del, pMonSoundsTxt->dwAtt2Prb, 0);
    }
}

// 1.12a: 0x6FAE77D0 (custom: eax = pUnit)
void __fastcall D2Client_PlayMonsterDeathSound(D2UnitStrc* pUnit)
{
    D2MonSoundsTxt* pMonSoundsTxt = D2Client_GetUnitMonSoundsTxt(pUnit);
    if (pMonSoundsTxt && pMonSoundsTxt->dwDeathSound > 0)
    {
        D2Client_PlayUnitPositionalSound(pUnit, pMonSoundsTxt->dwDeathSound, pMonSoundsTxt->dwDeaDel, 0, 0);
    }
}

// 1.12a: 0x6FAE7800 (custom: eax = pUnit -> eax = bPreloaded)
BOOL __fastcall D2Client_PreloadMonsterSounds(D2UnitStrc* pUnit)
{
    if (!pUnit || D2Client_GetUnitTypeChecked(pUnit) != UNIT_MONSTER)
    {
        return FALSE;
    }
    if (pUnit->bSoundsPreloaded)
    {
        return TRUE;
    }

    D2MonSoundsTxt* pMonSoundsTxt = D2Client_GetUnitMonSoundsTxt(pUnit);
    if (!pMonSoundsTxt)
    {
        return FALSE;
    }

    if (pMonSoundsTxt->dwAttack1 > 0)    D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwAttack1);
    if (pMonSoundsTxt->dwWeapon1 > 0)    D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwWeapon1);
    if (pMonSoundsTxt->dwAttack2 > 0)    D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwAttack2);
    if (pMonSoundsTxt->dwWeapon2 > 0)    D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwWeapon2);
    if (pMonSoundsTxt->dwHitSound > 0)   D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwHitSound);
    if (pMonSoundsTxt->dwDeathSound > 0) D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwDeathSound);
    if (pMonSoundsTxt->dwSkill[0] > 0)   D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwSkill[0]);
    if (pMonSoundsTxt->dwSkill[1] > 0)   D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwSkill[1]);
    if (pMonSoundsTxt->dwSkill[2] > 0)   D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwSkill[2]);
    if (pMonSoundsTxt->dwSkill[3] > 0)   D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwSkill[3]);
    if (pMonSoundsTxt->dwFootstep > 0)   D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwFootstep);
    if (pMonSoundsTxt->dwFootstepLayer > 0) D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwFootstepLayer);
    if (pMonSoundsTxt->dwNeutral > 0)    D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwNeutral);
    if (pMonSoundsTxt->dwInit > 0)       D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwInit);
    if (pMonSoundsTxt->dwTaunt > 0)      D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwTaunt);
    if (pMonSoundsTxt->dwFlee > 0)       D2Client_PreloadSound_6FAE1220(pMonSoundsTxt->dwFlee);

    pUnit->bSoundsPreloaded = 1;
    return TRUE;
}

// 1.12a: 0x6FAE78E0 (custom: eax = pUnit)
void __fastcall D2Client_PlayMonsterInitSound(D2UnitStrc* pUnit)
{
    D2MonSoundsTxt* pMonSoundsTxt = D2Client_GetUnitMonSoundsTxt(pUnit);
    if (!pMonSoundsTxt)
    {
        return;
    }

    int32_t nInitSound = pMonSoundsTxt->dwInit;
    if (nInitSound <= 0)
    {
        return;
    }

    D2MonStats2Txt* pMonStats2Txt = MONSTERMODE_GetMonStats2Txt(pUnit->dwClassId);
    if (pMonStats2Txt && (pMonStats2Txt->dwFlags & 0x00010000))
    {
        D2Client_PlayGlobalSound_6FAE1470(nInitSound, 0xFF);
        return;
    }

    if (D2Client_IsSoundLooping_6FAE20B0(nInitSound))
    {
        D2SoundNodeStrc* pNode = pUnit->pSoundList;
        while (pNode)
        {
            if (pNode->nSoundId == nInitSound)
            {
                return;
            }
            pNode = pNode->pNext;
        }
    }

    if (!D2Client_IsUnitSoundTooFarFromPlayer(pUnit) && !STATES_CheckState(pUnit, STATE_INVIS))
    {
        D2Client_PlayUnitPositionalSound(pUnit, nInitSound, 0, 0, 0);
        uint32_t dwTick = gdwClientTickCount;
        pUnit->dwLastGenericSoundTick = dwTick;
        pUnit->dwLastNeutralSoundTick = dwTick;
    }
}

// 1.12a: 0x6FAE79E0 (custom: eax = pUnit)
void __fastcall D2Client_PlayPlayerFootstepSound(D2UnitStrc* pUnit)
{
    if (!pUnit || D2Client_GetUnitTypeChecked(pUnit) != UNIT_PLAYER || !pUnit->pInventory)
    {
        return;
    }

    int32_t nAnimMode = D2Client_GetUnitAnimMode(pUnit);
    int32_t nFrame = UNITS_GetCurrentFrame(pUnit) >> 8;
    int32_t nMaxFrame = UNITS_GetAnimOrSeqFrameCount(pUnit);

    if (nFrame != 0 && nFrame != (nMaxFrame >> 1))
    {
        pUnit->dwLastFootstepTick = 0;
        return;
    }

    if ((int32_t)(gdwClientTickCount - pUnit->dwLastFootstepTick) <= 100)
    {
        return;
    }

    BOOL bHeavy = FALSE;
    if (nAnimMode == PLRMODE_RUN)
    {
        bHeavy = TRUE;
    }
    else
    {
        switch (INVENTORY_GetWornItemComponent(pUnit, pUnit->pInventory, 1))
        {
        case 1:
        case 2:
            bHeavy = TRUE;
            break;
        default:
            bHeavy = FALSE;
            break;
        }
    }

    int32_t nTileSoundMat = D2Client_GetUnitTileSoundMaterial(pUnit);
    int32_t nSoundId = bHeavy
        ? D2Client_GetHeavyFootstepSoundByMaterial(nTileSoundMat)
        : D2Client_GetLightFootstepSoundByMaterial(nTileSoundMat);

    D2Client_PlayUnitPositionalSound(pUnit, nSoundId, 0, 0, 0);
    uint32_t dwTick = gdwClientTickCount;
    pUnit->dwLastGenericSoundTick = dwTick;
    pUnit->dwLastFootstepTick = dwTick;
}

// 1.12a: 0x6FAE7AA0 (custom: eax = pUnit, edi = nSkillIndex)
void __fastcall D2Client_PlayMonsterSkillModeSound(D2UnitStrc* pUnit, int32_t nSkillIndex)
{
    D2MonSoundsTxt* pMonSoundsTxt = D2Client_GetUnitMonSoundsTxt(pUnit);
    if (!pMonSoundsTxt)
    {
        return;
    }
    int32_t nSkillSound = pMonSoundsTxt->dwSkill[nSkillIndex];
    if (nSkillSound > 0)
    {
        D2Client_PlayUnitPositionalSound(pUnit, nSkillSound, 0, 0, 0);
    }
    uint32_t dwTick = gdwClientTickCount;
    pUnit->dwLastGenericSoundTick = dwTick;
    gdwLastCombatSoundTick_6FBCC35C = dwTick;
}

// 1.12a: 0x6FAE7AE0 (custom: eax = pUnit)
void __fastcall D2Client_PlayMonsterTauntSound(D2UnitStrc* pUnit)
{
    if (!pUnit || D2Client_GetUnitTypeChecked(pUnit) != UNIT_MONSTER)
    {
        return;
    }
    D2MonSoundsTxt* pMonSoundsTxt = D2Client_GetUnitMonSoundsTxt(pUnit);
    if (!pMonSoundsTxt)
    {
        return;
    }
    if (pMonSoundsTxt->dwTaunt > 0)
    {
        D2Client_PlayGlobalSound_6FAE1470(pMonSoundsTxt->dwTaunt, 0xFF);
        uint32_t dwTick = gdwClientTickCount;
        pUnit->dwLastGenericSoundTick = dwTick;
        pUnit->dwLastNeutralSoundTick = dwTick;
    }
}

// 1.12a: 0x6FAE7B30 (__fastcall)
void __fastcall D2Client_PlayMonsterFleeSound(D2UnitStrc* pUnit)
{
    if (!pUnit || D2Client_GetUnitTypeChecked(pUnit) != UNIT_MONSTER)
    {
        return;
    }
    D2MonSoundsTxt* pMonSoundsTxt = D2Client_GetUnitMonSoundsTxt(pUnit);
    if (!pMonSoundsTxt)
    {
        return;
    }
    if (pMonSoundsTxt->dwFlee > 0)
    {
        D2Client_PlayUnitPositionalSound(pUnit, pMonSoundsTxt->dwFlee, 0, 0, 0);
        uint32_t dwTick = gdwClientTickCount;
        pUnit->dwLastGenericSoundTick = dwTick;
        pUnit->dwLastNeutralSoundTick = dwTick;
    }
}

// 1.12a: 0x6FAE7B80 (__fastcall)
void __fastcall D2Client_PlayPlayerTauntSpeech(D2UnitStrc* pUnit)
{
    if (!pUnit || D2Client_GetUnitTypeChecked(pUnit) != UNIT_PLAYER)
    {
        return;
    }
    int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
    D2CharVoiceEntryStrc* pVoice = D2Client_GetCharVoiceTableEntry(nClassId);
    if (pVoice && pVoice->dwSpeechSounds[22] > 0)
    {
        D2Client_PlayGlobalSound_6FAE1470(pVoice->dwSpeechSounds[22], 0xFF);
        pUnit->dwLastGenericSoundTick = gdwClientTickCount;
    }
}

// 1.12a: 0x6FAE7BD0 (__fastcall)
void __fastcall D2Client_PlayPlayerTownPortalSpeech(D2UnitStrc* pUnit)
{
    if (!pUnit || D2Client_GetUnitTypeChecked(pUnit) != UNIT_PLAYER)
    {
        return;
    }
    if (gdwClientTickCount < gdwLastPlayerSpeechTick_6FBA2090 + 3000)
    {
        return;
    }
    gdwLastPlayerSpeechTick_6FBA2090 = gdwClientTickCount;

    int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
    D2CharVoiceEntryStrc* pVoice = D2Client_GetCharVoiceTableEntry(nClassId);
    if (pVoice && pVoice->dwSpeechSounds[11] > 0)
    {
        D2Client_PlayGlobalSound_6FAE1470(pVoice->dwSpeechSounds[11], 0xFF);
    }
}

// 1.12a: 0x6FAE7C30 (__fastcall)
void __fastcall D2Client_TriggerQuestMonologueOrSound(D2UnitStrc* pUnit, int32_t nEventType)
{
    if (!pUnit || D2Client_GetUnitTypeChecked(pUnit) != UNIT_PLAYER)
    {
        return;
    }

    int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
    D2CharVoiceEntryStrc* pVoice = D2Client_GetCharVoiceTableEntry(nClassId);
    if (!pVoice)
    {
        return;
    }

    uint32_t dwSoundId = 0;
    uint32_t dwDelayTicks = 0;

    switch (nEventType)
    {
    case 10: dwSoundId = pVoice->dwSpeechSounds[11]; break;
    case 12: dwSoundId = pVoice->dwSpeechSounds[12]; break;
    case 13: dwSoundId = 1581; break;
    case 14: dwSoundId = 1582; break;
    case 15:
        dwSoundId = pVoice->dwSpeechSounds[13];
        D2Client_OpenSpecialQuestDialog_6FB1EBF0(pUnit, 37 + nClassId);
        break;
    case 16:
        dwSoundId = pVoice->dwSpeechSounds[14];
        D2Client_OpenSpecialQuestDialog_6FB1EBF0(pUnit, 58 + nClassId);
        D2Client_PlayGlobalSound_6FAE1470(1583, 0xFF);
        dwDelayTicks = 175;
        break;
    case 17: dwSoundId = pVoice->dwSpeechSounds[15]; break;
    case 18:
        dwSoundId = pVoice->dwSpeechSounds[16];
        D2Client_OpenSpecialQuestDialog_6FB1EBF0(pUnit, 80 + nClassId);
        break;
    case 84:
        dwSoundId = pVoice->dwSpeechSounds[17];
        D2Client_OpenSpecialQuestDialog_6FB1EBF0(pUnit, 594 + nClassId);
        break;
    case 85:
        dwSoundId = pVoice->dwSpeechSounds[18];
        D2Client_OpenSpecialQuestDialog_6FB1EBF0(pUnit, 608 + nClassId);
        break;
    case 86:
        dwSoundId = pVoice->dwSpeechSounds[17];
        D2Client_OpenSpecialQuestDialog_6FB1EBF0(pUnit, 594 + nClassId);
        dwDelayTicks = 300;
        break;
    case 87:
        dwSoundId = pVoice->dwSpeechSounds[18];
        D2Client_OpenSpecialQuestDialog_6FB1EBF0(pUnit, 608 + nClassId);
        dwDelayTicks = 300;
        break;
    case 90: dwSoundId = 4594; break;
    case 91: dwSoundId = 4695; break;
    case 92:
        D2Client_OpenSpecialQuestDialog_6FB1EBF0(pUnit, 659 + nClassId);
        dwSoundId = pVoice->dwSpeechSounds[56];
        dwDelayTicks = 75;
        break;
    case 93: dwSoundId = 4696; break;
    default:
    {
        int32_t nSlot = D2Client_MapQuestSpeechTypeToSlotIndex(nEventType);
        dwSoundId = pVoice->dwSpeechSounds[nSlot];
        break;
    }
    }

    if (dwSoundId && pUnit == gpPlayerUnit)
    {
        if (!dwDelayTicks)
        {
            D2Client_PlayGlobalSound_6FAE1470(dwSoundId, 0xFF);
            gnPendingQuestSpeechSoundId_6FBA2094 = 0;
        }
        else
        {
            gnPendingQuestSpeechSoundId_6FBA2094 = dwSoundId;
            D2Client_SetDelayedQuestSpeechTicks_6FB08DC0(dwDelayTicks);
        }
    }
}

// 1.12a: 0x6FAE7FC0 (custom: eax = pUnit, stack: nAttackerType)
void __stdcall D2Client_PlayUnitGetHitOrDeathSound(D2UnitStrc* pUnit, uint32_t dwAttackerType)
{
    uint32_t dwUnitType = D2Client_GetUnitTypeChecked(pUnit);
    uint32_t dwHitClass = pUnit->dwLastHitClass; // +0xB0

    if (dwUnitType == UNIT_MONSTER && STATES_CheckState(pUnit, STATE_SHATTER))
    {
        return;
    }

    int32_t nPhysSoundId = 0;
    int32_t nElemSoundId = 0;
    D2Client_ResolveHitClassImpactSounds(pUnit, &nPhysSoundId, dwHitClass, dwAttackerType, &nElemSoundId);

    int32_t nChannel = D2Client_PlayUnitPositionalSound(pUnit, nPhysSoundId, 0, 0, 0);
    D2Client_PlayUnitPositionalSound(pUnit, nElemSoundId, 0, 0, 0);

    if (nChannel && pUnit == gpPlayerUnit)
    {
        D2Client_PlayGlobalSound_6FAE1470(nChannel, 0xFF);
    }

    int32_t nAnimMode = pUnit->dwAnimMode;
    if (dwUnitType == UNIT_MONSTER && nAnimMode == MONMODE_DEATH)
    {
        D2Client_PlayMonsterDeathSound(pUnit);
        D2Client_StopAllUnitLoopingSoundsExcept(pUnit, -1);
    }

    if (STATES_CheckState(pUnit, STATE_FREEZE))
    {
        return;
    }
    if ((dwHitClass & 0xF0) == 0xA0)
    {
        return;
    }

    if (dwUnitType == UNIT_MONSTER)
    {
        D2MonSoundsTxt* pMonSoundsTxt = D2Client_GetUnitMonSoundsTxt(pUnit);
        if (pMonSoundsTxt)
        {
            int32_t nSoundId = 0;
            uint32_t dwDelay = 0;
            if (nAnimMode == MONMODE_GETHIT || nAnimMode == MONMODE_KNOCKBACK)
            {
                if (D2Client_IsUnitSoundChannelActive_6FAE11E0(pUnit, 0))
                {
                    goto UPDATE_TICKS;
                }
                nSoundId = pMonSoundsTxt->dwHitSound;
                dwDelay = pMonSoundsTxt->dwHitDelay;
            }
            else
            {
                nSoundId = pMonSoundsTxt->dwDeathSound;
                dwDelay = pMonSoundsTxt->dwDeaDel;
            }

            if (nSoundId)
            {
                D2Client_PlayUnitPositionalSound(pUnit, nSoundId, dwDelay, 0, 0);
            }
            if (nSoundId == 745) // Shenk/Catapult composite death
            {
                D2Client_PlayUnitPositionalSound(pUnit, 746, 106, 0, 0);
                D2Client_PlayUnitPositionalSound(pUnit, 747, 193, 0, 0);
            }
        }
    }
    else if (dwUnitType == UNIT_PLAYER)
    {
        int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
        D2CharVoiceEntryStrc* pVoice = D2Client_GetCharVoiceTableEntry(nClassId);
        if (pVoice)
        {
            int32_t nSoundId = 0;
            uint32_t dwDelay = 1;
            if (nAnimMode == PLRMODE_GETHIT || nAnimMode == PLRMODE_KNOCKBACK)
            {
                if (D2Client_IsUnitSoundChannelActive_6FAE11E0(pUnit, 0))
                {
                    goto UPDATE_TICKS;
                }
                nSoundId = pVoice->dwGetHitSound;
                dwDelay = 2;
            }
            else
            {
                nSoundId = pVoice->dwDeathSound;
            }

            if (nSoundId)
            {
                D2Client_PlayUnitPositionalSound(pUnit, nSoundId, dwDelay, 0, 0);
            }
        }
    }

UPDATE_TICKS:
    pUnit->dwLastGenericSoundTick = gdwClientTickCount;
    gdwLastCombatSoundTick_6FBCC35C = gdwClientTickCount;
}

// 1.12a: 0x6FAE8170 (custom: eax = pUnit)
void __fastcall D2Client_UpdateObjectSoundsPerTick(D2UnitStrc* pUnit)
{
    int32_t nAnimMode = D2Client_GetUnitAnimMode(pUnit);
    if (nAnimMode != pUnit->dwLastSoundMode)
    {
        pUnit->dwLastSoundMode = nAnimMode;
        pUnit->bSoundPlayed = 0;
    }

    switch (nAnimMode)
    {
    case OBJMODE_NEUTRAL:
    {
        int32_t nSoundId = D2Client_GetObjectOperateSoundId(pUnit);
        if (D2Client_IsSoundLooping_6FAE20B0(nSoundId))
        {
            D2Client_StopAllUnitLoopingSoundsExcept(pUnit, nSoundId);
            if (!pUnit->pSoundList)
            {
                D2Client_PlayUnitPositionalSound(pUnit, nSoundId, 0, 0, 0);
            }
        }
        else
        {
            D2Client_StopAllUnitLoopingSoundsExcept(pUnit, -1);
            if (!pUnit->bSoundPlayed)
            {
                D2Client_PlayUnitPositionalSound(pUnit, nSoundId, 0, 0, 0);
            }
        }
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, OBJMODE_NEUTRAL);
        break;
    }
    case OBJMODE_OPERATING:
    {
        D2Client_StopAllUnitLoopingSoundsExcept(pUnit, -1);
        if (!pUnit->bSoundPlayed)
        {
            int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
            int32_t nSoundId = 0;
            switch (nClassId)
            {
            case 1:
            case 5:
            case 6:
            case 8:
            case 240:
            case 241:
            case 242:
            case 243:
            case 540:
            case 545:
                nSoundId = 1179;
                break;
            case 2:   nSoundId = 1184; break;
            case 3:   nSoundId = 1177; break;
            case 4:   nSoundId = 1176; break;
            case 7:   nSoundId = 1178; break;
            case 9:   nSoundId = 1183; break;
            case 13:  nSoundId = 1170; break;
            case 14:  nSoundId = 1169; break;
            case 15:  nSoundId = 1171; break;
            case 16:  nSoundId = 1172; break;
            case 28:  nSoundId = 1599; break;
            default:  nSoundId = 0;    break;
            }
            D2Client_PlayUnitPositionalSound(pUnit, nSoundId, 0, 0, 0);
        }
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, OBJMODE_OPERATING);
        break;
    }
    case OBJMODE_OPENED:
    {
        int32_t nSoundId = D2Client_GetObjectOpenedSoundId(pUnit);
        if (D2Client_IsSoundLooping_6FAE20B0(nSoundId))
        {
            D2Client_StopAllUnitLoopingSoundsExcept(pUnit, nSoundId);
            if (!pUnit->pSoundList)
            {
                D2Client_PlayUnitPositionalSound(pUnit, nSoundId, 0, 0, 0);
            }
        }
        else
        {
            D2Client_StopAllUnitLoopingSoundsExcept(pUnit, -1);
            if (!pUnit->bSoundPlayed)
            {
                D2Client_PlayUnitPositionalSound(pUnit, nSoundId, 0, 0, 0);
            }
        }
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, OBJMODE_OPENED);
        break;
    }
    case OBJMODE_SPECIAL1:
        D2Client_StopAllUnitLoopingSoundsExcept(pUnit, -1);
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, OBJMODE_SPECIAL1);
        break;
    case OBJMODE_SPECIAL2:
        D2Client_StopAllUnitLoopingSoundsExcept(pUnit, -1);
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, OBJMODE_SPECIAL2);
        break;
    case OBJMODE_SPECIAL3:
        D2Client_StopAllUnitLoopingSoundsExcept(pUnit, -1);
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, OBJMODE_SPECIAL3);
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FAE83A0 (custom: esi = pUnit)
void __fastcall D2Client_UpdatePlayerSoundsPerTick(D2UnitStrc* pUnit)
{
    int32_t nAnimMode = D2Client_GetUnitAnimMode(pUnit);
    if (nAnimMode == PLRMODE_TOWNWALK)    nAnimMode = PLRMODE_WALK;
    if (nAnimMode == PLRMODE_TOWNNEUTRAL) nAnimMode = PLRMODE_NEUTRAL;

    if (nAnimMode != pUnit->dwLastSoundMode)
    {
        pUnit->dwLastSoundMode = nAnimMode;
        pUnit->bSoundPlayed = 0;
    }

    switch (nAnimMode)
    {
    case PLRMODE_DEATH:
    case PLRMODE_DEAD:
        break;
    case PLRMODE_NEUTRAL:
        if (!pUnit->bSoundPlayed)
        {
            pUnit->dwLastNeutralSoundTick = gdwClientTickCount;
        }
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_NEUTRAL);
        break;
    case PLRMODE_WALK:
        D2Client_PlayPlayerFootstepSound(pUnit);
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_WALK);
        break;
    case PLRMODE_RUN:
        D2Client_PlayPlayerFootstepSound(pUnit);
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_RUN);
        break;
    case PLRMODE_GETHIT:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_GETHIT);
        break;
    case PLRMODE_BLOCK:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_BLOCK);
        break;
    case PLRMODE_ATTACK1:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_ATTACK1);
        break;
    case PLRMODE_ATTACK2:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_ATTACK2);
        break;
    case PLRMODE_CAST:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_CAST);
        break;
    case PLRMODE_THROW:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_THROW);
        break;
    case PLRMODE_KICK:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_KICK);
        break;
    case PLRMODE_SKILL1:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_SKILL1);
        break;
    case PLRMODE_SKILL2:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_SKILL2);
        break;
    case PLRMODE_SKILL3:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_SKILL3);
        break;
    case PLRMODE_SKILL4:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_SKILL4);
        break;
    case PLRMODE_SEQUENCE:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, PLRMODE_SEQUENCE);
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FAE84C0 (custom: esi = pUnit)
void __fastcall D2Client_PlayMonsterNeutralSoundPerTick(D2UnitStrc* pUnit)
{
    if (!pUnit || D2Client_GetUnitTypeChecked(pUnit) != UNIT_MONSTER)
    {
        return;
    }

    D2MonSoundsTxt* pMonSoundsTxt = D2Client_GetUnitMonSoundsTxt(pUnit);
    if (!pMonSoundsTxt || pMonSoundsTxt->dwNeutral <= 0)
    {
        return;
    }

    if (STATES_CheckState(pUnit, STATE_INVIS))
    {
        return;
    }

    if (D2Client_IsSoundLooping_6FAE20B0(pMonSoundsTxt->dwNeutral))
    {
        int32_t nNeutralSound = pMonSoundsTxt->dwNeutral;
        D2SoundNodeStrc* pNode = pUnit->pSoundList;
        while (pNode)
        {
            if (pNode->nSoundId == nNeutralSound)
            {
                return;
            }
            pNode = pNode->pNext;
        }
        D2Client_PlayUnitPositionalSound(pUnit, nNeutralSound, 0, 0, 0);
        return;
    }

    if (!D2Client_IsUnitSoundTooFarFromPlayer(pUnit)
        && gdwClientTickCount > pMonSoundsTxt->dwNeuTime + pUnit->dwLastNeutralSoundTick)
    {
        D2Client_PlayUnitPositionalSound(pUnit, pMonSoundsTxt->dwNeutral, 0, 0, 0);
        uint32_t dwTick = gdwClientTickCount;
        pUnit->dwLastGenericSoundTick = dwTick;
        pUnit->dwLastNeutralSoundTick = dwTick;
    }
}

// 1.12a: 0x6FAE8590 (custom: esi = pUnit)
void __fastcall D2Client_PlayMonsterFootstepSoundPerTick(D2UnitStrc* pUnit)
{
    if (!pUnit || D2Client_GetUnitTypeChecked(pUnit) != UNIT_MONSTER)
    {
        return;
    }

    D2MonSoundsTxt* pMonSoundsTxt = D2Client_GetUnitMonSoundsTxt(pUnit);
    int32_t nFrame = UNITS_GetCurrentFrame(pUnit) >> 8;
    int32_t nMaxFrame = UNITS_GetAnimOrSeqFrameCount(pUnit);

    if (pMonSoundsTxt)
    {
        if (D2Client_IsSoundLooping_6FAE20B0(pMonSoundsTxt->dwFootstep))
        {
            int32_t nFootstepSound = pMonSoundsTxt->dwFootstep;
            D2SoundNodeStrc* pNode = pUnit->pSoundList;
            while (pNode)
            {
                if (pNode->nSoundId == nFootstepSound)
                {
                    return;
                }
                pNode = pNode->pNext;
            }
            D2Client_PlayUnitPositionalSound(pUnit, nFootstepSound, 0, 0, 0);
            return;
        }

        if (nFrame != 0 && nFrame != (nMaxFrame >> 1))
        {
            pUnit->dwLastFootstepTick = 0;
            return;
        }

        if ((int32_t)(gdwClientTickCount - pUnit->dwLastFootstepTick) <= 100)
        {
            return;
        }

        if (pMonSoundsTxt->dwFootstep > 0)
        {
            D2Client_PlayUnitPositionalSound(pUnit, pMonSoundsTxt->dwFootstep, pMonSoundsTxt->dwFsOff, pMonSoundsTxt->dwFsPrb, pMonSoundsTxt->dwFsVol);
            if (pMonSoundsTxt->dwFootstepLayer > 0)
            {
                D2Client_PlayUnitPositionalSound(pUnit, pMonSoundsTxt->dwFootstepLayer, pMonSoundsTxt->dwFsOff, pMonSoundsTxt->dwFsPrb, pMonSoundsTxt->dwFsVol);
            }
            uint32_t dwTick = gdwClientTickCount;
            pUnit->dwLastGenericSoundTick = dwTick;
            pUnit->dwLastFootstepTick = dwTick;
            return;
        }
    }
    else
    {
        if (nFrame != 0 && nFrame != (nMaxFrame >> 1))
        {
            pUnit->dwLastFootstepTick = 0;
            return;
        }
        if ((int32_t)(gdwClientTickCount - pUnit->dwLastFootstepTick) <= 100)
        {
            return;
        }
    }

    int32_t nTileSoundMat = D2Client_GetUnitTileSoundMaterial(pUnit);
    int32_t nSoundId = D2Client_GetLightFootstepSoundByMaterial(nTileSoundMat);
    D2Client_PlayUnitPositionalSound(pUnit, nSoundId, 0, 0, 0);

    uint32_t dwTick = gdwClientTickCount;
    pUnit->dwLastGenericSoundTick = dwTick;
    pUnit->dwLastFootstepTick = dwTick;
}

// 1.12a: 0x6FAE8720 (custom: esi = pUnit)
void __fastcall D2Client_UpdateMonsterSoundsPerTick(D2UnitStrc* pUnit)
{
    int32_t nAnimMode = D2Client_GetUnitAnimMode(pUnit);
    if (nAnimMode != pUnit->dwLastSoundMode)
    {
        pUnit->dwLastSoundMode = nAnimMode;
        pUnit->bSoundPlayed = 0;
    }

    switch (nAnimMode)
    {
    case MONMODE_DEATH:
    case MONMODE_DEAD:
        break;
    case MONMODE_NEUTRAL:
        D2Client_PlayMonsterNeutralSoundPerTick(pUnit);
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, MONMODE_NEUTRAL);
        break;
    case MONMODE_WALK:
    case MONMODE_RUN:
        D2Client_PlayMonsterFootstepSoundPerTick(pUnit);
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, MONMODE_WALK);
        break;
    case MONMODE_GETHIT:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, MONMODE_GETHIT);
        break;
    case MONMODE_BLOCK:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, MONMODE_BLOCK);
        break;
    case MONMODE_ATTACK1:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, MONMODE_ATTACK1);
        break;
    case MONMODE_ATTACK2:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, MONMODE_ATTACK2);
        break;
    case MONMODE_CAST:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, MONMODE_CAST);
        break;
    case MONMODE_SKILL1:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, MONMODE_SKILL1);
        break;
    case MONMODE_SKILL2:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, MONMODE_SKILL2);
        break;
    case MONMODE_SKILL3:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, MONMODE_SKILL3);
        break;
    case MONMODE_SKILL4:
        D2Client_UpdateSoundPlayedFlagForMode(pUnit, MONMODE_SKILL4);
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FAE87B0 (__fastcall)
void __fastcall D2Client_PlayUnitBlockOrAttackSwingSound(D2UnitStrc* pUnit)
{
    uint32_t dwUnitType = D2Client_GetUnitTypeChecked(pUnit);
    int32_t nAnimMode = D2Client_GetUnitAnimMode(pUnit);
    if (dwUnitType == UNIT_PLAYER)
    {
        if (nAnimMode == PLRMODE_TOWNWALK)    nAnimMode = PLRMODE_WALK;
        if (nAnimMode == PLRMODE_TOWNNEUTRAL) nAnimMode = PLRMODE_NEUTRAL;

        switch (nAnimMode)
        {
        case PLRMODE_BLOCK:
            if (pUnit->pInventory)
            {
                switch (INVENTORY_GetWornItemComponent(pUnit, pUnit->pInventory, 7))
                {
                case 1:
                case 3:
                    D2Client_PlayUnitPositionalSound(pUnit, 117, 1, 0, 0);
                    break;
                case 2:
                    D2Client_PlayUnitPositionalSound(pUnit, 113, 1, 0, 0);
                    break;
                default:
                    D2Client_PlayUnitPositionalSound(pUnit, 121, 1, 0, 0);
                    break;
                }
            }
            pUnit->dwLastGenericSoundTick = gdwClientTickCount;
            break;

        case PLRMODE_ATTACK1:
        case PLRMODE_ATTACK2:
        case PLRMODE_THROW:
        case PLRMODE_KICK:
        case PLRMODE_SEQUENCE:
        {
            int32_t nSwingSound = D2Client_GetUnitActiveWeaponSwingSound(pUnit);
            D2Client_PlayUnitPositionalSound(pUnit, nSwingSound, 3, 0, 0);
            uint32_t dwTick = gdwClientTickCount;
            pUnit->dwLastGenericSoundTick = dwTick;
            gdwLastCombatSoundTick_6FBCC35C = dwTick;
            break;
        }
        default:
            break;
        }
    }
    else if (dwUnitType == UNIT_MONSTER)
    {
        if (nAnimMode == MONMODE_BLOCK)
        {
            int32_t nClassId = D2Client_GetUnitClassIdChecked(pUnit);
            switch (nClassId)
            {
            case 21: case 22: case 23: case 24: case 25:
                D2Client_PlayUnitPositionalSound(pUnit, 558, 0, 0, 0);
                break;
            case 16: case 17: case 18:
                D2Client_PlayUnitPositionalSound(pUnit, 113, 0, 0, 0);
                break;
            default:
                D2Client_PlayUnitPositionalSound(pUnit, 117, 0, 0, 0);
                break;
            }
            pUnit->dwLastGenericSoundTick = gdwClientTickCount;
        }
        else if (nAnimMode == MONMODE_ATTACK1)
        {
            D2Client_PlayMonsterAttack1Sound(pUnit);
            uint32_t dwTick = gdwClientTickCount;
            pUnit->dwLastGenericSoundTick = dwTick;
            gdwLastCombatSoundTick_6FBCC35C = dwTick;
        }
        else if (nAnimMode == MONMODE_ATTACK2)
        {
            D2Client_PlayMonsterAttack2Sound(pUnit);
            uint32_t dwTick = gdwClientTickCount;
            pUnit->dwLastGenericSoundTick = dwTick;
            gdwLastCombatSoundTick_6FBCC35C = dwTick;
        }
    }
}

// 1.12a: 0x6FAE8960 (__fastcall)
void __fastcall D2Client_UpdateUnitSoundsPerTick(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return;
    }
    uint32_t dwUnitType = D2Client_GetUnitTypeChecked(pUnit);
    if (dwUnitType == UNIT_PLAYER)
    {
        D2Client_UpdatePlayerSoundsPerTick(pUnit);
    }
    else if (dwUnitType == UNIT_MONSTER)
    {
        D2Client_UpdateMonsterSoundsPerTick(pUnit);
    }
    else if (dwUnitType == UNIT_OBJECT)
    {
        D2Client_UpdateObjectSoundsPerTick(pUnit);
    }
}

// 1.12a: 0x6FAE8990 (__fastcall)
void __fastcall D2Client_PlayUnitModeChangeSound(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return;
    }

    uint32_t dwUnitType = D2Client_GetUnitTypeChecked(pUnit);
    int32_t nAnimMode = D2Client_GetUnitAnimMode(pUnit);

    if (dwUnitType == UNIT_PLAYER)
    {
        if (nAnimMode == PLRMODE_TOWNWALK)    nAnimMode = PLRMODE_WALK;
        if (nAnimMode == PLRMODE_TOWNNEUTRAL) nAnimMode = PLRMODE_NEUTRAL;

        switch (nAnimMode)
        {
        case PLRMODE_DEATH:
        case PLRMODE_GETHIT:
        case PLRMODE_KNOCKBACK:
            D2Client_PlayUnitGetHitOrDeathSound(pUnit, 0);
            break;

        case PLRMODE_BLOCK:
            D2Client_PlayUnitBlockOrAttackSwingSound(pUnit);
            break;

        case PLRMODE_ATTACK1:
        case PLRMODE_ATTACK2:
            if (pUnit == gpPlayerUnit && (STATES_CheckState(pUnit, STATE_BEAR) || STATES_CheckState(pUnit, STATE_WOLF)))
            {
                D2Client_PlayUnitPositionalSound(pUnit, 0x1206, 0, 0, 0);
            }
            else
            {
                D2Client_PlayUnitBlockOrAttackSwingSound(pUnit);
            }
            break;

        case PLRMODE_THROW:
            if (pUnit != gpPlayerUnit)
            {
                D2Client_PlayUnitBlockOrAttackSwingSound(pUnit);
            }
            break;

        case PLRMODE_KICK:
            D2Client_PlayUnitBlockOrAttackSwingSound(pUnit);
            break;

        case PLRMODE_SEQUENCE:
            if (STATES_CheckState(pUnit, STATE_BEAR) || STATES_CheckState(pUnit, STATE_WOLF))
            {
                D2Client_PlayUnitPositionalSound(pUnit, 0x1206, 0, 0, 0);
            }
            else if (pUnit->dwClassId != PCLASS_PALADIN)
            {
                D2Client_PlayUnitBlockOrAttackSwingSound(pUnit);
            }
            break;

        case PLRMODE_SKILL1:
            if (pUnit->dwClassId == PCLASS_BARBARIAN)
            {
                D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pUnit);
                D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pUnit);
                if ((pLeftSkill && SKILLS_GetSkillIdFromSkill(pLeftSkill, __FILE__, __LINE__) == 151) // Whirlwind
                    || (pRightSkill && SKILLS_GetSkillIdFromSkill(pRightSkill, __FILE__, __LINE__) == 151))
                {
                    D2Client_PlayUnitBlockOrAttackSwingSound(pUnit);
                }
            }
            else if (pUnit->dwClassId == PCLASS_PALADIN)
            {
                D2Client_PlayUnitBlockOrAttackSwingSound(pUnit);
            }
            break;

        default:
            break;
        }
    }
    else if (dwUnitType == UNIT_MONSTER)
    {
        switch (nAnimMode)
        {
        case MONMODE_DEATH:
        case MONMODE_GETHIT:
        case MONMODE_KNOCKBACK:
            D2Client_PlayUnitGetHitOrDeathSound(pUnit, 0);
            break;

        case MONMODE_BLOCK:
            if (pUnit->dwClassId != 429) // Death Mauler
            {
                D2Client_PlayUnitBlockOrAttackSwingSound(pUnit);
            }
            break;

        case MONMODE_ATTACK1:
        case MONMODE_ATTACK2:
            D2Client_PlayUnitBlockOrAttackSwingSound(pUnit);
            break;

        case MONMODE_SKILL1:
        case MONMODE_SKILL2:
        case MONMODE_SKILL3:
        case MONMODE_SKILL4:
            D2Client_PlayMonsterSkillModeSound(pUnit, nAnimMode - MONMODE_SKILL1);
            break;

        case MONMODE_DEAD:
            D2Client_StopAllUnitLoopingSoundsExcept(pUnit, -1);
            break;

        default:
            break;
        }
    }

    D2Client_UpdateUnitSoundsPerTick(pUnit);
}

// ============================================================================
// PART 2: UnitMode.cpp (0x6FAF0B70 .. 0x6FAF2060)
// ============================================================================

// 1.12a: 0x6FAF0B70 (__fastcall)
void __fastcall D2Client_RecacheUnitPathRoom(D2UnitStrc* pUnit)
{
    PATH_RecacheRoomIfNeeded(pUnit->pDynamicPath);
}

// 1.12a: 0x6FAF0B80 (__fastcall)
int32_t __fastcall D2Client_CheckUnitSkillOrInteractRange(D2UnitStrc* pUnit, D2UnitStrc* pTargetUnit)
{
    if (!pUnit || !pTargetUnit)
    {
        return 0;
    }

    D2SkillStrc* pSkill = (pUnit->dwFlags & 0x80)
        ? UNITS_GetRightSkill(pUnit)
        : UNITS_GetLeftSkill(pUnit);

    int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 918);
    if (nSkillId >= 0
        && nSkillId < sgptDataTables->nSkillsTxtRecordCount
        && sgptDataTables->pSkillsTxt[nSkillId].wSrvDoFunc == 19) // Throw
    {
        return 1;
    }

    if (pTargetUnit->dwUnitType >= UNIT_OBJECT)
    {
        if (!UNITS_IsObjectInInteractRange(pUnit, pTargetUnit))
        {
            return 0;
        }
        return UNITS_TestCollisionBetweenInteractingUnits(pTargetUnit, pUnit, 0x04) == 0;
    }

    return UNITS_IsInMeleeRange(pUnit, pTargetUnit, 0);
}

// 1.12a: 0x6FAF0C30 (__fastcall)
BOOL __fastcall D2Client_CanUnitInterruptCurrentMode(D2UnitStrc* pUnit)
{
    D2SkillStrc* pUsedSkill = UNITS_GetUsedSkill(pUnit);
    if (pUsedSkill)
    {
        int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pUsedSkill, "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 268);
        if (nSkillId >= 0
            && nSkillId < sgptDataTables->nSkillsTxtRecordCount
            && (&sgptDataTables->pSkillsTxt[nSkillId] != nullptr)
            && (sgptDataTables->pSkillsTxt[nSkillId].dwFlags[0] & gdwBitMasks[7]))
        {
            return TRUE;
        }
    }

    if (!pUnit || (pUnit->dwAnimMode != PLRMODE_NEUTRAL && pUnit->dwAnimMode != PLRMODE_TOWNNEUTRAL))
    {
        return FALSE;
    }
    return TRUE;
}

// 1.12a: 0x6FAF0CA0 (__fastcall)
void __fastcall D2Client_SetPathTargetServerUnit(D2DynamicPathStrc* pDynamicPath, int32_t nTargetUnitType, uint32_t dwTargetUnitId)
{
    D2UnitStrc* pTargetUnit = D2Client_FindServerSideUnit(dwTargetUnitId, nTargetUnitType);
    if (!pTargetUnit)
    {
        PATH_SetTargetUnit(pDynamicPath, nullptr);
    }
    else
    {
        PATH_SetTargetUnit(pDynamicPath, pTargetUnit);
        D2Common_10142(pDynamicPath, pDynamicPath->pUnit, 0);
    }
}

// 1.12a: 0x6FAF0CE0 (no args -> eax = nSlackSubtiles)
uint32_t __cdecl D2Client_GetPingSlackSubtiles(void)
{
    return (gdwClientPing_6FBB32BC + 50) >> 7;
}

// 1.12a: 0x6FAF0CF0 (__fastcall)
void __fastcall D2Client_SetPathTargetPosition(D2DynamicPathStrc* pDynamicPath, int32_t nTargetX, int32_t nTargetY)
{
    D2COMMON_10170_PathSetTargetPos(pDynamicPath, nTargetX, nTargetY);
    D2Common_10142(pDynamicPath, pDynamicPath->pUnit, 0);
}

// 1.12a: 0x6FAF0D20 (__fastcall)
BOOL __fastcall D2Client_ValidatePathTargetUnit(D2DynamicPathStrc* pDynamicPath)
{
    int32_t nTargetType = 0;
    uint32_t dwTargetGUID = 0;
    if (!PATH_GetTargetTypeAndGUID(pDynamicPath, &nTargetType, (int*)&dwTargetGUID))
    {
        return TRUE;
    }

    D2UnitStrc* pFoundTarget = D2Client_FindServerSideUnit(dwTargetGUID, nTargetType);
    D2UnitStrc* pCurrentTarget = PATH_GetTargetUnit(pDynamicPath);
    if (pFoundTarget != pCurrentTarget)
    {
        PATH_SetTargetUnit(pDynamicPath, nullptr);
        return FALSE;
    }

    if (pCurrentTarget->dwUnitType == UNIT_PLAYER || pCurrentTarget->dwUnitType == UNIT_MONSTER)
    {
        if (!UNITS_GetRoom(pCurrentTarget))
        {
            PATH_SetTargetUnit(pDynamicPath, nullptr);
            return FALSE;
        }
    }
    return TRUE;
}

// 1.12a: 0x6FAF0DC0 (__fastcall)
void __fastcall D2Client_SendMovementOrActionPacket(D2UnitStrc* pUnit, int32_t nMode, int32_t bTargeted, int32_t nTargetType, uint32_t dwTargetId, int32_t nTargetX, int32_t nTargetY)
{
    if (pUnit != gpPlayerUnit)
    {
        return;
    }

    switch (nMode)
    {
    case PLRMODE_WALK:
    case PLRMODE_TOWNWALK:
        if (bTargeted)
        {
            D2Client_SendPacket02_WalkToEntity_6FB6E730(nTargetType, dwTargetId);
        }
        else
        {
            D2Client_SendPacket01_WalkToLocation_6FB6ED00(nTargetX, nTargetY);
        }
        break;

    case PLRMODE_RUN:
        if (bTargeted)
        {
            D2Client_SendPacket04_RunToEntity_6FB6E770(nTargetType, dwTargetId);
        }
        else
        {
            D2Client_SendPacket03_RunToLocation_6FB6ED30(nTargetX, nTargetY);
        }
        break;

    case PLRMODE_ATTACK1:
    case PLRMODE_ATTACK2:
    case PLRMODE_CAST:
    case PLRMODE_THROW:
    case PLRMODE_KICK:
    case PLRMODE_SKILL1:
    case PLRMODE_SKILL2:
    case PLRMODE_SKILL3:
    case PLRMODE_SKILL4:
    case PLRMODE_SEQUENCE:
        if (bTargeted)
        {
            D2Client_SendTargetedSkillPacket_6FB6E5F0(pUnit, nTargetType, dwTargetId);
        }
        else
        {
            D2Client_SendLocationSkillPacket_6FB6EC60(pUnit, nTargetX, nTargetY);
        }
        break;

    default:
        break;
    }
}

// 1.12a: 0x6FAF0E50 (__fastcall)
void __fastcall D2Client_ChangeUnitAnimModeAndUpdateGfx(D2UnitStrc* pUnit, int32_t nMode)
{
    if (pUnit)
    {
        pUnit->dwFlags &= ~0x00000040; // ~UNITFLAG_SKILLEND
        UNITS_ChangeAnimMode(pUnit, nMode);
        D2Client_UpdateUnitGfxAndAnim_6FB42000(pUnit);
    }
}

// 1.12a: 0x6FAF0E80 (__fastcall)
void __fastcall D2Client_ChangeUnitAnimModeAndUpdateGfx_Alias(D2UnitStrc* pUnit, int32_t nMode)
{
    if (pUnit)
    {
        pUnit->dwFlags &= ~0x00000040; // ~UNITFLAG_SKILLEND
        UNITS_ChangeAnimMode(pUnit, nMode);
        D2Client_UpdateUnitGfxAndAnim_6FB42000(pUnit);
    }
}

// 1.12a: 0x6FAF0EB0 (__fastcall)
BOOL __fastcall D2Client_SyncUnitPosition(D2UnitStrc* pUnit, int32_t nServerX, int32_t nServerY)
{
    if (!pUnit)
    {
        return FALSE;
    }

    int32_t nUnitX = 0;
    int32_t nUnitY = 0;
    if (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER || pUnit->dwUnitType == UNIT_MISSILE)
    {
        nUnitX = pUnit->pDynamicPath ? PATH_GetXPosition(pUnit->pDynamicPath) : 0;
        nUnitY = pUnit->pDynamicPath ? PATH_GetYPosition(pUnit->pDynamicPath) : 0;
    }
    else
    {
        nUnitX = pUnit->pStaticPath ? D2COMMON_10175_PathGetFirstPointX(pUnit->pStaticPath) : 0;
        nUnitY = pUnit->pStaticPath ? D2COMMON_10176_PathGetFirstPointY(pUnit->pStaticPath) : 0;
    }

    int32_t nDistance = D2Common_10398(nUnitX, nUnitY, nServerX, nServerY);
    if (nDistance <= 1)
    {
        return TRUE;
    }

    uint32_t dwPingSlack = (gdwClientPing_6FBB32BC + 50) >> 7;
    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        if (pUnit == gpPlayerUnit)
        {
            switch (pUnit->dwAnimMode)
            {
            case PLRMODE_DEATH:
            case PLRMODE_DEAD:
                return TRUE;

            case PLRMODE_WALK:
            case PLRMODE_TOWNWALK:
                if (nDistance <= (int32_t)(dwPingSlack + 13))
                {
                    return TRUE;
                }
                break;

            case PLRMODE_RUN:
                if (nDistance <= (int32_t)(dwPingSlack + 20))
                {
                    return TRUE;
                }
                break;

            case PLRMODE_GETHIT:
            case PLRMODE_BLOCK:
            case PLRMODE_ATTACK1:
            case PLRMODE_ATTACK2:
            case PLRMODE_CAST:
            case PLRMODE_THROW:
            case PLRMODE_KICK:
            case PLRMODE_SKILL1:
            case PLRMODE_SKILL2:
            case PLRMODE_SKILL3:
            case PLRMODE_SKILL4:
            case PLRMODE_SEQUENCE:
                if (nDistance <= (int32_t)(dwPingSlack + 8))
                {
                    return TRUE;
                }
                break;

            case PLRMODE_KNOCKBACK:
                return FALSE;

            default:
                if (nDistance <= (int32_t)(dwPingSlack + 3))
                {
                    return TRUE;
                }
                break;
            }
        }
        else
        {
            switch (pUnit->dwAnimMode)
            {
            case PLRMODE_DEATH:
            case PLRMODE_DEAD:
                return TRUE;

            case PLRMODE_WALK:
            case PLRMODE_TOWNWALK:
                if (nDistance <= (int32_t)(dwPingSlack + 15))
                {
                    return TRUE;
                }
                break;

            case PLRMODE_RUN:
                if (nDistance <= (int32_t)(dwPingSlack + 20))
                {
                    return TRUE;
                }
                break;

            case PLRMODE_KNOCKBACK:
                return FALSE;

            default:
                if (nDistance <= 10)
                {
                    return TRUE;
                }
                break;
            }
        }
    }
    else if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        switch (pUnit->dwAnimMode)
        {
        case MONMODE_DEATH:
        case MONMODE_DEAD:
            return TRUE;

        case MONMODE_WALK:
            if (nDistance <= (int32_t)(dwPingSlack + 13))
            {
                return TRUE;
            }
            break;

        case MONMODE_RUN:
            if (nDistance <= (int32_t)(dwPingSlack + 18))
            {
                return TRUE;
            }
            break;

        case MONMODE_KNOCKBACK:
            return FALSE;

        default:
            if (nDistance <= 8)
            {
                return TRUE;
            }
            break;
        }
    }

    D2Client_ClearPlayerMapWaypoints_6FAE0490(pUnit);
    D2Common_10212(pUnit->pDynamicPath, pUnit, 0, nServerX, nServerY);
    if (pUnit == gpPlayerUnit)
    {
        D2Client_NotifySoundSystemPlayerTeleported_6FAE1240();
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        switch (pUnit->dwAnimMode)
        {
        case PLRMODE_WALK:
        case PLRMODE_RUN:
        case PLRMODE_TOWNWALK:
            D2Common_10142(pUnit->pDynamicPath, pUnit, 0);
            break;
        default:
            D2Client_ForcePlayerToNeutralMode_6FB09AA0(pUnit);
            break;
        }
    }
    else if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        int32_t nAnimMode = pUnit->dwAnimMode;
        if (nAnimMode == MONMODE_WALK || nAnimMode == MONMODE_RUN)
        {
            D2Common_10142(pUnit->pDynamicPath, pUnit, 0);
        }
        else
        {
            D2Client_DispatchMonsterModeChange_6FAE48D0(pUnit, MONMODE_NEUTRAL);
        }
    }

    return FALSE;
}

// 1.12a: 0x6FAF1160 (__fastcall)
void __fastcall D2Client_MoveUnitToTargetCoords(D2UnitStrc* pUnit, int32_t nTargetX, int32_t nTargetY, BOOL bRun)
{
    if (!pUnit)
    {
        return;
    }

    int32_t nUnitX = 0;
    int32_t nUnitY = 0;
    if (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER || pUnit->dwUnitType == UNIT_MISSILE)
    {
        nUnitX = pUnit->pDynamicPath ? PATH_GetXPosition(pUnit->pDynamicPath) : 0;
        nUnitY = pUnit->pDynamicPath ? PATH_GetYPosition(pUnit->pDynamicPath) : 0;
    }
    else
    {
        nUnitX = pUnit->pStaticPath ? D2COMMON_10175_PathGetFirstPointX(pUnit->pStaticPath) : 0;
        nUnitY = pUnit->pStaticPath ? D2COMMON_10176_PathGetFirstPointY(pUnit->pStaticPath) : 0;
    }

    if (D2Common_10398(nUnitX, nUnitY, nTargetX, nTargetY) > 1)
    {
        if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            if (bRun)
            {
                D2Client_DispatchPlayerModeChange_6FB0A820(pUnit, 23, 0, 0, 0, nTargetX, nTargetY, 0);
            }
            else
            {
                D2Client_DispatchPlayerModeChange_6FB0A820(pUnit, PLRMODE_WALK, 0, 0, 0, nTargetX, nTargetY, 0);
            }
        }
        else if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            if (bRun)
            {
                D2Client_DispatchMonsterMoveOrSkill_6FAE5100(pUnit, 23, 0, 0, nTargetX, nTargetY);
            }
            else
            {
                D2Client_DispatchMonsterMoveOrSkill_6FAE5100(pUnit, MONMODE_WALK, 0, 0, nTargetX, nTargetY);
            }
        }
        else
        {
            FOG_DisplayWarning("0", "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 476);
        }
    }
}

// 1.12a: 0x6FAF1290 (custom: edi = pUnit)
void __fastcall D2Client_ApplyElementalHitOverlay(D2UnitStrc* pUnit)
{
    uint32_t dwElemClass = pUnit->dwLastHitClass & 0xF0;
    if (dwElemClass == 0x20) // Cold
    {
        UNITS_SetOverlay(pUnit, 81, 0);
    }
    else if (dwElemClass == 0x30) // Fire
    {
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2MonStats2Txt* pMonStats2Txt = MONSTERMODE_GetMonStats2Txt(pUnit->dwClassId);
            if (!pMonStats2Txt)
            {
                return;
            }
            if (pMonStats2Txt->nOverlayHeight - 1 > 1)
            {
                UNITS_SetOverlay(pUnit, 35, 0);
            }
            else
            {
                UNITS_SetOverlay(pUnit, 36, 0);
            }
            return;
        }
        UNITS_SetOverlay(pUnit, 36, 0);
    }
    else if (dwElemClass == 0x40) // Lightning
    {
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2MonStats2Txt* pMonStats2Txt = MONSTERMODE_GetMonStats2Txt(pUnit->dwClassId);
            if (!pMonStats2Txt)
            {
                return;
            }
            if (pMonStats2Txt->nOverlayHeight - 1 > 1)
            {
                UNITS_SetOverlay(pUnit, 2, 0);
            }
            else
            {
                UNITS_SetOverlay(pUnit, 1, 0);
            }
            return;
        }
        UNITS_SetOverlay(pUnit, 1, 0);
    }
    else if (dwElemClass == 0x10) // Poison
    {
        UNITS_SetOverlay(pUnit, 54, 0);
    }
    else if (dwElemClass == 0x70) // Stun
    {
        UNITS_SetOverlay(pUnit, 147, 0);
    }
    else if (dwElemClass == 0x80) // Bash
    {
        UNITS_SetOverlay(pUnit, 21, 0);
    }
    else if (dwElemClass == 0x90) // Freeze
    {
        UNITS_SetOverlay(pUnit, 113, 0);
    }
}

// 1.12a: 0x6FAF13C0 (__fastcall)
void __fastcall D2Client_ChangeUnitModeWithHitOverlayAndSound(D2UnitStrc* pUnit, int32_t nMode)
{
    if (pUnit)
    {
        pUnit->dwFlags &= ~0x00000040; // ~UNITFLAG_SKILLEND
        UNITS_ChangeAnimMode(pUnit, nMode);
        D2Client_UpdateUnitGfxAndAnim_6FB42000(pUnit);
    }
    D2Client_ApplyElementalHitOverlay(pUnit);
    D2Client_PlayUnitModeChangeSound(pUnit);
}

// 1.12a: 0x6FAF1400 (__fastcall)
BOOL __fastcall D2Client_ProcessDynamicUnitMovement(D2UnitStrc* pUnit)
{
    if (!pUnit || !D2Client_ValidatePathTargetUnit(pUnit->pDynamicPath))
    {
        return FALSE;
    }

    if (STATES_CheckState(pUnit, STATE_BLAZE))
    {
        D2Client_ProcessUnitBlazeMissileTrail_6FB515F0(pUnit);
    }
    if (STATES_CheckState(pUnit, STATE_INFERNO))
    {
        D2Client_ProcessUnitInfernoState_6FB516D0(pUnit);
    }
    return D2Common_10226(pUnit, 0);
}

// 1.12a: 0x6FAF1450 (__fastcall)
void __fastcall D2Client_DispatchUnitModeChange(D2UnitStrc* pUnit, int32_t nMode)
{
    if (!pUnit)
    {
        return;
    }

    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
        D2Client_DispatchPlayerModeChange_6FB0A820(pUnit, nMode, 0, 0, 0, 0, 0, 0);
        break;
    case UNIT_MONSTER:
        D2Client_DispatchMonsterModeChange_6FAE48D0(pUnit, nMode);
        break;
    case UNIT_OBJECT:
        D2Client_ChangeObjectAnimMode_6FAEB3C0(pUnit, nMode);
        break;
    case UNIT_MISSILE:
        break;
    case UNIT_ITEM:
        D2Client_DispatchItemModeChange_6FB45810(pUnit, nMode);
        break;
    default:
        FOG_DisplayWarning("0", "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 698);
        break;
    }
}

// 1.12a: 0x6FAF14C0 (__fastcall)
void __fastcall D2Client_ExecuteLocalPlayerTargetedAction(int32_t nMode, int32_t nTargetUnitType, uint32_t dwTargetUnitId)
{
    if (!gpPlayerUnit)
    {
        return;
    }

    if (!D2Client_CanUnitInterruptCurrentMode(gpPlayerUnit))
    {
        D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(gpPlayerUnit);
        pPlayerData->dwQueuedActionMode = nMode;               // +0x150
        pPlayerData->dwQueuedTargetType = nTargetUnitType;     // +0x154
        pPlayerData->dwQueuedTargetGUID = dwTargetUnitId;      // +0x158
        pPlayerData->bHasQueuedTargetedAction = 1;             // +0x15C
        pPlayerData->bHasQueuedLocationAction = 0;             // +0x16C
        return;
    }

    if (nTargetUnitType == UNIT_ITEM)
    {
        D2UnitStrc* pItem = D2Client_FindServerSideUnit(dwTargetUnitId, UNIT_ITEM);
        if (!pItem)
        {
            return;
        }
        if (pItem->dwAnimMode != IMODE_ONGROUND && pItem->dwAnimMode != IMODE_DROPPING)
        {
            return;
        }
    }

    D2UnitStrc* pTarget = D2Client_FindServerSideUnit(dwTargetUnitId, nTargetUnitType);
    if (!pTarget)
    {
        return;
    }

    if (nMode == PLRMODE_WALK || nMode == PLRMODE_RUN || nMode == PLRMODE_TOWNWALK || nMode == 23)
    {
        if (D2Client_CheckUnitSkillOrInteractRange(gpPlayerUnit, pTarget))
        {
            D2Client_InteractWithServerUnit(nTargetUnitType, dwTargetUnitId);
            return;
        }

        if (nTargetUnitType == UNIT_PLAYER || nTargetUnitType == UNIT_MONSTER)
        {
            D2SkillStrc* pSkill = (gpPlayerUnit->dwFlags & 0x80)
                ? UNITS_GetRightSkill(gpPlayerUnit)
                : UNITS_GetLeftSkill(gpPlayerUnit);

            int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 991);
            if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
            {
                D2SkillsTxt* pSkillsTxt = &sgptDataTables->pSkillsTxt[nSkillId];
                if (pSkillsTxt && pSkillsTxt->nRange != 1 && pSkillsTxt->nRange != 3)
                {
                    if (nTargetUnitType == UNIT_PLAYER)
                    {
                        D2Client_TryAttackOrCastOnPlayer(UNIT_PLAYER, dwTargetUnitId);
                    }
                    else
                    {
                        D2Client_TryAttackOrCastOnMonster(UNIT_MONSTER, dwTargetUnitId);
                    }
                    return;
                }
            }
        }
        else if (nTargetUnitType == UNIT_OBJECT)
        {
            if (pTarget->pObjectData && pTarget->pObjectData->pObjectTxt->nSubClass == 32) // Waypoint/Portal ranged interact
            {
                D2Client_InteractWithServerUnit(UNIT_OBJECT, dwTargetUnitId);
                return;
            }
        }
    }

    D2Client_DispatchPlayerModeChange_6FB0A820(gpPlayerUnit, nMode, 1, nTargetUnitType, dwTargetUnitId, 0, 0, 1);
}

// 1.12a: 0x6FAF1740 (__fastcall)
void __fastcall D2Client_ExecuteOrQueuePlayerMoveOrInteract(D2UnitStrc* pUnit, int32_t nMode, int32_t nTargetX, int32_t nTargetY)
{
    if (!pUnit)
    {
        return;
    }

    int32_t nTargetType = 0;
    uint32_t dwTargetGUID = 0;
    if (PATH_GetTargetTypeAndGUID(pUnit->pDynamicPath, &nTargetType, (int*)&dwTargetGUID)
        && D2Client_FindServerSideUnit(dwTargetGUID, nTargetType))
    {
        D2Client_ExecuteLocalPlayerTargetedAction(nMode, nTargetType, dwTargetGUID);
        return;
    }

    if (pUnit == gpPlayerUnit && !D2Client_CanUnitInterruptCurrentMode(pUnit))
    {
        D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
        pPlayerData->dwQueuedLocationMode = nMode;     // +0x160
        pPlayerData->dwQueuedLocationX = nTargetX;     // +0x164
        pPlayerData->dwQueuedLocationY = nTargetY;     // +0x168
        pPlayerData->bHasQueuedLocationAction = 1;     // +0x16C
        pPlayerData->bHasQueuedTargetedAction = 0;     // +0x15C
        return;
    }

    if (nMode == PLRMODE_WALK || nMode == PLRMODE_RUN || nMode == PLRMODE_TOWNWALK || nMode == 23)
    {
        int32_t nUnitX = 0;
        int32_t nUnitY = 0;
        if (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER || pUnit->dwUnitType == UNIT_MISSILE)
        {
            nUnitX = pUnit->pDynamicPath ? PATH_GetXPosition(pUnit->pDynamicPath) : 0;
            nUnitY = pUnit->pDynamicPath ? PATH_GetYPosition(pUnit->pDynamicPath) : 0;
        }
        else
        {
            nUnitX = pUnit->pStaticPath ? D2COMMON_10175_PathGetFirstPointX(pUnit->pStaticPath) : 0;
            nUnitY = pUnit->pStaticPath ? D2COMMON_10176_PathGetFirstPointY(pUnit->pStaticPath) : 0;
        }

        if (nUnitX == nTargetX && nUnitY == nTargetY)
        {
            return;
        }

        int32_t nDistance = D2Common_10398(nUnitX, nUnitY, nTargetX, nTargetY);
        if (nDistance <= 1 && !D2Client_IsPlayerMouseButtonHeld_6FB08BF0())
        {
            return;
        }
        if (nDistance > 85)
        {
            return;
        }
    }

    D2Client_DispatchPlayerModeChange_6FB0A820(pUnit, nMode, 0, 0, 0, nTargetX, nTargetY, 1);
}

// 1.12a: 0x6FAF1910 (__fastcall)
void __fastcall D2Client_InteractWithServerUnit(int32_t nTargetUnitType, uint32_t dwTargetUnitId)
{
    D2UnitStrc* pTarget = D2Client_FindServerSideUnit(dwTargetUnitId, nTargetUnitType);
    if (!pTarget)
    {
        return;
    }

    switch (nTargetUnitType)
    {
    case UNIT_PLAYER:
        D2Client_TryAttackOrCastOnPlayer(UNIT_PLAYER, dwTargetUnitId);
        return;

    case UNIT_MONSTER:
        D2Client_TryAttackOrCastOnMonster(UNIT_MONSTER, dwTargetUnitId);
        return;

    case UNIT_OBJECT:
        if (pTarget->dwClassId == 404) // CompellingOrb
        {
            D2UnitStrc* pRightArm = INVENTORY_GetItemFromBodyLoc(gpPlayerUnit->pInventory, BODYLOC_RARM);
            if (!pRightArm || ITEMS_GetItemCode(pRightArm) != ' 2fq') // 'qf2 ' Khalim's Will
            {
                D2UnitStrc* pLeftArm = INVENTORY_GetItemFromBodyLoc(gpPlayerUnit->pInventory, BODYLOC_LARM);
                if (!pLeftArm || ITEMS_GetItemCode(pLeftArm) != ' 2fq')
                {
                    break;
                }
            }
            uint32_t dwSavedFlags = gpPlayerUnit->dwFlags;
            gpPlayerUnit->dwFlags &= ~0x80;
            D2SkillStrc* pSkill = UNITS_GetLeftSkill(gpPlayerUnit);
            int32_t nSavedSkillId = SKILLS_GetSkillIdFromSkill(pSkill, "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 815);
            D2Client_SetUnitSkillId_6FB51590(pSkill, 0); // Attack
            D2Client_ExecuteLocalPlayerTargetedAction(PLRMODE_ATTACK1, UNIT_OBJECT, dwTargetUnitId);
            D2Client_SetUnitSkillId_6FB51590(pSkill, nSavedSkillId);
            gpPlayerUnit->dwFlags = dwSavedFlags;
            return;
        }
        else if (pTarget->dwClassId == 376) // Hellforge
        {
            if (pTarget->dwAnimMode == OBJMODE_OPERATING)
            {
                D2UnitStrc* pRightArm = INVENTORY_GetItemFromBodyLoc(gpPlayerUnit->pInventory, BODYLOC_RARM);
                if (!pRightArm || ITEMS_GetItemCode(pRightArm) != ' hfh') // 'hfh ' Hellforge Hammer
                {
                    D2UnitStrc* pLeftArm = INVENTORY_GetItemFromBodyLoc(gpPlayerUnit->pInventory, BODYLOC_LARM);
                    if (!pLeftArm || ITEMS_GetItemCode(pLeftArm) != ' hfh')
                    {
                        break;
                    }
                }
                uint32_t dwSavedFlags = gpPlayerUnit->dwFlags;
                gpPlayerUnit->dwFlags &= ~0x80;
                D2SkillStrc* pSkill = UNITS_GetLeftSkill(gpPlayerUnit);
                int32_t nSavedSkillId = SKILLS_GetSkillIdFromSkill(pSkill, "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 832);
                D2Client_SetUnitSkillId_6FB51590(pSkill, 0); // Attack
                D2Client_ExecuteLocalPlayerTargetedAction(PLRMODE_ATTACK1, UNIT_OBJECT, dwTargetUnitId);
                D2Client_SetUnitSkillId_6FB51590(pSkill, nSavedSkillId);
                gpPlayerUnit->dwFlags = dwSavedFlags;
                return;
            }
        }
        break;

    default:
        break;
    }

    D2Client_DispatchPlayerModeChange_6FB0A820(gpPlayerUnit, PLRMODE_NEUTRAL, 0, 0, 0, 0, 0, 1);

    switch (nTargetUnitType)
    {
    case UNIT_OBJECT:
        D2Client_TryInteractWithObject(UNIT_OBJECT, dwTargetUnitId);
        break;
    case UNIT_ITEM:
        D2Client_TryPickUpGroundItem_6FB0A2D0(UNIT_ITEM, dwTargetUnitId);
        break;
    case UNIT_TILE:
        D2Client_TryTakeWarpTile_6FB09140(UNIT_TILE, dwTargetUnitId);
        break;
    default:
        FOG_DisplayWarning("0", "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 870);
        break;
    }
}

// 1.12a: 0x6FAF1B30 (__fastcall)
void __fastcall D2Client_TryInteractWithObject(int32_t nTargetUnitType, uint32_t dwTargetUnitId)
{
    D2UnitStrc* pObject = D2Client_FindServerSideUnit(dwTargetUnitId, nTargetUnitType);
    if (!pObject)
    {
        return;
    }

    D2SkillStrc* pSkill = (gpPlayerUnit->dwFlags & 0x80)
        ? UNITS_GetRightSkill(gpPlayerUnit)
        : UNITS_GetLeftSkill(gpPlayerUnit);

    if (SKILLS_GetSkillIdFromSkill(pSkill, "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 1106) == 43) // Telekinesis
    {
        D2Client_ExecuteLocalPlayerTargetedAction(PLRMODE_CAST, nTargetUnitType, dwTargetUnitId);
    }
    else
    {
        D2Client_OperateObjectClientSide(gpPlayerUnit, pObject);
    }
}

// 1.12a: 0x6FAF1BA0 (__fastcall)
void __fastcall D2Client_TryAttackOrCastOnMonster(int32_t nTargetUnitType, uint32_t dwTargetUnitId)
{
    D2UnitStrc* pMonster = D2Client_FindServerSideUnit(dwTargetUnitId, nTargetUnitType);
    if (!pMonster)
    {
        return;
    }

    if (D2Common_11017_CheckUnitIfConsumeable(pMonster, 0))
    {
        D2Client_DispatchPlayerModeChange_6FB0A820(gpPlayerUnit, PLRMODE_NEUTRAL, 0, 0, 0, 0, 0, 1);
        D2Client_InteractWithNPC_6FAE30C0(pMonster);
        return;
    }

    if (MONSTERS_IsDead(pMonster))
    {
        D2SkillStrc* pSkill = (gpPlayerUnit->dwFlags & 0x80)
            ? UNITS_GetRightSkill(gpPlayerUnit)
            : UNITS_GetLeftSkill(gpPlayerUnit);

        int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 1134);
        if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
        {
            D2SkillsTxt* pSkillsTxt = &sgptDataTables->pSkillsTxt[nSkillId];
            if (pSkillsTxt && (pSkillsTxt->dwFlags[0] & gdwBitMasks[10])) // SKILLFLAG_CORPSE
            {
                int32_t nActionMode = D2Client_GetPlayerSkillActionMode_6FB0B5B0(gpPlayerUnit);
                D2Client_ExecuteLocalPlayerTargetedAction(nActionMode, nTargetUnitType, dwTargetUnitId);
            }
        }
        return;
    }

    if (STATLIST_GetUnitAlignment(pMonster) == UNIT_ALIGNMENT_GOOD)
    {
        if (D2Client_IsInTownOrTownSkillRestricted_6FB0B900(gpPlayerUnit, 0))
        {
            D2Client_DispatchPlayerModeChange_6FB0A820(gpPlayerUnit, PLRMODE_NEUTRAL, 0, 0, 0, 0, 0, 1);
            D2Client_InteractWithNPC_6FAE30C0(pMonster);
        }
        return;
    }

    int32_t nActionMode = D2Client_GetPlayerSkillActionMode_6FB0B5B0(gpPlayerUnit);
    D2Client_ExecuteLocalPlayerTargetedAction(nActionMode, nTargetUnitType, dwTargetUnitId);
}

// 1.12a: 0x6FAF1CB0 (__fastcall)
void __fastcall D2Client_TryAttackOrCastOnPlayer(int32_t nTargetUnitType, uint32_t dwTargetUnitId)
{
    D2UnitStrc* pTargetPlayer = D2Client_FindServerSideUnit(dwTargetUnitId, nTargetUnitType);
    if (!pTargetPlayer)
    {
        return;
    }

    if (pTargetPlayer->dwAnimMode == PLRMODE_DEATH)
    {
        return;
    }

    if (pTargetPlayer->dwAnimMode == PLRMODE_DEAD)
    {
        if (dwTargetUnitId == gpPlayerUnit->dwUnitId)
        {
            D2Client_SendPacket13_UnitInteract_6FB6E7B0(UNIT_PLAYER, dwTargetUnitId);
            return;
        }

        if (D2Client_CheckPlayerRosterFlag_6FAE0830(gpPlayerUnit->dwUnitId, dwTargetUnitId, 4)) // Loot permission
        {
            D2Client_OpenCorpseInventory_6FB08730(pTargetPlayer);
            return;
        }
    }

    if (D2Client_AreUnitsHostile_6FB08B60(gpPlayerUnit, pTargetPlayer))
    {
        int32_t nActionMode = D2Client_GetPlayerSkillActionMode_6FB0B5B0(gpPlayerUnit);
        D2Client_ExecuteLocalPlayerTargetedAction(nActionMode, nTargetUnitType, dwTargetUnitId);
        return;
    }

    if (!D2Client_TryCastFriendlyTargetedSkill_6FB0BE20(gpPlayerUnit, UNIT_PLAYER, dwTargetUnitId))
    {
        D2RoomStrc* pRoom = UNITS_GetRoom(gpPlayerUnit);
        if (pRoom && DUNGEON_IsRoomInTown(pRoom))
        {
            D2Client_SendPacket13_UnitInteract_6FB6E7B0(UNIT_PLAYER, dwTargetUnitId);
        }
    }
}

// 1.12a: 0x6FAF1D90 (__fastcall)
void __fastcall D2Client_ProcessQueuedPlayerAction(D2UnitStrc* pUnit)
{
    if (pUnit != gpPlayerUnit)
    {
        return;
    }

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
    if (pPlayerData->bHasQueuedTargetedAction)
    {
        pPlayerData->bHasQueuedTargetedAction = 0;
        D2UnitStrc* pTarget = D2Client_FindServerSideUnit(pPlayerData->dwQueuedTargetGUID, pPlayerData->dwQueuedTargetType);
        if (pTarget && (pTarget->dwFlags & UNITFLAG_TARGETABLE))
        {
            D2Client_ExecuteLocalPlayerTargetedAction(
                pPlayerData->dwQueuedActionMode,
                pPlayerData->dwQueuedTargetType,
                pPlayerData->dwQueuedTargetGUID);
            return;
        }

        if (pPlayerData->dwQueuedTargetType == UNIT_PLAYER || pPlayerData->dwQueuedTargetType == UNIT_MONSTER)
        {
            int32_t nMouseX = D2Client_GetMouseX_6FB55BC0();
            int32_t nMouseY = D2Client_GetMouseY_6FB55BD0();
            int32_t nWorldX = 0;
            int32_t nWorldY = 0;
            D2Client_ScreenToWorldCoords_6FB23AD0(nMouseX, nMouseY, &nWorldX, &nWorldY);

            BOOL bRun = D2Client_ShouldPlayerRun_6FB08840();
            D2Client_ExecuteOrQueuePlayerMoveOrInteract(pUnit, bRun ? PLRMODE_RUN : PLRMODE_WALK, nWorldX, nWorldY);
        }
    }
    else if (pPlayerData->bHasQueuedLocationAction)
    {
        pPlayerData->bHasQueuedLocationAction = 0;
        D2Client_ExecuteOrQueuePlayerMoveOrInteract(
            pUnit,
            pPlayerData->dwQueuedLocationMode,
            pPlayerData->dwQueuedLocationX,
            pPlayerData->dwQueuedLocationY);
    }
}

// 1.12a: 0x6FAF1EE0 (__fastcall)
BOOL __fastcall D2Client_UpdateSingleUnitPerTick(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return FALSE;
    }

    if (pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000))
    {
        D2StatListStrc* pStateList = STATLIST_GetStatListFromUnitStateAndFlag(pUnit, 0, 0x40);
        while (pStateList)
        {
            D2StatListStrc* pNext = STATLIST_GetNextStatList(pStateList);
            int32_t nExpireFrame = D2Common_10472_GetStatListExpireFrame(pStateList);
            if (nExpireFrame && nExpireFrame <= (int32_t)pUnit->dwCorrectFrame)
            {
                int32_t nState = STATLIST_GetState(pStateList);
                D2Common_10474(pUnit, pStateList);
                STATES_ToggleState(pUnit, nState, FALSE);
                D2Client_ApplyUnitStateRemovedClientSide_6FACEBC0(pUnit, nState);
            }
            pStateList = pNext;
        }
    }

    if (pUnit->dwFlags & 0x00020000) // UNITFLAG_HASOVERLAYS
    {
        UNITS_UpdateOverlays(pUnit);
    }

    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
        D2Client_UpdatePlayerUnitPerTick_6FB0D140(pUnit);
        break;
    case UNIT_MONSTER:
        D2Client_UpdateMonsterUnitPerTick_6FAE6100(pUnit);
        break;
    case UNIT_OBJECT:
        D2Client_UpdateObjectPerTick(pUnit);
        break;
    case UNIT_MISSILE:
        D2Client_UpdateMissileUnitPerTick_6FAD3520(pUnit);
        break;
    case UNIT_ITEM:
        D2Client_UpdateItemUnitPerTick_6FB468F0(pUnit);
        break;
    case UNIT_TILE:
        break;
    default:
        FOG_DisplayWarning("0", "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 1044);
        break;
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        int32_t nAnimMode = pUnit->dwAnimMode;
        if (nAnimMode == PLRMODE_WALK || nAnimMode == PLRMODE_RUN || nAnimMode == PLRMODE_TOWNWALK || nAnimMode == PLRMODE_KNOCKBACK)
        {
            if (pUnit->pDynamicPath && !PATH_GetNumberOfPathPoints(pUnit->pDynamicPath))
            {
                D2Client_ForcePlayerToNeutralMode_6FB09AA0(pUnit);
            }
        }
        else if (pUnit->dwFlags & 0x00000040) // UNITFLAG_SKILLEND
        {
            D2Client_OnPlayerAnimSequenceFinished_6FB0CF70(pUnit);
        }
        return TRUE;
    }

    if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        int32_t nAnimMode = pUnit->dwAnimMode;
        if (nAnimMode == MONMODE_WALK || nAnimMode == MONMODE_RUN || nAnimMode == MONMODE_KNOCKBACK)
        {
            if (pUnit->pDynamicPath && !PATH_GetNumberOfPathPoints(pUnit->pDynamicPath))
            {
                D2Client_DispatchMonsterModeChange_6FAE48D0(pUnit, MONMODE_NEUTRAL);
            }
        }
        else if (pUnit->dwFlags & 0x00000040) // UNITFLAG_SKILLEND
        {
            D2Client_OnMonsterAnimSequenceFinished_6FAE5D10(pUnit);
        }
        return TRUE;
    }

    if (pUnit->dwFlags & 0x00000040) // UNITFLAG_SKILLEND
    {
        switch (pUnit->dwUnitType)
        {
        case UNIT_OBJECT:
            D2Client_OnObjectAnimEnd(pUnit);
            break;
        case UNIT_MISSILE:
            return D2Client_OnMissileAnimSequenceFinished_6FAD2F50(pUnit);
        case UNIT_ITEM:
            D2Client_OnItemAnimSequenceFinished_6FB450B0(pUnit);
            break;
        default:
            FOG_DisplayWarning("0", "..\\Source\\D2Client\\Unit\\UnitMode.cpp", 1084);
            break;
        }
    }

    return TRUE;
}
