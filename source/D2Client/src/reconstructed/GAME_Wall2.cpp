// =============================================================================
// D2Client.dll 1.12a (ImageBase = 0x6FAB0000)
// Reconstructed Translation Units:
//   - GAME/Wall2.cpp    (0x6FB1CE20 .. 0x6FB1E270, 27 functions)
//   - CORE/ARCHIVE.CPP  (0x6FB6EEB0 .. 0x6FB6F050, 5 functions)
//   - GAME/PalShift.cpp (0x6FB6F050 .. 0x6FB6F870, 8 functions)
//
// Source Assertion Paths:
//   - "..\Source\D2Client\GAME\Wall2.cpp"    (0x6FB84D78)
//   - "..\Source\D2Client\CORE\ARCHIVE.CPP"  (0x6FB807A0)
//   - "..\Source\D2Client\GAME\PalShift.cpp" (0x6FB80754)
//
// Cross-referenced against:
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Dungeon.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2DataTbls.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Units.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Path/Path.h
//   - H:/git/D2MOO-112a/source/D2CMP/include/D2CMP.h
//   - H:/git/D2MOO-112a/source/D2Win/include/D2WinArchive.h
//   - H:/git/D2MOO-112a/source/D2Gfx/include/Window.h
//   - H:/git/D2MOO-112a/source/Storm/include/Storm.h
// =============================================================================

#include <stdint.h>
#include <string.h>
#include <windows.h>

#include <Fog.h>
#include <Storm.h>
#include <D2Dungeon.h>
#include <D2DataTbls.h>
#include <D2Seed.h>
#include <Units/Units.h>
#include <Path/Path.h>
#include <D2CMP.h>
#include <D2WinArchive.h>
#include <D2Gfx.h>
#include <Window.h>

// =============================================================================
// External Imports & Internal Helpers
// =============================================================================
// Fog.#10102 / Fog.#10103
extern "C" BOOL  __fastcall FOG_FOpenFile(const char* szPath, void** ppFileHandle);
extern "C" void  __fastcall FOG_FCloseFile(void* pFileHandle);

// Internal D2Hell Archive helpers (0x6FABDA10, 0x6FABDB60, 0x6FABDB90, 0x6FABDD00)
void*   __fastcall ARCHIVE_LoadFileToMemory(HD2ARCHIVE hArchive, const char* szFilePath, uint32_t* pOutSize, int32_t nFileType); // 0x6FABDD00
int32_t __fastcall ARCHIVE_OpenMpqaFile(const char* szFilePath, void** ppOutFileHandle, int32_t bAllowMissing);                  // 0x6FABDB90
int32_t __fastcall ARCHIVE_ReadMpqaFile(void* pFileHandle, int32_t nBytesToRead, void* pOutBuffer);                              // 0x6FABDA10
void    __fastcall ARCHIVE_CloseMpqaFile(void* pFileHandle);                                                                     // 0x6FABDB60

// Internal Engine / Unit helpers
D2MonStatsTxt*  __fastcall ENGINE_GetMonStatsTxtRecord(int32_t nMonsterClassId);  // 0x6FAB1390
int32_t         __fastcall ENGINE_GetMonStatsRecordCount(void);                   // 0x6FAB13C0
D2MonStats2Txt* __fastcall ENGINE_GetMonStats2TxtRecord(int32_t nMonsterClassId); // 0x6FAB13D0
uint32_t        __fastcall UNIT_GetMonsterPaletteIndex(D2UnitStrc* pUnit);        // 0x6FACEF60
int32_t         __fastcall UNIT_IsCorpseOrFloorUnit(D2UnitStrc* pUnit);           // 0x6FACF870
void            __fastcall RECORD_ProcessWindowMessage(uint32_t uMsg, LPARAM lParam); // 0x6FB26CA0

// =============================================================================
// Data Structures: Storm Message Registration & Isometric Wall/Roof Sort Grid
// =============================================================================

enum D2SMsgHandlerType : uint32_t
{
    SMSG_TYPE_MESSAGE  = 0,
    SMSG_TYPE_COMMAND  = 1,
    SMSG_TYPE_KEYUP    = 2,
    SMSG_TYPE_KEYDOWN  = 3,
};

struct SMsgWindowMessage
{
    HWND hWnd;
    UINT uMessage;
    WPARAM wParam;
    LPARAM lParam;
    uint32_t dwCommand;
    uint32_t dwKeyFlags;
    BOOL bHandled;
    LRESULT lResult;
};

typedef void (__stdcall* SMsgCallback)(SMsgWindowMessage*);

struct D2SMsgRegEntryStrc // 0x0C bytes
{
    uint32_t          nType;      // +0x00 (D2SMsgHandlerType)
    uint32_t          uMsgOrId;   // +0x04
    SMsgCallback      pfHandler;  // +0x08
};

struct D2IsoSortNodeStrc // 0x14 (20) bytes
{
    int32_t            nNodeType;          // +0x00 (0 = unit, 1 = wall/roof tile, 2 = overlay unit)
    int32_t            nPrecisionSubtileX; // +0x04 (subtileX * 8)
    int32_t            nPrecisionSubtileY; // +0x08 (subtileY * 8)
    void*              pOwner;             // +0x0C (D2RoomTileStrc* or D2UnitStrc*)
    D2IsoSortNodeStrc* pNext;              // +0x10
};

struct D2IsoSortCellStrc // 0x24 (36) bytes
{
    uint32_t           dwFlags;            // +0x00 (0x04 = has cutaway wall)
    uint32_t           dwReserved04;       // +0x04
    int32_t            nTileX;             // +0x08
    int32_t            nTileY;             // +0x0C
    D2IsoSortNodeStrc* pFloorAndRoofList;  // +0x10
    D2IsoSortNodeStrc* pLowerWallList;     // +0x14
    D2IsoSortNodeStrc* pNormalUnitList;    // +0x18
    D2IsoSortNodeStrc* pSpecialWallList;   // +0x1C (orientation == 15)
    D2IsoSortNodeStrc* pUpperWallList;     // +0x20 (orientation 16..19)
};

struct D2IsoSortGridStrc // Embedded at D2ViewportStrc + 0x38 (size = 0xEA84)
{
    uint32_t           dwGridFlags;        // +0x0000 (Viewport +0x0038)
    D2IsoSortNodeStrc  aNodes[3000];       // +0x0004 (Viewport +0x003C .. +0xEA9B, 0xBB8 nodes)
    int32_t            nNodeCount;         // +0xEA64 (Viewport +0xEA9C)
    int32_t            nOriginTileX;       // +0xEA68 (Viewport +0xEAA0)
    int32_t            nOriginTileY;       // +0xEA6C (Viewport +0xEAA4)
    D2IsoSortCellStrc* pCells;             // +0xEA70 (Viewport +0xEAA8)
    int32_t            nTotalCellCount;    // +0xEA74 (Viewport +0xEAAC)
    int32_t            nGridDimension;     // +0xEA78 (Viewport +0xEAB0)
    int32_t            nWidthTiles;        // +0xEA7C (Viewport +0xEAB4)
    int32_t            nHeightTiles;       // +0xEA80 (Viewport +0xEAB8)
};

struct D2WallViewportStrc // Full viewport header + embedded sort grid
{
    uint32_t          dwFlags;             // +0x0000
    int32_t           nScreenLeft;         // +0x0004
    int32_t           nScreenTop;          // +0x0008
    int32_t           nScreenRight;        // +0x000C
    int32_t           nScreenBottom;       // +0x0010
    int32_t           nClipLeft;           // +0x0014
    int32_t           nClipTop;            // +0x0018
    int32_t           nClipRight;          // +0x001C
    int32_t           nClipBottom;         // +0x0020
    int32_t           nCameraIsoX;         // +0x0024
    int32_t           nCameraIsoY;         // +0x0028
    int32_t           nReserved2C[3];      // +0x002C .. +0x0037
    D2IsoSortGridStrc tSortGrid;           // +0x0038 .. +0xEABB
};
static_assert(sizeof(D2WallViewportStrc) == 0xEABC, "D2WallViewportStrc must match retail 0xEABC-byte layout");

struct D2WallRoomCallbackContextStrc // Stack context in WALL2_BuildViewportRoomGrid
{
    D2WallViewportStrc* pViewport;         // +0x00
    RECT                rcScreenBounds;    // +0x04 (left, top, right, bottom)
    int32_t             bSkipUnits;        // +0x14
    D2ActiveRoomStrc*   aVisited[256];     // Deduplication set so cross-level rooms are visited once
    int32_t             nVisitedCount;
};

struct D2RoomWallTileEntryStrc // 0x30 (48) bytes per room wall/roof tile
{
    int32_t  nIsoDrawX;                    // +0x00
    int32_t  nIsoDrawY;                    // +0x04
    int32_t  nLocalTileX;                  // +0x08
    int32_t  nLocalTileY;                  // +0x0C
    void*    pRoofData;                    // +0x10 (+0x28 = roof group ID, +0x00 = minX, +0x04 = minY)
    uint32_t dwTileFlags;                  // +0x14 (0x04 = non-fading, 0x1C000 = sort priority)
    void*    pTileLibraryEntry;            // +0x18
    int32_t  nOrientation;                 // +0x1C
    int32_t  nStyleIndex;                  // +0x20
    uint32_t dwFadeStateFlags;             // +0x24 (bit 0 = fading out, bit 1 = active transition)
    uint8_t  nCurrentAlpha;                // +0x28
    uint8_t  nStartAlpha;                  // +0x29
    uint8_t  nTargetAlpha;                 // +0x2A
    uint8_t  nPad2B;                       // +0x2B
    uint32_t dwFadeEndTick;                // +0x2C
};

// =============================================================================
// Data Structures: Monster Palette Shift & GreenBlood Tables (GAME/PalShift.cpp)
// =============================================================================

struct D2MonPalShiftNodeStrc // 0x1008 bytes
{
    int32_t                nMonsterClassId;            // +0x0000
    uint8_t                aPalettes[8][256];          // +0x0004 (0x800 bytes: 8 standard palettes)
    uint8_t                aGreenBloodPalettes[8][256];// +0x0804 (0x800 bytes: 8 green-blood remapped palettes)
    D2MonPalShiftNodeStrc* pNext;                      // +0x1004
};

struct D2MonPalShiftIndexEntryStrc // 0x08 bytes
{
    int32_t                nMonsterClassId;            // +0x00
    D2MonPalShiftNodeStrc* pNode;                      // +0x04
};

// =============================================================================
// Module Globals (1.12a Addresses)
// =============================================================================
static int32_t                     g_bRoofCutawayMode       = 1;        // 0x6FB93A7C
static int32_t                     g_bAppActiveState        = 1;        // 0x6FB93A80
static int32_t                     g_nPlayerTileX           = 0;        // 0x6FBB5558
int32_t                            g_nPlayerRoofGroupId     = 0;        // 0x6FBB555C
static int32_t                     g_nPlayerTileY           = 0;        // 0x6FBB5560
static RECT                        g_rcViewportClipRect     = { 0 };    // 0x6FBB5564 .. 0x6FBB5570

static D2MonPalShiftIndexEntryStrc* gpMonPalShiftIndexTable = nullptr;  // 0x6FBAAD44
static uint8_t                     g_abRandTransforms[30][256] = { 0 }; // 0x6FBAAD48 (0x1E00 bytes)
static D2MonPalShiftNodeStrc*      gpMonPalShiftList        = nullptr;  // 0x6FBACB48
static int32_t                     g_bRandTransformsLoaded  = 0;        // 0x6FBCB984
static uint8_t*                    gpGreenBloodTable        = nullptr;  // 0x6FBCB988

extern HACCEL                      ghClientAcceleratorTable;            // 0x6FBCC43C
extern int32_t                     gbRecordPlaybackActive;              // 0x6FBCC098
extern int32_t                     gbGreenBloodEnabled;                 // 0x6FBCBFE4
extern D2UnitStrc*                 gpClientPlayer;                      // 0x6FBCC3D0

// Forward declarations for default window message handlers (0x6FB93A84 table)
void __stdcall WALL2_OnDefaultWindowMessage(SMsgWindowMessage* pMsg);   // 0x6FB1D030
void __stdcall WALL2_OnActivateAppMessage(SMsgWindowMessage* pMsg);     // 0x6FB1D060
void __stdcall WALL2_OnDestroyWindowMessage(SMsgWindowMessage* pMsg);   // 0x6FB1D080

// 0x6FB93A84: Default 4-entry Storm message table
static const D2SMsgRegEntryStrc g_aDefaultWindowMsgTable[4] =
{
    { SMSG_TYPE_MESSAGE, WM_DESTROY,     WALL2_OnDestroyWindowMessage },
    { SMSG_TYPE_MESSAGE, WM_ACTIVATEAPP, WALL2_OnActivateAppMessage   },
    { SMSG_TYPE_MESSAGE, WM_PAINT,       WALL2_OnDefaultWindowMessage },
    { SMSG_TYPE_MESSAGE, WM_ERASEBKGND,  WALL2_OnDefaultWindowMessage },
};

// =============================================================================
// SECTION 1: Storm Message Dispatch & Window Procedure (0x6FB1CE20..0x6FB1D200)
// =============================================================================

// 1.12a: 0x6FB1CE20 (__fastcall)
void __fastcall WALL2_PopWindowMsgState(void)
{
    HWND hWnd = WINDOW_GetWindow();
    SMsgPopRegisterState(hWnd);
}

// 1.12a: 0x6FB1CE30 (__fastcall)
void __fastcall WALL2_PushWindowMsgState(void)
{
    HWND hWnd = WINDOW_GetWindow();
    SMsgPushRegisterState(hWnd);
}

// 1.12a: 0x6FB1CE40 (usercall: eax = hWnd, esi = nType, ecx = uMsgOrId, edx = pfHandler)
void __fastcall WALL2_UnregisterSingleMsgHandler(HWND hWnd, uint32_t nType, uint32_t uMsgOrId, SMsgCallback pfHandler)
{
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("hWnd != NULL", "..\\Source\\D2Client\\GAME\\Wall2.cpp", 343);
        exit(-1);
    }

    switch (nType)
    {
    case SMSG_TYPE_MESSAGE:
        if (uMsgOrId == WM_COMMAND)
        {
            FOG_DisplayHalt("uMsg != WM_COMMAND", "..\\Source\\D2Client\\GAME\\Wall2.cpp", 347);
            exit(-1);
        }
        SMsgUnregisterMessage(hWnd, uMsgOrId, pfHandler);
        break;
    case SMSG_TYPE_COMMAND:
        SMsgUnregisterCommand(hWnd, uMsgOrId, pfHandler);
        break;
    case SMSG_TYPE_KEYUP:
        SMsgUnregisterKeyUp(hWnd, uMsgOrId, pfHandler);
        break;
    case SMSG_TYPE_KEYDOWN:
        SMsgUnregisterKeyDown(hWnd, uMsgOrId, pfHandler);
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FB1CEC0 (usercall: eax = hWnd, esi = nType, ecx = uMsgOrId, edx = pfHandler)
void __fastcall WALL2_RegisterSingleMsgHandler(HWND hWnd, uint32_t nType, uint32_t uMsgOrId, SMsgCallback pfHandler)
{
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("hWnd != NULL", "..\\Source\\D2Client\\GAME\\Wall2.cpp", 310);
        exit(-1);
    }

    switch (nType)
    {
    case SMSG_TYPE_MESSAGE:
        if (uMsgOrId == WM_COMMAND)
        {
            FOG_DisplayHalt("uMsg != WM_COMMAND", "..\\Source\\D2Client\\GAME\\Wall2.cpp", 314);
            exit(-1);
        }
        SMsgRegisterMessage(hWnd, uMsgOrId, pfHandler);
        break;
    case SMSG_TYPE_COMMAND:
        SMsgRegisterCommand(hWnd, uMsgOrId, pfHandler);
        break;
    case SMSG_TYPE_KEYUP:
        SMsgRegisterKeyUp(hWnd, uMsgOrId, pfHandler);
        break;
    case SMSG_TYPE_KEYDOWN:
        SMsgRegisterKeyDown(hWnd, uMsgOrId, pfHandler);
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FB1CF40 (__fastcall)
// Pumps pending Windows messages via PeekMessageA / GetMessageA, translates accelerators,
// and dispatches messages while the application window is active.
int32_t __fastcall WALL2_PumpWindowMessages(void)
{
    MSG msg = { 0 };
    BOOL bGotMessage = TRUE;

    while (PeekMessageA(&msg, nullptr, 0, 0, PM_NOREMOVE))
    {
        bGotMessage = GetMessageA(&msg, nullptr, 0, 0);
        if (!bGotMessage)
        {
            break;
        }

        if (ghClientAcceleratorTable != nullptr)
        {
            HWND hWnd = WINDOW_GetWindow();
            if (TranslateAcceleratorA(hWnd, ghClientAcceleratorTable, &msg))
            {
                continue;
            }
        }

        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return bGotMessage ? 1 : 0;
}

// 1.12a: 0x6FB1D030 (__stdcall)
void __stdcall WALL2_OnDefaultWindowMessage(SMsgWindowMessage* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = DefWindowProcA(pMsg->hWnd, pMsg->uMessage, pMsg->wParam, pMsg->lParam);
}

// 1.12a: 0x6FB1D060 (__stdcall)
void __stdcall WALL2_OnActivateAppMessage(SMsgWindowMessage* pMsg)
{
    pMsg->bHandled = FALSE;
    g_bAppActiveState = (pMsg->wParam != 0) ? 1 : 0;
}

// 1.12a: 0x6FB1D080 (__stdcall)
void __stdcall WALL2_OnDestroyWindowMessage(SMsgWindowMessage* pMsg)
{
    PostQuitMessage(0);
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
}

// 1.12a: 0x6FB1D0A0 (usercall: eax = nCount, [esp+4] = hWnd, [esp+8] = pEntries)
void __fastcall WALL2_UnregisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries)
{
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("hWnd != NULL", "..\\Source\\D2Client\\GAME\\Wall2.cpp", 396);
        exit(-1);
    }

    for (uint32_t i = 0; i < nCount; ++i)
    {
        WALL2_UnregisterSingleMsgHandler(hWnd, pEntries[i].nType, pEntries[i].uMsgOrId, pEntries[i].pfHandler);
    }
}

// 1.12a: 0x6FB1D100 (usercall: eax = nCount, [esp+4] = hWnd, [esp+8] = pEntries)
void __fastcall WALL2_RegisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries)
{
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("hWnd != NULL", "..\\Source\\D2Client\\GAME\\Wall2.cpp", 377);
        exit(-1);
    }

    for (uint32_t i = 0; i < nCount; ++i)
    {
        WALL2_RegisterSingleMsgHandler(hWnd, pEntries[i].nType, pEntries[i].uMsgOrId, pEntries[i].pfHandler);
    }
}

// 1.12a: 0x6FB1D160 (usercall: ebx = hWnd)
void __fastcall WALL2_ShutdownDefaultWindowMsgHandlers(HWND hWnd)
{
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("hWnd != NULL", "..\\Source\\D2Client\\GAME\\Wall2.cpp", 396);
        exit(-1);
    }

    for (uint32_t i = 0; i < 4; ++i)
    {
        WALL2_UnregisterSingleMsgHandler(
            hWnd,
            g_aDefaultWindowMsgTable[i].nType,
            g_aDefaultWindowMsgTable[i].uMsgOrId,
            g_aDefaultWindowMsgTable[i].pfHandler);
    }
    SMsgPopRegisterState(hWnd);
}

// 1.12a: 0x6FB1D1C0 (usercall: esi = hWnd)
int32_t __fastcall WALL2_InitDefaultWindowMsgHandlers(HWND hWnd)
{
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("hWnd != NULL", "..\\Source\\D2Client\\GAME\\Wall2.cpp", 414);
        exit(-1);
    }

    WALL2_RegisterMsgHandlerTable(4, hWnd, g_aDefaultWindowMsgTable);
    SMsgPushRegisterState(hWnd);
    return 1;
}

// 1.12a: 0x6FB1D200 (__stdcall)
LRESULT __stdcall WALL2_MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL bHandled = FALSE;
    LRESULT lResult = 0;

    RECORD_ProcessWindowMessage(uMsg, lParam);

    if (!gbRecordPlaybackActive || uMsg == WM_CLOSE)
    {
        if (SMsgDispatchMessage(hWnd, uMsg, wParam, lParam, &bHandled, &lResult) && bHandled)
        {
            return lResult;
        }
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

// =============================================================================
// SECTION 2: Isometric Room/Wall/Roof/Unit Sort Grid (0x6FB1D280..0x6FB1E26D)
// =============================================================================

// 1.12a: 0x6FB1D280 (usercall: edi = pSortGrid, ebx = pWallTile, [esp+4] = pCell, [esp+8] = pRoom)
void __fastcall WALL2_InsertFloorOrRoofTileNode(
    D2IsoSortGridStrc* pSortGrid,
    D2RoomWallTileEntryStrc* pWallTile,
    D2IsoSortCellStrc* pCell,
    D2ActiveRoomStrc* pRoom)
{
    const int32_t nIndex = pSortGrid->nNodeCount;
    pSortGrid->nNodeCount = nIndex + 1;
    if (nIndex >= 3000)
    {
        return;
    }

    D2IsoSortNodeStrc* pNode = &pSortGrid->aNodes[nIndex];
    pNode->nNodeType = 1;
    pNode->pOwner = pWallTile;
    pNode->pNext = nullptr;

    D2DrlgCoordsStrc tRoomCoords = { 0 };
    DUNGEON_GetRoomCoordinates(pRoom, &tRoomCoords);

    pNode->nPrecisionSubtileX = (tRoomCoords.nSubtileX + pWallTile->nLocalTileX * 5) << 3;
    pNode->nPrecisionSubtileY = (tRoomCoords.nSubtileY + pWallTile->nLocalTileY * 5) << 3;

    pSortGrid->dwGridFlags |= gdwBitMasks[3]; // 0x08

    if (pCell->pFloorAndRoofList == nullptr)
    {
        pCell->pFloorAndRoofList = pNode;
        return;
    }

    D2IsoSortNodeStrc* pTail = pCell->pFloorAndRoofList;
    while (pTail->pNext != nullptr)
    {
        pTail = pTail->pNext;
    }
    pTail->pNext = pNode;
}

// 1.12a: 0x6FB1D330 (usercall: ecx = pSortGrid, eax = pWallTile, [esp+4] = pCell, [esp+8] = pRoom)
void __fastcall WALL2_InsertSpecialWallTileNode(
    D2IsoSortGridStrc* pSortGrid,
    D2RoomWallTileEntryStrc* pWallTile,
    D2IsoSortCellStrc* pCell,
    D2ActiveRoomStrc* pRoom)
{
    const int32_t nIndex = pSortGrid->nNodeCount;
    pSortGrid->nNodeCount = nIndex + 1;
    if (nIndex >= 3000)
    {
        return;
    }

    D2IsoSortNodeStrc* pNode = &pSortGrid->aNodes[nIndex];
    pNode->nNodeType = 1;
    pNode->pOwner = pWallTile;
    pNode->pNext = nullptr;

    D2DrlgCoordsStrc tRoomCoords = { 0 };
    DUNGEON_GetRoomCoordinates(pRoom, &tRoomCoords);

    pNode->nPrecisionSubtileX = (tRoomCoords.nSubtileX + pWallTile->nLocalTileX * 5) << 3;
    pNode->nPrecisionSubtileY = (tRoomCoords.nSubtileY + pWallTile->nLocalTileY * 5) << 3;

    pSortGrid->dwGridFlags |= gdwBitMasks[1]; // 0x02

    if (pCell->pSpecialWallList == nullptr)
    {
        pCell->pSpecialWallList = pNode;
        return;
    }

    const int32_t nNewLayer = (pWallTile->dwTileFlags >> 14) & 7;
    D2IsoSortNodeStrc* pPrev = nullptr;
    D2IsoSortNodeStrc* pCurr = pCell->pSpecialWallList;

    while (pCurr != nullptr)
    {
        const D2RoomWallTileEntryStrc* pCurrTile = static_cast<const D2RoomWallTileEntryStrc*>(pCurr->pOwner);
        const int32_t nCurrLayer = (pCurrTile->dwTileFlags >> 14) & 7;
        if (nNewLayer < nCurrLayer)
        {
            pNode->pNext = pCurr;
            if (pPrev != nullptr)
            {
                pPrev->pNext = pNode;
            }
            else
            {
                pCell->pSpecialWallList = pNode;
            }
            return;
        }
        pPrev = pCurr;
        pCurr = pCurr->pNext;
    }

    if (pPrev != nullptr)
    {
        pPrev->pNext = pNode;
    }
}

// 1.12a: 0x6FB1D420 (usercall: ecx = pSortGrid, eax = pWallTile, [esp+4] = pCell, [esp+8] = pRoom)
void __fastcall WALL2_InsertUpperWallTileNode(
    D2IsoSortGridStrc* pSortGrid,
    D2RoomWallTileEntryStrc* pWallTile,
    D2IsoSortCellStrc* pCell,
    D2ActiveRoomStrc* pRoom)
{
    if (pWallTile->nOrientation != 0 && pWallTile->nOrientation != 13 && !(pWallTile->dwTileFlags & 0x04))
    {
        pCell->dwFlags |= 0x04;
    }

    const int32_t nIndex = pSortGrid->nNodeCount;
    pSortGrid->nNodeCount = nIndex + 1;
    if (nIndex >= 3000)
    {
        return;
    }

    D2IsoSortNodeStrc* pNode = &pSortGrid->aNodes[nIndex];
    pNode->nNodeType = 1;
    pNode->pOwner = pWallTile;
    pNode->pNext = nullptr;

    D2DrlgCoordsStrc tRoomCoords = { 0 };
    DUNGEON_GetRoomCoordinates(pRoom, &tRoomCoords);

    pNode->nPrecisionSubtileX = (tRoomCoords.nSubtileX + pWallTile->nLocalTileX * 5) << 3;
    pNode->nPrecisionSubtileY = (tRoomCoords.nSubtileY + pWallTile->nLocalTileY * 5) << 3;

    pSortGrid->dwGridFlags |= gdwBitMasks[0]; // 0x01

    if (pCell->pUpperWallList == nullptr)
    {
        pCell->pUpperWallList = pNode;
        return;
    }

    const int32_t nNewLayer = (pWallTile->dwTileFlags >> 14) & 7;
    D2IsoSortNodeStrc* pPrev = nullptr;
    D2IsoSortNodeStrc* pCurr = pCell->pUpperWallList;

    while (pCurr != nullptr)
    {
        const D2RoomWallTileEntryStrc* pCurrTile = static_cast<const D2RoomWallTileEntryStrc*>(pCurr->pOwner);
        const int32_t nCurrLayer = (pCurrTile->dwTileFlags >> 14) & 7;
        if (nNewLayer < nCurrLayer)
        {
            pNode->pNext = pCurr;
            if (pPrev != nullptr)
            {
                pPrev->pNext = pNode;
            }
            else
            {
                pCell->pUpperWallList = pNode;
            }
            return;
        }
        pPrev = pCurr;
        pCurr = pCurr->pNext;
    }

    if (pPrev != nullptr)
    {
        pPrev->pNext = pNode;
    }
}

// 1.12a: 0x6FB1D520 (usercall: eax = pWallTile, [esp+4] = pRoom)
// Determines whether a wall/roof tile occludes the local player and should fade out.
int32_t __fastcall WALL2_ShouldFadeWallTile(D2RoomWallTileEntryStrc* pWallTile, D2ActiveRoomStrc* pRoom)
{
    if (g_bRoofCutawayMode != 0)
    {
        const int32_t* pRoofData = static_cast<const int32_t*>(pWallTile->pRoofData);
        if (pRoofData == nullptr)
        {
            return 0;
        }
        if (pRoofData[10] == g_nPlayerRoofGroupId) // +0x28 = roof group ID
        {
            return 0;
        }
        if (g_nPlayerTileX < pRoofData[0])
        {
            return 1;
        }
        return (g_nPlayerTileY < pRoofData[1]) ? 1 : 0;
    }

    D2DrlgCoordsStrc tRoomCoords = { 0 };
    DUNGEON_GetRoomCoordinates(pRoom, &tRoomCoords);

    const int32_t nWallSubtileX = tRoomCoords.nSubtileX + pWallTile->nLocalTileX * 5;
    const int32_t nWallSubtileY = tRoomCoords.nSubtileY + pWallTile->nLocalTileY * 5;
    const int32_t nPlayerSubtileX = g_nPlayerTileX * 5;
    const int32_t nPlayerSubtileY = g_nPlayerTileY * 5;

    if (nPlayerSubtileX < nWallSubtileX && nPlayerSubtileX > (nWallSubtileX - 20))
    {
        const int32_t nOrient = pWallTile->nOrientation;
        if (nOrient == 1 || nOrient == 3 || nOrient == 4 || nOrient == 5 ||
            nOrient == 8 || nOrient == 10 || nOrient == 11 || nOrient == 12)
        {
            return 1;
        }
    }

    if (nPlayerSubtileY < nWallSubtileY && nPlayerSubtileY > (nWallSubtileY - 20))
    {
        const int32_t nOrient = pWallTile->nOrientation;
        if (nOrient == 2 || nOrient == 3 || nOrient == 4 || nOrient == 6 ||
            nOrient == 7 || nOrient == 8 || nOrient == 9 || nOrient == 12)
        {
            return 1;
        }
    }

    return 0;
}

// 1.12a: 0x6FB1D640 (usercall: eax = nTileX, ecx = nTileY, edx = pSortGrid)
int32_t __fastcall WALL2_IsTileInsideSortGrid(int32_t nTileX, int32_t nTileY, const D2IsoSortGridStrc* pSortGrid)
{
    const int32_t nRelX = nTileX - pSortGrid->nOriginTileX;
    const int32_t nRelY = nTileY - pSortGrid->nOriginTileY;
    if (nRelX < 0 || nRelY < 0)
    {
        return 0;
    }
    const int32_t nDim = pSortGrid->nGridDimension;
    if (nRelX >= nDim || nRelY >= nDim)
    {
        return 0;
    }
    return 1;
}

// 1.12a: 0x6FB1D670 (usercall: esi = pViewport)
void __fastcall WALL2_FreeSortGridCells(D2WallViewportStrc* pViewport)
{
    if (pViewport->tSortGrid.pCells != nullptr)
    {
        FOG_Free(pViewport->tSortGrid.pCells, "..\\Source\\D2Client\\GAME\\Wall2.cpp", 85, 0);
    }
    pViewport->tSortGrid.pCells = nullptr;
    pViewport->tSortGrid.nGridDimension = 0;
    pViewport->tSortGrid.nNodeCount = 0;
    pViewport->tSortGrid.dwGridFlags = 0;
}

// 1.12a: 0x6FB1D6B0 (usercall: eax = pViewport)
void __fastcall WALL2_InitSortGridFields(D2WallViewportStrc* pViewport)
{
    pViewport->tSortGrid.nGridDimension = 0;
    pViewport->tSortGrid.nNodeCount = 0;
    pViewport->tSortGrid.dwGridFlags = 0;
    pViewport->tSortGrid.pCells = nullptr;
}

// 1.12a: 0x6FB1D6D0 (usercall: esi = pViewport)
// Computes the isometric tile grid dimensions from the viewport pixel bounds and reallocates
// `pViewport->tSortGrid.pCells` (`nTotalCellCount * 0x24` bytes) when the resolution changes.
void __fastcall WALL2_EnsureSortGridAllocated(D2WallViewportStrc* pViewport)
{
    const int32_t nWidthPx  = pViewport->nScreenRight - pViewport->nScreenLeft;
    const int32_t nHeightPx = pViewport->nScreenBottom - pViewport->nScreenTop;

    const int32_t nWidthTiles  = (nWidthPx / 160) + 11;
    const int32_t nHeightTiles = (nHeightPx / 80) + 12;
    const int32_t nGridDim     = nWidthTiles + nHeightTiles - 1;
    const int32_t nTotalCells  = nGridDim * nGridDim;

    if (pViewport->tSortGrid.pCells == nullptr || pViewport->tSortGrid.nTotalCellCount != nTotalCells)
    {
        if (pViewport->tSortGrid.pCells != nullptr)
        {
            FOG_Free(pViewport->tSortGrid.pCells, "..\\Source\\D2Client\\GAME\\Wall2.cpp", 58, 0);
        }
        pViewport->tSortGrid.pCells = static_cast<D2IsoSortCellStrc*>(
            FOG_Alloc(nTotalCells * sizeof(D2IsoSortCellStrc), "..\\Source\\D2Client\\GAME\\Wall2.cpp", 61, 0));
        pViewport->tSortGrid.nTotalCellCount = nTotalCells;
    }

    pViewport->tSortGrid.nWidthTiles = nWidthTiles;
    pViewport->tSortGrid.nGridDimension = nGridDim;
    pViewport->tSortGrid.nHeightTiles = nHeightTiles;
}

// Helper to get unit subtile X/Y
static inline int32_t WALL2_GetUnitSubtileX(D2UnitStrc* pUnit)
{
    if (pUnit->dwUnitType == UNIT_OBJECT || pUnit->dwUnitType == UNIT_ITEM || pUnit->dwUnitType == UNIT_TILE)
    {
        return pUnit->pStaticPath ? pUnit->pStaticPath->nXPos : 0;
    }
    return pUnit->pDynamicPath ? PATH_GetXPosition(pUnit->pDynamicPath) : 0;
}

static inline int32_t WALL2_GetUnitSubtileY(D2UnitStrc* pUnit)
{
    if (pUnit->dwUnitType == UNIT_OBJECT || pUnit->dwUnitType == UNIT_ITEM || pUnit->dwUnitType == UNIT_TILE)
    {
        return pUnit->pStaticPath ? pUnit->pStaticPath->nYPos : 0;
    }
    return pUnit->pDynamicPath ? PATH_GetYPosition(pUnit->pDynamicPath) : 0;
}

// 1.12a: 0x6FB1D780 (usercall: eax = pUnit, ebx = pSortGrid, [esp+4] = pCell)
void __fastcall WALL2_InsertOverlayUnitNode(D2UnitStrc* pUnit, D2IsoSortGridStrc* pSortGrid, D2IsoSortCellStrc* pCell)
{
    const int32_t nIndex = pSortGrid->nNodeCount;
    pSortGrid->nNodeCount = nIndex + 1;
    if (nIndex >= 3000)
    {
        return;
    }

    D2IsoSortNodeStrc* pNode = &pSortGrid->aNodes[nIndex];
    pNode->nNodeType = 2;
    pNode->pOwner = pUnit;
    pNode->pNext = nullptr;
    pNode->nPrecisionSubtileX = WALL2_GetUnitSubtileX(pUnit) << 3;
    pNode->nPrecisionSubtileY = WALL2_GetUnitSubtileY(pUnit) << 3;

    pSortGrid->dwGridFlags |= gdwBitMasks[3]; // 0x08

    if (pCell->pFloorAndRoofList == nullptr)
    {
        pCell->pFloorAndRoofList = pNode;
        return;
    }

    D2IsoSortNodeStrc* pTail = pCell->pFloorAndRoofList;
    while (pTail->pNext != nullptr)
    {
        pTail = pTail->pNext;
    }
    pTail->pNext = pNode;
}

// 1.12a: 0x6FB1D860 (usercall: eax = pUnit, ebx = pSortGrid, [esp+4] = pCell)
void __fastcall WALL2_InsertCorpseOrFloorUnitNode(D2UnitStrc* pUnit, D2IsoSortGridStrc* pSortGrid, D2IsoSortCellStrc* pCell)
{
    const int32_t nIndex = pSortGrid->nNodeCount;
    pSortGrid->nNodeCount = nIndex + 1;
    if (nIndex >= 3000)
    {
        return;
    }

    D2IsoSortNodeStrc* pNode = &pSortGrid->aNodes[nIndex];
    pNode->nNodeType = 0;
    pNode->pOwner = pUnit;
    pNode->pNext = nullptr;
    pNode->nPrecisionSubtileX = WALL2_GetUnitSubtileX(pUnit) << 3;
    pNode->nPrecisionSubtileY = WALL2_GetUnitSubtileY(pUnit) << 3;

    pSortGrid->dwGridFlags |= gdwBitMasks[3]; // 0x08

    if (pCell->pFloorAndRoofList == nullptr)
    {
        pCell->pFloorAndRoofList = pNode;
        return;
    }

    D2IsoSortNodeStrc* pTail = pCell->pFloorAndRoofList;
    while (pTail->pNext != nullptr)
    {
        pTail = pTail->pNext;
    }
    pTail->pNext = pNode;
}

// 1.12a: 0x6FB1D940 (usercall: ecx = pSortGrid, eax = pWallTile, [esp+4] = pCell, [esp+8] = pRoom)
// Updates wall cutaway fade alpha state (500ms transition between 0xFF and 0x80) and inserts
// the lower wall tile node into `pCell->pLowerWallList`.
void __fastcall WALL2_InsertLowerWallTileNode(
    D2IsoSortGridStrc* pSortGrid,
    D2RoomWallTileEntryStrc* pWallTile,
    D2IsoSortCellStrc* pCell,
    D2ActiveRoomStrc* pRoom)
{
    if (pWallTile->nOrientation != 0 && pWallTile->nOrientation != 13 && !(pWallTile->dwTileFlags & 0x04))
    {
        pCell->dwFlags |= 0x04;
    }

    const int32_t nIndex = pSortGrid->nNodeCount;
    pSortGrid->nNodeCount = nIndex + 1;
    if (nIndex >= 3000)
    {
        return;
    }

    const uint32_t dwTargetEndTick = GetTickCount() + 500;
    D2IsoSortNodeStrc* pNode = &pSortGrid->aNodes[nIndex];
    pNode->nNodeType = 1;
    pNode->pOwner = pWallTile;
    pNode->pNext = nullptr;

    if (!(pWallTile->dwTileFlags & 0x04))
    {
        const int32_t bShouldFade = WALL2_ShouldFadeWallTile(pWallTile, pRoom);
        uint32_t dwFadeFlags = pWallTile->dwFadeStateFlags;

        if (bShouldFade)
        {
            if (!(dwFadeFlags & 0x01))
            {
                const uint8_t nCurAlpha = pWallTile->nCurrentAlpha;
                pWallTile->nStartAlpha = 0xFF;
                pWallTile->nTargetAlpha = 0x80;
                if (nCurAlpha == 0xFF)
                {
                    pWallTile->dwFadeEndTick = dwTargetEndTick;
                }
                else
                {
                    pWallTile->dwFadeEndTick = dwTargetEndTick + ((static_cast<int32_t>(nCurAlpha) - 255) * 500) / 127;
                }
                dwFadeFlags |= 0x03;
                pWallTile->dwFadeStateFlags = dwFadeFlags;
            }
        }
        else
        {
            if (dwFadeFlags & 0x01)
            {
                const uint8_t nCurAlpha = pWallTile->nCurrentAlpha;
                pWallTile->nStartAlpha = 0x80;
                pWallTile->nTargetAlpha = 0xFF;
                if (nCurAlpha == 0x80)
                {
                    pWallTile->dwFadeEndTick = dwTargetEndTick;
                }
                else
                {
                    pWallTile->dwFadeEndTick = dwTargetEndTick + ((128 - static_cast<int32_t>(nCurAlpha)) * 500) / 127;
                }
                dwFadeFlags = (dwFadeFlags & ~0x01u) | 0x02u;
                pWallTile->dwFadeStateFlags = dwFadeFlags;
            }
        }
    }

    D2DrlgCoordsStrc tRoomCoords = { 0 };
    DUNGEON_GetRoomCoordinates(pRoom, &tRoomCoords);

    pNode->nPrecisionSubtileX = (tRoomCoords.nSubtileX + pWallTile->nLocalTileX * 5) << 3;
    pNode->nPrecisionSubtileY = (tRoomCoords.nSubtileY + pWallTile->nLocalTileY * 5) << 3;

    if (pCell->pLowerWallList == nullptr)
    {
        pCell->pLowerWallList = pNode;
        return;
    }

    const int32_t nNewLayer = (pWallTile->dwTileFlags >> 14) & 7;
    D2IsoSortNodeStrc* pPrev = nullptr;
    D2IsoSortNodeStrc* pCurr = pCell->pLowerWallList;

    while (pCurr != nullptr)
    {
        const D2RoomWallTileEntryStrc* pCurrTile = static_cast<const D2RoomWallTileEntryStrc*>(pCurr->pOwner);
        const int32_t nCurrLayer = (pCurrTile->dwTileFlags >> 14) & 7;
        if (nNewLayer < nCurrLayer)
        {
            pNode->pNext = pCurr;
            if (pPrev != nullptr)
            {
                pPrev->pNext = pNode;
            }
            else
            {
                pCell->pLowerWallList = pNode;
            }
            return;
        }
        pPrev = pCurr;
        pCurr = pCurr->pNext;
    }

    if (pPrev != nullptr)
    {
        pPrev->pNext = pNode;
    }
}

// 1.12a: 0x6FB1DB00 (usercall: ecx = pSortGrid, eax = pUnit, ebx = pCell)
void __fastcall WALL2_InsertNormalUnitNode(D2IsoSortGridStrc* pSortGrid, D2UnitStrc* pUnit, D2IsoSortCellStrc* pCell)
{
    const int32_t nIndex = pSortGrid->nNodeCount;
    pSortGrid->nNodeCount = nIndex + 1;
    if (nIndex >= 3000)
    {
        return;
    }

    D2IsoSortNodeStrc* pNode = &pSortGrid->aNodes[nIndex];
    pNode->nNodeType = 0;
    pNode->pOwner = pUnit;
    pNode->pNext = nullptr;
    pNode->nPrecisionSubtileX = WALL2_GetUnitSubtileX(pUnit) << 3;
    pNode->nPrecisionSubtileY = WALL2_GetUnitSubtileY(pUnit) << 3;

    if (pCell->pNormalUnitList == nullptr)
    {
        pCell->pNormalUnitList = pNode;
        return;
    }

    D2IsoSortNodeStrc* pTail = pCell->pNormalUnitList;
    while (pTail->pNext != nullptr)
    {
        pTail = pTail->pNext;
    }
    pTail->pNext = pNode;
}

// 1.12a: 0x6FB1DBD0 (__stdcall)
// Room callback invoked via DUNGEON_CallRoomCallback to bin all visible wall tiles, roof tiles,
// and units of `pRoom` into the viewport's isometric sort grid cells.
BOOL __stdcall WALL2_PopulateSortGridRoomCallback(D2ActiveRoomStrc* pRoom, void* pVoidContext)
{
    if (pRoom == nullptr)
    {
        return TRUE;
    }

    D2WallRoomCallbackContextStrc* pCtx = static_cast<D2WallRoomCallbackContextStrc*>(pVoidContext);
    for (int32_t v = 0; v < pCtx->nVisitedCount; ++v)
    {
        if (pCtx->aVisited[v] == pRoom)
        {
            return TRUE;
        }
    }
    if (pCtx->nVisitedCount < 256)
    {
        pCtx->aVisited[pCtx->nVisitedCount++] = pRoom;
    }

    D2IsoSortGridStrc* pSortGrid = &pCtx->pViewport->tSortGrid;

    RECT rcRoom = { 0 };
    D2Common_10052(pRoom, &rcRoom);

    if (rcRoom.left > pCtx->rcScreenBounds.right ||
        rcRoom.top > (pCtx->rcScreenBounds.bottom + 400) ||
        rcRoom.right < (pCtx->rcScreenBounds.left - 200) ||
        rcRoom.bottom < pCtx->rcScreenBounds.top)
    {
        return TRUE;
    }

    // 1. Bin Wall Tiles
    uint32_t nWallTileCount = 0;
    D2RoomWallTileEntryStrc* pWallTiles = reinterpret_cast<D2RoomWallTileEntryStrc*>(
        DUNGEON_GetWallTilesFromRoom(pRoom, reinterpret_cast<int*>(&nWallTileCount)));

    for (uint32_t i = 0; i < nWallTileCount; ++i)
    {
        D2RoomWallTileEntryStrc* pTile = &pWallTiles[i];
        if (pTile->pTileLibraryEntry == nullptr || (pTile->dwTileFlags & 0x201) != 0)
        {
            continue;
        }
        if ((pTile->dwFadeStateFlags & 3) == 0 && pTile->nCurrentAlpha == 0)
        {
            pTile->nCurrentAlpha = 0xFF;
        }

        const int32_t nTotalHeight = D2CMP_TileGetTotalHeight(static_cast<D2TileLibraryEntryStrc*>(pTile->pTileLibraryEntry));

        int32_t nGameTileX = 0;
        int32_t nGameTileY = 0;
        DUNGEON_ClientTileDrawPositionToGameCoords(pTile->nIsoDrawX, pTile->nIsoDrawY, &nGameTileX, &nGameTileY);
        nGameTileY -= 1;

        if (pTile->nIsoDrawX <= pCtx->rcScreenBounds.right &&
            (pTile->nIsoDrawX + 160) >= pCtx->rcScreenBounds.left)
        {
            const uint16_t wRoofOffset = *reinterpret_cast<const uint16_t*>(
                static_cast<const uint8_t*>(pTile->pTileLibraryEntry) + 4);
            const int32_t nBaseY = pTile->nIsoDrawY - wRoofOffset;

            if ((nBaseY + nTotalHeight) >= pCtx->rcScreenBounds.top &&
                (nBaseY - nTotalHeight) <= pCtx->rcScreenBounds.bottom &&
                WALL2_IsTileInsideSortGrid(nGameTileX, nGameTileY, pSortGrid))
            {
                const int32_t nRelX = nGameTileX - pSortGrid->nOriginTileX;
                const int32_t nRelY = nGameTileY - pSortGrid->nOriginTileY;
                D2IsoSortCellStrc* pCell = &pSortGrid->pCells[nRelY * pSortGrid->nGridDimension + nRelX];

                if (pTile->nOrientation == 15)
                {
                    WALL2_InsertSpecialWallTileNode(pSortGrid, pTile, pCell, pRoom);
                }
                else if (pTile->nOrientation > 15 && pTile->nOrientation <= 19)
                {
                    WALL2_InsertUpperWallTileNode(pSortGrid, pTile, pCell, pRoom);
                }
                else if (!(pTile->dwTileFlags & 0x1C000))
                {
                    WALL2_InsertLowerWallTileNode(pSortGrid, pTile, pCell, pRoom);
                }
                else
                {
                    WALL2_InsertLowerWallTileNode(pSortGrid, pTile, pCell, pRoom);
                }
            }
        }
    }

    // 2. Bin Roof / Shadow Tiles
    uint32_t nRoofTileCount = 0;
    D2RoomWallTileEntryStrc* pRoofTiles = reinterpret_cast<D2RoomWallTileEntryStrc*>(
        DUNGEON_GetRoofTilesFromRoom(pRoom, reinterpret_cast<int*>(&nRoofTileCount)));

    for (uint32_t i = 0; i < nRoofTileCount; ++i)
    {
        D2RoomWallTileEntryStrc* pTile = &pRoofTiles[i];
        if (pTile->pTileLibraryEntry == nullptr || (pTile->dwTileFlags & 0x201) != 0)
        {
            continue;
        }
        if ((pTile->dwFadeStateFlags & 3) == 0 && pTile->nCurrentAlpha == 0)
        {
            pTile->nCurrentAlpha = 0xFF;
        }

        const int32_t nTotalHeight = D2CMP_TileGetTotalHeight(static_cast<D2TileLibraryEntryStrc*>(pTile->pTileLibraryEntry));

        int32_t nGameTileX = 0;
        int32_t nGameTileY = 0;
        DUNGEON_ClientTileDrawPositionToGameCoords(pTile->nIsoDrawX, pTile->nIsoDrawY, &nGameTileX, &nGameTileY);
        nGameTileY -= 1;

        if (pTile->nIsoDrawX <= pCtx->rcScreenBounds.right &&
            (pTile->nIsoDrawX + 160) >= pCtx->rcScreenBounds.left &&
            (pTile->nIsoDrawY + nTotalHeight) >= pCtx->rcScreenBounds.top &&
            (pTile->nIsoDrawY - nTotalHeight) <= pCtx->rcScreenBounds.bottom &&
            WALL2_IsTileInsideSortGrid(nGameTileX, nGameTileY, pSortGrid))
        {
            const int32_t nRelX = nGameTileX - pSortGrid->nOriginTileX;
            const int32_t nRelY = nGameTileY - pSortGrid->nOriginTileY;
            D2IsoSortCellStrc* pCell = &pSortGrid->pCells[nRelY * pSortGrid->nGridDimension + nRelX];
            WALL2_InsertFloorOrRoofTileNode(pSortGrid, pTile, pCell, pRoom);
        }
    }

    // 3. Bin Room Units
    if (!pCtx->bSkipUnits)
    {
        for (D2UnitStrc* pUnit = DUNGEON_GetFirstUnitInRoom(pRoom); pUnit != nullptr; pUnit = pUnit->pRoomNext)
        {
            const int32_t nIsoX = UNITS_GetClientCoordX(pUnit);
            const int32_t nIsoY = UNITS_GetClientCoordY(pUnit);

            int32_t nGameTileX = 0;
            int32_t nGameTileY = 0;
            DUNGEON_ClientTileDrawPositionToGameCoords(nIsoX, nIsoY, &nGameTileX, &nGameTileY);

            const int32_t nRelX = nGameTileX - pSortGrid->nOriginTileX;
            const int32_t nRelY = nGameTileY - pSortGrid->nOriginTileY;
            const int32_t nDim  = pSortGrid->nGridDimension;

            if (nRelX < 0 || nRelY < 0 || nRelX >= nDim || nRelY >= nDim)
            {
                continue;
            }

            D2IsoSortCellStrc* pCell = &pSortGrid->pCells[nRelY * nDim + nRelX];

            if (UNIT_IsCorpseOrFloorUnit(pUnit) || ((pUnit->dwFlags >> 20) & 1))
            {
                WALL2_InsertCorpseOrFloorUnitNode(pUnit, pSortGrid, pCell);
            }
            else
            {
                WALL2_InsertNormalUnitNode(pSortGrid, pUnit, pCell);
                if ((pUnit->dwFlagsEx >> 7) & 1)
                {
                    WALL2_InsertOverlayUnitNode(pUnit, pSortGrid, pCell);
                }
            }
        }
    }

    return TRUE;
}

// 1.12a: 0x6FB1DFF0 (usercall: eax = pViewport, [esp+4] = bSkipUnits)
// Rebuilds the viewport's isometric sort grid for the current camera position and local player room.
void __fastcall WALL2_BuildViewportRoomGrid(D2WallViewportStrc* pViewport, int32_t bSkipUnits)
{
    if (pViewport == nullptr)
    {
        return;
    }

    WALL2_EnsureSortGridAllocated(pViewport);

    D2WallRoomCallbackContextStrc tCtx = {};
    tCtx.pViewport = pViewport;
    tCtx.bSkipUnits = bSkipUnits;

    const int32_t nWidthPx  = pViewport->nScreenRight - pViewport->nScreenLeft;
    const int32_t nHeightPx = pViewport->nScreenBottom - pViewport->nScreenTop;
    int32_t nLeft   = pViewport->nCameraIsoX;
    int32_t nTop    = pViewport->nCameraIsoY;
    int32_t nRight  = nLeft + nWidthPx;
    int32_t nBottom = nTop + nHeightPx;

    if (D2GFX_CheckPerspective())
    {
        nLeft  -= 48;
        nTop   -= 48;
        nRight += 48;
    }

    SetRect(&tCtx.rcScreenBounds, nLeft, nTop, nRight, nBottom);
    g_rcViewportClipRect = tCtx.rcScreenBounds;

    pViewport->tSortGrid.nNodeCount = 0;
    pViewport->tSortGrid.dwGridFlags = 0;

    if (pViewport->tSortGrid.pCells == nullptr)
    {
        return;
    }

    memset(
        pViewport->tSortGrid.pCells,
        0,
        pViewport->tSortGrid.nTotalCellCount * sizeof(D2IsoSortCellStrc));

    int32_t nOriginTileX = 0;
    int32_t nOriginTileY = 0;
    DUNGEON_ClientTileDrawPositionToGameCoords(
        pViewport->nCameraIsoX,
        pViewport->nCameraIsoY,
        &nOriginTileX,
        &nOriginTileY);

    nOriginTileX -= 3;
    nOriginTileY -= pViewport->tSortGrid.nWidthTiles;
    pViewport->tSortGrid.nOriginTileX = nOriginTileX;
    pViewport->tSortGrid.nOriginTileY = nOriginTileY;

    const int32_t nDim = pViewport->tSortGrid.nGridDimension;
    D2IsoSortCellStrc* pCellIter = pViewport->tSortGrid.pCells;
    for (int32_t y = 0; y < nDim; ++y)
    {
        for (int32_t x = 0; x < nDim; ++x)
        {
            pCellIter->nTileX = nOriginTileX + x;
            pCellIter->nTileY = nOriginTileY + y;
            ++pCellIter;
        }
    }

    extern D2DrlgActStrc* gpClientAct;
    extern D2UnitStrc* gpPlayerUnit;
    D2UnitStrc* pPlayer = gpClientPlayer ? gpClientPlayer : gpPlayerUnit;
    if (pPlayer != nullptr)
    {
        const int32_t nPlayerSubtileX = WALL2_GetUnitSubtileX(pPlayer);
        const int32_t nPlayerSubtileY = WALL2_GetUnitSubtileY(pPlayer);
        D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(pPlayer);
        if (pPlayerRoom == nullptr && gpClientAct != nullptr && (nPlayerSubtileX != 0 || nPlayerSubtileY != 0))
        {
            pPlayerRoom = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, nPlayerSubtileX, nPlayerSubtileY);
        }

        g_nPlayerTileX = nPlayerSubtileX / 5;
        g_nPlayerTileY = nPlayerSubtileY / 5;
        g_nPlayerRoofGroupId = pPlayerRoom ? D2Common_10095(pPlayerRoom, nPlayerSubtileX, nPlayerSubtileY) : 0;

        if (pPlayerRoom != nullptr)
        {
            DUNGEON_CallRoomCallback(pPlayerRoom, WALL2_PopulateSortGridRoomCallback, &tCtx);
        }
    }

    // Also inspect any streamed active rooms in gpClientAct (e.g. across town/wilderness level seams)
    if (gpClientAct != nullptr)
    {
        for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom != nullptr; pRoom = pRoom->pRoomNext)
        {
            WALL2_PopulateSortGridRoomCallback(pRoom, &tCtx);
        }
    }
}

// =============================================================================
// SECTION 3: Cell File Archive Loading (CORE/ARCHIVE.CPP, 0x6FB6EEB0..0x6FB6F009)
// =============================================================================

// 1.12a: 0x6FB6EEB0 (usercall: esi = pCellFile)
void __fastcall D2Client_ARCHIVE_FreeCellFile(D2CellFileStrc* pCellFile)
{
    D2CMP_CelFileFreeHardware(pCellFile);
    if (pCellFile != nullptr)
    {
        FOG_Free(pCellFile, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
    }
}

// 1.12a: 0x6FB6EED0 (usercall: eax = szFileName, [esp+4] = nFileType, [esp+8] = bNoLocale)
D2CellFileStrc* __fastcall ARCHIVE_LoadCellFile(const char* szFileName, int32_t nFileType, int32_t bNoLocale)
{
    (void)bNoLocale;
    char szFullPath[260];
    strcpy(szFullPath, szFileName);
    strcat(szFullPath, D2CMP_GetGfxFileExtension(nFileType));

    HD2ARCHIVE hArchive = D2Win_GetArchive();
    uint32_t dwFileSize = 0;
    void* pRawBuffer = ARCHIVE_LoadFileToMemory(hArchive, szFullPath, &dwFileSize, 1);

    D2CellFileStrc* pCellFile = nullptr;
    D2CMP_CelFileNormalize(
        static_cast<D2CellFileStrc*>(pRawBuffer),
        &pCellFile,
        "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP",
        107,
        -1,
        0);

    return pCellFile;
}

// 1.12a: 0x6FB6EF70 (usercall: eax = szFileName, [esp+4] = nFileType)
D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileDefault(const char* szFileName, int32_t nFileType)
{
    char szFullPath[260];
    strcpy(szFullPath, szFileName);
    strcat(szFullPath, D2CMP_GetGfxFileExtension(nFileType));

    HD2ARCHIVE hArchive = D2Win_GetArchive();
    uint32_t dwFileSize = 0;
    void* pRawBuffer = ARCHIVE_LoadFileToMemory(hArchive, szFullPath, &dwFileSize, 1);

    D2CellFileStrc* pCellFile = nullptr;
    D2CMP_CelFileNormalize(
        static_cast<D2CellFileStrc*>(pRawBuffer),
        &pCellFile,
        "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP",
        86,
        -1,
        0);

    return pCellFile;
}

// =============================================================================
// SECTION 4: Monster Palette Shifting & GreenBlood (GAME/PalShift.cpp, 0x6FB6F010..0x6FB6F870)
// =============================================================================

// 1.12a: 0x6FB6F010 (__fastcall)
uint8_t* __fastcall PALSHIFT_GetGreenBloodTable(void)
{
    return gpGreenBloodTable;
}

// 1.12a: 0x6FB6F020 (__fastcall: ecx = nMonsterClassId)
D2MonPalShiftNodeStrc* __fastcall PALSHIFT_FindMonsterPalShiftNode(int32_t nMonsterClassId)
{
    for (D2MonPalShiftNodeStrc* pNode = gpMonPalShiftList; pNode != nullptr; pNode = pNode->pNext)
    {
        if (pNode->nMonsterClassId == nMonsterClassId)
        {
            return pNode;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FB6F050 (__fastcall)
void __fastcall PALSHIFT_FreeTables(void)
{
    while (gpMonPalShiftList != nullptr)
    {
        D2MonPalShiftNodeStrc* pNext = gpMonPalShiftList->pNext;
        FOG_Free(gpMonPalShiftList, "..\\Source\\D2Client\\GAME\\PalShift.cpp", 132, 0);
        gpMonPalShiftList = pNext;
    }

    if (gpMonPalShiftIndexTable != nullptr)
    {
        FOG_Free(gpMonPalShiftIndexTable, "..\\Source\\D2Client\\GAME\\PalShift.cpp", 138, 0);
        gpMonPalShiftIndexTable = nullptr;
    }

    if (gpGreenBloodTable != nullptr)
    {
        FOG_Free(gpGreenBloodTable, "..\\Source\\D2Client\\GAME\\PalShift.cpp", 144, 0);
        gpGreenBloodTable = nullptr;
    }
}

// 1.12a: 0x6FB6F0D0 (__fastcall)
void __fastcall PALSHIFT_LoadGreenBloodTable(void)
{
    void* pFileHandle = nullptr;
    if (!FOG_FOpenFile("Data\\Global\\Monsters\\GreenBlood.dat", &pFileHandle))
    {
        if (GetLastError() != ERROR_FILE_NOT_FOUND)
        {
            FOG_Trace("Error opening file: %s", "Data\\Global\\Monsters\\GreenBlood.dat");
        }
        return;
    }

    uint8_t* pTable = static_cast<uint8_t*>(
        FOG_Alloc(256, "..\\Source\\D2Client\\GAME\\PalShift.cpp", 82, 0));
    memset(pTable, 0, 256);
    gpGreenBloodTable = pTable;

    ARCHIVE_ReadMpqaFile(pFileHandle, 256, pTable);
    if (pFileHandle == nullptr)
    {
        FOG_DisplayHalt("pFileHandle != NULL", "..\\Source\\D2Client\\GAME\\PalShift.cpp", 73);
        exit(-1);
    }
    FOG_FCloseFile(pFileHandle);
}

// 1.12a: 0x6FB6F170 (__fastcall)
void __fastcall PALSHIFT_InitTables(void)
{
    const int32_t nMonCount = sgptDataTables->nMonStatsTxtRecordCount;
    const int32_t nAllocBytes = nMonCount * sizeof(D2MonPalShiftIndexEntryStrc);

    D2MonPalShiftIndexEntryStrc* pIndexTable = static_cast<D2MonPalShiftIndexEntryStrc*>(
        FOG_Alloc(nAllocBytes, "..\\Source\\D2Client\\GAME\\PalShift.cpp", 99, 0));
    memset(pIndexTable, 0, nAllocBytes);
    gpMonPalShiftIndexTable = pIndexTable;

    for (int32_t i = 0; i < sgptDataTables->nMonStatsTxtRecordCount; ++i)
    {
        pIndexTable[i].nMonsterClassId = i;
    }

    gpMonPalShiftList = nullptr;

    void* pFileHandle = nullptr;
    if (!FOG_FOpenFile("Data\\Global\\Monsters\\RandTransforms.dat", &pFileHandle))
    {
        if (GetLastError() != ERROR_FILE_NOT_FOUND)
        {
            FOG_Trace("Error opening file: %s", "Data\\Global\\Monsters\\RandTransforms.dat");
        }
        PALSHIFT_LoadGreenBloodTable();
        return;
    }

    ARCHIVE_ReadMpqaFile(pFileHandle, sizeof(g_abRandTransforms), g_abRandTransforms);
    if (pFileHandle == nullptr)
    {
        FOG_DisplayHalt("pFileHandle != NULL", "..\\Source\\D2Client\\GAME\\PalShift.cpp", 73);
        exit(-1);
    }
    FOG_FCloseFile(pFileHandle);

    g_bRandTransformsLoaded = 1;
    PALSHIFT_LoadGreenBloodTable();
}

// 1.12a: 0x6FB6F260 (usercall: eax = pUnit, dx = wBossSeedOffset)
int32_t __fastcall PALSHIFT_GetRandomBossTransformIndex(D2UnitStrc* pUnit, uint16_t wBossSeedOffset)
{
    if (pUnit == nullptr)
    {
        return 9;
    }

    SEED_InitLowSeed(&pUnit->pSeed, pUnit->dwClassId + static_cast<int32_t>(wBossSeedOffset));
    const uint64_t ullNext = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
    pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(ullNext);
    pUnit->pSeed.nHighSeed = static_cast<uint32_t>(ullNext >> 32);

    return static_cast<int32_t>(pUnit->pSeed.nLowSeed % 30u) + 9;
}

// 1.12a: 0x6FB6F2B0 (usercall: eax = pUnit)
// Resolves the 256-byte palette shift table for a monster unit (including unique/champion random
// transforms and GreenBlood censorship remapping).
uint8_t* __fastcall PALSHIFT_GetUnitPaletteTransform(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return nullptr;
    }

    D2MonPalShiftNodeStrc* pNode = gpMonPalShiftIndexTable[pUnit->dwClassId].pNode;
    if (pNode == nullptr)
    {
        return nullptr;
    }

    const uint32_t nPalIdx = UNIT_GetMonsterPaletteIndex(pUnit);
    if (nPalIdx <= 7)
    {
        switch (nPalIdx)
        {
        case 0:
        case 1:
        case 2:
            if (pUnit->dwClassId >= 363 && pUnit->dwClassId <= 364)
            {
                return pNode->aPalettes[nPalIdx];
            }
            return nullptr;

        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            if (gbGreenBloodEnabled != 0)
            {
                D2MonStats2Txt* pMonStats2 = ENGINE_GetMonStats2TxtRecord(pUnit->dwClassId);
                if (pMonStats2 != nullptr && pMonStats2->nBloodLocal == 2)
                {
                    return pNode->aGreenBloodPalettes[nPalIdx];
                }
            }
            return pNode->aPalettes[nPalIdx];
        }
    }

    if (g_bRandTransformsLoaded != 0)
    {
        const int32_t nRandIdx = static_cast<int32_t>(nPalIdx) - 8;
        if (nRandIdx < 0 || nRandIdx >= 30)
        {
            FOG_DisplayHalt("nRandIdx >= 0 && nRandIdx < 30", "..\\Source\\D2Client\\GAME\\PalShift.cpp", 352);
            exit(-1);
        }
        return g_abRandTransforms[nRandIdx];
    }

    if (gbGreenBloodEnabled != 0)
    {
        D2MonStats2Txt* pMonStats2 = ENGINE_GetMonStats2TxtRecord(pUnit->dwClassId);
        if (pMonStats2 != nullptr && pMonStats2->nBloodLocal == 2)
        {
            return pNode->aGreenBloodPalettes[nPalIdx];
        }
    }
    return pNode->aPalettes[nPalIdx];
}

// 1.12a: 0x6FB6F3A0 (usercall: eax = nMonsterClassId, edx = pNode)
void __fastcall PALSHIFT_BuildGreenBloodPalette(int32_t nMonsterClassId, D2MonPalShiftNodeStrc* pNode)
{
    if (nMonsterClassId < 0 || nMonsterClassId >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        return;
    }

    const D2MonStatsTxt* pMonStats = &sgptDataTables->pMonStatsTxt[nMonsterClassId];
    const int32_t nMonStatsEx = pMonStats->wMonStatsEx;
    if (nMonStatsEx < 0 || nMonStatsEx >= sgptDataTables->nMonStats2TxtRecordCount)
    {
        return;
    }

    const D2MonStats2Txt* pMonStats2 = &sgptDataTables->pMonStats2Txt[nMonStatsEx];
    if (pMonStats2 == nullptr || pMonStats2->nBloodLocal != 2)
    {
        return;
    }

    memcpy(pNode->aGreenBloodPalettes, pNode->aPalettes, sizeof(pNode->aPalettes));
    if (gpGreenBloodTable == nullptr)
    {
        return;
    }

    for (int32_t nPal = 0; nPal < 8; ++nPal)
    {
        for (int32_t i = 0; i < 256; ++i)
        {
            const uint8_t nOrigColor = pNode->aPalettes[nPal][i];
            const uint8_t nMappedColor = gpGreenBloodTable[nOrigColor];
            if (nMappedColor != 0)
            {
                pNode->aGreenBloodPalettes[nPal][i] = nMappedColor;
            }
        }
    }
}

// 1.12a: 0x6FB6F4B0 (__fastcall: ecx = nMonsterClassId)
void __fastcall PALSHIFT_LoadMonsterPalShift(int32_t nMonsterClassId)
{
    D2MonStatsTxt* pMonStats = ENGINE_GetMonStatsTxtRecord(nMonsterClassId);
    if (pMonStats == nullptr)
    {
        return;
    }

    if (PALSHIFT_FindMonsterPalShiftNode(nMonsterClassId) != nullptr)
    {
        return;
    }

    char szToken[4] = { 0 };
    const char* pCode = reinterpret_cast<const char*>(&pMonStats->dwCode);
    szToken[0] = (pCode[0] != ' ') ? pCode[0] : '\0';
    szToken[1] = (pCode[1] != ' ') ? pCode[1] : '\0';
    szToken[2] = '\0';

    char szPath[260];
    strcpy(szPath, "Data\\Global\\Monsters\\");
    strcat(szPath, szToken);
    strcat(szPath, "\\Cof\\palshift.dat");

    void* pFileHandle = nullptr;
    if (!ARCHIVE_OpenMpqaFile(szPath, &pFileHandle, 1))
    {
        return;
    }

    D2MonPalShiftNodeStrc* pNode = static_cast<D2MonPalShiftNodeStrc*>(
        FOG_Alloc(sizeof(D2MonPalShiftNodeStrc), "..\\Source\\D2Client\\GAME\\PalShift.cpp", 244, 0));
    memset(pNode, 0, sizeof(D2MonPalShiftNodeStrc));

    ARCHIVE_ReadMpqaFile(pFileHandle, sizeof(pNode->aPalettes), pNode->aPalettes);
    ARCHIVE_CloseMpqaFile(pFileHandle);

    PALSHIFT_BuildGreenBloodPalette(nMonsterClassId, pNode);

    pNode->nMonsterClassId = nMonsterClassId;
    pNode->pNext = gpMonPalShiftList;
    gpMonPalShiftList = pNode;

    const int32_t nTotalMonsters = ENGINE_GetMonStatsRecordCount();
    for (int32_t i = 0; i < nTotalMonsters; ++i)
    {
        if (pNode->nMonsterClassId == i)
        {
            gpMonPalShiftIndexTable[i].pNode = pNode;
        }
    }
}

// 1.12a: 0x6FB6F7A0 (__fastcall)
void __fastcall PALSHIFT_ResetAndPreloadDefaultMonsters(void)
{
    while (gpMonPalShiftList != nullptr)
    {
        D2MonPalShiftNodeStrc* pNext = gpMonPalShiftList->pNext;
        FOG_Free(gpMonPalShiftList, "..\\Source\\D2Client\\GAME\\PalShift.cpp", 279, 0);
        gpMonPalShiftList = pNext;
    }

    const int32_t nMonCount = sgptDataTables->nMonStatsTxtRecordCount;
    const int32_t nAllocBytes = nMonCount * sizeof(D2MonPalShiftIndexEntryStrc);

    if (gpMonPalShiftIndexTable == nullptr)
    {
        gpMonPalShiftIndexTable = static_cast<D2MonPalShiftIndexEntryStrc*>(
            FOG_Alloc(nAllocBytes, "..\\Source\\D2Client\\GAME\\PalShift.cpp", 287, 0));
    }

    memset(gpMonPalShiftIndexTable, 0, nAllocBytes);
    for (int32_t i = 0; i < nMonCount; ++i)
    {
        gpMonPalShiftIndexTable[i].nMonsterClassId = i;
    }

    PALSHIFT_LoadMonsterPalShift(363); // 0x16B
    PALSHIFT_LoadMonsterPalShift(364); // 0x16C
    PALSHIFT_LoadMonsterPalShift(420); // 0x1A4
    PALSHIFT_LoadMonsterPalShift(421); // 0x1A5
}
