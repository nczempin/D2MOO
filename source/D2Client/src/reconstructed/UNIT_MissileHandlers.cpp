// Diablo II 1.12a - D2Client.dll: UNIT/Missile.cpp (Part 2: 0x6FAD5150..0x6FADDDA5)
// Client Missile CltDoFunc & CltHitFunc Handlers, Internal Helpers, and Dispatch Tables.

#include <cstdint>
#include <cstring>
#include <Windows.h>

#include <D2BasicTypes.h>
#include <D2CommonDefinitions.h>
#include <Fog.h>
#include <D2DataTbls.h>
#include <D2Seed.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <D2Monsters.h>
#include <D2States.h>
#include <D2Skills.h>
#include <DataTbls/MissilesIds.h>
#include <DataTbls/MissilesTbls.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/SkillsTbls.h>
#include <Path/Path.h>
#include <Path/Step.h>
#include <Units/Units.h>
#include <Units/Missile.h>
#include <Units/UnitFinds.h>

#pragma pack(push, 1)

// 1.12a: Client particle node (0x4C bytes), allocated in ENGINE/Particle.cpp (0x6FB75280)
struct D2ClientParticleStrc
{
    uint32_t dwFlags;           // 0x00
    int32_t nPosX;              // 0x04 (11-bit fixed point offset)
    int32_t nPosY;              // 0x08 (11-bit fixed point offset)
    int32_t nPosZ;              // 0x0C (11-bit fixed point height)
    int32_t nVelX;              // 0x10 (11-bit fixed point velocity)
    int32_t nVelY;              // 0x14 (11-bit fixed point velocity)
    int32_t nVelZ;              // 0x18 (11-bit fixed point velocity)
    int32_t nAccX;              // 0x1C (11-bit fixed point acceleration)
    int32_t nAccY;              // 0x20 (11-bit fixed point acceleration)
    int32_t nAccZ;              // 0x24 (11-bit fixed point acceleration)
    uint32_t unk0x28[6];        // 0x28
    int32_t nBounceVelParam;    // 0x40
    int32_t nBounceCountParam;  // 0x44
    int32_t nFrames;            // 0x48
};

#ifndef D2_GFX_DATA_STRC_DEFINED
#define D2_GFX_DATA_STRC_DEFINED
// 1.12a: D2GfxDataStrc client layout view (pUnit->pGfxData at +0x54; particle pointer at +0x30)
struct D2GfxDataStrc
{
    uint8_t unk0x00[0x30];              // 0x00
    D2ClientParticleStrc* pParticle;    // 0x30
};
#endif

// 1.12a: D2MissileStrc::dwFlags bitmask values (verified from 0x6FAD4B20 MISSILE_CreateClientMissile)
enum D2C_ClientMissileInitFlags : uint32_t
{
    CLT_MISSILE_INIT_USE_COORDS         = 0x00000001, // Use (nX, nY) instead of pOrigin coords
    CLT_MISSILE_INIT_TARGET_OFFSET      = 0x00000002, // Target pos = (nX + nTargetX, nY + nTargetY)
    CLT_MISSILE_INIT_USE_VELOCITY       = 0x00000004, // Use nVelocity override
    CLT_MISSILE_INIT_USE_LOOPS          = 0x00000008, // Add (nSubStop - nSubStart) * nLoops to frames
    CLT_MISSILE_INIT_PRECISE_VELOCITY   = 0x00000010, // nVelocity is already 8-bit fixed-point
    CLT_MISSILE_INIT_USE_TARGET_COORDS  = 0x00000020, // Target pos = (nTargetX, nTargetY)
    CLT_MISSILE_INIT_LOB                = 0x00000100, // Initialize parabolic lob arc with nGfxArg
    CLT_MISSILE_INIT_USE_FRAME          = 0x00000200, // Subtract nFrame from total frames and set wFrame = nFrame << 8
    CLT_MISSILE_INIT_DISTANCE_FRAMES    = 0x00000400, // Compute frame count from distance / velocity
    CLT_MISSILE_INIT_USE_ACTIVATE_FRAME = 0x00000800, // Use nActivateFrame instead of Missiles.txt nActivate
    CLT_MISSILE_INIT_RANDOM_DIRECTION   = 0x00002000, // Roll random direction [0..63]
    CLT_MISSILE_INIT_NO_LIGHT           = 0x00004000, // Skip lightmap allocation
    CLT_MISSILE_INIT_USE_RANGE          = 0x00008000, // Use nRange override instead of wRange + lvl * wLevRange
};

// 1.12a: Radial callback context passed to SKILLS_ForEachUnitInRadius (0x6FAFE9B0)
struct D2MissileRadialCallbackArgsStrc
{
    D2UnitStrc* pOwner;         // 0x00
    D2UnitStrc* pOrigin;        // 0x04
    int32_t nSkillId;           // 0x08
    int32_t nSkillLevel;        // 0x0C
    int32_t nMaxTargets;        // 0x10
    int32_t nSubMissileId;      // 0x14
};

struct D2ClientAuraCallbackStrc
{
    D2UnitStrc* pCaster;                        // 0x00
    int32_t nMatchCount;                        // 0x04
    D2MissileRadialCallbackArgsStrc* pArgs;     // 0x08
};

struct D2SpawnerStageEntryStrc
{
    int32_t nThresholdFrame;    // 0x00
    int32_t nPeriodA;           // 0x04
    int32_t nPeriodB;           // 0x08
};

struct D2DeathControlStageStrc
{
    int32_t nOffsetX;           // 0x00
    int32_t nOffsetY;           // 0x04
    int32_t nDirection;         // 0x08
};

#pragma pack(pop)

// 1.12a: Function pointer types for client missile dispatch tables
using CltMissileDoFunc = void(__fastcall*)(D2UnitStrc* pMissile);
// Verified from 0x6FAD3003..0x6FAD3007 (`mov edx, edi; mov ecx, esi; call eax`, 0 stack args, plain `ret`):
using CltMissileHitFunc = int32_t(__fastcall*)(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
using CltAuraUnitCallbackFunc = int32_t(__fastcall*)(D2ClientAuraCallbackStrc* pCallback, D2UnitStrc* pTarget);

// ============================================================================
// 1.12a Read-Only Lookup Tables (.rdata: 0x6FB861DC..0x6FB86C18) & Globals
// ============================================================================

// 1.12a: 0x6FB861DC (used in Part 1 / spawner stages)
static const D2SpawnerStageEntryStrc gSpawnerStages_6FB861DC[5] = {
    { 420, 64, 0 }, { 435, 20, 1 }, { 470, 8, 1 }, { 270, 5, 1 }, { 0, 64, 0 }
};

// 1.12a: 0x6FB86218 (used in 0x6FAD5C20 CltDo[36] Baal Death Control)
static const D2SpawnerStageEntryStrc gBaalDeathStages_6FB86218[5] = {
    { 350, 3, 5 }, { 300, 2, 3 }, { 200, 3, 2 }, { 175, 5, 1 }, { 0, 100, 5 }
};

// 1.12a: 0x6FB86254 (used in 0x6FAD6190 CltDo[33] Diablo Death Control)
static const D2SpawnerStageEntryStrc gDiabDeathStages_6FB86254[5] = {
    { 350, 10, 1 }, { 300, 5, 1 }, { 200, 4, 1 }, { 175, 3, 1 }, { 0, 100, 0 }
};

// 1.12a: 0x6FB86290
static const D2SpawnerStageEntryStrc gSpawnerStages_6FB86290[6] = {
    { 300, 25, 1 }, { 250, 10, 3 }, { 100, 8, 4 }, { 75, 6, 4 }, { 30, 4, 3 }, { 0, 100, 0 }
};

// 1.12a: 0x6FB862D8..0x6FB86333 (used in 0x6FAD65B0 CltDo[31] Mephisto Death Control; base ptr starts at +8 = 0x6FB862E0)
static const D2DeathControlStageStrc gMephDeathBeamOffsets_6FB862D8[7] = {
    {  3, -2,  4 },
    {  1, -3, 58 },
    {  3,  1, 13 },
    { -2, -3, 49 },
    {  1,  3, 22 },
    { -3,  0, 40 },
    { -2,  2, 31 },
};

// 1.12a: 0x6FB86330 & 0x6FB86378 (used in 0x6FAD75A0 Volcano/Molten Boulder crater pattern)
static const int32_t gnCraterOffsetsY_6FB86330[18] = {
    -2, -2, 2, 5, 3, 3, 3, 2, 1, -1, -1, -2, -2, -3, -4, -3, -3, -2
};
// 1.12a: 0x6FB86378
static const int32_t gnCraterOffsetsX_6FB86378[18] = {
    2, -2, 0, 0, -3, 0, 3, -1, 1, -1, 2, -4, -3, -1, 0, 1, 3, 4
};

// 1.12a: 0x6FB863C0 & 0x6FB864C0 (64-direction Nova Y/X ring offsets, radius 30, used in 0x6FAD7930 CltHit[4])
static const int32_t gnNovaOffsetsY_6FB863C0[64] = {
     0,  2,  5,  8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29,
    30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11,  8,  5,  2,
     0, -2, -5, -8,-11,-14,-16,-19,-21,-23,-24,-26,-27,-28,-29,-29,
   -30,-29,-29,-28,-27,-26,-24,-23,-21,-19,-16,-14,-11, -8, -5, -2
};
// 1.12a: 0x6FB864C0
static const int32_t gnNovaOffsetsX_6FB864C0[64] = {
    30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11,  8,  5,  2,
     0, -2, -5, -8,-11,-14,-16,-19,-21,-23,-24,-26,-27,-28,-29,-29,
   -30,-29,-29,-28,-27,-26,-24,-23,-21,-19,-16,-14,-11, -8, -5, -2,
     0,  2,  5,  8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29
};

// 1.12a: 0x6FB865C0 & 0x6FB866C0 (64-direction Frost Nova / Frozen Orb ring offsets, used in 0x6FAD7A40 CltHit[30])
static const int32_t gnFrostNovaOffsetsY_6FB865C0[64] = {
     0,  2,  5,  8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29,
    30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11,  8,  5,  2,
     0, -2, -5, -8,-11,-14,-16,-19,-21,-23,-24,-26,-27,-28,-29,-29,
   -30,-29,-29,-28,-27,-26,-24,-23,-21,-19,-16,-14,-11, -8, -5, -2
};
// 1.12a: 0x6FB866C0
static const int32_t gnFrostNovaOffsetsX_6FB866C0[64] = {
    30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11,  8,  5,  2,
     0, -2, -5, -8,-11,-14,-16,-19,-21,-23,-24,-26,-27,-28,-29,-29,
   -30,-29,-29,-28,-27,-26,-24,-23,-21,-19,-16,-14,-11, -8, -5, -2,
     0,  2,  5,  8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29
};

// 1.12a: 0x6FB867C0 & 0x6FB867E0 (8-direction ring offsets, used in 0x6FAD7CC0)
static const int32_t gnEightDirOffsetsY_6FB867C0[8] = {  2,  2,  0, -2, -2, -2,  0,  2 };
// 1.12a: 0x6FB867E0
static const int32_t gnEightDirOffsetsX_6FB867E0[8] = {  0,  2,  2,  2,  0, -2, -2, -2 };

// 1.12a: 0x6FB86800 & 0x6FB86814 (5-point cross offsets, used in 0x6FAD7F70 CltHit[18] Meteor)
static const int32_t gnMeteorCrossOffsetsY_6FB86800[5] = { 0,  0,  4,  0, -4 };
// 1.12a: 0x6FB86814
static const int32_t gnMeteorCrossOffsetsX_6FB86814[5] = { 0,  4,  0, -4,  0 };

// 1.12a: 0x6FB86828 & 0x6FB86870 (18-point ring offsets, used in 0x6FAD7F70 CltHit[18] Meteor fire ring)
static const int32_t gnMeteorRingOffsetsY_6FB86828[18] = {
    -2, -2, 2, 5, 3, 3, 3, 2, 1, -1, -1, -2, -2, -3, -4, -3, -3, -2
};
// 1.12a: 0x6FB86870
static const int32_t gnMeteorRingOffsetsX_6FB86870[18] = {
    2, -2, 0, 0, -3, 0, 3, -1, 1, -1, 2, -4, -3, -1, 0, 1, 3, 4
};

// 1.12a: 0x6FB868F4 & 0x6FB86914 (8-direction unit step offsets, used in 0x6FADB170 Poison Cloud)
static const int32_t gnUnitDirOffsetsX_6FB868F4[8] = { 0, 1, 1, 0, -1, -1, -1,  0 };
// 1.12a: 0x6FB86914
static const int32_t gnUnitDirOffsetsY_6FB86914[8] = { 1, 0, 1, 1,  1,  0, -1, -1 };

// 1.12a: 0x6FB86934 & 0x6FB86954 (8-direction radius-16 offsets, used in 0x6FAD84F0 CltHit[64])
static const int32_t gnEightWay16OffsetsY_6FB86934[8] = {  0, 16, 16, 16,  0, -16, -16, -16 };
// 1.12a: 0x6FB86954
static const int32_t gnEightWay16OffsetsX_6FB86954[8] = { 16, 16,  0, -16, -16, -16,  0,  16 };

// 1.12a: 0x6FB86978 & 0x6FB869B8 (16-direction radial burst offsets, used in 0x6FAD85F0)
static const int32_t gnSixteenDirOffsetsY_6FB86978[16] = {
    2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2, 2
};
// 1.12a: 0x6FB869B8
static const int32_t gnSixteenDirOffsetsX_6FB869B8[16] = {
    0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1
};

// 1.12a: 0x6FB869F8 & 0x6FB86A08 (4-cardinal offsets, used in 0x6FAD9A40 CltDo[28])
static const int32_t gnCardinalOffsetsY_6FB869F8[4] = {  0, -1,  0,  1 };
// 1.12a: 0x6FB86A08
static const int32_t gnCardinalOffsetsX_6FB86A08[4] = { -1,  0,  1,  0 };

// 1.12a: 0x6FB86A18 & 0x6FB86B18 (64-direction Frozen Orb bolt ring offsets, used in 0x6FADA4D0 & 0x6FADA5F0)
static const int32_t gnFrozenOrbOffsetsY_6FB86A18[64] = {
     0,  2,  5,  8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29,
    30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11,  8,  5,  2,
     0, -2, -5, -8,-11,-14,-16,-19,-21,-23,-24,-26,-27,-28,-29,-29,
   -30,-29,-29,-28,-27,-26,-24,-23,-21,-19,-16,-14,-11, -8, -5, -2
};
// 1.12a: 0x6FB86B18
static const int32_t gnFrozenOrbOffsetsX_6FB86B18[64] = {
    30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11,  8,  5,  2,
     0, -2, -5, -8,-11,-14,-16,-19,-21,-23,-24,-26,-27,-28,-29,-29,
   -30,-29,-29,-28,-27,-26,-24,-23,-21,-19,-16,-14,-11, -8, -5, -2,
     0,  2,  5,  8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29
};

// 1.12a: 0x6FBA676C (3 explosion missile IDs used in 0x6FADBA20 CltDo[64]: 69, 104, 105)
static const int32_t gnShenkExplosionMissiles_6FBA676C[3] = { 69, 104, 105 };

// 1.12a: Client globals referenced by UNIT/Missile.cpp Part 2
extern uint32_t gdwScreenShakeStartTick;            // 1.12a: 0x6FBAD104
extern int32_t gnScreenShakeFadeInMs;               // 1.12a: 0x6FBAD108
extern int32_t gnScreenShakeHoldMs;                 // 1.12a: 0x6FBAD10C
extern int32_t gnScreenShakeFadeOutMs;              // 1.12a: 0x6FBAD110
extern int32_t gnScreenShakeAmplitude;              // 1.12a: 0x6FBCB9B0
extern int32_t gnMissileRandFramePhase_6FBCB9AC;        // 1.12a: 0x6FBCB9AC
extern D2UnitStrc* gapClientSideUnitHashTables[6][128]; // 1.12a: 0x6FBC9D60
extern D2UnitStrc* gapServerSideUnitHashTables[6][128]; // 1.12a: 0x6FBCA960
#define gpServerUnitHashTables gapClientSideUnitHashTables
#define gpClientMissileLists   (gapClientSideUnitHashTables[UNIT_MISSILE])
#define gpClientUnitHashTables gapServerSideUnitHashTables

// ============================================================================
// External Helper Declarations (Part 1 0x6FAD1880..0x6FAD514F & Other Modules)
// ============================================================================

// --- UNIT/Missile.cpp Part 1 Core Helpers ---
// 1.12a: 0x6FAD1940
extern D2UnitStrc* __fastcall MISSILE_CreateClientMissileAtCoords(D2UnitStrc* pOwner, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkillId, int32_t nSkillLevel);
// 1.12a: 0x6FAD19B0
extern int32_t __fastcall MISSILE_GetAncientsControlSpawnDelay(int32_t nRemainingFrames);
// 1.12a: 0x6FAD1A10
extern int32_t __fastcall MISSILE_GetBaalFxSpawnRate(int32_t nRemainingFrames, D2MissilesTxt* pMissilesTxtRecord);
// 1.12a: 0x6FAD1A60
extern int32_t __fastcall MISSILE_GetTowerChestSpawnInterval(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD1CC0
extern void __fastcall MISSILE_InitFunc_ChargedBolt(D2UnitStrc* pMissile, int32_t nInitArg);
// 1.12a: 0x6FAD1E80
extern D2UnitStrc* __fastcall MISSILE_CreateClientMissileFromOrigin(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nTargetX, int32_t nTargetY, int32_t nSkillId, int32_t nSkillLevel);
// 1.12a: 0x6FAD2970
extern void __fastcall MISSILE_InitFunc_SyncLoopAnimFrame(D2UnitStrc* pMissile, int32_t pOriginUnit);
// 1.12a: 0x6FAD2E50
extern int32_t __fastcall MISSILE_ProcessCollisionHit(D2UnitStrc* pMissile, D2UnitStrc* pTarget, BOOL bForceDestroy);
// 1.12a: 0x6FAD3150
extern void __fastcall MISSILE_DeleteClientMissile(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD4310
extern void __fastcall MISSILE_InitFallingDebrisParticle(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD4830
extern void __fastcall MISSILE_InitRandomDurationDebrisParticle(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD4B20
extern D2UnitStrc* __fastcall MISSILE_CreateClientMissile(D2MissileStrc* pMissileParams);

// --- UNIT/Missile.cpp Part 1 CltDoFunc Entries (23 handlers for gCltMissileDoFuncTable) ---
// 1.12a: 0x6FAD31F0 (CltDo[1])
extern void __fastcall MISSILE_CltDo_BasicMissile(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD4A90 (CltDo[2])
extern void __fastcall MISSILE_CltDo_PlagueJavelin_PoisonTrap(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD49C0 (CltDo[5])
extern void __fastcall MISSILE_CltDo_FireWall_MeteorFire(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD48E0 (CltDo[7])
extern void __fastcall MISSILE_CltDo_GuidedArrow_BoneSpirit(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD47F0 (CltDo[11])
extern void __fastcall MISSILE_CltDo_FadeLightAtEndOfCycle(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD4730 (CltDo[12])
extern void __fastcall MISSILE_CltDo_DiabWallMaker(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD44B0 (CltDo[14])
extern void __fastcall MISSILE_CltDo_GrimWard(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD43E0 (CltDo[20])
extern void __fastcall MISSILE_CltDo_BladeCreeper(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD43B0 (CltDo[23])
extern void __fastcall MISSILE_CltDo_CountdownTimer(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD3D70 (CltDo[32])
extern void __fastcall MISSILE_CltDo_IzualDeathControl(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD39F0 (CltDo[34])
extern void __fastcall MISSILE_CltDo_AndarielDeathControl(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD3F30 (CltDo[37])
extern void __fastcall MISSILE_CltDo_ScreenShakeTrigger(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD3EB0 (CltDo[39])
extern void __fastcall MISSILE_CltDo_FollowOwnerRemaining(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD3ED0 (CltDo[41])
extern void __fastcall MISSILE_CltDo_AnimSpeedRamp(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD42C0 (CltDo[43])
extern void __fastcall MISSILE_CltDo_UpdateAttachedOwnerPos(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD41C0 (CltDo[44])
extern void __fastcall MISSILE_CltDo_TrailingSubmissile(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD4150 (CltDo[49])
extern void __fastcall MISSILE_CltDo_PeriodicSubmissile(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD4000 (CltDo[58])
extern void __fastcall MISSILE_CltDo_VineRecyclerDelay(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD36F0 (CltDo[59])
extern void __fastcall MISSILE_CltDo_TargetCoordCheck(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD3630 (CltDo[60])
extern void __fastcall MISSILE_CltDo_RemainingFrameSpawner(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD3950 (CltDo[63])
extern void __fastcall MISSILE_CltDo_PathCoordCheck(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD3740 (CltDo[65])
extern void __fastcall MISSILE_CltDo_NihlathakDeathControl(D2UnitStrc* pMissile);
// 1.12a: 0x6FAD4620 (CltDo[67])
extern void __fastcall MISSILE_CltDo_BaalTauntControl(D2UnitStrc* pMissile);

// --- UNIT/Missile.cpp Part 1 CltHitFunc Entries (14 handlers for gCltMissileHitFuncTable) ---
// 1.12a: 0x6FAD2AB0 (CltHit[3])
extern int32_t __fastcall MISSILE_CltHit_ExplosivePotion(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD3FA0 (CltHit[10])
extern int32_t __fastcall MISSILE_CltHit_GuidedArrow_BoneSpirit(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD2D80 (CltHit[13])
extern int32_t __fastcall MISSILE_CltHit_GlacialSpike(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD22A0 (CltHit[14])
extern int32_t __fastcall MISSILE_CltHit_MeteorCenter(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD2A30 (CltHit[19])
extern int32_t __fastcall MISSILE_CltHit_FingerMageSpider(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD2230 (CltHit[24])
extern int32_t __fastcall MISSILE_CltHit_PantherPotOrange(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD1FE0 (CltHit[31])
extern int32_t __fastcall MISSILE_CltHit_FireHead(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD2210 (CltHit[34])
extern int32_t __fastcall MISSILE_CltHit_OwnerCheckHit(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD1F00 (CltHit[39])
extern int32_t __fastcall MISSILE_CltHit_ImpSpawnMonsters(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD1FA0 (CltHit[40])
extern int32_t __fastcall MISSILE_CltHit_CatapultSpikeBall(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD2170 (CltHit[44])
extern int32_t __fastcall MISSILE_CltHit_ExplodingJavelin(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD25A0 (CltHit[48])
extern int32_t __fastcall MISSILE_CltHit_MoltenBoulderEmerge(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD2910 (CltHit[54])
extern int32_t __fastcall MISSILE_CltHit_BaalSpawnMonsters(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
// 1.12a: 0x6FAD1CA0 (CltHit[61])
extern int32_t __fastcall MISSILE_CltHit_ChargedBoltHit(D2UnitStrc* pMissile, D2UnitStrc* pTarget);

// --- External D2Client Subsystem Helpers ---
// 1.12a: 0x6FAB1670
extern BOOL __fastcall MONSTERS_CheckMonStats2Flag(int32_t nMonsterId, int32_t nFlagBitIndex);
// 1.12a: 0x6FAB1A00
extern D2ActiveRoomStrc* __fastcall DUNGEON_FindActRoomBySubtileCoords(D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY);
// 1.12a: 0x6FAB2E10
extern int32_t __fastcall SKILLS_GetSkillParam2PlusLevelParam3(int32_t nSkillId, int32_t nSkillLevel);
// 1.12a: 0x6FACE550
extern D2UnitStrc* __fastcall CUNIT_FindUnitInHashTable(D2UnitStrc** ppHashTable, D2UnitGUID dwUnitId, int32_t nUnitType);
// 1.12a: 0x6FACF1E0
extern BOOL __fastcall CUNIT_AreUnitsAlliedInParty(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2);
// 1.12a: 0x6FACF5B0
extern D2UnitStrc* __fastcall CUNIT_GetTargetUnit(D2UnitStrc* pUnit);
// 1.12a: 0x6FACF710
extern D2UnitStrc* __fastcall CUNIT_GetOwnerUnit(D2UnitStrc* pUnit);
// 1.12a: 0x6FACF7B0
extern BOOL __fastcall CUNIT_AreUnitsFriendlyPets(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2);
// 1.12a: 0x6FAD0620
extern void __fastcall CUNIT_RemoveUnitFromHashTable(D2UnitStrc** ppHashTable, int32_t nHashIndex, D2UnitGUID dwUnitId, uint32_t dwUnitType);
// 1.12a: 0x6FAE1FA0
extern void* __fastcall SOUND_PlayUnitSound(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nX, int32_t nY, BOOL bLoop);
// 1.12a: 0x6FAE2150
extern void __fastcall SOUND_StopUnitSound(D2UnitStrc* pUnit, BOOL bFadeOut);
// 1.12a: 0x6FAE6820
extern void* __fastcall SOUND_GetFirstSoundNode(D2UnitStrc* pUnit);
// 1.12a: 0x6FAFE320
extern BOOL __fastcall SKILLS_GetSkillTargetCoords(D2UnitStrc* pUnit, int32_t* pX, int32_t* pY);
// 1.12a: 0x6FAFE9B0
extern void __stdcall SKILLS_ForEachUnitInRadius(D2UnitStrc* pCaster, int32_t nX, int32_t nY, int32_t nRadius, uint32_t dwAuraFilter, CltAuraUnitCallbackFunc pfnCallback, void* pCallbackArgs, BOOL bCheckLos);
// 1.12a: 0x6FAFEBA0
extern D2UnitStrc* __fastcall SKILLS_FindNearestTargetInRadius(D2UnitStrc* pCaster, int32_t nX, int32_t nY, int32_t nRadius, uint32_t dwAuraFilter, D2UnitGUID dwExcludeGUID);
// 1.12a: 0x6FAFF710
extern void __fastcall SKILLS_SpawnChargedBoltFan(D2MissileStrc* pMissileParams, int32_t nBoltCount);
// 1.12a: 0x6FAFF900
extern void __stdcall SKILLS_SpawnRandomRadiusSubmissiles(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nCount, int32_t nRadius, int32_t nSkillId, int32_t nSkillLevel);
// 1.12a: 0x6FB01BC0
extern void __fastcall SKILLS_InitChargedBoltPath(D2UnitStrc* pMissile, int32_t nLowSeed);
// 1.12a: 0x6FB0E5E0
extern int32_t __fastcall LIGHTMAP_GetLightRadius(D2GfxLightStrc* pLight);
// 1.12a: 0x6FB0EA30
extern void __fastcall LIGHTMAP_SetLightRadius(D2GfxLightStrc* pLight, int32_t nRadius);
// 1.12a: 0x6FB0EAF0
extern D2GfxLightStrc* __fastcall LIGHTMAP_AllocLightMap(D2UnitStrc* pUnit, int32_t nType, int32_t nRadius, uint8_t nIntensity, uint8_t nRed, uint8_t nGreen, uint8_t nBlue);
// 1.12a: 0x6FB430E0
extern void __fastcall OVERLAY_SetUnitOverlay(int32_t nOverlayId, D2UnitStrc* pUnit, int32_t nMode, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8);
// 1.12a: 0x6FB6A420
extern void __fastcall CAMERA_StartScreenShake(int32_t nHoldMs, int32_t nAmplitude, int32_t nFadeInMs, int32_t nFadeOutMs);
// 1.12a: 0x6FB75280
extern void __fastcall PARTICLE_AllocMissileParticle(D2UnitStrc* pMissile);
// 1.12a: 0x6FB75400
extern void __fastcall PARTICLE_InitLobTrajectory(D2UnitStrc* pMissile, int32_t nFrames, int32_t nGfxArg);
// 1.12a: 0x6FB755B0
extern void __fastcall PARTICLE_InitVerticalRise(D2UnitStrc* pMissile, int32_t nStartZ, int32_t nTargetZ, int32_t nUnused);
// 1.12a: 0x6FB75640
extern int32_t __fastcall PARTICLE_GetHeightInPixels(D2UnitStrc* pMissile);
// 1.12a: 0x6FB75730
extern void __stdcall PARTICLE_EnableActiveUpdateFlag(D2UnitStrc* pMissile);

// ============================================================================
// Inlined Table & Unit Coordinate Helpers (matching 1.12a LTCG inlining)
// ============================================================================

static inline D2MissilesTxt* MISSILE_GetMissilesTxt(int32_t nMissileId)
{
    if (nMissileId >= 0 && nMissileId < sgptDataTables->nMissilesTxtRecordCount)
    {
        return &sgptDataTables->pMissilesTxt[nMissileId];
    }
    return nullptr;
}

static inline D2SkillsTxt* MISSILE_GetSkillsTxt(int32_t nSkillId)
{
    if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
    {
        return &sgptDataTables->pSkillsTxt[nSkillId];
    }
    return nullptr;
}

// 1.12a: 0x6FAB1590 (also inlined across UNIT/Missile.cpp)
static inline int32_t CLIENTS_GetUnitX(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }

    switch (pUnit->dwUnitType)
    {
    case UNIT_OBJECT:
    case UNIT_ITEM:
    case UNIT_TILE:
        return pUnit->pStaticPath ? pUnit->pStaticPath->nXPos : 0;
    default:
        return pUnit->pDynamicPath ? PATH_GetXPosition(pUnit->pDynamicPath) : 0;
    }
}

// 1.12a: 0x6FAB15C0 (also inlined across UNIT/Missile.cpp)
static inline int32_t CLIENTS_GetUnitY(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }

    switch (pUnit->dwUnitType)
    {
    case UNIT_OBJECT:
    case UNIT_ITEM:
    case UNIT_TILE:
        return pUnit->pStaticPath ? pUnit->pStaticPath->nYPos : 0;
    default:
        return pUnit->pDynamicPath ? PATH_GetYPosition(pUnit->pDynamicPath) : 0;
    }
}

static inline void MISSILE_SetParticlePosition(D2UnitStrc* pMissile, int32_t nPosX, int32_t nPosY, int32_t nPosZ)
{
    if (pMissile && pMissile->pGfxData && pMissile->pGfxData->pParticle)
    {
        D2ClientParticleStrc* pParticle = pMissile->pGfxData->pParticle;
        pParticle->nPosX = nPosX;
        pParticle->nPosY = nPosY;
        pParticle->nPosZ = nPosZ;
    }
}

static inline void MISSILE_SetParticleVelocity(D2UnitStrc* pMissile, int32_t nVelX, int32_t nVelY, int32_t nVelZ)
{
    if (pMissile && pMissile->pGfxData && pMissile->pGfxData->pParticle)
    {
        D2ClientParticleStrc* pParticle = pMissile->pGfxData->pParticle;
        pParticle->nVelX = nVelX;
        pParticle->nVelY = nVelY;
        pParticle->nVelZ = nVelZ;
    }
}

static inline void MISSILE_SetParticleAcceleration(D2UnitStrc* pMissile, int32_t nAccX, int32_t nAccY, int32_t nAccZ)
{
    if (pMissile && pMissile->pGfxData && pMissile->pGfxData->pParticle)
    {
        D2ClientParticleStrc* pParticle = pMissile->pGfxData->pParticle;
        pParticle->nAccX = nAccX;
        pParticle->nAccY = nAccY;
        pParticle->nAccZ = nAccZ;
    }
}

// ============================================================================
// Decompiled Procedures (0x6FAD5150..0x6FADDDA5)
// ============================================================================

// 1.12a: 0x6FAD5150
int32_t __fastcall MISSILE_CltHit_RandomTargetSubmissile(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wCltHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    SEED_InitLowSeed(&pMissile->pSeed, CLIENTS_GetUnitX(pMissile));

    int32_t nRadius = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
    if (nRadius < 1)
    {
        nRadius = 1;
    }

    const int32_t nUnitX = CLIENTS_GetUnitX(pMissile);
    const int32_t nUnitY = CLIENTS_GetUnitY(pMissile);
    const int32_t nSpan = 2 * nRadius + 1;
    const int32_t nTargetX = (nUnitX - nRadius) + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan));
    const int32_t nTargetY = (nUnitY - nRadius) + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan));

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_TARGET_COORDS;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nMissile = pMissilesTxtRecord->wCltHitSubMissile[0];
    missileParams.nTargetX = nTargetX;
    missileParams.nTargetY = nTargetY;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    MISSILE_CreateClientMissile(&missileParams);

    return 1;
}

// 1.12a: 0x6FAD5290
void __fastcall MISSILE_CltDo_SanctuarySpawner(D2UnitStrc* pMissile)
{
    if ((MISSILE_GetRemainingFrames(pMissile) % 3) == 0)
    {
        const int32_t nRandTargetOffsetX = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 20);
        const int32_t nRandTargetOffsetY = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 20);

        const int32_t nBaseX = CLIENTS_GetUnitX(pMissile);
        const int32_t nSpawnX = nBaseX + static_cast<int32_t>(SEED_RollRandomNumber(&pMissile->pSeed) & 1) - 1;

        const int32_t nBaseY = CLIENTS_GetUnitY(pMissile);
        const int32_t nSpawnY = nBaseY + static_cast<int32_t>(SEED_RollRandomNumber(&pMissile->pSeed) & 1) - 1;

        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS;
        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.nMissile = MISSILE_SANCTUARYBOLT; // 0x21E (542)
        missileParams.nX = nSpawnX;
        missileParams.nY = nSpawnY;
        missileParams.nTargetX = nSpawnX + nRandTargetOffsetX - 10;
        missileParams.nTargetY = nSpawnY + nRandTargetOffsetY - 10;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        MISSILE_CreateClientMissile(&missileParams);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD5410
static void __fastcall MISSILE_SpawnAncientsMarkerPair(D2UnitStrc* pMissile, D2UnitStrc* pOwner, int32_t nX, int32_t nY)
{
    if (!pMissile || !pOwner)
    {
        return;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
    if (!pRoom)
    {
        return;
    }

    BOOL bNeedValidSpot = TRUE;
    int32_t nAttempts = 0;
    while (nAttempts < 4)
    {
        const int32_t nDeltaX = 17 - static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 34);
        const int32_t nDeltaY = 17 - static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 34);

        if (nDeltaX < -1 || nDeltaX > 1 || nDeltaY < -1)
        {
            const int32_t nTestX = nX + nDeltaX;
            const int32_t nTestY = nY + nDeltaY;
            if (!COLLISION_CheckMask(pRoom, nTestX, nTestY, COLLIDE_MISSILE_BARRIER))
            {
                nX = nTestX;
                nY = nTestY;
                bNeedValidSpot = FALSE;
            }
        }

        ++nAttempts;
        if (!bNeedValidSpot)
        {
            break;
        }
    }

    if (bNeedValidSpot)
    {
        return;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = 0x267; // 615 (ancientdeathcloud / marker 1)
    missileParams.nX = nX;
    missileParams.nY = nY;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    MISSILE_CreateClientMissile(&missileParams);

    missileParams.nMissile = 0x268; // 616 (marker 2)
    MISSILE_CreateClientMissile(&missileParams);
}

// 1.12a: 0x6FAD5570
static void __fastcall MISSILE_SpawnFallingRubble(D2UnitStrc* pMissile, int32_t nX, int32_t nY)
{
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
    if (COLLISION_CheckMaskWithSize(pRoom, nX, nY, COLLISION_UNIT_SIZE_BIG, COLLIDE_WALL))
    {
        return;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_RANDOM_DIRECTION;
    missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
    missileParams.nMissile = 0x27B + static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 3);
    missileParams.nX = nX;
    missileParams.nY = nY;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    D2UnitStrc* pSubMissile = MISSILE_CreateClientMissile(&missileParams);
    if (pSubMissile)
    {
        MISSILE_SetParticlePosition(pSubMissile, 0, 0, 0x7D000);
        MISSILE_SetParticleVelocity(pSubMissile, 0, 0, -0x5000);
        MISSILE_SetTotalFrames(pSubMissile, 25);
        MISSILE_SetCurrentFrame(pSubMissile, 25);
        MISSILE_SetAnimSpeed(pSubMissile, 128);
    }
}

// 1.12a: 0x6FAD5670
static void __fastcall MISSILE_SpawnTowerChestEruptionBolt(D2UnitStrc* pMissile, D2UnitStrc* pOwner)
{
    int32_t nOffsetX = 20 - static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 40);
    const int32_t nOffsetY = 20 - static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 40);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nMissile = 0x11D; // 285
    missileParams.nTargetX = nOffsetX;
    missileParams.nTargetY = nOffsetY;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    if (nOffsetX == 0 && nOffsetY == 0)
    {
        nOffsetX = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 19) + 1;
        missileParams.nTargetX = nOffsetX;
    }

    D2UnitStrc* pSubMissile = MISSILE_CreateClientMissile(&missileParams);
    if (pSubMissile)
    {
        MISSILE_SetTargetX(pSubMissile, nOffsetX);
        MISSILE_SetTargetY(pSubMissile, nOffsetY);
        MISSILE_SetParticlePosition(pSubMissile, 0, 0, -0x5000);
    }
}

// 1.12a: 0x6FAD5790
static void __fastcall MISSILE_SpawnWorldstoneShardDebris(D2UnitStrc* pMissile, D2UnitStrc* pOwner)
{
    static const int32_t gnQuadOffsetsX[4] = { 1, -1, 0, 0 };
    static const int32_t gnQuadOffsetsY[4] = { 0, 0, -1, 0 };

    const int32_t nRandHeight = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 125);
    const int32_t nQuadIdx = static_cast<int32_t>(SEED_RollRandomNumber(&pMissile->pSeed) & 3);

    const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
    const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);
    const int32_t nStepX = gnQuadOffsetsX[nQuadIdx];
    const int32_t nStepY = gnQuadOffsetsY[nQuadIdx];
    const int32_t nSpawnX = nOriginX + nStepX;
    const int32_t nSpawnY = nOriginY + nStepY;

    int32_t nMissileId = 0x25F; // 607
    switch (static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 30)
    {
    case 0:
        nMissileId = 0x260; // 608
        break;
    case 1:
        nMissileId = 0x261; // 609
        break;
    case 2:
        nMissileId = 0x262; // 610
        break;
    case 3:
        nMissileId = 0x263; // 611
        break;
    default:
        nMissileId = 0x25F; // 607
        break;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_TARGET_OFFSET;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = nMissileId;
    missileParams.nX = nSpawnX;
    missileParams.nY = nSpawnY;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    // Note: 0x6FAD5915..0x6FAD5919 writes nStepY to +0x18 (nY) and -nStepX to +0x1C (nTargetX)
    missileParams.nY = nStepY;
    missileParams.nTargetX = -nStepX;

    D2UnitStrc* pSubMissile = MISSILE_CreateClientMissile(&missileParams);
    if (pSubMissile)
    {
        MISSILE_SetParticlePosition(pSubMissile, 0, 0, nRandHeight << 11);
        MISSILE_SetTargetX(pSubMissile, CLIENTS_GetUnitX(pMissile));
        MISSILE_SetTargetY(pSubMissile, CLIENTS_GetUnitY(pMissile));
    }
}

// 1.12a: 0x6FAD59F0
static void __fastcall MISSILE_SpawnOwnerDirectedMeteor(D2UnitStrc* pMissile, D2UnitStrc* pOwner, int32_t nMissileId)
{
    int32_t nDirX = 0;
    int32_t nDirY = 0;
    D2Common_11066(pOwner, &nDirX, &nDirY);
    D2_ASSERT(nDirX != 0 || nDirY != 0);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = nMissileId;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    const int32_t nBaseX = CLIENTS_GetUnitX(pMissile);
    missileParams.nX = nBaseX + static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 20) - 10;

    const int32_t nBaseY = CLIENTS_GetUnitY(pMissile);
    missileParams.nY = nBaseY + static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 20) - 10;

    D2UnitStrc* pSubMissile = MISSILE_CreateClientMissile(&missileParams);
    if (pSubMissile)
    {
        D2MissilesTxt* pSubMissilesTxt = MISSILE_GetMissilesTxt(nMissileId);
        D2_ASSERT(pSubMissilesTxt && pSubMissilesTxt->wRange == 25);

        int32_t nStartSubtileX = nDirX * -25;
        int32_t nStartSubtileY = nDirY * -25;
        int32_t nVelClientX = nDirX;
        int32_t nVelClientY = nDirY;

        // Match exact stack layout passed to DUNGEON_GameSubtileToClientCoords in 0x6FAD5B95..0x6FAD5BAE:
        // First call converts (&nStartSubtileX, &nStartSubtileY) -> client pixel offset
        // Second call converts (&nVelClientX, &nVelClientY) -> client pixel velocity per frame
        DUNGEON_GameSubtileToClientCoords(&nStartSubtileX, &nStartSubtileY);
        DUNGEON_GameSubtileToClientCoords(&nVelClientX, &nVelClientY);

        MISSILE_SetParticlePosition(pSubMissile, nStartSubtileX << 11, nStartSubtileY << 11, 0xC8000);
        MISSILE_SetParticleVelocity(pSubMissile, nVelClientX << 11, nVelClientY << 11, -0x8000);
        PARTICLE_EnableActiveUpdateFlag(pSubMissile);
    }
}

// 1.12a: 0x6FAD5C20
void __fastcall MISSILE_CltDo_BaalDeathControl(D2UnitStrc* pMissile)
{
    const int32_t nCurrentFrame = MISSILE_GetCurrentFrame(pMissile);
    if (nCurrentFrame == 325)
    {
        gdwScreenShakeStartTick = GetTickCount();
        gnScreenShakeFadeInMs = 0;
        gnScreenShakeHoldMs = 6000;
        gnScreenShakeFadeOutMs = 0;
        gnScreenShakeAmplitude = 20;
    }

    int32_t nStageIdx = 0;
    while (nStageIdx < 4 && nCurrentFrame <= gBaalDeathStages_6FB86218[nStageIdx].nThresholdFrame)
    {
        ++nStageIdx;
    }

    const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
    const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

    if ((nCurrentFrame % gBaalDeathStages_6FB86218[nStageIdx].nPeriodA) == 0)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_RANDOM_DIRECTION;
        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.nMissile = 0x2A; // 42
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        const int32_t nRandX = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 40);
        const int32_t nRandY = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 40);
        missileParams.nX = nOriginX + nRandX - 20;
        missileParams.nY = nOriginY + nRandY - 20;
        MISSILE_CreateClientMissile(&missileParams);
    }

    if ((nCurrentFrame % gBaalDeathStages_6FB86218[nStageIdx].nPeriodB) == 0)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_RANDOM_DIRECTION;
        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.nMissile = 0x16C; // 364
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        const int32_t nRandX = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 40);
        const int32_t nRandY = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 40);
        missileParams.nX = nOriginX + nRandX - 20;
        missileParams.nY = nOriginY + nRandY - 20;

        D2UnitStrc* pDebris = MISSILE_CreateClientMissile(&missileParams);
        if (pDebris)
        {
            MISSILE_SetParticlePosition(pDebris, 0, 0, 0x4B000);
            MISSILE_SetParticleVelocity(pDebris, 0, 0, -0x5000);
            MISSILE_SetTotalFrames(pDebris, 15);
            MISSILE_SetCurrentFrame(pDebris, 15);
            MISSILE_SetAnimSpeed(pDebris, 128);
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD5ED0
void __fastcall MISSILE_CltDo_CountessFirewallLobSpawner(D2UnitStrc* pMissile)
{
    if ((MISSILE_GetCurrentFrame(pMissile) % 4) == 0)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS
                              | CLT_MISSILE_INIT_USE_TARGET_COORDS
                              | CLT_MISSILE_INIT_LOB
                              | CLT_MISSILE_INIT_DISTANCE_FRAMES
                              | CLT_MISSILE_INIT_RANDOM_DIRECTION;
        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.nMissile = 0x162; // 354
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
        const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);
        missileParams.nX = nOriginX;
        missileParams.nY = nOriginY;

        const int32_t nRandX = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 12);
        const int32_t nRandY = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 13);
        missileParams.nTargetX = nOriginX - nRandX;
        missileParams.nTargetY = nOriginY + nRandY - 6;
        MISSILE_CreateClientMissile(&missileParams);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD6000
int32_t __fastcall MISSILE_CltHit_LinkReturnSoulMissile(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (pTarget)
    {
        return 0;
    }

    const int32_t nLinkedGUID = MISSILE_GetTargetX(pMissile);
    D2UnitStrc* pLinkedMissile = gpClientMissileLists[nLinkedGUID & 0x7F];
    while (pLinkedMissile && static_cast<int32_t>(pLinkedMissile->dwUnitId) != nLinkedGUID)
    {
        pLinkedMissile = pLinkedMissile->pListNext;
    }

    if (!pLinkedMissile)
    {
        return 1;
    }

    D2_ASSERT(pLinkedMissile->dwUnitType == UNIT_MISSILE);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS
                          | CLT_MISSILE_INIT_USE_TARGET_COORDS
                          | CLT_MISSILE_INIT_RANDOM_DIRECTION
                          | CLT_MISSILE_INIT_USE_RANGE;
    missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
    missileParams.nMissile = 0x160; // 352
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.nRange = MISSILE_GetCurrentFrame(pLinkedMissile);
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);
    missileParams.nTargetX = CLIENTS_GetUnitX(pLinkedMissile);
    missileParams.nTargetY = CLIENTS_GetUnitY(pLinkedMissile);

    D2UnitStrc* pSpawned = MISSILE_CreateClientMissile(&missileParams);
    if (pSpawned)
    {
        MISSILE_SetTargetX(pSpawned, static_cast<int32_t>(pLinkedMissile->dwUnitId));
    }

    return 1;
}

// 1.12a: 0x6FAD6190
void __fastcall MISSILE_CltDo_SoulCollectorControl(D2UnitStrc* pMissile)
{
    const int32_t nCurrentFrame = MISSILE_GetCurrentFrame(pMissile);

    int32_t nStageIdx = 0;
    while (nStageIdx < 4 && nCurrentFrame <= gDiabDeathStages_6FB86254[nStageIdx].nThresholdFrame)
    {
        ++nStageIdx;
    }

    if ((nCurrentFrame % gDiabDeathStages_6FB86254[nStageIdx].nPeriodA) == 0)
    {
        const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
        const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.nMissile = 0x15F; // 351
        (void)MISSILE_GetSkill(pMissile);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        for (int32_t i = 0; i < gDiabDeathStages_6FB86254[nStageIdx].nPeriodB; ++i)
        {
            int32_t nDeltaX = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 10) - 5;
            if ((nDeltaX < 0 ? -nDeltaX : nDeltaX) < 2)
            {
                nDeltaX = 5;
            }

            int32_t nDeltaY = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 10) - 5;
            if ((nDeltaY < 0 ? -nDeltaY : nDeltaY) < 2)
            {
                nDeltaY = 5;
            }

            missileParams.nX = nOriginX + nDeltaX * 2;
            missileParams.nY = nOriginY + nDeltaY * 2;

            D2UnitStrc* pSubMissile = MISSILE_CreateClientMissile(&missileParams);
            if (pSubMissile)
            {
                MISSILE_SetTargetX(pSubMissile, static_cast<int32_t>(pMissile->dwUnitId));
            }
        }
    }

    if (nCurrentFrame < 25)
    {
        MISSILE_SetTargetX(pMissile, 3);
    }
    else if (nCurrentFrame < 75)
    {
        MISSILE_SetTargetX(pMissile, 2);
    }

    if (nCurrentFrame <= 1)
    {
        const int32_t nObjectGUID = MISSILE_GetTargetY(pMissile);
        D2UnitStrc* pObject = gpClientUnitHashTables[UNIT_OBJECT][nObjectGUID & 0x7F];
        while (pObject && static_cast<int32_t>(pObject->dwUnitId) != nObjectGUID)
        {
            pObject = pObject->pListNext;
        }

        if (pObject)
        {
            D2_ASSERT(pObject->dwUnitType == UNIT_OBJECT);
            OVERLAY_SetUnitOverlay(0xB6, pObject, 3, 0, 0, 0, 0, 0);
            if (!pObject->pLight)
            {
                pObject->pLight = LIGHTMAP_AllocLightMap(pObject, 2, 60, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD6420
int32_t __fastcall MISSILE_CltHit_ChargedBoltBurstOnGround(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (pTarget)
    {
        return 0;
    }

    SEED_InitLowSeed(&pMissile->pSeed, MISSILE_GetTargetY(pMissile));
    const int32_t nBoltCount = MISSILE_GetTargetX(pMissile);
    const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
    const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS;
    missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
    missileParams.nMissile = 0x15D; // 349
    missileParams.nX = nOriginX;
    missileParams.nY = nOriginY;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    for (int32_t i = 0; i < nBoltCount; ++i)
    {
        const int32_t nTargetX = nOriginX + static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 20) - 10;
        const int32_t nTargetY = nOriginY + static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 20) - 10;
        missileParams.nTargetX = nTargetX;
        missileParams.nTargetY = nTargetY;

        if (nTargetX == nOriginX && nTargetY == nOriginY)
        {
            --i;
            continue;
        }

        D2UnitStrc* pBolt = MISSILE_CreateClientMissile(&missileParams);
        if (pBolt)
        {
            SKILLS_InitChargedBoltPath(pBolt, nTargetX);
        }
    }

    return 1;
}

// 1.12a: 0x6FAD65B0
void __fastcall MISSILE_CltDo_MephistoDeathControl(D2UnitStrc* pMissile)
{
    const int32_t nCurrentFrame = MISSILE_GetCurrentFrame(pMissile);
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);

    if (nCurrentFrame == 150)
    {
        gdwScreenShakeStartTick = GetTickCount();
        gnScreenShakeFadeInMs = 0;
        gnScreenShakeHoldMs = 4000;
        gnScreenShakeFadeOutMs = 0;
        gnScreenShakeAmplitude = 25;
    }

    int32_t nTriggerFrame = 325;
    for (int32_t i = 0; i < 7; ++i, nTriggerFrame -= 25)
    {
        if (nCurrentFrame > nTriggerFrame)
        {
            break;
        }

        if (nCurrentFrame == nTriggerFrame)
        {
            const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);
            const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
            const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
            const int32_t nSkillId = MISSILE_GetSkill(pMissile);

            if (pOwner)
            {
                D2MissileStrc missileParams = {};
                missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
                missileParams.pOwner = pOwner;
                missileParams.nMissile = 0x154; // 340
                missileParams.nX = nOriginX + gMephDeathBeamOffsets_6FB862D8[i].nOffsetX;
                missileParams.nY = nOriginY + gMephDeathBeamOffsets_6FB862D8[i].nOffsetY;
                missileParams.nSkill = nSkillId;
                missileParams.nSkillLevel = nSkillLevel;

                D2UnitStrc* pBeam = MISSILE_CreateClientMissile(&missileParams);
                if (pBeam)
                {
                    PATH_SetDirection(pBeam->pDynamicPath, static_cast<uint8_t>(gMephDeathBeamOffsets_6FB862D8[i].nDirection));
                    MISSILE_SetTotalFrames(pBeam, nCurrentFrame);
                    MISSILE_SetCurrentFrame(pBeam, nCurrentFrame);
                }
            }
        }
    }

    if (nCurrentFrame < 165 && (nCurrentFrame % 2) == 0)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS;
        missileParams.pOwner = pOwner;
        missileParams.nMissile = 0x153; // 339
        missileParams.nX = CLIENTS_GetUnitX(pMissile);
        missileParams.nY = CLIENTS_GetUnitY(pMissile);
        missileParams.nTargetX = CLIENTS_GetUnitX(pMissile) - 13;
        missileParams.nTargetY = CLIENTS_GetUnitY(pMissile) + 3;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        D2UnitStrc* pSpirit = MISSILE_CreateClientMissile(&missileParams);
        if (pSpirit)
        {
            MISSILE_SetParticlePosition(pSpirit, 0, 0, 0x23000);
            MISSILE_SetParticleVelocity(pSpirit, 0, 0, -0x800);
            PATH_SetDirection(pSpirit->pDynamicPath, 21);
            D2COMMON_10203_PATH_SetRotateFlag(pSpirit->pDynamicPath, FALSE);
        }
    }

    if (nCurrentFrame < 150)
    {
        const int32_t nBaseX = CLIENTS_GetUnitX(pMissile) - 13;
        const int32_t nBaseY = CLIENTS_GetUnitY(pMissile) + 3;

        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS;
        missileParams.pOwner = pOwner;
        missileParams.nMissile = 0x96; // 150
        missileParams.nX = nBaseX;
        missileParams.nY = nBaseY;

        const int32_t nRandX = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 5);
        const int32_t nRandY = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 10);
        missileParams.nTargetX = nBaseX + nRandX + 1;
        missileParams.nTargetY = nBaseY + nRandY - 5;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        D2UnitStrc* pSmoke = MISSILE_CreateClientMissile(&missileParams);
        if (pSmoke)
        {
            UNITS_SetAnimationSpeed(pSmoke, 64);
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD69E0
static void __fastcall MISSILE_SpawnHellforgeSoulBurst(D2UnitStrc* pMissile, int32_t nLinkedObjectGUID)
{
    static const int32_t gnOffsetsX[14] = { -4,  2,  4, -3, -2,  3,  5, -3, -5,  5, -6, -1,  0,  6 };
    static const int32_t gnOffsetsY[14] = {  6,  4, -4, -2,  1,  7,  0,  3, -4, -3,  4, -5,  5, -3 };

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(0x176); // 374
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET
                          | CLT_MISSILE_INIT_LOB
                          | CLT_MISSILE_INIT_USE_RANGE;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nMissile = 0x176; // 374
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    for (int32_t i = 0; i < 14; ++i)
    {
        const int32_t nOffX = gnOffsetsX[i];
        const int32_t nOffY = gnOffsetsY[i];
        missileParams.nTargetX = nOffX;
        missileParams.nTargetY = nOffY;

        const int32_t nAbsX = (nOffX < 0) ? -nOffX : nOffX;
        const int32_t nAbsY = (nOffY < 0) ? -nOffY : nOffY;
        const int32_t nWeightedDist = ((nOffX >= nOffY) ? (nAbsX + 2 * nAbsY) : (nAbsY + 2 * nAbsX)) >> 1;
        missileParams.nRange = (static_cast<int32_t>(static_cast<int16_t>(pMissilesTxtRecord->wRange)) * nWeightedDist) / 6;

        D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
        if (pSub)
        {
            MISSILE_SetTargetX(pSub, nLinkedObjectGUID);
            const int32_t nDelay = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 35) + 10;
            MISSILE_SetTargetY(pSub, nDelay);
        }
    }
}

// 1.12a: 0x6FAD6BF0
int32_t __fastcall MISSILE_CltHit_HellforgeSpiritStage1(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    D2MissilesTxt* pRefMissilesTxt = MISSILE_GetMissilesTxt(0x175); // 373
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS
                          | CLT_MISSILE_INIT_USE_VELOCITY
                          | CLT_MISSILE_INIT_USE_RANGE;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = 0x178; // 376
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.nVelocity = 0;
    missileParams.nRange = static_cast<int16_t>(pRefMissilesTxt->wRange);
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);

    D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
    if (pSub)
    {
        MISSILE_SetTargetX(pSub, MISSILE_GetTargetX(pMissile));
        const int32_t nDelay = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 35) + 10;
        MISSILE_SetTargetY(pSub, nDelay);
    }

    return 1;
}

// 1.12a: 0x6FAD6D30
int32_t __fastcall MISSILE_CltHit_HellforgeSpiritStage2(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nMissile = 0x177; // 375
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
    if (pSub)
    {
        MISSILE_SetTargetX(pSub, MISSILE_GetTargetX(pMissile));
    }

    return 1;
}

// 1.12a: 0x6FAD6DB0
static void __fastcall MISSILE_SpawnAscendingSpiritToObject(D2UnitStrc* pMissile, int32_t nObjectGUID, D2UnitStrc* pOwner)
{
    D2UnitStrc* pObject = gpClientUnitHashTables[UNIT_OBJECT][nObjectGUID & 0x7F];
    while (pObject && static_cast<int32_t>(pObject->dwUnitId) != nObjectGUID)
    {
        pObject = pObject->pListNext;
    }

    if (!pObject)
    {
        return;
    }

    D2_ASSERT(pObject->dwUnitType == UNIT_OBJECT);

    const int32_t nDeltaX = CLIENTS_GetUnitX(pMissile) - CLIENTS_GetUnitX(pObject);
    const int32_t nDeltaY = CLIENTS_GetUnitY(pMissile) - CLIENTS_GetUnitY(pObject);

    D2MissileStrc missileParams = {};
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nMissile = 0x179; // 377
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
    if (pSub)
    {
        PARTICLE_AllocMissileParticle(pSub);
        MISSILE_SetParticlePosition(pSub, 0, 0, 0);
        MISSILE_SetParticleVelocity(pSub, 0, 0, 0x1000);
        MISSILE_SetParticleAcceleration(pSub, 0, 0, 0);
        MISSILE_SetTargetX(pSub, nDeltaX);
        MISSILE_SetTargetY(pSub, nDeltaY);
    }
}

// 1.12a: 0x6FAD6F70
D2UnitStrc* __fastcall MISSILE_SpawnDeadFireTrailFrame(D2UnitStrc* pOrigin, int32_t nFrame)
{
    if (!pOrigin->pDynamicPath)
    {
        return nullptr;
    }

    const uint8_t nDir = PATH_GetDirection(pOrigin->pDynamicPath);

    D2MissileStrc missileParams = {};
    missileParams.pOwner = pOrigin;
    missileParams.pOrigin = pOrigin;
    missileParams.nMissile = 0x54; // 84 (MISSILE_FIRESMALL)
    missileParams.nSkillLevel = 1;

    D2UnitStrc* pTrail = MISSILE_CreateClientMissile(&missileParams);
    if (pTrail)
    {
        pTrail->wFrame = nFrame << 8;
        if (pTrail->pDynamicPath)
        {
            D2COMMON_10160_PathUpdateDirection(pTrail->pDynamicPath, nDir);
        }
        pTrail->dwFlags |= UNITFLAG_ISDEAD;
    }

    return pTrail;
}

// 1.12a: 0x6FAD6FF0
static void __fastcall MISSILE_SpawnRandomTargetSubmissileInBox(D2UnitStrc* pMissile, int32_t nHalfRadius, int32_t nMissileId)
{
    const int32_t nDiameter = nHalfRadius * 2;
    const int32_t nOffsetX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nDiameter)) - nHalfRadius;
    const int32_t nOffsetY = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nDiameter)) - nHalfRadius;

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS;
    missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
    missileParams.nMissile = nMissileId;
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);
    missileParams.nTargetX = CLIENTS_GetUnitX(pMissile) + nOffsetX;
    missileParams.nTargetY = CLIENTS_GetUnitY(pMissile) + nOffsetY;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    MISSILE_CreateClientMissile(&missileParams);
}

// 1.12a: 0x6FAD71A0
int32_t __fastcall MISSILE_CltHit_DualSubmissileWithHeight(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
    missileParams.pOrigin = pMissile;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    const int16_t nSub0 = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);
    if (nSub0 >= 0)
    {
        missileParams.nMissile = nSub0;
        MISSILE_CreateClientMissile(&missileParams);
    }

    const int16_t nSub1 = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[1]);
    if (nSub1 >= 0)
    {
        missileParams.nMissile = nSub1;
        D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
        if (pSub)
        {
            MISSILE_SetParticlePosition(pSub, 0, 0, static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]) << 11);
        }
    }

    return 1;
}

// 1.12a: 0x6FAD7280
int32_t __fastcall MISSILE_CltHit_ArmageddonSubmissileSameDir(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
    if (pSub)
    {
        PATH_SetDirection(pSub->pDynamicPath, PATH_GetDirection(pMissile->pDynamicPath));
    }

    return 1;
}

// 1.12a: 0x6FAD7360
int32_t __fastcall MISSILE_CltHit_TripleHitSubmissiles(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    for (int32_t i = 0; i < 3; ++i)
    {
        const int16_t nSubId = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[i]);
        if (nSubId >= 0)
        {
            missileParams.nMissile = nSubId;
            MISSILE_CreateClientMissile(&missileParams);
        }
    }

    return 1;
}

// 1.12a: 0x6FAD7440
int32_t __fastcall MISSILE_CltHit_BounceLobToFirstPoint(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS;
    missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.nTargetX = D2COMMON_10175_PathGetFirstPointX(pMissile->pDynamicPath);
    missileParams.nTargetY = D2COMMON_10176_PathGetFirstPointY(pMissile->pDynamicPath);

    D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
    if (pSub)
    {
        if (pSub->pGfxData && pSub->pGfxData->pParticle)
        {
            D2ClientParticleStrc* pParticle = pSub->pGfxData->pParticle;
            pParticle->dwFlags |= 0x04;
            pParticle->nBounceVelParam = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
            pParticle->nBounceCountParam = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]);
        }

        PARTICLE_InitLobTrajectory(pSub, static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[2]), 0);

        if (pSub->pGfxData && pSub->pGfxData->pParticle)
        {
            pSub->pGfxData->pParticle->dwFlags &= ~0x02u;
        }
    }

    return 1;
}

// 1.12a: 0x6FAD75A0
static void __fastcall MISSILE_SpawnCraterDebrisAndRing(D2UnitStrc* pMissile, int32_t nCountAndRadius, int32_t nVelocity, int32_t nDebrisMissileId, int32_t nRingMissileId)
{
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
    const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
    const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

    if (nDebrisMissileId >= 0)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_VELOCITY
                              | CLT_MISSILE_INIT_USE_TARGET_COORDS
                              | CLT_MISSILE_INIT_LOB
                              | CLT_MISSILE_INIT_DISTANCE_FRAMES;
        missileParams.pOwner = pOwner;
        missileParams.pOrigin = pMissile;
        missileParams.nMissile = 0x1C8; // 456 (moltenboulder / volcano debris)
        missileParams.nVelocity = nVelocity;
        missileParams.nSkill = nSkillId;
        missileParams.nSkillLevel = nSkillLevel;

        if (nCountAndRadius > 0)
        {
            const int32_t nHalfSpan = nCountAndRadius * 2;
            const int32_t nSpan = nHalfSpan * 2 + 1;
            for (int32_t i = nCountAndRadius; i != 0; --i)
            {
                const int32_t nTargetX = nOriginX + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nHalfSpan;
                const int32_t nTargetY = nOriginY + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nHalfSpan;
                if (static_cast<int32_t>(UNITS_GetDistanceToCoordinates(pMissile, nTargetX, nTargetY)) > 3)
                {
                    missileParams.nTargetX = nTargetX;
                    missileParams.nTargetY = nTargetY;
                    MISSILE_CreateClientMissile(&missileParams);
                }
            }
        }
    }

    if (nRingMissileId >= 0)
    {
        for (int32_t i = 0; i < 18; ++i)
        {
            D2MissileStrc missileParams = {};
            missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
            missileParams.pOwner = pOwner;
            missileParams.nMissile = nRingMissileId;
            missileParams.nX = nOriginX + gnCraterOffsetsX_6FB86378[i];
            missileParams.nY = nOriginY + gnCraterOffsetsY_6FB86330[i];
            missileParams.nSkill = nSkillId;
            missileParams.nSkillLevel = nSkillLevel;
            MISSILE_CreateClientMissile(&missileParams);
        }
    }
}

// 1.12a: 0x6FAD7800
int32_t __fastcall MISSILE_CltHit_TowerChestLobSpawner(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET | CLT_MISSILE_INIT_LOB;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);

    int32_t nRadius = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
    if (nRadius < 3)
    {
        nRadius = 3;
    }

    const int32_t nCount = MISSILE_GetTargetX(pMissile);
    if (nCount > 0)
    {
        const int32_t nSpan = 2 * nRadius + 1;
        for (int32_t i = nCount; i != 0; --i)
        {
            missileParams.nSkillLevel = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 5) + 1;

            int32_t nOffX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nRadius;
            const int32_t nOffY = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nRadius;
            if (nOffX == 0 && nOffY == 0)
            {
                nOffX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nRadius)) + 1;
            }

            missileParams.nTargetX = nOffX;
            missileParams.nTargetY = nOffY;
            MISSILE_CreateClientMissile(&missileParams);
        }
    }

    return 1;
}

// 1.12a: 0x6FAD7930
int32_t __fastcall MISSILE_CltHit_ExplodingArrow_NovaRing(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    int32_t nStep = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
    if (nStep < 1)
    {
        nStep = 1;
    }

    for (int32_t i = 0; i < 64; i += nStep)
    {
        missileParams.nTargetX = gnNovaOffsetsX_6FB864C0[i];
        missileParams.nTargetY = gnNovaOffsetsY_6FB863C0[i];

        D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
        if (pSub)
        {
            MISSILE_SetTargetX(pSub, gnNovaOffsetsX_6FB864C0[i]);
            MISSILE_SetTargetY(pSub, gnNovaOffsetsY_6FB863C0[i]);
        }
    }

    return 1;
}

// 1.12a: 0x6FAD7A40
int32_t __fastcall MISSILE_CltHit_FrozenOrbNovaRing(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    int32_t nStep = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
    if (nStep < 1)
    {
        nStep = 1;
    }

    for (int32_t i = 0; i < 64; i += nStep)
    {
        missileParams.nTargetX = gnFrostNovaOffsetsX_6FB866C0[i];
        missileParams.nTargetY = gnFrostNovaOffsetsY_6FB865C0[i];

        D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
        if (pSub)
        {
            MISSILE_SetTargetX(pSub, gnFrostNovaOffsetsX_6FB866C0[i]);
            MISSILE_SetTargetY(pSub, gnFrostNovaOffsetsY_6FB865C0[i]);
        }
    }

    return 1;
}

// 1.12a: 0x6FAD7B50
int32_t __fastcall MISSILE_CltHit_ImmolationFirePatch(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = MISSILE_GetSkillsTxt(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    int32_t nRange = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
    if (nRange == 0)
    {
        nRange = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
        if (nRange < 5)
        {
            nRange = 5;
        }
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);
    // Note: 0x6FAD7C4D..0x6FAD7C51 writes nSkillLevel (ebp) to both nSkill and nSkillLevel
    missileParams.nSkill = nSkillLevel;
    missileParams.nSkillLevel = nSkillLevel;

    if (nRange > 0)
    {
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_RANGE;
        missileParams.nRange = nRange;
    }

    D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
    if (pSub)
    {
        PATH_SetDirection(pSub->pDynamicPath, PATH_GetDirection(pMissile->pDynamicPath));
    }

    return 1;
}

// 1.12a: 0x6FAD7CC0
static int32_t __fastcall MISSILE_SpawnPantherPotGreenRing(D2UnitStrc* pOrigin, int32_t nMissileId, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel, int32_t nStep)
{
    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(nMissileId);
    if (!pMissilesTxtRecord)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS
                          | CLT_MISSILE_INIT_TARGET_OFFSET
                          | CLT_MISSILE_INIT_USE_VELOCITY
                          | CLT_MISSILE_INIT_USE_LOOPS
                          | CLT_MISSILE_INIT_PRECISE_VELOCITY;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pOrigin;
    missileParams.nMissile = nMissileId;
    missileParams.nX = CLIENTS_GetUnitX(pOrigin);
    missileParams.nY = CLIENTS_GetUnitY(pOrigin);
    missileParams.nVelocity = pMissilesTxtRecord->dwParam[0] << 7;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nLoops = nSkillLevel - 1;

    for (int32_t i = 0; i < 8; i += nStep)
    {
        missileParams.nTargetX = gnEightDirOffsetsX_6FB867E0[i];
        missileParams.nTargetY = gnEightDirOffsetsY_6FB867C0[i];
        MISSILE_CreateClientMissile(&missileParams);
    }

    return 1;
}

// 1.12a: 0x6FAD7DF0
static int32_t __fastcall MISSILE_LightningFury_AuraCallback(D2ClientAuraCallbackStrc* pCallback, D2UnitStrc* pTarget)
{
    D2MissileRadialCallbackArgsStrc* pArgs = pCallback->pArgs;
    if (pCallback->nMatchCount >= pArgs->nMaxTargets)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_TARGET_COORDS;
    missileParams.pOwner = pArgs->pOwner;
    missileParams.pOrigin = pArgs->pOrigin;
    missileParams.nMissile = pArgs->nSubMissileId;
    missileParams.nTargetX = CLIENTS_GetUnitX(pTarget);
    missileParams.nTargetY = CLIENTS_GetUnitY(pTarget);
    missileParams.nSkill = pArgs->nSkillId;
    missileParams.nSkillLevel = pArgs->nSkillLevel;
    MISSILE_CreateClientMissile(&missileParams);

    return 1;
}

// 1.12a: 0x6FAD7EB0
static int32_t __fastcall MISSILE_FistOfTheHeavens_AuraCallback(D2ClientAuraCallbackStrc* pCallback, D2UnitStrc* pTarget)
{
    D2MissileRadialCallbackArgsStrc* pArgs = pCallback->pArgs;
    if (pCallback->nMatchCount >= pArgs->nMaxTargets)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_TARGET_COORDS;
    missileParams.pOwner = pArgs->pOwner;
    missileParams.pOrigin = pArgs->pOrigin;
    missileParams.nMissile = pArgs->nSubMissileId;
    missileParams.nTargetX = CLIENTS_GetUnitX(pTarget);
    missileParams.nTargetY = CLIENTS_GetUnitY(pTarget);
    missileParams.nSkill = pArgs->nSkillId;
    missileParams.nSkillLevel = pArgs->nSkillLevel;
    MISSILE_CreateClientMissile(&missileParams);

    return 1;
}

// 1.12a: 0x6FAD7F70
int32_t __fastcall MISSILE_CltHit_MeteorImpact(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
    if (!MISSILE_GetSkillsTxt(nSkillId))
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
    const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_NO_LIGHT;
    missileParams.pOwner = pOwner;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    const int16_t nSub0 = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);
    if (nSub0 >= 0)
    {
        const int32_t nCount0 = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
        missileParams.nMissile = nSub0;
        for (int32_t i = 0; i < nCount0; ++i)
        {
            const uint32_t nIdx = static_cast<uint32_t>(i) % 5;
            missileParams.nX = nOriginX + gnMeteorCrossOffsetsX_6FB86814[nIdx];
            missileParams.nY = nOriginY + gnMeteorCrossOffsetsY_6FB86800[nIdx];
            MISSILE_CreateClientMissile(&missileParams);
        }
    }

    const int16_t nSub1 = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[1]);
    if (nSub1 >= 0)
    {
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_RANGE;
        missileParams.nMissile = nSub1;
        missileParams.nX = nOriginX;
        missileParams.nY = nOriginY;
        missileParams.nRange = SKILLS_GetSkillParam2PlusLevelParam3(nSkillId, nSkillLevel);

        D2UnitStrc* pCenterFire = MISSILE_CreateClientMissile(&missileParams);
        if (pCenterFire && pCenterFire->pLight)
        {
            LIGHTMAP_SetLightRadius(pCenterFire->pLight, 12);
        }
    }

    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_NO_LIGHT | CLT_MISSILE_INIT_USE_RANGE;
    int32_t nRingIndex = 0;
    const int16_t nSub2 = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[2]);
    if (nSub2 >= 0)
    {
        nRingIndex = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]);
        missileParams.nMissile = nSub2;
        for (int32_t i = 0; i < nRingIndex; ++i)
        {
            const uint32_t nIdx = static_cast<uint32_t>(i) % 18;
            missileParams.nX = nOriginX + gnMeteorRingOffsetsX_6FB86870[nIdx];
            missileParams.nY = nOriginY + gnMeteorRingOffsetsY_6FB86828[nIdx];
            MISSILE_CreateClientMissile(&missileParams);
        }
    }

    const int16_t nSub3 = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[3]);
    if (nSub3 >= 0)
    {
        const int32_t nEndIndex = nRingIndex + static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[2]);
        missileParams.nMissile = nSub3;
        for (int32_t i = nRingIndex; i < nEndIndex; ++i)
        {
            const int32_t nIdx = i % 18;
            missileParams.nX = nOriginX + gnMeteorRingOffsetsX_6FB86870[nIdx];
            missileParams.nY = nOriginY + gnMeteorRingOffsetsY_6FB86828[nIdx];
            MISSILE_CreateClientMissile(&missileParams);
        }
    }

    return 1;
}

// 1.12a: 0x6FAD8200
int32_t __fastcall MISSILE_CltHit_ChainLightning(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner || !pTarget)
    {
        return 1;
    }

    if (static_cast<int16_t>(pMissilesTxtRecord->wProgOverlay) > 0)
    {
        OVERLAY_SetUnitOverlay(static_cast<int16_t>(pMissilesTxtRecord->wProgOverlay), pTarget, 2, 0, 0, 0, 0, 0);
    }

    const int32_t nJumpsRemaining = MISSILE_GetTargetX(pMissile);
    if (nJumpsRemaining <= 1)
    {
        return 1;
    }

    const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
    const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

    int32_t nSearchRadius = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
    if (nSearchRadius <= 0)
    {
        const int32_t nSkillId = MISSILE_GetSkill(pMissile);
        const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
        D2SkillsTxt* pSkillsTxtRecord = MISSILE_GetSkillsTxt(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 1;
        }

        nSearchRadius = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
        if (nSearchRadius < 1)
        {
            nSearchRadius = 1;
        }
    }

    D2UnitStrc* pNextTarget = SKILLS_FindNearestTargetInRadius(pOwner, nOriginX, nOriginY, nSearchRadius, 0x88583, pTarget->dwUnitId);
    if (!pNextTarget || pNextTarget == pTarget)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS;
    missileParams.pOwner = pOwner;
    missileParams.pTarget = pNextTarget;
    missileParams.nMissile = pMissile->dwClassId;
    missileParams.nX = nOriginX;
    missileParams.nY = nOriginY;
    missileParams.nTargetX = CLIENTS_GetUnitX(pNextTarget);
    missileParams.nTargetY = CLIENTS_GetUnitY(pNextTarget);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    D2UnitStrc* pNextBolt = MISSILE_CreateClientMissile(&missileParams);
    if (pNextBolt)
    {
        MISSILE_SetTargetX(pNextBolt, nJumpsRemaining - 1);
        MISSILE_SetOwner(pNextBolt, pTarget);
    }

    return 1;
}

// 1.12a: 0x6FAD83C0
int32_t __fastcall MISSILE_CltHit_HolyBolt(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || !pTarget)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (pMissilesTxtRecord->dwCltHitPar[0] != 0 && pOwner)
    {
        if (CUNIT_AreUnitsAlliedInParty(pOwner, pTarget) || CUNIT_AreUnitsFriendlyPets(pOwner, pTarget))
        {
            if (static_cast<int16_t>(pMissilesTxtRecord->wProgOverlay) > 0)
            {
                OVERLAY_SetUnitOverlay(static_cast<int16_t>(pMissilesTxtRecord->wProgOverlay), pTarget, 2, 0, 0, 0, 0, 0);
            }
            return 1;
        }
    }

    if (pTarget->dwUnitType == UNIT_PLAYER)
    {
        if (pMissilesTxtRecord->dwCltHitPar[1] != 0)
        {
            return 0;
        }
    }
    else if (pTarget->dwUnitType == UNIT_MONSTER)
    {
        if (pMissilesTxtRecord->dwCltHitPar[1] == 2)
        {
            if (!MONSTERS_IsDemon(pTarget))
            {
                return 0;
            }
        }
        else
        {
            if (!MONSTERS_IsUndead(pTarget))
            {
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }

    if (static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) >= 0)
    {
        D2MissileStrc missileParams = {};
        missileParams.pOwner = pOwner;
        missileParams.pOrigin = pMissile;
        missileParams.nMissile = 0xCC; // 204 (holyboltexplode)
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        MISSILE_CreateClientMissile(&missileParams);
    }

    return 1;
}

// 1.12a: 0x6FAD84F0
int32_t __fastcall MISSILE_CltHit_EightWaySubmissileBurst(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner || !pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    int32_t nStep = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
    if (nStep < 1)
    {
        nStep = 1;
    }

    for (int32_t i = 0; i < 8; i += nStep)
    {
        missileParams.nTargetX = gnEightWay16OffsetsX_6FB86954[i];
        missileParams.nTargetY = gnEightWay16OffsetsY_6FB86934[i];

        D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
        if (pSub)
        {
            MISSILE_SetTargetX(pSub, gnEightWay16OffsetsX_6FB86954[i]);
            MISSILE_SetTargetY(pSub, gnEightWay16OffsetsY_6FB86934[i]);
        }
    }

    return 1;
}

// 1.12a: 0x6FAD85F0
void __fastcall MISSILE_SpawnDualRingPoisonCloud(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nLoops, int32_t nInnerStep, int32_t nOuterStep)
{
    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(nMissileId);
    if (nMissileId == 0)
    {
        return;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS
                          | CLT_MISSILE_INIT_TARGET_OFFSET
                          | CLT_MISSILE_INIT_USE_VELOCITY
                          | CLT_MISSILE_INIT_PRECISE_VELOCITY;
    if (nLoops > 0)
    {
        missileParams.dwFlags |= CLT_MISSILE_INIT_USE_LOOPS;
        missileParams.nLoops = nLoops;
    }

    missileParams.pOwner = CUNIT_GetOwnerUnit(pOrigin);
    missileParams.pOrigin = pOrigin;
    missileParams.nMissile = nMissileId;
    missileParams.nX = CLIENTS_GetUnitX(pOrigin);
    missileParams.nY = CLIENTS_GetUnitY(pOrigin);
    missileParams.nSkill = MISSILE_GetSkill(pOrigin);
    missileParams.nSkillLevel = MISSILE_GetLevel(pOrigin);
    missileParams.nVelocity = pMissilesTxtRecord->dwParam[0] << 7;

    if (nOuterStep < 1)
    {
        nOuterStep = 1;
    }

    for (int32_t i = 0; i < 16; i += nOuterStep)
    {
        missileParams.nTargetX = gnSixteenDirOffsetsX_6FB869B8[i];
        missileParams.nTargetY = gnSixteenDirOffsetsY_6FB86978[i];
        MISSILE_CreateClientMissile(&missileParams);
    }

    if (nInnerStep > 0)
    {
        missileParams.nVelocity = pMissilesTxtRecord->dwParam[1] << 7;
        for (int32_t i = 1; i < 16; i += nInnerStep)
        {
            missileParams.nTargetX = gnSixteenDirOffsetsX_6FB869B8[i];
            missileParams.nTargetY = gnSixteenDirOffsetsY_6FB86978[i];
            MISSILE_CreateClientMissile(&missileParams);
        }
    }
}

// 1.12a: 0x6FAD8770
void __fastcall MISSILE_SpawnCircularGroundField(int32_t nMissileId, D2UnitStrc* pOrigin, int32_t nCenterX, int32_t nCenterY, int32_t nRadius, int32_t nSkillId, int32_t nSkillLevel, int32_t nSkipChanceDivisor, int32_t nGridStep)
{
    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(nMissileId);

    D2UnitStrc* pOwner = pOrigin;
    if (!pOrigin || (pOrigin->dwUnitType != UNIT_PLAYER && pOrigin->dwUnitType != UNIT_MONSTER))
    {
        pOwner = CUNIT_GetOwnerUnit(pOrigin);
        if (!pOwner)
        {
            return;
        }
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_FRAME;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = nMissileId;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    const int32_t nRadiusSq = nRadius * nRadius;
    for (int32_t dy = -nRadius; dy <= nRadius; dy += nGridStep)
    {
        const int32_t nDySq = dy * dy;
        for (int32_t dx = -nRadius; dx <= nRadius; dx += nGridStep)
        {
            if (dx * dx + nDySq > nRadiusSq)
            {
                continue;
            }

            if (nSkipChanceDivisor > 0 && SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nSkipChanceDivisor) > 0)
            {
                continue;
            }

            const int32_t nSpawnX = nCenterX + dx;
            const int32_t nSpawnY = nCenterY + dy;
            missileParams.nX = nSpawnX;
            missileParams.nY = nSpawnY;

            if (pMissilesTxtRecord->dwRandStart != 0)
            {
                missileParams.nFrame = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, static_cast<int32_t>(pMissilesTxtRecord->dwRandStart)));
            }

            if (DUNGEON_FindActRoomBySubtileCoords(UNITS_GetRoom(pOrigin), nSpawnX, nSpawnY))
            {
                MISSILE_CreateClientMissile(&missileParams);
            }
        }
    }
}

// 1.12a: 0x6FAD8990
void __fastcall MISSILE_CltDo_BlessedHammerSpawner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    MISSILE_CltDo_UpdateAttachedOwnerPos(pMissile);

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!SKILLS_GetSkillTargetCoords(pOwner, &nTargetX, &nTargetY))
    {
        return;
    }

    const int32_t nCurCounter = MISSILE_GetTargetY(pMissile);
    const int32_t nMaxCounter = MISSILE_GetTargetX(pMissile);
    if (nCurCounter < nMaxCounter)
    {
        MISSILE_SetTargetY(pMissile, nCurCounter + 1);
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_TARGET_COORDS;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pOwner;
    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
    missileParams.nTargetX = nTargetX;
    missileParams.nTargetY = nTargetY;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
    if (pSub)
    {
        PATH_SetType(pSub->pDynamicPath, PATHTYPE_BLESSEDHAMMER);
        D2Common_10142(pSub->pDynamicPath, pSub, 0);
    }

    MISSILE_SetTargetY(pMissile, 0);
    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD8AD0
void __fastcall MISSILE_CltDo_PeriodicRandomSubmissileCollisionTrigger(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);

    int32_t nValidSubCount = 0;
    while (nValidSubCount < 3)
    {
        if (static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[nValidSubCount]) < 0
            || static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1 + nValidSubCount]) <= 0)
        {
            break;
        }
        ++nValidSubCount;
    }

    if (nValidSubCount <= 0)
    {
        MISSILE_ProcessCollisionHit(pMissile, nullptr, FALSE);
        return;
    }

    if (nRemainingFrames >= static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]))
    {
        SEED_InitLowSeed(&pMissile->pSeed, CLIENTS_GetUnitX(pMissile));
        const int32_t nSubIdx = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nValidSubCount));
        const int32_t nPeriod = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1 + nSubIdx]);

        if ((nRemainingFrames % nPeriod) == 0)
        {
            D2MissileStrc missileParams = {};
            missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
            missileParams.pOrigin = pMissile;
            missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[nSubIdx]);
            missileParams.nSkill = MISSILE_GetSkill(pMissile);
            missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

            D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
            if (pSub)
            {
                MISSILE_ProcessCollisionHit(pSub, nullptr, TRUE);
            }
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD8C50
void __fastcall MISSILE_CltDo_GuidedTrailingSubmissile(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.pOrigin = pMissile;
        missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        MISSILE_CreateClientMissile(&missileParams);
    }

    MISSILE_CltDo_GuidedArrow_BoneSpirit(pMissile);
}

// 1.12a: 0x6FAD8D00
void __fastcall MISSILE_CltDo_SymmetricDualWakeSubmissile(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        const int32_t nTargetX = MISSILE_GetTargetX(pMissile);
        const int32_t nTargetY = MISSILE_GetTargetY(pMissile);

        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET;
        missileParams.pOwner = pOwner;
        missileParams.pOrigin = pMissile;
        missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        missileParams.nTargetX = nTargetX;
        missileParams.nTargetY = nTargetY;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        MISSILE_CreateClientMissile(&missileParams);

        missileParams.nTargetX = -nTargetX;
        missileParams.nTargetY = -nTargetY;
        MISSILE_CreateClientMissile(&missileParams);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD8E10
void __fastcall MISSILE_CltDo_TimedClusterAndHideOwner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);

    const int16_t nSub0 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
    if (nSub0 >= 0 && nRemainingFrames == static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]))
    {
        const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
        const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_RANDOM_DIRECTION;
        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.nMissile = nSub0;
        missileParams.nX = nOriginX;
        missileParams.nY = nOriginY;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        MISSILE_CreateClientMissile(&missileParams);

        const int32_t nExtraCount = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2]);
        const int32_t nRadius = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[3]);
        if (nExtraCount > 0)
        {
            const int32_t nMinX = nOriginX - nRadius;
            const int32_t nMinY = nOriginY - nRadius;
            const int32_t nSpan = 2 * nRadius + 1;

            for (int32_t i = 0; i < nExtraCount; ++i)
            {
                missileParams.nX = nMinX + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan));
                missileParams.nY = nMinY + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan));
                MISSILE_CreateClientMissile(&missileParams);
            }
        }
    }

    const int16_t nSub1 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[1]);
    if (nSub1 >= 0 && nRemainingFrames == static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]))
    {
        D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
        if (pOwner)
        {
            pOwner->dwFlagEx |= UNITFLAGEX_NODRAW;

            D2MissileStrc missileParams = {};
            missileParams.dwFlags = CLT_MISSILE_INIT_RANDOM_DIRECTION;
            missileParams.pOwner = pOwner;
            missileParams.pOrigin = pMissile;
            missileParams.nMissile = nSub1;
            missileParams.nSkill = MISSILE_GetSkill(pMissile);
            missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

            D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
            if (pSub && static_cast<int16_t>(pMissilesTxtRecord->wProgSound) > 0)
            {
                SOUND_PlayUnitSound(static_cast<int16_t>(pMissilesTxtRecord->wProgSound), pSub, 0, 0, FALSE);
            }
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD8FF0
void __fastcall MISSILE_CltDo_Armageddon_HurricaneSpawner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
    if (!pRoom)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = MISSILE_GetSkillsTxt(nSkillId);
    if (!pSkillsTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
    const int32_t nRadius = SKILLS_EvaluateSkillFormula(CUNIT_GetOwnerUnit(pMissile), pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    int32_t nPeriod = SKILLS_EvaluateSkillFormula(CUNIT_GetOwnerUnit(pMissile), pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
    if (nPeriod < 1)
    {
        nPeriod = 1;
    }

    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);
    if ((nRemainingFrames % nPeriod) == 0)
    {
        SEED_InitLowSeed(&pMissile->pSeed, CLIENTS_GetUnitX(pMissile) + nRemainingFrames);

        const int32_t nHalfBox = nRadius - 1;
        const int32_t nSpan = 2 * nHalfBox;
        const int32_t nSpawnX = CLIENTS_GetUnitX(pMissile) + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nHalfBox;
        const int32_t nSpawnY = CLIENTS_GetUnitY(pMissile) + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nHalfBox;

        if (!COLLISION_CheckMask(pRoom, nSpawnX, nSpawnY, COLLIDE_WALL | COLLIDE_MISSILE_BARRIER | COLLIDE_MISSILE))
        {
            D2MissileStrc missileParams = {};
            missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_RANDOM_DIRECTION;
            missileParams.pOwner = pOwner;
            missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
            missileParams.nX = nSpawnX;
            missileParams.nY = nSpawnY;
            missileParams.nSkill = MISSILE_GetSkill(pMissile);
            missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

            D2UnitStrc* pSub0 = MISSILE_CreateClientMissile(&missileParams);
            if (pSub0)
            {
                pSub0->dwFlags |= UNITFLAG_ISDEAD;
            }

            const int16_t nSub1 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[1]);
            if (nSub1 >= 0)
            {
                missileParams.nMissile = nSub1;
                MISSILE_CreateClientMissile(&missileParams);
            }
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD91D0
void __fastcall MISSILE_CltDo_LoopedTrailSubmissile(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wCltSubMissile[0] == 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_LOOPS;
        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        missileParams.nX = CLIENTS_GetUnitX(pMissile);
        missileParams.nY = CLIENTS_GetUnitY(pMissile);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        missileParams.nLoops = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
        MISSILE_CreateClientMissile(&missileParams);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD92B0
void __fastcall MISSILE_CltDo_PerpendicularWallMaker(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        CUNIT_RemoveUnitFromHashTable(gpServerUnitHashTables[UNIT_TYPES_COUNT], 0x7F, static_cast<D2UnitGUID>(-1), UNIT_TYPES_COUNT);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0 || !pMissile->pDynamicPath)
    {
        CUNIT_RemoveUnitFromHashTable(gpServerUnitHashTables[pMissile->dwUnitType], pMissile->dwUnitId & 0x7F, pMissile->dwUnitId, pMissile->dwUnitType);
        return;
    }

    const int32_t nCurX = CLIENTS_GetUnitX(pMissile);
    const int32_t nCurY = CLIENTS_GetUnitY(pMissile);

    if (MISSILE_GetRemainingFrames(pMissile) < 2)
    {
        const int32_t nDeltaX = D2COMMON_10175_PathGetFirstPointX(pMissile->pDynamicPath) - nCurX;
        const int32_t nDeltaY = D2COMMON_10176_PathGetFirstPointY(pMissile->pDynamicPath) - nCurY;
        MISSILE_SetTargetX(pMissile, -nDeltaY);
        MISSILE_SetTargetY(pMissile, nDeltaX);
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS
                              | CLT_MISSILE_INIT_TARGET_OFFSET
                              | CLT_MISSILE_INIT_USE_LOOPS;
        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        missileParams.nX = nCurX;
        missileParams.nY = nCurY;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        missileParams.nLoops = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
        missileParams.nTargetX = MISSILE_GetTargetX(pMissile);
        missileParams.nTargetY = MISSILE_GetTargetY(pMissile);
        MISSILE_CreateClientMissile(&missileParams);

        missileParams.nTargetX = -MISSILE_GetTargetX(pMissile);
        missileParams.nTargetY = -MISSILE_GetTargetY(pMissile);
        MISSILE_CreateClientMissile(&missileParams);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD9420
void __fastcall MISSILE_CltDo_DelayedPeriodicScatterSpawner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);
    if (nRemainingFrames == static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]))
    {
        MISSILE_SetTargetY(pMissile, 1);
    }

    if (MISSILE_GetTargetY(pMissile) != 0)
    {
        int32_t nPeriod = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]);
        if (nPeriod < 1)
        {
            nPeriod = 1;
        }

        if ((nRemainingFrames % nPeriod) == 0)
        {
            const int32_t nRadius = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2]);
            const int32_t nSpan = 2 * nRadius + 1;
            const int32_t nOffsetX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nRadius;
            const int32_t nOffsetY = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nRadius;

            D2MissileStrc missileParams = {};
            missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS;
            missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
            missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
            missileParams.nX = CLIENTS_GetUnitX(pMissile);
            missileParams.nY = CLIENTS_GetUnitY(pMissile);
            missileParams.nTargetX = CLIENTS_GetUnitX(pMissile) + nOffsetX;
            missileParams.nTargetY = CLIENTS_GetUnitY(pMissile) + nOffsetY;
            missileParams.nSkill = MISSILE_GetSkill(pMissile);
            missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
            MISSILE_CreateClientMissile(&missileParams);
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD9570
static void __fastcall MISSILE_SpawnFallingIceOrMeteorInCircle(int32_t nMissileId, D2UnitStrc* pOwner, D2UnitStrc* pOrigin, int32_t nRadius)
{
    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(nMissileId);
    if (!pMissilesTxtRecord)
    {
        return;
    }

    if (!UNITS_GetRoom(pOrigin))
    {
        return;
    }

    const int32_t nMaxMissileId = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
    if (nMissileId < nMaxMissileId)
    {
        nMissileId += static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nMaxMissileId - nMissileId + 1));
        pMissilesTxtRecord = MISSILE_GetMissilesTxt(nMissileId);
        if (!pMissilesTxtRecord)
        {
            return;
        }
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = nMissileId;
    missileParams.nSkill = MISSILE_GetSkill(pOrigin);
    missileParams.nSkillLevel = MISSILE_GetLevel(pOrigin);

    const int32_t nRadiusSq = nRadius * nRadius;
    const int32_t nSpan = 2 * nRadius + 1;

    for (int32_t nAttempt = 1; nAttempt <= 4; ++nAttempt)
    {
        const int32_t nOffX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nSpan)) - nRadius;
        const int32_t nOffY = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nSpan)) - nRadius;
        if (nOffX * nOffX + nOffY * nOffY > nRadiusSq)
        {
            continue;
        }

        const int32_t nSpawnX = CLIENTS_GetUnitX(pOrigin) + nOffX;
        const int32_t nSpawnY = CLIENTS_GetUnitY(pOrigin) + nOffY;
        D2ActiveRoomStrc* pTargetRoom = DUNGEON_FindActRoomBySubtileCoords(UNITS_GetRoom(pOrigin), nSpawnX, nSpawnY);
        if (!pTargetRoom)
        {
            continue;
        }

        if (COLLISION_CheckAnyCollisionWithPattern(pTargetRoom, nSpawnX, nSpawnY, COLLISION_PATTERN_NONE, COLLIDE_WALL))
        {
            continue;
        }

        missileParams.nX = nSpawnX;
        missileParams.nY = nSpawnY;
        D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
        if (pSub)
        {
            PARTICLE_AllocMissileParticle(pSub);
            int32_t nStartHeight = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
            int32_t nFallSpeed = -static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]);
            if (nRadius == 6)
            {
                nStartHeight <<= 1;
                nFallSpeed <<= 1;
            }
            PARTICLE_InitVerticalRise(pSub, 0, nStartHeight, 0);
            MISSILE_SetParticleVelocity(pSub, 0, 0, nFallSpeed << 11);
            return;
        }
    }
}

// 1.12a: 0x6FAD9810
static void __fastcall MISSILE_SpawnRisingDebrisCluster(int32_t nScale, D2UnitStrc* pOrigin, D2UnitStrc* pOwner, int32_t nMissileId)
{
    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(nMissileId);
    if (!pMissilesTxtRecord)
    {
        return;
    }

    const int32_t nSpread = nScale * 7;
    const int32_t nCount = (nSpread > 8) ? 8 : nSpread;
    if (nCount <= 0)
    {
        return;
    }

    for (int32_t i = nCount; i != 0; --i)
    {
        D2MissileStrc missileParams = {};
        missileParams.pOwner = pOwner;
        missileParams.pOrigin = pOrigin;
        missileParams.nMissile = nMissileId;
        missileParams.nSkill = MISSILE_GetSkill(pOrigin);
        missileParams.nSkillLevel = MISSILE_GetLevel(pOrigin);

        D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
        if (pSub)
        {
            const int32_t nHalfSpread = nSpread / 2;
            const int32_t nPosX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nSpread)) - nHalfSpread;
            const int32_t nPosY = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nSpread)) - nHalfSpread;
            const int32_t nVelZ = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1])));

            PARTICLE_AllocMissileParticle(pSub);
            MISSILE_SetParticlePosition(pSub, nPosX << 11, nPosY << 11, static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]) << 11);
            MISSILE_SetParticleVelocity(pSub, 0, 0, nVelZ << 11);
            MISSILE_SetParticleAcceleration(pSub, 0, 0, static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2]) << 11);
        }
    }
}

// 1.12a: 0x6FAD9A40
void __fastcall MISSILE_CltDo_CardinalOwnerCrossSpawner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    if (MISSILE_GetRemainingFrames(pMissile) != static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]))
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    const int32_t nOwnerX = CLIENTS_GetUnitX(pOwner);
    const int32_t nOwnerY = CLIENTS_GetUnitY(pOwner);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_RANDOM_DIRECTION;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    for (int32_t i = 0; i < 4; ++i)
    {
        missileParams.nX = nOwnerX + gnCardinalOffsetsX_6FB86A08[i];
        missileParams.nY = nOwnerY + gnCardinalOffsetsY_6FB869F8[i];
        MISSILE_CreateClientMissile(&missileParams);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD9B60
void __fastcall MISSILE_CltDo_Tornado(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    if (static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) > 0 && D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.pOwner = pOwner;
        missileParams.pOrigin = pMissile;
        missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        MISSILE_CreateClientMissile(&missileParams);
    }

    int32_t nSteerPeriod = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
    if (nSteerPeriod < 1)
    {
        nSteerPeriod = 1;
    }

    if ((MISSILE_GetRemainingFrames(pMissile) % nSteerPeriod) == 0)
    {
        const int32_t nOldX = MISSILE_GetTargetX(pMissile);
        const int32_t nOldY = MISSILE_GetTargetY(pMissile);
        int32_t nNewX = nOldX;
        int32_t nNewY = nOldY;

        const uint32_t nChoice = static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) & 3;
        if (nChoice == 0)
        {
            nNewX = (3 * (nOldX - nOldY)) / 4;
            nNewY = (3 * (nOldX + nOldY)) / 4;
        }
        else if (nChoice == 2)
        {
            nNewX = (3 * (nOldX + nOldY)) / 4;
            nNewY = (3 * (nOldY - nOldX)) / 4;
        }

        D2COMMON_10170_PathSetTargetPos(pMissile->pDynamicPath, CLIENTS_GetUnitX(pMissile) + nNewX, CLIENTS_GetUnitY(pMissile) + nNewY);
        D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
        MISSILE_SetTargetX(pMissile, nNewX);
        MISSILE_SetTargetY(pMissile, nNewY);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD9CE0
static D2UnitStrc* __fastcall MISSILE_SpawnRandomDirectionSubmissile(D2UnitStrc* pOrigin, D2UnitStrc* pOwner, int32_t nMissileId, int32_t nMaxOffset)
{
    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pOrigin;
    missileParams.nMissile = nMissileId;
    missileParams.nSkill = MISSILE_GetSkill(pOrigin);
    missileParams.nSkillLevel = MISSILE_GetLevel(pOrigin);

    const int32_t nSpan = 2 * nMaxOffset + 1;
    int32_t nOffX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nSpan)) - nMaxOffset;
    const int32_t nOffY = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nSpan)) - nMaxOffset;

    if (nOffX == 0 && nOffY == 0)
    {
        nOffX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nMaxOffset - 1)) + 1;
    }

    missileParams.nTargetX = nOffX;
    missileParams.nTargetY = nOffY;

    D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
    if (pSub)
    {
        MISSILE_SetTargetX(pSub, nOffX);
        MISSILE_SetTargetY(pSub, nOffY);
    }

    return pSub;
}

// 1.12a: 0x6FAD9E00
void __fastcall MISSILE_CltDo_EruptionCenter(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    int32_t nPeriod = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2]);
    if (nPeriod < 1)
    {
        nPeriod = 1;
    }

    if ((MISSILE_GetRemainingFrames(pMissile) % nPeriod) != 0)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    const int32_t nTargetSpread = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
    const int32_t nTargetSpan = 2 * nTargetSpread + 1;
    const int32_t nTargetOffX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nTargetSpan)) - nTargetSpread;
    const int32_t nTargetOffY = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nTargetSpan)) - nTargetSpread;

    const int32_t nOriginSpread = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]);
    const int32_t nOriginSpan = 2 * nOriginSpread + 1;
    const int32_t nSpawnX = CLIENTS_GetUnitX(pMissile) + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nOriginSpan)) - nOriginSpread;
    const int32_t nSpawnY = CLIENTS_GetUnitY(pMissile) + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nOriginSpan)) - nOriginSpread;

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS;
    missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
    missileParams.nX = nSpawnX;
    missileParams.nY = nSpawnY;
    missileParams.nTargetX = nSpawnX + nTargetOffX;
    missileParams.nTargetY = nSpawnY + nTargetOffY;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    MISSILE_CreateClientMissile(&missileParams);

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FAD9F80
void __fastcall MISSILE_CltDo_Twister_FirestormCurve(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    if (static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) >= 0 && D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.pOwner = pOwner;
        missileParams.pOrigin = pMissile;
        missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        missileParams.pInitFunc = MISSILE_InitFunc_SyncLoopAnimFrame;
        missileParams.pInitArgs = reinterpret_cast<int32_t>(pMissile);

        D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
        if (pSub)
        {
            const int32_t nHeight = PARTICLE_GetHeightInPixels(pMissile);
            PARTICLE_InitVerticalRise(pSub, 0, nHeight, 0);
        }
    }

    int32_t nSteerPeriod = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
    if (nSteerPeriod < 1)
    {
        nSteerPeriod = 1;
    }

    if ((MISSILE_GetRemainingFrames(pMissile) % nSteerPeriod) == 0)
    {
        const int32_t nOldX = MISSILE_GetTargetX(pMissile);
        const int32_t nOldY = MISSILE_GetTargetY(pMissile);

        int32_t nRotX = 0;
        int32_t nRotY = 0;
        if ((SEED_RollRandomNumber(&pMissile->pSeed) & 1) != 0)
        {
            nRotX = nOldX - nOldY;
            nRotY = nOldX + nOldY;
        }
        else
        {
            nRotX = nOldX + nOldY;
            nRotY = nOldY - nOldX;
        }

        const int32_t nNewX = (3 * nRotX) / 4;
        const int32_t nNewY = (3 * nRotY) / 4;

        if (nNewX != 0 || nNewY != 0)
        {
            D2COMMON_10170_PathSetTargetPos(pMissile->pDynamicPath, CLIENTS_GetUnitX(pMissile) + nNewX, CLIENTS_GetUnitY(pMissile) + nNewY);
            D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
        }

        MISSILE_SetTargetX(pMissile, nNewX);
        MISSILE_SetTargetY(pMissile, nNewY);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADA120
void __fastcall MISSILE_CltDo_TowerChestSpawner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (pOwner)
    {
        int32_t nCountdown = MISSILE_GetTargetX(pMissile);
        if (nCountdown == 0)
        {
            D2MissileStrc missileParams = {};
            missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET;
            missileParams.pOwner = pOwner;
            missileParams.pOrigin = pOwner;
            missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
            missileParams.nSkill = MISSILE_GetSkill(pMissile);
            missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

            const int32_t nRadiusX = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
            int32_t nOffX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, 2 * nRadiusX + 1)) - nRadiusX;
            missileParams.nTargetX = nOffX;

            const int32_t nRadiusY = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
            const int32_t nOffY = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, 2 * nRadiusX + 1)) - nRadiusY;
            missileParams.nTargetY = nOffY;

            if (nOffX == 0 && nOffY == 0)
            {
                nOffX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nRadiusY)) + 1;
                missileParams.nTargetX = nOffX;
            }

            D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
            if (pSub)
            {
                MISSILE_SetTargetX(pSub, nOffX);
                MISSILE_SetTargetY(pSub, nOffY);
            }

            nCountdown = MISSILE_GetTowerChestSpawnInterval(pMissile);
        }

        MISSILE_SetTargetX(pMissile, nCountdown - 1);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADA270
void __fastcall MISSILE_CltDo_FrozenOrb(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    int32_t nPeriod = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
    if (nPeriod < 1)
    {
        nPeriod = 1;
    }

    if ((MISSILE_GetRemainingFrames(pMissile) % nPeriod) <= 0)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET;
        missileParams.pOwner = pOwner;
        missileParams.pOrigin = pMissile;
        missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        int32_t nDirIdx = MISSILE_GetTargetX(pMissile) % 64;
        if (nDirIdx < 0)
        {
            nDirIdx = -nDirIdx;
        }

        missileParams.nTargetX = gnFrozenOrbOffsetsX_6FB86B18[nDirIdx];
        missileParams.nTargetY = gnFrozenOrbOffsetsY_6FB86A18[nDirIdx];

        const int32_t nNextIdx = (nDirIdx + static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1])) % 64;
        MISSILE_SetTargetX(pMissile, nNextIdx);

        MISSILE_CreateClientMissile(&missileParams);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADA390
void __fastcall MISSILE_CltDo_AlignedWakeTrail(D2UnitStrc* pMissile)
{
    if (pMissile)
    {
        D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
        if (pMissilesTxtRecord
            && static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) >= 0
            && MISSILE_GetRemainingFrames(pMissile) >= static_cast<int32_t>(pMissilesTxtRecord->nInitSteps)
            && D2Common_10234(pMissile->pDynamicPath))
        {
            const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
            const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

            D2MissileStrc missileParams = {};
            missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
            missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
            missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
            missileParams.nX = nOriginX;
            missileParams.nY = nOriginY;
            missileParams.nSkill = MISSILE_GetSkill(pMissile);
            missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
            missileParams.nLoops = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);

            D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
            if (pSub)
            {
                PATH_SetDirection(pSub->pDynamicPath, PATH_GetDirection(pMissile->pDynamicPath));
                PATH_SetPrecisionX(pSub->pDynamicPath, UNITS_GetPrecisionX(pMissile));
                PATH_SetPrecisionY(pSub->pDynamicPath, UNITS_GetClientCoordX(pMissile));
                PARTICLE_InitVerticalRise(pSub, 0, PARTICLE_GetHeightInPixels(pMissile), 0);
            }
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADA4D0
void __fastcall MISSILE_CltDo_SpiderGooTrail(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0 || !CUNIT_GetOwnerUnit(pMissile))
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS;
        if (SEED_RollLimitedRandomNumber(&pMissile->pSeed, 4) != 0)
        {
            missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS | CLT_MISSILE_INIT_NO_LIGHT;
        }

        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        missileParams.nX = CLIENTS_GetUnitX(pMissile);
        missileParams.nY = CLIENTS_GetUnitY(pMissile);
        missileParams.nTargetX = CLIENTS_GetUnitX(pMissile);
        missileParams.nTargetY = CLIENTS_GetUnitY(pMissile);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        MISSILE_CreateClientMissile(&missileParams);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADA5F0
void __fastcall MISSILE_CltDo_LightningWallSeeker(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nInitialTargetX = MISSILE_GetTargetX(pMissile);
    if (nInitialTargetX <= -static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]))
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    if (nInitialTargetX <= 0)
    {
        MISSILE_SetTargetX(pMissile, nInitialTargetX - 1);
    }

    if (PATH_GetVelocity(pMissile->pDynamicPath) == 0)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pMissile->pDynamicPath);
    if (!pTarget)
    {
        pTarget = CUNIT_GetTargetUnit(pMissile);
    }

    int32_t nSteerPeriod = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2]);
    if (nSteerPeriod <= 0)
    {
        nSteerPeriod = 5;
    }

    if (pTarget
        && (MISSILE_GetCurrentFrame(pMissile) % nSteerPeriod) == 0
        && nInitialTargetX > 0
        && UNITS_GetDistanceToOtherUnit(pMissile, pTarget) < static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[3]))
    {
        const int32_t nCurX = CLIENTS_GetUnitX(pMissile);
        const int32_t nCurY = CLIENTS_GetUnitY(pMissile);
        const int32_t nTgtX = CLIENTS_GetUnitX(pTarget);
        const int32_t nTgtY = CLIENTS_GetUnitY(pTarget);

        int32_t nStepDist = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[4]);
        if (nStepDist < 1)
        {
            nStepDist = 1;
        }

        int32_t nStepX = 0;
        if (nTgtX > nCurX)
        {
            nStepX = nStepDist;
        }
        else if (nTgtX < nCurX)
        {
            nStepX = -nStepDist;
        }

        int32_t nStepY = 0;
        if (nTgtY > nCurY)
        {
            nStepY = nStepDist;
        }
        else if (nTgtY < nCurY)
        {
            nStepY = -nStepDist;
        }

        D2COMMON_10170_PathSetTargetPos(pMissile->pDynamicPath, CLIENTS_GetUnitX(pMissile) + nStepX, CLIENTS_GetUnitY(pMissile) + nStepY);
        D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
    }

    if (static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) > 0)
    {
        int32_t nTrailPeriod = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
        if (nTrailPeriod <= 1)
        {
            nTrailPeriod = 1;
        }

        if ((MISSILE_GetCurrentFrame(pMissile) % nTrailPeriod) == 0 && nInitialTargetX > 0)
        {
            D2MissileStrc missileParams = {};
            missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_VELOCITY;
            missileParams.pOwner = pOwner;
            missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
            missileParams.nX = CLIENTS_GetUnitX(pMissile);
            missileParams.nY = CLIENTS_GetUnitY(pMissile);
            missileParams.nVelocity = 0;
            missileParams.nSkill = MISSILE_GetSkill(pMissile);
            missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
            missileParams.nRange = MISSILE_GetTotalFrames(pMissile) - 1;

            D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
            if (pSub)
            {
                MISSILE_SetTargetX(pSub, 0);
            }
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADA820
static D2UnitStrc* __fastcall MISSILE_SpawnPeriodicSubmissileInBox(int32_t nPeriod, D2UnitStrc* pOrigin, int32_t nRadius, int32_t nMissileId)
{
    if (nPeriod <= 0 || (MISSILE_GetRemainingFrames(pOrigin) % nPeriod) != 0)
    {
        return nullptr;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pOrigin);
    if (!pOwner)
    {
        return nullptr;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pOrigin);
    if (!pRoom)
    {
        return nullptr;
    }

    SEED_InitLowSeed(&pOrigin->pSeed, MISSILE_GetRemainingFrames(pOrigin) + CLIENTS_GetUnitX(pOrigin));

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_TARGET_OFFSET | CLT_MISSILE_INIT_RANDOM_DIRECTION;
    missileParams.pOwner = pOwner;
    missileParams.nSkill = MISSILE_GetSkill(pOrigin);
    missileParams.nSkillLevel = MISSILE_GetLevel(pOrigin);
    missileParams.nMissile = nMissileId;

    const int32_t nHalfBox = nRadius - 1;
    const uint32_t nSpan = static_cast<uint32_t>(2 * nHalfBox);
    const int32_t nOffX = nHalfBox - static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, static_cast<int32_t>(nSpan)));
    const int32_t nOffY = nHalfBox - static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, static_cast<int32_t>(nSpan)));

    const int32_t nSpawnX = CLIENTS_GetUnitX(pOrigin) + nOffX;
    const int32_t nSpawnY = CLIENTS_GetUnitY(pOrigin) + nOffY;
    missileParams.nX = nSpawnX;
    missileParams.nY = nSpawnY;

    if (COLLISION_CheckMask(pRoom, nSpawnX, nSpawnY, COLLIDE_MISSILE_BARRIER))
    {
        return nullptr;
    }

    return MISSILE_CreateClientMissile(&missileParams);
}

// 1.12a: 0x6FADA930
void __fastcall MISSILE_CltDo_MeteorCenterLightAndFall(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = MISSILE_GetSkillsTxt(nSkillId);
    if (!pSkillsTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    int32_t nFallFrames = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
    if (nFallFrames < 1)
    {
        nFallFrames = 1;
    }

    int32_t nVelZ = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]);
    if (nVelZ < 1)
    {
        nVelZ = 1;
    }
    const int32_t nStartHeight = nVelZ * nFallFrames;

    int32_t nVelX = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2]);
    if (nVelX < 1)
    {
        nVelX = 1;
    }
    const int32_t nStartOffsetX = nVelX * nFallFrames;

    int32_t nMaxLightRadius = SKILLS_EvaluateSkillFormula(
        CUNIT_GetOwnerUnit(pMissile),
        pSkillsTxtRecord->dwCalc[0],
        nSkillId,
        MISSILE_GetLevel(pMissile));
    if (nMaxLightRadius <= 1)
    {
        nMaxLightRadius = 1;
    }
    else if (nMaxLightRadius >= 60)
    {
        nMaxLightRadius = 60;
    }

    const int32_t nLightRampStep = 60 / nMaxLightRadius;
    D2GfxLightStrc* pLight = pMissile->pLight;
    const int32_t nCurLightRadius = pLight ? LIGHTMAP_GetLightRadius(pLight) : 0;

    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);
    if (((nRemainingFrames + 1) % nLightRampStep) == 0 && nCurLightRadius < nMaxLightRadius && pLight)
    {
        LIGHTMAP_SetLightRadius(pLight, nCurLightRadius + 1);
    }

    if (nRemainingFrames == 0)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        missileParams.nX = CLIENTS_GetUnitX(pMissile);
        missileParams.nY = CLIENTS_GetUnitY(pMissile);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        D2UnitStrc* pSub0 = MISSILE_CreateClientMissile(&missileParams);
        if (pSub0)
        {
            PARTICLE_InitVerticalRise(pSub0, 0, nStartHeight, -nStartOffsetX);
            MISSILE_SetParticleVelocity(pSub0, nVelX << 11, 0, -(nVelZ << 11));
        }

        const int16_t nSub1 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[1]);
        if (nSub1 > 0)
        {
            missileParams.nMissile = nSub1;
            D2UnitStrc* pSub1 = MISSILE_CreateClientMissile(&missileParams);
            if (pSub1)
            {
                PARTICLE_InitVerticalRise(pSub1, 0, nStartHeight, -nStartOffsetX);
                MISSILE_SetParticleVelocity(pSub1, nVelX << 11, 0, -(nVelZ << 11));
            }
        }
    }

    if (nRemainingFrames == (nFallFrames - 2))
    {
        if (SOUND_GetFirstSoundNode(pMissile))
        {
            SOUND_StopUnitSound(pMissile, TRUE);
        }

        const int16_t nProgSound = static_cast<int16_t>(pMissilesTxtRecord->wProgSound);
        if (nProgSound > 0)
        {
            SOUND_PlayUnitSound(nProgSound, pMissile, 0, 0, FALSE);
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADABF0
void __fastcall MISSILE_CltDo_FireWallMakerSynced(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    if (MISSILE_GetRemainingFrames(pMissile) >= static_cast<int32_t>(pMissilesTxtRecord->nInitSteps)
        && D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
        missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
        missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        missileParams.nX = CLIENTS_GetUnitX(pMissile);
        missileParams.nY = CLIENTS_GetUnitY(pMissile);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        const int32_t nLoops = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
        missileParams.nLoops = (nLoops < 1) ? 1 : nLoops;
        missileParams.pInitFunc = MISSILE_InitFunc_SyncLoopAnimFrame;
        missileParams.pInitArgs = reinterpret_cast<int32_t>(pMissile);
        MISSILE_CreateClientMissile(&missileParams);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADACE0
void __fastcall MISSILE_CltDo_MoltenBoulderTrail(D2UnitStrc* pMissile)
{
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
        if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
        {
            MISSILE_DeleteClientMissile(pMissile);
            return;
        }

        const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
        const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS;
        missileParams.pOwner = pOwner;
        missileParams.nX = nOriginX;
        missileParams.nY = nOriginY;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        const int32_t nBaseSubId = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        missileParams.nMissile = nBaseSubId;

        D2MissilesTxt* pSubMissilesTxt = MISSILE_GetMissilesTxt(nBaseSubId);
        if (!pSubMissilesTxt)
        {
            MISSILE_DeleteClientMissile(pMissile);
            return;
        }

        if (pSubMissilesTxt->wRange != 0)
        {
            missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_TARGET_COORDS | CLT_MISSILE_INIT_USE_RANGE;
            missileParams.nRange = static_cast<int16_t>(pSubMissilesTxt->wRange);
        }

        if (static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[1]) >= 0)
        {
            if (static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[2]) >= 0)
            {
                const int32_t nRoll = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, 3));
                if (nRoll == 0)
                {
                    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[1]);
                }
                else if (nRoll == 1)
                {
                    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[2]);
                }
            }
            else if (SEED_RollLimitedRandomNumber(&pMissile->pSeed, 2) == 0)
            {
                missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[1]);
            }
        }

        const int32_t nLightChance = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
        if (nLightChance == 0 || SEED_RollLimitedRandomNumber(&pMissile->pSeed, nLightChance) != 0)
        {
            missileParams.dwFlags |= CLT_MISSILE_INIT_NO_LIGHT;
        }

        missileParams.nTargetX = CLIENTS_GetUnitX(pMissile);
        missileParams.nTargetY = CLIENTS_GetUnitY(pMissile);

        D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
        if (pSub)
        {
            const int32_t nPrecX = UNITS_GetPrecisionX(pMissile);
            const int32_t nPrecY = UNITS_GetClientCoordX(pMissile);
            PATH_SetPrecisionX(pSub->pDynamicPath, nPrecX);
            PATH_SetPrecisionY(pSub->pDynamicPath, nPrecY);
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADAEA0
static void __fastcall MISSILE_SpawnQuadrantScatterSubmissiles(D2UnitStrc* pOrigin, uint32_t nSkipRollMax, int32_t nCount, int32_t nHalfBox, int32_t nMissileId)
{
    if (!MISSILE_GetMissilesTxt(nMissileId))
    {
        return;
    }

    if (MISSILE_GetCurrentFrame(pOrigin) != 0)
    {
        if (SEED_RollLimitedRandomNumber(&pOrigin->pSeed, static_cast<int32_t>(nSkipRollMax)) > 0)
        {
            return;
        }
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_TARGET_COORDS;
    missileParams.pOwner = CUNIT_GetOwnerUnit(pOrigin);
    missileParams.pOrigin = pOrigin;
    missileParams.nSkill = MISSILE_GetSkill(pOrigin);
    missileParams.nSkillLevel = MISSILE_GetLevel(pOrigin);
    missileParams.nMissile = nMissileId;

    const int32_t nOriginX = CLIENTS_GetUnitX(pOrigin);
    const int32_t nOriginY = CLIENTS_GetUnitY(pOrigin);

    if (nCount <= 0)
    {
        return;
    }

    const int32_t nSpan = 2 * nHalfBox;
    for (int32_t i = nCount; i != 0; --i)
    {
        const int32_t nRawX = (nSpan > 0)
            ? (static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nSpan)) - nHalfBox)
            : -nHalfBox;
        const int32_t nRawY = (nSpan > 0)
            ? (static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nSpan)) - nHalfBox)
            : -nHalfBox;

        const int32_t nOffX = nRawX + ((nRawX >= 0) ? nHalfBox : -nHalfBox);
        const int32_t nOffY = nRawY + ((nRawY >= 0) ? nHalfBox : -nHalfBox);

        missileParams.nTargetX = nOriginX + nOffX;
        missileParams.nTargetY = nOriginY + nOffY;
        MISSILE_CreateClientMissile(&missileParams);
    }
}

// 1.12a: 0x6FADB080
static void __fastcall MISSILE_SpawnStationaryStepSubmissile(D2UnitStrc* pOrigin, int32_t nRangeOverride, int32_t nMissileId, int32_t nUseLoopsFlag)
{
    if (!D2Common_10234(pOrigin->pDynamicPath))
    {
        return;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_VELOCITY;
    if (nRangeOverride > 0)
    {
        missileParams.nRange = nRangeOverride;
        missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_VELOCITY | CLT_MISSILE_INIT_USE_RANGE;
    }
    if (nUseLoopsFlag > 0)
    {
        missileParams.dwFlags |= CLT_MISSILE_INIT_USE_LOOPS;
    }

    missileParams.pOwner = CUNIT_GetOwnerUnit(pOrigin);
    missileParams.pOrigin = pOrigin;
    missileParams.nMissile = nMissileId;
    missileParams.nX = CLIENTS_GetUnitX(pOrigin);
    missileParams.nY = CLIENTS_GetUnitY(pOrigin);
    missileParams.nSkill = MISSILE_GetSkill(pOrigin);

    const int32_t nSkillLevel = MISSILE_GetLevel(pOrigin);
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nLoops = 2 * nSkillLevel - 2;

    MISSILE_CreateClientMissile(&missileParams);
}

// 1.12a: 0x6FADB170
static void __stdcall MISSILE_SpawnRadialRaytracedField(
    D2UnitStrc* pOrigin,
    const int32_t* pMissileIdList,
    int32_t nMissileCount,
    int32_t nRadius,
    int32_t nBaseRange,
    int32_t nChancePct)
{
    if (!UNITS_GetRoom(pOrigin))
    {
        return;
    }

    D2SeedStrc* pSeed = &pOrigin->pSeed;
    if (!pSeed)
    {
        return;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = (nBaseRange != 0)
        ? (CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_USE_RANGE)
        : CLT_MISSILE_INIT_USE_COORDS;
    missileParams.pOwner = CUNIT_GetOwnerUnit(pOrigin);
    missileParams.nSkill = MISSILE_GetSkill(pOrigin);
    missileParams.nSkillLevel = MISSILE_GetLevel(pOrigin);

    const int32_t nOriginX = CLIENTS_GetUnitX(pOrigin);
    const int32_t nOriginY = CLIENTS_GetUnitY(pOrigin);
    const int32_t nRadiusSq = nRadius * nRadius;

    for (int32_t dx = -nRadius; dx <= nRadius; ++dx)
    {
        const int32_t nSpawnX = nOriginX + dx;
        for (int32_t dy = -nRadius; dy <= nRadius; ++dy)
        {
            const int32_t nSpawnY = nOriginY + dy;
            D2ActiveRoomStrc* pOriginRoom = UNITS_GetRoom(pOrigin);
            if (!pOriginRoom)
            {
                continue;
            }

            if (dx * dx + dy * dy > nRadiusSq)
            {
                continue;
            }

            // 1.12a 0x6FADB2C3..0x6FADB2EA: raytraces from (nSpawnX, nSpawnY) to (nSpawnX + dx, nSpawnY + dy)
            D2CoordStrc beginCoord = { nSpawnX, nSpawnY };
            D2CoordStrc endCoord = { nSpawnX + dx, nSpawnY + dy };
            if (COLLISION_RayTrace(pOriginRoom, &beginCoord, &endCoord, COLLIDE_MISSILE_BARRIER))
            {
                continue;
            }

            D2ActiveRoomStrc* pTargetRoom = DUNGEON_FindActRoomBySubtileCoords(UNITS_GetRoom(pOrigin), nSpawnX, nSpawnY);
            if (!pTargetRoom || DUNGEON_IsRoomInTown(pTargetRoom))
            {
                continue;
            }

            if (nBaseRange != 0)
            {
                const int32_t nJitter = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(pSeed)) % 25);
                missileParams.nRange = nBaseRange + nJitter - 12;
            }

            missileParams.nX = nSpawnX;
            missileParams.nY = nSpawnY;

            const int32_t nPickIdx = static_cast<int32_t>(SEED_RollLimitedRandomNumber(pSeed, nMissileCount));
            missileParams.nMissile = pMissileIdList[nPickIdx];

            if (static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(pSeed)) % 100) < nChancePct)
            {
                MISSILE_CreateClientMissile(&missileParams);
            }
        }
    }
}

// 1.12a: 0x6FADB400
D2UnitStrc* __fastcall MISSILE_SpawnLobMissileToSkillTarget(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pOrigin)
    {
        return nullptr;
    }

    D2UnitStrc* pOwner = pOrigin;
    if (pOrigin->dwUnitType != UNIT_PLAYER && pOrigin->dwUnitType != UNIT_MONSTER)
    {
        pOwner = CUNIT_GetOwnerUnit(pOrigin);
        if (!pOwner)
        {
            return nullptr;
        }
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!SKILLS_GetSkillTargetCoords(pOrigin, &nTargetX, &nTargetY))
    {
        return nullptr;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_TARGET_COORDS | CLT_MISSILE_INIT_DISTANCE_FRAMES;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pOrigin;
    missileParams.nMissile = nMissileId;
    missileParams.nTargetX = nTargetX;
    missileParams.nTargetY = nTargetY;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    return MISSILE_CreateClientMissile(&missileParams);
}

// 1.12a: 0x6FADB4B0
D2UnitStrc* __fastcall MISSILE_SpawnParabolicLobMissile(
    D2UnitStrc* pOrigin,
    int32_t nMissileId,
    int32_t nOffsetX,
    int32_t nOffsetY,
    int32_t nTargetX,
    int32_t nTargetY,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nVelocity)
{
    if (!pOrigin)
    {
        return nullptr;
    }

    D2UnitStrc* pOwner = pOrigin;
    if (pOrigin->dwUnitType != UNIT_PLAYER && pOrigin->dwUnitType != UNIT_MONSTER)
    {
        pOwner = CUNIT_GetOwnerUnit(pOrigin);
        if (!pOwner)
        {
            return nullptr;
        }
    }

    if (nTargetX == 0 || nTargetY == 0)
    {
        if (!SKILLS_GetSkillTargetCoords(pOrigin, &nTargetX, &nTargetY))
        {
            return nullptr;
        }
    }

    if (nTargetX == 0 || nTargetY == 0)
    {
        return nullptr;
    }

    if (nVelocity == 0)
    {
        nVelocity = 30;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS
                          | CLT_MISSILE_INIT_USE_TARGET_COORDS
                          | CLT_MISSILE_INIT_LOB
                          | CLT_MISSILE_INIT_DISTANCE_FRAMES;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = nMissileId;
    missileParams.nX = CLIENTS_GetUnitX(pOrigin) + nOffsetX;
    missileParams.nY = CLIENTS_GetUnitY(pOrigin) + nOffsetY;
    missileParams.nTargetX = nTargetX;
    missileParams.nTargetY = nTargetY;
    missileParams.nGfxArg = nVelocity;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    return MISSILE_CreateClientMissile(&missileParams);
}

// 1.12a: 0x6FADB5E0
int32_t __fastcall MISSILE_CltHit_OuterRingPoisonBurst(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (pMissile)
    {
        D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
        if (pMissilesTxtRecord && static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) >= 0)
        {
            int32_t nInnerStep = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
            if (nInnerStep < 1)
            {
                nInnerStep = 1;
            }
            MISSILE_SpawnDualRingPoisonCloud(pMissile, static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]), 0, nInnerStep, 2);
        }
    }

    return 1;
}

// 1.12a: 0x6FADB640
void __fastcall MISSILE_CltDo_AncientsDeathControl(D2UnitStrc* pMissile)
{
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);
    const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
    const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

    if (nRemainingFrames < 110)
    {
        int32_t nSparkCount = 0;
        if (nRemainingFrames == 0)
        {
            MISSILE_CreateClientMissileAtCoords(
                pOwner,
                0x214, // 532
                nOriginX,
                nOriginY,
                MISSILE_GetSkill(pMissile),
                MISSILE_GetLevel(pMissile));
            nSparkCount = 8;
        }
        else if (nRemainingFrames < 50)
        {
            nSparkCount = 8;
        }
        else
        {
            nSparkCount = (nRemainingFrames < 80) ? 5 : 1;
        }

        for (int32_t i = nSparkCount; i > 0; --i)
        {
            const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
            const int32_t nSkillId = MISSILE_GetSkill(pMissile);
            if (!pOwner)
            {
                continue;
            }

            D2MissileStrc missileParams = {};
            missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
            missileParams.pOwner = pOwner;
            missileParams.nMissile = 0x255; // 597
            missileParams.nX = nOriginX + 2;
            missileParams.nY = nOriginY + 2;
            missileParams.nSkill = nSkillId;
            missileParams.nSkillLevel = nSkillLevel;

            D2UnitStrc* pSpark = MISSILE_CreateClientMissile(&missileParams);
            if (pSpark)
            {
                const int32_t nPosX = 50 - static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 100);
                const int32_t nPosY = 50 - static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 100);
                const int32_t nVelZ = (static_cast<int32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) & 7) + 2;

                MISSILE_SetParticlePosition(pSpark, nPosX << 11, nPosY << 11, 0);
                MISSILE_SetParticleVelocity(pSpark, 0, 0, nVelZ << 11);
            }
        }
    }

    if (nRemainingFrames == 5)
    {
        gdwScreenShakeStartTick = GetTickCount();
        gnScreenShakeFadeInMs = 600;
        gnScreenShakeHoldMs = 4000;
        gnScreenShakeFadeOutMs = 3000;
        gnScreenShakeAmplitude = 3;
        if ((nRemainingFrames % 7) == 0)
        {
            MISSILE_SpawnAncientsMarkerPair(pMissile, pOwner, nOriginX, nOriginY);
        }
    }
    else if (nRemainingFrames < 200 && (nRemainingFrames % 7) == 0)
    {
        MISSILE_SpawnAncientsMarkerPair(pMissile, pOwner, nOriginX, nOriginY);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADB8D0
void __fastcall MISSILE_CltDo_WorldstoneQuakeSpawner(D2UnitStrc* pMissile)
{
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pOwner));
    if (nLevelId < LEVEL_THRONEOFDESTRUCTION || nLevelId > LEVEL_THEWORLDSTONECHAMBER)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2Common_10235_PATH_UpdateRiderPath(pMissile, pOwner);
    UNITS_UpdateFrame(pMissile);

    if (MISSILE_GetTargetX(pMissile) == 0 || (SEED_RollRandomNumber(&pMissile->pSeed) & 0x3F) == 0)
    {
        gdwScreenShakeStartTick = GetTickCount();
        gnScreenShakeFadeInMs = 2000;
        gnScreenShakeHoldMs = 3000;
        gnScreenShakeFadeOutMs = 2000;
        gnScreenShakeAmplitude = 6;
        MISSILE_SetTargetX(pMissile, 1);
    }

    if ((static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 10) == 0)
    {
        const int32_t nOffX = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 60) - 30;
        const int32_t nOffY = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 60) - 30;
        const int32_t nSpawnX = CLIENTS_GetUnitX(pMissile) + nOffX;
        const int32_t nSpawnY = CLIENTS_GetUnitY(pMissile) + nOffY;
        MISSILE_SpawnFallingRubble(pMissile, nSpawnX, nSpawnY);
    }
}

// 1.12a: 0x6FADBA20
void __fastcall MISSILE_CltDo_ShenkDeathControl(D2UnitStrc* pMissile)
{
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    const int32_t nCurrentFrame = MISSILE_GetCurrentFrame(pMissile);

    int32_t nStageIdx = 0;
    while (nStageIdx < 4 && nCurrentFrame <= gSpawnerStages_6FB861DC[nStageIdx].nThresholdFrame)
    {
        ++nStageIdx;
    }

    if ((nCurrentFrame % gSpawnerStages_6FB861DC[nStageIdx].nPeriodA) == 0
        && gSpawnerStages_6FB861DC[nStageIdx].nPeriodB != 0)
    {
        (void)SEED_RollRandomNumber(&pMissile->pSeed);

        D2MissileStrc missileParams = {};
        missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET;
        missileParams.pOwner = pOwner;
        missileParams.pOrigin = pMissile;
        missileParams.nMissile = 0x272; // 626
        missileParams.nTargetX = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 40) - 20;
        missileParams.nTargetY = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 40) - 20;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
        if (pSub)
        {
            MISSILE_SetTargetX(pSub, pMissile ? static_cast<int32_t>(pMissile->dwUnitId) : -1);
        }
    }

    MISSILE_SetTargetX(pMissile, (nCurrentFrame < 210) ? 2 : 1);

    switch (nCurrentFrame)
    {
    case 450:
        MISSILE_CreateClientMissileAtCoords(
            pOwner,
            0x277, // 631
            CLIENTS_GetUnitX(pMissile) + 1,
            CLIENTS_GetUnitY(pMissile) + 1,
            MISSILE_GetSkill(pMissile),
            MISSILE_GetLevel(pMissile));
        break;
    case 425:
        MISSILE_CreateClientMissileAtCoords(
            pOwner,
            0x278, // 632
            CLIENTS_GetUnitX(pMissile),
            CLIENTS_GetUnitY(pMissile),
            MISSILE_GetSkill(pMissile),
            MISSILE_GetLevel(pMissile));
        break;
    case 375:
        MISSILE_CreateClientMissileAtCoords(
            pOwner,
            0x279, // 633
            CLIENTS_GetUnitX(pMissile),
            CLIENTS_GetUnitY(pMissile),
            MISSILE_GetSkill(pMissile),
            MISSILE_GetLevel(pMissile));
        break;
    case 325:
        MISSILE_CreateClientMissileAtCoords(
            pOwner,
            0x27A, // 634
            CLIENTS_GetUnitX(pMissile),
            CLIENTS_GetUnitY(pMissile),
            MISSILE_GetSkill(pMissile),
            MISSILE_GetLevel(pMissile));
        break;
    default:
        break;
    }

    if (MISSILE_GetTargetY(pMissile) != 0)
    {
        D2UnitStrc* pMonster = UNITFINDS_FindUnitInNeighboredRooms(UNITS_GetRoom(pMissile), UNIT_MONSTER, 0x209); // 521
        if (pMonster)
        {
            const int32_t nOffX = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 10) - 5;
            const int32_t nOffY = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 10) - 5;
            MISSILE_SpawnFallingRubble(pMissile, CLIENTS_GetUnitX(pMonster) + nOffX, CLIENTS_GetUnitY(pMonster) + nOffY);
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADBED0
void __fastcall MISSILE_CltDo_WorldstoneDestructionControl(D2UnitStrc* pMissile)
{
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);
    if (!pOwner)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    MISSILE_SpawnWorldstoneShardDebris(pMissile, pOwner);
    MISSILE_SpawnWorldstoneShardDebris(pMissile, pOwner);

    if (nRemainingFrames > 60 && (nRemainingFrames % 20) == 0)
    {
        MISSILE_SpawnTowerChestEruptionBolt(pMissile, pOwner);
    }

    if (nRemainingFrames == 25)
    {
        MISSILE_CreateClientMissileAtCoords(
            pOwner,
            0x26A, // 618
            CLIENTS_GetUnitX(pMissile) + 2,
            CLIENTS_GetUnitY(pMissile) + 2,
            MISSILE_GetSkill(pMissile),
            MISSILE_GetLevel(pMissile));

        MISSILE_CreateClientMissileAtCoords(
            pOwner,
            0x269, // 617
            CLIENTS_GetUnitX(pMissile) + 2,
            CLIENTS_GetUnitY(pMissile) + 2,
            MISSILE_GetSkill(pMissile),
            MISSILE_GetLevel(pMissile));
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADC030
void __fastcall MISSILE_CltDo_ShenkCatapultMeteorControl(D2UnitStrc* pMissile)
{
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    int32_t nCountdown = MISSILE_GetTargetX(pMissile);
    if (!pOwner)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    if (nCountdown == 0)
    {
        const int32_t nMeteorId = ((SEED_RollRandomNumber(&pMissile->pSeed) & 1) != 0) ? 0x1A0 : 0x1A6; // 416 or 422
        MISSILE_SpawnOwnerDirectedMeteor(pMissile, pOwner, nMeteorId);
        nCountdown = MISSILE_GetAncientsControlSpawnDelay(MISSILE_GetRemainingFrames(pMissile));
    }

    MISSILE_SetTargetX(pMissile, nCountdown - 1);
    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADC0B0
void __fastcall MISSILE_CltDo_HephastoHellforgeDebrisControl(D2UnitStrc* pMissile)
{
    const int32_t nCurrentFrame = MISSILE_GetCurrentFrame(pMissile);
    D2MissilesTxt* pRefMissilesTxt = MISSILE_GetMissilesTxt(0x17B); // 379

    MISSILE_SpawnRandomTargetSubmissileInBox(pMissile, 10, 0x17C); // 380

    const int32_t nRefRange = static_cast<int16_t>(pRefMissilesTxt->wRange);
    if (nCurrentFrame < (nRefRange - static_cast<int32_t>(pRefMissilesTxt->dwParam[0]))
        && nCurrentFrame > (nRefRange - static_cast<int32_t>(pRefMissilesTxt->dwParam[1])))
    {
        D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
        if (pOwner)
        {
            const int32_t nTargetX = CLIENTS_GetUnitX(pOwner) + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, 10)) - 5;

            D2MissileStrc missileParams = {};
            missileParams.dwFlags = CLT_MISSILE_INIT_USE_TARGET_COORDS;
            missileParams.pOwner = pOwner;
            missileParams.pOrigin = pMissile;
            missileParams.nMissile = 0x17E; // 382
            missileParams.nTargetX = nTargetX;
            missileParams.nTargetY = CLIENTS_GetUnitY(pOwner) + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, 10)) - 5;
            missileParams.nSkill = MISSILE_GetSkill(pMissile);
            missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

            D2UnitStrc* pSub = MISSILE_CreateClientMissile(&missileParams);
            if (pSub)
            {
                const int32_t nAccZ = -3 - static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, 3));
                MISSILE_SetParticleAcceleration(pSub, 0, 0, nAccZ << 11);
                PARTICLE_InitLobTrajectory(pSub, 15, 0);
                if (pSub->pGfxData && pSub->pGfxData->pParticle)
                {
                    pSub->pGfxData->pParticle->dwFlags &= ~0x02u;
                }
            }
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADC210
void __fastcall MISSILE_CltDo_HellforgeSoulBurstTrigger(D2UnitStrc* pMissile)
{
    const int32_t nObjectGUID = MISSILE_GetTargetX(pMissile);
    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);

    D2UnitStrc* pObject = gpClientUnitHashTables[UNIT_OBJECT][nObjectGUID & 0x7F];
    while (pObject && static_cast<int32_t>(pObject->dwUnitId) != nObjectGUID)
    {
        pObject = pObject->pListNext;
    }

    if (!pObject)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    D2_ASSERT(pObject->dwUnitType == UNIT_OBJECT);

    if (nRemainingFrames == 10)
    {
        MISSILE_SpawnHellforgeSoulBurst(pMissile, nObjectGUID);
        gdwScreenShakeStartTick = GetTickCount();
        gnScreenShakeFadeInMs = 80;
        gnScreenShakeHoldMs = 80;
        gnScreenShakeFadeOutMs = 400;
        gnScreenShakeAmplitude = 4;
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADC2B0
void __fastcall MISSILE_CltDo_HellforgeAscendingSpiritSpawner(D2UnitStrc* pMissile)
{
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    int32_t nDelay = MISSILE_GetTargetY(pMissile);
    if (nDelay == 0)
    {
        MISSILE_SpawnAscendingSpiritToObject(pMissile, MISSILE_GetTargetX(pMissile), pOwner);
        nDelay = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed)) % 35) + 10;
    }

    MISSILE_SetTargetY(pMissile, nDelay - 1);
    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADC320
int32_t __fastcall MISSILE_CltHit_MoltenBoulderImpact(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    if (pTarget)
    {
        if (pTarget->dwUnitType != UNIT_MONSTER || !MONSTERS_CheckMonStats2Flag(pTarget->dwClassId, 11))
        {
            return 0;
        }
    }

    const int16_t nSub0 = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);
    if (nSub0 >= 0)
    {
        MISSILE_CreateClientMissileAtCoords(
            CUNIT_GetOwnerUnit(pMissile),
            nSub0,
            CLIENTS_GetUnitX(pMissile),
            CLIENTS_GetUnitY(pMissile),
            MISSILE_GetSkill(pMissile),
            MISSILE_GetLevel(pMissile));
    }

    MISSILE_SpawnCraterDebrisAndRing(
        pMissile,
        static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]),
        static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]),
        static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[1]),
        static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[2]));

    return 1;
}

// 1.12a: 0x6FADC3D0
int32_t __fastcall MISSILE_CltHit_ChargedBoltFanOnHit(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord
        || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0
        || static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]) <= 0)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_TARGET_OFFSET;
    missileParams.pOwner = CUNIT_GetOwnerUnit(pMissile);
    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    if (pMissilesTxtRecord->dwCltHitPar[1] != 0)
    {
        missileParams.pInitFunc = SKILLS_InitChargedBoltPath;
    }

    SKILLS_SpawnChargedBoltFan(&missileParams, static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]));
    return 1;
}

// 1.12a: 0x6FADC4A0
int32_t __fastcall MISSILE_CltHit_VolcanoCraterBurst(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (pMissile)
    {
        D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
        if (pMissilesTxtRecord)
        {
            MISSILE_SpawnCraterDebrisAndRing(
                pMissile,
                static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]),
                static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]),
                static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]),
                static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[1]));
        }
    }

    return 1;
}

// 1.12a: 0x6FADC4F0
int32_t __fastcall MISSILE_CltHit_ChargedStrikeBolts(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wHitSubMissile[0]) < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS | CLT_MISSILE_INIT_TARGET_OFFSET;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = static_cast<int16_t>(pMissilesTxtRecord->wHitSubMissile[0]);
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.pInitFunc = MISSILE_InitFunc_ChargedBolt;

    int32_t nBoltCount = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0])
        + (static_cast<int32_t>(MISSILE_GetLevel(pMissile)) - 1) * static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]);
    if (nBoltCount <= 0)
    {
        const int32_t nSkillId = MISSILE_GetSkill(pMissile);
        const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
        D2SkillsTxt* pSkillsTxtRecord = MISSILE_GetSkillsTxt(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 1;
        }

        nBoltCount = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
        if (nBoltCount < 1)
        {
            nBoltCount = 1;
        }
    }

    SKILLS_SpawnChargedBoltFan(&missileParams, nBoltCount);
    return 1;
}

// 1.12a: 0x6FADC620
int32_t __fastcall MISSILE_CltHit_CatapultPoisonCloudField(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (pTarget)
    {
        return 0;
    }

    const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);
    const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
    if (!COLLISION_CheckMaskWithSize(UNITS_GetRoom(pMissile), nOriginX, nOriginY, COLLISION_UNIT_SIZE_BIG, COLLIDE_WALL))
    {
        D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
        if (pMissilesTxtRecord)
        {
            MISSILE_SpawnRadialRaytracedField(
                pMissile,
                gnShenkExplosionMissiles_6FBA676C,
                3,
                static_cast<int32_t>(pMissilesTxtRecord->dwParam[0]),
                25,
                75);
        }
    }

    return 1;
}

// 1.12a: 0x6FADC6E0
int32_t __fastcall MISSILE_CltHit_PantherPotGreen(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (pOwner)
    {
        D2UnitStrc* pOrigin = pTarget ? pTarget : pMissile;
        int32_t nStep = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
        if (nStep < 1)
        {
            nStep = 1;
        }

        MISSILE_SpawnPantherPotGreenRing(
            pOrigin,
            static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]),
            pOwner,
            MISSILE_GetSkill(pMissile),
            MISSILE_GetLevel(pMissile),
            nStep);
    }

    return 1;
}

// 1.12a: 0x6FADC770
int32_t __fastcall MISSILE_CltHit_LightningFury(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wHitSubMissile[0]) < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = MISSILE_GetSkillsTxt(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    const int32_t nTargetUnitType = MISSILE_GetTargetX(pMissile);
    const int32_t nTargetUnitGUID = MISSILE_GetTargetY(pMissile);
    D2UnitStrc* pHitUnit = CUNIT_FindUnitInHashTable(
        gpClientUnitHashTables[nTargetUnitType],
        static_cast<D2UnitGUID>(nTargetUnitGUID),
        nTargetUnitType);
    if (!pHitUnit)
    {
        return 0;
    }

    const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
    const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

    int32_t nRadius = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
    if (nRadius <= 0)
    {
        nRadius = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
        if (nRadius < 1)
        {
            nRadius = 1;
        }
    }

    int32_t nMaxTargets = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]);
    if (nMaxTargets <= 0)
    {
        nMaxTargets = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
    }

    // 1.12a 0x6FADC8AF..0x6FADC8B6: `test ebx, ebx; je ...; mov ebx, 0A683h`
    const uint32_t dwAuraFilter = (pSkillsTxtRecord->dwAuraFilter != 0) ? 0xA683u : 0u;

    D2MissileRadialCallbackArgsStrc callbackArgs = {};
    callbackArgs.pOwner = pOwner;
    callbackArgs.pOrigin = pMissile;
    callbackArgs.nSkillId = nSkillId;
    callbackArgs.nSkillLevel = nSkillLevel;
    callbackArgs.nMaxTargets = nMaxTargets;
    callbackArgs.nSubMissileId = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);

    SKILLS_ForEachUnitInRadius(
        pOwner,
        nOriginX,
        nOriginY,
        nRadius,
        dwAuraFilter,
        MISSILE_LightningFury_AuraCallback,
        &callbackArgs,
        FALSE);

    return 1;
}

// 1.12a: 0x6FADC920
int32_t __fastcall MISSILE_CltHit_FistOfTheHeavens(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = MISSILE_GetSkillsTxt(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (pOwner)
    {
        const int32_t nOriginX = CLIENTS_GetUnitX(pMissile);
        const int32_t nOriginY = CLIENTS_GetUnitY(pMissile);

        int32_t nRadius = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
        if (nRadius <= 0)
        {
            nRadius = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
            if (nRadius < 1)
            {
                nRadius = 1;
            }
        }

        int32_t nMaxTargets = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]);
        if (nMaxTargets <= 0)
        {
            nMaxTargets = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
            if (nMaxTargets < 1)
            {
                nMaxTargets = 1;
            }
        }

        uint32_t dwAuraFilter = pSkillsTxtRecord->dwAuraFilter;
        if (dwAuraFilter == 0)
        {
            dwAuraFilter = 0xA783u;
        }

        D2MissileRadialCallbackArgsStrc callbackArgs = {};
        callbackArgs.pOwner = pOwner;
        callbackArgs.pOrigin = pMissile;
        callbackArgs.nSkillId = nSkillId;
        callbackArgs.nSkillLevel = nSkillLevel;
        callbackArgs.nMaxTargets = nMaxTargets;
        callbackArgs.nSubMissileId = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);

        SKILLS_ForEachUnitInRadius(
            pOwner,
            nOriginX,
            nOriginY,
            nRadius,
            dwAuraFilter,
            MISSILE_FistOfTheHeavens_AuraCallback,
            &callbackArgs,
            TRUE);
    }

    return 1;
}

// 1.12a: 0x6FADCAA0
int32_t __fastcall MISSILE_CltHit_PlagueJavelinCloud(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = MISSILE_GetSkillsTxt(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    int32_t nMissileIdList[3] = {};
    int32_t nValidSubCount = 0;
    for (int32_t i = 0; i < 3; ++i)
    {
        const int16_t nSubId = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[i]);
        if (nSubId < 0)
        {
            break;
        }
        nMissileIdList[nValidSubCount++] = nSubId;
    }

    if (nValidSubCount <= 0)
    {
        return 1;
    }

    int32_t nRadius = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]);
    if (nRadius <= 0)
    {
        nRadius = SKILLS_EvaluateSkillFormula(CUNIT_GetOwnerUnit(pMissile), pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
        if (nRadius < 1)
        {
            nRadius = 1;
        }
    }

    int32_t nBaseRange = MISSILE_EvaluateMissileFormula(
        pMissile,
        CUNIT_GetOwnerUnit(pMissile),
        pMissilesTxtRecord->dwHitCalc,
        pMissile->dwClassId,
        nSkillLevel);
    if (nBaseRange <= 0)
    {
        nBaseRange = static_cast<int16_t>(pMissilesTxtRecord->wRange)
            + (nSkillLevel - 1) * static_cast<int16_t>(pMissilesTxtRecord->wLevRange);
    }

    MISSILE_SpawnRadialRaytracedField(
        pMissile,
        nMissileIdList,
        nValidSubCount,
        nRadius,
        nBaseRange,
        static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]));

    return 1;
}

// 1.12a: 0x6FADCBE0
int32_t __fastcall MISSILE_CltHit_PoisonPotionRing(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (pMissile)
    {
        D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
        if (pMissilesTxtRecord && static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) >= 0)
        {
            MISSILE_SpawnDualRingPoisonCloud(
                pMissile,
                static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]),
                static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[2]),
                static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]),
                static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]));
        }
    }

    return 1;
}

// 1.12a: 0x6FADCC40
static void __fastcall MISSILE_SpawnCircularGroundFieldAtUnit(
    D2UnitStrc* pOrigin,
    int32_t nRadius,
    int32_t nMissileId,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nSkipChanceDivisor,
    int32_t nGridStep)
{
    const int32_t nCenterY = CLIENTS_GetUnitY(pOrigin);
    const int32_t nCenterX = CLIENTS_GetUnitX(pOrigin);
    MISSILE_SpawnCircularGroundField(
        nMissileId,
        pOrigin,
        nCenterX,
        nCenterY,
        nRadius,
        nSkillId,
        nSkillLevel,
        nSkipChanceDivisor,
        nGridStep);
}

// 1.12a: 0x6FADCCD0
void __fastcall MISSILE_CltDo_Volcano(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = MISSILE_GetSkillsTxt(nSkillId);
    if (!pSkillsTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);

    int32_t nRadius = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]);
    if (nRadius <= 0)
    {
        nRadius = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
        if (nRadius < 1)
        {
            nRadius = 1;
        }
    }

    int32_t nPeriod = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
    if (nPeriod <= 0)
    {
        nPeriod = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
        if (nPeriod < 1)
        {
            nPeriod = 1;
        }
    }

    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);
    if (nRemainingFrames > static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2])
        && nRemainingFrames < static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[3])
        && (nRemainingFrames % nPeriod) == 0)
    {
        SEED_InitLowSeed(&pMissile->pSeed, MISSILE_GetTargetX(pMissile));

        const int32_t nSpan = 2 * nRadius + 1;
        const int32_t nTargetX = CLIENTS_GetUnitX(pMissile) + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nRadius;
        const int32_t nTargetY = CLIENTS_GetUnitY(pMissile) + static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nRadius;

        MISSILE_SetTargetX(pMissile, SEED_GetLowSeed(&pMissile->pSeed));

        MISSILE_SpawnParabolicLobMissile(
            pMissile,
            static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]),
            0,
            0,
            nTargetX,
            nTargetY,
            MISSILE_GetSkill(pMissile),
            nSkillLevel,
            static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[4]));
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADCEA0
void __fastcall MISSILE_CltDo_MoltenBoulderBounceAndTrail(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int16_t nProgSound = static_cast<int16_t>(pMissilesTxtRecord->wProgSound);
    if (nProgSound > 0)
    {
        const int32_t nPrevParam = MISSILE_GetTargetX(pMissile);
        int32_t nCurParam = 0;
        if (pMissile->pGfxData && pMissile->pGfxData->pParticle)
        {
            nCurParam = pMissile->pGfxData->pParticle->nBounceVelParam;
        }

        if (nPrevParam != 0 && nPrevParam != nCurParam)
        {
            SOUND_PlayUnitSound(nProgSound, pMissile, 0, 0, FALSE);
        }

        MISSILE_SetTargetX(pMissile, nCurParam);
    }

    MISSILE_CltDo_MoltenBoulderTrail(pMissile);
}

// 1.12a: 0x6FADCF30
void __fastcall MISSILE_CltDo_CheckedQuadrantScatterSpawner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    MISSILE_SpawnQuadrantScatterSubmissiles(
        pMissile,
        pMissilesTxtRecord->dwCltParam[0],
        static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]),
        static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2]),
        static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]));

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADCFA0
static void __fastcall MISSILE_SpawnBaalFxFallingSubmissiles(int32_t nRemainingFrames, int32_t nMissileId, D2UnitStrc* pOwner, D2UnitStrc* pOrigin)
{
    if (nRemainingFrames > 200 && nRemainingFrames < 225)
    {
        MISSILE_SpawnFallingIceOrMeteorInCircle(nMissileId, pOwner, pOrigin, 6);
        MISSILE_SpawnFallingIceOrMeteorInCircle(nMissileId, pOwner, pOrigin, 6);
        return;
    }

    if ((nRemainingFrames % 4) == 0)
    {
        MISSILE_SpawnFallingIceOrMeteorInCircle(nMissileId, pOwner, pOrigin, 20);
    }
}

// 1.12a: 0x6FADCFE0
static void __stdcall MISSILE_SpawnBaalFxCore(D2UnitStrc* pOwner, D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nRemainingFrames, int32_t nStage)
{
    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(nMissileId);
    if (!pMissilesTxtRecord)
    {
        return;
    }

    const int16_t nSub0 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
    if (nSub0 > 0)
    {
        MISSILE_SpawnRisingDebrisCluster(nStage, pOrigin, pOwner, nSub0);
    }

    const int16_t nProgSound = static_cast<int16_t>(pMissilesTxtRecord->wProgSound);
    if (nProgSound > 0 && nRemainingFrames == 315)
    {
        SOUND_PlayUnitSound(nProgSound, pOwner, 0, 0, FALSE);
    }

    if ((nRemainingFrames % 2) != 0)
    {
        return;
    }

    if (nStage < 3 && (nRemainingFrames % 16) != 0)
    {
        return;
    }

    int32_t nSpread = 4;
    if (nStage < 3)
    {
        nSpread = 1;
    }
    else if (nStage < 6)
    {
        nSpread = 2;
    }
    else if (nStage < 9)
    {
        nSpread = 3;
    }

    int32_t nOffX = 0;
    int32_t nOffY = 0;
    if (nSpread > 1)
    {
        const int32_t nHalfSpread = nSpread / 2;
        nOffX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nSpread)) - nHalfSpread;
        nOffY = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pOrigin->pSeed, nSpread)) - nHalfSpread;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS;
    missileParams.pOwner = pOwner;
    missileParams.nMissile = nMissileId;
    missileParams.nX = CLIENTS_GetUnitX(pOrigin) + nOffX;
    missileParams.nY = CLIENTS_GetUnitY(pOrigin) + nOffY;
    missileParams.nSkill = MISSILE_GetSkill(pOrigin);
    missileParams.nSkillLevel = MISSILE_GetLevel(pOrigin);
    MISSILE_CreateClientMissile(&missileParams);
}

// 1.12a: 0x6FADD1B0
void __fastcall MISSILE_CltDo_DualTimerBoltAndLobSpawner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    const int32_t nMaxOffset = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2]);

    const int16_t nSub0 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
    if (nSub0 > 0)
    {
        int32_t nTimerX = MISSILE_GetTargetX(pMissile);
        if (nTimerX <= 0)
        {
            MISSILE_SpawnRandomDirectionSubmissile(pMissile, pOwner, nSub0, nMaxOffset);
            nTimerX = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
        }
        MISSILE_SetTargetX(pMissile, nTimerX - 1);
    }

    const int16_t nSub1 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[1]);
    if (nSub1 > 0)
    {
        int32_t nTimerY = MISSILE_GetTargetY(pMissile);
        if (nTimerY <= 0)
        {
            D2MissileStrc missileParams = {};
            missileParams.dwFlags = CLT_MISSILE_INIT_TARGET_OFFSET | CLT_MISSILE_INIT_LOB;
            missileParams.pOwner = pOwner;
            missileParams.pOrigin = pMissile;
            missileParams.nMissile = nSub1;
            missileParams.nSkill = MISSILE_GetSkill(pMissile);
            missileParams.nSkillLevel = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, 5)) + 1;

            const int32_t nSpan = 2 * nMaxOffset + 1;
            int32_t nOffX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nMaxOffset;
            const int32_t nOffY = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nSpan)) - nMaxOffset;
            if (nOffX == 0 && nOffY == 0)
            {
                nOffX = static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nMaxOffset - 1)) + 1;
            }
            missileParams.nTargetX = nOffX;
            missileParams.nTargetY = nOffY;

            D2UnitStrc* pLobSub = MISSILE_CreateClientMissile(&missileParams);
            if (pLobSub)
            {
                MISSILE_SetTargetX(pLobSub, static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, 5)));
            }

            nTimerY = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]);
        }
        MISSILE_SetTargetY(pMissile, nTimerY - 1);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADD320
void __fastcall MISSILE_CltDo_CairnStonesPortalSpawner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nCurrentFrame = MISSILE_GetCurrentFrame(pMissile);
    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);

    const int32_t nWindowMargin = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
    const int32_t nMaxWindowFrame = static_cast<int16_t>(pMissilesTxtRecord->wRange) - nWindowMargin;
    if (nCurrentFrame < nMaxWindowFrame && nCurrentFrame > nWindowMargin)
    {
        const int16_t nSub0 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
        if (nSub0 > 0)
        {
            D2UnitStrc* pDebris = MISSILE_SpawnPeriodicSubmissileInBox(
                static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]),
                pMissile,
                static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2]),
                nSub0);
            if (pDebris)
            {
                MISSILE_InitFallingDebrisParticle(pDebris);
            }
        }
    }

    if (nCurrentFrame < (nWindowMargin + 125) && nCurrentFrame > nWindowMargin)
    {
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
        for (D2UnitStrc* pUnit = pRoom->pUnitFirst; pUnit != nullptr; pUnit = pUnit->pRoomNext)
        {
            if (pUnit->dwUnitType != UNIT_OBJECT)
            {
                continue;
            }

            if (pUnit->dwClassId < OBJECT_STONEALPHA || pUnit->dwClassId > OBJECT_STONELAMBDA)
            {
                continue;
            }

            const int16_t nSub0 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
            if (nSub0 > 0)
            {
                D2UnitStrc* pStoneFx = MISSILE_CreateClientMissileAtCoords(
                    pOwner,
                    nSub0,
                    CLIENTS_GetUnitX(pUnit),
                    CLIENTS_GetUnitY(pUnit),
                    MISSILE_GetSkill(pMissile),
                    MISSILE_GetLevel(pMissile));
                if (pStoneFx)
                {
                    MISSILE_InitFallingDebrisParticle(pStoneFx);
                }
            }

            const int16_t nSub1 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[1]);
            if (nSub1 > 0 && (nCurrentFrame % 8) == 0)
            {
                const int32_t nPrevStoneGUID = MISSILE_GetTargetX(pMissile);
                D2UnitStrc* pPrevStone = nullptr;
                if (nPrevStoneGUID != 0)
                {
                    pPrevStone = gpClientUnitHashTables[UNIT_OBJECT][nPrevStoneGUID & 0x7F];
                    while (pPrevStone && static_cast<int32_t>(pPrevStone->dwUnitId) != nPrevStoneGUID)
                    {
                        pPrevStone = pPrevStone->pListNext;
                    }
                }

                if (!pPrevStone)
                {
                    MISSILE_SetTargetX(pMissile, static_cast<int32_t>(pUnit->dwUnitId));
                    continue;
                }

                D2_ASSERT(pPrevStone->dwUnitType == UNIT_OBJECT);

                D2MissileStrc missileParams = {};
                missileParams.dwFlags = CLT_MISSILE_INIT_USE_COORDS
                                      | CLT_MISSILE_INIT_USE_TARGET_COORDS
                                      | CLT_MISSILE_INIT_DISTANCE_FRAMES;
                missileParams.pOwner = pOwner;
                missileParams.nMissile = nSub1;
                missileParams.nX = CLIENTS_GetUnitX(pUnit);
                missileParams.nY = CLIENTS_GetUnitY(pUnit);
                missileParams.nTargetX = CLIENTS_GetUnitX(pPrevStone);
                missileParams.nTargetY = CLIENTS_GetUnitY(pPrevStone);
                missileParams.nSkill = MISSILE_GetSkill(pMissile);
                missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
                MISSILE_CreateClientMissile(&missileParams);

                if (SEED_RollLimitedRandomNumber(&pPrevStone->pSeed, 4) == 0)
                {
                    MISSILE_SetTargetX(pMissile, static_cast<int32_t>(pUnit->dwUnitId));
                }
            }
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADD570
void __fastcall MISSILE_CltDo_StreamGroundFieldSpawner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    int32_t nSubMissileId = MISSILE_GetStreamMissile(pMissile);
    if (nSubMissileId == 0)
    {
        nSubMissileId = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
    }
    if (nSubMissileId < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    const int32_t nStreamRange = MISSILE_GetStreamRange(pMissile);
    if (nStreamRange <= 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    if (MISSILE_GetRemainingFrames(pMissile) < static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0])
        && (MISSILE_GetRemainingFrames(pMissile) % 3) == 0)
    {
        const int32_t nSkipChanceDivisor = (nStreamRange > 4) ? (nStreamRange / 2) : nStreamRange;
        const int32_t nGridStep = (nStreamRange > 4) ? 2 : 1;
        MISSILE_SpawnCircularGroundFieldAtUnit(
            pMissile,
            nStreamRange,
            nSubMissileId,
            MISSILE_GetSkill(pMissile),
            MISSILE_GetLevel(pMissile),
            nSkipChanceDivisor,
            nGridStep);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADD640
void __fastcall MISSILE_CltDo_BlizzardSpawner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    int32_t nSubMissileId = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
    const int32_t nMaxSubId = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[1]);
    if (nSubMissileId < nMaxSubId)
    {
        nSubMissileId += static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nMaxSubId - nSubMissileId + 1));
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = MISSILE_GetSkillsTxt(nSkillId);
    if (!pSkillsTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
    const int32_t nRadius = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    const int32_t nPeriod = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);

    if (nPeriod == 0 || nRadius <= 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pSub = MISSILE_SpawnPeriodicSubmissileInBox(nPeriod, pMissile, nRadius, nSubMissileId);
    if (pSub)
    {
        MISSILE_InitRandomDurationDebrisParticle(pSub);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADD770
void __fastcall MISSILE_CltDo_MonBlizzardSpawner(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) < 0)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    int32_t nSubMissileId = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
    const int32_t nMaxSubId = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[1]);
    if (nSubMissileId < nMaxSubId)
    {
        nSubMissileId += static_cast<int32_t>(SEED_RollLimitedRandomNumber(&pMissile->pSeed, nMaxSubId - nSubMissileId + 1));
    }

    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
    int32_t nLevelDivisor = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2]);
    if (nLevelDivisor < 1)
    {
        nLevelDivisor = 1;
    }

    const int32_t nScaledLevel = nSkillLevel / nLevelDivisor;
    const int32_t nRadiusBonus = (nScaledLevel < 2) ? 2 : nScaledLevel;
    const int32_t nRadius = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]) + nRadiusBonus;

    int32_t nPeriod = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]) - nScaledLevel;
    if (nPeriod < 3)
    {
        nPeriod = 3;
    }

    D2UnitStrc* pSub = MISSILE_SpawnPeriodicSubmissileInBox(nPeriod, pMissile, nRadius, nSubMissileId);
    if (pSub)
    {
        MISSILE_InitRandomDurationDebrisParticle(pSub);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADD830
void __fastcall MISSILE_CltDo_QuadrantScatterSpawner(D2UnitStrc* pMissile)
{
    if (pMissile)
    {
        D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
        if (pMissilesTxtRecord)
        {
            MISSILE_SpawnQuadrantScatterSubmissiles(
                pMissile,
                pMissilesTxtRecord->dwCltParam[0],
                static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]),
                static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[2]),
                static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]));
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADD890
void __fastcall MISSILE_CltDo_BlazeStepTrail(D2UnitStrc* pMissile)
{
    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
        if (pMissilesTxtRecord && static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]) >= 0)
        {
            const int32_t nUseLoopsFlag = MISSILE_EvaluateMissileFormula(
                pMissile,
                CUNIT_GetOwnerUnit(pMissile),
                pMissilesTxtRecord->dwCltCalc,
                pMissile->dwClassId,
                MISSILE_GetLevel(pMissile));

            MISSILE_SpawnStationaryStepSubmissile(
                pMissile,
                0,
                static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]),
                nUseLoopsFlag);
        }
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADD910
int32_t __fastcall MISSILE_CltHit_BaalColdMakerBurst(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (pTarget)
    {
        return 0;
    }

    SKILLS_SpawnRandomRadiusSubmissiles(
        pMissile,
        0x16D, // 365
        3,
        6,
        MISSILE_GetSkill(pMissile),
        MISSILE_GetLevel(pMissile));

    MISSILE_CreateClientMissileFromOrigin(
        pMissile,
        0x16E, // 366
        0,
        0,
        MISSILE_GetSkill(pMissile),
        MISSILE_GetLevel(pMissile));

    return 1;
}

// 1.12a: 0x6FADD960
int32_t __fastcall MISSILE_CltHit_GroundCircularField(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0)
    {
        return 1;
    }

    if (pTarget)
    {
        return 0;
    }

    MISSILE_SpawnCircularGroundFieldAtUnit(
        pMissile,
        static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]),
        static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]),
        MISSILE_GetSkill(pMissile),
        MISSILE_GetLevel(pMissile),
        static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]),
        1);

    return 1;
}

// 1.12a: 0x6FADD9D0
int32_t __fastcall MISSILE_CltHit_ClusterSubmissileExplosion(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord || static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        return 1;
    }

    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);
    int32_t nCount = static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0])
        + (nSkillLevel - 1) * static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]);
    if (nCount <= 0)
    {
        const int32_t nSkillId = MISSILE_GetSkill(pMissile);
        D2SkillsTxt* pSkillsTxtRecord = MISSILE_GetSkillsTxt(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 1;
        }

        nCount = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
        if (nCount < 1)
        {
            nCount = 1;
        }
    }

    SKILLS_SpawnRandomRadiusSubmissiles(
        pMissile,
        static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]),
        nCount,
        nCount / 4,
        MISSILE_GetSkill(pMissile),
        nSkillLevel);

    const int16_t nSub1 = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[1]);
    if (nSub1 >= 0)
    {
        MISSILE_CreateClientMissileFromOrigin(
            pMissile,
            nSub1,
            CLIENTS_GetUnitX(pMissile),
            CLIENTS_GetUnitY(pMissile),
            MISSILE_GetSkill(pMissile),
            MISSILE_GetLevel(pMissile));
    }

    return 1;
}

// 1.12a: 0x6FADDAE0
int32_t __fastcall MISSILE_CltHit_FreezingArrow(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nSkillLevel = MISSILE_GetLevel(pMissile);

    const int16_t nSub0 = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]);
    if (nSub0 >= 0)
    {
        MISSILE_CreateClientMissileFromOrigin(pMissile, nSub0, 0, 0, nSkillId, nSkillLevel);
    }

    const int16_t nSub1 = static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[1]);
    if (nSub1 >= 0)
    {
        MISSILE_SpawnCircularGroundFieldAtUnit(
            pMissile,
            static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]),
            nSub1,
            nSkillId,
            nSkillLevel,
            static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]),
            1);
    }

    if (pTarget)
    {
        OVERLAY_SetUnitOverlay(0x97, pTarget, 2, 0, 0, 0, 0, 0);
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (pOwner && pTarget)
    {
        const int16_t nProgOverlay = static_cast<int16_t>(pMissilesTxtRecord->wProgOverlay);
        if (nProgOverlay > 0)
        {
            OVERLAY_SetUnitOverlay(nProgOverlay, pOwner, 2, 0, 0, 0, 0, 0);
        }
    }

    return 1;
}

// 1.12a: 0x6FADDBD0
int32_t __fastcall MISSILE_CltHit_FireballGroundField(D2UnitStrc* pMissile, D2UnitStrc* pTarget)
{
    (void)pTarget;

    if (pMissile)
    {
        D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
        if (pMissilesTxtRecord && static_cast<int16_t>(pMissilesTxtRecord->wCltHitSubMissile[0]) >= 0)
        {
            MISSILE_SpawnCircularGroundFieldAtUnit(
                pMissile,
                static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[0]),
                0x109, // 265
                MISSILE_GetSkill(pMissile),
                MISSILE_GetLevel(pMissile),
                static_cast<int32_t>(pMissilesTxtRecord->dwCltHitPar[1]),
                1);
        }
    }

    return 1;
}

// 1.12a: 0x6FADDC40
void __fastcall MISSILE_CltDo_BaalFxSequenceControl(D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = MISSILE_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        MISSILE_DeleteClientMissile(pMissile);
        return;
    }

    D2UnitStrc* pOwner = CUNIT_GetOwnerUnit(pMissile);
    if (!pOwner)
    {
        MISSILE_CltDo_BasicMissile(pMissile);
        return;
    }

    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);
    const int16_t nSub0 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[0]);
    if (nSub0 > 0 && nRemainingFrames > 10)
    {
        const int32_t nStage = MISSILE_GetBaalFxSpawnRate(nRemainingFrames, pMissilesTxtRecord);
        MISSILE_SpawnBaalFxCore(pOwner, pMissile, nSub0, nRemainingFrames, nStage);
    }

    int32_t nPeriod = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[0]);
    if (nPeriod < 1)
    {
        nPeriod = 1;
    }

    const int16_t nSub1 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[1]);
    if (nSub1 > 0 && (nRemainingFrames % nPeriod) == 0)
    {
        MISSILE_SpawnRandomDirectionSubmissile(pMissile, pOwner, nSub1, 20);
    }

    const int32_t nShakeDurationSec = static_cast<int32_t>(pMissilesTxtRecord->dwCltParam[1]);
    if (nShakeDurationSec > 0 && nRemainingFrames == 90)
    {
        const int32_t nThirdSec = nShakeDurationSec / 3;
        const int32_t nFadeMs = (nThirdSec * 25000) / 25;
        const int32_t nHoldMs = ((nShakeDurationSec - 2 * nThirdSec) * 25000) / 25;
        CAMERA_StartScreenShake(nHoldMs, 8, nFadeMs, nFadeMs);
    }

    const int16_t nSub2 = static_cast<int16_t>(pMissilesTxtRecord->wCltSubMissile[2]);
    if (nSub2 > 0 && nRemainingFrames > 115 && nRemainingFrames < 315)
    {
        MISSILE_SpawnBaalFxFallingSubmissiles(nRemainingFrames, nSub2, pOwner, pMissile);
    }

    MISSILE_CltDo_BasicMissile(pMissile);
}

// 1.12a: 0x6FADDDA0
int32_t __stdcall MISSILE_ReturnZero_6FADDDA0()
{
    return 0;
}

// ============================================================================
// 1.12a Client Missile Dispatch Tables (.data: 0x6FBA67C8 & 0x6FBA6938)
// ============================================================================

// 1.12a: 0x6FBA6934 (91 entries in D2Client.dll .data, checked at 0x6FAD360C)
extern const int32_t gnCltMissileDoFuncs = 91;

// 1.12a: 0x6FBA67C8 (91 active table slots [0..90], padded to [128])
extern const CltMissileDoFunc gCltMissileDoFuncTable[128] = {
    /* [  0] 0x00000000 */ nullptr,
    /* [  1] 0x6FAD31F0 */ MISSILE_CltDo_BasicMissile,
    /* [  2] 0x6FAD4A90 */ MISSILE_CltDo_PlagueJavelin_PoisonTrap,
    /* [  3] 0x6FADD890 */ MISSILE_CltDo_BlazeStepTrail,
    /* [  4] 0x6FADD830 */ MISSILE_CltDo_QuadrantScatterSpawner,
    /* [  5] 0x6FAD49C0 */ MISSILE_CltDo_FireWall_MeteorFire,
    /* [  6] 0x6FADACE0 */ MISSILE_CltDo_MoltenBoulderTrail,
    /* [  7] 0x6FAD48E0 */ MISSILE_CltDo_GuidedArrow_BoneSpirit,
    /* [  8] 0x6FADABF0 */ MISSILE_CltDo_FireWallMakerSynced,
    /* [  9] 0x6FADA930 */ MISSILE_CltDo_MeteorCenterLightAndFall,
    /* [ 10] 0x6FADD770 */ MISSILE_CltDo_MonBlizzardSpawner,
    /* [ 11] 0x6FAD47F0 */ MISSILE_CltDo_FadeLightAtEndOfCycle,
    /* [ 12] 0x6FAD4730 */ MISSILE_CltDo_DiabWallMaker,
    /* [ 13] 0x6FADD640 */ MISSILE_CltDo_BlizzardSpawner,
    /* [ 14] 0x6FAD44B0 */ MISSILE_CltDo_GrimWard,
    /* [ 15] 0x6FADA5F0 */ MISSILE_CltDo_LightningWallSeeker,
    /* [ 16] 0x6FADA4D0 */ MISSILE_CltDo_SpiderGooTrail,
    /* [ 17] 0x6FADD570 */ MISSILE_CltDo_StreamGroundFieldSpawner,
    /* [ 18] 0x6FADA390 */ MISSILE_CltDo_AlignedWakeTrail,
    /* [ 19] 0x6FADA270 */ MISSILE_CltDo_FrozenOrb,
    /* [ 20] 0x6FAD43E0 */ MISSILE_CltDo_BladeCreeper,
    /* [ 21] 0x6FADA120 */ MISSILE_CltDo_TowerChestSpawner,
    /* [ 22] 0x6FAD9F80 */ MISSILE_CltDo_Twister_FirestormCurve,
    /* [ 23] 0x6FAD43B0 */ MISSILE_CltDo_CountdownTimer,
    /* [ 24] 0x6FADD320 */ MISSILE_CltDo_CairnStonesPortalSpawner,
    /* [ 25] 0x6FAD9E00 */ MISSILE_CltDo_EruptionCenter,
    /* [ 26] 0x6FADD1B0 */ MISSILE_CltDo_DualTimerBoltAndLobSpawner,
    /* [ 27] 0x6FAD9B60 */ MISSILE_CltDo_Tornado,
    /* [ 28] 0x6FAD9A40 */ MISSILE_CltDo_CardinalOwnerCrossSpawner,
    /* [ 29] 0x6FADDC40 */ MISSILE_CltDo_BaalFxSequenceControl,
    /* [ 30] 0x6FAD9420 */ MISSILE_CltDo_DelayedPeriodicScatterSpawner,
    /* [ 31] 0x6FAD65B0 */ MISSILE_CltDo_MephistoDeathControl,
    /* [ 32] 0x6FAD3D70 */ MISSILE_CltDo_IzualDeathControl,
    /* [ 33] 0x6FAD6190 */ MISSILE_CltDo_SoulCollectorControl,
    /* [ 34] 0x6FAD39F0 */ MISSILE_CltDo_AndarielDeathControl,
    /* [ 35] 0x6FAD5ED0 */ MISSILE_CltDo_CountessFirewallLobSpawner,
    /* [ 36] 0x6FAD5C20 */ MISSILE_CltDo_BaalDeathControl,
    /* [ 37] 0x6FAD3F30 */ MISSILE_CltDo_ScreenShakeTrigger,
    /* [ 38] 0x6FADC210 */ MISSILE_CltDo_HellforgeSoulBurstTrigger,
    /* [ 39] 0x6FAD3EB0 */ MISSILE_CltDo_FollowOwnerRemaining,
    /* [ 40] 0x6FADC2B0 */ MISSILE_CltDo_HellforgeAscendingSpiritSpawner,
    /* [ 41] 0x6FAD3ED0 */ MISSILE_CltDo_AnimSpeedRamp,
    /* [ 42] 0x6FADC0B0 */ MISSILE_CltDo_HephastoHellforgeDebrisControl,
    /* [ 43] 0x6FAD42C0 */ MISSILE_CltDo_UpdateAttachedOwnerPos,
    /* [ 44] 0x6FAD41C0 */ MISSILE_CltDo_TrailingSubmissile,
    /* [ 45] 0x6FADCF30 */ MISSILE_CltDo_CheckedQuadrantScatterSpawner,
    /* [ 46] 0x6FAD92B0 */ MISSILE_CltDo_PerpendicularWallMaker,
    /* [ 47] 0x6FADCEA0 */ MISSILE_CltDo_MoltenBoulderBounceAndTrail,
    /* [ 48] 0x6FAD8FF0 */ MISSILE_CltDo_Armageddon_HurricaneSpawner,
    /* [ 49] 0x6FAD4150 */ MISSILE_CltDo_PeriodicSubmissile,
    /* [ 50] 0x6FADCCD0 */ MISSILE_CltDo_Volcano,
    /* [ 51] 0x6FAD8E10 */ MISSILE_CltDo_TimedClusterAndHideOwner,
    /* [ 52] 0x6FAD8D00 */ MISSILE_CltDo_SymmetricDualWakeSubmissile,
    /* [ 53] 0x6FAD8C50 */ MISSILE_CltDo_GuidedTrailingSubmissile,
    /* [ 54] 0x6FADB640 */ MISSILE_CltDo_AncientsDeathControl,
    /* [ 55] 0x6FAD5290 */ MISSILE_CltDo_SanctuarySpawner,
    /* [ 56] 0x6FAD8AD0 */ MISSILE_CltDo_PeriodicRandomSubmissileCollisionTrigger,
    /* [ 57] 0x6FAD8990 */ MISSILE_CltDo_BlessedHammerSpawner,
    /* [ 58] 0x6FAD4000 */ MISSILE_CltDo_VineRecyclerDelay,
    /* [ 59] 0x6FAD36F0 */ MISSILE_CltDo_TargetCoordCheck,
    /* [ 60] 0x6FAD3630 */ MISSILE_CltDo_RemainingFrameSpawner,
    /* [ 61] 0x6FADC030 */ MISSILE_CltDo_ShenkCatapultMeteorControl,
    /* [ 62] 0x6FADBED0 */ MISSILE_CltDo_WorldstoneDestructionControl,
    /* [ 63] 0x6FAD3950 */ MISSILE_CltDo_PathCoordCheck,
    /* [ 64] 0x6FADBA20 */ MISSILE_CltDo_ShenkDeathControl,
    /* [ 65] 0x6FAD3740 */ MISSILE_CltDo_NihlathakDeathControl,
    /* [ 66] 0x6FADB8D0 */ MISSILE_CltDo_WorldstoneQuakeSpawner,
    /* [ 67] 0x6FAD4620 */ MISSILE_CltDo_BaalTauntControl,
    /* [ 68] 0x6FAD91D0 */ MISSILE_CltDo_LoopedTrailSubmissile,
    /* [ 69] 0x00000000 */ nullptr,
    /* [ 70] 0x00000000 */ nullptr,
    /* [ 71] 0x00000000 */ nullptr,
    /* [ 72] 0x00000000 */ nullptr,
    /* [ 73] 0x00000000 */ nullptr,
    /* [ 74] 0x00000000 */ nullptr,
    /* [ 75] 0x00000000 */ nullptr,
    /* [ 76] 0x00000000 */ nullptr,
    /* [ 77] 0x00000000 */ nullptr,
    /* [ 78] 0x00000000 */ nullptr,
    /* [ 79] 0x00000000 */ nullptr,
    /* [ 80] 0x00000000 */ nullptr,
    /* [ 81] 0x00000000 */ nullptr,
    /* [ 82] 0x00000000 */ nullptr,
    /* [ 83] 0x00000000 */ nullptr,
    /* [ 84] 0x00000000 */ nullptr,
    /* [ 85] 0x00000000 */ nullptr,
    /* [ 86] 0x00000000 */ nullptr,
    /* [ 87] 0x00000000 */ nullptr,
    /* [ 88] 0x00000000 */ nullptr,
    /* [ 89] 0x00000000 */ nullptr,
    /* [ 90] 0x00000000 */ nullptr,
    /* [ 91..127]       */ nullptr
};

// 1.12a: 0x6FBA6A7C (81 entries in D2Client.dll .data, checked at 0x6FAD2FED)
extern const int32_t gnCltMissileHitFuncs = 81;

// 1.12a: 0x6FBA6938 (81 active table slots [0..80], padded to [128])
extern const CltMissileHitFunc gCltMissileHitFuncTable[128] = {
    /* [  0] 0x00000000 */ nullptr,
    /* [  1] 0x6FADDBD0 */ MISSILE_CltHit_FireballGroundField,
    /* [  2] 0x6FADCBE0 */ MISSILE_CltHit_PoisonPotionRing,
    /* [  3] 0x6FAD2AB0 */ MISSILE_CltHit_ExplosivePotion,
    /* [  4] 0x6FAD7930 */ MISSILE_CltHit_ExplodingArrow_NovaRing,
    /* [  5] 0x00000000 */ nullptr,
    /* [  6] 0x00000000 */ nullptr,
    /* [  7] 0x00000000 */ nullptr,
    /* [  8] 0x00000000 */ nullptr,
    /* [  9] 0x6FAD83C0 */ MISSILE_CltHit_HolyBolt,
    /* [ 10] 0x6FAD3FA0 */ MISSILE_CltHit_GuidedArrow_BoneSpirit,
    /* [ 11] 0x00000000 */ nullptr,
    /* [ 12] 0x6FADCAA0 */ MISSILE_CltHit_PlagueJavelinCloud,
    /* [ 13] 0x6FAD2D80 */ MISSILE_CltHit_GlacialSpike,
    /* [ 14] 0x6FAD22A0 */ MISSILE_CltHit_MeteorCenter,
    /* [ 15] 0x00000000 */ nullptr,
    /* [ 16] 0x6FAD8200 */ MISSILE_CltHit_ChainLightning,
    /* [ 17] 0x00000000 */ nullptr,
    /* [ 18] 0x6FAD7F70 */ MISSILE_CltHit_MeteorImpact,
    /* [ 19] 0x6FAD2A30 */ MISSILE_CltHit_FingerMageSpider,
    /* [ 20] 0x00000000 */ nullptr,
    /* [ 21] 0x00000000 */ nullptr,
    /* [ 22] 0x00000000 */ nullptr,
    /* [ 23] 0x00000000 */ nullptr,
    /* [ 24] 0x6FAD2230 */ MISSILE_CltHit_PantherPotOrange,
    /* [ 25] 0x6FADC920 */ MISSILE_CltHit_FistOfTheHeavens,
    /* [ 26] 0x6FADC770 */ MISSILE_CltHit_LightningFury,
    /* [ 27] 0x00000000 */ nullptr,
    /* [ 28] 0x6FADC6E0 */ MISSILE_CltHit_PantherPotGreen,
    /* [ 29] 0x6FAD7B50 */ MISSILE_CltHit_ImmolationFirePatch,
    /* [ 30] 0x6FAD7A40 */ MISSILE_CltHit_FrozenOrbNovaRing,
    /* [ 31] 0x6FAD1FE0 */ MISSILE_CltHit_FireHead,
    /* [ 32] 0x6FADDAE0 */ MISSILE_CltHit_FreezingArrow,
    /* [ 33] 0x6FAD7800 */ MISSILE_CltHit_TowerChestLobSpawner,
    /* [ 34] 0x6FAD2210 */ MISSILE_CltHit_OwnerCheckHit,
    /* [ 35] 0x00000000 */ nullptr,
    /* [ 36] 0x6FADC620 */ MISSILE_CltHit_CatapultPoisonCloudField,
    /* [ 37] 0x6FAD6420 */ MISSILE_CltHit_ChargedBoltBurstOnGround,
    /* [ 38] 0x6FAD6000 */ MISSILE_CltHit_LinkReturnSoulMissile,
    /* [ 39] 0x6FAD1F00 */ MISSILE_CltHit_ImpSpawnMonsters,
    /* [ 40] 0x6FAD1FA0 */ MISSILE_CltHit_CatapultSpikeBall,
    /* [ 41] 0x6FADD910 */ MISSILE_CltHit_BaalColdMakerBurst,
    /* [ 42] 0x6FAD6D30 */ MISSILE_CltHit_HellforgeSpiritStage2,
    /* [ 43] 0x6FAD6BF0 */ MISSILE_CltHit_HellforgeSpiritStage1,
    /* [ 44] 0x6FAD2170 */ MISSILE_CltHit_ExplodingJavelin,
    /* [ 45] 0x00000000 */ nullptr,
    /* [ 46] 0x6FADC4F0 */ MISSILE_CltHit_ChargedStrikeBolts,
    /* [ 47] 0x6FADD9D0 */ MISSILE_CltHit_ClusterSubmissileExplosion,
    /* [ 48] 0x6FAD25A0 */ MISSILE_CltHit_MoltenBoulderEmerge,
    /* [ 49] 0x00000000 */ nullptr,
    /* [ 50] 0x6FADC4A0 */ MISSILE_CltHit_VolcanoCraterBurst,
    /* [ 51] 0x6FADC3D0 */ MISSILE_CltHit_ChargedBoltFanOnHit,
    /* [ 52] 0x6FADC320 */ MISSILE_CltHit_MoltenBoulderImpact,
    /* [ 53] 0x6FAD7440 */ MISSILE_CltHit_BounceLobToFirstPoint,
    /* [ 54] 0x6FAD2910 */ MISSILE_CltHit_BaalSpawnMonsters,
    /* [ 55] 0x6FAD7360 */ MISSILE_CltHit_TripleHitSubmissiles,
    /* [ 56] 0x6FAD7280 */ MISSILE_CltHit_ArmageddonSubmissileSameDir,
    /* [ 57] 0x6FADD960 */ MISSILE_CltHit_GroundCircularField,
    /* [ 58] 0x00000000 */ nullptr,
    /* [ 59] 0x00000000 */ nullptr,
    /* [ 60] 0x6FAD71A0 */ MISSILE_CltHit_DualSubmissileWithHeight,
    /* [ 61] 0x6FAD1CA0 */ MISSILE_CltHit_ChargedBoltHit,
    /* [ 62] 0x6FAD5150 */ MISSILE_CltHit_RandomTargetSubmissile,
    /* [ 63] 0x6FADB5E0 */ MISSILE_CltHit_OuterRingPoisonBurst,
    /* [ 64] 0x6FAD84F0 */ MISSILE_CltHit_EightWaySubmissileBurst,
    /* [ 65] 0x00000000 */ nullptr,
    /* [ 66] 0x00000000 */ nullptr,
    /* [ 67] 0x00000000 */ nullptr,
    /* [ 68] 0x00000000 */ nullptr,
    /* [ 69] 0x00000000 */ nullptr,
    /* [ 70] 0x00000000 */ nullptr,
    /* [ 71] 0x00000000 */ nullptr,
    /* [ 72] 0x00000000 */ nullptr,
    /* [ 73] 0x00000000 */ nullptr,
    /* [ 74] 0x00000000 */ nullptr,
    /* [ 75] 0x00000000 */ nullptr,
    /* [ 76] 0x00000000 */ nullptr,
    /* [ 77] 0x00000000 */ nullptr,
    /* [ 78] 0x00000000 */ nullptr,
    /* [ 79] 0x00000000 */ nullptr,
    /* [ 80] 0x00000000 */ nullptr,
    /* [ 81] 0x00000000 */ nullptr,
    /* [ 82] 0x00000000 */ nullptr,
    /* [ 83..127]       */ nullptr
};
