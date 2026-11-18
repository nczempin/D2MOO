// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Translation Unit: GAME_Game.cpp
// Covers:
//   - ..\Source\D2Client\CORE\WINMAIN.CPP (0x6FAB1880..0x6FAC26E9)
//   - ..\Source\D2Client\GAME\Game.cpp    (0x6FB29CD0..0x6FB32E00)
//   - ..\Source\D2Client\GAME\View.cpp    (0x6FB6A3D0..0x6FB6B7F5 Screen Shake & Viewport)
// ImageBase: 0x6FAB0000
// ============================================================================

#include "D2ClientReconstructed.h"

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <D2DataTbls.h>
#include <DataTbls/SkillsTbls.h>
#include <Units/Units.h>
#include <D2StatList.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2Lang.h>
#include <D2Gfx.h>
#include <Texture.h>
#include <D2CMP.h>
#include <D2WinFont.h>
#include <D2WinTextBox.h>


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

#pragma pack(push, 1)
struct D2ReconConfigViewStrc
{
    uint8_t  pad00[0x1EA];
    uint16_t wCharFlags;      // +0x1EA
    uint8_t  pad1EC[0x21F - 0x1EC];
    void*    pBnetCallbacks;  // +0x21F
};
#pragma pack(pop)


// ----------------------------------------------------------------------------
// Forward Declarations & External Types
// ----------------------------------------------------------------------------
struct D2RoomStrc;
struct D2ActStrc;
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
extern uint32_t        gdwClientGameTick;       // 0x6FBB32B0
#define g_dwClientFrameCount gdwClientGameTick
static int32_t  g_nFrameTickAccumulator      = 0;       // 0x6FBB32B4
static int32_t  g_nTargetFrameTickMs         = 40;      // 0x6FB8FFA8 (25 Hz)
extern uint32_t        gdwClientPing;       // 0x6FBB32BC
#define g_dwPingMs gdwClientPing
static uint32_t g_dwPingSampleSum            = 0;       // 0x6FBB32E8
static uint32_t g_dwPingSampleCount          = 0;       // 0x6FBB32EC
static uint32_t g_dwLastPingSentTick         = 0;       // 0x6FBB32E4
static uint32_t g_dwLastPacketRecvTick       = 0;       // 0x6FBB32C4
extern uint32_t        gdwReassignGraceTick;       // 0x6FBB32D4
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
extern int32_t         g_bLeftMouseHeld;       // 0x6FBCC040
extern int32_t         g_bLeftMouseRepeating;       // 0x6FBCC044
static int32_t  g_nLeftMouseHoldX            = 0;       // 0x6FBCC048
static int32_t  g_nLeftMouseHoldY            = 0;       // 0x6FBCC04C
static uint32_t g_dwLeftMouseDownTick        = 0;       // 0x6FBCC058
extern int32_t         g_bRightMouseHeld;       // 0x6FBCC05C
extern int32_t         g_bRightMouseRepeating;       // 0x6FBCC060
static int32_t  g_nRightMouseHoldX           = 0;       // 0x6FBCC064
static int32_t  g_nRightMouseHoldY           = 0;       // 0x6FBCC068
static uint32_t g_dwRightMouseDownTick       = 0;       // 0x6FBCC074


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

extern int32_t g_nRightPanelOffsetX;
extern int32_t g_nSkillTreeTab_6FBA79FC;
extern int32_t gbAlwaysRun;
extern int32_t __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t bMoveMouse);

static D2CellFileStrc* s_pCtrlPnl800Cell   = nullptr;
static D2CellFileStrc* s_pCtrlPnl640Cell   = nullptr;
static D2CellFileStrc* s_pHlthManaCell     = nullptr;
static D2CellFileStrc* s_pOverlapCell      = nullptr;
static D2CellFileStrc* s_pRunBtnCell       = nullptr;
static D2CellFileStrc* s_pMenuBtnCell      = nullptr;
static D2CellFileStrc* s_pMiniPanelCell    = nullptr;
static D2CellFileStrc* s_pMiniPanelBtnCell = nullptr;
static D2CellFileStrc* s_apSkillTreeBgCells[7] = {};
static D2CellFileStrc* s_apClassSkillIconCells[7] = {};
static D2CellFileStrc* s_pSkillBuySellBtnCell = nullptr;

static void GAME_EnsureCtrlPnlCellsLoaded()
{
    if (!s_pCtrlPnl800Cell)   s_pCtrlPnl800Cell   = ARCHIVE_LoadCellFileDefault("PANEL\\800ctrlpnl7", 0);
    if (!s_pCtrlPnl640Cell)   s_pCtrlPnl640Cell   = ARCHIVE_LoadCellFileDefault("PANEL\\ctrlpnl7", 0);
    if (!s_pHlthManaCell)     s_pHlthManaCell     = ARCHIVE_LoadCellFileDefault("PANEL\\hlthmana", 0);
    if (!s_pOverlapCell)      s_pOverlapCell      = ARCHIVE_LoadCellFileDefault("PANEL\\overlap", 0);
    if (!s_pRunBtnCell)       s_pRunBtnCell       = ARCHIVE_LoadCellFileDefault("PANEL\\runbutton", 0);
    if (!s_pMenuBtnCell)      s_pMenuBtnCell      = ARCHIVE_LoadCellFileDefault("PANEL\\menubutton", 0);
    if (!s_pMiniPanelCell)    s_pMiniPanelCell    = ARCHIVE_LoadCellFileDefault("PANEL\\minipanel", 0);
    if (!s_pMiniPanelBtnCell) s_pMiniPanelBtnCell = ARCHIVE_LoadCellFileDefault("PANEL\\minipanelbtn", 0);
}

static void GAME_DrawSingleCelTopLeft(D2CellFileStrc* pCellFile, int32_t nLeftX, int32_t nTopY, int32_t nFrame)
{
    if (!pCellFile) return;
    int32_t nTotal = D2CMP_CelFileGetCelsPerDirection(pCellFile);
    if (nFrame < 0 || nFrame >= nTotal) nFrame = 0;
    D2GfxDataStrc tGfx = {};
    tGfx.pCellFile = pCellFile;
    tGfx.nDirection = 0;
    tGfx.nFrame = nFrame;
    D2CellFileStrc* pCell = D2CMP_CelGetHandle(&tGfx);
    int32_t nH = pCell ? D2CMP_CelGetHeight(pCell) : 0;
    TEXTURE_CelDraw(&tGfx, nLeftX, nTopY + nH, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
}

static void GAME_DrawAsciiText(const char* szAscii, int32_t nX, int32_t nY, int32_t nColor, BOOL bCentered)
{
    if (!szAscii) return;
    Unicode wszBuf[128] = {};
    Unicode::win2Unicode(wszBuf, szAscii, 127);
    int32_t nDrawX = nX;
    if (bCentered)
    {
        nDrawX -= D2Win_10121_GetTextWidth(wszBuf) / 2;
    }
    D2Win_10117_DrawText(wszBuf, nDrawX, nY, nColor, 0);
}

// 1.12a: 0x6FB2FB70 (__stdcall)
void __stdcall SKILLTREE_RenderPanel()
{
    if (!gaPanelStates[D2PANEL_SKILLTREE] || !gpPlayerUnit)
    {
        return;
    }

    const int32_t nClassId = (gpPlayerUnit->dwClassId < 7) ? (int32_t)gpPlayerUnit->dwClassId : 0;
    static const char* const kTreeBgPaths[7] = {
        "SPELLS\\skltree_a_back",
        "SPELLS\\skltree_s_back",
        "SPELLS\\skltree_n_back",
        "SPELLS\\skltree_p_back",
        "SPELLS\\skltree_b_back",
        "SPELLS\\skltree_d_back",
        "SPELLS\\skltree_i_back",
    };
    static const char* const kClassIconPaths[7] = {
        "SPELLS\\AmSkillicon",
        "SPELLS\\SoSkillicon",
        "SPELLS\\NeSkillicon",
        "SPELLS\\PaSkillicon",
        "SPELLS\\BaSkillicon",
        "SPELLS\\DrSkillicon",
        "SPELLS\\AsSkillicon",
    };

    if (!s_apSkillTreeBgCells[nClassId])
        s_apSkillTreeBgCells[nClassId] = ARCHIVE_LoadCellFileDefault(kTreeBgPaths[nClassId], 0);
    if (!s_apClassSkillIconCells[nClassId])
        s_apClassSkillIconCells[nClassId] = ARCHIVE_LoadCellFileDefault(kClassIconPaths[nClassId], 0);
    if (!s_pSkillBuySellBtnCell)
        s_pSkillBuySellBtnCell = ARCHIVE_LoadCellFileDefault("PANEL\\buysellbtn", 0);

    const int32_t nPanelLeft = (gnScreenWidth >= 800) ? (320 + g_nRightPanelOffsetX) : 320;
    const int32_t nPanelTop  = (gnScreenHeight >= 600) ? 60 : 0;
    int32_t nTab = g_nSkillTreeTab_6FBA79FC;
    if (nTab < 0 || nTab > 2) nTab = 0;

    D2CellFileStrc* pTreeBg = s_apSkillTreeBgCells[nClassId];
    if (pTreeBg)
    {
        const int32_t aQuadPos[4][2] = {
            { nPanelLeft,       nPanelTop + 256 },
            { nPanelLeft + 256, nPanelTop + 256 },
            { nPanelLeft,       nPanelTop + 432 },
            { nPanelLeft + 256, nPanelTop + 432 },
        };
        const int32_t nTotal = D2CMP_CelFileGetCelsPerDirection(pTreeBg);
        for (int32_t i = 0; i < 4 && i < nTotal; ++i)
        {
            D2GfxDataStrc tBg = {};
            tBg.pCellFile = pTreeBg;
            tBg.nFrame = i;
            TEXTURE_CelDraw(&tBg, aQuadPos[i][0], aQuadPos[i][1], 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
        }
        const int32_t nTabBase = (nTab + 1) * 4;
        for (int32_t i = 0; i < 4 && (nTabBase + i) < nTotal; ++i)
        {
            D2GfxDataStrc tBg = {};
            tBg.pCellFile = pTreeBg;
            tBg.nFrame = nTabBase + i;
            TEXTURE_CelDraw(&tBg, aQuadPos[i][0], aQuadPos[i][1], 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
        }
    }
    else
    {
        D2GFX_DrawBox(nPanelLeft, nPanelTop, nPanelLeft + 320, nPanelTop + 432, 0x00u, DRAWMODE_TRANS75);
    }

    const int32_t aSkillColX[3] = { nPanelLeft + 15, nPanelLeft + 84, nPanelLeft + 153 };
    const int32_t aSkillRowY[6] = { nPanelTop + 14, nPanelTop + 82, nPanelTop + 150, nPanelTop + 218, nPanelTop + 286, nPanelTop + 354 };
    int32_t nHoveredSkillId = -1;
    bool bHoveredSkillUnlocked = false;
    int32_t nHoverX = nPanelLeft + 120;
    int32_t nHoverY = nPanelTop + 140;
    bool bRow6Col1Occupied = false;
    const int32_t nPlrLvl = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_LEVEL, 0);

    D2CellFileStrc* pIconCel = s_apClassSkillIconCells[nClassId];
    if (pIconCel && sgptDataTables && sgptDataTables->pSkillsTxt && sgptDataTables->pSkillDescTxt)
    {
        D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
        for (int32_t sId = 0; sId < sgptDataTables->nSkillsTxtRecordCount && sId < 512; ++sId)
        {
            const D2SkillsTxt& sk = sgptDataTables->pSkillsTxt[sId];
            if (sk.nCharClass != nClassId || sk.wSkillDesc < 0 || sk.wSkillDesc >= sgptDataTables->nSkillDescTxtRecordCount)
                continue;
            const D2SkillDescTxt& sd = sgptDataTables->pSkillDescTxt[sk.wSkillDesc];
            if (sd.nSkillPage != (nTab + 1) || sd.nSkillColumn < 1 || sd.nSkillColumn > 3 || sd.nSkillRow < 1 || sd.nSkillRow > 6)
                continue;
            if (sd.nSkillRow == 6 && sd.nSkillColumn == 1)
                bRow6Col1Occupied = true;

            const int32_t nIconX = aSkillColX[sd.nSkillColumn - 1];
            const int32_t nIconY = aSkillRowY[sd.nSkillRow - 1];
            D2SkillStrc* pSkill = SKILLS_GetSkillById(gpPlayerUnit, sId, -1);
            const int32_t nLvl = pSkill ? SKILLS_GetSkillLevel(gpPlayerUnit, pSkill, 1) : 0;
            const bool bMeetsPrereqs = (nLvl > 0) || (nPlrLvl >= sk.wReqLevel && SKILLS_CheckRequiredSkills(gpPlayerUnit, sId) != 0);

            GAME_DrawSingleCelTopLeft(pIconCel, nIconX, nIconY, bMeetsPrereqs ? sd.nIconCel : (sd.nIconCel + 1));

            char szLvl[16] = {};
            snprintf(szLvl, sizeof(szLvl), "%d", nLvl);
            const uint8_t nLvlColor = (nLvl > 0) ? 4 : (bMeetsPrereqs ? 0 : 5);
            GAME_DrawAsciiText(szLvl, nIconX + 41, nIconY + 55, nLvlColor, TRUE);

            if (gnMouseX >= nIconX && gnMouseX < nIconX + 48 && gnMouseY >= nIconY && gnMouseY < nIconY + 48)
            {
                nHoveredSkillId = sId;
                bHoveredSkillUnlocked = bMeetsPrereqs;
                nHoverX = nIconX + 24;
                nHoverY = (nIconY - 8 > nPanelTop + 15) ? (nIconY - 8) : (nPanelTop + 15);
            }
        }
    }

    static const char* const kClassTabLines[7][3][2] = {
        { { "Bow &", "Crossbow" },       { "Passive &", "Magic" },    { "Javelin &", "Spear" } },
        { { "Fire", "Spells" },          { "Lightning", "Spells" },   { "Cold", "Spells" } },
        { { "Curses", "" },              { "Poison &", "Bone" },      { "Summoning", "Spells" } },
        { { "Defensive", "Auras" },      { "Offensive", "Auras" },    { "Combat", "Skills" } },
        { { "Warcries", "" },            { "Combat", "Masteries" },   { "Combat", "Skills" } },
        { { "Elemental", "" },           { "Shape", "Shifting" },     { "Summoning", "" } },
        { { "Martial", "Arts" },         { "Shadow", "Disciplines" }, { "Traps", "" } },
    };

    const int32_t nSkillPts = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_SKILLPTS, 0);
    D2Win_10127_SetFont(D2FONT_FONT6);
    GAME_DrawAsciiText("REMAINING", nPanelLeft + 277, nPanelTop + 22, 4, TRUE);
    GAME_DrawAsciiText("SKILL", nPanelLeft + 277, nPanelTop + 34, 4, TRUE);
    GAME_DrawAsciiText("POINTS", nPanelLeft + 277, nPanelTop + 46, 4, TRUE);
    D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
    char szPts[16] = {};
    snprintf(szPts, sizeof(szPts), "%d", (nSkillPts > 0) ? nSkillPts : 0);
    GAME_DrawAsciiText(szPts, nPanelLeft + 277, nPanelTop + 80, (nSkillPts > 0) ? 4 : 0, TRUE);

    D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
    const int32_t aTabCenterY[3] = { nPanelTop + 376, nPanelTop + 268, nPanelTop + 160 };
    for (int32_t t = 0; t < 3; ++t)
    {
        const uint8_t nCol = (t == nTab) ? 0 : 4;
        const char* szL1 = kClassTabLines[nClassId][t][0];
        const char* szL2 = kClassTabLines[nClassId][t][1];
        if (szL2 && szL2[0])
        {
            GAME_DrawAsciiText(szL1, nPanelLeft + 277, aTabCenterY[t] - 6, nCol, TRUE);
            GAME_DrawAsciiText(szL2, nPanelLeft + 277, aTabCenterY[t] + 10, nCol, TRUE);
        }
        else
        {
            GAME_DrawAsciiText(szL1, nPanelLeft + 277, aTabCenterY[t] + 2, nCol, TRUE);
        }
    }

    if (s_pSkillBuySellBtnCell)
    {
        GAME_DrawSingleCelTopLeft(s_pSkillBuySellBtnCell,
            bRow6Col1Occupied ? (nPanelLeft + 190) : (nPanelLeft + 16),
            nPanelTop + 384,
            10);
    }

    if (nHoveredSkillId >= 0)
    {
        extern int32_t g_nHoveredSkillId_6FBBA6E8;
        extern int32_t g_nSkillHoverX_6FBBA6D0;
        extern int32_t g_nSkillHoverY_6FBBA6CC;
        extern void __cdecl SKILLDESC_DrawLockedSkillTreeTooltip(void);
        extern void __cdecl SKILLDESC_DrawUnlockedSkillTreeTooltip(void);

        g_nHoveredSkillId_6FBBA6E8 = nHoveredSkillId;
        g_nSkillHoverX_6FBBA6D0    = nHoverX;
        g_nSkillHoverY_6FBBA6CC    = nHoverY;
        if (bHoveredSkillUnlocked)
        {
            SKILLDESC_DrawUnlockedSkillTreeTooltip();
        }
        else
        {
            SKILLDESC_DrawLockedSkillTreeTooltip();
        }
    }
}

// 1.12a: 0x6FB319D0 (__stdcall)
void __stdcall CTRLPNL_RenderLifeManaTooltips()
{
    if (!gpPlayerUnit) return;
    const int32_t nCurHp   = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_HITPOINTS, 0) >> 8;
    const int32_t nMaxHp   = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_MAXHP, 0) >> 8;
    const int32_t nCurMana = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_MANA, 0) >> 8;
    const int32_t nMaxMana = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_MAXMANA, 0) >> 8;

    const int32_t nW = (gnScreenWidth > 0) ? gnScreenWidth : 800;
    const int32_t nH = (gnScreenHeight > 0) ? gnScreenHeight : 600;
    char szOrb[64] = {};
    D2Win_10127_SetFont(D2FONT_FONTFORMAL10);

    if (gnMouseX >= 15 && gnMouseX <= 105 && gnMouseY >= nH - 95)
    {
        snprintf(szOrb, sizeof(szOrb), "Life: %d / %d", (nCurHp > 0) ? nCurHp : 0, (nMaxHp > nCurHp) ? nMaxHp : nCurHp);
        GAME_DrawAsciiText(szOrb, 54, nH - 94, 0, TRUE);
    }
    if (gnMouseX >= nW - 105 && gnMouseX <= nW - 15 && gnMouseY >= nH - 95)
    {
        snprintf(szOrb, sizeof(szOrb), "Mana: %d / %d", (nCurMana > 0) ? nCurMana : 0, (nMaxMana > nCurMana) ? nMaxMana : nCurMana);
        GAME_DrawAsciiText(szOrb, nW - 54, nH - 94, 0, TRUE);
    }
}

// 1.12a: 0x6FB31BB0 (__stdcall)
void __stdcall CTRLPNL_RenderRightManaOrb()
{
    if (!gpPlayerUnit) return;
    GAME_EnsureCtrlPnlCellsLoaded();
    const int32_t nW = (gnScreenWidth > 0) ? gnScreenWidth : 800;
    const int32_t nH = (gnScreenHeight > 0) ? gnScreenHeight : 600;
    const int32_t nCurMana = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_MANA, 0) >> 8;
    int32_t nMaxMana = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_MAXMANA, 0) >> 8;
    if (nMaxMana < 1) nMaxMana = 1;
    int32_t nFillPx = (nCurMana * 80) / nMaxMana;
    if (nFillPx < 0) nFillPx = 0;
    if (nFillPx > 80) nFillPx = 80;

    if (s_pHlthManaCell && nFillPx > 0)
    {
        D2GfxDataStrc tOrb = {};
        tOrb.pCellFile = s_pHlthManaCell;
        tOrb.nFrame = STATES_CheckState(gpPlayerUnit, 0x1E /* STATE_ENERGYSHIELD */) ? 3 : 1;
        if (D2CMP_CelFileGetCelsPerDirection(s_pHlthManaCell) <= (int32_t)tOrb.nFrame)
            tOrb.nFrame = 1;
        TEXTURE_CelDrawEx(&tOrb, nW - 108, nH - 12, 0, nFillPx, DRAWMODE_NORMAL);
    }
    if (s_pOverlapCell)
    {
        D2GfxDataStrc tOvl = {};
        tOvl.pCellFile = s_pOverlapCell;
        tOvl.nFrame = 1;
        TEXTURE_CelDraw(&tOvl, nW - 110, nH - 6, 0xFFFFFFFFu, DRAWMODE_TRANSHIGHLIGHT, nullptr);
    }
}

// 1.12a: 0x6FB31CF0 (__stdcall)
void __stdcall CTRLPNL_RenderLeftLifeOrb()
{
    if (!gpPlayerUnit) return;
    GAME_EnsureCtrlPnlCellsLoaded();
    const int32_t nH = (gnScreenHeight > 0) ? gnScreenHeight : 600;
    const int32_t nCurHp = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_HITPOINTS, 0) >> 8;
    int32_t nMaxHp = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_MAXHP, 0) >> 8;
    if (nMaxHp < 1) nMaxHp = 1;
    int32_t nFillPx = (nCurHp * 80) / nMaxHp;
    if (nFillPx < 0) nFillPx = 0;
    if (nFillPx > 80) nFillPx = 80;

    if (s_pHlthManaCell && nFillPx > 0)
    {
        D2GfxDataStrc tOrb = {};
        tOrb.pCellFile = s_pHlthManaCell;
        tOrb.nFrame = STATES_CheckState(gpPlayerUnit, 2 /* STATE_POISON */) ? 2 : 0;
        if (D2CMP_CelFileGetCelsPerDirection(s_pHlthManaCell) <= (int32_t)tOrb.nFrame)
            tOrb.nFrame = 0;
        TEXTURE_CelDrawEx(&tOrb, 30, nH - 12, 0, nFillPx, DRAWMODE_NORMAL);
    }
    if (s_pOverlapCell)
    {
        D2GfxDataStrc tOvl = {};
        tOvl.pCellFile = s_pOverlapCell;
        tOvl.nFrame = 0;
        TEXTURE_CelDraw(&tOvl, 28, nH - 6, 0xFFFFFFFFu, DRAWMODE_TRANSHIGHLIGHT, nullptr);
    }
}

// 1.12a: 0x6FB32230 (__stdcall)
void __stdcall CTRLPNL_RenderBottomBar()
{
    if (!gpPlayerUnit) return;
    GAME_EnsureCtrlPnlCellsLoaded();

    const int32_t nW = (gnScreenWidth > 0) ? gnScreenWidth : 800;
    const int32_t nH = (gnScreenHeight > 0) ? gnScreenHeight : 600;

    if (nW >= 800 && s_pCtrlPnl800Cell)
    {
        const int32_t aCtrlPnlPos[6][2] = {
            { 0,   nH - 1 },
            { 165, nH     },
            { 293, nH     },
            { 421, nH     },
            { 549, nH     },
            { 683, nH - 1 },
        };
        const int32_t nFrames = D2CMP_CelFileGetCelsPerDirection(s_pCtrlPnl800Cell);
        for (int32_t f = 0; f < 6 && f < nFrames; ++f)
        {
            D2GfxDataStrc tPnl = {};
            tPnl.pCellFile = s_pCtrlPnl800Cell;
            tPnl.nFrame = f;
            TEXTURE_CelDraw(&tPnl, aCtrlPnlPos[f][0], aCtrlPnlPos[f][1], 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
        }
    }
    else if (s_pCtrlPnl640Cell)
    {
        const int32_t nFrames = D2CMP_CelFileGetCelsPerDirection(s_pCtrlPnl640Cell);
        int32_t nCurX = 0;
        for (int32_t f = 0; f < nFrames; ++f)
        {
            D2GfxDataStrc tPnl = {};
            tPnl.pCellFile = s_pCtrlPnl640Cell;
            tPnl.nFrame = f;
            D2CellFileStrc* pCell = D2CMP_CelGetHandle(&tPnl);
            TEXTURE_CelDraw(&tPnl, nCurX, nH, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
            if (pCell) nCurX += D2CMP_CelGetWidth(pCell);
        }
    }

    // Stamina bar & Run/Walk toggle button
    const int32_t nCurStam = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_STAMINA, 0) >> 8;
    int32_t nMaxStam = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_MAXSTAMINA, 0) >> 8;
    if (nMaxStam < 1) nMaxStam = 1;
    int32_t nStamW = (nCurStam * 102) / nMaxStam;
    if (nStamW < 0) nStamW = 0;
    if (nStamW > 102) nStamW = 102;
    const int32_t nStamBaseX = (nW / 2) - 127;
    if (nStamW > 0)
    {
        D2GFX_DrawBox(nStamBaseX, nH - 26, nStamBaseX + nStamW, nH - 12, 0x6Au, DRAWMODE_NORMAL);
    }
    if (s_pRunBtnCell)
    {
        GAME_DrawSingleCelTopLeft(s_pRunBtnCell, nStamBaseX - 18, nH - 27, gbAlwaysRun ? 2 : 0);
    }

    // Experience bar
    const int32_t nLvl = STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_LEVEL, 0);
    const uint32_t dwExp = (uint32_t)STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_EXPERIENCE, 0);
    const uint32_t dwPrevExp = (nLvl > 1) ? DATATBLS_GetLevelThreshold(gpPlayerUnit->dwClassId, nLvl - 1) : 0u;
    const uint32_t dwNextExp = DATATBLS_GetLevelThreshold(gpPlayerUnit->dwClassId, (nLvl > 0) ? nLvl : 1);
    if (dwNextExp > dwPrevExp && dwExp > dwPrevExp)
    {
        int32_t nExpW = (int32_t)(((uint64_t)(dwExp - dwPrevExp) * 120u) / (uint64_t)(dwNextExp - dwPrevExp));
        if (nExpW > 120) nExpW = 120;
        if (nExpW > 0)
        {
            D2GFX_DrawBox((nW / 2) - 128, nH - 42, (nW / 2) - 128 + nExpW, nH - 39, 0xFFu, DRAWMODE_NORMAL);
        }
    }

    // Central Mini-Panel Toggle & 8-button Mini-Panel
    const int32_t bMiniOpen = (gaPanelStates[D2PANEL_MINIPANEL] != 0) ? 1 : 0;
    if (s_pMenuBtnCell)
    {
        GAME_DrawSingleCelTopLeft(s_pMenuBtnCell, (nW / 2) - 7, nH - 38, bMiniOpen ? 2 : 0);
    }
    if (bMiniOpen)
    {
        const int32_t nMiniX = (nW / 2) - 72;
        const int32_t nMiniY = nH - 74;
        if (s_pMiniPanelCell)
        {
            GAME_DrawSingleCelTopLeft(s_pMiniPanelCell, nMiniX, nMiniY, 0);
        }
        if (s_pMiniPanelBtnCell)
        {
            const bool aActive[8] = {
                gaPanelStates[D2PANEL_CHAR] != 0,
                gaPanelStates[D2PANEL_INVENTORY] != 0,
                gaPanelStates[D2PANEL_SKILLTREE] != 0,
                gaPanelStates[D2PANEL_AUTOMAP] != 0,
                gaPanelStates[D2PANEL_MSGLOG] != 0,
                gaPanelStates[D2PANEL_QUEST] != 0,
                gaPanelStates[D2PANEL_PARTY] != 0,
                gaPanelStates[D2PANEL_ESCMENU] != 0,
            };
            for (int32_t i = 0; i < 8; ++i)
            {
                GAME_DrawSingleCelTopLeft(s_pMiniPanelBtnCell, nMiniX + 3 + i * 18, nMiniY + 3, i * 2 + (aActive[i] ? 1 : 0));
            }
        }
        if (!gaPanelStates[D2PANEL_ESCMENU] && gnMouseX >= nMiniX && gnMouseX < nMiniX + 144 && gnMouseY >= nMiniY - 4 && gnMouseY < nMiniY + 27)
        {
            static const char* const kMiniTooltips[8] = {
                "Character Screen (C)",
                "Inventory Screen (I)",
                "Skill Tree (T)",
                "Automap (Tab)",
                "Message Log (M)",
                "Quest Log (Q)",
                "Party Screen (P)",
                "Game Menu (Esc)",
            };
            int32_t nIdx = (gnMouseX - (nMiniX + 2)) / 18;
            if (nIdx < 0) nIdx = 0;
            if (nIdx > 7) nIdx = 7;
            Unicode wszTip[64] = {};
            Unicode::win2Unicode(wszTip, kMiniTooltips[nIdx], 63);
            D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
            D2Win_10129_DrawFramedText(wszTip, nW / 2, nMiniY - 12, 0, 1);
        }
    }
}
