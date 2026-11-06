// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Translation Unit: UNIT_Player.cpp
// Combined Sources:
//   - ..\Source\D2Client\UNIT\PlayerList.cpp (asm/UNIT_PlayerList.asm, 0x6FAE03E0..0x6FAE0EB0)
//   - ..\Source\D2Client\UNIT\Player.cpp     (asm/UNIT_Player.asm,     0x6FB076B0..0x6FB0DFF0)
// ImageBase: 0x6FAB0000
// ============================================================================

#include <stdint.h>
#include <windows.h>

// ----------------------------------------------------------------------------
// Forward Declarations & Core Engine Structures (from D2Common / D2Client)
// ----------------------------------------------------------------------------
struct D2UnitStrc;
struct D2InventoryStrc;
struct D2SkillStrc;
struct D2SkillListStrc;
struct D2DynamicPathStrc;
struct D2StaticPathStrc;
struct D2RoomStrc;
struct D2RoomCoordListStrc;
struct D2CoordStrc { int32_t nX; int32_t nY; };
struct D2CellFileStrc;
struct D2GfxDataStrc;
struct D2StatListStrc;
struct D2PlayerDataStrc;
struct D2SkillsTxt;
struct D2CharStatsTxt;
struct D2MonStatsTxt;
struct D2AutomapLayerStrc;

// Player relationship list node (12 bytes, allocated in PlayerList.cpp)
struct D2PlayerListNodeStrc
{
    uint32_t dwUnitId;              // +0x00
    uint32_t dwFlags;               // +0x04
    D2PlayerListNodeStrc* pNext;    // +0x08
};

// Player relationship list head (4 bytes, allocated in PlayerList.cpp)
struct D2PlayerListStrc
{
    D2PlayerListNodeStrc* pFirst;   // +0x00
};

// Client roster entry (partial layout for PlayerList.cpp / Player.cpp)
struct D2RosterUnitStrc
{
    char szName[16];                // +0x00
    uint32_t dwUnitId;              // +0x10
    uint32_t dwPartyLife;           // +0x14
    uint32_t dwKillCount;           // +0x18
    uint32_t dwClassId;             // +0x1C
    uint16_t wLevel;                // +0x20
    uint16_t wPartyId;              // +0x22
    uint32_t dwLevelId;             // +0x24
    uint32_t dwPosX;                // +0x28
    uint32_t dwPosY;                // +0x2C
    uint32_t dwPartyFlags;          // +0x30
    D2PlayerListStrc* pPlayerList;  // +0x34
    uint8_t pad38[0x48];            // +0x38
    D2RosterUnitStrc* pNext;        // +0x80
};

// Speedbar skill icon button cell entry (16 bytes, array at 0x6FBCBC90)
struct D2SpeedBarCellStrc
{
    int32_t nSkillId;               // +0x00
    uint8_t bRightHand;             // +0x04
    uint8_t pad05[3];               // +0x05
    uint32_t dwSkillFlags;          // +0x08
    uint32_t dwOwnerGUID;           // +0x0C
};

// ----------------------------------------------------------------------------
// External Globals in D2Client.dll (1.12a)
// ----------------------------------------------------------------------------
extern D2UnitStrc* gpPlayerUnit_6FBCC3D0;              // 0x6FBCC3D0: Local player unit
extern D2UnitStrc* gPlayerUnits_6FBCB6D4[8];           // 0x6FBCB6D4: Active player unit table (8 slots)
extern int32_t gnViewportOffsetX_6FBC21D0;             // 0x6FBC21D0: Viewport X offset
extern int32_t gnViewportOffsetY_6FBC21CC;             // 0x6FBC21CC: Viewport Y offset
extern int32_t gbPerspectiveShakeActive_6FBCBE58;      // 0x6FBCBE58: Screen shake active flag
extern int32_t gnShakeOffsetX_6FBCBDCC;                // 0x6FBCBDCC: Screen shake X delta
extern int32_t gnShakeOffsetY_6FBCC3F0;                // 0x6FBCC3F0: Screen shake Y delta
extern int32_t gnScreenWidth_6FBBA740;                 // 0x6FBBA740: Screen width in pixels
extern int32_t gnScreenHeight_6FBBA744;                // 0x6FBBA744: Screen height in pixels
extern int32_t gbSpeedBarOpen_6FBCBC50;                // 0x6FBCBC50: Speedbar popup open flag
extern int32_t gbSpeedBarRightSide_6FBCBC54;           // 0x6FBCBC54: Speedbar opened for right-hand skill

// ============================================================================
// PART 1: Display / Cache / Performance Helpers (0x6FAE03E0..0x6FAE0C30)
// ============================================================================

// 1.12a: 0x6FAE03E0 (__stdcall)
int32_t __stdcall D2Client_GetPerfStat6FBA675C()
{
    return *(int32_t*)0x6FBA675C;
}

// 1.12a: 0x6FAE03F0 (__stdcall)
int32_t __stdcall D2Client_GetPerfStat6FBA6760()
{
    return *(int32_t*)0x6FBA6760;
}

// 1.12a: 0x6FAE0400 (__stdcall)
int32_t __stdcall D2Client_GetPerfStat6FBA6764()
{
    return *(int32_t*)0x6FBA6764;
}

// 1.12a: 0x6FAE0410 (custom: eax = nValue)
int32_t D2Client_SetPerfStat6FBA675C(/*eax*/ int32_t nValue)
{
    int32_t nPrev = *(int32_t*)0x6FBA675C;
    *(int32_t*)0x6FBA675C = nValue;
    return nPrev;
}

// 1.12a: 0x6FAE0420 (custom: eax = nValue)
int32_t D2Client_SetPerfStat6FBA6760(/*eax*/ int32_t nValue)
{
    int32_t nPrev = *(int32_t*)0x6FBA6760;
    *(int32_t*)0x6FBA6760 = nValue;
    return nPrev;
}

// 1.12a: 0x6FAE0430 (custom: eax = nValue)
int32_t D2Client_SetPerfStat6FBA6764(/*eax*/ int32_t nValue)
{
    int32_t nPrev = *(int32_t*)0x6FBA6764;
    *(int32_t*)0x6FBA6764 = nValue;
    return nPrev;
}

// 1.12a: 0x6FAE0440 (__stdcall)
void __stdcall D2Client_SetWeatherAndLightingQuality(BOOL bEnable)
{
    *(int32_t*)0x6FBA6764 = bEnable;
    *(int32_t*)0x6FBC97EC = bEnable;
    *(int32_t*)0x6FBA675C = bEnable;
    // D2Client_SetWeatherEnabled(bEnable); // 0x6FAD1C40
    // D2Gfx_10042(bEnable);
}

// 1.12a: 0x6FAE0470 (__stdcall)
void __stdcall D2Client_ResetFramePerfCounters()
{
    *(int32_t*)0x6FBC97E0 = 0;
    *(int32_t*)0x6FBC97E8 = 0;
    *(int32_t*)0x6FBC97DC = 0;
    *(int32_t*)0x6FBC97E4 = 0;
}

// 1.12a: 0x6FAE0490 (custom: ecx = nDeltaA, eax = nDeltaB)
void D2Client_AccumulatePerfCounters(/*ecx*/ int32_t nDeltaA, /*eax*/ int32_t nDeltaB)
{
    *(int32_t*)0x6FBC97E4 += nDeltaA;
    *(int32_t*)0x6FBC97E8 += nDeltaB;
}

// 1.12a: 0x6FAE04B0 (__stdcall)
void __stdcall D2Client_ShutdownTileAndSpriteCaches()
{
    // D2CMP_10040(); // DestroyTileCache
    // D2CMP_10099(1); // FlushSpriteCache(TRUE)
    void** ppPool = *(void***)0x6FBC97F0;
    if (ppPool)
    {
        // FOG_10144(ppPool); // FOG_FreePool
        *(void**)0x6FBC97F0 = nullptr;
    }
}

// 1.12a: 0x6FAE04F0 (custom: esi = bReinit)
void D2Client_InitTileAndSpriteCaches(/*esi*/ BOOL bReinit)
{
    // Queries FOG_10219() (system memory info) to size sprite/tile caches,
    // then calls D2CMP_10018 (InitTileCache) and D2CMP_10025 (InitSpriteCache).
}

// 1.12a: 0x6FAE0610 (__stdcall)
void __stdcall D2Client_ResetGfxAndTileCacheStats()
{
    // Resets 68-dword cache results table from D2Gfx_10051() and calls D2CMP_10021().
}

// 1.12a: 0x6FAE06B0 (__stdcall)
void __stdcall D2Client_RenderCacheStatsOverlay()
{
    // Renders tile cache and sprite cache statistics text overlay via D2Win_10117.
}

// 1.12a: 0x6FAE08D0 (__fastcall)
void __fastcall D2Client_SetGfxGamma(int32_t nGamma)
{
    // D2Gfx_10011(nGamma);
}

// 1.12a: 0x6FAE08E0 (__stdcall)
int32_t __stdcall D2Client_TogglePerspectiveMode()
{
    // Toggles perspective mode via D2Gfx_10046 / D2Gfx_10066.
    return 0;
}

// 1.12a: 0x6FAE0910 (__stdcall)
BOOL __stdcall D2Client_ToggleBlendedShadows()
{
    BOOL bNewVal = (*(int32_t*)0x6FBA6760 == 0);
    *(int32_t*)0x6FBA6760 = bNewVal;
    return bNewVal;
}

// 1.12a: 0x6FAE0930 (__stdcall)
BOOL __stdcall D2Client_ToggleLowQualityMode()
{
    BOOL bNewVal = (*(int32_t*)0x6FBA6764 == 0);
    D2Client_SetWeatherAndLightingQuality(bNewVal);
    return bNewVal;
}

// 1.12a: 0x6FAE0990 (custom: edi = nMode)
void D2Client_ToggleFpsDisplayMode(/*edi*/ int32_t nMode)
{
    if (*(int32_t*)0x6FBC97EC && *(int32_t*)0x6FBC97D8 == nMode)
    {
        *(int32_t*)0x6FBC97EC = 0;
    }
    else
    {
        *(int32_t*)0x6FBC97EC = 1;
        *(int32_t*)0x6FBC97D8 = nMode;
    }
}

// 1.12a: 0x6FAE09B0 (__stdcall)
void __stdcall D2Client_UpdatePerfHistoryRingBuffer()
{
    // Updates the 64-entry performance history ring buffer at 0x6FBC94D8 / 0x6FBC95D8 / 0x6FBC96D8.
}

// 1.12a: 0x6FAE09D0 (__stdcall)
void __stdcall D2Client_RenderFpsAndMemoryOverlay()
{
    // Renders FPS / Skip / Ping and GlobalMemoryStatus (Phys / Page / Video) overlay.
    // Uses 7-case switch jump table at 0x6FAE0C14..0x6FAE0C30.
}

// ============================================================================
// PART 2: PlayerList.cpp (0x6FAE0C30..0x6FAE0EB0)
// Source File: ..\Source\D2Client\UNIT\PlayerList.cpp
// ============================================================================

// 1.12a: 0x6FAE0C30 (custom: edi = pPlayerList)
void D2Client_FreePlayerList(/*edi*/ D2PlayerListStrc* pPlayerList)
{
    if (!pPlayerList)
    {
        return;
    }

    D2PlayerListNodeStrc* pNode = pPlayerList->pFirst;
    while (pNode)
    {
        D2PlayerListNodeStrc* pNext = pNode->pNext;
        // FOG_FreeClientMemory(pNode, "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 60, 0);
        pNode = pNext;
    }

    // FOG_FreeClientMemory(pPlayerList, "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 64, 0);
}

// 1.12a: 0x6FAE0C70 (__stdcall)
D2PlayerListStrc* __stdcall D2Client_AllocPlayerList()
{
    // D2PlayerListStrc* pList = (D2PlayerListStrc*)FOG_AllocClientMemory(sizeof(D2PlayerListStrc), "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 37, 0);
    D2PlayerListStrc* pList = nullptr;
    if (!pList)
    {
        // D2Client_FatalError(D2Client_GetLoadErrorString());
        return nullptr;
    }
    pList->pFirst = nullptr;
    return pList;
}

// 1.12a: 0x6FAE0CB0 (custom: eax = dwPlayerUnitId, esi = dwOtherUnitId)
void D2Client_RemovePlayerFromList(/*eax*/ uint32_t dwPlayerUnitId, /*esi*/ uint32_t dwOtherUnitId)
{
    // D2RosterUnitStrc* pRoster = D2Client_FindRosterUnitById(dwPlayerUnitId); // 0x6FB161D0
    D2RosterUnitStrc* pRoster = nullptr;
    if (!pRoster)
    {
        return;
    }

    D2PlayerListStrc* pList = pRoster->pPlayerList;
    if (!pList)
    {
        // FOG_DisplayWarning("pPlayerList", "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 148);
        return;
    }

    D2PlayerListNodeStrc* pPrev = nullptr;
    for (D2PlayerListNodeStrc* pCur = pList->pFirst; pCur != nullptr; pCur = pCur->pNext)
    {
        if (pCur->dwUnitId == dwOtherUnitId)
        {
            if (pPrev)
            {
                pPrev->pNext = pCur->pNext;
            }
            else
            {
                pList->pFirst = pCur->pNext;
            }
            // FOG_FreeClientMemory(pCur, "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 160, 0);
            return;
        }
        pPrev = pCur;
    }
}

// 1.12a: 0x6FAE0D20 (custom: edx = dwPlayerUnitId, esi = dwOtherUnitId)
D2PlayerListNodeStrc* D2Client_FindPlayerListEntry(/*edx*/ uint32_t dwPlayerUnitId, /*esi*/ uint32_t dwOtherUnitId)
{
    // D2RosterUnitStrc* pRoster = D2Client_FindRosterUnitById(dwPlayerUnitId); // 0x6FB161D0
    D2RosterUnitStrc* pRoster = nullptr;
    if (!pRoster || !pRoster->pPlayerList)
    {
        return nullptr;
    }

    for (D2PlayerListNodeStrc* pCur = pRoster->pPlayerList->pFirst; pCur != nullptr; pCur = pCur->pNext)
    {
        if (pCur->dwUnitId == dwOtherUnitId)
        {
            return pCur;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FAE0D50 (custom: ecx = dwPlayerUnitId, eax = dwOtherUnitId, stack arg_0 = dwFlags)
void __stdcall D2Client_SetPlayerListRelationFlags(/*ecx*/ uint32_t dwPlayerUnitId, /*eax*/ uint32_t dwOtherUnitId, uint32_t dwFlags)
{
    D2PlayerListNodeStrc* pEntry = D2Client_FindPlayerListEntry(dwPlayerUnitId, dwOtherUnitId);
    if (pEntry)
    {
        pEntry->dwFlags = dwFlags;
    }
    else
    {
        // D2Client_AddPlayerToList(dwPlayerUnitId, dwOtherUnitId, dwFlags);
    }
}

// 1.12a: 0x6FAE0D90 (custom: eax = dwPlayerUnitId, ecx = dwOtherUnitId)
uint16_t D2Client_GetPlayerListRelationFlags(/*eax*/ uint32_t dwPlayerUnitId, /*ecx*/ uint32_t dwOtherUnitId)
{
    if (dwPlayerUnitId == dwOtherUnitId)
    {
        return 0;
    }
    D2PlayerListNodeStrc* pEntry = D2Client_FindPlayerListEntry(dwPlayerUnitId, dwOtherUnitId);
    if (!pEntry)
    {
        return 0;
    }
    return (uint16_t)pEntry->dwFlags;
}

// 1.12a: 0x6FAE0DD0 (custom: eax = dwPlayerUnitId, ecx = dwOtherUnitId, edi = dwFlagMask)
uint32_t D2Client_TestPlayerListRelationFlag(/*eax*/ uint32_t dwPlayerUnitId, /*ecx*/ uint32_t dwOtherUnitId, /*edi*/ uint32_t dwFlagMask)
{
    if (dwPlayerUnitId == dwOtherUnitId)
    {
        return 0;
    }
    D2PlayerListNodeStrc* pEntry = D2Client_FindPlayerListEntry(dwPlayerUnitId, dwOtherUnitId);
    if (!pEntry)
    {
        return 0;
    }
    return pEntry->dwFlags & dwFlagMask;
}

// 1.12a: 0x6FAE0E10 (custom: eax = dwOtherUnitId, edx = dwPlayerUnitId, edi = dwFlagMask, stack arg_0 = bSet)
void __stdcall D2Client_TogglePlayerListRelationFlag(/*edx*/ uint32_t dwPlayerUnitId, /*eax*/ uint32_t dwOtherUnitId, /*edi*/ uint32_t dwFlagMask, BOOL bSet)
{
    D2PlayerListNodeStrc* pEntry = D2Client_FindPlayerListEntry(dwPlayerUnitId, dwOtherUnitId);
    if (!pEntry)
    {
        if (bSet)
        {
            // D2Client_AddPlayerToList(dwPlayerUnitId, dwOtherUnitId, dwFlagMask);
        }
        return;
    }

    if (bSet)
    {
        pEntry->dwFlags |= dwFlagMask;
    }
    else
    {
        pEntry->dwFlags &= ~dwFlagMask;
    }
}

// 1.12a: 0x6FAE0E60 (custom: eax = dwPlayerUnitId, ebx = dwFlags, stack arg_0 = dwOtherUnitId)
void __stdcall D2Client_AddPlayerToList(/*eax*/ uint32_t dwPlayerUnitId, /*ebx*/ uint32_t dwFlags, uint32_t dwOtherUnitId)
{
    // D2RosterUnitStrc* pRoster = D2Client_FindRosterUnitById(dwPlayerUnitId); // 0x6FB161D0
    D2RosterUnitStrc* pRoster = nullptr;
    if (!pRoster)
    {
        return;
    }

    D2PlayerListStrc* pList = pRoster->pPlayerList;
    if (!pList)
    {
        // FOG_DisplayWarning("pPlayerList", "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 113);
        return;
    }

    // D2PlayerListNodeStrc* pNode = (D2PlayerListNodeStrc*)FOG_AllocClientMemory(sizeof(D2PlayerListNodeStrc), "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 116, 0);
    D2PlayerListNodeStrc* pNode = nullptr;
    if (pNode)
    {
        pNode->dwUnitId = dwOtherUnitId;
        pNode->dwFlags = dwFlags;
        pNode->pNext = pList->pFirst;
        pList->pFirst = pNode;
    }
}

// ============================================================================
// PART 3: Speedbar & Skill Hotkey UI (0x6FB076B0..0x6FB094C0)
// ============================================================================

// 1.12a: 0x6FB076B0 (__stdcall)
int32_t __stdcall D2Client_IsSpeedBarOpen()
{
    return gbSpeedBarOpen_6FBCBC50;
}

// 1.12a: 0x6FB076C0 (custom: ecx = nX, edx = nY, stack arg_0 = nColor, arg_4 = nQuantity)
void __stdcall D2Client_DrawSkillIconQuantityOverlay(/*ecx*/ int32_t nX, /*edx*/ int32_t nY, int32_t nColor, int32_t nQuantity)
{
    // Formats nQuantity into "%d", selects font 6 (0x6FB87074), and draws at (nX, nY) via D2Win_10117.
}

// 1.12a: 0x6FB07770 (custom: eax = pUnit, edi = pSkill)
int32_t D2Client_GetSkillIconTintColor(/*eax*/ D2UnitStrc* pUnit, /*edi*/ D2SkillStrc* pSkill)
{
    // Calls SKILLS_GetSkillUseState (D2Common_10491); if > 1 returns 1 (red),
    // else if skill charges > 0 returns 3 (blue), else returns 0 (normal).
    return 0;
}

// 1.12a: 0x6FB077B0 (custom: eax = nMouseX, ecx = nRectBottom, edx = nRectLeft, stack arg_0 = nMouseY)
BOOL __stdcall D2Client_PtInSkillIconRect(/*eax*/ int32_t nMouseX, /*ecx*/ int32_t nRectBottom, /*edx*/ int32_t nRectLeft, int32_t nMouseY)
{
    return (nMouseX >= nRectLeft && nMouseX <= nRectLeft + 48 && nMouseY <= nRectBottom && nMouseY >= nRectBottom - 48);
}

// 1.12a: 0x6FB077F0 (__stdcall)
void __stdcall D2Client_FreeSkillIconCellFiles()
{
    gbSpeedBarOpen_6FBCBC50 = 0;
    // Frees class skill icon cell files at 0x6FBCBC6C..0x6FBCBC88 and general skill icons at 0x6FBCBC68.
}

// 1.12a: 0x6FB078A0 (custom: eax = nHotkeySlot, stack arg_0 = nSkillId, arg_4 = bLeftHand, arg_8 = dwOwnerGUID)
void __stdcall D2Client_AssignSkillHotkey(/*eax*/ int32_t nHotkeySlot, int32_t nSkillId, BOOL bLeftHand, uint32_t dwOwnerGUID)
{
    // Updates hotkey tables at 0x6FBCBB50 / 0x6FBCBB90 (right) or 0x6FBCBB10 / 0x6FBCBCD8 (left)
    // and sends packet 0x51 (D2Client_SendAssignSkillHotkeyPacket, 0x6FAEC6B0).
}

// 1.12a: 0x6FB07930 (custom: ecx = bLeftHand, edx = nSkillId, stack arg_0 = dwOwnerGUID)
void __stdcall D2Client_SelectActiveSkill(/*ecx*/ BOOL bLeftHand, /*edx*/ int32_t nSkillId, uint32_t dwOwnerGUID)
{
    // Checks SKILLS_GetSkillById, verifies non-passive, and sends packet 0x3C (0x6FAEC680).
}

// 1.12a: 0x6FB079E0 (custom: ebx = nHotkeySlot)
BOOL D2Client_BindSkillHotkeyFromSpeedBarOrActive(/*ebx*/ int32_t nHotkeySlot)
{
    // Binds hotkey slot 0..15 to either the hovered speedbar cell or the active left/right skill.
    return FALSE;
}

// 1.12a: 0x6FB07B80 (custom: esi = nSkillId, stack arg_0 = dwOwnerGUID)
void __stdcall D2Client_ClearDepletedItemSkillHotkeys(/*esi*/ int32_t nSkillId, uint32_t dwOwnerGUID)
{
    // Unbinds any hotkey slots bound to (nSkillId, dwOwnerGUID) and resets depleted active skills to Attack (0).
}

// 1.12a: 0x6FB07C20 (custom: eax = nDirectionDelta)
void D2Client_CycleSkillHotkeys(/*eax*/ int32_t nDirectionDelta)
{
    // Cycles through assigned skill hotkeys in direction nDirectionDelta (+1 or -1).
}

// 1.12a: 0x6FB07CC0 (custom: eax = nX, ebx = nSkillId, stack arg_0 = nY, arg_4 = dwOwnerGUID)
void __stdcall D2Client_DrawSkillButtonTooltip(/*eax*/ int32_t nX, /*ebx*/ int32_t nSkillId, int32_t nY, uint32_t dwOwnerGUID)
{
    // Draws skill popup description box via D2Win_10129.
}

// 1.12a: 0x6FB07E80 (__stdcall)
void __stdcall D2Client_AutoAssignDefaultSkillHotkeys()
{
    // Assigns default hotkeys on initial character creation for skills with witypes.
}

// 1.12a: 0x6FB07F10 (__stdcall)
void __stdcall D2Client_CheckDepletedQuantitySkills()
{
    // Checks all skills on gpPlayerUnit; if SKILLS_CheckSkillQuantity returns <= 0, clears hotkeys.
}

// 1.12a: 0x6FB07F80 (custom: edi = nSkillId, stack arg_0 = bLeftHand, arg_4 = dwOwnerGUID, arg_8 = nRightX, arg_C = nBottomY)
void __stdcall D2Client_DrawSkillHotkeyBindingLabel(/*edi*/ int32_t nSkillId, BOOL bLeftHand, uint32_t dwOwnerGUID, int32_t nRightX, int32_t nBottomY)
{
    // Draws the bound hotkey name (e.g. F1..F8) in the corner of a skill icon.
}

// 1.12a: 0x6FB080B0 (custom: eax = nIconCelIndex, edx = bCharClassIcon, stack arg_0 = nCharClass)
D2CellFileStrc* __stdcall D2Client_GetSkillIconCellFile(/*eax*/ int32_t nIconCelIndex, /*edx*/ uint8_t bCharClassIcon, int32_t nCharClass)
{
    // Returns the loaded DC6 cell file handle for a general or class-specific skill icon.
    return nullptr;
}

// 1.12a: 0x6FB08110 (__stdcall)
void __stdcall D2Client_InitSpeedBarAndSkillIcons()
{
    // Resets hotkey arrays and loads data\global\ui\Spells\skltree and CoSkillicon cell files.
}

// 1.12a: 0x6FB08300 (custom: ecx = nX, edx = nY)
void __fastcall D2Client_DrawRightSkillHotkeyLabel(int32_t nX, int32_t nY)
{
    // Draws hotkey text on the right-hand active skill button.
}

// 1.12a: 0x6FB086D0 (custom: ecx = nX, edx = nY)
void __fastcall D2Client_DrawLeftSkillHotkeyLabel(int32_t nX, int32_t nY)
{
    // Draws hotkey text on the left-hand active skill button.
}

// 1.12a: 0x6FB08AA0 (custom: eax = nY, edx = pSkill, stack arg_0 = nX, arg_4 = bPressed)
void __stdcall D2Client_DrawSpeedBarSkillIcon(/*eax*/ int32_t nY, /*edx*/ D2SkillStrc* pSkill, int32_t nX, BOOL bPressed)
{
    // Draws a single skill icon in the speedbar popup, plus charges/quantity overlay.
}

// 1.12a: 0x6FB08BF0 (custom: edi = bRightHand, stack arg_0 = nX, arg_4 = nY)
void __stdcall D2Client_DrawActiveSkillButtonIcon(/*edi*/ BOOL bRightHand, int32_t nX, int32_t nY)
{
    // Draws the active left or right skill icon on the bottom HUD control panel.
}

// 1.12a: 0x6FB08D80 (custom: ecx = bRightHand, edx = nX, stack arg_0 = nY)
void __fastcall D2Client_DrawActiveSkillButtonTooltip(BOOL bRightHand, int32_t nX, int32_t nY)
{
    // Draws hover tooltip for the active left or right skill button.
}

// 1.12a: 0x6FB08E00 (custom: stack arg_0 = bRender)
void __stdcall D2Client_LayoutAndRenderSpeedBarPopup(BOOL bRender)
{
    // Populates the 64-slot speedbar grid (0x6FBCBC90) and optionally renders the icons & hover tooltip.
    // Uses 4-case switch jump table at 0x6FB09130 (skill use state 0..3).
}

// 1.12a: 0x6FB09140 (__stdcall)
void __stdcall D2Client_CloseSpeedBarIfOpen()
{
    if (gbSpeedBarOpen_6FBCBC50)
    {
        gbSpeedBarOpen_6FBCBC50 = 0;
        D2Client_LayoutAndRenderSpeedBarPopup(FALSE);
    }
}

// 1.12a: 0x6FB09160 (__fastcall)
void __fastcall D2Client_ToggleSpeedBarPopup(BOOL bRightHand)
{
    if (gbSpeedBarOpen_6FBCBC50)
    {
        if (bRightHand != gbSpeedBarRightSide_6FBCBC54)
        {
            gbSpeedBarRightSide_6FBCBC54 = bRightHand;
            *(int32_t*)0x6FBCBD20 = -1;
            D2Client_LayoutAndRenderSpeedBarPopup(FALSE);
            return;
        }
        gbSpeedBarOpen_6FBCBC50 = 0;
    }
    else
    {
        // Close overlapping panels and open speedbar
        gbSpeedBarOpen_6FBCBC50 = 1;
        gbSpeedBarRightSide_6FBCBC54 = bRightHand;
        *(int32_t*)0x6FBCBD20 = -1;
    }
    D2Client_LayoutAndRenderSpeedBarPopup(FALSE);
}

// 1.12a: 0x6FB09210 (__stdcall)
BOOL __stdcall D2Client_HandleSpeedBarMouseClick(int32_t nMouseX, int32_t nMouseY)
{
    // Checks if click hits one of the speedbar popup icons; if so, selects that skill and closes speedbar.
    return FALSE;
}

// 1.12a: 0x6FB094B0 (__stdcall)
void __stdcall D2Client_RenderSpeedBarPopup()
{
    if (gbSpeedBarOpen_6FBCBC50)
    {
        D2Client_LayoutAndRenderSpeedBarPopup(TRUE);
    }
}

// ============================================================================
// PART 4: Player.cpp (0x6FB094C0..0x6FB0D540)
// Source File: ..\Source\D2Client\UNIT\Player.cpp
// ============================================================================

// 1.12a: 0x6FB094C0 (__stdcall)
void __stdcall D2Client_GetLocalPlayerSaveGfxInfo(uint8_t* pComponentsOut, uint8_t* pColorOut)
{
    if (!gpPlayerUnit_6FBCC3D0)
    {
        *pComponentsOut = 0;
        *pColorOut = 0;
        return;
    }
    // D2Common_11048_GetItemSaveGfxInfo(gpPlayerUnit_6FBCC3D0, pComponentsOut, pColorOut);
}

// 1.12a: 0x6FB09500 (custom: ecx = pUnit, edx = pComponentsOut, stack arg_0 = pColorOut)
void __fastcall D2Client_GetUnitSaveGfxInfo(D2UnitStrc* pUnit, uint8_t* pComponentsOut, uint8_t* pColorOut)
{
    if (!pUnit)
    {
        *pComponentsOut = 0;
        *pColorOut = 0;
        return;
    }
    // D2Common_11048_GetItemSaveGfxInfo(pUnit, pComponentsOut, pColorOut);
}

// 1.12a: 0x6FB09540 (custom: ebx = nBodyLoc, esi = pPlayerUnit)
int32_t D2Client_GetPlayerEquippedItemComponent(/*ebx*/ int32_t nBodyLoc, /*esi*/ D2UnitStrc* pPlayerUnit)
{
    if (!pPlayerUnit)
    {
        return 0;
    }

    D2InventoryStrc* pInventory = *(D2InventoryStrc**)((uint8_t*)pPlayerUnit + 0x60);
    if (!pInventory)
    {
        // FOG_DisplayWarning("ptPlayer->ptInventory", "..\\Source\\D2Client\\UNIT\\Player.cpp", 214);
        return 0;
    }

    // D2UnitStrc* pItem = INVENTORY_GetItemFromBodyLoc(pInventory, nBodyLoc); // D2Common_10434
    // Checks ITEMS_GetComponent(pItem) and handles dual-wielding RARM(4)/LARM(5) component 5 -> 6.
    return 0;
}

// 1.12a: 0x6FB095C0 (__stdcall)
void __stdcall D2Client_ClearPlayerMouseActionLock()
{
    *(int32_t*)0x6FBCB704 = 0;
}

// 1.12a: 0x6FB095D0 (custom: eax = nTargetX, ecx = nTargetY, edx = pTargetUnit, ebx = pSkill, edi = pPlayerUnit)
BOOL D2Client_IsPlayerSkillWithinRange(/*eax*/ int32_t nTargetX, /*ecx*/ int32_t nTargetY, /*edx*/ D2UnitStrc* pTargetUnit, /*ebx*/ D2SkillStrc* pSkill, /*edi*/ D2UnitStrc* pPlayerUnit)
{
    // Checks distance to pTargetUnit or (nTargetX, nTargetY) against skill range or weapon range.
    return FALSE;
}

// 1.12a: 0x6FB09650 (custom: ebx = pActionState)
BOOL D2Client_ValidatePlayerInputAction(/*ebx*/ void* pActionState)
{
    // Validates action state, checks if skill is usable in town, and verifies target unit state.
    return TRUE;
}

// 1.12a: 0x6FB096C0 (custom: eax = pSkill, esi = pPlayerUnit)
BOOL D2Client_CanUseSkillInCurrentRoom(/*eax*/ D2SkillStrc* pSkill, /*esi*/ D2UnitStrc* pPlayerUnit)
{
    // Returns TRUE if skill has InTown flag or player is not in town (DUNGEON_IsRoomInTown).
    return TRUE;
}

// 1.12a: 0x6FB09710 (custom: eax = pOutX, ecx = pOutY, ebx = pUnit)
BOOL D2Client_ComputePlayerStepMoveTarget(/*eax*/ int32_t* pOutX, /*ecx*/ int32_t* pOutY, /*ebx*/ D2UnitStrc* pUnit)
{
    // Computes next step movement target along mouse vector or towards target unit.
    return FALSE;
}

// 1.12a: 0x6FB098F0 (custom: esi = pPlayerUnit)
void D2Client_OnPlayerDeathCleanup(/*esi*/ D2UnitStrc* pPlayerUnit)
{
    // Clears unit collision footprint, clears selection if local player, sets UNITFLAG_DEAD (0x10000),
    // and sets STAT_HITPOINTS (6) to 0.
}

// 1.12a: 0x6FB09990 (custom: esi = pUnit)
void D2Client_FreeUnitHoverText(/*esi*/ D2UnitStrc* pUnit)
{
    void* pChatMsg = *(void**)((uint8_t*)pUnit + 0xA8);
    if (pChatMsg)
    {
        // D2Win_10064_FreeHoverText(pChatMsg);
        *(void**)((uint8_t*)pUnit + 0xA8) = nullptr;
    }
}

// 1.12a: 0x6FB099B0 (__stdcall)
void __stdcall D2Client_UpdateLocalPlayerDungeonPops()
{
    if (!gpPlayerUnit_6FBCC3D0)
    {
        return;
    }
    // Calls UNITS_GetRoom, queries subtile X/Y, and updates DUNGEON_UpdatePops (D2Common_11079).
}

// 1.12a: 0x6FB09A30 (custom: eax = pUnit)
void D2Client_UpdateUnitLightColor(/*eax*/ D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return;
    }
    // Reads STATLIST_UnitGetStatValue(pUnit, STAT_ITEM_LIGHTCOLOR (90), 0),
    // unpacks RGB or palette index via D2CMP_10075, and updates pUnit->pLight (+0x70).
}

// 1.12a: 0x6FB09A90 (custom: esi = pUnit, stack arg_0 = nX, arg_4 = nY)
BOOL __stdcall D2Client_SetUnitPositionAndResetPath(/*esi*/ D2UnitStrc* pUnit, int32_t nX, int32_t nY)
{
    // Repositions unit in room via D2Client_TeleportUnit (0x6FACE880), updates PATH_SetPosition,
    // and updates local player pops if pUnit == gpPlayerUnit_6FBCC3D0.
    return FALSE;
}

// 1.12a: 0x6FB09BB0 (custom: eax = pPlayerUnit, edx = pActionState)
BOOL D2Client_IsPlayerAlreadyMovingToTarget(/*eax*/ D2UnitStrc* pPlayerUnit, /*edx*/ void* pActionState)
{
    // Checks if player is in PLRMODE_WALK (2) or PLRMODE_RUN (3) and already moving to the requested target.
    return FALSE;
}

// 1.12a: 0x6FB09C10 (custom: eax = pUnit)
void D2Client_UpdateUnitLightRadius(/*eax*/ D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return;
    }
    // Computes clamped light radius (0..18) from STAT_ITEM_LIGHTRADIUS (89) + base 13,
    // and calls D2Client_SetLightRadius(pUnit->pLight, nRadius) (0x6FB65DC0).
}

// 1.12a: 0x6FB09C70 (custom: eax = pTargetMonster, ecx = nMouseX, edx = nMouseY)
BOOL D2Client_HandleMercenaryOrPetRightClick(/*eax*/ D2UnitStrc* pTargetMonster, /*ecx*/ int32_t nMouseX, /*edx*/ int32_t nMouseY)
{
    // Handles right-clicking on the local player's mercenary (opens merc inventory) or iron golem / pet.
    return FALSE;
}

// 1.12a: 0x6FB09D30 (__stdcall)
// Stat change callback registered in STATLIST_AllocStatListEx for client player units.
// Uses indexed jump table at 0x6FB09EA4 (5 entries) / index table at 0x6FB09EBC (stats 89..222).
void __stdcall D2Client_PlayerStatChangeCallback(D2UnitStrc* pUnit, int32_t nStatId, int32_t nLayer, int32_t nOldValue, int32_t nNewValue)
{
    switch (nStatId)
    {
    case 89:  // STAT_ITEM_LIGHTRADIUS
    case 216: // STAT_ITEM_LIGHTRADIUS_PERLEVEL (or adjacent light stat)
    case 217:
        D2Client_UpdateUnitLightRadius(pUnit);
        break;

    case 90:  // STAT_ITEM_LIGHTCOLOR
        D2Client_UpdateUnitLightColor(pUnit);
        break;

    case 97:  // STAT_ITEM_NONCLASSSKILL (oskill)
    case 107: // STAT_ITEM_SINGLESKILL
    case 219:
        if (nNewValue > 0 && nOldValue <= 0)
        {
            // SKILLS_AssignSkill(pUnit, nLayer, 0, 0, "..\\Source\\D2Client\\UNIT\\Player.cpp", 2681);
        }
        else if (nNewValue <= 0 && nOldValue > 0)
        {
            // SKILLS_RemoveSkill(pUnit, nLayer, "..\\Source\\D2Client\\UNIT\\Player.cpp", 2685);
            if (pUnit == gpPlayerUnit_6FBCC3D0)
            {
                D2Client_ClearDepletedItemSkillHotkeys(nLayer, (uint32_t)-1);
            }
        }
        break;

    case 151: // STAT_ITEM_AURA
        if (nNewValue <= 0 && nOldValue > 0)
        {
            // Clears aura state matching skill via D2Common_10954 / STATLIST_SetUnitState(..., 0)
        }
        break;

    default:
        break;
    }
}

// 1.12a: 0x6FB09F30 (custom: eax = pPlayerUnit)
void D2Client_ClearPlayerBearWolfTransformState(/*eax*/ D2UnitStrc* pPlayerUnit)
{
    if (!pPlayerUnit)
    {
        return;
    }
    // Resets player mode to neutral if in walk/run, refreshes inventory items, and updates light radius.
}

// 1.12a: 0x6FB09F80 (custom: ebx = pSkill, edi = pPlayerUnit)
BOOL D2Client_CheckPlayerSkillUseStateAndPlayBark(/*ebx*/ D2SkillStrc* pSkill, /*edi*/ D2UnitStrc* pPlayerUnit)
{
    // Calls SKILLS_GetSkillUseState (D2Common_10491); if unusable, plays class speech bark ("Not enough mana", etc.).
    return TRUE;
}

// 1.12a: 0x6FB0A020 (custom: eax = pActionState, edi = pPlayerUnit)
BOOL D2Client_ValidatePlayerActionSkillUseState(/*eax*/ void* pActionState, /*edi*/ D2UnitStrc* pPlayerUnit)
{
    // Validates that the skill in pActionState can be executed by pPlayerUnit.
    return TRUE;
}

// 1.12a: 0x6FB0A090 (__stdcall)
BOOL __stdcall D2Client_AlwaysTrueSkillHook()
{
    return TRUE;
}

// 1.12a: 0x6FB0A0A0 (__stdcall)
void __stdcall D2Client_RefreshMonsterUnitsWithState63InPlayerRooms()
{
    // Iterates adjacent rooms of gpPlayerUnit and refreshes units having state 63.
}

// 1.12a: 0x6FB0A120 (custom: esi = pPlayerUnit)
void D2Client_FreePlayerUnitClientSide(/*esi*/ D2UnitStrc* pPlayerUnit)
{
    if (!pPlayerUnit)
    {
        return;
    }

    for (int i = 0; i < 8; ++i)
    {
        if (gPlayerUnits_6FBCB6D4[i] == pPlayerUnit)
        {
            gPlayerUnits_6FBCB6D4[i] = nullptr;
            break;
        }
    }

    if (*(void**)((uint8_t*)pPlayerUnit + 0x2C))
    {
        // PATH_FreeDynamicPath(nullptr, pPlayerUnit->pDynamicPath); // D2Common_10157
        *(void**)((uint8_t*)pPlayerUnit + 0x2C) = nullptr;
    }

    // D2Client_ClearInventory(pPlayerUnit); // 0x6FB487B0
    // INVENTORY_FreeInventory(pPlayerUnit->pInventory); // D2Common_10361
    *(void**)((uint8_t*)pPlayerUnit + 0x60) = nullptr;

    if (*(void**)((uint8_t*)pPlayerUnit + 0x70))
    {
        // D2Client_FreeLight(pPlayerUnit->pLight); // 0x6FB665B0
        *(void**)((uint8_t*)pPlayerUnit + 0x70) = nullptr;
    }

    // UNITS_FreePlayerData(nullptr, pPlayerUnit); // D2Common_10631
}

// 1.12a: 0x6FB0A1D0 (custom: esi = pPlayerUnit)
void D2Client_InitPlayerUnitClientSide(/*esi*/ D2UnitStrc* pPlayerUnit)
{
    if (!pPlayerUnit)
    {
        return;
    }

    for (int i = 0; i < 8; ++i)
    {
        if (!gPlayerUnits_6FBCB6D4[i])
        {
            gPlayerUnits_6FBCB6D4[i] = pPlayerUnit;
            break;
        }
    }

    // PATH_AllocDynamicPath(nullptr, pPlayerUnit->pSeed, 0, 0, pPlayerUnit, 0); // D2Common_10152
    // INVENTORY_AllocInventory(nullptr, pPlayerUnit); // D2Common_10461
    // SKILLS_InitSkillList(pPlayerUnit); // D2Common_11062
    // STATLIST_AllocStatListEx(pPlayerUnit, 6, D2Client_PlayerStatChangeCallback, pPlayerUnit); // D2Common_10323
    // STATLIST_SetUnitStat(pPlayerUnit, STAT_VELOCITYPERCENT (67), 100, 0);
    // STATLIST_SetUnitStat(pPlayerUnit, STAT_ATTACKRATE (68), 100, 0);
    // STATLIST_SetUnitStat(pPlayerUnit, STAT_OTHER_ANIMRATE (69), 100, 0);
    // UNITS_InitPlayerData(pPlayerUnit); // D2Common_10633
}

// 1.12a: 0x6FB0A340 (custom: esi = pCorpseUnit)
void D2Client_InitPlayerCorpseEquipmentAndColor(/*esi*/ D2UnitStrc* pCorpseUnit)
{
    // Links player corpse to owner in roster, clones equipped body items onto corpse inventory,
    // and updates corpse composite appearance and light radius.
}

// 1.12a: 0x6FB0A570 (custom: eax = pActionState, ecx = pPlayerUnit)
int32_t __fastcall D2Client_PreparePlayerSkillInputAction(D2UnitStrc* pPlayerUnit, void* pActionState)
{
    // Validates skill on pPlayerUnit, checks range, and initiates walk/run approach or skill cast.
    return 0;
}

// 1.12a: 0x6FB0A700 (custom: esi = pPlayerUnit)
void D2Client_SetPlayerNeutralModeAndSyncPos(/*esi*/ D2UnitStrc* pPlayerUnit)
{
    // Transitions pPlayerUnit to PLRMODE_NEUTRAL (1) or PLRMODE_TOWNNEUTRAL (5) and syncs path position.
}

// 1.12a: 0x6FB0A7C0 (custom: eax = pPlayerUnit)
void D2Client_TriggerPlayerUsedSkillEnd(/*eax*/ D2UnitStrc* pPlayerUnit)
{
    // Invokes client skill end handler if a used skill is active on pPlayerUnit.
}

// 1.12a: 0x6FB0A820 (custom: eax = pModeChangeInfo, esi = pPlayerUnit)
// Central mode transition handler for UNIT_PLAYER (modes 0..25).
// Uses 16-entry jump table at 0x6FB0AD08 with 26-byte index table at 0x6FB0AD48.
void D2Client_ExecutePlayerModeTransition(/*eax*/ void* pModeChangeInfo, /*esi*/ D2UnitStrc* pPlayerUnit)
{
    int32_t nMode = *(int32_t*)((uint8_t*)pModeChangeInfo + 0x04);
    switch (nMode)
    {
    case 0:  // PLRMODE_DEATH
        // Plays death sound, clears path/states, calls D2Client_OnPlayerDeathCleanup
        break;
    case 1:  // PLRMODE_NEUTRAL
    case 5:  // PLRMODE_TOWNNEUTRAL
        D2Client_SetPlayerNeutralModeAndSyncPos(pPlayerUnit);
        break;
    case 2:  // PLRMODE_WALK
    case 6:  // PLRMODE_TOWNWALK
    case 25: // Knockback / special walk
        // Configures walk velocity and path step
        break;
    case 3:  // PLRMODE_RUN
        // Configures run velocity and path step
        break;
    case 4:  // PLRMODE_GETHIT
    case 10: // PLRMODE_BLOCK
    case 11: // PLRMODE_CAST
    case 12: // PLRMODE_THROW
    case 13: // PLRMODE_KICK
    case 14: // PLRMODE_SKILL1
    case 15: // PLRMODE_SKILL2
    case 16: // PLRMODE_SKILL3
    case 17: // PLRMODE_SKILL4
        // Sets weapon/skill animation speed and changes mode
        break;
    case 7:  // PLRMODE_ATTACK1
    case 8:  // PLRMODE_ATTACK2
    case 9:  // PLRMODE_BLOCK/SEQUENCE
    case 18: // PLRMODE_DEAD
    case 19: // PLRMODE_SEQUENCE
    case 20: // PLRMODE_KNOCKBACK
    case 21: // Whirlwind / custom client mode
    case 22: // Leap / custom client mode
    case 23: // Charge / custom client mode
    case 24: // LeapAttack / custom client mode
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FB0AD70 (__stdcall)
void __stdcall D2Client_CheckLocalPlayerStaminaDepleted()
{
    if (!gpPlayerUnit_6FBCC3D0)
    {
        return;
    }
    // If local player is running (mode 3) and STAT_STAMINA (10) == 0, switches to walk mode (2 or 6).
}

// 1.12a: 0x6FB0AEE0 (custom: ecx = pPlayerUnit)
void __fastcall D2Client_ProcessPlayerNextQueuedInputAction(D2UnitStrc* pPlayerUnit)
{
    // Dispatches queued input action after current attack/cast animation completes.
}

// 1.12a: 0x6FB0B000 (custom: eax = pActionState, ecx = pPlayerUnit)
void D2Client_StartPlayerSkillOrInteraction(/*eax*/ void* pActionState, /*ecx*/ D2UnitStrc* pPlayerUnit)
{
    // Initiates skill execution or unit interaction for pPlayerUnit.
}

// 1.12a: 0x6FB0B050 (custom: ebx = pActionState, edi = pPlayerUnit)
int32_t D2Client_ProcessLocalPlayerActionStateMachine(/*ebx*/ void* pActionState, /*edi*/ D2UnitStrc* pPlayerUnit)
{
    // Main local player input/movement/attack/cast state machine.
    return 0;
}

// 1.12a: 0x6FB0BA90 (custom: eax = pActionState, ecx = pPlayerUnit)
void D2Client_ExecuteLocalPlayerActionStep(/*eax*/ void* pActionState, /*ecx*/ D2UnitStrc* pPlayerUnit)
{
    // Evaluates D2Client_ProcessLocalPlayerActionStateMachine and transitions to neutral if finished.
}

// 1.12a: 0x6FB0BB10 (custom: eax = pActionState, edi = pPlayerUnit)
// Uses 5-case jump table at 0x6FB0C1A0.
void D2Client_HandleLocalPlayerMouseInputState(/*eax*/ void* pActionState, /*edi*/ D2UnitStrc* pPlayerUnit)
{
    // Handles continuous mouse hold, target tracking, object/item/monster interaction, and skill casting.
}

// 1.12a: 0x6FB0C1C0 (custom: edi = pPlayerUnit)
void D2Client_UpdatePlayerActionQueueOnModeEnd(/*edi*/ D2UnitStrc* pPlayerUnit)
{
    // Advances queued player action state when current animation mode finishes.
}

// 1.12a: 0x6FB0C310 (__stdcall)
void __stdcall D2Client_CancelLocalPlayerActiveAction()
{
    // Cancels current local player mouse/skill action and returns player to neutral mode.
}

// 1.12a: 0x6FB0C350 (custom: ecx = pActionState, stack arg_0..arg_14)
int32_t __fastcall D2Client_SubmitPlayerInputAction(
    void* pActionState,
    int32_t nActionType,
    int32_t nArg1,
    D2UnitStrc* pTargetUnit,
    int32_t nTargetX,
    int32_t nTargetY,
    int32_t nFlags)
{
    // Populates action state structure and dispatches local player action.
    return 0;
}

// 1.12a: 0x6FB0C530 (__fastcall)
int32_t __fastcall D2Client_CommandLocalPlayerMoveToCoords(int32_t nActionType, int32_t nX, int32_t nY, int32_t nFlags)
{
    // Issues a move-to-coordinates command on gpPlayerUnit_6FBCC3D0.
    return 0;
}

// 1.12a: 0x6FB0C610 (__fastcall)
int32_t __fastcall D2Client_CommandLocalPlayerInteractUnit(int32_t nActionType, D2UnitStrc* pTargetUnit, int32_t nFlags)
{
    // Issues an interact/attack-unit command on gpPlayerUnit_6FBCC3D0.
    return 0;
}

// 1.12a: 0x6FB0C6F0 (__fastcall)
int32_t __fastcall D2Client_CommandLocalPlayerUseSkillAtCoords(int32_t nActionType, int32_t nX, int32_t nY, int32_t nFlags)
{
    // Issues a cast-skill-at-coords command on gpPlayerUnit_6FBCC3D0.
    return 0;
}

// 1.12a: 0x6FB0C7E0 (__fastcall)
int32_t __fastcall D2Client_CommandLocalPlayerUseSkillOnUnit(int32_t nActionType, D2UnitStrc* pTargetUnit, int32_t nFlags)
{
    // Issues a cast-skill-on-unit command on gpPlayerUnit_6FBCC3D0.
    return 0;
}

// 1.12a: 0x6FB0CBB0 (__stdcall)
void __stdcall D2Client_UpdateLocalPlayerHeldMouseInput()
{
    // Polls held mouse button state for continuous movement or skill casting.
}

// 1.12a: 0x6FB0CC30 (__fastcall)
void __fastcall D2Client_SetPlayerInputTargetCoords(int32_t nX, int32_t nY)
{
    // Updates current mouse world target coordinates for local player movement.
}

// 1.12a: 0x6FB0CCB0 (custom: eax = pTargetUnit)
void D2Client_SetPlayerInputTargetUnit(/*eax*/ D2UnitStrc* pTargetUnit)
{
    // Updates current mouse target unit for local player action.
}

// 1.12a: 0x6FB0CDD0 (__stdcall)
void __stdcall D2Client_OnMouseButtonReleased()
{
    // Clears held mouse state and stops continuous repeat actions.
}

// 1.12a: 0x6FB0CE80 (__fastcall)
// Uses 5-case jump table at 0x6FB0D090.
int32_t __fastcall D2Client_DispatchMouseClickToPlayerAction(int32_t nClickType, int32_t nScreenX, int32_t nScreenY, int32_t nFlags)
{
    // Converts screen click to world coords / hovered unit and dispatches appropriate player command.
    return 0;
}

// 1.12a: 0x6FB0D0B0 (custom: eax = pPlayerUnit)
void D2Client_OnLocalPlayerRoomChanged(/*eax*/ D2UnitStrc* pPlayerUnit)
{
    if (pPlayerUnit != gpPlayerUnit_6FBCC3D0)
    {
        return;
    }
    // Updates portal flags, tile animation, town walk/run switching, and LOS draw.
}

// 1.12a: 0x6FB0D140 (__fastcall)
// Per-tick update handler for UNIT_PLAYER.
// Uses 4-case jump table at 0x6FB0D52C (frame events 0..3).
void __fastcall D2Client_UpdatePlayerUnitPerTick(D2UnitStrc* pPlayerUnit)
{
    if (!pPlayerUnit)
    {
        return;
    }
    // 1. Steps movement via D2Client_StepUnitMotion (0x6FAF1B10) for walk/run/knockback/leap/whirlwind
    // 2. Updates Blaze (state 19) / Inferno ground missiles
    // 3. Dispatches skill frame events (1 = melee/srvdofunc, 2 = missile, 3 = sound)
    // 4. Handles animation end transitions and queued input actions
    // 5. Checks room change (D2Client_OnLocalPlayerRoomChanged), stamina depletion, and collision unstuck
}

// ============================================================================
// PART 5: Viewport, Camera & Debug Room Overlays (0x6FB0D540..0x6FB0DFF0)
// ============================================================================

// 1.12a: 0x6FB0D540 (custom: ecx = nSubtileX, edx = nSubtileY, stack arg_0 = pScreenX, arg_4 = pScreenY)
void __fastcall D2Client_SubtileToScreenCoords(int32_t nSubtileX, int32_t nSubtileY, int32_t* pScreenX, int32_t* pScreenY)
{
    // Isometric projection: ScreenX = (X - Y) * 16, ScreenY = (X + Y) * 8
    *pScreenX = (nSubtileX - nSubtileY) * 16;
    *pScreenY = (nSubtileX + nSubtileY) * 8;
}

// 1.12a: 0x6FB0D5A0 (custom: ecx = pSubtileX, edx = pSubtileY, stack arg_0 = bApplyViewportOffset)
void __fastcall D2Client_ScreenToSubtileCoords(int32_t* pInOutX, int32_t* pInOutY, BOOL bApplyViewportOffset)
{
    // Converts screen pixel coordinates (with perspective or isometric transform) to world subtile coordinates.
}

// 1.12a: 0x6FB0D680 (custom: eax = nRectFlags, stack arg_0 = pRoomCoords, arg_4 = nColor)
void __stdcall D2Client_DrawRoomRectDebugOutline(/*eax*/ int32_t nRectFlags, int32_t* pRoomCoords, uint8_t nColor)
{
    // Projects room corners to screen space and draws 4 wireframe lines via D2Gfx_10057.
}

// 1.12a: 0x6FB0D6F0 (__stdcall) (spans 0x6FB0D6F0..0x6FB0DB03)
void __stdcall D2Client_RenderDebugRoomAndTileOverlays()
{
    // Renders debug room boundaries, subtile grid, and collision/tile coordinate labels.
}

// 1.12a: 0x6FB0DB10 (__stdcall)
void __stdcall D2Client_UpdateViewportOffsetFromPlayer()
{
    if (!gpPlayerUnit_6FBCC3D0)
    {
        return;
    }
    // Computes player screen position from UNITS_GetPrecisionX / UNITS_GetPrecisionY,
    // centers viewport (gnViewportOffsetX_6FBC21D0 / gnViewportOffsetY_6FBC21CC),
    // and adds screen shake (gnShakeOffsetX_6FBCBDCC / gnShakeOffsetY_6FBCC3F0) if active.
}

// 1.12a: 0x6FB0DC20 (__stdcall)
int32_t __stdcall D2Client_GetViewportOffsetX()
{
    return gnViewportOffsetX_6FBC21D0;
}

// 1.12a: 0x6FB0DC30 (__stdcall)
int32_t __stdcall D2Client_GetViewportOffsetY()
{
    return gnViewportOffsetY_6FBC21CC;
}

// 1.12a: 0x6FB0DC40 (__fastcall)
void __fastcall D2Client_ShiftViewportOffset(int32_t nDeltaX, int32_t nDeltaY)
{
    gnViewportOffsetX_6FBC21D0 += nDeltaX;
    gnViewportOffsetY_6FBC21CC += nDeltaY;
}

// 1.12a: 0x6FB0DC60 (__stdcall)
void __stdcall D2Client_RenderDebugRoomCoordListsAndWarps()
{
    // Renders room coord lists and warp boundaries when debug overlay flags are enabled.
}

// 1.12a: 0x6FB0DF80 (__stdcall)
void __stdcall D2Client_RenderDebugLevelAndRoomOutlines()
{
    // Renders level and room bounding boxes when debug overlay flags are enabled.
}
