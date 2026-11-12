// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Translation Unit
// Source Files:
//   - ..\Source\D2Client\UI\Hireables.cpp (0x6FB6AA00..0x6FB6EEB0, 83 functions)
//   - ..\Source\D2Client\UI\Party.cpp     (0x6FB0FDD0..0x6FB12960, 36 functions)
// ImageBase   : 0x6FAB0000
//
// Cross-referenced against:
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Units.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2DataTbls.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Items.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Dungeon.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Environment.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Seed.h
//   - H:/git/D2MOO-112a/source/D2Gfx/include/D2Gfx.h
//   - H:/git/D2MOO-112a/source/D2Gfx/include/Texture.h
//   - H:/git/D2MOO-112a/source/D2Gfx/include/Tile.h
//   - H:/git/D2MOO-112a/source/D2Gfx/include/Scale.h
//   - H:/git/D2MOO-112a/source/D2CMP/include/CelCmp.h
//   - H:/git/D2MOO-112a/source/D2Lang/include/D2Unicode.h
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// Forward Declarations & External Imports
// ----------------------------------------------------------------------------
struct D2UnitStrc;
struct D2ActiveRoomStrc;
typedef D2ActiveRoomStrc D2RoomStrc;
struct D2GfxLightStrc;
struct D2CellFileStrc;
struct D2GfxDataStrc;
struct D2TileLibraryEntryStrc;
struct D2SeedStrc;
struct D2WindowMsgStrc;
struct Unicode;

// D2Common.dll
extern "C" {
    int32_t __stdcall SEED_GetRandomValue(int32_t nValue);                             // D2Common.#10043
    void __fastcall SEED_InitLowSeed(D2SeedStrc* pSeed, int32_t nLowSeed);             // D2Common.#10073
    D2UnitStrc* __stdcall INVENTORY_GetCursorItem(void* pInventory);                   // D2Common.#10078
    int32_t __stdcall UNITS_GetClientCoordX(D2UnitStrc* pUnit);                        // D2Common.#10098
    int32_t __stdcall ITEMS_CheckItemTypeId(D2UnitStrc* pItem, int32_t nItemType);     // D2Common.#10231
    int32_t __stdcall UNITS_TestCollisionWithUnit(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int32_t nCollisionMask); // D2Common.#10233
    int32_t __stdcall STATES_CheckState(D2UnitStrc* pUnit, int32_t nState);            // D2Common.#10295
    D2RoomStrc* __stdcall UNITS_GetRoom(D2UnitStrc* pUnit);                            // D2Common.#10366
    int32_t __stdcall DUNGEON_IsRoomInTown(D2RoomStrc* pRoom);                         // D2Common.#10438
    int32_t __stdcall UNITS_GetClientCoordY(D2UnitStrc* pUnit);                        // D2Common.#10467
    int32_t __stdcall ITEMS_CheckWeaponIfTwoHanded(D2UnitStrc* pItem);                 // D2Common.#10480
    int32_t __stdcall DUNGEON_GetLevelIdFromRoom(D2RoomStrc* pRoom);                   // D2Common.#10511
    void* __stdcall DATATBLS_GetObjectsTxtRecord(int32_t nObjectId);                   // D2Common.#10670
    int32_t __stdcall DRLG_GetLOSDrawFromLevelId(int32_t nLevelId);                    // D2Common.#10698
    int32_t __stdcall UNITS_GetAnimOrSeqMode(D2UnitStrc* pUnit);                       // D2Common.#10706
    void __stdcall DUNGEON_ClientToGameSubtileCoords(int32_t* pX, int32_t* pY);        // D2Common.#10725
    int32_t __stdcall UNITS_GetPrecisionY(D2UnitStrc* pUnit);                          // D2Common.#10732
    void __stdcall ENVIRONMENT_GetLightColorFromAct(void* pAct, uint8_t* pR, uint8_t* pG, uint8_t* pB); // D2Common.#10744
    void __stdcall DUNGEON_GameSubtileToClientCoords(int32_t* pX, int32_t* pY);        // D2Common.#10912
    void __stdcall DUNGEON_GetRGB_IntensityFromRoom(D2RoomStrc* pRoom, uint8_t* pIntensity, uint8_t* pR, uint8_t* pG, uint8_t* pB); // D2Common.#10922
    int32_t __stdcall UNITS_GetPrecisionX(D2UnitStrc* pUnit);                          // D2Common.#10984
    int32_t __stdcall ITEMS_CheckItemFlag(D2UnitStrc* pItem, uint32_t dwFlag, int32_t nLine, const char* szFile); // D2Common.#10989
    uint8_t __stdcall ENVIRONMENT_GetIntensityFromAct(void* pAct);                     // D2Common.#10998
    int32_t __stdcall ITEMS_CheckRequirements(D2UnitStrc* pItem, D2UnitStrc* pUnit, int32_t bEquip, int32_t* pReqStr, int32_t* pReqDex, int32_t* pReqLvl); // D2Common.#11075
    void __fastcall SEED_InitSeed(D2SeedStrc* pSeed);                                  // D2Common.#11156
}

int32_t __stdcall TILE_TileDrawLit(D2TileLibraryEntryStrc* pTile, int32_t nX, int32_t nY, D2GfxLightStrc* pLight, int32_t nScreenPanels); // D2Gfx.#10014
int32_t __stdcall TILE_TileDrawTrans(D2TileLibraryEntryStrc* pTile, int32_t nX, int32_t nY, D2GfxLightStrc* pLight, int32_t nScreenPanels, uint8_t nAlpha); // D2Gfx.#10075

// D2Gfx.dll
extern "C" {
    int32_t __stdcall SCALE_SetPerspectiveScale(int32_t nX, int32_t nY);               // D2Gfx.#10015
    void __stdcall D2GFX_DebugDraw(void* pData1, void* pData2, int32_t nX, int32_t nY);// D2Gfx.#10021
    void __stdcall TEXTURE_CelDraw(void* pData, int32_t nX, int32_t nY, uint32_t dwGamma, int32_t nDrawMode, uint8_t* pPalette); // D2Gfx.#10024
    void __stdcall TEXTURE_CelDrawColor(void* pData, int32_t nX, int32_t nY, uint32_t dwGamma, int32_t nDrawMode, int32_t nColor); // D2Gfx.#10073
    void __stdcall D2GFX_DrawLine(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint8_t nColor, uint8_t nAlpha); // D2Gfx.#10031
    void __stdcall SCALE_UpdateFactor();                                               // D2Gfx.#10041
    int32_t __stdcall TILE_ShadowTileDraw(D2TileLibraryEntryStrc* pTile, int32_t nX, int32_t nY, int32_t nDrawMode, int32_t nScreenPanels); // D2Gfx.#10042
    int32_t __stdcall D2GFX_GetResolutionMode();                                       // D2Gfx.#10043
    void __stdcall SCALE_SetDefaultFactorPerspectiveFactor();                          // D2Gfx.#10049
    void __stdcall SCALE_ScalePerspectivePosition(int32_t nX, int32_t nY, int32_t nAngle, int32_t* pX, int32_t* pY, int32_t bOrder); // D2Gfx.#10052
    void __stdcall D2GFX_DrawRectFrame(RECT* pRect);                                   // D2Gfx.#10056
    void __stdcall SCALE_SetDefaultFactor();                                           // D2Gfx.#10060
    void __stdcall D2GFX_DrawBox(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, int32_t nTransTbl); // D2Gfx.#10062
    void __stdcall SCALE_CheckPerspectivePosition(int32_t nX, int32_t nY);             // D2Gfx.#10064
    int32_t __stdcall D2GFX_CheckPerspective();                                        // D2Gfx.#10071
    void __fastcall D2GFX_SetViewMatrixAdjust(int32_t nAdjust);                        // D2Gfx.#10072
    HWND __stdcall WINDOW_GetWindow();                                                 // D2Gfx.#10078
    int32_t __stdcall SCALE_GetFactor();                                               // D2Gfx.#10081
}

// D2CMP.dll
extern "C" {
    int32_t __stdcall D2CMP_CelGetWidth(void* pCelHandle);                             // D2CMP.#10031
    void* __stdcall D2CMP_CelGetHandle(void* pData);                                   // D2CMP.#10038
    int32_t __stdcall D2CMP_CelGetHeight(void* pCelHandle);                            // D2CMP.#10096
    int32_t __stdcall D2CMP_CelFileFreeHardware(D2CellFileStrc* pCellFile);            // D2CMP.#10106
}

// D2Win.dll
extern "C" {
    void __fastcall D2Win_10117_DrawText(const Unicode* wStr, int32_t xPos, int32_t yPos, int32_t dwColor, int32_t bCentered); // D2Win.#10001
    int32_t __fastcall D2Win_10127_SetFont(int32_t dwFont);                            // D2Win.#10010
    void __fastcall D2Win_10130_DrawFramedTextEx(const Unicode* wStr, int32_t xPos, int32_t yPos, int32_t nColor, int32_t nAlign); // D2Win.#10031
    void __fastcall D2Win_10129_DrawFramedText(const Unicode* wStr, int32_t xPos, int32_t yPos, int32_t nColor, int32_t nAlign); // D2Win.#10064
    uint8_t __stdcall D2Win_10034_MixRGB(uint8_t nRed, uint8_t nGreen, uint8_t nBlue); // D2Win.#10097
    int32_t __fastcall D2Win_10128_GetFontHeight();                                    // D2Win.#10125
    void __fastcall D2Win_10178(void* pData);                                          // D2Win.#10126
    int32_t __fastcall D2Win_10121_GetTextWidth(const Unicode* wStr);                  // D2Win.#10132
}

// D2Lang.dll
extern "C" {
    Unicode* __fastcall D2LANG_GetStringFromTblIndex(uint16_t nIndex);                 // D2Lang.#10005
    int32_t __fastcall Unicode_strlen(const Unicode* pStr);                            // D2Lang.?strlen@Unicode@@SIHPBU1@@Z
    Unicode* __fastcall Unicode_strncpy(Unicode* pDst, const Unicode* pSrc, int32_t nCount); // D2Lang.?strncpy@Unicode@@SIPAU1@PAU1@PBU1@H@Z
    Unicode* __fastcall Unicode_strcpy(Unicode* pDst, const Unicode* pSrc);            // D2Lang.?strcpy@Unicode@@SIPAU1@PAU1@PBU1@@Z
    Unicode* __fastcall Unicode_strcat(Unicode* pDst, const Unicode* pSrc);            // D2Lang.?strcat@Unicode@@SIPAU1@PAU1@PBU1@@Z
    Unicode* __fastcall Unicode_toUnicode(Unicode* pDst, const char* szSrc, int32_t nCount); // D2Lang.?toUnicode@Unicode@@SIPAU1@PAU1@PBDH@Z
    void __cdecl Unicode_sprintf(int32_t nBufferSize, Unicode* pDst, const Unicode* pFormat, ...); // D2Lang.?sprintf@Unicode@@SAXHPAU1@PBU1@ZZ
}

// Fog.dll
extern "C" {
    void __cdecl FOG_DisplayHalt(const char* szMsg, const char* szFile, int32_t nLine);// Fog.#10024
    void* __fastcall FOG_Alloc(int32_t nSize, const char* szFile, int32_t nLine, int32_t nReserved); // Fog.#10042
    void __fastcall FOG_Free(void* pMem, const char* szFile, int32_t nLine, int32_t nReserved);      // Fog.#10043
    void* __fastcall FOG_AllocPool(void* pMemPool, int32_t nSize, const char* szFile, int32_t nLine, int32_t nReserved); // Fog.#10045
    void __fastcall FOG_FreePool(void* pMemPool, void* pMem, const char* szFile, int32_t nLine, int32_t nReserved);      // Fog.#10046
    int32_t __stdcall FOG_IsExpansion();                                               // Fog.#10227
    const char* __cdecl FOG_GetErrorLocation(int32_t nLine);                           // Fog.#10265
}

// Storm.dll
extern "C" {
    void* __stdcall SMemAlloc(uint32_t amount, const char* logfilename, int32_t logline, uint32_t defaultValue); // Storm.#401
    BOOL __stdcall SRegLoadValue(const char* keyname, const char* valuename, uint8_t flags, uint32_t* value);    // Storm.#423
    BOOL __stdcall SRegSaveValue(const char* keyname, const char* valuename, uint8_t flags, uint32_t result);    // Storm.#426
    void __stdcall SMsgBreakHandlerChain(D2WindowMsgStrc* pMsg);                       // Storm.#511
}

// ----------------------------------------------------------------------------
// Data Structures
// ----------------------------------------------------------------------------
struct Unicode
{
    uint16_t wChar;
};

struct D2WindowMsgStrc
{
    HWND hWnd;                  // +0x00
    uint32_t uMsg;              // +0x04
    uint16_t wKey;              // +0x08
    int16_t nWheelDelta;        // +0x0A
    uint16_t wX;                // +0x0C
    uint16_t wY;                // +0x0E
    uint32_t dwParam10;         // +0x10
    uint32_t dwParam14;         // +0x14
    int32_t bHandled;           // +0x18
    int32_t nReturnValue;       // +0x1C
};

#include "D2ClientReconstructed.h"

struct D2RosterRelationStrc
{
    int32_t nUnitGuid;          // +0x00
    int32_t nRelationFlags;     // +0x04
    D2RosterRelationStrc* pNext;// +0x08
};

struct D2RosterUnitStrc
{
    char szName[16];            // +0x00
    int32_t dwUnitId;           // +0x10
    int32_t dwPartyLife;        // +0x14
    int32_t dwKills;            // +0x18
    int32_t dwClassId;          // +0x1C
    uint16_t wLevel;            // +0x20
    uint16_t wPartyId;          // +0x22
    int32_t dwLevelId;          // +0x24
    int32_t nXpos;              // +0x28
    int32_t nYpos;              // +0x2C
    int32_t dwPartyFlags;       // +0x30
    D2RosterRelationStrc** ppRelationList; // +0x34
    uint8_t aPad38[0x2E];       // +0x38
    char szNameEx[26];          // +0x66
    D2RosterUnitStrc* pNext;    // +0x80
};

struct D2PetInfoStrc
{
    int32_t nClassId;           // +0x00
    int32_t nPetType;           // +0x04
    int32_t nUnitGuid;          // +0x08
    int32_t nOwnerGuid;         // +0x0C
    int32_t nPetNameStringId;   // +0x10
    int32_t nScreenIconX;       // +0x14
    int32_t nScreenIconY;       // +0x18
    int32_t nLifePercent;       // +0x1C
    int32_t nOmitDraw;          // +0x20
    uint32_t dwMonSeed;         // +0x24
    uint32_t dwNameIdx;         // +0x28
    uint32_t dwSpecialFlag;     // +0x2C
    D2PetInfoStrc* pNext;       // +0x30
};

#pragma pack(push, 1)
struct D2PetTypeTxt
{
    uint32_t dwPetType;         // +0x00
    uint8_t nFlags;             // +0x04
    uint8_t aPad05[7];          // +0x05
    uint16_t wNameStr;          // +0x0C
    uint8_t nIconType;          // +0x0E
    char szBaseIcon[32];        // +0x0F
    char szAltIcon[4][32];      // +0x2F
    uint8_t aPadAF[3];          // +0xAF
    uint16_t wClass[4];         // +0xB2
    uint8_t aPadBA[0x26];       // +0xBA..0xDF
};
#pragma pack(pop)

struct D2PartyButtonStrc
{
    int32_t nButtonType;        // +0x00 (0 = 20x20 square button, 1 = 53x20 wide party button)
    int32_t nX;                 // +0x04
    int32_t nY;                 // +0x08
    D2CellFileStrc* pCellFile;  // +0x0C
    int32_t nFrame;             // +0x10
    int32_t nState;             // +0x14
    int32_t nTextStrId;         // +0x18
    int32_t bPressed;           // +0x1C
};

#pragma pack(push, 1)
struct D2PartyScreenNodeStrc
{
    D2PartyButtonStrc tMainBtn;     // +0x00 (Party / Invite / Accept / Leave)
    D2PartyButtonStrc tHostileBtn;  // +0x20 (Hostile / Unhostile)
    D2PartyButtonStrc tLootBtn;     // +0x40 (Loot Corpse toggle)
    D2PartyButtonStrc tMuteBtn;     // +0x60 (Squelch / Unsquelch)
    D2PartyButtonStrc tIgnoreBtn;   // +0x80 (Hear / Ignore party invites)
    char szName[25];                // +0xA0
    int32_t nClassId;               // +0xB9
    int32_t nLevel;                 // +0xBD
    int32_t nPartyLeaderGuid;       // +0xC1
    int32_t nUnitGuid;              // +0xC5
    int32_t nRowY;                  // +0xC9
    D2PartyScreenNodeStrc* pNext;   // +0xCD
};
#pragma pack(pop)

// ----------------------------------------------------------------------------
// Reconstructed Static Data Tables (.rdata / .data)
// ----------------------------------------------------------------------------

// 1.12a: 0x6FB80960 (Tile Orientation Light Vertex Count Table)
static const uint32_t g_aTileLightVertexCounts[20] = {
    0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0
};

// 1.12a: 0x6FB8C6F0 (Tile Light Subtile Offsets Table 0 - 10 orientations x 6 vertices x {dx, dy})
static const int32_t g_aTileLightOffsets0[10][12] = {
    {  0, 0,  0, 0,  0, 0,  0,  0,  0,  0,  0,  0 },
    {  2, 5,  2, 3,  2, 1,  2, -1,  2, -3,  2, -5 },
    { -5, 2, -3, 2, -1, 2,  1,  2,  3,  2,  5,  2 },
    {  2, 5,  2, 3,  2, 2,  2,  2,  3,  2,  5,  2 },
    { -5, 2, -3, 2, -1, 2,  2, -1,  2, -3,  2, -5 },
    {  0, 0,  0, 0,  0, 0,  0,  0,  0,  0,  0,  0 },
    {  2, 5,  2, 3,  2, 1,  2, -1,  2, -3,  2, -5 },
    { -5, 2, -3, 2, -1, 2,  1,  2,  3,  2,  5,  2 },
    {  2, 5,  2, 3,  2, 2,  2,  2,  3,  2,  5,  2 },
    { -5, 2, -3, 2, -1, 2,  2, -1,  2, -3,  2, -5 },
};

// 1.12a: 0x6FB8C8D0 (Tile Light Subtile Offsets Table 1 - 10 orientations x 6 vertices x {dx, dy})
static const int32_t g_aTileLightOffsets1[10][12] = {
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    { -2,  5, -2,  3, -2,  1, -2, -1, -2, -3, -2, -5 },
    { -5, -2, -3, -2, -1, -2,  1, -2,  3, -2, 5, -2 },
    { -2,  5, -2,  3, -2, -2, -2, -2,  3, -2, 5, -2 },
    { -5, -2, -3, -2, -1, -2, -2, -1, -2, -3, -2, -5 },
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    { -2,  5, -2,  3, -2,  1, -2, -1, -2, -3, -2, -5 },
    { -5, -2, -3, -2, -1, -2,  1, -2,  3, -2, 5, -2 },
    { -2,  5, -2,  3, -2, -2, -2, -2,  3, -2, 5, -2 },
    { -5, -2, -3, -2, -1, -2, -2, -1, -2, -3, -2, -5 },
};

// Global Variables Referenced by Party.cpp & Hireables.cpp
static D2PartyScreenNodeStrc* g_pPartyNodesHead = nullptr; // 0x6FBCC198
static D2CellFileStrc* g_pPartyBoxesCel = nullptr;         // 0x6FBCC19C
static D2CellFileStrc* g_pPartyPanelCel = nullptr;         // 0x6FBCC1A0
static D2CellFileStrc* g_pPartyCharsCel = nullptr;         // 0x6FBCC1A4
static D2CellFileStrc* g_pPartyBtn2Cel  = nullptr;         // 0x6FBCC1AC
static int32_t g_bPartyNeedRefresh      = 0;               // 0x6FBCC1B0
static int32_t g_bPartyCloseBtnPressed  = 0;               // 0x6FBCC1B4
static int32_t g_bPartyLeaveBtnPressed  = 0;               // 0x6FBCC1B8
static int32_t g_bPartyRowBtnPressed    = 0;               // 0x6FBCC1BC
static int32_t g_nPartyLastPlayerCount  = 0;               // 0x6FBA7908
#define g_pRosterHead gpRosterUnitList
#define g_pPetInfoList (reinterpret_cast<D2PetInfoStrc*>(gpRosterPetList))
static int32_t g_nPortraitDisplayMode   = 0;               // 0x6FBCB98C
static uint8_t g_nLifeBarColorGreen     = 0;               // 0x6FBCB990
static uint8_t g_nLifeBarColorYellow    = 0;               // 0x6FBCB991
static uint8_t g_nLifeBarColorRed       = 0;               // 0x6FBCB992
static uint8_t g_bLifeBarColorsInit     = 0;               // 0x6FBCB993
static int32_t g_nMercPortraitX         = 0;               // 0x6FBCB994
static int32_t g_nMercPortraitLabelY    = 0;               // 0x6FBCB998
static int32_t g_bMercPortraitHighlight = 0;               // 0x6FBCB99C
static int32_t g_bMercTooltipReady      = 0;               // 0x6FBCB9A0
static uint8_t g_bPopupHirelingPending  = 0;               // 0x6FBCB9A4
static uint32_t g_dwMercHoverStartTick  = 0;               // 0x6FBCB9A8
static D2CellFileStrc** g_ppPetTypeIcons= nullptr;         // 0x6FBACB4C
static D2CellFileStrc* g_apClassIcons[7]= { nullptr };     // 0x6FBACB50..0x6FBACB68
static D2CellFileStrc* g_pSummitCel     = nullptr;         // 0x6FBCB9D0
static D2CellFileStrc* g_pCloudCel      = nullptr;         // 0x6FBCB9D4
static uint8_t g_bCloudsInitialized     = 0;               // 0x6FBCB9DC
static int32_t g_bStarsInitialized      = 0;               // 0x6FBCB9D8
static int32_t g_bPartyNeedUiSync       = 0;
static int32_t g_nPartyClearedHover[4]  = { 0 };
static uint8_t g_aTileRenderPoolDesc[64]= { 0 };
static int32_t g_bDisableSummitBg       = 0;
static int32_t g_nSummitBaseY           = 0;
static int32_t g_aStarX[256]            = { 0 };
static int32_t g_aStarY[256]            = { 0 };
static int32_t g_aStarVel[256]          = { 0 };
static uint8_t g_aStarCol[256]          = { 0 };
static uint8_t g_aStarPal[8]            = { 0 };
static uint32_t g_dwStarLastTick        = 0;
static int32_t g_nSavedScaleFactor      = 0;
uint8_t g_nEnvLightB                    = 255;
uint8_t g_nEnvLightG                    = 255;
uint8_t g_nEnvLightIntensity            = 255;
uint8_t g_nEnvLightR                    = 255;

// Internal D2Client helpers
extern D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileDefault(const char* szPath, int32_t nFileType); // 0x6FB6EF70
extern D2CellFileStrc* __fastcall UI_LoadPanelCellFile(const char* szPath);                    // 0x6FB3C170
extern D2RosterUnitStrc* __fastcall ROSTER_FindByUnitGuid(int32_t nUnitGuid);                  // 0x6FB281C0
extern uint16_t __fastcall ROSTER_GetPartyId(D2RosterUnitStrc* pRoster);                       // 0x6FB27A10
extern void __stdcall MSG_SendLeavePartyPacket();                                              // 0x6FB29160
extern int32_t __fastcall MSG_SendPacket(void* pPacketData, size_t nSize);                     // 0x6FB291A0
extern const Unicode* __fastcall D2Client_GetNpcDisplayName(D2UnitStrc* pUnit);

// ============================================================================
// PART 1: Party Screen UI (..\Source\D2Client\UI\Party.cpp)
// Address Range: 0x6FB0FDD0..0x6FB12960 (36 functions in UI_Party.asm)
// ASM Block Headers in UI_Party.asm:
//   0x6FB0FDD0, 0x6FB0FDE0, 0x6FB0FE00, 0x6FB0FE90, 0x6FB0FEC0, 0x6FB0FF00,
//   0x6FB0FF60, 0x6FB0FFE0, 0x6FB100E0, 0x6FB10100, 0x6FB10120, 0x6FB10150,
//   0x6FB10230, 0x6FB10380, 0x6FB103E0, 0x6FB10430, 0x6FB10500, 0x6FB10690,
//   0x6FB106B0, 0x6FB107F0, 0x6FB108E0, 0x6FB10900, 0x6FB10B60, 0x6FB10C10,
//   0x6FB10F30, 0x6FB11060, 0x6FB110B0, 0x6FB11150, 0x6FB11220, 0x6FB11420,
//   0x6FB117F0, 0x6FB11A40, 0x6FB121D0, 0x6FB122B0, 0x6FB12390, 0x6FB12400
// ============================================================================

// 1.12a: 0x6FB0FDD0
// Calling convention: __stdcall
void __stdcall PARTY_UpdatePartyScreen()
{
    int32_t bPartyOpen = gaPanelStates[D2PANEL_PARTY];
    if (bPartyOpen)
    {
        g_bPartyNeedRefresh = 1;
        g_bPartyNeedUiSync = 1;
    }
}

// 1.12a: 0x6FB0FDF0
// Calling convention: __fastcall (eax = nClassId)
Unicode* __fastcall PARTY_GetClassString(int32_t nClassId)
{
    switch (nClassId)
    {
    case 0: return D2LANG_GetStringFromTblIndex(0x0FA7); // Amazon
    case 1: return D2LANG_GetStringFromTblIndex(0x0FA6); // Sorceress
    case 2: return D2LANG_GetStringFromTblIndex(0x0FA5); // Necromancer
    case 3: return D2LANG_GetStringFromTblIndex(0x0FA4); // Paladin
    case 4: return D2LANG_GetStringFromTblIndex(0x0FA3); // Barbarian
    case 5: return D2LANG_GetStringFromTblIndex(0x5212); // Druid
    case 6: return D2LANG_GetStringFromTblIndex(0x5213); // Assassin
    default:
        FOG_DisplayHalt(FOG_GetErrorLocation(188), "..\\Source\\D2Client\\UI\\Party.cpp", 188);
        exit(-1);
        return nullptr;
    }
}

// 1.12a: 0x6FB0FE90
// Calling convention: __stdcall
void __stdcall PARTY_LoadCellFiles()
{
    char szBtnPath[28];
    strcpy(szBtnPath, "UI\\FrontEnd\\Button2");
    g_pPartyBtn2Cel = UI_LoadPanelCellFile(szBtnPath);
    g_pPartyPanelCel = UI_LoadPanelCellFile("panel\\party");
    g_pPartyBoxesCel = UI_LoadPanelCellFile("panel\\pboxes");
    g_pPartyCharsCel = UI_LoadPanelCellFile("panel\\pchars");
}

// 1.12a: 0x6FB0FF50
// Calling convention: __stdcall
void __stdcall PARTY_FreeNodesAndCellFiles()
{
    D2PartyScreenNodeStrc* pNode = g_pPartyNodesHead;
    while (pNode)
    {
        D2PartyScreenNodeStrc* pNext = pNode->pNext;
        FOG_FreePool(nullptr, pNode, "..\\Source\\D2Client\\UI\\Party.cpp", 218, 0);
        pNode = pNext;
    }
    g_pPartyNodesHead = nullptr;

    D2CellFileStrc** ppCells[4] = {
        &g_pPartyBoxesCel, &g_pPartyPanelCel, &g_pPartyCharsCel, &g_pPartyBtn2Cel
    };
    for (int i = 0; i < 4; ++i)
    {
        D2CellFileStrc* pCel = *ppCells[i];
        if (pCel)
        {
            D2CMP_CelFileFreeHardware(pCel);
            FOG_Free(pCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
            *ppCells[i] = nullptr;
        }
    }
}

// 1.12a: 0x6FB10090
// Calling convention: __stdcall
void __stdcall PARTY_SetNeedRefreshFlag()
{
    g_bPartyNeedRefresh = 1;
}

// 1.12a: 0x6FB100A0
// Calling convention: __stdcall
int32_t __stdcall PARTY_CanPartyOrHostileInLevel()
{
    D2UnitStrc* pLocalPlayer = gpPlayerUnit;
    if (!pLocalPlayer)
    {
        FOG_DisplayHalt(FOG_GetErrorLocation(289), "..\\Source\\D2Client\\UI\\Party.cpp", 289);
        exit(-1);
    }
    D2RoomStrc* pRoom = UNITS_GetRoom(pLocalPlayer);
    if (!pRoom)
    {
        return 0;
    }
    return DUNGEON_GetLevelIdFromRoom(pRoom) != 0 ? 1 : 0;
}

// 1.12a: 0x6FB100F0
// Calling convention: __stdcall
void __stdcall PARTY_ClearCursorHover()
{
    if (gaPanelStates[D2PANEL_PARTY])
    {
        g_nPartyClearedHover[0] = 0;
        g_nPartyClearedHover[1] = 0;
        g_nPartyClearedHover[2] = 0;
        g_nPartyClearedHover[3] = 0;
    }
}

// 1.12a: 0x6FB10140
// Calling convention: __fastcall (eax = nUnitGuid, ecx = nFlagMask)
int32_t __fastcall PARTY_CheckRosterFlag(int32_t nUnitGuid, int32_t nFlagMask)
{
    D2RosterUnitStrc* pLocalRoster = g_pRosterHead;
    if (!pLocalRoster)
    {
        FOG_DisplayHalt(FOG_GetErrorLocation(324), "..\\Source\\D2Client\\UI\\Party.cpp", 324);
        exit(-1);
    }
    if (pLocalRoster->ppRelationList)
    {
        for (D2RosterRelationStrc* pRel = *pLocalRoster->ppRelationList; pRel; pRel = pRel->pNext)
        {
            if (pRel->nUnitGuid == nUnitGuid)
            {
                return (pRel->nRelationFlags & nFlagMask) != 0 ? 1 : 0;
            }
        }
    }
    return 0;
}

// 1.12a: 0x6FB101A0
// Calling convention: __stdcall
int32_t __stdcall PARTY_CountPlayersInParty(uint16_t wPartyId)
{
    int32_t nCount = 0;
    for (D2RosterUnitStrc* pCur = g_pRosterHead; pCur; pCur = pCur->pNext)
    {
        if (pCur->wPartyId == wPartyId)
        {
            ++nCount;
        }
    }
    return nCount;
}

// 1.12a: 0x6FB101D0
// Calling convention: __fastcall (ecx = nUnitGuid)
int32_t __fastcall PARTY_GetPartyLeaderGuid(int32_t nUnitGuid)
{
    for (D2RosterUnitStrc* pCur = g_pRosterHead; pCur; pCur = pCur->pNext)
    {
        if (pCur->dwUnitId == nUnitGuid)
        {
            uint16_t wPartyId = pCur->wPartyId;
            if (wPartyId == 0xFFFF)
            {
                return -1;
            }
            for (D2RosterUnitStrc* pMember = g_pRosterHead; pMember; pMember = pMember->pNext)
            {
                if (pMember->wPartyId == wPartyId && pMember->ppRelationList)
                {
                    for (D2RosterRelationStrc* pRel = *pMember->ppRelationList; pRel; pRel = pRel->pNext)
                    {
                        if (pRel->nUnitGuid == pMember->dwUnitId && (pRel->nRelationFlags & 8))
                        {
                            return pMember->dwUnitId;
                        }
                    }
                }
            }
            return -1;
        }
    }
    return -1;
}

// 1.12a: 0x6FB10250
// Calling convention: __fastcall (eax = pRosterUnit)
int32_t __fastcall PARTY_GetPlayerRelationshipState(D2RosterUnitStrc* pRosterUnit)
{
    D2RosterUnitStrc* pLocal = g_pRosterHead;
    if (!pLocal || !pRosterUnit)
    {
        return 0;
    }
    uint16_t wLocalParty = ROSTER_GetPartyId(pLocal);
    uint16_t wOtherParty = ROSTER_GetPartyId(pRosterUnit);
    if (wLocalParty != 0xFFFF && wLocalParty == wOtherParty)
    {
        return 1; // In same party
    }
    if (PARTY_CheckRosterFlag(pRosterUnit->dwUnitId, 8))
    {
        return 2; // Hostile
    }
    if (PARTY_CheckRosterFlag(pRosterUnit->dwUnitId, 2))
    {
        return 4; // Invited us
    }
    if (PARTY_CheckRosterFlag(pRosterUnit->dwUnitId, 1))
    {
        return 5; // We invited them
    }
    return (wOtherParty == 0xFFFF) ? 6 : 7;
}

// 1.12a: 0x6FB103E0
// Calling convention: __userpurge (eax = nX, edx = nY, [esp+4] = nWidth, [esp+8] = nHeight)
int32_t __stdcall PARTY_IsMouseInRect(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight)
{
    int32_t nMouseX = gnMouseX;
    int32_t nMouseY = gnMouseY;
    int32_t nLeft = nX + g_nPanelOffsetX;
    if (nMouseX >= nLeft && nMouseX <= nLeft + nWidth)
    {
        int32_t nBottom = nY - g_nPanelOffsetY;
        if (nMouseY >= nBottom - nHeight && nMouseY <= nBottom)
        {
            return 1;
        }
    }
    return 0;
}

// 1.12a: 0x6FB10440
// Calling convention: __stdcall
int32_t __stdcall PARTY_IsButtonEnabled(int32_t nButtonState)
{
    return (nButtonState != 0 && nButtonState != 8 && nButtonState != 9) ? 1 : 0;
}

// 1.12a: 0x6FB10460
// Calling convention: __fastcall (eax = pNode, ecx = pRosterUnit)
void __fastcall PARTY_UpdateMainPartyButton(D2PartyScreenNodeStrc* pNode, D2RosterUnitStrc* pRosterUnit)
{
    int32_t nRel = PARTY_GetPlayerRelationshipState(pRosterUnit);
    pNode->tMainBtn.nButtonType = 1;
    pNode->tMainBtn.pCellFile = g_pPartyBtn2Cel;
    pNode->tMainBtn.nX = 0x9F;
    pNode->tMainBtn.nY = pNode->nRowY + 0x16;
    pNode->tMainBtn.nState = nRel;
    switch (nRel)
    {
    case 4: // Accept invite
        pNode->tMainBtn.nFrame = 4;
        pNode->tMainBtn.nTextStrId = 0x1018;
        break;
    case 5: // Cancel invite
        pNode->tMainBtn.nFrame = 0;
        pNode->tMainBtn.nTextStrId = 0x1017;
        break;
    case 6: // Invite to party
        pNode->tMainBtn.nFrame = 0;
        pNode->tMainBtn.nTextStrId = 0x1016;
        break;
    default:
        pNode->tMainBtn.nFrame = 1;
        pNode->tMainBtn.nTextStrId = 0;
        break;
    }
}

// 1.12a: 0x6FB105B0
// Calling convention: __fastcall (esi = pNode, edi = pRosterUnit)
void __fastcall PARTY_UpdateHostileButton(D2PartyScreenNodeStrc* pNode, D2RosterUnitStrc* pRosterUnit)
{
    int32_t nRel = PARTY_GetPlayerRelationshipState(pRosterUnit);
    pNode->tHostileBtn.nButtonType = 0;
    pNode->tHostileBtn.pCellFile = g_pPartyBoxesCel;
    pNode->tHostileBtn.nX = 0x20;
    pNode->tHostileBtn.nY = pNode->nRowY + 0x16;
    if (!g_pRosterHead || g_pRosterHead->wLevel < 9 || pRosterUnit->wLevel < 9)
    {
        pNode->tHostileBtn.nState = 0;
        pNode->tHostileBtn.nFrame = 5;
        pNode->tHostileBtn.nTextStrId = 0x101E;
        return;
    }
    if (nRel == 2)
    {
        pNode->tHostileBtn.nState = 2;
        pNode->tHostileBtn.nFrame = 0;
        pNode->tHostileBtn.nTextStrId = 0x101C;
    }
    else
    {
        pNode->tHostileBtn.nState = 1;
        pNode->tHostileBtn.nFrame = 4;
        pNode->tHostileBtn.nTextStrId = 0x101D;
    }
}

// 1.12a: 0x6FB106C0
// Calling convention: __stdcall
void __stdcall PARTY_DrawWrappedText(const Unicode* wszText, int32_t nX, int32_t nY, int32_t nMaxWidth)
{
    D2Win_10127_SetFont(6);
    int32_t nTextWidth = D2Win_10121_GetTextWidth(wszText);
    if (nTextWidth <= nMaxWidth)
    {
        D2Win_10117_DrawText(wszText, nX + (nMaxWidth - nTextWidth) / 2, nY, 0, 0);
    }
    else
    {
        D2Win_10117_DrawText(wszText, nX, nY, 0, 0);
    }
}

// 1.12a: 0x6FB10840
// Calling convention: __fastcall (eax = pRosterUnit, esi = nRowY)
void __fastcall PARTY_DrawPlayerNameAndGuild(D2RosterUnitStrc* pRosterUnit, int32_t nRowY)
{
    Unicode wszName[100] = { 0 };
    Unicode_toUnicode(wszName, pRosterUnit->szNameEx, 25);
    D2Win_10127_SetFont(1);
    D2Win_10117_DrawText(
        wszName,
        0x50 + g_nPanelOffsetX,
        nRowY + 0x10 - g_nPanelOffsetY,
        0,
        0);
}

// 1.12a: 0x6FB108E0
// Calling convention: __fastcall (ecx = pNode, edx = nColor)
void __fastcall PARTY_DrawNodeLevelAndClass(D2PartyScreenNodeStrc* pNode, int32_t nColor)
{
    Unicode wszBuf[128] = { 0 };
    D2Win_10127_SetFont(6);
    Unicode* wszFmt = D2LANG_GetStringFromTblIndex(0x1028); // "Level %d %s"
    Unicode* wszClass = PARTY_GetClassString(pNode->nClassId);
    Unicode_sprintf(128, wszBuf, wszFmt, pNode->nLevel, wszClass);
    D2Win_10117_DrawText(
        wszBuf,
        0x50 + g_nPanelOffsetX,
        pNode->nRowY + 0x1D - g_nPanelOffsetY,
        nColor,
        0);
}

// 1.12a: 0x6FB109E0
// Calling convention: __fastcall (esi = pButton)
void __fastcall PARTY_DrawButton(D2PartyButtonStrc* pButton)
{
    if (!pButton->pCellFile)
    {
        return;
    }
    D2GfxDataStrc tGfxData = { 0 };
    tGfxData.pCellFile = pButton->pCellFile;
    tGfxData.nFrame = pButton->nFrame;
    TEXTURE_CelDraw(
        &tGfxData,
        pButton->nX + g_nPanelOffsetX,
        pButton->nY - g_nPanelOffsetY,
        0xFFFFFFFF,
        5,
        nullptr);
    if (pButton->nButtonType == 1 && pButton->nTextStrId != 0)
    {
        PARTY_DrawWrappedText(
            D2LANG_GetStringFromTblIndex((uint16_t)pButton->nTextStrId),
            pButton->nX + g_nPanelOffsetX,
            pButton->nY - g_nPanelOffsetY - 5,
            53);
    }
}

// 1.12a: 0x6FB10AB0
// Calling convention: __fastcall (esi = pButton)
void __fastcall PARTY_DrawButtonTooltip(D2PartyButtonStrc* pButton)
{
    if (pButton->nButtonType != 0 || pButton->nTextStrId == 0)
    {
        return;
    }
    if (PARTY_IsMouseInRect(pButton->nX, pButton->nY, 20, 20))
    {
        D2Win_10127_SetFont(6);
        Unicode* wszTip = D2LANG_GetStringFromTblIndex((uint16_t)pButton->nTextStrId);
        D2Win_10129_DrawFramedText(
            wszTip,
            pButton->nX + g_nPanelOffsetX + 10,
            pButton->nY - g_nPanelOffsetY - 22,
            0,
            1);
    }
}

// 1.12a: 0x6FB10C10
// Calling convention: __stdcall
void __stdcall PARTY_UpdateNodeButtons()
{
    D2RosterUnitStrc* pRoster = g_pRosterHead ? g_pRosterHead->pNext : nullptr;
    D2PartyScreenNodeStrc* pNode = g_pPartyNodesHead;
    while (pRoster && pNode)
    {
        PARTY_UpdateMainPartyButton(pNode, pRoster);
        PARTY_UpdateHostileButton(pNode, pRoster);

        // Corpse Loot button (+0x40)
        pNode->tLootBtn.nButtonType = 0;
        pNode->tLootBtn.nX = 0x35;
        pNode->tLootBtn.nY = pNode->nRowY + 0x16;
        pNode->tLootBtn.pCellFile = g_pPartyBoxesCel;
        if (PARTY_CheckRosterFlag(pRoster->dwUnitId, 4))
        {
            pNode->tLootBtn.nState = 2;
            pNode->tLootBtn.nFrame = 12;
            pNode->tLootBtn.nTextStrId = 0x101A;
        }
        else
        {
            pNode->tLootBtn.nState = 1;
            pNode->tLootBtn.nFrame = 16;
            pNode->tLootBtn.nTextStrId = 0x101B;
        }

        // Mute / Squelch button (+0x60)
        pNode->tMuteBtn.nButtonType = 0;
        pNode->tMuteBtn.nX = 0x127;
        pNode->tMuteBtn.nY = pNode->nRowY + 0x16;
        pNode->tMuteBtn.pCellFile = g_pPartyBoxesCel;
        if (PARTY_CheckRosterFlag(pRoster->dwUnitId, 0x10))
        {
            pNode->tMuteBtn.nState = 4;
            pNode->tMuteBtn.nFrame = 20;
            pNode->tMuteBtn.nTextStrId = 0x1019;
        }
        else
        {
            pNode->tMuteBtn.nState = 3;
            pNode->tMuteBtn.nFrame = 24;
            pNode->tMuteBtn.nTextStrId = 0x1031;
        }

        // Ignore Party Invites button (+0x80)
        pNode->tIgnoreBtn.nButtonType = 0;
        pNode->tIgnoreBtn.nX = 0x13C;
        pNode->tIgnoreBtn.nY = pNode->nRowY + 0x16;
        pNode->tIgnoreBtn.pCellFile = g_pPartyBoxesCel;
        if (PARTY_CheckRosterFlag(pRoster->dwUnitId, 0x0200))
        {
            pNode->tIgnoreBtn.nState = 2;
            pNode->tIgnoreBtn.nFrame = 28;
            pNode->tIgnoreBtn.nTextStrId = 0x1034;
        }
        else
        {
            pNode->tIgnoreBtn.nState = 1;
            pNode->tIgnoreBtn.nFrame = 32;
            pNode->tIgnoreBtn.nTextStrId = 0x1035;
        }

        pRoster = pRoster->pNext;
        pNode = pNode->pNext;
    }
}

// 1.12a: 0x6FB10E90
// Calling convention: __stdcall
void __stdcall PARTY_RebuildPartyNodeList()
{
    D2PartyScreenNodeStrc* pCur = g_pPartyNodesHead;
    while (pCur)
    {
        D2PartyScreenNodeStrc* pNext = pCur->pNext;
        FOG_FreePool(nullptr, pCur, "..\\Source\\D2Client\\UI\\Party.cpp", 772, 0);
        pCur = pNext;
    }
    g_pPartyNodesHead = nullptr;

    if (!g_pRosterHead)
    {
        return;
    }

    D2PartyScreenNodeStrc* pTail = nullptr;
    int32_t nRowY = 0x57;
    for (D2RosterUnitStrc* pRoster = g_pRosterHead->pNext; pRoster; pRoster = pRoster->pNext)
    {
        D2PartyScreenNodeStrc* pNew = (D2PartyScreenNodeStrc*)FOG_AllocPool(
            nullptr, sizeof(D2PartyScreenNodeStrc), "..\\Source\\D2Client\\UI\\Party.cpp", 785, 0);
        memset(pNew, 0, sizeof(D2PartyScreenNodeStrc));
        strncpy(pNew->szName, pRoster->szNameEx, 24);
        pNew->nClassId = pRoster->dwClassId;
        pNew->nLevel = pRoster->wLevel;
        pNew->nUnitGuid = pRoster->dwUnitId;
        pNew->nPartyLeaderGuid = PARTY_GetPartyLeaderGuid(pRoster->dwUnitId);
        pNew->nRowY = nRowY;
        nRowY += 0x2B;

        if (!g_pPartyNodesHead)
        {
            g_pPartyNodesHead = pNew;
        }
        else
        {
            pTail->pNext = pNew;
        }
        pTail = pNew;
    }
    PARTY_UpdateNodeButtons();
}

int32_t __stdcall PARTY_CountTotalRosterPlayers();

// 1.12a: 0x6FB110C0
// Calling convention: __stdcall
void __stdcall PARTY_SyncPartyNodePositions()
{
    int32_t nTotal = PARTY_CountTotalRosterPlayers();
    if (nTotal != g_nPartyLastPlayerCount || g_bPartyNeedRefresh)
    {
        g_nPartyLastPlayerCount = nTotal;
        g_bPartyNeedRefresh = 0;
        PARTY_RebuildPartyNodeList();
    }
}

// 1.12a: 0x6FB11240
// Calling convention: __fastcall (eax = pRosterUnit, ecx = pNode)
void __fastcall PARTY_DrawRelationshipStatusText(D2RosterUnitStrc* pRosterUnit, D2PartyScreenNodeStrc* pNode)
{
    int32_t nRel = PARTY_GetPlayerRelationshipState(pRosterUnit);
    D2Win_10127_SetFont(6);
    uint16_t wStrId = 0;
    int32_t nColor = 0;
    if (nRel == 2)
    {
        wStrId = 0x102A; // "Hostile"
        nColor = 1;
    }
    else if (nRel == 4)
    {
        wStrId = 0x102C; // "Invited You"
        nColor = 2;
    }
    else if (nRel == 5)
    {
        wStrId = 0x102B; // "Invited"
        nColor = 2;
    }
    if (wStrId != 0)
    {
        D2Win_10117_DrawText(
            D2LANG_GetStringFromTblIndex(wStrId),
            0xDC + g_nPanelOffsetX,
            pNode->nRowY + 0x14 - g_nPanelOffsetY,
            nColor,
            0);
    }
}

// 1.12a: 0x6FB11430
// Calling convention: __stdcall
void __stdcall PARTY_DrawPartyScreen()
{
    if (!gaPanelStates[D2PANEL_PARTY])
    {
        return;
    }
    if (!g_pPartyPanelCel)
    {
        PARTY_LoadCellFiles();
    }
    PARTY_SyncPartyNodePositions();

    int32_t nBaseX = g_nPanelOffsetX;
    int32_t nBaseY = g_nPanelOffsetY;

    if (g_pPartyPanelCel)
    {
        D2GfxDataStrc tGfxData = { 0 };
        tGfxData.pCellFile = g_pPartyPanelCel;

        // 4 quadrant tiles of panel\party
        tGfxData.nFrame = 0;
        TEXTURE_CelDraw(&tGfxData, nBaseX + 0x50, 0x100 - nBaseY, 0xFFFFFFFF, 5, nullptr);
        tGfxData.nFrame = 1;
        TEXTURE_CelDraw(&tGfxData, nBaseX + 0x100, 0x100 - nBaseY, 0xFFFFFFFF, 5, nullptr);
        tGfxData.nFrame = 2;
        TEXTURE_CelDraw(&tGfxData, nBaseX + 0x50, 0x1DF - nBaseY, 0xFFFFFFFF, 5, nullptr);
        tGfxData.nFrame = 3;
        TEXTURE_CelDraw(&tGfxData, nBaseX + 0x100, 0x1DF - nBaseY, 0xFFFFFFFF, 5, nullptr);
    }

    // Draw each party screen player row
    D2RosterUnitStrc* pRoster = g_pRosterHead ? g_pRosterHead->pNext : nullptr;
    for (D2PartyScreenNodeStrc* pNode = g_pPartyNodesHead; pNode && pRoster; pNode = pNode->pNext, pRoster = pRoster->pNext)
    {
        PARTY_DrawPlayerNameAndGuild(pRoster, pNode->nRowY);
        PARTY_DrawNodeLevelAndClass(pNode, 0);
        PARTY_DrawRelationshipStatusText(pRoster, pNode);
        PARTY_DrawButton(&pNode->tMainBtn);
        PARTY_DrawButton(&pNode->tHostileBtn);
        PARTY_DrawButton(&pNode->tLootBtn);
        PARTY_DrawButton(&pNode->tMuteBtn);
        PARTY_DrawButton(&pNode->tIgnoreBtn);
    }

    // Second pass for button hover tooltips
    for (D2PartyScreenNodeStrc* pNode = g_pPartyNodesHead; pNode; pNode = pNode->pNext)
    {
        PARTY_DrawButtonTooltip(&pNode->tHostileBtn);
        PARTY_DrawButtonTooltip(&pNode->tLootBtn);
        PARTY_DrawButtonTooltip(&pNode->tMuteBtn);
        PARTY_DrawButtonTooltip(&pNode->tIgnoreBtn);
    }
}

// 1.12a: 0x6FB11FC0
// Calling convention: __stdcall
void __stdcall PARTY_SendPartyActionPacket(uint8_t nAction, int32_t nTargetUnitGuid)
{
    #pragma pack(push, 1)
    struct D2Packet5EStrc {
        uint8_t nHeader;
        uint8_t nAction;
        int32_t nTargetGuid;
    } tPacket = { 0x5E, nAction, nTargetUnitGuid };
    #pragma pack(pop)
    MSG_SendPacket(&tPacket, sizeof(tPacket));
}

// 1.12a: 0x6FB11FE0
// Calling convention: __stdcall
void __stdcall PARTY_SendRelationActionPacket(uint8_t nAction, uint8_t bState, int32_t nTargetUnitGuid)
{
    #pragma pack(push, 1)
    struct D2Packet5DStrc {
        uint8_t nHeader;
        uint8_t nAction;
        uint8_t bState;
        int32_t nTargetGuid;
    } tPacket = { 0x5D, nAction, bState, nTargetUnitGuid };
    #pragma pack(pop)
    MSG_SendPacket(&tPacket, sizeof(tPacket));
}

// 1.12a: 0x6FB12010
// Calling convention: __fastcall (ecx = nSoundId)
void __fastcall PARTY_PlayUISound(int32_t nSoundId)
{
    // Calls SOUND_PlayUISound (0x6FAE1FA0) with ebx = nSoundId.
}

// 1.12a: 0x6FB12020
// Calling convention: __stdcall
void __stdcall PARTY_ClosePartyScreen()
{
    gaPanelStates[D2PANEL_PARTY] = 0;
    PARTY_FreeNodesAndCellFiles();
}

// 1.12a: 0x6FB12190
// Calling convention: __fastcall (eax = pNode, ecx = pRosterUnit)
void __fastcall PARTY_OnLootButtonClick(D2PartyScreenNodeStrc* pNode, D2RosterUnitStrc* pRosterUnit)
{
    if (pNode->tLootBtn.nState == 1)
    {
        PARTY_SendRelationActionPacket(2, 1, pRosterUnit->dwUnitId);
        pNode->tLootBtn.nState = 2;
        pNode->tLootBtn.nFrame = 12;
        pNode->tLootBtn.nTextStrId = 0x101A;
    }
    else if (pNode->tLootBtn.nState == 2)
    {
        PARTY_SendRelationActionPacket(2, 0, pRosterUnit->dwUnitId);
        pNode->tLootBtn.nState = 1;
        pNode->tLootBtn.nFrame = 16;
        pNode->tLootBtn.nTextStrId = 0x101B;
    }
}

// 1.12a: 0x6FB12230
// Calling convention: __fastcall (eax = pNode, ecx = pRosterUnit)
void __fastcall PARTY_OnMuteButtonClick(D2PartyScreenNodeStrc* pNode, D2RosterUnitStrc* pRosterUnit)
{
    if (pNode->tMuteBtn.nState == 3)
    {
        PARTY_SendRelationActionPacket(3, 1, pRosterUnit->dwUnitId);
        pNode->tMuteBtn.nState = 4;
        pNode->tMuteBtn.nFrame = 20;
        pNode->tMuteBtn.nTextStrId = 0x1019;
    }
    else if (pNode->tMuteBtn.nState == 4)
    {
        PARTY_SendRelationActionPacket(3, 0, pRosterUnit->dwUnitId);
        pNode->tMuteBtn.nState = 3;
        pNode->tMuteBtn.nFrame = 24;
        pNode->tMuteBtn.nTextStrId = 0x1031;
    }
}

// 1.12a: 0x6FB122C0
// Calling convention: __fastcall (eax = pNode, ecx = pRosterUnit)
void __fastcall PARTY_OnIgnoreButtonClick(D2PartyScreenNodeStrc* pNode, D2RosterUnitStrc* pRosterUnit)
{
    if (pNode->tIgnoreBtn.nState == 1)
    {
        PARTY_SendRelationActionPacket(1, 1, pRosterUnit->dwUnitId);
        pNode->tIgnoreBtn.nState = 2;
        pNode->tIgnoreBtn.nFrame = 28;
        pNode->tIgnoreBtn.nTextStrId = 0x1034;
    }
    else if (pNode->tIgnoreBtn.nState == 2)
    {
        PARTY_SendRelationActionPacket(1, 0, pRosterUnit->dwUnitId);
        pNode->tIgnoreBtn.nState = 1;
        pNode->tIgnoreBtn.nFrame = 32;
        pNode->tIgnoreBtn.nTextStrId = 0x1035;
    }
}

// 1.12a: 0x6FB12350
// Calling convention: __fastcall (ecx = nUnitGuid)
D2RosterUnitStrc* __fastcall PARTY_FindRosterUnitByGuid(int32_t nUnitGuid)
{
    for (D2RosterUnitStrc* pCur = g_pRosterHead; pCur; pCur = pCur->pNext)
    {
        if (pCur->dwUnitId == nUnitGuid)
        {
            return pCur;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FB12370
// Calling convention: __stdcall
int32_t __stdcall PARTY_CountTotalRosterPlayers()
{
    int32_t nCount = 0;
    for (D2RosterUnitStrc* pCur = g_pRosterHead; pCur; pCur = pCur->pNext)
    {
        ++nCount;
    }
    return nCount;
}

// 1.12a: 0x6FB12390
// Calling convention: __fastcall (eax = nBtnIdx, ecx = pNode, edx = pRosterUnit)
int32_t __fastcall PARTY_HandleMouseDownOnButton(int32_t nBtnIdx, D2PartyScreenNodeStrc* pNode, D2RosterUnitStrc* pRosterUnit)
{
    D2PartyButtonStrc* pButtons = &pNode->tMainBtn;
    D2PartyButtonStrc* pBtn = &pButtons[nBtnIdx];
    if (!PARTY_IsButtonEnabled(pBtn->nState))
    {
        return 0;
    }
    pBtn->bPressed = 1;
    pBtn->nFrame += 1;
    PARTY_PlayUISound(4);
    g_bPartyRowBtnPressed = 1;
    return 1;
}

// 1.12a: 0x6FB12400
// Calling convention: __fastcall (eax = nButtonState, ecx = pRosterUnit)
int32_t __fastcall PARTY_ExecuteButtonAction(int32_t nButtonState, D2RosterUnitStrc* pRosterUnit)
{
    switch (nButtonState)
    {
    case 4: // Accept party invite
        PARTY_SendPartyActionPacket(8, pRosterUnit->dwUnitId);
        return 1;
    case 5: // Cancel party invite
        PARTY_SendPartyActionPacket(7, pRosterUnit->dwUnitId);
        return 1;
    case 6: // Invite to party
    case 7:
        PARTY_SendPartyActionPacket(6, pRosterUnit->dwUnitId);
        return 1;
    default:
        return 0;
    }
}

// 1.12a: 0x6FB12530
// Calling convention: __stdcall
void __stdcall PARTY_HandleMouseUp(D2WindowMsgStrc* pMsg)
{
    if (g_bPartyCloseBtnPressed)
    {
        g_bPartyCloseBtnPressed = 0;
        if (PARTY_IsMouseInRect(0x11B, 0x1CC, 0x20, 0x20))
        {
            PARTY_ClosePartyScreen();
            pMsg->bHandled = 1;
            pMsg->nReturnValue = 0;
            SMsgBreakHandlerChain(pMsg);
            return;
        }
    }
    if (g_bPartyLeaveBtnPressed)
    {
        g_bPartyLeaveBtnPressed = 0;
        if (PARTY_IsMouseInRect(0xE3, 0x35, 0x35, 0x14))
        {
            if (g_pRosterHead && ROSTER_GetPartyId(g_pRosterHead) != 0xFFFF)
            {
                MSG_SendLeavePartyPacket();
                pMsg->bHandled = 1;
                pMsg->nReturnValue = 0;
                SMsgBreakHandlerChain(pMsg);
                return;
            }
        }
    }
    g_bPartyRowBtnPressed = 0;
}

// ============================================================================
// PART 2: Environment / Viewport Tile & Unit Drawing & UI Spinner & Hireables
// (..\Source\D2Client\UI\Hireables.cpp, 0x6FB6AA00..0x6FB6EEB0, 83 functions)
// ============================================================================

// 1.12a: 0x6FB6AA00
// Calling convention: __stdcall
void __stdcall ENV_FreeSummitCloudCellFiles()
{
    if (g_pSummitCel)
    {
        D2CMP_CelFileFreeHardware(g_pSummitCel);
        FOG_Free(g_pSummitCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
        g_pSummitCel = nullptr;
    }
    if (g_pCloudCel)
    {
        D2CMP_CelFileFreeHardware(g_pCloudCel);
        FOG_Free(g_pCloudCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
        g_pCloudCel = nullptr;
    }
}

// 1.12a: 0x6FB6AA60
// Calling convention: __stdcall
void __stdcall ENV_AllocTileRenderPools()
{
    D2Win_10178((void*)g_aTileRenderPoolDesc);
}

// 1.12a: 0x6FB6AB20
// Calling convention: __fastcall (ecx = pPlayerUnit)
void __fastcall ENV_DrawArreatSummitBackground(D2UnitStrc* pPlayerUnit)
{
    if (g_bDisableSummitBg != 0)
    {
        return;
    }
    if (!g_pSummitCel)
    {
        g_pSummitCel = ARCHIVE_LoadCellFileDefault("data\\global\\ui\\summit01", 0);
    }
    if (!g_pCloudCel)
    {
        g_pCloudCel = ARCHIVE_LoadCellFileDefault("data\\global\\ui\\cloud01", 0);
    }
    int32_t nPosY = UNITS_GetPrecisionY(pPlayerUnit) - g_nSummitBaseY;
    int32_t nScrollX = -((nPosY / 8) % 256);
    int32_t nBaseFrame = ((nPosY / 8) / 256) & 3;

    if (g_pSummitCel)
    {
        D2GfxDataStrc tGfx = { 0 };
        tGfx.pCellFile = g_pSummitCel;
        for (int row = 0; row < (D2GFX_GetResolutionMode() == 2 ? 3 : 2); ++row)
        {
            int32_t nY = (row + 1) * 0x100;
            for (int col = 0; col < 4; ++col)
            {
                tGfx.nFrame = ((nBaseFrame + col) & 3) + row * 4;
                TEXTURE_CelDraw(&tGfx, nScrollX + col * 0x100, nY, 0xFFFFFFFF, 5, nullptr);
            }
        }
    }
}

// 1.12a: 0x6FB6B0A0
// Calling convention: __usercall (eax = bRespawnAtEdge, edi = nStarIdx)
void __fastcall ENV_InitArcaneSanctuaryStar(int32_t bRespawnAtEdge, int32_t nStarIdx)
{
    int32_t* pStarX   = g_aStarX;
    int32_t* pStarY   = g_aStarY;
    int32_t* pStarVel = g_aStarVel;
    uint8_t* pStarCol = g_aStarCol;
    uint8_t* pStarPal = g_aStarPal;

    int32_t nScreenWidth  = gnViewportWidth;
    int32_t nScreenHeight = gnViewportHeight;
    pStarX[nStarIdx] = bRespawnAtEdge ? (nScreenWidth - 1) : (rand() % (nScreenWidth > 0 ? nScreenWidth : 1));
    pStarY[nStarIdx] = (nScreenHeight > 0) ? (rand() % nScreenHeight) : 0;
    pStarVel[nStarIdx] = -1 - (rand() % 5);
    pStarCol[nStarIdx] = pStarPal[rand() & 7];
}

// 1.12a: 0x6FB6B190
// Calling convention: __stdcall
void __stdcall ENV_FreeTileRenderPoolsCheck()
{
    ENV_FreeSummitCloudCellFiles();
}

// 1.12a: 0x6FB6B1D0
// Calling convention: __stdcall
void __stdcall ENV_FreeTileRenderPools()
{
    ENV_FreeSummitCloudCellFiles();
}

// 1.12a: 0x6FB6B250
// Calling convention: __fastcall (eax = nShakeAmplitude)
void __fastcall ENV_UpdateEarthquakeLoopSound(int32_t nShakeAmplitude)
{
    int32_t nVolume = (nShakeAmplitude * 255) / 20;
    if (nVolume < 0) nVolume = 0;
    if (nVolume > 255) nVolume = 255;
    // Updates looping sound 0x1C4 (earthquake rumble) at 0x6FBB0220.
}

// 1.12a: 0x6FB6B320
// Calling convention: __stdcall
void __stdcall ENV_DrawArcaneSanctuaryStars()
{
    int32_t* pStarX   = g_aStarX;
    int32_t* pStarY   = g_aStarY;
    int32_t* pStarVel = g_aStarVel;
    uint8_t* pStarCol = g_aStarCol;
    uint8_t* pStarPal = g_aStarPal;

    if (!g_bStarsInitialized)
    {
        for (int i = 0; i < 8; ++i)
        {
            pStarPal[i] = D2Win_10034_MixRGB(128 + i * 16, 128 + i * 16, 128 + i * 16);
        }
        for (int i = 0; i < 256; ++i)
        {
            ENV_InitArcaneSanctuaryStar(0, i);
        }
        g_bStarsInitialized = 1;
    }

    for (int i = 0; i < 256; ++i)
    {
        D2GFX_DrawLine(pStarX[i], pStarY[i], pStarX[i], pStarY[i], pStarCol[i], 0xFF);
    }

    uint32_t dwTick = GetTickCount();
    if (dwTick - g_dwStarLastTick > 40)
    {
        g_dwStarLastTick = dwTick;
        for (int i = 0; i < 256; ++i)
        {
            pStarX[i] += pStarVel[i];
            if (pStarX[i] < 0)
            {
                ENV_InitArcaneSanctuaryStar(1, i);
            }
        }
    }
}

// 1.12a: 0x6FB6B4E0
// Calling convention: epilog helper
void __stdcall ENV_DrawArcaneSanctuaryStars_Epilog()
{
}

// 1.12a: 0x6FB6B4F0
// Calling convention: __stdcall
void __stdcall ENV_ApplyScreenShake(int32_t* pOffsetX, int32_t* pOffsetY)
{
    (void)pOffsetX;
    (void)pOffsetY;
    uint32_t dwElapsed = GetTickCount() - g_dwShakeStartTick;
    uint32_t dwAttack  = (uint32_t)g_nShakeRampUpMs;
    uint32_t dwSustain = (uint32_t)g_nShakeHoldMs;
    uint32_t dwDecay   = (uint32_t)g_nShakeRampDownMs;
    if (dwElapsed > dwAttack + dwSustain + dwDecay)
    {
        ENV_UpdateEarthquakeLoopSound(0);
        g_nShakeMaxAmplitude = 0;
        g_nShakeOffsetX = 0;
        g_nShakeOffsetY = 0;
        g_bScreenShakeActive = 0;
        return;
    }
    int32_t nMaxAmp = g_nShakeMaxAmplitude;
    int32_t nAmp = nMaxAmp;
    if (dwElapsed < dwAttack && dwAttack > 0)
    {
        nAmp = (nMaxAmp * dwElapsed) / dwAttack;
    }
    else if (dwElapsed >= dwAttack + dwSustain && dwDecay > 0)
    {
        nAmp = (nMaxAmp * (dwAttack + dwSustain + dwDecay - dwElapsed)) / dwDecay;
    }
    ENV_UpdateEarthquakeLoopSound(nAmp);
}

// 1.12a: 0x6FB6B5E0
// Calling convention: __userpurge (esi = pViewport, [esp+4] = bClearBackground)
void __stdcall ENV_RenderViewportComplete(uint32_t* pViewport, int32_t bClearBackground)
{
    if (!pViewport)
    {
        FOG_DisplayHalt(FOG_GetErrorLocation(477), "..\\Source\\D2Client\\UI\\Hireables.cpp", 477);
        exit(-1);
    }
    pViewport[0] &= ~1u;
}

extern void __fastcall FLOOR_DrawViewportFloorTiles(
    D2RoomStrc* pCenterRoom,
    int32_t nViewportLeft,
    int32_t nViewportTop,
    int32_t nViewportRight,
    int32_t nViewportBottom);
extern void __fastcall FLOOR_DrawRoofTilesInCells(uint8_t* pViewport);
extern void __fastcall GFX_DrawUnit(D2UnitStrc* pUnit, uint32_t dwGamma, int nXPos, int nYPos, int bDrawHighlight, int nScreenMode);
extern void __fastcall GFX_DrawCompositeUnitShadow(D2UnitStrc* pUnit, int nXPos, int nYPos);
extern void __fastcall GFX_DrawSingleLayerUnitShadow(D2UnitStrc* pUnit, int nXPos, int nYPos);
extern D2UnitStrc* __fastcall SELECT_GetSelectedUnit(void);
extern int32_t gnOpenPanelMode;

void __stdcall ENV_UpdateActEnvironmentLighting();
void __stdcall ENV_DrawUnitInViewport(int32_t bForceDraw, D2UnitStrc* pUnit, uint32_t dwColor);
void __stdcall ENV_DrawWallTileList(void* pTileNode, int32_t bUpperWalls, void* pViewport);
void __stdcall ENV_DrawFloorTileList(void* pCellNode, void* pViewport);
void __fastcall ENV_DrawAllFloorTiles(uint8_t* pViewport);
void __stdcall ENV_DrawWallsAndUnitsInCells(uint8_t* pViewport);
void __fastcall ENV_DrawShadowTileNode(int32_t nScreenPanels, uint32_t* pShadowNode, uint32_t* pViewport);
void __fastcall ENV_DrawShadowsAndUpperWalls(uint8_t* pViewport);

// 1.12a: 0x6FB6B630
// Calling convention: __fastcall (eax = pViewport)
void __fastcall ENV_RenderGameViewport(uint32_t* pViewport)
{
    if (!pViewport || (!pViewport[3] && !pViewport[4]))
    {
        return;
    }
    SCALE_UpdateFactor();
    g_nSavedScaleFactor = SCALE_GetFactor();
    if (D2GFX_CheckPerspective())
    {
        D2UnitStrc* pPlayer = gpPlayerUnit;
        if (pPlayer)
        {
            int32_t nX = UNITS_GetPrecisionX(pPlayer);
            int32_t nY = UNITS_GetClientCoordX(pPlayer);
            SCALE_CheckPerspectivePosition(nX, nY);
        }
    }

    ENV_UpdateActEnvironmentLighting();

    D2UnitStrc* pPlayer = gpPlayerUnit;
    D2RoomStrc* pRoom = pPlayer ? UNITS_GetRoom(pPlayer) : nullptr;
    if (pRoom)
    {
        const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
        if (nLevelId == 74)
        {
            ENV_DrawArcaneSanctuaryStars();
        }
        if (nLevelId == 120)
        {
            ENV_DrawArreatSummitBackground(pPlayer);
        }
        else
        {
            ENV_FreeSummitCloudCellFiles();
        }
    }

    const uint32_t dwGridFlags = pViewport[14]; // +0x38 = tSortGrid.dwGridFlags

    // Pass 1: Cliff / river lower walls extending below floor level (orientations 16..19)
    if (dwGridFlags & 0x01u)
    {
        ENV_DrawAllFloorTiles(reinterpret_cast<uint8_t*>(pViewport));
    }

    // Pass 2: Isometric floor tiles across visible DRLG rooms
    FLOOR_DrawViewportFloorTiles(
        pRoom,
        static_cast<int32_t>(pViewport[5]),
        static_cast<int32_t>(pViewport[6]),
        static_cast<int32_t>(pViewport[7]),
        static_cast<int32_t>(pViewport[8]));

    // Pass 3: Floor overlays, corpses, terrain/unit shadows, and non-priority walls
    ENV_DrawShadowsAndUpperWalls(reinterpret_cast<uint8_t*>(pViewport));

    // Pass 4: Depth-sorted priority walls and live/preset world units
    ENV_DrawWallsAndUnitsInCells(reinterpret_cast<uint8_t*>(pViewport));

    // Pass 5: Overhead roof tiles (orientation 15) with cutaway fade
    if (dwGridFlags & 0x02u)
    {
        FLOOR_DrawRoofTilesInCells(reinterpret_cast<uint8_t*>(pViewport));
    }

    SCALE_SetDefaultFactor();
}

// 1.12a: 0x6FB6B800
// Calling convention: __fastcall (edi = pUnit)
void __fastcall ENV_DrawUnitDebugTargetCrosshair(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        FOG_DisplayHalt(FOG_GetErrorLocation(97), "..\\Source\\D2Client\\UI\\Hireables.cpp", 97);
        exit(-1);
    }
    int32_t nX = *(uint16_t*)((uint8_t*)pUnit + 0x8C);
    int32_t nY = *(uint16_t*)((uint8_t*)pUnit + 0x8E);
    DUNGEON_GameSubtileToClientCoords(&nY, &nX);
    int32_t nScreenX = nY + gnViewportShiftOffsetX - gnCameraScreenSkewX;
    int32_t nScreenY = nX - gnCameraScreenSkewY;
    D2GFX_DrawLine(nScreenX, nScreenY + 8, nScreenX + 16, nScreenY + 16, 0xFF, 0xFF);
    D2GFX_DrawLine(nScreenX, nScreenY + 8, nScreenX - 16, nScreenY + 16, 0xFF, 0xFF);
    D2GFX_DrawLine(nScreenX - 16, nScreenY + 16, nScreenX, nScreenY + 24, 0xFF, 0xFF);
    D2GFX_DrawLine(nScreenX + 16, nScreenY + 16, nScreenX, nScreenY + 24, 0xFF, 0xFF);
}

// 1.12a: 0x6FB6B980
// Calling convention: __fastcall (esi = pUnit)
int32_t __fastcall ENV_CheckUnitLineOfSight(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 1;
    }
    D2UnitStrc* pPlayer = gpPlayerUnit;
    if (!pPlayer || pPlayer == pUnit)
    {
        *(uint32_t*)((uint8_t*)pUnit + 0xC8) |= 0x80;
        return 1;
    }
    D2RoomStrc* pRoom = UNITS_GetRoom(pPlayer);
    if (pRoom && !DRLG_GetLOSDrawFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom)))
    {
        *(uint32_t*)((uint8_t*)pUnit + 0xC8) |= 0x80;
        return 1;
    }
    if (!UNITS_TestCollisionWithUnit(pPlayer, pUnit, 2))
    {
        *(uint32_t*)((uint8_t*)pUnit + 0xC8) |= 0x80;
        return 1;
    }
    *(uint32_t*)((uint8_t*)pUnit + 0xC8) &= ~0x80u;
    return 0;
}

// 1.12a: 0x6FB6BA30
// Calling convention: __userpurge (eax = bForceDraw, ecx = pUnit, [esp+4] = dwColor)
void __stdcall ENV_DrawUnitInViewport(int32_t bForceDraw, D2UnitStrc* pUnit, uint32_t dwColor)
{
    if (!pUnit)
    {
        return;
    }
    const int32_t* pViewport = static_cast<const int32_t*>(gpClientViewport);
    if (!pViewport)
    {
        return;
    }

    int32_t bPerspective = D2GFX_CheckPerspective();
    int32_t nPrecX = UNITS_GetPrecisionX(pUnit);
    int32_t nClientX = UNITS_GetClientCoordX(pUnit);
    int32_t nClientY = UNITS_GetClientCoordY(pUnit);
    if (bPerspective && !SCALE_SetPerspectiveScale(nPrecX, nClientX))
    {
        return;
    }

    if (ENV_CheckUnitLineOfSight(pUnit) || bForceDraw)
    {
        *(uint32_t*)((uint8_t*)pUnit + 0xC4) |= 0x10000000;

        const int32_t nScreenX = pViewport[1] - pViewport[9] + nClientX;
        const int32_t nScreenY = pViewport[2] - pViewport[10] + nClientY;

        const int32_t nUnitType = *reinterpret_cast<const int32_t*>(pUnit);
        if (!bForceDraw)
        {
            if (nUnitType == 0 || nUnitType == 1 || nUnitType == 2)
            {
                GFX_DrawCompositeUnitShadow(pUnit, nScreenX - 2, nScreenY);
            }
            else if (nUnitType == 4)
            {
                GFX_DrawSingleLayerUnitShadow(pUnit, nScreenX - 2, nScreenY);
            }
        }

        const int bHighlight = (pUnit == SELECT_GetSelectedUnit()) ? 1 : 0;
        GFX_DrawUnit(pUnit, dwColor, nScreenX, nScreenY, bHighlight, 0);
    }

    if (bPerspective)
    {
        SCALE_SetDefaultFactorPerspectiveFactor();
    }
}

// 1.12a: 0x6FB6BCB0
// Calling convention: __fastcall (esi = pUnit)
void __fastcall ENV_InitUnitGfxData(D2UnitStrc* pUnit)
{
    if (*(uint32_t*)pUnit > 4)
    {
        FOG_DisplayHalt(FOG_GetErrorLocation(40), "..\\Source\\D2Client\\UI\\Hireables.cpp", 40);
        exit(-1);
    }
}

// 1.12a: 0x6FB6BD20
// Calling convention: __stdcall
void __stdcall ENV_FreeUnitGfxDataThunk()
{
}

// 1.12a: 0x6FB6BD30
// Calling convention: __userpurge (eax = pTileOrientation, [esp+4] = pNumVertices, [esp+8] = nTableIdx)
const int32_t* __stdcall ENV_GetTileLightOffsets(int32_t* pTileOrientation, uint32_t* pNumVertices, int32_t nTableIdx)
{
    int32_t nOrientation = *pTileOrientation;
    *pNumVertices = g_aTileLightVertexCounts[nOrientation];
    return nTableIdx ? g_aTileLightOffsets1[nOrientation] : g_aTileLightOffsets0[nOrientation];
}

// 1.12a: 0x6FB6BD60
// Calling convention: __stdcall
void __stdcall ENV_UpdateActEnvironmentLighting()
{
    void* pAct = gpClientAct;
    if (pAct)
    {
        ENVIRONMENT_GetLightColorFromAct(pAct, &g_nEnvLightR, &g_nEnvLightG, &g_nEnvLightB);
        g_nEnvLightIntensity = ENVIRONMENT_GetIntensityFromAct(pAct);
    }

    D2UnitStrc* pPlayer = gpPlayerUnit;
    if (pPlayer)
    {
        D2RoomStrc* pRoom = UNITS_GetRoom(pPlayer);
        if (pRoom)
        {
            uint8_t nInt = 0, nR = 0, nG = 0, nB = 0;
            DUNGEON_GetRGB_IntensityFromRoom(pRoom, &nInt, &nR, &nG, &nB);
            if (nR || nG || nB)
            {
                g_nEnvLightIntensity = nInt;
                g_nEnvLightR = nR;
                g_nEnvLightG = nG;
                g_nEnvLightB = nB;
            }
        }
    }

    if (g_nEnvLightIntensity == 0 && g_nEnvLightR == 0 && g_nEnvLightG == 0 && g_nEnvLightB == 0)
    {
        g_nEnvLightIntensity = 0xFF;
        g_nEnvLightR = 0xFF;
        g_nEnvLightG = 0xFF;
        g_nEnvLightB = 0xFF;
    }
    else if (g_nEnvLightIntensity < 0xB0)
    {
        g_nEnvLightIntensity = 0xD8;
    }
}

// 1.12a: 0x6FB6BDF0
// Calling convention: __stdcall
void __stdcall ENV_DrawWallTileList(void* pTileNode, int32_t bUpperWalls, void* pViewport)
{
    if (bUpperWalls != 0 && bUpperWalls != 1)
    {
        FOG_DisplayHalt(FOG_GetErrorLocation(243), "..\\Source\\D2Client\\UI\\Hireables.cpp", 243);
        exit(-1);
    }
    if (!pTileNode || !pViewport)
    {
        return;
    }

    const int32_t* pView = static_cast<const int32_t*>(pViewport);
    const int32_t nScreenLeft = pView[1];
    const int32_t nScreenTop  = pView[2];
    const int32_t nCamIsoX    = pView[9];
    const int32_t nCamIsoY    = pView[10];

    uint32_t aWallLight[8];
    const uint32_t dwPackedLight =
        static_cast<uint32_t>(g_nEnvLightIntensity ? g_nEnvLightIntensity : 0xFFu) |
        (static_cast<uint32_t>(g_nEnvLightR ? g_nEnvLightR : 0xFFu) << 8) |
        (static_cast<uint32_t>(g_nEnvLightG ? g_nEnvLightG : 0xFFu) << 16) |
        (static_cast<uint32_t>(g_nEnvLightB ? g_nEnvLightB : 0xFFu) << 24);
    for (int k = 0; k < 8; ++k)
    {
        aWallLight[k] = dwPackedLight;
    }

    uint8_t* pCell = static_cast<uint8_t*>(pTileNode);
    uint8_t* pNode = *reinterpret_cast<uint8_t**>(pCell + 0x14);
    while (pNode != nullptr)
    {
        uint8_t* pWallTile = *reinterpret_cast<uint8_t**>(pNode + 0x0C);
        pNode = *reinterpret_cast<uint8_t**>(pNode + 0x10);
        if (!pWallTile)
        {
            continue;
        }

        const uint32_t dwTileFlags = *reinterpret_cast<const uint32_t*>(pWallTile + 0x14);
        const bool bPriorityGroup = (dwTileFlags & 0x1C000u) != 0;
        if ((bUpperWalls == 1 && bPriorityGroup) || (bUpperWalls == 0 && !bPriorityGroup))
        {
            continue;
        }

        D2TileLibraryEntryStrc* pTileLib = *reinterpret_cast<D2TileLibraryEntryStrc**>(pWallTile + 0x18);
        if (!pTileLib)
        {
            continue;
        }

        const uint8_t nAlpha = *(pWallTile + 0x28);
        if (nAlpha == 0)
        {
            continue;
        }

        const int32_t nIsoX = *reinterpret_cast<const int32_t*>(pWallTile + 0x00);
        const int32_t nIsoY = *reinterpret_cast<const int32_t*>(pWallTile + 0x04);
        const int32_t nScreenX = nIsoX + nScreenLeft - nCamIsoX;
        const int32_t nScreenY = nIsoY + nScreenTop - nCamIsoY;

        if (nAlpha == 0xFF)
        {
            TILE_TileDrawLit(pTileLib, nScreenX, nScreenY, reinterpret_cast<D2GfxLightStrc*>(aWallLight), 0);
        }
        else
        {
            TILE_TileDrawTrans(pTileLib, nScreenX, nScreenY, reinterpret_cast<D2GfxLightStrc*>(aWallLight), 0, nAlpha);
        }
    }
}

// 1.12a: 0x6FB6C090
// Calling convention: __userpurge (eax = pCellNode, [esp+4] = pViewport)
void __stdcall ENV_DrawFloorTileList(void* pCellNode, void* pViewport)
{
    if (!pCellNode || !pViewport)
    {
        return;
    }

    const int32_t* pView = static_cast<const int32_t*>(pViewport);
    const int32_t nScreenLeft = pView[1];
    const int32_t nScreenTop  = pView[2];
    const int32_t nCamIsoX    = pView[9];
    const int32_t nCamIsoY    = pView[10];

    uint32_t aWallLight[8];
    const uint32_t dwPackedLight =
        static_cast<uint32_t>(g_nEnvLightIntensity ? g_nEnvLightIntensity : 0xFFu) |
        (static_cast<uint32_t>(g_nEnvLightR ? g_nEnvLightR : 0xFFu) << 8) |
        (static_cast<uint32_t>(g_nEnvLightG ? g_nEnvLightG : 0xFFu) << 16) |
        (static_cast<uint32_t>(g_nEnvLightB ? g_nEnvLightB : 0xFFu) << 24);
    for (int k = 0; k < 8; ++k)
    {
        aWallLight[k] = dwPackedLight;
    }

    uint8_t* pCell = static_cast<uint8_t*>(pCellNode);
    uint8_t* pNode = *reinterpret_cast<uint8_t**>(pCell + 0x20); // pUpperWallList (orientations 16..19: lower cliff/river walls)
    while (pNode != nullptr)
    {
        uint8_t* pWallTile = *reinterpret_cast<uint8_t**>(pNode + 0x0C);
        pNode = *reinterpret_cast<uint8_t**>(pNode + 0x10);
        if (!pWallTile)
        {
            continue;
        }
        D2TileLibraryEntryStrc* pTileLib = *reinterpret_cast<D2TileLibraryEntryStrc**>(pWallTile + 0x18);
        if (!pTileLib)
        {
            continue;
        }
        const uint8_t nAlpha = *(pWallTile + 0x28);
        if (nAlpha == 0)
        {
            continue;
        }
        const int32_t nIsoX = *reinterpret_cast<const int32_t*>(pWallTile + 0x00);
        const int32_t nIsoY = *reinterpret_cast<const int32_t*>(pWallTile + 0x04);
        const int32_t nScreenX = nIsoX + nScreenLeft - nCamIsoX;
        const int32_t nScreenY = nIsoY + nScreenTop - nCamIsoY;
        if (nAlpha == 0xFF)
        {
            TILE_TileDrawLit(pTileLib, nScreenX, nScreenY, reinterpret_cast<D2GfxLightStrc*>(aWallLight), 0);
        }
        else
        {
            TILE_TileDrawTrans(pTileLib, nScreenX, nScreenY, reinterpret_cast<D2GfxLightStrc*>(aWallLight), 0, nAlpha);
        }
    }
}

// 1.12a: 0x6FB6C250
// Calling convention: __fastcall (edi = pViewport)
void __fastcall ENV_DrawAllFloorTiles(uint8_t* pViewport)
{
    int32_t nGridDim = *(int32_t*)(pViewport + 0xEAB0);
    int32_t nTotalCells = nGridDim * nGridDim;
    uint8_t* pCell = *(uint8_t**)(pViewport + 0xEAA8);
    if (!pCell)
    {
        return;
    }
    for (int32_t i = 0; i < nTotalCells; ++i, pCell += 0x24)
    {
        if (*(void**)(pCell + 0x20))
        {
            ENV_DrawFloorTileList(pCell, pViewport);
        }
    }
}

// 1.12a: 0x6FB6C280
// Calling convention: __stdcall
void __stdcall ENV_DrawWallsAndUnitsInCells(uint8_t* pViewport)
{
    int32_t nGridDim = *(int32_t*)(pViewport + 0xEAB0);
    int32_t nTotalCells = nGridDim * nGridDim;
    uint8_t* pCell = *(uint8_t**)(pViewport + 0xEAA8);
    if (!pCell)
    {
        return;
    }
    for (int32_t i = 0; i < nTotalCells; ++i, pCell += 0x24)
    {
        if (*(void**)(pCell + 0x14))
        {
            ENV_DrawWallTileList(pCell, 0, pViewport);
        }

        uint8_t* pNode = *reinterpret_cast<uint8_t**>(pCell + 0x18); // pNormalUnitList
        while (pNode != nullptr)
        {
            D2UnitStrc* pUnit = *reinterpret_cast<D2UnitStrc**>(pNode + 0x0C);
            pNode = *reinterpret_cast<uint8_t**>(pNode + 0x10);
            if (pUnit != nullptr)
            {
                ENV_DrawUnitInViewport(0, pUnit, 0xFFFFFFFFu);
            }
        }
    }
}

// 1.12a: 0x6FB6C5F0
// Calling convention: __fastcall (eax = nScreenPanels, ecx = pShadowNode, esi = pViewport)
void __fastcall ENV_DrawShadowTileNode(int32_t nScreenPanels, uint32_t* pShadowNode, uint32_t* pViewport)
{
    if (pShadowNode[0] & 1)
    {
        uint32_t* pTileData = (uint32_t*)pShadowNode[3];
        if (!pTileData || !pTileData[6])
        {
            return;
        }
        if (D2GFX_CheckPerspective())
        {
            TILE_ShadowTileDraw((D2TileLibraryEntryStrc*)pTileData[6], pShadowNode[1], pShadowNode[2], 4, nScreenPanels);
        }
        else
        {
            int32_t nX = static_cast<int32_t>(pViewport[1]) - static_cast<int32_t>(pViewport[9]) + static_cast<int32_t>(pTileData[0]);
            int32_t nY = static_cast<int32_t>(pTileData[1]) - static_cast<int32_t>(pViewport[10]) + static_cast<int32_t>(pViewport[2]);
            TILE_ShadowTileDraw((D2TileLibraryEntryStrc*)pTileData[6], nX, nY, 4, 0);
        }
    }
}

// 1.12a: 0x6FB6C660
// Calling convention: __fastcall (edi = pViewport)
void __fastcall ENV_DrawShadowsAndUpperWalls(uint8_t* pViewport)
{
    int32_t nGridDim = *(int32_t*)(pViewport + 0xEAB0);
    int32_t nTotalCells = nGridDim * nGridDim;
    uint8_t* pCell = *(uint8_t**)(pViewport + 0xEAA8);
    if (!pCell)
    {
        return;
    }
    const int32_t* pView = reinterpret_cast<const int32_t*>(pViewport);
    for (int32_t i = 0; i < nTotalCells; ++i, pCell += 0x24)
    {
        uint8_t* pNode = *reinterpret_cast<uint8_t**>(pCell + 0x10); // pFloorAndRoofList
        while (pNode != nullptr)
        {
            const int32_t nNodeType = *reinterpret_cast<const int32_t*>(pNode + 0x00);
            void* pOwner = *reinterpret_cast<void**>(pNode + 0x0C);
            uint8_t* pCurNode = pNode;
            pNode = *reinterpret_cast<uint8_t**>(pNode + 0x10);
            if (!pOwner)
            {
                continue;
            }
            if (nNodeType == 0)
            {
                ENV_DrawUnitInViewport(1, static_cast<D2UnitStrc*>(pOwner), 0xFFFFFFFFu);
            }
            else if (nNodeType & 1)
            {
                ENV_DrawShadowTileNode(0, reinterpret_cast<uint32_t*>(pCurNode), reinterpret_cast<uint32_t*>(pViewport));
            }
            else if (nNodeType & 2)
            {
                D2UnitStrc* pUnit = static_cast<D2UnitStrc*>(pOwner);
                const int32_t nScreenX = pView[1] - pView[9] + UNITS_GetClientCoordX(pUnit);
                const int32_t nScreenY = pView[2] - pView[10] + UNITS_GetClientCoordY(pUnit);
                GFX_DrawCompositeUnitShadow(pUnit, nScreenX - 2, nScreenY);
            }
        }

        if (*(void**)(pCell + 0x14))
        {
            ENV_DrawWallTileList(pCell, 1, pViewport);
        }
    }
}

// ----------------------------------------------------------------------------
// UI Spinner Widget Class (0x6FB6C7E0..0x6FB6CEB0)
// ----------------------------------------------------------------------------

// 1.12a: 0x6FB6C7E0
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Proxy_GetTickDelta(void* pThis, void* _edx, int32_t a1)
{
    void* pImpl = *((void**)pThis + 2);
    return (*(int32_t(__thiscall**)(void*, int32_t))(*(uintptr_t*)pImpl + 0x30))(pImpl, a1);
}

// 1.12a: 0x6FB6C7F0
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Proxy_GetState(void* pThis)
{
    void* pImpl = *((void**)pThis + 2);
    return (*(int32_t(__thiscall**)(void*))(*(uintptr_t*)pImpl + 0x2C))(pImpl);
}

// 1.12a: 0x6FB6C800
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Proxy_VFunc10(void* pThis, void* _edx, int32_t a1)
{
    void* pImpl = *((void**)pThis + 2);
    return (*(int32_t(__thiscall**)(void*, int32_t))(*(uintptr_t*)pImpl + 0x28))(pImpl, a1);
}

// 1.12a: 0x6FB6C810
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Proxy_VFunc9(void* pThis, void* _edx, int32_t a1)
{
    void* pImpl = *((void**)pThis + 2);
    return (*(int32_t(__thiscall**)(void*, int32_t))(*(uintptr_t*)pImpl + 0x24))(pImpl, a1);
}

// 1.12a: 0x6FB6C820
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Proxy_VFunc7(void* pThis, void* _edx, int32_t a1)
{
    void* pImpl = *((void**)pThis + 2);
    return (*(int32_t(__thiscall**)(void*, int32_t))(*(uintptr_t*)pImpl + 0x1C))(pImpl, a1);
}

// 1.12a: 0x6FB6C830
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Proxy_VFunc8(void* pThis, void* _edx, int32_t a1)
{
    void* pImpl = *((void**)pThis + 2);
    return (*(int32_t(__thiscall**)(void*, int32_t))(*(uintptr_t*)pImpl + 0x20))(pImpl, a1);
}

// 1.12a: 0x6FB6C840
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Proxy_VFunc6(void* pThis, void* _edx, int32_t a1)
{
    void* pImpl = *((void**)pThis + 2);
    return (*(int32_t(__thiscall**)(void*, int32_t))(*(uintptr_t*)pImpl + 0x18))(pImpl, a1);
}

// 1.12a: 0x6FB6C850
// Calling convention: __thiscall (ecx = this)
void __fastcall SPINNER_Proxy_Draw(void* pThis)
{
    void* pImpl = *((void**)pThis + 2);
    (*(void(__thiscall**)(void*))(*(uintptr_t*)pImpl + 0x04))(pImpl);
}

// 1.12a: 0x6FB6C860
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Proxy_VFunc5(void* pThis, void* _edx, int32_t a1)
{
    void* pImpl = *((void**)pThis + 2);
    return (*(int32_t(__thiscall**)(void*, int32_t))(*(uintptr_t*)pImpl + 0x14))(pImpl, a1);
}

// 1.12a: 0x6FB6C870
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Proxy_OnMouseUp(void* pThis, void* _edx, D2WindowMsgStrc* pMsg)
{
    void* pImpl = *((void**)pThis + 2);
    return (*(int32_t(__thiscall**)(void*, D2WindowMsgStrc*))(*(uintptr_t*)pImpl + 0x10))(pImpl, pMsg);
}

// 1.12a: 0x6FB6C880
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Proxy_OnMouseDown(void* pThis, void* _edx, D2WindowMsgStrc* pMsg)
{
    void* pImpl = *((void**)pThis + 2);
    return (*(int32_t(__thiscall**)(void*, D2WindowMsgStrc*))(*(uintptr_t*)pImpl + 0x0C))(pImpl, pMsg);
}

// 1.12a: 0x6FB6C890
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Proxy_OnKeyDown(void* pThis, void* _edx, D2WindowMsgStrc* pMsg)
{
    void* pImpl = *((void**)pThis + 2);
    return (*(int32_t(__thiscall**)(void*, D2WindowMsgStrc*))(*(uintptr_t*)pImpl + 0x08))(pImpl, pMsg);
}

extern const void* const g_aSpinnerProxyVtable[13];
extern const void* const g_aSpinnerImplVtable[13];
static const void* const g_aWidgetBaseVtable[13] = { nullptr };

// 1.12a: 0x6FB6C8A0
// Calling convention: __thiscall (ecx = this)
void __fastcall SPINNER_Proxy_Destructor(void* pThis)
{
    *(const void* const**)pThis = g_aSpinnerProxyVtable;
    void* pImpl = *((void**)pThis + 2);
    if (pImpl)
    {
        (*(void(__thiscall**)(void*, int32_t))(*(uintptr_t*)pImpl))(pImpl, 1);
    }
    *(const void* const**)pThis = g_aWidgetBaseVtable;
}

// 1.12a: 0x6FB6C900
// Calling convention: __stdcall
int32_t __stdcall SPINNER_Impl_Stub10(int32_t a1) { return 0; }

// 1.12a: 0x6FB6C910
// Calling convention: __stdcall
int32_t __stdcall SPINNER_Impl_Stub9(int32_t a1) { return 0; }

// 1.12a: 0x6FB6C920
// Calling convention: __stdcall
int32_t __stdcall SPINNER_Impl_Stub8(int32_t a1) { return 0; }

// 1.12a: 0x6FB6C930
// Calling convention: __stdcall
int32_t __stdcall SPINNER_Impl_Stub7(int32_t a1) { return 0; }

// 1.12a: 0x6FB6C940
// Calling convention: __stdcall
int32_t __stdcall SPINNER_Impl_Stub5(int32_t a1) { return 0; }

// 1.12a: 0x6FB6C950
// Calling convention: __stdcall
int32_t __stdcall SPINNER_Impl_Stub6(int32_t a1) { return 0; }

// 1.12a: 0x6FB6C960
// Calling convention: __thiscall (ecx = this, [esp+4] = pMsg)
int32_t __fastcall SPINNER_Impl_OnMouseUp(int32_t* pThis, void* _edx, D2WindowMsgStrc* pMsg)
{
    if (pThis[12] == 0)
    {
        return 0;
    }
    pThis[16] = 0;
    pThis[12] = 0;
    if (pThis[13] == 1)
    {
        pThis[13] = 0;
        return 1;
    }
    if (pThis[14] == 1)
    {
        pThis[14] = 0;
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB6C9A0
// Calling convention: __thiscall (ecx = this, [esp+4] = pMsg)
int32_t __fastcall SPINNER_Impl_OnMouseWheel(int32_t* pThis, void* _edx, D2WindowMsgStrc* pMsg)
{
    if (pMsg->nWheelDelta > 0)
    {
        pThis[13] = 1;
        pThis[14] = 0;
        pThis[8] = 1;
    }
    else if (pMsg->nWheelDelta < 0)
    {
        pThis[13] = 0;
        pThis[14] = 1;
        pThis[8] = 1;
    }
    pMsg->bHandled = 1;
    pMsg->nReturnValue = 0;
    SMsgBreakHandlerChain(pMsg);
    return 1;
}

// 1.12a: 0x6FB6C9F0
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Impl_GetRepeatStep(int32_t* pThis)
{
    if (!pThis[13] && !pThis[14])
    {
        return 0;
    }
    if (pThis[8])
    {
        pThis[13] = 0;
        pThis[14] = 0;
        pThis[8] = 0;
        return 1;
    }
    uint32_t dwTick = GetTickCount();
    if (pThis[19])
    {
        pThis[18] = dwTick;
        return 1;
    }
    uint32_t dwTotalElapsed = dwTick - (uint32_t)pThis[17];
    if (dwTick - (uint32_t)pThis[18] < 70)
    {
        return 0;
    }
    pThis[18] = dwTick;
    if (dwTotalElapsed > 4096) return (dwTotalElapsed >> 11) * pThis[16];
    if (dwTotalElapsed > 3000) return (uint32_t)pThis[16] >> 2;
    if (dwTotalElapsed > 2000) return (uint32_t)pThis[16] >> 4;
    if (dwTotalElapsed > 1000) return (uint32_t)pThis[16] >> 5;
    return 1;
}

// 1.12a: 0x6FB6CAA0
// Calling convention: __thiscall (ecx = this)
int32_t __fastcall SPINNER_Impl_GetActiveDirection(int32_t* pThis)
{
    if (pThis[13]) return 1;
    return pThis[14] ? 2 : 0;
}

// 1.12a: 0x6FB6CAC0
// Calling convention: __fastcall (eax = pSpinnerImpl, edx = pMsg)
int32_t __fastcall SPINNER_Impl_HitTestDownBtn(int32_t* pSpinnerImpl, D2WindowMsgStrc* pMsg)
{
    if (pMsg->wX > pSpinnerImpl[6] && pMsg->wX < pSpinnerImpl[6] + pSpinnerImpl[10] &&
        pMsg->wY > pSpinnerImpl[7] - pSpinnerImpl[9] && pMsg->wY < pSpinnerImpl[7])
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB6CB00
// Calling convention: __fastcall (eax = pSpinnerImpl, edx = pMsg)
int32_t __fastcall SPINNER_Impl_HitTestUpBtn(int32_t* pSpinnerImpl, D2WindowMsgStrc* pMsg)
{
    if (pMsg->wX > pSpinnerImpl[4] && pMsg->wX < pSpinnerImpl[4] + pSpinnerImpl[10] &&
        pMsg->wY > pSpinnerImpl[5] - pSpinnerImpl[9] && pMsg->wY <= pSpinnerImpl[5])
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB6CB40
// Calling convention: __thiscall (ecx = this)
void __fastcall SPINNER_Impl_Draw(int32_t* pThis)
{
    if (pThis[14] || pThis[13])
    {
        pThis[16]++;
        ((void(__stdcall*)())pThis[15])();
    }
    D2GfxDataStrc tGfx = { 0 };
    tGfx.pCellFile = (D2CellFileStrc*)pThis[20];
    tGfx.nFrame = pThis[13] << 1;
    TEXTURE_CelDraw(&tGfx, pThis[4], pThis[5], 0xFFFFFFFF, 5, nullptr);
    tGfx.nFrame = (pThis[14] << 1) + 1;
    TEXTURE_CelDraw(&tGfx, pThis[6], pThis[7], 0xFFFFFFFF, 5, nullptr);
}

// 1.12a: 0x6FB6CBD0
// Calling convention: __thiscall (ecx = this, [esp+4] = pMsg)
int32_t __fastcall SPINNER_Impl_OnMouseDown(int32_t* pThis, void* _edx, D2WindowMsgStrc* pMsg)
{
    pThis[12] = 1;
    if (SPINNER_Impl_HitTestUpBtn(pThis, pMsg))
    {
        pThis[17] = GetTickCount();
        pThis[13] = 1;
        pThis[19] = 1;
        ((void(__stdcall*)())pThis[15])();
        pThis[19] = 0;
        return 1;
    }
    if (SPINNER_Impl_HitTestDownBtn(pThis, pMsg))
    {
        pThis[17] = GetTickCount();
        pThis[14] = 1;
        pThis[19] = 1;
        ((void(__stdcall*)())pThis[15])();
        pThis[19] = 0;
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB6CC90
// Calling convention: __stdcall
void __stdcall SPINNER_Impl_Destructor(int32_t* pThis)
{
    *(const void* const**)pThis = g_aSpinnerImplVtable;
    D2CellFileStrc* pCel = (D2CellFileStrc*)pThis[20];
    D2CMP_CelFileFreeHardware(pCel);
    if (pCel)
    {
        FOG_Free(pCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
    }
    *(const void* const**)pThis = g_aWidgetBaseVtable;
}

static void __fastcall SPINNER_Impl_ScalarDeletingDestructor(int32_t* pThis, void* _edx, int32_t bDelete)
{
    SPINNER_Impl_Destructor(pThis);
    if (bDelete & 1)
    {
        FOG_Free(pThis, "delete", -1, 0);
    }
}

const void* const g_aSpinnerProxyVtable[13] = {
    (const void*)&SPINNER_Proxy_Destructor,
    (const void*)&SPINNER_Proxy_Draw,
    (const void*)&SPINNER_Proxy_OnKeyDown,
    (const void*)&SPINNER_Proxy_OnMouseDown,
    (const void*)&SPINNER_Proxy_OnMouseUp,
    (const void*)&SPINNER_Proxy_VFunc5,
    (const void*)&SPINNER_Proxy_VFunc6,
    (const void*)&SPINNER_Proxy_VFunc7,
    (const void*)&SPINNER_Proxy_VFunc8,
    (const void*)&SPINNER_Proxy_VFunc9,
    (const void*)&SPINNER_Proxy_VFunc10,
    (const void*)&SPINNER_Proxy_GetState,
    (const void*)&SPINNER_Proxy_GetTickDelta
};

const void* const g_aSpinnerImplVtable[13] = {
    (const void*)&SPINNER_Impl_ScalarDeletingDestructor,
    (const void*)&SPINNER_Impl_Draw,
    (const void*)&SPINNER_Impl_OnMouseWheel,
    (const void*)&SPINNER_Impl_OnMouseDown,
    (const void*)&SPINNER_Impl_OnMouseUp,
    (const void*)&SPINNER_Impl_Stub5,
    (const void*)&SPINNER_Impl_Stub6,
    (const void*)&SPINNER_Impl_Stub7,
    (const void*)&SPINNER_Impl_Stub8,
    (const void*)&SPINNER_Impl_Stub9,
    (const void*)&SPINNER_Impl_Stub10,
    (const void*)&SPINNER_Impl_GetActiveDirection,
    (const void*)&SPINNER_Impl_GetRepeatStep
};

// 1.12a: 0x6FB6CD00
// Calling convention: __stdcall
void* __stdcall SPINNER_Impl_Constructor(int32_t* pThis, int32_t nY, int32_t nX, int32_t bSideBySide, void* pfnCallback)
{
    char szPath[260];
    *(const void* const**)pThis = g_aSpinnerImplVtable;
    sprintf(szPath, "%s\\ui\\menu\\spinner", "DATA\\GLOBAL");
    D2CellFileStrc* pCel = ARCHIVE_LoadCellFileDefault(szPath, 0);
    pThis[2]  = nX;
    pThis[3]  = nY;
    pThis[11] = bSideBySide;
    pThis[15] = (int32_t)(uintptr_t)pfnCallback;
    pThis[20] = (int32_t)(uintptr_t)pCel;
    pThis[13] = 0;
    pThis[14] = 0;
    pThis[12] = 0;
    pThis[19] = 0;
    pThis[16] = 0;

    D2GfxDataStrc tGfx = { 0 };
    tGfx.pCellFile = pCel;
    void* pHandle = D2CMP_CelGetHandle(&tGfx);
    pThis[9]  = D2CMP_CelGetHeight(pHandle);
    pThis[10] = D2CMP_CelGetWidth(pHandle);
    pThis[5]  = nY;
    if (bSideBySide)
    {
        pThis[4] = nX + 1;
        pThis[6] = nX + pThis[10];
        pThis[7] = nY;
    }
    else
    {
        pThis[4] = nX + 1;
        pThis[6] = nX + 1;
        pThis[7] = nY + pThis[9] + 1;
    }
    return pThis;
}

// 1.12a: 0x6FB6CE30
// Calling convention: __stdcall
void* __stdcall SPINNER_Proxy_Constructor(int32_t* pThis, int32_t nX, int32_t nY, int32_t bSideBySide, void* pfnCallback)
{
    *(const void* const**)pThis = g_aSpinnerProxyVtable;
    int32_t* pImpl = (int32_t*)SMemAlloc(0x54, "new", -1, 0);
    if (pImpl)
    {
        pImpl = (int32_t*)SPINNER_Impl_Constructor(pImpl, nY, nX, bSideBySide, pfnCallback);
    }
    pThis[2] = (int32_t)(uintptr_t)pImpl;
    return pThis;
}

// ----------------------------------------------------------------------------
// Hireables / Pet / Party Portraits UI (0x6FB6CEC0..0x6FB6EEB0)
// ----------------------------------------------------------------------------

// 1.12a: 0x6FB6CEC0
// Calling convention: __stdcall
void __stdcall HIREABLES_InitPanelState()
{
    g_nPortraitDisplayMode = 0;
    uint32_t dwVal = 0;
    SRegLoadValue("Diablo II", "PopupHireling", 0, &dwVal);
    if (dwVal == 0)
    {
        g_bPopupHirelingPending = 1;
    }
}

// 1.12a: 0x6FB6CF10
// Calling convention: __stdcall
D2UnitStrc* __stdcall HIREABLES_GetLocalCursorItem()
{
    D2UnitStrc* pPlayer = gpPlayerUnit;
    if (!pPlayer)
    {
        return nullptr;
    }
    return INVENTORY_GetCursorItem(*(void**)((uint8_t*)pPlayer + 0x60));
}

// 1.12a: 0x6FB6CF30
// Calling convention: __fastcall (eax = pLabelY, edx = pStaggerOffset)
void __fastcall HIREABLES_AlternateLabelStagger(int32_t* pLabelY, int32_t* pStaggerOffset)
{
    if (*pStaggerOffset != 0)
    {
        *pLabelY -= *pStaggerOffset;
        *pStaggerOffset = 0;
    }
    else
    {
        *pStaggerOffset = 10;
        *pLabelY += 10;
    }
}

// 1.12a: 0x6FB6CF50
// Calling convention: __userpurge (esi = wszText, [esp+4] = nX, [esp+8] = nY, [esp+12] = nColor)
void __stdcall HIREABLES_DrawPortraitLabel(Unicode* wszText, int32_t nX, int32_t nY, int32_t nColor)
{
    int32_t nOldFont = D2Win_10127_SetFont(6);
    Unicode* pEnd = &wszText[31];
    while (D2Win_10121_GetTextWidth(wszText) > 0x42 && pEnd > wszText)
    {
        --pEnd;
        pEnd->wChar = 0;
    }
    int32_t nWidth = D2Win_10121_GetTextWidth(wszText);
    int32_t nDrawX = nX + 22 - (nWidth / 2);
    if (nDrawX < 0)
    {
        nDrawX = 0;
    }
    D2Win_10117_DrawText(wszText, nDrawX, nY, nColor, 0);
    D2Win_10127_SetFont(nOldFont);
}

// 1.12a: 0x6FB6CFD0
// Calling convention: __stdcall
void __stdcall HIREABLES_ResetLifeBarColors()
{
    g_bLifeBarColorsInit = 0;
}

// 1.12a: 0x6FB6CFE0
// Calling convention: __fastcall (eax = bForceReinit)
void __fastcall HIREABLES_InitLifeBarColors(int32_t bForceReinit)
{
    if (!bForceReinit)
    {
        if (g_bLifeBarColorsInit)
        {
            return;
        }
        g_bLifeBarColorsInit = 1;
    }
    g_nLifeBarColorGreen  = D2Win_10034_MixRGB(0, 0x80, 0);
    g_nLifeBarColorYellow = D2Win_10034_MixRGB(0xC0, 0xC0, 0);
    g_nLifeBarColorRed    = D2Win_10034_MixRGB(0xFF, 0, 0);
}

// 1.12a: 0x6FB6D040
// Calling convention: __stdcall
int32_t __stdcall HIREABLES_ArePortraitsVisible()
{
    return (g_nPortraitDisplayMode != 2) ? 1 : 0;
}

// 1.12a: 0x6FB6D050
// Calling convention: __fastcall (ecx = pMsg)
int32_t __fastcall HIREABLES_IsMouseOverMercPortrait(D2WindowMsgStrc* pMsg)
{
    if (g_nPortraitDisplayMode != 2 &&
        pMsg->wX >= g_nMercPortraitX && pMsg->wX <= g_nMercPortraitX + 46 &&
        pMsg->wY >= 24 && pMsg->wY <= 60)
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB6D090
// Calling convention: __stdcall
void __stdcall HIREABLES_DrawSummonCount(int32_t nCount, int32_t nX)
{
    char szCount[32];
    Unicode wszTemp[32] = { 0 };
    Unicode wszCount[32] = { 0 };
    sprintf(szCount, "%d", nCount);
    Unicode_toUnicode(wszTemp, szCount, 32);
    Unicode_strncpy(wszCount, wszTemp, 32);
    wszCount[31].wChar = 0;

    int32_t nOldFont = D2Win_10127_SetFont(6);
    D2Win_10117_DrawText(wszCount, nX + 8, 55, 0, 0);
    D2Win_10127_SetFont(nOldFont);
}

// 1.12a: 0x6FB6D180
// Calling convention: __stdcall
void __stdcall HIREABLES_DrawAsciiPortraitLabel(const char* szName, int32_t nX, int32_t nY)
{
    Unicode wszName[32] = { 0 };
    Unicode_toUnicode(wszName, szName, 32);
    HIREABLES_DrawPortraitLabel(wszName, nX, nY, 0);
}

// 1.12a: 0x6FB6D1D0
// Calling convention: __userpurge (eax = nPetType, [esp+4] = nX, [esp+8] = nY)
void __stdcall HIREABLES_DrawPetTypeNameLabel(int32_t nPetType, int32_t nX, int32_t nY)
{
    const D2PetTypeTxt* pRecord = static_cast<const D2PetTypeTxt*>(D2Client_GetPetTypeRecord(nPetType));
    if (!pRecord)
    {
        return;
    }
    if (pRecord->wNameStr > 0 && pRecord->wNameStr != 0x1506)
    {
        Unicode wszLabel[32] = { 0 };
        Unicode_strcpy(wszLabel, D2LANG_GetStringFromTblIndex(pRecord->wNameStr));
        HIREABLES_DrawPortraitLabel(wszLabel, nX, nY, 0);
    }
}

// 1.12a: 0x6FB6D260
// Calling convention: __stdcall
void __stdcall HIREABLES_FreePortraitCellFiles()
{
    for (int i = 0; i < 7; ++i)
    {
        if (g_apClassIcons[i])
        {
            D2CMP_CelFileFreeHardware(g_apClassIcons[i]);
            FOG_Free(g_apClassIcons[i], "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
            g_apClassIcons[i] = nullptr;
        }
    }
    if (g_ppPetTypeIcons)
    {
        int32_t nTotalIcons = D2Client_GetPetTypeCount() * 5;
        for (int i = 0; i < nTotalIcons; ++i)
        {
            if (g_ppPetTypeIcons[i])
            {
                D2CMP_CelFileFreeHardware(g_ppPetTypeIcons[i]);
                FOG_Free(g_ppPetTypeIcons[i], "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
            }
        }
        FOG_FreePool(nullptr, g_ppPetTypeIcons, "..\\Source\\D2Client\\UI\\Hireables.cpp", 183, 0);
        g_ppPetTypeIcons = nullptr;
    }
}

// 1.12a: 0x6FB6D320
// Calling convention: __stdcall
void __stdcall HIREABLES_OnRightClickMercPortrait(D2WindowMsgStrc* pMsg)
{
    if (g_nPortraitDisplayMode == 2)
    {
        pMsg->bHandled = 1;
        pMsg->nReturnValue = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }
    pMsg->bHandled = 0;
}

// 1.12a: 0x6FB6D3E0
// Calling convention: __userpurge (eax = nPetType, ecx = pDrawHpFlag, edx = pPlayerUnit, [esp+4] = pOutPetGuid)
D2CellFileStrc* __stdcall HIREABLES_GetOrLoadPetPortraitCellFile(
    int32_t nPetType,
    int32_t* pDrawHpFlag,
    D2UnitStrc* pPlayerUnit,
    int32_t* pOutPetGuid)
{
    const D2PetTypeTxt* pRecord = static_cast<const D2PetTypeTxt*>(D2Client_GetPetTypeRecord(nPetType));
    if (!pRecord)
    {
        return nullptr;
    }
    if (pDrawHpFlag)
    {
        *pDrawHpFlag = (pRecord->nFlags & 0x20) ? 1 : 0;
    }
    int32_t nFoundGuid = -1;
    if (pPlayerUnit)
    {
        int32_t nOwnerGuid = *(int32_t*)((uint8_t*)pPlayerUnit + 0x0C);
        for (D2PetInfoStrc* pPet = g_pPetInfoList; pPet; pPet = pPet->pNext)
        {
            if (pPet->nPetType == nPetType && pPet->nOwnerGuid == nOwnerGuid && pPet->nOmitDraw == 0)
            {
                nFoundGuid = pPet->nUnitGuid;
                break;
            }
        }
    }
    *pOutPetGuid = nFoundGuid;
    if (nFoundGuid == -1)
    {
        return nullptr;
    }
    int32_t nVariant = 0;
    int32_t nSlot = nPetType * 5 + nVariant;
    if (!g_ppPetTypeIcons[nSlot])
    {
        char szPath[260];
        sprintf(szPath, "hireables\\%s", pRecord->szBaseIcon);
        g_ppPetTypeIcons[nSlot] = UI_LoadPanelCellFile(szPath);
    }
    return g_ppPetTypeIcons[nSlot];
}

// 1.12a: 0x6FB6D520
// Calling convention: __userpurge (ebx = pMercUnit, [esp+4] = nX, [esp+8] = nY)
void __stdcall HIREABLES_DrawMercenaryName(D2UnitStrc* pMercUnit, int32_t nX, int32_t nY)
{
    const Unicode* pName = nullptr;
    int32_t nMercGuid = pMercUnit ? *(int32_t*)((uint8_t*)pMercUnit + 0x0C) : -1;
    for (D2PetInfoStrc* pPet = g_pPetInfoList; pPet; pPet = pPet->pNext)
    {
        if (pPet->nPetType == 7 && (nMercGuid == -1 || pPet->nUnitGuid == nMercGuid))
        {
            if (pPet->dwNameIdx != 0)
            {
                pName = D2LANG_GetStringFromTblIndex((uint16_t)pPet->dwNameIdx);
            }
            break;
        }
    }
    if (!pName && pMercUnit)
    {
        pName = D2Client_GetNpcDisplayName(pMercUnit);
    }
    if (pName)
    {
        Unicode wszName[32] = { 0 };
        for (int i = 0; i < 31 && pName[i].wChar != 0; ++i)
        {
            wszName[i] = pName[i];
        }
        HIREABLES_DrawPortraitLabel(wszName, nX, nY, 0);
    }
}

// 1.12a: 0x6FB6D590
// Calling convention: __userpurge (eax = nPetType, edx = nUnitGuid, [esp+4] = nX)
void __stdcall HIREABLES_DrawPortraitLifeBar(int32_t nPetType, int32_t nUnitGuid, int32_t nX)
{
    int32_t nLifePct = 0;
    if (nPetType != 0)
    {
        for (D2PetInfoStrc* pPet = g_pPetInfoList; pPet; pPet = pPet->pNext)
        {
            if (pPet->nUnitGuid == nUnitGuid)
            {
                nLifePct = pPet->nLifePercent;
                break;
            }
        }
    }
    else
    {
        D2RosterUnitStrc* pRoster = ROSTER_FindByUnitGuid(nUnitGuid);
        if (pRoster)
        {
            nLifePct = pRoster->dwPartyLife;
        }
    }
    if (nLifePct <= 0)
    {
        return;
    }
    if (nLifePct > 100)
    {
        nLifePct = 100;
    }
    HIREABLES_InitLifeBarColors(0);
    uint8_t nColor = g_nLifeBarColorGreen;
    if (nLifePct < 50) nColor = g_nLifeBarColorYellow;
    if (nLifePct < 25) nColor = g_nLifeBarColorRed;

    int32_t nFilledEnd = nX + (nLifePct * 46) / 100;
    D2GFX_DrawBox(nX, 14, nFilledEnd, 19, nColor, 5);
    D2GFX_DrawBox(nFilledEnd, 14, nX + 46, 19, 0, 5);
}

// 1.12a: 0x6FB6D6B0
// Calling convention: __stdcall
void __stdcall HIREABLES_LoadPortraitCellFiles()
{
    static const char* const s_apszBaseIcons[5] = {
        "Hireables\\AmazonIcon",
        "Hireables\\SorceressIcon",
        "Hireables\\BarbarianIcon",
        "Hireables\\PaladinIcon",
        "Hireables\\NecromancerIcon"
    };
    memset(g_apClassIcons, 0, sizeof(g_apClassIcons));
    for (int i = 0; i < 5; ++i)
    {
        g_apClassIcons[i] = UI_LoadPanelCellFile(s_apszBaseIcons[i]);
    }
    if (FOG_IsExpansion())
    {
        g_apClassIcons[5] = UI_LoadPanelCellFile("Hireables\\AssassinIcon");
        g_apClassIcons[6] = UI_LoadPanelCellFile("Hireables\\DruidIcon");
    }
    int32_t nAllocSize = D2Client_GetPetTypeCount() * 5 * sizeof(D2CellFileStrc*);
    g_ppPetTypeIcons = (D2CellFileStrc**)FOG_AllocPool(nullptr, nAllocSize, "..\\Source\\D2Client\\UI\\Hireables.cpp", 150, 0);
    memset(g_ppPetTypeIcons, 0, nAllocSize);
}

// 1.12a: 0x6FB6D980
// Calling convention: __userpurge (ebx = nPetType, edx = pPlayerUnit, esi = pLabelY, [esp+4] = pX, [esp+8] = pStaggerOffset)
int32_t __stdcall HIREABLES_DrawSingleSummonPortrait_Mode3(
    int32_t nPetType,
    D2UnitStrc* pPlayerUnit,
    int32_t* pLabelY,
    int32_t* pX,
    int32_t* pStaggerOffset)
{
    int32_t nPetGuid = -1;
    D2CellFileStrc* pCel = HIREABLES_GetOrLoadPetPortraitCellFile(nPetType, nullptr, pPlayerUnit, &nPetGuid);
    if (!pCel)
    {
        return 0;
    }
    HIREABLES_DrawPortraitLifeBar(nPetType, nPetGuid, *pX);
    HIREABLES_DrawPetTypeNameLabel(nPetType, *pX, *pLabelY);
    HIREABLES_AlternateLabelStagger(pLabelY, pStaggerOffset);

    D2GfxDataStrc tGfx = { 0 };
    tGfx.pCellFile = pCel;
    TEXTURE_CelDraw(&tGfx, *pX, 60, 0xFFFFFFFF, 1, nullptr);
    *pX += 56;
    return 1;
}

// 1.12a: 0x6FB6DA20
// Calling convention: __userpurge (eax = pPlayerUnit, ebx = nPetType, [esp+4] = pX, [esp+8] = pLabelY, [esp+12] = pStaggerOffset)
int32_t __stdcall HIREABLES_DrawStackedSummonPortrait_Mode2(
    D2UnitStrc* pPlayerUnit,
    int32_t nPetType,
    int32_t* pX,
    int32_t* pLabelY,
    int32_t* pStaggerOffset)
{
    int32_t nOwnerGuid = pPlayerUnit ? *(int32_t*)((uint8_t*)pPlayerUnit + 0x0C) : -1;
    int32_t nCount = 0;
    for (D2PetInfoStrc* pPet = g_pPetInfoList; pPet; pPet = pPet->pNext)
    {
        if (pPet->nPetType == nPetType && pPet->nOwnerGuid == nOwnerGuid)
        {
            ++nCount;
        }
    }
    if (nCount == 0)
    {
        return 0;
    }
    int32_t bDrawHp = 0;
    int32_t nPetGuid = -1;
    D2CellFileStrc* pCel = HIREABLES_GetOrLoadPetPortraitCellFile(nPetType, &bDrawHp, pPlayerUnit, &nPetGuid);
    if (!pCel)
    {
        return 0;
    }
    if (nCount == 1 && bDrawHp)
    {
        HIREABLES_DrawPortraitLifeBar(nPetType, nPetGuid, *pX);
    }
    HIREABLES_DrawSummonCount(nCount, *pX);
    HIREABLES_DrawPetTypeNameLabel(nPetType, *pX, *pLabelY);
    HIREABLES_AlternateLabelStagger(pLabelY, pStaggerOffset);

    D2GfxDataStrc tGfx = { 0 };
    tGfx.pCellFile = pCel;
    TEXTURE_CelDraw(&tGfx, *pX, 60, 0xFFFFFFFF, 1, nullptr);
    *pX += 56;
    return 1;
}

// 1.12a: 0x6FB6DB50
// Calling convention: __userpurge (ebx = nPetType, edx = pPlayerUnit, esi = pLabelY, [esp+4] = pX, [esp+8] = pStaggerOffset)
int32_t __stdcall HIREABLES_DrawSingleSummonPortrait_Mode1(
    int32_t nPetType,
    D2UnitStrc* pPlayerUnit,
    int32_t* pLabelY,
    int32_t* pX,
    int32_t* pStaggerOffset)
{
    return HIREABLES_DrawSingleSummonPortrait_Mode3(nPetType, pPlayerUnit, pLabelY, pX, pStaggerOffset);
}

// 1.12a: 0x6FB6DBF0
// Calling convention: __userpurge (edx = pPlayerUnit, [esp+4] = pX, [esp+8] = pLabelY, [esp+12] = pStaggerOffset)
int32_t __stdcall HIREABLES_DrawMercenaryPortrait(
    D2UnitStrc* pPlayerUnit,
    int32_t* pX,
    int32_t* pLabelY,
    int32_t* pStaggerOffset)
{
    int32_t nMercGuid = -1;
    D2CellFileStrc* pCel = HIREABLES_GetOrLoadPetPortraitCellFile(7, nullptr, pPlayerUnit, &nMercGuid);
    if (!pCel)
    {
        return 0;
    }
    HIREABLES_DrawPortraitLifeBar(7, nMercGuid, *pX);
    for (D2PetInfoStrc* pPet = g_pPetInfoList; pPet; pPet = pPet->pNext)
    {
        if (pPet->nPetType == 7 && pPet->nUnitGuid == nMercGuid && pPet->dwNameIdx != 0)
        {
            const Unicode* pMercName = D2LANG_GetStringFromTblIndex((uint16_t)pPet->dwNameIdx);
            if (pMercName)
            {
                Unicode wszName[32] = { 0 };
                for (int i = 0; i < 31 && pMercName[i].wChar != 0; ++i)
                {
                    wszName[i] = pMercName[i];
                }
                HIREABLES_DrawPortraitLabel(wszName, *pX, *pLabelY, 0);
            }
            break;
        }
    }
    HIREABLES_AlternateLabelStagger(pLabelY, pStaggerOffset);
    g_nMercPortraitX = *pX;
    g_nMercPortraitLabelY = *pLabelY;

    int32_t nDrawMode = g_bMercPortraitHighlight ? 7 : 1;
    D2GfxDataStrc tGfx = { 0 };
    tGfx.pCellFile = pCel;
    TEXTURE_CelDraw(&tGfx, *pX, 60, 0xFFFFFFFF, nDrawMode, nullptr);
    *pX += 56;
    return 1;
}

// 1.12a: 0x6FB6DD50
// Calling convention: __userpurge (eax = pPlayerUnit, ebx = pX, [esp+4] = pLabelY, [esp+8] = pStaggerOffset)
int32_t __stdcall HIREABLES_DrawPartyMemberPortraits(
    D2UnitStrc* pPlayerUnit,
    int32_t* pX,
    int32_t* pLabelY,
    int32_t* pStaggerOffset)
{
    if (!pPlayerUnit || g_nPortraitDisplayMode != 0 || !g_pRosterHead)
    {
        return 0;
    }
    int32_t nLocalGuid = *(int32_t*)((uint8_t*)pPlayerUnit + 0x0C);
    D2RosterUnitStrc* pLocalRoster = ROSTER_FindByUnitGuid(nLocalGuid);
    uint16_t wLocalParty = pLocalRoster ? pLocalRoster->wPartyId : 0xFFFF;
    if (wLocalParty == 0xFFFF)
    {
        return 0;
    }

    int32_t bDrewAny = 0;
    for (D2RosterUnitStrc* pCur = g_pRosterHead; pCur; pCur = pCur->pNext)
    {
        if (pCur->wPartyId == wLocalParty && pCur->dwUnitId != nLocalGuid && pCur->dwClassId <= 6)
        {
            static const int32_t s_aClassIconRemap[7] = { 0, 1, 4, 3, 2, 6, 5 };
            D2CellFileStrc* pCel = g_apClassIcons[s_aClassIconRemap[pCur->dwClassId]];
            if (pCel)
            {
                HIREABLES_DrawPortraitLifeBar(0, pCur->dwUnitId, *pX);
                HIREABLES_DrawAsciiPortraitLabel(pCur->szNameEx, *pX, *pLabelY);
                HIREABLES_AlternateLabelStagger(pLabelY, pStaggerOffset);

                D2GfxDataStrc tGfx = { 0 };
                tGfx.pCellFile = pCel;
                TEXTURE_CelDraw(&tGfx, *pX, 60, 0xFFFFFFFF, 1, nullptr);
                *pX += 56;
                bDrewAny = 1;
            }
        }
    }
    return bDrewAny;
}

// 1.12a: 0x6FB6DF80
// Calling convention: __stdcall
int32_t __stdcall HIREABLES_CanMercUseCursorItem(int32_t bPlayErrorSpeech)
{
    (void)bPlayErrorSpeech;
    if (!gbIsExpansion)
    {
        return 0;
    }
    D2UnitStrc* pItem = HIREABLES_GetLocalCursorItem();
    if (!pItem)
    {
        return 0;
    }
    // Check IDENTIFIED (0x10) and not BROKEN (0x100)
    if (!ITEMS_CheckItemFlag(pItem, 0x10, 955, "..\\Source\\D2Client\\UI\\Hireables.cpp") ||
        ITEMS_CheckItemFlag(pItem, 0x100, 955, "..\\Source\\D2Client\\UI\\Hireables.cpp"))
    {
        return 0;
    }
    // Potions: Healing (0x4C), Rejuvenation (0x50), Antidote/Thawing (0x51)
    if (ITEMS_CheckItemTypeId(pItem, 0x4C) ||
        ITEMS_CheckItemTypeId(pItem, 0x50) ||
        ITEMS_CheckItemTypeId(pItem, 0x51))
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB6E1E0
// Calling convention: __stdcall
void __stdcall HIREABLES_OnMouseMove(D2WindowMsgStrc* pMsg)
{
    if (!gbIsExpansion)
    {
        return;
    }
    pMsg->bHandled = 0;
    g_bMercTooltipReady = 0;
    if (!HIREABLES_IsMouseOverMercPortrait(pMsg))
    {
        g_bMercPortraitHighlight = 0;
        g_dwMercHoverStartTick = 0;
        return;
    }
    if (!g_dwMercHoverStartTick)
    {
        g_dwMercHoverStartTick = GetTickCount();
    }
    if (!HIREABLES_GetLocalCursorItem())
    {
        g_bMercTooltipReady = 1;
        if (GetTickCount() - g_dwMercHoverStartTick >= 700)
        {
            g_bMercPortraitHighlight = 1;
        }
    }
    else if (HIREABLES_CanMercUseCursorItem(0))
    {
        g_bMercPortraitHighlight = 1;
    }
}

// 1.12a: 0x6FB6E2E0
// Calling convention: __stdcall
void __stdcall HIREABLES_OnLeftMouseDown(D2WindowMsgStrc* pMsg)
{
    if (!gbIsExpansion)
    {
        return;
    }
    pMsg->bHandled = 0;
    if (!HIREABLES_IsMouseOverMercPortrait(pMsg))
    {
        g_dwMercHoverStartTick = 0;
        return;
    }
    if (HIREABLES_CanMercUseCursorItem(1))
    {
        uint8_t abPacket[3] = { 0x61, 0, 0 };
        MSG_SendPacket(abPacket, sizeof(abPacket));
        pMsg->bHandled = 1;
        pMsg->nReturnValue = 0;
        SMsgBreakHandlerChain(pMsg);
    }
}

// 1.12a: 0x6FB6E370
// Calling convention: __stdcall
void __stdcall HIREABLES_OnLeftMouseHold(D2WindowMsgStrc* pMsg)
{
    if (!gbIsExpansion)
    {
        return;
    }
    pMsg->bHandled = 0;
    if (!HIREABLES_IsMouseOverMercPortrait(pMsg))
    {
        g_dwMercHoverStartTick = 0;
        return;
    }
    if (!g_dwMercHoverStartTick)
    {
        g_dwMercHoverStartTick = GetTickCount();
    }
    if (GetTickCount() - g_dwMercHoverStartTick < 700)
    {
        g_dwMercHoverStartTick = 0;
        return;
    }
    pMsg->bHandled = 1;
    pMsg->nReturnValue = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB6E690
// Calling convention: __stdcall
void __stdcall HIREABLES_CyclePortraitDisplayMode()
{
    if (g_nPortraitDisplayMode == 0)
    {
        g_nPortraitDisplayMode = (gnGameType != 0) ? 1 : 2;
        g_dwMercHoverStartTick = 0;
    }
    else if (g_nPortraitDisplayMode == 1 && gnGameType != 0)
    {
        g_nPortraitDisplayMode = 2;
        g_dwMercHoverStartTick = 0;
    }
    else
    {
        g_nPortraitDisplayMode = 0;
    }
}

// 1.12a: 0x6FB6E880
// Calling convention: __stdcall
void __stdcall HIREABLES_UpdateMercScreenWindowState()
{
    if (g_nPortraitDisplayMode == 2)
    {
        g_dwMercHoverStartTick = 0;
        g_bMercTooltipReady = 0;
        g_bMercPortraitHighlight = 0;
    }
}

// 1.12a: 0x6FB6EAB0
// Calling convention: __stdcall
void __stdcall HIREABLES_DrawAllPortraits()
{
    int32_t nScreenPanels = gnViewportShiftMode;
    if (nScreenPanels == 2 || nScreenPanels == 3 || g_nPortraitDisplayMode == 2)
    {
        g_bMercPortraitHighlight = 0;
        g_bMercTooltipReady = 0;
        return;
    }

    D2UnitStrc* pPlayer = gpPlayerUnit;
    int32_t nX = 15;
    int32_t nLabelY = 72;
    int32_t nStaggerOffset = 0;

    int32_t bDrewAny = HIREABLES_DrawPartyMemberPortraits(pPlayer, &nX, &nLabelY, &nStaggerOffset);
    bDrewAny |= HIREABLES_DrawMercenaryPortrait(pPlayer, &nX, &nLabelY, &nStaggerOffset);

    int32_t nPetTypeCount = D2Client_GetPetTypeCount();
    for (int32_t nPetType = 0; nPetType < nPetTypeCount; ++nPetType)
    {
        const D2PetTypeTxt* pPetRec = static_cast<const D2PetTypeTxt*>(D2Client_GetPetTypeRecord(nPetType));
        if (!pPetRec) continue;
        switch (pPetRec->nIconType)
        {
        case 1:
            bDrewAny |= HIREABLES_DrawSingleSummonPortrait_Mode1(nPetType, pPlayer, &nLabelY, &nX, &nStaggerOffset);
            break;
        case 2:
            bDrewAny |= HIREABLES_DrawStackedSummonPortrait_Mode2(pPlayer, nPetType, &nX, &nLabelY, &nStaggerOffset);
            break;
        case 3:
            bDrewAny |= HIREABLES_DrawSingleSummonPortrait_Mode3(nPetType, pPlayer, &nLabelY, &nX, &nStaggerOffset);
            break;
        default:
            break;
        }
    }

    if (g_bPopupHirelingPending && gaPanelStates[D2PANEL_MERCINV] == 0)
    {
        SRegSaveValue("Diablo II", "PopupHireling", 0, 1);
        g_bPopupHirelingPending = 0;
    }

    if (g_bMercTooltipReady && g_bMercPortraitHighlight)
    {
        Unicode wszTooltip[256] = { 0 };
        Unicode* wszOpenMerc = D2LANG_GetStringFromTblIndex(0x58A2);
        Unicode* wszOrClick  = D2LANG_GetStringFromTblIndex(0x58A3);
        Unicode* wszMerc     = D2LANG_GetStringFromTblIndex(0x0F9E);
        Unicode_strcpy(wszTooltip, wszMerc);
        Unicode_strcat(wszTooltip, wszOpenMerc);
        Unicode_strcat(wszTooltip, wszOrClick);
        D2Win_10130_DrawFramedTextEx(wszTooltip, g_nMercPortraitX, g_nMercPortraitLabelY + 36, 0, 0);
    }
}
