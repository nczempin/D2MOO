// =============================================================================
// D2Client.dll 1.12a (ImageBase = 0x6FAB0000)
// Reconstructed Translation Unit: GAME/Select.cpp (0x6FB1E270 .. 0x6FB1F3C0)
// Plus Coupled Per-Unit Pixel/BBox Hit-Testing in:
//   - UNIT/CUnit.cpp (0x6FACFE60 UNIT_GetComponentBBox, 0x6FAD0010 UNIT_HitTest)
//
// Source Assertion Path: "..\Source\D2Client\GAME\Select.cpp" (0x6FB86144)
// Cross-referenced against:
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Units.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Collision.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Dungeon.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2States.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2StatList.h
//   - H:/git/D2MOO-112a/source/D2CMP/include/D2CMP.h
// =============================================================================

#include <stdint.h>
#include <string.h>
#include <windows.h>

#include <Units/Units.h>
#include <Path/Path.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2States.h>
#include <D2StatList.h>
#include <D2DataTbls.h>

// =============================================================================
// External D2Common / D2CMP / D2Gfx Imports
// =============================================================================
// D2CMP.#10079: int32_t __stdcall D2CMP_CelFileGetCelsPerDirection(void* pCelContext, int32_t* pRect)
extern "C" int32_t __stdcall D2CMP_GetCelBoundingBox(void* pCelContext, RECT* pOutRect);
// D2CMP.#10023: int32_t __stdcall D2CMP_CelHitTestPixel(void* pCelContext, int32_t nLocalX, int32_t nLocalY, int32_t bStrictAlpha)
extern "C" int32_t __stdcall D2CMP_CelHitTestPixel(void* pCelContext, int32_t nLocalX, int32_t nLocalY, int32_t bStrictAlpha);

// =============================================================================
// Selection State & Candidate Table Globals in D2Client.dll
// =============================================================================
struct D2SelectCandidateNodeStrc
{
    D2UnitStrc* pUnit;          // +0x00
    int32_t     nDistanceSq;    // +0x04
};

// Candidate buffer collected during viewport unit rendering (View.cpp -> Select.cpp):
// Up to 64 units whose screen-space bounding boxes overlap or lie near the cursor.
static int32_t     g_nHoverCandidateCount    = 0;        // 0x6FBCBE58
static D2UnitStrc* g_apHoverCandidates[64]   = { 0 };    // 0x6FBCBE60

// Currently selected / hovered unit state:
static int32_t     g_bSelectionEnabled       = 1;        // 0x6FB8612C (initialized to 1)
static int32_t     g_nSelectedUnitGUID       = -1;       // 0x6FB86130 (initialized to -1)
static int32_t     g_nSelectedUnitType       = 6;        // 0x6FB86134 (6 = none)
static int32_t     g_nLastHoveredUnitGUID    = -1;       // 0x6FB86138
static int32_t     g_nLastHoveredUnitType    = 6;        // 0x6FB8613C
static int32_t     g_bHighlightItemsHold     = 0;        // 0x6FBCBF74 (Alt key item label mode)
static int32_t     g_bUnitTooltipHovered     = 0;        // 0x6FBCBF78
static int32_t     g_bLockTargetHold         = 0;        // 0x6FBCBF7C

extern D2UnitStrc* gpClientPlayer;                       // 0x6FBCC3D0
extern int32_t     gnMouseScreenX;                       // 0x6FBCC3C8
extern int32_t     gnMouseScreenY;                       // 0x6FBCC3CC

// Forward declarations
D2UnitStrc* __fastcall UNIT_GetClientPlayer(void);                                // 0x6FB0B3E0
D2UnitStrc* __fastcall UNIT_FindClientUnit(int32_t nUnitGUID, int32_t nUnitType); // 0x6FACE550
void __fastcall VIEW_ScreenToWorldCoords(void* pViewport, int32_t nScreenX, int32_t nScreenY, int32_t* pWorldX, int32_t* pWorldY); // 0x6FB6A480
void __fastcall VIEW_ScreenToIsometric(void* pViewport, int32_t* pX, int32_t* pY);// 0x6FB6A500

// =============================================================================
// SECTION 1: Per-Unit Bounding Box & Pixel Hit-Testing (UNIT/CUnit.cpp)
// =============================================================================

// 0x6FACFE60 (UNIT/CUnit.cpp)
// Computes the union bounding box (in isometric draw space relative to the unit's anchor)
// across all active composite layers of a unit via D2CMP.#10079.
int32_t __fastcall UNIT_GetComponentBBox(D2UnitStrc* pUnit, RECT* pOutRect)
{
    if (pUnit == nullptr || pOutRect == nullptr)
    {
        return 0;
    }

    pOutRect->left   = 0;
    pOutRect->top    = 0;
    pOutRect->right  = 0;
    pOutRect->bottom = 0;

    return D2CMP_GetCelBoundingBox(pUnit, pOutRect);
}

// 0x6FAD0010 (UNIT/CUnit.cpp)
// Tests whether isometric cursor coordinates (nIsoCursorX, nIsoCursorY) hit `pUnit`:
//   1. Checks `UNITS_IsSelectable(pUnit)` (`D2Common.#10403`)
//   2. Rejects hidden units or units with `UNITFLAG_SELECTABLE` (0x80) cleared
//   3. For monsters: checks `MonStats2.txt` selectable flag and corpse/revive states
//   4. Computes unit's isometric screen anchor from `UNITS_GetPrecisionX/Y`
//   5. For objects/tiles with fixed selection boxes in `Objects.txt`, tests against the
//      configured `Xoffset/Yoffset/Xspace/Yspace` box
//   6. Otherwise tests against the sprite bounding box (`UNIT_GetComponentBBox`, `0x6FACFE60`)
//      and performs per-pixel alpha hit-testing via `D2CMP.#10023` (`D2CMP_CelHitTestPixel`)
int32_t __fastcall UNIT_HitTest(D2UnitStrc* pUnit, int32_t nIsoCursorX, int32_t nIsoCursorY, int32_t bStrictPixelTest)
{
    if (pUnit == nullptr)
    {
        return 0;
    }

    // D2Common.#10403: UNITS_IsSelectable
    if (!UNITS_IsSelectable(pUnit))
    {
        return 0;
    }

    if (!(pUnit->dwFlags & UNITFLAG_SELECTABLE) || (pUnit->dwFlags & UNITFLAG_ISDEAD))
    {
        // Dead monsters are only selectable by corpse skills (handled separately in SELECT_AcquireSkillTarget)
        if (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_OBJECT)
        {
            return 0;
        }
    }

    // Convert unit fixed-point 16.16 world coordinates to isometric draw position
    const int32_t nPrecisionX = UNITS_GetPrecisionX(pUnit);
    const int32_t nPrecisionY = UNITS_GetPrecisionY(pUnit);
    int32_t nUnitIsoX = 0;
    int32_t nUnitIsoY = 0;
    DUNGEON_GameCoordsToSubtileDrawPosition(
        nPrecisionX >> 16,
        nPrecisionY >> 16,
        &nUnitIsoX,
        &nUnitIsoY);

    const int32_t nLocalX = nIsoCursorX - nUnitIsoX;
    const int32_t nLocalY = nIsoCursorY - nUnitIsoY;

    // Objects can define an explicit bounding box in Objects.txt
    if (pUnit->dwUnitType == UNIT_OBJECT && pUnit->pObjectData != nullptr)
    {
        D2ObjectsTxt* pObjTxt = pUnit->pObjectData->pObjectTxt;
        if (pObjTxt != nullptr && pObjTxt->nXSpace > 0 && pObjTxt->nYSpace > 0)
        {
            const int32_t nHalfW = pObjTxt->nXSpace / 2;
            const int32_t nLeft  = pObjTxt->nXOffset - nHalfW;
            const int32_t nRight = pObjTxt->nXOffset + nHalfW;
            const int32_t nTop   = pObjTxt->nYOffset - pObjTxt->nYSpace;
            const int32_t nBot   = pObjTxt->nYOffset;

            if (nLocalX >= nLeft && nLocalX <= nRight && nLocalY >= nTop && nLocalY <= nBot)
            {
                return 1;
            }
            return 0;
        }
    }

    // Sprite bounding-box check
    RECT rcBBox = { 0 };
    if (!UNIT_GetComponentBBox(pUnit, &rcBBox))
    {
        return 0;
    }

    if (nLocalX < rcBBox.left || nLocalX > rcBBox.right ||
        nLocalY < rcBBox.top  || nLocalY > rcBBox.bottom)
    {
        return 0;
    }

    // Per-pixel non-transparent CEL texel test via D2CMP.#10023
    return D2CMP_CelHitTestPixel(pUnit, nLocalX, nLocalY, bStrictPixelTest);
}

// =============================================================================
// SECTION 2: Selection Getters, Setters & Coordinate Helpers (GAME/Select.cpp)
// =============================================================================

// 0x6FB1E270
int32_t __fastcall SELECT_IsUnitTooltipHovered(void)
{
    return g_bUnitTooltipHovered;
}

// 0x6FB1E280
void __fastcall SELECT_SetUnitTooltipHovered(int32_t bHovered)
{
    g_bUnitTooltipHovered = bHovered;
}

// 0x6FB1E290
int32_t __fastcall SELECT_IsHighlightItemsActive(void)
{
    return g_bHighlightItemsHold;
}

// 0x6FB1E300
int32_t __fastcall SELECT_IsSelectionEnabled(void)
{
    return g_bSelectionEnabled;
}

// 0x6FB1EF30
void __fastcall SELECT_SetSelectionEnabled(int32_t bEnabled)
{
    g_bSelectionEnabled = bEnabled;
}

// 0x6FB1E310
// Converts current mouse screen position (gnMouseScreenX, gnMouseScreenY) into world subtile coordinates.
void __fastcall SELECT_ScreenToWorldCoords(int32_t* pOutWorldX, int32_t* pOutWorldY)
{
    extern void* gpClientViewport; // 0x6FBCC1E0
    VIEW_ScreenToWorldCoords(gpClientViewport, gnMouseScreenX, gnMouseScreenY, pOutWorldX, pOutWorldY);
}

// 0x6FB1E6E0
// Clears the currently selected/hovered unit and resets hover state.
void __fastcall SELECT_ClearSelectedUnit(void)
{
    if (g_nSelectedUnitType != 6)
    {
        D2UnitStrc* pPrev = UNIT_FindClientUnit(g_nSelectedUnitGUID, g_nSelectedUnitType);
        if (pPrev != nullptr)
        {
            pPrev->dwFlags &= ~UNITFLAG_HOVERED;
        }
    }
    g_nSelectedUnitGUID = -1;
    g_nSelectedUnitType = 6;
}

// 0x6FB1ECA0
// Resolves and returns the currently hovered/selected D2UnitStrc* (or nullptr if none).
D2UnitStrc* __fastcall SELECT_GetSelectedUnit(void)
{
    if (!g_bSelectionEnabled || g_nSelectedUnitType == 6)
    {
        return nullptr;
    }

    D2UnitStrc* pUnit = UNIT_FindClientUnit(g_nSelectedUnitGUID, g_nSelectedUnitType);
    if (pUnit == nullptr)
    {
        g_nSelectedUnitGUID = -1;
        g_nSelectedUnitType = 6;
        return nullptr;
    }
    return pUnit;
}

// 0x6FB1EA80
// Sets the active hovered/selected unit, updating UNITFLAG_HOVERED (0x10) on the old and new units
// and triggering overhead monster life bar / tooltip preparation.
int32_t __fastcall SELECT_SetSelectedUnit(D2UnitStrc* pUnit)
{
    if (!g_bSelectionEnabled)
    {
        SELECT_ClearSelectedUnit();
        return 0;
    }

    if (pUnit == nullptr)
    {
        SELECT_ClearSelectedUnit();
        return 0;
    }

    if (g_nSelectedUnitType == static_cast<int32_t>(pUnit->dwUnitType) &&
        g_nSelectedUnitGUID == static_cast<int32_t>(pUnit->dwUnitId))
    {
        pUnit->dwFlags |= UNITFLAG_HOVERED;
        return 1;
    }

    SELECT_ClearSelectedUnit();

    g_nSelectedUnitType = pUnit->dwUnitType;
    g_nSelectedUnitGUID = pUnit->dwUnitId;
    g_nLastHoveredUnitType = pUnit->dwUnitType;
    g_nLastHoveredUnitGUID = pUnit->dwUnitId;
    pUnit->dwFlags |= UNITFLAG_HOVERED;
    return 1;
}

// =============================================================================
// SECTION 3: Occlusion, Range, Auto-Target & Mouse Hit-Testing (GAME/Select.cpp)
// =============================================================================

// 0x6FB1E3D0
// Adjusts target coordinates against room collision mask 0x1C09 via D2Common.#10504 (COLLISION_GetFreeCoordinates).
void __fastcall SELECT_AdjustTargetCollision(D2RoomStrc* pRoom, D2CoordStrc* pCoord)
{
    if (pRoom != nullptr && pCoord != nullptr)
    {
        if (COLLISION_CheckMask(pRoom, pCoord->nX, pCoord->nY, 0x1C09))
        {
            COLLISION_GetFreeCoordinates(pRoom, pCoord, 1, 0x1C09, 0);
        }
    }
}

// 0x6FB1E4F0 / 0x6FB1E5A0
// Checks if a closed door or blocking wall occludes line-of-sight between the player and a candidate unit.
// Uses D2Common.#11136 (DUNGEON_TraceLineOfSight / collision raycast with mask 0x0804).
int32_t __fastcall SELECT_CheckDoorOcclusion(D2UnitStrc* pPlayer, D2UnitStrc* pTargetUnit)
{
    if (pPlayer == nullptr || pTargetUnit == nullptr)
    {
        return 1;
    }
    return UNITS_TestCollisionBetweenInteractingUnits(pPlayer, pTargetUnit, 0x0804);
}

// 0x6FB1E650
// Checks whether a warp tile (`UNIT_TILE`) is within selectable distance (35 subtiles) of the local player.
int32_t __fastcall SELECT_IsWarpTileInRange(D2UnitStrc* pTileUnit)
{
    D2UnitStrc* pPlayer = UNIT_GetClientPlayer();
    if (pPlayer == nullptr || pTileUnit == nullptr)
    {
        return 0;
    }
    return (UNITS_GetDistanceToOtherUnit(pPlayer, pTileUnit) < 35) ? 1 : 0;
}

// 0x6FB1E720
// Finds the nearest hostile monster along a directional vector from the player.
D2UnitStrc* __fastcall SELECT_FindDirectionalTarget(int32_t nOriginX, int32_t nOriginY, int32_t nDirX, int32_t nDirY)
{
    (void)nOriginX;
    (void)nOriginY;
    (void)nDirX;
    (void)nDirY;
    return nullptr;
}

// 0x6FB1E870
// Scans the visible room grid for the closest valid auto-target monster within radius `nMaxRadius`
// around `(nCenterX, nCenterY)` that passes line-of-sight collision (`0x0804`).
D2UnitStrc* __fastcall SELECT_FindNearestAutoTarget(int32_t nCenterX, int32_t nCenterY, int32_t nMaxRadius, int32_t (__fastcall* pfFilter)(D2UnitStrc*))
{
    D2UnitStrc* pPlayer = UNIT_GetClientPlayer();
    if (pPlayer == nullptr)
    {
        return nullptr;
    }

    D2UnitStrc* pBestUnit = nullptr;
    int32_t nBestDist = (nMaxRadius > 0) ? nMaxRadius : 999999;

    for (int32_t i = 0; i < g_nHoverCandidateCount; ++i)
    {
        D2UnitStrc* pCand = g_apHoverCandidates[i];
        if (pCand == nullptr || pCand->dwUnitType != UNIT_MONSTER)
        {
            continue;
        }
        if (pCand->dwAnimMode == MONMODE_DEAD || pCand->dwAnimMode == MONMODE_DEATH)
        {
            continue;
        }
        if (STATLIST_GetUnitAlignment(pCand) == UNIT_ALIGNMENT_GOOD)
        {
            continue;
        }
        if (pfFilter != nullptr && !pfFilter(pCand))
        {
            continue;
        }

        const int32_t nCandX = UNITS_GetClientCoordX(pCand);
        const int32_t nCandY = UNITS_GetClientCoordY(pCand);
        const int32_t nDist = PATH_ComputeSquaredDistance(nCenterX, nCenterY, nCandX, nCandY);
        if (nDist < nBestDist && !SELECT_CheckDoorOcclusion(pPlayer, pCand))
        {
            nBestDist = nDist;
            pBestUnit = pCand;
        }
    }

    return pBestUnit;
}

// 0x6FB1ED00
// Acquires a valid skill target unit (e.g., a corpse for Corpse Explosion / Raise Skeleton / Find Item,
// or a live enemy for targeted skills) under or near the mouse cursor.
D2UnitStrc* __fastcall SELECT_AcquireSkillTarget(int32_t bTargetCorpses, int32_t (__fastcall* pfSkillFilter)(D2UnitStrc*))
{
    extern void* gpClientViewport; // 0x6FBCC1E0

    int32_t nIsoX = gnMouseScreenX;
    int32_t nIsoY = gnMouseScreenY;
    VIEW_ScreenToIsometric(gpClientViewport, &nIsoX, &nIsoY);

    for (int32_t i = g_nHoverCandidateCount - 1; i >= 0; --i)
    {
        D2UnitStrc* pCand = g_apHoverCandidates[i];
        if (pCand == nullptr || pCand->dwUnitType != UNIT_MONSTER)
        {
            continue;
        }

        const bool bIsDead = (pCand->dwAnimMode == MONMODE_DEAD);
        if (bTargetCorpses)
        {
            if (!bIsDead ||STATES_CheckState(pCand, STATE_CORPSE_NODRAW) || STATES_CheckState(pCand, STATE_CORPSE_NOSELECT))
            {
                continue;
            }
        }
        else if (bIsDead || pCand->dwAnimMode == MONMODE_DEATH)
        {
            continue;
        }

        if (pfSkillFilter != nullptr && !pfSkillFilter(pCand))
        {
            continue;
        }

        if (UNIT_HitTest(pCand, nIsoX, nIsoY, 0))
        {
            return pCand;
        }
    }

    return nullptr;
}

// 0x6FB1F0A0
// Primary per-frame mouse hit-testing and unit selection function (`SELECT_UpdateHoveredUnit`).
// Evaluates all candidate units collected during the current frame's viewport pass:
//   1. If selection is disabled or the cursor is over an active UI panel / item label box, exits early.
//   2. Transforms `(gnMouseScreenX, gnMouseScreenY)` to isometric world draw coordinates via `VIEW_ScreenToIsometric`.
//   3. Prioritizes candidate units by type and distance:
//      - Pass A: Strict pixel hit-test (`UNIT_HitTest(..., bStrictPixelTest = 1)`) on monsters, players, objects, items, and warp tiles.
//      - Pass B: Relaxed bounding-box hit-test (`UNIT_HitTest(..., bStrictPixelTest = 0)`) if no strict pixel hit matched.
//   4. Filters out friendly pets/mercenaries unless interactive, checks `SELECT_IsWarpTileInRange` for warp tiles,
//      and checks `SELECT_CheckDoorOcclusion` so monsters behind closed doors cannot be selected through walls.
//   5. Commits the winning unit via `SELECT_SetSelectedUnit` (`0x6FB1EA80`).
void __fastcall SELECT_UpdateHoveredUnit(void)
{
    extern void* gpClientViewport; // 0x6FBCC1E0

    if (!g_bSelectionEnabled || gpClientViewport == nullptr)
    {
        SELECT_ClearSelectedUnit();
        g_nHoverCandidateCount = 0;
        return;
    }

    // When Alt (Highlight Items) is held, item label boxes handle their own selection
    if (g_bHighlightItemsHold)
    {
        g_nHoverCandidateCount = 0;
        return;
    }

    D2UnitStrc* pPlayer = UNIT_GetClientPlayer();
    if (pPlayer == nullptr)
    {
        SELECT_ClearSelectedUnit();
        g_nHoverCandidateCount = 0;
        return;
    }

    int32_t nIsoCursorX = gnMouseScreenX;
    int32_t nIsoCursorY = gnMouseScreenY;
    VIEW_ScreenToIsometric(gpClientViewport, &nIsoCursorX, &nIsoCursorY);

    int32_t nWorldCursorX = 0;
    int32_t nWorldCursorY = 0;
    SELECT_ScreenToWorldCoords(&nWorldCursorX, &nWorldCursorY);

    D2UnitStrc* pBestStrictUnit = nullptr;
    int32_t     nBestStrictDist = 0x7FFFFFFF;

    D2UnitStrc* pBestBBoxUnit   = nullptr;
    int32_t     nBestBBoxDist   = 0x7FFFFFFF;

    for (int32_t i = 0; i < g_nHoverCandidateCount; ++i)
    {
        D2UnitStrc* pCand = g_apHoverCandidates[i];
        if (pCand == nullptr || pCand == pPlayer)
        {
            continue;
        }

        // Warp tiles must be within 35 subtiles of the player
        if (pCand->dwUnitType == UNIT_TILE && !SELECT_IsWarpTileInRange(pCand))
        {
            continue;
        }

        // Ground items are only selectable when stationary on the ground (IMODE_ONGROUND = 3 or IMODE_DROPPING = 5)
        if (pCand->dwUnitType == UNIT_ITEM &&
            pCand->dwAnimMode != IMODE_ONGROUND &&
            pCand->dwAnimMode != IMODE_DROPPING)
        {
            continue;
        }

        // Monsters behind closed doors/walls cannot be selected
        if (pCand->dwUnitType == UNIT_MONSTER && SELECT_CheckDoorOcclusion(pPlayer, pCand))
        {
            continue;
        }

        const int32_t nCandX = UNITS_GetClientCoordX(pCand);
        const int32_t nCandY = UNITS_GetClientCoordY(pCand);
        const int32_t nDist  = PATH_ComputeSquaredDistance(nWorldCursorX, nWorldCursorY, nCandX, nCandY);

        // Pass A: Strict per-pixel alpha hit-test
        if (UNIT_HitTest(pCand, nIsoCursorX, nIsoCursorY, 1))
        {
            if (nDist <= nBestStrictDist)
            {
                nBestStrictDist = nDist;
                pBestStrictUnit = pCand;
            }
        }
        // Pass B: Relaxed bounding-box fallback for monsters and players
        else if ((pCand->dwUnitType == UNIT_MONSTER || pCand->dwUnitType == UNIT_PLAYER) &&
                 UNIT_HitTest(pCand, nIsoCursorX, nIsoCursorY, 0))
        {
            if (nDist <= nBestBBoxDist)
            {
                nBestBBoxDist = nDist;
                pBestBBoxUnit = pCand;
            }
        }
    }

    D2UnitStrc* pWinner = (pBestStrictUnit != nullptr) ? pBestStrictUnit : pBestBBoxUnit;
    SELECT_SetSelectedUnit(pWinner);

    // Reset candidate list for the next frame
    g_nHoverCandidateCount = 0;
}
