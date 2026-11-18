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
struct D2InventoryStrc;

#ifndef D2_RECON_UNIT_TYPED_VIEW_DEFINED
#define D2_RECON_UNIT_TYPED_VIEW_DEFINED
#pragma pack(push, 1)
struct D2ReconItemDataViewStrc
{
    uint8_t pad00[0x45];
    uint8_t nInvPage; // +0x45
};
struct D2ReconObjectDataViewStrc
{
    void*   pObjectTxt; // +0x00
    uint8_t nShrineNo;  // +0x04
};
struct D2ReconUnitViewStrc
{
    uint32_t dwUnitType;                // +0x00
    int32_t  dwClassId;                 // +0x04
    void*    pMemoryPool;               // +0x08
    uint32_t dwUnitId;                  // +0x0C
    int32_t  dwAnimMode;                // +0x10
    void*    pUnitData;                 // +0x14
    int32_t  nAct;                      // +0x18
    void*    pDrlgAct;                  // +0x1C
    uint32_t dwSeed[2];                 // +0x20
    uint32_t dwInitSeed;                // +0x28
    void*    pDynamicPath;              // +0x2C
    void*    pAnimSeq;                  // +0x30
    uint32_t dwSeqFrameCount;           // +0x34
    int32_t  dwSeqFrame;                // +0x38
    uint32_t dwSeqSpeed;                // +0x3C
    uint32_t dwSeqMode;                 // +0x40
    int32_t  nSeqCurrentFramePrecise;   // +0x44
    int32_t  dwFrameCountPrecise;       // +0x48
    int16_t  wAnimSpeed;                // +0x4C
    uint8_t  nActionFrame;              // +0x4E
    uint8_t  unk0x4F;                   // +0x4F
    void*    pAnimData;                 // +0x50
    void*    pGfxData;                  // +0x54
    void*    pGfxDataCopy;              // +0x58
    void*    pStatListEx;               // +0x5C
    D2InventoryStrc* pInventory;        // +0x60
    void*    pLight;                    // +0x64
    uint32_t dwStartLight;              // +0x68
    int32_t  nPaletteIndex;             // +0x6C
    void*    pUnitSfxSlot;              // +0x70
    uint8_t  pad74[0x8C - 0x74];        // +0x74..+0x8B
    uint16_t nLastClickX;               // +0x8C
    uint16_t nLastClickY;               // +0x8E
    uint8_t  pad90[0xA8 - 0x90];        // +0x90..+0xA7
    void*    pSkills;                   // +0xA8
    uint8_t  padAC[0xC4 - 0xAC];        // +0xAC..+0xC3
    uint32_t dwFlags;                   // +0xC4
    uint32_t dwFlagEx;                  // +0xC8
    uint8_t  padCC[0xE4 - 0xCC];        // +0xCC..+0xE3
    D2UnitStrc* pListNext;              // +0xE4
    D2UnitStrc* pRoomNext;              // +0xE8
    void*    pMsgFirst;                 // +0xEC
    void*    pMsgLast;                  // +0xF0
};
static_assert(sizeof(D2ReconUnitViewStrc) == 0xF4, "D2ReconUnitViewStrc must be 0xF4 bytes");
struct D2ReconWinMsgViewStrc
{
    HWND     hWnd;         // +0x00
    uint32_t uMsg;         // +0x04
    union
    {
        int32_t wParam;    // +0x08
        struct
        {
            uint16_t wParamLow;  // +0x08
            int16_t  wWheelDelta;// +0x0A
        };
    };
    uint16_t nX;           // +0x0C
    uint16_t nY;           // +0x0E
    uint32_t dwKeyFlags;   // +0x10
    int32_t  nField14;     // +0x14
    int32_t  bHandled;     // +0x18
    int32_t  nReturnValue; // +0x1C
};
#pragma pack(pop)
static inline D2ReconUnitViewStrc* D2UnitView(D2UnitStrc* p) { return reinterpret_cast<D2ReconUnitViewStrc*>(p); }
static inline const D2ReconUnitViewStrc* D2UnitView(const D2UnitStrc* p) { return reinterpret_cast<const D2ReconUnitViewStrc*>(p); }
static inline D2ReconWinMsgViewStrc* D2MsgView(void* p) { return reinterpret_cast<D2ReconWinMsgViewStrc*>(p); }
#endif


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
    void __cdecl FOG_DisplayHalt(const char* szMsg, const char* szFile, int nLine); // Fog!#10024
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

static inline void FOG_DisplayHalt(const char* szFmt, const char* szLoc)
{
    FOG_DisplayHalt(szFmt ? szFmt : "Halt", szLoc ? szLoc : __FILE__, 0);
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










void __stdcall sub_6FB13EB0(void* pMsg);


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

extern void __stdcall DIALOG_OnMouseButtonUp(void* pMsg);
extern void __stdcall DIALOG_OnEscapeKey(void* pMsg);
extern void __stdcall DIALOG_OnMenuCancelHotkey(void* pMsg);
extern void __stdcall DIALOG_OnReturnKey(void* pMsg);
extern void __stdcall DIALOG_OnCharKey(void* pMsg);
extern void __stdcall DIALOG_OnDownArrow(void* pMsg);
extern void __stdcall DIALOG_OnUpArrow(void* pMsg);
extern void __stdcall DIALOG_OnLeftArrow(void* pMsg);
extern void __stdcall DIALOG_OnRightArrow(void* pMsg);
extern void __stdcall DIALOG_OnMouseWheel(void* pMsg);
extern void __stdcall DIALOG_OnMouseButtonDown(void* pMsg);
extern void __stdcall DIALOG_OnMouseMove(void* pMsg);

static D2SMsgRegEntryStrc s_aDialogMsgHandlers[14] = {
    { 3, VK_ESCAPE,      DIALOG_OnMenuCancelHotkey },
    { 3, VK_ESCAPE,      DIALOG_OnMenuCancelHotkey },
    { 0, WM_LBUTTONUP,   DIALOG_OnMouseButtonUp },
    { 0, WM_LBUTTONDOWN, DIALOG_OnMouseButtonDown },
    { 0, WM_RBUTTONUP,   DIALOG_OnMouseButtonUp },
    { 0, WM_RBUTTONDOWN, DIALOG_OnMouseButtonDown },
    { 3, VK_UP,          DIALOG_OnUpArrow },
    { 3, VK_DOWN,        DIALOG_OnDownArrow },
    { 3, VK_RIGHT,       DIALOG_OnRightArrow },
    { 3, VK_LEFT,        DIALOG_OnLeftArrow },
    { 3, VK_RETURN,      DIALOG_OnReturnKey },
    { 3, VK_ESCAPE,      DIALOG_OnEscapeKey },
    { 0, WM_CHAR,        DIALOG_OnCharKey },
    { 0, WM_MOUSEMOVE,   DIALOG_OnMouseMove },
};

static const D2SMsgRegEntryStrc s_aDialogWheelHandlers[1] = {
    { 0, WM_MOUSEWHEEL,  DIALOG_OnMouseWheel },
};

// ============================================================================
// Module Globals
// ============================================================================
extern D2DialogStrc* g_pActiveDialog;
extern int32_t g_bDialogOpen;
extern int32_t g_nActiveDialogCount;
extern D2CellFileStrc* g_pPentagramCel;
extern uint32_t g_dwPentagramFrame;
extern int32_t g_nDialogModalRefCount;
extern RECT g_rcDialogBounds;
extern int32_t g_bDialogBoundsValid;
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



// Forward declarations into reconstructed UI_dialog.cpp (0x6FB12960..0x6FB14190)
extern int32_t __fastcall DIALOG_GetSelectedIndex(D2DialogStrc* pDialog); // 0x6FB129C0
extern void __stdcall DIALOG_AppendLine(D2DialogStrc* pDialog, int nColor, const Unicode* wszText, int nLineHeight, int nUnkF4, int nFont, int (__cdecl* pfnAction)(void), int bSelectable); // 0x6FB12DF0
extern void __cdecl DIALOG_CloseActive(); // 0x6FB13290
extern void __stdcall DIALOG_OnMouseButtonUp(void* pMsg);
extern void __stdcall DIALOG_OnEscapeKey(void* pMsg);
extern void __stdcall DIALOG_OnMenuCancelHotkey(void* pMsg);
extern void __stdcall DIALOG_OnReturnKey(void* pMsg);
extern void __stdcall DIALOG_OnCharKey(void* pMsg);
extern void __stdcall DIALOG_OnDownArrow(void* pMsg);
extern void __stdcall DIALOG_OnUpArrow(void* pMsg);
extern void __stdcall DIALOG_OnLeftArrow(void* pMsg);
extern void __stdcall DIALOG_OnRightArrow(void* pMsg);
extern void __stdcall DIALOG_OnMouseButtonDown(void* pMsg);
extern void __stdcall DIALOG_OnMouseMove(void* pMsg);
extern D2DialogStrc* __stdcall DIALOG_CreateFramed(int nAnchorX, int nAnchorY, int (__cdecl* pfnCancel)(void), int (__cdecl* pfnOutsideClick)(void), int bBlockUnhandledKeys, int bUseNonSelectableHeightAnchor, int bAutoCalcBounds, int nWidth, int nHeight, int bCloseOnRightClick, int bIgnoreMenuHotkeys); // 0x6FB13600

static inline int32_t __cdecl sub_6FB129C0_shim(D2DialogStrc* pDialog) { return DIALOG_GetSelectedIndex(pDialog); }

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
    int nClassId = pNpcUnit ? D2UnitView(pNpcUnit)->dwClassId : -1;
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
    D2InventoryStrc* pInv = D2UnitView(pNpcUnit)->pInventory;
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
        DIALOG_CloseActive();
        g_pNpcMainMenuDialog = nullptr;
    }
}

// 1.12a: 0x6FAC84B0 (__cdecl)
void __cdecl sub_6FAC84B0()
{
    if (g_pNpcSubDialog) {
        DIALOG_CloseActive();
        g_pNpcSubDialog = nullptr;
    }
}

// 1.12a: 0x6FAC84D0 (__cdecl)
void __cdecl sub_6FAC84D0()
{
    if (g_pNpcPopupDialog) {
        DIALOG_CloseActive();
        g_pNpcPopupDialog = nullptr;
    }
}

// 1.12a: 0x6FAC8530 (__stdcall)
int __stdcall sub_6FAC8530(D2UnitStrc* pPlayer, D2UnitStrc* pItem, int nTransactionType)
{
    if (!pItem || !g_pClientQuestRecord) return 0;
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

void __cdecl sub_6FAC9760();
void __cdecl sub_6FAC9AF0();
extern int32_t __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t bMoveMouse);

// 1.12a: 0x6FAC8DD0 (__cdecl)
int __cdecl sub_6FAC8DD0()
{
    // Cain Identify Items (C2S 0x34)
    const uint32_t dwNpcGuid = static_cast<uint32_t>(g_nInteractingNpcGUID);
    sub_6FAC9AF0();
    if (dwNpcGuid != 0u) {
        MSG_SendByteDword(0x34, dwNpcGuid);
    }
    return 1;
}

// 1.12a: 0x6FAC8EE0 (__cdecl)
int __cdecl sub_6FAC8EE0()
{
    // Charsi Imbue / Socket / Personalize prompt
    sub_6FAC9760();
    UI_SetPanelState(D2PANEL_MODITEM, 0, 0);
    return 1;
}

// 1.12a: 0x6FAC8FF0 (__cdecl)
int __cdecl sub_6FAC8FF0()
{
    // Resurrect Mercenary (C2S 0x62)
    const uint32_t dwNpcGuid = static_cast<uint32_t>(g_nInteractingNpcGUID);
    sub_6FAC9AF0();
    if (dwNpcGuid != 0u) {
        MSG_SendByteDword(0x62, dwNpcGuid);
    }
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
    int nSelected = DIALOG_GetSelectedIndex(g_pNpcSubDialog);
    short nStrId = TEXT_GetStringIdOfMenuType2Node(g_pNpcTextHeader, nSelected);
    return nStrId != -1;
}

// 1.12a: 0x6FAC9760 (__cdecl)
void __cdecl sub_6FAC9760()
{
    extern void __fastcall D2Client_CloseNpcGossipSpeech(void);
    D2Client_CloseNpcGossipSpeech();
    sub_6FAC84D0();
    sub_6FAC84B0();
    sub_6FAC8450();
    gaPanelStates[D2PANEL_NPCMENU] = 0;
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
    g_nInteractingNpcGUID = 0;
    g_nInteractingNpcClassId = -1;
    g_pTradeNpcUnit = nullptr;
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
    extern int32_t g_nInvPanelMode;
    extern int32_t __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t bMoveMouse);
    // Request Trade Inventory (C2S 0x38, mode 1)
    sub_6FAC9760();
    g_nInvPanelMode = sub_6FAC7DF0(g_nInteractingNpcClassId) ? 1 : 3;
    MSG_SendByte3Dwords(0x38, 1, g_nInteractingNpcGUID, 0);
    UI_SetPanelState(D2PANEL_NPCSHOP, 0, 0);
    return 1;
}

int __cdecl sub_6FAC9D40()
{
    extern int32_t g_nInvPanelMode;
    extern int32_t __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t bMoveMouse);
    // Request Gamble Inventory (C2S 0x38, mode 2)
    sub_6FAC9760();
    g_nInvPanelMode = 2;
    MSG_SendByte3Dwords(0x38, 2, g_nInteractingNpcGUID, 0);
    UI_SetPanelState(D2PANEL_NPCSHOP, 0, 0);
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
    if (!pPlayer || !pItem || !g_pClientQuestRecord) return 0;
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
    (void)nActTarget;
    // Travel to Act (Warriv / Meshif / Tyrael): C2S 0x38 nAction=0, nItemGUID=0
    const uint32_t dwNpcGuid = static_cast<uint32_t>(g_nInteractingNpcGUID);
    sub_6FAC9760();
    g_bInteractingWithNpc = 0;
    g_nInteractingNpcGUID = 0;
    g_nInteractingNpcClassId = -1;
    g_pTradeNpcUnit = nullptr;
    if (dwNpcGuid != 0u) {
        MSG_SendByte3Dwords(0x38, 0, dwNpcGuid, 0);
    }
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
    return sub_6FACA340(0);
}

// 1.12a: 0x6FACB2B0 (__cdecl)
int __cdecl sub_6FACB2B0() { return sub_6FACA340(0); }

// 1.12a: 0x6FACB2E0 (__cdecl)
int __cdecl sub_6FACB2E0() { return sub_6FACA340(0); }

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

struct D2MercForHireEntryStrc
{
    uint16_t wNameIdx;
    uint16_t wPad;
    uint32_t dwMercSeed;
};
extern D2MercForHireEntryStrc g_aMercForHireEntries[16];
extern int32_t  g_nMercForHireCount;
extern uint16_t g_wMercDeadNameIdx;
extern uint32_t g_dwMercReviveCost;
extern int32_t  gbIsExpansion;
extern void __fastcall D2Client_FormatMercForHireRow(int32_t nIdx, wchar_t* wszOut, int32_t nMaxChars);
extern int32_t __fastcall UI_WorldUnitToScreen(D2UnitStrc* pUnit, int32_t* pOutScreenX, int32_t* pOutScreenY);
extern const Unicode* __fastcall D2Client_GetNpcDisplayName(D2UnitStrc* pUnit);
extern D2UnitStrc* __fastcall D2Client_FindServerSideUnit(int32_t nUnitId, int32_t nUnitType);
extern D2UnitStrc* __fastcall D2Client_GetLocalMercUnit(D2UnitStrc* pPlayer);
extern void __fastcall D2Client_OpenNpcGossipSpeech(D2UnitStrc* pNpc, int32_t nFallbackClassId);
extern void __fastcall D2Client_OpenNpcSpeechByTblId(D2UnitStrc* pNpc, int32_t nFallbackClassId, uint16_t wStringId);

static uint16_t s_awNpcQuestTopics[6] = {};
static int32_t  s_nNpcQuestTopicCount = 0;
static int32_t  s_bNpcHireDialogActive = 0;

void __fastcall NPCMENU_SetServerSpeechTopics(const uint16_t* pwTopics, int32_t nCount)
{
    s_nNpcQuestTopicCount = 0;
    memset(s_awNpcQuestTopics, 0, sizeof(s_awNpcQuestTopics));
    if (!pwTopics || nCount <= 0) return;
    for (int32_t i = 0; i < nCount && s_nNpcQuestTopicCount < 6; ++i) {
        if (pwTopics[i] != 0) {
            s_awNpcQuestTopics[s_nNpcQuestTopicCount++] = pwTopics[i];
        }
    }
}

int __stdcall sub_6FACCF00(D2UnitStrc* pNpcUnit);

static D2UnitStrc* NPCMENU_ResolveActiveNpcUnit()
{
    if (g_pTradeNpcUnit) return g_pTradeNpcUnit;
    if (g_nInteractingNpcGUID != 0) {
        return D2Client_FindServerSideUnit(g_nInteractingNpcGUID, 1 /* UNIT_MONSTER */);
    }
    return nullptr;
}

static void NPCMENU_GetNpcDialogAnchor(D2UnitStrc* pNpcUnit, int32_t* pOutX, int32_t* pOutY)
{
    int32_t nSx = 0, nSy = 0;
    if (pNpcUnit && UI_WorldUnitToScreen(pNpcUnit, &nSx, &nSy) && nSx > 0 && nSy > 0) {
        *pOutX = nSx;
        *pOutY = (nSy > 96) ? (nSy - 64) : 48;
        return;
    }
    *pOutX = (gnScreenWidth > 0) ? (gnScreenWidth / 2) : 400;
    *pOutY = (gnScreenHeight > 0) ? (gnScreenHeight / 3) : 180;
}

static int __cdecl NPCMENU_BackToMainMenu()
{
    s_bNpcHireDialogActive = 0;
    sub_6FAC84B0();
    D2UnitStrc* pNpc = NPCMENU_ResolveActiveNpcUnit();
    if (pNpc) {
        sub_6FACCF00(pNpc);
    } else {
        sub_6FAC9AF0();
    }
    return 1;
}

static int __cdecl NPCMENU_OnTalkIntro()
{
    D2UnitStrc* pNpc = NPCMENU_ResolveActiveNpcUnit();
    sub_6FAC84B0();
    for (int i = 0; i < 45; ++i) {
        if (g_aNpcGossipDefs[i].wNpcClassId == static_cast<uint16_t>(g_nInteractingNpcClassId)) {
            const uint16_t wIntroId = g_aNpcGossipDefs[i].wIntroIds[0];
            g_aNpcGossipDefs[i].bIntroShown = 1;
            if (wIntroId != 0) {
                D2Client_OpenNpcSpeechByTblId(pNpc, g_nInteractingNpcClassId, wIntroId);
                return 1;
            }
            break;
        }
    }
    D2Client_OpenNpcGossipSpeech(pNpc, g_nInteractingNpcClassId);
    return 1;
}

static int __cdecl NPCMENU_OnTalkGossip()
{
    D2UnitStrc* pNpc = NPCMENU_ResolveActiveNpcUnit();
    sub_6FAC84B0();
    D2Client_OpenNpcGossipSpeech(pNpc, g_nInteractingNpcClassId);
    return 1;
}

template <int kSlot>
static int __cdecl NPCMENU_OnTalkQuestTopic()
{
    D2UnitStrc* pNpc = NPCMENU_ResolveActiveNpcUnit();
    sub_6FAC84B0();
    if (kSlot < s_nNpcQuestTopicCount && s_awNpcQuestTopics[kSlot] != 0) {
        D2Client_OpenNpcSpeechByTblId(pNpc, g_nInteractingNpcClassId, s_awNpcQuestTopics[kSlot]);
    } else {
        D2Client_OpenNpcGossipSpeech(pNpc, g_nInteractingNpcClassId);
    }
    return 1;
}

template <int kSlot>
static int __cdecl NPCMENU_OnHireMercCandidate()
{
    s_bNpcHireDialogActive = 0;
    const uint32_t dwNpcGuid = static_cast<uint32_t>(g_nInteractingNpcGUID);
    const uint16_t wNameIdx = (kSlot < g_nMercForHireCount) ? g_aMercForHireEntries[kSlot].wNameIdx : 0u;
    sub_6FAC9AF0();
    if (dwNpcGuid != 0u && wNameIdx != 0u) {
        MSG_SendByte2Dwords(0x36, dwNpcGuid, static_cast<uint32_t>(wNameIdx));
    }
    return 1;
}

void __fastcall NPCMENU_RefreshHireSubDialog(void)
{
    if (!s_bNpcHireDialogActive || !g_bInteractingWithNpc) {
        return;
    }
    sub_6FAC84B0();
    D2UnitStrc* pNpc = NPCMENU_ResolveActiveNpcUnit();
    int32_t nAnchorX = 400, nAnchorY = 180;
    NPCMENU_GetNpcDialogAnchor(pNpc, &nAnchorX, &nAnchorY);
    g_pNpcSubDialog = DIALOG_CreateFramed(nAnchorX, nAnchorY, NPCMENU_BackToMainMenu, nullptr, 0, 1, 1, 0, 0, 0, 0);
    if (!g_pNpcSubDialog) {
        return;
    }
    gaPanelStates[D2PANEL_NPCMENU] = 1;
    const Unicode* wszTitle = D2LANG_GetStringFromTblIndex(3397); // "hire"
    if (wszTitle && wszTitle->ch != 0) {
        DIALOG_AppendLine(g_pNpcSubDialog, 4, wszTitle, 21, 0, 1, nullptr, 0);
    }

    static int (__cdecl* const s_apfnHireSlots[6])(void) = {
        NPCMENU_OnHireMercCandidate<0>, NPCMENU_OnHireMercCandidate<1>,
        NPCMENU_OnHireMercCandidate<2>, NPCMENU_OnHireMercCandidate<3>,
        NPCMENU_OnHireMercCandidate<4>, NPCMENU_OnHireMercCandidate<5>,
    };
    for (int32_t i = 0; i < g_nMercForHireCount && i < 6; ++i) {
        wchar_t wszRow[96] = {};
        D2Client_FormatMercForHireRow(i, wszRow, 96);
        if (wszRow[0] != 0) {
            DIALOG_AppendLine(g_pNpcSubDialog, 0, reinterpret_cast<const Unicode*>(wszRow), 18, 0, 1, s_apfnHireSlots[i], 1);
        }
    }

    const Unicode* wszCancel = D2LANG_GetStringFromTblIndex(3400); // "cancel"
    if (wszCancel) {
        DIALOG_AppendLine(g_pNpcSubDialog, 0, wszCancel, 18, 0, 1, NPCMENU_BackToMainMenu, 1);
    }
}

// 1.12a: 0x6FACBB80 (__cdecl)
int __cdecl sub_6FACBB80()
{
    // Open Hire Mercenary Selection Dialog & Request Hireling List (C2S 0x38, mode 3)
    sub_6FAC8450();
    s_bNpcHireDialogActive = 1;
    if (g_nInteractingNpcGUID != 0) {
        MSG_SendByte3Dwords(0x38, 3, static_cast<uint32_t>(g_nInteractingNpcGUID), 0);
    }
    NPCMENU_RefreshHireSubDialog();
    return 1;
}

// 1.12a: 0x6FACBE50 (__stdcall)
int __stdcall sub_6FACBE50(int bFromSubMenu)
{
    (void)bFromSubMenu;
    // Open NPC Talk Sub-Menu (Introduction + Gossip + Active Quest Speeches + Cancel)
    s_bNpcHireDialogActive = 0;
    sub_6FAC8450();
    sub_6FAC84B0();

    D2UnitStrc* pNpc = NPCMENU_ResolveActiveNpcUnit();
    int32_t nAnchorX = 400, nAnchorY = 180;
    NPCMENU_GetNpcDialogAnchor(pNpc, &nAnchorX, &nAnchorY);
    g_pNpcSubDialog = DIALOG_CreateFramed(nAnchorX, nAnchorY, NPCMENU_BackToMainMenu, nullptr, 0, 1, 1, 0, 0, 0, 0);
    if (!g_pNpcSubDialog) {
        return 0;
    }
    gaPanelStates[D2PANEL_NPCMENU] = 1;

    const Unicode* wszTalkHdr = D2LANG_GetStringFromTblIndex(3381); // "talk"
    if (wszTalkHdr && wszTalkHdr->ch != 0) {
        DIALOG_AppendLine(g_pNpcSubDialog, 4, wszTalkHdr, 21, 0, 1, nullptr, 0);
    }

    bool bShowIntro = false;
    for (int i = 0; i < 45; ++i) {
        if (g_aNpcGossipDefs[i].wNpcClassId == static_cast<uint16_t>(g_nInteractingNpcClassId)) {
            if (g_aNpcGossipDefs[i].bIntroShown == 0 && g_aNpcGossipDefs[i].wIntroIds[0] != 0) {
                bShowIntro = true;
            }
            break;
        }
    }
    if (bShowIntro) {
        if (const Unicode* wszIntro = D2LANG_GetStringFromTblIndex(3399)) { // "introduction"
            DIALOG_AppendLine(g_pNpcSubDialog, 0, wszIntro, 18, 0, 1, NPCMENU_OnTalkIntro, 1);
        }
    }

    static int (__cdecl* const s_apfnTopicSlots[6])(void) = {
        NPCMENU_OnTalkQuestTopic<0>, NPCMENU_OnTalkQuestTopic<1>,
        NPCMENU_OnTalkQuestTopic<2>, NPCMENU_OnTalkQuestTopic<3>,
        NPCMENU_OnTalkQuestTopic<4>, NPCMENU_OnTalkQuestTopic<5>,
    };
    for (int32_t t = 0; t < s_nNpcQuestTopicCount && t < 6; ++t) {
        if (const Unicode* wszTopic = D2LANG_GetStringFromTblIndex(static_cast<short>(s_awNpcQuestTopics[t]))) {
            if (wszTopic->ch != 0) {
                DIALOG_AppendLine(g_pNpcSubDialog, 0, wszTopic, 18, 0, 1, s_apfnTopicSlots[t], 1);
            }
        }
    }

    if (const Unicode* wszGossip = D2LANG_GetStringFromTblIndex(3395)) { // "gossip"
        DIALOG_AppendLine(g_pNpcSubDialog, 0, wszGossip, 18, 0, 1, NPCMENU_OnTalkGossip, 1);
    }

    if (const Unicode* wszCancel = D2LANG_GetStringFromTblIndex(3400)) { // "cancel"
        DIALOG_AppendLine(g_pNpcSubDialog, 0, wszCancel, 18, 0, 1, NPCMENU_BackToMainMenu, 1);
    }
    return 1;
}

// 1.12a: 0x6FACC100 (__cdecl)
int __cdecl sub_6FACC100()
{
    if (g_nInteractingNpcGUID != 0) {
        MSG_SendByte3Dwords(0x38, 0, static_cast<uint32_t>(g_nInteractingNpcGUID), 0);
    }
    return sub_6FACBE50(0);
}

// 1.12a: 0x6FACC130 (__stdcall)
void __stdcall sub_6FACC130(int nTab)
{
    (void)nTab;
}

// 1.12a: 0x6FACC2E0 (__cdecl)
int __cdecl sub_6FACC2E0()
{
    return sub_6FACBB80();
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
    (void)bGamble;
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
    return 1;
}

int __cdecl sub_6FACCC00()
{
    return sub_6FACCB10();
}

static bool NPCMENU_IsQuestCompleted(int nQuestStateFlag)
{
    if (!g_pClientQuestRecord) return false;
    return QUESTRECORD_GetQuestState(g_pClientQuestRecord, nQuestStateFlag, 0) != 0 ||
           QUESTRECORD_GetQuestState(g_pClientQuestRecord, nQuestStateFlag, 13) != 0;
}

static bool NPCMENU_IsQuestRewardPending(int nQuestStateFlag)
{
    if (!g_pClientQuestRecord) return false;
    return QUESTRECORD_GetQuestState(g_pClientQuestRecord, nQuestStateFlag, 1) != 0;
}

// 1.12a: 0x6FACCF00 (__stdcall)
// 1.12a: 0x6FACD250 (internal entry point)
int __stdcall sub_6FACCF00(D2UnitStrc* pNpcUnit)
{
    // Build & Open Main Town NPC Interaction Menu Dialog
    int nMenuIdx = sub_6FAC7D50(pNpcUnit);
    if (nMenuIdx < 0 || !g_aNpcMenuDefs[nMenuIdx].bHasMenu) return 0;

    s_bNpcHireDialogActive = 0;
    sub_6FAC9760();
    g_nActiveNpcMenuIdx = nMenuIdx;
    if (pNpcUnit) {
        g_pTradeNpcUnit = pNpcUnit;
        g_bInteractingWithNpc = 1;
        g_nInteractingNpcGUID = D2UnitView(pNpcUnit)->dwUnitId;
        g_nInteractingNpcClassId = D2UnitView(pNpcUnit)->dwClassId;
    }
    if (!g_pPentagramCel) {
        g_pPentagramCel = ARCHIVE_LoadCellFileDefault("CURSOR\\focus16", 0);
    }

    int32_t nAnchorX = 400, nAnchorY = 180;
    NPCMENU_GetNpcDialogAnchor(pNpcUnit, &nAnchorX, &nAnchorY);

    g_pNpcMainMenuDialog = DIALOG_CreateFramed(nAnchorX, nAnchorY, sub_6FAC9B40, nullptr, 0, 1, 1, 0, 0, 0, 0);
    if (!g_pNpcMainMenuDialog) {
        return 0;
    }
    gaPanelStates[D2PANEL_NPCMENU] = 1;

    const Unicode* wszNpcName = D2Client_GetNpcDisplayName(pNpcUnit);
    if (wszNpcName && wszNpcName->ch != 0) {
        DIALOG_AppendLine(g_pNpcMainMenuDialog, 4, wszNpcName, 21, 0, 1, nullptr, 0);
    }

    const D2NpcMenuDefStrc& rMenuDef = g_aNpcMenuDefs[nMenuIdx];
    for (int i = 0; i < 5; ++i) {
        if (rMenuDef.wStringIds[i] != 0 && rMenuDef.pfnCallbacks[i] != nullptr) {
            const Unicode* wszOpt = D2LANG_GetStringFromTblIndex(rMenuDef.wStringIds[i]);
            if (wszOpt) {
                DIALOG_AppendLine(
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

    // Evaluate Quest-Gated & Mercenary Revival Dynamic Menu Options (g_aNpcQuestActionDefs)
    const int32_t nCls = g_nInteractingNpcClassId;
    const bool bIsExp = (FOG_IsExpansion() && gbIsExpansion != 0);
    extern D2UnitStrc* gpPlayerUnit;

    // 1. Kashya (150, A1Q2) & Qual-Kehk (515, A5Q2) quest-gated "hire" (3397)
    if ((nCls == 150 && NPCMENU_IsQuestCompleted(2)) ||
        (nCls == 515 && bIsExp && NPCMENU_IsQuestCompleted(36))) {
        if (const Unicode* wszHire = D2LANG_GetStringFromTblIndex(3397)) {
            DIALOG_AppendLine(g_pNpcMainMenuDialog, 0, wszHire, 18, 0, 1, sub_6FACBB80, 1);
        }
    }

    // 2. Mercenary Captains "Resurrect" (22696 / 22695) when player's merc is dead
    if ((nCls == 150 || nCls == 198 || nCls == 252 || nCls == 367 || nCls == 521 || nCls == 515) &&
        g_dwMercReviveCost > 0u && (!gpPlayerUnit || D2Client_GetLocalMercUnit(gpPlayerUnit) == nullptr)) {
        wchar_t wszResRow[80] = {};
        const Unicode* pDeadName = g_wMercDeadNameIdx ? D2LANG_GetStringFromTblIndex(static_cast<short>(g_wMercDeadNameIdx)) : nullptr;
        if (pDeadName && pDeadName->ch != 0) {
            _snwprintf(wszResRow, 79, L"Resurrect %ls: %u", reinterpret_cast<const wchar_t*>(pDeadName), g_dwMercReviveCost);
        } else {
            _snwprintf(wszResRow, 79, L"Resurrect: %u", g_dwMercReviveCost);
        }
        wszResRow[79] = 0;
        DIALOG_AppendLine(g_pNpcMainMenuDialog, 0, reinterpret_cast<const Unicode*>(wszResRow), 18, 0, 1, sub_6FAC8FF0, 1);
    }

    // 3. Quest-gated Item Rewards: Charsi Imbue (154, A1Q3 -> 4017), Larzuk Sockets (511, A5Q1 -> 22748), Anya Personalize (512, A5Q4 -> 22749)
    if (nCls == 154 && NPCMENU_IsQuestRewardPending(3)) {
        if (const Unicode* wszImbue = D2LANG_GetStringFromTblIndex(4017)) {
            DIALOG_AppendLine(g_pNpcMainMenuDialog, 0, wszImbue, 18, 0, 1, sub_6FAC8EE0, 1);
        }
    } else if (nCls == 511 && bIsExp && NPCMENU_IsQuestRewardPending(35)) {
        if (const Unicode* wszSock = D2LANG_GetStringFromTblIndex(22748)) {
            DIALOG_AppendLine(g_pNpcMainMenuDialog, 0, wszSock, 18, 0, 1, sub_6FAC8EE0, 1);
        }
    } else if (nCls == 512 && bIsExp && NPCMENU_IsQuestRewardPending(38)) {
        if (const Unicode* wszPers = D2LANG_GetStringFromTblIndex(22749)) {
            DIALOG_AppendLine(g_pNpcMainMenuDialog, 0, wszPers, 18, 0, 1, sub_6FAC8EE0, 1);
        }
    }

    // 4. Quest-gated Act Travel: Warriv1 Go East (155, A1Q6 -> 3382), Meshif1 Sail East (210, A2Q6 -> 3384), Tyrael Travel to Harrogath (367/521, A4Q2 -> 22738)
    if (nCls == 155 && (NPCMENU_IsQuestCompleted(6) || NPCMENU_IsQuestCompleted(7))) {
        if (const Unicode* wszEast = D2LANG_GetStringFromTblIndex(3382)) {
            DIALOG_AppendLine(g_pNpcMainMenuDialog, 0, wszEast, 18, 0, 1, sub_6FACB2E0, 1);
        }
    } else if (nCls == 210 && (NPCMENU_IsQuestCompleted(14) || NPCMENU_IsQuestCompleted(15))) {
        if (const Unicode* wszSailEast = D2LANG_GetStringFromTblIndex(3384)) {
            DIALOG_AppendLine(g_pNpcMainMenuDialog, 0, wszSailEast, 18, 0, 1, sub_6FACB2B0, 1);
        }
    } else if ((nCls == 367 || nCls == 521) && bIsExp && (NPCMENU_IsQuestCompleted(26) || NPCMENU_IsQuestCompleted(28))) {
        if (const Unicode* wszHarrogath = D2LANG_GetStringFromTblIndex(22738)) {
            DIALOG_AppendLine(g_pNpcMainMenuDialog, 0, wszHarrogath, 18, 0, 1, sub_6FACB280, 1);
        }
    }

    // 5. Cancel (TBL 3400 = "cancel")
    const Unicode* wszCancel = D2LANG_GetStringFromTblIndex(3400);
    if (wszCancel) {
        DIALOG_AppendLine(g_pNpcMainMenuDialog, 0, wszCancel, 18, 0, 1, sub_6FAC9B40, 1);
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
    D2InventoryStrc* pInv = D2UnitView(pUnit)->pInventory;
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
        for (D2UnitStrc* pUnit = ppTable[i]; pUnit; pUnit = D2UnitView(pUnit)->pListNext) {
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
    for (D2UnitStrc* pUnit = ppTable[nUnitType * 128 + dwHash]; pUnit; pUnit = D2UnitView(pUnit)->pListNext) {
        if (D2UnitView(pUnit)->dwUnitId == nUnitGUID) {
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
    int nType = D2UnitView(pClientUnit)->dwUnitType;
    int nGUID = D2UnitView(pClientUnit)->dwUnitId;
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
    return pUnit ? D2UnitView(pUnit)->dwAnimMode : 0;
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
    return pUnit ? D2UnitView(pUnit)->dwClassId : -1;
}

// 1.12a: 0x6FACEAC0 (register: eax=pUnit)
int __fastcall sub_6FACEAC0(D2UnitStrc* pUnit)
{
    extern int32_t __fastcall D2Client_IsUnitDeadOrDying(D2UnitStrc* pUnit);
    return pUnit ? D2Client_IsUnitDeadOrDying(pUnit) : 0;
}

// 1.12a: 0x6FACEB00 (register: eax=pUnit)
D2RoomStrc* __fastcall sub_6FACEB00(D2UnitStrc* pUnit)
{
    return pUnit ? UNITS_GetRoom(pUnit) : nullptr;
}

// 1.12a: 0x6FACEB20 (__stdcall)
D2UnitStrc* __stdcall sub_6FACEB20(D2UnitStrc* pUnit)
{
    D2InventoryStrc* pInv = pUnit ? D2UnitView(pUnit)->pInventory : nullptr;
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
