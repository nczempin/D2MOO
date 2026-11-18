// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Translation Unit: UNIT_Player.cpp
// Combined Sources:
//   - ..\Source\D2Client\UNIT\PlayerList.cpp (asm/UNIT_PlayerList.asm, 0x6FAE03E0..0x6FAE0EB0)
//   - ..\Source\D2Client\UNIT\Player.cpp     (asm/UNIT_Player.asm,     0x6FB076B0..0x6FB0DFF0)
// ImageBase: 0x6FAB0000
// ============================================================================

#include <stdint.h>
#include <windows.h>


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

struct D2ReconModeChangeViewStrc { int32_t nField00; int32_t nMode; };


// ----------------------------------------------------------------------------
// Forward Declarations & Core Engine Structures (from D2Common / D2Client)
// ----------------------------------------------------------------------------
struct D2UnitStrc;
struct D2InventoryStrc;
struct D2SkillStrc;
struct D2SkillListStrc;
struct D2DynamicPathStrc;
struct D2StaticPathStrc;
struct D2RoomStrc;
struct D2RoomCoordListStrc;
struct D2CoordStrc { int32_t nX; int32_t nY; };
struct D2CellFileStrc;
struct D2GfxDataStrc;
struct D2StatListStrc;
struct D2PlayerDataStrc;
struct D2SkillsTxt;
struct D2CharStatsTxt;
struct D2MonStatsTxt;
struct D2AutomapLayerStrc;

// Player relationship list node (12 bytes, allocated in PlayerList.cpp)
struct D2PlayerListNodeStrc
{
    uint32_t dwUnitId;              // +0x00
    uint32_t dwFlags;               // +0x04
    D2PlayerListNodeStrc* pNext;    // +0x08
};

// Player relationship list head (4 bytes, allocated in PlayerList.cpp)
struct D2PlayerListStrc
{
    D2PlayerListNodeStrc* pFirst;   // +0x00
};

// Client roster entry (partial layout for PlayerList.cpp / Player.cpp)
struct D2RosterUnitStrc
{
    char szName[16];                // +0x00
    uint32_t dwUnitId;              // +0x10
    uint32_t dwPartyLife;           // +0x14
    uint32_t dwKillCount;           // +0x18
    uint32_t dwClassId;             // +0x1C
    uint16_t wLevel;                // +0x20
    uint16_t wPartyId;              // +0x22
    uint32_t dwLevelId;             // +0x24
    uint32_t dwPosX;                // +0x28
    uint32_t dwPosY;                // +0x2C
    uint32_t dwPartyFlags;          // +0x30
    D2PlayerListStrc* pPlayerList;  // +0x34
    uint8_t pad38[0x48];            // +0x38
    D2RosterUnitStrc* pNext;        // +0x80
};

// Speedbar skill icon button cell entry (16 bytes, array at 0x6FBCBC90)
struct D2SpeedBarCellStrc
{
    int32_t nSkillId;               // +0x00
    uint8_t bRightHand;             // +0x04
    uint8_t pad05[3];               // +0x05
    uint32_t dwSkillFlags;          // +0x08
    uint32_t dwOwnerGUID;           // +0x0C
};

// ----------------------------------------------------------------------------
// External Globals in D2Client.dll (1.12a)
// ----------------------------------------------------------------------------
extern D2UnitStrc* gpPlayerUnit_6FBCC3D0;              // 0x6FBCC3D0: Local player unit
extern D2UnitStrc* gPlayerUnits_6FBCB6D4[8];           // 0x6FBCB6D4: Active player unit table (8 slots)
extern int32_t gnViewportOffsetX_6FBC21D0;             // 0x6FBC21D0: Viewport X offset
extern int32_t gnViewportOffsetY_6FBC21CC;             // 0x6FBC21CC: Viewport Y offset
extern int32_t gbPerspectiveShakeActive_6FBCBE58;      // 0x6FBCBE58: Screen shake active flag
extern int32_t gnShakeOffsetX_6FBCBDCC;                // 0x6FBCBDCC: Screen shake X delta
extern int32_t gnShakeOffsetY_6FBCC3F0;                // 0x6FBCC3F0: Screen shake Y delta
extern int32_t gnScreenWidth_6FBBA740;                 // 0x6FBBA740: Screen width in pixels
extern int32_t gnScreenHeight_6FBBA744;                // 0x6FBBA744: Screen height in pixels
extern int32_t gbSpeedBarOpen_6FBCBC50;                // 0x6FBCBC50: Speedbar popup open flag
extern int32_t gbSpeedBarRightSide_6FBCBC54;           // 0x6FBCBC54: Speedbar opened for right-hand skill

// ============================================================================
// PART 1: Display / Cache / Performance Helpers (0x6FAE03E0..0x6FAE0C30)
// ============================================================================

static int32_t g_bLightingQuality = 1;
static int32_t g_bBlendedShadows = 1;
static int32_t g_bLowQualityMode = 0;
static int32_t g_nFpsDisplayMode = 0;
static int32_t g_bFpsDisplayActive = 0;
static int32_t g_nPerfCounterA = 0;
static int32_t g_nPerfCounterB = 0;
static int32_t g_nPerfCounterC = 0;
static int32_t g_nPerfCounterD = 0;
static void*   g_pPerfPool = nullptr;
static int32_t g_nSpeedBarHoverIdx = -1;
static int32_t g_bPlayerMouseActionLock = 0;

// 1.12a: 0x6FAE03E0 (__stdcall)
int32_t __stdcall D2Client_GetPerfStat6FBA675C()
{
    return g_bLightingQuality;
}

// 1.12a: 0x6FAE03F0 (__stdcall)
int32_t __stdcall D2Client_GetPerfStat6FBA6760()
{
    return g_bBlendedShadows;
}

// 1.12a: 0x6FAE0400 (__stdcall)
int32_t __stdcall D2Client_GetPerfStat6FBA6764()
{
    return g_bLowQualityMode;
}

// 1.12a: 0x6FAE0410 (custom: eax = nValue)
int32_t D2Client_SetPerfStat6FBA675C(/*eax*/ int32_t nValue)
{
    int32_t nPrev = g_bLightingQuality;
    g_bLightingQuality = nValue;
    return nPrev;
}

// 1.12a: 0x6FAE0420 (custom: eax = nValue)
int32_t D2Client_SetPerfStat6FBA6760(/*eax*/ int32_t nValue)
{
    int32_t nPrev = g_bBlendedShadows;
    g_bBlendedShadows = nValue;
    return nPrev;
}

// 1.12a: 0x6FAE0430 (custom: eax = nValue)
int32_t D2Client_SetPerfStat6FBA6764(/*eax*/ int32_t nValue)
{
    int32_t nPrev = g_bLowQualityMode;
    g_bLowQualityMode = nValue;
    return nPrev;
}

// 1.12a: 0x6FAE0440 (__stdcall)
void __stdcall D2Client_SetWeatherAndLightingQuality(BOOL bEnable)
{
    g_bLowQualityMode = bEnable;
    g_bFpsDisplayActive = bEnable;
    g_bLightingQuality = bEnable;
    // D2Client_SetWeatherEnabled(bEnable); // 0x6FAD1C40
    // D2Gfx_10042(bEnable);
}

// 1.12a: 0x6FAE0470 (__stdcall)
void __stdcall D2Client_ResetFramePerfCounters()
{
    g_nPerfCounterA = 0;
    g_nPerfCounterD = 0;
    g_nPerfCounterC = 0;
    g_nPerfCounterB = 0;
}

// 1.12a: 0x6FAE0490 (custom: ecx = nDeltaA, eax = nDeltaB)
void D2Client_AccumulatePerfCounters(/*ecx*/ int32_t nDeltaA, /*eax*/ int32_t nDeltaB)
{
    g_nPerfCounterB += nDeltaA;
    g_nPerfCounterD += nDeltaB;
}

// 1.12a: 0x6FAE04B0 (__stdcall)
void __stdcall D2Client_ShutdownTileAndSpriteCaches()
{
    // D2CMP_10040(); // DestroyTileCache
    // D2CMP_10099(1); // FlushSpriteCache(TRUE)
    void** ppPool = (void**)g_pPerfPool;
    if (ppPool)
    {
        // FOG_10144(ppPool); // FOG_FreePool
        g_pPerfPool = nullptr;
    }
}

// 1.12a: 0x6FAE04F0 (custom: esi = bReinit)
void D2Client_InitTileAndSpriteCaches(/*esi*/ BOOL bReinit)
{
    // Queries FOG_10219() (system memory info) to size sprite/tile caches,
    // then calls D2CMP_10018 (InitTileCache) and D2CMP_10025 (InitSpriteCache).
}

// 1.12a: 0x6FAE0610 (__stdcall)
void __stdcall D2Client_ResetGfxAndTileCacheStats()
{
    // Resets 68-dword cache results table from D2Gfx_10051() and calls D2CMP_10021().
}

// 1.12a: 0x6FAE06B0 (__stdcall)
void __stdcall D2Client_RenderCacheStatsOverlay()
{
    // Renders tile cache and sprite cache statistics text overlay via D2Win_10117.
}

// 1.12a: 0x6FAE08D0 (__fastcall)
void __fastcall D2Client_SetGfxGamma(int32_t nGamma)
{
    // D2Gfx_10011(nGamma);
}

// 1.12a: 0x6FAE08E0 (__stdcall)
int32_t __stdcall D2Client_TogglePerspectiveMode()
{
    // Toggles perspective mode via D2Gfx_10046 / D2Gfx_10066.
    return 0;
}

// 1.12a: 0x6FAE0910 (__stdcall)
BOOL __stdcall D2Client_ToggleBlendedShadows()
{
    BOOL bNewVal = (g_bBlendedShadows == 0);
    g_bBlendedShadows = bNewVal;
    return bNewVal;
}

// 1.12a: 0x6FAE0930 (__stdcall)
BOOL __stdcall D2Client_ToggleLowQualityMode()
{
    BOOL bNewVal = (g_bLowQualityMode == 0);
    D2Client_SetWeatherAndLightingQuality(bNewVal);
    return bNewVal;
}

// 1.12a: 0x6FAE0990 (custom: edi = nMode)
void D2Client_ToggleFpsDisplayMode(/*edi*/ int32_t nMode)
{
    if (g_bFpsDisplayActive && g_nFpsDisplayMode == nMode)
    {
        g_bFpsDisplayActive = 0;
    }
    else
    {
        g_bFpsDisplayActive = 1;
        g_nFpsDisplayMode = nMode;
    }
}

// 1.12a: 0x6FAE09B0 (__stdcall)
void __stdcall D2Client_UpdatePerfHistoryRingBuffer()
{
    // Updates the 64-entry performance history ring buffer at 0x6FBC94D8 / 0x6FBC95D8 / 0x6FBC96D8.
}

// 1.12a: 0x6FAE09D0 (__stdcall)
void __stdcall D2Client_RenderFpsAndMemoryOverlay()
{
    // Renders FPS / Skip / Ping and GlobalMemoryStatus (Phys / Page / Video) overlay.
    // Uses 7-case switch jump table at 0x6FAE0C14..0x6FAE0C30.
}

// ============================================================================
// PART 2: PlayerList.cpp (0x6FAE0C30..0x6FAE0EB0)
// Source File: ..\Source\D2Client\UNIT\PlayerList.cpp
// ============================================================================

// 1.12a: 0x6FAE0C30 (custom: edi = pPlayerList)
void D2Client_FreePlayerList(/*edi*/ D2PlayerListStrc* pPlayerList)
{
    if (!pPlayerList)
    {
        return;
    }

    D2PlayerListNodeStrc* pNode = pPlayerList->pFirst;
    while (pNode)
    {
        D2PlayerListNodeStrc* pNext = pNode->pNext;
        // FOG_FreeClientMemory(pNode, "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 60, 0);
        pNode = pNext;
    }

    // FOG_FreeClientMemory(pPlayerList, "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 64, 0);
}

// 1.12a: 0x6FAE0C70 (__stdcall)
D2PlayerListStrc* __stdcall D2Client_AllocPlayerList()
{
    // D2PlayerListStrc* pList = (D2PlayerListStrc*)FOG_AllocClientMemory(sizeof(D2PlayerListStrc), "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 37, 0);
    D2PlayerListStrc* pList = nullptr;
    if (!pList)
    {
        // D2Client_FatalError(D2Client_GetLoadErrorString());
        return nullptr;
    }
    pList->pFirst = nullptr;
    return pList;
}

// 1.12a: 0x6FAE0CB0 (custom: eax = dwPlayerUnitId, esi = dwOtherUnitId)
void D2Client_RemovePlayerFromList(/*eax*/ uint32_t dwPlayerUnitId, /*esi*/ uint32_t dwOtherUnitId)
{
    // D2RosterUnitStrc* pRoster = D2Client_FindRosterUnitById(dwPlayerUnitId); // 0x6FB161D0
    D2RosterUnitStrc* pRoster = nullptr;
    if (!pRoster)
    {
        return;
    }

    D2PlayerListStrc* pList = pRoster->pPlayerList;
    if (!pList)
    {
        // FOG_DisplayWarning("pPlayerList", "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 148);
        return;
    }

    D2PlayerListNodeStrc* pPrev = nullptr;
    for (D2PlayerListNodeStrc* pCur = pList->pFirst; pCur != nullptr; pCur = pCur->pNext)
    {
        if (pCur->dwUnitId == dwOtherUnitId)
        {
            if (pPrev)
            {
                pPrev->pNext = pCur->pNext;
            }
            else
            {
                pList->pFirst = pCur->pNext;
            }
            // FOG_FreeClientMemory(pCur, "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 160, 0);
            return;
        }
        pPrev = pCur;
    }
}

// 1.12a: 0x6FAE0D20 (custom: edx = dwPlayerUnitId, esi = dwOtherUnitId)
D2PlayerListNodeStrc* D2Client_FindPlayerListEntry(/*edx*/ uint32_t dwPlayerUnitId, /*esi*/ uint32_t dwOtherUnitId)
{
    // D2RosterUnitStrc* pRoster = D2Client_FindRosterUnitById(dwPlayerUnitId); // 0x6FB161D0
    D2RosterUnitStrc* pRoster = nullptr;
    if (!pRoster || !pRoster->pPlayerList)
    {
        return nullptr;
    }

    for (D2PlayerListNodeStrc* pCur = pRoster->pPlayerList->pFirst; pCur != nullptr; pCur = pCur->pNext)
    {
        if (pCur->dwUnitId == dwOtherUnitId)
        {
            return pCur;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FAE0D50 (custom: ecx = dwPlayerUnitId, eax = dwOtherUnitId, stack arg_0 = dwFlags)
void __stdcall D2Client_SetPlayerListRelationFlags(/*ecx*/ uint32_t dwPlayerUnitId, /*eax*/ uint32_t dwOtherUnitId, uint32_t dwFlags)
{
    D2PlayerListNodeStrc* pEntry = D2Client_FindPlayerListEntry(dwPlayerUnitId, dwOtherUnitId);
    if (pEntry)
    {
        pEntry->dwFlags = dwFlags;
    }
    else
    {
        // D2Client_AddPlayerToList(dwPlayerUnitId, dwOtherUnitId, dwFlags);
    }
}

// 1.12a: 0x6FAE0D90 (custom: eax = dwPlayerUnitId, ecx = dwOtherUnitId)
uint16_t D2Client_GetPlayerListRelationFlags(/*eax*/ uint32_t dwPlayerUnitId, /*ecx*/ uint32_t dwOtherUnitId)
{
    if (dwPlayerUnitId == dwOtherUnitId)
    {
        return 0;
    }
    D2PlayerListNodeStrc* pEntry = D2Client_FindPlayerListEntry(dwPlayerUnitId, dwOtherUnitId);
    if (!pEntry)
    {
        return 0;
    }
    return (uint16_t)pEntry->dwFlags;
}

// 1.12a: 0x6FAE0DD0 (custom: eax = dwPlayerUnitId, ecx = dwOtherUnitId, edi = dwFlagMask)
uint32_t D2Client_TestPlayerListRelationFlag(/*eax*/ uint32_t dwPlayerUnitId, /*ecx*/ uint32_t dwOtherUnitId, /*edi*/ uint32_t dwFlagMask)
{
    if (dwPlayerUnitId == dwOtherUnitId)
    {
        return 0;
    }
    D2PlayerListNodeStrc* pEntry = D2Client_FindPlayerListEntry(dwPlayerUnitId, dwOtherUnitId);
    if (!pEntry)
    {
        return 0;
    }
    return pEntry->dwFlags & dwFlagMask;
}

// 1.12a: 0x6FAE0E10 (custom: eax = dwOtherUnitId, edx = dwPlayerUnitId, edi = dwFlagMask, stack arg_0 = bSet)
void __stdcall D2Client_TogglePlayerListRelationFlag(/*edx*/ uint32_t dwPlayerUnitId, /*eax*/ uint32_t dwOtherUnitId, /*edi*/ uint32_t dwFlagMask, BOOL bSet)
{
    D2PlayerListNodeStrc* pEntry = D2Client_FindPlayerListEntry(dwPlayerUnitId, dwOtherUnitId);
    if (!pEntry)
    {
        if (bSet)
        {
            // D2Client_AddPlayerToList(dwPlayerUnitId, dwOtherUnitId, dwFlagMask);
        }
        return;
    }

    if (bSet)
    {
        pEntry->dwFlags |= dwFlagMask;
    }
    else
    {
        pEntry->dwFlags &= ~dwFlagMask;
    }
}

// 1.12a: 0x6FAE0E60 (custom: eax = dwPlayerUnitId, ebx = dwFlags, stack arg_0 = dwOtherUnitId)
void __stdcall D2Client_AddPlayerToList(/*eax*/ uint32_t dwPlayerUnitId, /*ebx*/ uint32_t dwFlags, uint32_t dwOtherUnitId)
{
    // D2RosterUnitStrc* pRoster = D2Client_FindRosterUnitById(dwPlayerUnitId); // 0x6FB161D0
    D2RosterUnitStrc* pRoster = nullptr;
    if (!pRoster)
    {
        return;
    }

    D2PlayerListStrc* pList = pRoster->pPlayerList;
    if (!pList)
    {
        // FOG_DisplayWarning("pPlayerList", "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 113);
        return;
    }

    // D2PlayerListNodeStrc* pNode = (D2PlayerListNodeStrc*)FOG_AllocClientMemory(sizeof(D2PlayerListNodeStrc), "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 116, 0);
    D2PlayerListNodeStrc* pNode = nullptr;
    if (pNode)
    {
        pNode->dwUnitId = dwOtherUnitId;
        pNode->dwFlags = dwFlags;
        pNode->pNext = pList->pFirst;
        pList->pFirst = pNode;
    }
}

