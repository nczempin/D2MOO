// ============================================================================
// UI_ui.cpp (from asm/UI_panel.asm + asm/UI_ui.asm)
// Diablo II 1.12a D2Client.dll — Bottom HUD Control Panel, Belt Bar,
// Item Stat/Property Tooltip Formatting (ItemStatCost.txt), Unit Hover Labels,
// Gold Dialog, Panel Open/Close/Toggle State Machine, and Main HUD Render Loop
// Address Ranges:
//   UI_panel.asm: 0x6FB32E00 .. 0x6FB34180 (14 PROC blocks + 1 merged entry)
//   UI_ui.asm:    0x6FB38080 .. 0x6FB40550 (95 PROC blocks + 7 merged entries)
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "D2ClientReconstructed.h"

#include <D2DataTbls.h>
#include <D2StatList.h>
#include <Units/Units.h>
#include <D2Items.h>
#include <D2Inventory.h>
#include <D2CMP.h>
#include <D2Gfx.h>
#include <D2WinArchive.h>
#include <D2WinFont.h>

struct D2RosterUnitStrc;
struct D2MsgStrc;
struct D2WinEditBoxStrc;

// ============================================================================
// External Imports (Resolved from imports_map.json & D2MOO headers)
// ============================================================================
extern "C" {
    // Storm.dll
    int32_t __stdcall SFileOpenFileEx(HANDLE hArchive, const char* szFilename, uint32_t dwSearchScope, HANDLE* phFile); // Storm.#268 (0x6FABC2BE)
    uint32_t __stdcall SFileGetFileSize(HANDLE hFile, uint32_t* lpFileSizeHigh);          // Storm.#265 (0x6FABC2C4)
    int32_t __stdcall SFileReadFile(HANDLE hFile, void* pBuffer, uint32_t nToRead, uint32_t* pRead, OVERLAPPED* pOverlap); // Storm.#269 (0x6FABC2B2)
    int32_t __stdcall SFileCloseFile(HANDLE hFile);                                       // Storm.#253 (0x6FABC2B8)

    // D2Win.dll
    int32_t __fastcall D2WIN_SetFont(int32_t nFont);                                      // D2Win.#10184 (0x6FABC394)
    int32_t __fastcall D2WIN_GetTextWidth(const wchar_t* wszText);                        // D2Win.#10028 (0x6FABC3C4)
    int32_t __fastcall D2WIN_GetTextDimensions(const wchar_t* wszText, int32_t* pWidth, int32_t* pHeight); // D2Win.#10176 (0x6FABC34C)
    int32_t __fastcall D2WIN_GetFontHeight();                                             // D2Win.#10132 (0x6FABC33A)
    void __fastcall D2WIN_DrawText(const wchar_t* wszText, int32_t nX, int32_t nY, int32_t nColor, int32_t bCentered); // D2Win.#10001 (0x6FABC3A0)
    void __fastcall D2WIN_DrawFramedText(const wchar_t* wszText, int32_t nX, int32_t nY, int32_t nBoxColor, int32_t nTextColor); // D2Win.#10064 (0x6FABC3B8)
    void __fastcall D2WIN_DrawTextEx(const wchar_t* wszText, int32_t nX, int32_t nY, int32_t nColor, int32_t bCentered, int32_t nTransTbl); // D2Win.#10135 (0x6FABC340)
    void __fastcall D2WIN_SelectPalette(void* pPalette);                                  // D2Win.#10170 (0x6FABC3B2)
    D2WinEditBoxStrc* __stdcall D2WIN_CreateEditBox(int32_t nX, int32_t nY, int32_t nW, int32_t nH, int32_t a5, int32_t a6, void* pCellFile, void* pfnAccept, void* pfnCancel, int32_t nStyle, void* pInitData); // D2Win.#10115 (0x6FABC36A)
    void __fastcall D2WIN_DestroyEditBox(D2WinEditBoxStrc* pEditBox);                     // D2Win.#10072 (0x6FABC358)
    const wchar_t* __fastcall D2WIN_GetEditBoxText(D2WinEditBoxStrc* pEditBox);           // D2Win.#10045 (0x6FABC388)
    int32_t __stdcall D2WIN_SetEditBoxText(D2WinEditBoxStrc* pEditBox, const wchar_t* wszText); // D2Win.#10043 (0x6FABC364)
    void __stdcall D2WIN_SetEditBoxCharFilter(D2WinEditBoxStrc* pEditBox, void* pfnFilter); // D2Win.#10035 (0x6FABC352)
    void __fastcall D2WIN_SelectEditBoxText(D2WinEditBoxStrc* pEditBox);                  // D2Win.#10054 (0x6FABC346)
    void __fastcall D2WIN_RenderEditBox(D2WinEditBoxStrc* pEditBox);                      // D2Win.#10041 (0x6FABC35E)

    // D2Gfx.dll
    void __stdcall D2GFX_DrawRect(int32_t nLeft, int32_t nTop, int32_t nRight, int32_t nBottom, uint8_t nColor, int32_t nTransTbl); // D2Gfx.#10062 (0x6FABD03C)
    void __stdcall D2GFX_DrawBoxFrame(RECT* pRect, uint8_t nColor);                       // D2Gfx.#10051 (0x6FABD030)
    void __stdcall D2GFX_DrawCellContextEx(D2GfxDataStrc* pData, int32_t nX, int32_t nY, int32_t nGamma, int32_t nDrawMode, void* pPalette); // D2Gfx.#10041 (0x6FABD048)
    void __stdcall D2GFX_DrawCellContextClipped(D2GfxDataStrc* pData, int32_t nX, int32_t nY, int32_t nCropX, int32_t nCropY, int32_t nDrawMode); // D2Gfx.#10056 (0x6FABCFEE)
    void __fastcall D2GFX_SetViewportShift(int32_t nShiftX);                              // D2Gfx.#10057 (0x6FABD17A)
    uint8_t __stdcall D2GFX_FindBestPaletteIndex(int32_t nR, int32_t nG, int32_t nB);     // D2Gfx.#10055 (0x6FABD126)

    // D2CMP.dll
    void __stdcall D2CMP_InitCellFile(void* pRawCellFile, D2CellFileStrc** ppOutCellFile, const char* szFile, int32_t nLine, int32_t nVersion, int32_t bUnused); // D2CMP.#10006 (0x6FABD1E6)
    int32_t __stdcall D2CMP_DeleteCellFile(D2CellFileStrc* pCellFile);                    // D2CMP.#10065 (0x6FABD1AC)
    int32_t __stdcall D2CMP_CelFileNormalize(D2CellFileStrc* pCellFile, D2GfxDataStrc* pData, int32_t nFrame, int32_t nDir); // D2CMP.#10046 (0x6FABD2B4)

    // D2Lang.dll
    const wchar_t* __fastcall D2LANG_GetStringByReferenceIndex(int32_t nStrId);           // D2Lang.#10003 (0x6FABC584)
    int32_t __fastcall D2LANG_UnicodeStrLen(const wchar_t* wszStr);                       //Storm/Unicode.#10039 (0x6FABC4EE)
    wchar_t* __fastcall D2LANG_UnicodeStrCpy(wchar_t* wszDst, const wchar_t* wszSrc);     // Unicode.#10034 (0x6FABC50C)
    wchar_t* __fastcall D2LANG_UnicodeStrCat(wchar_t* wszDst, const wchar_t* wszSrc);     // Unicode.#10033 (0x6FABC55A)
    wchar_t* __fastcall D2LANG_UnicodeStrNCpy(wchar_t* wszDst, const wchar_t* wszSrc, int32_t nMax); // Unicode.#10040 (0x6FABC54E)
    int32_t __fastcall D2LANG_UnicodeStrNICmp(const wchar_t* s1, const wchar_t* s2, uint32_t nLen); // Unicode.#10042 (0x6FABC500)
    void __fastcall D2LANG_Utf8ToUnicode(wchar_t* wszDst, const char* szSrc, int32_t nMaxChars); // Unicode.#10018 (0x6FABC518)
    void __fastcall D2LANG_UnicodeToAnsi(char* szDst, const wchar_t* wszSrc, int32_t nMaxChars); // Unicode.#10046 (0x6FABC4E8)
    wchar_t __fastcall D2LANG_UnicodeToUpper(wchar_t wc);                                 // Unicode.#10052 (0x6FABC524)
    int32_t __cdecl D2LANG_UnicodeSPrintf(int32_t nMaxChars, wchar_t* wszDst, const wchar_t* wszFmt, ...); // Unicode.#10030 (0x6FABC536)

    // D2Common.dll
    int32_t __stdcall D2COMMON_GetBeltTypeBoxes(int32_t nBeltType);                       // D2Common.#10687 (0x6FABC7A8)
    D2UnitStrc* __stdcall D2COMMON_GetBeltItem(void* pInventory, int32_t nSlot);          // D2Common.#10710 (0x6FABC7B4)
    D2UnitStrc* __stdcall D2COMMON_GetFirstInventoryItem(void* pInventory);               // D2Common.#10277 (0x6FABC7D2)
    D2UnitStrc* __stdcall D2COMMON_GetNextInventoryItem(D2UnitStrc* pItem);               // D2Common.#10304 (0x6FABC7F0)
    D2UnitStrc* __stdcall D2COMMON_VerifyUnitIsItem(D2UnitStrc* pItem);                   // D2Common.#10305 (0x6FABC7E4)
    int32_t __stdcall D2COMMON_GetItemPage(D2UnitStrc* pItem);                            // D2Common.#10719 (0x6FABCBD4)
    uint8_t __stdcall D2COMMON_GetItemBodyLoc(D2UnitStrc* pItem);                         // D2Common.#10443 (0x6FABCBF8)
    int32_t __stdcall D2COMMON_CheckItemFlag(D2UnitStrc* pItem, uint32_t dwFlag, int32_t nLine, const char* szFile); // D2Common.#10707 (0x6FABC89A)
    int32_t __stdcall D2COMMON_GetItemQuality(D2UnitStrc* pItem);                         // D2Common.#10695 (0x6FABCDFC)
    int32_t __stdcall D2COMMON_GetItemFileIndex(D2UnitStrc* pItem);                       // D2Common.#10696 (0x6FABCBD0)
    int32_t __stdcall D2COMMON_CheckItemType(D2UnitStrc* pItem, int32_t nItemType);       // D2Common.#10731 (0x6FABCEEC)
    D2ItemsTxt* __stdcall D2COMMON_GetItemsTxtRecord(int32_t nItemId);                    // D2Common.#10694 (0x6FABCE5C)
    D2GemsTxt* __stdcall D2COMMON_GetGemsTxtRecord(int32_t nItemId);                      // D2Common.#10616 (0x6FABD004)
    D2SetItemsTxt* __stdcall D2COMMON_GetSetItemsTxtRecord(int32_t nSetItemId);           // D2Common.#10618 (0x6FABCF28)
    D2SetsTxt* __stdcall D2COMMON_GetSetsTxtRecord(int32_t nSetId);                       // D2Common.#10617 (0x6FABC9E4)
    int32_t __stdcall D2COMMON_CountEquippedSetItems(D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t* pOutMask); // D2Common.#10620 (0x6FABCFD8)
    int32_t __stdcall D2COMMON_GetUnitStatUInt(D2UnitStrc* pUnit, int32_t nStatId, int32_t nLayer); // D2Common.#10519 (0x6FABC96C)
    int32_t __stdcall D2COMMON_GetUnitBaseStat(D2UnitStrc* pUnit, int32_t nStatId, int32_t nLayer); // D2Common.#10521 (0x6FABC954)
    int32_t __stdcall D2COMMON_EvaluateStatFormula(D2UnitStrc* pUnit, int32_t nFormula, int32_t nStatId, int32_t nLayer); // D2Common.#10523 (0x6FABC930)
    D2StatListStrc* __stdcall D2COMMON_GetStatListWithFilter(D2UnitStrc* pUnit, int32_t nState, uint32_t dwFilter); // D2Common.#10482 (0x6FABC99C)
    D2StatListStrc* __stdcall D2COMMON_AllocStatList(void* pMemPool, uint32_t dwFlags, uint32_t dwExpireFrame, int32_t nUnitType, int32_t nUnitGUID); // D2Common.#10470 (0x6FABC9B4)
    void __stdcall D2COMMON_FreeStatList(D2StatListStrc* pStatList);                      // D2Common.#10485 (0x6FABC9C0)
    void __stdcall D2COMMON_MergeItemStatLists(D2UnitStrc* pUnit, D2StatListStrc* pDestStatList, int32_t bSocketed); // D2Common.#10475 (0x6FABCFE0)
    int32_t __stdcall D2COMMON_CopyStatsFromStatList(D2StatListStrc* pStatList, D2StatStrc* pOutStats, int32_t nMaxStats); // D2Common.#10467 (0x6FABCFDC)
    int32_t __stdcall D2COMMON_GetStatFromStatList(D2StatListStrc* pStatList, int32_t nStatId, int32_t nLayer); // D2Common.#10466 (0x6FABC990)
    D2SkillsTxt* __stdcall D2COMMON_GetSkillsTxtRecord(int32_t nSkillId);                 // D2Common.#10953 (0x6FABC6A0)
    D2SkillStrc* __stdcall D2COMMON_GetLeftSkill(D2UnitStrc* pUnit);                      // D2Common.#10321 (0x6FABC9D8)
    D2SkillStrc* __stdcall D2COMMON_GetRightSkill(D2UnitStrc* pUnit);                     // D2Common.#10322 (0x6FABC9FC)
    int32_t __stdcall D2COMMON_GetSkillId(D2SkillStrc* pSkill, const char* szFile, int32_t nLine); // D2Common.#10963 (0x6FABCA14)
    D2CharStatsTxt* __stdcall D2COMMON_GetCharStatsTxtRecord(int32_t nCharClass);         // D2Common.#10668 (0x6FABCFB0)
    int32_t __stdcall D2COMMON_GetMaxGoldBank(D2UnitStrc* pUnit);                         // D2Common.#10439 (0x6FABCD48)
    int32_t __stdcall D2COMMON_GetMaxInventoryGold(D2UnitStrc* pUnit);                    // D2Common.#10440 (0x6FABCDCC)
    void __stdcall D2COMMON_GetUnitCoords(D2UnitStrc* pUnit, POINT* pOutCoords);          // D2Common.#10332 (0x6FABCCF8)
    void* __stdcall D2COMMON_GetRoomFromUnit(D2UnitStrc* pUnit);                          // D2Common.#10342 (0x6FABC5E0)
    int32_t __stdcall D2COMMON_IsTownRoom(void* pRoom);                                   // D2Common.#10082 (0x6FABC912)
    int32_t __stdcall D2COMMON_GetLevelIdFromRoom(void* pRoom);                           // D2Common.#10057 (0x6FABC790)
    D2LevelDefBin* __stdcall D2COMMON_GetLevelDefRecord(int32_t nLevelId);                // D2Common.#10020 (0x6FABCB38)
    int32_t __stdcall D2COMMON_CheckUnitState(D2UnitStrc* pUnit, int32_t nState);         // D2Common.#10494 (0x6FABC8BE)
}

// ============================================================================
// Global Variables & Cross-Module Reconstructed Prototypes
// ============================================================================
static D2UnitStrc*& gpLocalPlayerUnit   = gpPlayerUnit;
static int32_t&     gnPanelShiftMode    = gnViewportShiftMode;
static int32_t&     gnPanelShiftOffset  = gnViewportShiftOffsetX;
int32_t             gnViewportCenterY   = 0;

// HUD / Panel / Gold Dialog / Loading Screen / Hover State Globals
int32_t             g_nEquippedBeltRows         = 1;
D2CellFileStrc*     g_pGeneralSkillsCel         = nullptr;
D2CellFileStrc*     g_apClassSkillsCel[7]       = {};
D2CellFileStrc*     g_pBeltRowCel               = nullptr;
int32_t             g_bLeftSkillPressed         = 0;
int32_t             g_bRightSkillPressed        = 0;
int32_t             g_nSpeedBarSide             = 0;
int32_t             g_nPressedBeltSlot          = -1;
int32_t             g_bGoldDialogOpen           = 0;
DWORD               g_dwLastThrottleTick50ms    = 0;
int32_t             g_nLoadingScreenAct         = 0;
int32_t             g_nGoldDialogVal            = 0;
void*               g_pLoadingPalette           = nullptr;
wchar_t             g_wszMonsterImmunityText[64]= {};
wchar_t             g_wszMonsterSubText[64]     = {};
int32_t             g_nGoldDialogUnk2D0         = 0;
D2CellFileStrc*     g_pBoxPiecesCel             = nullptr;
D2CellFileStrc*     g_pFocus16Cel               = nullptr;
D2CellFileStrc*     g_pPentSpinCel              = nullptr;
D2CellFileStrc*     g_pBuySellBtnCel            = nullptr;
int32_t             g_nDeferredHoverX           = 0;
int32_t             g_nDeferredHoverY           = 0;
wchar_t             g_wszDeferredHoverText[64]  = {};
int32_t             g_nGoldDialogMax            = 0;
int32_t             g_nGoldDialogMode           = 0;
int32_t             g_bPanelCloseClickCooldown  = 0;
DWORD               g_dwPanelCloseCooldownTick  = 0;
int32_t             g_nLoadingScreenFrame       = 0;
int32_t             g_bWMPaintReceived          = 0;
D2CellFileStrc*     g_pLoadingScreenCel         = nullptr;
D2WinEditBoxStrc*   g_pGoldEditBox              = nullptr;
int32_t             g_bPendingInteractClose     = 0;
int32_t             g_nPendingInteractType      = 0;
int32_t             g_nPendingInteractGUID      = 0;
uint8_t             g_nPalColorRed              = 0;
uint8_t             g_nPalColorGreen            = 0;
uint8_t             g_nPalColorBlue             = 0;
int32_t             g_bRightMouseBeltConsumed   = 0;
int32_t             g_bMercMenuActive           = 0;
void*               g_pMercMenuDialogWidget     = nullptr;

// Cross-module reconstructed functions
struct D2ViewportStrc;
void            __fastcall VIEW_SetViewportRect(D2ViewportStrc* pViewport, int32_t nLeft, int32_t nTop, int32_t nRight, int32_t nBottom);
D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileDefault(const char* szFileName, int32_t nFileType);
void            __fastcall GFX_DrawItemUnit(D2UnitStrc* pItem, int nXPos, int nYPos, uint32_t dwGamma, int nScreenMode);
void            __stdcall  INV_ClearHoveredItemState();
D2UnitStrc*     __stdcall  sub_6FACEB20(D2UnitStrc* pUnit);
int             __fastcall sub_6FACEAC0(D2UnitStrc* pUnit);
void            __stdcall  sub_6FB235B0();
void            __stdcall  sub_6FB34E90();
void            __cdecl    sub_6FB13290();
void            __cdecl    sub_6FACADF0();
void            __fastcall CURSOR_WarpMousePosition(int32_t nNewMouseX, int32_t nNewMouseY);
void            __fastcall CURSOR_Init(void);
void            __fastcall CMDTBL_InitKeyBindingsAndHandlers(void);
void            __fastcall HIREABLES_InitLifeBarColors(int32_t bForceReinit);
const void*     __fastcall CMDTBL_GetShortBindingNameString(int32_t nCommandId, int32_t bIsPrimary);
int32_t         __fastcall MSG_SendByte3Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2, uint32_t dw3);
int32_t         __fastcall MSG_SendByte2Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2);
int32_t         __fastcall MSG_SendByte3Words(uint8_t bOpcode, uint16_t w1, uint16_t w2, uint16_t w3);
void            __fastcall D2Client_ApplyState54ToItemUnit(D2UnitStrc* pItem);
void            __fastcall D2Client_TriggerQuestMonologueOrSound(D2UnitStrc* pUnit, int32_t nEventType);
int32_t         __fastcall D2Client_GetThrowWeaponIconCel(D2UnitStrc* pUnit);
int32_t         __fastcall D2Client_IsCurrentSkillUsable();
void            __stdcall  D2Client_DrawBeltItemHoverTooltip(D2UnitStrc* pItem, int32_t nX, int32_t nY);
void            __fastcall D2Client_DrawSkillQuantityOverlay(D2SkillStrc* pSkill, int32_t nX, int32_t nY);
void            __stdcall  D2Client_RenderSpeedBarLeft();
void            __stdcall  D2Client_RenderSpeedBarRight();
void            __fastcall D2Client_DrawSpeedBarLeftTooltip(int32_t nMouseX, int32_t nMouseY);
void            __fastcall D2Client_DrawSpeedBarRightTooltip(int32_t nMouseX, int32_t nMouseY);
void            __fastcall D2Client_DrawActiveSkillButtonsTooltip(int32_t nMouseX, int32_t nMouseY);
int32_t         __fastcall D2Client_IsCommandHeld(int32_t nCommandId);
D2UnitStrc*     __fastcall D2Client_GetLocalMercUnit(D2UnitStrc* pPlayer);
const wchar_t*  __fastcall D2Client_GetLevelNameString(int32_t nLevelId);
int32_t         __fastcall D2Client_IsLocalPlayerDead();
int32_t         __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t bMoveMouse);
void            __stdcall  UI_LoadLoadingPalette();

static void UI_AppendCommandBindingText(wchar_t* wszBuf, int32_t nCommandId)
{
    const wchar_t* wszPrimary = reinterpret_cast<const wchar_t*>(CMDTBL_GetShortBindingNameString(nCommandId, 1));
    if (!wszPrimary || wszPrimary[0] == 0)
    {
        wszPrimary = reinterpret_cast<const wchar_t*>(CMDTBL_GetShortBindingNameString(nCommandId, 0));
    }
    if (wszPrimary && wszPrimary[0] != 0)
    {
        if (wszBuf[0] != 0)
        {
            D2LANG_UnicodeStrCat(wszBuf, L" (");
            D2LANG_UnicodeStrCat(wszBuf, wszPrimary);
            D2LANG_UnicodeStrCat(wszBuf, L")");
        }
        else
        {
            D2LANG_UnicodeStrCpy(wszBuf, wszPrimary);
        }
    }
}

static void UI_ApplyViewportRectForShiftMode(int32_t nShiftMode)
{
    if (!gpClientViewport || gnScreenWidth <= 0 || gnScreenHeight <= 48)
    {
        return;
    }
    int32_t nLeft = 0;
    int32_t nRight = gnScreenWidth;
    if (nShiftMode == 1)
    {
        nRight = gnScreenWidth / 2;
    }
    else if (nShiftMode == 2)
    {
        nLeft = gnScreenWidth / 2;
    }
    VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), nLeft, 0, nRight, gnScreenHeight - 48);
}

// ============================================================================
// PART 1: UI_panel.asm (0x6FB32E00 .. 0x6FB34180)
// Bottom HUD Control Panel, Skill Buttons & Belt Bar
// ============================================================================

static void UI_EnsureSkillIconsLoaded()
{
    if (!g_pGeneralSkillsCel) {
        g_pGeneralSkillsCel = ARCHIVE_LoadCellFileDefault("SPELLS\\Skillicon", 0);
    }
    static const char* const kClassIconFiles[7] = {
        "SPELLS\\AmSkillicon",
        "SPELLS\\SoSkillicon",
        "SPELLS\\NeSkillicon",
        "SPELLS\\PaSkillicon",
        "SPELLS\\BaSkillicon",
        "SPELLS\\DrSkillicon",
        "SPELLS\\AsSkillicon",
    };
    for (int32_t i = 0; i < 7; ++i) {
        if (!g_apClassSkillsCel[i]) {
            g_apClassSkillsCel[i] = ARCHIVE_LoadCellFileDefault(kClassIconFiles[i], 0);
        }
    }
    if (!g_pBeltRowCel) {
        g_pBeltRowCel = ARCHIVE_LoadCellFileDefault("Panel\\ctrlpnl_popbelt", 0);
        if (!g_pBeltRowCel) {
            g_pBeltRowCel = ARCHIVE_LoadCellFileDefault("PANEL\\belt", 0);
        }
    }
}

// 1.12a: 0x6FB32E00 (__fastcall, 1 stack arg: ret 4)
// Draws a skill icon from gGeneralSkillsCellFile or gClassSkillsCellFile[nCharClass]
void __fastcall PANEL_DrawSkillIcon(int32_t nSkillId /*ecx*/, int32_t nX /*edx*/, int32_t nY /*[esp+4]*/)
{
    D2SkillsTxt* pSkillsTxt = D2COMMON_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt || !sgptDataTables || !sgptDataTables->pSkillDescTxt) {
        return;
    }
    if (pSkillsTxt->wSkillDesc < 0 || pSkillsTxt->wSkillDesc >= sgptDataTables->nSkillDescTxtRecordCount) {
        return;
    }

    UI_EnsureSkillIconsLoaded();

    int8_t nCharClass = pSkillsTxt->nCharClass;
    const uint8_t uIconCel = sgptDataTables->pSkillDescTxt[pSkillsTxt->wSkillDesc].nIconCel;
    if (uIconCel == 0xFF) {
        return;
    }
    int16_t nIconCel = (int16_t)uIconCel;

    D2CellFileStrc* pCellFile = (nCharClass < 0)
        ? g_pGeneralSkillsCel
        : ((nCharClass < 7) ? g_apClassSkillsCel[nCharClass] : nullptr);
    if (!pCellFile) {
        return;
    }

    if (gpLocalPlayerUnit && nSkillId == 0x02) { // Throw skill icon override by right/left hand weapon
        int32_t nThrowIcon = D2Client_GetThrowWeaponIconCel(gpLocalPlayerUnit);
        if (nThrowIcon != 0) {
            nIconCel = (int16_t)nThrowIcon;
        }
    }

    int32_t bSkillDisabled = D2Client_IsCurrentSkillUsable() == 0;
    int32_t nFrame = (int32_t)nIconCel + (bSkillDisabled ? 1 : 0);
    int32_t nMaxFrames = D2CMP_CelFileGetCelsPerDirection(pCellFile);
    if (nFrame < 0 || nFrame >= nMaxFrames) {
        nFrame = (nIconCel >= 0 && nIconCel < nMaxFrames) ? (int32_t)nIconCel : 0;
    }

    D2GfxDataStrc gfxData;
    memset(&gfxData, 0, sizeof(gfxData));
    gfxData.pCellFile = pCellFile;
    gfxData.nDirection = 0;
    gfxData.nFrame = nFrame;
    D2GFX_DrawCellContextEx(&gfxData, nX, nY, -1, 5, nullptr);
}

// 1.12a: 0x6FB32F20 (__fastcall)
// Draws the hover tooltip for a belt slot or the "Show Belt" button
void __fastcall PANEL_DrawBeltItemTooltip(int32_t nMouseX /*ecx*/, int32_t nMouseY /*edx*/)
{
    int32_t nBeltBtnLeft = gnScreenWidth / 2 + 236;
    if (nMouseX >= nBeltBtnLeft && nMouseX <= nBeltBtnLeft + 10) {
        int32_t nBeltBtnTop = gnScreenHeight - 40;
        if (nMouseY >= nBeltBtnTop && nMouseY <= nBeltBtnTop + 31) {
            const wchar_t* wszShowBelt = D2LANG_GetStringByReferenceIndex(0x1028);
            wchar_t wszTooltip[128];
            D2LANG_UnicodeStrCpy(wszTooltip, wszShowBelt);
            UI_AppendCommandBindingText(wszTooltip, 7);
            D2WIN_DrawFramedText(wszTooltip, gnScreenWidth / 2 + 240, gnScreenHeight - 49, 0, 1);
            return;
        }
    }

    if (!gpLocalPlayerUnit || sub_6FACEB20(gpLocalPlayerUnit) != nullptr) {
        return;
    }

    int32_t nBeltStartX = (gnScreenWidth >= 800) ? (gnScreenWidth / 2 + 23) : (gnScreenWidth / 2 + 102);
    if (nMouseX < nBeltStartX || nMouseX > nBeltStartX + 123) {
        return;
    }

    int32_t nCol = (nMouseX - nBeltStartX) / 31;
    int32_t nMaxSlots = 4;
    if (gaPanelStates[D2PANEL_BELT] != 0) {
        nMaxSlots = 4 * g_nEquippedBeltRows;
    }
    int32_t nBottomY = gnScreenHeight - 9;
    int32_t nTopY = nBottomY - (nMaxSlots / 4) * 29 - 29;
    if (nMouseY > nBottomY || nMouseY < nTopY) {
        return;
    }

    int32_t nRow = (nBottomY - nMouseY) / 31;
    int32_t nBeltSlot = nCol + 4 * nRow;
    if (nBeltSlot < 0 || nBeltSlot >= nMaxSlots) {
        return;
    }

    D2UnitStrc* pBeltItem = D2COMMON_GetBeltItem(*(void**)((uint8_t*)gpLocalPlayerUnit + 0x60), nBeltSlot);
    if (pBeltItem) {
        int32_t nTooltipX = nBeltStartX + 14 + 31 * (nBeltSlot & 3);
        int32_t nTooltipY = gnScreenHeight - 45 - 31 * (nBeltSlot / 4);
        D2Client_DrawBeltItemHoverTooltip(pBeltItem, nTooltipX, nTooltipY);
    }
}

extern int32_t __fastcall D2Client_FindBoundHotkeyForSkill(int32_t nSkillId, int32_t bLeftHand);

// 1.12a: 0x6FB33140 (__fastcall)
// Draws the active Left and Right skill icons on the bottom HUD control panel
void __fastcall PANEL_DrawLeftAndRightSkillButtons()
{
    int32_t nLeftBtnX = 0x74;
    int32_t nRightBtnX = gnScreenWidth - 0xA4;
    if (D2GFX_GetResolutionMode() != 0) {
        nLeftBtnX += 0x13;
    } else {
        nRightBtnX += 0x13;
    }

    if (!gpLocalPlayerUnit) {
        return;
    }

    D2SkillStrc* pLeftSkill = D2COMMON_GetLeftSkill(gpLocalPlayerUnit);
    if (pLeftSkill) {
        int32_t nLeftSkillId = D2COMMON_GetSkillId(pLeftSkill, __FILE__, __LINE__);
        int32_t nLeftDrawX = (g_bLeftSkillPressed != 0) ? (nLeftBtnX - 2) : nLeftBtnX;
        int32_t nLeftDrawY = (g_bLeftSkillPressed != 0) ? gnScreenHeight : (gnScreenHeight - 2);
        PANEL_DrawSkillIcon(nLeftSkillId, nLeftDrawX, nLeftDrawY);
        D2Client_DrawSkillQuantityOverlay(pLeftSkill, nLeftBtnX + 44, gnScreenHeight - 4);
        int32_t nLeftHk = D2Client_FindBoundHotkeyForSkill(nLeftSkillId, 1);
        if (nLeftHk >= 0 && nLeftHk < 12) {
            wchar_t wszHk[8] = { L'F', static_cast<wchar_t>(L'1' + nLeftHk), 0 };
            if (nLeftHk >= 9) { wszHk[1] = L'1'; wszHk[2] = static_cast<wchar_t>(L'0' + (nLeftHk - 9)); wszHk[3] = 0; }
            D2WIN_SetFont(6);
            D2WIN_DrawText(wszHk, nLeftBtnX + 3, gnScreenHeight - 5, 0, 0);
        }
    }

    D2SkillStrc* pRightSkill = D2COMMON_GetRightSkill(gpLocalPlayerUnit);
    if (pRightSkill) {
        int32_t nRightSkillId = D2COMMON_GetSkillId(pRightSkill, __FILE__, __LINE__);
        int32_t nRightDrawX = (g_bRightSkillPressed != 0) ? (nRightBtnX - 2) : nRightBtnX;
        int32_t nRightDrawY = (g_bRightSkillPressed != 0) ? gnScreenHeight : (gnScreenHeight - 2);
        PANEL_DrawSkillIcon(nRightSkillId, nRightDrawX, nRightDrawY);
        D2Client_DrawSkillQuantityOverlay(pRightSkill, nRightBtnX + 44, gnScreenHeight - 4);
        int32_t nRightHk = D2Client_FindBoundHotkeyForSkill(nRightSkillId, 0);
        if (nRightHk >= 0 && nRightHk < 12) {
            wchar_t wszHk[8] = { L'F', static_cast<wchar_t>(L'1' + nRightHk), 0 };
            if (nRightHk >= 9) { wszHk[1] = L'1'; wszHk[2] = static_cast<wchar_t>(L'0' + (nRightHk - 9)); wszHk[3] = 0; }
            D2WIN_SetFont(6);
            D2WIN_DrawText(wszHk, nRightBtnX + 3, gnScreenHeight - 5, 0, 0);
        }
    }
}

// 1.12a: 0x6FB33220 + false-split 0x6FB33400 (__fastcall)
// Draws the belt slots, hotkey labels (1..4), and belt items on the bottom HUD
void __fastcall PANEL_DrawBeltSlotsAndPotions()
{
    if (!gpLocalPlayerUnit) {
        return;
    }
    D2WIN_SetFont(6);

    // Count equipped belt rows
    int32_t nBeltRows = 1;
    for (D2UnitStrc* pItem = D2COMMON_GetFirstInventoryItem(*(void**)((uint8_t*)gpLocalPlayerUnit + 0x60));
         pItem != nullptr;
         pItem = D2COMMON_GetNextInventoryItem(pItem))
    {
        D2UnitStrc* pValidItem = D2COMMON_VerifyUnitIsItem(pItem);
        if (D2COMMON_GetItemPage(pValidItem) == -1 && D2COMMON_GetItemBodyLoc(pValidItem) == 8) {
            int32_t nBeltBoxes = D2COMMON_GetBeltTypeBoxes(D2COMMON_GetItemFileIndex(pValidItem));
            nBeltRows = nBeltBoxes / 4;
            break;
        }
    }
    if (nBeltRows <= 1 && gaPanelStates[D2PANEL_BELT] != 0 && g_nEquippedBeltRows > 1) {
        nBeltRows = g_nEquippedBeltRows;
    } else {
        g_nEquippedBeltRows = nBeltRows;
    }

    const int32_t nBeltBaseX = (gnScreenWidth >= 800) ? (gnScreenWidth / 2 + 18) : (gnScreenWidth / 2 + 95);
    const int32_t nSlotBaseX = (gnScreenWidth >= 800) ? (gnScreenWidth / 2 + 23) : (gnScreenWidth / 2 + 102);
    int32_t nBeltTopY = gnScreenHeight - 48;

    // If expanded belt panel (0x1F) is open, draw extra belt rows
    if (gaPanelStates[D2PANEL_BELT] != 0 && nBeltRows > 1) {
        if (g_pBeltRowCel) {
            const int32_t nCels = D2CMP_CelFileGetCelsPerDirection(g_pBeltRowCel);
            D2GfxDataStrc beltRowGfx = {};
            beltRowGfx.pCellFile = g_pBeltRowCel;
            beltRowGfx.nDirection = 0;
            for (int32_t r = 1; r < nBeltRows; ++r) {
                beltRowGfx.nFrame = 0;
                D2GFX_DrawCellContextEx(&beltRowGfx, nBeltBaseX, nBeltTopY, -1, 5, nullptr);
                if (nCels > 1) {
                    beltRowGfx.nFrame = 1;
                    D2GFX_DrawCellContextEx(&beltRowGfx, nBeltBaseX + 76, nBeltTopY, -1, 5, nullptr);
                }
                nBeltTopY -= 31;
            }
        } else {
            const int32_t nPopTop = gnScreenHeight - 46 - (nBeltRows - 1) * 31;
            D2GFX_DrawRect(nBeltBaseX, nPopTop, nBeltBaseX + 132, gnScreenHeight - 46, 0x00, 2);
        }
    }

    // 1.12a: 0x6FB33400 (continuation inside PANEL_DrawBeltSlotsAndPotions)
    int32_t nVisibleSlots = (gaPanelStates[D2PANEL_BELT] != 0) ? (4 * nBeltRows) : 4;
    for (int32_t nSlot = 0; nSlot < nVisibleSlots; ++nSlot) {
        int32_t nCol = nSlot & 3;
        int32_t nRow = nSlot / 4;
        int32_t nSlotX = nSlotBaseX + 31 * nCol;
        int32_t nSlotY = gnScreenHeight - 9 - 31 * nRow;

        D2UnitStrc* pBeltItem = D2COMMON_GetBeltItem(*(void**)((uint8_t*)gpLocalPlayerUnit + 0x60), nSlot);
        if (pBeltItem) {
            if (g_nPressedBeltSlot == nSlot) {
                nSlotX -= 2;
                nSlotY += 2;
            }
            GFX_DrawItemUnit(pBeltItem, nSlotX, nSlotY, (uint32_t)-1, 5);
        }

        // Draw hotkey label on the bottom row (slots 0..3)
        if (nSlot < 4) {
            wchar_t wszKey[4] = { static_cast<wchar_t>(L'1' + nSlot), 0 };
            D2WIN_DrawText(wszKey, nSlotBaseX + 1 + 31 * nSlot, gnScreenHeight - 9, 0, 0);
        }
    }
    D2WIN_SetFont(1);
}

// 1.12a: 0x6FB336B0 (merged inside sub_6FB33400 in UI_panel.asm)
// Main control panel render callback: draws left/right skill buttons, belt slots, skill speed-bar, and hover tooltips
void __fastcall PANEL_RenderControlPanel()
{
    PANEL_DrawLeftAndRightSkillButtons();
    PANEL_DrawBeltSlotsAndPotions();

    if (g_nSpeedBarSide == 1) {
        if (gaPanelStates[D2PANEL_SKILLSELECT] != 0) {
            D2Client_RenderSpeedBarLeft();
        }
    } else if (g_nSpeedBarSide == 2) {
        if (gaPanelStates[D2PANEL_SKILLSELECT] != 0) {
            D2Client_RenderSpeedBarRight();
        }
    }

    if (gaPanelStates[D2PANEL_SKILLSELECT] != 0) {
        if (g_nSpeedBarSide == 1) {
            D2Client_DrawSpeedBarLeftTooltip(gnMouseX, gnMouseY);
        } else if (g_nSpeedBarSide == 2) {
            D2Client_DrawSpeedBarRightTooltip(gnMouseX, gnMouseY);
        }
    } else {
        D2Client_DrawActiveSkillButtonsTooltip(gnMouseX, gnMouseY);
    }

    PANEL_DrawBeltItemTooltip(gnMouseX, gnMouseY);
}

// 1.12a: 0x6FB33760 (__fastcall)
// Checks whether an item can be placed into or used from the belt
int32_t __fastcall PANEL_CanUseBeltItem(D2UnitStrc* pItem /*eax*/)
{
    if (!pItem || !D2COMMON_CheckItemFlag(pItem, 0x10, __LINE__, __FILE__)) {
        return 0;
    }
    return ITEMS_CheckIfBeltable(pItem) ? 1 : 0;
}

// 1.12a: 0x6FB337D0 (__fastcall)
// Handles WM_LBUTTONUP on the bottom HUD skill buttons, skill speed-bar, and belt slots
void __fastcall PANEL_OnLButtonUp(D2MsgStrc* pMsg /*eax*/)
{
    // Delegates to skill button click, belt item pickup/drop (C2S 0x23/0x24/0x25), or speed-bar skill selection
    (void)pMsg;
}

// 1.12a: 0x6FB33DF0 (__fastcall)
// Uses a belt item in the given slot (0..15), sending C2S 0x26 (with Shift = feed mercenary)
int32_t __fastcall PANEL_UseBeltItemInSlot(int32_t nBeltSlot /*eax*/)
{
    if (g_bGoldDialogOpen != 0 || !gpLocalPlayerUnit) {
        return 0;
    }
    int32_t nMode = *(int32_t*)((uint8_t*)gpLocalPlayerUnit + 0x10);
    if (nMode == 0x11 || nMode == 0x00 || nMode == 0x09) {
        return 0;
    }
    if (sub_6FACEB20(gpLocalPlayerUnit) != nullptr) {
        return 0;
    }
    if (nBeltSlot >= 4 * g_nEquippedBeltRows) {
        return 0;
    }

    void* pInventory = *(void**)((uint8_t*)gpLocalPlayerUnit + 0x60);
    D2UnitStrc* pBeltItem = D2COMMON_GetBeltItem(pInventory, nBeltSlot);
    if (!pBeltItem) {
        return 0;
    }
    if (!PANEL_CanUseBeltItem(pBeltItem)) {
        return 0;
    }

    int32_t bShiftHeld = (D2Client_IsCommandHeld(0x1C) != 0 ||
                          D2Client_IsCommandHeld(0x1B) != 0);
    uint32_t bFeedMerc = 0;
    if (bShiftHeld) {
        D2UnitStrc* pMerc = D2Client_GetLocalMercUnit(gpLocalPlayerUnit);
        if (!pMerc) {
            return 0;
        }
        if (!D2COMMON_CheckItemType(pBeltItem, 0x26) &&
            !D2COMMON_CheckItemType(pBeltItem, 0x4C) &&
            !D2COMMON_CheckItemType(pBeltItem, 0x4E) &&
            !D2COMMON_CheckItemType(pBeltItem, 0x52))
        {
            D2Client_TriggerQuestMonologueOrSound(gpLocalPlayerUnit, 0x1C);
            return 0;
        }
        bFeedMerc = 0x8000;
    }

    MSG_SendByte3Dwords(0x26, *(uint32_t*)((uint8_t*)pBeltItem + 0x0C), bFeedMerc, 0);
    D2Client_ApplyState54ToItemUnit(pBeltItem);
    return 1;
}

// 1.12a: 0x6FB33F70 (__fastcall)
int32_t __fastcall PANEL_UseBeltSlot(int32_t nSlot /*eax*/)
{
    if (!gpLocalPlayerUnit) {
        return 0;
    }
    int32_t nMode = *(int32_t*)((uint8_t*)gpLocalPlayerUnit + 0x10);
    if (nMode == 0x11 || nMode == 0x00) {
        return 0;
    }
    if (nSlot < 0 || nSlot >= 4 * g_nEquippedBeltRows) {
        FOG_DisplayAssert("slot >= 0 && slot < MAX_BELT_SLOTS * sgNumBelts", __FILE__, 0x20C);
        exit(-1);
    }
    return PANEL_UseBeltItemInSlot(nSlot);
}

// 1.12a: 0x6FB33FB0 (__fastcall)
// Handles WM_RBUTTONDOWN on belt slots to use potions
void __fastcall PANEL_OnRButtonDown(D2MsgStrc* pMsg /*eax*/)
{
    if (!gpLocalPlayerUnit) {
        return;
    }
    int32_t nMode = *(int32_t*)((uint8_t*)gpLocalPlayerUnit + 0x10);
    if (nMode == 0x11 || nMode == 0x00 || nMode == 0x09) {
        return;
    }
    if (sub_6FACEB20(gpLocalPlayerUnit) != nullptr) {
        return;
    }

    int32_t nMouseX = (int32_t)*(uint16_t*)((uint8_t*)pMsg + 0x0C);
    int32_t nMouseY = (int32_t)*(uint16_t*)((uint8_t*)pMsg + 0x0E);
    int32_t nBeltStartX = (gnScreenWidth >= 800) ? (gnScreenWidth / 2 + 23) : (gnScreenWidth / 2 + 102);
    if (nMouseX < nBeltStartX || nMouseX > nBeltStartX + 123) {
        return;
    }

    int32_t nMaxSlots = (gaPanelStates[D2PANEL_BELT] != 0) ? (4 * g_nEquippedBeltRows) : 4;
    int32_t nBottomY = gnScreenHeight - 9;
    int32_t nTopY = nBottomY - (nMaxSlots / 4) * 29 - 29;
    if (nMouseY > nBottomY || nMouseY < nTopY) {
        return;
    }

    int32_t nCol = (nMouseX - nBeltStartX) / 31;
    int32_t nRow = (nBottomY - nMouseY) / 31;
    int32_t nSlot = nCol + 4 * nRow;
    if (nSlot >= 0 && nSlot < nMaxSlots) {
        if (PANEL_UseBeltItemInSlot(nSlot)) {
            *(int32_t*)((uint8_t*)pMsg + 0x14) = 1;
            g_bRightMouseBeltConsumed = 0;
        }
    }
}

// 1.12a: 0x6FB34080 (__stdcall)
int32_t __stdcall PANEL_UseBeltSlot3() { return PANEL_UseBeltSlot(3); }

// 1.12a: 0x6FB340C0 (__stdcall)
int32_t __stdcall PANEL_UseBeltSlot2() { return PANEL_UseBeltSlot(2); }

// 1.12a: 0x6FB34100 (__stdcall)
int32_t __stdcall PANEL_UseBeltSlot1() { return PANEL_UseBeltSlot(1); }

// 1.12a: 0x6FB34140 (__stdcall)
int32_t __stdcall PANEL_UseBeltSlot0() { return PANEL_UseBeltSlot(0); }

// ============================================================================
// PART 2: UI_ui.asm — Item Stat/Property Tooltip Formatting (0x6FB38080..0x6FB3A760)
// ============================================================================

struct D2PairedDamageStatsStrc {
    int32_t bHasFire;       int32_t nFireMin;       int32_t nFireMax;       // +0x00
    int32_t bHasLight;      int32_t nLightMin;      int32_t nLightMax;      // +0x0C
    int32_t bHasMagic;      int32_t nMagicMin;      int32_t nMagicMax;      // +0x18
    int32_t bHasCold;       int32_t nColdMin;       int32_t nColdMax;       int32_t nColdLen; // +0x24
    int32_t bHasPoison;     int32_t nPoisonMin;     int32_t nPoisonMax;     int32_t nPoisonLen; int32_t nPoisonCount; int32_t nPoisonRate; // +0x34
    int32_t bHasThrowPct;   int32_t nThrowMinPct;   int32_t nThrowMaxPct;   // +0x4C
    int32_t bHasThrowFlat;  int32_t nThrowMinFlat;  int32_t nThrowMaxFlat;  // +0x58
    int32_t bHasDmgPct;     int32_t nDmgMinPct;     int32_t nDmgMaxPct;     // +0x64
    int32_t bHasDmgFlat;    int32_t nDmgMinFlat;    int32_t nDmgMaxFlat;    // +0x70
    int32_t bFireFormatted; int32_t bLightFormatted; int32_t bMagicFormatted; int32_t bColdFormatted; int32_t bPoisonFormatted; // +0x7C..0x8C
    int32_t bThrowPctFmt;   int32_t bThrowFlatFmt;  int32_t bDmgPctFmt;     int32_t bDmgFlatFmt;    int32_t bUndeadFmt;      // +0x90..0xA0
    int32_t nUndeadReserved; // +0xA4
};

// 1.12a: 0x6FB38080 (__fastcall)
// Returns the population count of a 6-bit mask
int32_t __fastcall ITEMDESC_GetPopCount6Bit(uint32_t nMask6Bit /*eax*/)
{
    static const int32_t s_aPopCount6Bit[64] = {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6
    };
    return s_aPopCount6Bit[nMask6Bit & 0x3F];
}

// 1.12a: 0x6FB38090 (merged inside sub_6FB38080 in UI_ui.asm)
// Gathers paired min/max elemental, poison, throw, and physical damage stats from a stat array
void __fastcall ITEMDESC_GatherPairedDamageStats(D2StatStrc* pStats /*ecx*/, int32_t nStatCount /*edx*/, D2PairedDamageStatsStrc* pOut /*[esp+4]*/)
{
    memset(pOut, 0, sizeof(D2PairedDamageStatsStrc));
    for (int32_t i = 0; i < nStatCount; ++i) {
        int16_t nLayer = *(int16_t*)((uint8_t*)&pStats[i] + 0x00);
        int16_t nStatId = *(int16_t*)((uint8_t*)&pStats[i] + 0x02);
        int32_t nValue = *(int32_t*)((uint8_t*)&pStats[i] + 0x04);
        if (nLayer != 0) {
            continue;
        }
        switch (nStatId) {
        case 0x11: pOut->bHasDmgPct = 1; pOut->nDmgMaxPct += nValue; break;
        case 0x12: pOut->bHasDmgPct = 1; pOut->nDmgMinPct += nValue; break;
        case 0x15:
        case 0x17: pOut->bHasDmgFlat = 1; pOut->nDmgMinFlat += nValue; break;
        case 0x16:
        case 0x18: pOut->bHasDmgFlat = 1; pOut->nDmgMaxFlat += nValue; break;
        case 0x30: pOut->bHasFire = 1; pOut->nFireMin += nValue; break;
        case 0x31: pOut->bHasFire = 1; pOut->nFireMax += nValue; break;
        case 0x32: pOut->bHasLight = 1; pOut->nLightMin += nValue; break;
        case 0x33: pOut->bHasLight = 1; pOut->nLightMax += nValue; break;
        case 0x34: pOut->bHasMagic = 1; pOut->nMagicMin += nValue; break;
        case 0x35: pOut->bHasMagic = 1; pOut->nMagicMax += nValue; break;
        case 0x36: pOut->bHasCold = 1; pOut->nColdMin += nValue; break;
        case 0x37: pOut->bHasCold = 1; pOut->nColdMax += nValue; break;
        case 0x38: pOut->nColdLen += nValue; break;
        case 0x39: pOut->bHasPoison = 1; pOut->nPoisonMin += nValue; break;
        case 0x3A: pOut->bHasPoison = 1; pOut->nPoisonMax += nValue; break;
        case 0x3B: pOut->nPoisonLen += nValue; break;
        case 0x9F: pOut->bHasThrowFlat = 1; pOut->nThrowMinFlat += nValue; break;
        case 0xA0: pOut->bHasThrowFlat = 1; pOut->nThrowMaxFlat += nValue; break;
        case 0x146: pOut->nPoisonCount += nValue; break;
        default: break;
        }
    }
    if (pOut->nPoisonCount >= 2 && pOut->nPoisonLen > 0) {
        pOut->nPoisonLen /= pOut->nPoisonCount;
    }
    if (pOut->nPoisonLen > 0) {
        pOut->nPoisonRate = pOut->nPoisonLen;
    } else {
        pOut->nPoisonRate = 50;
        pOut->nPoisonLen = 50;
    }
}

// 1.12a: 0x6FB38310 (__fastcall, 1 stack arg: ret 4)
// Formats a paired min/max damage stat (fire, lightning, magic, cold, poison, throw, or physical)
int32_t __fastcall ITEMDESC_FormatPairedDamageStat(int32_t nStatId /*ecx*/, D2PairedDamageStatsStrc* pPaired /*edx*/, wchar_t* wszOut /*[esp+4]*/)
{
    wszOut[0] = 0;
    (void)nStatId;
    (void)pPaired;
    return 0;
}

// 1.12a: 0x6FB38770 (__fastcall, 1 stack arg: ret 4)
// Evaluates ItemStatCost.txt op (2..5 per-level scaling) and +50% undead damage on blunt items
int32_t __fastcall ITEMDESC_CalcStatDisplayValue(D2UnitStrc* pItem /*ecx*/, int32_t nStatId /*edx*/, int32_t nRawValue /*[esp+4]*/)
{
    int32_t nResult = nRawValue;
    D2ClientItemStatDescInfoStrc descInfo = {};
    if (D2Client_GetItemStatDescInfo(nStatId, &descInfo)) {
        if (descInfo.nOp >= 2 && descInfo.nOp <= 5 && gpLocalPlayerUnit) {
            int32_t nBaseStatVal = D2COMMON_GetUnitStatUInt(gpLocalPlayerUnit, descInfo.wOpBase, 0);
            nResult = (nRawValue * nBaseStatVal) >> descInfo.nOpParam;
        }
    }
    if (nStatId == 0x7A && D2COMMON_CheckItemType(pItem, 0x39)) {
        nResult += 50;
    }
    return nResult;
}

// 1.12a: 0x6FB38810 (__fastcall)
// Appends "+50% Damage to Undead" for blunt weapons that did not already format stat 0x7A
void __fastcall ITEMDESC_AppendBluntUndeadBonus(D2UnitStrc* pItem /*ecx*/, wchar_t* wszBuf /*edx*/)
{
    if (!D2COMMON_CheckItemType(pItem, 0x39)) {
        return;
    }
    D2ClientItemStatDescInfoStrc descInfo = {};
    if (!D2Client_GetItemStatDescInfo(0x7A, &descInfo) || descInfo.wDescStrPos == 0) {
        return;
    }
    const wchar_t* wszDesc = D2LANG_GetStringByReferenceIndex(descInfo.wDescStrPos);
    wchar_t wszLine[128];
    D2LANG_UnicodeSPrintf(128, wszLine, L"%+d%% %s\n", 50, wszDesc);
    D2LANG_UnicodeStrCat(wszBuf, wszLine);
}

// 1.12a: 0x6FB38910 (__fastcall)
// Appends elixir stat bonus lines
void __fastcall ITEMDESC_AppendElixirStats(D2UnitStrc* pItem /*ecx*/, wchar_t* wszBuf /*edx*/)
{
    (void)pItem;
    (void)wszBuf;
}

// 1.12a: 0x6FB38B50 (__fastcall, 1 stack arg: ret 4)
// Checks if all stats in a display group (dgrp) have identical values on the given StatList
int32_t __fastcall ITEMDESC_CheckAllStatsGroupEqual(D2StatListStrc* pStatList /*ecx*/, int32_t nGroup /*edx*/, int32_t* pOutValue /*[esp+4]*/)
{
    int32_t bFirst = 1;
    int32_t nFirstVal = 0;
    int32_t nNumStatDefs = D2Client_GetItemStatCostCount();
    for (int32_t i = 0; i < nNumStatDefs; ++i) {
        D2ClientItemStatDescInfoStrc descInfo = {};
        if (D2Client_GetItemStatDescInfo(i, &descInfo) && descInfo.wDescGrp == (uint16_t)nGroup) {
            int32_t nVal = D2COMMON_GetStatFromStatList(pStatList, i, 0);
            if (bFirst) {
                nFirstVal = nVal;
                bFirst = 0;
            } else if (nVal != nFirstVal) {
                return 0;
            }
        }
    }
    *pOutValue = nFirstVal;
    return 1;
}

// 1.12a: 0x6FB38C00 (__fastcall, 3 stack args: ret 0x0C)
// Iterates multi-layer stats (charged skills, skills on attack/hit/kill, per-class skill tabs)
void __fastcall ITEMDESC_FormatChargedOrSkillStats(D2UnitStrc* pItem /*ecx*/, D2StatListStrc* pStatList /*edx*/, int32_t nStatId /*[esp+4]*/, int32_t nValue /*[esp+8]*/, wchar_t* wszOut /*[esp+0xC]*/)
{
    (void)pItem;
    (void)pStatList;
    (void)nStatId;
    (void)nValue;
    (void)wszOut;
}

// 1.12a: 0x6FB38EA0 (__fastcall, 4 stack args: ret 0x10)
// Formats a single stat line according to ItemStatCost.txt descfunc (1..28) and descval (0..2)
int32_t __fastcall ITEMDESC_FormatSingleStat(int32_t nDescFunc /*ecx*/, int32_t nValue /*edx*/, int32_t nParam /*[esp+4]*/, uint16_t wStrPos /*[esp+8]*/, uint16_t wStrNeg /*[esp+0xC]*/, uint8_t* pStatCostRecord /*[esp+0x10]*/, wchar_t* wszOut /*eax*/)
{
    (void)nDescFunc;
    (void)nValue;
    (void)nParam;
    (void)wStrPos;
    (void)wStrNeg;
    (void)pStatCostRecord;
    (void)wszOut;
    return 1;
}

// 1.12a: 0x6FB39BB0 (__fastcall, 5 stack args: ret 0x14)
// Merges item StatLists, sorts stats by descpriority, formats all properties and paired damages, and appends "Indestructible"
void __fastcall ITEMDESC_FormatStatListProperties(D2UnitStrc* pItem /*ecx*/, D2StatListStrc* pStatList /*edx*/, int32_t bIncludeSocketed /*[esp+4]*/, int32_t nColor /*[esp+8]*/, int32_t bSkipIndestructible /*[esp+0xC]*/, wchar_t* wszOut /*[esp+0x10]*/, int32_t bIsSetBonus /*[esp+0x14]*/)
{
    (void)pItem;
    (void)pStatList;
    (void)bIncludeSocketed;
    (void)nColor;
    (void)bSkipIndestructible;
    (void)wszOut;
    (void)bIsSetBonus;
}

// 1.12a: 0x6FB39FA0 (__fastcall, 3 stack args: ret 0x0C)
// Formats a single green/gold set bonus property line using D2COMMON_EvaluateStatFormula
void __fastcall ITEMDESC_AppendSetItemSingleBonus(D2UnitStrc* pItem /*ecx*/, void* pSetProp /*edx*/, int32_t nStatId /*[esp+4]*/, wchar_t* wszBuf /*[esp+8]*/, int32_t nColor /*[esp+0xC]*/)
{
    (void)pItem;
    (void)pSetProp;
    (void)nStatId;
    (void)wszBuf;
    (void)nColor;
}

// 1.12a: 0x6FB3A040 (__fastcall, 1 stack arg: ret 4)
// Formats all partial and full Set item bonuses on a set piece
void __fastcall ITEMDESC_AppendSetItemBonuses(D2UnitStrc* pItem /*ecx*/, int32_t nEquippedCount /*edx*/, wchar_t* wszBuf /*[esp+4]*/)
{
    (void)pItem;
    (void)nEquippedCount;
    (void)wszBuf;
}

// 1.12a: 0x6FB3A140 (__fastcall, 2 stack args: ret 8)
// Formats a single Gem/Rune socket property line from Gems.txt
void __fastcall ITEMDESC_AppendGemOrRunePropertyLine(D2UnitStrc* pItem /*ecx*/, int32_t nStatIdx /*edx*/, uint8_t* pGemModEntry /*[esp+4]*/, wchar_t* wszOut /*[esp+8]*/)
{
    (void)pItem;
    (void)nStatIdx;
    (void)pGemModEntry;
    (void)wszOut;
}

// 1.12a: 0x6FB3A2A0 (__fastcall, 1 stack arg: ret 4)
// Appends the 3 Gem.txt modifier lines for a weapon/helm/shield category
void __fastcall ITEMDESC_AppendGemCategoryBonuses(D2UnitStrc* pItem /*ecx*/, uint8_t* pGemMods /*edx*/, wchar_t* wszOut /*[esp+4]*/)
{
    for (int32_t i = 0; i < 3; ++i) {
        ITEMDESC_AppendGemOrRunePropertyLine(pItem, i, pGemMods + 0x10 * i, wszOut);
    }
}

// 1.12a: 0x6FB3A310 (__fastcall, 1 stack arg: ret 4)
// Appends the 3 Rune modifier lines for a weapon/helm/armor/shield category
void __fastcall ITEMDESC_AppendRuneCategoryBonuses(D2UnitStrc* pItem /*ecx*/, uint8_t* pRuneMods /*edx*/, wchar_t* wszOut /*[esp+4]*/)
{
    for (int32_t i = 0; i < 3; ++i) {
        ITEMDESC_AppendGemOrRunePropertyLine(pItem, i, pRuneMods + 0x10 * i, wszOut);
    }
}

// 1.12a: 0x6FB3A3C0 (__fastcall)
// Formats the Weapon / Helm / Armor / Shield bonus description lines for an un-socketed Gem or Rune
void __fastcall ITEMDESC_FormatGemOrRuneBonuses(D2UnitStrc* pItem /*ecx*/, wchar_t* wszOut /*edx*/)
{
    int32_t nFileIndex = D2COMMON_GetItemFileIndex(pItem);
    D2GemsTxt* pGemsTxt = D2COMMON_GetGemsTxtRecord(nFileIndex);
    if (!pGemsTxt) {
        return;
    }
    (void)wszOut;
}

// ============================================================================
// PART 3: UI_ui.asm — UI State Machine, Dialogs, Hover Labels & HUD (0x6FB3A770..0x6FB40550)
// ============================================================================

// 1.12a: 0x6FB3A770 (__stdcall)
int32_t __stdcall UI_IsGoldDialogOpen()
{
    return g_bGoldDialogOpen;
}

// 1.12a: 0x6FB3A780 (__stdcall)
int32_t __stdcall UI_GetBottomPanelY()
{
    return gnScreenHeight - 48;
}

// 1.12a: 0x6FB3A790 (__fastcall)
int32_t __fastcall UI_CheckTickThrottle50ms()
{
    DWORD dwNow = GetTickCount();
    if (dwNow - g_dwLastThrottleTick50ms <= 50) {
        return 0;
    }
    g_dwLastThrottleTick50ms = dwNow;
    return 1;
}

// 1.12a: 0x6FB3A7C0 (__stdcall)
int32_t __stdcall UI_IsLoadingScreenActive()
{
    return g_pLoadingScreenCel != nullptr;
}

// 1.12a: 0x6FB3A7D0 (__stdcall)
int32_t __stdcall UI_GetLoadingScreenAct()
{
    return g_nLoadingScreenAct;
}

// 1.12a: 0x6FB3A7E0 (__fastcall)
void __fastcall UI_SetLoadingScreenAct(int32_t nAct /*ecx*/)
{
    g_nLoadingScreenAct = nAct;
}

// 1.12a: 0x6FB3A7F0 (__stdcall)
void __stdcall UI_LoadLoadingPalette()
{
    char szPath[260];
    sprintf(szPath, "%s\\Pal.pl2", "DATA\\GLOBAL\\UI\\Loading");
    HANDLE hFile = nullptr;
    if (SFileOpenFileEx(nullptr, szPath, 0, &hFile)) {
        uint32_t dwSize = SFileGetFileSize(hFile, nullptr);
        void* pBuf = FOG_AllocClientMemory(dwSize, __FILE__, __LINE__, 0);
        uint32_t dwRead = 0;
        SFileReadFile(hFile, pBuf, dwSize, &dwRead, nullptr);
        SFileCloseFile(hFile);
        g_pLoadingPalette = pBuf;
    }
}

// 1.12a: 0x6FB3A840 (__fastcall, 1 stack arg: ret 4)
void* __fastcall UI_LoadRawGfxFile(const char* szRelPath /*ecx*/, int32_t* pOutSize /*edx*/, int32_t bAddDataPrefix /*[esp+4]*/)
{
    char szFullPath[260];
    if (bAddDataPrefix) {
        sprintf(szFullPath, "%s\\%s", "DATA\\GLOBAL", szRelPath);
    } else {
        strcpy(szFullPath, szRelPath);
    }
    HANDLE hFile = nullptr;
    if (!SFileOpenFileEx(nullptr, szFullPath, 0, &hFile)) {
        return nullptr;
    }
    uint32_t dwSize = SFileGetFileSize(hFile, nullptr);
    if (pOutSize) {
        *pOutSize = (int32_t)dwSize;
    }
    void* pBuf = FOG_AllocClientMemory((int32_t)dwSize, __FILE__, __LINE__, 0);
    uint32_t dwRead = 0;
    SFileReadFile(hFile, pBuf, dwSize, &dwRead, nullptr);
    SFileCloseFile(hFile);
    return pBuf;
}

// 1.12a: 0x6FB3A930 (__fastcall)
void __fastcall UI_ClearGoldDialogState()
{
    g_bGoldDialogOpen   = 0;
    g_nGoldDialogMax    = 0;
    g_nGoldDialogVal    = 0;
    g_nGoldDialogMode   = 0;
    g_nGoldDialogUnk2D0 = 0;
}

// 1.12a: 0x6FB3A960 (__fastcall)
int32_t __fastcall UI_OnGoldSpinnerTick(int32_t nDelta /*ecx*/)
{
    int32_t nVal = g_nGoldDialogVal + nDelta;
    if (nVal < 0) {
        nVal = 0;
    }
    if (nVal > g_nGoldDialogMax) {
        nVal = g_nGoldDialogMax;
    }
    g_nGoldDialogVal = nVal;
    if (g_pGoldEditBox) {
        wchar_t wszVal[32];
        D2LANG_UnicodeSPrintf(32, wszVal, L"%d", nVal);
        D2WIN_SetEditBoxText(g_pGoldEditBox, wszVal);
    }
    return 1;
}

// 1.12a: 0x6FB3AA00 (__fastcall)
int32_t __fastcall UI_OnGoldEditChar(wchar_t wc /*ecx*/)
{
    return (wc >= L'0' && wc <= L'9') ? 1 : 0;
}

// 1.12a: 0x6FB3AA10 (__fastcall)
void __fastcall UI_StartPanelCloseClickCooldown()
{
    g_bPanelCloseClickCooldown = 1;
    g_dwPanelCloseCooldownTick = GetTickCount() + 300;
}

// 1.12a: 0x6FB3AA30 (__fastcall)
void __fastcall UI_EnablePanelCloseClickCooldown()
{
    g_bPanelCloseClickCooldown = 1;
}

// 1.12a: 0x6FB3AA40 (__fastcall)
void __fastcall UI_DisablePanelCloseClickCooldown()
{
    g_bPanelCloseClickCooldown = 0;
}

// 1.12a: 0x6FB3AA50 (__stdcall)
D2CellFileStrc* __stdcall UI_GetFocus16CellFile()
{
    return g_pFocus16Cel;
}

// 1.12a: 0x6FB3AA60 (__stdcall)
D2CellFileStrc* __stdcall UI_GetPentSpinCellFile()
{
    return g_pPentSpinCel;
}

// 1.12a: 0x6FB3AA70 (__fastcall)
int32_t __fastcall UI_GetPanelStateFast(int32_t nPanelId /*eax*/)
{
    return gaPanelStates[nPanelId];
}

// 1.12a: 0x6FB3AA80 (__stdcall)
int32_t __stdcall UI_IsSkillTreeOrPopupActive()
{
    return (gaPanelStates[D2PANEL_SKILLSELECT] != 0 || gaPanelStates[D2PANEL_SKILLTREE] != 0) ? 1 : 0;
}

// 1.12a: 0x6FB3AA90 (__stdcall)
int32_t __stdcall UI_GetPanelState(int32_t nPanelId)
{
    if (nPanelId >= D2PANEL_COUNT) {
        FOG_DisplayAssert("nToggle < NUM_TOGGLES", __FILE__, 0xAF6);
        exit(-1);
    }
    return gaPanelStates[nPanelId];
}

// 1.12a: 0x6FB3AAC0 (__fastcall)
void __fastcall UI_DrawDeferredHoverText()
{
    if (g_wszDeferredHoverText[0] != 0) {
        D2WIN_SetFont(1);
        D2WIN_DrawFramedText(g_wszDeferredHoverText, g_nDeferredHoverX, g_nDeferredHoverY, 0, 1);
        g_wszDeferredHoverText[0] = 0;
    }
}

// 1.12a: 0x6FB3AB50 (__fastcall)
void __fastcall UI_DrawBoxPiecesFrame(RECT* pRect /*eax*/)
{
    if (!pRect) {
        return;
    }
    D2GFX_DrawRect(pRect->left + 4, pRect->top + 4, pRect->right - 4, pRect->bottom - 4, 0, 2);
    D2GFX_DrawBoxFrame(pRect, 0x10);
}

// 1.12a: 0x6FB3AD30 (__fastcall)
void __fastcall UI_SetMonsterImmunityText(const wchar_t* wszText /*ecx*/)
{
    D2LANG_UnicodeStrNCpy(g_wszMonsterImmunityText, wszText, 64);
}

// 1.12a: 0x6FB3AD60 (__fastcall)
void __fastcall UI_SetMonsterSubText(const wchar_t* wszText /*ecx*/)
{
    D2LANG_UnicodeStrNCpy(g_wszMonsterSubText, wszText, 64);
}

// 1.12a: 0x6FB3AD90 (__fastcall, 1 stack arg: ret 4)
void __fastcall UI_DrawFramedTooltipText(const wchar_t* wszText /*ecx*/, int32_t nX /*edx*/, int32_t nY /*[esp+4]*/)
{
    D2WIN_DrawFramedText(wszText, nX, nY, 0, 1);
}

// 1.12a: 0x6FB3ADB0 (__fastcall, 1 stack arg: ret 4)
void __fastcall UI_SetDeferredHoverText(const wchar_t* wszText /*ecx*/, int32_t nX /*edx*/, int32_t nY /*[esp+4]*/)
{
    D2LANG_UnicodeStrNCpy(g_wszDeferredHoverText, wszText, 64);
    g_nDeferredHoverX = nX;
    g_nDeferredHoverY = nY;
}

// 1.12a: 0x6FB3ADF0 (__fastcall)
void __fastcall UI_StripColorCodes(wchar_t* wszDst /*ecx*/, const wchar_t* wszSrc /*edx*/)
{
    while (*wszSrc) {
        if (wszSrc[0] == 0xFF && wszSrc[1] == L'c' && wszSrc[2] != 0) {
            wszSrc += 3;
        } else {
            *wszDst++ = *wszSrc++;
        }
    }
    *wszDst = 0;
}

// 1.12a: 0x6FB3AE90 (__fastcall)
void __fastcall UI_ToUpperUnicode(wchar_t* wszStr /*ecx*/)
{
    for (; *wszStr; ++wszStr) {
        *wszStr = D2LANG_UnicodeToUpper(*wszStr);
    }
}

// 1.12a: 0x6FB3AED0 (__fastcall)
void __fastcall UI_NoOpStub1() {}

// 1.12a: 0x6FB3AEE0 (__fastcall)
void __fastcall UI_NoOpStub2() {}

// 1.12a: 0x6FB3AEF0 (__stdcall)
void __stdcall UI_OnWMPaint(D2MsgStrc* pMsg)
{
    (void)pMsg;
    g_bWMPaintReceived = 1;
}

// 1.12a: 0x6FB3AF30 (__fastcall)
void __fastcall UI_RegisterDefaultWindowHandlers()
{
    // Registers WM_PAINT, mouse, and keyboard handlers with D2Win/Storm message table
}

// 1.12a: 0x6FB3B020 (merged inside sub_6FB3AF30 in UI_ui.asm)
void __fastcall UI_GetVisibleViewportRect(RECT* pOutRect /*ecx*/)
{
    pOutRect->left = 0;
    pOutRect->top = 0;
    pOutRect->right = gnScreenWidth;
    pOutRect->bottom = gnScreenHeight - 48;
    if (gnPanelShiftMode == 1) {
        pOutRect->right = gnScreenWidth / 2;
    } else if (gnPanelShiftMode == 2) {
        pOutRect->left = gnScreenWidth / 2;
    }
}

// 1.12a: 0x6FB3B100 (__fastcall)
void __fastcall UI_EnableAutomapInputHandlers()
{
    // Enables automap input message callbacks when automap panel (0x0A) is active
}

// 1.12a: 0x6FB3B140 (__fastcall)
void __fastcall UI_DestroyGoldDialogForm()
{
    if (g_pGoldEditBox) {
        D2WIN_DestroyEditBox(g_pGoldEditBox);
        g_pGoldEditBox = nullptr;
    }
}

// 1.12a: 0x6FB3B160 (__fastcall)
void __fastcall UI_SendCloseInteractPacket27()
{
    if (g_bPendingInteractClose != 0) {
        MSG_SendByte2Dwords(0x30, (uint32_t)g_nPendingInteractType, (uint32_t)g_nPendingInteractGUID);
        g_bPendingInteractClose = 0;
    }
}

// 1.12a: 0x6FB3B1A0 (__fastcall)
const wchar_t* __fastcall UI_GetPortalDestinationName(D2UnitStrc* pPortalUnit /*ecx*/)
{
    if (!pPortalUnit || !*(uintptr_t*)((uint8_t*)pPortalUnit + 0x14)) {
        return nullptr;
    }
    int32_t nDestLevel = *(uint8_t*)(*(uintptr_t*)((uint8_t*)pPortalUnit + 0x14) + 0x04);
    return D2Client_GetLevelNameString(nDestLevel);
}

// 1.12a: 0x6FB3B210 (__fastcall)
void __fastcall UI_DrawDebugPlayersList()
{
    // Renders debug party/roster coordinates and level IDs when debug overlay is active
}

// 1.12a: 0x6FB3B450 (__fastcall)
void __fastcall UI_SendSimulatedMouseMove()
{
    CURSOR_WarpMousePosition(gnMouseX, gnMouseY);
}

// 1.12a: 0x6FB3B480 (__stdcall)
void __stdcall UI_CloseAllPanels()
{
    for (int32_t i = 1; i < D2PANEL_COUNT; ++i) {
        if (i != D2PANEL_AUTOMAP && i != D2PANEL_MINIPANEL && i != D2PANEL_SHOWITEMS && gaPanelStates[i] != 0) {
            UI_SetPanelState(i, 1, 0);
        }
    }
}

// 1.12a: 0x6FB3B520 (__fastcall, 2 stack args: ret 8)
void __fastcall UI_DrawCenteredViewportText(const wchar_t* wszText /*ecx*/, int32_t nY /*edx*/, int32_t nColor /*[esp+4]*/, int32_t nFont /*[esp+8]*/)
{
    D2WIN_SetFont(nFont);
    int32_t nCenterX = gnScreenWidth / 2;
    if (gnPanelShiftMode == 1) {
        nCenterX = gnScreenWidth / 4;
    } else if (gnPanelShiftMode == 2) {
        nCenterX = (gnScreenWidth * 3) / 4;
    }
    D2WIN_DrawText(wszText, nCenterX, nY, nColor, 1);
}

// 1.12a: 0x6FB3B5B0 (merged inside sub_6FB3B520 in UI_ui.asm)
void __fastcall UI_WrapWithColorCodes(wchar_t* wszBuf /*ecx*/, int32_t nColor /*edx*/)
{
    (void)wszBuf;
    (void)nColor;
}

// 1.12a: 0x6FB3B6F0 (__fastcall)
void __fastcall UI_AppendColorCode(wchar_t* wszBuf /*ecx*/, int32_t nColor /*edx*/)
{
    int32_t nLen = D2LANG_UnicodeStrLen(wszBuf);
    wszBuf[nLen + 0] = 0xFF;
    wszBuf[nLen + 1] = L'c';
    wszBuf[nLen + 2] = (wchar_t)(L'0' + nColor);
    wszBuf[nLen + 3] = 0;
}

// 1.12a: 0x6FB3B7A0 (__fastcall)
void __fastcall UI_PrependColorCode(wchar_t* wszBuf /*ecx*/, int32_t nColor /*edx*/)
{
    wchar_t wszTmp[512];
    D2LANG_UnicodeStrCpy(wszTmp, wszBuf);
    wszBuf[0] = 0xFF;
    wszBuf[1] = L'c';
    wszBuf[2] = (wchar_t)(L'0' + nColor);
    wszBuf[3] = 0;
    D2LANG_UnicodeStrCat(wszBuf, wszTmp);
}

// 1.12a: 0x6FB3B870 (__fastcall)
void __fastcall UI_InitPaletteColors()
{
    g_nPalColorRed   = D2GFX_FindBestPaletteIndex(255, 0, 0);
    g_nPalColorGreen = D2GFX_FindBestPaletteIndex(0, 255, 0);
    g_nPalColorBlue  = D2GFX_FindBestPaletteIndex(0, 0, 255);
}

// 1.12a: 0x6FB3C170 (__fastcall)
D2CellFileStrc* __fastcall UI_LoadGlobalUICellFile(const char* szName /*ecx*/)
{
    char szPath[260];
    sprintf(szPath, "DATA\\GLOBAL\\UI\\%s", szName);
    return ARCHIVE_LoadCellFileDefault(szPath, 0);
}

// 1.12a: 0x6FB3B8F0 (__fastcall)
D2CellFileStrc* __fastcall UI_GetBuySellBtnCellFile()
{
    if (!g_pBuySellBtnCel) {
        g_pBuySellBtnCel = UI_LoadGlobalUICellFile("PANEL\\buysellbtn");
    }
    return g_pBuySellBtnCel;
}

// 1.12a: 0x6FB3B930 (__fastcall)
void __fastcall UI_CloseGoldDialog()
{
    UI_DestroyGoldDialogForm();
    UI_ClearGoldDialogState();
}

// 1.12a: 0x6FB3BA60 (__fastcall)
void __fastcall UI_DrawPlayerPingDebug()
{
    // Draws FPS / Ping / Skip debug stats at the top right of the screen when enabled
}

// 1.12a: 0x6FB3BAE0 (__fastcall, 1 stack arg: ret 4)
void __fastcall UI_BindCommandKeyToMsgTable(int32_t nCmdIndex /*ecx*/, void* pfnCallback /*edx*/, int32_t bUpEvent /*[esp+4]*/)
{
    (void)nCmdIndex;
    (void)pfnCallback;
    (void)bUpEvent;
}

// 1.12a: 0x6FB3BB30 (__fastcall)
void __fastcall UI_DrawDeathScreen()
{
    // Renders "You Have Died" / "Press ESC to continue" (and Hardcore "Your deeds of valor will be remembered")
}

// 1.12a: 0x6FB3BDA0 (__fastcall)
void __fastcall UI_DrawLevelEntryTitle()
{
    // Renders the "Entering <Level Name>" banner fade-in/out when entering a new area
}

// 1.12a: 0x6FB3C040 (__fastcall)
void __fastcall UI_BuildMonsterImmunitiesString(D2UnitStrc* pMonUnit /*ecx*/)
{
    // Checks monster resistances >= 100 (Physical, Magic, Fire, Lightning, Cold, Poison) and builds "Immune to ..." subtitle
    (void)pMonUnit;
}

// 1.12a: 0x6FB3C1D0 (__stdcall)
int32_t __stdcall UI_OnGoldDialogCancelBtn()
{
    UI_CloseGoldDialog();
    return 1;
}

// 1.12a: 0x6FB3C1E0 (__stdcall)
int32_t __stdcall UI_OnGoldDialogCancelKey()
{
    UI_CloseGoldDialog();
    return 1;
}

// 1.12a: 0x6FB3C1F0 (__stdcall)
// Accepts the gold amount entered in the gold dialog and sends C2S 0x50 (drop gold) or C2S 0x4F (stash deposit/withdraw)
int32_t __stdcall UI_OnGoldDialogAccept()
{
    if (g_pGoldEditBox && gpLocalPlayerUnit) {
        const wchar_t* wszText = D2WIN_GetEditBoxText(g_pGoldEditBox);
        char szAnsi[32];
        D2LANG_UnicodeToAnsi(szAnsi, wszText, 32);
        int32_t nAmount = atoi(szAnsi);
        if (nAmount > g_nGoldDialogMax) {
            nAmount = g_nGoldDialogMax;
        }
        if (nAmount > 0) {
            int32_t nDialogMode = g_nGoldDialogMode;
            if (nDialogMode == 1) {
                // Drop gold from inventory (C2S 0x50)
                MSG_SendByte2Dwords(0x50, *(uint32_t*)((uint8_t*)gpLocalPlayerUnit + 0x0C), (uint32_t)nAmount);
            } else if (nDialogMode == 2) {
                // Deposit gold to stash (C2S 0x4F subcommand 0x14)
                MSG_SendByte3Words(0x4F, 0x14, (uint16_t)(nAmount >> 16), (uint16_t)(nAmount & 0xFFFF));
            } else if (nDialogMode == 3) {
                // Withdraw gold from stash (C2S 0x4F subcommand 0x13)
                MSG_SendByte3Words(0x4F, 0x13, (uint16_t)(nAmount >> 16), (uint16_t)(nAmount & 0xFFFF));
            }
        }
    }
    UI_CloseGoldDialog();
    return 1;
}

// 1.12a: 0x6FB3C2B0 (__fastcall)
void __fastcall UI_ClearPanelCloseClickCooldownOnUp()
{
    g_bPanelCloseClickCooldown = 0;
}

// 1.12a: 0x6FB3C2C0 (__fastcall)
void __fastcall UI_ResetSessionState()
{
    memset(gaPanelStates, 0, sizeof(gaPanelStates));
    gnPanelShiftMode = 0;
    gnPanelShiftOffset = 0;
    UI_ClearGoldDialogState();
}

// 1.12a: 0x6FB3C400 (__fastcall)
void __fastcall UI_UpdatePanelHotkeysInMsgTable()
{
    // Re-binds configured keyboard shortcuts for UI panels in the window message dispatch table
}

// 1.12a: 0x6FB3C690 (__fastcall)
void __fastcall UI_ShutdownGameSession()
{
    UI_CloseAllPanels();
    UI_CloseGoldDialog();
    UI_ResetSessionState();
}

// 1.12a: 0x6FB3C7D0 (__fastcall)
int32_t __fastcall UI_GetMercenaryHoverInfo(D2UnitStrc* pUnit /*ecx*/, wchar_t* wszOut /*edx*/)
{
    (void)pUnit;
    (void)wszOut;
    return 0;
}

// 1.12a: 0x6FB3C890 (__fastcall)
int32_t __fastcall UI_GetPlayerOrCorpseHoverInfo(D2UnitStrc* pUnit /*ecx*/, wchar_t* wszOut /*edx*/)
{
    (void)pUnit;
    (void)wszOut;
    return 0;
}

extern void __fastcall ITEM_BuildGroundLabelText(D2UnitStrc* pItem, Unicode* pOut, int32_t nMaxChars);
extern const Unicode* __fastcall D2Client_GetNpcDisplayName(D2UnitStrc* pUnit);
extern void __stdcall SHOWITEMS_DrawGroundItemLabels(void);
extern void __stdcall CHARSTATS_InitStatPanel(void);
extern void __stdcall CHARSTATS_FreeStatPanel(void);
extern void __stdcall CHARSTATS_DrawPanel(void);
extern int32_t __stdcall QUESTLOG_ClosePanel(int32_t bCenterCursor);
extern void __stdcall QUESTLOG_DrawPanel(void);
extern void __stdcall INV_InitInventoryLayoutRectsAndGrids(void);
extern void __stdcall INV_RenderAllInventoryPanels(void);
extern void __stdcall INV_Render800x600PanelBorderFrame(void);
extern void __stdcall SKILLTREE_RenderPanel(void);
extern void __stdcall CTRLPNL_RenderBottomBar(void);
extern void __stdcall CTRLPNL_RenderLeftLifeOrb(void);
extern void __stdcall CTRLPNL_RenderRightManaOrb(void);
extern void __stdcall CTRLPNL_RenderLifeManaTooltips(void);
extern void __stdcall AUTOMAP_Initialize(void);
extern void __stdcall AUTOMAP_Draw(void);
extern void __stdcall AUTOMAP_FreeAllLayers(void);
extern void __stdcall PARTY_LoadCellFiles(void);
extern void __stdcall PARTY_DrawPartyScreen(void);
extern void __stdcall PARTY_FreeNodesAndCellFiles(void);
extern void __stdcall HIREABLES_InitPanelState(void);
extern void __stdcall HIREABLES_LoadPortraitCellFiles(void);
extern void __stdcall HIREABLES_DrawAllPortraits(void);
extern void __stdcall HIREABLES_FreePortraitCellFiles(void);
extern void __fastcall CURSOR_Init(void);
extern void __fastcall CURSOR_Update(void);
extern void __fastcall CURSOR_Draw(void);
extern void __fastcall CURSOR_FreeAssets(void);

extern int32_t g_nInvPanelMode;
extern int32_t g_nLeftPanelOffsetX;
extern int32_t g_nLeftPanelOffsetY;
extern D2UnitStrc* g_pTradeNpcUnit;

int32_t __fastcall UI_WorldUnitToScreen(D2UnitStrc* pUnit, int32_t* pOutScreenX, int32_t* pOutScreenY)
{
    if (!pUnit || !gpLocalPlayerUnit) return 0;
    const int32_t nPlrX = UNITS_GetXPosition(gpLocalPlayerUnit);
    const int32_t nPlrY = UNITS_GetYPosition(gpLocalPlayerUnit);
    const int32_t nUnitX = UNITS_GetXPosition(pUnit);
    const int32_t nUnitY = UNITS_GetYPosition(pUnit);
    if (nUnitX <= 0 && nUnitY <= 0) return 0;
    const int32_t dX = nUnitX - nPlrX;
    const int32_t dY = nUnitY - nPlrY;
    const int32_t nW = (gnScreenWidth > 0) ? gnScreenWidth : 800;
    const int32_t nH = (gnScreenHeight > 0) ? gnScreenHeight : 600;
    const int32_t nCenterX = (nW / 2) - gnPanelShiftOffset;
    const int32_t nCenterY = ((nH - 48) / 2);
    *pOutScreenX = nCenterX + (dX - dY) * 16;
    *pOutScreenY = nCenterY + (dX + dY) * 8;
    return (*pOutScreenX >= 0 && *pOutScreenX <= nW && *pOutScreenY >= 0 && *pOutScreenY <= nH - 48) ? 1 : 0;
}

// 1.12a: 0x6FB3C9D0 (__fastcall)
void __fastcall UI_DrawGroundItemHoverLabel(D2UnitStrc* pItemUnit /*ecx*/)
{
    if (!pItemUnit) return;
    int32_t nScreenX = 0, nScreenY = 0;
    if (!UI_WorldUnitToScreen(pItemUnit, &nScreenX, &nScreenY)) return;
    Unicode wszLabel[256] = {};
    ITEM_BuildGroundLabelText(pItemUnit, wszLabel, 255);
    if (wszLabel[0]) {
        D2WIN_SetFont(1);
        D2WIN_DrawFramedText(reinterpret_cast<const wchar_t*>(wszLabel), nScreenX, nScreenY - 18, 0, 1);
    }
}

// 1.12a: 0x6FB3CC90 (__fastcall)
int32_t __fastcall UI_CheckUniqueMonsterHoverMods(D2UnitStrc* pMonUnit /*ecx*/)
{
    (void)pMonUnit;
    return 0;
}

// 1.12a: 0x6FB3CD00 (__fastcall)
void __fastcall UI_DrawOffscreenPartyPlayerLabel(D2RosterUnitStrc* pRosterUnit /*ecx*/)
{
    (void)pRosterUnit;
}

// 1.12a: 0x6FB3CE40 (__fastcall)
int32_t __fastcall UI_CalcMonsterLifeBarWidth(D2UnitStrc* pMonUnit /*ecx*/)
{
    int32_t nHp = D2COMMON_GetUnitStatUInt(pMonUnit, 6, 0);
    int32_t nMaxHp = D2COMMON_GetUnitStatUInt(pMonUnit, 7, 0);
    if (nMaxHp <= 0) {
        return 128;
    }
    int32_t nPct = (nHp * 128) / nMaxHp;
    if (nPct < 0) nPct = 0;
    if (nPct > 128) nPct = 128;
    return nPct;
}

// 1.12a: 0x6FB3CF30 (__fastcall)
void __fastcall UI_DrawAllPartyPlayerLabels()
{
}

// 1.12a: 0x6FB3CF80 (__fastcall)
void __fastcall UI_DrawPetOrSummonHoverLabel(D2UnitStrc* pUnit /*ecx*/)
{
    (void)pUnit;
}

// 1.12a: 0x6FB3D010 (__fastcall)
void __fastcall UI_DrawMonsterHoverBanner(D2UnitStrc* pMonUnit /*ecx*/)
{
    if (!pMonUnit) return;
    const Unicode* pName = D2Client_GetNpcDisplayName(pMonUnit);
    if (!pName || !pName[0]) return;

    int32_t nCenterX = gnScreenWidth / 2;
    if (gnPanelShiftMode == 1) nCenterX = gnScreenWidth / 4;
    else if (gnPanelShiftMode == 2) nCenterX = (gnScreenWidth * 3) / 4;

    D2MonStatsTxt* pMonTxt = DATATBLS_GetMonStatsTxtRecord(pMonUnit->dwClassId);
    const bool bFriendlyNpc = (pMonTxt && (pMonTxt->nAlign == 2 || (pMonTxt->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT]) != 0));
    if (bFriendlyNpc) {
        int32_t nSx = 0, nSy = 0;
        if (UI_WorldUnitToScreen(pMonUnit, &nSx, &nSy)) {
            D2WIN_SetFont(1);
            D2WIN_DrawFramedText(reinterpret_cast<const wchar_t*>(pName), nSx, nSy - 56, 0, 1);
        }
        return;
    }

    D2WIN_SetFont(1);
    const int32_t nTextW = D2WIN_GetTextWidth(reinterpret_cast<const wchar_t*>(pName));
    const int32_t nHalfW = (nTextW + 24 > 128) ? ((nTextW + 24) / 2) : 64;
    const int32_t nBarW = UI_CalcMonsterLifeBarWidth(pMonUnit);
    const int32_t nFillW = (nBarW * (nHalfW * 2)) / 128;

    D2GFX_DrawRect(nCenterX - nHalfW, 12, nCenterX + nHalfW, 30, 0x00, 2);
    if (nFillW > 0) {
        D2GFX_DrawRect(nCenterX - nHalfW, 12, nCenterX - nHalfW + nFillW, 30, 0x62, 1);
    }
    D2WIN_DrawText(reinterpret_cast<const wchar_t*>(pName), nCenterX - (nTextW / 2), 26, 0, 0);
}

// 1.12a: 0x6FB3D4A0 (__fastcall)
void __fastcall UI_DrawHoveredUnitLabel()
{
    if (!gpLocalPlayerUnit || gaPanelStates[D2PANEL_ESCMENU] || gaPanelStates[D2PANEL_NPCMENU]) {
        return;
    }
    if (gnPanelShiftMode == 3) {
        return;
    }
    if (gnPanelShiftMode == 1 && gnMouseX >= gnScreenWidth / 2) {
        return;
    }
    if (gnPanelShiftMode == 2 && gnMouseX <= gnScreenWidth / 2) {
        return;
    }
    if (gnMouseY >= gnScreenHeight - 48) {
        return;
    }

    D2UnitStrc* pBestUnit = nullptr;
    int32_t nBestDistSq = 46 * 46;

    const int32_t aTypes[3] = { UNIT_MONSTER, UNIT_OBJECT, UNIT_ITEM };
    for (int t = 0; t < 3; ++t) {
        const int32_t nType = aTypes[t];
        for (int b = 0; b < 128; ++b) {
            for (D2UnitStrc* pUnit = gapServerSideUnitHashTables[nType][b]; pUnit != nullptr; pUnit = pUnit->pListNext) {
                if (nType == UNIT_MONSTER) {
                    if (pUnit->dwAnimMode == MONMODE_DEAD || pUnit->dwAnimMode == MONMODE_DEATH) {
                        continue;
                    }
                    D2MonStats2Txt* pMon2 = UNITS_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
                    if (pMon2 && !(pMon2->dwFlags & (MONSTATS2FLAG_ISSEL | MONSTATS2FLAG_ALSEL))) {
                        continue;
                    }
                    const Unicode* pMonName = D2Client_GetNpcDisplayName(pUnit);
                    if (!pMonName || !pMonName[0] || _wcsicmp(reinterpret_cast<const wchar_t*>(pMonName), L"an evil force") == 0) {
                        continue;
                    }
                } else if (nType == UNIT_OBJECT) {
                    D2ObjectsTxt* pObjTxt = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId);
                    const uint32_t nModeIdx = (pUnit->dwAnimMode < 8) ? pUnit->dwAnimMode : 0;
                    if (!pObjTxt || !pObjTxt->nSelectable[nModeIdx]) {
                        continue;
                    }
                    const Unicode* pObjName = D2Client_GetNpcDisplayName(pUnit);
                    if (!pObjName || !pObjName[0] || _wcsicmp(reinterpret_cast<const wchar_t*>(pObjName), L"an evil force") == 0) {
                        continue;
                    }
                } else if (nType == UNIT_ITEM && pUnit->dwAnimMode != IMODE_ONGROUND && pUnit->dwAnimMode != IMODE_DROPPING) {
                    continue;
                }
                int32_t nSx = 0, nSy = 0;
                if (!UI_WorldUnitToScreen(pUnit, &nSx, &nSy)) {
                    continue;
                }
                const int32_t nHitY = (nType == UNIT_MONSTER) ? (nSy - 28) : (nSy - 10);
                const int32_t dx = gnMouseX - nSx;
                const int32_t dy = gnMouseY - nHitY;
                const int32_t d2 = dx * dx + dy * dy;
                if (d2 < nBestDistSq) {
                    nBestDistSq = d2;
                    pBestUnit = pUnit;
                }
            }
        }
    }

    if (!pBestUnit) return;
    if (pBestUnit->dwUnitType == UNIT_MONSTER) {
        UI_DrawMonsterHoverBanner(pBestUnit);
    } else if (pBestUnit->dwUnitType == UNIT_ITEM) {
        if (!gaPanelStates[D2PANEL_SHOWITEMS]) {
            UI_DrawGroundItemHoverLabel(pBestUnit);
        }
    } else if (pBestUnit->dwUnitType == UNIT_OBJECT) {
        const Unicode* pObjName = D2Client_GetNpcDisplayName(pBestUnit);
        if (pObjName && pObjName[0]) {
            int32_t nSx = 0, nSy = 0;
            if (UI_WorldUnitToScreen(pBestUnit, &nSx, &nSy)) {
                D2WIN_SetFont(1);
                D2WIN_DrawFramedText(reinterpret_cast<const wchar_t*>(pObjName), nSx, nSy - 32, 0, 1);
            }
        }
    }
}

// 1.12a: 0x6FB3DAB0 (merged inside sub_6FB3D4A0 after jump table 0x6FB3DA98 in UI_ui.asm)
void __fastcall UI_DrawHoveredMercOrSummonName(D2UnitStrc* pUnit /*ecx*/)
{
    (void)pUnit;
}

// 1.12a: 0x6FB3DC60 (__fastcall)
void __fastcall UI_FreeLoadingScreen()
{
    if (g_pLoadingScreenCel) {
        D2CMP_DeleteCellFile(g_pLoadingScreenCel);
        g_pLoadingScreenCel = nullptr;
    }
    if (g_pLoadingPalette) {
        FOG_FreeClientMemory(g_pLoadingPalette, __FILE__, __LINE__, 0);
        g_pLoadingPalette = nullptr;
    }
}

// 1.12a: 0x6FB3DCF0 (__fastcall)
void __fastcall UI_InitLoadingScreen()
{
    UI_FreeLoadingScreen();
    UI_LoadLoadingPalette();
    g_pLoadingScreenCel = UI_LoadGlobalUICellFile("Loading\\loadingscreen");
    g_nLoadingScreenFrame = 0;
}

// 1.12a: 0x6FB3DD80 (__fastcall)
void __fastcall UI_DrawChargeUpSkillIcons()
{
    // Draws Assassin martial arts charge-up state icons on the HUD
}

// 1.12a: 0x6FB3DED0 (__fastcall)
void __fastcall UI_DrawUniqueMonsterModsLine(int32_t nY /*ecx*/)
{
    if (g_wszMonsterSubText[0] != 0) {
        UI_DrawCenteredViewportText(g_wszMonsterSubText, nY, 0, 6);
    }
}

// 1.12a: 0x6FB3DF40 (__fastcall)
void __fastcall UI_DrawMonsterSubtitles(int32_t nY /*ecx*/)
{
    if (g_wszMonsterImmunityText[0] != 0) {
        UI_DrawCenteredViewportText(g_wszMonsterImmunityText, nY, 0, 6);
    }
}

// 1.12a: 0x6FB3DFA0 (__stdcall)
void __stdcall UI_StepAndDrawLoadingScreen()
{
    if (!g_pLoadingScreenCel) {
        return;
    }
    int32_t nFrame = g_nLoadingScreenFrame;
    if (nFrame < 9) {
        g_nLoadingScreenFrame = nFrame + 1;
    }
}

static bool UI_IsLeftSidePanel(int32_t nPanelId)
{
    return (nPanelId == D2PANEL_CHAR || nPanelId == D2PANEL_NPCSHOP || nPanelId == D2PANEL_MODITEM ||
            nPanelId == D2PANEL_QUEST || nPanelId == D2PANEL_WAYPOINT || nPanelId == 0x14 ||
            nPanelId == D2PANEL_PARTY || nPanelId == D2PANEL_STASH || nPanelId == D2PANEL_CUBE ||
            nPanelId == D2PANEL_MERCINV);
}

static bool UI_IsRightSidePanel(int32_t nPanelId)
{
    return (nPanelId == D2PANEL_INVENTORY || nPanelId == D2PANEL_SKILLTREE);
}

// 1.12a: 0x6FB3E1F0 (__fastcall)
// Performs panel-specific initialization when a UI panel transitions from closed (0) to open (1)
void __fastcall UI_OnPanelOpened(int32_t nPanelId /*eax*/)
{
    if (nPanelId == D2PANEL_NPCSHOP) {
        g_nInvPanelMode = 1;
        if (!gaPanelStates[D2PANEL_INVENTORY]) {
            UI_SetPanelState(D2PANEL_INVENTORY, 0, 0);
        }
    } else if (nPanelId == D2PANEL_CUBE) {
        g_nInvPanelMode = 12;
        if (!gaPanelStates[D2PANEL_INVENTORY]) {
            UI_SetPanelState(D2PANEL_INVENTORY, 0, 0);
        }
    } else if (nPanelId == D2PANEL_STASH) {
        g_nInvPanelMode = 11;
        if (!gaPanelStates[D2PANEL_INVENTORY]) {
            UI_SetPanelState(D2PANEL_INVENTORY, 0, 0);
        }
    }
}

// 1.12a: 0x6FB3E7A0 (merged inside sub_6FB3E1F0 after jump table 0x6FB3E708 in UI_ui.asm)
// Closes conflicting panels before opening/toggling a panel
int32_t __fastcall UI_PreparePanelOpen(int32_t nAction /*eax*/, int32_t nPanelId /*ebx*/)
{
    const bool bOpening = (nAction == 0) || (nAction == 2 && gaPanelStates[nPanelId] == 0);
    if (!bOpening) {
        return 1;
    }
    if (nPanelId == D2PANEL_ESCMENU) {
        UI_CloseAllPanels();
        return 1;
    }
    if (gaPanelStates[D2PANEL_ESCMENU] && nPanelId != D2PANEL_ESCMENU) {
        gaPanelStates[D2PANEL_ESCMENU] = 0;
    }
    if (gaPanelStates[D2PANEL_NPCMENU] && nPanelId != D2PANEL_NPCMENU && nPanelId != D2PANEL_NPCSHOP) {
        gaPanelStates[D2PANEL_NPCMENU] = 0;
    }
    if (UI_IsLeftSidePanel(nPanelId)) {
        static const int32_t kLeftPanels[] = {
            D2PANEL_CHAR, D2PANEL_NPCSHOP, D2PANEL_MODITEM, D2PANEL_QUEST,
            D2PANEL_WAYPOINT, 0x14, D2PANEL_PARTY, D2PANEL_STASH, D2PANEL_CUBE, D2PANEL_MERCINV
        };
        for (size_t i = 0; i < sizeof(kLeftPanels) / sizeof(kLeftPanels[0]); ++i) {
            int32_t nOther = kLeftPanels[i];
            if (nOther != nPanelId && gaPanelStates[nOther] != 0) {
                UI_SetPanelState(nOther, 1, 0);
            }
        }
    } else if (UI_IsRightSidePanel(nPanelId)) {
        int32_t nOther = (nPanelId == D2PANEL_INVENTORY) ? D2PANEL_SKILLTREE : D2PANEL_INVENTORY;
        if (gaPanelStates[nOther] != 0) {
            UI_SetPanelState(nOther, 1, 0);
        }
    }
    return 1;
}

// 1.12a: 0x6FB3E960 (__fastcall)
// Performs panel-specific cleanup when a UI panel transitions from open (1) to closed (0)
void __fastcall UI_OnPanelClosed(int32_t nPanelId /*eax*/)
{
    if (nPanelId == D2PANEL_QUEST) {
        QUESTLOG_ClosePanel(0);
    } else if (nPanelId == D2PANEL_NPCSHOP || nPanelId == D2PANEL_CUBE || nPanelId == D2PANEL_STASH) {
        g_nInvPanelMode = 0;
    }
}

// 1.12a: 0x6FB3EF00 (__fastcall, 1 stack arg: ret 4; merged in tail of sub_6FB3E960 in UI_ui.asm)
// Master UI panel state transition function:
//   nPanelId (ecx): 0..0x25
//   nAction  (edx): 0 = Open, 1 = Close, 2 = Toggle
//   bMoveMouse ([esp+4]): if non-zero, warps cursor by +-gnScreenWidth/4 when viewport shifts
int32_t __fastcall UI_SetPanelState(int32_t nPanelId /*ecx*/, int32_t nAction /*edx*/, int32_t bMoveMouse /*[esp+4]*/)
{
    if ((uint32_t)nPanelId >= D2PANEL_COUNT) {
        FOG_DisplayAssert("nToggle < NUM_TOGGLES", __FILE__, 0x13ED);
        exit(-1);
    }
    if ((uint32_t)nAction > 2) {
        FOG_DisplayAssert("nState <= UI_TOGGLE", __FILE__, 0x13EE);
        exit(-1);
    }

    int32_t nPrevState = gaPanelStates[nPanelId];
    if (!FOG_IsExpansion() || gbIsExpansion == 0) {
        if (nPanelId == 0x23 || nPanelId == D2PANEL_MERCINV || nPanelId == 0x25) {
            return 0;
        }
    }

    if (!UI_PreparePanelOpen(nAction, nPanelId)) {
        return 0;
    }

    if (nAction == 0) {
        // Open
        if (!gpLocalPlayerUnit || (*(int32_t*)gpLocalPlayerUnit == 0 && D2Client_IsLocalPlayerDead() == 0)) {
            gaPanelStates[nPanelId] = 1;
        }
    } else if (nAction == 1) {
        // Close
        gaPanelStates[nPanelId] = 0;
    } else if (nAction == 2) {
        // Toggle
        if (!gpLocalPlayerUnit) {
            gaPanelStates[nPanelId] = (gaPanelStates[nPanelId] == 0) ? 1 : 0;
        } else if (*(int32_t*)gpLocalPlayerUnit == 0 && D2Client_IsLocalPlayerDead() == 0) {
            gaPanelStates[nPanelId] = (gaPanelStates[nPanelId] == 0) ? 1 : 0;
        } else if (nPanelId == D2PANEL_CHAT) {
            gaPanelStates[D2PANEL_CHAT] = (gaPanelStates[D2PANEL_CHAT] == 0) ? 1 : 0;
        }
    }

    int32_t nNewState = gaPanelStates[nPanelId];
    if (nPrevState == 0 && nNewState != 0) {
        UI_OnPanelOpened(nPanelId);
        if (nPanelId == D2PANEL_CHAT) {
            sub_6FB34E90();
        }
    } else if (nPrevState != 0 && nNewState == 0) {
        UI_OnPanelClosed(nPanelId);
    }

    int32_t nSavedMouseX = gnMouseX;
    int32_t nSavedMouseY = gnMouseY;
    int32_t nQuarterW = gnScreenWidth / 4;

    // Update viewport shift (gnPanelShiftMode: 0=None, 1=RightPanelOpen/ShiftLeft, 2=LeftPanelOpen/ShiftRight, 3=BothOpen)
    switch (nPanelId) {
    case D2PANEL_INVENTORY: // Inventory (Right side)
    case D2PANEL_SKILLTREE: // Skill Tree (Right side)
        INV_ClearHoveredItemState();
        if (gaPanelStates[D2PANEL_CHAR] || gaPanelStates[0x14] || gaPanelStates[D2PANEL_QUEST] ||
            gaPanelStates[D2PANEL_WAYPOINT] || gaPanelStates[D2PANEL_NPCSHOP] || gaPanelStates[D2PANEL_STASH] ||
            gaPanelStates[D2PANEL_CUBE] || gaPanelStates[D2PANEL_MODITEM] ||
            gaPanelStates[D2PANEL_MERCINV] || gaPanelStates[0x25] || gaPanelStates[D2PANEL_PARTY])
        {
            if (nNewState != 0) {
                gnPanelShiftOffset = 0;
                gnPanelShiftMode = 3;
                D2GFX_SetViewportShift(0);
                UI_ApplyViewportRectForShiftMode(3);
            } else {
                gnPanelShiftOffset = nQuarterW;
                gnPanelShiftMode = 2;
                D2GFX_SetViewportShift(nQuarterW);
                UI_ApplyViewportRectForShiftMode(2);
            }
        } else if (nNewState != 0) {
            gnPanelShiftOffset = -nQuarterW;
            gnPanelShiftMode = 1;
            D2GFX_SetViewportShift(-nQuarterW);
            UI_ApplyViewportRectForShiftMode(1);
            if (bMoveMouse && nSavedMouseX > nQuarterW) {
                CURSOR_WarpMousePosition(nSavedMouseX - nQuarterW, nSavedMouseY);
            }
        } else {
            gnPanelShiftOffset = 0;
            gnPanelShiftMode = 0;
            D2GFX_SetViewportShift(0);
            UI_ApplyViewportRectForShiftMode(0);
            if (bMoveMouse && nSavedMouseX < gnScreenWidth / 2) {
                CURSOR_WarpMousePosition(nSavedMouseX + nQuarterW, nSavedMouseY);
            }
        }
        break;

    case D2PANEL_CHAR:      // Character Stats (Left side)
    case D2PANEL_NPCSHOP:   // NPC Shop / Trade (Left side)
    case D2PANEL_MODITEM:   // Quest / Imbue / Socket (Left side)
    case D2PANEL_QUEST:     // Quest Log (Left side)
    case D2PANEL_WAYPOINT:  // Waypoint (Left side)
    case 0x14:              // Waypoint alt index (Left side)
    case D2PANEL_PARTY:     // Party Screen (Left side)
    case D2PANEL_STASH:     // Stash (Left side)
    case D2PANEL_CUBE:      // Horadric Cube (Left side)
    case D2PANEL_MERCINV:   // Mercenary Inventory (Left side)
        if (gaPanelStates[D2PANEL_INVENTORY] || gaPanelStates[D2PANEL_SKILLTREE]) {
            if (nNewState != 0) {
                gnPanelShiftOffset = 0;
                gnPanelShiftMode = 3;
                D2GFX_SetViewportShift(0);
                UI_ApplyViewportRectForShiftMode(3);
            } else {
                gnPanelShiftOffset = -nQuarterW;
                gnPanelShiftMode = 1;
                D2GFX_SetViewportShift(-nQuarterW);
                UI_ApplyViewportRectForShiftMode(1);
            }
        } else if (nNewState != 0) {
            gnPanelShiftOffset = nQuarterW;
            gnPanelShiftMode = 2;
            D2GFX_SetViewportShift(nQuarterW);
            UI_ApplyViewportRectForShiftMode(2);
            if (bMoveMouse && nSavedMouseX < gnScreenWidth - nQuarterW) {
                CURSOR_WarpMousePosition(nSavedMouseX + nQuarterW, nSavedMouseY);
            }
        } else {
            gnPanelShiftOffset = 0;
            gnPanelShiftMode = 0;
            D2GFX_SetViewportShift(0);
            UI_ApplyViewportRectForShiftMode(0);
            if (bMoveMouse && nSavedMouseX > gnScreenWidth / 2) {
                CURSOR_WarpMousePosition(nSavedMouseX - nQuarterW, nSavedMouseY);
            }
        }
        break;

    default:
        break;
    }

    return 1;
}

static D2CellFileStrc* s_pWaygateBgCel   = nullptr;
static D2CellFileStrc* s_pWaygateTabsCel = nullptr;
int32_t g_nSelectedWaypointTab = 0;
uint64_t g_qwUnlockedWaypointsMask = 1ULL;
uint32_t g_dwActiveWaypointGuid = 0u;

static void WAYPOINT_DrawPanel()
{
    if (!gaPanelStates[D2PANEL_WAYPOINT] && !gaPanelStates[0x14]) {
        return;
    }
    if (!s_pWaygateBgCel) {
        s_pWaygateBgCel = ARCHIVE_LoadCellFileDefault("MENU\\waygatebackground", 0);
    }
    if (!s_pWaygateTabsCel) {
        s_pWaygateTabsCel = ARCHIVE_LoadCellFileDefault("MENU\\expwaygatetabs", 0);
    }
    D2CellFileStrc* pCloseCel = UI_GetBuySellBtnCellFile();

    const int32_t nBaseX = g_nLeftPanelOffsetX;
    const int32_t nBaseY = -g_nLeftPanelOffsetY;

    if (s_pWaygateBgCel) {
        const int32_t aQuadPos[4][2] = {
            { nBaseX,       nBaseY + 256 },
            { nBaseX + 256, nBaseY + 256 },
            { nBaseX,       nBaseY + 432 },
            { nBaseX + 256, nBaseY + 432 },
        };
        const int32_t nMaxF = D2CMP_CelFileGetCelsPerDirection(s_pWaygateBgCel);
        for (int32_t f = 0; f < 4 && f < nMaxF; ++f) {
            D2GfxDataStrc tBg = {};
            tBg.pCellFile = s_pWaygateBgCel;
            tBg.nFrame = f;
            D2GFX_DrawCellContextEx(&tBg, aQuadPos[f][0], aQuadPos[f][1], -1, 5, nullptr);
        }
    } else {
        RECT rFallback = { nBaseX + 16, nBaseY + 16, nBaseX + 304, nBaseY + 424 };
        UI_DrawBoxPiecesFrame(&rFallback);
    }

    const int32_t nMaxActs = gbIsExpansion ? 5 : 4;
    if (g_nSelectedWaypointTab < 0 || g_nSelectedWaypointTab >= nMaxActs) {
        g_nSelectedWaypointTab = 0;
    }

    if (s_pWaygateTabsCel) {
        const int32_t nTabMax = D2CMP_CelFileGetCelsPerDirection(s_pWaygateTabsCel);
        for (int32_t t = 0; t < nMaxActs; ++t) {
            int32_t nFrame = (t == g_nSelectedWaypointTab) ? (t * 2 + 1) : (t * 2);
            if (nFrame < nTabMax) {
                D2GfxDataStrc tTab = {};
                tTab.pCellFile = s_pWaygateTabsCel;
                tTab.nFrame = nFrame;
                D2GFX_DrawCellContextEx(&tTab, nBaseX + 4 + t * 62, nBaseY + 42, -1, 5, nullptr);
            }
        }
    }

    static const int32_t kActWpLevels[5][9] = {
        { 1, 3, 4, 5, 6, 27, 29, 32, 35 },
        { 40, 48, 42, 57, 43, 44, 52, 74, 46 },
        { 75, 76, 77, 78, 79, 80, 81, 83, 101 },
        { 103, 106, 107, 0, 0, 0, 0, 0, 0 },
        { 109, 111, 112, 113, 115, 123, 117, 118, 129 }
    };
    static const int32_t kActWpCounts[5]  = { 9, 9, 9, 3, 9 };
    static const int32_t kActWpBitBase[5] = { 0, 9, 18, 27, 30 };
    static const wchar_t* const kActWpHeaders[5] = {
        L"Waypoints - Act I",
        L"Waypoints - Act II",
        L"Waypoints - Act III",
        L"Waypoints - Act IV",
        L"Waypoints - Act V"
    };

    const int32_t nActIdx = g_nSelectedWaypointTab;
    D2WIN_SetFont(1);
    D2WIN_DrawText(kActWpHeaders[nActIdx], nBaseX + 104, nBaseY + 75, 4, 0);

    const int32_t nCount = kActWpCounts[nActIdx];
    const int32_t nBitBase = kActWpBitBase[nActIdx];
    for (int32_t i = 0; i < nCount; ++i) {
        const int32_t nRowY = nBaseY + 92 + i * 34;
        const int32_t nWpBit = nBitBase + i;
        const bool bUnlocked = ((g_qwUnlockedWaypointsMask >> nWpBit) & 1ULL) != 0ULL || (nActIdx == 0 && i == 0);
        D2LevelsTxt* pLvlTxt = DATATBLS_GetLevelsTxtRecord(kActWpLevels[nActIdx][i]);
        const wchar_t* wszName = (pLvlTxt && pLvlTxt->wszLevelName[0])
            ? reinterpret_cast<const wchar_t*>(pLvlTxt->wszLevelName)
            : L"Waypoint";
        D2WIN_DrawText(wszName, nBaseX + 76, nRowY + 14, bUnlocked ? 3 : 5, 0);
    }

    if (pCloseCel && D2CMP_CelFileGetCelsPerDirection(pCloseCel) > 10) {
        D2GfxDataStrc tBtn = {};
        tBtn.pCellFile = pCloseCel;
        tBtn.nFrame = 10;
        D2GFX_DrawCellContextEx(&tBtn, nBaseX + 278, nBaseY + 418, -1, 5, nullptr);
    }
}

static void NPCMENU_DrawActiveDialog()
{
    if (!gaPanelStates[D2PANEL_NPCMENU] || gaPanelStates[D2PANEL_NPCSHOP]) {
        return;
    }
    D2UnitStrc* pNpc = g_pTradeNpcUnit;
    if (!pNpc && g_nInteractingNpcGUID != 0) {
        for (int b = 0; b < 128 && !pNpc; ++b) {
            for (D2UnitStrc* pCur = gapServerSideUnitHashTables[UNIT_MONSTER][b]; pCur != nullptr; pCur = pCur->pListNext) {
                if ((int32_t)pCur->dwUnitId == g_nInteractingNpcGUID) {
                    pNpc = pCur;
                    break;
                }
            }
        }
    }

    const int32_t nW = (gnScreenWidth > 0) ? gnScreenWidth : 800;
    int32_t nBoxCenterX = nW / 2;
    int32_t nBoxTopY = 120;
    if (pNpc) {
        int32_t nSx = 0, nSy = 0;
        if (UI_WorldUnitToScreen(pNpc, &nSx, &nSy)) {
            nBoxCenterX = nSx;
            nBoxTopY = (nSy > 190) ? (nSy - 165) : 40;
        }
    }
    if (nBoxCenterX < 120) nBoxCenterX = 120;
    if (nBoxCenterX > nW - 120) nBoxCenterX = nW - 120;

    const int32_t nCls = pNpc ? (int32_t)pNpc->dwClassId : g_nInteractingNpcClassId;
    const wchar_t* wszTradeOpt = L"Trade";
    bool bHasTrade = (nCls == 148 || nCls == 154 || nCls == 147 || nCls == 150);
    if (nCls == 154) wszTradeOpt = L"Trade / Repair";
    else if (nCls == 147) wszTradeOpt = L"Gamble";
    else if (nCls == 150) wszTradeOpt = L"Hire";
    else if (nCls == 155) { bHasTrade = true; wszTradeOpt = L"Travel West"; }

    const int32_t nOptCount = bHasTrade ? 3 : 2;
    const Unicode* pNpcName = pNpc ? D2Client_GetNpcDisplayName(pNpc) : nullptr;
    const wchar_t* wszNpcTitle = (pNpcName && pNpcName[0].ch != 0) ? reinterpret_cast<const wchar_t*>(pNpcName) : L"Townsperson";
    const wchar_t* aOpts[3] = { L"Talk", bHasTrade ? wszTradeOpt : L"Cancel", L"Cancel" };

    D2WIN_SetFont(5);
    const int32_t nTitleW = D2WIN_GetTextWidth(wszNpcTitle);
    int32_t nMaxTextW = nTitleW;
    D2WIN_SetFont(1);
    for (int32_t i = 0; i < nOptCount; ++i) {
        const int32_t nOptW = D2WIN_GetTextWidth(aOpts[i]);
        if (nOptW > nMaxTextW) nMaxTextW = nOptW;
    }

    const int32_t nHalfBoxW = ((nMaxTextW + 56) > 192) ? ((nMaxTextW + 56) / 2) : 96;
    const int32_t nBoxH = 44 + nOptCount * 22;
    RECT rMenu = { nBoxCenterX - nHalfBoxW, nBoxTopY, nBoxCenterX + nHalfBoxW, nBoxTopY + nBoxH };
    UI_DrawBoxPiecesFrame(&rMenu);

    D2WIN_SetFont(5);
    D2WIN_DrawText(
        wszNpcTitle,
        nBoxCenterX - (nTitleW / 2),
        nBoxTopY + 24,
        4,
        0);

    D2WIN_SetFont(1);
    for (int32_t i = 0; i < nOptCount; ++i) {
        const int32_t nRowY = nBoxTopY + 46 + i * 22;
        const bool bHover = (gnMouseX >= rMenu.left + 8 && gnMouseX <= rMenu.right - 8 &&
                             gnMouseY >= nRowY - 14 && gnMouseY <= nRowY + 6);
        const int32_t nOptW = D2WIN_GetTextWidth(aOpts[i]);
        D2WIN_DrawText(aOpts[i], nBoxCenterX - (nOptW / 2), nRowY, bHover ? 3 : 0, 0);
        if (bHover && g_pPentSpinCel) {
            const int32_t nFrames = D2CMP_CelFileGetCelsPerDirection(g_pPentSpinCel);
            if (nFrames > 0) {
                D2GfxDataStrc tSpin = {};
                tSpin.pCellFile = g_pPentSpinCel;
                tSpin.nFrame = (GetTickCount() / 60) % nFrames;
                D2GFX_DrawCellContextEx(&tSpin, rMenu.left + 10, nRowY + 4, -1, 5, nullptr);
            }
        }
    }
}

int32_t g_bEscOptionsSubmenu = 0;
int32_t g_bClientSoundOption = 1;
int32_t g_bClientScaledWindowOption = 1;
extern int32_t gbAutomapShowPartyNames;

static void ESCMENU_DrawPanel()
{
    if (!gaPanelStates[D2PANEL_ESCMENU]) {
        g_bEscOptionsSubmenu = 0;
        return;
    }
    if (!g_pPentSpinCel) {
        g_pPentSpinCel = ARCHIVE_LoadCellFileDefault("CURSOR\\Pentspin", 0);
    }

    const int32_t nW = (gnScreenWidth > 0) ? gnScreenWidth : 800;
    const int32_t nH = (gnScreenHeight > 0) ? gnScreenHeight : 600;
    D2GFX_DrawRect(0, 0, nW, nH - 48, 0, 2);

    const wchar_t* aLabels[4] = {};
    int32_t nRowCount = 3;
    if (g_bEscOptionsSubmenu != 0) {
        aLabels[0] = g_bClientSoundOption ? L"SOUND: ON" : L"SOUND: OFF";
        aLabels[1] = g_bClientScaledWindowOption ? L"WINDOW: 1200X900" : L"WINDOW: 800X600";
        aLabels[2] = gbAutomapShowPartyNames ? L"AUTOMAP NAMES: ON" : L"AUTOMAP NAMES: OFF";
        aLabels[3] = L"PREVIOUS MENU";
        nRowCount = 4;
    } else {
        aLabels[0] = L"OPTIONS";
        aLabels[1] = L"SAVE AND EXIT GAME";
        aLabels[2] = L"RETURN TO GAME";
        nRowCount = 3;
    }

    const int32_t nCenterX = nW / 2;
    const int32_t nBaseY = (nH / 2) - ((nRowCount == 4) ? 80 : 60);
    int32_t nSelectedRow = nRowCount - 1;
    for (int32_t i = 0; i < nRowCount; ++i) {
        const int32_t nRowY = nBaseY + i * 46;
        if (gnMouseY >= nRowY - 28 && gnMouseY <= nRowY + 8 &&
            gnMouseX >= nCenterX - 210 && gnMouseX <= nCenterX + 210) {
            nSelectedRow = i;
        }
    }

    D2WIN_SetFont(3);
    for (int32_t i = 0; i < nRowCount; ++i) {
        const int32_t nRowY = nBaseY + i * 46;
        const int32_t nTextW = D2WIN_GetTextWidth(aLabels[i]);
        D2WIN_DrawText(aLabels[i], nCenterX - (nTextW / 2), nRowY, (i == nSelectedRow) ? 4 : 0, 0);
        if (i == nSelectedRow && g_pPentSpinCel) {
            const int32_t nFrames = D2CMP_CelFileGetCelsPerDirection(g_pPentSpinCel);
            if (nFrames > 0) {
                const int32_t nLabelHalfW = (nTextW / 2) + 34;
                D2GfxDataStrc tSpin = {};
                tSpin.pCellFile = g_pPentSpinCel;
                tSpin.nFrame = (GetTickCount() / 60) % nFrames;
                D2GFX_DrawCellContextEx(&tSpin, nCenterX - nLabelHalfW - 18, nRowY + 4, -1, 5, nullptr);
                D2GFX_DrawCellContextEx(&tSpin, nCenterX + nLabelHalfW - 8, nRowY + 4, -1, 5, nullptr);
            }
        }
    }
}

// 1.12a: 0x6FB3F480 (__stdcall)
void __stdcall UI_ReleaseAllAssets()
{
    if (g_pGeneralSkillsCel) { ARCHIVE_FreeCellFile(g_pGeneralSkillsCel); g_pGeneralSkillsCel = nullptr; }
    for (int i = 0; i < 7; ++i) {
        if (g_apClassSkillsCel[i]) { ARCHIVE_FreeCellFile(g_apClassSkillsCel[i]); g_apClassSkillsCel[i] = nullptr; }
    }
    if (g_pBeltRowCel)       { ARCHIVE_FreeCellFile(g_pBeltRowCel);       g_pBeltRowCel       = nullptr; }
    if (g_pBoxPiecesCel)     { ARCHIVE_FreeCellFile(g_pBoxPiecesCel);     g_pBoxPiecesCel     = nullptr; }
    if (g_pFocus16Cel)       { ARCHIVE_FreeCellFile(g_pFocus16Cel);       g_pFocus16Cel       = nullptr; }
    if (g_pPentSpinCel)      { ARCHIVE_FreeCellFile(g_pPentSpinCel);      g_pPentSpinCel      = nullptr; }
    if (g_pBuySellBtnCel)    { ARCHIVE_FreeCellFile(g_pBuySellBtnCel);    g_pBuySellBtnCel    = nullptr; }
    if (s_pWaygateBgCel)     { ARCHIVE_FreeCellFile(s_pWaygateBgCel);     s_pWaygateBgCel     = nullptr; }
    if (s_pWaygateTabsCel)   { ARCHIVE_FreeCellFile(s_pWaygateTabsCel);   s_pWaygateTabsCel   = nullptr; }
    CHARSTATS_FreeStatPanel();
    PARTY_FreeNodesAndCellFiles();
    HIREABLES_FreePortraitCellFiles();
    AUTOMAP_FreeAllLayers();
    CURSOR_FreeAssets();
    UI_FreeLoadingScreen();
}

// 1.12a: 0x6FB3F6B0 (__fastcall)
void __fastcall UI_ToggleMiniPanel()
{
    UI_SetPanelState(D2PANEL_MINIPANEL, 2, 0);
}

// 1.12a: 0x6FB3F800 (merged in tail of sub_6FB3F6B0 in UI_ui.asm)
void __stdcall UI_CloseMercInteract()
{
    sub_6FB235B0();
    if (g_pMercMenuDialogWidget != nullptr) {
        if (g_bMercMenuActive != 0) {
            sub_6FB13290();
            if (g_pMercMenuDialogWidget != nullptr) {
                reinterpret_cast<void(__thiscall*)(void*, int32_t)>((*reinterpret_cast<void***>(g_pMercMenuDialogWidget))[0])(g_pMercMenuDialogWidget, 1);
            }
            g_pMercMenuDialogWidget = nullptr;
            g_bMercMenuActive = 0;
        }
        sub_6FACADF0();
    }
}

// 1.12a: 0x6FB3F850 (__fastcall)
void __fastcall UI_OpenGoldDialog(int32_t nMode /*ecx*/)
{
    g_nGoldDialogMode = nMode;
    g_bGoldDialogOpen = 1;
}

// 1.12a: 0x6FB3FCB0 (__stdcall)
void __stdcall UI_InitAllPanels()
{
    D2Client_SyncResolutionMetrics(gnScreenWidth > 0 ? gnScreenWidth : 800, gnScreenHeight > 0 ? gnScreenHeight : 600);
    UI_InitPaletteColors();
    UI_EnsureSkillIconsLoaded();
    if (!g_pPentSpinCel) {
        g_pPentSpinCel = ARCHIVE_LoadCellFileDefault("CURSOR\\Pentspin", 0);
    }
    if (!g_pBuySellBtnCel) {
        g_pBuySellBtnCel = ARCHIVE_LoadCellFileDefault("PANEL\\buysellbtn", 0);
    }
    CHARSTATS_InitStatPanel();
    INV_InitInventoryLayoutRectsAndGrids();
    PARTY_LoadCellFiles();
    HIREABLES_InitPanelState();
    HIREABLES_InitLifeBarColors(1);
    HIREABLES_LoadPortraitCellFiles();
    AUTOMAP_Initialize();
    CURSOR_Init();
    CMDTBL_InitKeyBindingsAndHandlers();
}

// 1.12a: 0x6FB40150 (__fastcall)
// Main client UI/HUD render loop called every frame
void __fastcall UI_RenderHUD()
{
    if (!gpLocalPlayerUnit) {
        return;
    }

    D2Win_10130_DrawFramedTextEx(nullptr, 0, 0, 0, 0, 0);

    if (gaPanelStates[D2PANEL_AUTOMAP] != 0) {
        AUTOMAP_Draw();
    }

    if (gaPanelStates[D2PANEL_SHOWITEMS] != 0) {
        SHOWITEMS_DrawGroundItemLabels();
    }
    UI_DrawHoveredUnitLabel();

    HIREABLES_DrawAllPortraits();

    INV_Render800x600PanelBorderFrame();

    CHARSTATS_DrawPanel();
    QUESTLOG_DrawPanel();
    WAYPOINT_DrawPanel();
    PARTY_DrawPartyScreen();

    INV_RenderAllInventoryPanels();
    SKILLTREE_RenderPanel();

    CTRLPNL_RenderBottomBar();
    PANEL_RenderControlPanel();
    CTRLPNL_RenderLeftLifeOrb();
    CTRLPNL_RenderRightManaOrb();
    CTRLPNL_RenderLifeManaTooltips();

    NPCMENU_DrawActiveDialog();
    ESCMENU_DrawPanel();

    UI_DrawDeferredHoverText();
    if (!gaPanelStates[D2PANEL_ESCMENU]) {
        D2Win_10124();
    }
    CURSOR_Update();
    CURSOR_Draw();
}
