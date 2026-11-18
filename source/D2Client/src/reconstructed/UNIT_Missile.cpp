// ============================================================================
// Diablo II 1.12a - D2Client.dll Reconstructed Translation Unit
// Source File : ..\Source\D2Client\UNIT\Missile.cpp (Part 1: Core & Handlers)
// VA Slice    : 0x6FAD1880 .. 0x6FAD514F (ImageBase = 0x6FAB0000)
//
// Note on slice start:
//   0x6FAD1882..0x6FAD193B is the trailing half of D2Client_UpdateAllUnitsPerTick
//   (0x6FAD1840..0x6FAD193B) in ..\Source\D2Client\UNIT\CUnit.cpp.
//   The first procedure of ..\Source\D2Client\UNIT\Missile.cpp begins at 0x6FAD1940.
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <D2BasicTypes.h>
#include <D2CommonDefinitions.h>
#include <D2BitManip.h>
#include <D2Collision.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2Seed.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <DataTbls/MissilesIds.h>
#include <DataTbls/MissilesTbls.h>
#include <DataTbls/MonsterTbls.h>
#include <Path/Path.h>
#include <Path/PathMisc.h>
#include <Path/Step.h>
#include <Units/Missile.h>
#include <Units/MissileStream.h>
#include <Units/UnitRoom.h>
#include <Units/Units.h>
#include <Fog.h>
#include <FogTables.h>

// ============================================================================
// Client Unit Gfx / Particle / Light Auxiliary Layouts (1.12a)
// ============================================================================

#pragma pack(push, 1)

// 0x4C-byte 3D particle/kinematics block attached at pUnitGfxData->pParticle (ENGINE/Particle.cpp)
struct D2ParticleStrc
{
    uint32_t dwFlags;            // +0x00 (0x01=finished, 0x02=gravityArc, 0x04=bounce, 0x08=cancelled, 0x10=attached, 0x20=greaterBounds)
    int32_t  nPosX;              // +0x04 (fixed-point 21.11)
    int32_t  nPosY;              // +0x08 (fixed-point 21.11)
    int32_t  nPosZ;              // +0x0C (fixed-point 21.11)
    int32_t  nVelX;              // +0x10 (fixed-point 21.11)
    int32_t  nVelY;              // +0x14 (fixed-point 21.11)
    int32_t  nVelZ;              // +0x18 (fixed-point 21.11)
    int32_t  nAccelX;            // +0x1C (fixed-point 21.11)
    int32_t  nAccelY;            // +0x20 (fixed-point 21.11)
    int32_t  nAccelZ;            // +0x24 (fixed-point 21.11)
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
static_assert(sizeof(D2ParticleStrc) == 0x4C, "D2ParticleStrc must be 0x4C bytes");

// 0x3C-byte client unit graphics container pointed to by D2UnitStrc::pGfxData (+0x54)
struct D2ClientUnitGfxDataStrc
{
    uint16_t        aLayerFlags[20];     // +0x00 .. +0x27
    void*           pCofList;            // +0x28
    void*           pOverlayList;        // +0x2C
    D2ParticleStrc* pParticle;           // +0x30
    int32_t         nHeightIndex;        // +0x34
    int32_t         nDefaultHeightIndex; // +0x38
};
static_assert(offsetof(D2ClientUnitGfxDataStrc, pParticle) == 0x30, "pParticle must be at +0x30");
static_assert(sizeof(D2ClientUnitGfxDataStrc) == 0x3C, "D2ClientUnitGfxDataStrc must be 0x3C bytes");

// 0x34-byte client dynamic light node pointed to by D2UnitStrc::pLight (+0x64)
struct D2ClientLightNodeStrc
{
    int32_t                nUnitType;           // +0x00
    int32_t                nUnitGUID;           // +0x04
    uint8_t                bClientUnit;         // +0x08
    uint8_t                pad0x09[3];          // +0x09
    int32_t                nLightType;          // +0x0C
    int32_t                nX8;                 // +0x10
    int32_t                nY8;                 // +0x14
    int32_t                nCurrentRadius8;     // +0x18 (radius * 8)
    int32_t                nTargetRadius8;      // +0x1C (target radius * 8)
    int32_t                bFadingOut;          // +0x20
    uint8_t                nIntensity;          // +0x24
    uint8_t                nRed;                // +0x25
    uint8_t                nGreen;              // +0x26
    uint8_t                nBlue;               // +0x27
    D2ClientLightNodeStrc* pNext;               // +0x28
    int32_t                bStaticMapValid;     // +0x2C
    int32_t*               pStaticOcclusionMap; // +0x30
};
static_assert(offsetof(D2ClientLightNodeStrc, nCurrentRadius8) == 0x18, "nCurrentRadius8 must be at +0x18");

// Argument block passed to client missile collision unit-find callbacks (0x6FAD33BB..0x6FAD3420)
struct D2ClientMissileFindArgStrc
{
    D2UnitStrc*    pMissile;     // +0x00
    D2UnitStrc*    pOwner;       // +0x04
    D2MissilesTxt* pMissilesTxt; // +0x08
};
static_assert(sizeof(D2ClientMissileFindArgStrc) == 0x0C, "D2ClientMissileFindArgStrc must be 0x0C bytes");

// Entry in gClientMissileCollideTable (0x6FBA6780, 9 entries indexed by D2MissilesTxt::nCollideType)
using D2ClientMissileUnitFindFn_t = int32_t(__fastcall*)(D2UnitStrc* pUnit, void* pArg);
struct D2ClientMissileCollideEntryStrc
{
    D2ClientMissileUnitFindFn_t pfUnitFindCallback; // +0x00 (0x6FBA6780 + idx * 8)
    uint32_t                    dwMoveTestMask;     // +0x04 (0x6FBA6784 + idx * 8)
};

// Accumulator block passed to SKILLS_FindTargetsInRadius (0x6FAFE9B0) + 0x6FAFDB00 in 0x6FAD2CA0
struct D2GuidedTargetSearchStrc
{
    D2UnitStrc* pBestNextTarget; // +0x00
    uint32_t    dwMinGreaterGUID;// +0x04
    D2UnitStrc* pWrapTarget;     // +0x08
    uint32_t    dwMinOverallGUID;// +0x0C
    uint32_t    dwLastGUID;      // +0x10
    int32_t     nCandidatesFound;// +0x14
};
static_assert(sizeof(D2GuidedTargetSearchStrc) == 0x18, "D2GuidedTargetSearchStrc must be 0x18 bytes");

// Meteor Center debris directional pattern entry (0x6FB868B8)
struct D2MeteorDebrisPatternStrc
{
    int32_t nCount;    // +0x00
    int32_t aDirs[4];  // +0x04 .. +0x13
};

// Radament death missile spawn stage entry (0x6FB86290)
struct D2RadamentStageStrc
{
    int32_t nFrameThreshold; // +0x00
    int32_t nSpawnPeriod;    // +0x04
    int32_t nTargetXParam;   // +0x08
};

#pragma pack(pop)

static_assert(sizeof(D2MissileStrc) == 0x5C, "D2MissileStrc must be 0x5C bytes");
static_assert(sizeof(D2MissilesTxt) == 0x1A4, "D2MissilesTxt must be 0x1A4 bytes");

// ============================================================================
// Client Missile Dispatch Function Pointer Types & Tables
// ============================================================================

using D2CltMissileDoFunc_t  = void(__fastcall*)(D2UnitStrc* pMissile);
using D2CltMissileHitFunc_t = int32_t(__fastcall*)(D2UnitStrc* pMissile, D2UnitStrc* pTarget);

// 1.12a: 0x6FBA67C8 (91 / 0x5B slots; count at 0x6FBA6934)
extern const D2CltMissileDoFunc_t gCltMissileDoFuncTable[128];
extern const int32_t              gnCltMissileDoFuncs;   // 0x6FBA6934 (= 91 / 0x5B)

// 1.12a: 0x6FBA6938 (81 / 0x51 slots in 1.12a up to 0x6FBA6A7C; sized to 83 / 0x53 for safety)
extern const D2CltMissileHitFunc_t gCltMissileHitFuncTable[128];
extern const int32_t               gnCltMissileHitFuncs;  // 0x6FBA6A7C (= 81 / 0x51)

// ============================================================================
// D2MissileStrc::dwFlags Bit Constants (verified from 0x6FAD4B20)
// ============================================================================

enum D2ClientMissileInitFlags : uint32_t
{
    CLTMISSILEINIT_USE_COORDS        = 0x0001, // Use nX, nY instead of pOrigin coords
    CLTMISSILEINIT_REL_TARGET_COORDS = 0x0002, // Target = (nX + nTargetX, nY + nTargetY)
    CLTMISSILEINIT_CUSTOM_VELOCITY   = 0x0004, // Use pMissileInit->nVelocity
    CLTMISSILEINIT_ADD_SUBLOOP_RANGE = 0x0008, // Add nLoops * (nSubStop - nSubStart) to range
    CLTMISSILEINIT_RAW_VELOCITY_FP8  = 0x0010, // nVelocity is already 8-bit fixed point (skip << 8)
    CLTMISSILEINIT_ABS_TARGET_COORDS = 0x0020, // Target = (nTargetX, nTargetY)
    CLTMISSILEINIT_GRAVITY_ARC       = 0x0100, // Parabolic lob via PARTICLE_InitGravityArc(nGfxArg)
    CLTMISSILEINIT_START_FRAME       = 0x0200, // Pre-advance wFrame and subtract nFrame from range
    CLTMISSILEINIT_DIST_DIV_VEL      = 0x0400, // Recompute total frames from distance / velocity
    CLTMISSILEINIT_CUSTOM_ACTIVATE   = 0x0800, // Use pMissileInit->nActivateFrame instead of nActivate
    CLTMISSILEINIT_RANDOM_DIRECTION  = 0x2000, // Randomize initial facing direction (0..63)
    CLTMISSILEINIT_NO_LIGHT          = 0x4000, // Suppress missile dynamic light creation
    CLTMISSILEINIT_CUSTOM_RANGE      = 0x8000, // Use pMissileInit->nRange instead of wRange + lvl*wLevRange
};

// ============================================================================
// External Globals in D2Client.dll (1.12a addresses)
// ============================================================================

extern D2UnitStrc*    gpPlayerUnit;                          // 0x6FBCC3D0
extern D2DrlgActStrc* gpClientAct;                           // 0x6FBCC01C
extern int32_t        gnGameType;                            // 0x6FBCBFF8
extern int32_t        gnFrameDeltaTicks;                     // 0x6FBB32DC

extern D2UnitStrc*    gapClientSideUnitHashTables[6][128];   // 0x6FBC9D60 (UNIT_MISSILE @ 0x6FBCA360)
extern D2UnitStrc*    gapServerSideUnitHashTables[6][128];   // 0x6FBCA960

// Viewport & Screen Shake Globals
extern int32_t        gnViewportOffsetX_6FBC21D0;            // 0x6FBC21D0
extern int32_t        gnViewportOffsetY_6FBC21CC;            // 0x6FBC21CC
extern int32_t        gnViewportShiftOffsetX;                // 0x6FBCC1D4
extern int32_t        gnViewportWidth;                       // 0x6FBCD528
extern int32_t        gnViewportHeight;                      // 0x6FBCD524

extern int32_t        g_nShakeMaxAmplitude;                  // 0x6FBCB9B0
extern int32_t        g_nShakeOffsetX;                       // 0x6FBCB9B4
extern int32_t        g_nShakeOffsetY;                       // 0x6FBAD114
extern uint32_t       g_dwShakeStartTick;                    // 0x6FBAD104
extern int32_t        g_nShakeRampUpMs;                      // 0x6FBAD108
extern int32_t        g_nShakeHoldMs;                        // 0x6FBAD10C
extern int32_t        g_nShakeRampDownMs;                    // 0x6FBAD110

// ============================================================================
// External Helpers in D2Client.dll / Fog.dll (outside 0x6FAD1880..0x6FADDDA5)
// ============================================================================

// Fog.dll!#10265 (0x6FABBF8C)
extern "C" const char* __cdecl FOG_GetErrorString(int32_t nErrorCode);

// Engine / Math / Room / Position Helpers (0x6FAB1020..0x6FAB1A00)
extern uint32_t          __fastcall D2Client_RollRandom_6FAB1020(D2SeedStrc* pSeed, int32_t nMax);                       // 0x6FAB1020 (ABI: ecx=pSeed, eax=nMax)
extern uint32_t          __fastcall D2Client_RollRandom_6FAB1070(D2SeedStrc* pSeed, int32_t nMax);                       // 0x6FAB1070 (ABI: ecx=pSeed, esi=nMax)
extern int32_t           __fastcall D2Client_GetUnitX_6FAB1590(D2UnitStrc* pUnit);                                       // 0x6FAB1590 (ABI: ecx=pUnit)
extern int32_t           __fastcall D2Client_GetUnitY_6FAB15C0(D2UnitStrc* pUnit);                                       // 0x6FAB15C0 (ABI: ecx=pUnit)
extern int32_t           __fastcall ENGINE_SafeMulDiv(int32_t nValue, int32_t nMul, int32_t nDiv);                       // 0x6FAB18D0 (ABI: eax=nValue, edx=nMul, ecx=nDiv)
extern D2ActiveRoomStrc* __fastcall DUNGEON_FindRoomBySubtile(D2ActiveRoomStrc* pHintRoom, int32_t nX, int32_t nY);      // 0x6FAB1A00 (ABI: eax=pHintRoom, ebx=nX, edi=nY)

// Unit Management Helpers (UNIT/CUnit.cpp, 0x6FACEAA0..0x6FAD1320)
extern void        __fastcall D2Client_FreeUnitLight(D2UnitStrc* pUnit);                                                 // 0x6FACEAA0 (ABI: edi=pUnit)
extern int32_t     __fastcall D2Client_IsUnitDeadOrDying(D2UnitStrc* pUnit);                                             // 0x6FACEAC0 (ABI: eax=pUnit)
extern void        __fastcall D2Client_RemoveServerSideUnit(uint32_t nUnitType, uint32_t dwUnitGUID);                    // 0x6FACF180 (ABI: ecx=nUnitType, edx=dwUnitGUID)
extern D2UnitStrc* __fastcall D2Client_ClearActionFrameAndGetTarget(D2UnitStrc* pUnit);                                  // 0x6FACF5B0 (ABI: esi=pUnit)
extern D2UnitStrc* __fastcall D2Client_GetUnitOwner(D2UnitStrc* pUnit);                                                  // 0x6FACF710 (ABI: eax=pUnit)
extern int32_t     __stdcall  D2Client_AreUnitsHostileOrAttackable(D2UnitStrc* pTargetUnit, D2UnitStrc* pAttackerUnit);  // 0x6FACF980 (ABI: eax=pTargetUnit, stack=pAttackerUnit)
extern void        __fastcall D2Client_RemoveUnitFromHashTable(D2UnitStrc** ppHashTable, uint32_t nBucket, uint32_t dwUnitGUID, uint32_t nUnitType); // 0x6FAD0620
extern void        __fastcall D2Client_RemoveClientSideUnit(uint32_t nUnitType, uint32_t dwUnitGUID);                    // 0x6FAD0A70 (ABI: ecx=nUnitType, edx=dwUnitGUID)
extern D2UnitStrc* __fastcall D2Client_CreateClientSideUnit(int32_t nX, int32_t nY, int32_t nClassId, uint32_t nUnitType, int32_t nMode); // 0x6FAD1320 (raw ABI: ecx=nClassId, edx=nX, stack=nY, nUnitType, nMode)

// Sound Helpers (Sound/SoundHdr.cpp, 0x6FAE1FA0..0x6FAE6C10)
extern int32_t     __fastcall D2Client_PlaySound(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nTicksDelay, BOOL bLoop, uint32_t dwFlags); // 0x6FAE1FA0 (ABI: ebx=nSoundId, stack=pUnit, nTicksDelay, bLoop, dwFlags)
extern void        __fastcall D2Client_StopSoundHandle_6FAE2120(uint32_t dwSoundHandle);                                 // 0x6FAE2120 (ABI: edx=dwSoundHandle)
extern void        __stdcall  D2Client_DetachUnitFromSoundHandle_6FAE2150(uint32_t dwSoundHandle, D2UnitStrc* pUnit, BOOL bFadeOut); // 0x6FAE2150 (ABI: eax=dwSoundHandle, stack=pUnit, bFadeOut)
extern uint32_t    __fastcall D2Client_FindUnitActiveSoundHandle_6FAE6C10(D2UnitStrc* pUnit, int32_t nSoundId);          // 0x6FAE6C10 (ABI: ecx=pUnit, eax=nSoundId)

// UnitMode & Skill Helpers (0x6FAF0D20..0x6FAFE9B0)
extern void        __fastcall UNITMODE_ClearActionFrame(D2UnitStrc* pUnit);                                              // 0x6FAF0D20 (ABI: eax=pUnit)
extern int32_t     __fastcall SKILLS_TargetFindCallback_SelectNextByGUID_6FAFDB00(void* pUnitFindData, D2UnitStrc* pCandidate); // 0x6FAFDB00 (ABI: ecx=pUnitFindData, edx=pCandidate)
extern void        __stdcall  SKILLS_GetForwardOffsetCoords_6FAFDC40(D2UnitStrc* pUnit, int32_t* pOutX, int32_t* pOutY); // 0x6FAFDC40 (ABI: eax=pUnit, stack=pOutX, pOutY)
extern void        __stdcall  SKILLS_FindTargetsInRadius_6FAFE9B0(D2UnitStrc* pOwner, int32_t nX, int32_t nY, int32_t nRadius, uint32_t dwFilterFlags, void* pfnCallback, void* pCallbackArg, int32_t nExtra); // 0x6FAFE9B0

// LightMap & Overlay Helpers (0x6FB0E4C0..0x6FB430E0)
extern void        __fastcall DLIGHTMAP_StartFadeOutLight_6FB0E4C0(D2GfxLightStrc* pLight);                              // 0x6FB0E4C0 (ABI: eax=pLight)
extern void        __fastcall DLIGHTMAP_RemoveUnitLight(D2GfxLightStrc* pLight);                                         // 0x6FB0E500 (ABI: esi=pLight)
extern void        __fastcall D2Client_SetLightMapRadius_6FB0EAB0(void* pLight, int32_t nTargetRadius);                  // 0x6FB0EAB0 (ABI: ecx=pLight, eax=nTargetRadius)
extern void*       __stdcall  D2Client_AllocLightMap_6FB0EAF0(D2UnitStrc* pUnit, int32_t nLightType, int32_t nRadius, int32_t nIntensity, int32_t nRed, int32_t nGreen, int32_t nBlue); // 0x6FB0EAF0 (ABI: edi=pUnit, ebx=nLightType, stack=5 args)
extern void        __fastcall D2Client_FreeUnitOverlays_6FB40ED0(D2UnitStrc* pUnit);                                     // 0x6FB40ED0 (ABI: eax=pUnit)
extern void        __stdcall  D2Client_SetUnitOverlay_6FB430E0(int32_t nOverlayId, D2UnitStrc* pUnit, int32_t nFlags, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8); // 0x6FB430E0 (ABI: eax=nOverlayId, stack=7 args)

// Monster UMod, UnitEvent, Screen Shake, Roster, and Particle Helpers (0x6FB6A1F0..0x6FB75730)
extern void        __fastcall MONUNIQUE_TriggerUModEvent4(D2UnitStrc* pUnit, D2UnitStrc* pTargetUnit);                   // 0x6FB6A1F0 (ABI: eax=pUnit, ecx=pTargetUnit)
extern int32_t     __stdcall  UNITEVENT_TriggerEvent(D2UnitStrc* pUnit, int32_t nEventType, int32_t nParam1, int32_t nParam2); // 0x6FB6A250 (ABI: edi=pUnit, ebx=nEventType, stack=2 args)
void __stdcall VIEW_StartScreenShake_Stdcall(uint32_t dwSustainMs, int32_t nAmplitude, uint32_t dwAttackMs, uint32_t dwReleaseMs);
#define VIEW_StartScreenShake_Call VIEW_StartScreenShake_Stdcall // 0x6FB6A420 (ABI: esi=dwSustainMs, stack=3 args)
extern uint32_t    __fastcall ROSTERPETS_GetOwnerGUIDForPet(uint32_t dwPetGUID);                                         // 0x6FB73160 (ABI: ecx=dwPetGUID)

extern void        __fastcall PARTICLE_AllocForUnit(void* pUnit);                                                        // 0x6FB75280 (ABI: eax=pUnit)
extern void        __fastcall PARTICLE_CancelFinished(void* pUnit);                                                      // 0x6FB75370 (ABI: eax=pUnit)
extern void        __fastcall PARTICLE_SetFinished(void* pUnit);                                                         // 0x6FB753B0 (ABI: eax=pUnit)
extern void        __stdcall  PARTICLE_InitGravityArc(void* pUnit, int32_t nFrames, int32_t nInitialZ);                  // 0x6FB75400 (ABI: eax=pUnit, ecx=nFrames, stack=nInitialZ)
extern void        __stdcall  PARTICLE_SetPosition(void* pUnit, int32_t nPosX, int32_t nPosY, int32_t nPosZ);            // 0x6FB755B0 (ABI: eax=pUnit, edx=nPosY, ecx=nPosZ, stack=nPosX)
extern int32_t     __fastcall PARTICLE_GetIntPosZ(void* pUnit);                                                          // 0x6FB75640 (ABI: eax=pUnit)
extern int32_t     __fastcall PARTICLE_GetIntPosY(void* pUnit);                                                          // 0x6FB75660 (ABI: eax=pUnit)
extern int32_t     __fastcall PARTICLE_GetIntPosX(void* pUnit);                                                          // 0x6FB75680 (ABI: eax=pUnit)
extern void        __stdcall  PARTICLE_UpdateUnitPhysics(void* pUnit);                                                   // 0x6FB75730 (__stdcall)

// ============================================================================
// Forward Declarations within UNIT_Missile_Part1.cpp
// ============================================================================

D2UnitStrc* __fastcall D2Client_SpawnClientMissileAtCoords(D2UnitStrc* pOwner, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkillId, int32_t nSkillLevel);
void        __fastcall MISSILE_ToggleStateOffCallback(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList);
int32_t     __fastcall MISSILE_UnitFindCallback_CanMissileDestroy(D2UnitStrc* pUnit, void* pArg);
void        __fastcall MISSILE_RetargetGuidedMissile(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
D2UnitStrc* __fastcall D2Client_SpawnSubMissileFromMissile(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nTargetX, int32_t nTargetY, int32_t nSkillId, int32_t nSkillLevel);
void        __fastcall MISSILE_SpawnAlignedSubMissile(D2UnitStrc* pMissile, int32_t nSubMissileId);
void        __fastcall MISSILE_InitPierceCountOnMissile(D2UnitStrc* pOwner, D2UnitStrc* pMissile);
void        __fastcall MISSILE_UpdateFlickerLight(D2UnitStrc* pMissile);
BOOL        __fastcall MISSILE_CanCollideWithTargetUnit(D2UnitStrc* pTarget, D2UnitStrc* pOwner, const D2MissilesTxt* pMissilesTxt, D2UnitStrc* pMissile);
int32_t     __fastcall MISSILE_UnitFindCallback_CanCollideWithPlayerOrMonster(D2UnitStrc* pUnit, void* pArg);
int32_t     __fastcall MISSILE_UnitFindCallback_CanCollideWithGoodAlignmentUnit(D2UnitStrc* pUnit, void* pArg);
int32_t     __fastcall MISSILE_UnitFindCallback_CanCollideWithMonster(D2UnitStrc* pUnit, void* pArg);
int32_t     __fastcall MISSILE_TryAcquireGuidedTarget(D2UnitStrc* pMissile);
int32_t     __fastcall D2Client_DestroyClientMissile(D2UnitStrc* pMissile, D2UnitStrc* pTarget, int32_t bForceExpire);
void        __fastcall D2Client_FreeMissileUnitAndLight(D2UnitStrc* pMissile);
void        __fastcall D2Client_StepMissileMovementAndCollision(D2UnitStrc* pMissile);
void        __fastcall D2Client_DispatchMissileCltDoFunc(D2UnitStrc* pMissile);
D2UnitStrc* __fastcall D2Client_CreateClientMissile(D2MissileStrc* pMissileInit);

// ============================================================================
// Module-Static & Read-Only Tables (1.12a .data / .rdata)
// ============================================================================

static const char* const s_szMissileFile  = "..\\Source\\D2Client\\UNIT\\Missile.cpp";    // 0x6FB86C18
static const char* const s_szParticleFile = "..\\Source\\D2Client\\ENGINE\\Particle.cpp"; // 0x6FB85D80

// 1.12a: 0x6FBA6778 (initialized to 1)
static BOOL gbMissileLightEffectsEnabled = TRUE;

// 1.12a: 0x6FBA6780 (9 entries indexed by D2MissilesTxt::nCollideType)
static const D2ClientMissileCollideEntryStrc gClientMissileCollideTable[9] =
{
    /*[0]*/ { nullptr,                                                    COLLIDE_NONE },
    /*[1]*/ { MISSILE_UnitFindCallback_CanCollideWithGoodAlignmentUnit,   COLLIDE_PLAYER | COLLIDE_MISSILE_BARRIER },
    /*[2]*/ { MISSILE_UnitFindCallback_CanCollideWithMonster,             COLLIDE_MONSTER | COLLIDE_MISSILE_BARRIER },
    /*[3]*/ { MISSILE_UnitFindCallback_CanCollideWithPlayerOrMonster,     COLLIDE_MONSTER | COLLIDE_PLAYER | COLLIDE_MISSILE_BARRIER },
    /*[4]*/ { nullptr,                                                    COLLIDE_NONE },
    /*[5]*/ { MISSILE_UnitFindCallback_CanCollideWithMonster,             COLLIDE_MONSTER | COLLIDE_MISSILE_BARRIER },
    /*[6]*/ { nullptr,                                                    COLLIDE_MISSILE_BARRIER },
    /*[7]*/ { MISSILE_UnitFindCallback_CanMissileDestroy,                 COLLIDE_MISSILE },
    /*[8]*/ { MISSILE_UnitFindCallback_CanCollideWithPlayerOrMonster,     COLLIDE_MONSTER | COLLIDE_PLAYER | COLLIDE_MISSILE_BARRIER | COLLIDE_WALL },
};

// 1.12a: 0x6FB868B8..0x6FB86933 (Meteor Center debris patterns & 8-direction subtile offsets)
static const D2MeteorDebrisPatternStrc s_aMeteorDebrisPatterns[3] =
{
    { 4, { 0, 1, 4, 5 } }, // 0x6FB868B8
    { 4, { 0, 2, 4, 6 } }, // 0x6FB868CC
    { 3, { 0, 3, 5, 0 } }, // 0x6FB868E0
};
static const int32_t s_aMeteorOctantOffsetY[8] = {  1,  1,  0, -1, -1, -1,  0,  1 }; // 0x6FB868F4
static const int32_t s_aMeteorOctantOffsetX[8] = {  0,  1,  1,  1,  0, -1, -1, -1 }; // 0x6FB86914

// 1.12a: 0x6FB86290..0x6FB862D7 (Radament death stage thresholds & periods)
static const D2RadamentStageStrc s_aRadamentStageTable[6] =
{
    { 300,  25, 1 }, // 0x6FB86290
    { 250,  10, 3 }, // 0x6FB8629C
    { 100,   8, 4 }, // 0x6FB862A8
    {  75,   6, 4 }, // 0x6FB862B4
    {  30,   4, 3 }, // 0x6FB862C0
    {   0, 100, 0 }, // 0x6FB862CC
};

// ============================================================================
// Inline Typed Accessor Helpers (matching Blizzard's header inlines)
// ============================================================================

static inline D2MissilesTxt* MISSILE_GetMissilesTxtRecordInline(int32_t nMissileId)
{
    if (nMissileId >= 0 && sgptDataTables != nullptr && nMissileId < sgptDataTables->nMissilesTxtRecordCount)
    {
        return &sgptDataTables->pMissilesTxt[nMissileId];
    }
    return nullptr;
}

static inline D2ClientUnitGfxDataStrc* MISSILE_GetUnitGfxDataInline(D2UnitStrc* pUnit)
{
    return (pUnit != nullptr) ? reinterpret_cast<D2ClientUnitGfxDataStrc*>(pUnit->pGfxData) : nullptr;
}

static inline D2ParticleStrc* MISSILE_GetParticleInline(D2UnitStrc* pUnit)
{
    D2ClientUnitGfxDataStrc* pGfxData = MISSILE_GetUnitGfxDataInline(pUnit);
    return (pGfxData != nullptr) ? pGfxData->pParticle : nullptr;
}

static inline int32_t MISSILE_GetUnitSubtileXInline(D2UnitStrc* pUnit)
{
    if (pUnit->dwUnitType == UNIT_OBJECT || (pUnit->dwUnitType > UNIT_MISSILE && pUnit->dwUnitType <= UNIT_TILE))
    {
        return pUnit->pStaticPath->nXPos;
    }
    return (pUnit->pDynamicPath != nullptr) ? PATH_GetXPosition(pUnit->pDynamicPath) : 0;
}

static inline int32_t MISSILE_GetUnitSubtileYInline(D2UnitStrc* pUnit)
{
    if (pUnit->dwUnitType == UNIT_OBJECT || (pUnit->dwUnitType > UNIT_MISSILE && pUnit->dwUnitType <= UNIT_TILE))
    {
        return pUnit->pStaticPath->nYPos;
    }
    return (pUnit->pDynamicPath != nullptr) ? PATH_GetYPosition(pUnit->pDynamicPath) : 0;
}

static inline D2UnitStrc* MISSILE_FindClientMissileByGUIDInline(uint32_t dwUnitGUID)
{
    D2UnitStrc* pUnit = gapClientSideUnitHashTables[UNIT_MISSILE][dwUnitGUID & 0x7Fu];
    while (pUnit != nullptr)
    {
        if (pUnit->dwUnitId == dwUnitGUID)
        {
            return pUnit;
        }
        pUnit = pUnit->pListNext;
    }
    return nullptr;
}

// ============================================================================
// PART 1: CORE MISSILE CREATION, COLLISION, STEPPING, AND DISPATCH HANDLERS
// (0x6FAD1940 .. 0x6FAD514F, 68 Procedures)
// ============================================================================

// 1.12a: 0x6FAD1940
// Raw ABI: edx = pOwner, stack = [nMissileId, nX, nY, nSkillId, nSkillLevel] (ret 0x14)
D2UnitStrc* __fastcall D2Client_SpawnClientMissileAtCoords(
    D2UnitStrc* pOwner,
    int32_t nMissileId,
    int32_t nX,
    int32_t nY,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    if (pOwner == nullptr)
    {
        return nullptr;
    }

    D2MissileStrc missileInit = {};
    missileInit.dwFlags     = CLTMISSILEINIT_USE_COORDS;
    missileInit.pOwner      = pOwner;
    missileInit.nMissile    = nMissileId;
    missileInit.nX          = nX;
    missileInit.nY          = nY;
    missileInit.nSkill      = nSkillId;
    missileInit.nSkillLevel = nSkillLevel;

    return D2Client_CreateClientMissile(&missileInit);
}

// 1.12a: 0x6FAD19A0
// StatList remove callback for STATE_JUSTHIT (0x56) NextDelay expiration
void __fastcall MISSILE_ToggleStateOffCallback(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    D2_MAYBE_UNUSED(pStatList);
    STATES_ToggleState(pUnit, nState, FALSE);
}

// 1.12a: 0x6FAD19B0
// Raw ABI: eax = nRemainingFrames
int32_t __fastcall MISSILE_GetSmokeSpawnIntervalByRemainingFrames(int32_t nRemainingFrames)
{
    if (nRemainingFrames < 150)
    {
        return 20;
    }
    if (nRemainingFrames < 200)
    {
        return 15;
    }
    if (nRemainingFrames < 225)
    {
        return 10;
    }
    return (nRemainingFrames >= 375) ? 25 : 5;
}

// 1.12a: 0x6FAD19F0
BOOL __stdcall MISSILE_GetLightEffectsEnabled()
{
    return gbMissileLightEffectsEnabled;
}

// 1.12a: 0x6FAD1A00
// Raw ABI: eax = bEnabled
void __fastcall MISSILE_SetLightEffectsEnabled(BOOL bEnabled)
{
    gbMissileLightEffectsEnabled = bEnabled;
}

// 1.12a: 0x6FAD1A10
// Raw ABI: eax = nFrame, ecx = pMissilesTxt
int32_t __fastcall MISSILE_GetRampValueByFrameAndRange(int32_t nFrame, const D2MissilesTxt* pMissilesTxt)
{
    if (nFrame < 30)
    {
        return 1;
    }
    if (nFrame < 85)
    {
        return (nFrame - 15) / 4;
    }

    const int32_t nRange     = static_cast<int16_t>(pMissilesTxt->wRange);
    const int32_t nFadeStart = nRange - 50;
    if (nFrame < nFadeStart)
    {
        return 12;
    }
    if (nFrame < nRange)
    {
        return 12 - (nFrame - nFadeStart) / 4;
    }
    return 1;
}

// 1.12a: 0x6FAD1A60
// Raw ABI: eax = pMissile
int32_t __fastcall MISSILE_GetSpawnPeriodByRemainingFrames(D2UnitStrc* pMissile)
{
    const int32_t nElapsed = MISSILE_GetRemainingFrames(pMissile);
    if (nElapsed < 125)
    {
        return 25;
    }
    if (nElapsed < 175)
    {
        return 10;
    }
    return (nElapsed >= 300) ? 15 : 3;
}

// 1.12a: 0x6FAD1AA0
// Raw ABI: ecx = nOffsetX, edx = pRoom, eax = nOffsetY, stack = [nMaxDistSq, nOriginX, nOriginY] (ret 0x0C)
BOOL __fastcall MISSILE_CheckRayTraceWithinSquaredRadius(
    int32_t nOffsetX,
    D2ActiveRoomStrc* pRoom,
    int32_t nOffsetY,
    int32_t nMaxDistSq,
    int32_t nOriginX,
    int32_t nOriginY)
{
    if (pRoom == nullptr)
    {
        return FALSE;
    }

    if ((nOffsetX * nOffsetX + nOffsetY * nOffsetY) > nMaxDistSq)
    {
        return FALSE;
    }

    D2CoordStrc startCoord = { nOriginX, nOriginY };
    D2CoordStrc endCoord   = { nOriginX + nOffsetX, nOriginY + nOffsetY };
    return COLLISION_RayTrace(pRoom, &startCoord, &endCoord, COLLIDE_MISSILE_BARRIER) == 0;
}

// 1.12a: 0x6FAD1B10
// Raw ABI: esi = pMissile
// Returns bitmask: bit 1 (0x02) = trigger hit effects, bit 0 (0x01) = destroy missile (cleared when piercing)
int32_t __fastcall MISSILE_EvaluatePierceOnHit(D2UnitStrc* pMissile)
{
    int32_t nOutcome = 3;
    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(MISSILE_GetClassId(pMissile));
    if ((pMissilesTxt->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_PIERCE]) == 0)
    {
        return 3;
    }

    const int32_t nPierceRemaining = STATLIST_UnitGetStatValue(pMissile, STAT_PIERCE_IDX, 0);
    if (nPierceRemaining != 0)
    {
        STATLIST_SetUnitStat(pMissile, STAT_PIERCE_IDX, nPierceRemaining - 1, 0);
        nOutcome = 2;
    }
    return nOutcome;
}

// 1.12a: 0x6FAD1B80
// Raw ABI: esi = pMissile, stack = [pRoom, nX, nY] (ret 0x0C)
void __fastcall MISSILE_InitClientMissile(D2UnitStrc* pMissile, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY)
{
    if (pMissile == nullptr)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        return;
    }

    MISSILE_AllocMissileData(pMissile);
    UNITS_ChangeAnimMode(pMissile, pMissilesTxt->nCollideType);
    PATH_AllocDynamicPath(nullptr, pRoom, nX, nY, pMissile, TRUE);

    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    PATH_SetFootprintCollisionMask(pPath, pMissilesTxt->nCollision ? COLLIDE_MISSILE : COLLIDE_NONE);
    PATH_SetMoveTestCollisionMask(pPath, static_cast<int32_t>(gClientMissileCollideTable[pMissilesTxt->nCollideType].dwMoveTestMask));

    uint32_t dwFlags = pMissile->dwFlags;
    if ((pMissilesTxt->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_CANDESTROY]) != 0)
    {
        dwFlags |= UNITFLAG_CANBEATTACKED;
    }
    else
    {
        dwFlags &= ~static_cast<uint32_t>(UNITFLAG_CANBEATTACKED);
    }

    pMissile->dwFlags = dwFlags;
    pMissile->dwFlags = (dwFlags & ~static_cast<uint32_t>(UNITFLAG_TARGETABLE | UNITFLAG_ISVALIDTARGET)) | UNITFLAG_DOESNOTBLOCKLIGHT;
}

// 1.12a: 0x6FAD1C50
// Referenced by gClientMissileCollideTable[7] (0x6FBA67B8)
int32_t __fastcall MISSILE_UnitFindCallback_CanMissileDestroy(D2UnitStrc* pUnit, void* pArg)
{
    D2_MAYBE_UNUSED(pArg);
    if (pUnit == nullptr || pUnit->dwUnitType != UNIT_MISSILE)
    {
        return 0;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pUnit->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        return 0;
    }

    return ((pMissilesTxt->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_CANDESTROY]) != 0) ? 1 : 0;
}

// 1.12a: 0x6FAD1CA0
// Referenced by gCltMissileHitFuncTable[61] (0x6FBA6A2C)
int32_t __fastcall MISSILE_CltHit61_ClearScreenShake(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    D2_MAYBE_UNUSED(pMissile);
    if (pTarget == nullptr)
    {
        g_nShakeMaxAmplitude = 0;
        g_nShakeOffsetX      = 0;
        g_nShakeOffsetY      = 0;
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FAD1CC0
// MissileInitFunc callback for Charged Bolt (referenced at 0x6FADC5A6)
void __fastcall MISSILE_InitChargedBoltPath(D2UnitStrc* pMissile, int32_t nInitArg)
{
    D2_MAYBE_UNUSED(nInitArg);
    if (pMissile == nullptr)
    {
        return;
    }

    const int32_t nTotalFrames = MISSILE_GetTotalFrames(pMissile);
    const uint8_t nMaxDist     = (nTotalFrames > 255) ? 255 : static_cast<uint8_t>(nTotalFrames);

    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    SEED_InitLowSeed(&pMissile->pSeed, D2COMMON_10175_PathGetFirstPointX(pPath));
    PATH_SetType(pPath, PATHTYPE_CHARGEDBOLT);
    PATH_SetNewDistance(pPath, nMaxDist);
    D2Common_10142(pPath, pMissile, 0);
}

// 1.12a: 0x6FAD1D10
// Raw ABI: esi = pMissile, eax = pTarget
void __fastcall MISSILE_RetargetGuidedMissile(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (pMissile == nullptr)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        return;
    }

    if (pTarget != nullptr)
    {
        if ((pTarget->dwFlags & UNITFLAG_ISDEAD) != 0)
        {
            pTarget = nullptr;
        }
        else if (pTarget->dwUnitType == UNIT_PLAYER)
        {
            if (pTarget->dwAnimMode == PLRMODE_DEATH || pTarget->dwAnimMode == PLRMODE_DEAD)
            {
                pTarget = nullptr;
            }
        }
        else if (pTarget->dwUnitType == UNIT_MONSTER)
        {
            if (pTarget->dwAnimMode == MONMODE_DEATH || pTarget->dwAnimMode == MONMODE_DEAD)
            {
                pTarget = nullptr;
            }
        }
    }

    const int32_t nLevel  = static_cast<int32_t>(MISSILE_GetLevel(pMissile));
    const int32_t nFrames = static_cast<int16_t>(pMissilesTxt->wRange) + (nLevel - 1) * static_cast<int16_t>(pMissilesTxt->wLevRange);

    MISSILE_SetTotalFrames(pMissile, nFrames);
    MISSILE_SetCurrentFrame(pMissile, nFrames);

    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    int32_t nDistance = 0;

    if (pTarget != nullptr)
    {
        PATH_SetTargetUnit(pPath, pTarget);
        MISSILE_SetTargetX(pMissile, 5);
        nDistance = UNITS_GetDistanceToOtherUnit(pMissile, pTarget);
    }
    else
    {
        const uint32_t dwPackedOffset = static_cast<uint32_t>(MISSILE_GetTargetY(pMissile));
        const int32_t nDestX = MISSILE_GetUnitSubtileXInline(pMissile) + static_cast<int16_t>(dwPackedOffset & 0xFFFFu);
        const int32_t nDestY = MISSILE_GetUnitSubtileYInline(pMissile) + static_cast<int16_t>(dwPackedOffset >> 16);

        D2COMMON_10170_PathSetTargetPos(pPath, nDestX, nDestY);
        MISSILE_SetTargetX(pMissile, 6);
        nDistance = static_cast<int32_t>(UNITS_GetDistanceToCoordinates(pMissile, nDestX, nDestY));
    }

    if (nDistance < 25)
    {
        D2Common_10142(pPath, pMissile, 0);
    }
}

// 1.12a: 0x6FAD1E80
// Raw ABI: esi = pOrigin, stack = [nMissileId, nTargetX, nTargetY, nSkillId, nSkillLevel] (ret 0x14)
D2UnitStrc* __fastcall D2Client_SpawnSubMissileFromMissile(
    D2UnitStrc* pOrigin,
    int32_t nMissileId,
    int32_t nTargetX,
    int32_t nTargetY,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    if (pOrigin == nullptr)
    {
        return nullptr;
    }

    D2UnitStrc* pOwner = pOrigin;
    if (pOrigin->dwUnitType != UNIT_PLAYER && pOrigin->dwUnitType != UNIT_MONSTER)
    {
        pOwner = D2Client_GetUnitOwner(pOrigin);
        if (pOwner == nullptr)
        {
            return nullptr;
        }
    }

    D2MissileStrc missileInit = {};
    missileInit.dwFlags     = CLTMISSILEINIT_ABS_TARGET_COORDS;
    missileInit.pOwner      = pOwner;
    missileInit.pOrigin     = pOrigin;
    missileInit.nMissile    = nMissileId;
    missileInit.nTargetX    = nTargetX;
    missileInit.nTargetY    = nTargetY;
    missileInit.nSkill      = nSkillId;
    missileInit.nSkillLevel = nSkillLevel;

    return D2Client_CreateClientMissile(&missileInit);
}

// 1.12a: 0x6FAD1F00
// Referenced by gCltMissileHitFuncTable[39] (0x6FBA69D4)
int32_t __fastcall MISSILE_CltHit39_SpawnRandomNextSubMissile(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    D2_MAYBE_UNUSED(pTarget);
    const int32_t nCoinFlip = static_cast<int32_t>(SEED_RollRandomNumber(&pMissile->pSeed) & 1ULL);
    if (nCoinFlip <= 0)
    {
        const int32_t nBaseClassId = (pMissile != nullptr) ? pMissile->dwClassId : -1;
        const int32_t nVariantBit  = static_cast<int32_t>(SEED_RollRandomNumber(&pMissile->pSeed) & 1ULL);
        D2UnitStrc* pSubMissile = D2Client_SpawnSubMissileFromMissile(
            pMissile,
            nBaseClassId + nVariantBit + 1,
            0,
            0,
            MISSILE_GetSkill(pMissile),
            static_cast<int32_t>(MISSILE_GetLevel(pMissile)));

        if (pSubMissile != nullptr)
        {
            D2DynamicPathStrc* pSubPath = pSubMissile->pDynamicPath;
            PATH_SetDirection(pSubPath, PATH_GetDirection(pMissile->pDynamicPath));
            D2COMMON_10203_PATH_SetRotateFlag(pSubPath, FALSE);
        }
    }
    return 1;
}

// 1.12a: 0x6FAD1FA0
// Referenced by gCltMissileHitFuncTable[40] (0x6FBA69D8)
int32_t __fastcall MISSILE_CltHit40_FilterPlayerOwnPetCollision(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (pTarget == nullptr)
    {
        return 1;
    }

    D2UnitStrc* pOwner = D2Client_GetUnitOwner(pMissile);
    if (pOwner == nullptr)
    {
        return 1;
    }

    if (pOwner->dwUnitType != UNIT_PLAYER || pTarget->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    return (ROSTERPETS_GetOwnerGUIDForPet(pTarget->dwUnitId) == pOwner->dwUnitId) ? 1 : 0;
}

// 1.12a: 0x6FAD1FE0
// Referenced by gCltMissileHitFuncTable[31] (0x6FBA69B4)
int32_t __fastcall MISSILE_CltHit31_IceBreakMelt(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (pTarget != nullptr)
    {
        return 0;
    }

    int32_t nMeltMissileId = -1;
    switch (MISSILE_GetClassId(pMissile))
    {
    case MISSILE_ICEBREAKSMALL:
        nMeltMissileId = MISSILE_ICEBREAKSMALLMELT;
        break;
    case MISSILE_ICEBREAKMEDIUM:
    case MISSILE_ICEBREAKLARGE:
    case MISSILE_CATAPULT_COLD_EXPLOSION:
        nMeltMissileId = MISSILE_ICEBREAKLARGEMELT;
        break;
    default:
        break;
    }

    const int32_t nY = MISSILE_GetUnitSubtileYInline(pMissile);
    const int32_t nX = MISSILE_GetUnitSubtileXInline(pMissile);
    D2UnitStrc* pOwner = D2Client_GetUnitOwner(pMissile);

    D2UnitStrc* pMeltMissile = D2Client_SpawnClientMissileAtCoords(
        pOwner,
        nMeltMissileId,
        nX,
        nY,
        MISSILE_GetSkill(pMissile),
        static_cast<int32_t>(MISSILE_GetLevel(pMissile)));

    if (pMeltMissile != nullptr)
    {
        PATH_SetDirection(pMeltMissile->pDynamicPath, PATH_GetDirection(pMissile->pDynamicPath));
        UNITS_SetAnimationSpeed(pMeltMissile, 128);
    }

    return 1;
}

// 1.12a: 0x6FAD2170
// Referenced by gCltMissileHitFuncTable[44] (0x6FBA69E8)
int32_t __fastcall MISSILE_CltHit44_SpawnSubMissileCopyDirectionAndTargetX(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (pTarget != nullptr)
    {
        return 0;
    }

    if (pMissile != nullptr)
    {
        D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
        if (pMissilesTxt != nullptr && static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]) >= 0)
        {
            D2UnitStrc* pSubMissile = D2Client_SpawnSubMissileFromMissile(
                pMissile,
                static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]),
                0,
                0,
                MISSILE_GetSkill(pMissile),
                static_cast<int32_t>(MISSILE_GetLevel(pMissile)));

            if (pSubMissile != nullptr)
            {
                D2DynamicPathStrc* pSubPath = pSubMissile->pDynamicPath;
                PATH_SetDirection(pSubPath, PATH_GetDirection(pMissile->pDynamicPath));
                D2COMMON_10203_PATH_SetRotateFlag(pSubPath, FALSE);
                MISSILE_SetTargetX(pSubMissile, MISSILE_GetTargetX(pMissile));
            }
        }
    }

    return 1;
}

// 1.12a: 0x6FAD2210
// Referenced by gCltMissileHitFuncTable[34] (0x6FBA69C0)
int32_t __fastcall MISSILE_CltHit34_HideOwnerUnit(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    D2_MAYBE_UNUSED(pTarget);
    D2UnitStrc* pOwner = D2Client_GetUnitOwner(pMissile);
    if (pOwner != nullptr)
    {
        pOwner->dwFlagEx |= UNITFLAGEX_NODRAW;
    }
    return 1;
}

// 1.12a: 0x6FAD2230
// Referenced by gCltMissileHitFuncTable[24] (0x6FBA6998)
int32_t __fastcall MISSILE_CltHit24_SpawnSubMissileOnUnitHitOnly(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (pTarget == nullptr)
    {
        return 1;
    }

    if (pMissile != nullptr)
    {
        D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
        if (pMissilesTxt != nullptr && static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]) >= 0)
        {
            D2Client_SpawnSubMissileFromMissile(
                pMissile,
                static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]),
                0,
                0,
                MISSILE_GetSkill(pMissile),
                static_cast<int32_t>(MISSILE_GetLevel(pMissile)));
            return 0;
        }
    }

    return 1;
}

// 1.12a: 0x6FAD22A0
// Referenced by gCltMissileHitFuncTable[14] (0x6FBA6970)
int32_t __fastcall MISSILE_CltHit14_MeteorCenter(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    D2_MAYBE_UNUSED(pTarget);
    if (pMissile == nullptr)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        return 1;
    }

    const int32_t nX = MISSILE_GetUnitSubtileXInline(pMissile);
    const int32_t nY = MISSILE_GetUnitSubtileYInline(pMissile);

    if (static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]) >= 0)
    {
        D2Client_SpawnSubMissileFromMissile(
            pMissile,
            static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]),
            0,
            0,
            MISSILE_GetSkill(pMissile),
            static_cast<int32_t>(MISSILE_GetLevel(pMissile)));
    }

    if (static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[1]) >= 0)
    {
        const uint32_t nPatternIdx = static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 3u;
        const uint32_t nRotOffset  = static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) & 7u;

        const D2MeteorDebrisPatternStrc& pattern = s_aMeteorDebrisPatterns[nPatternIdx];
        for (int32_t i = 0; i < pattern.nCount; ++i)
        {
            const uint32_t nDirIdx = (static_cast<uint32_t>(pattern.aDirs[i]) + nRotOffset) & 7u;
            D2UnitStrc* pDebris = D2Client_SpawnSubMissileFromMissile(
                pMissile,
                static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[1]),
                0,
                0,
                MISSILE_GetSkill(pMissile),
                static_cast<int32_t>(MISSILE_GetLevel(pMissile)));

            if (pDebris != nullptr)
            {
                D2Common_10216(
                    pDebris->pDynamicPath,
                    nX + s_aMeteorOctantOffsetX[nDirIdx],
                    nY + s_aMeteorOctantOffsetY[nDirIdx],
                    0);
            }
        }
    }

    return 1;
}

// 1.12a: 0x6FAD2450
// Raw ABI: eax = pMissile, stack = [nSubMissileId] (ret 0x04)
void __fastcall MISSILE_SpawnAlignedSubMissile(D2UnitStrc* pMissile, int32_t nSubMissileId)
{
    D2UnitStrc* pSubMissile = D2Client_SpawnSubMissileFromMissile(
        pMissile,
        nSubMissileId,
        0,
        0,
        MISSILE_GetSkill(pMissile),
        static_cast<int32_t>(MISSILE_GetLevel(pMissile)));

    if (pSubMissile != nullptr)
    {
        PATH_SetDirection(pSubMissile->pDynamicPath, PATH_GetDirection(pMissile->pDynamicPath));
    }
}

// 1.12a: 0x6FAD2490
// Raw ABI: eax = pOwner, stack = [pMissile] (ret 0x04)
void __fastcall MISSILE_InitPierceCountOnMissile(D2UnitStrc* pOwner, D2UnitStrc* pMissile)
{
    if (pOwner != nullptr && pOwner->dwUnitType != UNIT_PLAYER && pOwner->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(MISSILE_GetClassId(pMissile));
    if ((pMissilesTxt->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_PIERCE]) == 0)
    {
        return;
    }

    int32_t nPierceChance = STATLIST_UnitGetItemStatOrSkillStatValue(pOwner, STAT_ITEM_PIERCE, 0)
                          + STATLIST_UnitGetStatValue(pOwner, STAT_SKILL_PIERCE, 0);
    if (nPierceChance == 0)
    {
        return;
    }

    int32_t nSeedInit = 0;
    if (pOwner != nullptr && pOwner->dwUnitType == UNIT_PLAYER && pOwner != gpPlayerUnit)
    {
        nPierceChance = 100;
        nSeedInit     = 0;
    }
    else
    {
        nSeedInit = STATLIST_GetUnitStatBonus(pOwner, STAT_PIERCE_IDX, 0);
    }

    D2SeedStrc pierceSeed = {};
    SEED_InitLowSeed(&pierceSeed, nSeedInit);

    int32_t nPierceCount = 0;
    while (nPierceCount < 4)
    {
        const int32_t nRoll = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pierceSeed)) % 100u);
        if (nRoll >= nPierceChance)
        {
            break;
        }
        ++nPierceCount;
    }

    STATLIST_SetUnitStat(pMissile, STAT_PIERCE_IDX, nPierceCount, 0);
}

// 1.12a: 0x6FAD25A0
// Referenced by gCltMissileHitFuncTable[48] (0x6FBA69F8)
int32_t __fastcall MISSILE_CltHit48_CatapultColdBall(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    D2_MAYBE_UNUSED(pTarget);

    const int32_t aDirX[8] = {  0,  1,  1,  1,  0, -1, -1, -1 };
    const int32_t aDirY[8] = {  1,  1,  0, -1, -1, -1,  0,  1 };

    const int32_t nX = MISSILE_GetUnitSubtileXInline(pMissile);
    const int32_t nY = MISSILE_GetUnitSubtileYInline(pMissile);

    D2Client_SpawnSubMissileFromMissile(
        pMissile,
        MISSILE_FREEZINGARROWEXP1,
        0,
        0,
        MISSILE_GetSkill(pMissile),
        static_cast<int32_t>(MISSILE_GetLevel(pMissile)));

    for (int32_t i = 0; i < 8; ++i)
    {
        const int32_t nDestX = nX + aDirX[i];
        const int32_t nDestY = nY + aDirY[i];

        D2UnitStrc* pDirBurst = D2Client_SpawnSubMissileFromMissile(
            pMissile,
            MISSILE_FREEZINGARROWEXP2,
            0,
            0,
            MISSILE_GetSkill(pMissile),
            static_cast<int32_t>(MISSILE_GetLevel(pMissile)));

        if (pDirBurst != nullptr)
        {
            D2Common_10216(pDirBurst->pDynamicPath, nDestX, nDestY, 0);
        }

        D2UnitStrc* pChunk = D2Client_SpawnSubMissileFromMissile(
            pMissile,
            MISSILE_CATAPULT_COLD_EXPLOSION,
            nDestX,
            nDestY,
            MISSILE_GetSkill(pMissile),
            static_cast<int32_t>(MISSILE_GetLevel(pMissile)));

        if (pChunk != nullptr)
        {
            const int32_t nTotalFrames = MISSILE_GetTotalFrames(pChunk);
            const int32_t nHalfFrames  = nTotalFrames / 2;
            const int32_t nFrameReduce = (nHalfFrames > 0)
                ? static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nHalfFrames))
                : 0;
            const int32_t nNewFrames = nTotalFrames - nFrameReduce;

            MISSILE_SetTotalFrames(pChunk, nNewFrames);

            D2DynamicPathStrc* pChunkPath = pChunk->pDynamicPath;
            const int32_t nMaxVel  = PATH_GetVelocity(pChunkPath);
            const int32_t nRandVel = (nMaxVel > 0)
                ? static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nMaxVel))
                : 0;

            PATH_SetVelocity(pChunkPath, nRandVel, s_szMissileFile, 6763);
            PARTICLE_InitGravityArc(pChunk, nNewFrames, 30);
        }
    }

    return 1;
}

// 1.12a: 0x6FAD27D0
// Raw ABI: ecx = pMissile, eax = nRemainingFrames
void __fastcall MISSILE_UpdateTowerMistAnimSpeedAndAscent(D2UnitStrc* pMissile, int32_t nRemainingFrames)
{
    if (nRemainingFrames == 8)
    {
        D2ParticleStrc* pParticle = MISSILE_GetParticleInline(pMissile);
        if (pParticle != nullptr)
        {
            pParticle->nAccelX = 0;
            pParticle->nAccelY = 0;
            pParticle->nAccelZ = 0x100;
        }
        UNITS_SetAnimationSpeed(pMissile, static_cast<int32_t>(pMissile->wAnimSpeed) / 2);
    }
    else if (nRemainingFrames == 35)
    {
        UNITS_SetAnimationSpeed(pMissile, static_cast<int32_t>(pMissile->wAnimSpeed) * 2);
    }
}

// 1.12a: 0x6FAD2820
void __fastcall MISSILE_UpdateFlickerLight(D2UnitStrc* pMissile)
{
    D2MissilesTxt* pMissilesTxt = (pMissile != nullptr)
        ? MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId)
        : nullptr;

    if ((pMissile->nSeqCurrentFramePrecise & 0x300) != 0)
    {
        return;
    }

    D2ClientLightNodeStrc* pLight = reinterpret_cast<D2ClientLightNodeStrc*>(pMissile->pLight);
    if (pLight == nullptr)
    {
        return;
    }

    if ((pLight->nCurrentRadius8 >> 3) >= static_cast<int32_t>(pMissilesTxt->nLight))
    {
        const int32_t nFlickerAdd = static_cast<int32_t>(D2Client_RollRandom_6FAB1070(&pMissile->pSeed, pMissilesTxt->nFlicker));
        D2Client_SetLightMapRadius_6FB0EAB0(pLight, static_cast<int32_t>(pMissilesTxt->nLight) + nFlickerAdd);
    }
}

// 1.12a: 0x6FAD28A0
// Raw ABI: esi = pMissile (matches UNIT_CUnit.cpp declaration)
void __fastcall MISSILE_FreeClientMissile(D2UnitStrc* pMissile)
{
    MISSTREAM_FreeMissileStream(pMissile);
    MISSILE_FreeMissileData(pMissile);

    D2ClientUnitGfxDataStrc* pGfxData = MISSILE_GetUnitGfxDataInline(pMissile);
    if (pGfxData != nullptr && pGfxData->pParticle != nullptr)
    {
        FOG_Free(pGfxData->pParticle, s_szParticleFile, 100, 0);
        pGfxData = MISSILE_GetUnitGfxDataInline(pMissile);
        if (pGfxData != nullptr)
        {
            pGfxData->pParticle = nullptr;
        }
    }

    D2Client_FreeUnitOverlays_6FB40ED0(pMissile);

    if (gpClientAct != nullptr)
    {
        D2Common_10228(pMissile);
        UNITROOM_RemoveUnitFromRoom(pMissile);
    }

    PATH_FreeDynamicPath(nullptr, pMissile->pDynamicPath);
    pMissile->pDynamicPath = nullptr;
}

// 1.12a: 0x6FAD2910
// Referenced by gCltMissileHitFuncTable[54] (0x6FBA6A10)
int32_t __fastcall MISSILE_CltHit54_SpawnSubMissileOnExpireOnly(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (pMissile != nullptr)
    {
        D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
        if (pMissilesTxt != nullptr && static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]) >= 0)
        {
            if (pTarget != nullptr)
            {
                return 0;
            }
            MISSILE_SpawnAlignedSubMissile(pMissile, static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]));
        }
    }
    return 1;
}

// 1.12a: 0x6FAD2970
// MissileInitFunc callback (referenced at 0x6FADA025 and 0x6FADACBF)
void __fastcall MISSILE_InitTrailSubMissileHeightAndFrame(D2UnitStrc* pMissile, int32_t nParentMissilePtr)
{
    if (pMissile == nullptr)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    D2UnitStrc* pParentMissile  = reinterpret_cast<D2UnitStrc*>(nParentMissilePtr);
    if (pMissilesTxt == nullptr || pParentMissile == nullptr)
    {
        return;
    }

    int32_t nZOffset = 0;
    const int32_t nZSpread = static_cast<int32_t>(pMissilesTxt->dwCltParam[1]);
    if (nZSpread > 0)
    {
        nZOffset = static_cast<int32_t>(D2Client_RollRandom_6FAB1020(&pParentMissile->pSeed, nZSpread))
                 - (static_cast<int32_t>(pMissilesTxt->dwCltParam[1]) / 2);
    }

    D2ParticleStrc* pParticle = MISSILE_GetParticleInline(pMissile);
    if (pParticle != nullptr)
    {
        pParticle->nPosX = 0;
        pParticle->nPosY = 0;
        pParticle->nPosZ = nZOffset << 11;
    }

    PATH_SetDirection(pMissile->pDynamicPath, PATH_GetDirection(pParentMissile->pDynamicPath));
    D2COMMON_10203_PATH_SetRotateFlag(pMissile->pDynamicPath, FALSE);

    const int32_t nRandFrame = static_cast<int32_t>(D2Client_RollRandom_6FAB1070(&pMissile->pSeed, pMissilesTxt->nAnimLen));
    pMissile->nSeqCurrentFramePrecise = nRandFrame << 8;
}

// 1.12a: 0x6FAD2A30
// Referenced by gCltMissileHitFuncTable[19] (0x6FBA6984)
int32_t __fastcall MISSILE_CltHit19_SpawnRandomRangeHitSubMissile(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    D2_MAYBE_UNUSED(pTarget);
    if (pMissile != nullptr)
    {
        D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
        if (pMissilesTxt != nullptr && static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]) >= 0)
        {
            const int32_t nMinId = static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]);
            const int32_t nMaxId = static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[1]);
            int32_t nSpawnId = nMinId;
            if (nMaxId > nMinId)
            {
                nSpawnId += static_cast<int32_t>(D2Client_RollRandom_6FAB1070(&pMissile->pSeed, nMaxId - nMinId + 1));
            }

            D2Client_SpawnSubMissileFromMissile(
                pMissile,
                nSpawnId,
                0,
                0,
                MISSILE_GetSkill(pMissile),
                static_cast<int32_t>(MISSILE_GetLevel(pMissile)));
        }
    }
    return 1;
}

// 1.12a: 0x6FAD2AB0
// Referenced by gCltMissileHitFuncTable[3] (0x6FBA6944)
int32_t __fastcall MISSILE_CltHit03_SpawnHitAndGroundDebrisSubMissiles(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    D2_MAYBE_UNUSED(pTarget);
    if (pMissile != nullptr)
    {
        D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
        if (pMissilesTxt != nullptr && static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]) >= 0)
        {
            D2Client_SpawnSubMissileFromMissile(
                pMissile,
                static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[0]),
                0,
                0,
                MISSILE_GetSkill(pMissile),
                static_cast<int32_t>(MISSILE_GetLevel(pMissile)));

            const int32_t nDebrisMin = static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[1]);
            if (nDebrisMin >= 0)
            {
                const int32_t nDebrisMax = static_cast<int16_t>(pMissilesTxt->wCltHitSubMissile[2]);
                int32_t nDebrisId = nDebrisMin;
                if (nDebrisMax > nDebrisMin)
                {
                    nDebrisId = nDebrisMin + static_cast<int32_t>(D2Client_RollRandom_6FAB1070(&pMissile->pSeed, nDebrisMax - nDebrisMin + 1));
                }

                D2UnitStrc* pDebris = D2Client_SpawnSubMissileFromMissile(
                    pMissile,
                    nDebrisId,
                    0,
                    0,
                    MISSILE_GetSkill(pMissile),
                    static_cast<int32_t>(MISSILE_GetLevel(pMissile)));

                if (pDebris != nullptr)
                {
                    pDebris->dwFlags |= UNITFLAG_ISDEAD;
                }
            }
        }
    }
    return 1;
}

// 1.12a: 0x6FAD2B80
// Raw ABI: esi = pTarget, edi = pOwner, ebx = pMissilesTxt, stack = [pMissile] (ret 0x04)
BOOL __fastcall MISSILE_CanCollideWithTargetUnit(
    D2UnitStrc* pTarget,
    D2UnitStrc* pOwner,
    const D2MissilesTxt* pMissilesTxt,
    D2UnitStrc* pMissile)
{
    if (pTarget == nullptr)
    {
        return FALSE;
    }

    if ((pTarget->dwFlags & UNITFLAG_ISVALIDTARGET) == 0 || (pTarget->dwFlags & UNITFLAG_CANBEATTACKED) == 0)
    {
        return FALSE;
    }

    if (pMissilesTxt->nNextHit != 0 && STATES_CheckState(pTarget, STATE_JUSTHIT))
    {
        return FALSE;
    }

    if (MISSILE_CheckUnitIfOwner(pMissile, pTarget))
    {
        return FALSE;
    }

    if (pOwner != nullptr && !D2Client_AreUnitsHostileOrAttackable(pTarget, pOwner) && pMissilesTxt->nCollideFriend == 0)
    {
        return FALSE;
    }

    return TRUE;
}

// 1.12a: 0x6FAD2C00
// Referenced by gClientMissileCollideTable[3] & [8] (0x6FBA6798, 0x6FBA67C0)
int32_t __fastcall MISSILE_UnitFindCallback_CanCollideWithPlayerOrMonster(D2UnitStrc* pUnit, void* pArg)
{
    if (pUnit == nullptr || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER))
    {
        return 0;
    }

    const D2ClientMissileFindArgStrc* pFindArg = static_cast<const D2ClientMissileFindArgStrc*>(pArg);
    return MISSILE_CanCollideWithTargetUnit(pUnit, pFindArg->pOwner, pFindArg->pMissilesTxt, pFindArg->pMissile) ? 1 : 0;
}

// 1.12a: 0x6FAD2C30
// Referenced by gClientMissileCollideTable[1] (0x6FBA6788)
int32_t __fastcall MISSILE_UnitFindCallback_CanCollideWithGoodAlignmentUnit(D2UnitStrc* pUnit, void* pArg)
{
    if (pUnit == nullptr)
    {
        return 0;
    }

    if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        return (STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_GOOD) ? 1 : 0;
    }

    if (pUnit->dwUnitType != UNIT_PLAYER)
    {
        return 0;
    }

    const D2ClientMissileFindArgStrc* pFindArg = static_cast<const D2ClientMissileFindArgStrc*>(pArg);
    return MISSILE_CanCollideWithTargetUnit(pUnit, pFindArg->pOwner, pFindArg->pMissilesTxt, pFindArg->pMissile) ? 1 : 0;
}

// 1.12a: 0x6FAD2C70
// Referenced by gClientMissileCollideTable[2] & [5] (0x6FBA6790, 0x6FBA67A8)
int32_t __fastcall MISSILE_UnitFindCallback_CanCollideWithMonster(D2UnitStrc* pUnit, void* pArg)
{
    if (pUnit == nullptr || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    const D2ClientMissileFindArgStrc* pFindArg = static_cast<const D2ClientMissileFindArgStrc*>(pArg);
    return MISSILE_CanCollideWithTargetUnit(pUnit, pFindArg->pOwner, pFindArg->pMissilesTxt, pFindArg->pMissile) ? 1 : 0;
}

// 1.12a: 0x6FAD2CA0
// Raw ABI: eax = pMissile
int32_t __fastcall MISSILE_TryAcquireGuidedTarget(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        return 1;
    }

    const int32_t nSearchRadius = pMissilesTxt->dwParam[1];
    if ((MISSILE_GetTargetX(pMissile) & 0x04) != 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = D2Client_GetUnitOwner(pMissile);
    if (pOwner == nullptr)
    {
        return 1;
    }

    const int32_t nY = D2Client_GetUnitY_6FAB15C0(pMissile);
    const int32_t nX = D2Client_GetUnitX_6FAB1590(pMissile);

    D2GuidedTargetSearchStrc search = {};
    search.pBestNextTarget  = nullptr;
    search.dwMinGreaterGUID = 0xFFFFFFFFu;
    search.pWrapTarget      = nullptr;
    search.dwMinOverallGUID = 0xFFFFFFFFu;
    search.dwLastGUID       = 0xFFFFFFFFu;
    search.nCandidatesFound = 0;

    SKILLS_FindTargetsInRadius_6FAFE9B0(
        pOwner,
        nX,
        nY,
        nSearchRadius,
        0xA283u,
        reinterpret_cast<void*>(&SKILLS_TargetFindCallback_SelectNextByGUID_6FAFDB00),
        &search,
        0);

    D2UnitStrc* pSelectedTarget = (search.pBestNextTarget != nullptr) ? search.pBestNextTarget : search.pWrapTarget;
    MISSILE_RetargetGuidedMissile(pMissile, pSelectedTarget);
    return 0;
}

// 1.12a: 0x6FAD2D80
// Referenced by gCltMissileHitFuncTable[13] (0x6FBA696C)
int32_t __fastcall MISSILE_CltHit13_GuidedArrow_BoneSpirit(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    D2UnitStrc* pOwner       = D2Client_GetUnitOwner(pMissile);

    if (pOwner == nullptr || (pOwner->dwFlags & UNITFLAG_ISDEAD) != 0)
    {
        return 1;
    }

    if (pOwner->dwUnitType == UNIT_PLAYER)
    {
        if (pOwner->dwAnimMode == PLRMODE_DEATH || pOwner->dwAnimMode == PLRMODE_DEAD)
        {
            return 1;
        }
    }
    else if (pOwner->dwUnitType == UNIT_MONSTER)
    {
        if (pOwner->dwAnimMode == MONMODE_DEATH || pOwner->dwAnimMode == MONMODE_DEAD)
        {
            return 1;
        }
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
    if (pRoom != nullptr && DUNGEON_IsRoomInTown(pRoom))
    {
        return 1;
    }

    const int32_t nGuidedFlags = MISSILE_GetTargetX(pMissile);
    if (pTarget == nullptr && (nGuidedFlags & 0x04) != 0)
    {
        return 1;
    }

    if ((nGuidedFlags & 0x01) != 0)
    {
        if (pTarget == nullptr)
        {
            return 1;
        }
        return (pTarget == PATH_GetTargetUnit(pPath)) ? 1 : 0;
    }

    if ((nGuidedFlags & 0x02) == 0)
    {
        return 1;
    }

    if (MISSILE_GetCurrentFrame(pMissile) <= 0)
    {
        return MISSILE_TryAcquireGuidedTarget(pMissile) ? 1 : 0;
    }

    return (static_cast<uint8_t>(nGuidedFlags) >> 2) & 1;
}

// 1.12a: 0x6FAD2E50
// Raw ABI: ecx = pMissile, eax = pTarget, stack = [bForceExpire] (ret 0x04)
// Returns EAX = spawned explosion missile pointer (or 0)
int32_t __fastcall D2Client_DestroyClientMissile(D2UnitStrc* pMissile, D2UnitStrc* pTarget, int32_t bForceExpire)
{
    D2MissilesTxt* pMissilesTxt = (pMissile != nullptr)
        ? MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId)
        : nullptr;

    const int32_t nExplosionMissileId = static_cast<int16_t>(pMissilesTxt->wExplosionMissile);
    int32_t nHitOutcome = 3;
    D2UnitStrc* pOwner  = D2Client_GetUnitOwner(pMissile);
    D2UnitStrc* pExplosionMissile = nullptr;

    if (pMissile == nullptr)
    {
        return 0;
    }

    if (pTarget != nullptr)
    {
        if (pMissilesTxt->nNextHit != 0 && STATES_CheckState(pTarget, STATE_JUSTHIT))
        {
            return 0;
        }

        if (MISSILE_CheckUnitIfOwner(pMissile, pTarget))
        {
            return 0;
        }

        MISSILE_SetOwner(pMissile, pTarget);

        if ((pTarget->dwFlags & UNITFLAG_CANBEATTACKED) == 0)
        {
            return 0;
        }

        if (pOwner != nullptr && !D2Client_AreUnitsHostileOrAttackable(pTarget, pOwner) && pMissilesTxt->nCollideFriend == 0)
        {
            return 0;
        }
    }

    if (pOwner != nullptr && pTarget != nullptr)
    {
        if ((pMissilesTxt->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_PIERCE]) != 0
            && (pOwner->dwUnitType == UNIT_PLAYER || pOwner->dwUnitType == UNIT_MONSTER))
        {
            if (STATES_CheckState(pOwner, STATE_PIERCE)
                || STATLIST_UnitGetItemStatOrSkillStatValue(pOwner, STAT_ITEM_PIERCE, 0) != 0)
            {
                nHitOutcome = MISSILE_EvaluatePierceOnHit(pMissile);
            }
        }
    }

    if ((pTarget != nullptr || bForceExpire != 0 || pMissilesTxt->nAlwaysExplode != 0)
        && (nHitOutcome & 0x02) != 0)
    {
        if (pMissilesTxt->nNextHit != 0 && pTarget != nullptr)
        {
            D2StatListStrc* pDelayStatList = STATLIST_AllocStatList(
                nullptr,
                2,
                pMissilesTxt->nNextDelay,
                pTarget->dwUnitType,
                pTarget->dwUnitId);

            if (pDelayStatList != nullptr)
            {
                STATLIST_SetState(pDelayStatList, STATE_JUSTHIT);
                STATLIST_SetStatRemoveCallback(pDelayStatList, MISSILE_ToggleStateOffCallback);
                D2COMMON_10475_PostStatToStatList(pTarget, pDelayStatList, TRUE);
                STATES_ToggleState(pTarget, STATE_JUSTHIT, TRUE);
            }
        }

        UNITEVENT_TriggerEvent(pTarget, 0, static_cast<int32_t>(reinterpret_cast<intptr_t>(pMissile)), 0);

        const int32_t nCltHitFunc = static_cast<int16_t>(pMissilesTxt->wCltHitFunc);
        if (nCltHitFunc > 0 && nCltHitFunc < gnCltMissileHitFuncs && gCltMissileHitFuncTable[nCltHitFunc] != nullptr)
        {
            if (gCltMissileHitFuncTable[nCltHitFunc](pMissile, pTarget) == 0)
            {
                return 0;
            }
        }

        const int16_t wHitSound = static_cast<int16_t>(pMissilesTxt->wHitSound);
        if (wHitSound >= 0)
        {
            D2Client_PlaySound(wHitSound, pMissile, 0, FALSE, 0);
        }

        if (static_cast<int16_t>(pMissilesTxt->wExplosionMissile) >= 0 && nExplosionMissileId != -1)
        {
            pExplosionMissile = D2Client_SpawnSubMissileFromMissile(
                pMissile,
                nExplosionMissileId,
                0,
                0,
                MISSILE_GetSkill(pMissile),
                static_cast<int32_t>(MISSILE_GetLevel(pMissile)));

            if (pExplosionMissile != nullptr)
            {
                const int32_t nPosZ = PARTICLE_GetIntPosZ(pMissile);
                const int32_t nPosY = PARTICLE_GetIntPosY(pMissile);
                const int32_t nPosX = PARTICLE_GetIntPosX(pMissile);

                PARTICLE_SetPosition(pExplosionMissile, nPosX, nPosY, nPosZ);
                PARTICLE_SetFinished(pExplosionMissile);
                PARTICLE_CancelFinished(pExplosionMissile);

                uint8_t nExplosionDir = 0;
                if (pExplosionMissile->dwClassId == MISSILE_SPIDERGOO)
                {
                    nExplosionDir = static_cast<uint8_t>(D2Client_RollRandom_6FAB1020(&pExplosionMissile->pSeed, 64));
                }
                else
                {
                    nExplosionDir = PATH_GetDirection(pMissile->pDynamicPath);
                }

                D2COMMON_10160_PathUpdateDirection(pExplosionMissile->pDynamicPath, nExplosionDir);
            }
        }
    }

    if (pTarget != nullptr && pMissilesTxt->nCollideKill == 0)
    {
        return 0;
    }

    const int16_t wTravelSound = static_cast<int16_t>(pMissilesTxt->wTravelSound);
    if (wTravelSound >= 0)
    {
        const uint32_t dwSoundHandle = D2Client_FindUnitActiveSoundHandle_6FAE6C10(pMissile, wTravelSound);
        if (dwSoundHandle != 0)
        {
            D2Client_DetachUnitFromSoundHandle_6FAE2150(dwSoundHandle, pMissile, FALSE);
        }
    }

    D2GfxLightStrc* pLight = UNITS_ResetLightMap(pMissile);
    if (pLight != nullptr)
    {
        DLIGHTMAP_StartFadeOutLight_6FB0E4C0(pLight);
    }

    if ((nHitOutcome & 0x01) != 0)
    {
        if ((pMissile->dwFlags & UNITFLAG_ISASYNC) != 0)
        {
            D2Client_RemoveClientSideUnit(pMissile->dwUnitType, pMissile->dwUnitId);
        }
        else
        {
            D2Client_RemoveServerSideUnit(pMissile->dwUnitType, pMissile->dwUnitId);
        }
    }

    return static_cast<int32_t>(reinterpret_cast<intptr_t>(pExplosionMissile));
}

// 1.12a: 0x6FAD3150
// Raw ABI: esi = pMissile
void __fastcall D2Client_FreeMissileUnitAndLight(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt != nullptr)
    {
        const int16_t wTravelSound = static_cast<int16_t>(pMissilesTxt->wTravelSound);
        if (wTravelSound >= 0)
        {
            const uint32_t dwSoundHandle = D2Client_FindUnitActiveSoundHandle_6FAE6C10(pMissile, wTravelSound);
            if (dwSoundHandle != 0)
            {
                D2Client_DetachUnitFromSoundHandle_6FAE2150(dwSoundHandle, pMissile, FALSE);
            }
        }
    }

    D2GfxLightStrc* pLight = UNITS_ResetLightMap(pMissile);
    if (pLight != nullptr)
    {
        DLIGHTMAP_StartFadeOutLight_6FB0E4C0(pLight);
    }

    D2UnitStrc** ppHashTable = ((pMissile->dwFlags & UNITFLAG_ISASYNC) != 0)
        ? gapClientSideUnitHashTables[pMissile->dwUnitType]
        : gapServerSideUnitHashTables[pMissile->dwUnitType];

    D2Client_RemoveUnitFromHashTable(ppHashTable, pMissile->dwUnitId & 0x7Fu, pMissile->dwUnitId, pMissile->dwUnitType);
}

// 1.12a: 0x6FAD31F0
// Referenced by gCltMissileDoFuncTable[1] (0x6FBA67CC)
void __fastcall D2Client_StepMissileMovementAndCollision(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        return;
    }

    BOOL bAllowExtraFirstStep = FALSE;
    switch (gnGameType)
    {
    case 0:
    case 1:
    case 6:
    case 8:
        if (D2Client_GetUnitOwner(pMissile) == gpPlayerUnit)
        {
            bAllowExtraFirstStep = TRUE;
        }
        break;
    default:
        break;
    }

    for (;;)
    {
        const int32_t nActivateFrame = MISSILE_GetActivateFrame(pMissile);
        const BOOL bIsActivated      = (MISSILE_GetCurrentFrame(pMissile) <= nActivateFrame) ? TRUE : FALSE;

        if (bIsActivated)
        {
            if (MISSILE_GetRemainingFrames(pMissile) > static_cast<int32_t>(pMissilesTxt->nInitSteps))
            {
                pMissile->dwFlagEx &= ~static_cast<uint32_t>(UNITFLAGEX_NODRAW);
            }
        }

        D2ParticleStrc* pParticle = MISSILE_GetParticleInline(pMissile);
        if (pParticle != nullptr && (pParticle->dwFlags & 0x01u) != 0 && (pParticle->dwFlags & 0x08u) == 0)
        {
            D2Client_DestroyClientMissile(pMissile, nullptr, 1);
            return;
        }

        if (bIsActivated)
        {
            if (pMissilesTxt->nLoopAnim == 0)
            {
                if (!UNITS_IsAtEndOfFrameCycle(pMissile))
                {
                    UNITS_UpdateFrame(pMissile);
                }
            }
            else if (pMissilesTxt->nSubLoop != 0
                && MISSILE_GetCurrentFrame(pMissile) > (static_cast<int32_t>(pMissilesTxt->nAnimLen) - static_cast<int32_t>(pMissilesTxt->nSubStop) + 1))
            {
                D2COMMON_10375_UNITS_SetFrameNonRate(pMissile, pMissilesTxt->nSubStart, pMissilesTxt->nSubStop);
            }
            else
            {
                UNITS_UpdateFrame(pMissile);
            }
        }

        D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
        if (PATH_GetVelocity(pPath) != 0)
        {
            D2Common_10226(pMissile, gnFrameDeltaTicks);
        }

        MISSILE_SetCurrentFrame(pMissile, MISSILE_GetCurrentFrame(pMissile) - 1);
        if (MISSILE_GetCurrentFrame(pMissile) <= 0)
        {
            D2Client_DestroyClientMissile(pMissile, nullptr, 0);
            return;
        }

        if (!bIsActivated)
        {
            return;
        }

        if (pMissilesTxt->nCltSrcTown > 0)
        {
            BOOL bOwnerInTown = FALSE;
            D2UnitStrc* pOwner = D2Client_GetUnitOwner(pMissile);
            if (pOwner == nullptr)
            {
                bOwnerInTown = TRUE;
            }
            else
            {
                D2ActiveRoomStrc* pOwnerRoom = UNITS_GetRoom(pOwner);
                if (pOwnerRoom != nullptr && DUNGEON_IsRoomInTown(pOwnerRoom))
                {
                    bOwnerInTown = TRUE;
                }
            }

            if (bOwnerInTown)
            {
                const int32_t nTownFrames = pMissilesTxt->nCltSrcTown;
                if (MISSILE_GetCurrentFrame(pMissile) >= nTownFrames)
                {
                    MISSILE_SetCurrentFrame(pMissile, nTownFrames);
                    if (pMissilesTxt->nLoopAnim == 0)
                    {
                        const int32_t nSkipFrames = static_cast<int32_t>(pMissilesTxt->nAnimLen) - static_cast<int32_t>(pMissilesTxt->nCltSrcTown);
                        if (nSkipFrames > 0)
                        {
                            pMissile->nSeqCurrentFramePrecise = nSkipFrames << 8;
                        }
                    }
                }
            }
        }

        const uint32_t dwCollideMode = pMissile->dwMissileMode;
        if (dwCollideMode == MISSILESCOLLIDE_NONE)
        {
            return;
        }

        const uint16_t wCollidedMask = D2Common_10201(pPath);
        if ((wCollidedMask & (COLLIDE_WALL | COLLIDE_MISSILE_BARRIER)) != 0)
        {
            D2Client_DestroyClientMissile(pMissile, nullptr, 1);
            return;
        }

        if (dwCollideMode == MISSILESCOLLIDE_MISSILE_BARRIER)
        {
            return;
        }

        D2ClientMissileFindArgStrc findArg;
        findArg.pMissile     = pMissile;
        findArg.pOwner       = D2Client_GetUnitOwner(pMissile);
        findArg.pMissilesTxt = pMissilesTxt;

        if (pMissilesTxt->nClientCol != 0 && wCollidedMask != 0)
        {
            D2PathPointStrc* pSaveSteps = nullptr;
            const int32_t nSaveSteps    = D2COMMON_10198_PathGetSaveStep(pPath, &pSaveSteps);
            D2ActiveRoomStrc* pRoom     = UNITS_GetRoom(pMissile);

            for (int32_t i = 0; i < nSaveSteps; ++i)
            {
                D2UnitStrc* pHitUnit = D2Common_10407(
                    pRoom,
                    pSaveSteps[i].X,
                    pSaveSteps[i].Y,
                    gClientMissileCollideTable[dwCollideMode].pfUnitFindCallback,
                    &findArg,
                    UNITS_GetUnitSizeX(pMissile));

                if (pHitUnit != nullptr && !D2Client_IsUnitDeadOrDying(pHitUnit))
                {
                    D2Client_DestroyClientMissile(pMissile, pHitUnit, 0);
                    return;
                }
            }
        }

        if (bAllowExtraFirstStep && MISSILE_GetRemainingFrames(pMissile) == 1 && PATH_GetVelocity(pPath) != 0)
        {
            bAllowExtraFirstStep = FALSE;
            continue;
        }
        break;
    }
}

// 1.12a: 0x6FAD3520
// Raw ABI: eax = pMissile
void __fastcall D2Client_DispatchMissileCltDoFunc(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    if (pMissilesTxt->nFlicker != 0)
    {
        MISSILE_UpdateFlickerLight(pMissile);
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
    if (pRoom == nullptr)
    {
        const int32_t nY = MISSILE_GetUnitSubtileYInline(pMissile);
        const int32_t nX = MISSILE_GetUnitSubtileXInline(pMissile);
        pRoom = DUNGEON_FindRoomBySubtile(nullptr, nX, nY);
    }

    if ((pMissilesTxt->dwMissileFlags & MISSILESFLAG_TOWN) == 0 && DUNGEON_IsRoomInTown(pRoom))
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    const int32_t nCltDoFunc = static_cast<int16_t>(pMissilesTxt->wCltDoFunc);
    if (nCltDoFunc > 0 && nCltDoFunc < gnCltMissileDoFuncs && gCltMissileDoFuncTable[nCltDoFunc] != nullptr)
    {
        gCltMissileDoFuncTable[nCltDoFunc](pMissile);
    }
}

// 1.12a: 0x6FAD3630
// Referenced by gCltMissileDoFuncTable[60] (0x6FBA68B8)
void __fastcall MISSILE_CltDo60_RotateEveryOtherFrame(D2UnitStrc* pMissile)
{
    if ((MISSILE_GetRemainingFrames(pMissile) % 2) == 0)
    {
        D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
        const int32_t nX     = MISSILE_GetUnitSubtileXInline(pMissile);
        const int32_t nY     = MISSILE_GetUnitSubtileYInline(pMissile);
        const int32_t nDiffX = nX - MISSILE_GetTargetX(pMissile);
        const int32_t nDiffY = nY - MISSILE_GetTargetY(pMissile);

        D2COMMON_10170_PathSetTargetPos(pPath, nX + nDiffY, nY - nDiffX);
        D2Common_10142(pPath, pMissile, 0);
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD36F0
// Referenced by gCltMissileDoFuncTable[59] (0x6FBA68B4)
void __fastcall MISSILE_CltDo59_CullOutsideVerticalBounds(D2UnitStrc* pMissile)
{
    const int32_t nMinZ = -MISSILE_GetTargetX(pMissile);
    const int32_t nMaxZ = MISSILE_GetTargetY(pMissile);

    D2ParticleStrc* pParticle = MISSILE_GetParticleInline(pMissile);
    const int32_t nCurZ = (pParticle != nullptr) ? (pParticle->nPosZ >> 11) : 0;

    if (nCurZ < nMinZ || nCurZ > nMaxZ)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD3740
// Referenced by gCltMissileDoFuncTable[65] (0x6FBA68CC)
void __fastcall MISSILE_CltDo65_IzualControlOrbitSpirit(D2UnitStrc* pMissile)
{
    int32_t nLocalState      = MISSILE_GetTargetY(pMissile);
    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    if (pPath == nullptr)
    {
        D2Client_StepMissileMovementAndCollision(pMissile);
        return;
    }

    const uint32_t dwControllerGUID = static_cast<uint32_t>(MISSILE_GetTargetX(pMissile));
    D2UnitStrc* pController         = MISSILE_FindClientMissileByGUIDInline(dwControllerGUID);
    if (pController == nullptr)
    {
        D2Client_StepMissileMovementAndCollision(pMissile);
        return;
    }

    if (pController->dwUnitType != UNIT_MISSILE)
    {
        FOG_DisplayHalt(FOG_GetErrorString(90), "", 0);
        exit(-1);
    }

    const int32_t nControllerState = MISSILE_GetTargetX(pController);
    if (nLocalState != nControllerState && nControllerState >= 2)
    {
        if (D2Client_RollRandom_6FAB1020(&pMissile->pSeed, 25) == 0)
        {
            MISSILE_SetTargetY(pMissile, nControllerState);
            if (nControllerState == 2)
            {
                PATH_SetVelocity(pPath, 0xF00, s_szMissileFile, 6619);
                D2COMMON_10170_PathSetTargetPos(pPath, 0x3B1F, 0x170C);
                D2Common_10142(pPath, pMissile, 0);
            }
            D2Client_StepMissileMovementAndCollision(pMissile);
            return;
        }
    }

    if (nLocalState == 0)
    {
        if ((static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 10u) == 0)
        {
            MISSILE_SetTargetY(pMissile, 1);
            PARTICLE_CancelFinished(pMissile);
            PATH_SetVelocity(pPath, 0xF00, s_szMissileFile, 6630);

            if (D2Client_RollRandom_6FAB1020(&pMissile->pSeed, 2) != 0)
            {
                D2ParticleStrc* pParticle = MISSILE_GetParticleInline(pMissile);
                if (pParticle != nullptr)
                {
                    pParticle->nVelX = 0;
                    pParticle->nVelY = 0;
                    pParticle->nVelZ = 0x800;
                }
            }
        }
    }
    else if (nLocalState == 1)
    {
        int32_t nDiffX = D2Client_GetUnitX_6FAB1590(pMissile) - D2Client_GetUnitX_6FAB1590(pController);
        int32_t nDiffY = D2Client_GetUnitY_6FAB15C0(pMissile) - D2Client_GetUnitY_6FAB15C0(pController);

        if ((pMissile->dwUnitId & 2u) != 0)
        {
            nDiffX = -nDiffX;
            nDiffY = -nDiffY;
        }

        if (nDiffX != 0 || nDiffY != 0)
        {
            const int32_t nTargetY = D2Client_GetUnitY_6FAB15C0(pMissile) + nDiffX;
            const int32_t nTargetX = D2Client_GetUnitX_6FAB1590(pMissile) - nDiffY;
            D2COMMON_10170_PathSetTargetPos(pPath, nTargetX, nTargetY);
            D2Common_10142(pPath, pMissile, 0);
        }
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD3950
// Referenced by gCltMissileDoFuncTable[63] (0x6FBA68C4)
void __fastcall MISSILE_CltDo63_RotateEveryFrame(D2UnitStrc* pMissile)
{
    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    const int32_t nX     = MISSILE_GetUnitSubtileXInline(pMissile);
    const int32_t nY     = MISSILE_GetUnitSubtileYInline(pMissile);
    const int32_t nDiffX = nX - MISSILE_GetTargetX(pMissile);
    const int32_t nDiffY = nY - MISSILE_GetTargetY(pMissile);

    D2COMMON_10170_PathSetTargetPos(pPath, nX + nDiffY, nY - nDiffX);
    D2Common_10142(pPath, pMissile, 0);
    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD39F0
// Referenced by gCltMissileDoFuncTable[34] (0x6FBA6850)
void __fastcall MISSILE_CltDo34_BrainDeathSpirit(D2UnitStrc* pMissile)
{
    const int32_t nLocalState = MISSILE_GetTargetY(pMissile);
    D2DynamicPathStrc* pPath  = pMissile->pDynamicPath;
    if (pPath == nullptr)
    {
        D2Client_StepMissileMovementAndCollision(pMissile);
        return;
    }

    const uint32_t dwControllerGUID = static_cast<uint32_t>(MISSILE_GetTargetX(pMissile));
    D2UnitStrc* pController         = MISSILE_FindClientMissileByGUIDInline(dwControllerGUID);
    if (pController == nullptr)
    {
        D2Client_StepMissileMovementAndCollision(pMissile);
        return;
    }

    if (pController->dwUnitType != UNIT_MISSILE)
    {
        FOG_DisplayHalt(FOG_GetErrorString(90), "", 0);
        exit(-1);
    }

    const int32_t nControllerState = MISSILE_GetTargetX(pController);
    const int32_t nDist            = UNITS_GetDistanceToOtherUnit(pMissile, pController);
    int32_t nCloseDistThreshold    = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 5u);
    if (nCloseDistThreshold <= 1)
    {
        nCloseDistThreshold = 1;
    }

    if (nLocalState != nControllerState && nControllerState > 1)
    {
        MISSILE_SetTargetY(pMissile, nControllerState);
        switch (nControllerState)
        {
        case 2:
        {
            PATH_SetVelocity(pPath, 0, s_szMissileFile, 6004);
            PARTICLE_CancelFinished(pMissile);
            D2ParticleStrc* pParticle = MISSILE_GetParticleInline(pMissile);
            if (pParticle != nullptr)
            {
                pParticle->nVelX = 0;
                pParticle->nVelY = 0;
                pParticle->nVelZ = 0x3000;
            }

            const int32_t nTargetZ = static_cast<int32_t>(D2Client_RollRandom_6FAB1020(&pMissile->pSeed, 40)) + 170;
            pParticle = MISSILE_GetParticleInline(pMissile);
            if (pParticle != nullptr)
            {
                pParticle->nTargetX = 0;
                pParticle->nTargetY = 0;
                pParticle->nTargetZ = nTargetZ;
                pParticle->dwFlags |= 0x20u;
            }

            D2COMMON_10170_PathSetTargetPos(pPath, D2Client_GetUnitX_6FAB1590(pController), D2Client_GetUnitY_6FAB15C0(pController));
            PATH_SetVelocity(pPath, 0x300, s_szMissileFile, 6012);
            D2Common_10142(pPath, pMissile, 0);
            break;
        }
        case 3:
        {
            PARTICLE_CancelFinished(pMissile);
            D2ParticleStrc* pParticle = MISSILE_GetParticleInline(pMissile);
            if (pParticle != nullptr)
            {
                pParticle->nVelX = 0;
                pParticle->nVelY = 0;
                pParticle->nVelZ = -0x3000;
            }

            const int32_t nTargetZ = static_cast<int32_t>(D2Client_RollRandom_6FAB1020(&pMissile->pSeed, 40));
            pParticle = MISSILE_GetParticleInline(pMissile);
            if (pParticle != nullptr)
            {
                pParticle->nTargetX = 0;
                pParticle->nTargetY = 0;
                pParticle->nTargetZ = nTargetZ;
                pParticle->dwFlags &= ~0x20u;
            }

            D2Common_10142(pPath, pMissile, 0);
            break;
        }
        case 4:
        {
            PARTICLE_CancelFinished(pMissile);
            D2ParticleStrc* pParticle = MISSILE_GetParticleInline(pMissile);
            if (pParticle != nullptr)
            {
                pParticle->nVelX    = 0;
                pParticle->nVelY    = 0;
                pParticle->nVelZ    = 0xA000;
                pParticle->nTargetX = 0;
                pParticle->nTargetY = 0;
                pParticle->nTargetZ = 400;
                pParticle->dwFlags |= 0x20u;
            }

            D2Common_10142(pPath, pMissile, 0);
            break;
        }
        default:
            break;
        }

        D2Client_StepMissileMovementAndCollision(pMissile);
        return;
    }

    if (nDist < nCloseDistThreshold && nLocalState == 0)
    {
        MISSILE_SetTargetY(pMissile, 1);
        PARTICLE_CancelFinished(pMissile);
        D2ParticleStrc* pParticle = MISSILE_GetParticleInline(pMissile);
        if (pParticle != nullptr)
        {
            pParticle->nVelX = 0;
            pParticle->nVelY = 0;
            pParticle->nVelZ = 0x800;
        }
    }
    else if (nLocalState == 1)
    {
        const int32_t nDiffX = D2Client_GetUnitX_6FAB1590(pMissile) - D2Client_GetUnitX_6FAB1590(pController);
        const int32_t nDiffY = D2Client_GetUnitY_6FAB15C0(pMissile) - D2Client_GetUnitY_6FAB15C0(pController);
        if (nDiffX != 0 || nDiffY != 0)
        {
            const int32_t nTargetY = D2Client_GetUnitY_6FAB15C0(pMissile) + nDiffX;
            const int32_t nTargetX = D2Client_GetUnitX_6FAB1590(pMissile) - nDiffY;
            D2COMMON_10170_PathSetTargetPos(pPath, nTargetX, nTargetY);
            PATH_SetVelocity(pPath, 0xA00, s_szMissileFile, 6046);
            D2Common_10142(pPath, pMissile, 0);
        }
    }
    else if (nLocalState == 3)
    {
        D2COMMON_10170_PathSetTargetPos(pPath, D2Client_GetUnitX_6FAB1590(pController), D2Client_GetUnitY_6FAB15C0(pController));
        PATH_SetVelocity(pPath, 0x200, s_szMissileFile, 6055);
        D2Common_10142(pPath, pMissile, 0);
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD3D70
// Referenced by gCltMissileDoFuncTable[32] (0x6FBA6848)
void __fastcall MISSILE_CltDo32_RadamentDeath(D2UnitStrc* pMissile)
{
    const int32_t nCurFrame = MISSILE_GetCurrentFrame(pMissile);
    D2UnitStrc* pOwner      = D2Client_GetUnitOwner(pMissile);

    int32_t nStageIdx = 0;
    while (nStageIdx < 5 && nCurFrame <= s_aRadamentStageTable[nStageIdx].nFrameThreshold)
    {
        ++nStageIdx;
    }

    if ((nCurFrame % s_aRadamentStageTable[nStageIdx].nSpawnPeriod) == 0)
    {
        const int32_t nY     = MISSILE_GetUnitSubtileYInline(pMissile);
        const int32_t nX     = MISSILE_GetUnitSubtileXInline(pMissile);
        const int32_t nLevel = static_cast<int32_t>(MISSILE_GetLevel(pMissile));
        const int32_t nSkill = MISSILE_GetSkill(pMissile);

        if (pOwner != nullptr)
        {
            D2MissileStrc missileInit = {};
            missileInit.dwFlags     = CLTMISSILEINIT_USE_COORDS;
            missileInit.pOwner      = pOwner;
            missileInit.nMissile    = MISSILE_RADAMENTHANDOFGOD;
            missileInit.nX          = nX;
            missileInit.nY          = nY;
            missileInit.nSkill      = nSkill;
            missileInit.nSkillLevel = nLevel;

            D2UnitStrc* pBeam = D2Client_CreateClientMissile(&missileInit);
            if (pBeam != nullptr)
            {
                MISSILE_SetTargetX(pBeam, s_aRadamentStageTable[nStageIdx].nTargetXParam);
                MISSILE_SetTargetY(pBeam, nCurFrame);
            }
        }
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD3EB0
// Referenced by gCltMissileDoFuncTable[39] (0x6FBA6864)
void __fastcall MISSILE_CltDo39_FreezeAnimFrameWhileOwnerExists(D2UnitStrc* pMissile)
{
    if (D2Client_GetUnitOwner(pMissile) != nullptr)
    {
        pMissile->nSeqCurrentFramePrecise = 0;
    }
    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD3ED0
// Referenced by gCltMissileDoFuncTable[41] (0x6FBA686C)
void __fastcall MISSILE_CltDo41_TowerMistTrail(D2UnitStrc* pMissile)
{
    const int32_t nElapsed = MISSILE_GetRemainingFrames(pMissile);
    MISSILE_UpdateTowerMistAnimSpeedAndAscent(pMissile, nElapsed);
    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD3F30
// Referenced by gCltMissileDoFuncTable[37] (0x6FBA685C)
void __fastcall MISSILE_CltDo37_DiabloAppearsShakeAndRoar(D2UnitStrc* pMissile)
{
    const int32_t nCurFrame = MISSILE_GetCurrentFrame(pMissile);
    if (nCurFrame == 150)
    {
        g_dwShakeStartTick   = GetTickCount();
        g_nShakeRampUpMs     = 0;
        g_nShakeHoldMs       = 4000;
        g_nShakeRampDownMs   = 0;
        g_nShakeMaxAmplitude = 25;
    }
    else if (nCurFrame == 50)
    {
        D2Client_PlaySound(0x121E, nullptr, 0, FALSE, 0);
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD3FA0
// Referenced by gCltMissileHitFuncTable[10] (0x6FBA6960)
int32_t __fastcall MISSILE_CltHit10_AttachProgOverlayToTarget(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (pTarget != nullptr && pMissile != nullptr)
    {
        D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
        if (pMissilesTxt != nullptr && static_cast<int16_t>(pMissilesTxt->wProgOverlay) > 0)
        {
            D2Client_SetUnitOverlay_6FB430E0(static_cast<int16_t>(pMissilesTxt->wProgOverlay), pTarget, 2, 0, 0, 0, 0, 0);
        }
    }
    return 1;
}

// 1.12a: 0x6FAD4000
// Referenced by gCltMissileDoFuncTable[58] (0x6FBA68B0)
void __fastcall MISSILE_CltDo58_SpiralTwisterPath(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    const int32_t nPeriod = (pMissilesTxt->dwParam[0] >= 1) ? pMissilesTxt->dwParam[0] : 1;
    if ((MISSILE_GetRemainingFrames(pMissile) % nPeriod) != 0)
    {
        D2Client_StepMissileMovementAndCollision(pMissile);
        return;
    }

    D2SeedStrc* pSeed = &pMissile->pSeed;
    SEED_InitLowSeed(pSeed, MISSILE_GetTargetX(pMissile));

    const uint32_t dwPackedVec = static_cast<uint32_t>(MISSILE_GetTargetY(pMissile));
    int32_t nVecX = static_cast<int16_t>(dwPackedVec & 0xFFFFu);
    int32_t nVecY = static_cast<int16_t>(dwPackedVec >> 16);

    const uint32_t dwSide = static_cast<uint32_t>(SEED_RollRandomNumber(pSeed)) & 1u;
    int32_t nPerpX = nVecY;
    int32_t nPerpY = nVecX;
    if (dwSide != 0)
    {
        nPerpX = -nPerpX;
    }
    else
    {
        nPerpY = -nPerpY;
    }

    nVecX = (nVecX * 4 + nPerpX) / 4;
    nVecY = (nVecY * 4 + nPerpY) / 4;
    if (nVecX == 0)
    {
        nVecX = 1;
    }
    if (nVecY == 0)
    {
        nVecY = 1;
    }

    D2COMMON_10170_PathSetTargetPos(
        pMissile->pDynamicPath,
        D2Client_GetUnitX_6FAB1590(pMissile) + nVecX,
        D2Client_GetUnitY_6FAB15C0(pMissile) + nVecY);
    D2Common_10142(pMissile->pDynamicPath, pMissile, 0);

    MISSILE_SetTargetX(pMissile, static_cast<int32_t>(SEED_GetLowSeed(pSeed)));
    MISSILE_SetTargetY(pMissile, static_cast<int32_t>((static_cast<uint32_t>(static_cast<uint16_t>(nVecY)) << 16) + static_cast<uint16_t>(nVecX)));
    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD4150
// Referenced by gCltMissileDoFuncTable[49] (0x6FBA688C)
void __fastcall MISSILE_CltDo49_PeriodicTrailingSubMissile(D2UnitStrc* pMissile)
{
    if (pMissile != nullptr)
    {
        D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
        if (pMissilesTxt != nullptr && static_cast<int16_t>(pMissilesTxt->wCltSubMissile[0]) >= 0)
        {
            const int32_t nPeriod = (static_cast<int32_t>(pMissilesTxt->dwCltParam[0]) >= 1)
                ? static_cast<int32_t>(pMissilesTxt->dwCltParam[0])
                : 1;

            if ((MISSILE_GetRemainingFrames(pMissile) % nPeriod) == 0)
            {
                MISSILE_SpawnAlignedSubMissile(pMissile, static_cast<int16_t>(pMissilesTxt->wCltSubMissile[0]));
            }

            D2Client_StepMissileMovementAndCollision(pMissile);
            return;
        }
    }

    D2Client_FreeMissileUnitAndLight(pMissile);
}

// 1.12a: 0x6FAD41C0
// Referenced by gCltMissileDoFuncTable[44] (0x6FBA6878)
void __fastcall MISSILE_CltDo44_BlazeTrailFollowOwner(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        D2Client_RemoveUnitFromHashTable(gapClientSideUnitHashTables[UNIT_TYPES_COUNT], 0x7Fu, 0xFFFFFFFFu, UNIT_TYPES_COUNT);
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr || static_cast<int16_t>(pMissilesTxt->wCltSubMissile[0]) < 0)
    {
        D2Client_RemoveUnitFromHashTable(
            gapClientSideUnitHashTables[pMissile->dwUnitType],
            pMissile->dwUnitId & 0x7Fu,
            pMissile->dwUnitId,
            pMissile->dwUnitType);
        return;
    }

    D2UnitStrc* pOwner = D2Client_GetUnitOwner(pMissile);
    if (pOwner == nullptr)
    {
        D2Client_RemoveUnitFromHashTable(
            gapClientSideUnitHashTables[pMissile->dwUnitType],
            pMissile->dwUnitId & 0x7Fu,
            pMissile->dwUnitId,
            pMissile->dwUnitType);
        return;
    }

    D2SkillStrc* pUsedSkill = UNITS_GetUsedSkill(pOwner);
    if (pUsedSkill == nullptr || SKILLS_GetSkillIdFromSkill(pUsedSkill, s_szMissileFile, 2285) != MISSILE_GetSkill(pMissile))
    {
        D2Client_RemoveUnitFromHashTable(
            gapClientSideUnitHashTables[pMissile->dwUnitType],
            pMissile->dwUnitId & 0x7Fu,
            pMissile->dwUnitId,
            pMissile->dwUnitType);
        return;
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2Client_SpawnSubMissileFromMissile(
            pMissile,
            static_cast<int16_t>(pMissilesTxt->wCltSubMissile[0]),
            0,
            0,
            MISSILE_GetSkill(pMissile),
            static_cast<int32_t>(MISSILE_GetLevel(pMissile)));
    }

    D2Common_10235_PATH_UpdateRiderPath(pMissile, pOwner);
    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD42C0
// Referenced by gCltMissileDoFuncTable[43] (0x6FBA6874) and called at 0x6FAD89FC
void __fastcall MISSILE_CltDo43_FollowOwnerRiderMissile(D2UnitStrc* pMissile)
{
    D2UnitStrc* pOwner = D2Client_GetUnitOwner(pMissile);
    if (pOwner == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    if ((pOwner->dwUnitType == UNIT_PLAYER || pOwner->dwUnitType == UNIT_MONSTER)
        && D2Client_IsUnitDeadOrDying(pOwner))
    {
        D2Client_DestroyClientMissile(pMissile, nullptr, 0);
        return;
    }

    D2Common_10235_PATH_UpdateRiderPath(pMissile, pOwner);
    UNITS_UpdateFrame(pMissile);
}

// 1.12a: 0x6FAD4310
// Raw ABI: eax = pMissile (called at 0x6FADD3B9 and 0x6FADD43B)
void __fastcall MISSILE_InitFallingVerticalParticle(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    const int32_t nFallSpeed = static_cast<int32_t>(pMissilesTxt->dwCltParam[1]);
    if (nFallSpeed > 0)
    {
        const int32_t nStartHeight = static_cast<int32_t>(pMissilesTxt->dwCltParam[0]);
        D2ParticleStrc* pParticle  = MISSILE_GetParticleInline(pMissile);
        if (pParticle != nullptr)
        {
            pParticle->nPosX = 0;
            pParticle->nPosY = 0;
            pParticle->nPosZ = nStartHeight << 11;
            pParticle->nVelX = 0;
            pParticle->nVelY = 0;
            pParticle->nVelZ = (-nFallSpeed) << 11;
        }

        const int32_t nFrames = nStartHeight / nFallSpeed;
        MISSILE_SetTotalFrames(pMissile, nFrames);
        MISSILE_SetCurrentFrame(pMissile, nFrames);
    }
}

// 1.12a: 0x6FAD43B0
// Referenced by gCltMissileDoFuncTable[23] (0x6FBA6824)
void __fastcall MISSILE_CltDo23_LoopLifetimeFrames(D2UnitStrc* pMissile)
{
    if (MISSILE_GetCurrentFrame(pMissile) < 100)
    {
        MISSILE_SetCurrentFrame(pMissile, 500);
    }
    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD43E0
// Referenced by gCltMissileDoFuncTable[20] (0x6FBA6818)
void __fastcall MISSILE_CltDo20_SpiralOutwardCurve(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    const int32_t nPeriod  = (static_cast<int32_t>(pMissilesTxt->dwCltParam[1]) >= 1)
        ? static_cast<int32_t>(pMissilesTxt->dwCltParam[1])
        : 1;
    const int32_t nElapsed = MISSILE_GetRemainingFrames(pMissile);

    if (nElapsed < static_cast<int32_t>(pMissilesTxt->dwCltParam[0]) && (nElapsed % nPeriod) == 0)
    {
        const int32_t nOldDX = MISSILE_GetTargetX(pMissile);
        const int32_t nOldDY = MISSILE_GetTargetY(pMissile);
        const int32_t nNewDX = (nOldDX - nOldDY) / 2;
        const int32_t nNewDY = (nOldDX + nOldDY) / 2;

        D2COMMON_10170_PathSetTargetPos(
            pMissile->pDynamicPath,
            D2Client_GetUnitX_6FAB1590(pMissile) + nNewDX,
            D2Client_GetUnitY_6FAB15C0(pMissile) + nNewDY);
        D2Common_10142(pMissile->pDynamicPath, pMissile, 0);

        MISSILE_SetTargetX(pMissile, nNewDX);
        MISSILE_SetTargetY(pMissile, nNewDY);
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD44B0
// Referenced by gCltMissileDoFuncTable[14] (0x6FBA6800)
void __fastcall MISSILE_CltDo14_HomingStepTowardTarget(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr || D2Client_GetUnitOwner(pMissile) == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    UNITMODE_ClearActionFrame(pMissile);

    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    if (pPath == nullptr)
    {
        FOG_DisplayHalt(FOG_GetErrorString(2374), "", 0);
        exit(-1);
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pPath);
    if (pTarget == nullptr)
    {
        pTarget = D2Client_ClearActionFrameAndGetTarget(pMissile);
        if (pTarget == nullptr)
        {
            D2Client_StepMissileMovementAndCollision(pMissile);
            return;
        }
    }

    const int32_t nPeriod = (static_cast<int32_t>(pMissilesTxt->dwCltParam[2]) > 0)
        ? static_cast<int32_t>(pMissilesTxt->dwCltParam[2])
        : 5;

    if ((MISSILE_GetRemainingFrames(pMissile) % nPeriod) == 0
        && UNITS_GetDistanceToOtherUnit(pMissile, pTarget) <= static_cast<int32_t>(pMissilesTxt->dwCltParam[3]))
    {
        const int32_t nMissileX = D2Client_GetUnitX_6FAB1590(pMissile);
        const int32_t nMissileY = D2Client_GetUnitY_6FAB15C0(pMissile);
        const int32_t nTargetX  = D2Client_GetUnitX_6FAB1590(pTarget);
        const int32_t nTargetY  = D2Client_GetUnitY_6FAB15C0(pTarget);

        const int32_t nStep = (static_cast<int32_t>(pMissilesTxt->dwCltParam[4]) >= 1)
            ? static_cast<int32_t>(pMissilesTxt->dwCltParam[4])
            : 1;

        int32_t nDeltaX = 0;
        if (nTargetX > nMissileX)
        {
            nDeltaX = nStep;
        }
        else if (nTargetX < nMissileX)
        {
            nDeltaX = -nStep;
        }

        int32_t nDeltaY = 0;
        if (nTargetY > nMissileY)
        {
            nDeltaY = nStep;
        }
        else if (nTargetY < nMissileY)
        {
            nDeltaY = -nStep;
        }

        D2COMMON_10170_PathSetTargetPos(
            pMissile->pDynamicPath,
            D2Client_GetUnitX_6FAB1590(pMissile) + nDeltaX,
            D2Client_GetUnitY_6FAB15C0(pMissile) + nDeltaY);
        D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD4620
// Referenced by gCltMissileDoFuncTable[67] (0x6FBA68D4)
void __fastcall MISSILE_CltDo67_TimedThreeSubMissileSpawner(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    for (int32_t i = 0; i < 3; ++i)
    {
        const int16_t nSubMissileId = static_cast<int16_t>(pMissilesTxt->wCltSubMissile[i]);
        if (nSubMissileId > 0 && MISSILE_GetRemainingFrames(pMissile) == static_cast<int32_t>(pMissilesTxt->dwCltParam[i]))
        {
            D2Client_SpawnClientMissileAtCoords(
                D2Client_GetUnitOwner(pMissile),
                nSubMissileId,
                D2Client_GetUnitX_6FAB1590(pMissile),
                D2Client_GetUnitY_6FAB15C0(pMissile),
                MISSILE_GetSkill(pMissile),
                static_cast<int32_t>(MISSILE_GetLevel(pMissile)));
        }
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD4730
// Referenced by gCltMissileDoFuncTable[12] (0x6FBA67F8)
void __fastcall MISSILE_CltDo12_StartScreenShakeOnFirstFrame(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr
        || static_cast<int32_t>(pMissilesTxt->dwCltParam[0]) <= 0
        || static_cast<int32_t>(pMissilesTxt->dwCltParam[1]) <= 0)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    if (MISSILE_GetRemainingFrames(pMissile) == 0)
    {
        // dwCltParam[1] duration split into 1/3 attack, remainder sustain, 1/3 release
        // (constant-folded ((sec * 25) * 1000) / 25 in original binary: imul 0x61A8, / 25)
        const int32_t nTotalSec   = static_cast<int32_t>(pMissilesTxt->dwCltParam[1]);
        const int32_t nThirdSec   = nTotalSec / 3;
        const uint32_t dwRampMs   = static_cast<uint32_t>((nThirdSec * 25000) / 25);
        const uint32_t dwSustainMs = static_cast<uint32_t>(((nTotalSec - 2 * nThirdSec) * 25000) / 25);

        VIEW_StartScreenShake_Stdcall(dwSustainMs, static_cast<int32_t>(pMissilesTxt->dwCltParam[0]), dwRampMs, dwRampMs);
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD47F0
// Referenced by gCltMissileDoFuncTable[11] (0x6FBA67F4)
void __fastcall MISSILE_CltDo11_StopAndMarkDeadAtAnimEnd(D2UnitStrc* pMissile)
{
    if (UNITS_IsAtEndOfFrameCycle(pMissile))
    {
        if (pMissile != nullptr)
        {
            pMissile->dwFlags |= UNITFLAG_ISDEAD;
        }
        if (pMissile->pLight != nullptr)
        {
            DLIGHTMAP_RemoveUnitLight(pMissile->pLight);
            pMissile->pLight = nullptr;
        }
        return;
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD4830
// Raw ABI: eax = pMissile (called at 0x6FADD744 and 0x6FADD814)
void __fastcall MISSILE_InitClampedFallingVerticalParticle(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    const int32_t nFallSpeed = (static_cast<int32_t>(pMissilesTxt->dwCltParam[1]) >= 1)
        ? static_cast<int32_t>(pMissilesTxt->dwCltParam[1])
        : 1;
    int32_t nStartHeight = static_cast<int32_t>(pMissilesTxt->dwCltParam[0]);
    if (nStartHeight < nFallSpeed)
    {
        nStartHeight = nFallSpeed;
    }

    D2ParticleStrc* pParticle = MISSILE_GetParticleInline(pMissile);
    if (pParticle != nullptr)
    {
        pParticle->nPosX = 0;
        pParticle->nPosY = 0;
        pParticle->nPosZ = nStartHeight << 11;
        pParticle->nVelX = 0;
        pParticle->nVelY = 0;
        pParticle->nVelZ = (-nFallSpeed) << 11;
    }

    const int32_t nFrames = nStartHeight / nFallSpeed;
    MISSILE_SetTotalFrames(pMissile, nFrames);
    MISSILE_SetCurrentFrame(pMissile, nFrames);
}

// 1.12a: 0x6FAD48E0
// Referenced by gCltMissileDoFuncTable[7] (0x6FBA67E4) and called at 0x6FAD8CE5
void __fastcall MISSILE_CltDo07_GuidedArrow_BoneSpirit(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2UnitStrc* pOwner = D2Client_GetUnitOwner(pMissile);
    if (DUNGEON_IsRoomInTown(UNITS_GetRoom(pMissile)))
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    if ((MISSILE_GetTargetX(pMissile) & 0x01) != 0)
    {
        UNITMODE_ClearActionFrame(pMissile);
        D2UnitStrc* pTarget = PATH_GetTargetUnit(pMissile->pDynamicPath);
        if (pTarget != nullptr)
        {
            if (D2Client_IsUnitDeadOrDying(pTarget)
                || (pOwner != nullptr && !D2Client_AreUnitsHostileOrAttackable(pTarget, pOwner)))
            {
                pTarget = nullptr;
            }
        }

        const int32_t nRecalcPeriod = (static_cast<int32_t>(pMissilesTxt->dwCltParam[0]) > 0)
            ? static_cast<int32_t>(pMissilesTxt->dwCltParam[0])
            : 5;

        if (pTarget != nullptr && (MISSILE_GetRemainingFrames(pMissile) % nRecalcPeriod) == 0)
        {
            const int32_t nDist = UNITS_GetDistanceToOtherUnit(pMissile, pTarget);
            if (nDist > 3 && nDist < 25)
            {
                D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
            }
        }
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD49C0
// Referenced by gCltMissileDoFuncTable[5] (0x6FBA67DC)
void __fastcall MISSILE_CltDo05_FireWall_SubLoopAndFadeDown(D2UnitStrc* pMissile)
{
    if (pMissile == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissile->dwClassId);
    if (pMissilesTxt == nullptr)
    {
        D2Client_FreeMissileUnitAndLight(pMissile);
        return;
    }

    const int32_t nSubStart     = pMissilesTxt->nSubStart;
    const int32_t nCurAnimFrame = pMissile->nSeqCurrentFramePrecise >> 8;

    if (nCurAnimFrame == nSubStart - 1)
    {
        const int32_t nRandAdd = static_cast<int32_t>(
            D2Client_RollRandom_6FAB1070(&pMissile->pSeed, static_cast<int32_t>(pMissilesTxt->nSubStop) - nSubStart));
        pMissile->nSeqCurrentFramePrecise = (nCurAnimFrame + nRandAdd) << 8;
    }
    else if (MISSILE_GetCurrentFrame(pMissile) == nSubStart)
    {
        int32_t nRewindFrame = nSubStart - 3;
        if (nRewindFrame < 0)
        {
            nRewindFrame = 0;
        }
        pMissile->nSeqCurrentFramePrecise = nRewindFrame << 8;
    }
    else if (MISSILE_GetCurrentFrame(pMissile) < nSubStart)
    {
        int32_t nFadeFrame = nCurAnimFrame - 2;
        if (nFadeFrame < 0)
        {
            nFadeFrame = 0;
        }
        pMissile->nSeqCurrentFramePrecise = nFadeFrame << 8;
    }

    D2Client_StepMissileMovementAndCollision(pMissile);
}

// 1.12a: 0x6FAD4A90
// Referenced by gCltMissileDoFuncTable[2] (0x6FBA67D0)
void __fastcall MISSILE_CltDo02_StopOnAnimEndAndKeepInViewport(D2UnitStrc* pMissile)
{
    if (UNITS_IsAtEndOfFrameCycle(pMissile))
    {
        if (pMissile != nullptr)
        {
            pMissile->dwFlags |= UNITFLAG_ISDEAD;
        }
        PATH_SetVelocity(pMissile->pDynamicPath, 0, s_szMissileFile, 127);
    }

    const int32_t nViewMinX = gnViewportOffsetX_6FBC21D0 - gnViewportShiftOffsetX;
    const int32_t nRelX     = UNITS_GetClientCoordX(pMissile) - nViewMinX;
    if (nRelX >= 0 && nRelX <= gnViewportWidth)
    {
        const int32_t nViewMinY = gnViewportOffsetY_6FBC21CC - 8;
        const int32_t nRelY     = UNITS_GetClientCoordY(pMissile) - nViewMinY;
        if (nRelY >= 0 && nRelY <= (gnViewportHeight + 64))
        {
            MISSILE_SetCurrentFrame(pMissile, 128);
            D2Client_StepMissileMovementAndCollision(pMissile);
            return;
        }
    }

    D2Client_DestroyClientMissile(pMissile, nullptr, 0);
}

// 1.12a: 0x6FAD4B20
// Master Client Missile Creation Function (ecx = pMissileInit)
D2UnitStrc* __fastcall D2Client_CreateClientMissile(D2MissileStrc* pMissileInit)
{
    D2MissilesTxt* pMissilesTxt = MISSILE_GetMissilesTxtRecordInline(pMissileInit->nMissile);
    if (pMissilesTxt == nullptr)
    {
        return nullptr;
    }

    int32_t nX = 0;
    int32_t nY = 0;
    if ((pMissileInit->dwFlags & CLTMISSILEINIT_USE_COORDS) != 0)
    {
        nX = pMissileInit->nX;
        nY = pMissileInit->nY;
    }
    else
    {
        if (pMissileInit->pOrigin == nullptr)
        {
            return nullptr;
        }
        nX = D2Client_GetUnitX_6FAB1590(pMissileInit->pOrigin);
        nY = D2Client_GetUnitY_6FAB15C0(pMissileInit->pOrigin);
    }

    if (pMissileInit->pOwner != nullptr)
    {
        D2ActiveRoomStrc* pRoom = DUNGEON_FindRoomBySubtile(UNITS_GetRoom(pMissileInit->pOwner), nX, nY);
        if (pRoom == nullptr)
        {
            pRoom = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, nX, nY);
            if (pRoom == nullptr)
            {
                return nullptr;
            }
        }
    }
    else if (gpPlayerUnit != nullptr)
    {
        D2ActiveRoomStrc* pRoom = DUNGEON_FindRoomBySubtile(UNITS_GetRoom(gpPlayerUnit), nX, nY);
        if (pRoom == nullptr)
        {
            pRoom = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, nX, nY);
            if (pRoom == nullptr)
            {
                return nullptr;
            }
        }
    }

    int32_t nTargetX = nX;
    int32_t nTargetY = nY;
    if ((pMissileInit->dwFlags & CLTMISSILEINIT_REL_TARGET_COORDS) != 0)
    {
        nTargetX = nX + pMissileInit->nTargetX;
        nTargetY = nY + pMissileInit->nTargetY;
    }
    else if ((pMissileInit->dwFlags & CLTMISSILEINIT_ABS_TARGET_COORDS) != 0)
    {
        nTargetX = pMissileInit->nTargetX;
        nTargetY = pMissileInit->nTargetY;
    }

    int32_t nVelocity = 0;
    if ((pMissileInit->dwFlags & CLTMISSILEINIT_CUSTOM_VELOCITY) != 0)
    {
        nVelocity = pMissileInit->nVelocity;
        if ((pMissileInit->dwFlags & CLTMISSILEINIT_RAW_VELOCITY_FP8) == 0)
        {
            nVelocity <<= 8;
        }
    }
    else
    {
        nVelocity = ((static_cast<int32_t>(pMissilesTxt->nVelLev) * pMissileInit->nSkillLevel) / 8
                  + static_cast<int32_t>(pMissilesTxt->nVel)) << 8;
    }

    int32_t nSlowMissilesPct = 100;
    if (pMissileInit->pOwner != nullptr
        && (pMissilesTxt->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_CANSLOW]) != 0
        && STATES_CheckState(pMissileInit->pOwner, STATE_SLOWMISSILES))
    {
        D2StatListStrc* pSlowStatList = STATLIST_GetStatListFromUnitAndState(pMissileInit->pOwner, STATE_SLOWMISSILES);
        if (pSlowStatList != nullptr)
        {
            nSlowMissilesPct = STATLIST_GetStatValue(pSlowStatList, STAT_SKILL_HANDOFATHENA, 0);
            nVelocity        = ENGINE_SafeMulDiv(nSlowMissilesPct, nVelocity, 100);
        }
    }

    if (nVelocity != 0)
    {
        nVelocity = ENGINE_SafeMulDiv(nVelocity, 75, 100);
    }

    D2UnitStrc* pTarget = pMissileInit->pTarget;
    if (nVelocity != 0)
    {
        if (pMissileInit->pOwner != nullptr && pTarget == nullptr)
        {
            if (nTargetX == nX && nTargetY == nY)
            {
                SKILLS_GetForwardOffsetCoords_6FAFDC40(pMissileInit->pOwner, &nTargetX, &nTargetY);
            }
        }
        else if (pTarget != nullptr && pMissileInit->pOwner != nullptr && pTarget != pMissileInit->pOwner)
        {
            if (D2Client_GetUnitX_6FAB1590(pTarget) == D2Client_GetUnitX_6FAB1590(pMissileInit->pOwner)
                && D2Client_GetUnitY_6FAB15C0(pTarget) == D2Client_GetUnitY_6FAB15C0(pMissileInit->pOwner))
            {
                SKILLS_GetForwardOffsetCoords_6FAFDC40(pMissileInit->pOwner, &nTargetX, &nTargetY);
                pTarget = nullptr;
            }
        }

        const int32_t nDestCheckX = (pTarget != nullptr) ? D2Client_GetUnitX_6FAB1590(pTarget) : nTargetX;
        const int32_t nDestCheckY = (pTarget != nullptr) ? D2Client_GetUnitY_6FAB15C0(pTarget) : nTargetY;

        if (abs(nDestCheckX - nX) >= 100 || abs(nDestCheckY - nY) >= 100)
        {
            return nullptr;
        }
    }

    D2UnitStrc* pMissile = D2Client_CreateClientSideUnit(nX, nY, pMissileInit->nMissile, UNIT_MISSILE, 0);
    if (pMissile == nullptr)
    {
        return nullptr;
    }

    UNITS_SetAnimStartFrame(pMissile);
    STATLIST_AllocStatListEx(pMissile, 0, nullptr, nullptr);

    pMissile->dwFrameCountPrecise = static_cast<int32_t>(pMissilesTxt->nAnimLen) << 8;
    MISSILE_SetAnimSpeed(pMissile, static_cast<int32_t>(pMissilesTxt->nAnimSpeed) << 4);

    int32_t nRange = 0;
    if ((pMissileInit->dwFlags & CLTMISSILEINIT_CUSTOM_RANGE) != 0)
    {
        nRange = pMissileInit->nRange;
    }
    else
    {
        nRange = static_cast<int16_t>(pMissilesTxt->wRange)
               + pMissileInit->nSkillLevel * static_cast<int16_t>(pMissilesTxt->wLevRange);

        if ((pMissileInit->dwFlags & CLTMISSILEINIT_ADD_SUBLOOP_RANGE) != 0 && pMissilesTxt->nSubLoop != 0)
        {
            nRange += pMissileInit->nLoops * (static_cast<int32_t>(pMissilesTxt->nSubStop) - static_cast<int32_t>(pMissilesTxt->nSubStart));
        }

        if (nSlowMissilesPct != 0 && nSlowMissilesPct != 100)
        {
            nRange += ENGINE_SafeMulDiv(100, nRange, nSlowMissilesPct);
        }
    }

    if ((pMissileInit->dwFlags & CLTMISSILEINIT_START_FRAME) != 0)
    {
        nRange -= pMissileInit->nFrame;
        pMissile->nSeqCurrentFramePrecise = pMissileInit->nFrame << 8;
    }

    MISSILE_SetTotalFrames(pMissile, nRange);
    MISSILE_SetCurrentFrame(pMissile, nRange);

    const int32_t nActivateDelay = ((pMissileInit->dwFlags & CLTMISSILEINIT_CUSTOM_ACTIVATE) != 0)
        ? pMissileInit->nActivateFrame
        : static_cast<int32_t>(pMissilesTxt->nActivate);
    MISSILE_SetActivateFrame(pMissile, nRange - nActivateDelay);

    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    if (pPath == nullptr)
    {
        D2Client_RemoveUnitFromHashTable(
            gapClientSideUnitHashTables[pMissile->dwUnitType],
            pMissile->dwUnitId & 0x7Fu,
            pMissile->dwUnitId,
            pMissile->dwUnitType);
        return nullptr;
    }

    PATH_SetVelocity(pPath, 0, s_szMissileFile, 7924);
    if (pTarget != nullptr)
    {
        PATH_SetTargetUnit(pPath, pTarget);
    }
    else
    {
        D2COMMON_10170_PathSetTargetPos(pPath, nTargetX, nTargetY);
    }

    if (nVelocity != 0)
    {
        PATH_SetVelocity(pPath, nVelocity, s_szMissileFile, 7935);
        D2Common_10142(pPath, pMissile, 0);
    }

    if (pMissilesTxt->nInitSteps != 0)
    {
        pMissile->dwFlagEx |= UNITFLAGEX_NODRAW;
    }

    MISSILE_SetOwner(pMissile, pMissileInit->pOwner);
    PATH_SetAcceleration(pPath, pMissilesTxt->wAccel);
    PATH_SetMaxVelocity(pPath, static_cast<int32_t>(pMissilesTxt->nMaxVel) << 8);

    if ((pMissileInit->dwFlags & CLTMISSILEINIT_RANDOM_DIRECTION) != 0)
    {
        const uint8_t nRandDir = static_cast<uint8_t>(D2Client_RollRandom_6FAB1020(&pMissile->pSeed, 64));
        PATH_SetDirection(pMissile->pDynamicPath, nRandDir);
    }

    PARTICLE_AllocForUnit(pMissile);
    PARTICLE_CancelFinished(pMissile);

    if ((pMissileInit->dwFlags & CLTMISSILEINIT_DIST_DIV_VEL) != 0)
    {
        uint32_t nDist = UNITS_GetDistanceToCoordinates(pMissile, nTargetX, nTargetY);
        if (nDist == 0)
        {
            nDist = 1;
        }

        const uint32_t nSpeed16 = static_cast<uint32_t>(nVelocity << 4);
        if (nSpeed16 != 0)
        {
            const int32_t nTravelFrames = static_cast<int32_t>((nDist << 16) / nSpeed16);
            MISSILE_SetCurrentFrame(pMissile, nTravelFrames);
            MISSILE_SetTotalFrames(pMissile, nTravelFrames);
        }
    }

    if ((pMissileInit->dwFlags & CLTMISSILEINIT_GRAVITY_ARC) != 0)
    {
        PARTICLE_InitGravityArc(pMissile, MISSILE_GetTotalFrames(pMissile), pMissileInit->nGfxArg);
        PARTICLE_UpdateUnitPhysics(pMissile);

        const uint8_t nLobDir = static_cast<uint8_t>((nTargetY + nTargetX) & 0x3F);
        PATH_SetDirection(pPath, nLobDir);
        D2COMMON_10203_PATH_SetRotateFlag(pPath, FALSE);
    }

    MISSILE_SetSkill(pMissile, pMissileInit->nSkill);
    MISSILE_SetLevel(pMissile, static_cast<uint16_t>(pMissileInit->nSkillLevel));

    if (pMissileInit->pOwner != nullptr)
    {
        UNITS_StoreOwner(pMissile, pMissileInit->pOwner);
        MISSILE_InitPierceCountOnMissile(pMissileInit->pOwner, pMissile);
    }

    pMissile->dwFlags &= ~static_cast<uint32_t>(UNITFLAG_ISVALIDTARGET);

    if (gbMissileLightEffectsEnabled && (pMissileInit->dwFlags & CLTMISSILEINIT_NO_LIGHT) == 0)
    {
        if (pMissilesTxt->nLight != 0 || static_cast<uint8_t>(pMissileInit->nLightRadius) != 0)
        {
            D2Client_FreeUnitLight(pMissile);
            pMissile->pLight = static_cast<D2GfxLightStrc*>(D2Client_AllocLightMap_6FB0EAF0(
                pMissile,
                1,
                pMissilesTxt->nLight,
                255,
                pMissilesTxt->nRGB[0],
                pMissilesTxt->nRGB[1],
                pMissilesTxt->nRGB[2]));
        }
    }
    else
    {
        D2Client_FreeUnitLight(pMissile);
    }

    if (pMissileInit->pInitFunc != nullptr)
    {
        pMissileInit->pInitFunc(pMissile, pMissileInit->pInitArgs);
    }

    const int16_t wTravelSound = static_cast<int16_t>(pMissilesTxt->wTravelSound);
    if (wTravelSound != 0)
    {
        D2Client_PlaySound(wTravelSound, pMissile, 0, FALSE, 0);
        if (pMissileInit->pOwner != nullptr)
        {
            const uint32_t dwOwnerStartSound = D2Client_FindUnitActiveSoundHandle_6FAE6C10(pMissileInit->pOwner, 314);
            if (dwOwnerStartSound != 0)
            {
                D2Client_StopSoundHandle_6FAE2120(dwOwnerStartSound);
            }
        }
    }

    if (pMissileInit->pOwner != nullptr)
    {
        MONUNIQUE_TriggerUModEvent4(pMissileInit->pOwner, pMissile);
    }

    return pMissile;
}

// ============================================================================
// Reference Table Slot Mappings for Part 1 Handlers:
//
// gCltMissileDoFuncTable (0x6FBA67C8, 91 slots, gnCltMissileDoFuncs = 91):
//   [ 1] = D2Client_StepMissileMovementAndCollision      (0x6FAD31F0)
//   [ 2] = MISSILE_CltDo02_StopOnAnimEndAndKeepInViewport(0x6FAD4A90)
//   [ 5] = MISSILE_CltDo05_FireWall_SubLoopAndFadeDown   (0x6FAD49C0)
//   [ 7] = MISSILE_CltDo07_GuidedArrow_BoneSpirit        (0x6FAD48E0)
//   [11] = MISSILE_CltDo11_StopAndMarkDeadAtAnimEnd      (0x6FAD47F0)
//   [12] = MISSILE_CltDo12_StartScreenShakeOnFirstFrame  (0x6FAD4730)
//   [14] = MISSILE_CltDo14_HomingStepTowardTarget        (0x6FAD44B0)
//   [20] = MISSILE_CltDo20_SpiralOutwardCurve            (0x6FAD43E0)
//   [23] = MISSILE_CltDo23_LoopLifetimeFrames            (0x6FAD43B0)
//   [32] = MISSILE_CltDo32_RadamentDeath                 (0x6FAD3D70)
//   [34] = MISSILE_CltDo34_BrainDeathSpirit              (0x6FAD39F0)
//   [37] = MISSILE_CltDo37_DiabloAppearsShakeAndRoar     (0x6FAD3F30)
//   [39] = MISSILE_CltDo39_FreezeAnimFrameWhileOwnerExists(0x6FAD3EB0)
//   [41] = MISSILE_CltDo41_TowerMistTrail                (0x6FAD3ED0)
//   [43] = MISSILE_CltDo43_FollowOwnerRiderMissile       (0x6FAD42C0)
//   [44] = MISSILE_CltDo44_BlazeTrailFollowOwner         (0x6FAD41C0)
//   [49] = MISSILE_CltDo49_PeriodicTrailingSubMissile    (0x6FAD4150)
//   [58] = MISSILE_CltDo58_SpiralTwisterPath             (0x6FAD4000)
//   [59] = MISSILE_CltDo59_CullOutsideVerticalBounds     (0x6FAD36F0)
//   [60] = MISSILE_CltDo60_RotateEveryOtherFrame         (0x6FAD3630)
//   [63] = MISSILE_CltDo63_RotateEveryFrame              (0x6FAD3950)
//   [65] = MISSILE_CltDo65_IzualControlOrbitSpirit       (0x6FAD3740)
//   [67] = MISSILE_CltDo67_TimedThreeSubMissileSpawner   (0x6FAD4620)
//
// gCltMissileHitFuncTable (0x6FBA6938, 81/83 slots, gnCltMissileHitFuncs = 81):
//   [ 3] = MISSILE_CltHit03_SpawnHitAndGroundDebrisSubMissiles (0x6FAD2AB0)
//   [10] = MISSILE_CltHit10_AttachProgOverlayToTarget          (0x6FAD3FA0)
//   [13] = MISSILE_CltHit13_GuidedArrow_BoneSpirit             (0x6FAD2D80)
//   [14] = MISSILE_CltHit14_MeteorCenter                       (0x6FAD22A0)
//   [19] = MISSILE_CltHit19_SpawnRandomRangeHitSubMissile      (0x6FAD2A30)
//   [24] = MISSILE_CltHit24_SpawnSubMissileOnUnitHitOnly       (0x6FAD2230)
//   [31] = MISSILE_CltHit31_IceBreakMelt                       (0x6FAD1FE0)
//   [34] = MISSILE_CltHit34_HideOwnerUnit                      (0x6FAD2210)
//   [39] = MISSILE_CltHit39_SpawnRandomNextSubMissile          (0x6FAD1F00)
//   [40] = MISSILE_CltHit40_FilterPlayerOwnPetCollision        (0x6FAD1FA0)
//   [44] = MISSILE_CltHit44_SpawnSubMissileCopyDirectionAndTargetX (0x6FAD2170)
//   [48] = MISSILE_CltHit48_CatapultColdBall                   (0x6FAD25A0)
//   [54] = MISSILE_CltHit54_SpawnSubMissileOnExpireOnly        (0x6FAD2910)
//   [61] = MISSILE_CltHit61_ClearScreenShake                   (0x6FAD1CA0)
// ============================================================================


// ============================================================================
// Cross-Slice Forwarding Aliases for UNIT_MissileHandlers.cpp (Part 2)
// ============================================================================
D2UnitStrc* __fastcall MISSILE_CreateClientMissileAtCoords(D2UnitStrc* pOwner, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkillId, int32_t nSkillLevel) { return D2Client_SpawnClientMissileAtCoords(pOwner, nMissileId, nX, nY, nSkillId, nSkillLevel); }
int32_t __fastcall MISSILE_GetAncientsControlSpawnDelay(int32_t nRemainingFrames) { return MISSILE_GetSmokeSpawnIntervalByRemainingFrames(nRemainingFrames); }
int32_t __fastcall MISSILE_GetBaalFxSpawnRate(int32_t nRemainingFrames, D2MissilesTxt* pMissilesTxtRecord) { return MISSILE_GetRampValueByFrameAndRange(nRemainingFrames, pMissilesTxtRecord); }
int32_t __fastcall MISSILE_GetTowerChestSpawnInterval(D2UnitStrc* pMissile) { return MISSILE_GetSpawnPeriodByRemainingFrames(pMissile); }
void __fastcall MISSILE_InitFunc_ChargedBolt(D2UnitStrc* pMissile, int32_t nInitArg) { MISSILE_InitChargedBoltPath(pMissile, nInitArg); }
D2UnitStrc* __fastcall MISSILE_CreateClientMissileFromOrigin(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nTargetX, int32_t nTargetY, int32_t nSkillId, int32_t nSkillLevel) { return D2Client_SpawnSubMissileFromMissile(pOrigin, nMissileId, nTargetX, nTargetY, nSkillId, nSkillLevel); }
void __fastcall MISSILE_InitFunc_SyncLoopAnimFrame(D2UnitStrc* pMissile, int32_t pOriginUnit) { MISSILE_InitTrailSubMissileHeightAndFrame(pMissile, pOriginUnit); }
int32_t __fastcall MISSILE_ProcessCollisionHit(D2UnitStrc* pMissile, D2UnitStrc* pTarget, BOOL bForceDestroy) { return D2Client_DestroyClientMissile(pMissile, pTarget, bForceDestroy); }
void __fastcall MISSILE_DeleteClientMissile(D2UnitStrc* pMissile) { D2Client_FreeMissileUnitAndLight(pMissile); }
void __fastcall MISSILE_InitFallingDebrisParticle(D2UnitStrc* pMissile) { MISSILE_InitFallingVerticalParticle(pMissile); }
void __fastcall MISSILE_InitRandomDurationDebrisParticle(D2UnitStrc* pMissile) { MISSILE_InitClampedFallingVerticalParticle(pMissile); }
D2UnitStrc* __fastcall MISSILE_CreateClientMissile(D2MissileStrc* pMissileParams) { return D2Client_CreateClientMissile(pMissileParams); }
void __fastcall MISSILE_CltDo_BasicMissile(D2UnitStrc* pMissile) { D2Client_StepMissileMovementAndCollision(pMissile); }
void __fastcall MISSILE_CltDo_PlagueJavelin_PoisonTrap(D2UnitStrc* pMissile) { MISSILE_CltDo02_StopOnAnimEndAndKeepInViewport(pMissile); }
void __fastcall MISSILE_CltDo_FireWall_MeteorFire(D2UnitStrc* pMissile) { MISSILE_CltDo05_FireWall_SubLoopAndFadeDown(pMissile); }
void __fastcall MISSILE_CltDo_GuidedArrow_BoneSpirit(D2UnitStrc* pMissile) { MISSILE_CltDo07_GuidedArrow_BoneSpirit(pMissile); }
void __fastcall MISSILE_CltDo_FadeLightAtEndOfCycle(D2UnitStrc* pMissile) { MISSILE_CltDo11_StopAndMarkDeadAtAnimEnd(pMissile); }
void __fastcall MISSILE_CltDo_DiabWallMaker(D2UnitStrc* pMissile) { MISSILE_CltDo12_StartScreenShakeOnFirstFrame(pMissile); }
void __fastcall MISSILE_CltDo_GrimWard(D2UnitStrc* pMissile) { MISSILE_CltDo14_HomingStepTowardTarget(pMissile); }
void __fastcall MISSILE_CltDo_BladeCreeper(D2UnitStrc* pMissile) { MISSILE_CltDo20_SpiralOutwardCurve(pMissile); }
void __fastcall MISSILE_CltDo_CountdownTimer(D2UnitStrc* pMissile) { MISSILE_CltDo23_LoopLifetimeFrames(pMissile); }
void __fastcall MISSILE_CltDo_IzualDeathControl(D2UnitStrc* pMissile) { MISSILE_CltDo32_RadamentDeath(pMissile); }
void __fastcall MISSILE_CltDo_AndarielDeathControl(D2UnitStrc* pMissile) { MISSILE_CltDo34_BrainDeathSpirit(pMissile); }
void __fastcall MISSILE_CltDo_ScreenShakeTrigger(D2UnitStrc* pMissile) { MISSILE_CltDo37_DiabloAppearsShakeAndRoar(pMissile); }
void __fastcall MISSILE_CltDo_FollowOwnerRemaining(D2UnitStrc* pMissile) { MISSILE_CltDo39_FreezeAnimFrameWhileOwnerExists(pMissile); }
void __fastcall MISSILE_CltDo_AnimSpeedRamp(D2UnitStrc* pMissile) { MISSILE_CltDo41_TowerMistTrail(pMissile); }
void __fastcall MISSILE_CltDo_UpdateAttachedOwnerPos(D2UnitStrc* pMissile) { MISSILE_CltDo43_FollowOwnerRiderMissile(pMissile); }
void __fastcall MISSILE_CltDo_TrailingSubmissile(D2UnitStrc* pMissile) { MISSILE_CltDo44_BlazeTrailFollowOwner(pMissile); }
void __fastcall MISSILE_CltDo_PeriodicSubmissile(D2UnitStrc* pMissile) { MISSILE_CltDo49_PeriodicTrailingSubMissile(pMissile); }
void __fastcall MISSILE_CltDo_VineRecyclerDelay(D2UnitStrc* pMissile) { MISSILE_CltDo58_SpiralTwisterPath(pMissile); }
void __fastcall MISSILE_CltDo_TargetCoordCheck(D2UnitStrc* pMissile) { MISSILE_CltDo59_CullOutsideVerticalBounds(pMissile); }
void __fastcall MISSILE_CltDo_RemainingFrameSpawner(D2UnitStrc* pMissile) { MISSILE_CltDo60_RotateEveryOtherFrame(pMissile); }
void __fastcall MISSILE_CltDo_PathCoordCheck(D2UnitStrc* pMissile) { MISSILE_CltDo63_RotateEveryFrame(pMissile); }
void __fastcall MISSILE_CltDo_NihlathakDeathControl(D2UnitStrc* pMissile) { MISSILE_CltDo65_IzualControlOrbitSpirit(pMissile); }
void __fastcall MISSILE_CltDo_BaalTauntControl(D2UnitStrc* pMissile) { MISSILE_CltDo67_TimedThreeSubMissileSpawner(pMissile); }
int32_t __fastcall MISSILE_CltHit_ExplosivePotion(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit03_SpawnHitAndGroundDebrisSubMissiles(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_GuidedArrow_BoneSpirit(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit10_AttachProgOverlayToTarget(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_GlacialSpike(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit13_GuidedArrow_BoneSpirit(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_MeteorCenter(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit14_MeteorCenter(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_FingerMageSpider(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit19_SpawnRandomRangeHitSubMissile(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_PantherPotOrange(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit24_SpawnSubMissileOnUnitHitOnly(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_FireHead(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit31_IceBreakMelt(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_OwnerCheckHit(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit34_HideOwnerUnit(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_ImpSpawnMonsters(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit39_SpawnRandomNextSubMissile(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_CatapultSpikeBall(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit40_FilterPlayerOwnPetCollision(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_ExplodingJavelin(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit44_SpawnSubMissileCopyDirectionAndTargetX(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_MoltenBoulderEmerge(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit48_CatapultColdBall(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_BaalSpawnMonsters(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit54_SpawnSubMissileOnExpireOnly(pMissile, pTarget); }
int32_t __fastcall MISSILE_CltHit_ChargedBoltHit(D2UnitStrc* pMissile, D2UnitStrc* pTarget) { return MISSILE_CltHit61_ClearScreenShake(pMissile, pTarget); }


// ============================================================================
// Cross-TU VA-Mapped Adapters for UNIT_Missile.cpp & UNIT_MissileHandlers.cpp
// ============================================================================
struct D2LightNodeStrc;
struct D2UnitLightViewStrc;
extern void __fastcall LIGHTMAP_StartFadeOutLightNode(D2LightNodeStrc* pLight); // 0x6FB0E4C0
extern int32_t __fastcall LIGHTMAP_GetLightNodeRadius(D2LightNodeStrc* pLight); // 0x6FB0E5E0
extern void __fastcall LIGHTMAP_SetLightNodeRadiusImmediate(int32_t nRadius, D2LightNodeStrc* pLight); // 0x6FB0EA30
extern D2LightNodeStrc* __stdcall LIGHTMAP_CreateLightNode(D2UnitLightViewStrc* pUnit, int32_t nLightType, int32_t nRadius, uint8_t nIntensity, uint8_t nRed, uint8_t nGreen, uint8_t nBlue); // 0x6FB0EAF0
extern void __fastcall PARTICLE_AllocForUnit_Real(D2UnitLightViewStrc* pUnit);
extern void __fastcall GFX_AttachOverlayToUnit(D2UnitStrc* pUnit, int nOverlayId, int bPreDrawOverride); // 0x6FB430E0
extern void VIEW_StartScreenShake(uint32_t dwSustainMs, int32_t nAmplitude, uint32_t dwAttackMs, uint32_t dwReleaseMs);
extern D2ActiveRoomStrc* __fastcall ENGINE_FindRoomBySubtileCoords(D2ActiveRoomStrc* pStartRoom, int32_t nX, int32_t nY); // 0x6FAB1A00
extern int32_t __fastcall D2Client_CanTargetOwnPetWithEquippedSkill(D2UnitStrc* pPlayer, D2UnitStrc* pTargetMonster); // 0x6FACF1E0
extern int32_t __fastcall D2Client_CanTargetAlliedUnitWithEquippedSkill(D2UnitStrc* pPlayer, D2UnitStrc* pTargetUnit); // 0x6FACF7B0
extern void __fastcall D2Client_FadeOutAndStopSoundHandle(uint32_t dwHandle, uint32_t dwDurationMs); // 0x6FAE2120
extern void __fastcall D2Client_DetachUnitFromSoundChannel(uint32_t dwHandle, D2UnitStrc* pUnit); // 0x6FAE2150
extern int32_t __fastcall D2Client_GetUnitLastSoundMode(D2UnitStrc* pUnit); // 0x6FAE6820
extern void __fastcall MISSILE_SpawnDualRingPoisonCloud(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nLoops, int32_t nInnerStep, int32_t nOuterStep); // 0x6FAD85F0
extern D2UnitStrc* __fastcall MISSILE_SpawnLobMissileToSkillTarget(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel); // 0x6FADB400
extern D2UnitStrc* __fastcall MISSILE_SpawnParabolicLobMissile(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nSkillId, int32_t nSkillLevel, int32_t nExtra); // 0x6FADB4B0

// Globals aliased by UNIT_MissileHandlers.cpp
int32_t gnFrameDeltaTicks = 1; // 0x6FBB32DC
uint32_t gdwScreenShakeStartTick = 0; // 0x6FBAD104
int32_t gnScreenShakeFadeInMs = 0;    // 0x6FBAD108
int32_t gnScreenShakeHoldMs = 0;      // 0x6FBAD10C
int32_t gnScreenShakeFadeOutMs = 0;   // 0x6FBAD110
int32_t gnScreenShakeAmplitude = 0;   // 0x6FBCB9B0
int32_t gnMissileRandFramePhase_6FBCB9AC = 0; // 0x6FBCB9AC

// 1.12a: 0x6FAB1020
uint32_t __fastcall D2Client_RollRandom_6FAB1020(D2SeedStrc* pSeed, int32_t nMax)
{
    return (pSeed && nMax > 0) ? static_cast<uint32_t>(SEED_RollLimitedRandomNumber(pSeed, nMax)) : 0u;
}
// 1.12a: 0x6FAB1070
uint32_t __fastcall D2Client_RollRandom_6FAB1070(D2SeedStrc* pSeed, int32_t nMax)
{
    return (pSeed && nMax > 0) ? static_cast<uint32_t>(SEED_RollLimitedRandomNumber(pSeed, nMax)) : 0u;
}
// 1.12a: 0x6FAB1670
BOOL __fastcall MONSTERS_CheckMonStats2Flag(int32_t nMonsterId, int32_t nFlagBitIndex)
{
    if (nMonsterId < 0 || nFlagBitIndex < 0 || nFlagBitIndex >= 32 || !sgptDataTables || nMonsterId >= sgptDataTables->nMonStatsTxtRecordCount) return FALSE;
    const int16_t nIdx = sgptDataTables->pMonStatsTxt[nMonsterId].wMonStatsEx;
    if (nIdx < 0 || nIdx >= sgptDataTables->nMonStats2TxtRecordCount) return FALSE;
    return (sgptDataTables->pMonStats2Txt[nIdx].dwFlags & gdwBitMasks[nFlagBitIndex & 31]) != 0;
}
// 1.12a: 0x6FAB1A00
D2ActiveRoomStrc* __fastcall DUNGEON_FindActRoomBySubtileCoords(D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY)
{
    return ENGINE_FindRoomBySubtileCoords(pRoom, nX, nY);
}
D2ActiveRoomStrc* __fastcall D2Client_FindRoomCoveringCoords_6FAB1A00(D2ActiveRoomStrc* pOriginRoom, int32_t nX, int32_t nY)
{
    return ENGINE_FindRoomBySubtileCoords(pOriginRoom, nX, nY);
}
// 1.12a: 0x6FAB2E10
int32_t __fastcall SKILLS_GetSkillParam2PlusLevelParam3(int32_t nSkillId, int32_t nSkillLevel)
{
    if (nSkillId < 0 || !sgptDataTables || nSkillId >= sgptDataTables->nSkillsTxtRecordCount || nSkillLevel <= 0) return 0;
    const D2SkillsTxt& sk = sgptDataTables->pSkillsTxt[nSkillId];
    return sk.dwParam[2] + (nSkillLevel - 1) * sk.dwParam[3];
}
// 1.12a: 0x6FACE550
D2UnitStrc* __fastcall CUNIT_FindUnitInHashTable(D2UnitStrc** ppHashTable, D2UnitGUID dwUnitId, int32_t nUnitType)
{
    if (!ppHashTable) return nullptr;
    for (D2UnitStrc* p = ppHashTable[dwUnitId & 0x7Fu]; p != nullptr; p = p->pListNext)
    {
        if (p->dwUnitId == dwUnitId && static_cast<int32_t>(p->dwUnitType) == nUnitType) return p;
    }
    return nullptr;
}
D2UnitStrc* __fastcall D2Client_FindUnitInHashChain_6FACE550(D2UnitStrc* pHead, D2UnitGUID dwUnitGUID, int32_t nUnitType)
{
    for (D2UnitStrc* p = pHead; p != nullptr; p = p->pListNext)
    {
        if (p->dwUnitId == dwUnitGUID && static_cast<int32_t>(p->dwUnitType) == nUnitType) return p;
    }
    return nullptr;
}
// 1.12a: 0x6FACF1E0 / 0x6FACF5B0 / 0x6FACF710 / 0x6FACF7B0 / 0x6FAD0620
BOOL __fastcall CUNIT_AreUnitsAlliedInParty(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2) { return D2Client_CanTargetOwnPetWithEquippedSkill(pUnit1, pUnit2); }
D2UnitStrc* __fastcall CUNIT_GetTargetUnit(D2UnitStrc* pUnit) { return D2Client_ClearActionFrameAndGetTarget(pUnit); }
D2UnitStrc* __fastcall D2Client_GetUnitTargetUnit_6FACF5B0(D2UnitStrc* pUnit) { return D2Client_ClearActionFrameAndGetTarget(pUnit); }
D2UnitStrc* __fastcall CUNIT_GetOwnerUnit(D2UnitStrc* pUnit) { return D2Client_GetUnitOwner(pUnit); }
D2UnitStrc* __fastcall D2Client_GetMissileOwnerUnit_6FACF710(D2UnitStrc* pUnit) { return D2Client_GetUnitOwner(pUnit); }
D2UnitStrc* __fastcall D2Client_ResolveUnitOwner(D2UnitStrc* pUnit) { return D2Client_GetUnitOwner(pUnit); }
BOOL __fastcall CUNIT_AreUnitsFriendlyPets(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2) { return D2Client_CanTargetAlliedUnitWithEquippedSkill(pUnit1, pUnit2); }
BOOL __fastcall D2Client_CanTargetAlliedUnit_6FACF7B0(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2) { return D2Client_CanTargetAlliedUnitWithEquippedSkill(pUnit1, pUnit2); }
BOOL __fastcall D2Client_CanUnitAttackTarget_6FACF980(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2) { return D2Client_AreUnitsHostileOrAttackable(pUnit2, pUnit1); }
void __fastcall CUNIT_RemoveUnitFromHashTable(D2UnitStrc** ppHashTable, int32_t nHashIndex, D2UnitGUID dwUnitId, uint32_t dwUnitType)
{
    D2Client_RemoveUnitFromHashTable(ppHashTable, static_cast<uint32_t>(nHashIndex), dwUnitId, dwUnitType);
}
// Sound & LightMap & Overlay & ScreenShake adapters
void __fastcall D2Client_StopSoundHandle_6FAE2120(uint32_t dwSoundHandle) { D2Client_FadeOutAndStopSoundHandle(dwSoundHandle, 0); }
void __stdcall D2Client_DetachUnitFromSoundHandle_6FAE2150(uint32_t dwSoundHandle, D2UnitStrc* pUnit, BOOL bFadeOut) { (void)bFadeOut; D2Client_DetachUnitFromSoundChannel(dwSoundHandle, pUnit); }
uint32_t __fastcall D2Client_FindUnitActiveSoundHandle_6FAE6C10(D2UnitStrc* pUnit, int32_t nSoundId) { (void)pUnit; (void)nSoundId; return 0; }
uint32_t __fastcall D2Client_FindUnitActiveSoundHandle(D2UnitStrc* pUnit, int32_t nSoundId) { (void)pUnit; (void)nSoundId; return 0; }
void* __fastcall SOUND_PlayUnitSound(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nX, int32_t nY, BOOL bLoop) { (void)nX; (void)nY; D2Client_PlaySound(nSoundId, pUnit, 0, bLoop, 0); return nullptr; }
void __fastcall SOUND_StopUnitSound(D2UnitStrc* pUnit, BOOL bFadeOut) { (void)bFadeOut; D2Client_DetachUnitFromSoundChannel(0, pUnit); }
void* __fastcall SOUND_GetFirstSoundNode(D2UnitStrc* pUnit) { return pUnit ? pUnit->pSoundList : nullptr; }
void __fastcall DLIGHTMAP_StartFadeOutLight_6FB0E4C0(D2GfxLightStrc* pLight) { LIGHTMAP_StartFadeOutLightNode(reinterpret_cast<D2LightNodeStrc*>(pLight)); }
int32_t __fastcall LIGHTMAP_GetLightRadius(D2GfxLightStrc* pLight) { return LIGHTMAP_GetLightNodeRadius(reinterpret_cast<D2LightNodeStrc*>(pLight)); }
void __fastcall LIGHTMAP_SetLightRadius(D2GfxLightStrc* pLight, int32_t nRadius) { LIGHTMAP_SetLightNodeRadiusImmediate(nRadius, reinterpret_cast<D2LightNodeStrc*>(pLight)); }
D2GfxLightStrc* __fastcall LIGHTMAP_AllocLightMap(D2UnitStrc* pUnit, int32_t nLightType, int32_t nRadius, uint8_t nIntensity, uint8_t nRed, uint8_t nGreen, uint8_t nBlue)
{
    return reinterpret_cast<D2GfxLightStrc*>(LIGHTMAP_CreateLightNode(reinterpret_cast<D2UnitLightViewStrc*>(pUnit), nLightType, nRadius, nIntensity, nRed, nGreen, nBlue));
}
void __fastcall OVERLAY_SetUnitOverlay(int32_t nOverlayId, D2UnitStrc* pUnit, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8)
{
    (void)a4; (void)a5; (void)a6; (void)a7; (void)a8;
    GFX_AttachOverlayToUnit(pUnit, nOverlayId, a3);
}
extern void VIEW_StartScreenShake(uint32_t dwSustainMs, int32_t nAmplitude, uint32_t dwAttackMs, uint32_t dwReleaseMs);
void __stdcall VIEW_StartScreenShake_Stdcall(uint32_t dwSustainMs, int32_t nAmplitude, uint32_t dwAttackMs, uint32_t dwReleaseMs)
{
    VIEW_StartScreenShake(dwSustainMs, nAmplitude, dwAttackMs, dwReleaseMs);
}
void __fastcall CAMERA_StartScreenShake(int32_t nHoldMs, int32_t nAmp, int32_t nAttackMs, int32_t nReleaseMs)
{
    VIEW_StartScreenShake(static_cast<uint32_t>(nHoldMs), nAmp, static_cast<uint32_t>(nAttackMs), static_cast<uint32_t>(nReleaseMs));
}

// Cross-missile spawn aliases used by SKILLS_Skills.cpp & SKILLS_Classes.cpp
struct D2ClientMissileInitStrc;
D2UnitStrc* __fastcall D2Client_CreateClientMissile(D2ClientMissileInitStrc* pInit) { return D2Client_CreateClientMissile(reinterpret_cast<D2MissileStrc*>(pInit)); }
D2UnitStrc* __fastcall D2Client_CreateClientMissile_6FAD4B20(D2MissileStrc* pInit) { return D2Client_CreateClientMissile(pInit); }
D2UnitStrc* __fastcall D2Client_SpawnTargetedLinearMissile_6FAD1940(D2UnitStrc* pOwner, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkillId, int32_t nSkillLevel) { return D2Client_SpawnClientMissileAtCoords(pOwner, nMissileId, nX, nY, nSkillId, nSkillLevel); }
D2UnitStrc* __fastcall D2Client_SpawnLinearSkillMissile_6FAD1E80(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nTargetX, int32_t nTargetY, int32_t nSkillId, int32_t nSkillLevel) { return D2Client_SpawnSubMissileFromMissile(pOrigin, nMissileId, nTargetX, nTargetY, nSkillId, nSkillLevel); }
D2UnitStrc* __fastcall D2Client_SpawnFallingTargetMissile(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel) { return MISSILE_SpawnLobMissileToSkillTarget(pOrigin, nMissileId, nSkillId, nSkillLevel); }
D2UnitStrc* __fastcall D2Client_SpawnLobMissile_6FADB4B0(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nSkillId, int32_t nSkillLevel, int32_t nExtra) { return MISSILE_SpawnParabolicLobMissile(pOrigin, nMissileId, nX, nY, nTargetX, nTargetY, nSkillId, nSkillLevel, nExtra); }
D2UnitStrc* __fastcall D2Client_SpawnLobbedMissile(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nSkillId, int32_t nSkillLevel, int32_t nExtra) { return MISSILE_SpawnParabolicLobMissile(pOrigin, nMissileId, nX, nY, nTargetX, nTargetY, nSkillId, nSkillLevel, nExtra); }
void __fastcall D2Client_SpawnRadialSubMissileBurst(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nRangeOverride, int32_t nSecondaryStride, int32_t nPrimaryStride) { MISSILE_SpawnDualRingPoisonCloud(pOrigin, nMissileId, nRangeOverride, nSecondaryStride, nPrimaryStride); }
