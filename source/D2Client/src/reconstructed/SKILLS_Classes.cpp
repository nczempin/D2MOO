// ============================================================================
// SKILLS_Part2_Classes.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll (ImageBase = 0x6FAB0000)
//
// Client Skill Start (`gpfnCltStFunc`) & Do (`gpfnCltDoFunc`) Slices:
//   - Section 1: Paladin Skills                (0x6FB373F0..0x6FB38077, 10 procs)
//   - Section 2: Shared Multi-Hit & Monsters   (0x6FB4C270..0x6FB4E92E, 41 procs)
//   - Section 3: Barbarian Skills              (0x6FB73920..0x6FB75252, 24 procs)
//   - Section 4: Druid, Exp. Monsters, Necro,
//                Proc Skills (SCmd 0x99/0x9A),
//                PvP Alignment & Assassin      (0x6FB76050..0x6FB798C4, 57 procs)
//
// Skipped Non-Skill / Already-Reconstructed Subranges:
//   - 0x6FB38080            : UI_ui.cpp
//   - 0x6FB75260..0x6FB7604F: ENGINE/Particle.cpp, Ambient Music, RC4 &
//                             SOUND_GetMonsterModeSoundId (in DRAW_dLightMap.cpp)
//   - 0x6FB78970..0x6FB789EC: zlib 1.1.4 inflate helpers
//   - >= 0x6FB798C7         : MSVC 6.0 CRT stubs
// ============================================================================

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <Windows.h>

#include "D2CommonDefinitions.h"
#include "D2DataTbls.h"
#include "DataTbls/SkillsIds.h"
#include "DataTbls/MissilesIds.h"
#include "DataTbls/MonsterIds.h"
#include "DataTbls/SequenceTbls.h"
#include "D2Skills.h"
#include "D2States.h"
#include "D2StatList.h"
#include "D2Items.h"
#include "D2Dungeon.h"
#include "D2Collision.h"
#include "D2Monsters.h"
#include "D2Seed.h"
#include "Units/Units.h"
#include "Units/Missile.h"
#include "Units/Monster.h"
#include "Units/UnitFinds.h"
#include "Path/Path.h"
#include "Path/Step.h"
#include "UselessOrdinals.h"
#include "D2Gfx.h"
#include "Scale.h"
#include "D2CMP.h"
#include "Fog.h"

// ============================================================================
// Compile-Time Verification of 1.12a Enum & Struct Constants
// ============================================================================
static_assert(MONSTER_SANDLEAPER1 == 0x4E, "1.12a MONSTER_SANDLEAPER1 must equal 78 (0x4E)");
static_assert(MONSTER_DURIEL == 0xD3, "1.12a MONSTER_DURIEL must equal 211 (0xD3)");
static_assert(MONSTER_REANIMATEDHORDE1 == 0x1B4, "1.12a MONSTER_REANIMATEDHORDE1 must equal 436 (0x1B4)");
static_assert(MONSTER_SIEGEBEAST1 == 0x1B9, "1.12a MONSTER_SIEGEBEAST1 must equal 441 (0x1B9)");
static_assert(MONSTER_ANCIENTBARB1 == 0x21C, "1.12a MONSTER_ANCIENTBARB1 must equal 540 (0x21C)");
static_assert(MISSILE_CURSECENTER == 0xBF, "1.12a MISSILE_CURSECENTER must equal 191 (0xBF)");
static_assert(MISSILE_CORPSEEXPLOSION == 0x73, "1.12a MISSILE_CORPSEEXPLOSION must equal 115 (0x73)");
static_assert(MISSILE_VINE_BEAST_NEUTRAL == 0x1D4, "1.12a MISSILE_VINE_BEAST_NEUTRAL must equal 468 (0x1D4)");
static_assert(MISSILE_BAALCORPSEEXPLODEDELAY == 0x24B, "1.12a MISSILE_BAALCORPSEEXPLODEDELAY must equal 587 (0x24B)");
static_assert(offsetof(D2MonStats2Txt, nInfernoAnim) == 0x109, "1.12a D2MonStats2Txt::nInfernoAnim must be at +0x109");
static_assert(offsetof(D2MonsterDataStrc, nComponent) == 0x04, "1.12a D2MonsterDataStrc::nComponent must be at +0x04");
static_assert(sizeof(D2SkillsTxt) == 0x23C, "1.12a D2SkillsTxt must be 0x23C bytes");
static_assert(sizeof(D2MissilesTxt) == 0x1A4, "1.12a D2MissilesTxt must be 0x1A4 bytes");

// ============================================================================
// Client Skill Function Pointer Type & Internal Client Structures
// ============================================================================

typedef int32_t (__fastcall *D2CltSkillFunc_t)(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);

#pragma pack(push, 1)

// 0x4C-byte 3D particle/arc attachment stored at pUnit->pGfxData (+0x54) -> +0x30
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
static_assert(sizeof(D2ParticleStrc) == 0x4C, "1.12a D2ParticleStrc must be 0x4C bytes");

// Client-side unit graphics attachment pointed to by D2UnitStrc::pGfxData (+0x54)
struct D2ClientUnitGfxInfoStrc
{
    uint8_t         pad0x00[0x24]; // +0x00..+0x23 (component layer pointers & state)
    uint32_t        dwFlags;       // +0x24 (0x08 = force directional override during draw)
    uint8_t         pad0x28[0x08]; // +0x28..+0x2F
    D2ParticleStrc* pParticle;     // +0x30
};
static_assert(offsetof(D2ClientUnitGfxInfoStrc, dwFlags) == 0x24, "D2ClientUnitGfxInfoStrc::dwFlags must be at +0x24");
static_assert(offsetof(D2ClientUnitGfxInfoStrc, pParticle) == 0x30, "D2ClientUnitGfxInfoStrc::pParticle must be at +0x30");

// 0x5C-byte (23-dword) parameter block passed to D2Client_CreateClientMissile (0x6FAD4B20)
struct D2ClientMissileInitStrc
{
    uint32_t    dwFlags;            // +0x00
    D2UnitStrc* pOwner;             // +0x04
    D2UnitStrc* pOrigin;            // +0x08
    D2UnitStrc* pTarget;            // +0x0C
    int32_t     nMissileId;         // +0x10
    int32_t     nX;                 // +0x14
    int32_t     nY;                 // +0x18
    int32_t     nTargetX;           // +0x1C
    int32_t     nTargetY;           // +0x20
    int32_t     nZOffset;           // +0x24
    int32_t     nVelocity;          // +0x28
    int32_t     nSkillId;           // +0x2C
    int32_t     nSkillLevel;        // +0x30
    int32_t     nLoops;             // +0x34
    int32_t     nFrame;             // +0x38
    int32_t     nActivateFrame;     // +0x3C
    int32_t     nRange;             // +0x40
    D2SeedStrc* pSeed;              // +0x44
    int32_t     nCltSubMissileIndex;// +0x48
    int32_t     nSubLoopIndex;      // +0x4C
    void (__fastcall* pfPostInit)(D2UnitStrc* pMissile, int32_t nInitSeed); // +0x50
    int32_t     nPostInitArg;       // +0x54
    uint32_t    dwPathFlags;        // +0x58
};
static_assert(sizeof(D2ClientMissileInitStrc) == 0x5C, "1.12a D2ClientMissileInitStrc must be 0x5C bytes");

// Player PvP/relation list structures matching GAME_Roster.cpp (0x6FB27890)
struct D2PlayerRelationNodeStrc
{
    uint32_t                  dwTargetUnitId;  // +0x00
    uint32_t                  dwRelationFlags; // +0x04 (0x08 = Hostile)
    D2PlayerRelationNodeStrc* pNext;           // +0x08
};

struct D2PlayerRelationListStrc
{
    D2PlayerRelationNodeStrc* pHead;           // +0x00
};

struct D2RosterUnitStrc
{
    char                      szName[16];       // +0x00
    uint32_t                  dwUnitId;         // +0x10
    uint32_t                  dwPartyLife;      // +0x14
    int32_t                   nArenaScore;      // +0x18
    uint32_t                  dwClassId;        // +0x1C
    uint16_t                  wLevel;           // +0x20
    uint16_t                  wPartyId;         // +0x22
    uint32_t                  dwLevelId;        // +0x24
    int32_t                   nXpos;            // +0x28
    int32_t                   nYpos;            // +0x2C
    uint32_t                  dwPartyFlags;     // +0x30
    D2PlayerRelationListStrc* pPvpRelationList; // +0x34
    void*                     pCorpseList;      // +0x38
    uint32_t                  dwQuestFlagsLow;  // +0x3C
    uint32_t                  dwQuestFlagsHigh; // +0x40
    uint16_t                  wGuildEmblem;     // +0x44
    char                      szGuildTag[4];    // +0x46
    char                      szAccountName[28];// +0x4A
    char                      szFormattedName[26];// +0x66
    D2RosterUnitStrc*         pNext;            // +0x80
};
static_assert(sizeof(D2RosterUnitStrc) == 0x84, "1.12a D2RosterUnitStrc must be 0x84 bytes");

// Callback state block for Paladin Charge replacement target search (0x6FB377A0 -> 0x6FAFDB00)
struct D2ChargeTargetFindArgStrc
{
    D2UnitStrc* pNextHigherUnit; // +0x00
    uint32_t    dwMinHigherId;   // +0x04
    D2UnitStrc* pLowestUnit;     // +0x08
    uint32_t    dwLowestId;      // +0x0C
    uint32_t    dwPrevTargetId;  // +0x10
    int32_t     nVisitedCount;   // +0x14
};
static_assert(sizeof(D2ChargeTargetFindArgStrc) == 0x18, "D2ChargeTargetFindArgStrc must be 0x18 bytes");

// Leap parabolic arc piece entry at 0x6FB86FBC
struct D2LeapArcPieceStrc
{
    int32_t nMaxDist;   // +0x00
    int32_t nSlope;     // +0x04
    int32_t nIntercept; // +0x08
};

// Whirlwind weapon hit-class swing sound entry at 0x6FBA3D70
struct D2WhirlwindSwingSoundStrc
{
    int32_t nBaseSoundId; // +0x00
    int32_t nVariantCount;// +0x04
};

// SCmd 0x99 (SkillTriggeredOnUnit) & 0x9A (SkillTriggeredAtCoord) server->client packets
struct D2GSPacketSrv99Strc
{
    uint8_t  nHeader;       // +0x00 (0x99)
    uint8_t  nAttackerType; // +0x01
    uint32_t dwAttackerGUID;// +0x02
    uint8_t  nTargetType;   // +0x06
    uint32_t dwTargetGUID;  // +0x07
    uint16_t wSkillId;      // +0x0B
    uint8_t  nSkillLevel;   // +0x0D
};

struct D2GSPacketSrv9AStrc
{
    uint8_t  nHeader;       // +0x00 (0x9A)
    uint8_t  nAttackerType; // +0x01
    uint32_t dwAttackerGUID;// +0x02
    uint16_t wSkillId;      // +0x06
    uint8_t  nSkillLevel;   // +0x08
    uint16_t wTargetX;      // +0x09
    uint16_t wTargetY;      // +0x0B
};

#pragma pack(pop)

// ============================================================================
// 1.12a .rdata & .data Lookup Tables (Extracted Directly from D2Client.dll)
// ============================================================================

static const char s_szSkillsPalFile[]  = "..\\Source\\D2Client\\SKILLS\\SkillsPal.cpp";  // 1.12a: 0x6FB84550
static const char s_szSkillsMonFile[]  = "..\\Source\\D2Client\\SKILLS\\SkillsMon.cpp";  // 1.12a: 0x6FB83F68
static const char s_szSkillsBarFile[]  = "..\\Source\\D2Client\\SKILLS\\SkillsBar.cpp";  // 1.12a: 0x6FB86FD4
static const char s_szSkillsEMonFile[] = "..\\Source\\D2Client\\SKILLS\\SkillsEMon.cpp"; // 1.12a: 0x6FB85DC8
static const char s_szSkillsNecFile[]  = "..\\Source\\D2Client\\SKILLS\\SkillsNec.cpp";  // 1.12a: 0x6FB870B8

// 1.12a: 0x6FB84598 & 0x6FB84578 (Blessed Hammer 8-direction target offsets)
static const int32_t g_aBlessedHammerOffsetX_6FB84598[8] = { 0, -1, -2, -1, 0, 1, 2, 1 };
static const int32_t g_aBlessedHammerOffsetY_6FB84578[8] = { 2, 1, 0, -1, -2, -1, 0, 1 };

// 1.12a: 0x6FB84080 & 0x6FB84088 (Andariel Poison Spray facing & per-frame fan directions)
static const uint8_t g_aAndarielBaseDir_6FB84080[8] = { 29, 28, 27, 26, 25, 24, 31, 30 };
static const uint8_t g_aAndarielSprayDirs_6FB84088[8][9] = {
    { 27, 14, 15,  3, 99,  7, 21, 22, 31 },
    { 26, 12, 13,  2, 99,  6, 19, 20, 30 },
    { 25, 10, 11,  1, 99,  5, 17, 18, 29 },
    { 24,  8,  9,  0, 99,  4, 15, 16, 28 },
    { 31, 22, 23,  7, 99,  3, 13, 14, 27 },
    { 30, 20,  7,  6, 99,  2,  1, 12, 26 },
    { 29, 18, 19,  5, 99,  1,  9, 10, 25 },
    { 28, 16, 17,  4, 99,  0, 23,  8, 24 },
};

// 1.12a: 0x6FB83E68 & 0x6FB83D68 (Prime Poison Nova 64-direction circle radius-30 offsets)
static const int32_t g_aPrimePoisonNovaOffsetX_6FB83E68[64] = {
     30,  29,  29,  28,  27,  26,  24,  23,  21,  19,  16,  14,  11,   8,   5,   2,
      0,  -2,  -5,  -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29,
    -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11,  -8,  -5,  -2,
      0,   2,   5,   8,  11,  14,  16,  19,  21,  23,  24,  26,  27,  28,  29,  29,
};
static const int32_t g_aPrimePoisonNovaOffsetY_6FB83D68[64] = {
      0,   2,   5,   8,  11,  14,  16,  19,  21,  23,  24,  26,  27,  28,  29,  29,
     30,  29,  29,  28,  27,  26,  24,  23,  21,  19,  16,  14,  11,   8,   5,   2,
      0,  -2,  -5,  -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29,
    -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11,  -8,  -5,  -2,
};

// 1.12a: 0x6FB83FA8 & 0x6FB83FE8 (Diablo Fire Wall 16-direction step vectors)
static const int32_t g_aDiabWallStepX_6FB83FA8[16] = {
     2,  2,  2,  1,  0, -1, -2, -2, -2, -2, -2, -1,  0,  1,  2,  2
};
static const int32_t g_aDiabWallStepY_6FB83FE8[16] = {
     0,  1,  2,  2,  2,  2,  2,  1,  0, -1, -2, -2, -2, -2, -2, -1
};

// 1.12a: 0x6FB83F90 & 0x6FB84028 (Monster / Imp Inferno random velocity adjustments)
static const int32_t g_aMonInfernoVelDeltas_6FB83F90[5] = { -1, -3, -2, -1,  0 };
static const int32_t g_aImpInfernoVelDeltas_6FB84028[5] = { -2, -1,  0,  1,  2 };

// 1.12a: 0x6FB8403C & 0x6FB84040 (Arcane Tower / Lightning Spire 64->4 cardinal direction snap)
static const uint8_t g_aArcaneTowerSnapDir_6FB8403C[4] = { 49, 31, 17, 0 };
static const uint8_t g_aArcaneTowerQuadrant_6FB84040[64] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    0, 0, 0, 0, 0, 0, 0
};

// 1.12a: 0x6FB86FBC (Barbarian Leap / Leap Attack parabolic gravity slope table)
static const D2LeapArcPieceStrc g_aLeapArcTable_6FB86FBC[2] = {
    { 27, -270, 8462 },
    { 36,  -72, 1224 },
};

// 1.12a: 0x6FBA3D70 (Barbarian Whirlwind weapon hit-class swing sound table)
static const D2WhirlwindSwingSoundStrc g_aWhirlwindSwingSounds_6FBA3D70[14] = {
    {   0, 0 }, { 251, 6 }, { 262, 6 }, { 268, 6 },
    { 274, 8 }, { 280, 8 }, { 286, 6 }, { 292, 8 },
    { 268, 6 }, { 298, 6 }, {   0, 0 }, {   0, 0 },
    {   0, 0 }, {   0, 0 },
};

// 1.12a: 0x6FB838D8 & 0x6FB838F8 (Druid Vine / Eruption 8-neighbor offsets)
static const int32_t g_aDruidVineOffsetX_6FB838D8[8] = {  0,  0,  1, -1,  1, -1, -1,  1 };
static const int32_t g_aDruidVineOffsetY_6FB838F8[8] = {  1, -1,  0,  0,  1, -1,  1, -1 };

// 1.12a: 0x6FB83918 (Molten Boulder / Eruption random velocity adjustments)
static const int32_t g_aMoltenBoulderVelDeltas_6FB83918[5] = { -2, -1,  0,  1,  2 };

// 1.12a: 0x6FB850EC & 0x6FB850CC (Hurricane / Armageddon / Blade Shield 8-point ring)
static const int32_t g_aHurricaneRingOffsetX_6FB850EC[8] = {  7,  5,  0, -5, -7, -5,  0,  5 };
static const int32_t g_aHurricaneRingOffsetY_6FB850CC[8] = {  0,  5,  7,  5,  0, -5, -7, -5 };

// ============================================================================
// External Client Globals & Reconstructed Engine Helpers
// ============================================================================

extern D2UnitStrc*           gpPlayerUnit;                         // 1.12a: 0x6FBCC3D0
extern D2DrlgActStrc*        gpClientAct;                          // 1.12a: 0x6FBCC01C
extern D2RosterUnitStrc*     gpRosterUnitList;                     // 1.12a: 0x6FBCC080
extern D2UnitStrc*           gapClientSideUnitHashTables[6][128];  // 1.12a: 0x6FBC9D60
extern D2UnitStrc*           gapServerSideUnitHashTables[6][128];  // 1.12a: 0x6FBCA960
extern int32_t               gnViewportShiftOffsetX;               // 1.12a: 0x6FBCC1D4
extern int32_t               gnCameraScreenSkewX;                  // 1.12a: 0x6FBC21D0
extern int32_t               gnCameraScreenSkewY;                  // 1.12a: 0x6FBC21CC
extern uint32_t              gdwClientGameTick;                    // 1.12a: 0x6FBB32B0

// External D2Client functions defined in sibling reconstructed TUs
extern int32_t     __fastcall ENGINE_SafeMulDiv(int32_t nValue, int32_t nMul, int32_t nDiv); // 0x6FAB18D0 (GAME_Game.cpp)
extern D2ActiveRoomStrc* __fastcall DUNGEON_FindRoomBySubtile(D2ActiveRoomStrc* pHintRoom, int32_t nSubtileX, int32_t nSubtileY); // 0x6FAB1A00 (DRAW_dLightMap.cpp)
extern int32_t     __fastcall D2Client_GetUnitX_6FAB1590(D2UnitStrc* pUnit); // 0x6FAB1590 (D2ClientLinkBridge.cpp)
extern int32_t     __fastcall D2Client_GetUnitY_6FAB15C0(D2UnitStrc* pUnit); // 0x6FAB15C0 (D2ClientLinkBridge.cpp)
extern D2UnitStrc* __fastcall D2Client_CreateClientMissile(D2ClientMissileInitStrc* pInit); // 0x6FAD4B20 (UNIT_PlrSkills.cpp)
extern int32_t     __fastcall D2Client_GetUniqueClientUnitId(); // 0x6FACFE10 (D2ClientLinkBridge.cpp)
extern D2UnitStrc* __fastcall D2Client_AllocateClientUnit(int32_t nUnitType, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode, int32_t nUnitId, int32_t nArg7, int32_t dwFlags); // 0x6FAD16C0 (UNIT_PlrSkills.cpp)
extern void        __fastcall D2Client_RemoveUnitFromRoomAndFree(D2UnitStrc* pUnit); // 0x6FAD0690 (UNIT_PlrSkills.cpp)
extern D2UnitStrc* __fastcall D2Client_ClearActionFrameAndGetTarget(D2UnitStrc* pUnit); // 0x6FACF5B0 (UNIT_CUnit.cpp)
extern void        __stdcall  D2Client_FaceUnitTowardDynamicTarget(D2UnitStrc* pUnit, int32_t bForce); // 0x6FACF5F0 (UNIT_CUnit.cpp)
extern void        __fastcall D2Client_SetUnitDirectionAndStep(D2UnitStrc* pUnit, int32_t nDir); // 0x6FACF4C0 (UNIT_PlrSkills.cpp)
extern int32_t     __fastcall D2Client_DispatchUnitModeChange_6FAF1450(int32_t nMode, D2UnitStrc* pUnit, void* pModeArgs, int32_t bFromSkill); // 0x6FAF1450 (D2ClientLinkBridge.cpp)
extern void        __fastcall D2Client_ApplyMonsterShapeshiftMode(D2UnitStrc* pUnit, int32_t nMode); // 0x6FAF1E60 (UNIT_PlrSkills.cpp)
extern void        __fastcall D2Client_PlayUnitBlockOrAttackSwingSound(D2UnitStrc* pUnit); // 0x6FAE87B0 (UNIT_UnitMode.cpp)
extern void        __stdcall  D2Client_PlayUnitSound_6FAE1FA0(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nTicksDelay, int32_t bLoop, int32_t dwFlags); // 0x6FAE1FA0 (D2ClientLinkBridge.cpp)
extern void        __fastcall D2Client_PlaySoundAtCoords(int32_t nSoundId, int32_t nX, int32_t nY); // 0x6FAE7450 / 0x6FAE2020 (D2ClientLinkBridge.cpp)
extern void        __fastcall GFX_AttachOverlayToUnit(D2UnitStrc* pUnit, int32_t nOverlayId, int32_t bPreDrawOverride); // 0x6FB430E0 (ENGINE_Gfx.cpp)
extern void        __fastcall GFX_DrawUnit(D2UnitStrc* pUnit, uint32_t dwGamma, int32_t nXPos, int32_t nYPos, int32_t bDrawHighlight, int32_t nScreenMode); // 0x6FB449C0 (ENGINE_Gfx.cpp)
extern void        __fastcall VIEW_StartScreenShake(int32_t nAmplitude, int32_t nSustainTicks); // 0x6FB6A440 (View.cpp)
extern D2RosterUnitStrc* __fastcall ROSTER_FindPlayerByUnitId(uint32_t dwUnitId); // 0x6FB281C0 (GAME_Roster.cpp)
extern int32_t     __fastcall ROSTER_ArePlayersHostile(uint32_t dwUnitIdA, uint32_t dwUnitIdB); // 0x6FB287F0 (GAME_Roster.cpp)
extern uint32_t    __fastcall ROSTERPETS_FindOwnerIdForPet(uint32_t dwPetUnitId); // 0x6FB73520 (GAME_Roster.cpp)
extern int32_t     __fastcall SOUND_GetMonsterModeSoundId(void* pUnit, int32_t nAnimMode); // 0x6FB75F30 (DRAW_dLightMap.cpp)
extern void        __fastcall PARTICLE_AllocForUnit(void* pUnit); // 0x6FB75280 (DRAW_dLightMap.cpp)
extern BOOL        __fastcall PARTICLE_IsActiveOrCancelled(void* pUnit); // 0x6FB75340 (DRAW_dLightMap.cpp)
extern void        __fastcall PARTICLE_CancelFinished(void* pUnit); // 0x6FB75370 (DRAW_dLightMap.cpp)
extern BOOL        __fastcall PARTICLE_IsFinished(void* pUnit); // 0x6FB75390 (DRAW_dLightMap.cpp)

// External SKILLS_Skills.cpp core helpers (0x6FAFBA80..0x6FB02DB0)
extern int32_t     __fastcall CLTSKILL_AuraCallback_SelectNextRingTarget_Thunk(D2UnitStrc* pUnit, int32_t nArgEdx); // 0x6FAFDB00
extern int32_t     __fastcall CLTSKILL_FindUnitsInRadius(D2UnitStrc* pOrigin, int32_t nX, int32_t nY, int32_t nRadius, uint32_t dwFilterFlags, void* pfnCallback, void* pCallbackArg, int32_t nExtra); // 0x6FAFE9B0

// ============================================================================
// Typed Inline Accessors & Shared Micro-Helpers (Zero Raw Pointer Offset Casts)
// ============================================================================

static inline D2ClientUnitGfxInfoStrc* CLTSKILL_GetUnitClientGfx(D2UnitStrc* pUnit)
{
    return pUnit ? reinterpret_cast<D2ClientUnitGfxInfoStrc*>(pUnit->pGfxData) : nullptr;
}

static inline D2ParticleStrc* CLTSKILL_GetUnitParticle(D2UnitStrc* pUnit)
{
    D2ClientUnitGfxInfoStrc* pGfx = CLTSKILL_GetUnitClientGfx(pUnit);
    return pGfx ? pGfx->pParticle : nullptr;
}

static inline D2SkillsTxt* CLTSKILL_GetSkillsTxtRecord(int32_t nSkillId)
{
    if (!sgptDataTables || nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount || !sgptDataTables->pSkillsTxt)
    {
        return nullptr;
    }
    return &sgptDataTables->pSkillsTxt[nSkillId];
}

static inline D2MissilesTxt* CLTSKILL_GetMissilesTxtRecord(int32_t nMissileId)
{
    if (!sgptDataTables || nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount || !sgptDataTables->pMissilesTxt)
    {
        return nullptr;
    }
    return &sgptDataTables->pMissilesTxt[nMissileId];
}

static inline bool CLTSKILL_IsValidMissileId(int32_t nMissileId)
{
    return sgptDataTables && nMissileId >= 0 && nMissileId < sgptDataTables->nMissilesTxtRecordCount;
}

static inline D2MonStatsTxt* CLTSKILL_GetMonStatsTxtRecord(int32_t nMonsterId)
{
    if (!sgptDataTables || nMonsterId < 0 || nMonsterId >= sgptDataTables->nMonStatsTxtRecordCount || !sgptDataTables->pMonStatsTxt)
    {
        return nullptr;
    }
    return &sgptDataTables->pMonStatsTxt[nMonsterId];
}

static inline D2MonStats2Txt* CLTSKILL_GetMonStats2TxtRecordFromMonsterId(int32_t nMonsterId)
{
    D2MonStatsTxt* pMonStatsTxt = CLTSKILL_GetMonStatsTxtRecord(nMonsterId);
    if (!pMonStatsTxt)
    {
        return nullptr;
    }
    const int32_t nMonStatsEx = static_cast<int16_t>(pMonStatsTxt->wMonStatsEx);
    if (nMonStatsEx < 0 || nMonStatsEx >= sgptDataTables->nMonStats2TxtRecordCount || !sgptDataTables->pMonStats2Txt)
    {
        return nullptr;
    }
    return &sgptDataTables->pMonStats2Txt[nMonStatsEx];
}

static inline int32_t CLTSKILL_GetBaseMonsterClassId(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return -1;
    }
    D2MonStatsTxt* pMonStatsTxt = CLTSKILL_GetMonStatsTxtRecord(pUnit->dwClassId);
    const int32_t nBaseId = pMonStatsTxt ? static_cast<int32_t>(pMonStatsTxt->nBaseId) : pUnit->dwClassId;
    if (!sgptDataTables || nBaseId < 0 || nBaseId >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        return -1;
    }
    return nBaseId;
}

static inline D2CharStatsTxt* CLTSKILL_GetCharStatsTxtRecord(int32_t nCharClassId)
{
    if (!sgptDataTables || nCharClassId < 0 || nCharClassId >= sgptDataTables->nCharStatsTxtRecordCount || !sgptDataTables->pCharStatsTxt)
    {
        return nullptr;
    }
    return &sgptDataTables->pCharStatsTxt[nCharClassId];
}

static inline int32_t CLTSKILL_GetUnitXInline(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }
    if (pUnit->dwUnitType == UNIT_OBJECT || (pUnit->dwUnitType > UNIT_MISSILE && pUnit->dwUnitType <= UNIT_TILE))
    {
        return pUnit->pStaticPath ? pUnit->pStaticPath->nXPos : 0;
    }
    return pUnit->pDynamicPath ? PATH_GetXPosition(pUnit->pDynamicPath) : 0;
}

static inline int32_t CLTSKILL_GetUnitYInline(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }
    if (pUnit->dwUnitType == UNIT_OBJECT || (pUnit->dwUnitType > UNIT_MISSILE && pUnit->dwUnitType <= UNIT_TILE))
    {
        return pUnit->pStaticPath ? pUnit->pStaticPath->nYPos : 0;
    }
    return pUnit->pDynamicPath ? PATH_GetYPosition(pUnit->pDynamicPath) : 0;
}

static inline D2UnitStrc* CLTSKILL_FindServerUnit(int32_t nUnitType, uint32_t dwUnitId)
{
    if (nUnitType < 0 || nUnitType >= 6)
    {
        return nullptr;
    }
    for (D2UnitStrc* pCurr = gapServerSideUnitHashTables[nUnitType][dwUnitId & 0x7Fu]; pCurr != nullptr; pCurr = pCurr->pListNext)
    {
        if (pCurr->dwUnitType == static_cast<uint32_t>(nUnitType) && pCurr->dwUnitId == dwUnitId)
        {
            return pCurr;
        }
    }
    return nullptr;
}

// Mirrors 0x6FB00830: resolves wCltMissileA, or wCltMissileB/C/D when progressive charge state is active
static inline int32_t CLTSKILL_ResolveSkillClientMissile(D2UnitStrc* pUnit, int32_t nSkillId)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return -1;
    }

    if ((pSkillsTxt->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_PROGRESSIVE]) != 0)
    {
        const int32_t nAuraState = pSkillsTxt->nAuraState;
        const int32_t nPrgStat = static_cast<int16_t>(pSkillsTxt->wAuraStat[0]);
        if (nAuraState >= 0 && nAuraState < sgptDataTables->nStatesTxtRecordCount
            && nPrgStat >= 0 && nPrgStat < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nAuraState);
            if (pStatList)
            {
                int32_t nCharges = STATLIST_GetStatValue(pStatList, nPrgStat, 0);
                if (nCharges <= 1)
                {
                    return static_cast<int16_t>(pSkillsTxt->wCltMissileB);
                }
                if (nCharges >= 3)
                {
                    return static_cast<int16_t>(pSkillsTxt->wCltMissileD);
                }
                return static_cast<int16_t>(pSkillsTxt->wCltMissileC);
            }
        }
    }

    return static_cast<int16_t>(pSkillsTxt->wCltMissileA);
}

// Mirrors 0x6FAFE320: resolves dynamic path target unit coordinates or static target coordinates
static inline BOOL CLTSKILL_GetUnitTargetCoords(D2UnitStrc* pUnit, int32_t* pOutX, int32_t* pOutY)
{
    if (!pUnit || !pUnit->pDynamicPath || !pOutX || !pOutY)
    {
        return FALSE;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (pTarget)
    {
        *pOutX = CLTSKILL_GetUnitXInline(pTarget);
        *pOutY = CLTSKILL_GetUnitYInline(pTarget);
    }
    else
    {
        *pOutX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
        *pOutY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
    }

    if (*pOutX == 0 || *pOutY == 0)
    {
        int32_t nTargetType = 0;
        D2UnitGUID dwTargetGUID = 0;
        PATH_GetTargetTypeAndGUID(pUnit->pDynamicPath, &nTargetType, &dwTargetGUID);
        *pOutX = CLTSKILL_GetUnitXInline(pUnit);
        *pOutY = CLTSKILL_GetUnitYInline(pUnit);
    }
    return TRUE;
}

// Mirrors 0x6FAD1940: creates a stationary client missile at (nX, nY)
static inline D2UnitStrc* CLTSKILL_CreateMissileAtCoords(
    D2UnitStrc* pOwner,
    int32_t nMissileId,
    int32_t nX,
    int32_t nY,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    if (!pOwner)
    {
        return nullptr;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x01;
    tInit.pOwner = pOwner;
    tInit.nMissileId = nMissileId;
    tInit.nX = nX;
    tInit.nY = nY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    return D2Client_CreateClientMissile(&tInit);
}

// Mirrors 0x6FAD1E80: creates a directional client missile launched from pOrigin toward (nTargetX, nTargetY)
static inline D2UnitStrc* CLTSKILL_CreateMissileFromUnitTowardCoords(
    D2UnitStrc* pOrigin,
    int32_t nMissileId,
    int32_t nTargetX,
    int32_t nTargetY,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    if (!pOrigin)
    {
        return nullptr;
    }

    D2UnitStrc* pOwner = pOrigin;
    if (pOrigin->dwUnitType != UNIT_PLAYER && pOrigin->dwUnitType != UNIT_MONSTER)
    {
        pOwner = CLTSKILL_FindServerUnit(static_cast<int32_t>(pOrigin->dwOwnerType), pOrigin->dwOwnerGUID);
        if (!pOwner)
        {
            return nullptr;
        }
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x20;
    tInit.pOwner = pOwner;
    tInit.pOrigin = pOrigin;
    tInit.nMissileId = nMissileId;
    tInit.nTargetX = nTargetX;
    tInit.nTargetY = nTargetY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    return D2Client_CreateClientMissile(&tInit);
}

// Mirrors 0x6FAFE690: spawns a stationary missile at pUnit's target coordinates if within 100 subtiles
static inline D2UnitStrc* CLTSKILL_CreateMissileAtUnitTarget(
    D2UnitStrc* pUnit,
    int32_t nMissileId,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY) || nTargetX == 0 || nTargetY == 0)
    {
        return nullptr;
    }

    const int32_t nUnitX = D2Client_GetUnitX_6FAB1590(pUnit);
    const int32_t nUnitY = D2Client_GetUnitY_6FAB15C0(pUnit);
    const int32_t nDx = nTargetX - nUnitX;
    const int32_t nDy = nTargetY - nUnitY;
    if ((nDx * nDx + nDy * nDy) > 10000)
    {
        return nullptr;
    }

    return CLTSKILL_CreateMissileAtCoords(pUnit, nMissileId, nTargetX, nTargetY, nSkillId, nSkillLevel);
}

// Mirrors 0x6FAFE710: spawns a missile from pUnit toward pUnit's current target coordinates
static inline D2UnitStrc* CLTSKILL_CreateMissileTowardUnitTarget(
    D2UnitStrc* pUnit,
    int32_t nMissileId,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY) || nTargetX == 0 || nTargetY == 0)
    {
        return nullptr;
    }

    return CLTSKILL_CreateMissileFromUnitTowardCoords(pUnit, nMissileId, nTargetX, nTargetY, nSkillId, nSkillLevel);
}

// Mirrors 0x6FB00190: transitions a player in melee range from Charge/LeapAttack into standard attack mode
static inline int32_t CLTSKILL_DispatchMeleeFallbackOnTarget(D2UnitStrc* pUnit, D2UnitStrc* pTarget)
{
    int32_t aModeArgs[7] = {};
    aModeArgs[1] = -1;
    aModeArgs[2] = pTarget ? static_cast<int32_t>(pTarget->dwUnitType) : 6;
    aModeArgs[4] = pTarget ? static_cast<int32_t>(pTarget->dwUnitId) : -1;
    aModeArgs[5] = 1;
    D2Client_DispatchUnitModeChange_6FAF1450(22, pUnit, aModeArgs, 1);
    return 1;
}

// Mirrors 0x6FB00370: attaches the movement-skill cancel state listener to pUnit's stat list
extern void __fastcall CLTSKILL_StatRemove_PostAttackSkillEnd(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList);
static inline void CLTSKILL_AttachMovementSkillCancelStatList(D2UnitStrc* pUnit)
{
    const int32_t nOwnerType = pUnit ? static_cast<int32_t>(pUnit->dwUnitType) : 6;
    const D2UnitGUID dwOwnerGUID = pUnit ? pUnit->dwUnitId : static_cast<D2UnitGUID>(-1);
    D2StatListStrc* pStatList = STATLIST_AllocStatList(nullptr, 6, 50, nOwnerType, dwOwnerGUID);
    if (pStatList)
    {
        STATLIST_SetStatRemoveCallback(pStatList, CLTSKILL_StatRemove_PostAttackSkillEnd);
        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, TRUE);
    }
}

// ============================================================================
// SECTION 1: Paladin Client Skills (`..\Source\D2Client\SKILLS\SkillsPal.cpp`)
// VA Range: 0x6FB373F0 .. 0x6FB38077 (10 procedures)
// ============================================================================

// 1.12a: 0x6FB373F0
void __fastcall CLTSKILL_CancelChargeCallback(D2UnitStrc* pUnit, int32_t nSkillId)
{
    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    const uint32_t dwFlags = SKILLS_GetFlags(pSkill);
    if ((dwFlags & 0x01) != 0)
    {
        SKILLS_SetFlags(pSkill, dwFlags | 0x02);
    }
    STATES_ToggleState(pUnit, STATE_UNINTERRUPTABLE, FALSE);
}

// 1.12a: 0x6FB37420
int32_t __fastcall CLTSKILL_GetSeqFirstZeroEventFrame(D2UnitStrc* pUnit)
{
    D2AnimSeqTxt* pMonSeqTxt = DATATBLS_GetMonSeqTxtRecordFromUnit(pUnit);
    if (pMonSeqTxt)
    {
        const int32_t nSeqFrameCount = DATATBLS_GetSeqFrameCount(pUnit);
        for (int32_t nFrame = 0; nFrame < nSeqFrameCount; ++nFrame)
        {
            if (pMonSeqTxt->nEvent == ANIMSEQ_EVENT_NONE)
            {
                return nFrame - 1;
            }
        }
    }
    return 7;
}

// 1.12a: 0x6FB37460
void __fastcall CLTSKILL_DrawChargeMotionTrail(D2UnitStrc* pUnit, uint32_t dwGamma, int32_t bAfterUnit)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return;
    }

    const int32_t nBaseScreenX = UNITS_GetClientCoordX(pUnit);
    const int32_t nBaseScreenY = UNITS_GetClientCoordY(pUnit);

    int32_t nVelX = 0;
    int32_t nVelY = 0;
    PATH_GetClientCoordsVelocity(pUnit, &nVelX, &nVelY);

    if (bAfterUnit != 0)
    {
        if (nVelY < 0)
        {
            return;
        }
    }
    else
    {
        if (nVelY >= 0)
        {
            return;
        }
    }

    const int32_t nAbsVelX = (nVelX < 0) ? -nVelX : nVelX;
    const int32_t nAbsVelY = (nVelY < 0) ? -nVelY : nVelY;
    int32_t nApproxSpeed = ((nAbsVelX > nAbsVelY) ? (nAbsVelY + 2 * nAbsVelX) : (nAbsVelX + 2 * nAbsVelY)) / 2;
    if (nApproxSpeed == 0)
    {
        nApproxSpeed = 1;
    }

    const int32_t nStepX = (nVelX * 15) / nApproxSpeed;
    const int32_t nStepY = (nVelY * 15) / nApproxSpeed;
    const BOOL bPerspective = D2GFX_CheckPerspective();
    const int32_t nSavedFrame = pUnit->nSeqCurrentFramePrecise >> 8;
    int32_t nRunningFrame = nSavedFrame;

    const int32_t nTrailCount = SKILLS_GetParam3(pSkill);
    if (nTrailCount > 0)
    {
        int32_t nForwardOffsetX = nStepX;
        int32_t nForwardOffsetY = nStepY;
        int32_t nBackwardOffsetX = nTrailCount * nStepX;
        int32_t nBackwardOffsetY = nTrailCount * nStepY;

        for (int32_t i = nTrailCount; i > 0; --i)
        {
            --nRunningFrame;
            if (nRunningFrame < 0)
            {
                nRunningFrame += 8;
            }
            pUnit->nSeqCurrentFramePrecise = nRunningFrame << 8;

            int32_t nDrawX = 0;
            int32_t nDrawY = 0;
            if (bPerspective)
            {
                int32_t nPerspX = 0;
                int32_t nPerspY = 0;
                SCALE_ScalePerspectivePosition(
                    UNITS_GetPrecisionX(pUnit),
                    UNITS_GetPrecisionY(pUnit),
                    96,
                    &nPerspX,
                    &nPerspY,
                    0);
                nDrawX = nPerspX + (gnCameraScreenSkewX - gnViewportShiftOffsetX);
                nDrawY = nPerspY + gnCameraScreenSkewY - 8;
            }
            else
            {
                nDrawX = nBaseScreenX;
                nDrawY = nBaseScreenY;
            }

            if (bAfterUnit != 0)
            {
                nDrawX -= nBackwardOffsetX;
                nDrawY -= nBackwardOffsetY;
            }
            else
            {
                nDrawX -= nForwardOffsetX;
                nDrawY -= nForwardOffsetY;
            }

            GFX_DrawUnit(pUnit, dwGamma, nDrawX, nDrawY, 1, 0);

            nForwardOffsetX += nStepX;
            nForwardOffsetY += nStepY;
            nBackwardOffsetX -= nStepX;
            nBackwardOffsetY -= nStepY;
        }
    }

    pUnit->nSeqCurrentFramePrecise = nSavedFrame << 8;
}

// 1.12a: 0x6FB37650 (gpfnCltDoFunc[34])
int32_t __fastcall CLTSKILL_Do_BlessedHammer(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    if (!D2Client_ClearActionFrameAndGetTarget(pUnit) || !pUnit->pDynamicPath)
    {
        return 0;
    }

    int32_t nDirOctant = 0;
    if (PATH_GetVelocity(pUnit->pDynamicPath) != 0)
    {
        nDirOctant = UNITS_GetDirection(pUnit) >> 3;
    }
    else
    {
        nDirOctant = SEED_RollLimitedRandomNumber(&pUnit->pSeed, 8);
    }

    const int32_t nJitter = SEED_RollLimitedRandomNumber(&pUnit->pSeed, 3);
    const int32_t nWrappedOpposite = (nDirOctant + 4) & 7;
    const int32_t nFinalDir = (nWrappedOpposite + nJitter - 1) & 7;

    const int32_t nTargetX = D2Client_GetUnitX_6FAB1590(pUnit) + g_aBlessedHammerOffsetX_6FB84598[nFinalDir];
    const int32_t nTargetY = D2Client_GetUnitY_6FAB15C0(pUnit) + g_aBlessedHammerOffsetY_6FB84578[nFinalDir];

    const int32_t nMissileRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);
    if (nMissileRange > 0)
    {
        nMissileId += SEED_RollLimitedRandomNumber(&pUnit->pSeed, nMissileRange);
    }

    CLTSKILL_CreateMissileFromUnitTowardCoords(pUnit, nMissileId, nTargetX, nTargetY, nSkillId, nSkillLevel);
    return 1;
}

// 1.12a: 0x6FB377A0
D2UnitStrc* __fastcall CLTSKILL_FindChargeReplacementTarget(D2UnitStrc* pUnit, uint32_t dwPrevTargetId)
{
    D2ChargeTargetFindArgStrc tFindArg = {};
    tFindArg.pNextHigherUnit = nullptr;
    tFindArg.dwMinHigherId = 0xFFFFFFFFu;
    tFindArg.pLowestUnit = nullptr;
    tFindArg.dwLowestId = dwPrevTargetId;
    tFindArg.dwPrevTargetId = dwPrevTargetId;
    tFindArg.nVisitedCount = 0;

    CLTSKILL_FindUnitsInRadius(
        pUnit,
        0,
        0,
        3,
        0xA283u,
        reinterpret_cast<void*>(CLTSKILL_AuraCallback_SelectNextRingTarget_Thunk),
        &tFindArg,
        0);

    return tFindArg.pNextHigherUnit ? tFindArg.pNextHigherUnit : tFindArg.pLowestUnit;
}

// 1.12a: 0x6FB37800 (gpfnCltDoFunc[37])
int32_t __fastcall CLTSKILL_Do_Charge(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const uint32_t dwSkillFlags = SKILLS_GetFlags(pSkill);

    int32_t nSeqFrameCount = 15;
    if (DATATBLS_GetMonSeqTxtRecordFromUnit(pUnit) != nullptr)
    {
        nSeqFrameCount = DATATBLS_GetSeqFrameCount(pUnit);
    }

    const int32_t nLoopFrame = CLTSKILL_GetSeqFirstZeroEventFrame(pUnit);

    if (pUnit && (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER) && pUnit->nActionFrame == 3)
    {
        D2Client_PlayUnitBlockOrAttackSwingSound(pUnit);
        return 0;
    }

    const int32_t nTargetType = SKILLS_GetParam1(pSkill);
    const uint32_t dwTargetGUID = static_cast<uint32_t>(SKILLS_GetParam2(pSkill));
    D2UnitStrc* pTarget = nullptr;
    if (nTargetType != 6)
    {
        pTarget = CLTSKILL_FindServerUnit(nTargetType, dwTargetGUID);
    }

    if ((dwSkillFlags & 0x01) != 0)
    {
        if (pTarget && UNITS_IsInMeleeRange(pUnit, pTarget, 0))
        {
            SKILLS_SetFlags(pSkill, 0);
            UNITS_SetAnimationFrame(pUnit, nLoopFrame + 1);
            pUnit->dwFrameCountPrecise = nSeqFrameCount << 8;
            return 1;
        }

        if ((dwSkillFlags & 0x02) != 0)
        {
            pTarget = CLTSKILL_FindChargeReplacementTarget(pUnit, dwTargetGUID);
            if (pTarget && UNITS_IsInMeleeRange(pUnit, pTarget, 0))
            {
                SKILLS_SetFlags(pSkill, 0);
                SKILLS_SetParam1(pSkill, static_cast<int32_t>(pTarget->dwUnitType));
                SKILLS_SetParam2(pSkill, static_cast<int32_t>(pTarget->dwUnitId));
                return 1;
            }

            SKILLS_SetFlags(pSkill, 0);
            if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
            {
                const int32_t nBaseId = CLTSKILL_GetBaseMonsterClassId(pUnit);
                if (nBaseId == MONSTER_REANIMATEDHORDE1 || nBaseId == MONSTER_SIEGEBEAST1)
                {
                    return 1;
                }
            }

            pUnit->dwFrameCountPrecise = 0x100;
            return 1;
        }

        if ((pUnit->dwSeqFrame >> 8) >= nLoopFrame)
        {
            UNITS_SetAnimationFrame(pUnit, 0);
            pUnit->dwFrameCountPrecise = nSeqFrameCount << 8;
            SKILLS_SetFlags(pSkill, 0x01);
        }

        pUnit->dwFlags &= ~UNITFLAG_SKSRVDOFUNC;
        int32_t nTrailLength = SKILLS_GetParam3(pSkill);
        if (nTrailLength < 2)
        {
            ++nTrailLength;
        }
        SKILLS_SetParam3(pSkill, nTrailLength);
        return 1;
    }

    SKILLS_SetFlags(pSkill, 0);
    SKILLS_SetParam1(pSkill, 0);
    SKILLS_SetParam2(pSkill, 0);
    pUnit->dwFrameCountPrecise = (nSeqFrameCount - (pUnit->dwSeqFrame >> 8)) << 8;

    if (!pTarget)
    {
        pTarget = CLTSKILL_FindChargeReplacementTarget(pUnit, 0xFFFFFFFFu);
        if (!pTarget)
        {
            return 0;
        }
    }

    D2Common_10216(
        pUnit->pDynamicPath,
        D2Client_GetUnitX_6FAB1590(pTarget),
        D2Client_GetUnitY_6FAB15C0(pTarget),
        0);
    return 1;
}

// 1.12a: 0x6FB37A80 (gpfnCltDoFunc[86])
int32_t __fastcall CLTSKILL_Do_HolyFreeze_SanctuaryAura(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    const int32_t nRadius = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel);
    if (nRadius <= 0)
    {
        return 0;
    }

    const int32_t nBaseRadius = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, 1);
    if (nBaseRadius <= 0)
    {
        return 0;
    }

    const int32_t nRadiusSq = nRadius * nRadius;
    const int32_t nRandDiv = SEED_RollLimitedRandomNumber(&pUnit->pSeed, 18) + 1;
    const int32_t nSpawnCount = (nRadiusSq / (nBaseRadius * nBaseRadius)) / nRandDiv;

    const int32_t nUnitX = D2Client_GetUnitX_6FAB1590(pUnit);
    const int32_t nUnitY = D2Client_GetUnitY_6FAB15C0(pUnit);

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x2001;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    for (int32_t i = 0; i < nSpawnCount; ++i)
    {
        const int32_t nDiameter = 2 * nRadius + 1;
        const int32_t nOffsetX = SEED_RollLimitedRandomNumber(&pUnit->pSeed, nDiameter) - nRadius;
        const int32_t nOffsetY = SEED_RollLimitedRandomNumber(&pUnit->pSeed, nDiameter) - nRadius;

        if ((nOffsetX * nOffsetX + nOffsetY * nOffsetY) < nRadiusSq)
        {
            tInit.nX = nUnitX + nOffsetX + 2;
            tInit.nY = nUnitY + nOffsetY + 2;
            D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
            if (pMissile)
            {
                const uint8_t nRandDir = static_cast<uint8_t>(SEED_RollLimitedRandomNumber(&pUnit->pSeed, 64));
                PATH_SetDirection(pMissile->pDynamicPath, nRandDir);
            }
        }
    }

    return 1;
}

// 1.12a: 0x6FB37C50 (gpfnCltDoFunc[36])
int32_t __fastcall CLTSKILL_Do_FistOfTheHeavens(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pTarget = D2Client_ClearActionFrameAndGetTarget(pUnit);
    if (!pTarget)
    {
        return 0;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x01;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nX = D2Client_GetUnitX_6FAB1590(pTarget);
    tInit.nY = D2Client_GetUnitY_6FAB15C0(pTarget);
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
    if (!pMissile)
    {
        return 0;
    }

    MISSILE_SetTargetX(pMissile, static_cast<int32_t>(pTarget->dwUnitType));
    MISSILE_SetTargetY(pMissile, static_cast<int32_t>(pTarget->dwUnitId));
    return 1;
}

// 1.12a: 0x6FB37D60 (gpfnCltDoFunc[35])
int32_t __fastcall CLTSKILL_Do_HolyBolt_SmiteMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x20;
    tInit.pOwner = pUnit;
    tInit.pOrigin = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nTargetX = nTargetX;
    tInit.nTargetY = nTargetY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
    if (!pMissile || !pMissile->pDynamicPath)
    {
        return 0;
    }

    PATH_SetType(pMissile->pDynamicPath, PATHTYPE_CHARGEDBOLT);
    D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
    return 1;
}

// 1.12a: 0x6FB37E70 (gpfnCltStFunc[25])
int32_t __fastcall CLTSKILL_St_Charge(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    if (SKILLS_GetSkillIdFromSkill(pSkill, s_szSkillsPalFile, 374) == nSkillId
        && pUnit == gpPlayerUnit
        && (SKILLS_GetFlags(pSkill) & 0x01) != 0)
    {
        return 0;
    }

    D2DynamicPathStrc* pDynamicPath = pUnit->pDynamicPath;
    if (!pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = D2Client_ClearActionFrameAndGetTarget(pUnit);
    if (pUnit->dwUnitType == UNIT_PLAYER && pTarget && UNITS_IsInMeleeRange(pUnit, pTarget, 0))
    {
        return CLTSKILL_DispatchMeleeFallbackOnTarget(pUnit, pTarget);
    }

    int32_t nSoundId = 0;
    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        const int16_t nDoSoundA = static_cast<int16_t>(pSkillsTxt->wDoSoundA);
        if (nDoSoundA > 0)
        {
            nSoundId = nDoSoundA;
        }
    }
    else if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        const int32_t nMonSound = SOUND_GetMonsterModeSoundId(pUnit, static_cast<int32_t>(pUnit->dwAnimMode));
        if (nMonSound > 0)
        {
            nSoundId = nMonSound;
        }
    }

    if (nSoundId > 0)
    {
        D2Client_PlayUnitSound_6FAE1FA0(nSoundId, pUnit, 0, 0, 0);
    }

    if (pTarget && !STATES_CheckState(pTarget, STATE_UNINTERRUPTABLE))
    {
        SKILLS_SetParam1(pSkill, static_cast<int32_t>(pTarget->dwUnitType));
        SKILLS_SetParam2(pSkill, static_cast<int32_t>(pTarget->dwUnitId));
    }
    else
    {
        SKILLS_SetParam1(pSkill, 6);
        SKILLS_SetParam2(pSkill, 0);
    }

    int32_t nBaseSpeed = 0x100;
    SKILLS_SetParam4(pSkill, 7);

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        D2CharStatsTxt* pCharStatsTxt = CLTSKILL_GetCharStatsTxtRecord(pUnit->dwClassId);
        if (pCharStatsTxt)
        {
            nBaseSpeed = static_cast<int32_t>(static_cast<uint8_t>(pCharStatsTxt->nRunSpeed)) << 8;
        }
    }
    else
    {
        D2MonStatsTxt* pMonStatsTxt = CLTSKILL_GetMonStatsTxtRecord(pUnit->dwClassId);
        if (pMonStatsTxt)
        {
            nBaseSpeed = static_cast<int32_t>(pMonStatsTxt->nRun) << 8;
            if (pUnit->dwClassId == MONSTER_DURIEL)
            {
                SKILLS_SetParam4(pSkill, 15);
            }
            PATH_SetNewDistance(pDynamicPath, 30);
        }
    }

    int32_t nVelocityPercent = STATLIST_UnitGetStatValue(pUnit, STAT_VELOCITYPERCENT, 0);
    if (nVelocityPercent <= 50)
    {
        nVelocityPercent = 50;
    }

    const int32_t nChargeBonusPct = pSkillsTxt->dwParam[0];
    const int32_t nVelocity = ENGINE_SafeMulDiv(nVelocityPercent + nChargeBonusPct, nBaseSpeed, 100);
    PATH_SetVelocity(pDynamicPath, nVelocity, s_szSkillsPalFile, 458);
    D2Common_10142(pDynamicPath, pUnit, 0);

    if (pTarget)
    {
        D2Client_FaceUnitTowardDynamicTarget(pUnit, 1);
    }

    SKILLS_SetFlags(pSkill, 0x1001);
    SKILLS_SetParam3(pSkill, 0);
    CLTSKILL_AttachMovementSkillCancelStatList(pUnit);
    return 1;
}

// ============================================================================
// SECTION 2: Shared Multi-Hit & Monster Client Skills
// (`..\Source\D2Client\SKILLS\SkillsMon.cpp` & Shared Melee Sequence Handlers)
// VA Range: 0x6FB4C270 .. 0x6FB4E92E (41 procedures)
// ============================================================================

// 1.12a: 0x6FB4C270 (gpfnCltStFunc[41])
int32_t __fastcall CLTSKILL_St_ClearUsedSkillFlags(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    SKILLS_SetFlags(pSkill, 0);
    return 1;
}

// 1.12a: 0x6FB4C290 (gpfnCltDoFunc[52])
int32_t __fastcall CLTSKILL_Do_ClearActionFrameAndCheckMetricsA(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    int32_t nFrame = 0;
    int32_t nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrame, &nFrameCount);
    pUnit->nActionFrame = 0;
    if (nFrame <= 0)
    {
        pUnit->dwSeqSpeed = 0;
    }
    return 1;
}

// 1.12a: 0x6FB4C2D0 (gpfnCltDoFunc[47])
int32_t __fastcall CLTSKILL_Do_ClearActionFrameAndCheckMetricsB(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    int32_t nFrame = 0;
    int32_t nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrame, &nFrameCount);
    pUnit->nActionFrame = 0;
    if (nFrame <= 0)
    {
        pUnit->dwSeqSpeed = 0;
    }
    return 1;
}

// 1.12a: 0x6FB4C310 (gpfnCltDoFunc[46])
int32_t __fastcall CLTSKILL_Do_ResetSeqSpeed(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    pUnit->dwSeqSpeed = 0;
    return 1;
}

// 1.12a: 0x6FB4C320 (gpfnCltDoFunc[62])
int32_t __fastcall CLTSKILL_Do_MultiHitRollbackLoop(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nTargetType = SKILLS_GetParam3(pSkill);
    const uint32_t dwTargetGUID = static_cast<uint32_t>(SKILLS_GetParam2(pSkill));
    D2UnitStrc* pTarget = CLTSKILL_FindServerUnit(nTargetType, dwTargetGUID);
    if (!pTarget)
    {
        SKILLS_SetParam1(pSkill, 0);
        return 0;
    }

    if (!UNITS_IsInMeleeRange(pUnit, pTarget, 0))
    {
        return 0;
    }

    const int32_t nRemainingSwings = SKILLS_GetParam1(pSkill) - 1;
    if (nRemainingSwings <= 0)
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, nRemainingSwings);
    UNITS_SetAnimationFrame(pUnit, pSkillsTxt->dwParam[0]);
    pUnit->dwFrameCountPrecise = (pUnit->dwFrameCountPrecise & 0xFFFFFF00) + 0x100;
    return 1;
}

// 1.12a: 0x6FB4C3E0 (gpfnCltDoFunc[59])
int32_t __fastcall CLTSKILL_Do_DiabRun(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    int32_t nFrame = 0;
    int32_t nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrame, &nFrameCount);
    if (nFrame > pSkillsTxt->dwParam[3])
    {
        return 0;
    }

    D2DynamicPathStrc* pDynamicPath = pUnit->pDynamicPath;
    if (!pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pDynamicPath);
    if (pTarget && UNITS_GetDistanceToOtherUnit(pTarget, pUnit) <= 1)
    {
        SKILLS_SetFlags(pSkill, 0);
        pUnit->dwFrameCountPrecise = pSkillsTxt->dwParam[0] << 8;
        UNITS_SetAnimationFrame(pUnit, pSkillsTxt->dwParam[1]);
        return 1;
    }

    if ((SKILLS_GetFlags(pSkill) & 0x02) != 0)
    {
        SKILLS_SetFlags(pSkill, 0);
        pUnit->dwFrameCountPrecise = pSkillsTxt->dwParam[0] << 8;
        UNITS_SetAnimationFrame(pUnit, pSkillsTxt->dwParam[1]);
        return 1;
    }

    if (nFrame == pSkillsTxt->dwParam[2])
    {
        int32_t nSpeedPrecise = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel) << 8;
        if (nSpeedPrecise <= 0x100)
        {
            nSpeedPrecise = 0x100;
        }

        const int32_t nVelocityPct = STATLIST_UnitGetStatValue(pUnit, STAT_VELOCITYPERCENT, 0);
        const int32_t nVelocity = ENGINE_SafeMulDiv(nVelocityPct, nSpeedPrecise, 100);
        PATH_SetVelocity(pDynamicPath, nVelocity, s_szSkillsMonFile, 1126);
        PATH_SetType(pDynamicPath, PATHTYPE_STRAIGHT);
        D2Common_10142(pDynamicPath, pUnit, 0);
        SKILLS_SetFlags(pSkill, 0x01);
        return 1;
    }

    if (nFrame == pSkillsTxt->dwParam[3])
    {
        pUnit->dwFrameCountPrecise = pSkillsTxt->dwParam[4] << 8;
        UNITS_SetAnimationFrame(pUnit, pSkillsTxt->dwParam[5]);
    }

    return 1;
}

// 1.12a: 0x6FB4C580 (gpfnCltStFunc[40])
int32_t __fastcall CLTSKILL_St_DiabRun(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nResult = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);
    SKILLS_SetParam1(pSkill, nResult);
    return 1;
}

// 1.12a: 0x6FB4C5F0 (gpfnCltStFunc[38])
int32_t __fastcall CLTSKILL_St_Emerge(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (pUnit)
    {
        pUnit->dwFlags |= (UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED | UNITFLAG_ISVALIDTARGET);
    }
    return 1;
}

// 1.12a: 0x6FB4C610 (gpfnCltStFunc[37])
int32_t __fastcall CLTSKILL_St_Submerge_MaggotEgg(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (pUnit)
    {
        pUnit->dwFlags &= ~(UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED | UNITFLAG_ISVALIDTARGET);
    }
    return 1;
}

// 1.12a: 0x6FB4C630 (gpfnCltDoFunc[50])
int32_t __fastcall CLTSKILL_Do_Nest_EvilHutSpawner(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nSpawnX = SKILLS_GetParam2(pSkill);
    const int32_t nSpawnY = SKILLS_GetParam3(pSkill);
    if (nSpawnX == 0 || nSpawnY == 0)
    {
        return 0;
    }

    SKILLS_SetParam2(pSkill, 0);
    SKILLS_SetParam3(pSkill, 0);

    D2ActiveRoomStrc* pRoom = DUNGEON_FindRoomBySubtile(UNITS_GetRoom(pUnit), nSpawnX, nSpawnY);
    if (!pRoom)
    {
        return 0;
    }

    COLLISION_ResetMaskWithPattern(pRoom, nSpawnX, nSpawnY, 1, COLLIDE_MONSTER);
    return 1;
}

// 1.12a: 0x6FB4C6B0 (gpfnCltStFunc[36])
int32_t __fastcall CLTSKILL_St_Nest_EvilHutSpawner(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_GetSkillsTxtRecord(nSkillId))
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    int32_t nSpawnMode = 0;
    int32_t nSpawnX = 0;
    int32_t nSpawnY = 0;
    if (MONSTERS_GetSpawnMode_XY(pUnit, TRUE, nSkillId, nSkillLevel, &nSpawnMode, &nSpawnX, &nSpawnY) < 0)
    {
        return 0;
    }

    SKILLS_SetParam2(pSkill, nSpawnX);
    SKILLS_SetParam3(pSkill, nSpawnY);

    D2ActiveRoomStrc* pRoom = DUNGEON_FindRoomBySubtile(UNITS_GetRoom(pUnit), nSpawnX, nSpawnY);
    COLLISION_SetMaskWithPattern(pRoom, nSpawnX, nSpawnY, 1, COLLIDE_MONSTER);
    return 1;
}

// 1.12a: 0x6FB4C770 (gpfnCltDoFunc[49])
int32_t __fastcall CLTSKILL_Do_SwarmMove_QuickStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    uint32_t dwCalc = pSkillsTxt->dwCalc[0];
    if ((SKILLS_GetFlags(pSkill) & 0x02) != 0)
    {
        SKILLS_SetFlags(pSkill, 0);
        dwCalc = pSkillsTxt->dwCalc[1];
    }

    const int32_t nTargetFrame = SKILLS_EvaluateSkillFormula(pUnit, dwCalc, nSkillId, nSkillLevel);
    UNITS_SetAnimationFrame(pUnit, nTargetFrame);
    return 1;
}

// 1.12a: 0x6FB4C800 (gpfnCltStFunc[33])
int32_t __fastcall CLTSKILL_St_MaggotDown(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (pUnit)
    {
        pUnit->dwFlags &= ~(UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED | UNITFLAG_ISVALIDTARGET);
    }

    const int32_t nUnitY = CLTSKILL_GetUnitYInline(pUnit);
    const int32_t nUnitX = CLTSKILL_GetUnitXInline(pUnit);
    COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pUnit), nUnitX, nUnitY, 5, COLLIDE_CORPSE);
    return 1;
}

// 1.12a: 0x6FB4C890
void __fastcall CLTSKILL_InitFingerMageSpiderMissile(D2UnitStrc* pMissile, int32_t nInitSeed)
{
    if (!pMissile)
    {
        return;
    }

    int32_t nTotalFrames = MISSILE_GetTotalFrames(pMissile);
    if (nTotalFrames >= 78)
    {
        nTotalFrames = 77;
        MISSILE_SetTotalFrames(pMissile, 77);
        MISSILE_SetCurrentFrame(pMissile, 77);
    }

    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    const int32_t nFirstX = D2COMMON_10175_PathGetFirstPointX(pPath);
    SEED_InitLowSeed(&pMissile->pSeed, nFirstX + nInitSeed);

    if (SEED_RollLimitedRandomNumber(&pMissile->pSeed, 100) >= 20)
    {
        PATH_SetType(pPath, PATHTYPE_CHARGEDBOLT);
        PATH_SetNewDistance(pPath, nTotalFrames);
        D2Common_10142(pPath, pMissile, 0);
    }
}

// 1.12a: 0x6FB4C930
void __fastcall CLTSKILL_AdvanceInfernoAnimFrame(D2UnitStrc* pUnit, D2SkillsTxt* pSkillsTxt)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        UNITS_SetAnimationFrame(pUnit, 8);
        pUnit->dwFrameCountPrecise = 0x500;
        return;
    }

    D2MonStats2Txt* pMonStats2Txt = CLTSKILL_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
    if (!pMonStats2Txt)
    {
        return;
    }

    if (pSkillsTxt->nMonAnim != MONMODE_SEQUENCE)
    {
        UNITS_SetAnimationFrame(pUnit, static_cast<int32_t>(pMonStats2Txt->nInfernoAnim) - 1);
        pUnit->dwFrameCountPrecise = (static_cast<int32_t>(pMonStats2Txt->nInfernoAnim) + 1) << 8;
    }
    else
    {
        pUnit->nSeqCurrentFramePrecise = (static_cast<int32_t>(pMonStats2Txt->nInfernoAnim) << 8) - 0x100;
    }
}

// 1.12a: 0x6FB4C9A0 (gpfnCltStFunc[39])
int32_t __fastcall CLTSKILL_St_MonInferno(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (!pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    if (!PATH_GetTargetUnit(pUnit->pDynamicPath))
    {
        return 0;
    }

    pUnit->dwFlagEx &= ~UNITFLAGEX_NODRAW;
    return 1;
}

// 1.12a: 0x6FB4CA00
int32_t __fastcall CLTSKILL_ConfigureFlameMissileRangeAndSpeed(
    D2UnitStrc* pOwner,
    D2UnitStrc* pMissile,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt || !pMissile)
    {
        return 0;
    }

    D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxt)
    {
        return 0;
    }

    int32_t nRange = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    if (nRange <= 0)
    {
        nRange = static_cast<int32_t>(pMissilesTxt->wRange) + static_cast<int32_t>(pMissilesTxt->wLevRange) * (nSkillLevel - 1);
    }

    if (nRange <= 1)
    {
        nRange = 1;
    }
    else if (nRange >= 255)
    {
        nRange = 255;
    }

    PATH_SetNewDistance(pMissile->pDynamicPath, nRange);

    const int32_t nTotalAnimFramesPrecise = static_cast<int32_t>(pMissilesTxt->nAnimLen) << 8;
    UNITS_SetAnimationSpeed(pMissile, nTotalAnimFramesPrecise / nRange);
    MISSILE_SetTotalFrames(pMissile, nRange);
    MISSILE_SetCurrentFrame(pMissile, nRange);
    return 1;
}

// 1.12a: 0x6FB4CAD0 (gpfnCltStFunc[35])
int32_t __fastcall CLTSKILL_St_SwarmMove_QuickStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (!pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    if (!PATH_GetTargetUnit(pUnit->pDynamicPath))
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    D2DynamicPathStrc* pDynamicPath = pUnit->pDynamicPath;
    if (!pDynamicPath)
    {
        return 0;
    }

    PATH_SetNewDistance(pDynamicPath, 5);
    PATH_SetType(pDynamicPath, PATHTYPE_TOWARD);
    D2Common_10142(pDynamicPath, pUnit, 0);
    if (PATH_GetNumberOfPathPoints(pDynamicPath) <= 0)
    {
        PATH_SetType(pDynamicPath, PATHTYPE_STRAIGHT);
        D2Common_10142(pDynamicPath, pUnit, 0);
        if (PATH_GetNumberOfPathPoints(pDynamicPath) <= 0)
        {
            return 0;
        }
    }

    SKILLS_SetFlags(pSkill, 0x01);
    return 1;
}

// 1.12a: 0x6FB4CB90 (gpfnCltDoFunc[48])
int32_t __fastcall CLTSKILL_Do_AndarielSpray(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    int32_t nTargetX = SKILLS_GetParam1(pSkill);
    int32_t nTargetY = SKILLS_GetParam2(pSkill);
    if (nTargetX == 0 || nTargetY == 0)
    {
        if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
        {
            return 0;
        }
    }

    const uint8_t nDir64 = UNITS_GetDirectionToCoords(pUnit, nTargetX, nTargetY);
    const uint32_t nDirOctant = D2Common_11053(nDir64) & 7u;

    int32_t nAimX = D2Client_GetUnitX_6FAB1590(pUnit);
    int32_t nAimY = D2Client_GetUnitY_6FAB15C0(pUnit);

    int32_t nVecX = 0;
    int32_t nVecY = 0;
    D2Common_11055(g_aAndarielBaseDir_6FB84080[nDirOctant], &nVecX, &nVecY);
    nAimX += nVecX;
    nAimY += nVecY;

    int32_t nFrame = 0;
    int32_t nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrame, &nFrameCount);

    int32_t nSprayIdx = nFrame - 4;
    if (nSprayIdx <= 0)
    {
        nSprayIdx = 0;
    }
    else if (nSprayIdx >= 8)
    {
        nSprayIdx = 8;
    }

    const uint8_t nSprayDir = g_aAndarielSprayDirs_6FB84088[nDirOctant][nSprayIdx];
    if (nSprayDir != 99)
    {
        D2Common_11055(nSprayDir, &nVecX, &nVecY);
        nAimX += nVecX;
        nAimY += nVecY;
    }

    CLTSKILL_CreateMissileFromUnitTowardCoords(pUnit, nMissileId, nAimX, nAimY, nSkillId, nSkillLevel);
    return 1;
}

// 1.12a: 0x6FB4CD30 (gpfnCltStFunc[34])
int32_t __fastcall CLTSKILL_St_AndarielSpray(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (!pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, CLTSKILL_GetUnitXInline(pTarget));
    SKILLS_SetParam2(pSkill, CLTSKILL_GetUnitYInline(pTarget));
    return 1;
}

// 1.12a: 0x6FB4CE10 (gpfnCltStFunc[32])
int32_t __fastcall CLTSKILL_St_MaggotUp(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    UNITS_SetAnimStartFrame(pUnit);
    if (pUnit)
    {
        pUnit->dwFlags |= (UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED | UNITFLAG_ISVALIDTARGET);
    }

    int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (nMissileId > 0 && CLTSKILL_IsValidMissileId(nMissileId))
    {
        const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit));
        if (DRLG_GetActNoFromLevelId(nLevelId) == ACT_II)
        {
            const int32_t nAct2Missile = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
            if (nAct2Missile > 0 && CLTSKILL_IsValidMissileId(nAct2Missile))
            {
                nMissileId = nAct2Missile;
            }
        }

        CLTSKILL_CreateMissileFromUnitTowardCoords(
            pUnit,
            nMissileId,
            D2Client_GetUnitX_6FAB1590(pUnit),
            D2Client_GetUnitY_6FAB15C0(pUnit),
            nSkillId,
            nSkillLevel);
    }

    D2CoordStrc tCoords = {};
    tCoords.nX = CLTSKILL_GetUnitXInline(pUnit);
    tCoords.nY = CLTSKILL_GetUnitYInline(pUnit);

    D2ActiveRoomStrc* pFreeRoom = COLLISION_GetFreeCoordinates(
        UNITS_GetRoom(pUnit),
        &tCoords,
        UNITS_GetUnitSizeX(pUnit),
        0x1C09u,
        TRUE);
    if (!pFreeRoom || !pUnit->pDynamicPath)
    {
        return 0;
    }

    if (!D2Common_10231(pUnit->pDynamicPath, pUnit, pFreeRoom, tCoords.nX, tCoords.nY))
    {
        return 0;
    }

    COLLISION_SetMaskWithPattern(
        UNITS_GetRoom(pUnit),
        D2Client_GetUnitX_6FAB1590(pUnit),
        D2Client_GetUnitY_6FAB15C0(pUnit),
        1,
        COLLIDE_CORPSE);
    return 1;
}

// 1.12a: 0x6FB4CFB0 (gpfnCltStFunc[44])
int32_t __fastcall CLTSKILL_St_HellMeteor(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pFallingMeteorA = CLTSKILL_CreateMissileAtUnitTarget(pUnit, 0x150, nSkillId, nSkillLevel);
    D2UnitStrc* pFallingMeteorB = CLTSKILL_CreateMissileAtUnitTarget(pUnit, 0x151, nSkillId, nSkillLevel);

    D2UnitStrc* pLaunchA = CLTSKILL_CreateMissileAtCoords(
        pUnit,
        0x14F,
        CLTSKILL_GetUnitXInline(pUnit),
        CLTSKILL_GetUnitYInline(pUnit),
        nSkillId,
        nSkillLevel);

    D2UnitStrc* pLaunchB = CLTSKILL_CreateMissileAtCoords(
        pUnit,
        0x151,
        CLTSKILL_GetUnitXInline(pUnit),
        CLTSKILL_GetUnitYInline(pUnit),
        nSkillId,
        nSkillLevel);

    D2UnitStrc* pSmokeA = CLTSKILL_CreateMissileAtCoords(
        pUnit,
        0x14E,
        CLTSKILL_GetUnitXInline(pUnit),
        CLTSKILL_GetUnitYInline(pUnit),
        nSkillId,
        nSkillLevel);

    D2UnitStrc* pSmokeB = CLTSKILL_CreateMissileAtCoords(
        pUnit,
        0x14D,
        CLTSKILL_GetUnitXInline(pUnit),
        CLTSKILL_GetUnitYInline(pUnit),
        nSkillId,
        nSkillLevel);

    D2ParticleStrc* pLaunchAParticle = CLTSKILL_GetUnitParticle(pLaunchA);
    if (pLaunchAParticle)
    {
        pLaunchAParticle->nVelX = 0;
        pLaunchAParticle->nVelY = 0;
        pLaunchAParticle->nVelZ = 0x7800;
    }

    D2ParticleStrc* pLaunchBParticle = CLTSKILL_GetUnitParticle(pLaunchB);
    if (pLaunchBParticle)
    {
        pLaunchBParticle->nVelX = 0;
        pLaunchBParticle->nVelY = 0;
        pLaunchBParticle->nVelZ = 0x7800;
    }

    D2ParticleStrc* pFallingAParticle = CLTSKILL_GetUnitParticle(pFallingMeteorA);
    if (pFallingAParticle)
    {
        pFallingAParticle->nPosX = 0;
        pFallingAParticle->nPosY = 0;
        pFallingAParticle->nPosZ = 0x00AFC800;
        pFallingAParticle->nVelX = 0;
        pFallingAParticle->nVelY = 0;
        pFallingAParticle->nVelZ = -0xC800;
    }

    D2ParticleStrc* pFallingBParticle = CLTSKILL_GetUnitParticle(pFallingMeteorB);
    if (pFallingBParticle)
    {
        pFallingBParticle->nPosX = 0;
        pFallingBParticle->nPosY = 0;
        pFallingBParticle->nPosZ = 0x00AFC800;
        pFallingBParticle->nVelX = 0;
        pFallingBParticle->nVelY = 0;
        pFallingBParticle->nVelZ = -0xC800;
    }

    if (pSmokeA)
    {
        UNITS_SetAnimationSpeed(pSmokeB, 128);
        UNITS_SetAnimationSpeed(pSmokeA, 128);
    }

    return 1;
}

// 1.12a: 0x6FB4D270 (gpfnCltStFunc[43])
int32_t __fastcall CLTSKILL_St_SpawnMissileTowardTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    CLTSKILL_CreateMissileTowardUnitTarget(pUnit, nMissileId, nSkillId, nSkillLevel);
    return 1;
}

// 1.12a: 0x6FB4D2D0 (gpfnCltStFunc[42])
int32_t __fastcall CLTSKILL_St_MultiHitRollCount(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    if (!pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    D2SeedStrc tSeed = {};
    SEED_InitSeed(&tSeed);
    SEED_InitLowSeed(&tSeed, static_cast<int32_t>(pTarget->dwUnitId + pUnit->dwUnitId));

    const int32_t nMinHits = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    const int32_t nMaxHits = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[1], nSkillId, nSkillLevel);

    int32_t nHitCount = nMinHits + SEED_RollLimitedRandomNumber(&tSeed, nMaxHits - nMinHits);
    if (nHitCount < 1)
    {
        nHitCount = 1;
    }

    SKILLS_SetParam1(pSkill, nHitCount);
    SKILLS_SetParam2(pSkill, static_cast<int32_t>(pTarget->dwUnitId));
    SKILLS_SetParam3(pSkill, static_cast<int32_t>(pTarget->dwUnitType));
    return 1;
}

// External missile & sound helpers referenced in Section 2B & Section 3
extern void        __fastcall D2Client_ChangeUnitModeWithHitOverlayAndSound(D2UnitStrc* pUnit, int32_t nMode); // 0x6FAF13C0 (UNIT_UnitMode.cpp)
extern void        __fastcall CLTSKILL_SpawnScatteredMissileCluster(D2ClientMissileInitStrc* pInit, int32_t nCount, int32_t nSpreadRadius); // 0x6FAFF4D0 (SKILLS_Skills.cpp)
extern void        __fastcall D2Client_SpawnMissileRing(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nCenterX, int32_t nCenterY, int32_t nRadius, int32_t nSkillId, int32_t nSkillLevel, int32_t nStep, int32_t bCheckCollision); // 0x6FAD8770 (UNIT_Missile.cpp)
extern D2UnitStrc* __fastcall D2Client_SpawnLobbedMissile(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t nSkillId, int32_t nSkillLevel, int32_t nExtra); // 0x6FADB4B0 (UNIT_Missile.cpp)
extern void        __stdcall  PARTICLE_InitGravityArc(void* pUnit, int32_t nFrames, int32_t nInitialZ); // 0x6FB75400 (DRAW_dLightMap.cpp)
extern uint32_t    __fastcall D2Client_FindUnitActiveSoundHandle(D2UnitStrc* pUnit, int32_t nSoundId); // 0x6FAE6C10 (UNIT_Monster.cpp)
extern void        __fastcall D2Client_StopActiveSoundNode(void* pSoundNode); // 0x6FAE1A80 (Sound_SoundHdr.cpp)
extern void        __fastcall D2Client_FadeOutAndStopSoundHandle(uint32_t dwHandle, uint32_t dwDurationMs); // 0x6FAE2120 (UNIT_Monster.cpp)
extern void        __fastcall D2Client_SpawnCorpseGoreMissiles(D2UnitStrc* pCorpse, int32_t bPlaySound, int32_t nCount); // 0x6FAE3380 (UNIT_Monster.cpp)
extern void        __fastcall D2Client_PlayUnitSpeechOrVoiceSound(D2UnitStrc* pUnit, int32_t nSpeechType); // 0x6FAE7250 (UNIT_UnitMode.cpp)
extern int32_t     __fastcall D2Client_GetUnitFootstepSoundId(D2UnitStrc* pUnit, int32_t bLanding); // 0x6FAE79E0 (UNIT_UnitMode.cpp)
extern int32_t     __fastcall D2Client_GetWeaponSwingSoundByHitClass(D2UnitStrc* pUnit, int32_t nHitClass); // 0x6FAE6780 (UNIT_UnitMode.cpp)
extern void        __stdcall  D2Client_TeleportUnitToCoordinates(D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t bTeleportEffect); // 0x6FAD1550 (UNIT_CUnit.cpp)

// Mirrors 0x6FAFD990: posts an attack-rate stat modifier list to pUnit and updates animation rate
static inline void CLTSKILL_ApplySkillAttackRateBonus(D2UnitStrc* pUnit, int32_t nAttackRateBonus)
{
    const int32_t nOwnerType = pUnit ? static_cast<int32_t>(pUnit->dwUnitType) : 6;
    const D2UnitGUID dwOwnerGUID = pUnit ? pUnit->dwUnitId : static_cast<D2UnitGUID>(-1);
    D2StatListStrc* pStatList = STATLIST_AllocStatList(nullptr, 4, 0, nOwnerType, dwOwnerGUID);
    if (pStatList)
    {
        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, TRUE);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_ATTACKRATE, nAttackRateBonus, 0);
        UNITS_UpdateAnimRateAndVelocity(pUnit, s_szSkillsBarFile, 2532);
    }
}

// 1.12a: 0x6FB4D3F0 (gpfnCltDoFunc[64])
int32_t __fastcall CLTSKILL_Do_DesertTurret_Jump(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    const int32_t nLoopCounter = SKILLS_GetParam1(pSkill);
    const int32_t nCurFrame = pUnit->nSeqCurrentFramePrecise >> 8;

    if (nCurFrame == 6)
    {
        UNITS_SetAnimationFrame(pUnit, 7);
        pUnit->dwFrameCountPrecise = 0x3200;
        return 1;
    }

    if (nCurFrame == 22)
    {
        if (nLoopCounter >= 4)
        {
            UNITS_SetAnimationFrame(pUnit, 40);
            pUnit->dwFrameCountPrecise = 0x1100;
            return 1;
        }

        SKILLS_SetParam1(pSkill, nLoopCounter + 1);
        return 1;
    }

    if (nCurFrame == 30)
    {
        D2Client_ChangeUnitModeWithHitOverlayAndSound(pUnit, MONMODE_DEAD);
        pUnit->dwFlags |= UNITFLAG_ISDEAD;
    }

    return 1;
}

// 1.12a: 0x6FB4D480
D2UnitStrc* __fastcall CLTSKILL_SpawnSpiderGooLay(D2UnitStrc* pUnit)
{
    D2DynamicPathStrc* pPath = pUnit->pDynamicPath;
    if (!pPath || !D2Common_10234(pPath))
    {
        return nullptr;
    }

    D2SkillStrc* pSpiderLaySkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, SKILL_SPIDERLAY);
    const int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSpiderLaySkill, TRUE);
    D2ActiveRoomStrc* pHintRoom = UNITS_GetRoom(pUnit);

    int32_t nOriginX = CLTSKILL_GetUnitXInline(pUnit);
    int32_t nOriginY = CLTSKILL_GetUnitYInline(pUnit);

    static const uint8_t s_aRearGooDirs[8] = { 10, 8, 22, 20, 18, 16, 14, 12 };
    const uint8_t nNewDir = PATH_GetNewDirection(pPath);
    const uint8_t nOctant = static_cast<uint8_t>(D2Common_11053(nNewDir));

    int32_t nVecX = 0;
    int32_t nVecY = 0;
    D2Common_11055(s_aRearGooDirs[nOctant & 7u], &nVecX, &nVecY);

    nOriginX += nVecX;
    nOriginY += nVecY;

    D2ActiveRoomStrc* pGooRoom = DUNGEON_FindRoomBySubtile(pHintRoom, nOriginX, nOriginY);
    if (DUNGEON_IsRoomInTown(pGooRoom))
    {
        return nullptr;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x25;
    tInit.pOwner = pUnit;
    tInit.nMissileId = MISSILE_SPIDERGOOLAY;
    tInit.nX = nOriginX;
    tInit.nY = nOriginY;
    tInit.nTargetX = nOriginX + 2 * nVecX;
    tInit.nTargetY = nOriginY + 2 * nVecY;
    tInit.nVelocity = 3;
    tInit.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
    D2Client_PlayUnitSound_6FAE1FA0(1830, pUnit, 0, 0, 0);
    return pMissile;
}

// 1.12a: 0x6FB4D620 (gpfnCltDoFunc[65])
int32_t __fastcall CLTSKILL_Do_MonCurseCast(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nStreamMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (!CLTSKILL_IsValidMissileId(nStreamMissileId))
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nStreamRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel);

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x01;
    tInit.pOwner = pUnit;
    tInit.nMissileId = MISSILE_CURSECENTER; // 0xBF (191)
    tInit.nX = nTargetX;
    tInit.nY = nTargetY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
    if (!pMissile)
    {
        return 0;
    }

    MISSILE_SetStreamMissile(pMissile, nStreamMissileId);
    MISSILE_SetStreamRange(pMissile, nStreamRange);
    return 1;
}

// 1.12a: 0x6FB4D740 (gpfnCltDoFunc[63])
int32_t __fastcall CLTSKILL_Do_FetishAura_Cluster(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileA = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileA))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x2021;
    tInit.pOwner = pUnit;
    tInit.pOrigin = pUnit;
    tInit.nMissileId = nMissileA;
    tInit.nX = D2Client_GetUnitX_6FAB1590(pUnit);
    tInit.nY = D2Client_GetUnitY_6FAB15C0(pUnit);
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    CLTSKILL_SpawnScatteredMissileCluster(&tInit, 1, 2);

    const int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (CLTSKILL_IsValidMissileId(nMissileB))
    {
        tInit.nMissileId = nMissileB;
        CLTSKILL_SpawnScatteredMissileCluster(&tInit, 5, 4);
    }

    return 1;
}

// 1.12a: 0x6FB4D850 (gpfnCltDoFunc[61])
int32_t __fastcall CLTSKILL_Do_PrimePoisonNova(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    if (!D2Client_ClearActionFrameAndGetTarget(pUnit))
    {
        return 0;
    }

    const int32_t nVelocity = DATATBLS_GetMissileVelocityFromMissilesTxt(nMissileId, nSkillLevel);

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x07;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nX = D2Client_GetUnitX_6FAB1590(pUnit);
    tInit.nY = D2Client_GetUnitY_6FAB15C0(pUnit);
    tInit.nVelocity = nVelocity;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    for (int32_t i = 0; i < 64; ++i)
    {
        tInit.nTargetX = g_aPrimePoisonNovaOffsetX_6FB83E68[i];
        tInit.nTargetY = g_aPrimePoisonNovaOffsetY_6FB83D68[i];
        D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
        D2ParticleStrc* pParticle = CLTSKILL_GetUnitParticle(pMissile);
        if (pParticle)
        {
            pParticle->dwFlags = ((pParticle->dwFlags & ~0x01u) | 0x08u) & ~0x20u;
            pParticle->nVelX = 0;
            pParticle->nVelY = 0;
            pParticle->nVelZ = -0x3000;
            pParticle->nTargetX = 0;
            pParticle->nTargetY = 0;
            pParticle->nTargetZ = 0;
            pParticle->nPosX = 0;
            pParticle->nPosY = 0;
            pParticle->nPosZ = 0x3C000;
        }
    }

    return 1;
}

// 1.12a: 0x6FB4D9F0 (gpfnCltDoFunc[60])
int32_t __fastcall CLTSKILL_Do_DiabCold(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    const int32_t nUnitX = D2Client_GetUnitX_6FAB1590(pUnit);
    const int32_t nUnitY = D2Client_GetUnitY_6FAB15C0(pUnit);
    int32_t nDeltaX = nTargetX - nUnitX;
    int32_t nDeltaY = nTargetY - nUnitY;
    if (nDeltaX == 0 && nDeltaY == 0)
    {
        return 0;
    }

    int32_t nPerpStepX = 0;
    int32_t nPerpStepY = 0;
    if (nDeltaX < 0 && nDeltaY < 0)
    {
        nPerpStepX = 2;
        nPerpStepY = -2;
    }
    else if (nDeltaX == 0)
    {
        if (nDeltaY < 0)
        {
            nPerpStepX = 2;
        }
        else if (nDeltaY > 0)
        {
            nPerpStepX = -2;
        }
    }
    else if (nDeltaX > 0)
    {
        if (nDeltaY < 0)
        {
            nPerpStepX = 2;
            nPerpStepY = 2;
        }
        else if (nDeltaY == 0)
        {
            nPerpStepY = 2;
        }
        else
        {
            nPerpStepX = -2;
            nPerpStepY = 2;
        }
    }
    else
    {
        if (nDeltaY > 0)
        {
            nPerpStepX = -2;
            nPerpStepY = -2;
        }
        else if (nDeltaY == 0)
        {
            nPerpStepY = -2;
        }
    }

    const int32_t nSpreadCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    const int32_t nHalfSpread = nSpreadCount / 2;
    nDeltaX -= nHalfSpread * nPerpStepX;
    nDeltaY -= nHalfSpread * nPerpStepY;

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x03;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    for (int32_t i = 0; i < nSpreadCount; ++i)
    {
        tInit.nX = nUnitX + (nDeltaX / 3);
        tInit.nY = nUnitY + (nDeltaY / 3);
        tInit.nTargetX = nDeltaX;
        tInit.nTargetY = nDeltaY;
        D2Client_CreateClientMissile(&tInit);

        nDeltaX += nPerpStepX;
        nDeltaY += nPerpStepY;
    }

    return 1;
}

// 1.12a: 0x6FB4DC00 (gpfnCltDoFunc[58])
int32_t __fastcall CLTSKILL_Do_FingerMageSpider(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nMissileCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x21;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nX = D2Client_GetUnitX_6FAB1590(pUnit);
    tInit.nY = D2Client_GetUnitY_6FAB15C0(pUnit);
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    tInit.pfPostInit = CLTSKILL_InitFingerMageSpiderMissile;

    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &tInit.nTargetX, &tInit.nTargetY))
    {
        return 0;
    }

    for (int32_t i = 0; i < nMissileCount; ++i)
    {
        tInit.nPostInitArg = i;
        D2Client_CreateClientMissile(&tInit);
    }

    return 1;
}

// 1.12a: 0x6FB4DD10 (gpfnCltDoFunc[57])
int32_t __fastcall CLTSKILL_Do_DiabLight(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_GetMissilesTxtRecord(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x03;
    tInit.pOwner = pUnit;
    tInit.pOrigin = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nX = D2Client_GetUnitX_6FAB1590(pUnit);
    tInit.nY = D2Client_GetUnitY_6FAB15C0(pUnit);
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    tInit.nLoops = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);

    D2UnitStrc* pTarget = D2Client_ClearActionFrameAndGetTarget(pUnit);
    if (pTarget)
    {
        D2Common_10216(
            pUnit->pDynamicPath,
            D2Client_GetUnitX_6FAB1590(pTarget),
            D2Client_GetUnitY_6FAB15C0(pTarget),
            0);
        tInit.nTargetX = D2Client_GetUnitX_6FAB1590(pTarget) - D2Client_GetUnitX_6FAB1590(pUnit);
        tInit.nTargetY = D2Client_GetUnitY_6FAB15C0(pTarget) - D2Client_GetUnitY_6FAB15C0(pUnit);
    }
    else
    {
        tInit.nTargetX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath) - D2Client_GetUnitX_6FAB1590(pUnit);
        tInit.nTargetY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath) - D2Client_GetUnitY_6FAB15C0(pUnit);
    }

    D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
    if (pMissile)
    {
        MISSILE_SetTargetX(pMissile, nSkillLevel);
    }

    return 1;
}

// 1.12a: 0x6FB4DE90 (gpfnCltDoFunc[55])
int32_t __fastcall CLTSKILL_Do_DiabWall(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxtRecord(nMissileId);
    if (!pMissilesTxt)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x1F;
    tInit.pOwner = pUnit;
    tInit.pOrigin = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nX = D2Client_GetUnitX_6FAB1590(pUnit);
    tInit.nY = D2Client_GetUnitY_6FAB15C0(pUnit);
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    tInit.nLoops = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    tInit.nVelocity = pMissilesTxt->dwParam[0] << 6;

    int32_t nStepStride = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[1], nSkillId, nSkillLevel);
    if (nStepStride < 1)
    {
        nStepStride = 1;
    }

    for (int32_t i = 0; i < 16; i += 2)
    {
        tInit.nTargetX = g_aDiabWallStepY_6FB83FE8[i];
        tInit.nTargetY = g_aDiabWallStepX_6FB83FA8[i];
        D2Client_CreateClientMissile(&tInit);
    }

    tInit.nVelocity = pMissilesTxt->dwParam[1] << 6;
    if (nStepStride > 1)
    {
        for (int32_t i = 1; i < 16; i += nStepStride)
        {
            tInit.nTargetX = g_aDiabWallStepY_6FB83FE8[i];
            tInit.nTargetY = g_aDiabWallStepX_6FB83FA8[i];
            D2Client_CreateClientMissile(&tInit);
        }
    }

    return 1;
}

// 1.12a: 0x6FB4E010
D2UnitStrc* __fastcall CLTSKILL_SpawnFlameMissileWithRandomVel(
    D2UnitStrc* pUnit,
    int32_t nMissileId,
    const int32_t* pVelOffsets,
    int32_t nTargetX,
    int32_t nTargetY,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    D2ClientMissileInitStrc tInit = {};
    const uint32_t dwSoundRoll = static_cast<uint32_t>(SEED_RollLimitedRandomNumber(&pUnit->pSeed, 4));
    tInit.dwFlags = (dwSoundRoll > 0) ? 0x4025u : 0x25u;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nX = CLTSKILL_GetUnitXInline(pUnit);
    tInit.nY = CLTSKILL_GetUnitYInline(pUnit);
    tInit.nTargetX = nTargetX;
    tInit.nTargetY = nTargetY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxtRecord(nMissileId);
    if (!pMissilesTxt)
    {
        return nullptr;
    }

    const int32_t nVelIdx = SEED_RollLimitedRandomNumber(&pUnit->pSeed, 5);
    tInit.nVelocity = static_cast<int32_t>(pMissilesTxt->nVel) + pVelOffsets[nVelIdx];

    D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
    if (!pMissile)
    {
        return nullptr;
    }

    CLTSKILL_ConfigureFlameMissileRangeAndSpeed(pUnit, pMissile, nSkillId, nSkillLevel);
    return pMissile;
}

// 1.12a: 0x6FB4E170 (gpfnCltDoFunc[53])
int32_t __fastcall CLTSKILL_Do_DiabPrison(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    int32_t nRadius = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel) - 6;
    if (nRadius < 1)
    {
        nRadius = 1;
    }

    const int32_t nStep = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);
    D2Client_SpawnMissileRing(pUnit, nMissileId, nTargetX, nTargetY, nRadius, nSkillId, nSkillLevel, nStep, 1);
    return 1;
}

// 1.12a: 0x6FB4E260 (gpfnCltDoFunc[51])
int32_t __fastcall CLTSKILL_Do_ArcaneTower(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pTarget = D2Client_ClearActionFrameAndGetTarget(pUnit);
    if (!pTarget)
    {
        return 0;
    }

    const int32_t nTargetX = D2Client_GetUnitX_6FAB1590(pTarget);
    const int32_t nTargetY = D2Client_GetUnitY_6FAB15C0(pTarget);
    const int32_t nUnitX = D2Client_GetUnitX_6FAB1590(pUnit);
    const int32_t nUnitY = D2Client_GetUnitY_6FAB15C0(pUnit);

    const int32_t nAbsDx = (nTargetX >= nUnitX) ? (nTargetX - nUnitX) : (nUnitX - nTargetX);
    const int32_t nAbsDy = (nTargetY >= nUnitY) ? (nTargetY - nUnitY) : (nUnitY - nTargetY);

    int32_t nAimX = nTargetX;
    int32_t nAimY = nTargetY;

    if (nAbsDx < nAbsDy)
    {
        nAimX = nUnitX;
        for (int32_t i = 0; i < 4; ++i)
        {
            if (nAimX > nTargetX)
            {
                --nAimX;
            }
            else if (nAimX < nTargetX)
            {
                ++nAimX;
            }
        }
    }
    else
    {
        nAimY = nUnitY;
        for (int32_t i = 0; i < 4; ++i)
        {
            if (nAimY > nTargetY)
            {
                --nAimY;
            }
            else if (nAimY < nTargetY)
            {
                ++nAimY;
            }
        }
    }

    const int32_t nDeltaX = nAimX - nUnitX;
    const int32_t nDeltaY = nAimY - nUnitY;

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x03;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nX = nUnitX + (nDeltaX / 3) - 1;
    tInit.nY = nUnitY + (nDeltaY / 3) - 1;
    tInit.nTargetX = nDeltaX;
    tInit.nTargetY = nDeltaY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    const uint8_t nDir64 = UNITS_GetDirectionToCoords(pUnit, nAimX, nAimY) & 0x3Fu;
    const uint8_t nQuadrant = g_aArcaneTowerQuadrant_6FB84040[nDir64] & 0x03u;
    PATH_SetDirection(pUnit->pDynamicPath, g_aArcaneTowerSnapDir_6FB8403C[nQuadrant]);

    D2Client_CreateClientMissile(&tInit);
    return 1;
}

// 1.12a: 0x6FB4E430 (gpfnCltDoFunc[66])
int32_t __fastcall CLTSKILL_Do_Hireable_RogueMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    int32_t nWeaponType = 0;
    const int32_t nWeaponMissileId = D2COMMON_11039_CheckWeaponIsMissileBased(pUnit, &nWeaponType);
    if (nWeaponMissileId != 0 && nWeaponMissileId != 31)
    {
        if (!CLTSKILL_GetMissilesTxtRecord(nWeaponMissileId))
        {
            return 0;
        }

        CLTSKILL_CreateMissileTowardUnitTarget(pUnit, nWeaponMissileId, nSkillId, nSkillLevel);
        return 1;
    }

    const int32_t nSkillMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_GetMissilesTxtRecord(nSkillMissileId))
    {
        return 0;
    }

    if ((pSkillsTxt->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_LOB]) != 0)
    {
        D2Client_SpawnLobbedMissile(pUnit, nSkillMissileId, 0, 0, 0, 0, nSkillId, nSkillLevel, 0);
        return 1;
    }

    CLTSKILL_CreateMissileTowardUnitTarget(pUnit, nSkillMissileId, nSkillId, nSkillLevel);
    return 1;
}

// 1.12a: 0x6FB4E550 (gpfnCltDoFunc[95])
int32_t __fastcall CLTSKILL_Do_NecromageMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    uint8_t nVariantOffset = 0;
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        nVariantOffset = pUnit->pMonsterData->nComponent[MONSTATS2COMPONENTFLAGINDEX_S4]; // +0x0F (component 11)
    }

    nMissileId += static_cast<int32_t>(nVariantOffset);
    if (!CLTSKILL_GetMissilesTxtRecord(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    if ((pSkillsTxt->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_LOB]) != 0)
    {
        D2Client_SpawnLobbedMissile(pUnit, nMissileId, 0, 0, 0, 0, nSkillId, nSkillLevel, 0);
        return 1;
    }

    CLTSKILL_CreateMissileTowardUnitTarget(pUnit, nMissileId, nSkillId, nSkillLevel);
    return 1;
}

// 1.12a: 0x6FB4E630 (gpfnCltDoFunc[94])
int32_t __fastcall CLTSKILL_Do_DoomKnightMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    uint8_t nVariantOffset = 0;
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        nVariantOffset = pUnit->pMonsterData->nComponent[MONSTATS2COMPONENTFLAGINDEX_S3]; // +0x0E (component 10)
    }

    nMissileId += static_cast<int32_t>(nVariantOffset);
    if (!CLTSKILL_GetMissilesTxtRecord(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    if ((pSkillsTxt->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_LOB]) != 0)
    {
        D2Client_SpawnLobbedMissile(pUnit, nMissileId, 0, 0, 0, 0, nSkillId, nSkillLevel, 0);
        return 1;
    }

    CLTSKILL_CreateMissileTowardUnitTarget(pUnit, nMissileId, nSkillId, nSkillLevel);
    return 1;
}

// 1.12a: 0x6FB4E710 (gpfnCltDoFunc[56])
int32_t __fastcall CLTSKILL_Do_MonInferno(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nInterval = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);
    const int32_t nNextTick = SKILLS_GetParam1(pSkill) + 1;
    if (nNextTick < nInterval)
    {
        SKILLS_SetParam1(pSkill, nNextTick);
        return 0;
    }

    SKILLS_SetParam1(pSkill, 0);

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    D2Common_10216(pUnit->pDynamicPath, nTargetX, nTargetY, 0);
    CLTSKILL_SpawnFlameMissileWithRandomVel(
        pUnit,
        nMissileId,
        g_aMonInfernoVelDeltas_6FB83F90,
        nTargetX,
        nTargetY,
        nSkillId,
        nSkillLevel);
    CLTSKILL_AdvanceInfernoAnimFrame(pUnit, pSkillsTxt);
    return 1;
}

// 1.12a: 0x6FB4E820 (gpfnCltDoFunc[54])
int32_t __fastcall CLTSKILL_Do_ImpInferno(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileA = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileA))
    {
        return 0;
    }

    const int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    D2Common_10216(pUnit->pDynamicPath, nTargetX, nTargetY, 0);

    for (int32_t i = 0; i < 2; ++i)
    {
        int32_t nChosenMissile = nMissileA;
        if (nMissileB > 0 && SEED_RollLimitedRandomNumber(&pUnit->pSeed, 2) != 0)
        {
            nChosenMissile = nMissileB;
        }

        CLTSKILL_SpawnFlameMissileWithRandomVel(
            pUnit,
            nChosenMissile,
            g_aImpInfernoVelDeltas_6FB84028,
            nTargetX,
            nTargetY,
            nSkillId,
            nSkillLevel);
    }

    return 1;
}

// ============================================================================
// SECTION 3: Barbarian Client Skills (`..\Source\D2Client\SKILLS\SkillsBar.cpp`)
// VA Range: 0x6FB73920 .. 0x6FB75252 (24 procedures)
// ============================================================================

// 1.12a: 0x6FB73920
int32_t __fastcall CLTSKILL_StartLeapPath(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
    D2DynamicPathStrc* pPath = pUnit->pDynamicPath;
    if (!pPath)
    {
        return 0;
    }

    const int32_t nTargetX = SKILLS_GetParam1(pSkill);
    const int32_t nTargetY = SKILLS_GetParam2(pSkill);
    if (nTargetX == 0 || nTargetY == 0)
    {
        return 0;
    }

    SKILLS_SetFlags(pSkill, 0x1101);
    PATH_SetMoveTestCollisionMask(pPath, COLLIDE_NONE);
    PATH_SetFootprintCollisionMask(pPath, COLLIDE_NONE);
    D2COMMON_10170_PathSetTargetPos(pPath, nTargetX, nTargetY);
    PATH_SetType(pPath, PATHTYPE_LEAP);

    int32_t nLeapVelocity = 0;
    if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        D2MonStatsTxt* pMonStatsTxt = CLTSKILL_GetMonStatsTxtRecord(pUnit->dwClassId);
        if (!pMonStatsTxt)
        {
            return 0;
        }
        nLeapVelocity = static_cast<int32_t>(pMonStatsTxt->nRun) << 8;
    }
    else
    {
        D2CharStatsTxt* pCharStatsTxt = CLTSKILL_GetCharStatsTxtRecord(pUnit->dwClassId);
        nLeapVelocity = static_cast<int32_t>(static_cast<uint8_t>(pCharStatsTxt->nRunSpeed)) << 8;
    }

    PATH_SetVelocity(pPath, nLeapVelocity, s_szSkillsBarFile, 567);
    D2Common_10142(pPath, pUnit, 0);
    return 1;
}

// 1.12a: 0x6FB739E0
int32_t __fastcall CLTSKILL_CheckLeapTargetRoomValid(int32_t nSkillId, D2UnitStrc* pUnit, int32_t nTargetX, int32_t nTargetY)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2ActiveRoomStrc* pUnitRoom = UNITS_GetRoom(pUnit);
    if (!pUnitRoom)
    {
        return 0;
    }

    D2ActiveRoomStrc* pTargetRoom = DUNGEON_FindRoomBySubtile(pUnitRoom, nTargetX, nTargetY);
    if (!pTargetRoom)
    {
        return 0;
    }

    if ((pSkillsTxt->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_INTOWN]) == 0 && DUNGEON_IsRoomInTown(pTargetRoom))
    {
        return 0;
    }

    return 1;
}

// 1.12a: 0x6FB73A60
void __fastcall CLTSKILL_RestoreLeapCollisionAndPath(D2UnitStrc* pUnit, int32_t nReservedX, int32_t nReservedY)
{
    const uint32_t dwUnitType = pUnit ? pUnit->dwUnitType : 6u;
    D2DynamicPathStrc* pPath = pUnit->pDynamicPath;
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pPath || !pRoom)
    {
        return;
    }

    if (dwUnitType == UNIT_MONSTER)
    {
        if (nReservedX != 0 && nReservedY != 0)
        {
            const uint32_t dwPattern = PATH_GetUnitCollisionPattern(pUnit);
            COLLISION_ResetMaskWithPattern(pRoom, nReservedX, nReservedY, dwPattern, COLLIDE_MONSTER);
        }
        PATH_SetFootprintCollisionMask(pPath, COLLIDE_MONSTER);
        PATH_SetMoveTestCollisionMask(pPath, 0x3C01u);
        PATH_SetType(pPath, PATHTYPE_TOWARD);
    }
    else
    {
        if (nReservedX != 0 && nReservedY != 0)
        {
            const uint32_t dwPattern = PATH_GetUnitCollisionPattern(pUnit);
            COLLISION_ResetMaskWithPattern(pRoom, nReservedX, nReservedY, dwPattern, COLLIDE_PLAYER);
        }
        PATH_SetFootprintCollisionMask(pPath, COLLIDE_PLAYER);
        PATH_SetMoveTestCollisionMask(pPath, 0x1C09u);
        PATH_SetType(pPath, PATHTYPE_ASTAR);
    }

    bool bDeadOrDying = (pUnit->dwFlags & UNITFLAG_ISDEAD) != 0;
    if (!bDeadOrDying)
    {
        if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            bDeadOrDying = (pUnit->dwAnimMode == PLRMODE_DEATH || pUnit->dwAnimMode == PLRMODE_DEAD);
        }
        else if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            bDeadOrDying = (pUnit->dwAnimMode == MONMODE_DEATH || pUnit->dwAnimMode == MONMODE_DEAD);
        }
    }

    if (bDeadOrDying)
    {
        PATH_SetUnitDeadCollision(pUnit, FALSE);
    }
}

// 1.12a: 0x6FB73B50
void __fastcall CLTSKILL_StepSandLeaperKnockbackPath(
    D2SkillStrc* pSkill,
    D2UnitStrc* pUnit,
    int32_t nLandingX,
    int32_t nLandingY)
{
    D2DynamicPathStrc* pPath = pUnit->pDynamicPath;
    if (!pPath)
    {
        return;
    }

    const int32_t nTargetType = SKILLS_GetParam3(pSkill);
    const uint32_t dwTargetGUID = static_cast<uint32_t>(SKILLS_GetParam4(pSkill));
    D2UnitStrc* pTarget = CLTSKILL_FindServerUnit(nTargetType, dwTargetGUID);
    if (!pTarget)
    {
        return;
    }

    SKILLS_SetFlags(pSkill, 0x01);
    UNITS_SetAnimationFrame(pUnit, 12);
    pUnit->dwFrameCountPrecise = (pUnit->dwFrameCountPrecise & 0xFFFFFF00) + 0x100;
    D2COMMON_10212_PATH_SetMoveFlags(pUnit, FALSE);
    PATH_RecacheRoomIfNeeded(pPath);
    PATH_SetNewDistance(pPath, 5);
    PATH_SetStepNum(pPath, 1);
    PATH_SetTargetUnit(pPath, nullptr);

    const int32_t nBounceX = 3 * nLandingX - 2 * CLTSKILL_GetUnitXInline(pTarget);
    const int32_t nBounceY = 3 * nLandingY - 2 * CLTSKILL_GetUnitYInline(pTarget);

    D2ActiveRoomStrc* pBounceRoom = DUNGEON_FindRoomBySubtile(UNITS_GetRoom(pUnit), nBounceX, nBounceY);
    if (pBounceRoom && DUNGEON_IsRoomInTown(pBounceRoom))
    {
        return;
    }

    D2COMMON_10170_PathSetTargetPos(pPath, nBounceX, nBounceY);
    PATH_SetType(pPath, PATHTYPE_KNOCKBACK_SERVER);
    D2COMMON_10190_PATH_SetDistance(pPath, 5);
    D2Common_10142(pPath, pUnit, 0);
}

// 1.12a: 0x6FB73C90 (gpfnCltStFunc[27])
int32_t __fastcall CLTSKILL_St_Bash_Stun_Concentrate(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nAttackRateBonus = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);
    CLTSKILL_ApplySkillAttackRateBonus(pUnit, nAttackRateBonus);
    return 1;
}

// 1.12a: 0x6FB73CE0
void __fastcall CLTSKILL_SetupLeapArcParticle(D2SkillStrc* pSkill, D2UnitStrc* pUnit)
{
    if (!pSkill || !pUnit->pDynamicPath)
    {
        return;
    }

    const int32_t nTargetX = SKILLS_GetParam1(pSkill);
    const int32_t nTargetY = SKILLS_GetParam2(pSkill);
    const int32_t nSpeedSubtiles = PATH_GetVelocity(pUnit->pDynamicPath) >> 8;
    if (nSpeedSubtiles == 0)
    {
        return;
    }

    int32_t nDistance = UNITS_GetDistanceToCoordinates(pUnit, nTargetX, nTargetY);
    if (nDistance < 1)
    {
        nDistance = 1;
    }

    int32_t nArcFrames = (nDistance << 16) / (nSpeedSubtiles << 12);
    if (nArcFrames > 1)
    {
        --nArcFrames;
    }

    int32_t nTableIdx = 0;
    if (nDistance >= g_aLeapArcTable_6FB86FBC[0].nMaxDist)
    {
        nTableIdx = 1;
    }

    int32_t nGravityMag = g_aLeapArcTable_6FB86FBC[nTableIdx].nSlope * nDistance
                        + g_aLeapArcTable_6FB86FBC[nTableIdx].nIntercept;
    if (nGravityMag < 500)
    {
        nGravityMag = 500;
    }

    PARTICLE_AllocForUnit(pUnit);
    D2ParticleStrc* pParticle = CLTSKILL_GetUnitParticle(pUnit);
    if (pParticle)
    {
        pParticle->nAccelX = 0;
        pParticle->nAccelY = 0;
        pParticle->nAccelZ = -nGravityMag;
    }

    PARTICLE_InitGravityArc(pUnit, nArcFrames, 0);
}

// 1.12a: 0x6FB73DD0 (gpfnCltStFunc[28])
int32_t __fastcall CLTSKILL_St_FindItem(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (!pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    return D2COMMON_11019(pTarget) ? 1 : 0;
}

// 1.12a: 0x6FB73E20 (gpfnCltStFunc[26])
int32_t __fastcall CLTSKILL_St_FindPotion_GrimWard(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (!pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    return D2COMMON_11018(pTarget) ? 1 : 0;
}

// 1.12a: 0x6FB73E70
void __fastcall CLTSKILL_CancelWhirlwindCallback(D2UnitStrc* pUnit, int32_t nSkillId)
{
    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (pSkill)
    {
        const uint32_t dwFlags = SKILLS_GetFlags(pSkill);
        if ((dwFlags & 0x01) != 0)
        {
            SKILLS_SetFlags(pSkill, dwFlags | 0x02);
        }
    }

    const uint32_t dwLoopSoundHandle = D2Client_FindUnitActiveSoundHandle(pUnit, 2531); // 0x9E3: barbarian_whirlwind_loop
    if (dwLoopSoundHandle != 0)
    {
        D2Client_FadeOutAndStopSoundHandle(dwLoopSoundHandle, 0);
    }
}

// 1.12a: 0x6FB73ED0 (gpfnCltDoFunc[45])
int32_t __fastcall CLTSKILL_Do_WhirlwindEnd(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2DynamicPathStrc* pPath = pUnit->pDynamicPath;
    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill || !pPath)
    {
        return 0;
    }

    const bool bIsPlayer = (pUnit->dwUnitType == UNIT_PLAYER);
    const uint32_t dwFootprintMask = bIsPlayer ? COLLIDE_PLAYER : COLLIDE_MONSTER;

    const int32_t nReservedX = SKILLS_GetParam1(pSkill);
    const int32_t nReservedY = SKILLS_GetParam2(pSkill);
    const uint32_t dwFlags = SKILLS_GetFlags(pSkill);

    if ((dwFlags & 0x03) == 0x03)
    {
        const uint32_t dwPattern = PATH_GetUnitCollisionPattern(pUnit);
        COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pUnit), nReservedX, nReservedY, dwPattern, dwFootprintMask);
        PATH_SetFootprintCollisionMask(pPath, dwFootprintMask);
        PATH_SetMoveTestCollisionMask(pPath, bIsPlayer ? 0x1C09u : 0x3C01u);
        SKILLS_SetFlags(pSkill, 0);

        const uint32_t dwLoopSoundHandle = D2Client_FindUnitActiveSoundHandle(pUnit, 2531);
        D2Client_FadeOutAndStopSoundHandle(dwLoopSoundHandle, 0);
        return 1;
    }

    if ((dwFlags & 0x01) != 0)
    {
        UNITS_SetAnimationFrame(pUnit, 3);
        pUnit->dwFrameCountPrecise = 0x500;
    }

    return 1;
}

// 1.12a: 0x6FB73FD0 (gpfnCltDoFunc[40])
int32_t __fastcall CLTSKILL_Do_FindItem(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (!pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget || !UNITS_IsSoftMonster(pTarget))
    {
        return 0;
    }

    pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    D2Client_SpawnCorpseGoreMissiles(pTarget, 0, 5);
    return 1;
}

// 1.12a: 0x6FB74040 (gpfnCltDoFunc[38])
int32_t __fastcall CLTSKILL_Do_FindPotion(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (!pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    D2Client_SpawnCorpseGoreMissiles(pTarget, 0, 5);
    return 1;
}

// 1.12a: 0x6FB740A0
int32_t __fastcall CLTSKILL_ClampAndValidateLeapDestination(
    int32_t nSkillId,
    D2UnitStrc* pUnit,
    int32_t nSkillLevel,
    int32_t* pInOutTargetX,
    int32_t* pInOutTargetY)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nOriginX = CLTSKILL_GetUnitXInline(pUnit);
    const int32_t nOriginY = CLTSKILL_GetUnitYInline(pUnit);
    const int32_t nMaxRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel);

    int32_t nDist = UNITS_GetDistanceToCoordinates(pUnit, *pInOutTargetX, *pInOutTargetY);
    if (nDist <= 1)
    {
        nDist = 1;
    }

    if (nDist > nMaxRange)
    {
        *pInOutTargetX = nOriginX + ((*pInOutTargetX - nOriginX) * nMaxRange) / nDist;
        *pInOutTargetY = nOriginY + ((*pInOutTargetY - nOriginY) * nMaxRange) / nDist;
    }

    int32_t nStepDeltaY = *pInOutTargetY - nOriginY;
    if (nStepDeltaY <= -2)
    {
        nStepDeltaY = -2;
    }
    else if (nStepDeltaY >= 2)
    {
        nStepDeltaY = 2;
    }

    int32_t nStepDeltaX = nOriginX - *pInOutTargetX;
    if (nStepDeltaX <= -2)
    {
        nStepDeltaX = -2;
    }
    else if (nStepDeltaX >= 2)
    {
        nStepDeltaX = 2;
    }

    D2CoordStrc tCandidate = { *pInOutTargetX, *pInOutTargetY };
    for (int32_t nAttempt = 3; nAttempt > 0; --nAttempt)
    {
        if (nAttempt == 2)
        {
            tCandidate.nX = *pInOutTargetX + nStepDeltaY;
            tCandidate.nY = *pInOutTargetY + nStepDeltaX;
        }
        else if (nAttempt == 1)
        {
            tCandidate.nX = *pInOutTargetX - nStepDeltaY;
            tCandidate.nY = *pInOutTargetY - nStepDeltaX;
        }

        D2ActiveRoomStrc* pSearchRoom = DUNGEON_FindRoomBySubtile(UNITS_GetRoom(pUnit), tCandidate.nX, tCandidate.nY);
        D2ActiveRoomStrc* pFreeRoom = COLLISION_GetFreeCoordinates(
            pSearchRoom,
            &tCandidate,
            UNITS_GetUnitSizeX(pUnit),
            0x1C09u,
            TRUE);
        if (pFreeRoom)
        {
            const uint32_t dwPattern = PATH_GetUnitCollisionPattern(pUnit);
            if (!COLLISION_CheckAnyCollisionWithPattern(pFreeRoom, tCandidate.nX, tCandidate.nY, dwPattern, 0x1C09u)
                && D2Common_11025(nOriginX, nOriginY, tCandidate.nX, tCandidate.nY, pFreeRoom, 0x804u)
                && static_cast<int32_t>(UNITS_GetDistanceToCoordinates(pUnit, tCandidate.nX, tCandidate.nY)) <= nMaxRange)
            {
                *pInOutTargetX = tCandidate.nX;
                *pInOutTargetY = tCandidate.nY;
                return 1;
            }
        }
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        D2Client_PlayUnitSpeechOrVoiceSound(pUnit, 19);
    }

    return 0;
}

// 1.12a: 0x6FB74340 (gpfnCltDoFunc[39])
int32_t __fastcall CLTSKILL_Do_WhirlwindAttack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if ((pUnit->dwSeqFrame & 0xFFFFFF00) < 0x600)
    {
        const int32_t nHitClass = UNITS_GetHitClass(pUnit) & 0x0F;
        if (nHitClass >= 0 && nHitClass < 14)
        {
            const int32_t nSwingSound = g_aWhirlwindSwingSounds_6FBA3D70[nHitClass].nBaseSoundId;
            D2Client_PlayUnitSound_6FAE1FA0(nSwingSound, pUnit, 4, 0, 0);
        }

        if (!pUnit->pDynamicPath)
        {
            return 0;
        }

        D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
        if (pTarget && pUnit->pDynamicPath)
        {
            D2Common_10216(
                pUnit->pDynamicPath,
                D2Client_GetUnitX_6FAB1590(pTarget),
                D2Client_GetUnitY_6FAB15C0(pTarget),
                0);
        }
        return 1;
    }

    if (!pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pCurTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    const uint32_t dwPrevTargetId = pCurTarget ? pCurTarget->dwUnitId : 0xFFFFFFFFu;

    D2ChargeTargetFindArgStrc tFindArg = {};
    tFindArg.pNextHigherUnit = nullptr;
    tFindArg.dwMinHigherId = 0xFFFFFFFFu;
    tFindArg.pLowestUnit = nullptr;
    tFindArg.dwLowestId = dwPrevTargetId;
    tFindArg.dwPrevTargetId = dwPrevTargetId;
    tFindArg.nVisitedCount = 0;

    CLTSKILL_FindUnitsInRadius(
        pUnit,
        0,
        0,
        3,
        0xA783u,
        reinterpret_cast<void*>(CLTSKILL_AuraCallback_SelectNextRingTarget_Thunk),
        &tFindArg,
        0);

    D2UnitStrc* pChosenTarget = tFindArg.pNextHigherUnit ? tFindArg.pNextHigherUnit : tFindArg.pLowestUnit;
    if (pChosenTarget && pUnit->pDynamicPath)
    {
        D2Common_10216(
            pUnit->pDynamicPath,
            D2Client_GetUnitX_6FAB1590(pChosenTarget),
            D2Client_GetUnitY_6FAB15C0(pChosenTarget),
            0);
    }

    return 1;
}

// 1.12a: 0x6FB744A0
int32_t __fastcall CLTSKILL_UpdateLeapLanding(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t bAttachLandingOverlay)
{
    const int32_t nDestX = SKILLS_GetParam1(pSkill);
    const int32_t nDestY = SKILLS_GetParam2(pSkill);

    D2ParticleStrc* pParticle = CLTSKILL_GetUnitParticle(pUnit);
    const int32_t nHeightBlocks = pParticle ? (pParticle->nPosZ >> 11) : 0;

    const bool bIsMonster = pUnit && (pUnit->dwUnitType == UNIT_MONSTER);
    const int32_t nBaseMonId = bIsMonster ? CLTSKILL_GetBaseMonsterClassId(pUnit) : -1;

    if (nHeightBlocks != 0)
    {
        const bool bReachedXY = (CLTSKILL_GetUnitXInline(pUnit) == nDestX && CLTSKILL_GetUnitYInline(pUnit) == nDestY);
        if (!bReachedXY)
        {
            int32_t nAirborneFrame = 10;
            if (bIsMonster)
            {
                nAirborneFrame = 0;
                if (nBaseMonId == MONSTER_SANDLEAPER1)
                {
                    nAirborneFrame = 8;
                }
                else if (nBaseMonId == MONSTER_ANCIENTBARB1)
                {
                    nAirborneFrame = (UNITS_GetDistanceToCoordinates(pUnit, nDestX, nDestY) < 2) ? 12 : 10;
                }
            }

            UNITS_SetAnimationFrame(pUnit, nAirborneFrame);
            pUnit->dwFrameCountPrecise = (pUnit->dwFrameCountPrecise & 0xFFFFFF00) + 0x100;
            return 0;
        }
    }

    CLTSKILL_RestoreLeapCollisionAndPath(pUnit, nDestX, nDestY);

    if (pParticle)
    {
        pParticle->nPosX = 0;
        pParticle->nPosY = 0;
        pParticle->nPosZ = 0;
    }

    if (bAttachLandingOverlay != 0)
    {
        GFX_AttachOverlayToUnit(pUnit, 80, 2); // Overlay 0x50: barbarian_leap_dust
    }

    const int32_t nFootstepSound = D2Client_GetUnitFootstepSoundId(pUnit, 1);
    D2Client_PlayUnitSound_6FAE1FA0(nFootstepSound, pUnit, 0, 0, 0);

    if (bIsMonster)
    {
        if (nBaseMonId == MONSTER_SANDLEAPER1)
        {
            CLTSKILL_StepSandLeaperKnockbackPath(pSkill, pUnit, nDestX, nDestY);
        }
    }
    else
    {
        D2Client_PlayUnitSound_6FAE1FA0(2517, pUnit, 0, 0, 0); // 0x9D5: barbarian_leap_land
    }

    return 1;
}

// 1.12a: 0x6FB74700 (gpfnCltDoFunc[44])
int32_t __fastcall CLTSKILL_Do_LeapAttack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill)
    {
        return 0;
    }

    int32_t bHitTarget = 0;
    const uint32_t dwFlags = SKILLS_GetFlags(pSkill);

    if ((dwFlags & 0x100) != 0)
    {
        if (!CLTSKILL_UpdateLeapLanding(pUnit, pSkill, 1))
        {
            return 0;
        }

        const int32_t nTargetType = SKILLS_GetParam3(pSkill);
        const uint32_t dwTargetGUID = static_cast<uint32_t>(SKILLS_GetParam4(pSkill));
        D2UnitStrc* pTarget = nullptr;
        if (nTargetType != 6)
        {
            pTarget = CLTSKILL_FindServerUnit(nTargetType, dwTargetGUID);
        }

        if (!pTarget)
        {
            D2ChargeTargetFindArgStrc tFindArg = {};
            tFindArg.pNextHigherUnit = nullptr;
            tFindArg.dwMinHigherId = 0xFFFFFFFFu;
            tFindArg.pLowestUnit = nullptr;
            tFindArg.dwLowestId = 0xFFFFFFFFu;
            tFindArg.dwPrevTargetId = 0xFFFFFFFFu;
            tFindArg.nVisitedCount = 0;

            CLTSKILL_FindUnitsInRadius(
                pUnit,
                0,
                0,
                3,
                0xA783u,
                reinterpret_cast<void*>(CLTSKILL_AuraCallback_SelectNextRingTarget_Thunk),
                &tFindArg,
                0);

            pTarget = tFindArg.pNextHigherUnit ? tFindArg.pNextHigherUnit : tFindArg.pLowestUnit;
        }

        if (pTarget)
        {
            UNITS_SetTargetUnitForDynamicUnit(pUnit, pTarget);
            D2Client_FaceUnitTowardDynamicTarget(pUnit, 0);
            const int32_t nSwingSound = D2Client_GetWeaponSwingSoundByHitClass(pUnit, UNITS_GetHitClass(pUnit));
            D2Client_PlayUnitSound_6FAE1FA0(nSwingSound, pUnit, 0, 0, 0);
            bHitTarget = 1;
        }
        else
        {
            pUnit->dwFrameCountPrecise = 0x400;
        }
    }

    if ((dwFlags & 0x80) != 0)
    {
        CLTSKILL_StartLeapPath(pUnit, pSkill);
        CLTSKILL_SetupLeapArcParticle(pSkill, pUnit);
        return 1;
    }

    SKILLS_SetFlags(pSkill, 0x1000);
    return bHitTarget;
}

// 1.12a: 0x6FB74840 (gpfnCltDoFunc[43])
int32_t __fastcall CLTSKILL_Do_Leap(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill)
    {
        return 0;
    }

    const uint32_t dwFlags = SKILLS_GetFlags(pSkill);
    if ((dwFlags & 0x100) != 0)
    {
        if (!CLTSKILL_UpdateLeapLanding(pUnit, pSkill, 1))
        {
            return 0;
        }
    }

    if ((dwFlags & 0x80) != 0)
    {
        CLTSKILL_StartLeapPath(pUnit, pSkill);
        CLTSKILL_SetupLeapArcParticle(pSkill, pUnit);
        return 1;
    }

    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        SKILLS_SetFlags(pSkill, 0x1000);
    }

    return 1;
}

// 1.12a: 0x6FB748C0
void __fastcall CLTSKILL_DrawWhirlwindRotatedGhost(D2UnitStrc* pUnit, uint32_t dwGamma, int32_t bFirstAngleStep)
{
    D2DynamicPathStrc* pPath = pUnit->pDynamicPath;
    if (!pPath)
    {
        return;
    }

    const uint8_t nSavedDir = PATH_GetDirection(pPath);
    const uint8_t nRotatedDir = static_cast<uint8_t>((nSavedDir + (bFirstAngleStep ? 21 : 42)) & 0x3Fu);
    PATH_SetDirection(pPath, nRotatedDir);

    int32_t nDrawX = 0;
    int32_t nDrawY = 0;
    if (D2GFX_CheckPerspective())
    {
        SCALE_ScalePerspectivePosition(
            UNITS_GetPrecisionX(pUnit),
            UNITS_GetPrecisionY(pUnit),
            96,
            &nDrawX,
            &nDrawY,
            0);
        nDrawX += gnCameraScreenSkewX - gnViewportShiftOffsetX;
        nDrawY += gnCameraScreenSkewY - 8;
    }
    else
    {
        nDrawX = UNITS_GetClientCoordX(pUnit);
        nDrawY = UNITS_GetClientCoordY(pUnit);
    }

    const int32_t bDrawHighlight = (pUnit->dwUnitType == UNIT_PLAYER) ? 1 : 0;
    GFX_DrawUnit(pUnit, dwGamma, nDrawX, nDrawY, bDrawHighlight, 0);
    PATH_SetDirection(pPath, nSavedDir);
}

// 1.12a: 0x6FB749A0 (gpfnCltDoFunc[41])
int32_t __fastcall CLTSKILL_Do_GrimWard(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt || !pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget || !UNITS_IsSoftMonster(pTarget))
    {
        return 0;
    }

    pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;

    D2CoordStrc tTotemCoord = {};
    tTotemCoord.nX = D2Client_GetUnitX_6FAB1590(pTarget);
    tTotemCoord.nY = D2Client_GetUnitY_6FAB15C0(pTarget);

    D2ActiveRoomStrc* pTargetRoom = UNITS_GetRoom(pTarget);
    if (!pTargetRoom)
    {
        return 0;
    }

    D2ActiveRoomStrc* pSubtileRoom = DUNGEON_FindRoomBySubtile(pTargetRoom, tTotemCoord.nX, tTotemCoord.nY);
    if (!pSubtileRoom)
    {
        return 0;
    }

    if (!COLLISION_GetFreeCoordinates(pSubtileRoom, &tTotemCoord, 2, COLLIDE_CORPSE, TRUE))
    {
        return 0;
    }

    if (pTarget->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    D2MonStats2Txt* pMonStats2Txt = CLTSKILL_GetMonStats2TxtRecordFromMonsterId(pTarget->dwClassId);
    if (!pMonStats2Txt)
    {
        return 0;
    }

    int32_t nTotemMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if ((pMonStats2Txt->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_LARGE]) != 0)
    {
        nTotemMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileC);
    }
    else if ((pMonStats2Txt->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_SMALL]) != 0)
    {
        nTotemMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    }

    if (!CLTSKILL_IsValidMissileId(nTotemMissileId))
    {
        return 0;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x2001;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nTotemMissileId;
    tInit.nX = tTotemCoord.nX;
    tInit.nY = tTotemCoord.nY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    D2Client_CreateClientMissile(&tInit);

    tInit.nMissileId = MISSILE_CORPSEEXPLOSION; // 0x73 (115)
    D2Client_CreateClientMissile(&tInit);
    return 1;
}

// 1.12a: 0x6FB74B70 (gpfnCltDoFunc[42])
int32_t __fastcall CLTSKILL_Do_DoubleThrow(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pWeapon = D2Common_10434(pUnit, TRUE);
    if (!pWeapon)
    {
        return 0;
    }

    ITEMS_CheckIfStackable(pWeapon);
    if ((pUnit->dwSeqFrame & 0xFFFFFF00) >= 0x900)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    if (!ITEMS_CheckIfStackable(pWeapon)
        && STATLIST_UnitGetItemStatOrSkillStatValue(pWeapon, STAT_ITEM_THROWABLE, 0) != 0)
    {
        return 0;
    }

    const int32_t nMissileId = ITEMS_GetMissileType(pWeapon);
    if (nMissileId <= 0 || !CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_MISSILE_POTION))
    {
        if (!D2Client_SpawnLobbedMissile(pUnit, nMissileId, 0, 0, 0, 0, nSkillId, nSkillLevel, 0))
        {
            return 0;
        }
        return 1;
    }

    CLTSKILL_CreateMissileTowardUnitTarget(pUnit, nMissileId, nSkillId, nSkillLevel);
    return 1;
}

// 1.12a: 0x6FB74C30
void __fastcall CLTSKILL_CancelLeapCallback(D2UnitStrc* pUnit)
{
    const int32_t nExpectedFootprint = (pUnit->dwUnitType == UNIT_MONSTER) ? COLLIDE_MONSTER : COLLIDE_PLAYER;
    if (PATH_GetFootprintCollisionMask(pUnit->pDynamicPath) == nExpectedFootprint)
    {
        return;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    const int32_t nDestX = pSkill ? SKILLS_GetParam1(pSkill) : CLTSKILL_GetUnitXInline(pUnit);
    const int32_t nDestY = pSkill ? SKILLS_GetParam2(pSkill) : CLTSKILL_GetUnitYInline(pUnit);

    const uint32_t dwPattern = PATH_GetUnitCollisionPattern(pUnit);
    COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pUnit), nDestX, nDestY, dwPattern, static_cast<uint16_t>(nExpectedFootprint));
    D2Client_TeleportUnitToCoordinates(pUnit, nDestX, nDestY, 0);
    CLTSKILL_RestoreLeapCollisionAndPath(pUnit, nDestX, nDestY);
}

// 1.12a: 0x6FB74D10 (gpfnCltStFunc[31])
int32_t __fastcall CLTSKILL_St_Whirlwind(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    if (!pUnit || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER))
    {
        return 0;
    }

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    D2DynamicPathStrc* pPath = pUnit->pDynamicPath;
    if (!pSkill || !pPath)
    {
        return 0;
    }

    if (pUnit == gpPlayerUnit)
    {
        if (SKILLS_GetSkillIdFromSkill(pSkill, s_szSkillsBarFile, 1113) == nSkillId
            && (SKILLS_GetFlags(pSkill) & 0x01) != 0)
        {
            return 0;
        }
        if (STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
        {
            return 0;
        }
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    D2UnitStrc* pTarget = D2Client_ClearActionFrameAndGetTarget(pUnit);
    if (pTarget && UNITS_IsInMeleeRange(pUnit, pTarget, 0))
    {
        return CLTSKILL_DispatchMeleeFallbackOnTarget(pUnit, pTarget);
    }

    const bool bIsPlayer = (pUnit->dwUnitType == UNIT_PLAYER);
    const uint32_t dwDefaultMoveMask = bIsPlayer ? 0x1C09u : 0x3C01u;
    const uint32_t dwFootprintMask = bIsPlayer ? COLLIDE_PLAYER : COLLIDE_MONSTER;

    PATH_SetMoveTestCollisionMask(pPath, 0x0C01u);
    PATH_SetType(pPath, PATHTYPE_ASTAR);
    if (!D2Common_10142(pPath, pUnit, 0))
    {
        PATH_SetMoveTestCollisionMask(pPath, dwDefaultMoveMask);
        return 0;
    }

    if (bIsPlayer)
    {
        D2CharStatsTxt* pCharStatsTxt = CLTSKILL_GetCharStatsTxtRecord(pUnit->dwClassId);
        if (pCharStatsTxt)
        {
            const int32_t nWalkVelocity = static_cast<int32_t>(static_cast<uint8_t>(pCharStatsTxt->nWalkSpeed)) << 8;
            PATH_SetVelocity(pPath, nWalkVelocity, s_szSkillsBarFile, 1161);
        }
    }
    else
    {
        D2MonStatsTxt* pMonStatsTxt = CLTSKILL_GetMonStatsTxtRecord(pUnit->dwClassId);
        const int32_t nWalkVelocity = static_cast<int32_t>(pMonStatsTxt->nVelocity) << 8;
        PATH_SetVelocity(pPath, nWalkVelocity, s_szSkillsBarFile, 1167);
    }

    PATH_SetMoveTestCollisionMask(pPath, 0x0401u);

    D2PathPointStrc* pPathPoints = nullptr;
    const int32_t nNumPoints = PATH_GetPathPoints(pPath, &pPathPoints);
    if (nNumPoints == 0)
    {
        PATH_SetMoveTestCollisionMask(pPath, dwDefaultMoveMask);
        return 0;
    }

    const int32_t nEndX = static_cast<uint16_t>(pPathPoints[nNumPoints - 1].X);
    const int32_t nEndY = static_cast<uint16_t>(pPathPoints[nNumPoints - 1].Y);

    const uint32_t dwPattern = PATH_GetUnitCollisionPattern(pUnit);
    COLLISION_SetMaskWithPattern(UNITS_GetRoom(pUnit), nEndX, nEndY, dwPattern, dwFootprintMask);

    SKILLS_SetFlags(pSkill, 0x01);
    SKILLS_SetParam1(pSkill, nEndX);
    SKILLS_SetParam2(pSkill, nEndY);
    CLTSKILL_AttachMovementSkillCancelStatList(pUnit);
    return 1;
}

// 1.12a: 0x6FB74EF0 (gpfnCltStFunc[30])
int32_t __fastcall CLTSKILL_St_LeapAttack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill)
    {
        return 0;
    }

    if (pUnit == gpPlayerUnit)
    {
        if (STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
        {
            return 0;
        }
        if (SKILLS_GetSkillIdFromSkill(pSkill, s_szSkillsBarFile, 922) == nSkillId
            && (SKILLS_GetFlags(pSkill) & 0x01) != 0)
        {
            return 0;
        }
    }

    if (!pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    int32_t nLeapX = 0;
    int32_t nLeapY = 0;
    if (!SKILLS_CheckIfCanLeapTo(pUnit, pTarget, &nLeapX, &nLeapY))
    {
        if (pTarget && UNITS_IsInMeleeRange(pUnit, pTarget, 0))
        {
            return CLTSKILL_DispatchMeleeFallbackOnTarget(pUnit, pTarget);
        }
        if (pUnit->dwUnitType != UNIT_MONSTER)
        {
            D2Client_PlayUnitSpeechOrVoiceSound(pUnit, 19);
        }
        return 0;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
    {
        return 0;
    }

    const uint32_t dwPattern = PATH_GetUnitCollisionPattern(pUnit);
    COLLISION_SetMaskWithPattern(pRoom, nLeapX, nLeapY, dwPattern, COLLIDE_PLAYER);

    SKILLS_SetParam1(pSkill, nLeapX);
    SKILLS_SetParam2(pSkill, nLeapY);
    if (pTarget)
    {
        SKILLS_SetParam3(pSkill, static_cast<int32_t>(pTarget->dwUnitType));
        SKILLS_SetParam4(pSkill, static_cast<int32_t>(pTarget->dwUnitId));
    }
    else
    {
        SKILLS_SetParam3(pSkill, 6);
    }

    SKILLS_SetFlags(pSkill, 0x1080);
    CLTSKILL_AttachMovementSkillCancelStatList(pUnit);
    return 1;
}

// 1.12a: 0x6FB75060 (gpfnCltStFunc[29])
int32_t __fastcall CLTSKILL_St_Leap(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    if (pUnit == gpPlayerUnit)
    {
        if (STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
        {
            return 0;
        }
        if (SKILLS_GetSkillIdFromSkill(pSkill, s_szSkillsBarFile, 442) == nSkillId
            && (SKILLS_GetFlags(pSkill) & 0x01) != 0)
        {
            return 0;
        }
    }

    const bool bIsMonster = pUnit && (pUnit->dwUnitType == UNIT_MONSTER);
    int32_t nLeapX = 0;
    int32_t nLeapY = 0;

    if (bIsMonster)
    {
        D2UnitStrc* pTarget = D2Client_ClearActionFrameAndGetTarget(pUnit);
        D2CoordStrc tMonLeapCoord = {};
        if (!pTarget)
        {
            if (!CLTSKILL_GetUnitTargetCoords(pUnit, &tMonLeapCoord.nX, &tMonLeapCoord.nY))
            {
                return 0;
            }
            SKILLS_SetParam4(pSkill, -1);
        }
        else
        {
            const int32_t nTargetX = D2Client_GetUnitX_6FAB1590(pTarget);
            const int32_t nTargetY = D2Client_GetUnitY_6FAB15C0(pTarget);
            tMonLeapCoord.nX = 2 * nTargetX - D2Client_GetUnitX_6FAB1590(pUnit);
            tMonLeapCoord.nY = 2 * nTargetY - D2Client_GetUnitY_6FAB15C0(pUnit);
        }

        if (!COLLISION_GetFreeCoordinates(
                UNITS_GetRoom(pUnit),
                &tMonLeapCoord,
                UNITS_GetUnitSizeX(pUnit),
                0x3C01u,
                FALSE))
        {
            return 0;
        }

        nLeapX = tMonLeapCoord.nX;
        nLeapY = tMonLeapCoord.nY;
    }
    else
    {
        if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nLeapX, &nLeapY))
        {
            return 0;
        }
        if (!CLTSKILL_CheckLeapTargetRoomValid(nSkillId, pUnit, nLeapX, nLeapY))
        {
            return 0;
        }
        if (!CLTSKILL_ClampAndValidateLeapDestination(nSkillId, pUnit, nSkillLevel, &nLeapX, &nLeapY))
        {
            return 0;
        }
    }

    D2ActiveRoomStrc* pDestRoom = DUNGEON_FindRoomBySubtile(UNITS_GetRoom(pUnit), nLeapX, nLeapY);
    const uint32_t dwPattern = PATH_GetUnitCollisionPattern(pUnit);
    COLLISION_SetMaskWithPattern(pDestRoom, nLeapX, nLeapY, dwPattern, COLLIDE_PLAYER);

    SKILLS_SetParam1(pSkill, nLeapX);
    SKILLS_SetParam2(pSkill, nLeapY);

    if (bIsMonster)
    {
        D2UnitStrc* pTarget = D2Client_ClearActionFrameAndGetTarget(pUnit);
        if (pTarget)
        {
            SKILLS_SetParam3(pSkill, static_cast<int32_t>(pTarget->dwUnitType));
            SKILLS_SetParam4(pSkill, static_cast<int32_t>(pTarget->dwUnitId));
        }
    }

    SKILLS_SetFlags(pSkill, 0x1080);
    CLTSKILL_AttachMovementSkillCancelStatList(pUnit);
    return 1;
}

// ============================================================================
// SECTION 4A: Druid & Expansion Monster Client Skills
// (`..\Source\D2Client\SKILLS\SkillsEMon.cpp` & Druid Elemental/Vine Skills)
// VA Range: 0x6FB76050 .. 0x6FB77C0F (26 procedures)
// ============================================================================

extern D2UnitStrc* __fastcall D2Client_ResolveUnitOwner(D2UnitStrc* pUnit); // 0x6FACF710 (UNIT_CUnit.cpp)
extern int32_t     __fastcall D2Client_DestroyClientMissile(D2UnitStrc* pMissile, D2UnitStrc* pTarget, int32_t bForceExpire); // 0x6FAD2E50 (UNIT_PlrSkills.cpp)
extern D2UnitStrc* __fastcall D2Client_SpawnFallingTargetMissile(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel); // 0x6FADB400 (UNIT_Missile.cpp)
extern D2UnitStrc* __fastcall CLTSKILL_SpawnInfernoArcticFlameMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel); // 0x6FB02990 (SKILLS_Skills.cpp)
extern void        __fastcall CLTSKILL_InitBezierVineMissile(D2UnitStrc* pMissile, int32_t nSeedOffset); // 0x6FB01BC0 (SKILLS_Skills.cpp)
extern void        __fastcall D2Client_GetUnitComponentScreenOffset(D2UnitStrc* pUnit, int32_t nComponent, int32_t* pOutOffsetX, int32_t* pOutOffsetY); // 0x6FB42490 (ENGINE_Gfx.cpp)
extern void        __stdcall  PARTICLE_SetPosition(void* pUnit, int32_t nPosX, int32_t nPosY, int32_t nPosZ); // 0x6FB755B0 (DRAW_dLightMap.cpp)
extern int32_t     __fastcall PARTICLE_GetIntPosZ(void* pUnit); // 0x6FB75640 (DRAW_dLightMap.cpp)
extern void        __stdcall  PARTICLE_UpdateUnitPhysics(void* pUnit); // 0x6FB75730 (DRAW_dLightMap.cpp)

extern uint32_t g_dwShakeStartTick;    // 0x6FBAD104 (GAME_Game.cpp)
extern int32_t  g_nShakeRampUpMs;      // 0x6FBAD108 (GAME_Game.cpp)
extern int32_t  g_nShakeHoldMs;        // 0x6FBAD10C (GAME_Game.cpp)
extern int32_t  g_nShakeRampDownMs;    // 0x6FBAD110 (GAME_Game.cpp)
extern int32_t  g_nShakeMaxAmplitude;  // 0x6FBCB9B0 (GAME_Game.cpp)

// Mirrors 0x6FAFD950 + 0x6FAFD910: computes a unit-step perpendicular vector for line-wave skills
static inline void CLTSKILL_ComputePerpendicularLineStep(int32_t* pInOutStepX, int32_t* pInOutStepY)
{
    int32_t nDx = *pInOutStepX;
    int32_t nDy = *pInOutStepY;
    int32_t nLenSq = nDx * nDx + nDy * nDy;
    if (nLenSq < 4)
    {
        nDx *= 4;
        nDy *= 4;
        nLenSq = nDx * nDx + nDy * nDy;
    }
    if (nLenSq < 16)
    {
        nDx *= 2;
        nDy *= 2;
    }

    int32_t nPerpX = nDy;
    int32_t nPerpY = -nDx;
    while ((nPerpX * nPerpX + nPerpY * nPerpY) > 3)
    {
        nPerpX /= 2;
        nPerpY /= 2;
    }

    *pInOutStepX = nPerpX;
    *pInOutStepY = nPerpY;
}

// Mirrors 0x6FB01C20: scales missile animation speed so Missiles.txt nTotalFrames plays over nLifetimeFrames
static inline void CLTSKILL_SyncMissileAnimSpeedToFrames(D2UnitStrc* pMissile, int32_t nLifetimeFrames)
{
    if (!pMissile || pMissile->dwUnitType != UNIT_MISSILE || nLifetimeFrames == 0)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxtRecord(pMissile->dwClassId);
    if (pMissilesTxt)
    {
        const int32_t nTotalAnimPrecise = static_cast<int32_t>(pMissilesTxt->nAnimLen) << 8;
        UNITS_SetAnimationSpeed(pMissile, nTotalAnimPrecise / nLifetimeFrames);
    }
}

// 1.12a: 0x6FB76050
int32_t __fastcall CLTSKILL_IsVineBeastNeutralMissile(D2UnitStrc* pUnit, void* pCallbackArg)
{
    (void)pCallbackArg;

    if (!pUnit || pUnit->dwUnitType != UNIT_MISSILE)
    {
        return 0;
    }
    return (pUnit->dwClassId == MISSILE_VINE_BEAST_NEUTRAL) ? 1 : 0; // 0x1D4 (468)
}

// 1.12a: 0x6FB76070 (gpfnCltDoFunc[79])
int32_t __fastcall CLTSKILL_Do_ImpTeleport(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nTargetX = SKILLS_GetParam1(pSkill);
    const int32_t nTargetY = SKILLS_GetParam2(pSkill);
    if (nTargetX == 0 || nTargetY == 0)
    {
        return 0;
    }

    D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nTargetX, nTargetY);

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (CLTSKILL_IsValidMissileId(nMissileId))
    {
        CLTSKILL_CreateMissileAtCoords(pUnit, nMissileId, nTargetX, nTargetY, nSkillId, nSkillLevel);
    }

    return 1;
}

// 1.12a: 0x6FB76130 (gpfnCltStFunc[52])
int32_t __fastcall CLTSKILL_St_ArcticBlast_Inferno(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    int32_t nDurationFrames = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[1], nSkillId, nSkillLevel);
    if (nDurationFrames < 1)
    {
        nDurationFrames = 1;
    }

    SKILLS_SetParam1(pSkill, static_cast<int32_t>(gdwClientGameTick) + nDurationFrames);
    if (!STATES_CheckState(pUnit, STATE_INFERNO))
    {
        STATES_ToggleState(pUnit, STATE_INFERNO, TRUE);
    }

    return 1;
}

// 1.12a: 0x6FB761D0
int32_t __fastcall CLTSKILL_SpawnStationaryNonRotatingMissile(
    D2UnitStrc* pUnit,
    int32_t nMissileId,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t bMarkDeadFlag)
{
    const int32_t nUnitY = CLTSKILL_GetUnitYInline(pUnit);
    const int32_t nUnitX = CLTSKILL_GetUnitXInline(pUnit);

    D2UnitStrc* pMissile = CLTSKILL_CreateMissileAtCoords(pUnit, nMissileId, nUnitX, nUnitY, nSkillId, nSkillLevel);
    if (!pMissile)
    {
        return 0;
    }

    const uint8_t nCasterDir = UNITS_GetDirection(pUnit);
    PATH_SetDirection(pMissile->pDynamicPath, nCasterDir);
    D2COMMON_10203_PATH_SetRotateFlag(pMissile->pDynamicPath, FALSE);

    if (bMarkDeadFlag != 0)
    {
        pMissile->dwFlags |= UNITFLAG_ISDEAD;
    }

    return 1;
}

// 1.12a: 0x6FB76290 (gpfnCltDoFunc[73])
int32_t __fastcall CLTSKILL_Do_SpawnCltMissileBAtCaster(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (!CLTSKILL_IsValidMissileId(nMissileB))
    {
        return 1;
    }

    D2UnitStrc* pMissile = CLTSKILL_CreateMissileAtCoords(
        pUnit,
        nMissileB,
        D2Client_GetUnitX_6FAB1590(pUnit),
        D2Client_GetUnitY_6FAB15C0(pUnit),
        nSkillId,
        1);
    return pMissile ? 1 : 0;
}

// 1.12a: 0x6FB76310 (gpfnCltStFunc[48])
int32_t __fastcall CLTSKILL_St_SpawnCltMissileAAtCaster(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileA = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (CLTSKILL_IsValidMissileId(nMissileA))
    {
        CLTSKILL_CreateMissileAtCoords(
            pUnit,
            nMissileA,
            D2Client_GetUnitX_6FAB1590(pUnit),
            D2Client_GetUnitY_6FAB15C0(pUnit),
            nSkillId,
            nSkillLevel);
    }

    return 1;
}

// 1.12a: 0x6FB76380 (gpfnCltDoFunc[82])
int32_t __fastcall CLTSKILL_Do_BaalCorpseExplode(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nRadius = 0;
    D2SkillsTxt* pBaalCorpseTxt = CLTSKILL_GetSkillsTxtRecord(SKILL_BAALCORPSEEXPLODE); // 0x11D (285)
    if (pBaalCorpseTxt && nSkillLevel > 0)
    {
        nRadius = pBaalCorpseTxt->dwParam[4] + (nSkillLevel - 1) * pBaalCorpseTxt->dwParam[5];
    }

    const int32_t nOriginX = CLTSKILL_GetUnitXInline(pUnit);
    const int32_t nOriginY = CLTSKILL_GetUnitYInline(pUnit);

    D2UnitFindArgStrc tFindArg = {};
    tFindArg.nFlags = 0x3002;
    tFindArg.pUnit = pUnit;
    tFindArg.nX = nOriginX;
    tFindArg.nY = nOriginY;
    tFindArg.nSize = nRadius;

    D2UnitFindDataStrc tFindData = {};
    UNITFINDS_InitializeUnitFindData(
        nullptr,
        &tFindData,
        UNITS_GetRoom(pUnit),
        nOriginX,
        nOriginY,
        nRadius,
        nullptr,
        &tFindArg);
    UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(&tFindData);

    for (int32_t i = 0; i < tFindData.nIndex; ++i)
    {
        D2UnitStrc* pCorpse = tFindData.pUnitsArray[i];
        const int32_t nCorpseY = CLTSKILL_GetUnitYInline(pCorpse);
        const int32_t nCorpseX = CLTSKILL_GetUnitXInline(pCorpse);

        D2ClientMissileInitStrc tInit = {};
        tInit.dwFlags = 0x01;
        tInit.pOwner = pUnit;
        tInit.nMissileId = MISSILE_BAALCORPSEEXPLODEDELAY; // 0x24B (587)
        tInit.nX = nCorpseX;
        tInit.nY = nCorpseY;
        tInit.nSkillId = nSkillId;
        tInit.nSkillLevel = nSkillLevel;

        D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
        if (pMissile)
        {
            const int32_t nBaseFrames = MISSILE_GetTotalFrames(pMissile);
            const int32_t nJitter = SEED_RollLimitedRandomNumber(&pUnit->pSeed, 20);
            const int32_t nAdjustedFrames = nBaseFrames + nJitter - 10;
            MISSILE_SetTotalFrames(pMissile, nAdjustedFrames);
            MISSILE_SetCurrentFrame(pMissile, nAdjustedFrames);
        }
    }

    UNITFINDS_FreeUnitFindData(&tFindData);
    return 1;
}

// 1.12a: 0x6FB765F0 (gpfnCltDoFunc[88])
int32_t __fastcall CLTSKILL_Do_UpdateRiderPath(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2UnitStrc* pMount = D2Client_ResolveUnitOwner(pUnit);
    if (!pMount)
    {
        return 0;
    }

    D2Common_10235_PATH_UpdateRiderPath(pUnit, pMount);
    return 1;
}

// 1.12a: 0x6FB76620 (gpfnCltStFunc[51])
int32_t __fastcall CLTSKILL_St_Corpse_VineCycler(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || !pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget || !D2COMMON_11018(pTarget))
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, D2Client_GetUnitX_6FAB1590(pTarget));
    SKILLS_SetParam2(pSkill, D2Client_GetUnitY_6FAB15C0(pTarget));
    SKILLS_SetParam3(pSkill, pTarget->dwClassId);
    return 1;
}

// 1.12a: 0x6FB766B0 (gpfnCltStFunc[50])
int32_t __fastcall CLTSKILL_St_SaveTargetCoords(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || !pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, CLTSKILL_GetUnitXInline(pTarget));
    SKILLS_SetParam2(pSkill, CLTSKILL_GetUnitYInline(pTarget));
    return 1;
}

// 1.12a: 0x6FB76790 (gpfnCltDoFunc[75])
int32_t __fastcall CLTSKILL_Do_SiegeBeastStomp(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)pUnit;
    (void)nSkillLevel;

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    const int32_t nAmplitude  = pSkillsTxt ? pSkillsTxt->dwParam[0] : 0;
    const int32_t nRampUpMs   = pSkillsTxt ? ((pSkillsTxt->dwParam[1] * 1000) / 25) : 0;
    const int32_t nSustainMs  = pSkillsTxt ? ((pSkillsTxt->dwParam[2] * 1000) / 25) : 0;
    const int32_t nRampDownFrames = pSkillsTxt ? pSkillsTxt->dwParam[3] : 0;

    if (nSustainMs != 0)
    {
        g_dwShakeStartTick = GetTickCount();
        g_nShakeRampUpMs = nRampUpMs;
        g_nShakeHoldMs = nSustainMs;
        g_nShakeRampDownMs = (nRampDownFrames * 1000) / 25;
        g_nShakeMaxAmplitude = nAmplitude;
    }

    return 1;
}

// 1.12a: 0x6FB768C0 (gpfnCltDoFunc[81])
int32_t __fastcall CLTSKILL_Do_DirectionalMissileWithPerpTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_GetSkillsTxtRecord(nSkillId))
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    D2UnitStrc* pMissile = CLTSKILL_CreateMissileTowardUnitTarget(pUnit, nMissileId, nSkillId, nSkillLevel);
    if (!pMissile)
    {
        return 0;
    }

    const int32_t nDx = nTargetX - D2Client_GetUnitX_6FAB1590(pUnit);
    const int32_t nDy = nTargetY - D2Client_GetUnitY_6FAB15C0(pUnit);
    MISSILE_SetTargetX(pMissile, -nDy);
    MISSILE_SetTargetY(pMissile, nDx);
    return 1;
}

// 1.12a: 0x6FB76990 (gpfnCltDoFunc[78])
int32_t __fastcall CLTSKILL_Do_SpawnSavedCoordMissiles(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nSavedX = SKILLS_GetParam1(pSkill);
    const int32_t nSavedY = SKILLS_GetParam2(pSkill);
    if (nSavedX == 0 || nSavedY == 0)
    {
        return 0;
    }

    const int32_t nMissileA = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (CLTSKILL_IsValidMissileId(nMissileA))
    {
        D2UnitStrc* pMissileA = CLTSKILL_CreateMissileAtCoords(pUnit, nMissileA, nSavedX, nSavedY, nSkillId, nSkillLevel);
        if (!pMissileA)
        {
            return 0;
        }

        const int16_t nPrgSound = static_cast<int16_t>(pSkillsTxt->wPrgSound);
        if (nPrgSound > 0)
        {
            D2Client_PlayUnitSound_6FAE1FA0(nPrgSound, pMissileA, 0, 0, 0);
        }
    }

    const int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (CLTSKILL_IsValidMissileId(nMissileB))
    {
        CLTSKILL_CreateMissileAtCoords(pUnit, nMissileB, nSavedX, nSavedY, nSkillId, nSkillLevel);
    }

    return 1;
}

// 1.12a: 0x6FB76AA0 (gpfnCltDoFunc[76])
int32_t __fastcall CLTSKILL_Do_OverseerWhip(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nChainMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    D2MissilesTxt* pChainMissileTxt = CLTSKILL_GetMissilesTxtRecord(nChainMissileId);
    if (!pChainMissileTxt)
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    int32_t nTotalDist = UNITS_GetDistanceToCoordinates(pUnit, nTargetX, nTargetY);
    if (nTotalDist < 1)
    {
        nTotalDist = 1;
    }

    const int32_t nOriginX = D2Client_GetUnitX_6FAB1590(pUnit);
    const int32_t nOriginY = D2Client_GetUnitY_6FAB15C0(pUnit);
    const int32_t nStepScaledX = ((nTargetX - nOriginX) * 100) / nTotalDist;
    const int32_t nStepScaledY = ((nTargetY - nOriginY) * 100) / nTotalDist;

    int32_t nDistStride = pChainMissileTxt->dwParam[0];
    if (nDistStride < 1)
    {
        nDistStride = 1;
    }

    int32_t nDelayPerSegment = pChainMissileTxt->dwParam[1];
    if (nDelayPerSegment < 1)
    {
        nDelayPerSegment = 1;
    }

    int32_t nSegmentCount = 0;
    int32_t nAccumDelay = 0;
    int32_t nRunningX = nDistStride * nStepScaledX;
    int32_t nRunningY = nDistStride * nStepScaledY;
    const int32_t nDeltaScaledX = nRunningX;
    const int32_t nDeltaScaledY = nRunningY;

    for (int32_t nCurDist = nDistStride; nCurDist < nTotalDist; nCurDist += nDistStride)
    {
        D2UnitStrc* pSegment = CLTSKILL_CreateMissileAtCoords(
            pUnit,
            nChainMissileId,
            nOriginX + (nRunningX / 100),
            nOriginY + (nRunningY / 100),
            nSkillId,
            nSkillLevel);
        if (pSegment)
        {
            D2COMMON_10160_PathUpdateDirection(pSegment->pDynamicPath, UNITS_GetDirection(pUnit));
            const int32_t nBaseFrames = MISSILE_GetTotalFrames(pSegment);
            MISSILE_SetTotalFrames(pSegment, nBaseFrames + nAccumDelay);
            MISSILE_SetCurrentFrame(pSegment, nBaseFrames + nAccumDelay);
            MISSILE_SetActivateFrame(pSegment, nBaseFrames - nAccumDelay);
            pSegment->dwFlagEx |= UNITFLAGEX_NODRAW;
        }

        nRunningX += nDeltaScaledX;
        nRunningY += nDeltaScaledY;
        nAccumDelay += nDelayPerSegment;
        ++nSegmentCount;
    }

    const int32_t nTipMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (CLTSKILL_IsValidMissileId(nTipMissileId))
    {
        D2UnitStrc* pTipMissile = CLTSKILL_CreateMissileAtUnitTarget(pUnit, nTipMissileId, nSkillId, nSkillLevel);
        if (!pTipMissile)
        {
            return 0;
        }

        const int32_t nAnimSpeed = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
        if (nAnimSpeed > 0)
        {
            UNITS_SetAnimationSpeed(pTipMissile, nAnimSpeed);
        }

        const int32_t nTotalDelay = nSegmentCount * nDelayPerSegment;
        const int32_t nTipBaseFrames = MISSILE_GetTotalFrames(pTipMissile);
        MISSILE_SetTotalFrames(pTipMissile, nTipBaseFrames + nTotalDelay);
        MISSILE_SetCurrentFrame(pTipMissile, nTipBaseFrames + nTotalDelay);
        MISSILE_SetActivateFrame(pTipMissile, nTipBaseFrames - nTotalDelay);
        pTipMissile->dwFlagEx |= UNITFLAGEX_NODRAW;
    }

    return 1;
}

// 1.12a: 0x6FB76D80 (gpfnCltStFunc[46])
int32_t __fastcall CLTSKILL_St_BaalTaunt(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nResolvedMissile = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nResolvedMissile))
    {
        return 0;
    }

    const int16_t nPrgSound = static_cast<int16_t>(pSkillsTxt->wPrgSound);
    if (nPrgSound > 0)
    {
        D2Client_PlayUnitSound_6FAE1FA0(nPrgSound, nullptr, 0, 0, 0);
    }

    CLTSKILL_CreateMissileAtUnitTarget(pUnit, MISSILE_BAAL_TAUNT_CONTROL, nSkillId, nSkillLevel); // 0x222 (546)
    return 1;
}

// 1.12a: 0x6FB76E10 (gpfnCltDoFunc[74])
int32_t __fastcall CLTSKILL_Do_SuckBlood(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (!CLTSKILL_IsValidMissileId(nMissileB))
    {
        return 1;
    }

    D2UnitStrc* pMissile = CLTSKILL_CreateMissileAtUnitTarget(pUnit, nMissileB, nSkillId, 1);
    if (!pMissile)
    {
        return 0;
    }

    D2UnitStrc* pTarget = D2Client_ClearActionFrameAndGetTarget(pUnit);
    if (pTarget)
    {
        PARTICLE_AllocForUnit(pMissile);
        int32_t nCompOffsetX = 0;
        int32_t nCompOffsetY = 0;
        D2Client_GetUnitComponentScreenOffset(pTarget, 14, &nCompOffsetX, &nCompOffsetY);
        PARTICLE_SetPosition(pMissile, nCompOffsetX, 0, -nCompOffsetY);
    }

    return 1;
}

// 1.12a: 0x6FB76EB0 (gpfnCltStFunc[47])
int32_t __fastcall CLTSKILL_St_Raven_DruidSummon(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxtRecord(nMissileId);
    if (!pMissilesTxt)
    {
        return 0;
    }

    int32_t nVecX = 0;
    int32_t nVecY = 0;
    D2Common_11066(pUnit, &nVecX, &nVecY);
    if (nVecX == 0 && nVecY == 0)
    {
        return 0;
    }

    D2UnitStrc* pMissile = CLTSKILL_CreateMissileAtUnitTarget(pUnit, nMissileId, nSkillId, nSkillLevel);
    if (!pMissile)
    {
        return 0;
    }

    int32_t nRange = static_cast<int16_t>(pMissilesTxt->wRange);
    if (nRange < 1)
    {
        nRange = 1;
    }

    const int32_t nVertSpeed = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);

    int32_t nStartSubtileX = -(nRange * nVecX);
    int32_t nStartSubtileY = -(nRange * nVecY);
    DUNGEON_GameSubtileToClientCoords(&nStartSubtileX, &nStartSubtileY);

    int32_t nStepSubtileX = nVecX;
    int32_t nStepSubtileY = nVecY;
    DUNGEON_GameSubtileToClientCoords(&nStepSubtileX, &nStepSubtileY);

    PARTICLE_SetPosition(pMissile, nStartSubtileX, nStartSubtileY, nVertSpeed * nRange);

    D2ParticleStrc* pParticle = CLTSKILL_GetUnitParticle(pMissile);
    if (pParticle)
    {
        pParticle->nVelX = nStepSubtileX << 11;
        pParticle->nVelY = nStepSubtileY << 11;
        pParticle->nVelZ = (-nVertSpeed) << 11;
    }

    PARTICLE_UpdateUnitPhysics(pMissile);
    return 1;
}

// 1.12a: 0x6FB77050 (gpfnCltDoFunc[89])
int32_t __fastcall CLTSKILL_Do_VineAttack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    D2SkillStrc* pVineSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, SKILL_VINEATTACK); // 0x126 (294)
    if (!pVineSkill)
    {
        SKILLS_AssignSkill(pUnit, SKILL_VINEATTACK, 1, FALSE, s_szSkillsEMonFile, 860);
        pVineSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, SKILL_VINEATTACK);
        if (!pVineSkill)
        {
            return 0;
        }
    }

    const int32_t nActualSkillId = SKILLS_GetSkillIdFromSkill(pVineSkill, s_szSkillsEMonFile, 868);
    const int32_t nActualLevel = SKILLS_GetSkillLevel(pUnit, pVineSkill, TRUE);

    const int32_t nTickCount = SKILLS_GetParam1(pVineSkill);
    SKILLS_SetParam1(pVineSkill, nTickCount + 1);
    const int32_t nLastX = SKILLS_GetParam2(pVineSkill);
    const int32_t nLastY = SKILLS_GetParam3(pVineSkill);

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nActualSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    bool bShouldSpawn = false;
    const int32_t nMaxTicks = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[1], nActualSkillId, nActualLevel);
    if (nTickCount >= nMaxTicks)
    {
        bShouldSpawn = true;
    }

    const int32_t nMaxDist = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[1], nActualSkillId, nActualLevel);
    if (static_cast<int32_t>(UNITS_GetDistanceToCoordinates(pUnit, nLastX, nLastY)) >= nMaxDist)
    {
        bShouldSpawn = true;
    }

    int32_t nSpawnMissileId = -1;
    if (pUnit->dwAnimMode == MONMODE_NEUTRAL)
    {
        if (bShouldSpawn)
        {
            D2UnitStrc* pExistingNeutral = D2Common_10407(
                UNITS_GetRoom(pUnit),
                D2Client_GetUnitX_6FAB1590(pUnit),
                D2Client_GetUnitY_6FAB15C0(pUnit),
                reinterpret_cast<int(__fastcall*)(D2UnitStrc*, void*)>(CLTSKILL_IsVineBeastNeutralMissile),
                nullptr,
                1);
            if (pExistingNeutral)
            {
                D2Client_DestroyClientMissile(pExistingNeutral, nullptr, 0);
            }
        }
        nSpawnMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileD);
    }
    else if (pUnit->dwAnimMode == MONMODE_WALK)
    {
        nSpawnMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileC) + SEED_RollLimitedRandomNumber(&pUnit->pSeed, 2);
    }
    else
    {
        return 1;
    }

    if (bShouldSpawn && nSpawnMissileId > 0)
    {
        SKILLS_SetParam1(pVineSkill, 0);
        SKILLS_SetParam2(pVineSkill, D2Client_GetUnitX_6FAB1590(pUnit));
        SKILLS_SetParam3(pVineSkill, D2Client_GetUnitY_6FAB15C0(pUnit));
        CLTSKILL_SpawnStationaryNonRotatingMissile(pUnit, nSpawnMissileId, nActualSkillId, nActualLevel, 1);
    }

    return 1;
}

// 1.12a: 0x6FB77240 (gpfnCltDoFunc[80])
int32_t __fastcall CLTSKILL_Do_WakeOfDestruction_BaalCold(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    const int32_t nWaveWidth = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);

    int32_t nPerpX = nTargetX - D2Client_GetUnitX_6FAB1590(pUnit);
    int32_t nPerpY = nTargetY - D2Client_GetUnitY_6FAB15C0(pUnit);
    CLTSKILL_ComputePerpendicularLineStep(&nPerpX, &nPerpY);

    int32_t nAimX = nTargetX - ((nPerpX * nWaveWidth) / 2);
    int32_t nAimY = nTargetY - ((nPerpY * nWaveWidth) / 2);

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x20;
    tInit.pOwner = pUnit;
    tInit.pOrigin = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    tInit.nSubLoopIndex = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[1], nSkillId, nSkillLevel);

    for (int32_t i = 0; i < nWaveWidth; ++i)
    {
        tInit.nTargetX = nAimX;
        tInit.nTargetY = nAimY;
        D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
        if (pMissile)
        {
            CLTSKILL_SyncMissileAnimSpeedToFrames(pMissile, MISSILE_GetTotalFrames(pMissile));
        }

        nAimX += nPerpX;
        nAimY += nPerpY;
    }

    if (STATES_CheckState(pUnit, STATE_INFERNO))
    {
        UNITS_SetAnimationFrame(pUnit, 8);
        pUnit->dwFrameCountPrecise = 0x600;
    }

    return 1;
}

// 1.12a: 0x6FB77430 (gpfnCltDoFunc[93])
int32_t __fastcall CLTSKILL_Do_FenrisRage(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (!CLTSKILL_IsValidMissileId(nMissileB))
    {
        return 0;
    }

    D2UnitStrc* pTarget = D2Client_ClearActionFrameAndGetTarget(pUnit);
    if (!pTarget)
    {
        return 0;
    }

    D2Client_SpawnCorpseGoreMissiles(pTarget, 0, 3);

    D2ClientMissileInitStrc tInit = {};
    tInit.pOwner = pTarget;
    tInit.pOrigin = pTarget;
    tInit.nMissileId = nMissileB;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    D2Client_CreateClientMissile(&tInit);
    return 1;
}

// 1.12a: 0x6FB774F0 (gpfnCltDoFunc[77])
int32_t __fastcall CLTSKILL_Do_ArcticBlast_Channel(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    if (gdwClientGameTick > static_cast<uint32_t>(SKILLS_GetParam1(pSkill)))
    {
        STATES_ToggleState(pUnit, STATE_INFERNO, FALSE);
        return 0;
    }

    D2UnitStrc* pMissile = CLTSKILL_SpawnInfernoArcticFlameMissile(pUnit, nSkillId, nSkillLevel);
    if (!pMissile)
    {
        return 0;
    }

    PARTICLE_AllocForUnit(pMissile);
    const int32_t nStartZ = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);
    PARTICLE_SetPosition(pMissile, 0, 0, nStartZ);

    D2ParticleStrc* pParticle = CLTSKILL_GetUnitParticle(pMissile);
    if (pParticle)
    {
        pParticle->dwFlags |= 0x20u;
    }

    if (pUnit)
    {
        pUnit->dwFlags &= ~UNITFLAG_SKSRVDOFUNC;
    }

    return 1;
}

// 1.12a: 0x6FB775C0
int32_t __fastcall CLTSKILL_SpawnDruidVineCluster(
    D2UnitStrc* pUnit,
    int32_t nMissileId,
    int32_t nCount,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x03;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    tInit.pfPostInit = CLTSKILL_InitBezierVineMissile;

    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &tInit.nX, &tInit.nY))
    {
        return 0;
    }

    for (int32_t i = 0; i < nCount; ++i)
    {
        tInit.nTargetX = g_aDruidVineOffsetX_6FB838D8[i & 7];
        tInit.nTargetY = g_aDruidVineOffsetY_6FB838F8[i & 7];
        tInit.nPostInitArg = i;
        D2Client_CreateClientMissile(&tInit);
    }

    return 1;
}

// 1.12a: 0x6FB77660 (gpfnCltDoFunc[71])
int32_t __fastcall CLTSKILL_Do_Eruption_MoltenBoulder(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileA = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxtRecord(nMissileA);
    if (!pMissilesTxt)
    {
        return 0;
    }

    int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (!CLTSKILL_IsValidMissileId(nMissileB))
    {
        nMissileB = -1;
    }

    D2UnitStrc* pTarget = D2Client_ClearActionFrameAndGetTarget(pUnit);
    if (pTarget)
    {
        D2COMMON_10170_PathSetTargetPos(
            pUnit->pDynamicPath,
            D2Client_GetUnitX_6FAB1590(pTarget),
            D2Client_GetUnitY_6FAB15C0(pTarget));
        D2Client_FaceUnitTowardDynamicTarget(pUnit, 1);
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    if (!pTarget)
    {
        UNITS_UpdateDirectionAndSpeed(pUnit, nTargetX, nTargetY);
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x25;
    tInit.pOwner = pUnit;
    tInit.nX = D2Client_GetUnitX_6FAB1590(pUnit);
    tInit.nY = D2Client_GetUnitY_6FAB15C0(pUnit);
    tInit.nTargetX = nTargetX;
    tInit.nTargetY = nTargetY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    const int32_t nStartZ = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel)
                          + PARTICLE_GetIntPosZ(pUnit);
    const int32_t nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[1], nSkillId, nSkillLevel)
                         + pMissilesTxt->dwParam[1];

    int32_t nAnimSpeed = static_cast<int32_t>(pMissilesTxt->nAnimLen) << 8;
    int32_t nVelZ = -(2 * nStartZ);
    if (nRange != 0)
    {
        nAnimSpeed /= nRange;
        nVelZ /= nRange;
    }

    tInit.nMissileId = nMissileA;
    if (nMissileB > 0 && SEED_RollLimitedRandomNumber(&pUnit->pSeed, 2) != 0)
    {
        tInit.nMissileId = nMissileB;
    }

    const int32_t nVelIdx = SEED_RollLimitedRandomNumber(&pUnit->pSeed, 5);
    tInit.nVelocity = static_cast<int32_t>(pMissilesTxt->nVel) + g_aMoltenBoulderVelDeltas_6FB83918[nVelIdx];

    D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
    if (pMissile)
    {
        PATH_SetNewDistance(pMissile->pDynamicPath, nRange);
        UNITS_SetAnimationSpeed(pMissile, nAnimSpeed);
        MISSILE_SetTotalFrames(pMissile, nRange);
        MISSILE_SetCurrentFrame(pMissile, nRange);
        PARTICLE_SetPosition(pMissile, 0, 0, nStartZ);
        PARTICLE_UpdateUnitPhysics(pMissile);

        D2ParticleStrc* pParticle = CLTSKILL_GetUnitParticle(pMissile);
        if (pParticle)
        {
            pParticle->nVelX = 0;
            pParticle->nVelY = 0;
            pParticle->nVelZ = nVelZ << 11;
        }
    }

    return 1;
}

// 1.12a: 0x6FB77910 (gpfnCltDoFunc[70])
int32_t __fastcall CLTSKILL_Do_Firestorm(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_GetSkillsTxtRecord(nSkillId))
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nDeltaX = nTargetX - D2Client_GetUnitX_6FAB1590(pUnit);
    const int32_t nDeltaY = nTargetY - D2Client_GetUnitY_6FAB15C0(pUnit);

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x02;
    tInit.pOwner = pUnit;
    tInit.pOrigin = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nTargetX = nDeltaX;
    tInit.nTargetY = nDeltaY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
    if (!pMissile)
    {
        return 0;
    }

    MISSILE_SetTargetX(pMissile, -nDeltaY);
    MISSILE_SetTargetY(pMissile, nDeltaX);
    return 1;
}

// 1.12a: 0x6FB77A30 (gpfnCltStFunc[49])
int32_t __fastcall CLTSKILL_St_Vines_MinionSpawner(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nMissileA = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (CLTSKILL_IsValidMissileId(nMissileA))
    {
        CLTSKILL_SpawnStationaryNonRotatingMissile(pUnit, nMissileA, nSkillId, nSkillLevel, 0);
    }

    const int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (CLTSKILL_IsValidMissileId(nMissileB))
    {
        const int32_t nCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);
        CLTSKILL_SpawnDruidVineCluster(pUnit, nMissileB, nCount, nSkillId, nSkillLevel);
    }

    return 1;
}

// 1.12a: 0x6FB77AE0 (gpfnCltDoFunc[72])
int32_t __fastcall CLTSKILL_Do_Meteor_Volcano_Catapult(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pMissile = D2Client_SpawnFallingTargetMissile(pUnit, nMissileId, nSkillId, nSkillLevel);
    if (!pMissile)
    {
        return 0;
    }

    const int32_t nTotalFrames = MISSILE_GetTotalFrames(pMissile);
    const int32_t nStartHeight = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[1], nSkillId, nSkillLevel)
                               + PARTICLE_GetIntPosZ(pUnit);
    const int32_t nFallSpeed = (nTotalFrames != 0) ? (nStartHeight / nTotalFrames) : (nStartHeight / 2);

    PARTICLE_SetPosition(pMissile, 0, 0, nStartHeight);

    D2ParticleStrc* pParticle = CLTSKILL_GetUnitParticle(pMissile);
    if (pParticle)
    {
        pParticle->nVelX = 0;
        pParticle->nVelY = 0;
        pParticle->nVelZ = (-nFallSpeed) << 11;
    }

    PARTICLE_UpdateUnitPhysics(pMissile);
    return 1;
}


// ============================================================================
// SECTION 4B: Necromancer, Proc Skills (`SCmd 0x99/0x9A`), PvP Alignment,
//             Druid Shapeshift/Elemental & Assassin Client Skills
// (`..\Source\D2Client\SKILLS\SkillsNec.cpp`, `PlayerList.cpp`, `SkillsAss.cpp`)
// VA Range: 0x6FB77C10 .. 0x6FB78965 & 0x6FB789F0 .. 0x6FB798C4 (31 procedures)
// (0x6FB78970..0x6FB789EC skipped: zlib 1.1.4 inflate wrapper; >=0x6FB798C7: CRT)
// ============================================================================

extern BOOL        __fastcall D2Client_IsNormalSwitchableAIMonster(D2UnitStrc* pUnit); // 0x6FAE3AE0 (UNIT_Monster.cpp)
extern void        __fastcall D2Client_SpawnRadialSubMissileBurst(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nRangeOverride, int32_t nSecondaryStride, int32_t nPrimaryStride); // 0x6FAD85F0 (UNIT_Missile.cpp)
extern void        __fastcall D2Client_FreeUnitLight(D2UnitStrc* pUnit); // 0x6FACEAA0 (UNIT_CUnit.cpp)
extern int32_t     __fastcall D2Client_IsUnitDeadOrDying(D2UnitStrc* pUnit); // 0x6FACEAC0 (UNIT_CUnit.cpp)
extern void*       __stdcall  D2Client_AllocLightMap_6FB0EAF0(D2UnitStrc* pUnit, int32_t nLightType, int32_t nRadius, int32_t nIntensity, int32_t nRed, int32_t nGreen, int32_t nBlue); // 0x6FB0EAF0 (DRAW_dLightMap.cpp)
extern void        __fastcall D2Client_SetLightMapRadius_6FB0EAB0(void* pLightMap, int32_t nRadius); // 0x6FB0EAB0 (DRAW_dLightMap.cpp)
extern void        __fastcall D2Client_DetachUnitFromSoundChannel(uint32_t dwHandle, D2UnitStrc* pUnit); // 0x6FAE2150 (UNIT_Monster.cpp)
extern int32_t     __fastcall PLAYERLIST_TestRelationFlag(uint32_t dwSourceId, uint32_t dwTargetId, uint32_t dwFlag); // 0x6FAE0DD0 (GAME_Roster.cpp)
extern void        __fastcall PLAYERLIST_EnsureRelationEntry(uint32_t dwSourceId, uint32_t dwTargetId); // 0x6FAE0E60 (GAME_Roster.cpp)
extern int32_t     __fastcall CLTSKILL_ExecuteSkillStartFunc(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nOwnerGUID, int32_t nSkillLevel); // 0x6FAFEE40 (SKILLS_Skills.cpp)
extern int32_t     __stdcall  CLTSKILL_ExecuteSkillDoFunc(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t bPlayDoSound); // 0x6FAFFDE0 (SKILLS_Skills.cpp)

// Mirrors 0x6FAB1670: tests a bit index in D2MonStats2Txt::nFlags (+0x04) for a monster class ID
static inline BOOL CLTSKILL_CheckMonStats2FlagIndex(int32_t nMonsterId, int32_t nFlagBitIndex)
{
    D2MonStats2Txt* pMonStats2Txt = CLTSKILL_GetMonStats2TxtRecordFromMonsterId(nMonsterId);
    if (!pMonStats2Txt || nFlagBitIndex < 0 || nFlagBitIndex >= 32)
    {
        return FALSE;
    }
    return (pMonStats2Txt->dwFlags & gdwBitMasks[nFlagBitIndex]) != 0;
}

// 1.12a: 0x6FB77C10 (gpfnCltStFunc[23])
int32_t __fastcall CLTSKILL_St_IronGolem(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (!pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    if (pTarget->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (!ITEMS_IsMetalItem(pTarget->dwClassId))
    {
        return 0;
    }

    if (pTarget->dwAnimMode != IMODE_ONGROUND)
    {
        return 0;
    }

    if (!ITEMS_CheckItemFlag(pTarget, IFLAG_IDENTIFIED, 556, s_szSkillsNecFile))
    {
        return 0;
    }

    if (!UNITS_GetRoom(pTarget))
    {
        return 0;
    }

    return (STATLIST_GetOwner(pTarget, nullptr) == nullptr) ? 1 : 0;
}

// 1.12a: 0x6FB77CA0 (gpfnCltStFunc[22])
int32_t __fastcall CLTSKILL_St_Golem(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    D2ActiveRoomStrc* pTargetRoom = DUNGEON_FindRoomBySubtile(pRoom, nTargetX, nTargetY);
    if (!pTargetRoom)
    {
        return 0;
    }

    if ((pSkillsTxt->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_INTOWN]) == 0
        && DUNGEON_IsRoomInTown(pTargetRoom))
    {
        return 0;
    }

    return 1;
}

// 1.12a: 0x6FB77D50 (gpfnCltStFunc[21])
int32_t __fastcall CLTSKILL_St_RaiseSkeleton_Mage(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || !pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    if (pUnit == gpPlayerUnit && !D2COMMON_11021(pTarget))
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, CLTSKILL_GetUnitXInline(pTarget));
    SKILLS_SetParam2(pSkill, CLTSKILL_GetUnitYInline(pTarget));
    SKILLS_SetParam3(pSkill, pTarget->dwClassId);
    return 1;
}

// 1.12a: 0x6FB77E30 (gpfnCltStFunc[20])
int32_t __fastcall CLTSKILL_St_Poison_CorpseExplosion(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || !pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget || !D2COMMON_11017_CheckUnitIfConsumeable(pTarget, 0))
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, D2Client_GetUnitX_6FAB1590(pTarget));
    SKILLS_SetParam2(pSkill, D2Client_GetUnitY_6FAB15C0(pTarget));
    return 1;
}

// 1.12a: 0x6FB77EC0 (gpfnCltStFunc[18])
int32_t __fastcall CLTSKILL_St_Curse(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileC = static_cast<int16_t>(pSkillsTxt->wCltMissileC);
    if (CLTSKILL_IsValidMissileId(nMissileC))
    {
        CLTSKILL_CreateMissileTowardUnitTarget(pUnit, nMissileC, nSkillId, nSkillLevel);
    }

    return 1;
}

// 1.12a: 0x6FB77F20
D2UnitStrc* __fastcall CLTSKILL_GetTargetRevivableCorpse(D2UnitStrc* pUnit)
{
    if (!pUnit || !pUnit->pDynamicPath)
    {
        return nullptr;
    }

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget || pTarget->dwUnitType != UNIT_MONSTER)
    {
        return nullptr;
    }

    if (!CLTSKILL_CheckMonStats2FlagIndex(pTarget->dwClassId, MONSTATS2FLAGINDEX_REVIVE))
    {
        return nullptr;
    }

    if (!D2COMMON_11017_CheckUnitIfConsumeable(pTarget, 0))
    {
        return nullptr;
    }

    return D2Client_IsNormalSwitchableAIMonster(pTarget) ? pTarget : nullptr;
}

// 1.12a: 0x6FB77F90 (gpfnCltStFunc[24])
int32_t __fastcall CLTSKILL_St_Revive(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2UnitStrc* pCorpse = CLTSKILL_GetTargetRevivableCorpse(pUnit);
    if (!pCorpse)
    {
        return 0;
    }

    D2MonStats2Txt* pMonStats2Txt = CLTSKILL_GetMonStats2TxtRecordFromMonsterId(pCorpse->dwClassId);
    if (!pMonStats2Txt)
    {
        return 0;
    }

    int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (pMonStats2Txt->nSizeX == 1)
    {
        nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    }
    else if (pMonStats2Txt->nSizeX == 3)
    {
        nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileC);
    }

    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x2000;
    tInit.pOwner = pUnit;
    tInit.pOrigin = pCorpse;
    tInit.nMissileId = nMissileId;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    return D2Client_CreateClientMissile(&tInit) ? 1 : 0;
}

// 1.12a: 0x6FB78080
int32_t __fastcall CLTSKILL_SpawnRaiseSkeletonEffects(
    int32_t nMonsterClassId,
    int32_t nSkillId,
    int32_t nY,
    D2UnitStrc* pUnit,
    int32_t nSkillLevel,
    int32_t nX)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileA = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileA))
    {
        return 0;
    }

    if (!CLTSKILL_GetMonStatsTxtRecord(nMonsterClassId) || nX == 0 || nY == 0)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x2001;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileA;
    tInit.nX = nX;
    tInit.nY = nY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    D2UnitStrc* pMissileA = D2Client_CreateClientMissile(&tInit);

    const int32_t nMissileC = static_cast<int16_t>(pSkillsTxt->wCltMissileC);
    if (nMissileC > 0 && CLTSKILL_IsValidMissileId(nMissileC))
    {
        tInit.nMissileId = nMissileC;
        tInit.nSkillLevel = 2;
        D2Client_CreateClientMissile(&tInit);
    }

    if (pMissileA)
    {
        const int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
        if (nMissileB > 0 && CLTSKILL_IsValidMissileId(nMissileB))
        {
            D2Client_SpawnRadialSubMissileBurst(pMissileA, nMissileB, 0, 0, 2);
        }

        if (!D2Client_ClearActionFrameAndGetTarget(pUnit))
        {
            const int16_t nTgtSound = static_cast<int16_t>(pSkillsTxt->wTgtSound);
            if (nTgtSound > 0)
            {
                D2Client_PlayUnitSound_6FAE1FA0(nTgtSound, pMissileA, 0, 0, 0);
            }
        }
    }

    return 1;
}

// 1.12a: 0x6FB78210
int32_t __fastcall CLTSKILL_SpawnCorpseExplosionGoreAndMissiles(
    D2UnitStrc* pUnit,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nX,
    int32_t nY,
    int32_t nMonsterClassId)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileA = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileA) || nX == 0 || nY == 0)
    {
        return 0;
    }

    D2MonStats2Txt* pMonStats2Txt = CLTSKILL_GetMonStats2TxtRecordFromMonsterId(nMonsterClassId);
    if (!pMonStats2Txt)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x2021;
    tInit.pOwner = pUnit;
    tInit.pOrigin = pUnit;
    tInit.nMissileId = nMissileA;
    tInit.nX = nX;
    tInit.nY = nY;
    tInit.nTargetX = nX + SEED_RollLimitedRandomNumber(&pUnit->pSeed, 3) - 1;
    tInit.nTargetY = nY + SEED_RollLimitedRandomNumber(&pUnit->pSeed, 3) - 1;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    if ((pMonStats2Txt->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_SMALL]) != 0)
    {
        CLTSKILL_SpawnScatteredMissileCluster(&tInit, 1, 0);
    }
    else if ((pMonStats2Txt->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_LARGE]) != 0)
    {
        CLTSKILL_SpawnScatteredMissileCluster(&tInit, 3, 3);
        D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxtRecord(nMissileA);
        if (pMissilesTxt)
        {
            const int32_t nSubMissile0 = static_cast<int16_t>(pMissilesTxt->wSubMissile[0]);
            if (nSubMissile0 > 0 && CLTSKILL_IsValidMissileId(nSubMissile0))
            {
                tInit.nMissileId = nSubMissile0;
                CLTSKILL_SpawnScatteredMissileCluster(&tInit, 4, 2);
            }
        }
    }
    else
    {
        CLTSKILL_SpawnScatteredMissileCluster(&tInit, 2, 2);
        D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxtRecord(nMissileA);
        if (pMissilesTxt)
        {
            const int32_t nSubMissile1 = static_cast<int16_t>(pMissilesTxt->wSubMissile[1]);
            if (nSubMissile1 > 0 && CLTSKILL_IsValidMissileId(nSubMissile1))
            {
                tInit.nMissileId = nSubMissile1;
                CLTSKILL_SpawnScatteredMissileCluster(&tInit, 3, 1);
            }
        }
    }

    tInit.nX = nX;
    tInit.nY = nY;

    D2UnitStrc* pMissileB = nullptr;
    const int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (nMissileB > 0 && CLTSKILL_IsValidMissileId(nMissileB))
    {
        tInit.nMissileId = nMissileB;
        pMissileB = D2Client_CreateClientMissile(&tInit);
    }

    const int32_t nMissileC = static_cast<int16_t>(pSkillsTxt->wCltMissileC);
    if (nMissileC > 0 && CLTSKILL_IsValidMissileId(nMissileC))
    {
        tInit.nMissileId = nMissileC;
        tInit.nSkillLevel = 2;
        D2Client_CreateClientMissile(&tInit);
    }

    if (!D2Client_ClearActionFrameAndGetTarget(pUnit) && pMissileB)
    {
        const int16_t nTgtSound = static_cast<int16_t>(pSkillsTxt->wTgtSound);
        if (nTgtSound > 0)
        {
            D2Client_PlayUnitSound_6FAE1FA0(nTgtSound, pMissileB, 0, 0, 0);
        }
    }

    return 1;
}

// 1.12a: 0x6FB78480 (gpfnCltDoFunc[31])
int32_t __fastcall CLTSKILL_Do_PoisonExplosion(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_GetSkillsTxtRecord(nSkillId))
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x2001;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nX = SKILLS_GetParam1(pSkill);
    tInit.nY = SKILLS_GetParam2(pSkill);
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    D2Client_CreateClientMissile(&tInit);

    SKILLS_SetParam1(pSkill, 0);
    SKILLS_SetParam2(pSkill, 0);
    return 1;
}

// 1.12a: 0x6FB78560
int32_t __fastcall CLTSKILL_SpawnBoneWallPrisonControlMissile(
    D2UnitStrc* pUnit,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nMissileId)
{
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    const int32_t nDx = nTargetX - CLTSKILL_GetUnitXInline(pUnit);
    const int32_t nDy = nTargetY - CLTSKILL_GetUnitYInline(pUnit);

    int32_t nDist = UNITS_GetDistanceToCoordinates(pUnit, nTargetX, nTargetY);
    if (nDist <= 1)
    {
        nDist = 1;
    }

    const int32_t nSpawnX = CLTSKILL_GetUnitXInline(pUnit) + ((2 * nDx) / nDist);
    const int32_t nSpawnY = CLTSKILL_GetUnitYInline(pUnit) + ((2 * nDy) / nDist);

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x03;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nX = nSpawnX;
    tInit.nY = nSpawnY;
    tInit.nTargetX = 5 * nDx;
    tInit.nTargetY = 5 * nDy;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
    if (!pMissile)
    {
        return 0;
    }

    const int32_t nDir = UNITS_GetDirectionToCoords(pUnit, nTargetX, nTargetY);
    D2COMMON_10160_PathUpdateDirection(pMissile->pDynamicPath, nDir);
    return 1;
}

// 1.12a: 0x6FB78720 (gpfnCltDoFunc[30])
int32_t __fastcall CLTSKILL_Do_Curse(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileA = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_IsValidMissileId(nMissileA))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    int32_t nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel) - 3;
    if (nRange < 2)
    {
        nRange = 2;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x01;
    tInit.pOwner = pUnit;
    tInit.nMissileId = MISSILE_CURSECENTER; // 0xBF (191)
    tInit.nX = nTargetX;
    tInit.nY = nTargetY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    D2UnitStrc* pCenterMissile = D2Client_CreateClientMissile(&tInit);
    if (!pCenterMissile)
    {
        return 0;
    }

    D2Client_FreeUnitLight(pCenterMissile);
    void* pLight = D2Client_AllocLightMap_6FB0EAF0(pCenterMissile, 1, 1, 0xFF, 0xFF, 0, 0);
    if (pLight)
    {
        D2Client_SetLightMapRadius_6FB0EAB0(pLight, nRange);
        pCenterMissile->pLight = static_cast<D2GfxLightStrc*>(pLight);
    }

    MISSILE_SetStreamMissile(pCenterMissile, nMissileA);
    MISSILE_SetStreamRange(pCenterMissile, nRange);
    return 1;
}

// 1.12a: 0x6FB78870 (gpfnCltDoFunc[33])
int32_t __fastcall CLTSKILL_Do_RaiseSkeleton_Mage(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nX = SKILLS_GetParam1(pSkill);
    const int32_t nY = SKILLS_GetParam2(pSkill);
    const int32_t nMonsterClassId = SKILLS_GetParam3(pSkill);
    return CLTSKILL_SpawnRaiseSkeletonEffects(nMonsterClassId, nSkillId, nY, pUnit, nSkillLevel, nX);
}

// 1.12a: 0x6FB788C0 (gpfnCltDoFunc[32])
int32_t __fastcall CLTSKILL_Do_CorpseExplosion(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nX = SKILLS_GetParam1(pSkill);
    const int32_t nY = SKILLS_GetParam2(pSkill);
    const int32_t nMonsterClassId = SKILLS_GetParam3(pSkill);
    return CLTSKILL_SpawnCorpseExplosionGoreAndMissiles(pUnit, nSkillId, nSkillLevel, nX, nY, nMonsterClassId);
}

// 1.12a: 0x6FB78910 (gpfnCltStFunc[19])
int32_t __fastcall CLTSKILL_St_BoneWall_BonePrison(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileC = static_cast<int16_t>(pSkillsTxt->wCltMissileC);
    if (!CLTSKILL_IsValidMissileId(nMissileC))
    {
        return 0;
    }

    return CLTSKILL_SpawnBoneWallPrisonControlMissile(pUnit, nSkillId, nSkillLevel, nMissileC);
}

// 1.12a: 0x6FB789F0 (gpfnCltDoFunc[85])
int32_t __fastcall CLTSKILL_Do_SpawnPeriodicGore(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    static uint32_t s_dwNextGoreTick_6FBD3690 = 0; // 1.12a: 0x6FBD3690
    if (gdwClientGameTick > s_dwNextGoreTick_6FBD3690)
    {
        s_dwNextGoreTick_6FBD3690 = gdwClientGameTick + 5;
        D2Client_SpawnCorpseGoreMissiles(pUnit, 0, 1);
    }

    return 1;
}

// 1.12a: 0x6FB78A20 (Triggered / Item-Proc Skill Execution — Core of SCmd 0x99 & 0x9A)
int32_t __fastcall CLTSKILL_TriggerProcSkill(
    D2UnitStrc* pUnit,
    D2UnitStrc* pTarget,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nTargetX,
    int32_t nTargetY,
    int32_t bPlayDoSound)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt || !pUnit || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* const pSavedTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    const int32_t nSavedTargetX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
    const int32_t nSavedTargetY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
    const bool bSavedSrvDoFlag = (pUnit->dwFlags & UNITFLAG_SKSRVDOFUNC) != 0;

    if (pTarget)
    {
        UNITS_SetTargetUnitForDynamicUnit(pUnit, pTarget);
    }
    else
    {
        D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nTargetX, nTargetY);
    }

    int32_t nResult = 0;
    bool bCanExecuteDoFunc = true;

    // dwFlags[1] bit 2 (byte +0x08 & 0x04): requires client StFunc validation before DoFunc
    if ((pSkillsTxt->dwFlags[1] & gdwBitMasks[2]) != 0)
    {
        bool bTemporarilyClearedJustHit = false;
        if (pTarget && D2Client_IsUnitDeadOrDying(pTarget) && STATES_CheckState(pTarget, STATE_JUSTHIT))
        {
            bTemporarilyClearedJustHit = true;
            STATES_ToggleState(pTarget, STATE_JUSTHIT, FALSE);
        }

        if (!CLTSKILL_ExecuteSkillStartFunc(pUnit, nSkillId, -1, nSkillLevel))
        {
            bCanExecuteDoFunc = false;
        }
        else if (bTemporarilyClearedJustHit)
        {
            STATES_ToggleState(pTarget, STATE_JUSTHIT, TRUE);
        }
    }

    if (bCanExecuteDoFunc)
    {
        const int16_t nItemCastSound = static_cast<int16_t>(pSkillsTxt->wItemCastSound);
        if (nItemCastSound >= 0)
        {
            D2Client_PlayUnitSound_6FAE1FA0(nItemCastSound, pUnit, 0, 0, 0);
        }

        const int32_t nItemCastOverlay = static_cast<int16_t>(pSkillsTxt->wItemCastOverlay);
        if (nItemCastOverlay >= 0 && sgptDataTables && nItemCastOverlay < sgptDataTables->nOverlayTxtRecordCount)
        {
            GFX_AttachOverlayToUnit(pUnit, nItemCastOverlay, 2);
        }

        nResult = CLTSKILL_ExecuteSkillDoFunc(pUnit, nSkillId, nSkillLevel, static_cast<uint16_t>(bPlayDoSound));
    }

    if (pSavedTarget)
    {
        UNITS_SetTargetUnitForDynamicUnit(pUnit, pSavedTarget);
    }
    else
    {
        D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nSavedTargetX, nSavedTargetY);
    }

    if (bSavedSrvDoFlag)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }
    else
    {
        pUnit->dwFlags &= ~UNITFLAG_SKSRVDOFUNC;
    }

    return nResult;
}

// 1.12a: 0x6FB78C10 (SCmd 0x9A wrapper: Triggered Skill at Coordinates)
int32_t __fastcall CLTSKILL_TriggerProcSkillAtCoord(
    int32_t nTargetX,
    int32_t bPlayDoSound,
    D2UnitStrc* pUnit,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nTargetY)
{
    if (!pUnit || nTargetX == 0 || nTargetY == 0)
    {
        return 0;
    }

    return CLTSKILL_TriggerProcSkill(pUnit, nullptr, nSkillId, nSkillLevel, nTargetX, nTargetY, bPlayDoSound);
}

// 1.12a: 0x6FB78C40 (SCmd 0x99 wrapper: Triggered Skill on Target Unit)
int32_t __fastcall CLTSKILL_TriggerProcSkillOnUnit(
    int32_t bPlayDoSound,
    int32_t nSkillLevel,
    D2UnitStrc* pUnit,
    int32_t nSkillId,
    D2UnitStrc* pTarget)
{
    if (!pUnit || !pTarget)
    {
        return 0;
    }

    return CLTSKILL_TriggerProcSkill(pUnit, pTarget, nSkillId, nSkillLevel, 0, 0, bPlayDoSound);
}

// 1.12a: 0x6FB78C70
BOOL __fastcall CLTSKILL_AreUnitsAligned(D2UnitStrc* pUnitA, D2UnitStrc* pUnitB)
{
    const uint32_t dwTypeA = pUnitA ? pUnitA->dwUnitType : 6;
    if (dwTypeA == UNIT_PLAYER)
    {
        const uint32_t dwTargetId = pUnitB ? pUnitB->dwUnitId : 0xFFFFFFFFu;
        const uint32_t dwSourceId = pUnitA ? pUnitA->dwUnitId : 0xFFFFFFFFu;
        return PLAYERLIST_TestRelationFlag(dwSourceId, dwTargetId, 0x08u) == 0;
    }

    if (dwTypeA == UNIT_MONSTER)
    {
        const uint32_t dwTypeB = pUnitB ? pUnitB->dwUnitType : 6;
        if (dwTypeB == UNIT_PLAYER)
        {
            const uint32_t dwSourceId = pUnitA ? pUnitA->dwUnitId : 0xFFFFFFFFu;
            const uint32_t dwTargetId = pUnitB ? pUnitB->dwUnitId : 0xFFFFFFFFu;
            return PLAYERLIST_TestRelationFlag(dwSourceId, dwTargetId, 0x08u) == 0;
        }
        if (dwTypeB == UNIT_MONSTER)
        {
            return STATLIST_AreUnitsAligned(pUnitA, pUnitB);
        }
    }

    return FALSE;
}

// 1.12a: 0x6FB78D10
void __fastcall CLTSKILL_SetRosterHostileFlag(D2UnitStrc* pPlayerA, D2UnitStrc* pPlayerB)
{
    if (!pPlayerA || pPlayerA->dwUnitType != UNIT_PLAYER)
    {
        return;
    }

    const uint32_t dwSourceId = pPlayerA->dwUnitId;
    const uint32_t dwTargetId = pPlayerB ? pPlayerB->dwUnitId : 0xFFFFFFFFu;

    PLAYERLIST_EnsureRelationEntry(dwSourceId, dwTargetId);

    D2RosterUnitStrc* pRoster = ROSTER_FindPlayerByUnitId(dwSourceId);
    if (!pRoster || !pRoster->pPvpRelationList || dwSourceId == dwTargetId)
    {
        return;
    }

    for (D2PlayerRelationNodeStrc* pNode = pRoster->pPvpRelationList->pHead; pNode != nullptr; pNode = pNode->pNext)
    {
        if (pNode->dwTargetUnitId == dwTargetId)
        {
            pNode->dwRelationFlags |= 0x08u;
            return;
        }
    }
}

// 1.12a: 0x6FB78D90
int32_t __fastcall CLTSKILL_ClearRosterHostileFlag(D2UnitStrc* pPlayerA, D2UnitStrc* pPlayerB)
{
    if (!pPlayerA || pPlayerA->dwUnitType != UNIT_PLAYER)
    {
        return 0;
    }

    const uint32_t dwSourceId = pPlayerA->dwUnitId;
    const uint32_t dwTargetId = pPlayerB ? pPlayerB->dwUnitId : 0xFFFFFFFFu;

    PLAYERLIST_EnsureRelationEntry(dwSourceId, dwTargetId);

    D2RosterUnitStrc* pRoster = ROSTER_FindPlayerByUnitId(dwSourceId);
    if (!pRoster || !pRoster->pPvpRelationList || dwSourceId == dwTargetId)
    {
        return 1;
    }

    for (D2PlayerRelationNodeStrc* pNode = pRoster->pPvpRelationList->pHead; pNode != nullptr; pNode = pNode->pNext)
    {
        if (pNode->dwTargetUnitId == dwTargetId)
        {
            pNode->dwRelationFlags &= ~0x08u;
            return 1;
        }
    }

    return 1;
}

// 1.12a: 0x6FB78E10 (gpfnCltStFunc[45])
int32_t __fastcall CLTSKILL_St_Wearwolf_Wearbear(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt || !sgptDataTables)
    {
        return 0;
    }

    const int32_t nAuraState = pSkillsTxt->nAuraState;
    if (nAuraState < 0 || nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    const int32_t nOverlayCount = sgptDataTables->nOverlayTxtRecordCount;
    int32_t nOverlayId = static_cast<int16_t>(pSkillsTxt->wCltOverlayA);
    if (nOverlayId < 0 || nOverlayId >= nOverlayCount)
    {
        return 0;
    }

    const int32_t nRevertOverlayId = static_cast<int16_t>(pSkillsTxt->wCltOverlayB);
    if (nRevertOverlayId >= 0 && nRevertOverlayId < nOverlayCount && STATES_CheckState(pUnit, nAuraState))
    {
        nOverlayId = nRevertOverlayId;
    }

    GFX_AttachOverlayToUnit(pUnit, nOverlayId, 2);
    return 1;
}

// 1.12a: 0x6FB78ED0 (gpfnCltDoFunc[91])
int32_t __fastcall CLTSKILL_Do_ShockField_Sentry(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x01;
    tInit.pOwner = pUnit;
    tInit.nX = nTargetX;
    tInit.nY = nTargetY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    const int32_t nMissileA = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (CLTSKILL_IsValidMissileId(nMissileA))
    {
        tInit.nMissileId = nMissileA;
        D2Client_CreateClientMissile(&tInit);
    }

    const int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (CLTSKILL_IsValidMissileId(nMissileB))
    {
        tInit.nMissileId = nMissileB;
        D2UnitStrc* pMissileB = D2Client_CreateClientMissile(&tInit);
        if (pMissileB && pUnit)
        {
            MISSILE_SetTargetX(pMissileB, static_cast<int32_t>(pUnit->dwInitSeed));
        }
    }

    return 1;
}

// 1.12a: 0x6FB78FE0 (gpfnCltDoFunc[92])
int32_t __fastcall CLTSKILL_Do_PhoenixStrike_FallingMeteorPair(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    if (SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel) <= 0)
    {
        return 0;
    }

    const int32_t nFallFrames = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);
    if (nFallFrames <= 0)
    {
        return 0;
    }

    const int32_t nVelZ = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[1], nSkillId, nSkillLevel);
    if (nVelZ <= 0)
    {
        return 0;
    }

    const int32_t nVelX = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[2], nSkillId, nSkillLevel);
    if (nVelX <= 0)
    {
        return 0;
    }

    const int32_t nStartPosZ = nVelZ * nFallFrames;
    const int32_t nStartPosX = nVelX * nFallFrames;

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x01;
    tInit.pOwner = pUnit;
    tInit.nX = nTargetX;
    tInit.nY = nTargetY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;

    const int32_t nMissileA = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (nMissileA >= 0)
    {
        tInit.nMissileId = nMissileA;
        D2UnitStrc* pMissileA = D2Client_CreateClientMissile(&tInit);
        if (pMissileA)
        {
            PARTICLE_SetPosition(pMissileA, nStartPosX, 0, nStartPosZ);
            D2ParticleStrc* pParticleA = CLTSKILL_GetUnitParticle(pMissileA);
            if (pParticleA)
            {
                pParticleA->nVelX = (-nVelX) << 11;
                pParticleA->nVelY = 0;
                pParticleA->nVelZ = (-nVelZ) << 11;
            }
        }
    }

    const int32_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (nMissileB >= 0)
    {
        tInit.nMissileId = nMissileB;
        D2UnitStrc* pMissileB = D2Client_CreateClientMissile(&tInit);
        if (pMissileB)
        {
            PARTICLE_SetPosition(pMissileB, nStartPosX, 0, nStartPosZ);
            D2ParticleStrc* pParticleB = CLTSKILL_GetUnitParticle(pMissileB);
            if (pParticleB)
            {
                pParticleB->nVelX = (-nVelX) << 11;
                pParticleB->nVelY = 0;
                pParticleB->nVelZ = (-nVelZ) << 11;
            }
        }
    }

    return 1;
}

// 1.12a: 0x6FB791C0
void __fastcall CLTSKILL_SpawnHurricaneArmageddonDebris(
    D2UnitStrc* pUnit,
    D2SkillsTxt* pSkillsTxt,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    if (!pUnit || !pSkillsTxt)
    {
        return;
    }

    const uint32_t dwRingIdx = static_cast<uint32_t>(SEED_RollRandomNumber(&pUnit->pSeed)) & 7u;
    const int32_t nUnitX = CLTSKILL_GetUnitXInline(pUnit);
    const int32_t nUnitY = CLTSKILL_GetUnitYInline(pUnit);

    const int32_t nRingOffsetX = g_aHurricaneRingOffsetX_6FB850EC[dwRingIdx];
    const int32_t nRingOffsetY = g_aHurricaneRingOffsetY_6FB850CC[dwRingIdx];
    const int32_t nSpawnX = nUnitX + nRingOffsetX;
    const int32_t nSpawnY = nUnitY + nRingOffsetY;

    int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (nMissileId < 0)
    {
        return;
    }

    const int16_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (nMissileB >= 0)
    {
        const int16_t nMissileC = static_cast<int16_t>(pSkillsTxt->wCltMissileC);
        if (nMissileC >= 0)
        {
            const uint32_t dwPick = static_cast<uint32_t>(SEED_RollRandomNumber(&pUnit->pSeed)) % 3u;
            if (dwPick == 0)
            {
                nMissileId = nMissileB;
            }
            else if (dwPick == 1)
            {
                nMissileId = nMissileC;
            }
        }
        else
        {
            if ((static_cast<uint32_t>(SEED_RollRandomNumber(&pUnit->pSeed)) & 1u) == 0)
            {
                nMissileId = nMissileB;
            }
        }
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = 0x03;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nX = nSpawnX;
    tInit.nY = nSpawnY;
    tInit.nTargetX = nRingOffsetY;
    tInit.nTargetY = -nRingOffsetX;
    tInit.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
    if (!pMissile)
    {
        return;
    }

    const int32_t nMaxStartZ = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[1], nSkillId, nSkillLevel);
    const int32_t nRandomZ = SEED_RollLimitedRandomNumber(&pUnit->pSeed, nMaxStartZ);

    D2ParticleStrc* pParticle = CLTSKILL_GetUnitParticle(pMissile);
    if (pParticle)
    {
        pParticle->nPosX = 0;
        pParticle->nPosY = 0;
        pParticle->nPosZ = nRandomZ << 11;
    }

    MISSILE_SetTargetX(pMissile, CLTSKILL_GetUnitXInline(pUnit));
    MISSILE_SetTargetY(pMissile, CLTSKILL_GetUnitYInline(pUnit));
}

// 1.12a: 0x6FB79440
int32_t __fastcall CLTSKILL_SpawnTwisterTornadoSpread(
    D2UnitStrc* pUnit,
    int32_t nCount,
    int32_t nMissileId,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nAnimSpeed,
    int32_t bIncludeCenter,
    int32_t bDisableRotateAndSound)
{
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_GetUnitTargetCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    if (bIncludeCenter != 0)
    {
        --nCount;
        D2UnitStrc* pCenterMissile = CLTSKILL_CreateMissileTowardUnitTarget(pUnit, nMissileId, nSkillId, nSkillLevel);
        if (pCenterMissile)
        {
            if (bDisableRotateAndSound != 0)
            {
                D2COMMON_10203_PATH_SetRotateFlag(pCenterMissile->pDynamicPath, FALSE);
            }
            UNITS_SetAnimationSpeed(pCenterMissile, nAnimSpeed);
        }
    }

    D2ClientMissileInitStrc tInit = {};
    tInit.dwFlags = (bDisableRotateAndSound != 0) ? 0x2021u : 0x21u;
    tInit.pOwner = pUnit;
    tInit.nMissileId = nMissileId;
    tInit.nX = CLTSKILL_GetUnitXInline(pUnit);
    tInit.nY = CLTSKILL_GetUnitYInline(pUnit);
    tInit.nTargetX = nTargetX;
    tInit.nTargetY = nTargetY;
    tInit.nSkillId = nSkillId;
    tInit.nSkillLevel = nSkillLevel;
    tInit.pfPostInit = CLTSKILL_InitBezierVineMissile;

    for (int32_t i = 0; i < nCount; ++i)
    {
        tInit.nPostInitArg = i;
        D2UnitStrc* pMissile = D2Client_CreateClientMissile(&tInit);
        if (pMissile)
        {
            if (bDisableRotateAndSound != 0)
            {
                D2COMMON_10203_PATH_SetRotateFlag(pMissile->pDynamicPath, FALSE);
            }
            UNITS_SetAnimationSpeed(pMissile, nAnimSpeed);
        }
    }

    return 1;
}

// 1.12a: 0x6FB795D0 (gpfnCltDoFunc[90])
int32_t __fastcall CLTSKILL_Do_Armageddon_Hurricane(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    const bool bInTown = (!pRoom || DUNGEON_IsRoomInTown(pRoom));

    const int16_t nPrgSound = static_cast<int16_t>(pSkillsTxt->wPrgSound);
    if (nPrgSound > 0)
    {
        const uint32_t dwActiveHandle = D2Client_FindUnitActiveSoundHandle(pUnit, nPrgSound);
        if (bInTown)
        {
            if (dwActiveHandle != 0)
            {
                D2Client_DetachUnitFromSoundChannel(dwActiveHandle, pUnit);
            }
            return 0;
        }

        if (dwActiveHandle == 0)
        {
            D2Client_PlayUnitSound_6FAE1FA0(nPrgSound, pUnit, 0, 0, 0);
        }
    }
    else if (bInTown)
    {
        return 0;
    }

    const int32_t nBursts = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);
    for (int32_t i = 0; i < nBursts; ++i)
    {
        CLTSKILL_SpawnHurricaneArmageddonDebris(pUnit, pSkillsTxt, nSkillId, nSkillLevel);
        CLTSKILL_SpawnHurricaneArmageddonDebris(pUnit, pSkillsTxt, nSkillId, nSkillLevel);
        CLTSKILL_SpawnHurricaneArmageddonDebris(pUnit, pSkillsTxt, nSkillId, nSkillLevel);
    }

    return 1;
}

// 1.12a: 0x6FB796C0 (gpfnCltDoFunc[69])
int32_t __fastcall CLTSKILL_Do_Twister_Tornado(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    if (nCount <= 0)
    {
        return 0;
    }

    return CLTSKILL_SpawnTwisterTornadoSpread(pUnit, nCount, nMissileId, nSkillId, nSkillLevel, 0x200, 0, 1);
}

// 1.12a: 0x6FB79770 (gpfnCltDoFunc[68])
int32_t __fastcall CLTSKILL_Do_BladeFury_RoyalStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    if (nCount <= 0)
    {
        return 0;
    }

    return CLTSKILL_SpawnTwisterTornadoSpread(pUnit, nCount, nMissileId, nSkillId, nSkillLevel, 0x100, 0, 1);
}

// 1.12a: 0x6FB79820 (gpfnCltDoFunc[67])
int32_t __fastcall CLTSKILL_Do_ChargedBoltSentry_ShockWeb(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveSkillClientMissile(pUnit, nSkillId);
    if (!CLTSKILL_IsValidMissileId(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    if (nCount <= 0)
    {
        return 0;
    }

    return CLTSKILL_SpawnTwisterTornadoSpread(pUnit, nCount, nMissileId, nSkillId, nSkillLevel, 0x200, 1, 0);
}

// ============================================================================
// Reference Dispatch Table Mapping for Slices 0x6FB373F0..0x6FB38080,
// 0x6FB4C270..0x6FB4E900, and 0x6FB73920..0x6FB79900
// ============================================================================
#if 0
// gpfnCltStFunc[18..52] (1.12a: 0x6FBA3510 + 4 * index)
/*  18 */ CLTSKILL_St_Curse,                       // 0x6FB77EC0
/*  19 */ CLTSKILL_St_BoneWall_BonePrison,         // 0x6FB78910
/*  20 */ CLTSKILL_St_Poison_CorpseExplosion,      // 0x6FB77E30
/*  21 */ CLTSKILL_St_RaiseSkeleton_Mage,          // 0x6FB77D50
/*  22 */ CLTSKILL_St_Golem,                       // 0x6FB77CA0
/*  23 */ CLTSKILL_St_IronGolem,                   // 0x6FB77C10
/*  24 */ CLTSKILL_St_Revive,                      // 0x6FB77F90
/*  25 */ CLTSKILL_St_Charge,                      // 0x6FB37E70
/*  26 */ CLTSKILL_St_FindPotion_GrimWard,         // 0x6FB73E20
/*  27 */ CLTSKILL_St_Bash_Stun_Concentrate,       // 0x6FB73C90
/*  28 */ CLTSKILL_St_FindItem,                    // 0x6FB73DD0
/*  29 */ CLTSKILL_St_Leap,                        // 0x6FB75060
/*  30 */ CLTSKILL_St_LeapAttack,                  // 0x6FB74EF0
/*  31 */ CLTSKILL_St_Whirlwind,                   // 0x6FB74D10
/*  32 */ CLTSKILL_St_MaggotUp,                    // 0x6FB4CE10
/*  33 */ CLTSKILL_St_MaggotDown,                  // 0x6FB4C800
/*  34 */ CLTSKILL_St_AndarielSpray,               // 0x6FB4CD30
/*  35 */ CLTSKILL_St_SwarmMove_QuickStrike,       // 0x6FB4CAD0
/*  36 */ CLTSKILL_St_Nest_EvilHutSpawner,         // 0x6FB4C6B0
/*  37 */ CLTSKILL_St_Submerge_MaggotEgg,          // 0x6FB4C610
/*  38 */ CLTSKILL_St_Emerge,                      // 0x6FB4C5F0
/*  39 */ CLTSKILL_St_MonInferno,                  // 0x6FB4C9A0
/*  40 */ CLTSKILL_St_DiabRun,                     // 0x6FB4C580
/*  41 */ CLTSKILL_St_ClearUsedSkillFlags,         // 0x6FB4C270
/*  42 */ CLTSKILL_St_MultiHitRollCount,           // 0x6FB4D2D0
/*  43 */ CLTSKILL_St_SpawnMissileTowardTarget,    // 0x6FB4D270
/*  44 */ CLTSKILL_St_HellMeteor,                  // 0x6FB4CFB0
/*  45 */ CLTSKILL_St_Wearwolf_Wearbear,           // 0x6FB78E10
/*  46 */ CLTSKILL_St_BaalTaunt,                   // 0x6FB76D80
/*  47 */ CLTSKILL_St_Raven_DruidSummon,           // 0x6FB76EB0
/*  48 */ CLTSKILL_St_SpawnCltMissileAAtCaster,    // 0x6FB76310
/*  49 */ CLTSKILL_St_Vines_MinionSpawner,         // 0x6FB77A30
/*  50 */ CLTSKILL_St_SaveTargetCoords,            // 0x6FB766B0
/*  51 */ CLTSKILL_St_Corpse_VineCycler,           // 0x6FB76620
/*  52 */ CLTSKILL_St_ArcticBlast_Inferno,         // 0x6FB76130

// gpfnCltDoFunc[30..95] (1.12a: 0x6FBA3630 + 4 * index; 83/84/87 live in 0x6FB0xxxx)
/*  30 */ CLTSKILL_Do_Curse,                       // 0x6FB78720
/*  31 */ CLTSKILL_Do_PoisonExplosion,             // 0x6FB78480
/*  32 */ CLTSKILL_Do_CorpseExplosion,             // 0x6FB788C0
/*  33 */ CLTSKILL_Do_RaiseSkeleton_Mage,          // 0x6FB78870
/*  34 */ CLTSKILL_Do_BlessedHammer,               // 0x6FB37650
/*  35 */ CLTSKILL_Do_HolyBolt_SmiteMissile,       // 0x6FB37D60
/*  36 */ CLTSKILL_Do_FistOfTheHeavens,            // 0x6FB37C50
/*  37 */ CLTSKILL_Do_Charge,                      // 0x6FB37800
/*  38 */ CLTSKILL_Do_FindPotion,                  // 0x6FB74040
/*  39 */ CLTSKILL_Do_WhirlwindAttack,             // 0x6FB74340
/*  40 */ CLTSKILL_Do_FindItem,                    // 0x6FB73FD0
/*  41 */ CLTSKILL_Do_GrimWard,                    // 0x6FB749A0
/*  42 */ CLTSKILL_Do_DoubleThrow,                 // 0x6FB74B70
/*  43 */ CLTSKILL_Do_Leap,                        // 0x6FB74840
/*  44 */ CLTSKILL_Do_LeapAttack,                  // 0x6FB74700
/*  45 */ CLTSKILL_Do_WhirlwindEnd,                // 0x6FB73ED0
/*  46 */ CLTSKILL_Do_ResetSeqSpeed,               // 0x6FB4C310
/*  47 */ CLTSKILL_Do_ClearActionFrameAndCheckMetricsB, // 0x6FB4C2D0
/*  48 */ CLTSKILL_Do_AndarielSpray,               // 0x6FB4CB90
/*  49 */ CLTSKILL_Do_SwarmMove_QuickStrike,       // 0x6FB4C770
/*  50 */ CLTSKILL_Do_Nest_EvilHutSpawner,         // 0x6FB4C630
/*  51 */ CLTSKILL_Do_ArcaneTower,                 // 0x6FB4E260
/*  52 */ CLTSKILL_Do_ClearActionFrameAndCheckMetricsA, // 0x6FB4C290
/*  53 */ CLTSKILL_Do_DiabPrison,                  // 0x6FB4E170
/*  54 */ CLTSKILL_Do_ImpInferno,                  // 0x6FB4E820
/*  55 */ CLTSKILL_Do_DiabWall,                    // 0x6FB4DE90
/*  56 */ CLTSKILL_Do_MonInferno,                  // 0x6FB4E710
/*  57 */ CLTSKILL_Do_DiabLight,                   // 0x6FB4DD10
/*  58 */ CLTSKILL_Do_FingerMageSpider,            // 0x6FB4DC00
/*  59 */ CLTSKILL_Do_DiabRun,                     // 0x6FB4C3E0
/*  60 */ CLTSKILL_Do_DiabCold,                    // 0x6FB4D9F0
/*  61 */ CLTSKILL_Do_PrimePoisonNova,             // 0x6FB4D850
/*  62 */ CLTSKILL_Do_MultiHitRollbackLoop,        // 0x6FB4C320
/*  63 */ CLTSKILL_Do_FetishAura_Cluster,          // 0x6FB4D740
/*  64 */ CLTSKILL_Do_DesertTurret_Jump,           // 0x6FB4D3F0
/*  65 */ CLTSKILL_Do_MonCurseCast,                // 0x6FB4D620
/*  66 */ CLTSKILL_Do_Hireable_RogueMissile,       // 0x6FB4E430
/*  67 */ CLTSKILL_Do_ChargedBoltSentry_ShockWeb,  // 0x6FB79820
/*  68 */ CLTSKILL_Do_BladeFury_RoyalStrike,       // 0x6FB79770
/*  69 */ CLTSKILL_Do_Twister_Tornado,             // 0x6FB796C0
/*  70 */ CLTSKILL_Do_Firestorm,                   // 0x6FB77910
/*  71 */ CLTSKILL_Do_Eruption_MoltenBoulder,      // 0x6FB77660
/*  72 */ CLTSKILL_Do_Meteor_Volcano_Catapult,     // 0x6FB77AE0
/*  73 */ CLTSKILL_Do_SpawnCltMissileBAtCaster,    // 0x6FB76290
/*  74 */ CLTSKILL_Do_SuckBlood,                   // 0x6FB76E10
/*  75 */ CLTSKILL_Do_SiegeBeastStomp,             // 0x6FB76790
/*  76 */ CLTSKILL_Do_OverseerWhip,                // 0x6FB76AA0
/*  77 */ CLTSKILL_Do_ArcticBlast_Channel,         // 0x6FB774F0
/*  78 */ CLTSKILL_Do_SpawnSavedCoordMissiles,     // 0x6FB76990
/*  79 */ CLTSKILL_Do_ImpTeleport,                 // 0x6FB76070
/*  80 */ CLTSKILL_Do_WakeOfDestruction_BaalCold,  // 0x6FB77240
/*  81 */ CLTSKILL_Do_DirectionalMissileWithPerpTarget, // 0x6FB768C0
/*  82 */ CLTSKILL_Do_BaalCorpseExplode,           // 0x6FB76380
/*  85 */ CLTSKILL_Do_SpawnPeriodicGore,           // 0x6FB789F0
/*  86 */ CLTSKILL_Do_HolyFreeze_SanctuaryAura,    // 0x6FB37A80
/*  88 */ CLTSKILL_Do_UpdateRiderPath,             // 0x6FB765F0
/*  89 */ CLTSKILL_Do_VineAttack,                  // 0x6FB77050
/*  90 */ CLTSKILL_Do_Armageddon_Hurricane,        // 0x6FB795D0
/*  91 */ CLTSKILL_Do_ShockField_Sentry,           // 0x6FB78ED0
/*  92 */ CLTSKILL_Do_PhoenixStrike_FallingMeteorPair, // 0x6FB78FE0
/*  93 */ CLTSKILL_Do_FenrisRage,                  // 0x6FB77430
/*  94 */ CLTSKILL_Do_DoomKnightMissile,           // 0x6FB4E630
/*  95 */ CLTSKILL_Do_NecromageMissile,            // 0x6FB4E550
#endif
