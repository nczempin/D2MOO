// ============================================================================
// UNIT_PlrSkills.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll (ImageBase = 0x6FAB0000)
// Source slices:
//   - asm/UNIT_PlrSkills.asm (0x6FB51570..0x6FB54EF0)
//       * 0x6FB51570..0x6FB535D6: Skill Hotkeys (F1..F16), Belt Slot Hotkeys,
//         and UI Panel Toggle Hotkey Handlers
//       * 0x6FB535E0..0x6FB54EE5: ..\Source\D2Client\UNIT\PlrSkills.cpp
//         (Client Skill Start/Do Channeled Dispatch, State Applied/Removed
//          Callbacks, State Color/Light/Overlay Sync, Shapeshift State Hooks)
//   - asm/UNIT_Missile.asm   (0x6FAD1880..0x6FAE03E0)
//       * 0x6FAD1880..0x6FAD514A: ..\Source\D2Client\UNIT\Missile.cpp
//         (Client Missile Allocation, Path Init, Per-Tick Stepping, RayTrace/
//          Pierce/NextDelay Collision, CltDoFunc & CltHitFunc Dispatch,
//          Particle Trail Helpers)
//       * 0x6FAD5150..0x6FADDDA3: Missiles.txt CltDoFunc & CltHitFunc Handlers
//       * 0x6FADDDB0..0x6FAE03E0: DialogBox Wrapper & In-Game Help Screen UI
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <math.h>

// D2MOO 1.12a Headers (cross-referenced for exact struct layouts)
#include "D2CommonDefinitions.h"
#include "D2Structs.h"
#include "D2DataTbls.h"
#include "Units/Units.h"
#include "Units/Missile.h"
#include "Units/MissileStream.h"
#include "D2Skills.h"
#include "D2States.h"
#include "D2StatList.h"
#include "D2Collision.h"
#include "D2Dungeon.h"
#include "Path/Path.h"
#include "D2Seed.h"
#include "Fog.h"

inline void __stdcall D2CMP_10064_LightSetColor(void* pLight, uint8_t nR, uint8_t nG, uint8_t nB) { (void)pLight; (void)nR; (void)nG; (void)nB; }
inline void __stdcall D2CMP_10085_RemoveOverlay(void* pOverlayControl, int32_t nOverlayId) { (void)pOverlayControl; (void)nOverlayId; }
inline D2UnitGUID __stdcall MISSILE_GetOwnerGUID(D2UnitStrc* pMissile) { return (pMissile && pMissile->dwUnitType == UNIT_MISSILE && pMissile->pMissileData) ? pMissile->pMissileData->dwOwnerGUID : (D2UnitGUID)-1; }
inline int32_t __stdcall MISSILE_GetOwnerType(D2UnitStrc* pMissile) { return (pMissile && pMissile->dwUnitType == UNIT_MISSILE && pMissile->pMissileData) ? pMissile->pMissileData->nOwnerType : -1; }

// ============================================================================
// Client Missile Initialization Struct (0x5C bytes)
// Used by D2Client_CreateClientMissile (0x6FAD4B20)
// ============================================================================
#pragma pack(push, 1)
struct D2ClientMissileInitStrc
{
    uint32_t dwFlags;               // +0x00 (bits: 0x01=nLoops, 0x02=nFrame, 0x04=use target XY, 0x08=seed, 0x10=vel, 0x20=cltSubMissile, 0x40=subLoop, 0x80=pathFlags, 0x100=postInitCallback, 0x200=flag4000, 0x400=customRange, 0x800=customActivate, 0x1000=zOffset, 0x2000=skipCreateSound)
    D2UnitStrc* pOwner;             // +0x04
    D2UnitStrc* pOrigin;            // +0x08
    D2UnitStrc* pTarget;            // +0x0C
    int32_t nMissileId;             // +0x10
    int32_t nX;                     // +0x14
    int32_t nY;                     // +0x18
    int32_t nTargetX;               // +0x1C
    int32_t nTargetY;               // +0x20
    int32_t nZOffset;               // +0x24
    int32_t nVelocity;              // +0x28
    int32_t nSkillId;               // +0x2C
    int32_t nSkillLevel;            // +0x30
    int32_t nLoops;                 // +0x34
    int32_t nFrame;                 // +0x38
    int32_t nActivateFrame;         // +0x3C
    int32_t nRange;                 // +0x40
    D2SeedStrc* pSeed;              // +0x44
    int32_t nCltSubMissileIndex;    // +0x48
    int32_t nSubLoopIndex;          // +0x4C
    void (__fastcall* pfPostInit)(D2UnitStrc* pMissile, void* pUserData); // +0x50
    void* pPostInitUserData;        // +0x54
    uint32_t dwPathFlags;           // +0x58
};
#pragma pack(pop)

// ============================================================================
// Client State Callback Table Function Pointer (4 bytes, 2-arg __fastcall)
// Dispatch tables at 0x6FB8CF28 (Applied, indexed by wSetFunc) and 0x6FB8CFA8 (Removed, indexed by wRemFunc)
// ============================================================================
typedef int32_t (__fastcall* D2ClientStateCallbackFn_t)(D2UnitStrc* pUnit, int32_t nState);

typedef void (__fastcall* D2CltMissileDoFunc_t)(D2UnitStrc* pMissile);
typedef int32_t (__fastcall* D2CltMissileHitFunc_t)(D2UnitStrc* pMissile, D2UnitStrc* pTarget, int32_t nX, int32_t nY);

// ============================================================================
// External Globals in D2Client.dll (1.12a addresses)
// ============================================================================
extern D2UnitStrc* gpPlayerUnit;                // 0x6FBCC3D0
extern int32_t gnClientSkillUseState;           // 0x6FBCBC48
extern int32_t gdwaSkillDelayEndFrame;          // 0x6FBB3314
extern int32_t gnGameFrame;                     // 0x6FBCC390
extern int32_t gbRunWalkToggle;                 // 0x6FBCBE58
extern int32_t gbStandStillHeld;                // 0x6FBCBC9C
extern int32_t gbSkillSelectPopupOpen;          // 0x6FBD3400
extern int32_t gaPanelStates[38];               // 0x6FBB2B58
#define gbCharacterPanelOpen (gaPanelStates[1])
#define gbInventoryPanelOpen (gaPanelStates[2])
#define gbQuestLogPanelOpen  (gaPanelStates[4])
#define gbHelpScreenOpen     (gaPanelStates[6])
#define gbSkillTreePanelOpen (gaPanelStates[22])
extern int32_t gbAutomapOpen;                   // 0x6FBCC1B0
extern int32_t gnOpenPanelMask;                 // 0x6FBB590C
extern int32_t gbExpansionMode;                 // 0x6FBC97E0

extern D2ClientStateCallbackFn_t gStateAppliedCallbacks[31]; // 0x6FB8CF28 (indexed by D2StatesTxt::wSetFunc)
extern D2ClientStateCallbackFn_t gStateRemovedCallbacks[30]; // 0x6FB8CFA8 (indexed by D2StatesTxt::wRemFunc)
extern D2CltMissileDoFunc_t gCltMissileDoFuncTable[];      // 0x6FBA67C8 (39 entries)
extern D2CltMissileHitFunc_t gCltMissileHitFuncTable[];    // 0x6FBA6938 (66 entries)

// External helpers in D2Client.dll
extern D2UnitStrc* __fastcall D2Client_GetPlayerUnit();                                             // 0x6FB082E0
extern void __fastcall D2Client_AssignSkillHotkeyFromPopup(int32_t nHotkeyIndex);                   // 0x6FB07930
extern void __fastcall D2Client_SelectHotkeyedSkill(int32_t nHotkeyIndex);                          // 0x6FB079E0
extern void __fastcall D2Client_SendWeaponSwitchPacket(int32_t bPlaySound);                         // 0x6FB71FD0
extern void __fastcall D2Client_UseBeltItemSlot(BOOL bShiftHeld, int32_t nBeltSlot);                // 0x6FB33DF0
extern D2UnitStrc* __fastcall D2Client_FindServerSideUnit(int32_t nUnitId, int32_t nUnitType);      // 0x6FACFE80
extern D2UnitStrc* __fastcall D2Client_FindClientSideUnit(int32_t nUnitId, int32_t nUnitType);      // 0x6FACFE40
extern D2UnitStrc* __fastcall D2Client_AllocateClientUnit(int32_t nUnitType, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode, int32_t nUnitId, int32_t a7, int32_t nFlags); // 0x6FAD16C0
extern void __fastcall D2Client_RemoveUnitFromRoomAndFree(D2UnitStrc* pUnit);                       // 0x6FAD0690
extern int32_t __fastcall D2Client_GetUniqueClientUnitId();                                         // 0x6FACFE10
extern void __fastcall D2Client_SetUnitDirectionAndStep(D2UnitStrc* pUnit, int32_t nDir);           // 0x6FACF4C0
extern void __fastcall D2Client_PlayUnitSound(D2UnitStrc* pUnit, int32_t nSoundId);                 // 0x6FAE74E0
extern void __fastcall D2Client_PlaySoundAtCoords(int32_t nSoundId, int32_t nX, int32_t nY);        // 0x6FAE7450
extern void __fastcall D2Client_RefreshPlayerSpeedAndWalkRun(D2UnitStrc* pUnit, int32_t bForce);    // 0x6FB0A800
extern void __fastcall D2Client_RebuildUnitCompositeAppearance(D2UnitStrc* pUnit, int32_t bForce);  // 0x6FACF500
extern void __fastcall D2Client_ApplyMonsterShapeshiftMode(D2UnitStrc* pUnit, int32_t nMode);       // 0x6FAF1E60
extern int32_t __fastcall D2Client_CanStartChanneledSkill(D2UnitStrc* pUnit, D2SkillStrc* pSkill);  // 0x6FB3CF60
extern int32_t __fastcall D2Client_StartChanneledSkillAnim(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nSkillId, int32_t nSLvl); // 0x6FB3CC20
extern int32_t __fastcall D2Client_StepChanneledSkillAnim(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nSkillId, int32_t nSLvl);  // 0x6FB3CD90
extern int32_t __fastcall D2Client_StartInfernoArcticSkill(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nSkillId, int32_t nSLvl); // 0x6FB40380
extern int32_t __fastcall D2Client_StepInfernoArcticSkill(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nSkillId, int32_t nSLvl);  // 0x6FB3EB40

// Forward declarations within this file
D2UnitStrc* __fastcall D2Client_CreateClientMissile(D2ClientMissileInitStrc* pInit);
int32_t __fastcall D2Client_DestroyClientMissile(D2UnitStrc* pMissile, D2UnitStrc* pTarget, int32_t bForceExpire);
void __fastcall D2Client_FreeMissileUnitAndLight(D2UnitStrc* pMissile);


// ============================================================================
// PART 1: SKILL HOTKEYS, BELT SLOTS, AND UI PANEL HOTKEYS
// (0x6FB51570 .. 0x6FB535D6)
// ============================================================================

// Helper logic shared by F1..F16 skill hotkeys (indices 15..0)
static inline void D2Client_HandleSkillHotkeyIndex(int32_t nHotkeyIndex)
{
    if (gbSkillSelectPopupOpen)
    {
        D2Client_AssignSkillHotkeyFromPopup(nHotkeyIndex);
    }
    else
    {
        D2Client_SelectHotkeyedSkill(nHotkeyIndex);
    }
}

// 1.12a: 0x6FB51570 (__cdecl)
void __cdecl D2Client_Hotkey_Skill16() { D2Client_HandleSkillHotkeyIndex(15); }
// 1.12a: 0x6FB515B0 (__cdecl)
void __cdecl D2Client_Hotkey_Skill15() { D2Client_HandleSkillHotkeyIndex(14); }
// 1.12a: 0x6FB515F0 (__cdecl)
void __cdecl D2Client_Hotkey_Skill14() { D2Client_HandleSkillHotkeyIndex(13); }
// 1.12a: 0x6FB51630 (__cdecl)
void __cdecl D2Client_Hotkey_Skill13() { D2Client_HandleSkillHotkeyIndex(12); }
// 1.12a: 0x6FB51670 (__cdecl)
void __cdecl D2Client_Hotkey_Skill12() { D2Client_HandleSkillHotkeyIndex(11); }
// 1.12a: 0x6FB516B0 (__cdecl)
void __cdecl D2Client_Hotkey_Skill11() { D2Client_HandleSkillHotkeyIndex(10); }
// 1.12a: 0x6FB516F0 (__cdecl)
void __cdecl D2Client_Hotkey_Skill10() { D2Client_HandleSkillHotkeyIndex(9); }
// 1.12a: 0x6FB51730 (__cdecl)
void __cdecl D2Client_Hotkey_Skill9()  { D2Client_HandleSkillHotkeyIndex(8); }
// 1.12a: 0x6FB51770 (__cdecl)
void __cdecl D2Client_Hotkey_Skill8()  { D2Client_HandleSkillHotkeyIndex(7); }
// 1.12a: 0x6FB517B0 (__cdecl)
void __cdecl D2Client_Hotkey_Skill7()  { D2Client_HandleSkillHotkeyIndex(6); }
// 1.12a: 0x6FB517F0 (__cdecl)
void __cdecl D2Client_Hotkey_Skill6()  { D2Client_HandleSkillHotkeyIndex(5); }
// 1.12a: 0x6FB51830 (__cdecl)
void __cdecl D2Client_Hotkey_Skill5()  { D2Client_HandleSkillHotkeyIndex(4); }
// 1.12a: 0x6FB51870 (__cdecl)
void __cdecl D2Client_Hotkey_Skill4()  { D2Client_HandleSkillHotkeyIndex(3); }
// 1.12a: 0x6FB518B0 (__cdecl)
void __cdecl D2Client_Hotkey_Skill3()  { D2Client_HandleSkillHotkeyIndex(2); }
// 1.12a: 0x6FB518F0 (__cdecl)
void __cdecl D2Client_Hotkey_Skill2()  { D2Client_HandleSkillHotkeyIndex(1); }
// 1.12a: 0x6FB51930 (__cdecl)
void __cdecl D2Client_Hotkey_Skill1()  { D2Client_HandleSkillHotkeyIndex(0); }

// 1.12a: 0x6FB519E0 (__cdecl)
// Toggle Run/Walk state hotkey
void __cdecl D2Client_Hotkey_ToggleRunWalk()
{
    gbRunWalkToggle = (gbRunWalkToggle == 0) ? 1 : 0;
    D2Client_PlayUnitSound(nullptr, 4);
}

// 1.12a: 0x6FB51A30 (__cdecl)
// Hold Run/Walk key down
void __cdecl D2Client_Hotkey_HoldRunWalk()
{
    gbRunWalkToggle = (gbRunWalkToggle == 0) ? 1 : 0;
}

// 1.12a: 0x6FB51A80 (__cdecl)
// Release Run/Walk key
void __cdecl D2Client_Hotkey_ReleaseRunWalk()
{
    gbRunWalkToggle = (gbRunWalkToggle == 0) ? 1 : 0;
}

// 1.12a: 0x6FB534B0 (__cdecl)
// Weapon Switch hotkey ('W')
void __cdecl D2Client_Hotkey_WeaponSwitch()
{
    if (gbExpansionMode)
    {
        D2Client_SendWeaponSwitchPacket(1);
    }
}

// 1.12a: 0x6FB534D0 (__cdecl)
void __cdecl D2Client_Hotkey_UseBeltSlot4()
{
    BOOL bShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    D2Client_UseBeltItemSlot(bShift, 3);
}

// 1.12a: 0x6FB53510 (__cdecl)
void __cdecl D2Client_Hotkey_UseBeltSlot3()
{
    BOOL bShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    D2Client_UseBeltItemSlot(bShift, 2);
}

// 1.12a: 0x6FB53550 (__cdecl)
void __cdecl D2Client_Hotkey_UseBeltSlot2()
{
    BOOL bShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    D2Client_UseBeltItemSlot(bShift, 1);
}

// 1.12a: 0x6FB53590 (__cdecl)
void __cdecl D2Client_Hotkey_UseBeltSlot1()
{
    BOOL bShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    D2Client_UseBeltItemSlot(bShift, 0);
}

// 1.12a: 0x6FB535D0 (__cdecl)
const void* __fastcall CMDTBL_GetCommandCategoryTable(void);
void* __cdecl D2Client_GetHotkeyTableBase()
{
    return const_cast<void*>(CMDTBL_GetCommandCategoryTable());
}


// ============================================================================
// PART 2: PLAYER SKILLS & CLIENT STATE CALLBACKS
// (..\Source\D2Client\UNIT\PlrSkills.cpp, 0x6FB535E0 .. 0x6FB54EE5)
// ============================================================================

// 1.12a: 0x6FB535E0 (__fastcall)
void __fastcall D2Client_SetClientSkillUseState(int32_t nState)
{
    if (nState != 4 || gnClientSkillUseState != 5)
    {
        gnClientSkillUseState = nState;
    }
}

// 1.12a: 0x6FB53600 (__cdecl)
int32_t __cdecl D2Client_GetClientSkillUseState()
{
    return gnClientSkillUseState;
}

// 1.12a: 0x6FB53610 (__fastcall)
void __fastcall D2Client_StartClientSkillCastingDelay(D2UnitStrc* pUnit, int32_t nDelayFrames)
{
    if (pUnit == D2Client_GetPlayerUnit())
    {
        if (nDelayFrames > 0)
        {
            gdwaSkillDelayEndFrame = gnGameFrame + nDelayFrames;
        }
        else
        {
            gdwaSkillDelayEndFrame = 0;
        }
    }
}

// 1.12a: 0x6FB53640 (__cdecl)
void __cdecl D2Client_ResetClientSkillCastingDelay()
{
    gdwaSkillDelayEndFrame = 0;
}

// 1.12a: 0x6FB53650 (__cdecl)
int32_t __cdecl D2Client_GetClientSkillCastingDelayEndFrame()
{
    return gdwaSkillDelayEndFrame;
}

// 1.12a: 0x6FB53660 (__fastcall)
// State callback: refreshes passive skills on unit
int32_t __fastcall D2Client_RefreshPassiveSkillsStateCallback(D2UnitStrc* pUnit, int32_t nState)
{
    (void)nState;
    SKILLS_RefreshPassiveSkills(pUnit);
    return 1;
}

// 1.12a: 0x6FB53670 & 0x6FB536A0 (__fastcall)
// Computes charge/progressive tier (1..3) from current vs max value
int32_t __fastcall D2Client_GetChargeTierFromRatio(int32_t nMaxVal, int32_t nCurVal)
{
    if (nCurVal <= 0 || nMaxVal <= 0)
    {
        return 0;
    }
    if (nCurVal > nMaxVal)
    {
        return 3;
    }
    int32_t nTier = ((nCurVal - 1) * 3) / nMaxVal + 1;
    if (nTier <= 1) return 1;
    if (nTier >= 3) return 3;
    return nTier;
}

// 1.12a: 0x6FB536D0 (__fastcall)
// State removed callback for Conversion/MindBlast: clears unit owner
int32_t __fastcall D2Client_ClearUnitOwnerStateCallback(D2UnitStrc* pUnit, int32_t nState)
{
    (void)nState;
    UNITS_StoreOwner(pUnit, nullptr);
    return 1;
}

// 1.12a: 0x6FB536E0 (__fastcall)
// State applied callback: reads owner type (0x161) and GUID (0x162) from state statlist
int32_t __fastcall D2Client_SetUnitOwnerFromStateStatList(D2UnitStrc* pUnit, int32_t nState)
{
    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nState);
    if (!pStatList)
    {
        return 0;
    }

    int32_t nOwnerGUID = STATLIST_GetStatValue(pStatList, 0x162, 0);
    int32_t nOwnerType = STATLIST_GetStatValue(pStatList, 0x161, 0);
    UNITS_StoreOwnerTypeAndGUID(pUnit, nOwnerType, nOwnerGUID);
    return 1;
}

// 1.12a: 0x6FB53720 (Custom ABI: EAX = pUnit, ECX = nStatVal, Stack = [nState, nStatId, nLayer])
void __stdcall D2Client_SetUnitStateStatAndRefresh(D2UnitStrc* pUnit, int32_t nStatVal, int32_t nState, int32_t nStatId, int32_t nLayer)
{
    if (!pUnit)
    {
        return;
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nState);
    if (!pStatList)
    {
        uint32_t dwStatFlags = 0x40;
        if (STATES_CheckStateMaskCurseByStateId(nState))
        {
            dwStatFlags |= 0x20;
        }
        if (STATES_CheckStateMaskAuraByStateId(nState))
        {
            dwStatFlags |= 0x08;
        }

        pStatList = STATLIST_AllocStatList(pUnit->pMemoryPool, dwStatFlags, 0, pUnit->dwUnitType, pUnit->dwUnitId);
        if (!pStatList)
        {
            return;
        }
        STATLIST_SetState(pStatList, nState);
        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
    }

    STATLIST_SetStatIfListIsValid(pStatList, nStatId, nStatVal, nLayer);

    if (nStatId == STAT_ALIGNMENT)
    {
        D2StatListStrc* pStateList = STATLIST_GetStatListFromUnitAndFlag(pUnit, 0x40);
        if (pStateList)
        {
            STATLIST_SetStatIfListIsValid(pStateList, STAT_ALIGNMENT, nStatVal, 0);
        }
        else
        {
            STATLIST_SetUnitStat(pUnit, STAT_ALIGNMENT, nStatVal, 0);
        }
    }

    D2ItemStatCostTxt* pItemStatCostTxt = DATATBLS_GetItemStatCostTxtRecord(nStatId);
    if (pItemStatCostTxt && (pItemStatCostTxt->dwItemStatFlags & 0x800))
    {
        UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
        if (pUnit == D2Client_GetPlayerUnit())
        {
            D2Client_RefreshPlayerSpeedAndWalkRun(pUnit, 1);
        }
    }
}

// 1.12a: 0x6FB53810 (__fastcall)
// Evaluates active States.txt records on pUnit and applies the highest priority palette shift & light radius color
int32_t __fastcall D2Client_UpdateUnitStateColorAndLight(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }

    D2StatesTxt* pBestColorState = nullptr;
    D2StatesTxt* pBestLightState = nullptr;
    uint8_t nBestColorPri = 0;
    uint8_t nBestLightPri = 0;

    int32_t nStateCount = sgptDataTables->nStatesTxtRecordCount;
    for (int32_t i = 0; i < nStateCount; ++i)
    {
        if (STATES_CheckState(pUnit, i))
        {
            D2StatesTxt* pStateTxt = DATATBLS_GetStatesTxtRecord(i);
            if (pStateTxt)
            {
                if (pStateTxt->nColorPri >= nBestColorPri)
                {
                    if (pStateTxt->nColorShift != 0)
                    {
                        pBestColorState = pStateTxt;
                        nBestColorPri = pStateTxt->nColorPri;
                    }
                    if (pStateTxt->nLightR != 0 || pStateTxt->nLightG != 0 || pStateTxt->nLightB != 0)
                    {
                        pBestLightState = pStateTxt;
                        nBestLightPri = pStateTxt->nColorPri;
                    }
                }
            }
        }
    }

    if (pBestColorState)
    {
        pUnit->nPGsv = pBestColorState->nColorShift;
    }
    else
    {
        pUnit->nPGsv = 0;
    }

    if (pUnit->pLight)
    {
        if (pBestLightState)
        {
            D2CMP_10064_LightSetColor(pUnit->pLight, pBestLightState->nLightR, pBestLightState->nLightG, pBestLightState->nLightB);
        }
        else
        {
            D2CMP_10064_LightSetColor(pUnit->pLight, 255, 255, 255);
        }
    }

    return 1;
}

// 1.12a: 0x6FB53920 (__fastcall)
// State callback for Cloak of Shadows / Invisibility: sets UNITFLAGEX_ISINVISIBLE
int32_t __fastcall D2Client_ApplyInvisibilityStateCallback(D2UnitStrc* pUnit, int32_t nState)
{
    (void)nState;
    if (pUnit != D2Client_GetPlayerUnit())
    {
        pUnit->dwFlagEx |= UNITFLAGEX_ISINVISIBLE;
    }
    return 1;
}

// 1.12a: 0x6FB53940 (__fastcall)
// State callback for Cloak of Shadows / Invisibility: clears UNITFLAGEX_ISINVISIBLE
int32_t __fastcall D2Client_RemoveInvisibilityStateCallback(D2UnitStrc* pUnit, int32_t nState)
{
    (void)nState;
    if (pUnit != D2Client_GetPlayerUnit())
    {
        pUnit->dwFlagEx &= ~UNITFLAGEX_ISINVISIBLE;
    }
    return 1;
}

// 1.12a: 0x6FB53960 (__fastcall)
// State applied callback for Hide/Burrow: clears UNITFLAG_TARGETABLE
int32_t __fastcall D2Client_ApplyUntargetableStateCallback(D2UnitStrc* pUnit, int32_t nState)
{
    (void)nState;
    pUnit->dwFlags &= ~UNITFLAG_TARGETABLE;
    return 1;
}

// 1.12a: 0x6FB53970 (__fastcall)
// State removed callback for Monster Leap (skill 0xA7): restores unit collision footprint
int32_t __fastcall D2Client_ClearMonsterLeapCollisionOnStateEnd(D2UnitStrc* pUnit, int32_t nState)
{
    (void)nState;
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, 0xA7, -1);
    if (!pSkill)
    {
        return 0;
    }

    if (!SKILLS_GetSkillsTxtRecordFromSkill(pSkill) || SKILLS_GetSkillsTxtRecordFromSkill(pSkill)->nSkillId != 0xA7)
    {
        return 0;
    }

    if (SKILLS_GetParam1(pSkill))
    {
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
        if (pRoom)
        {
            int32_t nX = UNITS_GetClientCoordX(pUnit);
            int32_t nY = UNITS_GetClientCoordY(pUnit);
            int32_t nSize = UNITS_GetUnitSizeX(pUnit);
            COLLISION_ResetMaskWithPattern(pRoom, nX, nY, nSize, 0x100);
        }
        SKILLS_SetParam1(pSkill, 0);
    }

    return 1;
}

// 1.12a: 0x6FB53A00 (__fastcall)
// State applied callback (wSetFunc == 3): syncs remote player skill assignment from state statlist
int32_t __fastcall D2Client_SyncRemotePlayerSkillLevelFromState(D2UnitStrc* pUnit, int32_t nState)
{
    if (!pUnit || pUnit == D2Client_GetPlayerUnit())
    {
        return 1;
    }

    D2StatesTxt* pStatesTxt = DATATBLS_GetStatesTxtRecord(nState);
    if (!pStatesTxt)
    {
        return 0;
    }

    int16_t nSkillId = pStatesTxt->wSkill;
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nState);
    if (!pStatList)
    {
        return 0;
    }

    int16_t nStatId = pStatesTxt->wStat;
    if (nStatId < 0 || nStatId >= sgptDataTables->nItemStatCostTxtRecordCount)
    {
        return 0;
    }

    int32_t nSkillLevel = STATLIST_GetStatValue(pStatList, nStatId, 0);
    SKILLS_AssignSkill(pUnit, nSkillId, nSkillLevel, 1, __FILE__, __LINE__);
    return 1;
}

// 1.12a: 0x6FB53AB0 (__fastcall)
// State callback (wSetFunc == 2): assigns skill level 1 on remote player from state record
int32_t __fastcall D2Client_AssignDefaultStateSkillCallback(D2UnitStrc* pUnit, int32_t nState)
{
    if (!pUnit || pUnit == D2Client_GetPlayerUnit())
    {
        return 1;
    }

    D2StatesTxt* pStatesTxt = DATATBLS_GetStatesTxtRecord(nState);
    if (!pStatesTxt)
    {
        return 0;
    }

    int16_t nSkillId = pStatesTxt->wSkill;
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }

    SKILLS_AssignSkill(pUnit, nSkillId, 1, 1, __FILE__, __LINE__);
    return 1;
}

// 1.12a: 0x6FB53BC0 (__fastcall)
// Checks if player or monster is currently in Skill Cast animation mode
BOOL __fastcall D2Client_IsUnitInSkillCastAnimMode(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return FALSE;
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        return (pUnit->dwAnimMode == PLRMODE_CAST || pUnit->dwAnimMode == PLRMODE_SEQUENCE);
    }
    else if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        return (pUnit->dwAnimMode == MONMODE_CAST || pUnit->dwAnimMode == MONMODE_SEQUENCE);
    }
    return FALSE;
}

// 1.12a: 0x6FB53C10 (__fastcall)
// Dispatches client state removal callbacks from gStateRemovedCallbacks (0x6FB8CFA8, indexed by D2StatesTxt::wRemFunc)
int32_t __fastcall D2Client_OnStateRemovedDispatch(D2UnitStrc* pUnit, int32_t nState)
{
    if (!pUnit || nState < 0)
    {
        return 0;
    }

    D2StatesTxt* pStatesTxt = DATATBLS_GetStatesTxtRecord(nState);
    if (!pStatesTxt)
    {
        return 0;
    }

    int32_t nResult = 1;
    const int16_t wRemFunc = static_cast<int16_t>(pStatesTxt->wRemFunc);
    if (wRemFunc >= 0 && wRemFunc < 30 && gStateRemovedCallbacks[wRemFunc] != nullptr)
    {
        nResult = gStateRemovedCallbacks[wRemFunc](pUnit, nState);
    }

    if (STATES_CheckStateMaskOnSoundByStateId(nState) && static_cast<int16_t>(pStatesTxt->wOffSound) > 0)
    {
        D2Client_PlayUnitSound(pUnit, pStatesTxt->wOffSound);
    }

    return nResult;
}

// 1.12a: 0x6FB53C90 (__fastcall)
// Dispatches client state application callbacks from gStateAppliedCallbacks (0x6FB8CF28, indexed by D2StatesTxt::wSetFunc)
int32_t __fastcall D2Client_OnStateAppliedDispatch(D2UnitStrc* pUnit, int32_t nState, int32_t bIsNewState)
{
    if (!pUnit || nState < 0)
    {
        return 0;
    }

    D2StatesTxt* pStatesTxt = DATATBLS_GetStatesTxtRecord(nState);
    if (!pStatesTxt)
    {
        return 0;
    }

    int32_t nResult = 1;
    const int16_t wSetFunc = static_cast<int16_t>(pStatesTxt->wSetFunc);
    if (wSetFunc >= 0 && wSetFunc < 31 && gStateAppliedCallbacks[wSetFunc] != nullptr)
    {
        nResult = gStateAppliedCallbacks[wSetFunc](pUnit, nState);
    }

    if (bIsNewState && static_cast<int16_t>(pStatesTxt->wOnSound) > 0)
    {
        D2Client_PlayUnitSound(pUnit, pStatesTxt->wOnSound);
    }

    return nResult;
}

// 1.12a: 0x6FB53D70 (__fastcall)
// Applies a randomized 1-of-N overlay from Overlay.txt onto pUnit
int32_t __fastcall D2Client_SetUnitRandomOverlay(D2UnitStrc* pUnit, int32_t nOverlayId)
{
    if (!pUnit || nOverlayId < 0 || nOverlayId >= sgptDataTables->nOverlayTxtRecordCount)
    {
        return 0;
    }

    int32_t nRandomizedOverlay = DATATBLS_Get1OfNFromOverlayTxt(nOverlayId, &pUnit->pSeed);
    if (nRandomizedOverlay < 0 || nRandomizedOverlay >= sgptDataTables->nOverlayTxtRecordCount)
    {
        return 0;
    }

    UNITS_SetOverlay(pUnit, nRandomizedOverlay, 0);
    return 1;
}

// 1.12a: 0x6FB53DE0 (__fastcall)
// Removes all 1-of-N variants of an overlay from pUnit->pMonSeq (or unitgfx)
int32_t __fastcall D2Client_RemoveUnitOverlayVariants(D2UnitStrc* pUnit, int32_t nOverlayId)
{
    if (!pUnit || !pUnit->pMonSeq || nOverlayId < 0)
    {
        return 0;
    }

    D2OverlayTxt* pOverlayTxt = DATATBLS_GetOverlayTxtRecord(nOverlayId);
    if (!pOverlayTxt)
    {
        return 0;
    }

    int32_t nVariants = pOverlayTxt->dw1OfN;
    for (int32_t i = 0; i < nVariants; ++i)
    {
        D2CMP_10085_RemoveOverlay(pUnit->pMonSeq, nOverlayId + i);
    }
    return 1;
}

// 1.12a: 0x6FB53F10 (Thunk at 0x6FB542C0) (__fastcall)
// Client Skill Start dispatch for channeled skills (Inferno / Arctic Blast / Blade Fury)
int32_t __fastcall D2Client_ClientSkillStart_ChanneledSkill(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxt = DATATBLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    if (!D2Client_CanStartChanneledSkill(pUnit, pSkill))
    {
        return 0;
    }

    int16_t wSrvDoFunc = pSkillsTxt->wSrvDoFunc;
    if (wSrvDoFunc == 0x43)
    {
        return D2Client_StartChanneledSkillAnim(pUnit, pSkill, nSkillId, nSkillLevel);
    }
    else if (wSrvDoFunc == 0x4C)
    {
        return D2Client_StartInfernoArcticSkill(pUnit, pSkill, nSkillId, nSkillLevel);
    }

    return 0;
}

// 1.12a: 0x6FB53F90 (Thunk at 0x6FB542D0) (__fastcall)
// Client Skill Do dispatch for channeled skills
int32_t __fastcall D2Client_ClientSkillDo_ChanneledSkill(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxt = DATATBLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    if (!D2Client_CanStartChanneledSkill(pUnit, pSkill))
    {
        return 0;
    }

    int16_t wSrvDoFunc = pSkillsTxt->wSrvDoFunc;
    if (wSrvDoFunc == 0x43)
    {
        return D2Client_StepChanneledSkillAnim(pUnit, pSkill, nSkillId, nSkillLevel);
    }
    else if (wSrvDoFunc == 0x4C)
    {
        return D2Client_StepInfernoArcticSkill(pUnit, pSkill, nSkillId, nSkillLevel);
    }

    return 0;
}

// 1.12a: 0x6FB54B40 (__fastcall)
// State applied/removed callback for Shapeshift (Werewolf / Werebear / Delirium)
int32_t __fastcall D2Client_ShapeshiftStateCallback(D2UnitStrc* pUnit, int32_t nState, int32_t bSet)
{
    if (!pUnit)
    {
        return 0;
    }

    D2Client_RebuildUnitCompositeAppearance(pUnit, 1);

    int32_t nNeutralMode = (pUnit->dwUnitType == UNIT_PLAYER) ? PLRMODE_NEUTRAL : MONMODE_NEUTRAL;
    D2Client_ApplyMonsterShapeshiftMode(pUnit, nNeutralMode);

    if (pUnit == D2Client_GetPlayerUnit())
    {
        D2Client_RefreshPlayerSpeedAndWalkRun(pUnit, 1);
    }

    return 1;
}

// 1.12a: 0x6FB54C90 (__fastcall)
// Clears a state on pUnit on the client side, removing overlays, colors, and running state-removed callbacks
int32_t __fastcall D2Client_ClearUnitStateClientSide(D2UnitStrc* pUnit, int32_t nState)
{
    if (!pUnit)
    {
        return 0;
    }

    D2StatesTxt* pStatesTxt = DATATBLS_GetStatesTxtRecord(nState);
    if (!pStatesTxt)
    {
        return 0;
    }

    if (!STATES_CheckState(pUnit, nState))
    {
        return 0;
    }

    STATES_ToggleState(pUnit, nState, 0);

    // Remove state overlays (wOverlay[0..3])
    for (int32_t i = 0; i < 4; ++i)
    {
        if (pStatesTxt->wOverlay[i] >= 0)
        {
            D2Client_RemoveUnitOverlayVariants(pUnit, pStatesTxt->wOverlay[i]);
        }
    }

    // Play cast/remove overlay if configured
    if (pStatesTxt->wRemOverlay >= 0)
    {
        D2Client_SetUnitRandomOverlay(pUnit, pStatesTxt->wRemOverlay);
    }

    D2Client_UpdateUnitStateColorAndLight(pUnit);
    D2Client_OnStateRemovedDispatch(pUnit, nState);

    if (STATES_CheckStateMaskDisguiseByStateId(nState))
    {
        D2Client_ShapeshiftStateCallback(pUnit, nState, 0);
    }

    UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
    if (pUnit == D2Client_GetPlayerUnit())
    {
        D2Client_RefreshPlayerSpeedAndWalkRun(pUnit, 1);
    }

    return 1;
}

// 1.12a: 0x6FB54DC0 (__fastcall)
// Sets a state on pUnit on the client side, applying overlays, colors, and running state-applied callbacks
int32_t __fastcall D2Client_SetUnitStateClientSide(D2UnitStrc* pUnit, int32_t nState)
{
    if (!pUnit)
    {
        return 0;
    }

    D2StatesTxt* pStatesTxt = DATATBLS_GetStatesTxtRecord(nState);
    if (!pStatesTxt)
    {
        return 0;
    }

    BOOL bAlreadySet = STATES_CheckState(pUnit, nState);
    if (!bAlreadySet)
    {
        STATES_ToggleState(pUnit, nState, 1);
    }

    // Apply cast overlay on initial state application
    if (!bAlreadySet && pStatesTxt->wCastOverlay >= 0)
    {
        D2Client_SetUnitRandomOverlay(pUnit, pStatesTxt->wCastOverlay);
    }

    // Apply persistent state overlays (wOverlay[0..3])
    for (int32_t i = 0; i < 4; ++i)
    {
        if (pStatesTxt->wOverlay[i] >= 0)
        {
            D2Client_SetUnitRandomOverlay(pUnit, pStatesTxt->wOverlay[i]);
        }
    }

    D2Client_UpdateUnitStateColorAndLight(pUnit);
    D2Client_OnStateAppliedDispatch(pUnit, nState, !bAlreadySet);

    if (!bAlreadySet && STATES_CheckStateMaskDisguiseByStateId(nState))
    {
        D2Client_ShapeshiftStateCallback(pUnit, nState, 1);
    }

    UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
    if (pUnit == D2Client_GetPlayerUnit())
    {
        D2Client_RefreshPlayerSpeedAndWalkRun(pUnit, 1);
    }

    return 1;
}


// ============================================================================
// PART 3: CLIENT MISSILE CORE CREATION, STEPPING, AND COLLISION
// (..\Source\D2Client\UNIT\Missile.cpp, 0x6FAD1880 .. 0x6FAD514A)
// ============================================================================

// 1.12a: 0x6FAD1940 (__fastcall)
// Spawns a basic client missile at (nX, nY) -> (nTargetX, nTargetY)
D2UnitStrc* __fastcall D2Client_SpawnClientMissileAtCoords(
    int32_t nMissileId,
    D2UnitStrc* pOwner,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nX,
    int32_t nY,
    int32_t nTargetX,
    int32_t nTargetY,
    int32_t nLoops)
{
    D2ClientMissileInitStrc missileInit = {};
    missileInit.dwFlags = 0;
    missileInit.pOwner = pOwner;
    missileInit.nMissileId = nMissileId;
    missileInit.nX = nX;
    missileInit.nY = nY;
    missileInit.nSkillId = nSkillId;
    missileInit.nSkillLevel = nSkillLevel;

    if (nTargetX != 0 || nTargetY != 0)
    {
        missileInit.dwFlags |= 0x04;
        missileInit.nTargetX = nTargetX;
        missileInit.nTargetY = nTargetY;
    }

    if (nLoops > 0)
    {
        missileInit.dwFlags |= 0x01;
        missileInit.nLoops = nLoops;
    }

    return D2Client_CreateClientMissile(&missileInit);
}

// 1.12a: 0x6FAD19A0 (__stdcall)
// StatList expire callback for State 0x56 (Missile NextDelay)
void __stdcall D2Client_OnMissileHitState56Removed(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    STATES_ToggleState(pUnit, nState, 0);
}

// 1.12a: 0x6FAD1A10 (Custom ABI: EAX = pUnit, ECX = pTargetX, EDX = pTargetY)
// Raytraces from pUnit to (*pTargetX, *pTargetY) against collision mask 0x04
BOOL __fastcall D2Client_RayTraceMissileWallCollision(D2UnitStrc* pUnit, int32_t* pTargetX, int32_t* pTargetY)
{
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
    {
        return FALSE;
    }

    D2CoordStrc pCoords[2];
    pCoords[0].nX = UNITS_GetClientCoordX(pUnit);
    pCoords[0].nY = UNITS_GetClientCoordY(pUnit);
    pCoords[1].nX = *pTargetX;
    pCoords[1].nY = *pTargetY;

    BOOL bClear = COLLISION_RayTrace(pRoom, &pCoords[0], &pCoords[1], 0x04);
    *pTargetX = pCoords[1].nX;
    *pTargetY = pCoords[1].nY;
    return !bClear;
}

// 1.12a: 0x6FAD1A60 (Custom ABI: EDI = pMissile)
// Evaluates Pierce chance (STAT_PIERCE_IDX = 0x148) using missile seed
BOOL __fastcall D2Client_RollMissilePierce(D2UnitStrc* pMissile)
{
    if (!pMissile || pMissile->dwUnitType != UNIT_MISSILE)
    {
        return FALSE;
    }

    int32_t nPiercePct = STATLIST_UnitGetStatValue(pMissile, STAT_PIERCE_IDX, 0);
    if (nPiercePct <= 0)
    {
        return FALSE;
    }

    int32_t nRoll = SEED_RollLimitedRandomNumber(&pMissile->pSeed, 100);
    return (nRoll <= nPiercePct);
}

// 1.12a: 0x6FAD1B80 (Custom ABI: EBX = pMissile, EAX = nX, Stack = [nY, dwPathFlags])
// Allocates missile data, sets anim mode 0, allocates dynamic path, and sets collision masks
int32_t __fastcall D2Client_InitMissileUnitDataAndPath(D2UnitStrc* pMissile, int32_t nX, int32_t nY, uint32_t dwPathFlags)
{
    if (!pMissile || pMissile->dwUnitType != UNIT_MISSILE)
    {
        return 0;
    }

    MISSILE_AllocMissileData(pMissile);
    UNITS_ChangeAnimMode(pMissile, 0);
    PATH_AllocDynamicPath(pMissile->pMemoryPool, nullptr, nX, nY, pMissile, 0);

    if (!pMissile->pDynamicPath)
    {
        return 0;
    }

    PATH_SetFootprintCollisionMask(pMissile->pDynamicPath, 0);

    D2MissilesTxt* pMissilesTxt = DATATBLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxt)
    {
        return 0;
    }

    uint16_t wCollideMask = 0;
    switch (pMissilesTxt->nCollideType)
    {
    case 0: wCollideMask = 0x0000; break;
    case 1: wCollideMask = 0x0804; break;
    case 2: wCollideMask = 0x0805; break;
    case 3: wCollideMask = 0x1C09; break;
    case 4: wCollideMask = 0x0002; break;
    case 5: wCollideMask = 0x1C00; break;
    case 6: wCollideMask = 0x1C0F; break;
    case 7: wCollideMask = 0x1C01; break;
    case 8: wCollideMask = 0x080D; break;
    default: wCollideMask = 0x0000; break;
    }

    PATH_SetMoveTestCollisionMask(pMissile->pDynamicPath, wCollideMask);
    return 1;
}

// 1.12a: 0x6FAD1E80 (__fastcall)
// Spawns a client sub-missile from an existing parent missile
D2UnitStrc* __fastcall D2Client_SpawnSubMissileFromMissile(
    int32_t nMissileId,
    D2UnitStrc* pParentMissile,
    int32_t nSubMissileIndex,
    int32_t nTargetOffsetX,
    int32_t nTargetOffsetY,
    int32_t nZOffset,
    int32_t nVelocity,
    int32_t nLoops)
{
    D2UnitStrc* pOwner = D2Client_FindServerSideUnit(
        MISSILE_GetOwnerGUID(pParentMissile),
        MISSILE_GetOwnerType(pParentMissile));

    D2ClientMissileInitStrc missileInit = {};
    missileInit.dwFlags = 0x20; // Sub-missile flag
    missileInit.pOwner = pOwner;
    missileInit.pOrigin = pParentMissile;
    missileInit.nMissileId = nMissileId;
    missileInit.nX = UNITS_GetClientCoordX(pParentMissile);
    missileInit.nY = UNITS_GetClientCoordY(pParentMissile);
    missileInit.nSkillId = MISSILE_GetSkill(pParentMissile);
    missileInit.nSkillLevel = MISSILE_GetLevel(pParentMissile);
    missileInit.nCltSubMissileIndex = nSubMissileIndex;

    if (nTargetOffsetX != 0 || nTargetOffsetY != 0)
    {
        missileInit.dwFlags |= 0x04;
        missileInit.nTargetX = missileInit.nX + nTargetOffsetX;
        missileInit.nTargetY = missileInit.nY + nTargetOffsetY;
    }

    if (nZOffset != 0)
    {
        missileInit.dwFlags |= 0x1000;
        missileInit.nZOffset = nZOffset;
    }

    if (nVelocity != 0)
    {
        missileInit.dwFlags |= 0x10;
        missileInit.nVelocity = nVelocity;
    }

    if (nLoops > 0)
    {
        missileInit.dwFlags |= 0x01;
        missileInit.nLoops = nLoops;
    }

    return D2Client_CreateClientMissile(&missileInit);
}

// 1.12a: 0x6FAD2E50 (__fastcall)
// Evaluates missile collision/expiration, pierce, NextDelay (state 0x56), CltHitFunc dispatch, and hit sub-missiles
int32_t __fastcall D2Client_DestroyClientMissile(D2UnitStrc* pMissile, D2UnitStrc* pTarget, int32_t bForceExpire)
{
    if (!pMissile || pMissile->dwUnitType != UNIT_MISSILE)
    {
        return 0;
    }

    D2MissilesTxt* pMissilesTxt = DATATBLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxt)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return 2;
    }

    int32_t nX = UNITS_GetClientCoordX(pMissile);
    int32_t nY = UNITS_GetClientCoordY(pMissile);

    if (pTarget)
    {
        // Ignore friendly/owner collisions unless CanCollideFriend is set
        if (MISSILE_CheckUnitIfOwner(pMissile, pTarget) && !(pMissilesTxt->dwMissileFlags & 0x80))
        {
            return 1;
        }

        // NextHit / NextDelay check (State 0x56)
        if ((pMissilesTxt->dwMissileFlags & 0x2000) && pMissilesTxt->nNextDelay > 0)
        {
            if (STATES_CheckState(pTarget, 0x56))
            {
                return 1;
            }

            D2StatListStrc* pDelayStatList = STATLIST_AllocStatList(
                pTarget->pMemoryPool,
                0x02,
                gnGameFrame + pMissilesTxt->nNextDelay,
                pTarget->dwUnitType,
                pTarget->dwUnitId);

            if (pDelayStatList)
            {
                STATLIST_SetState(pDelayStatList, 0x56);
                STATLIST_SetStatRemoveCallback(pDelayStatList, D2Client_OnMissileHitState56Removed);
                D2COMMON_10475_PostStatToStatList(pTarget, pDelayStatList, 1);
                STATES_ToggleState(pTarget, 0x56, 1);
            }
        }
    }

    // Play HitSound or ProgSound
    if (pMissilesTxt->wHitSound > 0)
    {
        D2Client_PlaySoundAtCoords(pMissilesTxt->wHitSound, nX, nY);
    }

    // Dispatch Missiles.txt wCltHitFunc (table at 0x6FBA6938, 66 entries)
    int16_t wCltHitFunc = pMissilesTxt->wCltHitFunc;
    int32_t nHitResult = 2;
    if (wCltHitFunc > 0 && wCltHitFunc < 66 && gCltMissileHitFuncTable[wCltHitFunc] != nullptr)
    {
        nHitResult = gCltMissileHitFuncTable[wCltHitFunc](pMissile, pTarget, nX, nY);
    }

    // Check Pierce (STAT_PIERCE_IDX = 0x148) if allowed by Missiles.txt Pierce flag (0x04)
    if (!bForceExpire && pTarget && (pMissilesTxt->dwMissileFlags & 0x04))
    {
        if (D2Client_RollMissilePierce(pMissile))
        {
            return 1; // Keep flying through target
        }
    }

    if (nHitResult == 2 || bForceExpire)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return 2;
    }

    return nHitResult;
}

// 1.12a: 0x6FAD3150 (__fastcall)
// Frees a client missile unit, resetting its light map and removing it from its room
void __fastcall D2Client_FreeMissileUnitAndLight(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return;
    }

    UNITS_ResetLightMap(pMissile);
    D2Client_RemoveUnitFromRoomAndFree(pMissile);
}

// 1.12a: 0x6FAD31F0 (__fastcall)
// Steps missile movement, frame countdown, town kill checks, and path step collision detection
void __fastcall D2Client_StepMissileMovementAndCollision(D2UnitStrc* pMissile)
{
    if (!pMissile || pMissile->dwUnitType != UNIT_MISSILE)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxt = DATATBLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxt)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);
    if (nRemainingFrames <= 0)
    {
        D2Client_DestroyClientMissile(pMissile, nullptr, 1);
        return;
    }

    MISSILE_SetRemainingFrames(pMissile, nRemainingFrames - 1);

    // Step dynamic path if active
    if (pMissile->pDynamicPath && MISSILE_GetRemainingFrames(pMissile) <= MISSILE_GetActivateFrame(pMissile))
    {
        D2PathPointStrc* pSaveSteps = nullptr;
        int32_t nSaveStepCount = D2COMMON_10198_PathGetSaveStep(pMissile->pDynamicPath, &pSaveSteps);
        (void)nSaveStepCount;

        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
        if (!pRoom)
        {
            D2Client_FreeMissileUnitAndLight(pMissile);
            return;
        }

        // Check town kill flag (+0x12C / Town bit in Missiles.txt)
        if (DUNGEON_IsRoomInTown(pRoom) && !(pMissilesTxt->dwMissileFlags & 0x1000))
        {
            D2Client_DestroyClientMissile(pMissile, nullptr, 1);
            return;
        }
    }
}

// 1.12a: 0x6FAD3520 (__fastcall)
// Per-tick Client Missile DoFunc dispatcher (table at 0x6FBA67C8, 39 entries)
void __fastcall D2Client_DispatchMissileCltDoFunc(D2UnitStrc* pMissile)
{
    if (!pMissile || pMissile->dwUnitType != UNIT_MISSILE)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxt = DATATBLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxt)
    {
        return;
    }

    int16_t wCltDoFunc = pMissilesTxt->wCltDoFunc;
    if (wCltDoFunc > 0 && wCltDoFunc < 39 && gCltMissileDoFuncTable[wCltDoFunc] != nullptr)
    {
        gCltMissileDoFuncTable[wCltDoFunc](pMissile);
    }
    else
    {
        D2Client_StepMissileMovementAndCollision(pMissile);
    }
}

// 1.12a: 0x6FAD4B20 (__fastcall)
// Master Client Missile Creation Function (takes D2ClientMissileInitStrc* in ECX)
D2UnitStrc* __fastcall D2Client_CreateClientMissile(D2ClientMissileInitStrc* pInit)
{
    if (!pInit || pInit->nMissileId < 0 || pInit->nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return nullptr;
    }

    D2MissilesTxt* pMissilesTxt = DATATBLS_GetMissilesTxtRecord(pInit->nMissileId);
    if (!pMissilesTxt)
    {
        return nullptr;
    }

    int32_t nUnitId = D2Client_GetUniqueClientUnitId();
    uint32_t dwCreateFlags = UNITFLAG_ISCLIENTUNIT;
    if (pInit->dwFlags & 0x200)
    {
        dwCreateFlags |= 0x4000;
    }

    D2UnitStrc* pMissile = D2Client_AllocateClientUnit(
        UNIT_MISSILE,
        pInit->nMissileId,
        pInit->nX,
        pInit->nY,
        0,
        nUnitId,
        1,
        dwCreateFlags);

    if (!pMissile)
    {
        return nullptr;
    }

    // Seed initialization
    if (pInit->dwFlags & 0x08 && pInit->pSeed)
    {
        pMissile->pSeed.nLowSeed = pInit->pSeed->nLowSeed;
        pMissile->pSeed.nHighSeed = pInit->pSeed->nHighSeed;
    }

    // Link owner and record stats
    if (pInit->pOwner)
    {
        UNITS_StoreOwner(pMissile, pInit->pOwner);
    }

    MISSILE_InitMissileStats(pMissile, pInit->pOwner, pInit->nMissileId, pInit->nSkillId, pInit->nSkillLevel);

    // Compute total range & activate frame
    int32_t nRange = (pInit->dwFlags & 0x400)
        ? pInit->nRange
        : MISSILE_CalculateMasteryBonus(pMissile, pInit->pOwner, pInit->nMissileId, pInit->nSkillId, pInit->nSkillLevel);

    int32_t nActivateFrame = (pInit->dwFlags & 0x800)
        ? pInit->nActivateFrame
        : pMissilesTxt->nActivate;

    MISSILE_SetTotalFrames(pMissile, nRange);
    MISSILE_SetRemainingFrames(pMissile, nRange);
    MISSILE_SetActivateFrame(pMissile, nRange - nActivateFrame);

    // Play travel sound if configured and not suppressed
    if (!(pInit->dwFlags & 0x2000) && pMissilesTxt->wTravelSound > 0)
    {
        D2Client_PlayUnitSound(pMissile, pMissilesTxt->wTravelSound);
    }

    // Run optional post-init callback
    if ((pInit->dwFlags & 0x100) && pInit->pfPostInit)
    {
        pInit->pfPostInit(pMissile, pInit->pPostInitUserData);
    }

    return pMissile;
}
