// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Slice: GAME/Game.cpp Core Loop
// Address Range: 0x6FB29CD0 .. 0x6FB2D85F (157 procedures + SMsg & Data Tables)
// Source Path:   "..\Source\D2Client\GAME\Game.cpp" (0x6FB84A84)
// ImageBase:     0x6FAB0000
// ============================================================================

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef D2_VERSION_MAJOR
#define D2_VERSION_112A 1
#define D2_VERSION_MAJOR 1
#define D2_VERSION_MINOR 12
#define D2_VERSION_PATCH 'A'
#endif

#include <windows.h>
#include <tlhelp32.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <algorithm>
#include <queue>
#include <vector>

#include <D2BasicTypes.h>
#include <D2Constants.h>
#include <D2Config.h>
#include <BnClient.h>
#include <Fog.h>
#include <Storm.h>
#include <D2Lang.h>
#include <D2StrTable.h>
#include <D2Unicode.h>
#include <D2Gfx.h>
#include <Texture.h>
#include <Window.h>
#include <D2CMP.h>
#include <D2Sound.h>
#include <D2WinMain.h>
#include <D2WinComp.h>
#include <D2WinFont.h>
#include <D2WinPalette.h>
#include <D2DataTbls.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/LevelsTbls.h>
#include <DataTbls/TokenTbls.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <D2Environment.h>
#include <D2Inventory.h>
#include <D2Items.h>
#include <D2Log.h>
#include <D2QuestRecord.h>
#include <D2StatList.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgActivate.h>
#include <Units/UnitRoom.h>
#include <Units/Units.h>
#include <Units/Player.h>
#include <Client.h>
#include <D2MCPClient.h>
#include <PLAYER/PlrSave.h>

#include "D2ClientReconstructed.h"

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#ifndef WM_XBUTTONDOWN
#define WM_XBUTTONDOWN 0x020B
#endif
#ifndef WM_XBUTTONUP
#define WM_XBUTTONUP 0x020C
#endif

// ============================================================================
// Typed Internal Client Structures
// ============================================================================

// Client state machine IDs indexed into g_apfnClientStateHandlers[7] (0x6FBA8B34)
enum D2ClientAppState : int32_t
{
    D2CLIENT_STATE_NONE        = 0, // 0x6FBA8B34: nullptr
    D2CLIENT_STATE_EXIT        = 1, // 0x6FBA8B38: nullptr (terminates D2Client_Main loop)
    D2CLIENT_STATE_GAME_LOOP   = 2, // 0x6FBA8B3C: 0x6FB2D2D0 (GAME_MainLoop)
    D2CLIENT_STATE_WAIT_JOIN   = 3, // 0x6FBA8B40: 0x6FB2A690 (GAME_WaitJoinScreen, initial state)
    D2CLIENT_STATE_PREPARE_NET = 4, // 0x6FBA8B44: 0x6FB2BC80 (GAME_LeaveGameCleanupState)
    D2CLIENT_STATE_START_NET   = 5, // 0x6FBA8B48: 0x6FAEBBF0 (connect / join network session)
    D2CLIENT_STATE_LEAVE_GAME  = 6, // 0x6FBA8B4C: 0x6FAEBBB0 (close network session)
};

#pragma pack(push, 1)
struct D2PongPacketSrv8FStrc
{
    uint8_t  nHeader;          // +0x00 (0x8F)
    uint32_t adwPongDwords[8]; // +0x01..+0x20
};
static_assert(sizeof(D2PongPacketSrv8FStrc) == 0x21, "D2PongPacketSrv8FStrc must be 33 bytes");
#pragma pack(pop)

enum D2SMsgDispatchType : uint32_t
{
    SMSG_DISPATCH_MESSAGE = 0,
    SMSG_DISPATCH_COMMAND = 1,
    SMSG_DISPATCH_KEYUP   = 2,
    SMSG_DISPATCH_KEYDOWN = 3,
};

struct D2SMsgRegEntryStrc
{
    uint32_t    nType;      // +0x00 (D2SMsgDispatchType)
    uint32_t    uMsgOrId;   // +0x04 (WM_* or VK_*)
    SMSGHANDLER pfHandler;  // +0x08
};
static_assert(sizeof(D2SMsgRegEntryStrc) == 0x0C, "D2SMsgRegEntryStrc must be 12 bytes");

#pragma pack(push, 1)
struct D2BetaScreenEntryStrc
{
    uint32_t        dwLoadTick;  // +0x00
    D2CellFileStrc* pCellFile;   // +0x04
    uint8_t         nActPalette; // +0x08
    const char*     szFileName;  // +0x09
};
static_assert(sizeof(D2BetaScreenEntryStrc) == 0x0D, "D2BetaScreenEntryStrc must be 13 bytes");

struct D2KeyBindingStrc
{
    int32_t  nCommandId; // +0x00
    uint16_t wKey;       // +0x04
    int32_t  bIsPrimary; // +0x06
};
static_assert(sizeof(D2KeyBindingStrc) == 0x0A, "D2KeyBindingStrc must be 10 bytes");
#pragma pack(pop)

struct D2ActSeedPairStrc
{
    uint32_t dwInitSeed; // +0x00
    uint32_t dwEndSeed;  // +0x04
};
static_assert(sizeof(D2ActSeedPairStrc) == 0x08, "D2ActSeedPairStrc must be 8 bytes");

enum D2ViewportFlags : uint32_t
{
    VIEWFLAG_DIRTY       = 0x00000001,
    VIEWFLAG_SHAKING     = 0x00000008,
    VIEWFLAG_ROOF_UPDATE = 0x00000010,
};

struct D2IsoSortNodeStrc
{
    int32_t            nNodeType;          // +0x00
    int32_t            nPrecisionSubtileX; // +0x04
    int32_t            nPrecisionSubtileY; // +0x08
    void*              pOwner;             // +0x0C
    D2IsoSortNodeStrc* pNext;              // +0x10
};

struct D2IsoSortCellStrc
{
    uint32_t           dwFlags;            // +0x00
    uint32_t           dwReserved04;       // +0x04
    int32_t            nTileX;             // +0x08
    int32_t            nTileY;             // +0x0C
    D2IsoSortNodeStrc* pFloorAndRoofList;  // +0x10
    D2IsoSortNodeStrc* pLowerWallList;     // +0x14
    D2IsoSortNodeStrc* pNormalUnitList;    // +0x18
    D2IsoSortNodeStrc* pSpecialWallList;   // +0x1C
    D2IsoSortNodeStrc* pUpperWallList;     // +0x20
};

struct D2IsoSortGridStrc
{
    uint32_t           dwGridFlags;        // +0x0000 (Viewport +0x0038)
    D2IsoSortNodeStrc  aNodes[3000];       // +0x0004 (Viewport +0x003C .. +0xEA9B)
    int32_t            nNodeCount;         // +0xEA64 (Viewport +0xEA9C)
    int32_t            nOriginTileX;       // +0xEA68 (Viewport +0xEAA0)
    int32_t            nOriginTileY;       // +0xEA6C (Viewport +0xEAA4)
    D2IsoSortCellStrc* pCells;             // +0xEA70 (Viewport +0xEAA8)
    int32_t            nTotalCellCount;    // +0xEA74 (Viewport +0xEAAC)
    int32_t            nGridDimension;     // +0xEA78 (Viewport +0xEAB0)
    int32_t            nWidthTiles;        // +0xEA7C (Viewport +0xEAB4)
    int32_t            nHeightTiles;       // +0xEA80 (Viewport +0xEAB8)
};

struct D2ViewportStrc
{
    uint32_t          dwFlags;            // +0x0000
    int32_t           nScreenLeft;        // +0x0004
    int32_t           nScreenTop;         // +0x0008
    int32_t           nScreenRight;       // +0x000C
    int32_t           nScreenBottom;      // +0x0010
    int32_t           nClipLeft;          // +0x0014
    int32_t           nClipTop;           // +0x0018
    int32_t           nClipRight;         // +0x001C
    int32_t           nClipBottom;        // +0x0020
    int32_t           nViewOffsetX;       // +0x0024
    int32_t           nViewOffsetY;       // +0x0028
    int32_t           nCameraTargetIsoX;  // +0x002C
    int32_t           nCameraTargetIsoY;  // +0x0030
    int32_t           nShakePacked;       // +0x0034
    D2IsoSortGridStrc tSortGrid;          // +0x0038 .. +0xEABB
};
static_assert(sizeof(D2ViewportStrc) == 0xEABC, "D2ViewportStrc must match retail 0xEABC-byte layout");

struct D2ClientTimingBlockStrc
{
    int32_t  nRenderFrameMode;       // 0x6FBB3298
    void     (__fastcall* pfnRenderFrame)(int32_t bSkipCursor); // 0x6FBB329C
    void*    pClientPoolSystem;      // 0x6FBB32A0
    uint32_t dwCurrentTickMs;        // 0x6FBB32A4
    uint32_t dwLastLogicTickMs;      // 0x6FBB32A8
    uint32_t dwRenderedFrameCount;   // 0x6FBB32AC
    uint32_t dwClientGameTick;       // 0x6FBB32B0
    uint32_t dwNextPingSendTick;     // 0x6FBB32B4
    uint32_t dwLastPingSentTick;     // 0x6FBB32B8
    uint32_t dwClientPing;           // 0x6FBB32BC
    int32_t  nMeasuredFps;           // 0x6FBB32C0
    int32_t  nFpsSampleFrames;       // 0x6FBB32C4
    int32_t  nSkippedFps;            // 0x6FBB32C8
    int32_t  nSkippedSampleFrames;   // 0x6FBB32CC
    uint32_t dwFpsSampleStartTick;   // 0x6FBB32D0
    uint32_t dwReassignGraceTick;    // 0x6FBB32D4
    uint32_t dwReserved32D8;         // 0x6FBB32D8
    int32_t  nServerStatusFlag;      // 0x6FBB32DC
    int32_t  nNetworkQualityMetric;  // 0x6FBB32E0
    uint32_t dwInitialPingCount;     // 0x6FBB32E4
    uint32_t dwAverageInitialPing;   // 0x6FBB32E8
    uint32_t adwPongPayload[8];      // 0x6FBB32EC .. 0x6FBB330B
    int32_t  bIsExpansion;           // 0x6FBB330C
    int32_t  bIsLadderClient;        // 0x6FBB3310
    int32_t  nAutoSkillState;        // 0x6FBB3314
};
static_assert(sizeof(D2ClientTimingBlockStrc) == 0x80, "D2ClientTimingBlockStrc must be 32 dwords (0x80 bytes)");

typedef void (__fastcall* D2ConfigApplyFn)(D2ConfigStrc* pCfg);
typedef void (__fastcall* D2RenderFrameFn)(int32_t bSkipCursor);

// ============================================================================
// External Cross-Module & Fog / D2Client Function Declarations
// ============================================================================

extern "C" const char* __cdecl    Fog_10265(int32_t nErrorCode);                      // Fog.#10265 (0x6FABBF8C)
inline int32_t FOG_10191(void) { return FOG_IsSlowLegacyCPU(); }
inline void    FOG_10061(void* pFixedPool) { DATATBLS_DestroyDataArray(static_cast<D2FogDataArrayStrc*>(pFixedPool)); }

// CORE / WINMAIN / ARCHIVE
extern void            __fastcall ARCHIVE_ReadMpqaFileBytes(HSFILE hFile, uint32_t nBytesToRead, void* pOutBuffer); // 0x6FABDA10
extern int32_t         __fastcall GAME_SetResolutionMode(int32_t nResolutionMode);    // 0x6FAC2630
extern D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileDefault(const char* szFileName, int32_t nFileType); // 0x6FB6EF70

// ENVIRONMENT / WEATHER / QUEST / CUTSCENE
extern int32_t      __stdcall  QUEST_SpawnDenOfEvilLightBeamInRoom(D2ActiveRoomStrc* pRoom); // 0x6FAC3A80
extern void         __stdcall  QUEST_UpdateEnvironmentEffects(void);                  // 0x6FAC3C50
extern void         __stdcall  WEATHER_InitEnvironmentPools(void);                    // 0x6FAC3F10
extern void         __stdcall  WEATHER_FreeEnvironmentCellFiles(void);                // 0x6FAC4460
extern void         __stdcall  WEATHER_UpdateParticles(void);                         // 0x6FAC5410
extern D2LevelsTxt* __stdcall  OBJ_SpawnRoomEnvSoundObjects(D2ActiveRoomStrc* pRoom); // 0x6FAC7320
extern int32_t      __stdcall  CUTSCENE_PlayActMovie(int32_t nMovieIndex);            // 0x6FAC7770

// UNIT / PLAYER / UNITMODE
extern void         __stdcall  D2Client_UpdateObjectsOnPeriodOfDayChange(void);       // 0x6FACFDB0
extern void         __stdcall  D2Client_FreeAllUnits(void);                           // 0x6FAD0730
extern void         __fastcall D2Client_SpawnPresetClientUnitsInRoom(D2ActiveRoomStrc* pRoom); // 0x6FAD1430
extern void         __stdcall  D2Client_UpdateAllUnitsPerTick(void);                  // 0x6FAD1840
extern void         __fastcall D2Client_InitTileAndSpriteCaches_Adapter(void* pPoolSystem, BOOL bReinit); // 0x6FAE04F0
extern void         __stdcall  D2Client_RenderFpsAndMemoryOverlay(void);              // 0x6FAE09D0
extern void         __fastcall D2Client_ProcessQueuedPlayerAction(D2UnitStrc* pUnit); // 0x6FAF1D90
extern int32_t      __stdcall  D2Client_DispatchMouseClickToPlayerAction(int32_t nClickType, int32_t nScreenX, int32_t nScreenY, int32_t dwKeyFlags); // 0x6FB0CE80
extern void         __stdcall  D2Client_UpdateViewportOffsetFromPlayer(void);         // 0x6FB0DB10

// SOUND / SOUNDHDR / SOUNDCHANNEL
extern void         __stdcall  SOUND_StopAllActiveUnitSounds(void);                   // 0x6FAE1AD0
extern void         __cdecl    D2Client_InitSoundChannelPool(void);                   // 0x6FAE2280
extern void         __stdcall  SOUNDHDR_FreeSoundTxtTables(void);                     // 0x6FB195E0
extern void         __stdcall  SOUNDHDR_UpdateFrame(void);                            // 0x6FB1A660
extern void         __stdcall  SOUNDHDR_Init(void);                                   // 0x6FB1A6C0
extern void         __stdcall  SOUNDCHANNEL_StopAllChannels(void);                    // 0x6FB2E0F0
extern void         __stdcall  SOUNDCHANNEL_DestroyAllChannels(void);                 // 0x6FB2E160

// AUTOMAP / LIGHTMAP / ENGINE GFX / PALSHIFT / VIEW
extern void         __stdcall  AUTOMAP_InitColors(void);                              // 0x6FAECFD0
extern void         __stdcall  AUTOMAP_UpdateViewportBounds(void);                    // 0x6FAEE530
extern void         __fastcall AUTOMAP_CenterView(int32_t bForceCenter);              // 0x6FAEF1B0
extern void         __stdcall  AUTOMAP_InitTownLayer(int32_t nLevelId, int32_t nVariant, int32_t nTileX, int32_t nTileY); // 0x6FAF06A0
extern void         __stdcall  AUTOMAP_RevealRoomFull(D2ActiveRoomStrc* pRoom);       // 0x6FAF09E0
extern void         __stdcall  AUTOMAP_UpdateReveal(void);                            // 0x6FAF0A30
extern void         __fastcall VIEW_SetSkewMode(int32_t nSkewMode);                   // 0x6FB0DFF0
extern void         __stdcall  LIGHTMAP_FreeAllLightNodes(void);                      // 0x6FB0E180
extern void         __stdcall  LIGHTMAP_InitializeReciprocalTableAndActCallback(void);// 0x6FB0F8B0
extern void         __stdcall  LIGHTMAP_ResetAmbientGrid(BOOL bClearToZero);          // 0x6FB0FB60
extern void         __cdecl    GFX_FreeMonsterCofCache(void);                         // 0x6FB40CF0
extern void         __cdecl    GFX_FreePlayerCofCache(void);                          // 0x6FB40D40
struct D2CofCacheTableStrc;
extern int          __fastcall GFX_LoadPackedCofArchive(D2CofCacheTableStrc* pCofCache, const char* szArchivePath); // 0x6FB411D0
extern void         __fastcall GFX_PrecacheNearbyMonsterSprites(int32_t bAsync);      // 0x6FB44DD0
extern D2ViewportStrc* __fastcall VIEW_AllocViewport(void);                           // 0x6FB6A5B0
extern void         __fastcall VIEW_SetViewportRect(D2ViewportStrc* pViewport, int32_t nLeft, int32_t nTop, int32_t nRight, int32_t nBottom); // 0x6FB6A990
extern void         __stdcall  ENV_AllocTileRenderPools(void);                        // 0x6FB6AA60
extern void         __stdcall  ENV_FreeTileRenderPoolsCheck(void);                    // 0x6FB6B190
extern void         __stdcall  VIEW_UpdateScreenShake(int32_t* pInOutCameraX, int32_t* pInOutCameraY); // 0x6FB6B4F0
extern void         __fastcall VIEW_UpdateViewportVisibility(D2ViewportStrc* pViewport, D2UnitStrc* pPlayer); // 0x6FB6B5E0
extern void         __fastcall VIEW_RenderViewport(D2ViewportStrc* pViewport);        // 0x6FB6B630
extern void         __fastcall PALSHIFT_FreeTables(void);                             // 0x6FB6F050
extern void         __fastcall PALSHIFT_InitTables(void);                             // 0x6FB6F170

// SCMD / MSG / WALL2 / SELECT / RECORD / ROSTER
extern void         __fastcall SCMD_HandlerB3_ReceiveSaveChunk(const uint8_t* pPacket);    // 0x6FB15020
extern void         __fastcall SCMD_ProcessPacketStream(uint8_t* pBuffer, int32_t nSize);  // 0x6FB150D0
extern void         __fastcall SCMD_HandlerB4_ConnectionRefusedReason(const uint8_t* pPacket); // 0x6FB15870
extern void         __fastcall WALL2_RegisterSingleMsgHandler(HWND hWnd, uint32_t nType, uint32_t uMsgOrId, SMSGHANDLER pfHandler); // 0x6FB1CEC0
extern int32_t      __fastcall WALL2_RunModalMessageLoop(SMSGIDLEPROC pfIdleCallback);     // 0x6FB1CF40
extern void         __fastcall WALL2_UnregisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries); // 0x6FB1D0A0
extern void         __fastcall WALL2_RegisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries);   // 0x6FB1D100
extern void         __fastcall SELECT_UpdateHoveredUnit(void);                             // 0x6FB1F0A0
extern void         __fastcall RECORD_FreeAllocatedChunks(void);                           // 0x6FB26800
extern void         __fastcall RECORD_SaveToFile(void);                                    // 0x6FB26A20
extern void         __fastcall RECORD_UpdatePlaybackTick(void);                            // 0x6FB26B70
extern void         __stdcall  RECORD_SaveAndInvokeExitCallback(int32_t a1, int32_t a2, int32_t a3); // 0x6FB26C50
extern void         __fastcall ROSTER_FreeAllRosterLists(void);                            // 0x6FB28090
extern void         __fastcall ROSTERPETS_FreeAllPets(void);                               // 0x6FB730F0
extern int32_t      __fastcall MSG_SendLeaveGame_69(void);                                 // 0x6FB29420
extern int32_t      __fastcall MSG_SendCreateGame_67(const char* szGameName);              // 0x6FB294E0
extern int32_t      __fastcall MSG_SendJoinGame_68(uint32_t dwJoinToken, uint16_t wGameId);// 0x6FB29920
extern void         __fastcall MSG_SendPing_6D(void);                                      // 0x6FB299D0
extern int32_t      __stdcall  MSG_UploadSaveFile_6C(int32_t bLocalOrHost);                // 0x6FB29B50

// UI / CMDTBL / TEXT
extern void         __stdcall  TEXT_PollBnetRealmChat(void);                               // 0x6FB35DF0
extern int32_t      __stdcall  UI_CloseOpenPanels(int32_t bPlaySoundOrMoveMouse, int32_t bCloseAutomap); // 0x6FB3B480
extern void         __fastcall UI_InitPaletteColors(void);                                 // 0x6FB3B870
extern void         __fastcall UI_ShutdownGameSession(void);                               // 0x6FB3C690
extern void         __fastcall UI_FreeLoadingScreen(void);                                 // 0x6FB3DC60
extern void         __fastcall UI_LoadLoadingScreen(void);                                 // 0x6FB3DCF0
extern void         __stdcall  UI_StepAndDrawLoadingScreen(int32_t bAdvanceFrame);         // 0x6FB3DFA0
extern void         __stdcall  UI_ReleaseAllAssets(void);                                  // 0x6FB3F480
extern void         __stdcall  UI_UpdatePanelResolution(void);                             // 0x6FB3F800
extern void         __stdcall  UI_RenderHUD(D2ViewportStrc* pViewport);                    // 0x6FB40150
extern void         __fastcall CURSOR_ResetToDefault(void);                                // 0x6FB4EDF0
extern int32_t      __fastcall CMDTBL_InitKeyBindings(int32_t nMode);                      // 0x6FB4F1E0
extern void         __fastcall CURSOR_RenderCursor(void);                                  // 0x6FB4F5B0
extern uint16_t     __fastcall CMDTBL_GetBoundKey(int32_t nCommandId, int32_t bIsPrimary); // 0x6FB4FA40
extern void         __fastcall CMDTBL_DispatchMouseWheel(int32_t nWheelSteps);             // 0x6FB4FC60
extern void         __fastcall CMDTBL_DispatchMouseButton4Up(void);                        // 0x6FB4FCD0
extern void         __fastcall CMDTBL_DispatchMouseButton4Down(void);                      // 0x6FB4FD10
extern void         __fastcall CMDTBL_DispatchMouseButton3Up(void);                        // 0x6FB4FD50
extern void         __fastcall CMDTBL_DispatchMouseButton3Down(void);                      // 0x6FB4FD90
extern int32_t      __fastcall CMDTBL_HasKeyBound(int32_t nCommandId, int32_t bIsPrimary); // 0x6FB50740

// Alternate render frame callbacks in g_apfnRenderFrameModes (0x6FB8FF98)
extern void         __fastcall RENDER_RenderCutsceneOverlayMode1(int32_t bSkipCursor);     // 0x6FAFD290
extern void         __fastcall RENDER_RenderCutsceneOverlayMode2(int32_t bSkipCursor);     // 0x6FAFD1F0

// ============================================================================
// Global Variables (Exact 1.12a D2Client.dll .rdata / .data / .bss Addresses)
// ============================================================================

extern int32_t               gnScreenWidth;                           // 0x6FB8C6E0
extern int32_t               gnScreenHeight;                          // 0x6FB8C6E4
// [aliased] gnScreenPitchWidth
extern int32_t               gnRegisteredPanelCount;                  // 0x6FB8F368 (initialized to 0x26)
extern int32_t               gnCursorDefaultStateA;                   // 0x6FB8FCD4 (initialized to 7)
extern int32_t               gnCursorDefaultStateB;                   // 0x6FB8FCD8 (initialized to 7)

// Forward declarations for SMsg & Config table function pointers
void    __fastcall GAME_RenderFrame(int32_t bSkipCursor);             // 0x6FB2CA10
void    CALLBACK   GAME_OnEndGameKeyDown(SMSGHANDLER_PARAMS* pMsg);   // 0x6FB2A600
void    CALLBACK   GAME_OnBetaScreenKeyDown(SMSGHANDLER_PARAMS* pMsg);// 0x6FB2A630
void    CALLBACK   GAME_OnBetaScreenMouseDown(SMSGHANDLER_PARAMS* pMsg); // 0x6FB2A660
void    CALLBACK   GAME_OnWinKeyEat(SMSGHANDLER_PARAMS* pMsg);        // 0x6FB2A8E0
void    CALLBACK   GAME_OnSysCommand(SMSGHANDLER_PARAMS* pMsg);       // 0x6FB2A900
void    CALLBACK   GAME_OnXButtonUp(SMSGHANDLER_PARAMS* pMsg);        // 0x6FB2B3D0
void    CALLBACK   GAME_OnXButtonDown(SMSGHANDLER_PARAMS* pMsg);      // 0x6FB2B420
void    CALLBACK   GAME_OnMButtonDown(SMSGHANDLER_PARAMS* pMsg);      // 0x6FB2B470
void    CALLBACK   GAME_OnMButtonUp(SMSGHANDLER_PARAMS* pMsg);        // 0x6FB2B4D0
void    CALLBACK   GAME_OnMouseWheel(SMSGHANDLER_PARAMS* pMsg);       // 0x6FB2B530
void    CALLBACK   GAME_OnEndGameMouseUp(SMSGHANDLER_PARAMS* pMsg);   // 0x6FB2B670
void    CALLBACK   GAME_OnEndGameMouseDown(SMSGHANDLER_PARAMS* pMsg); // 0x6FB2B700
void    CALLBACK   GAME_OnCloseWindow(SMSGHANDLER_PARAMS* pMsg);      // 0x6FB2BD70
void    CALLBACK   GAME_OnRButtonUp(SMSGHANDLER_PARAMS* pMsg);        // 0x6FB2C5C0
void    CALLBACK   GAME_OnRButtonDown(SMSGHANDLER_PARAMS* pMsg);      // 0x6FB2C6C0
void    CALLBACK   GAME_OnLButtonDown(SMSGHANDLER_PARAMS* pMsg);      // 0x6FB2C870
void    CALLBACK   GAME_OnActivateApp(SMSGHANDLER_PARAMS* pMsg);      // 0x6FB2C910
void    CALLBACK   GAME_OnLButtonUpMsg(SMSGHANDLER_PARAMS* pMsg);     // 0x6FB2CA00

void    __fastcall GAME_ConfigApply_NoOp29FF0(D2ConfigStrc* pCfg);    // 0x6FB29FF0
void    __fastcall GAME_ConfigApply_TxtMode(D2ConfigStrc* pCfg);      // 0x6FB2A000
void    __fastcall GAME_ConfigApply_CompressMode(D2ConfigStrc* pCfg); // 0x6FB2A020
void    __fastcall GAME_ConfigApply_NoOp2A040(D2ConfigStrc* pCfg);    // 0x6FB2A040
void    __fastcall GAME_ConfigApply_NoOp2A050(D2ConfigStrc* pCfg);    // 0x6FB2A050
void    __fastcall GAME_ConfigApply_ComInterface(D2ConfigStrc* pCfg); // 0x6FB2A060
void    __fastcall GAME_ConfigApply_CharTemplate(D2ConfigStrc* pCfg); // 0x6FB2A070
void    __fastcall GAME_ConfigApply_NoOp2A080(D2ConfigStrc* pCfg);    // 0x6FB2A080
void    __fastcall GAME_ConfigApply_NoOp2A090(D2ConfigStrc* pCfg);    // 0x6FB2A090
void    __fastcall GAME_ConfigApply_NoOp2A0A0(D2ConfigStrc* pCfg);    // 0x6FB2A0A0
void    __fastcall GAME_ConfigApply_CheatsArenaFlag(D2ConfigStrc* pCfg); // 0x6FB2A0B0
void    __fastcall GAME_ConfigApply_RealmName(D2ConfigStrc* pCfg);    // 0x6FB2A0C0
void    __fastcall GAME_ConfigApply_CharName(D2ConfigStrc* pCfg);     // 0x6FB2A0E0
void    __fastcall GAME_ConfigApply_NoOp2A150(D2ConfigStrc* pCfg);    // 0x6FB2A150
void    __fastcall GAME_ConfigApply_Seed(D2ConfigStrc* pCfg);         // 0x6FB2A160
void    __fastcall GAME_ConfigApply_Log(D2ConfigStrc* pCfg);          // 0x6FB2A180
void    __fastcall GAME_ConfigApply_NoOp2A1A0(D2ConfigStrc* pCfg);    // 0x6FB2A1A0
void    __fastcall GAME_ConfigApply_NoOp2A1B0(D2ConfigStrc* pCfg);    // 0x6FB2A1B0
void    __fastcall GAME_ConfigApply_NoOp2A1C0(D2ConfigStrc* pCfg);    // 0x6FB2A1C0
void    __fastcall GAME_ConfigApply_NoOp2A1D0(D2ConfigStrc* pCfg);    // 0x6FB2A1D0
void    __fastcall GAME_ConfigApply_NoOp2A1E0(D2ConfigStrc* pCfg);    // 0x6FB2A1E0
void    __fastcall GAME_ConfigApply_VSync(D2ConfigStrc* pCfg);        // 0x6FB2A1F0
void    __fastcall GAME_ConfigApply_Gamma(D2ConfigStrc* pCfg);        // 0x6FB2A200
void    __fastcall GAME_ConfigApply_NoOp2A210(D2ConfigStrc* pCfg);    // 0x6FB2A210
void    __fastcall GAME_ConfigApply_RenderRave(D2ConfigStrc* pCfg);   // 0x6FB2A220
void    __fastcall GAME_ConfigApply_RenderD3D(D2ConfigStrc* pCfg);    // 0x6FB2A240
void    __fastcall GAME_ConfigApply_RenderOpenGL(D2ConfigStrc* pCfg); // 0x6FB2A260
void    __fastcall GAME_ConfigApply_Render3DFX(D2ConfigStrc* pCfg);   // 0x6FB2A280
void    __fastcall GAME_ConfigApply_RenderWindowed(D2ConfigStrc* pCfg); // 0x6FB2A2A0
void    __fastcall GAME_ConfigApply_NoOp2A2C0(D2ConfigStrc* pCfg);    // 0x6FB2A2C0
void    __fastcall GAME_ConfigApply_NoOp2A2D0(D2ConfigStrc* pCfg);    // 0x6FB2A2D0
void    __fastcall GAME_ConfigApply_NoOp2A2E0(D2ConfigStrc* pCfg);    // 0x6FB2A2E0
void    __fastcall GAME_ConfigApply_PlayerClass(D2ConfigStrc* pCfg);  // 0x6FB2A2F0
void    __fastcall GAME_ConfigApply_McpIp(D2ConfigStrc* pCfg);        // 0x6FB2A370
void    __fastcall GAME_ConfigApply_BattleNetIp(D2ConfigStrc* pCfg);  // 0x6FB2A3A0
void    __fastcall GAME_ConfigApply_NetworkSession(D2ConfigStrc* pCfg); // 0x6FB2A3D0
void    __fastcall GAME_ConfigApply_Act(D2ConfigStrc* pCfg);          // 0x6FB2A430
void    __fastcall GAME_ConfigApply_NoSave(D2ConfigStrc* pCfg);       // 0x6FB2A480

// 0x6FB8FF98: Render mode callback table (3 entries)
static const D2RenderFrameFn g_apfnRenderFrameModes[3] =              // 0x6FB8FF98
{
    GAME_RenderFrame,                                                 // [0] 0x6FB2CA10
    RENDER_RenderCutsceneOverlayMode1,                                // [1] 0x6FAFD290
    RENDER_RenderCutsceneOverlayMode2,                                // [2] 0x6FAFD1F0
};
static int32_t  g_nJoinStartNetState        = D2CLIENT_STATE_START_NET; // 0x6FB8FFA4 (5)
static int32_t  g_nTargetFrameTickMs        = 40;                     // 0x6FB8FFA8 (40 ms = 25 Hz)
char            g_szGameServerIp[264]       = "207.82.87.243";        // 0x6FB8FFB0
static char     g_szBattleNetIp[264]        = "207.82.87.139";        // 0x6FB900B8
static char     g_szMcpServerIp[264]        = "207.82.87.133";        // 0x6FB901C0

// 0x6FB902C8: Beta slideshow screens table (10 entries, 13 bytes each)
static D2BetaScreenEntryStrc g_aBetaScreens[10] =                     // 0x6FB902C8
{
    { 0, nullptr, 0, "screen01" },                                    // 0x6FB902C8
    { 0, nullptr, 0, "screen02" },                                    // 0x6FB902D5
    { 0, nullptr, 0, "screen03" },                                    // 0x6FB902E2
    { 0, nullptr, 0, "screen04" },                                    // 0x6FB902EF
    { 0, nullptr, 0, "screen05" },                                    // 0x6FB902FC
    { 0, nullptr, 0, "screen06" },                                    // 0x6FB90309
    { 0, nullptr, 0, "screen07" },                                    // 0x6FB90316
    { 0, nullptr, 0, "screen08" },                                    // 0x6FB90323
    { 0, nullptr, 0, "screen09" },                                    // 0x6FB90330
    { 0, nullptr, 0, "screen10" },                                    // 0x6FB9033D
};
static int32_t  g_nBetaScreenCount          = 10;                     // 0x6FB9034C
int32_t         g_nLeftMouseButtonState     = 0x10;                   // 0x6FB90354 (0x04=Down, 0x08=Hold, 0x10=Up)
int32_t         g_nRightMouseButtonState    = 0x10;                   // 0x6FB90358 (0x04=Down, 0x08=Hold, 0x10=Up)
static int32_t  g_nPostActLoadBlackFrames   = 1;                      // 0x6FB9035C

// 0x6FB90360: Primary In-Game Storm Message Registration Table (10 entries = 0x78 bytes)
static const D2SMsgRegEntryStrc g_aInGameSMsgTable[10] =              // 0x6FB90360
{
    { SMSG_DISPATCH_MESSAGE, WM_LBUTTONDOWN, GAME_OnLButtonDown  },   // 0x6FB90360
    { SMSG_DISPATCH_MESSAGE, WM_LBUTTONUP,   GAME_OnLButtonUpMsg },   // 0x6FB9036C
    { SMSG_DISPATCH_MESSAGE, WM_RBUTTONDOWN, GAME_OnRButtonDown  },   // 0x6FB90378
    { SMSG_DISPATCH_MESSAGE, WM_RBUTTONUP,   GAME_OnRButtonUp    },   // 0x6FB90384
    { SMSG_DISPATCH_KEYDOWN, VK_LWIN,        GAME_OnWinKeyEat    },   // 0x6FB90390
    { SMSG_DISPATCH_KEYDOWN, VK_RWIN,        GAME_OnWinKeyEat    },   // 0x6FB9039C
    { SMSG_DISPATCH_KEYDOWN, VK_APPS,        GAME_OnWinKeyEat    },   // 0x6FB903A8
    { SMSG_DISPATCH_MESSAGE, WM_SYSCOMMAND,  GAME_OnSysCommand   },   // 0x6FB903B4
    { SMSG_DISPATCH_MESSAGE, WM_CLOSE,       GAME_OnCloseWindow  },   // 0x6FB903C0
    { SMSG_DISPATCH_MESSAGE, WM_ACTIVATEAPP, GAME_OnActivateApp  },   // 0x6FB903CC
};

// 0x6FB903D8..0x6FB9041F: Mouse Wheel, Middle Button, XButton & Magellan Wheel SMsg Tables
static const D2SMsgRegEntryStrc g_aMouseWheelSMsgTable[1] =           // 0x6FB903D8
{
    { SMSG_DISPATCH_MESSAGE, WM_MOUSEWHEEL,  GAME_OnMouseWheel   },   // 0x6FB903D8
};
static const D2SMsgRegEntryStrc g_aMiddleMouseSMsgTable[2] =          // 0x6FB903E4
{
    { SMSG_DISPATCH_MESSAGE, WM_MBUTTONUP,   GAME_OnMButtonUp    },   // 0x6FB903E4
    { SMSG_DISPATCH_MESSAGE, WM_MBUTTONDOWN, GAME_OnMButtonDown  },   // 0x6FB903F0
};
static const D2SMsgRegEntryStrc g_aXButtonMouseSMsgTable[2] =         // 0x6FB903FC
{
    { SMSG_DISPATCH_MESSAGE, WM_XBUTTONDOWN, GAME_OnXButtonDown  },   // 0x6FB903FC
    { SMSG_DISPATCH_MESSAGE, WM_XBUTTONUP,   GAME_OnXButtonUp    },   // 0x6FB90408
};
static D2SMsgRegEntryStrc g_aMagellanWheelSMsgTable[1] =              // 0x6FB90414
{
    { SMSG_DISPATCH_MESSAGE, 0,              GAME_OnMouseWheel   },   // 0x6FB90414 (uMsgOrId @ 0x6FB90418 set dynamically)
};

// 0x6FB90420: Disconnect / Connection Refused Error String IDs (30 uint16_t entries)
static const uint16_t g_awDisconnectErrorStrIds[30] =                 // 0x6FB90420
{
    0x14F5, 0x14F6, 0x14F7, 0x14F8, 0x14F9, 0x14FB, 0x14FA, 0x14ED,   // [0..7]
    0x14EE, 0x14FC, 0x14FD, 0x14FE, 0x14FF, 0x1500, 0x1501, 0x1502,   // [8..15]
    0x1503, 0x1504, 0x1505, 0x14F0, 0x14F4, 0x14F3, 0x14F2, 0x14F1,   // [16..23]
    0x14EF, 0x2775, 0x2776, 0x14FA, 0x14FB, 0x0000,                   // [24..29]
};

// 0x6FB90460: Beta Slideshow Screen SMsg Table (7 entries)
static const D2SMsgRegEntryStrc g_aBetaScreenSMsgTable[7] =           // 0x6FB90460
{
    { SMSG_DISPATCH_MESSAGE, WM_LBUTTONDOWN, GAME_OnBetaScreenMouseDown }, // 0x6FB90460
    { SMSG_DISPATCH_MESSAGE, WM_RBUTTONDOWN, GAME_OnBetaScreenMouseDown }, // 0x6FB9046C
    { SMSG_DISPATCH_MESSAGE, WM_KEYDOWN,     GAME_OnBetaScreenKeyDown   }, // 0x6FB90478
    { SMSG_DISPATCH_MESSAGE, WM_ACTIVATEAPP, GAME_OnActivateApp         }, // 0x6FB90484
    { SMSG_DISPATCH_KEYDOWN, VK_LWIN,        GAME_OnWinKeyEat           }, // 0x6FB90490
    { SMSG_DISPATCH_KEYDOWN, VK_RWIN,        GAME_OnWinKeyEat           }, // 0x6FB9049C
    { SMSG_DISPATCH_KEYDOWN, VK_APPS,        GAME_OnWinKeyEat           }, // 0x6FB904A8
};

// 0x6FB904B8: End-Game Victory Screen SMsg Table (9 entries)
static const D2SMsgRegEntryStrc g_aEndGameScreenSMsgTable[9] =        // 0x6FB904B8
{
    { SMSG_DISPATCH_MESSAGE, WM_LBUTTONDOWN, GAME_OnEndGameMouseDown }, // 0x6FB904B8
    { SMSG_DISPATCH_MESSAGE, WM_RBUTTONDOWN, GAME_OnEndGameMouseDown }, // 0x6FB904C4
    { SMSG_DISPATCH_MESSAGE, WM_LBUTTONUP,   GAME_OnEndGameMouseUp   }, // 0x6FB904D0
    { SMSG_DISPATCH_MESSAGE, WM_RBUTTONUP,   GAME_OnEndGameMouseUp   }, // 0x6FB904DC
    { SMSG_DISPATCH_MESSAGE, WM_KEYDOWN,     GAME_OnEndGameKeyDown   }, // 0x6FB904E8
    { SMSG_DISPATCH_MESSAGE, WM_ACTIVATEAPP, GAME_OnActivateApp      }, // 0x6FB904F4
    { SMSG_DISPATCH_KEYDOWN, VK_LWIN,        GAME_OnWinKeyEat        }, // 0x6FB90500
    { SMSG_DISPATCH_KEYDOWN, VK_RWIN,        GAME_OnWinKeyEat        }, // 0x6FB9050C
    { SMSG_DISPATCH_KEYDOWN, VK_APPS,        GAME_OnWinKeyEat        }, // 0x6FB90518
};

int32_t        g_bConfigApplyResult = 1;                              // 0x6FB90524

// 0x6FB90528: Command-Line / D2ConfigStrc Application Handler Table (49 entries = 0xC4 bytes)
extern const D2ConfigApplyFn g_apfnApplyConfigCallbacks[49];
const D2ConfigApplyFn g_apfnApplyConfigCallbacks[49] =                // 0x6FB90528
{
    GAME_ConfigApply_RenderWindowed,                                  // [ 0] 0x6FB90528 -> 0x6FB2A2A0
    GAME_ConfigApply_Render3DFX,                                      // [ 1] 0x6FB9052C -> 0x6FB2A280
    GAME_ConfigApply_RenderOpenGL,                                    // [ 2] 0x6FB90530 -> 0x6FB2A260
    GAME_ConfigApply_RenderD3D,                                       // [ 3] 0x6FB90534 -> 0x6FB2A240
    GAME_ConfigApply_RenderRave,                                      // [ 4] 0x6FB90538 -> 0x6FB2A220
    GAME_ConfigApply_NoOp2A210,                                       // [ 5] 0x6FB9053C -> 0x6FB2A210
    GAME_ConfigApply_Gamma,                                           // [ 6] 0x6FB90540 -> 0x6FB2A200
    GAME_ConfigApply_VSync,                                           // [ 7] 0x6FB90544 -> 0x6FB2A1F0
    GAME_ConfigApply_NoOp2A1C0,                                       // [ 8] 0x6FB90548 -> 0x6FB2A1C0
    GAME_ConfigApply_NetworkSession,                                  // [ 9] 0x6FB9054C -> 0x6FB2A3D0
    GAME_ConfigApply_NetworkSession,                                  // [10] 0x6FB90550 -> 0x6FB2A3D0
    GAME_ConfigApply_NetworkSession,                                  // [11] 0x6FB90554 -> 0x6FB2A3D0
    GAME_ConfigApply_NetworkSession,                                  // [12] 0x6FB90558 -> 0x6FB2A3D0
    GAME_ConfigApply_NetworkSession,                                  // [13] 0x6FB9055C -> 0x6FB2A3D0
    GAME_ConfigApply_BattleNetIp,                                     // [14] 0x6FB90560 -> 0x6FB2A3A0
    GAME_ConfigApply_McpIp,                                           // [15] 0x6FB90564 -> 0x6FB2A370
    GAME_ConfigApply_PlayerClass,                                     // [16] 0x6FB90568 -> 0x6FB2A2F0
    GAME_ConfigApply_PlayerClass,                                     // [17] 0x6FB9056C -> 0x6FB2A2F0
    GAME_ConfigApply_PlayerClass,                                     // [18] 0x6FB90570 -> 0x6FB2A2F0
    GAME_ConfigApply_PlayerClass,                                     // [19] 0x6FB90574 -> 0x6FB2A2F0
    GAME_ConfigApply_PlayerClass,                                     // [20] 0x6FB90578 -> 0x6FB2A2F0
    GAME_ConfigApply_NoOp2A2C0,                                       // [21] 0x6FB9057C -> 0x6FB2A2C0
    GAME_ConfigApply_CharName,                                        // [22] 0x6FB90580 -> 0x6FB2A0E0
    GAME_ConfigApply_RealmName,                                       // [23] 0x6FB90584 -> 0x6FB2A0C0
    GAME_ConfigApply_CharTemplate,                                    // [24] 0x6FB90588 -> 0x6FB2A070
    GAME_ConfigApply_NoOp2A2E0,                                       // [25] 0x6FB9058C -> 0x6FB2A2E0
    GAME_ConfigApply_NoOp2A2D0,                                       // [26] 0x6FB90590 -> 0x6FB2A2D0
    GAME_ConfigApply_NoOp2A1E0,                                       // [27] 0x6FB90594 -> 0x6FB2A1E0
    GAME_ConfigApply_NoOp2A1B0,                                       // [28] 0x6FB90598 -> 0x6FB2A1B0
    GAME_ConfigApply_NoOp2A1A0,                                       // [29] 0x6FB9059C -> 0x6FB2A1A0
    GAME_ConfigApply_Act,                                             // [30] 0x6FB905A0 -> 0x6FB2A430
    GAME_ConfigApply_Log,                                             // [31] 0x6FB905A4 -> 0x6FB2A180
    GAME_ConfigApply_NoOp2A150,                                       // [32] 0x6FB905A8 -> 0x6FB2A150
    GAME_ConfigApply_NoSave,                                          // [33] 0x6FB905AC -> 0x6FB2A480
    GAME_ConfigApply_Seed,                                            // [34] 0x6FB905B0 -> 0x6FB2A160
    GAME_ConfigApply_CheatsArenaFlag,                                 // [35] 0x6FB905B4 -> 0x6FB2A0B0
    GAME_ConfigApply_NoOp2A0A0,                                       // [36] 0x6FB905B8 -> 0x6FB2A0A0
    GAME_ConfigApply_NoOp2A090,                                       // [37] 0x6FB905BC -> 0x6FB2A090
    GAME_ConfigApply_NoOp2A1D0,                                       // [38] 0x6FB905C0 -> 0x6FB2A1D0
    GAME_ConfigApply_NoOp2A050,                                       // [39] 0x6FB905C4 -> 0x6FB2A050
    nullptr,                                                          // [40] 0x6FB905C8 -> 0x00000000
    GAME_ConfigApply_NoOp2A040,                                       // [41] 0x6FB905CC -> 0x6FB2A040
    GAME_ConfigApply_NoOp2A080,                                       // [42] 0x6FB905D0 -> 0x6FB2A080
    GAME_ConfigApply_ComInterface,                                    // [43] 0x6FB905D4 -> 0x6FB2A060
    GAME_ConfigApply_NetworkSession,                                  // [44] 0x6FB905D8 -> 0x6FB2A3D0
    nullptr,                                                          // [45] 0x6FB905DC -> 0x00000000
    GAME_ConfigApply_CompressMode,                                    // [46] 0x6FB905E0 -> 0x6FB2A020
    GAME_ConfigApply_TxtMode,                                         // [47] 0x6FB905E4 -> 0x6FB2A000
    GAME_ConfigApply_NoOp29FF0,                                       // [48] 0x6FB905E8 -> 0x6FB29FF0
};

// Cross-module UI / Window / Renderer / Quest / Automap Globals
extern int32_t               gnWallCursorStateA;                      // 0x6FB93A5C
extern int32_t               gnWallCursorStateB;                      // 0x6FB93A60
extern int32_t               gbAppActiveState;                        // 0x6FB93A80
extern int32_t               gbAutomapOn;                             // 0x6FBA3B9C
extern int32_t               g_nDisplayRenderMode;                    // 0x6FBA6758
extern uint32_t              gdwArenaFlags;                           // 0x6FBA675C
extern int32_t               g_nArenaCharTemplate;                    // 0x6FBA6760
// [aliased] g_nDenOfEvilSunTimer
// [aliased] g_nActQuestEnvTimer
// [aliased] g_bExitAppFlag

// 0x6FBAA2FC: Per-Act Packed Monster COF Archive Names
static const char* const g_aszActMonsterCofFiles[5] =                 // 0x6FBAA2FC
{
    "cmncof_a1.d2",                                                   // [0] 0x6FB84200
    "cmncof_a2.d2",                                                   // [1] 0x6FB841F0
    "cmncof_a3.d2",                                                   // [2] 0x6FB841E0
    "cmncof_a4.d2",                                                   // [3] 0x6FB841D0
    "cmncof_a6.d2",                                                   // [4] 0x6FB841C0
};

static int32_t               g_nMaxOpenGamePlayers    = 8;            // 0x6FBAA3C4
static int32_t               g_nPrevCameraY           = -1;           // 0x6FBAA3C8
static int32_t               g_nPrevCameraX           = -1;           // 0x6FBAA3CC

extern void                  (*g_pfnMButtonDownCallback)(void);       // 0x6FBB0D24
extern D2KeyBindingStrc gKeyBindings[114];
#define g_aKeyBindings gKeyBindings                     // 0x6FBB11A0 (..0x6FBB1614; bIsPrimary[0] @ 0x6FBB11A6)
extern void                  (*g_pfnMButtonUpCallback)(void);         // 0x6FBB1624
extern int32_t               gnMouseY;                                // 0x6FBB1634
extern int32_t               gnMouseX;                                // 0x6FBB1638
extern D2CofCacheTableStrc   g_MonsterCofCache;                       // 0x6FBB1B08
extern void*                 g_pGfxScratchBuffer;                     // 0x6FBB2B24
// [aliased] g_bEscMenuOpen
// [aliased] g_bNpcMenuOpen
// [aliased] g_bAltShowItemsHeld
extern int32_t               g_nEscMenuSelection;                     // 0x6FBB2BF0

static CRITICAL_SECTION      g_csServerThread;                        // 0x6FBB327C
static D2ClientTimingBlockStrc g_tClientTiming = {};                  // 0x6FBB3298 .. 0x6FBB3317


inline const char* Fog_10265(int nErrorIndex) { (void)nErrorIndex; return FOG_GetCallerReturnAddress(); }
extern int32_t gaPanelStates[38];
extern int32_t gnSelectedTargetA;
extern int32_t gnSelectedTargetB;
extern D2ConfigStrc* gpD2Config_6FBCB908;
extern int32_t ganClientStateQueue_6FBCB8F4[5];
extern int32_t gnClientStateQueueCount_6FBCC438;
extern int32_t gbIs800x600Mode_6FBCB980;
extern BOOL    gbExitApplication_6FBA8B70;
extern int32_t gnScreenPitch_6FB8C6E8;
struct D2FogDataListStrc;
extern D2FogDataListStrc* gpEnvSplashesPool_6FBCC3E0;
extern D2FogDataListStrc* gpEnvBubblesPool_6FBCC3E4;
extern D2FogDataListStrc* gpEnvParticlesPool_6FBCC3E8;
extern uint8_t gbCutsceneFromMainMenu_6FBCB6CC;
extern uint8_t gbHighResCutscene_6FBCB6CE;
extern uint8_t gbInCutscenePlayback_6FBCB6CF;
extern uint8_t gbChainAct5IntroVideo_6FBCB6D0;
extern int32_t gnTristramPortalFadeTick_6FBA8AD0;
extern uint32_t gdwDiabloUnitGUID_6FBA8AD8;
extern BOOL    gbTristramPortalOpened_6FBCC418;
extern BOOL    gbDiabloSealTintActive_6FBCC41C;
extern int32_t gnExitCountdownEndSyncTime_6FBCC420;
extern BOOL    gbNihlathakTauntReady_6FBCC424;

#define g_bEscMenuOpen        (gaPanelStates[9])
#define g_bNpcMenuOpen        (gaPanelStates[8])
#define g_bAltShowItemsHeld   (gaPanelStates[13])
#define g_bSkillSelectHold    gnSelectedTargetA
#define g_bSkillTreeHold      gnSelectedTargetB
#define g_pGameData           gpD2Config_6FBCB908
#define g_anClientStateStack  ganClientStateQueue_6FBCB8F4
#define g_nClientStateStackDepth gnClientStateQueueCount_6FBCC438
#define g_nCurrentResolutionFlag gbIs800x600Mode_6FBCB980
#define g_bExitAppFlag        gbExitApplication_6FBA8B70
#define gnScreenPitchWidth    gnScreenPitch_6FB8C6E8
#define g_hEnvSplashesPool    gpEnvSplashesPool_6FBCC3E0
#define g_hEnvBubblesPool     gpEnvBubblesPool_6FBCC3E4
#define g_hEnvParticlesPool   gpEnvParticlesPool_6FBCC3E8
#define g_bCutsceneSkipFlagA  gbCutsceneFromMainMenu_6FBCB6CC
#define g_bDirectSoundCapable gbHighResCutscene_6FBCB6CE
#define g_bMoviePlayingFlag   gbInCutscenePlayback_6FBCB6CF
#define g_bCutsceneSkipFlagB  gbChainAct5IntroVideo_6FBCB6D0
#define g_nDenOfEvilSunTimer  gnTristramPortalFadeTick_6FBA8AD0
#define g_nActQuestEnvTimer   gdwDiabloUnitGUID_6FBA8AD8
#define g_bDenOfEvilCleared   gbTristramPortalOpened_6FBCC418
#define g_bBloodRavenCleared  gbDiabloSealTintActive_6FBCC41C
#define g_nQuestStateFlag420  gnExitCountdownEndSyncTime_6FBCC420
#define g_nQuestStateFlag424  gbNihlathakTauntReady_6FBCC424

// Remaining GAME_Game.cpp-owned state globals
int32_t  g_bActionDispatchedThisFrame = 0; // 0x6FBCC1D8
int32_t  g_bForceSpritePrecache       = 0; // 0x6FBCBD1C
int32_t  g_bQuickExitRequested        = 0; // 0x6FBC47A0
int32_t  g_bWindowedMode              = 1; // 0x6FBCC384
uint32_t g_dwLastSpritePrecacheTick   = 0; // 0x6FBCBD0C
int32_t  g_nArenaCharTemplate         = 0; // 0x6FBA6760
int32_t  g_nCurrentActIndex           = 0; // 0x6FBCBD5C
int32_t  g_nDisplayRenderMode         = 4; // 0x6FBA6758
int32_t  g_nEscMenuSelection          = 0; // 0x6FBB2BF0
int32_t  g_nHoveredWarpState          = 0; // 0x6FBCBF48
// [aliased] g_nQuestStateFlag420
// [aliased] g_nQuestStateFlag424
void*    g_pGfxScratchBuffer          = nullptr; // 0x6FBB2B24
uint16_t g_wHoveredWarpId             = 0; // 0x6FBCBF44
int32_t  gbAppActiveState             = 1; // 0x6FB93A80
int32_t  gbAutomapOn                  = 0; // 0x6FBA3B9C
int32_t  gbRecordingInputActive       = 0; // 0x6FBCC08C
int32_t  gbRecordingInputReady        = 0; // 0x6FBCC090
int32_t  gbRecordPlaybackReady        = 0; // 0x6FBCC098
int32_t  gnAutomapBaseX               = 0; // 0x6FBCC2E0
int32_t  gnAutomapBaseY               = 0; // 0x6FBCC2E4
int32_t  gnCursorDefaultStateA        = 7; // 0x6FB8FCD4
int32_t  gnCursorDefaultStateB        = 7; // 0x6FB8FCD8
int32_t  gnRegisteredPanelCount       = 38;// 0x6FB8F368
int32_t  gnWallCursorStateA           = 0; // 0x6FB93A5C
int32_t  gnWallCursorStateB           = 0; // 0x6FB93A60
void     (__stdcall* g_pfWindowExitCallback)(int32_t, int32_t, int32_t) = nullptr;
void     (__stdcall* g_pfChainedExitCallback)(int32_t, int32_t, int32_t) = nullptr;
void     (*g_pfnMButtonDownCallback)(void) = nullptr;
void     (*g_pfnMButtonUpCallback)(void)   = nullptr;

// Shared canonical aliases into 0x6FBB3298..0x6FBB3317
uint32_t                     gdwClientGameTick        = 0;            // 0x6FBB32B0
uint32_t                     gdwClientPing            = 0;            // 0x6FBB32BC
uint32_t                     gdwReassignGraceTick     = 0;            // 0x6FBB32D4
int32_t                      gbIsExpansion            = 0;            // 0x6FBB330C
int32_t                      gbIsLadderClient         = 0;            // 0x6FBB3310

static char                  g_szConfigRealm[32]      = {};           // 0x6FBB3318
static uint16_t              g_wJoinGameId            = 0;            // 0x6FBB3338
static uint32_t              g_dwActUnloadStartTick   = 0;            // 0x6FBB333C
static OSVERSIONINFOA        g_tOsVersionInfo         = {};           // 0x6FBB3340 (dwMajorVersion @ 0x6FBB3344, dwMinorVersion @ 0x6FBB3348, dwPlatformId @ 0x6FBB3350)
extern uint8_t gnLocalPlayerCharClass;
#define g_nSelectedCharClass gnLocalPlayerCharClass            // 0x6FBB33D4
static HANDLE                g_hOpenServerThread      = nullptr;      // 0x6FBB33D8
static uint32_t              g_dwJoinServerToken      = 0;            // 0x6FBB33DC
char                         g_szConfigCharName[16]   = {};           // 0x6FBB33E0
static int32_t               g_nDisconnectErrorIndex  = 0;            // 0x6FBB33F0
static char                  g_szConfigGameName[20]   = {};           // 0x6FBB33F8
static DWORD                 g_dwOpenServerThreadId   = 0;            // 0x6FBB340C

extern void                  (__stdcall* g_pfChainedExitCallback)(int32_t, int32_t, int32_t); // 0x6FBB5364
extern int32_t               g_bQuickExitRequested;                   // 0x6FBC47A0
extern D2UnitStrc*           gapClientSideUnitHashTables[6][128];     // 0x6FBC9D60 (0xC00 bytes)
extern D2UnitStrc*           gapServerSideUnitHashTables[6][128];     // 0x6FBCA960 (0xC00 bytes)

// [aliased] g_bCutsceneSkipFlagA..B
static int32_t               g_aEnvStateResetBlock[8] = {};           // 0x6FBCB6D4 .. 0x6FBCB6F3 (0x6FBCB6D4..0x6FBCB6F0)
static int32_t               g_aQuestEnvTimers[11]    = {};           // 0x6FBCB7C4 .. 0x6FBCB7EF

// [aliased] g_anClientStateStack
// [aliased] g_pGameData
// [aliased] g_nCurrentResolutionFlag
extern uint32_t              g_dwLastSpritePrecacheTick;              // 0x6FBCBD0C
extern int32_t               g_bForceSpritePrecache;                  // 0x6FBCBD1C
extern int32_t               g_nCurrentActIndex;                      // 0x6FBCBD5C
extern uint16_t              g_wHoveredWarpId;                        // 0x6FBCBF44
extern int32_t               g_nHoveredWarpState;                     // 0x6FBCBF48

static BnClientInterface*    g_pBnetComInterface      = nullptr;      // 0x6FBCBFDC
static uint32_t              g_dwConnectStartTick     = 0;            // 0x6FBCBFE0
static int32_t               g_bColorTxtFlag          = 0;            // 0x6FBCBFE4
static int32_t               g_bShowBetaScreens       = 0;            // 0x6FBCBFE8
static int32_t               g_bShowClassicEndGameScreen = 0;         // 0x6FBCBFEC
static int32_t               g_bEndGameScreenDismissed = 0;           // 0x6FBCBFF0
uint8_t                      gnClientDifficulty       = 0;            // 0x6FBCBFF4
int32_t                      gnGameType               = 0;            // 0x6FBCBFF8

int32_t                      g_bSystemConnected       = 0;            // 0x6FBCC000
int32_t                      gbActLoadedFlag          = 0;            // 0x6FBCC004
int32_t                      g_bExitGameFlag          = 0;            // 0x6FBCC008
int32_t                      gbActUnloadedFlag        = 0;            // 0x6FBCC00C
int32_t                      g_bPlayExpansionEpilogueMovie = 0;       // 0x6FBCC010
int32_t                      g_bShowExpansionEndGameScreen = 0;       // 0x6FBCC014
extern D2DrlgActStrc*        gpClientAct;                             // 0x6FBCC01C
static D2ActSeedPairStrc*    g_pActSeeds              = nullptr;      // 0x6FBCC020
static uint32_t              g_dwCurrentTownLevelId   = 0;            // 0x6FBCC024
extern void* gpClientViewport;
#define g_pClientViewport (*reinterpret_cast<D2ViewportStrc**>(&gpClientViewport))      // 0x6FBCC028
static uint8_t               g_nCurrentBetaScreenIdx  = 0;            // 0x6FBCC02C
static int32_t               g_bOsVersionValid        = 0;            // 0x6FBCC030
static int32_t               g_nDisconnectScreenFramesLeft = 0;       // 0x6FBCC034
static int32_t               g_bStopServerThread      = 0;            // 0x6FBCC038
static int32_t               g_bServerThreadReady     = 0;            // 0x6FBCC03C

int32_t                      g_bLeftMouseHeld         = 0;            // 0x6FBCC040
int32_t                      g_bRightMouseHeld        = 0;            // 0x6FBCC044
int32_t                      g_bSuppressLeftMouseHold = 0;            // 0x6FBCC048
int32_t                      gbRunLock                = 0;            // 0x6FBCC04C
int32_t                      gbAlwaysRun              = 0;            // 0x6FBCC050
int32_t                      gbStandStillHeld         = 0;            // 0x6FBCC054
int32_t                      g_bLeftMouseRepeating    = 0;            // 0x6FBCC05C
int32_t                      g_bRightMouseRepeating   = 0;            // 0x6FBCC060
static uint8_t               g_bDeferredTaintedSunInit = 0;           // 0x6FBCC064
int32_t                      g_bWindowClosedExit      = 0;            // 0x6FBCC068
int32_t                      g_nCameraDeltaX          = 0;            // 0x6FBCC06C
int32_t                      g_nCameraDeltaY          = 0;            // 0x6FBCC070
static int32_t               g_bEndGameOkButtonPressed = 0;           // 0x6FBCC074

extern int32_t               gbRecordingInputActive;                  // 0x6FBCC08C
extern int32_t               gbRecordingInputReady;                   // 0x6FBCC090
extern int32_t               gbRecordPlaybackActive;                  // 0x6FBCC094
extern int32_t               gbRecordPlaybackReady;                   // 0x6FBCC098
// [aliased] g_bSkillSelectHold
// [aliased] g_bSkillTreeHold
extern int32_t               gnOpenPanelMode;                         // 0x6FBCC1D0
extern int32_t               gnViewportShiftOffsetX;                  // 0x6FBCC1D4
extern int32_t               g_bActionDispatchedThisFrame;            // 0x6FBCC1D8
extern int32_t               gbAutomapCenters;                        // 0x6FBCC2A0
extern int32_t               gnAutomapOffsetX;                        // 0x6FBCC2D8
extern int32_t               gnAutomapOffsetY;                        // 0x6FBCC2DC
extern int32_t               gnAutomapBaseY;                          // 0x6FBCC328
extern int32_t               gnAutomapBaseX;                          // 0x6FBCC32C
extern int32_t               g_bWindowedMode;                         // 0x6FBCC384
extern D2UnitStrc*           gpPlayerUnit;                            // 0x6FBCC3D0
// [aliased] g_hEnvSplashesPool..g_nQuestStateFlag424
// [aliased] g_nClientStateStackDepth
extern void                  (__stdcall* g_pfWindowExitCallback)(int32_t, int32_t, int32_t); // 0x6FBCC54C

extern int32_t               gnViewportHeight;                        // 0x6FBCD524
extern int32_t               gnViewportWidth;                         // 0x6FBCD528

static D2CellFileStrc*       g_pEndGameOkBtnCell      = nullptr;      // 0x6FBD3420
static D2CellFileStrc*       g_pEndGameBgCell         = nullptr;      // 0x6FBD3424
static int32_t               g_nConsecutiveSkippedFrames = 0;         // 0x6FBD3428
static int32_t               g_nMouseWheelDeltaAccum  = 0;            // 0x6FBD342C

// ============================================================================
// SECTION 1: Session State Accessors, Color.txt & Act Tile Preloader
// (0x6FB29CD0 .. 0x6FB29FEF)
// ============================================================================

// 1.12a: 0x6FB29CD0
uint8_t __stdcall GAME_GetDifficulty(void)
{
    return gnClientDifficulty;
}

// 1.12a: 0x6FB29CE0
int32_t __stdcall GAME_IsExpansion(void)
{
    return gbIsExpansion;
}

// 1.12a: 0x6FB29CF0
// Returns gbIsLadderClient for Closed Battle.net realm games (types 2..3), or 1 for local/open/TCP-IP.
int32_t __stdcall GAME_IsLadderOrSinglePlayer(void)
{
    if (gnGameType >= 2 && gnGameType <= 3)
    {
        return gbIsLadderClient;
    }
    return 1;
}

// 1.12a: 0x6FB29D10 (register arg in eax)
void __fastcall GAME_SetLadderFlag(int32_t bIsLadder)
{
    gbIsLadderClient = bIsLadder;
    g_tClientTiming.bIsLadderClient = bIsLadder;
}

// 1.12a: 0x6FB29D20 (register arg in al)
void __fastcall GAME_SetExpansionFlag(uint8_t bExpansion)
{
    gbIsExpansion = static_cast<int32_t>(bExpansion);
    g_tClientTiming.bIsExpansion = gbIsExpansion;
}

// 1.12a: 0x6FB29D30 (register arg in al)
void __fastcall GAME_SetDifficulty(uint8_t nDifficulty)
{
    gnClientDifficulty = nDifficulty;
}

// 1.12a: 0x6FB29D40
int32_t __stdcall GAME_GetColorTxtFlag(void)
{
    return g_bColorTxtFlag;
}

// 1.12a: 0x6FB29D50
void __stdcall GAME_ToggleColorTxtFlag(void)
{
    g_bColorTxtFlag = (g_bColorTxtFlag == 0) ? 1 : 0;
}

// 1.12a: 0x6FB29D70
void __stdcall GAME_LoadColorTxtFlag(void)
{
    if (g_bColorTxtFlag != 0)
    {
        return;
    }

    HSFILE hFile = nullptr;
    if (!FOG_FOpenFile("Data\\Local\\Color.txt", &hFile))
    {
        if (GetLastError() != ERROR_FILE_NOT_FOUND)
        {
            FOG_Trace("Error opening file: %s", "Data\\Local\\Color.txt");
        }
        return;
    }

    uint8_t chFlag = 0;
    ARCHIVE_ReadMpqaFileBytes(hFile, 1, &chFlag);
    if (hFile == nullptr)
    {
        FOG_DisplayHalt("", Fog_10265(0x49), 0x49);
        exit(-1);
    }

    FOG_FCloseFile(hFile);
    if (chFlag == '1')
    {
        g_bColorTxtFlag = 1;
    }
}

// 1.12a: 0x6FB29E10
void __stdcall GAME_PreloadActTileLibrary(uint8_t nAct)
{
    const uint8_t nActOneBased = static_cast<uint8_t>(nAct + 1);
    D2TileLibraryHashStrc* apTileLibraryHash[32] = {};
    char szTileFileName[60] = {};

    for (int32_t nLevelType = 0; nLevelType < 36; ++nLevelType)
    {
        if (DATATBLS_CheckActInLevelTypesTxt(nLevelType, nActOneBased))
        {
            memset(apTileLibraryHash, 0, sizeof(apTileLibraryHash));
            for (int32_t nFileId = 0; nFileId < 32; ++nFileId)
            {
                DATATBLS_GetFileNameFromLevelTypeAndFileId(nLevelType, nFileId, szTileFileName);
                if (szTileFileName[0] != '0' && szTileFileName[0] != '\0')
                {
                    D2CMP_10087_LoadTileLibrarySlot(apTileLibraryHash, szTileFileName);
                }
            }
        }
    }

    memset(apTileLibraryHash, 0, sizeof(apTileLibraryHash));
    D2CMP_10087_LoadTileLibrarySlot(apTileLibraryHash, "data\\global\\tiles\\act1\\barracks\\warp.dt1");
    D2CMP_10087_LoadTileLibrarySlot(apTileLibraryHash, "data\\global\\tiles\\act1\\outdoors\\blank.dt1");
}

// 1.12a: 0x6FB29E90
void __stdcall GAME_ExtendGracePeriod(uint32_t dwExtraMs)
{
    const uint32_t dwTargetTick = GetTickCount() + dwExtraMs;
    if (dwTargetTick > gdwReassignGraceTick)
    {
        gdwReassignGraceTick = dwTargetTick;
        g_tClientTiming.dwReassignGraceTick = dwTargetTick;
    }
}

// 1.12a: 0x6FB29EB0
int32_t __stdcall GAME_GetGameType(void)
{
    return gnGameType;
}

// 1.12a: 0x6FB29EC0
void* __stdcall GAME_GetClientPoolSystem(void)
{
    return g_tClientTiming.pClientPoolSystem;
}

// 1.12a: 0x6FB29ED0
int32_t __stdcall GAME_GetServerStatusFlag(void)
{
    return g_tClientTiming.nServerStatusFlag;
}

// 1.12a: 0x6FB29EE0
uint32_t __stdcall GAME_GetClientGameTick(void)
{
    return gdwClientGameTick;
}

// 1.12a: 0x6FB29EF0
int32_t __stdcall GAME_GetSkippedFps(void)
{
    return g_tClientTiming.nSkippedFps;
}

// 1.12a: 0x6FB29F00
int32_t __stdcall GAME_GetMeasuredFps(void)
{
    return g_tClientTiming.nMeasuredFps;
}

// 1.12a: 0x6FB29F10 (register arg in eax)
void __fastcall GAME_SetNetworkQualityMetric(int32_t nMetric)
{
    g_tClientTiming.nNetworkQualityMetric = nMetric;
}

// 1.12a: 0x6FB29F20
int32_t __stdcall GAME_GetNetworkQualityMetric(void)
{
    return g_tClientTiming.nNetworkQualityMetric;
}

// 1.12a: 0x6FB29F30
uint32_t __stdcall GAME_GetRenderedFrameCount(void)
{
    return g_tClientTiming.dwRenderedFrameCount;
}

// 1.12a: 0x6FB29F40
uint32_t __stdcall GAME_GetCurrentTickMs(void)
{
    return g_tClientTiming.dwCurrentTickMs;
}

// 1.12a: 0x6FB29F50
uint32_t __stdcall GAME_GetClientFrameCount(void)
{
    return gdwClientGameTick;
}

// 1.12a: 0x6FB29F60
void __stdcall GAME_ClearMouseHoldFlags(void)
{
    g_bLeftMouseHeld      = 0;
    g_bRightMouseHeld     = 0;
    g_bLeftMouseRepeating = 0;
    g_bRightMouseRepeating = 0;
}

// 1.12a: 0x6FB29F80
void __stdcall GAME_ResetAllMouseStates(void)
{
    g_nRightMouseButtonState = 0x10;
    g_nLeftMouseButtonState  = 0x10;
    g_bLeftMouseHeld         = 0;
    g_bRightMouseHeld        = 0;
    g_bLeftMouseRepeating    = 0;
    g_bRightMouseRepeating   = 0;
    g_bSkillTreeHold         = 0;
    g_bSkillSelectHold       = 0;
}

// 1.12a: 0x6FB29FB0
int32_t __stdcall GAME_GetExitGameFlag(void)
{
    return g_bExitGameFlag;
}

// 1.12a: 0x6FB29FC0 (register arg pCfg in edi)
int32_t __fastcall GAME_ApplyConfigTable(D2ConfigStrc* pCfg)
{
    g_bConfigApplyResult = 1;
    for (size_t i = 0; i < 49; ++i)
    {
        if (g_apfnApplyConfigCallbacks[i] != nullptr)
        {
            g_apfnApplyConfigCallbacks[i](pCfg);
        }
    }
    return g_bConfigApplyResult;
}

// ============================================================================
// SECTION 2: Command-Line / D2ConfigStrc Option Handlers
// (0x6FB29FF0 .. 0x6FB2A4AF)
// ============================================================================

// 1.12a: 0x6FB29FF0
void __fastcall GAME_ConfigApply_NoOp29FF0(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A000
void __fastcall GAME_ConfigApply_TxtMode(D2ConfigStrc* pCfg)
{
    DATATBLS_GetCompositTxtRecord((pCfg->bTxt == 0) ? 1 : 0);
}

// 1.12a: 0x6FB2A020
void __fastcall GAME_ConfigApply_CompressMode(D2ConfigStrc* pCfg)
{
    if (pCfg->bNoCompress != 0)
    {
        D2CMP_SetCompressedDataMode(static_cast<BOOL>(pCfg->bNoCompress));
    }
}

// 1.12a: 0x6FB2A040
void __fastcall GAME_ConfigApply_NoOp2A040(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A050
void __fastcall GAME_ConfigApply_NoOp2A050(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A060
void __fastcall GAME_ConfigApply_ComInterface(D2ConfigStrc* pCfg)
{
    if (pCfg->pComInterface != nullptr)
    {
        g_pBnetComInterface = pCfg->pComInterface;
    }
}

// 1.12a: 0x6FB2A070
void __fastcall GAME_ConfigApply_CharTemplate(D2ConfigStrc* pCfg)
{
    g_nArenaCharTemplate = static_cast<int32_t>(pCfg->unpackedCTemp.nUnk);
}

// 1.12a: 0x6FB2A080
void __fastcall GAME_ConfigApply_NoOp2A080(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A090
void __fastcall GAME_ConfigApply_NoOp2A090(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A0A0
void __fastcall GAME_ConfigApply_NoOp2A0A0(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A0B0
void __fastcall GAME_ConfigApply_CheatsArenaFlag(D2ConfigStrc* pCfg)
{
    (void)pCfg;
    gdwArenaFlags |= 1u;
}

// 1.12a: 0x6FB2A0C0
void __fastcall GAME_ConfigApply_RealmName(D2ConfigStrc* pCfg)
{
    SStrCopy(g_szConfigRealm, pCfg->szRealm, sizeof(g_szConfigRealm));
}

// 1.12a: 0x6FB2A0E0
void __fastcall GAME_ConfigApply_CharName(D2ConfigStrc* pCfg)
{
    if (strcmp(pCfg->szName, "0") != 0)
    {
        memset(g_szConfigCharName, 0, sizeof(g_szConfigCharName));
        size_t nCopyLen = strlen(pCfg->szName) + 1;
        if (nCopyLen > sizeof(g_szConfigCharName))
        {
            nCopyLen = sizeof(g_szConfigCharName);
        }
        strncpy(g_szConfigCharName, pCfg->szName, nCopyLen);
    }
}

// 1.12a: 0x6FB2A150
void __fastcall GAME_ConfigApply_NoOp2A150(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A160
void __fastcall GAME_ConfigApply_Seed(D2ConfigStrc* pCfg)
{
    if (pCfg->dwSeed != 0)
    {
        GAME_SetInitSeed(static_cast<int32_t>(pCfg->dwSeed));
    }
}

// 1.12a: 0x6FB2A180
void __fastcall GAME_ConfigApply_Log(D2ConfigStrc* pCfg)
{
    if (pCfg->bLog != 0)
    {
        LOG_11102(reinterpret_cast<void*>(static_cast<intptr_t>(1)));
    }
}

// 1.12a: 0x6FB2A1A0
void __fastcall GAME_ConfigApply_NoOp2A1A0(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A1B0
void __fastcall GAME_ConfigApply_NoOp2A1B0(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A1C0
void __fastcall GAME_ConfigApply_NoOp2A1C0(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A1D0
void __fastcall GAME_ConfigApply_NoOp2A1D0(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A1E0
void __fastcall GAME_ConfigApply_NoOp2A1E0(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A1F0
void __fastcall GAME_ConfigApply_VSync(D2ConfigStrc* pCfg)
{
    if (pCfg->bVSync != 0)
    {
        D2GFX_EnableVSync();
    }
}

// 1.12a: 0x6FB2A200
void __fastcall GAME_ConfigApply_Gamma(D2ConfigStrc* pCfg)
{
    if (pCfg->dwGamma != 0)
    {
        D2GFX_SetGamma(pCfg->dwGamma);
    }
}

// 1.12a: 0x6FB2A210
void __fastcall GAME_ConfigApply_NoOp2A210(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A220
void __fastcall GAME_ConfigApply_RenderRave(D2ConfigStrc* pCfg)
{
    if (pCfg->bRave != 0)
    {
        g_nDisplayRenderMode = 7;
    }
}

// 1.12a: 0x6FB2A240
void __fastcall GAME_ConfigApply_RenderD3D(D2ConfigStrc* pCfg)
{
    if (pCfg->bD3D != 0)
    {
        g_nDisplayRenderMode = 6;
    }
}

// 1.12a: 0x6FB2A260
void __fastcall GAME_ConfigApply_RenderOpenGL(D2ConfigStrc* pCfg)
{
    if (pCfg->bOpenGL != 0)
    {
        g_nDisplayRenderMode = 5;
    }
}

// 1.12a: 0x6FB2A280
void __fastcall GAME_ConfigApply_Render3DFX(D2ConfigStrc* pCfg)
{
    if (pCfg->b3DFX != 0)
    {
        g_nDisplayRenderMode = 4;
    }
}

// 1.12a: 0x6FB2A2A0
void __fastcall GAME_ConfigApply_RenderWindowed(D2ConfigStrc* pCfg)
{
    if (pCfg->bWindow != 0)
    {
        g_nDisplayRenderMode = 1;
        g_bWindowedMode      = 1;
    }
}

// 1.12a: 0x6FB2A2C0
void __fastcall GAME_ConfigApply_NoOp2A2C0(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A2D0
void __fastcall GAME_ConfigApply_NoOp2A2D0(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A2E0
void __fastcall GAME_ConfigApply_NoOp2A2E0(D2ConfigStrc* pCfg)
{
    (void)pCfg;
}

// 1.12a: 0x6FB2A2F0
void __fastcall GAME_ConfigApply_PlayerClass(D2ConfigStrc* pCfg)
{
    if (pCfg->bAmazon != 0)      g_nSelectedCharClass = PCLASS_AMAZON;
    if (pCfg->bPaladin != 0)     g_nSelectedCharClass = PCLASS_PALADIN;
    if (pCfg->bSorceress != 0)   g_nSelectedCharClass = PCLASS_SORCERESS;
    if (pCfg->bNecromancer != 0) g_nSelectedCharClass = PCLASS_NECROMANCER;
    if (pCfg->bBarbarian != 0)   g_nSelectedCharClass = PCLASS_BARBARIAN;
    if (pCfg->bDruid != 0)       g_nSelectedCharClass = PCLASS_DRUID;
    if (pCfg->bAssassin != 0)    g_nSelectedCharClass = PCLASS_ASSASSIN;
}

// 1.12a: 0x6FB2A370
void __fastcall GAME_ConfigApply_McpIp(D2ConfigStrc* pCfg)
{
    if (strcmp(pCfg->szMCPIP, "0") != 0)
    {
        SStrCopy(g_szMcpServerIp, pCfg->szMCPIP, INT_MAX);
    }
}

// 1.12a: 0x6FB2A3A0
void __fastcall GAME_ConfigApply_BattleNetIp(D2ConfigStrc* pCfg)
{
    if (strcmp(pCfg->szBattleNetIP, "0") != 0)
    {
        SStrCopy(g_szBattleNetIp, pCfg->szServerIP, INT_MAX);
    }
}

// 1.12a: 0x6FB2A3D0
void __fastcall GAME_ConfigApply_NetworkSession(D2ConfigStrc* pCfg)
{
    if (strcmp(pCfg->szServerIP, "0") != 0)
    {
        SStrCopy(g_szGameServerIp, pCfg->szServerIP, INT_MAX);
    }
    SStrCopy(g_szConfigGameName, pCfg->szGameName, INT_MAX);
    gnGameType          = static_cast<int32_t>(pCfg->dwGameType);
    g_wJoinGameId       = pCfg->wJoinID;
    g_dwJoinServerToken = pCfg->nTokenId;
}

// 1.12a: 0x6FB2A430
void __fastcall GAME_ConfigApply_Act(D2ConfigStrc* pCfg)
{
    const int32_t nActOneBased = static_cast<int32_t>(pCfg->dwAct);
    if (nActOneBased == 0)
    {
        return;
    }

    if (nActOneBased >= 6)
    {
        FOG_DisplayHalt("", Fog_10265(0xDA8), 0xDA8);
        exit(-1);
    }
    if (nActOneBased <= 0)
    {
        FOG_DisplayHalt("", Fog_10265(0xDA9), 0xDA9);
        exit(-1);
    }

    GAME_SetGlobalAct(nActOneBased - 1);
}

// 1.12a: 0x6FB2A480
void __fastcall GAME_ConfigApply_NoSave(D2ConfigStrc* pCfg)
{
    if (pCfg->bNoSave != 0)
    {
        D2GAME_10036_PLRSAVE_EnableSaveFileWriting(0);
    }
}

// 1.12a: 0x6FB2A4A0
BnClientInterface* __stdcall GAME_GetBnetComInterface(void)
{
    return g_pBnetComInterface;
}

// ============================================================================
// SECTION 3: End-Game Flags, OS Mouse Capability Checks, Pong, FPS & Input
// (0x6FB2A4B0 .. 0x6FB2B05F)
// ============================================================================

// 1.12a: 0x6FB2A4B0 (register arg in eax)
void __fastcall GAME_SetPlayExpansionEpilogueMovie(int32_t bPlayMovie)
{
    g_bPlayExpansionEpilogueMovie = bPlayMovie;
}

// 1.12a: 0x6FB2A4C0
int32_t __fastcall GAME_HasFourButtonMouse(void)
{
    if (g_bOsVersionValid != 0 &&
        g_tOsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
        g_tOsVersionInfo.dwMajorVersion == 5)
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB2A4F0
int32_t __fastcall GAME_HasThreeButtonMouse(void)
{
    if (g_bOsVersionValid == 0)
    {
        return 0;
    }
    if (g_tOsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
        return 1;
    }
    if (g_tOsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
        (g_tOsVersionInfo.dwMajorVersion == 4 || g_tOsVersionInfo.dwMajorVersion == 5))
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB2A520
int32_t __fastcall GAME_HasMouseWheelB(void)
{
    if (!GetSystemMetrics(SM_MOUSEWHEELPRESENT) || g_bOsVersionValid == 0)
    {
        return 0;
    }
    if (g_tOsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
        return (g_tOsVersionInfo.dwMinorVersion != 0) ? 1 : 0;
    }
    if (g_tOsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
        (g_tOsVersionInfo.dwMajorVersion == 4 || g_tOsVersionInfo.dwMajorVersion == 5))
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB2A570
int32_t __fastcall GAME_HasMouseWheelA(void)
{
    if (!GetSystemMetrics(SM_MOUSEWHEELPRESENT) || g_bOsVersionValid == 0)
    {
        return 0;
    }
    if (g_tOsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
        return (g_tOsVersionInfo.dwMinorVersion == 0) ? 1 : 0;
    }
    return 0;
}

// 1.12a: 0x6FB2A5B0
void __stdcall GAME_SetExitGameFlag(void)
{
    g_bExitGameFlag = 1;
}

// 1.12a: 0x6FB2A5C0
void __stdcall GAME_TriggerEndGameSequence(void)
{
    g_bShowExpansionEndGameScreen = 1;
}

// 1.12a: 0x6FB2A5D0
void __stdcall GAME_TriggerBetaEndGameSequence(void)
{
    g_bShowBetaScreens       = 1;
    g_nCurrentBetaScreenIdx  = 0;
    MSG_SendLeaveGame_69();
    g_bExitGameFlag          = 1;
}

// 1.12a: 0x6FB2A600
void CALLBACK GAME_OnEndGameKeyDown(SMSGHANDLER_PARAMS* pMsg)
{
    if ((pMsg->lParam & 0x40000000) == 0)
    {
        g_bEndGameScreenDismissed = 1;
    }
    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2A630
void CALLBACK GAME_OnBetaScreenKeyDown(SMSGHANDLER_PARAMS* pMsg)
{
    if ((pMsg->lParam & 0x40000000) == 0)
    {
        g_nCurrentBetaScreenIdx += 1;
    }
    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2A660
void CALLBACK GAME_OnBetaScreenMouseDown(SMSGHANDLER_PARAMS* pMsg)
{
    g_nCurrentBetaScreenIdx += 1;
    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2A690
int32_t __stdcall GAME_WaitJoinScreen(void)
{
    const int32_t nNextState = g_nJoinStartNetState;
    if (nNextState == D2CLIENT_STATE_START_NET)
    {
        if (g_nClientStateStackDepth >= nNextState)
        {
            FOG_DisplayHalt("", Fog_10265(0xA6), 0xA6);
            exit(-1);
        }
        g_anClientStateStack[g_nClientStateStackDepth] = D2CLIENT_STATE_GAME_LOOP;
        g_nClientStateStackDepth += 1;
    }
    return nNextState;
}

// 1.12a: 0x6FB2A6E0
uint32_t* __stdcall GAME_GetPongPayloadBuffer(void)
{
    return g_tClientTiming.adwPongPayload;
}

// 1.12a: 0x6FB2A6F0
uint32_t __stdcall GAME_GetAverageInitialPing(void)
{
    return g_tClientTiming.dwAverageInitialPing;
}

// 1.12a: 0x6FB2A700
uint32_t __stdcall GAME_GetClientPing(void)
{
    return gdwClientPing;
}

// 1.12a: 0x6FB2A710
int32_t __stdcall GAME_GetAutoSkillState(void)
{
    return g_tClientTiming.nAutoSkillState;
}

// 1.12a: 0x6FB2A720 (register arg in eax)
void __fastcall GAME_SetAutoSkillState(int32_t nState)
{
    g_tClientTiming.nAutoSkillState = nState;
}

// 1.12a: 0x6FB2A730 (register arg pPacket in eax)
void __fastcall GAME_HandlePongPacket(const uint8_t* pPacket)
{
    const D2PongPacketSrv8FStrc* pPong = reinterpret_cast<const D2PongPacketSrv8FStrc*>(pPacket);
    for (int32_t i = 0; i < 8; ++i)
    {
        g_tClientTiming.adwPongPayload[i] = pPong->adwPongDwords[i];
    }

    g_tClientTiming.adwPongPayload[4] = GetTickCount();
    const uint32_t dwRtt = GetTickCount() - g_tClientTiming.dwLastPingSentTick;
    gdwClientPing = dwRtt;
    g_tClientTiming.dwClientPing = dwRtt;

    if (g_tClientTiming.dwInitialPingCount < 10u)
    {
        const uint32_t dwPrevCount = g_tClientTiming.dwInitialPingCount;
        const uint32_t dwNewCount  = dwPrevCount + 1u;
        g_tClientTiming.dwInitialPingCount   = dwNewCount;
        g_tClientTiming.dwAverageInitialPing = (g_tClientTiming.dwAverageInitialPing * dwPrevCount + dwRtt) / dwNewCount;
    }
}

// 1.12a: 0x6FB2A7C0
void __stdcall GAME_ResetFpsCounters(void)
{
    g_tClientTiming.dwFpsSampleStartTick = g_tClientTiming.dwCurrentTickMs;
    g_tClientTiming.nMeasuredFps         = 25;
    g_tClientTiming.nSkippedFps          = 0;
    g_tClientTiming.nFpsSampleFrames     = 0;
    g_tClientTiming.nSkippedSampleFrames = 0;
}

// 1.12a: 0x6FB2A7F0
void __stdcall GAME_UpdateFpsCounters(void)
{
    const uint32_t dwCurrentTick = g_tClientTiming.dwCurrentTickMs;
    if ((dwCurrentTick - g_tClientTiming.dwFpsSampleStartTick) > 3000u)
    {
        g_tClientTiming.nMeasuredFps         = g_tClientTiming.nFpsSampleFrames / 3;
        g_tClientTiming.nSkippedFps          = g_tClientTiming.nSkippedSampleFrames / 3;
        g_tClientTiming.dwFpsSampleStartTick = dwCurrentTick;
        g_tClientTiming.nFpsSampleFrames     = 0;
        g_tClientTiming.nSkippedSampleFrames = 0;
    }
}

// 1.12a: 0x6FB2A850 (ecx = nMissedTicks, edx = nMeasuredFps)
int32_t __fastcall GAME_CalcMaxFrameSkip(int32_t nMissedTicks, int32_t nMeasuredFps)
{
    int32_t nMaxSkip = (nMeasuredFps < 7) ? 1 : 0;
    if (nMissedTicks > 1)  nMaxSkip += 1;
    if (nMissedTicks > 9)  nMaxSkip += 1;
    if (nMissedTicks > 19) nMaxSkip += 1;
    if (nMissedTicks > 29) nMaxSkip += 1;
    return nMaxSkip;
}

// 1.12a: 0x6FB2A880 (register arg in eax)
void __fastcall GAME_SetRenderFrameMode(int32_t nMode)
{
    g_tClientTiming.nRenderFrameMode = nMode;
    g_tClientTiming.pfnRenderFrame   = g_apfnRenderFrameModes[nMode];
}

// 1.12a: 0x6FB2A8A0
int32_t __stdcall GAME_GetWindowClosedExit(void)
{
    return g_bWindowClosedExit;
}

// 1.12a: 0x6FB2A8B0 (register arg in eax)
void __fastcall GAME_SetWindowClosedExit(int32_t bClosedExit)
{
    g_bWindowClosedExit = bClosedExit;
}

// 1.12a: 0x6FB2A8C0
void __stdcall GAME_DestroyWindowAndGfxCallback(int32_t nArg1, int32_t nArg2, int32_t nArg3)
{
    (void)nArg1;
    (void)nArg2;
    (void)nArg3;
    WINDOW_Destroy();
    D2GFX_Release();
}

// 1.12a: 0x6FB2A8D0
void __stdcall GAME_NoOp2A8D0(void)
{
}

// 1.12a: 0x6FB2A8E0
void CALLBACK GAME_OnWinKeyEat(SMSGHANDLER_PARAMS* pMsg)
{
    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2A900
void CALLBACK GAME_OnSysCommand(SMSGHANDLER_PARAMS* pMsg)
{
    const uint16_t wSysCmd = static_cast<uint16_t>(pMsg->wParam & 0xFFF0u);
    if (wSysCmd == SC_TASKLIST ||
        wSysCmd == SC_KEYMENU ||
        (wSysCmd == SC_MOVE && WINDOW_IsFullScreen()))
    {
        pMsg->bUseResult = TRUE;
        pMsg->lResult    = 0;
        SMsgBreakHandlerChain(pMsg);
    }
}

// 1.12a: 0x6FB2A950
void __stdcall GAME_CancelRightMouseHold(void)
{
    g_nRightMouseButtonState = 0x10;
    g_bRightMouseHeld        = 0;
    g_bRightMouseRepeating   = 1;
    g_bSuppressLeftMouseHold = 0;
    g_bSkillTreeHold         = 0;
}

// 1.12a: 0x6FB2A980 (retail passes nX in edi, nY in ebx)
int32_t __fastcall GAME_IsPointInsideRightOpenPanel(int32_t nX, int32_t nY)
{
    if (gnOpenPanelMode != 1)
    {
        return 0;
    }
    const int32_t nHalfWidth = gnScreenWidth / 2;
    if (nX > nHalfWidth && nX < gnScreenWidth && nY > 0 && nY < gnViewportHeight)
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB2A9C0 (retail passes nX in esi, nY in edi)
int32_t __fastcall GAME_IsPointInsideLeftOpenPanel(int32_t nX, int32_t nY)
{
    if (gnOpenPanelMode != 2)
    {
        return 0;
    }
    const int32_t nHalfWidth = gnScreenWidth / 2;
    if (nX > 0 && nX < nHalfWidth && nY > 0 && nY < (gnScreenHeight - 47))
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB2AA00
int32_t __stdcall GAME_IsRightMouseRepeating(void)
{
    return (g_bRightMouseRepeating != 0) ? 1 : 0;
}

// 1.12a: 0x6FB2AA10
int32_t __stdcall GAME_IsLeftMouseRepeating(void)
{
    return (g_bLeftMouseRepeating != 0) ? 1 : 0;
}

// 1.12a: 0x6FB2AA20
int32_t __stdcall GAME_IsLeftMouseHeld(void)
{
    return g_bLeftMouseHeld;
}

// 1.12a: 0x6FB2AA30
int32_t __stdcall GAME_IsRightMouseHeld(void)
{
    return g_bRightMouseHeld;
}

// 1.12a: 0x6FB2AA40
int32_t __stdcall GAME_GetRightMouseButtonState(void)
{
    return g_nRightMouseButtonState;
}

// 1.12a: 0x6FB2AA50
int32_t __stdcall GAME_GetLeftMouseButtonState(void)
{
    return g_nLeftMouseButtonState;
}

// 1.12a: 0x6FB2AA60
void __stdcall GAME_SuppressLeftMouseHold(void)
{
    g_bSuppressLeftMouseHold = 1;
}

// 1.12a: 0x6FB2AA70
uint32_t __stdcall GAME_BuildMovementKeyFlags(void)
{
    uint32_t dwFlags = 0;
    if (gbRunLock != 0)
    {
        dwFlags |= MK_CONTROL; // 0x08
    }
    if (gbAlwaysRun != 0)
    {
        dwFlags |= MK_CONTROL; // 0x08
    }
    if (gbStandStillHeld != 0)
    {
        dwFlags |= MK_SHIFT;   // 0x04
    }
    return dwFlags;
}

// 1.12a: 0x6FB2AAA0
int32_t __stdcall GAME_GetStandStillHeld(void)
{
    return gbStandStillHeld;
}

// 1.12a: 0x6FB2AAB0 (register arg in eax)
void __fastcall GAME_SetStandStillHeld(int32_t bHeld)
{
    gbStandStillHeld = bHeld;
}

// 1.12a: 0x6FB2AAC0
int32_t __stdcall GAME_GetAlwaysRun(void)
{
    return gbAlwaysRun;
}

// 1.12a: 0x6FB2AAD0 (register arg in eax)
void __fastcall GAME_SetAlwaysRun(int32_t bAlwaysRun)
{
    gbAlwaysRun = bAlwaysRun;
}

// 1.12a: 0x6FB2AAE0
int32_t __stdcall GAME_GetRunLock(void)
{
    return gbRunLock;
}

// 1.12a: 0x6FB2AAF0 (register arg in eax)
void __fastcall GAME_SetRunLock(int32_t bRunLock)
{
    gbRunLock = bRunLock;
}

// 1.12a: 0x6FB2AB00
void __stdcall GAME_RequestExitGame(void)
{
    g_bExitAppFlag      = 0;
    g_bWindowClosedExit = 1;
    MSG_SendLeaveGame_69();
    g_bExitGameFlag     = 1;
}

// 1.12a: 0x6FB2AB30
void __stdcall GAME_RegisterMouseWheelSupport(void)
{
    if (GAME_HasMouseWheelB())
    {
        WALL2_RegisterMsgHandlerTable(1, WINDOW_GetWindow(), g_aMouseWheelSMsgTable);
    }

    if (GAME_HasThreeButtonMouse())
    {
        WALL2_RegisterMsgHandlerTable(2, WINDOW_GetWindow(), g_aMiddleMouseSMsgTable);
    }

    if (GAME_HasFourButtonMouse())
    {
        WALL2_RegisterMsgHandlerTable(2, WINDOW_GetWindow(), g_aXButtonMouseSMsgTable);
    }

    if (GAME_HasMouseWheelA())
    {
        const UINT uWheelSupportMsg = RegisterWindowMessageA("MSH_WHEELSUPPORT_MSG");
        const HWND hWheelWnd        = FindWindowA("MouseZ", "Magellan MSWHEEL");
        if (hWheelWnd != nullptr && uWheelSupportMsg != 0)
        {
            if (SendMessageA(hWheelWnd, uWheelSupportMsg, 0, 0) != 0)
            {
                g_aMagellanWheelSMsgTable[0].uMsgOrId = uWheelSupportMsg;
                WALL2_RegisterMsgHandlerTable(1, WINDOW_GetWindow(), g_aMagellanWheelSMsgTable);
            }
        }
    }
}

// 1.12a: 0x6FB2AC70
void __stdcall GAME_UnregisterMouseWheelSupport(void)
{
    if (GAME_HasMouseWheelB())
    {
        WALL2_UnregisterMsgHandlerTable(1, WINDOW_GetWindow(), g_aMouseWheelSMsgTable);
    }

    if (GAME_HasThreeButtonMouse())
    {
        WALL2_UnregisterMsgHandlerTable(2, WINDOW_GetWindow(), g_aMiddleMouseSMsgTable);
    }

    if (GAME_HasFourButtonMouse())
    {
        WALL2_UnregisterMsgHandlerTable(2, WINDOW_GetWindow(), g_aXButtonMouseSMsgTable);
    }

    if (GAME_HasMouseWheelA() && g_aInGameSMsgTable[0].uMsgOrId != 0)
    {
        WALL2_UnregisterMsgHandlerTable(1, WINDOW_GetWindow(), g_aMagellanWheelSMsgTable);
    }
}

// 1.12a: 0x6FB2AD80
void __stdcall GAME_TriggerClassicEndGameSequence(void)
{
    g_bShowClassicEndGameScreen = 1;
    if (g_pGameData != nullptr)
    {
        const uint16_t wDiffRank  = static_cast<uint16_t>((static_cast<uint16_t>(gnClientDifficulty) + 1u) << 10);
        const uint16_t wSaveFlags = static_cast<uint16_t>((g_pGameData->unpackedCTemp.nCharacterSaveFlags & 0xE0FFu) | wDiffRank);
        g_pGameData->unpackedCTemp.nCharacterSaveFlags = wSaveFlags;
    }
}

// 1.12a: 0x6FB2ADC0
int32_t __stdcall GAME_CalcCenteredStringX(uint16_t wStringId, int32_t nCenterX)
{
    Unicode wszText[100] = {};
    Unicode::strcpy(wszText, D2LANG_GetStringFromTblIndex(static_cast<short>(wStringId)));
    const int32_t nHalfWidth = D2Win_10121_GetTextWidth(wszText) / 2;
    return nCenterX - nHalfWidth;
}

// 1.12a: 0x6FB2AE20
void __stdcall GAME_RenderEndGameCharacterTitle(void)
{
    D2ConfigStrc* pCfg = g_pGameData;
    if (pCfg == nullptr)
    {
        return;
    }

    Unicode wszCharName[100] = {};
    char szCharName[100]     = {};
    strcpy(szCharName, pCfg->szName);
    Unicode::toUnicode(wszCharName, szCharName, 64);

    const int32_t nNameHalfWidth = D2Win_10121_GetTextWidth(wszCharName) / 2;
    D2Win_10117_DrawText(wszCharName, (gnScreenWidth / 2) - nNameHalfWidth, 187, 4, FALSE);

    const uint8_t  nDiff      = gnClientDifficulty;
    const uint8_t  nCharClass = g_nSelectedCharClass;
    const uint16_t wSaveFlags = pCfg->unpackedCTemp.nCharacterSaveFlags;
    const int32_t  nHardcore  = static_cast<int32_t>(wSaveFlags & 0x04u);
    const BOOL     bExpansion = static_cast<BOOL>(wSaveFlags & 0x20u);

    Unicode wszFullTitle[100] = {};
    int32_t nRank = 0;
    if (nDiff == 0)
    {
        nRank = (bExpansion != 0 ? 1 : 0) + 4;
    }
    else if (nDiff == 1)
    {
        nRank = ((bExpansion != 0 ? 1 : 0) * 2) + 8;
    }
    else if (nDiff == 2)
    {
        nRank = (bExpansion != 0 ? 3 : 0) + 12;
    }
    else
    {
        return;
    }

    const char* szTitlePrefix = D2Win_10161(static_cast<int>(nCharClass), nRank, nHardcore, bExpansion);
    if (szTitlePrefix == nullptr)
    {
        return;
    }

    char szFullTitle[200] = {};
    strcpy(szFullTitle, szTitlePrefix);
    strcat(szFullTitle, szCharName);
    Unicode::toUnicode(wszFullTitle, szFullTitle, 64);

    const int32_t nTitleHalfWidth = D2Win_10121_GetTextWidth(wszFullTitle) / 2;
    const int32_t nTitleY         = (gbIsExpansion != 0) ? 357 : 277;
    D2Win_10117_DrawText(wszFullTitle, (gnScreenWidth / 2) - nTitleHalfWidth, nTitleY, 4, FALSE);
}

// 1.12a: 0x6FB2B020 (retail passes nLeft in eax, nBottom in edx, nWidth in esi, nHeight on stack)
int32_t __fastcall GAME_IsMouseInsideButtonRect(int32_t nLeft, int32_t nBottom, int32_t nWidth, int32_t nHeight)
{
    if (gnMouseX >= nLeft && gnMouseX <= (nLeft + nWidth))
    {
        const int32_t nTop = nBottom - nHeight;
        if (gnMouseY >= nTop && gnMouseY <= nBottom)
        {
            return 1;
        }
    }
    return 0;
}

// ============================================================================
// SECTION 4: Open Server Thread, Ping, Disconnect Screen & Extra Mouse Buttons
// (0x6FB2B060 .. 0x6FB2BDCE)
// ============================================================================

// 1.12a: 0x6FB2B060
DWORD WINAPI GAME_OpenServerThreadProc(LPVOID lpParameter)
{
    (void)lpParameter;
    int32_t nSavedPlayerCount = 0;
    int32_t bUpdatedGames     = 0;

    EnterCriticalSection(&g_csServerThread);
    int32_t bStopThread  = g_bStopServerThread;
    int32_t bServerReady = g_bServerThreadReady;
    LeaveCriticalSection(&g_csServerThread);

    while (!bStopThread)
    {
        if (bServerReady != 0)
        {
            GAME_ProcessNetworkMessages();
            bUpdatedGames = GAME_UpdateGamesProgress(1);
            if (bUpdatedGames != 0)
            {
                GAME_UpdateClients(0, 0);
            }
        }

        if (bUpdatedGames != 0)
        {
            Sleep(30);

            D2ConfigStrc* pCfg = g_pGameData;
            BnClientInterface* pCom = (pCfg != nullptr) ? pCfg->pComInterface : nullptr;
            if (pCom != nullptr)
            {
                const int32_t nPlayerCount = GAME_GetPlayerUnitsCount(1);
                if (nPlayerCount != nSavedPlayerCount)
                {
                    const int32_t nMaxPlayers = g_nMaxOpenGamePlayers;
                    if (nSavedPlayerCount < nMaxPlayers)
                    {
                        if (nPlayerCount >= nMaxPlayers)
                        {
                            pCom->SendKeepAlivePacket();
                        }
                    }
                    else if (nPlayerCount < nMaxPlayers)
                    {
                        int32_t nGameFlags = 0;
                        if (pCfg->nArenaDifficulty == 1)
                        {
                            nGameFlags = 1;
                        }
                        if (pCfg->nArenaDifficulty == 2)
                        {
                            nGameFlags |= 2;
                        }
                        const uint16_t wSaveFlags = pCfg->unpackedCTemp.nCharacterSaveFlags;
                        if ((wSaveFlags & 0x04u) != 0)
                        {
                            nGameFlags |= 4;
                        }
                        if ((wSaveFlags & 0x20u) != 0)
                        {
                            nGameFlags |= 8;
                        }
                        pCom->RefreshGame(pCfg->szGameName, pCfg->szGamePassword, pCfg->szGameStatstring, nGameFlags);
                    }
                    nSavedPlayerCount = nPlayerCount;
                }
            }
        }
        else
        {
            Sleep(10);
        }

        EnterCriticalSection(&g_csServerThread);
        bStopThread  = g_bStopServerThread;
        bServerReady = g_bServerThreadReady;
        LeaveCriticalSection(&g_csServerThread);
    }

    GAME_CloseAllGames();
    GAME_UpdateClients(0, 1);
    DeleteCriticalSection(&g_csServerThread);
    OutputDebugStringA("Open server thread shut down\n");
    return 1;
}

// 1.12a: 0x6FB2B1C0
void __stdcall GAME_SendPeriodicPing(void)
{
    const uint32_t dwNow = GetTickCount();
    if (g_tClientTiming.dwNextPingSendTick < dwNow)
    {
        MSG_SendPing_6D();
        if (g_pGameData != nullptr)
        {
            (void)g_pGameData->unpackedCTemp.nCharacterSaveFlags;
        }
        g_tClientTiming.dwLastPingSentTick = dwNow;
        g_tClientTiming.dwNextPingSendTick = dwNow + 5000u;
    }
}

// 1.12a: 0x6FB2B200
void __stdcall GAME_RenderDisconnectErrorScreen(void)
{
    if (WINDOW_GetState() != 0)
    {
        return;
    }
    if (!D2GFX_StartDraw(1, 0, 0, 0))
    {
        return;
    }

    D2GFX_ClearScreen(0);
    D2Win_10127_SetFont(D2FONT_FONT24);

    int32_t nErrIdx = g_nDisconnectErrorIndex;
    const Unicode* pwszError = D2LANG_GetStringFromTblIndex(static_cast<short>(g_awDisconnectErrorStrIds[nErrIdx]));
    if (g_pGameData != nullptr && (g_pGameData->unpackedCTemp.nCharacterSaveFlags & 0x20u) != 0)
    {
        if (nErrIdx == 20)
        {
            pwszError = D2LANG_GetStringFromTblIndex(0x5522);
        }
        else if (nErrIdx == 21)
        {
            pwszError = D2LANG_GetStringFromTblIndex(0x5521);
        }
    }

    const int32_t nTextWidth = D2Win_10121_GetTextWidth(pwszError);
    if (nTextWidth < (gnViewportWidth - 40))
    {
        D2Win_10117_DrawText(pwszError, (gnViewportWidth - nTextWidth) / 2, 200, 0, FALSE);
        D2Win_10019();
        D2GFX_EndScene();
        return;
    }

    D2Win_10127_SetFont(D2FONT_FONT16);
    int32_t nLineCount = 0;
    D2SplittedTextStrc* pWrappedHead = D2Win_10199(pwszError, &nLineCount, 600);
    D2SplittedTextStrc* pNode        = pWrappedHead;
    for (int32_t nLine = 0; nLine < nLineCount && pNode != nullptr; ++nLine)
    {
        const int32_t nLineWidth = D2Win_10121_GetTextWidth(pNode->wszLine);
        const int32_t nDrawX     = (gnViewportWidth - nLineWidth) / 2;
        const int32_t nFontH     = D2Win_10125();
        const int32_t nDrawY     = 200 + (nFontH * nLine) * 2;
        D2Win_10117_DrawText(pNode->wszLine, nDrawX, nDrawY, 0, FALSE);
        pNode = pNode->pNextLine;
    }
    D2Win_10200(pWrappedHead);
    D2Win_10019();
    D2GFX_EndScene();
}

// 1.12a: 0x6FB2B330
int32_t __stdcall GAME_GetPlayerRoomClearBackgroundFlag(void)
{
    if (gpPlayerUnit == nullptr)
    {
        return 0;
    }
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(gpPlayerUnit);
    if (pRoom == nullptr)
    {
        return 0;
    }

    const int32_t nLevelId  = DUNGEON_GetLevelIdFromRoom(pRoom);
    D2LevelsTxt* pLevelsTxt = DATATBLS_GetLevelsTxtRecord(nLevelId);
    if (pLevelsTxt == nullptr)
    {
        FOG_DisplayHalt("", Fog_10265(0x5D7), 0x5D7);
        exit(-1);
    }

    return static_cast<int32_t>(pLevelsTxt->dwBlankScreen);
}

// 1.12a: 0x6FB2B380
void __stdcall GAME_FreeWeatherResourcesThunk(void)
{
    WEATHER_FreeEnvironmentCellFiles();
}

// 1.12a: 0x6FB2B390
void __stdcall GAME_StartTaintedSunEclipse(void)
{
    if (gpClientAct != nullptr)
    {
        D2ActiveRoomStrc* pPlayerRoom = (gpPlayerUnit != nullptr) ? UNITS_GetRoom(gpPlayerUnit) : nullptr;
        ENVIRONMENT_InitializeEnvironment(gpClientAct, pPlayerRoom, ENVCYCLE_NIGHT, 0, TRUE);
        return;
    }
    g_bDeferredTaintedSunInit = 1;
}

// 1.12a: 0x6FB2B3D0
void CALLBACK GAME_OnXButtonUp(SMSGHANDLER_PARAMS* pMsg)
{
    if (gbActLoadedFlag == 0)
    {
        return;
    }

    const uint16_t wButton = HIWORD(pMsg->wParam);
    if (wButton == XBUTTON1)
    {
        CMDTBL_DispatchMouseButton3Up();
    }
    else if (wButton == XBUTTON2)
    {
        CMDTBL_DispatchMouseButton4Up();
    }

    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2B420
void CALLBACK GAME_OnXButtonDown(SMSGHANDLER_PARAMS* pMsg)
{
    if (gbActLoadedFlag == 0)
    {
        return;
    }

    const uint16_t wButton = HIWORD(pMsg->wParam);
    if (wButton == XBUTTON1)
    {
        CMDTBL_DispatchMouseButton3Down();
    }
    else if (wButton == XBUTTON2)
    {
        CMDTBL_DispatchMouseButton4Down();
    }

    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2B470
void CALLBACK GAME_OnMButtonDown(SMSGHANDLER_PARAMS* pMsg)
{
    if (gbActLoadedFlag == 0)
    {
        return;
    }

    if (g_pfnMButtonDownCallback != nullptr)
    {
        if (IsBadCodePtr(reinterpret_cast<FARPROC>(g_pfnMButtonDownCallback)))
        {
            FOG_DisplayHalt("", Fog_10265(0x339), 0x339);
            exit(-1);
        }
        g_pfnMButtonDownCallback();
    }

    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2B4D0
void CALLBACK GAME_OnMButtonUp(SMSGHANDLER_PARAMS* pMsg)
{
    if (gbActLoadedFlag == 0)
    {
        return;
    }

    if (g_pfnMButtonUpCallback != nullptr)
    {
        if (IsBadCodePtr(reinterpret_cast<FARPROC>(g_pfnMButtonUpCallback)))
        {
            FOG_DisplayHalt("", Fog_10265(0x345), 0x345);
            exit(-1);
        }
        g_pfnMButtonUpCallback();
    }

    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2B530
void CALLBACK GAME_OnMouseWheel(SMSGHANDLER_PARAMS* pMsg)
{
    if (gbActLoadedFlag == 0)
    {
        return;
    }

    const int16_t nWheelDelta = static_cast<int16_t>(HIWORD(pMsg->wParam));
    const int32_t nAccum      = g_nMouseWheelDeltaAccum + static_cast<int32_t>(nWheelDelta);
    g_nMouseWheelDeltaAccum   = nAccum;

    if (abs(nAccum) >= WHEEL_DELTA)
    {
        CMDTBL_DispatchMouseWheel(nAccum / WHEEL_DELTA);
        g_nMouseWheelDeltaAccum = 0;
    }

    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2B5A0
void __stdcall GAME_RenderEndGameCongratulationText(void)
{
    const int32_t nCenterX = gnScreenWidth / 2;

    const int32_t nCongratsX = GAME_CalcCenteredStringX(0x13EA, nCenterX);
    const int32_t nCongratsY = (gbIsExpansion != 0) ? 100 : 62;
    D2Win_10117_DrawText(D2LANG_GetStringFromTblIndex(0x13EA), nCongratsX, nCongratsY, 4, FALSE);

    if (gbIsExpansion == 0)
    {
        const int32_t nSubLineX = GAME_CalcCenteredStringX(0x13EB, nCenterX);
        D2Win_10117_DrawText(D2LANG_GetStringFromTblIndex(0x13EB), nSubLineX, 107, 4, FALSE);
    }

    const int32_t nKnownAsY = (gbIsExpansion != 0) ? 272 : 232;
    const int32_t nKnownAsX = GAME_CalcCenteredStringX(0x13EC, nCenterX);
    D2Win_10117_DrawText(D2LANG_GetStringFromTblIndex(0x13EC), nKnownAsX, nKnownAsY, 4, FALSE);
}

// 1.12a: 0x6FB2B670
void CALLBACK GAME_OnEndGameMouseUp(SMSGHANDLER_PARAMS* pMsg)
{
    const int32_t nBtnLeft   = (gnScreenWidth / 2) - 48;
    const int32_t nBtnBottom = gnScreenHeight - 80;
    g_bEndGameOkButtonPressed = 0;

    if (gnMouseX >= nBtnLeft && gnMouseX <= (nBtnLeft + 96) &&
        gnMouseY >= (nBtnBottom - 32) && gnMouseY <= nBtnBottom)
    {
        g_bEndGameScreenDismissed = 1;
        pMsg->bUseResult = TRUE;
        pMsg->lResult    = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }

    g_bLeftMouseHeld       = 0;
    g_bRightMouseHeld      = 0;
    g_bLeftMouseRepeating  = 0;
    g_bRightMouseRepeating = 0;
    pMsg->bUseResult       = TRUE;
    pMsg->lResult          = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2B700
void CALLBACK GAME_OnEndGameMouseDown(SMSGHANDLER_PARAMS* pMsg)
{
    const int32_t nBtnLeft   = (gnScreenWidth / 2) - 48;
    const int32_t nBtnBottom = gnScreenHeight - 80;

    if (gnMouseX >= nBtnLeft && gnMouseX <= (nBtnLeft + 96) &&
        gnMouseY >= (nBtnBottom - 32) && gnMouseY <= nBtnBottom)
    {
        g_bEndGameOkButtonPressed = 1;
    }

    g_bLeftMouseHeld       = 0;
    g_bRightMouseHeld      = 0;
    g_bLeftMouseRepeating  = 0;
    g_bRightMouseRepeating = 0;
    pMsg->bUseResult       = TRUE;
    pMsg->lResult          = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2B780
BOOL CALLBACK GAME_BetaScreenLoop(DWORD dwCount)
{
    (void)dwCount;

    if (WINDOW_GetState() == 0)
    {
        if (!D2GFX_StartDraw(1, 0, 0, 0))
        {
            return FALSE;
        }

        const int32_t nCurIdx = static_cast<int32_t>(g_nCurrentBetaScreenIdx);
        if (nCurIdx < g_nBetaScreenCount)
        {
            if (g_aBetaScreens[nCurIdx].pCellFile == nullptr)
            {
                char szPath[260] = {};
                wsprintfA(szPath, "%s\\ui\\betascreens\\%s", "DATA\\GLOBAL", g_aBetaScreens[nCurIdx].szFileName);
                D2CellFileStrc* pLoaded = ARCHIVE_LoadCellFileDefault(szPath, 0);
                const uint8_t nReloadIdx = g_nCurrentBetaScreenIdx;
                g_aBetaScreens[nReloadIdx].pCellFile  = pLoaded;
                g_aBetaScreens[nReloadIdx].dwLoadTick = GetTickCount();
                D2Win_10177(g_aBetaScreens[nReloadIdx].nActPalette);
            }

            D2GfxDataStrc tGfx = {};
            tGfx.pCellFile  = g_aBetaScreens[g_nCurrentBetaScreenIdx].pCellFile;
            tGfx.nDirection = 0;

            static const int32_t kTileCoords[6][2] =
            {
                {   0, 256 },
                { 256, 256 },
                { 512, 256 },
                {   0, 480 },
                { 256, 480 },
                { 512, 480 },
            };
            for (uint32_t nFrame = 0; nFrame < 6u; ++nFrame)
            {
                tGfx.nFrame = nFrame;
                TEXTURE_CelDraw(&tGfx, kTileCoords[nFrame][0], kTileCoords[nFrame][1], 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
            }
        }

        D2GFX_EndScene();
    }

    uint8_t nIdx = g_nCurrentBetaScreenIdx;
    if (static_cast<int32_t>(nIdx) < g_nBetaScreenCount)
    {
        if (GetTickCount() <= (g_aBetaScreens[nIdx].dwLoadTick + 10000u))
        {
            return FALSE;
        }
        nIdx = g_nCurrentBetaScreenIdx;
    }

    nIdx += 1;
    g_nCurrentBetaScreenIdx = nIdx;
    if (static_cast<int32_t>(nIdx) < g_nBetaScreenCount)
    {
        return FALSE;
    }

    for (int32_t i = 0; i < g_nBetaScreenCount; ++i)
    {
        D2CellFileStrc* pCel = g_aBetaScreens[i].pCellFile;
        if (pCel != nullptr)
        {
            D2CMP_CelFileFreeHardware(pCel);
            FOG_Free(pCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
            g_aBetaScreens[i].pCellFile  = nullptr;
            g_aBetaScreens[i].dwLoadTick = 0;
        }
    }

    g_bExitAppFlag = 0;
    return TRUE;
}

// 1.12a: 0x6FB2B9B0
void __stdcall GAME_UpdateStandStillKeyState(void)
{
    constexpr int32_t kCmdStandStill = 0x24;

    if (CMDTBL_HasKeyBound(kCmdStandStill, 1))
    {
        for (int32_t i = 0; i < 114; ++i)
        {
            if (g_aKeyBindings[i].nCommandId == kCmdStandStill && g_aKeyBindings[i].bIsPrimary == 1)
            {
                const uint16_t wKey = g_aKeyBindings[i].wKey;
                if (wKey != 0xFFFFu && (GetAsyncKeyState(static_cast<int>(wKey)) & 0x8000) != 0)
                {
                    gbStandStillHeld = 1;
                    return;
                }
                break;
            }
        }
    }

    if (CMDTBL_HasKeyBound(kCmdStandStill, 0))
    {
        for (int32_t i = 0; i < 114; ++i)
        {
            if (g_aKeyBindings[i].nCommandId == kCmdStandStill && g_aKeyBindings[i].bIsPrimary == 0)
            {
                const uint16_t wKey = g_aKeyBindings[i].wKey;
                if (wKey != 0xFFFFu && (GetAsyncKeyState(static_cast<int>(wKey)) & 0x8000) != 0)
                {
                    gbStandStillHeld = 1;
                    return;
                }
                gbStandStillHeld = 0;
                return;
            }
        }
    }

    gbStandStillHeld = 0;
}

// 1.12a: 0x6FB2BA80 (register arg nErrorIndex in eax)
void __fastcall GAME_SetDisconnectErrorScreen(int32_t nErrorIndex)
{
    if (g_nDisconnectScreenFramesLeft != 0 ||
        g_bShowClassicEndGameScreen != 0 ||
        g_bShowExpansionEndGameScreen != 0)
    {
        return;
    }

    g_nDisconnectErrorIndex = (nErrorIndex < 29) ? nErrorIndex : 9;
    UI_CloseOpenPanels(1, 0);
    g_nDisconnectScreenFramesLeft = 600;
    g_bExitGameFlag               = 1;
    gbActLoadedFlag               = 0;
    g_bSystemConnected            = 0;
    g_bExitAppFlag                = 0;
}

// 1.12a: 0x6FB2BAF0
void __stdcall GAME_CheckNetworkConnectionHealth(void)
{
    if (gnGameType == 0 || gnGameType == 1)
    {
        return;
    }

    const uint32_t dwNow = GetTickCount();
    if (!D2NET_10025() && g_bExitGameFlag == 0 && gbActLoadedFlag != 0)
    {
        GAME_SetDisconnectErrorScreen(7);
    }

    if (gbActUnloadedFlag != 0 && (dwNow - g_dwActUnloadStartTick) > 180000u)
    {
        GAME_SetDisconnectErrorScreen(7);
    }

    if ((dwNow - g_dwConnectStartTick) > 30000u &&
        gbActLoadedFlag == 0 &&
        g_bExitGameFlag == 0 &&
        gbActUnloadedFlag == 0)
    {
        GAME_SetDisconnectErrorScreen(6);
    }

    if (g_bSystemConnected == 0 &&
        gbActLoadedFlag == 0 &&
        g_bExitGameFlag == 0 &&
        gbActUnloadedFlag == 0 &&
        (gnGameType == 9 || gnGameType == 7))
    {
        GAME_SetDisconnectErrorScreen(6);
    }
}

// 1.12a: 0x6FB2BBD0
void __fastcall GAME_ClearAllOpenPanels(void)
{
    if (gbActLoadedFlag == 0)
    {
        return;
    }
    if (UI_CloseOpenPanels(0, 1) != 0)
    {
        return;
    }

    if (gbAutomapOn != 0)
    {
        AUTOMAP_UpdateViewportBounds();
        if (gbAutomapCenters != 1)
        {
            gnAutomapOffsetX = 0;
            gnAutomapOffsetY = 0;
            UI_CloseOpenPanels(1, 0);
            return;
        }

        gnAutomapOffsetX = (gnScreenWidth / 3) - gnAutomapBaseX - 16;
        gnAutomapOffsetY = (gnScreenHeight / 3) - gnAutomapBaseY - 16;
    }

    UI_CloseOpenPanels(1, 0);
}

// 1.12a: 0x6FB2BC80
int32_t __stdcall GAME_LeaveGameCleanupState(void)
{
    ENV_FreeTileRenderPoolsCheck();
    CURSOR_ResetToDefault();
    RECORD_FreeAllocatedChunks();
    ROSTER_FreeAllRosterLists();
    ROSTERPETS_FreeAllPets();
    SMsgPopRegisterState(WINDOW_GetWindow());
    return D2CLIENT_STATE_EXIT;
}

// 1.12a: 0x6FB2BCB0
// Drains D2Net system packets (0xAF..0xB4 via jump table at 0x6FB2BD50) and game server packets.
void __stdcall GAME_ReceiveNetworkPackets(void)
{
    uint8_t aPacketBuffer[0x200];

    int32_t nSysPacketSize = 0;
    do
    {
        nSysPacketSize = CLIENT_DequeueSystemPacket(aPacketBuffer, sizeof(aPacketBuffer));
        if (nSysPacketSize != -1)
        {
            const uint8_t bSysOpcode = aPacketBuffer[0];
            switch (bSysOpcode)
            {
#ifdef D2_VERSION_110F
            case 0xAE: // 1.10f system connect
                g_bSystemConnected = 1;
                break;

            case 0xAF: // 1.10f system disconnect
                g_bSystemConnected = 0;
                if (g_bWindowClosedExit != 0)
                {
                    g_bExitGameFlag = 1;
                }
                break;
#else
            case 0xAF: // 0x6FB2BCEC: System connect / compression handshake
                g_bSystemConnected = 1;
                break;

            case 0xB0: // 0x6FB2BCF8: System disconnect
                g_bSystemConnected = 0;
                if (g_bWindowClosedExit != 0)
                {
                    g_bExitGameFlag = 1;
                }
                break;
#endif

            case 0xB1: // 0x6FB2BD18: No-op in client loop
            case 0xB2: // 0x6FB2BD18: No-op in client loop
                break;

            case 0xB3: // 0x6FB2BD04: Receive save chunk
                SCMD_HandlerB3_ReceiveSaveChunk(aPacketBuffer);
                break;

            case 0xB4: // 0x6FB2BD0F: Server connection refused / timeout reason
                SCMD_HandlerB4_ConnectionRefusedReason(aPacketBuffer);
                break;

            default:
                break;
            }
        }
        Sleep(0);
    } while (nSysPacketSize != -1);

    int32_t nGamePacketSize = 0;
    do
    {
        nGamePacketSize = CLIENT_DequeueGamePacket(aPacketBuffer, sizeof(aPacketBuffer));
        SCMD_ProcessPacketStream(aPacketBuffer, nGamePacketSize);
        Sleep(0);
    } while (nGamePacketSize != -1);
}

// 1.12a: 0x6FB2BD70
void CALLBACK GAME_OnCloseWindow(SMSGHANDLER_PARAMS* pMsg)
{
    gbRunLock = 0;
    if (g_bExitGameFlag != 0)
    {
        UI_ShutdownGameSession();
        gnWallCursorStateA = 0x13;
        gnWallCursorStateB = 0x13;
        pMsg->bUseResult   = FALSE;
        return;
    }

    MSG_SendLeaveGame_69();
    g_bWindowClosedExit = 1;
    pMsg->bUseResult    = TRUE;
    pMsg->lResult       = 0;
}

// ============================================================================
// SECTION 5: End-Game & Disconnect Loops, Act Loading, State Init/Free & Input
// (0x6FB2BDD0 .. 0x6FB2CE0F)
// ============================================================================

// 1.12a: 0x6FB2BDD0
BOOL CALLBACK GAME_EndGameScreenLoop(DWORD dwCount)
{
    (void)dwCount;

    if (WINDOW_GetState() == 0)
    {
        if (!D2GFX_StartDraw(1, 0, 0, 0))
        {
            return FALSE;
        }

        if (g_pEndGameBgCell == nullptr)
        {
            char szMenuName[16]  = {};
            char szMenuPath[260] = {};
            char szPalDat[260]   = {};
            char szPalPl2[260]   = {};

            strcpy(szMenuName, (gbIsExpansion != 0) ? "endgame2" : "endgame");
            sprintf(szMenuPath, "%s\\ui\\menu\\%s", "DATA\\GLOBAL", szMenuName);
            g_pEndGameBgCell = ARCHIVE_LoadCellFileDefault(szMenuPath, 0);

            sprintf(szPalDat, "%s\\Palette\\%s\\pal.dat", "DATA\\GLOBAL", szMenuName);
            sprintf(szPalPl2, "%s\\Palette\\%s\\pal.pl2", "DATA\\GLOBAL", szMenuName);
            D2Win_10028(szPalDat, szPalPl2);
        }

        if (g_pEndGameOkBtnCell == nullptr)
        {
            char szOkBtnPath[260] = {};
            sprintf(szOkBtnPath, "%s\\ui\\menu\\endgameok", "DATA\\GLOBAL");
            g_pEndGameOkBtnCell = ARCHIVE_LoadCellFileDefault(szOkBtnPath, 0);
        }

        D2Win_10179();

        D2GfxDataStrc tBgGfx = {};
        tBgGfx.pCellFile  = g_pEndGameBgCell;
        tBgGfx.nDirection = 0;

        if (gbIsExpansion == 0)
        {
            const int32_t nCenterX = gnScreenWidth / 2;
            const int32_t nBottomY = gnScreenHeight;
            const int32_t aClassicQuads[8][2] =
            {
                { 0,              256          },
                { 256,            256          },
                { 0,              nBottomY - 1 },
                { 256,            nBottomY - 1 },
                { nCenterX,       256          },
                { nCenterX + 256, 256          },
                { nCenterX,       nBottomY     },
                { nCenterX + 256, nBottomY     },
            };
            for (uint32_t i = 0; i < 8u; ++i)
            {
                tBgGfx.nFrame = i;
                TEXTURE_CelDraw(&tBgGfx, aClassicQuads[i][0], aClassicQuads[i][1], 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
            }
        }
        else
        {
            static const int32_t kExpQuads[12][2] =
            {
                {   0, 256 }, { 256, 256 }, { 512, 256 }, { 768, 256 },
                {   0, 512 }, { 256, 512 }, { 512, 512 }, { 768, 512 },
                {   0, 600 }, { 256, 600 }, { 512, 600 }, { 768, 600 },
            };
            for (uint32_t i = 0; i < 12u; ++i)
            {
                tBgGfx.nFrame = i;
                TEXTURE_CelDraw(&tBgGfx, kExpQuads[i][0], kExpQuads[i][1], 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
            }
        }

        D2Win_10127_SetFont(D2FONT_FONT24);
        GAME_RenderEndGameCongratulationText();
        GAME_RenderEndGameCharacterTitle();

        D2Win_10127_SetFont(D2FONT_FONT16);
        D2GfxDataStrc tBtnGfx = {};
        tBtnGfx.pCellFile  = g_pEndGameOkBtnCell;
        tBtnGfx.nDirection = 0;
        tBtnGfx.nFrame     = (g_bEndGameOkButtonPressed != 0) ? 1u : 0u;
        TEXTURE_CelDraw(&tBtnGfx, (gnScreenWidth / 2) - 48, gnScreenHeight - 80, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);

        const int32_t nOkX      = GAME_CalcCenteredStringX(0x13EE, gnScreenWidth / 2);
        const int32_t nShiftPx  = (g_bEndGameOkButtonPressed != 0) ? 2 : 0;
        D2Win_10117_DrawText(D2LANG_GetStringFromTblIndex(0x13EE), nOkX - nShiftPx, (gnScreenHeight - 88) + nShiftPx, 0, FALSE);

        CURSOR_RenderCursor();
        D2GFX_EndScene();
    }

    if (g_bEndGameScreenDismissed == 0)
    {
        return FALSE;
    }

    if (g_pEndGameBgCell != nullptr)
    {
        D2CMP_CelFileFreeHardware(g_pEndGameBgCell);
        FOG_Free(g_pEndGameBgCell, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
        g_pEndGameBgCell = nullptr;
    }
    if (g_pEndGameOkBtnCell != nullptr)
    {
        D2CMP_CelFileFreeHardware(g_pEndGameOkBtnCell);
        FOG_Free(g_pEndGameOkBtnCell, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
        g_pEndGameOkBtnCell = nullptr;
    }

    g_bExitAppFlag = 0;
    return TRUE;
}

// 1.12a: 0x6FB2C2F0
BOOL CALLBACK GAME_DisconnectScreenLoop(DWORD dwCount)
{
    (void)dwCount;

    if (g_nDisconnectScreenFramesLeft != 0)
    {
        GAME_RenderDisconnectErrorScreen();
        g_nDisconnectScreenFramesLeft -= 1;
        if (g_nDisconnectScreenFramesLeft == 0)
        {
            UI_FreeLoadingScreen();
            return TRUE;
        }
    }
    return FALSE;
}

extern "C" D2ActiveRoomStrc* __fastcall D2Client_SafeStreamRoomAtTile(int32_t nTileX, int32_t nTileY, int32_t nExplicitLevelId);
static void GAME_PrestreamTownAndAdjacentWilderness(int32_t nTownLevelId);

// 1.12a: 0x6FB2C320
void __fastcall GAME_LoadAct(uint8_t nAct, uint32_t dwMapSeed, uint16_t wLevelId, uint32_t dwFlags)
{
    GAME_PreloadActTileLibrary(nAct);

    if (gpClientAct != nullptr)
    {
        for (int32_t t = 0; t < UNIT_TYPES_COUNT; ++t)
        {
            for (int32_t b = 0; b < 128; ++b)
            {
                for (D2UnitStrc* pU = gapServerSideUnitHashTables[t][b]; pU != nullptr; pU = pU->pListNext)
                {
                    UNITROOM_RemoveUnitFromRoom(pU);
                }
                for (D2UnitStrc* pU = gapClientSideUnitHashTables[t][b]; pU != nullptr; pU = pU->pListNext)
                {
                    UNITROOM_RemoveUnitFromRoom(pU);
                }
            }
        }
        DUNGEON_FreeAct(gpClientAct);
    }

    gpClientAct = DUNGEON_AllocAct(
        nAct,
        dwMapSeed,
        TRUE,
        nullptr,
        gnClientDifficulty,
        nullptr,
        static_cast<int>(wLevelId),
        AUTOMAP_RevealRoomFull,
        AUTOMAP_InitTownLayer);

    if (g_bDeferredTaintedSunInit != 0 && nAct == ACT_II && gpClientAct != nullptr)
    {
        D2ActiveRoomStrc* pPlayerRoom = (gpPlayerUnit != nullptr) ? UNITS_GetRoom(gpPlayerUnit) : nullptr;
        ENVIRONMENT_InitializeEnvironment(gpClientAct, pPlayerRoom, ENVCYCLE_NIGHT, 0, TRUE);
    }

    g_bDeferredTaintedSunInit = 0;
    g_nCurrentActIndex        = static_cast<int32_t>(nAct);
    UI_StepAndDrawLoadingScreen(1);

    if (g_pActSeeds != nullptr)
    {
        g_pActSeeds->dwInitSeed = dwMapSeed;
        g_pActSeeds->dwEndSeed  = dwFlags;
    }

    AUTOMAP_InitColors();
    LIGHTMAP_InitializeReciprocalTableAndActCallback();
    UI_InitPaletteColors();
    extern void __fastcall ENV_InitActWeatherPaletteAndOverlays(int32_t nLevelId);
    ENV_InitActWeatherPaletteAndOverlays(static_cast<int32_t>(wLevelId));

    g_dwCurrentTownLevelId = static_cast<uint32_t>(wLevelId);
    GFX_FreeMonsterCofCache();

    char szCofPath[260] = {};
    wsprintfA(szCofPath, "%s\\%s", "DATA\\GLOBAL", g_aszActMonsterCofFiles[nAct]);
    GFX_LoadPackedCofArchive(&g_MonsterCofCache, szCofPath);

    GAME_PrestreamTownAndAdjacentWilderness(static_cast<int32_t>(wLevelId));

    g_nPostActLoadBlackFrames = 1;
    const uint32_t dwGraceTarget = GetTickCount() + 10000u;
    if (dwGraceTarget > gdwReassignGraceTick)
    {
        gdwReassignGraceTick = dwGraceTarget;
        g_tClientTiming.dwReassignGraceTick = dwGraceTarget;
    }
}

// 1.12a: 0x6FB2C450
void __stdcall GAME_InitializeNewActRooms(void)
{
    for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct);
         pRoom != nullptr;
         pRoom = pRoom->pRoomNext)
    {
        if ((pRoom->dwFlags & 1u) == 0)
        {
            OBJ_SpawnRoomEnvSoundObjects(pRoom);
            D2Client_SpawnPresetClientUnitsInRoom(pRoom);
            if (DUNGEON_GetLevelIdFromRoom(pRoom) == 8 && g_bDenOfEvilCleared != 0)
            {
                QUEST_SpawnDenOfEvilLightBeamInRoom(pRoom);
            }
            pRoom->dwFlags |= 1u;
        }
    }
}

// 1.12a: 0x6FB2C4A0
void __stdcall GAME_FreeClientState(void)
{
    WEATHER_FreeEnvironmentCellFiles();
    PALSHIFT_FreeTables();
    D2Client_FreeAllUnits();

    if (gpClientAct != nullptr)
    {
        DUNGEON_FreeAct(gpClientAct);
    }
    gpClientAct = nullptr;

    D2CMP_SpriteFreeAsyncLoads();
    D2CMP_TileFreeAsyncLoads();
    D2CMP_DestroyTileCache(FALSE);
    D2CMP_FlushSpriteCache(FALSE);

    GFX_FreePlayerCofCache();
    GFX_FreeMonsterCofCache();

    if (g_pGfxScratchBuffer != nullptr)
    {
        FOG_FreePool(nullptr, g_pGfxScratchBuffer, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 0x7F9, 0);
        g_pGfxScratchBuffer = nullptr;
    }

    if (g_pClientViewport != nullptr)
    {
        if (g_pClientViewport->tSortGrid.pCells != nullptr)
        {
            FOG_Free(g_pClientViewport->tSortGrid.pCells, "..\\Source\\D2Client\\GAME\\Wall2.cpp", 0x55, 0);
        }
        g_pClientViewport->tSortGrid.pCells         = nullptr;
        g_pClientViewport->tSortGrid.nGridDimension = 0;
        g_pClientViewport->tSortGrid.nNodeCount     = 0;
        g_pClientViewport->tSortGrid.dwGridFlags    = 0;
        FOG_Free(g_pClientViewport, "..\\Source\\D2Client\\GAME\\View.cpp", 0x141, 0);
        g_pClientViewport = nullptr;
    }

    UI_ReleaseAllAssets();
    LIGHTMAP_FreeAllLightNodes();

    FOG_10061(g_hEnvParticlesPool);
    FOG_10061(g_hEnvSplashesPool);
    FOG_10061(g_hEnvBubblesPool);

    FOG_DestroyMemoryPoolSystem(g_tClientTiming.pClientPoolSystem);
    memset(&g_tClientTiming, 0, sizeof(g_tClientTiming));
    gdwClientGameTick    = 0;
    gdwClientPing        = 0;
    gdwReassignGraceTick = 0;
    gbIsExpansion        = 0;
    gbIsLadderClient     = 0;

    FOG_Free(g_pActSeeds, "..\\Source\\D2Client\\GAME\\Game.cpp", 0x592, 0);
    g_pActSeeds = nullptr;
}

// 1.12a: 0x6FB2C5C0
void CALLBACK GAME_OnRButtonUp(SMSGHANDLER_PARAMS* pMsg)
{
    g_nRightMouseButtonState = 0x10;
    g_bRightMouseHeld        = 0;
    g_bRightMouseRepeating   = 1;
    g_bSuppressLeftMouseHold = 0;
    g_bSkillTreeHold         = 0;

    if (gbActLoadedFlag == 0)
    {
        return;
    }

    if (gpPlayerUnit != nullptr && gpPlayerUnit->dwUnitType == UNIT_PLAYER)
    {
        const int32_t  nX      = static_cast<int32_t>(LOWORD(pMsg->lParam));
        const int32_t  nY      = static_cast<int32_t>(HIWORD(pMsg->lParam));
        const uint32_t dwFlags = static_cast<uint32_t>(LOWORD(pMsg->wParam));
        g_bRightMouseHeld = D2Client_DispatchMouseClickToPlayerAction(5, nX, nY, dwFlags);
    }

    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2C640
void __stdcall GAME_ProcessRightMouseHold(void)
{
    g_nRightMouseButtonState = 0x08;
    if (gbActLoadedFlag == 0 || g_bRightMouseHeld == 0)
    {
        return;
    }

    g_bRightMouseRepeating = 1;
    const uint32_t dwKeyFlags = GAME_BuildMovementKeyFlags();
    if (gpPlayerUnit != nullptr && gpPlayerUnit->dwUnitType == UNIT_PLAYER)
    {
        D2Client_DispatchMouseClickToPlayerAction(4, gnMouseX, gnMouseY, dwKeyFlags);
    }
}

// 1.12a: 0x6FB2C6C0
void CALLBACK GAME_OnRButtonDown(SMSGHANDLER_PARAMS* pMsg)
{
    g_nRightMouseButtonState = 0x04;
    g_bSuppressLeftMouseHold = 0;
    g_bSkillTreeHold         = 0;

    if (gbActLoadedFlag == 0 || gnOpenPanelMode == 3)
    {
        return;
    }

    const int32_t nX = static_cast<int32_t>(LOWORD(pMsg->lParam));
    const int32_t nY = static_cast<int32_t>(HIWORD(pMsg->lParam));

    if (GAME_IsPointInsideLeftOpenPanel(nX, nY) || GAME_IsPointInsideRightOpenPanel(nX, nY))
    {
        pMsg->bUseResult = FALSE;
        pMsg->lResult    = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }

    g_bRightMouseHeld = 1;
    const uint32_t dwKeyFlags = GAME_BuildMovementKeyFlags();
    if (gpPlayerUnit != nullptr && gpPlayerUnit->dwUnitType == UNIT_PLAYER)
    {
        g_bRightMouseHeld = D2Client_DispatchMouseClickToPlayerAction(3, nX, nY, dwKeyFlags);
    }

    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2C790
void __stdcall GAME_HandleLButtonUp(void)
{
    g_nLeftMouseButtonState  = 0x10;
    g_bLeftMouseHeld         = 0;
    g_bLeftMouseRepeating    = 0;
    g_bSuppressLeftMouseHold = 0;
    g_bSkillTreeHold         = 0;

    if (gbActLoadedFlag == 0)
    {
        return;
    }

    if (gpPlayerUnit != nullptr && gpPlayerUnit->dwUnitType == UNIT_PLAYER)
    {
        const uint32_t dwKeyFlags = GAME_BuildMovementKeyFlags();
        D2Client_DispatchMouseClickToPlayerAction(2, gnMouseX, gnMouseY, dwKeyFlags);
    }
}

// 1.12a: 0x6FB2C810
void __stdcall GAME_ProcessLeftMouseHold(void)
{
    g_nLeftMouseButtonState = 0x08;
    if (gbActLoadedFlag == 0 || g_bLeftMouseHeld == 0 || g_bSuppressLeftMouseHold != 0)
    {
        return;
    }

    g_bLeftMouseRepeating = 1;
    const uint32_t dwKeyFlags = GAME_BuildMovementKeyFlags();
    if (gpPlayerUnit != nullptr && gpPlayerUnit->dwUnitType == UNIT_PLAYER)
    {
        D2Client_DispatchMouseClickToPlayerAction(1, gnMouseX, gnMouseY, dwKeyFlags);
    }
}

// 1.12a: 0x6FB2C870
void CALLBACK GAME_OnLButtonDown(SMSGHANDLER_PARAMS* pMsg)
{
    g_nLeftMouseButtonState  = 0x04;
    g_bSuppressLeftMouseHold = 0;
    g_bSkillTreeHold         = 0;

    if (gbActLoadedFlag == 0)
    {
        return;
    }

    g_bLeftMouseHeld = 1;
    const uint32_t dwKeyFlags = GAME_BuildMovementKeyFlags();
    if (gpPlayerUnit != nullptr && gpPlayerUnit->dwUnitType == UNIT_PLAYER)
    {
        const int32_t nX = static_cast<int32_t>(LOWORD(pMsg->lParam));
        const int32_t nY = static_cast<int32_t>(HIWORD(pMsg->lParam));
        g_bLeftMouseHeld = D2Client_DispatchMouseClickToPlayerAction(0, nX, nY, dwKeyFlags);
    }

    pMsg->bUseResult = TRUE;
    pMsg->lResult    = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB2C910
void CALLBACK GAME_OnActivateApp(SMSGHANDLER_PARAMS* pMsg)
{
    const int32_t bActive = static_cast<int32_t>(pMsg->wParam);
    gbAppActiveState = bActive;

    if (bActive != 0)
    {
        FOG_IsExpansion();
        if (gnRegisteredPanelCount != 0x26)
        {
            FOG_DisplayHalt("", Fog_10265(0x1389), 0x1389);
            exit(-1);
        }

        g_bAltShowItemsHeld = 0;
        gbStandStillHeld    = 0;
        g_bSkillTreeHold    = 0;
        g_bSkillSelectHold  = 0;
        D2Win_10176(0);
        pMsg->bUseResult = FALSE;
        return;
    }

    constexpr int32_t kCmdRunWalk = 0x22;
    if (CMDTBL_HasKeyBound(kCmdRunWalk, 1))
    {
        const uint16_t wRunKey = CMDTBL_GetBoundKey(kCmdRunWalk, 1);
        if ((GetAsyncKeyState(static_cast<int>(wRunKey)) & 0x8000) == 0)
        {
            gbRunLock = 0;
        }
    }

    if (g_nLeftMouseButtonState != 0x10)
    {
        GAME_HandleLButtonUp();
    }
    if (g_nRightMouseButtonState != 0x10)
    {
        g_nRightMouseButtonState = 0x10;
        g_bRightMouseHeld        = 0;
        g_bRightMouseRepeating   = 1;
        g_bSuppressLeftMouseHold = 0;
        g_bSkillTreeHold         = 0;
    }

    pMsg->bUseResult = FALSE;
}

// 1.12a: 0x6FB2CA00
void CALLBACK GAME_OnLButtonUpMsg(SMSGHANDLER_PARAMS* pMsg)
{
    (void)pMsg;
    GAME_HandleLButtonUp();
}

// 1.12a: 0x6FB2CA10
void __fastcall GAME_RenderFrame(int32_t bSkipCursor)
{
    UI_FreeLoadingScreen();
    if (WINDOW_GetState() != 0)
    {
        return;
    }

    D2ViewportStrc* pView = g_pClientViewport;
    const int32_t nHalfW  = (pView->nScreenRight - pView->nScreenLeft) / 2;
    const int32_t nHalfH  = (pView->nScreenBottom - pView->nScreenTop) / 2;

    int32_t nCameraX = UNITS_GetClientCoordX(gpPlayerUnit) + gnViewportShiftOffsetX - nHalfW;
    int32_t nCameraY = UNITS_GetClientCoordY(gpPlayerUnit) - nHalfH;
    VIEW_UpdateScreenShake(&nCameraX, &nCameraY);

    if (g_nPrevCameraX != -1 || g_nPrevCameraY != -1)
    {
        g_nCameraDeltaX = nCameraX - g_nPrevCameraX;
        g_nCameraDeltaY = nCameraY - g_nPrevCameraY;
    }
    g_nPrevCameraX = nCameraX;
    g_nPrevCameraY = nCameraY;

    pView = g_pClientViewport;
    if (pView->nViewOffsetX != nCameraX || pView->nViewOffsetY != nCameraY)
    {
        pView->nViewOffsetX = nCameraX;
        pView->nViewOffsetY = nCameraY;
        pView->dwFlags     |= VIEWFLAG_DIRTY;
    }

    VIEW_UpdateViewportVisibility(pView, gpPlayerUnit);
    WEATHER_UpdateParticles();

    uint8_t nAmbRed   = 0;
    uint8_t nAmbGreen = 0;
    uint8_t nAmbBlue  = 0;
    ENVIRONMENT_GetLightColorFromAct(gpClientAct, &nAmbRed, &nAmbGreen, &nAmbBlue);
    D2GFX_SetAmbientColor(nAmbBlue, nAmbGreen, nAmbRed);

    const int32_t bBlankScreen = GAME_GetPlayerRoomClearBackgroundFlag();
    if (!D2GFX_StartDraw(bBlankScreen, 0, 0, 0))
    {
        return;
    }

    D2Win_10179();
    D2Client_UpdateViewportOffsetFromPlayer();

    if (gnOpenPanelMode != 3)
    {
        VIEW_RenderViewport(g_pClientViewport);
    }

    UI_RenderHUD(g_pClientViewport);
    D2Win_10019();

    if (bSkipCursor == 0)
    {
        CURSOR_RenderCursor();
    }

    D2Client_RenderFpsAndMemoryOverlay();

    if (g_nPostActLoadBlackFrames != 0)
    {
        D2GFX_ClearScreen(0);
        g_nPostActLoadBlackFrames -= 1;
        if (g_nPostActLoadBlackFrames < 0)
        {
            g_nPostActLoadBlackFrames = 0;
        }
    }

    D2GFX_EndScene();
}

// 1.12a: 0x6FB2CBB0
void __stdcall GAME_InitClientState(void)
{
    g_nDisconnectScreenFramesLeft = 0;
    g_bShowExpansionEndGameScreen = 0;
    g_bPlayExpansionEpilogueMovie = 0;
    g_bEndGameScreenDismissed     = 0;
    gbActUnloadedFlag             = 0;
    gbStandStillHeld              = 0;

    memset(&g_tClientTiming, 0, sizeof(g_tClientTiming));
    gdwClientGameTick    = 0;
    gdwClientPing        = 0;
    gdwReassignGraceTick = 0;

    FOG_CreateNewPoolSystem(&g_tClientTiming.pClientPoolSystem, "Client Pool System", 0x40000u, 0x1000u);

    if (gpPlayerUnit != nullptr)
    {
        UI_ShutdownGameSession();
    }
    gpPlayerUnit = nullptr;

    GAME_LoadColorTxtFlag();

    if (g_pClientViewport == nullptr)
    {
        g_pClientViewport = VIEW_AllocViewport();
    }

    DWORD dwSavedResolution = FOG_IsExpansion() ? 1u : 0u;
    if (!SRegLoadValue("Diablo II", "Resolution", 0, &dwSavedResolution))
    {
        dwSavedResolution = FOG_IsExpansion() ? 1u : 0u;
    }
    const int32_t nResMode = (dwSavedResolution != 0u) ? D2GAMERES_800x600 : D2GAMERES_640x480;
    GAME_SetResolutionMode(nResMode);

    extern void __cdecl GFXUTIL_InitDirectionTables(void);
    extern void __stdcall  QUESTLOG_InitSession(void);
    extern void __fastcall D2Client_ClearScreenMessages(void);
    extern void __fastcall D2Client_ClearMercForHireList(void);
    extern void __fastcall D2Client_ClearPortalInfoTable(void);
    extern D2BitBufferStrc* g_pPlayerQuestRecord;
    extern D2BitBufferStrc* g_pGameQuestRecord;
    GFXUTIL_InitDirectionTables();
    QUESTLOG_InitSession();
    D2Client_ClearScreenMessages();
    D2Client_ClearMercForHireList();
    D2Client_ClearPortalInfoTable();
    if (!g_pPlayerQuestRecord)
    {
        g_pPlayerQuestRecord = QUESTRECORD_AllocRecord(nullptr);
    }
    if (!g_pGameQuestRecord)
    {
        g_pGameQuestRecord = QUESTRECORD_AllocRecord(nullptr);
    }

    UI_LoadLoadingScreen();
    UI_StepAndDrawLoadingScreen(1);

    if (gnGameType == 7 || gnGameType == 9 || gnGameType == 6 || gnGameType == 8)
    {
        MSG_UploadSaveFile_6C(1);
    }

    g_pfWindowExitCallback = GAME_DestroyWindowAndGfxCallback;
    PALSHIFT_InitTables();
    D2CMP_PalShiftInit();
    D2Client_InitTileAndSpriteCaches_Adapter(g_tClientTiming.pClientPoolSystem, TRUE);

    g_bExitGameFlag          = (g_bQuickExitRequested != 0) ? 1 : 0;
    g_dwActUnloadStartTick   = GetTickCount();
    g_dwConnectStartTick     = GetTickCount();

    g_tClientTiming.pfnRenderFrame       = GAME_RenderFrame;
    g_tClientTiming.dwClientGameTick     = 0;
    g_tClientTiming.dwRenderedFrameCount = 0;

    if (g_pClientViewport->nViewOffsetX != 0 || g_pClientViewport->nViewOffsetY != 0)
    {
        g_pClientViewport->nViewOffsetX = 0;
        g_pClientViewport->nViewOffsetY = 0;
        g_pClientViewport->dwFlags     |= VIEWFLAG_DIRTY;
    }

    gnViewportShiftOffsetX = 0;
    gnOpenPanelMode        = 0;
    D2GFX_SetViewMatrixAdjust(0);
    VIEW_SetViewportRect(g_pClientViewport, 0, 0, gnViewportWidth, gnViewportHeight);

    memset(g_aEnvStateResetBlock, 0, sizeof(g_aEnvStateResetBlock));
    g_nDenOfEvilSunTimer = -1;
    g_nActQuestEnvTimer  = -1;
    memset(g_aQuestEnvTimers, 0, sizeof(g_aQuestEnvTimers));

    g_tClientTiming.nAutoSkillState = 0;
    g_bDenOfEvilCleared  = 0;
    g_bBloodRavenCleared = 0;
    g_nQuestStateFlag420 = 0;
    g_nQuestStateFlag424 = 0;

    g_pActSeeds = static_cast<D2ActSeedPairStrc*>(
        FOG_Alloc(sizeof(D2ActSeedPairStrc), "..\\Source\\D2Client\\GAME\\Game.cpp", 0x53F, 0));

    memset(gapServerSideUnitHashTables, 0, sizeof(gapServerSideUnitHashTables));
    memset(gapClientSideUnitHashTables, 0, sizeof(gapClientSideUnitHashTables));

    LIGHTMAP_ResetAmbientGrid(TRUE);
    WEATHER_InitEnvironmentPools();

    g_nEscMenuSelection    = 0;
    g_wHoveredWarpId       = 0;
    g_nHoveredWarpState    = 0;
    g_bWindowClosedExit    = 0;
    gbActLoadedFlag        = 0;
    gnCursorDefaultStateA  = 7;
    gnCursorDefaultStateB  = 7;
}

// ============================================================================
// SECTION 6: 25 Hz World Progress Tick, Per-Frame Loop & Session MainLoop
// (0x6FB2CE10 .. 0x6FB2D85F)
// ============================================================================

// 1.12a: 0x6FB2CE10
void __stdcall GAME_UpdateProgress(void)
{
    g_tClientTiming.dwCurrentTickMs = GetTickCount();

    GAME_InitializeNewActRooms();
    D2Client_UpdateAllUnitsPerTick();

    D2ActiveRoomStrc* pPlayerRoom = (gpPlayerUnit != nullptr) ? UNITS_GetRoom(gpPlayerUnit) : nullptr;
    DUNGEON_AnimateTiles(pPlayerRoom);
    QUEST_UpdateEnvironmentEffects();

    if (g_bForceSpritePrecache != 0 || (GetTickCount() >= (g_dwLastSpritePrecacheTick + 600u)))
    {
        GFX_PrecacheNearbyMonsterSprites(1);
        g_dwLastSpritePrecacheTick = GetTickCount();
    }

    pPlayerRoom = (gpPlayerUnit != nullptr) ? UNITS_GetRoom(gpPlayerUnit) : nullptr;
    if (ENVIRONMENT_UpdatePeriodOfDay(gpClientAct, pPlayerRoom))
    {
        D2Client_UpdateObjectsOnPeriodOfDayChange();
    }

    DRLGACTIVATE_Update(DUNGEON_GetDrlgFromAct(gpClientAct));
    AUTOMAP_UpdateReveal();

    const uint32_t dwNextTick = gdwClientGameTick + 1u;
    gdwClientGameTick               = dwNextTick;
    g_tClientTiming.dwClientGameTick = dwNextTick;

    if ((dwNextTick % 13u) == 0u)
    {
        DUNGEON_UpdateAndFreeInactiveRooms(gpClientAct);
    }
}

// 1.12a: 0x6FB2CEE0
BOOL CALLBACK GAME_FrameLoop(DWORD dwCount)
{
    (void)dwCount;

    D2UnitStrc* pSavedPlayer = gpPlayerUnit;
    BOOL bTickElapsed        = FALSE;
    BOOL bUpdatedProgress    = FALSE;

    // Single-player / local pause handling while minimized or in Esc menu
    if (gnGameType == 0 || gnGameType == 1)
    {
        const HWND hGameWnd = WINDOW_GetWindow();
        const bool bMinimizedOrPaused = WINDOW_IsFullScreen()
            ? (WINDOW_GetState() != 0)
            : (hGameWnd != nullptr && IsIconic(hGameWnd) != 0);
        if (bMinimizedOrPaused && g_bExitGameFlag == 0)
        {
            g_tClientTiming.dwLastLogicTickMs = GetTickCount();
            return FALSE;
        }

        if (g_bEscMenuOpen != 0 &&
            g_bExitGameFlag == 0 &&
            pSavedPlayer != nullptr &&
            gpPlayerUnit != nullptr &&
            UNITS_GetRoom(gpPlayerUnit) != nullptr)
        {
            g_tClientTiming.dwLastLogicTickMs = GetTickCount();
            if (g_tClientTiming.pfnRenderFrame != nullptr)
            {
                g_tClientTiming.pfnRenderFrame(0);
            }
            SOUNDHDR_UpdateFrame();
            return FALSE;
        }
    }

    // 1. Input playback, held mouse buttons, Stand Still key, and 5s keepalive ping
    RECORD_UpdatePlaybackTick();
    if (g_bLeftMouseHeld != 0)
    {
        GAME_ProcessLeftMouseHold();
    }
    if (g_bRightMouseHeld != 0)
    {
        GAME_ProcessRightMouseHold();
    }
    GAME_UpdateStandStillKeyState();

    const uint32_t dwPingCheckTick = GetTickCount();
    if (g_tClientTiming.dwNextPingSendTick < dwPingCheckTick)
    {
        MSG_SendPing_6D();
        if (g_pGameData != nullptr)
        {
            (void)g_pGameData->unpackedCTemp.nCharacterSaveFlags;
        }
        g_tClientTiming.dwLastPingSentTick = dwPingCheckTick;
        g_tClientTiming.dwNextPingSendTick = dwPingCheckTick + 5000u;
    }

    // 2. 25 Hz (40 ms) frame tick delta & grace period clamping
    const uint32_t dwNow = GetTickCount();
    const int32_t  nTargetTickMs = g_nTargetFrameTickMs;
    g_tClientTiming.dwCurrentTickMs = dwNow;

    if (g_tClientTiming.dwLastLogicTickMs == 0u)
    {
        g_tClientTiming.dwLastLogicTickMs = dwNow - static_cast<uint32_t>(nTargetTickMs);
    }

    // [Out-of-scope: 0x6FB2CFE5..0x6FB2CFFF Warden client tick & module instantiation omitted]

    GAME_UpdateFpsCounters();

    uint32_t dwElapsedMs = g_tClientTiming.dwCurrentTickMs - g_tClientTiming.dwLastLogicTickMs;
    if (dwElapsedMs >= static_cast<uint32_t>(nTargetTickMs))
    {
        g_tClientTiming.dwLastLogicTickMs = g_tClientTiming.dwCurrentTickMs;
        bTickElapsed = TRUE;

        if (g_tClientTiming.dwCurrentTickMs < gdwReassignGraceTick)
        {
            int32_t nRemainder = static_cast<int32_t>(dwElapsedMs) - nTargetTickMs;
            if (nRemainder >= (nTargetTickMs - 1))
            {
                nRemainder = nTargetTickMs - 1;
            }
            g_tClientTiming.dwLastLogicTickMs    = g_tClientTiming.dwCurrentTickMs - static_cast<uint32_t>(nRemainder);
            g_nConsecutiveSkippedFrames          = 0;
            dwElapsedMs                          = static_cast<uint32_t>(nTargetTickMs);
            g_tClientTiming.dwFpsSampleStartTick = g_tClientTiming.dwCurrentTickMs;
            g_tClientTiming.nMeasuredFps         = 25;
            g_tClientTiming.nSkippedFps          = 0;
            g_tClientTiming.nFpsSampleFrames     = 0;
            g_tClientTiming.nSkippedSampleFrames = 0;
        }

        if (gbActLoadedFlag != 0)
        {
            D2Client_ProcessQueuedPlayerAction(pSavedPlayer);
        }
    }

    // 3. Step local D2Game server simulation for Single Player (0) and Local Loopback (1)
    int32_t bServerStepped = 0;
    if (gnGameType == 0)
    {
        GAME_ProcessNetworkMessages();
        bServerStepped = GAME_UpdateGamesProgress(1);
        if (bServerStepped != 0)
        {
            GAME_UpdateClients(1, 0);
        }
    }
    else if (gnGameType == 1)
    {
        GAME_ProcessNetworkMessages();
        bServerStepped = GAME_UpdateGamesProgress(1);
        GAME_UpdateClients(0, 0);
    }

    // 4. Drain network packets, realm chat, MCP messages, and verify connection health
    GAME_ReceiveNetworkPackets();
    TEXT_PollBnetRealmChat();
    D2MCPClient_10003();
    GAME_CheckNetworkConnectionHealth();

    // 5. Advance client world simulation & render frame
    if (gbActLoadedFlag != 0)
    {
        if (gnGameType != 0 && gnGameType != 1)
        {
            if (bTickElapsed)
            {
                GAME_UpdateProgress();
                g_tClientTiming.dwLastLogicTickMs += static_cast<uint32_t>(nTargetTickMs) - dwElapsedMs;
                bUpdatedProgress = TRUE;

                const int32_t nMissedTicks = static_cast<int32_t>(dwElapsedMs / static_cast<uint32_t>(nTargetTickMs));
                int32_t nMaxSkip = GAME_CalcMaxFrameSkip(nMissedTicks, g_tClientTiming.nMeasuredFps);
                if (g_tClientTiming.nMeasuredFps > 28)
                {
                    nMaxSkip = 0;
                }
                if (g_nConsecutiveSkippedFrames > nMaxSkip)
                {
                    g_nConsecutiveSkippedFrames = 0;
                }
            }
            else if (g_tClientTiming.nMeasuredFps > 22)
            {
                g_nConsecutiveSkippedFrames = 0;
            }
        }
        else if (bServerStepped != 0)
        {
            GAME_UpdateProgress();
            bUpdatedProgress = TRUE;

            if (dwElapsedMs <= static_cast<uint32_t>(nTargetTickMs))
            {
                g_nConsecutiveSkippedFrames = 0;
            }
            else if (dwElapsedMs < static_cast<uint32_t>(nTargetTickMs * 2))
            {
                g_nConsecutiveSkippedFrames       = 0;
                g_tClientTiming.dwLastLogicTickMs += static_cast<uint32_t>(nTargetTickMs) - dwElapsedMs;
            }
            else
            {
                const int32_t bAllowSkip          = (g_nConsecutiveSkippedFrames <= 1) ? 1 : 0;
                g_tClientTiming.dwLastLogicTickMs += static_cast<uint32_t>(1 - nTargetTickMs);
                g_nConsecutiveSkippedFrames       = bAllowSkip;
            }
        }

        if (gbActLoadedFlag != 0)
        {
            g_bActionDispatchedThisFrame = 0;
            SELECT_UpdateHoveredUnit();

            if (bUpdatedProgress)
            {
                extern void __fastcall D2Client_RunNativeAutotestTick(void);
                D2Client_RunNativeAutotestTick();
            }

            if (gpPlayerUnit != nullptr && UNITS_GetRoom(gpPlayerUnit) != nullptr)
            {
                extern void __fastcall D2Client_UpdateAreaMusic(int32_t nActNo, int32_t nLevelId);
                const int32_t nCurLvl = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(gpPlayerUnit));
                D2Client_UpdateAreaMusic(g_nCurrentActIndex, nCurLvl);
            }

            if (g_nConsecutiveSkippedFrames == 0)
            {
                if (gpPlayerUnit != nullptr &&
                    UNITS_GetRoom(gpPlayerUnit) != nullptr &&
                    gdwClientGameTick >= 3u)
                {
                    if (g_tClientTiming.pfnRenderFrame != nullptr)
                    {
                        g_tClientTiming.pfnRenderFrame(0);
                    }
                    g_tClientTiming.dwRenderedFrameCount += 1u;
                    g_tClientTiming.nFpsSampleFrames     += 1;
                }
                else
                {
                    UI_StepAndDrawLoadingScreen(0);
                }
            }
            else if (bTickElapsed)
            {
                g_tClientTiming.nSkippedSampleFrames += 1;
            }

            if (bUpdatedProgress)
            {
                SOUNDHDR_UpdateFrame();
            }
            g_nConsecutiveSkippedFrames += 1;
        }
        else
        {
            if (WINDOW_IsFullScreen()) WINDOW_GetState();
        }
    }
    else
    {
        if (WINDOW_IsFullScreen()) WINDOW_GetState();
    }

    // 6. Continue modal loop while g_bExitGameFlag == 0; otherwise shut down session
    if (g_bExitGameFlag == 0)
    {
        return FALSE;
    }

    // [Out-of-scope: 0x6FB2D250 WARDEN_UnloadModule omitted]
    D2Win_10177(0);
    UI_CloseOpenPanels(1, 0);

    if (gnGameType == 0 || gnGameType == 1)
    {
        GAME_CloseAllGames();
    }
    if (gnGameType == 6 || gnGameType == 8)
    {
        EnterCriticalSection(&g_csServerThread);
        g_bStopServerThread = 1;
        LeaveCriticalSection(&g_csServerThread);
        WaitForSingleObject(g_hOpenServerThread, 10000);
        g_hOpenServerThread = nullptr;
    }

    return TRUE;
}

// 1.12a: 0x6FB2D2D0
int32_t __stdcall GAME_MainLoop(void)
{
    if (g_nDisconnectScreenFramesLeft == 0)
    {
        g_bExitAppFlag = 1;
    }

    GetTickCount();
    g_tOsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
#pragma warning(push)
#pragma warning(disable : 4996)
    if (GetVersionExA(&g_tOsVersionInfo))
#pragma warning(pop)
    {
        g_bOsVersionValid = 1;
    }

    ENV_AllocTileRenderPools();
    g_bCutsceneSkipFlagA  = 0;
    g_bDirectSoundCapable = (FOG_10191() != 0) ? 1u : 0u;
    g_bCutsceneSkipFlagB  = 0;
    g_bMoviePlayingFlag   = 0;

    SMsgPushRegisterState(WINDOW_GetWindow());
    CMDTBL_InitKeyBindings(0);

    const HWND hMainWnd = WINDOW_GetWindow();
    if (hMainWnd == nullptr)
    {
        FOG_DisplayHalt("", Fog_10265(0x179), 0x179);
        exit(-1);
    }

    for (size_t i = 0; i < 10; ++i)
    {
        WALL2_RegisterSingleMsgHandler(
            hMainWnd,
            g_aInGameSMsgTable[i].nType,
            g_aInGameSMsgTable[i].uMsgOrId,
            g_aInGameSMsgTable[i].pfHandler);
    }
    extern void __fastcall UI_RegisterHudAndPanelMsgHandlers(HWND hWnd);
    extern void __fastcall UI_UnregisterHudAndPanelMsgHandlers(HWND hWnd);
    UI_RegisterHudAndPanelMsgHandlers(hMainWnd);
    GAME_RegisterMouseWheelSupport();

    // Establish local/host or remote game connection
    if (gnGameType == 3 || gnGameType == 7 || gnGameType == 9)
    {
        MSG_SendJoinGame_68(g_dwJoinServerToken, g_wJoinGameId);
        if (gnGameType == 2 || gnGameType == 3)
        {
            // [Out-of-scope: 0x6FB2D41A WARDEN_InitClient(g_dwJoinServerToken) omitted]
        }
    }
    else
    {
        MSG_SendCreateGame_67(g_szConfigGameName);
        if (gnGameType == 6 || gnGameType == 8)
        {
            g_bStopServerThread  = 0;
            g_bServerThreadReady = 0;
            InitializeCriticalSection(&g_csServerThread);
            g_hOpenServerThread = CreateThread(
                nullptr,
                0,
                GAME_OpenServerThreadProc,
                nullptr,
                0,
                &g_dwOpenServerThreadId);
        }
    }

    SOUNDHDR_Init();
    GAME_InitClientState();

    // Drain any stale window messages if input recording or playback is enabled
    if (gbRecordingInputActive != 0 || gbRecordPlaybackActive != 0)
    {
        MSG msg = {};
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
        {
        }

        if (gbRecordPlaybackActive != 0)
        {
            gbRecordPlaybackReady = 1;
        }
        else if (gbRecordingInputActive != 0)
        {
            g_pfChainedExitCallback = g_pfWindowExitCallback;
            gbRecordingInputReady   = 1;
            g_pfWindowExitCallback  = RECORD_SaveAndInvokeExitCallback;
        }
    }

    FOG_SetMemoryPoolSystemMode(nullptr, FALSE);

    if (gnGameType == 6 || gnGameType == 8)
    {
        EnterCriticalSection(&g_csServerThread);
        g_bServerThreadReady = 1;
        LeaveCriticalSection(&g_csServerThread);
    }

    // Run primary in-game modal message & frame loop
    WALL2_RunModalMessageLoop(GAME_FrameLoop);

    GetTickCount();
    D2SOUND_SmpFadeMasterVolume(180);
    SOUNDCHANNEL_StopAllChannels();
    SOUND_StopAllActiveUnitSounds();

    // Optional disconnect error screen modal loop
    if (g_nDisconnectScreenFramesLeft != 0 &&
        g_bShowClassicEndGameScreen == 0 &&
        g_bShowExpansionEndGameScreen == 0)
    {
        WALL2_RunModalMessageLoop(GAME_DisconnectScreenLoop);
    }

    if (gbRecordingInputActive != 0)
    {
        RECORD_SaveToFile();
    }

    // Post-game victory / beta / epilogue cinematic flows
    uint8_t bShowExpansionVictoryScreen = 1;

    if (g_bShowBetaScreens != 0)
    {
        WALL2_UnregisterMsgHandlerTable(10, WINDOW_GetWindow(), g_aInGameSMsgTable);
        GAME_UnregisterMouseWheelSupport();
        WALL2_RegisterMsgHandlerTable(7, WINDOW_GetWindow(), g_aBetaScreenSMsgTable);
        WALL2_RunModalMessageLoop(GAME_BetaScreenLoop);
    }
    else if (g_bShowClassicEndGameScreen != 0)
    {
        CUTSCENE_PlayActMovie(5);
        WALL2_UnregisterMsgHandlerTable(10, WINDOW_GetWindow(), g_aInGameSMsgTable);
        GAME_UnregisterMouseWheelSupport();

        if (D2GFX_GetResolutionMode() != D2GAMERES_640x480)
        {
            gnScreenWidth            = 640;
            gnScreenHeight           = 480;
            g_nCurrentResolutionFlag = 0;
            gnViewportWidth          = 640;
            gnViewportHeight         = 440;
            gnScreenPitchWidth       = 640;
            D2Win_10005(D2GAMERES_640x480);
            VIEW_SetSkewMode(gnOpenPanelMode);
            AUTOMAP_CenterView(1);
            UI_UpdatePanelResolution();
        }

        WALL2_RegisterMsgHandlerTable(9, WINDOW_GetWindow(), g_aEndGameScreenSMsgTable);
        WALL2_RunModalMessageLoop(GAME_EndGameScreenLoop);
    }
    else if (g_bPlayExpansionEpilogueMovie != 0)
    {
        CUTSCENE_PlayActMovie(7);
        g_bShowExpansionEndGameScreen = 1;
    }

    if (g_bShowExpansionEndGameScreen != 0 && g_pGameData != nullptr)
    {
        const uint32_t nDiff     = static_cast<uint32_t>(gnClientDifficulty);
        const uint32_t nCurTitle = (static_cast<uint32_t>(g_pGameData->unpackedCTemp.nCharacterSaveFlags) >> 8) & 0x1Fu;

        if (nDiff == 0)
        {
            if (nCurTitle == 8u || nCurTitle == 12u)
            {
                bShowExpansionVictoryScreen = 0;
            }
        }
        else if (nDiff == 1)
        {
            if (nCurTitle == 12u)
            {
                bShowExpansionVictoryScreen = 0;
            }
        }

        const uint16_t wExpRank   = static_cast<uint16_t>((nDiff << 10) + 0x500u);
        const uint16_t wSaveFlags = static_cast<uint16_t>((g_pGameData->unpackedCTemp.nCharacterSaveFlags & 0xE0FFu) | wExpRank);
        g_pGameData->unpackedCTemp.nCharacterSaveFlags = wSaveFlags;

        if (bShowExpansionVictoryScreen != 0)
        {
            WALL2_UnregisterMsgHandlerTable(10, WINDOW_GetWindow(), g_aInGameSMsgTable);
            GAME_UnregisterMouseWheelSupport();

            if (D2GFX_GetResolutionMode() != D2GAMERES_800x600)
            {
                gnScreenWidth            = 800;
                gnScreenHeight           = 600;
                g_nCurrentResolutionFlag = 1;
                gnViewportWidth          = 800;
                gnViewportHeight         = 560;
                gnScreenPitchWidth       = 800;
                D2Win_10005(D2GAMERES_800x600);
                VIEW_SetSkewMode(gnOpenPanelMode);
                AUTOMAP_CenterView(1);
                UI_UpdatePanelResolution();
            }

            WALL2_RegisterMsgHandlerTable(9, WINDOW_GetWindow(), g_aEndGameScreenSMsgTable);
            WALL2_RunModalMessageLoop(GAME_EndGameScreenLoop);
        }
    }

    // Tear down world, caches, sound channels, and draw caches
    GAME_FreeClientState();

    if (D2SOUND_GetDirectSound() != nullptr)
    {
        SOUNDCHANNEL_StopAllChannels();
        SOUND_StopAllActiveUnitSounds();
        D2Client_InitSoundChannelPool();
        SOUNDCHANNEL_DestroyAllChannels();
        SOUNDHDR_FreeSoundTxtTables();
    }

    ENV_FreeTileRenderPoolsCheck();
    CURSOR_ResetToDefault();
    D2Win_10006_ClearDrawCaches();

    // Push D2CLIENT_STATE_PREPARE_NET (4) onto client state stack
    if (g_nClientStateStackDepth >= 5)
    {
        FOG_DisplayHalt("", Fog_10265(0xA6), 0xA6);
        exit(-1);
    }
    g_anClientStateStack[g_nClientStateStackDepth] = D2CLIENT_STATE_PREPARE_NET;
    g_nClientStateStackDepth += 1;

    // Unregister whichever SMsg handler table remained active
    if (g_bShowBetaScreens != 0)
    {
        WALL2_UnregisterMsgHandlerTable(7, WINDOW_GetWindow(), g_aBetaScreenSMsgTable);
        g_bShowBetaScreens = 0;
    }
    else if (g_bShowClassicEndGameScreen != 0)
    {
        WALL2_UnregisterMsgHandlerTable(9, WINDOW_GetWindow(), g_aEndGameScreenSMsgTable);
        g_bShowClassicEndGameScreen = 0;
    }
    else if (bShowExpansionVictoryScreen != 0 && g_bShowExpansionEndGameScreen != 0)
    {
        WALL2_UnregisterMsgHandlerTable(9, WINDOW_GetWindow(), g_aEndGameScreenSMsgTable);
        g_bShowExpansionEndGameScreen = 0;
    }
    else
    {
        UI_UnregisterHudAndPanelMsgHandlers(WINDOW_GetWindow());
        WALL2_UnregisterMsgHandlerTable(10, WINDOW_GetWindow(), g_aInGameSMsgTable);
        GAME_UnregisterMouseWheelSupport();
    }

    // Notify Battle.net interface of game leave for online game types
    if ((gnGameType == 2 || gnGameType == 6 || gnGameType == 3 || gnGameType == 7) &&
        g_pBnetComInterface != nullptr)
    {
        g_pBnetComInterface->LeaveGame();
    }

    return D2CLIENT_STATE_LEAVE_GAME;
}


// =============================================================================
// Cross-TU VA-Mapped Adapters for GAME_Game.cpp
// =============================================================================
extern void __fastcall CURSOR_DrawLockedItemTooltip(void);
extern void __stdcall CUTSCENE_PlayCutscene(int32_t nCutsceneIndex);
extern void D2Client_InitTileAndSpriteCaches(BOOL bReinit);
extern void __fastcall WINMAIN_SetResolutionMode(int32_t nResolutionMode);
extern int32_t __fastcall MSG_SendCreateGame_67(const char* szGameName, uint8_t nGameType, uint8_t nCharClass, uint8_t nTemplate, uint8_t nDifficulty, const char* szCharName, uint16_t wFlags1, uint32_t dwArenaFlags, uint8_t nUnk2B, uint8_t nUnk2C, uint8_t nLocale);
extern int32_t __fastcall MSG_SendJoinGame_68(uint32_t dwServerHash, uint16_t wServerToken, uint8_t nCharClass, uint32_t dwClientVersion, uint32_t dwArenaFlags, uint8_t nLocale, const char* szCharName);
extern int32_t __fastcall MSG_UploadSaveFile_6C(void* pSaveBuffer, uint32_t dwSaveSize);
extern void __stdcall UI_StepAndDrawLoadingScreen(void);
extern void __fastcall UI_RenderHUD(void);
extern void __stdcall DRLG_InitRoomClientCritters(D2ActiveRoomStrc* pRoom);
extern void __stdcall QUESTFX_SpawnTristramFiresInRoom(D2ActiveRoomStrc* pRoom);
extern void __stdcall QUESTFX_UpdatePortalAndSealFades(void);
extern void __fastcall MODAL_RenderOkScreen(BOOL bUnused);
extern void __fastcall MODAL_RenderOkCancelScreen(BOOL bUnused);
extern void __stdcall sub_6FB35DF0(void);
extern void __stdcall UI_CloseAllPanels(void);
extern void __fastcall UI_InitLoadingScreen(void);
extern void __stdcall UI_CloseMercInteract(void);
extern void __stdcall ENV_RenderViewportComplete(uint32_t* pViewport, int32_t bClearBackground);
struct SMsgWindowMessage;
extern void __fastcall WALL2_RegisterSingleMsgHandler(HWND hWnd, uint32_t nType, uint32_t uMsgOrId, void (__stdcall* pfHandler)(SMsgWindowMessage*));
extern int32_t __fastcall WALL2_PumpWindowMessages(void);
extern void __stdcall ENV_FreeOverlayCellFiles(void);
extern void __stdcall ENV_CreateAllPools(void);
extern void __stdcall ENV_UpdateEnvironmentFrame(void);
extern void __fastcall CMDTBL_InitKeyBindingsAndHandlers(void);

void __fastcall ARCHIVE_ReadMpqaFileBytes(HSFILE hFile, uint32_t nBytesToRead, void* pOutBuffer)
{
    DWORD dwRead = 0;
    if (hFile && pOutBuffer && nBytesToRead > 0) SFileReadFile(reinterpret_cast<int>(hFile), reinterpret_cast<int>(pOutBuffer), static_cast<int>(nBytesToRead), reinterpret_cast<int>(&dwRead), 0);
}
int32_t __fastcall CMDTBL_InitKeyBindings(int32_t nMode) { (void)nMode; CMDTBL_InitKeyBindingsAndHandlers(); return 1; }
void __fastcall CURSOR_RenderCursor(void) { CURSOR_DrawLockedItemTooltip(); }
int32_t __stdcall CUTSCENE_PlayActMovie(int32_t nMovie) { CUTSCENE_PlayCutscene(nMovie); return 1; }
void __fastcall D2Client_InitTileAndSpriteCaches_Adapter(void* pPoolSystem, BOOL bReinit) { (void)pPoolSystem; D2Client_InitTileAndSpriteCaches(bReinit); }
int32_t __fastcall GAME_SetResolutionMode(int32_t nMode) { WINMAIN_SetResolutionMode(nMode); return 1; }
void __fastcall GFX_PrecacheNearbyMonsterSprites(int32_t bAsync) { (void)bAsync; }
extern int32_t __stdcall UI_CloseOpenPanelsCount(int32_t bCloseAutomap);
// STRTABLE_GetLanguage is declared in <D2StrTable.h>

int32_t __fastcall MSG_SendCreateGame_67(const char* szGameName)
{
    if (gpD2Config_6FBCB908 != nullptr)
    {
        if (gpD2Config_6FBCB908->szName[0] != '\0')
        {
            SStrCopy(g_szConfigCharName, gpD2Config_6FBCB908->szName, sizeof(g_szConfigCharName));
        }
        gnLocalPlayerCharClass = gpD2Config_6FBCB908->unpackedCTemp.nCharacterClassId;
        gnClientDifficulty     = gpD2Config_6FBCB908->nArenaDifficulty;
        g_nArenaCharTemplate   = gpD2Config_6FBCB908->nArenaTemplate;
        if (gpD2Config_6FBCB908->nArenaFlags == 0)
        {
            gpD2Config_6FBCB908->nArenaFlags = 0x100004u;
        }
        gdwArenaFlags = gpD2Config_6FBCB908->nArenaFlags;
        gbIsExpansion = ((gpD2Config_6FBCB908->unpackedCTemp.nCharacterSaveFlags & 0x20u) != 0 || FOG_IsExpansion()) ? 1 : 0;
    }
    else
    {
        if (gdwArenaFlags == 0)
        {
            gdwArenaFlags = 0x100004u;
        }
        gbIsExpansion = FOG_IsExpansion() ? 1 : 0;
    }

    char szConnectHost[128] = {};
    if (GetEnvironmentVariableA("D2MOO_CONNECT_HOST", szConnectHost, sizeof(szConnectHost)) > 0 && szConnectHost[0] != '\0')
    {
        return MSG_SendJoinGame_68(1u, 1u);
    }

    const uint8_t nGameMode =
        (gnGameType == 0) ? 3u :
        (gnGameType == 6) ? 1u :
        (gnGameType == 8) ? 2u : 0u;
    const uint16_t wArena = gpD2Config_6FBCB908 ? gpD2Config_6FBCB908->wArena : 0u;
    const uint8_t nLocale = static_cast<uint8_t>(STRTABLE_GetLanguage());
    const char* szChar = g_szConfigCharName[0] ? g_szConfigCharName : "Hero";

    return MSG_SendCreateGame_67(
        (szGameName && szGameName[0]) ? szGameName : szChar,
        nGameMode,
        gnLocalPlayerCharClass,
        static_cast<uint8_t>(g_nArenaCharTemplate),
        gnClientDifficulty,
        szChar,
        wArena,
        gdwArenaFlags,
        0,
        0,
        nLocale);
}
int32_t __fastcall MSG_SendJoinGame_68(uint32_t dwJoinToken, uint16_t wGameId)
{
    return MSG_SendJoinGame_68(dwJoinToken, wGameId, gnLocalPlayerCharClass, 0x0Cu, gdwArenaFlags ? gdwArenaFlags : 0x506DFC26u, 0, g_szConfigCharName[0] ? g_szConfigCharName : "Hero");
}
int32_t __stdcall MSG_UploadSaveFile_6C(int32_t bLocalOrHost)
{
    (void)bLocalOrHost;
    if (!g_szConfigCharName[0])
    {
        return 0;
    }
    char szSaveDir[MAX_PATH] = {};
    char szSaveFile[MAX_PATH] = {};
    FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
    snprintf(szSaveFile, sizeof(szSaveFile), "%s%s.d2s", szSaveDir, g_szConfigCharName);
    HANDLE hFile = FOG_CreateFileA(szSaveFile, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
    const DWORD dwSize = GetFileSize(hFile, nullptr);
    if (dwSize == INVALID_FILE_SIZE || dwSize == 0 || dwSize > 0x20000u)
    {
        FOG_CloseFile(hFile);
        return 0;
    }
    void* pBuf = FOG_Alloc(static_cast<int>(dwSize), __FILE__, __LINE__, 0);
    DWORD dwRead = 0;
    const BOOL bOk = pBuf && FOG_ReadFile(hFile, pBuf, dwSize, &dwRead, nullptr) && dwRead == dwSize;
    FOG_CloseFile(hFile);
    int32_t nRes = 0;
    if (bOk)
    {
        nRes = MSG_UploadSaveFile_6C(pBuf, dwSize);
    }
    if (pBuf)
    {
        FOG_Free(pBuf, __FILE__, __LINE__, 0);
    }
    return nRes;
}
D2LevelsTxt* __stdcall OBJ_SpawnRoomEnvSoundObjects(D2ActiveRoomStrc* pRoom) { DRLG_InitRoomClientCritters(pRoom); return nullptr; }
int32_t __stdcall QUEST_SpawnDenOfEvilLightBeamInRoom(D2ActiveRoomStrc* pRoom) { QUESTFX_SpawnTristramFiresInRoom(pRoom); return 1; }
void __stdcall QUEST_UpdateEnvironmentEffects(void) { QUESTFX_UpdatePortalAndSealFades(); }
void __fastcall RENDER_RenderCutsceneOverlayMode1(BOOL bArg) { MODAL_RenderOkScreen(bArg); }
void __fastcall RENDER_RenderCutsceneOverlayMode2(BOOL bArg) { MODAL_RenderOkCancelScreen(bArg); }
void __fastcall SCMD_HandlerB3_ReceiveSaveChunk(const uint8_t* pPacket) { (void)pPacket; }
void __fastcall SCMD_HandlerB4_ConnectionRefusedReason(const uint8_t* pPacket) { (void)pPacket; }
void __stdcall SOUNDCHANNEL_DestroyAllChannels(void) {}
void __stdcall SOUNDCHANNEL_StopAllChannels(void) {}
void __stdcall SOUND_StopAllActiveUnitSounds(void) {}
void __stdcall TEXT_PollBnetRealmChat(void) { sub_6FB35DF0(); }
int32_t __stdcall UI_CloseOpenPanels(int32_t bPlaySoundOrMoveMouse, int32_t bCloseAutomap) { (void)bPlaySoundOrMoveMouse; return UI_CloseOpenPanelsCount(bCloseAutomap); }
void __fastcall UI_LoadLoadingScreen(void) { UI_InitLoadingScreen(); }
void __stdcall UI_StepAndDrawLoadingScreen(int32_t bAdvanceFrame) { (void)bAdvanceFrame; UI_StepAndDrawLoadingScreen(); }
void __stdcall UI_RenderHUD(D2ViewportStrc* pViewport) { (void)pViewport; UI_RenderHUD(); }
void __stdcall UI_UpdatePanelResolution(void) { UI_CloseMercInteract(); }
void __fastcall WALL2_RegisterSingleMsgHandler(HWND hWnd, uint32_t nType, uint32_t uMsg, void (__stdcall* pfHandler)(SMSGHANDLER_PARAMS*))
{
    WALL2_RegisterSingleMsgHandler(hWnd, nType, uMsg, reinterpret_cast<void (__stdcall*)(SMsgWindowMessage*)>(pfHandler));
}
int32_t __fastcall WALL2_RunModalMessageLoop(SMSGIDLEPROC pfIdleCallback)
{
    DWORD dwCount = 0;
    while (WALL2_PumpWindowMessages() && pfIdleCallback && !pfIdleCallback(dwCount++))
    {
    }
    return 1;
}
void __stdcall WEATHER_FreeEnvironmentCellFiles(void) { ENV_FreeOverlayCellFiles(); }
void __stdcall WEATHER_InitEnvironmentPools(void) { ENV_CreateAllPools(); }
void __stdcall WEATHER_UpdateParticles(void) { ENV_UpdateEnvironmentFrame(); }

// =============================================================================
// Client DRLG Room Streaming & Boundary Prestreaming Helpers
// =============================================================================
struct D2AutomapLayerStrc;
extern D2AutomapLayerStrc* __fastcall AUTOMAP_InitLayer(int32_t nLayerNo);

extern "C" D2ActiveRoomStrc* __fastcall D2Client_SafeStreamRoomAtTile(int32_t nTileX, int32_t nTileY, int32_t nExplicitLevelId)
{
    if (!gpClientAct || !gpClientAct->pDrlg)
    {
        return nullptr;
    }
    if (D2ActiveRoomStrc* pExisting = DUNGEON_FindRoomByTileCoordinates(gpClientAct, nTileX, nTileY))
    {
        if (nExplicitLevelId <= 0 || DUNGEON_GetLevelIdFromRoom(pExisting) == nExplicitLevelId)
        {
            return pExisting;
        }
    }

    D2DrlgStrc* pDrlg = gpClientAct->pDrlg;
    D2DrlgRoomStrc* pMatchedRoomEx = nullptr;
    int32_t nMatchedLevelId = 0;
    D2DrlgLevelStrc* pExplicitInitLvl = nullptr;

    if (nExplicitLevelId > 0)
    {
        if (D2DrlgLevelStrc* pExplicitLvl = DRLG_GetLevel(pDrlg, nExplicitLevelId))
        {
            if (!pExplicitLvl->pFirstRoomEx)
            {
                DRLG_InitLevel(pExplicitLvl);
            }
            pExplicitInitLvl = pExplicitLvl;
            for (D2DrlgRoomStrc* pRoomEx = pExplicitLvl->pFirstRoomEx; pRoomEx != nullptr; pRoomEx = pRoomEx->pDrlgRoomNext)
            {
                if (pRoomEx->nTileWidth > 0 && pRoomEx->nTileHeight > 0 &&
                    nTileX >= pRoomEx->nTileXPos && nTileX < pRoomEx->nTileXPos + pRoomEx->nTileWidth &&
                    nTileY >= pRoomEx->nTileYPos && nTileY < pRoomEx->nTileYPos + pRoomEx->nTileHeight)
                {
                    pMatchedRoomEx = pRoomEx;
                    nMatchedLevelId = nExplicitLevelId;
                    break;
                }
            }
        }
    }

    if (!pMatchedRoomEx)
    {
        const int32_t nCurPlrLvl = (gpPlayerUnit && UNITS_GetRoom(gpPlayerUnit)) ? DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(gpPlayerUnit)) : 0;
        const D2LevelsTxt* pCurPlrLvlTxt = (nCurPlrLvl > 0) ? DATATBLS_GetLevelsTxtRecord(nCurPlrLvl) : nullptr;
        const bool bPreferCurrentIndoor = (pCurPlrLvlTxt != nullptr && pCurPlrLvlTxt->nIsInside != 0);

        for (int32_t pass = 0; pass < 2 && !pMatchedRoomEx; ++pass)
        {
            for (D2DrlgLevelStrc* pLvl = pDrlg->pLevel; pLvl != nullptr; pLvl = pLvl->pNextLevel)
            {
                if (pLvl->nLevelId <= 0 || !pLvl->pFirstRoomEx)
                {
                    continue;
                }
                if (pass == 0 && bPreferCurrentIndoor && pLvl->nLevelId != nCurPlrLvl)
                {
                    continue;
                }
                if (pass == 0 && !bPreferCurrentIndoor)
                {
                    const D2LevelsTxt* pCandTxt = DATATBLS_GetLevelsTxtRecord(pLvl->nLevelId);
                    if (pCandTxt && pCandTxt->nIsInside != 0)
                    {
                        continue;
                    }
                }
                for (D2DrlgRoomStrc* pRoomEx = pLvl->pFirstRoomEx; pRoomEx != nullptr; pRoomEx = pRoomEx->pDrlgRoomNext)
                {
                    if (pRoomEx->nTileWidth > 0 && pRoomEx->nTileHeight > 0 &&
                        nTileX >= pRoomEx->nTileXPos && nTileX < pRoomEx->nTileXPos + pRoomEx->nTileWidth &&
                        nTileY >= pRoomEx->nTileYPos && nTileY < pRoomEx->nTileYPos + pRoomEx->nTileHeight)
                    {
                        pMatchedRoomEx = pRoomEx;
                        nMatchedLevelId = pLvl->nLevelId;
                        break;
                    }
                }
                if (pMatchedRoomEx)
                {
                    break;
                }
            }
        }
    }

    if (!pMatchedRoomEx)
    {
        for (D2DrlgLevelStrc* pLvl = pDrlg->pLevel; pLvl != nullptr; pLvl = pLvl->pNextLevel)
        {
            if (pLvl->nLevelId <= 0 || pLvl->pFirstRoomEx != nullptr || pLvl->nWidth <= 0 || pLvl->nHeight <= 0)
            {
                continue;
            }
            const D2LevelsTxt* pCandTxt = DATATBLS_GetLevelsTxtRecord(pLvl->nLevelId);
            if (pCandTxt && pCandTxt->nIsInside != 0)
            {
                continue;
            }
            if (nTileX >= pLvl->nPosX && nTileX < pLvl->nPosX + pLvl->nWidth &&
                nTileY >= pLvl->nPosY && nTileY < pLvl->nPosY + pLvl->nHeight)
            {
                DRLG_InitLevel(pLvl);
                for (D2DrlgRoomStrc* pRoomEx = pLvl->pFirstRoomEx; pRoomEx != nullptr; pRoomEx = pRoomEx->pDrlgRoomNext)
                {
                    if (pRoomEx->nTileWidth > 0 && pRoomEx->nTileHeight > 0 &&
                        nTileX >= pRoomEx->nTileXPos && nTileX < pRoomEx->nTileXPos + pRoomEx->nTileWidth &&
                        nTileY >= pRoomEx->nTileYPos && nTileY < pRoomEx->nTileYPos + pRoomEx->nTileHeight)
                    {
                        pMatchedRoomEx = pRoomEx;
                        nMatchedLevelId = pLvl->nLevelId;
                        break;
                    }
                }
                if (pMatchedRoomEx)
                {
                    break;
                }
            }
        }
    }

    if (!pMatchedRoomEx || nMatchedLevelId <= 0)
    {
        return nullptr;
    }

    D2ActiveRoomStrc* pPlayerRoom = gpPlayerUnit ? UNITS_GetRoom(gpPlayerUnit) : nullptr;
    DUNGEON_SetClientIsInSight(gpClientAct, nMatchedLevelId, pMatchedRoomEx->nTileXPos, pMatchedRoomEx->nTileYPos, pPlayerRoom);
    D2ActiveRoomStrc* pStreamed = pMatchedRoomEx->pRoom;
    if (!pStreamed)
    {
        pStreamed = DUNGEON_FindRoomByTileCoordinates(gpClientAct, pMatchedRoomEx->nTileXPos, pMatchedRoomEx->nTileYPos);
    }
    if (pStreamed)
    {
        AUTOMAP_RevealRoomFull(pStreamed);
    }

    static bool s_bPrestreamingIndoor = false;
    if (!s_bPrestreamingIndoor && pExplicitInitLvl && pExplicitInitLvl->pFirstRoomEx)
    {
        const D2LevelsTxt* pExpTxt = DATATBLS_GetLevelsTxtRecord(nExplicitLevelId);
        if (pExpTxt && pExpTxt->nIsInside != 0)
        {
            s_bPrestreamingIndoor = true;
            int32_t nSafety = 0;
            for (D2DrlgRoomStrc* pSubEx = pExplicitInitLvl->pFirstRoomEx; pSubEx != nullptr && nSafety < 96; pSubEx = pSubEx->pDrlgRoomNext, ++nSafety)
            {
                if (!pSubEx->pRoom && pSubEx->nTileWidth > 0 && pSubEx->nTileHeight > 0)
                {
                    D2Client_SafeStreamRoomAtTile(pSubEx->nTileXPos, pSubEx->nTileYPos, nExplicitLevelId);
                }
            }
            s_bPrestreamingIndoor = false;
        }
    }

    return pStreamed;
}

static int32_t GAME_GetAdjacentWildernessLevelForTown(int32_t nTownLevelId)
{
    switch (nTownLevelId)
    {
    case LEVEL_ROGUEENCAMPMENT:        return LEVEL_BLOODMOOR;
    case LEVEL_LUTGHOLEIN:             return LEVEL_ROCKYWASTE;
    case LEVEL_KURASTDOCKTOWN:         return LEVEL_SPIDERFOREST;
    case LEVEL_THEPANDEMONIUMFORTRESS: return LEVEL_OUTERSTEPPES;
    case LEVEL_HARROGATH:              return LEVEL_BLOODYFOOTHILLS;
    default:                           return 0;
    }
}

static void GAME_PrestreamTownAndAdjacentWilderness(int32_t nTownLevelId)
{
    if (!gpClientAct || !gpClientAct->pDrlg || nTownLevelId <= 0)
    {
        return;
    }

    D2DrlgLevelStrc* pTownLvl = DRLG_GetLevel(gpClientAct->pDrlg, nTownLevelId);
    if (!pTownLvl)
    {
        return;
    }
    if (!pTownLvl->pFirstRoomEx)
    {
        DRLG_InitLevel(pTownLvl);
    }

    const int32_t nWildLevelId = GAME_GetAdjacentWildernessLevelForTown(nTownLevelId);
    D2DrlgLevelStrc* pWildLvl = (nWildLevelId > 0) ? DRLG_GetLevel(gpClientAct->pDrlg, nWildLevelId) : nullptr;
    if (pWildLvl && !pWildLvl->pFirstRoomEx)
    {
        DRLG_InitLevel(pWildLvl);
    }

    D2LevelDefBin* pLvlDef = DATATBLS_GetLevelDefRecord(nTownLevelId);
    const int32_t nLayerNo = (pLvlDef && pLvlDef->dwLayer > 0) ? static_cast<int32_t>(pLvlDef->dwLayer) : nTownLevelId;
    AUTOMAP_InitLayer(nLayerNo);

    for (D2DrlgRoomStrc* pRoomEx = pTownLvl->pFirstRoomEx; pRoomEx != nullptr; pRoomEx = pRoomEx->pDrlgRoomNext)
    {
        D2Client_SafeStreamRoomAtTile(pRoomEx->nTileXPos, pRoomEx->nTileYPos, nTownLevelId);
        if (pRoomEx->ppRoomsNear && pRoomEx->nRoomsNear > 0)
        {
            for (int32_t i = 0; i < pRoomEx->nRoomsNear; ++i)
            {
                if (D2DrlgRoomStrc* pNear = pRoomEx->ppRoomsNear[i])
                {
                    const int32_t nNearLvl = pNear->pLevel ? pNear->pLevel->nLevelId : 0;
                    D2Client_SafeStreamRoomAtTile(pNear->nTileXPos, pNear->nTileYPos, nNearLvl);
                }
            }
        }
    }

    if (pWildLvl && pWildLvl->pFirstRoomEx)
    {
        const int32_t nTownMinX = pTownLvl->nPosX - 28;
        const int32_t nTownMaxX = pTownLvl->nPosX + pTownLvl->nWidth + 28;
        const int32_t nTownMinY = pTownLvl->nPosY - 28;
        const int32_t nTownMaxY = pTownLvl->nPosY + pTownLvl->nHeight + 28;
        for (D2DrlgRoomStrc* pWildRoom = pWildLvl->pFirstRoomEx; pWildRoom != nullptr; pWildRoom = pWildRoom->pDrlgRoomNext)
        {
            const int32_t rx0 = pWildRoom->nTileXPos;
            const int32_t ry0 = pWildRoom->nTileYPos;
            const int32_t rx1 = rx0 + pWildRoom->nTileWidth;
            const int32_t ry1 = ry0 + pWildRoom->nTileHeight;
            if (rx1 >= nTownMinX && rx0 <= nTownMaxX && ry1 >= nTownMinY && ry0 <= nTownMaxY)
            {
                D2Client_SafeStreamRoomAtTile(rx0, ry0, nWildLevelId);
            }
        }
    }
}

// =============================================================================
// Self-Contained Opt-In Verification Harness (D2MOO_NATIVE_AUTOTEST=1)
// =============================================================================
extern int32_t __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t bMoveMouse);
extern void __stdcall QUESTLOG_TogglePanel(int32_t bPlaySound);
extern int32_t __stdcall QUESTLOG_ClosePanel(int32_t bPlaySound);
extern void __fastcall D2Client_InteractWithNPC_6FAE30C0(D2UnitStrc* pNpc);
extern void __stdcall D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(D2UnitStrc* pUnit, int32_t nArgXOrType, uint32_t dwArgYOrGUID, int32_t nActionCode, BOOL bForce);
extern int32_t g_nInvPanelMode;
extern D2UnitStrc* g_pTradeNpcUnit;
extern int32_t g_bInteractingWithNpc;
extern int32_t g_nInteractingNpcGUID;
extern int32_t g_nInteractingNpcClassId;
extern int32_t gdwActiveInteractNpcGUID;
extern D2UnitStrc* __fastcall D2Client_FindServerSideUnit(uint32_t dwUnitId, uint32_t dwUnitType);
extern void __fastcall MSG_GetTrafficCounters(uint32_t* pOutBytesRecv, uint32_t* pOutPktsRecv, uint32_t* pOutBytesSent, uint32_t* pOutPktsSent);
extern void __fastcall MSG_ResetLastSentPacket(void);
extern int32_t __fastcall MSG_SendByte2Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2);
extern int32_t __fastcall MSG_SendByte3Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2, uint32_t dw3);

static void Autotest_StreamNeighborhoodAroundTile(int32_t nCenterTileX, int32_t nCenterTileY, int32_t nRadiusTiles)
{
    if (!gpClientAct) return;
    for (int32_t dy = -nRadiusTiles; dy <= nRadiusTiles; dy += 4)
    {
        for (int32_t dx = -nRadiusTiles; dx <= nRadiusTiles; dx += 4)
        {
            D2Client_SafeStreamRoomAtTile(nCenterTileX + dx, nCenterTileY + dy, 0);
        }
    }
}

static bool Autotest_IsSubtileWalkable(int32_t sx, int32_t sy, bool bRequireMargin)
{
    if (sx <= 0 || sy <= 0 || !gpClientAct) return false;
    D2ActiveRoomStrc* pRoom = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, sx, sy);
    if (!pRoom) pRoom = D2Client_SafeStreamRoomAtTile(sx / 5, sy / 5, 0);
    if (!pRoom || COLLISION_CheckMaskWithSize(pRoom, sx, sy, COLLISION_UNIT_SIZE_SMALL, COLLIDE_WALL | COLLIDE_NOPLAYER | COLLIDE_OBJECT | COLLIDE_DOOR | COLLIDE_BLANK))
    {
        return false;
    }
    if (bRequireMargin)
    {
        static const int32_t kOffs[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
        for (int32_t i = 0; i < 4; ++i)
        {
            const int32_t nx = sx + kOffs[i][0];
            const int32_t ny = sy + kOffs[i][1];
            D2ActiveRoomStrc* pAdj = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, nx, ny);
            if (!pAdj) pAdj = D2Client_SafeStreamRoomAtTile(nx / 5, ny / 5, 0);
            if (!pAdj || COLLISION_CheckMask(pAdj, nx, ny, COLLIDE_WALL | COLLIDE_NOPLAYER | COLLIDE_OBJECT | COLLIDE_BLANK))
            {
                return false;
            }
        }
    }
    return true;
}

static void Autotest_FindNearestWalkableSubtile(int32_t nBaseX, int32_t nBaseY, int32_t nMaxRadius, int32_t* pOutX, int32_t* pOutY)
{
    *pOutX = nBaseX;
    *pOutY = nBaseY;
    for (int32_t pass = 0; pass < 2; ++pass)
    {
        const bool bMargin = (pass == 0);
        for (int32_t r = 0; r <= nMaxRadius; ++r)
        {
            for (int32_t dy = -r; dy <= r; ++dy)
            {
                for (int32_t dx = -r; dx <= r; ++dx)
                {
                    if (abs(dx) != r && abs(dy) != r) continue;
                    const int32_t sx = nBaseX + dx;
                    const int32_t sy = nBaseY + dy;
                    if (Autotest_IsSubtileWalkable(sx, sy, bMargin))
                    {
                        *pOutX = sx;
                        *pOutY = sy;
                        return;
                    }
                }
            }
        }
    }
}

static bool Autotest_ComputeWalkablePath(int32_t rawStartX, int32_t rawStartY, int32_t rawGoalX, int32_t rawGoalY, std::vector<POINT>& outWaypoints)
{
    outWaypoints.clear();
    if (!gpClientAct || rawStartX <= 0 || rawStartY <= 0 || rawGoalX <= 0 || rawGoalY <= 0) return false;

    GAME_PrestreamTownAndAdjacentWilderness(static_cast<int32_t>(g_dwCurrentTownLevelId ? g_dwCurrentTownLevelId : LEVEL_ROGUEENCAMPMENT));
    Autotest_StreamNeighborhoodAroundTile(rawStartX / 5, rawStartY / 5, 24);
    Autotest_StreamNeighborhoodAroundTile(rawGoalX / 5, rawGoalY / 5, 28);

    int32_t startX = rawStartX, startY = rawStartY;
    int32_t goalX = rawGoalX, goalY = rawGoalY;
    Autotest_FindNearestWalkableSubtile(rawStartX, rawStartY, 10, &startX, &startY);
    Autotest_FindNearestWalkableSubtile(rawGoalX, rawGoalY, 10, &goalX, &goalY);

    const int32_t minX = ((startX < goalX ? startX : goalX) - 180) & ~1;
    const int32_t maxX = ((startX > goalX ? startX : goalX) + 180) & ~1;
    const int32_t minY = ((startY < goalY ? startY : goalY) - 180) & ~1;
    const int32_t maxY = ((startY > goalY ? startY : goalY) + 180) & ~1;

    const int32_t gridW = (maxX - minX) / 2 + 1;
    const int32_t gridH = (maxY - minY) / 2 + 1;
    if (gridW <= 2 || gridH <= 2 || gridW * gridH > 262144)
    {
        outWaypoints.push_back({ goalX, goalY });
        return true;
    }

    const int32_t sgx = (startX - minX) / 2;
    const int32_t sgy = (startY - minY) / 2;
    const int32_t ggx = (goalX - minX) / 2;
    const int32_t ggy = (goalY - minY) / 2;
    const int32_t startIdx = sgy * gridW + sgx;

    static const int32_t kDirs[8][2] = {
        { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 },
        { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }
    };

    std::vector<int32_t> vParent;
    int32_t reachedIdx = -1;
    for (int32_t pass = 0; pass < 2 && reachedIdx < 0; ++pass)
    {
        const bool bMargin = (pass == 0);
        vParent.assign(static_cast<size_t>(gridW * gridH), -1);
        std::queue<int32_t> q;
        vParent[startIdx] = startIdx;
        q.push(startIdx);

        int32_t bestFallbackIdx = startIdx;
        int64_t bestFallbackDistSq = 0x7FFFFFFFFFFFFFFFLL;

        while (!q.empty())
        {
            const int32_t cur = q.front();
            q.pop();
            const int32_t cx = cur % gridW;
            const int32_t cy = cur / gridW;
            const int64_t ddx = static_cast<int64_t>(cx - ggx);
            const int64_t ddy = static_cast<int64_t>(cy - ggy);
            const int64_t d2 = ddx * ddx + ddy * ddy;
            if (d2 < bestFallbackDistSq)
            {
                bestFallbackDistSq = d2;
                bestFallbackIdx = cur;
            }
            if (d2 <= 1)
            {
                reachedIdx = cur;
                break;
            }
            const int32_t curSubX = minX + cx * 2;
            const int32_t curSubY = minY + cy * 2;
            for (int32_t d = 0; d < 8; ++d)
            {
                const int32_t nx = cx + kDirs[d][0];
                const int32_t ny = cy + kDirs[d][1];
                if (nx < 0 || nx >= gridW || ny < 0 || ny >= gridH) continue;
                const int32_t nIdx = ny * gridW + nx;
                if (vParent[nIdx] != -1) continue;
                const int32_t nextSubX = minX + nx * 2;
                const int32_t nextSubY = minY + ny * 2;
                const int32_t midSubX = curSubX + kDirs[d][0];
                const int32_t midSubY = curSubY + kDirs[d][1];
                if (!Autotest_IsSubtileWalkable(midSubX, midSubY, false) ||
                    !Autotest_IsSubtileWalkable(nextSubX, nextSubY, bMargin))
                {
                    continue;
                }
                vParent[nIdx] = cur;
                q.push(nIdx);
            }
        }
        if (reachedIdx < 0 && pass == 1 && bestFallbackDistSq <= 9)
        {
            reachedIdx = bestFallbackIdx;
        }
    }

    if (reachedIdx < 0)
    {
        outWaypoints.push_back({ goalX, goalY });
        return false;
    }

    std::vector<POINT> vDense;
    int32_t trace = reachedIdx;
    for (int32_t guard = 0; guard < gridW * gridH; ++guard)
    {
        const int32_t tx = minX + (trace % gridW) * 2;
        const int32_t ty = minY + (trace / gridW) * 2;
        vDense.push_back({ tx, ty });
        if (trace == startIdx) break;
        trace = vParent[trace];
    }
    std::reverse(vDense.begin(), vDense.end());

    POINT ptLast = { startX, startY };
    for (size_t i = 1; i < vDense.size(); ++i)
    {
        const double dx = static_cast<double>(vDense[i].x - ptLast.x);
        const double dy = static_cast<double>(vDense[i].y - ptLast.y);
        if (sqrt(dx * dx + dy * dy) >= 7.0)
        {
            outWaypoints.push_back(vDense[i]);
            ptLast = vDense[i];
        }
    }
    if (outWaypoints.empty() || outWaypoints.back().x != goalX || outWaypoints.back().y != goalY)
    {
        outWaypoints.push_back({ goalX, goalY });
    }
    return true;
}

static bool Autotest_IsCorridorClear(int32_t bx, int32_t by, int32_t nx, int32_t ny, int32_t dMin, int32_t dMax)
{
    for (int32_t d = dMin; d <= dMax; d += 2)
    {
        const int32_t sx = bx + nx * d;
        const int32_t sy = by + ny * d;
        D2ActiveRoomStrc* pRoom = D2Client_SafeStreamRoomAtTile(sx / 5, sy / 5, 0);
        if (!pRoom || COLLISION_CheckMask(pRoom, sx, sy, COLLIDE_WALL)) return false;
    }
    return true;
}

static bool Autotest_ComputeBloodMoorRoute(POINT aOutWaypoints[4])
{
    if (!gpClientAct || !gpClientAct->pDrlg) return false;
    GAME_PrestreamTownAndAdjacentWilderness(LEVEL_ROGUEENCAMPMENT);
    D2DrlgLevelStrc* pTownLvl = DRLG_GetLevel(gpClientAct->pDrlg, LEVEL_ROGUEENCAMPMENT);
    if (!pTownLvl) return false;

    const int32_t nTownCenterX = (pTownLvl->nPosX + pTownLvl->nWidth / 2) * 5;
    const int32_t nTownCenterY = (pTownLvl->nPosY + pTownLvl->nHeight / 2) * 5;
    int32_t wp0X = nTownCenterX, wp0Y = nTownCenterY;
    Autotest_FindNearestWalkableSubtile(nTownCenterX, nTownCenterY, 24, &wp0X, &wp0Y);
    aOutWaypoints[0] = { wp0X, wp0Y };

    int32_t nBestBridgeX = 0, nBestBridgeY = 0, nBestNormX = 0, nBestNormY = 0, nBestClearScore = -1;
    for (D2ActiveRoomStrc* pTownRoom = DUNGEON_GetRoomFromAct(gpClientAct); pTownRoom != nullptr; pTownRoom = pTownRoom->pRoomNext)
    {
        if (DUNGEON_GetLevelIdFromRoom(pTownRoom) != LEVEL_ROGUEENCAMPMENT) continue;
        RECT rcT = {};
        DUNGEON_GetSubtileRect(pTownRoom, &rcT);
        for (D2ActiveRoomStrc* pMoorRoom = DUNGEON_GetRoomFromAct(gpClientAct); pMoorRoom != nullptr; pMoorRoom = pMoorRoom->pRoomNext)
        {
            if (DUNGEON_GetLevelIdFromRoom(pMoorRoom) != LEVEL_BLOODMOOR) continue;
            RECT rcM = {};
            DUNGEON_GetSubtileRect(pMoorRoom, &rcM);

            int32_t nx = 0, ny = 0, segStart = 0, segEnd = 0, fixedCoord = 0;
            bool bVert = false;
            if (rcT.right == rcM.left && (rcT.bottom < rcM.bottom ? rcT.bottom : rcM.bottom) > (rcT.top > rcM.top ? rcT.top : rcM.top))
            {
                nx = 1; ny = 0; bVert = true; fixedCoord = rcT.right;
                segStart = (rcT.top > rcM.top ? rcT.top : rcM.top) + 2;
                segEnd   = (rcT.bottom < rcM.bottom ? rcT.bottom : rcM.bottom) - 2;
            }
            else if (rcT.left == rcM.right && (rcT.bottom < rcM.bottom ? rcT.bottom : rcM.bottom) > (rcT.top > rcM.top ? rcT.top : rcM.top))
            {
                nx = -1; ny = 0; bVert = true; fixedCoord = rcT.left;
                segStart = (rcT.top > rcM.top ? rcT.top : rcM.top) + 2;
                segEnd   = (rcT.bottom < rcM.bottom ? rcT.bottom : rcM.bottom) - 2;
            }
            else if (rcT.bottom == rcM.top && (rcT.right < rcM.right ? rcT.right : rcM.right) > (rcT.left > rcM.left ? rcT.left : rcM.left))
            {
                nx = 0; ny = 1; bVert = false; fixedCoord = rcT.bottom;
                segStart = (rcT.left > rcM.left ? rcT.left : rcM.left) + 2;
                segEnd   = (rcT.right < rcM.right ? rcT.right : rcM.right) - 2;
            }
            else if (rcT.top == rcM.bottom && (rcT.right < rcM.right ? rcT.right : rcM.right) > (rcT.left > rcM.left ? rcT.left : rcM.left))
            {
                nx = 0; ny = -1; bVert = false; fixedCoord = rcT.top;
                segStart = (rcT.left > rcM.left ? rcT.left : rcM.left) + 2;
                segEnd   = (rcT.right < rcM.right ? rcT.right : rcM.right) - 2;
            }
            else continue;

            for (int32_t t = segStart; t <= segEnd; ++t)
            {
                const int32_t bx = bVert ? fixedCoord : t;
                const int32_t by = bVert ? t : fixedCoord;
                if (!Autotest_IsCorridorClear(bx, by, nx, ny, -16, 20)) continue;
                const int32_t px = -ny, py = nx;
                int32_t nScore = 1;
                for (int32_t lat = 1; lat <= 4; ++lat)
                {
                    if (Autotest_IsCorridorClear(bx + px * lat, by + py * lat, nx, ny, -10, 12)) ++nScore;
                    if (Autotest_IsCorridorClear(bx - px * lat, by - py * lat, nx, ny, -10, 12)) ++nScore;
                }
                if (nScore > nBestClearScore)
                {
                    nBestClearScore = nScore;
                    nBestBridgeX = bx;
                    nBestBridgeY = by;
                    nBestNormX = nx;
                    nBestNormY = ny;
                }
            }
        }
    }
    if (nBestClearScore < 0) return false;

    int32_t wp1X = nBestBridgeX - nBestNormX * 28;
    int32_t wp1Y = nBestBridgeY - nBestNormY * 28;
    Autotest_FindNearestWalkableSubtile(wp1X, wp1Y, 12, &wp1X, &wp1Y);

    int32_t wp3X = nBestBridgeX + nBestNormX * 20;
    int32_t wp3Y = nBestBridgeY + nBestNormY * 20;
    Autotest_StreamNeighborhoodAroundTile(wp3X / 5, wp3Y / 5, 28);
    Autotest_FindNearestWalkableSubtile(wp3X, wp3Y, 10, &wp3X, &wp3Y);

    aOutWaypoints[1] = { wp1X, wp1Y };
    aOutWaypoints[2] = { nBestBridgeX - nBestNormX * 8, nBestBridgeY - nBestNormY * 8 };
    aOutWaypoints[3] = { wp3X, wp3Y };
    return true;
}

static D2UnitStrc* Autotest_FindTownVendor()
{
    D2UnitStrc* pBest = nullptr;
    int64_t nBestD2 = 0x7FFFFFFFFFFFFFFFLL;
    const int32_t px = gpPlayerUnit ? UNITS_GetXPosition(gpPlayerUnit) : 0;
    const int32_t py = gpPlayerUnit ? UNITS_GetYPosition(gpPlayerUnit) : 0;
    for (int32_t b = 0; b < 128; ++b)
    {
        for (D2UnitStrc* pU = gapServerSideUnitHashTables[UNIT_MONSTER][b]; pU != nullptr; pU = pU->pListNext)
        {
            const uint32_t nCls = pU->dwClassId;
            if (nCls != 148 && nCls != 154 && nCls != 147) continue;
            const int64_t dx = static_cast<int64_t>(UNITS_GetXPosition(pU) - px);
            const int64_t dy = static_cast<int64_t>(UNITS_GetYPosition(pU) - py);
            const int64_t d2 = dx * dx + dy * dy;
            if (d2 < nBestD2)
            {
                nBestD2 = d2;
                pBest = pU;
            }
        }
    }
    return pBest;
}

static void Autotest_CaptureBmp(const char* szOutDir, const char* szFileName)
{
    g_nPostActLoadBlackFrames = 0;
    GAME_RenderFrame(0);
    HWND hWnd = WINDOW_GetWindow();
    if (!hWnd) return;

    char szOutPath[MAX_PATH * 2] = {};
    snprintf(szOutPath, sizeof(szOutPath), "%s\\%s", szOutDir, szFileName);

    int32_t nSurfW = 0, nSurfH = 0;
    WINDOW_GetDimensions(&nSurfW, &nSurfH);
    if (nSurfW > 0 && nSurfH > 0)
    {
        std::vector<uint8_t> vRgb(static_cast<size_t>(nSurfW) * static_cast<size_t>(nSurfH) * 3u);
        if (D2GFX_GetBackBuffer(vRgb.data()))
        {
            const uint32_t dwRowStride = ((static_cast<uint32_t>(nSurfW) * 3u + 3u) & ~3u);
            const uint32_t dwImgSize = dwRowStride * static_cast<uint32_t>(nSurfH);
            std::vector<uint8_t> vBmpRows(dwImgSize, 0u);
            for (int32_t y = 0; y < nSurfH; ++y)
            {
                const uint8_t* pSrc = vRgb.data() + static_cast<size_t>(nSurfH - 1 - y) * static_cast<size_t>(nSurfW) * 3u;
                uint8_t* pDst = vBmpRows.data() + static_cast<size_t>(y) * dwRowStride;
                for (int32_t x = 0; x < nSurfW; ++x)
                {
                    pDst[x * 3 + 0] = pSrc[x * 3 + 2];
                    pDst[x * 3 + 1] = pSrc[x * 3 + 1];
                    pDst[x * 3 + 2] = pSrc[x * 3 + 0];
                }
            }
            BITMAPFILEHEADER bfh = {};
            bfh.bfType = 0x4D42;
            bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
            bfh.bfSize = bfh.bfOffBits + dwImgSize;
            BITMAPINFOHEADER bih = {};
            bih.biSize = sizeof(BITMAPINFOHEADER);
            bih.biWidth = nSurfW;
            bih.biHeight = nSurfH;
            bih.biPlanes = 1;
            bih.biBitCount = 24;
            bih.biCompression = BI_RGB;
            if (FILE* pFile = fopen(szOutPath, "wb"))
            {
                fwrite(&bfh, 1, sizeof(bfh), pFile);
                fwrite(&bih, 1, sizeof(bih), pFile);
                fwrite(vBmpRows.data(), 1, dwImgSize, pFile);
                fclose(pFile);
                return;
            }
        }
    }

    RECT rcClient = {};
    if (!GetClientRect(hWnd, &rcClient)) return;
    const int32_t nW = (rcClient.right - rcClient.left) > 1 ? (rcClient.right - rcClient.left) : 1;
    const int32_t nH = (rcClient.bottom - rcClient.top) > 1 ? (rcClient.bottom - rcClient.top) : 1;
    HDC hWndDc = GetDC(hWnd);
    if (!hWndDc) return;
    HDC hMemDc = CreateCompatibleDC(hWndDc);
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = nW;
    bmi.bmiHeader.biHeight = nH;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* pBits = nullptr;
    HBITMAP hBmp = CreateDIBSection(hWndDc, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
    if (hMemDc && hBmp && pBits)
    {
        HGDIOBJ hOld = SelectObject(hMemDc, hBmp);
        BitBlt(hMemDc, 0, 0, nW, nH, hWndDc, 0, 0, SRCCOPY);
        SelectObject(hMemDc, hOld);
        const uint32_t dwRowStride = ((static_cast<uint32_t>(nW) * 3u + 3u) & ~3u);
        const uint32_t dwImgSize = dwRowStride * static_cast<uint32_t>(nH);
        BITMAPFILEHEADER bfh = {};
        bfh.bfType = 0x4D42;
        bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        bfh.bfSize = bfh.bfOffBits + dwImgSize;
        if (FILE* pFile = fopen(szOutPath, "wb"))
        {
            fwrite(&bfh, 1, sizeof(bfh), pFile);
            fwrite(&bmi.bmiHeader, 1, sizeof(bmi.bmiHeader), pFile);
            fwrite(pBits, 1, dwImgSize, pFile);
            fclose(pFile);
        }
    }
    if (hBmp) DeleteObject(hBmp);
    if (hMemDc) DeleteDC(hMemDc);
    ReleaseDC(hWnd, hWndDc);
}

static void Autotest_SimulateClick(int32_t x800, int32_t y600)
{
    HWND hWnd = WINDOW_GetWindow();
    if (!hWnd) return;
    RECT rcClient = {};
    int32_t physX = x800, physY = y600;
    if (GetClientRect(hWnd, &rcClient) && rcClient.right > 0 && rcClient.bottom > 0)
    {
        physX = (x800 * rcClient.right) / 800;
        physY = (y600 * rcClient.bottom) / 600;
    }
    const LPARAM lParam = MAKELPARAM(physX, physY);
    SendMessageA(hWnd, WM_MOUSEMOVE, 0, lParam);
    SendMessageA(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
    SendMessageA(hWnd, WM_LBUTTONUP, 0, lParam);
}

static void Autotest_WriteReportJson(const char* szOutDir, uint32_t dwVendorShopItems)
{
    int32_t nHashUnits = 0, nActiveNpcs = 0, nActiveMonsters = 0;
    for (int32_t t = 0; t < UNIT_TYPES_COUNT; ++t)
    {
        for (int32_t b = 0; b < 128; ++b)
        {
            for (D2UnitStrc* pU = gapServerSideUnitHashTables[t][b]; pU != nullptr; pU = pU->pListNext)
            {
                ++nHashUnits;
                if (t == UNIT_MONSTER)
                {
                    D2MonStatsTxt* pMonTxt = DATATBLS_GetMonStatsTxtRecord(pU->dwClassId);
                    if (pMonTxt && (pMonTxt->dwMonStatsFlags & ((1u << 9) | (1u << 8) | (1u << 24))) != 0) ++nActiveNpcs;
                    else ++nActiveMonsters;
                }
            }
        }
    }

    int32_t nEquippedItems = 0, nInventoryItems = 0, nBeltItems = 0;
    if (gpPlayerUnit && gpPlayerUnit->pInventory)
    {
        for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(gpPlayerUnit->pInventory); pItem != nullptr; pItem = INVENTORY_GetNextItem(pItem))
        {
            if (!INVENTORY_UnitIsItem(pItem)) continue;
            if (pItem->dwAnimMode == IMODE_EQUIP) ++nEquippedItems;
            else if (pItem->dwAnimMode == IMODE_INBELT) ++nBeltItems;
            else if (pItem->dwAnimMode == IMODE_STORED) ++nInventoryItems;
        }
    }

    int32_t nClientTownRooms = 0, nClientMoorRooms = 0;
    if (gpClientAct)
    {
        for (D2ActiveRoomStrc* pR = DUNGEON_GetRoomFromAct(gpClientAct); pR != nullptr; pR = pR->pRoomNext)
        {
            const int32_t nLvl = DUNGEON_GetLevelIdFromRoom(pR);
            if (nLvl == LEVEL_ROGUEENCAMPMENT) ++nClientTownRooms;
            else if (nLvl == LEVEL_BLOODMOOR) ++nClientMoorRooms;
        }
    }

    const int32_t nPlrX = gpPlayerUnit ? UNITS_GetXPosition(gpPlayerUnit) : 0;
    const int32_t nPlrY = gpPlayerUnit ? UNITS_GetYPosition(gpPlayerUnit) : 0;
    const int32_t nCurLvl = (gpPlayerUnit && UNITS_GetRoom(gpPlayerUnit)) ? DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(gpPlayerUnit)) : 0;
    const int32_t nCharLvl = gpPlayerUnit ? STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_LEVEL, 0) : 1;
    const int32_t nStatStr = gpPlayerUnit ? STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_STRENGTH, 0) : 0;
    const int32_t nStatDex = gpPlayerUnit ? STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_DEXTERITY, 0) : 0;
    const int32_t nStatVit = gpPlayerUnit ? STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_VITALITY, 0) : 0;
    const int32_t nStatEne = gpPlayerUnit ? STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_ENERGY, 0) : 0;
    const int32_t nStatHp  = gpPlayerUnit ? (STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_HITPOINTS, 0) >> 8) : 0;
    const int32_t nStatMp  = gpPlayerUnit ? (STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_MANA, 0) >> 8) : 0;
    const int32_t nGoldBank = gpPlayerUnit ? STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_GOLDBANK, 0) : 0;

    uint32_t dwBytesRecv = 0, dwPktsRecv = 0, dwSentBytes = 0, dwSentPkts = 0;
    MSG_GetTrafficCounters(&dwBytesRecv, &dwPktsRecv, &dwSentBytes, &dwSentPkts);

    bool bZeroOriginalDlls = true;
    int32_t nD2MooModules = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 me = {};
        me.dwSize = sizeof(me);
        if (Module32First(hSnap, &me))
        {
            do
            {
                if (strstr(me.szExePath, "Original112a") != nullptr || strstr(me.szExePath, "targets\\diablo-ii-1.12a") != nullptr)
                {
                    bZeroOriginalDlls = false;
                }
                if (_stricmp(me.szModule, "D2Client.dll") == 0 || _stricmp(me.szModule, "D2Common.dll") == 0 ||
                    _stricmp(me.szModule, "D2Game.dll") == 0 || _stricmp(me.szModule, "D2Win.dll") == 0 ||
                    _stricmp(me.szModule, "D2gfx.dll") == 0 || _stricmp(me.szModule, "D2Gdi.dll") == 0 ||
                    _stricmp(me.szModule, "D2Glide.dll") == 0 || _stricmp(me.szModule, "D2CMP.dll") == 0 ||
                    _stricmp(me.szModule, "D2Lang.dll") == 0 || _stricmp(me.szModule, "D2Net.dll") == 0 ||
                    _stricmp(me.szModule, "D2sound.dll") == 0 || _stricmp(me.szModule, "Fog.dll") == 0 ||
                    _stricmp(me.szModule, "Storm.dll") == 0 || _stricmp(me.szModule, "D2Launch.dll") == 0)
                {
                    ++nD2MooModules;
                }
            } while (Module32Next(hSnap, &me));
        }
        CloseHandle(hSnap);
    }

    char szJsonPath[MAX_PATH * 2] = {};
    snprintf(szJsonPath, sizeof(szJsonPath), "%s\\native_autotest_report.json", szOutDir);
    if (FILE* pJson = fopen(szJsonPath, "wb"))
    {
        fprintf(
            pJson,
            "{\n"
            "  \"charName\": \"%s\",\n"
            "  \"charClass\": %u,\n"
            "  \"charLevel\": %d,\n"
            "  \"clientTicks\": %u,\n"
            "  \"c2sPacketsSentViaMsgCpp\": %u,\n"
            "  \"c2sBytesSentViaMsgCpp\": %u,\n"
            "  \"playerGuid\": %u,\n"
            "  \"reconstructedLocalPlayerUnitLinked\": %s,\n"
            "  \"reconstructedUnitHashTableCount\": %d,\n"
            "  \"playerSubX\": %d,\n"
            "  \"playerSubY\": %d,\n"
            "  \"currentLevelId\": %d,\n"
            "  \"clientTownRoomsLoaded\": %d,\n"
            "  \"clientBloodMoorRoomsLoaded\": %d,\n"
            "  \"activeNpcs\": %d,\n"
            "  \"activeMonsters\": %d,\n"
            "  \"equippedItems\": %d,\n"
            "  \"inventoryItems\": %d,\n"
            "  \"beltItems\": %d,\n"
            "  \"vendorShopItems\": %u,\n"
            "  \"stashGold\": %d,\n"
            "  \"statStrength\": %d,\n"
            "  \"statDexterity\": %d,\n"
            "  \"statVitality\": %d,\n"
            "  \"statEnergy\": %d,\n"
            "  \"statHitpoints\": %d,\n"
            "  \"statMana\": %d,\n"
            "  \"serverMapSeed\": %u,\n"
            "  \"zeroOriginalBlizzardDlls\": %s,\n"
            "  \"displayType\": %d,\n"
            "  \"perspectiveEnabled\": %s,\n"
            "  \"d2mooCompiledModulesLoaded\": %d\n"
            "}\n",
            g_szConfigCharName,
            static_cast<unsigned int>(gnLocalPlayerCharClass),
            nCharLvl,
            gdwClientGameTick,
            dwSentPkts,
            dwSentBytes,
            gpPlayerUnit ? gpPlayerUnit->dwUnitId : 0u,
            (gpPlayerUnit != nullptr) ? "true" : "false",
            nHashUnits,
            nPlrX,
            nPlrY,
            nCurLvl,
            nClientTownRooms,
            nClientMoorRooms,
            nActiveNpcs,
            nActiveMonsters,
            nEquippedItems,
            nInventoryItems,
            nBeltItems,
            dwVendorShopItems,
            nGoldBank,
            nStatStr,
            nStatDex,
            nStatVit,
            nStatEne,
            nStatHp,
            nStatMp,
            g_pActSeeds ? g_pActSeeds->dwInitSeed : 0u,
            bZeroOriginalDlls ? "true" : "false",
            static_cast<int32_t>(D2GFX_GetDisplayType()),
            D2GFX_CheckPerspective() ? "true" : "false",
            nD2MooModules);
        fclose(pJson);
    }

    char szExpectedAutoChar[32] = {};
    if (GetEnvironmentVariableA("D2MOO_AUTO_CHAR", szExpectedAutoChar, sizeof(szExpectedAutoChar)) > 0 &&
        szExpectedAutoChar[0] != '\0' &&
        _stricmp(g_szConfigCharName, szExpectedAutoChar) != 0)
    {
        ExitProcess(2);
    }
}

void __fastcall D2Client_RunNativeAutotestTick(void)
{
    static int32_t s_nCheckedEnv = 0;
    static bool s_bEnabled = false;
    static char s_szOutDir[MAX_PATH] = {};
    static int32_t s_nPhase = 0;
    static uint32_t s_dwPhaseTick = 0;
    static uint32_t s_dwStepSentTick = 0;
    static uint32_t s_dwVendorGuid = 0;
    static uint32_t s_dwVendorShopItems = 0;
    static std::vector<POINT> s_vPath;
    static size_t s_uPathIdx = 0;
    static int32_t s_nInteractSubStage = 0;
    static POINT s_aMoorRoute[4] = {};
    static bool s_bHasMoorRoute = false;

    if (!s_nCheckedEnv)
    {
        s_nCheckedEnv = 1;
        char szEnv[32] = {};
        const char* szCmdLine = GetCommandLineA();
        if ((GetEnvironmentVariableA("D2MOO_NATIVE_AUTOTEST", szEnv, sizeof(szEnv)) > 0 && szEnv[0] != '0') ||
            (szCmdLine && strstr(szCmdLine, "--autotest-native") != nullptr))
        {
            s_bEnabled = true;
            if (GetEnvironmentVariableA("D2MOO_AUTOTEST_DIR", s_szOutDir, sizeof(s_szOutDir)) == 0 || s_szOutDir[0] == '\0')
            {
                GetModuleFileNameA(nullptr, s_szOutDir, sizeof(s_szOutDir));
                if (char* pSlash = strrchr(s_szOutDir, '\\')) *pSlash = '\0';
            }
            else
            {
                CreateDirectoryA(s_szOutDir, nullptr);
            }
        }
    }
    if (!s_bEnabled || !gpPlayerUnit) return;

    const uint32_t dwTick = gdwClientGameTick;
    switch (s_nPhase)
    {
    case 0:
        if (dwTick >= 8u && UNITS_GetRoom(gpPlayerUnit) != nullptr)
        {
            gbAlwaysRun = 1;
            Autotest_CaptureBmp(s_szOutDir, "native_step1_hud.bmp");
            s_dwVendorGuid = 0;
            s_vPath.clear();
            s_uPathIdx = 0;
            s_nInteractSubStage = 0;
            if (D2UnitStrc* pVendor = Autotest_FindTownVendor())
            {
                s_dwVendorGuid = pVendor->dwUnitId;
                int32_t gx = UNITS_GetXPosition(pVendor), gy = UNITS_GetYPosition(pVendor);
                Autotest_FindNearestWalkableSubtile(gx, gy, 6, &gx, &gy);
                Autotest_ComputeWalkablePath(UNITS_GetXPosition(gpPlayerUnit), UNITS_GetYPosition(gpPlayerUnit), gx, gy, s_vPath);
            }
            s_dwPhaseTick = dwTick;
            s_dwStepSentTick = 0;
            s_nPhase = 1;
        }
        break;

    case 1:
    {
        D2UnitStrc* pVendor = (s_dwVendorGuid != 0) ? D2Client_FindServerSideUnit(s_dwVendorGuid, UNIT_MONSTER) : Autotest_FindTownVendor();
        if (!pVendor)
        {
            if ((dwTick - s_dwPhaseTick) >= 40u) { s_dwPhaseTick = dwTick; s_nPhase = 2; }
            break;
        }
        if (s_dwVendorGuid == 0)
        {
            s_dwVendorGuid = pVendor->dwUnitId;
            int32_t gx = UNITS_GetXPosition(pVendor), gy = UNITS_GetYPosition(pVendor);
            Autotest_FindNearestWalkableSubtile(gx, gy, 6, &gx, &gy);
            Autotest_ComputeWalkablePath(UNITS_GetXPosition(gpPlayerUnit), UNITS_GetYPosition(gpPlayerUnit), gx, gy, s_vPath);
            s_uPathIdx = 0;
            s_dwStepSentTick = 0;
        }
        if (gaPanelStates[8] != 0 || (dwTick - s_dwPhaseTick) >= 260u)
        {
            D2Client_InteractWithNPC_6FAE30C0(pVendor);
            s_dwPhaseTick = dwTick;
            s_nPhase = 2;
            break;
        }
        if (s_uPathIdx < s_vPath.size())
        {
            const int32_t dx = UNITS_GetXPosition(gpPlayerUnit) - s_vPath[s_uPathIdx].x;
            const int32_t dy = UNITS_GetYPosition(gpPlayerUnit) - s_vPath[s_uPathIdx].y;
            if (s_dwStepSentTick == 0)
            {
                Autotest_StreamNeighborhoodAroundTile(s_vPath[s_uPathIdx].x / 5, s_vPath[s_uPathIdx].y / 5, 24);
                MSG_ResetLastSentPacket();
                D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(gpPlayerUnit, s_vPath[s_uPathIdx].x, static_cast<uint32_t>(s_vPath[s_uPathIdx].y), 3, TRUE);
                s_dwStepSentTick = dwTick;
            }
            else if ((dx * dx + dy * dy) <= 25 || (dwTick - s_dwStepSentTick) >= 14u)
            {
                ++s_uPathIdx;
                s_dwStepSentTick = 0;
            }
        }
        else if (s_nInteractSubStage == 0)
        {
            MSG_ResetLastSentPacket();
            D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(gpPlayerUnit, UNIT_MONSTER, s_dwVendorGuid, 4, TRUE);
            s_dwStepSentTick = dwTick;
            s_nInteractSubStage = 1;
        }
        else
        {
            const int32_t vdx = UNITS_GetXPosition(gpPlayerUnit) - UNITS_GetXPosition(pVendor);
            const int32_t vdy = UNITS_GetYPosition(gpPlayerUnit) - UNITS_GetYPosition(pVendor);
            if (((vdx * vdx + vdy * vdy) <= 36 && (dwTick - s_dwStepSentTick) >= 4u) || (dwTick - s_dwStepSentTick) >= 14u)
            {
                MSG_ResetLastSentPacket();
                D2Client_InteractWithNPC_6FAE30C0(pVendor);
                s_dwPhaseTick = dwTick;
                s_nPhase = 2;
            }
        }
        break;
    }

    case 2:
        if ((dwTick - s_dwPhaseTick) >= 4u)
        {
            UI_SetPanelState(8, 0, 0);
            Autotest_CaptureBmp(s_szOutDir, "native_step2_npc_dialog.bmp");
            D2UnitStrc* pVendor = (s_dwVendorGuid != 0) ? D2Client_FindServerSideUnit(s_dwVendorGuid, UNIT_MONSTER) : nullptr;
            const uint32_t dwMenuAction = (pVendor && pVendor->dwClassId == 147) ? 2u : 1u;
            if (s_dwVendorGuid != 0)
            {
                MSG_ResetLastSentPacket();
                MSG_SendByte3Dwords(0x38u, dwMenuAction, s_dwVendorGuid, 0u);
            }
            UI_SetPanelState(8, 1, 0);
            g_nInvPanelMode = (dwMenuAction == 2u) ? 2 : 1;
            UI_SetPanelState(12, 0, 0);
            s_dwPhaseTick = dwTick;
            s_nPhase = 3;
        }
        break;

    case 3:
    {
        D2UnitStrc* pVendor = g_pTradeNpcUnit ? g_pTradeNpcUnit : D2Client_FindServerSideUnit(s_dwVendorGuid, UNIT_MONSTER);
        uint32_t dwShopCount = 0;
        if (pVendor && pVendor->pInventory)
        {
            for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pVendor->pInventory); pItem != nullptr; pItem = INVENTORY_GetNextItem(pItem))
            {
                if (INVENTORY_UnitIsItem(pItem)) ++dwShopCount;
            }
        }
        if ((dwShopCount >= 10u && (dwTick - s_dwPhaseTick) >= 4u) || (dwTick - s_dwPhaseTick) >= 75u)
        {
            s_dwVendorShopItems = dwShopCount;
            Autotest_SimulateClick(120, 102);
            Autotest_CaptureBmp(s_szOutDir, "native_step3_vendor_trade.bmp");
            if (s_dwVendorGuid != 0)
            {
                MSG_SendByte2Dwords(0x30u, UNIT_MONSTER, s_dwVendorGuid);
            }
            g_bInteractingWithNpc = 0;
            g_nInteractingNpcGUID = 0;
            g_nInteractingNpcClassId = 0;
            gdwActiveInteractNpcGUID = 0;
            g_pTradeNpcUnit = nullptr;
            UI_SetPanelState(12, 1, 0);
            UI_CloseOpenPanelsCount(0);
            s_dwPhaseTick = dwTick;
            s_nPhase = 4;
        }
        break;
    }

    case 4:
        if ((dwTick - s_dwPhaseTick) >= 2u)
        {
            UI_SetPanelState(2, 0, 0);
            UI_SetPanelState(4, 0, 0);
            Autotest_CaptureBmp(s_szOutDir, "native_step4_char_skill_inv.bmp");
            s_dwPhaseTick = dwTick;
            s_nPhase = 5;
        }
        break;

    case 5:
        if ((dwTick - s_dwPhaseTick) >= 2u)
        {
            UI_SetPanelState(4, 1, 0);
            UI_SetPanelState(2, 1, 0);
            QUESTLOG_TogglePanel(0);
            UI_SetPanelState(15, 0, 0);
            UI_SetPanelState(1, 1, 0);
            UI_SetPanelState(10, 0, 0);
            AUTOMAP_CenterView(1);
            Autotest_CaptureBmp(s_szOutDir, "native_step5_quest_automap.bmp");
            s_dwPhaseTick = dwTick;
            s_nPhase = 6;
        }
        break;

    case 6:
        if ((dwTick - s_dwPhaseTick) >= 2u)
        {
            UI_SetPanelState(10, 1, 0);
            QUESTLOG_ClosePanel(0);
            UI_SetPanelState(15, 1, 0);
            g_nInvPanelMode = 11;
            UI_SetPanelState(25, 0, 0);
            UI_SetPanelState(1, 0, 0);
            UI_SetPanelState(31, 0, 0);
            Autotest_CaptureBmp(s_szOutDir, "native_step6_stash_belt.bmp");
            s_dwPhaseTick = dwTick;
            s_nPhase = 7;
        }
        break;

    case 7:
        if ((dwTick - s_dwPhaseTick) >= 2u)
        {
            UI_SetPanelState(31, 1, 0);
            UI_SetPanelState(25, 1, 0);
            g_nInvPanelMode = 12;
            UI_SetPanelState(26, 0, 0);
            UI_SetPanelState(1, 0, 0);
            Autotest_SimulateClick(248, 326);
            Autotest_CaptureBmp(s_szOutDir, "native_step7_horadric_cube.bmp");
            s_dwPhaseTick = dwTick;
            s_nPhase = 8;
        }
        break;

    case 8:
        if ((dwTick - s_dwPhaseTick) >= 2u)
        {
            UI_SetPanelState(26, 1, 0);
            UI_SetPanelState(16, 0, 0);
            Autotest_CaptureBmp(s_szOutDir, "native_step8_waypoint_menu.bmp");
            s_dwPhaseTick = dwTick;
            s_nPhase = 9;
        }
        break;

    case 9:
        if ((dwTick - s_dwPhaseTick) >= 2u)
        {
            UI_SetPanelState(16, 1, 0);
            UI_SetPanelState(36, 0, 0);
            UI_SetPanelState(1, 0, 0);
            Autotest_CaptureBmp(s_szOutDir, "native_step9_mercenary.bmp");
            UI_CloseOpenPanelsCount(1);
            g_nInvPanelMode = 0;

            extern void __fastcall UI_OpenEscOptionsMenu(int32_t bSaveAndClosePanels, int32_t nMenuPage);
            extern void __stdcall UI_CloseEscOptionsMenu(int32_t bRestorePanels);
            UI_OpenEscOptionsMenu(1, 0);
            Autotest_CaptureBmp(s_szOutDir, "native_step9b_esc_options.bmp");
            UI_CloseEscOptionsMenu(0);

            s_vPath.clear();
            s_uPathIdx = 0;
            s_dwStepSentTick = 0;
            s_bHasMoorRoute = Autotest_ComputeBloodMoorRoute(s_aMoorRoute);
            if (s_bHasMoorRoute)
            {
                int32_t curX = UNITS_GetXPosition(gpPlayerUnit);
                int32_t curY = UNITS_GetYPosition(gpPlayerUnit);
                for (int32_t leg = 0; leg < 4; ++leg)
                {
                    std::vector<POINT> vLeg;
                    Autotest_ComputeWalkablePath(curX, curY, s_aMoorRoute[leg].x, s_aMoorRoute[leg].y, vLeg);
                    for (size_t k = 0; k < vLeg.size(); ++k)
                    {
                        s_vPath.push_back(vLeg[k]);
                    }
                    if (!vLeg.empty())
                    {
                        curX = vLeg.back().x;
                        curY = vLeg.back().y;
                    }
                    else
                    {
                        curX = s_aMoorRoute[leg].x;
                        curY = s_aMoorRoute[leg].y;
                    }
                }
            }
            s_dwPhaseTick = dwTick;
            s_nPhase = 10;
        }
        break;

    case 10:
        if (s_bHasMoorRoute && !s_vPath.empty())
        {
            const int32_t nCurLvl = UNITS_GetRoom(gpPlayerUnit) ? DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(gpPlayerUnit)) : 0;
            if (s_uPathIdx < s_vPath.size())
            {
                const int32_t dx = UNITS_GetXPosition(gpPlayerUnit) - s_vPath[s_uPathIdx].x;
                const int32_t dy = UNITS_GetYPosition(gpPlayerUnit) - s_vPath[s_uPathIdx].y;
                if (s_dwStepSentTick == 0)
                {
                    Autotest_StreamNeighborhoodAroundTile(s_vPath[s_uPathIdx].x / 5, s_vPath[s_uPathIdx].y / 5, 24);
                    MSG_ResetLastSentPacket();
                    D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(gpPlayerUnit, s_vPath[s_uPathIdx].x, static_cast<uint32_t>(s_vPath[s_uPathIdx].y), 3, TRUE);
                    s_dwStepSentTick = dwTick;
                }
                else if ((dx * dx + dy * dy) <= 25 || (dwTick - s_dwStepSentTick) >= 14u)
                {
                    ++s_uPathIdx;
                    s_dwStepSentTick = 0;
                }
            }
            else
            {
                if (nCurLvl == LEVEL_BLOODMOOR || dwTick >= 850u)
                {
                    s_dwPhaseTick = dwTick;
                    s_nPhase = 11;
                    break;
                }
                Autotest_ComputeWalkablePath(UNITS_GetXPosition(gpPlayerUnit), UNITS_GetYPosition(gpPlayerUnit), s_aMoorRoute[3].x, s_aMoorRoute[3].y, s_vPath);
                s_uPathIdx = 0;
                s_dwStepSentTick = 0;
            }
            if ((nCurLvl == LEVEL_BLOODMOOR && s_uPathIdx + 1 >= s_vPath.size()) || dwTick >= 850u)
            {
                s_dwPhaseTick = dwTick;
                s_nPhase = 11;
            }
        }
        else
        {
            s_dwPhaseTick = dwTick;
            s_nPhase = 11;
        }
        break;

    case 11:
        if ((dwTick - s_dwPhaseTick) >= 5u)
        {
            Autotest_CaptureBmp(s_szOutDir, "native_step10_blood_moor.bmp");
            Autotest_WriteReportJson(s_szOutDir, s_dwVendorShopItems);
            s_nPhase = 12;
            MSG_SendLeaveGame_69();
            g_bExitAppFlag = 1;
            g_bExitGameFlag = 1;
        }
        break;

    default:
        break;
    }
}
