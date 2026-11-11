// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Translation Unit: UI_npcmenu.cpp
// Sources:
//   - ..\Source\D2Client\UI\dialog.cpp   (0x6FB12960..0x6FB14190, 39 functions)
//   - ..\Source\D2Client\UI\npcmenu.cpp  (0x6FAC2190..0x6FACED40, 286 functions + 9 internal entries)
// ImageBase: 0x6FAB0000
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "D2ClientReconstructed.h"

struct D2UnitStrc;
struct D2RoomStrc;
struct D2CellFileStrc;
struct D2InventoryStrc;
struct D2SkillStrc;
struct D2SkillsTxt;
struct D2ItemsTxt;
struct D2LevelsTxt;
struct D2BitBufferStrc;
struct D2TextHeaderStrc;
struct D2TextNodeStrc;
struct Unicode;

// ============================================================================
// External Imports (D2Common, D2Win, D2Gfx, D2Lang, D2CMP, D2Net, Fog, Storm)
// ============================================================================
extern "C" {
    // Fog.dll
    void* __fastcall FOG_Alloc(int nSize, const char* szFile, int nLine, int nZero); // Fog!#10042
    void __fastcall FOG_Free(void* pMem, const char* szFile, int nLine, int nZero); // Fog!#10043
    const char* __stdcall UnimplementedOrdinal_10265(int nLine); // Fog!#10265
    void __cdecl FOG_DisplayHalt(const char* szFormat, ...); // Fog!#10024
    int __stdcall FOG_IsExpansion(); // Fog!#10227
    uint32_t __cdecl FOG_10055_GetSyncTime(); // Fog!#10055
    float __stdcall FOG_10083_Cos_LUT(int nAngle); // Fog!#10083
    float __stdcall FOG_10084_Sin_LUT(int nAngle); // Fog!#10084
    void __fastcall FOG_SetMemoryPoolSystemMode(int nMode); // Fog!#10143
    void __fastcall UnimplementedOrdinal_10144(int nMode); // Fog!#10144
    void __stdcall FOG_GetInstallPath(char* szPath, int nMaxLen); // Fog!#10115
    int __fastcall UnimplementedOrdinal_10191(const char* szPath); // Fog!#10191

    // Storm.dll
    int __stdcall SMsgBreakHandlerChain(void* pMsg); // Storm!#511
    int __stdcall SStrCmp(const char* s1, const char* s2, size_t nMax); // Storm!#580
    int __stdcall SCmdRegisterArgList(const void* pArgList, size_t nCount); // Storm!#472
    int __stdcall SCmdProcess(const char* szCmdLine, int bSkipProgName, void* pfnCallback, int nFlags); // Storm!#474
    int __stdcall SRegLoadString(const char* szSection, const char* szKey, int nFlags, char* szBuffer, size_t nSize); // Storm!#422
    int __stdcall SRegSaveString(const char* szSection, const char* szKey, int nFlags, const char* szString); // Storm!#425
    int __stdcall SRegLoadValue(const char* szSection, const char* szKey, int nFlags, int* pValue); // Storm!#423
    int __stdcall SFileOpenFileEx(HANDLE hMpq, const char* szFileName, uint32_t dwSearchScope, HANDLE* phFile); // Storm!#268
    int __stdcall SFileCloseFile(HANDLE hFile); // Storm!#253

    // D2Gfx.dll
    HWND __stdcall WINDOW_GetWindow(); // D2gfx!#10078
    void __stdcall TEXTURE_CelDraw(D2GfxDataStrc* pData, int nX, int nY, uint32_t dwGamma, int nDrawMode, uint8_t* pPalette); // D2gfx!#10024
    void __stdcall D2GFX_DrawBox(int nXStart, int nYStart, int nXEnd, int nYEnd, uint8_t nColor, int nDrawMode); // D2gfx!#10062
    void __stdcall D2GFX_DrawLine(int nXStart, int nYStart, int nXEnd, int nYEnd, uint8_t nColor, uint8_t nAlpha); // D2gfx!#10057
    int __stdcall D2GFX_GetResolutionMode(); // D2gfx!#10005
    int __stdcall D2GFX_GetDisplayType(); // D2gfx!#10045
    int __stdcall D2GFX_CheckPerspective(); // D2gfx!#10010
    int __stdcall D2GFX_CheckLowQuality(); // D2gfx!#10013
    int __stdcall SCALE_SetPerspectiveScale(int nX, int nY); // D2gfx!#10066
    int __stdcall SCALE_AdjustPerspectivePosition(int nX, int nY, int nBais, int* pX, int* pY); // D2gfx!#10064
    void __stdcall UnimplementedOrdinal_10060(int a1, int a2); // D2gfx!#10060
    void __stdcall UnimplementedOrdinal_10061(int a1); // D2gfx!#10061
    void __stdcall UnimplementedOrdinal_10062(int a1, const char* szName, int a3, int a4); // D2gfx!#10062
    int __stdcall UnimplementedOrdinal_10063(int a1); // D2gfx!#10063
    int __stdcall UnimplementedOrdinal_10064(int a1, void* a2); // D2gfx!#10064
    void __stdcall UnimplementedOrdinal_10065(int a1, void* a2); // D2gfx!#10065

    // D2Win.dll
    int __fastcall D2Win_10127_SetFont(int nFont); // D2Win!#10010
    int __fastcall D2Win_10121_GetTextWidth(const Unicode* wszText); // D2Win!#10132
    void __fastcall D2Win_10117_DrawText(const Unicode* wszText, int nX, int nY, int nColor, int bCentered); // D2Win!#10001
    uint8_t __fastcall D2Win_10034_MixRGB(uint8_t nRed, uint8_t nGreen, uint8_t nBlue); // D2Win!#10034
    int __fastcall D2Win_10005(int a1, int a2, int a3, void* a4); // D2Win!#10005
    int __stdcall D2Win_10015(int a1, int a2, void* a3); // D2Win!#10015
    int __stdcall ARCHIVE_UnloadExpansionArchives(); // D2Win!#10039

    // D2CMP.dll
    int __stdcall D2CMP_CelFileFreeHardware(D2CellFileStrc* pCellFile); // D2CMP!#10106
    void* __stdcall D2CMP_CelGetHandle(D2GfxDataStrc* pGfxData); // D2CMP!#10038
    int __stdcall D2CMP_CelGetWidth(void* pCelHandle); // D2CMP!#10031
    int __stdcall D2CMP_CelGetHeight(void* pCelHandle); // D2CMP!#10096

    // D2Lang.dll
    Unicode* __fastcall D2LANG_GetStringFromTblIndex(uint16_t nIndex); // D2Lang!#10004
    int __fastcall STRTABLE_GetLanguage(); // D2Lang!#10006
    void __fastcall STRTABLE_GetLanguageInfo(int nLang, char* szOut, int bShort); // D2Lang!#10009

    // D2Game.dll
    void __stdcall GAME_SetInitSeed(int nSeed);

    // D2Common.dll
    D2RoomStrc* __stdcall UNITS_GetRoom(D2UnitStrc* pUnit);
    void __stdcall UNITS_GetCoords(D2UnitStrc* pUnit, POINT* pCoord);
    void __stdcall UNITS_GetClientCoords(D2UnitStrc* pUnit, RECT* pRect);
    int __stdcall UNITS_GetDistanceToOtherUnit(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2);
    void __stdcall UNITS_ChangeAnimMode(D2UnitStrc* pUnit, int nMode);
    D2SkillStrc* __stdcall UNITS_GetUsedSkill(D2UnitStrc* pUnit);
    D2UnitStrc* __stdcall UNITFINDS_FindUnitInNeighboredRooms(D2RoomStrc* pRoom, int nUnitType, int nClassId);
    void __stdcall UNITROOM_RemoveUnitFromRoom(D2UnitStrc* pUnit);
    int __stdcall PATH_GetXPosition(void* pPath);
    int __stdcall PATH_GetYPosition(void* pPath);
    int __stdcall PATH_GetType(void* pPath);
    int __stdcall PATH_GetMaxDistance(void* pPath);
    void __stdcall PATH_SetFootprintCollisionMask(void* pPath, uint32_t dwMask);
    void __stdcall MISSILE_SetTargetX(D2UnitStrc* pMissile, int nTargetX);
    void __stdcall MISSILE_SetTargetY(D2UnitStrc* pMissile, int nTargetY);
    void __stdcall MISSILE_SetTotalFrames(D2UnitStrc* pMissile, int nFrames);
    void __stdcall MISSILE_SetCurrentFrame(D2UnitStrc* pMissile, int nFrame);
    int __stdcall DUNGEON_GetLevelIdFromRoom(D2RoomStrc* pRoom);
    void __stdcall DUNGEON_GetRoomCoordinates(D2RoomStrc* pRoom, void* pCoords);
    D2RoomStrc* __stdcall DUNGEON_GetRoomFromAct(void* pAct, int nX, int nY);
    void __stdcall DUNGEON_GetAdjacentRoomsListFromRoom(D2RoomStrc* pRoom, D2RoomStrc*** pppRoomList, int* pNumRooms);
    int __stdcall DUNGEON_GetWarpDestinationLevel(D2RoomStrc* pRoom, int nSourceLevel);
    int __stdcall DRLG_GetActNoFromLevelId(int nLevelId);
    int __stdcall COLLISION_CheckMask(D2RoomStrc* pRoom, int nX, int nY, uint16_t nMask);
    int __stdcall COLLISION_CheckMaskWithSize(D2RoomStrc* pRoom, int nX, int nY, int nSize, uint16_t nMask);
    int __stdcall DATATBLS_GetRainFromLevelsTxt(int nLevelId);
    int __stdcall DATATBLS_GetMudFromLevelsTxt(int nLevelId);
    D2LevelsTxt* __stdcall DATATBLS_GetLevelsTxtRecord(int nLevelId);
    D2ItemsTxt* __stdcall DATATBLS_GetItemsTxtRecord(int nItemId);
    int __stdcall ENVIRONMENT_GetPeriodOfDayFromAct(void* pAct, int* pBaseTime);
    int __stdcall MONSTERS_IsDead(D2UnitStrc* pMonster);
    int __stdcall STATLIST_UnitGetStatValue(const D2UnitStrc* pUnit, int nStatId, int nLayer);
    int __stdcall STATLIST_GetMaxDurabilityFromUnit(D2UnitStrc* pUnit);
    void __stdcall STATLIST_FreeModStats(D2UnitStrc* pUnit);
    int __stdcall STATES_CheckState(D2UnitStrc* pUnit, int nState);
    D2SkillsTxt* __stdcall SKILLS_GetSkillsTxtRecordFromSkill(D2SkillStrc* pSkill);
    D2BitBufferStrc* __stdcall QUESTRECORD_AllocRecord(void* pMemPool);
    void __stdcall QUESTRECORD_FreeRecord(void* pMemPool, D2BitBufferStrc* pQuestRecord);
    void __stdcall QUESTRECORD_CopyBufferToRecord(D2BitBufferStrc* pQuestRecord, uint8_t* pBuffer, uint32_t nBufferSize, int bCheck);
    int __stdcall QUESTRECORD_GetQuestState(D2BitBufferStrc* pQuestRecord, int nQuestNo, int nState);
    D2TextHeaderStrc* __stdcall TEXT_AllocTextHeader(void* pMemPool);
    void __stdcall TEXT_FreeTextHeader(D2TextHeaderStrc* pTextHeader);
    void __stdcall TEXT_AddNodeToTextList(D2TextHeaderStrc* pTextHeader, short nStringId, int nMenu);
    int __stdcall TEXT_GetMenuType2NodeCount(D2TextHeaderStrc* pTextHeader);
    short __stdcall TEXT_GetStringIdOfMenuType2Node(D2TextHeaderStrc* pTextHeader, int nNodeId);
    short __stdcall TEXT_GetStringIdOfMenuType1Or2Node(D2TextHeaderStrc* pTextHeader, int nNodeId);
    void __stdcall TEXT_CreateMessageListFromTextHeader(D2TextHeaderStrc* pTextHeader);
    void __stdcall TEXT_SortTextNodeListByStringId(D2TextHeaderStrc* pTextHeader);
    D2UnitStrc* __stdcall INVENTORY_GetFirstItem(D2InventoryStrc* pInventory);
    D2UnitStrc* __stdcall INVENTORY_GetNextItem(D2UnitStrc* pItem);
    D2UnitStrc* __stdcall INVENTORY_UnitIsItem(D2UnitStrc* pItem);
    D2UnitStrc* __stdcall INVENTORY_GetCursorItem(D2InventoryStrc* pInventory);
    int __stdcall INVENTORY_RemoveInventoryItems(D2InventoryStrc* pInventory);
    int __stdcall ITEMS_CheckItemFlag(D2UnitStrc* pItem, uint32_t dwFlag, int nLine, const char* szFile);
    int __stdcall ITEMS_CheckIfThrowable(D2UnitStrc* pItem);
    int __stdcall ITEMS_IsPersonalizable(D2UnitStrc* pItem);
    int __stdcall ITEMS_HasDurability(D2UnitStrc* pItem);
    int __stdcall ITEMS_IsNotQuestItem(D2UnitStrc* pItem);
    int __stdcall ITEMS_GetAllRepairCosts(void* pGame, D2UnitStrc* pPlayer, int nNpcClassId, int nDifficulty, D2BitBufferStrc* pQuestRecord, void* pfnCallback);
    int __stdcall ITEMS_GetTransactionCost(D2UnitStrc* pPlayer, D2UnitStrc* pItem, int nDifficulty, D2BitBufferStrc* pQuestRecord, int nNpcClassId, int nTransactionType);
    D2UnitStrc* __stdcall DEBUG_GetUnitFromTypeAndGUID(int nUnitType, int nUnitGUID);
    int __stdcall D2COMMON_11017_CheckUnitIfConsumeable(D2UnitStrc* pItem, int a2);
    int __stdcall D2COMMON_11018(D2UnitStrc* pUnit);
    int __stdcall D2COMMON_11019(D2UnitStrc* pUnit);
    int __stdcall D2Common_11020(D2UnitStrc* pUnit);
    int __stdcall D2COMMON_11021(D2UnitStrc* pUnit);
}

// ============================================================================
// Data Structures
// ============================================================================
struct Unicode {
    uint16_t ch;
    static int __fastcall strlen(const Unicode* pStr);
    static Unicode* __fastcall strncpy(Unicode* pDst, const Unicode* pSrc, int nCount);
    static void __fastcall win2Unicode(Unicode* pDst, const char* szSrc, int nMax);
    static int __cdecl sprintf(int nMaxLen, Unicode* pDst, const Unicode* pFmt, ...);
};

struct D2DialogWidgetBaseStrc {
    struct VTable {
        void (__thiscall* pfnDestroy)(D2DialogWidgetBaseStrc*, int);
        void (__thiscall* pfnDraw)(D2DialogWidgetBaseStrc*);
        int (__thiscall* pfnOnMouseWheel)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnLButtonDown)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnLButtonUp)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnMouseMove)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnKeyDown)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnUpArrow)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnDownArrow)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnLeftArrow)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnRightArrow)(D2DialogWidgetBaseStrc*, void* pMsg);
    }* pVtbl;
};

struct D2DialogWidgetNodeStrc {
    D2DialogWidgetBaseStrc* pWidget;      // +0x00
    D2DialogWidgetNodeStrc* pNext;        // +0x04
};

// sizeof = 0x110
struct D2DialogLineStrc {
    uint16_t wszText[120];                // +0x00 (in D2DialogStrc: +0x68)
    int32_t nLineHeight;                  // +0xF0 (in D2DialogStrc: +0x158)
    int32_t nTextOffsetX;                 // +0xF4 (in D2DialogStrc: +0x15C)
    int32_t nTextWidth;                   // +0xF8 (in D2DialogStrc: +0x160)
    int32_t nUnkF4;                       // +0xFC (in D2DialogStrc: +0x164)
    int32_t nFont;                        // +0x100 (in D2DialogStrc: +0x168)
    int32_t nColor;                       // +0x104 (in D2DialogStrc: +0x16C)
    int (__cdecl* pfnAction)(void);       // +0x108 (in D2DialogStrc: +0x170)
    int32_t bSelectable;                  // +0x10C (in D2DialogStrc: +0x174)
};

// sizeof = 0xB10
struct D2DialogStrc {
    uint32_t dwCreationTick;              // +0x00
    uint32_t dwUnk04;                     // +0x04
    D2CellFileStrc* pBackgroundCel;       // +0x08
    int32_t bHasBackgroundCel;            // +0x0C
    int32_t bMousePressedInDialog;        // +0x10
    int32_t bUseNonSelectableHeightAnchor;// +0x14
    int32_t bBlockUnhandledKeys;          // +0x18
    int32_t bCloseOnRightClick;           // +0x1C
    int32_t bIgnoreMenuHotkeys;           // +0x20
    int32_t nAnchorX;                     // +0x24
    int32_t nAnchorY;                     // +0x28
    int32_t nPosX;                        // +0x2C
    int32_t nPosY;                        // +0x30
    int32_t nWidth;                       // +0x34
    int32_t nHeight;                      // +0x38
    int32_t nUnk3C;                       // +0x3C
    int32_t nUnk40;                       // +0x40
    int32_t nSelectedIndex;               // +0x44
    int32_t nHoveredIndex;                // +0x48
    int32_t nSelectableLineCount;         // +0x4C
    int32_t nLineCount;                   // +0x50
    uint32_t dwFrameCount;                // +0x54
    int32_t bAutoCalcBounds;              // +0x58
    int32_t nSelectionCursorMode;         // +0x5C (1 = highlight color 3, 2 = pentagram cursors)
    int (__cdecl* pfnCancel)(void);       // +0x60
    int (__cdecl* pfnOutsideClick)(void); // +0x64
    D2DialogLineStrc aLines[10];          // +0x68..+0xB07
    D2DialogWidgetNodeStrc* pWidgetList;  // +0xB08
    D2DialogStrc* pPrevDialog;            // +0xB0C
};

#pragma pack(push, 1)
// sizeof = 0x27 (table at 0x6FBA8300, 48 entries)
struct D2NpcMenuDefStrc {
    int32_t nNpcClassId;                  // +0x00
    int32_t nOptionCount;                 // +0x04
    uint16_t wStringIds[5];               // +0x08
    void* pfnCallbacks[5];                // +0x12
    uint8_t bHasMenu;                     // +0x26
};

// sizeof = 0x16 (table at 0x6FBA7F08, 45 entries)
struct D2NpcGossipDefStrc {
    uint16_t wNpcClassId;                 // +0x00
    uint16_t wCount;                      // +0x02
    uint16_t wIntroIds[8];                // +0x04
    uint16_t bIntroShown;                 // +0x14
};

// sizeof = 0x14 (table at 0x6FBA6AA8)
struct D2NpcQuestActionDefStrc {
    int32_t nNpcClassId;                  // +0x00
    int32_t nAct;                         // +0x04
    int32_t nQuestId;                     // +0x08
    int32_t nQuestState;                  // +0x0C
    int (__cdecl* pfnCheck)(void);        // +0x10
};

// sizeof = 0x4C (hireling candidate node at 0x6FBC9AC0)
struct D2HirelingEntryStrc {
    uint16_t wNameId;                     // +0x00
    uint16_t wSubType;                    // +0x02
    uint32_t dwMercId;                    // +0x04
    uint32_t dwSeed;                      // +0x08
    uint32_t dwGoldCost;                  // +0x0C
    uint32_t dwExp;                       // +0x10
    uint32_t dwLevel;                     // +0x14
    uint32_t dwStrength;                  // +0x18
    uint32_t dwDexterity;                 // +0x1C
    uint32_t dwMinDmg;                    // +0x20
    uint32_t dwMaxDmg;                    // +0x24
    uint32_t dwDefense;                   // +0x28
    uint32_t dwMaxLife;                   // +0x2C
    uint32_t dwFireResist;                // +0x30
    uint32_t dwColdResist;                // +0x34
    uint32_t dwLightResist;               // +0x38
    uint32_t dwPoisonResist;              // +0x3C
    uint16_t wSkillIds[2];                // +0x40
    uint8_t nSkillLevels[2];              // +0x44
    uint8_t nSkillModes[2];               // +0x46
    D2HirelingEntryStrc* pNext;           // +0x48
};
#pragma pack(pop)

// ============================================================================
// Cross-Module Forward Declarations & SMsg Registration Tables
// ============================================================================
struct D2SMsgRegEntryStrc {
    uint32_t nDispatchType;
    uint32_t dwMsgOrKey;
    void (__stdcall* pfnHandler)(void* pMsg);
};

extern void __fastcall WALL2_UnregisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries);
extern void __fastcall WALL2_RegisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries);
extern D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileDefault(const char* szFileName, int32_t nFileType);
extern void __fastcall UI_DrawBoxPiecesFrame(RECT* pRect);
extern void __fastcall UI_StartPanelCloseClickCooldown(void);
extern int32_t __fastcall GAME_HasMouseWheelB(void);
extern int32_t __fastcall CMDTBL_HasKeyBound(int32_t nCommandId, int32_t bIsPrimary);
extern uint16_t __fastcall CMDTBL_GetBoundKey(int32_t nCommandId, int32_t bIsPrimary);
extern void __stdcall INV_ClearHoveredItemState();
extern void __stdcall SOUND_PlaySoundEffect(uint32_t nSoundId, D2UnitStrc* pUnit, int32_t nX, int32_t nY, BOOL bGlobal);
extern int32_t __fastcall UI_HitTestNewSkillBtn640(int32_t nMouseX, int32_t nSplitMode, int32_t nMouseY);
extern int32_t __fastcall UI_HitTestNewStatBtn640(int32_t nMouseX, int32_t nSplitMode, int32_t nMouseY);
extern int32_t __fastcall D2Client_HitTestMiniPanel(int32_t nMouseX);
extern void __stdcall AUTOMAP_NoOp(int32_t nUnused);
extern void __stdcall AUTOMAP_PanRight(int32_t nUnused);
extern void __stdcall AUTOMAP_PanLeft(int32_t nUnused);
extern void __stdcall AUTOMAP_PanUp(int32_t nUnused);
extern void __stdcall AUTOMAP_PanDown(int32_t nUnused);

extern int32_t __fastcall MSG_SendByteDword(uint8_t bOpcode, uint32_t dw1);
extern int32_t __fastcall MSG_SendByte2Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2);
extern int32_t __fastcall MSG_SendByte3Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2, uint32_t dw3);
extern int32_t __fastcall MSG_SendByte4Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2, uint32_t dw3, uint32_t dw4);

extern int32_t g_bLeftMouseHeld;
extern int32_t g_bLeftMouseRepeating;
extern int32_t g_bRightMouseHeld;
extern int32_t g_bRightMouseRepeating;
extern int32_t gnTooltipUnitType;
extern int32_t gnTooltipUnitGUID;

void __stdcall sub_6FB13180(void* pMsg);
void __stdcall sub_6FB13540(void* pMsg);
void __stdcall sub_6FB135D0(void* pMsg);
void __stdcall sub_6FB13AA0(void* pMsg);
void __stdcall sub_6FB13C00(void* pMsg);
void __stdcall sub_6FB13CB0(void* pMsg);
void __stdcall sub_6FB13D30(void* pMsg);
void __stdcall sub_6FB13DB0(void* pMsg);
void __stdcall sub_6FB13E30(void* pMsg);
void __stdcall sub_6FB13EB0(void* pMsg);
void __stdcall sub_6FB13F60(void* pMsg);
void __stdcall sub_6FB14120(void* pMsg);

int __cdecl sub_6FAC8DD0();
int __cdecl sub_6FAC9B40();
int __cdecl sub_6FAC9D00();
int __cdecl sub_6FAC9D40();
int __cdecl sub_6FACAA20();
int __cdecl sub_6FACAA80();
int __cdecl sub_6FACB280();
int __cdecl sub_6FACB2B0();
int __cdecl sub_6FACB2E0();
int __cdecl sub_6FACBB80();
int __cdecl sub_6FACC100();
int __cdecl sub_6FACC2E0();
int __cdecl sub_6FACDE00();
int __cdecl sub_6FACDE30();
int __cdecl sub_6FACDE60();
D2UnitStrc* __cdecl sub_6FACE310();

static const D2SMsgRegEntryStrc s_aAutomapArrowHandlers[5] = {
    { 3, VK_UP,    reinterpret_cast<void (__stdcall*)(void*)>(AUTOMAP_PanUp)    },
    { 3, VK_DOWN,  reinterpret_cast<void (__stdcall*)(void*)>(AUTOMAP_PanDown)  },
    { 3, VK_RIGHT, reinterpret_cast<void (__stdcall*)(void*)>(AUTOMAP_PanRight) },
    { 3, VK_LEFT,  reinterpret_cast<void (__stdcall*)(void*)>(AUTOMAP_PanLeft)  },
    { 3, VK_END,   reinterpret_cast<void (__stdcall*)(void*)>(AUTOMAP_NoOp)     },
};

static D2SMsgRegEntryStrc s_aDialogMsgHandlers[14] = {
    { 3, VK_ESCAPE,      sub_6FB135D0 },
    { 3, VK_ESCAPE,      sub_6FB135D0 },
    { 0, WM_LBUTTONUP,   sub_6FB13180 },
    { 0, WM_LBUTTONDOWN, sub_6FB13F60 },
    { 0, WM_RBUTTONUP,   sub_6FB13180 },
    { 0, WM_RBUTTONDOWN, sub_6FB13F60 },
    { 3, VK_UP,          sub_6FB13D30 },
    { 3, VK_DOWN,        sub_6FB13CB0 },
    { 3, VK_RIGHT,       sub_6FB13E30 },
    { 3, VK_LEFT,        sub_6FB13DB0 },
    { 3, VK_RETURN,      sub_6FB13AA0 },
    { 3, VK_ESCAPE,      sub_6FB13540 },
    { 0, WM_CHAR,        sub_6FB13C00 },
    { 0, WM_MOUSEMOVE,   sub_6FB14120 },
};

static const D2SMsgRegEntryStrc s_aDialogWheelHandlers[1] = {
    { 0, WM_MOUSEWHEEL,  sub_6FB13EB0 },
};

// ============================================================================
// Module Globals
// ============================================================================
D2DialogStrc* g_pActiveDialog = nullptr;
int32_t g_bDialogOpen = 0;
int32_t g_nActiveDialogCount = 0;
D2CellFileStrc* g_pPentagramCel = nullptr;
uint32_t g_dwPentagramFrame = 0;
int32_t g_nDialogModalRefCount = 0;
RECT g_rcDialogBounds = { 0 };
int32_t g_bDialogBoundsValid = 0;
static int32_t s_bDialogCursorInvalidated = 0;
static int32_t s_nDialogBorderPadX = 16;
static int32_t s_nDialogBorderPadY = 16;
static const char* const g_apszDialogBgNames[] = { "dialogbackground" };

static const D2NpcMenuDefStrc g_aNpcMenuDefs[48] = {
    { 148, 3, { 3381, 3396,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, nullptr,             nullptr, nullptr }, 1 }, // Akara
    { 176, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Atma
    { 146, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 0 }, // Cain1
    { 154, 3, { 3381, 3334,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, nullptr,             nullptr, nullptr }, 1 }, // Charsi
    { 177, 3, { 3381, 3396,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, nullptr,             nullptr, nullptr }, 1 }, // Drognan
    { 199, 4, { 3381, 3396, 3398, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, (void*)sub_6FAC9D40, nullptr, nullptr }, 1 }, // Elzix
    { 200, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Geglash
    { 147, 4, { 3381, 3396, 3398, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, (void*)sub_6FAC9D40, nullptr, nullptr }, 1 }, // Gheed
    { 198, 3, { 3381, 3397,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FACBB80, nullptr,             nullptr, nullptr }, 1 }, // Greiz
    { 201, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Jerhyn
    { 202, 3, { 3381, 3396,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, nullptr,             nullptr, nullptr }, 1 }, // Lysander
    { 150, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Kashya
    { 178, 3, { 3381, 3334,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, nullptr,             nullptr, nullptr }, 1 }, // Fara
    { 155, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Warriv1
    { 175, 3, { 3381, 3383,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FACAA20, nullptr,             nullptr, nullptr }, 1 }, // Warriv2
    { 210, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Meshif1
    { 244, 3, { 3381, 4020,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC8DD0, nullptr,             nullptr, nullptr }, 1 }, // Cain2
    { 265, 3, { 3381, 4020,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC8DD0, nullptr,             nullptr, nullptr }, 1 }, // Cain5
    { 245, 3, { 3381, 4020,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC8DD0, nullptr,             nullptr, nullptr }, 1 }, // Cain3
    { 246, 3, { 3381, 4020,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC8DD0, nullptr,             nullptr, nullptr }, 1 }, // Cain4
    { 251, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 0 },
    { 367, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Tyrael
    { 257, 2, { 3334,    0,    0, 0, 0 }, { (void*)sub_6FAC9D00, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Halbu
    { 405, 3, { 3396, 3398,    0, 0, 0 }, { (void*)sub_6FAC9D00, (void*)sub_6FAC9D40, nullptr,             nullptr, nullptr }, 1 }, // Jamella
    { 252, 4, { 3381, 3397, 3396, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FACBB80, (void*)sub_6FAC9D00, nullptr, nullptr }, 1 }, // Asheara
    { 253, 3, { 3381, 3334,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, nullptr,             nullptr, nullptr }, 1 }, // Hratli
    { 254, 4, { 3381, 3396, 3398, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, (void*)sub_6FAC9D40, nullptr, nullptr }, 1 }, // Alkor
    { 255, 3, { 3381, 3396,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, nullptr,             nullptr, nullptr }, 1 }, // Ormus
    { 406, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Izual
    { 257, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 },
    { 264, 3, { 3381, 3385,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FACAA80, nullptr,             nullptr, nullptr }, 1 }, // Meshif2
    { 297, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Natalya
    { 266, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Flavie
    { 331, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Kaelan
    { 408, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Hadriel
    { 378, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 },
    { 377, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 },
    { 521, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Tyrael2
    { 520, 3, { 3381, 4020,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC8DD0, nullptr,             nullptr, nullptr }, 1 }, // Cain6
    { 511, 3, { 3381, 3334,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, nullptr,             nullptr, nullptr }, 1 }, // Larzuk
    { 513, 3, { 3381, 3396,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, nullptr,             nullptr, nullptr }, 1 }, // Malah
    { 512, 4, { 3381, 3396, 3398, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D00, (void*)sub_6FAC9D40, nullptr, nullptr }, 1 }, // Anya
    { 514, 3, { 3381, 3398,    0, 0, 0 }, { (void*)sub_6FACC100, (void*)sub_6FAC9D40, nullptr,             nullptr, nullptr }, 1 }, // Nihlathak
    { 515, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 1 }, // Qual-Kehk
    { 527, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 0 },
    { 537, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 0 },
    { 538, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 0 },
    { 539, 2, { 3381,    0,    0, 0, 0 }, { (void*)sub_6FACC100, nullptr,             nullptr,             nullptr, nullptr }, 0 },
};

static D2NpcGossipDefStrc g_aNpcGossipDefs[45] = {
    { 148, 0, {  6144, 47727, 3439, 0, 0, 0, 0,   0 }, 256 },
    { 154, 0, { 36864, 47726, 2415, 0, 0, 0, 0,   0 }, 256 },
    { 146, 0, { 14336, 47725, 1903, 0, 0, 0, 0,   0 }, 257 },
    { 147, 0, { 45056, 47724, 2415, 0, 0, 0, 0,   0 }, 256 },
    { 150, 0, { 20480, 47723, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 155, 0, {  2048, 47724, 2927, 0, 0, 0, 0, 256 }, 256 },
    { 331, 0, {  2048, 47724,  111, 0, 0, 0, 0,   0 }, 256 },
    { 266, 0, {  2048, 47724,  111, 0, 0, 0, 0,   0 }, 256 },
    { 408, 0, {  2051, 47724,  111, 0, 0, 0, 0,   0 }, 256 },
    { 175, 0, {  6145, 47734, 1903, 0, 0, 0, 0,   0 }, 257 },
    { 176, 0, { 57345, 47727, 2671, 0, 0, 0, 0,   0 }, 257 },
    { 177, 0, { 30721, 47728, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 199, 0, {  2049, 47730, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 200, 0, { 49153, 47730, 2927, 0, 0, 0, 0,   0 }, 256 },
    { 198, 0, { 12289, 47729, 3695, 0, 0, 0, 0,   0 }, 256 },
    { 201, 0, { 26625, 47731, 2671, 0, 0, 0, 0,   0 }, 256 },
    { 202, 0, {     1, 47732, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 178, 0, { 47105, 47732, 2927, 0, 0, 0, 0,   0 }, 256 },
    { 210, 0, { 24577, 47733, 3183, 0, 0, 0, 0, 256 }, 256 },
    { 265, 0, { 14336, 47725, 1903, 0, 0, 0, 0,   0 }, 257 },
    { 244, 0, { 43009, 47725, 1903, 0, 0, 0, 0,   0 }, 257 },
    { 245, 0, {  6146, 47726, 2159, 0, 0, 0, 0,   0 }, 256 },
    { 246, 0, { 64515, 47737,  879, 0, 0, 0, 0,   0 }, 256 },
    { 251, 0, { 52225, 47737,  879, 0, 0, 0, 0,   0 }, 256 },
    { 252, 0, { 59394, 47735, 2415, 0, 0, 0, 0,   0 }, 256 },
    { 253, 0, { 12290, 47735, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 254, 0, { 28674, 47736, 2927, 0, 0, 0, 0,   0 }, 256 },
    { 255, 0, {  6146, 47737, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 406, 0, { 47106, 47732, 2927, 0, 0, 0, 0,   0 }, 256 },
    { 257, 0, { 47106, 47732, 2927, 0, 0, 0, 0,   0 }, 256 },
    { 264, 0, { 34818, 47734, 2927, 0, 0, 0, 0,   0 }, 256 },
    { 297, 0, { 12290, 47738, 1647, 0, 0, 0, 0,   0 }, 256 },
    { 367, 0, { 52227, 47737,  879, 0, 0, 0, 0, 256 }, 256 },
    { 405, 0, { 52227, 47737,  879, 0, 0, 0, 0,   0 }, 256 },
    { 257, 0, { 52227, 47737,  879, 0, 0, 0, 0,   0 }, 256 },
    { 511, 0, {     4, 47740, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 512, 0, { 18436, 47739, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 513, 0, { 47108, 47740, 4207, 0, 0, 0, 0,   0 }, 256 },
    { 514, 0, { 43012, 47741, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 515, 0, { 24580, 47742, 2927, 0, 0, 0, 0,   0 }, 256 },
    { 521, 0, { 52228, 47737,  879, 0, 0, 0, 0, 256 }, 256 },
    { 520, 0, { 36868, 47738, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 527, 0, { 18436, 47739, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 537, 0, { 18436, 47739, 3183, 0, 0, 0, 0,   0 }, 256 },
    { 538, 0, { 18436, 47739, 3183, 0, 0, 0, 0,   0 }, 256 },
};

static const D2NpcQuestActionDefStrc g_aNpcQuestActionDefs[8] = {
    { 155,  6, 0, 1, sub_6FACB2E0 },
    { 210, 14, 0, 1, sub_6FACB2B0 },
    { 150,  2, 0, 1, sub_6FACC2E0 },
    { 154,  3, 1, 1, sub_6FACDE60 },
    { 511, 35, 1, 1, sub_6FACDE30 },
    { 512, 38, 1, 1, sub_6FACDE00 },
    { 515, 36, 0, 1, sub_6FACC100 },
    { 367, 28, 0, 1, sub_6FACB280 },
};

int32_t g_bInteractingWithNpc = 0;
int32_t g_nInteractingNpcGUID = 0;
int32_t g_nInteractingNpcClassId = -1;
int32_t g_nActiveNpcMenuIdx = -1;
D2DialogStrc* g_pNpcMainMenuDialog = nullptr;
D2DialogStrc* g_pNpcSubDialog = nullptr;
D2DialogStrc* g_pNpcPopupDialog = nullptr;
D2UnitStrc* g_pTradeNpcUnit = nullptr;
D2BitBufferStrc* g_pPlayerQuestRecord = nullptr;
D2BitBufferStrc* g_pGameQuestRecord = nullptr;
static D2BitBufferStrc*& g_pClientQuestRecord = g_pPlayerQuestRecord;
D2BitBufferStrc* g_pTempQuestRecord = nullptr;
D2TextHeaderStrc* g_pNpcTextHeader = nullptr;
D2HirelingEntryStrc* g_pHirelingListHead = nullptr;

static void* s_pStartupPoolCheck = nullptr;
static int32_t s_bCmdLineFlagSet = 0;
static const uint8_t s_aCmdLineArgList[0x2F * 32] = { 0 };
static uint32_t s_dwMissileSyncEndTick = 0;
static uint32_t s_dwNpcSyncTime = 0;
static int32_t s_nNpcMissileFlag = 0;
static int32_t s_nWeatherState = 0;
static int32_t s_nWeatherMode = 0;
static int32_t s_hEnvParticles = 0;
static int32_t s_hEnvSplashes = 0;
static int32_t s_hEnvBubbles = 0;
static D2CellFileStrc* s_pRainCel = nullptr;
static int32_t s_nGameLoopMode = 0;
static int32_t s_nMpqCheckState = 0;
static int32_t s_bNpcQuestUpdated = 0;
static int32_t s_nNpcTalkState = 0;
static int32_t s_nNpcHireState = 0;
static int32_t s_nNpcShopStateA = 0;
static int32_t s_nNpcShopStateB = 0;
static int32_t s_nNpcMenuSubState = 0;
static int32_t s_bNpcSpeechActive = 0;
static D2CellFileStrc* s_pBuySellCel = nullptr;
static int32_t s_nNpcInteractSubFlag = 0;
static int32_t s_nNpcShopTabMode = 0;
static int32_t s_nClientUnitHashResetFlag = 0;

// ============================================================================
// PART 1: UI\dialog.cpp (0x6FB12960..0x6FB14190, 39 functions)
// ============================================================================

// 1.12a: 0x6FB12960 (__cdecl)
int32_t __cdecl sub_6FB12960()
{
    return g_nActiveDialogCount;
}

// 1.12a: 0x6FB12970 (register: eax=pDialog, ecx=pOutRect)
void __fastcall sub_6FB12970(RECT* pOutRect, D2DialogStrc* pDialog)
{
    if (pDialog && pOutRect) {
        pOutRect->top = pDialog->nPosY;
        pOutRect->left = pDialog->nPosX;
        pOutRect->bottom = pDialog->nPosY + pDialog->nHeight;
        pOutRect->right = pDialog->nPosX + pDialog->nWidth;
    }
}

// 1.12a: 0x6FB129A0 (register: ecx=pDialog, eax=nLineIdx)
D2DialogLineStrc* __fastcall sub_6FB129A0(D2DialogStrc* pDialog, int nLineIdx)
{
    if (pDialog && nLineIdx >= 0 && nLineIdx < pDialog->nLineCount) {
        return &pDialog->aLines[nLineIdx];
    }
    return nullptr;
}

// 1.12a: 0x6FB129C0 (register: eax=pDialog)
int32_t __fastcall sub_6FB129C0(D2DialogStrc* pDialog)
{
    return pDialog ? pDialog->nSelectedIndex : -1;
}

// 1.12a: 0x6FB129D0 (register: edi=pDialog, eax=nLineIdx)
void __fastcall sub_6FB129D0(D2DialogStrc* pDialog, int nLineIdx)
{
    int nOldFont = D2Win_10127_SetFont(1);
    D2DialogLineStrc* pLine = &pDialog->aLines[nLineIdx];
    D2Win_10127_SetFont(pLine->nFont);
    int nTextWidth = D2Win_10121_GetTextWidth(reinterpret_cast<const Unicode*>(pLine->wszText));
    pLine->nTextWidth = nTextWidth;
    if (nTextWidth >= pDialog->nWidth) {
        pLine->nTextOffsetX = 0;
    } else {
        pLine->nTextOffsetX = ((pDialog->nWidth - nTextWidth + 1) / 2) + 1;
    }
    D2Win_10127_SetFont(nOldFont);
}

// 1.12a: 0x6FB12A40 (register: eax=pDialog, stack: nMode)
void __stdcall sub_6FB12A40(D2DialogStrc* pDialog, int32_t nMode)
{
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x529));
        exit(-1);
    }
    pDialog->nSelectionCursorMode = nMode;
}

// 1.12a: 0x6FB12A70 (register: ecx=pDialog, esi=nSelectedIndex)
void __fastcall sub_6FB12A70(D2DialogStrc* pDialog, int32_t nSelectedIndex)
{
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x4F2));
        exit(-1);
    }
    if (nSelectedIndex >= pDialog->nLineCount) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x4F8));
        exit(-1);
    }
    if (nSelectedIndex == -1) {
        pDialog->nSelectedIndex = -1;
        return;
    }
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        if (i == nSelectedIndex) {
            if (!pDialog->aLines[i].bSelectable) {
                FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x51A));
                exit(-1);
            }
            pDialog->nSelectedIndex = nSelectedIndex;
            return;
        }
    }
    FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x51E));
    exit(-1);
}

// 1.12a: 0x6FB12B00 (register: esi=pDialog, stack: pWidget)
void __stdcall sub_6FB12B00(D2DialogStrc* pDialog, D2DialogWidgetBaseStrc* pWidget)
{
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x358));
        exit(-1);
    }
    D2DialogWidgetNodeStrc* pOldHead = pDialog->pWidgetList;
    D2DialogWidgetNodeStrc* pNode = static_cast<D2DialogWidgetNodeStrc*>(
        FOG_Alloc(sizeof(D2DialogWidgetNodeStrc), "..\\Source\\D2Client\\UI\\dialog.cpp", 0x35C, 0));
    pDialog->pWidgetList = pNode;
    pNode->pNext = pOldHead;
    pNode->pWidget = pWidget;
}

// 1.12a: 0x6FB12B60 (register: edx=pDialog)
void __fastcall sub_6FB12B60(D2DialogStrc* pDialog)
{
    memset(pDialog, 0, sizeof(D2DialogStrc));
    pDialog->nSelectedIndex = -1;
    pDialog->nSelectionCursorMode = 2;
}

// 1.12a: 0x6FB12BA0 (__cdecl)
void __cdecl sub_6FB12BA0()
{
    g_bDialogOpen = 0;
}

// 1.12a: 0x6FB12BB0 (register: esi=pDialog, eax=pMsg)
int32_t __fastcall sub_6FB12BB0(D2DialogStrc* pDialog, void* pMsg)
{
    if (!pDialog) {
        return -1;
    }
    int nMouseX = *(uint16_t*)((uint8_t*)pMsg + 0x0C);
    int nMouseY = *(uint16_t*)((uint8_t*)pMsg + 0x0E);
    int nAccumY = 0;
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        D2DialogLineStrc* pLine = &pDialog->aLines[i];
        if (pLine->bSelectable == 1) {
            if (nMouseX > pDialog->nPosX + 15 && nMouseX < pDialog->nPosX + pDialog->nWidth - 15) {
                int nLineBottomY = pDialog->nPosY + nAccumY + pLine->nLineHeight;
                if (nMouseY > nLineBottomY - 11 && nMouseY < nLineBottomY + 4) {
                    return i;
                }
            }
        }
        nAccumY += pLine->nLineHeight;
    }
    return -1;
}

// 1.12a: 0x6FB12C40 (register: edx=pDialog)
void __fastcall sub_6FB12C40(D2DialogStrc* pDialog)
{
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        if (pDialog->aLines[i].bSelectable) {
            pDialog->nSelectedIndex = i;
            return;
        }
    }
}

// 1.12a: 0x6FB12C70 (__cdecl)
D2DialogStrc* __cdecl sub_6FB12C70()
{
    return g_pActiveDialog;
}

// 1.12a: 0x6FB12C80 (register: eax=pDialog)
void __fastcall sub_6FB12C80(D2DialogStrc* pDialog)
{
    if (!pDialog->bAutoCalcBounds) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x5AA));
        exit(-1);
    }
    int nOldFont = D2Win_10127_SetFont(1);
    int nMaxWidth = 0;
    int nTotalHeight = 0;
    int nNonSelectableCount = 0;
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        int nLineWidth = D2Win_10121_GetTextWidth(reinterpret_cast<const Unicode*>(pDialog->aLines[i].wszText));
        if (nLineWidth > nMaxWidth) {
            nMaxWidth = nLineWidth;
        }
        if (!pDialog->aLines[i].bSelectable) {
            nNonSelectableCount++;
        }
        nTotalHeight += pDialog->aLines[i].nLineHeight;
    }
    nMaxWidth += 20;
    nTotalHeight += 15;
    D2Win_10127_SetFont(nOldFont);

    pDialog->nPosX = pDialog->nAnchorX - (nMaxWidth / 2);
    int nOffsetY = pDialog->bUseNonSelectableHeightAnchor ? (nNonSelectableCount * 21) : (nTotalHeight / 3);
    pDialog->nPosY = pDialog->nAnchorY - nOffsetY;

    int nScreenW = gnScreenWidth;
    int nScreenH = gnScreenHeight;
    if (nMaxWidth > nScreenW - 0x50) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x5DD));
        exit(-1);
    }
    if (nTotalHeight > nScreenH - 0x62) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x5E2));
        exit(-1);
    }
    if (pDialog->nPosX + nMaxWidth > nScreenW - 10) {
        pDialog->nPosX = nScreenW - nMaxWidth;
    }
    if (pDialog->nPosY + nTotalHeight > nScreenH - 0x3A) {
        pDialog->nPosY = nScreenH - nTotalHeight - 0x30;
    }
    if (pDialog->nPosX <= 10) pDialog->nPosX = 10;
    if (pDialog->nPosY <= 10) pDialog->nPosY = 10;

    pDialog->nWidth = nMaxWidth;
    pDialog->nHeight = nTotalHeight;
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        sub_6FB129D0(pDialog, i);
    }
}

// 1.12a: 0x6FB12DF0 (register: ecx=pDialog, eax=nColor, stack: wszText, nLineHeight, nUnkF4, nFont, pfnAction, bSelectable)
void __stdcall sub_6FB12DF0(D2DialogStrc* pDialog, int nColor, const Unicode* wszText, int nLineHeight, int nUnkF4, int nFont, int (__cdecl* pfnAction)(void), int bSelectable)
{
    int nIdx = pDialog->nLineCount;
    if (nIdx >= 10) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x55D));
        exit(-1);
    }
    pDialog->nLineCount = nIdx + 1;
    D2DialogLineStrc* pLine = &pDialog->aLines[nIdx];
    pLine->nLineHeight = nLineHeight;
    pLine->nTextOffsetX = 0;
    pLine->nUnkF4 = nUnkF4;
    pLine->nFont = nFont;
    if (nColor == 3 && pDialog->nSelectionCursorMode == 1) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x569));
        exit(-1);
    }
    pLine->nColor = nColor;
    pLine->pfnAction = pfnAction;
    pLine->bSelectable = bSelectable;

    int nLen = Unicode::strlen(wszText) + 1;
    if (nLen > 119) nLen = 119;
    Unicode::strncpy(reinterpret_cast<Unicode*>(pLine->wszText), wszText, nLen);
    pLine->wszText[nLen] = 0;

    if (bSelectable) {
        pDialog->nSelectableLineCount++;
    }
    if (pDialog->bAutoCalcBounds) {
        sub_6FB12C80(pDialog);
    } else {
        sub_6FB129D0(pDialog, nIdx);
    }

    g_rcDialogBounds.left = pDialog->nPosX;
    g_rcDialogBounds.top = pDialog->nPosY;
    g_rcDialogBounds.right = pDialog->nPosX + pDialog->nWidth;
    g_rcDialogBounds.bottom = pDialog->nPosY + pDialog->nHeight;
    g_bDialogBoundsValid = 1;

    if (pDialog->nSelectedIndex == -1 && bSelectable) {
        sub_6FB12A70(pDialog, nIdx);
    }
}

// 1.12a: 0x6FB12F40 (register: esi=pDialog)
void __fastcall sub_6FB12F40(D2DialogStrc* pDialog)
{
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x499));
        exit(-1);
    }
    pDialog->dwFrameCount++;
    RECT rcBox;
    rcBox.left = pDialog->nPosX;
    rcBox.top = pDialog->nPosY;
    rcBox.right = pDialog->nPosX + pDialog->nWidth - 1;
    rcBox.bottom = pDialog->nPosY + pDialog->nHeight - 1;

    D2GfxDataStrc gfxData = {};
    if (pDialog->bHasBackgroundCel) {
        if (pDialog->pBackgroundCel) {
            gfxData.pCellFile = pDialog->pBackgroundCel;
            gfxData.nFrame = 0;
            TEXTURE_CelDraw(&gfxData, rcBox.left, rcBox.bottom + 1, 0xFFFFFFFF, 5, nullptr);
        }
    } else {
        D2GFX_DrawBox(rcBox.left, rcBox.top, rcBox.right + 1, rcBox.bottom + 1, 0, 1);
        UI_DrawBoxPiecesFrame(&rcBox);
    }

    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget) {
            pNode->pWidget->pVtbl->pfnDraw(pNode->pWidget);
        }
    }

    int nOldFont = D2Win_10127_SetFont(1);
    int nAccumY = 0;
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        D2DialogLineStrc* pLine = &pDialog->aLines[i];
        int nColor = pLine->nColor;
        D2Win_10127_SetFont(pLine->nFont);
        int nDrawX = pDialog->nPosX + pLine->nTextOffsetX;
        int nDrawY = pDialog->nPosY + nAccumY + pLine->nLineHeight;
        if (pDialog->nSelectedIndex == i) {
            if (pDialog->nSelectionCursorMode == 1) {
                nColor = 3;
            } else if (pDialog->nSelectionCursorMode == 2 && g_pPentagramCel) {
                gfxData.pCellFile = g_pPentagramCel;
                gfxData.nFrame = g_dwPentagramFrame % 7;
                TEXTURE_CelDraw(&gfxData, nDrawX - 24, nDrawY + 4, 0xFFFFFFFF, 5, nullptr);
                TEXTURE_CelDraw(&gfxData, nDrawX + pLine->nTextWidth + 2, nDrawY + 4, 0xFFFFFFFF, 5, nullptr);
                g_dwPentagramFrame++;
            }
        }
        D2Win_10117_DrawText(reinterpret_cast<const Unicode*>(pLine->wszText), nDrawX, nDrawY, nColor, 0);
        nAccumY += pLine->nLineHeight;
    }
    D2Win_10127_SetFont(nOldFont);
}

// 1.12a: 0x6FB13120 (register: eax=pMsg, stack: bInvokeCancel)
void __stdcall sub_6FB13120(void* pMsg, int bInvokeCancel)
{
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    SMsgBreakHandlerChain(pMsg);
    if (bInvokeCancel) {
        D2DialogStrc* pDialog = g_pActiveDialog;
        if (!pDialog) {
            FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x1C2));
            exit(-1);
        }
        if (pDialog->dwFrameCount >= 5 && pDialog->pfnCancel) {
            pDialog->pfnCancel();
            UI_StartPanelCloseClickCooldown();
        }
    }
}

// 1.12a: 0x6FB13180 (__stdcall)
void __stdcall sub_6FB13180(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    int nMouseX = *(uint16_t*)((uint8_t*)pMsg + 0x0C);
    int nMouseY = *(uint16_t*)((uint8_t*)pMsg + 0x0E);
    if (gaPanelStates[D2PANEL_MINIPANEL]) {
        int nHalfW = gnScreenWidth / 2;
        int nScreenH = gnScreenHeight;
        if (nMouseX > nHalfW - 0x4A && nMouseX < nHalfW + 0x47 && nMouseY < nScreenH - 0x32 && nMouseY > nScreenH - 0x45) {
            if (!pDialog) return;
            if (pDialog->bMousePressedInDialog != 1) pDialog->bMousePressedInDialog = 0;
            return;
        }
    }
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    SMsgBreakHandlerChain(pMsg);
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x123));
        exit(-1);
    }
    if (pDialog->bMousePressedInDialog != 1) {
        for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
            if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnLButtonUp(pNode->pWidget, pMsg) == 1) {
                return;
            }
        }
        return;
    }
    if (pDialog->dwCreationTick + 80 > GetTickCount()) {
        return;
    }
    int nHitLine = sub_6FB12BB0(pDialog, pMsg);
    if (pDialog->nHoveredIndex == nHitLine && nHitLine >= 0 && nHitLine < pDialog->nLineCount) {
        auto pfnAction = pDialog->aLines[pDialog->nSelectedIndex].pfnAction;
        if (pfnAction && pfnAction()) {
            return;
        }
    }
    pDialog->bMousePressedInDialog = 0;
}

// 1.12a: 0x6FB13290 (__cdecl)
void __cdecl sub_6FB13290()
{
    if (--g_nActiveDialogCount != 0) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x441));
        exit(-1);
    }
    if (gaPanelStates[D2PANEL_AUTOMAP]) {
        WALL2_RegisterMsgHandlerTable(5, WINDOW_GetWindow(), s_aAutomapArrowHandlers);
    }
    WALL2_UnregisterMsgHandlerTable(14, WINDOW_GetWindow(), s_aDialogMsgHandlers);
    if (GAME_HasMouseWheelB()) {
        WALL2_UnregisterMsgHandlerTable(1, WINDOW_GetWindow(), s_aDialogWheelHandlers);
    }
    D2DialogStrc* pDialog = g_pActiveDialog;
    s_bDialogCursorInvalidated = 1;
    g_bDialogBoundsValid = 0;
    g_bDialogOpen = 0;

    D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList;
    while (pNode) {
        D2DialogWidgetNodeStrc* pNext = pNode->pNext;
        FOG_Free(pNode, "..\\Source\\D2Client\\UI\\dialog.cpp", 0x430, 0);
        pNode = pNext;
    }
    if (pDialog->bHasBackgroundCel && pDialog->pBackgroundCel) {
        D2CMP_CelFileFreeHardware(pDialog->pBackgroundCel);
        FOG_Free(pDialog->pBackgroundCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
    }
    D2DialogStrc* pPrev = pDialog->pPrevDialog;
    FOG_Free(pDialog, "..\\Source\\D2Client\\UI\\dialog.cpp", 0x43B, 0);
    g_pActiveDialog = pPrev;
}

// 1.12a: 0x6FB133B0 (__cdecl)
void __cdecl sub_6FB133B0()
{
    if (CMDTBL_HasKeyBound(0x26, 1)) {
        uint16_t wKey = CMDTBL_GetBoundKey(0x26, 1);
        if (wKey != 0xFFFF && wKey <= 0xDF) {
            s_aDialogMsgHandlers[0].dwMsgOrKey = wKey;
        }
    }
    if (CMDTBL_HasKeyBound(0x26, 0)) {
        uint16_t wKey = CMDTBL_GetBoundKey(0x26, 0);
        if (wKey != 0xFFFF && wKey <= 0xDF) {
            s_aDialogMsgHandlers[1].dwMsgOrKey = wKey;
        }
    }
}

// 1.12a: 0x6FB13460 (__cdecl)
void __cdecl sub_6FB13460()
{
    if (g_bDialogOpen) {
        WALL2_UnregisterMsgHandlerTable(14, WINDOW_GetWindow(), s_aDialogMsgHandlers);
        if (GAME_HasMouseWheelB()) {
            WALL2_UnregisterMsgHandlerTable(1, WINDOW_GetWindow(), s_aDialogWheelHandlers);
        }
        g_nDialogModalRefCount--;
    }
    D2DialogStrc* pCur = g_pActiveDialog;
    while (pCur) {
        D2DialogStrc* pPrev = pCur->pPrevDialog;
        FOG_Free(pCur, "..\\Source\\D2Client\\UI\\dialog.cpp", 0x336, 0);
        pCur = pPrev;
    }
    g_pActiveDialog = nullptr;
}

// 1.12a: 0x6FB134E0 (register: eax=pDialog, edi=nKey)
int32_t __fastcall sub_6FB134E0(D2DialogStrc* pDialog, int nKey)
{
    if (pDialog->bIgnoreMenuHotkeys) {
        return 0;
    }
    if (CMDTBL_HasKeyBound(5, 1) && CMDTBL_GetBoundKey(5, 1) == nKey) {
        return 1;
    }
    if (CMDTBL_HasKeyBound(5, 0) && CMDTBL_GetBoundKey(5, 0) == nKey) {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB13540 (__stdcall)
void __stdcall sub_6FB13540(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (pDialog && pDialog->dwCreationTick + 80 > GetTickCount()) {
        *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
        *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    SMsgBreakHandlerChain(pMsg);
    pDialog = g_pActiveDialog;
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x1C2));
        exit(-1);
    }
    if (pDialog->dwFrameCount >= 5 && pDialog->pfnCancel) {
        pDialog->pfnCancel();
        UI_StartPanelCloseClickCooldown();
    }
}

// 1.12a: 0x6FB135D0 (__stdcall)
void __stdcall sub_6FB135D0(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (pDialog->bIgnoreMenuHotkeys || gaPanelStates[D2PANEL_CHAT] == 0) {
        sub_6FB13120(pMsg, pDialog->bCloseOnRightClick == 0);
    }
}

// 1.12a: 0x6FB13600 (__stdcall)
// 1.12a: 0x6FB13630 (continuation block of sub_6FB13600)
D2DialogStrc* __stdcall sub_6FB13600(
    int nAnchorX, int nAnchorY,
    int (__cdecl* pfnCancel)(void),
    int (__cdecl* pfnOutsideClick)(void),
    int bBlockUnhandledKeys,
    int bUseNonSelectableHeightAnchor,
    int bAutoCalcBounds,
    int nWidth, int nHeight,
    int bCloseOnRightClick,
    int bIgnoreMenuHotkeys)
{
    if (gaPanelStates[D2PANEL_AUTOMAP]) {
        WALL2_UnregisterMsgHandlerTable(5, WINDOW_GetWindow(), s_aAutomapArrowHandlers);
    }
    INV_ClearHoveredItemState();
    s_nDialogBorderPadX = 16;
    s_nDialogBorderPadY = 16;
    s_bDialogCursorInvalidated = 0;
    g_bLeftMouseHeld = 0;
    g_bLeftMouseRepeating = 0;
    g_bRightMouseHeld = 0;
    g_bRightMouseRepeating = 0;
    gnTooltipUnitGUID = 0;
    gnTooltipUnitType = 0;
    if (nAnchorX == 0) {
        return nullptr;
    }
    g_nActiveDialogCount++;
    D2DialogStrc* pDialog = static_cast<D2DialogStrc*>(
        FOG_Alloc(sizeof(D2DialogStrc), "..\\Source\\D2Client\\UI\\dialog.cpp", 0x3ED, 0));
    memset(pDialog, 0, sizeof(D2DialogStrc));
    pDialog->pPrevDialog = g_pActiveDialog;
    g_pActiveDialog = pDialog;

    pDialog->nAnchorX = nAnchorX;
    pDialog->nPosX = nAnchorX;
    pDialog->nAnchorY = nAnchorY;
    pDialog->nPosY = nAnchorY;
    pDialog->pfnCancel = pfnCancel;
    pDialog->pfnOutsideClick = pfnOutsideClick;
    pDialog->bBlockUnhandledKeys = bBlockUnhandledKeys;
    pDialog->bUseNonSelectableHeightAnchor = bUseNonSelectableHeightAnchor;
    pDialog->bAutoCalcBounds = bAutoCalcBounds;
    pDialog->nWidth = nWidth;
    pDialog->nHeight = nHeight;
    pDialog->bCloseOnRightClick = bCloseOnRightClick;
    pDialog->bIgnoreMenuHotkeys = bIgnoreMenuHotkeys;
    pDialog->nSelectedIndex = -1;
    pDialog->nHoveredIndex = -1;
    pDialog->nSelectionCursorMode = 2;
    pDialog->dwCreationTick = GetTickCount();

    g_bDialogOpen = 1;
    sub_6FB133B0();
    WALL2_RegisterMsgHandlerTable(14, WINDOW_GetWindow(), s_aDialogMsgHandlers);
    if (GAME_HasMouseWheelB()) {
        WALL2_RegisterMsgHandlerTable(1, WINDOW_GetWindow(), s_aDialogWheelHandlers);
    }
    g_nDialogModalRefCount++;
    if (g_nActiveDialogCount != 1) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x410));
        exit(-1);
    }
    return pDialog;
}

// 1.12a: 0x6FB13800 (__stdcall)
// 1.12a: 0x6FB13850 (continuation block of sub_6FB13800)
D2DialogStrc* __stdcall sub_6FB13800(
    int nPosX, int nPosY,
    int (__cdecl* pfnCancel)(void),
    int (__cdecl* pfnOutsideClick)(void),
    int nBgNameIdx,
    int bBlockUnhandledKeys)
{
    if (gaPanelStates[D2PANEL_AUTOMAP]) {
        WALL2_UnregisterMsgHandlerTable(5, WINDOW_GetWindow(), s_aAutomapArrowHandlers);
    }
    s_nDialogBorderPadX = 16;
    s_nDialogBorderPadY = 16;
    s_bDialogCursorInvalidated = 0;
    INV_ClearHoveredItemState();
    g_bLeftMouseHeld = 0;
    g_bLeftMouseRepeating = 0;
    g_bRightMouseHeld = 0;
    g_bRightMouseRepeating = 0;
    gnTooltipUnitGUID = 0;
    gnTooltipUnitType = 0;
    if (nPosX == 0 || !pfnCancel) {
        return nullptr;
    }
    g_nActiveDialogCount++;
    D2DialogStrc* pDialog = static_cast<D2DialogStrc*>(
        FOG_Alloc(sizeof(D2DialogStrc), "..\\Source\\D2Client\\UI\\dialog.cpp", 0x399, 0));
    memset(pDialog, 0, sizeof(D2DialogStrc));
    pDialog->pPrevDialog = g_pActiveDialog;
    g_pActiveDialog = pDialog;

    pDialog->nSelectedIndex = -1;
    pDialog->nHoveredIndex = -1;
    pDialog->nSelectionCursorMode = 2;
    pDialog->pfnCancel = pfnCancel;
    pDialog->pfnOutsideClick = pfnOutsideClick;
    pDialog->dwCreationTick = GetTickCount();
    pDialog->bBlockUnhandledKeys = bBlockUnhandledKeys;
    pDialog->bHasBackgroundCel = 1;

    char szPath[260];
    wsprintfA(szPath, "%s\\ui\\menu\\%s", "DATA\\GLOBAL", g_apszDialogBgNames[nBgNameIdx]);
    pDialog->pBackgroundCel = ARCHIVE_LoadCellFileDefault(szPath, 0);

    D2GfxDataStrc gfxData = {};
    gfxData.pCellFile = pDialog->pBackgroundCel;
    void* pCelHandle = D2CMP_CelGetHandle(&gfxData);
    if (!pCelHandle) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x3B6));
        exit(-1);
    }
    pDialog->nWidth = D2CMP_CelGetWidth(pCelHandle);
    pDialog->nHeight = D2CMP_CelGetHeight(pCelHandle);
    pDialog->nAnchorX = nPosX;
    pDialog->nAnchorY = nPosY;
    pDialog->nPosX = nPosX;
    pDialog->nPosY = nPosY;

    g_bDialogOpen = 1;
    sub_6FB133B0();
    WALL2_RegisterMsgHandlerTable(14, WINDOW_GetWindow(), s_aDialogMsgHandlers);
    if (GAME_HasMouseWheelB()) {
        WALL2_RegisterMsgHandlerTable(1, WINDOW_GetWindow(), s_aDialogWheelHandlers);
    }
    g_nDialogModalRefCount++;
    if (g_nActiveDialogCount != 1) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x3CE));
        exit(-1);
    }
    return pDialog;
}

// 1.12a: 0x6FB13AA0 (__stdcall)
void __stdcall sub_6FB13AA0(void* pMsg)
{
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    SMsgBreakHandlerChain(pMsg);
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x2C8));
        exit(-1);
    }
    if (pDialog->dwFrameCount < 5 || sub_6FB134E0(pDialog, VK_RETURN)) {
        return;
    }
    if (pDialog->nSelectedIndex >= 0 && pDialog->nSelectedIndex < pDialog->nLineCount) {
        auto pfnAction = pDialog->aLines[pDialog->nSelectedIndex].pfnAction;
        if (pfnAction && pfnAction() == 1) {
            UI_StartPanelCloseClickCooldown();
        }
    } else {
        sub_6FB12C40(pDialog);
    }
}

// 1.12a: 0x6FB13B40 (register: eax=pDialog, stack: pMsg)
int32_t __stdcall sub_6FB13B40(D2DialogStrc* pDialog, void* pMsg)
{
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0xE1));
        exit(-1);
    }
    int nPrevSelected = pDialog->nSelectedIndex;
    int nHitLine = sub_6FB12BB0(pDialog, pMsg);
    pDialog->nSelectedIndex = nHitLine;
    pDialog->nHoveredIndex = nHitLine;
    if (nHitLine != nPrevSelected && nHitLine != -1) {
        SOUND_PlaySoundEffect(1, nullptr, 0, 0, 0);
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB13BB0 (register: edi=pDialog, ecx=nDelta)
void __fastcall sub_6FB13BB0(D2DialogStrc* pDialog, int nDelta)
{
    if (pDialog->nSelectableLineCount <= 1) {
        return;
    }
    int nCur = pDialog->nSelectedIndex;
    do {
        nCur += nDelta;
        if (nCur >= pDialog->nLineCount) {
            nCur = 0;
        } else if (nCur < 0) {
            nCur = pDialog->nLineCount - 1;
        }
    } while (!pDialog->aLines[nCur].bSelectable);

    SOUND_PlaySoundEffect(1, nullptr, 0, 0, 0);
    pDialog->nSelectedIndex = nCur;
}

// 1.12a: 0x6FB13C00 (__stdcall)
void __stdcall sub_6FB13C00(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x2EE));
        exit(-1);
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnKeyDown(pNode->pWidget, pMsg)) {
            return;
        }
    }
    int nKey = *(int32_t*)((uint8_t*)pMsg + 0x08);
    if (sub_6FB134E0(pDialog, nKey)) {
        return;
    }
    if (nKey == VK_RETURN) {
        sub_6FB13AA0(pMsg);
    } else if (nKey == VK_ESCAPE) {
        sub_6FB13540(pMsg);
    } else if (pDialog->bBlockUnhandledKeys == 1) {
        *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
        *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
        SMsgBreakHandlerChain(pMsg);
    }
}

// 1.12a: 0x6FB13CB0 (__stdcall)
void __stdcall sub_6FB13CB0(void* pMsg)
{
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    SMsgBreakHandlerChain(pMsg);
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x290));
        exit(-1);
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnDownArrow(pNode->pWidget, pMsg) == 1) {
            return;
        }
    }
    sub_6FB13BB0(pDialog, 1);
}

// 1.12a: 0x6FB13D30 (__stdcall)
void __stdcall sub_6FB13D30(void* pMsg)
{
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    SMsgBreakHandlerChain(pMsg);
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x272));
        exit(-1);
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnUpArrow(pNode->pWidget, pMsg) == 1) {
            return;
        }
    }
    sub_6FB13BB0(pDialog, -1);
}

// 1.12a: 0x6FB13DB0 (__stdcall)
void __stdcall sub_6FB13DB0(void* pMsg)
{
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    SMsgBreakHandlerChain(pMsg);
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x255));
        exit(-1);
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnRightArrow(pNode->pWidget, pMsg) == 1) {
            return;
        }
    }
    sub_6FB13BB0(pDialog, 1);
}

// 1.12a: 0x6FB13E30 (__stdcall)
void __stdcall sub_6FB13E30(void* pMsg)
{
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    SMsgBreakHandlerChain(pMsg);
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x235));
        exit(-1);
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnLeftArrow(pNode->pWidget, pMsg) == 1) {
            return;
        }
    }
    sub_6FB13BB0(pDialog, -1);
}

// 1.12a: 0x6FB13EB0 (__stdcall)
void __stdcall sub_6FB13EB0(void* pMsg)
{
    int16_t nWheelDelta = *(int16_t*)((uint8_t*)pMsg + 0x0A);
    int nSteps = nWheelDelta / 120;
    if (nWheelDelta != 0 && nSteps != 0) {
        D2DialogStrc* pDialog = g_pActiveDialog;
        if (!pDialog) {
            FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x1F9));
            exit(-1);
        }
        for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
            if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnMouseWheel(pNode->pWidget, pMsg) == 1) {
                return;
            }
        }
        sub_6FB13BB0(pDialog, nSteps > 0 ? -1 : 1);
    }
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB13F60 (__stdcall)
void __stdcall sub_6FB13F60(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x15B));
        exit(-1);
    }
    if (pDialog->dwCreationTick + 80 > GetTickCount()) {
        *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
        *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnLButtonDown(pNode->pWidget, pMsg) == 1) {
            *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
            *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
            SMsgBreakHandlerChain(pMsg);
            return;
        }
    }
    pDialog->bMousePressedInDialog = 1;
    if (sub_6FB13B40(pDialog, pMsg)) {
        *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
        *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }
    int nMouseX = *(uint16_t*)((uint8_t*)pMsg + 0x0C);
    int nMouseY = *(uint16_t*)((uint8_t*)pMsg + 0x0E);
    if (nMouseX >= pDialog->nPosX - 4 && nMouseX <= pDialog->nPosX + pDialog->nWidth + 4 &&
        nMouseY >= pDialog->nPosY - 4 && nMouseY <= pDialog->nPosY + pDialog->nHeight + 4) {
        *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
        *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }
    if (gaPanelStates[D2PANEL_MINIPANEL] && D2Client_HitTestMiniPanel(nMouseX)) {
        return;
    }
    if (pDialog->pfnOutsideClick) {
        if (!pDialog->pfnOutsideClick()) {
            return;
        }
        if (gaPanelStates[D2PANEL_NEWSTATS] && UI_HitTestNewStatBtn640(nMouseX, gnViewportShiftMode, nMouseY)) {
            *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
            *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
            SMsgBreakHandlerChain(pMsg);
        }
        if (gaPanelStates[D2PANEL_NEWSKILLS] && UI_HitTestNewSkillBtn640(nMouseX, gnViewportShiftMode, nMouseY)) {
            *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
            *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
            SMsgBreakHandlerChain(pMsg);
        }
        return;
    }
    if (!pDialog->pfnCancel) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x1AA));
        exit(-1);
    }
    pDialog->pfnCancel();
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB14120 (__stdcall)
void __stdcall sub_6FB14120(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (pDialog->dwCreationTick + 80 > GetTickCount()) {
        return;
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnMouseMove(pNode->pWidget, pMsg) == 1) {
            *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
            *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
            SMsgBreakHandlerChain(pMsg);
            return;
        }
    }
    sub_6FB13B40(pDialog, pMsg);
}

// ============================================================================
// PART 2: UI\npcmenu.cpp (0x6FAC2190..0x6FACED40, 286 functions + 9 internal entries)
// ============================================================================

// --- Section 2A: Command-Line Parsing & Startup (0x6FAC2190..0x6FAC26C7) ---

// 1.12a: 0x6FAC2190 (__stdcall)
int __stdcall sub_6FAC2190(const char* szArg, int a2)
{
    char szUser[24];
    DWORD dwSize = sizeof(szUser);
    GetUserNameA(szUser, &dwSize);
    if (!SStrCmp(szUser, "rseis", 24)) {
        MessageBoxA(nullptr,
            "Available command line options:\n  -set[default command line]\t\tSet new default command line string\n  -lng[lngfile]\t\t\tUse \"lngfile\" for language resources\n",
            "Diablo II", MB_ICONASTERISK);
    }
    return 1;
}

// 1.12a: 0x6FAC2200 (__cdecl)
void __cdecl sub_6FAC2200()
{
    if (!s_pStartupPoolCheck) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0xB4));
        exit(-1);
    }
}

// 1.12a: 0x6FAC2260 (__stdcall)
int __stdcall sub_6FAC2260(void* pErr)
{
    FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0xD8));
    exit(-1);
    return 1;
}

// 1.12a: 0x6FAC22A0 (__stdcall)
int __stdcall sub_6FAC22A0(const char* szCmd, int a2)
{
    return sub_6FAC2190(szCmd, a2);
}

// 1.12a: 0x6FAC2350 (__stdcall)
int __stdcall sub_6FAC2350(int a1)
{
    return 1;
}

// 1.12a: 0x6FAC2360 (__stdcall)
int __stdcall sub_6FAC2360(int a1)
{
    return 1;
}

// 1.12a: 0x6FAC2370 (__stdcall)
int __stdcall sub_6FAC2370(const char* szArg, int a2)
{
    s_bCmdLineFlagSet = 1;
    return 1;
}

// 1.12a: 0x6FAC23A0 (register: eax=szCmdLine)
void __fastcall sub_6FAC23A0(const char* szCmdLine)
{
    SCmdRegisterArgList(s_aCmdLineArgList, 0x2F);
    char szDefaultCmd[256] = { 0 };
    if (SRegLoadString("Diablo II", "Default Command Line", 0, szDefaultCmd, sizeof(szDefaultCmd))) {
        SCmdProcess(szDefaultCmd, 0, nullptr, 0);
    }
    if (szCmdLine) {
        SCmdProcess(szCmdLine, 0, (void*)sub_6FAC2260, 0);
    }
}

// 1.12a: 0x6FAC2480 (__stdcall)
void __stdcall sub_6FAC2480(int nSeed, int bSetSeed)
{
    if (bSetSeed) {
        GAME_SetInitSeed(nSeed);
    }
}

// 1.12a: 0x6FAC24C0 (__fastcall)
int __fastcall sub_6FAC24C0(const char* szCmdLine, int nMode)
{
    FOG_SetMemoryPoolSystemMode(1);
    UnimplementedOrdinal_10144(1);
    sub_6FAC23A0(szCmdLine);
    return 1;
}

// 1.12a: 0x6FAC2630 (__cdecl)
int __cdecl sub_6FAC2630()
{
    int nRes = D2GFX_GetResolutionMode();
    return D2Win_10005(nRes != 0, 0, 0, nullptr);
}

// --- Section 2B: Client Missile & Unit Find Helpers (0x6FAC26D0..0x6FAC3D37) ---

// 1.12a: 0x6FAC26D0 (register: eax=pUnit)
void __fastcall sub_6FAC26D0(D2UnitStrc* pUnit)
{
    s_dwMissileSyncEndTick = FOG_10055_GetSyncTime() + 500;
}

// 1.12a: 0x6FAC26F0 (register: eax=pUnit)
void __fastcall sub_6FAC26F0(D2UnitStrc* pUnit)
{
    if (pUnit) {
        *(uint32_t*)((uint8_t*)pUnit + 0xC4) |= 0x40;
    }
}

// 1.12a: 0x6FAC2730 (__stdcall)
int __stdcall sub_6FAC2730(D2UnitStrc* pUnit)
{
    return pUnit != nullptr;
}

// 1.12a: 0x6FAC2760 (register: eax=pUnit)
void __fastcall sub_6FAC2760(D2UnitStrc* pUnit)
{
    if (pUnit) {
        *(uint32_t*)((uint8_t*)pUnit + 0xC4) &= ~0x40;
    }
}

// 1.12a: 0x6FAC27A0 (register: eax=pMissile)
void __fastcall sub_6FAC27A0(D2UnitStrc* pMissile)
{
    if (pMissile) {
        MISSILE_SetTargetX(pMissile, 0);
        MISSILE_SetTargetY(pMissile, 0);
    }
}

// 1.12a: 0x6FAC2850 (register: eax=pMissile)
void __fastcall sub_6FAC2850(D2UnitStrc* pMissile)
{
    if (pMissile) {
        MISSILE_SetTargetX(pMissile, 1);
    }
}

// 1.12a: 0x6FAC28E0 (register: eax=pMissile)
void __fastcall sub_6FAC28E0(D2UnitStrc* pMissile)
{
    if (!pMissile) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x114));
        exit(-1);
    }
}

// 1.12a: 0x6FAC2940 (__cdecl)
int __cdecl sub_6FAC2940()
{
    return static_cast<int>(s_dwNpcSyncTime);
}

// 1.12a: 0x6FAC2950 (__cdecl)
void __cdecl sub_6FAC2950()
{
    s_dwNpcSyncTime = FOG_10055_GetSyncTime();
}

// 1.12a: 0x6FAC2970 (__cdecl)
void __cdecl sub_6FAC2970()
{
    s_dwNpcSyncTime = 0;
}

// 1.12a: 0x6FAC2980 (__stdcall)
int __stdcall sub_6FAC2980(D2UnitStrc* pUnit, int nX, int nY)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x14B));
        exit(-1);
    }
    return 1;
}

// 1.12a: 0x6FAC2AA0 (register: eax=pUnit)
D2UnitStrc* __fastcall sub_6FAC2AA0(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    return pRoom ? UNITFINDS_FindUnitInNeighboredRooms(pRoom, 1, 243) : nullptr;
}

// 1.12a: 0x6FAC2B30 (__cdecl)
void __cdecl sub_6FAC2B30()
{
    s_nNpcMissileFlag = 0;
}

// 1.12a: 0x6FAC2B50 (register: eax=pUnit)
void __fastcall sub_6FAC2B50(D2UnitStrc* pUnit)
{
    if (pUnit && *(void**)((uint8_t*)pUnit + 0x2C)) {
        PATH_GetXPosition(*(void**)((uint8_t*)pUnit + 0x2C));
        PATH_GetYPosition(*(void**)((uint8_t*)pUnit + 0x2C));
    }
}

// 1.12a: 0x6FAC2C10 (register: eax=pUnit)
void __fastcall sub_6FAC2C10(D2UnitStrc* pUnit)
{
    sub_6FAC2B50(pUnit);
}

// 1.12a: 0x6FAC2CD0 (register: eax=pUnit)
void __fastcall sub_6FAC2CD0(D2UnitStrc* pUnit)
{
    sub_6FAC2B50(pUnit);
}

// 1.12a: 0x6FAC2D90 (register: eax=pUnit)
void __fastcall sub_6FAC2D90(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom) {
        UNITFINDS_FindUnitInNeighboredRooms(pRoom, 2, 398);
    }
}

// 1.12a: 0x6FAC2F20 (register: eax=pUnit)
void __fastcall sub_6FAC2F20(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom) {
        UNITFINDS_FindUnitInNeighboredRooms(pRoom, 2, 398);
    }
}

// 1.12a: 0x6FAC3020 (register: eax=pUnit)
void __fastcall sub_6FAC3020(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom) {
        UNITFINDS_FindUnitInNeighboredRooms(pRoom, 1, 242);
    }
}

// 1.12a: 0x6FAC30A0 (register: eax=pUnit)
void __fastcall sub_6FAC30A0(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom && UNITFINDS_FindUnitInNeighboredRooms(pRoom, 1, 242)) {
        MISSILE_SetTargetX(pUnit, 1);
    }
}

// 1.12a: 0x6FAC3100 (register: eax=pUnit)
void __fastcall sub_6FAC3100(D2UnitStrc* pUnit)
{
    if (pUnit && *(void**)((uint8_t*)pUnit + 0x2C)) {
        MISSILE_SetTargetX(pUnit, PATH_GetXPosition(*(void**)((uint8_t*)pUnit + 0x2C)));
    }
}

// 1.12a: 0x6FAC3180 (register: eax=pUnit)
void __fastcall sub_6FAC3180(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom) {
        UNITFINDS_FindUnitInNeighboredRooms(pRoom, 1, 544);
    }
}

// 1.12a: 0x6FAC3230 (register: eax=pUnit)
void __fastcall sub_6FAC3230(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom) {
        UNITFINDS_FindUnitInNeighboredRooms(pRoom, 1, 543);
    }
}

// 1.12a: 0x6FAC32C0 (register: eax=pUnit)
void __fastcall sub_6FAC32C0(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x320));
        exit(-1);
    }
}

// 1.12a: 0x6FAC33E0 (register: eax=pUnit)
int __fastcall sub_6FAC33E0(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    return pRoom ? DUNGEON_GetLevelIdFromRoom(pRoom) : 0;
}

// 1.12a: 0x6FAC3430 (thunk)
void __cdecl sub_6FAC3430()
{
}

// 1.12a: 0x6FAC3440 (register: eax=pUnit)
void __fastcall sub_6FAC3440(D2UnitStrc* pUnit)
{
    if (pUnit) {
        UNITS_GetRoom(pUnit);
    }
}

// 1.12a: 0x6FAC35A0 (register: eax=pUnit)
int __fastcall sub_6FAC35A0(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    return pRoom ? DUNGEON_GetLevelIdFromRoom(pRoom) : 0;
}

// 1.12a: 0x6FAC3600 (register: eax=pUnit)
void __fastcall sub_6FAC3600(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom) {
        UNITFINDS_FindUnitInNeighboredRooms(pRoom, 1, 496);
    }
}

// 1.12a: 0x6FAC36D0 (register: eax=pUnit)
void __fastcall sub_6FAC36D0(D2UnitStrc* pUnit)
{
}

// 1.12a: 0x6FAC3730 (register: eax=pUnit)
void __fastcall sub_6FAC3730(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom) {
        UNITFINDS_FindUnitInNeighboredRooms(pRoom, 1, 544);
    }
}

// 1.12a: 0x6FAC3880 (register: eax=pUnit)
void __fastcall sub_6FAC3880(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom && UNITFINDS_FindUnitInNeighboredRooms(pRoom, 1, 544)) {
        MISSILE_SetTargetX(pUnit, 1);
        MISSILE_SetTargetY(pUnit, 1);
    }
}

// 1.12a: 0x6FAC3920 (register: eax=pUnit)
int __fastcall sub_6FAC3920(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    return pRoom ? DUNGEON_GetLevelIdFromRoom(pRoom) : 0;
}

// 1.12a: 0x6FAC3980 (register: eax=pUnit)
void __fastcall sub_6FAC3980(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom) {
        UNITFINDS_FindUnitInNeighboredRooms(pRoom, 2, 473);
    }
}

// 1.12a: 0x6FAC3A80 (__stdcall)
int __stdcall sub_6FAC3A80(D2RoomStrc* pRoom)
{
    int aCoords[8];
    DUNGEON_GetRoomCoordinates(pRoom, aCoords);
    return COLLISION_CheckMask(pRoom, aCoords[0], aCoords[1], 1);
}

// 1.12a: 0x6FAC3BE0 (register: eax=pRoom)
int __fastcall sub_6FAC3BE0(D2RoomStrc* pRoom)
{
    if (!pRoom) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x5A0));
        exit(-1);
    }
    return DUNGEON_GetLevelIdFromRoom(pRoom);
}

// 1.12a: 0x6FAC3C20 (register: eax=pAct, edx=nX, ecx=nY)
int __fastcall sub_6FAC3C20(int nY, int nX, void* pAct)
{
    D2RoomStrc* pRoom = DUNGEON_GetRoomFromAct(pAct, nX, nY);
    return pRoom ? DUNGEON_GetLevelIdFromRoom(pRoom) : 0;
}

// 1.12a: 0x6FAC3C50 (__cdecl)
void __cdecl sub_6FAC3C50()
{
}

// 1.12a: 0x6FAC3CC0 (__cdecl)
void __cdecl sub_6FAC3CC0() {}

// 1.12a: 0x6FAC3CD0 (__cdecl)
void __cdecl sub_6FAC3CD0() {}

// 1.12a: 0x6FAC3CE0 (__cdecl)
void __cdecl sub_6FAC3CE0() {}

// 1.12a: 0x6FAC3CF0 (__cdecl)
void __cdecl sub_6FAC3CF0() {}

// 1.12a: 0x6FAC3D00 (__cdecl)
void __cdecl sub_6FAC3D00() {}

// 1.12a: 0x6FAC3D10 (__cdecl)
void __cdecl sub_6FAC3D10() {}

// 1.12a: 0x6FAC3D20 (__cdecl)
int __cdecl sub_6FAC3D20() { return s_nWeatherState; }

// 1.12a: 0x6FAC3D30 (__cdecl)
void __cdecl sub_6FAC3D30(int nVal) { s_nWeatherState = nVal; }

// --- Section 2C: Environment Weather & Rain/Snow Particles (0x6FAC3D40..0x6FAC5B4F) ---

// 1.12a: 0x6FAC3D40 (__stdcall)
void __stdcall sub_6FAC3D40(void* pParticle)
{
    if (D2GFX_CheckPerspective()) {
        UnimplementedOrdinal_10065(s_hEnvParticles, pParticle);
    }
}

// 1.12a: 0x6FAC3E90 (__cdecl)
void __cdecl sub_6FAC3E90()
{
    if (!UnimplementedOrdinal_10063(s_hEnvParticles)) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0xCF));
        exit(-1);
    }
}

// 1.12a: 0x6FAC3ED0 (__cdecl)
int __cdecl sub_6FAC3ED0()
{
    return s_nWeatherMode;
}

// 1.12a: 0x6FAC3EE0 (__cdecl)
void __cdecl sub_6FAC3EE0()
{
    UnimplementedOrdinal_10061(s_hEnvParticles);
    UnimplementedOrdinal_10061(s_hEnvSplashes);
    UnimplementedOrdinal_10061(s_hEnvBubbles);
}

// 1.12a: 0x6FAC3F10 (__cdecl)
void __cdecl sub_6FAC3F10()
{
    UnimplementedOrdinal_10060(300, 0x28);
    UnimplementedOrdinal_10062(s_hEnvParticles, "Environment Particles", 300, 0x28);
    UnimplementedOrdinal_10062(s_hEnvSplashes, "Environment Splashes", 100, 0x28);
    UnimplementedOrdinal_10062(s_hEnvBubbles, "Environment Bubbles", 50, 0x28);
}

// 1.12a: 0x6FAC3FB0 (register: eax=pSplash)
void __fastcall sub_6FAC3FB0(void* pSplash)
{
    if (D2GFX_CheckPerspective()) {
        int nX = 0, nY = 0;
        SCALE_AdjustPerspectivePosition(*(int*)pSplash, *((int*)pSplash + 1), 0, &nX, &nY);
    }
}

// 1.12a: 0x6FAC41A0 (register: eax=pBubble)
void __fastcall sub_6FAC41A0(void* pBubble)
{
    int nX = *(int*)pBubble;
    int nY = *((int*)pBubble + 1);
    D2GFX_DrawBox(nX, nY, nX + 2, nY + 2, 0xFF, 2);
}

// 1.12a: 0x6FAC4210 (register: eax=pParticle)
void __fastcall sub_6FAC4210(void* pParticle)
{
    int nX = *(int*)pParticle;
    int nY = *((int*)pParticle + 1);
    D2GFX_DrawLine(nX, nY, nX + 2, nY + 8, 0xD0, 0x80);
}

// 1.12a: 0x6FAC4460 (__cdecl)
void __cdecl sub_6FAC4460()
{
    if (s_pRainCel) {
        D2CMP_CelFileFreeHardware(s_pRainCel);
        FOG_Free(s_pRainCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
        s_pRainCel = nullptr;
    }
}

// 1.12a: 0x6FAC44E0 (__cdecl)
void __cdecl sub_6FAC44E0()
{
    D2UnitStrc* pPlayer = sub_6FACE310();
    if (pPlayer) {
        D2RoomStrc* pRoom = UNITS_GetRoom(pPlayer);
        if (pRoom) {
            int nLevel = DUNGEON_GetLevelIdFromRoom(pRoom);
            DATATBLS_GetRainFromLevelsTxt(nLevel);
            DATATBLS_GetMudFromLevelsTxt(nLevel);
        }
    }
}

// 1.12a: 0x6FAC4560 (__cdecl)
void __cdecl sub_6FAC4560()
{
    sub_6FAC44E0();
}

// 1.12a: 0x6FAC4590 (__stdcall)
void __stdcall sub_6FAC4590(int bForce)
{
    sub_6FAC44E0();
}

// 1.12a: 0x6FAC4670 (__cdecl)
void __cdecl sub_6FAC4670()
{
    sub_6FAC44E0();
}

// 1.12a: 0x6FAC47D0 (__stdcall)
void __stdcall sub_6FAC47D0(int a1)
{
    UnimplementedOrdinal_10064(s_hEnvParticles, nullptr);
}

// 1.12a: 0x6FAC4A10 (__stdcall)
void __stdcall sub_6FAC4A10(int nX, int nY, int a3, int a4)
{
    UnimplementedOrdinal_10064(s_hEnvSplashes, nullptr);
}

// 1.12a: 0x6FAC4B40 (__stdcall)
void __stdcall sub_6FAC4B40(int nX, int nY, int a3, int a4)
{
    UnimplementedOrdinal_10064(s_hEnvBubbles, nullptr);
}

// 1.12a: 0x6FAC4CB0 (__cdecl)
void __cdecl sub_6FAC4CB0()
{
    sub_6FAC44E0();
}

// 1.12a: 0x6FAC4F00 (__cdecl)
void __cdecl sub_6FAC4F00()
{
}

// 1.12a: 0x6FAC5030 (__stdcall)
void __stdcall sub_6FAC5030(void* pParticle)
{
    UnimplementedOrdinal_10065(s_hEnvParticles, pParticle);
}

// 1.12a: 0x6FAC51F0 (__stdcall)
void __stdcall sub_6FAC51F0(void* pSplash)
{
    if (!pSplash) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x3E4));
        exit(-1);
    }
}

// 1.12a: 0x6FAC53C0 (register: eax=pBubble)
void __fastcall sub_6FAC53C0(void* pBubble)
{
    if (!pBubble) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x42A));
        exit(-1);
    }
}

// 1.12a: 0x6FAC5410 (__cdecl)
void __cdecl sub_6FAC5410()
{
    sub_6FAC44E0();
}

// 1.12a: 0x6FAC5540 (__stdcall)
void __stdcall sub_6FAC5540(int nCount)
{
    UnimplementedOrdinal_10063(s_hEnvParticles);
}

// 1.12a: 0x6FAC55C0 (__cdecl)
void __cdecl sub_6FAC55C0()
{
    UnimplementedOrdinal_10063(s_hEnvParticles);
}

// 1.12a: 0x6FAC5760 (__cdecl)
void __cdecl sub_6FAC5760()
{
    UnimplementedOrdinal_10063(s_hEnvSplashes);
}

// 1.12a: 0x6FAC57F0 (__cdecl)
void __cdecl sub_6FAC57F0()
{
    UnimplementedOrdinal_10063(s_hEnvParticles);
    UnimplementedOrdinal_10063(s_hEnvSplashes);
}

// 1.12a: 0x6FAC5820 (__cdecl)
void __cdecl sub_6FAC5820()
{
    char szPath[260];
    wsprintfA(szPath, "%s\\UncompOverlays\\Rain1", "DATA\\GLOBAL");
    s_pRainCel = ARCHIVE_LoadCellFileDefault(szPath, 0);
}

// --- Section 2D: Client Pathfinding, Distance & Collision Helpers (0x6FAC5B50..0x6FAC7471) ---

// 1.12a: 0x6FAC5B50 (register: eax=pUnit)
int __fastcall sub_6FAC5B50(D2UnitStrc* pUnit)
{
    return pUnit ? *(int32_t*)((uint8_t*)pUnit + 0x10) : 0;
}

// 1.12a: 0x6FAC5B80 (register: eax=pUnit)
int __fastcall sub_6FAC5B80(D2UnitStrc* pUnit)
{
    return pUnit ? *(int32_t*)((uint8_t*)pUnit + 0x04) : -1;
}

// 1.12a: 0x6FAC5BA0 (__stdcall)
int __stdcall sub_6FAC5BA0(D2UnitStrc* pUnit)
{
    return pUnit != nullptr;
}

// 1.12a: 0x6FAC5BD0 (__stdcall)
int __stdcall sub_6FAC5BD0(D2UnitStrc* pUnit, D2RoomStrc* pTargetRoom)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom) return 0;
    D2RoomStrc** ppRooms = nullptr;
    int nNumRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRooms, &nNumRooms);
    for (int i = 0; i < nNumRooms; ++i) {
        if (ppRooms[i] == pTargetRoom) return 1;
    }
    return 0;
}

// 1.12a: 0x6FAC5C60 (register: eax=pUnit1, edx=pUnit2)
int __fastcall sub_6FAC5C60(D2UnitStrc* pUnit2, D2UnitStrc* pUnit1)
{
    return UNITS_GetDistanceToOtherUnit(pUnit1, pUnit2);
}

// 1.12a: 0x6FAC5D30 (__stdcall)
int __stdcall sub_6FAC5D30(D2UnitStrc* pUnit)
{
    extern D2UnitStrc* __cdecl sub_6FACE310();
    D2UnitStrc* pPlayer = sub_6FACE310();
    return (pPlayer && pUnit) ? UNITS_GetDistanceToOtherUnit(pPlayer, pUnit) : 0x7FFFFFFF;
}

// 1.12a: 0x6FAC5D80 (__stdcall)
int __stdcall sub_6FAC5D80(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2)
{
    return (pUnit1 && pUnit2) ? UNITS_GetDistanceToOtherUnit(pUnit1, pUnit2) : 0x7FFFFFFF;
}

// 1.12a: 0x6FAC5E00 (__stdcall)
int __stdcall sub_6FAC5E00(D2UnitStrc* pUnit, int nMaxDist)
{
    return sub_6FAC5D30(pUnit) <= nMaxDist;
}

// 1.12a: 0x6FAC5E60 (__stdcall)
int __stdcall sub_6FAC5E60(D2UnitStrc* pUnit, uint16_t nMask)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom) return 0;
    POINT pt;
    UNITS_GetCoords(pUnit, &pt);
    return COLLISION_CheckMaskWithSize(pRoom, pt.x, pt.y, 1, nMask);
}

// 1.12a: 0x6FAC5F90 (register: eax=pUnit)
int __fastcall sub_6FAC5F90(D2UnitStrc* pUnit)
{
    return pUnit ? *(int32_t*)((uint8_t*)pUnit + 0x00) : -1;
}

// 1.12a: 0x6FAC5FC0 (register: eax=pMissile)
void __fastcall sub_6FAC5FC0(D2UnitStrc* pMissile)
{
    if (pMissile) {
        MISSILE_SetTotalFrames(pMissile, 16);
        MISSILE_SetCurrentFrame(pMissile, 0);
    }
}

// 1.12a: 0x6FAC6220 (register: eax=pUnit)
int __fastcall sub_6FAC6220(D2UnitStrc* pUnit)
{
    return (pUnit && *(void**)((uint8_t*)pUnit + 0x2C)) ? PATH_GetXPosition(*(void**)((uint8_t*)pUnit + 0x2C)) : 0;
}

// 1.12a: 0x6FAC62B0 (register: eax=pUnit)
int __fastcall sub_6FAC62B0(D2UnitStrc* pUnit)
{
    return (pUnit && *(void**)((uint8_t*)pUnit + 0x2C)) ? PATH_GetYPosition(*(void**)((uint8_t*)pUnit + 0x2C)) : 0;
}

// 1.12a: 0x6FAC6340 (__stdcall)
int __stdcall sub_6FAC6340(D2UnitStrc* pUnit, int nType)
{
    void* pPath = pUnit ? *(void**)((uint8_t*)pUnit + 0x2C) : nullptr;
    return pPath ? PATH_GetMaxDistance(pPath) : 0;
}

// 1.12a: 0x6FAC6390 (__stdcall)
int __stdcall sub_6FAC6390(D2UnitStrc* pUnit, int nX, int nY)
{
    void* pPath = pUnit ? *(void**)((uint8_t*)pUnit + 0x2C) : nullptr;
    return pPath ? PATH_GetType(pPath) : 0;
}

// 1.12a: 0x6FAC64B0 (__stdcall)
int __stdcall sub_6FAC64B0(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int a3, int a4)
{
    return sub_6FAC5D80(pUnit1, pUnit2);
}

// 1.12a: 0x6FAC6650 (__stdcall)
int __stdcall sub_6FAC6650(D2UnitStrc* pUnit, int a2)
{
    return sub_6FAC6340(pUnit, a2);
}

// 1.12a: 0x6FAC66C0 (register: eax=pUnit)
int __fastcall sub_6FAC66C0(D2UnitStrc* pUnit)
{
    return pUnit ? !MONSTERS_IsDead(pUnit) : 0;
}

// 1.12a: 0x6FAC67B0 (register: eax=pUnit)
int __fastcall sub_6FAC67B0(D2UnitStrc* pUnit)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom || !*(void**)((uint8_t*)pUnit + 0x2C)) return 0;
    int nX = PATH_GetXPosition(*(void**)((uint8_t*)pUnit + 0x2C));
    int nY = PATH_GetYPosition(*(void**)((uint8_t*)pUnit + 0x2C));
    return COLLISION_CheckMask(pRoom, nX, nY, 1);
}

// 1.12a: 0x6FAC6900 (__stdcall)
int __stdcall sub_6FAC6900(D2UnitStrc* pUnit)
{
    return pUnit ? !MONSTERS_IsDead(pUnit) : 0;
}

// 1.12a: 0x6FAC6990 (__stdcall)
int __stdcall sub_6FAC6990(D2UnitStrc* pUnit)
{
    return pUnit != nullptr;
}

// 1.12a: 0x6FAC6B10 (__stdcall)
int __stdcall sub_6FAC6B10(D2UnitStrc* pUnit)
{
    return pUnit ? !MONSTERS_IsDead(pUnit) : 0;
}

// 1.12a: 0x6FAC6BB0 (register: eax=pUnit)
int __fastcall sub_6FAC6BB0(D2UnitStrc* pUnit)
{
    return sub_6FAC5D30(pUnit);
}

// 1.12a: 0x6FAC6DC0 (__stdcall)
int __stdcall sub_6FAC6DC0(int a1, int a2)
{
    return 1;
}

// 1.12a: 0x6FAC6E30 (register: eax=pUnit)
void __fastcall sub_6FAC6E30(D2UnitStrc* pUnit)
{
    if (pUnit && *(void**)((uint8_t*)pUnit + 0x2C)) {
        PATH_SetFootprintCollisionMask(*(void**)((uint8_t*)pUnit + 0x2C), 0);
    }
}

// 1.12a: 0x6FAC6F20 (register: eax=pPath)
int __fastcall sub_6FAC6F20(void* pPath)
{
    return pPath != nullptr;
}

// 1.12a: 0x6FAC7150 (__stdcall)
int __stdcall sub_6FAC7150(D2RoomStrc* pRoom, uint16_t nMask)
{
    int aCoords[8];
    DUNGEON_GetRoomCoordinates(pRoom, aCoords);
    return COLLISION_CheckMaskWithSize(pRoom, aCoords[0], aCoords[1], 1, nMask);
}

// 1.12a: 0x6FAC7320 (__stdcall)
D2LevelsTxt* __stdcall sub_6FAC7320(D2RoomStrc* pRoom)
{
    int nLevelId = pRoom ? DUNGEON_GetLevelIdFromRoom(pRoom) : 0;
    return nLevelId ? DATATBLS_GetLevelsTxtRecord(nLevelId) : nullptr;
}

// --- Section 2E: Game Loop, MPQ & Registry Setup (0x6FAC7480..0x6FAC7CD8) ---

// 1.12a: 0x6FAC7480 (__cdecl)
int __cdecl sub_6FAC7480() { return s_nGameLoopMode; }

// 1.12a: 0x6FAC7490 (__cdecl)
void __cdecl sub_6FAC7490(int nVal) { s_nGameLoopMode = nVal; }

// 1.12a: 0x6FAC74A0 (register: ecx=szPath)
int __fastcall sub_6FAC74A0(const char* szPath)
{
    return UnimplementedOrdinal_10191(szPath);
}

// 1.12a: 0x6FAC74D0 (__cdecl)
void __cdecl sub_6FAC74D0(int nVal) { s_nMpqCheckState = nVal; }

// 1.12a: 0x6FAC74E0 (register: ecx=szFileName)
int __fastcall sub_6FAC74E0(const char* szFileName)
{
    HANDLE hFile = nullptr;
    if (SFileOpenFileEx(nullptr, szFileName, 0, &hFile)) {
        SFileCloseFile(hFile);
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FAC7510 (__cdecl)
int __cdecl sub_6FAC7510() { return s_nMpqCheckState; }

// 1.12a: 0x6FAC7540 (__cdecl)
int __cdecl sub_6FAC7540()
{
    char szLang[16] = { 0 };
    STRTABLE_GetLanguageInfo(STRTABLE_GetLanguage(), szLang, 1);
    return 1;
}

// 1.12a: 0x6FAC7600 (__cdecl)
int __cdecl sub_6FAC7600()
{
    char szBuf[64] = { 0 };
    return SRegLoadString("Diablo II", "Aux Battle.net", 0, szBuf, sizeof(szBuf));
}

// 1.12a: 0x6FAC7770 (__stdcall)
int __stdcall sub_6FAC7770(int nMode)
{
    char szInstallPath[260];
    FOG_GetInstallPath(szInstallPath, sizeof(szInstallPath));
    return 1;
}

// 1.12a: 0x6FAC7C30 (__cdecl)
void __cdecl sub_6FAC7C30()
{
}

// 1.12a: 0x6FAC7C90 (__cdecl)
void __cdecl sub_6FAC7C90()
{
}

// --- Section 2F: Town NPC Interaction Menu, Trade/Repair/Gamble/Hire & Speech (0x6FAC7CE0..0x6FACDE83) ---

// 1.12a: 0x6FAC7CE0 (__cdecl)
void __cdecl sub_6FAC7CE0() {}

// 1.12a: 0x6FAC7CF0 (__cdecl)
int __cdecl sub_6FAC7CF0() { return g_bInteractingWithNpc; }

// 1.12a: 0x6FAC7D00 (__cdecl)
int __cdecl sub_6FAC7D00() { return g_nInteractingNpcGUID; }

// 1.12a: 0x6FAC7D10 (register: eax=bInteracting, ecx=nNpcGUID, edx=nNpcClassId)
void __fastcall sub_6FAC7D10(int nNpcGUID, int nNpcClassId, int bInteracting)
{
    g_bInteractingWithNpc = bInteracting;
    g_nInteractingNpcGUID = nNpcGUID;
    g_nInteractingNpcClassId = nNpcClassId;
}

// 1.12a: 0x6FAC7D50 (register: eax=pNpcUnit)
int __fastcall sub_6FAC7D50(D2UnitStrc* pNpcUnit)
{
    int nClassId = pNpcUnit ? *(int32_t*)((uint8_t*)pNpcUnit + 0x04) : -1;
    for (int i = 0; i < 48; ++i) {
        if (g_aNpcMenuDefs[i].nNpcClassId == nClassId) {
            return i;
        }
    }
    return -1;
}

// 1.12a: 0x6FAC7D90 (__cdecl)
void __cdecl sub_6FAC7D90() { s_bNpcQuestUpdated = 1; }

// 1.12a: 0x6FAC7DA0 (__cdecl)
int __cdecl sub_6FAC7DA0() { return s_bNpcQuestUpdated; }

// 1.12a: 0x6FAC7DB0 (register: eax=pBuffer)
void __fastcall sub_6FAC7DB0(uint8_t* pBuffer)
{
    QUESTRECORD_CopyBufferToRecord(g_pClientQuestRecord, pBuffer, 0x54, 1);
}

// 1.12a: 0x6FAC7DD0 (register: eax=nNpcClassId)
int __fastcall sub_6FAC7DD0(int nNpcClassId)
{
    return nNpcClassId == 148 || nNpcClassId == 177 || nNpcClassId == 255 || nNpcClassId == 405 || nNpcClassId == 513;
}

// 1.12a: 0x6FAC7DF0 (register: eax=nNpcClassId)
int __fastcall sub_6FAC7DF0(int nNpcClassId)
{
    return nNpcClassId == 154 || nNpcClassId == 178 || nNpcClassId == 253 || nNpcClassId == 405 || nNpcClassId == 511;
}

// 1.12a: 0x6FAC7E10 (register: eax=nNpcClassId)
int __fastcall sub_6FAC7E10(int nNpcClassId)
{
    return nNpcClassId == 147 || nNpcClassId == 199 || nNpcClassId == 254 || nNpcClassId == 405 || nNpcClassId == 514;
}

// 1.12a: 0x6FAC7E30 (__cdecl)
int __cdecl sub_6FAC7E30() { return 0; }

// 1.12a: 0x6FAC7E40 (register: eax=pItem)
int __fastcall sub_6FAC7E40(D2UnitStrc* pItem)
{
    if (!pItem) return 0;
    if (ITEMS_CheckIfThrowable(pItem) || ITEMS_HasDurability(pItem)) {
        return STATLIST_GetMaxDurabilityFromUnit(pItem) > 0;
    }
    return 0;
}

// 1.12a: 0x6FAC7ED0 (register: eax=pItem)
int __fastcall sub_6FAC7ED0(D2UnitStrc* pItem)
{
    return ITEMS_CheckItemFlag(pItem, 0x10, 0x397, "..\\Source\\D2Client\\UI\\npcmenu.cpp");
}

// 1.12a: 0x6FAC7EF0 (__cdecl)
int __cdecl sub_6FAC7EF0() { return 0; }

// 1.12a: 0x6FAC7F00 (__cdecl)
int __cdecl sub_6FAC7F00()
{
    return g_pNpcMainMenuDialog != nullptr || g_pNpcSubDialog != nullptr || g_pNpcPopupDialog != nullptr;
}

// 1.12a: 0x6FAC7F50 (register: eax=nNpcClassId)
int __fastcall sub_6FAC7F50(int nNpcClassId)
{
    for (int i = 0; i < 48; ++i) {
        if (g_aNpcMenuDefs[i].nNpcClassId == nNpcClassId) {
            return g_aNpcMenuDefs[i].bHasMenu;
        }
    }
    return 0;
}

// 1.12a: 0x6FAC7FC0 (__stdcall)
int __stdcall sub_6FAC7FC0(int nQuestId, int nState)
{
    if (!g_pClientQuestRecord) return 0;
    return QUESTRECORD_GetQuestState(g_pClientQuestRecord, nQuestId, nState);
}

// 1.12a: 0x6FAC8030 (__cdecl)
void __cdecl sub_6FAC8030()
{
    for (int i = 0; i < 45; ++i) {
        g_aNpcGossipDefs[i].bIntroShown = 0;
    }
}

// 1.12a: 0x6FAC8050 (__cdecl)
void __cdecl sub_6FAC8050() { s_nNpcTalkState = 0; }

// 1.12a: 0x6FAC8060 (__cdecl)
int __cdecl sub_6FAC8060() { return s_nNpcTalkState; }

// 1.12a: 0x6FAC8090 (__cdecl)
void __cdecl sub_6FAC8090(int nVal) { s_nNpcHireState = nVal; }

// 1.12a: 0x6FAC80A0 (__cdecl)
int __cdecl sub_6FAC80A0() { return s_nNpcHireState; }

// 1.12a: 0x6FAC80B0 (__cdecl)
int __cdecl sub_6FAC80B0() { return s_nNpcShopStateA; }

// 1.12a: 0x6FAC80C0 (__cdecl)
int __cdecl sub_6FAC80C0() { return s_nNpcShopStateB; }

// 1.12a: 0x6FAC80D0 (register: eax=nNpcClassId)
int __fastcall sub_6FAC80D0(int nNpcClassId)
{
    return nNpcClassId >= 0;
}

// 1.12a: 0x6FAC8130 (__cdecl)
void __cdecl sub_6FAC8130()
{
    s_nNpcMenuSubState = 0;
}

// 1.12a: 0x6FAC8150 (__cdecl)
int __cdecl sub_6FAC8150()
{
    int nVal = 0;
    SRegLoadValue("Diablo II", "PopupHireling", 0, &nVal);
    return nVal;
}

// 1.12a: 0x6FAC81B0 (__cdecl)
// 1.12a: 0x6FAC81E0 (internal entry point)
void __cdecl sub_6FAC81B0()
{
    s_bNpcSpeechActive = 0;
}

void __cdecl sub_6FAC81E0()
{
    s_bNpcSpeechActive = 1;
}

// 1.12a: 0x6FAC81F0 (__cdecl)
// 1.12a: 0x6FAC8270 (internal entry point)
int __cdecl sub_6FAC81F0()
{
    if (!g_pNpcTextHeader) return 0;
    return TEXT_GetStringIdOfMenuType1Or2Node(g_pNpcTextHeader, 0);
}

int __cdecl sub_6FAC8270()
{
    return sub_6FAC81F0();
}

// 1.12a: 0x6FAC82A0 (__cdecl)
void __cdecl sub_6FAC82A0()
{
    char szPath[260];
    wsprintfA(szPath, "%s\\ui\\panel\\buysell", "DATA\\GLOBAL");
    s_pBuySellCel = ARCHIVE_LoadCellFileDefault(szPath, 0);
    g_pClientQuestRecord = QUESTRECORD_AllocRecord(nullptr);
    g_pTempQuestRecord = QUESTRECORD_AllocRecord(nullptr);
}

// 1.12a: 0x6FAC8360 (register: eax=pNpcUnit)
void __fastcall sub_6FAC8360(D2UnitStrc* pNpcUnit)
{
    if (!pNpcUnit) return;
    D2InventoryStrc* pInv = *(D2InventoryStrc**)((uint8_t*)pNpcUnit + 0x60);
    if (pInv) {
        INVENTORY_RemoveInventoryItems(pInv);
    }
}

// 1.12a: 0x6FAC8410 (__cdecl)
void __cdecl sub_6FAC8410()
{
    if (g_pTradeNpcUnit) {
        sub_6FAC8360(g_pTradeNpcUnit);
        g_pTradeNpcUnit = nullptr;
    }
}

// 1.12a: 0x6FAC8450 (__cdecl)
void __cdecl sub_6FAC8450()
{
    if (g_pNpcMainMenuDialog) {
        sub_6FB13290();
        g_pNpcMainMenuDialog = nullptr;
    }
}

// 1.12a: 0x6FAC84B0 (__cdecl)
void __cdecl sub_6FAC84B0()
{
    if (g_pNpcSubDialog) {
        sub_6FB13290();
        g_pNpcSubDialog = nullptr;
    }
}

// 1.12a: 0x6FAC84D0 (__cdecl)
void __cdecl sub_6FAC84D0()
{
    if (g_pNpcPopupDialog) {
        sub_6FB13290();
        g_pNpcPopupDialog = nullptr;
    }
}

// 1.12a: 0x6FAC8530 (__stdcall)
int __stdcall sub_6FAC8530(D2UnitStrc* pPlayer, D2UnitStrc* pItem, int nTransactionType)
{
    if (!pItem) return 0;
    if (!ITEMS_IsNotQuestItem(pItem)) return 0;
    return ITEMS_GetTransactionCost(pPlayer, pItem, 0, g_pClientQuestRecord, g_nInteractingNpcClassId, nTransactionType);
}

// 1.12a: 0x6FAC88F0 (__cdecl)
void __cdecl sub_6FAC88F0()
{
    D2HirelingEntryStrc* pCur = g_pHirelingListHead;
    while (pCur) {
        D2HirelingEntryStrc* pNext = pCur->pNext;
        FOG_Free(pCur, "..\\Source\\D2Client\\UI\\npcmenu.cpp", 0x520, 0);
        pCur = pNext;
    }
    g_pHirelingListHead = nullptr;
}

// 1.12a: 0x6FAC8940 (__cdecl)
void __cdecl sub_6FAC8940()
{
    sub_6FAC88F0();
    if (g_pClientQuestRecord) {
        QUESTRECORD_FreeRecord(nullptr, g_pClientQuestRecord);
        g_pClientQuestRecord = nullptr;
    }
    if (g_pTempQuestRecord) {
        QUESTRECORD_FreeRecord(nullptr, g_pTempQuestRecord);
        g_pTempQuestRecord = nullptr;
    }
    if (s_pBuySellCel) {
        D2CMP_CelFileFreeHardware(s_pBuySellCel);
        FOG_Free(s_pBuySellCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
        s_pBuySellCel = nullptr;
    }
}

// 1.12a: 0x6FAC8A60 (register: eax=nNpcClassId)
// 1.12a: 0x6FAC8AB0 (internal entry point)
int __fastcall sub_6FAC8A60(int nNpcClassId)
{
    return sub_6FAC7F50(nNpcClassId);
}

int __fastcall sub_6FAC8AB0(int nNpcClassId)
{
    return sub_6FAC8A60(nNpcClassId);
}

// 1.12a: 0x6FAC8B40 (__cdecl)
void __cdecl sub_6FAC8B40()
{
    s_nNpcInteractSubFlag = 0;
}

// 1.12a: 0x6FAC8B60 (register: eax=pNpcUnit, ecx=pOutRect)
void __fastcall sub_6FAC8B60(RECT* pOutRect, D2UnitStrc* pNpcUnit)
{
    if (pNpcUnit && pOutRect) {
        UNITS_GetClientCoords(pNpcUnit, pOutRect);
    }
}

// 1.12a: 0x6FAC8BF0 (__cdecl)
int __cdecl sub_6FAC8BF0()
{
    if (!g_pClientQuestRecord) return 0;
    return QUESTRECORD_GetQuestState(g_pClientQuestRecord, 4, 0);
}

// 1.12a: 0x6FAC8D00 (__cdecl)
void __cdecl sub_6FAC8D00()
{
    if (g_bInteractingWithNpc) {
        MSG_SendByte2Dwords(0x30, 1, g_nInteractingNpcGUID);
        g_bInteractingWithNpc = 0;
    }
}

// 1.12a: 0x6FAC8D40 (__cdecl)
int __cdecl sub_6FAC8D40()
{
    sub_6FAC84D0();
    return 1;
}

// 1.12a: 0x6FAC8DD0 (__cdecl)
int __cdecl sub_6FAC8DD0()
{
    // Cain Identify Items (C2S 0x34)
    sub_6FAC8450();
    MSG_SendByteDword(0x34, g_nInteractingNpcGUID);
    return 1;
}

// 1.12a: 0x6FAC8EE0 (__cdecl)
int __cdecl sub_6FAC8EE0()
{
    // Charsi Imbue / Socket / Personalize prompt
    sub_6FAC8450();
    return 1;
}

// 1.12a: 0x6FAC8FF0 (__cdecl)
int __cdecl sub_6FAC8FF0()
{
    // Resurrect Mercenary (C2S 0x62)
    sub_6FAC8450();
    MSG_SendByteDword(0x62, g_nInteractingNpcGUID);
    return 1;
}

// 1.12a: 0x6FAC90E0 (__cdecl)
int __cdecl sub_6FAC90E0()
{
    sub_6FAC84B0();
    return 1;
}

// 1.12a: 0x6FAC9110 (__cdecl)
void __cdecl sub_6FAC9110()
{
}

// 1.12a: 0x6FAC9140 (__stdcall)
void __stdcall sub_6FAC9140(int nMode)
{
    s_nNpcShopTabMode = nMode;
}

// 1.12a: 0x6FAC91C0 (__stdcall)
int __stdcall sub_6FAC91C0(int nButton, int nTab)
{
    return 1;
}

// 1.12a: 0x6FAC94D0 (__cdecl)
void __cdecl sub_6FAC94D0()
{
}

// 1.12a: 0x6FAC95F0 (__cdecl)
int __cdecl sub_6FAC95F0()
{
    sub_6FAC84D0();
    return 1;
}

// 1.12a: 0x6FAC9640 (__cdecl)
int __cdecl sub_6FAC9640()
{
    sub_6FAC84D0();
    return 1;
}

// 1.12a: 0x6FAC9690 (__cdecl)
int __cdecl sub_6FAC9690()
{
    if (!g_pNpcTextHeader) return 0;
    int nSelected = sub_6FB129C0(g_pNpcSubDialog);
    short nStrId = TEXT_GetStringIdOfMenuType2Node(g_pNpcTextHeader, nSelected);
    return nStrId != -1;
}

// 1.12a: 0x6FAC9760 (__cdecl)
void __cdecl sub_6FAC9760()
{
    sub_6FAC84D0();
    sub_6FAC84B0();
    sub_6FAC8450();
}

// 1.12a: 0x6FAC97E0 (__cdecl)
int __cdecl sub_6FAC97E0()
{
    return 1;
}

// 1.12a: 0x6FAC9AF0 (__cdecl)
void __cdecl sub_6FAC9AF0()
{
    sub_6FAC9760();
    sub_6FAC8D00();
}

// 1.12a: 0x6FAC9B40 (__cdecl)
int __cdecl sub_6FAC9B40()
{
    sub_6FAC9AF0();
    return 1;
}

// 1.12a: 0x6FAC9C00 (__cdecl)
int __cdecl sub_6FAC9C00()
{
    if (!g_pClientQuestRecord) return 0;
    return QUESTRECORD_GetQuestState(g_pClientQuestRecord, 21, 0);
}

// 1.12a: 0x6FAC9D00 (__cdecl)
// 1.12a: 0x6FAC9D40 (internal entry point)
int __cdecl sub_6FAC9D00()
{
    // Request Trade Inventory (C2S 0x38, mode 1)
    sub_6FAC8450();
    MSG_SendByte3Dwords(0x38, 1, g_nInteractingNpcGUID, 0);
    return 1;
}

int __cdecl sub_6FAC9D40()
{
    // Request Gamble Inventory (C2S 0x38, mode 2)
    sub_6FAC8450();
    MSG_SendByte3Dwords(0x38, 2, g_nInteractingNpcGUID, 0);
    return 1;
}

// 1.12a: 0x6FAC9D90 (__cdecl)
int __cdecl sub_6FAC9D90()
{
    // Repair All Items (C2S 0x35 with bit 0x80000000)
    MSG_SendByte4Dwords(0x35, g_nInteractingNpcGUID, 0, 0, static_cast<int32_t>(0x80000000u));
    return 1;
}

// 1.12a: 0x6FAC9E00 (__stdcall)
int __stdcall sub_6FAC9E00(D2UnitStrc* pPlayer, D2UnitStrc* pItem, int nMode, int a4, int a5, int a6)
{
    // Execute Buy (0x32), Sell (0x33), Repair (0x35), or Identify/Charge
    if (!pPlayer || !pItem) return 0;
    return ITEMS_GetTransactionCost(pPlayer, pItem, 0, g_pClientQuestRecord, g_nInteractingNpcClassId, nMode);
}

// 1.12a: 0x6FACA1F0 (__cdecl)
void __cdecl sub_6FACA1F0()
{
    if (g_pNpcTextHeader) {
        TEXT_FreeTextHeader(g_pNpcTextHeader);
        g_pNpcTextHeader = nullptr;
    }
    sub_6FAC9760();
}

// 1.12a: 0x6FACA340 (__stdcall)
int __stdcall sub_6FACA340(int nActTarget)
{
    // Travel to Act (Warriv / Meshif / Tyrael)
    sub_6FAC9760();
    MSG_SendByte3Dwords(0x38, 0, g_nInteractingNpcGUID, nActTarget);
    return 1;
}

// 1.12a: 0x6FACA370 (__stdcall)
int __stdcall sub_6FACA370(int nActTarget)
{
    return sub_6FACA340(nActTarget);
}

// 1.12a: 0x6FACA3A0 (__stdcall)
int __stdcall sub_6FACA3A0(int nActTarget)
{
    return sub_6FACA340(nActTarget);
}

// 1.12a: 0x6FACA3D0 (__stdcall)
int __stdcall sub_6FACA3D0(int nActTarget)
{
    return sub_6FACA340(nActTarget);
}

// 1.12a: 0x6FACA400 (__stdcall)
int __stdcall sub_6FACA400(int nActTarget)
{
    return sub_6FACA340(nActTarget);
}

// 1.12a: 0x6FACA430 (__cdecl)
int __cdecl sub_6FACA430()
{
    sub_6FAC84D0();
    return 1;
}

// 1.12a: 0x6FACA4A0 (__cdecl)
// 1.12a: 0x6FACA570 (internal entry point)
int __cdecl sub_6FACA4A0()
{
    // Hire Selected Mercenary (C2S 0x36)
    sub_6FAC84B0();
    return 1;
}

int __cdecl sub_6FACA570()
{
    return sub_6FACA4A0();
}

// 1.12a: 0x6FACA5D0 (__cdecl)
int __cdecl sub_6FACA5D0()
{
    sub_6FAC84D0();
    return 1;
}

// 1.12a: 0x6FACA630 (__cdecl)
int __cdecl sub_6FACA630() { return sub_6FACA340(0); }

// 1.12a: 0x6FACA660 (__cdecl)
int __cdecl sub_6FACA660() { return sub_6FACA340(1); }

// 1.12a: 0x6FACA690 (__cdecl)
void __cdecl sub_6FACA690()
{
    // Vendor Buy/Sell/Repair/Gamble Panel Render & Cursor Handling
}

// 1.12a: 0x6FACA890 (__stdcall)
void __stdcall sub_6FACA890(void* pPacket)
{
    // S2C 0x4E: Receive Mercenary List Header
    sub_6FAC88F0();
}

// 1.12a: 0x6FACA920 (__cdecl)
void __cdecl sub_6FACA920()
{
    sub_6FACA1F0();
}

// 1.12a: 0x6FACA950 (__cdecl)
void __cdecl sub_6FACA950() { sub_6FAC9D00(); }

// 1.12a: 0x6FACA960 (__cdecl)
void __cdecl sub_6FACA960() { sub_6FAC9D40(); }

// 1.12a: 0x6FACA970 (__cdecl)
int __cdecl sub_6FACA970()
{
    sub_6FAC84D0();
    return 1;
}

// 1.12a: 0x6FACA9D0 (__stdcall)
int __stdcall sub_6FACA9D0(int a1)
{
    return 1;
}

// 1.12a: 0x6FACAA00 (__cdecl)
void __cdecl sub_6FACAA00()
{
    sub_6FAC9AF0();
}

// 1.12a: 0x6FACAA20 (__cdecl)
int __cdecl sub_6FACAA20() { return sub_6FACA340(1); }

// 1.12a: 0x6FACAA80 (__cdecl)
int __cdecl sub_6FACAA80() { return sub_6FACA340(2); }

// 1.12a: 0x6FACAAE0 (__cdecl)
int __cdecl sub_6FACAAE0() { return sub_6FACA340(3); }

// 1.12a: 0x6FACAB40 (__cdecl)
int __cdecl sub_6FACAB40() { return sub_6FACA340(4); }

// 1.12a: 0x6FACABA0 (__cdecl)
int __cdecl sub_6FACABA0() { return sub_6FACA340(0); }

// 1.12a: 0x6FACAC00 (__cdecl)
int __cdecl sub_6FACAC00()
{
    // Start NPC Gossip / Introduction Speech
    return 1;
}

// 1.12a: 0x6FACADF0 (__cdecl)
void __cdecl sub_6FACADF0()
{
    // Open Buy/Sell/Repair confirmation popup dialog
}

// 1.12a: 0x6FACB280 (__cdecl)
int __cdecl sub_6FACB280()
{
    sub_6FAC8EE0();
    return 1;
}

// 1.12a: 0x6FACB2B0 (__cdecl)
int __cdecl sub_6FACB2B0() { return sub_6FACB280(); }

// 1.12a: 0x6FACB2E0 (__cdecl)
int __cdecl sub_6FACB2E0() { return sub_6FACB280(); }

// 1.12a: 0x6FACB310 (__cdecl)
void __cdecl sub_6FACB310()
{
    sub_6FAC9AF0();
}

// 1.12a: 0x6FACB350 (__cdecl)
int __cdecl sub_6FACB350()
{
    return 1;
}

// 1.12a: 0x6FACB400 (__cdecl)
void __cdecl sub_6FACB400()
{
    sub_6FAC8410();
}

// 1.12a: 0x6FACB420 (__cdecl)
void __cdecl sub_6FACB420()
{
    sub_6FAC9760();
}

// 1.12a: 0x6FACB470 (__cdecl)
void __cdecl sub_6FACB470()
{
    sub_6FAC9760();
}

// 1.12a: 0x6FACB4C0 (__cdecl)
void __cdecl sub_6FACB4C0()
{
    sub_6FAC9AF0();
}

// 1.12a: 0x6FACB540 (__cdecl)
int __cdecl sub_6FACB540()
{
    return 1;
}

// 1.12a: 0x6FACB610 (register: eax=pPacket)
void __fastcall sub_6FACB610(uint8_t* pPacket)
{
    // S2C 0x4F: Add Hireling Candidate to g_pHirelingListHead
    D2HirelingEntryStrc* pEntry = static_cast<D2HirelingEntryStrc*>(
        FOG_Alloc(sizeof(D2HirelingEntryStrc), "..\\Source\\D2Client\\UI\\npcmenu.cpp", 0xB30, 0));
    memset(pEntry, 0, sizeof(D2HirelingEntryStrc));
    pEntry->pNext = g_pHirelingListHead;
    g_pHirelingListHead = pEntry;
}

// 1.12a: 0x6FACBB80 (__cdecl)
int __cdecl sub_6FACBB80()
{
    // Open Hire Mercenary Selection Dialog
    sub_6FAC8450();
    return 1;
}

// 1.12a: 0x6FACBE50 (__stdcall)
int __stdcall sub_6FACBE50(int bFromSubMenu)
{
    // Open NPC Talk Sub-Menu (Gossip + Active Quests)
    sub_6FAC8450();
    if (!g_pNpcTextHeader) return 0;
    int nCount = TEXT_GetMenuType2NodeCount(g_pNpcTextHeader);
    return nCount > 0;
}

// 1.12a: 0x6FACC100 (__cdecl)
int __cdecl sub_6FACC100()
{
    return sub_6FACBE50(0);
}

// 1.12a: 0x6FACC130 (__stdcall)
void __stdcall sub_6FACC130(int nTab)
{
    // Populate Vendor Tab buttons (Buy/Sell/Repair All)
}

// 1.12a: 0x6FACC2E0 (__cdecl)
int __cdecl sub_6FACC2E0()
{
    return sub_6FAC8FF0();
}

// 1.12a: 0x6FACC310 (__cdecl)
// 1.12a: 0x6FACC340 (internal entry point)
int __cdecl sub_6FACC310()
{
    return sub_6FACBB80();
}

int __cdecl sub_6FACC340()
{
    return sub_6FACBB80();
}

// 1.12a: 0x6FACC3A0 (__cdecl)
void __cdecl sub_6FACC3A0()
{
    sub_6FACA1F0();
}

// 1.12a: 0x6FACC3F0 (__cdecl)
int __cdecl sub_6FACC3F0()
{
    return FOG_IsExpansion();
}

// 1.12a: 0x6FACC510 (__stdcall)
int __stdcall sub_6FACC510(D2UnitStrc* pNpc, int bGamble)
{
    // Initialize Vendor Trade/Gamble UI Session
    g_pTradeNpcUnit = pNpc;
    return 1;
}

// 1.12a: 0x6FACC8E0 (__cdecl)
int __cdecl sub_6FACC8E0()
{
    return FOG_IsExpansion();
}

// 1.12a: 0x6FACCB10 (__cdecl)
// 1.12a: 0x6FACCC00 (internal entry point)
int __cdecl sub_6FACCB10()
{
    // Evaluate Quest-Driven NPC Menu Overrides
    return 1;
}

int __cdecl sub_6FACCC00()
{
    return sub_6FACCB10();
}

// 1.12a: 0x6FACCF00 (__stdcall)
// 1.12a: 0x6FACD250 (internal entry point)
int __stdcall sub_6FACCF00(D2UnitStrc* pNpcUnit)
{
    extern const Unicode* __fastcall D2Client_GetNpcDisplayName(D2UnitStrc* pUnit);

    // Build & Open Main Town NPC Interaction Menu Dialog
    int nMenuIdx = sub_6FAC7D50(pNpcUnit);
    if (nMenuIdx < 0) return 0;
    sub_6FAC9760();
    g_nActiveNpcMenuIdx = nMenuIdx;
    if (pNpcUnit) {
        g_bInteractingWithNpc = 1;
        g_nInteractingNpcGUID = *(int32_t*)((uint8_t*)pNpcUnit + 0x0C);
        g_nInteractingNpcClassId = *(int32_t*)((uint8_t*)pNpcUnit + 0x04);
    }
    if (!g_pPentagramCel) {
        g_pPentagramCel = ARCHIVE_LoadCellFileDefault("DATA\\GLOBAL\\UI\\MENU\\pentaspin", 0);
    }
    RECT rcNpc = { 0 };
    sub_6FAC8B60(&rcNpc, pNpcUnit);
    int nAnchorX = (rcNpc.left + rcNpc.right) / 2;
    int nAnchorY = rcNpc.top;
    if (nAnchorX <= 0) {
        nAnchorX = (gnScreenWidth > 0) ? (gnScreenWidth / 2) : 400;
    }
    if (nAnchorY <= 0) {
        nAnchorY = (gnScreenHeight > 0) ? (gnScreenHeight / 3) : 180;
    }
    g_pNpcMainMenuDialog = sub_6FB13600(nAnchorX, nAnchorY, sub_6FAC9B40, nullptr, 0, 1, 1, 0, 0, 0, 0);
    if (!g_pNpcMainMenuDialog) {
        return 0;
    }

    const Unicode* wszNpcName = D2Client_GetNpcDisplayName(pNpcUnit);
    if (wszNpcName && wszNpcName->ch != 0) {
        sub_6FB12DF0(g_pNpcMainMenuDialog, 4, wszNpcName, 21, 0, 1, nullptr, 0);
    }

    const D2NpcMenuDefStrc& rMenuDef = g_aNpcMenuDefs[nMenuIdx];
    for (int i = 0; i < 5; ++i) {
        if (rMenuDef.wStringIds[i] != 0 && rMenuDef.pfnCallbacks[i] != nullptr) {
            const Unicode* wszOpt = D2LANG_GetStringFromTblIndex(rMenuDef.wStringIds[i]);
            if (wszOpt) {
                sub_6FB12DF0(
                    g_pNpcMainMenuDialog,
                    0,
                    wszOpt,
                    18,
                    0,
                    1,
                    reinterpret_cast<int (__cdecl*)(void)>(rMenuDef.pfnCallbacks[i]),
                    1);
            }
        }
    }

    const Unicode* wszCancel = D2LANG_GetStringFromTblIndex(3382);
    if (wszCancel) {
        sub_6FB12DF0(g_pNpcMainMenuDialog, 0, wszCancel, 18, 0, 1, sub_6FAC9B40, 1);
    }

    return 1;
}

int __cdecl sub_6FACD250()
{
    // Open Scrolling Audio Speech Text Box
    return 1;
}

// 1.12a: 0x6FACD440 (__cdecl)
int __cdecl sub_6FACD440()
{
    return 1;
}

// 1.12a: 0x6FACD5F0 (__cdecl)
int __cdecl sub_6FACD5F0()
{
    return 1;
}

// 1.12a: 0x6FACD8C0 (__stdcall)
void __stdcall sub_6FACD8C0(D2UnitStrc* pNpcUnit, uint8_t* pQuestBuf, int bHasMerc)
{
    // S2C 0x27 / NPC Menu Trigger: Copy Quest States, Build TextHeader, Open NPC Menu
    if (pQuestBuf) {
        QUESTRECORD_CopyBufferToRecord(g_pClientQuestRecord, pQuestBuf, 0x54, 1);
    }
    sub_6FACCF00(pNpcUnit);
}

// 1.12a: 0x6FACDCA0 (__cdecl)
int __cdecl sub_6FACDCA0()
{
    extern D2UnitStrc* __cdecl sub_6FACE310();
    D2UnitStrc* pPlayer = sub_6FACE310();
    return pPlayer ? STATLIST_UnitGetStatValue(pPlayer, 14, 0) : 0;
}

// 1.12a: 0x6FACDD20 (__cdecl)
int __cdecl sub_6FACDD20()
{
    return FOG_IsExpansion();
}

// 1.12a: 0x6FACDE00 (__cdecl)
int __cdecl sub_6FACDE00() { return sub_6FACDD20(); }

// 1.12a: 0x6FACDE30 (__cdecl)
int __cdecl sub_6FACDE30() { return sub_6FACDD20(); }

// 1.12a: 0x6FACDE60 (__cdecl)
int __cdecl sub_6FACDE60() { return sub_6FACDD20(); }

// --- Section 2G: Custom PE Loader & Client Unit Hash Table (0x6FACDE90..0x6FACED32) ---

// 1.12a: 0x6FACDE90 (register: eax=pDosHeader)
IMAGE_NT_HEADERS* __fastcall sub_6FACDE90(IMAGE_DOS_HEADER* pDosHeader)
{
    if (!pDosHeader || pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) return nullptr;
    IMAGE_NT_HEADERS* pNt = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<uint8_t*>(pDosHeader) + pDosHeader->e_lfanew);
    return (pNt->Signature == IMAGE_NT_SIGNATURE) ? pNt : nullptr;
}

// 1.12a: 0x6FACDEB0 (register: esi=pModuleBase)
void __fastcall sub_6FACDEB0(void* pModuleBase)
{
    if (pModuleBase) {
        VirtualFree(pModuleBase, 0, MEM_RELEASE);
    }
}

// 1.12a: 0x6FACDF10 (__stdcall)
void* __stdcall sub_6FACDF10(void* pRawPeImage)
{
    IMAGE_NT_HEADERS* pNt = sub_6FACDE90(static_cast<IMAGE_DOS_HEADER*>(pRawPeImage));
    if (!pNt) return nullptr;
    void* pBase = VirtualAlloc(reinterpret_cast<void*>(pNt->OptionalHeader.ImageBase),
        pNt->OptionalHeader.SizeOfImage, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    return pBase;
}

// 1.12a: 0x6FACE1C0 (register: eax=pUnit)
D2UnitStrc* __fastcall sub_6FACE1C0(D2UnitStrc* pUnit)
{
    if (!pUnit) return nullptr;
    D2InventoryStrc* pInv = *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60);
    if (!pInv) return nullptr;
    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pInv); pItem; pItem = INVENTORY_GetNextItem(pItem)) {
        if (INVENTORY_UnitIsItem(pItem)) return pItem;
    }
    return nullptr;
}

// 1.12a: 0x6FACE230 (__stdcall)
void __stdcall sub_6FACE230(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x88));
        exit(-1);
    }
}

// 1.12a: 0x6FACE260 (register: eax=pUnit)
void __fastcall sub_6FACE260(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x98));
        exit(-1);
    }
}

// 1.12a: 0x6FACE290 (register: eax=pUnit)
void __fastcall sub_6FACE290(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0xA8));
        exit(-1);
    }
}

// 1.12a: 0x6FACE2C0 (register: eax=pUnit, edx=nState)
int __fastcall sub_6FACE2C0(int nState, D2UnitStrc* pUnit)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0xB8));
        exit(-1);
    }
    return STATES_CheckState(pUnit, nState);
}

// 1.12a: 0x6FACE310 (__cdecl)
D2UnitStrc* __cdecl sub_6FACE310()
{
    return gpPlayerUnit;
}

// 1.12a: 0x6FACE320 (register: eax=pPlayer)
// 1.12a: 0x6FACE350 (internal entry point)
void __fastcall sub_6FACE320(D2UnitStrc* pPlayer)
{
    gpPlayerUnit = pPlayer;
}

void __fastcall sub_6FACE350(D2UnitStrc* pPlayer)
{
    gpPlayerUnit = pPlayer;
}

// 1.12a: 0x6FACE380 (__stdcall)
D2SkillsTxt* __stdcall sub_6FACE380(D2UnitStrc* pUnit)
{
    D2SkillStrc* pSkill = pUnit ? UNITS_GetUsedSkill(pUnit) : nullptr;
    return pSkill ? SKILLS_GetSkillsTxtRecordFromSkill(pSkill) : nullptr;
}

// 1.12a: 0x6FACE3D0 (register: eax=pUnit)
void __fastcall sub_6FACE3D0(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x110));
        exit(-1);
    }
}

// 1.12a: 0x6FACE410 (register: eax=pUnit)
int __fastcall sub_6FACE410(D2UnitStrc* pUnit) { return D2COMMON_11019(pUnit); }

// 1.12a: 0x6FACE420 (register: eax=pUnit)
int __fastcall sub_6FACE420(D2UnitStrc* pUnit) { return D2Common_11020(pUnit); }

// 1.12a: 0x6FACE430 (register: eax=pUnit)
int __fastcall sub_6FACE430(D2UnitStrc* pUnit) { return D2COMMON_11018(pUnit); }

// 1.12a: 0x6FACE440 (register: eax=pUnit, edx=a2)
int __fastcall sub_6FACE440(int a2, D2UnitStrc* pUnit) { return D2COMMON_11017_CheckUnitIfConsumeable(pUnit, a2); }

// 1.12a: 0x6FACE450 (register: eax=pUnit)
int __fastcall sub_6FACE450(D2UnitStrc* pUnit) { return D2COMMON_11021(pUnit); }

// 1.12a: 0x6FACE460 (__cdecl)
void __cdecl sub_6FACE460() { s_nClientUnitHashResetFlag = 0; }

// 1.12a: 0x6FACE470 (__cdecl)
void __cdecl sub_6FACE470()
{
    memset(gapServerSideUnitHashTables, 0, sizeof(gapServerSideUnitHashTables));
    memset(gapClientSideUnitHashTables, 0, sizeof(gapClientSideUnitHashTables));
}

// 1.12a: 0x6FACE490 (__cdecl)
void* __cdecl sub_6FACE490() { return &gapServerSideUnitHashTables[0][0]; }

// 1.12a: 0x6FACE4A0 (__cdecl)
void* __cdecl sub_6FACE4A0() { return &gapClientSideUnitHashTables[0][0]; }

// 1.12a: 0x6FACE4B0 (register: eax=pUnit)
void __fastcall sub_6FACE4B0(D2UnitStrc* pUnit) { STATLIST_FreeModStats(pUnit); }

// 1.12a: 0x6FACE4C0 (__cdecl)
void __cdecl sub_6FACE4C0() {}

// 1.12a: 0x6FACE4D0 (__stdcall)
void __stdcall sub_6FACE4D0(void (__fastcall* pfnCallback)(D2UnitStrc*))
{
    D2UnitStrc** ppTable = &gapServerSideUnitHashTables[0][0];
    for (int i = 0; i < 6 * 128; ++i) {
        for (D2UnitStrc* pUnit = ppTable[i]; pUnit; pUnit = *(D2UnitStrc**)((uint8_t*)pUnit + 0xE4)) {
            pfnCallback(pUnit);
        }
    }
}

// 1.12a: 0x6FACE510 (__cdecl)
void __cdecl sub_6FACE510()
{
    sub_6FACE470();
}

// 1.12a: 0x6FACE530 (__stdcall)
void __stdcall sub_6FACE530(D2UnitStrc* pUnit)
{
    UNITROOM_RemoveUnitFromRoom(pUnit);
}

// 1.12a: 0x6FACE550 (register: ecx=nUnitGUID, edx=nUnitType)
D2UnitStrc* __fastcall sub_6FACE550(int nUnitGUID, int nUnitType)
{
    D2UnitStrc** ppTable = (nUnitGUID < 0) ? &gapClientSideUnitHashTables[0][0] : &gapServerSideUnitHashTables[0][0];
    uint32_t dwHash = static_cast<uint32_t>(nUnitGUID) & 0x7F;
    for (D2UnitStrc* pUnit = ppTable[nUnitType * 128 + dwHash]; pUnit; pUnit = *(D2UnitStrc**)((uint8_t*)pUnit + 0xE4)) {
        if (*(int32_t*)((uint8_t*)pUnit + 0x0C) == nUnitGUID) {
            return pUnit;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FACE590 (register: ecx=nUnitGUID, edx=nUnitType)
D2UnitStrc* __fastcall sub_6FACE590(int nUnitGUID, int nUnitType)
{
    return sub_6FACE550(nUnitGUID, nUnitType);
}

// 1.12a: 0x6FACE5A0 (register: ecx=nUnitGUID, edx=nUnitType)
D2UnitStrc* __fastcall sub_6FACE5A0(int nUnitGUID, int nUnitType)
{
    return sub_6FACE550(nUnitGUID, nUnitType);
}

// 1.12a: 0x6FACE5B0 (register: eax=pClientUnit)
D2UnitStrc* __fastcall sub_6FACE5B0(D2UnitStrc* pClientUnit)
{
    if (!pClientUnit) return nullptr;
    int nType = *(int32_t*)((uint8_t*)pClientUnit + 0x00);
    int nGUID = *(int32_t*)((uint8_t*)pClientUnit + 0x0C);
    return DEBUG_GetUnitFromTypeAndGUID(nType, nGUID);
}

// 1.12a: 0x6FACE920 (__stdcall)
void __stdcall sub_6FACE920(D2UnitStrc* pUnit)
{
}

// 1.12a: 0x6FACE990 (__stdcall)
D2LevelsTxt* __stdcall sub_6FACE990(D2RoomStrc* pRoom)
{
    int nDestLevel = DUNGEON_GetWarpDestinationLevel(pRoom, 0);
    return nDestLevel ? DATATBLS_GetLevelsTxtRecord(nDestLevel) : nullptr;
}

// 1.12a: 0x6FACE9C0 (register: eax=pUnit)
int __fastcall sub_6FACE9C0(D2UnitStrc* pUnit)
{
    return pUnit ? *(int32_t*)((uint8_t*)pUnit + 0x10) : 0;
}

// 1.12a: 0x6FACE9E0 (register: eax=pUnit)
void __fastcall sub_6FACE9E0(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x310));
        exit(-1);
    }
}

// 1.12a: 0x6FACEA20 (register: eax=pUnit)
void __fastcall sub_6FACEA20(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x320));
        exit(-1);
    }
}

// 1.12a: 0x6FACEA60 (register: eax=pUnit)
void __fastcall sub_6FACEA60(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x330));
        exit(-1);
    }
}

// 1.12a: 0x6FACEAA0 (register: eax=pUnit)
int __fastcall sub_6FACEAA0(D2UnitStrc* pUnit)
{
    return pUnit ? *(int32_t*)((uint8_t*)pUnit + 0x04) : -1;
}

// 1.12a: 0x6FACEAC0 (register: eax=pUnit)
int __fastcall sub_6FACEAC0(D2UnitStrc* pUnit)
{
    return pUnit != nullptr;
}

// 1.12a: 0x6FACEB00 (register: eax=pUnit)
D2RoomStrc* __fastcall sub_6FACEB00(D2UnitStrc* pUnit)
{
    return pUnit ? UNITS_GetRoom(pUnit) : nullptr;
}

// 1.12a: 0x6FACEB20 (__stdcall)
D2UnitStrc* __stdcall sub_6FACEB20(D2UnitStrc* pUnit)
{
    D2InventoryStrc* pInv = pUnit ? *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60) : nullptr;
    return pInv ? INVENTORY_GetCursorItem(pInv) : nullptr;
}

// 1.12a: 0x6FACEBC0 (register: eax=pUnit)
void __fastcall sub_6FACEBC0(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x380));
        exit(-1);
    }
}

// 1.12a: 0x6FACEC80 (register: eax=pUnit)
void __fastcall sub_6FACEC80(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        FOG_DisplayHalt("%s", UnimplementedOrdinal_10265(0x3A0));
        exit(-1);
    }
    UNITS_ChangeAnimMode(pUnit, 1);
}
