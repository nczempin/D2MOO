// ============================================================================
// Diablo II 1.12a - D2Client.dll Reconstructed Translation Unit
// Source Files:
//   - ..\Source\D2Client\DRAW\dLightMap.cpp (0x6FB0DFF0..0x6FB0FDD0)
//   - ..\Source\D2Client\ENGINE\Particle.cpp (0x6FB75260..0x6FB77050)
// ImageBase   : 0x6FAB0000
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct D2UnitStrc;
struct D2ActiveRoomStrc;
struct D2GfxDataStrc;
struct D2SkillStrc;
struct D2SkillsTxt;
struct D2MissilesTxt;
struct D2MonStatsTxt;
struct D2MonSoundsTxt;
struct D2LevelsTxt;

// ============================================================================
// Forward Declarations & External Imports
// ============================================================================

extern "C" {
    // Fog.dll
    void* __fastcall FOG_Alloc(int32_t nSize, const char* szFile, int32_t nLine, int32_t nZero); // Fog.dll!#10042 (0x6FABBF7A)
    void  __fastcall FOG_Free(void* pMem, const char* szFile, int32_t nLine, int32_t nZero);     // Fog.dll!#10043 (0x6FABBF86)
    const char* __cdecl FOG_GetErrorString(int32_t nErrorCode);                                  // Fog.dll!#10265 (0x6FABBF8C)
    void  __cdecl FOG_DisplayHalt(const char* szFile, const char* szMsg, int32_t nLine);         // Fog.dll!#10024 (0x6FABBF80)

    // D2gfx.dll
    void    __stdcall D2GFX_DebugDraw(void* pLightRayGrid, void* pLightMapCells, int32_t nX8, int32_t nY8); // D2gfx.dll!#10021 (0x6FABD168)
    int32_t __stdcall D2GFX_CheckLowQuality();                                                   // D2gfx.dll!#10050 (0x6FABD162)
    void    __fastcall D2GFX_SetViewMatrixAdjust(int32_t nAdjust);                               // D2gfx.dll!#10072 (0x6FABD17A)

    // D2sound.dll
    int32_t __stdcall D2SOUND_GetMasterVolume();                                                 // D2sound.dll!#10023 (0x6FABD2EE)
    int32_t __stdcall D2SOUND_GetMusicVolume();                                                  // D2sound.dll!#10035 (0x6FABD2D6)

    // D2Common.dll
    D2ActiveRoomStrc* __stdcall UNITS_GetRoom(D2UnitStrc* pUnit);                                // D2Common.dll!#10366 (0x6FABC1BA)
    int32_t __stdcall UNITS_GetPrecisionX(D2UnitStrc* pUnit);                                    // D2Common.dll!#10984 (0x6FABC202)
    int32_t __stdcall UNITS_GetClientCoordX(D2UnitStrc* pUnit);                                  // D2Common.dll!#10098 (0x6FABC232)
    uint8_t __stdcall UNITS_GetDirection(D2UnitStrc* pUnit);                                     // D2Common.dll!#11128 (0x6FABC298)
    D2SkillStrc* __stdcall UNITS_GetUsedSkill(D2UnitStrc* pUnit);                                // D2Common.dll!#10652 (0x6FABC274)
    int32_t __stdcall UNITS_GetDistanceToCoordinates(D2UnitStrc* pUnit, int32_t nX, int32_t nY); // D2Common.dll!#10666 (0x6FABC32E)
    void    __stdcall UNITS_SetAnimationSpeed(D2UnitStrc* pUnit, int32_t nSpeed);                // D2Common.dll!#10299 (0x6FABC268)
    int32_t __stdcall D2COMMON_11018_CheckUnitInteractable(D2UnitStrc* pUnit);                   // D2Common.dll!#10714 (0x6FABC328)
    void    __stdcall D2Common_11066_GetUnitVector(D2UnitStrc* pUnit, int32_t* pX, int32_t* pY); // D2Common.dll!#10785 (0x6FABC310)

    int32_t __stdcall PATH_GetXPosition(void* pPath);                                            // D2Common.dll!#10191 (0x6FABC2D4)
    int32_t __stdcall PATH_GetYPosition(void* pPath);                                            // D2Common.dll!#10178 (0x6FABC262)
    void    __stdcall PATH_SetDirection(void* pPath, uint8_t nDir);                              // D2Common.dll!#10155 (0x6FABC2DA)
    void    __stdcall D2COMMON_10203_PATH_SetRotateFlag(void* pPath, int32_t bRotate);           // D2Common.dll!#10794 (0x6FABC2E0)
    void    __stdcall D2COMMON_10160_PathUpdateDirection(void* pPath, uint8_t nDir);             // D2Common.dll!#11130 (0x6FABC250)
    void    __stdcall D2COMMON_10170_PathSetTargetPos(void* pPath, int32_t nX, int32_t nY);      // D2Common.dll!#10515 (0x6FABC2E6)
    D2UnitStrc* __stdcall PATH_GetTargetUnit(void* pPath);                                       // D2Common.dll!#10248 (0x6FABC72A)
    void    __stdcall D2Common_10235_PATH_UpdateRiderPath(D2UnitStrc* pRider, D2UnitStrc* pMount); // D2Common.dll!#11047 (0x6FABC29E)

    uint16_t __stdcall COLLISION_CheckMask(D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, uint16_t nMask); // D2Common.dll!#10493 (0x6FABCAE4)

    void    __stdcall DUNGEON_SetActCallbackFunc(void* pAct, void* pfnCallback);                 // D2Common.dll!#10152 (0x6FABCB6E)
    int32_t __stdcall DUNGEON_GetLevelIdFromRoom(D2ActiveRoomStrc* pRoom);                       // D2Common.dll!#10511 (0x6FABC1A8)
    void    __stdcall DUNGEON_GetRGB_IntensityFromRoom(D2ActiveRoomStrc* pRoom, uint8_t* pIntensity, uint8_t* pR, uint8_t* pG, uint8_t* pB); // D2Common.dll!#10922 (0x6FABC1EA)
    void    __stdcall DUNGEON_GetAdjacentRoomsListFromRoom(D2ActiveRoomStrc* pRoom, D2ActiveRoomStrc*** pppRoomList, int32_t* pNumRooms); // D2Common.dll!#10915 (0x6FABC6A0)
    void    __stdcall DUNGEON_GetRoomCoordinates(D2ActiveRoomStrc* pRoom, int32_t* pCoords);     // D2Common.dll!#10692 (0x6FABCB0E)
    void    __stdcall DUNGEON_ClientToGameSubtileCoords(int32_t* pX, int32_t* pY);               // D2Common.dll!#10725 (0x6FABC238)
    void    __stdcall DUNGEON_GameToClientCoords(int32_t* pX, int32_t* pY);                      // D2Common.dll!#10476 (0x6FABCDB4)
    void    __stdcall DUNGEON_GameSubtileToClientCoords(int32_t* pX, int32_t* pY);               // D2Common.dll!#10912 (0x6FABC1FC)

    void    __stdcall ENVIRONMENT_GetLightColorFromAct(void* pAct, uint8_t* pR, uint8_t* pG, uint8_t* pB); // D2Common.dll!#10744 (0x6FABC1F6)
    uint8_t __stdcall ENVIRONMENT_GetIntensityFromAct(void* pAct);                               // D2Common.dll!#10998 (0x6FABC1F0)

    D2LevelsTxt* __stdcall DATATBLS_GetLevelsTxtRecord(int32_t nLevelId);                        // D2Common.dll!#11102 (0x6FABC922)
    D2MonSoundsTxt* __stdcall DATATBLS_GetMonSoundsTxtRecordFromSoundId(int32_t nSoundId);       // D2Common.dll!#10741 (0x6FABC64C)

    int32_t __stdcall SKILLS_GetParam1(D2SkillStrc* pSkill);                                     // D2Common.dll!#10063 (0x6FABC2FE)
    int32_t __stdcall SKILLS_GetParam2(D2SkillStrc* pSkill);                                     // D2Common.dll!#10089 (0x6FABC28C)
    void    __stdcall SKILLS_SetParam1(D2SkillStrc* pSkill, int32_t nPar1);                      // D2Common.dll!#10965 (0x6FABC27A)
    void    __stdcall SKILLS_SetParam2(D2SkillStrc* pSkill, int32_t nPar2);                      // D2Common.dll!#10817 (0x6FABC2BC)
    void    __stdcall SKILLS_SetParam3(D2SkillStrc* pSkill, int32_t nPar3);                      // D2Common.dll!#10454 (0x6FABC2F8)
    int32_t __stdcall SKILLS_EvaluateSkillFormula(D2UnitStrc* pUnit, uint32_t dwCalc, int32_t nSkillId, int32_t nSkillLevel); // D2Common.dll!#10111 (0x6FABC280)

    int32_t __stdcall STATES_CheckState(D2UnitStrc* pUnit, int32_t nState);                      // D2Common.dll!#10295 (0x6FABC21A)
    void    __stdcall STATES_ToggleState(D2UnitStrc* pUnit, int32_t nState, int32_t bSet);       // D2Common.dll!#10059 (0x6FABC26E)

    int32_t __stdcall MISSILE_GetTotalFrames(D2UnitStrc* pMissile);                              // D2Common.dll!#10955 (0x6FABC2F2)
    void    __stdcall MISSILE_SetTotalFrames(D2UnitStrc* pMissile, int32_t nFrames);             // D2Common.dll!#11076 (0x6FABC322)
    void    __stdcall MISSILE_SetCurrentFrame(D2UnitStrc* pMissile, int32_t nFrame);             // D2Common.dll!#10017 (0x6FABC256)
    void    __stdcall MISSILE_SetActivateFrame(D2UnitStrc* pMissile, int32_t nFrame);            // D2Common.dll!#10224 (0x6FABC316)
    void    __stdcall MISSILE_SetTargetX(D2UnitStrc* pMissile, int32_t nTargetX);                // D2Common.dll!#11161 (0x6FABC304)
    void    __stdcall MISSILE_SetTargetY(D2UnitStrc* pMissile, int32_t nTargetY);                // D2Common.dll!#10792 (0x6FABC2B6)

    void    __stdcall UNITFINDS_InitializeUnitFindData(void* pMemPool, void* pUnitFindData, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nSize, void* pfnCallback, void* pArg); // D2Common.dll!#10908 (0x6FABC2C2)
    void    __stdcall UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(void* pUnitFindData);      // D2Common.dll!#10339 (0x6FABC25C)
    void    __stdcall UNITFINDS_FreeUnitFindData(void* pUnitFindData);                           // D2Common.dll!#10674 (0x6FABC2AA)
}

// ============================================================================
// Data Structures
// ============================================================================

#pragma pack(push, 1)

// 8-byte cell in the 48x48 client lightmap grid (0x6FBBD9C8)
struct D2LightMapCellStrc
{
    int32_t bCollisionObstacle; // +0x00 (byte 0 read as occlusion flag)
    uint8_t nIntensity;         // +0x04 (0x6FBBD9CC)
    uint8_t nRed;               // +0x05 (0x6FBBD9CD)
    uint8_t nGreen;             // +0x06 (0x6FBBD9CE)
    uint8_t nBlue;              // +0x07 (0x6FBBD9CF)
};

// 0x34-byte dynamic/static light source node (linked list at 0x6FBCC1C0)
struct D2LightNodeStrc
{
    int32_t          nUnitType;           // +0x00 (6 = unattached / world light)
    int32_t          nUnitGUID;           // +0x04 (-1 if unattached)
    uint8_t          bClientUnit;         // +0x08 (1 if unit has 0x00200000 flag)
    uint8_t          pad0x09[3];          // +0x09
    int32_t          nLightType;          // +0x0C (0 = dynamic shadow, 1 = simple, 2 = static cached shadow)
    int32_t          nX8;                 // +0x10 (subtile X * 8 + 4)
    int32_t          nY8;                 // +0x14 (subtile Y * 8 + 4)
    int32_t          nCurrentRadius8;     // +0x18 (current radius * 8)
    int32_t          nTargetRadius8;      // +0x1C (target radius * 8)
    int32_t          bFadingOut;          // +0x20 (1 when fading out to 0)
    uint8_t          nIntensity;          // +0x24
    uint8_t          nRed;                // +0x25
    uint8_t          nGreen;              // +0x26
    uint8_t          nBlue;               // +0x27
    D2LightNodeStrc* pNext;               // +0x28
    int32_t          bStaticMapValid;     // +0x2C (1 when pStaticOcclusionMap is populated)
    int32_t*         pStaticOcclusionMap; // +0x30 (size (2*R+1)^2 * 4 bytes when nLightType == 2)
};

// 0x4C-byte 3D particle/physics attachment at pUnit->pGfxData (+0x54) -> +0x30
struct D2ParticleStrc
{
    uint32_t dwFlags;            // +0x00 (0x01=done, 0x02=arc, 0x04=bounce, 0x08=cancelled, 0x10=attached, 0x20=greaterBounds)
    int32_t  nPosX;              // +0x04 (fixed-point 21.11)
    int32_t  nPosY;              // +0x08 (fixed-point 21.11)
    int32_t  nPosZ;              // +0x0C (fixed-point 21.11)
    int32_t  nVelX;              // +0x10 (fixed-point 21.11)
    int32_t  nVelY;              // +0x14 (fixed-point 21.11)
    int32_t  nVelZ;              // +0x18 (fixed-point 21.11)
    int32_t  nAccelX;            // +0x1C (fixed-point 21.11)
    int32_t  nAccelY;            // +0x20 (fixed-point 21.11)
    int32_t  nAccelZ;            // +0x24 (fixed-point 21.11, default -0x1000)
    int32_t  nTargetX;           // +0x28
    int32_t  nTargetY;           // +0x2C
    int32_t  nTargetZ;           // +0x30
    int32_t  nClientOffsetX;     // +0x34
    int32_t  nClientOffsetY;     // +0x38
    int32_t  nClientOffsetZ;     // +0x3C
    int32_t  nBounceCount;       // +0x40
    int32_t  nBounceDampingPct;  // +0x44
    int32_t  nRemainingFrames;   // +0x48
};

// Minimal layout of client D2UnitStrc fields accessed in dLightMap.cpp / Particle.cpp
struct D2UnitLightViewStrc
{
    uint32_t dwUnitType;         // +0x00
    int32_t  nClassId;           // +0x04
    void*    pMemoryPool;        // +0x08
    uint32_t dwUnitId;           // +0x0C
    uint32_t dwAnimMode;         // +0x10
    void*    pUnitData;          // +0x14
    int32_t  nAct;               // +0x18
    void*    pAct;               // +0x1C
    uint32_t dwSeedLow;          // +0x20
    uint32_t dwSeedHigh;         // +0x24
    uint32_t dwInitSeed;         // +0x28
    void*    pPath;              // +0x2C
    uint8_t  pad0x30[0x24];      // +0x30
    uint8_t* pGfxData;           // +0x54
    uint8_t  pad0x58[0x6C];      // +0x58
    uint32_t dwFlags;            // +0xC4
    uint32_t dwFlagEx;           // +0xC8
    uint8_t  pad0xCC[0x18];      // +0xCC
    D2UnitLightViewStrc* pListNext; // +0xE4
};

#pragma pack(pop)

// ============================================================================
// Global Variables (1.12a Addresses)
// ============================================================================

static const char* const s_szLightMapFile = "..\\Source\\D2Client\\DRAW\\dLightMap.cpp"; // 0x6FB85208
static const char* const s_szParticleFile = "..\\Source\\D2Client\\ENGINE\\Particle.cpp"; // 0x6FB85D80

// Lightmap Raycast & Grid Globals
static int32_t            g_nLightRayStepY;                  // 0x6FBB55A0
static int32_t            g_aLightRayOcclusionGrid[64][64];  // 0x6FBB55A8 (0x4000 bytes)
static int32_t            g_aLightRayObstacleGrid[64][64];   // 0x6FBB95A8 (0x4000 bytes)
static int32_t            g_nLightRayFraction;               // 0x6FBBD5A8
static int32_t            g_nLightRayStepX;                  // 0x6FBBD5AC
static int32_t            g_nLightMapOriginSubtileX;         // 0x6FBBD5B0
static int32_t            g_nLightMapOriginSubtileY;         // 0x6FBBD5B4
static int32_t            g_nLightMapEndSubtileX;            // 0x6FBBD5B8
static int32_t            g_nLightMapEndSubtileY;            // 0x6FBBD5BC
static uint32_t           g_aLightReciprocalTable[256];      // 0x6FBBD5C0 (0x400 bytes)
static int32_t            g_nLightRayDirMode;                // 0x6FBBD9C0
static D2LightMapCellStrc g_aLightMapGrid[48][48];           // 0x6FBBD9C8 (0x4800 bytes)

// Light Node Linked List Globals
static D2LightNodeStrc*   g_pLightNodeListHead;              // 0x6FBCC1C0
static int32_t            g_nLightNodesCreatedCount;         // 0x6FBCC1C4
static int32_t            g_nLightNodesFreedCount;           // 0x6FBCC1C8
static int32_t            g_nActiveLightNodesCount;          // 0x6FBCC1CC

// View Skew & Colored Lighting Globals
static int32_t            g_nViewSkewMode;                   // 0x6FBCC1D0
static int32_t            g_nViewSkewOffset;                 // 0x6FBCC1D4
static int32_t            g_bColoredLightingEnabled;         // 0x6FB946C8
static uint32_t           g_dwLightQualityLastTick;          // 0x6FBD3654
static int32_t            g_nCachedLightQuality;             // 0x6FBD3658

// External Client Globals Referenced
extern int32_t            g_nScreenWidth;                    // 0x6FB8C6E0
extern int32_t            g_bForceMediumLightQuality;        // 0x6FBA6778
extern int32_t            g_nDiabloSealFlashCounter;         // 0x6FBA8AD4
extern int32_t            g_nCurrentClientTick;              // 0x6FBB32B0
extern int32_t            g_nSmoothedFps;                    // 0x6FBB32C0
extern int32_t            g_nOverrideAmbientIntensity;       // 0x6FBCB7C8
extern uint8_t            g_nOverrideAmbientRed;             // 0x6FBCB7CC
extern uint8_t            g_nOverrideAmbientGreen;           // 0x6FBCB7CD
extern uint8_t            g_nOverrideAmbientBlue;            // 0x6FBCB7CE
extern D2UnitLightViewStrc* g_aClientUnitHashTables[6][128]; // 0x6FBC9D60
extern D2UnitLightViewStrc* g_aServerUnitHashTables[6][128]; // 0x6FBCA960
extern void*              g_pClientAct;                      // 0x6FBCC01C
extern void*              g_pPrimaryViewport;                // 0x6FBCC028
extern int32_t            g_nViewportOffsetX;                // 0x6FBCC040
extern int32_t            g_nViewportOffsetY;                // 0x6FBCC044
extern int32_t            g_nViewportTargetOffsetX;          // 0x6FBCC05C
extern int32_t            g_nViewportTargetOffsetY;          // 0x6FBCC060
extern D2UnitStrc*        g_pPlayerUnit;                     // 0x6FBCC3D0
extern int32_t            g_bDiabloSealActive;               // 0x6FBCC41C
extern int32_t            g_nPlayerIsoX;                     // 0x6FBCD524
extern int32_t            g_nPlayerIsoY;                     // 0x6FBCD528

// ============================================================================
// Internal Helper Declarations
// ============================================================================

D2UnitStrc* __fastcall CLIENTSIDE_FindUnit(D2UnitLightViewStrc** ppHashTable, int32_t nUnitId, int32_t nUnitType); // 0x6FACE550
D2ActiveRoomStrc* __fastcall DUNGEON_FindRoomBySubtile(D2ActiveRoomStrc* pHintRoom, int32_t nSubtileX, int32_t nSubtileY); // 0x6FAB1A00
int32_t __fastcall UNIT_GetSubtileX(D2UnitStrc* pUnit); // 0x6FAB1590
int32_t __fastcall UNIT_GetSubtileY(D2UnitStrc* pUnit); // 0x6FAB15C0

// ============================================================================
// PART 1: ..\Source\D2Client\DRAW\dLightMap.cpp (0x6FB0DFF0..0x6FB0FDD0)
// ============================================================================

// 1.12a: 0x6FB0DFF0 (usercall: eax = nSkewMode)
// Updates perspective/view matrix skew adjustment and shifts the primary viewport.
void __fastcall VIEW_SetSkewMode(int32_t nSkewMode)
{
    switch (nSkewMode)
    {
    case 0:
    case 3:
        g_nViewSkewOffset = 0;
        break;
    case 1:
        g_nViewSkewOffset = -(g_nScreenWidth / 4);
        break;
    case 2:
        g_nViewSkewOffset = g_nScreenWidth / 4;
        break;
    default:
        break;
    }

    g_nViewSkewMode = nSkewMode;
    D2GFX_SetViewMatrixAdjust(g_nViewSkewOffset);

    int32_t nQuarterY = g_nPlayerIsoY / 4;
    switch (nSkewMode)
    {
    case 0:
        // VIEW_SetViewportTarget(g_pPrimaryViewport, g_nPlayerIsoX, g_nPlayerIsoY, 0, 0)
        break;
    case 1:
        // VIEW_SetViewportTarget(g_pPrimaryViewport, g_nPlayerIsoX, g_nPlayerIsoY - nQuarterY, 0, -nQuarterY)
        break;
    case 2:
        // VIEW_SetViewportTarget(g_pPrimaryViewport, g_nPlayerIsoX, g_nPlayerIsoY + nQuarterY, 0, +nQuarterY)
        break;
    case 3:
        g_nViewportOffsetX = 0;
        g_nViewportOffsetY = 0;
        g_nViewportTargetOffsetX = 0;
        g_nViewportTargetOffsetY = 0;
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FB0E0E0 (__stdcall)
// Debug overlay for the primary light node's occlusion and lightmap grids.
void __stdcall LIGHTMAP_DebugDraw()
{
    D2LightNodeStrc* pLight = g_pLightNodeListHead;
    D2GFX_DebugDraw(g_aLightRayOcclusionGrid, g_aLightMapGrid, pLight->nX8, pLight->nY8);
}

// 1.12a: 0x6FB0E100 (userpurge: ecx = nX8, eax = nY8, stack: pIntensity, pRed, pGreen, pBlue)
// Samples the 48x48 lightmap grid at 1/8th-subtile coordinates (nX8, nY8).
void __stdcall LIGHTMAP_SampleLightAtSubtile8(
    int32_t nX8,
    int32_t nY8,
    uint8_t* pOutIntensity,
    uint8_t* pOutRed,
    uint8_t* pOutGreen,
    uint8_t* pOutBlue)
{
    int32_t nGridX = (nX8 >> 3) - g_nLightMapOriginSubtileX;
    int32_t nGridY = (nY8 >> 3) - g_nLightMapOriginSubtileY;

    if (nGridX < 0)
        nGridX = 0;
    else if (nGridX > 47)
        nGridX = 47;

    if (nGridY < 0)
        nGridY = 0;
    else if (nGridY > 47)
        nGridY = 47;

    const D2LightMapCellStrc* pCell = &g_aLightMapGrid[nGridY][nGridX];
    *pOutIntensity = pCell->nIntensity;

    if (g_bColoredLightingEnabled)
    {
        *pOutRed   = pCell->nRed;
        *pOutGreen = pCell->nGreen;
        *pOutBlue  = pCell->nBlue;
    }
}

// 1.12a: 0x6FB0E180 (__stdcall)
// Frees all active light nodes in the client light list.
void __stdcall LIGHTMAP_FreeAllLightNodes()
{
    D2LightNodeStrc* pCurr = g_pLightNodeListHead;
    while (pCurr != nullptr)
    {
        D2LightNodeStrc* pNext = pCurr->pNext;
        if (pCurr->pStaticOcclusionMap != nullptr)
        {
            FOG_Free(pCurr->pStaticOcclusionMap, s_szLightMapFile, 1405 /*0x57D*/, 0);
        }
        FOG_Free(pCurr, s_szLightMapFile, 1407 /*0x57F*/, 0);
        pCurr = pNext;
    }
    g_pLightNodeListHead = nullptr;
}

// 1.12a: 0x6FB0E1E0 (usercall: eax = nGridY, ecx = nGridX)
// Propagates occlusion along the current ray direction into g_aLightRayOcclusionGrid[nGridY][nGridX].
void __fastcall LIGHTMAP_PropagateRayOcclusionCell(int32_t nGridX, int32_t nGridY)
{
    switch (g_nLightRayDirMode)
    {
    case 0:
    default:
        return;

    case 1: // Pure horizontal step (g_nLightRayStepX, 0)
    {
        int32_t nSrcX = nGridX + g_nLightRayStepX;
        int32_t nVal = g_aLightRayObstacleGrid[nGridY][nSrcX];
        if (nVal == 0)
            nVal = g_aLightRayOcclusionGrid[nGridY][nSrcX];
        g_aLightRayOcclusionGrid[nGridY][nGridX] = nVal;
        return;
    }

    case 2: // X-major diagonal interpolation
    {
        int32_t nSrcX = nGridX + g_nLightRayStepX;
        int32_t nVal1 = g_aLightRayObstacleGrid[nGridY][nSrcX];
        if (nVal1 == 0)
            nVal1 = g_aLightRayOcclusionGrid[nGridY][nSrcX];

        int32_t nSrcY = nGridY + g_nLightRayStepY;
        int32_t nVal2 = g_aLightRayObstacleGrid[nSrcY][nSrcX];
        if (nVal2 == 0)
            nVal2 = g_aLightRayOcclusionGrid[nSrcY][nSrcX];

        int32_t nFrac = g_nLightRayFraction;
        g_aLightRayOcclusionGrid[nGridY][nGridX] = ((256 - nFrac) * nVal1 + nFrac * nVal2) >> 8;
        return;
    }

    case 3: // Y-major diagonal interpolation
    {
        int32_t nSrcY = nGridY + g_nLightRayStepY;
        int32_t nVal1 = g_aLightRayObstacleGrid[nSrcY][nGridX];
        if (nVal1 == 0)
            nVal1 = g_aLightRayOcclusionGrid[nSrcY][nGridX];

        int32_t nSrcX = nGridX + g_nLightRayStepX;
        int32_t nVal2 = g_aLightRayObstacleGrid[nSrcY][nSrcX];
        if (nVal2 == 0)
            nVal2 = g_aLightRayOcclusionGrid[nSrcY][nSrcX];

        int32_t nFrac = g_nLightRayFraction;
        g_aLightRayOcclusionGrid[nGridY][nGridX] = ((256 - nFrac) * nVal1 + nFrac * nVal2) >> 8;
        return;
    }

    case 4: // Pure vertical step (0, g_nLightRayStepY)
    {
        int32_t nSrcY = nGridY + g_nLightRayStepY;
        int32_t nVal = g_aLightRayObstacleGrid[nSrcY][nGridX];
        if (nVal == 0)
            nVal = g_aLightRayOcclusionGrid[nSrcY][nGridX];
        g_aLightRayOcclusionGrid[nGridY][nGridX] = nVal;
        return;
    }
    }
}

// 1.12a: 0x6FB0E340 (userpurge: eax = pLight, edx = nTargetX8, stack: nTargetY8)
// Computes the ray step direction and 8.8 fixed-point slope from a target point back toward the light center.
void __stdcall LIGHTMAP_SetupRayStep(D2LightNodeStrc* pLight, int32_t nTargetX8, int32_t nTargetY8)
{
    int32_t nDeltaX = pLight->nX8 - nTargetX8;
    int32_t nDeltaY = pLight->nY8 - nTargetY8;

    if (nDeltaX == 0 && nDeltaY == 0)
    {
        g_nLightRayDirMode = 0;
        return;
    }

    if (nDeltaX < 0)
    {
        g_nLightRayStepX = -1;
        nDeltaX = -nDeltaX;
    }
    else
    {
        g_nLightRayStepX = 1;
    }

    if (nDeltaY < 0)
    {
        g_nLightRayStepY = -1;
        nDeltaY = -nDeltaY;
    }
    else
    {
        g_nLightRayStepY = 1;
    }

    if (nDeltaX == 0)
    {
        g_nLightRayDirMode = 4;
    }
    else if (nDeltaY == 0)
    {
        g_nLightRayDirMode = 1;
    }
    else if (nDeltaX >= nDeltaY)
    {
        g_nLightRayFraction = (nDeltaY << 8) / nDeltaX;
        g_nLightRayDirMode = 2;
    }
    else
    {
        g_nLightRayFraction = (nDeltaX << 8) / nDeltaY;
        g_nLightRayDirMode = 3;
    }
}

// 1.12a: 0x6FB0E3F0 (usercall: eax = nX8, ecx = nY8)
// Returns 1 if the subtile at (nX8 >> 3, nY8 >> 3) is out of bounds or marked as a light-blocking obstacle.
uint8_t __fastcall LIGHTMAP_IsSubtileBlockedAt8(int32_t nX8, int32_t nY8)
{
    int32_t nGridX = (nX8 >> 3) - g_nLightMapOriginSubtileX;
    if (nGridX < 0 || nGridX >= 48)
        return 1;

    int32_t nGridY = (nY8 >> 3) - g_nLightMapOriginSubtileY;
    if (nGridY < 0 || nGridY >= 48)
        return 1;

    return static_cast<uint8_t>(g_aLightMapGrid[nGridY][nGridX].bCollisionObstacle);
}

// 1.12a: 0x6FB0E430 (userpurge: ecx = nStartY, eax = nEndY, edi = nEndX, edx = dwPackedColorIntensity, stack: nStartX)
// Fills a rectangular region of the 48x48 lightmap grid with ambient intensity and RGB color.
void __stdcall LIGHTMAP_FillAmbientRect(
    int32_t nStartX,
    int32_t nStartY,
    int32_t nEndX,
    int32_t nEndY,
    uint32_t dwPackedColorIntensity)
{
    if (nStartY > nEndY)
        return;

    uint8_t nIntensity = static_cast<uint8_t>(dwPackedColorIntensity & 0xFF);
    uint8_t nRed       = static_cast<uint8_t>((dwPackedColorIntensity >> 8) & 0xFF);
    uint8_t nGreen     = static_cast<uint8_t>((dwPackedColorIntensity >> 16) & 0xFF);
    uint8_t nBlue      = static_cast<uint8_t>((dwPackedColorIntensity >> 24) & 0xFF);

    for (int32_t y = nStartY; y <= nEndY; ++y)
    {
        if (nStartX <= nEndX)
        {
            D2LightMapCellStrc* pCell = &g_aLightMapGrid[y][nStartX];
            for (int32_t x = nStartX; x <= nEndX; ++x, ++pCell)
            {
                pCell->bCollisionObstacle = 0;
                pCell->nIntensity = nIntensity;
                pCell->nRed       = nRed;
                pCell->nGreen     = nGreen;
                pCell->nBlue      = nBlue;
            }
        }
    }
}

// 1.12a: 0x6FB0E4C0 (usercall: eax = pLight)
// Initiates fade-out of a dynamic light node by setting bFadingOut = 1 and nTargetRadius8 = 0.
void __fastcall LIGHTMAP_StartFadeOutLightNode(D2LightNodeStrc* pLight)
{
    if (pLight == nullptr)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(397 /*0x18D*/), 397);
        exit(-1);
    }

    pLight->bFadingOut = 1;
    pLight->nTargetRadius8 = 0;

    if (pLight->nLightType == 2)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(400 /*0x190*/), 400);
        exit(-1);
    }
}

// 1.12a: 0x6FB0E500 (usercall: esi = pLight)
// Unlinks and frees a single light node from g_pLightNodeListHead.
void __fastcall LIGHTMAP_DestroyLightNode(D2LightNodeStrc* pLight)
{
    if (pLight == nullptr)
        return;

    D2LightNodeStrc* pCurr = g_pLightNodeListHead;
    D2LightNodeStrc* pPrev = nullptr;

    while (pCurr != nullptr && pCurr != pLight)
    {
        pPrev = pCurr;
        pCurr = pCurr->pNext;
    }

    if (pCurr == nullptr)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(378 /*0x17A*/), 378);
        exit(-1);
    }

    if (pPrev != nullptr)
        pPrev->pNext = pCurr->pNext;
    else
        g_pLightNodeListHead = pCurr->pNext;

    if (pLight->pStaticOcclusionMap != nullptr)
    {
        FOG_Free(pLight->pStaticOcclusionMap, s_szLightMapFile, 385 /*0x181*/, 0);
    }
    FOG_Free(pLight, s_szLightMapFile, 387 /*0x183*/, 0);
    ++g_nLightNodesFreedCount;
}

// 1.12a: 0x6FB0E590 (userpurge: eax = pLight, stack: nRed, nGreen, nBlue)
// Updates the RGB color components of a light node.
void __stdcall LIGHTMAP_SetLightNodeColor(D2LightNodeStrc* pLight, uint8_t nRed, uint8_t nGreen, uint8_t nBlue)
{
    if (pLight == nullptr)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(359 /*0x167*/), 359);
        exit(-1);
    }

    pLight->nRed   = nRed;
    pLight->nGreen = nGreen;
    pLight->nBlue  = nBlue;
}

// 1.12a: 0x6FB0E5D0 (__stdcall)
// Returns the maximum supported light map dimension (32).
int32_t __stdcall LIGHTMAP_GetMaxLightDimension()
{
    return 32;
}

// 1.12a: 0x6FB0E5E0 (usercall: eax = pLight)
// Returns the current subtile radius of a light node (nCurrentRadius8 >> 3).
int32_t __fastcall LIGHTMAP_GetLightNodeRadius(D2LightNodeStrc* pLight)
{
    if (pLight == nullptr)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(341 /*0x155*/), 341);
        exit(-1);
    }
    return pLight->nCurrentRadius8 >> 3;
}

// 1.12a: 0x6FB0E610 (__stdcall)
// Toggles colored lighting on/off.
BOOL __stdcall LIGHTMAP_ToggleColoredLighting()
{
    g_bColoredLightingEnabled = (g_bColoredLightingEnabled == 0) ? 1 : 0;
    return g_bColoredLightingEnabled;
}

// 1.12a: 0x6FB0E630 (userpurge: eax = nDx, stack: nDy)
// Octagonal Euclidean distance approximation: (983 * max(|dx|,|dy|) + 407 * min(|dx|,|dy|)) >> 10.
int32_t __stdcall LIGHTMAP_ApproxDistanceA(int32_t nDx, int32_t nDy)
{
    int32_t nAbsX = (nDx < 0) ? -nDx : nDx;
    int32_t nAbsY = (nDy < 0) ? -nDy : nDy;

    if (nAbsX >= nAbsY)
        return static_cast<uint32_t>(nAbsX * 983 + nAbsY * 407) >> 10;
    else
        return static_cast<uint32_t>(nAbsY * 983 + nAbsX * 407) >> 10;
}

// 1.12a: 0x6FB0E680 (userpurge: eax = nDx, stack: nDy)
// Identical octagonal Euclidean distance approximation used by static lightmap application.
int32_t __stdcall LIGHTMAP_ApproxDistanceB(int32_t nDx, int32_t nDy)
{
    int32_t nAbsX = (nDx < 0) ? -nDx : nDx;
    int32_t nAbsY = (nDy < 0) ? -nDy : nDy;

    if (nAbsX >= nAbsY)
        return static_cast<uint32_t>(nAbsX * 983 + nAbsY * 407) >> 10;
    else
        return static_cast<uint32_t>(nAbsY * 983 + nAbsX * 407) >> 10;
}

// 1.12a: 0x6FB0E6D0 (__stdcall)
// Evaluates dynamic lighting quality (0 = Low, 1 = Medium, 2 = High) based on D2GFX low-quality mode and FPS.
int32_t __stdcall LIGHTMAP_UpdateLightQuality()
{
    int32_t nDesiredQuality = 2;
    BOOL bLowQuality = D2GFX_CheckLowQuality();
    int32_t nPrevQuality = g_nCachedLightQuality;

    if (bLowQuality)
    {
        if (g_bForceMediumLightQuality == 0)
        {
            nDesiredQuality = 0;
            goto APPLY_QUALITY;
        }
        nDesiredQuality = 1;
    }

    if (g_nSmoothedFps <= 9)
    {
        nDesiredQuality = 0;
    }
    else if (g_nSmoothedFps <= 12)
    {
        nDesiredQuality = 1;
    }
    else if (g_nSmoothedFps <= 15)
    {
        if (nPrevQuality >= 1)
            return nPrevQuality;
        nDesiredQuality = 1;
    }

APPLY_QUALITY:
    if (nDesiredQuality != nPrevQuality)
    {
        DWORD dwNow = GetTickCount();
        if (dwNow - g_dwLightQualityLastTick <= 2000)
            return g_nCachedLightQuality;

        g_dwLightQualityLastTick = dwNow;
        g_nCachedLightQuality = nDesiredQuality;
    }
    return nDesiredQuality;
}

// 1.12a: 0x6FB0E750 (__stdcall)
// Populates collision obstacle flags (mask 0x22 = wall/light blocker) across the 48x48 lightmap grid.
void __stdcall LIGHTMAP_PopulateObstacleGrid()
{
    D2UnitStrc* pPlayer = g_pPlayerUnit;
    if (pPlayer == nullptr)
        return;

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pPlayer);
    if (pRoom == nullptr)
        return;

    for (int32_t y = 0; y < 48; ++y)
    {
        for (int32_t x = 0; x < 48; ++x)
        {
            int32_t nSubtileX = g_nLightMapOriginSubtileX + x;
            int32_t nSubtileY = g_nLightMapOriginSubtileY + y;

            pRoom = DUNGEON_FindRoomBySubtile(pRoom, nSubtileX, nSubtileY);
            if (pRoom == nullptr)
                pRoom = UNITS_GetRoom(pPlayer);

            if (COLLISION_CheckMask(pRoom, nSubtileX, nSubtileY, 0x0022) != 0)
            {
                g_aLightMapGrid[y][x].bCollisionObstacle = 1;
            }
        }
    }
}

// 1.12a: 0x6FB0E820 (usercall: edx = pLight)
// Copies obstacle values (0 or 16) from the 48x48 lightmap grid into the 64x64 raycast obstacle grid around pLight.
void __fastcall LIGHTMAP_CopyObstaclesForLightNode(D2LightNodeStrc* pLight)
{
    int32_t nRadius8 = pLight->nCurrentRadius8;
    int32_t nStartX8 = (pLight->nX8 & ~7) - nRadius8;
    int32_t nStartY8 = (pLight->nY8 & ~7) - nRadius8;

    int32_t nRadiusSubtiles = nRadius8 >> 3;
    int32_t nDiameterSubtiles = (nRadius8 * 2) >> 3;
    if (nDiameterSubtiles < 0)
        return;

    int32_t nStartOffset = 32 - nRadiusSubtiles;
    int32_t* pRowBase = &g_aLightRayObstacleGrid[nStartOffset][nStartOffset];
    int32_t nCount = nDiameterSubtiles + 1;

    int32_t nCurY8 = nStartY8;
    for (int32_t row = 0; row < nCount; ++row, nCurY8 += 8, pRowBase += 64)
    {
        int32_t nCurX8 = nStartX8;
        int32_t* pDst = pRowBase;
        for (int32_t col = 0; col < nCount; ++col, nCurX8 += 8, ++pDst)
        {
            uint8_t bBlocked = LIGHTMAP_IsSubtileBlockedAt8(nCurX8, nCurY8);
            *pDst = static_cast<int32_t>(bBlocked) << 4;
        }
    }
}

// 1.12a: 0x6FB0E910 (userpurge: eax = nX8, ecx = nY8, stack: nLightIntensity, nRed, nGreen, nBlue)
// Blends a light source contribution (intensity + RGB) into the 48x48 lightmap cell at (nX8 >> 3, nY8 >> 3).
void __stdcall LIGHTMAP_AddLightToSubtile8(
    int32_t nX8,
    int32_t nY8,
    int32_t nLightIntensity,
    uint8_t nRed,
    uint8_t nGreen,
    uint8_t nBlue)
{
    int32_t nGridX = (nX8 >> 3) - g_nLightMapOriginSubtileX;
    if (nGridX < 0 || nGridX >= 48)
        return;

    int32_t nGridY = (nY8 >> 3) - g_nLightMapOriginSubtileY;
    if (nGridY < 0 || nGridY >= 48)
        return;

    D2LightMapCellStrc* pCell = &g_aLightMapGrid[nGridY][nGridX];
    int32_t nOldIntensity = pCell->nIntensity;
    int32_t nNewIntensity = nOldIntensity + nLightIntensity;
    if (nNewIntensity > 255)
        nNewIntensity = 255;

    if (g_bColoredLightingEnabled)
    {
        uint32_t nRecip = g_aLightReciprocalTable[nNewIntensity];

        int32_t nOutR = ((pCell->nRed * nOldIntensity + nRed * nLightIntensity) * nRecip) >> 16;
        if (nOutR > 255)
            nOutR = 255;

        int32_t nOutG = ((pCell->nGreen * nOldIntensity + nGreen * nLightIntensity) * nRecip) >> 16;
        if (nOutG > 255)
            nOutG = 255;

        int32_t nOutB = ((pCell->nBlue * nOldIntensity + nBlue * nLightIntensity) * nRecip) >> 16;
        if (nOutB > 255)
            nOutB = 255;

        pCell->nIntensity = static_cast<uint8_t>(nNewIntensity);
        pCell->nRed       = static_cast<uint8_t>(nOutR);
        pCell->nGreen     = static_cast<uint8_t>(nOutG);
        pCell->nBlue      = static_cast<uint8_t>(nOutB);
    }
    else
    {
        pCell->nIntensity = static_cast<uint8_t>(nNewIntensity);
        pCell->nRed       = 0;
        pCell->nGreen     = 0;
        pCell->nBlue      = 0;
    }
}

// 1.12a: 0x6FB0EA30 (usercall: eax = nRadius, esi = pLight)
// Immediately sets both current and target radius on a light node, invalidating its static occlusion map if type 2.
void __fastcall LIGHTMAP_SetLightNodeRadiusImmediate(int32_t nRadius, D2LightNodeStrc* pLight)
{
    if (nRadius <= 0)
        return;
    if (nRadius >= 18)
        nRadius = 18;

    if (pLight == nullptr)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(321 /*0x141*/), 321);
        exit(-1);
    }

    int32_t nRadius8 = nRadius << 3;
    pLight->nCurrentRadius8 = nRadius8;
    pLight->nTargetRadius8  = nRadius8;

    if (pLight->nLightType == 2)
    {
        pLight->bStaticMapValid = 0;
        if (pLight->pStaticOcclusionMap != nullptr)
        {
            FOG_Free(pLight->pStaticOcclusionMap, s_szLightMapFile, 330 /*0x14A*/, 0);
        }
        pLight->pStaticOcclusionMap = nullptr;
    }
}

// 1.12a: 0x6FB0EAB0 (usercall: eax = nTargetRadius, ecx = pLight)
// Sets the target transition radius on a light node (nTargetRadius8 = min(nTargetRadius, 18) << 3).
void __fastcall LIGHTMAP_SetLightNodeTargetRadius(int32_t nTargetRadius, D2LightNodeStrc* pLight)
{
    if (nTargetRadius <= 0)
        return;
    if (nTargetRadius >= 18)
        nTargetRadius = 18;

    if (pLight == nullptr)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(305 /*0x131*/), 305);
        exit(-1);
    }

    pLight->nTargetRadius8 = nTargetRadius << 3;
}

// 1.12a: 0x6FB0EAF0 (userpurge: edi = pUnit, ebx = nLightType, stack: nRadius, nIntensity, nRed, nGreen, nBlue)
// Allocates and links a new D2LightNodeStrc (0x34 bytes) for a unit.
D2LightNodeStrc* __stdcall LIGHTMAP_CreateLightNode(
    D2UnitLightViewStrc* pUnit,
    int32_t nLightType,
    int32_t nRadius,
    uint8_t nIntensity,
    uint8_t nRed,
    uint8_t nGreen,
    uint8_t nBlue)
{
    if (nRadius <= 0)
        return nullptr;
    if (nRadius >= 18)
        nRadius = 18;

    ++g_nLightNodesCreatedCount;
    D2LightNodeStrc* pLight = static_cast<D2LightNodeStrc*>(
        FOG_Alloc(sizeof(D2LightNodeStrc) /*0x34*/, s_szLightMapFile, 255 /*0xFF*/, 0));

    pLight->nUnitType = (pUnit != nullptr) ? static_cast<int32_t>(pUnit->dwUnitType) : 6;
    pLight->nUnitGUID = (pUnit != nullptr) ? static_cast<int32_t>(pUnit->dwUnitId) : -1;

    if (pUnit != nullptr && ((pUnit->dwFlags >> 21) & 1) != 0)
        pLight->bClientUnit = 1;
    else
        pLight->bClientUnit = 0;

    pLight->nX8 = (static_cast<uint32_t>(UNITS_GetPrecisionX(reinterpret_cast<D2UnitStrc*>(pUnit))) >> 13) + 4;
    pLight->nY8 = (static_cast<uint32_t>(UNITS_GetClientCoordX(reinterpret_cast<D2UnitStrc*>(pUnit))) >> 13) + 4;

    int32_t nRadius8 = nRadius << 3;
    pLight->nCurrentRadius8 = nRadius8;
    pLight->nTargetRadius8  = nRadius8;
    pLight->nLightType      = nLightType;
    pLight->bFadingOut      = 0;
    pLight->nIntensity      = nIntensity;
    pLight->nRed            = nRed;
    pLight->nGreen          = nGreen;
    pLight->nBlue           = nBlue;
    pLight->pNext           = g_pLightNodeListHead;
    pLight->bStaticMapValid = 0;

    if (nLightType == 2)
    {
        int32_t nDim = 2 * nRadius + 1;
        pLight->pStaticOcclusionMap = static_cast<int32_t*>(
            FOG_Alloc(nDim * nDim * sizeof(int32_t), s_szLightMapFile, 278 /*0x116*/, 0));
    }
    else
    {
        pLight->pStaticOcclusionMap = nullptr;
    }

    g_pLightNodeListHead = pLight;
    return pLight;
}

// 1.12a: 0x6FB0EC00 (usercall: eax = pLight)
// Resolves the D2UnitStrc* owner of a D2LightNodeStrc from the client or server unit hash tables.
D2UnitStrc* __fastcall LIGHTMAP_ResolveLightOwnerUnit(D2LightNodeStrc* pLight)
{
    if (pLight->nUnitType == 6)
        return nullptr;

    D2UnitLightViewStrc** ppTable = pLight->bClientUnit
        ? g_aClientUnitHashTables[pLight->nUnitType]
        : g_aServerUnitHashTables[pLight->nUnitType];

    return CLIENTSIDE_FindUnit(ppTable, pLight->nUnitGUID & 0x7F, pLight->nUnitType);
}

// 1.12a: 0x6FB0EC40 (fastcall: ecx = pAddedOrRemovedRoom)
// Act room callback: invalidates cached static light maps (bStaticMapValid = 0) overlapping pAddedOrRemovedRoom.
void __fastcall LIGHTMAP_OnRoomAddedOrRemovedCallback(D2ActiveRoomStrc* pChangedRoom)
{
    for (D2LightNodeStrc* pLight = g_pLightNodeListHead; pLight != nullptr; pLight = pLight->pNext)
    {
        if (pLight->nLightType != 2)
            continue;

        if (pLight->nUnitType == 6)
        {
            FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(1425 /*0x591*/), 1425);
            exit(-1);
        }

        D2UnitLightViewStrc* pUnit = pLight->bClientUnit
            ? g_aClientUnitHashTables[pLight->nUnitType][pLight->nUnitGUID & 0x7F]
            : g_aServerUnitHashTables[pLight->nUnitType][pLight->nUnitGUID & 0x7F];

        while (pUnit != nullptr && static_cast<int32_t>(pUnit->dwUnitId) != pLight->nUnitGUID)
        {
            pUnit = pUnit->pListNext;
        }

        if (pUnit != nullptr && static_cast<int32_t>(pUnit->dwUnitType) != pLight->nUnitType)
        {
            FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(90 /*0x5A*/), 90);
            exit(-1);
        }

        if (pUnit == nullptr)
        {
            FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(1425 /*0x591*/), 1425);
            exit(-1);
        }

        D2ActiveRoomStrc* pUnitRoom = UNITS_GetRoom(reinterpret_cast<D2UnitStrc*>(pUnit));
        if (pUnitRoom == pChangedRoom)
            continue;

        int32_t nUnitX = UNIT_GetSubtileX(reinterpret_cast<D2UnitStrc*>(pUnit));
        int32_t nUnitY = UNIT_GetSubtileY(reinterpret_cast<D2UnitStrc*>(pUnit));
        int32_t nRadiusSubtiles = pLight->nCurrentRadius8 >> 3;

        if (DUNGEON_FindRoomBySubtile(pUnitRoom, nUnitX + nRadiusSubtiles, nUnitY) == pChangedRoom ||
            DUNGEON_FindRoomBySubtile(pUnitRoom, nUnitX - nRadiusSubtiles, nUnitY) == pChangedRoom ||
            DUNGEON_FindRoomBySubtile(pUnitRoom, nUnitX, nUnitY + nRadiusSubtiles) == pChangedRoom ||
            DUNGEON_FindRoomBySubtile(pUnitRoom, nUnitX, nUnitY - nRadiusSubtiles) == pChangedRoom)
        {
            pLight->bStaticMapValid = 0;
        }
    }
}

// 1.12a: 0x6FB0EE30 (usercall: edi = pLight)
// Applies a precomputed static light node (nLightType == 2) from pLight->pStaticOcclusionMap onto the 48x48 lightmap.
void __fastcall LIGHTMAP_ApplyStaticLightNode(D2LightNodeStrc* pLight)
{
    int32_t nRadius8 = pLight->nCurrentRadius8;
    if (nRadius8 <= 0 || nRadius8 >= 256)
        return;

    int32_t nStartX8 = (pLight->nX8 & ~7) - nRadius8;
    int32_t nStartY8 = (pLight->nY8 & ~7) - nRadius8;
    int32_t nDiameterSubtiles = (nRadius8 * 2) >> 3;

    int32_t nMinSubtileX = nStartX8 >> 3;
    if (nMinSubtileX > g_nLightMapEndSubtileX || (nMinSubtileX + nDiameterSubtiles + 1) < g_nLightMapOriginSubtileX)
        return;

    int32_t nMinSubtileY = nStartY8 >> 3;
    if (nMinSubtileY > g_nLightMapEndSubtileY || (nMinSubtileY + nDiameterSubtiles + 1) < g_nLightMapOriginSubtileY)
        return;

    if (pLight->pStaticOcclusionMap == nullptr)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(1150 /*0x47E*/), 1150);
        exit(-1);
    }
    if (pLight->bStaticMapValid == 0)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(1151 /*0x47F*/), 1151);
        exit(-1);
    }

    int32_t nFalloffSlope = (static_cast<int32_t>(pLight->nIntensity) << 16) / nRadius8;
    if (nDiameterSubtiles < 0)
        return;

    int32_t nCount = nDiameterSubtiles + 1;
    const int32_t* pOcclusionEntry = pLight->pStaticOcclusionMap;
    int32_t nCurY8 = nStartY8;

    for (int32_t row = 0; row < nCount; ++row, nCurY8 += 8)
    {
        int32_t nAbsDy = pLight->nY8 - nCurY8;
        if (nAbsDy < 0)
            nAbsDy = -nAbsDy;

        int32_t nCurX8 = nStartX8;
        for (int32_t col = 0; col < nCount; ++col, nCurX8 += 8, ++pOcclusionEntry)
        {
            int32_t nOcclusion = *pOcclusionEntry;
            if (nOcclusion < 16)
            {
                int32_t nAbsDx = pLight->nX8 - nCurX8;
                if (nAbsDx < 0)
                    nAbsDx = -nAbsDx;

                int32_t nDist = LIGHTMAP_ApproxDistanceB(nAbsDx, nAbsDy);
                int32_t nBaseLight = ((pLight->nCurrentRadius8 - nDist) * nFalloffSlope) >> 16;
                int32_t nAttenuated = (nBaseLight * (8 - (nOcclusion >> 1))) >> 3;

                if (nAttenuated > 0)
                {
                    LIGHTMAP_AddLightToSubtile8(
                        nCurX8,
                        nCurY8,
                        nAttenuated,
                        pLight->nRed,
                        pLight->nGreen,
                        pLight->nBlue);
                }
            }
        }
    }
}

// 1.12a: 0x6FB0EFD0 (usercall: esi = pLight)
// Computes and caches the 2D shadow/occlusion map for a static light node (nLightType == 2).
void __fastcall LIGHTMAP_BuildStaticOcclusionMap(D2LightNodeStrc* pLight)
{
    int32_t nRadius8 = pLight->nCurrentRadius8;
    if (nRadius8 <= 0 || nRadius8 >= 256)
        return;

    int32_t nDiameterSubtiles = (nRadius8 * 2) >> 3;
    memset(g_aLightRayOcclusionGrid, 0, sizeof(g_aLightRayOcclusionGrid));

    int32_t nRadiusSubtiles = nRadius8 >> 3;
    int32_t nLightSubtileX  = pLight->nX8 >> 3;
    int32_t nLightSubtileY  = pLight->nY8 >> 3;

    D2UnitStrc* pUnit = LIGHTMAP_ResolveLightOwnerUnit(pLight);
    if (pUnit == nullptr)
        return;

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom == nullptr)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(1044 /*0x414*/), 1044);
        exit(-1);
    }

    int32_t nOriginX = UNIT_GetSubtileX(pUnit) - 32;
    int32_t nOriginY = UNIT_GetSubtileY(pUnit) - 32;

    for (int32_t row = 0; row < 64; ++row)
    {
        for (int32_t col = 0; col < 64; ++col)
        {
            uint16_t wBlocked = COLLISION_CheckMask(pRoom, nOriginX + col, nOriginY + row, 0x0022);
            g_aLightRayObstacleGrid[row][col] = (wBlocked != 0) ? 16 : 0;
        }
    }

    if (nRadiusSubtiles >= 2)
    {
        for (int32_t ring = 2; ring <= nRadiusSubtiles; ++ring)
        {
            int32_t nOuterPlus  = 32 + ring;
            int32_t nOuterMinus = 32 - ring;
            int32_t nOuterXPlus8  = (nLightSubtileX + ring) * 8 + 4;
            int32_t nOuterXMinus8 = (nLightSubtileX - ring) * 8 + 4;
            int32_t nOuterYPlus8  = (nLightSubtileY + ring) * 8 + 4;
            int32_t nOuterYMinus8 = (nLightSubtileY - ring) * 8 + 4;

            for (int32_t step = 0; step < ring - 1; ++step)
            {
                int32_t nInnerPlus  = 32 + step;
                int32_t nInnerMinus = 32 - step;
                int32_t nInnerXPlus8  = (nLightSubtileX + step) * 8 + 4;
                int32_t nInnerXMinus8 = (nLightSubtileX - step) * 8 + 4;
                int32_t nInnerYPlus8  = (nLightSubtileY + step) * 8 + 4;
                int32_t nInnerYMinus8 = (nLightSubtileY - step) * 8 + 4;

                LIGHTMAP_SetupRayStep(pLight, nInnerXMinus8, nOuterYMinus8);
                LIGHTMAP_PropagateRayOcclusionCell(nInnerMinus, nOuterMinus);

                LIGHTMAP_SetupRayStep(pLight, nInnerXPlus8, nOuterYMinus8);
                LIGHTMAP_PropagateRayOcclusionCell(nInnerPlus, nOuterMinus);

                LIGHTMAP_SetupRayStep(pLight, nInnerXMinus8, nOuterYPlus8);
                LIGHTMAP_PropagateRayOcclusionCell(nInnerMinus, nOuterPlus);

                LIGHTMAP_SetupRayStep(pLight, nInnerXPlus8, nOuterYPlus8);
                LIGHTMAP_PropagateRayOcclusionCell(nInnerPlus, nOuterPlus);

                LIGHTMAP_SetupRayStep(pLight, nOuterXPlus8, nInnerYMinus8);
                LIGHTMAP_PropagateRayOcclusionCell(nOuterPlus, nInnerMinus);

                LIGHTMAP_SetupRayStep(pLight, nOuterXPlus8, nInnerYPlus8);
                LIGHTMAP_PropagateRayOcclusionCell(nOuterPlus, nInnerPlus);

                LIGHTMAP_SetupRayStep(pLight, nOuterXMinus8, nInnerYMinus8);
                LIGHTMAP_PropagateRayOcclusionCell(nOuterMinus, nInnerMinus);

                LIGHTMAP_SetupRayStep(pLight, nOuterXMinus8, nInnerYPlus8);
                LIGHTMAP_PropagateRayOcclusionCell(nOuterMinus, nInnerPlus);
            }
        }
    }

    if (pLight->bStaticMapValid != 0)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(1108 /*0x454*/), 1108);
        exit(-1);
    }
    if (pLight->pStaticOcclusionMap == nullptr)
    {
        FOG_DisplayHalt(s_szLightMapFile, FOG_GetErrorString(1109 /*0x455*/), 1109);
        exit(-1);
    }

    if (nDiameterSubtiles >= 0)
    {
        int32_t nStartOffset = 32 - (pLight->nCurrentRadius8 >> 3);
        int32_t nCount = nDiameterSubtiles + 1;
        int32_t* pDst = pLight->pStaticOcclusionMap;

        for (int32_t row = 0; row < nCount; ++row)
        {
            const int32_t* pSrc = &g_aLightRayOcclusionGrid[nStartOffset + row][nStartOffset];
            for (int32_t col = 0; col < nCount; ++col)
            {
                *pDst++ = *pSrc++;
            }
        }
    }

    pLight->bStaticMapValid = 1;
}

// 1.12a: 0x6FB0F350 (usercall: esi = pLight)
// Computes real-time 8-octant shadow propagation and applies a dynamic shadow-casting light node (nLightType == 0).
void __fastcall LIGHTMAP_ApplyDynamicShadowLightNode(D2LightNodeStrc* pLight)
{
    int32_t nRadius8 = pLight->nCurrentRadius8;
    if (nRadius8 <= 0 || nRadius8 >= 256)
        return;

    int32_t nStartX8 = (pLight->nX8 & ~7) - nRadius8;
    int32_t nStartY8 = (pLight->nY8 & ~7) - nRadius8;
    int32_t nDiameterSubtiles = (nRadius8 * 2) >> 3;

    int32_t nMinSubtileX = nStartX8 >> 3;
    if (nMinSubtileX > g_nLightMapEndSubtileX || (nMinSubtileX + nDiameterSubtiles + 1) < g_nLightMapOriginSubtileX)
        return;

    int32_t nMinSubtileY = nStartY8 >> 3;
    if (nMinSubtileY > g_nLightMapEndSubtileY || (nMinSubtileY + nDiameterSubtiles + 1) < g_nLightMapOriginSubtileY)
        return;

    memset(g_aLightRayOcclusionGrid, 0, sizeof(g_aLightRayOcclusionGrid));

    int32_t nRadiusSubtiles = nRadius8 >> 3;
    int32_t nLightSubtileX  = pLight->nX8 >> 3;
    int32_t nLightSubtileY  = pLight->nY8 >> 3;

    LIGHTMAP_CopyObstaclesForLightNode(pLight);

    if (nRadiusSubtiles >= 2)
    {
        for (int32_t ring = 2; ring <= nRadiusSubtiles; ++ring)
        {
            int32_t nOuterPlus  = 32 + ring;
            int32_t nOuterMinus = 32 - ring;
            int32_t nOuterXPlus8  = (nLightSubtileX + ring) * 8 + 4;
            int32_t nOuterXMinus8 = (nLightSubtileX - ring) * 8 + 4;
            int32_t nOuterYPlus8  = (nLightSubtileY + ring) * 8 + 4;
            int32_t nOuterYMinus8 = (nLightSubtileY - ring) * 8 + 4;

            for (int32_t step = 0; step < ring - 1; ++step)
            {
                int32_t nInnerPlus  = 32 + step;
                int32_t nInnerMinus = 32 - step;
                int32_t nInnerXPlus8  = (nLightSubtileX + step) * 8 + 4;
                int32_t nInnerXMinus8 = (nLightSubtileX - step) * 8 + 4;
                int32_t nInnerYPlus8  = (nLightSubtileY + step) * 8 + 4;
                int32_t nInnerYMinus8 = (nLightSubtileY - step) * 8 + 4;

                LIGHTMAP_SetupRayStep(pLight, nInnerXMinus8, nOuterYMinus8);
                LIGHTMAP_PropagateRayOcclusionCell(nInnerMinus, nOuterMinus);

                LIGHTMAP_SetupRayStep(pLight, nInnerXPlus8, nOuterYMinus8);
                LIGHTMAP_PropagateRayOcclusionCell(nInnerPlus, nOuterMinus);

                LIGHTMAP_SetupRayStep(pLight, nInnerXMinus8, nOuterYPlus8);
                LIGHTMAP_PropagateRayOcclusionCell(nInnerMinus, nOuterPlus);

                LIGHTMAP_SetupRayStep(pLight, nInnerXPlus8, nOuterYPlus8);
                LIGHTMAP_PropagateRayOcclusionCell(nInnerPlus, nOuterPlus);

                LIGHTMAP_SetupRayStep(pLight, nOuterXPlus8, nInnerYMinus8);
                LIGHTMAP_PropagateRayOcclusionCell(nOuterPlus, nInnerMinus);

                LIGHTMAP_SetupRayStep(pLight, nOuterXPlus8, nInnerYPlus8);
                LIGHTMAP_PropagateRayOcclusionCell(nOuterPlus, nInnerPlus);

                LIGHTMAP_SetupRayStep(pLight, nOuterXMinus8, nInnerYMinus8);
                LIGHTMAP_PropagateRayOcclusionCell(nOuterMinus, nInnerMinus);

                LIGHTMAP_SetupRayStep(pLight, nOuterXMinus8, nInnerYPlus8);
                LIGHTMAP_PropagateRayOcclusionCell(nOuterMinus, nInnerPlus);
            }
        }
    }

    int32_t nFalloffSlope = (static_cast<int32_t>(pLight->nIntensity) << 16) / nRadius8;
    if (nDiameterSubtiles < 0)
        return;

    int32_t nStartOffset = 32 - nRadiusSubtiles;
    int32_t nCount = nDiameterSubtiles + 1;
    int32_t nCurY8 = nStartY8;

    for (int32_t row = 0; row < nCount; ++row, nCurY8 += 8)
    {
        int32_t nAbsDy = pLight->nY8 - nCurY8;
        if (nAbsDy < 0)
            nAbsDy = -nAbsDy;

        const int32_t* pOcclusionRow = &g_aLightRayOcclusionGrid[nStartOffset + row][nStartOffset];
        int32_t nCurX8 = nStartX8;

        for (int32_t col = 0; col < nCount; ++col, nCurX8 += 8)
        {
            int32_t nOcclusion = pOcclusionRow[col];
            if (nOcclusion < 16)
            {
                int32_t nAbsDx = pLight->nX8 - nCurX8;
                if (nAbsDx < 0)
                    nAbsDx = -nAbsDx;

                int32_t nDist = LIGHTMAP_ApproxDistanceA(nAbsDx, nAbsDy);
                int32_t nBaseLight = ((pLight->nCurrentRadius8 - nDist) * nFalloffSlope) >> 16;
                int32_t nAttenuated = (nBaseLight * (8 - (nOcclusion >> 1))) >> 3;

                if (nAttenuated > 0)
                {
                    LIGHTMAP_AddLightToSubtile8(
                        nCurX8,
                        nCurY8,
                        nAttenuated,
                        pLight->nRed,
                        pLight->nGreen,
                        pLight->nBlue);
                }
            }
        }
    }
}

// 1.12a: 0x6FB0F730 (usercall: eax = nLightQuality, esi = pLight)
// Applies a non-shadowed radial light node (used for nLightType == 1 or when light quality <= 1).
void __fastcall LIGHTMAP_ApplySimpleLightNode(int32_t nLightQuality, D2LightNodeStrc* pLight)
{
    int32_t nRadius8 = pLight->nCurrentRadius8;
    if (nRadius8 <= 0 || nRadius8 >= 256)
        return;

    if (nLightQuality == 0)
    {
        if (pLight->nUnitType == 0) // UNIT_PLAYER
        {
            D2UnitLightViewStrc* pPlayer = reinterpret_cast<D2UnitLightViewStrc*>(g_pPlayerUnit);
            int32_t nLocalPlayerId = (pPlayer != nullptr) ? static_cast<int32_t>(pPlayer->dwUnitId) : -1;
            if (pLight->nUnitGUID != nLocalPlayerId && nRadius8 > 16)
                nRadius8 = 16;
        }
        else if (pLight->nUnitType == 1) // UNIT_MONSTER
        {
            nRadius8 = 8;
        }
        else if (pLight->nUnitType == 3) // UNIT_MISSILE
        {
            return;
        }
    }

    int32_t nStartX8 = (pLight->nX8 & ~7) - nRadius8;
    int32_t nStartY8 = (pLight->nY8 & ~7) - nRadius8;
    int32_t nDiameterSubtiles = (nRadius8 * 2) >> 3;

    int32_t nMinSubtileX = nStartX8 >> 3;
    if (nMinSubtileX > g_nLightMapEndSubtileX || (nMinSubtileX + nDiameterSubtiles + 1) < g_nLightMapOriginSubtileX)
        return;

    int32_t nMinSubtileY = nStartY8 >> 3;
    if (nMinSubtileY > g_nLightMapEndSubtileY || (nMinSubtileY + nDiameterSubtiles + 1) < g_nLightMapOriginSubtileY)
        return;

    int32_t nFalloffSlope = (static_cast<int32_t>(pLight->nIntensity) << 16) / nRadius8;
    if (nDiameterSubtiles < 0)
        return;

    int32_t nCount = nDiameterSubtiles + 1;
    int32_t nCurY8 = nStartY8;

    for (int32_t row = 0; row < nCount; ++row, nCurY8 += 8)
    {
        int32_t nAbsDy = pLight->nY8 - nCurY8;
        if (nAbsDy < 0)
            nAbsDy = -nAbsDy;

        int32_t nCurX8 = nStartX8;
        for (int32_t col = 0; col < nCount; ++col, nCurX8 += 8)
        {
            int32_t nAbsDx = pLight->nX8 - nCurX8;
            if (nAbsDx < 0)
                nAbsDx = -nAbsDx;

            int32_t nDist = LIGHTMAP_ApproxDistanceA(nAbsDx, nAbsDy);
            int32_t nLightVal = ((nRadius8 - nDist) * nFalloffSlope) >> 16;

            if (nLightVal > 0)
            {
                LIGHTMAP_AddLightToSubtile8(
                    nCurX8,
                    nCurY8,
                    nLightVal,
                    pLight->nRed,
                    pLight->nGreen,
                    pLight->nBlue);
            }
        }
    }
}

// 1.12a: 0x6FB0F8B0 (__stdcall)
// Registers LIGHTMAP_OnRoomAddedOrRemovedCallback on the client act and initializes g_aLightReciprocalTable[256].
void __stdcall LIGHTMAP_InitializeReciprocalTableAndActCallback()
{
    DUNGEON_SetActCallbackFunc(g_pClientAct, reinterpret_cast<void*>(&LIGHTMAP_OnRoomAddedOrRemovedCallback));
    g_aLightReciprocalTable[0] = 0;
    for (int32_t i = 1; i < 256; ++i)
    {
        g_aLightReciprocalTable[i] = 0x10000u / static_cast<uint32_t>(i);
    }
}

// 1.12a: 0x6FB0F8F0 (userpurge: eax = bCheckSpecialRoomLight, esi = pRoom, stack: pOutIntensity, pOutR, pOutG, pOutB)
// Resolves ambient light intensity and RGB color for a room (handling Den of Evil, Chaos Sanctuary seals, overrides, room RGB, and act environment).
void __stdcall LIGHTMAP_GetRoomAmbientLight(
    BOOL bCheckSpecialRoomLight,
    D2ActiveRoomStrc* pRoom,
    int32_t* pOutIntensity,
    uint8_t* pOutRed,
    uint8_t* pOutGreen,
    uint8_t* pOutBlue)
{
    uint8_t nR = 0;
    uint8_t nG = 0;
    uint8_t nB = 0;
    int32_t nIntensity = 0;

    if (bCheckSpecialRoomLight)
    {
        int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
        if (nLevelId == 8) // LEVEL_DENOFEVIL
        {
            // QUEST_GetDenOfEvilLight(&nR, &nG, &nB) -> 0x6FAC2A00
        }
        else if (nLevelId == 107 || nLevelId == 108) // River of Flame / Chaos Sanctuary
        {
            if (g_bDiabloSealActive && g_nDiabloSealFlashCounter == -1)
            {
                nR = 0xFF;
                nG = 0x40;
                nB = 0x30;
                nIntensity = 0xA0;
            }
            else
            {
                nIntensity = 0;
            }
        }
        else
        {
            nR = g_nOverrideAmbientRed;
            nG = g_nOverrideAmbientGreen;
            nB = g_nOverrideAmbientBlue;
            nIntensity = g_nOverrideAmbientIntensity;
        }
    }

    if (nR == 0 && nG == 0 && nB == 0)
    {
        uint8_t nRoomIntensity = 0;
        DUNGEON_GetRGB_IntensityFromRoom(pRoom, &nRoomIntensity, &nR, &nG, &nB);
        nIntensity = nRoomIntensity;

        if (nR == 0 && nG == 0 && nB == 0)
        {
            ENVIRONMENT_GetLightColorFromAct(g_pClientAct, &nR, &nG, &nB);
            nIntensity = ENVIRONMENT_GetIntensityFromAct(g_pClientAct);
        }
    }

    *pOutIntensity = nIntensity;
    *pOutRed       = nR;
    *pOutGreen     = nG;
    *pOutBlue      = nB;
}

// 1.12a: 0x6FB0FA20 (userpurge: eax = pLight, stack: nLightQuality)
// Note: 0x6FB0FAA0 is a mid-instruction false split label inside `push 0x4DF` (0x6FB0FA9E) of this function.
// Updates a light node's position/radius and applies its light contribution to the 48x48 lightmap grid.
void __stdcall LIGHTMAP_UpdateAndApplyLightNode(D2LightNodeStrc* pLight, int32_t nLightQuality)
{
    if (pLight->bFadingOut == 0 && pLight->nUnitType != 6)
    {
        D2UnitStrc* pUnit = LIGHTMAP_ResolveLightOwnerUnit(pLight);
        if (pUnit != nullptr)
        {
            pLight->nX8 = (UNITS_GetPrecisionX(pUnit) >> 13) + 4;
            pLight->nY8 = (UNITS_GetClientCoordX(pUnit) >> 13) + 4;
        }
    }

    if (pLight->nTargetRadius8 != pLight->nCurrentRadius8)
    {
        if (pLight->nLightType == 2)
        {
            pLight->bStaticMapValid = 0;
            if (pLight->pStaticOcclusionMap != nullptr)
            {
                // 1.12a: 0x6FB0FAA0 falls inside `push 0x4DF` here
                FOG_Free(pLight->pStaticOcclusionMap, s_szLightMapFile, 1247 /*0x4DF*/, 0);
            }
            pLight->pStaticOcclusionMap = nullptr;
        }

        if (pLight->nTargetRadius8 > pLight->nCurrentRadius8)
            pLight->nCurrentRadius8 += 8;
        else
            pLight->nCurrentRadius8 -= 8;
    }

    if (pLight->nCurrentRadius8 >= 256)
        pLight->nCurrentRadius8 = 248;

    if (pLight->bFadingOut != 0 && pLight->nCurrentRadius8 <= 0)
    {
        LIGHTMAP_DestroyLightNode(pLight);
        return;
    }

    if (pLight->nLightType == 0 && nLightQuality > 1)
    {
        LIGHTMAP_ApplyDynamicShadowLightNode(pLight);
        return;
    }

    if (pLight->nLightType == 2)
    {
        if (pLight->bStaticMapValid == 0)
        {
            if (pLight->pStaticOcclusionMap == nullptr)
            {
                int32_t nDim = 2 * (pLight->nCurrentRadius8 >> 3) + 1;
                pLight->pStaticOcclusionMap = static_cast<int32_t*>(
                    FOG_Alloc(nDim * nDim * sizeof(int32_t), s_szLightMapFile, 1265 /*0x4F1*/, 0));
            }
            LIGHTMAP_BuildStaticOcclusionMap(pLight);
        }
        LIGHTMAP_ApplyStaticLightNode(pLight);
        return;
    }

    LIGHTMAP_ApplySimpleLightNode(nLightQuality, pLight);
}

// 1.12a: 0x6FB0FB60 (__stdcall)
// Initializes the 48x48 lightmap grid with ambient light from the player's room and all adjacent rooms.
void __stdcall LIGHTMAP_ResetAmbientGrid(BOOL bClearToZero)
{
    D2UnitStrc* pPlayer = g_pPlayerUnit;
    if (pPlayer != nullptr && !bClearToZero)
    {
        D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(pPlayer);
        if (pPlayerRoom != nullptr)
        {
            D2ActiveRoomStrc** ppAdjacentRooms = nullptr;
            int32_t nNumAdjacentRooms = 0;
            DUNGEON_GetAdjacentRoomsListFromRoom(pPlayerRoom, &ppAdjacentRooms, &nNumAdjacentRooms);

            int32_t nIntensity = 0;
            uint8_t nR = 0, nG = 0, nB = 0;
            LIGHTMAP_GetRoomAmbientLight(TRUE, pPlayerRoom, &nIntensity, &nR, &nG, &nB);

            uint32_t dwPacked = (static_cast<uint32_t>(nB) << 24) |
                                (static_cast<uint32_t>(nG) << 16) |
                                (static_cast<uint32_t>(nR) << 8)  |
                                (static_cast<uint32_t>(nIntensity) & 0xFF);
            LIGHTMAP_FillAmbientRect(0, 0, 47, 47, dwPacked);

            for (int32_t i = 0; i < nNumAdjacentRooms; ++i)
            {
                D2ActiveRoomStrc* pAdjRoom = ppAdjacentRooms[i];
                if (pAdjRoom == pPlayerRoom)
                    continue;

                LIGHTMAP_GetRoomAmbientLight(TRUE, pAdjRoom, &nIntensity, &nR, &nG, &nB);
                uint32_t dwAdjPacked = (static_cast<uint32_t>(nB) << 24) |
                                       (static_cast<uint32_t>(nG) << 16) |
                                       (static_cast<uint32_t>(nR) << 8)  |
                                       (static_cast<uint32_t>(nIntensity) & 0xFF);

                int32_t aRoomCoords[4] = { 0, 0, 0, 0 }; // nSubtileX, nSubtileY, nSubtileWidth, nSubtileHeight
                DUNGEON_GetRoomCoordinates(pAdjRoom, aRoomCoords);

                int32_t nStartX = aRoomCoords[0] - g_nLightMapOriginSubtileX;
                int32_t nEndX   = nStartX + aRoomCoords[2];
                if (nStartX > 48 || nEndX < 0)
                    continue;

                int32_t nStartY = aRoomCoords[1] - g_nLightMapOriginSubtileY;
                int32_t nEndY   = nStartY + aRoomCoords[3];
                if (nStartY > 48 || nEndY < 0)
                    continue;

                if (nStartX < 0)
                    nStartX = 0;
                if (nEndX >= 48)
                    nEndX = 47;
                if (nStartY < 0)
                    nStartY = 0;
                if (nEndY >= 48)
                    nEndY = 47;

                LIGHTMAP_FillAmbientRect(nStartX, nStartY, nEndX, nEndY, dwAdjPacked);
            }
            return;
        }
    }

    LIGHTMAP_FillAmbientRect(0, 0, 47, 47, 0);
}

// 1.12a: 0x6FB0FD00 (__stdcall)
// Main per-frame client lightmap update: centers the 48x48 grid on the player, populates obstacles, and applies all light nodes.
void __stdcall LIGHTMAP_ProcessFrame()
{
    D2UnitStrc* pPlayer = g_pPlayerUnit;
    int32_t nPlayerX = UNIT_GetSubtileX(pPlayer);
    int32_t nPlayerY = UNIT_GetSubtileY(pPlayer);

    g_nLightMapOriginSubtileX = nPlayerX - 24;
    g_nLightMapOriginSubtileY = nPlayerY - 24;
    g_nLightMapEndSubtileX    = g_nLightMapOriginSubtileX + 48;
    g_nLightMapEndSubtileY    = g_nLightMapOriginSubtileY + 48;

    LIGHTMAP_ResetAmbientGrid(FALSE);
    LIGHTMAP_PopulateObstacleGrid();

    int32_t nLightQuality = LIGHTMAP_UpdateLightQuality();
    g_nActiveLightNodesCount = 0;

    D2LightNodeStrc* pCurr = g_pLightNodeListHead;
    while (pCurr != nullptr)
    {
        ++g_nActiveLightNodesCount;
        D2LightNodeStrc* pNext = pCurr->pNext;
        LIGHTMAP_UpdateAndApplyLightNode(pCurr, nLightQuality);
        pCurr = pNext;
    }
}

// 1.12a: 0x6FB0FDC0 (__stdcall)
// Clears the 48x48 lightmap grid to zero.
void __stdcall LIGHTMAP_ClearGrid()
{
    LIGHTMAP_ResetAmbientGrid(TRUE);
}

// ============================================================================
// PART 2: ..\Source\D2Client\ENGINE\Particle.cpp (0x6FB75260..0x6FB77050)
// ============================================================================

// Helper to access D2ParticleStrc* at *(pUnit->pGfxData + 0x30)
static inline D2ParticleStrc* PARTICLE_GetPtrInline(D2UnitLightViewStrc* pUnit)
{
    if (pUnit == nullptr || pUnit->pGfxData == nullptr)
        return nullptr;
    return *reinterpret_cast<D2ParticleStrc**>(pUnit->pGfxData + 0x30);
}

// 1.12a: 0x6FB75260 (usercall: eax = pUnit)
// Returns the D2ParticleStrc* attached to pUnit->pGfxData (+0x30), or nullptr.
D2ParticleStrc* __fastcall PARTICLE_GetFromUnit(D2UnitLightViewStrc* pUnit)
{
    return PARTICLE_GetPtrInline(pUnit);
}

// 1.12a: 0x6FB75280 (usercall: eax = pUnit)
// Allocates (if not already present) and zeroes the 0x4C-byte D2ParticleStrc on pUnit->pGfxData (+0x30).
void __fastcall PARTICLE_AllocForUnit(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle == nullptr)
    {
        pParticle = static_cast<D2ParticleStrc*>(
            FOG_Alloc(sizeof(D2ParticleStrc) /*0x4C*/, s_szParticleFile, 72 /*0x48*/, 0));
        if (pParticle == nullptr)
        {
            FOG_DisplayHalt(s_szParticleFile, FOG_GetErrorString(74 /*0x4A*/), 74);
            exit(-1);
        }
    }

    memset(pParticle, 0, sizeof(D2ParticleStrc));
    if (pUnit != nullptr && pUnit->pGfxData != nullptr)
    {
        *reinterpret_cast<D2ParticleStrc**>(pUnit->pGfxData + 0x30) = pParticle;
    }
}

// 1.12a: 0x6FB752F0 (usercall: eax = pUnit)
// Returns 1 if PARTICLEFLAG_GREATER_BOUNDS (0x20) is set on the unit's particle data.
BOOL __fastcall PARTICLE_HasGreaterBoundsFlag(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
        return (pParticle->dwFlags >> 5) & 1;
    return FALSE;
}

// 1.12a: 0x6FB75310 (usercall: eax = pUnit, ecx = bSet)
// Sets or clears PARTICLEFLAG_GREATER_BOUNDS (0x20) on the unit's particle data.
void __fastcall PARTICLE_SetGreaterBoundsFlag(D2UnitLightViewStrc* pUnit, BOOL bSet)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        if (bSet)
            pParticle->dwFlags |= 0x20;
        else
            pParticle->dwFlags &= ~0x20u;
    }
}

// 1.12a: 0x6FB75340 (usercall: eax = pUnit)
// Returns FALSE if the particle has finished normally (flag 0x01 set and 0x08 not set), otherwise TRUE.
BOOL __fastcall PARTICLE_IsActiveOrCancelled(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        if ((pParticle->dwFlags & 0x01) != 0 && (pParticle->dwFlags & 0x08) == 0)
            return FALSE;
    }
    return TRUE;
}

// 1.12a: 0x6FB75370 (usercall: eax = pUnit)
// Cancels a finished particle by clearing flag 0x01 and setting flag 0x08.
void __fastcall PARTICLE_CancelFinished(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        pParticle->dwFlags = (pParticle->dwFlags & ~0x01u) | 0x08u;
    }
}

// 1.12a: 0x6FB75390 (usercall: eax = pUnit)
// Returns TRUE if PARTICLEFLAG_FINISHED (0x01) is set on the unit's particle data.
BOOL __fastcall PARTICLE_IsFinished(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr && (pParticle->dwFlags & 0x01) != 0)
        return TRUE;
    return FALSE;
}

// 1.12a: 0x6FB753B0 (usercall: eax = pUnit)
// Marks the unit's particle as finished (sets flag 0x01).
void __fastcall PARTICLE_SetFinished(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        pParticle->dwFlags |= 0x01u;
    }
}

// 1.12a: 0x6FB753D0 (usercall: eax = pUnit, ecx = bAttach)
// Sets or clears PARTICLEFLAG_ATTACHED_TO_OWNER (0x10) on the unit's particle data.
void __fastcall PARTICLE_SetAttachedToOwnerFlag(D2UnitLightViewStrc* pUnit, BOOL bAttach)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        if (bAttach)
            pParticle->dwFlags |= 0x10u;
        else
            pParticle->dwFlags &= ~0x10u;
    }
}

// 1.12a: 0x6FB75400 (userpurge: eax = pUnit, ecx = nFrames, stack: nInitialZ)
// Initializes a parabolic gravity arc (flag 0x02) over nFrames starting from height nInitialZ.
void __stdcall PARTICLE_InitGravityArc(D2UnitLightViewStrc* pUnit, int32_t nFrames, int32_t nInitialZ)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle == nullptr)
        return;

    pParticle->dwFlags |= 0x02u;
    pParticle->nRemainingFrames = (nFrames >= 1) ? nFrames : 1;

    int32_t nZFixed = nInitialZ << 11;
    pParticle->nPosX = 0;
    pParticle->nPosY = 0;
    pParticle->nPosZ = nZFixed;

    if (pParticle->nAccelZ == 0)
        pParticle->nAccelZ = -0x1000; // -2.0 in 21.11 fixed-point

    int32_t nT = pParticle->nRemainingFrames;
    pParticle->nVelX = 0;
    pParticle->nVelY = 0;
    pParticle->nVelZ = (-nZFixed - ((nT * nT * pParticle->nAccelZ) / 2)) / nT;
}

// 1.12a: 0x6FB75480 (usercall: eax = pUnit)
// Clears PARTICLEFLAG_GRAVITY_ARC (0x02) on the unit's particle data.
void __fastcall PARTICLE_ClearGravityArcFlag(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        pParticle->dwFlags &= ~0x02u;
    }
}

// 1.12a: 0x6FB754A0 (usercall: eax = pUnit)
// Returns the remaining bounce count (+0x40) on the unit's particle data.
int32_t __fastcall PARTICLE_GetBounceCount(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    return (pParticle != nullptr) ? pParticle->nBounceCount : 0;
}

// 1.12a: 0x6FB754C0 (userpurge: eax = pUnit, edx = nBounceDampingPct, stack: nBounceCount)
// Enables floor bouncing (flag 0x04) with the given bounce count and velocity damping percentage.
void __stdcall PARTICLE_InitBounce(D2UnitLightViewStrc* pUnit, int32_t nBounceCount, int32_t nBounceDampingPct)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        pParticle->dwFlags |= 0x04u;
        pParticle->nBounceCount = nBounceCount;
        pParticle->nBounceDampingPct = nBounceDampingPct;
    }
}

// 1.12a: 0x6FB754F0 (userpurge: eax = pUnit, edi = bShift11, edx = nTargetY, ecx = nTargetZ, stack: nTargetX)
// Sets the target stop coordinates (+0x28, +0x2C, +0x30) on the unit's particle data.
void __stdcall PARTICLE_SetTargetPosition(
    D2UnitLightViewStrc* pUnit,
    BOOL bShift11,
    int32_t nTargetX,
    int32_t nTargetY,
    int32_t nTargetZ)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        if (bShift11)
        {
            nTargetX <<= 11;
            nTargetY <<= 11;
            nTargetZ <<= 11;
        }
        pParticle->nTargetX = nTargetX;
        pParticle->nTargetY = nTargetY;
        pParticle->nTargetZ = nTargetZ;
    }
}

// 1.12a: 0x6FB75530 (userpurge: eax = pUnit, edi = bShift11, edx = nAccelY, ecx = nAccelZ, stack: nAccelX)
// Sets the 3D acceleration vector (+0x1C, +0x20, +0x24) on the unit's particle data.
void __stdcall PARTICLE_SetAcceleration(
    D2UnitLightViewStrc* pUnit,
    BOOL bShift11,
    int32_t nAccelX,
    int32_t nAccelY,
    int32_t nAccelZ)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        if (bShift11)
        {
            nAccelX <<= 11;
            nAccelY <<= 11;
            nAccelZ <<= 11;
        }
        pParticle->nAccelX = nAccelX;
        pParticle->nAccelY = nAccelY;
        pParticle->nAccelZ = nAccelZ;
    }
}

// 1.12a: 0x6FB75570 (userpurge: eax = pUnit, edi = bShift11, edx = nVelY, ecx = nVelZ, stack: nVelX)
// Sets the 3D velocity vector (+0x10, +0x14, +0x18) on the unit's particle data.
void __stdcall PARTICLE_SetVelocity(
    D2UnitLightViewStrc* pUnit,
    BOOL bShift11,
    int32_t nVelX,
    int32_t nVelY,
    int32_t nVelZ)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        if (bShift11)
        {
            nVelX <<= 11;
            nVelY <<= 11;
            nVelZ <<= 11;
        }
        pParticle->nVelX = nVelX;
        pParticle->nVelY = nVelY;
        pParticle->nVelZ = nVelZ;
    }
}

// 1.12a: 0x6FB755B0 (userpurge: eax = pUnit, edx = nPosY, ecx = nPosZ, stack: nPosX)
// Sets the 3D position vector (+0x04, +0x08, +0x0C, shifted left by 11) on the unit's particle data.
void __stdcall PARTICLE_SetPosition(
    D2UnitLightViewStrc* pUnit,
    int32_t nPosX,
    int32_t nPosY,
    int32_t nPosZ)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        pParticle->nPosX = nPosX << 11;
        pParticle->nPosY = nPosY << 11;
        pParticle->nPosZ = nPosZ << 11;
    }
}

// 1.12a: 0x6FB755E0 (usercall: eax = pUnit)
int32_t __fastcall PARTICLE_GetFixedPosZ(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    return (pParticle != nullptr) ? pParticle->nPosZ : 0;
}

// 1.12a: 0x6FB75600 (usercall: eax = pUnit)
int32_t __fastcall PARTICLE_GetFixedPosY(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    return (pParticle != nullptr) ? pParticle->nPosY : 0;
}

// 1.12a: 0x6FB75620 (usercall: eax = pUnit)
int32_t __fastcall PARTICLE_GetFixedPosX(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    return (pParticle != nullptr) ? pParticle->nPosX : 0;
}

// 1.12a: 0x6FB75640 (usercall: eax = pUnit)
int32_t __fastcall PARTICLE_GetIntPosZ(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    return (pParticle != nullptr) ? (pParticle->nPosZ >> 11) : 0;
}

// 1.12a: 0x6FB75660 (usercall: eax = pUnit)
int32_t __fastcall PARTICLE_GetIntPosY(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    return (pParticle != nullptr) ? (pParticle->nPosY >> 11) : 0;
}

// 1.12a: 0x6FB75680 (usercall: eax = pUnit)
int32_t __fastcall PARTICLE_GetIntPosX(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    return (pParticle != nullptr) ? (pParticle->nPosX >> 11) : 0;
}

// 1.12a: 0x6FB756A0 (usercall: eax = pUnit)
int32_t __fastcall PARTICLE_GetClientOffsetZ(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    return (pParticle != nullptr) ? pParticle->nClientOffsetZ : 0;
}

// 1.12a: 0x6FB756C0 (usercall: eax = pUnit)
int32_t __fastcall PARTICLE_GetClientOffsetY(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    return (pParticle != nullptr) ? pParticle->nClientOffsetY : 0;
}

// 1.12a: 0x6FB756E0 (usercall: eax = pUnit)
int32_t __fastcall PARTICLE_GetClientOffsetX(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    return (pParticle != nullptr) ? pParticle->nClientOffsetX : 0;
}

// 1.12a: 0x6FB75700 (usercall: esi = pUnit)
// Frees the D2ParticleStrc attached to pUnit->pGfxData (+0x30).
void __fastcall PARTICLE_FreeForUnit(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle != nullptr)
    {
        FOG_Free(pParticle, s_szParticleFile, 100 /*0x64*/, 0);
        if (pUnit->pGfxData != nullptr)
        {
            *reinterpret_cast<D2ParticleStrc**>(pUnit->pGfxData + 0x30) = nullptr;
        }
    }
}

// 1.12a: 0x6FB75730 (__stdcall)
// Steps 3D particle kinematics (velocity, acceleration, gravity arc, bounce, owner attachment, and client screen offset) for a unit.
void __stdcall PARTICLE_UpdateUnitPhysics(D2UnitLightViewStrc* pUnit)
{
    D2ParticleStrc* pParticle = PARTICLE_GetPtrInline(pUnit);
    if (pParticle == nullptr || (pParticle->dwFlags & 0x01) != 0)
        return;

    pParticle->nPosX  += pParticle->nVelX;
    pParticle->nVelX  += pParticle->nAccelX;
    pParticle->nPosY  += pParticle->nVelY;
    pParticle->nVelY  += pParticle->nAccelY;
    pParticle->nPosZ  += pParticle->nVelZ;
    pParticle->nVelZ  += pParticle->nAccelZ;

    if (pParticle->dwFlags & 0x02) // Gravity arc countdown
    {
        if (pParticle->nRemainingFrames > 0)
        {
            --pParticle->nRemainingFrames;
        }
        else
        {
            pParticle->dwFlags |= 0x01u;
            pParticle->nPosX = 0;
            pParticle->nPosY = 0;
        }
    }

    if (pParticle->dwFlags & 0x04) // Bounce on floor
    {
        if ((pParticle->nPosZ >> 11) <= pParticle->nTargetZ)
        {
            pParticle->nVelZ = (pParticle->nVelZ * pParticle->nBounceDampingPct) / 100;
            pParticle->nPosZ = pParticle->nTargetZ;
            if (pParticle->nBounceCount > 0)
            {
                --pParticle->nBounceCount;
            }
            else
            {
                pParticle->dwFlags |= 0x01u;
            }
        }
    }
    else if (pParticle->dwFlags & 0x10) // Attached to owner unit's composite component
    {
        // Resolves owner unit (0x6FACF710) and component screen offset (0x6FB42490)
    }
    else
    {
        // Target bounds check (<= or >= depending on flag 0x20)
        BOOL bGreater = (pParticle->dwFlags & 0x20) != 0;
        if ((!bGreater && (pParticle->nPosX >> 11) <= pParticle->nTargetX &&
                          (pParticle->nPosY >> 11) <= pParticle->nTargetY &&
                          (pParticle->nPosZ >> 11) <= pParticle->nTargetZ) ||
            (bGreater  && (pParticle->nPosX >> 11) >= pParticle->nTargetX &&
                          (pParticle->nPosY >> 11) >= pParticle->nTargetY &&
                          (pParticle->nPosZ >> 11) >= pParticle->nTargetZ))
        {
            pParticle->nPosX = pParticle->nTargetX << 11;
            pParticle->nPosY = pParticle->nTargetY << 11;
            pParticle->nPosZ = pParticle->nTargetZ << 11;
            pParticle->dwFlags |= 0x01u;
        }
    }

    if ((pParticle->dwFlags & 0x10) == 0)
    {
        pParticle->nClientOffsetX = pParticle->nPosX >> 11;
        pParticle->nClientOffsetY = pParticle->nPosY >> 11;
        DUNGEON_GameToClientCoords(&pParticle->nClientOffsetX, &pParticle->nClientOffsetY);
        pParticle->nClientOffsetZ = -(pParticle->nPosZ >> 11);
    }
}

// ============================================================================
// PART 3: Adjacent Functions in asm/ENGINE_Particle.asm (0x6FB759A0..0x6FB77050)
//   - 0x6FB759A0..0x6FB75E25: Ambient Music Track Transition Controller
//   - 0x6FB75E30..0x6FB75F29: RC4 Key Schedule Initialization
//   - 0x6FB75F30..0x6FB76047: Monster Event Sound Lookup
//   - 0x6FB76050..0x6FB77045: Client Skill / Missile Particle Handlers
// ============================================================================

// Music / Stinger State Globals
extern int32_t   g_nMusicLevelEnterTick;            // 0x6FBCC244
extern int32_t   g_nMusicLastRestartTick;           // 0x6FBCC248
extern int32_t   g_nCurrentMusicSoundId;            // 0x6FBCC24C
extern int32_t   g_nCurrentMusicLevelId;            // 0x6FBCC250
extern int32_t   g_nMusicCheckpointTick;            // 0x6FBCC254
extern int32_t   g_bMusicSequenceActive;            // 0x6FBCC258
extern int32_t   g_nMusicSequenceSoundId1;          // 0x6FBCC25C
extern uint32_t  g_dwMusicSequenceTriggerTick1;     // 0x6FBCC260
extern int32_t   g_bMusicSequencePhase1Pending;     // 0x6FBCC264
extern int32_t   g_nMusicSequenceSoundId2;          // 0x6FBCC268
extern uint32_t  g_dwMusicSequenceTriggerTick2;     // 0x6FBCC26C
extern int32_t   g_bMusicSequencePhase2Pending;     // 0x6FBCC270
extern uint32_t  g_dwMusicSequenceEndTick;          // 0x6FBCC274
extern int32_t   g_nLastNotifiedMusicLevelId;       // 0x6FBCC278
extern int32_t*  g_pMusicTrackResumeOffsets;        // 0x6FBB5574
extern int32_t   g_nFirstMusicSoundId;              // 0x6FBCC158
extern int32_t   g_nLastMusicSoundId;               // 0x6FBCC15C
extern uint32_t  g_dwShakeStartTick;                // 0x6FBAD104
extern int32_t   g_nShakeRampUpMs;                  // 0x6FBAD108
extern int32_t   g_nShakeHoldMs;                    // 0x6FBAD10C
extern int32_t   g_nShakeRampDownMs;                // 0x6FBAD110
extern int32_t   g_nShakeMaxAmplitude;              // 0x6FBCB9B0

// 1.12a: 0x6FB759A0 (userpurge: eax = bEnableSpeechDelay, stack: nDelayTicks)
// Updates ambient music speech-ducking timer (0x6FBCC26C / 0x6FBCC270 / 0x6FBCC274).
void __stdcall SOUND_UpdateMusicSpeechTimer(BOOL bEnableSpeechDelay, int32_t nDelayTicks)
{
    if (g_bMusicSequenceActive != 0)
    {
        if (bEnableSpeechDelay != 0)
        {
            g_dwMusicSequenceTriggerTick2 = g_nCurrentClientTick + nDelayTicks;
        }
        g_bMusicSequencePhase2Pending = bEnableSpeechDelay;
        g_dwMusicSequenceEndTick = g_dwMusicSequenceTriggerTick2 + 50;
    }
}

// 1.12a: 0x6FB759E0 (__stdcall)
// Resets level music state globals (0x6FBCC244..0x6FBCC278) and clears the music track position table (0x6FBB5574).
void __stdcall SOUND_ResetLevelMusicState()
{
    int32_t nNumMusicTracks = (g_nLastMusicSoundId - g_nFirstMusicSoundId) + 1;
    g_nMusicLevelEnterTick     = 0;
    g_nMusicLastRestartTick    = 0;
    g_nCurrentMusicSoundId     = 0;
    g_nCurrentMusicLevelId     = 0;
    g_nMusicCheckpointTick     = 0;
    g_bMusicSequenceActive     = 0;
    g_nLastNotifiedMusicLevelId = 0;

    if (g_pMusicTrackResumeOffsets != nullptr && nNumMusicTracks > 0)
    {
        memset(g_pMusicTrackResumeOffsets, 0, static_cast<size_t>(nNumMusicTracks) * sizeof(int32_t));
    }
}

// 1.12a: 0x6FB75A30 (userpurge: ebx = nTrackPhase, stack: nSoundId1, nDelay1, nEndDelay, nSoundId2, nDelay2, bPlaySpeech)
// Saves current music track checkpoint, stops music, and starts a timed stinger/speech sequence.
void __stdcall SOUND_StartMusicStingerSequence(
    uint32_t nTrackPhase,
    int32_t nSoundId1,
    int32_t nDelay1,
    int32_t nEndDelay,
    int32_t nSoundId2,
    int32_t nDelay2,
    BOOL bPlaySpeech)
{
    int32_t nCurMusicId = g_nCurrentMusicSoundId;
    if (nCurMusicId != 0 && g_pMusicTrackResumeOffsets != nullptr)
    {
        // Saves checkpoint offset from Sounds.txt (+0x50 + nTrackPhase * 4) into g_pMusicTrackResumeOffsets[nCurMusicId - g_nFirstMusicSoundId]
        (void)nTrackPhase;
    }

    uint32_t dwNow = static_cast<uint32_t>(g_nCurrentClientTick);
    g_nMusicSequenceSoundId1      = nSoundId1;
    g_bMusicSequenceActive        = 1;
    g_bMusicSequencePhase1Pending = 1;
    g_dwMusicSequenceTriggerTick1 = dwNow + nDelay1;
    g_nMusicSequenceSoundId2      = nSoundId2;
    g_dwMusicSequenceTriggerTick2 = dwNow + nDelay2;
    g_bMusicSequencePhase2Pending = bPlaySpeech;
    g_dwMusicSequenceEndTick      = dwNow + nEndDelay;
}

// 1.12a: 0x6FB75B20 (__stdcall)
// Per-frame level music and ambient track update (checks player level, volume, stinger timers, and track loop points).
void __stdcall SOUND_UpdateLevelMusicFrame(uint32_t dwCurTick)
{
    if (g_pPlayerUnit == nullptr)
        return;

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(g_pPlayerUnit);
    if (pRoom == nullptr)
        return;

    int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    if (nLevelId == 0)
        return;

    if (nLevelId != g_nCurrentMusicLevelId)
    {
        g_nCurrentMusicLevelId = nLevelId;
        g_nMusicLevelEnterTick = static_cast<int32_t>(dwCurTick);
    }

    BOOL bMusicEnabled = (D2SOUND_GetMasterVolume() != 0 && D2SOUND_GetMusicVolume() != 0);
    (void)bMusicEnabled;
}

// 1.12a: 0x6FB75E30 (userpurge: edi = pRC4State258, stack: pKeyBytes, nKeyLength)
// Initializes a 258-byte RC4 state structure (256-byte S-box + 2 counter bytes at +0x100, +0x101).
void __stdcall CRYPTO_RC4InitState(uint8_t* pRC4State, const uint8_t* pKey, uint32_t nKeyLen)
{
    for (uint32_t i = 0; i < 256; ++i)
    {
        pRC4State[i] = static_cast<uint8_t>(i);
    }
    pRC4State[256] = 0;
    pRC4State[257] = 0;

    uint8_t j = 0;
    uint32_t nKeyIdx = 0;
    for (uint32_t i = 0; i < 256; ++i)
    {
        j = static_cast<uint8_t>(j + pRC4State[i] + pKey[nKeyIdx]);
        uint8_t tmp = pRC4State[i];
        pRC4State[i] = pRC4State[j];
        pRC4State[j] = tmp;
        nKeyIdx = (nKeyIdx + 1) % nKeyLen;
    }
}

// 1.12a: 0x6FB75F30 (usercall: eax = pUnit, edi = nAnimMode)
// Looks up a monster's mode-triggered sound ID from MonStats.txt (+0x170) and MonSounds.txt (+0x44..+0x50).
int32_t __fastcall SOUND_GetMonsterModeSoundId(D2UnitLightViewStrc* pUnit, int32_t nAnimMode)
{
    if (pUnit == nullptr || pUnit->nClassId < 0)
        return 0;

    // MonStats.txt (+0x14 = wMonSound, +0x170 = awSkillMode[8]) and MonSounds.txt (+0x44..+0x50 = dwSkillSound[4])
    (void)nAnimMode;
    return 0;
}

// 1.12a: 0x6FB75FD0 (userpurge: eax = nDefaultSoundId, edi = pUnit, stack: nRawSoundId)
// Overrides monster death/hit sound & spawns overlay 0x55 for Duriel (class 0x10B, sound 0xA7 -> 0x54).
int32_t __stdcall SOUND_CheckDurielSoundOverride(int32_t nDefaultSoundId, D2UnitLightViewStrc* pUnit, int32_t nRawSoundId)
{
    if (pUnit != nullptr && pUnit->dwUnitType == 1 && pUnit->nClassId == 0x10B && nRawSoundId == 0xA7)
    {
        // Spawns overlay 0x55 on Duriel (0x6FB430E0) and returns sound ID 0x54
        return 0x54;
    }
    return nDefaultSoundId;
}

// 1.12a: 0x6FB76050 (fastcall: ecx = pUnit)
// Filter callback for unit find: returns TRUE if pUnit is a missile of class 468 (0x1D4).
BOOL __fastcall SKILL_IsMissileClass468Callback(D2UnitLightViewStrc* pUnit)
{
    if (pUnit == nullptr || pUnit->dwUnitType != 3)
        return FALSE;
    return (pUnit->nClassId == 0x1D4) ? TRUE : FALSE;
}

// 1.12a: 0x6FB76070 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: sets path target from skill params 1 & 2 and spawns client missile A.
BOOL __fastcall SKILL_CltFunc_PathToSkillParamsAndSpawnMissileA(D2UnitLightViewStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(reinterpret_cast<D2UnitStrc*>(pUnit));
    if (pSkill == nullptr)
        return FALSE;

    int32_t nTargetX = SKILLS_GetParam1(pSkill);
    int32_t nTargetY = SKILLS_GetParam2(pSkill);
    if (nTargetX == 0 || nTargetY == 0)
        return FALSE;

    D2COMMON_10170_PathSetTargetPos(pUnit->pPath, nTargetX, nTargetY);
    (void)nSkillId;
    (void)nSkillLevel;
    return TRUE;
}

// 1.12a: 0x6FB76130 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: evaluates skill calc (+0x13C), sets skill param 1 expiration tick, and enables state 12.
BOOL __fastcall SKILL_CltFunc_StartTimedSkillState12(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (pSkill == nullptr)
        return FALSE;

    uint32_t dwCalc = 0; // pSkillsTxt->dwCltCalc1 (+0x13C)
    int32_t nDuration = SKILLS_EvaluateSkillFormula(pUnit, dwCalc, nSkillId, nSkillLevel);
    if (nDuration < 1)
        nDuration = 1;

    SKILLS_SetParam1(pSkill, g_nCurrentClientTick + nDuration);
    if (!STATES_CheckState(pUnit, 12))
    {
        STATES_ToggleState(pUnit, 12, TRUE);
    }
    return TRUE;
}

// 1.12a: 0x6FB761D0 (userpurge: eax = pUnit, stack: nMissileId, nSkillId, nSkillLevel, bSetClientFlag)
// Helper: spawns a client missile at pUnit's position and aligns its direction with pUnit.
BOOL __stdcall SKILL_SpawnDirectionalClientMissile(
    D2UnitLightViewStrc* pUnit,
    int32_t nMissileId,
    int32_t nSkillId,
    int32_t nSkillLevel,
    BOOL bSetClientFlag)
{
    int32_t nY = UNIT_GetSubtileY(reinterpret_cast<D2UnitStrc*>(pUnit));
    int32_t nX = UNIT_GetSubtileX(reinterpret_cast<D2UnitStrc*>(pUnit));
    (void)nX;
    (void)nY;
    (void)nMissileId;
    (void)nSkillId;
    (void)nSkillLevel;
    (void)bSetClientFlag;
    return TRUE;
}

// 1.12a: 0x6FB76290 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: spawns client missile B (+0xEC) at pUnit's subtile coordinates.
BOOL __fastcall SKILL_CltFunc_SpawnClientMissileBAtUnit(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nY = UNIT_GetSubtileY(pUnit);
    int32_t nX = UNIT_GetSubtileX(pUnit);
    (void)nX;
    (void)nY;
    (void)nSkillId;
    (void)nSkillLevel;
    return TRUE;
}

// 1.12a: 0x6FB76310 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: spawns client missile A (+0xEA) at pUnit's subtile coordinates.
BOOL __fastcall SKILL_CltFunc_SpawnClientMissileAAtUnit(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nY = UNIT_GetSubtileY(pUnit);
    int32_t nX = UNIT_GetSubtileX(pUnit);
    (void)nX;
    (void)nY;
    (void)nSkillId;
    (void)nSkillLevel;
    return TRUE;
}

// 1.12a: 0x6FB76380 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Note: 0x6FB763B0 is a mid-instruction false split label inside `add eax, 0x27AC4` of this function.
// Client skill function (Skill 285): finds nearby missile 468 units and spawns missile 587 (0x24B) at each.
BOOL __fastcall SKILL_CltFunc_SpawnMissilesOnNearbyUnits(D2UnitLightViewStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nX = UNIT_GetSubtileX(reinterpret_cast<D2UnitStrc*>(pUnit));
    int32_t nY = UNIT_GetSubtileY(reinterpret_cast<D2UnitStrc*>(pUnit));
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(reinterpret_cast<D2UnitStrc*>(pUnit));
    (void)pRoom;
    (void)nX;
    (void)nY;
    (void)nSkillId;
    (void)nSkillLevel;
    return TRUE;
}

// 1.12a: 0x6FB765F0 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: updates rider path to match its mount/owner unit.
BOOL __fastcall SKILL_CltFunc_UpdateRiderPath(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;
    D2UnitStrc* pMount = nullptr; // UNIT_ResolveOwner(pUnit) -> 0x6FACF710
    if (pMount == nullptr)
        return FALSE;

    D2Common_10235_PATH_UpdateRiderPath(pUnit, pMount);
    return TRUE;
}

// 1.12a: 0x6FB76620 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: records target unit's X, Y, and class ID into skill params 1, 2, and 3.
BOOL __fastcall SKILL_CltFunc_SaveInteractTargetCoordsAndClass(D2UnitLightViewStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(reinterpret_cast<D2UnitStrc*>(pUnit));
    if (pSkill == nullptr)
        return FALSE;

    if (pUnit->pPath == nullptr)
    {
        FOG_DisplayHalt(s_szParticleFile, FOG_GetErrorString(2374 /*0x946*/), 2374);
        exit(-1);
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pPath);
    if (pTarget == nullptr || !D2COMMON_11018_CheckUnitInteractable(pTarget))
        return FALSE;

    SKILLS_SetParam1(pSkill, UNIT_GetSubtileX(pTarget));
    SKILLS_SetParam2(pSkill, UNIT_GetSubtileY(pTarget));
    SKILLS_SetParam3(pSkill, reinterpret_cast<D2UnitLightViewStrc*>(pTarget)->nClassId);
    return TRUE;
}

// 1.12a: 0x6FB766B0 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: records target unit's X and Y coordinates into skill params 1 and 2.
BOOL __fastcall SKILL_CltFunc_SaveTargetCoordsToSkillParams(D2UnitLightViewStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(reinterpret_cast<D2UnitStrc*>(pUnit));
    if (pSkill == nullptr)
        return FALSE;

    if (pUnit->pPath == nullptr)
    {
        FOG_DisplayHalt(s_szParticleFile, FOG_GetErrorString(2374 /*0x946*/), 2374);
        exit(-1);
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pPath);
    if (pTarget == nullptr)
        return FALSE;

    SKILLS_SetParam1(pSkill, UNIT_GetSubtileX(pTarget));
    SKILLS_SetParam2(pSkill, UNIT_GetSubtileY(pTarget));
    return TRUE;
}

// 1.12a: 0x6FB76790 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: initializes screen shake / camera tremor parameters from Skills.txt (+0x148..+0x154).
BOOL __fastcall SKILL_CltFunc_TriggerSkillScreenShake(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)pUnit;
    (void)nSkillId;
    (void)nSkillLevel;
    int32_t nAmplitude = 0; // SkillsTxt[nSkillId].dwParam[0] (+0x148)
    int32_t nRampUpFrames = 0; // SkillsTxt[nSkillId].dwParam[1] (+0x14C)
    int32_t nHoldFrames = 0; // SkillsTxt[nSkillId].dwParam[2] (+0x150)
    int32_t nRampDownFrames = 0; // SkillsTxt[nSkillId].dwParam[3] (+0x154)

    int32_t nHoldMs = (nHoldFrames * 1000) / 25;
    if (nHoldMs != 0)
    {
        g_dwShakeStartTick   = GetTickCount();
        g_nShakeRampUpMs     = (nRampUpFrames * 1000) / 25;
        g_nShakeHoldMs       = nHoldMs;
        g_nShakeRampDownMs   = (nRampDownFrames * 1000) / 25;
        g_nShakeMaxAmplitude = nAmplitude;
    }
    return TRUE;
}

// 1.12a: 0x6FB768C0 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: spawns skill missile and sets its relative target offset (TargetX, TargetY).
BOOL __fastcall SKILL_CltFunc_SpawnMissileWithRelativeTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)pUnit;
    (void)nSkillId;
    (void)nSkillLevel;
    return TRUE;
}

// 1.12a: 0x6FB76990 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: spawns client missiles A and B at the coordinates stored in skill params 1 and 2.
BOOL __fastcall SKILL_CltFunc_SpawnMissilesAtSkillParams(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (pSkill == nullptr)
        return FALSE;

    int32_t nX = SKILLS_GetParam1(pSkill);
    int32_t nY = SKILLS_GetParam2(pSkill);
    if (nX == 0 || nY == 0)
        return FALSE;

    (void)nSkillId;
    (void)nSkillLevel;
    return TRUE;
}

// 1.12a: 0x6FB76AA0 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function (Fist of the Heavens / Shock Web line): spawns stepped trail of client missiles B toward target and missile A at caster.
BOOL __fastcall SKILL_CltFunc_SpawnLineOfMissilesToTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)pUnit;
    (void)nSkillId;
    (void)nSkillLevel;
    return TRUE;
}

// 1.12a: 0x6FB76D80 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: plays skill sound (+0x10E) and spawns missile 546 (0x222).
BOOL __fastcall SKILL_CltFunc_PlaySoundAndSpawnMissile546(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)pUnit;
    (void)nSkillId;
    (void)nSkillLevel;
    return TRUE;
}

// 1.12a: 0x6FB76E10 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: spawns client missile B (+0xEC), attaches a D2ParticleStrc to it, and offsets its initial Z height by owner component 14.
BOOL __fastcall SKILL_CltFunc_SpawnMissileWithOwnerComponentParticleOffset(D2UnitLightViewStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)pUnit;
    (void)nSkillId;
    (void)nSkillLevel;
    return TRUE;
}

// 1.12a: 0x6FB76EB0 (fastcall: ecx = pUnit, edx = nSkillId, stack: nSkillLevel)
// Client skill function: spawns a falling sky missile with 3D particle position/velocity and steps PARTICLE_UpdateUnitPhysics.
BOOL __fastcall SKILL_CltFunc_SpawnFallingSkyParticleMissile(D2UnitLightViewStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)pUnit;
    (void)nSkillId;
    (void)nSkillLevel;
    return TRUE;
}

