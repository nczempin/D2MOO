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

struct D2IsoSortNodeStrc // 0x14 (20) bytes
{
    int32_t            nNodeType;          // +0x00
    int32_t            nPrecisionSubtileX; // +0x04
    int32_t            nPrecisionSubtileY; // +0x08
    void*              pOwner;             // +0x0C
    D2IsoSortNodeStrc* pNext;              // +0x10
};

struct D2IsoSortCellStrc // 0x24 (36) bytes
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

struct D2IsoSortGridStrc // Embedded at D2ViewportStrc + 0x38 (size = 0xEA84)
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

struct D2ViewportStrc // Total size: 0xEABC bytes (60,092 bytes)
{
    uint32_t              dwFlags;                    // +0x0000 (D2ViewportFlags)
    int32_t               nScreenLeft;                // +0x0004
    int32_t               nScreenTop;                 // +0x0008
    int32_t               nScreenRight;               // +0x000C
    int32_t               nScreenBottom;              // +0x0010
    int32_t               nClipLeft;                  // +0x0014 (nScreenLeft - 160)
    int32_t               nClipTop;                   // +0x0018 (nScreenTop - 160)
    int32_t               nClipRight;                 // +0x001C (nScreenRight + 160)
    int32_t               nClipBottom;                // +0x0020 (nScreenBottom + 160)
    int32_t               nViewOffsetX;               // +0x0024 (nCameraIsoX: top-left camera world iso X)
    int32_t               nViewOffsetY;               // +0x0028 (nCameraIsoY: top-left camera world iso Y)
    int32_t               nCameraTargetIsoX;          // +0x002C
    int32_t               nCameraTargetIsoY;          // +0x0030
    int32_t               nShakePacked;               // +0x0034
    D2IsoSortGridStrc     tSortGrid;                  // +0x0038 .. +0xEABB
};
static_assert(sizeof(D2ViewportStrc) == 0xEABC, "D2ViewportStrc must match retail 0xEABC-byte layout");

// =============================================================================
// Viewport & Camera Globals in D2Client.dll
// =============================================================================
extern void*           gpClientViewport;    // 0x6FBCC1E0 (D2ViewportStrc*)
extern D2UnitStrc*     gpClientPlayer;      // 0x6FBCC3D0
extern D2DrlgActStrc*  gpClientAct;         // 0x6FBCC02C
extern uint32_t        gdwClientGameTick;   // 0x6FBB32B0
extern int32_t         gnCameraShakeOffsetX;// 0x6FBCC3BC
extern int32_t         gnCameraShakeOffsetY;// 0x6FBCC3C0
extern int32_t         gnCameraScreenSkewX; // 0x6FBC21D0
extern int32_t         gnCameraScreenSkewY; // 0x6FBC21CC

struct D2WallViewportStrc;
extern void __fastcall WALL2_FreeSortGridCells(D2WallViewportStrc* pViewport);
extern void __fastcall WALL2_BuildViewportRoomGrid(D2WallViewportStrc* pViewport, int32_t bSkipUnits);
extern void __fastcall ENV_RenderGameViewport(uint32_t* pViewport);

// =============================================================================
// SECTION 1: Viewport Lifecycle & Coordinate Transforms (GAME/View.cpp)
// =============================================================================

// 0x6FB6A3D0
void __fastcall VIEW_SetShakeOffsets(D2ViewportStrc* pViewport, int32_t nShakeX, int32_t nShakeY)
{
    gnCameraShakeOffsetX = nShakeX;
    gnCameraShakeOffsetY = nShakeY;
    if (pViewport != nullptr)
    {
        pViewport->dwFlags |= VIEWFLAG_DIRTY;
    }
}

// 0x6FB6A3F0
void __fastcall VIEW_StartCameraShake(D2ViewportStrc* pViewport, int32_t nDurationFrames, int32_t nIntensityType)
{
    if (pViewport == nullptr)
    {
        return;
    }
    const int32_t nCurType = (pViewport->nShakePacked >> 16) & 0xFFFF;
    if (!(pViewport->dwFlags & VIEWFLAG_SHAKING) || nIntensityType >= nCurType)
    {
        pViewport->dwFlags |= VIEWFLAG_SHAKING;
        pViewport->nShakePacked = (nDurationFrames & 0xFFFF) | ((nIntensityType & 0xFFFF) << 16);
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
    *pWidth = pViewport->nScreenRight - pViewport->nScreenLeft;
    *pHeight = pViewport->nScreenBottom - pViewport->nScreenTop;
}

// 0x6FB6A480
void __fastcall VIEW_ScreenToWorldCoords(D2ViewportStrc* pViewport, int32_t nScreenX, int32_t nScreenY, int32_t* pWorldX, int32_t* pWorldY)
{
    int32_t nIsoX = nScreenX;
    int32_t nIsoY = nScreenY;

    if (D2GFX_CheckPerspectiveMode())
    {
        D2GFX_ScalePerspectivePosition(nScreenX, nScreenY, 0, &nIsoX, &nIsoY, 1);
    }
    else if (pViewport != nullptr)
    {
        nIsoX += pViewport->nViewOffsetX - pViewport->nScreenLeft;
        nIsoY += pViewport->nViewOffsetY - pViewport->nScreenTop;
    }

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
    pViewport->nClipLeft = nLeft - 160;
    pViewport->nClipTop = nTop - 160;
    pViewport->nClipRight = nRight + 160;
    pViewport->nClipBottom = nBottom + 160;

    const int32_t nHalfWidth = (nRight - nLeft) / 2;
    const int32_t nHalfHeight = (nBottom - nTop) / 2;
    pViewport->nViewOffsetX = pViewport->nCameraTargetIsoX + gnCameraShakeOffsetX - nHalfWidth;
    pViewport->nViewOffsetY = pViewport->nCameraTargetIsoY + gnCameraShakeOffsetY - nHalfHeight;
    pViewport->dwFlags |= VIEWFLAG_DIRTY;
}

// 0x6FB6A5B0
D2ViewportStrc* __fastcall VIEW_AllocViewport(void)
{
    D2ViewportStrc* pViewport = static_cast<D2ViewportStrc*>(
        FOG_AllocClientMemory(sizeof(D2ViewportStrc), "..\\Source\\D2Client\\GAME\\View.cpp", 112, 0));
    memset(pViewport, 0, sizeof(D2ViewportStrc));

    int32_t nWidth = 800;
    int32_t nHeight = 600;
    WINDOW_GetDimensions(&nWidth, &nHeight);
    if (nWidth <= 0) nWidth = 800;
    if (nHeight <= 48) nHeight = 600;
    VIEW_SetViewportRect(pViewport, 0, 0, nWidth, nHeight - 48);
    return pViewport;
}

// 0x6FB6A560
void __fastcall VIEW_FreeViewport(D2ViewportStrc* pViewport)
{
    if (pViewport == nullptr)
    {
        return;
    }
    WALL2_FreeSortGridCells(reinterpret_cast<D2WallViewportStrc*>(pViewport));
    FOG_FreeClientMemory(pViewport, "..\\Source\\D2Client\\GAME\\View.cpp", 145, 0);
}

// 0x6FB6B4F0
void __fastcall VIEW_UpdateCameraShake(D2ViewportStrc* pViewport)
{
    if (pViewport == nullptr || !(pViewport->dwFlags & VIEWFLAG_SHAKING))
    {
        return;
    }

    int32_t nFrames = pViewport->nShakePacked & 0xFFFF;
    const int32_t nType = (pViewport->nShakePacked >> 16) & 0xFFFF;
    if (nFrames <= 0)
    {
        pViewport->dwFlags &= ~VIEWFLAG_SHAKING;
        pViewport->nShakePacked = 0;
        gnCameraShakeOffsetX = 0;
        gnCameraShakeOffsetY = 0;
        pViewport->dwFlags |= VIEWFLAG_DIRTY;
        return;
    }

    nFrames -= 1;
    pViewport->nShakePacked = (nFrames & 0xFFFF) | ((nType & 0xFFFF) << 16);
    pViewport->dwFlags |= VIEWFLAG_DIRTY;
}

// =============================================================================
// SECTION 2: Viewport Room Grid & Isometric Render Passes (GAME/View.cpp)
// =============================================================================

// 0x6FB6B5E0
void __fastcall VIEW_UpdateViewportVisibility(D2ViewportStrc* pViewport, D2UnitStrc* pPlayer)
{
    if (pViewport == nullptr)
    {
        return;
    }
    (void)pPlayer;
    WALL2_BuildViewportRoomGrid(reinterpret_cast<D2WallViewportStrc*>(pViewport), 0);
    pViewport->dwFlags &= ~VIEWFLAG_DIRTY;
}

// 0x6FB6BD60
void __fastcall VIEW_UpdateRoomLightColor(D2VisibleRoomCellStrc* pRoomCell)
{
    if (pRoomCell != nullptr && !pRoomCell->bLightComputed)
    {
        pRoomCell->dwLightColor = 0xFFFFFFFF;
        pRoomCell->bLightComputed = 1;
    }
}

// 0x6FB6C090 / 0x6FB6C250
void __fastcall VIEW_DrawFloorTiles(D2ViewportStrc* pViewport)
{
    (void)pViewport;
}

// 0x6FB6C5F0 / 0x6FB6C660
void __fastcall VIEW_DrawShadows(D2ViewportStrc* pViewport)
{
    (void)pViewport;
}

// 0x6FB6BDF0
void __fastcall VIEW_DrawWallTiles(D2ViewportStrc* pViewport, int32_t nWallLayerMask)
{
    (void)pViewport;
    (void)nWallLayerMask;
}

// 0x6FB6C280
void __fastcall VIEW_DrawUnitsAndRoofs(D2ViewportStrc* pViewport)
{
    (void)pViewport;
}

// 0x6FB6B630
void __fastcall VIEW_RenderViewport(D2ViewportStrc* pViewport)
{
    if (pViewport == nullptr)
    {
        return;
    }
    ENV_RenderGameViewport(reinterpret_cast<uint32_t*>(pViewport));
}

// =============================================================================
// SECTION 3: Camera Tracking & DRLG Room Streaming Pipeline
//            (GAME/Game.cpp & UNIT/Player.cpp)
// =============================================================================

// 0x6FB0DB10 (UNIT/Player.cpp)
void __fastcall PLAYER_UpdateCameraOffsets(void)
{
}

// 0x6FB2CA10 (GAME/Game.cpp)
void __fastcall GAME_RenderFrame(int32_t bSkipWorldRender)
{
    D2ViewportStrc* pViewport = static_cast<D2ViewportStrc*>(gpClientViewport);
    if (gpClientPlayer == nullptr || pViewport == nullptr)
    {
        return;
    }

    VIEW_UpdateCameraShake(pViewport);

    int32_t nTargetIsoX = UNITS_GetClientCoordX(gpClientPlayer);
    int32_t nTargetIsoY = UNITS_GetClientCoordY(gpClientPlayer);
    if (nTargetIsoX == 0 && nTargetIsoY == 0)
    {
        const int32_t nPrecisionX = UNITS_GetPrecisionX(gpClientPlayer);
        const int32_t nPrecisionY = UNITS_GetPrecisionY(gpClientPlayer);
        DUNGEON_GameCoordsToSubtileDrawPosition(
            nPrecisionX >> 16,
            nPrecisionY >> 16,
            &nTargetIsoX,
            &nTargetIsoY);
    }

    nTargetIsoX += gnCameraScreenSkewX;
    nTargetIsoY += gnCameraScreenSkewY;

    const int32_t nHalfWidth = (pViewport->nScreenRight - pViewport->nScreenLeft) / 2;
    const int32_t nHalfHeight = (pViewport->nScreenBottom - pViewport->nScreenTop) / 2;
    const int32_t nNewOffsetX = nTargetIsoX + gnCameraShakeOffsetX - nHalfWidth;
    const int32_t nNewOffsetY = nTargetIsoY + gnCameraShakeOffsetY - nHalfHeight;

    if (pViewport->nCameraTargetIsoX != nTargetIsoX ||
        pViewport->nCameraTargetIsoY != nTargetIsoY ||
        pViewport->nViewOffsetX != nNewOffsetX ||
        pViewport->nViewOffsetY != nNewOffsetY)
    {
        pViewport->nCameraTargetIsoX = nTargetIsoX;
        pViewport->nCameraTargetIsoY = nTargetIsoY;
        pViewport->nViewOffsetX = nNewOffsetX;
        pViewport->nViewOffsetY = nNewOffsetY;
        pViewport->dwFlags |= VIEWFLAG_DIRTY;
    }

    VIEW_UpdateViewportVisibility(pViewport, gpClientPlayer);
    PLAYER_UpdateCameraOffsets();

    if (!bSkipWorldRender)
    {
        VIEW_RenderViewport(pViewport);
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
    if (gpClientAct->pRoom != nullptr)
    {
        DUNGEON_AnimateTiles(gpClientAct->pRoom);
    }

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
        DUNGEON_InitRoomTileAnimation(gpClientAct, pCurrentRoom, pNextRoom);
    }

    if (pCurrentRoom != nullptr)
    {
        // D2Common.#10200: DUNGEON_CheckLOSDraw
        DUNGEON_CheckLOSDraw(pCurrentRoom);
    }
}
