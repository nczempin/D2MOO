// =============================================================================
// D2Client.dll 1.12a (ImageBase = 0x6FAB0000)
// Reconstructed Translation Unit: UI/QuestLog.cpp (0x6FB6F870 .. 0x6FB72F90)
//
// Source Assertion Path: "..\Source\D2Client\UI\QuestLog.cpp" (0x6FB80330)
// Cross-referenced against:
//   - H:/git/D2MOO-112a/source/D2Common/include/D2QuestRecord.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Dungeon.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Drlg/D2DrlgDrlg.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Units.h
//   - H:/git/D2MOO-112a/source/D2Lang/include/D2StrTable.h
//   - H:/git/D2MOO-112a/source/D2Lang/include/D2Unicode.h
//   - H:/git/D2MOO-112a/source/D2Gfx/include/Texture.h
//   - H:/git/D2MOO-112a/source/D2Gfx/include/Window.h
//   - H:/git/D2MOO-112a/source/D2CMP/include/CelCmp.h
// =============================================================================

#include <stdint.h>
#include <string.h>
#include <windows.h>

#include <D2QuestRecord.h>
#include <D2Dungeon.h>
#include <Units/Units.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Unicode.h>

// =============================================================================
// External Imports (D2Common, D2Gfx, D2Win, D2Lang, D2CMP, Fog, Storm)
// =============================================================================
// D2Gfx.#10024
extern "C" void __stdcall TEXTURE_CelDraw(void* pData, int32_t nX, int32_t nY, uint32_t dwGamma, int32_t nDrawMode, uint8_t* pPalette);
// D2Gfx.#10043
extern "C" int32_t __stdcall D2GFX_GetResolutionMode(void);
// D2Gfx.#10072
extern "C" void __fastcall D2GFX_SetViewMatrixAdjust(int32_t nAdjust);
// D2Gfx.#10078
extern "C" HWND __stdcall WINDOW_GetWindow(void);

// D2CMP.#10031
extern "C" int32_t __stdcall D2CMP_CelGetWidth(void* pCelHandle);
// D2CMP.#10038
extern "C" void* __stdcall D2CMP_CelGetHandle(void* pCelData);
// D2CMP.#10106
extern "C" int32_t __stdcall D2CMP_CelFileFreeHardware(void* pFile);

// D2Win.#10001
extern "C" void __fastcall D2Win_10117_DrawText(const Unicode* pText, int32_t nX, int32_t nY, int32_t nColor, int32_t bCentered);
// D2Win.#10010
extern "C" int32_t __fastcall D2Win_10127_SetFont(int32_t nFont);
// D2Win.#10031
extern "C" void __fastcall D2Win_10130_DrawFramedTextEx(const Unicode* pText, int32_t nX, int32_t nY, int32_t nColor, int32_t nAlign);
// D2Win.#10117
struct D2WinWrappedTextNodeStrc
{
    Unicode* pLineText;                 // +0x00
    D2WinWrappedTextNodeStrc* pNext;    // +0x04
};
extern "C" D2WinWrappedTextNodeStrc* __fastcall D2Win_10199_WrapTextLines(Unicode* pText, int32_t* pOutLineCount, int32_t nMaxWidth);
// D2Win.#10132
extern "C" int32_t __fastcall D2Win_10121_GetTextWidth(const Unicode* pText);

// Fog.#10265
extern "C" const char* __stdcall FOG_GetErrorLocation(int32_t nLine);

// Storm.#401, #426, #481, #482, #483, #511
extern "C" void* __stdcall SMemAlloc(size_t nSize, const char* szFile, int32_t nLine, uint32_t dwFlags);
extern "C" int32_t __stdcall SRegSaveValue(const char* szSection, const char* szKey, uint32_t dwFlags, uint32_t dwValue);
extern "C" int32_t __stdcall SMemFindNextBlock(void* hHeap, void* pPrevBlock, void** ppNextBlock, void* pBlockDetails);
extern "C" int32_t __stdcall SMemFindNextHeap(void* pPrevHeap, void** ppNextHeap, void* pHeapDetails);
extern "C" void* __stdcall SMemGetHeapByCaller(const char* szFile, int32_t nLine);
extern "C" void __stdcall SMsgBreakHandlerChain(void* pMsg);

// Internal D2Client Cross-Unit Helpers
struct D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileDefault(const char* szPath, int32_t nFileType);        // 0x6FB6EF70
void __stdcall SOUND_PlaySoundEffect(int32_t nUnitType, int32_t nUnitId, int32_t nX, int32_t nY);          // 0x6FAE1FA0 (ebx = nSoundId)
void __stdcall GFX_PlayUnitOverlayOnPlayer(int32_t bLoop);                                                 // 0x6FB41F40 (ebx = nOverlayId)
void __fastcall CUNIT_IterateClientUnitsOfType(int32_t nUnitType, void(__fastcall* pfCallback)(D2UnitStrc*)); // 0x6FACEA20
void __fastcall MSG_SendBytePacket(uint8_t nOpcode);                                                       // 0x6FB29900
void __fastcall MSG_SendByteWordPacket(uint8_t nOpcode, uint16_t wValue);                                  // 0x6FB298E0
int32_t __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t bPlaySound);                // 0x6FB3A700
int32_t __fastcall MSG_SendPacket(void* pPacketData, size_t nSize);                                        // 0x6FB291A0
void __stdcall TEXT_DisplaySystemMessage(const Unicode* wszText, uint8_t nColor);                          // 0x6FB21740
void __stdcall TEXT_PlayQuestSpeechBox(uint16_t wStringId, int32_t nZero);                                 // 0x6FB21340
void __stdcall TEXT_CloseQuestSpeechBox(void);                                                             // 0x6FB22E40
void __stdcall GAME_TriggerhoradricAltarEffect(void);                                                      // 0x6FB2B390
void __stdcall GAME_TriggerDiabloSealScreenShake(void);                                                    // 0x6FB2AD80
void __fastcall NPCMENU_TriggerCainRescueUpdate(int32_t nState);                                           // 0x6FAC26D0
int32_t __stdcall CUNIT_IsPlayerInTownOrBusy(void);                                                        // 0x6FACEAC0
void __fastcall NPCMENU_CloseActiveMenu(int32_t nNpcGUID);                                                 // 0x6FACA1F0
void __fastcall UI_ClosePanelByIndex(int32_t nPanelIdx, int32_t bPlaySound);                               // 0x6FB3EF00
void __stdcall UI_RegisterWindowMsgTable(HWND hWnd, const void* pTable);                                   // 0x6FB1D100 (eax = nCount)
void __stdcall UI_UnregisterWindowMsgTable(HWND hWnd, const void* pTable);                                 // 0x6FB1D0A0 (eax = nCount)
void __stdcall UI_RegisterTooltipRegions(void);                                                            // 0x6FB3C400 (edi = pRegions)
void __stdcall UI_ClearTooltipRegions(void);                                                               // 0x6FB3AA10
void __stdcall VIEW_UpdateViewportDimensions(int32_t nWidth, int32_t nHeight, int32_t nOffset);            // 0x6FB6A990
void __stdcall CMDTBL_CenterCursorOnHalfScreen(void);                                                      // 0x6FB4ECD0

// =============================================================================
// Structures & Constants
// =============================================================================
#pragma pack(push, 1)

struct D2QuestStateDescPairStrc
{
    uint16_t wTextStrId;                // +0x00
    uint16_t wAudioStrId;               // +0x02
};

// 0x40 (64) bytes per quest in 0x6FB8BCC4 .. 0x6FB8C3C4
struct D2QuestDescTableStrc
{
    uint16_t wTitleStrId;               // +0x00
    uint16_t wDoneAudioStrId;           // +0x02
    uint16_t wRewardStateIdx;           // +0x04 (0xFFFF if none)
    D2QuestStateDescPairStrc aStates[13]; // +0x06 .. +0x39 (indexed by quest state 1..13)
    uint16_t wPad[3];                   // +0x3A .. +0x3F
};

// 0x10 (16) bytes per quest log record in g_aQuestLogRecords (0x6FB8C450, 41 entries)
struct D2QuestLogTableRecordStrc
{
    uint8_t bActive;                    // +0x00 (0x6FB8C450)
    uint8_t nIconIndex;                 // +0x01 (0x6FB8C451: 0..26 index into g_apszQuestIconNames)
    uint8_t nSocketIndex;               // +0x02 (0x6FB8C452: 0..5 socket position in Act tab)
    uint8_t nActNo;                     // +0x03 (0x6FB8C453: 0..4 Act I..V, 9 = unused)
    const D2QuestDescTableStrc* pDesc;  // +0x04 (0x6FB8C454)
    int32_t nQuestIndex;                // +0x08 (0x6FB8C458: 1..37 server quest ID)
    int32_t nQuestBitIndex;             // +0x0C (0x6FB8C45C: 0..40 bit buffer quest ID)
};

// 0x10 (16) bytes per socket in g_aQuestSocketCoords (0x6FB8C3C8, 6 entries)
struct D2QuestSocketPosStrc
{
    int32_t nDrawX;                     // +0x00 (0x6FB8C3C8)
    int32_t nDrawY;                     // +0x04 (0x6FB8C3CC)
    int32_t nHitLeft;                   // +0x08 (0x6FB8C3D0, width = 0x38)
    int32_t nHitTop;                    // +0x0C (0x6FB8C3D4, height = 0x34)
};

// 0x08 bytes per act in g_aActQuestIconRanges (0x6FB8C428, 5 entries)
struct D2ActQuestIconRangeStrc
{
    int32_t nFirstIconIdx;              // +0x00
    int32_t nLastIconIdx;               // +0x04
};

// 0x14 (20) bytes per resolution/offset mode in g_aQuestAlertBtnRects (0x6FBA9E00, 4 entries)
struct D2QuestAlertBtnRectStrc
{
    int32_t nLeft;                      // +0x00
    int32_t nRight;                     // +0x04
    int32_t nTop;                       // +0x08
    int32_t nBottom;                    // +0x0C
    int32_t nLabelY;                    // +0x10
};

// 0x26A (618) bytes per socket slot in g_aActiveQuestSockets (0x6FBCC661, 6 entries)
struct D2QuestSocketStateStrc
{
    uint8_t bValid;                     // +0x000 (0x6FBCC661)
    int32_t nQuestBitIndex;             // +0x001 (0x6FBCC662)
    uint16_t wTitleStrId;               // +0x005 (0x6FBCC666)
    Unicode wszDesc[300];               // +0x007 (0x6FBCC668 .. 0x6FBCC8BE)
    uint16_t wAudioStrId;               // +0x25F (0x6FBCC8C0)
    uint8_t nSocketIndex;               // +0x261 (0x6FBCC8C2)
    uint8_t nIconIndex;                 // +0x262 (0x6FBCC8C3)
    uint8_t bStateChanged;              // +0x263 (0x6FBCC8C4)
    uint8_t bHasDescription;            // +0x264 (0x6FBCC8C5)
    uint8_t nStateIndex;                // +0x265 (0x6FBCC8C6)
    int32_t nDisplayMode;               // +0x266 (0x6FBCC8C7: 0 = anim, 1 = done, 2 = inactive, 3 = active)
};
static_assert(sizeof(D2QuestSocketStateStrc) == 0x26A, "D2QuestSocketStateStrc must be 0x26A bytes");

// S2C 0x29 / 0x5D Quest Item Info Packet (6 bytes)
struct D2GSPacketSrv5DStrc
{
    uint8_t nHeader;                    // +0x00 (0x5D)
    uint8_t nQuestId;                   // +0x01
    uint8_t nAlertFlags;                // +0x02 (bit 0: event FX, bit 1: completion FX, bit 4: sound, bit 5: state update)
    uint8_t nQuestFilterState;          // +0x03
    int16_t nExtraParam;                // +0x04 (remaining monsters / resistance scroll / etc.)
};

// S2C 0x53 / sub-packet for monster/altar counters
struct D2QuestCounterPacketStrc
{
    uint8_t nHeader;                    // +0x00
    uint16_t wSubType;                  // +0x01 (1 = update counters)
    uint16_t wMonsterCount;             // +0x03
    uint16_t wSocketFrameIndex;         // +0x05
    uint16_t wExtraStrId;               // +0x07
};

#pragma pack(pop)

// =============================================================================
// Initialized Static Data (.data 0x6FB8BC40 .. 0x6FB8C6E0, 0x6FBA9E00)
// =============================================================================
static int32_t g_nQuestDescBaseY       = 270;  // 0x6FB8BC40
static int32_t g_nQuestTitleBaseY      = 248;  // 0x6FB8BC44
static int32_t g_nQuestDescBaseX       = 16;   // 0x6FB8BC48
static int32_t g_nSocketSelectOffsetY  = -1;   // 0x6FB8BC4C
static int32_t g_nDoneIconPressOffsetY = -1;   // 0x6FB8BC50

// 0x6FB8BC58: 27 Quest Icon DC6 Filenames
static const char* const g_apszQuestIconNames[27] = {
    "a1q1", "a1q2", "a1q3", "a1q4", "a1q5", "a1q6",
    "a2q1", "a2q2", "a2q3", "a2q4", "a2q5", "a2q6",
    "a3q1", "a3q2", "a3q3", "a3q4", "a3q5", "a3q6",
    "a4q1", "a4q2", "a4q3",
    "a5q1", "a5q2", "a5q3", "a5q4", "a5q5", "a5q6"
};

// 0x6FB8C3C8: 6 Quest Socket Positions & Hit Rectangles
static const D2QuestSocketPosStrc g_aQuestSocketCoords[6] = {
    {  26, 121,  32,  65 },
    { 123, 121, 128,  65 },
    { 220, 121, 224,  65 },
    {  26, 218,  32, 163 },
    { 123, 218, 128, 163 },
    { 220, 218, 224, 163 }
};

// 0x6FB8C428: Act I..V Quest Icon Index Ranges
static const D2ActQuestIconRangeStrc g_aActQuestIconRanges[5] = {
    {  0,  5 }, // Act I
    {  6, 11 }, // Act II
    { 12, 17 }, // Act III
    { 18, 20 }, // Act IV
    { 21, 26 }  // Act V
};

// 0x6FBA9E00: Quest Alert Corner Button Rectangles
static const D2QuestAlertBtnRectStrc g_aQuestAlertBtnRects[4] = {
    {  40,  75, 340, 375, 337 },
    { 360, 395, 340, 375, 337 },
    {  40,  75, 285, 320, 282 },
    { 360, 395, 285, 320, 282 }
};

// 0x6FBAAD30: Pending selected socket index per Act (initialized to -1)
static int32_t g_anPendingActSocket[5] = { -1, -1, -1, -1, -1 };

// =============================================================================
// Module Globals (.bss 0x6FBCC550 .. 0x6FBCD520)
// =============================================================================
extern D2UnitStrc*      g_pPlayerUnit;                  // 0x6FBCC3D0
extern D2BitBufferStrc* g_pPlayerQuestRecord;           // 0x6FBCB643
extern D2BitBufferStrc* g_pGameQuestRecord;             // 0x6FBCB647
extern int32_t          g_nScreenWidth;                 // 0x6FB8C6E0
extern int32_t          g_nScreenHeight;                // 0x6FB8C6E4
extern int32_t          g_nLeftPanelOffsetX;            // 0x6FBCBD28
extern int32_t          g_nLeftPanelOffsetY;            // 0x6FBCBD2C
extern int32_t          g_nMouseX;                      // 0x6FBB1638
extern int32_t          g_nMouseY;                      // 0x6FBB1634
extern int32_t          g_nViewShiftState;              // 0x6FBCC1D0
extern int32_t          g_nViewShiftOffset;             // 0x6FBCC1D4
extern int32_t          g_bExpansionChar;               // 0x6FBB330C
extern uint8_t          g_bCutsceneActive;              // 0x6FBCB6CF

static int32_t  g_nQuestLogOpenState;                   // 0x6FBCC550 (0 = closed, 1 = alert button shown, 2 = panel open)
static int32_t  g_nActiveActSocketCount;                // 0x6FBCC558 (0..6)
static int32_t  g_nDenOfEvilMonstersLeft;               // 0x6FBCC55C
static int32_t  g_nInifussScrollFrame;                  // 0x6FBCC560
static int32_t  g_nQuestExtraStringId;                  // 0x6FBCC564
static uint8_t  g_bAlertBtnPressed;                     // 0x6FBCC568
static uint8_t  g_bSocketBtnPressed;                    // 0x6FBCC569
static uint8_t  g_bCloseBtnPressed;                     // 0x6FBCC56A
static uint8_t  g_bQuestSpeechPlaying;                  // 0x6FBCC56B
static uint8_t  g_bAudioBtnPressed;                     // 0x6FBCC56C
static int32_t  g_nPressedSocketIdx;                    // 0x6FBCC56D
static int32_t  g_nSelectedSocketIdx;                   // 0x6FBCC571
static int32_t  g_anSavedActSelectedSocket[5];          // 0x6FBCC575 .. 0x6FBCC585
static void*    g_pCelQuestSockets;                     // 0x6FBCC589 ("%s\\ui\\menu\\questsockets")
static void*    g_pCelQuestBackground;                  // 0x6FBCC58D ("%s\\ui\\menu\\questbackground")
static void*    g_pCelQuestTabs;                        // 0x6FBCC591 ("%s\\ui\\menu\\expquesttabs" or "questtabs")
static void*    g_pCelQuestDone;                        // 0x6FBCC595 ("%s\\ui\\menu\\questdone")
static void*    g_apCelQuestIcons[27];                  // 0x6FBCC599 .. 0x6FBCC604
static void*    g_pCelQuestLast;                        // 0x6FBCC605 ("%s\\ui\\menu\\questlast")
static void*    g_pCelInvPs;                            // 0x6FBCC609 ("%s\\ui\\menu\\invps")
static uint8_t  g_anServerQuestFilterState[41];         // 0x6FBCC60E .. 0x6FBCC636
static uint8_t  g_anPrevQuestFilterState[41];           // 0x6FBCC638 .. 0x6FBCC660
static D2QuestSocketStateStrc g_aActiveQuestSockets[6]; // 0x6FBCC661 .. 0x6FBCD4DC
static int32_t  g_anSocketAnimFrame[6];                 // 0x6FBCD4DD .. 0x6FBCD4F4
static uint32_t g_adwSocketAnimTick[6];                 // 0x6FBCD4F5 .. 0x6FBCD50C
static int32_t  g_nCurrentQuestActTab;                  // 0x6FBCD50D (0..4)
static uint32_t g_dwLastToggleTick;                     // 0x6FBCD511
static uint8_t  g_bHoradricStaffFXTriggered;            // 0x6FBCD515
static uint8_t  g_bKhalimFlailFXTriggered;              // 0x6FBCD516
static uint8_t  g_bStaffTombFXActive;                   // 0x6FBCD517
static uint8_t  g_bStaffTombSmokeSpawned;               // 0x6FBCD518
static int32_t  g_nStaffTombSmokeUnitGUID;              // 0x6FBCD519
static int32_t  g_nStaffTombOrificeGUID;                // 0x6FBCD51D

// 0x6FB8BCC4 .. 0x6FB8C3C4: 28 Quest Description String/Audio Tables (0x40 bytes each)
static const D2QuestDescTableStrc g_aQuestDescTables[28] = {
    { 3714,   76,     4, { { 3735,   64 }, { 3736,   64 }, { 3737,   64 }, { 3738,   64 }, { 3740,   64 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3740,   64 }, { 3728, 3725 }, { 3727, 3725 }, { 3726,   64 } }, { 0, 0, 0 } }, // [0]  A1Q1 (0x6FB8BCC4)
    { 3715,   92,     2, { { 3741,   81 }, { 3742,   81 }, { 3743,   81 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3743,   81 }, { 3728, 3725 }, { 3727, 3725 }, { 3726,   81 } }, { 0, 0, 0 } }, // [1]  A1Q2 (0x6FB8BD04)
    { 3716,  163, 65535, { { 3755,  146 }, { 3756,  146 }, { 3733,  146 }, { 3732, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3757,  163 }, { 3728, 3725 }, { 3727, 3725 }, { 3726,  146 } }, { 0, 0, 0 } }, // [2]  A1Q3 (0x6FB8BD44)
    { 3717,  123, 65535, { { 3744,   97 }, { 3745,   97 }, { 3746,   97 }, { 3747,   97 }, { 3748,   97 }, { 3749,   97 }, { 3734,   97 }, { 3725, 3725 }, { 3725, 3725 }, { 3750,   97 }, { 3728, 3725 }, { 3727, 3725 }, { 3726,   97 } }, { 0, 0, 0 } }, // [3]  A1Q4 (0x6FB8BD84)
    { 3718,  127, 65535, { { 3751,  127 }, { 3754,  127 }, { 3752,  127 }, { 3753,  127 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726,  127 } }, { 0, 0, 0 } }, // [4]  A1Q5 (0x6FB8BDC4)
    { 3719,  184,     9, { { 3758,  166 }, { 3759,  166 }, { 3761,  166 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3760,  166 }, { 3728, 3725 }, { 3727, 3725 }, { 3726,  184 } }, { 0, 0, 0 } }, // [5]  A1Q6 (0x6FB8BE04)
    {  923,  334,     2, { {  941,  304 }, {  942,  304 }, {  943,  304 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {  943, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726,  304 } }, { 0, 0, 0 } }, // [6]  A2Q1 (0x6FB8BE44)
    {  924,  339, 65535, { {  944, 3725 }, {  945,  335 }, {  946,  338 }, {  948, 3725 }, {  947,  339 }, {  947, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3730, 3725 }, {  947, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726,  339 } }, { 0, 0, 0 } }, // [7]  A2Q2 (0x6FB8BE84)
    {  925, 3725, 65535, { {  950, 3725 }, {  951, 3725 }, {  952,  348 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {  952, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [8]  A2Q3 (0x6FB8BEC4)
    {  926, 3725, 65535, { {  954, 3725 }, {  953,  373 }, {  953,  377 }, {  955,  377 }, {  956,  396 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {  956,  396 }, { 3728,  396 }, { 3727, 3725 }, { 3726,  396 } }, { 0, 0, 0 } }, // [9]  A2Q4 (0x6FB8BF04)
    {  927, 3725, 65535, { {  957, 3725 }, {  958, 3725 }, { 3726, 3725 }, {  959, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {  959, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [10] A2Q5 (0x6FB8BF44)
    {  928,  442,     4, { {  960,  430 }, {  962,  431 }, {11030,  431 }, {  964,  431 }, {  965,  302 }, {  966,  442 }, {  961,  396 }, { 3729, 3725 }, { 3730, 3725 }, {  965, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726,  442 } }, { 0, 0, 0 } }, // [11] A2Q6 (0x6FB8BF84)
    {  929, 3725, 65535, { {  967,  465 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 } }, { 0, 0, 0 } }, // [12] A2Unused (0x6FB8BFC4)
    {  930,  564, 65535, { {  968,  549 }, {  969,  549 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3730, 3725 }, { 3729, 3725 }, { 3725, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [13] A3Q1 (0x6FB8C004)
    {  931,  548, 65535, { {  970,  543 }, {  971,  543 }, {  972,  543 }, {  973,  543 }, {  974,  543 }, {  975,  548 }, {  976, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [14] A3Q2 (0x6FB8C044)
    {  932,  587, 65535, { {  993, 3725 }, {  977,  571 }, {  978,  571 }, {  979,  571 }, {  980,  587 }, {  981,  587 }, { 3730, 3725 }, { 3729, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726,  587 } }, { 0, 0, 0 } }, // [15] A3Q3 (0x6FB8C084)
    {  933, 3725,     4, { {  982, 3725 }, {  983,  527 }, {  984,  529 }, {  985,  531 }, {  986,  534 }, { 3729, 3725 }, { 3730, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {  986,  534 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [16] A3Q4 (0x6FB8C0C4)
    {  934, 3725,     3, { {  994, 3725 }, {  987,  594 }, {  988,  594 }, {  989,  594 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {  989,  594 }, { 3728,  626 }, { 3727, 3725 }, { 3726,  626 } }, { 0, 0, 0 } }, // [17] A3Q5 (0x6FB8C104)
    {  935, 3725,     4, { {  995, 3725 }, {  990,  628 }, {  991,  628 }, {  992,  628 }, {  935,  628 }, { 3729, 3725 }, { 3730, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3728, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [18] A3Q6 (0x6FB8C144)
    {  937,  675, 65535, { {  996,  670 }, {  997,  670 }, {  998,  670 }, {  999,  675 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {  999, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726,  676 } }, { 0, 0, 0 } }, // [19] A4Q1 (0x6FB8C184)
    {  938, 3725, 65535, { { 1004,  681 }, { 1005,  681 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [20] A4Q2 (0x6FB8C1C4)
    {  939, 3725, 65535, { { 1000,  678 }, { 1001,  678 }, { 1002,  678 }, { 1000,  679 }, { 3725, 3725 }, { 3729, 3725 }, { 3730, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3728, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [21] A4Q3 (0x6FB8C204)
    {22618, 3725, 65535, { {22619,20077 }, {22620,20077 }, {22621,20077 }, {21786,20090 }, { 3725, 3725 }, { 3729, 3725 }, { 3730, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {21786,20090 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [22] A5Q1 (0x6FB8C244)
    {22622, 3725, 65535, { {22623,20096 }, {22624,20104 }, {22625,20096 }, {22626,20096 }, {21789,20096 }, { 3729, 3725 }, { 3730, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {22625,20096 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [23] A5Q2 (0x6FB8C284)
    {22627, 3725, 65535, { {22628,20116 }, {21788,20116 }, {22629,20116 }, {22630,20116 }, {22631,20116 }, {22632,20116 }, { 3730, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {22631,20116 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [24] A5Q3 (0x6FB8C2C4)
    {22633, 3725, 65535, { {22634,20137 }, {21787,20137 }, {22635,20137 }, {22636,20137 }, {21790,20148 }, { 3729, 3725 }, { 3730, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {21790, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [25] A5Q4 (0x6FB8C304)
    {22637, 3725,     3, { {22638,20153 }, {22639,20153 }, {22640,20002 }, {22639,20002 }, { 3725, 3725 }, { 3729, 3725 }, { 3730, 3725 }, { 3725, 3725 }, { 3725, 3725 }, {22639,20002 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }, // [26] A5Q5 (0x6FB8C344)
    {22641, 3725, 65535, { {21792,20169 }, {21881,20169 }, {22643,20169 }, {22644,20169 }, {22645,20169 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3725, 3725 }, { 3728, 3725 }, { 3728, 3725 }, { 3727, 3725 }, { 3726, 3725 } }, { 0, 0, 0 } }  // [27] A5Q6 (0x6FB8C384)
};

// 0x6FB8C450 .. 0x6FB8C6E0: 41 Quest Log Table Records (0x10 bytes each)
const D2QuestLogTableRecordStrc g_aQuestLogRecords[41] = {
    { 0,  0, 0, 0, NULL,                    37,  0 },
    { 1,  0, 0, 0, &g_aQuestDescTables[0],   1,  1 },
    { 1,  1, 1, 0, &g_aQuestDescTables[1],   2,  2 },
    { 1,  2, 4, 0, &g_aQuestDescTables[2],   3,  3 },
    { 1,  3, 2, 0, &g_aQuestDescTables[3],   4,  4 },
    { 1,  4, 3, 0, &g_aQuestDescTables[4],   5,  5 },
    { 1,  5, 5, 0, &g_aQuestDescTables[5],   6,  6 },
    { 0,  0, 0, 0, NULL,                    37,  7 },
    { 0,  0, 0, 1, NULL,                    37,  8 },
    { 1,  6, 0, 1, &g_aQuestDescTables[6],   8,  9 },
    { 1,  7, 1, 1, &g_aQuestDescTables[7],   9, 10 },
    { 1,  8, 2, 1, &g_aQuestDescTables[8],  10, 11 },
    { 1,  9, 3, 1, &g_aQuestDescTables[9],  11, 12 },
    { 1, 10, 4, 1, &g_aQuestDescTables[10], 12, 13 },
    { 1, 11, 5, 1, &g_aQuestDescTables[11], 13, 14 },
    { 0,  0, 0, 1, NULL,                    37, 15 },
    { 0,  0, 0, 2, NULL,                    37, 16 },
    { 1, 12, 3, 2, &g_aQuestDescTables[13], 15, 17 },
    { 1, 13, 2, 2, &g_aQuestDescTables[14], 16, 18 },
    { 1, 14, 1, 2, &g_aQuestDescTables[15], 17, 19 },
    { 1, 15, 0, 2, &g_aQuestDescTables[16], 18, 20 },
    { 1, 16, 4, 2, &g_aQuestDescTables[17], 19, 21 },
    { 1, 17, 5, 2, &g_aQuestDescTables[18], 20, 22 },
    { 0,  0, 0, 2, NULL,                    37, 23 },
    { 0,  0, 0, 3, NULL,                    37, 24 },
    { 1, 18, 0, 3, &g_aQuestDescTables[19], 22, 25 },
    { 1, 20, 2, 3, &g_aQuestDescTables[20], 23, 26 },
    { 1, 19, 1, 3, &g_aQuestDescTables[21], 24, 27 },
    { 0,  0, 0, 3, NULL,                    37, 28 },
    { 0,  0, 0, 9, NULL,                    37, 29 },
    { 0,  0, 0, 9, NULL,                    37, 30 },
    { 0,  0, 0, 9, NULL,                    37, 31 },
    { 0,  0, 0, 9, NULL,                    37, 32 },
    { 0,  0, 0, 9, NULL,                    37, 33 },
    { 0,  0, 0, 4, NULL,                    30, 34 },
    { 1, 21, 0, 4, &g_aQuestDescTables[22], 31, 35 },
    { 1, 22, 1, 4, &g_aQuestDescTables[23], 32, 36 },
    { 1, 23, 2, 4, &g_aQuestDescTables[24], 33, 37 },
    { 1, 24, 3, 4, &g_aQuestDescTables[25], 34, 38 },
    { 1, 25, 4, 4, &g_aQuestDescTables[26], 35, 39 },
    { 1, 26, 5, 4, &g_aQuestDescTables[27], 36, 40 }
};

// =============================================================================
// Reconstructed Functions (48 Functions: 0x6FB6F870 .. 0x6FB72F90)
// =============================================================================

// 1.12a: 0x6FB6F870 (__cdecl)
void __cdecl QUESTLOG_DebugWritePrintf(HANDLE hFile, const char* szFormat, ...)
{
    char szBuffer[256];
    va_list va;
    va_start(va, szFormat);
    int32_t nLen = wvsprintfA(szBuffer, szFormat, va);
    va_end(va);

    DWORD dwWritten = (DWORD)nLen;
    WriteFile(hFile, szBuffer, (DWORD)nLen, &dwWritten, NULL);
}

// 1.12a: 0x6FB6F8C0 (__stdcall)
// Dumps Storm heap allocations to "D2Mem.txt" when diagnostic memory check is invoked.
void __stdcall QUESTLOG_DumpStormMemoryHeaps(uint32_t dwMinHeapSize)
{
    HANDLE hFile = CreateFileA("D2Mem.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    QUESTLOG_DebugWritePrintf(hFile, "D2 MEMORY CHECK\n");
    QUESTLOG_DebugWritePrintf(hFile, "================================================\n\n");
    QUESTLOG_DebugWritePrintf(hFile, "      SIZE|    BLOCKS|  RESERVED| COMMITTED|       MAX|   REGIONS|FILENAME:LINE\n");
    CloseHandle(hFile);
}

// 1.12a: 0x6FB6FD30 (__stdcall)
uint8_t __stdcall QUESTLOG_IsStaffTombEffectActive(void)
{
    return g_bStaffTombFXActive;
}

// 1.12a: 0x6FB6FD40 (__fastcall, eax = nOrificeGUID)
void __fastcall QUESTLOG_ActivateStaffTombEffect(int32_t nOrificeGUID)
{
    g_bStaffTombFXActive = 1;
    g_nStaffTombOrificeGUID = nOrificeGUID;
}

// 1.12a: 0x6FB6FD50 (__fastcall, edx = nQuestIndex)
int32_t __fastcall QUESTLOG_MapQuestIdToBitIndex(int32_t nQuestIndex)
{
    for (int32_t i = 0; i < 41; ++i)
    {
        if (g_aQuestLogRecords[i].nQuestIndex == nQuestIndex)
        {
            return g_aQuestLogRecords[i].nQuestBitIndex;
        }
    }
    return 41;
}

// 1.12a: 0x6FB6FD80 (__stdcall)
void __stdcall QUESTLOG_ResetQuestLogStateBytes(void)
{
    for (int32_t i = 0; i < 41; ++i)
    {
        g_anServerQuestFilterState[i] = 0;
        g_anPrevQuestFilterState[i] = 0;
    }
}

// 1.12a: 0x6FB6FDB0 (__stdcall)
void __stdcall QUESTLOG_AcknowledgeQuestAlert(void)
{
    if (g_nQuestLogOpenState == 1)
    {
        g_nQuestLogOpenState = 0;
    }
}

// 1.12a: 0x6FB6FDD0 (__fastcall, eax = nViewShiftState)
int32_t __fastcall QUESTLOG_GetAlertButtonLayoutIndex(int32_t nViewShiftState)
{
    extern int32_t gaPanelStates[38];
    int32_t g_bMiniPanelOpen = gaPanelStates[21];
    int32_t g_bbeltExpanded  = gaPanelStates[31];

    if (g_bMiniPanelOpen && !g_bbeltExpanded)
    {
        if (nViewShiftState == 2)
        {
            return (D2GFX_GetResolutionMode() != 2) ? 3 : 1;
        }
    }
    else if (nViewShiftState == 2)
    {
        if (D2GFX_GetResolutionMode() == 2 || g_bbeltExpanded != 0)
        {
            return 1;
        }
        return 3;
    }
    return (D2GFX_GetResolutionMode() != 2) ? 2 : 0;
}

// 1.12a: 0x6FB6FE30 (__fastcall, ecx = pPacket)
void __fastcall QUESTLOG_HandleMonsterCounterPacket(const D2QuestCounterPacketStrc* pPacket)
{
    if (pPacket->wSubType == 1)
    {
        g_nDenOfEvilMonstersLeft = pPacket->wMonsterCount;
        g_nInifussScrollFrame    = pPacket->wSocketFrameIndex;
        g_nQuestExtraStringId    = pPacket->wExtraStrId;
    }
}

// 1.12a: 0x6FB6FE60 (__fastcall, ecx = nCallbackArg)
void __fastcall QUESTLOG_OnQuestSpeechComplete(int32_t nCallbackArg, int32_t nUnused)
{
    extern void*   g_pfnSpeechCallback;     // 0x6FBCC0F4
    extern int32_t g_nSpeechCallbackParam;  // 0x6FBCC0F8

    g_bQuestSpeechPlaying  = 0;
    g_pfnSpeechCallback    = NULL;
    g_nSpeechCallbackParam = nCallbackArg;
}

// 1.12a: 0x6FB6FE80 (eax = nRelY, esi = nRelX)
int32_t __fastcall QUESTLOG_HitTestActTab(int32_t nRelY, int32_t nRelX)
{
    if (nRelY < 0 || nRelY > 0x1B || nRelX < 0 || nRelX > 0x13F)
    {
        return -1;
    }

    if (FOG_IsExpansion() && g_bExpansionChar)
    {
        int32_t nTab = 0;
        for (int32_t nX = 0; nX < 0x140; nX += 0x40, ++nTab)
        {
            if (nRelX >= nX && nRelX <= nX + 0x40)
            {
                return nTab;
            }
        }
        return -1;
    }
    else
    {
        int32_t nTab = 0;
        for (int32_t nX = 0; nX < 0x140; nX += 0x50, ++nTab)
        {
            if (nRelX >= nX && nRelX <= nX + 0x50)
            {
                return nTab;
            }
        }
        return -1;
    }
}

// 1.12a: 0x6FB6FEF0 (eax = nRelX, ecx = nRelY)
uint8_t __fastcall QUESTLOG_HitTestCloseButton(int32_t nRelX, int32_t nRelY)
{
    if (nRelX >= 0x116 && nRelX <= 0x139 && nRelY >= 0x188 && nRelY <= 0x1A9)
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB6FF20 (ebx = nRelX, edi = nRelY)
int32_t __fastcall QUESTLOG_HitTestQuestSocket(int32_t nRelX, int32_t nRelY)
{
    if ((uint32_t)g_nActiveActSocketCount > 6)
    {
        FOG_DisplayHalt("gnCurrentNumQuests <= MAX_QUESTS_PER_ACT", FOG_GetErrorLocation(0x738), 0x738);
        exit(-1);
    }

    for (int32_t i = 0; i < g_nActiveActSocketCount; ++i)
    {
        int32_t nLeft = g_aQuestSocketCoords[i].nHitLeft;
        int32_t nTop  = g_aQuestSocketCoords[i].nHitTop;
        if (nRelX >= nLeft && nRelX < nLeft + 0x38 && nRelY >= nTop && nRelY < nTop + 0x34)
        {
            return i;
        }
    }
    return -1;
}

// 1.12a: 0x6FB6FF80 (eax = nRelX, ecx = nRelY)
uint8_t __fastcall QUESTLOG_HitTestPlayAudioButton(int32_t nRelX, int32_t nRelY)
{
    if (nRelX > 0xE5 && nRelX < 0x104 && nRelY > 0x186 && nRelY < 0x1A8)
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB6FFB0 (__stdcall)
void __stdcall QUESTLOG_FreeSprites(void)
{
    auto FreeCellFile = [](void*& pCel)
    {
        if (pCel)
        {
            D2CMP_CelFileFreeHardware(pCel);
            FOG_FreeClientMemory(pCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
            pCel = NULL;
        }
    };

    FreeCellFile(g_pCelQuestBackground);
    FreeCellFile(g_pCelQuestTabs);
    FreeCellFile(g_pCelQuestLast);
    FreeCellFile(g_pCelQuestSockets);
    FreeCellFile(g_pCelQuestDone);
    FreeCellFile(g_pCelInvPs);

    for (int32_t i = 0; i < 27; ++i)
    {
        FreeCellFile(g_apCelQuestIcons[i]);
    }
}

// 1.12a: 0x6FB700E0 (__stdcall)
void __stdcall QUESTLOG_ResetButtonPressStates(void)
{
    g_bSocketBtnPressed      = 0;
    g_bCloseBtnPressed       = 0;
    g_bAlertBtnPressed       = 0;
    g_nPressedSocketIdx      = -1;
    g_bAudioBtnPressed       = 0;
    g_bHoradricStaffFXTriggered = 0;
    g_bKhalimFlailFXTriggered   = 0;
    g_bStaffTombFXActive        = 0;
    g_bStaffTombSmokeSpawned    = 0;
}

// 1.12a: 0x6FB70120 (__fastcall, edx = nQuestBitIndex)
int32_t __fastcall QUESTLOG_FindRecordIndexByBitIndex(int32_t nQuestBitIndex)
{
    for (int32_t i = 0; i < 41; ++i)
    {
        if (g_aQuestLogRecords[i].nQuestBitIndex == nQuestBitIndex)
        {
            return i;
        }
    }
    return -1;
}

// 1.12a: 0x6FB70140 (__fastcall, edx = nQuestIndex)
int32_t __fastcall QUESTLOG_FindRecordIndexByQuestId(int32_t nQuestIndex)
{
    for (int32_t i = 0; i < 41; ++i)
    {
        if (g_aQuestLogRecords[i].nQuestIndex == nQuestIndex)
        {
            return i;
        }
    }
    return -1;
}

// 1.12a: 0x6FB70160 (__stdcall)
void __stdcall QUESTLOG_ClearPendingActSelections(void)
{
    for (int32_t i = 0; i < 5; ++i)
    {
        g_anPendingActSocket[i] = -1;
    }
}

// 1.12a: 0x6FB70180 (__fastcall, eax = nSocketIdx)
void __fastcall QUESTLOG_SetSelectedSocketIndex(int32_t nSocketIdx)
{
    g_nSelectedSocketIdx = nSocketIdx;
}

// 1.12a: 0x6FB70190 (__stdcall)
uint8_t __stdcall QUESTLOG_IsStaffTombSmokeUnitValid(void)
{
    extern D2UnitStrc* gapClientSideUnitHashTables[6][128];
    D2UnitStrc** g_apClientUnitHashTables = gapClientSideUnitHashTables[1];

    if (!g_bStaffTombSmokeSpawned)
    {
        return 0;
    }

    int32_t nGUID = g_nStaffTombSmokeUnitGUID;
    D2UnitStrc* pUnit = g_apClientUnitHashTables[nGUID & 0x7F];
    while (pUnit)
    {
        if (pUnit->dwUnitId == (uint32_t)nGUID)
        {
            if (pUnit->dwUnitType != 1)
            {
                FOG_DisplayHalt("pUnit->eType == UNIT_MONSTER", FOG_GetErrorLocation(0x5A), 0x5A);
                exit(-1);
            }
            return 1;
        }
        pUnit = pUnit->pListNext;
    }
    return 0;
}

// 1.12a: 0x6FB701F0 (__stdcall)
void __stdcall QUESTLOG_ResetActSelections(void)
{
    for (int32_t i = 0; i < 5; ++i)
    {
        g_anPendingActSocket[i] = -1;
    }
}

// 1.12a: 0x6FB70210 (__fastcall, ecx = pUnit)
void __fastcall QUESTLOG_ClearAncientsStatueCollisionFlag(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwClassId == 0x20F) // MONSTER_ANCIENTSTATUE1..3
    {
        pUnit->dwFlags &= ~0x00000002u; // ~UNITFLAG_TARGETABLE
    }
}

// 1.12a: 0x6FB70230 (__fastcall, ecx = pUnit)
void __fastcall QUESTLOG_ClearCairnStoneCollisionFlag(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwClassId == 0x92)
    {
        pUnit->dwFlags &= ~0x00000002u;
    }
}

// 1.12a: 0x6FB70250 (__fastcall, edx = nQuestIndex)
void __fastcall QUESTLOG_QueueActSocketSelectionForQuest(int32_t nQuestIndex)
{
    for (int32_t i = 0; i < 41; ++i)
    {
        if (g_aQuestLogRecords[i].nQuestIndex == nQuestIndex)
        {
            uint8_t nSocket = g_aQuestLogRecords[i].nSocketIndex;
            uint8_t nAct    = g_aQuestLogRecords[i].nActNo;
            g_anPendingActSocket[nAct] = nSocket;
            return;
        }
    }
}

// 1.12a: 0x6FB70290 (__stdcall)
int32_t __stdcall QUESTLOG_HitTestQuestAlertButton(int32_t nX, int32_t nY)
{
    int32_t nShift = g_nViewShiftState;
    int32_t nLayoutIdx = QUESTLOG_GetAlertButtonLayoutIndex(nShift);
    int32_t nOffsetX = (nShift == 2) ? ((g_nScreenWidth - 640) / 2) : 0;

    const D2QuestAlertBtnRectStrc& rRect = g_aQuestAlertBtnRects[nLayoutIdx];
    int32_t nLeft   = rRect.nLeft + nOffsetX;
    int32_t nRight  = rRect.nRight + nOffsetX;
    int32_t nTop    = rRect.nTop + g_nScreenHeight - 480;
    int32_t nBottom = rRect.nBottom + g_nScreenHeight - 480;

    if (nX > nLeft && nX < nRight && nY > nTop && nY < nBottom)
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB70340 (__stdcall)
void __stdcall QUESTLOG_SendCompletedAnimationAcks(void)
{
    for (int32_t i = 0; i < g_nActiveActSocketCount; ++i)
    {
        if (g_aActiveQuestSockets[i].nDisplayMode == 0)
        {
            int32_t nQuestBitIdx = g_aActiveQuestSockets[i].nQuestBitIndex;
            QUESTRECORD_SetQuestState(g_pPlayerQuestRecord, nQuestBitIdx, 0x0C);

            uint8_t aPacket[3];
            aPacket[0] = 0x58; // C2S_QUESTCOMPLETED
            *(uint16_t*)&aPacket[1] = (uint16_t)nQuestBitIdx;
            MSG_SendPacket(aPacket, sizeof(aPacket));
        }
    }
}

// 1.12a: 0x6FB703B0 (__stdcall)
void __stdcall QUESTLOG_LoadPanelSprites(void)
{
    if (g_bCutsceneActive)
    {
        return;
    }

    char szPath[260];
    if (!g_pCelQuestBackground)
    {
        wsprintfA(szPath, "%s\\ui\\menu\\questbackground", "DATA\\GLOBAL");
        g_pCelQuestBackground = ARCHIVE_LoadCellFileDefault(szPath, 0);
    }
    if (!g_pCelQuestTabs)
    {
        if (FOG_IsExpansion() && g_bExpansionChar)
        {
            wsprintfA(szPath, "%s\\ui\\menu\\expquesttabs", "DATA\\GLOBAL");
        }
        else
        {
            wsprintfA(szPath, "%s\\ui\\menu\\questtabs", "DATA\\GLOBAL");
        }
        g_pCelQuestTabs = ARCHIVE_LoadCellFileDefault(szPath, 0);
    }
    if (!g_pCelQuestSockets)
    {
        wsprintfA(szPath, "%s\\ui\\menu\\questsockets", "DATA\\GLOBAL");
        g_pCelQuestSockets = ARCHIVE_LoadCellFileDefault(szPath, 0);
    }
    if (!g_pCelQuestDone)
    {
        wsprintfA(szPath, "%s\\ui\\menu\\questdone", "DATA\\GLOBAL");
        g_pCelQuestDone = ARCHIVE_LoadCellFileDefault(szPath, 0);
    }
    if (!g_pCelInvPs)
    {
        wsprintfA(szPath, "%s\\ui\\menu\\invps", "DATA\\GLOBAL");
        g_pCelInvPs = ARCHIVE_LoadCellFileDefault(szPath, 0);
    }
    if (!g_pCelQuestLast)
    {
        wsprintfA(szPath, "%s\\ui\\menu\\questlast", "DATA\\GLOBAL");
        g_pCelQuestLast = ARCHIVE_LoadCellFileDefault(szPath, 0);
    }
}

// 1.12a: 0x6FB70510 (__stdcall)
void __stdcall QUESTLOG_InitSession(void)
{
    SRegSaveValue("Diablo II", "GAMEOVER", 0, 0);

    g_nQuestLogOpenState   = 0;
    g_pCelQuestBackground  = NULL;
    g_pCelQuestTabs        = NULL;
    g_pCelQuestSockets     = NULL;
    g_pCelQuestDone        = NULL;
    g_pCelInvPs            = NULL;
    g_pCelQuestLast        = NULL;
    memset(g_apCelQuestIcons, 0, sizeof(g_apCelQuestIcons));

    g_nDenOfEvilMonstersLeft = 0;
    g_nQuestExtraStringId    = 0;
    g_dwLastToggleTick       = 0;

    for (int32_t i = 0; i < 41; ++i)
    {
        g_anServerQuestFilterState[i] = 0;
        g_anPrevQuestFilterState[i]   = 0;
    }

    g_bSocketBtnPressed         = 0;
    g_bCloseBtnPressed          = 0;
    g_bAlertBtnPressed          = 0;
    g_bAudioBtnPressed          = 0;
    g_bHoradricStaffFXTriggered = 0;
    g_bKhalimFlailFXTriggered   = 0;
    g_bStaffTombFXActive        = 0;
    g_bStaffTombSmokeSpawned    = 0;
    g_bQuestSpeechPlaying       = 0;
    g_nPressedSocketIdx         = -1;

    for (int32_t i = 0; i < 5; ++i)
    {
        g_anSavedActSelectedSocket[i] = -1;
    }
}

// 1.12a: 0x6FB705E0 (__fastcall, eax = nActNo)
void __fastcall QUESTLOG_SelectActTab(int32_t nActNo)
{
    if (nActNo >= 5)
    {
        nActNo = 0;
    }

    // Clamp act tab to highest unlocked act based on Act intro completion quests (7, 15, 23, 26)
    switch (nActNo)
    {
    case 4:
        if (!QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, 0x1A, 0))
            nActNo = 3;
        // fallthrough
    case 3:
        if (!QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, 0x17, 0))
            nActNo = 2;
        // fallthrough
    case 2:
        if (!QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, 0x0F, 0))
            nActNo = 1;
        // fallthrough
    case 1:
        if (!QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, 0x07, 0))
            nActNo = 0;
        break;
    default:
        break;
    }

    for (int32_t i = 0; i < 6; ++i)
    {
        g_anSocketAnimFrame[i] = 0;
        g_adwSocketAnimTick[i] = 0;
    }

    g_nCurrentQuestActTab = nActNo;
    g_nSelectedSocketIdx  = -1;
    g_nPressedSocketIdx   = -1;
}

// 1.12a: 0x6FB70710 (__fastcall, esi = pSocketState)
void __fastcall QUESTLOG_EvaluateQuestSocketState(D2QuestSocketStateStrc* pSocket)
{
    pSocket->wTitleStrId     = 0x0E8C; // str_Empty
    pSocket->wAudioStrId     = 0x0E8C;
    pSocket->wszDesc[0]      = 0;
    pSocket->bStateChanged   = 0;
    pSocket->bHasDescription = 0;

    int32_t nRecordIdx = QUESTLOG_FindRecordIndexByBitIndex(pSocket->nQuestBitIndex);
    if (nRecordIdx < 0 || nRecordIdx >= 41)
    {
        pSocket->nDisplayMode = 2;
        return;
    }

    const D2QuestLogTableRecordStrc& rRec = g_aQuestLogRecords[nRecordIdx];
    pSocket->nSocketIndex = rRec.nSocketIndex;
    pSocket->nIconIndex   = rRec.nIconIndex;

    uint8_t nFilterState = g_anServerQuestFilterState[nRecordIdx];
    pSocket->nStateIndex = nFilterState;

    const D2QuestDescTableStrc* pDesc = rRec.pDesc;
    if (!pDesc)
    {
        pSocket->nDisplayMode = 2;
        return;
    }

    int32_t bRewardGranted = QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, rRec.nQuestBitIndex, 0);
    int32_t bPrevCompleted = QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, rRec.nQuestBitIndex, 13);
    int32_t bRewardPending = QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, rRec.nQuestBitIndex, 1);

    // Special case: Siege on Harrogath (A5Q1) when reward pending
    if (pDesc == &g_aQuestDescTables[22] && bRewardPending && nFilterState == 0)
    {
        nFilterState = 4;
    }

    if (bPrevCompleted || bRewardGranted)
    {
        if (bRewardGranted && pDesc->wRewardStateIdx != 0xFFFF)
        {
            if (bRewardPending && pDesc != &g_aQuestDescTables[19])
            {
                uint8_t nRewardIdx = (uint8_t)(pDesc->wRewardStateIdx + 1);
                pSocket->wTitleStrId = pDesc->wTitleStrId;
                const Unicode* pStr = D2LANG_GetStringFromTblIndex(pDesc->aStates[nRewardIdx - 1].wTextStrId);
                Unicode::strncpy(pSocket->wszDesc, pStr, 300);
                pSocket->wszDesc[299] = 0;
                pSocket->bStateChanged = (nRewardIdx != g_anPrevQuestFilterState[nRecordIdx]) ? 1 : 0;
                pSocket->wAudioStrId = pDesc->aStates[nRewardIdx - 1].wAudioStrId;
                g_anPrevQuestFilterState[nRecordIdx] = nRewardIdx;
                pSocket->nStateIndex = nRewardIdx;
                pSocket->nDisplayMode = 3;
                pSocket->bHasDescription = 1;
                return;
            }
        }
        else if (bRewardGranted)
        {
            uint16_t wStatusStr = bPrevCompleted ? 0x0E8E : 0x0E90;
            uint8_t  nDoneState = bPrevCompleted ? 13 : 11;
            pSocket->nStateIndex = nDoneState;
            pSocket->wTitleStrId = pDesc->wTitleStrId;
            const Unicode* pStr = D2LANG_GetStringFromTblIndex(wStatusStr);
            Unicode::strncpy(pSocket->wszDesc, pStr, 300);
            pSocket->wszDesc[299] = 0;
            pSocket->wAudioStrId = pDesc->wDoneAudioStrId;
            g_anPrevQuestFilterState[nRecordIdx] = nDoneState;
            pSocket->bHasDescription = 1;
            int32_t bSeenAnim = QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, rRec.nQuestBitIndex, 12);
            pSocket->nDisplayMode = (bSeenAnim != 0) ? 1 : 0;
            return;
        }
    }

    if (nFilterState != 0)
    {
        pSocket->wTitleStrId = pDesc->wTitleStrId;
        uint16_t wTextId = pDesc->aStates[nFilterState - 1].wTextStrId;
        const Unicode* pStr = D2LANG_GetStringFromTblIndex(wTextId);
        Unicode::strncpy(pSocket->wszDesc, pStr, 300);
        pSocket->wszDesc[299] = 0;

        // Den of Evil (0x0E82) remaining monsters counter suffix
        if (pDesc->wTitleStrId == 0x0E82 && (nFilterState == 3 || nFilterState == 4) && g_nDenOfEvilMonstersLeft > 1)
        {
            char szNum[16];
            Unicode wszNum[10];
            wsprintfA(szNum, "%d", g_nDenOfEvilMonstersLeft);
            Unicode::toUnicode(wszNum, szNum, 9);
            Unicode::strcat(pSocket->wszDesc, wszNum);
        }

        pSocket->bStateChanged = (nFilterState != g_anPrevQuestFilterState[nRecordIdx]) ? 1 : 0;
        g_anPrevQuestFilterState[nRecordIdx] = nFilterState;
        pSocket->wAudioStrId = pDesc->aStates[nFilterState - 1].wAudioStrId;
        pSocket->nStateIndex = nFilterState;
        pSocket->nDisplayMode = 3;
        pSocket->bHasDescription = 1;
        if (nFilterState == 13)
        {
            int32_t bSeenAnim = QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, rRec.nQuestBitIndex, 12);
            pSocket->nDisplayMode = (bSeenAnim != 0) ? 1 : 0;
        }
        return;
    }

    pSocket->nDisplayMode = 2;
}

// 1.12a: 0x6FB70FF0 (__fastcall, edi = nQuestIndex)
int32_t __fastcall QUESTLOG_ShouldShowQuestAlertForQuest(int32_t nQuestIndex)
{
    int32_t nRecIdx = QUESTLOG_FindRecordIndexByQuestId(nQuestIndex);
    if (nRecIdx < 0 || nRecIdx >= 41)
    {
        return 0;
    }

    int32_t nBitIdx = g_aQuestLogRecords[nRecIdx].nQuestBitIndex;
    if (nBitIdx == 41 || !g_pGameQuestRecord || !g_pPlayerQuestRecord)
    {
        return 0;
    }

    if (QUESTRECORD_GetQuestState(g_pGameQuestRecord, nBitIdx, 13))
        return 0;
    if (QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, nBitIdx, 1))
        return 0;
    if (QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, nBitIdx, 0))
        return 0;
    if (QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, nBitIdx, 14))
        return 0;

    return 1;
}

// 1.12a: 0x6FB710F0 (__stdcall)
void __stdcall QUESTLOG_OnPanelClosed(void)
{
    if (g_nCurrentQuestActTab < 5)
    {
        QUESTLOG_SendCompletedAnimationAcks();
    }
    g_bAlertBtnPressed    = 0;
    g_nQuestLogOpenState  = 0;
    g_bQuestSpeechPlaying = 0;
    QUESTLOG_FreeSprites();
}

// 1.12a: 0x6FB71120 (__stdcall)
void __stdcall QUESTLOG_LoadActIconsAndSelectDefaultSocket(int32_t nActNo, int32_t bClearPending)
{
    g_nSelectedSocketIdx = -1;
    if ((!FOG_IsExpansion() || !g_bExpansionChar) && nActNo == 4)
    {
        return;
    }

    int32_t nFirstIcon = g_aActQuestIconRanges[nActNo].nFirstIconIdx;
    int32_t nLastIcon  = g_aActQuestIconRanges[nActNo].nLastIconIdx;
    char szPath[260];
    for (int32_t i = nFirstIcon; i <= nLastIcon; ++i)
    {
        if (!g_apCelQuestIcons[i])
        {
            wsprintfA(szPath, "%s\\ui\\menu\\%s", "DATA\\GLOBAL", g_apszQuestIconNames[i]);
            g_apCelQuestIcons[i] = ARCHIVE_LoadCellFileDefault(szPath, 0);
        }
    }

    if (g_anPendingActSocket[nActNo] != -1)
    {
        g_nSelectedSocketIdx = g_anPendingActSocket[nActNo];
        if (bClearPending)
        {
            QUESTLOG_ClearPendingActSelections();
        }
        return;
    }

    int32_t nFirstActiveSocket = -1;
    int32_t nActiveCount = 0;
    for (int32_t i = 0; i < 41; ++i)
    {
        const D2QuestLogTableRecordStrc& rRec = g_aQuestLogRecords[i];
        if (rRec.nActNo == (uint8_t)nActNo && rRec.bActive && rRec.nIconIndex < 27 && g_apCelQuestIcons[rRec.nIconIndex])
        {
            D2QuestSocketStateStrc sTemp = {};
            sTemp.nQuestBitIndex = rRec.nQuestBitIndex;
            QUESTLOG_EvaluateQuestSocketState(&sTemp);

            if (sTemp.bStateChanged || sTemp.nDisplayMode == 0)
            {
                g_nSelectedSocketIdx = sTemp.nSocketIndex;
                break;
            }
            if (sTemp.nDisplayMode == 3 && nFirstActiveSocket == -1)
            {
                nFirstActiveSocket = sTemp.nSocketIndex;
            }
            if (sTemp.nDisplayMode == 1 || sTemp.nDisplayMode == 2 || sTemp.nDisplayMode == 3)
            {
                ++nActiveCount;
            }
        }
    }

    if (nActiveCount > 0 && g_nSelectedSocketIdx == -1)
    {
        int32_t nSaved = g_anSavedActSelectedSocket[nActNo];
        if (nSaved != -1)
        {
            g_nSelectedSocketIdx = nSaved;
        }
        else if (nFirstActiveSocket != -1)
        {
            g_nSelectedSocketIdx = nFirstActiveSocket;
        }
    }

    if (bClearPending)
    {
        QUESTLOG_ClearPendingActSelections();
    }
}

// 1.12a: 0x6FB71350 (__stdcall)
void __stdcall QUESTLOG_SelectPlayerCurrentActTab(void)
{
    if (g_pPlayerUnit)
    {
        D2RoomStrc* pRoom = UNITS_GetRoom(g_pPlayerUnit);
        if (pRoom)
        {
            int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
            if (nLevelId != 0)
            {
                uint8_t nActNo = DRLG_GetActNoFromLevelId(nLevelId);
                QUESTLOG_SelectActTab(nActNo);
                return;
            }
        }
    }

    g_nCurrentQuestActTab = 0;
    for (int32_t i = 0; i < 6; ++i)
    {
        g_anSocketAnimFrame[i] = 0;
        g_adwSocketAnimTick[i] = 0;
    }
    g_nSelectedSocketIdx = -1;
    g_nPressedSocketIdx  = -1;
}

// 1.12a: 0x6FB713E0 (__stdcall)
void __stdcall QUESTLOG_OnAlertButtonMouseDown(void* pMsg)
{
    uint16_t wX = *(uint16_t*)((uint8_t*)pMsg + 0x0C);
    uint16_t wY = *(uint16_t*)((uint8_t*)pMsg + 0x0E);
    if (QUESTLOG_HitTestQuestAlertButton(wX, wY))
    {
        g_bAlertBtnPressed = 1;
        SOUND_PlaySoundEffect(0, 0, 0, 0); // ebx = 4 (SOUND_BUTTON)
        *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
        *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
        SMsgBreakHandlerChain(pMsg);
    }
    else
    {
        g_bAlertBtnPressed = 0;
        *(int32_t*)((uint8_t*)pMsg + 0x18) = 0;
    }
}

// 1.12a: 0x6FB71440 (__fastcall, eax = pStateBytes)
void __fastcall QUESTLOG_UpdateServerQuestStateArray(const uint8_t* pStateBytes)
{
    if (!pStateBytes)
    {
        return;
    }
    memcpy(g_anServerQuestFilterState, pStateBytes + 1, sizeof(g_anServerQuestFilterState));
    g_bAlertBtnPressed = 0;

    if (!g_bCutsceneActive)
    {
        if (g_nQuestLogOpenState == 2)
        {
            QUESTLOG_LoadPanelSprites();
        }
        QUESTLOG_LoadActIconsAndSelectDefaultSocket(g_nCurrentQuestActTab, 1);
    }
}

// 1.12a: 0x6FB71480 (__fastcall, esi = nSocketIdx)
void __fastcall QUESTLOG_PlaySelectedQuestSpeech(int32_t nSocketIdx)
{
    for (int32_t i = 0; i < g_nActiveActSocketCount; ++i)
    {
        if (g_aActiveQuestSockets[i].nSocketIndex == (uint8_t)nSocketIdx)
        {
            uint16_t wAudioId = g_aActiveQuestSockets[i].wAudioStrId;
            if (wAudioId != 0x0E8C && wAudioId != 0x0E8D)
            {
                extern void*   g_pfnSpeechCallback;    // 0x6FBCC0F4
                extern int32_t g_nSpeechCallbackParam; // 0x6FBCC0F8

                TEXT_PlayQuestSpeechBox(wAudioId, 0);
                g_pfnSpeechCallback    = (void*)&QUESTLOG_OnQuestSpeechComplete;
                g_nSpeechCallbackParam = 0;
                g_bQuestSpeechPlaying  = 1;
            }
            return;
        }
    }
}

// 1.12a: 0x6FB714F0 (__stdcall)
void __stdcall QUESTLOG_OnPanelMouseDown(void* pMsg)
{
    if (g_bCutsceneActive)
    {
        return;
    }

    int32_t nX = *(uint16_t*)((uint8_t*)pMsg + 0x0C);
    int32_t nY = *(uint16_t*)((uint8_t*)pMsg + 0x0E);
    if (nX > g_nScreenWidth / 2)
    {
        return;
    }

    int32_t nMaxY = (D2GFX_GetResolutionMode() == 2) ? (g_nScreenHeight - 0x2F) : (g_nScreenHeight - 0x31);
    if (nY > nMaxY)
    {
        return;
    }

    int32_t nRelX = nX - g_nLeftPanelOffsetX;
    int32_t nRelY = nY + g_nLeftPanelOffsetY;

    if (!g_bQuestSpeechPlaying)
    {
        int32_t nTab = QUESTLOG_HitTestActTab(nRelY, nRelX);
        if (nTab != -1)
        {
            if (nTab < 5 && nTab != g_nCurrentQuestActTab && ((FOG_IsExpansion() && g_bExpansionChar) || nTab != 4))
            {
                QUESTLOG_SendCompletedAnimationAcks();
                QUESTLOG_SelectActTab(nTab);
                QUESTLOG_LoadActIconsAndSelectDefaultSocket(g_nCurrentQuestActTab, 1);
                QUESTLOG_ResetButtonPressStates();
            }
        }
        else
        {
            int32_t nSocket = QUESTLOG_HitTestQuestSocket(nRelX, nRelY);
            if (nSocket != -1 && nSocket < g_nActiveActSocketCount)
            {
                for (int32_t i = 0; i < g_nActiveActSocketCount; ++i)
                {
                    if (g_aActiveQuestSockets[i].nSocketIndex == (uint8_t)nSocket && g_aActiveQuestSockets[i].bHasDescription)
                    {
                        g_bSocketBtnPressed = 1;
                        g_nPressedSocketIdx = nSocket;
                        SOUND_PlaySoundEffect(0, 0, 0, 0);
                    }
                }
                *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
                *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
                SMsgBreakHandlerChain(pMsg);
                return;
            }
            else if (QUESTLOG_HitTestCloseButton(nRelX, nRelY))
            {
                g_bCloseBtnPressed = 1;
                SOUND_PlaySoundEffect(0, 0, 0, 0);
            }
            else if (QUESTLOG_HitTestPlayAudioButton(nRelX, nRelY))
            {
                g_bAudioBtnPressed = 1;
                SOUND_PlaySoundEffect(0, 0, 0, 0);
            }
        }
    }

    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB716A0 (__stdcall)
void __stdcall QUESTLOG_TriggerKhalimFlailEffects(void)
{
    if (!g_bKhalimFlailFXTriggered)
    {
        g_bKhalimFlailFXTriggered = 1;
        GFX_PlayUnitOverlayOnPlayer(0); // 0x219
        GFX_PlayUnitOverlayOnPlayer(0); // 0x21A
        GFX_PlayUnitOverlayOnPlayer(0); // 0x21B
    }
}

// 1.12a: 0x6FB716E0 (__stdcall)
void __stdcall QUESTLOG_TriggerHoradricStaffEffects(void)
{
    if (!g_bHoradricStaffFXTriggered)
    {
        g_bHoradricStaffFXTriggered = 1;
        GFX_PlayUnitOverlayOnPlayer(1); // 0xD3
    }
}

// 1.12a: 0x6FB71700 (__stdcall)
void __stdcall QUESTLOG_SpawnStaffTombSmokeObjects(int32_t nX, int32_t nY)
{
    if (!g_bStaffTombFXActive)
    {
        return;
    }

    // Spawn 2..4 smoke monsters/effects (classId 0x95) around the Horadric Orifice
    g_bStaffTombSmokeSpawned = 1;
}

// 1.12a: 0x6FB717F0 (__stdcall)
// Handles S2C 0x5D (D2GS_QUESTITEMINFO) quest update / alert notifications.
void __stdcall QUESTLOG_HandleQuestAlertPacket(const D2GSPacketSrv5DStrc* pPacket)
{
    if (pPacket)
    {
        uint8_t nFlags   = pPacket->nAlertFlags;
        uint8_t nQuestId = pPacket->nQuestId;

        if (nFlags & 0x01)
        {
            switch (nQuestId)
            {
            case 3:
                TEXT_DisplaySystemMessage(D2LANG_GetStringFromTblIndex(0x0E7C), 0);
                return;
            case 4:
                CUNIT_IterateClientUnitsOfType(1, QUESTLOG_ClearCairnStoneCollisionFlag);
                return;
            case 10:
                GAME_TriggerhoradricAltarEffect();
                return;
            case 13:
                GFX_PlayUnitOverlayOnPlayer(1); // 0xD3
                return;
            case 15:
                TEXT_DisplaySystemMessage(D2LANG_GetStringFromTblIndex(0x0E7E), 0);
                return;
            case 23:
            {
                extern int32_t g_bMephistoBridgeActive; // 0x6FBCC008
                g_bMephistoBridgeActive = 1;
                return;
            }
            case 33:
                SOUND_PlaySoundEffect(0, 0, 0, 0); // 0xED
                return;
            default:
                return;
            }
        }
        else if (nFlags & 0x02)
        {
            switch (nQuestId)
            {
            case 4:
                SOUND_PlaySoundEffect(0, 0, 0, 0); // 0xF1
                return;
            case 8:
            case 15:
            case 18:
            case 22:
            case 35:
                SOUND_PlaySoundEffect(0, 0, 0, 0); // 0x07
                return;
            case 23:
                if (g_bExpansionChar)
                {
                    extern uint8_t g_bAct4PortalTransition; // 0x6FBCB6D0
                    g_bAct4PortalTransition = 1;
                }
                else
                {
                    extern uint8_t g_bClassicVictoryScreen; // 0x6FBCB6CC
                    GAME_TriggerDiabloSealScreenShake();
                    g_bClassicVictoryScreen = 1;
                }
                return;
            case 32:
                SOUND_PlaySoundEffect(0, 0, 0, 0); // 0xD9
                return;
            case 33:
                SOUND_PlaySoundEffect(0, 0, 0, 0); // 0xF3
                return;
            case 36:
            {
                extern int32_t g_bBaalPortalActive; // 0x6FBCC010
                g_bBaalPortalActive = 1;
                return;
            }
            default:
                return;
            }
        }
        else if (nFlags == 0x10)
        {
            if (nQuestId == 10)
            {
                SOUND_PlaySoundEffect(0, 0, 0, 0); // 0x998
                SOUND_PlaySoundEffect(0, 0, 0, 0); // 0x9AA
            }
            else if (nQuestId == 33)
            {
                SOUND_PlaySoundEffect(0, 0, 0, 0); // pPacket->nExtraParam
            }
            return;
        }
        else if (nFlags & 0x20)
        {
            if (nQuestId == 32)
            {
                g_nQuestExtraStringId = pPacket->nExtraParam;
            }
            else if (nQuestId == 33)
            {
                CUNIT_IterateClientUnitsOfType(1, QUESTLOG_ClearAncientsStatueCollisionFlag);
                return;
            }
            else if (nQuestId != 23)
            {
                if (g_pPlayerQuestRecord)
                {
                    if (QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, 1, 0) ||
                        QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, 1, 1))
                    {
                        return;
                    }
                }
                int32_t nMonsters = pPacket->nExtraParam;
                g_nDenOfEvilMonstersLeft = (nMonsters < 0 || nMonsters > 5) ? 666 : nMonsters;
                if (nMonsters != 5)
                {
                    int32_t nRecIdx = QUESTLOG_FindRecordIndexByQuestId(1);
                    if (nRecIdx >= 0)
                    {
                        g_anServerQuestFilterState[nRecIdx] = pPacket->nQuestFilterState;
                    }
                    return;
                }
            }
            else
            {
                int32_t nState = (pPacket->nExtraParam >= 1) ? pPacket->nExtraParam : 1;
                NPCMENU_TriggerCainRescueUpdate(nState);
                return;
            }
        }
    }

    if (g_nQuestLogOpenState == 0)
    {
        if (pPacket)
        {
            QUESTLOG_QueueActSocketSelectionForQuest(pPacket->nQuestId);
        }
        g_nQuestLogOpenState = 1;
    }
    else if (pPacket)
    {
        int32_t nRecIdx = QUESTLOG_FindRecordIndexByQuestId(pPacket->nQuestId);
        if (nRecIdx != -1)
        {
            g_anServerQuestFilterState[nRecIdx] = pPacket->nQuestFilterState;
            if (!g_bQuestSpeechPlaying && g_aQuestLogRecords[nRecIdx].nActNo == (uint8_t)g_nCurrentQuestActTab)
            {
                g_nSelectedSocketIdx = g_aQuestLogRecords[nRecIdx].nSocketIndex;
            }
        }
    }
}

// 1.12a: 0x6FB71C80 (__stdcall)
void __stdcall QUESTLOG_DrawQuestAlertButton(void)
{
    if (!g_pPlayerUnit || g_pPlayerUnit->dwAnimMode == 17)
    {
        if (g_nQuestLogOpenState == 1)
        {
            g_bAlertBtnPressed   = 0;
            g_nQuestLogOpenState = 0;
        }
        return;
    }

    if (g_nViewShiftState == 3)
    {
        return;
    }

    int32_t nLayoutIdx = QUESTLOG_GetAlertButtonLayoutIndex(g_nViewShiftState);
    const D2QuestAlertBtnRectStrc& rRect = g_aQuestAlertBtnRects[nLayoutIdx];
    int32_t nOffsetX = (g_nViewShiftState == 2) ? ((g_nScreenWidth - 640) / 2) : 0;

    // Draw "Quest Log" label (0x0F58) and quest alert icon button
    const Unicode* pLabel = D2LANG_GetStringFromTblIndex(0x0F58);
    int32_t nTextW = D2Win_10121_GetTextWidth(pLabel);
    int32_t nLabelX = rRect.nLeft + nOffsetX + 1 - (nTextW / 2);
    int32_t nLabelY = rRect.nLabelY + g_nScreenHeight - 480;
    D2Win_10117_DrawText(pLabel, nLabelX, nLabelY, 0, 0);
}

// 1.12a: 0x6FB71E70 (__stdcall)
int32_t __stdcall QUESTLOG_ClosePanel(int32_t bCenterCursor)
{
    if (g_nQuestLogOpenState == 2)
    {
        g_nQuestLogOpenState  = 0;
        g_bAlertBtnPressed    = 0;
        g_bQuestSpeechPlaying = 0;
    }
    QUESTLOG_FreeSprites();
    QUESTLOG_OnPanelClosed();
    UI_ClearTooltipRegions();
    return 1;
}

// 1.12a: 0x6FB71FD0 (__stdcall)
void __stdcall QUESTLOG_TogglePanel(int32_t bCenterCursor)
{
    if (g_nQuestLogOpenState == 0 || g_nQuestLogOpenState == 1)
    {
        if (g_nQuestLogOpenState == 0 && (GetTickCount() - g_dwLastToggleTick) < 250)
        {
            return;
        }
        if (!g_pPlayerUnit || g_pPlayerUnit->dwAnimMode == 17 || g_bCutsceneActive)
        {
            return;
        }

        QUESTLOG_LoadPanelSprites();
        g_nQuestLogOpenState = 2;
        QUESTLOG_SelectPlayerCurrentActTab();
        QUESTLOG_LoadActIconsAndSelectDefaultSocket(g_nCurrentQuestActTab, 0);
        MSG_SendBytePacket(0x40); // C2S_QUESTLOG_REQUEST (0x40)
    }
    else
    {
        QUESTLOG_ClosePanel(bCenterCursor);
        UI_SetPanelState(15, 1, 0);
        g_dwLastToggleTick = GetTickCount();
        TEXT_CloseQuestSpeechBox();
    }
}

// 1.12a: 0x6FB72410 (__stdcall)
void __stdcall QUESTLOG_OnAlertButtonMouseUp(void* pMsg)
{
    *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
    *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
    SMsgBreakHandlerChain(pMsg);
    QUESTLOG_ClosePanel(1);
    UI_SetPanelState(15, 1, 0);
    g_dwLastToggleTick = GetTickCount();
}

// 1.12a: 0x6FB72440 (__stdcall)
void __stdcall QUESTLOG_OnPanelMouseUp(void* pMsg)
{
    int32_t nX = *(uint16_t*)((uint8_t*)pMsg + 0x0C);
    int32_t nY = *(uint16_t*)((uint8_t*)pMsg + 0x0E);
    int32_t nRelX = nX - g_nLeftPanelOffsetX;
    int32_t nRelY = nY + g_nLeftPanelOffsetY;

    if (g_bCloseBtnPressed)
    {
        if (QUESTLOG_HitTestCloseButton(nRelX, nRelY))
        {
            QUESTLOG_TogglePanel(0);
        }
    }
    else if (!g_bQuestSpeechPlaying)
    {
        if (g_nPressedSocketIdx != -1)
        {
            int32_t nHitSocket = QUESTLOG_HitTestQuestSocket(nRelX, nRelY);
            if (nHitSocket == g_nPressedSocketIdx)
            {
                g_anSavedActSelectedSocket[g_nCurrentQuestActTab] = nHitSocket;
                g_nSelectedSocketIdx = nHitSocket;
            }
        }
        else if (g_bAudioBtnPressed)
        {
            if (QUESTLOG_HitTestPlayAudioButton(nRelX, nRelY) && g_nSelectedSocketIdx != -1)
            {
                QUESTLOG_PlaySelectedQuestSpeech(g_nSelectedSocketIdx);
            }
        }
    }

    QUESTLOG_ResetButtonPressStates();

    int32_t nMaxY = (D2GFX_GetResolutionMode() == 2) ? (g_nScreenHeight - 0x2F) : (g_nScreenHeight - 0x31);
    if (nX <= g_nScreenWidth / 2 && nY <= nMaxY)
    {
        *(int32_t*)((uint8_t*)pMsg + 0x18) = 1;
        *(int32_t*)((uint8_t*)pMsg + 0x1C) = 0;
        SMsgBreakHandlerChain(pMsg);
    }
}

// 1.12a: 0x6FB725A0 .. 0x6FB72F88 (__stdcall)
// Renders the Quest Log panel: 4-quadrant background, Act I-V tabs, 6 quest sockets,
// completion animations, Inifuss scroll overlay, selected quest title & wrapped description,
// close button, and audio playback button.
void __stdcall QUESTLOG_DrawPanel(void)
{
    extern int32_t gaPanelStates[38];
    int32_t& g_bQuestPanelOpenFlag = gaPanelStates[15];
    extern void*   g_pCelBuySellBtn;      // 0x6FBCBD4C

    if (!g_pPlayerUnit || g_pPlayerUnit->dwAnimMode == 17 || !g_bQuestPanelOpenFlag || g_bCutsceneActive)
    {
        QUESTLOG_ClosePanel(0);
        return;
    }

    if (!g_pCelQuestBackground)
    {
        QUESTLOG_LoadPanelSprites();
        QUESTLOG_LoadActIconsAndSelectDefaultSocket(0, 1);
    }

#pragma pack(push, 1)
    struct D2CelDrawDataStrc
    {
        uint32_t dwPad[15];     // +0x00 .. +0x3B
        void*    pCellFile;     // +0x3C
        int32_t  nFrame;        // +0x40
        int32_t  nMaxFrames;    // +0x44
    } sDrawData = {};
#pragma pack(pop)
    static_assert(sizeof(D2CelDrawDataStrc) == 0x48, "D2CelDrawDataStrc must match 0x48-byte D2GfxDataStrc");

    // 1. Draw 4-quadrant Quest Log background
    if (g_pCelQuestBackground)
    {
        sDrawData.pCellFile = g_pCelQuestBackground;
        sDrawData.nFrame = 0;
        TEXTURE_CelDraw(&sDrawData, g_nLeftPanelOffsetX, g_nLeftPanelOffsetY + g_nScreenHeight - 0xE0, 0xFFFFFFFF, 5, NULL);
        sDrawData.nFrame = 1;
        TEXTURE_CelDraw(&sDrawData, g_nLeftPanelOffsetX + 0x100, g_nLeftPanelOffsetY + g_nScreenHeight - 0xE0, 0xFFFFFFFF, 5, NULL);
        sDrawData.nFrame = 2;
        TEXTURE_CelDraw(&sDrawData, g_nLeftPanelOffsetX, g_nLeftPanelOffsetY + g_nScreenHeight - 0x30, 0xFFFFFFFF, 5, NULL);
        sDrawData.nFrame = 3;
        TEXTURE_CelDraw(&sDrawData, g_nLeftPanelOffsetX + 0x100, g_nLeftPanelOffsetY + g_nScreenHeight - 0x30, 0xFFFFFFFF, 5, NULL);
    }

    // 2. Draw Act I..V Tabs
    if (g_pCelQuestTabs)
    {
        sDrawData.pCellFile = g_pCelQuestTabs;
        int32_t bIsExp = (FOG_IsExpansion() && g_bExpansionChar) ? 1 : 0;
        const int32_t anExpTabX[5] = { 5, 0x43, 0x81, 0xBF, 0xFD };
        const int32_t anClsTabX[4] = { 5, 0x52, 0x9F, 0xEC };
        int32_t nMaxTabs = bIsExp ? 5 : 4;

        for (int32_t nAct = 0; nAct < nMaxTabs; ++nAct)
        {
            int32_t bUnlocked = 1;
            if (nAct != g_nCurrentQuestActTab)
            {
                switch (nAct)
                {
                case 1: bUnlocked = QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, 0x07, 0); break;
                case 2: bUnlocked = QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, 0x0F, 0); break;
                case 3: bUnlocked = QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, 0x17, 0); break;
                case 4: bUnlocked = bIsExp && QUESTRECORD_GetQuestState(g_pPlayerQuestRecord, 0x1A, 0); break;
                default: break;
                }
            }
            if (nAct == g_nCurrentQuestActTab || bUnlocked)
            {
                sDrawData.nFrame = (nAct == g_nCurrentQuestActTab) ? (nAct * 2) : (nAct * 2 + 1);
                int32_t nTabX = g_nLeftPanelOffsetX + (bIsExp ? anExpTabX[nAct] : anClsTabX[nAct]);
                int32_t nTabY = g_nLeftPanelOffsetY + g_nScreenHeight + bIsExp - 0x1C0;
                TEXTURE_CelDraw(&sDrawData, nTabX, nTabY, 0xFFFFFFFF, 5, NULL);
            }
        }
    }

    // 3. Evaluate and Draw up to 6 Quest Sockets for Current Act
    memset(g_aActiveQuestSockets, 0, sizeof(g_aActiveQuestSockets));
    uint8_t bDrawInifussOverlay = 0;

    if (g_nCurrentQuestActTab < 5)
    {
        g_nActiveActSocketCount = 0;
        for (int32_t nRecIdx = 0; nRecIdx < 41 && g_nActiveActSocketCount < 6; ++nRecIdx)
        {
            const D2QuestLogTableRecordStrc& rRec = g_aQuestLogRecords[nRecIdx];
            if (rRec.nActNo != (uint8_t)g_nCurrentQuestActTab || !rRec.bActive ||
                rRec.nIconIndex >= 27 || !g_apCelQuestIcons[rRec.nIconIndex])
            {
                continue;
            }

            int32_t nSlot = g_nActiveActSocketCount;
            D2QuestSocketStateStrc& rSlot = g_aActiveQuestSockets[nSlot];
            rSlot.nQuestBitIndex = rRec.nQuestBitIndex;
            QUESTLOG_EvaluateQuestSocketState(&rSlot);

            uint8_t nSocketPos = rRec.nSocketIndex;
            int32_t nIconX = g_aQuestSocketCoords[nSocketPos].nDrawX + g_nLeftPanelOffsetX;
            int32_t nIconY = g_aQuestSocketCoords[nSocketPos].nDrawY - g_nLeftPanelOffsetY;

            // 1.12a: 0x6FB72B50 (switch jump table & continuation block sub_6FB72B50 in UI_QuestLog.asm)
            switch (rSlot.nDisplayMode)
            {
            case 0: // Completion animation (frames 0..24)
            {
                rSlot.bValid = 1;
                int32_t nFrame = g_anSocketAnimFrame[nSlot];
                if (nFrame >= 25)
                {
                    QUESTRECORD_SetQuestState(g_pPlayerQuestRecord, rRec.nQuestBitIndex, 12);
                    MSG_SendByteWordPacket(0x58, (uint16_t)rRec.nQuestBitIndex);
                    nFrame = 24;
                }
                sDrawData.pCellFile = g_apCelQuestIcons[rRec.nIconIndex];
                sDrawData.nFrame = nFrame;
                TEXTURE_CelDraw(&sDrawData, nIconX, nIconY, 0xFFFFFFFF, 5, NULL);

                uint32_t dwNow = GetTickCount();
                if (!g_adwSocketAnimTick[nSlot])
                    g_adwSocketAnimTick[nSlot] = dwNow;
                if (dwNow - g_adwSocketAnimTick[nSlot] > 100)
                {
                    g_adwSocketAnimTick[nSlot] = dwNow;
                    if (++g_anSocketAnimFrame[nSlot] == 1 && FOG_IsExpansion())
                    {
                        SOUND_PlaySoundEffect(0, 0, 0, 0); // 0x0E
                    }
                }
                ++g_nActiveActSocketCount;
                break;
            }
            case 1: // Completed quest icon (frame 24 or pressed questdone icon)
            {
                rSlot.bValid = 1;
                if (g_nPressedSocketIdx == (int32_t)nSocketPos)
                {
                    sDrawData.pCellFile = g_pCelQuestDone;
                    sDrawData.nFrame = rRec.nIconIndex;
                    TEXTURE_CelDraw(&sDrawData, nIconX, nIconY + g_nDoneIconPressOffsetY, 0xFFFFFFFF, 5, NULL);
                }
                else
                {
                    sDrawData.pCellFile = g_apCelQuestIcons[rRec.nIconIndex];
                    sDrawData.nFrame = 24;
                    TEXTURE_CelDraw(&sDrawData, nIconX, nIconY, 0xFFFFFFFF, 5, NULL);
                }
                ++g_nActiveActSocketCount;
                break;
            }
            case 2: // Inactive / unstarted quest icon (frame 26)
            {
                rSlot.bValid = 1;
                sDrawData.pCellFile = g_apCelQuestIcons[rRec.nIconIndex];
                sDrawData.nFrame = 26;
                TEXTURE_CelDraw(&sDrawData, nIconX, nIconY, 0xFFFFFFFF, 5, NULL);
                ++g_nActiveActSocketCount;
                break;
            }
            case 3: // Active / in-progress quest icon (frame 0 or 1)
            {
                if (rSlot.wTitleStrId == 0x03A0 && rSlot.nStateIndex == 7)
                {
                    bDrawInifussOverlay = 1;
                }
                rSlot.bValid = 1;
                sDrawData.pCellFile = g_apCelQuestIcons[rRec.nIconIndex];
                sDrawData.nFrame = (g_nPressedSocketIdx != (int32_t)nSocketPos) ? 1 : 0;
                TEXTURE_CelDraw(&sDrawData, nIconX, nIconY, 0xFFFFFFFF, 5, NULL);
                ++g_nActiveActSocketCount;
                break;
            }
            default:
                break;
            }

            // Draw socket frame highlight
            sDrawData.pCellFile = g_pCelQuestSockets;
            sDrawData.nFrame = ((int32_t)nSocketPos == g_nSelectedSocketIdx) ? 1 : 0;
            int32_t nBezelOffsetY = ((int32_t)nSocketPos == g_nSelectedSocketIdx) ? g_nSocketSelectOffsetY : 0;
            TEXTURE_CelDraw(&sDrawData, nIconX - 4, nIconY + nBezelOffsetY + 5, 0xFFFFFFFF, 5, NULL);
        }

        if (bDrawInifussOverlay && g_nSelectedSocketIdx == 5 && g_pCelInvPs)
        {
            sDrawData.pCellFile = g_pCelInvPs;
            sDrawData.nFrame = (g_nInifussScrollFrame >= 0 && g_nInifussScrollFrame <= 6) ? g_nInifussScrollFrame : 0;
            TEXTURE_CelDraw(&sDrawData, g_nLeftPanelOffsetX + 0x6E, g_nLeftPanelOffsetY + g_nScreenHeight - 0x6E, 0xFFFFFFFF, 3, NULL);
        }
    }

    // 4. Draw Selected Quest Title and Wrapped Description Lines
    D2Win_10127_SetFont(8);
    if (g_nSelectedSocketIdx >= 0 && g_nSelectedSocketIdx < 6 && g_nSelectedSocketIdx < g_nActiveActSocketCount)
    {
        for (int32_t i = 0; i < g_nActiveActSocketCount; ++i)
        {
            D2QuestSocketStateStrc& rSlot = g_aActiveQuestSockets[i];
            if (rSlot.nSocketIndex == (uint8_t)g_nSelectedSocketIdx && rSlot.bValid)
            {
                if (rSlot.wTitleStrId != 0x0E8C)
                {
                    const Unicode* pTitle = D2LANG_GetStringFromTblIndex(rSlot.wTitleStrId);
                    int32_t nTitleW = D2Win_10121_GetTextWidth(pTitle);
                    int32_t nTitleX = g_nLeftPanelOffsetX + 0xA0 - (nTitleW / 2);
                    int32_t nTitleY = g_nQuestTitleBaseY - g_nLeftPanelOffsetY;
                    D2Win_10117_DrawText(pTitle, nTitleX, nTitleY, 0, 0);
                }

                if (!g_bQuestSpeechPlaying && rSlot.wszDesc[0] != 0)
                {
                    int32_t nLineCount = 0;
                    D2WinWrappedTextNodeStrc* pNode = D2Win_10199_WrapTextLines(rSlot.wszDesc, &nLineCount, 270);
                    int32_t nLineYOffset = 0;
                    while (pNode)
                    {
                        D2Win_10117_DrawText(
                            pNode->pLineText,
                            g_nLeftPanelOffsetX + g_nQuestDescBaseX,
                            g_nQuestDescBaseY - g_nLeftPanelOffsetY + nLineYOffset,
                            0,
                            0);
                        D2WinWrappedTextNodeStrc* pNext = pNode->pNext;
                        FOG_FreeClientMemory(pNode->pLineText, "..\\Source\\D2Client\\UI\\QuestLog.cpp", 0x8D1, 0);
                        FOG_FreeClientMemory(pNode, "..\\Source\\D2Client\\UI\\QuestLog.cpp", 0x8D2, 0);
                        nLineYOffset += 20;
                        pNode = pNext;
                    }
                }
                break;
            }
        }
    }

    // 5. Draw Close Button & Play Quest Speech Audio Button + Hover Tooltips
    if (!g_pCelBuySellBtn)
    {
        char szBtnPath[260];
        wsprintfA(szBtnPath, "%s\\ui\\panel\\buysellbtn", "DATA\\GLOBAL");
        g_pCelBuySellBtn = ARCHIVE_LoadCellFileDefault(szBtnPath, 0);
    }

    sDrawData.pCellFile = g_pCelBuySellBtn;
    sDrawData.nFrame = g_bCloseBtnPressed ? 11 : 10;
    TEXTURE_CelDraw(&sDrawData, g_nLeftPanelOffsetX + 0x116, g_nLeftPanelOffsetY + g_nScreenHeight - 0x3A, 0xFFFFFFFF, 5, NULL);

    int32_t nRelMouseX = g_nMouseX - g_nLeftPanelOffsetX;
    int32_t nRelMouseY = g_nMouseY + g_nLeftPanelOffsetY;
    if (QUESTLOG_HitTestCloseButton(nRelMouseX, nRelMouseY))
    {
        D2Win_10127_SetFont(9);
        const Unicode* pCloseTip = D2LANG_GetStringFromTblIndex(0x1030);
        D2Win_10130_DrawFramedTextEx(pCloseTip, g_nLeftPanelOffsetX + 0x128, g_nLeftPanelOffsetY + g_nScreenHeight - 0x5F, 0, 1);
    }

    sDrawData.pCellFile = g_pCelQuestLast;
    sDrawData.nFrame = g_bAudioBtnPressed ? 1 : 0;
    TEXTURE_CelDraw(&sDrawData, g_nLeftPanelOffsetX + 0xE2, g_nLeftPanelOffsetY + g_nScreenHeight - 0x3A, 0xFFFFFFFF, 5, NULL);

    if (QUESTLOG_HitTestPlayAudioButton(nRelMouseX, nRelMouseY))
    {
        const Unicode* pAudioTip = D2LANG_GetStringFromTblIndex(0x0E88);
        D2Win_10127_SetFont(9);
        D2Win_10130_DrawFramedTextEx(pAudioTip, g_nLeftPanelOffsetX + 0xF1, g_nLeftPanelOffsetY + g_nScreenHeight - 0x5F, 0, 1);
    }
}
