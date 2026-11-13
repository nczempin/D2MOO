// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Translation Unit: UI_automap.cpp
// Source File : ..\Source\D2Client\UI\automap.cpp (plus preceding object/net/levelup UI)
// .text Range : 0x6FAEB410..0x6FAF0B70 (102 functions including 0x6FAEF6C0)
// ============================================================================

#include "D2ClientReconstructed.h"

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <D2DataTbls.h>
#include <DataTbls/LevelsTbls.h>
#include <DataTbls/MonsterTbls.h>
#include <DataTbls/ObjectsTbls.h>
#include <D2Dungeon.h>
#include <D2Collision.h>
#include <Units/Units.h>
#include <D2Gfx.h>
#include <Texture.h>
#include <D2CMP.h>
#include <D2WinFont.h>
#include <D2WinPalette.h>
#include <D2WinArchive.h>
#include <D2Unicode.h>
#include <D2Lang.h>

struct D2RoomExStrc;
struct D2StormMsgStrc;

// ============================================================================
// Automap Data Structures
// ============================================================================

struct D2AutomapCellStrc {
    int32_t fSaved;                // +0x00
    int16_t nCellNo;               // +0x04
    int16_t xPixel;                // +0x06
    int16_t yPixel;                // +0x08
    int16_t wWeight;               // +0x0A (AVL balance factor: -1, 0, +1)
    D2AutomapCellStrc* pLess;      // +0x0C (left child)
    D2AutomapCellStrc* pMore;      // +0x10 (right child)
};

struct D2AutomapCellPageStrc {
    D2AutomapCellStrc aCells[512]; // +0x0000 (512 * 20 = 0x2800 bytes)
    D2AutomapCellPageStrc* pNext;  // +0x2800
};

struct D2AutomapLayerStrc {
    int32_t nLayerNo;              // +0x00
    int32_t fSaved;                // +0x04 (town map type: 1=Act2, 2=Act4, 3=Act5)
    D2AutomapCellStrc* pFloors;    // +0x08
    D2AutomapCellStrc* pWalls;     // +0x0C
    D2AutomapCellStrc* pObjects;   // +0x10
    D2AutomapCellStrc* pExtras;    // +0x14
    D2AutomapLayerStrc* pNext;     // +0x18
};

struct D2AutomapDrawRectStrc {
    D2CellFileStrc* pCellFile;     // +0x00
    int32_t nLeft;                 // +0x04
    int32_t nRight;                // +0x08
    int32_t nTop;                  // +0x0C
    int32_t nBottom;               // +0x10
    int32_t nOffsetX;              // +0x14
    int32_t nOffsetY;              // +0x18
};

struct D2AutomapLayerFileRecordStrc {
    int32_t nLayerNo;              // +0x00
    int32_t nTownType;             // +0x04
    uint32_t dwSeed;               // +0x08
    int32_t nFloorsBytes;          // +0x0C
    int32_t nWallsBytes;           // +0x10
    int32_t nObjectsBytes;         // +0x14
    int32_t nExtrasBytes;          // +0x18
    void* pFloorsData;             // +0x1C
    void* pWallsData;              // +0x20
    void* pObjectsData;            // +0x24
    void* pExtrasData;             // +0x28
};

struct D2AutomapCelGroupInitStrc {
    int32_t nCellNo;               // +0x00
    int32_t nGroupId;              // +0x04
};

struct D2AutomapLineOffsetStrc {
    int32_t x1;                    // +0x00
    int32_t y1;                    // +0x04
    int32_t x2;                    // +0x08
    int32_t y2;                    // +0x0C
};

// ============================================================================
// Static Data Tables (.rdata / .data)
// ============================================================================

// 1.12a: 0x6FB831A4..0x6FB831D4
static const int32_t gAct1TownSkipCells[13] = {
    0, 10, 15, 16, 19, 20, 25, 30, 35, 36, 39, -1, 0
};

// 1.12a: 0x6FB831D8..0x6FB83238
static const D2AutomapLineOffsetStrc gAutomapCrosshairLines[6] = {
    {  0, -1,  2, -2 },
    {  4, -1,  2,  0 },
    {  4,  1,  2,  2 },
    {  0,  1, -2,  2 },
    { -4,  1, -2,  0 },
    { -4, -1, -2, -2 },
};

// 1.12a: 0x6FBA3A10..0x6FBA3B98
static const D2AutomapCelGroupInitStrc gAutomapCelGroupInitTable[49] = {
    {   0,  0 }, {   1,  0 }, {   2,  0 }, {   3,  0 },
    {   6,  1 }, {   7,  1 }, {   8,  1 },
    {  11,  2 }, {  12,  2 },
    {  13,  3 }, {  14,  3 },
    {  20,  4 }, {  38,  4 },
    {  21,  5 }, {  39,  5 },
    {  46,  6 }, {  47,  6 }, {  48,  6 }, {  49,  6 },
    {  51,  7 }, {  52,  7 }, {  53,  7 }, {  54,  7 },
    {  60,  8 }, {  70,  8 },
    {  61,  9 }, {  71,  9 },
    { 120, 10 }, { 169, 10 }, { 171, 10 },
    { 121, 11 }, { 170, 11 }, { 172, 11 },
    { 257, 12 }, { 258, 12 }, { 259, 12 },
    { 266, 13 }, { 267, 13 },
    { 337, 14 }, { 338, 14 },
    { 472, 15 }, { 473, 15 }, { 474, 15 }, { 475, 15 },
    { 520, 16 }, { 521, 16 }, { 522, 16 },
    { 533, 17 }, { 534, 17 },
};

// ============================================================================
// Global Variables
// ============================================================================

extern int32_t gnScreenWidth;                     // 0x6FB8C6E0
extern int32_t gnScreenHeight;                    // 0x6FB8C6E4
extern int32_t gnViewportWidth;                   // 0x6FBCD528
extern int32_t gnViewportHeight;                  // 0x6FBCD524
extern int32_t gnViewportSplitMode;               // 0x6FBCC1D0
extern int32_t gnViewportShiftX;                  // 0x6FBCC1D4
extern int32_t gnMouseX;                          // 0x6FBB1638
extern int32_t gnMouseY;                          // 0x6FBB1634
extern D2UnitStrc* gpPlayerUnit;                  // 0x6FBCC3D0
extern D2RosterUnitStrc* gpRosterUnitList;        // 0x6FBCC080

extern int32_t gnAutomapDivisor;                  // 0x6FBA3B98 (10 = full, 20 = mini)
extern int32_t gbAutomapCenters;                  // 0x6FBA3B9C
extern int32_t gbAutomapShowParty;                // 0x6FBA3BA0
extern int32_t gbAutomapShowPartyNames;           // 0x6FBA3BA4
extern int32_t gaAutomapCelGroups[2048];          // 0x6FBC2798
extern int32_t gnAutomapMode;                     // 0x6FBCC2A0 (0 = full, 1 = mini)
extern int16_t* gpAutomapSerializeCursor;         // 0x6FBCC2A4
extern D2AutomapCellPageStrc* gpAutomapCellPages; // 0x6FBCC2A8
extern int32_t gnAutomapAllocatedCells;           // 0x6FBCC2AC
extern D2AutomapLayerStrc* gpAutomapLayerList;    // 0x6FBCC2B0
extern D2AutomapLayerStrc* gpCurrentAutomapLayer; // 0x6FBCC2B4
extern D2CellFileStrc* gpAutomapSprites[4];       // 0x6FBCC2B8 (MaxiMap, Act2Map, Act4Map, ExTnMap)
extern int32_t gnAutomapSpriteCels[4];            // 0x6FBCC2C8
extern int32_t gnAutomapPanX;                     // 0x6FBCC2D8
extern int32_t gnAutomapPanY;                     // 0x6FBCC2DC
extern int32_t gnAutomapBaseX;                    // 0x6FBCC2E0
extern int32_t gnAutomapBaseY;                    // 0x6FBCC2E4
extern int32_t gnAutomapOffsetX;                  // 0x6FBCC2E8
extern int32_t gnAutomapOffsetY;                  // 0x6FBCC2EC
extern int32_t gbAutomapRevealAll;                // 0x6FBCC2F0
extern int32_t gnAutomapRevealDelayTicks;         // 0x6FBCC2F4
extern int32_t gnAutomapFadeMode;                 // 0x6FBCC2F8
extern int32_t gnAutomapSavedFadeMode;            // 0x6FBCC2FC
extern uint8_t gAutomapColors[11];                // 0x6FBCC300..0x6FBCC30A
extern int32_t gnAutomapTextY;                    // 0x6FBCC30C
extern int32_t gnAutomapBoundsMinX;               // 0x6FBCC318
extern int32_t gnAutomapBoundsMinY;               // 0x6FBCC31C
extern int32_t gnAutomapBoundsMaxX;               // 0x6FBCC320
extern int32_t gnAutomapBoundsMaxY;               // 0x6FBCC324
extern int32_t gnAutomapMiniTopY;                 // 0x6FBCC328
extern int32_t gnAutomapMiniLeftX;                // 0x6FBCC32C
extern int32_t gbAutomapLeft;                     // 0x6FBCC330
extern int32_t gbAutomapLeftSaved1;               // 0x6FBCC334
extern int32_t gbAutomapLeftSaved2;               // 0x6FBCC338

extern D2CellFileStrc* gpLevelUpBtnSprite;        // 0x6FBCC33C ('Panel\Level')
extern D2CellFileStrc* gpLevelUpSocketSprite;     // 0x6FBCC340 ('Panel\Levelsocket')
extern int32_t gbNewStatBtnPressed;               // 0x6FBCC344
extern int32_t gbNewSkillBtnPressed;              // 0x6FBCC348
extern uint32_t gdwScreenFadeEndTick;             // 0x6FBCC34C

// ============================================================================
// Preceding Object Operating Handlers (0x6FAEB410..0x6FAEB8D0)
// ============================================================================

// 1.12a: 0x6FAEB410 (__fastcall)
void __fastcall OBJECTS_SpawnFireRingA(D2UnitStrc* pObject, D2UnitStrc* pPlayer, int32_t nUnused)
{
    (void)nUnused;
    // Spawns 6 client missiles (missile ID 0x520) in a ring around pObject using stat 0x30 (STAT_FIRESKILL / firemindam)
}

// 1.12a: 0x6FAEB560 (__fastcall)
void __fastcall OBJECTS_SpawnFireRingB(D2UnitStrc* pObject, D2UnitStrc* pPlayer, int32_t nUnused)
{
    (void)nUnused;
    // Spawns 6 client missiles (missile ID 0x520) in a ring around pObject using stat 0x2D
}

// 1.12a: 0x6FAEB6B0 (__fastcall)
void __fastcall OBJECTS_SpawnShrineNovaGrid(D2UnitStrc* pObject, D2UnitStrc* pPlayer, int32_t nUnused)
{
    (void)nUnused;
    // Spawns a 4x4 grid of client missiles (missile ID 3) around pObject using stat 0x3E
}

// 1.12a: 0x6FAEB840 (__fastcall)
uint8_t __fastcall OBJECTS_TriggerWaypointMenuAtUnit(D2UnitStrc* pObject)
{
    // Triggers waypoint/portal coordinate handler at pObject's position if 0x6FBCD517 is set
    (void)pObject;
    return 1;
}

// 1.12a: 0x6FAEB8D0 (__fastcall)
uint8_t __fastcall OBJECTS_OperateAncientStatue(D2UnitStrc* pObject)
{
    // Checks Act 5 Ancients quest state (quest 0x27) and distance < 25, sends C2S interact packet, sets anim mode 2
    (void)pObject;
    return 1;
}

// ============================================================================
// Preceding Game/Network Session Init & Shutdown (0x6FAEB9E0..0x6FAEBBF0)
// ============================================================================

extern int32_t gaPanelStates[38];
static int32_t g_bSinglePlayerServerRunning = 0;
static int32_t g_bConnectionTimedOut = 0;

// 1.12a: 0x6FAEB9E0 (__stdcall)
int32_t __stdcall NET_HasConnectionTimedOut(void)
{
    return g_bConnectionTimedOut;
}

// 1.12a: 0x6FAEB9F0 (__stdcall)
int32_t __stdcall GAME_LeaveSinglePlayerServer(void)
{
    if (!g_bSinglePlayerServerRunning) {
        return 0;
    }
    g_bSinglePlayerServerRunning = 0;
    // Tail-calls D2Game_10050 (#10036)
    return 1;
}

// 1.12a: 0x6FAEBA10 (__stdcall)
int32_t __stdcall GAME_StepSinglePlayerServer(void)
{
    g_bSinglePlayerServerRunning = 1;
    // Tail-calls D2Game_10046 (#10046)
    return 1;
}

// 1.12a: 0x6FAEBA20 (__stdcall)
int32_t __stdcall NET_WaitForConnectStep(int32_t nUnused)
{
    (void)nUnused;
    // Polls WINDOW_GetState and D2NET_10017; sets 0x6FBC47A0 = 1 on disconnect
    return 1;
}

// 1.12a: 0x6FAEBA80 (custom: eax = szServerIp, ecx = nMode)
int32_t __fastcall NET_ConnectClientToServer(int32_t nMode, const char* szServerIp)
{
    (void)nMode;
    (void)szServerIp;
    // Calls CLIENT_Initialize(nMode, szServerIp) and runs modal wait loop with NET_WaitForConnectStep
    return 1;
}

// 1.12a: 0x6FAEBAA0 (__stdcall)
int32_t __stdcall NET_WaitForDisconnectStep(int32_t nUnused)
{
    (void)nUnused;
    // Processes remaining network messages for up to 10,000 ms (0x2710) or until 0x6FBCC000 == 0
    return 1;
}

// 1.12a: 0x6FAEBB10 (__stdcall)
int32_t __stdcall NET_WaitForInitialHandshakeStep(int32_t nUnused)
{
    (void)nUnused;
    // Waits up to 10,000 ms for D2NET_10025 and CLIENT_DequeueSystemPacket until 0x6FBCC000 != 0
    return 1;
}

// 1.12a: 0x6FAEBBB0 (__stdcall)
void __stdcall NET_ShutdownClientAndServer(void)
{
    // Runs NET_WaitForDisconnectStep modal loop, calls CLIENT_Release and SERVER_Release
}

// 1.12a: 0x6FAEBBF0 (__stdcall)
void __stdcall NET_InitializeSessionByGameType(void)
{
    // Initializes SERVER_Initialize, CLIENT_Initialize, and GAME_InitGameDataTable based on game type (0x6FBCBFF8)
}

// ============================================================================
// Preceding Screen Fade & Level-Up "+" Buttons (0x6FAEBCF0..0x6FAECDE0)
// ============================================================================

// 1.12a: 0x6FAEBCF0 (__stdcall)
void __stdcall UI_ResetScreenFadeTimer(void)
{
    gdwScreenFadeEndTick = 0;
}

// 1.12a: 0x6FAEBD00 (__stdcall)
void __stdcall UI_StartScreenFadeTimer(void)
{
    gdwScreenFadeEndTick = GetTickCount() + 500;
}

// 1.12a: 0x6FAEBD20 (__stdcall)
void __stdcall UI_DrawScreenFadeBox(void)
{
    if (!gdwScreenFadeEndTick) {
        return;
    }
    uint32_t dwNow = GetTickCount();
    if (dwNow > gdwScreenFadeEndTick) {
        gdwScreenFadeEndTick = 0;
        return;
    }
    // Draws alpha fade box over viewport using D2GFX_DrawBoxAlpha
}

// 1.12a: 0x6FAEBDB0 (custom: eax = nSplitMode, ecx = nMouseX, edi = nMouseY)
int32_t __fastcall UI_HitTestNewSkillBtn640(int32_t nMouseX, int32_t nSplitMode, int32_t nMouseY)
{
    int32_t nBaseX = (nSplitMode == 1) ? (gnScreenWidth - gnScreenWidth / 2) : gnScreenWidth;
    if (nMouseX > nBaseX - 0x49 && nMouseX < nBaseX - 0x28 &&
        nMouseY > gnScreenHeight - 0x8A && nMouseY < gnScreenHeight - 0x66) {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FAEBE00 (custom: eax = nSplitMode, ecx = nMouseX, esi = nMouseY)
int32_t __fastcall UI_HitTestNewStatBtn640(int32_t nMouseX, int32_t nSplitMode, int32_t nMouseY)
{
    if (nSplitMode == 2) {
        int32_t nHalfW = gnScreenWidth / 2;
        if (nMouseX <= nHalfW + 0x28 || nMouseX >= nHalfW + 0x4A) {
            return 0;
        }
    } else {
        if (nMouseX <= 0x28 || nMouseX >= 0x4A) {
            return 0;
        }
    }
    if (nMouseY > gnScreenHeight - 0x8B && nMouseY < gnScreenHeight - 0x66) {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FAEBE60 (custom: eax = nSplitMode)
uint8_t __fastcall UI_IsRightSideBlockedForNewSkillBtn(int32_t nSplitMode)
{
    if (nSplitMode == 3 || gaPanelStates[4] || gaPanelStates[12] ||
        (gaPanelStates[22] && gaPanelStates[1])) {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FAEBE90 (custom: eax = nSplitMode)
uint8_t __fastcall UI_IsLeftSideBlockedForNewStatBtn(int32_t nSplitMode)
{
    if (nSplitMode == 3 || gaPanelStates[2]) {
        return 1;
    }
    if (gaPanelStates[22] && (gaPanelStates[1] || gaPanelStates[4])) {
        return 1;
    }
    return gaPanelStates[12] != 0;
}

// 1.12a: 0x6FAEBED0 (__stdcall)
int32_t __stdcall UI_HitTestNewSkillBtn800(void)
{
    if (gnMouseX > 0x233 && gnMouseX < 0x255 &&
        gnMouseY > gnScreenHeight - 0x2A && gnMouseY < gnScreenHeight - 8) {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FAEBF10 (__stdcall)
int32_t __stdcall UI_HitTestNewStatBtn800(void)
{
    if (gnMouseX > 0xCE && gnMouseX < 0xF0 &&
        gnMouseY > gnScreenHeight - 0x2A && gnMouseY < gnScreenHeight - 8) {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FAEBF50 (__stdcall)
void __stdcall UI_FreeLevelUpButtonSprites(void)
{
    // Frees gpLevelUpBtnSprite (0x6FBCC33C) and gpLevelUpSocketSprite (0x6FBCC340)
}

// 1.12a: 0x6FAEBFB0 (__stdcall)
void __stdcall UI_DrawNewSkillButton(void)
{
    // Draws "New Skills" (0xF93) button and socket at 640x480 or 800x600 HUD position
}

// 1.12a: 0x6FAEC1F0 (__stdcall)
void __stdcall UI_DrawDisabledNewSkillButton800(void)
{
    // Draws disabled frame 2 of gpLevelUpBtnSprite at X=0x233 in 800x600 mode
}

// 1.12a: 0x6FAEC2A0 (__stdcall)
void __stdcall UI_DrawNewStatButton(void)
{
    // Draws "New Stats" (0xF92) button and socket at 640x480 or 800x600 HUD position
}

// 1.12a: 0x6FAEC4E0 (__stdcall)
void __stdcall UI_DrawDisabledNewStatButton800(void)
{
    // Draws disabled frame 2 of gpLevelUpBtnSprite at X=0xCE in 800x600 mode
}

// 1.12a: 0x6FAEC590 (__stdcall)
void __stdcall UI_OnNewSkillButtonDown(D2StormMsgStrc* pMsg)
{
    (void)pMsg;
    // Handles LButtonDown on New Skill button, sets gbNewSkillBtnPressed = 1, plays click sound 4
}

// 1.12a: 0x6FAEC660 (__stdcall)
void __stdcall UI_OnNewStatButtonDown(D2StormMsgStrc* pMsg)
{
    (void)pMsg;
    // Handles LButtonDown on New Stat button, sets gbNewStatBtnPressed = 1, plays click sound 4
}

// 1.12a: 0x6FAEC740 (__stdcall)
void __stdcall UI_LoadLevelUpButtonSprites(void)
{
    // Loads 'Panel\Level' into gpLevelUpBtnSprite and 'Panel\Levelsocket' into gpLevelUpSocketSprite
}

// 1.12a: 0x6FAEC770 (__stdcall)
void __stdcall UI_OnNewSkillButtonUp(D2StormMsgStrc* pMsg)
{
    (void)pMsg;
    // Handles LButtonUp on New Skill button, opens Skill Tree panel (UI panel 4)
}

// 1.12a: 0x6FAECAD0 (__stdcall)
void __stdcall UI_OnNewStatButtonUp(D2StormMsgStrc* pMsg)
{
    (void)pMsg;
    // Handles LButtonUp on New Stat button, opens Character Stats panel (UI panel 2)
}

// 1.12a: 0x6FAECDE0 (__stdcall)
void __stdcall UI_UpdateLevelUpButtonsState(void)
{
    // Checks STAT_STATPTS (4) and STAT_NEWSSKILLS (5) on gpPlayerUnit and toggles panels 6 & 7
}

// ============================================================================
// Automap Subsystem (0x6FAECFC0..0x6FAF0B69)
// ============================================================================

extern "C" {
    int32_t __stdcall SRegLoadValue(const char* szSection, const char* szKey, uint8_t nFlags, int32_t* pValue);
    int32_t __stdcall SRegSaveValue(const char* szSection, const char* szKey, uint8_t nFlags, int32_t nValue);
}

extern D2UnitStrc* __fastcall D2Client_GetLocalMercUnit(D2UnitStrc* pPlayer);
extern const Unicode* __fastcall D2Client_GetNpcDisplayName(D2UnitStrc* pNpc);

// Forward declarations within UI_automap.cpp
void __stdcall AUTOMAP_SaveCurrentLayer(void);
void __stdcall AUTOMAP_FreeAllCells(void);
void __stdcall AUTOMAP_LoadCurrentLayerFromFile(void);
void __fastcall AUTOMAP_CenterView(int32_t bForceCenter);
int32_t __fastcall AUTOMAP_InsertCell(D2AutomapCellStrc* pCell, D2AutomapCellStrc** ppRoot);

// 1.12a: 0x6FAECFC0 (__stdcall)
int32_t __stdcall AUTOMAP_GetViewMode(void)
{
    return gnAutomapMode;
}

// 1.12a: 0x6FAECFD0 (__stdcall)
void __stdcall AUTOMAP_InitColors(void)
{
    gAutomapColors[0]  = D2Win_10034_MixRGB(0x00, 0x00, 0xFF); // Blue (Local player)
    gAutomapColors[1]  = D2Win_10034_MixRGB(0xFF, 0x00, 0x00); // Red (Hostile / Boss)
    gAutomapColors[2]  = D2Win_10034_MixRGB(0xFF, 0x00, 0xFF); // Magenta (Corpse)
    gAutomapColors[3]  = D2Win_10034_MixRGB(0x00, 0xFF, 0x00); // Green (Party member)
    gAutomapColors[4]  = D2Win_10034_MixRGB(0x44, 0x70, 0x74); // Teal (Minion / Summon)
    gAutomapColors[5]  = D2Win_10034_MixRGB(0x48, 0xA0, 0x34); // Dark Green (Hireling / Neutral)
    gAutomapColors[6]  = D2Win_10034_MixRGB(0xF4, 0xF4, 0xF4); // White (NPC)
    gAutomapColors[7]  = D2Win_10034_MixRGB(0x80, 0x80, 0x80); // Grey
    gAutomapColors[8]  = D2Win_10034_MixRGB(0xF4, 0xF4, 0x00); // Yellow (Portal)
    gAutomapColors[9]  = D2Win_10034_MixRGB(0x00, 0x00, 0xFF); // Blue
    gAutomapColors[10] = D2Win_10034_MixRGB(0x00, 0xFF, 0x00); // Green
}

// 1.12a: 0x6FAED0C0 (custom: eax = nY, ecx = nX, stack = nColor)
void __fastcall AUTOMAP_DrawCrosshair(int32_t nX, int32_t nY, uint8_t nColor)
{
    if (gnAutomapMode == 1) {
        nX -= 1;
        nY += 5;
    }
    for (int i = 0; i < 6; ++i) {
        D2GFX_DrawLine(
            nX + gAutomapCrosshairLines[i].x1 * 2,
            nY + gAutomapCrosshairLines[i].y1 * 2,
            nX + gAutomapCrosshairLines[i].x2 * 2,
            nY + gAutomapCrosshairLines[i].y2 * 2,
            nColor,
            0xFF
        );
    }
}

// 1.12a: 0x6FAED130 (__stdcall)
void __stdcall AUTOMAP_ResetRevealDelay(void)
{
    gnAutomapRevealDelayTicks = 2;
}

// 1.12a: 0x6FAED140 (__stdcall)
D2AutomapLayerStrc* __stdcall AUTOMAP_AllocLayer(int32_t nLayerNo)
{
    D2AutomapLayerStrc* pLayer = (D2AutomapLayerStrc*)FOG_Alloc(
        sizeof(D2AutomapLayerStrc), "..\\Source\\D2Client\\UI\\automap.cpp", 0x5ED, 0);
    memset(pLayer, 0, sizeof(D2AutomapLayerStrc));
    pLayer->nLayerNo = nLayerNo;
    pLayer->pNext = gpAutomapLayerList;
    gpAutomapLayerList = pLayer;
    return pLayer;
}

// 1.12a: 0x6FAED190 (custom: esi = hFile, edi = pLayerOffsets, stack = nPrevNodeOffset)
void __stdcall AUTOMAP_TruncateCorruptMapFile(HANDLE hFile, uint32_t* pLayerOffsets, uint32_t nPrevNodeOffset)
{
    (void)hFile;
    if (nPrevNodeOffset != 0) {
        // Seek to nPrevNodeOffset and write 0 as next pointer
    } else if (gpCurrentAutomapLayer) {
        pLayerOffsets[gpCurrentAutomapLayer->nLayerNo] = 0;
        // Seek to 0 and rewrite 100-entry (0x190 byte) layer offset header
    }
}

// 1.12a: 0x6FAED200 (__stdcall)
void __stdcall AUTOMAP_FreeAllCells(void)
{
    if (!gpCurrentAutomapLayer) {
        return;
    }
    D2AutomapCellPageStrc* pPage = gpAutomapCellPages;
    while (pPage) {
        D2AutomapCellPageStrc* pNext = pPage->pNext;
        FOG_Free(pPage, "..\\Source\\D2Client\\UI\\automap.cpp", 0x51E, 0);
        pPage = pNext;
    }
    gpAutomapCellPages = nullptr;
    gnAutomapAllocatedCells = 0;
    gpCurrentAutomapLayer->pFloors = nullptr;
    gpCurrentAutomapLayer->pWalls = nullptr;
    gpCurrentAutomapLayer->pObjects = nullptr;
    gpCurrentAutomapLayer->pExtras = nullptr;
}

// 1.12a: 0x6FAED260 (__fastcall)
void __fastcall AUTOMAP_SerializeCellTree(D2AutomapCellStrc* pNode)
{
    while (pNode) {
        AUTOMAP_SerializeCellTree(pNode->pLess);
        if (pNode->fSaved == 0) {
            int16_t* pOut = gpAutomapSerializeCursor;
            pOut[0] = pNode->nCellNo;
            pOut[1] = pNode->xPixel;
            pOut[2] = pNode->yPixel;
            gpAutomapSerializeCursor = pOut + 3;
        }
        pNode = pNode->pMore;
    }
}

// 1.12a: 0x6FAED2B0 (__fastcall)
void __fastcall AUTOMAP_CountSavedCellsInTree(D2AutomapCellStrc* pNode, int32_t* pByteCount)
{
    while (pNode) {
        AUTOMAP_CountSavedCellsInTree(pNode->pLess, pByteCount);
        if (pNode->fSaved == 0) {
            *pByteCount += 6;
        }
        pNode = pNode->pMore;
    }
}

// 1.12a: 0x6FAED2E0 (__stdcall)
HANDLE __stdcall AUTOMAP_OpenMapSaveFile(void)
{
    // Opens <SavePath>\<CharName>.map, checks 24-byte header (version == 0x0C and act seed),
    // rotates/deletes .ma0..ma3 when act seed is new, and opens <SavePath>\<CharName>.ma<idx>
    return INVALID_HANDLE_VALUE;
}

// 1.12a: 0x6FAED5B0 (__stdcall)
D2AutomapCellStrc* __stdcall AUTOMAP_NewCell(void)
{
    int32_t nIndex = gnAutomapAllocatedCells;
    int32_t nPageIdx = nIndex / 512;
    int32_t nCellIdx = nIndex % 512;
    gnAutomapAllocatedCells = nIndex + 1;

    D2AutomapCellPageStrc* pPrev = nullptr;
    D2AutomapCellPageStrc* pPage = gpAutomapCellPages;
    int32_t i = 0;
    while (pPage && i < nPageIdx) {
        pPrev = pPage;
        pPage = pPage->pNext;
        ++i;
    }
    if (!pPage) {
        pPage = (D2AutomapCellPageStrc*)FOG_Alloc(
            sizeof(D2AutomapCellPageStrc), "..\\Source\\D2Client\\UI\\automap.cpp", 0x35D, 0);
        memset(pPage, 0, sizeof(D2AutomapCellPageStrc));
        if (pPrev) {
            pPrev->pNext = pPage;
        } else {
            gpAutomapCellPages = pPage;
        }
    }

    D2AutomapCellStrc* pCell = &pPage->aCells[nCellIdx];
    memset(pCell, 0, sizeof(D2AutomapCellStrc));
    return pCell;
}

// 1.12a: 0x6FAED660 (__fastcall)
int32_t __fastcall AUTOMAP_CompareCells(const D2AutomapCellStrc* pA, const D2AutomapCellStrc* pB)
{
    int32_t nDiffY = (int32_t)pA->yPixel - (int32_t)pB->yPixel;
    if (nDiffY != 0) {
        return nDiffY;
    }
    int32_t nDiffX = (int32_t)pA->xPixel - (int32_t)pB->xPixel;
    if (nDiffX != 0) {
        return nDiffX;
    }
    int32_t nGroupA = gaAutomapCelGroups[pA->nCellNo];
    if (nGroupA != -1) {
        int32_t nGroupB = gaAutomapCelGroups[pB->nCellNo];
        if (nGroupA == nGroupB) {
            return 0;
        }
        return (int32_t)pA->nCellNo - (int32_t)pB->nCellNo;
    }
    return 0;
}

// 1.12a: 0x6FAED6A0 (custom: edx = ppNode)
void __fastcall AUTOMAP_RotateRight(int32_t nUnused, D2AutomapCellStrc** ppNode)
{
    (void)nUnused;
    D2AutomapCellStrc* pRoot = *ppNode;
    D2AutomapCellStrc* pLeft = pRoot->pLess;
    pRoot->pLess = pLeft->pMore;
    pLeft->pMore = pRoot;
    *ppNode = pLeft;
}

// 1.12a: 0x6FAED6F0 (custom: edx = ppNode)
void __fastcall AUTOMAP_RotateLeft(int32_t nUnused, D2AutomapCellStrc** ppNode)
{
    (void)nUnused;
    D2AutomapCellStrc* pRoot = *ppNode;
    D2AutomapCellStrc* pRight = pRoot->pMore;
    pRoot->pMore = pRight->pLess;
    pRight->pLess = pRoot;
    *ppNode = pRight;
}

// 1.12a: 0x6FAED740 (custom: esi = pX, ecx = pY)
void __fastcall AUTOMAP_ClientToAutomapCoords(int32_t* pY, int32_t* pX)
{
    *pX = (*pX / gnAutomapDivisor) + 8 - gnAutomapOffsetX;
    *pY = (*pY / gnAutomapDivisor) - 8 - gnAutomapOffsetY;
}

// 1.12a: 0x6FAED780 (custom: ecx = pX, eax = pY)
void __fastcall AUTOMAP_AdjustAutomapCoords(int32_t* pX, int32_t* pY)
{
    *pX -= gnAutomapOffsetX;
    *pY -= gnAutomapOffsetY;
}

// 1.12a: 0x6FAED7A0 (custom: esi = bEnable)
int32_t __fastcall AUTOMAP_SetShowPartyNames(int32_t bEnable)
{
    SRegSaveValue("Diablo II", "AutoMap Party Names", 0, bEnable);
    gbAutomapShowPartyNames = bEnable;
    return bEnable;
}

// 1.12a: 0x6FAED7C0 (__stdcall)
int32_t __stdcall AUTOMAP_GetShowPartyNames(void)
{
    return gbAutomapShowPartyNames;
}

// 1.12a: 0x6FAED7D0 (custom: esi = bEnable)
int32_t __fastcall AUTOMAP_SetShowParty(int32_t bEnable)
{
    SRegSaveValue("Diablo II", "AutoMap Party", 0, bEnable);
    gbAutomapShowParty = bEnable;
    return bEnable;
}

// 1.12a: 0x6FAED7F0 (__stdcall)
int32_t __stdcall AUTOMAP_GetShowParty(void)
{
    return gbAutomapShowParty;
}

// 1.12a: 0x6FAED800 (custom: esi = bLeft)
int32_t __fastcall AUTOMAP_SetLeftMinimap(int32_t bLeft)
{
    SRegSaveValue("Diablo II", "AutoMap Left", 0, bLeft);
    gbAutomapLeftSaved1 = bLeft;
    gbAutomapLeftSaved2 = bLeft;
    gbAutomapLeft = bLeft;
    return bLeft;
}

// 1.12a: 0x6FAED830 (__stdcall)
int32_t __stdcall AUTOMAP_GetLeftMinimap(void)
{
    return gbAutomapLeft;
}

// 1.12a: 0x6FAED840 (custom: esi = bCenter)
int32_t __fastcall AUTOMAP_SetCenterMode(int32_t bCenter)
{
    SRegSaveValue("Diablo II", "AutoMap Centers", 0, bCenter);
    gbAutomapCenters = bCenter;
    return bCenter;
}

// 1.12a: 0x6FAED860 (__stdcall)
int32_t __stdcall AUTOMAP_GetCenterMode(void)
{
    return gbAutomapCenters;
}

// 1.12a: 0x6FAED870 (custom: eax = nFadeMode)
int32_t __fastcall AUTOMAP_SetFadeMode(int32_t nFadeMode)
{
    if (gnAutomapMode == 1 && nFadeMode == 1) {
        gnAutomapFadeMode = 1;
        nFadeMode = (gnAutomapSavedFadeMode == 0) ? 2 : 0;
    }
    SRegSaveValue("Diablo II", "AutoMapFade", 0, nFadeMode);
    gnAutomapSavedFadeMode = nFadeMode;
    gnAutomapFadeMode = nFadeMode;
    return nFadeMode;
}

// 1.12a: 0x6FAED8C0 (__stdcall)
int32_t __stdcall AUTOMAP_GetFadeMode(void)
{
    int32_t nMode = gnAutomapFadeMode;
    if (gnAutomapMode == 1 && nMode == 1) {
        nMode = (gnAutomapSavedFadeMode == 0) ? 2 : 0;
        gnAutomapFadeMode = nMode;
    }
    return nMode;
}

// 1.12a: 0x6FAED8F0 (__stdcall)
void __stdcall AUTOMAP_NoOp(int32_t nUnused)
{
    (void)nUnused;
}

// 1.12a: 0x6FAED900 (__stdcall)
void __stdcall AUTOMAP_PanRight(int32_t nUnused)
{
    (void)nUnused;
    gnAutomapPanX += 16;
}

// 1.12a: 0x6FAED910 (__stdcall)
void __stdcall AUTOMAP_PanLeft(int32_t nUnused)
{
    (void)nUnused;
    gnAutomapPanX -= 16;
}

// 1.12a: 0x6FAED920 (__stdcall)
void __stdcall AUTOMAP_PanUp(int32_t nUnused)
{
    (void)nUnused;
    gnAutomapPanY -= 16;
}

// 1.12a: 0x6FAED930 (__stdcall)
void __stdcall AUTOMAP_PanDown(int32_t nUnused)
{
    (void)nUnused;
    gnAutomapPanY += 16;
}

// 1.12a: 0x6FAED940 (__stdcall)
void __stdcall AUTOMAP_FreeSprites(void)
{
    for (int i = 0; i < 4; ++i) {
        if (gpAutomapSprites[i]) {
            ARCHIVE_FreeCellFile(gpAutomapSprites[i]);
            gpAutomapSprites[i] = nullptr;
            gnAutomapSpriteCels[i] = 0;
        }
    }
}

static void AUTOMAP_DrawRightAlignedUnicodeText(const Unicode* wszText, int32_t nColor, int32_t nFont, int32_t nLineAdvance)
{
    if (!wszText || !wszText[0]) return;
    D2Win_10127_SetFont(nFont);
    const int32_t nTextW = D2Win_10121_GetTextWidth(wszText);
    const int32_t nRightX = (gnAutomapBoundsMaxX > 0) ? (gnAutomapBoundsMaxX - 16) : 784;
    D2Win_10117_DrawText(wszText, nRightX - nTextW, gnAutomapTextY, nColor, 0);
    gnAutomapTextY += nLineAdvance;
}

// 1.12a: 0x6FAED980 (__stdcall)
void __stdcall AUTOMAP_DrawDifficultyText(void)
{
    const char* szDiff = "Difficulty: Normal";
    if (gnClientDifficulty == 1) szDiff = "Difficulty: Nightmare";
    else if (gnClientDifficulty >= 2) szDiff = "Difficulty: Hell";
    Unicode wszDiff[64] = {};
    Unicode::win2Unicode(wszDiff, szDiff, 63);
    AUTOMAP_DrawRightAlignedUnicodeText(wszDiff, 4, D2FONT_FONTFORMAL11, 14);
}

// 1.12a: 0x6FAEDA70 (__stdcall)
void __stdcall AUTOMAP_DrawHostIpText(void)
{
}

// 1.12a: 0x6FAEDAF0 (__stdcall)
void __stdcall AUTOMAP_DrawVersionAndTimeText(void)
{
}

// 1.12a: 0x6FAEDBD0 (__stdcall)
void __stdcall AUTOMAP_DrawExpansionText(void)
{
    if (!gbIsExpansion) return;
    const Unicode* wszExp = D2LANG_GetStringFromTblIndex(0x58CA);
    Unicode wszFallback[64] = {};
    if (!wszExp || !wszExp[0]) {
        Unicode::win2Unicode(wszFallback, "Expansion Character", 63);
        wszExp = wszFallback;
    }
    AUTOMAP_DrawRightAlignedUnicodeText(wszExp, 2, D2FONT_FONTFORMAL11, 14);
}

// 1.12a: 0x6FAEDC40 (__stdcall)
void __stdcall AUTOMAP_DrawGameNameAndPassText(void)
{
}

// 1.12a: 0x6FAEDE00 (custom: ebx = szLabel, stack = nX, nY, nTextColor)
void __stdcall AUTOMAP_DrawMarkerLabel(const char* szLabel, int32_t nX, int32_t nY, uint8_t nTextColor)
{
    if (!szLabel || !szLabel[0]) return;
    Unicode wszLabel[32] = {};
    Unicode::win2Unicode(wszLabel, szLabel, 24);
    D2Win_10127_SetFont(D2FONT_FONT6);
    const int32_t nW = D2Win_10121_GetTextWidth(wszLabel);
    D2Win_10117_DrawText(wszLabel, nX - (nW / 2), nY - 10, nTextColor, 0);
}

// 1.12a: 0x6FAEDEB0 (__fastcall)
void __fastcall AUTOMAP_DrawCellTree(D2AutomapCellStrc* pNode, const D2AutomapDrawRectStrc* pRect)
{
    if (!pRect || !pRect->pCellFile || gnAutomapDivisor <= 0) return;
    const int32_t nMaxFrames = D2CMP_CelFileGetCelsPerDirection(pRect->pCellFile);
    while (pNode) {
        int32_t x = ((int32_t)pNode->xPixel * 10) / gnAutomapDivisor - gnAutomapOffsetX;
        int32_t y = ((int32_t)pNode->yPixel * 10) / gnAutomapDivisor - gnAutomapOffsetY;
        if (y >= pRect->nTop) {
            AUTOMAP_DrawCellTree(pNode->pLess, pRect);
            if (y >= pRect->nTop && y <= pRect->nBottom && x >= pRect->nLeft && x <= pRect->nRight) {
                if (pNode->nCellNo >= 0 && pNode->nCellNo < nMaxFrames) {
                    D2GfxDataStrc tGfx = {};
                    tGfx.pCellFile = pRect->pCellFile;
                    tGfx.nDirection = 0;
                    tGfx.nFrame = (uint32_t)pNode->nCellNo;
                    const DrawMode eMode = (gnAutomapFadeMode != 0) ? DRAWMODE_TRANS50 : DRAWMODE_NORMAL;
                    TEXTURE_CelDraw(&tGfx, x - pRect->nOffsetX, y - pRect->nOffsetY, 0xFFFFFFFFu, eMode, nullptr);
                }
            }
        }
        if (y > pRect->nBottom) {
            break;
        }
        pNode = pNode->pMore;
    }
}

// 1.12a: 0x6FAEE170 (custom: ebx = pRecord)
void __stdcall AUTOMAP_WriteLayerToFile(const D2AutomapLayerFileRecordStrc* pRecord)
{
    (void)pRecord;
    // Appends or links serialized layer record into the active .ma<idx> file
}

// 1.12a: 0x6FAEE3B0 (__stdcall)
void __stdcall AUTOMAP_BalanceLeft(D2AutomapCellStrc** ppNode)
{
    D2AutomapCellStrc* pRoot = *ppNode;
    D2AutomapCellStrc* pRight = pRoot->pMore;
    if (pRight->wWeight == 1) {
        pRoot->wWeight = 0;
        pRight->wWeight = 0;
        AUTOMAP_RotateLeft(0, &pRoot);
        *ppNode = pRoot;
    } else {
        D2AutomapCellStrc* pRL = pRight->pLess;
        if (pRL->wWeight == -1) {
            pRoot->wWeight = 0;
            pRight->wWeight = 1;
        } else if (pRL->wWeight == 0) {
            pRoot->wWeight = 0;
            pRight->wWeight = 0;
        } else if (pRL->wWeight == 1) {
            pRoot->wWeight = -1;
            pRight->wWeight = 0;
        }
        pRL->wWeight = 0;
        AUTOMAP_RotateRight(0, &pRoot->pMore);
        AUTOMAP_RotateLeft(0, &pRoot);
        *ppNode = pRoot;
    }
}

// 1.12a: 0x6FAEE470 (__stdcall)
void __stdcall AUTOMAP_BalanceRight(D2AutomapCellStrc** ppNode)
{
    D2AutomapCellStrc* pRoot = *ppNode;
    D2AutomapCellStrc* pLeft = pRoot->pLess;
    if (pLeft->wWeight == -1) {
        pRoot->wWeight = 0;
        pLeft->wWeight = 0;
        AUTOMAP_RotateRight(0, &pRoot);
        *ppNode = pRoot;
    } else {
        D2AutomapCellStrc* pLR = pLeft->pMore;
        if (pLR->wWeight == -1) {
            pRoot->wWeight = 1;
            pLeft->wWeight = 0;
        } else if (pLR->wWeight == 0) {
            pRoot->wWeight = 0;
            pLeft->wWeight = 0;
        } else if (pLR->wWeight == 1) {
            pRoot->wWeight = 0;
            pLeft->wWeight = -1;
        }
        pLR->wWeight = 0;
        AUTOMAP_RotateLeft(0, &pRoot->pLess);
        AUTOMAP_RotateRight(0, &pRoot);
        *ppNode = pRoot;
    }
}

D2AutomapLayerStrc* __fastcall AUTOMAP_InitLayer(int32_t nLayerNo);

static void AUTOMAP_SubtileToScreen(int32_t nSubX, int32_t nSubY, int32_t* pOutScreenX, int32_t* pOutScreenY)
{
    const int32_t nDiv = (gnAutomapDivisor > 0) ? gnAutomapDivisor : 10;
    int32_t nIsoX = (nSubX - nSubY) * 16;
    int32_t nIsoY = (nSubX + nSubY) * 8;
    *pOutScreenX = (nIsoX / nDiv) + 8 - gnAutomapOffsetX;
    *pOutScreenY = (nIsoY / nDiv) - 8 - gnAutomapOffsetY;
}

// 1.12a: 0x6FAEE530 (__stdcall)
void __stdcall AUTOMAP_UpdateViewportBounds(void)
{
    const int32_t nW = (gnScreenWidth > 0) ? gnScreenWidth : 800;
    const int32_t nH = (gnScreenHeight > 0) ? gnScreenHeight : 600;
    if (gnAutomapDivisor <= 0) {
        gnAutomapDivisor = (gnAutomapMode == 1) ? 20 : 10;
    }
    gnAutomapBoundsMinX = 0;
    gnAutomapBoundsMinY = 0;
    gnAutomapBoundsMaxX = nW;
    gnAutomapBoundsMaxY = (nH > 48) ? (nH - 48) : nH;
    if (gnViewportShiftMode == 1) {
        gnAutomapBoundsMaxX = nW / 2;
    } else if (gnViewportShiftMode == 2) {
        gnAutomapBoundsMinX = nW / 2;
    }
    gnAutomapMiniLeftX = gbAutomapLeft ? 16 : (nW / 2);
    gnAutomapMiniTopY = 16;
}

// 1.12a: 0x6FAEE640 (__stdcall, spans 0x6FAEE640..0x6FAEE807 including 0x6FAEE7A0)
void __stdcall AUTOMAP_LoadSprites(void)
{
    AUTOMAP_FreeSprites();
    const bool bMini = (gnAutomapMode == 1);
    gpAutomapSprites[0] = ARCHIVE_LoadCellFileDefault(bMini ? "AUTOMAP\\MaxiMapS" : "AUTOMAP\\MaxiMap", 0);
    gpAutomapSprites[3] = ARCHIVE_LoadCellFileDefault(bMini ? "AUTOMAP\\ExTnMapS" : "AUTOMAP\\ExTnMap", 0);
    for (int i = 0; i < 4; ++i) {
        gnAutomapSpriteCels[i] = gpAutomapSprites[i] ? D2CMP_CelFileGetCelsPerDirection(gpAutomapSprites[i]) : 0;
    }
}

// 1.12a: 0x6FAEE810 (__stdcall)
void __stdcall AUTOMAP_DrawLevelNameText(void)
{
    if (!gpPlayerUnit) return;
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(gpPlayerUnit);
    const int32_t nLevelId = pRoom ? DUNGEON_GetLevelIdFromRoom(pRoom) : 1;
    D2LevelsTxt* pLevelsTxt = DATATBLS_GetLevelsTxtRecord(nLevelId);
    if (pLevelsTxt && pLevelsTxt->wszLevelName[0]) {
        AUTOMAP_DrawRightAlignedUnicodeText(pLevelsTxt->wszLevelName, 4, D2FONT_FONT16, 18);
    } else {
        char szFallback[48] = {};
        snprintf(szFallback, sizeof(szFallback), "Area %d", nLevelId);
        Unicode wszFallback[48] = {};
        Unicode::win2Unicode(wszFallback, szFallback, 47);
        AUTOMAP_DrawRightAlignedUnicodeText(wszFallback, 4, D2FONT_FONT16, 18);
    }
}

// 1.12a: 0x6FAEE860 (custom: edi = pRosterUnit)
void __stdcall AUTOMAP_DrawRemotePartyMemberMarker(D2RosterUnitStrc* pRosterUnit)
{
    (void)pRosterUnit;
}

// 1.12a: 0x6FAEE9B0 (__stdcall)
void __stdcall AUTOMAP_DrawAllLayers(void)
{
    if (!gpPlayerUnit) return;
    const int32_t nDiv = (gnAutomapDivisor > 0) ? gnAutomapDivisor : 10;
    const int32_t nPlrX = UNITS_GetXPosition(gpPlayerUnit);
    const int32_t nPlrY = UNITS_GetYPosition(gpPlayerUnit);
    const int32_t nCenterX = ((gnAutomapBoundsMinX + gnAutomapBoundsMaxX) / 2) + gnAutomapPanX;
    const int32_t nCenterY = ((gnAutomapBoundsMinY + gnAutomapBoundsMaxY) / 2) + gnAutomapPanY;
    const int32_t nPlrIsoX = (nPlrX - nPlrY) * 16;
    const int32_t nPlrIsoY = (nPlrX + nPlrY) * 8;
    gnAutomapOffsetX = (nPlrIsoX / nDiv) + 8 - nCenterX;
    gnAutomapOffsetY = (nPlrIsoY / nDiv) - 8 - nCenterY;

    D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(gpPlayerUnit);
    const int32_t nLevelId = pPlayerRoom ? DUNGEON_GetLevelIdFromRoom(pPlayerRoom) : 1;
    D2LevelDefBin* pLevelDef = DATATBLS_GetLevelDefRecord(nLevelId);
    const int32_t nLayerNo = (pLevelDef && pLevelDef->dwLayer > 0) ? (int32_t)pLevelDef->dwLayer : nLevelId;
    if (!gpCurrentAutomapLayer || gpCurrentAutomapLayer->nLayerNo != nLayerNo) {
        AUTOMAP_InitLayer(nLayerNo);
    }

    if (gpCurrentAutomapLayer && gpAutomapSprites[0]) {
        D2AutomapDrawRectStrc drawRect = {};
        drawRect.pCellFile = gpAutomapSprites[0];
        drawRect.nLeft = gnAutomapBoundsMinX - 48;
        drawRect.nRight = gnAutomapBoundsMaxX + 48;
        drawRect.nTop = gnAutomapBoundsMinY - 48;
        drawRect.nBottom = gnAutomapBoundsMaxY + 48;
        drawRect.nOffsetX = 0;
        drawRect.nOffsetY = 0;
        AUTOMAP_DrawCellTree(gpCurrentAutomapLayer->pFloors, &drawRect);
        AUTOMAP_DrawCellTree(gpCurrentAutomapLayer->pWalls, &drawRect);
        AUTOMAP_DrawCellTree(gpCurrentAutomapLayer->pObjects, &drawRect);
        AUTOMAP_DrawCellTree(gpCurrentAutomapLayer->pExtras, &drawRect);
    }

    // Render loaded room collision boundary edges from gpClientAct so walls, fences, riverbanks, and paths render crisply
    if (gpClientAct) {
        const uint8_t nWallCol = gAutomapColors[7] ? gAutomapColors[7] : 0x19u;
        const uint8_t nAlpha = (gnAutomapFadeMode != 0) ? 0x78u : 0xC8u;
        for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom != nullptr; pRoom = pRoom->pRoomNext) {
            D2RoomCollisionGridStrc* pGrid = pRoom->pCollisionGrid;
            if (!pGrid || !pGrid->pCollisionMask) continue;
            const int32_t nSubLeft = pGrid->pRoomCoords.nSubtileX;
            const int32_t nSubTop  = pGrid->pRoomCoords.nSubtileY;
            const int32_t nSubW    = pGrid->pRoomCoords.nSubtileWidth;
            const int32_t nSubH    = pGrid->pRoomCoords.nSubtileHeight;
            if (nSubW <= 0 || nSubH <= 0) continue;
            if (nSubLeft + nSubW < nPlrX - 160 || nSubLeft > nPlrX + 160 ||
                nSubTop  + nSubH < nPlrY - 160 || nSubTop  > nPlrY + 160) {
                continue;
            }

            const uint16_t kBlockMask = COLLIDE_WALL | COLLIDE_OBJECT | COLLIDE_DOOR | COLLIDE_NO_PATH | COLLIDE_MISSILE_BARRIER;
            for (int32_t ly = 0; ly < nSubH; ly += 2) {
                for (int32_t lx = 0; lx < nSubW; lx += 2) {
                    const bool bBlocked = (pGrid->pCollisionMask[ly * nSubW + lx] & kBlockMask) != 0;
                    const int32_t wx = nSubLeft + lx;
                    const int32_t wy = nSubTop + ly;

                    if (lx + 2 < nSubW) {
                        const bool bRightBlocked = (pGrid->pCollisionMask[ly * nSubW + (lx + 2)] & kBlockMask) != 0;
                        if (bBlocked != bRightBlocked) {
                            int32_t x1 = 0, y1 = 0, x2 = 0, y2 = 0;
                            AUTOMAP_SubtileToScreen(wx + 1, wy, &x1, &y1);
                            AUTOMAP_SubtileToScreen(wx + 1, wy + 2, &x2, &y2);
                            if (x1 >= gnAutomapBoundsMinX && x1 <= gnAutomapBoundsMaxX &&
                                y1 >= gnAutomapBoundsMinY && y1 <= gnAutomapBoundsMaxY) {
                                D2GFX_DrawLine(x1, y1, x2, y2, nWallCol, nAlpha);
                            }
                        }
                    }
                    if (ly + 2 < nSubH) {
                        const bool bDownBlocked = (pGrid->pCollisionMask[(ly + 2) * nSubW + lx] & kBlockMask) != 0;
                        if (bBlocked != bDownBlocked) {
                            int32_t x1 = 0, y1 = 0, x2 = 0, y2 = 0;
                            AUTOMAP_SubtileToScreen(wx, wy + 1, &x1, &y1);
                            AUTOMAP_SubtileToScreen(wx + 2, wy + 1, &x2, &y2);
                            if (x1 >= gnAutomapBoundsMinX && x1 <= gnAutomapBoundsMaxX &&
                                y1 >= gnAutomapBoundsMinY && y1 <= gnAutomapBoundsMaxY) {
                                D2GFX_DrawLine(x1, y1, x2, y2, nWallCol, nAlpha);
                            }
                        }
                    }
                }
            }
        }
    }
}

// 1.12a: 0x6FAEEEB0 (__stdcall)
void __stdcall AUTOMAP_SaveCurrentLayer(void)
{
    if (!gpCurrentAutomapLayer) {
        return;
    }
    // Counts and serializes pFloors, pWalls, pObjects, pExtras trees and writes them via AUTOMAP_WriteLayerToFile
}

// 1.12a: 0x6FAEF090 (__fastcall)
int32_t __fastcall AUTOMAP_InsertCell(D2AutomapCellStrc* pCell, D2AutomapCellStrc** ppRoot)
{
    D2AutomapCellStrc* pRoot = *ppRoot;
    if (!pRoot) {
        pCell->pLess = nullptr;
        pCell->pMore = nullptr;
        pCell->wWeight = 0;
        *ppRoot = pCell;
        return 1;
    }

    int32_t nCmp = AUTOMAP_CompareCells(pCell, pRoot);
    if (nCmp < 0) {
        if (!AUTOMAP_InsertCell(pCell, &pRoot->pLess)) {
            *ppRoot = pRoot;
            return 0;
        }
        if (pRoot->wWeight == -1) {
            AUTOMAP_BalanceRight(&pRoot);
            *ppRoot = pRoot;
            return 0;
        } else if (pRoot->wWeight == 0) {
            pRoot->wWeight = -1;
            *ppRoot = pRoot;
            return 1;
        } else {
            pRoot->wWeight = 0;
            *ppRoot = pRoot;
            return 0;
        }
    } else if (nCmp > 0) {
        if (!AUTOMAP_InsertCell(pCell, &pRoot->pMore)) {
            *ppRoot = pRoot;
            return 0;
        }
        if (pRoot->wWeight == -1) {
            pRoot->wWeight = 0;
            *ppRoot = pRoot;
            return 0;
        } else if (pRoot->wWeight == 0) {
            pRoot->wWeight = 1;
            *ppRoot = pRoot;
            return 1;
        } else {
            AUTOMAP_BalanceLeft(&pRoot);
            *ppRoot = pRoot;
            return 0;
        }
    }

    *ppRoot = pRoot;
    return 0;
}

// 1.12a: 0x6FAEF1B0 (__fastcall)
void __fastcall AUTOMAP_CenterView(int32_t bForceCenter)
{
    if (!gbAutomapCenters && !bForceCenter) {
        return;
    }
    AUTOMAP_UpdateViewportBounds();
    if (gnAutomapMode != 1) {
        gnAutomapPanX = 0;
        gnAutomapPanY = 0;
    } else {
        gnAutomapPanX = (gnScreenWidth / 3) - gnAutomapMiniLeftX - 16;
        gnAutomapPanY = (gnScreenHeight / 3) - gnAutomapMiniTopY - 16;
    }
}

// 1.12a: 0x6FAEF230 (custom: eax = nMode)
void __fastcall AUTOMAP_SetViewMode(int32_t nMode)
{
    if (nMode != gnAutomapMode) {
        gnAutomapMode = nMode;
        gnAutomapDivisor = (nMode == 1) ? 20 : 10;
        AUTOMAP_LoadSprites();
        AUTOMAP_CenterView(1);
    }
}

// 1.12a: 0x6FAEF270 (__stdcall)
void __stdcall AUTOMAP_FreeAllLayers(void)
{
    AUTOMAP_SaveCurrentLayer();
    AUTOMAP_FreeAllCells();
    AUTOMAP_FreeSprites();

    D2AutomapLayerStrc* pLayer = gpAutomapLayerList;
    while (pLayer) {
        D2AutomapLayerStrc* pNext = pLayer->pNext;
        FOG_Free(pLayer, "..\\Source\\D2Client\\UI\\automap.cpp", 0xC1A, 0);
        pLayer = pNext;
    }
    gpAutomapLayerList = nullptr;
    gpCurrentAutomapLayer = nullptr;
}

// 1.12a: 0x6FAEF2C0 (__stdcall)
void __stdcall AUTOMAP_Initialize(void)
{
    AUTOMAP_LoadSprites();
    for (int i = 0; i < 2048; ++i) {
        gaAutomapCelGroups[i] = -1;
    }
    for (int i = 0; i < 49; ++i) {
        gaAutomapCelGroups[gAutomapCelGroupInitTable[i].nCellNo] = gAutomapCelGroupInitTable[i].nGroupId;
    }

    int32_t nVal = 0;
    SRegLoadValue("Diablo II", "AutoMapFade", 0, &nVal);
    gnAutomapFadeMode = nVal;

    nVal = 1;
    SRegLoadValue("Diablo II", "AutoMap Centers", 0, &nVal);
    gbAutomapCenters = nVal;

    nVal = 1;
    SRegLoadValue("Diablo II", "AutoMap Party", 0, &nVal);
    gbAutomapShowParty = nVal;

    nVal = 1;
    SRegLoadValue("Diablo II", "AutoMap Party Names", 0, &nVal);
    gbAutomapShowPartyNames = nVal;

    nVal = 1;
    SRegLoadValue("Diablo II", "AutoMap Left", 0, &nVal);
    gbAutomapLeft = nVal;
    gbAutomapLeftSaved1 = nVal;
    gbAutomapLeftSaved2 = nVal;
}

// 1.12a: 0x6FAEF3D0 (custom: eax = pUnit, ebx = pOutColor)
int32_t __fastcall AUTOMAP_GetUnitMarkerColor(D2UnitStrc* pUnit, uint8_t* pOutColor)
{
    if (!pUnit || !pOutColor) return 0;
    if (pUnit == gpPlayerUnit) {
        *pOutColor = gAutomapColors[6] ? gAutomapColors[6] : 0xFFu;
        return 1;
    }
    if (pUnit->dwUnitType == UNIT_PLAYER) {
        *pOutColor = gAutomapColors[3] ? gAutomapColors[3] : 0x84u;
        return 1;
    }
    if (pUnit->dwUnitType == UNIT_MONSTER) {
        if (pUnit->dwAnimMode == MONMODE_DEAD || pUnit->dwAnimMode == MONMODE_DEATH) {
            return 0;
        }
        if (gpPlayerUnit && pUnit == D2Client_GetLocalMercUnit(gpPlayerUnit)) {
            *pOutColor = gAutomapColors[3] ? gAutomapColors[3] : 0x84u;
            return 1;
        }
        D2MonStatsTxt* pMonTxt = DATATBLS_GetMonStatsTxtRecord(pUnit->dwClassId);
        if (!pMonTxt) {
            return 0;
        }
        const D2MonStats2Txt* pMon2 = (sgptDataTables && pMonTxt->wMonStatsEx < sgptDataTables->nMonStats2TxtRecordCount)
            ? &sgptDataTables->pMonStats2Txt[pMonTxt->wMonStatsEx]
            : nullptr;
        if (pMon2 && (pMon2->dwFlags & (MONSTATS2FLAG_NOMAP | MONSTATS2FLAG_CRITTER)) != 0) {
            return 0;
        }
        if ((pMonTxt->dwMonStatsFlags & MONSTATSFLAG_NPC) != 0 &&
            (pMonTxt->dwMonStatsFlags & MONSTATSFLAG_INTERACT) != 0) {
            const Unicode* wszName = D2Client_GetNpcDisplayName(pUnit);
            if (!wszName || wszName[0].ch == 0 ||
                _wcsicmp(reinterpret_cast<const wchar_t*>(wszName), L"an evil force") == 0) {
                return 0;
            }
            *pOutColor = gAutomapColors[6] ? gAutomapColors[6] : 0xFFu;
            return 1;
        }
        if (pMonTxt->nAlign == MONALIGN_EVIL && (!pMon2 || (pMon2->dwFlags & MONSTATS2FLAG_ISATT) != 0)) {
            *pOutColor = gAutomapColors[1] ? gAutomapColors[1] : 0x62u;
            return 1;
        }
        return 0;
    }
    if (pUnit->dwUnitType == UNIT_OBJECT) {
        if (pUnit->dwClassId == 59 || pUnit->dwClassId == 60) {
            *pOutColor = gAutomapColors[8] ? gAutomapColors[8] : 0x69u;
            return 1;
        }
        D2ObjectsTxt* pObjTxt = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId);
        if (pObjTxt && (pObjTxt->nSubClass & 0x40)) {
            *pOutColor = gAutomapColors[6] ? gAutomapColors[6] : 0xFFu;
            return 1;
        }
        if (pUnit->dwClassId == 267) {
            *pOutColor = gAutomapColors[8] ? gAutomapColors[8] : 0x69u;
            return 1;
        }
    }
    return 0;
}

// 1.12a: 0x6FAEF6C0 (custom: ebx = pUnit, stack = nCellNo, ppCellTree)
void __stdcall AUTOMAP_AddUnitCell(D2UnitStrc* pUnit, int32_t nCellNo, D2AutomapCellStrc** ppCellTree)
{
    if (!pUnit || !ppCellTree) return;
    D2AutomapCellStrc* pCell = AUTOMAP_NewCell();
    pCell->nCellNo = (int16_t)nCellNo;
    const int32_t nSubX = UNITS_GetXPosition(pUnit);
    const int32_t nSubY = UNITS_GetYPosition(pUnit);
    pCell->xPixel = (int16_t)(((nSubX - nSubY) * 16) / 10 + 1);
    pCell->yPixel = (int16_t)(((nSubX + nSubY) * 8) / 10 - 3);
    AUTOMAP_InsertCell(pCell, ppCellTree);
}

// 1.12a: 0x6FAEF780 (__stdcall)
void __stdcall AUTOMAP_AddExplicitCell(int16_t nCellNo, int16_t xPixel, int16_t yPixel, D2AutomapCellStrc** ppCellTree, int32_t fSaved)
{
    D2AutomapCellStrc* pCell = AUTOMAP_NewCell();
    pCell->fSaved = fSaved;
    pCell->nCellNo = nCellNo;
    pCell->xPixel = xPixel;
    pCell->yPixel = yPixel;
    AUTOMAP_InsertCell(pCell, ppCellTree);
}

// 1.12a: 0x6FAEF7C0 (custom: esi = pTileEntry, edi = pRoomEx, stack = ppCellTree)
void __stdcall AUTOMAP_AddRoomTileCell(D2TileLibraryEntryStrc* pTileEntry, D2RoomExStrc* pRoomEx, D2AutomapCellStrc** ppCellTree)
{
    (void)pTileEntry;
    (void)pRoomEx;
    (void)ppCellTree;
}

// 1.12a: 0x6FAEF920 (custom: eax = pUnit)
void __fastcall AUTOMAP_DrawUnitMarker(D2UnitStrc* pUnit)
{
    if (!pUnit) return;
    uint8_t nColor = 0;
    if (!AUTOMAP_GetUnitMarkerColor(pUnit, &nColor)) return;

    const int32_t nSubX = UNITS_GetXPosition(pUnit);
    const int32_t nSubY = UNITS_GetYPosition(pUnit);
    if (nSubX <= 0 && nSubY <= 0) return;

    int32_t nScreenX = 0, nScreenY = 0;
    AUTOMAP_SubtileToScreen(nSubX, nSubY, &nScreenX, &nScreenY);
    if (nScreenX < gnAutomapBoundsMinX || nScreenX > gnAutomapBoundsMaxX ||
        nScreenY < gnAutomapBoundsMinY || nScreenY > gnAutomapBoundsMaxY) {
        return;
    }

    AUTOMAP_DrawCrosshair(nScreenX, nScreenY, nColor);

    if (!gbAutomapShowPartyNames) {
        return;
    }

    if (pUnit->dwUnitType == UNIT_MONSTER && pUnit != gpPlayerUnit) {
        D2MonStatsTxt* pMonTxt = DATATBLS_GetMonStatsTxtRecord(pUnit->dwClassId);
        if (pMonTxt && pMonTxt->wNameStr != 0 &&
            (pMonTxt->dwMonStatsFlags & MONSTATSFLAG_NPC) != 0 &&
            (pMonTxt->dwMonStatsFlags & MONSTATSFLAG_INTERACT) != 0) {
            const Unicode* wszName = D2Client_GetNpcDisplayName(pUnit);
            if (wszName && wszName[0].ch != 0 &&
                _wcsicmp(reinterpret_cast<const wchar_t*>(wszName), L"an evil force") != 0) {
                D2Win_10127_SetFont(D2FONT_FONT6);
                const int32_t nW = D2Win_10121_GetTextWidth(wszName);
                D2Win_10117_DrawText(wszName, nScreenX - (nW / 2), nScreenY - 10, 0, 0);
            }
        }
    } else if (pUnit->dwUnitType == UNIT_OBJECT) {
        D2ObjectsTxt* pObjTxt = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId);
        if (pObjTxt && (pObjTxt->nSubClass & 0x40)) {
            AUTOMAP_DrawMarkerLabel("WP", nScreenX, nScreenY, 0);
        } else if (pUnit->dwClassId == 267) {
            AUTOMAP_DrawMarkerLabel("Stash", nScreenX, nScreenY, 4);
        }
    }
}

// 1.12a: 0x6FAEFC40 (__stdcall)
void __stdcall AUTOMAP_RevealRoomUnits(D2RoomStrc* pRoom, D2AutomapCellStrc** ppCellTree)
{
    if (!pRoom || !ppCellTree) return;
    for (D2UnitStrc* pUnit = DUNGEON_GetFirstUnitInRoom(pRoom); pUnit != nullptr; pUnit = pUnit->pRoomNext) {
        if (pUnit->dwUnitType == UNIT_OBJECT) {
            D2ObjectsTxt* pObjTxt = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId);
            if (pObjTxt && pObjTxt->dwAutomap > 0) {
                AUTOMAP_AddUnitCell(pUnit, (int32_t)pObjTxt->dwAutomap, ppCellTree);
            }
        }
    }
}

// 1.12a: 0x6FAEFDB0 (__stdcall)
void __stdcall AUTOMAP_LoadCurrentLayerFromFile(void)
{
}

// 1.12a: 0x6FAF0350 (__stdcall)
void __stdcall AUTOMAP_DrawUnitsAndParty(void)
{
    const int32_t aTypes[3] = { UNIT_OBJECT, UNIT_MONSTER, UNIT_PLAYER };
    for (int t = 0; t < 3; ++t) {
        const int32_t nType = aTypes[t];
        for (int b = 0; b < 128; ++b) {
            for (D2UnitStrc* pUnit = gapServerSideUnitHashTables[nType][b]; pUnit != nullptr; pUnit = pUnit->pListNext) {
                if (pUnit != gpPlayerUnit) {
                    AUTOMAP_DrawUnitMarker(pUnit);
                }
            }
        }
    }
    if (gpPlayerUnit) {
        AUTOMAP_DrawUnitMarker(gpPlayerUnit);
    }
}

// 1.12a: 0x6FAF04C0 (__stdcall)
void __stdcall AUTOMAP_RevealRoom(D2RoomStrc* pRoom, int32_t bRevealAllInRoom, D2AutomapLayerStrc* pLayer)
{
    (void)bRevealAllInRoom;
    if (!pRoom || !pLayer) return;
    AUTOMAP_RevealRoomUnits(pRoom, &pLayer->pObjects);
}

// 1.12a: 0x6FAF05A0 (__stdcall)
void __stdcall AUTOMAP_SwitchLayer(D2AutomapLayerStrc* pLayer)
{
    AUTOMAP_SaveCurrentLayer();
    AUTOMAP_FreeAllCells();
    gpCurrentAutomapLayer = pLayer;
    AUTOMAP_LoadCurrentLayerFromFile();
}

// 1.12a: 0x6FAF05C0 (__stdcall)
void __stdcall AUTOMAP_Draw(void)
{
    if (!gpPlayerUnit) return;
    if (gAutomapColors[6] == 0) {
        AUTOMAP_InitColors();
    }
    if (!gpAutomapSprites[0]) {
        AUTOMAP_LoadSprites();
    }
    AUTOMAP_UpdateViewportBounds();
    AUTOMAP_DrawAllLayers();
    AUTOMAP_DrawUnitsAndParty();

    gnAutomapTextY = 20;
    AUTOMAP_DrawLevelNameText();
    AUTOMAP_DrawDifficultyText();
    AUTOMAP_DrawExpansionText();
}

// 1.12a: 0x6FAF0650 (custom: eax = nLayerNo)
D2AutomapLayerStrc* __fastcall AUTOMAP_InitLayer(int32_t nLayerNo)
{
    D2AutomapLayerStrc* pLayer = gpAutomapLayerList;
    while (pLayer) {
        if (pLayer->nLayerNo == nLayerNo) {
            break;
        }
        pLayer = pLayer->pNext;
    }
    if (!pLayer) {
        pLayer = AUTOMAP_AllocLayer(nLayerNo);
    }
    if (pLayer != gpCurrentAutomapLayer) {
        AUTOMAP_SaveCurrentLayer();
        AUTOMAP_FreeAllCells();
        gpCurrentAutomapLayer = pLayer;
        AUTOMAP_LoadCurrentLayerFromFile();
    }
    return pLayer;
}

// 1.12a: 0x6FAF06A0 (custom: ebx = nTileX, stack = nLevelId, nTileWidth, nTileY, nVariant)
void __stdcall AUTOMAP_InitTownLayer(int32_t nLevelId, int32_t nVariant, int32_t nTileX, int32_t nTileY)
{
    (void)nLevelId;
    (void)nVariant;
    (void)nTileX;
    (void)nTileY;
}

// 1.12a: 0x6FAF09E0 (__stdcall)
void __stdcall AUTOMAP_RevealRoomFull(D2RoomStrc* pRoom)
{
    if (!pRoom || !gpCurrentAutomapLayer) return;
    AUTOMAP_RevealRoom(pRoom, 1, gpCurrentAutomapLayer);
}

// 1.12a: 0x6FAF0A30 (__stdcall)
void __stdcall AUTOMAP_UpdateReveal(void)
{
    if (gnAutomapRevealDelayTicks != 0) {
        --gnAutomapRevealDelayTicks;
        return;
    }
    if (!gpPlayerUnit || !gpCurrentAutomapLayer) return;
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(gpPlayerUnit);
    if (pRoom) {
        AUTOMAP_RevealRoom(pRoom, 0, gpCurrentAutomapLayer);
    }
}
