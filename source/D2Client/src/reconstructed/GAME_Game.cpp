// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Translation Unit: GAME_Game.cpp
// Covers:
//   - ..\Source\D2Client\CORE\WINMAIN.CPP (0x6FAB1880..0x6FAC26E9)
//   - ..\Source\D2Client\GAME\Game.cpp    (0x6FB29CD0..0x6FB32E00)
//   - ..\Source\D2Client\GAME\View.cpp    (0x6FB6A3D0..0x6FB6B7F5 Screen Shake & Viewport)
// ImageBase: 0x6FAB0000
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <stdio.h>

// ----------------------------------------------------------------------------
// Forward Declarations & External Types
// ----------------------------------------------------------------------------
struct D2UnitStrc;
struct D2RoomStrc;
struct D2ActStrc;
struct D2InventoryStrc;
struct D2SkillStrc;
struct D2SkillsTxt;
struct D2SkillDescTxt;
struct D2MonStatsTxt;
struct D2MonStats2Txt;
struct D2MissilesTxt;
struct D2CharStatsTxt;
struct D2BeltsTxt;
struct D2ItemsTxt;
struct D2CellFileStrc;
struct D2GfxDataStrc;
struct D2GameInfoStrc;
struct D2ViewStrc;
struct SMsg;

// ----------------------------------------------------------------------------
// Global Variables (1.12a D2Client.dll .data / .bss addresses)
// ----------------------------------------------------------------------------
extern int32_t  gnScreenWidth;                            // 0x6FB8C6E0
extern int32_t  gnScreenHeight;                           // 0x6FB8C6E4
int32_t         gnViewportHeight            = 440;        // 0x6FBCD524
int32_t         gnViewportWidth             = 800;        // 0x6FBCD528
extern int32_t  gnOpenPanelMode;                          // 0x6FBCC1D0
int32_t         gnViewportShiftOffsetX      = 0;          // 0x6FBCC1D4
int32_t         g_nPanelOffsetX             = 0;          // 0x6FBCBD28
int32_t         g_nPanelOffsetY             = 0;          // 0x6FBCBD2C
#define g_nScreenWidth       gnScreenWidth
#define g_nScreenHeight      gnScreenHeight
#define g_nViewportHeight    gnViewportHeight
#define g_nViewportWidth     gnViewportWidth
#define g_nViewportShiftMode gnOpenPanelMode
#define g_nViewportOffsetX   gnViewportShiftOffsetX

// Client State Machine Globals (CORE/WINMAIN.CPP)
static int32_t  g_nClientState               = 3;       // 0x6FBA8B30
static int32_t  (*g_apfnClientStateHandlers[7])(void);  // 0x6FBA8B34
static int32_t  g_bExitAppFlag               = 0;       // 0x6FBA8B70
static D2GameInfoStrc* g_pGameData           = nullptr; // 0x6FBCB908
static HMODULE  g_hD2ClientModule            = nullptr; // 0x6FBCC428
static int32_t  g_nClientExitStatus          = 0;       // 0x6FBCC42C
static int32_t  g_bDataTablesLoaded          = 0;       // 0x6FBCC430
static int32_t  g_bClientInitialized         = 0;       // 0x6FBCC434
static int32_t  g_nClientStateStackDepth     = 0;       // 0x6FBCC438
static int32_t  g_anClientStateStack[5]      = { 0 };   // 0x6FBCB8F4
static uint32_t g_dwGameExitTargetTick       = 0;       // 0x6FBCC420

// In-Game Loop & Network Globals (GAME/Game.cpp)
extern D2UnitStrc* gpPlayerUnit;                        // 0x6FBCC3D0
extern struct D2DrlgActStrc* gpClientAct;               // 0x6FBCC01C
#define g_pPlayerUnit gpPlayerUnit
#define g_pClientAct  (*reinterpret_cast<D2ActStrc**>(&gpClientAct))
static D2ViewStrc* g_pClientView             = nullptr; // 0x6FBCC028
static void*    g_pClientPoolSystem          = nullptr; // 0x6FBCC000
static int32_t  g_bExitGameFlag              = 0;       // 0x6FBCC008
static int32_t  g_nGameConnectionType        = 0;       // 0x6FBCC034
static int32_t  g_bStopServerThread          = 0;       // 0x6FBCC038
static int32_t  g_bServerThreadActive        = 0;       // 0x6FBCC03C
static HANDLE   g_hOpenServerThread          = nullptr; // 0x6FBB32DC
static DWORD    g_dwOpenServerThreadId       = 0;       // 0x6FBB32D0
static CRITICAL_SECTION g_csServerThread;               // 0x6FBB327C

extern uint8_t  gnClientDifficulty;                     // 0x6FBCBFF4
extern int32_t  gnGameType;                             // 0x6FBCBFF8
extern int32_t  gbIsExpansion;                          // 0x6FBB330C
#define g_nDifficulty      gnClientDifficulty
#define g_nGameType        gnGameType
#define g_bIsExpansionGame gbIsExpansion
static int32_t  g_bIsHardcoreGame            = 0;       // 0x6FBB3308
static int32_t  g_bNoSoundCmdLine            = 0;       // 0x6FBCBFF0
static int32_t  g_bBuildColorTable           = 0;       // 0x6FBCBFEC

static void     (*g_pfnRenderFrame)(int32_t) = nullptr; // 0x6FBB329C
static uint32_t g_dwLastFrameTick            = 0;       // 0x6FBB32AC
uint32_t        gdwClientGameTick            = 0;       // 0x6FBB32B0
#define g_dwClientFrameCount gdwClientGameTick
static int32_t  g_nFrameTickAccumulator      = 0;       // 0x6FBB32B4
static int32_t  g_nTargetFrameTickMs         = 40;      // 0x6FB8FFA8 (25 Hz)
uint32_t        gdwClientPing                = 0;       // 0x6FBB32BC
#define g_dwPingMs gdwClientPing
static uint32_t g_dwPingSampleSum            = 0;       // 0x6FBB32E8
static uint32_t g_dwPingSampleCount          = 0;       // 0x6FBB32EC
static uint32_t g_dwLastPingSentTick         = 0;       // 0x6FBB32E4
static uint32_t g_dwLastPacketRecvTick       = 0;       // 0x6FBB32C4
uint32_t        gdwReassignGraceTick         = 0;       // 0x6FBB32D4
#define g_dwActLoadGraceTick gdwReassignGraceTick
static uint32_t g_dwDisconnectScreenTick     = 0;       // 0x6FBB32D8
static int32_t  g_nDisconnectErrorStringIdx  = -1;      // 0x6FB90358
static int32_t  g_bServerPaused              = 0;       // 0x6FBCC02C
static int32_t  g_bClientPaused              = 0;       // 0x6FBCC030
static int32_t  g_bMinimizedInactive         = 0;       // 0x6FBCC00C

// Screen Shake Globals (GAME/View.cpp)
int32_t         g_nShakeMaxAmplitude         = 0;       // 0x6FBCB9B0
#define g_nShakeAmplitude g_nShakeMaxAmplitude
int32_t         g_nShakeOffsetX              = 0;       // 0x6FBCB9B4
int32_t         g_nShakeOffsetY              = 0;       // 0x6FBAD114
uint32_t        g_dwShakeStartTick           = 0;       // 0x6FBAD104
int32_t         g_nShakeRampUpMs             = 0;       // 0x6FBAD108
int32_t         g_nShakeHoldMs               = 0;       // 0x6FBAD10C
int32_t         g_nShakeRampDownMs           = 0;       // 0x6FBAD110
#define g_dwShakeAttackMs  (*reinterpret_cast<uint32_t*>(&g_nShakeRampUpMs))
#define g_dwShakeSustainMs (*reinterpret_cast<uint32_t*>(&g_nShakeHoldMs))
#define g_dwShakeReleaseMs (*reinterpret_cast<uint32_t*>(&g_nShakeRampDownMs))
static int32_t  g_nEarthquakeSoundHandle     = 0;       // 0x6FBB0220

// Mouse Hold / Repeat State
int32_t         g_bLeftMouseHeld             = 0;       // 0x6FBCC040
int32_t         g_bLeftMouseRepeating        = 0;       // 0x6FBCC044
static int32_t  g_nLeftMouseHoldX            = 0;       // 0x6FBCC048
static int32_t  g_nLeftMouseHoldY            = 0;       // 0x6FBCC04C
static uint32_t g_dwLeftMouseDownTick        = 0;       // 0x6FBCC058
int32_t         g_bRightMouseHeld            = 0;       // 0x6FBCC05C
int32_t         g_bRightMouseRepeating       = 0;       // 0x6FBCC060
static int32_t  g_nRightMouseHoldX           = 0;       // 0x6FBCC064
static int32_t  g_nRightMouseHoldY           = 0;       // 0x6FBCC068
static uint32_t g_dwRightMouseDownTick       = 0;       // 0x6FBCC074

// ============================================================================
// PART 1: Engine.h & DataTable Inline Helpers (0x6FAB1880..0x6FAB1D40)
// ============================================================================

// 1.12a: 0x6FAB1880 (__fastcall)
D2InventoryStrc* __fastcall UNITS_GetClientInventory(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return nullptr;
    }
    return *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60);
}

// 1.12a: 0x6FAB18D0 (__fastcall)
int32_t __fastcall ENGINE_SafeMulDiv(int32_t nValue, int32_t nMul, int32_t nDiv)
{
    if (nDiv <= 0)
    {
        return 0;
    }
    if (nValue <= 0x100000)
    {
        if (nMul <= 0x10000)
        {
            return (nValue * nMul) / nDiv;
        }
        if (nDiv >= (nMul >> 4))
        {
            return nValue / (nDiv / nMul);
        }
    }
    return (int32_t)(((int64_t)nValue * (int64_t)nMul) / (int64_t)nDiv);
}

// 1.12a: 0x6FAB1A00 (__fastcall)
D2RoomStrc* __fastcall ENGINE_FindRoomBySubtileCoords(D2RoomStrc* pStartRoom, int32_t nX, int32_t nY)
{
    if (!pStartRoom)
    {
        return nullptr;
    }
    // Checks pStartRoom bounds via DUNGEON_GetRoomCoordinates (#10085),
    // then searches adjacent rooms via DUNGEON_GetAdjacentRoomsListFromRoom (#10685)
    // and falls back to DUNGEON_FindRoomBySubtileCoordinates (#10486).
    return pStartRoom;
}

// ============================================================================
// PART 2: CORE/WINMAIN.CPP — Client State Machine & Window Management
// (0x6FAC1FE0..0x6FAC26E9)
// ============================================================================

// 1.12a: 0x6FAC1FE0 (__stdcall)
int32_t __stdcall WINMAIN_GetExitAppFlag()
{
    return g_bExitAppFlag;
}

// 1.12a: 0x6FAC1FF0 (__fastcall)
void __fastcall WINMAIN_SetExitAppFlag(int32_t bExit)
{
    g_bExitAppFlag = bExit;
}

// 1.12a: 0x6FAC2000 (__stdcall)
void __stdcall WINMAIN_RequestQuitClientState()
{
    g_nClientState = 1;
}

// 1.12a: 0x6FAC2010 (__stdcall)
D2GameInfoStrc* __stdcall WINMAIN_GetGameData()
{
    return g_pGameData;
}

// 1.12a: 0x6FAC2020 (__stdcall)
void* __stdcall WINMAIN_GetBnetCallbacks()
{
    return g_pGameData ? *(void**)((uint8_t*)g_pGameData + 0x21F) : nullptr;
}

// 1.12a: 0x6FAC2030 (__stdcall)
int32_t __stdcall WINMAIN_IsExpansionCharacter()
{
    if (!g_pGameData)
    {
        return 0;
    }
    return ((*(uint16_t*)((uint8_t*)g_pGameData + 0x1EA) >> 5) & 1);
}

// 1.12a: 0x6FAC2060 (__stdcall)
int32_t __stdcall WINMAIN_IsHardcoreCharacter()
{
    if (!g_pGameData)
    {
        return 0;
    }
    return ((*(uint16_t*)((uint8_t*)g_pGameData + 0x1EA) >> 2) & 1);
}

// 1.12a: 0x6FAC2080 (__stdcall)
uint8_t __stdcall WINMAIN_GetCharacterDifficultyFromFlags()
{
    if (!g_pGameData)
    {
        return 0;
    }
    uint8_t nDiff = (uint8_t)((*(uint16_t*)((uint8_t*)g_pGameData + 0x1EA) >> 8) & 0x0F);
    return (nDiff >= 3) ? 0 : nDiff;
}

// 1.12a: 0x6FAC20B0 (__stdcall)
const char* __stdcall WINMAIN_GetGamePassword()
{
    if (!g_pGameData || !*((char*)g_pGameData + 0x257))
    {
        return nullptr;
    }
    return (const char*)g_pGameData + 0x257;
}

// 1.12a: 0x6FAC20E0 (__stdcall)
const char* __stdcall WINMAIN_GetGameName()
{
    return g_pGameData ? ((const char*)g_pGameData + 0x23F) : nullptr;
}

// 1.12a: 0x6FAC20F0 (__stdcall)
const char* __stdcall WINMAIN_GetAccountName()
{
    return g_pGameData ? ((const char*)g_pGameData + 0xB8) : nullptr;
}

// 1.12a: 0x6FAC2100 (__stdcall)
void __stdcall WINMAIN_RestoreApplicationWindow()
{
    // Sends WM_ACTIVATEAPP (0x1C, wParam=1) to WINDOW_GetWindow() and calls WINDOW_ShowAll()
}

// 1.12a: 0x6FAC2120 (usercall: eax = szCmdLine)
int32_t WINMAIN_SaveDefaultCommandLine(const char* szCmdLine)
{
    // SRegSaveString("Diablo II", "Default Command Line", 0, szCmdLine);
    return 1;
}

// 1.12a: 0x6FAC2140 (__stdcall)
void __stdcall WINMAIN_PreTouchMemoryPages()
{
    // Allocates 96 MB (0x6000000 bytes) via FOG_Alloc, touches one byte per 4 KB page
    // to warm the working set, then frees via FOG_Free.
}

// 1.12a: 0x6FAC2200 (__stdcall)
int32_t __stdcall WINMAIN_PopClientState()
{
    if (g_nClientStateStackDepth <= 0)
    {
        return 1;
    }
    --g_nClientStateStackDepth;
    return g_anClientStateStack[g_nClientStateStackDepth];
}

// 1.12a: 0x6FAC2260 (__fastcall)
void __fastcall WINMAIN_PushClientState(int32_t nState)
{
    if (g_nClientStateStackDepth < 5)
    {
        g_anClientStateStack[g_nClientStateStackDepth++] = nState;
    }
}

// 1.12a: 0x6FAC23A0 (__stdcall)
void __stdcall WINMAIN_ProcessCommandLine()
{
    // Registers command-line argument table via SCmdRegisterArgList,
    // loads "Default Command Line" from registry if -skiptobnet is not set,
    // invokes SCmdProcess, and initializes player character name and connection type.
}

// 1.12a: 0x6FAC24C0 (__stdcall)
// Main Client Entry Point & State Machine (Exported via D2Client Interface)
int32_t __stdcall D2Client_Main(D2GameInfoStrc* pGameData, HMODULE hModule)
{
    g_pGameData = pGameData;
    g_hD2ClientModule = hModule;

    // Initialize FOG memory pool mode (1 if < 110 MB physical RAM, else 2)
    // FOG_SetMemoryPoolSystemMode(...);
    WINMAIN_ProcessCommandLine();

    if (pGameData && *(int32_t*)pGameData != 0)
    {
        g_bClientInitialized = 1;
        g_bDataTablesLoaded  = 1;
    }

    if (!g_bClientInitialized)
    {
        // D2Win_10015_InitSoundAndArchives(SOUND_StopAll);
        // D2Win_10014_SetWndProc(WALL_WndProc /*0x6FB1D200*/);
        // STRTABLE_Initialize(...)
        g_bClientInitialized = 1;
    }

    if (!g_bDataTablesLoaded)
    {
        // DATATBLS_LoadAllTxts(0, 0, 0);
        g_bDataTablesLoaded = 1;
    }

    g_nClientState = 3;
    while (g_nClientState != 1)
    {
        int32_t (*pfnStateHandler)(void) = g_apfnClientStateHandlers[g_nClientState];
        if (!pfnStateHandler)
        {
            break;
        }
        g_nClientState = pfnStateHandler();
        if (g_nClientState == 1)
        {
            break;
        }
    }

    if (g_nClientExitStatus == 2)
    {
        // D2Win_10158_CloseArchives();
    }

    if (!g_pGameData || g_nClientExitStatus != 2)
    {
        // DATATBLS_UnloadAllBins();
        g_bDataTablesLoaded = 0;
        // STRTABLE_Destroy();
    }

    int32_t nResult = g_nClientExitStatus;
    g_nClientExitStatus = 0;
    return nResult;
}

// 1.12a: 0x6FAC2630 (usercall: ebx = nResolutionMode)
int32_t GAME_SetResolutionMode(int32_t nResolutionMode)
{
    int32_t nNewWidth  = (nResolutionMode != 0) ? 800 : 640;
    int32_t nNewHeight = (nResolutionMode != 0) ? 600 : 480;

    if (nNewWidth == g_nScreenWidth && nNewHeight == g_nScreenHeight)
    {
        return 1;
    }

    int32_t nDisplayMode = (nResolutionMode == 2) ? 2 : nResolutionMode;
    // if (!D2Win_10005_SetResolutionMode(nDisplayMode)) return 0;

    // D2GFX_GetModeParams(&g_nScreenWidth, &g_nScreenHeight);
    g_nScreenWidth  = nNewWidth;
    g_nScreenHeight = nNewHeight;
    g_nViewportWidth  = g_nScreenWidth;
    g_nViewportHeight = g_nScreenHeight - 40;

    // LIGHTMAP_Resize(g_nScreenWidth, g_nScreenHeight); // 0x6FB0DFF0
    // UI_UpdatePanelResolution();                       // 0x6FB3F800
    return 1;
}

// 1.12a: 0x6FAC26D0 (usercall: eax = nDelayMs)
void GAME_SetExitAppTimer(uint32_t nDelayMs)
{
    // g_dwGameExitTargetTick = FOG_GetSyncTime() + nDelayMs;
    (void)nDelayMs;
}

// ============================================================================
// PART 3: GAME/View.cpp — Screen Shake & Viewport Management
// (0x6FB6A3D0..0x6FB6AA00 & 0x6FB6B250..0x6FB6B7F5)
// ============================================================================

// 1.12a: 0x6FB6A3D0 (__stdcall)
void __stdcall VIEW_ResetScreenShake()
{
    g_nShakeAmplitude = 0;
    g_nShakeOffsetX   = 0;
    g_nShakeOffsetY   = 0;
}

// 1.12a: 0x6FB6A3F0 (__fastcall)
int32_t __fastcall VIEW_GetScreenShakeOffsets(int32_t* pOutOffsetX, int32_t* pOutOffsetY)
{
    if (!g_nShakeAmplitude)
    {
        return 0;
    }
    *pOutOffsetX = g_nShakeOffsetX;
    *pOutOffsetY = g_nShakeOffsetY;
    return 1;
}

// 1.12a: 0x6FB6A420 (usercall: esi = dwSustainMs, stack = nAmplitude, dwAttackMs, dwReleaseMs)
void VIEW_StartScreenShake(uint32_t dwSustainMs, int32_t nAmplitude, uint32_t dwAttackMs, uint32_t dwReleaseMs)
{
    if (dwSustainMs != 0)
    {
        g_dwShakeStartTick = GetTickCount();
        g_dwShakeAttackMs  = dwAttackMs;
        g_dwShakeSustainMs = dwSustainMs;
        g_dwShakeReleaseMs = dwReleaseMs;
        g_nShakeAmplitude  = nAmplitude;
    }
}

// 1.12a: 0x6FB6B250 (usercall: eax = nShakeIntensity)
void VIEW_UpdateEarthquakeLoopSound(int32_t nShakeIntensity)
{
    int32_t nVolume = (nShakeIntensity * 255) / 20;
    if (nVolume < 0)
    {
        nVolume = 0;
    }
    else if (nVolume > 255)
    {
        nVolume = 255;
    }

    if (nVolume == 0 && g_nEarthquakeSoundHandle != 0)
    {
        // Stop earthquake rumble sound (0x1C4)
        g_nEarthquakeSoundHandle = 0;
    }
}

// 1.12a: 0x6FB6B4F0 (__stdcall)
void __stdcall VIEW_UpdateScreenShake(int32_t* pInOutCameraX, int32_t* pInOutCameraY)
{
    uint32_t dwElapsed = GetTickCount() - g_dwShakeStartTick;
    uint32_t dwTotalDuration = g_dwShakeAttackMs + g_dwShakeSustainMs + g_dwShakeReleaseMs;

    if (dwElapsed > dwTotalDuration)
    {
        VIEW_UpdateEarthquakeLoopSound(0);
        g_nShakeAmplitude = 0;
        g_nShakeOffsetX   = 0;
        g_nShakeOffsetY   = 0;
        return;
    }

    int32_t nCurrentAmp = 0;
    if (dwElapsed < g_dwShakeAttackMs)
    {
        if (g_dwShakeAttackMs != 0)
        {
            nCurrentAmp = (g_nShakeAmplitude * dwElapsed) / g_dwShakeAttackMs;
        }
    }
    else if (dwElapsed < g_dwShakeAttackMs + g_dwShakeSustainMs)
    {
        nCurrentAmp = g_nShakeAmplitude;
    }
    else if (g_dwShakeReleaseMs != 0)
    {
        uint32_t dwRemaining = dwTotalDuration - dwElapsed;
        nCurrentAmp = (g_nShakeAmplitude * dwRemaining) / g_dwShakeReleaseMs;
    }

    if (nCurrentAmp != 0 && g_pPlayerUnit != nullptr)
    {
        // Sample random shake offsets in [-nCurrentAmp, +nCurrentAmp] using player seed (+0x20)
        *pInOutCameraX += g_nShakeOffsetX;
        *pInOutCameraY += g_nShakeOffsetY;
    }

    VIEW_UpdateEarthquakeLoopSound(nCurrentAmp);
}

// ============================================================================
// PART 4: GAME/Game.cpp — In-Game Loop, Server Thread, Packet Drain & Shutdown
// (0x6FB29CD0..0x6FB2D850)
// ============================================================================

// 1.12a: 0x6FB29CD0 (__stdcall)
uint8_t __stdcall GAME_GetDifficulty()
{
    return g_nDifficulty;
}

// 1.12a: 0x6FB29CE0 (__stdcall)
int32_t __stdcall GAME_IsExpansion()
{
    return g_bIsExpansionGame;
}

// 1.12a: 0x6FB29CF0 (__stdcall)
int32_t __stdcall GAME_GetGameType()
{
    return g_nGameType;
}

// 1.12a: 0x6FB29D00 (__stdcall)
void __stdcall GAME_TriggerEndGameSequence()
{
    // Starts end-game fade and sets g_bExitGameFlag
    g_bExitGameFlag = 1;
}

// 1.12a: 0x6FB29D20 (__stdcall)
void __stdcall GAME_RequestExitGame()
{
    // Sends close-game message (0x6FB29760), stops sound, sets g_bExitGameFlag = 1
    g_bExitGameFlag = 1;
}

// 1.12a: 0x6FB29E10 (usercall: edi = nAct)
void GAME_PreloadActTileLibrary(uint8_t nAct)
{
    // If - preload / low-memory mode is not active, preloads the act's .dt1 tile library
    // from g_aszActTileLibraries[nAct] via D2CMP_10087_PreloadTileLibrary.
    (void)nAct;
}

// 1.12a: 0x6FB2A730 (usercall: eax = pPacket)
void GAME_HandlePongPacket(const uint8_t* pPacket)
{
    (void)pPacket;
    uint32_t dwNow = GetTickCount();
    g_dwLastPacketRecvTick = dwNow;

    uint32_t dwRtt = dwNow - g_dwLastPingSentTick;
    int32_t nSubTick = g_nTargetFrameTickMs;
    if ((int32_t)dwRtt > nSubTick)
    {
        dwRtt -= nSubTick;
    }
    else
    {
        dwRtt /= 2;
    }

    g_dwPingSampleSum += dwRtt;
    g_dwPingSampleCount += 1;
    if (g_dwPingSampleCount >= 10)
    {
        g_dwPingMs = g_dwPingSampleSum / g_dwPingSampleCount;
        g_dwPingSampleSum = 0;
        g_dwPingSampleCount = 0;
    }
}

// 1.12a: 0x6FB2AB30 (__fastcall)
void __fastcall GAME_RegisterMouseWheelSupport(HWND hWnd)
{
    (void)hWnd;
    // Registers MSH_WHEELSUPPORT_MSG / MSH_MOUSEWHEEL and WM_MOUSEWHEEL / WM_XBUTTONDOWN handlers
}

// 1.12a: 0x6FB2AC70 (__stdcall)
void __stdcall GAME_UnregisterMouseWheelSupport()
{
    // Unregisters mouse wheel and XButton handlers via SMsgUnregisterMessage
}

// 1.12a: 0x6FB2B060 (__stdcall)
// Open Battle.net / TCP/IP Host Server Thread Procedure
DWORD WINAPI GAME_OpenServerThreadProc(LPVOID lpParameter)
{
    (void)lpParameter;
    EnterCriticalSection(&g_csServerThread);

    if (g_bStopServerThread)
    {
        g_bServerThreadActive = 0;
        LeaveCriticalSection(&g_csServerThread);
        DeleteCriticalSection(&g_csServerThread);
        return 0;
    }

    uint32_t dwStartTick = GetTickCount();
    uint32_t dwLastTick  = dwStartTick;
    int32_t  nSavedPlayerCount = 1;

    while (!g_bStopServerThread)
    {
        LeaveCriticalSection(&g_csServerThread);

        // D2Game.dll!#10058 -> GAME_ProcessNetworkMessages();
        uint32_t dwNow = GetTickCount();
        if (dwNow - dwLastTick > 35)
        {
            // D2Game.dll!#10004 -> GAME_UpdateGamesProgress(1);
            dwLastTick = dwNow;
        }

        // D2Game.dll!#10016 -> GAME_UpdateClients(0, 0);

        if ((GetTickCount() - dwStartTick) >= 5000)
        {
            Sleep(30);
        }
        else
        {
            Sleep(10);
        }

        // Update Open Battle.net hosted game player count if changed
        void* pBnetCallbacks = WINMAIN_GetBnetCallbacks();
        if (pBnetCallbacks)
        {
            (void)nSavedPlayerCount;
        }

        EnterCriticalSection(&g_csServerThread);
    }

    // D2Game.dll!#10019 -> GAME_CloseAllGames();
    g_bServerThreadActive = 0;
    LeaveCriticalSection(&g_csServerThread);
    DeleteCriticalSection(&g_csServerThread);
    return 0;
}

// 1.12a: 0x6FB2B1C0 (__stdcall)
void __stdcall GAME_SendPeriodicPing()
{
    uint32_t dwNow = GetTickCount();
    if (dwNow - g_dwLastPingSentTick > 5000)
    {
        // MSG_SendPingPacket(); // 0x6FB299D0 (Packet 0x6D)
        g_dwLastPingSentTick = dwNow;
    }
}

// 1.12a: 0x6FB2B200 (__stdcall)
void __stdcall GAME_RenderDisconnectErrorScreen()
{
    // Renders black screen with centered connection error string from g_awDisconnectErrorStrIds[g_nDisconnectErrorStringIdx]
    if (GetTickCount() - g_dwDisconnectScreenTick > 3000)
    {
        GAME_RequestExitGame();
    }
}

// 1.12a: 0x6FB2BA80 (usercall: eax = nErrorIndex)
void GAME_SetDisconnectErrorScreen(int32_t nErrorIndex)
{
    if (!g_dwDisconnectScreenTick)
    {
        g_dwDisconnectScreenTick = GetTickCount();
        g_pfnRenderFrame = (void(*)(int32_t))GAME_RenderDisconnectErrorScreen;
        g_nDisconnectErrorStringIdx = nErrorIndex;
    }
}

// 1.12a: 0x6FB2BAF0 (__stdcall)
void __stdcall GAME_CheckNetworkConnectionHealth()
{
    if (g_nGameConnectionType == 3 || g_nGameConnectionType == 6 || g_nGameConnectionType == 7)
    {
        // Checks D2Net_10025_IsConnected() and packet timeout (30s normal, 180s during act load grace period)
        uint32_t dwNow = GetTickCount();
        uint32_t dwTimeout = (dwNow - g_dwActLoadGraceTick >= 10000) ? 30000 : 180000;
        if (dwNow - g_dwLastPacketRecvTick > dwTimeout)
        {
            GAME_SetDisconnectErrorScreen(3);
        }
    }
}

// 1.12a: 0x6FB2BC90 (__fastcall)
void __fastcall GAME_SetServerPause(int32_t bPaused)
{
    g_bServerPaused = bPaused;
}

// 1.12a: 0x6FB2BCA0 (__stdcall)
int32_t __stdcall GAME_IsPaused()
{
    return (g_bServerPaused || g_bClientPaused) ? 1 : 0;
}

// 1.12a: 0x6FB2BCB0 (__stdcall)
// Network Packet Drain (System Packets 0xAF..0xB4 & Game Server Packets)
void __stdcall GAME_ReceiveNetworkPackets()
{
    uint8_t aPacketBuffer[524];

    // 1. Drain system packets from D2Net.dll!#10004 (CLIENT_DequeueSystemPacket)
    //    Handles 0xAF (connect/version), 0xB0 (disconnect), 0xB1 (pause/resume),
    //    0xB2 (save status), 0xB3 (save chunk), 0xB4 (timeout/error).
    (void)aPacketBuffer;

    // 2. Drain game packets from D2Net.dll!#10005 (CLIENT_DequeueGamePacket)
    //    For each packet: updates g_dwLastPacketRecvTick = GetTickCount() and
    //    dispatches to SCMD_ProcessGamePacket(aPacketBuffer, nPacketSize) (0x6FB150D0).

    GAME_CheckNetworkConnectionHealth();
}

// 1.12a: 0x6FB2C320 (usercall: eax = nAct, edx = dwTownLevelId, ecx = dwInitSeed, stack = dwEndSeed)
void GAME_LoadAct(uint8_t nAct, uint32_t dwTownLevelId, uint32_t dwInitSeed, uint32_t dwEndSeed)
{
    (void)dwTownLevelId;
    (void)dwInitSeed;
    (void)dwEndSeed;

    GAME_PreloadActTileLibrary(nAct);

    if (g_pClientAct)
    {
        // D2Common.dll!#10878 -> DUNGEON_FreeAct(g_pClientAct);
        g_pClientAct = nullptr;
    }

    // g_pClientAct = DUNGEON_AllocAct(nAct, dwInitSeed, 1, 0, g_nDifficulty, g_pClientPoolSystem, dwTownLevelId, CLIENT_LoadActCallback, ClientAutomapRoomCallback);
    // ENVIRONMENT_InitializeEnvironment(g_pClientAct, g_pClientPoolSystem, nAct, 0);
    // GFX_FreeMonsterCofCache();  // 0x6FB40CF0
    // GFX_InitMonsterCofCache();  // 0x6FB411D0

    g_dwActLoadGraceTick = GetTickCount();
}

// 1.12a: 0x6FB2C450 (__stdcall)
void __stdcall GAME_InitializeNewActRooms()
{
    if (!g_pClientAct)
    {
        return;
    }
    // Iterates DUNGEON_GetRoomFromAct(g_pClientAct), initializing client room data (0x6FAC7320)
    // and room units (0x6FAD1430) for any room where room flags bit 17 (0x20000) is not yet set.
}

// 1.12a: 0x6FB2C4A0 (__stdcall)
// Full In-Game State Teardown
void __stdcall GAME_FreeClientState()
{
    // Unhook window/UI callbacks, free roster/party/pet lists, quests, automap, missiles, sounds
    if (g_pClientAct)
    {
        // DUNGEON_FreeAct(g_pClientAct);
        g_pClientAct = nullptr;
    }

    // D2CMP_10012_SpriteFreeAsyncLoads();
    // D2CMP_10008_TileFreeAsyncLoads();
    // D2CMP_10063_DestroyTileCache();
    // D2CMP_10075_FlushSpriteCache(1);

    // Free player & monster COF caches (0x6FB40D40, 0x6FB40CF0)
    // Free client view (VIEW_FreeView(g_pClientView), 0x6FB6A560)
    g_pClientView = nullptr;

    // Destroy lightmap (LIGHTMAP_Destroy(), 0x6FB0E180)
    // Free client unit hash tables (0x6FBCA960, 0x6FBC9D60)

    if (g_pClientPoolSystem)
    {
        // FOG_DestroyMemoryPoolSystem(g_pClientPoolSystem);
        g_pClientPoolSystem = nullptr;
    }
}

// 1.12a: 0x6FB2CA10 (__fastcall)
// Main In-Game Frame Renderer (registered at g_pfnRenderFrame = 0x6FBB329C)
// 1.12a: 0x6FB6AE30 (__fastcall, defined in View.cpp)
void __fastcall GAME_RenderFrame(int32_t bSkipWorldRender);

// 1.12a: 0x6FB2CBB0 (__stdcall)
// Full In-Game State Initialization
void __stdcall GAME_InitClientState()
{
    // FOG_CreateNewPoolSystem(&g_pClientPoolSystem, "Client Pool System", 0, 0);
    // Initialize lightmap (LIGHTMAP_Alloc, 0x6FB0E140), wall fade tables (0x6FB6B020), view (0x6FB6A5B0)
    // Load "Resolution" from registry and call GAME_SetResolutionMode
    // Initialize palette shift tables (PALSHIFT_Init, 0x6FB6F170; D2CMP_PalShiftInit)
    // Clear unit hash tables (0x6FBCA960, 0x6FBC9D60) and initialize UI subsystems
    g_pfnRenderFrame    = reinterpret_cast<decltype(g_pfnRenderFrame)>(GAME_RenderFrame);
    g_dwLastFrameTick   = GetTickCount();
    g_dwClientFrameCount = 0;
    g_nFrameTickAccumulator = 0;
}

// 1.12a: 0x6FB2CE10 (__stdcall)
// 25 Hz Client Logic Progress Tick
void __stdcall GAME_UpdateProgress()
{
    if (!g_pClientAct || !g_pPlayerUnit)
    {
        return;
    }

    // 1. Initialize any newly streamed rooms in the act
    GAME_InitializeNewActRooms();

    if (!GAME_IsPaused())
    {
        // 2. Animate room tiles (DUNGEON_AnimateTiles)
        // 3. Async-precache nearby monster/unit sprites (0x6FB44DD0)
        // 4. Advance environment time-of-day (ENVIRONMENT_UpdatePeriodOfDay)
        // 5. Update client units, missiles, and particles
        // 6. Update DRLG activation (DRLGACTIVATE_Update)
        g_dwClientFrameCount += 1;

        if ((g_dwClientFrameCount % 13) == 0)
        {
            // DUNGEON_UpdateAndFreeInactiveRooms(g_pClientAct);
        }
    }
}

// 1.12a: 0x6FB2CEE0 (contains 0x6FB2CFF0 Warden & Tick Loop) (__fastcall)
// Per-Frame In-Game Loop Callback (invoked by message pump 0x6FB1CF40)
int32_t __fastcall GAME_FrameLoop(int32_t bProcessLogic)
{
    if (g_bMinimizedInactive)
    {
        Sleep(10);
    }

    // 1. Process input recording / playback (0x6FB26B70)
    // 2. Process held left/right mouse button repeat actions (0x6FB2C810, 0x6FB2C640)
    // 3. Send periodic keepalive ping (0x6FB299D0)

    // 0x6FB2CFF0: Warden Client Tick & 25 Hz Frame Accumulator
    // WARDEN_Update(); // 0x6FB1CBD0

    uint32_t dwNow = GetTickCount();
    uint32_t dwDeltaMs = dwNow - g_dwLastFrameTick;
    g_dwLastFrameTick = dwNow;

    if (dwDeltaMs > 500)
    {
        dwDeltaMs = 40;
    }
    g_nFrameTickAccumulator += (int32_t)dwDeltaMs;

    int32_t bSteppedLogic = 0;
    while (g_nFrameTickAccumulator >= g_nTargetFrameTickMs)
    {
        g_nFrameTickAccumulator -= g_nTargetFrameTickMs;
        bSteppedLogic = 1;

        // If single-player or local server host, step D2Game server logic
        if (g_nGameConnectionType == 1 || g_nGameConnectionType == 2 || g_nGameConnectionType == 5)
        {
            // D2Game.dll!#10058 -> GAME_ProcessNetworkMessages();
            // D2Game.dll!#10004 -> GAME_UpdateGamesProgress(0);
            // D2Game.dll!#10016 -> GAME_UpdateClients(0, 0);
        }

        // Drain incoming network packets
        GAME_ReceiveNetworkPackets();

        // Advance client world simulation by 1 tick (40 ms)
        if (bProcessLogic)
        {
            GAME_UpdateProgress();
        }
    }

    if (!bSteppedLogic)
    {
        GAME_ReceiveNetworkPackets();
    }

    // Render current frame
    if (g_pfnRenderFrame)
    {
        g_pfnRenderFrame(0);
    }

    // Update sound engine (SOUND_Update, 0x6FB1A660)

    if (g_bExitGameFlag)
    {
        return 0;
    }
    return 1;
}

// 1.12a: 0x6FB2D2D0 (__stdcall)
// In-Game Client State Handler (State 4 Entry Point)
int32_t __stdcall GAME_MainLoop()
{
    g_bExitGameFlag = 0;

    // 1. Push Storm window message registration state (SMsgPushRegisterState)
    //    and register in-game WM_* / key / mouse handlers from 0x6FB90360.
    // 2. Establish local or remote connection (0x6FB294E0 / 0x6FB29920).
    // 3. If hosting Open Battle.net / TCP/IP game (connection type 6 or 8),
    //    initialize g_csServerThread and spawn GAME_OpenServerThreadProc (0x6FB2B060).
    if (g_nGameConnectionType == 6 || g_nGameConnectionType == 8)
    {
        InitializeCriticalSection(&g_csServerThread);
        g_bStopServerThread   = 0;
        g_bServerThreadActive = 1;
        g_hOpenServerThread   = CreateThread(nullptr, 0, GAME_OpenServerThreadProc, nullptr, 0, &g_dwOpenServerThreadId);
    }

    // 4. Initialize client state, pools, lightmap, and UI
    GAME_InitClientState();

    // 5. Run main in-game message pump & frame loop (0x6FB1CF40 -> GAME_FrameLoop 0x6FB2CEE0)
    while (GAME_FrameLoop(1))
    {
        // Pump Windows messages via WALL_MessagePump (0x6FB1CF40)
        break;
    }

    // 6. Stop Open server thread if active
    if (g_hOpenServerThread)
    {
        EnterCriticalSection(&g_csServerThread);
        g_bStopServerThread = 1;
        LeaveCriticalSection(&g_csServerThread);
        WaitForSingleObject(g_hOpenServerThread, INFINITE);
        CloseHandle(g_hOpenServerThread);
        g_hOpenServerThread = nullptr;
    }

    // 7. Tear down client state and pop Storm message handlers
    GAME_FreeClientState();

    return 6;
}

// ============================================================================
// PART 5: SoundChannel Mixer (0x6FB2D860..0x6FB2E60F) &
//         SkillTree / ControlPanel UI (0x6FB2E610..0x6FB32E00)
// ============================================================================

// 1.12a: 0x6FB2DD80 (usercall: eax = nUnitType, ecx = dwUnitId, stack = nSoundId, bLoop, dwLoopDuration)
int32_t SOUNDCHANNEL_PlayPositionalUnitSound(int32_t nUnitType, uint32_t dwUnitId, int32_t nSoundId, int32_t bLoop, uint32_t dwLoopDuration)
{
    // Resolves unit coordinates relative to g_pPlayerUnit, computes stereo pan and distance volume
    // attenuation, and starts or updates one of the 16 active sound channels (0x6FBB3078..0x6FBB3278).
    (void)nUnitType;
    (void)dwUnitId;
    (void)nSoundId;
    (void)bLoop;
    (void)dwLoopDuration;
    return 0;
}

// 1.12a: 0x6FB2FB70 (__stdcall)
void __stdcall SKILLTREE_RenderPanel()
{
    // Renders the active character class skill tree tab (0x6FB8FE44), skill icons (0x6FB2F470),
    // remaining skill points counter, and close button.
}

// 1.12a: 0x6FB319D0 (__stdcall)
void __stdcall CTRLPNL_RenderLifeManaTooltips()
{
    // Renders "Life: X / Y" (string 0x1044) and "Mana: X / Y" framed text tooltips when hovering over orbs.
}

// 1.12a: 0x6FB31BB0 (__stdcall)
void __stdcall CTRLPNL_RenderRightManaOrb()
{
    // Computes player mana fill height (STAT_MANA / STAT_MAXMANA * 80px), checks Energy Shield state (0x6A),
    // and draws the mana orb liquid and overlap glass via TEXTURE_CelDrawEx / TEXTURE_CelDraw.
}

// 1.12a: 0x6FB31CF0 (__stdcall)
void __stdcall CTRLPNL_RenderLeftLifeOrb()
{
    // Computes player life fill height (STAT_HITPOINTS / STAT_MAXHP * 80px), checks Poison state (0x02),
    // and draws the health orb liquid and overlap glass via TEXTURE_CelDrawEx / TEXTURE_CelDraw.
}

// 1.12a: 0x6FB32230 (__stdcall)
void __stdcall CTRLPNL_RenderBottomBar()
{
    // Draws either 640x480 ('Panel\CtrlPnl7') or 800x600 ('Panel\800CtrlPnl7' + 'Panel\800BorderFrame')
    // bottom control panel frames, stamina bar, belt slots, mini-panel toggle, and experience bar.
}
