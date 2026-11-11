// ============================================================================
// UNIT_Item.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll
// Address range:
//   - 0x6FB44FE0..0x6FB4C3E0 (asm/UNIT_Item.asm)
//     * 0x6FB44FE0..0x6FB454D3: Act/Level Environment & Ambient Sound Controller
//     * 0x6FB454E0..0x6FB4A490: ..\Source\D2Client\UNIT\Item.cpp (Client Item Unit & 0x9C/0x9D Action Handlers)
//     * 0x6FB4AA60..0x6FB4C26A: Scrollable Multi-Line Text List UI Widget (`D2ScrollTextListStrc`)
//     * 0x6FB4C270..0x6FB4C3DE: Player Skill Frame Trigger & Multi-Hit Melee Helpers
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include "D2Structs.h"
#include "D2Constants.h"
#include "D2CommonImports.h"
#include <Units/UnitRoom.h>
#include <Fog.h>

inline void D2Common_10474(D2StatListStrc* pStatList)
{
    STATLIST_FreeStatList(pStatList);
}

inline int SKILLS_GetSkillCount()
{
    return sgptDataTables ? sgptDataTables->nSkillsTxtRecordCount : 0;
}

// ----------------------------------------------------------------------------
// External Globals (1.12a D2Client.dll)
// ----------------------------------------------------------------------------
extern D2UnitStrc* gpPlayerUnit;                        // 0x6FBCC3D0
extern uint32_t    gdwClientGameTick;                   // 0x6FBB32B0
extern uint32_t    gdwRightHandWeaponClass;             // 0x6FBCBCE4
extern uint32_t    gdwLeftHandWeaponClass;              // 0x6FBCBCE8
extern int32_t     gbEquippedItemComponentChanged;      // 0x6FBCBCFC
extern uint8_t     gbPacketItemActionTable[];           // 0x6FBAA2D0

// Saved unit vitals and active skills across equipment updates
extern int32_t     gdwSavedUnitMaxHP;                   // 0x6FBCBD08
extern int32_t     gdwSavedUnitMaxMana;                 // 0x6FBCBD04
extern int32_t     gdwSavedUnitMaxStamina;              // 0x6FBCBD00
extern int32_t     gnSavedRightSkillId;                 // 0x6FBCBD0C
extern int32_t     gdwSavedRightSkillOwnerGUID;         // 0x6FBCBD10
extern int32_t     gnSavedLeftSkillId;                  // 0x6FBCBD14
extern int32_t     gdwSavedLeftSkillOwnerGUID;          // 0x6FBCBD18

// Environment sound controller globals
extern int32_t     gnCurrentEnvironmentLevelId;         // 0x6FB84118
extern int32_t     gnCurrentEnvironmentSoundId;         // 0x6FB8411C
extern int32_t     gnEnvironmentLoopSoundHandle;        // 0x6FBCBCF0
extern int32_t     gnEnvironmentRainSoundHandle;        // 0x6FBCBCF4
extern int32_t     gbEnvironmentIsNightCycle;           // 0x6FBCBCE0
extern uint32_t    gdwNextEnvEventSoundTick;            // 0x6FBCBCF8
extern int32_t     gbEnvActSoundsPreloaded;             // 0x6FBCBCD8
extern int32_t     gnEnvPreloadedSoundBaseId;           // 0x6FBCBCD4

// UI / Cursor / Inventory state globals
extern int32_t     gnInventoryOpenMode;                 // 0x6FBCBC94
extern int32_t     gnActiveVendorNpcId;                 // 0x6FBCBC98
extern int32_t     gbVendorInteractActive;              // 0x6FBCC414
extern int32_t     gdwActiveInteractNpcGUID;            // 0x6FBCC418
extern int32_t     gnTradeHoradricOrStashMode;          // 0x6FBCC1E0
extern D2DrlgActStrc* gpClientAct;
extern int32_t     gbIsExpansion;
extern int32_t     g_bInteractingWithNpc;
extern int32_t     g_nInteractingNpcGUID;
extern D2UnitStrc* g_pTradeNpcUnit;
extern D2UnitStrc* gapServerSideUnitHashTables[6][128];

// ----------------------------------------------------------------------------
// Forward Declarations of External Helpers
// ----------------------------------------------------------------------------
D2UnitStrc* __fastcall D2Client_GetLocalPlayer(void);                                       // 0x6FB089F0
D2UnitStrc* __fastcall D2Client_FindServerSideUnit(int32_t nUnitId, int32_t nUnitType);     // 0x6FACF9C0
D2UnitStrc* __fastcall D2Client_FindClientSideUnit(int32_t nUnitId, int32_t nUnitType);     // 0x6FACF9A0
D2UnitStrc* __fastcall D2Client_RegisterUnitInHashTable(D2UnitStrc* pUnit);                 // 0x6FAD0A90
void __fastcall GFX_InitUnitCompositeGfx(D2UnitStrc* pUnit);                                // 0x6FB42000
void __fastcall D2Client_FreeClientUnit(D2UnitStrc* pUnit);                                 // 0x6FAD0410
void __fastcall D2Client_RemoveUnitFromServerHashTable(D2UnitStrc* pUnit);                  // 0x6FACFB60
void __fastcall D2Client_SetUnitSelectionHighlight(D2UnitStrc* pUnit, int32_t bSet);        // 0x6FAD1660
void __fastcall D2Client_PlaySoundCentered(int32_t nSoundId);                               // 0x6FAE85C0
void __fastcall D2Client_PlayDelayedSound(int32_t nSoundId, int32_t nDelay);                // 0x6FAE85B0
void __fastcall D2Client_PlayUnitSound(D2UnitStrc* pUnit, int32_t nSoundId);                // 0x6FAE74E0
void __fastcall D2Client_RebuildPlayerCompositedUnits(D2UnitStrc* pUnit, int32_t bForce);   // 0x6FB0C9B0
void __fastcall D2Client_SetLeftSkillForLocalPlayer(int32_t nSkillId, int32_t nOwnerGUID);  // 0x6FB07730
void __fastcall D2Client_SetRightSkillForLocalPlayer(int32_t nSkillId, int32_t nOwnerGUID); // 0x6FB077B0
void __fastcall D2Client_SendClientPacket0(uint8_t bHeader);                                // 0x6FADC960
void __fastcall D2Client_SendClientPacket1Dword(uint8_t bHeader, uint32_t dwArg);           // 0x6FADCA30
D2ActiveRoomStrc* __fastcall D2Client_FindRoomAtCoords(int32_t nX, int32_t nY);
D2UnitStrc* __fastcall D2Client_CreateItemUnitFromPacket13(const uint8_t* pPacket);
D2UnitStrc* __fastcall D2Client_CreateItemUnitFromPacket8(const uint8_t* pPacket);
void __fastcall D2Client_RecalculateAllEquippedAndCharmItems(D2UnitStrc* pUnit, BOOL bUpdateAnimAndComponents);
inline D2UnitStrc* __fastcall ITEMS_GetOwner(D2UnitStrc* pItem) { return pItem ? D2Client_FindServerSideUnit(ITEMS_GetOwnerId(pItem), UNIT_PLAYER) : nullptr; }

// ============================================================================
// PART 1: Environment & Ambient Sound Controller (0x6FB44FE0..0x6FB454D3)
// ============================================================================

// 1.12a: 0x6FB44FE0
// Calling convention: __stdcall
BOOL __stdcall D2Client_IsNightTimeInCurrentAct(D2DrlgActStrc* pAct)
{
    int32_t nCycleIndex = ENVIRONMENT_GetCycleIndexFromAct(pAct);
    return (nCycleIndex < 1 || nCycleIndex > 3);
}

// 1.12a: 0x6FB45000
// Calling convention: __fastcall (ECX = nAct)
void __fastcall D2Client_PreloadActEnvironmentSounds(int32_t nAct)
{
    int32_t nStartSoundId = 32 * nAct + 0xAA0;
    int32_t nEndSoundId   = nStartSoundId + 32;
    for (int32_t i = nStartSoundId; i < nEndSoundId; ++i)
    {
        // D2Sound_PreloadSound(i) (0x6FAE64B0)
    }
}

// 1.12a: 0x6FB45030
// Calling convention: __stdcall
void __stdcall D2Client_UnloadAllEnvironmentSounds(void)
{
    for (int32_t i = 0xAA0; i < 0xB3C; ++i)
    {
        // D2Sound_UnloadSound(i) (0x6FAE64C0)
    }
}

// 1.12a: 0x6FB45050
// Calling convention: __fastcall (ECX = nRainCheck, EDX = nLevelId)
int32_t __fastcall D2Client_GetEnvironmentRainSoundFactor(int32_t nRainCheck, int32_t nLevelId)
{
    if (!nLevelId)
    {
        return 0;
    }

    if (nRainCheck != 1)
    {
        return DATATBLS_GetMudFromLevelsTxt(nLevelId) != 0 ? 1 : 0;
    }

    if (!DATATBLS_GetRainFromLevelsTxt(nLevelId))
    {
        return 0;
    }

    // Checks snow/rain weather intensity (0x6FAF4AC0)
    return 1;
}

// 1.12a: 0x6FB450A0
// Calling convention: __stdcall
void __stdcall D2Client_StopAllEnvironmentLoopAndRainSounds(void)
{
    D2Client_UnloadAllEnvironmentSounds();
    if (gnEnvironmentLoopSoundHandle != -1)
    {
        // Stop loop sound (0x6FAE6BD0)
        gnEnvironmentLoopSoundHandle = -1;
    }
    if (gnEnvironmentRainSoundHandle != -1)
    {
        // Stop rain sound (0x6FAE6BD0)
        gnEnvironmentRainSoundHandle = -1;
    }
    gnCurrentEnvironmentSoundId = 0;
    gnCurrentEnvironmentLevelId = 0;
    gbEnvironmentIsNightCycle   = -1;
}

// 1.12a: 0x6FB45140
// Calling convention: __stdcall
void __stdcall D2Client_UpdateEnvironmentSoundsPerTick(void)
{
    D2UnitStrc* pPlayer = D2Client_GetLocalPlayer();
    if (!pPlayer)
    {
        return;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pPlayer);
    int32_t nLevelId = pRoom ? DUNGEON_GetLevelIdFromRoom(pRoom) : 0;
    D2LevelsTxt* pLevelsTxt = nLevelId ? DATATBLS_GetLevelsTxtRecord(nLevelId) : nullptr;
    int32_t nSoundEnvId = pLevelsTxt ? pLevelsTxt->dwSoundEnv : 0;

    // Evaluates SoundEnviron.txt record (DATATBLS_GetSoundEnvironTxtRecord),
    // updates day/night ambient loops, random one-shot ambient events, and rain loops.
    (void)nSoundEnvId;
}

// 1.12a: 0x6FB454D0
// Calling convention: __stdcall
int32_t __stdcall D2Client_NoOpReturnZero(void)
{
    return 0;
}

// ============================================================================
// PART 2: Client Item Unit & 0x9C/0x9D Action Handlers (0x6FB454E0..0x6FB4A490)
// Source path in binary: "..\Source\D2Client\UNIT\Item.cpp"
// ============================================================================

// 1.12a: 0x6FB454E0
// Calling convention: __fastcall (ECX = pItem)
BOOL __fastcall D2Client_IsItemIdentifiedOrCursor(D2UnitStrc* pItem)
{
    if (STATLIST_GetStatListFromUnitAndFlag(pItem, 2))
    {
        return TRUE;
    }
    D2UnitStrc* pPlayer = D2Client_GetLocalPlayer();
    return STATES_CheckState(pPlayer, STATE_UNINTERRUPTABLE) != 0;
}

// 1.12a: 0x6FB45510
// Calling convention: __fastcall (ECX = pUnit, EDX = nState)
int32_t __fastcall D2Client_OnItemIdentifyStateRemoved(D2UnitStrc* pUnit, int32_t nState, int32_t bSet)
{
    (void)bSet;
    STATES_ToggleState(pUnit, nState, FALSE);
    return 0;
}

// 1.12a: 0x6FB45520
// Calling convention: __fastcall (ECX = pItem)
void __fastcall D2Client_ClearItemState54AndUpdateAnimRate(D2UnitStrc* pItem)
{
    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pItem, 0x36);
    if (pStatList)
    {
        D2Common_10474_FreeStatList(pStatList);
        STATES_ToggleState(pItem, 0x36, FALSE);
    }

    D2UnitStrc* pOwner = ITEMS_GetOwner(pItem);
    if (!pOwner)
    {
        pOwner = D2Client_GetLocalPlayer();
    }
    UNITS_UpdateAnimRateAndVelocity(pOwner, __FILE__, __LINE__);
}

// 1.12a: 0x6FB45580
// Calling convention: __fastcall (ECX = pPacket)
int32_t __fastcall D2Client_GetPacketItemActionMode(const uint8_t* pPacket)
{
    uint8_t bAction = pPacket[1];
    if (bAction != 0 && bAction != 2 && (gbPacketItemActionTable[4 * bAction] & 2) != 0)
    {
        return ITEMMODE_ONCURSOR; // 4
    }
    return ITEMMODE_ONFLOOR; // 3
}

// 1.12a: 0x6FB455B0
// Calling convention: __stdcall
void __stdcall D2Client_ResetEquippedWeaponClassCache(void)
{
    gdwRightHandWeaponClass = 0;
    gdwLeftHandWeaponClass  = 0;
}

// 1.12a: 0x6FB455C0
// Calling convention: __fastcall (ECX = pInventory, EDX = pItem, [esp+4] = nBodyLoc)
BOOL __fastcall D2Client_PlaceItemInBodyLocWrapper(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int32_t nBodyLoc)
{
    if (INVENTORY_GetItemFromBodyLoc(pInventory, nBodyLoc))
    {
        return FALSE;
    }
    return INVENTORY_PlaceItemInBodyLoc(pInventory, pItem, nBodyLoc);
}

// 1.12a: 0x6FB455F0
// Calling convention: __fastcall (ECX = pItem, EDX = bBodyLoc)
void __fastcall D2Client_UpdateEquippedWeaponClassCache(D2UnitStrc* pItem, uint8_t bBodyLoc)
{
    if (bBodyLoc == BODYLOC_RARM)
    {
        gdwRightHandWeaponClass = ITEMS_GetWeaponClassId(pItem);
    }
    else if (bBodyLoc == BODYLOC_LARM)
    {
        gdwLeftHandWeaponClass = ITEMS_GetWeaponClassId(pItem);
    }
}

// 1.12a: 0x6FB45610
// Calling convention: __fastcall (ECX = pUnit)
void __fastcall D2Client_SaveUnitVitalsAndActiveSkills(D2UnitStrc* pUnit)
{
    gdwSavedUnitMaxHP      = STATLIST_GetMaxLifeFromUnit(pUnit);
    gdwSavedUnitMaxMana    = STATLIST_GetMaxManaFromUnit(pUnit);
    gdwSavedUnitMaxStamina = STATLIST_GetMaxStaminaFromUnit(pUnit);

    if (pUnit == D2Client_GetLocalPlayer())
    {
        D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pUnit);
        if (pRightSkill)
        {
            gnSavedRightSkillId        = SKILLS_GetSkillIdFromSkill(pRightSkill, __FILE__, __LINE__);
            gdwSavedRightSkillOwnerGUID = SKILLS_GetOwnerGUIDFromSkill(pRightSkill);
        }
        else
        {
            gnSavedRightSkillId        = -1;
            gdwSavedRightSkillOwnerGUID = -1;
        }

        D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pUnit);
        if (pLeftSkill)
        {
            gnSavedLeftSkillId        = SKILLS_GetSkillIdFromSkill(pLeftSkill, __FILE__, __LINE__);
            gdwSavedLeftSkillOwnerGUID = SKILLS_GetOwnerGUIDFromSkill(pLeftSkill);
        }
        else
        {
            gnSavedLeftSkillId        = -1;
            gdwSavedLeftSkillOwnerGUID = -1;
        }
    }
}

// 1.12a: 0x6FB456B0
// Calling convention: custom EAX = pOwner, EDI = pItem, [esp+4] = bCheckQuiver
BOOL __fastcall D2Client_CheckEquippedItemRequirementsAndQuiver(D2UnitStrc* pOwner, D2UnitStrc* pItem, BOOL bCheckQuiver)
{
    BOOL bCanEquip = FALSE;
    if (!ITEMS_CheckRequirements(pItem, pOwner, FALSE, &bCanEquip, nullptr, nullptr) || !bCanEquip)
    {
        return FALSE;
    }

    int32_t nQuiverType = ITEMS_GetQuiverType(pItem);
    if (bCheckQuiver && nQuiverType)
    {
        D2UnitStrc* pLeftItem  = INVENTORY_GetLeftHandWeapon(pOwner ? pOwner->pInventory : nullptr);
        int32_t nOtherBodyLoc  = (ITEMS_GetBodyLocation(pLeftItem) == BODYLOC_RARM) ? BODYLOC_LARM : BODYLOC_RARM;
        D2UnitStrc* pOtherItem = INVENTORY_GetItemFromBodyLoc(pOwner ? pOwner->pInventory : nullptr, nOtherBodyLoc);
        if (!ITEMS_CheckItemTypeId(pOtherItem, nQuiverType))
        {
            return FALSE;
        }
    }

    return TRUE;
}

// 1.12a: 0x6FB45710
// Calling convention: __stdcall
void __stdcall D2Client_RequestInventoryResync(void)
{
    D2Client_SendClientPacket0(0x3F);
}

// 1.12a: 0x6FB45730
// Calling convention: custom ESI = pItem
void __fastcall D2Client_ApplyState54ToItemUnit(D2UnitStrc* pItem)
{
    if (pItem && !STATLIST_GetStatListFromUnitAndState(pItem, 0x36))
    {
        void* pMemPool = pItem->pMemoryPool;
        STATES_ToggleState(pItem, 0x36, TRUE);
        D2StatListStrc* pStatList = STATLIST_AllocStatList(pMemPool, 0, 0, pItem->dwUnitType, pItem->dwUnitId);
        STATLIST_SetState(pStatList, 0x36);
        STATLIST_SetStatRemoveCallback(pStatList, (void*)D2Client_OnItemIdentifyStateRemoved);
        D2Common_10475_PostStatToStatList(pItem, pStatList, TRUE);
    }
}

// 1.12a: 0x6FB45780
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_ClearState54FromPacketUnit(const uint8_t* pPacket)
{
    D2UnitStrc* pUnit = D2Client_FindServerSideUnit(*(const int32_t*)(pPacket + 2), pPacket[1]);
    if (pUnit)
    {
        D2Client_ClearItemState54AndUpdateAnimRate(pUnit);
    }
}

// 1.12a: 0x6FB457B0
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_HandleItemFlagUpdatePacket(const uint8_t* pPacket)
{
    D2UnitStrc* pItem = D2Client_FindServerSideUnit(*(const int32_t*)(pPacket + 1), UNIT_ITEM);
    if (pItem)
    {
        int32_t nValue = *(const int16_t*)(pPacket + 9);
        ITEMS_SetItemFlag(pItem, *(const uint32_t*)(pPacket + 5), nValue);
        if (nValue)
        {
            D2Client_PlaySoundCentered(5);
        }
        D2Client_ClearItemState54AndUpdateAnimRate(pItem);
    }
}

// 1.12a: 0x6FB45810
// Calling convention: __stdcall
BOOL __stdcall D2Client_ChangeItemUnitModeCallback(D2UnitStrc* pItem, int32_t nMode)
{
    if (!pItem)
    {
        return FALSE;
    }

    UNITS_ChangeAnimMode(pItem, nMode);
    if (nMode == ITEMMODE_ONFLOOR)
    {
        pItem->dwFlags |= UNITFLAG_TARGETABLE;
    }
    else
    {
        pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    }
    return TRUE;
}

// 1.12a: 0x6FB45850 (and thunk 0x6FB45D50)
// Calling convention: custom EAX = pItem (0x6FB45D50: __fastcall ECX = pItem)
int32_t __fastcall D2Client_GetItemUseSoundId(D2UnitStrc* pItem)
{
    if (!pItem || pItem->dwUnitType != UNIT_ITEM)
    {
        return -1;
    }

    D2ItemsTxt* pItemsTxt = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
    if (!pItemsTxt)
    {
        return -1;
    }

    int32_t nUseSound = pItemsTxt->wUseSound;
    if (ITEMS_CheckItemFlag(pItem, ITEMFLAG_IDENTIFIED, __LINE__, __FILE__))
    {
        int32_t nQuality = ITEMS_GetItemQuality(pItem);
        if (nQuality == ITEMQUAL_UNIQUE)
        {
            int32_t nFileIndex = ITEMS_GetFileIndex(pItem);
            if (nFileIndex >= 0 && nFileIndex < sgptDataTables->nUniqueItemsTxtRecordCount)
            {
                int16_t wUniqueSound = sgptDataTables->pUniqueItemsTxt[nFileIndex].wUseSound;
                if (wUniqueSound > 0)
                {
                    nUseSound = wUniqueSound;
                }
            }
        }
        else if (nQuality == ITEMQUAL_SET)
        {
            int32_t nFileIndex = ITEMS_GetFileIndex(pItem);
            if (nFileIndex >= 0 && nFileIndex < sgptDataTables->nSetItemsTxtRecordCount)
            {
                int16_t wSetSound = sgptDataTables->pSetItemsTxt[nFileIndex].wUseSound;
                if (wSetSound > 0)
                {
                    nUseSound = wSetSound;
                }
            }
        }
    }
    return nUseSound;
}

// 1.12a: 0x6FB45920 (and wrapper 0x6FB45D60)
// Calling convention: custom EAX = pItem, EDX = pnDropSoundFrame (0x6FB45D60: __fastcall ECX = pItem)
int32_t __fastcall D2Client_GetItemDropSoundIdAndDelay(D2UnitStrc* pItem, int32_t* pnDropSoundFrame)
{
    if (!pItem || pItem->dwUnitType != UNIT_ITEM)
    {
        return -1;
    }

    D2ItemsTxt* pItemsTxt = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
    if (!pItemsTxt)
    {
        return -1;
    }

    int32_t nDropSound = pItemsTxt->wDropSound;
    int32_t nDropFrame = pItemsTxt->nDropSfxFrame;

    if (ITEMS_CheckItemFlag(pItem, ITEMFLAG_IDENTIFIED, __LINE__, __FILE__))
    {
        int32_t nQuality = ITEMS_GetItemQuality(pItem);
        if (nQuality == ITEMQUAL_UNIQUE)
        {
            int32_t nFileIndex = ITEMS_GetFileIndex(pItem);
            if (nFileIndex >= 0 && nFileIndex < sgptDataTables->nUniqueItemsTxtRecordCount)
            {
                if (sgptDataTables->pUniqueItemsTxt[nFileIndex].wDropSound > 0)
                {
                    nDropSound = sgptDataTables->pUniqueItemsTxt[nFileIndex].wDropSound;
                }
                if (sgptDataTables->pUniqueItemsTxt[nFileIndex].nDropSfxFrame > 0)
                {
                    nDropFrame = sgptDataTables->pUniqueItemsTxt[nFileIndex].nDropSfxFrame;
                }
            }
        }
        else if (nQuality == ITEMQUAL_SET)
        {
            int32_t nFileIndex = ITEMS_GetFileIndex(pItem);
            if (nFileIndex >= 0 && nFileIndex < sgptDataTables->nSetItemsTxtRecordCount)
            {
                if (sgptDataTables->pSetItemsTxt[nFileIndex].wDropSound > 0)
                {
                    nDropSound = sgptDataTables->pSetItemsTxt[nFileIndex].wDropSound;
                }
                if (sgptDataTables->pSetItemsTxt[nFileIndex].nDropSfxFrame > 0)
                {
                    nDropFrame = sgptDataTables->pSetItemsTxt[nFileIndex].nDropSfxFrame;
                }
            }
        }
    }

    if (pnDropSoundFrame)
    {
        *pnDropSoundFrame = nDropFrame;
    }
    return nDropSound;
}

// 1.12a: 0x6FB45A40
// Calling convention: custom EAX = pUnit, ECX = nSkillId, [esp+4] = nOwnerGUID
BOOL __fastcall D2Client_IsSkillAvailableForAutoRestore(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nOwnerGUID)
{
    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill)
    {
        return FALSE;
    }
    if (SKILLS_GetOwnerGUIDFromSkill(pSkill) != nOwnerGUID)
    {
        return FALSE;
    }
    int32_t nUseState = SKILLS_GetUseState(pUnit, pSkill);
    return (nUseState == 0 || nUseState == 1 || nUseState == 4);
}

// 1.12a: 0x6FB45A80
// Calling convention: __fastcall (ECX = pOwner, EDX = pItem)
void __fastcall D2Client_ExpireItemStatListOnOwner(D2UnitStrc* pOwner, D2UnitStrc* pItem)
{
    if (!pItem || pItem->dwUnitType != UNIT_ITEM)
    {
        return;
    }

    if (pOwner)
    {
        STATLIST_ExpireUnitStatlist(pOwner, pItem);
        STATLIST_ClampStaminaManaHP(pOwner);
        D2Client_ClearItemState54AndUpdateAnimRate(pItem);
    }
    else
    {
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndFlag(pItem, 1);
        if (pStatList)
        {
            D2Common_10474_FreeStatList(pStatList);
        }
    }
}

// 1.12a: 0x6FB45AD0
// Calling convention: __fastcall (ECX = pTargetUnit, EDX = pItem)
void __fastcall D2Client_ApplySocketedOrEquippedItemStats(D2UnitStrc* pTargetUnit, D2UnitStrc* pItem)
{
    if (!pItem || pItem->dwUnitType != UNIT_ITEM || !pTargetUnit)
    {
        return;
    }

    if (pTargetUnit->dwUnitType == UNIT_ITEM &&
        (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_GEM) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_RUNE)))
    {
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndFlag(pItem, 0x40);
        if (pStatList)
        {
            D2Common_10474_FreeStatList(pStatList);
        }
        pStatList = STATLIST_AllocStatList(pItem->pMemoryPool, 0x40, 0, pItem->dwUnitType, pItem->dwUnitId);
        D2Common_10475_PostStatToStatList(pItem, pStatList, FALSE);

        int32_t nApplyType = ITEMS_GetGemApplyType(pTargetUnit);
        D2GemsTxt* pGemsTxt = DATATBLS_GetGemsTxtRecord(pItem->dwClassId);
        if (pGemsTxt)
        {
            (void)nApplyType;
        }
    }

    D2StatListStrc* pItemStatList = STATLIST_GetStatListFromUnitAndFlag(pItem, 1);
    if (!pItemStatList)
    {
        return;
    }

    if (!ITEMS_CheckItemFlag(pItem, ITEMFLAG_IDENTIFIED, __LINE__, __FILE__))
    {
        return;
    }

    ITEMS_UpdateSets(pTargetUnit, pItem, 0, 1);

    int32_t bIsSecondaryWeapon = 0;
    if (ITEMS_CheckIfSocketableByItemId(pItem->dwClassId))
    {
        D2InventoryStrc* pInv = pTargetUnit->pInventory;
        if (pInv && INVENTORY_GetLeftHandWeapon(pInv) != pItem)
        {
            bIsSecondaryWeapon = 1;
        }
    }

    STATLIST_SetStatListFlags(pItemStatList, bIsSecondaryWeapon ? 0x800 : 0);
    STATLIST_MergeStatLists(pTargetUnit, pItem, TRUE);
    D2Client_ClearItemState54AndUpdateAnimRate(pItem);
}

// 1.12a: 0x6FB45C60 (spans 0x6FB45C60..0x6FB45D45; includes 0x6FB45D30 slice label)
// Calling convention: custom EAX = nStatValue, ECX = nStatParam, [esp+4] = pItem, [esp+8] = nStatId
void __fastcall D2Client_UpdateItemChargedSkillStat(int32_t nStatParam, int32_t nStatValue, D2UnitStrc* pItem, int32_t nStatId)
{
    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndFlag(pItem, 0x40);
    if (!pStatList)
    {
        pStatList = STATLIST_GetStatListFromUnitAndFlag(pItem, 1);
    }

    if (nStatId == STAT_CHARGED_SKILL)
    {
        int32_t nSkillId = nStatParam >> 6;
        int32_t nLevel   = nStatParam & 0x3F;
        if (nLevel > 0 && nSkillId >= 0 && nSkillId < SKILLS_GetSkillCount())
        {
            D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pItem, nSkillId);
            if (pSkill)
            {
                SKILLS_SetCharges(pSkill, (uint8_t)nStatValue);
            }
            if (!pStatList)
            {
                return;
            }
            int32_t nMaxCharges = STATLIST_GetStatValue(pStatList, STAT_CHARGED_SKILL, nStatParam) >> 8;
            nStatValue = (nMaxCharges << 8) | (nStatValue & 0xFF);
        }
    }

    STATLIST_SetStatIfListIsValid(pStatList, nStatId, nStatValue, nStatParam);
}

static D2UnitStrc* __fastcall D2Client_ResolveItemPacketOwner(const uint8_t* pPacket)
{
    if (!pPacket)
    {
        return nullptr;
    }
    if (pPacket[0] == 0x9D)
    {
        uint8_t bOwnerType = pPacket[8];
        int32_t nOwnerGUID = *(const int32_t*)(pPacket + 9);
        if (bOwnerType == 6 || nOwnerGUID == -1)
        {
            return nullptr;
        }
        D2UnitStrc* pOwner = D2Client_FindServerSideUnit(nOwnerGUID, bOwnerType);
        if (!pOwner && bOwnerType == UNIT_PLAYER)
        {
            D2UnitStrc* pLocal = D2Client_GetLocalPlayer();
            if (pLocal && (int32_t)pLocal->dwUnitId == nOwnerGUID)
            {
                return pLocal;
            }
        }
        return pOwner;
    }
    return D2Client_GetLocalPlayer();
}

// 1.12a: 0x6FB45D70
// Calling convention: custom ESI = pPacket, EDI = pItem
D2UnitStrc* __fastcall D2Client_DecodeFullItemPacket(const uint8_t* pPacket, D2UnitStrc* pItem)
{
    if (!pPacket || !pItem)
    {
        return nullptr;
    }

    const uint32_t nHdrSize = (pPacket[0] == 0x9D) ? 13u : 8u;
    const uint32_t dwPacketSize = pPacket[2];
    if (dwPacketSize <= nHdrSize)
    {
        return nullptr;
    }

    if (gbIsExpansion)
    {
        pItem->dwFlagEx |= UNITFLAGEX_ISEXPANSION;
    }
    pItem->pDrlgAct = gpClientAct;

    int nSocketed = 0;
    BOOL bFail = FALSE;
    if (!ITEMS_DecodeItemFromBitstream(pItem, (uint8_t*)(pPacket + nHdrSize), dwPacketSize - nHdrSize, FALSE, &nSocketed, 96, &bFail) || bFail)
    {
        return nullptr;
    }

    if (pItem->pInventory)
    {
        for (D2UnitStrc* pSocketed = INVENTORY_GetFirstItem(pItem->pInventory);
             pSocketed != nullptr;
             pSocketed = INVENTORY_GetNextItem(pSocketed))
        {
            D2UnitStrc* pChecked = INVENTORY_UnitIsItem(pSocketed);
            D2Client_ExpireItemStatListOnOwner(pItem, pChecked);
            D2Client_ApplySocketedOrEquippedItemStats(pItem, pChecked);
        }
    }
    return pItem;
}

// 1.12a: 0x6FB45E60
// Calling convention: __stdcall
void __stdcall D2Client_UnequipAndDeleteItemUnit(D2UnitStrc* pItem)
{
    if (!pItem)
    {
        return;
    }

    D2UnitStrc* pOwner = D2Client_FindServerSideUnit(ITEMS_GetOwnerId(pItem), UNIT_PLAYER);
    if (pOwner)
    {
        D2InventoryStrc* pInv = pOwner->pInventory;
        if (pInv)
        {
            for (D2UnitStrc* pIter = INVENTORY_GetFirstItem(pInv); pIter != nullptr; pIter = INVENTORY_GetNextItem(pIter))
            {
                if (INVENTORY_UnitIsItem(pIter) == pItem)
                {
                    if (pItem->dwAnimMode == ITEMMODE_EQUIP)
                    {
                        uint8_t bBodyLoc = ITEMS_GetBodyLocation(pItem);
                        D2Client_ExpireItemStatListOnOwner(pOwner, pItem);
                        if (pOwner == D2Client_GetLocalPlayer())
                        {
                            D2Client_UpdateEquippedWeaponClassCache(nullptr, bBodyLoc);
                        }
                    }
                    INVENTORY_RemoveItem(pItem);
                    break;
                }
            }
        }
    }

    D2Client_RemoveUnitFromServerHashTable(pItem);
    D2Client_FreeClientUnit(pItem);
}

// 1.12a: 0x6FB45F20
// Calling convention: __fastcall (ECX = pItem)
void __fastcall D2Client_FreeItemUnitClientSide(D2UnitStrc* pItem)
{
    if (pItem->pInventory)
    {
        for (D2UnitStrc* pSubItem = INVENTORY_GetFirstItem(pItem->pInventory);
             pSubItem != nullptr;
             pSubItem = INVENTORY_GetNextItem(pSubItem))
        {
            D2UnitStrc* pChecked = INVENTORY_UnitIsItem(pSubItem);
            INVENTORY_RemoveItem(pChecked);
            D2Client_RemoveUnitFromServerHashTable(pChecked);
            D2Client_FreeClientUnit(pChecked);
        }
        INVENTORY_FreeInventory(pItem->pInventory);
        pItem->pInventory = nullptr;
    }

    if (pItem->pStaticPath)
    {
        UNITS_FreeCollisionPath(pItem);
        UNITROOM_RemoveUnitFromRoom(pItem);
    }
}

// 1.12a: 0x6FB45F80
// Calling convention: custom EDX = pOwner, ESI = pItem
void __fastcall D2Client_OnEquipItemComponentUpdate(D2UnitStrc* pOwner, D2UnitStrc* pItem)
{
    if (!pOwner || pOwner->dwUnitType != UNIT_PLAYER)
    {
        return;
    }

    D2ItemsTxt* pItemsTxt = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
    if (pItemsTxt && pItemsTxt->nComponent != 16)
    {
        gbEquippedItemComponentChanged = TRUE;
    }
    UNITS_ChangeAnimMode(pItem, ITEMMODE_EQUIP);
}

// 1.12a: 0x6FB45FE0
// Calling convention: custom EDX = pOwner, ESI = pItem
void __fastcall D2Client_OnUnequipItemComponentUpdate(D2UnitStrc* pOwner, D2UnitStrc* pItem)
{
    if (!pOwner || pOwner->dwUnitType != UNIT_PLAYER)
    {
        return;
    }

    D2ItemsTxt* pItemsTxt = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
    if (pItemsTxt && pItemsTxt->nComponent != 16)
    {
        gbEquippedItemComponentChanged = TRUE;
    }
    ITEMS_SetBodyLocation(pItem, 0);
}

// 1.12a: 0x6FB46030
// Calling convention: __fastcall (ECX = pUnit)
void __fastcall D2Client_RestoreUnitActiveSkillsAfterEquip(D2UnitStrc* pUnit)
{
    if (pUnit == D2Client_GetLocalPlayer())
    {
        if (gnSavedRightSkillId != -1)
        {
            if (D2Client_IsSkillAvailableForAutoRestore(pUnit, gnSavedRightSkillId, gdwSavedRightSkillOwnerGUID))
            {
                D2Client_SetRightSkillForLocalPlayer(gnSavedRightSkillId, gdwSavedRightSkillOwnerGUID);
            }
            else
            {
                D2Client_SetRightSkillForLocalPlayer(0, -1);
            }
        }

        if (gnSavedLeftSkillId != -1)
        {
            if (D2Client_IsSkillAvailableForAutoRestore(pUnit, gnSavedLeftSkillId, gdwSavedLeftSkillOwnerGUID))
            {
                D2Client_SetLeftSkillForLocalPlayer(gnSavedLeftSkillId, gdwSavedLeftSkillOwnerGUID);
            }
            else
            {
                D2Client_SetLeftSkillForLocalPlayer(0, -1);
            }
        }
    }
}

// 1.12a: 0x6FB460B0
// Calling convention: custom EAX = pOwner, EDI = pItem, [esp+4..16] = nX, nY, nMode, bPlaySound
BOOL __fastcall D2Client_PlaceItemInUnitInventoryPage(
    D2UnitStrc* pOwner,
    D2UnitStrc* pItem,
    int32_t nX,
    int32_t nY,
    int32_t nMode,
    BOOL bPlaySound)
{
    uint8_t bInvPage = ITEMS_GetInvPage(pItem);
    UNITS_SetXForStaticUnit(pItem, nX);
    UNITS_SetYForStaticUnit(pItem, nY);

    if (nMode == ITEMMODE_SOCKET)
    {
        if (!pOwner->pInventory)
        {
            pOwner->pInventory = INVENTORY_AllocInventory(pOwner->pMemoryPool, pOwner);
        }
        int32_t nRecordId = UNITS_GetInventoryRecordId(pOwner, bInvPage, pOwner->dwFlags & UNITFLAG_ISEXPANSION);
        if (!INVENTORY_PlaceItemInSocket(pOwner->pInventory, pItem, 1))
        {
            (void)nRecordId;
            return FALSE;
        }
        UNITS_ChangeAnimMode(pItem, ITEMMODE_SOCKET);
        D2Client_ApplySocketedOrEquippedItemStats(pOwner, pItem);
        return TRUE;
    }

    int32_t nRecordId = UNITS_GetInventoryRecordId(pOwner, bInvPage, pOwner->dwFlags & UNITFLAG_ISEXPANSION);
    if (!INVENTORY_PlaceItemAtInventoryPage(pOwner->pInventory, pItem, nX, nY, nRecordId, TRUE, bInvPage))
    {
        return FALSE;
    }

    if (bPlaySound)
    {
        int32_t nDropSound = D2Client_GetItemDropSoundIdAndDelay(pItem, nullptr);
        D2Client_PlaySoundCentered(nDropSound);
    }

    UNITS_ChangeAnimMode(pItem, ITEMMODE_STORED);
    return TRUE;
}

// 1.12a: 0x6FB46210
// Calling convention: __fastcall (ECX = pItem)
void __fastcall D2Client_PlayItemDropFlippyAndLandSound(D2UnitStrc* pItem)
{
    int32_t nDropSoundFrame = 0;
    int32_t nDropSound = D2Client_GetItemDropSoundIdAndDelay(pItem, &nDropSoundFrame);
    int32_t nDelayTicks = (nDropSoundFrame << 8) / (pItem->nActionFrame != 0 ? pItem->nActionFrame : 256);

    D2Client_PlayUnitSound(pItem, 0xD8); // Flippy sound
    D2Client_PlayDelayedSound(nDropSound, nDelayTicks);
}

// 1.12a: 0x6FB46270
// Calling convention: custom EAX = nMode, ECX = nY, EDX = nX, ESI = pItem, [esp+4] = bPlaySound
BOOL __fastcall D2Client_SetupItemUnitAnimModeAndPath(
    D2UnitStrc* pItem,
    int32_t nX,
    int32_t nY,
    int32_t nMode,
    BOOL bPlaySound)
{
    switch (nMode)
    {
    case ITEMMODE_STORED:
    case ITEMMODE_EQUIP:
    case ITEMMODE_INBELT:
    case ITEMMODE_SOCKET:
        UNITS_SetXForStaticUnit(pItem, nX);
        UNITS_SetYForStaticUnit(pItem, nY);
        D2Client_ChangeItemUnitModeCallback(pItem, nMode);
        break;

    case ITEMMODE_ONFLOOR:
    case ITEMMODE_DROPPING:
    {
        D2ActiveRoomStrc* pRoom = D2Client_FindRoomAtCoords(nX, nY);
        if (!pRoom)
        {
            return FALSE;
        }
        UNITS_InitializeStaticPath(pItem, pRoom, nX, nY);
        UNITS_BlockCollisionPath(pItem, pRoom, nX, nY);
        UNITROOM_AddUnitToRoom(pItem, pRoom);
        if (bPlaySound && nMode == ITEMMODE_DROPPING)
        {
            D2Client_PlayItemDropFlippyAndLandSound(pItem);
        }
        D2Client_ChangeItemUnitModeCallback(pItem, nMode);
        break;
    }

    case ITEMMODE_ONCURSOR:
        D2Client_ChangeItemUnitModeCallback(pItem, ITEMMODE_ONCURSOR);
        break;

    default:
        return FALSE;
    }

    int32_t nStorePage = ITEMS_GetStorePage(pItem);
    if (nStorePage < 0)
    {
        if (nStorePage == -2 && !STATLIST_GetStatListFromUnitAndFlag(pItem, 0))
        {
            D2StatListStrc* pStatList = STATLIST_AllocStatList(pItem->pMemoryPool, 0, 0, pItem->dwUnitType, pItem->dwUnitId);
            D2Common_10475_PostStatToStatList(pItem, pStatList, TRUE);
        }
    }
    else if (!pItem->pInventory)
    {
        pItem->pInventory = INVENTORY_AllocInventory(pItem->pMemoryPool, pItem);
    }

    return TRUE;
}

// 1.12a: 0x6FB463D0
// Calling convention: custom EAX = pItem, ECX = nY, EDX = nX, [esp+4..12] = nMode, bInvPage, bPlaySound
BOOL __fastcall D2Client_RepositionExistingItemUnit(
    D2UnitStrc* pItem,
    int32_t nX,
    int32_t nY,
    int32_t nMode,
    uint8_t bInvPage,
    BOOL bPlaySound)
{
    if (!pItem)
    {
        return FALSE;
    }

    if (pItem->dwAnimMode == ITEMMODE_ONFLOOR || pItem->dwAnimMode == ITEMMODE_DROPPING)
    {
        UNITS_FreeCollisionPath(pItem);
        UNITROOM_RemoveUnitFromRoom(pItem);
    }

    ITEMS_SetInvPage(pItem, bInvPage);
    if (!D2Client_SetupItemUnitAnimModeAndPath(pItem, nX, nY, nMode, bPlaySound))
    {
        return FALSE;
    }
    D2Client_ClearItemState54AndUpdateAnimRate(pItem);
    return TRUE;
}

// 1.12a: 0x6FB46450
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemPutInBelt(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
    if (!pOwner || !pOwner->pInventory)
    {
        return;
    }

    D2UnitStrc* pItem = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (!pItem)
    {
        return;
    }

    int32_t nBeltSlot = UNITS_GetXPosition(pItem);
    INVENTORY_RemoveItem(pItem);
    if (INVENTORY_PlaceItemInBeltSlot(pOwner->pInventory, pItem, nBeltSlot))
    {
        UNITS_ChangeAnimMode(pItem, ITEMMODE_INBELT);
        if (pOwner == D2Client_GetLocalPlayer())
        {
            int32_t nSound = D2Client_GetItemDropSoundIdAndDelay(pItem, nullptr);
            D2Client_PlaySoundCentered(nSound);
        }
    }
    else
    {
        D2Client_UnequipAndDeleteItemUnit(pItem);
        D2Client_RequestInventoryResync();
    }
}

// 1.12a: 0x6FB46570
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemRemoveFromBelt(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
    if (!pOwner || !pOwner->pInventory)
    {
        return;
    }

    D2UnitStrc* pItem = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (!pItem)
    {
        return;
    }

    if (INVENTORY_RemoveItem(pItem))
    {
        if (pPacket[1] == 0x0F && !ITEMS_CheckItemFlag(pItem, IFLAG_DELETED, __LINE__, __FILE__))
        {
            D2Client_RepositionExistingItemUnit(pItem, 0, 0, ITEMMODE_ONCURSOR, 0xFF, TRUE);
            INVENTORY_SetCursorItem(pOwner->pInventory, pItem);
            if (pOwner == D2Client_GetLocalPlayer())
            {
                D2Client_PlaySoundCentered(4);
            }
        }
        else
        {
            D2Client_UnequipAndDeleteItemUnit(pItem);
        }
    }
    else
    {
        D2Client_UnequipAndDeleteItemUnit(pItem);
    }
}

// 1.12a: 0x6FB466B0
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemSwapInBelt(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
    if (!pOwner || !pOwner->pInventory)
    {
        return;
    }

    D2UnitStrc* pNewItem = D2Client_CreateItemUnitFromPacket8(pPacket);
    if (!pNewItem)
    {
        return;
    }

    int32_t nBeltSlot = UNITS_GetXPosition(pNewItem);
    D2UnitStrc* pOldItem = INVENTORY_GetItemFromBeltSlot(pOwner->pInventory, nBeltSlot);
    if (pOldItem && pOldItem != pNewItem)
    {
        INVENTORY_RemoveItem(pOldItem);
    }
    if (INVENTORY_GetCursorItem(pOwner->pInventory) == pNewItem)
    {
        INVENTORY_SetCursorItem(pOwner->pInventory, nullptr);
    }
    INVENTORY_RemoveItem(pNewItem);

    if (INVENTORY_PlaceItemInBeltSlot(pOwner->pInventory, pNewItem, nBeltSlot))
    {
        UNITS_ChangeAnimMode(pNewItem, ITEMMODE_INBELT);
        if (pOwner == D2Client_GetLocalPlayer())
        {
            int32_t nSound = D2Client_GetItemDropSoundIdAndDelay(pNewItem, nullptr);
            D2Client_PlaySoundCentered(nSound);
        }
        if (pOldItem && pOldItem != pNewItem)
        {
            D2Client_RepositionExistingItemUnit(pOldItem, 0, 0, ITEMMODE_ONCURSOR, 0xFF, TRUE);
            INVENTORY_SetCursorItem(pOwner->pInventory, pOldItem);
        }
    }
    else
    {
        D2Client_UnequipAndDeleteItemUnit(pNewItem);
        D2Client_RequestInventoryResync();
    }
}

// 1.12a: 0x6FB46860
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemShiftInBelt(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
    if (!pOwner || !pOwner->pInventory)
    {
        return;
    }

    D2UnitStrc* pItem = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (!pItem)
    {
        return;
    }

    int32_t nBeltSlot = UNITS_GetXPosition(pItem);
    INVENTORY_RemoveItem(pItem);
    if (INVENTORY_PlaceItemInBeltSlot(pOwner->pInventory, pItem, nBeltSlot))
    {
        UNITS_ChangeAnimMode(pItem, ITEMMODE_INBELT);
    }
    else
    {
        D2Client_UnequipAndDeleteItemUnit(pItem);
        D2Client_RequestInventoryResync();
    }
}

// 1.12a: 0x6FB468F0
// Calling convention: __fastcall (ECX = pItem)
void __fastcall D2Client_UpdateItemUnitPerTick(D2UnitStrc* pItem)
{
    if (!pItem)
    {
        return;
    }

    if (pItem->dwAnimMode == ITEMMODE_DROPPING && UNITS_IsAtEndOfFrameCycle(pItem))
    {
        D2Client_ChangeItemUnitModeCallback(pItem, ITEMMODE_ONFLOOR);
        D2Client_SetUnitSelectionHighlight(pItem, TRUE);
    }

    if (pItem->dwAnimMode == ITEMMODE_ONFLOOR || pItem->dwAnimMode == ITEMMODE_DROPPING)
    {
        D2ItemsTxt* pItemsTxt = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
        if (pItemsTxt && (pItemsTxt->dwBitField1 & 1))
        {
            // Spawns sparkle client missile (missile ID 0xFF) periodically on floor item
        }
    }
}

// 1.12a: 0x6FB469A0
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_ClearPlayerCursorItemPacketHandler(const uint8_t* pPacket)
{
    D2UnitStrc* pPlayer = D2Client_FindServerSideUnit(*(const int32_t*)(pPacket + 1), UNIT_PLAYER);
    if (!pPlayer)
    {
        return;
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pPlayer->pInventory);
    if (pCursorItem)
    {
        D2Client_UnequipAndDeleteItemUnit(pCursorItem);
    }
    D2Client_SaveUnitVitalsAndActiveSkills(pPlayer);
    D2Client_RecalculateAllEquippedAndCharmItems(pPlayer, TRUE);
    D2Client_RestoreUnitActiveSkillsAfterEquip(pPlayer);
}

// 1.12a: 0x6FB46A30
// Calling convention: __fastcall (ECX = pUnit, EDX = bUpdateAnimAndComponents)
void __fastcall D2Client_RecalculateAllEquippedAndCharmItems(D2UnitStrc* pUnit, BOOL bUpdateAnimAndComponents)
{
    if (!pUnit || !pUnit->pInventory)
    {
        return;
    }

    // 1. First pass: Check charms in inventory (ITEMTYPE_CHARM)
    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory);
         pItem != nullptr;
         pItem = INVENTORY_GetNextItem(pItem))
    {
        D2UnitStrc* pChecked = INVENTORY_UnitIsItem(pItem);
        if (pChecked && INVENTORY_GetItemNodePage(pChecked) == 1 && ITEMS_CheckItemTypeId(pChecked, ITEMTYPE_CHARM))
        {
            BOOL bMeetsReq = D2Client_CheckEquippedItemRequirementsAndQuiver(pUnit, pChecked, FALSE);
            BOOL bDisabled = ITEMS_CheckItemFlag(pChecked, ITEMFLAG_DISABLED, __LINE__, __FILE__);
            if (bMeetsReq && bDisabled)
            {
                ITEMS_SetItemFlag(pChecked, ITEMFLAG_DISABLED, FALSE);
                D2Client_ApplySocketedOrEquippedItemStats(pUnit, pChecked);
            }
            else if (!bMeetsReq && !bDisabled)
            {
                ITEMS_SetItemFlag(pChecked, ITEMFLAG_DISABLED, TRUE);
                D2Client_ExpireItemStatListOnOwner(pUnit, pChecked);
            }
        }
    }

    // 2. Iterative pass over body locations 1..10 until equipment requirement dependencies stabilize
    BOOL bChanged = FALSE;
    do
    {
        bChanged = FALSE;
        for (int32_t nBodyLoc = 1; nBodyLoc < 11; ++nBodyLoc)
        {
            D2UnitStrc* pItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc);
            if (!pItem)
            {
                continue;
            }

            BOOL bMeetsReq = D2Client_CheckEquippedItemRequirementsAndQuiver(pUnit, pItem, FALSE);
            BOOL bDisabled = ITEMS_CheckItemFlag(pItem, ITEMFLAG_DISABLED, __LINE__, __FILE__);
            if (bMeetsReq && bDisabled)
            {
                ITEMS_SetItemFlag(pItem, ITEMFLAG_DISABLED, FALSE);
                D2Client_ApplySocketedOrEquippedItemStats(pUnit, pItem);
                if (pUnit == D2Client_GetLocalPlayer())
                {
                    D2Client_UpdateEquippedWeaponClassCache(pItem, (uint8_t)nBodyLoc);
                }
                bChanged = TRUE;
            }
            else if (!bMeetsReq && !bDisabled)
            {
                ITEMS_SetItemFlag(pItem, ITEMFLAG_DISABLED, TRUE);
                D2Client_ExpireItemStatListOnOwner(pUnit, pItem);
                if (pUnit == D2Client_GetLocalPlayer())
                {
                    D2Client_UpdateEquippedWeaponClassCache(nullptr, (uint8_t)nBodyLoc);
                }
                bChanged = TRUE;
            }
        }
    } while (bChanged);

    // 3. Recompute set item bonuses and update player composite layers if needed
    for (int32_t nBodyLoc = 1; nBodyLoc < 11; ++nBodyLoc)
    {
        D2UnitStrc* pItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc);
        if (pItem && !ITEMS_CheckItemFlag(pItem, ITEMFLAG_DISABLED, __LINE__, __FILE__))
        {
            ITEMS_UpdateSets(pUnit, pItem, 0, 1);
        }
    }

    if (bUpdateAnimAndComponents && gbEquippedItemComponentChanged && pUnit->dwUnitType == UNIT_PLAYER)
    {
        D2Client_RebuildPlayerCompositedUnits(pUnit, TRUE);
        gbEquippedItemComponentChanged = FALSE;
    }
}

// 1.12a: 0x6FB46E40
// Calling convention: custom EDI = pPacket
D2UnitStrc* __fastcall D2Client_CreateItemUnitFromPacket13(const uint8_t* pPacket)
{
    if (!pPacket)
    {
        return nullptr;
    }
    const uint32_t nHdrSize = (pPacket[0] == 0x9D) ? 13u : 8u;
    const uint32_t dwPacketSize = pPacket[2];
    if (dwPacketSize <= nHdrSize)
    {
        return nullptr;
    }

    int32_t nUnitId = *(const int32_t*)(pPacket + 4);
    D2UnitStrc* pItem = D2Client_FindServerSideUnit(nUnitId, UNIT_ITEM);
    if (pItem)
    {
        return D2Client_DecodeFullItemPacket(pPacket, pItem);
    }

    D2ItemSaveStrc itemSave = {};
    if (!ITEMS_GetCompactItemDataFromBitstream((uint8_t*)(pPacket + nHdrSize), dwPacketSize - nHdrSize, FALSE, &itemSave))
    {
        return nullptr;
    }
    int32_t nClassId = itemSave.nClassId;
    if (nClassId < 0)
    {
        return nullptr;
    }

    pItem = UNITS_AllocUnit(nullptr, UNIT_ITEM);
    if (!pItem)
    {
        return nullptr;
    }

    pItem->dwUnitType = UNIT_ITEM;
    pItem->dwClassId  = nClassId;
    pItem->dwUnitId   = nUnitId;
    pItem->pDrlgAct   = gpClientAct;
    if (gbIsExpansion)
    {
        pItem->dwFlagEx |= UNITFLAGEX_ISEXPANSION;
    }
    ITEMS_AllocItemData(pItem->pMemoryPool, pItem);
    UNITS_AllocStaticPath(pItem);

    if (!D2Client_DecodeFullItemPacket(pPacket, pItem))
    {
        ITEMS_FreeItemData(pItem->pMemoryPool, pItem);
        UNITS_FreeStaticPath(pItem);
        UNITS_FreeUnit(pItem);
        return nullptr;
    }

    pItem = D2Client_RegisterUnitInHashTable(pItem);
    if (pItem)
    {
        GFX_InitUnitCompositeGfx(pItem);
    }
    return pItem;
}

// 1.12a: 0x6FB47010
// Calling convention: custom EDI = pPacket
D2UnitStrc* __fastcall D2Client_CreateItemUnitFromPacket8(const uint8_t* pPacket)
{
    return D2Client_CreateItemUnitFromPacket13(pPacket);
}

// 1.12a: 0x6FB471E0
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemAddToGround(const uint8_t* pPacket)
{
    D2UnitStrc* pItem = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (pItem)
    {
        int32_t nX    = UNITS_GetXPosition(pItem);
        int32_t nY    = UNITS_GetYPosition(pItem);
        int32_t nMode = (pPacket[1] == 0x02) ? ITEMMODE_DROPPING : D2Client_GetPacketItemActionMode(pPacket);
        D2Client_SetupItemUnitAnimModeAndPath(pItem, nX, nY, nMode, FALSE);
    }
}

// 1.12a: 0x6FB472D0
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemDropToGround(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
    D2UnitStrc* pItem = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (pItem)
    {
        if (pOwner && pOwner->pInventory && INVENTORY_GetCursorItem(pOwner->pInventory) == pItem)
        {
            INVENTORY_SetCursorItem(pOwner->pInventory, nullptr);
        }
        INVENTORY_RemoveItem(pItem);
        int32_t nX = UNITS_GetXPosition(pItem);
        int32_t nY = UNITS_GetYPosition(pItem);
        D2Client_RepositionExistingItemUnit(pItem, nX, nY, ITEMMODE_DROPPING, 0xFF, TRUE);
    }
}

// 1.12a: 0x6FB47430
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemPutInInventory(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
    D2UnitStrc* pItem  = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (pOwner && pItem)
    {
        if (!pOwner->pInventory)
        {
            pOwner->pInventory = INVENTORY_AllocInventory(pOwner->pMemoryPool, pOwner);
        }
        if (INVENTORY_GetCursorItem(pOwner->pInventory) == pItem)
        {
            INVENTORY_SetCursorItem(pOwner->pInventory, nullptr);
        }
        INVENTORY_RemoveItem(pItem);
        int32_t nX = UNITS_GetXPosition(pItem);
        int32_t nY = UNITS_GetYPosition(pItem);
        D2Client_PlaceItemInUnitInventoryPage(pOwner, pItem, nX, nY, ITEMMODE_STORED, (pOwner == D2Client_GetLocalPlayer()));
        if (pOwner->dwUnitType == UNIT_PLAYER)
        {
            D2Client_SaveUnitVitalsAndActiveSkills(pOwner);
            D2Client_RecalculateAllEquippedAndCharmItems(pOwner, TRUE);
            D2Client_RestoreUnitActiveSkillsAfterEquip(pOwner);
        }
    }
}

// 1.12a: 0x6FB47520
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemRemoveFromInventory(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
    D2UnitStrc* pItem  = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (pOwner && pOwner->pInventory && pItem)
    {
        if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_CHARM) && INVENTORY_GetItemNodePage(pItem) == 1)
        {
            D2Client_ExpireItemStatListOnOwner(pOwner, pItem);
        }
        INVENTORY_RemoveItem(pItem);
        if (!ITEMS_CheckItemFlag(pItem, IFLAG_DELETED, __LINE__, __FILE__))
        {
            D2Client_RepositionExistingItemUnit(pItem, 0, 0, ITEMMODE_ONCURSOR, 0xFF, TRUE);
            INVENTORY_SetCursorItem(pOwner->pInventory, pItem);
        }
        else
        {
            D2Client_UnequipAndDeleteItemUnit(pItem);
        }
        if (pOwner->dwUnitType == UNIT_PLAYER)
        {
            D2Client_SaveUnitVitalsAndActiveSkills(pOwner);
            D2Client_RecalculateAllEquippedAndCharmItems(pOwner, TRUE);
            D2Client_RestoreUnitActiveSkillsAfterEquip(pOwner);
        }
    }
}

// 1.12a: 0x6FB47750
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemEquip(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
    D2UnitStrc* pItem  = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (pOwner && pItem)
    {
        if (!pOwner->pInventory)
        {
            pOwner->pInventory = INVENTORY_AllocInventory(pOwner->pMemoryPool, pOwner);
        }
        if (INVENTORY_GetCursorItem(pOwner->pInventory) == pItem)
        {
            INVENTORY_SetCursorItem(pOwner->pInventory, nullptr);
        }
        INVENTORY_RemoveItem(pItem);

        uint8_t bBodyLoc = ITEMS_GetBodyLocation(pItem);
        if (pOwner->dwUnitType == UNIT_PLAYER)
        {
            D2Client_SaveUnitVitalsAndActiveSkills(pOwner);
        }
        if (D2Client_PlaceItemInBodyLocWrapper(pOwner->pInventory, pItem, bBodyLoc))
        {
            D2Client_OnEquipItemComponentUpdate(pOwner, pItem);
            D2Client_ApplySocketedOrEquippedItemStats(pOwner, pItem);
            if (pOwner == D2Client_GetLocalPlayer())
            {
                D2Client_UpdateEquippedWeaponClassCache(pItem, bBodyLoc);
                int32_t nSound = D2Client_GetItemDropSoundIdAndDelay(pItem, nullptr);
                D2Client_PlaySoundCentered(nSound);
            }
            if (pOwner->dwUnitType == UNIT_PLAYER)
            {
                D2Client_RecalculateAllEquippedAndCharmItems(pOwner, TRUE);
                D2Client_RestoreUnitActiveSkillsAfterEquip(pOwner);
            }
        }
    }
}

// 1.12a: 0x6FB47910
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemSwapBodyLoc(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner   = D2Client_ResolveItemPacketOwner(pPacket);
    D2UnitStrc* pNewItem = D2Client_CreateItemUnitFromPacket8(pPacket);
    if (pOwner && pOwner->pInventory && pNewItem)
    {
        uint8_t bBodyLoc = ITEMS_GetBodyLocation(pNewItem);
        D2UnitStrc* pOldItem = INVENTORY_GetItemFromBodyLoc(pOwner->pInventory, bBodyLoc);
        if (pOwner->dwUnitType == UNIT_PLAYER)
        {
            D2Client_SaveUnitVitalsAndActiveSkills(pOwner);
        }
        if (INVENTORY_GetCursorItem(pOwner->pInventory) == pNewItem)
        {
            INVENTORY_SetCursorItem(pOwner->pInventory, nullptr);
        }
        INVENTORY_RemoveItem(pNewItem);

        if (pOldItem && pOldItem != pNewItem)
        {
            D2Client_ExpireItemStatListOnOwner(pOwner, pOldItem);
            INVENTORY_RemoveItem(pOldItem);
            D2Client_OnUnequipItemComponentUpdate(pOwner, pOldItem);
        }
        if (D2Client_PlaceItemInBodyLocWrapper(pOwner->pInventory, pNewItem, bBodyLoc))
        {
            D2Client_OnEquipItemComponentUpdate(pOwner, pNewItem);
            D2Client_ApplySocketedOrEquippedItemStats(pOwner, pNewItem);
            if (pOldItem && pOldItem != pNewItem)
            {
                D2Client_RepositionExistingItemUnit(pOldItem, 0, 0, ITEMMODE_ONCURSOR, 0xFF, TRUE);
                INVENTORY_SetCursorItem(pOwner->pInventory, pOldItem);
            }
            if (pOwner->dwUnitType == UNIT_PLAYER)
            {
                D2Client_RecalculateAllEquippedAndCharmItems(pOwner, TRUE);
                D2Client_RestoreUnitActiveSkillsAfterEquip(pOwner);
            }
        }
    }
}

// 1.12a: 0x6FB47AD0
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemSwapTwoHandedBodyLoc(const uint8_t* pPacket)
{
    D2Client_PacketHandler_ItemSwapBodyLoc(pPacket);
}

// 1.12a: 0x6FB47D50
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemUnequip(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
    D2UnitStrc* pItem  = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (pOwner && pOwner->pInventory && pItem)
    {
        uint8_t bBodyLoc = ITEMS_GetBodyLocation(pItem);
        if (pOwner->dwUnitType == UNIT_PLAYER)
        {
            D2Client_SaveUnitVitalsAndActiveSkills(pOwner);
        }
        D2Client_ExpireItemStatListOnOwner(pOwner, pItem);
        INVENTORY_RemoveItem(pItem);
        D2Client_OnUnequipItemComponentUpdate(pOwner, pItem);
        if (pOwner == D2Client_GetLocalPlayer())
        {
            D2Client_UpdateEquippedWeaponClassCache(nullptr, bBodyLoc);
            D2Client_PlaySoundCentered(4);
        }
        if (!ITEMS_CheckItemFlag(pItem, IFLAG_DELETED, __LINE__, __FILE__))
        {
            D2Client_RepositionExistingItemUnit(pItem, 0, 0, ITEMMODE_ONCURSOR, 0xFF, TRUE);
            INVENTORY_SetCursorItem(pOwner->pInventory, pItem);
        }
        if (pOwner->dwUnitType == UNIT_PLAYER)
        {
            D2Client_RecalculateAllEquippedAndCharmItems(pOwner, TRUE);
            D2Client_RestoreUnitActiveSkillsAfterEquip(pOwner);
        }
    }
}

// 1.12a: 0x6FB47F30
// Calling convention: __fastcall (ECX = pUnit)
void __fastcall D2Client_RecalculateItemsNoAnimUpdate(D2UnitStrc* pUnit)
{
    D2Client_RecalculateAllEquippedAndCharmItems(pUnit, FALSE);
}

// 1.12a: 0x6FB47F40
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemSwapInInventory(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner   = D2Client_ResolveItemPacketOwner(pPacket);
    D2UnitStrc* pNewItem = D2Client_CreateItemUnitFromPacket8(pPacket);
    if (pOwner && pOwner->pInventory && pNewItem)
    {
        int32_t nX = UNITS_GetXPosition(pNewItem);
        int32_t nY = UNITS_GetYPosition(pNewItem);
        uint8_t bInvPage = ITEMS_GetInvPage(pNewItem);
        int32_t nRecordId = UNITS_GetInventoryRecordId(pOwner, bInvPage, (pOwner->dwFlagEx & UNITFLAGEX_ISEXPANSION) != 0);
        int nCellX = 0, nCellY = 0;
        D2UnitStrc* pOldItem = INVENTORY_GetItemFromInventoryPage(pOwner->pInventory, nX, nY, &nCellX, &nCellY, nRecordId, bInvPage);

        if (pOwner->dwUnitType == UNIT_PLAYER)
        {
            D2Client_SaveUnitVitalsAndActiveSkills(pOwner);
        }
        if (INVENTORY_GetCursorItem(pOwner->pInventory) == pNewItem)
        {
            INVENTORY_SetCursorItem(pOwner->pInventory, nullptr);
        }
        INVENTORY_RemoveItem(pNewItem);

        if (pOldItem && pOldItem != pNewItem)
        {
            if (ITEMS_CheckItemTypeId(pOldItem, ITEMTYPE_CHARM) && INVENTORY_GetItemNodePage(pOldItem) == 1)
            {
                D2Client_ExpireItemStatListOnOwner(pOwner, pOldItem);
            }
            INVENTORY_RemoveItem(pOldItem);
        }
        D2Client_PlaceItemInUnitInventoryPage(pOwner, pNewItem, nX, nY, ITEMMODE_STORED, (pOwner == D2Client_GetLocalPlayer()));
        if (pOldItem && pOldItem != pNewItem)
        {
            D2Client_RepositionExistingItemUnit(pOldItem, 0, 0, ITEMMODE_ONCURSOR, 0xFF, TRUE);
            INVENTORY_SetCursorItem(pOwner->pInventory, pOldItem);
        }
        if (pOwner->dwUnitType == UNIT_PLAYER)
        {
            D2Client_RecalculateAllEquippedAndCharmItems(pOwner, TRUE);
            D2Client_RestoreUnitActiveSkillsAfterEquip(pOwner);
        }
    }
}

static D2UnitStrc* __fastcall D2Client_ResolveActiveShopNpc(void)
{
    if (g_pTradeNpcUnit && g_pTradeNpcUnit->dwUnitType == UNIT_MONSTER)
    {
        return g_pTradeNpcUnit;
    }
    if (g_nInteractingNpcGUID != 0)
    {
        D2UnitStrc* pNpc = D2Client_FindServerSideUnit(g_nInteractingNpcGUID, UNIT_MONSTER);
        if (pNpc)
        {
            g_pTradeNpcUnit = pNpc;
            return pNpc;
        }
    }
    if (gdwActiveInteractNpcGUID != 0)
    {
        D2UnitStrc* pNpc = D2Client_FindServerSideUnit(gdwActiveInteractNpcGUID, UNIT_MONSTER);
        if (pNpc)
        {
            g_pTradeNpcUnit = pNpc;
            return pNpc;
        }
    }
    D2UnitStrc* pPlayer = D2Client_GetLocalPlayer();
    D2UnitStrc* pClosestVendor = nullptr;
    uint32_t dwBestDist = 0xFFFFFFFFu;
    for (int b = 0; b < 128; ++b)
    {
        for (D2UnitStrc* u = gapServerSideUnitHashTables[UNIT_MONSTER][b]; u; u = u->pListNext)
        {
            D2MonStatsTxt* pMon = DATATBLS_GetMonStatsTxtRecord(u->dwClassId);
            if (pMon && (pMon->dwMonStatsFlags & (1u << 24))) // MONSTATSFLAG_INVENTORY
            {
                if (!pPlayer)
                {
                    return u;
                }
                uint32_t dist = UNITS_GetDistanceToOtherUnit(pPlayer, u);
                if (dist < dwBestDist)
                {
                    dwBestDist = dist;
                    pClosestVendor = u;
                }
            }
        }
    }
    if (pClosestVendor)
    {
        g_pTradeNpcUnit = pClosestVendor;
    }
    return pClosestVendor;
}

// 1.12a: 0x6FB481A0
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemAddToShopOrContainer(const uint8_t* pPacket)
{
    D2UnitStrc* pItem = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (!pItem)
    {
        return;
    }

    D2UnitStrc* pNpc = D2Client_ResolveActiveShopNpc();
    if (!pNpc)
    {
        return;
    }

    if (!pNpc->pInventory)
    {
        pNpc->pInventory = INVENTORY_AllocInventory(pNpc->pMemoryPool, pNpc);
    }
    if (!pNpc->pInventory)
    {
        return;
    }

    INVENTORY_RemoveItem(pItem);
    ITEMS_SetItemFlag(pItem, IFLAG_INSTORE, TRUE);

    int32_t nX = UNITS_GetXPosition(pItem);
    int32_t nY = UNITS_GetYPosition(pItem);
    int32_t nStorePage = ITEMS_GetStorePage(pItem);
    uint8_t bPage = ITEMS_GetInvPage(pItem);
    if (bPage > 4)
    {
        bPage = (nStorePage >= 0 && nStorePage <= 3) ? (uint8_t)nStorePage : 0;
        ITEMS_SetInvPage(pItem, bPage);
    }

    int32_t nRecordId = UNITS_GetInventoryRecordId(pNpc, bPage, gbIsExpansion != 0);
    if (!INVENTORY_PlaceItemAtInventoryPage(pNpc->pInventory, pItem, nX, nY, nRecordId, TRUE, bPage))
    {
        INVENTORY_PlaceItemAtFreePosition(pNpc->pInventory, pItem, nRecordId, TRUE, bPage, __FILE__, __LINE__);
    }
    UNITS_ChangeAnimMode(pItem, ITEMMODE_STORED);
}

// 1.12a: 0x6FB483E0
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemRemoveFromShopOrContainer(const uint8_t* pPacket)
{
    D2UnitStrc* pItem = D2Client_FindServerSideUnit(*(const int32_t*)(pPacket + 4), UNIT_ITEM);
    if (pItem)
    {
        INVENTORY_RemoveItem(pItem);
        D2Client_RemoveUnitFromServerHashTable(pItem);
        D2Client_FreeClientUnit(pItem);
    }
}

// 1.12a: 0x6FB48540
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemPutOnCursor(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
    D2UnitStrc* pItem  = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (pOwner && pItem)
    {
        if (!pOwner->pInventory)
        {
            pOwner->pInventory = INVENTORY_AllocInventory(pOwner->pMemoryPool, pOwner);
        }
        INVENTORY_RemoveItem(pItem);
        D2Client_RepositionExistingItemUnit(pItem, 0, 0, ITEMMODE_ONCURSOR, 0xFF, TRUE);
        INVENTORY_SetCursorItem(pOwner->pInventory, pItem);
    }
}

// 1.12a: 0x6FB48600
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemUpdateStats(const uint8_t* pPacket)
{
    D2UnitStrc* pItem = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (pItem)
    {
        D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
        if (!pOwner)
        {
            pOwner = ITEMS_GetOwner(pItem);
        }
        if (pOwner && pOwner->dwUnitType == UNIT_PLAYER)
        {
            D2Client_RecalculateAllEquippedAndCharmItems(pOwner, TRUE);
        }
    }
}

// 1.12a: 0x6FB486A0
// Calling convention: __fastcall (ECX = pUnit)
void __fastcall D2Client_RecalculateItemsWithAnimUpdate(D2UnitStrc* pUnit)
{
    D2Client_RecalculateAllEquippedAndCharmItems(pUnit, TRUE);
}

// 1.12a: 0x6FB486B0
// Calling convention: __fastcall (ECX = pUnit)
void __fastcall D2Client_StripAndFreeUnitInventoryItems(D2UnitStrc* pUnit)
{
    if (!pUnit || !pUnit->pInventory)
    {
        return;
    }

    D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory);
    while (pItem)
    {
        D2UnitStrc* pNext = INVENTORY_GetNextItem(pItem);
        D2UnitStrc* pChecked = INVENTORY_UnitIsItem(pItem);
        if (pChecked)
        {
            D2Client_UnequipAndDeleteItemUnit(pChecked);
        }
        pItem = pNext;
    }
}

// 1.12a: 0x6FB48840
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemPutInSocket(const uint8_t* pPacket)
{
    D2UnitStrc* pTargetItem = D2Client_ResolveItemPacketOwner(pPacket);
    D2UnitStrc* pSocketItem = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (pTargetItem && pSocketItem)
    {
        int32_t nX = UNITS_GetXPosition(pSocketItem);
        int32_t nY = UNITS_GetYPosition(pSocketItem);
        D2Client_PlaceItemInUnitInventoryPage(pTargetItem, pSocketItem, nX, nY, ITEMMODE_SOCKET, TRUE);
        D2UnitStrc* pOwner = ITEMS_GetOwner(pTargetItem);
        if (pOwner && pOwner->dwUnitType == UNIT_PLAYER)
        {
            D2Client_RecalculateAllEquippedAndCharmItems(pOwner, TRUE);
        }
    }
}

// 1.12a: 0x6FB48950
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemAutoUnequipToInventory(const uint8_t* pPacket)
{
    D2UnitStrc* pOwner = D2Client_ResolveItemPacketOwner(pPacket);
    D2UnitStrc* pItem  = D2Client_CreateItemUnitFromPacket13(pPacket);
    if (pOwner && pOwner->pInventory && pItem)
    {
        int32_t nX = UNITS_GetXPosition(pItem);
        int32_t nY = UNITS_GetYPosition(pItem);
        D2Client_ExpireItemStatListOnOwner(pOwner, pItem);
        INVENTORY_RemoveItem(pItem);
        D2Client_OnUnequipItemComponentUpdate(pOwner, pItem);
        D2Client_PlaceItemInUnitInventoryPage(pOwner, pItem, nX, nY, ITEMMODE_STORED, (pOwner == D2Client_GetLocalPlayer()));
        if (pOwner->dwUnitType == UNIT_PLAYER)
        {
            D2Client_RecalculateAllEquippedAndCharmItems(pOwner, TRUE);
        }
    }
}

// 1.12a: 0x6FB48F30
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_WeaponSwitch(const uint8_t* pPacket)
{
    D2UnitStrc* pPlayer = pPacket ? D2Client_ResolveItemPacketOwner(pPacket) : D2Client_GetLocalPlayer();
    if (!pPlayer || !pPlayer->pInventory)
    {
        return;
    }

    D2Client_SaveUnitVitalsAndActiveSkills(pPlayer);

    D2InventoryStrc* pInv = pPlayer->pInventory;
    D2UnitStrc* pRArm   = INVENTORY_GetItemFromBodyLoc(pInv, BODYLOC_RARM);
    D2UnitStrc* pLArm   = INVENTORY_GetItemFromBodyLoc(pInv, BODYLOC_LARM);
    D2UnitStrc* pSwRArm = INVENTORY_GetItemFromBodyLoc(pInv, BODYLOC_SWRARM);
    D2UnitStrc* pSwLArm = INVENTORY_GetItemFromBodyLoc(pInv, BODYLOC_SWLARM);

    if (pRArm)   { D2Client_ExpireItemStatListOnOwner(pPlayer, pRArm);   INVENTORY_RemoveItem(pRArm); }
    if (pLArm)   { D2Client_ExpireItemStatListOnOwner(pPlayer, pLArm);   INVENTORY_RemoveItem(pLArm); }
    if (pSwRArm) { D2Client_ExpireItemStatListOnOwner(pPlayer, pSwRArm); INVENTORY_RemoveItem(pSwRArm); }
    if (pSwLArm) { D2Client_ExpireItemStatListOnOwner(pPlayer, pSwLArm); INVENTORY_RemoveItem(pSwLArm); }

    if (pSwRArm && INVENTORY_PlaceItemInBodyLoc(pInv, pSwRArm, BODYLOC_RARM))
    {
        ITEMS_SetBodyLocation(pSwRArm, BODYLOC_RARM);
        D2Client_OnEquipItemComponentUpdate(pPlayer, pSwRArm);
        D2Client_ApplySocketedOrEquippedItemStats(pPlayer, pSwRArm);
    }
    if (pSwLArm && INVENTORY_PlaceItemInBodyLoc(pInv, pSwLArm, BODYLOC_LARM))
    {
        ITEMS_SetBodyLocation(pSwLArm, BODYLOC_LARM);
        D2Client_OnEquipItemComponentUpdate(pPlayer, pSwLArm);
        D2Client_ApplySocketedOrEquippedItemStats(pPlayer, pSwLArm);
    }
    if (pRArm && INVENTORY_PlaceItemInBodyLoc(pInv, pRArm, BODYLOC_SWRARM))
    {
        ITEMS_SetBodyLocation(pRArm, BODYLOC_SWRARM);
        UNITS_ChangeAnimMode(pRArm, ITEMMODE_EQUIP);
    }
    if (pLArm && INVENTORY_PlaceItemInBodyLoc(pInv, pLArm, BODYLOC_SWLARM))
    {
        ITEMS_SetBodyLocation(pLArm, BODYLOC_SWLARM);
        UNITS_ChangeAnimMode(pLArm, ITEMMODE_EQUIP);
    }

    if (pPlayer == D2Client_GetLocalPlayer())
    {
        D2Client_UpdateEquippedWeaponClassCache(pSwRArm, BODYLOC_RARM);
        D2Client_UpdateEquippedWeaponClassCache(pSwLArm, BODYLOC_LARM);
    }
    gbEquippedItemComponentChanged = TRUE;
    D2Client_RecalculateAllEquippedAndCharmItems(pPlayer, TRUE);
    D2Client_RestoreUnitActiveSkillsAfterEquip(pPlayer);
}

// 1.12a: 0x6FB49160
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_MercenaryItemAction(const uint8_t* pPacket)
{
    D2Client_PacketHandler_ItemEquip(pPacket);
}

// 1.12a: 0x6FB493E0 (spans 0x6FB493E0..0x6FB494EC; includes 0x6FB49430 slice label)
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemAction9C(const uint8_t* pPacket)
{
    if (!pPacket)
    {
        return;
    }
    uint8_t bAction = pPacket[1];
    switch (bAction)
    {
    case 0x00: D2Client_PacketHandler_ItemAddToGround(pPacket); break;             // ITEMACTION_ADDTOGROUND
    case 0x01: D2Client_PacketHandler_ItemPutOnCursor(pPacket); break;             // ITEMACTION_GROUNDTOCURSOR
    case 0x02: D2Client_PacketHandler_ItemDropToGround(pPacket); break;            // ITEMACTION_DROPTOGROUND
    case 0x03: D2Client_PacketHandler_ItemAddToGround(pPacket); break;             // ITEMACTION_ONGROUND
    case 0x04: D2Client_PacketHandler_ItemPutInInventory(pPacket); break;          // ITEMACTION_PUTINCONTAINER
    case 0x05: D2Client_PacketHandler_ItemRemoveFromInventory(pPacket); break;     // ITEMACTION_REMOVEFROMCONTAINER
    case 0x06: D2Client_PacketHandler_ItemEquip(pPacket); break;                   // ITEMACTION_EQUIP
    case 0x07: D2Client_PacketHandler_ItemSwapTwoHandedBodyLoc(pPacket); break;    // ITEMACTION_INDIRECTLYSWAPBODYITEM
    case 0x08: D2Client_PacketHandler_ItemUnequip(pPacket); break;                 // ITEMACTION_UNEQUIP
    case 0x09: D2Client_PacketHandler_ItemSwapBodyLoc(pPacket); break;             // ITEMACTION_SWAPBODYITEM
    case 0x0A: D2Client_PacketHandler_ItemUpdateStats(pPacket); break;             // ITEMACTION_ADDQUANTITY
    case 0x0B: D2Client_PacketHandler_ItemAddToShopOrContainer(pPacket); break;    // ITEMACTION_ADDTOSHOP
    case 0x0C: D2Client_PacketHandler_ItemRemoveFromShopOrContainer(pPacket); break;// ITEMACTION_REMOVEFROMSHOP
    case 0x0D: D2Client_PacketHandler_ItemSwapInInventory(pPacket); break;         // ITEMACTION_SWAPINCONTAINER
    case 0x0E: D2Client_PacketHandler_ItemPutInBelt(pPacket); break;               // ITEMACTION_PUTINBELT
    case 0x0F: D2Client_PacketHandler_ItemRemoveFromBelt(pPacket); break;          // ITEMACTION_REMOVEFROMBELT
    case 0x10: D2Client_PacketHandler_ItemSwapInBelt(pPacket); break;              // ITEMACTION_SWAPINBELT
    case 0x11: D2Client_PacketHandler_ItemAutoUnequipToInventory(pPacket); break;  // ITEMACTION_AUTOUNEQUIP
    case 0x12: D2Client_PacketHandler_ItemPutOnCursor(pPacket); break;             // ITEMACTION_TOCURSOR
    case 0x13: D2Client_PacketHandler_ItemPutInSocket(pPacket); break;             // ITEMACTION_ITEMINSOCKET
    case 0x14: D2Client_PacketHandler_ItemUpdateStats(pPacket); break;             // ITEMACTION_UNKNOWN0x14
    case 0x15: D2Client_PacketHandler_ItemUpdateStats(pPacket); break;             // ITEMACTION_UPDATESTATS
    case 0x16: D2Client_PacketHandler_ItemShiftInBelt(pPacket); break;             // ITEMACTION_UNKNOWN0x16
    case 0x17: D2Client_PacketHandler_WeaponSwitch(pPacket); break;                // ITEMACTION_WEAPONSWITCH
    default: break;
    }
}

// 1.12a: 0x6FB494F0
// Calling convention: __fastcall (ECX = pPacket)
void __fastcall D2Client_PacketHandler_ItemAction9D(const uint8_t* pPacket)
{
    D2Client_PacketHandler_ItemAction9C(pPacket);
}

// 1.12a: 0x6FB4A1F0, 0x6FB4A200, 0x6FB4A230, 0x6FB4A260, 0x6FB4A400, 0x6FB4A430, 0x6FB4A490
// Item cursor / trade / horadric cube / stash helper callbacks
void __stdcall D2Client_RefreshLocalPlayerEquipment(void)
{
    D2UnitStrc* pPlayer = D2Client_GetLocalPlayer();
    if (pPlayer)
    {
        D2Client_RecalculateAllEquippedAndCharmItems(pPlayer, TRUE);
    }
}

// ============================================================================
// PART 3: Scrollable Multi-Line Text List UI Widget (0x6FB4AA60..0x6FB4C26A)
// Total class instance size: 0x180776 bytes (128 entries * 0x300E bytes/entry)
// ============================================================================

#pragma pack(push, 1)
struct D2ScrollTextEntryStrc
{
    wchar_t wszLines[6][1024]; // 6 lines of 1024 wide chars (0x3000 bytes)
    uint8_t nLineColors[6];    // +0x3000
    uint8_t nLineCount;        // +0x3006
    uint8_t nSubType;          // +0x3007
    int32_t nEntryType;        // +0x3008
    int16_t wFlags;            // +0x300C
};

struct D2ScrollTextListStrc
{
    int32_t               nX;                 // +0x00
    int32_t               nY;                 // +0x04
    int32_t               nWidth;             // +0x08
    int32_t               nHeight;            // +0x0C
    int32_t               nVisibleLines;      // +0x10
    int32_t               nFont;              // +0x14
    D2ScrollTextEntryStrc aEntries[128];      // +0x18 (0x180700 bytes)
    // Additional scrollbar and layout state up to +0x180776
};
#pragma pack(pop)

// 1.12a: 0x6FB4AB40 (thunk at 0x6FB4AA60)
// Calling convention: __stdcall
D2ScrollTextListStrc* __stdcall D2Client_CreateScrollTextListWidget(void)
{
    D2ScrollTextListStrc* pWidget = (D2ScrollTextListStrc*)FOG_AllocClientMemory(sizeof(D2ScrollTextListStrc), __FILE__, __LINE__, 0);
    if (pWidget)
    {
        memset(pWidget, 0, sizeof(D2ScrollTextListStrc));
    }
    return pWidget;
}

// 1.12a: 0x6FB4AC20 (thunk at 0x6FB4AA70)
// Calling convention: __fastcall (ECX = pWidget)
void __fastcall D2Client_DestroyScrollTextListWidget(D2ScrollTextListStrc* pWidget)
{
    if (pWidget)
    {
        FOG_FreeClientMemory(pWidget, __FILE__, __LINE__, 0);
    }
}

// ============================================================================
// PART 4: Player Skill Frame Trigger & Multi-Hit Melee Helpers (0x6FB4C270..0x6FB4C3DE)
// ============================================================================

// 1.12a: 0x6FB4C270
// Calling convention: __fastcall (ECX = pUnit)
void __fastcall D2Client_ResetUsedSkillFlags(D2UnitStrc* pUnit)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (pSkill)
    {
        SKILLS_SetFlags(pSkill, 0);
    }
}

// 1.12a: 0x6FB4C290
// Calling convention: __fastcall (ECX = pUnit)
BOOL __fastcall D2Client_CheckSkillFrameTriggerA(D2UnitStrc* pUnit)
{
    int32_t nFrame = 0;
    int32_t nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrame, &nFrameCount);
    if (pUnit->nSeqCurrentFramePrecise != 0)
    {
        return (nFrame == 0);
    }
    return ((pUnit->dwFrameFlags & 1) != 0);
}

// 1.12a: 0x6FB4C2D0
// Calling convention: __fastcall (ECX = pUnit)
BOOL __fastcall D2Client_CheckSkillFrameTriggerB(D2UnitStrc* pUnit)
{
    int32_t nFrame = 0;
    int32_t nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrame, &nFrameCount);
    if (pUnit->nSeqCurrentFramePrecise != 0)
    {
        return (nFrame == 0);
    }
    return ((pUnit->dwFrameFlags & 2) != 0);
}

// 1.12a: 0x6FB4C310
// Calling convention: __fastcall (ECX = pUnit)
void __fastcall D2Client_ResetUnitFrameIndex(D2UnitStrc* pUnit)
{
    pUnit->wFrame = 0;
}

// 1.12a: 0x6FB4C320
// Calling convention: __fastcall (ECX = pUnit)
int32_t __fastcall D2Client_StepMultiHitMeleeSkillFrame(D2UnitStrc* pUnit)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    D2SkillsTxt* pSkillsTxt = DATATBLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    int32_t nTargetType = SKILLS_GetParam2(pSkill);
    int32_t nTargetGUID = SKILLS_GetParam3(pSkill);
    D2UnitStrc* pTarget = D2Client_FindServerSideUnit(nTargetGUID, nTargetType);
    if (!pTarget || !UNITS_IsInMeleeRange(pUnit, pTarget, 0))
    {
        SKILLS_SetFlags(pSkill, 0);
        return 0;
    }

    if (D2Client_CheckSkillFrameTriggerA(pUnit))
    {
        int32_t nRemainingHits = SKILLS_GetParam1(pSkill) - 1;
        SKILLS_SetParam1(pSkill, nRemainingHits);
        if (nRemainingHits > 0)
        {
            UNITS_SetAnimationFrame(pUnit, pSkillsTxt->nSeqInput);
            pUnit->dwFrameFlags &= ~1u;
        }
        return 1;
    }

    return 0;
}
