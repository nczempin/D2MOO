// ============================================================================
// SKILLS_Part1_CoreAmaSor.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll (0x6FAFB7E0 .. 0x6FB02F04)
//
// Slice contents (146 procedures in exact 1.12a VA order):
//   1. Amazon Client Skill Start & Do Handlers  (0x6FAFB7E0 .. 0x6FAFBA73,
//                                                0x6FAFBC20 .. 0x6FAFC82E)
//   2. Zeal / Fury Shared Start Handler         (0x6FAFBA80 .. 0x6FAFBC18)
//   3. Hardcore Death / Expansion Modal Dialogs (0x6FAFC830 .. 0x6FAFD720)
//   4. Core Client Skill Engine & Aura Search   (0x6FAFD730 .. 0x6FB00419)
//   5. Assassin & Shared Progressive/Sentry     (0x6FB00420 .. 0x6FB01BB9)
//   6. Sorceress Client Skill Start/Do/Events   (0x6FB01BC0 .. 0x6FB02DC5)
//   7. Belt UI Hit-Test & Slot Layout           (0x6FB02DD0 .. 0x6FB02F04)
// ============================================================================

#ifndef D2_VERSION_112A
#define D2_VERSION_112A 1
#endif
#ifdef D2_VERSION_110F
#undef D2_VERSION_110F
#endif

#include <cstddef>
#include <cstdint>
#include <cstring>

#ifndef FOG_DLL_DECL
#define FOG_DLL_DECL __declspec(dllimport)
#endif
#ifndef STORM_DLL_DECL
#define STORM_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2COMMON_DLL_DECL
#define D2COMMON_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2GFX_DLL_DECL
#define D2GFX_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2CMP_DLL_DECL
#define D2CMP_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2LANG_DLL_DECL
#define D2LANG_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2WIN_DLL_DECL
#define D2WIN_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2GAME_DLL_DECL
#define D2GAME_DLL_DECL __declspec(dllimport)
#endif

#include "D2CommonDefinitions.h"
#include "Storm.h"
#include "D2DataTbls.h"
#include "D2Skills.h"
#include "D2States.h"
#include "D2StatList.h"
#include "D2Items.h"
#include "D2Inventory.h"
#include "D2Composit.h"
#include "D2Dungeon.h"
#include "D2Collision.h"
#include "D2Monsters.h"
#include "D2Seed.h"
#include "D2Log.h"
#include "Units/Units.h"
#include "Units/Missile.h"
#include "Path/Path.h"
#include "Path/Step.h"
#include "D2Gfx.h"
#include "Texture.h"
#include "Window.h"
#include "D2CMP.h"
#include "D2Lang.h"
#include "D2WinFont.h"
#include "D2WinPalette.h"
#include "Fog.h"
#include "D2BitManip.h"

// ============================================================================
// Compile-Time Layout Verification (1.12a Binary Struct Sizes)
// ============================================================================
static_assert(sizeof(D2MissileStrc) == 0x5C, "D2MissileStrc must be 0x5C bytes");
static_assert(sizeof(D2SkillsTxt)   == 0x23C, "D2SkillsTxt must be 0x23C bytes");
static_assert(sizeof(D2StatesTxt)   == 0x3C, "D2StatesTxt must be 0x3C bytes");
static_assert(sizeof(D2MissilesTxt) == 0x1A4, "D2MissilesTxt must be 0x1A4 bytes");
static_assert(sizeof(D2MonStatsTxt) == 0x1A8, "D2MonStatsTxt must be 0x1A8 bytes");
static_assert(sizeof(D2PetTypeTxt)  == 0xE0, "D2PetTypeTxt must be 0xE0 bytes");
static_assert(sizeof(D2GfxDataStrc) == 0x48, "1.12a D2GfxDataStrc must be 0x48 bytes");

// ============================================================================
// Client-Specific Typed Structures Used in 0x6FAFB7E0..0x6FB02F04
// ============================================================================
#pragma pack(push, 1)

// Storm window message registration entry (0x6FBA3844, 0x6FBA3874)
struct D2StormMsgRegEntryStrc
{
    int32_t nDispatchType;                              // +0x00 (0 = WM_*, 3 = WM_KEYDOWN)
    uint32_t uMsgOrKey;                                 // +0x04
    BOOL (__stdcall *pfnHandler)(SMSGHANDLER_PARAMS*);  // +0x08
};
static_assert(sizeof(D2StormMsgRegEntryStrc) == 0x0C, "D2StormMsgRegEntryStrc must be 0x0C bytes");

// Client roster pet node (linked list at 0x6FBCB97C)
struct D2RosterPetStrc
{
    int32_t nClassId;               // +0x00
    int32_t nPetType;               // +0x04
    D2UnitGUID dwUnitGUID;          // +0x08
    D2UnitGUID dwOwnerGUID;         // +0x0C
    uint32_t unk0x10[8];            // +0x10
    D2RosterPetStrc* pNext;         // +0x30
};
static_assert(offsetof(D2RosterPetStrc, dwUnitGUID)  == 0x08, "D2RosterPetStrc::dwUnitGUID offset");
static_assert(offsetof(D2RosterPetStrc, dwOwnerGUID) == 0x0C, "D2RosterPetStrc::dwOwnerGUID offset");
static_assert(offsetof(D2RosterPetStrc, pNext)       == 0x30, "D2RosterPetStrc::pNext offset");

// Client roster player entry (returned by 0x6FB281C0)
struct D2RosterUnitPartialStrc
{
    char szName[16];                // +0x00
    D2UnitGUID dwUnitId;            // +0x10
    uint32_t dwPartyLife;           // +0x14
    uint32_t unk0x18;               // +0x18
    uint32_t dwClassId;             // +0x1C
    uint16_t wLevel;                // +0x20
    uint16_t wPartyId;              // +0x22
};
static_assert(offsetof(D2RosterUnitPartialStrc, wPartyId) == 0x22, "D2RosterUnitPartialStrc::wPartyId offset");

// Client unit draw/elevation offset block (at D2ClientUnitGfxStrc::pDrawOffset, +0x30)
struct D2ClientUnitDrawOffsetStrc
{
    int32_t unk0x00;                // +0x00
    int32_t nPosXPrecise;           // +0x04 (subpixel << 11)
    int32_t nPosYPrecise;           // +0x08 (subpixel << 11)
    int32_t nPosZPrecise;           // +0x0C (subpixel << 11)
    int32_t nVelXPrecise;           // +0x10
    int32_t nVelYPrecise;           // +0x14
    int32_t nVelZPrecise;           // +0x18
};
static_assert(offsetof(D2ClientUnitDrawOffsetStrc, nVelZPrecise) == 0x18, "D2ClientUnitDrawOffsetStrc::nVelZPrecise offset");

// Client unit composite/overlay/draw-offset container pointed to by pUnit->pGfxData (+0x54)
struct D2ClientUnitGfxStrc
{
    uint8_t unk0x00[0x2C];                      // +0x00
    void* pOverlays;                            // +0x2C
    D2ClientUnitDrawOffsetStrc* pDrawOffset;    // +0x30
};
static_assert(offsetof(D2ClientUnitGfxStrc, pDrawOffset) == 0x30, "D2ClientUnitGfxStrc::pDrawOffset offset");

// Client light map node allocated by 0x6FB0EAF0 and freed by 0x6FB0E500
struct D2ClientLightMapStrc
{
    uint32_t unk0x00[7];            // +0x00
    int32_t nRadiusSubtiles;        // +0x1C (nLight << 3)
    uint32_t unk0x20[5];            // +0x20
};
static_assert(sizeof(D2ClientLightMapStrc) == 0x34, "D2ClientLightMapStrc must be 0x34 bytes");
static_assert(offsetof(D2ClientLightMapStrc, nRadiusSubtiles) == 0x1C, "D2ClientLightMapStrc::nRadiusSubtiles offset");

// Skill start request packet/command block passed to 0x6FAFD7B0, 0x6FAFF380, 0x6FB00210
struct D2SkillStartRequestStrc
{
    int32_t nSkillId;               // +0x00
    int32_t nSkillLevel;            // +0x04
    union
    {
        int32_t nTargetX;           // +0x08 (location cast: 0x6FAFF380)
        int32_t nTargetType;        // +0x08 (unit cast:     0x6FB00210)
    };
    union
    {
        int32_t nTargetY;           // +0x0C (location cast: 0x6FAFF380)
        D2UnitGUID dwTargetGUID;    // +0x0C (unit cast:     0x6FB00210)
    };
    D2UnitGUID dwItemOwnerGUID;     // +0x10
};
static_assert(sizeof(D2SkillStartRequestStrc) == 0x14, "D2SkillStartRequestStrc must be 0x14 bytes");

// Unit mode transition parameter block passed to 0x6FAF1450 (sizeof == 0x1C)
struct D2UnitModeTransitionStrc
{
    D2UnitStrc* pUnit;              // +0x00
    int32_t nSkillId;               // +0x04
    int32_t nTargetType;            // +0x08
    D2UnitGUID dwTargetGUID;        // +0x0C
    int32_t bHasTargetUnit;         // +0x10
    int32_t nTargetX;               // +0x14
    int32_t nTargetY;               // +0x18
};
static_assert(sizeof(D2UnitModeTransitionStrc) == 0x1C, "D2UnitModeTransitionStrc must be 0x1C bytes");

// Aura search callback context passed in ECX to callbacks invoked by 0x6FAFE9B0
struct D2AuraCallbackCtxStrc
{
    D2UnitStrc* pCaster;            // +0x00
    int32_t nAcceptedCount;         // +0x04
    void* pUserData;                // +0x08
};
static_assert(sizeof(D2AuraCallbackCtxStrc) == 0x0C, "D2AuraCallbackCtxStrc must be 0x0C bytes");

// Deterministic round-robin GUID ring selector state used by 0x6FAFDB00 & 0x6FAFEBA0
struct D2AuraRingTargetTrackerStrc
{
    D2UnitStrc* pNextBestUnit;      // +0x00 (smallest GUID strictly > dwPivotGUID)
    uint32_t dwNextBestGUID;        // +0x04
    D2UnitStrc* pWrapMinUnit;       // +0x08 (smallest GUID <= dwWrapMinGUID)
    uint32_t dwWrapMinGUID;         // +0x0C
    uint32_t dwPivotGUID;           // +0x10
    int32_t nMatchCount;            // +0x14
};
static_assert(sizeof(D2AuraRingTargetTrackerStrc) == 0x18, "D2AuraRingTargetTrackerStrc must be 0x18 bytes");

#pragma pack(pop)

// Function pointer typedefs
typedef int32_t (__fastcall *D2CltSkillFunc_t)(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
typedef int32_t (__fastcall *D2AuraFilterCallback_t)(D2AuraCallbackCtxStrc* pCtx, D2UnitStrc* pTargetUnit);
typedef int32_t (__fastcall *D2CltUnitEventFunc_t)(int32_t nEventType, D2UnitStrc* pUnit, D2UnitStrc* pSourceUnit, int32_t nArg1, int32_t nSkillId, int32_t nSkillLevel);

// ============================================================================
// 1.12a Client Skill Start (`gpfnCltStFunc`) & Do (`gpfnCltDoFunc`) Handler
// Declarations in This Slice (0x6FAFB7E0 .. 0x6FB02F04)
// ============================================================================
// All 18 gpfnCltStFunc entries in 0x6FAFB7E0..0x6FB02F04:
//   [ 1] 0x6FAFE0A0 -> CLTSKILL_St01_Attack
//   [ 2] 0x6FAFE070 -> CLTSKILL_St02_Throw
//   [ 3] 0x6FAFDFC0 -> CLTSKILL_St03_Unsummon
//   [ 4] 0x6FAFD880 -> CLTSKILL_St04_Arrow_Bolt
//   [ 5] 0x6FB00C30 -> CLTSKILL_St05_HostileNonTownTarget
//   [ 6] 0x6FB00970 -> CLTSKILL_St06_DragonTalon
//   [ 7] 0x6FB00420 -> CLTSKILL_St07_CloakOfShadows
//   [ 8] 0x6FB01060 -> CLTSKILL_St08_BladeFury
//   [ 9] 0x6FB006E0 -> CLTSKILL_St09_VelocityPercent
//   [10] 0x6FB00680 -> CLTSKILL_St10_ValidPlayerOrMonsterTarget
//   [11] 0x6FAFB800 -> CLTSKILL_St11_CheckAmmo
//   [12] 0x6FAFB7E0 -> CLTSKILL_St12_Jab
//   [13] 0x6FAFBDD0 -> CLTSKILL_St13_Strafe
//   [14] 0x6FAFBC20 -> CLTSKILL_St14_Fend
//   [15] 0x6FB02150 -> CLTSKILL_St15_Inferno_ArcticBlast
//   [16] 0x6FB01F90 -> CLTSKILL_St16_Telekinesis
//   [17] 0x6FB01C70 -> CLTSKILL_St17_Teleport_CheckTown
//   [53] 0x6FAFBA80 -> CLTSKILL_St53_Zeal_Fury
//
// All 33 gpfnCltDoFunc entries in 0x6FAFB7E0..0x6FB02F04:
//   [  1] 0x6FAFF450 -> CLTSKILL_Do001_Attack
//   [  2] 0x6FB000B0 -> CLTSKILL_Do002_Throw
//   [  3] 0x6FB00CC0 -> CLTSKILL_Do003_PsychicHammer
//   [  4] 0x6FB008F0 -> CLTSKILL_Do004_DragonTalon
//   [  5] 0x6FB01B10 -> CLTSKILL_Do005_ProgressiveLobRain
//   [  6] 0x6FB01620 -> CLTSKILL_Do006_BladeFury
//   [  7] 0x6FB00B90 -> CLTSKILL_Do007_ProgressiveTargetPointMissile
//   [  8] 0x6FB01510 -> CLTSKILL_Do008_StreamSentryAtTarget
//   [  9] 0x6FB01A00 -> CLTSKILL_Do009_ProgressiveGroundPatchCircle
//   [ 10] 0x6FB00AD0 -> CLTSKILL_Do010_ProgressiveTargetNova
//   [ 11] 0x6FB01910 -> CLTSKILL_Do011_ProgressiveRadialRing
//   [ 12] 0x6FB00DE0 -> CLTSKILL_Do012_ProgressiveChargeTargetOverlay
//   [ 13] 0x6FB00CF0 -> CLTSKILL_Do013_ProgressiveChargeSelfOverlay
//   [ 14] 0x6FB00A40 -> CLTSKILL_Do014_ProgressiveCoordMissile
//   [ 15] 0x6FB01770 -> CLTSKILL_Do015_ProgressiveRandomScatter
//   [ 16] 0x6FAFB810 -> CLTSKILL_Do016_Jab
//   [ 17] 0x6FAFC670 -> CLTSKILL_Do017_ParallelSpreadMissiles
//   [ 18] 0x6FAFC510 -> CLTSKILL_Do018_GuidedArrow_BoneSpirit
//   [ 19] 0x6FAFC380 -> CLTSKILL_Do019_ChargedStrike
//   [ 20] 0x6FAFC0F0 -> CLTSKILL_Do020_Strafe
//   [ 21] 0x6FAFB8A0 -> CLTSKILL_Do021_Fend_Zeal_Fury
//   [ 22] 0x6FAFBF30 -> CLTSKILL_Do022_LightningStrike
//   [ 23] 0x6FB02B20 -> CLTSKILL_Do023_ChargedBolt
//   [ 24] 0x6FB02DB0 -> CLTSKILL_Do024_Inferno_ArcticBlast
//   [ 25] 0x6FB01ED0 -> CLTSKILL_Do025_Nova_FrostNova
//   [ 26] 0x6FB027D0 -> CLTSKILL_Do026_FireWall
//   [ 27] 0x6FB01E20 -> CLTSKILL_Do027_ChainLightning
//   [ 28] 0x6FB01D90 -> CLTSKILL_Do028_CollisionCheckedTargetMissile
//   [ 29] 0x6FB01D10 -> CLTSKILL_Do029_LinearSkillMissile
//   [ 83] 0x6FB01820 -> CLTSKILL_Do083_ProgressiveChargedBoltRing
//   [ 84] 0x6FB023B0 -> CLTSKILL_Do084_ThunderStormStrike
//   [ 87] 0x6FB02C40 -> CLTSKILL_Do087_PeriodicAreaLob
//   [ 96] 0x6FB02670 -> CLTSKILL_Do096_TargetOffsetMissile

int32_t __fastcall CLTSKILL_St12_Jab(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St11_CheckAmmo(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do016_Jab(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do021_Fend_Zeal_Fury(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St53_Zeal_Fury(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St14_Fend(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St13_Strafe(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do022_LightningStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do020_Strafe(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do019_ChargedStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do018_GuidedArrow_BoneSpirit(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do017_ParallelSpreadMissiles(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St04_Arrow_Bolt(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St03_Unsummon(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St02_Throw(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St01_Attack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do001_Attack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do002_Throw(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St07_CloakOfShadows(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St10_ValidPlayerOrMonsterTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St09_VelocityPercent(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do004_DragonTalon(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St06_DragonTalon(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do014_ProgressiveCoordMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do010_ProgressiveTargetNova(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do007_ProgressiveTargetPointMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St05_HostileNonTownTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do003_PsychicHammer(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do013_ProgressiveChargeSelfOverlay(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do012_ProgressiveChargeTargetOverlay(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St08_BladeFury(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do008_StreamSentryAtTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do006_BladeFury(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do015_ProgressiveRandomScatter(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do083_ProgressiveChargedBoltRing(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do011_ProgressiveRadialRing(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do009_ProgressiveGroundPatchCircle(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do005_ProgressiveLobRain(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St17_Teleport_CheckTown(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do029_LinearSkillMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do028_CollisionCheckedTargetMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do027_ChainLightning(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do025_Nova_FrostNova(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St16_Telekinesis(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_St15_Inferno_ArcticBlast(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do084_ThunderStormStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do096_TargetOffsetMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do026_FireWall(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do023_ChargedBolt(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do087_PeriodicAreaLob(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
int32_t __fastcall CLTSKILL_Do024_Inferno_ArcticBlast(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);

// Forward declarations for internal helpers referenced before their VA definition
void __fastcall CLTSKILL_RollBackSeqFrameByPercent(D2UnitStrc* pUnit, int32_t nRollbackPct);
int32_t __fastcall CLTSKILL_CheckAmmoAvailable(D2UnitStrc* pUnit);
int32_t __fastcall CLTSKILL_AuraCallback_CountTarget(D2AuraCallbackCtxStrc* pCtx, D2UnitStrc* pTargetUnit);
int32_t __fastcall CLTSKILL_AuraCallback_SelectNextRingTarget(D2AuraCallbackCtxStrc* pCtx, D2UnitStrc* pTargetUnit);
int32_t __fastcall CLTSKILL_ResolveTargetOrDirectionCoords(D2UnitStrc* pUnit, int32_t* pOutX, int32_t* pOutY);
void __stdcall CLTSKILL_SearchUnitsInRadius(D2UnitStrc* pCaster, int32_t nOriginX, int32_t nOriginY, int32_t nRadius, uint32_t dwAuraFilter, D2AuraFilterCallback_t pfnCallback, void* pCallbackUserData, BOOL bCheckMonAuraImmune);
D2UnitStrc* __fastcall CLTSKILL_FindNextTargetInRadius(D2UnitStrc* pCaster, uint32_t dwPivotGUID, uint32_t dwExtraFilter, int32_t nRadius, int32_t nOriginX, int32_t nOriginY, int32_t* pOutMatchCount);
int32_t __fastcall CLTSKILL_CountTargetsInRadius(D2UnitStrc* pCaster, int32_t nRadius, uint32_t dwExtraFilter, int32_t nOriginX, int32_t nOriginY);
int32_t __fastcall CLTSKILL_ResolveProgressiveMissileId(D2UnitStrc* pUnit, int32_t nSkillId);
void __fastcall CLTSKILL_MissileInit_ChargedBoltFromFirstPoint(D2UnitStrc* pMissile, int32_t nSeedOffset);
int32_t __fastcall CLTSKILL_CheckUnitEventChillingArmor(int32_t nEventType, D2UnitStrc* pUnit, D2UnitStrc* pSourceUnit, int32_t nArg1, int32_t nSkillId, int32_t nSkillLevel);

// Forward declarations for Modal UI handlers referenced by Storm message tables
BOOL __stdcall MODAL_OnCancelKeyEscape(SMSGHANDLER_PARAMS* pMsg);
BOOL __stdcall MODAL_OnOkKeyEscape(SMSGHANDLER_PARAMS* pMsg);
BOOL __stdcall MODAL_OnOkCancelKeyReturn(SMSGHANDLER_PARAMS* pMsg);
BOOL __stdcall MODAL_OnOkKeyReturn(SMSGHANDLER_PARAMS* pMsg);
BOOL __stdcall MODAL_OnOkCancelLButtonUp(SMSGHANDLER_PARAMS* pMsg);
BOOL __stdcall MODAL_OnOkLButtonUp(SMSGHANDLER_PARAMS* pMsg);
BOOL __stdcall MODAL_OnOkCancelLButtonDown(SMSGHANDLER_PARAMS* pMsg);
BOOL __stdcall MODAL_OnOkLButtonDown(SMSGHANDLER_PARAMS* pMsg);
void __fastcall MODAL_RenderOkCancelScreen(BOOL bUnused);
void __fastcall MODAL_RenderOkScreen(BOOL bUnused);

// ============================================================================
// 1.12a .rdata & .data Tables / Globals Used by 0x6FAFB7E0 .. 0x6FB02F04
// ============================================================================

// 64-direction radial offset tables (radius = 30 subtiles):
// Identical values at 0x6FB853B8 / 0x6FB855F8 / 0x6FB85890 (X)
// and 0x6FB854B8 / 0x6FB856F8 / 0x6FB85990 (Y) in 1.12a D2Client.dll .rdata.
static const int32_t g_anNovaDirOffsetX_30[64] = {
      0,   2,   5,   8,  11,  14,  16,  19,  21,  23,  24,  26,  27,  28,  29,  29,
     30,  29,  29,  28,  27,  26,  24,  23,  21,  19,  16,  14,  11,   8,   5,   2,
      0,  -2,  -5,  -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29,
    -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11,  -8,  -5,  -2,
};

static const int32_t g_anNovaDirOffsetY_30[64] = {
     30,  29,  29,  28,  27,  26,  24,  23,  21,  19,  16,  14,  11,   8,   5,   2,
      0,  -2,  -5,  -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29,
    -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11,  -8,  -5,  -2,
      0,   2,   5,   8,  11,  14,  16,  19,  21,  23,  24,  26,  27,  28,  29,  29,
};

// 8-octant Charged Bolt path step tables (0x6FB855D8 = X, 0x6FB855B8 = Y)
static const int32_t g_anChargedBoltOctantStepX[8] = {  2,  2,  0, -2, -2, -2,  0,  2 }; // 1.12a: 0x6FB855D8
static const int32_t g_anChargedBoltOctantStepY[8] = {  0,  2,  2,  2,  0, -2, -2, -2 }; // 1.12a: 0x6FB855B8

// Charged Bolt wiggle tuning constants (0x6FBAA3D0, 0x6FBAA3D4)
static const int32_t g_nChargedBoltBaseTurnStep     = 2;  // 1.12a: 0x6FBAA3D0
static const int32_t g_nChargedBoltTurnFlipInterval = 15; // 1.12a: 0x6FBAA3D4

// Multi-ring radial burst tables used by 0x6FAFF710 (0x6FB857F8 .. 0x6FB8588C)
static const int32_t g_anRingCardinalDiagY[8] = {   0,   0,  20, -20,  14,  14, -14, -14 }; // 1.12a: 0x6FB857F8
static const int32_t g_anRingCardinalDiagX[8] = {  20, -20,   0,   0,  14, -14, -14,  14 }; // 1.12a: 0x6FB85818
static const int32_t g_anRingMinorAxis[7]     = {   8,   2,  11,   4,  13,   6,   9 };      // 1.12a: 0x6FB85838
static const int32_t g_anRingMajorAxis[7]     = {  18,  20,  17,  20,  15,  19,  18 };      // 1.12a: 0x6FB85854
static const int32_t g_anQuadrantSignY[4]     = {  -1,  -1,   1,   1 };                     // 1.12a: 0x6FB85870
static const int32_t g_anQuadrantSignX[4]     = {  -1,   1,   1,  -1 };                     // 1.12a: 0x6FB85880

// Client unit event handler table (1.12a: 0x6FBA383C)
static const D2CltUnitEventFunc_t g_apfnCltUnitEventFuncs[2] = {
    nullptr,                                // [0]
    CLTSKILL_CheckUnitEventChillingArmor,   // [1] 0x6FB02270
};

// Storm message handler tables for Modal UI (1.12a: 0x6FBA3844, 0x6FBA3874)
static const D2StormMsgRegEntryStrc g_aOkModalMsgTable[4] = {
    { 0, WM_LBUTTONDOWN, MODAL_OnOkLButtonDown },       // 0x6FAFD140
    { 0, WM_LBUTTONUP,   MODAL_OnOkLButtonUp   },       // 0x6FAFCF70
    { 3, VK_ESCAPE,      MODAL_OnOkKeyEscape   },       // 0x6FAFC900
    { 3, VK_RETURN,      MODAL_OnOkKeyReturn   },       // 0x6FAFCCB0
};

static const D2StormMsgRegEntryStrc g_aOkCancelModalMsgTable[4] = {
    { 0, WM_LBUTTONDOWN, MODAL_OnOkCancelLButtonDown }, // 0x6FAFD060
    { 0, WM_LBUTTONUP,   MODAL_OnOkCancelLButtonUp   }, // 0x6FAFCE90
    { 3, VK_ESCAPE,      MODAL_OnCancelKeyEscape     }, // 0x6FAFC840
    { 3, VK_RETURN,      MODAL_OnOkCancelKeyReturn   }, // 0x6FAFCAC0
};

// External client skill dispatch tables (1.12a: 0x6FBA3510, 0x6FBA3628, 0x6FBA3630, 0x6FBA3838)
extern const D2CltSkillFunc_t gpfnCltStFunc[70];
extern const int32_t          gnCltStFuncs;
extern const D2CltSkillFunc_t gpfnCltDoFunc[130];
extern const int32_t          gnCltDoFuncs;

// External D2Client globals referenced by 0x6FAFB7E0..0x6FB02F04
extern int32_t          g_nScreenWidth;                      // 1.12a: 0x6FB8C6E0
extern int32_t          g_nScreenHeight;                     // 1.12a: 0x6FB8C6E4
extern void (__fastcall *gpfnDefaultScreenRender)(BOOL);     // 1.12a: 0x6FB8FF98 (0x6FB2CA10)
extern void (__fastcall *gpfnModalOkScreenRender)(BOOL);     // 1.12a: 0x6FB8FF9C (0x6FAFD290)
extern void (__fastcall *gpfnModalOkCancelRender)(BOOL);     // 1.12a: 0x6FB8FFA0 (0x6FAFD1F0)
extern int32_t          g_nMouseY;                           // 1.12a: 0x6FBB1634
extern int32_t          g_nMouseX;                           // 1.12a: 0x6FBB1638
extern int32_t          gnModalScreenState;                  // 1.12a: 0x6FBB3298
extern void (__fastcall *gpfnActiveScreenRender)(BOOL);      // 1.12a: 0x6FBB329C
extern uint32_t         gdwClientTickCount;                  // 1.12a: 0x6FBB32B0
extern int32_t          gdwaSkillDelayEndFrame;              // 1.12a: 0x6FBB3314
extern int32_t          gaBeltSlotScreenX[8];                // 1.12a: 0x6FBC26F0
extern int32_t          gaBeltSlotScreenY[8];                // 1.12a: 0x6FBC2710
extern int32_t          gaBeltSlotGridCol[8];                // 1.12a: 0x6FBC2730
extern int32_t          gaBeltSlotFlatIndex[8];              // 1.12a: 0x6FBC2770
extern BOOL             gbModalUseExpansionPromptText;       // 1.12a: 0x6FBC2790
extern D2UnitStrc*      gapServerSideUnitHashTables[6][128]; // 1.12a: 0x6FBCA960
extern D2RosterPetStrc* gpRosterPetList;                     // 1.12a: 0x6FBCB97C
extern int32_t          gnUiOffsetX;                         // 1.12a: 0x6FBCBD28
extern int32_t          gnUiOffsetY;                         // 1.12a: 0x6FBCBD2C
extern int32_t          gnInputDragStartX;                   // 1.12a: 0x6FBCC040
extern int32_t          gnInputDragStartY;                   // 1.12a: 0x6FBCC044
extern int32_t          gnInputDragEndX;                     // 1.12a: 0x6FBCC05C
extern int32_t          gnInputDragEndY;                     // 1.12a: 0x6FBCC060
extern BOOL             gbBeltPopupOpen;                     // 1.12a: 0x6FBCC238
extern BOOL             gbMiniPanelCompactLayout;            // 1.12a: 0x6FBCC23C
extern D2CellFileStrc*  gpPopupOkBoxCell1;                   // 1.12a: 0x6FBCC27C
extern D2CellFileStrc*  gpOkCancelButtonCell;                // 1.12a: 0x6FBCC280
extern D2CellFileStrc*  gpPopupOkBoxCell2;                   // 1.12a: 0x6FBCC284
extern BOOL             gbModalButtonMouseCaptured;          // 1.12a: 0x6FBCC288
extern BOOL             gbModalOkButtonPressed;              // 1.12a: 0x6FBCC28C
extern BOOL             gbModalCancelButtonPressed;          // 1.12a: 0x6FBCC290
extern BOOL             gbModalOkSingleDismissed;            // 1.12a: 0x6FBCC294
extern BOOL             gbModalOkAccepted;                   // 1.12a: 0x6FBCC298
extern BOOL             gbModalCancelAccepted;               // 1.12a: 0x6FBCC29C
extern D2UnitStrc*      gpPlayerUnit;                        // 1.12a: 0x6FBCC3D0

// ============================================================================
// External D2Client Helper Declarations (Outside 0x6FAFB7E0..0x6FB02F04)
// ============================================================================
extern D2ActiveRoomStrc*    __fastcall D2Client_FindRoomCoveringCoords_6FAB1A00(D2ActiveRoomStrc* pOriginRoom, int32_t nX, int32_t nY);            // 1.12a: 0x6FAB1A00
extern void                 __fastcall D2Client_ScreenShake_6FAC3E60(int32_t nShakeType);                                                          // 1.12a: 0x6FAC3E60
extern D2UnitStrc*          __fastcall D2Client_FindUnitInHashChain_6FACE550(D2UnitStrc* pHead, D2UnitGUID dwUnitGUID, int32_t nUnitType);         // 1.12a: 0x6FACE550
extern D2UnitStrc*          __fastcall D2Client_GetUnitTargetUnit_6FACF5B0(D2UnitStrc* pUnit);                                                     // 1.12a: 0x6FACF5B0
extern void                 __fastcall D2Client_FaceUnitTarget_6FACF5F0(D2UnitStrc* pUnit, int32_t bForce);                                        // 1.12a: 0x6FACF5F0
extern D2UnitStrc*          __fastcall D2Client_GetMissileOwnerUnit_6FACF710(D2UnitStrc* pMissile);                                                // 1.12a: 0x6FACF710
extern BOOL                 __fastcall D2Client_CanTargetAlliedUnit_6FACF7B0(D2UnitStrc* pPlayer, D2UnitStrc* pTargetUnit);                             // 1.12a: 0x6FACF7B0
extern BOOL                 __fastcall D2Client_CanUnitAttackTarget_6FACF980(D2UnitStrc* pTarget, D2UnitStrc* pAttacker);                          // 1.12a: 0x6FACF980
extern D2UnitStrc*          __fastcall D2Client_SpawnTargetedLinearMissile_6FAD1940(D2UnitStrc* pOwner, int32_t nMissileId, int32_t nTargetX, int32_t nTargetY, int32_t nSkillId, int32_t nSkillLevel); // 1.12a: 0x6FAD1940
extern D2UnitStrc*          __fastcall D2Client_SpawnLinearSkillMissile_6FAD1E80(D2UnitStrc* pOwner, int32_t nMissileId, int32_t nTargetX, int32_t nTargetY, int32_t nSkillId, int32_t nSkillLevel);    // 1.12a: 0x6FAD1E80
extern D2UnitStrc*          __fastcall D2Client_CreateClientMissile_6FAD4B20(D2MissileStrc* pMissileParams);                                       // 1.12a: 0x6FAD4B20
extern D2UnitStrc*          __fastcall D2Client_SpawnLobMissile_6FADB4B0(D2UnitStrc* pOwner, int32_t nMissileId, int32_t nOriginOffsetX, int32_t nOriginOffsetY, int32_t nTargetX, int32_t nTargetY, int32_t nSkillId, int32_t nSkillLevel, int32_t nExtraRange); // 1.12a: 0x6FADB4B0
extern void                 __fastcall D2Client_PlaySoundEx_6FAE1FA0(D2UnitStrc* pUnit, int32_t nSoundId, int32_t nDelayFrames, int32_t nArg4, int32_t nArg5); // 1.12a: 0x6FAE1FA0
extern int32_t              __fastcall D2Client_GetWeaponSoundDelayFrames_6FAE6720(D2UnitStrc* pUnit, int32_t nHitClass);                          // 1.12a: 0x6FAE6720
extern int32_t              __fastcall D2Client_GetWeaponSwingSoundId_6FAE6780(D2UnitStrc* pUnit, int32_t nHitClass);                              // 1.12a: 0x6FAE6780
extern void                 __fastcall D2Client_PlayCharacterBarkSound_6FAE7250(D2UnitStrc* pUnit, int32_t nBarkIndex);                            // 1.12a: 0x6FAE7250
extern void                 __fastcall D2Client_EnsureUnitPathAllocated_6FAF0D20(D2UnitStrc* pUnit);                                               // 1.12a: 0x6FAF0D20
extern void                 __fastcall D2Client_SetUnitCastOrNeutralAnimMode_6FAF13C0(D2UnitStrc* pUnit, int32_t nAnimMode);                       // 1.12a: 0x6FAF13C0
extern void                 __fastcall D2Client_ChangeUnitModeEx_6FAF1450(int32_t nAnimMode, D2UnitStrc* pUnit, D2UnitModeTransitionStrc* pParams, BOOL bHasParams); // 1.12a: 0x6FAF1450
extern void                 __fastcall D2Client_FreeClientLightMap_6FB0E500(D2ClientLightMapStrc* pLightMap);                                      // 1.12a: 0x6FB0E500
extern D2ClientLightMapStrc* __fastcall D2Client_AllocClientLightMap_6FB0EAF0(int32_t nType, int32_t nIntensity, uint8_t nR, uint8_t nG, uint8_t nB); // 1.12a: 0x6FB0EAF0
extern void                 __cdecl    D2Client_EndRenderFramePresent_6FB1A660(void);                                                              // 1.12a: 0x6FB1A660
extern void                 __fastcall D2Client_UnregisterStormMsgEntry_6FB1CE40(HWND hWnd, int32_t nDispatchType, uint32_t uMsgOrKey, BOOL (__stdcall *pfnHandler)(SMSGHANDLER_PARAMS*)); // 1.12a: 0x6FB1CE40
extern void                 __fastcall D2Client_RegisterStormMsgEntry_6FB1CEC0(HWND hWnd, int32_t nDispatchType, uint32_t uMsgOrKey, BOOL (__stdcall *pfnHandler)(SMSGHANDLER_PARAMS*));   // 1.12a: 0x6FB1CEC0
extern void                 __fastcall D2Client_RunModalEventLoop_6FB1CF40(BOOL (__stdcall *pfnModalPump)(uint32_t));                              // 1.12a: 0x6FB1CF40
extern D2RosterUnitPartialStrc* __fastcall D2Client_FindRosterPlayerByGUID_6FB281C0(D2UnitGUID dwUnitGUID);                                        // 1.12a: 0x6FB281C0
extern void                 __cdecl    D2Client_PumpNetworkAndAudio_6FB299D0(void);                                                                // 1.12a: 0x6FB299D0
extern void                 __fastcall D2Client_FinishPaladinCharge_6FB373F0(D2UnitStrc* pUnit, int32_t nSkillId);                                  // 1.12a: 0x6FB373F0
extern void                 __cdecl    D2Client_ClearUiDirtyState_6FB3A7F0(void);                                                                  // 1.12a: 0x6FB3A7F0
extern BOOL                 __fastcall D2Client_HasUnitOverlay_6FB40A60(D2UnitStrc* pUnit, int32_t nOverlayId);                                    // 1.12a: 0x6FB40A60
extern void                 __fastcall D2Client_AttachUnitOverlay_6FB430E0(D2UnitStrc* pUnit, int32_t nOverlayId, int32_t nType, int32_t nArg3, int32_t nArg4, int32_t nArg5, int32_t nArg6, int32_t nArg7); // 1.12a: 0x6FB430E0
extern void                 __cdecl    D2Client_DrawSoftwareCursor_6FB4F5B0(void);                                                                 // 1.12a: 0x6FB4F5B0
extern int32_t              __fastcall D2Client_DispatchUnitEventList_6FB6A300(D2UnitStrc* pUnit, int32_t nEventType, int32_t nArg1, D2CltUnitEventFunc_t pfnCallback, D2UnitStrc* pSourceUnit, int32_t nSkillId); // 1.12a: 0x6FB6A300
extern D2CellFileStrc*      __fastcall D2Client_LoadUiCelArchive_6FB6EF70(const char* szPath, int32_t nFileType);                                  // 1.12a: 0x6FB6EF70
extern int32_t              __fastcall D2Client_GetRosterPetTypeByGUID_6FB73130(D2UnitGUID dwPetGUID);                                             // 1.12a: 0x6FB73130
extern void                 __fastcall D2Client_FinishBarbarianLeap_6FB73E70(D2UnitStrc* pUnit, int32_t nSkillId);                                  // 1.12a: 0x6FB73E70
extern void                 __fastcall D2Client_FinishBarbarianWhirlwind_6FB74C30(D2UnitStrc* pUnit);                                              // 1.12a: 0x6FB74C30
extern void                 __fastcall D2Client_SetUnitDrawElevationOffset_6FB755B0(D2UnitStrc* pUnit, int32_t nOffsetX, int32_t nOffsetY, int32_t nOffsetZ); // 1.12a: 0x6FB755B0

// ============================================================================
// Inline Table / Unit Coordinate Helpers Matching 1.12a Inlined Expansions
// ============================================================================
static inline D2SkillsTxt* CLTSKILL_GetSkillsTxt(int32_t nSkillId)
{
    if (nSkillId >= 0 && sgptDataTables && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
    {
        return &sgptDataTables->pSkillsTxt[nSkillId];
    }
    return nullptr;
}

static inline D2MissilesTxt* CLTSKILL_GetMissilesTxt(int32_t nMissileId)
{
    if (nMissileId >= 0 && sgptDataTables && nMissileId < sgptDataTables->nMissilesTxtRecordCount)
    {
        return &sgptDataTables->pMissilesTxt[nMissileId];
    }
    return nullptr;
}

static inline D2StatesTxt* CLTSKILL_GetStatesTxt(int32_t nStateId)
{
    if (nStateId >= 0 && sgptDataTables && nStateId < sgptDataTables->nStatesTxtRecordCount)
    {
        return &sgptDataTables->pStatesTxt[nStateId];
    }
    return nullptr;
}

static inline D2SkillDescTxt* CLTSKILL_GetSkillDescTxtFromSkill(int32_t nSkillId)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return nullptr;
    }
    const int32_t nSkillDesc = pSkillsTxt->wSkillDesc;
    if (nSkillDesc >= 0 && nSkillDesc < sgptDataTables->nSkillDescTxtRecordCount)
    {
        return &sgptDataTables->pSkillDescTxt[nSkillDesc];
    }
    return nullptr;
}

static inline D2MonStats2Txt* CLTSKILL_GetMonStats2TxtFromMonsterId(int32_t nMonsterId)
{
    D2MonStatsTxt* pMonStatsTxt = DATATBLS_GetMonStatsTxtRecord(nMonsterId);
    if (!pMonStatsTxt)
    {
        return nullptr;
    }
    const int32_t nMonStatsEx = pMonStatsTxt->wMonStatsEx;
    if (nMonStatsEx >= 0 && nMonStatsEx < sgptDataTables->nMonStats2TxtRecordCount)
    {
        return &sgptDataTables->pMonStats2Txt[nMonStatsEx];
    }
    return nullptr;
}

static inline D2PetTypeTxt* CLTSKILL_GetPetTypeTxt(int32_t nPetType)
{
    if (nPetType >= 0 && sgptDataTables && nPetType < sgptDataTables->nPetTypeTxtRecordCount)
    {
        return &sgptDataTables->pPetTypeTxt[nPetType];
    }
    return nullptr;
}

static inline int32_t CLTSKILL_GetSkillMaxLevel(int32_t nSkillId)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 20;
    }
    return (pSkillsTxt->wMaxLvl > 0) ? pSkillsTxt->wMaxLvl : 20;
}

static inline int32_t CLTSKILL_GetUnitX(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }
    if (pUnit->dwUnitType == UNIT_OBJECT || pUnit->dwUnitType == UNIT_ITEM || pUnit->dwUnitType == UNIT_TILE)
    {
        return pUnit->pStaticPath ? pUnit->pStaticPath->nXPos : 0;
    }
    return pUnit->pDynamicPath ? PATH_GetXPosition(pUnit->pDynamicPath) : 0;
}

static inline int32_t CLTSKILL_GetUnitY(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }
    if (pUnit->dwUnitType == UNIT_OBJECT || pUnit->dwUnitType == UNIT_ITEM || pUnit->dwUnitType == UNIT_TILE)
    {
        return pUnit->pStaticPath ? pUnit->pStaticPath->nYPos : 0;
    }
    return pUnit->pDynamicPath ? PATH_GetYPosition(pUnit->pDynamicPath) : 0;
}

static inline D2UnitStrc* CLTSKILL_FindServerUnit(int32_t nUnitType, D2UnitGUID dwUnitGUID)
{
    if (nUnitType < 0 || nUnitType >= UNIT_TYPES_COUNT)
    {
        return nullptr;
    }
    D2UnitStrc* pBucketHead = gapServerSideUnitHashTables[nUnitType][dwUnitGUID & 0x7Fu];
    return D2Client_FindUnitInHashChain_6FACE550(pBucketHead, dwUnitGUID, nUnitType);
}

// ============================================================================
// Section 1: Amazon Client Skills + Zeal/Fury Start (0x6FAFB7E0 .. 0x6FAFC82E)
// ============================================================================

// 1.12a: 0x6FAFB7E0 (gpfnCltStFunc[12])
int32_t __fastcall CLTSKILL_St12_Jab(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, 0);
    return 1;
}

// 1.12a: 0x6FAFB800 (gpfnCltStFunc[11])
int32_t __fastcall CLTSKILL_St11_CheckAmmo(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    return CLTSKILL_CheckAmmoAvailable(pUnit) != 0 ? 1 : 0;
}

// 1.12a: 0x6FAFB810 (gpfnCltDoFunc[16])
int32_t __fastcall CLTSKILL_Do016_Jab(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    if (pUnit->nActionFrame == 3)
    {
        D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
        if (!pSkillsTxt)
        {
            return 0;
        }

        if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            const int16_t nSoundA = static_cast<int16_t>(pSkillsTxt->wDoSoundA);
            if (nSoundA > 0)
            {
                D2Client_PlaySoundEx_6FAE1FA0(pUnit, nSoundA, 0, 0, 0);
            }
        }

        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            const int16_t nSoundB = static_cast<int16_t>(pSkillsTxt->wDoSoundB);
            if (nSoundB > 0)
            {
                D2Client_PlaySoundEx_6FAE1FA0(pUnit, nSoundB, 0, 0, 0);
            }
        }
    }

    return 1;
}

// 1.12a: 0x6FAFB8A0 (gpfnCltDoFunc[21])
int32_t __fastcall CLTSKILL_Do021_Fend_Zeal_Fury(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    if (!CLTSKILL_GetSkillsTxt(nSkillId))
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const D2UnitGUID dwTargetGUID = static_cast<D2UnitGUID>(SKILLS_GetParam3(pSkill));
    const int32_t nTargetType = SKILLS_GetParam2(pSkill);
    D2UnitStrc* pTarget = CLTSKILL_FindServerUnit(nTargetType, dwTargetGUID);
    const int32_t nSearchRadius = UNITS_GetMeleeRange(pUnit) + 4;

    if (!pTarget || !UNITS_IsInMeleeRange(pUnit, pTarget, 0))
    {
        D2AuraRingTargetTrackerStrc tracker = {};
        tracker.pNextBestUnit  = nullptr;
        tracker.dwNextBestGUID = D2UnitInvalidGUID;
        tracker.pWrapMinUnit   = nullptr;
        tracker.dwWrapMinGUID  = dwTargetGUID;
        tracker.dwPivotGUID    = dwTargetGUID;
        tracker.nMatchCount    = 0;

        CLTSKILL_SearchUnitsInRadius(
            pUnit,
            0,
            0,
            nSearchRadius,
            0x2A283u,
            CLTSKILL_AuraCallback_SelectNextRingTarget,
            &tracker,
            FALSE);

        pTarget = tracker.pNextBestUnit ? tracker.pNextBestUnit : tracker.pWrapMinUnit;
        if (!pTarget)
        {
            return 0;
        }
    }

    const int32_t nRemainingSwings = SKILLS_GetParam1(pSkill) - 1;
    SKILLS_SetParam1(pSkill, nRemainingSwings);

    if (nRemainingSwings > 0)
    {
        const uint32_t dwPivotGUID = pTarget ? pTarget->dwUnitId : D2UnitInvalidGUID;
        D2AuraRingTargetTrackerStrc nextTracker = {};
        nextTracker.pNextBestUnit  = nullptr;
        nextTracker.dwNextBestGUID = D2UnitInvalidGUID;
        nextTracker.pWrapMinUnit   = nullptr;
        nextTracker.dwWrapMinGUID  = dwPivotGUID;
        nextTracker.dwPivotGUID    = dwPivotGUID;
        nextTracker.nMatchCount    = 0;

        CLTSKILL_SearchUnitsInRadius(
            pUnit,
            0,
            0,
            nSearchRadius,
            0x2A283u,
            CLTSKILL_AuraCallback_SelectNextRingTarget,
            &nextTracker,
            FALSE);

        D2UnitStrc* pNextTarget = nextTracker.pNextBestUnit ? nextTracker.pNextBestUnit : nextTracker.pWrapMinUnit;
        if (!pNextTarget)
        {
            return 0;
        }

        D2Common_10216(pUnit->pDynamicPath, CLTSKILL_GetUnitX(pNextTarget), CLTSKILL_GetUnitY(pNextTarget), 0);
        SKILLS_SetParam2(pSkill, pNextTarget->dwUnitType);
        SKILLS_SetParam3(pSkill, pNextTarget->dwUnitId);

        const int32_t nUsedSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
        D2SkillsTxt* pUsedSkillsTxt = CLTSKILL_GetSkillsTxt(nUsedSkillId);
        const int32_t nRollbackPct = pUsedSkillsTxt ? pUsedSkillsTxt->dwParam[1] : 0;
        CLTSKILL_RollBackSeqFrameByPercent(pUnit, nRollbackPct);

        const int32_t nHitClass = UNITS_GetHitClass(pUnit);
        const int32_t nSwingSound = D2Client_GetWeaponSwingSoundId_6FAE6780(pUnit, nHitClass);
        D2Client_PlaySoundEx_6FAE1FA0(pUnit, nSwingSound, 0, 0, 0);
    }

    return 1;
}

// 1.12a: 0x6FAFBA80 (gpfnCltStFunc[53])
int32_t __fastcall CLTSKILL_St53_Zeal_Fury(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nSearchRadius = UNITS_GetMeleeRange(pUnit) + 4;
    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        D2AuraRingTargetTrackerStrc tracker = {};
        tracker.pNextBestUnit  = nullptr;
        tracker.dwNextBestGUID = D2UnitInvalidGUID;
        tracker.pWrapMinUnit   = nullptr;
        tracker.dwWrapMinGUID  = D2UnitInvalidGUID;
        tracker.dwPivotGUID    = D2UnitInvalidGUID;
        tracker.nMatchCount    = 0;

        CLTSKILL_SearchUnitsInRadius(
            pUnit,
            0,
            0,
            nSearchRadius,
            0x2A283u,
            CLTSKILL_AuraCallback_SelectNextRingTarget,
            &tracker,
            FALSE);

        pTarget = tracker.pNextBestUnit ? tracker.pNextBestUnit : tracker.pWrapMinUnit;
        if (!pTarget)
        {
            SKILLS_SetParam1(pSkill, 0);
            return 1;
        }
    }

    const int32_t nSwings = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    SKILLS_SetParam1(pSkill, nSwings);

    const int32_t nTargetY = CLTSKILL_GetUnitY(pTarget);
    const int32_t nTargetX = CLTSKILL_GetUnitX(pTarget);
    D2Common_10216(pUnit->pDynamicPath, nTargetX, nTargetY, 0);

    SKILLS_SetParam2(pSkill, pTarget->dwUnitType);
    SKILLS_SetParam3(pSkill, pTarget->dwUnitId);
    return 1;
}

// 1.12a: 0x6FAFBC20 (gpfnCltStFunc[14])
int32_t __fastcall CLTSKILL_St14_Fend(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nSearchRadius = UNITS_GetMeleeRange(pUnit) + 4;
    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    int32_t nNearbyCount = 0;

    if (!pTarget)
    {
        pTarget = CLTSKILL_FindNextTargetInRadius(
            pUnit,
            D2UnitInvalidGUID,
            0x20003u,
            nSearchRadius,
            0,
            0,
            &nNearbyCount);
        if (!pTarget)
        {
            SKILLS_SetParam1(pSkill, 0);
            return 1;
        }
    }
    else
    {
        CLTSKILL_SearchUnitsInRadius(
            pUnit,
            0,
            0,
            nSearchRadius,
            0x2A783u,
            CLTSKILL_AuraCallback_CountTarget,
            &nNearbyCount,
            FALSE);
    }

    int32_t nSwings = nNearbyCount;
    const int32_t nMaxSwings = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    if (nSwings >= nMaxSwings)
    {
        nSwings = nMaxSwings;
    }

    SKILLS_SetParam1(pSkill, nSwings);

    const int32_t nTargetY = CLTSKILL_GetUnitY(pTarget);
    const int32_t nTargetX = CLTSKILL_GetUnitX(pTarget);
    D2Common_10216(pUnit->pDynamicPath, nTargetX, nTargetY, 0);

    SKILLS_SetParam2(pSkill, pTarget->dwUnitType);
    SKILLS_SetParam3(pSkill, pTarget->dwUnitId);
    return 1;
}

// 1.12a: 0x6FAFBDD0 (gpfnCltStFunc[13])
int32_t __fastcall CLTSKILL_St13_Strafe(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_CheckAmmoAvailable(pUnit))
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel);
    D2UnitStrc* pTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);
    int32_t nTargetCount = 0;

    if (!pTarget)
    {
        pTarget = CLTSKILL_FindNextTargetInRadius(
            pUnit,
            D2UnitInvalidGUID,
            0x03u,
            nAuraRange,
            0,
            0,
            &nTargetCount);
    }
    else
    {
        nTargetCount = CLTSKILL_CountTargetsInRadius(pUnit, nAuraRange, 0x03u, 0, 0);
    }

    const int32_t nMinShots = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[2], nSkillId, nSkillLevel);
    const int32_t nMaxShots = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);

    int32_t nClampedCount = nTargetCount;
    if (nClampedCount > nMaxShots)
    {
        nClampedCount = nMaxShots;
    }
    if (nMinShots > nClampedCount)
    {
        nClampedCount = nMinShots;
    }
    int32_t nFinalShots = nMaxShots;
    if (nClampedCount < nFinalShots)
    {
        nFinalShots = nClampedCount;
    }

    SKILLS_SetParam1(pSkill, nFinalShots);

    if (pTarget)
    {
        SKILLS_SetParam2(pSkill, pTarget->dwUnitType);
        SKILLS_SetParam3(pSkill, pTarget->dwUnitId);
        D2Common_10216(pUnit->pDynamicPath, CLTSKILL_GetUnitX(pTarget), CLTSKILL_GetUnitY(pTarget), 0);
    }
    else
    {
        SKILLS_SetParam2(pSkill, UNIT_MONSTER);
        SKILLS_SetParam3(pSkill, static_cast<int32_t>(D2UnitInvalidGUID));
    }

    return 1;
}

// 1.12a: 0x6FAFBF30 (gpfnCltDoFunc[22])
int32_t __fastcall CLTSKILL_Do022_LightningStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    D2UnitStrc* pPrimaryTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);
    if (!pPrimaryTarget)
    {
        return 0;
    }

    const int32_t nSearchRadius = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    const D2UnitGUID dwPrimaryGUID = pPrimaryTarget->dwUnitId;
    const int32_t nOriginY = CLTSKILL_GetUnitY(pPrimaryTarget);
    const int32_t nOriginX = CLTSKILL_GetUnitX(pPrimaryTarget);

    D2AuraRingTargetTrackerStrc tracker = {};
    tracker.pNextBestUnit  = nullptr;
    tracker.dwNextBestGUID = D2UnitInvalidGUID;
    tracker.pWrapMinUnit   = nullptr;
    tracker.dwWrapMinGUID  = dwPrimaryGUID;
    tracker.dwPivotGUID    = dwPrimaryGUID;
    tracker.nMatchCount    = 0;

    CLTSKILL_SearchUnitsInRadius(
        pUnit,
        nOriginX,
        nOriginY,
        nSearchRadius,
        0x0A283u,
        CLTSKILL_AuraCallback_SelectNextRingTarget,
        &tracker,
        FALSE);

    D2UnitStrc* pBounceTarget = tracker.pNextBestUnit ? tracker.pNextBestUnit : tracker.pWrapMinUnit;
    if (!pBounceTarget)
    {
        return 0;
    }

    const int32_t nChainHits = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[1], nSkillId, nSkillLevel);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x20u;
    missileParams.pOwner      = pUnit;
    missileParams.pOrigin     = pPrimaryTarget;
    missileParams.nMissile    = nMissileId;
    missileParams.nTargetX    = CLTSKILL_GetUnitX(pBounceTarget);
    missileParams.nTargetY    = CLTSKILL_GetUnitY(pBounceTarget);
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile_6FAD4B20(&missileParams);
    if (pMissile)
    {
        MISSILE_SetTargetX(pMissile, nChainHits);
    }

    return 1;
}

// 1.12a: 0x6FAFC0F0 (gpfnCltDoFunc[20])
int32_t __fastcall CLTSKILL_Do020_Strafe(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nShotsRemaining = SKILLS_GetParam1(pSkill);
    if (nShotsRemaining <= 0)
    {
        return 0;
    }

    const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel);
    const D2UnitGUID dwStoredGUID = static_cast<D2UnitGUID>(SKILLS_GetParam3(pSkill));
    const int32_t nStoredType = SKILLS_GetParam2(pSkill);
    D2UnitStrc* pTarget = CLTSKILL_FindServerUnit(nStoredType, dwStoredGUID);

    if (!pTarget)
    {
        D2AuraRingTargetTrackerStrc tracker = {};
        tracker.pNextBestUnit  = nullptr;
        tracker.dwNextBestGUID = D2UnitInvalidGUID;
        tracker.pWrapMinUnit   = nullptr;
        tracker.dwWrapMinGUID  = dwStoredGUID;
        tracker.dwPivotGUID    = dwStoredGUID;
        tracker.nMatchCount    = 0;

        CLTSKILL_SearchUnitsInRadius(
            pUnit,
            0,
            0,
            nAuraRange,
            0x0A283u,
            CLTSKILL_AuraCallback_SelectNextRingTarget,
            &tracker,
            FALSE);

        pTarget = tracker.pNextBestUnit ? tracker.pNextBestUnit : tracker.pWrapMinUnit;
    }

    if (pTarget)
    {
        UNITS_SetTargetUnitForDynamicUnit(pUnit, pTarget);
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (UNITS_GetWeaponClass(pUnit) != WEAPONCLASS_BOW)
    {
        const int16_t nBoltMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
        if (nBoltMissileId >= 0)
        {
            nMissileId = nBoltMissileId;
        }
    }

    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x21u;
    missileParams.pOwner      = pUnit;
    missileParams.nMissile    = nMissileId;
    missileParams.nX          = CLTSKILL_GetUnitX(pUnit);
    missileParams.nY          = CLTSKILL_GetUnitY(pUnit);
    missileParams.nTargetX    = nTargetX;
    missileParams.nTargetY    = nTargetY;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    D2Client_CreateClientMissile_6FAD4B20(&missileParams);

    const int32_t nNewRemaining = nShotsRemaining - 1;
    SKILLS_SetParam1(pSkill, nNewRemaining);

    if (nNewRemaining > 0)
    {
        const uint32_t dwPivotGUID = pTarget ? pTarget->dwUnitId : D2UnitInvalidGUID;
        D2AuraRingTargetTrackerStrc nextTracker = {};
        nextTracker.pNextBestUnit  = nullptr;
        nextTracker.dwNextBestGUID = D2UnitInvalidGUID;
        nextTracker.pWrapMinUnit   = nullptr;
        nextTracker.dwWrapMinGUID  = dwPivotGUID;
        nextTracker.dwPivotGUID    = dwPivotGUID;
        nextTracker.nMatchCount    = 0;

        CLTSKILL_SearchUnitsInRadius(
            pUnit,
            0,
            0,
            nAuraRange,
            0x0A283u,
            CLTSKILL_AuraCallback_SelectNextRingTarget,
            &nextTracker,
            FALSE);

        D2UnitStrc* pNextTarget = nextTracker.pNextBestUnit ? nextTracker.pNextBestUnit : nextTracker.pWrapMinUnit;
        if (!pNextTarget)
        {
            return 0;
        }

        D2Common_10216(pUnit->pDynamicPath, CLTSKILL_GetUnitX(pNextTarget), CLTSKILL_GetUnitY(pNextTarget), 0);
        SKILLS_SetParam2(pSkill, pNextTarget->dwUnitType);
        SKILLS_SetParam3(pSkill, pNextTarget->dwUnitId);

        const int32_t nRollbackPct = pSkillsTxt->dwParam[5];
        CLTSKILL_RollBackSeqFrameByPercent(pUnit, nRollbackPct);
    }

    return 1;
}

// 1.12a: 0x6FAFC380 (gpfnCltDoFunc[19])
int32_t __fastcall CLTSKILL_Do019_ChargedStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    const int32_t nTargetX = CLTSKILL_GetUnitX(pTarget);
    const int32_t nTargetY = CLTSKILL_GetUnitY(pTarget);
    const int32_t nCasterX = CLTSKILL_GetUnitX(pUnit);
    const int32_t nCasterY = CLTSKILL_GetUnitY(pUnit);

    const int32_t nDestX = (2 * nTargetX) - nCasterX;
    const int32_t nDestY = (2 * nTargetY) - nCasterY;
    const int32_t nBoltCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x21u;
    missileParams.pOwner      = pUnit;
    missileParams.nMissile    = nMissileId;
    missileParams.nX          = nTargetX;
    missileParams.nY          = nTargetY;
    missileParams.nTargetX    = nDestX;
    missileParams.nTargetY    = nDestY;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.pInitFunc   = CLTSKILL_MissileInit_ChargedBoltFromFirstPoint;

    for (int32_t i = 0; i < nBoltCount; ++i)
    {
        missileParams.pInitArgs = i;
        D2Client_CreateClientMissile_6FAD4B20(&missileParams);
    }

    return 1;
}

// 1.12a: 0x6FAFC510 (gpfnCltDoFunc[18])
int32_t __fastcall CLTSKILL_Do018_GuidedArrow_BoneSpirit(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_GetSkillsTxt(nSkillId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    D2UnitStrc* pTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = pTarget ? 0x20u : 0x420u;
    missileParams.pOwner      = pUnit;
    missileParams.pOrigin     = pUnit;
    missileParams.pTarget     = pTarget;
    missileParams.nMissile    = nMissileId;
    missileParams.nTargetX    = nTargetX;
    missileParams.nTargetY    = nTargetY;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile_6FAD4B20(&missileParams);
    if (pMissile)
    {
        MISSILE_SetTargetX(pMissile, pTarget ? 1 : 2);

        const int32_t nDeltaX = nTargetX - CLTSKILL_GetUnitX(pUnit);
        const int32_t nDeltaY = nTargetY - CLTSKILL_GetUnitY(pUnit);
        const int32_t nPackedDelta = (static_cast<int32_t>(static_cast<int16_t>(nDeltaY)) << 16)
                                   + static_cast<int32_t>(static_cast<int16_t>(nDeltaX));
        MISSILE_SetTargetY(pMissile, nPackedDelta);
    }

    return 1;
}

// 1.12a: 0x6FAFC670 (gpfnCltDoFunc[17])
int32_t __fastcall CLTSKILL_Do017_ParallelSpreadMissiles(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (UNITS_GetWeaponClass(pUnit) != WEAPONCLASS_BOW)
    {
        const int16_t nAltMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
        if (nAltMissileId >= 0)
        {
            nMissileId = nAltMissileId;
        }
    }

    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    const int32_t nCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    int32_t nStepX = nTargetX - CLTSKILL_GetUnitX(pUnit);
    int32_t nStepY = nTargetY - CLTSKILL_GetUnitY(pUnit);

    // Scale up very short vectors (< 4 squared length), then rotate 90 degrees and normalize down to <= 3 squared length
    int32_t nSqLen = nStepX * nStepX + nStepY * nStepY;
    if (nSqLen < 4)
    {
        nStepX *= 4;
        nStepY *= 4;
        nSqLen = nStepX * nStepX + nStepY * nStepY;
    }
    if (nSqLen < 16)
    {
        nStepX *= 2;
        nStepY *= 2;
    }

    int32_t nPerpX = nStepY;
    int32_t nPerpY = -nStepX;
    while (nPerpX * nPerpX + nPerpY * nPerpY > 3)
    {
        nPerpX /= 2;
        nPerpY /= 2;
    }

    int32_t nCurX = nTargetX - ((nPerpX * nCount) / 2);
    int32_t nCurY = nTargetY - ((nPerpY * nCount) / 2);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags        = 0x820u;
    missileParams.pOwner         = pUnit;
    missileParams.pOrigin        = pUnit;
    missileParams.nMissile       = nMissileId;
    missileParams.nSkill         = nSkillId;
    missileParams.nSkillLevel    = nSkillLevel;
    missileParams.nActivateFrame = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[1], nSkillId, nSkillLevel);

    for (int32_t i = 0; i < nCount; ++i)
    {
        missileParams.nTargetX = nCurX;
        missileParams.nTargetY = nCurY;
        D2Client_CreateClientMissile_6FAD4B20(&missileParams);
        nCurX += nPerpX;
        nCurY += nPerpY;
    }

    return 1;
}

// ============================================================================
// Section 2: Modal Confirmation / Death / Expansion UI (0x6FAFC830 .. 0x6FAFD720)
// ============================================================================

// 1.12a: 0x6FAFC830 (ABI: EAX = bUseExpansionText)
void __fastcall MODAL_SetUseExpansionPromptText(BOOL bUseExpansionText)
{
    gbModalUseExpansionPromptText = bUseExpansionText;
}

// 1.12a: 0x6FAFC840
BOOL __stdcall MODAL_OnCancelKeyEscape(SMSGHANDLER_PARAMS* pMsg)
{
    (void)pMsg;
    gbModalCancelAccepted = TRUE;
    return FALSE;
}

// 1.12a: 0x6FAFC850
BOOL __cdecl MODAL_IsOkOrCancelAccepted(void)
{
    return (gbModalOkAccepted != 0 || gbModalCancelAccepted != 0) ? TRUE : FALSE;
}

// 1.12a: 0x6FAFC870
void __stdcall MODAL_DrawOkCancelButtons(int32_t nOkFrame, int32_t nCancelFrame)
{
    D2GfxDataStrc gfxData = {};
    gfxData.pCellFile  = gpOkCancelButtonCell;
    gfxData.nDirection = 0;
    gfxData.nFrame     = static_cast<uint32_t>(nOkFrame);

    TEXTURE_CelDraw(
        &gfxData,
        gnUiOffsetX + 206,
        318 - gnUiOffsetY,
        0xFFFFFFFFu,
        DRAWMODE_NORMAL,
        nullptr);

    gfxData.nFrame = static_cast<uint32_t>(nCancelFrame);
    TEXTURE_CelDraw(
        &gfxData,
        gnUiOffsetX + 338,
        318 - gnUiOffsetY,
        0xFFFFFFFFu,
        DRAWMODE_NORMAL,
        nullptr);
}

// 1.12a: 0x6FAFC900
BOOL __stdcall MODAL_OnOkKeyEscape(SMSGHANDLER_PARAMS* pMsg)
{
    (void)pMsg;
    gbModalOkSingleDismissed = FALSE;
    return FALSE;
}

// 1.12a: 0x6FAFC910
BOOL __cdecl MODAL_IsOkSingleDismissed(void)
{
    return (gbModalOkSingleDismissed != 0) ? TRUE : FALSE;
}

// 1.12a: 0x6FAFC920
void __stdcall MODAL_DrawSingleOkButton(int32_t nFrame)
{
    D2GfxDataStrc gfxData = {};
    gfxData.pCellFile  = gpOkCancelButtonCell;
    gfxData.nDirection = 0;
    gfxData.nFrame     = static_cast<uint32_t>(nFrame);

    TEXTURE_CelDraw(
        &gfxData,
        gnUiOffsetX + 272,
        318 - gnUiOffsetY,
        0xFFFFFFFFu,
        DRAWMODE_NORMAL,
        nullptr);
}

// 1.12a: 0x6FAFC980
void __stdcall MODAL_DrawPopupFrameBackground(BOOL bUsePrimaryPopupBox)
{
    D2GfxDataStrc gfxData = {};
    gfxData.pCellFile  = bUsePrimaryPopupBox ? gpPopupOkBoxCell1 : gpPopupOkBoxCell2;
    gfxData.nDirection = 0;
    gfxData.nFrame     = 0;

    TEXTURE_CelDraw(
        &gfxData,
        gnUiOffsetX + 188,
        328 - gnUiOffsetY,
        0xFFFFFFFFu,
        DRAWMODE_NORMAL,
        nullptr);

    gfxData.nFrame = 1;
    TEXTURE_CelDraw(
        &gfxData,
        gnUiOffsetX + 444,
        328 - gnUiOffsetY,
        0xFFFFFFFFu,
        DRAWMODE_NORMAL,
        nullptr);
}

// 1.12a: 0x6FAFCA20
void __cdecl MODAL_FreePopupCellFiles(void)
{
    if (gpPopupOkBoxCell1)
    {
        D2CMP_CelFileFreeHardware(gpPopupOkBoxCell1);
        FOG_Free(gpPopupOkBoxCell1, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
        gpPopupOkBoxCell1 = nullptr;
    }

    if (gpPopupOkBoxCell2)
    {
        D2CMP_CelFileFreeHardware(gpPopupOkBoxCell2);
        FOG_Free(gpPopupOkBoxCell2, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
        gpPopupOkBoxCell2 = nullptr;
    }

    if (gpOkCancelButtonCell)
    {
        D2CMP_CelFileFreeHardware(gpOkCancelButtonCell);
        FOG_Free(gpOkCancelButtonCell, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
        gpOkCancelButtonCell = nullptr;
    }

    D2Client_ClearUiDirtyState_6FB3A7F0();
}

// 1.12a: 0x6FAFCAC0
BOOL __stdcall MODAL_OnOkCancelKeyReturn(SMSGHANDLER_PARAMS* pMsg)
{
    gbModalOkAccepted          = TRUE;
    gbModalButtonMouseCaptured = FALSE;
    gnInputDragStartX          = 0;
    gnInputDragStartY          = 0;
    gnInputDragEndX            = 0;
    gnInputDragEndY            = 0;
    pMsg->bUseResult           = TRUE;
    pMsg->lResult              = 0;
    return SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FAFCB00
void __cdecl MODAL_DrawOkCancelButtonLabels(void)
{
    const Font ePrevFont = D2Win_10127_SetFont(static_cast<Font>(1));

    const Unicode* wszOk = D2LANG_GetStringFromTblIndex(0x13EE);
    const int32_t nTextY1 = 310 - gnUiOffsetY;
    int32_t nOkX = gnUiOffsetX + 206;
    const int32_t nOkBoxWidth = (gnUiOffsetX + 303) - nOkX;
    const int32_t nOkTextWidth = D2Win_10121_GetTextWidth(wszOk);
    if (nOkTextWidth < nOkBoxWidth)
    {
        nOkX += (nOkBoxWidth - nOkTextWidth) / 2;
    }
    D2Win_10117_DrawText(wszOk, nOkX, nTextY1, 0, FALSE);

    const Unicode* wszCancel = D2LANG_GetStringFromTblIndex(0x13EF);
    const int32_t nTextY2 = 310 - gnUiOffsetY;
    int32_t nCancelX = gnUiOffsetX + 338;
    const int32_t nCancelBoxWidth = (gnUiOffsetX + 435) - nCancelX;
    const int32_t nCancelTextWidth = D2Win_10121_GetTextWidth(wszCancel);
    if (nCancelTextWidth < nCancelBoxWidth)
    {
        nCancelX += (nCancelBoxWidth - nCancelTextWidth) / 2;
    }
    D2Win_10117_DrawText(wszCancel, nCancelX, nTextY2, 0, FALSE);

    D2Win_10127_SetFont(ePrevFont);
}

// 1.12a: 0x6FAFCBC0
void __cdecl MODAL_DrawOkCancelPromptText(void)
{
    const Font ePrevFont = D2Win_10127_SetFont(static_cast<Font>(1));

    const Unicode* wszLine1 = D2LANG_GetStringFromTblIndex(0x1430);
    const int32_t nLine1Y = 190 - gnUiOffsetY;
    int32_t nLine1X = (g_nScreenWidth / 2) - 132;
    const int32_t nSpan1 = (gnUiOffsetX + 453) - nLine1X;
    const int32_t nWidth1 = D2Win_10121_GetTextWidth(wszLine1);
    if (nWidth1 < nSpan1)
    {
        nLine1X += (nSpan1 - nWidth1) / 2;
    }
    D2Win_10117_DrawText(wszLine1, nLine1X, nLine1Y, 0, FALSE);

    const int16_t nLine2TblIdx = gbModalUseExpansionPromptText ? 0x5578 : 0x1432;
    const Unicode* wszLine2 = D2LANG_GetStringFromTblIndex(nLine2TblIdx);
    const int32_t nLine2Y = 205 - gnUiOffsetY;
    int32_t nLine2X = (g_nScreenWidth / 2) - 132;
    const int32_t nSpan2 = (gnUiOffsetX + 453) - nLine2X;
    const int32_t nWidth2 = D2Win_10121_GetTextWidth(wszLine2);
    if (nWidth2 < nSpan2)
    {
        nLine2X += (nSpan2 - nWidth2) / 2;
    }
    D2Win_10117_DrawText(wszLine2, nLine2X, nLine2Y, 0, FALSE);

    D2Win_10127_SetFont(ePrevFont);
}

// 1.12a: 0x6FAFCCB0
BOOL __stdcall MODAL_OnOkKeyReturn(SMSGHANDLER_PARAMS* pMsg)
{
    gbModalOkSingleDismissed   = TRUE;
    gbModalButtonMouseCaptured = FALSE;
    gnInputDragStartX          = 0;
    gnInputDragStartY          = 0;
    gnInputDragEndX            = 0;
    gnInputDragEndY            = 0;
    pMsg->bUseResult           = TRUE;
    pMsg->lResult              = 0;
    return SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FAFCCF0
void __cdecl MODAL_DrawSingleOkButtonLabel(void)
{
    const Font ePrevFont = D2Win_10127_SetFont(static_cast<Font>(1));

    const Unicode* wszOk = D2LANG_GetStringFromTblIndex(0x13EE);
    const int32_t nTextY = 310 - gnUiOffsetY;
    int32_t nOkX = gnUiOffsetX + 272;
    const int32_t nBoxWidth = (gnUiOffsetX + 369) - nOkX;
    const int32_t nTextWidth = D2Win_10121_GetTextWidth(wszOk);
    if (nTextWidth < nBoxWidth)
    {
        nOkX += (nBoxWidth - nTextWidth) / 2;
    }
    D2Win_10117_DrawText(wszOk, nOkX, nTextY, 0, FALSE);

    D2Win_10127_SetFont(ePrevFont);
}

// 1.12a: 0x6FAFCD60
void __cdecl MODAL_DrawSingleOkPromptText(void)
{
    const Font ePrevFont = D2Win_10127_SetFont(static_cast<Font>(1));

    const Unicode* wszLine1 = D2LANG_GetStringFromTblIndex(0x1430);
    const int32_t nLine1Y = 190 - gnUiOffsetY;
    int32_t nLine1X = (g_nScreenWidth / 2) - 132;
    const int32_t nSpan1 = (gnUiOffsetX + 453) - nLine1X;
    const int32_t nWidth1 = D2Win_10121_GetTextWidth(wszLine1);
    if (nWidth1 < nSpan1)
    {
        nLine1X += (nSpan1 - nWidth1) / 2;
    }
    D2Win_10117_DrawText(wszLine1, nLine1X, nLine1Y, 0, FALSE);

    const int16_t nLine2TblIdx = FOG_IsExpansion() ? 0x5578 : 0x1431;
    const Unicode* wszLine2 = D2LANG_GetStringFromTblIndex(nLine2TblIdx);
    const int32_t nLine2Y = 205 - gnUiOffsetY;
    int32_t nLine2X = (g_nScreenWidth / 2) - 132;
    const int32_t nSpan2 = (gnUiOffsetX + 453) - nLine2X;
    const int32_t nWidth2 = D2Win_10121_GetTextWidth(wszLine2);
    if (nWidth2 < nSpan2)
    {
        nLine2X += (nSpan2 - nWidth2) / 2;
    }
    D2Win_10117_DrawText(wszLine2, nLine2X, nLine2Y, 0, FALSE);

    D2Win_10127_SetFont(ePrevFont);
}

// 1.12a: 0x6FAFCE50 (ABI: EAX = nLeft, EDX = nBottom, ESI = nWidth, [esp+4] = nHeight, ret 4)
BOOL __stdcall MODAL_IsMouseInsideButtonRect(int32_t nLeft, int32_t nBottom, int32_t nWidth, int32_t nHeight)
{
    if (g_nMouseX < nLeft || g_nMouseX > (nLeft + nWidth))
    {
        return FALSE;
    }

    const int32_t nTop = nBottom - nHeight;
    if (g_nMouseY < nTop || g_nMouseY > nBottom)
    {
        return FALSE;
    }

    return TRUE;
}

// 1.12a: 0x6FAFCE90
BOOL __stdcall MODAL_OnOkCancelLButtonUp(SMSGHANDLER_PARAMS* pMsg)
{
    const BOOL bWasCaptured    = gbModalButtonMouseCaptured;
    gbModalOkButtonPressed     = FALSE;
    gbModalCancelButtonPressed = FALSE;

    if (bWasCaptured)
    {
        const int32_t nBottom = 310 - gnUiOffsetY;
        const int32_t nTop    = nBottom - 32;
        const int32_t nOkLeft = gnUiOffsetX + 206;

        if (g_nMouseX >= nOkLeft && g_nMouseX <= (nOkLeft + 96) && g_nMouseY >= nTop && g_nMouseY <= nBottom)
        {
            gbModalOkAccepted          = TRUE;
            gbModalButtonMouseCaptured = FALSE;
        }

        const int32_t nCancelLeft = gnUiOffsetX + 338;
        if (g_nMouseX >= nCancelLeft && g_nMouseX <= (nCancelLeft + 96) && g_nMouseY >= nTop && g_nMouseY <= nBottom)
        {
            gbModalCancelAccepted      = TRUE;
            gbModalButtonMouseCaptured = FALSE;
        }
    }
    else
    {
        gnInputDragStartX = 0;
        gnInputDragStartY = 0;
        gnInputDragEndX   = 0;
        gnInputDragEndY   = 0;
    }

    pMsg->lResult    = 0;
    pMsg->bUseResult = TRUE;
    return SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FAFCF70
BOOL __stdcall MODAL_OnOkLButtonUp(SMSGHANDLER_PARAMS* pMsg)
{
    const BOOL bWasCaptured = gbModalButtonMouseCaptured;
    gbModalOkButtonPressed  = FALSE;

    if (bWasCaptured)
    {
        const int32_t nBottom = 310 - gnUiOffsetY;
        const int32_t nTop    = nBottom - 32;
        const int32_t nOkLeft = gnUiOffsetX + 272;

        if (g_nMouseX >= nOkLeft && g_nMouseX <= (nOkLeft + 96) && g_nMouseY >= nTop && g_nMouseY <= nBottom)
        {
            gbModalButtonMouseCaptured = FALSE;
            gbModalOkSingleDismissed   = TRUE;
            pMsg->lResult              = 0;
            pMsg->bUseResult           = TRUE;
            return SMsgBreakHandlerChain(pMsg);
        }
    }

    gnInputDragStartX = 0;
    gnInputDragStartY = 0;
    gnInputDragEndX   = 0;
    gnInputDragEndY   = 0;
    pMsg->lResult     = 0;
    pMsg->bUseResult  = TRUE;
    return SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FAFD020
int32_t __cdecl MODAL_LoadPopupCellFilesAndResetPalette(void)
{
    gpPopupOkBoxCell1    = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Bigmenu\\popupok", 0);
    gpPopupOkBoxCell2    = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Bigmenu\\popupok", 0);
    gpOkCancelButtonCell = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Menu\\okcancelbtn", 0);
    return D2Win_10177(0);
}

// 1.12a: 0x6FAFD060
BOOL __stdcall MODAL_OnOkCancelLButtonDown(SMSGHANDLER_PARAMS* pMsg)
{
    const int32_t nBottom     = 310 - gnUiOffsetY;
    const int32_t nTop        = nBottom - 32;
    const int32_t nOkLeft     = gnUiOffsetX + 206;
    const int32_t nCancelLeft = gnUiOffsetX + 338;

    BOOL bHitButton = FALSE;
    if (g_nMouseX >= nOkLeft && g_nMouseX <= (nOkLeft + 96) && g_nMouseY >= nTop && g_nMouseY <= nBottom)
    {
        gbModalOkButtonPressed = TRUE;
        bHitButton             = TRUE;
    }
    else if (g_nMouseX >= nCancelLeft && g_nMouseX <= (nCancelLeft + 96) && g_nMouseY >= nTop && g_nMouseY <= nBottom)
    {
        gbModalCancelButtonPressed = TRUE;
        bHitButton                 = TRUE;
    }

    if (bHitButton)
    {
        gbModalButtonMouseCaptured = TRUE;
        D2Client_PlaySoundEx_6FAE1FA0(nullptr, 4, 0, 0, 0);
        pMsg->lResult    = 0;
        pMsg->bUseResult = TRUE;
        return SMsgBreakHandlerChain(pMsg);
    }

    gnInputDragStartX = 0;
    gnInputDragStartY = 0;
    gnInputDragEndX   = 0;
    gnInputDragEndY   = 0;
    pMsg->lResult     = 0;
    pMsg->bUseResult  = TRUE;
    return SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FAFD140
BOOL __stdcall MODAL_OnOkLButtonDown(SMSGHANDLER_PARAMS* pMsg)
{
    const int32_t nBottom = 310 - gnUiOffsetY;
    const int32_t nTop    = nBottom - 32;
    const int32_t nOkLeft = gnUiOffsetX + 272;

    if (g_nMouseX >= nOkLeft && g_nMouseX <= (nOkLeft + 96) && g_nMouseY >= nTop && g_nMouseY <= nBottom)
    {
        gbModalOkButtonPressed     = TRUE;
        gbModalButtonMouseCaptured = TRUE;
        D2Client_PlaySoundEx_6FAE1FA0(nullptr, 4, 0, 0, 0);
        pMsg->lResult    = 0;
        pMsg->bUseResult = TRUE;
        return SMsgBreakHandlerChain(pMsg);
    }

    gnInputDragStartX = 0;
    gnInputDragStartY = 0;
    gnInputDragEndX   = 0;
    gnInputDragEndY   = 0;
    pMsg->lResult     = 0;
    pMsg->bUseResult  = TRUE;
    return SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FAFD1F0
void __fastcall MODAL_RenderOkCancelScreen(BOOL bUnused)
{
    (void)bUnused;

    if (WINDOW_GetState() == 0 && D2GFX_StartDraw(1, 0, 0, 0))
    {
        D2GFX_ClearScreen(0);
        D2Win_10179();
        MODAL_DrawPopupFrameBackground(FALSE);
        MODAL_DrawOkCancelPromptText();
        MODAL_DrawOkCancelButtons(gbModalOkButtonPressed, gbModalCancelButtonPressed);
        MODAL_DrawOkCancelButtonLabels();
        D2Client_DrawSoftwareCursor_6FB4F5B0();
        D2GFX_EndScene();
    }
}

// 1.12a: 0x6FAFD250
BOOL __stdcall MODAL_PumpOkCancelFrame(uint32_t dwUnused)
{
    (void)dwUnused;

    D2Client_PumpNetworkAndAudio_6FB299D0();
    MODAL_RenderOkCancelScreen(TRUE);
    D2Client_EndRenderFramePresent_6FB1A660();
    return (gbModalOkAccepted != 0 || gbModalCancelAccepted != 0) ? TRUE : FALSE;
}

// 1.12a: 0x6FAFD290
void __fastcall MODAL_RenderOkScreen(BOOL bUnused)
{
    (void)bUnused;

    if (WINDOW_GetState() == 0 && D2GFX_StartDraw(1, 0, 0, 0))
    {
        D2GFX_ClearScreen(0);
        D2Win_10179();
        MODAL_DrawPopupFrameBackground(TRUE);
        MODAL_DrawSingleOkPromptText();
        MODAL_DrawSingleOkButton(gbModalOkButtonPressed);
        MODAL_DrawSingleOkButtonLabel();
        D2Client_DrawSoftwareCursor_6FB4F5B0();
        D2GFX_EndScene();
    }
}

// 1.12a: 0x6FAFD2E0
BOOL __stdcall MODAL_PumpOkFrame(uint32_t dwUnused)
{
    (void)dwUnused;

    D2Client_PumpNetworkAndAudio_6FB299D0();
    if (WINDOW_GetState() == 0 && D2GFX_StartDraw(1, 0, 0, 0))
    {
        D2GFX_ClearScreen(0);
        D2Win_10179();
        MODAL_DrawPopupFrameBackground(TRUE);
        MODAL_DrawSingleOkPromptText();
        MODAL_DrawSingleOkButton(gbModalOkButtonPressed);
        MODAL_DrawSingleOkButtonLabel();
        D2Client_DrawSoftwareCursor_6FB4F5B0();
        D2GFX_EndScene();
    }
    D2Client_EndRenderFramePresent_6FB1A660();
    return (gbModalOkSingleDismissed != 0) ? TRUE : FALSE;
}

// 1.12a: 0x6FAFD350
BOOL __cdecl MODAL_RunOkCancelDialog(void)
{
    BOOL bResult          = TRUE;
    gbModalCancelAccepted = FALSE;
    gbModalOkAccepted     = FALSE;

    D2Win_10176(0);
    gpPopupOkBoxCell1    = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Bigmenu\\popupok", 0);
    gpPopupOkBoxCell2    = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Bigmenu\\popupok", 0);
    gpOkCancelButtonCell = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Menu\\okcancelbtn", 0);
    D2Win_10177(0);

    gnModalScreenState     = 2;
    gpfnActiveScreenRender = gpfnModalOkCancelRender;

    HWND hWnd = WINDOW_GetWindow();
    D2_ASSERT(hWnd != nullptr);

    for (size_t i = 0; i < 4; ++i)
    {
        D2Client_RegisterStormMsgEntry_6FB1CEC0(
            hWnd,
            g_aOkCancelModalMsgTable[i].nDispatchType,
            g_aOkCancelModalMsgTable[i].uMsgOrKey,
            g_aOkCancelModalMsgTable[i].pfnHandler);
    }

    D2Client_RunModalEventLoop_6FB1CF40(MODAL_PumpOkCancelFrame);

    if (gbModalCancelAccepted != 0)
    {
        bResult = FALSE;
    }

    hWnd = WINDOW_GetWindow();
    D2_ASSERT(hWnd != nullptr);

    for (size_t i = 0; i < 4; ++i)
    {
        D2Client_UnregisterStormMsgEntry_6FB1CE40(
            hWnd,
            g_aOkCancelModalMsgTable[i].nDispatchType,
            g_aOkCancelModalMsgTable[i].uMsgOrKey,
            g_aOkCancelModalMsgTable[i].pfnHandler);
    }

    gnModalScreenState     = 0;
    gpfnActiveScreenRender = gpfnDefaultScreenRender;
    MODAL_FreePopupCellFiles();
    return bResult;
}

// 1.12a: 0x6FAFD4A0
BOOL __cdecl MODAL_RunSingleOkDialogA(void)
{
    gbModalOkSingleDismissed = FALSE;

    D2Win_10176(0);
    gpPopupOkBoxCell1    = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Bigmenu\\popupok", 0);
    gpPopupOkBoxCell2    = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Bigmenu\\popupok", 0);
    gpOkCancelButtonCell = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Menu\\okcancelbtn", 0);
    D2Win_10177(0);

    gnModalScreenState     = 1;
    gpfnActiveScreenRender = gpfnModalOkScreenRender;

    HWND hWnd = WINDOW_GetWindow();
    D2_ASSERT(hWnd != nullptr);

    for (size_t i = 0; i < 4; ++i)
    {
        D2Client_RegisterStormMsgEntry_6FB1CEC0(
            hWnd,
            g_aOkModalMsgTable[i].nDispatchType,
            g_aOkModalMsgTable[i].uMsgOrKey,
            g_aOkModalMsgTable[i].pfnHandler);
    }

    D2Client_RunModalEventLoop_6FB1CF40(MODAL_PumpOkFrame);

    hWnd = WINDOW_GetWindow();
    D2_ASSERT(hWnd != nullptr);

    for (size_t i = 0; i < 4; ++i)
    {
        D2Client_UnregisterStormMsgEntry_6FB1CE40(
            hWnd,
            g_aOkModalMsgTable[i].nDispatchType,
            g_aOkModalMsgTable[i].uMsgOrKey,
            g_aOkModalMsgTable[i].pfnHandler);
    }

    gnModalScreenState     = 0;
    gpfnActiveScreenRender = gpfnDefaultScreenRender;
    MODAL_FreePopupCellFiles();
    return TRUE;
}

// 1.12a: 0x6FAFD5E0
BOOL __cdecl MODAL_RunSingleOkDialogB(void)
{
    gbModalOkSingleDismissed = FALSE;

    D2Win_10176(0);
    gpPopupOkBoxCell1    = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Bigmenu\\popupok", 0);
    gpPopupOkBoxCell2    = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Bigmenu\\popupok", 0);
    gpOkCancelButtonCell = D2Client_LoadUiCelArchive_6FB6EF70("Data\\Global\\UI\\Menu\\okcancelbtn", 0);
    D2Win_10177(0);

    gnModalScreenState     = 1;
    gpfnActiveScreenRender = gpfnModalOkScreenRender;

    HWND hWnd = WINDOW_GetWindow();
    D2_ASSERT(hWnd != nullptr);

    for (size_t i = 0; i < 4; ++i)
    {
        D2Client_RegisterStormMsgEntry_6FB1CEC0(
            hWnd,
            g_aOkModalMsgTable[i].nDispatchType,
            g_aOkModalMsgTable[i].uMsgOrKey,
            g_aOkModalMsgTable[i].pfnHandler);
    }

    D2Client_RunModalEventLoop_6FB1CF40(MODAL_PumpOkFrame);

    hWnd = WINDOW_GetWindow();
    D2_ASSERT(hWnd != nullptr);

    for (size_t i = 0; i < 4; ++i)
    {
        D2Client_UnregisterStormMsgEntry_6FB1CE40(
            hWnd,
            g_aOkModalMsgTable[i].nDispatchType,
            g_aOkModalMsgTable[i].uMsgOrKey,
            g_aOkModalMsgTable[i].pfnHandler);
    }

    gnModalScreenState     = 0;
    gpfnActiveScreenRender = gpfnDefaultScreenRender;
    MODAL_FreePopupCellFiles();
    return TRUE;
}

// 1.12a: 0x6FAFD720
void __cdecl MODAL_NoOpCallback(void)
{
}

// ============================================================================
// Section 3: Core Client Skill Engine & Aura Target Search
//            (0x6FAFD730 .. 0x6FB00419)
// ============================================================================

// 1.12a: 0x6FAFD730
int32_t __fastcall CLTSKILL_GetMeleeRangePlus4(D2UnitStrc* pUnit)
{
    return UNITS_GetMeleeRange(pUnit) + 4;
}

// 1.12a: 0x6FAFD740
void __fastcall CLTSKILL_RollBackSeqFrameByPercent(D2UnitStrc* pUnit, int32_t nRollbackPct)
{
    const int32_t nKeepPct   = 100 - nRollbackPct;
    const int32_t nWholeFrm  = pUnit->nSeqCurrentFramePrecise >> 8;
    const int32_t nNewWhole  = (nWholeFrm * nKeepPct) / 100;
    pUnit->nSeqCurrentFramePrecise = nNewWhole << 8;
}

// 1.12a: 0x6FAFD770
int32_t __fastcall CLTSKILL_GetDefaultBowOrCrossbowMissileId(D2UnitStrc* pUnit)
{
    int nWeaponClassId = 0;
    COMPOSIT_GetWeaponClassId(pUnit, pUnit->pInventory, &nWeaponClassId, -1, TRUE);
    return (nWeaponClassId == WEAPONCLASS_XBW) ? 31 : 0;
}

// 1.12a: 0x6FAFD7A0
void __fastcall CLTSKILL_SetUnitPathDirection(D2UnitStrc* pUnit, int32_t nDirection)
{
    PATH_SetDirection(pUnit->pDynamicPath, static_cast<uint8_t>(nDirection));
}

// 1.12a: 0x6FAFD7B0
void __fastcall CLTSKILL_ResolveDualWieldNormalAttackToLeftHandSwing(D2UnitStrc* pUnit, D2SkillStartRequestStrc* pReq)
{
    if (pReq->nSkillId != 0)
    {
        return;
    }

    D2InventoryStrc* pInventory = pUnit->pInventory;

    D2UnitStrc* pRightItem = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM);
    if (pRightItem && (!ITEMS_CanBeEquipped(pRightItem) || ITEMS_CheckIfThrowable(pRightItem) == 0x26))
    {
        pRightItem = nullptr;
    }

    D2UnitStrc* pLeftItem = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM);
    if (pLeftItem && (!ITEMS_CanBeEquipped(pLeftItem) || ITEMS_CheckIfThrowable(pLeftItem) == 0x26))
    {
        pLeftItem = nullptr;
    }

    if (!ITEMS_CheckItemTypeId(pRightItem, 0x2D) || !ITEMS_CheckItemTypeId(pLeftItem, 0x2D))
    {
        return;
    }

    D2SkillStrc* pAttackSkill = SKILLS_GetSkillById(pUnit, 0, D2UnitInvalidGUID);
    if (SKILLS_GetParam4(pAttackSkill) == 0)
    {
        pReq->nSkillId = 5;
        SKILLS_SetParam4(pAttackSkill, 5);
        D2SkillStrc* pLeftSwingSkill = SKILLS_GetSkillById(pUnit, 5, D2UnitInvalidGUID);
        UNITS_SetUsedSkill(pUnit, pLeftSwingSkill);
    }
    else
    {
        SKILLS_SetParam4(pAttackSkill, 0);
        UNITS_SetUsedSkill(pUnit, pAttackSkill);
    }
}

// 1.12a: 0x6FAFD880 (gpfnCltStFunc[4])
int32_t __fastcall CLTSKILL_St04_Arrow_Bolt(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)pUnit;
    (void)nSkillId;
    (void)nSkillLevel;
    return 1;
}

// 1.12a: 0x6FAFD890
int32_t __fastcall CLTSKILL_AuraCallback_CountTarget(D2AuraCallbackCtxStrc* pCtx, D2UnitStrc* pTargetUnit)
{
    (void)pTargetUnit;
    int32_t* pCounter = static_cast<int32_t*>(pCtx->pUserData);
    ++(*pCounter);
    return 1;
}

// 1.12a: 0x6FAFD8A0
BOOL __fastcall CLTSKILL_DoesCircleOverlapRoom(D2ActiveRoomStrc* pRoom, int32_t nCenterX, int32_t nCenterY, int32_t nRadius)
{
    D2DrlgCoordsStrc roomCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &roomCoords);

    if (nCenterX + nRadius >= roomCoords.nSubtileX)
    {
        if (nCenterY + nRadius >= roomCoords.nSubtileY)
        {
            return TRUE;
        }
        return (nCenterY - nRadius <= roomCoords.nSubtileY + roomCoords.nSubtileHeight) ? TRUE : FALSE;
    }

    if (nCenterX - nRadius > roomCoords.nSubtileX + roomCoords.nSubtileWidth)
    {
        return FALSE;
    }

    if (nCenterY + nRadius >= roomCoords.nSubtileY)
    {
        return TRUE;
    }

    return (nCenterY - nRadius <= roomCoords.nSubtileY + roomCoords.nSubtileHeight) ? TRUE : FALSE;
}

// 1.12a: 0x6FAFD910
void __fastcall CLTSKILL_RotatePerpendicularUnitStep(int32_t* pInOutX, int32_t* pInOutY)
{
    int32_t nPerpY = -(*pInOutX);
    int32_t nPerpX = *pInOutY;

    while (nPerpY * nPerpY + nPerpX * nPerpX > 3)
    {
        nPerpX /= 2;
        nPerpY /= 2;
    }

    *pInOutX = nPerpX;
    *pInOutY = nPerpY;
}

// 1.12a: 0x6FAFD950
void __fastcall CLTSKILL_ScaleShortDirectionVector(int32_t* pInOutX, int32_t* pInOutY)
{
    int32_t nX = *pInOutX;
    int32_t nY = *pInOutY;
    int32_t nSqLen = nY * nY + nX * nX;

    if (nSqLen < 4)
    {
        nX *= 4;
        nY *= 4;
        *pInOutX = nX;
        *pInOutY = nY;
        nSqLen = nY * nY + nX * nX;
    }

    if (nSqLen < 16)
    {
        *pInOutX = nX * 2;
        *pInOutY *= 2;
    }
}

// 1.12a: 0x6FAFD990
void __fastcall CLTSKILL_PostTempAttackRateStatList(D2UnitStrc* pUnit, int32_t nAttackRateValue)
{
    const int32_t nUnitType    = pUnit ? static_cast<int32_t>(pUnit->dwUnitType) : 6;
    const D2UnitGUID dwUnitGUID = pUnit ? pUnit->dwUnitId : D2UnitInvalidGUID;

    D2StatListStrc* pStatList = STATLIST_AllocStatList(nullptr, 0x04u, 0u, nUnitType, dwUnitGUID);
    if (pStatList)
    {
        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, TRUE);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_ATTACKRATE, nAttackRateValue, 0);
        UNITS_UpdateAnimRateAndVelocity(pUnit, "..\\Source\\D2Client\\SKILLS\\Skills.cpp", 2532);
    }
}

// 1.12a: 0x6FAFD9F0
void __stdcall CLTSKILL_ProcessActiveStatesDoFuncs(D2UnitStrc* pUnit)
{
    if (!STATES_CheckStateMaskActiveOnUnit(pUnit) || !sgptDataTables)
    {
        return;
    }

    for (int32_t nIdx = 0; nIdx < static_cast<int16_t>(sgptDataTables->nActionStates); ++nIdx)
    {
        const int32_t nStateId = sgptDataTables->pActionStates[nIdx];
        if (!STATES_CheckState(pUnit, nStateId))
        {
            continue;
        }

        D2StatesTxt* pStatesTxt = CLTSKILL_GetStatesTxt(nStateId);
        if (!pStatesTxt)
        {
            continue;
        }

        const int16_t nActiveFuncIdx = static_cast<int16_t>(pStatesTxt->wCltActiveFunc);
        if (nActiveFuncIdx < 0 || nActiveFuncIdx >= gnCltDoFuncs)
        {
            continue;
        }

        if (!gpfnCltDoFunc[nActiveFuncIdx])
        {
            continue;
        }

        int32_t nSkillId    = 0;
        int32_t nSkillLevel = 1;
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nStateId);
        if (pStatList)
        {
            nSkillId    = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);
            nSkillLevel = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0);
        }

        const int16_t nDispatchIdx = static_cast<int16_t>(pStatesTxt->wCltActiveFunc);
        gpfnCltDoFunc[nDispatchIdx](pUnit, nSkillId, nSkillLevel);
    }
}

// 1.12a: 0x6FAFDB00
int32_t __fastcall CLTSKILL_AuraCallback_SelectNextRingTarget(D2AuraCallbackCtxStrc* pCtx, D2UnitStrc* pTargetUnit)
{
    D2AuraRingTargetTrackerStrc* pTracker = static_cast<D2AuraRingTargetTrackerStrc*>(pCtx->pUserData);
    const uint32_t dwCandidateGUID = pTargetUnit ? pTargetUnit->dwUnitId : D2UnitInvalidGUID;

    if (dwCandidateGUID > pTracker->dwPivotGUID && dwCandidateGUID < pTracker->dwNextBestGUID)
    {
        pTracker->dwNextBestGUID = dwCandidateGUID;
        pTracker->pNextBestUnit  = pTargetUnit;
        ++pTracker->nMatchCount;
        return 1;
    }

    if (dwCandidateGUID <= pTracker->dwWrapMinGUID)
    {
        pTracker->dwWrapMinGUID = dwCandidateGUID;
        pTracker->pWrapMinUnit  = pTargetUnit;
    }

    ++pTracker->nMatchCount;
    return 1;
}

// 1.12a: 0x6FAFDB40
int32_t __fastcall CLTSKILL_CheckAmmoAvailable(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 0;
    }

    if (pUnit->dwUnitType != UNIT_PLAYER)
    {
        return 1;
    }

    D2UnitStrc* pActiveWeapon = D2Common_10434(pUnit, FALSE);
    BOOL bMustMatchActiveWeapon = TRUE;

    if (!pActiveWeapon)
    {
        return 1;
    }

    if (ITEMS_GetBaseCode(pActiveWeapon) != 0)
    {
        bMustMatchActiveWeapon = FALSE;
        D2SkillStrc* pUsedSkill = UNITS_GetUsedSkill(pUnit);
        if (SKILLS_GetSkillIdFromSkill(pUsedSkill, "..\\Source\\D2Client\\SKILLS\\Skills.cpp", 182) == 0)
        {
            if (STATLIST_UnitGetItemStatOrSkillStatValue(pActiveWeapon, STAT_ITEM_MAGICARROW, 0) != 0)
            {
                return 1;
            }
        }
    }

    D2InventoryStrc* pInventory = pUnit->pInventory;
    if (!pInventory)
    {
        return 0;
    }

    D2UnitStrc* pRightHand = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM);
    if (pRightHand && (!bMustMatchActiveWeapon || pRightHand == pActiveWeapon))
    {
        if (ITEMS_CheckIfSocketable(pRightHand)
            || ITEMS_CheckIfStackable(pRightHand)
            || STATLIST_UnitGetItemStatOrSkillStatValue(pRightHand, STAT_ITEM_THROWABLE, 0) != 0)
        {
            return ITEMS_CanItemBeUsedForThrowSkill(pRightHand);
        }
    }

    D2UnitStrc* pLeftHand = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM);
    if (pLeftHand && (!bMustMatchActiveWeapon || pLeftHand == pActiveWeapon))
    {
        if (ITEMS_CheckIfSocketable(pLeftHand)
            || ITEMS_CheckIfStackable(pLeftHand)
            || STATLIST_UnitGetItemStatOrSkillStatValue(pLeftHand, STAT_ITEM_THROWABLE, 0) != 0)
        {
            return ITEMS_CanItemBeUsedForThrowSkill(pLeftHand);
        }
    }

    return 0;
}

// 1.12a: 0x6FAFDC40
void __fastcall CLTSKILL_GetUnitForwardStepCoords(D2UnitStrc* pUnit, int32_t* pOutX, int32_t* pOutY)
{
    static const int32_t s_anOctantStepX[8] = {  0, -1, -2, -1,  0,  1,  2,  1 };
    static const int32_t s_anOctantStepY[8] = {  2,  1,  0, -1, -2, -1,  0,  1 };

    const uint32_t nOctant = (static_cast<uint32_t>(UNITS_GetDirection(pUnit)) >> 3) & 7u;
    *pOutX = CLTSKILL_GetUnitX(pUnit) + s_anOctantStepX[nOctant];
    *pOutY = CLTSKILL_GetUnitY(pUnit) + s_anOctantStepY[nOctant];
}

// 1.12a: 0x6FAFDD50
int32_t __fastcall CLTSKILL_DispatchClientSkillEvent(
    int32_t nEventFuncId,
    D2UnitStrc* pUnit,
    int32_t nEventType,
    int32_t nArg1,
    D2UnitStrc* pSourceUnit,
    int32_t nSkillId)
{
    if (nEventFuncId < 0 || nEventFuncId >= 2)
    {
        return 0;
    }

    D2CltUnitEventFunc_t pfnEventFunc = g_apfnCltUnitEventFuncs[nEventFuncId];
    if (!pfnEventFunc)
    {
        return 0;
    }

    return D2Client_DispatchUnitEventList_6FB6A300(pUnit, nEventType, nArg1, pfnEventFunc, pSourceUnit, nSkillId);
}

// 1.12a: 0x6FAFDD90
void __fastcall CLTSKILL_ApplyItemSkillBonusToEnhanceableSkills(
    D2UnitStrc* pUnit,
    int32_t nBonusAmount,
    int32_t nElemTypeFilter,
    uint8_t nSkillTreeFilter)
{
    D2_ASSERT(pUnit != nullptr);
    D2_ASSERT(nBonusAmount != 0);
    D2_ASSERT(pUnit->pSkills != nullptr);

    D2SkillStrc* pSkill = SKILLS_GetFirstSkillFromSkillList(pUnit->pSkills);
    while (pSkill)
    {
        D2SkillStrc* pNextSkill = SKILLS_GetNextSkill(pSkill);
        const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, "..\\Source\\D2Client\\SKILLS\\Skills.cpp", 2419);

        D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
        D2SkillDescTxt* pSkillDescTxt = pSkillsTxt ? CLTSKILL_GetSkillDescTxtFromSkill(nSkillId) : nullptr;
        if (pSkillsTxt && pSkillDescTxt)
        {
            if (SKILLS_GetSkillLevel(pUnit, pSkill, TRUE) > 0 && SKILLS_IsEnhanceable(nSkillId))
            {
                if (nElemTypeFilter == 0 || static_cast<int32_t>(pSkillsTxt->nEType) == nElemTypeFilter)
                {
                    if (nSkillTreeFilter == 4
                        || static_cast<int32_t>(static_cast<int8_t>(pSkillDescTxt->nSkillPage)) == static_cast<int32_t>(nSkillTreeFilter) + 1)
                    {
                        if (SKILLS_GetOwnerGUIDFromSkill(pSkill) == D2UnitInvalidGUID)
                        {
                            D2Common_11031(pUnit, nSkillId, nBonusAmount);
                            D2SkillStrc* pLookupSkill = SKILLS_GetSkillById(pUnit, nSkillId, D2UnitInvalidGUID);
                            if (pLookupSkill && SKILLS_GetSkillLevel(pUnit, pLookupSkill, TRUE) == 0)
                            {
                                SKILLS_RemoveSkill(pUnit, nSkillId, "..\\Source\\D2Client\\SKILLS\\Skills.cpp", 2464);
                            }
                        }
                    }
                }
            }
        }

        pSkill = pNextSkill;
    }

    SKILLS_RefreshPassiveSkills(pUnit);
}

// 1.12a: 0x6FAFDF00
void __fastcall CLTSKILL_Spawn64DirectionNovaRing(D2MissileStrc* pPrimaryParams, D2MissileStrc* pAltParams)
{
    if (pAltParams)
    {
        for (int32_t i = 0; i < 64; ++i)
        {
            const uint32_t dwRand = static_cast<uint32_t>(SEED_RollRandomNumber(&pPrimaryParams->pOwner->pSeed));
            const int32_t nOffsetX = g_anNovaDirOffsetY_30[i];
            const int32_t nOffsetY = g_anNovaDirOffsetX_30[i];

            if ((dwRand % 6u) != 0)
            {
                pPrimaryParams->nTargetX = nOffsetX;
                pPrimaryParams->nTargetY = nOffsetY;
                D2Client_CreateClientMissile_6FAD4B20(pPrimaryParams);
            }
            else
            {
                pAltParams->nTargetX = nOffsetX;
                pAltParams->nTargetY = nOffsetY;
                D2Client_CreateClientMissile_6FAD4B20(pAltParams);
            }
        }
    }
    else
    {
        for (int32_t i = 0; i < 64; ++i)
        {
            pPrimaryParams->nTargetX = g_anNovaDirOffsetY_30[i];
            pPrimaryParams->nTargetY = g_anNovaDirOffsetX_30[i];
            if (pPrimaryParams->nMissile == 176)
            {
                pPrimaryParams->dwFlags ^= 0x4000u;
            }
            D2Client_CreateClientMissile_6FAD4B20(pPrimaryParams);
        }
    }
}

// 1.12a: 0x6FAFDFC0 (gpfnCltStFunc[3])
int32_t __fastcall CLTSKILL_St03_Unsummon(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget || pTarget->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    const D2UnitGUID dwPetGUID = pTarget->dwUnitId;
    D2UnitGUID dwOwnerGUID = D2UnitInvalidGUID;
    for (D2RosterPetStrc* pPet = gpRosterPetList; pPet != nullptr; pPet = pPet->pNext)
    {
        if (pPet->dwUnitGUID == dwPetGUID)
        {
            dwOwnerGUID = pPet->dwOwnerGUID;
            break;
        }
    }

    if (dwOwnerGUID != pUnit->dwUnitId)
    {
        return 0;
    }

    const int32_t nPetType = D2Client_GetRosterPetTypeByGUID_6FB73130(dwPetGUID);
    D2PetTypeTxt* pPetTypeTxt = CLTSKILL_GetPetTypeTxt(nPetType);
    if (!pPetTypeTxt)
    {
        return 0;
    }

    return (pPetTypeTxt->dwPetFlags & gdwBitMasks[PETTYPEFLAGINDEX_UNSUMMON]) ? 1 : 0;
}

// 1.12a: 0x6FAFE070 (gpfnCltStFunc[2])
int32_t __fastcall CLTSKILL_St02_Throw(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (!CLTSKILL_CheckAmmoAvailable(pUnit))
    {
        return 0;
    }

    return (D2Common_10434(pUnit, FALSE) != nullptr) ? 1 : 0;
}

// 1.12a: 0x6FAFE0A0 (gpfnCltStFunc[1])
int32_t __fastcall CLTSKILL_St01_Attack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    if (COMPOSIT_IsWeaponBowOrXBow(pUnit))
    {
        return CLTSKILL_CheckAmmoAvailable(pUnit) ? 1 : 0;
    }

    D2UnitStrc* pActiveWeapon = D2Common_10434(pUnit, FALSE);
    if (pActiveWeapon && ITEMS_CheckItemTypeId(pActiveWeapon, 0x26))
    {
        return 0;
    }

    return 1;
}

// 1.12a: 0x6FAFE0E0
BOOL __fastcall CLTSKILL_ValidateTargetCorpseOrSelectability(D2UnitStrc* pUnit, int32_t nSkillId)
{
    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return TRUE;
    }

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return FALSE;
    }

    if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_SEARCHENEMYXY) != 0
        && pTarget->dwUnitType == UNIT_MONSTER
        && pTarget->dwAnimMode == MONMODE_DEAD)
    {
        return (pSkillsTxt->dwFlags[0] & SKILLSFLAG_TARGETCORPSE) ? TRUE : FALSE;
    }

    return TRUE;
}

// 1.12a: 0x6FAFE180
int32_t __fastcall CLTSKILL_GetPlayerAttackAnimMode(D2UnitStrc* pUnit)
{
    int32_t nAnimMode = PLRMODE_ATTACK1;

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    BOOL bRandomizeSwing = TRUE;

    if (pTarget && pTarget->dwUnitType == UNIT_MONSTER)
    {
        D2MonStats2Txt* pMonStats2Txt = CLTSKILL_GetMonStats2TxtFromMonsterId(pTarget->dwClassId);
        if (pMonStats2Txt)
        {
            if (pMonStats2Txt->nHeight == 2)
            {
                nAnimMode       = PLRMODE_ATTACK2;
                bRandomizeSwing = FALSE;
            }
            if (pMonStats2Txt->nHeight == 1)
            {
                bRandomizeSwing = FALSE;
            }
        }
    }

    if (COMPOSIT_IsWeaponBowOrXBow(pUnit))
    {
        nAnimMode       = PLRMODE_ATTACK1;
        bRandomizeSwing = FALSE;
    }

    D2UnitStrc* pEquippedWeapon = nullptr;
    int nBodyLoc                = 0;
    BOOL bIsLeftHand            = FALSE;
    if (!INVENTORY_GetEquippedWeapon(pUnit->pInventory, &pEquippedWeapon, &nBodyLoc, &bIsLeftHand))
    {
        return PLRMODE_ATTACK1;
    }

    if (bRandomizeSwing)
    {
        const uint32_t dwRoll = static_cast<uint32_t>(SEED_RollRandomNumber(&pUnit->pSeed)) & 0x3Fu;
        if (dwRoll < 0x20u)
        {
            nAnimMode = PLRMODE_ATTACK2;
        }
    }

    return nAnimMode;
}

// 1.12a: 0x6FAFE260
void __fastcall CLTSKILL_AttachMissileLightMap(int32_t nMissileId, D2UnitStrc* pUnit)
{
    D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxt(nMissileId);
    if (!pMissilesTxt || pMissilesTxt->nLight == 0)
    {
        return;
    }

    D2ClientLightMapStrc* pLightMap = D2Client_AllocClientLightMap_6FB0EAF0(
        1,
        0xFF,
        pMissilesTxt->nRGB[0],
        pMissilesTxt->nRGB[1],
        pMissilesTxt->nRGB[2]);

    int32_t nLight = pMissilesTxt->nLight;
    if (nLight > 0)
    {
        if (nLight >= 18)
        {
            nLight = 18;
        }
        D2_ASSERT(pLightMap != nullptr);
        pLightMap->nRadiusSubtiles = nLight << 3;
    }

    D2ClientLightMapStrc* pPrevLightMap = static_cast<D2ClientLightMapStrc*>(D2Common_10958(pUnit, pLightMap));
    if (pPrevLightMap)
    {
        D2Client_FreeClientLightMap_6FB0E500(pPrevLightMap);
    }
}

// 1.12a: 0x6FAFE320
int32_t __fastcall CLTSKILL_ResolveTargetOrDirectionCoords(D2UnitStrc* pUnit, int32_t* pOutX, int32_t* pOutY)
{
    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (pTarget)
    {
        *pOutX = CLTSKILL_GetUnitX(pTarget);
        *pOutY = CLTSKILL_GetUnitY(pTarget);
    }
    else
    {
        *pOutX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
        *pOutY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
    }

    if (*pOutX == 0 || *pOutY == 0)
    {
        CLTSKILL_GetUnitForwardStepCoords(pUnit, pOutX, pOutY);
    }

    return 1;
}

// 1.12a: 0x6FAFE3F0
BOOL __fastcall CLTSKILL_CanSpawnMissileAtTargetCoords(int32_t nMissileId, D2UnitStrc* pUnit)
{
    D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxt(nMissileId);
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    D2_ASSERT(pMissilesTxt != nullptr);

    if (!pRoom)
    {
        return FALSE;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY);
    if (nTargetX == 0 || nTargetY == 0)
    {
        return FALSE;
    }

    const uint32_t nSize = pMissilesTxt->nSize;
    return (COLLISION_CheckMaskWithSizeXY(pRoom, nTargetX, nTargetY, nSize, nSize, 0x05u) == 0) ? TRUE : FALSE;
}

// 1.12a: 0x6FAFE4B0
void __fastcall CLTSKILL_SpawnNovaMissilesFromUnit(
    D2UnitStrc* pOriginUnit,
    int32_t nVelOverride,
    D2UnitStrc* pOwnerUnit,
    int32_t nPrimaryMissileId,
    int32_t nAltMissileId,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    D2MissileStrc primaryParams = {};
    primaryParams.dwFlags     = 0x03u;
    primaryParams.nX          = CLTSKILL_GetUnitX(pOriginUnit);
    primaryParams.nY          = CLTSKILL_GetUnitY(pOriginUnit);
    primaryParams.pOwner      = pOwnerUnit;
    primaryParams.nMissile    = nPrimaryMissileId;
    primaryParams.nSkill      = nSkillId;
    primaryParams.nSkillLevel = nSkillLevel;

    if (nVelOverride != 0)
    {
        primaryParams.dwFlags  |= 0x04u;
        primaryParams.nVelocity = nVelOverride;
    }

    if (nAltMissileId == -1)
    {
        CLTSKILL_Spawn64DirectionNovaRing(&primaryParams, nullptr);
    }
    else
    {
        D2MissileStrc altParams = primaryParams;
        altParams.nMissile      = nAltMissileId;
        CLTSKILL_Spawn64DirectionNovaRing(&primaryParams, &altParams);
    }
}

// 1.12a: 0x6FAFE5A0
BOOL __fastcall CLTSKILL_AreUnitsAlliedOrSameParty(D2UnitStrc* pTargetUnit, D2UnitStrc* pCasterUnit)
{
    D2UnitGUID dwTargetPlayerGUID = D2UnitInvalidGUID;
    if (pTargetUnit)
    {
        dwTargetPlayerGUID = pTargetUnit->dwUnitId;
        if (pTargetUnit->dwUnitType == UNIT_MONSTER)
        {
            if (!gpRosterPetList)
            {
                return FALSE;
            }

            BOOL bFoundPet = FALSE;
            for (D2RosterPetStrc* pPet = gpRosterPetList; pPet != nullptr; pPet = pPet->pNext)
            {
                if (pPet->dwUnitGUID == dwTargetPlayerGUID)
                {
                    dwTargetPlayerGUID = pPet->dwOwnerGUID;
                    bFoundPet = TRUE;
                    break;
                }
            }

            if (!bFoundPet || dwTargetPlayerGUID == D2UnitInvalidGUID)
            {
                return FALSE;
            }
        }
    }

    const D2UnitGUID dwCasterGUID = pCasterUnit ? pCasterUnit->dwUnitId : D2UnitInvalidGUID;
    D2RosterUnitPartialStrc* pCasterRoster = D2Client_FindRosterPlayerByGUID_6FB281C0(dwCasterGUID);
    D2RosterUnitPartialStrc* pTargetRoster = D2Client_FindRosterPlayerByGUID_6FB281C0(dwTargetPlayerGUID);

    if (!pCasterRoster || !pTargetRoster)
    {
        return FALSE;
    }

    if (pCasterRoster->wPartyId != pTargetRoster->wPartyId || pCasterRoster->wPartyId == 0xFFFFu)
    {
        return FALSE;
    }

    return TRUE;
}

// 1.12a: 0x6FAFE620
void __fastcall CLTSKILL_ScaleLobMissileAnimToTargetDistance(D2UnitStrc* pCaster, D2UnitStrc* pMissile)
{
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pCaster, &nTargetX, &nTargetY))
    {
        return;
    }

    int32_t nDist = static_cast<int32_t>(UNITS_GetDistanceToCoordinates(pCaster, nTargetX, nTargetY));
    if (nDist < 1)
    {
        nDist = 1;
    }

    UNITS_SetAnimationSpeed(pMissile, 0xA00 / nDist);
    MISSILE_SetCurrentFrame(pMissile, nDist + 2);
    pMissile->nSeqCurrentFramePrecise = 0x200;
}

// 1.12a: 0x6FAFE690
D2UnitStrc* __fastcall CLTSKILL_SpawnTargetedMissileWithin100Subtiles(
    D2UnitStrc* pUnit,
    int32_t nMissileId,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return nullptr;
    }

    if (nTargetX == 0 || nTargetY == 0)
    {
        return nullptr;
    }

    const int32_t nSqDist = PATH_ComputeSquaredDistance(
        CLTSKILL_GetUnitX(pUnit),
        CLTSKILL_GetUnitY(pUnit),
        nTargetX,
        nTargetY);
    if (nSqDist > 10000)
    {
        return nullptr;
    }

    return D2Client_SpawnTargetedLinearMissile_6FAD1940(
        pUnit,
        nMissileId,
        nTargetX,
        nTargetY,
        nSkillId,
        nSkillLevel);
}

// 1.12a: 0x6FAFE710
D2UnitStrc* __fastcall CLTSKILL_SpawnLinearMissileToResolvedTarget(
    D2UnitStrc* pUnit,
    int32_t nMissileId,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return nullptr;
    }

    if (nTargetX == 0 || nTargetY == 0)
    {
        return nullptr;
    }

    return D2Client_SpawnLinearSkillMissile_6FAD1E80(
        pUnit,
        nMissileId,
        nTargetX,
        nTargetY,
        nSkillId,
        nSkillLevel);
}

// 1.12a: 0x6FAFE770
BOOL __fastcall CLTSKILL_CheckAuraFilter(D2UnitStrc* pTarget, uint32_t dwAuraFilter, D2UnitStrc* pCaster)
{
    if (!pTarget)
    {
        return FALSE;
    }

    switch (pTarget->dwUnitType)
    {
    case UNIT_PLAYER:
        if ((dwAuraFilter & 0x00000001u) == 0)
        {
            return FALSE;
        }
        if ((dwAuraFilter & 0x00001000u) != 0)
        {
            if (pTarget->dwAnimMode != PLRMODE_DEAD)
            {
                return FALSE;
            }
        }
        else
        {
            if (pTarget->dwAnimMode == PLRMODE_DEAD || pTarget->dwAnimMode == PLRMODE_DEATH)
            {
                return FALSE;
            }
        }
        break;

    case UNIT_MONSTER:
        if ((dwAuraFilter & 0x00000002u) == 0)
        {
            return FALSE;
        }
        if ((dwAuraFilter & 0x00001000u) != 0)
        {
            if (pTarget->dwAnimMode != MONMODE_DEAD)
            {
                return FALSE;
            }
        }
        else
        {
            if (pTarget->dwAnimMode == MONMODE_DEAD || pTarget->dwAnimMode == MONMODE_DEATH)
            {
                return FALSE;
            }
        }
        if ((dwAuraFilter & 0x00000004u) != 0 && !MONSTERS_IsUndead(pTarget))
        {
            return FALSE;
        }
        if ((dwAuraFilter & 0x00004000u) != 0 && MONSTERS_IsBoss(nullptr, pTarget))
        {
            return FALSE;
        }
        if ((dwAuraFilter & 0x00040000u) != 0 && MONSTERS_IsPrimeEvil(pTarget))
        {
            return FALSE;
        }
        break;

    case UNIT_OBJECT:
        if ((dwAuraFilter & 0x00000010u) == 0)
        {
            return FALSE;
        }
        break;

    case UNIT_MISSILE:
    {
        if ((dwAuraFilter & 0x00000008u) == 0)
        {
            return FALSE;
        }
        D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxt(pTarget->dwClassId);
        if (!pMissilesTxt || (pMissilesTxt->dwMissileFlags & MISSILESFLAG_EXPLOSION) != 0)
        {
            return FALSE;
        }
        break;
    }

    case UNIT_ITEM:
        if ((dwAuraFilter & 0x00000020u) == 0)
        {
            return FALSE;
        }
        break;

    default:
        return FALSE;
    }

    if ((dwAuraFilter & 0x00000080u) != 0 && (pTarget->dwFlags & UNITFLAG_CANBEATTACKED) == 0)
    {
        return FALSE;
    }

    if ((dwAuraFilter & 0x00000400u) != 0 && (pTarget->dwFlags & UNITFLAG_ISVALIDTARGET) == 0)
    {
        return FALSE;
    }

    if ((dwAuraFilter & 0x00000100u) != 0 && DUNGEON_IsRoomInTown(UNITS_GetRoom(pTarget)))
    {
        return FALSE;
    }

    if ((dwAuraFilter & 0x00010000u) != 0)
    {
        if (!CLTSKILL_AreUnitsAlliedOrSameParty(pTarget, pCaster))
        {
            return FALSE;
        }

        if (pTarget->dwUnitType == UNIT_MONSTER)
        {
            D2MonStatsTxt* pMonStatsTxt = DATATBLS_GetMonStatsTxtRecord(pTarget->dwClassId);
            if (!pMonStatsTxt || (pMonStatsTxt->dwMonStatsFlags & MONSTATSFLAG_NPC) != 0)
            {
                return FALSE;
            }
        }
    }

    if ((dwAuraFilter & 0x00008000u) != 0 && !D2Client_CanUnitAttackTarget_6FACF980(pTarget, pCaster))
    {
        return FALSE;
    }

    if ((dwAuraFilter & 0x00020000u) != 0 && !UNITS_IsInMeleeRange(pCaster, pTarget, 0))
    {
        return FALSE;
    }

    if ((dwAuraFilter & 0x00080000u) != 0 && STATES_CheckState(pTarget, 0x56))
    {
        return FALSE;
    }

    if ((dwAuraFilter & 0x00000200u) != 0)
    {
        D2ActiveRoomStrc* pCasterRoom = UNITS_GetRoom(pCaster);
        if (!pCasterRoom || !UNITS_GetRoom(pTarget))
        {
            return FALSE;
        }

        D2CoordStrc beginCoord = {};
        beginCoord.nX = CLTSKILL_GetUnitX(pCaster);
        beginCoord.nY = CLTSKILL_GetUnitY(pCaster);

        D2CoordStrc endCoord = {};
        endCoord.nX = CLTSKILL_GetUnitX(pTarget);
        endCoord.nY = CLTSKILL_GetUnitY(pTarget);

        if (COLLISION_RayTrace(pCasterRoom, &beginCoord, &endCoord, 0x04u))
        {
            return FALSE;
        }
    }

    return TRUE;
}

// 1.12a: 0x6FAFE9B0
void __stdcall CLTSKILL_SearchUnitsInRadius(
    D2UnitStrc* pCaster,
    int32_t nOriginX,
    int32_t nOriginY,
    int32_t nRadius,
    uint32_t dwAuraFilter,
    D2AuraFilterCallback_t pfnCallback,
    void* pCallbackUserData,
    BOOL bCheckMonAuraImmune)
{
    if (!pCaster || !pfnCallback || nRadius == 0)
    {
        return;
    }

    D2ActiveRoomStrc* pOriginRoom = UNITS_GetRoom(pCaster);
    if (!pOriginRoom)
    {
        return;
    }

    if (nOriginX == 0 || nOriginY == 0)
    {
        nOriginX = CLTSKILL_GetUnitX(pCaster);
        nOriginY = CLTSKILL_GetUnitY(pCaster);
    }

    if (dwAuraFilter == 0)
    {
        dwAuraFilter = 0x583u;
    }

    D2AuraCallbackCtxStrc callbackCtx = {};
    callbackCtx.pCaster        = pCaster;
    callbackCtx.nAcceptedCount = 0;
    callbackCtx.pUserData      = pCallbackUserData;

    const int32_t nRadiusSq = nRadius * nRadius;
    D2ActiveRoomStrc** ppRoomList = nullptr;
    int nNumRooms                 = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pOriginRoom, &ppRoomList, &nNumRooms);

    const BOOL bExcludeTownRooms = (dwAuraFilter & 0x2000u) != 0;

    for (uint32_t uRoomIdx = 0; uRoomIdx < static_cast<uint32_t>(nNumRooms); ++uRoomIdx)
    {
        D2ActiveRoomStrc* pAdjRoom = ppRoomList[uRoomIdx];
        if (bExcludeTownRooms && DUNGEON_IsRoomInTown(pAdjRoom))
        {
            continue;
        }

        if (!CLTSKILL_DoesCircleOverlapRoom(pAdjRoom, nOriginX, nOriginY, nRadius))
        {
            continue;
        }

        for (D2UnitStrc* pCandidate = pAdjRoom->pUnitFirst; pCandidate != nullptr; )
        {
            D2UnitStrc* pNextInRoom = pCandidate->pRoomNext;
            if (pCandidate != pCaster)
            {
                const int32_t nCandY = CLTSKILL_GetUnitY(pCandidate);
                const int32_t nCandX = CLTSKILL_GetUnitX(pCandidate);
                if (PATH_ComputeSquaredDistance(nOriginX, nOriginY, nCandX, nCandY) <= nRadiusSq)
                {
                    if (CLTSKILL_CheckAuraFilter(pCandidate, dwAuraFilter, pCaster))
                    {
                        BOOL bAuraImmune = FALSE;
                        if (bCheckMonAuraImmune && pCandidate->dwUnitType == UNIT_MONSTER)
                        {
                            D2MonStatsTxt* pMonStatsTxt = DATATBLS_GetMonStatsTxtRecord(pCandidate->dwClassId);
                            if (pMonStatsTxt && (pMonStatsTxt->dwMonStatsFlags & MONSTATSFLAG_NOAURA) != 0)
                            {
                                bAuraImmune = TRUE;
                            }
                        }

                        if (!bAuraImmune && pfnCallback(&callbackCtx, pCandidate))
                        {
                            ++callbackCtx.nAcceptedCount;
                        }
                    }
                }
            }
            pCandidate = pNextInRoom;
        }
    }
}

// 1.12a: 0x6FAFEBA0
D2UnitStrc* __fastcall CLTSKILL_FindNextTargetInRadius(
    D2UnitStrc* pCaster,
    uint32_t dwPivotGUID,
    uint32_t dwExtraFilter,
    int32_t nRadius,
    int32_t nOriginX,
    int32_t nOriginY,
    int32_t* pOutMatchCount)
{
    D2AuraRingTargetTrackerStrc tracker = {};
    tracker.pNextBestUnit  = nullptr;
    tracker.dwNextBestGUID = D2UnitInvalidGUID;
    tracker.pWrapMinUnit   = nullptr;
    tracker.dwWrapMinGUID  = dwPivotGUID;
    tracker.dwPivotGUID    = dwPivotGUID;
    tracker.nMatchCount    = 0;

    uint32_t dwFilter = (dwExtraFilter != 0) ? dwExtraFilter : 0x583u;
    dwFilter |= 0xA280u;

    CLTSKILL_SearchUnitsInRadius(
        pCaster,
        nOriginX,
        nOriginY,
        nRadius,
        dwFilter,
        CLTSKILL_AuraCallback_SelectNextRingTarget,
        &tracker,
        FALSE);

    D2UnitStrc* pSelected = tracker.pNextBestUnit ? tracker.pNextBestUnit : tracker.pWrapMinUnit;
    if (pOutMatchCount)
    {
        *pOutMatchCount = tracker.nMatchCount;
    }

    return pSelected;
}

// 1.12a: 0x6FAFEC20
int32_t __fastcall CLTSKILL_CountTargetsInRadius(
    D2UnitStrc* pCaster,
    int32_t nRadius,
    uint32_t dwExtraFilter,
    int32_t nOriginX,
    int32_t nOriginY)
{
    int32_t nMatchCount = 0;
    CLTSKILL_SearchUnitsInRadius(
        pCaster,
        nOriginX,
        nOriginY,
        nRadius,
        dwExtraFilter | 0xA783u,
        CLTSKILL_AuraCallback_CountTarget,
        &nMatchCount,
        FALSE);
    return nMatchCount;
}

// 1.12a: 0x6FAFEC60
void __fastcall CLTSKILL_ExecuteSkillCltPrgFunc0WithTempTarget(
    D2UnitStrc* pUnit,
    D2UnitStrc* pOverrideTargetUnit,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nOverrideX,
    int32_t nOverrideY,
    uint8_t nOverrideSeedByte)
{
    if (!pUnit)
    {
        return;
    }

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return;
    }

    const uint32_t dwSavedInitSeed = pUnit->dwInitSeed;
    int32_t nSavedX = 0;
    int32_t nSavedY = 0;
    const int32_t bHadSavedCoords = CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nSavedX, &nSavedY);

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);
    D2UnitStrc* pSavedTargetUnit = PATH_GetTargetUnit(pUnit->pDynamicPath);

    if (pOverrideTargetUnit)
    {
        UNITS_SetTargetUnitForDynamicUnit(pUnit, pOverrideTargetUnit);
        pUnit->dwInitSeed = nOverrideSeedByte;
    }
    else if (nOverrideX != 0)
    {
        if (nOverrideY != 0)
        {
            D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nOverrideX, nOverrideY);
            pUnit->dwInitSeed = nOverrideSeedByte;
        }
        else
        {
            pUnit->dwInitSeed = static_cast<uint32_t>(nOverrideX);
        }
    }

    const int16_t nPrgFuncIdx = static_cast<int16_t>(pSkillsTxt->wCltPrgFunc[0]);
    if (nPrgFuncIdx >= 0 && nPrgFuncIdx < gnCltDoFuncs && gpfnCltDoFunc[nPrgFuncIdx])
    {
        if (gpfnCltDoFunc[nPrgFuncIdx](pUnit, nSkillId, nSkillLevel))
        {
            const int16_t nDoSound = static_cast<int16_t>(pSkillsTxt->wDoSound);
            if (nDoSound > 0)
            {
                D2Client_PlaySoundEx_6FAE1FA0(pUnit, nDoSound, 0, 0, 0);
            }

            if (static_cast<int16_t>(pSkillsTxt->wTgtSound) > 0)
            {
                D2UnitStrc* pTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);
                if (pTarget)
                {
                    D2Client_PlaySoundEx_6FAE1FA0(pTarget, static_cast<int16_t>(pSkillsTxt->wTgtSound), 0, 0, 0);
                }
            }

            if (static_cast<int16_t>(pSkillsTxt->wTgtOverlay) > 0)
            {
                D2UnitStrc* pTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);
                if (pTarget)
                {
                    D2Client_AttachUnitOverlay_6FB430E0(
                        pTarget,
                        static_cast<int16_t>(pSkillsTxt->wTgtOverlay),
                        2,
                        0,
                        0,
                        0,
                        0,
                        0);
                }
            }
        }
    }

    UNITS_SetTargetUnitForDynamicUnit(pUnit, pSavedTargetUnit);
    if (bHadSavedCoords && !pSavedTargetUnit)
    {
        D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nSavedX, nSavedY);
    }
    pUnit->dwInitSeed = dwSavedInitSeed;
}

// 1.12a: 0x6FAFEE40
int32_t __fastcall CLTSKILL_StartSkill(
    D2UnitStrc* pUnit,
    int32_t nSkillId,
    D2UnitGUID dwItemOwnerGUID,
    int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    if (pSkillsTxt->nLineOfSight != SKILLSLOS_NONE)
    {
        int32_t nTargetX = 0;
        int32_t nTargetY = 0;
        if (CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
        {
            uint16_t wColMask = 0;
            switch (pSkillsTxt->nLineOfSight)
            {
            case SKILLSLOS_MISSILE_BARRIER: wColMask = 0x0004u; break;
            case SKILLSLOS_PLAYER_PATH:     wColMask = 0x1C09u; break;
            case SKILLSLOS_PLAYER_MONSTER:  wColMask = 0x0180u; break;
            case SKILLSLOS_PLAYER_FLYING:   wColMask = 0x0804u; break;
            case SKILLSLOS_RADIAL_BARRIER:  wColMask = 0x0805u; break;
            default:
                return 0;
            }

            if (!D2Common_11026(nTargetX, nTargetY, pUnit, wColMask))
            {
                return 0;
            }
        }
    }

    if (!CLTSKILL_ValidateTargetCorpseOrSelectability(pUnit, nSkillId))
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || SKILLS_GetSkillIdFromSkill(pSkill, "..\\Source\\D2Client\\SKILLS\\Skills.cpp", 1284) != nSkillId)
    {
        pSkill = SKILLS_GetSkillById(pUnit, nSkillId, dwItemOwnerGUID);
        if (!pSkill)
        {
            SKILLS_AddSkill(pUnit, nSkillId);
            pSkill = SKILLS_GetSkillById(pUnit, nSkillId, dwItemOwnerGUID);
            if (pSkill && pUnit == gpPlayerUnit && dwItemOwnerGUID == D2UnitInvalidGUID)
            {
                SKILLS_SetFlags(pSkill, 0x08);
            }

            const int32_t nMaxLvl = CLTSKILL_GetSkillMaxLevel(nSkillId);
            if (nSkillLevel > nMaxLvl)
            {
                SKILLS_AssignSkill(pUnit, nSkillId, nMaxLvl, FALSE, "..\\Source\\D2Client\\SKILLS\\Skills.cpp", 1299);
                D2Common_11030(pUnit, nSkillId, nSkillLevel - nMaxLvl);
            }
            else
            {
                SKILLS_AssignSkill(pUnit, nSkillId, nSkillLevel, FALSE, "..\\Source\\D2Client\\SKILLS\\Skills.cpp", 1304);
            }

            if (!pSkill)
            {
                return 0;
            }
        }

        UNITS_SetUsedSkill(pUnit, pSkill);
    }

    if (dwItemOwnerGUID != D2UnitInvalidGUID)
    {
        D2UnitGUID dwInfoOwner = 0;
        int nInfoSkillId       = 0;
        int nInfoSkillLvl      = nSkillLevel;
        int nCharges           = 0;
        if (!SKILLS_GetSkillInfo(pSkill, &dwInfoOwner, &nInfoSkillId, &nInfoSkillLvl, &nCharges) || nCharges == 0)
        {
            return 0;
        }
        nSkillLevel = nInfoSkillLvl;
    }

    if (pUnit != gpPlayerUnit)
    {
        const int32_t nMaxLvl = CLTSKILL_GetSkillMaxLevel(nSkillId);
        if (nSkillLevel > nMaxLvl)
        {
            SKILLS_AssignSkill(pUnit, nSkillId, nMaxLvl, FALSE, "..\\Source\\D2Client\\SKILLS\\Skills.cpp", 1338);
            D2Common_11030(pUnit, nSkillId, nSkillLevel - nMaxLvl);
        }
        else
        {
            SKILLS_AssignSkill(pUnit, nSkillId, nSkillLevel, FALSE, "..\\Source\\D2Client\\SKILLS\\Skills.cpp", 1343);
        }
    }
    else
    {
        nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, TRUE);
    }

    D2UnitStrc* pTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);
    if (pTarget && (pSkillsTxt->dwFlags[0] & SKILLSFLAG_TARGETALLY) == 0)
    {
        if (D2Client_CanTargetAlliedUnit_6FACF7B0(pUnit, pTarget))
        {
            return 0;
        }
    }

    if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_INTOWN) == 0 && DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
    {
        UNITS_SetUsedSkill(pUnit, nullptr);
        D2UnitModeTransitionStrc modeParams = {};
        D2Client_ChangeUnitModeEx_6FAF1450(7, pUnit, &modeParams, FALSE);
        if (pUnit == gpPlayerUnit)
        {
            D2Client_PlayCharacterBarkSound_6FAE7250(pUnit, 24);
        }
        return 0;
    }

    int32_t nAnimMode = 0;
    if (pUnit && static_cast<uint8_t>(pUnit->dwUnitType) == UNIT_PLAYER && nSkillId == 0)
    {
        nAnimMode = CLTSKILL_GetPlayerAttackAnimMode(pUnit);
    }
    else
    {
        nAnimMode = SKILLS_GetSkillModeFromUnit(pUnit, pSkill);
    }

    if (nAnimMode != 0)
    {
        D2Client_SetUnitCastOrNeutralAnimMode_6FAF13C0(pUnit, nAnimMode);
    }
    else if (!(STATES_CheckState(pUnit, STATE_INFERNO) && (pSkillsTxt->dwFlags[0] & SKILLSFLAG_REPEAT) != 0))
    {
        if (pUnit && (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER))
        {
            D2Client_SetUnitCastOrNeutralAnimMode_6FAF13C0(pUnit, 1);
        }
    }

    int32_t nStartResult = 1;
    const int16_t nCltStFuncIdx = static_cast<int16_t>(pSkillsTxt->wCltStFunc);
    if (nCltStFuncIdx >= 0 && nCltStFuncIdx < gnCltStFuncs && gpfnCltStFunc[nCltStFuncIdx])
    {
        nStartResult = gpfnCltStFunc[nCltStFuncIdx](pUnit, nSkillId, nSkillLevel);
        if (!nStartResult)
        {
            UNITS_SetUsedSkill(pUnit, nullptr);
            if (pUnit && (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER))
            {
                D2Client_SetUnitCastOrNeutralAnimMode_6FAF13C0(pUnit, 1);
            }
            if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_STSUCCESSONLY) != 0)
            {
                return nStartResult;
            }
        }
    }

    int16_t nStartSound = static_cast<int16_t>(pSkillsTxt->wStSound);
    if (dwItemOwnerGUID != D2UnitInvalidGUID)
    {
        const int16_t nItemCastSound = static_cast<int16_t>(pSkillsTxt->wItemCastSound);
        if (nItemCastSound > 0)
        {
            nStartSound = nItemCastSound;
        }
    }

    if (nStartSound > 0)
    {
        int32_t nDelayFrames = 0;
        if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_STSOUNDDELAY) != 0)
        {
            nDelayFrames = D2Client_GetWeaponSoundDelayFrames_6FAE6720(pUnit, UNITS_GetHitClass(pUnit)) - 3;
            if (nDelayFrames < 0)
            {
                nDelayFrames = 0;
            }
        }
        D2Client_PlaySoundEx_6FAE1FA0(pUnit, nStartSound, nDelayFrames, 0, 0);
    }

    if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_WEAPONSND) != 0)
    {
        const int32_t nSwingSound = D2Client_GetWeaponSwingSoundId_6FAE6780(pUnit, UNITS_GetHitClass(pUnit));
        if (nSwingSound != 0)
        {
            int32_t nDelayFrames = D2Client_GetWeaponSoundDelayFrames_6FAE6720(pUnit, UNITS_GetHitClass(pUnit));
            if (nDelayFrames < 0)
            {
                nDelayFrames = 0;
            }
            D2Client_PlaySoundEx_6FAE1FA0(pUnit, nSwingSound, nDelayFrames, 0, 0);
        }
    }

    const int16_t nClassStartSound = static_cast<int16_t>(pSkillsTxt->nStSoundClass);
    if ((dwItemOwnerGUID == D2UnitInvalidGUID || static_cast<int16_t>(pSkillsTxt->wItemCastSound) <= 0)
        && nClassStartSound > 0
        && pUnit
        && pUnit->dwUnitType == UNIT_PLAYER
        && pUnit->dwClassId == static_cast<uint8_t>(pSkillsTxt->nCharClass))
    {
        int32_t nDelayFrames = 0;
        if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_STSOUNDDELAY) != 0)
        {
            nDelayFrames = D2Client_GetWeaponSoundDelayFrames_6FAE6720(pUnit, UNITS_GetHitClass(pUnit)) - 3;
            if (nDelayFrames < 0)
            {
                nDelayFrames = 0;
            }
        }
        D2Client_PlaySoundEx_6FAE1FA0(pUnit, nClassStartSound, nDelayFrames, 0, 0);
    }

    int32_t nCastOverlay = static_cast<int16_t>(pSkillsTxt->wCastOverlay);
    if (dwItemOwnerGUID != D2UnitInvalidGUID)
    {
        const int16_t nItemCastOverlay = static_cast<int16_t>(pSkillsTxt->wItemCastOverlay);
        if (nItemCastOverlay > 0)
        {
            nCastOverlay = nItemCastOverlay;
        }
    }

    if (nCastOverlay > 0 && sgptDataTables && nCastOverlay < sgptDataTables->nOverlayTxtRecordCount)
    {
        if (!D2Client_HasUnitOverlay_6FB40A60(pUnit, nCastOverlay))
        {
            D2Client_AttachUnitOverlay_6FB430E0(pUnit, nCastOverlay, 2, 0, 0, 0, 0, 0);
        }
    }

    if (nStartResult != 0 && static_cast<int16_t>(pSkillsTxt->wCltMissile) > 0)
    {
        CLTSKILL_AttachMissileLightMap(static_cast<int16_t>(pSkillsTxt->wCltMissile), pUnit);
    }

    return nStartResult;
}

// 1.12a: 0x6FAFF380
int32_t __fastcall CLTSKILL_StartSkillAtCoords(D2UnitStrc* pUnit, D2SkillStartRequestStrc* pReq)
{
    const int32_t nTargetX = pReq->nTargetX;
    const int32_t nTargetY = pReq->nTargetY;

    if (UNITS_CanDualWield(pUnit))
    {
        CLTSKILL_ResolveDualWieldNormalAttackToLeftHandSwing(pUnit, pReq);
    }

    D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nTargetX, nTargetY);
    UNITS_UpdateDirectionAndSpeed(pUnit, nTargetX, nTargetY);

    if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        D2MonStatsTxt* pMonStatsTxt = DATATBLS_GetMonStatsTxtRecord(pUnit->dwClassId);
        if (pMonStatsTxt && pMonStatsTxt->dwCode == 0x20204250u)
        {
            const uint8_t nAdjustedDir = static_cast<uint8_t>((static_cast<int32_t>(PATH_GetNewDirection(pUnit->pDynamicPath)) + 56) & 63);
            D2COMMON_10160_PathUpdateDirection(pUnit->pDynamicPath, nAdjustedDir);
        }
    }

    UNITS_SetTargetUnitForDynamicUnit(pUnit, nullptr);
    return CLTSKILL_StartSkill(pUnit, pReq->nSkillId, pReq->dwItemOwnerGUID, pReq->nSkillLevel);
}

// 1.12a: 0x6FAFF440
int32_t __fastcall CLTSKILL_StartSkillSimple(
    D2UnitStrc* pUnit,
    int32_t nSkillId,
    int32_t nSkillLevel,
    D2UnitGUID dwItemOwnerGUID)
{
    return CLTSKILL_StartSkill(pUnit, nSkillId, dwItemOwnerGUID, nSkillLevel);
}

// 1.12a: 0x6FAFF450 (gpfnCltDoFunc[1])
int32_t __fastcall CLTSKILL_Do001_Attack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    if (COMPOSIT_IsWeaponBowOrXBow(pUnit))
    {
        int nOutMissileLevel = nSkillLevel;
        const int32_t nMissileId = D2COMMON_11039_CheckWeaponIsMissileBased(pUnit, &nOutMissileLevel);
        if (nMissileId != -1)
        {
            CLTSKILL_SpawnLinearMissileToResolvedTarget(pUnit, nMissileId, nSkillId, nOutMissileLevel);
        }
        return 1;
    }

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);
    PATH_GetTargetUnit(pUnit->pDynamicPath);
    return 1;
}

// 1.12a: 0x6FAFF4D0
void __fastcall CLTSKILL_SpawnQuadrantScatterMissiles(
    D2UnitStrc* pSeedUnit,
    D2MissileStrc* pMissileParams,
    int32_t nCount,
    int32_t nRadius)
{
    int32_t nQuadrant = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&pSeedUnit->pSeed)) & 3u);
    const int32_t nBaseY = pMissileParams->nY;
    const int32_t nBaseX = pMissileParams->nX;

    if (nCount <= 0)
    {
        return;
    }

    const int32_t nDiameter = nRadius * 2;
    for (int32_t i = 0; i < nCount; ++i)
    {
        int32_t nDirOffX = 0;
        int32_t nDirOffY = 0;
        switch (nQuadrant)
        {
        case 0: nDirOffX = -16; nDirOffY = -16; break;
        case 1: nDirOffX = -16; nDirOffY =  16; break;
        case 2: nDirOffX =  16; nDirOffY =  16; break;
        case 3: nDirOffX =  16; nDirOffY = -16; break;
        default: break;
        }

        nQuadrant = (nQuadrant + 1) & 3;

        const int32_t nRandX = (nDiameter > 0) ? SEED_RollLimitedRandomNumber(&pSeedUnit->pSeed, nDiameter) : 0;
        const int32_t nSpawnX = nBaseX + (nRandX - nRadius);
        pMissileParams->nX = nSpawnX;

        const int32_t nRandY = (nDiameter > 0) ? SEED_RollLimitedRandomNumber(&pSeedUnit->pSeed, nDiameter) : 0;
        const int32_t nSpawnY = nBaseY + (nRandY - nRadius);
        pMissileParams->nY = nSpawnY;

        pMissileParams->nTargetX  = nSpawnX + nDirOffX;
        pMissileParams->nTargetY  = nSpawnY + nDirOffY;
        pMissileParams->pInitFunc = CLTSKILL_SetUnitPathDirection;
        pMissileParams->pInitArgs = nQuadrant << 4;

        D2Client_CreateClientMissile_6FAD4B20(pMissileParams);
    }
}

// 1.12a: 0x6FAFF690
void __fastcall CLTSKILL_SpawnStationaryMissileAtTargetOrCoords(
    D2UnitStrc* pOwner,
    int32_t nMissileId,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nX,
    int32_t nY)
{
    if (nX == 0 || nY == 0)
    {
        if (!CLTSKILL_ResolveTargetOrDirectionCoords(pOwner, &nX, &nY))
        {
            return;
        }
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x01u;
    missileParams.pOwner      = pOwner;
    missileParams.nMissile    = nMissileId;
    missileParams.nX          = nX;
    missileParams.nY          = nY;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    D2Client_CreateClientMissile_6FAD4B20(&missileParams);
}

// 1.12a: 0x6FAFF710
void __fastcall CLTSKILL_SpawnMultiRingRadialBurst(D2MissileStrc* pMissileParams, int32_t nTotalCount)
{
    int32_t nRemaining = nTotalCount - 1;
    D2_ASSERT((nRemaining / 5) <= 7);

    pMissileParams->nTargetX = 14;
    pMissileParams->nTargetY = -14;
    D2Client_CreateClientMissile_6FAD4B20(pMissileParams);

    for (int32_t nRingIdx = 0; nRingIdx < 7 && nRemaining != 0; ++nRingIdx)
    {
        for (int32_t nQuad = 0; nQuad < 4; ++nQuad)
        {
            if (nRemaining > 0)
            {
                pMissileParams->nTargetX = g_anQuadrantSignX[nQuad] * g_anRingMajorAxis[nRingIdx];
                pMissileParams->nTargetY = g_anQuadrantSignY[nQuad] * g_anRingMinorAxis[nRingIdx];
                D2Client_CreateClientMissile_6FAD4B20(pMissileParams);

                pMissileParams->nTargetX = g_anQuadrantSignX[nQuad] * g_anRingMinorAxis[nRingIdx];
                pMissileParams->nTargetY = g_anQuadrantSignY[nQuad] * g_anRingMajorAxis[nRingIdx];
                D2Client_CreateClientMissile_6FAD4B20(pMissileParams);

                nRemaining -= 2;
            }
        }

        pMissileParams->nTargetX = g_anRingCardinalDiagX[nRingIdx];
        pMissileParams->nTargetY = g_anRingCardinalDiagY[nRingIdx];
        D2Client_CreateClientMissile_6FAD4B20(pMissileParams);
        --nRemaining;
    }
}

// 1.12a: 0x6FAFF800
D2UnitStrc* __fastcall CLTSKILL_SpawnLinearMissileWithOriginOffset(
    D2UnitStrc* pOwner,
    int32_t nTargetX,
    int32_t nMissileId,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nOriginOffsetX,
    int32_t nOriginOffsetY,
    int32_t nTargetY)
{
    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x21u;
    missileParams.pOwner      = pOwner;
    missileParams.nX          = CLTSKILL_GetUnitX(pOwner) + nOriginOffsetX;
    missileParams.nY          = CLTSKILL_GetUnitY(pOwner) + nOriginOffsetY;
    missileParams.nMissile    = nMissileId;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    if (nTargetX != 0 && nTargetY != 0)
    {
        missileParams.nTargetX = nTargetX;
        missileParams.nTargetY = nTargetY;
    }
    else if (!CLTSKILL_ResolveTargetOrDirectionCoords(pOwner, &missileParams.nTargetX, &missileParams.nTargetY))
    {
        return nullptr;
    }

    if (missileParams.nTargetX == 0 || missileParams.nTargetY == 0)
    {
        return nullptr;
    }

    return D2Client_CreateClientMissile_6FAD4B20(&missileParams);
}

// 1.12a: 0x6FAFF900
int32_t __stdcall CLTSKILL_SpawnSeededRandomLobRain(
    D2UnitStrc* pUnit,
    int32_t nMissileId,
    int32_t nCount,
    int32_t nRadius,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    int32_t nCenterX = 0;
    int32_t nCenterY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nCenterX, &nCenterY))
    {
        return 0;
    }

    SEED_InitLowSeed(&pUnit->pSeed, nCenterX);

    const int32_t nDiameter = nRadius * 2;
    for (int32_t i = 0; i < nCount; ++i)
    {
        const int32_t nRandX = (nDiameter > 0) ? SEED_RollLimitedRandomNumber(&pUnit->pSeed, nDiameter) : 0;
        const int32_t nDropX = nCenterX + (nRandX - nRadius);

        const int32_t nRandY = (nDiameter > 0) ? SEED_RollLimitedRandomNumber(&pUnit->pSeed, nDiameter) : 0;
        const int32_t nDropY = nCenterY + (nRandY - nRadius);

        const int32_t nSqDist = PATH_ComputeSquaredDistance(
            CLTSKILL_GetUnitX(pUnit),
            CLTSKILL_GetUnitY(pUnit),
            nDropX,
            nDropY);
        if (nSqDist >= 4)
        {
            D2Client_SpawnLobMissile_6FADB4B0(
                pUnit,
                nMissileId,
                0,
                0,
                nDropX,
                nDropY,
                nSkillId,
                nSkillLevel,
                0);
        }
    }

    return 1;
}

// 1.12a: 0x6FAFFAA0
void __fastcall CLTSKILL_ExecuteFinishingMoveCharges(
    D2UnitStrc* pUnit,
    D2UnitStrc* pOverrideTargetUnit,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nOverrideX,
    int32_t nOverrideY,
    int32_t nChargeCount)
{
    if (!pUnit || !sgptDataTables)
    {
        return;
    }

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return;
    }

    const int16_t nAuraState = pSkillsTxt->nAuraState;
    if (nAuraState < 0 || nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return;
    }

    const int16_t nAuraStat = pSkillsTxt->wAuraStat[0];
    if (nAuraStat < 0 || nAuraStat >= sgptDataTables->nItemStatCostTxtRecordCount)
    {
        return;
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nAuraState);
    if (!pStatList)
    {
        pStatList = STATLIST_AllocStatList(nullptr, 0u, 0u, pUnit->dwUnitType, pUnit->dwUnitId);
        if (!pStatList)
        {
            return;
        }
        STATLIST_SetState(pStatList, nAuraState);
        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, TRUE);
        STATLIST_SetStatIfListIsValid(pStatList, nAuraStat, nChargeCount, 0);
    }

    int32_t nSavedX = 0;
    int32_t nSavedY = 0;
    const int32_t bHadSavedCoords = CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nSavedX, &nSavedY);
    D2UnitStrc* pSavedTargetUnit  = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);

    if (pOverrideTargetUnit)
    {
        UNITS_SetTargetUnitForDynamicUnit(pUnit, pOverrideTargetUnit);
    }
    else if (nOverrideX != 0 && nOverrideY != 0)
    {
        D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nOverrideX, nOverrideY);
    }

    const uint32_t dwSavedInitSeed = pUnit->dwInitSeed;
    if (nOverrideX != 0 && nOverrideY == 0)
    {
        pUnit->dwInitSeed = static_cast<uint32_t>(nOverrideX);
    }

    const int32_t nFinalStageIdx = nChargeCount - 1;
    if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_PRGSTACK) != 0)
    {
        for (int32_t nStage = 0; nStage < nFinalStageIdx; ++nStage)
        {
            const int16_t nStageFuncIdx = static_cast<int16_t>(pSkillsTxt->wCltPrgFunc[nStage]);
            if (nStageFuncIdx >= 0 && nStageFuncIdx < gnCltDoFuncs && gpfnCltDoFunc[nStageFuncIdx])
            {
                STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxt->wAuraStat[0], nStage + 1, 0);
                gpfnCltDoFunc[nStageFuncIdx](pUnit, nSkillId, nSkillLevel);
            }
        }
        STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxt->wAuraStat[0], nFinalStageIdx + 1, 0);
    }

    if (nFinalStageIdx >= 0 && nFinalStageIdx < 3)
    {
        const int16_t nFinalFuncIdx = static_cast<int16_t>(pSkillsTxt->wCltPrgFunc[nFinalStageIdx]);
        if (nFinalFuncIdx >= 0 && nFinalFuncIdx < gnCltDoFuncs)
        {
            const int16_t nCltMissile = static_cast<int16_t>(pSkillsTxt->wCltMissile);
            if (nCltMissile > 0)
            {
                if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_LOB) != 0)
                {
                    int32_t nLobX = 0;
                    int32_t nLobY = 0;
                    CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nLobX, &nLobY);
                    D2Client_SpawnLobMissile_6FADB4B0(pUnit, nCltMissile, 0, 0, nLobX, nLobY, nSkillId, nSkillLevel, 0);
                }
                else
                {
                    CLTSKILL_SpawnLinearMissileToResolvedTarget(pUnit, nCltMissile, nSkillId, nSkillLevel);
                }
                pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
            }

            if (gpfnCltDoFunc[nFinalFuncIdx])
            {
                if (gpfnCltDoFunc[nFinalFuncIdx](pUnit, nSkillId, nSkillLevel))
                {
                    if (static_cast<int16_t>(pSkillsTxt->wDoSound) > 0)
                    {
                        D2Client_PlaySoundEx_6FAE1FA0(pUnit, static_cast<int16_t>(pSkillsTxt->wDoSound), 0, 0, 0);
                    }

                    if (static_cast<int16_t>(pSkillsTxt->wTgtSound) > 0)
                    {
                        D2UnitStrc* pTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);
                        if (pTarget)
                        {
                            D2Client_PlaySoundEx_6FAE1FA0(pTarget, static_cast<int16_t>(pSkillsTxt->wTgtSound), 0, 0, 0);
                        }
                    }

                    if (static_cast<int16_t>(pSkillsTxt->wTgtOverlay) > 0)
                    {
                        D2UnitStrc* pTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);
                        if (pTarget)
                        {
                            D2Client_AttachUnitOverlay_6FB430E0(
                                pTarget,
                                static_cast<int16_t>(pSkillsTxt->wTgtOverlay),
                                2,
                                0,
                                0,
                                0,
                                0,
                                0);
                        }
                    }
                }
            }
            else
            {
                pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
            }
        }
    }

    UNITS_SetTargetUnitForDynamicUnit(pUnit, pSavedTargetUnit);
    if (bHadSavedCoords)
    {
        D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nSavedX, nSavedY);
    }
    if (nOverrideX != 0 && nOverrideY == 0)
    {
        pUnit->dwInitSeed = dwSavedInitSeed;
    }
}

// 1.12a: 0x6FAFFDE0
int32_t __stdcall CLTSKILL_DoSkill(
    D2UnitStrc* pUnit,
    int32_t nSkillId,
    int32_t nSkillLevel,
    BOOL bUseItemCltEffect)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int16_t nBaseDoFunc = static_cast<int16_t>(pSkillsTxt->wCltDoFunc);
    if (nBaseDoFunc < 0 || nBaseDoFunc >= gnCltDoFuncs)
    {
        return 0;
    }

    BOOL bExecuted = FALSE;
    const int16_t nCltMissile = static_cast<int16_t>(pSkillsTxt->wCltMissile);
    if (nCltMissile > 0)
    {
        int32_t nOriginOffsetX = 0;
        int32_t nOriginOffsetY = 0;
        int32_t nTargetX       = 0;
        int32_t nTargetY       = 0;

        if (bUseItemCltEffect != 0)
        {
            CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nOriginOffsetX, &nOriginOffsetY);
            if (nOriginOffsetX != 0 && nOriginOffsetY != 0)
            {
                nTargetX = nOriginOffsetX;
                nTargetY = nOriginOffsetY;
                nOriginOffsetX -= CLTSKILL_GetUnitX(pUnit);
                nOriginOffsetY -= CLTSKILL_GetUnitY(pUnit);
                nTargetX += nOriginOffsetX;
                nTargetY += nOriginOffsetY;
            }
        }

        D2UnitStrc* pSpawnedMissile = nullptr;
        if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_LOB) != 0)
        {
            pSpawnedMissile = D2Client_SpawnLobMissile_6FADB4B0(
                pUnit,
                nCltMissile,
                nOriginOffsetX,
                nOriginOffsetY,
                nTargetX,
                nTargetY,
                nSkillId,
                nSkillLevel,
                0);
        }
        else
        {
            pSpawnedMissile = CLTSKILL_SpawnLinearMissileWithOriginOffset(
                pUnit,
                nTargetX,
                nCltMissile,
                nSkillId,
                nSkillLevel,
                nOriginOffsetX,
                nOriginOffsetY,
                nTargetY);
        }

        if (pSpawnedMissile)
        {
            bExecuted = TRUE;
        }

        if (pUnit)
        {
            pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
        }
    }

    int32_t nDispatchIdx = nBaseDoFunc;
    if (bUseItemCltEffect != 0)
    {
        const int16_t nItemCltEffect = static_cast<int16_t>(pSkillsTxt->wItemCltEffect);
        if (nItemCltEffect > 1)
        {
            nDispatchIdx = nItemCltEffect;
        }
    }

    if (nDispatchIdx >= 0 && nDispatchIdx < gnCltDoFuncs && gpfnCltDoFunc[nDispatchIdx])
    {
        if (gpfnCltDoFunc[nDispatchIdx](pUnit, nSkillId, nSkillLevel))
        {
            bExecuted = TRUE;
        }
    }
    else
    {
        if (pUnit)
        {
            pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
        }
        bExecuted = TRUE;
    }

    D2ClientLightMapStrc* pPrevLightMap = static_cast<D2ClientLightMapStrc*>(D2Common_10958(pUnit, nullptr));
    if (pPrevLightMap)
    {
        D2Client_FreeClientLightMap_6FB0E500(pPrevLightMap);
    }

    if (!bExecuted)
    {
        return 0;
    }

    if (static_cast<int16_t>(pSkillsTxt->wDoSound) > 0)
    {
        D2Client_PlaySoundEx_6FAE1FA0(pUnit, static_cast<int16_t>(pSkillsTxt->wDoSound), 0, 0, 0);
    }

    if (static_cast<int16_t>(pSkillsTxt->wTgtSound) > 0)
    {
        D2UnitStrc* pTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);
        if (pTarget)
        {
            D2Client_PlaySoundEx_6FAE1FA0(pTarget, static_cast<int16_t>(pSkillsTxt->wTgtSound), 0, 0, 0);
        }
    }

    if (static_cast<int16_t>(pSkillsTxt->wTgtOverlay) > 0)
    {
        D2UnitStrc* pTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);
        if (pTarget)
        {
            D2Client_AttachUnitOverlay_6FB430E0(
                pTarget,
                static_cast<int16_t>(pSkillsTxt->wTgtOverlay),
                2,
                0,
                0,
                0,
                0,
                0);
        }
    }

    int32_t nDelayFrames = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwDelay, nSkillId, nSkillLevel);
    if (nDelayFrames > 0 && pUnit == gpPlayerUnit)
    {
        if (nDelayFrames > 12)
        {
            nDelayFrames = 12;
        }

        if (pSkillsTxt->nAnim != PLRMODE_SEQUENCE || (pUnit->dwSeqFrame & 0xFFFFFF00) == 0)
        {
            gdwaSkillDelayEndFrame = static_cast<int32_t>(gdwClientTickCount) + nDelayFrames;
        }
    }

    return 1;
}

// 1.12a: 0x6FB000B0 (gpfnCltDoFunc[2])
int32_t __fastcall CLTSKILL_Do002_Throw(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pWeapon = D2Common_10434(pUnit, FALSE);
    if (!pWeapon)
    {
        return 0;
    }

    const int32_t nMissileId = ITEMS_GetMissileType(pWeapon);
    if (ITEMS_CheckIfThrowable(pWeapon) == 0x26)
    {
        int32_t nTargetX = 0;
        int32_t nTargetY = 0;
        CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY);
        D2Client_SpawnLobMissile_6FADB4B0(pUnit, nMissileId, 0, 0, nTargetX, nTargetY, nSkillId, 1, 0);
        return 1;
    }

    CLTSKILL_SpawnLinearMissileToResolvedTarget(pUnit, nMissileId, nSkillId, 1);
    return 1;
}

// 1.12a: 0x6FB00150
int32_t __fastcall CLTSKILL_DoUsedSkill(D2UnitStrc* pUnit)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nSkillId    = SKILLS_GetSkillIdFromSkill(pSkill, "..\\Source\\D2Client\\SKILLS\\Skills.cpp", 1687);
    const int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, TRUE);
    return CLTSKILL_DoSkill(pUnit, nSkillId, nSkillLevel, FALSE);
}

// 1.12a: 0x6FB00190
int32_t __fastcall CLTSKILL_EnterSequenceModeWithTarget(D2UnitStrc* pTarget, D2UnitStrc* pUnit)
{
    D2UnitModeTransitionStrc modeParams = {};
    modeParams.nSkillId       = -1;
    modeParams.nTargetType    = pTarget ? static_cast<int32_t>(pTarget->dwUnitType) : 6;
    modeParams.dwTargetGUID   = pTarget ? pTarget->dwUnitId : D2UnitInvalidGUID;
    modeParams.bHasTargetUnit = 1;

    D2Client_ChangeUnitModeEx_6FAF1450(22, pUnit, &modeParams, TRUE);
    return 1;
}

// 1.12a: 0x6FB00210
int32_t __fastcall CLTSKILL_StartSkillOnTargetUnit(D2UnitStrc* pUnit, D2SkillStartRequestStrc* pReq)
{
    if (UNITS_CanDualWield(pUnit) && (!pUnit || pUnit->dwUnitType != UNIT_MONSTER))
    {
        CLTSKILL_ResolveDualWieldNormalAttackToLeftHandSwing(pUnit, pReq);
    }

    D2UnitStrc* pTarget = CLTSKILL_FindServerUnit(pReq->nTargetType, pReq->dwTargetGUID);
    if (pTarget)
    {
        UNITS_SetTargetUnitForDynamicUnit(pUnit, pTarget);
        D2Client_FaceUnitTarget_6FACF5F0(pUnit, 0);
        return CLTSKILL_StartSkill(pUnit, pReq->nSkillId, pReq->dwItemOwnerGUID, pReq->nSkillLevel);
    }

    D2Client_ChangeUnitModeEx_6FAF1450(7, pUnit, nullptr, FALSE);
    LOG_11100(
        1,
        2,
        4,
        0x40,
        "..\\Source\\D2Client\\SKILLS\\Skills.cpp",
        1986,
        "StartsStartSkillID - Invalid Target");
    return 0;
}

// 1.12a: 0x6FB002B0
void __fastcall CLTSKILL_StatRemove_PostAttackSkillEnd(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    (void)nState;
    (void)pStatList;

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return;
    }

    if (STATES_CheckState(pUnit, STATE_STUNNED))
    {
        return;
    }

    if ((SKILLS_GetFlags(pSkill) & 0x02) != 0)
    {
        return;
    }

    const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, "..\\Source\\D2Client\\SKILLS\\Skills.cpp", 2364);
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return;
    }

    switch (static_cast<int16_t>(pSkillsTxt->wSrvDoFunc))
    {
    case 67:
        D2Client_FinishPaladinCharge_6FB373F0(pUnit, nSkillId);
        break;
    case 76:
        D2Client_FinishBarbarianLeap_6FB73E70(pUnit, nSkillId);
        break;
    case 77:
    case 78:
        D2Client_FinishBarbarianWhirlwind_6FB74C30(pUnit);
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FB00370
void __fastcall CLTSKILL_PostDeferredMovementSkillEndStatList(D2UnitStrc* pUnit)
{
    const int32_t nUnitType    = pUnit ? static_cast<int32_t>(pUnit->dwUnitType) : 6;
    const D2UnitGUID dwUnitGUID = pUnit ? pUnit->dwUnitId : D2UnitInvalidGUID;

    D2StatListStrc* pStatList = STATLIST_AllocStatList(nullptr, 0x06u, 50u, nUnitType, dwUnitGUID);
    if (pStatList)
    {
        STATLIST_SetStatRemoveCallback(pStatList, CLTSKILL_StatRemove_PostAttackSkillEnd);
        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, TRUE);
    }
}

// 1.12a: 0x6FB003B0
void __fastcall CLTSKILL_MissileInit_ChargedBoltPath10(D2UnitStrc* pMissile, int32_t nLowSeed)
{
    if (!pMissile)
    {
        return;
    }

    int32_t nFrames = MISSILE_GetTotalFrames(pMissile);
    if (nFrames >= 78)
    {
        nFrames = 77;
        MISSILE_SetTotalFrames(pMissile, 77);
        MISSILE_SetCurrentFrame(pMissile, 77);
    }

    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    SEED_InitLowSeed(&pMissile->pSeed, nLowSeed);
    PATH_SetType(pPath, 10);
    PATH_SetNewDistance(pPath, static_cast<uint8_t>(nFrames));
    D2Common_10142(pPath, pMissile, 0);
}

// 1.12a: 0x6FB00410
void __fastcall CLTSKILL_StatRemove_ClearState(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    (void)pStatList;
    STATES_ToggleState(pUnit, nState, FALSE);
}

// ============================================================================
// Section 4: Assassin & Shared Progressive / Charge-Up / Sentry Client Skills
//            (0x6FB00420 .. 0x6FB01BB9)
// ============================================================================

// 1.12a: 0x6FB00420 (gpfnCltStFunc[7])
int32_t __fastcall CLTSKILL_St07_CloakOfShadows(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt || !sgptDataTables)
    {
        return 0;
    }

    const int16_t nAuraState = pSkillsTxt->nAuraState;
    if (nAuraState < 0 || nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    return STATES_CheckState(pUnit, nAuraState) ? 0 : 1;
}

// 1.12a: 0x6FB00480
void __fastcall CLTSKILL_MissileInit_CustomChargedBoltWiggle(D2UnitStrc* pMissile, int32_t nLowSeed)
{
    if (!pMissile)
    {
        return;
    }

    int32_t nFrames = MISSILE_GetTotalFrames(pMissile);
    if (nFrames >= 78)
    {
        nFrames = 77;
        MISSILE_SetTotalFrames(pMissile, 77);
        MISSILE_SetCurrentFrame(pMissile, 77);
    }

    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    SEED_InitLowSeed(&pMissile->pSeed, nLowSeed);
    PATH_SetType(pPath, 10);
    PATH_SetNewDistance(pPath, static_cast<uint8_t>(nFrames));

    D2PathPointStrc* pPathPoints = nullptr;
    PATH_GetPathPoints(pPath, &pPathPoints);

    int32_t nCurX = CLTSKILL_GetUnitX(pMissile);
    int32_t nCurY = CLTSKILL_GetUnitY(pMissile);

    const int32_t nFirstY = D2COMMON_10176_PathGetFirstPointY(pMissile->pDynamicPath);
    const int32_t nFirstX = D2COMMON_10175_PathGetFirstPointX(pMissile->pDynamicPath);
    int32_t nDir64 = static_cast<uint8_t>(UNITS_GetDirectionToCoords(pMissile, nFirstX, nFirstY));

    const uint32_t dwSignRoll = static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed));
    int32_t nTurnSign = ((dwSignRoll & 1u) != 0) ? 1 : -1;

    const uint32_t dwStepRoll = static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed));
    int32_t nTurnStep = static_cast<int32_t>(dwStepRoll % 3u) + g_nChargedBoltBaseTurnStep;

    for (int32_t i = 0; i < nFrames; ++i)
    {
        nDir64 += nTurnStep * nTurnSign;
        const uint8_t nOctant = D2Common_11053(static_cast<uint8_t>(nDir64 & 0x3F));

        nCurX += g_anChargedBoltOctantStepX[nOctant];
        nCurY += g_anChargedBoltOctantStepY[nOctant];

        pPathPoints[i].X = static_cast<uint16_t>(nCurX);
        pPathPoints[i].Y = static_cast<uint16_t>(nCurY);

        if ((i % g_nChargedBoltTurnFlipInterval) == 0)
        {
            nTurnSign = -nTurnSign;
            const uint32_t dwNewRoll = static_cast<uint32_t>(SEED_RollRandomNumber(&pMissile->pSeed));
            nTurnStep = static_cast<int32_t>(dwNewRoll % 3u) + g_nChargedBoltBaseTurnStep;
        }
    }

    PATH_SetNumberOfPathPoints(pPath, nFrames);
}

// 1.12a: 0x6FB00680 (gpfnCltStFunc[10])
int32_t __fastcall CLTSKILL_St10_ValidPlayerOrMonsterTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    return (pTarget->dwUnitType == UNIT_MONSTER || pTarget->dwUnitType == UNIT_PLAYER) ? 1 : 0;
}

// 1.12a: 0x6FB006E0 (gpfnCltStFunc[9])
int32_t __fastcall CLTSKILL_St09_VelocityPercent(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    const int32_t nAttackRateBonus = pSkillsTxt ? pSkillsTxt->dwParam[3] : 0;
    CLTSKILL_PostTempAttackRateStatList(pUnit, nAttackRateBonus);
    return 1;
}

// 1.12a: 0x6FB00730
int32_t __fastcall CLTSKILL_EvalProgressiveCalcByChargeCount(
    D2UnitStrc* pUnit,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    uint32_t dwCalcFormula = pSkillsTxt->dwPrgCalc[0];
    if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_PROGRESSIVE) != 0 && sgptDataTables)
    {
        const int16_t nAuraState = pSkillsTxt->nAuraState;
        const int16_t nAuraStat  = pSkillsTxt->wAuraStat[0];
        if (nAuraState >= 0
            && nAuraState < sgptDataTables->nStatesTxtRecordCount
            && nAuraStat >= 0
            && nAuraStat < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nAuraState);
            if (pStatList)
            {
                int32_t nCharges = STATLIST_GetStatValue(pStatList, nAuraStat, 0);
                if (nCharges <= 1)
                {
                    nCharges = 1;
                }
                else if (nCharges >= 3)
                {
                    nCharges = 3;
                }
                dwCalcFormula = pSkillsTxt->dwPrgCalc[nCharges - 1];
            }
        }
    }

    return SKILLS_EvaluateSkillFormula(pUnit, dwCalcFormula, nSkillId, nSkillLevel);
}

// 1.12a: 0x6FB00830
int32_t __fastcall CLTSKILL_ResolveProgressiveMissileId(D2UnitStrc* pUnit, int32_t nSkillId)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return -1;
    }

    if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_PROGRESSIVE) != 0 && sgptDataTables)
    {
        const int16_t nAuraState = pSkillsTxt->nAuraState;
        const int16_t nAuraStat  = pSkillsTxt->wAuraStat[0];
        if (nAuraState >= 0
            && nAuraState < sgptDataTables->nStatesTxtRecordCount
            && nAuraStat >= 0
            && nAuraStat < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nAuraState);
            if (pStatList)
            {
                int32_t nCharges = STATLIST_GetStatValue(pStatList, nAuraStat, 0);
                if (nCharges <= 1)
                {
                    return static_cast<int16_t>(pSkillsTxt->wCltMissileA);
                }
                if (nCharges == 2)
                {
                    return static_cast<int16_t>(pSkillsTxt->wCltMissileB);
                }
                return static_cast<int16_t>(pSkillsTxt->wCltMissileC);
            }
        }
    }

    return static_cast<int16_t>(pSkillsTxt->wCltMissileA);
}

// 1.12a: 0x6FB008F0 (gpfnCltDoFunc[4])
int32_t __fastcall CLTSKILL_Do004_DragonTalon(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);
    if (!PATH_GetTargetUnit(pUnit->pDynamicPath))
    {
        return 0;
    }

    const int32_t nRemainingKicks = SKILLS_GetParam1(pSkill) - 1;
    if (nRemainingKicks > 0)
    {
        SKILLS_SetParam1(pSkill, nRemainingKicks);
        pUnit->nSeqCurrentFramePrecise = 0;
    }

    return 1;
}

// 1.12a: 0x6FB00970 (gpfnCltStFunc[6])
int32_t __fastcall CLTSKILL_St06_DragonTalon(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    if (!pTarget)
    {
        SKILLS_SetParam1(pSkill, 0);
        return 1;
    }

    const int32_t nKickCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    SKILLS_SetParam1(pSkill, nKickCount);
    return 1;
}

// 1.12a: 0x6FB00A40 (gpfnCltDoFunc[14])
int32_t __fastcall CLTSKILL_Do014_ProgressiveCoordMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_GetSkillsTxt(nSkillId))
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (nMissileId < 0)
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    CLTSKILL_SpawnTargetedMissileWithin100Subtiles(pUnit, nMissileId, nSkillId, nSkillLevel);
    return 1;
}

// 1.12a: 0x6FB00AD0 (gpfnCltDoFunc[10])
int32_t __fastcall CLTSKILL_Do010_ProgressiveTargetNova(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_GetSkillsTxt(nSkillId))
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (nMissileId < 0)
    {
        return 0;
    }

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    const int32_t nVelocity = DATATBLS_GetMissileVelocityFromMissilesTxt(nMissileId, nSkillLevel);
    CLTSKILL_SpawnNovaMissilesFromUnit(pTarget, nVelocity, pUnit, nMissileId, -1, nSkillId, nSkillLevel);
    return 1;
}

// 1.12a: 0x6FB00B90 (gpfnCltDoFunc[7])
int32_t __fastcall CLTSKILL_Do007_ProgressiveTargetPointMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    if (!PATH_GetTargetUnit(pUnit->pDynamicPath))
    {
        return 0;
    }

    if (!CLTSKILL_GetSkillsTxt(nSkillId))
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (nMissileId < 0)
    {
        return 0;
    }

    CLTSKILL_SpawnTargetedMissileWithin100Subtiles(pUnit, nMissileId, nSkillId, nSkillLevel);
    pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    return 1;
}

// 1.12a: 0x6FB00C30 (gpfnCltStFunc[5])
int32_t __fastcall CLTSKILL_St05_HostileNonTownTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillId;
    (void)nSkillLevel;

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    if (DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)) || DUNGEON_IsRoomInTown(UNITS_GetRoom(pTarget)))
    {
        return 0;
    }

    const int32_t nTargetType = static_cast<int32_t>(pTarget->dwUnitType);
    if (nTargetType < UNIT_PLAYER || nTargetType > UNIT_MONSTER)
    {
        return 0;
    }

    return D2Client_CanUnitAttackTarget_6FACF980(pTarget, pUnit) ? 1 : 0;
}

// 1.12a: 0x6FB00CC0 (gpfnCltDoFunc[3])
int32_t __fastcall CLTSKILL_Do003_PsychicHammer(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_St05_HostileNonTownTarget(pUnit, nSkillId, nSkillLevel))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    return 1;
}

// 1.12a: 0x6FB00CF0 (gpfnCltDoFunc[13])
int32_t __fastcall CLTSKILL_Do013_ProgressiveChargeSelfOverlay(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    int32_t nChargeOffset = 0;
    if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_PROGRESSIVE) != 0 && sgptDataTables)
    {
        const int16_t nAuraState = pSkillsTxt->nAuraState;
        const int16_t nAuraStat  = pSkillsTxt->wAuraStat[0];
        if (nAuraState >= 0
            && nAuraState < sgptDataTables->nStatesTxtRecordCount
            && nAuraStat >= 0
            && nAuraStat < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nAuraState);
            if (pStatList)
            {
                int32_t nCharges = STATLIST_GetStatValue(pStatList, nAuraStat, 0);
                if (nCharges <= 1)
                {
                    nCharges = 1;
                }
                else if (nCharges >= 3)
                {
                    nCharges = 3;
                }
                nChargeOffset = nCharges - 1;
            }
        }
    }

    const int32_t nOverlayId = static_cast<int16_t>(pSkillsTxt->wPrgOverlay) + nChargeOffset;
    const int32_t nSoundId   = static_cast<int16_t>(pSkillsTxt->wPrgSound) + nChargeOffset;

    D2Client_AttachUnitOverlay_6FB430E0(pUnit, nOverlayId, 2, 0, 0, 0, 0, 0);
    D2Client_PlaySoundEx_6FAE1FA0(pUnit, nSoundId, 0, 0, 0);
    return 1;
}

// 1.12a: 0x6FB00DE0 (gpfnCltDoFunc[12])
int32_t __fastcall CLTSKILL_Do012_ProgressiveChargeTargetOverlay(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    int32_t nChargeOffset = 0;
    if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_PROGRESSIVE) != 0 && sgptDataTables)
    {
        const int16_t nAuraState = pSkillsTxt->nAuraState;
        const int16_t nAuraStat  = pSkillsTxt->wAuraStat[0];
        if (nAuraState >= 0
            && nAuraState < sgptDataTables->nStatesTxtRecordCount
            && nAuraStat >= 0
            && nAuraStat < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nAuraState);
            if (pStatList)
            {
                int32_t nCharges = STATLIST_GetStatValue(pStatList, nAuraStat, 0);
                if (nCharges <= 1)
                {
                    nCharges = 1;
                }
                else if (nCharges >= 3)
                {
                    nCharges = 3;
                }
                nChargeOffset = nCharges - 1;
            }
        }
    }

    const int32_t nOverlayId = static_cast<int16_t>(pSkillsTxt->wPrgOverlay) + nChargeOffset;
    const int32_t nSoundId   = static_cast<int16_t>(pSkillsTxt->wPrgSound) + nChargeOffset;

    D2Client_AttachUnitOverlay_6FB430E0(pTarget, nOverlayId, 2, 0, 0, 0, 0, 0);
    D2Client_PlaySoundEx_6FAE1FA0(pUnit, nSoundId, 0, 0, 0);
    return 1;
}

// 1.12a: 0x6FB00F20
int32_t __fastcall CLTSKILL_StartChanneledInfernoState(D2UnitStrc* pUnit, int32_t nSkillId)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    if (!STATES_CheckState(pUnit, STATE_INFERNO))
    {
        const int32_t nPrgOverlay = static_cast<int16_t>(pSkillsTxt->wPrgOverlay);
        if (nPrgOverlay > 0 && sgptDataTables && nPrgOverlay < sgptDataTables->nOverlayTxtRecordCount)
        {
            D2Client_AttachUnitOverlay_6FB430E0(pUnit, nPrgOverlay, 2, 0, 0, 0, 0, 0);
        }

        const int16_t nPrgSound = static_cast<int16_t>(pSkillsTxt->wPrgSound);
        if (nPrgSound > 0)
        {
            D2Client_PlaySoundEx_6FAE1FA0(nullptr, nPrgSound, 0, 0, 0);
        }
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_INFERNO);
    if (!pStatList)
    {
        const int32_t nUnitType    = pUnit ? static_cast<int32_t>(pUnit->dwUnitType) : 6;
        const D2UnitGUID dwUnitGUID = pUnit ? pUnit->dwUnitId : D2UnitInvalidGUID;

        D2StatListStrc* pNewStatList = STATLIST_AllocStatList(nullptr, 0x02u, 20u, nUnitType, dwUnitGUID);
        if (!pNewStatList)
        {
            return 0;
        }

        D2SkillStrc* pBestSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
        if (!pBestSkill)
        {
            return 0;
        }

        SKILLS_SetParam1(pBestSkill, 0);
        D2COMMON_10475_PostStatToStatList(pUnit, pNewStatList, TRUE);
        STATLIST_SetStatRemoveCallback(pNewStatList, CLTSKILL_StatRemove_ClearState);
        STATLIST_SetState(pNewStatList, STATE_INFERNO);
        STATES_ToggleState(pUnit, STATE_INFERNO, TRUE);
        return 1;
    }

    UNITS_SetAnimationFrame(pUnit, static_cast<int32_t>(pSkillsTxt->nSeqInput) - 1);
    pUnit->dwFrameCountPrecise = 0x800;
    STATLIST_SetExpireFrame(pStatList, 7);
    return 1;
}

// 1.12a: 0x6FB01060 (gpfnCltStFunc[8])
int32_t __fastcall CLTSKILL_St08_BladeFury(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;
    return CLTSKILL_StartChanneledInfernoState(pUnit, nSkillId);
}

// 1.12a: 0x6FB01070
void __fastcall CLTSKILL_SpawnRandomScatterMissilesWithPackedDelta(
    int32_t nCount,
    int32_t nLowSeed,
    D2UnitStrc* pOwner,
    int32_t nX,
    int32_t nY,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nMissileId)
{
    D2SeedStrc seed = {};
    SEED_InitLowSeed(&seed, nLowSeed);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x03u;
    missileParams.pOwner      = pOwner;
    missileParams.nMissile    = nMissileId;
    missileParams.nX          = nX;
    missileParams.nY          = nY;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    for (int32_t i = 0; i < nCount; ++i)
    {
        int32_t nDeltaX = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&seed)) % 40u) - 20;
        int32_t nDeltaY = static_cast<int32_t>(static_cast<uint32_t>(SEED_RollRandomNumber(&seed)) % 40u) - 20;
        if (nDeltaX == 0 && nDeltaY == 0)
        {
            nDeltaX = 20;
        }

        missileParams.nTargetX = nDeltaX;
        missileParams.nTargetY = nDeltaY;

        D2UnitStrc* pMissile = D2Client_CreateClientMissile_6FAD4B20(&missileParams);
        if (pMissile)
        {
            MISSILE_SetTargetX(pMissile, static_cast<int32_t>(SEED_GetLowSeed(&seed)));
            const int32_t nPackedDelta = (static_cast<int32_t>(static_cast<uint16_t>(nDeltaY)) << 16)
                                       + static_cast<int32_t>(static_cast<uint16_t>(nDeltaX));
            MISSILE_SetTargetY(pMissile, nPackedDelta);
        }
    }
}

// 1.12a: 0x6FB01180
void __fastcall CLTSKILL_SpawnChargedBoltRingWithStep(
    int32_t nSkillId,
    int32_t nLowSeed,
    D2UnitStrc* pOwner,
    int32_t nMissileId,
    int32_t nX,
    int32_t nY,
    int32_t nSkillLevel,
    int32_t nDirStep)
{
    D2SeedStrc seed = {};
    SEED_InitLowSeed(&seed, nLowSeed);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x03u;
    missileParams.pOwner      = pOwner;
    missileParams.nMissile    = nMissileId;
    missileParams.nX          = nX;
    missileParams.nY          = nY;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.pInitFunc   = CLTSKILL_MissileInit_CustomChargedBoltWiggle;

    for (int32_t nDirIdx = 0; nDirIdx < 64; nDirIdx += nDirStep)
    {
        missileParams.nTargetX  = g_anNovaDirOffsetY_30[nDirIdx];
        missileParams.nTargetY  = g_anNovaDirOffsetX_30[nDirIdx];
        missileParams.pInitArgs = static_cast<int32_t>(SEED_RollRandomNumber(&seed));

        D2UnitStrc* pMissile = D2Client_CreateClientMissile_6FAD4B20(&missileParams);
        D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
        const int32_t nBoltState = (pSkillsTxt ? pSkillsTxt->dwParam[1] : 0) + 1;
        MISSILE_SetTargetX(pMissile, nBoltState);
    }
}

// 1.12a: 0x6FB01280
void __fastcall CLTSKILL_SpawnRadialRingWithPath10(
    int32_t nSkillId,
    int32_t nLowSeed,
    D2UnitStrc* pOwner,
    int32_t nMissileId,
    int32_t nX,
    int32_t nY,
    int32_t nSkillLevel,
    int32_t nDirStep)
{
    D2SeedStrc seed = {};
    SEED_InitLowSeed(&seed, nLowSeed);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x03u;
    missileParams.pOwner      = pOwner;
    missileParams.nMissile    = nMissileId;
    missileParams.nX          = nX;
    missileParams.nY          = nY;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.pInitFunc   = CLTSKILL_MissileInit_ChargedBoltPath10;

    for (int32_t nDirIdx = 0; nDirIdx < 64; nDirIdx += nDirStep)
    {
        missileParams.nTargetX  = g_anNovaDirOffsetY_30[nDirIdx];
        missileParams.nTargetY  = g_anNovaDirOffsetX_30[nDirIdx];
        missileParams.pInitArgs = static_cast<int32_t>(SEED_RollRandomNumber(&seed));

        D2UnitStrc* pMissile = D2Client_CreateClientMissile_6FAD4B20(&missileParams);
        D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
        const int32_t nBoltState = (pSkillsTxt ? pSkillsTxt->dwParam[1] : 0) + 1;
        MISSILE_SetTargetX(pMissile, nBoltState);
    }
}

// 1.12a: 0x6FB01380
void __fastcall CLTSKILL_SpawnCircularGroundPatchMissiles(
    D2UnitStrc* pOwner,
    int32_t nLowSeed,
    int32_t nMissileId,
    int32_t nCenterX,
    int32_t nCenterY,
    int32_t nRadius,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    D2SeedStrc seed = {};
    SEED_InitLowSeed(&seed, nLowSeed);

    const int32_t nRadiusSq = nRadius * nRadius;
    const int32_t nDiameter = nRadius * 2;

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x01u;
    missileParams.pOwner      = pOwner;
    missileParams.nMissile    = nMissileId;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    D2ActiveRoomStrc* pOriginRoom = UNITS_GetRoom(pOwner);

    for (int32_t i = 0; i < nRadiusSq; ++i)
    {
        const int32_t nRandX = (nDiameter > 0) ? SEED_RollLimitedRandomNumber(&seed, nDiameter) : 0;
        const int32_t nOffX  = nRadius - nRandX;

        const int32_t nRandY = (nDiameter > 0) ? SEED_RollLimitedRandomNumber(&seed, nDiameter) : 0;
        const int32_t nOffY  = nRadius - nRandY;

        if (nOffX * nOffX + nOffY * nOffY <= nRadiusSq)
        {
            const int32_t nSpawnX = nCenterX + nOffX;
            const int32_t nSpawnY = nCenterY + nOffY;
            missileParams.nX = nSpawnX;
            missileParams.nY = nSpawnY;

            if (D2Client_FindRoomCoveringCoords_6FAB1A00(pOriginRoom, nSpawnX, nSpawnY))
            {
                D2Client_CreateClientMissile_6FAD4B20(&missileParams);
            }
        }
    }
}

// 1.12a: 0x6FB01510 (gpfnCltDoFunc[8])
int32_t __fastcall CLTSKILL_Do008_StreamSentryAtTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    const int16_t nSentryMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    const int16_t nStreamMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (nSentryMissileId < 0 || nStreamMissileId < 0)
    {
        return 0;
    }

    const int32_t nStreamRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x01u;
    missileParams.pOwner      = pUnit;
    missileParams.nMissile    = nSentryMissileId;
    missileParams.nX          = nTargetX;
    missileParams.nY          = nTargetY;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile_6FAD4B20(&missileParams);
    if (pMissile)
    {
        MISSILE_SetStreamMissile(pMissile, static_cast<uint16_t>(nStreamMissileId));
        MISSILE_SetStreamRange(pMissile, static_cast<int16_t>(nStreamRange));
    }

    return 1;
}

// 1.12a: 0x6FB01620 (gpfnCltDoFunc[6])
int32_t __fastcall CLTSKILL_Do006_BladeFury(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (nMissileId < 0)
    {
        return 0;
    }

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nFirePeriod = CLTSKILL_EvalProgressiveCalcByChargeCount(pUnit, nSkillId, nSkillLevel);
    if (nFirePeriod <= 0)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags &= ~UNITFLAG_SKSRVDOFUNC;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    const int32_t nCooldownFrames = SKILLS_GetParam1(pSkill);
    if (nCooldownFrames <= 0)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags     = 0x20u;
        missileParams.pOwner      = pUnit;
        missileParams.pOrigin     = pUnit;
        missileParams.nMissile    = nMissileId;
        missileParams.nTargetX    = nTargetX;
        missileParams.nTargetY    = nTargetY;
        missileParams.nSkill      = nSkillId;
        missileParams.nSkillLevel = nSkillLevel;

        D2UnitStrc* pMissile = D2Client_CreateClientMissile_6FAD4B20(&missileParams);
        if (pMissile)
        {
            D2Client_SetUnitDrawElevationOffset_6FB755B0(pMissile, 0, 0, 15);
        }

        SKILLS_SetParam1(pSkill, nFirePeriod);
    }
    else
    {
        SKILLS_SetParam1(pSkill, nCooldownFrames - 1);
    }

    if (STATES_CheckState(pUnit, STATE_INFERNO))
    {
        UNITS_SetAnimationFrame(pUnit, static_cast<int32_t>(pSkillsTxt->nSeqInput) - 1);
        pUnit->dwFrameCountPrecise = 0x800;
    }

    return 1;
}

// 1.12a: 0x6FB01770 (gpfnCltDoFunc[15])
int32_t __fastcall CLTSKILL_Do015_ProgressiveRandomScatter(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_GetSkillsTxt(nSkillId))
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (nMissileId < 0)
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    const int32_t nCount = CLTSKILL_EvalProgressiveCalcByChargeCount(pUnit, nSkillId, nSkillLevel);
    if (nCount == 0)
    {
        return 0;
    }

    CLTSKILL_SpawnRandomScatterMissilesWithPackedDelta(
        nCount,
        nTargetX,
        pUnit,
        nTargetX,
        nTargetY,
        nSkillId,
        nSkillLevel,
        nMissileId);
    return 1;
}

// 1.12a: 0x6FB01820 (gpfnCltDoFunc[83])
int32_t __fastcall CLTSKILL_Do083_ProgressiveChargedBoltRing(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (nMissileId < 0)
    {
        return 0;
    }

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    int32_t nDirStep = CLTSKILL_EvalProgressiveCalcByChargeCount(pUnit, nSkillId, nSkillLevel);
    if (nDirStep <= 0)
    {
        nDirStep = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel);
    }

    const int32_t nLowSeed = static_cast<int32_t>(pUnit->dwInitSeed);
    const int32_t nTargetX = CLTSKILL_GetUnitX(pTarget);
    const int32_t nTargetY = CLTSKILL_GetUnitY(pTarget);

    CLTSKILL_SpawnChargedBoltRingWithStep(
        nSkillId,
        nLowSeed,
        pUnit,
        nMissileId,
        nTargetX,
        nTargetY,
        nSkillLevel,
        nDirStep);
    return 1;
}

// 1.12a: 0x6FB01910 (gpfnCltDoFunc[11])
int32_t __fastcall CLTSKILL_Do011_ProgressiveRadialRing(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (nMissileId < 0)
    {
        return 0;
    }

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    int32_t nDirStep = CLTSKILL_EvalProgressiveCalcByChargeCount(pUnit, nSkillId, nSkillLevel);
    if (nDirStep <= 0)
    {
        nDirStep = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel);
    }

    const int32_t nLowSeed = static_cast<int32_t>(pUnit->dwInitSeed);
    const int32_t nTargetX = CLTSKILL_GetUnitX(pTarget);
    const int32_t nTargetY = CLTSKILL_GetUnitY(pTarget);

    CLTSKILL_SpawnRadialRingWithPath10(
        nSkillId,
        nLowSeed,
        pUnit,
        nMissileId,
        nTargetX,
        nTargetY,
        nSkillLevel,
        nDirStep);
    return 1;
}

// 1.12a: 0x6FB01A00 (gpfnCltDoFunc[9])
int32_t __fastcall CLTSKILL_Do009_ProgressiveGroundPatchCircle(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (nMissileId < 0)
    {
        return 0;
    }

    int32_t nRadius = CLTSKILL_EvalProgressiveCalcByChargeCount(pUnit, nSkillId, nSkillLevel);
    if (nRadius <= 0)
    {
        nRadius = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel);
    }

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    int32_t nCenterX = 0;
    int32_t nCenterY = 0;
    if (pTarget)
    {
        nCenterX = CLTSKILL_GetUnitX(pTarget);
        nCenterY = CLTSKILL_GetUnitY(pTarget);
    }
    else if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nCenterX, &nCenterY))
    {
        return 0;
    }

    CLTSKILL_SpawnCircularGroundPatchMissiles(
        pUnit,
        nCenterX,
        nMissileId,
        nCenterX,
        nCenterY,
        nRadius,
        nSkillId,
        nSkillLevel);
    return 1;
}

// 1.12a: 0x6FB01B10 (gpfnCltDoFunc[5])
int32_t __fastcall CLTSKILL_Do005_ProgressiveLobRain(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (nMissileId < 0)
    {
        return 0;
    }

    const int32_t nDropCount = CLTSKILL_EvalProgressiveCalcByChargeCount(pUnit, nSkillId, nSkillLevel);
    if (nDropCount <= 0)
    {
        return 0;
    }

    const int32_t nRadius = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel);
    CLTSKILL_SpawnSeededRandomLobRain(pUnit, nMissileId, nDropCount, nRadius, nSkillId, nSkillLevel);
    return 1;
}

// ============================================================================
// Section 5: Sorceress Client Skill Start / Do / Event Handlers
//            (0x6FB01BB0 .. 0x6FB02DC5)
// ============================================================================

// 1.12a: 0x6FB01BB0
void __fastcall CLTSKILL_StatRemove_ClearInfernoState(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    (void)pStatList;
    STATES_ToggleState(pUnit, nState, FALSE);
}

// 1.12a: 0x6FB01BC0
void __fastcall CLTSKILL_MissileInit_ChargedBoltFromFirstPoint(D2UnitStrc* pMissile, int32_t nSeedOffset)
{
    if (!pMissile)
    {
        return;
    }

    int32_t nFrames = MISSILE_GetTotalFrames(pMissile);
    if (nFrames >= 78)
    {
        nFrames = 77;
        MISSILE_SetTotalFrames(pMissile, 77);
        MISSILE_SetCurrentFrame(pMissile, 77);
    }

    D2DynamicPathStrc* pPath = pMissile->pDynamicPath;
    const int32_t nLowSeed = D2COMMON_10175_PathGetFirstPointX(pPath) + nSeedOffset;
    SEED_InitLowSeed(&pMissile->pSeed, nLowSeed);
    PATH_SetType(pPath, 10);
    PATH_SetNewDistance(pPath, static_cast<uint8_t>(nFrames));
    D2Common_10142(pPath, pMissile, 0);
}

// 1.12a: 0x6FB01C20
void __fastcall CLTSKILL_ScaleMissileAnimSpeedToRange(D2UnitStrc* pMissile, int32_t nRangeFrames)
{
    if (!pMissile || pMissile->dwUnitType != UNIT_MISSILE || nRangeFrames == 0)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxt = CLTSKILL_GetMissilesTxt(pMissile->dwClassId);
    if (!pMissilesTxt)
    {
        return;
    }

    const int32_t nSpeed = (static_cast<int32_t>(pMissilesTxt->nAnimLen) << 8) / nRangeFrames;
    UNITS_SetAnimationSpeed(pMissile, nSpeed);
}

// 1.12a: 0x6FB01C70 (gpfnCltStFunc[17])
int32_t __fastcall CLTSKILL_St17_Teleport_CheckTown(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2ActiveRoomStrc* pOriginRoom = UNITS_GetRoom(pUnit);
    if (!pOriginRoom)
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    D2ActiveRoomStrc* pDestRoom = D2Client_FindRoomCoveringCoords_6FAB1A00(pOriginRoom, nTargetX, nTargetY);
    if (!pDestRoom)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_INTOWN) == 0 && DUNGEON_IsRoomInTown(pDestRoom))
    {
        return 0;
    }

    return 1;
}

// 1.12a: 0x6FB01D10 (gpfnCltDoFunc[29])
int32_t __fastcall CLTSKILL_Do029_LinearSkillMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_GetSkillsTxt(nSkillId))
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    return CLTSKILL_SpawnLinearMissileToResolvedTarget(pUnit, nMissileId, nSkillId, nSkillLevel) ? 1 : 0;
}

// 1.12a: 0x6FB01D90 (gpfnCltDoFunc[28])
int32_t __fastcall CLTSKILL_Do028_CollisionCheckedTargetMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!CLTSKILL_GetSkillsTxt(nSkillId))
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    if (!CLTSKILL_CanSpawnMissileAtTargetCoords(nMissileId, pUnit))
    {
        return 0;
    }

    return CLTSKILL_SpawnTargetedMissileWithin100Subtiles(pUnit, nMissileId, nSkillId, nSkillLevel) ? 1 : 0;
}

// 1.12a: 0x6FB01E20 (gpfnCltDoFunc[27])
int32_t __fastcall CLTSKILL_Do027_ChainLightning(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nMaxBounces = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    D2UnitStrc* pMissile = CLTSKILL_SpawnLinearMissileToResolvedTarget(pUnit, nMissileId, nSkillId, nSkillLevel);
    if (!pMissile)
    {
        return 0;
    }

    MISSILE_SetTargetX(pMissile, nMaxBounces);
    return 1;
}

// 1.12a: 0x6FB01ED0 (gpfnCltDoFunc[25])
int32_t __fastcall CLTSKILL_Do025_Nova_FrostNova(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nBaseVelocity  = DATATBLS_GetMissileVelocityFromMissilesTxt(nMissileId, nSkillLevel);
    const int32_t nBonusVelocity = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);

    CLTSKILL_SpawnNovaMissilesFromUnit(
        pUnit,
        nBaseVelocity + nBonusVelocity,
        pUnit,
        nMissileId,
        -1,
        nSkillId,
        nSkillLevel);
    return 1;
}

// 1.12a: 0x6FB01F90 (gpfnCltStFunc[16])
int32_t __fastcall CLTSKILL_St16_Telekinesis(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    D2Client_EnsureUnitPathAllocated_6FAF0D20(pUnit);
    D2_ASSERT(pUnit->pDynamicPath != nullptr);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (!pTarget)
    {
        return 0;
    }

    const int32_t nMaxRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwAuraRangeCalc, nSkillId, nSkillLevel);
    const int32_t nSqDist   = PATH_ComputeSquaredDistance(
        CLTSKILL_GetUnitX(pUnit),
        CLTSKILL_GetUnitY(pUnit),
        CLTSKILL_GetUnitX(pTarget),
        CLTSKILL_GetUnitY(pTarget));

    if (nSqDist > nMaxRange * nMaxRange)
    {
        return 0;
    }

    const int32_t nTargetType = static_cast<int32_t>(pTarget->dwUnitType);
    if (nTargetType >= UNIT_PLAYER && nTargetType <= UNIT_MONSTER)
    {
        D2ActiveRoomStrc* pCasterRoom = UNITS_GetRoom(pUnit);
        if (!pCasterRoom || DUNGEON_IsRoomInTown(pCasterRoom))
        {
            return 0;
        }

        D2ActiveRoomStrc* pTargetRoom = UNITS_GetRoom(pTarget);
        if (!pTargetRoom || DUNGEON_IsRoomInTown(pTargetRoom))
        {
            return 0;
        }

        if (!D2Client_CanUnitAttackTarget_6FACF980(pTarget, pUnit))
        {
            return 0;
        }
    }

    return 1;
}

// 1.12a: 0x6FB02150 (gpfnCltStFunc[15])
int32_t __fastcall CLTSKILL_St15_Inferno_ArcticBlast(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    (void)nSkillLevel;

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    if (!STATES_CheckState(pUnit, STATE_INFERNO))
    {
        const int32_t nPrgOverlay = static_cast<int16_t>(pSkillsTxt->wPrgOverlay);
        if (nPrgOverlay > 0 && sgptDataTables && nPrgOverlay < sgptDataTables->nOverlayTxtRecordCount)
        {
            D2Client_AttachUnitOverlay_6FB430E0(pUnit, nPrgOverlay, 2, 0, 0, 0, 0, 0);
        }

        const int16_t nPrgSound = static_cast<int16_t>(pSkillsTxt->wPrgSound);
        if (nPrgSound > 0)
        {
            D2Client_PlaySoundEx_6FAE1FA0(nullptr, nPrgSound, 0, 0, 0);
        }
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_INFERNO);
    if (!pStatList)
    {
        const int32_t nUnitType    = pUnit ? static_cast<int32_t>(pUnit->dwUnitType) : 6;
        const D2UnitGUID dwUnitGUID = pUnit ? pUnit->dwUnitId : D2UnitInvalidGUID;

        D2StatListStrc* pNewStatList = STATLIST_AllocStatList(nullptr, 0x02u, 21u, nUnitType, dwUnitGUID);
        if (!pNewStatList)
        {
            return 0;
        }

        D2COMMON_10475_PostStatToStatList(pUnit, pNewStatList, TRUE);
        STATLIST_SetStatRemoveCallback(pNewStatList, CLTSKILL_StatRemove_ClearInfernoState);
        STATLIST_SetState(pNewStatList, STATE_INFERNO);
        STATES_ToggleState(pUnit, STATE_INFERNO, TRUE);
        return 1;
    }

    UNITS_SetAnimationFrame(pUnit, 8);
    pUnit->dwFrameCountPrecise = 0x500;
    STATLIST_SetExpireFrame(pStatList, 7);
    return 1;
}

// 1.12a: 0x6FB02270
int32_t __fastcall CLTSKILL_CheckUnitEventChillingArmor(
    int32_t nEventType,
    D2UnitStrc* pUnit,
    D2UnitStrc* pSourceUnit,
    int32_t nArg1,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    (void)nArg1;

    if (nEventType != 0 || !pUnit || !pSourceUnit)
    {
        return 0;
    }

    D2UnitStrc* pAttacker = D2Client_GetMissileOwnerUnit_6FACF710(pSourceUnit);
    if (!pAttacker)
    {
        return 0;
    }

    if (!D2Client_CanUnitAttackTarget_6FACF980(pAttacker, pUnit))
    {
        return 0;
    }

    D2MissilesTxt* pIncomingMissilesTxt = CLTSKILL_GetMissilesTxt(pSourceUnit->dwClassId);
    if (!pIncomingMissilesTxt || (pIncomingMissilesTxt->dwMissileFlags & MISSILESFLAG_RETURNFIRE) == 0)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nRetaliateMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_GetMissilesTxt(nRetaliateMissileId))
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x20u;
    missileParams.pOwner      = pUnit;
    missileParams.pOrigin     = pUnit;
    missileParams.nMissile    = nRetaliateMissileId;
    missileParams.nTargetX    = CLTSKILL_GetUnitX(pAttacker);
    missileParams.nTargetY    = CLTSKILL_GetUnitY(pAttacker);
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    D2Client_CreateClientMissile_6FAD4B20(&missileParams);

    const int32_t nOverlayId = static_cast<int16_t>(pSkillsTxt->wCltOverlayA);
    if (nOverlayId >= 0 && sgptDataTables && nOverlayId < sgptDataTables->nOverlayTxtRecordCount)
    {
        D2Client_AttachUnitOverlay_6FB430E0(pUnit, nOverlayId, 2, 0, 0, 0, 0, 0);
    }

    return 1;
}

// 1.12a: 0x6FB023B0 (gpfnCltDoFunc[84])
int32_t __fastcall CLTSKILL_Do084_ThunderStormStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    D2UnitStrc* pTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);
    if (!pTarget || !pUnit)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x01u;
    missileParams.pOwner      = pUnit;
    missileParams.nMissile    = nMissileId;
    missileParams.nX          = CLTSKILL_GetUnitX(pTarget);
    missileParams.nY          = CLTSKILL_GetUnitY(pTarget);
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile_6FAD4B20(&missileParams);
    if (pMissile)
    {
        D2Client_SetUnitDrawElevationOffset_6FB755B0(pMissile, 0, 0, 280);
        D2ClientUnitGfxStrc* pUnitGfx = reinterpret_cast<D2ClientUnitGfxStrc*>(pMissile->pGfxData);
        if (pUnitGfx && pUnitGfx->pDrawOffset)
        {
            pUnitGfx->pDrawOffset->nVelXPrecise = 0;
            pUnitGfx->pDrawOffset->nVelYPrecise = 0;
            pUnitGfx->pDrawOffset->nVelZPrecise = -0x14000;
        }
    }

    D2Client_ScreenShake_6FAC3E60(0);

    const int16_t nPrgOverlay = static_cast<int16_t>(pSkillsTxt->wPrgOverlay);
    if (nPrgOverlay > 0)
    {
        D2Client_AttachUnitOverlay_6FB430E0(pTarget, nPrgOverlay, 2, 0, 0, 0, 0, 0);
    }

    return 1;
}

// 1.12a: 0x6FB024E0
void __fastcall CLTSKILL_UpdateBlazeStepTrail(D2UnitStrc* pUnit)
{
    if (!pUnit->pDynamicPath || !D2Common_10234(pUnit->pDynamicPath))
    {
        return;
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_BLAZE);
    if (!pStatList)
    {
        return;
    }

    const int32_t nSkillId = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);
    int32_t nSkillLevel    = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0);
    if (nSkillId <= 0 || !sgptDataTables || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return;
    }

    if (nSkillLevel < 0)
    {
        nSkillLevel = 1;
    }

    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return;
    }

    int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return;
    }

    if (DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
    {
        return;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x21u;
    missileParams.pOwner      = pUnit;
    missileParams.nX          = CLTSKILL_GetUnitX(pUnit);
    missileParams.nY          = CLTSKILL_GetUnitY(pUnit);
    missileParams.nMissile    = nMissileId;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    const int16_t nMissileB = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (nMissileB >= 0)
    {
        const int16_t nMissileC = static_cast<int16_t>(pSkillsTxt->wCltMissileC);
        if (nMissileC >= 0)
        {
            const int32_t nRoll = SEED_RollLimitedRandomNumber(&pUnit->pSeed, 3);
            if (nRoll == 0)
            {
                missileParams.nMissile = nMissileB;
            }
            else if (nRoll == 1)
            {
                missileParams.nMissile = nMissileC;
            }
        }
        else if (SEED_RollLimitedRandomNumber(&pUnit->pSeed, 2) == 0)
        {
            missileParams.nMissile = nMissileB;
        }
    }

    D2Client_CreateClientMissile_6FAD4B20(&missileParams);
}

// 1.12a: 0x6FB02670 (gpfnCltDoFunc[96])
int32_t __fastcall CLTSKILL_Do096_TargetOffsetMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nCalcParam = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);

    int32_t nDeltaX = 0;
    int32_t nDeltaY = 0;
    D2UnitStrc* pTarget = D2Client_GetUnitTargetUnit_6FACF5B0(pUnit);
    if (pTarget)
    {
        nDeltaX = CLTSKILL_GetUnitX(pTarget) - CLTSKILL_GetUnitX(pUnit);
        nDeltaY = CLTSKILL_GetUnitY(pTarget) - CLTSKILL_GetUnitY(pUnit);
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x21u;
    missileParams.pOwner      = pUnit;
    missileParams.nMissile    = nMissileId;
    missileParams.nX          = CLTSKILL_GetUnitX(pUnit) + nDeltaX;
    missileParams.nY          = CLTSKILL_GetUnitY(pUnit) + nDeltaY;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &missileParams.nTargetX, &missileParams.nTargetY))
    {
        return 0;
    }

    if (missileParams.nTargetX == 0 || missileParams.nTargetY == 0)
    {
        return 0;
    }

    D2UnitStrc* pMissile = D2Client_CreateClientMissile_6FAD4B20(&missileParams);
    if (!pMissile)
    {
        return 0;
    }

    MISSILE_SetTargetX(pMissile, nCalcParam);
    return 1;
}

// 1.12a: 0x6FB027D0 (gpfnCltDoFunc[26])
int32_t __fastcall CLTSKILL_Do026_FireWall(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nCenterMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    D2MissilesTxt* pCenterMissilesTxt = CLTSKILL_GetMissilesTxt(nCenterMissileId);
    if (!pCenterMissilesTxt)
    {
        return 0;
    }

    const int32_t nWingMakerMissileId = static_cast<int16_t>(pCenterMissilesTxt->wSubMissile[0]);
    if (!CLTSKILL_GetMissilesTxt(nWingMakerMissileId))
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    if (!CLTSKILL_CanSpawnMissileAtTargetCoords(nCenterMissileId, pUnit))
    {
        return 0;
    }

    D2ActiveRoomStrc* pOriginRoom = UNITS_GetRoom(pUnit);
    if (!pOriginRoom)
    {
        return 0;
    }

    D2ActiveRoomStrc* pDestRoom = D2Client_FindRoomCoveringCoords_6FAB1A00(pOriginRoom, nTargetX, nTargetY);
    if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_INTOWN) == 0 && DUNGEON_IsRoomInTown(pDestRoom))
    {
        return 0;
    }

    const int32_t nVecX = CLTSKILL_GetUnitX(pUnit) - nTargetX;
    const int32_t nVecY = CLTSKILL_GetUnitY(pUnit) - nTargetY;

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x21u;
    missileParams.pOwner      = pUnit;
    missileParams.nMissile    = nWingMakerMissileId;
    missileParams.nX          = nTargetX;
    missileParams.nY          = nTargetY;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    missileParams.nTargetX = nTargetX - nVecY;
    missileParams.nTargetY = nTargetY + nVecX;
    D2Client_CreateClientMissile_6FAD4B20(&missileParams);

    missileParams.nTargetX = nTargetX + nVecY;
    missileParams.nTargetY = nTargetY - nVecX;
    D2Client_CreateClientMissile_6FAD4B20(&missileParams);

    missileParams.dwFlags  = 0x01u;
    missileParams.nMissile = nCenterMissileId;
    missileParams.nTargetX = 0;
    missileParams.nTargetY = 0;
    D2Client_CreateClientMissile_6FAD4B20(&missileParams);

    return 1;
}

// 1.12a: 0x6FB02990
D2UnitStrc* __fastcall CLTSKILL_SpawnInfernoJetSegment(
    D2UnitStrc* pUnit,
    int32_t nSkillId,
    int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return nullptr;
    }

    int32_t nMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileA);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return nullptr;
    }

    int32_t nAltMissileId = static_cast<int16_t>(pSkillsTxt->wCltMissileB);
    if (!CLTSKILL_GetMissilesTxt(nAltMissileId))
    {
        nAltMissileId = -1;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &nTargetX, &nTargetY))
    {
        return nullptr;
    }

    if (nAltMissileId >= 0 && SEED_RollLimitedRandomNumber(&pUnit->pSeed, 2) != 0)
    {
        nMissileId = nAltMissileId;
    }

    int32_t nRangeFrames = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);
    if (nRangeFrames < 1)
    {
        nRangeFrames = 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x8020u;
    missileParams.pOwner      = pUnit;
    missileParams.pOrigin     = pUnit;
    missileParams.nMissile    = nMissileId;
    missileParams.nTargetX    = nTargetX;
    missileParams.nTargetY    = nTargetY;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nRange      = nRangeFrames;

    D2UnitStrc* pMissile = D2Client_CreateClientMissile_6FAD4B20(&missileParams);
    if (pMissile)
    {
        CLTSKILL_ScaleMissileAnimSpeedToRange(pMissile, nRangeFrames);
    }

    if (STATES_CheckState(pUnit, STATE_INFERNO))
    {
        if (pUnit->pAnimSeq)
        {
            UNITS_SetAnimationFrame(pUnit, 8);
            pUnit->dwFrameCountPrecise = 0x500;
        }
        else
        {
            pUnit->nSeqCurrentFramePrecise = (pUnit->nSeqCurrentFramePrecise & ~0xFF) - 0x200;
        }
    }

    return pMissile;
}

// 1.12a: 0x6FB02B20 (gpfnCltDoFunc[23])
int32_t __fastcall CLTSKILL_Do023_ChargedBolt(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nBoltCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCalc[0], nSkillId, nSkillLevel);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags     = 0x21u;
    missileParams.pOwner      = pUnit;
    missileParams.nX          = CLTSKILL_GetUnitX(pUnit);
    missileParams.nY          = CLTSKILL_GetUnitY(pUnit);
    missileParams.nMissile    = nMissileId;
    missileParams.nSkill      = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.pInitFunc   = CLTSKILL_MissileInit_ChargedBoltFromFirstPoint;

    if (!CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, &missileParams.nTargetX, &missileParams.nTargetY))
    {
        return 0;
    }

    for (int32_t i = 0; i < nBoltCount; ++i)
    {
        missileParams.pInitArgs = i;
        D2Client_CreateClientMissile_6FAD4B20(&missileParams);
    }

    return 1;
}

// 1.12a: 0x6FB02C40 (gpfnCltDoFunc[87])
int32_t __fastcall CLTSKILL_Do087_PeriodicAreaLob(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxt = CLTSKILL_GetSkillsTxt(nSkillId);
    if (!pSkillsTxt)
    {
        return 0;
    }

    const int32_t nMissileId = CLTSKILL_ResolveProgressiveMissileId(pUnit, nSkillId);
    if (!CLTSKILL_GetMissilesTxt(nMissileId))
    {
        return 0;
    }

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nPeriod = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[0], nSkillId, nSkillLevel);
    if (nPeriod <= 0)
    {
        return 0;
    }

    const int32_t nRadius = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[1], nSkillId, nSkillLevel);
    if (nRadius <= 0)
    {
        return 0;
    }

    const int32_t nExtraRangeCap = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxt->dwCltCalc[2], nSkillId, nSkillLevel);
    if (nExtraRangeCap <= 0)
    {
        return 0;
    }

    const int32_t nTickCount = SKILLS_GetParam1(pSkill);
    SKILLS_SetParam1(pSkill, nTickCount + 1);

    if ((nTickCount % nPeriod) == 0)
    {
        if (!pUnit)
        {
            return 0;
        }

        const int32_t nSpan    = nRadius * 2 + 1;
        const int32_t nTargetX = CLTSKILL_GetUnitX(pUnit) + (SEED_RollLimitedRandomNumber(&pUnit->pSeed, nSpan) - nRadius);
        const int32_t nTargetY = CLTSKILL_GetUnitY(pUnit) + (SEED_RollLimitedRandomNumber(&pUnit->pSeed, nSpan) - nRadius);
        const int32_t nExtraRange = SEED_RollLimitedRandomNumber(&pUnit->pSeed, nExtraRangeCap);

        D2Client_SpawnLobMissile_6FADB4B0(
            pUnit,
            nMissileId,
            0,
            0,
            nTargetX,
            nTargetY,
            nSkillId,
            nSkillLevel,
            nExtraRange);
    }

    return 1;
}

// 1.12a: 0x6FB02DB0 (gpfnCltDoFunc[24])
int32_t __fastcall CLTSKILL_Do024_Inferno_ArcticBlast(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    return CLTSKILL_SpawnInfernoJetSegment(pUnit, nSkillId, nSkillLevel) ? 1 : 0;
}

// ============================================================================
// Section 6: Belt Bar Hit-Test & Slot Screen Layout (0x6FB02DD0 .. 0x6FB02F04)
// ============================================================================

// 1.12a: 0x6FB02DD0
BOOL __fastcall BELT_IsCursorOverBeltBar(int32_t nCursorX, int32_t nCursorY)
{
    const int32_t nHalfScreenW = g_nScreenWidth / 2;
    if (nCursorX <= nHalfScreenW - 74 || nCursorX >= nHalfScreenW + 71)
    {
        return FALSE;
    }

    if (nCursorY >= g_nScreenHeight - 50 || nCursorY <= g_nScreenHeight - 69)
    {
        return FALSE;
    }

    return TRUE;
}

// 1.12a: 0x6FB02E10
BOOL __cdecl BELT_IsBeltPopupOpen(void)
{
    return (gbBeltPopupOpen != 0) ? TRUE : FALSE;
}

// 1.12a: 0x6FB02E20
void __fastcall BELT_BuildSlotScreenLayout(int32_t nBeltDockMode)
{
    const BOOL bCompactLayout = gbMiniPanelCompactLayout;
    const int32_t nSlotCount  = bCompactLayout ? 7 : 8;

    if (nBeltDockMode == 0)
    {
        return;
    }

    const int32_t nHalfScreenW = g_nScreenWidth / 2;
    int32_t nCurX = bCompactLayout ? (nHalfScreenW - 74) : (nHalfScreenW - 84);
    const int32_t nSlotY = g_nScreenHeight - 50;

    if (nBeltDockMode == 1)
    {
        nCurX = nHalfScreenW - 202;
    }
    else if (nBeltDockMode == 3)
    {
        nCurX = bCompactLayout ? (nHalfScreenW + 57) : (nHalfScreenW + 35);
    }

    gaBeltSlotGridCol[0]   = 0;
    gaBeltSlotFlatIndex[0] = 0;
    gaBeltSlotScreenX[0]   = nCurX;
    gaBeltSlotScreenY[0]   = nSlotY;
    nCurX += 21;

    int32_t nColSkip  = 0;
    int32_t nFlatSkip = 0;

    for (int32_t nSlot = 1; nSlot < nSlotCount; ++nSlot)
    {
        gaBeltSlotScreenX[nSlot] = nCurX;
        gaBeltSlotScreenY[nSlot] = nSlotY;

        int32_t nFlatIdx = 0;
        if (bCompactLayout)
        {
            if (nSlot == 3)
            {
                nFlatSkip += 2;
                ++nColSkip;
            }
            gaBeltSlotGridCol[nSlot] = nColSkip + nSlot;
            nFlatIdx = nFlatSkip + nSlot * 2;
        }
        else
        {
            gaBeltSlotGridCol[nSlot] = nSlot;
            nFlatIdx = nSlot * 2;
        }

        nCurX += 21;
        gaBeltSlotFlatIndex[nSlot] = nFlatIdx;
    }
}

// ============================================================================
// Ready-to-Wire Table Snippet for gpfnCltStFunc[70] & gpfnCltDoFunc[130]
// ============================================================================
#if 0
// gpfnCltStFunc (0x6FBA3510) entries implemented in SKILLS_Part1_CoreAmaSor.cpp:
//   [ 1] = CLTSKILL_St01_Attack,                        // 0x6FAFE0A0
//   [ 2] = CLTSKILL_St02_Throw,                         // 0x6FAFE070
//   [ 3] = CLTSKILL_St03_Unsummon,                      // 0x6FAFDFC0
//   [ 4] = CLTSKILL_St04_Arrow_Bolt,                    // 0x6FAFD880
//   [ 5] = CLTSKILL_St05_HostileNonTownTarget,          // 0x6FB00C30
//   [ 6] = CLTSKILL_St06_DragonTalon,                   // 0x6FB00970
//   [ 7] = CLTSKILL_St07_CloakOfShadows,                // 0x6FB00420
//   [ 8] = CLTSKILL_St08_BladeFury,                     // 0x6FB01060
//   [ 9] = CLTSKILL_St09_VelocityPercent,               // 0x6FB006E0
//   [10] = CLTSKILL_St10_ValidPlayerOrMonsterTarget,    // 0x6FB00680
//   [11] = CLTSKILL_St11_CheckAmmo,                     // 0x6FAFB800
//   [12] = CLTSKILL_St12_Jab,                           // 0x6FAFB7E0
//   [13] = CLTSKILL_St13_Strafe,                        // 0x6FAFBDD0
//   [14] = CLTSKILL_St14_Fend,                          // 0x6FAFBC20
//   [15] = CLTSKILL_St15_Inferno_ArcticBlast,           // 0x6FB02150
//   [16] = CLTSKILL_St16_Telekinesis,                   // 0x6FB01F90
//   [17] = CLTSKILL_St17_Teleport_CheckTown,            // 0x6FB01C70
//   [53] = CLTSKILL_St53_Zeal_Fury,                     // 0x6FAFBA80
//
// gpfnCltDoFunc (0x6FBA3630) entries implemented in SKILLS_Part1_CoreAmaSor.cpp:
//   [  1] = CLTSKILL_Do001_Attack,                          // 0x6FAFF450
//   [  2] = CLTSKILL_Do002_Throw,                           // 0x6FB000B0
//   [  3] = CLTSKILL_Do003_PsychicHammer,                   // 0x6FB00CC0
//   [  4] = CLTSKILL_Do004_DragonTalon,                     // 0x6FB008F0
//   [  5] = CLTSKILL_Do005_ProgressiveLobRain,              // 0x6FB01B10
//   [  6] = CLTSKILL_Do006_BladeFury,                       // 0x6FB01620
//   [  7] = CLTSKILL_Do007_ProgressiveTargetPointMissile,   // 0x6FB00B90
//   [  8] = CLTSKILL_Do008_StreamSentryAtTarget,            // 0x6FB01510
//   [  9] = CLTSKILL_Do009_ProgressiveGroundPatchCircle,    // 0x6FB01A00
//   [ 10] = CLTSKILL_Do010_ProgressiveTargetNova,           // 0x6FB00AD0
//   [ 11] = CLTSKILL_Do011_ProgressiveRadialRing,           // 0x6FB01910
//   [ 12] = CLTSKILL_Do012_ProgressiveChargeTargetOverlay,  // 0x6FB00DE0
//   [ 13] = CLTSKILL_Do013_ProgressiveChargeSelfOverlay,    // 0x6FB00CF0
//   [ 14] = CLTSKILL_Do014_ProgressiveCoordMissile,         // 0x6FB00A40
//   [ 15] = CLTSKILL_Do015_ProgressiveRandomScatter,        // 0x6FB01770
//   [ 16] = CLTSKILL_Do016_Jab,                             // 0x6FAFB810
//   [ 17] = CLTSKILL_Do017_ParallelSpreadMissiles,          // 0x6FAFC670
//   [ 18] = CLTSKILL_Do018_GuidedArrow_BoneSpirit,          // 0x6FAFC510
//   [ 19] = CLTSKILL_Do019_ChargedStrike,                   // 0x6FAFC380
//   [ 20] = CLTSKILL_Do020_Strafe,                          // 0x6FAFC0F0
//   [ 21] = CLTSKILL_Do021_Fend_Zeal_Fury,                  // 0x6FAFB8A0
//   [ 22] = CLTSKILL_Do022_LightningStrike,                 // 0x6FAFBF30
//   [ 23] = CLTSKILL_Do023_ChargedBolt,                     // 0x6FB02B20
//   [ 24] = CLTSKILL_Do024_Inferno_ArcticBlast,             // 0x6FB02DB0
//   [ 25] = CLTSKILL_Do025_Nova_FrostNova,                  // 0x6FB01ED0
//   [ 26] = CLTSKILL_Do026_FireWall,                        // 0x6FB027D0
//   [ 27] = CLTSKILL_Do027_ChainLightning,                  // 0x6FB01E20
//   [ 28] = CLTSKILL_Do028_CollisionCheckedTargetMissile,   // 0x6FB01D90
//   [ 29] = CLTSKILL_Do029_LinearSkillMissile,              // 0x6FB01D10
//   [ 83] = CLTSKILL_Do083_ProgressiveChargedBoltRing,      // 0x6FB01820
//   [ 84] = CLTSKILL_Do084_ThunderStormStrike,              // 0x6FB023B0
//   [ 87] = CLTSKILL_Do087_PeriodicAreaLob,                 // 0x6FB02C40
//   [ 96] = CLTSKILL_Do096_TargetOffsetMissile,             // 0x6FB02670
#endif


// ============================================================================
// 1.12a Live Client Skill Dispatch Tables (0x6FBA3510 & 0x6FBA3630)
// ============================================================================
int32_t __fastcall CLTSKILL_AuraCallback_SelectNextRingTarget_Thunk(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    return CLTSKILL_AuraCallback_SelectNextRingTarget(reinterpret_cast<D2AuraCallbackCtxStrc*>(pUnit), reinterpret_cast<D2UnitStrc*>(nArgEdx));
}

extern int32_t __fastcall CLTSKILL_Do_AndarielSpray(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_ArcaneTower(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_ArcticBlast_Channel(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_Armageddon_Hurricane(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_BaalCorpseExplode(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_BladeFury_RoyalStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_BlessedHammer(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_Charge(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_ChargedBoltSentry_ShockWeb(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_ClearActionFrameAndCheckMetricsA(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_ClearActionFrameAndCheckMetricsB(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_CorpseExplosion(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_Curse(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_DesertTurret_Jump(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_DiabCold(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_DiabLight(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_DiabPrison(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_DiabRun(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_DiabWall(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_DirectionalMissileWithPerpTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_DoomKnightMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_DoubleThrow(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_Eruption_MoltenBoulder(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_FenrisRage(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_FetishAura_Cluster(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_FindItem(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_FindPotion(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_FingerMageSpider(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_Firestorm(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_FistOfTheHeavens(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_GrimWard(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_Hireable_RogueMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_HolyBolt_SmiteMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_HolyFreeze_SanctuaryAura(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_ImpInferno(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_ImpTeleport(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_Leap(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_LeapAttack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_Meteor_Volcano_Catapult(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_MonCurseCast(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_MonInferno(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_MultiHitRollbackLoop(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_NecromageMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_Nest_EvilHutSpawner(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_OverseerWhip(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_PhoenixStrike_FallingMeteorPair(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_PoisonExplosion(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_PrimePoisonNova(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_RaiseSkeleton_Mage(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_ResetSeqSpeed(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_ShockField_Sentry(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_SiegeBeastStomp(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_SpawnCltMissileBAtCaster(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_SpawnPeriodicGore(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_SpawnSavedCoordMissiles(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_SuckBlood(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_SwarmMove_QuickStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_Twister_Tornado(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_UpdateRiderPath(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_VineAttack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_WakeOfDestruction_BaalCold(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_WhirlwindAttack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_Do_WhirlwindEnd(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_AndarielSpray(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_ArcticBlast_Inferno(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_BaalTaunt(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Bash_Stun_Concentrate(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_BoneWall_BonePrison(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Charge(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_ClearUsedSkillFlags(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Corpse_VineCycler(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Curse(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_DiabRun(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Emerge(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_FindItem(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_FindPotion_GrimWard(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Golem(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_HellMeteor(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_IronGolem(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Leap(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_LeapAttack(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_MaggotDown(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_MaggotUp(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_MonInferno(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_MultiHitRollCount(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Nest_EvilHutSpawner(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Poison_CorpseExplosion(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_RaiseSkeleton_Mage(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Raven_DruidSummon(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Revive(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_SaveTargetCoords(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_SpawnCltMissileAAtCaster(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_SpawnMissileTowardTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Submerge_MaggotEgg(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_SwarmMove_QuickStrike(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Vines_MinionSpawner(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Wearwolf_Wearbear(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
extern int32_t __fastcall CLTSKILL_St_Whirlwind(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);

// 1.12a: 0x6FBA3628
extern const int32_t gnCltStFuncs = 70;
// 1.12a: 0x6FBA3510
extern const D2CltSkillFunc_t gpfnCltStFunc[70] = {
    /* [ 0] 0x00000000 */ nullptr,
    /* [ 1] 0x6FAFE0A0 */ CLTSKILL_St01_Attack,
    /* [ 2] 0x6FAFE070 */ CLTSKILL_St02_Throw,
    /* [ 3] 0x6FAFDFC0 */ CLTSKILL_St03_Unsummon,
    /* [ 4] 0x6FAFD880 */ CLTSKILL_St04_Arrow_Bolt,
    /* [ 5] 0x6FB00C30 */ CLTSKILL_St05_HostileNonTownTarget,
    /* [ 6] 0x6FB00970 */ CLTSKILL_St06_DragonTalon,
    /* [ 7] 0x6FB00420 */ CLTSKILL_St07_CloakOfShadows,
    /* [ 8] 0x6FB01060 */ CLTSKILL_St08_BladeFury,
    /* [ 9] 0x6FB006E0 */ CLTSKILL_St09_VelocityPercent,
    /* [10] 0x6FB00680 */ CLTSKILL_St10_ValidPlayerOrMonsterTarget,
    /* [11] 0x6FAFB800 */ CLTSKILL_St11_CheckAmmo,
    /* [12] 0x6FAFB7E0 */ CLTSKILL_St12_Jab,
    /* [13] 0x6FAFBDD0 */ CLTSKILL_St13_Strafe,
    /* [14] 0x6FAFBC20 */ CLTSKILL_St14_Fend,
    /* [15] 0x6FB02150 */ CLTSKILL_St15_Inferno_ArcticBlast,
    /* [16] 0x6FB01F90 */ CLTSKILL_St16_Telekinesis,
    /* [17] 0x6FB01C70 */ CLTSKILL_St17_Teleport_CheckTown,
    /* [18] 0x6FB77EC0 */ CLTSKILL_St_Curse,
    /* [19] 0x6FB78910 */ CLTSKILL_St_BoneWall_BonePrison,
    /* [20] 0x6FB77E30 */ CLTSKILL_St_Poison_CorpseExplosion,
    /* [21] 0x6FB77D50 */ CLTSKILL_St_RaiseSkeleton_Mage,
    /* [22] 0x6FB77CA0 */ CLTSKILL_St_Golem,
    /* [23] 0x6FB77C10 */ CLTSKILL_St_IronGolem,
    /* [24] 0x6FB77F90 */ CLTSKILL_St_Revive,
    /* [25] 0x6FB37E70 */ CLTSKILL_St_Charge,
    /* [26] 0x6FB73E20 */ CLTSKILL_St_FindPotion_GrimWard,
    /* [27] 0x6FB73C90 */ CLTSKILL_St_Bash_Stun_Concentrate,
    /* [28] 0x6FB73DD0 */ CLTSKILL_St_FindItem,
    /* [29] 0x6FB75060 */ CLTSKILL_St_Leap,
    /* [30] 0x6FB74EF0 */ CLTSKILL_St_LeapAttack,
    /* [31] 0x6FB74D10 */ CLTSKILL_St_Whirlwind,
    /* [32] 0x6FB4CE10 */ CLTSKILL_St_MaggotUp,
    /* [33] 0x6FB4C800 */ CLTSKILL_St_MaggotDown,
    /* [34] 0x6FB4CD30 */ CLTSKILL_St_AndarielSpray,
    /* [35] 0x6FB4CAD0 */ CLTSKILL_St_SwarmMove_QuickStrike,
    /* [36] 0x6FB4C6B0 */ CLTSKILL_St_Nest_EvilHutSpawner,
    /* [37] 0x6FB4C610 */ CLTSKILL_St_Submerge_MaggotEgg,
    /* [38] 0x6FB4C5F0 */ CLTSKILL_St_Emerge,
    /* [39] 0x6FB4C9A0 */ CLTSKILL_St_MonInferno,
    /* [40] 0x6FB4C580 */ CLTSKILL_St_DiabRun,
    /* [41] 0x6FB4C270 */ CLTSKILL_St_ClearUsedSkillFlags,
    /* [42] 0x6FB4D2D0 */ CLTSKILL_St_MultiHitRollCount,
    /* [43] 0x6FB4D270 */ CLTSKILL_St_SpawnMissileTowardTarget,
    /* [44] 0x6FB4CFB0 */ CLTSKILL_St_HellMeteor,
    /* [45] 0x6FB78E10 */ CLTSKILL_St_Wearwolf_Wearbear,
    /* [46] 0x6FB76D80 */ CLTSKILL_St_BaalTaunt,
    /* [47] 0x6FB76EB0 */ CLTSKILL_St_Raven_DruidSummon,
    /* [48] 0x6FB76310 */ CLTSKILL_St_SpawnCltMissileAAtCaster,
    /* [49] 0x6FB77A30 */ CLTSKILL_St_Vines_MinionSpawner,
    /* [50] 0x6FB766B0 */ CLTSKILL_St_SaveTargetCoords,
    /* [51] 0x6FB76620 */ CLTSKILL_St_Corpse_VineCycler,
    /* [52] 0x6FB76130 */ CLTSKILL_St_ArcticBlast_Inferno,
    /* [53] 0x6FAFBA80 */ CLTSKILL_St53_Zeal_Fury,
    /* [54] 0x00000000 */ nullptr,
    /* [55] 0x00000000 */ nullptr,
    /* [56] 0x00000000 */ nullptr,
    /* [57] 0x00000000 */ nullptr,
    /* [58] 0x00000000 */ nullptr,
    /* [59] 0x00000000 */ nullptr,
    /* [60] 0x00000000 */ nullptr,
    /* [61] 0x00000000 */ nullptr,
    /* [62] 0x00000000 */ nullptr,
    /* [63] 0x00000000 */ nullptr,
    /* [64] 0x00000000 */ nullptr,
    /* [65] 0x00000000 */ nullptr,
    /* [66] 0x00000000 */ nullptr,
    /* [67] 0x00000000 */ nullptr,
    /* [68] 0x00000000 */ nullptr,
    /* [69] 0x00000000 */ nullptr,
};

// 1.12a: 0x6FBA3838
extern const int32_t gnCltDoFuncs = 130;
// 1.12a: 0x6FBA3630
extern const D2CltSkillFunc_t gpfnCltDoFunc[130] = {
    /* [  0] 0x00000000 */ nullptr,
    /* [  1] 0x6FAFF450 */ CLTSKILL_Do001_Attack,
    /* [  2] 0x6FB000B0 */ CLTSKILL_Do002_Throw,
    /* [  3] 0x6FB00CC0 */ CLTSKILL_Do003_PsychicHammer,
    /* [  4] 0x6FB008F0 */ CLTSKILL_Do004_DragonTalon,
    /* [  5] 0x6FB01B10 */ CLTSKILL_Do005_ProgressiveLobRain,
    /* [  6] 0x6FB01620 */ CLTSKILL_Do006_BladeFury,
    /* [  7] 0x6FB00B90 */ CLTSKILL_Do007_ProgressiveTargetPointMissile,
    /* [  8] 0x6FB01510 */ CLTSKILL_Do008_StreamSentryAtTarget,
    /* [  9] 0x6FB01A00 */ CLTSKILL_Do009_ProgressiveGroundPatchCircle,
    /* [ 10] 0x6FB00AD0 */ CLTSKILL_Do010_ProgressiveTargetNova,
    /* [ 11] 0x6FB01910 */ CLTSKILL_Do011_ProgressiveRadialRing,
    /* [ 12] 0x6FB00DE0 */ CLTSKILL_Do012_ProgressiveChargeTargetOverlay,
    /* [ 13] 0x6FB00CF0 */ CLTSKILL_Do013_ProgressiveChargeSelfOverlay,
    /* [ 14] 0x6FB00A40 */ CLTSKILL_Do014_ProgressiveCoordMissile,
    /* [ 15] 0x6FB01770 */ CLTSKILL_Do015_ProgressiveRandomScatter,
    /* [ 16] 0x6FAFB810 */ CLTSKILL_Do016_Jab,
    /* [ 17] 0x6FAFC670 */ CLTSKILL_Do017_ParallelSpreadMissiles,
    /* [ 18] 0x6FAFC510 */ CLTSKILL_Do018_GuidedArrow_BoneSpirit,
    /* [ 19] 0x6FAFC380 */ CLTSKILL_Do019_ChargedStrike,
    /* [ 20] 0x6FAFC0F0 */ CLTSKILL_Do020_Strafe,
    /* [ 21] 0x6FAFB8A0 */ CLTSKILL_Do021_Fend_Zeal_Fury,
    /* [ 22] 0x6FAFBF30 */ CLTSKILL_Do022_LightningStrike,
    /* [ 23] 0x6FB02B20 */ CLTSKILL_Do023_ChargedBolt,
    /* [ 24] 0x6FB02DB0 */ CLTSKILL_Do024_Inferno_ArcticBlast,
    /* [ 25] 0x6FB01ED0 */ CLTSKILL_Do025_Nova_FrostNova,
    /* [ 26] 0x6FB027D0 */ CLTSKILL_Do026_FireWall,
    /* [ 27] 0x6FB01E20 */ CLTSKILL_Do027_ChainLightning,
    /* [ 28] 0x6FB01D90 */ CLTSKILL_Do028_CollisionCheckedTargetMissile,
    /* [ 29] 0x6FB01D10 */ CLTSKILL_Do029_LinearSkillMissile,
    /* [ 30] 0x6FB78720 */ CLTSKILL_Do_Curse,
    /* [ 31] 0x6FB78480 */ CLTSKILL_Do_PoisonExplosion,
    /* [ 32] 0x6FB788C0 */ CLTSKILL_Do_CorpseExplosion,
    /* [ 33] 0x6FB78870 */ CLTSKILL_Do_RaiseSkeleton_Mage,
    /* [ 34] 0x6FB37650 */ CLTSKILL_Do_BlessedHammer,
    /* [ 35] 0x6FB37D60 */ CLTSKILL_Do_HolyBolt_SmiteMissile,
    /* [ 36] 0x6FB37C50 */ CLTSKILL_Do_FistOfTheHeavens,
    /* [ 37] 0x6FB37800 */ CLTSKILL_Do_Charge,
    /* [ 38] 0x6FB74040 */ CLTSKILL_Do_FindPotion,
    /* [ 39] 0x6FB74340 */ CLTSKILL_Do_WhirlwindAttack,
    /* [ 40] 0x6FB73FD0 */ CLTSKILL_Do_FindItem,
    /* [ 41] 0x6FB749A0 */ CLTSKILL_Do_GrimWard,
    /* [ 42] 0x6FB74B70 */ CLTSKILL_Do_DoubleThrow,
    /* [ 43] 0x6FB74840 */ CLTSKILL_Do_Leap,
    /* [ 44] 0x6FB74700 */ CLTSKILL_Do_LeapAttack,
    /* [ 45] 0x6FB73ED0 */ CLTSKILL_Do_WhirlwindEnd,
    /* [ 46] 0x6FB4C310 */ CLTSKILL_Do_ResetSeqSpeed,
    /* [ 47] 0x6FB4C2D0 */ CLTSKILL_Do_ClearActionFrameAndCheckMetricsB,
    /* [ 48] 0x6FB4CB90 */ CLTSKILL_Do_AndarielSpray,
    /* [ 49] 0x6FB4C770 */ CLTSKILL_Do_SwarmMove_QuickStrike,
    /* [ 50] 0x6FB4C630 */ CLTSKILL_Do_Nest_EvilHutSpawner,
    /* [ 51] 0x6FB4E260 */ CLTSKILL_Do_ArcaneTower,
    /* [ 52] 0x6FB4C290 */ CLTSKILL_Do_ClearActionFrameAndCheckMetricsA,
    /* [ 53] 0x6FB4E170 */ CLTSKILL_Do_DiabPrison,
    /* [ 54] 0x6FB4E820 */ CLTSKILL_Do_ImpInferno,
    /* [ 55] 0x6FB4DE90 */ CLTSKILL_Do_DiabWall,
    /* [ 56] 0x6FB4E710 */ CLTSKILL_Do_MonInferno,
    /* [ 57] 0x6FB4DD10 */ CLTSKILL_Do_DiabLight,
    /* [ 58] 0x6FB4DC00 */ CLTSKILL_Do_FingerMageSpider,
    /* [ 59] 0x6FB4C3E0 */ CLTSKILL_Do_DiabRun,
    /* [ 60] 0x6FB4D9F0 */ CLTSKILL_Do_DiabCold,
    /* [ 61] 0x6FB4D850 */ CLTSKILL_Do_PrimePoisonNova,
    /* [ 62] 0x6FB4C320 */ CLTSKILL_Do_MultiHitRollbackLoop,
    /* [ 63] 0x6FB4D740 */ CLTSKILL_Do_FetishAura_Cluster,
    /* [ 64] 0x6FB4D3F0 */ CLTSKILL_Do_DesertTurret_Jump,
    /* [ 65] 0x6FB4D620 */ CLTSKILL_Do_MonCurseCast,
    /* [ 66] 0x6FB4E430 */ CLTSKILL_Do_Hireable_RogueMissile,
    /* [ 67] 0x6FB79820 */ CLTSKILL_Do_ChargedBoltSentry_ShockWeb,
    /* [ 68] 0x6FB79770 */ CLTSKILL_Do_BladeFury_RoyalStrike,
    /* [ 69] 0x6FB796C0 */ CLTSKILL_Do_Twister_Tornado,
    /* [ 70] 0x6FB77910 */ CLTSKILL_Do_Firestorm,
    /* [ 71] 0x6FB77660 */ CLTSKILL_Do_Eruption_MoltenBoulder,
    /* [ 72] 0x6FB77AE0 */ CLTSKILL_Do_Meteor_Volcano_Catapult,
    /* [ 73] 0x6FB76290 */ CLTSKILL_Do_SpawnCltMissileBAtCaster,
    /* [ 74] 0x6FB76E10 */ CLTSKILL_Do_SuckBlood,
    /* [ 75] 0x6FB76790 */ CLTSKILL_Do_SiegeBeastStomp,
    /* [ 76] 0x6FB76AA0 */ CLTSKILL_Do_OverseerWhip,
    /* [ 77] 0x6FB774F0 */ CLTSKILL_Do_ArcticBlast_Channel,
    /* [ 78] 0x6FB76990 */ CLTSKILL_Do_SpawnSavedCoordMissiles,
    /* [ 79] 0x6FB76070 */ CLTSKILL_Do_ImpTeleport,
    /* [ 80] 0x6FB77240 */ CLTSKILL_Do_WakeOfDestruction_BaalCold,
    /* [ 81] 0x6FB768C0 */ CLTSKILL_Do_DirectionalMissileWithPerpTarget,
    /* [ 82] 0x6FB76380 */ CLTSKILL_Do_BaalCorpseExplode,
    /* [ 83] 0x6FB01820 */ CLTSKILL_Do083_ProgressiveChargedBoltRing,
    /* [ 84] 0x6FB023B0 */ CLTSKILL_Do084_ThunderStormStrike,
    /* [ 85] 0x6FB789F0 */ CLTSKILL_Do_SpawnPeriodicGore,
    /* [ 86] 0x6FB37A80 */ CLTSKILL_Do_HolyFreeze_SanctuaryAura,
    /* [ 87] 0x6FB02C40 */ CLTSKILL_Do087_PeriodicAreaLob,
    /* [ 88] 0x6FB765F0 */ CLTSKILL_Do_UpdateRiderPath,
    /* [ 89] 0x6FB77050 */ CLTSKILL_Do_VineAttack,
    /* [ 90] 0x6FB795D0 */ CLTSKILL_Do_Armageddon_Hurricane,
    /* [ 91] 0x6FB78ED0 */ CLTSKILL_Do_ShockField_Sentry,
    /* [ 92] 0x6FB78FE0 */ CLTSKILL_Do_PhoenixStrike_FallingMeteorPair,
    /* [ 93] 0x6FB77430 */ CLTSKILL_Do_FenrisRage,
    /* [ 94] 0x6FB4E630 */ CLTSKILL_Do_DoomKnightMissile,
    /* [ 95] 0x6FB4E550 */ CLTSKILL_Do_NecromageMissile,
    /* [ 96] 0x6FB02670 */ CLTSKILL_Do096_TargetOffsetMissile,
    /* [ 97] 0x00000000 */ nullptr,
    /* [ 98] 0x00000000 */ nullptr,
    /* [ 99] 0x00000000 */ nullptr,
    /* [100] 0x00000000 */ nullptr,
    /* [101] 0x00000000 */ nullptr,
    /* [102] 0x00000000 */ nullptr,
    /* [103] 0x00000000 */ nullptr,
    /* [104] 0x00000000 */ nullptr,
    /* [105] 0x00000000 */ nullptr,
    /* [106] 0x00000000 */ nullptr,
    /* [107] 0x00000000 */ nullptr,
    /* [108] 0x00000000 */ nullptr,
    /* [109] 0x00000000 */ nullptr,
    /* [110] 0x00000000 */ nullptr,
    /* [111] 0x00000000 */ nullptr,
    /* [112] 0x00000000 */ nullptr,
    /* [113] 0x00000000 */ nullptr,
    /* [114] 0x00000000 */ nullptr,
    /* [115] 0x00000000 */ nullptr,
    /* [116] 0x00000000 */ nullptr,
    /* [117] 0x00000000 */ nullptr,
    /* [118] 0x00000000 */ nullptr,
    /* [119] 0x00000000 */ nullptr,
    /* [120] 0x00000000 */ nullptr,
    /* [121] 0x00000000 */ nullptr,
    /* [122] 0x00000000 */ nullptr,
    /* [123] 0x00000000 */ nullptr,
    /* [124] 0x00000000 */ nullptr,
    /* [125] 0x00000000 */ nullptr,
    /* [126] 0x00000000 */ nullptr,
    /* [127] 0x00000000 */ nullptr,
    /* [128] 0x00000000 */ nullptr,
    /* [129] 0x00000000 */ nullptr,
};


// ============================================================================
// Cross-TU VA-Mapped Globals & Adapters for SKILLS_Skills.cpp / SKILLS_Classes.cpp
// ============================================================================
struct D2UnitLightViewStrc;
struct D2LightNodeStrc;
extern void __fastcall PARTICLE_AllocForUnit(D2UnitLightViewStrc* pUnit);
extern void __fastcall PARTICLE_CancelFinished(D2UnitLightViewStrc* pUnit);
extern void __fastcall PARTICLE_SetFinished(D2UnitLightViewStrc* pUnit);
extern void __stdcall PARTICLE_InitGravityArc(D2UnitLightViewStrc* pUnit, int32_t nFrames, int32_t nInitialZ);
extern void __stdcall PARTICLE_SetPosition(D2UnitLightViewStrc* pUnit, int32_t nPosX, int32_t nPosY, int32_t nPosZ);
extern int32_t __fastcall PARTICLE_GetIntPosX(D2UnitLightViewStrc* pUnit);
extern int32_t __fastcall PARTICLE_GetIntPosY(D2UnitLightViewStrc* pUnit);
extern int32_t __fastcall PARTICLE_GetIntPosZ(D2UnitLightViewStrc* pUnit);
extern void __stdcall PARTICLE_UpdateUnitPhysics(D2UnitLightViewStrc* pUnit);
extern int32_t __fastcall SOUND_GetMonsterModeSoundId(D2UnitLightViewStrc* pUnit, int32_t nAnimMode);
extern D2LightNodeStrc* __stdcall LIGHTMAP_CreateLightNode(D2UnitLightViewStrc* pUnit, int32_t nLightType, int32_t nRadius, uint8_t nIntensity, uint8_t nRed, uint8_t nGreen, uint8_t nBlue);
extern void __fastcall LIGHTMAP_DestroyLightNode(D2LightNodeStrc* pLight);
extern void __fastcall GFX_AttachOverlayToUnit(D2UnitStrc* pUnit, int nOverlayId, int bPreDrawOverride);
extern int __fastcall GFX_GetSingleLayerUnitCelOffset(D2UnitStrc* pUnit, int* pOutOffsetX, int* pOutOffsetY);
extern void __stdcall D2Client_FaceUnitTowardDynamicTarget(D2UnitStrc* pUnit, int32_t nArg);
extern int32_t __fastcall D2Client_PlaySound(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nTicksDelay, BOOL bLoop, uint32_t dwFlags);
extern void __fastcall D2Client_SpawnMonsterIdleOrWalkTrailMissiles(D2UnitStrc* pUnit);
extern int32_t __fastcall D2Client_GetWeaponImpactSoundByHitClass(int32_t nHitClassLowNibble);
extern int32_t __fastcall D2Client_GetElementalImpactSoundByHitClass(int32_t nHitClassHighNibble);
extern void __fastcall D2Client_PlayUnitSpeechOrVoiceSound(D2UnitStrc* pUnit, int32_t nSpeechType);
extern void __fastcall D2Client_PlayPlayerFootstepSound(D2UnitStrc* pUnit);
extern BOOL __fastcall D2Client_ValidatePathTargetUnit(D2DynamicPathStrc* pDynamicPath);
extern void __fastcall D2Client_ChangeUnitModeWithHitOverlayAndSound(D2UnitStrc* pUnit, int32_t nMode);
extern void __fastcall D2Client_DispatchUnitModeChange(D2UnitStrc* pUnit, int32_t nMode);
struct SMsgWindowMessage;
struct D2RosterUnitStrc;
struct D2UnitEventStrc;
extern void __stdcall SOUNDHDR_UpdateFrame();
extern void __fastcall WALL2_UnregisterSingleMsgHandler(HWND hWnd, uint32_t nType, uint32_t uMsgOrId, void (__stdcall* pfHandler)(SMsgWindowMessage*));
extern void __fastcall WALL2_RegisterSingleMsgHandler(HWND hWnd, uint32_t nType, uint32_t uMsgOrId, void (__stdcall* pfHandler)(SMsgWindowMessage*));
extern int32_t __fastcall WALL2_PumpWindowMessages(void);
extern D2RosterUnitStrc* __fastcall ROSTER_FindPlayerByUnitId(uint32_t dwUnitId);
extern int32_t __fastcall ROSTERPETS_GetPetType(uint32_t dwPetUnitId);
extern void __fastcall MSG_SendPing_6D(void);
extern void __stdcall UI_LoadLoadingPalette();
extern void __fastcall CURSOR_DrawLockedItemTooltip(void);
extern D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileDefault(const char* szFileName, int32_t nFileType);
extern D2UnitEventStrc* __stdcall UNITEVENT_AllocUnitEvent(D2UnitStrc* pUnit, int32_t nEventType, int32_t nArg1, int32_t nArg2, int32_t (__fastcall* pfnCallback)(int32_t, D2UnitStrc*, int32_t, int32_t, int32_t, int32_t), int32_t nQueueId, int32_t nEventId);
extern void __fastcall CLTSKILL_CancelChargeCallback(D2UnitStrc* pUnit, int32_t nSkillId);
extern void __fastcall CLTSKILL_CancelWhirlwindCallback(D2UnitStrc* pUnit, int32_t nSkillId);
extern void __fastcall CLTSKILL_CancelLeapCallback(D2UnitStrc* pUnit);

// Modal & Belt globals (0x6FB8FF98..0x6FBCC29C)
void (__fastcall *gpfnDefaultScreenRender)(BOOL) = nullptr;
void (__fastcall *gpfnModalOkScreenRender)(BOOL) = nullptr;
void (__fastcall *gpfnModalOkCancelRender)(BOOL) = nullptr;
void (__fastcall *gpfnActiveScreenRender)(BOOL)  = nullptr;
int32_t gnModalScreenState = 0;
int32_t gaBeltSlotScreenX[8] = {};
int32_t gaBeltSlotScreenY[8] = {};
int32_t gaBeltSlotGridCol[8] = {};
int32_t gaBeltSlotFlatIndex[8] = {};
BOOL gbModalUseExpansionPromptText = FALSE;
D2RosterPetStrc* gpRosterPetList = nullptr;
int32_t gnUiOffsetX = 0;
int32_t gnUiOffsetY = 0;
int32_t gnInputDragStartX = 0;
int32_t gnInputDragStartY = 0;
int32_t gnInputDragEndX = 0;
int32_t gnInputDragEndY = 0;
BOOL gbBeltPopupOpen = FALSE;
BOOL gbMiniPanelCompactLayout = FALSE;
D2CellFileStrc* gpPopupOkBoxCell1 = nullptr;
D2CellFileStrc* gpOkCancelButtonCell = nullptr;
D2CellFileStrc* gpPopupOkBoxCell2 = nullptr;
BOOL gbModalButtonMouseCaptured = FALSE;
BOOL gbModalOkButtonPressed = FALSE;
BOOL gbModalCancelButtonPressed = FALSE;
BOOL gbModalOkSingleDismissed = FALSE;
BOOL gbModalOkAccepted = FALSE;
BOOL gbModalCancelAccepted = FALSE;

// PARTICLE_* typed/untyped overloads
void __fastcall PARTICLE_AllocForUnit(void* pUnit) { PARTICLE_AllocForUnit(reinterpret_cast<D2UnitLightViewStrc*>(pUnit)); }
void __fastcall PARTICLE_AllocMissileParticle(D2UnitStrc* pUnit) { PARTICLE_AllocForUnit(reinterpret_cast<D2UnitLightViewStrc*>(pUnit)); }
void __fastcall PARTICLE_CancelFinished(void* pUnit) { PARTICLE_CancelFinished(reinterpret_cast<D2UnitLightViewStrc*>(pUnit)); }
void __fastcall PARTICLE_SetFinished(void* pUnit) { PARTICLE_SetFinished(reinterpret_cast<D2UnitLightViewStrc*>(pUnit)); }
void __stdcall PARTICLE_InitGravityArc(void* pUnit, int32_t nFrames, int32_t nInitialZ) { PARTICLE_InitGravityArc(reinterpret_cast<D2UnitLightViewStrc*>(pUnit), nFrames, nInitialZ); }
void __fastcall PARTICLE_InitLobTrajectory(D2UnitStrc* pUnit, int32_t nFrames, int32_t nInitialZ) { PARTICLE_InitGravityArc(reinterpret_cast<D2UnitLightViewStrc*>(pUnit), nFrames, nInitialZ); }
void __stdcall PARTICLE_SetPosition(void* pUnit, int32_t nPosX, int32_t nPosY, int32_t nPosZ) { PARTICLE_SetPosition(reinterpret_cast<D2UnitLightViewStrc*>(pUnit), nPosX, nPosY, nPosZ); }
void __fastcall PARTICLE_InitVerticalRise(D2UnitStrc* pUnit, int32_t nPosX, int32_t nPosY, int32_t nPosZ) { PARTICLE_SetPosition(reinterpret_cast<D2UnitLightViewStrc*>(pUnit), nPosX, nPosY, nPosZ); }
void __fastcall D2Client_SetUnitDrawElevationOffset_6FB755B0(D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nZ) { PARTICLE_SetPosition(reinterpret_cast<D2UnitLightViewStrc*>(pUnit), nX, nY, nZ); }
int32_t __fastcall PARTICLE_GetIntPosX(void* pUnit) { return PARTICLE_GetIntPosX(reinterpret_cast<D2UnitLightViewStrc*>(pUnit)); }
int32_t __fastcall PARTICLE_GetIntPosY(void* pUnit) { return PARTICLE_GetIntPosY(reinterpret_cast<D2UnitLightViewStrc*>(pUnit)); }
int32_t __fastcall PARTICLE_GetIntPosZ(void* pUnit) { return PARTICLE_GetIntPosZ(reinterpret_cast<D2UnitLightViewStrc*>(pUnit)); }
int32_t __fastcall PARTICLE_GetHeightInPixels(D2UnitStrc* pUnit) { return PARTICLE_GetIntPosZ(reinterpret_cast<D2UnitLightViewStrc*>(pUnit)); }
void __stdcall PARTICLE_UpdateUnitPhysics(void* pUnit) { PARTICLE_UpdateUnitPhysics(reinterpret_cast<D2UnitLightViewStrc*>(pUnit)); }
void __stdcall PARTICLE_EnableActiveUpdateFlag(D2UnitStrc* pUnit) { PARTICLE_UpdateUnitPhysics(reinterpret_cast<D2UnitLightViewStrc*>(pUnit)); }
int32_t __fastcall SOUND_GetMonsterModeSoundId(void* pUnit, int32_t nAnimMode) { return SOUND_GetMonsterModeSoundId(reinterpret_cast<D2UnitLightViewStrc*>(pUnit), nAnimMode); }

// Intra-SKILLS & Cross-TU adapters
extern void __fastcall MISSILE_SpawnCircularGroundField(int32_t nMissileId, D2UnitStrc* pOrigin, int32_t nCenterX, int32_t nCenterY, int32_t nRadius, int32_t nSkillId, int32_t nSkillLevel, int32_t nSkipChanceDivisor, int32_t nGridStep);

int32_t __fastcall CLTSKILL_ExecuteSkillStartFunc(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nOwnerGUID, int32_t nSkillLevel)
{
    return CLTSKILL_StartSkill(pUnit, nSkillId, static_cast<D2UnitGUID>(nOwnerGUID), nSkillLevel);
}
int32_t __stdcall CLTSKILL_ExecuteSkillDoFunc(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t bPlayDoSound)
{
    return CLTSKILL_DoSkill(pUnit, nSkillId, nSkillLevel, bPlayDoSound);
}
int32_t __fastcall CLTSKILL_FindUnitsInRadius(D2UnitStrc* pOrigin, int32_t nX, int32_t nY, int32_t nRadius, uint32_t dwFilterFlags, void* pfnCallback, void* pCallbackArg, int32_t nExtra)
{
    CLTSKILL_SearchUnitsInRadius(pOrigin, nX, nY, nRadius, dwFilterFlags, reinterpret_cast<D2AuraFilterCallback_t>(pfnCallback), pCallbackArg, nExtra);
    return 1;
}
void __stdcall SKILLS_FindTargetsInRadius_6FAFE9B0(D2UnitStrc* pOwner, int32_t nX, int32_t nY, int32_t nRadius, uint32_t dwFilterFlags, void* pfnCallback, void* pCallbackArg, int32_t nExtra)
{
    CLTSKILL_SearchUnitsInRadius(pOwner, nX, nY, nRadius, dwFilterFlags, reinterpret_cast<D2AuraFilterCallback_t>(pfnCallback), pCallbackArg, nExtra);
}
struct D2ClientAuraCallbackStrc;
void __stdcall SKILLS_ForEachUnitInRadius(D2UnitStrc* pCaster, int32_t nX, int32_t nY, int32_t nRadius, uint32_t dwAuraFilter, int32_t (__fastcall* pfnCallback)(D2ClientAuraCallbackStrc*, D2UnitStrc*), void* pCallbackArgs, BOOL bCheckLos)
{
    CLTSKILL_SearchUnitsInRadius(pCaster, nX, nY, nRadius, dwAuraFilter, reinterpret_cast<D2AuraFilterCallback_t>(pfnCallback), pCallbackArgs, bCheckLos);
}
D2UnitStrc* __fastcall SKILLS_FindNearestTargetInRadius(D2UnitStrc* pCaster, int32_t nX, int32_t nY, int32_t nRadius, uint32_t dwAuraFilter, D2UnitGUID dwExcludeGUID)
{
    return CLTSKILL_FindNextTargetInRadius(pCaster, dwExcludeGUID, dwAuraFilter, nRadius, nX, nY, nullptr);
}
BOOL __fastcall SKILLS_GetSkillTargetCoords(D2UnitStrc* pUnit, int32_t* pX, int32_t* pY) { return CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, pX, pY); }
void __stdcall SKILLS_GetForwardOffsetCoords_6FAFDC40(D2UnitStrc* pUnit, int32_t* pOutX, int32_t* pOutY) { CLTSKILL_GetUnitForwardStepCoords(pUnit, pOutX, pOutY); }
int32_t __fastcall SKILLS_TargetFindCallback_SelectNextByGUID_6FAFDB00(void* pUnitFindData, D2UnitStrc* pCandidate)
{
    return CLTSKILL_AuraCallback_SelectNextRingTarget(reinterpret_cast<D2AuraCallbackCtxStrc*>(pUnitFindData), pCandidate);
}
void __fastcall SKILLS_InitChargedBoltPath(D2UnitStrc* pMissile, int32_t nLowSeed) { CLTSKILL_MissileInit_ChargedBoltFromFirstPoint(pMissile, nLowSeed); }
void __fastcall CLTSKILL_InitBezierVineMissile(D2UnitStrc* pMissile, int32_t nSeedOffset) { CLTSKILL_MissileInit_ChargedBoltFromFirstPoint(pMissile, nSeedOffset); }
void __fastcall SKILLS_SpawnChargedBoltFan(D2MissileStrc* pMissileParams, int32_t nBoltCount) { CLTSKILL_SpawnMultiRingRadialBurst(pMissileParams, nBoltCount); }
void __stdcall SKILLS_SpawnRandomRadiusSubmissiles(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nCount, int32_t nRadius, int32_t nSkillId, int32_t nSkillLevel)
{
    CLTSKILL_SpawnSeededRandomLobRain(pOrigin, nMissileId, nCount, nRadius, nSkillId, nSkillLevel);
}
struct D2ClientMissileInitStrc;
void __fastcall CLTSKILL_SpawnScatteredMissileCluster(D2ClientMissileInitStrc* pInit, int32_t nCount, int32_t nSpreadRadius)
{
    D2MissileStrc* pMis = reinterpret_cast<D2MissileStrc*>(pInit);
    if (pMis && pMis->pOwner)
    {
        CLTSKILL_SpawnQuadrantScatterMissiles(pMis->pOwner, pMis, nCount, nSpreadRadius);
    }
}
D2UnitStrc* __fastcall CLTSKILL_SpawnInfernoArcticFlameMissile(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    return CLTSKILL_SpawnInfernoJetSegment(pUnit, nSkillId, nSkillLevel);
}
void __fastcall D2Client_SpawnMissileRing(D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nCenterX, int32_t nCenterY, int32_t nRadius, int32_t nSkillId, int32_t nSkillLevel, int32_t nStep, int32_t bCheckCollision)
{
    MISSILE_SpawnCircularGroundField(nMissileId, pOrigin, nCenterX, nCenterY, nRadius, nSkillId, nSkillLevel, nStep, bCheckCollision);
}
void __fastcall D2Client_FinishPaladinCharge_6FB373F0(D2UnitStrc* pUnit, int32_t nSkillId) { CLTSKILL_CancelChargeCallback(pUnit, nSkillId); }
void __fastcall D2Client_FinishBarbarianLeap_6FB73E70(D2UnitStrc* pUnit, int32_t nSkillId) { CLTSKILL_CancelWhirlwindCallback(pUnit, nSkillId); }
void __fastcall D2Client_FinishBarbarianWhirlwind_6FB74C30(D2UnitStrc* pUnit) { CLTSKILL_CancelLeapCallback(pUnit); }

// Remaining external subsystem adapters
int32_t __fastcall D2Client_DispatchUnitEventList_6FB6A300(D2UnitStrc* pUnit, int32_t nEventType, int32_t nArg1, D2CltUnitEventFunc_t pfnCallback, D2UnitStrc* pSourceUnit, int32_t nSkillId)
{
    return UNITEVENT_AllocUnitEvent(pUnit, nEventType, nArg1, 0, reinterpret_cast<int32_t (__fastcall*)(int32_t, D2UnitStrc*, int32_t, int32_t, int32_t, int32_t)>(pfnCallback), reinterpret_cast<int32_t>(pSourceUnit), nSkillId) != nullptr ? 1 : 0;
}
int32_t __fastcall D2Client_GetRosterPetTypeByGUID_6FB73130(uint32_t dwGUID) { return ROSTERPETS_GetPetType(dwGUID); }
int32_t __fastcall D2Client_GetUnitFootstepSoundId(D2UnitStrc* pUnit, int32_t bLanding) { (void)bLanding; D2Client_PlayPlayerFootstepSound(pUnit); return 0; }
int32_t __fastcall D2Client_GetWeaponSoundDelayFrames_6FAE6720(D2UnitStrc* pUnit, int32_t nHitClass) { (void)pUnit; return D2Client_GetWeaponImpactSoundByHitClass(nHitClass); }
int32_t __fastcall D2Client_GetWeaponSwingSoundByHitClass(D2UnitStrc* pUnit, int32_t nHitClass) { (void)pUnit; return D2Client_GetElementalImpactSoundByHitClass(nHitClass); }
int32_t __fastcall D2Client_GetWeaponSwingSoundId_6FAE6780(D2UnitStrc* pUnit, int32_t nHitClass) { (void)pUnit; return D2Client_GetElementalImpactSoundByHitClass(nHitClass); }
extern int __fastcall GFX_HasUnitOverlay(D2UnitStrc* pUnit, int nOverlayId);
BOOL __fastcall D2Client_HasUnitOverlay_6FB40A60(D2UnitStrc* pUnit, int32_t nOverlayId)
{
    return GFX_HasUnitOverlay(pUnit, nOverlayId);
}
D2CellFileStrc* __fastcall D2Client_LoadUiCelArchive_6FB6EF70(const char* szPath, int32_t nType) { return ARCHIVE_LoadCellFileDefault(szPath, nType); }
D2ClientLightMapStrc* __fastcall D2Client_AllocClientLightMap_6FB0EAF0(int32_t nType, int32_t nIntensity, uint8_t nR, uint8_t nG, uint8_t nB)
{
    return reinterpret_cast<D2ClientLightMapStrc*>(LIGHTMAP_CreateLightNode(nullptr, nType, 8, static_cast<uint8_t>(nIntensity), nR, nG, nB));
}
void __fastcall D2Client_FreeClientLightMap_6FB0E500(D2ClientLightMapStrc* pLight) { LIGHTMAP_DestroyLightNode(reinterpret_cast<D2LightNodeStrc*>(pLight)); }
D2RosterUnitPartialStrc* __fastcall D2Client_FindRosterPlayerByGUID_6FB281C0(uint32_t dwGUID) { return reinterpret_cast<D2RosterUnitPartialStrc*>(ROSTER_FindPlayerByUnitId(dwGUID)); }
void __cdecl D2Client_ClearUiDirtyState_6FB3A7F0(void) { UI_LoadLoadingPalette(); }
void __cdecl D2Client_DrawSoftwareCursor_6FB4F5B0(void) { CURSOR_DrawLockedItemTooltip(); }
void __cdecl D2Client_EndRenderFramePresent_6FB1A660(void) { SOUNDHDR_UpdateFrame(); }
void __cdecl D2Client_PumpNetworkAndAudio_6FB299D0(void) { MSG_SendPing_6D(); }
void __fastcall D2Client_AttachUnitOverlay_6FB430E0(D2UnitStrc* pUnit, int32_t nOverlayId, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8)
{
    (void)a4; (void)a5; (void)a6; (void)a7; (void)a8;
    GFX_AttachOverlayToUnit(pUnit, nOverlayId, a3);
}
void __fastcall D2Client_ChangeUnitModeEx_6FAF1450(int32_t nMode, D2UnitStrc* pUnit, D2UnitModeTransitionStrc* pTrans, int32_t bFlag)
{
    (void)pTrans; (void)bFlag;
    D2Client_DispatchUnitModeChange(pUnit, nMode);
}
void __fastcall D2Client_EnsureUnitPathAllocated_6FAF0D20(D2UnitStrc* pUnit) { if (pUnit && pUnit->pDynamicPath) D2Client_ValidatePathTargetUnit(pUnit->pDynamicPath); }
void __fastcall D2Client_FaceUnitTarget_6FACF5F0(D2UnitStrc* pUnit, int32_t bForce) { D2Client_FaceUnitTowardDynamicTarget(pUnit, bForce); }
void __fastcall D2Client_GetUnitComponentScreenOffset(D2UnitStrc* pUnit, int32_t nComponent, int32_t* pOutX, int32_t* pOutY)
{
    (void)nComponent;
    GFX_GetSingleLayerUnitCelOffset(pUnit, pOutX, pOutY);
}
void __fastcall D2Client_PlayCharacterBarkSound_6FAE7250(D2UnitStrc* pUnit, int32_t nBark) { D2Client_PlayUnitSpeechOrVoiceSound(pUnit, nBark); }
void __fastcall D2Client_PlaySoundEx_6FAE1FA0(D2UnitStrc* pUnit, int32_t nSoundId, int32_t nDelay, int32_t bLoop, int32_t dwFlags)
{
    D2Client_PlaySound(nSoundId, pUnit, nDelay, bLoop, static_cast<uint32_t>(dwFlags));
}
void __fastcall D2Client_RegisterStormMsgEntry_6FB1CEC0(HWND hWnd, int32_t nType, uint32_t uMsg, BOOL (__stdcall* pfn)(SMSGHANDLER_PARAMS*))
{
    WALL2_RegisterSingleMsgHandler(hWnd, static_cast<uint32_t>(nType), uMsg, reinterpret_cast<void (__stdcall*)(SMsgWindowMessage*)>(pfn));
}
void __fastcall D2Client_UnregisterStormMsgEntry_6FB1CE40(HWND hWnd, int32_t nType, uint32_t uMsg, BOOL (__stdcall* pfn)(SMSGHANDLER_PARAMS*))
{
    WALL2_UnregisterSingleMsgHandler(hWnd, static_cast<uint32_t>(nType), uMsg, reinterpret_cast<void (__stdcall*)(SMsgWindowMessage*)>(pfn));
}
void __fastcall D2Client_RunModalEventLoop_6FB1CF40(BOOL (__stdcall* pfnPump)(uint32_t))
{
    while (pfnPump && pfnPump(0)) { WALL2_PumpWindowMessages(); }
}
void __fastcall D2Client_ScreenShake_6FAC3E60(int32_t nShakeType) { (void)nShakeType; }
void __fastcall D2Client_SetUnitCastOrNeutralAnimMode_6FAF13C0(D2UnitStrc* pUnit, int32_t nMode) { D2Client_ChangeUnitModeWithHitOverlayAndSound(pUnit, nMode); }
void __fastcall D2Client_SpawnCorpseGoreMissiles(D2UnitStrc* pCorpse, int32_t bPlaySound, int32_t nCount) { (void)bPlaySound; (void)nCount; D2Client_SpawnMonsterIdleOrWalkTrailMissiles(pCorpse); }
