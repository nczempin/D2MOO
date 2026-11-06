// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Translation Unit: UI_CmdTbl.cpp
// Source File : ..\Source\D2Client\UI\CmdTbl.cpp
// .text Range : 0x6FB4C580..0x6FB51570 (asm/UI_CmdTbl.asm, 108 PROC blocks)
//               + 0x6FB51570..0x6FB5360F (CmdTbl.cpp tail in asm/UNIT_PlrSkills.asm)
// ImageBase   : 0x6FAB0000
// ============================================================================

#include <windows.h>
#include <stdint.h>

struct D2UnitStrc;
struct D2InventoryStrc;
struct D2SkillStrc;
struct D2SkillsTxt;
struct D2MonStatsTxt;
struct D2CellFileStrc;
struct D2GfxDataStrc;
struct D2RoomStrc;
struct D2StatListStrc;
struct D2RosterUnitStrc;
struct Unicode;

struct D2WindowMsgStrc
{
    HWND hWnd;              // +0x00
    UINT uMsg;              // +0x04
    WPARAM wParam;          // +0x08
    LPARAM lParam;          // +0x0C
    uint32_t dwCommand;     // +0x10
    uint32_t dwKeyFlags;    // +0x14
    int32_t bHandled;       // +0x18
    LRESULT lResult;        // +0x1C
};

typedef void (__fastcall *D2CmdCallback_t)(void);
typedef int32_t (__stdcall *D2MsgHandler_t)(D2WindowMsgStrc* pMsg);

struct D2InputCallbackStrc
{
    int32_t nFlags;         // +0x00
    UINT uMsg;              // +0x04
    D2MsgHandler_t pfnProc; // +0x08
};

// 10-byte packed key binding entry in gKeyBindings (0x6FBB11A0) and gDefaultKeyBindings (0x6FB8D028)
#pragma pack(push, 1)
struct D2KeyBindingStrc
{
    int32_t nCommandId;     // +0x00 (0..56)
    uint16_t wKey;          // +0x04 (VK_* or 0x100..0x104 mouse button, 0xFFFF = unbound)
    int32_t bIsPrimary;     // +0x06 (1 = primary slot, 0 = secondary slot)
};
#pragma pack(pop)

// 12-byte command dispatch descriptor at 0x6FB8D4A0 (57 entries, cmd 0..56)
struct D2CmdDispatchStrc
{
    D2CmdCallback_t pfnOnDown;  // +0x00
    D2CmdCallback_t pfnOnUp;    // +0x04
    int32_t bAllowInChat;       // +0x08
};

// 28-byte cursor sprite descriptor at 0x6FB8D7B8 (7 entries + terminator)
struct D2CursorDescStrc
{
    int32_t nAnimId;            // +0x00
    int32_t bLoop;              // +0x04
    int32_t nTotalFrames;       // +0x08
    int32_t nFrameDeltaFixed8;  // +0x0C
    int32_t nEndFrame;          // +0x10
    void (__fastcall *pfnUpdate)(void); // +0x14
    const char* szCelName;      // +0x18
};

// ============================================================================
// External Imports (Resolved via IAT & ordinal headers)
// ============================================================================
extern "C" {
    // Fog.dll
    void __cdecl FOG_DisplayHalt(const char* szFile, int32_t nLine);                 // Fog.#10024 (0x6FABBF80)
    void __cdecl FOG_Trace(const char* szFormat, ...);                               // Fog.#10029 (0x6FABBF3E)
    void __fastcall FOG_Free(void* pMem, const char* szFile, int32_t nLine, int32_t nPool); // Fog.#10043 (0x6FABBF86)
    void* __fastcall FOG_AllocPool(void* pMemPool, int32_t nSize, const char* szFile, int32_t nLine, int32_t nNull); // Fog.#10045 (0x6FABBF98)
    HANDLE __fastcall FOG_CreateFileA(const char* szPath, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile); // Fog.#10107 (0x6FABBFB0)
    BOOL __fastcall FOG_CloseFile(HANDLE hFile);                                     // Fog.#10109 (0x6FABBFC2)
    BOOL __fastcall FOG_ReadFile(HANDLE hFile, void* pBuf, DWORD nBytesToRead, LPDWORD lpBytesRead, LPOVERLAPPED lpOverlapped); // Fog.#10110 (0x6FABBFBC)
    BOOL __fastcall FOG_WriteFile(HANDLE hFile, const void* pBuf, DWORD nBytesToWrite, LPDWORD lpBytesWritten, LPOVERLAPPED lpOverlapped); // Fog.#10111 (0x6FABBFC8)
    BOOL __fastcall FOG_CreatePathHierarchy(const char* szPath);                     // Fog.#10114 (0x6FABBFA4)
    DWORD __fastcall FOG_GetSavePath(char* szPath, size_t nBufferSize);              // Fog.#10115 (0x6FABBFAA)
    DWORD __fastcall FOG_GetInstallPath(char* szPath, size_t nBufferSize);           // Fog.#10116 (0x6FABBFB6)
    int32_t __fastcall FOG_IsExpansion(void);                                        // Fog.#10227 (0x6FABBF92)
    const char* __cdecl FOG_GetErrorLocation(int32_t nLine);                         // Fog.#10265 (0x6FABBF8C)

    // Storm.dll
    BOOL __stdcall SRegLoadValue(const char* szSection, const char* szKey, BYTE nFlags, int32_t* pValue); // Storm.#423 (0x6FABBE90)
    BOOL __stdcall SRegSaveValue(const char* szSection, const char* szKey, BYTE nFlags, DWORD dwValue);   // Storm.#426 (0x6FABBE72)
    int32_t __stdcall SMsgBreakHandlerChain(void* pMsg);                             // Storm.#301 (0x6FABBE8A)
    size_t __cdecl SStrPrintf(char* szDest, size_t nMax, const char* szFmt, ...);    // Storm.#578 (0x6FABBE96)

    // D2Common.dll
    extern void* sgptDataTables;                                                     // D2Common.#10042
    extern uint32_t gdwBitMasks[];                                                   // D2Common.#10025
    D2RoomStrc* __stdcall DUNGEON_FindRoomBySubtileCoordinates(void* pAct, int32_t nX, int32_t nY); // D2Common.#10099 (0x6FABCA18)
    int32_t __stdcall STATLIST_GetMaxLifeFromUnit(D2UnitStrc* pUnit);                // D2Common.#10136 (0x6FABC58C)
    int32_t __stdcall PATH_GetXPosition(void* pPath);                                // D2Common.#10190 (0x6FABC694)
    void __stdcall PATH_SetTargetPos(void* pPath, int32_t nX, int32_t nY);           // D2Common.#10194 (0x6FABCD42)
    int32_t __stdcall SKILLS_GetParam1(D2SkillStrc* pSkill);                         // D2Common.#10248 (0x6FABC4A8)
    int32_t __stdcall SKILLS_GetParam2(D2SkillStrc* pSkill);                         // D2Common.#10277 (0x6FABC51A)
    D2UnitStrc* __stdcall INV_GetFirstItem(D2InventoryStrc* pInv);                   // D2Common.#10283 (0x6FABC1C6)
    int32_t __stdcall ITEMS_CheckIfSocketable(D2UnitStrc* pItem);                    // D2Common.#10292 (0x6FABCF28)
    void __stdcall SKILLS_SetParam1(D2SkillStrc* pSkill, int32_t nVal);              // D2Common.#10310 (0x6FABC5EC)
    int32_t __stdcall SKILLS_GetParam3(D2SkillStrc* pSkill);                         // D2Common.#10324 (0x6FABC60A)
    int32_t __stdcall UNITS_GetCurrentLifePercentage(D2UnitStrc* pUnit);             // D2Common.#10348 (0x6FABCD48)
    void __stdcall UNITS_SetTargetUnitForDynamicUnit(D2UnitStrc* pUnit, D2UnitStrc* pTarget); // D2Common.#10366 (0x6FABC7FC)
    int32_t __stdcall UNITS_GetSubtileY(D2UnitStrc* pUnit);                          // D2Common.#10386 (0x6FABC24A)
    D2UnitStrc* __stdcall INVENTORY_GetCursorItem(D2InventoryStrc* pInv);            // D2Common.#10389 (0x6FABC394)
    D2UnitStrc* __stdcall INVENTORY_UnitIsItem(D2UnitStrc* pUnit);                   // D2Common.#10402 (0x6FABC274)
    D2SkillStrc* __stdcall UNITS_GetUsedSkill(D2UnitStrc* pUnit);                    // D2Common.#10405 (0x6FABC424)
    int32_t __stdcall ITEMS_CanBeRare(D2UnitStrc* pItem);                            // D2Common.#10407 (0x6FABCF1C)
    D2UnitStrc* __stdcall INV_GetItemFromBodyLoc(D2InventoryStrc* pInv, int32_t nBodyLoc); // D2Common.#10461 (0x6FABC3B8)
    D2SkillStrc* __stdcall SKILLS_GetHighestLevelSkillFromUnitAndId(D2UnitStrc* pUnit, int32_t nSkillId); // D2Common.#10465 (0x6FABC4D8)
    int32_t __stdcall SKILLS_GetFlags(D2SkillStrc* pSkill);                          // D2Common.#10466 (0x6FABC43C)
    void* __stdcall DATATBLS_GetMonSeqTxtRecordFromUnit(D2UnitStrc* pUnit);          // D2Common.#10485 (0x6FABCC3A)
    void __stdcall SKILLS_SetFlags(D2SkillStrc* pSkill, int32_t nFlags);             // D2Common.#10492 (0x6FABC436)
    int32_t __stdcall ITEMS_CheckItemTypeId(D2UnitStrc* pItem, int32_t nItemType);   // D2Common.#10506 (0x6FABC1D8)
    int32_t __stdcall PATH_GetTargetType(void* pPath);                               // D2Common.#10523 (0x6FABC8CE)
    void __stdcall STATLIST_SetUnitStat(D2UnitStrc* pUnit, int32_t nStatId, int32_t nValue, int32_t nLayer); // D2Common.#10562 (0x6FABC3A0)
    int32_t __stdcall UNITS_IsInMovingMode(D2UnitStrc* pUnit);                       // D2Common.#10567 (0x6FABC6E2)
    int32_t __stdcall SKILLS_GetSkillIdFromSkill(D2SkillStrc* pSkill, const char* szFile, int32_t nLine); // D2Common.#10599 (0x6FABC40C)
    int32_t __stdcall D2Common_10611(D2RoomStrc* pRoom, int32_t nX, int32_t nY);     // D2Common.#10611 (0x6FABCA1E)
    void __stdcall UNITS_GetShiftedFrameMetrics(D2UnitStrc* pUnit, int32_t* pFrameNo, int32_t* pFrameCount); // D2Common.#10631 (0x6FABC7A8)
    void __stdcall MONSTERAI_UpdateAiState(D2UnitStrc* pUnit, int32_t nState);       // D2Common.#10646 (0x6FABCF16)
    int32_t __stdcall STATLIST_UnitGetStatValue(const D2UnitStrc* pUnit, int32_t nStatId, uint16_t nLayer); // D2Common.#10658 (0x6FABC352)
    void __stdcall SKILLS_SetParam2(D2SkillStrc* pSkill, int32_t nVal);              // D2Common.#10674 (0x6FABC61C)
    void __stdcall SKILLS_SetParam4(D2SkillStrc* pSkill, int32_t nVal);              // D2Common.#10688 (0x6FABC640)
    void __stdcall MONSTERS_GetMinionSpawnInfo(D2UnitStrc* pUnit, int32_t* pId, int32_t* pCount, int32_t* pMode); // D2Common.#10691 (0x6FABCA00)
    int32_t __stdcall SKILLS_GetUseState(D2UnitStrc* pUnit, D2SkillStrc* pSkill);    // D2Common.#10715 (0x6FABC67C)
    int32_t __stdcall ITEMS_GetItemQuality(const D2UnitStrc* pItem);                 // D2Common.#10722 (0x6FABC1CE)
    void __stdcall SKILLS_SetParam3(D2SkillStrc* pSkill, int32_t nVal);              // D2Common.#10723 (0x6FABC502)
    D2UnitStrc* __stdcall INV_GetNextItem(D2UnitStrc* pItem);                        // D2Common.#10770 (0x6FABC1CC)
    int32_t __stdcall ITEMS_CheckItemFlag(const D2UnitStrc* pItem, uint32_t dwFlag, int32_t nLine, const char* szFile); // D2Common.#10776 (0x6FABC1B4)
    D2RoomStrc* __stdcall UNITS_GetRoom(const D2UnitStrc* pUnit);                    // D2Common.#10795 (0x6FABC34C)
    int32_t __stdcall UNITS_ChangeAnimMode(D2UnitStrc* pUnit, int32_t nMode);        // D2Common.#10847 (0x6FABC2BE)
    int32_t __stdcall SKILLS_GetLevel(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t bBonus); // D2Common.#10870 (0x6FABC4E4)
    int32_t __stdcall SKILLS_EvaluateSkillFormula(D2UnitStrc* pUnit, uint32_t nCalc, int32_t nSkillId, int32_t nSkillLevel); // D2Common.#10884 (0x6FABC628)
    void __stdcall STATLIST_SetBaseStat2(D2StatListStrc* pStatList, int32_t nStatId, int32_t nValue, uint16_t nLayer, D2UnitStrc* pUnit); // D2Common.#10885 (0x6FABC4A2)
    int32_t __stdcall UNITS_GetSubtileX(D2UnitStrc* pUnit);                          // D2Common.#10896 (0x6FABC244)
    int32_t __stdcall PATH_GetYPosition(void* pPath);                                // D2Common.#10913 (0x6FABC69A)
    D2UnitStrc* __stdcall PATH_GetTargetUnit(void* pPath);                           // D2Common.#10935 (0x6FABC538)
    int32_t __stdcall STATLIST_UnitGetItemStatOrSkillStatValue(D2UnitStrc* pUnit, int32_t nStatId, uint16_t nLayer); // D2Common.#10973 (0x6FABC3F4)
    int32_t __stdcall SKILLS_GetParam4(D2SkillStrc* pSkill);                         // D2Common.#11022 (0x6FABC63A)
    void __stdcall UNITS_SetAnimActionFrame(D2UnitStrc* pUnit, int32_t nAction);     // D2Common.#11044 (0x6FABC7A2)
    void __stdcall D2Common_11066(D2UnitStrc* pUnit, void* pMonSeq, int32_t* pOutFrameCount, int32_t* pOutDirCount); // D2Common.#11066 (0x6FABCC34)

    // D2Gfx.dll
    int32_t __stdcall TEXTURE_CelDrawColor(D2GfxDataStrc* pData, int32_t nX, int32_t nY, uint32_t dwGamma, int32_t nDrawMode, uint8_t nColor); // D2Gfx.#10029 (0x6FABD1B6)
    void __stdcall D2GFX_SetViewMatrixAdjust(int32_t nAdjust);                       // D2Gfx.#10072 (0x6FABD17A)
    int32_t __stdcall TEXTURE_CelDraw(D2GfxDataStrc* pData, int32_t nX, int32_t nY, uint32_t dwGamma, int32_t nDrawMode, void* pPalette); // D2Gfx.#10076 (0x6FABD10E)
    HWND __stdcall WINDOW_GetWindow(void);                                           // D2Gfx.#10078 (0x6FABD0EA)

    // D2CMP.dll
    int32_t __stdcall D2CMP_CelFileFreeHardware(D2CellFileStrc* pCellFile);          // D2CMP.#10065 (0x6FABD348)

    // D2Lang.dll
    int32_t __fastcall STRTABLE_GetLanguageInfo(char* szLangCode, int32_t bShort);   // D2Lang.#10004 (0x6FABD1E6)
    const Unicode* __fastcall D2LANG_GetStringFromTblIndex(uint16_t nIndex);         // D2Lang.#10005 (0x6FABD1D4)

    // D2Win.dll
    void* __stdcall D2Win_GetArchive(void);                                          // D2Win.#10014 (0x6FABD372)
    void __stdcall D2Win_TakeScreenshot(void);                                       // D2Win.#10062 (0x6FABD38A)
}

// ============================================================================
// Internal D2Client Helpers & Globals
// ============================================================================
extern D2UnitStrc* gpPlayerUnit;                // 0x6FBCC3D0
extern int32_t gnClientLoopCount;               // 0x6FBB32B0
extern uint32_t gdwGameTickCounter;             // 0x6FBB32BC
extern int32_t gbIsExpansionClient;             // 0x6FBB330C
extern char gszRealmName[200];                  // 0x6FBB3318
extern char gszAccountOrCharName[64];           // 0x6FBB33E0
extern int32_t gnRealmStatusValue;              // 0x6FBCC020
extern int32_t gnScreenWidth;                   // 0x6FB8C6E0
extern int32_t gnScreenHeight;                  // 0x6FB8C6E4
extern int32_t gnViewportWidth;                 // 0x6FBCD528
extern int32_t gnViewportHeight;                // 0x6FBCD524
extern int32_t gnCameraShakeType;               // 0x6FBCC028
extern int32_t gnViewAdjustMode;                // 0x6FBCC1D0
extern int32_t gnViewAdjustOffset;              // 0x6FBCC1D4
extern int32_t gbEscMenuOpen;                   // 0x6FBCC008
extern int32_t gbGoldDialogOpen;                // 0x6FBB5490
extern int32_t gbSkillSelectOpen;               // 0x6FBB2B64
extern int32_t gbSkillTreeOpen;                 // 0x6FBB2B68
extern int32_t gbChatBoxOpen;                   // 0x6FBB2B6C
extern int32_t gbEscOptionsOpen;                // 0x6FBB2B7C
extern int32_t gbAutomapActive;                 // 0x6FBB2B80
extern int32_t gbTradeOpen;                     // 0x6FBB2B88
extern int32_t gbShowGroundItems;               // 0x6FBB2B8C
extern int32_t gbStashOpen;                     // 0x6FBB2B94
extern int32_t gbCubeOpen;                      // 0x6FBB2B98
extern int32_t gbQuestLogOpen;                  // 0x6FBB2BA8
extern int32_t gbPartyScreenOpen;               // 0x6FBB2BB0
extern int32_t gbNpcMenuOpen;                   // 0x6FBB2BB4
extern int32_t gbMessageLogOpen;                // 0x6FBB2BB8
extern int32_t gbVendorInteractOpen;            // 0x6FBB2BBC
extern int32_t gbBeltShowOpen;                  // 0x6FBB2BD4
extern int32_t gbHelpScreenOpen;                // 0x6FBB2BDC
extern int32_t gbHireableScreenOpen;            // 0x6FBB2BE8
extern int32_t gbWaypointMenuOpen;              // 0x6FBB2BEC
extern int32_t gbInventoryOpen;                 // 0x6FBB2B5C
extern int32_t gbCharacterScreenOpen;           // 0x6FBB2B60
extern int32_t gabPanelStates[38];              // 0x6FBB2B58
extern int32_t gnPanelCount;                    // 0x6FB8F368 (0x26 = 38)
extern void* gapPanelCloseRules[38];            // 0x6FB8F2D0

// Cursor state globals (0x6FBB1630..0x6FBB1678)
extern int32_t gbCursorTooltipLocked;           // 0x6FBB1630
extern int32_t gnMouseY;                        // 0x6FBB1634
extern int32_t gnMouseX;                        // 0x6FBB1638
extern int32_t gnCursorHoverUnitId;             // 0x6FBB163C
extern int32_t gnCursorHoverUnitType;           // 0x6FBB1640
extern D2UnitStrc* gpCursorLockedItem;          // 0x6FBB1644
extern D2CellFileStrc* gapCursorCellFiles[7];   // 0x6FBB1648..0x6FBB1660
extern int32_t gnCursorAnimId;                  // 0x6FBB1664
extern int32_t gnCursorFrameFixed8;             // 0x6FBB1668
extern int32_t gnCursorSoundId;                 // 0x6FBB166C
extern D2CellFileStrc* gpCursorItemCellFile;    // 0x6FBB1670
extern int32_t gnCursorState;                   // 0x6FBB1674 (1 = normal, 2 = item on cursor, 3..6 = spell target/buy/sell/repair/identify)
extern uint8_t gnCursorItemColorIndex;          // 0x6FBB1678

// Mouse & shortcut globals (0x6FBB0D24..0x6FBB162C)
extern D2CmdCallback_t gpfnMouseLButtonDown;    // 0x6FBB0D24
extern D2KeyBindingStrc gSavedKeyBindings[114]; // 0x6FBB0D28..0x6FBB119C
extern D2KeyBindingStrc gKeyBindings[114];      // 0x6FBB11A0..0x6FBB1614
extern D2CmdCallback_t gpfnMouseRButtonUp;      // 0x6FBB1614
extern D2CmdCallback_t gpfnMouseMButtonUp;      // 0x6FBB1618
extern D2CmdCallback_t gpfnMouseWheelUp;        // 0x6FBB161C
extern D2CmdCallback_t gpfnMouseWheelDown;      // 0x6FBB1620
extern D2CmdCallback_t gpfnMouseLButtonUp;      // 0x6FBB1624
extern D2CmdCallback_t gpfnMouseMButtonDown;    // 0x6FBB1628
extern D2CmdCallback_t gpfnMouseRButtonDown;    // 0x6FBB162C

extern int32_t gbCmdTblInputEnabled;            // 0x6FBCBCC4
extern int32_t gbCmdTblKeyHandlersRegistered;   // 0x6FBD33FC
extern int32_t gbCmdTblAltKeyHandlersRegistered;// 0x6FBD33F8
extern int32_t gbStandStillActive;              // 0x6FBCC054
extern int32_t gbAlwaysRunEnabled;              // 0x6FBCC050
extern int32_t gbRunKeyHeld;                    // 0x6FBCC04C
extern int32_t gbAutomapLeftAlign;              // 0x6FBCC330
extern int32_t gbAutomapLeftAlign2;             // 0x6FBCC334
extern int32_t gbAutomapLeftAlign3;             // 0x6FBCC338
extern int32_t gbAutomapShowPartyNames;         // 0x6FBA3BA4
extern int32_t gbAutomapShowParty;              // 0x6FBA3BA0
extern int32_t gnAutomapFadeMode;               // 0x6FBCC2F8
extern int32_t gnAutomapFadeDefault;            // 0x6FBCC2FC
extern int32_t gbAutomapMinimapMode;            // 0x6FBCC2A0
extern Unicode gwszKeyCharBuf[4];               // 0x6FBCF530

// Forward declarations of internal cursor & command callbacks
void __fastcall CURSOR_UpdateAnimatedFrame(void);       // 0x6FB4F390
void __fastcall CURSOR_UpdateStaticFrame(void);         // 0x6FB4EAE0
int32_t __stdcall CURSOR_OnMouseMove(D2WindowMsgStrc* pMsg);      // 0x6FB4EC90
int32_t __stdcall CURSOR_OnNcMouseMove(D2WindowMsgStrc* pMsg);    // 0x6FB4EB90
int32_t __stdcall CURSOR_OnRButtonUpReset(D2WindowMsgStrc* pMsg); // 0x6FB4EE90
int32_t __stdcall CMDTBL_OnKeyDown(D2WindowMsgStrc* pMsg);        // 0x6FB4F7A0
int32_t __stdcall CMDTBL_OnKeyUp(D2WindowMsgStrc* pMsg);          // 0x6FB4F6C0

// ============================================================================
// Part 1: Monster Skill Client Functions (0x6FB4C580..0x6FB4E820)
//         Continuation of ..\Source\D2Client\SKILLS\SkillsMon.cpp
// ============================================================================

// 1.12a: 0x6FB4C580 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_TargetCoordsAndEndAtAction(D2UnitStrc* pUnit)
{
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        int32_t nX = SKILLS_GetParam1(pSkill);
        int32_t nY = SKILLS_GetParam2(pSkill);
        if (nX && nY)
        {
            PATH_SetTargetPos(*(void**)((uint8_t*)pUnit + 0x2C), nX, nY);
            UNITS_SetTargetUnitForDynamicUnit(pUnit, NULL);
        }
    }
    int32_t nFrameNo = 0, nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4C5F0 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_ReturnOneA(void)
{
    return 1;
}

// 1.12a: 0x6FB4C610 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_ReturnOneB(void)
{
    return 1;
}

// 1.12a: 0x6FB4C630 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_SpawnLoopSoundAndMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    if (*((uint8_t*)pUnit + 0x24) == 0 && (*((uint8_t*)pUnit + 0xC4) & 0x08) == 0)
        return 0;

    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    extern D2UnitStrc* __stdcall MISSILE_CreateClientMissile(int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nParam); // 0x6FAFC400

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x84);
    if (nSoundId > 0)
        UNIT_PlaySound(pUnit, nSoundId);

    int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
    if (nMissileId >= 0)
        MISSILE_CreateClientMissile(nMissileId, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
    return 1;
}

// 1.12a: 0x6FB4C6B0 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_MinionSpawner(D2UnitStrc* pUnit)
{
    int32_t nSpawnId = 0, nSpawnCount = 0, nSpawnMode = 0;
    MONSTERS_GetMinionSpawnInfo(pUnit, &nSpawnId, &nSpawnCount, &nSpawnMode);
    if (nSpawnId < 0)
        return 0;

    D2MonStatsTxt* pMonStats = (D2MonStatsTxt*)((uint8_t*)(*(void**)((uint8_t*)sgptDataTables + 0xA78)) + nSpawnId * 0x1A8);
    if (!pMonStats)
        return 0;

    extern void __fastcall UNIT_LoadMonsterPaletteAssets(int32_t nClassId, int32_t nZero, uint8_t* pComponents); // 0x6FAE1F70
    uint8_t aComponents[16] = { 0 };
    UNIT_LoadMonsterPaletteAssets(nSpawnId, 0, aComponents);
    UNITS_ChangeAnimMode(pUnit, nSpawnMode);

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    SKILLS_SetFlags(pSkill, 1);
    return 1;
}

// 1.12a: 0x6FB4C770 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_MinionSpawner(D2UnitStrc* pUnit)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        int32_t nSpawnId = 0;
        MONSTERS_GetMinionSpawnInfo(pUnit, &nSpawnId, &nFrameNo, &nFrameCount);
        if (nSpawnId >= 0)
        {
            if ((D2MonStatsTxt*)((uint8_t*)(*(void**)((uint8_t*)sgptDataTables + 0xA78)) + nSpawnId * 0x1A8) != NULL)
            {
                extern void __stdcall PARTICLE_SpawnOverlayOrEffect(D2UnitStrc* pUnit, int32_t nArg1, int32_t nArg2, int32_t nArg3); // 0x6FB75BA0
                PARTICLE_SpawnOverlayOrEffect(pUnit, 0, 1, 0);
            }
        }
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4C800 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_PlaySkillSoundAndSaveTarget(D2UnitStrc* pUnit, int32_t nSkillId)
{
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    extern int32_t __fastcall SKILLS_SaveTargetCoordsToSkill(D2UnitStrc* pUnit, D2SkillStrc* pSkill); // 0x6FAFC4C0

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x84);
    if (nSoundId > 0)
        UNIT_PlaySound(pUnit, nSoundId);

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    if (!SKILLS_SaveTargetCoordsToSkill(pUnit, pSkill))
        return 0;
    SKILLS_SetFlags(pSkill, 1);
    return 1;
}

// 1.12a: 0x6FB4C890 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_CastMissileAtSavedCoords(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
        extern void __stdcall SKILLS_LaunchMissileAtCoords(D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nTargetX, int32_t nTargetY, int32_t nZero); // 0x6FAFC460
        D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (pSkillsTxt)
        {
            int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
            if (nMissileId > 0)
            {
                int32_t nTargetY = SKILLS_GetParam2(pSkill);
                int32_t nTargetX = SKILLS_GetParam1(pSkill);
                SKILLS_LaunchMissileAtCoords(pUnit, nMissileId, nSkillId, nSkillLevel, nTargetX, nTargetY, 0);
            }
        }
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4C930 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_FingerMageSpider(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    if (SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, 0x2CE) != nSkillId)
        return 0;

    extern int32_t __fastcall SKILLS_SaveTargetUnitToSkill(D2UnitStrc* pUnit, D2SkillStrc* pSkill); // 0x6FAFC6A0
    if (!SKILLS_SaveTargetUnitToSkill(pUnit, pSkill))
        return 0;
    SKILLS_SetFlags(pSkill, 1);
    SKILLS_SetParam3(pSkill, 0);
    return 1;
}

// 1.12a: 0x6FB4C9A0 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_FingerMageSpider(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    if (SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, 0x2EA) != nSkillId)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        extern void __stdcall SKILLS_SpawnHomingMissileFromSkill(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nSkillId, int32_t nSkillLevel, int32_t nZero); // 0x6FAFD3D0
        SKILLS_SpawnHomingMissileFromSkill(pUnit, pSkill, nSkillId, nSkillLevel, 0);
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4CA00 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_DiabloColdTouch(D2UnitStrc* pUnit, int32_t nSkillId)
{
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern D2UnitStrc* __stdcall MISSILE_CreateClientMissile(int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nParam); // 0x6FAFC400

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
    if (nMissileId < 0 || nMissileId >= *(int32_t*)((uint8_t*)sgptDataTables + 0xB94))
        return 0;

    if (pUnit)
    {
        int32_t nX = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
            ? PATH_GetXPosition(*(void**)((uint8_t*)pUnit + 0x2C))
            : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x0C);
        int32_t nY = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
            ? PATH_GetYPosition(*(void**)((uint8_t*)pUnit + 0x2C))
            : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x10);
        MISSILE_CreateClientMissile(nMissileId, pUnit, nSkillId, 1, nX, nY, 0, 0, 0);
    }
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    SKILLS_SetFlags(pSkill, 1);
    return 1;
}

// 1.12a: 0x6FB4CAD0 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_SubmergeToggle(D2UnitStrc* pUnit)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    int32_t nFlags = SKILLS_GetFlags(pSkill);
    if ((nFlags & 0x04) == 0)
    {
        UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
        return nFrameNo != 0;
    }

    SKILLS_SetFlags(pSkill, nFlags & ~0x04);
    int32_t nMode = *((int32_t*)pUnit + 4);
    if (nMode == 15) // MONMODE_SKILL3 -> submerge
    {
        UNITS_ChangeAnimMode(pUnit, 5); // MONMODE_SEQUENCE / submerged
        STATLIST_SetUnitStat(pUnit, 0xAC, 1, 0);
        return 1;
    }
    if (nMode == 16) // MONMODE_SKILL4 -> emerge
    {
        UNITS_ChangeAnimMode(pUnit, 1); // MONMODE_NEUTRAL
        STATLIST_SetUnitStat(pUnit, 0xAC, 0, 0);
    }
    return 1;
}

// 1.12a: 0x6FB4CB90 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_MaggotEgg(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit)
        return 0;
    if (nSkillId >= 0 && nSkillId < *(int32_t*)((uint8_t*)sgptDataTables + 0xB98))
    {
        D2SkillsTxt* pSkillsTxt = (D2SkillsTxt*)((uint8_t*)(*(void**)((uint8_t*)sgptDataTables + 0xB88)) + nSkillId * 0x23C);
        if (pSkillsTxt && *(int32_t*)((uint8_t*)pSkillsTxt + 0x188) != 0)
        {
            D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
            if (pSkill)
            {
                if (nSkillLevel <= 0)
                    nSkillLevel = SKILLS_GetLevel(pUnit, pSkill, 1);
                int32_t nVal = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)((uint8_t*)pSkillsTxt + 0x188), nSkillId, nSkillLevel);
                if (nVal != 0)
                {
                    int32_t nMaxLife = STATLIST_GetMaxLifeFromUnit(pUnit);
                    if (nMaxLife <= 0)
                        nMaxLife = 1;
                    STATLIST_SetBaseStat2(*(D2StatListStrc**)((uint8_t*)pUnit + 0x5C), 6, 1, 0, pUnit);
                    STATLIST_SetBaseStat2(*(D2StatListStrc**)((uint8_t*)pUnit + 0x5C), 7, nMaxLife, 0, pUnit);
                }
            }
        }
    }
    D2SkillStrc* pUsedSkill = UNITS_GetUsedSkill(pUnit);
    if (!pUsedSkill)
        return 0;
    SKILLS_SetFlags(pUsedSkill, 1);
    return 1;
}

// 1.12a: 0x6FB4CD30 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_QueenSpit(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
        extern void __stdcall SKILLS_LaunchMissileFromOffset(D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nOffsetX, int32_t nOffsetY, int32_t nZero); // 0x6FAFCE10
        D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxt)
            return 0;
        int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
        if (nMissileId < 0 || nMissileId >= *(int32_t*)((uint8_t*)sgptDataTables + 0xB94))
            return 0;
        int32_t nOffset = *(int32_t*)((uint8_t*)pSkillsTxt + 0x154);
        int32_t nNegOffset = -nOffset;
        SKILLS_LaunchMissileFromOffset(pUnit, nMissileId, nSkillId, nSkillLevel, nNegOffset, nNegOffset, 0);
        SKILLS_LaunchMissileFromOffset(pUnit, nMissileId, nSkillId, nSkillLevel, nNegOffset, nOffset, 0);
        SKILLS_LaunchMissileFromOffset(pUnit, nMissileId, nSkillId, nSkillLevel, nOffset, nNegOffset, 0);
        SKILLS_LaunchMissileFromOffset(pUnit, nMissileId, nSkillId, nSkillLevel, nOffset, nOffset, 0);
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4CE10 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_MephistoLightningOrFrostNova(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
        extern void __stdcall SKILLS_LaunchNovaMissiles(D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nVel); // 0x6FAFCC70
        extern D2UnitStrc* __stdcall MISSILE_CreateClientMissile(int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nParam); // 0x6FAFC400

        D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxt)
            return 0;
        int16_t nNovaMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x40);
        if (nNovaMissileId >= 0)
            SKILLS_LaunchNovaMissiles(pUnit, nNovaMissileId, nSkillId, nSkillLevel, 0);

        int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
        if (nMissileId < 0)
            return 0;

        int32_t nX = 0, nY = 0;
        if (pUnit)
        {
            nX = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
                ? PATH_GetXPosition(*(void**)((uint8_t*)pUnit + 0x2C))
                : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x0C);
            nY = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
                ? PATH_GetYPosition(*(void**)((uint8_t*)pUnit + 0x2C))
                : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x10);
        }
        int32_t nTargetX = SKILLS_GetParam1(pSkill);
        int32_t nTargetY = SKILLS_GetParam2(pSkill);
        if (!nTargetX || !nTargetY)
        {
            extern int32_t __fastcall SKILLS_GetUnitTargetCoords(D2UnitStrc* pUnit, int32_t* pX, int32_t* pY); // 0x6FAFC980
            if (!SKILLS_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
            {
                nTargetX = nX + 1;
                nTargetY = nY + 1;
            }
        }
        MISSILE_CreateClientMissile(nMissileId, pUnit, nSkillId, nSkillLevel, nX, nY, nTargetX, nTargetY, 0);
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4CFB0 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_DiabloFirestorm(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    if (nSkillId < 0 || nSkillId >= *(int32_t*)((uint8_t*)sgptDataTables + 0xB98))
        return 0;
    D2SkillsTxt* pSkillsTxt = (D2SkillsTxt*)((uint8_t*)(*(void**)((uint8_t*)sgptDataTables + 0xB88)) + nSkillId * 0x23C);
    if (!pSkillsTxt)
        return 0;

    int16_t nMissileA = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
    if (nMissileA >= 0 && nMissileA < *(int32_t*)((uint8_t*)sgptDataTables + 0xB94))
    {
        int16_t nMissileB = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3E);
        if (nMissileB >= 0 && nMissileB < *(int32_t*)((uint8_t*)sgptDataTables + 0xB94))
        {
            if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
            {
                int32_t nTargetX = SKILLS_GetParam1(pSkill);
                int32_t nTargetY = SKILLS_GetParam2(pSkill);
                if (!nTargetX || !nTargetY)
                {
                    extern int32_t __fastcall SKILLS_GetUnitTargetCoords(D2UnitStrc* pUnit, int32_t* pX, int32_t* pY); // 0x6FAFC980
                    if (!SKILLS_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
                        return 0;
                }
                int32_t nX = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
                    ? PATH_GetXPosition(*(void**)((uint8_t*)pUnit + 0x2C))
                    : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x0C);
                int32_t nY = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
                    ? PATH_GetYPosition(*(void**)((uint8_t*)pUnit + 0x2C))
                    : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x10);
                int32_t nDx = nTargetX - nX;
                int32_t nDy = nTargetY - nY;
                extern void __stdcall SKILLS_RotateVector2D(int32_t* pDx, int32_t* pDy, int32_t nAngle); // 0x6FAFD340
                extern D2UnitStrc* __stdcall MISSILE_CreateClientMissile(int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nParam); // 0x6FAFC400

                int32_t nStreams = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)((uint8_t*)pSkillsTxt + 0x188), nSkillId, nSkillLevel);
                if (nStreams > 1)
                    SKILLS_RotateVector2D(&nDx, &nDy, -8 * nStreams);

                for (int32_t i = 0; i < nStreams; ++i)
                {
                    int32_t nDestX = nX + nDx;
                    int32_t nDestY = nY + nDy;
                    D2UnitStrc* pMissile = MISSILE_CreateClientMissile(nMissileA, pUnit, nSkillId, nSkillLevel, nX, nY, nDestX, nDestY, 0);
                    if (pMissile)
                    {
                        MISSILE_CreateClientMissile(nMissileB, pUnit, nSkillId, nSkillLevel, nX, nY, nDestX, nDestY, 0);
                        PATH_SetTargetPos(*(void**)((uint8_t*)pMissile + 0x2C), nDestX, nDestY);
                        UNITS_SetTargetUnitForDynamicUnit(pMissile, pMissile);
                    }
                    SKILLS_RotateVector2D(&nDx, &nDy, 16);
                }
            }
            UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
            return nFrameNo != 0;
        }
    }
    return 0;
}

// 1.12a: 0x6FB4D270 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_DiabloLaser(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    extern int32_t __fastcall SKILLS_SaveTargetCoordsToSkill(D2UnitStrc* pUnit, D2SkillStrc* pSkill); // 0x6FAFC4C0
    if (!SKILLS_SaveTargetCoordsToSkill(pUnit, pSkill))
        return 0;
    SKILLS_SetFlags(pSkill, 1);

    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x84);
    if (nSoundId > 0)
        UNIT_PlaySound(pUnit, nSoundId);
    return 1;
}

// 1.12a: 0x6FB4D2D0 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_DiabloLaser(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;

    int32_t nFlags = SKILLS_GetFlags(pSkill);
    if ((nFlags & 0x04) != 0)
        SKILLS_SetFlags(pSkill, nFlags | 0x02);

    if ((SKILLS_GetFlags(pSkill) & 0x02) != 0)
    {
        int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
        if (nMissileId < 0 || nMissileId >= *(int32_t*)((uint8_t*)sgptDataTables + 0xB94))
            return 0;

        int32_t nTargetX = SKILLS_GetParam1(pSkill);
        int32_t nTargetY = SKILLS_GetParam2(pSkill);
        if (nTargetX && nTargetY)
        {
            extern void __stdcall SKILLS_LaunchMissileWithVelocity(D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nTargetX, int32_t nTargetY, int32_t nVel); // 0x6FAFCAD0
            SKILLS_LaunchMissileWithVelocity(pUnit, nMissileId, nSkillId, nSkillLevel, nTargetX, nTargetY, 1);
            UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
            return nFrameNo != 0;
        }
        return 0;
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4D3F0 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_DiabloWallOfFire(D2UnitStrc* pUnit, int32_t nSkillId)
{
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    extern void __stdcall SKILLS_LaunchNovaMissiles(D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nVel); // 0x6FAFCC70

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x84);
    if (nSoundId > 0)
        UNIT_PlaySound(pUnit, nSoundId);

    int16_t nNovaMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x40);
    if (nNovaMissileId >= 0)
        SKILLS_LaunchNovaMissiles(pUnit, nNovaMissileId, nSkillId, 1, *(int32_t*)((uint8_t*)pSkillsTxt + 0x154));

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    SKILLS_SetFlags(pSkill, 1);
    return 1;
}

// 1.12a: 0x6FB4D480 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_DiabloWallOfFire(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern void __stdcall SKILLS_LaunchNovaMissiles(D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nVel); // 0x6FAFCC70
    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;

    int16_t nMissileA = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
    if (nMissileA >= 0)
    {
        int16_t nMissileB = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3E);
        if (nMissileB >= 0)
        {
            D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
            if (!pSkill)
                return 0;
            if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
            {
                int32_t nParamA = *(int32_t*)((uint8_t*)pSkillsTxt + 0x154);
                int32_t nParamB = *(int32_t*)((uint8_t*)pSkillsTxt + 0x158);
                int32_t nMaxVel = nParamA + (nSkillLevel - 1) * nParamB;
                int32_t nMinVel = nParamB / 2;
                if (nMinVel < 1)
                    nMinVel = 1;
                if (nMinVel < nMaxVel)
                {
                    int32_t nCurVel = nMinVel;
                    int32_t nDoubleVel = nMinVel * 2;
                    do
                    {
                        SKILLS_LaunchNovaMissiles(pUnit, nMissileA, nSkillId, nSkillLevel, nCurVel);
                        if (nCurVel >= nMaxVel - nMinVel)
                            SKILLS_LaunchNovaMissiles(pUnit, nMissileB, nSkillId, nSkillLevel, nDoubleVel + 1);
                        else
                            SKILLS_LaunchNovaMissiles(pUnit, nMissileA, nSkillId, nSkillLevel, nDoubleVel + 1);
                        nCurVel += nMinVel;
                        nDoubleVel += nMinVel * 2;
                    } while (nCurVel < nMaxVel);
                }
                SKILLS_LaunchNovaMissiles(pUnit, nMissileB, nSkillId, nSkillLevel, nMaxVel);
            }
            UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
            return nFrameNo != 0;
        }
    }
    return 0;
}

// 1.12a: 0x6FB4D620 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_SpawnMissileOnTargetUnit(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern D2UnitStrc* __fastcall UNIT_FindUnitById(int32_t nUnitId, int32_t nUnitType); // 0x6FACF9C0
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    extern D2UnitStrc* __stdcall MISSILE_CreateClientMissile(int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nParam); // 0x6FAFC400

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x40);
    if (nMissileId < 0)
        return 0;

    int32_t nTargetType = PATH_GetTargetType(*(void**)((uint8_t*)pUnit + 0x2C));
    D2UnitStrc* pTarget = PATH_GetTargetUnit(*(void**)((uint8_t*)pUnit + 0x2C));
    D2UnitStrc* pFound = UNIT_FindUnitById((int32_t)(uintptr_t)pTarget, nTargetType);
    if (pFound)
    {
        int32_t nX = (*((uint32_t*)pFound) <= 3 || *((uint32_t*)pFound) == 5)
            ? PATH_GetXPosition(*(void**)((uint8_t*)pFound + 0x2C))
            : *(int32_t*)(*(uint8_t**)((uint8_t*)pFound + 0x2C) + 0x0C);
        int32_t nY = (*((uint32_t*)pFound) <= 3 || *((uint32_t*)pFound) == 5)
            ? PATH_GetYPosition(*(void**)((uint8_t*)pFound + 0x2C))
            : *(int32_t*)(*(uint8_t**)((uint8_t*)pFound + 0x2C) + 0x10);
        SKILLS_SetParam1(pSkill, nX);
        SKILLS_SetParam2(pSkill, nY);
        int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x84);
        if (nSoundId > 0)
            UNIT_PlaySound(pUnit, nSoundId);
        MISSILE_CreateClientMissile(nMissileId, pUnit, nSkillId, nSkillLevel, nX, nY, 0, 0, 0);
        SKILLS_SetFlags(pSkill, 1);
        return 1;
    }
    SKILLS_SetParam1(pSkill, 0);
    SKILLS_SetParam2(pSkill, 0);
    return 0;
}

// 1.12a: 0x6FB4D740 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_SpawnMissileAtSavedCoords(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern D2UnitStrc* __stdcall MISSILE_CreateClientMissile(int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nParam); // 0x6FAFC400

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
    if (nMissileId < 0)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        int32_t nX = SKILLS_GetParam1(pSkill);
        int32_t nY = SKILLS_GetParam2(pSkill);
        if (!nX || !nY)
            return 0;
        if (DUNGEON_FindRoomBySubtileCoordinates(*(void**)((uint8_t*)pUnit + 0x08), nX, nY))
            MISSILE_CreateClientMissile(nMissileId, pUnit, nSkillId, nSkillLevel, nX, nY, 0, 0, 0);
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4D850 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_VileMotherSpawn(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    extern D2UnitStrc* __stdcall MISSILE_CreateClientMissile(int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nParam); // 0x6FAFC400

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
    if (nMissileId < 0)
        return 0;

    int32_t nTargetX = SKILLS_GetParam1(pSkill);
    int32_t nTargetY = SKILLS_GetParam2(pSkill);
    if (!nTargetX || !nTargetY)
    {
        extern int32_t __fastcall SKILLS_GetUnitTargetCoords(D2UnitStrc* pUnit, int32_t* pX, int32_t* pY); // 0x6FAFC980
        if (!SKILLS_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
            return 0;
    }

    int32_t nSpawnId = 0, nSpawnCount = 0, nSpawnMode = 0;
    MONSTERS_GetMinionSpawnInfo(pUnit, &nSpawnId, &nSpawnCount, &nSpawnMode);
    if (nSpawnId >= 0 && (D2MonStatsTxt*)((uint8_t*)(*(void**)((uint8_t*)sgptDataTables + 0xA78)) + nSpawnId * 0x1A8) != NULL)
    {
        extern void __fastcall UNIT_LoadMonsterPaletteAssets(int32_t nClassId, int32_t nZero, uint8_t* pComponents); // 0x6FAE1F70
        uint8_t aComponents[16] = { 0 };
        UNIT_LoadMonsterPaletteAssets(nSpawnId, 0, aComponents);
    }

    int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x84);
    if (nSoundId > 0)
        UNIT_PlaySound(pUnit, nSoundId);

    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom && !D2Common_10611(pRoom, nTargetX, nTargetY))
        MISSILE_CreateClientMissile(nMissileId, pUnit, nSkillId, nSkillLevel, nTargetX, nTargetY, 0, 0, 0);

    SKILLS_SetFlags(pSkill, 1);
    return 1;
}

// 1.12a: 0x6FB4D9F0 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_EvilHutSpawner(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern D2UnitStrc* __stdcall MISSILE_CreateClientMissile(int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nParam); // 0x6FAFC400

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    int16_t nMissileA = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
    if (nMissileA >= 0)
    {
        int16_t nMissileB = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3E);
        if (nMissileB >= 0 && *(int16_t*)((uint8_t*)pSkillsTxt + 0x40) >= 0)
        {
            if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
            {
                int32_t nX = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
                    ? PATH_GetXPosition(*(void**)((uint8_t*)pUnit + 0x2C))
                    : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x0C);
                int32_t nY = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
                    ? PATH_GetYPosition(*(void**)((uint8_t*)pUnit + 0x2C))
                    : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x10);
                int32_t nTargetX = SKILLS_GetParam1(pSkill);
                int32_t nTargetY = SKILLS_GetParam2(pSkill);
                if (!nTargetX || !nTargetY)
                {
                    extern int32_t __fastcall SKILLS_GetUnitTargetCoords(D2UnitStrc* pUnit, int32_t* pX, int32_t* pY); // 0x6FAFC980
                    if (!SKILLS_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
                    {
                        nTargetX = nX + 1;
                        nTargetY = nY + 1;
                    }
                }
                D2UnitStrc* pMissileA = MISSILE_CreateClientMissile(nMissileA, pUnit, nSkillId, nSkillLevel, nX, nY, nTargetX, nTargetY, 0);
                if (pMissileA)
                {
                    PATH_SetTargetPos(*(void**)((uint8_t*)pMissileA + 0x2C), nTargetX, nTargetY);
                    D2UnitStrc* pMissileB = MISSILE_CreateClientMissile(nMissileB, pUnit, nSkillId, nSkillLevel, nX, nY, nTargetX, nTargetY, 0);
                    if (pMissileB)
                        PATH_SetTargetPos(*(void**)((uint8_t*)pMissileB + 0x2C), nTargetX, nTargetY);
                }
            }
            UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
            return nFrameNo != 0;
        }
    }
    return 0;
}

// 1.12a: 0x6FB4DC00 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_MosquitoDrain(D2UnitStrc* pUnit, int32_t nSkillId)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern D2UnitStrc* __fastcall UNIT_FindUnitById(int32_t nUnitId, int32_t nUnitType); // 0x6FACF9C0
    extern void __stdcall PARTICLE_SpawnOverlayOrEffect(D2UnitStrc* pUnit, int32_t nArg1, int32_t nArg2, int32_t nArg3); // 0x6FB75BA0

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        int32_t nTargetType = PATH_GetTargetType(*(void**)((uint8_t*)pUnit + 0x2C));
        D2UnitStrc* pTarget = PATH_GetTargetUnit(*(void**)((uint8_t*)pUnit + 0x2C));
        D2UnitStrc* pFound = UNIT_FindUnitById((int32_t)(uintptr_t)pTarget, nTargetType);
        if (!pFound || UNITS_IsInMovingMode(pFound))
            return 0;
        if (!*((int32_t*)pFound + 3))
            return 0;

        int32_t nCount = SKILLS_GetParam1(pSkill);
        if (nCount == 1)
            PARTICLE_SpawnOverlayOrEffect(pUnit, 0, 1, 0);
        if (nCount < *(int32_t*)((uint8_t*)pSkillsTxt + 0x154))
        {
            SKILLS_SetParam1(pSkill, nCount + 1);
            UNITS_SetAnimActionFrame(pUnit, 1);
        }
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4DD10 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_RegurgitatorEat(D2UnitStrc* pUnit, int32_t nSkillId)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern D2UnitStrc* __fastcall UNIT_FindUnitById(int32_t nUnitId, int32_t nUnitType); // 0x6FACF9C0
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    extern void __stdcall PARTICLE_SpawnRegurgitatorCorpseEffect(D2UnitStrc* pUnit, D2UnitStrc* pCorpse, int32_t nZero); // 0x6FB75DF0

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;

    int32_t nTargetType = PATH_GetTargetType(*(void**)((uint8_t*)pUnit + 0x2C));
    D2UnitStrc* pTarget = PATH_GetTargetUnit(*(void**)((uint8_t*)pUnit + 0x2C));
    D2UnitStrc* pCorpse = UNIT_FindUnitById((int32_t)(uintptr_t)pTarget, nTargetType);
    if (pCorpse && *((int32_t*)pCorpse + 4) == 12) // MONMODE_DEAD
    {
        if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
        {
            int32_t nStep = SKILLS_GetParam1(pSkill);
            int32_t nMaxStep = *(int32_t*)((uint8_t*)pSkillsTxt + 0x154);
            if (nStep < nMaxStep)
            {
                int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x88);
                if (nSoundId > 0)
                    UNIT_PlaySound(pUnit, nSoundId);
                int32_t nNextStep = nStep + 1;
                SKILLS_SetParam1(pSkill, nNextStep);
                UNITS_SetAnimActionFrame(pUnit, 2);
                if (nNextStep == nMaxStep)
                {
                    *((uint32_t*)pCorpse + 0x31) &= ~0x00000004u;
                    PARTICLE_SpawnRegurgitatorCorpseEffect(pUnit, pCorpse, 0);
                }
            }
        }
        UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
        return nFrameNo != 0;
    }
    return 0;
}

// 1.12a: 0x6FB4DE90 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_SuicideMinionExplode(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern void __stdcall SKILLS_LaunchNovaMissiles(D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nVel); // 0x6FAFCC70

    if (!SKILLS_GetSkillsTxtRecord(nSkillId))
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        if (STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 0xD2, 0))
            SKILLS_LaunchNovaMissiles(pUnit, 0x63, nSkillId, nSkillLevel, 0);
        if (STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 0xD0, 0))
            SKILLS_LaunchNovaMissiles(pUnit, 0xF3, nSkillId, nSkillLevel, 0);

        UNITS_ChangeAnimMode(pUnit, 12); // MONMODE_DEAD
        *((uint32_t*)pUnit + 0x31) &= ~0x00000004u;
        int32_t nClassId = *((int32_t*)pUnit + 1);
        if (nClassId == 0x208 || nClassId == 0x209)
        {
            extern void __stdcall PARTICLE_SpawnSuicideMinionGore(D2UnitStrc* pUnit, int32_t nGoreType, int32_t nZero); // 0x6FB75BA0 / 0x6FB75FA0
            PARTICLE_SpawnSuicideMinionGore(pUnit, 3, 0);
            PARTICLE_SpawnSuicideMinionGore(pUnit, 4, 0);
        }
        else
        {
            extern void __stdcall PARTICLE_SpawnSuicideMinionGore(D2UnitStrc* pUnit, int32_t nGoreType, int32_t nZero);
            for (int32_t i = 0; i < 6; ++i)
                PARTICLE_SpawnSuicideMinionGore(pUnit, 1, 0);
            for (int32_t i = 0; i < 6; ++i)
                PARTICLE_SpawnSuicideMinionGore(pUnit, 2, 0);
            PARTICLE_SpawnSuicideMinionGore(pUnit, 0, 0);
        }
        return 0;
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4E010 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_CatapultSiegeShot(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    extern D2UnitStrc* __stdcall MISSILE_CreateClientMissile(int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nParam); // 0x6FAFC400

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        int32_t nTargetX = SKILLS_GetParam1(pSkill);
        int32_t nTargetY = SKILLS_GetParam2(pSkill);
        int32_t nMissileId = SKILLS_GetParam3(pSkill);
        if (!nTargetX || !nTargetY)
        {
            extern int32_t __fastcall SKILLS_GetUnitTargetCoords(D2UnitStrc* pUnit, int32_t* pX, int32_t* pY); // 0x6FAFC980
            if (!SKILLS_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
                return 0;
        }
        if (nMissileId < 0 || nMissileId >= *(int32_t*)((uint8_t*)sgptDataTables + 0xB94))
            return 0;
        int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x88);
        if (nSoundId > 0)
            UNIT_PlaySound(pUnit, nSoundId);

        int32_t nX = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
            ? PATH_GetXPosition(*(void**)((uint8_t*)pUnit + 0x2C))
            : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x0C);
        int32_t nY = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
            ? PATH_GetYPosition(*(void**)((uint8_t*)pUnit + 0x2C))
            : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x10);
        D2UnitStrc* pMissile = MISSILE_CreateClientMissile(nMissileId, pUnit, nSkillId, nSkillLevel, nX - 4, nY - 6, nTargetX, nTargetY, 0);
        if (pMissile)
            PATH_SetTargetPos(*(void**)((uint8_t*)pMissile + 0x2C), nTargetX, nTargetY);
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4E170 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_AncientsWhirlwind(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (!pUnit)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;

    int32_t nFlags = SKILLS_GetFlags(pSkill);
    if ((nFlags & 0x02) != 0)
    {
        if (!UNITS_IsInMovingMode(pUnit))
        {
            SKILLS_SetFlags(pSkill, 0);
            return 0;
        }
        int32_t nNextSoundTick = SKILLS_GetParam4(pSkill);
        if (nNextSoundTick <= gnClientLoopCount)
        {
            int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x84);
            if (nSoundId > 0)
                UNIT_PlaySound(pUnit, nSoundId);
            SKILLS_SetParam4(pSkill, gnClientLoopCount + 20);
        }
        return 1;
    }
    if ((nFlags & 0x01) != 0)
    {
        int32_t nX = SKILLS_GetParam1(pSkill);
        int32_t nY = SKILLS_GetParam2(pSkill);
        SKILLS_SetParam4(pSkill, gnClientLoopCount);
        extern void __stdcall PLAYER_StartMoveToTarget(D2UnitStrc* pUnit, int32_t nZero1, int32_t nZero2, int32_t nX, int32_t nY, int32_t nMode); // 0x6FB08DC0
        PLAYER_StartMoveToTarget(pUnit, 0, 0, nX, nY, *(uint8_t*)((uint8_t*)pSkillsTxt + 0x35));
        SKILLS_SetFlags(pSkill, 0x22);
    }
    return 1;
}

// 1.12a: 0x6FB4E260 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_BaalTentacleOrFesteringAppendages(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern D2UnitStrc* __fastcall UNIT_FindUnitById(int32_t nUnitId, int32_t nUnitType); // 0x6FACF9C0
    extern D2UnitStrc* __stdcall MISSILE_CreateClientMissile(int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nParam); // 0x6FAFC400

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        int32_t nTargetId = SKILLS_GetParam1(pSkill);
        int32_t nTargetType = SKILLS_GetParam2(pSkill);
        if (nTargetId != -1)
        {
            D2UnitStrc* pTarget = UNIT_FindUnitById(nTargetId, nTargetType);
            if (pTarget)
            {
                int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
                if (nMissileId >= 0)
                {
                    int32_t nX = (*((uint32_t*)pTarget) <= 3 || *((uint32_t*)pTarget) == 5)
                        ? PATH_GetXPosition(*(void**)((uint8_t*)pTarget + 0x2C))
                        : *(int32_t*)(*(uint8_t**)((uint8_t*)pTarget + 0x2C) + 0x0C);
                    int32_t nY = (*((uint32_t*)pTarget) <= 3 || *((uint32_t*)pTarget) == 5)
                        ? PATH_GetYPosition(*(void**)((uint8_t*)pTarget + 0x2C))
                        : *(int32_t*)(*(uint8_t**)((uint8_t*)pTarget + 0x2C) + 0x10);
                    MISSILE_CreateClientMissile(nMissileId, pUnit, nSkillId, nSkillLevel, nX, nY, 0, 0, 0);
                }
                int32_t nSpawnId = 0, nSpawnCount = 0, nSpawnMode = 0;
                MONSTERS_GetMinionSpawnInfo(pUnit, &nSpawnId, &nSpawnCount, &nSpawnMode);
                if (nSpawnId >= 0 && (D2MonStatsTxt*)((uint8_t*)(*(void**)((uint8_t*)sgptDataTables + 0xA78)) + nSpawnId * 0x1A8) != NULL)
                {
                    extern void __fastcall UNIT_LoadMonsterPaletteAssets(int32_t nClassId, int32_t nZero, uint8_t* pComponents); // 0x6FAE1F70
                    uint8_t aComponents[16] = { 0 };
                    UNIT_LoadMonsterPaletteAssets(nSpawnId, 0, aComponents);
                }
            }
        }
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4E430 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_NihlathakCorpseExplosion(D2UnitStrc* pUnit, int32_t nSkillId)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern D2UnitStrc* __fastcall UNIT_FindUnitById(int32_t nUnitId, int32_t nUnitType); // 0x6FACF9C0
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    extern void __stdcall PARTICLE_SpawnCorpseExplosionGore(D2UnitStrc* pCorpse, int32_t nGoreType, int32_t nZero); // 0x6FB75BA0

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        int32_t nTargetId = SKILLS_GetParam1(pSkill);
        int32_t nTargetType = SKILLS_GetParam2(pSkill);
        if (nTargetId != -1)
        {
            D2UnitStrc* pCorpse = UNIT_FindUnitById(nTargetId, nTargetType);
            if (!pCorpse || *((int32_t*)pCorpse + 4) != 12)
                return 0;
            *((uint32_t*)pCorpse + 0x31) &= ~0x00000004u;
            int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x88);
            if (nSoundId > 0)
                UNIT_PlaySound(pCorpse, nSoundId);
            for (int32_t i = 0; i < 6; ++i)
                PARTICLE_SpawnCorpseExplosionGore(pCorpse, 1, 0);
            for (int32_t i = 0; i < 6; ++i)
                PARTICLE_SpawnCorpseExplosionGore(pCorpse, 2, 0);
            PARTICLE_SpawnCorpseExplosionGore(pCorpse, 0, 0);
        }
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4E550 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_OverseerWhip(D2UnitStrc* pUnit, int32_t nSkillId)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern D2UnitStrc* __fastcall UNIT_FindUnitById(int32_t nUnitId, int32_t nUnitType); // 0x6FACF9C0
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    extern void __stdcall PARTICLE_SpawnOverlayOrEffect(D2UnitStrc* pUnit, int32_t nArg1, int32_t nArg2, int32_t nArg3); // 0x6FB75BA0

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        int32_t nTargetId = SKILLS_GetParam1(pSkill);
        int32_t nTargetType = SKILLS_GetParam2(pSkill);
        if (nTargetId != -1)
        {
            D2UnitStrc* pTarget = UNIT_FindUnitById(nTargetId, nTargetType);
            if (!pTarget)
                return 0;
            int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x88);
            if (nSoundId > 0)
                UNIT_PlaySound(pTarget, nSoundId);
            PARTICLE_SpawnOverlayOrEffect(pTarget, 0, 1, 0);
        }
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4E630 (__fastcall)
int32_t __fastcall SKILLSMON_CltSt_BaalNova(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    extern void __stdcall SKILLS_LaunchBaalNovaMissiles(D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nCount, int32_t nVel); // 0x6FAFCDB0

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    int16_t nSoundId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x84);
    if (nSoundId > 0)
        UNIT_PlaySound(pUnit, nSoundId);

    int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x40);
    if (nMissileId >= 0)
        SKILLS_LaunchBaalNovaMissiles(pUnit, nMissileId, nSkillId, 1, *(int32_t*)((uint8_t*)pSkillsTxt + 0x154), *(int32_t*)((uint8_t*)pSkillsTxt + 0x158));

    SKILLS_SetFlags(pSkill, 1);
    void* pMonSeq = DATATBLS_GetMonSeqTxtRecordFromUnit(pUnit);
    if (!pMonSeq)
        return 1;
    int32_t nFrameCount = 0, nDirCount = 0;
    D2Common_11066(pUnit, pMonSeq, &nFrameCount, &nDirCount);
    int32_t nStep = (nFrameCount << 8) / 64;
    if (nStep <= 0)
        nStep = 1;
    SKILLS_SetParam3(pSkill, nStep);
    SKILLS_SetParam4(pSkill, nStep);
    return 1;
}

// 1.12a: 0x6FB4E710 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_BaalNova(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern void __stdcall SKILLS_LaunchBaalNovaMissiles(D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nCount, int32_t nVel); // 0x6FAFCDB0

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    int32_t nCurPos = *((int32_t*)pUnit + 0x10);
    int32_t nNextTrigger = SKILLS_GetParam4(pSkill);
    int32_t nStep = SKILLS_GetParam3(pSkill);
    if (nStep <= 0)
        return 0;
    if (nCurPos >= nNextTrigger)
    {
        do
        {
            nNextTrigger += nStep;
        } while (nCurPos >= nNextTrigger);
        SKILLS_SetParam4(pSkill, nNextTrigger);
        int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
        if (nMissileId < 0)
            return 0;
        SKILLS_LaunchBaalNovaMissiles(pUnit, nMissileId, nSkillId, nSkillLevel, *(int32_t*)((uint8_t*)pSkillsTxt + 0x154), 0);
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// 1.12a: 0x6FB4E820 (__fastcall)
int32_t __fastcall SKILLSMON_CltDo_BaalColdMissiles(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nFrameNo = 0, nFrameCount = 0;
    if (!pUnit)
        return 0;
    extern D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId); // 0x6FAFC270
    extern D2UnitStrc* __stdcall MISSILE_CreateClientMissile(int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nParam); // 0x6FAFC400

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
        return 0;
    int16_t nMissileId = *(int16_t*)((uint8_t*)pSkillsTxt + 0x3C);
    if (nMissileId < 0)
        return 0;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
        return 0;

    if ((SKILLS_GetFlags(pSkill) & 0x04) != 0)
    {
        int32_t nX = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
            ? PATH_GetXPosition(*(void**)((uint8_t*)pUnit + 0x2C))
            : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x0C);
        int32_t nY = (*((uint32_t*)pUnit) <= 3 || *((uint32_t*)pUnit) == 5)
            ? PATH_GetYPosition(*(void**)((uint8_t*)pUnit + 0x2C))
            : *(int32_t*)(*(uint8_t**)((uint8_t*)pUnit + 0x2C) + 0x10);
        int32_t nTargetX = SKILLS_GetParam1(pSkill);
        int32_t nTargetY = SKILLS_GetParam2(pSkill);
        if (!nTargetX || !nTargetY)
        {
            extern int32_t __fastcall SKILLS_GetUnitTargetCoords(D2UnitStrc* pUnit, int32_t* pX, int32_t* pY); // 0x6FAFC980
            if (!SKILLS_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
            {
                nTargetX = nX + 1;
                nTargetY = nY + 1;
            }
        }
        MISSILE_CreateClientMissile(nMissileId, pUnit, nSkillId, nSkillLevel, nX, nY, nTargetX, nTargetY, 0);
    }
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);
    return nFrameNo != 0;
}

// ============================================================================
// Part 2: Client Realm / Exported Ordinals & S2C Monster AI Update (0x6FB4E930..0x6FB4EA7A)
// ============================================================================

// 1.12a: 0x6FB4E930 (__fastcall)
void __fastcall CLIENT_SetRealmStatusValue(int32_t nValue)
{
    gnRealmStatusValue = nValue;
}

// 1.12a: 0x6FB4E940 (__stdcall, Exported D2Client.#10002)
void __stdcall D2Client_10002_SetRealmInfo(int32_t bIsExpansion, const char* szRealmName, int32_t nStatus)
{
    gbIsExpansionClient = bIsExpansion;
    strncpy(gszRealmName, szRealmName, 0xC8);
    gnRealmStatusValue = nStatus;
}

// 1.12a: 0x6FB4E980 (__stdcall, Exported D2Client.#10001)
int32_t __stdcall D2Client_10001_GetRealmStatusValue(void)
{
    return gnRealmStatusValue;
}

// 1.12a: 0x6FB4E990 (__fastcall)
int32_t __fastcall CLIENT_GetCurrentFrameTick(void)
{
    if (!gpPlayerUnit)
        return 0;
    return gnClientLoopCount;
}

// 1.12a: 0x6FB4E9B0 (__fastcall)
uint32_t __fastcall CLIENT_GetGameTickCounter(void)
{
    return gdwGameTickCounter;
}

// 1.12a: 0x6FB4E9C0 (__fastcall)
void __fastcall S2C_HandleMonsterAiAndLifeUpdate(const uint8_t* pPacket)
{
    extern D2UnitStrc* __fastcall UNIT_FindUnitById(int32_t nUnitId, int32_t nUnitType); // 0x6FACF9C0
    D2UnitStrc* pUnit = UNIT_FindUnitById(*(const int32_t*)(pPacket + 1), 1);
    if (!pUnit)
        return;

    int32_t nMode = *((int32_t*)pUnit + 4);
    if (nMode == 0 || nMode == 12)
        return;

    MONSTERAI_UpdateAiState(pUnit, pPacket[5]);

    uint8_t nNewLifePct = pPacket[10];
    int32_t nCurLifePct = UNITS_GetCurrentLifePercentage(pUnit);
    if ((nNewLifePct & 0x80) != 0)
    {
        nNewLifePct &= 0x7F;
        if (nCurLifePct > nNewLifePct)
        {
            extern void __stdcall UNIT_TriggerHitHighlight(D2UnitStrc* pUnit); // 0x6FAD1600
            UNIT_TriggerHitHighlight(pUnit);
        }
    }
    int32_t nMaxLife = STATLIST_UnitGetStatValue(pUnit, 7, 0);
    int32_t nScaledLife = 2 * nNewLifePct;
    if (nMaxLife > 0)
        nScaledLife = (nNewLifePct * (nMaxLife >> 7)) & ~0xFF;
    STATLIST_SetUnitStat(pUnit, 6, nScaledLife, 0);

    int32_t nCurX = UNITS_GetSubtileX(pUnit);
    int32_t nCurY = UNITS_GetSubtileY(pUnit);
    uint16_t nDstX = *(const uint16_t*)(pPacket + 6);
    uint16_t nDstY = *(const uint16_t*)(pPacket + 8);
    int32_t nDistX = (nCurX >= nDstX) ? (nCurX - nDstX) : (nDstX - nCurX);
    int32_t nDistY = (nCurY >= nDstY) ? (nCurY - nDstY) : (nDstY - nCurY);
    if (nDistX > 5 || nDistY > 5)
    {
        extern void __stdcall MONSTER_TeleportToCoords(D2UnitStrc* pUnit, int32_t nX, int32_t nY); // 0x6FAE1CC0
        MONSTER_TeleportToCoords(pUnit, nDstX, nDstY);
    }
}

// ============================================================================
// Part 3: Cursor State & Sprite Subsystem (0x6FB4EA80..0x6FB4F6B4)
// ============================================================================

// .data: 0x6FB8D788..0x6FB8D7B7 - Cursor window message callbacks
static D2InputCallbackStrc gCursorRButtonUpCallbacks[1] = {
    { 0, WM_RBUTTONUP, CURSOR_OnRButtonUpReset } // 0x6FB8D788 (also 0x6FB8D790 with WM_SYSKEYUP in table)
};

// .data: 0x6FB8D794..0x6FB8D7B7 - Cursor mouse move callbacks
static D2InputCallbackStrc gCursorMouseMoveCallbacks[3] = {
    { 0, WM_MOUSEMOVE,   CURSOR_OnMouseMove   }, // 0x6FB8D794
    { 0, WM_NCMOUSEMOVE, CURSOR_OnNcMouseMove }, // 0x6FB8D7A0
    { 0, WM_LBUTTONUP,   CURSOR_OnMouseMove   }  // 0x6FB8D7AC
};

// .data: 0x6FB8D7B8..0x6FB8D894 - Cursor descriptor table (7 entries + sentinel)
static D2CursorDescStrc gCursorDescTable[8] = {
    { 0, 0, 1,  0, 1, CURSOR_UpdateAnimatedFrame, "Gaunt"   }, // [0] 0x6FB8D7B8
    { 1, 0, 8,  0, 1, CURSOR_UpdateAnimatedFrame, "grasp"   }, // [1] 0x6FB8D7D4
    { 1, 0, 8, 64, 1, CURSOR_UpdateAnimatedFrame, "ohand"   }, // [2] 0x6FB8D7F0
    { 1, 1, 8, 32, 1, CURSOR_UpdateAnimatedFrame, "orotate" }, // [3] 0x6FB8D80C
    { 1, 1, 8, 64, 1, CURSOR_UpdateAnimatedFrame, "ppress"  }, // [4] 0x6FB8D828
    { 1, 1, 8, 64, 1, CURSOR_UpdateAnimatedFrame, "protate" }, // [5] 0x6FB8D844
    { 0, 0, 1,  0, 0, CURSOR_UpdateStaticFrame,   "buysell" }, // [6] 0x6FB8D860
    { 7, 0, 0, -1, 0, (void (__fastcall*)(void))-1, (const char*)-1 } // [7] 0x6FB8D87C (active cursor state slot!)
};

// 1.12a: 0x6FB4EA80 (__fastcall)
void __fastcall CURSOR_UnlockTooltip(void)
{
    gbCursorTooltipLocked = 0;
}

// 1.12a: 0x6FB4EA90 (__fastcall)
int32_t __fastcall CURSOR_IsTooltipLocked(void)
{
    return gbCursorTooltipLocked;
}

// 1.12a: 0x6FB4EAA0 (__fastcall)
void __fastcall CURSOR_LockItemTooltip(D2UnitStrc* pItem)
{
    gbCursorTooltipLocked = 1;
    gpCursorLockedItem = pItem;
}

// 1.12a: 0x6FB4EAC0 (__fastcall)
int32_t __fastcall CURSOR_GetMouseY(void)
{
    return gnMouseY;
}

// 1.12a: 0x6FB4EAD0 (__fastcall)
int32_t __fastcall CURSOR_GetMouseX(void)
{
    return gnMouseX;
}

// 1.12a: 0x6FB4EAE0 (__fastcall)
void __fastcall CURSOR_UpdateStaticFrame(void)
{
    int32_t nEndFrame = gCursorDescTable[7].nEndFrame;
    if ((gnCursorFrameFixed8 >> 8) == nEndFrame)
    {
        if (gCursorDescTable[7].bLoop == 0)
            gCursorDescTable[7].nFrameDeltaFixed8 = 0;
        else
            gnCursorFrameFixed8 = gCursorDescTable[7].nTotalFrames << 8;
    }
    int32_t nDelta = gCursorDescTable[7].nFrameDeltaFixed8;
    if (nDelta)
    {
        gnCursorFrameFixed8 += nDelta;
        if ((gnCursorFrameFixed8 >> 8) < 0)
            gnCursorFrameFixed8 = (gCursorDescTable[7].nTotalFrames - 1) << 8;
    }
}

// 1.12a: 0x6FB4EB40 (__fastcall)
int32_t __fastcall CURSOR_GetSoundId(void)
{
    return gnCursorSoundId;
}

// 1.12a: 0x6FB4EB50 (__fastcall)
void __fastcall CURSOR_SetHoverUnit(int32_t nUnitId, int32_t nUnitType, int32_t nAnimFrame)
{
    if (gnCursorState == 6)
    {
        gnCursorHoverUnitId = nUnitId;
        gnCursorHoverUnitType = nUnitType;
        gnCursorFrameFixed8 = nAnimFrame << 8;
    }
}

// 1.12a: 0x6FB4EB80 (__fastcall)
int32_t __fastcall CURSOR_GetCurrentFrame(void)
{
    return gnCursorFrameFixed8 >> 8;
}

// 1.12a: 0x6FB4EB90 (__stdcall)
int32_t __stdcall CURSOR_OnNcMouseMove(D2WindowMsgStrc* pMsg)
{
    int32_t bHandled = 0;
    SMsgBreakHandlerChain(&bHandled);
    return 0;
}

// 1.12a: 0x6FB4EBB0 (__fastcall)
int32_t __fastcall CURSOR_GetState(void)
{
    return gnCursorState;
}

// 1.12a: 0x6FB4EBC0 (__fastcall)
void __fastcall CURSOR_SetDefaultHand(void)
{
    gnCursorState = 1;
    memcpy(&gCursorDescTable[7], &gCursorDescTable[1], sizeof(D2CursorDescStrc));
    gnCursorAnimId = gCursorDescTable[7].nAnimId;
    gnCursorFrameFixed8 = 0;
}

// 1.12a: 0x6FB4EBF0 (__fastcall)
void __fastcall CURSOR_SetNormalIfHoldingItem(void)
{
    if (gnCursorState == 2)
        gnCursorState = 1;
}

// 1.12a: 0x6FB4EC00 (__fastcall)
void __fastcall CURSOR_SetItemSprite(D2CellFileStrc* pCellFile, uint8_t nColorIndex)
{
    if (gCursorDescTable[7].nAnimId == 7)
    {
        FOG_DisplayHalt(__FILE__, 0x203);
        exit(-1);
    }
    gpCursorItemCellFile = pCellFile;
    gnCursorItemColorIndex = nColorIndex;
    gnCursorState = 2;
}

// 1.12a: 0x6FB4EC50 (__fastcall)
void __fastcall CURSOR_Update(void)
{
    if (gCursorDescTable[7].nAnimId == 7)
    {
        FOG_DisplayHalt(__FILE__, 0x1E0);
        exit(-1);
    }
    gCursorDescTable[7].pfnUpdate();
}

// 1.12a: 0x6FB4EC90 (__stdcall)
int32_t __stdcall CURSOR_OnMouseMove(D2WindowMsgStrc* pMsg)
{
    int32_t bHandled = 0;
    uint32_t lParam = (uint32_t)pMsg->lParam;
    gnMouseX = (uint16_t)lParam;
    gnMouseY = lParam >> 16;
    if (pMsg->uMsg == WM_LBUTTONUP)
        SMsgBreakHandlerChain(&bHandled);
    return 0;
}

// 1.12a: 0x6FB4ECD0 (custom usercall: ebx = nNewMouseX, edi = nNewMouseY)
void __fastcall CURSOR_WarpMousePosition(int32_t nNewMouseX, int32_t nNewMouseY)
{
    POINT pt;
    pt.x = nNewMouseX - gnMouseX;
    pt.y = nNewMouseY - gnMouseY;
    gnMouseX = nNewMouseX;
    gnMouseY = nNewMouseY;
    HWND hWnd = WINDOW_GetWindow();
    ClientToScreen(hWnd, &pt);
    POINT ptCur;
    GetCursorPos(&ptCur);
    SetCursorPos(ptCur.x + pt.x, ptCur.y + pt.y);
}

// 1.12a: 0x6FB4ED70 (__fastcall)
void __fastcall CURSOR_FreeAssets(void)
{
    extern void __fastcall ARCHIVE_FreeCellFile(D2CellFileStrc* pCellFile);
    for (int32_t i = 0; i < 7; ++i)
    {
        D2CellFileStrc* pCell = gapCursorCellFiles[i];
        if (pCell)
        {
            D2CMP_CelFileFreeHardware(pCell);
            FOG_Free(pCell, __FILE__, 0x10B, 0);
            gapCursorCellFiles[i] = NULL;
        }
    }
    HWND hWnd = WINDOW_GetWindow();
    extern void __fastcall INPUT_UnregisterCallbacks(int32_t nCount, HWND hWnd, D2InputCallbackStrc* pTable); // 0x6FB1D0A0
    INPUT_UnregisterCallbacks(3, hWnd, gCursorMouseMoveCallbacks);
}

// 1.12a: 0x6FB4EDF0 (__fastcall)
void __fastcall CURSOR_ResetToDefault(void)
{
    if (gCursorDescTable[7].nAnimId == 7)
    {
        FOG_DisplayHalt(__FILE__, 0xDF);
        exit(-1);
    }
    if (gnCursorState == 6)
    {
        HWND hWnd = WINDOW_GetWindow();
        extern void __fastcall INPUT_UnregisterCallbacks(int32_t nCount, HWND hWnd, D2InputCallbackStrc* pTable); // 0x6FB1D0A0
        INPUT_UnregisterCallbacks(1, hWnd, gCursorRButtonUpCallbacks);
    }
    gnCursorState = 1;
    memcpy(&gCursorDescTable[7], &gCursorDescTable[1], sizeof(D2CursorDescStrc));
    gnCursorAnimId = gCursorDescTable[7].nAnimId;
    gnCursorFrameFixed8 = 0;
}

// 1.12a: 0x6FB4EE90 (__stdcall)
int32_t __stdcall CURSOR_OnRButtonUpReset(D2WindowMsgStrc* pMsg)
{
    int32_t bHandled = 0;
    CURSOR_ResetToDefault();
    extern void __fastcall UNIT_PlaySound(D2UnitStrc* pUnit, uint16_t nSoundId); // 0x6FAE7D00
    UNIT_PlaySound(gpPlayerUnit, 2);
    SMsgBreakHandlerChain(&bHandled);
    return 1;
}

// 1.12a: 0x6FB4EEC0 (__fastcall)
void __fastcall CURSOR_Init(void)
{
    extern D2CellFileStrc* __fastcall ARCHIVE_LoadCellFile(const char* szName, int32_t nType); // 0x6FB6EEB0
    for (int32_t i = 0; i < 7; ++i)
    {
        gapCursorCellFiles[i] = ARCHIVE_LoadCellFile(gCursorDescTable[i].szCelName, 0);
    }
    gnCursorState = 1;
    memcpy(&gCursorDescTable[7], &gCursorDescTable[1], sizeof(D2CursorDescStrc));
    gnCursorAnimId = gCursorDescTable[7].nAnimId;
    gnCursorFrameFixed8 = 0;
    HWND hWnd = WINDOW_GetWindow();
    extern void __fastcall INPUT_RegisterCallbacks(int32_t nCount, HWND hWnd, D2InputCallbackStrc* pTable); // 0x6FB1D100
    INPUT_RegisterCallbacks(3, hWnd, gCursorMouseMoveCallbacks);
}

// 1.12a: 0x6FB4EF20 (__fastcall)
void __fastcall CURSOR_OnResolutionChanged(int32_t nOldWidth, int32_t nOldHeight)
{
    int32_t nNewX = (nOldWidth >= gnScreenWidth)   ? (gnMouseX * 80 / 100) : (gnMouseX * 125 / 100);
    int32_t nNewY = (nOldHeight >= gnScreenHeight) ? (gnMouseY * 80 / 100) : (gnMouseY * 125 / 100);
    CURSOR_WarpMousePosition(nNewX, nNewY);
}

// 1.12a: 0x6FB4EF90 (__fastcall)
void __fastcall CURSOR_BeginTargetMode(int32_t nState, int32_t nSoundId, int32_t nDescIndex)
{
    if (gCursorDescTable[7].nAnimId == 7)
    {
        FOG_DisplayHalt(__FILE__, 0xB8);
        exit(-1);
    }
    if (nDescIndex < 0 || nDescIndex >= 7)
    {
        FOG_DisplayHalt(__FILE__, 0xBA);
        exit(-1);
    }
    if (nState != 3 && nState != 4 && nState != 5 && nState != 6)
    {
        FOG_DisplayHalt(__FILE__, 0xBF);
        exit(-1);
    }
    if (gnCursorState != 6 && nState == 6)
    {
        HWND hWnd = WINDOW_GetWindow();
        extern void __fastcall INPUT_RegisterCallbacks(int32_t nCount, HWND hWnd, D2InputCallbackStrc* pTable); // 0x6FB1D100
        INPUT_RegisterCallbacks(1, hWnd, gCursorRButtonUpCallbacks);
    }
    memcpy(&gCursorDescTable[7], &gCursorDescTable[nDescIndex], sizeof(D2CursorDescStrc));
    gnCursorAnimId = gCursorDescTable[7].nAnimId;
    gnCursorSoundId = nSoundId;
    gnCursorState = nState;
    gnCursorFrameFixed8 = 0;
    gnCursorHoverUnitId = 0;
    gnCursorHoverUnitType = 0;
}

// 1.12a: 0x6FB4F0B0 (__fastcall)
int32_t __fastcall CURSOR_CanItemBeTargeted(D2UnitStrc* pItem)
{
    if (gCursorDescTable[7].nAnimId == 7)
    {
        FOG_DisplayHalt(__FILE__, 0x170);
        exit(-1);
    }
    switch (gnCursorState)
    {
    case 1:
    case 2:
    case 5:
        return 1;
    case 3: // Identify cursor
        if (!ITEMS_CheckItemFlag(pItem, 0x10, 0x17C, __FILE__))
            return 1;
        return 0;
    case 4: // Repair cursor
        if (ITEMS_CheckItemFlag(pItem, 0x400000, 0x181, __FILE__))
            return 0;
        if (!STATLIST_UnitGetStatValue(pItem, 0x49, 0))
            return 0;
        if (STATLIST_UnitGetStatValue(pItem, 0x48, 0) < STATLIST_UnitGetStatValue(pItem, 0x49, 0))
            return 1;
        if (ITEMS_CheckItemTypeId(pItem, 0x2A) || ITEMS_CheckItemTypeId(pItem, 0x2B) ||
            ITEMS_CheckItemTypeId(pItem, 0x2C) || ITEMS_CheckItemTypeId(pItem, 0x57))
        {
            if (STATLIST_UnitGetStatValue(pItem, 0x46, 0) < STATLIST_UnitGetItemStatOrSkillStatValue(pItem, 0xFE, 0))
                return 1;
        }
        if (STATLIST_UnitGetItemStatOrSkillStatValue(pItem, 0xCC, 0) <= 0)
            return 0;
        return 1;
    default:
        return 0;
    }
}

// 1.12a: 0x6FB4F1E0 (__fastcall)
int32_t __fastcall CURSOR_CheckQuestTargetItem(D2UnitStrc* pItem)
{
    if (!pItem)
        return 0;
    switch (gnCursorHoverUnitType)
    {
    case 1: // Shenk socket reward (Lazurk)
    {
        int32_t nQuality = ITEMS_GetItemQuality(pItem);
        if (nQuality < 1 || nQuality > 8 || nQuality == 5 || nQuality == 7)
            return 0;
        if (!ITEMS_CheckIfSocketable(pItem))
            return 0;
        if (ITEMS_CheckItemFlag(pItem, 0x800, 0x1B1, __FILE__))
            return 0;
        if (!ITEMS_CheckItemFlag(pItem, 0x10, 0x1B6, __FILE__))
            return 0;
        if (ITEMS_CheckItemFlag(pItem, 0x4000000, 0x1BB, __FILE__))
            return 0;
        return 1;
    }
    case 2: // Charsi imbue
    {
        int32_t nQuality = ITEMS_GetItemQuality(pItem);
        if (nQuality < 1 || nQuality > 3)
            return 0;
        if (ITEMS_CheckItemFlag(pItem, 0x800, 0x1CD, __FILE__))
            return 0;
        return ITEMS_CanBeRare(pItem) != 0;
    }
    case 4: // Anya personalize
    {
        int32_t nQuality = ITEMS_GetItemQuality(pItem);
        if (nQuality < 1 || nQuality > 8)
            return 0;
        if (!ITEMS_CheckItemFlag(pItem, 0x10, 0x1C3, __FILE__))
            return 0;
        return ITEMS_CheckItemFlag(pItem, 0x1000000, 0x1C7, __FILE__) == 0;
    }
    default:
        return 0;
    }
}

// 1.12a: 0x6FB4F2F0 (__fastcall)
void __fastcall CURSOR_SyncCursorItemState(void)
{
    if (gCursorDescTable[7].nAnimId == 7)
    {
        FOG_DisplayHalt(__FILE__, 0x14B);
        exit(-1);
    }
    if (!gpPlayerUnit)
        return;

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(*(D2InventoryStrc**)((uint8_t*)gpPlayerUnit + 0x60));
    if (pCursorItem)
    {
        if (gnCursorState != 2)
        {
            extern D2CellFileStrc* __fastcall ITEM_GetCursorCellFile(D2UnitStrc* pItem); // 0x6FB487B0
            gpCursorItemCellFile = ITEM_GetCursorCellFile(pCursorItem);
            gnCursorItemColorIndex = 0;
            gnCursorState = 2;
        }
    }
    else if (gnCursorState == 2)
    {
        gnCursorState = 1;
        memcpy(&gCursorDescTable[7], &gCursorDescTable[1], sizeof(D2CursorDescStrc));
        gnCursorAnimId = gCursorDescTable[7].nAnimId;
        gnCursorFrameFixed8 = 0;
    }
}

// 1.12a: 0x6FB4F390 (__fastcall)
void __fastcall CURSOR_UpdateAnimatedFrame(void)
{
    extern int32_t __fastcall UI_IsAnyModalOrShopOpen(void); // 0x6FB3B210
    CURSOR_SyncCursorItemState();
    if (!UI_IsAnyModalOrShopOpen())
    {
        int32_t nEndFrame = gCursorDescTable[7].nEndFrame;
        if ((gnCursorFrameFixed8 >> 8) == nEndFrame)
        {
            if (gCursorDescTable[7].bLoop == 0)
                gCursorDescTable[7].nFrameDeltaFixed8 = 0;
            else
                gnCursorFrameFixed8 = gCursorDescTable[7].nTotalFrames << 8;
        }
        int32_t nDelta = gCursorDescTable[7].nFrameDeltaFixed8;
        if (nDelta)
        {
            gnCursorFrameFixed8 += nDelta;
            if ((gnCursorFrameFixed8 >> 8) < 0)
                gnCursorFrameFixed8 = (gCursorDescTable[7].nTotalFrames - 1) << 8;
        }
    }
}

// 1.12a: 0x6FB4F430 (__fastcall)
void __fastcall CURSOR_Draw(void)
{
    if (gCursorDescTable[7].nAnimId == 7)
    {
        FOG_DisplayHalt(__FILE__, 0x220);
        exit(-1);
    }
    extern int32_t __fastcall GAME_ShouldHideCursor(void); // 0x6FB2BC40
    if (GAME_ShouldHideCursor())
        return;

    CURSOR_SyncCursorItemState();
    D2GfxDataStrc gfxData;
    memset(&gfxData, 0, sizeof(gfxData));

    if (gnCursorState == 1 || gnCursorState == 3 || gnCursorState == 4 || gnCursorState == 5 || gnCursorState == 6)
    {
        gfxData.pCellFile = gapCursorCellFiles[gnCursorAnimId];
        gfxData.nDirection = 0;
        gfxData.nFrame = gnCursorFrameFixed8 >> 8;
        TEXTURE_CelDraw(&gfxData, gnMouseX, gnMouseY, 0xFFFFFFFF, 5, NULL);
        return;
    }

    if (gnCursorState != 2 || !gpPlayerUnit)
    {
        FOG_DisplayHalt(__FILE__, 0x26C);
        exit(-1);
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(*(D2InventoryStrc**)((uint8_t*)gpPlayerUnit + 0x60));
    if (!pCursorItem)
    {
        FOG_Trace("Trying to draw the item that isn't in the player cursor");
        gnCursorState = 1;
        return;
    }

    extern uint8_t* __stdcall ITEM_GetPaletteShiftTable(D2UnitStrc* pUnit, D2UnitStrc* pItem, uint8_t* pOutColorIndex, int32_t nTrans); // 0x6FB45040
    uint8_t nColorIdx = 0;
    uint8_t* pPalTable = ITEM_GetPaletteShiftTable(gpPlayerUnit, pCursorItem, &nColorIdx, 0);
    gfxData.pCellFile = gpCursorItemCellFile;
    gfxData.nDirection = 0;
    gfxData.nFrame = 0;
    if (pPalTable)
        TEXTURE_CelDrawColor(&gfxData, gnMouseX, gnMouseY, 0xFFFFFFFF, 5, nColorIdx);
    else
        TEXTURE_CelDraw(&gfxData, gnMouseX, gnMouseY, 0xFFFFFFFF, 5, NULL);
}

// 1.12a: 0x6FB4F5B0 (__fastcall)
void __fastcall CURSOR_DrawLockedItemTooltip(void)
{
    if (!gbCursorTooltipLocked || !gpPlayerUnit)
        return;

    D2InventoryStrc* pInv = *(D2InventoryStrc**)((uint8_t*)gpPlayerUnit + 0x60);
    if (!pInv)
        return;

    int32_t bFound = 0;
    for (D2UnitStrc* pItem = INV_GetFirstItem(pInv); pItem != NULL; pItem = INV_GetNextItem(pItem))
    {
        if (INVENTORY_UnitIsItem(pItem) && pItem == gpCursorLockedItem)
        {
            bFound = 1;
            break;
        }
    }
    if (!bFound)
    {
        gbCursorTooltipLocked = 0;
        return;
    }

    RECT rcSlot;
    extern void __stdcall INV_GetItemGridScreenRect(D2UnitStrc* pUnit, D2UnitStrc* pItem, RECT* pOutRect, int32_t nInvGridId); // 0x6FB55B80
    extern void __fastcall INV_DrawItemHoverTooltip(D2UnitStrc* pItem, int32_t nX, int32_t nY, int32_t nZero1, int32_t nZero2); // 0x6FB5DD50

    if (*((int32_t*)gpCursorLockedItem + 4) == 1) // ITEMMODE_EQUIP
    {
        INV_GetItemGridScreenRect(gpPlayerUnit, gpCursorLockedItem, &rcSlot, 0);
    }
    else
    {
        uint8_t nPage = *(uint8_t*)(*(uint8_t**)((uint8_t*)gpCursorLockedItem + 0x14) + 0x45);
        if (nPage == 0)
            INV_GetItemGridScreenRect(gpPlayerUnit, gpCursorLockedItem, &rcSlot, 2);
        else if (nPage == 4)
            INV_GetItemGridScreenRect(gpPlayerUnit, gpCursorLockedItem, &rcSlot, 9);
        else if (nPage == 3)
            INV_GetItemGridScreenRect(gpPlayerUnit, gpCursorLockedItem, &rcSlot, 8);
        else
            return;
    }
    INV_DrawItemHoverTooltip(gpCursorLockedItem, (rcSlot.left + rcSlot.right) / 2, rcSlot.top, 0, 0);
}

// ============================================================================
// Part 4: Keyboard/Mouse Shortcut Bindings & Command Dispatch (0x6FB4F6C0..0x6FB5360F)
// ============================================================================

// Forward declarations of command callbacks (0x6FB4FE50..0x6FB5360F)
void __fastcall CMDTBL_OnCharacterScreenDown(void);   // 0x6FB531A0 (cmd 0)
void __fastcall CMDTBL_OnInventoryScreenDown(void);   // 0x6FB52E90 (cmd 1)
void __fastcall CMDTBL_OnPartyScreenDown(void);       // 0x6FB52BB0 (cmd 2)
void __fastcall CMDTBL_OnMessageLogDown(void);        // 0x6FB52AA0 (cmd 3)
void __fastcall CMDTBL_OnQuestLogDown(void);          // 0x6FB534B0 (cmd 4)
void __fastcall CMDTBL_OnChatInputDown(void);         // 0x6FB52800 (cmd 5)
void __fastcall CMDTBL_OnHelpScreenDown(void);        // 0x6FB52660 (cmd 6)
void __fastcall CMDTBL_OnAutomapToggleDown(void);     // 0x6FB524A0 (cmd 7)
void __fastcall CMDTBL_OnAutomapCenterDown(void);     // 0x6FB51390 (cmd 8)
void __fastcall CMDTBL_OnAutomapFadeDown(void);       // 0x6FB4FF50 (cmd 9)
void __fastcall CMDTBL_OnAutomapPartyDown(void);      // 0x6FB4FF20 (cmd 10)
void __fastcall CMDTBL_OnAutomapPartyNamesDown(void); // 0x6FB4FEF0 (cmd 11)
void __fastcall CMDTBL_OnSkillTreeDown(void);         // 0x6FB521A0 (cmd 12)
void __fastcall CMDTBL_OnSkillSelectDown(void);       // 0x6FB52060 (cmd 13)
void __fastcall CMDTBL_OnSkill1Down(void);            // 0x6FB51930 (cmd 14)
void __fastcall CMDTBL_OnSkill2Down(void);            // 0x6FB518F0 (cmd 15)
void __fastcall CMDTBL_OnSkill3Down(void);            // 0x6FB518B0 (cmd 16)
void __fastcall CMDTBL_OnSkill4Down(void);            // 0x6FB51870 (cmd 17)
void __fastcall CMDTBL_OnSkill5Down(void);            // 0x6FB51830 (cmd 18)
void __fastcall CMDTBL_OnSkill6Down(void);            // 0x6FB517F0 (cmd 19)
void __fastcall CMDTBL_OnSkill7Down(void);            // 0x6FB517B0 (cmd 20)
void __fastcall CMDTBL_OnSkill8Down(void);            // 0x6FB51770 (cmd 21)
void __fastcall CMDTBL_OnBeltShowDown(void);          // 0x6FB51EF0 (cmd 22)
void __fastcall CMDTBL_OnUseBeltSlot1Down(void);      // 0x6FB53590 (cmd 23)
void __fastcall CMDTBL_OnUseBeltSlot2Down(void);      // 0x6FB53550 (cmd 24)
void __fastcall CMDTBL_OnUseBeltSlot3Down(void);      // 0x6FB53510 (cmd 25)
void __fastcall CMDTBL_OnUseBeltSlot4Down(void);      // 0x6FB534D0 (cmd 26)
void __fastcall CMDTBL_OnVoiceHelpMeDown(void);       // 0x6FB50F10 (cmd 27)
void __fastcall CMDTBL_OnVoiceFollowMeDown(void);     // 0x6FB50EF0 (cmd 28)
void __fastcall CMDTBL_OnVoiceThisIsForYouDown(void); // 0x6FB50ED0 (cmd 29)
void __fastcall CMDTBL_OnVoiceThanksDown(void);       // 0x6FB50EB0 (cmd 30)
void __fastcall CMDTBL_OnVoiceSorryDown(void);        // 0x6FB50E90 (cmd 31)
void __fastcall CMDTBL_OnVoiceByeDown(void);          // 0x6FB50E70 (cmd 32)
void __fastcall CMDTBL_OnVoiceDieDown(void);          // 0x6FB50E50 (cmd 33)
void __fastcall CMDTBL_OnRunHoldDown(void);           // 0x6FB51A30 (cmd 34 down)
void __fastcall CMDTBL_OnRunHoldUp(void);             // 0x6FB519E0 (cmd 34 up)
void __fastcall CMDTBL_OnToggleAlwaysRunDown(void);   // 0x6FB4FE90 (cmd 35)
void __fastcall CMDTBL_OnStandStillDown(void);        // 0x6FB4FE80 (cmd 36 down)
void __fastcall CMDTBL_OnStandStillUp(void);          // 0x6FB4FE50 (cmd 36 up)
void __fastcall CMDTBL_OnShowItemsDown(void);         // 0x6FB51E20 (cmd 37 down)
void __fastcall CMDTBL_OnShowItemsUp(void);           // 0x6FB51A90 (cmd 37 up)
void __fastcall CMDTBL_OnClearScreenDown(void);       // 0x6FB51310 (cmd 38)
void __fastcall CMDTBL_OnPrevSkillDown(void);         // 0x6FB512F0 (cmd 39)
void __fastcall CMDTBL_OnNextSkillDown(void);         // 0x6FB512D0 (cmd 40)
void __fastcall CMDTBL_OnClearMessagesDown(void);     // 0x6FB4FE60 (cmd 41)
extern void __fastcall HIREABLES_OnTogglePortraitsDown(void); // 0x6FB6E690 (cmd 43)
void __fastcall CMDTBL_OnSwapWeaponsDown(void);       // 0x6FB519B0 (cmd 44)
void __fastcall CMDTBL_OnAutomapLeftToggleDown(void); // 0x6FB4FEB0 (cmd 45)
void __fastcall CMDTBL_OnSkill9Down(void);            // 0x6FB51730 (cmd 46)
void __fastcall CMDTBL_OnSkill10Down(void);           // 0x6FB516F0 (cmd 47)
void __fastcall CMDTBL_OnSkill11Down(void);           // 0x6FB516B0 (cmd 48)
void __fastcall CMDTBL_OnSkill12Down(void);           // 0x6FB51670 (cmd 49)
void __fastcall CMDTBL_OnSkill13Down(void);           // 0x6FB51630 (cmd 50)
void __fastcall CMDTBL_OnSkill14Down(void);           // 0x6FB515F0 (cmd 51)
void __fastcall CMDTBL_OnSkill15Down(void);           // 0x6FB515B0 (cmd 52)
void __fastcall CMDTBL_OnSkill16Down(void);           // 0x6FB51570 (cmd 53)
void __fastcall CMDTBL_OnHireableScreenDown(void);    // 0x6FB51AD0 (cmd 54)
void __fastcall CMDTBL_OnVoiceRetreatDown(void);      // 0x6FB50E30 (cmd 55)
void __fastcall CMDTBL_OnEscMenuDown(void);           // 0x6FB53470 (cmd 56)

// .data: 0x6FB8D028..0x6FB8D49C - Default 114-entry key binding table (57 commands x 2 slots)
static D2KeyBindingStrc gDefaultKeyBindings[114] = {
    {  0, 0x0041, 1 }, {  0, 0x0043, 0 }, // cmd  0: Character Screen ('A', 'C')
    {  1, 0x0042, 0 }, {  1, 0x0049, 1 }, // cmd  1: Inventory Screen ('I', 'B')
    {  2, 0x0050, 1 }, {  2, 0xFFFF, 0 }, // cmd  2: Party Screen ('P')
    {  3, 0x004D, 1 }, {  3, 0xFFFF, 0 }, // cmd  3: Message Log ('M')
    {  4, 0x0051, 1 }, {  4, 0xFFFF, 0 }, // cmd  4: Quest Log ('Q')
    {  5, 0x000D, 1 }, {  5, 0xFFFF, 0 }, // cmd  5: Chat (VK_RETURN)
    {  6, 0x0048, 1 }, {  6, 0xFFFF, 0 }, // cmd  6: Help Screen ('H')
    {  7, 0x0009, 1 }, {  7, 0x0100, 0 }, // cmd  7: Automap (VK_TAB, Mouse3)
    {  8, 0x0078, 1 }, {  8, 0xFFFF, 0 }, // cmd  8: Center Automap (VK_F9)
    {  9, 0x0079, 1 }, {  9, 0xFFFF, 0 }, // cmd  9: Fade Automap (VK_F10)
    { 10, 0x007A, 1 }, { 10, 0xFFFF, 0 }, // cmd 10: Party on Automap (VK_F11)
    { 11, 0x007B, 1 }, { 11, 0xFFFF, 0 }, // cmd 11: Names on Automap (VK_F12)
    { 12, 0x0054, 1 }, { 12, 0xFFFF, 0 }, // cmd 12: Skill Tree ('T')
    { 13, 0x0053, 1 }, { 13, 0xFFFF, 0 }, // cmd 13: Skill Speed Bar ('S')
    { 14, 0x0070, 1 }, { 14, 0xFFFF, 0 }, // cmd 14: Skill 1 (VK_F1)
    { 15, 0x0071, 1 }, { 15, 0xFFFF, 0 }, // cmd 15: Skill 2 (VK_F2)
    { 16, 0x0072, 1 }, { 16, 0xFFFF, 0 }, // cmd 16: Skill 3 (VK_F3)
    { 17, 0x0073, 1 }, { 17, 0xFFFF, 0 }, // cmd 17: Skill 4 (VK_F4)
    { 18, 0x0074, 1 }, { 18, 0xFFFF, 0 }, // cmd 18: Skill 5 (VK_F5)
    { 19, 0x0075, 1 }, { 19, 0xFFFF, 0 }, // cmd 19: Skill 6 (VK_F6)
    { 20, 0x0076, 1 }, { 20, 0xFFFF, 0 }, // cmd 20: Skill 7 (VK_F7)
    { 22, 0x00C0, 1 }, { 22, 0xFFFF, 0 }, // cmd 22: Show Belt ('~')
    { 23, 0x0031, 1 }, { 23, 0xFFFF, 0 }, // cmd 23: Use Belt 1 ('1')
    { 24, 0x0032, 1 }, { 24, 0xFFFF, 0 }, // cmd 24: Use Belt 2 ('2')
    { 25, 0x0033, 1 }, { 25, 0xFFFF, 0 }, // cmd 25: Use Belt 3 ('3')
    { 26, 0x0034, 1 }, { 26, 0xFFFF, 0 }, // cmd 26: Use Belt 4 ('4')
    { 27, 0x0060, 1 }, { 27, 0xFFFF, 0 }, // cmd 27: Say "Help" (VK_NUMPAD0)
    { 28, 0x0061, 1 }, { 28, 0xFFFF, 0 }, // cmd 28: Say "Follow me" (VK_NUMPAD1)
    { 29, 0x0062, 1 }, { 29, 0xFFFF, 0 }, // cmd 29: Say "This is for you" (VK_NUMPAD2)
    { 30, 0x0063, 1 }, { 30, 0xFFFF, 0 }, // cmd 30: Say "Thanks" (VK_NUMPAD3)
    { 31, 0x0064, 1 }, { 31, 0xFFFF, 0 }, // cmd 31: Say "Sorry" (VK_NUMPAD4)
    { 32, 0x0065, 1 }, { 32, 0xFFFF, 0 }, // cmd 32: Say "Bye" (VK_NUMPAD5)
    { 33, 0x0066, 1 }, { 33, 0xFFFF, 0 }, // cmd 33: Say "Die" (VK_NUMPAD6)
    { 34, 0x0011, 1 }, { 34, 0xFFFF, 0 }, // cmd 34: Run (VK_CONTROL)
    { 35, 0x0052, 1 }, { 35, 0x0102, 0 }, // cmd 35: Toggle Run/Walk ('R', Mouse5)
    { 36, 0x0010, 1 }, { 36, 0xFFFF, 0 }, // cmd 36: Stand Still (VK_SHIFT)
    { 37, 0x0012, 1 }, { 37, 0x0101, 0 }, // cmd 37: Show Items (VK_MENU, Mouse4)
    { 38, 0x0020, 1 }, { 38, 0xFFFF, 0 }, // cmd 38: Clear Screen (VK_SPACE)
    { 39, 0x0103, 1 }, { 39, 0xFFFF, 0 }, // cmd 39: Select Previous Skill (WheelUp)
    { 40, 0x0104, 1 }, { 40, 0xFFFF, 0 }, // cmd 40: Select Next Skill (WheelDown)
    { 41, 0x004E, 1 }, { 41, 0xFFFF, 0 }, // cmd 41: Clear All Messages ('N')
    { 42, 0x002C, 1 }, { 42, 0xFFFF, 0 }, // cmd 42: Screen Shot (VK_SNAPSHOT)
    { 43, 0x005A, 1 }, { 43, 0xFFFF, 0 }, // cmd 43: Portraits ('Z')
    { 44, 0x0057, 1 }, { 44, 0xFFFF, 0 }, // cmd 44: Swap Weapons ('W')
    { 45, 0x0056, 1 }, { 45, 0xFFFF, 0 }, // cmd 45: Toggle MiniMap ('V')
    { 21, 0x0077, 1 }, { 21, 0xFFFF, 0 }, // cmd 21: Skill 8 (VK_F8)
    { 46, 0xFFFF, 1 }, { 46, 0xFFFF, 0 }, // cmd 46: Skill 9
    { 47, 0xFFFF, 1 }, { 47, 0xFFFF, 0 }, // cmd 47: Skill 10
    { 48, 0xFFFF, 1 }, { 48, 0xFFFF, 0 }, // cmd 48: Skill 11
    { 49, 0xFFFF, 1 }, { 49, 0xFFFF, 0 }, // cmd 49: Skill 12
    { 50, 0xFFFF, 1 }, { 50, 0xFFFF, 0 }, // cmd 50: Skill 13
    { 51, 0xFFFF, 1 }, { 51, 0xFFFF, 0 }, // cmd 51: Skill 14
    { 52, 0xFFFF, 1 }, { 52, 0xFFFF, 0 }, // cmd 52: Skill 15
    { 53, 0xFFFF, 1 }, { 53, 0xFFFF, 0 }, // cmd 53: Skill 16
    { 54, 0x004F, 1 }, { 54, 0xFFFF, 0 }, // cmd 54: Hireling Screen ('O')
    { 55, 0x0067, 1 }, { 55, 0xFFFF, 0 }, // cmd 55: Say "Retreat" (VK_NUMPAD7)
    { 56, 0x001B, 1 }, { 56, 0xFFFF, 0 }  // cmd 56: Esc Menu (VK_ESCAPE)
};

// .data: 0x6FB8D4A0..0x6FB8D74C - Command dispatch table (57 entries)
static D2CmdDispatchStrc gCmdDispatchTable[57] = {
    { CMDTBL_OnCharacterScreenDown,   NULL,                    0 }, // cmd  0
    { CMDTBL_OnInventoryScreenDown,   NULL,                    0 }, // cmd  1
    { CMDTBL_OnPartyScreenDown,       NULL,                    0 }, // cmd  2
    { CMDTBL_OnMessageLogDown,        NULL,                    0 }, // cmd  3
    { CMDTBL_OnQuestLogDown,          NULL,                    0 }, // cmd  4
    { CMDTBL_OnChatInputDown,         NULL,                    1 }, // cmd  5
    { CMDTBL_OnHelpScreenDown,        NULL,                    0 }, // cmd  6
    { CMDTBL_OnAutomapToggleDown,     NULL,                    0 }, // cmd  7
    { CMDTBL_OnAutomapCenterDown,     NULL,                    0 }, // cmd  8
    { CMDTBL_OnAutomapFadeDown,       NULL,                    0 }, // cmd  9
    { CMDTBL_OnAutomapPartyDown,      NULL,                    0 }, // cmd 10
    { CMDTBL_OnAutomapPartyNamesDown, NULL,                    0 }, // cmd 11
    { CMDTBL_OnSkillTreeDown,         NULL,                    0 }, // cmd 12
    { CMDTBL_OnSkillSelectDown,       NULL,                    0 }, // cmd 13
    { CMDTBL_OnSkill1Down,            NULL,                    0 }, // cmd 14
    { CMDTBL_OnSkill2Down,            NULL,                    0 }, // cmd 15
    { CMDTBL_OnSkill3Down,            NULL,                    0 }, // cmd 16
    { CMDTBL_OnSkill4Down,            NULL,                    0 }, // cmd 17
    { CMDTBL_OnSkill5Down,            NULL,                    0 }, // cmd 18
    { CMDTBL_OnSkill6Down,            NULL,                    0 }, // cmd 19
    { CMDTBL_OnSkill7Down,            NULL,                    0 }, // cmd 20
    { CMDTBL_OnSkill8Down,            NULL,                    0 }, // cmd 21
    { CMDTBL_OnBeltShowDown,          NULL,                    1 }, // cmd 22
    { CMDTBL_OnUseBeltSlot1Down,      NULL,                    0 }, // cmd 23
    { CMDTBL_OnUseBeltSlot2Down,      NULL,                    0 }, // cmd 24
    { CMDTBL_OnUseBeltSlot3Down,      NULL,                    0 }, // cmd 25
    { CMDTBL_OnUseBeltSlot4Down,      NULL,                    0 }, // cmd 26
    { CMDTBL_OnVoiceHelpMeDown,       NULL,                    1 }, // cmd 27
    { CMDTBL_OnVoiceFollowMeDown,     NULL,                    1 }, // cmd 28
    { CMDTBL_OnVoiceThisIsForYouDown, NULL,                    1 }, // cmd 29
    { CMDTBL_OnVoiceThanksDown,       NULL,                    1 }, // cmd 30
    { CMDTBL_OnVoiceSorryDown,        NULL,                    1 }, // cmd 31
    { CMDTBL_OnVoiceByeDown,          NULL,                    1 }, // cmd 32
    { CMDTBL_OnVoiceDieDown,          NULL,                    1 }, // cmd 33
    { CMDTBL_OnRunHoldDown,           CMDTBL_OnRunHoldUp,      0 }, // cmd 34
    { CMDTBL_OnToggleAlwaysRunDown,   NULL,                    0 }, // cmd 35
    { CMDTBL_OnStandStillDown,        CMDTBL_OnStandStillUp,   0 }, // cmd 36
    { CMDTBL_OnShowItemsDown,         CMDTBL_OnShowItemsUp,    0 }, // cmd 37
    { CMDTBL_OnClearScreenDown,       NULL,                    0 }, // cmd 38
    { CMDTBL_OnPrevSkillDown,         NULL,                    0 }, // cmd 39
    { CMDTBL_OnNextSkillDown,         NULL,                    0 }, // cmd 40
    { CMDTBL_OnClearMessagesDown,     NULL,                    1 }, // cmd 41
    { NULL, (D2CmdCallback_t)D2Win_TakeScreenshot,             1 }, // cmd 42
    { HIREABLES_OnTogglePortraitsDown,NULL,                    0 }, // cmd 43
    { CMDTBL_OnSwapWeaponsDown,       NULL,                    1 }, // cmd 44
    { CMDTBL_OnAutomapLeftToggleDown, NULL,                    1 }, // cmd 45
    { CMDTBL_OnSkill9Down,            NULL,                    0 }, // cmd 46
    { CMDTBL_OnSkill10Down,           NULL,                    0 }, // cmd 47
    { CMDTBL_OnSkill11Down,           NULL,                    0 }, // cmd 48
    { CMDTBL_OnSkill12Down,           NULL,                    0 }, // cmd 49
    { CMDTBL_OnSkill13Down,           NULL,                    0 }, // cmd 50
    { CMDTBL_OnSkill14Down,           NULL,                    0 }, // cmd 51
    { CMDTBL_OnSkill15Down,           NULL,                    0 }, // cmd 52
    { CMDTBL_OnSkill16Down,           NULL,                    0 }, // cmd 53
    { CMDTBL_OnHireableScreenDown,    NULL,                    0 }, // cmd 54
    { CMDTBL_OnVoiceRetreatDown,      NULL,                    1 }, // cmd 55
    { CMDTBL_OnEscMenuDown,           NULL,                    0 }  // cmd 56
};

// .data: 0x6FB8D74C..0x6FB8D77B - Keyboard message handlers (4 entries)
static D2InputCallbackStrc gCmdTblKeyHandlers[4] = {
    { 0, WM_KEYDOWN,    CMDTBL_OnKeyDown }, // 0x6FB8D74C
    { 0, WM_SYSKEYDOWN, CMDTBL_OnKeyDown }, // 0x6FB8D758
    { 0, WM_KEYUP,      CMDTBL_OnKeyUp   }, // 0x6FB8D764
    { 0, WM_SYSKEYUP,   CMDTBL_OnKeyUp   }  // 0x6FB8D770
};

// .data: 0x6FB8D77C..0x6FB8D793 - Key-up-only message handlers (2 entries)
static D2InputCallbackStrc gCmdTblKeyUpOnlyHandlers[2] = {
    { 0, WM_KEYUP,    CMDTBL_OnKeyUp }, // 0x6FB8D77C
    { 0, WM_SYSKEYUP, CMDTBL_OnKeyUp }  // 0x6FB8D788
};

// 1.12a: 0x6FB4F6C0 (__stdcall)
int32_t __stdcall CMDTBL_OnKeyUp(D2WindowMsgStrc* pMsg)
{
    int32_t bHandled = 0;
    uint16_t wKey = (uint16_t)pMsg->wParam;

    int32_t nIndex = 0;
    while (nIndex < 114)
    {
        if (gKeyBindings[nIndex].wKey == wKey)
            break;
        ++nIndex;
    }
    if (nIndex >= 114)
        return 0;

    int32_t nCmdId = gKeyBindings[nIndex].nCommandId;
    if (nCmdId < 0 || nCmdId >= 57)
        return 0;

    D2CmdCallback_t pfnOnUp = gCmdDispatchTable[nCmdId].pfnOnUp;
    if (pfnOnUp)
    {
        if (IsBadCodePtr((FARPROC)pfnOnUp))
        {
            FOG_DisplayHalt(__FILE__, 0x3BD);
            exit(-1);
        }
        pfnOnUp();
    }
    SMsgBreakHandlerChain(&bHandled);
    return 0;
}

// 1.12a: 0x6FB4F7A0 (__stdcall)
int32_t __stdcall CMDTBL_OnKeyDown(D2WindowMsgStrc* pMsg)
{
    int32_t bHandled = 0;
    uint16_t wKey = (uint16_t)pMsg->wParam;

    int32_t nIndex = 0;
    while (nIndex < 114)
    {
        if (gKeyBindings[nIndex].wKey == wKey)
            break;
        ++nIndex;
    }
    if (nIndex >= 114)
        return 0;

    int32_t nCmdId = gKeyBindings[nIndex].nCommandId;
    if (nCmdId < 0 || nCmdId >= 57)
        return 0;

    if (!gCmdDispatchTable[nCmdId].bAllowInChat && (pMsg->dwKeyFlags & 0x40000000) != 0)
        return 0;

    if (wKey == VK_MENU && (GetKeyState(VK_TAB) & 0xFFFE) != 0)
        return 0;

    D2CmdCallback_t pfnOnDown = gCmdDispatchTable[nCmdId].pfnOnDown;
    if (pfnOnDown)
    {
        if (IsBadCodePtr((FARPROC)pfnOnDown))
        {
            FOG_DisplayHalt(__FILE__, 0x3AB);
            exit(-1);
        }
        pfnOnDown();
    }
    SMsgBreakHandlerChain(&bHandled);
    return 0;
}

// 1.12a: 0x6FB4F8A0 (__fastcall)
int32_t __fastcall CMDTBL_GetCommandIdForKey(uint16_t wKey)
{
    for (int32_t i = 0; i < 114; ++i)
    {
        if (gKeyBindings[i].wKey == wKey)
            return gKeyBindings[i].nCommandId;
    }
    return -1;
}

// 1.12a: 0x6FB4F8C0 (custom usercall: si = wKey)
int32_t __fastcall CMDTBL_IsKeyBindable(uint16_t wKey)
{
    switch (wKey)
    {
    case 0x01: case 0x02: case 0x03: case 0x04:
    case 0x15: case 0x17: case 0x18: case 0x19:
    case 0x1B: case 0x1C: case 0x1D: case 0x1E: case 0x1F:
    case 0x25: case 0x26: case 0x27: case 0x28: case 0x29:
    case 0x2B: case 0x5B: case 0x5C: case 0x5D: case 0x90:
        return 0;

    case 0x08: case 0x09: case 0x0C: case 0x0D:
    case 0x10: case 0x11: case 0x12: case 0x13: case 0x14:
    case 0x20: case 0x21: case 0x22: case 0x23: case 0x24:
    case 0x2A: case 0x2C: case 0x2D: case 0x2E: case 0x2F:
    case 0x60: case 0x61: case 0x62: case 0x63: case 0x64:
    case 0x65: case 0x66: case 0x67: case 0x68: case 0x69:
    case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E:
    case 0x6F: case 0x70: case 0x71: case 0x72: case 0x73:
    case 0x74: case 0x75: case 0x76: case 0x77: case 0x78:
    case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D:
    case 0x7E: case 0x7F: case 0x80: case 0x81: case 0x82:
    case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
    case 0x91: case 0xBA: case 0xBB: case 0xBC: case 0xBD:
    case 0xBE: case 0xBF: case 0xC0: case 0xDB: case 0xDC:
    case 0xDD: case 0xDE:
    case 0x100: case 0x101: case 0x102: case 0x103: case 0x104:
        return 1;

    default:
        if (isalnum(wKey) || ispunct(wKey))
            return 1;
        return 0;
    }
}

// 1.12a: 0x6FB4FA40 (custom usercall: esi = nCommandId, edx = bIsPrimary)
uint16_t __fastcall CMDTBL_GetBoundKey(int32_t nCommandId, int32_t bIsPrimary)
{
    for (int32_t i = 0; i < 114; ++i)
    {
        if (gKeyBindings[i].nCommandId == nCommandId && gKeyBindings[i].bIsPrimary == bIsPrimary)
            return gKeyBindings[i].wKey;
    }
    return 0xFFFF;
}

// 1.12a: 0x6FB4FA70 (custom usercall: ebx = dwDesiredAccess, esi = dwCreationDisposition)
HANDLE __fastcall CMDTBL_OpenPlayerKeyFile(DWORD dwDesiredAccess, DWORD dwCreationDisposition)
{
    char szSavePath[264] = { 0 };
    char szRealmDir[264] = { 0 };
    char szFilePath[264] = { 0 };

    FOG_GetSavePath(szSavePath, 260);
    if (gszRealmName[0] != '\0')
    {
        SStrPrintf(szRealmDir, 260, "%s%s\\", szSavePath, gszRealmName);
        FOG_CreatePathHierarchy(szRealmDir);
        SStrPrintf(szFilePath, 260, "%s%s.key", szRealmDir, gszAccountOrCharName);
        HANDLE hFile = FOG_CreateFileA(szFilePath, dwDesiredAccess, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
            return hFile;
    }
    SStrPrintf(szFilePath, 260, "%s%s.key", szSavePath, gszAccountOrCharName);
    return FOG_CreateFileA(szFilePath, dwDesiredAccess, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
}

// 1.12a: 0x6FB4FBA0 (custom usercall: eax = pBindingA, edx = pBindingB)
int32_t __fastcall CMDTBL_CompareKeyBindings(const D2KeyBindingStrc* pA, const D2KeyBindingStrc* pB)
{
    if (pA->nCommandId < pB->nCommandId)
        return -1;
    if (pA->nCommandId > pB->nCommandId)
        return 1;
    if (pA->bIsPrimary != 0)
        return (pB->bIsPrimary != 0) ? 0 : -1;
    return (pB->bIsPrimary != 0) ? 1 : 0;
}

// 1.12a: 0x6FB4FBE0 (custom usercall: ax = wKey, edi = nCommandId, esi = pOutErrorStringId)
int32_t __fastcall CMDTBL_ValidateMouseBinding(uint16_t wKey, int32_t nCommandId, uint16_t* pOutErrorStringId)
{
    extern int32_t __fastcall GAME_HasThreeButtonMouse(void); // 0x6FB2A4F0
    extern int32_t __fastcall GAME_HasFourButtonMouse(void);  // 0x6FB2A4C0
    extern int32_t __fastcall GAME_HasMouseWheelA(void);      // 0x6FB2A570
    extern int32_t __fastcall GAME_HasMouseWheelB(void);      // 0x6FB2A520

    if (wKey < 0x100 || wKey > 0x104)
        return 1;

    switch (wKey)
    {
    case 0x100: // Middle mouse button
        if (!GAME_HasThreeButtonMouse())
        {
            if (pOutErrorStringId)
                *pOutErrorStringId = 0x0F89;
            return 0;
        }
        return 1;
    case 0x101: // Mouse button 4
    case 0x102: // Mouse button 5
        if (!GAME_HasFourButtonMouse())
        {
            if (pOutErrorStringId)
                *pOutErrorStringId = 0x0F89;
            return 0;
        }
        return 1;
    case 0x103: // Mouse wheel up
    case 0x104: // Mouse wheel down
        if ((GAME_HasMouseWheelA() || GAME_HasMouseWheelB()) && gCmdDispatchTable[nCommandId].pfnOnUp == NULL)
            return 1;
        if (pOutErrorStringId)
            *pOutErrorStringId = 0x0F8A;
        return 0;
    default:
        return 1;
    }
}

// 1.12a: 0x6FB4FC60 (custom usercall: eax = nWheelDelta)
void __fastcall CMDTBL_DispatchMouseWheel(int32_t nWheelDelta)
{
    if (nWheelDelta > 0)
    {
        if (gpfnMouseWheelUp)
        {
            if (IsBadCodePtr((FARPROC)gpfnMouseWheelUp))
            {
                FOG_DisplayHalt(__FILE__, 0x383);
                exit(-1);
            }
            gpfnMouseWheelUp();
        }
    }
    else if (nWheelDelta < 0)
    {
        if (gpfnMouseWheelDown)
        {
            if (IsBadCodePtr((FARPROC)gpfnMouseWheelDown))
            {
                FOG_DisplayHalt(__FILE__, 0x38B);
                exit(-1);
            }
            gpfnMouseWheelDown();
        }
    }
}

// 1.12a: 0x6FB4FCD0 (__fastcall)
void __fastcall CMDTBL_DispatchMouseButton4Up(void)
{
    if (gpfnMouseMButtonUp)
    {
        if (IsBadCodePtr((FARPROC)gpfnMouseMButtonUp))
        {
            FOG_DisplayHalt(__FILE__, 0x375);
            exit(-1);
        }
        gpfnMouseMButtonUp();
    }
}

// 1.12a: 0x6FB4FD10 (__fastcall)
void __fastcall CMDTBL_DispatchMouseButton4Down(void)
{
    if (gpfnMouseMButtonDown)
    {
        if (IsBadCodePtr((FARPROC)gpfnMouseMButtonDown))
        {
            FOG_DisplayHalt(__FILE__, 0x369);
            exit(-1);
        }
        gpfnMouseMButtonDown();
    }
}

// 1.12a: 0x6FB4FD50 (__fastcall)
void __fastcall CMDTBL_DispatchMouseButton3Up(void)
{
    if (gpfnMouseRButtonUp)
    {
        if (IsBadCodePtr((FARPROC)gpfnMouseRButtonUp))
        {
            FOG_DisplayHalt(__FILE__, 0x35D);
            exit(-1);
        }
        gpfnMouseRButtonUp();
    }
}

// 1.12a: 0x6FB4FD90 (__fastcall)
void __fastcall CMDTBL_DispatchMouseButton3Down(void)
{
    if (gpfnMouseRButtonDown)
    {
        if (IsBadCodePtr((FARPROC)gpfnMouseRButtonDown))
        {
            FOG_DisplayHalt(__FILE__, 0x350);
            exit(-1);
        }
        gpfnMouseRButtonDown();
    }
}

// 1.12a: 0x6FB4FDD0 (__fastcall)
void __fastcall CMDTBL_DispatchMiddleMouseUp(void)
{
    if (gpfnMouseLButtonUp)
    {
        if (IsBadCodePtr((FARPROC)gpfnMouseLButtonUp))
        {
            FOG_DisplayHalt(__FILE__, 0x345);
            exit(-1);
        }
        gpfnMouseLButtonUp();
    }
}

// 1.12a: 0x6FB4FE10 (__fastcall)
void __fastcall CMDTBL_DispatchMiddleMouseDown(void)
{
    if (gpfnMouseLButtonDown)
    {
        if (IsBadCodePtr((FARPROC)gpfnMouseLButtonDown))
        {
            FOG_DisplayHalt(__FILE__, 0x339);
            exit(-1);
        }
        gpfnMouseLButtonDown();
    }
}

// 1.12a: 0x6FB4FE50 (__fastcall)
void __fastcall CMDTBL_OnStandStillUp(void)
{
    gbStandStillActive = 0;
}

// 1.12a: 0x6FB4FE60 (__fastcall)
void __fastcall CMDTBL_OnClearMessagesDown(void)
{
    extern void* gpChatMessageList; // 0x6FBB54CC
    uint8_t* pNode = (uint8_t*)gpChatMessageList;
    while (pNode != NULL)
    {
        *(int32_t*)(pNode + 0x1E) = 0;
        pNode = *(uint8_t**)(pNode + 0x22);
    }
}

// 1.12a: 0x6FB4FE80 (__fastcall)
void __fastcall CMDTBL_OnStandStillDown(void)
{
    gbStandStillActive = 1;
}

// 1.12a: 0x6FB4FE90 (__fastcall)
void __fastcall CMDTBL_OnToggleAlwaysRunDown(void)
{
    gbAlwaysRunEnabled = (gbAlwaysRunEnabled == 0);
}

// 1.12a: 0x6FB4FEB0 (__fastcall)
void __fastcall CMDTBL_OnAutomapLeftToggleDown(void)
{
    int32_t bNewVal = (gbAutomapLeftAlign == 0);
    SRegSaveValue("Diablo II", "AutoMap Left", 0, bNewVal);
    gbAutomapLeftAlign2 = bNewVal;
    gbAutomapLeftAlign3 = bNewVal;
    gbAutomapLeftAlign  = bNewVal;
}

// 1.12a: 0x6FB4FEF0 (__fastcall)
void __fastcall CMDTBL_OnAutomapPartyNamesDown(void)
{
    int32_t bNewVal = (gbAutomapShowPartyNames == 0);
    SRegSaveValue("Diablo II", "AutoMap Party Names", 0, bNewVal);
    gbAutomapShowPartyNames = bNewVal;
}

// 1.12a: 0x6FB4FF20 (__fastcall)
void __fastcall CMDTBL_OnAutomapPartyDown(void)
{
    int32_t bNewVal = (gbAutomapShowParty == 0);
    SRegSaveValue("Diablo II", "AutoMap Party", 0, bNewVal);
    gbAutomapShowParty = bNewVal;
}

// 1.12a: 0x6FB4FF50 (__fastcall)
void __fastcall CMDTBL_OnAutomapFadeDown(void)
{
    if (gbAutomapMinimapMode == 1 && gnAutomapFadeMode == 1)
    {
        gnAutomapFadeMode = (gnAutomapFadeDefault == 0) ? 2 : 0;
    }
    extern void __fastcall AUTOMAP_SetFadeMode(int32_t nMode); // 0x6FAED870
    AUTOMAP_SetFadeMode((gnAutomapFadeMode + 1) % 4);
}

// 1.12a: 0x6FB4FFA0 (custom usercall: eax = bEnable, esi = bKeepKeyUpHandlers)
void __fastcall CMDTBL_EnableKeyHandlers(int32_t bEnable, int32_t bKeepKeyUpHandlers)
{
    extern void __fastcall INPUT_UnregisterCallbacks(int32_t nCount, HWND hWnd, D2InputCallbackStrc* pTable); // 0x6FB1D0A0
    extern void __fastcall INPUT_RegisterCallbacks(int32_t nCount, HWND hWnd, D2InputCallbackStrc* pTable);   // 0x6FB1D100

    gbCmdTblInputEnabled = bEnable;
    if (bEnable)
    {
        if (gbCmdTblKeyHandlersRegistered == 0)
        {
            gbCmdTblKeyHandlersRegistered = 1;
            if (bKeepKeyUpHandlers && gbCmdTblAltKeyHandlersRegistered != 0)
            {
                INPUT_UnregisterCallbacks(2, WINDOW_GetWindow(), gCmdTblKeyUpOnlyHandlers);
                gbCmdTblAltKeyHandlersRegistered = 0;
            }
            INPUT_RegisterCallbacks(4, WINDOW_GetWindow(), gCmdTblKeyHandlers);
        }
    }
    else if (gbCmdTblKeyHandlersRegistered == 1)
    {
        gbCmdTblKeyHandlersRegistered = 0;
        INPUT_UnregisterCallbacks(4, WINDOW_GetWindow(), gCmdTblKeyHandlers);
        if (bKeepKeyUpHandlers && gbCmdTblAltKeyHandlersRegistered == 0)
        {
            INPUT_RegisterCallbacks(2, WINDOW_GetWindow(), gCmdTblKeyUpOnlyHandlers);
            gbCmdTblAltKeyHandlersRegistered = 1;
        }
    }
}

// 1.12a: 0x6FB50060 (custom usercall: esi = nCommandId, edx = bIsPrimary)
const Unicode* __fastcall CMDTBL_GetKeyNameString(int32_t nCommandId, int32_t bIsPrimary)
{
    uint16_t wKey = 0xFFFF;
    for (int32_t i = 0; i < 114; ++i)
    {
        if (gKeyBindings[i].nCommandId == nCommandId && gKeyBindings[i].bIsPrimary == bIsPrimary)
        {
            wKey = gKeyBindings[i].wKey;
            break;
        }
    }
    if (wKey == 0xFFFF)
        return D2LANG_GetStringFromTblIndex(0x0EB2);

    if (wKey > 0x00DF)
    {
        switch (wKey)
        {
        case 0x0100: return D2LANG_GetStringFromTblIndex(0x0EB6);
        case 0x0101: return D2LANG_GetStringFromTblIndex(0x0EB7);
        case 0x0102: return D2LANG_GetStringFromTblIndex(0x0EB8);
        case 0x0103: return D2LANG_GetStringFromTblIndex(0x0EB9);
        case 0x0104: return D2LANG_GetStringFromTblIndex(0x0EBA);
        default: break;
        }
    }
    else
    {
        switch (wKey)
        {
        case 0x01: return D2LANG_GetStringFromTblIndex(0x0EB3);
        case 0x02: return D2LANG_GetStringFromTblIndex(0x0EB4);
        case 0x03: return D2LANG_GetStringFromTblIndex(0x0EB5);
        case 0x04: return D2LANG_GetStringFromTblIndex(0x0EB6);
        case 0x08: return D2LANG_GetStringFromTblIndex(0x0ECE);
        case 0x09: return D2LANG_GetStringFromTblIndex(0x0ECF);
        case 0x0C: return D2LANG_GetStringFromTblIndex(0x0ED0);
        case 0x0D: return D2LANG_GetStringFromTblIndex(0x0ED1);
        case 0x10: return D2LANG_GetStringFromTblIndex(0x0ED2);
        case 0x11: return D2LANG_GetStringFromTblIndex(0x0ED3);
        case 0x12: return D2LANG_GetStringFromTblIndex(0x0ED4);
        case 0x13: return D2LANG_GetStringFromTblIndex(0x0ED5);
        case 0x14: return D2LANG_GetStringFromTblIndex(0x0ED6);
        case 0x15: return D2LANG_GetStringFromTblIndex(0x0EBB);
        case 0x17: return D2LANG_GetStringFromTblIndex(0x0EBC);
        case 0x18: return D2LANG_GetStringFromTblIndex(0x0EBD);
        case 0x19: return D2LANG_GetStringFromTblIndex(0x0EBE);
        case 0x1B: return D2LANG_GetStringFromTblIndex(0x0EBF);
        case 0x1C: return D2LANG_GetStringFromTblIndex(0x0EC0);
        case 0x1D: return D2LANG_GetStringFromTblIndex(0x0EC1);
        case 0x1E: return D2LANG_GetStringFromTblIndex(0x0EC2);
        case 0x1F: return D2LANG_GetStringFromTblIndex(0x0EC3);
        case 0x20: return D2LANG_GetStringFromTblIndex(0x0ED7);
        case 0x21: return D2LANG_GetStringFromTblIndex(0x0ED8);
        case 0x22: return D2LANG_GetStringFromTblIndex(0x0ED9);
        case 0x23: return D2LANG_GetStringFromTblIndex(0x0EDA);
        case 0x24: return D2LANG_GetStringFromTblIndex(0x0EDB);
        case 0x25: return D2LANG_GetStringFromTblIndex(0x0EC4);
        case 0x26: return D2LANG_GetStringFromTblIndex(0x0EC5);
        case 0x27: return D2LANG_GetStringFromTblIndex(0x0EC6);
        case 0x28: return D2LANG_GetStringFromTblIndex(0x0EC7);
        case 0x29: return D2LANG_GetStringFromTblIndex(0x0EC8);
        case 0x2A: return D2LANG_GetStringFromTblIndex(0x0EDC);
        case 0x2B: return D2LANG_GetStringFromTblIndex(0x0EC9);
        case 0x2C: return D2LANG_GetStringFromTblIndex(0x0EDD);
        case 0x2D: return D2LANG_GetStringFromTblIndex(0x0EDE);
        case 0x2E: return D2LANG_GetStringFromTblIndex(0x0EDF);
        case 0x2F: return D2LANG_GetStringFromTblIndex(0x0EE0);
        case 0x5B: return D2LANG_GetStringFromTblIndex(0x0ECA);
        case 0x5C: return D2LANG_GetStringFromTblIndex(0x0ECB);
        case 0x5D: return D2LANG_GetStringFromTblIndex(0x0ECC);
        case 0x60: return D2LANG_GetStringFromTblIndex(0x0EE1);
        case 0x61: return D2LANG_GetStringFromTblIndex(0x0EE2);
        case 0x62: return D2LANG_GetStringFromTblIndex(0x0EE3);
        case 0x63: return D2LANG_GetStringFromTblIndex(0x0EE4);
        case 0x64: return D2LANG_GetStringFromTblIndex(0x0EE5);
        case 0x65: return D2LANG_GetStringFromTblIndex(0x0EE6);
        case 0x66: return D2LANG_GetStringFromTblIndex(0x0EE7);
        case 0x67: return D2LANG_GetStringFromTblIndex(0x0EE8);
        case 0x68: return D2LANG_GetStringFromTblIndex(0x0EE9);
        case 0x69: return D2LANG_GetStringFromTblIndex(0x0EEA);
        case 0x6A: return D2LANG_GetStringFromTblIndex(0x0EEB);
        case 0x6B: return D2LANG_GetStringFromTblIndex(0x0EEC);
        case 0x6C: return D2LANG_GetStringFromTblIndex(0x0EED);
        case 0x6D: return D2LANG_GetStringFromTblIndex(0x0EEE);
        case 0x6E: return D2LANG_GetStringFromTblIndex(0x0EEF);
        case 0x6F: return D2LANG_GetStringFromTblIndex(0x0EF0);
        case 0x70: return D2LANG_GetStringFromTblIndex(0x0EF1);
        case 0x71: return D2LANG_GetStringFromTblIndex(0x0EF2);
        case 0x72: return D2LANG_GetStringFromTblIndex(0x0EF3);
        case 0x73: return D2LANG_GetStringFromTblIndex(0x0EF4);
        case 0x74: return D2LANG_GetStringFromTblIndex(0x0EF5);
        case 0x75: return D2LANG_GetStringFromTblIndex(0x0EF6);
        case 0x76: return D2LANG_GetStringFromTblIndex(0x0EF7);
        case 0x77: return D2LANG_GetStringFromTblIndex(0x0EF8);
        case 0x78: return D2LANG_GetStringFromTblIndex(0x0EF9);
        case 0x79: return D2LANG_GetStringFromTblIndex(0x0EFA);
        case 0x7A: return D2LANG_GetStringFromTblIndex(0x0EFB);
        case 0x7B: return D2LANG_GetStringFromTblIndex(0x0EFC);
        case 0x7C: return D2LANG_GetStringFromTblIndex(0x0EFD);
        case 0x7D: return D2LANG_GetStringFromTblIndex(0x0EFE);
        case 0x7E: return D2LANG_GetStringFromTblIndex(0x0EFF);
        case 0x7F: return D2LANG_GetStringFromTblIndex(0x0F00);
        case 0x80: return D2LANG_GetStringFromTblIndex(0x0F01);
        case 0x81: return D2LANG_GetStringFromTblIndex(0x0F02);
        case 0x82: return D2LANG_GetStringFromTblIndex(0x0F03);
        case 0x83: return D2LANG_GetStringFromTblIndex(0x0F04);
        case 0x84: return D2LANG_GetStringFromTblIndex(0x0F05);
        case 0x85: return D2LANG_GetStringFromTblIndex(0x0F06);
        case 0x86: return D2LANG_GetStringFromTblIndex(0x0F07);
        case 0x87: return D2LANG_GetStringFromTblIndex(0x0F08);
        case 0x90: return D2LANG_GetStringFromTblIndex(0x0ECD);
        case 0x91: return D2LANG_GetStringFromTblIndex(0x0F09);
        case 0xBA: return D2LANG_GetStringFromTblIndex(0x0F0A);
        case 0xBB: return D2LANG_GetStringFromTblIndex(0x0F0B);
        case 0xBC: return D2LANG_GetStringFromTblIndex(0x0F0C);
        case 0xBD: return D2LANG_GetStringFromTblIndex(0x0F0D);
        case 0xBE: return D2LANG_GetStringFromTblIndex(0x0F0E);
        case 0xBF: return D2LANG_GetStringFromTblIndex(0x0F0F);
        case 0xC0: return D2LANG_GetStringFromTblIndex(0x0F10);
        case 0xDB: return D2LANG_GetStringFromTblIndex(0x0F11);
        case 0xDC: return D2LANG_GetStringFromTblIndex(0x0F12);
        case 0xDD: return D2LANG_GetStringFromTblIndex(0x0F13);
        case 0xDE: return D2LANG_GetStringFromTblIndex(0x0F14);
        default: break;
        }
    }

    memset(gwszKeyCharBuf, 0, sizeof(gwszKeyCharBuf));
    *(uint16_t*)&gwszKeyCharBuf[0] = (uint8_t)wKey;
    return gwszKeyCharBuf;
}

// 1.12a: 0x6FB50740 (custom usercall: edx = nCommandId, esi = bIsPrimary)
int32_t __fastcall CMDTBL_HasKeyBound(int32_t nCommandId, int32_t bIsPrimary)
{
    for (int32_t i = 0; i < 114; ++i)
    {
        if (gKeyBindings[i].nCommandId == nCommandId && gKeyBindings[i].bIsPrimary == bIsPrimary)
            return gKeyBindings[i].wKey != 0xFFFF;
    }
    return 0;
}

// 1.12a: 0x6FB50790 (custom usercall: ecx = nCommandId, eax = pOutErrorStringId, stack: wKey, bIsPrimary)
int32_t __stdcall CMDTBL_BindCommandKey(int32_t nCommandId, uint16_t* pOutErrorStringId, uint16_t wKey, int32_t bIsPrimary)
{
    if (pOutErrorStringId)
        *pOutErrorStringId = 0;
    if (nCommandId >= 56)
        return 0;

    if (!CMDTBL_IsKeyBindable(wKey))
    {
        if (pOutErrorStringId)
            *pOutErrorStringId = 0x0F8B;
        return 0;
    }
    if (!CMDTBL_ValidateMouseBinding(wKey, nCommandId, pOutErrorStringId))
        return 0;

    if (wKey == VK_SNAPSHOT && gCmdDispatchTable[nCommandId].pfnOnDown != NULL)
    {
        if (pOutErrorStringId)
            *pOutErrorStringId = 0x0F8B;
        return 0;
    }

    int32_t nPreviousSlot = 114;
    for (int32_t i = 0; i < 114; ++i)
    {
        if (gKeyBindings[i].wKey == wKey)
        {
            gKeyBindings[i].wKey = 0xFFFF;
            nPreviousSlot = i;
            break;
        }
    }

    for (int32_t i = 0; i < 114; ++i)
    {
        if (gKeyBindings[i].nCommandId == nCommandId && gKeyBindings[i].bIsPrimary == bIsPrimary)
        {
            gKeyBindings[i].wKey = wKey;
            return 1;
        }
    }

    if (nPreviousSlot != 114)
        gKeyBindings[nPreviousSlot].wKey = wKey;
    return 0;
}

// 1.12a: 0x6FB50880 (__stdcall)
void __stdcall CMDTBL_LoadDefaultKeyFile(int32_t bTryLocalDiskFirst)
{
    char szLangCode[8] = { 0 };
    STRTABLE_GetLanguageInfo(szLangCode, 0);

    uint8_t* pFileData = NULL;
    if (bTryLocalDiskFirst)
    {
        char szInstallPath[260] = { 0 };
        FOG_GetInstallPath(szInstallPath, 220);
        strcat(szInstallPath, "default.key");
        HANDLE hFile = FOG_CreateFileA(szInstallPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            uint8_t abBuffer[0x47A];
            DWORD dwBytesRead = 0;
            FOG_ReadFile(hFile, abBuffer, 0x47A, &dwBytesRead, NULL);
            FOG_CloseFile(hFile);
            if (dwBytesRead == 0x47A)
            {
                pFileData = (uint8_t*)FOG_AllocPool(NULL, 0x47A, __FILE__, 0x527, 0);
                memcpy(pFileData, abBuffer, 0x47A);
            }
        }
    }

    if (!pFileData)
    {
        char szMpqPath[260];
        sprintf(szMpqPath, "%s\\CMD\\%s\\default.key", "DATA\\LOCAL", szLangCode);
        extern void* __fastcall ARCHIVE_LoadFileToBuffer(const char* szPath, void* hArchive, DWORD* pdwSize); // 0x6FABDD00
        pFileData = (uint8_t*)ARCHIVE_LoadFileToBuffer(szMpqPath, D2Win_GetArchive(), NULL);
    }

    if (pFileData)
    {
        if (*(uint16_t*)(pFileData + 0) == 0x5357 &&
            *(uint16_t*)(pFileData + 2) == 0x0025 &&
            *(uint16_t*)(pFileData + 4) == 0x047A)
        {
            memcpy(gDefaultKeyBindings, pFileData + 6, sizeof(gDefaultKeyBindings));
            FOG_Free(pFileData, __FILE__, 0x544, 0);
        }
        else
        {
            FOG_Free(pFileData, __FILE__, 0x549, 0);
        }
    }
}

// 1.12a: 0x6FB509D0 (__fastcall)
void __fastcall CMDTBL_ShutdownKeyHandlers(void)
{
    gbCmdTblInputEnabled = 0;
    if (gbCmdTblKeyHandlersRegistered == 1)
    {
        gbCmdTblKeyHandlersRegistered = 0;
        extern void __fastcall INPUT_UnregisterCallbacks(int32_t nCount, HWND hWnd, D2InputCallbackStrc* pTable); // 0x6FB1D0A0
        INPUT_UnregisterCallbacks(4, WINDOW_GetWindow(), gCmdTblKeyHandlers);
    }
}

// 1.12a: 0x6FB50A00 (__fastcall)
void __fastcall CMDTBL_SavePlayerKeyBindings(void)
{
    HANDLE hFile = CMDTBL_OpenPlayerKeyFile(GENERIC_WRITE, CREATE_ALWAYS);
    if (hFile == INVALID_HANDLE_VALUE)
        return;

    #pragma pack(push, 1)
    struct D2KeyFileBufferStrc
    {
        uint16_t wVersion;
        D2KeyBindingStrc aBindings[114];
    } fileBuf;
    #pragma pack(pop)

    memset(&fileBuf, 0, sizeof(fileBuf));
    fileBuf.wVersion = 0x0025;
    memcpy(fileBuf.aBindings, gKeyBindings, sizeof(gKeyBindings));

    DWORD dwBytesWritten = 0;
    FOG_WriteFile(hFile, &fileBuf, sizeof(fileBuf), &dwBytesWritten, NULL);
    FOG_CloseFile(hFile);

    char szInstallPath[260] = { 0 };
    FOG_GetInstallPath(szInstallPath, 220);
    strcat(szInstallPath, "default.key");
    HANDLE hDefaultFile = FOG_CreateFileA(szInstallPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDefaultFile != INVALID_HANDLE_VALUE)
    {
        uint16_t awHeader[3] = { 0x5357, 0x0025, 0x047A };
        FOG_WriteFile(hDefaultFile, awHeader, sizeof(awHeader), &dwBytesWritten, NULL);
        FOG_WriteFile(hDefaultFile, gKeyBindings, sizeof(gKeyBindings), &dwBytesWritten, NULL);
        FOG_CloseFile(hDefaultFile);
    }
}

// 1.12a: 0x6FB50B40 (__fastcall)
void __fastcall CMDTBL_RefreshMouseBindings(void)
{
    gpfnMouseWheelUp    = NULL;
    gpfnMouseWheelDown  = NULL;
    gpfnMouseLButtonDown= NULL;
    gpfnMouseLButtonUp  = NULL;
    gpfnMouseRButtonDown= NULL;
    gpfnMouseRButtonUp  = NULL;
    gpfnMouseMButtonDown= NULL;
    gpfnMouseMButtonUp  = NULL;

    for (int32_t i = 0; i < 114; ++i)
    {
        uint16_t wKey = gKeyBindings[i].wKey;
        int32_t nCmdId = gKeyBindings[i].nCommandId;
        if (wKey <= 0x00DF || nCmdId >= 57)
            continue;

        if (!CMDTBL_ValidateMouseBinding(wKey, nCmdId, NULL))
        {
            gKeyBindings[i].wKey = 0xFFFF;
            continue;
        }

        switch (wKey)
        {
        case 0x0100: // Middle mouse button
            gpfnMouseLButtonDown = gCmdDispatchTable[nCmdId].pfnOnDown;
            gpfnMouseLButtonUp   = gCmdDispatchTable[nCmdId].pfnOnUp;
            break;
        case 0x0101: // Mouse button 3
            gpfnMouseRButtonDown = gCmdDispatchTable[nCmdId].pfnOnDown;
            gpfnMouseRButtonUp   = gCmdDispatchTable[nCmdId].pfnOnUp;
            break;
        case 0x0102: // Mouse button 4
            gpfnMouseMButtonDown = gCmdDispatchTable[nCmdId].pfnOnDown;
            gpfnMouseMButtonUp   = gCmdDispatchTable[nCmdId].pfnOnUp;
            break;
        case 0x0103: // Mouse wheel up
            gpfnMouseWheelUp     = gCmdDispatchTable[nCmdId].pfnOnDown;
            break;
        case 0x0104: // Mouse wheel down
            gpfnMouseWheelDown   = gCmdDispatchTable[nCmdId].pfnOnDown;
            break;
        default:
            break;
        }
    }
}

// 1.12a: 0x6FB50E30 (__fastcall)
void __fastcall CMDTBL_OnVoiceRetreatDown(void)
{
    extern void __stdcall NET_SendC2SPacket(const void* pPacket, int32_t nSize); // 0x6FB291A0 (ebx = nSize, stack = pPacket)
    uint8_t abPacket[3] = { 0x3F, 0x20, 0x00 };
    NET_SendC2SPacket(abPacket, 3);
}

// 1.12a: 0x6FB50E50 (__fastcall)
void __fastcall CMDTBL_OnVoiceDieDown(void)
{
    extern void __stdcall NET_SendC2SPacket(const void* pPacket, int32_t nSize);
    uint8_t abPacket[3] = { 0x3F, 0x1F, 0x00 };
    NET_SendC2SPacket(abPacket, 3);
}

// 1.12a: 0x6FB50E70 (__fastcall)
void __fastcall CMDTBL_OnVoiceByeDown(void)
{
    extern void __stdcall NET_SendC2SPacket(const void* pPacket, int32_t nSize);
    uint8_t abPacket[3] = { 0x3F, 0x1E, 0x00 };
    NET_SendC2SPacket(abPacket, 3);
}

// 1.12a: 0x6FB50E90 (__fastcall)
void __fastcall CMDTBL_OnVoiceSorryDown(void)
{
    extern void __stdcall NET_SendC2SPacket(const void* pPacket, int32_t nSize);
    uint8_t abPacket[3] = { 0x3F, 0x1D, 0x00 };
    NET_SendC2SPacket(abPacket, 3);
}

// 1.12a: 0x6FB50EB0 (__fastcall)
void __fastcall CMDTBL_OnVoiceThanksDown(void)
{
    extern void __stdcall NET_SendC2SPacket(const void* pPacket, int32_t nSize);
    uint8_t abPacket[3] = { 0x3F, 0x1C, 0x00 };
    NET_SendC2SPacket(abPacket, 3);
}

// 1.12a: 0x6FB50ED0 (__fastcall)
void __fastcall CMDTBL_OnVoiceThisIsForYouDown(void)
{
    extern void __stdcall NET_SendC2SPacket(const void* pPacket, int32_t nSize);
    uint8_t abPacket[3] = { 0x3F, 0x1B, 0x00 };
    NET_SendC2SPacket(abPacket, 3);
}

// 1.12a: 0x6FB50EF0 (__fastcall)
void __fastcall CMDTBL_OnVoiceFollowMeDown(void)
{
    extern void __stdcall NET_SendC2SPacket(const void* pPacket, int32_t nSize);
    uint8_t abPacket[3] = { 0x3F, 0x1A, 0x00 };
    NET_SendC2SPacket(abPacket, 3);
}

// 1.12a: 0x6FB50F10 (__fastcall)
void __fastcall CMDTBL_OnVoiceHelpMeDown(void)
{
    extern void __stdcall NET_SendC2SPacket(const void* pPacket, int32_t nSize);
    uint8_t abPacket[3] = { 0x3F, 0x19, 0x00 };
    NET_SendC2SPacket(abPacket, 3);
}

// 1.12a: 0x6FB50F30 (custom usercall: eax = nCommandId, edx = bIsPrimary)
const Unicode* __fastcall CMDTBL_GetShortBindingNameString(int32_t nCommandId, int32_t bIsPrimary)
{
    uint16_t wKey = 0xFFFF;
    for (int32_t i = 0; i < 114; ++i)
    {
        if (gKeyBindings[i].nCommandId == nCommandId && gKeyBindings[i].bIsPrimary == bIsPrimary)
        {
            wKey = gKeyBindings[i].wKey;
            break;
        }
    }

    switch (wKey)
    {
    case 0x0008: return D2LANG_GetStringFromTblIndex(0x0F2C);
    case 0x0010: return D2LANG_GetStringFromTblIndex(0x0F2F);
    case 0x0011: return D2LANG_GetStringFromTblIndex(0x0F30);
    case 0x001B: return D2LANG_GetStringFromTblIndex(0x0F1E);
    case 0x0021: return D2LANG_GetStringFromTblIndex(0x0F34);
    case 0x0022: return D2LANG_GetStringFromTblIndex(0x0F35);
    case 0x002C: return D2LANG_GetStringFromTblIndex(0x0F37);
    case 0x002D: return D2LANG_GetStringFromTblIndex(0x0F38);
    case 0x002E: return D2LANG_GetStringFromTblIndex(0x0F39);
    case 0x0060: return D2LANG_GetStringFromTblIndex(0x0F3B);
    case 0x0061: return D2LANG_GetStringFromTblIndex(0x0F3C);
    case 0x0062: return D2LANG_GetStringFromTblIndex(0x0F3D);
    case 0x0063: return D2LANG_GetStringFromTblIndex(0x0F3E);
    case 0x0064: return D2LANG_GetStringFromTblIndex(0x0F3F);
    case 0x0065: return D2LANG_GetStringFromTblIndex(0x0F40);
    case 0x0066: return D2LANG_GetStringFromTblIndex(0x0F41);
    case 0x0067: return D2LANG_GetStringFromTblIndex(0x0F42);
    case 0x0068: return D2LANG_GetStringFromTblIndex(0x0F43);
    case 0x0069: return D2LANG_GetStringFromTblIndex(0x0F44);
    case 0x006A: return D2LANG_GetStringFromTblIndex(0x0F45);
    case 0x006B: return D2LANG_GetStringFromTblIndex(0x0F46);
    case 0x006C: return D2LANG_GetStringFromTblIndex(0x0F48);
    case 0x006D: return D2LANG_GetStringFromTblIndex(0x0F47);
    case 0x006F: return D2LANG_GetStringFromTblIndex(0x0F49);
    case 0x0090: return D2LANG_GetStringFromTblIndex(0x0F2B);
    case 0x0091: return D2LANG_GetStringFromTblIndex(0x0F4A);
    case 0x0100: return D2LANG_GetStringFromTblIndex(0x0F15);
    case 0x0101: return D2LANG_GetStringFromTblIndex(0x0F16);
    case 0x0102: return D2LANG_GetStringFromTblIndex(0x0F17);
    case 0x0103: return D2LANG_GetStringFromTblIndex(0x0F18);
    case 0x0104: return D2LANG_GetStringFromTblIndex(0x0F19);
    default:
        return CMDTBL_GetKeyNameString(nCommandId, bIsPrimary);
    }
}

// 1.12a: 0x6FB51270 (__fastcall)
void __fastcall CMDTBL_RestoreSavedKeyBindings(void)
{
    memcpy(gKeyBindings, gSavedKeyBindings, sizeof(gKeyBindings));
    CMDTBL_RefreshMouseBindings();
}

// 1.12a: 0x6FB51290 (__fastcall)
void __fastcall CMDTBL_ApplyAndSaveKeyBindings(void)
{
    CMDTBL_SavePlayerKeyBindings();
    CMDTBL_RefreshMouseBindings();
}

// 1.12a: 0x6FB512A0 (__fastcall)
void __fastcall CMDTBL_BackupCurrentKeyBindings(void)
{
    memcpy(gSavedKeyBindings, gKeyBindings, sizeof(gKeyBindings));
    CMDTBL_RefreshMouseBindings();
}

// 1.12a: 0x6FB512C0 (custom usercall: eax = bTryLocalDiskFirst)
void __fastcall CMDTBL_ReloadDefaultKeyFile(int32_t bTryLocalDiskFirst)
{
    CMDTBL_LoadDefaultKeyFile(bTryLocalDiskFirst);
}

// 1.12a: 0x6FB512D0 (__fastcall)
void __fastcall CMDTBL_OnNextSkillDown(void)
{
    if (!gbEscMenuOpen && gpPlayerUnit && *((int32_t*)gpPlayerUnit + 4) != 17)
    {
        extern void __stdcall PLAYER_CycleActiveSkillHotkey(int32_t nDirection); // 0x6FB07C20
        PLAYER_CycleActiveSkillHotkey(1);
    }
}

// 1.12a: 0x6FB512F0 (__fastcall)
void __fastcall CMDTBL_OnPrevSkillDown(void)
{
    if (!gbEscMenuOpen && gpPlayerUnit && *((int32_t*)gpPlayerUnit + 4) != 17)
    {
        extern void __stdcall PLAYER_CycleActiveSkillHotkey(int32_t nDirection); // 0x6FB07C20
        PLAYER_CycleActiveSkillHotkey(-1);
    }
}

// 1.12a: 0x6FB51310 (__fastcall)
void __fastcall CMDTBL_OnClearScreenDown(void)
{
    extern void __fastcall GAME_ClearAllOpenPanels(void); // 0x6FB2BBD0
    GAME_ClearAllOpenPanels();
}

// 1.12a: 0x6FB51320 (custom usercall: eax = nSkillHotkeyIndex)
void __fastcall CMDTBL_TriggerSkillHotkey(int32_t nSkillHotkeyIndex)
{
    if (gbEscMenuOpen || !gpPlayerUnit || *((int32_t*)gpPlayerUnit + 4) == 17)
        return;
    if (nSkillHotkeyIndex > 8 && !gbIsExpansionClient)
        return;
    if (nSkillHotkeyIndex < 0 && nSkillHotkeyIndex > 15)
    {
        FOG_DisplayHalt(__FILE__, 0xFC);
        exit(-1);
    }
    extern void __stdcall PLAYER_AssignSkillHotkeyFromBar(int32_t nHotkeyIndex); // 0x6FB079E0
    extern void __fastcall PLAYER_ActivateSkillHotkey(int32_t nHotkeyIndex);     // 0x6FB07930 (esi = nHotkeyIndex)
    if (gbSkillSelectOpen)
        PLAYER_AssignSkillHotkeyFromBar(nSkillHotkeyIndex);
    else
        PLAYER_ActivateSkillHotkey(nSkillHotkeyIndex);
}

// 1.12a: 0x6FB51390 (__fastcall)
void __fastcall CMDTBL_OnAutomapCenterDown(void)
{
    extern void __fastcall AUTOMAP_CenterOnPlayer(int32_t bResetOffset); // 0x6FAEF1B0
    AUTOMAP_CenterOnPlayer(1);
}

// 1.12a: 0x6FB513A0 (custom usercall: eax = bTryLocalDiskFirst)
void __fastcall CMDTBL_ResetToDefaultKeyBindings(int32_t bTryLocalDiskFirst)
{
    CMDTBL_LoadDefaultKeyFile(bTryLocalDiskFirst);
    for (int32_t i = 0; i < 114; ++i)
        gKeyBindings[i].wKey = 0xFFFF;
    memcpy(gKeyBindings, gDefaultKeyBindings, sizeof(gKeyBindings));
    CMDTBL_RefreshMouseBindings();
}

// 1.12a: 0x6FB513E0 (__fastcall)
void __fastcall CMDTBL_LoadPlayerKeyBindings(void)
{
    HANDLE hFile = CMDTBL_OpenPlayerKeyFile(GENERIC_READ, OPEN_EXISTING);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        #pragma pack(push, 1)
        struct D2KeyFileBufferStrc
        {
            uint16_t wVersion;
            D2KeyBindingStrc aBindings[114];
        } fileBuf;
        #pragma pack(pop)

        memset(&fileBuf, 0, sizeof(fileBuf));
        DWORD dwBytesRead = 0;
        FOG_ReadFile(hFile, &fileBuf, sizeof(fileBuf), &dwBytesRead, NULL);
        FOG_CloseFile(hFile);

        if (dwBytesRead == sizeof(fileBuf) && fileBuf.wVersion == 0x0025)
        {
            int32_t bValidCommands = 1;
            for (int32_t nCmd = 0; nCmd < 57; ++nCmd)
            {
                int32_t bFound = 0;
                for (int32_t j = 0; j < 114; ++j)
                {
                    if (fileBuf.aBindings[j].nCommandId == nCmd)
                    {
                        bFound = 1;
                        break;
                    }
                }
                if (!bFound)
                {
                    bValidCommands = 0;
                    break;
                }
            }

            if (bValidCommands)
            {
                int32_t bNoDuplicates = 1;
                for (int32_t i = 0; i < 114 && bNoDuplicates; ++i)
                {
                    uint16_t wKey = fileBuf.aBindings[i].wKey;
                    if (wKey == 0xFFFF)
                        continue;
                    for (int32_t j = 0; j < 114; ++j)
                    {
                        if (i != j && fileBuf.aBindings[j].wKey == wKey)
                        {
                            bNoDuplicates = 0;
                            break;
                        }
                    }
                }

                if (bNoDuplicates)
                {
                    for (int32_t i = 0; i < 114; ++i)
                        gKeyBindings[i].wKey = 0xFFFF;
                    memcpy(gKeyBindings, fileBuf.aBindings, sizeof(gKeyBindings));
                    CMDTBL_RefreshMouseBindings();
                    return;
                }
            }
        }
    }

    CMDTBL_LoadDefaultKeyFile(1);
    for (int32_t i = 0; i < 114; ++i)
        gKeyBindings[i].wKey = 0xFFFF;
    memcpy(gKeyBindings, gDefaultKeyBindings, sizeof(gKeyBindings));
    CMDTBL_RefreshMouseBindings();
    CMDTBL_SavePlayerKeyBindings();
}

// ============================================================================
// Part 5: CmdTbl.cpp Tail Functions (0x6FB51570..0x6FB5360F in UNIT_PlrSkills.asm)
// ============================================================================

// 1.12a: 0x6FB51570 (__fastcall)
void __fastcall CMDTBL_OnSkill16Down(void) { CMDTBL_TriggerSkillHotkey(15); }
// 1.12a: 0x6FB515B0 (__fastcall)
void __fastcall CMDTBL_OnSkill15Down(void) { CMDTBL_TriggerSkillHotkey(14); }
// 1.12a: 0x6FB515F0 (__fastcall)
void __fastcall CMDTBL_OnSkill14Down(void) { CMDTBL_TriggerSkillHotkey(13); }
// 1.12a: 0x6FB51630 (__fastcall)
void __fastcall CMDTBL_OnSkill13Down(void) { CMDTBL_TriggerSkillHotkey(12); }
// 1.12a: 0x6FB51670 (__fastcall)
void __fastcall CMDTBL_OnSkill12Down(void) { CMDTBL_TriggerSkillHotkey(11); }
// 1.12a: 0x6FB516B0 (__fastcall)
void __fastcall CMDTBL_OnSkill11Down(void) { CMDTBL_TriggerSkillHotkey(10); }
// 1.12a: 0x6FB516F0 (__fastcall)
void __fastcall CMDTBL_OnSkill10Down(void) { CMDTBL_TriggerSkillHotkey(9); }
// 1.12a: 0x6FB51730 (__fastcall)
void __fastcall CMDTBL_OnSkill9Down(void)  { CMDTBL_TriggerSkillHotkey(8); }
// 1.12a: 0x6FB51770 (__fastcall)
void __fastcall CMDTBL_OnSkill8Down(void)  { CMDTBL_TriggerSkillHotkey(7); }
// 1.12a: 0x6FB517B0 (__fastcall)
void __fastcall CMDTBL_OnSkill7Down(void)  { CMDTBL_TriggerSkillHotkey(6); }
// 1.12a: 0x6FB517F0 (__fastcall)
void __fastcall CMDTBL_OnSkill6Down(void)  { CMDTBL_TriggerSkillHotkey(5); }
// 1.12a: 0x6FB51830 (__fastcall)
void __fastcall CMDTBL_OnSkill5Down(void)  { CMDTBL_TriggerSkillHotkey(4); }
// 1.12a: 0x6FB51870 (__fastcall)
void __fastcall CMDTBL_OnSkill4Down(void)  { CMDTBL_TriggerSkillHotkey(3); }
// 1.12a: 0x6FB518B0 (__fastcall)
void __fastcall CMDTBL_OnSkill3Down(void)  { CMDTBL_TriggerSkillHotkey(2); }
// 1.12a: 0x6FB518F0 (__fastcall)
void __fastcall CMDTBL_OnSkill2Down(void)  { CMDTBL_TriggerSkillHotkey(1); }
// 1.12a: 0x6FB51930 (__fastcall)
void __fastcall CMDTBL_OnSkill1Down(void)  { CMDTBL_TriggerSkillHotkey(0); }

// 1.12a: 0x6FB51970 (__fastcall)
void __fastcall CMDTBL_InitKeyBindingsAndHandlers(void)
{
    CMDTBL_LoadPlayerKeyBindings();
    gbCmdTblInputEnabled = 1;
    if (gbCmdTblKeyHandlersRegistered == 0)
    {
        gbCmdTblKeyHandlersRegistered = 1;
        extern void __fastcall INPUT_RegisterCallbacks(int32_t nCount, HWND hWnd, D2InputCallbackStrc* pTable); // 0x6FB1D100
        INPUT_RegisterCallbacks(4, WINDOW_GetWindow(), gCmdTblKeyHandlers);
    }
}

// 1.12a: 0x6FB519B0 (__fastcall)
void __fastcall CMDTBL_OnSwapWeaponsDown(void)
{
    if (!gbTradeOpen && !gbNpcMenuOpen && !gbVendorInteractOpen)
    {
        extern void __fastcall INV_SendSwapWeaponsPacket(void); // 0x6FB5AFF0
        INV_SendSwapWeaponsPacket();
    }
}

// 1.12a: 0x6FB519E0 (__fastcall)
void __fastcall CMDTBL_OnRunHoldUp(void)
{
    gbRunKeyHeld = 0;
    if (gpPlayerUnit && !gbAlwaysRunEnabled && *((int32_t*)gpPlayerUnit) == 0 && *((int32_t*)gpPlayerUnit + 4) == 3)
    {
        extern void __fastcall UNITMODE_SetPlayerMoveMode(int32_t nMode); // 0x6FAF13C0 (edi = nMode)
        extern void __stdcall NET_SendC2SPacket(const void* pPacket, int32_t nSize); // 0x6FB291A0
        UNITMODE_SetPlayerMoveMode(2);
        uint8_t bPacket = 0x54;
        NET_SendC2SPacket(&bPacket, 1);
    }
}

// 1.12a: 0x6FB51A30 (__fastcall)
void __fastcall CMDTBL_OnRunHoldDown(void)
{
    gbRunKeyHeld = 1;
    if (gpPlayerUnit && !gbEscMenuOpen && *((int32_t*)gpPlayerUnit + 4) != 17 &&
        *((int32_t*)gpPlayerUnit) == 0 && *((int32_t*)gpPlayerUnit + 4) == 2)
    {
        if (STATLIST_UnitGetStatValue(gpPlayerUnit, 10, 0) != 0)
        {
            extern void __fastcall UNITMODE_SetPlayerMoveMode(int32_t nMode); // 0x6FAF13C0
            extern void __fastcall MSG_SendSingleBytePacket(uint8_t bOpcode); // 0x6FB29900
            UNITMODE_SetPlayerMoveMode(3);
            MSG_SendSingleBytePacket(0x53);
        }
    }
}

// 1.12a: 0x6FB51A90 (__fastcall)
void __fastcall CMDTBL_OnShowItemsUp(void)
{
    FOG_IsExpansion();
    if (gnPanelCount != 0x26)
    {
        FOG_DisplayHalt(__FILE__, 0x1389);
        exit(-1);
    }
    gbShowGroundItems = 0;
    extern int32_t gnHoveredGroundItemId;   // 0x6FBCC11C
    extern int32_t gnHoveredGroundItemType; // 0x6FBCC118
    gnHoveredGroundItemId = 0;
    gnHoveredGroundItemType = 0;
}

// 1.12a: 0x6FB51AD0 (__fastcall)
void __fastcall CMDTBL_OnHireableScreenDown(void)
{
    extern void __fastcall UI_TogglePanelWithRule(int32_t nPanelIdx, int32_t bState, int32_t nArg); // 0x6FB3EF00
    extern int32_t __fastcall CUNIT_HasCursorItem(void); // 0x6FACEAC0
    extern void __fastcall VIEW_SetViewportDimensions(int32_t nHeight, int32_t nWidth, int32_t nZero, int32_t nShake, int32_t nOffset); // 0x6FB6A990
    if (!gbIsExpansionClient || !gpPlayerUnit)
        return;
    UI_TogglePanelWithRule(36, !gbHireableScreenOpen, 0);
}

// 1.12a: 0x6FB51E20 (__fastcall)
void __fastcall CMDTBL_OnShowItemsDown(void)
{
    FOG_IsExpansion();
    if (gnPanelCount != 0x26)
    {
        FOG_DisplayHalt(__FILE__, 0x1389);
        exit(-1);
    }
    if (!gbGoldDialogOpen)
    {
        extern int32_t __fastcall CUNIT_HasCursorItem(void); // 0x6FACEAC0
        if (!gpPlayerUnit || (*((int32_t*)gpPlayerUnit) == 0 && !CUNIT_HasCursorItem()))
            gbShowGroundItems = 1;
    }
}

// 1.12a: 0x6FB51EF0 (__fastcall)
void __fastcall CMDTBL_OnBeltShowDown(void)
{
    if (!gpPlayerUnit)
        return;
    D2UnitStrc* pBeltItem = INV_GetItemFromBodyLoc(*(D2InventoryStrc**)((uint8_t*)gpPlayerUnit + 0x60), 8);
    if (!pBeltItem || !ITEMS_CheckItemTypeId(pBeltItem, 0x13))
        return;
    extern void __fastcall UI_TogglePanelWithRule(int32_t nPanelIdx, int32_t bState, int32_t nArg); // 0x6FB3EF00
    UI_TogglePanelWithRule(31, !gbBeltShowOpen, 0);
}

// 1.12a: 0x6FB52060 (__fastcall)
void __fastcall CMDTBL_OnSkillSelectDown(void)
{
    extern void __fastcall UI_TogglePanelWithRule(int32_t nPanelIdx, int32_t bState, int32_t nArg); // 0x6FB3EF00
    UI_TogglePanelWithRule(3, !gbSkillSelectOpen, 0);
}

// 1.12a: 0x6FB521A0 (__fastcall)
void __fastcall CMDTBL_OnSkillTreeDown(void)
{
    extern void __fastcall UI_TogglePanelWithRule(int32_t nPanelIdx, int32_t bState, int32_t nArg); // 0x6FB3EF00
    UI_TogglePanelWithRule(4, !gbSkillTreeOpen, 0);
}

// 1.12a: 0x6FB524A0 (__fastcall)
void __fastcall CMDTBL_OnAutomapToggleDown(void)
{
    extern void __fastcall UI_TogglePanelWithRule(int32_t nPanelIdx, int32_t bState, int32_t nArg); // 0x6FB3EF00
    UI_TogglePanelWithRule(10, !gbAutomapActive, 0);
}

// 1.12a: 0x6FB52660 (__fastcall)
void __fastcall CMDTBL_OnHelpScreenDown(void)
{
    if (gbEscMenuOpen || !gpPlayerUnit || *((int32_t*)gpPlayerUnit + 4) == 17)
        return;
    extern void __fastcall UI_TogglePanelWithRule(int32_t nPanelIdx, int32_t bState, int32_t nArg); // 0x6FB3EF00
    UI_TogglePanelWithRule(33, !gbHelpScreenOpen, 0);
}

// 1.12a: 0x6FB52800 (__fastcall)
void __fastcall CMDTBL_OnChatInputDown(void)
{
    if (gbEscMenuOpen)
        return;
    extern void __fastcall UI_TogglePanelWithRule(int32_t nPanelIdx, int32_t bState, int32_t nArg); // 0x6FB3EF00
    UI_TogglePanelWithRule(5, !gbChatBoxOpen, 0);
}

// 1.12a: 0x6FB52AA0 (__fastcall)
void __fastcall CMDTBL_OnMessageLogDown(void)
{
    extern void __fastcall UI_TogglePanelWithRule(int32_t nPanelIdx, int32_t bState, int32_t nArg); // 0x6FB3EF00
    UI_TogglePanelWithRule(24, !gbMessageLogOpen, 0);
}

// 1.12a: 0x6FB52BB0 (__fastcall)
void __fastcall CMDTBL_OnPartyScreenDown(void)
{
    extern int32_t gbMultiplayerGame; // 0x6FBCBFF8
    if (!gbMultiplayerGame)
        return;
    extern void __fastcall UI_TogglePanelWithRule(int32_t nPanelIdx, int32_t bState, int32_t nArg); // 0x6FB3EF00
    UI_TogglePanelWithRule(22, !gbPartyScreenOpen, 0);
}

// 1.12a: 0x6FB52E90 (__fastcall)
void __fastcall CMDTBL_OnInventoryScreenDown(void)
{
    extern void __fastcall UI_TogglePanelWithRule(int32_t nPanelIdx, int32_t bState, int32_t nArg); // 0x6FB3EF00
    UI_TogglePanelWithRule(1, !gbInventoryOpen, 0);
}

// 1.12a: 0x6FB531A0 (__fastcall)
void __fastcall CMDTBL_OnCharacterScreenDown(void)
{
    extern void __fastcall UI_TogglePanelWithRule(int32_t nPanelIdx, int32_t bState, int32_t nArg); // 0x6FB3EF00
    UI_TogglePanelWithRule(2, !gbCharacterScreenOpen, 0);
}

// 1.12a: 0x6FB53470 (__fastcall)
void __fastcall CMDTBL_OnEscMenuDown(void)
{
    extern int32_t __fastcall NPCMENU_IsActive(void); // 0x6FAC7C90
    extern void __stdcall UI_CloseEscOptionsMenu(int32_t bArg); // 0x6FB05560
    extern int32_t __stdcall UI_CloseAllOpenPanels(int32_t bArg); // 0x6FB3B480
    extern void __fastcall UI_OpenEscOptionsMenu(int32_t nType, int32_t nZero); // 0x6FB05610
    if (NPCMENU_IsActive() || gbGoldDialogOpen)
        return;
    if (gbEscOptionsOpen)
    {
        UI_CloseEscOptionsMenu(1);
        return;
    }
    if (!UI_CloseAllOpenPanels(0))
        UI_OpenEscOptionsMenu(1, 0);
}

// 1.12a: 0x6FB534B0 (__fastcall)
void __fastcall CMDTBL_OnQuestLogDown(void)
{
    if (!gbEscMenuOpen && gpPlayerUnit && *((int32_t*)gpPlayerUnit + 4) != 17)
    {
        extern void __stdcall QUESTLOG_TogglePanel(int32_t bPlaySound); // 0x6FB71FD0
        QUESTLOG_TogglePanel(1);
    }
}

// 1.12a: 0x6FB534D0 (__fastcall)
void __fastcall CMDTBL_OnUseBeltSlot4Down(void)
{
    extern uint8_t gabBeltSlotReady[4]; // 0x6FBCBF88..0x6FBCBF8B
    extern void __fastcall PANEL_UseBeltPotionSlot(D2InventoryStrc* pInv, D2UnitStrc* pPlayer, int32_t bShiftHeld, int32_t nSlot); // 0x6FB33DF0
    if (!gbEscMenuOpen && gpPlayerUnit && *((int32_t*)gpPlayerUnit + 4) != 17 && gabBeltSlotReady[3] == 1)
    {
        int32_t bShiftHeld = GetKeyState(VK_SHIFT) & 0x8000;
        PANEL_UseBeltPotionSlot(*(D2InventoryStrc**)((uint8_t*)gpPlayerUnit + 0x60), gpPlayerUnit, bShiftHeld, 3);
    }
}

// 1.12a: 0x6FB53510 (__fastcall)
void __fastcall CMDTBL_OnUseBeltSlot3Down(void)
{
    extern uint8_t gabBeltSlotReady[4];
    extern void __fastcall PANEL_UseBeltPotionSlot(D2InventoryStrc* pInv, D2UnitStrc* pPlayer, int32_t bShiftHeld, int32_t nSlot); // 0x6FB33DF0
    if (!gbEscMenuOpen && gpPlayerUnit && *((int32_t*)gpPlayerUnit + 4) != 17 && gabBeltSlotReady[2] == 1)
    {
        int32_t bShiftHeld = GetKeyState(VK_SHIFT) & 0x8000;
        PANEL_UseBeltPotionSlot(*(D2InventoryStrc**)((uint8_t*)gpPlayerUnit + 0x60), gpPlayerUnit, bShiftHeld, 2);
    }
}

// 1.12a: 0x6FB53550 (__fastcall)
void __fastcall CMDTBL_OnUseBeltSlot2Down(void)
{
    extern uint8_t gabBeltSlotReady[4];
    extern void __fastcall PANEL_UseBeltPotionSlot(D2InventoryStrc* pInv, D2UnitStrc* pPlayer, int32_t bShiftHeld, int32_t nSlot); // 0x6FB33DF0
    if (!gbEscMenuOpen && gpPlayerUnit && *((int32_t*)gpPlayerUnit + 4) != 17 && gabBeltSlotReady[1] == 1)
    {
        int32_t bShiftHeld = GetKeyState(VK_SHIFT) & 0x8000;
        PANEL_UseBeltPotionSlot(*(D2InventoryStrc**)((uint8_t*)gpPlayerUnit + 0x60), gpPlayerUnit, bShiftHeld, 1);
    }
}

// 1.12a: 0x6FB53590 (__fastcall)
void __fastcall CMDTBL_OnUseBeltSlot1Down(void)
{
    extern uint8_t gabBeltSlotReady[4];
    extern void __fastcall PANEL_UseBeltPotionSlot(D2InventoryStrc* pInv, D2UnitStrc* pPlayer, int32_t bShiftHeld, int32_t nSlot); // 0x6FB33DF0
    if (!gbEscMenuOpen && gpPlayerUnit && *((int32_t*)gpPlayerUnit + 4) != 17 && gabBeltSlotReady[0] == 1)
    {
        int32_t bShiftHeld = GetKeyState(VK_SHIFT) & 0x8000;
        PANEL_UseBeltPotionSlot(*(D2InventoryStrc**)((uint8_t*)gpPlayerUnit + 0x60), gpPlayerUnit, bShiftHeld, 0);
    }
}

// 1.12a: 0x6FB535D0 (__fastcall)
const void* __fastcall CMDTBL_GetCommandCategoryTable(void)
{
    return (const void*)0x6FB8D020;
}
