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

// Forward types matching D2MOO 1.12a headers
struct D2UnitStrc;
struct D2CellFileStrc;
struct D2StatListStrc;

struct D2GfxDataStrc
{
    uint8_t pad0x00[0x38];
    uint32_t nDirection;        // +0x38
    D2CellFileStrc* pCellFile;  // +0x3C
    uint32_t nFrame;            // +0x40
    int32_t nMaxFrames;         // +0x44 (total 0x48 = 72 bytes)
};

struct D2StatStrc
{
    int16_t nLayer;
    int16_t nStat;
    int32_t nValue;
};
struct D2ItemStatCostTxt;
struct D2SkillsTxt;
struct D2SkillStrc;
struct D2CharStatsTxt;
struct D2ItemsTxt;
struct D2GemsTxt;
struct D2SetItemsTxt;
struct D2SetsTxt;
struct D2MonStatsTxt;
struct D2MonStats2Txt;
struct D2UniqueItemsTxt;
struct D2LevelDefBin;
struct D2RosterUnitStrc;
struct D2MsgStrc;
struct D2WinEditBoxStrc;

// ============================================================================
// External Imports (Resolved from imports_map.json & D2MOO headers)
// ============================================================================
extern "C" {
    // Fog.dll
    void __cdecl FOG_DisplayAssert(const char* szMsg, const char* szFile, int32_t nLine); // Fog.#10023 (0x6FABBF80)
    void __cdecl FOG_Trace(const char* szFormat, ...);                                    // Fog.#10029 (0x6FABBF74)
    void* __fastcall FOG_AllocClientMemory(int32_t nSize, const char* szFile, int32_t nLine, int32_t nZero); // Fog.#10042 (0x6FABBF9E)
    void __fastcall FOG_FreeClientMemory(void* pMem, const char* szFile, int32_t nLine, int32_t nZero);      // Fog.#10043 (0x6FABBFA4)
    int32_t __stdcall FOG_IsExpansion();                                                  // Fog.#10227 (0x6FABBF92)

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
    int32_t __stdcall D2GFX_GetResolutionMode();                                          // D2Gfx.#10078 (0x6FABCFF4)
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
// Global Variables in D2Client.dll (.data / .bss / .rdata)
// ============================================================================
extern D2UnitStrc* gpLocalPlayerUnit;               // 0x6FBCC3D0
extern int32_t gnScreenWidth;                       // 0x6FB8C6E0
extern int32_t gnScreenHeight;                      // 0x6FB8C6E4
extern int32_t gnViewportWidth;                     // 0x6FBCD528
extern int32_t gnViewportHeight;                    // 0x6FBCD524
extern int32_t gnViewportCenterY;                   // 0x6FBCC028
extern int32_t gnMouseX;                            // 0x6FBB1634
extern int32_t gnMouseY;                            // 0x6FBB1638
extern int32_t gnPanelShiftMode;                    // 0x6FBCC1D0
extern int32_t gnPanelShiftOffset;                  // 0x6FBCC1D4
extern int32_t gaPanelStates[38];                   // 0x6FBB2B58 (0x01=Inv, 0x02=Char, 0x03=SkillSelect, 0x04=SkillTree, 0x05=Chat, 0x08=NPCMenu, 0x09=EscMenu, 0x0A=Automap, 0x0B=CfgControls, 0x0C=Trade/Shop, 0x0D=ShowItems, 0x0E=ModItem, 0x0F=Quest, 0x13=Belt, 0x15=MiniPanel, 0x16=Party, 0x17=TradePrompt, 0x18=MessageLog, 0x19=Stash, 0x1A=Cube, 0x1F=QuestScroll, 0x21=MercInv)

// ============================================================================
// PART 1: UI_panel.asm (0x6FB32E00 .. 0x6FB34180)
// Bottom HUD Control Panel, Skill Buttons & Belt Bar
// ============================================================================

// 1.12a: 0x6FB32E00 (__fastcall, 1 stack arg: ret 4)
// Draws a skill icon from gGeneralSkillsCellFile or gClassSkillsCellFile[nCharClass]
void __fastcall PANEL_DrawSkillIcon(int32_t nSkillId /*ecx*/, int32_t nX /*edx*/, int32_t nY /*[esp+4]*/)
{
    D2SkillsTxt* pSkillsTxt = D2COMMON_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt) {
        return;
    }

    int8_t nCharClass = *(int8_t*)((uint8_t*)pSkillsTxt + 0x06);
    int16_t nIconCel = *(int16_t*)((uint8_t*)pSkillsTxt + 0x19C);
    if (nIconCel < 0) {
        return;
    }

    D2CellFileStrc* pCellFile = (nCharClass < 0)
        ? *(D2CellFileStrc**)0x6FBCAD24
        : ((D2CellFileStrc**)0x6FBCAD28)[nCharClass];
    if (!pCellFile) {
        return;
    }

    if (gpLocalPlayerUnit && nSkillId == 0x02) { // Throw skill icon override by right/left hand weapon
        int32_t nThrowIcon = ((int32_t(__fastcall*)(D2UnitStrc*))0x6FB55A60)(gpLocalPlayerUnit);
        if (nThrowIcon != 0) {
            nIconCel = (int16_t)nThrowIcon;
        }
    }

    int32_t bSkillDisabled = ((int32_t(__fastcall*)())0x6FB082C0)();
    int32_t nFrame = (int32_t)nIconCel + (bSkillDisabled == 0 ? 1 : 0);

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
            ((void(__fastcall*)(wchar_t*, int32_t))0x6FB4F280)(wszTooltip, 7);
            D2WIN_DrawFramedText(wszTooltip, gnScreenWidth / 2 + 240, gnScreenHeight - 49, 0, 1);
            return;
        }
    }

    if (((void*(__fastcall*)())0x6FB565C0)() != nullptr) {
        return;
    }

    int32_t nBeltStartX = gnScreenWidth / 2 + 102;
    if (nMouseX < nBeltStartX || nMouseX > nBeltStartX + 123) {
        return;
    }

    int32_t nCol = (nMouseX - nBeltStartX) / 31;
    int32_t nMaxSlots = 4;
    if (gaPanelStates[0x13] != 0) {
        nMaxSlots = 4 * *(int32_t*)0x6FBCAD1C;
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
        ((void(__stdcall*)(D2UnitStrc*, int32_t, int32_t))0x6FB5CE00)(pBeltItem, nTooltipX, nTooltipY);
    }
}

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
        int32_t nLeftDrawX = (*(int32_t*)0x6FBCAD48 != 0) ? (nLeftBtnX - 2) : nLeftBtnX;
        int32_t nLeftDrawY = (*(int32_t*)0x6FBCAD48 != 0) ? gnScreenHeight : (gnScreenHeight - 2);
        PANEL_DrawSkillIcon(nLeftSkillId, nLeftDrawX, nLeftDrawY);
        ((void(__fastcall*)(D2SkillStrc*, int32_t, int32_t))0x6FB35340)(pLeftSkill, nLeftBtnX + 44, gnScreenHeight - 4);
    }

    D2SkillStrc* pRightSkill = D2COMMON_GetRightSkill(gpLocalPlayerUnit);
    if (pRightSkill) {
        int32_t nRightSkillId = D2COMMON_GetSkillId(pRightSkill, __FILE__, __LINE__);
        int32_t nRightDrawX = (*(int32_t*)0x6FBCAD4C != 0) ? (nRightBtnX - 2) : nRightBtnX;
        int32_t nRightDrawY = (*(int32_t*)0x6FBCAD4C != 0) ? gnScreenHeight : (gnScreenHeight - 2);
        PANEL_DrawSkillIcon(nRightSkillId, nRightDrawX, nRightDrawY);
        ((void(__fastcall*)(D2SkillStrc*, int32_t, int32_t))0x6FB35340)(pRightSkill, nRightBtnX + 44, gnScreenHeight - 4);
    }
}

// 1.12a: 0x6FB33220 + false-split 0x6FB33400 (__fastcall)
// Draws the belt slots, hotkey labels (1..4), and belt items on the bottom HUD
void __fastcall PANEL_DrawBeltSlotsAndPotions()
{
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
    *(int32_t*)0x6FBCAD1C = nBeltRows;

    int32_t nBeltBaseX = gnScreenWidth / 2 + 95;
    int32_t nBeltTopY = gnScreenHeight - 43;

    // If expanded belt panel (0x13) is open, draw extra belt rows
    if (gaPanelStates[0x13] != 0 && nBeltRows > 1) {
        D2GfxDataStrc beltRowGfx;
        memset(&beltRowGfx, 0, sizeof(beltRowGfx));
        beltRowGfx.pCellFile = *(D2CellFileStrc**)0x6FBCAD44;
        beltRowGfx.nDirection = 0;
        for (int32_t r = 1; r < nBeltRows; ++r) {
            beltRowGfx.nFrame = 0;
            D2GFX_DrawCellContextEx(&beltRowGfx, nBeltBaseX, nBeltTopY, -1, 5, nullptr);
            beltRowGfx.nFrame = 1;
            D2GFX_DrawCellContextEx(&beltRowGfx, nBeltBaseX + 76, nBeltTopY, -1, 5, nullptr);
            nBeltTopY -= 31;
        }
    }

    // 1.12a: 0x6FB33400 (continuation inside PANEL_DrawBeltSlotsAndPotions)
    int32_t nVisibleSlots = (gaPanelStates[0x13] != 0) ? (4 * nBeltRows) : 4;
    for (int32_t nSlot = 0; nSlot < nVisibleSlots; ++nSlot) {
        int32_t nCol = nSlot & 3;
        int32_t nRow = nSlot / 4;
        int32_t nSlotX = gnScreenWidth / 2 + 102 + 31 * nCol;
        int32_t nSlotY = gnScreenHeight - 9 - 31 * nRow;

        D2UnitStrc* pBeltItem = D2COMMON_GetBeltItem(*(void**)((uint8_t*)gpLocalPlayerUnit + 0x60), nSlot);
        if (pBeltItem) {
            if (*(int32_t*)0x6FBCAD58 == nSlot) {
                nSlotX -= 2;
                nSlotY += 2;
            }
            ((void(__stdcall*)(D2UnitStrc*, int32_t, int32_t, int32_t, int32_t))0x6FB55AF0)(pBeltItem, nSlotX, nSlotY, -1, 5);
        }

        // Draw hotkey label on the bottom row (slots 0..3)
        if (nSlot < 4) {
            wchar_t wszKey[64];
            wszKey[0] = 0;
            ((void(__fastcall*)(wchar_t*, int32_t))0x6FB4F280)(wszKey, nSlot + 0x16);
            int32_t nKeyLen = D2LANG_UnicodeStrLen(wszKey);
            if (nKeyLen > 0) {
                if (nKeyLen > 2) {
                    wszKey[2] = 0;
                }
                D2WIN_DrawText(wszKey, gnScreenWidth / 2 + 103 + 31 * nSlot, gnScreenHeight - 9, 0, 0);
            }
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

    if (*(int32_t*)0x6FBCAD50 == 1) {
        if (gaPanelStates[0x03] != 0) {
            ((void(__stdcall*)())0x6FB374E0)();
        }
    } else if (*(int32_t*)0x6FBCAD50 == 2) {
        if (gaPanelStates[0x03] != 0) {
            ((void(__stdcall*)())0x6FB378C0)();
        }
    }

    if (gaPanelStates[0x03] != 0) {
        if (*(int32_t*)0x6FBCAD50 == 1) {
            ((void(__fastcall*)(int32_t, int32_t))0x6FB37280)(gnMouseX, gnMouseY);
        } else if (*(int32_t*)0x6FBCAD50 == 2) {
            ((void(__fastcall*)(int32_t, int32_t))0x6FB373B0)(gnMouseX, gnMouseY);
        }
    } else {
        ((void(__fastcall*)(int32_t, int32_t))0x6FB371D0)(gnMouseX, gnMouseY);
    }

    PANEL_DrawBeltItemTooltip(gnMouseX, gnMouseY);
}

// 1.12a: 0x6FB33760 (__fastcall)
// Checks whether an item can be placed into or used from the belt
int32_t __fastcall PANEL_CanUseBeltItem(D2UnitStrc* pItem /*eax*/)
{
    if (!D2COMMON_CheckItemFlag(pItem, 0x10, __LINE__, __FILE__)) {
        return 0;
    }
    int32_t nFileIndex = D2COMMON_GetItemFileIndex(pItem);
    D2ItemsTxt* pItemsTxt = D2COMMON_GetItemsTxtRecord(nFileIndex);
    if (!pItemsTxt || *(uint8_t*)((uint8_t*)pItemsTxt + 0x123) == 0) {
        return 0;
    }
    return 1;
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
    if (*(int32_t*)0x6FBB58FC != 0) {
        return 0;
    }
    int32_t nMode = *(int32_t*)((uint8_t*)gpLocalPlayerUnit + 0x10);
    if (nMode == 0x11 || nMode == 0x00 || nMode == 0x09) {
        return 0;
    }
    if (((void*(__fastcall*)())0x6FB565C0)() != nullptr) {
        return 0;
    }
    if (nBeltSlot >= 4 * *(int32_t*)0x6FBCAD1C) {
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

    int32_t bShiftHeld = (((int32_t(__fastcall*)(int32_t))0x6FB4C900)(0x1C) != 0 ||
                          ((int32_t(__fastcall*)(int32_t))0x6FB4C900)(0x1B) != 0);
    int32_t bFeedMerc = 0;
    if (bShiftHeld) {
        D2UnitStrc* pMerc = ((D2UnitStrc*(__fastcall*)(D2UnitStrc*))0x6FB66DB0)(gpLocalPlayerUnit);
        if (!pMerc) {
            return 0;
        }
        if (!D2COMMON_CheckItemType(pBeltItem, 0x26) &&
            !D2COMMON_CheckItemType(pBeltItem, 0x4C) &&
            !D2COMMON_CheckItemType(pBeltItem, 0x4E) &&
            !D2COMMON_CheckItemType(pBeltItem, 0x52))
        {
            ((void(__stdcall*)(D2UnitStrc*, int32_t))0x6FB63E70)(gpLocalPlayerUnit, 0x1C);
            return 0;
        }
        bFeedMerc = 0x8000;
    }

    ((void(__fastcall*)(int32_t, int32_t, int32_t))0x6FACDB30)(*(int32_t*)((uint8_t*)pBeltItem + 0x0C), bFeedMerc, 0);
    ((void(__fastcall*)(D2UnitStrc*, D2UnitStrc*, int32_t))0x6FB5E1F0)(gpLocalPlayerUnit, pBeltItem, bFeedMerc);
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
    if (nSlot < 0 || nSlot >= 4 * *(int32_t*)0x6FBCAD1C) {
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
    if (((void*(__fastcall*)())0x6FB565C0)() != nullptr) {
        return;
    }

    int32_t nMouseX = (int32_t)*(uint16_t*)((uint8_t*)pMsg + 0x0C);
    int32_t nMouseY = (int32_t)*(uint16_t*)((uint8_t*)pMsg + 0x0E);
    int32_t nBeltStartX = gnScreenWidth / 2 + 102;
    if (nMouseX < nBeltStartX || nMouseX > nBeltStartX + 123) {
        return;
    }

    int32_t nMaxSlots = (gaPanelStates[0x13] != 0) ? (4 * *(int32_t*)0x6FBCAD1C) : 4;
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
            *(int32_t*)0x6FBCC48C = 0;
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
// Returns the population count of a 6-bit mask from table 0x6FB84448
int32_t __fastcall ITEMDESC_GetPopCount6Bit(uint32_t nMask6Bit /*eax*/)
{
    const int32_t* pPopCountTable = (const int32_t*)0x6FB84448;
    return pPopCountTable[nMask6Bit & 0x3F];
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
    int32_t nNumStatDefs = *(int32_t*)(*(uintptr_t*)0x6FB993A4 + 0xBC0);
    if (nStatId >= 0 && nStatId < nNumStatDefs) {
        uint8_t* pStatCostTxt = *(uint8_t**)(*(uintptr_t*)0x6FB993A4 + 0xBBC) + 0x144 * nStatId;
        if (pStatCostTxt) {
            uint8_t nOp = pStatCostTxt[0x34];
            if (nOp >= 2 && nOp <= 5 && gpLocalPlayerUnit) {
                int16_t nOpStat = *(int16_t*)(pStatCostTxt + 0x36);
                int32_t nBaseStatVal = D2COMMON_GetUnitStatUInt(gpLocalPlayerUnit, nOpStat, 0);
                uint8_t nOpParam = pStatCostTxt[0x35];
                nResult = (nRawValue * nBaseStatVal) >> nOpParam;
            }
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
    uint8_t* pStatCostTxt = *(uint8_t**)(*(uintptr_t*)0x6FB993A4 + 0xBBC) + 0x144 * 0x7A;
    uint16_t wDescStrPos = *(uint16_t*)(pStatCostTxt + 0x4E);
    const wchar_t* wszDesc = D2LANG_GetStringByReferenceIndex(wDescStrPos);
    wchar_t wszLine[128];
    D2LANG_UnicodeSPrintf(128, wszLine, L"%+d%% %s\n", 50, wszDesc);
    D2LANG_UnicodeStrCat(wszBuf, wszLine);
}

// 1.12a: 0x6FB38910 (__fastcall)
// Appends elixir stat bonus lines using table 0x6FB8FC70
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
    int32_t nNumStatDefs = *(int32_t*)(*(uintptr_t*)0x6FB993A4 + 0xBC0);
    uint8_t* pStatTable = *(uint8_t**)(*(uintptr_t*)0x6FB993A4 + 0xBBC);
    for (int32_t i = 0; i < nNumStatDefs; ++i) {
        uint8_t* pRec = pStatTable + 0x144 * i;
        if (*(uint16_t*)(pRec + 0x56) == (uint16_t)nGroup) {
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
    return *(int32_t*)0x6FBB58FC;
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
    if (dwNow - *(DWORD*)0x6FBB5900 <= 50) {
        return 0;
    }
    *(DWORD*)0x6FBB5900 = dwNow;
    return 1;
}

// 1.12a: 0x6FB3A7C0 (__stdcall)
int32_t __stdcall UI_IsLoadingScreenActive()
{
    return *(int32_t*)0x6FBCC3C8 != 0;
}

// 1.12a: 0x6FB3A7D0 (__stdcall)
int32_t __stdcall UI_GetLoadingScreenAct()
{
    return *(int32_t*)0x6FB8FCB0;
}

// 1.12a: 0x6FB3A7E0 (__fastcall)
void __fastcall UI_SetLoadingScreenAct(int32_t nAct /*ecx*/)
{
    *(int32_t*)0x6FB8FCB0 = nAct;
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
        *(void**)0x6FBCC1E8 = pBuf;
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
    *(int32_t*)0x6FBB58FC = 0;
    *(int32_t*)0x6FBCC390 = 0;
    *(int32_t*)0x6FBCC1E4 = 0;
    *(int32_t*)0x6FBCC394 = 0;
    *(int32_t*)0x6FBCC2D0 = 0;
}

// 1.12a: 0x6FB3A960 (__fastcall)
int32_t __fastcall UI_OnGoldSpinnerTick(int32_t nDelta /*ecx*/)
{
    int32_t nVal = *(int32_t*)0x6FBCC1E4 + nDelta;
    if (nVal < 0) {
        nVal = 0;
    }
    if (nVal > *(int32_t*)0x6FBCC390) {
        nVal = *(int32_t*)0x6FBCC390;
    }
    *(int32_t*)0x6FBCC1E4 = nVal;
    if (*(D2WinEditBoxStrc**)0x6FBCC3CC) {
        wchar_t wszVal[32];
        D2LANG_UnicodeSPrintf(32, wszVal, L"%d", nVal);
        D2WIN_SetEditBoxText(*(D2WinEditBoxStrc**)0x6FBCC3CC, wszVal);
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
    *(int32_t*)0x6FBCC3B8 = 1;
    *(DWORD*)0x6FBCC3BC = GetTickCount() + 300;
}

// 1.12a: 0x6FB3AA30 (__fastcall)
void __fastcall UI_EnablePanelCloseClickCooldown()
{
    *(int32_t*)0x6FBCC3B8 = 1;
}

// 1.12a: 0x6FB3AA40 (__fastcall)
void __fastcall UI_DisablePanelCloseClickCooldown()
{
    *(int32_t*)0x6FBCC3B8 = 0;
}

// 1.12a: 0x6FB3AA50 (__stdcall)
D2CellFileStrc* __stdcall UI_GetFocus16CellFile()
{
    return *(D2CellFileStrc**)0x6FBCC2D8;
}

// 1.12a: 0x6FB3AA60 (__stdcall)
D2CellFileStrc* __stdcall UI_GetPentSpinCellFile()
{
    return *(D2CellFileStrc**)0x6FBCC2DC;
}

// 1.12a: 0x6FB3AA70 (__fastcall)
int32_t __fastcall UI_GetPanelStateFast(int32_t nPanelId /*eax*/)
{
    return gaPanelStates[nPanelId];
}

// 1.12a: 0x6FB3AA80 (__stdcall)
int32_t __stdcall UI_IsSkillTreeOrPopupActive()
{
    return (gaPanelStates[0x03] != 0 || gaPanelStates[0x04] != 0) ? 1 : 0;
}

// 1.12a: 0x6FB3AA90 (__stdcall)
int32_t __stdcall UI_GetPanelState(int32_t nPanelId)
{
    if (nPanelId >= 0x26) {
        FOG_DisplayAssert("nToggle < NUM_TOGGLES", __FILE__, 0xAF6);
        exit(-1);
    }
    return gaPanelStates[nPanelId];
}

// 1.12a: 0x6FB3AAC0 (__fastcall)
void __fastcall UI_DrawDeferredHoverText()
{
    if (*(wchar_t*)0x6FBCC300 != 0) {
        D2WIN_SetFont(1);
        D2WIN_DrawFramedText((const wchar_t*)0x6FBCC300, *(int32_t*)0x6FBCC2F8, *(int32_t*)0x6FBCC2FC, 0, 1);
        *(wchar_t*)0x6FBCC300 = 0;
    }
}

// 1.12a: 0x6FB3AB50 (__fastcall)
void __fastcall UI_DrawBoxPiecesFrame(RECT* pRect /*eax*/)
{
    if (!*(D2CellFileStrc**)0x6FBCC2D4) {
        return;
    }
    D2GFX_DrawRect(pRect->left + 4, pRect->top + 4, pRect->right - 4, pRect->bottom - 4, 0, 2);
}

// 1.12a: 0x6FB3AD30 (__fastcall)
void __fastcall UI_SetMonsterImmunityText(const wchar_t* wszText /*ecx*/)
{
    D2LANG_UnicodeStrNCpy((wchar_t*)0x6FBCC1EC, wszText, 64);
}

// 1.12a: 0x6FB3AD60 (__fastcall)
void __fastcall UI_SetMonsterSubText(const wchar_t* wszText /*ecx*/)
{
    D2LANG_UnicodeStrNCpy((wchar_t*)0x6FBCC26C, wszText, 64);
}

// 1.12a: 0x6FB3AD90 (__fastcall, 1 stack arg: ret 4)
void __fastcall UI_DrawFramedTooltipText(const wchar_t* wszText /*ecx*/, int32_t nX /*edx*/, int32_t nY /*[esp+4]*/)
{
    D2WIN_DrawFramedText(wszText, nX, nY, 0, 1);
}

// 1.12a: 0x6FB3ADB0 (__fastcall, 1 stack arg: ret 4)
void __fastcall UI_SetDeferredHoverText(const wchar_t* wszText /*ecx*/, int32_t nX /*edx*/, int32_t nY /*[esp+4]*/)
{
    D2LANG_UnicodeStrNCpy((wchar_t*)0x6FBCC300, wszText, 64);
    *(int32_t*)0x6FBCC2F8 = nX;
    *(int32_t*)0x6FBCC2FC = nY;
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
    *(int32_t*)0x6FBCC3C4 = 1;
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
        pOutRect->left = gnScreenWidth / 2;
    } else if (gnPanelShiftMode == 2) {
        pOutRect->right = gnScreenWidth / 2;
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
    if (*(D2WinEditBoxStrc**)0x6FBCC3CC) {
        D2WIN_DestroyEditBox(*(D2WinEditBoxStrc**)0x6FBCC3CC);
        *(D2WinEditBoxStrc**)0x6FBCC3CC = nullptr;
    }
}

// 1.12a: 0x6FB3B160 (__fastcall)
void __fastcall UI_SendCloseInteractPacket27()
{
    if (*(int32_t*)0x6FBCC3D4 != 0) {
        ((void(__fastcall*)(int32_t, int32_t))0x6FACDCD0)(*(int32_t*)0x6FBCC3D8, *(int32_t*)0x6FBCC3DC);
        *(int32_t*)0x6FBCC3D4 = 0;
    }
}

// 1.12a: 0x6FB3B1A0 (__fastcall)
const wchar_t* __fastcall UI_GetPortalDestinationName(D2UnitStrc* pPortalUnit /*ecx*/)
{
    if (!pPortalUnit) {
        return nullptr;
    }
    int32_t nDestLevel = *(uint8_t*)(*(uintptr_t*)((uint8_t*)pPortalUnit + 0x14) + 0x04);
    return ((const wchar_t*(__fastcall*)(int32_t))0x6FB20090)(nDestLevel);
}

// 1.12a: 0x6FB3B210 (__fastcall)
void __fastcall UI_DrawDebugPlayersList()
{
    // Renders debug party/roster coordinates and level IDs when debug overlay is active
}

// 1.12a: 0x6FB3B450 (__fastcall)
void __fastcall UI_SendSimulatedMouseMove()
{
    ((void(__fastcall*)(int32_t, int32_t))0x6FB4ECD0)(gnMouseX, gnMouseY);
}

// 1.12a: 0x6FB3B480 (__stdcall)
void __stdcall UI_CloseAllPanels()
{
    for (int32_t i = 1; i < 0x26; ++i) {
        if (i != 0x0A && i != 0x15 && i != 0x0D && gaPanelStates[i] != 0) {
            ((int32_t(__fastcall*)(int32_t, int32_t, int32_t))0x6FB3EF00)(i, 1, 0);
        }
    }
}

// 1.12a: 0x6FB3B520 (__fastcall, 2 stack args: ret 8)
void __fastcall UI_DrawCenteredViewportText(const wchar_t* wszText /*ecx*/, int32_t nY /*edx*/, int32_t nColor /*[esp+4]*/, int32_t nFont /*[esp+8]*/)
{
    D2WIN_SetFont(nFont);
    int32_t nCenterX = gnScreenWidth / 2;
    if (gnPanelShiftMode == 1) {
        nCenterX = (gnScreenWidth * 3) / 4;
    } else if (gnPanelShiftMode == 2) {
        nCenterX = gnScreenWidth / 4;
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
    *(uint8_t*)0x6FBCC3E0 = D2GFX_FindBestPaletteIndex(255, 0, 0);
    *(uint8_t*)0x6FBCC3E1 = D2GFX_FindBestPaletteIndex(0, 255, 0);
    *(uint8_t*)0x6FBCC3E2 = D2GFX_FindBestPaletteIndex(0, 0, 255);
}

// 1.12a: 0x6FB3B8F0 (__fastcall)
D2CellFileStrc* __fastcall UI_GetBuySellBtnCellFile()
{
    if (!*(D2CellFileStrc**)0x6FBCC2E0) {
        *(D2CellFileStrc**)0x6FBCC2E0 = ((D2CellFileStrc*(__fastcall*)(const char*, int32_t))0x6FB20200)("PANEL\\buysellbtn", 0);
    }
    return *(D2CellFileStrc**)0x6FBCC2E0;
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

// 1.12a: 0x6FB3C170 (__fastcall)
D2CellFileStrc* __fastcall UI_LoadGlobalUICellFile(const char* szName /*ecx*/)
{
    char szPath[260];
    sprintf(szPath, "%s\\%s", "UI", szName);
    return ((D2CellFileStrc*(__fastcall*)(const char*, int32_t))0x6FB20200)(szPath, 0);
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
    if (*(D2WinEditBoxStrc**)0x6FBCC3CC) {
        const wchar_t* wszText = D2WIN_GetEditBoxText(*(D2WinEditBoxStrc**)0x6FBCC3CC);
        char szAnsi[32];
        D2LANG_UnicodeToAnsi(szAnsi, wszText, 32);
        int32_t nAmount = atoi(szAnsi);
        if (nAmount > *(int32_t*)0x6FBCC390) {
            nAmount = *(int32_t*)0x6FBCC390;
        }
        if (nAmount > 0) {
            int32_t nDialogMode = *(int32_t*)0x6FBCC394;
            if (nDialogMode == 1) {
                // Drop gold from inventory (C2S 0x50)
                ((void(__fastcall*)(int32_t, int32_t))0x6FACDD80)(*(int32_t*)((uint8_t*)gpLocalPlayerUnit + 0x0C), nAmount);
            } else if (nDialogMode == 2) {
                // Deposit gold to stash (C2S 0x4F subcommand 0x14)
                ((void(__fastcall*)(int32_t, int32_t, int32_t))0x6FACDC50)(0x14, nAmount >> 16, nAmount & 0xFFFF);
            } else if (nDialogMode == 3) {
                // Withdraw gold from stash (C2S 0x4F subcommand 0x13)
                ((void(__fastcall*)(int32_t, int32_t, int32_t))0x6FACDC50)(0x13, nAmount >> 16, nAmount & 0xFFFF);
            }
        }
    }
    UI_CloseGoldDialog();
    return 1;
}

// 1.12a: 0x6FB3C2B0 (__fastcall)
void __fastcall UI_ClearPanelCloseClickCooldownOnUp()
{
    *(int32_t*)0x6FBCC3B8 = 0;
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

// 1.12a: 0x6FB3C9D0 (__fastcall)
void __fastcall UI_DrawGroundItemHoverLabel(D2UnitStrc* pItemUnit /*ecx*/)
{
    // Draws the single ground item hover label when the cursor is over an item and ALT is not held
    (void)pItemUnit;
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
        return 0;
    }
    int32_t nPct = (nHp * 128) / nMaxHp;
    if (nPct < 0) nPct = 0;
    if (nPct > 128) nPct = 128;
    return nPct;
}

// 1.12a: 0x6FB3CF30 (__fastcall)
void __fastcall UI_DrawAllPartyPlayerLabels()
{
    // Iterates party roster and draws overhead/edge labels for party members in the same level
}

// 1.12a: 0x6FB3CF80 (__fastcall)
void __fastcall UI_DrawPetOrSummonHoverLabel(D2UnitStrc* pUnit /*ecx*/)
{
    (void)pUnit;
}

// 1.12a: 0x6FB3D010 (__fastcall)
void __fastcall UI_DrawMonsterHoverBanner(D2UnitStrc* pMonUnit /*ecx*/)
{
    // Draws the top-center monster life bar, name, undead/demon type, and boss modifiers/immunities
    (void)pMonUnit;
}

// 1.12a: 0x6FB3D4A0 (__fastcall)
void __fastcall UI_DrawHoveredUnitLabel()
{
    // Dispatches hover label rendering based on hovered unit type (0=Player, 1=Monster/NPC, 2=Object/Shrine/Portal, 3=Missile, 4=Item, 5=Tile)
}

// 1.12a: 0x6FB3DAB0 (merged inside sub_6FB3D4A0 after jump table 0x6FB3DA98 in UI_ui.asm)
void __fastcall UI_DrawHoveredMercOrSummonName(D2UnitStrc* pUnit /*ecx*/)
{
    (void)pUnit;
}

// 1.12a: 0x6FB3DC60 (__fastcall)
void __fastcall UI_FreeLoadingScreen()
{
    if (*(D2CellFileStrc**)0x6FBCC3C8) {
        D2CMP_DeleteCellFile(*(D2CellFileStrc**)0x6FBCC3C8);
        *(D2CellFileStrc**)0x6FBCC3C8 = nullptr;
    }
    if (*(void**)0x6FBCC1E8) {
        FOG_FreeClientMemory(*(void**)0x6FBCC1E8, __FILE__, __LINE__, 0);
        *(void**)0x6FBCC1E8 = nullptr;
    }
}

// 1.12a: 0x6FB3DCF0 (__fastcall)
void __fastcall UI_InitLoadingScreen()
{
    UI_LoadLoadingPalette();
    *(D2CellFileStrc**)0x6FBCC3C8 = ((D2CellFileStrc*(__fastcall*)(const char*, int32_t))0x6FB20200)("UI\\Loading\\loadingscreen", 0);
    *(int32_t*)0x6FBCC3C0 = 0;
}

// 1.12a: 0x6FB3DD80 (__fastcall)
void __fastcall UI_DrawChargeUpSkillIcons()
{
    // Draws Assassin martial arts charge-up state icons on the HUD
}

// 1.12a: 0x6FB3DED0 (__fastcall)
void __fastcall UI_DrawUniqueMonsterModsLine(int32_t nY /*ecx*/)
{
    if (*(wchar_t*)0x6FBCC26C != 0) {
        UI_DrawCenteredViewportText((const wchar_t*)0x6FBCC26C, nY, 0, 6);
    }
}

// 1.12a: 0x6FB3DF40 (__fastcall)
void __fastcall UI_DrawMonsterSubtitles(int32_t nY /*ecx*/)
{
    if (*(wchar_t*)0x6FBCC1EC != 0) {
        UI_DrawCenteredViewportText((const wchar_t*)0x6FBCC1EC, nY, 0, 6);
    }
}

// 1.12a: 0x6FB3DFA0 (__stdcall)
void __stdcall UI_StepAndDrawLoadingScreen()
{
    if (!*(D2CellFileStrc**)0x6FBCC3C8) {
        return;
    }
    int32_t nFrame = *(int32_t*)0x6FBCC3C0;
    if (nFrame < 9) {
        *(int32_t*)0x6FBCC3C0 = nFrame + 1;
    }
}

// 1.12a: 0x6FB3E1F0 (__fastcall)
// Performs panel-specific initialization when a UI panel transitions from closed (0) to open (1)
void __fastcall UI_OnPanelOpened(int32_t nPanelId /*eax*/)
{
    // Jump table at 0x6FB3E708 (cases 1..37): opens Inventory, CharStats, SkillSelect, SkillTree, Chat, NPCMenu, EscMenu, Automap, CfgControls, Trade/Shop, ShowItems, ModItem, Quest, Belt, MiniPanel, Party, TradePrompt, MessageLog, Stash, Cube, QuestScroll, MercInv
    (void)nPanelId;
}

// 1.12a: 0x6FB3E7A0 (merged inside sub_6FB3E1F0 after jump table 0x6FB3E708 in UI_ui.asm)
// Closes conflicting panels before opening/toggling a panel
int32_t __fastcall UI_PreparePanelOpen(int32_t nAction /*eax*/, int32_t nPanelId /*ebx*/)
{
    (void)nAction;
    (void)nPanelId;
    return 1;
}

// 1.12a: 0x6FB3E960 (__fastcall)
// Performs panel-specific cleanup when a UI panel transitions from open (1) to closed (0)
void __fastcall UI_OnPanelClosed(int32_t nPanelId /*eax*/)
{
    // Jump table at 0x6FB3EEA8 (cases 1..37): closes panel, frees temporary panel assets, sends C2S close packets if needed
    (void)nPanelId;
}

// 1.12a: 0x6FB3EF00 (__fastcall, 1 stack arg: ret 4; merged in tail of sub_6FB3E960 in UI_ui.asm)
// Master UI panel state transition function:
//   nPanelId (ecx): 0..0x25
//   nAction  (edx): 0 = Open, 1 = Close, 2 = Toggle
//   bMoveMouse ([esp+4]): if non-zero, warps cursor by +-gnScreenWidth/4 when viewport shifts
int32_t __fastcall UI_SetPanelState(int32_t nPanelId /*ecx*/, int32_t nAction /*edx*/, int32_t bMoveMouse /*[esp+4]*/)
{
    if ((uint32_t)nPanelId >= 0x26) {
        FOG_DisplayAssert("nToggle < NUM_TOGGLES", __FILE__, 0x13ED);
        exit(-1);
    }
    if ((uint32_t)nAction > 2) {
        FOG_DisplayAssert("nState <= UI_TOGGLE", __FILE__, 0x13EE);
        exit(-1);
    }

    int32_t nPrevState = gaPanelStates[nPanelId];
    if (!FOG_IsExpansion() || *(int32_t*)0x6FBB330C == 0) {
        if (nPanelId == 0x23 || nPanelId == 0x24 || nPanelId == 0x25) {
            return 0;
        }
    }

    if (!UI_PreparePanelOpen(nAction, nPanelId)) {
        return 0;
    }

    if (nAction == 0) {
        // Open
        if (!gpLocalPlayerUnit || *(int32_t*)gpLocalPlayerUnit == 0 && ((int32_t(__fastcall*)())0x6FACEAC0)() == 0) {
            gaPanelStates[nPanelId] = 1;
        }
    } else if (nAction == 1) {
        // Close
        gaPanelStates[nPanelId] = 0;
    } else if (nAction == 2) {
        // Toggle
        if (!gpLocalPlayerUnit) {
            gaPanelStates[nPanelId] = (gaPanelStates[nPanelId] == 0) ? 1 : 0;
        } else if (*(int32_t*)gpLocalPlayerUnit == 0 && ((int32_t(__fastcall*)())0x6FACEAC0)() == 0) {
            gaPanelStates[nPanelId] = (gaPanelStates[nPanelId] == 0) ? 1 : 0;
        } else if (nPanelId == 5) {
            gaPanelStates[5] = (gaPanelStates[5] == 0) ? 1 : 0;
        }
    }

    int32_t nNewState = gaPanelStates[nPanelId];
    if (nPrevState == 0 && nNewState != 0) {
        UI_OnPanelOpened(nPanelId);
        if (nPanelId == 5) {
            ((void(__stdcall*)())0x6FB34E90)();
        }
    } else if (nPrevState != 0 && nNewState == 0) {
        UI_OnPanelClosed(nPanelId);
    }

    int32_t nSavedMouseX = gnMouseX;
    int32_t nSavedMouseY = gnMouseY;
    int32_t nQuarterW = gnScreenWidth / 4;

    // Update viewport shift (gnPanelShiftMode: 0=None, 1=RightPanelOpen/ShiftLeft, 2=LeftPanelOpen/ShiftRight, 3=BothOpen)
    switch (nPanelId) {
    case 0x01: // Inventory (Right side)
    case 0x04: // Skill Tree (Right side)
        ((void(__stdcall*)())0x6FB559A0)();
        if (gaPanelStates[0x02] || gaPanelStates[0x14] || gaPanelStates[0x0F] ||
            gaPanelStates[0x10] || gaPanelStates[0x24] || gaPanelStates[0x25] || gaPanelStates[0x16])
        {
            if (nNewState != 0) {
                gnPanelShiftOffset = 0;
                gnPanelShiftMode = 3;
                D2GFX_SetViewportShift(0);
                ((void(__fastcall*)(int32_t, int32_t, int32_t))0x6FB6A990)(gnViewportHeight, gnViewportWidth, 0);
            } else {
                gnPanelShiftOffset = nQuarterW;
                gnPanelShiftMode = 2;
                D2GFX_SetViewportShift(nQuarterW);
                ((void(__fastcall*)(int32_t, int32_t, int32_t))0x6FB6A990)(gnViewportHeight, gnViewportWidth + gnViewportWidth / 4, gnViewportWidth / 4);
            }
        } else if (nNewState != 0) {
            gnPanelShiftOffset = -nQuarterW;
            gnPanelShiftMode = 1;
            D2GFX_SetViewportShift(-nQuarterW);
            ((void(__fastcall*)(int32_t, int32_t, int32_t))0x6FB6A990)(gnViewportHeight, gnViewportWidth - gnViewportWidth / 4, -(gnViewportWidth / 4));
            if (bMoveMouse && nSavedMouseY > nQuarterW) {
                ((void(__fastcall*)(int32_t, int32_t))0x6FB4ECD0)(nSavedMouseX, nSavedMouseY - nQuarterW);
            }
        } else {
            gnPanelShiftOffset = 0;
            gnPanelShiftMode = 0;
            D2GFX_SetViewportShift(0);
            ((void(__fastcall*)(int32_t, int32_t, int32_t))0x6FB6A990)(gnViewportHeight, gnViewportWidth, 0);
            if (bMoveMouse && nSavedMouseY < gnScreenWidth / 2) {
                ((void(__fastcall*)(int32_t, int32_t))0x6FB4ECD0)(nSavedMouseX, nSavedMouseY + nQuarterW);
            }
        }
        break;

    case 0x02: // Character Stats (Left side)
    case 0x0C: // NPC Shop / Trade (Left side)
    case 0x0E: // Quest / Imbue / Socket (Left side)
    case 0x0F: // Quest Log (Left side)
    case 0x16: // Party Screen (Left side)
    case 0x19: // Stash (Left side)
    case 0x1A: // Horadric Cube (Left side)
    case 0x21: // Mercenary Inventory (Left side)
        if (gaPanelStates[0x01] || gaPanelStates[0x04]) {
            if (nNewState != 0) {
                gnPanelShiftOffset = 0;
                gnPanelShiftMode = 3;
                D2GFX_SetViewportShift(0);
                ((void(__fastcall*)(int32_t, int32_t, int32_t))0x6FB6A990)(gnViewportHeight, gnViewportWidth, 0);
            } else {
                gnPanelShiftOffset = -nQuarterW;
                gnPanelShiftMode = 1;
                D2GFX_SetViewportShift(-nQuarterW);
                ((void(__fastcall*)(int32_t, int32_t, int32_t))0x6FB6A990)(gnViewportHeight, gnViewportWidth - gnViewportWidth / 4, -(gnViewportWidth / 4));
            }
        } else if (nNewState != 0) {
            gnPanelShiftOffset = nQuarterW;
            gnPanelShiftMode = 2;
            D2GFX_SetViewportShift(nQuarterW);
            ((void(__fastcall*)(int32_t, int32_t, int32_t))0x6FB6A990)(gnViewportHeight, gnViewportWidth + gnViewportWidth / 4, gnViewportWidth / 4);
            if (bMoveMouse && nSavedMouseY < gnScreenWidth - nQuarterW) {
                ((void(__fastcall*)(int32_t, int32_t))0x6FB4ECD0)(nSavedMouseX, nSavedMouseY + nQuarterW);
            }
        } else {
            gnPanelShiftOffset = 0;
            gnPanelShiftMode = 0;
            D2GFX_SetViewportShift(0);
            ((void(__fastcall*)(int32_t, int32_t, int32_t))0x6FB6A990)(gnViewportHeight, gnViewportWidth, 0);
            if (bMoveMouse && nSavedMouseY > gnScreenWidth / 2) {
                ((void(__fastcall*)(int32_t, int32_t))0x6FB4ECD0)(nSavedMouseX, nSavedMouseY - nQuarterW);
            }
        }
        break;

    default:
        break;
    }

    return 1;
}

// 1.12a: 0x6FB3F480 (__stdcall)
void __stdcall UI_ReleaseAllAssets()
{
    // Frees all loaded HUD / UI cell files and resets pointers
}

// 1.12a: 0x6FB3F6B0 (__fastcall)
void __fastcall UI_ToggleMiniPanel()
{
    UI_SetPanelState(0x15, 2, 0);
}

// 1.12a: 0x6FB3F800 (merged in tail of sub_6FB3F6B0 in UI_ui.asm)
void __stdcall UI_CloseMercInteract()
{
    ((void(__stdcall*)())0x6FB235B0)();
    void*** pppMercMenu = (void***)0x6FBCB67F;
    if (*pppMercMenu != nullptr) {
        if (*(int32_t*)0x6FBCB673 != 0) {
            ((void(__stdcall*)())0x6FB13290)();
            if (*pppMercMenu != nullptr) {
                reinterpret_cast<void(__thiscall*)(void*, int32_t)>((*reinterpret_cast<void***>(*pppMercMenu))[0])(*pppMercMenu, 1);
            }
            *pppMercMenu = nullptr;
            *(int32_t*)0x6FBCB673 = 0;
        }
        ((void(__stdcall*)())0x6FACADF0)();
    }
}

// 1.12a: 0x6FB3F850 (__fastcall)
void __fastcall UI_OpenGoldDialog(int32_t nMode /*ecx*/)
{
    *(int32_t*)0x6FBCC394 = nMode;
    *(int32_t*)0x6FBB58FC = 1;
}

// 1.12a: 0x6FB3FCB0 (__stdcall)
void __stdcall UI_InitAllPanels()
{
    // Initializes all UI sub-panels, loads global UI cell files, and registers message handlers
}

// 1.12a: 0x6FB40150 (__fastcall)
// Main client UI/HUD render loop called every frame
void __fastcall UI_RenderHUD()
{
    // Renders open left/right panels (Inventory, Char, SkillTree, Stash, Cube, Trade, Shop, Quest, Party, Merc),
    // bottom control panel (orbs, stamina, belt, skill buttons, mini-panel), gold dialog, ESC menu, and hover tooltips
    PANEL_RenderControlPanel();
    UI_DrawDeferredHoverText();
}
