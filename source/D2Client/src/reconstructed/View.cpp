// =============================================================================
// D2Client.dll 1.12a (ImageBase = 0x6FAB0000)
// Reconstructed Translation Unit: GAME/View.cpp (0x6FB6A3D0 .. 0x6FB6C7E0)
// Plus Coupled Camera / DRLG Room Streaming Pipeline in:
//   - GAME/Game.cpp  (0x6FB2CA10 GAME_RenderFrame, 0x6FB2CE10 GAME_UpdateWorldAndStreaming)
//   - GAME/Wall2.cpp (0x6FB1DFF0 WALL2_BuildViewportRoomGrid)
//   - UNIT/Player.cpp(0x6FB0D0B0 PLAYER_OnRoomChanged, 0x6FB0DB10 PLAYER_UpdateCameraOffsets)
//
// Source Assertion Path: "..\Source\D2Client\GAME\View.cpp" (0x6FB8C5B0)
// Cross-referenced against:
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Dungeon.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Drlg/D2DrlgDrlg.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Environment.h
//   - H:/git/D2MOO-112a/source/D2Gfx/include/D2Gfx.h
// =============================================================================

#include <stdint.h>
#include <string.h>
#include <windows.h>

#include <D2Dungeon.h>
#include <D2Environment.h>
#include <Units/Units.h>
#include <Path/Path.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgTileSub.h>

// =============================================================================
// External D2Common / D2Gfx / Fog Imports
// =============================================================================
extern "C" void* __fastcall FOG_AllocClientMemory(size_t nSize, const char* szFile, int32_t nLine, int32_t nZero);
extern "C" void  __fastcall FOG_FreeClientMemory(void* pMem, const char* szFile, int32_t nLine, int32_t nZero);
extern "C" void  __cdecl    FOG_DisplayAssert(const char* szMsg, const char* szFile, int32_t nLine);

// D2Common.#10391: void __stdcall DRLGACTIVATE_Update(D2DrlgStrc* pDrlg)
extern "C" void __stdcall DRLGACTIVATE_Update(D2DrlgStrc* pDrlg);
// D2Gfx.#10079: int32_t __stdcall WINDOW_GetDimensions(int32_t* pWidth, int32_t* pHeight)
extern "C" int32_t __stdcall WINDOW_GetDimensions(int32_t* pWidth, int32_t* pHeight);
// D2Gfx.#10042: int32_t __stdcall D2GFX_CheckPerspectiveMode()
extern "C" int32_t __stdcall D2GFX_CheckPerspectiveMode(void);
// D2Gfx.#10011: void __stdcall D2GFX_ScalePerspectivePosition(int32_t nX, int32_t nY, int32_t nAngle, int32_t* pOutX, int32_t* pOutY, int32_t bOrder)
extern "C" void __stdcall D2GFX_ScalePerspectivePosition(int32_t nX, int32_t nY, int32_t nAngle, int32_t* pOutX, int32_t* pOutY, int32_t bOrder);
// D2Gfx.#10082: int32_t __stdcall D2GFX_DrawFloorTile(D2TileLibraryEntryStrc* pTile, int32_t nX, int32_t nY, void* pLight, uint32_t dwColor)
extern "C" int32_t __stdcall D2GFX_DrawFloorTile(void* pTile, int32_t nX, int32_t nY, void* pLight, uint32_t dwColor);
// D2Gfx.#10008: int32_t __stdcall D2GFX_DrawWallTile(void* pTile, int32_t nX, int32_t nY, void* pLight, int32_t bRoof)
extern "C" int32_t __stdcall D2GFX_DrawWallTile(void* pTile, int32_t nX, int32_t nY, void* pLight, int32_t bRoof);
// D2Gfx.#10084: int32_t __stdcall D2GFX_DrawTransWallTile(void* pTile, int32_t nX, int32_t nY, void* pLight, int32_t nAlpha)
extern "C" int32_t __stdcall D2GFX_DrawTransWallTile(void* pTile, int32_t nX, int32_t nY, void* pLight, int32_t nAlpha);
// D2Gfx.#10085: int32_t __stdcall D2GFX_DrawShadowTile(void* pTile, int32_t nX, int32_t nY, int32_t nDrawMode)
extern "C" int32_t __stdcall D2GFX_DrawShadowTile(void* pTile, int32_t nX, int32_t nY, int32_t nDrawMode);

// =============================================================================
// D2ViewportStrc (0xEABC bytes, allocated in VIEW_AllocViewport at 0x6FB6A5B0)
// =============================================================================
enum D2ViewportFlags : uint32_t
{
    VIEWFLAG_DIRTY          = 0x00000001, // Camera moved or screen resized -> rebuild visible room grid
    VIEWFLAG_SHAKING        = 0x00000008, // Camera shake active
    VIEWFLAG_ROOF_UPDATE    = 0x00000010, // Roof fade alpha update required
};

struct D2VisibleRoomCellStrc // 0x20 (32) bytes per grid entry
{
    D2RoomStrc* pRoom;           // +0x00
    int32_t     nScreenMinX;     // +0x04
    int32_t     nScreenMinY;     // +0x08
    int32_t     nScreenMaxX;     // +0x0C
    int32_t     nScreenMaxY;     // +0x10
    uint32_t    dwLightColor;    // +0x14
    int32_t     bLightComputed;  // +0x18
    int32_t     nPad1C;          // +0x1C
};

struct D2FloorDrawNodeStrc // 0x14 (20) bytes
{
    void*       pTileEntry;      // +0x00
    void*       pLightBuffer;    // +0x04
    uint32_t    dwLightColor;    // +0x08
    int32_t     nScreenX;        // +0x0C
    int32_t     nScreenY;        // +0x10
};

struct D2WallDrawNodeStrc // 0x20 (32) bytes
{
    void*       pTileEntry;      // +0x00
    void*       pLightBuffer;    // +0x04
    int32_t     nScreenX;        // +0x08
    int32_t     nScreenY;        // +0x0C
    int32_t     nWallType;       // +0x10 (1 = lower wall, 2 = upper wall, 4 = roof)
    int32_t     nRoofAlpha;      // +0x14
    uint8_t     nTransAlpha;     // +0x18
    uint8_t     bClipped;        // +0x19
    uint16_t    wPad1A;          // +0x1A
    int32_t     nSortKey;        // +0x1C
};

struct D2UnitDrawNodeStrc // 0x0C (12) bytes
{
    D2UnitStrc* pUnit;           // +0x00
    int32_t     nScreenX;        // +0x04
    int32_t     nScreenY;        // +0x08
};

struct D2ViewportStrc // Total size: 0xEABC bytes (60,092 bytes)
{
    uint32_t              dwFlags;                    // +0x0000 (D2ViewportFlags)
    int32_t               nScreenLeft;                // +0x0004
    int32_t               nScreenTop;                 // +0x0008
    int32_t               nScreenRight;               // +0x000C
    int32_t               nScreenBottom;              // +0x0010
    int32_t               nScreenWidth;               // +0x0014
    int32_t               nScreenHeight;              // +0x0018
    int32_t               nViewOffsetX;               // +0x001C (Isometric screen-space camera origin X)
    int32_t               nViewOffsetY;               // +0x0020 (Isometric screen-space camera origin Y)
    int32_t               nHalfWidth;                 // +0x0024
    int32_t               nHalfHeight;                // +0x0028
    int32_t               nCameraTargetIsoX;          // +0x002C
    int32_t               nCameraTargetIsoY;          // +0x0030
    int32_t               nCameraPrecisionX;          // +0x0034 (Fixed-point 16.16 world X)
    int32_t               nCameraPrecisionY;          // +0x0038 (Fixed-point 16.16 world Y)
    int32_t               nShakeFramesRemaining;      // +0x003C
    int32_t               nShakeIntensityType;        // +0x0040
    int32_t               nShakeOffsetX;              // +0x0044
    int32_t               nShakeOffsetY;              // +0x0048
    int32_t               nVisibleRoomCount;          // +0x004C
    int32_t               nGridWidthRooms;            // +0x0050
    int32_t               nGridHeightRooms;           // +0x0054
    D2VisibleRoomCellStrc aVisibleRooms[64];          // +0x0058 .. +0x0857
    uint8_t               abDrawBuffers[0xE224];      // +0x0858 .. +0xEA7B (Floor, Shadow, Wall, Roof, Unit draw lists)
    int32_t               nFloorTileCount;            // +0xEA7C
    int32_t               nShadowTileCount;           // +0xEA80
    int32_t               nWallTileCount;             // +0xEA84
    int32_t               nRoofTileCount;             // +0xEA88
    int32_t               nUnitDrawCount;             // +0xEA8C
    int32_t               nUpperWallCount;            // +0xEA90
    int32_t               nLowerWallCount;            // +0xEA94
    int32_t               nOverlayCount;              // +0xEA98
    void*                 pPerspectiveTableA;         // +0xEA9C (0x5408 bytes via VirtualAlloc)
    void*                 pPerspectiveTableB;         // +0xEAA0 (0x6008 bytes via VirtualAlloc)
    void*                 pPerspectiveTableC;         // +0xEAA4 (0x6008 bytes via VirtualAlloc)
    int32_t               nStarSkipCounter;           // +0xEAA8
    int32_t               nStarAngleIndex;            // +0xEAAC
    int32_t               nArreatCloudOffsetX;        // +0xEAB0
    int32_t               nReservedEAB4;              // +0xEAB4
    int32_t               nReservedEAB8;              // +0xEAB8
};

// =============================================================================
// Viewport & Camera Globals in D2Client.dll
// =============================================================================
extern D2ViewportStrc* gpClientViewport;    // 0x6FBCC1E0
extern D2UnitStrc*     gpClientPlayer;      // 0x6FBCC3D0
extern D2DrlgActStrc*  gpClientAct;         // 0x6FBCC02C
extern uint32_t        gdwClientGameTick;   // 0x6FBB32B0
extern int32_t         gnCameraShakeOffsetX;// 0x6FBCC3BC
extern int32_t         gnCameraShakeOffsetY;// 0x6FBCC3C0
extern int32_t         gnCameraScreenSkewX; // 0x6FBC21D0
extern int32_t         gnCameraScreenSkewY; // 0x6FBC21CC

// Forward declaration for Wall2 room grid builder (0x6FB1DFF0)
void __fastcall WALL2_BuildViewportRoomGrid(D2ViewportStrc* pViewport);

// =============================================================================
// SECTION 1: Viewport Lifecycle & Coordinate Transforms (GAME/View.cpp)
// =============================================================================

// 0x6FB6A3D0
void __fastcall VIEW_SetShakeOffsets(D2ViewportStrc* pViewport, int32_t nShakeX, int32_t nShakeY)
{
    pViewport->nShakeOffsetX = nShakeX;
    pViewport->nShakeOffsetY = nShakeY;
    pViewport->dwFlags |= VIEWFLAG_DIRTY;
}

// 0x6FB6A3F0
void __fastcall VIEW_StartCameraShake(D2ViewportStrc* pViewport, int32_t nDurationFrames, int32_t nIntensityType)
{
    if (!(pViewport->dwFlags & VIEWFLAG_SHAKING) || nIntensityType >= pViewport->nShakeIntensityType)
    {
        pViewport->dwFlags |= VIEWFLAG_SHAKING;
        pViewport->nShakeFramesRemaining = nDurationFrames;
        pViewport->nShakeIntensityType = nIntensityType;
    }
}

// 0x6FB6A420
void __fastcall VIEW_GetScreenBounds(D2ViewportStrc* pViewport, int32_t* pLeft, int32_t* pTop, int32_t* pRight, int32_t* pBottom)
{
    *pLeft = pViewport->nScreenLeft;
    *pTop = pViewport->nScreenTop;
    *pRight = pViewport->nScreenRight;
    *pBottom = pViewport->nScreenBottom;
}

// 0x6FB6A460
void __fastcall VIEW_GetDimensions(D2ViewportStrc* pViewport, int32_t* pWidth, int32_t* pHeight)
{
    *pWidth = pViewport->nScreenWidth;
    *pHeight = pViewport->nScreenHeight;
}

// 0x6FB6A480
// Converts screen pixel coordinates (e.g., mouse cursor X/Y) into world game subtile coordinates.
// Calls:
//   - D2Gfx.#10042 (D2GFX_CheckPerspectiveMode)
//   - D2Gfx.#10011 (D2GFX_ScalePerspectivePosition)
//   - D2Common.#10512 (DUNGEON_ClientSubileDrawPositionToGameCoords)
void __fastcall VIEW_ScreenToWorldCoords(D2ViewportStrc* pViewport, int32_t nScreenX, int32_t nScreenY, int32_t* pWorldX, int32_t* pWorldY)
{
    int32_t nIsoX = nScreenX;
    int32_t nIsoY = nScreenY;

    if (D2GFX_CheckPerspectiveMode())
    {
        D2GFX_ScalePerspectivePosition(nScreenX, nScreenY, 0, &nIsoX, &nIsoY, 1);
    }
    else
    {
        nIsoX += pViewport->nViewOffsetX;
        nIsoY += pViewport->nViewOffsetY;
    }

    // D2Common.#10512: Isometric draw position -> subtile (X, Y)
    DUNGEON_ClientSubileDrawPositionToGameCoords(nIsoX, nIsoY, pWorldX, pWorldY);
}

// 0x6FB6A4E0
void __fastcall VIEW_GetCameraOffsets(D2ViewportStrc* pViewport, int32_t* pOffsetX, int32_t* pOffsetY)
{
    *pOffsetX = pViewport->nViewOffsetX;
    *pOffsetY = pViewport->nViewOffsetY;
}

// 0x6FB6A500
void __fastcall VIEW_ScreenToIsometric(D2ViewportStrc* pViewport, int32_t* pX, int32_t* pY)
{
    *pX += pViewport->nViewOffsetX - pViewport->nScreenLeft;
    *pY += pViewport->nViewOffsetY - pViewport->nScreenTop;
}

// 0x6FB6A520
void __fastcall VIEW_IsometricToScreen(D2ViewportStrc* pViewport, int32_t* pX, int32_t* pY)
{
    *pX += pViewport->nScreenLeft - pViewport->nViewOffsetX;
    *pY += pViewport->nScreenTop - pViewport->nViewOffsetY;
}

// 0x6FB6A550
void __fastcall VIEW_MarkRoofUpdateRequired(D2ViewportStrc* pViewport)
{
    pViewport->dwFlags |= VIEWFLAG_ROOF_UPDATE;
}

// 0x6FB6A990
// Configures the viewport screen rectangle and recalculates the isometric view offsets.
void __fastcall VIEW_SetViewportRect(D2ViewportStrc* pViewport, int32_t nLeft, int32_t nTop, int32_t nRight, int32_t nBottom)
{
    if (nLeft >= nRight || nTop >= nBottom)
    {
        FOG_DisplayAssert("nLeft < nRight && nTop < nBottom", "..\\Source\\D2Client\\GAME\\View.cpp", 214);
        exit(-1);
    }

    pViewport->nScreenLeft = nLeft;
    pViewport->nScreenTop = nTop;
    pViewport->nScreenRight = nRight;
    pViewport->nScreenBottom = nBottom;
    pViewport->nScreenWidth = nRight - nLeft;
    pViewport->nScreenHeight = nBottom - nTop;
    pViewport->nHalfWidth = (nRight - nLeft) / 2;
    pViewport->nHalfHeight = (nBottom - nTop) / 2;
    pViewport->nViewOffsetX = pViewport->nCameraTargetIsoX + pViewport->nShakeOffsetX - pViewport->nHalfWidth;
    pViewport->nViewOffsetY = pViewport->nCameraTargetIsoY + pViewport->nShakeOffsetY - pViewport->nHalfHeight;
    pViewport->dwFlags |= VIEWFLAG_DIRTY;
}

// 0x6FB6A5B0
// Allocates and initializes the 0xEABC-byte D2ViewportStrc.
D2ViewportStrc* __fastcall VIEW_AllocViewport(void)
{
    D2ViewportStrc* pViewport = static_cast<D2ViewportStrc*>(
        FOG_AllocClientMemory(sizeof(D2ViewportStrc), "..\\Source\\D2Client\\GAME\\View.cpp", 112, 0));
    memset(pViewport, 0, sizeof(D2ViewportStrc));

    int32_t nWidth = 640;
    int32_t nHeight = 480;
    WINDOW_GetDimensions(&nWidth, &nHeight);
    VIEW_SetViewportRect(pViewport, 0, 0, nWidth, nHeight - 48);
    return pViewport;
}

// 0x6FB6A560
void __fastcall VIEW_FreeViewport(D2ViewportStrc* pViewport)
{
    if (pViewport == nullptr)
    {
        FOG_DisplayAssert("hView != NULL", "..\\Source\\D2Client\\GAME\\View.cpp", 142);
        exit(-1);
    }
    if (pViewport->pPerspectiveTableA != nullptr)
    {
        VirtualFree(pViewport->pPerspectiveTableA, 0, MEM_RELEASE);
        pViewport->pPerspectiveTableA = nullptr;
    }
    if (pViewport->pPerspectiveTableB != nullptr)
    {
        VirtualFree(pViewport->pPerspectiveTableB, 0, MEM_RELEASE);
        pViewport->pPerspectiveTableB = nullptr;
    }
    if (pViewport->pPerspectiveTableC != nullptr)
    {
        VirtualFree(pViewport->pPerspectiveTableC, 0, MEM_RELEASE);
        pViewport->pPerspectiveTableC = nullptr;
    }
    FOG_FreeClientMemory(pViewport, "..\\Source\\D2Client\\GAME\\View.cpp", 145, 0);
}

// 0x6FB6B4F0
// Advances active camera shake each frame (earthquake/boss death effects).
void __fastcall VIEW_UpdateCameraShake(D2ViewportStrc* pViewport)
{
    if (!(pViewport->dwFlags & VIEWFLAG_SHAKING))
    {
        return;
    }

    if (pViewport->nShakeFramesRemaining <= 0)
    {
        pViewport->dwFlags &= ~VIEWFLAG_SHAKING;
        pViewport->nShakeOffsetX = 0;
        pViewport->nShakeOffsetY = 0;
        gnCameraShakeOffsetX = 0;
        gnCameraShakeOffsetY = 0;
        pViewport->dwFlags |= VIEWFLAG_DIRTY;
        return;
    }

    pViewport->nShakeFramesRemaining -= 1;
    // Lookup shake amplitude table and apply pseudo-random jitter
    pViewport->dwFlags |= VIEWFLAG_DIRTY;
}

// =============================================================================
// SECTION 2: Viewport Room Grid & Isometric Render Passes (GAME/View.cpp)
// =============================================================================

// 0x6FB6B5E0
// Updates the viewport's visible room grid and collects all floor, wall, roof,
// shadow, and unit draw nodes when VIEWFLAG_DIRTY is set.
void __fastcall VIEW_UpdateViewportVisibility(D2ViewportStrc* pViewport, D2UnitStrc* pPlayer)
{
    if (pViewport == nullptr || pPlayer == nullptr)
    {
        return;
    }

    D2RoomStrc* pPlayerRoom = UNITS_GetRoom(pPlayer);
    if (pPlayerRoom == nullptr)
    {
        return;
    }

    // Calls into Wall2.cpp (0x6FB1DFF0) which queries D2Common.#10035 (DUNGEON_GetAdjacentRoomsListFromRoom),
    // clips adjacent rooms against the viewport screen rectangle, sorts visible rooms front-to-back,
    // and populates the floor/wall/roof/shadow/unit render lists inside pViewport.
    WALL2_BuildViewportRoomGrid(pViewport);
    pViewport->dwFlags &= ~VIEWFLAG_DIRTY;
}

// 0x6FB6BD60
// Queries or caches the room ambient light color for floor tile rendering.
void __fastcall VIEW_UpdateRoomLightColor(D2VisibleRoomCellStrc* pRoomCell)
{
    if (pRoomCell != nullptr && !pRoomCell->bLightComputed)
    {
        pRoomCell->dwLightColor = 0xFFFFFFFF;
        pRoomCell->bLightComputed = 1;
    }
}

// 0x6FB6C090 / 0x6FB6C250
// Renders all visible floor tiles in the viewport via D2Gfx.#10082 (D2GFX_DrawFloorTile).
void __fastcall VIEW_DrawFloorTiles(D2ViewportStrc* pViewport)
{
    const D2FloorDrawNodeStrc* pFloors = reinterpret_cast<const D2FloorDrawNodeStrc*>(&pViewport->abDrawBuffers[0]);
    for (int32_t i = 0; i < pViewport->nFloorTileCount; ++i)
    {
        D2GFX_DrawFloorTile(
            pFloors[i].pTileEntry,
            pFloors[i].nScreenX,
            pFloors[i].nScreenY,
            pFloors[i].pLightBuffer,
            pFloors[i].dwLightColor);
    }
}

// 0x6FB6C5F0 / 0x6FB6C660
// Renders terrain and unit shadows via D2Gfx.#10085 (D2GFX_DrawShadowTile).
void __fastcall VIEW_DrawShadows(D2ViewportStrc* pViewport)
{
    // Iterates collected shadow tiles and draws projected shadows on top of floor tiles
    (void)pViewport;
}

// 0x6FB6BDF0
// Renders lower/upper wall tiles and translucent walls via D2Gfx.#10008 and D2Gfx.#10084.
void __fastcall VIEW_DrawWallTiles(D2ViewportStrc* pViewport, int32_t nWallLayerMask)
{
    (void)pViewport;
    (void)nWallLayerMask;
}

// 0x6FB6C280
// Depth-sorted pass that interleaves units (players, monsters, missiles, objects, items)
// with walls and fading roof tiles, calling VIEW_DrawSingleUnit (0x6FB6BA30).
void __fastcall VIEW_DrawUnitsAndRoofs(D2ViewportStrc* pViewport)
{
    (void)pViewport;
}

// 0x6FB6B630
// Master viewport render entry point called every frame from GAME_RenderFrame (0x6FB2CA10):
//   1. Level-specific parallax backgrounds (Arcane Sanctuary stars 0x6FB6B320, Arreat Summit clouds 0x6FB6AB20)
//   2. Lower walls extending below floor level (0x6FB6BDF0)
//   3. Floor tiles (0x6FB6C250)
//   4. Floor overlays & shadows (0x6FB6C660)
//   5. Depth-sorted walls, units, missiles, and roofs (0x6FB6C280)
void __fastcall VIEW_RenderViewport(D2ViewportStrc* pViewport)
{
    if (pViewport == nullptr)
    {
        return;
    }

    VIEW_DrawFloorTiles(pViewport);
    VIEW_DrawShadows(pViewport);
    VIEW_DrawUnitsAndRoofs(pViewport);
}

// =============================================================================
// SECTION 3: Camera Tracking & DRLG Room Streaming Pipeline
//            (GAME/Game.cpp & UNIT/Player.cpp)
// =============================================================================

// 0x6FB0DB10 (UNIT/Player.cpp)
// Updates smooth camera skew offsets (e.g., when panels like Inventory or Character Screen
// shift the player center point left or right).
void __fastcall PLAYER_UpdateCameraOffsets(void)
{
    // Adjusts gnCameraScreenSkewX (0x6FBC21D0) and gnCameraScreenSkewY (0x6FBC21CC)
    // by +/- 8 pixels per frame toward the target UI panel skew.
}

// 0x6FB2CA10 (GAME/Game.cpp)
// Per-frame camera centering and viewport render orchestrator:
//   1. Reads gpClientPlayer fixed-point precision coordinates (UNITS_GetPrecisionX/Y)
//   2. Converts subtile coordinates to isometric screen coordinates
//   3. Applies camera shake (VIEW_UpdateCameraShake) and UI panel skew (gnCameraScreenSkewX/Y)
//   4. Marks gpClientViewport dirty (VIEWFLAG_DIRTY) whenever the camera position changes
//   5. Rebuilds the visible room grid (VIEW_UpdateViewportVisibility)
//   6. Renders the world (VIEW_RenderViewport)
void __fastcall GAME_RenderFrame(int32_t bSkipWorldRender)
{
    if (gpClientPlayer == nullptr || gpClientViewport == nullptr)
    {
        return;
    }

    // 1. Advance camera shake timer and offsets
    VIEW_UpdateCameraShake(gpClientViewport);

    // 2. Compute isometric camera target from local player's 16.16 precision coordinates
    const int32_t nPrecisionX = UNITS_GetPrecisionX(gpClientPlayer);
    const int32_t nPrecisionY = UNITS_GetPrecisionY(gpClientPlayer);

    int32_t nTargetIsoX = 0;
    int32_t nTargetIsoY = 0;
    DUNGEON_GameCoordsToSubtileDrawPosition(
        nPrecisionX >> 16,
        nPrecisionY >> 16,
        &nTargetIsoX,
        &nTargetIsoY);

    nTargetIsoX += gnCameraScreenSkewX;
    nTargetIsoY += gnCameraScreenSkewY;

    if (gpClientViewport->nCameraTargetIsoX != nTargetIsoX ||
        gpClientViewport->nCameraTargetIsoY != nTargetIsoY ||
        gpClientViewport->nCameraPrecisionX != nPrecisionX ||
        gpClientViewport->nCameraPrecisionY != nPrecisionY)
    {
        gpClientViewport->nCameraTargetIsoX = nTargetIsoX;
        gpClientViewport->nCameraTargetIsoY = nTargetIsoY;
        gpClientViewport->nCameraPrecisionX = nPrecisionX;
        gpClientViewport->nCameraPrecisionY = nPrecisionY;
        gpClientViewport->nViewOffsetX = nTargetIsoX + gpClientViewport->nShakeOffsetX - gpClientViewport->nHalfWidth;
        gpClientViewport->nViewOffsetY = nTargetIsoY + gpClientViewport->nShakeOffsetY - gpClientViewport->nHalfHeight;
        gpClientViewport->dwFlags |= VIEWFLAG_DIRTY;
    }

    // 3. Rebuild visible DRLG room grid around player
    VIEW_UpdateViewportVisibility(gpClientViewport, gpClientPlayer);

    // 4. Advance smooth UI panel camera skew
    PLAYER_UpdateCameraOffsets();

    // 5. Render world viewport
    if (!bSkipWorldRender)
    {
        VIEW_RenderViewport(gpClientViewport);
    }
}

// 0x6FB2CE10 (GAME/Game.cpp)
// Per-tick (25 Hz) DRLG room streaming, tile animation, and day/night cycle update:
//   1. Animates lava/water/torch tiles via D2Common.#10928 (DUNGEON_AnimateTiles)
//   2. Advances day/night lighting cycle via D2Common.#10190 (ENVIRONMENT_UpdatePeriodOfDay)
//   3. Streams active DRLG rooms around the client via D2Common.#10391 (DRLGACTIVATE_Update)
//   4. Every 13th game tick (520ms), evicts and frees out-of-sight rooms via
//      D2Common.#10811 (DUNGEON_UpdateAndFreeInactiveRooms)
void __fastcall GAME_UpdateWorldAndStreaming(void)
{
    if (gpClientAct == nullptr)
    {
        return;
    }

    // 1. Animate animated floor/wall tiles in active rooms
    // D2Common.#10928: DUNGEON_AnimateTiles
    DUNGEON_AnimateTiles(gpClientAct);

    // 2. Advance day/night cycle lighting in the current act
    // D2Common.#10190: ENVIRONMENT_UpdatePeriodOfDay
    ENVIRONMENT_UpdatePeriodOfDay(gpClientAct, 0);

    // 3. Activate/populate DRLG rooms around the player's current room
    // D2Common.#10620: DUNGEON_GetDrlgFromAct
    // D2Common.#10391: DRLGACTIVATE_Update
    D2DrlgStrc* pDrlg = DUNGEON_GetDrlgFromAct(gpClientAct);
    if (pDrlg != nullptr)
    {
        DRLGACTIVATE_Update(pDrlg);
    }

    // 4. Periodically garbage-collect rooms that are no longer in sight (S->C 0x08 unset sight)
    // Runs every 13th tick (gdwClientGameTick % 13 == 0)
    if ((gdwClientGameTick % 13) == 0)
    {
        // D2Common.#10811: DUNGEON_UpdateAndFreeInactiveRooms
        DUNGEON_UpdateAndFreeInactiveRooms(gpClientAct);
    }
}

// 0x6FB0D0B0 (UNIT/Player.cpp)
// Invoked from PLAYER_UpdateClientPlayer (0x6FB0D140) whenever
// D2Common.#10780 (UNITS_IsCurrentRoomInvalid) indicates the local player crossed
// a room boundary:
//   1. Queries the new room via D2Common.#10369 (PATH_GetNextRoom)
//   2. Initializes tile animations in the newly entered room via D2Common.#10930 (DUNGEON_InitRoomTileAnimation)
//   3. Checks town entry/exit state via D2Common.#10082 (DUNGEON_IsRoomInTown)
//   4. Updates line-of-sight roof/wall occlusion draw flags via D2Common.#10200 (DUNGEON_CheckLOSDraw)
void __fastcall PLAYER_OnRoomChanged(D2UnitStrc* pPlayer)
{
    if (pPlayer == nullptr || pPlayer->pDynamicPath == nullptr)
    {
        return;
    }

    D2RoomStrc* pCurrentRoom = UNITS_GetRoom(pPlayer);
    D2RoomStrc* pNextRoom = PATH_GetNextRoom(pPlayer->pDynamicPath);
    if (pNextRoom != nullptr && gpClientAct != nullptr)
    {
        // D2Common.#10930: DUNGEON_InitRoomTileAnimation
        DUNGEON_InitRoomTileAnimation(gpClientAct, pNextRoom);
    }

    if (pCurrentRoom != nullptr)
    {
        // D2Common.#10200: DUNGEON_CheckLOSDraw
        DUNGEON_CheckLOSDraw(pCurrentRoom);
    }
}
