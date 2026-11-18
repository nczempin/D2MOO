// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Slice: UNIT/Player.cpp (Part 4 & 5)
// Source File : ..\Source\D2Client\UNIT\Player.cpp
// Address Span: 0x6FB099B0 .. 0x6FB0DFEF (ImageBase: 0x6FAB0000)
// ============================================================================

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <Units/Units.h>
#include <Units/Player.h>
#include <Units/UnitRoom.h>
#include <Path/Path.h>
#include <Path/PathMisc.h>
#include <Path/Step.h>
#include <Drlg/D2DrlgDrlg.h>
#include <DataTbls/LevelsTbls.h>
#include <DataTbls/MonsterTbls.h>
#include <DataTbls/ObjectsTbls.h>
#include <DataTbls/SkillsTbls.h>
#include <D2Collision.h>
#include <D2Composit.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2Gfx.h>
#include <D2CMP.h>
#include <D2Inventory.h>
#include <D2Items.h>
#include <D2Monsters.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <UselessOrdinals.h>
#include <Fog.h>

// ============================================================================
// Structures (1.12a D2Client UNIT/Player.cpp)
// ============================================================================

#pragma pack(push, 1)

enum D2PlayerActionFlags : uint32_t
{
    PLAYERACT_LEFT_HAND      = 0x00000001, // Left mouse button / left-hand skill
    PLAYERACT_RIGHT_HAND     = 0x00000002, // Right mouse button / right-hand skill
    PLAYERACT_BUTTON_DOWN    = 0x00000004, // Initial mouse button press
    PLAYERACT_BUTTON_HELD    = 0x00000008, // Continuous mouse button hold / repeat
    PLAYERACT_BUTTON_UP      = 0x00000010, // Mouse button release
    PLAYERACT_STAND_STILL    = 0x00000020, // Shift / stand-still modifier active
    PLAYERACT_FORCE_RUN      = 0x00000040, // Run toggle / modifier active (with stamina > 0)
    PLAYERACT_MOVE_ONLY      = 0x00000080, // Allow continuous movement / melee approach
    PLAYERACT_FORCE_APPROACH = 0x00000100, // Force approach to warp tile on right-click
};

// 0x20-byte local player input/skill/movement action descriptor (stack-allocated in 0x6FB0CE80 / 0x6FB0C310)
struct D2PlayerActionStateStrc
{
    uint32_t     dwFlags;           // +0x00: D2PlayerActionFlags bitmask
    D2UnitStrc*  pPlayerUnit;       // +0x04: Acting player unit (gpPlayerUnit)
    D2UnitStrc*  pTargetUnit;       // +0x08: Hovered / acquired target unit (or nullptr)
    int32_t      nTargetX;          // +0x0C: World subtile X target
    int32_t      nTargetY;          // +0x10: World subtile Y target
    int32_t      nLocationMoveMode; // +0x14: 1 = WalkToLocation, 3 = RunToLocation
    int32_t      nTargetMoveMode;   // +0x18: 2 = WalkToUnit/Interact, 4 = RunToUnit
    D2SkillStrc* pSkill;            // +0x1C: Selected left/right skill pointer
};
static_assert(sizeof(D2PlayerActionStateStrc) == 0x20, "D2PlayerActionStateStrc must be 0x20 bytes");

// 0x1C-byte mode transition argument block passed to D2Client_ExecutePlayerModeTransition (0x6FB0A820)
// and D2Client_DispatchUnitModeChange_6FAF1450 (0x6FAF1450).
struct D2PlayerModeChangeStrc
{
    int32_t  nSkillIdOrXOrType;   // +0x00: SkillId (21/22), TargetX (1/6/7/8/18/19/20/23/25), or TargetUnitType (0/2/24)
    uint32_t dwOwnerOrYOrGUID;    // +0x04: SkillOwnerGUID (21/22), TargetY (1/6/7/8/18/19/20/23/25), or TargetUnitId (0/2/24)
    int32_t  nTargetXOrTypeOrHit; // +0x08: Skill TargetX (21), Skill TargetUnitType (22), or HitClass (6/8/18/19/20)
    uint32_t dwTargetYOrGUID;     // +0x0C: Skill TargetY (21) or Skill TargetUnitId (22)
    int32_t  nParam4;             // +0x10: Reserved / zeroed
    int32_t  nParam5;             // +0x14: Reserved / zeroed
    int32_t  nParam6;             // +0x18: Reserved / zeroed
};
static_assert(sizeof(D2PlayerModeChangeStrc) == 0x1C, "D2PlayerModeChangeStrc must be 0x1C bytes");

// 10-byte S->C 0x74 (PlayerCorpseAssign) packet handled by 0x6FB0A340
struct D2GSPacketSrv74CorpseAssignStrc
{
    uint8_t  nHeader;        // +0x00: 0x74
    uint8_t  bAssign;        // +0x01: Non-zero when assigning corpse to player
    uint32_t dwPlayerUnitId; // +0x02: Owner player unit GUID
    uint32_t dwCorpseUnitId; // +0x06: Corpse player unit GUID
};
static_assert(sizeof(D2GSPacketSrv74CorpseAssignStrc) == 0x0A, "D2GSPacketSrv74CorpseAssignStrc must be 10 bytes");

// Client roster pet node (0x34 bytes, list head at 0x6FBCB97C)
struct D2ClientRosterPetStrc
{
    int32_t                nClassId;         // +0x00
    int32_t                nPetType;         // +0x04 (7 = Mercenary / Hireling)
    uint32_t               dwUnitId;         // +0x08
    uint32_t               dwOwnerId;        // +0x0C
    int32_t                nPetNameStringId; // +0x10
    int32_t                nScreenIconX;     // +0x14
    int32_t                nScreenIconY;     // +0x18
    int32_t                nLifePercent;     // +0x1C
    int32_t                bIsDeadHireling;  // +0x20
    uint32_t               dwMonSeed;        // +0x24
    uint32_t               dwNameIdx;        // +0x28
    uint32_t               dwSpecialFlag;    // +0x2C
    D2ClientRosterPetStrc* pNext;            // +0x30
};
static_assert(sizeof(D2ClientRosterPetStrc) == 0x34, "D2ClientRosterPetStrc must be 0x34 bytes");

// Client lightmap node view (partial layout for +0x18 fixed-point radius read in 0x6FB0A1D0)
struct D2ClientLightNodeViewStrc
{
    uint8_t pad00[0x18];
    int32_t nRadiusFixed; // +0x18
};

// Linked debug line target node attached to UNIT_TILE (+0x54) in 0x6FB0DDE0
struct D2TileDebugLinkNodeStrc
{
    int32_t                  nScreenX; // +0x00
    int32_t                  nScreenY; // +0x04
    uint8_t                  pad08[0x18];
    D2TileDebugLinkNodeStrc* pNext;    // +0x20
};

// Per-mode update control entry (12 bytes per mode, 20 entries at 0x6FB946D8..0x6FB947C7)
struct D2PlayerModeUpdateEntryStrc
{
    int32_t nStepType;       // +0x00 (0x6FB946D8): 0=none, 1=path step, 2=skill/channeled step
    int32_t nFrameEndType;   // +0x04 (0x6FB946DC): 0..3 sequence/frame clamping mode (jt 0x6FB0D52C)
    int32_t nCycleEndAction; // +0x08 (0x6FB946E0): 0=none, 1=on path end, 2/3=on frame cycle end
};

// Skill-use-state speech bark entry (4 bytes per state, 9 entries at 0x6FB947C8..0x6FB947EB)
struct D2SkillUseSpeechEntryStrc
{
    uint16_t wSpeechId; // +0x00
    uint16_t wReserved; // +0x02
};

// Outgoing C->S packet layouts built on stack in UNIT/Player.cpp
struct D2C2SPacketByteDwordStrc
{
    uint8_t  nHeader; // +0x00 (e.g. 0x17 C2S_DROPITEM, 0x4C)
    uint32_t dwArg;   // +0x01
};

struct D2C2SPacketSkillAtLocStrc
{
    uint8_t  nHeader; // +0x00 (0x08, 0x0C, 0x0F)
    uint16_t wX;      // +0x01
    uint16_t wY;      // +0x03
};

struct D2C2SPacketSkillOnUnitStrc
{
    uint8_t  nHeader;    // +0x00 (0x07, 0x09, 0x0A, 0x0D, 0x0E, 0x10, 0x11)
    uint32_t dwUnitType; // +0x01
    uint32_t dwUnitId;   // +0x05
};

#pragma pack(pop)

// ============================================================================
// Constant & Mutable Data Tables in D2Client.dll (1.12a VAs)
// ============================================================================

// 0x6FB946D8..0x6FB947C7: Player mode update control table (20 modes: PLRMODE_DEATH..PLRMODE_KNOCKBACK)
static const D2PlayerModeUpdateEntryStrc g_aPlayerModeUpdateTable_6FB946D8[NUMBER_OF_PLRMODES] = {
    /*  0 PLRMODE_DEATH       */ { 0, 2, 0 }, // 0x6FB946D8
    /*  1 PLRMODE_NEUTRAL     */ { 0, 0, 0 }, // 0x6FB946E4
    /*  2 PLRMODE_WALK        */ { 1, 0, 1 }, // 0x6FB946F0
    /*  3 PLRMODE_RUN         */ { 1, 0, 1 }, // 0x6FB946FC
    /*  4 PLRMODE_GETHIT      */ { 0, 1, 2 }, // 0x6FB94708
    /*  5 PLRMODE_TOWNNEUTRAL */ { 0, 0, 0 }, // 0x6FB94714
    /*  6 PLRMODE_TOWNWALK    */ { 1, 0, 1 }, // 0x6FB94720
    /*  7 PLRMODE_ATTACK1     */ { 2, 1, 3 }, // 0x6FB9472C
    /*  8 PLRMODE_ATTACK2     */ { 2, 1, 3 }, // 0x6FB94738
    /*  9 PLRMODE_BLOCK       */ { 0, 1, 2 }, // 0x6FB94744
    /* 10 PLRMODE_CAST        */ { 2, 1, 3 }, // 0x6FB94750
    /* 11 PLRMODE_THROW       */ { 2, 1, 3 }, // 0x6FB9475C
    /* 12 PLRMODE_KICK        */ { 2, 1, 3 }, // 0x6FB94768
    /* 13 PLRMODE_SPECIAL1    */ { 0, 1, 2 }, // 0x6FB94774
    /* 14 PLRMODE_SPECIAL2    */ { 2, 1, 3 }, // 0x6FB94780
    /* 15 PLRMODE_SPECIAL3    */ { 2, 1, 3 }, // 0x6FB9478C
    /* 16 PLRMODE_SPECIAL4    */ { 2, 1, 3 }, // 0x6FB94798
    /* 17 PLRMODE_DEAD        */ { 0, 2, 0 }, // 0x6FB947A4
    /* 18 PLRMODE_SEQUENCE    */ { 2, 1, 3 }, // 0x6FB947B0
    /* 19 PLRMODE_KNOCKBACK   */ { 1, 3, 1 }, // 0x6FB947BC
};

// 0x6FB947C8..0x6FB947EB: Skill use-state -> character voice bark ID table
static const D2SkillUseSpeechEntryStrc g_aSkillUseStateSpeechTable_6FB947C8[9] = {
    /* 0: Usable        */ {  0, 0 }, // 0x6FB947C8
    /* 1: No mana       */ { 21, 0 }, // 0x6FB947CC
    /* 2: Unusable      */ { 19, 0 }, // 0x6FB947D0
    /* 3: Unusable      */ { 19, 0 }, // 0x6FB947D4
    /* 4: No ammo/qty   */ { 19, 0 }, // 0x6FB947D8
    /* 5: Passive/other */ {  0, 0 }, // 0x6FB947DC
    /* 6: Reserved      */ {  0, 0 }, // 0x6FB947E0
    /* 7: Reserved      */ {  0, 0 }, // 0x6FB947E4
    /* 8: Skill delay   */ {  0, 0 }, // 0x6FB947E8
};

// 0x6FB947EC: Entry count of g_aSkillUseStateSpeechTable_6FB947C8
static const int32_t g_nSkillUseStateSpeechCount_6FB947EC = 9; // 0x6FB947EC

// 0x6FB946CC & 0x6FB946D0: Debug tile/room overlay tuning globals
static int32_t g_nDebugTileStep_6FB946CC = 64; // 0x6FB946CC
static int32_t g_bDebugRoomFlag_6FB946D0 = 1;  // 0x6FB946D0

// ============================================================================
// External Globals in D2Client.dll (Annotated with exact 1.12a VAs)
// ============================================================================

struct D2RosterPetEntryStrc;

extern D2UnitStrc*            gpPlayerUnit;                          // 0x6FBCC3D0: Local player unit
extern D2UnitStrc*            gapClientDecoyTargets_6FBCB6D4[8];     // 0x6FBCB6D4
#define gPlayerUnits_6FBCB6D4 gapClientDecoyTargets_6FBCB6D4
extern D2DrlgActStrc*         gpClientAct;                           // 0x6FBCC01C: Active client DRLG act
extern D2UnitStrc*            gapServerSideUnitHashTables[6][128];   // 0x6FBCA960: Client-side unit hash tables [type][guid & 0x7F] (0x6FBCAB60=mon, 0x6FBCAD60=obj)
extern D2RosterPetEntryStrc*  gpRosterPetList;                       // 0x6FBCB97C: Client roster pet / mercenary list
extern int32_t                gaPanelStates[38];                     // 0x6FBB2B58: UI panel open states ([9] @ 0x6FBB2B7C = EscMenu)
extern int32_t                gnScreenWidth;                         // 0x6FB8C6E0: Display screen width in pixels
extern int32_t                gnScreenHeight;                        // 0x6FB8C6E4: Display screen height in pixels
extern int32_t                gnMouseY;                              // 0x6FBB1634: Current mouse cursor Y coordinate
extern int32_t                gnCursorState;                         // 0x6FBB1674: Active cursor mode (6 = spell/target, 8 = identify/repair)
extern uint32_t               gdwClientGameTick;                     // 0x6FBB32B0: Client game frame tick counter
extern int32_t                gnViewportOffsetX_6FBC21D0;            // 0x6FBC21D0: Camera viewport world-to-screen X offset
extern int32_t                gnViewportOffsetY_6FBC21CC;            // 0x6FBC21CC: Camera viewport world-to-screen Y offset
extern int32_t                gnViewportShiftMode;                   // 0x6FBCC1D0: Open-panel viewport shift mode (0, 1, 2=left, 3=right)
extern int32_t                gnViewportShiftOffsetX;                // 0x6FBCC1D4: Horizontal viewport shift applied for open half-screen panels
extern int32_t                g_bScreenShakeActive;                  // 0x6FBCB9B0: Screen shake active flag
extern int32_t                g_nShakeOffsetX;                       // 0x6FBCB9B4: Screen shake X offset (0x6FBCB9B4)
extern int32_t                g_nShakeOffsetY;                       // 0x6FBAD114: Screen shake Y offset (0x6FBAD114)
extern int32_t                gnSelectedTargetA;                     // 0x6FBCC118: Target selection latch A
extern int32_t                gnSelectedTargetB;                     // 0x6FBCC11C: Target selection latch B

// Module/engine globals referenced by UNIT/Player.cpp that may not yet have shared definitions
// [aliased] gnLeftMouseBtnFlags_6FB90354
// [aliased] gnRightMouseBtnFlags_6FB90358
// [aliased] gdwCurTickMs_6FBB32A4
// [aliased] gnStepMoveFlag_6FBB32DC
// [aliased] gdwNextWarpClickTick_6FBB32E0
// [aliased] gnPanelViewportBiasX_6FBC21C8
// [aliased] gbLocalPlayerInTown_6FBC21D4
// [aliased] gbObjectOrItemClickLock_6FBCC048
// [aliased] gbPlayerMouseActionLock_6FBCC1D8
// [aliased] gbLOSDrawEnabled_6FBCC3F4

// ============================================================================
// External D2Client Cross-Module Function Declarations (Annotated with 1.12a VAs)
// ============================================================================

// Engine / DataTables / CUnit helpers
extern D2MonStatsTxt*  __fastcall ENGINE_GetMonStatsTxtRecord(int32_t nMonsterClassId);                                                   // 0x6FAB1390
extern D2MonStats2Txt* __fastcall ENGINE_GetMonStats2TxtRecord(int32_t nMonsterClassId);                                                  // 0x6FAB13D0
extern int32_t         __fastcall D2Client_GetUnitX_6FAB1590(D2UnitStrc* pUnit);                                                          // 0x6FAB1590
extern int32_t         __fastcall D2Client_GetUnitY_6FAB15C0(D2UnitStrc* pUnit);                                                          // 0x6FAB15C0
extern void            __fastcall D2Client_NotifyAutomapRoomChanged_6FAC35A0(D2ActiveRoomStrc* pRoom);                                    // 0x6FAC35A0
extern void            __fastcall D2Client_OnPlayerLeaveTown_6FACC3F0(D2UnitStrc* pPlayerUnit);                                           // 0x6FACC3F0
extern D2UnitStrc*     __fastcall D2Client_FindUnitInHashTable_6FACE550(D2UnitStrc** ppTable, int32_t nBucket, int32_t nUnitType);        // 0x6FACE550
extern BOOL            __fastcall D2Client_IsUnitDead_6FACEAC0(D2UnitStrc* pUnit);                                                        // 0x6FACEAC0
extern BOOL            __stdcall  D2Client_CanPlayerStartSkillWithMode_6FACEB20(D2UnitStrc* pPlayerUnit, D2SkillStrc* pSkill);            // 0x6FACEB20
extern D2UnitStrc*     __fastcall D2Client_FindServerSideUnit(uint32_t dwUnitId, int32_t nUnitType);                                      // 0x6FACF1C0
extern void            __stdcall  D2Client_FreeUnitInventoryItems_6FACF290(D2UnitStrc* pUnit, BOOL bKeepSharedItems);                     // 0x6FACF290
extern D2UnitStrc*     __fastcall D2Client_GetValidatedPathTargetUnit_6FACF5B0(D2UnitStrc* pUnit);                                        // 0x6FACF5B0
extern void            __fastcall UNIT_FreeUnitGfxData(D2UnitStrc* pUnit);                                                                // 0x6FACF900
extern BOOL            __stdcall  D2Client_AreUnitsHostile_6FACF980(D2UnitStrc* pTargetUnit, D2UnitStrc* pAttackerUnit);                  // 0x6FACF980
extern void            __fastcall D2Client_RemoveUnitFromHashTable_6FAD0620(D2UnitStrc** ppHashTable, int32_t nBucket, int32_t dwUnitId, int32_t nUnitType); // 0x6FAD0620
extern void            __stdcall  D2Client_TeleportClientUnit_6FAD1550(D2UnitStrc* pUnit, int32_t nX, int32_t nY, BOOL bEffect);          // 0x6FAD1550

// UnitSnd & UnitMode helpers
extern void            __stdcall  SOUND_PlayUnitSpeech(D2UnitStrc* pUnit, uint32_t nSpeechId);                                            // 0x6FAE7250
extern void            __stdcall  D2Client_PlayPlayerFootstepSound_6FAE8590(D2UnitStrc* pUnit);                                           // 0x6FAE8590
extern void            __fastcall UNITSND_PlayUnitModeSound(D2UnitStrc* pUnit, int32_t nMode, int32_t bAllowRepeat);                      // 0x6FAE8990
extern BOOL            __fastcall D2Client_CanUnitInterruptCurrentMode(D2UnitStrc* pUnit);                                                // 0x6FAF0C30
extern int32_t         __fastcall D2Client_SetUnitPathTargetServerUnit_6FAF0CA0(D2UnitStrc* pUnit, int32_t nTargetType, uint32_t dwTargetId); // 0x6FAF0CA0
extern int32_t         __fastcall D2Client_SetUnitPathTargetPosition_6FAF0CF0(D2UnitStrc* pUnit, int32_t nTargetX, int32_t nTargetY);     // 0x6FAF0CF0
extern BOOL            __fastcall D2Client_ValidatePathTargetUnit(D2DynamicPathStrc* pDynamicPath);                                       // 0x6FAF0D20
extern BOOL            __stdcall  D2Client_SyncUnitPosition_6FAF0EB0(D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t bCheckDrift, int32_t a5, int32_t a6); // 0x6FAF0EB0
extern void            __fastcall D2Client_MoveUnitToTargetCoords(D2UnitStrc* pUnit, int32_t nTargetX, int32_t nTargetY, BOOL bRun);      // 0x6FAF1160
extern void            __fastcall UNITMODE_OnModeChangeComplete(int32_t nNewMode, D2UnitStrc* pUnit);                                     // 0x6FAF1290
extern void            __fastcall D2Client_ChangeUnitModeWithHitOverlayAndSound(D2UnitStrc* pUnit, int32_t nMode);                        // 0x6FAF13C0
extern BOOL            __fastcall D2Client_ProcessDynamicUnitMovement(D2UnitStrc* pUnit);                                                 // 0x6FAF1400
extern int32_t         __fastcall D2Client_DispatchUnitModeChange_6FAF1450(int32_t nMode, D2UnitStrc* pUnit, void* pModeArgs, int32_t bAllowInterrupt); // 0x6FAF1450
extern void            __stdcall  D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(D2UnitStrc* pUnit, int32_t nArgXOrType, uint32_t dwArgYOrGUID, int32_t nActionCode, BOOL bForce); // 0x6FAF14C0
extern void            __fastcall D2Client_InteractWithServerUnit(int32_t nTargetUnitType, uint32_t dwTargetUnitId);                      // 0x6FAF1910

// Skills, LightMap, Select, Network, UI, and Composite Gfx helpers
extern void            __stdcall  D2Client_ProcessActiveUnitStates_6FAFD9F0(D2UnitStrc* pUnit);                                           // 0x6FAFD9F0
extern void            __stdcall  D2Client_GetUnitCurrentPathTargetCoords_6FAFE320(D2UnitStrc* pUnit, int32_t* pOutX, int32_t* pOutY);    // 0x6FAFE320
extern BOOL            __stdcall  D2Client_StartSkillAtLocation_6FAFF380(D2UnitStrc* pPlayerUnit, D2PlayerModeChangeStrc* pModeChange);   // 0x6FAFF380
extern int32_t         __fastcall D2Client_InvokeClientSkillDoFunc_6FB00150(D2UnitStrc* pPlayerUnit);                                     // 0x6FB00150
extern BOOL            __fastcall D2Client_StartSkillOnTarget_6FB00210(D2UnitStrc* pPlayerUnit, D2PlayerModeChangeStrc* pModeChange);     // 0x6FB00210
extern void            __fastcall D2Client_UpdateBlazeTrail_6FB024E0(D2UnitStrc* pPlayerUnit);                                            // 0x6FB024E0
extern void            __fastcall D2Client_FreeLightMap_6FB0E500(void* pLight);                                                           // 0x6FB0E500
extern void            __stdcall  LIGHTMAP_SetLightColor_6FB0E590(D2GfxLightStrc* pLight, uint8_t nRed, uint8_t nGreen, uint8_t nBlue);   // 0x6FB0E590
extern void            __fastcall LIGHTMAP_SetLightRadiusImmediate_6FB0EA30(D2GfxLightStrc* pLight, int32_t nRadius);                     // 0x6FB0EA30
extern void*           __stdcall  D2Client_AllocLightMap_6FB0EAF0(D2UnitStrc* pUnit, int32_t nLightType, int32_t nRadius, int32_t nIntensity, int32_t nRed, int32_t nGreen, int32_t nBlue); // 0x6FB0EAF0
extern void            __fastcall D2Client_SetMercPetSpecialFlag_6FB1E6E0(D2UnitStrc* pPlayerUnit, D2UnitStrc* pMercUnit, int32_t nFlag); // 0x6FB1E6E0
extern D2UnitStrc*     __fastcall SELECT_GetSelectedUnit(void);                                                                           // 0x6FB1ECA0
extern D2UnitStrc*     __stdcall  SELECT_AcquireSkillTarget_6FB1ED00(D2UnitStrc* pCurrentTarget, D2SkillStrc* pSkill, int32_t* pInOutX, int32_t* pInOutY, uint32_t dwActionFlags, BOOL bFilterMode); // 0x6FB1ED00
extern void            __fastcall SELECT_SetSelectedGroundItem(D2UnitStrc* pUnit);                                                        // 0x6FB1F050
extern void            __fastcall MSG_SendClientPacket(size_t nBytes, const void* pPacket);                                               // 0x6FB291A0
extern void            __fastcall MSG_SendTownFolkInvite_6FB29770(int32_t nUnitType, uint32_t dwUnitId, uint8_t bOpcode, int32_t nX, int32_t nY); // 0x6FB29770
extern void            __cdecl    D2Client_ClearMouseHoldState_6FB29F80(void);                                                            // 0x6FB29F80
extern void            __stdcall  UI_CancelActiveCursorState(void);                                                                       // 0x6FB3B160
extern void            __stdcall  UI_CloseConflictingPanels(int32_t bKeepLeft, int32_t bKeepRight);                                       // 0x6FB3B480
extern void            __fastcall UI_ShutdownGameSession(void);                                                                           // 0x6FB3C690
extern void            __fastcall D2Client_FreeUnitOverlays_6FB40ED0(D2UnitStrc* pUnit);                                                  // 0x6FB40ED0
extern void            __fastcall D2Client_ClearUnitComponentGfx_6FB41550(D2UnitStrc* pUnit, uint8_t nComponent);                         // 0x6FB41550
extern void            __stdcall  D2Client_UpdateUnitAnimGraphics_6FB42000(D2UnitStrc* pUnit, int32_t bForce);                            // 0x6FB42000
extern void            __stdcall  D2Client_SetUnitOverlay_6FB430E0(int32_t nOverlayId, D2UnitStrc* pUnit, int32_t nType, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8); // 0x6FB430E0
extern void            __fastcall D2Client_UpdateState22Effect_6FB4D480(D2UnitStrc* pUnit);                                               // 0x6FB4D480
extern int32_t         __fastcall D2Client_GetPlayerSkillUseState_6FB53610(D2UnitStrc* pPlayerUnit, D2SkillStrc* pSkill);                 // 0x6FB53610

// Earlier Part 4 helpers in UNIT/Player.cpp (0x6FB09540..0x6FB098F0)
extern uint8_t         __stdcall  D2Client_GetPlayerEquippedItemComponent_6FB09540(D2UnitStrc* pItem, D2UnitStrc* pPlayerUnit);           // 0x6FB09540
extern BOOL            __stdcall  D2Client_IsPlayerSkillWithinRange_6FB095D0(D2SkillStrc* pSkill, D2UnitStrc* pTargetUnit, int32_t* pOutMaxRange, int32_t* pOutDistance); // 0x6FB095D0
extern BOOL            __fastcall D2Client_ValidatePlayerInputAction_6FB09650(D2PlayerActionStateStrc* pActionState);                     // 0x6FB09650
extern BOOL            __fastcall D2Client_CanUseSkillInTown_6FB096C0(D2SkillStrc* pSkill);                                               // 0x6FB096C0
extern BOOL            __stdcall  D2Client_ComputePlayerStepMoveTarget_6FB09710(D2UnitStrc* pPlayerUnit, D2PlayerActionStateStrc* pActionState); // 0x6FB09710
extern void            __fastcall D2Client_OnPlayerDeathCleanup_6FB098F0(D2UnitStrc* pPlayerUnit);                                        // 0x6FB098F0

inline const char* Fog_10265(int nErrorCode) { (void)nErrorCode; return FOG_GetCallerReturnAddress(); }

// ============================================================================
// Inline Engine Table & Client PlayerData Queue Helpers (0x6FAB1500, 0x6FAB2C90..0x6FAB2D20)
// ============================================================================

// 1.12a: 0x6FAB1500 (inline helper: eax = nSkillId)
static inline D2SkillsTxt* D2Client_GetSkillsTxtRecordInline(int32_t nSkillId)
{
    if (nSkillId < 0 || !sgptDataTables || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return nullptr;
    }
    return &sgptDataTables->pSkillsTxt[nSkillId];
}

// Helper for reading static/dynamic X coordinate matching exact inlined pattern at 0x6FB099B0, 0x6FB0A748, etc.
static inline int32_t D2Client_GetUnitSubtileXInline(D2UnitStrc* pUnit)
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

// Helper for reading static/dynamic Y coordinate matching exact inlined pattern at 0x6FB099DF, 0x6FB0A774, etc.
static inline int32_t D2Client_GetUnitSubtileYInline(D2UnitStrc* pUnit)
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

// 1.12a: 0x6FAB2C90 (clears D2PlayerDataStrc client action queue fields at +0x150..+0x15C = unk0xA8[42..45])
static inline void D2Client_ClearPlayerQueuedActionInline(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && pUnit->pPlayerData)
    {
        pUnit->pPlayerData->unk0xA8[42] = 0; // +0x150: bQueuedActionReady
        pUnit->pPlayerData->unk0xA8[43] = 0; // +0x154: dwQueuedActionCode
        pUnit->pPlayerData->unk0xA8[44] = 0; // +0x158: dwQueuedTargetTypeOrX
        pUnit->pPlayerData->unk0xA8[45] = 0; // +0x15C: dwQueuedTargetGUIDOrY
    }
}

// 1.12a: 0x6FAB2CD0 (populates D2PlayerDataStrc client action queue fields at +0x150..+0x15C)
static inline void D2Client_SetPlayerQueuedActionInline(D2UnitStrc* pUnit, uint32_t dwActionCode, uint32_t dwArg0, uint32_t dwArg1)
{
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && pUnit->pPlayerData)
    {
        pUnit->pPlayerData->unk0xA8[42] = 0;            // +0x150: bQueuedActionReady (set to 1 when approach path finishes)
        pUnit->pPlayerData->unk0xA8[43] = dwActionCode; // +0x154: dwQueuedActionCode (0x06, 0x0D, 0x13)
        pUnit->pPlayerData->unk0xA8[44] = dwArg0;       // +0x158: dwQueuedTargetType
        pUnit->pPlayerData->unk0xA8[45] = dwArg1;       // +0x15C: dwQueuedTargetGUID
    }
}

// 1.12a: 0x6FAB2D20 (returns D2PlayerDataStrc::unk0xA8[43] at +0x154)
static inline uint32_t D2Client_GetPlayerQueuedActionCodeInline(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && pUnit->pPlayerData)
    {
        return pUnit->pPlayerData->unk0xA8[43];
    }
    return 0;
}

static inline D2UnitStrc* D2Client_FindClientHashUnitInline(int32_t nUnitType, uint32_t dwUnitId)
{
    if (nUnitType < 0 || nUnitType >= UNIT_TYPES_COUNT)
    {
        return nullptr;
    }
    for (D2UnitStrc* pUnit = gapServerSideUnitHashTables[nUnitType][dwUnitId & 0x7F]; pUnit != nullptr; pUnit = pUnit->pListNext)
    {
        if (pUnit->dwUnitId == dwUnitId)
        {
            return pUnit;
        }
    }
    return nullptr;
}

static inline void D2Client_DisplayErrorHaltInline(int32_t nErrorCode)
{
    FOG_DisplayHalt("", Fog_10265(nErrorCode), 0);
    exit(-1);
}

// Forward declarations within this slice
BOOL __fastcall D2Client_ExecutePlayerModeTransition(int32_t nTransitionType, D2UnitStrc* pPlayerUnit, D2PlayerModeChangeStrc* pModeChange, BOOL bAllowInterrupt);
BOOL __stdcall D2Client_ProcessLocalPlayerActionStateMachine(D2PlayerActionStateStrc* pActionState, BOOL bTargeted);
void __fastcall D2Client_ScreenToSubtileCoords(int32_t* pInOutX, int32_t* pInOutY);


extern int32_t  g_bSuppressLeftMouseHold;  // 0x6FBCC048
extern int32_t  g_bActionDispatchedThisFrame; // 0x6FBCC1D8
extern int32_t  gnFrameDeltaTicks;         // 0x6FBB32DC
extern int32_t  g_nLeftMouseButtonState;   // 0x6FB90354
extern int32_t  g_nRightMouseButtonState;  // 0x6FB90358
extern int32_t  gnEnvUnusedSetting_6FBCC3F4; // 0x6FBCC3F4

#define gbObjectOrItemClickLock_6FBCC048 g_bSuppressLeftMouseHold
#define gbPlayerMouseActionLock_6FBCC1D8 g_bActionDispatchedThisFrame
#define gnStepMoveFlag_6FBB32DC          gnFrameDeltaTicks
#define gnLeftMouseBtnFlags_6FB90354     g_nLeftMouseButtonState
#define gnRightMouseBtnFlags_6FB90358    g_nRightMouseButtonState
#define gbLOSDrawEnabled_6FBCC3F4        gnEnvUnusedSetting_6FBCC3F4

int32_t  gbLocalPlayerInTown_6FBC21D4  = 0;
uint32_t gdwCurTickMs_6FBB32A4         = 0;
uint32_t gdwNextWarpClickTick_6FBB32E0 = 0;
int32_t  gnPanelViewportBiasX_6FBC21C8 = 0;

// ============================================================================
// PART 4: Player.cpp (0x6FB099B0 .. 0x6FB0D53F)
// ============================================================================

// 1.12a: 0x6FB099B0 (register ABI: esi = pUnit)
void __fastcall D2Client_UpdateUnitDungeonPops(D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return;
    }

    const int32_t nX = D2Client_GetUnitSubtileXInline(pUnit);
    const int32_t nY = D2Client_GetUnitSubtileYInline(pUnit);
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
    {
        return;
    }
    const BOOL bRoomInvalid = UNITS_IsCurrentRoomInvalid(pUnit);
    DUNGEON_UpdatePops(pRoom, nX, nY, bRoomInvalid);
}

// Compatibility wrapper matching 0-arg declaration in UNIT_Player.cpp
void __stdcall D2Client_UpdateLocalPlayerDungeonPops()
{
    if (gpPlayerUnit)
    {
        D2Client_UpdateUnitDungeonPops(gpPlayerUnit);
    }
}

// 1.12a: 0x6FB09A30 (register ABI: eax = pUnit, edx = nPackedColor)
void __fastcall D2Client_UpdateUnitLightColor(D2UnitStrc* pUnit, int32_t nPackedColor)
{
    if (!pUnit || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER))
    {
        return;
    }

    D2GfxLightStrc* pLight = pUnit->pLight;
    if (!pLight)
    {
        return;
    }

    uint8_t nRed   = 0xFF;
    uint8_t nGreen = 0xFF;
    uint8_t nBlue  = 0xFF;
    if (nPackedColor != 0)
    {
        nRed   = static_cast<uint8_t>(nPackedColor >> 16);
        nGreen = static_cast<uint8_t>(nPackedColor >> 8);
        nBlue  = static_cast<uint8_t>(nPackedColor);
    }

    LIGHTMAP_SetLightColor_6FB0E590(pLight, nRed, nGreen, nBlue);
}

// 1.12a: 0x6FB09A90 (register ABI: eax = pUnit, stack: pRoom, nX, nY; ret 0x0C)
void __fastcall D2Client_SetUnitPositionAndResetPath(D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom, uint32_t nX, int32_t nY)
{
    if (!pUnit)
    {
        return;
    }

    const int32_t nAnimMode = static_cast<int32_t>(pUnit->dwAnimMode);
    D2DynamicPathStrc* pDynamicPath = pUnit->pDynamicPath;
    BOOL bWasMoving = FALSE;
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nSavedTargetX = 0;
    int32_t nSavedTargetY = 0;

    if (nAnimMode == PLRMODE_WALK || nAnimMode == PLRMODE_RUN || nAnimMode == PLRMODE_TOWNWALK)
    {
        bWasMoving = TRUE;
        D2Client_ValidatePathTargetUnit(pUnit->pDynamicPath);
        if (!pUnit->pDynamicPath)
        {
            D2Client_DisplayErrorHaltInline(0x946);
        }

        pTargetUnit = PATH_GetTargetUnit(pUnit->pDynamicPath);
        if (!pTargetUnit)
        {
            D2Client_GetUnitCurrentPathTargetCoords_6FAFE320(pUnit, &nSavedTargetX, &nSavedTargetY);
        }
    }

    D2Common_10230(pDynamicPath, static_cast<int>(reinterpret_cast<intptr_t>(pUnit)), pRoom, nX, static_cast<int16_t>(nY));

    if (bWasMoving)
    {
        D2PlayerModeChangeStrc tModeChange = {};
        int32_t nTransitionType = 0;
        if (pTargetUnit)
        {
            tModeChange.nSkillIdOrXOrType = static_cast<int32_t>(pTargetUnit->dwUnitType);
            tModeChange.dwOwnerOrYOrGUID  = pTargetUnit->dwUnitId;
            nTransitionType = (nAnimMode == PLRMODE_RUN) ? 24 : 0;
        }
        else
        {
            tModeChange.nSkillIdOrXOrType = nSavedTargetX;
            tModeChange.dwOwnerOrYOrGUID  = static_cast<uint32_t>(nSavedTargetY);
            nTransitionType = (nAnimMode == PLRMODE_RUN) ? 23 : 1;
        }

        D2Client_ExecutePlayerModeTransition(nTransitionType, pUnit, &tModeChange, FALSE);
    }
}

// 1.12a: 0x6FB09BB0 (__stdcall, ret 0x08)
BOOL __stdcall D2Client_IsPlayerAlreadyMovingToTarget(int32_t nTargetTypeOrX, uint32_t dwTargetIdOrY)
{
    D2UnitStrc* pPlayer = gpPlayerUnit;
    if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER || !pPlayer->pPlayerData)
    {
        return FALSE;
    }

    if (pPlayer->pPlayerData->unk0xA8[43] != 0x13)
    {
        return FALSE;
    }

    const uint32_t dwQueuedArg0 = pPlayer->pPlayerData->unk0xA8[44];
    const uint32_t dwQueuedArg1 = pPlayer->pPlayerData->unk0xA8[45];
    if (dwQueuedArg0 != static_cast<uint32_t>(nTargetTypeOrX) || dwQueuedArg1 != dwTargetIdOrY)
    {
        return FALSE;
    }

    return TRUE;
}

// 1.12a: 0x6FB09C10 (register ABI: edi = pUnit, stack: nRadiusBonus; ret 0x04)
void __fastcall D2Client_UpdateUnitLightRadius(D2UnitStrc* pUnit, int32_t nRadiusBonus)
{
    if (!pUnit || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER))
    {
        return;
    }

    D2GfxLightStrc* pLight = pUnit->pLight;
    if (!pLight)
    {
        return;
    }

    int32_t nBaseRadius = 13;
    if (pUnit->dwUnitType != UNIT_PLAYER)
    {
        D2MonStats2Txt* pMonStats2 = ENGINE_GetMonStats2TxtRecord(pUnit->dwClassId);
        if (!pMonStats2)
        {
            return;
        }

        nBaseRadius = static_cast<int32_t>(MONSTERS_GetMaximalLightRadius(pUnit));
        const int32_t nMinMonLight = static_cast<int32_t>(pMonStats2->nLight);
        if (nBaseRadius < nMinMonLight)
        {
            nBaseRadius = nMinMonLight;
        }
    }

    int32_t nTotalRadius = nBaseRadius + nRadiusBonus;
    if (nTotalRadius <= 0)
    {
        nTotalRadius = 0;
    }

    LIGHTMAP_SetLightRadiusImmediate_6FB0EA30(pLight, nTotalRadius);
}

// 1.12a: 0x6FB09C70 (register ABI: ecx = pActionState; ret 0x00)
int32_t __fastcall D2Client_HandleMercenaryOrPetRightClick(D2PlayerActionStateStrc* pActionState)
{
    D2Client_ValidatePlayerInputAction_6FB09650(pActionState);

    D2UnitStrc* pPlayer = pActionState ? pActionState->pPlayerUnit : nullptr;
    D2ClientRosterPetStrc* pPetHead = reinterpret_cast<D2ClientRosterPetStrc*>(gpRosterPetList);
    if (!pPlayer || !pPetHead)
    {
        return 0;
    }

    const uint32_t dwPlayerId = pPlayer->dwUnitId;
    D2ClientRosterPetStrc* pMercPet = pPetHead;
    while (pMercPet)
    {
        if (pMercPet->nPetType == 7 && pMercPet->dwOwnerId == dwPlayerId && pMercPet->bIsDeadHireling == 0)
        {
            break;
        }
        pMercPet = pMercPet->pNext;
    }

    if (!pMercPet)
    {
        return 0;
    }

    const uint32_t dwMercUnitId = pMercPet->dwUnitId;
    if (dwMercUnitId == D2UnitInvalidGUID)
    {
        return 0;
    }

    D2UnitStrc* pMercUnit = D2Client_FindClientHashUnitInline(UNIT_MONSTER, dwMercUnitId);
    if (!pMercUnit)
    {
        return 0;
    }

    if (pMercUnit->dwUnitType != UNIT_MONSTER)
    {
        D2Client_DisplayErrorHaltInline(0x5A);
    }

    for (D2ClientRosterPetStrc* pNode = pPetHead; pNode != nullptr; pNode = pNode->pNext)
    {
        if (pNode->dwUnitId == dwMercUnitId)
        {
            if (pNode->dwSpecialFlag == 2)
            {
                D2Client_SetMercPetSpecialFlag_6FB1E6E0(pPlayer, pMercUnit, 0);
            }
            break;
        }
    }

    return 0;
}

// 1.12a: 0x6FB09D30 (__fastcall StatListValueChangeFunc, ret 0x10)
// Uses 5-entry jump table at 0x6FB09EA4 and 116-byte index table at 0x6FB09EB8 (stats 89..204).
void __fastcall D2Client_PlayerStatChangeCallback(
    D2GameStrc* pGame,
    D2UnitStrc* pUnit,
    D2UnitStrc* pItem,
    int32_t nStatAndLayer,
    int32_t nPreviousValue,
    int32_t nNewValue)
{
    (void)pGame;
    (void)nPreviousValue;

    if (!pUnit)
    {
        return;
    }

    const uint32_t dwStatId = static_cast<uint32_t>(nStatAndLayer) >> 16;
    const uint16_t wLayer   = static_cast<uint16_t>(nStatAndLayer);

    switch (dwStatId)
    {
    case STAT_ITEM_LIGHTRADIUS: // 89 (jt[0] = 0x6FB09D64)
        D2Client_UpdateUnitLightRadius(pUnit, nNewValue);
        break;

    case STAT_ITEM_LIGHTCOLOR: // 90 (jt[1] = 0x6FB09D74)
        D2Client_UpdateUnitLightColor(pUnit, nNewValue);
        break;

    case STAT_ITEM_NONCLASSSKILL: // 97  (jt[2] = 0x6FB09D85)
    case STAT_ITEM_SINGLESKILL:   // 107 (jt[2] = 0x6FB09D85)
    {
        if (pUnit->dwUnitType != UNIT_PLAYER)
        {
            break;
        }

        const int32_t nSkillId = static_cast<int32_t>(wLayer);
        D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, D2UnitInvalidGUID);
        if (nNewValue == 0)
        {
            if (pSkill && SKILLS_GetSkillLevel(pUnit, pSkill, FALSE) <= 0)
            {
                SKILLS_RemoveSkill(pUnit, nSkillId, "..\\Source\\D2Client\\UNIT\\Player.cpp", 221);
            }
        }
        else if (!pSkill)
        {
            D2SkillsTxt* pSkillsTxt = D2Client_GetSkillsTxtRecordInline(nSkillId);
            if (dwStatId == STAT_ITEM_NONCLASSSKILL
                || (pSkillsTxt && static_cast<int32_t>(pSkillsTxt->nCharClass) == pUnit->dwClassId))
            {
                SKILLS_AssignSkill(pUnit, nSkillId, 0, FALSE, "..\\Source\\D2Client\\UNIT\\Player.cpp", 232);
            }
        }
        break;
    }

    case STAT_ITEM_CHARGED_SKILL: // 204 (jt[3] = 0x6FB09E1A)
    {
        if (pUnit->dwUnitType != UNIT_PLAYER || !pItem || !sgptDataTables)
        {
            break;
        }

        const int32_t nSkillId    = static_cast<int32_t>(wLayer) >> sgptDataTables->nStuff;
        const int32_t nSkillLevel = static_cast<int32_t>(wLayer) & sgptDataTables->nShiftedStuff;
        const D2UnitGUID dwItemId = pItem->dwUnitId;
        const int32_t nCharges    = STATLIST_UnitGetStatValue(pItem, static_cast<int>(dwStatId), wLayer) & 0xFF;

        if (nCharges > 0 && STATLIST_GetOwner(pItem, nullptr) == pUnit)
        {
            D2Common_10954(pUnit, dwItemId, nSkillId, nSkillLevel, nCharges, FALSE);
        }
        else
        {
            D2Common_10954(pUnit, dwItemId, nSkillId, nSkillLevel, 0, TRUE);
        }
        break;
    }

    default: // jt[4] = 0x6FB09E9C
        break;
    }
}

// 1.12a: 0x6FB09F30 (register ABI: eax = nSkillUseState, stack: pPlayerUnit; ret 0x04)
// Plays class voice bark from g_aSkillUseStateSpeechTable_6FB947C8 when a skill cannot be used.
// Kept with legacy alias D2Client_ClearPlayerBearWolfTransformState for compatibility with UNIT_Player.cpp.
void __fastcall D2Client_PlaySkillUseStateSpeechBark(int32_t nSkillUseState, D2UnitStrc* pPlayerUnit)
{
    if (nSkillUseState < 0 || nSkillUseState >= g_nSkillUseStateSpeechCount_6FB947EC)
    {
        D2Client_DisplayErrorHaltInline(0x2CB);
    }

    const uint16_t wSpeechId = g_aSkillUseStateSpeechTable_6FB947C8[nSkillUseState].wSpeechId;
    if (wSpeechId != 0)
    {
        SOUND_PlayUnitSpeech(pPlayerUnit, wSpeechId);
    }
}

void __fastcall D2Client_ClearPlayerBearWolfTransformState(int32_t nSkillUseState, D2UnitStrc* pPlayerUnit)
{
    D2Client_PlaySkillUseStateSpeechBark(nSkillUseState, pPlayerUnit);
}

// 1.12a: 0x6FB09F80 (register ABI: eax = pPlayerUnit, ebx = ppSkill, stack: pOutUseState; ret 0x04)
BOOL __fastcall D2Client_CheckPlayerSkillUseStateAndPlayBark(D2UnitStrc* pPlayerUnit, D2SkillStrc** ppSkill, int32_t* pOutUseState)
{
    if (pPlayerUnit != gpPlayerUnit)
    {
        return TRUE;
    }

    if (!ppSkill || !*ppSkill)
    {
        return FALSE;
    }

    int32_t nUseState = D2Client_GetPlayerSkillUseState_6FB53610(pPlayerUnit, *ppSkill);
    if (pOutUseState)
    {
        *pOutUseState = nUseState;
    }

    D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(*ppSkill);
    if (!pSkillsTxt)
    {
        return FALSE;
    }

    // If out of mana/ammo (states 1, 2, 4) and skill allows fallback to normal Attack (bit 28 = byte +0x07 bit 4)
    if (nUseState > 0 && (nUseState <= 2 || nUseState == 4))
    {
        const uint8_t nHighFlagsByte = static_cast<uint8_t>(pSkillsTxt->dwFlags[0] >> 24);
        if ((nHighFlagsByte & static_cast<uint8_t>(gdwBitMasks[4])) != 0)
        {
            *ppSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pPlayerUnit, 0);
            nUseState = D2Client_GetPlayerSkillUseState_6FB53610(pPlayerUnit, *ppSkill);
        }
    }

    if (nUseState == 5 || nUseState == 0)
    {
        return TRUE;
    }

    D2Client_PlaySkillUseStateSpeechBark(nUseState, pPlayerUnit);
    return FALSE;
}

// 1.12a: 0x6FB0A020 (register ABI: eax = pPlayerUnit, esi = pModeChange, stack: pOutUseState; ret 0x04)
BOOL __fastcall D2Client_ValidatePlayerActionSkillUseState(D2UnitStrc* pPlayerUnit, D2PlayerModeChangeStrc* pModeChange, int32_t* pOutUseState)
{
    D2SkillStrc* pSkill = SKILLS_GetSkillById(
        pPlayerUnit,
        pModeChange->nSkillIdOrXOrType,
        pModeChange->dwOwnerOrYOrGUID);

    if (pSkill)
    {
        if (!D2Client_CheckPlayerSkillUseStateAndPlayBark(pPlayerUnit, &pSkill, pOutUseState))
        {
            return FALSE;
        }

        const int32_t nOldSkillId = pModeChange->nSkillIdOrXOrType;
        const int32_t nNewSkillId = SKILLS_GetSkillIdFromSkill(pSkill, "..\\Source\\D2Client\\UNIT\\Player.cpp", 812);
        pModeChange->nSkillIdOrXOrType = nNewSkillId;
        if (nNewSkillId != nOldSkillId)
        {
            pModeChange->dwOwnerOrYOrGUID = D2UnitInvalidGUID;
        }
    }

    return TRUE;
}

// 1.12a: 0x6FB0A090 (register ABI: eax = pActionState, ecx = pOutUseState; ret 0x00)
BOOL __fastcall D2Client_CheckActionStateSkillUseAndPlayBark(D2PlayerActionStateStrc* pActionState, int32_t* pOutUseState)
{
    return D2Client_CheckPlayerSkillUseStateAndPlayBark(pActionState->pPlayerUnit, &pActionState->pSkill, pOutUseState);
}

BOOL __stdcall D2Client_AlwaysTrueSkillHook()
{
    return TRUE;
}

// 1.12a: 0x6FB0A0A0 (__stdcall, ret 0x00)
void __stdcall D2Client_RefreshMonsterUnitsWithState63InPlayerRooms()
{
    if (!gpPlayerUnit)
    {
        return;
    }

    D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(gpPlayerUnit);
    if (!pPlayerRoom)
    {
        return;
    }

    D2ActiveRoomStrc** ppRoomList = nullptr;
    int32_t nNumRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pPlayerRoom, &ppRoomList, &nNumRooms);

    for (int32_t i = 0; i < nNumRooms; ++i)
    {
        D2ActiveRoomStrc* pRoom = ppRoomList[i];
        for (D2UnitStrc* pUnit = pRoom ? pRoom->pUnitFirst : nullptr; pUnit != nullptr; pUnit = pUnit->pRoomNext)
        {
            if (pUnit->dwUnitType == UNIT_MONSTER && STATES_CheckState(pUnit, 63))
            {
                D2Client_UpdateUnitAnimGraphics_6FB42000(pUnit, 0);
            }
        }
    }
}

// 1.12a: 0x6FB0A120 (register ABI: eax = pPlayerUnit; ret 0x00)
void __fastcall D2Client_FreePlayerUnitClientSide(D2UnitStrc* pPlayerUnit)
{
    if (!pPlayerUnit)
    {
        return;
    }

    for (int32_t i = 0; i < 8; ++i)
    {
        if (gPlayerUnits_6FBCB6D4[i] == pPlayerUnit)
        {
            gPlayerUnits_6FBCB6D4[i] = nullptr;
            break;
        }
    }

    D2Client_FreeUnitOverlays_6FB40ED0(pPlayerUnit);

    if (gpClientAct)
    {
        D2Common_10228(pPlayerUnit);
        UNITROOM_RemoveUnitFromRoom(pPlayerUnit);
    }

    PATH_FreeDynamicPath(nullptr, pPlayerUnit->pDynamicPath);
    pPlayerUnit->pDynamicPath = nullptr;

    D2Client_FreeUnitInventoryItems_6FACF290(pPlayerUnit, FALSE);
    INVENTORY_FreeInventory(pPlayerUnit->pInventory);
    pPlayerUnit->pInventory = nullptr;

    void* pSkillLight = D2Common_10958(pPlayerUnit, nullptr);
    if (pSkillLight)
    {
        D2Client_FreeLightMap_6FB0E500(pSkillLight);
    }

    if (pPlayerUnit->pLight)
    {
        D2Client_FreeLightMap_6FB0E500(pPlayerUnit->pLight);
        pPlayerUnit->pLight = nullptr;
    }

    if (pPlayerUnit == gpPlayerUnit)
    {
        if (gpPlayerUnit)
        {
            UI_ShutdownGameSession();
        }
        gpPlayerUnit = nullptr;
    }

    UNITS_FreePlayerData(nullptr, pPlayerUnit);
    D2Client_RefreshMonsterUnitsWithState63InPlayerRooms();
}

// 1.12a: 0x6FB0A1D0 (register ABI: ecx = pPlayerUnit, eax = pRoom, stack: nX, nY; ret 0x08)
void __fastcall D2Client_InitPlayerUnitClientSide(D2UnitStrc* pPlayerUnit, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY)
{
    if (!pPlayerUnit)
    {
        return;
    }

    for (int32_t i = 0; i < 8; ++i)
    {
        if (!gPlayerUnits_6FBCB6D4[i])
        {
            gPlayerUnits_6FBCB6D4[i] = pPlayerUnit;
            break;
        }
    }

    PATH_AllocDynamicPath(nullptr, pRoom, nX, nY, pPlayerUnit, TRUE);

    if (!pPlayerUnit->pInventory)
    {
        pPlayerUnit->pInventory = INVENTORY_AllocInventory(nullptr, pPlayerUnit);
    }

    pPlayerUnit->pSkills = SKILLS_AllocSkillList(nullptr);
    SKILLS_InitSkillList(pPlayerUnit);

    STATLIST_AllocStatListEx(pPlayerUnit, 0, D2Client_PlayerStatChangeCallback, nullptr);
    STATLIST_SetUnitStat(pPlayerUnit, STAT_ATTACKRATE, 100, 0);
    STATLIST_SetUnitStat(pPlayerUnit, STAT_VELOCITYPERCENT, 100, 0);
    STATLIST_SetUnitStat(pPlayerUnit, STAT_OTHER_ANIMRATE, 100, 0);

    gbLocalPlayerInTown_6FBC21D4 = DUNGEON_IsRoomInTown(pRoom);
    UNITS_ChangeAnimMode(pPlayerUnit, PLRMODE_TOWNNEUTRAL);

    pPlayerUnit->dwFlags = (pPlayerUnit->dwFlags & ~UNITFLAG_DRAWSHADOW)
        | (UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED | UNITFLAG_ISVALIDTARGET);

    if (pPlayerUnit->dwAnimMode == PLRMODE_DEAD || pPlayerUnit->dwAnimMode == PLRMODE_DEATH)
    {
        PATH_RemoveCollisionFootprintForUnit(pPlayerUnit, TRUE);
        PATH_SetUnitCollisionPattern(pPlayerUnit, 5);
        PATH_SetFootprintCollisionMask(pPlayerUnit->pDynamicPath, 0x8000);
    }

    D2UnitStrc* pLocalPlayer = gpPlayerUnit;
    if (D2COMMON_10083_Return0(pRoom) == 0)
    {
        const int32_t nLightType = (pLocalPlayer && pPlayerUnit != pLocalPlayer) ? 1 : 0;
        D2GfxLightStrc* pLight = static_cast<D2GfxLightStrc*>(D2Client_AllocLightMap_6FB0EAF0(pPlayerUnit, nLightType, 13, 0xFF, 0xFF, 0xFF, 0xFF));
        if (pLight)
        {
            pPlayerUnit->pLight = pLight;
            const D2ClientLightNodeViewStrc* pLightView = reinterpret_cast<const D2ClientLightNodeViewStrc*>(pLight);
            pPlayerUnit->dwStartLight = static_cast<uint32_t>(pLightView->nRadiusFixed >> 3);
        }
    }

    UNITS_AllocPlayerData(pPlayerUnit);

    if (pPlayerUnit != pLocalPlayer)
    {
        const uint64_t qwNextSeed = static_cast<uint64_t>(pPlayerUnit->pSeed.nLowSeed) * 0x6AC690C5ULL
            + static_cast<uint64_t>(pPlayerUnit->pSeed.nHighSeed);
        pPlayerUnit->pSeed.nLowSeed  = static_cast<uint32_t>(qwNextSeed);
        pPlayerUnit->pSeed.nHighSeed = static_cast<uint32_t>(qwNextSeed >> 32);
        PATH_SetDirection(pPlayerUnit->pDynamicPath, static_cast<uint8_t>(pPlayerUnit->pSeed.nLowSeed & 0x3F));
    }

    D2Client_RefreshMonsterUnitsWithState63InPlayerRooms();
}

// 1.12a: 0x6FB0A340 (register ABI: eax = pCorpseAssignPacket; ret 0x00)
void __fastcall D2Client_InitPlayerCorpseEquipmentAndColor(const D2GSPacketSrv74CorpseAssignStrc* pPacket)
{
    if (!pPacket || !pPacket->bAssign)
    {
        return;
    }

    D2UnitStrc* pPlayerUnit = D2Client_FindClientHashUnitInline(UNIT_PLAYER, pPacket->dwPlayerUnitId);
    if (!pPlayerUnit)
    {
        return;
    }

    if (pPlayerUnit->dwUnitType != UNIT_PLAYER)
    {
        D2Client_DisplayErrorHaltInline(0x5A);
    }

    if (!(pPlayerUnit->dwFlags & UNITFLAG_ISDEAD)
        && pPlayerUnit->dwAnimMode != PLRMODE_DEATH
        && pPlayerUnit->dwAnimMode != PLRMODE_DEAD)
    {
        D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, PLRMODE_DEATH);
    }

    if (pPlayerUnit == gpPlayerUnit)
    {
        D2UnitStrc* pCorpseUnit = D2Client_FindClientHashUnitInline(UNIT_PLAYER, pPacket->dwCorpseUnitId);
        if (!pCorpseUnit)
        {
            return;
        }

        if (pCorpseUnit->dwUnitType != UNIT_PLAYER)
        {
            D2Client_DisplayErrorHaltInline(0x5A);
        }

        PATH_SetMoveTestCollisionMask(pCorpseUnit->pDynamicPath, 0);
        PATH_SetUnitCollisionPattern(pCorpseUnit, 5);

        const uint32_t dwPlayerX = static_cast<uint32_t>(D2Client_GetUnitX_6FAB1590(pPlayerUnit));
        const uint32_t dwPlayerY = static_cast<uint32_t>(D2Client_GetUnitY_6FAB15C0(pPlayerUnit));
        D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(pPlayerUnit);
        if (!D2Common_10229(pCorpseUnit->pDynamicPath, pCorpseUnit, pPlayerRoom, dwPlayerX, dwPlayerY))
        {
            D2Client_DisplayErrorHaltInline(0xABA);
        }

        PATH_SetDirection(pCorpseUnit->pDynamicPath, PATH_GetDirection(pPlayerUnit->pDynamicPath));
    }

    D2InventoryStrc* pInventory = pPlayerUnit->pInventory;
    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pInventory); pItem != nullptr; )
    {
        D2UnitStrc* pNextItem = INVENTORY_GetNextItem(pItem);
        if (INVENTORY_GetItemNodePage(pItem) == NODEPAGE_EQUIP)
        {
            pItem = INVENTORY_UnitIsItem(pItem);
            INVENTORY_UpdateWeaponGUIDOnRemoval(pInventory, pItem);

            const uint8_t nComp = D2Client_GetPlayerEquippedItemComponent_6FB09540(pItem, pPlayerUnit);
            D2Client_ClearUnitComponentGfx_6FB41550(pPlayerUnit, nComp);

            if (!INVENTORY_CompareWithItemsParentInventory(pInventory, pItem))
            {
                D2Client_DisplayErrorHaltInline(0xAC8);
            }

            if (pPlayerUnit->dwUnitType == UNIT_PLAYER)
            {
                INVENTORY_GetLeftHandWeapon(pPlayerUnit->pInventory);
            }

            if (!ITEMS_CheckItemFlag(pItem, 0x100, 150, "..\\Source\\D2Client\\UNIT\\Item.cpp"))
            {
                STATLIST_ExpireUnitStatlist(pPlayerUnit, pItem);
            }

            const uint8_t nBodyLoc = ITEMS_GetBodyLocation(pItem);
            D2COMMON_10254_Return0(reinterpret_cast<int>(pInventory), nBodyLoc);
            D2Client_UpdateUnitAnimGraphics_6FB42000(pPlayerUnit, 0);

            const int32_t nItemType = pItem ? static_cast<int32_t>(pItem->dwUnitType) : UNIT_TYPES_COUNT;
            const int32_t nItemId   = pItem ? static_cast<int32_t>(pItem->dwUnitId)   : -1;
            if (nItemType >= 0 && nItemType < UNIT_TYPES_COUNT)
            {
                D2Client_RemoveUnitFromHashTable_6FAD0620(
                    &gapServerSideUnitHashTables[nItemType][0],
                    nItemId & 0x7F,
                    nItemId,
                    nItemType);
            }
        }
        pItem = pNextItem;
    }
}

// 1.12a: 0x6FB0A570 (register ABI: esi = pActionState; ret 0x00)
BOOL __fastcall D2Client_PreparePlayerSkillInputAction(D2PlayerActionStateStrc* pActionState)
{
    const int32_t nSkillTargetState = D2Common_11016(pActionState->pPlayerUnit, pActionState->pSkill);

    if (pActionState->pSkill
        && (pActionState->dwFlags & PLAYERACT_RIGHT_HAND)
        && nSkillTargetState != 1)
    {
        D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pActionState->pSkill);
        if (!pSkillsTxt)
        {
            return FALSE;
        }

        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pActionState->pPlayerUnit);
        const uint8_t nFlagsByte1 = static_cast<uint8_t>(pSkillsTxt->dwFlags[0] >> 8);
        if ((nFlagsByte1 & static_cast<uint8_t>(gdwBitMasks[0])) == 0 && DUNGEON_IsRoomInTown(pRoom))
        {
            SOUND_PlayUnitSpeech(pActionState->pPlayerUnit, 24);
            return FALSE;
        }

        const uint8_t nFlagsByte0 = static_cast<uint8_t>(pSkillsTxt->dwFlags[0]);
        if ((nFlagsByte0 & static_cast<uint8_t>(gdwBitMasks[4])) != 0)
        {
            return FALSE;
        }
    }

    if (pActionState->dwFlags & PLAYERACT_FORCE_RUN)
    {
        pActionState->nLocationMoveMode = 3;
        pActionState->nTargetMoveMode   = 4;
    }
    else
    {
        pActionState->nLocationMoveMode = 1;
        pActionState->nTargetMoveMode   = 2;
    }

    D2UnitStrc* pPlayer = pActionState->pPlayerUnit;
    const uint32_t dwQueuedCode = D2Client_GetPlayerQueuedActionCodeInline(pPlayer);
    if (dwQueuedCode != 0)
    {
        if (dwQueuedCode != 0x13)
        {
            D2Client_ClearPlayerQueuedActionInline(pPlayer);
        }
        else
        {
            const int32_t nQueuedType = static_cast<int32_t>(pPlayer->pPlayerData->unk0xA8[44]);
            const uint32_t dwQueuedId = pPlayer->pPlayerData->unk0xA8[45];
            if (nQueuedType >= 0 && nQueuedType < UNIT_TYPES_COUNT
                && D2Client_FindUnitInHashTable_6FACE550(
                    &gapServerSideUnitHashTables[nQueuedType][0],
                    static_cast<int32_t>(dwQueuedId & 0x7F),
                    nQueuedType))
            {
                D2Client_ClearPlayerQueuedActionInline(pPlayer);
            }
        }
    }

    if (!pActionState->pSkill)
    {
        return FALSE;
    }

    BOOL bFilterMode = (pActionState->dwFlags >> 5) & 1;
    if ((pActionState->dwFlags & PLAYERACT_RIGHT_HAND) && !pActionState->pTargetUnit)
    {
        bFilterMode = TRUE;
    }

    pActionState->pTargetUnit = SELECT_AcquireSkillTarget_6FB1ED00(
        pActionState->pTargetUnit,
        pActionState->pSkill,
        &pActionState->nTargetX,
        &pActionState->nTargetY,
        pActionState->dwFlags,
        bFilterMode);

    if (gnCursorState == 6)
    {
        UI_CancelActiveCursorState();
        return TRUE;
    }

    if (gnCursorState == 8)
    {
        D2C2SPacketByteDwordStrc tPacket;
        tPacket.nHeader = 0x4C;
        tPacket.dwArg   = D2UnitInvalidGUID;
        MSG_SendClientPacket(sizeof(tPacket), &tPacket);
    }

    return TRUE;
}

// 1.12a: 0x6FB0A700 (register ABI: ecx = pPlayerUnit, eax = nNeutralMode, stack: bSyncPosition; ret 0x04)
void __fastcall D2Client_SetPlayerNeutralModeAndSyncPos(D2UnitStrc* pPlayerUnit, int32_t nNeutralMode, BOOL bSyncPosition)
{
    if (pPlayerUnit)
    {
        pPlayerUnit->dwFlags &= ~UNITFLAG_SKSRVDOFUNC;
    }

    if (UNITS_ChangeAnimMode(pPlayerUnit, nNeutralMode))
    {
        D2Client_UpdateUnitAnimGraphics_6FB42000(pPlayerUnit, 0);
        UNITMODE_OnModeChangeComplete(nNeutralMode, pPlayerUnit);
        UNITSND_PlayUnitModeSound(pPlayerUnit, nNeutralMode, 0);
    }

    if (bSyncPosition && pPlayerUnit == gpPlayerUnit)
    {
        const int32_t nX = D2Client_GetUnitSubtileXInline(pPlayerUnit);
        const int32_t nY = D2Client_GetUnitSubtileYInline(pPlayerUnit);
        if (nX != 0 && nY != 0)
        {
            D2Client_SyncUnitPosition_6FAF0EB0(pPlayerUnit, nX, nY, 0, 0, 0);
        }
    }
}

// 1.12a: 0x6FB0A7C0 (register ABI: eax = pPlayerUnit, stack: bSkipIfNotActive; ret 0x04)
void __fastcall D2Client_TriggerPlayerUsedSkillEnd(D2UnitStrc* pPlayerUnit, BOOL bSkipIfNotActive)
{
    D2SkillStrc* pUsedSkill = UNITS_GetUsedSkill(pPlayerUnit);
    if (!pUsedSkill)
    {
        return;
    }

    const int32_t nFlags = SKILLS_GetFlags(pUsedSkill);
    if (nFlags & 1)
    {
        SKILLS_SetFlags(pUsedSkill, nFlags | 2);
        D2Client_InvokeClientSkillDoFunc_6FB00150(pPlayerUnit);
        return;
    }

    if (!bSkipIfNotActive)
    {
        if (!pPlayerUnit || ((pPlayerUnit->dwFlags >> 6) & 1) == 0)
        {
            D2Client_InvokeClientSkillDoFunc_6FB00150(pPlayerUnit);
        }
    }
}

// 1.12a: 0x6FB0A820 (__fastcall: ecx = nTransitionType, edx = pPlayerUnit, stack: pModeChange, bAllowInterrupt; ret 0x08)
// Central 26-entry player mode transition dispatcher (jump table 0x6FB0AD08, index table 0x6FB0AD48).
BOOL __fastcall D2Client_ExecutePlayerModeTransition(
    int32_t nTransitionType,
    D2UnitStrc* pPlayerUnit,
    D2PlayerModeChangeStrc* pModeChange,
    BOOL bAllowInterrupt)
{
    void* pSkillLight = D2Common_10958(pPlayerUnit, nullptr);
    if (pSkillLight)
    {
        D2Client_FreeLightMap_6FB0E500(pSkillLight);
    }

    if (nTransitionType != 19)
    {
        D2Client_TriggerPlayerUsedSkillEnd(pPlayerUnit, (pPlayerUnit == gpPlayerUnit) ? TRUE : FALSE);
        UNITS_SetUsedSkill(pPlayerUnit, nullptr);
    }

    D2DynamicPathStrc* pDynamicPath = pPlayerUnit->pDynamicPath;
    if (pPlayerUnit->dwAnimMode == PLRMODE_KNOCKBACK)
    {
        if (!bAllowInterrupt)
        {
            return TRUE;
        }
        PATH_RecacheRoomIfNeeded(pDynamicPath);
    }

    PATH_ResetToPreviousType(pDynamicPath);

    const BOOL bInTown = DUNGEON_IsRoomInTown(UNITS_GetRoom(pPlayerUnit));
    const int32_t nNeutralMode = bInTown ? PLRMODE_TOWNNEUTRAL : PLRMODE_NEUTRAL;
    const int32_t nWalkMode    = bInTown ? PLRMODE_TOWNWALK    : PLRMODE_WALK;

    switch (nTransitionType)
    {
    case 0: // jt[0] = 0x6FB0A953: Walk to target unit
        if (!D2Client_SetUnitPathTargetServerUnit_6FAF0CA0(
                pPlayerUnit,
                pModeChange->nSkillIdOrXOrType,
                pModeChange->dwOwnerOrYOrGUID))
        {
            D2Client_SetPlayerNeutralModeAndSyncPos(pPlayerUnit, nNeutralMode, TRUE);
            return FALSE;
        }
        D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, nWalkMode);
        return TRUE;

    case 1: // jt[1] = 0x6FB0A929: Walk to coordinates
        if (!D2Client_SetUnitPathTargetPosition_6FAF0CF0(
                pPlayerUnit,
                pModeChange->nSkillIdOrXOrType,
                static_cast<int32_t>(pModeChange->dwOwnerOrYOrGUID)))
        {
            D2Client_SetPlayerNeutralModeAndSyncPos(pPlayerUnit, nNeutralMode, TRUE);
            return FALSE;
        }
        D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, nWalkMode);
        return TRUE;

    case 2: // jt[2] = 0x6FB0AC9A: Interact with unit
        D2Client_InteractWithServerUnit(
            static_cast<uint16_t>(pModeChange->nSkillIdOrXOrType),
            pModeChange->dwOwnerOrYOrGUID);
        return TRUE;

    case 6: // jt[3] = 0x6FB0A9C1: GetHit (PLRMODE_GETHIT)
        pPlayerUnit->dwLastHitClass = static_cast<uint32_t>(pModeChange->nTargetXOrTypeOrHit);
        D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, PLRMODE_GETHIT);
        D2Client_SyncUnitPosition_6FAF0EB0(
            pPlayerUnit,
            static_cast<uint16_t>(pModeChange->nSkillIdOrXOrType),
            static_cast<uint16_t>(pModeChange->dwOwnerOrYOrGUID),
            1, 0, 0);
        return TRUE;

    case 7: // jt[4] = 0x6FB0AC29: Neutral / Realign
    {
        const BOOL bWasDead = D2Client_IsUnitDead_6FACEAC0(pPlayerUnit);
        UNIT_FreeUnitGfxData(pPlayerUnit);
        pPlayerUnit->dwFlags = (pPlayerUnit->dwFlags & ~UNITFLAG_SKSRVDOFUNC) | UNITFLAG_TARGETABLE;
        if (UNITS_ChangeAnimMode(pPlayerUnit, nNeutralMode))
        {
            D2Client_UpdateUnitAnimGraphics_6FB42000(pPlayerUnit, 0);
            UNITMODE_OnModeChangeComplete(nNeutralMode, pPlayerUnit);
            UNITSND_PlayUnitModeSound(pPlayerUnit, nNeutralMode, 0);
        }
        if (pModeChange)
        {
            D2Client_MoveUnitToTargetCoords(
                pPlayerUnit,
                pModeChange->nSkillIdOrXOrType,
                static_cast<int32_t>(pModeChange->dwOwnerOrYOrGUID),
                bWasDead);
        }
        return TRUE;
    }

    case 8: // jt[5] = 0x6FB0AB7D: Death (PLRMODE_DEATH)
        if (pPlayerUnit == gpPlayerUnit)
        {
            UI_CloseConflictingPanels(1, 0);
        }
        pPlayerUnit->dwLastHitClass = static_cast<uint32_t>(pModeChange->nTargetXOrTypeOrHit);
        if (SELECT_GetSelectedUnit() == pPlayerUnit)
        {
            if ((gnLeftMouseBtnFlags_6FB90354 & 0x08) || (gnRightMouseBtnFlags_6FB90358 & 0x08))
            {
                D2Client_ClearMouseHoldState_6FB29F80();
                gnSelectedTargetB = 1;
            }
            else
            {
                gnSelectedTargetB = 0;
            }
            gnSelectedTargetA = 0;
        }
        D2Client_SyncUnitPosition_6FAF0EB0(
            pPlayerUnit,
            static_cast<uint16_t>(pModeChange->nSkillIdOrXOrType),
            static_cast<uint16_t>(pModeChange->dwOwnerOrYOrGUID),
            0, 0, 0);
        D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, PLRMODE_DEATH);
        D2Client_OnPlayerDeathCleanup_6FB098F0(pPlayerUnit);
        D2Client_ClearPlayerQueuedActionInline(pPlayerUnit);
        return TRUE;

    case 9: // jt[6] = 0x6FB0AC0B: Corpse / Dead (PLRMODE_DEAD)
        D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, PLRMODE_DEAD);
        D2Client_OnPlayerDeathCleanup_6FB098F0(pPlayerUnit);
        return TRUE;

    case 18: // jt[7] = 0x6FB0AB16: Block (PLRMODE_BLOCK)
    {
        pPlayerUnit->dwLastHitClass = static_cast<uint32_t>(pModeChange->nTargetXOrTypeOrHit);
        int32_t nWeaponClassId = 0;
        if (INVENTORY_GetEquippedShield(pPlayerUnit->pInventory, nullptr)
            || (COMPOSIT_GetWeaponClassId(pPlayerUnit, pPlayerUnit->pInventory, &nWeaponClassId, -1, TRUE), nWeaponClassId == 13))
        {
            D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, PLRMODE_BLOCK);
        }
        D2Client_SyncUnitPosition_6FAF0EB0(
            pPlayerUnit,
            static_cast<uint16_t>(pModeChange->nSkillIdOrXOrType),
            static_cast<uint16_t>(pModeChange->dwOwnerOrYOrGUID),
            1, 0, 0);
        return TRUE;
    }

    case 19: // jt[8] = 0x6FB0A9FD: Elemental hit overlay & hit sound without interrupting current anim
    {
        const uint32_t dwHitClass = static_cast<uint32_t>(pModeChange->nTargetXOrTypeOrHit);
        pPlayerUnit->dwLastHitClass = dwHitClass;
        if (pPlayerUnit->dwUnitType == UNIT_PLAYER && (dwHitClass & 0xF0) != 0 && (dwHitClass & 0x0F) != 0)
        {
            int32_t nOverlayId = 0;
            switch (dwHitClass & 0xF0)
            {
            case 0x20: nOverlayId = 0x51; break; // Cold
            case 0x30: nOverlayId = 0x24; break; // Fire
            case 0x40: nOverlayId = 0x01; break; // Lightning
            case 0x10: nOverlayId = 0x36; break; // Poison
            case 0x70: nOverlayId = 0x93; break; // Stun
            case 0x80: nOverlayId = 0x15; break; // Bash
            case 0x90: nOverlayId = 0x71; break; // Freeze
            default:   nOverlayId = 0;    break;
            }
            if (nOverlayId != 0)
            {
                D2Client_SetUnitOverlay_6FB430E0(nOverlayId, pPlayerUnit, 2, 0, 0, 0, 0, 0);
            }
        }
        const int32_t nSoundMode = STATES_IsUnitShapeShifted(pPlayerUnit) ? 3 : 19;
        UNITSND_PlayUnitModeSound(pPlayerUnit, nSoundMode, 1);
        D2Client_SyncUnitPosition_6FAF0EB0(
            pPlayerUnit,
            static_cast<uint16_t>(pModeChange->nSkillIdOrXOrType),
            static_cast<uint16_t>(pModeChange->dwOwnerOrYOrGUID),
            1, 0, 0);
        return TRUE;
    }

    case 20: // jt[9] = 0x6FB0AAC4: Knockback (PLRMODE_KNOCKBACK)
        pPlayerUnit->dwLastHitClass = static_cast<uint32_t>(pModeChange->nTargetXOrTypeOrHit);
        PATH_RecacheRoomIfNeeded(pDynamicPath);
        PATH_SetType(pDynamicPath, 11);
        D2COMMON_10190_PATH_SetDistance(pDynamicPath, 5);
        D2COMMON_10170_PathSetTargetPos(
            pDynamicPath,
            pModeChange->nSkillIdOrXOrType,
            static_cast<int32_t>(pModeChange->dwOwnerOrYOrGUID));
        D2Common_10142(pDynamicPath, pPlayerUnit, 0);
        D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, PLRMODE_KNOCKBACK);
        return TRUE;

    case 21: // jt[10] = 0x6FB0A8FF: Start skill at coordinates
        if (!D2Client_ValidatePlayerActionSkillUseState(pPlayerUnit, pModeChange, nullptr)
            || !D2Client_StartSkillAtLocation_6FAFF380(pPlayerUnit, pModeChange))
        {
            D2Client_SetPlayerNeutralModeAndSyncPos(pPlayerUnit, nNeutralMode, TRUE);
            return FALSE;
        }
        return TRUE;

    case 22: // jt[11] = 0x6FB0A8CC: Start skill on target unit
        if (!D2Client_ValidatePlayerActionSkillUseState(pPlayerUnit, pModeChange, nullptr)
            || !D2Client_StartSkillOnTarget_6FB00210(pPlayerUnit, pModeChange))
        {
            D2Client_SetPlayerNeutralModeAndSyncPos(pPlayerUnit, nNeutralMode, TRUE);
            return FALSE;
        }
        return TRUE;

    case 23: // jt[12] = 0x6FB0A963: Run to coordinates (PLRMODE_RUN)
        if (!D2Client_SetUnitPathTargetPosition_6FAF0CF0(
                pPlayerUnit,
                pModeChange->nSkillIdOrXOrType,
                static_cast<int32_t>(pModeChange->dwOwnerOrYOrGUID)))
        {
            D2Client_SetPlayerNeutralModeAndSyncPos(pPlayerUnit, nNeutralMode, TRUE);
            return FALSE;
        }
        D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, PLRMODE_RUN);
        return TRUE;

    case 24: // jt[13] = 0x6FB0A992: Run to target unit (PLRMODE_RUN)
        if (!D2Client_SetUnitPathTargetServerUnit_6FAF0CA0(
                pPlayerUnit,
                pModeChange->nSkillIdOrXOrType,
                pModeChange->dwOwnerOrYOrGUID))
        {
            D2Client_SetPlayerNeutralModeAndSyncPos(pPlayerUnit, nNeutralMode, TRUE);
            return FALSE;
        }
        D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, PLRMODE_RUN);
        return TRUE;

    case 25: // jt[14] = 0x6FB0ACB6: Special1 / Weapon switch (PLRMODE_SPECIAL1)
        D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, PLRMODE_SPECIAL1);
        D2Client_SyncUnitPosition_6FAF0EB0(
            pPlayerUnit,
            static_cast<uint16_t>(pModeChange->nSkillIdOrXOrType),
            static_cast<uint16_t>(pModeChange->dwOwnerOrYOrGUID),
            0, 0, 0);
        return TRUE;

    default: // Cases 3, 4, 5, 10..17, >25 -> jt[15] = 0x6FB0ACE8
        D2Client_DisplayErrorHaltInline(0x432);
        return FALSE;
    }
}

// 1.12a: 0x6FB0AD70 (register ABI: ebx = pPlayerUnit; ret 0x00)
void __fastcall D2Client_CheckPlayerStaminaDepleted(D2UnitStrc* pPlayerUnit)
{
    if (!pPlayerUnit || pPlayerUnit != gpPlayerUnit)
    {
        return;
    }

    if (STATLIST_UnitGetStatValue(pPlayerUnit, STAT_STAMINA, 0) != 0)
    {
        return;
    }

    D2DynamicPathStrc* pSavedPath = pPlayerUnit->pDynamicPath;
    D2Client_ValidatePathTargetUnit(pPlayerUnit->pDynamicPath);
    if (!pPlayerUnit->pDynamicPath)
    {
        D2Client_DisplayErrorHaltInline(0x946);
    }

    D2UnitStrc* pTargetUnit = PATH_GetTargetUnit(pPlayerUnit->pDynamicPath);
    if (PATH_GetTargetUnit(pSavedPath) != nullptr)
    {
        D2PlayerModeChangeStrc tModeChange = {};
        tModeChange.nSkillIdOrXOrType = pTargetUnit ? static_cast<int32_t>(pTargetUnit->dwUnitType) : UNIT_TYPES_COUNT;
        tModeChange.dwOwnerOrYOrGUID  = pTargetUnit ? pTargetUnit->dwUnitId : D2UnitInvalidGUID;
        D2Client_ExecutePlayerModeTransition(0, pPlayerUnit, &tModeChange, FALSE);
        return;
    }

    const int32_t nFirstX = D2COMMON_10175_PathGetFirstPointX(pSavedPath);
    const int32_t nFirstY = D2COMMON_10176_PathGetFirstPointY(pSavedPath);

    void* pSkillLight = D2Common_10958(pPlayerUnit, nullptr);
    if (pSkillLight)
    {
        D2Client_FreeLightMap_6FB0E500(pSkillLight);
    }

    D2Client_TriggerPlayerUsedSkillEnd(pPlayerUnit, (pPlayerUnit == gpPlayerUnit) ? TRUE : FALSE);
    UNITS_SetUsedSkill(pPlayerUnit, nullptr);

    if (pPlayerUnit->dwAnimMode != PLRMODE_KNOCKBACK)
    {
        PATH_ResetToPreviousType(pPlayerUnit->pDynamicPath);
        const BOOL bInTown = DUNGEON_IsRoomInTown(UNITS_GetRoom(pPlayerUnit));
        const int32_t nNeutralMode = bInTown ? PLRMODE_TOWNNEUTRAL : PLRMODE_NEUTRAL;
        const int32_t nWalkMode    = bInTown ? PLRMODE_TOWNWALK    : PLRMODE_WALK;

        if (D2Client_SetUnitPathTargetPosition_6FAF0CF0(pPlayerUnit, nFirstX, nFirstY))
        {
            D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, nWalkMode);
        }
        else
        {
            D2Client_SetPlayerNeutralModeAndSyncPos(pPlayerUnit, nNeutralMode, TRUE);
        }
    }
}

void __stdcall D2Client_CheckLocalPlayerStaminaDepleted()
{
    if (gpPlayerUnit)
    {
        D2Client_CheckPlayerStaminaDepleted(gpPlayerUnit);
    }
}

// 1.12a: 0x6FB0AEE0 (register ABI: ecx = pActionState, eax = dwTargetPlayerId; ret 0x00)
void __fastcall D2Client_ProcessPlayerNextQueuedInputAction(D2PlayerActionStateStrc* pActionState, uint32_t dwTargetPlayerId)
{
    if (D2Common_10399(pActionState->pTargetUnit, pActionState->pPlayerUnit) <= 2
        && !UNITS_TestCollisionBetweenInteractingUnits(pActionState->pTargetUnit, pActionState->pPlayerUnit, 0x0804))
    {
        D2UnitStrc* pPlayer = pActionState->pPlayerUnit;
        if (D2Client_CanUnitInterruptCurrentMode(pPlayer))
        {
            D2PlayerModeChangeStrc tModeChange = {};
            tModeChange.nSkillIdOrXOrType = UNIT_PLAYER;
            tModeChange.dwOwnerOrYOrGUID  = dwTargetPlayerId;
            D2Client_DispatchUnitModeChange_6FAF1450(2, pPlayer, &tModeChange, 0);
            D2Client_ClearPlayerQueuedActionInline(pPlayer);
        }
    }
    else
    {
        D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(
            pActionState->pPlayerUnit,
            UNIT_PLAYER,
            dwTargetPlayerId,
            pActionState->nTargetMoveMode,
            FALSE);
        D2Client_SetPlayerQueuedActionInline(pActionState->pPlayerUnit, 0x13, UNIT_PLAYER, dwTargetPlayerId);
    }
}

// 1.12a: 0x6FB0B000 (register ABI: ecx = nTargetX, eax = nTargetY, stack: nMoveActionCode, pPlayerUnit; ret 0x08)
BOOL __fastcall D2Client_StartPlayerSkillOrInteraction(int32_t nTargetX, int32_t nTargetY, int32_t nMoveActionCode, D2UnitStrc* pPlayerUnit)
{
    D2DynamicPathStrc* pDynamicPath = pPlayerUnit->pDynamicPath;
    D2COMMON_10170_PathSetTargetPos(pDynamicPath, nTargetX, nTargetY);
    if (!D2Common_10142(pDynamicPath, pPlayerUnit, 0))
    {
        return FALSE;
    }

    const int32_t nLastX = D2COMMON_10177_PATH_GetLastPointX(pDynamicPath);
    const int32_t nLastY = D2COMMON_10178_PATH_GetLastPointY(pDynamicPath);
    D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(
        pPlayerUnit,
        nLastX,
        static_cast<uint32_t>(nLastY),
        nMoveActionCode,
        FALSE);
    return TRUE;
}

// Internal helper encapsulating the repeated client skill dispatch + stat 328 bump + packet send in 0x6FB0B050
static void D2Client_DispatchLocalSkillTransitionAndSendPacket(
    D2UnitStrc* pPlayer,
    D2SkillStrc* pSkill,
    const char* szFile,
    int32_t nLine,
    int32_t nTransitionType,
    int32_t nTargetXOrType,
    uint32_t dwTargetYOrGUID,
    uint8_t nPacketOpcode,
    BOOL bIsUnitPacket)
{
    if (!D2Client_CanUnitInterruptCurrentMode(pPlayer))
    {
        return;
    }

    D2PlayerModeChangeStrc tModeChange = {};
    tModeChange.nSkillIdOrXOrType   = SKILLS_GetSkillIdFromSkill(pSkill, szFile, nLine);
    tModeChange.dwOwnerOrYOrGUID    = SKILLS_GetOwnerGUIDFromSkill(pSkill);
    tModeChange.nTargetXOrTypeOrHit = nTargetXOrType;
    tModeChange.dwTargetYOrGUID     = dwTargetYOrGUID;

    if (D2Client_DispatchUnitModeChange_6FAF1450(nTransitionType, pPlayer, &tModeChange, 0))
    {
        const int32_t nCastCount = STATLIST_GetUnitStatBonus(pPlayer, 328, 0) + 1;
        STATLIST_SetUnitStat(pPlayer, 328, nCastCount, 0);

        if (bIsUnitPacket)
        {
            D2C2SPacketSkillOnUnitStrc tPkt;
            tPkt.nHeader    = nPacketOpcode;
            tPkt.dwUnitType = static_cast<uint32_t>(nTargetXOrType);
            tPkt.dwUnitId   = dwTargetYOrGUID;
            MSG_SendClientPacket(sizeof(tPkt), &tPkt);
        }
        else
        {
            D2C2SPacketSkillAtLocStrc tPkt;
            tPkt.nHeader = nPacketOpcode;
            tPkt.wX      = static_cast<uint16_t>(nTargetXOrType);
            tPkt.wY      = static_cast<uint16_t>(dwTargetYOrGUID);
            MSG_SendClientPacket(sizeof(tPkt), &tPkt);
        }
    }

    D2Client_ClearPlayerQueuedActionInline(pPlayer);
}

// 1.12a: 0x6FB0B050 (__stdcall, ret 0x08)
BOOL __stdcall D2Client_ProcessLocalPlayerActionStateMachine(D2PlayerActionStateStrc* pActionState, BOOL bTargeted)
{
    if (gnMouseY > gnScreenHeight - 49)
    {
        return FALSE;
    }

    const uint32_t dwFlags = pActionState->dwFlags;
    D2UnitStrc* pPlayer    = pActionState->pPlayerUnit;

    if (bTargeted == 1)
    {
        const int32_t nTargetType = pActionState->pTargetUnit
            ? static_cast<int32_t>(pActionState->pTargetUnit->dwUnitType)
            : UNIT_TYPES_COUNT;
        const uint32_t dwTargetId = pActionState->pTargetUnit
            ? pActionState->pTargetUnit->dwUnitId
            : D2UnitInvalidGUID;

        if (dwFlags & PLAYERACT_LEFT_HAND)
        {
            if (dwFlags & PLAYERACT_STAND_STILL)
            {
                const uint8_t nOpcode = (dwFlags & PLAYERACT_BUTTON_HELD) ? 0x0A : 0x07;
                D2Client_DispatchLocalSkillTransitionAndSendPacket(
                    pPlayer,
                    UNITS_GetLeftSkill(pPlayer),
                    "..\\Source\\D2Client\\UNIT\\UnitMode.cpp",
                    822,
                    22,
                    nTargetType,
                    dwTargetId,
                    nOpcode,
                    TRUE);
            }
            else if (dwFlags & PLAYERACT_BUTTON_HELD)
            {
                D2Client_DispatchLocalSkillTransitionAndSendPacket(
                    pPlayer,
                    UNITS_GetLeftSkill(pPlayer),
                    "..\\Source\\D2Client\\UNIT\\UnitMode.cpp",
                    822,
                    22,
                    nTargetType,
                    dwTargetId,
                    0x09,
                    TRUE);
            }
            else
            {
                D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(pPlayer, nTargetType, dwTargetId, 6, FALSE);
            }
        }

        if (pActionState->dwFlags & PLAYERACT_RIGHT_HAND)
        {
            uint8_t nOpcode = 0x0D;
            if (pActionState->dwFlags & PLAYERACT_STAND_STILL)
            {
                nOpcode = (pActionState->dwFlags & PLAYERACT_BUTTON_HELD) ? 0x11 : 0x0E;
            }
            else if (pActionState->dwFlags & PLAYERACT_BUTTON_HELD)
            {
                nOpcode = 0x10;
            }

            D2Client_DispatchLocalSkillTransitionAndSendPacket(
                pPlayer,
                UNITS_GetRightSkill(pPlayer),
                "..\\Source\\D2Client\\UNIT\\UnitMode.cpp",
                840,
                22,
                nTargetType,
                dwTargetId,
                nOpcode,
                TRUE);
        }

        return TRUE;
    }

    // Location-targeted cast / attack branch (0x6FB0B761..0x6FB0BA7B)
    if (dwFlags & PLAYERACT_LEFT_HAND)
    {
        if (dwFlags & PLAYERACT_BUTTON_HELD)
        {
            D2Client_DispatchLocalSkillTransitionAndSendPacket(
                pPlayer,
                UNITS_GetLeftSkill(pPlayer),
                "..\\Source\\D2Client\\UNIT\\UnitMode.cpp",
                812,
                21,
                pActionState->nTargetX,
                static_cast<uint32_t>(pActionState->nTargetY),
                0x08,
                FALSE);
        }
        else
        {
            D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(
                pPlayer,
                pActionState->nTargetX,
                static_cast<uint32_t>(pActionState->nTargetY),
                5,
                FALSE);
        }
    }

    if (pActionState->dwFlags & PLAYERACT_RIGHT_HAND)
    {
        const uint8_t nOpcode = (pActionState->dwFlags & PLAYERACT_BUTTON_HELD) ? 0x0F : 0x0C;
        D2Client_DispatchLocalSkillTransitionAndSendPacket(
            pPlayer,
            UNITS_GetRightSkill(pPlayer),
            "..\\Source\\D2Client\\UNIT\\UnitMode.cpp",
            830,
            21,
            pActionState->nTargetX,
            static_cast<uint32_t>(pActionState->nTargetY),
            nOpcode,
            FALSE);
    }

    return TRUE;
}

// 1.12a: 0x6FB0BA90 (register ABI: edi = pActionState; ret 0x00)
void __fastcall D2Client_ExecuteLocalPlayerActionStep(D2PlayerActionStateStrc* pActionState)
{
    D2UnitStrc* pPlayer = pActionState ? pActionState->pPlayerUnit : nullptr;
    if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER)
    {
        return;
    }

    D2Client_ValidatePathTargetUnit(pPlayer->pDynamicPath);
    if (!pPlayer->pDynamicPath)
    {
        D2Client_DisplayErrorHaltInline(0x946);
    }

    if (PATH_GetTargetUnit(pPlayer->pDynamicPath) != nullptr)
    {
        return;
    }

    switch (pPlayer->dwAnimMode)
    {
    case PLRMODE_WALK:
    case PLRMODE_TOWNWALK:
        D2Client_StartPlayerSkillOrInteraction(pActionState->nTargetX, pActionState->nTargetY, 1, pPlayer);
        break;

    case PLRMODE_RUN:
        D2Client_StartPlayerSkillOrInteraction(pActionState->nTargetX, pActionState->nTargetY, 3, pPlayer);
        break;

    default:
        break;
    }
}

static BOOL D2Client_VerifyTargetItemSelectableIfItem(int32_t nTargetUnitType, uint32_t dwTargetUnitId)
{
    if (nTargetUnitType != UNIT_ITEM)
    {
        return TRUE;
    }

    D2UnitStrc* pItem = D2Client_FindServerSideUnit(dwTargetUnitId, UNIT_ITEM);
    if (!pItem || pItem->dwUnitType != UNIT_ITEM)
    {
        return TRUE;
    }

    return ((pItem->dwFlags >> 1) & 1) != 0;
}

// 1.12a: 0x6FB0BB10 (register ABI: eax = nTargetUnitType, stack: pActionState, dwTargetUnitId; ret 0x08)
// Uses 6-entry jump table at 0x6FB0C1A0 (UNIT_PLAYER..UNIT_TILE).
void __fastcall D2Client_HandleLocalPlayerMouseInputState(
    int32_t nTargetUnitType,
    D2PlayerActionStateStrc* pActionState,
    uint32_t dwTargetUnitId)
{
    D2UnitStrc* pTarget = pActionState->pTargetUnit;
    D2UnitStrc* pPlayer = pActionState->pPlayerUnit;
    const uint32_t dwSwitchType = pTarget ? pTarget->dwUnitType : UNIT_TYPES_COUNT;

    switch (dwSwitchType)
    {
    case UNIT_PLAYER: // jt[0] = 0x6FB0BDAA
        if (D2Common_10399(pTarget, pPlayer) <= 4
            && !UNITS_TestCollisionBetweenInteractingUnits(pPlayer, pTarget, 0x0804))
        {
            if (D2Client_CanUnitInterruptCurrentMode(pPlayer)
                && D2Client_VerifyTargetItemSelectableIfItem(nTargetUnitType, dwTargetUnitId))
            {
                D2PlayerModeChangeStrc tModeChange = {};
                tModeChange.nSkillIdOrXOrType = nTargetUnitType;
                tModeChange.dwOwnerOrYOrGUID  = dwTargetUnitId;
                D2Client_DispatchUnitModeChange_6FAF1450(2, pPlayer, &tModeChange, 0);
                D2Client_ClearPlayerQueuedActionInline(pPlayer);
            }
        }
        else
        {
            D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(
                pPlayer,
                nTargetUnitType,
                dwTargetUnitId,
                pActionState->nTargetMoveMode,
                FALSE);
            D2Client_SetPlayerQueuedActionInline(pPlayer, 0x13, static_cast<uint32_t>(nTargetUnitType), dwTargetUnitId);
        }
        return;

    case UNIT_OBJECT: // jt[2] = 0x6FB0BB40
    {
        if (pActionState->dwFlags & PLAYERACT_BUTTON_HELD)
        {
            return;
        }

        DATATBLS_GetObjectsTxtRecord(pTarget ? pTarget->dwClassId : -1);

        BOOL bCanOperate = TRUE;
        if (pTarget && pTarget->dwClassId == 59 && STATES_CheckState(gpPlayerUnit, 102))
        {
            bCanOperate = FALSE;
        }

        if (UNITS_IsObjectInInteractRange(pPlayer, pTarget)
            && !UNITS_TestCollisionBetweenInteractingUnits(pPlayer, pTarget, 0x0804))
        {
            if (!bCanOperate)
            {
                return;
            }
            if (D2Client_CanUnitInterruptCurrentMode(pPlayer)
                && D2Client_VerifyTargetItemSelectableIfItem(nTargetUnitType, dwTargetUnitId))
            {
                D2PlayerModeChangeStrc tModeChange = {};
                tModeChange.nSkillIdOrXOrType = nTargetUnitType;
                tModeChange.dwOwnerOrYOrGUID  = dwTargetUnitId;
                D2Client_DispatchUnitModeChange_6FAF1450(2, pPlayer, &tModeChange, 0);
                D2Client_ClearPlayerQueuedActionInline(pPlayer);
            }
            gbObjectOrItemClickLock_6FBCC048 = 1;
        }
        else
        {
            if (D2Client_IsPlayerAlreadyMovingToTarget(nTargetUnitType, dwTargetUnitId))
            {
                return;
            }

            const int32_t nObjX = D2Client_GetUnitX_6FAB1590(pTarget);
            const int32_t nObjY = D2Client_GetUnitY_6FAB15C0(pTarget);
            D2Client_StartPlayerSkillOrInteraction(nObjX, nObjY, pActionState->nLocationMoveMode, pPlayer);

            if (bCanOperate)
            {
                D2Client_SetPlayerQueuedActionInline(pPlayer, 0x13, static_cast<uint32_t>(nTargetUnitType), dwTargetUnitId);
                gbObjectOrItemClickLock_6FBCC048 = 1;
            }
        }
        return;
    }

    case UNIT_ITEM: // jt[4] = 0x6FB0BC8C
        if (pActionState->dwFlags & PLAYERACT_BUTTON_HELD)
        {
            return;
        }

        if (D2Common_10399(pTarget, pPlayer) <= 4
            && !UNITS_TestCollisionBetweenInteractingUnits(pPlayer, pTarget, 0x0804))
        {
            if (D2Client_CanUnitInterruptCurrentMode(pPlayer)
                && D2Client_VerifyTargetItemSelectableIfItem(nTargetUnitType, dwTargetUnitId))
            {
                D2PlayerModeChangeStrc tModeChange = {};
                tModeChange.nSkillIdOrXOrType = nTargetUnitType;
                tModeChange.dwOwnerOrYOrGUID  = dwTargetUnitId;
                D2Client_DispatchUnitModeChange_6FAF1450(2, pPlayer, &tModeChange, 0);
                D2Client_ClearPlayerQueuedActionInline(pPlayer);
            }
            gbObjectOrItemClickLock_6FBCC048 = 1;
        }
        else
        {
            if (D2Client_IsPlayerAlreadyMovingToTarget(nTargetUnitType, dwTargetUnitId))
            {
                return;
            }
            D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(
                pPlayer,
                nTargetUnitType,
                dwTargetUnitId,
                pActionState->nTargetMoveMode,
                FALSE);
            D2Client_SetPlayerQueuedActionInline(pPlayer, 0x13, static_cast<uint32_t>(nTargetUnitType), dwTargetUnitId);
            gbObjectOrItemClickLock_6FBCC048 = 1;
        }
        return;

    case UNIT_TILE: // jt[5] = 0x6FB0BEC0
        gdwCurTickMs_6FBB32A4 = GetTickCount();
        if (gdwCurTickMs_6FBB32A4 < gdwNextWarpClickTick_6FBB32E0)
        {
            return;
        }

        if (D2Common_10399(pTarget, pPlayer) > 4)
        {
            D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(
                pPlayer,
                nTargetUnitType,
                dwTargetUnitId,
                pActionState->nTargetMoveMode,
                FALSE);
            D2Client_SetPlayerQueuedActionInline(pPlayer, 0x13, static_cast<uint32_t>(nTargetUnitType), dwTargetUnitId);
        }
        else if (D2Client_CanUnitInterruptCurrentMode(pPlayer)
            && D2Client_VerifyTargetItemSelectableIfItem(nTargetUnitType, dwTargetUnitId))
        {
            D2PlayerModeChangeStrc tModeChange = {};
            tModeChange.nSkillIdOrXOrType = nTargetUnitType;
            tModeChange.dwOwnerOrYOrGUID  = dwTargetUnitId;
            D2Client_DispatchUnitModeChange_6FAF1450(2, pPlayer, &tModeChange, 0);
            D2Client_ClearPlayerQueuedActionInline(pPlayer);
        }

        gdwNextWarpClickTick_6FBB32E0 = gdwCurTickMs_6FBB32A4 + 500;
        return;

    case UNIT_MONSTER: // jt[1] = 0x6FB0BFDA
    case UNIT_MISSILE: // jt[3] = 0x6FB0C009
    default:
    {
        int32_t nInteractRange = 5;
        if (dwSwitchType == UNIT_MONSTER)
        {
            D2MonStatsTxt* pMonStatsTxt = ENGINE_GetMonStatsTxtRecord(pTarget ? pTarget->dwClassId : -1);
            if (pMonStatsTxt && (pMonStatsTxt->nMonStatsFlags[1] & static_cast<uint8_t>(gdwBitMasks[1])))
            {
                nInteractRange = 2;
            }
        }

        if (pTarget && pTarget->dwUnitType == UNIT_MONSTER)
        {
            D2MonStatsTxt* pMonStatsTxt = ENGINE_GetMonStatsTxtRecord(pTarget->dwClassId);
            if (pMonStatsTxt
                && (pMonStatsTxt->nMonStatsFlags[1] & static_cast<uint8_t>(gdwBitMasks[1]))
                && (pMonStatsTxt->nMonStatsFlags[1] & static_cast<uint8_t>(gdwBitMasks[0])))
            {
                if (pTarget->pDynamicPath)
                {
                    D2Common_10153(pTarget->pDynamicPath);
                }

                const int32_t nMonX = D2Client_GetUnitX_6FAB1590(pTarget);
                const int32_t nMonY = D2Client_GetUnitY_6FAB15C0(pTarget);
                MSG_SendTownFolkInvite_6FB29770(
                    static_cast<int32_t>(pTarget->dwUnitType),
                    pTarget->dwUnitId,
                    0x59,
                    nMonX,
                    nMonY);

                if (pTarget->pMonsterData)
                {
                    pTarget->pMonsterData->pAiControl = reinterpret_cast<D2AiControlStrc*>(
                        reinterpret_cast<uintptr_t>(pTarget->pMonsterData->pAiControl) | 0x01u);
                }
                D2Client_ChangeUnitModeWithHitOverlayAndSound(pTarget, MONMODE_NEUTRAL);
            }
        }

        if (D2Common_10399(pTarget, pPlayer) > nInteractRange)
        {
            D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(
                pPlayer,
                nTargetUnitType,
                dwTargetUnitId,
                pActionState->nTargetMoveMode,
                FALSE);
            D2Client_SetPlayerQueuedActionInline(pPlayer, 0x13, static_cast<uint32_t>(nTargetUnitType), dwTargetUnitId);
        }
        else if (D2Client_CanUnitInterruptCurrentMode(pPlayer)
            && D2Client_VerifyTargetItemSelectableIfItem(nTargetUnitType, dwTargetUnitId))
        {
            D2PlayerModeChangeStrc tModeChange = {};
            tModeChange.nSkillIdOrXOrType = nTargetUnitType;
            tModeChange.dwOwnerOrYOrGUID  = dwTargetUnitId;
            D2Client_DispatchUnitModeChange_6FAF1450(2, pPlayer, &tModeChange, 0);
            D2Client_ClearPlayerQueuedActionInline(pPlayer);
        }
        return;
    }
    }
}

// 1.12a: 0x6FB0C1C0 (register ABI: esi = pActionState, eax = dwTargetUnitId, stack: nTargetUnitType; ret 0x04)
BOOL __fastcall D2Client_UpdatePlayerActionQueueOnModeEnd(
    D2PlayerActionStateStrc* pActionState,
    uint32_t dwTargetUnitId,
    int32_t nTargetUnitType)
{
    if (!pActionState->pTargetUnit)
    {
        D2Client_DisplayErrorHaltInline(0x52F);
    }
    if (!pActionState->pPlayerUnit)
    {
        D2Client_DisplayErrorHaltInline(0x530);
    }

    if (pActionState->dwFlags & PLAYERACT_STAND_STILL)
    {
        D2Client_ProcessLocalPlayerActionStateMachine(pActionState, TRUE);
        return TRUE;
    }

    int32_t nMaxRange = 0;
    int32_t nCurDistance = 0;
    if (D2Client_IsPlayerSkillWithinRange_6FB095D0(
            pActionState->pSkill,
            pActionState->pTargetUnit,
            &nMaxRange,
            &nCurDistance))
    {
        D2Client_ProcessLocalPlayerActionStateMachine(pActionState, TRUE);
        return TRUE;
    }

    if (pActionState->dwFlags & PLAYERACT_LEFT_HAND)
    {
        D2Client_SetPlayerQueuedActionInline(pActionState->pPlayerUnit, 0x06, static_cast<uint32_t>(nTargetUnitType), dwTargetUnitId);
    }
    if (pActionState->dwFlags & PLAYERACT_RIGHT_HAND)
    {
        D2Client_SetPlayerQueuedActionInline(pActionState->pPlayerUnit, 0x0D, static_cast<uint32_t>(nTargetUnitType), dwTargetUnitId);
    }

    const int32_t nStepDistance = nCurDistance - nMaxRange;
    if (nStepDistance <= 0)
    {
        D2Client_DisplayErrorHaltInline(0x548);
    }
    if (nCurDistance == 0)
    {
        D2Client_DisplayErrorHaltInline(0x549);
    }

    const int32_t nPlayerX = D2Client_GetUnitX_6FAB1590(pActionState->pPlayerUnit);
    const int32_t nPlayerY = D2Client_GetUnitY_6FAB15C0(pActionState->pPlayerUnit);
    const int32_t nTargetX = D2Client_GetUnitX_6FAB1590(pActionState->pTargetUnit);
    const int32_t nTargetY = D2Client_GetUnitY_6FAB15C0(pActionState->pTargetUnit);

    const int32_t nApproachX = nPlayerX + ((nTargetX - nPlayerX) * nStepDistance) / nCurDistance;
    const int32_t nApproachY = nPlayerY + ((nTargetY - nPlayerY) * nStepDistance) / nCurDistance;

    D2Client_StartPlayerSkillOrInteraction(nApproachX, nApproachY, pActionState->nLocationMoveMode, pActionState->pPlayerUnit);
    return TRUE;
}

// 1.12a: 0x6FB0C310 (register ABI: eax = pTargetPlayerUnit; ret 0x00)
void __fastcall D2Client_InteractWithPlayerUnit(D2UnitStrc* pTargetPlayerUnit)
{
    D2PlayerActionStateStrc tAction = {};
    tAction.pPlayerUnit     = gpPlayerUnit;
    tAction.pTargetUnit     = pTargetPlayerUnit;
    tAction.nTargetMoveMode = 2;

    const uint32_t dwTargetId = pTargetPlayerUnit ? pTargetPlayerUnit->dwUnitId : D2UnitInvalidGUID;
    D2Client_ProcessPlayerNextQueuedInputAction(&tAction, dwTargetId);
}

void __stdcall D2Client_CancelLocalPlayerActiveAction()
{
    D2Client_InteractWithPlayerUnit(nullptr);
}

// 1.12a: 0x6FB0C350 (register ABI: eax = pActionState, stack: dwObjectUnitId; ret 0x04)
void __fastcall D2Client_SubmitPlayerInputAction(D2PlayerActionStateStrc* pActionState, uint32_t dwObjectUnitId)
{
    D2UnitStrc* pObject = D2Client_FindClientHashUnitInline(UNIT_OBJECT, dwObjectUnitId);
    if (!pObject)
    {
        return;
    }

    if (pObject->dwUnitType != UNIT_OBJECT)
    {
        D2Client_DisplayErrorHaltInline(0x5A);
    }

    if (!DATATBLS_GetObjectsTxtRecord(pObject->dwClassId))
    {
        D2Client_DisplayErrorHaltInline(0x4C4);
    }

    BOOL bCanOperate = TRUE;
    if (pObject->dwClassId == 59 && STATES_CheckState(gpPlayerUnit, 102))
    {
        bCanOperate = FALSE;
    }

    if (UNITS_IsObjectInInteractRange(pActionState->pPlayerUnit, pActionState->pTargetUnit)
        && !UNITS_TestCollisionBetweenInteractingUnits(pActionState->pPlayerUnit, pActionState->pTargetUnit, 0x0804))
    {
        if (bCanOperate)
        {
            D2UnitStrc* pPlayer = pActionState->pPlayerUnit;
            if (D2Client_CanUnitInterruptCurrentMode(pPlayer))
            {
                D2PlayerModeChangeStrc tModeChange = {};
                tModeChange.nSkillIdOrXOrType = UNIT_OBJECT;
                tModeChange.dwOwnerOrYOrGUID  = dwObjectUnitId;
                D2Client_DispatchUnitModeChange_6FAF1450(2, pPlayer, &tModeChange, 0);
                D2Client_ClearPlayerQueuedActionInline(pPlayer);
            }
        }
    }
    else
    {
        const int32_t nObjX = D2Client_GetUnitSubtileXInline(pActionState->pTargetUnit);
        const int32_t nObjY = D2Client_GetUnitSubtileYInline(pActionState->pTargetUnit);
        D2Client_StartPlayerSkillOrInteraction(nObjX, nObjY, pActionState->nLocationMoveMode, pActionState->pPlayerUnit);

        if (bCanOperate)
        {
            D2Client_SetPlayerQueuedActionInline(pActionState->pPlayerUnit, 0x13, UNIT_OBJECT, dwObjectUnitId);
        }
    }
}

// 1.12a: 0x6FB0C530 (register ABI: esi = pPlayerUnit; ret 0x00)
void __fastcall D2Client_CommandLocalPlayerMoveToCoords(D2UnitStrc* pPlayerUnit)
{
    const uint32_t dwAnimMode = pPlayerUnit ? pPlayerUnit->dwAnimMode : 0;
    if (dwAnimMode != PLRMODE_NEUTRAL && dwAnimMode != PLRMODE_TOWNNEUTRAL)
    {
        return;
    }

    D2CoordStrc tCoord;
    tCoord.nX = D2Client_GetUnitSubtileXInline(pPlayerUnit);
    tCoord.nY = D2Client_GetUnitSubtileYInline(pPlayerUnit);

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pPlayerUnit);
    if (!pRoom)
    {
        return;
    }

    const int32_t nCollisionPattern = PATH_GetUnitCollisionPattern(pPlayerUnit);
    if (COLLISION_CheckMaskWithPattern(pRoom, tCoord.nX, tCoord.nY, nCollisionPattern, 0x0C01) != 0)
    {
        COLLISION_GetFreeCoordinates(pRoom, &tCoord, UNITS_GetUnitSizeX(pPlayerUnit), 0x0C01, TRUE);
        D2Client_TeleportClientUnit_6FAD1550(pPlayerUnit, tCoord.nX, tCoord.nY, FALSE);
    }
}

// 1.12a: 0x6FB0C610 (register ABI: eax = pActionState; ret 0x00)
int32_t __fastcall D2Client_CommandLocalPlayerInteractUnit(D2PlayerActionStateStrc* pActionState)
{
    D2Client_ValidatePlayerInputAction_6FB09650(pActionState);

    D2UnitStrc* pPlayer = pActionState ? pActionState->pPlayerUnit : nullptr;
    D2ClientRosterPetStrc* pPetHead = reinterpret_cast<D2ClientRosterPetStrc*>(gpRosterPetList);
    if (pPlayer && pPetHead)
    {
        const uint32_t dwPlayerId = pPlayer->dwUnitId;
        D2ClientRosterPetStrc* pMercPet = pPetHead;
        while (pMercPet)
        {
            if (pMercPet->nPetType == 7 && pMercPet->dwOwnerId == dwPlayerId && pMercPet->bIsDeadHireling == 0)
            {
                break;
            }
            pMercPet = pMercPet->pNext;
        }

        if (!pMercPet)
        {
            D2Client_ExecuteLocalPlayerActionStep(pActionState);
            return 0;
        }

        const uint32_t dwMercUnitId = pMercPet->dwUnitId;
        if (dwMercUnitId != D2UnitInvalidGUID)
        {
            D2UnitStrc* pMercUnit = D2Client_FindClientHashUnitInline(UNIT_MONSTER, dwMercUnitId);
            if (!pMercUnit)
            {
                D2Client_ExecuteLocalPlayerActionStep(pActionState);
                return 0;
            }
            if (pMercUnit->dwUnitType != UNIT_MONSTER)
            {
                D2Client_DisplayErrorHaltInline(0x5A);
            }

            for (D2ClientRosterPetStrc* pNode = pPetHead; pNode != nullptr; pNode = pNode->pNext)
            {
                if (pNode->dwUnitId == dwMercUnitId)
                {
                    if (pNode->dwSpecialFlag == 2)
                    {
                        D2Client_SetMercPetSpecialFlag_6FB1E6E0(pPlayer, pMercUnit, 0);
                    }
                    break;
                }
            }
        }
    }

    D2Client_ExecuteLocalPlayerActionStep(pActionState);
    return 0;
}

// 1.12a: 0x6FB0C6F0 (register ABI: ecx = pActionState, eax = dwTargetUnitId, stack: nTargetUnitType; ret 0x04)
void __fastcall D2Client_CommandLocalPlayerUseSkillAtCoords(
    D2PlayerActionStateStrc* pActionState,
    uint32_t dwTargetUnitId,
    int32_t nTargetUnitType)
{
    if (!D2Client_CheckPlayerSkillUseStateAndPlayBark(pActionState->pPlayerUnit, &pActionState->pSkill, nullptr))
    {
        return;
    }

    const int32_t nSkillTargetState = D2Common_11016(pActionState->pPlayerUnit, pActionState->pSkill);
    if (nTargetUnitType < 0 || nTargetUnitType >= UNIT_TYPES_COUNT)
    {
        return;
    }

    D2UnitStrc* pHashTarget = D2Client_FindUnitInHashTable_6FACE550(
        &gapServerSideUnitHashTables[nTargetUnitType][0],
        static_cast<int32_t>(dwTargetUnitId & 0x7F),
        nTargetUnitType);
    if (!pHashTarget)
    {
        return;
    }

    const BOOL bTargetMoving = UNITS_IsInMovingModeEx(pHashTarget);
    if (UNITS_IsInMeleeRange(pActionState->pPlayerUnit, pActionState->pTargetUnit, bTargetMoving))
    {
        D2Client_ProcessLocalPlayerActionStateMachine(pActionState, TRUE);
        return;
    }

    if (pActionState->pSkill)
    {
        D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pActionState->pSkill);
        if (pSkillsTxt && pSkillsTxt->wSrvDoFunc == 19)
        {
            D2Client_UpdatePlayerActionQueueOnModeEnd(pActionState, dwTargetUnitId, nTargetUnitType);
            return;
        }
    }

    if (nSkillTargetState != 1 && nSkillTargetState != 4)
    {
        D2Client_ProcessLocalPlayerActionStateMachine(pActionState, TRUE);
        return;
    }

    if (pActionState->dwFlags & PLAYERACT_STAND_STILL)
    {
        D2Client_ProcessLocalPlayerActionStateMachine(pActionState, FALSE);
        return;
    }

    D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(
        pActionState->pPlayerUnit,
        nTargetUnitType,
        dwTargetUnitId,
        pActionState->nTargetMoveMode,
        FALSE);

    if (pActionState->dwFlags & PLAYERACT_LEFT_HAND)
    {
        D2Client_SetPlayerQueuedActionInline(pActionState->pPlayerUnit, 0x06, static_cast<uint32_t>(nTargetUnitType), dwTargetUnitId);
    }
    if (pActionState->dwFlags & PLAYERACT_RIGHT_HAND)
    {
        D2Client_SetPlayerQueuedActionInline(pActionState->pPlayerUnit, 0x0D, static_cast<uint32_t>(nTargetUnitType), dwTargetUnitId);
    }
}

// 1.12a: 0x6FB0C7E0 (register ABI: eax = pActionState; ret 0x00)
void __fastcall D2Client_CommandLocalPlayerUseSkillOnUnit(D2PlayerActionStateStrc* pActionState)
{
    if (!D2Client_PreparePlayerSkillInputAction(pActionState))
    {
        return;
    }

    D2UnitStrc* pPlayer    = pActionState->pPlayerUnit;
    D2UnitStrc* pTarget    = pActionState->pTargetUnit;
    uint32_t    dwFlags    = pActionState->dwFlags;
    const int32_t nSkillTargetState = D2Common_11016(pPlayer, pActionState->pSkill);

    D2SkillsTxt* pSkillsTxt = pActionState->pSkill
        ? SKILLS_GetSkillsTxtRecordFromSkill(pActionState->pSkill)
        : nullptr;

    BOOL bUseLocationBranch = FALSE;
    BOOL bAllowObjectTarget = FALSE;

    if (!pSkillsTxt || !pTarget)
    {
        bUseLocationBranch = TRUE;
    }
    else
    {
        if ((dwFlags & PLAYERACT_RIGHT_HAND) && nSkillTargetState != 1)
        {
            if (pTarget->dwUnitType == UNIT_OBJECT || pTarget->dwUnitType == UNIT_ITEM)
            {
                const uint8_t nHighByte = static_cast<uint8_t>(pSkillsTxt->dwFlags[0] >> 24);
                if (nHighByte & static_cast<uint8_t>(gdwBitMasks[3]))
                {
                    bAllowObjectTarget = TRUE;
                }
                else
                {
                    bUseLocationBranch = TRUE;
                }
            }
            else if (pTarget->dwUnitType == UNIT_TILE)
            {
                dwFlags |= PLAYERACT_FORCE_APPROACH;
                bUseLocationBranch = TRUE;
            }
        }

        if (!bUseLocationBranch
            && pTarget->dwUnitType == UNIT_MONSTER
            && D2Client_IsUnitDead_6FACEAC0(pTarget))
        {
            const uint8_t nHighByte = static_cast<uint8_t>(pSkillsTxt->dwFlags[0] >> 24);
            if ((nHighByte & static_cast<uint8_t>(gdwBitMasks[0])) == 0)
            {
                bUseLocationBranch = TRUE;
            }
        }
    }

    if (bUseLocationBranch)
    {
        const BOOL bStandStill = (dwFlags & PLAYERACT_STAND_STILL) != 0;
        if (!bStandStill && (!(dwFlags & PLAYERACT_RIGHT_HAND) || nSkillTargetState == 1))
        {
            if (D2Client_ComputePlayerStepMoveTarget_6FB09710(pPlayer, pActionState))
            {
                D2Client_StartPlayerSkillOrInteraction(
                    pActionState->nTargetX,
                    pActionState->nTargetY,
                    pActionState->nLocationMoveMode,
                    pPlayer);
                gnSelectedTargetB = 1;
                gnSelectedTargetA = 0;
            }
            return;
        }

        int32_t nUseState = 0;
        if (!D2Client_CheckPlayerSkillUseStateAndPlayBark(pPlayer, &pActionState->pSkill, &nUseState))
        {
            return;
        }

        if (pActionState->pSkill
            && SKILLS_GetSkillIdFromSkill(pActionState->pSkill, "..\\Source\\D2Client\\UNIT\\Player.cpp", 1967) == 0
            && nUseState == 1
            && !bStandStill)
        {
            if (D2Client_ComputePlayerStepMoveTarget_6FB09710(pPlayer, pActionState))
            {
                D2Client_StartPlayerSkillOrInteraction(
                    pActionState->nTargetX,
                    pActionState->nTargetY,
                    pActionState->nLocationMoveMode,
                    pPlayer);
                gnSelectedTargetB = 1;
                gnSelectedTargetA = 0;
            }
            return;
        }

        D2Client_ProcessLocalPlayerActionStateMachine(pActionState, FALSE);
        gnSelectedTargetB = 1;
        gnSelectedTargetA = 0;
        return;
    }

    // Targeted unit branch (0x6FB0C99F..0x6FB0CB9E)
    const int32_t  nTargetType = static_cast<int32_t>(pTarget->dwUnitType);
    const uint32_t dwTargetId  = pTarget->dwUnitId;

    if (!bAllowObjectTarget
        && (nTargetType == UNIT_OBJECT || nTargetType == UNIT_ITEM || nTargetType == UNIT_TILE)
        && (dwFlags & PLAYERACT_RIGHT_HAND)
        && nSkillTargetState != 1)
    {
        return;
    }

    if (SKILLS_GetSkillModeFromUnit(pPlayer, pActionState->pSkill) != 0)
    {
        D2SkillsTxt* pCurSkillsTxt = pActionState->pSkill
            ? SKILLS_GetSkillsTxtRecordFromSkill(pActionState->pSkill)
            : nullptr;

        if (pCurSkillsTxt)
        {
            const uint8_t nHighByte = static_cast<uint8_t>(pCurSkillsTxt->dwFlags[0] >> 24);
            if (nHighByte & static_cast<uint8_t>(gdwBitMasks[3]))
            {
                if (D2Client_CheckPlayerSkillUseStateAndPlayBark(pPlayer, &pActionState->pSkill, nullptr))
                {
                    D2Client_ProcessLocalPlayerActionStateMachine(pActionState, TRUE);
                }
                return;
            }
        }

        if ((pTarget->dwFlags >> 2) & 1) // UNITFLAG_CANBEATTACKED
        {
            const uint32_t dwHighFlags = pSkillsTxt ? static_cast<uint32_t>(pSkillsTxt->dwFlags[0] >> 24) : 0;
            BOOL bValidCombatTarget = FALSE;
            if ((dwHighFlags & gdwBitMasks[1]) || (dwHighFlags & gdwBitMasks[2]))
            {
                bValidCombatTarget = TRUE;
            }
            else
            {
                bValidCombatTarget = D2Client_AreUnitsHostile_6FACF980(pTarget, pPlayer);
            }

            if (bValidCombatTarget)
            {
                if (nTargetType == UNIT_OBJECT)
                {
                    if (!(dwFlags & PLAYERACT_STAND_STILL))
                    {
                        D2Client_SubmitPlayerInputAction(pActionState, dwTargetId);
                    }
                    return;
                }

                const BOOL bTargetInTown = DUNGEON_IsRoomInTown(UNITS_GetRoom(pTarget));
                if (nTargetType == UNIT_PLAYER)
                {
                    if (bTargetInTown && DUNGEON_IsRoomInTown(UNITS_GetRoom(pPlayer)))
                    {
                        D2Client_ProcessPlayerNextQueuedInputAction(pActionState, dwTargetId);
                    }
                    else
                    {
                        D2Client_CommandLocalPlayerUseSkillAtCoords(pActionState, dwTargetId, UNIT_PLAYER);
                    }
                    return;
                }

                if (bTargetInTown && !D2Client_CanUseSkillInTown_6FB096C0(pActionState->pSkill))
                {
                    return;
                }

                D2Client_CommandLocalPlayerUseSkillAtCoords(pActionState, dwTargetId, nTargetType);
                return;
            }
        }

        if (dwFlags & PLAYERACT_STAND_STILL)
        {
            if (D2Client_CheckPlayerSkillUseStateAndPlayBark(pPlayer, &pActionState->pSkill, nullptr))
            {
                D2Client_ProcessLocalPlayerActionStateMachine(pActionState, FALSE);
            }
            return;
        }
    }
    else if (dwFlags & PLAYERACT_STAND_STILL)
    {
        return;
    }

    if (nTargetType != UNIT_MONSTER)
    {
        D2Client_HandleLocalPlayerMouseInputState(nTargetType, pActionState, dwTargetId);
        return;
    }

    if (dwFlags & PLAYERACT_BUTTON_DOWN)
    {
        D2MonStatsTxt* pMonStatsTxt = ENGINE_GetMonStatsTxtRecord(pTarget->dwClassId);
        if (pMonStatsTxt && (pMonStatsTxt->nMonStatsFlags[1] & static_cast<uint8_t>(gdwBitMasks[1])))
        {
            D2Client_HandleLocalPlayerMouseInputState(UNIT_MONSTER, pActionState, dwTargetId);
        }
        else
        {
            D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(
                pPlayer,
                UNIT_MONSTER,
                dwTargetId,
                pActionState->nTargetMoveMode,
                FALSE);
        }
    }
}

// 1.12a: 0x6FB0CBB0 (register ABI: esi = pActionState; ret 0x00)
BOOL __fastcall D2Client_UpdateLocalPlayerHeldMouseInput(D2PlayerActionStateStrc* pActionState)
{
    if (D2Client_ValidatePlayerInputAction_6FB09650(pActionState))
    {
        pActionState->pSkill = UNITS_GetRightSkill(pActionState->pPlayerUnit);
        if (D2Client_CanPlayerStartSkillWithMode_6FACEB20(pActionState->pPlayerUnit, pActionState->pSkill))
        {
            const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(
                pActionState->pSkill,
                "..\\Source\\D2Client\\UNIT\\Player.cpp",
                2557);
            D2SkillsTxt* pSkillsTxt = D2Client_GetSkillsTxtRecordInline(nSkillId);
            if (pSkillsTxt && pSkillsTxt->nRange == 1 && !(pActionState->dwFlags & PLAYERACT_FORCE_RUN))
            {
                pActionState->dwFlags |= PLAYERACT_MOVE_ONLY;
            }
            D2Client_CommandLocalPlayerUseSkillOnUnit(pActionState);
        }
    }
    return TRUE;
}

// 1.12a: 0x6FB0CC30 (register ABI: esi = pActionState; ret 0x00)
BOOL __fastcall D2Client_SetPlayerInputTargetCoords(D2PlayerActionStateStrc* pActionState)
{
    if (D2Client_ValidatePlayerInputAction_6FB09650(pActionState))
    {
        pActionState->pSkill = UNITS_GetRightSkill(pActionState->pPlayerUnit);
        if (D2Client_CanPlayerStartSkillWithMode_6FACEB20(pActionState->pPlayerUnit, pActionState->pSkill))
        {
            const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(
                pActionState->pSkill,
                "..\\Source\\D2Client\\UNIT\\Player.cpp",
                2525);
            D2SkillsTxt* pSkillsTxt = D2Client_GetSkillsTxtRecordInline(nSkillId);
            if (pSkillsTxt && pSkillsTxt->nRange == 1 && !(pActionState->dwFlags & PLAYERACT_FORCE_RUN))
            {
                pActionState->dwFlags |= PLAYERACT_MOVE_ONLY;
            }
            D2Client_CommandLocalPlayerUseSkillOnUnit(pActionState);
        }
    }
    return TRUE;
}

// 1.12a: 0x6FB0CCB0 (register ABI: edi = pActionState; ret 0x00)
BOOL __fastcall D2Client_SetPlayerInputTargetUnit(D2PlayerActionStateStrc* pActionState)
{
    if (D2Client_ValidatePlayerInputAction_6FB09650(pActionState))
    {
        D2UnitStrc* pPlayer = pActionState->pPlayerUnit;
        pActionState->pSkill = UNITS_GetLeftSkill(pPlayer);
        if (D2Client_CanPlayerStartSkillWithMode_6FACEB20(pPlayer, pActionState->pSkill))
        {
            int32_t nWalkSpeed = 0;
            int32_t nRunSpeed  = 0;
            const int32_t nCharId = pPlayer ? pPlayer->dwClassId : -1;
            UNITS_GetRunAndWalkSpeedForPlayer(
                static_cast<int>(reinterpret_cast<intptr_t>(pPlayer)),
                nCharId,
                &nWalkSpeed,
                &nRunSpeed);

            const int32_t nVelPct = STATLIST_UnitGetStatValue(pPlayer, STAT_VELOCITYPERCENT, 0);
            const int32_t nExpectedWalkVel = ((nWalkSpeed << 8) * nVelPct) / 100;
            const int32_t nCurrentPathVel  = PATH_GetVelocity(pPlayer->pDynamicPath);

            BOOL bSpeedMismatch = FALSE;
            if (nCurrentPathVel == nExpectedWalkVel)
            {
                if (pActionState->dwFlags & PLAYERACT_FORCE_RUN)
                {
                    bSpeedMismatch = TRUE;
                }
            }
            else if (!(pActionState->dwFlags & PLAYERACT_FORCE_RUN))
            {
                bSpeedMismatch = TRUE;
            }

            if (pActionState->pTargetUnit)
            {
                D2UnitStrc* pCurPathTarget = D2Client_GetValidatedPathTargetUnit_6FACF5B0(pPlayer);
                if (pActionState->pTargetUnit != pCurPathTarget || bSpeedMismatch)
                {
                    PATH_SetTargetUnit(pPlayer->pDynamicPath, pActionState->pTargetUnit);
                    pActionState->dwFlags |= PLAYERACT_MOVE_ONLY;
                }
                D2Client_CommandLocalPlayerUseSkillOnUnit(pActionState);
                return TRUE;
            }

            if (!(pActionState->dwFlags & PLAYERACT_STAND_STILL))
            {
                if (D2Client_GetPlayerQueuedActionCodeInline(pPlayer) != 0)
                {
                    return TRUE;
                }
                pActionState->dwFlags |= PLAYERACT_MOVE_ONLY;
            }

            D2Client_CommandLocalPlayerUseSkillOnUnit(pActionState);
        }
    }
    return TRUE;
}

// 1.12a: 0x6FB0CDD0 (register ABI: esi = pActionState; ret 0x00)
BOOL __fastcall D2Client_OnMouseButtonReleased(D2PlayerActionStateStrc* pActionState)
{
    if (!D2Client_ValidatePlayerInputAction_6FB09650(pActionState))
    {
        return TRUE;
    }

    D2UnitStrc* pPlayer = pActionState->pPlayerUnit;
    pActionState->pSkill = UNITS_GetLeftSkill(pPlayer);
    if (D2Client_CanPlayerStartSkillWithMode_6FACEB20(pPlayer, pActionState->pSkill))
    {
        pActionState->dwFlags |= PLAYERACT_MOVE_ONLY;
        D2Client_CommandLocalPlayerUseSkillOnUnit(pActionState);
        return TRUE;
    }

    if (gnCursorState == 6)
    {
        UI_CancelActiveCursorState();
    }
    else if (gnCursorState == 8)
    {
        D2C2SPacketByteDwordStrc tCancelPkt;
        tCancelPkt.nHeader = 0x4C;
        tCancelPkt.dwArg   = D2UnitInvalidGUID;
        MSG_SendClientPacket(sizeof(tCancelPkt), &tCancelPkt);
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pPlayer->pInventory);
    if (!pCursorItem)
    {
        return TRUE;
    }

    D2C2SPacketByteDwordStrc tDropPkt;
    tDropPkt.nHeader = 0x17; // C2S_DROPITEM
    tDropPkt.dwArg   = pCursorItem->dwUnitId;
    MSG_SendClientPacket(sizeof(tDropPkt), &tDropPkt);
    return FALSE;
}

// 1.12a: 0x6FB0CE80 (__stdcall, ret 0x10)
// Uses 6-entry jump table at 0x6FB0D090 (mouse click types 0..5).
int32_t __stdcall D2Client_DispatchMouseClickToPlayerAction(
    int32_t nClickType,
    int32_t nScreenX,
    int32_t nScreenY,
    int32_t nKeyFlags)
{
    if (gaPanelStates[9] != 0) // D2PANEL_ESCMENU
    {
        return 0;
    }

    if (gbPlayerMouseActionLock_6FBCC1D8 != 0 && (nScreenX != 0 || nScreenY != 0))
    {
        return 0;
    }

    gbPlayerMouseActionLock_6FBCC1D8 = 1;

    int32_t nWorldX = nScreenX;
    int32_t nWorldY = nScreenY;
    D2Client_ScreenToSubtileCoords(&nWorldX, &nWorldY);

    D2PlayerActionStateStrc tAction = {};
    tAction.pPlayerUnit = gpPlayerUnit;
    tAction.pTargetUnit = SELECT_GetSelectedUnit();
    tAction.nTargetX    = nWorldX;
    tAction.nTargetY    = nWorldY;

    if (tAction.pTargetUnit
        && (tAction.pTargetUnit->dwUnitType == UNIT_OBJECT || tAction.pTargetUnit->dwUnitType == UNIT_ITEM))
    {
        tAction.nTargetX = D2Client_GetUnitX_6FAB1590(tAction.pTargetUnit);
        tAction.nTargetY = D2Client_GetUnitY_6FAB15C0(tAction.pTargetUnit);
    }

    uint32_t dwActionFlags = 0;
    if ((nKeyFlags & 0x08) && STATLIST_UnitGetStatValue(tAction.pPlayerUnit, STAT_STAMINA, 0) != 0)
    {
        dwActionFlags = PLAYERACT_FORCE_RUN;
    }
    if (nKeyFlags & 0x04)
    {
        dwActionFlags |= PLAYERACT_STAND_STILL;
    }

    switch (nClickType)
    {
    case 0: // jt[0] = 0x6FB0CF6C: Left mouse button down
        if (tAction.pTargetUnit)
        {
            SELECT_SetSelectedGroundItem(tAction.pTargetUnit);
        }
        else
        {
            gnSelectedTargetB = 0;
            gnSelectedTargetA = 0;
        }
        tAction.dwFlags = dwActionFlags | PLAYERACT_LEFT_HAND | PLAYERACT_BUTTON_DOWN;
        return D2Client_OnMouseButtonReleased(&tAction);

    case 1: // jt[1] = 0x6FB0CFB5: Left mouse button held
        tAction.dwFlags = dwActionFlags | PLAYERACT_LEFT_HAND | PLAYERACT_BUTTON_HELD;
        return D2Client_SetPlayerInputTargetUnit(&tAction);

    case 2: // jt[2] = 0x6FB0CFCF: Left mouse button up
        gnSelectedTargetB = 0;
        gnSelectedTargetA = 0;
        tAction.dwFlags = dwActionFlags | PLAYERACT_LEFT_HAND | PLAYERACT_BUTTON_UP;
        return D2Client_CommandLocalPlayerInteractUnit(&tAction);

    case 3: // jt[3] = 0x6FB0CFF5: Right mouse button down
        if (tAction.pTargetUnit)
        {
            SELECT_SetSelectedGroundItem(tAction.pTargetUnit);
        }
        else
        {
            gnSelectedTargetB = 0;
            gnSelectedTargetA = 0;
        }
        tAction.dwFlags = dwActionFlags | PLAYERACT_RIGHT_HAND | PLAYERACT_BUTTON_DOWN;
        return D2Client_SetPlayerInputTargetCoords(&tAction);

    case 4: // jt[4] = 0x6FB0D03E: Right mouse button held
        tAction.dwFlags = dwActionFlags | PLAYERACT_RIGHT_HAND | PLAYERACT_BUTTON_HELD;
        return D2Client_UpdateLocalPlayerHeldMouseInput(&tAction);

    case 5: // jt[5] = 0x6FB0D058: Right mouse button up
        gnSelectedTargetB = 0;
        gnSelectedTargetA = 0;
        tAction.dwFlags = dwActionFlags | PLAYERACT_RIGHT_HAND | PLAYERACT_BUTTON_UP;
        return D2Client_HandleMercenaryOrPetRightClick(&tAction);

    default:
        return 1;
    }
}

// 1.12a: 0x6FB0D0B0 (register ABI: edi = pPlayerUnit; ret 0x00)
void __fastcall D2Client_OnLocalPlayerRoomChanged(D2UnitStrc* pPlayerUnit)
{
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pPlayerUnit);
    if (!pRoom)
    {
        return;
    }

    const int32_t nLevelId     = DUNGEON_GetLevelIdFromRoom(pRoom);
    const int32_t nPortalFlags = UNITS_GetPlayerPortalFlags(pPlayerUnit) | DUNGEON_GetPortalFlagFromLevelId(nLevelId);
    UNITS_SetPlayerPortalFlags(pPlayerUnit, nPortalFlags);

    D2ActiveRoomStrc* pCurrentRoom = UNITS_GetRoom(pPlayerUnit);
    D2ActiveRoomStrc* pNextRoom    = PATH_GetNextRoom(pPlayerUnit->pDynamicPath);
    DUNGEON_InitRoomTileAnimation(gpClientAct, pNextRoom, pCurrentRoom);

    const int32_t bNowInTown = DUNGEON_IsRoomInTown(pRoom) ? 1 : 0;
    if (gbLocalPlayerInTown_6FBC21D4 != bNowInTown && bNowInTown == 0)
    {
        D2Client_OnPlayerLeaveTown_6FACC3F0(pPlayerUnit);
    }

    gbLocalPlayerInTown_6FBC21D4 = bNowInTown;
    gbLOSDrawEnabled_6FBCC3F4    = DUNGEON_CheckLOSDraw(pRoom);
    D2Client_NotifyAutomapRoomChanged_6FAC35A0(pRoom);
}

// 1.12a: 0x6FB0D140 (__stdcall in binary, ret 0x04; exposed as __fastcall for UNIT_Player.cpp callers)
// Per-tick update handler for UNIT_PLAYER.
// Uses g_aPlayerModeUpdateTable_6FB946D8 and 4-case jump table at 0x6FB0D52C.
void __fastcall D2Client_UpdatePlayerUnitPerTick(D2UnitStrc* pPlayerUnit)
{
    if (!pPlayerUnit)
    {
        return;
    }

    D2DynamicPathStrc* pSavedPath = pPlayerUnit->pDynamicPath;
    const uint32_t dwInitialMode  = pPlayerUnit->dwAnimMode;
    BOOL bPathStepEnded           = FALSE;

    // 1. Dynamic path stepping (Walk / Run / TownWalk / Knockback)
    if (dwInitialMode < NUMBER_OF_PLRMODES
        && g_aPlayerModeUpdateTable_6FB946D8[dwInitialMode].nStepType == 1)
    {
        D2Client_ValidatePathTargetUnit(pPlayerUnit->pDynamicPath);

        if (STATES_CheckState(pPlayerUnit, 13)) // STATE_BLAZE
        {
            D2Client_UpdateBlazeTrail_6FB024E0(pPlayerUnit);
        }
        if (STATES_CheckState(pPlayerUnit, 22))
        {
            D2Client_UpdateState22Effect_6FB4D480(pPlayerUnit);
        }

        bPathStepEnded = (D2Common_10226(pPlayerUnit, gnStepMoveFlag_6FBB32DC) == 0) ? TRUE : FALSE;

        if (pPlayerUnit == gpPlayerUnit)
        {
            D2Client_UpdateUnitDungeonPops(pPlayerUnit);
        }

        D2Client_PlayPlayerFootstepSound_6FAE8590(pPlayerUnit);
    }

    // 2. Active skill movement & frame events 1 / 2 / 3 -> Client Skill Do/Start dispatch
    const uint32_t dwSkillCheckMode = pPlayerUnit->dwAnimMode;
    if (dwSkillCheckMode < NUMBER_OF_PLRMODES
        && g_aPlayerModeUpdateTable_6FB946D8[dwSkillCheckMode].nStepType == 2)
    {
        D2SkillStrc* pUsedSkill = UNITS_GetUsedSkill(pPlayerUnit);
        if (pUsedSkill)
        {
            const int32_t nSkillFlags = SKILLS_GetFlags(pUsedSkill);
            BOOL bCheckActionFrame = TRUE;

            if (nSkillFlags & 1)
            {
                if (D2Client_ProcessDynamicUnitMovement(pPlayerUnit))
                {
                    SKILLS_SetFlags(pUsedSkill, nSkillFlags | 2);
                    D2Client_InvokeClientSkillDoFunc_6FB00150(pPlayerUnit);
                    bCheckActionFrame = FALSE;
                }

                if (pPlayerUnit == gpPlayerUnit)
                {
                    D2Client_UpdateUnitDungeonPops(pPlayerUnit);
                }
            }

            if (bCheckActionFrame && ((pPlayerUnit->dwFlags >> 6) & 1) == 0)
            {
                const uint8_t nEvent = pPlayerUnit->nActionFrame;
                if (nEvent == 1 || nEvent == 2 || nEvent == 3)
                {
                    D2Client_InvokeClientSkillDoFunc_6FB00150(pPlayerUnit);
                }
            }
        }
    }

    // 3. Sequence / end-of-cycle frame clamping (4-entry jump table at 0x6FB0D52C)
    const uint32_t dwClampMode = pPlayerUnit->dwAnimMode;
    if (dwClampMode < NUMBER_OF_PLRMODES)
    {
        switch (g_aPlayerModeUpdateTable_6FB946D8[dwClampMode].nFrameEndType)
        {
        case 0: // jt[0] = 0x6FB0D270
            UNITS_StopSequence(pPlayerUnit);
            break;

        case 1: // jt[1] = 0x6FB0D266
            if (!UNITS_IsAtEndOfFrameCycle(pPlayerUnit))
            {
                UNITS_StopSequence(pPlayerUnit);
            }
            break;

        case 2: // jt[2] = 0x6FB0D278
            if (!UNITS_IsAtEndOfFrameCycle(pPlayerUnit))
            {
                UNITS_StopSequence(pPlayerUnit);
            }
            else
            {
                pPlayerUnit->nSeqCurrentFramePrecise = pPlayerUnit->dwFrameCountPrecise - 0x100;
                pPlayerUnit->wAnimSpeed = 0;
            }
            break;

        case 3: // jt[3] = 0x6FB0D28F
            pPlayerUnit->nSeqCurrentFramePrecise = 0x100;
            pPlayerUnit->wAnimSpeed = 0;
            break;

        default:
            break;
        }
    }

    // 4. Skill sequence composite gfx refresh
    if (pPlayerUnit->dwAnimMode == PLRMODE_SEQUENCE && ((pPlayerUnit->dwFlags >> 14) & 1) != 0)
    {
        D2Client_UpdateUnitAnimGraphics_6FB42000(pPlayerUnit, 0);
    }

    // 5. Cycle / path completion transitions
    const uint32_t dwEndCheckMode = pPlayerUnit->dwAnimMode;
    BOOL bCycleFinished = FALSE;
    if (dwEndCheckMode < NUMBER_OF_PLRMODES)
    {
        const int32_t nEndAction = g_aPlayerModeUpdateTable_6FB946D8[dwEndCheckMode].nCycleEndAction;
        if (nEndAction == 1)
        {
            bCycleFinished = bPathStepEnded;
        }
        else if (nEndAction == 2 || nEndAction == 3)
        {
            bCycleFinished = UNITS_IsAtEndOfFrameCycle(pPlayerUnit);
        }
    }

    if (bCycleFinished)
    {
        if (pPlayerUnit->dwAnimMode == PLRMODE_KNOCKBACK)
        {
            const int32_t nSyncX = D2Client_GetUnitSubtileXInline(pPlayerUnit);
            const int32_t nSyncY = D2Client_GetUnitSubtileYInline(pPlayerUnit);

            PATH_RecacheRoomIfNeeded(pSavedPath);
            PATH_ResetToPreviousType(pSavedPath);

            void* pSkillLight = D2Common_10958(pPlayerUnit, nullptr);
            if (pSkillLight)
            {
                D2Client_FreeLightMap_6FB0E500(pSkillLight);
            }

            D2Client_TriggerPlayerUsedSkillEnd(pPlayerUnit, (pPlayerUnit == gpPlayerUnit) ? TRUE : FALSE);
            UNITS_SetUsedSkill(pPlayerUnit, nullptr);

            if (pPlayerUnit->dwAnimMode == PLRMODE_KNOCKBACK)
            {
                PATH_RecacheRoomIfNeeded(pPlayerUnit->pDynamicPath);
            }
            PATH_ResetToPreviousType(pPlayerUnit->pDynamicPath);
            DUNGEON_IsRoomInTown(UNITS_GetRoom(pPlayerUnit));

            pPlayerUnit->dwLastHitClass = 0;
            D2Client_ChangeUnitModeWithHitOverlayAndSound(pPlayerUnit, PLRMODE_GETHIT);
            D2Client_SyncUnitPosition_6FAF0EB0(pPlayerUnit, nSyncX, nSyncY, 1, 0, 0);
        }
        else if (pPlayerUnit->dwUnitType == UNIT_PLAYER
            && pPlayerUnit->pPlayerData
            && pPlayerUnit->pPlayerData->unk0xA8[43] != 0)
        {
            pPlayerUnit->pPlayerData->unk0xA8[42] = 1;
        }
        else
        {
            void* pSkillLight = D2Common_10958(pPlayerUnit, nullptr);
            if (pSkillLight)
            {
                D2Client_FreeLightMap_6FB0E500(pSkillLight);
            }

            D2Client_TriggerPlayerUsedSkillEnd(pPlayerUnit, (pPlayerUnit == gpPlayerUnit) ? TRUE : FALSE);
            UNITS_SetUsedSkill(pPlayerUnit, nullptr);

            if (pPlayerUnit->dwAnimMode == PLRMODE_KNOCKBACK)
            {
                PATH_RecacheRoomIfNeeded(pPlayerUnit->pDynamicPath);
            }
            PATH_ResetToPreviousType(pPlayerUnit->pDynamicPath);

            const BOOL bInTown = DUNGEON_IsRoomInTown(UNITS_GetRoom(pPlayerUnit));
            const int32_t nNeutralMode = bInTown ? PLRMODE_TOWNNEUTRAL : PLRMODE_NEUTRAL;

            UNIT_FreeUnitGfxData(pPlayerUnit);
            pPlayerUnit->dwFlags = (pPlayerUnit->dwFlags & ~UNITFLAG_SKSRVDOFUNC) | UNITFLAG_TARGETABLE;
            if (UNITS_ChangeAnimMode(pPlayerUnit, nNeutralMode))
            {
                D2Client_UpdateUnitAnimGraphics_6FB42000(pPlayerUnit, 0);
                UNITMODE_OnModeChangeComplete(nNeutralMode, pPlayerUnit);
                UNITSND_PlayUnitModeSound(pPlayerUnit, nNeutralMode, 0);
            }
        }
    }

    // 6. Room crossing, path direction smoothing, stamina check, active state callbacks, and wall unstuck
    if (UNITS_IsCurrentRoomInvalid(pPlayerUnit))
    {
        UNITS_SetCurrentRoomInvalid(pPlayerUnit, 0);
        if (pPlayerUnit == gpPlayerUnit)
        {
            D2Client_OnLocalPlayerRoomChanged(pPlayerUnit);
        }
    }

    D2COMMON_10193_PATH_AdjustDirection(pSavedPath);

    if (pPlayerUnit->dwAnimMode == PLRMODE_RUN)
    {
        D2Client_CheckPlayerStaminaDepleted(pPlayerUnit);
    }

    if (STATES_CheckStateMaskActiveOnUnit(pPlayerUnit))
    {
        D2Client_ProcessActiveUnitStates_6FAFD9F0(pPlayerUnit);
    }

    D2Client_CommandLocalPlayerMoveToCoords(pPlayerUnit);
}

// ============================================================================
// PART 5: Viewport, Camera & Debug Room/Tile Overlays (0x6FB0D540 .. 0x6FB0DFEF)
// ============================================================================

// 1.12a: 0x6FB0D540 (register ABI: eax = nClientX, stack: nClientY, nPaletteColor; ret 0x08)
void __fastcall D2Client_DrawClientDiamondOverlay(int32_t nClientX, int32_t nClientY, uint8_t nPaletteColor)
{
    const int32_t nScreenX = nClientX - gnViewportOffsetX_6FBC21D0;
    const int32_t nScreenY = nClientY - gnViewportOffsetY_6FBC21CC;

    RECT tRect;
    tRect.left   = nScreenX - 16;
    tRect.top    = nScreenY;
    tRect.right  = nScreenX + 16;
    tRect.bottom = nScreenY + 16;
    D2GFX_UtilDiamond(&tRect, nPaletteColor);
}

// Compatibility helper retained for callers expecting isometric projection
void __fastcall D2Client_SubtileToScreenCoords(int32_t nSubtileX, int32_t nSubtileY, int32_t* pScreenX, int32_t* pScreenY)
{
    *pScreenX = (nSubtileX - nSubtileY) * 16;
    *pScreenY = (nSubtileX + nSubtileY) * 8;
}

// 1.12a: 0x6FB0D580 (__stdcall, ret 0x00)
void __stdcall D2Client_ViewportDebugStub6FB0D580()
{
}

// 1.12a: 0x6FB0D590 (__stdcall, ret 0x00)
void __stdcall D2Client_ViewportDebugStub6FB0D590()
{
}

// 1.12a: 0x6FB0D5A0 (register ABI: ebx = pInOutX, edi = pInOutY; ret 0x00)
void __fastcall D2Client_ScreenToSubtileCoords(int32_t* pInOutX, int32_t* pInOutY)
{
    int32_t nX = *pInOutX;
    int32_t nY = *pInOutY;

    if (D2GFX_CheckPerspective())
    {
        // Exact 32x32->64 fixed-point perspective unprojection: nY += (-nY / 40)
        const int64_t qwProd = static_cast<int64_t>(nY) * static_cast<int32_t>(0x99999999u);
        const int32_t nHigh  = static_cast<int32_t>(qwProd >> 32) >> 4;
        const int32_t nDeltaY = nHigh + static_cast<int32_t>(static_cast<uint32_t>(nHigh) >> 31);
        nY += nDeltaY;

        const int32_t nHalfWidth = gnScreenWidth / 2;
        if (nHalfWidth != 0)
        {
            const int32_t nPerspectiveScale = 40 - (nY / 4);
            nX += ((nX - nHalfWidth) * nPerspectiveScale) / nHalfWidth;
        }
    }

    nX += (gnViewportOffsetX_6FBC21D0 - gnViewportShiftOffsetX);
    nY += gnViewportOffsetY_6FBC21CC;

    DUNGEON_ClientToGameSubtileCoords(&nX, &nY);

    *pInOutX = nX;
    *pInOutY = nY;
}

// 1.12a: 0x6FB0D650 (register ABI: eax = nOffsetX, ecx = nOffsetY; ret 0x00)
void __fastcall D2Client_SetViewportOffset(int32_t nOffsetX, int32_t nOffsetY)
{
    gnViewportOffsetX_6FBC21D0 = nOffsetX;
    gnViewportOffsetY_6FBC21CC = nOffsetY;
}

void __fastcall D2Client_ShiftViewportOffset(int32_t nDeltaX, int32_t nDeltaY)
{
    gnViewportOffsetX_6FBC21D0 += nDeltaX;
    gnViewportOffsetY_6FBC21CC += nDeltaY;
}

// 1.12a: 0x6FB0D660 (__stdcall, ret 0x00)
int32_t __stdcall D2Client_GetViewportOffsetYAdjusted()
{
    return gnViewportOffsetY_6FBC21CC - 8;
}

// 1.12a: 0x6FB0D670 (__stdcall, ret 0x00)
int32_t __stdcall D2Client_GetViewportOffsetXShifted()
{
    return gnViewportOffsetX_6FBC21D0 - gnViewportShiftOffsetX;
}

// 1.12a: 0x6FB0D680 (__stdcall, ret 0x00)
int32_t __stdcall D2Client_GetViewportOffsetY()
{
    return gnViewportOffsetY_6FBC21CC;
}

// 1.12a: 0x6FB0D690 (__stdcall, ret 0x00)
int32_t __stdcall D2Client_GetViewportOffsetX()
{
    return gnViewportOffsetX_6FBC21D0;
}

// 1.12a: 0x6FB0D6A0 (__stdcall, ret 0x00)
int32_t __stdcall D2Client_GetViewportShiftMode()
{
    return gnViewportShiftMode;
}

// 1.12a: 0x6FB0D6B0 (__stdcall, ret 0x00)
int32_t __stdcall D2Client_GetDebugRoomFlag()
{
    return g_bDebugRoomFlag_6FB946D0;
}

// 1.12a: 0x6FB0D6C0 (register ABI: eax = bEnable; ret 0x00)
void __fastcall D2Client_SetDebugRoomFlag(int32_t bEnable)
{
    g_bDebugRoomFlag_6FB946D0 = bEnable;
}

// 1.12a: 0x6FB0D6D0 (__stdcall, ret 0x00)
int32_t __stdcall D2Client_GetDebugTileStep()
{
    return g_nDebugTileStep_6FB946CC;
}

// 1.12a: 0x6FB0D6E0 (register ABI: eax = nStep; ret 0x00)
void __fastcall D2Client_SetDebugTileStep(int32_t nStep)
{
    g_nDebugTileStep_6FB946CC = nStep;
}

// 1.12a: 0x6FB0D6F0 (__stdcall, ret 0x04)
void __stdcall D2Client_RenderDebugRoomAndTileOverlays(D2ActiveRoomStrc* pRoom)
{
    int32_t nWallCount = 0;
    D2DrlgTileDataStrc* pWallTiles = DUNGEON_GetWallTilesFromRoom(pRoom, &nWallCount);
    if (!pWallTiles && nWallCount != 0)
    {
        D2Client_DisplayErrorHaltInline(0x2D5);
    }

    D2DrlgCoordsStrc tRoomCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &tRoomCoords);

    for (int32_t i = 0; i < nWallCount; ++i)
    {
        const D2DrlgTileDataStrc& rTile = pWallTiles[i];
        if (rTile.unk0x10 == 0)
        {
            continue;
        }

        int32_t nClientX = (rTile.nPosX + tRoomCoords.nTileXPos) * 5 + 3;
        int32_t nClientY = (rTile.nPosY + tRoomCoords.nTileYPos) * 5 + 3;
        DUNGEON_GameSubtileToClientCoords(&nClientX, &nClientY);

        if (rTile.nTileType == 15 || (rTile.dwFlags & 0x1C000) != 0x4000)
        {
            continue;
        }

        const int32_t nTileLibFlags = D2CMP_10079_GetTileFlags(rTile.pTile);
        if (nTileLibFlags & 0x04)
        {
            D2Client_DrawClientDiamondOverlay(nClientX + 32, nClientY, 0x78);
        }
        else if (rTile.dwFlags & 0x0800)
        {
            D2Client_DrawClientDiamondOverlay(nClientX + 16, nClientY + 8, 0x78);
        }
        else
        {
            D2Client_DrawClientDiamondOverlay(nClientX, nClientY, 0xFF);
        }

        const int32_t nLabelX = nClientX - gnViewportOffsetX_6FBC21D0 - 7;
        const int32_t nLabelY = nClientY - gnViewportOffsetY_6FBC21CC;
        const int32_t* pOwnerRoomInfo = reinterpret_cast<const int32_t*>(static_cast<intptr_t>(rTile.unk0x10));
        D2GFX_OutputString(nLabelX, nLabelY, "%d", pOwnerRoomInfo[10]); // +0x28 = nIndex
    }

    D2ActiveRoomStrc* pPlayerRoom = gpPlayerUnit ? UNITS_GetRoom(gpPlayerUnit) : nullptr;
    if (pRoom != pPlayerRoom)
    {
        return;
    }

    for (D2RoomCoordListStrc* pCoordNode = DUNGEON_GetRoomCoordList(pRoom); pCoordNode != nullptr; pCoordNode = pCoordNode->pNext)
    {
        int32_t aCornerX[4] = {
            pCoordNode->pBox[0].nPosX,
            pCoordNode->pBox[0].nPosX,
            pCoordNode->pBox[0].nWidth,
            pCoordNode->pBox[0].nWidth
        };
        int32_t aCornerY[4] = {
            pCoordNode->pBox[0].nPosY,
            pCoordNode->pBox[0].nHeight,
            pCoordNode->pBox[0].nPosY,
            pCoordNode->pBox[0].nHeight
        };

        for (int32_t k = 0; k < 4; ++k)
        {
            DUNGEON_GameTileToClientCoords(&aCornerX[k], &aCornerY[k]);
            aCornerX[k] -= gnViewportOffsetX_6FBC21D0;
            aCornerY[k] -= gnViewportOffsetY_6FBC21CC;
        }

        D2ActiveRoomStrc* pCurPlayerRoom = gpPlayerUnit ? UNITS_GetRoom(gpPlayerUnit) : nullptr;
        if (pRoom == pCurPlayerRoom)
        {
            D2GFX_DrawLine(aCornerX[0], aCornerY[0], aCornerX[0], aCornerY[0] + 16, 0x78, 0xFF);
            D2GFX_DrawLine(aCornerX[1], aCornerY[1], aCornerX[1] + 16, aCornerY[1], 0x78, 0xFF);
            D2GFX_DrawLine(aCornerX[2], aCornerY[2], aCornerX[2] - 16, aCornerY[2], 0x78, 0xFF);
            D2GFX_DrawLine(aCornerX[3], aCornerY[3], aCornerX[3], aCornerY[3] - 16, 0x78, 0xFF);
        }

        D2GFX_DrawLine(aCornerX[0], aCornerY[0], aCornerX[1], aCornerY[1], 0x78, 0xFF);
        D2GFX_DrawLine(aCornerX[0], aCornerY[0], aCornerX[2], aCornerY[2], 0x78, 0xFF);
        D2GFX_DrawLine(aCornerX[1], aCornerY[1], aCornerX[3], aCornerY[3], 0x78, 0xFF);
        D2GFX_DrawLine(aCornerX[2], aCornerY[2], aCornerX[3], aCornerY[3], 0x78, 0xFF);
    }
}

// 1.12a: 0x6FB0D9F0 (__stdcall, ret 0x04)
void __stdcall D2Client_RenderDebugSubtileLogicalRoomGrid(D2ActiveRoomStrc* pRoom)
{
    D2DrlgCoordsStrc tCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &tCoords);

    for (int32_t nSubY = tCoords.nSubtileY + 2;
         nSubY < tCoords.nSubtileY + tCoords.nSubtileHeight + 5;
         nSubY += 5)
    {
        for (int32_t nSubX = tCoords.nSubtileX + 3;
             nSubX < tCoords.nSubtileX + tCoords.nSubtileWidth + 5;
             nSubX += 5)
        {
            const int32_t nLogicalRoomIdx = D2Common_10095(pRoom, nSubX, nSubY);
            int32_t nClientX = nSubX;
            int32_t nClientY = nSubY;
            DUNGEON_GameSubtileToClientCoords(&nClientX, &nClientY);

            const int32_t nScreenX = nClientX - gnViewportOffsetX_6FBC21D0;
            const int32_t nScreenY = nClientY - gnViewportOffsetY_6FBC21CC;

            RECT tDiamondRect;
            tDiamondRect.left   = nScreenX - 16;
            tDiamondRect.top    = nScreenY;
            tDiamondRect.right  = nScreenX + 16;
            tDiamondRect.bottom = nScreenY + 16;
            D2GFX_UtilDiamond(&tDiamondRect, 0x46);

            D2GFX_OutputString(
                nClientX - gnViewportOffsetX_6FBC21D0 - 7,
                nClientY - gnViewportOffsetY_6FBC21CC,
                "%d",
                nLogicalRoomIdx);
        }
    }
}

// 1.12a: 0x6FB0DB10 (__stdcall, ret 0x00)
void __stdcall D2Client_UpdateViewportOffsetFromPlayer()
{
    gnPanelViewportBiasX_6FBC21C8 = -160;
    if (gnViewportShiftMode == 2)
    {
        gnPanelViewportBiasX_6FBC21C8 = (gnScreenWidth / 2) - 160;
    }
    else if (gnViewportShiftMode == 3)
    {
        gnPanelViewportBiasX_6FBC21C8 = 0;
    }

    D2UnitStrc* pPlayer = gpPlayerUnit;
    int32_t nOffsetX = UNITS_GetClientCoordX(pPlayer) - (gnScreenWidth / 2);
    int32_t nOffsetY = UNITS_GetClientCoordY(pPlayer) - (gnScreenHeight / 2) + 16;

    gnViewportOffsetX_6FBC21D0 = nOffsetX;
    gnViewportOffsetY_6FBC21CC = nOffsetY;

    if (g_bScreenShakeActive)
    {
        nOffsetX += g_nShakeOffsetX;
        nOffsetY += g_nShakeOffsetY;
        gnViewportOffsetX_6FBC21D0 = nOffsetX;
        gnViewportOffsetY_6FBC21CC = nOffsetY;
    }
}

// 1.12a: 0x6FB0DBB0 (__stdcall, ret 0x04)
void __stdcall D2Client_RenderDebugWarpBoundsAndExit(D2UnitStrc* pTileUnit)
{
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pTileUnit);
    D2LvlWarpTxt* pLvlWarpTxt = DUNGEON_GetLvlWarpTxtRecordFromRoomAndUnit(pRoom, pTileUnit);
    if (!pLvlWarpTxt)
    {
        return;
    }

    int32_t nWarpClientX = D2Client_GetUnitSubtileXInline(pTileUnit);
    int32_t nWarpClientY = D2Client_GetUnitSubtileYInline(pTileUnit);
    DUNGEON_GameSubtileToClientCoords(&nWarpClientX, &nWarpClientY);

    const int32_t nBoxLeft   = static_cast<int32_t>(pLvlWarpTxt->dwSelectX) - gnViewportOffsetX_6FBC21D0 + nWarpClientX;
    const int32_t nBoxTop    = static_cast<int32_t>(pLvlWarpTxt->dwSelectY) - gnViewportOffsetY_6FBC21CC + nWarpClientY;
    const int32_t nBoxRight  = nBoxLeft + static_cast<int32_t>(pLvlWarpTxt->dwSelectDX);
    const int32_t nBoxBottom = nBoxTop  + static_cast<int32_t>(pLvlWarpTxt->dwSelectDY);

    D2GFX_DrawLine(nBoxLeft,  nBoxTop,    nBoxLeft,  nBoxBottom, 0xFF, 0xFF);
    D2GFX_DrawLine(nBoxLeft,  nBoxTop,    nBoxRight, nBoxTop,    0xFF, 0xFF);
    D2GFX_DrawLine(nBoxLeft,  nBoxBottom, nBoxRight, nBoxBottom, 0xFF, 0xFF);
    D2GFX_DrawLine(nBoxRight, nBoxTop,    nBoxRight, nBoxBottom, 0xFF, 0xFF);

    const int32_t nOriginScreenX = (nWarpClientX + 1) - gnViewportOffsetX_6FBC21D0;
    const int32_t nOriginScreenY = nWarpClientY - gnViewportOffsetY_6FBC21CC;
    RECT tOriginDiamond = { nOriginScreenX - 16, nOriginScreenY, nOriginScreenX + 16, nOriginScreenY + 16 };
    D2GFX_UtilDiamond(&tOriginDiamond, 0xFF);

    int32_t nExitClientX = D2Client_GetUnitSubtileXInline(pTileUnit) + static_cast<int32_t>(pLvlWarpTxt->dwExitWalkX);
    int32_t nExitClientY = D2Client_GetUnitSubtileYInline(pTileUnit) + static_cast<int32_t>(pLvlWarpTxt->dwExitWalkY);
    DUNGEON_GameSubtileToClientCoords(&nExitClientX, &nExitClientY);

    const int32_t nExitScreenX = (nExitClientX + 1) - gnViewportOffsetX_6FBC21D0;
    const int32_t nExitScreenY = nExitClientY - gnViewportOffsetY_6FBC21CC;
    RECT tExitDiamond = { nExitScreenX - 16, nExitScreenY, nExitScreenX + 16, nExitScreenY + 16 };
    D2GFX_UtilDiamond(&tExitDiamond, 0x2D);
}

// 1.12a: 0x6FB0DDE0 (__stdcall, ret 0x04)
void __stdcall D2Client_RenderDebugRoomUnitsAndTileLinks(D2ActiveRoomStrc* pRoom)
{
    D2ActiveRoomStrc** ppRoomList = nullptr;
    int32_t nNumRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

    for (uint32_t i = 0; i < static_cast<uint32_t>(nNumRooms); ++i)
    {
        for (D2UnitStrc* pUnit = pRoom ? pRoom->pUnitFirst : nullptr; pUnit != nullptr; pUnit = pUnit->pRoomNext)
        {
            const int32_t nUnitScreenX = UNITS_GetClientCoordX(pUnit) - gnViewportOffsetX_6FBC21D0;
            const int32_t nUnitScreenY = UNITS_GetClientCoordY(pUnit) - gnViewportOffsetY_6FBC21CC;
            D2GFX_DrawBox(nUnitScreenX, nUnitScreenY, nUnitScreenX + 4, nUnitScreenY + 4, 0xFF, static_cast<DrawMode>(5));

            if (pUnit->dwUnitType == UNIT_TILE && pUnit->pGfxData)
            {
                for (const D2TileDebugLinkNodeStrc* pNode = reinterpret_cast<const D2TileDebugLinkNodeStrc*>(pUnit->pGfxData);
                     pNode != nullptr;
                     pNode = pNode->pNext)
                {
                    const int32_t nTargetX = pNode->nScreenX - gnViewportOffsetX_6FBC21D0;
                    const int32_t nTargetY = pNode->nScreenY - gnViewportOffsetY_6FBC21CC;
                    const int32_t nSrcY    = UNITS_GetClientCoordY(pUnit) - gnViewportOffsetY_6FBC21CC;
                    const int32_t nSrcX    = UNITS_GetClientCoordX(pUnit) - gnViewportOffsetX_6FBC21D0;
                    D2GFX_DrawLine(nSrcX, nSrcY, nTargetX, nTargetY, 0xFF, 0xFF);
                }
            }
        }
    }
}

// 1.12a: 0x6FB0DED0 (__stdcall, ret 0x00)
void __stdcall D2Client_RenderDebugRoomCoordListsAndWarps()
{
    if (D2GFX_GetDisplayType() == DISPLAYTYPE_GDI)
    {
        return;
    }

    D2UnitStrc* pPlayer = gpPlayerUnit;
    if (!pPlayer)
    {
        return;
    }

    const int32_t nPlayerX = D2Client_GetUnitSubtileXInline(pPlayer);
    const int32_t nPlayerY = D2Client_GetUnitSubtileYInline(pPlayer);
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pPlayer);

    const int32_t nLogicalRoom = D2Common_10095(pRoom, nPlayerX, nPlayerY);
    D2GFX_OutputString(10, 55, "Logical Room->%d", nLogicalRoom);

    D2Client_RenderDebugSubtileLogicalRoomGrid(pRoom);
    D2Client_RenderDebugRoomAndTileOverlays(pRoom);
}

// 1.12a: 0x6FB0DF80 (__stdcall, ret 0x00)
void __stdcall D2Client_RenderDebugLevelAndRoomOutlines()
{
    if (!gpPlayerUnit)
    {
        return;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(gpPlayerUnit);
    if (!pRoom)
    {
        return;
    }

    D2ActiveRoomStrc** ppRoomList = nullptr;
    int32_t nNumRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

    for (uint32_t i = 0; i < static_cast<uint32_t>(nNumRooms); ++i)
    {
        for (D2UnitStrc* pUnit = pRoom->pUnitFirst; pUnit != nullptr; pUnit = pUnit->pRoomNext)
        {
            if (pUnit->dwUnitType == UNIT_TILE)
            {
                D2Client_RenderDebugWarpBoundsAndExit(pUnit);
            }
        }
    }
}


// Cross-TU VA-Mapped Adapters for UNIT_Player.cpp
struct D2PlayerActionStrc;
extern int32_t __stdcall D2Client_AreUnitsHostileOrAttackable(D2UnitStrc* pTargetUnit, D2UnitStrc* pAttackerUnit);
extern D2UnitStrc* __stdcall sub_6FACEB20(D2UnitStrc* pUnit);
extern BOOL __fastcall D2Client_CanUseSkillInCurrentRoom(D2SkillStrc* pSkill);
extern void __stdcall GAME_ResetAllMouseStates(void);
extern BOOL __stdcall D2Client_ComputePlayerStepMoveTarget(D2UnitStrc* pUnit, D2PlayerActionStrc* pAction);
extern void __fastcall D2Client_ExecuteLocalPlayerTargetedAction(int32_t nMode, int32_t nTargetUnitType, uint32_t dwTargetUnitId);
extern D2UnitStrc* __stdcall D2Client_FindUnitInHashBucket(uint32_t nBucketIndex, uint32_t dwUnitGUID, D2UnitStrc** ppHashTable, uint32_t nUnitType);
extern void __stdcall D2Client_FreeUnitInventoryItems(D2UnitStrc* pUnit, int32_t bKeepSharedItems);
extern uint8_t __stdcall D2Client_GetPlayerEquippedItemComponent(D2UnitStrc* pPlayerUnit, D2UnitStrc* pItem);
extern int32_t __fastcall WINMAIN_GetPlayerSkillUseState(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
extern BOOL __fastcall CLTSKILL_ResolveTargetOrDirectionCoords(D2UnitStrc* pUnit, int32_t* pOutX, int32_t* pOutY);
extern D2UnitStrc* __fastcall D2Client_ClearActionFrameAndGetTarget(D2UnitStrc* pUnit);
extern int32_t __fastcall CLTSKILL_DoUsedSkill(D2UnitStrc* pUnit);
extern BOOL __stdcall D2Client_IsPlayerSkillWithinRange(int32_t* pOutRange, D2SkillStrc* pSkill, D2UnitStrc* pTargetUnit, int32_t* pOutDist);
extern int __fastcall sub_6FACEAC0(D2UnitStrc* pUnit);
extern void __fastcall QUESTFX_CheckNihlathakTempleTaunt(D2ActiveRoomStrc* pRoom);
extern void __fastcall D2Client_OnPlayerDeathCleanup(D2UnitStrc* pPlayerUnit);
extern int __cdecl sub_6FACC3F0(void);
extern void __fastcall D2Client_PlayMonsterFootstepSoundPerTick(D2UnitStrc* pUnit);
extern void __stdcall CLTSKILL_ProcessActiveStatesDoFuncs(D2UnitStrc* pUnit);
extern void __fastcall SELECT_ClearSelectedUnit(void);
extern void __fastcall D2Client_SetPathTargetPosition(D2DynamicPathStrc* pDynamicPath, int32_t nTargetX, int32_t nTargetY);
extern void __fastcall D2Client_SetPathTargetServerUnit(D2DynamicPathStrc* pDynamicPath, int32_t nTargetUnitType, uint32_t dwTargetUnitId);
struct D2SkillStartRequestStrc;
extern int32_t __fastcall CLTSKILL_StartSkillAtCoords(D2UnitStrc* pUnit, D2SkillStartRequestStrc* pReq);
extern int32_t __fastcall CLTSKILL_StartSkillOnTargetUnit(D2UnitStrc* pUnit, D2SkillStartRequestStrc* pReq);
extern int32_t __fastcall UNITMODE_CheckPositionDrift(D2UnitStrc* pUnit, int32_t nServerX, int32_t nServerY);
extern void __stdcall D2Client_TeleportUnitToCoordinates(D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t bTeleportEffect);
extern void __fastcall CLTSKILL_UpdateBlazeStepTrail(D2UnitStrc* pUnit);
extern D2UnitStrc* __fastcall CLTSKILL_SpawnSpiderGooLay(D2UnitStrc* pUnit);
extern BOOL __fastcall D2Client_ValidatePlayerInputAction(D2PlayerActionStrc* pAction);
struct D2LightNodeStrc;
extern void __stdcall LIGHTMAP_SetLightNodeColor(D2LightNodeStrc* pLight, uint8_t nRed, uint8_t nGreen, uint8_t nBlue);
extern void __fastcall LIGHTMAP_SetLightNodeRadiusImmediate(int32_t nRadius, D2LightNodeStrc* pLight);
extern int32_t __fastcall MSG_SendByte4Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2, uint32_t dw3, uint32_t dw4);
extern D2UnitStrc* __fastcall SELECT_AcquireSkillTarget(int32_t bTargetCorpses, int32_t (__fastcall* pfSkillFilter)(D2UnitStrc*));
extern void __fastcall GFX_FreeComponentCellData(D2UnitStrc* pUnit, int nComponent);

BOOL __stdcall D2Client_AreUnitsHostile_6FACF980(D2UnitStrc* pTarget, D2UnitStrc* pAttacker) { return D2Client_AreUnitsHostileOrAttackable(pTarget, pAttacker); }
extern int32_t __stdcall D2Client_CanPlayerInitiateAction(D2UnitStrc* pUnit, int32_t nArg);
BOOL __stdcall D2Client_CanPlayerStartSkillWithMode_6FACEB20(D2UnitStrc* pUnit, D2SkillStrc* pSkill) { (void)pSkill; return pUnit ? D2Client_CanPlayerInitiateAction(pUnit, 0) : FALSE; }
BOOL __fastcall D2Client_CanUseSkillInTown_6FB096C0(D2SkillStrc* pSkill) { return D2Client_CanUseSkillInCurrentRoom(pSkill); }
void __cdecl D2Client_ClearMouseHoldState_6FB29F80(void) { GAME_ResetAllMouseStates(); }
void __fastcall D2Client_ClearUnitComponentGfx_6FB41550(D2UnitStrc* pUnit, uint8_t nComp) { GFX_FreeComponentCellData(pUnit, nComp); }
BOOL __stdcall D2Client_ComputePlayerStepMoveTarget_6FB09710(D2UnitStrc* pUnit, D2PlayerActionStateStrc* pAction) { return D2Client_ComputePlayerStepMoveTarget(pUnit, reinterpret_cast<D2PlayerActionStrc*>(pAction)); }
extern void __fastcall D2Client_SendPacket01_WalkToLocation_6FB6ED00(int32_t nX, int32_t nY);
extern void __fastcall D2Client_SendPacket02_WalkToEntity_6FB6E730(int32_t nTargetType, uint32_t dwTargetId);
extern void __fastcall D2Client_SendPacket03_RunToLocation_6FB6ED30(int32_t nX, int32_t nY);
extern void __fastcall D2Client_SendPacket04_RunToEntity_6FB6E770(int32_t nTargetType, uint32_t dwTargetId);

void __stdcall D2Client_ExecuteLocalPlayerUnitOrLocationAction_6FAF14C0(D2UnitStrc* pUnit, int32_t nArgXOrType, uint32_t dwArgYOrGUID, int32_t nActionCode, BOOL bForce)
{
    if (!pUnit)
    {
        return;
    }

    D2PlayerModeChangeStrc tModeChange = {};
    tModeChange.nSkillIdOrXOrType = nArgXOrType;
    tModeChange.dwOwnerOrYOrGUID  = dwArgYOrGUID;

    switch (nActionCode)
    {
    case 1: // C2S 0x01: Walk to location
        if (D2Client_ExecutePlayerModeTransition(1, pUnit, &tModeChange, bForce) && pUnit == gpPlayerUnit)
        {
            D2Client_SendPacket01_WalkToLocation_6FB6ED00(nArgXOrType, static_cast<int32_t>(dwArgYOrGUID));
        }
        break;

    case 2: // C2S 0x02: Walk to target unit
        if (D2Client_ExecutePlayerModeTransition(0, pUnit, &tModeChange, bForce) && pUnit == gpPlayerUnit)
        {
            D2Client_SendPacket02_WalkToEntity_6FB6E730(nArgXOrType, dwArgYOrGUID);
        }
        break;

    case 3: // C2S 0x03: Run to location
        if (D2Client_ExecutePlayerModeTransition(23, pUnit, &tModeChange, bForce) && pUnit == gpPlayerUnit)
        {
            D2Client_SendPacket03_RunToLocation_6FB6ED30(nArgXOrType, static_cast<int32_t>(dwArgYOrGUID));
        }
        break;

    case 4: // C2S 0x04: Run to target unit
        if (D2Client_ExecutePlayerModeTransition(24, pUnit, &tModeChange, bForce) && pUnit == gpPlayerUnit)
        {
            D2Client_SendPacket04_RunToEntity_6FB6E770(nArgXOrType, dwArgYOrGUID);
        }
        break;

    case 5: // C2S 0x05: Left skill on location
        D2Client_DispatchLocalSkillTransitionAndSendPacket(
            pUnit,
            UNITS_GetLeftSkill(pUnit),
            "..\\Source\\D2Client\\UNIT\\UnitMode.cpp",
            812,
            21,
            nArgXOrType,
            dwArgYOrGUID,
            0x05,
            FALSE);
        break;

    case 6: // C2S 0x06: Left skill on target unit
        D2Client_DispatchLocalSkillTransitionAndSendPacket(
            pUnit,
            UNITS_GetLeftSkill(pUnit),
            "..\\Source\\D2Client\\UNIT\\UnitMode.cpp",
            822,
            22,
            nArgXOrType,
            dwArgYOrGUID,
            0x06,
            TRUE);
        break;

    case 0x0D: // C2S 0x0D: Right skill on target unit (from queued action)
        D2Client_DispatchLocalSkillTransitionAndSendPacket(
            pUnit,
            UNITS_GetRightSkill(pUnit),
            "..\\Source\\D2Client\\UNIT\\UnitMode.cpp",
            840,
            22,
            nArgXOrType,
            dwArgYOrGUID,
            0x0D,
            TRUE);
        break;

    default:
        D2Client_ExecuteLocalPlayerTargetedAction(nActionCode, nArgXOrType, dwArgYOrGUID);
        break;
    }
}
D2UnitStrc* __fastcall D2Client_FindUnitInHashTable_6FACE550(D2UnitStrc** ppHash, int32_t dwGUID, int32_t nType)
{
    return ppHash ? D2Client_FindUnitInHashBucket(static_cast<uint32_t>(dwGUID) & 0x7Fu, static_cast<uint32_t>(dwGUID), ppHash, static_cast<uint32_t>(nType)) : nullptr;
}
void __stdcall D2Client_FreeUnitInventoryItems_6FACF290(D2UnitStrc* pUnit, BOOL bKeep) { D2Client_FreeUnitInventoryItems(pUnit, bKeep); }
uint8_t __stdcall D2Client_GetPlayerEquippedItemComponent_6FB09540(D2UnitStrc* pPlayer, D2UnitStrc* pItem) { return D2Client_GetPlayerEquippedItemComponent(pPlayer, pItem); }
int32_t __fastcall D2Client_GetPlayerSkillUseState_6FB53610(D2UnitStrc* pUnit, D2SkillStrc* pSkill) { return WINMAIN_GetPlayerSkillUseState(pUnit, pSkill); }
void __stdcall D2Client_GetUnitCurrentPathTargetCoords_6FAFE320(D2UnitStrc* pUnit, int32_t* pX, int32_t* pY) { CLTSKILL_ResolveTargetOrDirectionCoords(pUnit, pX, pY); }
D2UnitStrc* __fastcall D2Client_GetValidatedPathTargetUnit_6FACF5B0(D2UnitStrc* pUnit) { return D2Client_ClearActionFrameAndGetTarget(pUnit); }
int32_t __fastcall D2Client_InvokeClientSkillDoFunc_6FB00150(D2UnitStrc* pUnit) { return CLTSKILL_DoUsedSkill(pUnit); }
BOOL __stdcall D2Client_IsPlayerSkillWithinRange_6FB095D0(D2SkillStrc* pSkill, D2UnitStrc* pTarget, int32_t* pOutMaxRange, int32_t* pOutDist)
{
    return D2Client_IsPlayerSkillWithinRange(pOutMaxRange, pSkill, pTarget, pOutDist);
}
BOOL __fastcall D2Client_IsUnitDead_6FACEAC0(D2UnitStrc* pUnit) { return sub_6FACEAC0(pUnit); }
void __fastcall D2Client_NotifyAutomapRoomChanged_6FAC35A0(D2ActiveRoomStrc* pRoom) { QUESTFX_CheckNihlathakTempleTaunt(pRoom); }
void __fastcall D2Client_OnPlayerDeathCleanup_6FB098F0(D2UnitStrc* pUnit) { D2Client_OnPlayerDeathCleanup(pUnit); }
void __fastcall D2Client_OnPlayerLeaveTown_6FACC3F0(D2UnitStrc* pPlayerUnit) { (void)pPlayerUnit; sub_6FACC3F0(); }
void __stdcall D2Client_PlayPlayerFootstepSound_6FAE8590(D2UnitStrc* pUnit) { D2Client_PlayMonsterFootstepSoundPerTick(pUnit); }
void __stdcall D2Client_ProcessActiveUnitStates_6FAFD9F0(D2UnitStrc* pUnit) { CLTSKILL_ProcessActiveStatesDoFuncs(pUnit); }
void __fastcall D2Client_SetMercPetSpecialFlag_6FB1E6E0(D2UnitStrc* pPlayer, D2UnitStrc* pMerc, int32_t nFlag) { (void)pPlayer;(void)pMerc;(void)nFlag; SELECT_ClearSelectedUnit(); }
int32_t __fastcall D2Client_SetUnitPathTargetPosition_6FAF0CF0(D2UnitStrc* pUnit, int32_t nX, int32_t nY) { if (pUnit && pUnit->pDynamicPath) D2Client_SetPathTargetPosition(pUnit->pDynamicPath, nX, nY); return 1; }
int32_t __fastcall D2Client_SetUnitPathTargetServerUnit_6FAF0CA0(D2UnitStrc* pUnit, int32_t nType, uint32_t dwGUID) { if (pUnit && pUnit->pDynamicPath) D2Client_SetPathTargetServerUnit(pUnit->pDynamicPath, nType, dwGUID); return 1; }
BOOL __stdcall D2Client_StartSkillAtLocation_6FAFF380(D2UnitStrc* pPlayer, D2PlayerModeChangeStrc* pModeChange)
{
    return CLTSKILL_StartSkillAtCoords(pPlayer, reinterpret_cast<D2SkillStartRequestStrc*>(pModeChange));
}
BOOL __fastcall D2Client_StartSkillOnTarget_6FB00210(D2UnitStrc* pPlayer, D2PlayerModeChangeStrc* pModeChange)
{
    return CLTSKILL_StartSkillOnTargetUnit(pPlayer, reinterpret_cast<D2SkillStartRequestStrc*>(pModeChange));
}
BOOL __stdcall D2Client_SyncUnitPosition_6FAF0EB0(D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t bCheckDrift, int32_t a5, int32_t a6)
{
    (void)bCheckDrift; (void)a5; (void)a6;
    return UNITMODE_CheckPositionDrift(pUnit, nX, nY);
}
void __stdcall D2Client_TeleportClientUnit_6FAD1550(D2UnitStrc* pUnit, int32_t nX, int32_t nY, BOOL bFx) { D2Client_TeleportUnitToCoordinates(pUnit, nX, nY, bFx); }
void __fastcall D2Client_UpdateBlazeTrail_6FB024E0(D2UnitStrc* pUnit) { CLTSKILL_UpdateBlazeStepTrail(pUnit); }
void __fastcall D2Client_UpdateState22Effect_6FB4D480(D2UnitStrc* pUnit) { CLTSKILL_SpawnSpiderGooLay(pUnit); }
BOOL __fastcall D2Client_ValidatePlayerInputAction_6FB09650(D2PlayerActionStateStrc* pAction) { return D2Client_ValidatePlayerInputAction(reinterpret_cast<D2PlayerActionStrc*>(pAction)); }
void __stdcall LIGHTMAP_SetLightColor_6FB0E590(D2GfxLightStrc* pLight, uint8_t r, uint8_t g, uint8_t b) { LIGHTMAP_SetLightNodeColor(reinterpret_cast<D2LightNodeStrc*>(pLight), r, g, b); }
void __fastcall LIGHTMAP_SetLightRadiusImmediate_6FB0EA30(D2GfxLightStrc* pLight, int32_t nRadius) { LIGHTMAP_SetLightNodeRadiusImmediate(nRadius, reinterpret_cast<D2LightNodeStrc*>(pLight)); }
void __fastcall MSG_SendTownFolkInvite_6FB29770(int32_t nUnitType, uint32_t dwUnitId, uint8_t bOpcode, int32_t nX, int32_t nY)
{
    MSG_SendByte4Dwords(bOpcode ? bOpcode : 0x59u, static_cast<uint32_t>(nUnitType), dwUnitId, static_cast<uint32_t>(nX), static_cast<uint32_t>(nY));
}
D2UnitStrc* __stdcall SELECT_AcquireSkillTarget_6FB1ED00(D2UnitStrc* pCurrentTarget, D2SkillStrc* pSkill, int32_t* pInOutX, int32_t* pInOutY, uint32_t dwActionFlags, BOOL bFilterMode)
{
    (void)pSkill; (void)pInOutX; (void)pInOutY; (void)dwActionFlags;
    if (pCurrentTarget)
    {
        return pCurrentTarget;
    }
    return SELECT_AcquireSkillTarget(bFilterMode, nullptr);
}
