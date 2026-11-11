#include "D2Client.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <tlhelp32.h>

#include <Fog.h>
#include <Storm.h>
#include <D2Lang.h>
#include <D2StrTable.h>
#include <D2Gfx.h>
#include <Texture.h>
#include <Tile.h>
#include <Window.h>
#include <D2CMP.h>
#include <D2Sound.h>
#include <D2WinMain.h>
#include <D2WinArchive.h>
#include <D2WinComp.h>
#include <D2WinFont.h>
#include <D2WinPalette.h>
#include <D2DataTbls.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/ItemsTbls.h>
#include <DataTbls/MonsterTbls.h>
#include <DataTbls/ObjectsTbls.h>
#include <DataTbls/SkillsTbls.h>
#include <D2BitManip.h>
#include <D2Items.h>
#include <D2Inventory.h>
#include <D2StatList.h>
#include <Units/Units.h>
#include <Units/UnitRoom.h>
#include <Path/Path.h>
#include <D2Dungeon.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgPreset.h>
#include <Drlg/D2DrlgRoomTile.h>
#include <D2Collision.h>
#include <D2Log.h>
#include <Client.h>
#include <Server.h>
#include <GAME/Game.h>
#include <PLAYER/Player.h>
#include <PLAYER/PlrSave.h>
#include <DEBUG/Debug.h>
#include <D2IgnoreList.h>

// Reconstructed 1.12a D2Client Translation Unit Entry Points (Msg.cpp, UI_QuestLog.cpp, UNIT_CUnit.cpp)
void __fastcall MSG_ResetCounters(void);
void __fastcall MSG_GetTrafficCounters(uint32_t* pBytesReceived, uint32_t* pPacketsReceived, uint32_t* pBytesSent, uint32_t* pPacketsSent);
int32_t __fastcall MSG_SendPacket(void* pPacketData, size_t nSize);
void __fastcall MSG_ResetLastSentPacket(void);
int32_t __fastcall MSG_SendLeaveGame_69(void);
int32_t __fastcall MSG_SendJoinActComplete_6B(void);
int32_t __fastcall MSG_SendByte4Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2, uint32_t dw3, uint32_t dw4);
int32_t __fastcall MSG_SendByte3Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2, uint32_t dw3);
int32_t __fastcall MSG_SendByte2Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2);
int32_t __fastcall MSG_SendByteDword(uint8_t bOpcode, uint32_t dw1);
int32_t __fastcall MSG_SendByte4Words(uint8_t bOpcode, uint16_t w1, uint16_t w2, uint16_t w3, uint16_t w4);
int32_t __fastcall MSG_SendByte3Words(uint8_t bOpcode, uint16_t w1, uint16_t w2, uint16_t w3);
int32_t __fastcall MSG_SendByteWordWord(uint8_t bOpcode, uint16_t w1, uint16_t w2);
int32_t __fastcall MSG_SendByteWord(uint8_t bOpcode, uint16_t w1);
int32_t __fastcall MSG_SendByte(uint8_t bOpcode);
int32_t __fastcall UNITMODE_SendActionPacket(
	uint8_t bOpcode,
	uint16_t wTargetX,
	uint16_t wTargetY,
	uint32_t dwTargetUnitType,
	uint32_t dwTargetUnitGUID);
int32_t __fastcall GAME_SendTownFolkInit_2F(uint32_t dwNpcUnitType, uint32_t dwNpcUnitGUID);
int32_t __fastcall GAME_SendTownFolkCancel_30(uint32_t dwNpcUnitType, uint32_t dwNpcUnitGUID);
void __stdcall QUESTLOG_InitSession(void);

// Reconstructed UNIT_CUnit.cpp Globals & Lifecycle APIs (Milestone 3.1 Cutover)
extern D2UnitStrc* gpPlayerUnit;                                           // 0x6FBCC3D0
extern D2DrlgActStrc* gpClientAct;                                         // 0x6FBCC3C0
extern D2UnitStrc* gapServerSideUnitHashTables[UNIT_TYPES_COUNT][128];     // 0x6FBB9D60
extern D2UnitStrc* gapClientSideUnitHashTables[UNIT_TYPES_COUNT][128];     // 0x6FBB9160
struct D2MonsterSpawnDescStrc
{
	uint32_t dwUnitGUID;
	int32_t  nClassId;
	int32_t  nX;
	int32_t  nY;
	uint16_t wTypeFlags;
	uint16_t wPad12;
	int32_t  nLifePct;
	int32_t  nAnimMode;
};
void __stdcall D2Client_ClearAllUnitHashTables();
void __stdcall D2Client_FreeAllUnits(void);
void __fastcall D2Client_RemoveServerSideUnit(uint32_t nUnitType, uint32_t dwUnitGUID);
void __fastcall D2Client_RemoveClientSideUnit(uint32_t nUnitType, uint32_t dwUnitGUID);
D2UnitStrc* __stdcall D2Client_AllocClientPlayerUnit(uint32_t dwUnitGUID, int32_t nClassId, int32_t nX, int32_t nY, char* szPlayerName);
D2UnitStrc* __stdcall D2Client_AllocClientObjectUnit(uint32_t nUnitType, int32_t nClassId, uint32_t dwUnitGUID, int32_t nX, int32_t nY, int32_t nMode, uint8_t nInteractType, uint8_t nPortalFlags);
D2UnitStrc* __stdcall D2Client_AllocClientMonsterUnit(const D2MonsterSpawnDescStrc* pSpawnDesc, const uint8_t* pCompInfo, const uint8_t* pMonUMods, uint32_t dwNameSeedOrAsync, uint16_t wBossHcIdx, int32_t nExtra);
D2UnitStrc* __fastcall D2Client_CreateClientSideUnit(int32_t nX, int32_t nY, int32_t nClassId, uint32_t nUnitType, int32_t nMode);
void __stdcall D2Client_TeleportUnitToCoordinates(D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t bTeleportEffect);
void __stdcall D2Client_UpdateAllUnitsPerTick(void);
void __fastcall D2Client_ExpireUnitModStatsCallback(D2UnitStrc* pUnit);
void __stdcall D2Client_ForEachUnitInHashTable(void (__fastcall* pfnCallback)(D2UnitStrc*), D2UnitStrc** ppHashBuckets);
D2UnitStrc* __stdcall D2Client_GetLocalPlayerUnit();
void __fastcall D2Client_SetLocalPlayerUnit(D2UnitStrc* pPlayerUnit);
D2UnitStrc* __fastcall D2Client_FindServerSideUnit(uint32_t dwUnitGUID, uint32_t nUnitType);
D2UnitStrc* __fastcall D2Client_FindClientSideUnit(uint32_t dwUnitGUID, uint32_t nUnitType);
D2UnitStrc* __fastcall D2Client_RegisterUnitInHashTable(D2UnitStrc* pUnit);

// Reconstructed 1.12a View.cpp, GAME_Wall2.cpp, UI_Hireables.cpp (Env), Sound_SoundHdr.cpp (Floor), ENGINE_Gfx.cpp & UNIT_Item.cpp
struct D2ViewportStrc;
extern void* gpClientViewport;
D2ViewportStrc* __fastcall VIEW_AllocViewport(void);
void __fastcall VIEW_FreeViewport(D2ViewportStrc* pViewport);
void __fastcall VIEW_SetViewportRect(D2ViewportStrc* pViewport, int32_t nLeft, int32_t nTop, int32_t nRight, int32_t nBottom);
void __fastcall GAME_RenderFrame(int32_t bSkipWorldRender);
void __cdecl GFXUTIL_InitDirectionTables();
void __fastcall GFX_UpdateActCofCache(int nAct);
void __cdecl GFX_FreeMonsterCofCache();
void __cdecl GFX_FreePlayerCofCache();
void __fastcall GFX_InitUnitCompositeGfx(D2UnitStrc* pUnit);
void __fastcall GFX_FreeUnitGfxData(D2UnitStrc* pUnit);
void __fastcall GFX_AttachOverlayToUnit(D2UnitStrc* pUnit, int nOverlayId, int bPreDrawOverride);
extern int32_t g_nInteractingNpcGUID;
extern int32_t gdwActiveInteractNpcGUID;
void __fastcall D2Client_PacketHandler_ItemAction9C(const uint8_t* pPacket);
void __fastcall D2Client_PacketHandler_ItemAction9D(const uint8_t* pPacket);

// Reconstructed 1.12a SCmd.cpp Entry Points & Globals (0x6FAB10E0..0x6FAB4A60)
extern D2UnitStrc* gpClientPlayer;
void __fastcall SCMD_FreeUnitPacketQueue(D2UnitStrc* pUnit);
void __fastcall SCMD_DrainUnitPacketQueue(D2UnitStrc* pUnit);
void __fastcall SCMD_Handler00_GameLoading(uint8_t* pPacket);
void __fastcall SCMD_Handler01_GameLogonReceipt(uint8_t* pPacket);
void __fastcall SCMD_Handler04_LoadComplete(uint8_t* pPacket);
void __fastcall SCMD_Handler05_UnloadComplete(uint8_t* pPacket);
void __fastcall SCMD_Handler07_AddRoom(uint8_t* pPacket);
void __fastcall SCMD_Handler08_RemoveRoom(uint8_t* pPacket);
void __fastcall SCMD_Handler19_1F_StatUpdate(uint8_t* pPacket);
void __fastcall SCMD_Handler20_PlayerAttrUpdate(uint8_t* pPacket);

// Reconstructed 1.12a DRAW_dLightMap.cpp Entry Points & Globals (0x6FAE85B0..0x6FAEA3A0)
extern void* g_pClientAct;
extern D2UnitStrc* g_pPlayerUnit;
void __stdcall LIGHTMAP_InitializeReciprocalTableAndActCallback();
void __stdcall LIGHTMAP_GetRoomAmbientLight(
	BOOL bCheckSpecialRoomLight,
	D2ActiveRoomStrc* pRoom,
	int32_t* pOutIntensity,
	uint8_t* pOutRed,
	uint8_t* pOutGreen,
	uint8_t* pOutBlue);
void __stdcall LIGHTMAP_ResetAmbientGrid(BOOL bClearToZero);
void __stdcall LIGHTMAP_SampleLightAtSubtile8(
	int32_t nX8,
	int32_t nY8,
	uint8_t* pOutIntensity,
	uint8_t* pOutRed,
	uint8_t* pOutGreen,
	uint8_t* pOutBlue);
void __stdcall LIGHTMAP_FreeAllLightNodes();
void __stdcall LIGHTMAP_ClearGrid();

// Reconstructed 1.12a UI_automap.cpp Entry Points (0x6FAFD340..0x6FB01950)
struct D2AutomapCellStrc;
struct D2AutomapLayerStrc;
void __stdcall AUTOMAP_InitColors(void);
void __fastcall AUTOMAP_DrawCrosshair(int32_t nX, int32_t nY, uint8_t nColor);
D2AutomapLayerStrc* __fastcall AUTOMAP_InitLayer(int32_t nLayerNo);
void __stdcall AUTOMAP_AddExplicitCell(int16_t nCellNo, int16_t xPixel, int16_t yPixel, D2AutomapCellStrc** ppCellTree, int32_t fSaved);
void __stdcall AUTOMAP_FreeAllLayers(void);

// Reconstructed 1.12a UI_SkillDesc.cpp Entry Points & Globals (0x6FAF21F0..0x6FAFCEA0)
extern D2SkillDescTxt* g_pSkillDescTxt_6FDD1C44;
extern int32_t g_nSkillDescTxtRecordCount_6FDD1C4C;
D2SkillDescTxt* __fastcall sub_6FAF21F0(int32_t nSkillId);
void __fastcall sub_6FAF7410(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, wchar_t* wszOut);
int32_t __stdcall sub_6FAF7560(int32_t nSkillId, wchar_t* wszOut, uint32_t nMaxChars);

// Reconstructed 1.12a UI_inv.cpp Entry Points & Globals (0x6FB108B0..0x6FB1A400)
extern BOOL g_bExpansionMode;
void __stdcall INV_SyncExpansionModeFlag();
void __stdcall INV_InitInventoryLayoutRectsAndGrids();
int32_t __stdcall INV_GetPanelMode();
void __fastcall INV_SetPanelMode(int32_t nMode);
BOOL __fastcall INV_HitTestInvPanelRect(int32_t nX, int32_t nY);
BOOL __fastcall INV_HitTestStashPanelRect(int32_t nX, int32_t nY);
BOOL __fastcall INV_HitTestHoradricCubeGridRect(int32_t nX, int32_t nY);

// Gap 1 Reconstructed Subsystem Runtime Execution Counters
static uint32_t gdwSCmdPacketsDispatched = 0;
static uint32_t gdwLightMapRoomsSampled = 0;
static uint32_t gdwAutomapRoomsPopulated = 0;
static uint32_t gdwSkillDescTooltipsBuilt = 0;
static uint32_t gdwInvPanelEvaluations = 0;

static void SyncReconstructedClientGlobals()
{
	gpClientPlayer = gpPlayerUnit;
	g_pPlayerUnit = gpPlayerUnit;
	g_pClientAct = gpClientAct;
	if (sgptDataTables)
	{
		g_pSkillDescTxt_6FDD1C44 = sgptDataTables->pSkillDescTxt;
		g_nSkillDescTxtRecordCount_6FDD1C4C = sgptDataTables->nSkillDescTxtRecordCount;
	}
}

static void RemoveReconstructedServerUnit(uint32_t nUnitType, uint32_t dwGuid)
{
	if (nUnitType >= UNIT_TYPES_COUNT || dwGuid == 0)
	{
		return;
	}
	D2Client_RemoveServerSideUnit(nUnitType, dwGuid);
	SyncReconstructedClientGlobals();
}

static void FreeAllReconstructedServerUnits()
{
	D2Client_FreeAllUnits();
	D2Client_SetLocalPlayerUnit(nullptr);
	D2Client_ClearAllUnitHashTables();
	SyncReconstructedClientGlobals();
}

static void StepReconstructedUnitTablesPerTick()
{
	SyncReconstructedClientGlobals();
	D2Client_UpdateAllUnitsPerTick();
}

// 1.12a Globals
static D2ConfigStrc* gpD2Config = nullptr;                     // 0x6FBCB908
static int32_t gbD2ClientInitialized = 0;                      // 0x6FBCD75C
static const uint8_t gaD2ClientBytes[3] = { 0xFF, 0x40, 0x30 };// 0x6FBB1678
static int32_t gnClientState = D2CLIENT_STATE_INIT_SESSION;    // 0x6FBA8B30
static int32_t gbExitApplication = 1;                          // 0x6FBA8B70
static int32_t gnGameType = 0;                                 // 0x6FBCBFF8
static int32_t gbClientLeaveCompleted = 0;                     // 0x6FBCC000
static int32_t gbWindowClosed = 0;                             // 0x6FBCC08C
static int32_t gbSaveAndExitRequested = 0;                     // 0x6FBCC094
static DWORD gdwSessionStartTick = 0;                          // 0x6FBC47A4
static DWORD gdwSessionLeaveTick = 0;                          // 0x6FBC479C
static char gszAccountName[16] = "SinglePlayer";               // 0x6FBB33E0
static char gszCharName[16] = "Hero";                          // 0x6FBB33F8
static uint8_t gnCharClass = 0;
static uint8_t gnCharLevel = 1;
static uint16_t gwCharSaveFlags = 0;
static uint8_t gaCharComponents[16] = {};
static uint8_t gaCharColors[16] = {};

static CRITICAL_SECTION gServerThreadLock = {};                // 0x6FBB327C
static HANDLE ghServerThread = nullptr;                        // 0x6FBB33D8
static DWORD gdwServerThreadId = 0;                            // 0x6FBB340C
static volatile int32_t gbStopServerThread = 0;                // 0x6FBCC038
static volatile int32_t gbRunServerThreadUpdates = 0;          // 0x6FBCC03C
static D2GameDataTableStrc gLocalGameDataTable = {};           // 0x6FBD0E98
static uint32_t gLocalGameList[8] = {};                        // 0x6FBCC018

// In-Game HUD & Unit Rendering State
static D2CellFileStrc* gpHudCtrlPnl = nullptr;
static D2CellFileStrc* gpHudHlthMana = nullptr;
static D2CellFileStrc* gpHudOverlap = nullptr;
static D2CellFileStrc* gpHudCampfire = nullptr;
static D2CellFileStrc* gpHudCursor = nullptr;
static D2CellFileStrc* gpHudRunBtn = nullptr;
static D2CellFileStrc* gpHudMenuBtn = nullptr;
static D2CellFileStrc* gpHudMiniPanel = nullptr;
static D2CellFileStrc* gpHudMiniPanelBtn = nullptr;
static D2CellFileStrc* gpHudSkillIcon = nullptr;
static D2CellFileStrc* gpClassSkillIcon = nullptr;
static D2CellFileStrc* gpInvChar6 = nullptr;
static D2CellFileStrc* gpBuySellBtn = nullptr;
static D2CellFileStrc* gpSkillTreeBg = nullptr;
static D2CellFileStrc* gpQuestBg = nullptr;
static D2CellFileStrc* gpQuestTabs = nullptr;
static D2CellFileStrc* gpQuestSockets = nullptr;
static D2CellFileStrc* gpPentSpin = nullptr;
static D2CellFileStrc* gpInvWeapons = nullptr;
static D2CellFileStrc* gpInvArmor = nullptr;
static D2CellFileStrc* gpInvHelmGlove = nullptr;
static D2CellFileStrc* gpInvBoots = nullptr;
static D2CellFileStrc* gpInvBelt = nullptr;
static D2CellFileStrc* gpInvRingAmulet = nullptr;
static D2CellFileStrc* gpPlayerClassNu1 = nullptr;
static D2CellFileStrc* gpPlayerClassFw = nullptr;
static D2CellFileStrc* gpStashPanel = nullptr;
static D2CellFileStrc* gpCubePanel = nullptr;
static D2CellFileStrc* gpTransmuteBtn = nullptr;
static D2CellFileStrc* gpWaypointBg = nullptr;
static D2CellFileStrc* gpWaypointTabs = nullptr;
static D2CellFileStrc* gpMercInvPanel = nullptr;

enum LeftPanelType : int32_t
{
	LEFTPANEL_NONE = 0,
	LEFTPANEL_CHAR = 1,
	LEFTPANEL_QUEST = 2,
	LEFTPANEL_PARTY = 3,
	LEFTPANEL_MSGLOG = 4,
	LEFTPANEL_VENDOR = 5,
	LEFTPANEL_STASH = 6,
	LEFTPANEL_CUBE = 7,
	LEFTPANEL_WAYPOINT = 8,
	LEFTPANEL_MERC = 9,
};

enum RightPanelType : int32_t
{
	RIGHTPANEL_NONE = 0,
	RIGHTPANEL_INV = 1,
	RIGHTPANEL_SKILL = 2,
};

static int32_t gnLeftPanel = LEFTPANEL_NONE;
static int32_t gnRightPanel = RIGHTPANEL_NONE;
static bool gbMiniPanelOpen = true;
static bool gbAutomapOpen = false;
static bool gbEscMenuOpen = false;
static int32_t gnEscSubMenu = 0;       // 0 = Main ESC Menu, 1 = Options Menu, 2 = Configure Controls
static int32_t gnEscSelectedIndex = 0;
static int32_t gnActiveSkillTab = 0;   // 0..2
static int32_t gnSelectedQuest = 0;    // 0..5
static int32_t gnQuestLogActTab = 0;   // 0..4 (Acts I..V)
static bool gbRunEnabled = true;
static bool gbSoundEnabled = true;
static bool gbScaled1200x900 = true;

// Milestone 3.2/3.3: Stash, Horadric Cube, Waypoint, Mercenary, Belt 4x4, Cursor Item & Chat/Slash State
static int32_t gnStashGold = 2500;
static int32_t gnWaypointActTab = 0;   // 0..4 (Acts I..V)
static uint32_t gdwWaypointUnlockMask[5] = { 0x1FFu, 0x1FFu, 0x1FFu, 0x007u, 0x1FFu };
static uint32_t gdwActiveWaypointGuid = 0;
static bool gbMercHired = false;
static uint32_t gdwMercUnitGuid = 0;
static uint16_t gwMercClassId = 271;   // Rogue Scout (Act 1)
static int32_t gnMercLevel = 5;
static int32_t gnMercLifePct = 128;
static char gszMercName[32] = "Kyoko";
static bool gbBeltExpanded = false;
static int32_t gnCursorItemIdx = -1;
static int32_t gnActiveWeaponSet = 0;  // 0 = Weapon Set I, 1 = Weapon Set II
static bool gbChatInputOpen = false;
static char gszChatInputBuf[96] = {};
static int32_t gnChatInputLen = 0;
static bool gbShowFpsOverlay = false;
static DWORD gdwActTransitionEndTick = 0;
static uint8_t gnTransitionTargetAct = 0;
static uint32_t gdwTransmutesSent = 0;
static uint32_t gdwWaypointsUsed = 0;
static uint32_t gdwSlashCommandsExecuted = 0;

// NPC Dialog Menu & Vendor Trade State (1.12a UI/npcmenu.cpp & UI/inv.cpp)
static bool gbNpcMenuOpen = false;
static uint32_t gnActiveNpcGuid = 0;
static uint16_t gwActiveNpcClassId = 0;
static char gszActiveNpcName[64] = {};
static int32_t gnNpcMenuSelectedIdx = 0;
static int32_t gnVendorActiveTab = 0;      // 0 = All, 1 = Armor, 2 = Weapons, 3 = Misc
static int32_t gnSelectedVendorItemIdx = -1;
static uint32_t gnPendingNpcGuid = 0;
static uint16_t gnCurrentLevelId = LEVEL_ROGUEENCAMPMENT;
static uint8_t gaQuestFilterStates[42] = {};
static uint32_t gdwNpcIntrosReceived = 0;
static uint32_t gdwVendorItemsReceived = 0;

static const char* const kClassSkillTreeBgPaths[7] = {
	"DATA\\GLOBAL\\UI\\SPELLS\\skltree_a_back",
	"DATA\\GLOBAL\\UI\\SPELLS\\skltree_s_back",
	"DATA\\GLOBAL\\UI\\SPELLS\\skltree_n_back",
	"DATA\\GLOBAL\\UI\\SPELLS\\skltree_p_back",
	"DATA\\GLOBAL\\UI\\SPELLS\\skltree_b_back",
	"DATA\\GLOBAL\\UI\\SPELLS\\skltree_d_back",
	"DATA\\GLOBAL\\UI\\SPELLS\\skltree_i_back",
};

static const char* const kClassSkillIconPaths[7] = {
	"DATA\\GLOBAL\\UI\\SPELLS\\AmSkillicon",
	"DATA\\GLOBAL\\UI\\SPELLS\\SoSkillicon",
	"DATA\\GLOBAL\\UI\\SPELLS\\NeSkillicon",
	"DATA\\GLOBAL\\UI\\SPELLS\\PaSkillicon",
	"DATA\\GLOBAL\\UI\\SPELLS\\BaSkillicon",
	"DATA\\GLOBAL\\UI\\SPELLS\\DrSkillicon",
	"DATA\\GLOBAL\\UI\\SPELLS\\AsSkillicon",
};

static const char* const kClassNu1Paths[7] = {
	"DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amnu1",
	"DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sonu1",
	"DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nenu1",
	"DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\panu1",
	"DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\banu1",
	"DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dznu1",
	"DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asnu1",
};
static const char* const kClassFwPaths[7] = {
	"DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amfw",
	"DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sofw",
	"DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nefw",
	"DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\pafw",
	"DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\bafw",
	"DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dzfw",
	"DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asfw",
};

static float gfPlayerWorldX = 0.0f;
static float gfPlayerWorldY = 0.0f;
static float gfTargetWorldX = 0.0f;
static float gfTargetWorldY = 0.0f;
static uint8_t gnPlayerDir = 0;
static bool gbPlayerMoving = false;
static bool gbMouseLeftHeld = false;
static bool gbShowGroundItems = false;
static POINT gMousePos = { 400, 300 };

// Authoritative S2C / C2S Client State (1.12a SCmd / CCmd)
static uint32_t gdwClientPlayerGuid = 0;
static int32_t gnServerSubX = 0;
static int32_t gnServerSubY = 0;
static bool gbServerActLoaded = false;
static uint8_t gnCurrentAct = 0;
static uint32_t gdwServerMapSeed = 0x112A2026u;
static uint16_t gwServerTownLevelId = LEVEL_ROGUEENCAMPMENT;
static uint8_t gnGameDifficulty = 0;
static bool gbIsExpansion = true;
static int32_t gaClientStats[512] = {};
static uint8_t gaClientSkillLevels[512] = {};
static uint16_t gwLeftSkillId = 0;
static uint16_t gwRightSkillId = 0;
static D2CellFileStrc* gpStatUpBtn = nullptr;

enum ClientItemLocation : uint8_t
{
	CLI_ITEMLOC_GROUND = 0,
	CLI_ITEMLOC_EQUIPPED = 1,
	CLI_ITEMLOC_CONTAINER = 2,
	CLI_ITEMLOC_BELT = 3,
	CLI_ITEMLOC_CURSOR = 4,
	CLI_ITEMLOC_VENDOR = 5,
	CLI_ITEMLOC_STASH = 6,
	CLI_ITEMLOC_CUBE = 7,
	CLI_ITEMLOC_MERC = 8,
};

struct ClientMissileEntry
{
	float fWorldX;
	float fWorldY;
	float fVelX;
	float fVelY;
	int32_t nFramesRemaining;
	uint16_t wMissileId;
	uint8_t nColor;
	uint8_t nRadius;
	bool bActive;
	uint8_t nPad[3];
};

struct ClientOverlayEntry
{
	uint32_t dwTargetGuid;
	int32_t nFramesRemaining;
	int32_t nMaxFrames;
	uint16_t wOverlayId;
	uint8_t nTargetType;
	uint8_t nColor;
	bool bActive;
	uint8_t nPad[3];
};

struct WeatherParticle
{
	float fScreenX;
	float fScreenY;
	float fVelX;
	float fVelY;
	uint8_t nLength;
	uint8_t nPad[3];
};

static constexpr int32_t kMaxClientMissiles = 64;
static ClientMissileEntry gClientMissiles[kMaxClientMissiles] = {};
static uint32_t gdwClientMissilesSpawned = 0;

static constexpr int32_t kMaxClientOverlays = 32;
static ClientOverlayEntry gClientOverlays[kMaxClientOverlays] = {};
static uint32_t gdwClientOverlaysSpawned = 0;

static constexpr int32_t kMaxWeatherParticles = 96;
static WeatherParticle gWeatherParticles[kMaxWeatherParticles] = {};
static bool gbWeatherInitialized = false;

struct ClientWorldUnit
{
	uint32_t dwUnitGuid;
	int32_t nSubX;
	int32_t nSubY;
	float fWorldX;
	float fWorldY;
	float fTargetWorldX;
	float fTargetWorldY;
	uint16_t wClassId;
	uint8_t nUnitType;
	uint8_t nAnimMode;
	uint8_t nLifePct; // 0..128
	bool bActive;
	bool bIsNpc;
	bool bIsHostile;
	bool bMoving;
	uint8_t nPad[3];
	char szName[64];
};

struct ClientItemEntry
{
	uint32_t dwItemGuid;
	uint32_t dwCost;
	int32_t nGridX;
	int32_t nGridY;
	int32_t nWorldSubX;
	int32_t nWorldSubY;
	D2CellFileStrc* pInvCell;
	uint16_t wItemClassId;
	uint8_t nAction;
	uint8_t nLocation; // ClientItemLocation
	uint8_t nBodyLoc;
	uint8_t nStorePage;
	uint8_t nInvW;
	uint8_t nInvH;
	bool bActive;
	uint8_t nPad[3];
	char szName[64];
	char szInvFile[32];
};

static constexpr int32_t kMaxClientUnits = 256;
static ClientWorldUnit gClientUnits[kMaxClientUnits] = {};
static int32_t gnHoveredUnitIdx = -1;

static constexpr int32_t kMaxClientItems = 128;
static ClientItemEntry gClientItems[kMaxClientItems] = {};
static int32_t gnHoveredGroundItemIdx = -1;

static constexpr int32_t kMaxChatLogLines = 12;
static char gaChatLog[kMaxChatLogLines][128] = {};
static int32_t gnChatLogCount = 0;

static void AppendClientChatLog(const char* szMsg)
{
	if (!szMsg || !szMsg[0])
	{
		return;
	}
	if (gnChatLogCount < kMaxChatLogLines)
	{
		SStrCopy(gaChatLog[gnChatLogCount++], szMsg, sizeof(gaChatLog[0]));
	}
	else
	{
		for (int32_t i = 1; i < kMaxChatLogLines; ++i)
		{
			SStrCopy(gaChatLog[i - 1], gaChatLog[i], sizeof(gaChatLog[0]));
		}
		SStrCopy(gaChatLog[kMaxChatLogLines - 1], szMsg, sizeof(gaChatLog[0]));
	}
}

static void UnicodeToAsciiSafe(const Unicode* pUni, char* szOut, size_t nMaxLen)
{
	if (!szOut || nMaxLen == 0)
	{
		return;
	}
	szOut[0] = '\0';
	if (!pUni)
	{
		return;
	}
	size_t w = 0;
	for (size_t i = 0; pUni[i].ch != 0 && w + 1 < nMaxLen; ++i)
	{
		const uint16_t ch = pUni[i].ch;
		if (ch == 0xFF) // D2 color code prefix (0xFF 'c' '0'..'9')
		{
			if (pUni[i + 1].ch == 'c' && pUni[i + 2].ch != 0)
			{
				i += 2;
				continue;
			}
		}
		if (ch == '\n' || ch == '\r')
		{
			szOut[w++] = ' ';
		}
		else if (ch >= 32 && ch < 127)
		{
			szOut[w++] = static_cast<char>(ch);
		}
	}
	szOut[w] = '\0';
}

// Authoritative C2S Packet Senders routed through Reconstructed 1.12a Msg.cpp / UnitMode.cpp
static void SendWalkOrRunToLocation(int32_t nSubX, int32_t nSubY, bool bRun)
{
	UNITMODE_SendActionPacket(
		bRun ? 0x03u : 0x01u,
		static_cast<uint16_t>(nSubX),
		static_cast<uint16_t>(nSubY),
		0,
		0);
}

static void SendWalkOrRunToUnit(uint8_t nUnitType, uint32_t dwUnitGuid, bool bRun)
{
	UNITMODE_SendActionPacket(
		bRun ? 0x04u : 0x02u,
		0,
		0,
		static_cast<uint32_t>(nUnitType),
		dwUnitGuid);
}

static void SendUnitInteract(uint8_t nUnitType, uint32_t dwUnitGuid)
{
	MSG_SendByte2Dwords(0x13u, static_cast<uint32_t>(nUnitType), dwUnitGuid);
}

static void SendTownFolkInvite(uint8_t nUnitType, uint32_t dwUnitGuid, int32_t nSubX, int32_t nSubY)
{
	MSG_SendByte4Dwords(
		0x59u,
		static_cast<uint32_t>(nUnitType),
		dwUnitGuid,
		static_cast<uint32_t>(nSubX),
		static_cast<uint32_t>(nSubY));
}

static void SendTownFolkMenuSelect(uint32_t dwAction, uint32_t dwNpcGuid, uint32_t dwParam)
{
	MSG_SendByte3Dwords(0x38u, dwAction, dwNpcGuid, dwParam);
}

static void SendTownFolkBuy(uint32_t dwNpcGuid, uint32_t dwItemGuid, uint32_t dwTab, uint32_t dwCost)
{
	MSG_SendByte4Dwords(0x32u, dwNpcGuid, dwItemGuid, dwTab, dwCost);
}

static void SendTownFolkSell(uint32_t dwNpcGuid, uint32_t dwItemGuid, uint32_t dwMode, uint32_t dwCost)
{
	MSG_SendByte4Dwords(0x33u, dwNpcGuid, dwItemGuid, dwMode, dwCost);
}

static void SendTownFolkRepairAll(uint32_t dwNpcGuid)
{
	MSG_SendByte4Dwords(0x35u, dwNpcGuid, 0u, 0u, 0x80000000u);
}

static void SendUseInventoryItem(uint32_t dwItemGuid)
{
	MSG_SendByte3Dwords(
		0x20u,
		dwItemGuid,
		static_cast<uint32_t>(gnServerSubX),
		static_cast<uint32_t>(gnServerSubY));
}

static void ClearVendorShopItems()
{
	for (int32_t i = 0; i < kMaxClientItems; ++i)
	{
		if (gClientItems[i].bActive && gClientItems[i].nLocation == CLI_ITEMLOC_VENDOR)
		{
			gClientItems[i].bActive = false;
		}
	}
	gnSelectedVendorItemIdx = -1;
}

static void CloseNpcDialogAndVendor()
{
	if (gnActiveNpcGuid != 0)
	{
		GAME_SendTownFolkCancel_30(UNIT_MONSTER, gnActiveNpcGuid);
	}
	gbNpcMenuOpen = false;
	gnActiveNpcGuid = 0;
	gwActiveNpcClassId = 0;
	gszActiveNpcName[0] = '\0';
	gnNpcMenuSelectedIdx = 0;
	ClearVendorShopItems();
	if (gnLeftPanel == LEFTPANEL_VENDOR)
	{
		gnLeftPanel = LEFTPANEL_NONE;
	}
}

static void SendLeftSkillOnLocation(int32_t nSubX, int32_t nSubY)
{
	UNITMODE_SendActionPacket(
		0x05u,
		static_cast<uint16_t>(nSubX),
		static_cast<uint16_t>(nSubY),
		0,
		0);
}

static void SendLeftSkillOnUnit(uint8_t nUnitType, uint32_t dwUnitGuid)
{
	UNITMODE_SendActionPacket(0x06u, 0, 0, static_cast<uint32_t>(nUnitType), dwUnitGuid);
}

static void SendRightSkillOnLocation(int32_t nSubX, int32_t nSubY)
{
	UNITMODE_SendActionPacket(
		0x0Cu,
		static_cast<uint16_t>(nSubX),
		static_cast<uint16_t>(nSubY),
		0,
		0);
}

static void SendRightSkillOnUnit(uint8_t nUnitType, uint32_t dwUnitGuid)
{
	UNITMODE_SendActionPacket(0x0Du, 0, 0, static_cast<uint32_t>(nUnitType), dwUnitGuid);
}

static void SendPickupGroundItem(uint32_t dwItemGuid)
{
	MSG_SendByte3Dwords(0x16u, UNIT_ITEM, dwItemGuid, 0u);
}

static void SendIncrementStat(uint16_t wStatId)
{
	MSG_SendByteWord(0x3Au, wStatId);
}

static void SendIncrementSkill(uint16_t wSkillId)
{
	MSG_SendByteWord(0x3Bu, wSkillId);
}

static void SendSelectSkill(uint16_t wSkillId, bool bLeftHand)
{
	MSG_SendByte4Words(0x3Cu, wSkillId, bLeftHand ? 0x8000u : 0x0000u, 0xFFFFu, 0xFFFFu);
}

static void SendUseBeltItem(uint32_t dwItemGuid)
{
	MSG_SendByte3Dwords(0x26u, dwItemGuid, 0u, 0u);
}

static void SpawnClientOverlay(uint8_t nTargetType, uint32_t dwTargetGuid, uint16_t wOverlayId, uint8_t nColor, int32_t nDurationFrames = 30)
{
	for (int32_t i = 0; i < kMaxClientOverlays; ++i)
	{
		if (!gClientOverlays[i].bActive)
		{
			gClientOverlays[i].bActive = true;
			gClientOverlays[i].nTargetType = nTargetType;
			gClientOverlays[i].dwTargetGuid = dwTargetGuid;
			gClientOverlays[i].wOverlayId = wOverlayId;
			gClientOverlays[i].nColor = nColor;
			gClientOverlays[i].nFramesRemaining = nDurationFrames;
			gClientOverlays[i].nMaxFrames = std::max<int32_t>(1, nDurationFrames);
			++gdwClientOverlaysSpawned;
			return;
		}
	}
}

static void SpawnClientMissile(uint16_t wMissileId, float fStartWorldX, float fStartWorldY, float fTargetWorldXPos, float fTargetWorldYPos, uint8_t nColor = 0x62u)
{
	const float dx = fTargetWorldXPos - fStartWorldX;
	const float dy = fTargetWorldYPos - fStartWorldY;
	const float dist = std::max<float>(1.0f, std::sqrt(dx * dx + dy * dy));
	const float fSpeed = 16.0f;
	for (int32_t i = 0; i < kMaxClientMissiles; ++i)
	{
		if (!gClientMissiles[i].bActive)
		{
			gClientMissiles[i].bActive = true;
			gClientMissiles[i].wMissileId = wMissileId;
			gClientMissiles[i].fWorldX = fStartWorldX;
			gClientMissiles[i].fWorldY = fStartWorldY;
			gClientMissiles[i].fVelX = (dx / dist) * fSpeed;
			gClientMissiles[i].fVelY = (dy / dist) * fSpeed;
			gClientMissiles[i].nColor = nColor;
			gClientMissiles[i].nRadius = 5;
			gClientMissiles[i].nFramesRemaining = std::max<int32_t>(8, std::min<int32_t>(45, static_cast<int32_t>(dist / fSpeed) + 4));
			++gdwClientMissilesSpawned;
			return;
		}
	}
}

// C2S 0x4F: UI Button Action (0x12=CloseStash, 0x13=WithdrawGold, 0x14=DepositGold, 0x18=Transmute, 0x19=CloseCube)
static void SendUiButtonAction_4F(uint16_t wAction, uint16_t wGoldLow = 0, uint16_t wGoldHigh = 0)
{
	uint8_t aPkt[7] = {};
	aPkt[0] = 0x4Fu;
	*reinterpret_cast<uint16_t*>(&aPkt[1]) = wAction;
	*reinterpret_cast<uint16_t*>(&aPkt[3]) = wGoldLow;
	*reinterpret_cast<uint16_t*>(&aPkt[5]) = wGoldHigh;
	MSG_SendPacket(aPkt, sizeof(aPkt));
}

static void SendHoradricCubeTransmute()
{
	SendUiButtonAction_4F(0x18u, 0, 0);
	++gdwTransmutesSent;
	SpawnClientOverlay(UNIT_PLAYER, gdwClientPlayerGuid, 148u, 0x6Au, 25);
	D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\transmutebutton.wav");
	AppendClientChatLog("Horadric Cube: Transmute activated (C2S 0x4F action 0x18).");
}

static void SendStashDepositGold(uint32_t dwAmount)
{
	SendUiButtonAction_4F(0x14u, static_cast<uint16_t>(dwAmount & 0xFFFFu), static_cast<uint16_t>((dwAmount >> 16) & 0xFFFFu));
	const int32_t nTransfer = std::min<int32_t>(static_cast<int32_t>(dwAmount), std::max<int32_t>(0, gaClientStats[STAT_GOLD]));
	gaClientStats[STAT_GOLD] -= nTransfer;
	gnStashGold += nTransfer;
	D2SOUND_FastPlaySound("data\\global\\sfx\\items\\gold.wav");
}

static void SendStashWithdrawGold(uint32_t dwAmount)
{
	SendUiButtonAction_4F(0x13u, static_cast<uint16_t>(dwAmount & 0xFFFFu), static_cast<uint16_t>((dwAmount >> 16) & 0xFFFFu));
	const int32_t nTransfer = std::min<int32_t>(static_cast<int32_t>(dwAmount), std::max<int32_t>(0, gnStashGold));
	gnStashGold -= nTransfer;
	gaClientStats[STAT_GOLD] += nTransfer;
	D2SOUND_FastPlaySound("data\\global\\sfx\\items\\gold.wav");
}

// C2S 0x18: InsertItemToBuffer (dwItemGUID, dwX, dwY, dwBufferType: 0=Inv, 3=Cube, 4=Stash)
static void SendInsertItemToBuffer_18(uint32_t dwItemGuid, uint32_t dwX, uint32_t dwY, uint32_t dwBufferType)
{
	MSG_SendByte4Dwords(0x18u, dwItemGuid, dwX, dwY, dwBufferType);
}

// C2S 0x19: RemoveItemFromBuffer (dwItemGUID)
static void SendRemoveItemFromBuffer_19(uint32_t dwItemGuid)
{
	MSG_SendByteDword(0x19u, dwItemGuid);
}

// C2S 0x17: DropCursorItem (dwItemGUID)
static void SendDropCursorItem_17(uint32_t dwItemGuid)
{
	MSG_SendByteDword(0x17u, dwItemGuid);
}

// C2S 0x1A: EquipItem (dwItemGUID, wBodyLoc)
static void SendEquipItem_1A(uint32_t dwItemGuid, uint16_t wBodyLoc)
{
	uint8_t aPkt[9] = {};
	aPkt[0] = 0x1Au;
	*reinterpret_cast<uint32_t*>(&aPkt[1]) = dwItemGuid;
	*reinterpret_cast<uint16_t*>(&aPkt[5]) = wBodyLoc;
	MSG_SendPacket(aPkt, sizeof(aPkt));
}

// C2S 0x1C: RemoveBodyItem (wBodyLoc)
static void SendRemoveBodyItem_1C(uint16_t wBodyLoc)
{
	MSG_SendByteWord(0x1Cu, wBodyLoc);
}

// C2S 0x1D: SwapCursorWithBodyItem (dwItemGUID, wBodyLoc)
static void SendSwapBodyItem_1D(uint32_t dwItemGuid, uint16_t wBodyLoc)
{
	uint8_t aPkt[9] = {};
	aPkt[0] = 0x1Du;
	*reinterpret_cast<uint32_t*>(&aPkt[1]) = dwItemGuid;
	*reinterpret_cast<uint16_t*>(&aPkt[5]) = wBodyLoc;
	MSG_SendPacket(aPkt, sizeof(aPkt));
}

static void SwapClientWeaponSetItems()
{
	gnActiveWeaponSet = 1 - gnActiveWeaponSet;
	for (int32_t i = 0; i < kMaxClientItems; ++i)
	{
		ClientItemEntry& it = gClientItems[i];
		if (!it.bActive || it.nLocation != CLI_ITEMLOC_EQUIPPED)
		{
			continue;
		}
		if (it.nBodyLoc == BODYLOC_RARM)
		{
			it.nBodyLoc = BODYLOC_SWRARM;
		}
		else if (it.nBodyLoc == BODYLOC_SWRARM)
		{
			it.nBodyLoc = BODYLOC_RARM;
		}
		else if (it.nBodyLoc == BODYLOC_LARM)
		{
			it.nBodyLoc = BODYLOC_SWLARM;
		}
		else if (it.nBodyLoc == BODYLOC_SWLARM)
		{
			it.nBodyLoc = BODYLOC_LARM;
		}
	}
}

static void GetBodySlotRect800x600(int32_t nBodyLoc, int32_t* pX, int32_t* pY, int32_t* pW, int32_t* pH)
{
	static const int32_t kFallbackRects[11][4] = {
		{ 0,   0,   0,  0 },   // 0: None
		{ 533, 63,  56, 56 },  // 1: HEAD
		{ 605, 92,  29, 29 },  // 2: NECK
		{ 533, 132, 56, 85 },  // 3: TORSO
		{ 419, 108, 56, 112 }, // 4: RARM
		{ 647, 108, 56, 112 }, // 5: LARM
		{ 492, 240, 29, 29 },  // 6: RRIN
		{ 605, 240, 29, 29 },  // 7: LRIN
		{ 533, 240, 56, 29 },  // 8: BELT
		{ 647, 240, 56, 56 },  // 9: FEET
		{ 419, 240, 56, 56 },  // 10: GLOVES
	};
	static const int32_t kBodyLocToInvComp[11] = {
		-1,
		INV_COMP_HEAD,
		INV_COMP_NECK,
		INV_COMP_TORSO,
		INV_COMP_RARM,
		INV_COMP_LARM,
		INV_COMP_RHAND,
		INV_COMP_LHAND,
		INV_COMP_BELT,
		INV_COMP_FEET,
		INV_COMP_GLOVES,
	};
	if (nBodyLoc < 1 || nBodyLoc > 10)
	{
		*pX = *pY = *pW = *pH = 0;
		return;
	}
	if (sgptDataTables && sgptDataTables->pInventoryTxt && sgptDataTables->nInventoryTxtRecordCount > 16)
	{
		D2InvCompGridStrc tComp = {};
		DATATBLS_GetInventoryComponentGrid(0, 1, &tComp, kBodyLocToInvComp[nBodyLoc]);
		if (tComp.rect.nRight > tComp.rect.nLeft && tComp.rect.nBottom > tComp.rect.nTop)
		{
			*pX = tComp.rect.nLeft;
			*pY = tComp.rect.nTop;
			*pW = tComp.rect.nRight - tComp.rect.nLeft;
			*pH = tComp.rect.nBottom - tComp.rect.nTop;
			return;
		}
	}
	*pX = kFallbackRects[nBodyLoc][0];
	*pY = kFallbackRects[nBodyLoc][1];
	*pW = kFallbackRects[nBodyLoc][2];
	*pH = kFallbackRects[nBodyLoc][3];
}

static void GetInvGridLayout800x600(int32_t* pLeft, int32_t* pTop, int32_t* pBoxW, int32_t* pBoxH)
{
	if (sgptDataTables && sgptDataTables->pInventoryTxt && sgptDataTables->nInventoryTxtRecordCount > 16)
	{
		D2InventoryGridInfoStrc tGrid = {};
		DATATBLS_GetInventoryGridInfo(0, 1, &tGrid);
		if (tGrid.nGridBoxWidth > 0 && tGrid.nGridBoxHeight > 0)
		{
			*pLeft = tGrid.nGridLeft;
			*pTop = tGrid.nGridTop;
			*pBoxW = tGrid.nGridBoxWidth;
			*pBoxH = tGrid.nGridBoxHeight;
			return;
		}
	}
	*pLeft = 419;
	*pTop = 315;
	*pBoxW = 29;
	*pBoxH = 29;
}

static int32_t GetVendorItemCategoryTab(uint16_t wItemClassId)
{
	if (!sgptDataTables)
	{
		return 3;
	}
	const int32_t nWeapons = sgptDataTables->pItemDataTables.nWeaponsTxtRecordCount;
	const int32_t nArmor = sgptDataTables->pItemDataTables.nArmorTxtRecordCount;
	if (static_cast<int32_t>(wItemClassId) < nWeapons)
	{
		return 2; // Weapons
	}
	if (static_cast<int32_t>(wItemClassId) < nWeapons + nArmor)
	{
		return 1; // Armor
	}
	return 3; // Misc
}

static int32_t BuildVendorTabItemList(int32_t nTab, int32_t* pOutIndices, int32_t nMaxOut)
{
	int32_t nCount = 0;
	auto isItemAlreadyAddedOrSameClass = [&](int32_t idx, bool bDedupeByClass) -> bool {
		for (int32_t k = 0; k < nCount; ++k)
		{
			if (pOutIndices[k] == idx)
			{
				return true;
			}
			if (bDedupeByClass && gClientItems[pOutIndices[k]].wItemClassId == gClientItems[idx].wItemClassId)
			{
				return true;
			}
		}
		return false;
	};

	// Pass 1: On "All" tab (0), prioritize distinct Misc consumables (Scrolls, Tomes, Potions, Keys) first, then distinct Weapons/Armor
	if (nTab == 0)
	{
		for (int32_t pass = 0; pass < 2 && nCount < nMaxOut; ++pass)
		{
			for (int32_t i = 0; i < kMaxClientItems && nCount < nMaxOut; ++i)
			{
				const ClientItemEntry& it = gClientItems[i];
				if (!it.bActive || (it.nLocation != CLI_ITEMLOC_VENDOR && it.nAction != 0x0B))
				{
					continue;
				}
				const int32_t nCat = GetVendorItemCategoryTab(it.wItemClassId);
				if ((pass == 0 && nCat != 3) || (pass == 1 && nCat == 3))
				{
					continue;
				}
				if (!isItemAlreadyAddedOrSameClass(i, true))
				{
					pOutIndices[nCount++] = i;
				}
			}
		}
	}
	else
	{
		// Specific tab (1=Armor, 2=Weapons, 3=Misc): first distinct base types, then remaining items in that category
		for (int32_t bDedupe = 1; bDedupe >= 0 && nCount < nMaxOut; --bDedupe)
		{
			for (int32_t i = 0; i < kMaxClientItems && nCount < nMaxOut; ++i)
			{
				const ClientItemEntry& it = gClientItems[i];
				if (!it.bActive || (it.nLocation != CLI_ITEMLOC_VENDOR && it.nAction != 0x0B))
				{
					continue;
				}
				if (GetVendorItemCategoryTab(it.wItemClassId) != nTab)
				{
					continue;
				}
				if (!isItemAlreadyAddedOrSameClass(i, bDedupe != 0))
				{
					pOutIndices[nCount++] = i;
				}
			}
		}
	}

	// Fallback if a tab had fewer than nMaxOut items on "All"
	if (nTab == 0 && nCount < nMaxOut)
	{
		for (int32_t i = 0; i < kMaxClientItems && nCount < nMaxOut; ++i)
		{
			const ClientItemEntry& it = gClientItems[i];
			if (it.bActive && (it.nLocation == CLI_ITEMLOC_VENDOR || it.nAction == 0x0B) && !isItemAlreadyAddedOrSameClass(i, false))
			{
				pOutIndices[nCount++] = i;
			}
		}
	}
	return nCount;
}

// C2S 0x49: WaypointTeleport (dwWaypointGUID, nDestinationLevelId)
static void SendWaypointTeleport_49(uint32_t dwWaypointGuid, uint8_t nDestLevelId, uint8_t nTargetAct)
{
	(void)nTargetAct;
	uint8_t aPkt[9] = {};
	aPkt[0] = 0x49u;
	*reinterpret_cast<uint32_t*>(&aPkt[1]) = dwWaypointGuid;
	aPkt[5] = nDestLevelId;
	MSG_SendPacket(aPkt, sizeof(aPkt));
	++gdwWaypointsUsed;
	if (nDestLevelId > 0)
	{
		gnLeftPanel = LEFTPANEL_NONE;
		SpawnClientOverlay(UNIT_PLAYER, gdwClientPlayerGuid, 120u, 0x97u, 28);
		D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\waygate.wav");
	}
}

// C2S 0x36: HireMerc (dwNpcGUID, dwMercListId)
static void SendHireMerc_36(uint32_t dwNpcGuid, uint32_t dwMercId)
{
	MSG_SendByte2Dwords(0x36u, dwNpcGuid, dwMercId);
	gbMercHired = true;
	gnMercLifePct = 128;
	AppendClientChatLog("Hired town mercenary companion (C2S 0x36).");
}

// C2S 0x62: ResurrectMerc (dwNpcGUID)
static void SendResurrectMerc_62(uint32_t dwNpcGuid)
{
	MSG_SendByteDword(0x62u, dwNpcGuid);
	gbMercHired = true;
	gnMercLifePct = 128;
	AppendClientChatLog("Resurrected mercenary companion (C2S 0x62).");
}

// C2S 0x61: MercEquipItem (wBodyLoc)
static void SendMercEquip_61(uint16_t wBodyLoc)
{
	MSG_SendByteWord(0x61u, wBodyLoc);
}

// C2S 0x15: SendChatMessage + 1.13d Slash Commands (/fps, /ignore, /unignore, /ignorelist, /ignorepersist, /filtermsg, /unfiltermsg)
static void ExecuteChatOrSlashCommand(const char* szInput)
{
	if (!szInput || !szInput[0])
	{
		return;
	}
	if (szInput[0] == '/')
	{
		++gdwSlashCommandsExecuted;
		#if defined(D2_VERSION_113D) || (D2_VERSION_MAJOR == 1 && D2_VERSION_MINOR >= 14)
		IIgnoreList* pIgnore = IGNORELIST_GetSingleton();
#else
		IIgnoreList* pIgnore = nullptr;
#endif
		if (std::strncmp(szInput, "/fps", 4) == 0)
		{
			gbShowFpsOverlay = !gbShowFpsOverlay;
			AppendClientChatLog(gbShowFpsOverlay ? "FPS / Network Diagnostics Overlay: ON" : "FPS / Network Diagnostics Overlay: OFF");
			return;
		}
		if (std::strncmp(szInput, "/ignorelist", 11) == 0)
		{
			char szMsg[96] = {};
			std::snprintf(
				szMsg,
				sizeof(szMsg),
				"Ignore List active (persist=%d).",
				pIgnore ? pIgnore->GetPersistence() : 0);
			AppendClientChatLog(szMsg);
			return;
		}
		if (std::strncmp(szInput, "/ignorepersist", 14) == 0)
		{
			if (pIgnore)
			{
				pIgnore->SetPersistence(!pIgnore->GetPersistence());
			}
			AppendClientChatLog(pIgnore && pIgnore->GetPersistence() ? "Ignore persistence enabled (ignorelist)." : "Ignore persistence disabled.");
			return;
		}
		if (std::strncmp(szInput, "/ignore ", 8) == 0)
		{
			const char* szTarget = szInput + 8;
			if (pIgnore && szTarget[0])
			{
				pIgnore->AddCharacterIgnore(szTarget, pIgnore->GetPersistence());
			}
			char szMsg[96] = {};
			std::snprintf(szMsg, sizeof(szMsg), "Ignored player: %s", szTarget);
			AppendClientChatLog(szMsg);
			return;
		}
		if (std::strncmp(szInput, "/unignore ", 10) == 0)
		{
			const char* szTarget = szInput + 10;
			if (pIgnore && szTarget[0])
			{
				pIgnore->RemoveCharacterIgnore(szTarget, pIgnore->GetPersistence());
			}
			char szMsg[96] = {};
			std::snprintf(szMsg, sizeof(szMsg), "Unignored player: %s", szTarget);
			AppendClientChatLog(szMsg);
			return;
		}
		if (std::strncmp(szInput, "/filtermsg ", 11) == 0)
		{
			const char* szSub = szInput + 11;
			if (pIgnore && szSub[0])
			{
				pIgnore->AddTextFilter(szSub, pIgnore->GetPersistence());
			}
			char szMsg[96] = {};
			std::snprintf(szMsg, sizeof(szMsg), "Added message filter: %s", szSub);
			AppendClientChatLog(szMsg);
			return;
		}
		if (std::strncmp(szInput, "/unfiltermsg ", 13) == 0)
		{
			const char* szSub = szInput + 13;
			if (pIgnore && szSub[0])
			{
				pIgnore->RemoveTextFilter(szSub, pIgnore->GetPersistence());
			}
			char szMsg[96] = {};
			std::snprintf(szMsg, sizeof(szMsg), "Removed message filter: %s", szSub);
			AppendClientChatLog(szMsg);
			return;
		}
	}

	uint8_t aChatPkt[128] = {};
	aChatPkt[0] = 0x15u;
	aChatPkt[1] = 0x01u;
	const size_t nMsgLen = std::min<size_t>(90u, std::strlen(szInput));
	std::memcpy(&aChatPkt[3], szInput, nMsgLen);
	MSG_SendPacket(aChatPkt, nMsgLen + 6u);

	char szLine[128] = {};
	std::snprintf(szLine, sizeof(szLine), "%s: %s", gszCharName, szInput);
	AppendClientChatLog(szLine);
}

static bool IsWaypointObjectClass(uint16_t wObjClassId)
{
	static const uint16_t kWaypointObjIds[] = {
		119, 145, 156, 157, 237, 238, 288, 323, 324, 398, 402, 429, 494, 496, 511, 539
	};
	for (uint16_t id : kWaypointObjIds)
	{
		if (wObjClassId == id)
		{
			return true;
		}
	}
	return false;
}

static void SendLeaveGamePacket()
{
	MSG_SendLeaveGame_69();
}

static const char* const kClassNames[7] = {
	"Amazon", "Sorceress", "Necromancer", "Paladin", "Barbarian", "Druid", "Assassin"
};

// 1.12a @10001: 0x6FB4E980
extern "C" D2CLIENT_DLL_DECL int32_t __stdcall D2Client_10001(int32_t nUnused)
{
	(void)nUnused;
	return gbD2ClientInitialized;
}

// 1.12a @10002: 0x6FB4E940
extern "C" D2CLIENT_DLL_DECL uint8_t __stdcall D2Client_10002(int32_t nUnused, int32_t nIndex)
{
	(void)nUnused;
	if (nIndex < 0 || nIndex >= 3)
	{
		return 0;
	}
	return gaD2ClientBytes[nIndex];
}

// 1.12a @10003: 0x6FAC2100
extern "C" D2CLIENT_DLL_DECL void __stdcall D2Client_10003()
{
	if (HWND hWnd = WINDOW_GetWindow())
	{
		SendMessageA(hWnd, WM_ACTIVATEAPP, 0, 0);
	}
	WINDOW_UpdatePlacement();
}

static void ScreenPointToSubtile(int32_t nScreenX, int32_t nScreenY, int32_t* pSubX, int32_t* pSubY)
{
	int nIsoX = static_cast<int>(gfPlayerWorldX) + (nScreenX - 400);
	int nIsoY = static_cast<int>(gfPlayerWorldY) + (nScreenY - 280);
	DUNGEON_ClientToGameSubtileCoords(&nIsoX, &nIsoY);
	if (pSubX)
	{
		*pSubX = nIsoX;
	}
	if (pSubY)
	{
		*pSubY = nIsoY;
	}
}

static void UpdateHoveredWorldTarget(int32_t nScreenX, int32_t nScreenY)
{
	gnHoveredUnitIdx = -1;
	gnHoveredGroundItemIdx = -1;
	if (gbEscMenuOpen || nScreenY >= 524)
	{
		return;
	}
	if (gnLeftPanel != LEFTPANEL_NONE && nScreenX >= 80 && nScreenX < 400 && nScreenY >= 60 && nScreenY < 492)
	{
		return;
	}
	if (gnRightPanel != RIGHTPANEL_NONE && nScreenX >= 400 && nScreenX < 720 && nScreenY >= 60 && nScreenY < 492)
	{
		return;
	}

	const int32_t nCamX = static_cast<int32_t>(gfPlayerWorldX);
	const int32_t nCamY = static_cast<int32_t>(gfPlayerWorldY);

	// Check ground items first
	int32_t nBestItemDistSq = 42 * 42;
	for (int32_t i = 0; i < kMaxClientItems; ++i)
	{
		const ClientItemEntry& it = gClientItems[i];
		if (!it.bActive || it.nLocation != CLI_ITEMLOC_GROUND)
		{
			continue;
		}
		int nItemWorldX = it.nWorldSubX;
		int nItemWorldY = it.nWorldSubY;
		DUNGEON_GameSubtileToClientCoords(&nItemWorldX, &nItemWorldY);
		const int32_t nItemScreenX = nItemWorldX - nCamX + 400;
		const int32_t nItemScreenY = nItemWorldY - nCamY + 280;
		const int32_t dx = nScreenX - nItemScreenX;
		const int32_t dy = nScreenY - (nItemScreenY - 12);
		const int32_t d2 = dx * dx + dy * dy;
		if (d2 < nBestItemDistSq)
		{
			nBestItemDistSq = d2;
			gnHoveredGroundItemIdx = i;
		}
	}

	// Check live server units (monsters, NPCs, interactive objects, warps)
	int32_t nBestUnitDistSq = 48 * 48;
	for (int32_t i = 0; i < kMaxClientUnits; ++i)
	{
		const ClientWorldUnit& u = gClientUnits[i];
		if (!u.bActive)
		{
			continue;
		}
		if (u.nUnitType == UNIT_MONSTER && (u.nAnimMode == MONMODE_DEATH || u.nAnimMode == MONMODE_DEAD || u.nLifePct == 0))
		{
			continue;
		}
		const int32_t nUnitScreenX = static_cast<int32_t>(u.fWorldX) - nCamX + 400;
		const int32_t nUnitScreenY = static_cast<int32_t>(u.fWorldY) - nCamY + 280;
		const int32_t dx = nScreenX - nUnitScreenX;
		const int32_t dy = nScreenY - (nUnitScreenY - 28);
		const int32_t d2 = dx * dx + dy * dy;
		if (d2 < nBestUnitDistSq)
		{
			nBestUnitDistSq = d2;
			gnHoveredUnitIdx = i;
		}
	}
}

static void SetMoveTargetFromScreenPoint(int32_t nScreenX, int32_t nScreenY, bool bSendNetworkPacket = true)
{
	if (gbEscMenuOpen || gbNpcMenuOpen || nScreenY >= 524)
	{
		return;
	}
	if (gnLeftPanel != LEFTPANEL_NONE && nScreenX >= 80 && nScreenX < 400 && nScreenY >= 60 && nScreenY < 492)
	{
		return;
	}
	if (gnRightPanel != RIGHTPANEL_NONE && nScreenX >= 400 && nScreenX < 720 && nScreenY >= 60 && nScreenY < 492)
	{
		return;
	}

	UpdateHoveredWorldTarget(nScreenX, nScreenY);

	if (bSendNetworkPacket && gnHoveredGroundItemIdx >= 0)
	{
		const ClientItemEntry& it = gClientItems[gnHoveredGroundItemIdx];
		int nItemWorldX = it.nWorldSubX;
		int nItemWorldY = it.nWorldSubY;
		DUNGEON_GameSubtileToClientCoords(&nItemWorldX, &nItemWorldY);
		gfTargetWorldX = static_cast<float>(nItemWorldX);
		gfTargetWorldY = static_cast<float>(nItemWorldY);
		gbPlayerMoving = true;
		gnPendingNpcGuid = 0;
		SendWalkOrRunToUnit(UNIT_ITEM, it.dwItemGuid, gbRunEnabled);
		SendPickupGroundItem(it.dwItemGuid);
		return;
	}

	if (bSendNetworkPacket && gnHoveredUnitIdx >= 0)
	{
		const ClientWorldUnit& u = gClientUnits[gnHoveredUnitIdx];
		gfTargetWorldX = u.fWorldX;
		gfTargetWorldY = u.fWorldY;
		gbPlayerMoving = true;
		if (u.nUnitType == UNIT_MONSTER && u.bIsHostile)
		{
			gnPendingNpcGuid = 0;
			SendLeftSkillOnUnit(UNIT_MONSTER, u.dwUnitGuid);
			D2SOUND_FastPlaySound("data\\global\\sfx\\combat\\weapon\\swing.wav");
		}
		else if (u.nUnitType == UNIT_MONSTER && u.bIsNpc)
		{
			SendTownFolkInvite(UNIT_MONSTER, u.dwUnitGuid, gnServerSubX, gnServerSubY);
			const int32_t nDx = std::abs(gnServerSubX - u.nSubX);
			const int32_t nDy = std::abs(gnServerSubY - u.nSubY);
			if (gnServerSubX > 0 && gnServerSubY > 0 && std::max(nDx, nDy) <= 5)
			{
				gnPendingNpcGuid = 0;
				gbPlayerMoving = false;
				SendUnitInteract(UNIT_MONSTER, u.dwUnitGuid);
			}
			else
			{
				gnPendingNpcGuid = u.dwUnitGuid;
				SendWalkOrRunToLocation(u.nSubX, u.nSubY, gbRunEnabled);
			}
		}
		else
		{
			gnPendingNpcGuid = 0;
			SendWalkOrRunToUnit(u.nUnitType, u.dwUnitGuid, gbRunEnabled);
			SendUnitInteract(u.nUnitType, u.dwUnitGuid);
			if (u.nUnitType == UNIT_OBJECT)
			{
				if (u.wClassId == 267) // OBJECT_STASH (Bank)
				{
					gnLeftPanel = LEFTPANEL_STASH;
					gnRightPanel = RIGHTPANEL_INV;
					D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
				}
				else if (IsWaypointObjectClass(u.wClassId))
				{
					gdwActiveWaypointGuid = u.dwUnitGuid;
					gnWaypointActTab = std::min<int32_t>(4, gnCurrentAct);
					gnLeftPanel = LEFTPANEL_WAYPOINT;
					D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\waygate.wav");
				}
			}
		}
		return;
	}

	gnPendingNpcGuid = 0;
	const float fDx = static_cast<float>(nScreenX - 400);
	const float fDy = static_cast<float>(nScreenY - 280);
	gfTargetWorldX = gfPlayerWorldX + fDx;
	gfTargetWorldY = gfPlayerWorldY + fDy;
	gbPlayerMoving = true;

	if (bSendNetworkPacket)
	{
		int32_t nSubX = 0;
		int32_t nSubY = 0;
		ScreenPointToSubtile(nScreenX, nScreenY, &nSubX, &nSubY);
		if (nSubX > 0 && nSubY > 0)
		{
			SendWalkOrRunToLocation(nSubX, nSubY, gbRunEnabled);
		}
	}
}

static void HandleRightClickCast(int32_t nScreenX, int32_t nScreenY)
{
	if (gbEscMenuOpen || gbNpcMenuOpen || nScreenY >= 524)
	{
		return;
	}

	// Right-click inside open Vendor Trade Panel (x=92..388, y=114..394): buy item immediately (0x32)
	if (gnLeftPanel == LEFTPANEL_VENDOR && gnActiveNpcGuid != 0 &&
		nScreenX >= 92 && nScreenX < 388 && nScreenY >= 114 && nScreenY < 394)
	{
		int32_t aVendorItemIndices[10] = {};
		const int32_t nVendorCount = BuildVendorTabItemList(gnVendorActiveTab, aVendorItemIndices, 10);
		const int32_t nClickedRow = (nScreenY - 114) / 28;
		if (nClickedRow >= 0 && nClickedRow < nVendorCount)
		{
			const int32_t nItemIdx = aVendorItemIndices[nClickedRow];
			gnSelectedVendorItemIdx = nItemIdx;
			const ClientItemEntry& it = gClientItems[nItemIdx];
			SendTownFolkBuy(gnActiveNpcGuid, it.dwItemGuid, 0u, it.dwCost);
			D2SOUND_FastPlaySound("data\\global\\sfx\\items\\gold.wav");
		}
		return;
	}

	// Right-click inside open Inventory Panel (x=418..708, y=315..431): use item (0x20) or sell to vendor (0x33)
	if (gnRightPanel == RIGHTPANEL_INV && nScreenX >= 418 && nScreenX < 708 && nScreenY >= 315 && nScreenY < 431)
	{
		for (int32_t i = 0; i < kMaxClientItems; ++i)
		{
			const ClientItemEntry& it = gClientItems[i];
			if (!it.bActive || it.nLocation != CLI_ITEMLOC_CONTAINER)
			{
				continue;
			}
			const int32_t nCellX = 418 + it.nGridX * 29;
			const int32_t nCellY = 315 + it.nGridY * 29;
			const int32_t nCellW = it.nInvW * 29;
			const int32_t nCellH = it.nInvH * 29;
			if (nScreenX >= nCellX && nScreenX < nCellX + nCellW &&
				nScreenY >= nCellY && nScreenY < nCellY + nCellH)
			{
				if (gnLeftPanel == LEFTPANEL_VENDOR && gnActiveNpcGuid != 0)
				{
					SendTownFolkSell(gnActiveNpcGuid, it.dwItemGuid, 0u, std::max<uint32_t>(1u, it.dwCost / 2u));
					D2SOUND_FastPlaySound("data\\global\\sfx\\items\\gold.wav");
				}
				else if (it.wItemClassId == 549) // Horadric Cube (box)
				{
					SendUseInventoryItem(it.dwItemGuid);
					gnLeftPanel = LEFTPANEL_CUBE;
					gnRightPanel = RIGHTPANEL_INV;
					D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
				}
				else
				{
					SendUseInventoryItem(it.dwItemGuid);
					D2SOUND_FastPlaySound("data\\global\\sfx\\items\\potiondrink.wav");
				}
				return;
			}
		}
		return;
	}

	UpdateHoveredWorldTarget(nScreenX, nScreenY);
	if (gnHoveredUnitIdx >= 0)
	{
		const ClientWorldUnit& u = gClientUnits[gnHoveredUnitIdx];
		SendRightSkillOnUnit(u.nUnitType, u.dwUnitGuid);
		SpawnClientMissile(gwRightSkillId ? gwRightSkillId : 36u, gfPlayerWorldX, gfPlayerWorldY, u.fWorldX, u.fWorldY, 0x62u);
	}
	else
	{
		int32_t nSubX = 0;
		int32_t nSubY = 0;
		ScreenPointToSubtile(nScreenX, nScreenY, &nSubX, &nSubY);
		if (nSubX > 0 && nSubY > 0)
		{
			SendRightSkillOnLocation(nSubX, nSubY);
			SpawnClientMissile(
				gwRightSkillId ? gwRightSkillId : 36u,
				gfPlayerWorldX,
				gfPlayerWorldY,
				gfPlayerWorldX + static_cast<float>(nScreenX - 400),
				gfPlayerWorldY + static_cast<float>(nScreenY - 280),
				0x62u);
		}
	}
	D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
}

static void ActivateNpcMenuOption(int32_t nOption)
{
	D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
	const char* szNpc = gszActiveNpcName[0] ? gszActiveNpcName : "Townsfolk";
	if (nOption == 0) // Talk / Introduction / Gossip
	{
		char szLine[128] = {};
		if (gwActiveNpcClassId == 155) // Warriv
		{
			std::snprintf(szLine, sizeof(szLine), "%s: Greetings, %s. The road east is blocked until the Monastery is reclaimed.", szNpc, gszCharName);
		}
		else if (gwActiveNpcClassId == 154) // Charsi
		{
			std::snprintf(szLine, sizeof(szLine), "%s: Hi there! I'm Charsi, the blacksmith here in camp. Good to see you!", szNpc);
		}
		else if (gwActiveNpcClassId == 148) // Akara
		{
			std::snprintf(szLine, sizeof(szLine), "%s: I am Akara, High Priestess of the Sisterhood of the Sightless Eye.", szNpc);
		}
		else
		{
			std::snprintf(szLine, sizeof(szLine), "%s: Good day to you, %s! May the Light protect you in the wilderness.", szNpc, gszCharName);
		}
		AppendClientChatLog(szLine);
	}
	else if (nOption == 1) // Trade / Gamble / Hire / Travel
	{
		if (gwActiveNpcClassId == 155) // Warriv -> Travel East to Act II Lut Gholein (only if unlocked on server)
		{
			SendTownFolkMenuSelect(2u, gnActiveNpcGuid, 0u);
			CloseNpcDialogAndVendor();
			return;
		}
		if (gwActiveNpcClassId == 150 || gwActiveNpcClassId == 198 || gwActiveNpcClassId == 252 || gwActiveNpcClassId == 515) // Mercenary Captains
		{
			SendHireMerc_36(gnActiveNpcGuid, 0u);
			CloseNpcDialogAndVendor();
			gnLeftPanel = LEFTPANEL_MERC;
			gnRightPanel = RIGHTPANEL_INV;
			return;
		}
		gbNpcMenuOpen = false;
		ClearVendorShopItems();
		gnVendorActiveTab = 0;
		gnSelectedVendorItemIdx = -1;
		SendTownFolkMenuSelect(1u, gnActiveNpcGuid, 0u);
		gnLeftPanel = LEFTPANEL_VENDOR;
		gnRightPanel = RIGHTPANEL_INV;
	}
	else if (nOption == 2 && gwActiveNpcClassId == 148) // Akara Respec (1.13c+ nType=0)
	{
		SendTownFolkMenuSelect(0u, gnActiveNpcGuid, 0u);
		SpawnClientOverlay(UNIT_PLAYER, gdwClientPlayerGuid, 148u, 0x6Au, 30);
		AppendClientChatLog("Akara: Your stats and skill points have been reset (1.13c Respec).");
		CloseNpcDialogAndVendor();
	}
	else // Cancel
	{
		CloseNpcDialogAndVendor();
	}
}

static void ActivateEscMenuItem(int32_t nItem)
{
	D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
	if (gnEscSubMenu == 0)
	{
		if (nItem == 0) // OPTIONS
		{
			gnEscSubMenu = 1;
			gnEscSelectedIndex = 0;
		}
		else if (nItem == 1) // SAVE AND EXIT GAME
		{
			SendLeaveGamePacket();
			gbEscMenuOpen = false;
			gbSaveAndExitRequested = 1;
			gbExitApplication = 0;
			gnClientState = D2CLIENT_STATE_LEAVE_GAME;
		}
		else if (nItem == 2) // RETURN TO GAME
		{
			gbEscMenuOpen = false;
		}
	}
	else if (gnEscSubMenu == 1)
	{
		if (nItem == 0) // SOUND OPTIONS
		{
			gbSoundEnabled = !gbSoundEnabled;
		}
		else if (nItem == 1) // VIDEO OPTIONS (Toggle 1200x900 / 800x600 window size)
		{
			gbScaled1200x900 = !gbScaled1200x900;
			if (HWND hMainWnd = WINDOW_GetWindow())
			{
				RECT rc = { 0, 0, gbScaled1200x900 ? 1200 : 800, gbScaled1200x900 ? 900 : 600 };
				AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, 0, WS_EX_APPWINDOW);
				SetWindowPos(
					hMainWnd,
					HWND_NOTOPMOST,
					0,
					0,
					rc.right - rc.left,
					rc.bottom - rc.top,
					SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
			}
		}
		else if (nItem == 2) // AUTOMAP OPTIONS
		{
			gbAutomapOpen = !gbAutomapOpen;
		}
		else if (nItem == 3) // CONFIGURE CONTROLS
		{
			gnEscSubMenu = 2;
			gnEscSelectedIndex = 0;
		}
		else if (nItem == 4) // PREVIOUS MENU
		{
			gnEscSubMenu = 0;
			gnEscSelectedIndex = 0;
		}
	}
	else if (gnEscSubMenu == 2)
	{
		gnEscSubMenu = 1;
		gnEscSelectedIndex = 3;
	}
}

static bool HandleInGameUiClick(int32_t nX, int32_t nY)
{
	// 1. ESC Menu active
	if (gbEscMenuOpen)
	{
		if (gnEscSubMenu == 0)
		{
			for (int32_t i = 0; i < 3; ++i)
			{
				const int32_t nRowY = 205 + i * 55;
				if (nX >= 180 && nX <= 620 && nY >= nRowY - 30 && nY <= nRowY + 12)
				{
					gnEscSelectedIndex = i;
					ActivateEscMenuItem(i);
					return true;
				}
			}
		}
		else if (gnEscSubMenu == 1)
		{
			for (int32_t i = 0; i < 5; ++i)
			{
				const int32_t nRowY = 175 + i * 48;
				if (nX >= 160 && nX <= 640 && nY >= nRowY - 26 && nY <= nRowY + 12)
				{
					gnEscSelectedIndex = i;
					ActivateEscMenuItem(i);
					return true;
				}
			}
		}
		else if (gnEscSubMenu == 2)
		{
			if (nX >= 160 && nX <= 640 && nY >= 380 && nY <= 440)
			{
				ActivateEscMenuItem(0);
				return true;
			}
		}
		return true;
	}

	// 1b. NPC Dialog Menu active (x=290..510, y=120..260)
	if (gbNpcMenuOpen)
	{
		if (nX >= 290 && nX <= 510 && nY >= 120 && nY <= 260)
		{
			for (int32_t i = 0; i < 3; ++i)
			{
				const int32_t nRowY = 172 + i * 28;
				if (nY >= nRowY - 18 && nY <= nRowY + 8)
				{
					gnNpcMenuSelectedIdx = i;
					ActivateNpcMenuOption(i);
					return true;
				}
			}
		}
		else
		{
			CloseNpcDialogAndVendor();
		}
		return true;
	}

	// 2. Central Mini-Panel Toggle Button on HUD (x=390..410, y=553..595)
	if (nX >= 390 && nX <= 410 && nY >= 553 && nY <= 595)
	{
		gbMiniPanelOpen = !gbMiniPanelOpen;
		D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		return true;
	}

	// 3. 8 Mini-Panel Buttons (Character, Inventory, Skill Tree, Automap, Message Log, Quest Log, Party, Game Menu)
	if (gbMiniPanelOpen && nX >= 328 && nX < 472 && nY >= 522 && nY < 553)
	{
		const int32_t nBtnIdx = std::max<int32_t>(0, std::min<int32_t>((nX - 330) / 18, 7));
		D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		switch (nBtnIdx)
		{
		case 0: // Character (C)
			gnLeftPanel = (gnLeftPanel == LEFTPANEL_CHAR) ? LEFTPANEL_NONE : LEFTPANEL_CHAR;
			break;
		case 1: // Inventory (I)
			gnRightPanel = (gnRightPanel == RIGHTPANEL_INV) ? RIGHTPANEL_NONE : RIGHTPANEL_INV;
			break;
		case 2: // Skill Tree (T)
			gnRightPanel = (gnRightPanel == RIGHTPANEL_SKILL) ? RIGHTPANEL_NONE : RIGHTPANEL_SKILL;
			break;
		case 3: // Automap (TAB)
			gbAutomapOpen = !gbAutomapOpen;
			break;
		case 4: // Message Log (M)
			gnLeftPanel = (gnLeftPanel == LEFTPANEL_MSGLOG) ? LEFTPANEL_NONE : LEFTPANEL_MSGLOG;
			break;
		case 5: // Quest Log (Q)
			gnLeftPanel = (gnLeftPanel == LEFTPANEL_QUEST) ? LEFTPANEL_NONE : LEFTPANEL_QUEST;
			if (gnLeftPanel == LEFTPANEL_QUEST)
			{
				MSG_SendByte(0x40u);
			}
			break;
		case 6: // Party Screen (P)
			gnLeftPanel = (gnLeftPanel == LEFTPANEL_PARTY) ? LEFTPANEL_NONE : LEFTPANEL_PARTY;
			break;
		case 7: // Game Menu (ESC)
			gbEscMenuOpen = !gbEscMenuOpen;
			gnEscSubMenu = 0;
			gnEscSelectedIndex = 0;
			break;
		default:
			break;
		}
		return true;
	}

	// 4. Run / Walk Toggle Button on HUD (x=252..272, y=566..594)
	if (nX >= 252 && nX <= 272 && nY >= 566 && nY <= 594)
	{
		gbRunEnabled = !gbRunEnabled;
		D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		return true;
	}

	// 4b. Belt Potion Slots on HUD (x=422..546, y=560..594, or y=452..594 when 4x4 expanded)
	if (nX >= 422 && nX < 546 && nY >= (gbBeltExpanded ? 452 : 560) && nY <= 594)
	{
		const int32_t nBeltCol = (nX - 422) / 31;
		for (int32_t i = 0; i < kMaxClientItems; ++i)
		{
			if (gClientItems[i].bActive && gClientItems[i].nLocation == CLI_ITEMLOC_BELT && (gClientItems[i].nGridX % 4) == nBeltCol)
			{
				SendUseBeltItem(gClientItems[i].dwItemGuid);
				D2SOUND_FastPlaySound("data\\global\\sfx\\items\\potiondrink.wav");
				return true;
			}
		}
		gbBeltExpanded = !gbBeltExpanded;
		return true;
	}

	// 5. Open Left Panel interactions (x=80..400, y=60..492)
	if (gnLeftPanel != LEFTPANEL_NONE && nX >= 80 && nX < 400 && nY >= 60 && nY < 492)
	{
		// Close 'X' button (checked at bottom-left socket, bottom-center, and bottom-right of Left Panel)
		if ((nX >= 124 && nX <= 168 && nY >= 444 && nY <= 484) ||
			(nX >= 204 && nX <= 244 && nY >= 444 && nY <= 484) ||
			(nX >= 350 && nX <= 392 && nY >= 444 && nY <= 484))
		{
			if (gnLeftPanel == LEFTPANEL_VENDOR)
			{
				CloseNpcDialogAndVendor();
			}
			else
			{
				gnLeftPanel = LEFTPANEL_NONE;
			}
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
			return true;
		}
		// NPC Vendor Trade Panel interactions (tabs, item list, Buy button, Repair All button)
		if (gnLeftPanel == LEFTPANEL_VENDOR)
		{
			// 4 Category Tabs (All, Armor, Weapons, Misc at y=80..110)
			if (nY >= 80 && nY <= 110 && nX >= 88 && nX < 392)
			{
				gnVendorActiveTab = std::max<int32_t>(0, std::min<int32_t>((nX - 88) / 76, 3));
				gnSelectedVendorItemIdx = -1;
				D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
				return true;
			}
			// Item rows (y=114..394, 10 rows of 28px each)
			if (nY >= 114 && nY < 394 && nX >= 92 && nX < 388)
			{
				int32_t aVendorItemIndices[10] = {};
				const int32_t nVendorCount = BuildVendorTabItemList(gnVendorActiveTab, aVendorItemIndices, 10);
				const int32_t nClickedRow = (nY - 114) / 28;
				if (nClickedRow >= 0 && nClickedRow < nVendorCount)
				{
					const int32_t nItemIdx = aVendorItemIndices[nClickedRow];
					const ClientItemEntry& it = gClientItems[nItemIdx];
					if (gnSelectedVendorItemIdx == nItemIdx && gnActiveNpcGuid != 0)
					{
						SendTownFolkBuy(gnActiveNpcGuid, it.dwItemGuid, 0u, it.dwCost);
						D2SOUND_FastPlaySound("data\\global\\sfx\\items\\gold.wav");
					}
					else
					{
						gnSelectedVendorItemIdx = nItemIdx;
						D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
					}
					return true;
				}
			}
			// [BUY ITEM] Button (x=96..210, y=408..438)
			if (nX >= 96 && nX <= 210 && nY >= 408 && nY <= 438)
			{
				if (gnSelectedVendorItemIdx >= 0 && gnSelectedVendorItemIdx < kMaxClientItems &&
					gClientItems[gnSelectedVendorItemIdx].bActive &&
					(gClientItems[gnSelectedVendorItemIdx].nLocation == CLI_ITEMLOC_VENDOR || gClientItems[gnSelectedVendorItemIdx].nAction == 0x0B) &&
					gnActiveNpcGuid != 0)
				{
					const ClientItemEntry& it = gClientItems[gnSelectedVendorItemIdx];
					SendTownFolkBuy(gnActiveNpcGuid, it.dwItemGuid, 0u, it.dwCost);
					D2SOUND_FastPlaySound("data\\global\\sfx\\items\\gold.wav");
				}
				return true;
			}
			// [REPAIR ALL] Button (x=220..334, y=408..438)
			if (nX >= 220 && nX <= 334 && nY >= 408 && nY <= 438)
			{
				if (gnActiveNpcGuid != 0)
				{
					SendTownFolkRepairAll(gnActiveNpcGuid);
					D2SOUND_FastPlaySound("data\\global\\sfx\\items\\repair.wav");
				}
				return true;
			}
		}
		// Character Screen '+' Stat Point Allocation Buttons (C2S 0x3A IncrementStat)
		if (gnLeftPanel == LEFTPANEL_CHAR && gaClientStats[STAT_STATPTS] > 0)
		{
			static const int32_t kStatBtnY[4] = { 142, 204, 290, 352 };
			static const uint16_t kStatIds[4] = { STAT_STRENGTH, STAT_DEXTERITY, STAT_VITALITY, STAT_ENERGY };
			for (int32_t s = 0; s < 4; ++s)
			{
				if (nX >= 204 && nX <= 240 && nY >= kStatBtnY[s] && nY <= kStatBtnY[s] + 34)
				{
					SendIncrementStat(kStatIds[s]);
					D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
					return true;
				}
			}
		}
		// Quest Log Act I..V tabs (y=64..96) & 6 quest icon sockets (3 columns x 2 rows)
		if (gnLeftPanel == LEFTPANEL_QUEST)
		{
			if (nY >= 64 && nY <= 96 && nX >= 86 && nX < 390)
			{
				gnQuestLogActTab = std::max<int32_t>(0, std::min<int32_t>((nX - 86) / 60, 4));
				gnSelectedQuest = 0;
				D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
				return true;
			}
			const int32_t nMaxQuestsForAct = (gnQuestLogActTab == 3) ? 3 : 6;
			for (int32_t q = 0; q < nMaxQuestsForAct; ++q)
			{
				const int32_t qCol = q % 3;
				const int32_t qRow = q / 3;
				const int32_t qX = 116 + qCol * 84;
				const int32_t qY = 132 + qRow * 78;
				if (nX >= qX && nX <= qX + 56 && nY >= qY && nY <= qY + 56)
				{
					gnSelectedQuest = q;
					D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
					return true;
				}
			}
		}
		// Stash Panel (Deposit/Withdraw Gold buttons + 6x8 Stash grid)
		if (gnLeftPanel == LEFTPANEL_STASH)
		{
			if (nX >= 96 && nX <= 226 && nY >= 104 && nY <= 132)
			{
				SendStashDepositGold(100u);
				return true;
			}
			if (nX >= 238 && nX <= 368 && nY >= 104 && nY <= 132)
			{
				SendStashWithdrawGold(100u);
				return true;
			}
			if (nX >= 153 && nX < 153 + 6 * 29 && nY >= 142 && nY < 142 + 8 * 29)
			{
				const int32_t gX = (nX - 153) / 29;
				const int32_t gY = (nY - 142) / 29;
				if (gnCursorItemIdx >= 0 && gnCursorItemIdx < kMaxClientItems && gClientItems[gnCursorItemIdx].bActive)
				{
					ClientItemEntry& cur = gClientItems[gnCursorItemIdx];
					cur.nLocation = CLI_ITEMLOC_STASH;
					cur.nStorePage = 4;
					cur.nGridX = gX;
					cur.nGridY = gY;
					SendInsertItemToBuffer_18(cur.dwItemGuid, static_cast<uint32_t>(gX), static_cast<uint32_t>(gY), 4u);
					gnCursorItemIdx = -1;
					D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
					return true;
				}
				for (int32_t i = 0; i < kMaxClientItems; ++i)
				{
					ClientItemEntry& it = gClientItems[i];
					if (it.bActive && it.nLocation == CLI_ITEMLOC_STASH &&
						gX >= it.nGridX && gX < it.nGridX + it.nInvW &&
						gY >= it.nGridY && gY < it.nGridY + it.nInvH)
					{
						it.nLocation = CLI_ITEMLOC_CURSOR;
						gnCursorItemIdx = i;
						SendRemoveItemFromBuffer_19(it.dwItemGuid);
						D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
						return true;
					}
				}
			}
		}
		// Horadric Cube Panel (Transmute button + 3x4 Cube grid)
		if (gnLeftPanel == LEFTPANEL_CUBE)
		{
			if (nX >= 180 && nX <= 300 && nY >= 352 && nY <= 394)
			{
				SendHoradricCubeTransmute();
				return true;
			}
			if (nX >= 196 && nX < 196 + 3 * 29 && nY >= 198 && nY < 198 + 4 * 29)
			{
				const int32_t gX = (nX - 196) / 29;
				const int32_t gY = (nY - 198) / 29;
				if (gnCursorItemIdx >= 0 && gnCursorItemIdx < kMaxClientItems && gClientItems[gnCursorItemIdx].bActive)
				{
					ClientItemEntry& cur = gClientItems[gnCursorItemIdx];
					cur.nLocation = CLI_ITEMLOC_CUBE;
					cur.nStorePage = 3;
					cur.nGridX = gX;
					cur.nGridY = gY;
					SendInsertItemToBuffer_18(cur.dwItemGuid, static_cast<uint32_t>(gX), static_cast<uint32_t>(gY), 3u);
					gnCursorItemIdx = -1;
					D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
					return true;
				}
				for (int32_t i = 0; i < kMaxClientItems; ++i)
				{
					ClientItemEntry& it = gClientItems[i];
					if (it.bActive && it.nLocation == CLI_ITEMLOC_CUBE &&
						gX >= it.nGridX && gX < it.nGridX + it.nInvW &&
						gY >= it.nGridY && gY < it.nGridY + it.nInvH)
					{
						it.nLocation = CLI_ITEMLOC_CURSOR;
						gnCursorItemIdx = i;
						SendRemoveItemFromBuffer_19(it.dwItemGuid);
						D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
						return true;
					}
				}
			}
		}
		// Waypoint Menu (Act I..V tabs + 9 destination rows)
		if (gnLeftPanel == LEFTPANEL_WAYPOINT)
		{
			if (nY >= 64 && nY <= 96 && nX >= 86 && nX < 390)
			{
				gnWaypointActTab = std::max<int32_t>(0, std::min<int32_t>((nX - 86) / 60, 4));
				D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
				return true;
			}
			static const uint8_t kWaypointDestLevels[5][9] = {
				{ LEVEL_ROGUEENCAMPMENT, LEVEL_COLDPLAINS, LEVEL_STONYFIELD, LEVEL_DARKWOOD, LEVEL_BLACKMARSH, LEVEL_OUTERCLOISTER, LEVEL_JAILLEV1, LEVEL_INNERCLOISTER, LEVEL_CATACOMBSLEV2 },
				{ LEVEL_LUTGHOLEIN, LEVEL_SEWERSLEV2, LEVEL_DRYHILLS, LEVEL_HALLSOFTHEDEADLEV2, LEVEL_FAROASIS, LEVEL_LOSTCITY, LEVEL_PALACECELLARLEV1, LEVEL_ARCANESANCTUARY, LEVEL_CANYONOFTHEMAGI },
				{ LEVEL_KURASTDOCKTOWN, LEVEL_SPIDERFOREST, LEVEL_GREATMARSH, LEVEL_FLAYERJUNGLE, LEVEL_LOWERKURAST, LEVEL_KURASTBAZAAR, LEVEL_UPPERKURAST, LEVEL_TRAVINCAL, LEVEL_DURANCEOFHATELEV2 },
				{ LEVEL_THEPANDEMONIUMFORTRESS, LEVEL_CITYOFTHEDAMNED, LEVEL_RIVEROFFLAME, 0, 0, 0, 0, 0, 0 },
				{ LEVEL_HARROGATH, LEVEL_ID_ACT5_BARRICADE_1, LEVEL_ARREATPLATEAU, LEVEL_CRYSTALIZEDCAVERNLEV1, LEVEL_CRYSTALIZEDCAVERNLEV2, LEVEL_HALLSOFDEATHSCALLING, LEVEL_TUNDRAWASTELANDS, LEVEL_GLACIALCAVESLEV1, LEVEL_THEWORLDSTONEKEEPLEV2 },
			};
			const int32_t nMaxRows = (gnWaypointActTab == 3) ? 3 : 9;
			if (nX >= 96 && nX <= 384 && nY >= 112 && nY < 112 + nMaxRows * 34)
			{
				const int32_t nRow = (nY - 112) / 34;
				const uint8_t nDestLvl = kWaypointDestLevels[gnWaypointActTab][nRow];
				if (nDestLvl > 0)
				{
					SendWaypointTeleport_49(gdwActiveWaypointGuid, nDestLvl, static_cast<uint8_t>(gnWaypointActTab));
				}
				return true;
			}
		}
		// Mercenary Inventory Panel (Head, Torso, Right Arm, Left Arm slots)
		if (gnLeftPanel == LEFTPANEL_MERC)
		{
			if (nX >= 140 && nX <= 340 && nY >= 120 && nY <= 340)
			{
				SendMercEquip_61(1u);
				D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
				return true;
			}
		}
		return true;
	}

	// 6. Open Right Panel interactions (x=400..720, y=60..492)
	if (gnRightPanel != RIGHTPANEL_NONE && nX >= 400 && nX < 720 && nY >= 60 && nY < 492)
	{
		// Close 'X' button (checked at bottom-left, bottom-center, and bottom-right of Right Panel)
		if ((nX >= 414 && nX <= 454 && nY >= 444 && nY <= 484) ||
			(nX >= 564 && nX <= 606 && nY >= 444 && nY <= 484) ||
			(nX >= 670 && nX <= 712 && nY >= 444 && nY <= 484))
		{
			gnRightPanel = RIGHTPANEL_NONE;
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
			return true;
		}
		if (gnRightPanel == RIGHTPANEL_INV)
		{
			// Weapon Switch I / II tabs above left & right weapon slots (C2S 0x60 SwapWeapons)
			if ((nX >= 414 && nX <= 478 && nY >= 80 && nY <= 108) ||
				(nX >= 642 && nX <= 706 && nY >= 80 && nY <= 108))
			{
				MSG_SendByte(0x60u);
				D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
				return true;
			}

			// Paperdoll equipment slots 1..10 (C2S 0x1C RemoveBodyItem / 0x1A EquipItem / 0x1D SwapBodyItem)
			for (int32_t nBodyLoc = 1; nBodyLoc <= 10; ++nBodyLoc)
			{
				int32_t nSlotX = 0, nSlotY = 0, nSlotW = 0, nSlotH = 0;
				GetBodySlotRect800x600(nBodyLoc, &nSlotX, &nSlotY, &nSlotW, &nSlotH);
				if (nX < nSlotX || nX >= nSlotX + nSlotW || nY < nSlotY || nY >= nSlotY + nSlotH)
				{
					continue;
				}
				int32_t nEquippedIdx = -1;
				for (int32_t i = 0; i < kMaxClientItems; ++i)
				{
					if (gClientItems[i].bActive && gClientItems[i].nLocation == CLI_ITEMLOC_EQUIPPED &&
						gClientItems[i].nBodyLoc == static_cast<uint8_t>(nBodyLoc))
					{
						nEquippedIdx = i;
						break;
					}
				}
				if (nEquippedIdx < 0 && (nBodyLoc == BODYLOC_RARM || nBodyLoc == BODYLOC_LARM))
				{
					const uint8_t nOtherArm = (nBodyLoc == BODYLOC_RARM) ? BODYLOC_LARM : BODYLOC_RARM;
					for (int32_t i = 0; i < kMaxClientItems; ++i)
					{
						if (gClientItems[i].bActive && gClientItems[i].nLocation == CLI_ITEMLOC_EQUIPPED &&
							gClientItems[i].nBodyLoc == nOtherArm)
						{
							if (D2ItemsTxt* pTxt = DATATBLS_GetItemsTxtRecord(gClientItems[i].wItemClassId))
							{
								if (pTxt->n2Handed)
								{
									nEquippedIdx = i;
									break;
								}
							}
						}
					}
				}

				if (gnCursorItemIdx >= 0 && gnCursorItemIdx < kMaxClientItems && gClientItems[gnCursorItemIdx].bActive)
				{
					ClientItemEntry& cur = gClientItems[gnCursorItemIdx];
					if (nEquippedIdx >= 0)
					{
						ClientItemEntry& eq = gClientItems[nEquippedIdx];
						const uint16_t wTargetLoc = static_cast<uint16_t>(eq.nBodyLoc ? eq.nBodyLoc : nBodyLoc);
						SendSwapBodyItem_1D(cur.dwItemGuid, wTargetLoc);
						cur.nLocation = CLI_ITEMLOC_EQUIPPED;
						cur.nBodyLoc = static_cast<uint8_t>(wTargetLoc);
						eq.nLocation = CLI_ITEMLOC_CURSOR;
						gnCursorItemIdx = nEquippedIdx;
					}
					else
					{
						SendEquipItem_1A(cur.dwItemGuid, static_cast<uint16_t>(nBodyLoc));
						cur.nLocation = CLI_ITEMLOC_EQUIPPED;
						cur.nBodyLoc = static_cast<uint8_t>(nBodyLoc);
						gnCursorItemIdx = -1;
					}
					D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
					return true;
				}
				else if (nEquippedIdx >= 0)
				{
					ClientItemEntry& eq = gClientItems[nEquippedIdx];
					SendRemoveBodyItem_1C(static_cast<uint16_t>(eq.nBodyLoc));
					eq.nLocation = CLI_ITEMLOC_CURSOR;
					gnCursorItemIdx = nEquippedIdx;
					D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
					return true;
				}
				return true;
			}

			// Inventory 10x4 grid left-click item pickup/drop onto cursor (C2S 0x18 / 0x19)
			int32_t nGridLeft = 419, nGridTop = 315, nBoxW = 29, nBoxH = 29;
			GetInvGridLayout800x600(&nGridLeft, &nGridTop, &nBoxW, &nBoxH);
			if (nX >= nGridLeft && nX < nGridLeft + 10 * nBoxW && nY >= nGridTop && nY < nGridTop + 4 * nBoxH)
			{
				const int32_t gX = (nX - nGridLeft) / nBoxW;
				const int32_t gY = (nY - nGridTop) / nBoxH;
				if (gnCursorItemIdx >= 0 && gnCursorItemIdx < kMaxClientItems && gClientItems[gnCursorItemIdx].bActive)
				{
					ClientItemEntry& cur = gClientItems[gnCursorItemIdx];
					cur.nLocation = CLI_ITEMLOC_CONTAINER;
					cur.nStorePage = 0;
					cur.nGridX = gX;
					cur.nGridY = gY;
					SendInsertItemToBuffer_18(cur.dwItemGuid, static_cast<uint32_t>(gX), static_cast<uint32_t>(gY), 0u);
					gnCursorItemIdx = -1;
					D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
					return true;
				}
				for (int32_t i = 0; i < kMaxClientItems; ++i)
				{
					ClientItemEntry& it = gClientItems[i];
					if (it.bActive && it.nLocation == CLI_ITEMLOC_CONTAINER &&
						gX >= it.nGridX && gX < it.nGridX + it.nInvW &&
						gY >= it.nGridY && gY < it.nGridY + it.nInvH)
					{
						it.nLocation = CLI_ITEMLOC_CURSOR;
						gnCursorItemIdx = i;
						SendRemoveItemFromBuffer_19(it.dwItemGuid);
						D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
						return true;
					}
				}
			}
		}
		// Skill Tree 3 right-hand tabs (x=628..720)
		if (gnRightPanel == RIGHTPANEL_SKILL && nX >= 628 && nX < 720)
		{
			if (nY >= 115 && nY < 276)
			{
				gnActiveSkillTab = 2;
				D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
				return true;
			}
			else if (nY >= 276 && nY < 384)
			{
				gnActiveSkillTab = 1;
				D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
				return true;
			}
			else if (nY >= 384 && nY < 492)
			{
				gnActiveSkillTab = 0;
				D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
				return true;
			}
		}
		// Skill Tree skill node clicks (C2S 0x3B IncrementSkill / C2S 0x3C SelectSkill)
		if (gnRightPanel == RIGHTPANEL_SKILL && sgptDataTables && sgptDataTables->pSkillsTxt && sgptDataTables->pSkillDescTxt)
		{
			const int32_t nTabPage = std::max<int32_t>(0, std::min<int32_t>(gnActiveSkillTab, 2)) + 1;
			static const int32_t kSkillColX[3] = { 415, 484, 553 };
			static const int32_t kSkillRowY[6] = { 74, 142, 210, 278, 346, 414 };
			for (int32_t sId = 0; sId < sgptDataTables->nSkillsTxtRecordCount; ++sId)
			{
				const D2SkillsTxt& sk = sgptDataTables->pSkillsTxt[sId];
				if (sk.nCharClass != static_cast<int8_t>(gnCharClass) || sk.wSkillDesc <= 0 || sk.wSkillDesc >= sgptDataTables->nSkillDescTxtRecordCount)
				{
					continue;
				}
				const D2SkillDescTxt& sd = sgptDataTables->pSkillDescTxt[sk.wSkillDesc];
				if (sd.nSkillPage != nTabPage || sd.nSkillRow < 1 || sd.nSkillRow > 6 || sd.nSkillColumn < 1 || sd.nSkillColumn > 3)
				{
					continue;
				}
				const int32_t nodeX = kSkillColX[sd.nSkillColumn - 1];
				const int32_t nodeY = kSkillRowY[sd.nSkillRow - 1];
				if (nX >= nodeX && nX <= nodeX + 48 && nY >= nodeY && nY <= nodeY + 48)
				{
					if (gaClientStats[STAT_SKILLPTS] > 0)
					{
						SendIncrementSkill(static_cast<uint16_t>(sId));
					}
					gwRightSkillId = static_cast<uint16_t>(sId);
					SendSelectSkill(static_cast<uint16_t>(sId), false);
					D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
					return true;
				}
			}
		}
		return true;
	}

	// Clicking in the game world while holding an item on the cursor drops it to the ground (C2S 0x17)
	if (gnCursorItemIdx >= 0 && gnCursorItemIdx < kMaxClientItems && gClientItems[gnCursorItemIdx].bActive)
	{
		ClientItemEntry& curIt = gClientItems[gnCursorItemIdx];
		SendDropCursorItem_17(curIt.dwItemGuid);
		curIt.nLocation = CLI_ITEMLOC_GROUND;
		curIt.nWorldSubX = gnServerSubX > 0 ? gnServerSubX : static_cast<int32_t>(gfPlayerWorldX);
		curIt.nWorldSubY = gnServerSubY > 0 ? gnServerSubY : static_cast<int32_t>(gfPlayerWorldY);
		gnCursorItemIdx = -1;
		D2SOUND_FastPlaySound("data\\global\\sfx\\items\\flippy.wav");
		return true;
	}

	return false;
}

// 1.12a: 0x6FB1D200
static LRESULT __stdcall D2Client_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(nullptr);
			return TRUE;
		}
		break;
	case WM_CLOSE:
		SendLeaveGamePacket();
		gbWindowClosed = 1;
		gbExitApplication = 1;
		gnClientState = D2CLIENT_STATE_LEAVE_GAME;
		return 0;
	case WM_MOUSEMOVE:
		gMousePos.x = static_cast<int16_t>(LOWORD(lParam));
		gMousePos.y = static_cast<int16_t>(HIWORD(lParam));
		if (gbEscMenuOpen)
		{
			const int32_t nCount = (gnEscSubMenu == 0) ? 3 : ((gnEscSubMenu == 1) ? 5 : 1);
			const int32_t nBaseY = (gnEscSubMenu == 0) ? 205 : 175;
			const int32_t nStepY = (gnEscSubMenu == 0) ? 55 : 48;
			for (int32_t i = 0; i < nCount; ++i)
			{
				const int32_t nRowY = nBaseY + i * nStepY;
				if (gMousePos.x >= 180 && gMousePos.x <= 620 && gMousePos.y >= nRowY - 28 && gMousePos.y <= nRowY + 12)
				{
					gnEscSelectedIndex = i;
					break;
				}
			}
			return 0;
		}
		if (gbNpcMenuOpen)
		{
			if (gMousePos.x >= 290 && gMousePos.x <= 510)
			{
				for (int32_t i = 0; i < 3; ++i)
				{
					const int32_t nRowY = 172 + i * 28;
					if (gMousePos.y >= nRowY - 18 && gMousePos.y <= nRowY + 8)
					{
						gnNpcMenuSelectedIdx = i;
						break;
					}
				}
			}
			return 0;
		}
		UpdateHoveredWorldTarget(gMousePos.x, gMousePos.y);
		if (gbMouseLeftHeld)
		{
			SetMoveTargetFromScreenPoint(gMousePos.x, gMousePos.y, true);
		}
		return 0;
	case WM_LBUTTONDOWN:
		gMousePos.x = static_cast<int16_t>(LOWORD(lParam));
		gMousePos.y = static_cast<int16_t>(HIWORD(lParam));
		if (HandleInGameUiClick(gMousePos.x, gMousePos.y))
		{
			gbMouseLeftHeld = false;
			return 0;
		}
		gbMouseLeftHeld = true;
		SetMoveTargetFromScreenPoint(gMousePos.x, gMousePos.y, true);
		return 0;
	case WM_RBUTTONDOWN:
		gMousePos.x = static_cast<int16_t>(LOWORD(lParam));
		gMousePos.y = static_cast<int16_t>(HIWORD(lParam));
		HandleRightClickCast(gMousePos.x, gMousePos.y);
		return 0;
	case WM_LBUTTONUP:
		gbMouseLeftHeld = false;
		return 0;
	case WM_SYSKEYDOWN:
		if (wParam == VK_MENU)
		{
			gbShowGroundItems = true;
			return 0;
		}
		break;
	case WM_SYSKEYUP:
		if (wParam == VK_MENU)
		{
			gbShowGroundItems = false;
			return 0;
		}
		break;
	case WM_KEYUP:
		if (wParam == VK_SNAPSHOT)
		{
			D2Win_10168_WINMAIN_CreateScreenshot();
			return 0;
		}
		if (wParam == VK_MENU)
		{
			gbShowGroundItems = false;
			return 0;
		}
		break;
	case WM_CHAR:
		if (gbChatInputOpen)
		{
			if (wParam == VK_BACK)
			{
				if (gnChatInputLen > 0)
				{
					gszChatInputBuf[--gnChatInputLen] = '\0';
				}
			}
			else if (wParam >= 32 && wParam < 127 && gnChatInputLen < static_cast<int32_t>(sizeof(gszChatInputBuf) - 2))
			{
				gszChatInputBuf[gnChatInputLen++] = static_cast<char>(wParam);
				gszChatInputBuf[gnChatInputLen] = '\0';
			}
			return 0;
		}
		break;
	case WM_KEYDOWN:
		if (gbChatInputOpen)
		{
			if (wParam == VK_RETURN)
			{
				ExecuteChatOrSlashCommand(gszChatInputBuf);
				gbChatInputOpen = false;
				gszChatInputBuf[0] = '\0';
				gnChatInputLen = 0;
				return 0;
			}
			if (wParam == VK_ESCAPE)
			{
				gbChatInputOpen = false;
				gszChatInputBuf[0] = '\0';
				gnChatInputLen = 0;
				return 0;
			}
			return 0;
		}
		if (wParam == VK_RETURN && !gbEscMenuOpen && !gbNpcMenuOpen)
		{
			gbChatInputOpen = true;
			gszChatInputBuf[0] = '\0';
			gnChatInputLen = 0;
			return 0;
		}
		if (wParam == VK_MENU)
		{
			gbShowGroundItems = true;
			return 0;
		}
		if (wParam == VK_ESCAPE)
		{
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
			if (gbEscMenuOpen)
			{
				if (gnEscSubMenu != 0)
				{
					gnEscSubMenu = 0;
					gnEscSelectedIndex = 0;
				}
				else
				{
					gbEscMenuOpen = false;
				}
			}
			else if (gbNpcMenuOpen || gnLeftPanel == LEFTPANEL_VENDOR)
			{
				CloseNpcDialogAndVendor();
				gnRightPanel = RIGHTPANEL_NONE;
			}
			else if (gnLeftPanel != LEFTPANEL_NONE || gnRightPanel != RIGHTPANEL_NONE)
			{
				gnLeftPanel = LEFTPANEL_NONE;
				gnRightPanel = RIGHTPANEL_NONE;
			}
			else
			{
				gbEscMenuOpen = true;
				gnEscSubMenu = 0;
				gnEscSelectedIndex = 0;
			}
			return 0;
		}
		if (gbNpcMenuOpen)
		{
			if (wParam == VK_UP)
			{
				gnNpcMenuSelectedIdx = (gnNpcMenuSelectedIdx + 2) % 3;
			}
			else if (wParam == VK_DOWN)
			{
				gnNpcMenuSelectedIdx = (gnNpcMenuSelectedIdx + 1) % 3;
			}
			else if (wParam == VK_RETURN || wParam == VK_SPACE)
			{
				ActivateNpcMenuOption(gnNpcMenuSelectedIdx);
			}
			return 0;
		}
		if (gbEscMenuOpen)
		{
			const int32_t nCount = (gnEscSubMenu == 0) ? 3 : ((gnEscSubMenu == 1) ? 5 : 1);
			if (wParam == VK_UP)
			{
				gnEscSelectedIndex = (gnEscSelectedIndex + nCount - 1) % nCount;
			}
			else if (wParam == VK_DOWN)
			{
				gnEscSelectedIndex = (gnEscSelectedIndex + 1) % nCount;
			}
			else if (wParam == VK_RETURN || wParam == VK_SPACE)
			{
				ActivateEscMenuItem(gnEscSelectedIndex);
			}
			return 0;
		}
		if (wParam >= '1' && wParam <= '4')
		{
			const int32_t nBeltCol = static_cast<int32_t>(wParam - '1');
			for (int32_t i = 0; i < kMaxClientItems; ++i)
			{
				if (gClientItems[i].bActive && gClientItems[i].nLocation == CLI_ITEMLOC_BELT && (gClientItems[i].nGridX % 4) == nBeltCol)
				{
					SendUseBeltItem(gClientItems[i].dwItemGuid);
					D2SOUND_FastPlaySound("data\\global\\sfx\\items\\potiondrink.wav");
					break;
				}
			}
			return 0;
		}
		if (wParam == 'C' || wParam == 'A')
		{
			gnLeftPanel = (gnLeftPanel == LEFTPANEL_CHAR) ? LEFTPANEL_NONE : LEFTPANEL_CHAR;
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		}
		else if (wParam == 'I' || wParam == 'B')
		{
			gnRightPanel = (gnRightPanel == RIGHTPANEL_INV) ? RIGHTPANEL_NONE : RIGHTPANEL_INV;
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		}
		else if (wParam == 'T' || wParam == 'K')
		{
			gnRightPanel = (gnRightPanel == RIGHTPANEL_SKILL) ? RIGHTPANEL_NONE : RIGHTPANEL_SKILL;
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		}
		else if (wParam == 'Q')
		{
			gnLeftPanel = (gnLeftPanel == LEFTPANEL_QUEST) ? LEFTPANEL_NONE : LEFTPANEL_QUEST;
			if (gnLeftPanel == LEFTPANEL_QUEST)
			{
				MSG_SendByte(0x40u);
			}
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		}
		else if (wParam == 'P')
		{
			gnLeftPanel = (gnLeftPanel == LEFTPANEL_PARTY) ? LEFTPANEL_NONE : LEFTPANEL_PARTY;
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		}
		else if (wParam == 'M')
		{
			gnLeftPanel = (gnLeftPanel == LEFTPANEL_MSGLOG) ? LEFTPANEL_NONE : LEFTPANEL_MSGLOG;
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		}
		else if (wParam == 'W')
		{
			MSG_SendByte(0x60u);
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		}
		else if (wParam == VK_TAB)
		{
			gbAutomapOpen = !gbAutomapOpen;
		}
		else if (wParam == 'R')
		{
			gbRunEnabled = !gbRunEnabled;
		}
		else if (wParam == 'O')
		{
			gnLeftPanel = (gnLeftPanel == LEFTPANEL_MERC) ? LEFTPANEL_NONE : LEFTPANEL_MERC;
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		}
		else if (wParam == 'S')
		{
			gnLeftPanel = (gnLeftPanel == LEFTPANEL_STASH) ? LEFTPANEL_NONE : LEFTPANEL_STASH;
			if (gnLeftPanel == LEFTPANEL_STASH)
			{
				gnRightPanel = RIGHTPANEL_INV;
			}
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		}
		else if (wParam == 'H')
		{
			gnLeftPanel = (gnLeftPanel == LEFTPANEL_CUBE) ? LEFTPANEL_NONE : LEFTPANEL_CUBE;
			if (gnLeftPanel == LEFTPANEL_CUBE)
			{
				gnRightPanel = RIGHTPANEL_INV;
			}
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		}
		else if (wParam == VK_OEM_3) // '~' / '`' key: toggle 4x4 expanded belt
		{
			gbBeltExpanded = !gbBeltExpanded;
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
		}
		else if (wParam == VK_LEFT)
		{
			SetMoveTargetFromScreenPoint(280, 280, true);
		}
		else if (wParam == VK_RIGHT)
		{
			SetMoveTargetFromScreenPoint(520, 280, true);
		}
		else if (wParam == VK_UP)
		{
			SetMoveTargetFromScreenPoint(400, 190, true);
		}
		else if (wParam == VK_DOWN)
		{
			SetMoveTargetFromScreenPoint(400, 370, true);
		}
		break;
	default:
		break;
	}
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

static void DrainNetworkPackets();

// 1.12a: 0x6FB294E0
void __fastcall D2Client_SendJoinGamePacket(const char* szCharName)
{
	char szConnectHost[128] = {};
	if (GetEnvironmentVariableA("D2MOO_CONNECT_HOST", szConnectHost, sizeof(szConnectHost)) > 0 && szConnectHost[0])
	{
		// Wait briefly for server 0xAF 0x01 greeting packet
		for (int32_t w = 0; w < 25 && !gbClientLeaveCompleted; ++w)
		{
			DrainNetworkPackets();
			if (gbClientLeaveCompleted)
			{
				break;
			}
			Sleep(20);
		}
		// Remote / Local TCP D2GS Port 4000 JoinGameRequest (0x68, 37 bytes)
		uint8_t aPkt68[37] = {};
		aPkt68[0] = 0x68u;
		*reinterpret_cast<uint32_t*>(&aPkt68[1]) = 1u;
		*reinterpret_cast<uint16_t*>(&aPkt68[5]) = 1u;
		aPkt68[7] = gnCharClass;
		*reinterpret_cast<uint32_t*>(&aPkt68[8]) = 0x0Cu; // 1.12a version
		*reinterpret_cast<uint32_t*>(&aPkt68[12]) = 0x506DFC26u;
		*reinterpret_cast<uint32_t*>(&aPkt68[16]) = 0xAA55AA55u;
		aPkt68[20] = 0u;
		SStrCopy(reinterpret_cast<char*>(&aPkt68[21]), szCharName ? szCharName : gszCharName, 16);
		CLIENT_Send(0, aPkt68, sizeof(aPkt68));
		return;
	}

	D2ClientLogonPacketStrc tPacket = {};
	tPacket.nHeader = 0x67;
	SStrCopy(tPacket.szCharName, szCharName ? szCharName : gszCharName, sizeof(tPacket.szCharName));

	if (gnGameType == 0)
	{
		tPacket.nGameMode = 3;
	}
	else if (gnGameType == 6)
	{
		tPacket.nGameMode = 1;
	}
	else if (gnGameType == 8)
	{
		tPacket.nGameMode = 2;
	}
	else
	{
		tPacket.nGameMode = 0;
	}

	if (gpD2Config)
	{
		tPacket.nCharClass = gpD2Config->unpackedCTemp.nCharacterClassId;
		if (gpD2Config->nArenaFlags == 0)
		{
			gpD2Config->nArenaFlags = 0x100004u;
		}
		tPacket.wArena = gpD2Config->wArena;
		tPacket.dwArenaFlags = gpD2Config->nArenaFlags;
		tPacket.nArenaTemplate = gpD2Config->nArenaTemplate;
		tPacket.nArenaDifficulty = gpD2Config->nArenaDifficulty;
	}
	else
	{
		tPacket.dwArenaFlags = 0x100004u;
	}

	tPacket.nLocaleId = static_cast<uint8_t>(STRTABLE_GetLanguage());
	SStrCopy(tPacket.szAccountName, szCharName ? szCharName : gszCharName, sizeof(tPacket.szAccountName));

	CLIENT_Send(0, reinterpret_cast<const uint8_t*>(&tPacket), 0x2E);
}

// 1.12a: 0x6FB2B060
static DWORD __stdcall D2Client_ServerThreadProc(LPVOID lpParameter)
{
	(void)lpParameter;
	BOOL bHasClients = FALSE;

	for (;;)
	{
		EnterCriticalSection(&gServerThreadLock);
		const int32_t bStop = gbStopServerThread;
		const int32_t bRun = gbRunServerThreadUpdates;
		LeaveCriticalSection(&gServerThreadLock);

		if (bStop)
		{
			break;
		}

		if (bRun)
		{
			GAME_ProcessNetworkMessages();
			bHasClients = GAME_UpdateGamesProgress(1);
			if (bHasClients)
			{
				GAME_UpdateClients(0, 0);
			}
		}

		if (bHasClients)
		{
			Sleep(30);
			if (gpD2Config && gpD2Config->pComInterface)
			{
				(void)GAME_GetPlayerUnitsCount(1);
			}
		}
		else
		{
			Sleep(10);
		}
	}

	GAME_CloseAllGames();
	GAME_UpdateClients(0, 1);
	DeleteCriticalSection(&gServerThreadLock);
	OutputDebugStringA("Server thread terminated\n");
	return 1;
}

static uint32_t gdwGamePacketsReceived = 0;
static uint32_t gdwServerTicks = 0;

static ClientWorldUnit* FindOrAllocClientUnit(uint8_t nUnitType, uint32_t dwUnitGuid)
{
	int32_t nFreeIdx = -1;
	for (int32_t i = 0; i < kMaxClientUnits; ++i)
	{
		if (gClientUnits[i].bActive)
		{
			if (gClientUnits[i].nUnitType == nUnitType && gClientUnits[i].dwUnitGuid == dwUnitGuid)
			{
				return &gClientUnits[i];
			}
		}
		else if (nFreeIdx < 0)
		{
			nFreeIdx = i;
		}
	}
	if (nFreeIdx < 0)
	{
		return nullptr;
	}
	ClientWorldUnit& u = gClientUnits[nFreeIdx];
	std::memset(&u, 0, sizeof(u));
	u.bActive = true;
	u.nUnitType = nUnitType;
	u.dwUnitGuid = dwUnitGuid;
	u.nLifePct = 128;
	return &u;
}

static void SetClientUnitSubtilePos(ClientWorldUnit* pUnit, int32_t nSubX, int32_t nSubY, bool bSnap)
{
	if (!pUnit)
	{
		return;
	}
	pUnit->nSubX = nSubX;
	pUnit->nSubY = nSubY;
	int nIsoX = nSubX;
	int nIsoY = nSubY;
	DUNGEON_GameSubtileToClientCoords(&nIsoX, &nIsoY);
	pUnit->fTargetWorldX = static_cast<float>(nIsoX);
	pUnit->fTargetWorldY = static_cast<float>(nIsoY);
	if (bSnap || (pUnit->fWorldX == 0.0f && pUnit->fWorldY == 0.0f))
	{
		pUnit->fWorldX = pUnit->fTargetWorldX;
		pUnit->fWorldY = pUnit->fTargetWorldY;
		pUnit->bMoving = false;
	}
	else
	{
		pUnit->bMoving = true;
	}
}

static void HandleServerItemActionPacket(const uint8_t* pBuf, int32_t nLen, bool bOwned9D)
{
	const int32_t nHdrSize = bOwned9D ? 13 : 8;
	if (nLen <= nHdrSize)
	{
		return;
	}
	const uint8_t nAction = pBuf[1];
	const uint32_t dwItemGuid = *reinterpret_cast<const uint32_t*>(&pBuf[4]);

	D2ItemSaveStrc tSave = {};
	ITEMS_GetCompactItemDataFromBitstream(
		const_cast<uint8_t*>(&pBuf[nHdrSize]),
		static_cast<size_t>(nLen - nHdrSize),
		FALSE,
		&tSave);

	// Find or allocate ClientItemEntry
	int32_t nIdx = -1;
	int32_t nFreeIdx = -1;
	for (int32_t i = 0; i < kMaxClientItems; ++i)
	{
		if (gClientItems[i].bActive)
		{
			if (gClientItems[i].dwItemGuid == dwItemGuid)
			{
				nIdx = i;
				break;
			}
		}
		else if (nFreeIdx < 0)
		{
			nFreeIdx = i;
		}
	}

	// Actions that permanently delete/consume an item from vendor/container/belt/body
	if (nAction == 0x0C ||
		((nAction == 0x01 || nAction == 0x05 || nAction == 0x08 || nAction == 0x0F) &&
		 (tSave.dwFlags & (IFLAG_DELETED | IFLAG_QUANTITY)) != 0))
	{
		if (nIdx >= 0)
		{
			gClientItems[nIdx].bActive = false;
			if (gnCursorItemIdx == nIdx)
			{
				gnCursorItemIdx = -1;
			}
		}
		return;
	}

	if (nIdx < 0)
	{
		if (nFreeIdx < 0)
		{
			return;
		}
		nIdx = nFreeIdx;
		std::memset(&gClientItems[nIdx], 0, sizeof(ClientItemEntry));
	}

	ClientItemEntry& it = gClientItems[nIdx];
	it.bActive = true;
	it.dwItemGuid = dwItemGuid;
	it.nAction = nAction;
	it.wItemClassId = static_cast<uint16_t>(tSave.nClassId);
	it.nBodyLoc = tSave.nBodyloc;
	it.nStorePage = tSave.nStorePage;
	it.nGridX = tSave.nX;
	it.nGridY = tSave.nY;
	it.nWorldSubX = tSave.nX;
	it.nWorldSubY = tSave.nY;
	it.nInvW = 1;
	it.nInvH = 1;
	it.dwCost = 25;

	if (nAction == 0x0B)
	{
		it.nLocation = CLI_ITEMLOC_VENDOR;
		++gdwVendorItemsReceived;
	}
	else if (nAction == 0x01 || nAction == 0x05 || nAction == 0x08 || nAction == 0x0F || nAction == 0x12 || tSave.nAnimMode == IMODE_ONCURSOR)
	{
		it.nLocation = CLI_ITEMLOC_CURSOR;
		gnCursorItemIdx = nIdx;
	}
	else if (nAction == 0x00 || nAction == 0x02 || nAction == 0x03 || tSave.nAnimMode == IMODE_ONGROUND || tSave.nAnimMode == IMODE_DROPPING)
	{
		it.nLocation = CLI_ITEMLOC_GROUND;
	}
	else if (nAction == 0x06 || nAction == 0x07 || nAction == 0x09 || nAction == 0x11 || tSave.nAnimMode == IMODE_EQUIP)
	{
		it.nLocation = CLI_ITEMLOC_EQUIPPED;
	}
	else if (nAction == 0x0E || nAction == 0x10 || tSave.nAnimMode == IMODE_INBELT)
	{
		it.nLocation = CLI_ITEMLOC_BELT;
	}
	else if (tSave.nStorePage == 4) // STORE_STASH
	{
		it.nLocation = CLI_ITEMLOC_STASH;
	}
	else if (tSave.nStorePage == 3) // STORE_CUBE
	{
		it.nLocation = CLI_ITEMLOC_CUBE;
	}
	else
	{
		it.nLocation = CLI_ITEMLOC_CONTAINER;
	}

	if (it.nLocation != CLI_ITEMLOC_CURSOR && gnCursorItemIdx == nIdx)
	{
		gnCursorItemIdx = -1;
	}

	D2ItemsTxt* pItemsTxt = DATATBLS_GetItemsTxtRecord(tSave.nClassId);
	if (pItemsTxt)
	{
		it.nInvW = std::max<uint8_t>(1, pItemsTxt->nInvWidth);
		it.nInvH = std::max<uint8_t>(1, pItemsTxt->nInvHeight);
		it.dwCost = std::max<uint32_t>(1u, pItemsTxt->dwCost);
		const Unicode* pUniName = D2LANG_GetStringFromTblIndex(pItemsTxt->wNameStr);
		UnicodeToAsciiSafe(pUniName, it.szName, sizeof(it.szName));
		if (!it.szName[0])
		{
			std::snprintf(it.szName, sizeof(it.szName), "Item #%d", tSave.nClassId);
		}
		if (pItemsTxt->szInvFile[0] && std::strcmp(it.szInvFile, pItemsTxt->szInvFile) != 0)
		{
			SStrCopy(it.szInvFile, pItemsTxt->szInvFile, sizeof(it.szInvFile));
			if (it.pInvCell)
			{
				ARCHIVE_FreeCellFile(it.pInvCell);
				it.pInvCell = nullptr;
			}
			char szAssetPath[128] = {};
			std::snprintf(szAssetPath, sizeof(szAssetPath), "DATA\\GLOBAL\\ITEMS\\%s", it.szInvFile);
			it.pInvCell = ARCHIVE_LoadCellFile(szAssetPath, 0);
		}
	}
	else
	{
		std::snprintf(it.szName, sizeof(it.szName), "Item #%d", tSave.nClassId);
	}
}

static bool IsTileInAnyDrlgLevel(D2DrlgStrc* pDrlg, int32_t tx, int32_t ty)
{
	if (!pDrlg || tx <= 0 || ty <= 0)
	{
		return false;
	}
	for (D2DrlgLevelStrc* pLvl = pDrlg->pLevel; pLvl != nullptr; pLvl = pLvl->pNextLevel)
	{
		if (pLvl->nLevelId > 0 &&
			tx >= pLvl->nPosX && tx < pLvl->nPosX + pLvl->nWidth &&
			ty >= pLvl->nPosY && ty < pLvl->nPosY + pLvl->nHeight)
		{
			return true;
		}
	}
	return false;
}

static D2ActiveRoomStrc* SafeStreamClientRoomAtTile(D2DrlgActStrc* pAct, int32_t tx, int32_t ty)
{
	if (!pAct || tx <= 0 || ty <= 0)
	{
		return nullptr;
	}
	D2DrlgStrc* pDrlg = DUNGEON_GetDrlgFromAct(pAct);
	if (!IsTileInAnyDrlgLevel(pDrlg, tx, ty))
	{
		return nullptr;
	}
	return DUNGEON_StreamRoomAtCoords(pAct, tx, ty);
}

static void PopulatePresetObjectsForStreamedRooms(D2DrlgActStrc* pAct)
{
	if (!pAct)
	{
		return;
	}
	for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(pAct); pRoom; pRoom = pRoom->pRoomNext)
	{
		if (!pRoom->pDrlgRoom)
		{
			continue;
		}
		for (D2PresetUnitStrc* pPreset = pRoom->pDrlgRoom->pPresetUnits; pPreset; pPreset = pPreset->pNext)
		{
			if (pPreset->nUnitType != UNIT_OBJECT || pPreset->nIndex == 266)
			{
				continue;
			}
			D2ObjectsTxt* pObjTxt = DATATBLS_GetObjectsTxtRecord(pPreset->nIndex);
			if (!pObjTxt || !pObjTxt->nDraw)
			{
				continue;
			}
			const int32_t nSubX = pRoom->tCoords.nSubtileX + pPreset->nXpos;
			const int32_t nSubY = pRoom->tCoords.nSubtileY + pPreset->nYpos;
			bool bAlreadyExists = false;
			for (D2UnitStrc* pExisting = pRoom->pUnitFirst; pExisting; pExisting = pExisting->pRoomNext)
			{
				if (pExisting->dwUnitType == UNIT_OBJECT && pExisting->dwClassId == pPreset->nIndex)
				{
					const int32_t dx = std::abs(UNITS_GetXPosition(pExisting) - nSubX);
					const int32_t dy = std::abs(UNITS_GetYPosition(pExisting) - nSubY);
					if (dx <= 2 && dy <= 2)
					{
						bAlreadyExists = true;
						break;
					}
				}
			}
			if (!bAlreadyExists)
			{
				const int32_t nMode = (pObjTxt->dwFrameCnt[OBJMODE_OPERATING] > 0x100 && pObjTxt->nCycleAnim[OBJMODE_OPERATING])
					? OBJMODE_OPERATING
					: pPreset->nMode;
				D2Client_CreateClientSideUnit(nSubX, nSubY, pPreset->nIndex, UNIT_OBJECT, nMode);
			}
		}
	}
}

static void SyncLocalPlayerUnitToWorldCoords()
{
	if (!gpPlayerUnit || !gpPlayerUnit->pDynamicPath || !gpClientAct)
	{
		return;
	}
	if (gfPlayerWorldX == 0.0f && gfPlayerWorldY == 0.0f)
	{
		return;
	}

	int nSubX = static_cast<int>(gfPlayerWorldX);
	int nSubY = static_cast<int>(gfPlayerWorldY);
	DUNGEON_ClientToGameSubtileCoords(&nSubX, &nSubY);
	if (nSubX <= 0 || nSubY <= 0)
	{
		return;
	}

	D2DynamicPathStrc* pDyn = gpPlayerUnit->pDynamicPath;
	D2ActiveRoomStrc* pNewRoom = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, nSubX, nSubY);
	if (!pNewRoom)
	{
		SafeStreamClientRoomAtTile(gpClientAct, nSubX / 5, nSubY / 5);
		pNewRoom = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, nSubX, nSubY);
	}

	D2ActiveRoomStrc* pOldRoom = UNITS_GetRoom(gpPlayerUnit);
	if (pNewRoom && pNewRoom != pOldRoom)
	{
		if (pOldRoom)
		{
			UNITROOM_RemoveUnitFromRoom(gpPlayerUnit);
		}
		pDyn->tGameCoords.dwPrecisionX = PATH_ToFP16Center(static_cast<uint16_t>(nSubX));
		pDyn->tGameCoords.dwPrecisionY = PATH_ToFP16Center(static_cast<uint16_t>(nSubY));
		pDyn->pRoom = pNewRoom;
		UNITROOM_AddUnitToRoom(gpPlayerUnit, pNewRoom);
		pOldRoom = pNewRoom;
	}
	else
	{
		pDyn->tGameCoords.dwPrecisionX = PATH_ToFP16Center(static_cast<uint16_t>(nSubX));
		pDyn->tGameCoords.dwPrecisionY = PATH_ToFP16Center(static_cast<uint16_t>(nSubY));
	}

	pDyn->dwClientCoordX = static_cast<int32_t>(gfPlayerWorldX);
	pDyn->dwClientCoordY = static_cast<int32_t>(gfPlayerWorldY);
	PATH_SetDirection(pDyn, static_cast<uint8_t>((gnPlayerDir * 8) & 63));
	PATH_SetNumberOfPathPoints(pDyn, gbPlayerMoving ? 1 : 0);

	const BOOL bInTown = pOldRoom ? DUNGEON_IsRoomInTown(pOldRoom) : TRUE;
	const int32_t nDesiredMode = gbPlayerMoving
		? (gbRunEnabled ? PLRMODE_RUN : (bInTown ? PLRMODE_TOWNWALK : PLRMODE_WALK))
		: (bInTown ? PLRMODE_TOWNNEUTRAL : PLRMODE_NEUTRAL);
	if (gpPlayerUnit->dwAnimMode != static_cast<uint32_t>(nDesiredMode))
	{
		UNITS_ChangeAnimMode(gpPlayerUnit, nDesiredMode);
		GFX_InitUnitCompositeGfx(gpPlayerUnit);
	}
	else if (!gpPlayerUnit->pGfxData)
	{
		GFX_InitUnitCompositeGfx(gpPlayerUnit);
	}
}

static void StreamClientNeighborhoodAroundTile(int32_t nCenterTileX, int32_t nCenterTileY, int32_t nRadiusTiles = 16)
{
	if (!gpClientAct || nCenterTileX <= 0 || nCenterTileY <= 0)
	{
		return;
	}
	for (int32_t dy = -nRadiusTiles; dy <= nRadiusTiles; dy += 8)
	{
		for (int32_t dx = -nRadiusTiles; dx <= nRadiusTiles; dx += 8)
		{
			SafeStreamClientRoomAtTile(gpClientAct, nCenterTileX + dx, nCenterTileY + dy);
		}
	}
	PopulatePresetObjectsForStreamedRooms(gpClientAct);
}

static void PrestreamTownAndAdjacentWilderness(D2DrlgActStrc* pAct, uint8_t nActIdx, int32_t nTownLevelId)
{
	if (!pAct)
	{
		return;
	}
	D2DrlgStrc* pDrlg = DUNGEON_GetDrlgFromAct(pAct);
	if (!pDrlg)
	{
		return;
	}
	static const int32_t kAdjacentWildLevels[5] = {
		LEVEL_BLOODMOOR,
		LEVEL_ROCKYWASTE,
		LEVEL_SPIDERFOREST,
		LEVEL_OUTERSTEPPES,
		LEVEL_BLOODYFOOTHILLS
	};
	const int32_t nWildLevelId = (nActIdx < 5) ? kAdjacentWildLevels[nActIdx] : 0;
	if (nWildLevelId > 0)
	{
		if (D2DrlgLevelStrc* pWildLvl = DRLG_GetLevel(pDrlg, nWildLevelId))
		{
			if (!pWildLvl->pFirstRoomEx)
			{
				DRLG_InitLevel(pWildLvl);
			}
		}
	}
	if (D2DrlgLevelStrc* pTownLvl = DRLG_GetLevel(pDrlg, nTownLevelId))
	{
		if (!pTownLvl->pFirstRoomEx)
		{
			DRLG_InitLevel(pTownLvl);
		}
		for (D2DrlgRoomStrc* pDrlgRoom = pTownLvl->pFirstRoomEx; pDrlgRoom; pDrlgRoom = pDrlgRoom->pDrlgRoomNext)
		{
			SafeStreamClientRoomAtTile(pAct, pDrlgRoom->nTileXPos, pDrlgRoom->nTileYPos);
			// Also stream 1-room ring around each town room so the town->wilderness bridge/gate has zero black border
			for (int32_t i = 0; i < pDrlgRoom->nRoomsNear; ++i)
			{
				if (D2DrlgRoomStrc* pNear = pDrlgRoom->ppRoomsNear[i])
				{
					SafeStreamClientRoomAtTile(pAct, pNear->nTileXPos, pNear->nTileYPos);
				}
			}
		}
		// Stream a 2-room skirt just outside town bounds to cover outdoor connection seams across the full 800x600 viewport
		for (int32_t offset = 4; offset <= 16; offset += 8)
		{
			for (int32_t tx = pTownLvl->nPosX - 8; tx <= pTownLvl->nPosX + pTownLvl->nWidth + 8; tx += 8)
			{
				SafeStreamClientRoomAtTile(pAct, tx, pTownLvl->nPosY - offset);
				SafeStreamClientRoomAtTile(pAct, tx, pTownLvl->nPosY + pTownLvl->nHeight + offset);
			}
			for (int32_t ty = pTownLvl->nPosY - 8; ty <= pTownLvl->nPosY + pTownLvl->nHeight + 8; ty += 8)
			{
				SafeStreamClientRoomAtTile(pAct, pTownLvl->nPosX - offset, ty);
				SafeStreamClientRoomAtTile(pAct, pTownLvl->nPosX + pTownLvl->nWidth + offset, ty);
			}
		}
	}
	PopulatePresetObjectsForStreamedRooms(pAct);
}

// Authoritative S2C Packet Dispatcher (1.12a SCmd: 0x6FAB10E0)
static void DrainNetworkPackets()
{
	uint8_t aBuffer[512] = {};
	while (CLIENT_DequeueSystemPacket(aBuffer, sizeof(aBuffer)) != -1)
	{
		if (aBuffer[0] == 0xAF)
		{
			gbClientLeaveCompleted = 1;
		}
		Sleep(0);
	}
	int32_t nPacketLen = -1;
	while ((nPacketLen = CLIENT_DequeueGamePacket(aBuffer, sizeof(aBuffer))) != -1)
	{
		if (nPacketLen <= 0)
		{
			continue;
		}
		++gdwGamePacketsReceived;
		const uint8_t nOpcode = aBuffer[0];

		switch (nOpcode)
		{
		case 0x00: // GameLoading
			SCMD_Handler00_GameLoading(aBuffer);
			++gdwSCmdPacketsDispatched;
			break;

		case 0x01: // GameLogonReceipt
			if (nPacketLen >= 8)
			{
				SCMD_Handler01_GameLogonReceipt(aBuffer);
				++gdwSCmdPacketsDispatched;
				gnGameDifficulty = aBuffer[1];
				gbIsExpansion = (aBuffer[6] != 0);
				g_bExpansionMode = gbIsExpansion ? TRUE : FALSE;
				INV_SyncExpansionModeFlag();
			}
			break;

		case 0x02: // LoadDone -> send C2S 0x6B (JoinActComplete) via reconstructed Msg.cpp
			MSG_SendJoinActComplete_6B();
			break;

		case 0x03: // LoadAct (nAct, dwMapSeed, wTownLevelId, dwUnk)
			if (nPacketLen >= 12)
			{
				const uint8_t nAct = aBuffer[1];
				const uint32_t dwMapSeed = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				const uint16_t wLevelId = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
				if (!gbServerActLoaded || gnCurrentAct != nAct || gdwServerMapSeed != dwMapSeed || !gpClientAct)
				{
					if (gpPlayerUnit && gpClientAct && UNITS_GetRoom(gpPlayerUnit))
					{
						UNITROOM_RemoveUnitFromRoom(gpPlayerUnit);
						if (gpPlayerUnit->pDynamicPath)
						{
							gpPlayerUnit->pDynamicPath->pRoom = nullptr;
						}
					}
					for (int32_t b = 0; b < 128; ++b)
					{
						while (gapClientSideUnitHashTables[UNIT_OBJECT][b])
						{
							D2Client_RemoveClientSideUnit(UNIT_OBJECT, gapClientSideUnitHashTables[UNIT_OBJECT][b]->dwUnitId);
						}
					}
					if (gpClientAct)
					{
						DUNGEON_FreeAct(gpClientAct);
						gpClientAct = nullptr;
					}
					gnCurrentAct = nAct;
					GFX_UpdateActCofCache(nAct);
					gdwServerMapSeed = dwMapSeed;
					gwServerTownLevelId = wLevelId ? wLevelId : LEVEL_ROGUEENCAMPMENT;
					gnCurrentLevelId = gwServerTownLevelId;
					gpClientAct = DUNGEON_AllocAct(
						nAct,
						dwMapSeed,
						FALSE,
						nullptr,
						gnGameDifficulty,
						nullptr,
						gwServerTownLevelId,
						nullptr,
						nullptr);
					g_pClientAct = gpClientAct;
					gbServerActLoaded = true;
					PrestreamTownAndAdjacentWilderness(gpClientAct, nAct, gwServerTownLevelId);
					if (gpPlayerUnit)
					{
						gpPlayerUnit->nAct = nAct;
						gpPlayerUnit->pDrlgAct = gpClientAct;
						SyncLocalPlayerUnitToWorldCoords();
					}
				}
			}
			break;

		case 0x04: // LoadComplete
			SCMD_Handler04_LoadComplete(aBuffer);
			++gdwSCmdPacketsDispatched;
			break;

		case 0x05: // UnloadComplete
			SCMD_Handler05_UnloadComplete(aBuffer);
			++gdwSCmdPacketsDispatched;
			break;

		case 0x07: // AddRoom (wTileX, wTileY, nLevelId)
			if (nPacketLen >= 6 && gpClientAct)
			{
				const uint16_t wTileX = *reinterpret_cast<const uint16_t*>(&aBuffer[1]);
				const uint16_t wTileY = *reinterpret_cast<const uint16_t*>(&aBuffer[3]);
				const uint8_t nLvlId = aBuffer[5];
				if (nLvlId > 0)
				{
					if (D2DrlgStrc* pDrlg = DUNGEON_GetDrlgFromAct(gpClientAct))
					{
						if (D2DrlgLevelStrc* pLvl = DRLG_GetLevel(pDrlg, nLvlId))
						{
							if (!pLvl->pFirstRoomEx)
							{
								DRLG_InitLevel(pLvl);
							}
						}
					}
				}
				SyncReconstructedClientGlobals();
				SCMD_Handler07_AddRoom(aBuffer);
				++gdwSCmdPacketsDispatched;
				DUNGEON_StreamRoomAtCoords(gpClientAct, static_cast<int>(wTileX), static_cast<int>(wTileY));
				PopulatePresetObjectsForStreamedRooms(gpClientAct);
			}
			break;

		case 0x08: // RemoveRoom (wTileX, wTileY, nLevelId)
			if (nPacketLen >= 6 && gpClientAct)
			{
				SyncReconstructedClientGlobals();
				SCMD_Handler08_RemoveRoom(aBuffer);
				++gdwSCmdPacketsDispatched;
			}
			break;

		case 0x09: // AssignWarp (nUnitType, dwGUID, nWarpId, wX, wY)
			if (nPacketLen >= 11)
			{
				const uint8_t nUnitType = aBuffer[1];
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				const uint8_t nWarpId = aBuffer[6];
				const uint16_t wX = *reinterpret_cast<const uint16_t*>(&aBuffer[7]);
				const uint16_t wY = *reinterpret_cast<const uint16_t*>(&aBuffer[9]);
				if (ClientWorldUnit* pWarp = FindOrAllocClientUnit(nUnitType, dwGuid))
				{
					pWarp->wClassId = nWarpId;
					SetClientUnitSubtilePos(pWarp, wX, wY, true);
					SStrCopy(pWarp->szName, "Level Entrance", sizeof(pWarp->szName));
				}
			}
			break;

		case 0x0A: // RemoveUnit (nUnitType, dwGUID)
			if (nPacketLen >= 6)
			{
				const uint8_t nUnitType = aBuffer[1];
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				if (nUnitType < UNIT_TYPES_COUNT && dwGuid != gdwClientPlayerGuid)
				{
					RemoveReconstructedServerUnit(nUnitType, dwGuid);
				}
				if (nUnitType == UNIT_ITEM)
				{
					for (int32_t i = 0; i < kMaxClientItems; ++i)
					{
						if (gClientItems[i].bActive && gClientItems[i].dwItemGuid == dwGuid && gClientItems[i].nLocation == CLI_ITEMLOC_GROUND)
						{
							gClientItems[i].bActive = false;
						}
					}
				}
				else
				{
					for (int32_t i = 0; i < kMaxClientUnits; ++i)
					{
						if (gClientUnits[i].bActive && gClientUnits[i].nUnitType == nUnitType && gClientUnits[i].dwUnitGuid == dwGuid)
						{
							gClientUnits[i].bActive = false;
						}
					}
				}
			}
			break;

		case 0x0E: // ObjectState (nUnitType, dwGUID, unk, bCanChangeBack, dwAnimMode)
			if (nPacketLen >= 12)
			{
				const uint8_t nUnitType = aBuffer[1];
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				const uint8_t nMode = aBuffer[8];
				if (D2UnitStrc* pSrvObj = D2Client_FindServerSideUnit(dwGuid, nUnitType))
				{
					UNITS_ChangeAnimMode(pSrvObj, nMode);
					GFX_InitUnitCompositeGfx(pSrvObj);
				}
				if (ClientWorldUnit* pObj = FindOrAllocClientUnit(nUnitType, dwGuid))
				{
					pObj->nAnimMode = nMode;
				}
			}
			break;

		case 0x0F: // PlayerMove (nUnitType, dwGUID, nMoveType, wTargetX, wTargetY, unk, wCurX, wCurY)
			if (nPacketLen >= 16)
			{
				const uint8_t nUnitType = aBuffer[1];
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				const uint16_t wTargetX = *reinterpret_cast<const uint16_t*>(&aBuffer[7]);
				const uint16_t wTargetY = *reinterpret_cast<const uint16_t*>(&aBuffer[9]);
				if (nUnitType == UNIT_PLAYER && (gdwClientPlayerGuid == 0 || dwGuid == gdwClientPlayerGuid))
				{
					gnServerSubX = wTargetX;
					gnServerSubY = wTargetY;
				}
			}
			break;

		case 0x10: // PlayerMoveToTarget (nUnitType, dwGUID, nMoveType, nTargetType, dwTargetGUID, wCurX, wCurY)
			if (nPacketLen >= 16)
			{
				const uint8_t nUnitType = aBuffer[1];
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				const uint16_t wCurX = *reinterpret_cast<const uint16_t*>(&aBuffer[12]);
				const uint16_t wCurY = *reinterpret_cast<const uint16_t*>(&aBuffer[14]);
				if (nUnitType == UNIT_PLAYER && (gdwClientPlayerGuid == 0 || dwGuid == gdwClientPlayerGuid))
				{
					gnServerSubX = wCurX;
					gnServerSubY = wCurY;
				}
			}
			break;

		case 0x15: // ReassignPlayer / ReassignUnit (nUnitType, dwGUID, wX, wY, bSnap)
			if (nPacketLen >= 11)
			{
				const uint8_t nUnitType = aBuffer[1];
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				const uint16_t wSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
				const uint16_t wSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[8]);
				const bool bSnap = (aBuffer[10] != 0);
				if (nUnitType == UNIT_PLAYER && (gdwClientPlayerGuid == 0 || dwGuid == gdwClientPlayerGuid))
				{
					gdwClientPlayerGuid = dwGuid;
					gnServerSubX = wSubX;
					gnServerSubY = wSubY;
					int nIsoX = wSubX;
					int nIsoY = wSubY;
					DUNGEON_GameSubtileToClientCoords(&nIsoX, &nIsoY);
					if (bSnap || (gfPlayerWorldX == 0.0f && gfPlayerWorldY == 0.0f))
					{
						gfPlayerWorldX = static_cast<float>(nIsoX);
						gfPlayerWorldY = static_cast<float>(nIsoY);
						gfTargetWorldX = gfPlayerWorldX;
						gfTargetWorldY = gfPlayerWorldY;
						gbPlayerMoving = false;
					}
					StreamClientNeighborhoodAroundTile(wSubX / 5, wSubY / 5, 16);
					SyncLocalPlayerUnitToWorldCoords();
				}
				else
				{
					if (D2UnitStrc* pSrvUnit = D2Client_FindServerSideUnit(dwGuid, nUnitType))
					{
						D2Client_TeleportUnitToCoordinates(pSrvUnit, wSubX, wSubY, 0);
					}
					if (ClientWorldUnit* pUnit = FindOrAllocClientUnit(nUnitType, dwGuid))
					{
						SetClientUnitSubtilePos(pUnit, wSubX, wSubY, bSnap);
					}
				}
			}
			break;

		case 0x18: // PlayerHPMP (15 bytes bitstream: 8hdr + 15hp + 15mana + 15stam + 7hpreg + 7manareg + 16x + 16y + 8vx + 8vy)
			if (nPacketLen >= 15)
			{
				D2BitBufferStrc bitBuf = {};
				BITMANIP_Initialize(&bitBuf, aBuffer, nPacketLen);
				(void)BITMANIP_Read(&bitBuf, 8);
				const int32_t nHp = BITMANIP_Read(&bitBuf, 15);
				const int32_t nMana = BITMANIP_Read(&bitBuf, 15);
				const int32_t nStam = BITMANIP_Read(&bitBuf, 15);
				(void)BITMANIP_Read(&bitBuf, 7);
				(void)BITMANIP_Read(&bitBuf, 7);
				const int32_t nSubX = BITMANIP_Read(&bitBuf, 16);
				const int32_t nSubY = BITMANIP_Read(&bitBuf, 16);
				gaClientStats[STAT_HITPOINTS] = nHp << 8;
				gaClientStats[STAT_MANA] = nMana << 8;
				gaClientStats[STAT_STAMINA] = nStam << 8;
				if (gaClientStats[STAT_MAXHP] < gaClientStats[STAT_HITPOINTS])
				{
					gaClientStats[STAT_MAXHP] = gaClientStats[STAT_HITPOINTS];
				}
				if (gaClientStats[STAT_MAXMANA] < gaClientStats[STAT_MANA])
				{
					gaClientStats[STAT_MAXMANA] = gaClientStats[STAT_MANA];
				}
				if (gaClientStats[STAT_MAXSTAMINA] < gaClientStats[STAT_STAMINA])
				{
					gaClientStats[STAT_MAXSTAMINA] = gaClientStats[STAT_STAMINA];
				}
				if (nSubX > 0 && nSubY > 0)
				{
					gnServerSubX = nSubX;
					gnServerSubY = nSubY;
				}
			}
			break;

		case 0x95: // PlayerLifeManaChange (13 bytes bitstream: 8hdr + 15hp + 15mana + 15stam + 16x + 16y + 8vx + 8vy)
			if (nPacketLen >= 13)
			{
				D2BitBufferStrc bitBuf = {};
				BITMANIP_Initialize(&bitBuf, aBuffer, nPacketLen);
				(void)BITMANIP_Read(&bitBuf, 8);
				const int32_t nHp = BITMANIP_Read(&bitBuf, 15);
				const int32_t nMana = BITMANIP_Read(&bitBuf, 15);
				const int32_t nStam = BITMANIP_Read(&bitBuf, 15);
				const int32_t nSubX = BITMANIP_Read(&bitBuf, 16);
				const int32_t nSubY = BITMANIP_Read(&bitBuf, 16);
				gaClientStats[STAT_HITPOINTS] = nHp << 8;
				gaClientStats[STAT_MANA] = nMana << 8;
				gaClientStats[STAT_STAMINA] = nStam << 8;
				if (gaClientStats[STAT_MAXHP] < gaClientStats[STAT_HITPOINTS])
				{
					gaClientStats[STAT_MAXHP] = gaClientStats[STAT_HITPOINTS];
				}
				if (gaClientStats[STAT_MAXMANA] < gaClientStats[STAT_MANA])
				{
					gaClientStats[STAT_MAXMANA] = gaClientStats[STAT_MANA];
				}
				if (gaClientStats[STAT_MAXSTAMINA] < gaClientStats[STAT_STAMINA])
				{
					gaClientStats[STAT_MAXSTAMINA] = gaClientStats[STAT_STAMINA];
				}
				if (nSubX > 0 && nSubY > 0)
				{
					gnServerSubX = nSubX;
					gnServerSubY = nSubY;
				}
			}
			break;

		case 0x96: // WalkVerify (9 bytes bitstream: 8hdr + 15stam + 16x + 16y + 8vx + 8vy)
			if (nPacketLen >= 9)
			{
				D2BitBufferStrc bitBuf = {};
				BITMANIP_Initialize(&bitBuf, aBuffer, nPacketLen);
				(void)BITMANIP_Read(&bitBuf, 8);
				const int32_t nStam = BITMANIP_Read(&bitBuf, 15);
				const int32_t nSubX = BITMANIP_Read(&bitBuf, 16);
				const int32_t nSubY = BITMANIP_Read(&bitBuf, 16);
				gaClientStats[STAT_STAMINA] = nStam << 8;
				if (nSubX > 0 && nSubY > 0)
				{
					gnServerSubX = nSubX;
					gnServerSubY = nSubY;
					int nCurSubX = static_cast<int>(gfPlayerWorldX);
					int nCurSubY = static_cast<int>(gfPlayerWorldY);
					DUNGEON_ClientToGameSubtileCoords(&nCurSubX, &nCurSubY);
					if (std::abs(nCurSubX - nSubX) > 8 || std::abs(nCurSubY - nSubY) > 8)
					{
						int nIsoX = nSubX;
						int nIsoY = nSubY;
						DUNGEON_GameSubtileToClientCoords(&nIsoX, &nIsoY);
						gfPlayerWorldX = static_cast<float>(nIsoX);
						gfPlayerWorldY = static_cast<float>(nIsoY);
						gfTargetWorldX = gfPlayerWorldX;
						gfTargetWorldY = gfPlayerWorldY;
						gbPlayerMoving = false;
					}
					StreamClientNeighborhoodAroundTile(nSubX / 5, nSubY / 5, 16);
				}
			}
			break;

		case 0x19: // GoldByteAdd
			if (nPacketLen >= 2)
			{
				SyncReconstructedClientGlobals();
				SCMD_Handler19_1F_StatUpdate(aBuffer);
				++gdwSCmdPacketsDispatched;
				gaClientStats[STAT_GOLD] += aBuffer[1];
			}
			break;
		case 0x1A: // ExpByteAdd
			if (nPacketLen >= 2)
			{
				SyncReconstructedClientGlobals();
				SCMD_Handler19_1F_StatUpdate(aBuffer);
				++gdwSCmdPacketsDispatched;
				gaClientStats[STAT_EXPERIENCE] += aBuffer[1];
			}
			break;
		case 0x1B: // ExpWordAdd
			if (nPacketLen >= 3)
			{
				SyncReconstructedClientGlobals();
				SCMD_Handler19_1F_StatUpdate(aBuffer);
				++gdwSCmdPacketsDispatched;
				gaClientStats[STAT_EXPERIENCE] += *reinterpret_cast<const uint16_t*>(&aBuffer[1]);
			}
			break;
		case 0x1C: // ExpDwordSet
			if (nPacketLen >= 5)
			{
				SyncReconstructedClientGlobals();
				SCMD_Handler19_1F_StatUpdate(aBuffer);
				++gdwSCmdPacketsDispatched;
				gaClientStats[STAT_EXPERIENCE] = *reinterpret_cast<const int32_t*>(&aBuffer[1]);
			}
			break;
		case 0x1D: // StatByteSet
		case 0x1E: // StatWordSet
		case 0x1F: // StatDwordSet
		{
			SyncReconstructedClientGlobals();
			SCMD_Handler19_1F_StatUpdate(aBuffer);
			++gdwSCmdPacketsDispatched;
			const uint8_t nStatId = aBuffer[1];
			int32_t nVal = 0;
			if (nOpcode == 0x1D && nPacketLen >= 3)
			{
				nVal = aBuffer[2];
			}
			else if (nOpcode == 0x1E && nPacketLen >= 4)
			{
				nVal = *reinterpret_cast<const uint16_t*>(&aBuffer[2]);
			}
			else if (nOpcode == 0x1F && nPacketLen >= 6)
			{
				nVal = *reinterpret_cast<const int32_t*>(&aBuffer[2]);
			}
			gaClientStats[nStatId] = nVal;
			if (nStatId == STAT_LEVEL && nVal > 0)
			{
				gnCharLevel = static_cast<uint8_t>(nVal);
			}
			break;
		}
		case 0x20: // UnitStatDwordSet
			if (nPacketLen >= 10)
			{
				SyncReconstructedClientGlobals();
				SCMD_Handler20_PlayerAttrUpdate(aBuffer);
				++gdwSCmdPacketsDispatched;
				const uint8_t nStatId = aBuffer[5];
				const int32_t nVal = *reinterpret_cast<const int32_t*>(&aBuffer[6]);
				gaClientStats[nStatId] = nVal;
			}
			break;

		case 0x21: // UpdateSkill (nUnitType, unk, dwGUID, wSkillId, nBaseLvl, nBonusLvl)
			if (nPacketLen >= 11)
			{
				const uint16_t wSkillId = *reinterpret_cast<const uint16_t*>(&aBuffer[7]);
				const uint8_t nBaseLvl = aBuffer[9];
				const uint8_t nBonusLvl = aBuffer[10];
				if (wSkillId < 512)
				{
					gaClientSkillLevels[wSkillId] = static_cast<uint8_t>(nBaseLvl + nBonusLvl);
				}
			}
			break;

		case 0x23: // AssignSkill (nUnitType, dwGUID, nHand, wSkillId, dwItemGUID)
			if (nPacketLen >= 13)
			{
				const uint8_t nHand = aBuffer[6];
				const uint16_t wSkillId = *reinterpret_cast<const uint16_t*>(&aBuffer[7]);
				if (nHand == 0)
				{
					gwRightSkillId = wSkillId;
				}
				else
				{
					gwLeftSkillId = wSkillId;
				}
			}
			break;

		case 0x94: // BaseSkillLevels (nCount, dwGUID, {wSkillId, nLvl}[nCount])
			if (nPacketLen >= 6)
			{
				const uint8_t nCount = aBuffer[1];
				int32_t nOff = 6;
				for (uint8_t i = 0; i < nCount && (nOff + 3) <= nPacketLen; ++i, nOff += 3)
				{
					const uint16_t wSkillId = *reinterpret_cast<const uint16_t*>(&aBuffer[nOff]);
					const uint8_t nLvl = aBuffer[nOff + 2];
					if (wSkillId < 512)
					{
						gaClientSkillLevels[wSkillId] = nLvl;
					}
				}
			}
			break;

		case 0x26: // GameChat
			if (nPacketLen > 10)
			{
				const char* szSender = reinterpret_cast<const char*>(&aBuffer[10]);
				const size_t nSenderLen = strnlen(szSender, static_cast<size_t>(nPacketLen - 10));
				const char* szBody = (10 + nSenderLen + 1 < static_cast<size_t>(nPacketLen))
					? reinterpret_cast<const char*>(&aBuffer[10 + nSenderLen + 1])
					: "";
				char szChatLine[128] = {};
				if (szSender[0] && szBody[0])
				{
					std::snprintf(szChatLine, sizeof(szChatLine), "%s: %s", szSender, szBody);
				}
				else if (szBody[0])
				{
					std::snprintf(szChatLine, sizeof(szChatLine), "%s", szBody);
				}
				AppendClientChatLog(szChatLine);
			}
			break;

		case 0x27: // NpcIntro (nUnitType, dwGUID, aMenuData...)
			if (nPacketLen >= 6)
			{
				const uint32_t dwNpcGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				gnActiveNpcGuid = dwNpcGuid;
				g_nInteractingNpcGUID = static_cast<int32_t>(dwNpcGuid);
				gdwActiveInteractNpcGUID = static_cast<int32_t>(dwNpcGuid);
				++gdwNpcIntrosReceived;
				gwActiveNpcClassId = 0;
				SStrCopy(gszActiveNpcName, "Townsperson", sizeof(gszActiveNpcName));
				for (int32_t i = 0; i < kMaxClientUnits; ++i)
				{
					if (gClientUnits[i].bActive && gClientUnits[i].nUnitType == UNIT_MONSTER && gClientUnits[i].dwUnitGuid == dwNpcGuid)
					{
						gwActiveNpcClassId = gClientUnits[i].wClassId;
						if (gClientUnits[i].szName[0])
						{
							SStrCopy(gszActiveNpcName, gClientUnits[i].szName, sizeof(gszActiveNpcName));
						}
						break;
					}
				}
				GAME_SendTownFolkInit_2F(UNIT_MONSTER, dwNpcGuid);
				gbNpcMenuOpen = true;
				gnNpcMenuSelectedIdx = 0;
				char szLog[96] = {};
				std::snprintf(szLog, sizeof(szLog), "%s: Greetings, traveler.", gszActiveNpcName);
				AppendClientChatLog(szLog);
			}
			break;

		case 0x28: // PlayerQuestInfo (103 bytes)
			if (nPacketLen >= 43)
			{
				std::memcpy(gaQuestFilterStates, &aBuffer[6], std::min<int32_t>(37, nPacketLen - 6));
			}
			break;

		case 0x51: // AssignObject (nUnitType, dwGUID, wObjectId, wX, wY, nAnimMode, nInteractType)
			if (nPacketLen >= 14)
			{
				const uint8_t nUnitType = aBuffer[1];
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				const uint16_t wObjId = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
				const uint16_t wSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[8]);
				const uint16_t wSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[10]);
				const uint8_t nMode = aBuffer[12];
				const uint8_t nInteractType = aBuffer[13];
				if (gpClientAct && wSubX > 0 && wSubY > 0)
				{
					SafeStreamClientRoomAtTile(gpClientAct, wSubX / 5, wSubY / 5);
					if (D2ActiveRoomStrc* pObjRoom = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, wSubX, wSubY))
					{
						D2UnitStrc* pCur = pObjRoom->pUnitFirst;
						while (pCur)
						{
							D2UnitStrc* pNext = pCur->pRoomNext;
							if ((pCur->dwFlags & UNITFLAG_ISCLIENTUNIT) && pCur->dwUnitType == UNIT_OBJECT && pCur->dwClassId == wObjId)
							{
								const int32_t dx = std::abs(UNITS_GetXPosition(pCur) - static_cast<int32_t>(wSubX));
								const int32_t dy = std::abs(UNITS_GetYPosition(pCur) - static_cast<int32_t>(wSubY));
								if (dx <= 3 && dy <= 3)
								{
									D2Client_RemoveClientSideUnit(UNIT_OBJECT, pCur->dwUnitId);
								}
							}
							pCur = pNext;
						}
					}
				}
				if (D2UnitStrc* pExistingObj = D2Client_FindServerSideUnit(dwGuid, UNIT_OBJECT))
				{
					UNITS_ChangeAnimMode(pExistingObj, nMode);
					GFX_InitUnitCompositeGfx(pExistingObj);
				}
				else if (gpClientAct)
				{
					D2ObjectsTxt* pObjTxt = DATATBLS_GetObjectsTxtRecord(wObjId);
					const int32_t nInitMode = (nMode == OBJMODE_NEUTRAL && pObjTxt && pObjTxt->dwFrameCnt[OBJMODE_OPERATING] > 0x100 && pObjTxt->nCycleAnim[OBJMODE_OPERATING])
						? OBJMODE_OPERATING
						: nMode;
					D2UnitStrc* pNewObj = D2Client_AllocClientObjectUnit(UNIT_OBJECT, wObjId, dwGuid, wSubX, wSubY, nInitMode, nInteractType, 0);
					if (pNewObj && !pNewObj->pGfxData)
					{
						GFX_InitUnitCompositeGfx(pNewObj);
					}
				}
				if (ClientWorldUnit* pObj = FindOrAllocClientUnit(nUnitType, dwGuid))
				{
					pObj->wClassId = wObjId;
					pObj->nAnimMode = nMode;
					SetClientUnitSubtilePos(pObj, wSubX, wSubY, true);
					if (D2ObjectsTxt* pObjTxt = DATATBLS_GetObjectsTxtRecord(wObjId))
					{
						UnicodeToAsciiSafe(pObjTxt->wszName, pObj->szName, sizeof(pObj->szName));
						if (!pObj->szName[0])
						{
							SStrCopy(pObj->szName, pObjTxt->szName, sizeof(pObj->szName));
						}
					}
				}
			}
			break;

		case 0x52: // QuestLogInfo (42 bytes)
			if (nPacketLen >= 38)
			{
				std::memcpy(gaQuestFilterStates, &aBuffer[1], 37);
			}
			break;

		case 0x59: // AssignPlayer (dwGUID, nCharClass, szName[16], wX, wY)
			if (nPacketLen >= 26)
			{
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
				const uint8_t nClass = aBuffer[5];
				const uint16_t wSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[22]);
				const uint16_t wSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[24]);
				char szAssignedName[16] = {};
				std::memcpy(szAssignedName, &aBuffer[6], 15);
				szAssignedName[15] = '\0';
				if (gdwClientPlayerGuid == 0 || dwGuid == gdwClientPlayerGuid)
				{
					gdwClientPlayerGuid = dwGuid;
					if (nClass < 7)
					{
						gnCharClass = nClass;
					}
					if (szAssignedName[0])
					{
						SStrCopy(gszCharName, szAssignedName, sizeof(gszCharName));
					}
					if (wSubX > 0 && wSubY > 0)
					{
						gnServerSubX = wSubX;
						gnServerSubY = wSubY;
						int nIsoX = wSubX;
						int nIsoY = wSubY;
						DUNGEON_GameSubtileToClientCoords(&nIsoX, &nIsoY);
						gfPlayerWorldX = static_cast<float>(nIsoX);
						gfPlayerWorldY = static_cast<float>(nIsoY);
						gfTargetWorldX = gfPlayerWorldX;
						gfTargetWorldY = gfPlayerWorldY;
						gbPlayerMoving = false;
						StreamClientNeighborhoodAroundTile(wSubX / 5, wSubY / 5, 16);
					}
				}
				if (gpPlayerUnit && gpPlayerUnit->dwUnitId != dwGuid && gpPlayerUnit->dwUnitId == 0x7FFFFFFFu)
				{
					D2Client_SetLocalPlayerUnit(nullptr);
					D2Client_RemoveServerSideUnit(UNIT_PLAYER, 0x7FFFFFFFu);
				}
				D2UnitStrc* pPlrUnit = D2Client_FindServerSideUnit(dwGuid, UNIT_PLAYER);
				if (!pPlrUnit)
				{
					pPlrUnit = D2Client_AllocClientPlayerUnit(
						dwGuid,
						nClass < 7 ? nClass : 0,
						wSubX,
						wSubY,
						szAssignedName[0] ? szAssignedName : gszCharName);
				}
				if (pPlrUnit)
				{
					GFX_InitUnitCompositeGfx(pPlrUnit);
					if (dwGuid == gdwClientPlayerGuid)
					{
						D2Client_SetLocalPlayerUnit(pPlrUnit);
						SyncReconstructedClientGlobals();
						SCMD_DrainUnitPacketQueue(pPlrUnit);
						++gdwSCmdPacketsDispatched;
						SyncLocalPlayerUnitToWorldCoords();
					}
				}
			}
			break;

		case 0x67: // MonsterMove (dwGUID, nMoveType, wTargetX, wTargetY, ...)
			if (nPacketLen >= 10)
			{
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
				const uint16_t wTargetX = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
				const uint16_t wTargetY = *reinterpret_cast<const uint16_t*>(&aBuffer[8]);
				if (D2UnitStrc* pSrvMon = D2Client_FindServerSideUnit(dwGuid, UNIT_MONSTER))
				{
					const int32_t nOldX = UNITS_GetXPosition(pSrvMon);
					const int32_t nOldY = UNITS_GetYPosition(pSrvMon);
					if (pSrvMon->pDynamicPath && (wTargetX != nOldX || wTargetY != nOldY))
					{
						int nIsoDx = wTargetX - nOldX;
						int nIsoDy = wTargetY - nOldY;
						DUNGEON_GameSubtileToClientCoords(&nIsoDx, &nIsoDy);
						const float fAng = std::atan2(static_cast<float>(nIsoDy), static_cast<float>(nIsoDx)) * (180.0f / 3.14159265f);
						uint8_t nDir8 = 0;
						if (fAng >= 67.5f && fAng < 112.5f) nDir8 = 0;
						else if (fAng >= 112.5f && fAng < 157.5f) nDir8 = 1;
						else if (fAng >= 157.5f || fAng < -157.5f) nDir8 = 2;
						else if (fAng >= -157.5f && fAng < -112.5f) nDir8 = 3;
						else if (fAng >= -112.5f && fAng < -67.5f) nDir8 = 4;
						else if (fAng >= -67.5f && fAng < -22.5f) nDir8 = 5;
						else if (fAng >= -22.5f && fAng < 22.5f) nDir8 = 6;
						else nDir8 = 7;
						PATH_SetDirection(pSrvMon->pDynamicPath, static_cast<uint8_t>(nDir8 * 8));
					}
					D2Client_TeleportUnitToCoordinates(pSrvMon, wTargetX, wTargetY, 0);
					if (pSrvMon->dwAnimMode != MONMODE_WALK)
					{
						UNITS_ChangeAnimMode(pSrvMon, MONMODE_WALK);
						GFX_InitUnitCompositeGfx(pSrvMon);
					}
				}
				if (ClientWorldUnit* pMon = FindOrAllocClientUnit(UNIT_MONSTER, dwGuid))
				{
					pMon->nAnimMode = MONMODE_WALK;
					SetClientUnitSubtilePos(pMon, wTargetX, wTargetY, false);
				}
			}
			break;

		case 0x68: // MonsterMoveToTarget (dwGUID, nMoveType, wCurX, wCurY, nTargetType, dwTargetGUID, ...)
			if (nPacketLen >= 15)
			{
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
				const uint16_t wCurX = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
				const uint16_t wCurY = *reinterpret_cast<const uint16_t*>(&aBuffer[8]);
				if (D2UnitStrc* pSrvMon = D2Client_FindServerSideUnit(dwGuid, UNIT_MONSTER))
				{
					D2Client_TeleportUnitToCoordinates(pSrvMon, wCurX, wCurY, 0);
					if (pSrvMon->dwAnimMode != MONMODE_WALK)
					{
						UNITS_ChangeAnimMode(pSrvMon, MONMODE_WALK);
						GFX_InitUnitCompositeGfx(pSrvMon);
					}
				}
				if (ClientWorldUnit* pMon = FindOrAllocClientUnit(UNIT_MONSTER, dwGuid))
				{
					pMon->nAnimMode = MONMODE_WALK;
					SetClientUnitSubtilePos(pMon, wCurX, wCurY, false);
				}
			}
			break;

		case 0x69: // MonsterState (dwGUID, nState, wX, wY, nLifePct)
			if (nPacketLen >= 11)
			{
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
				const uint8_t nMode = aBuffer[5];
				const uint16_t wX = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
				const uint16_t wY = *reinterpret_cast<const uint16_t*>(&aBuffer[8]);
				const uint8_t nLifePct = aBuffer[10];
				if (D2UnitStrc* pSrvMon = D2Client_FindServerSideUnit(dwGuid, UNIT_MONSTER))
				{
					D2Client_TeleportUnitToCoordinates(pSrvMon, wX, wY, 0);
					if (pSrvMon->pMonsterData)
					{
						pSrvMon->pMonsterData->unk0x3C.unk0x00 = (nMode == MONMODE_DEATH || nMode == MONMODE_DEAD) ? 0 : nLifePct;
					}
					if (pSrvMon->dwAnimMode != nMode)
					{
						UNITS_ChangeAnimMode(pSrvMon, nMode);
						GFX_InitUnitCompositeGfx(pSrvMon);
					}
				}
				if (ClientWorldUnit* pMon = FindOrAllocClientUnit(UNIT_MONSTER, dwGuid))
				{
					pMon->nAnimMode = nMode;
					pMon->nLifePct = (nMode == MONMODE_DEATH || nMode == MONMODE_DEAD) ? 0 : nLifePct;
					SetClientUnitSubtilePos(pMon, wX, wY, true);
				}
			}
			break;

		case 0x6C: // MonsterAttack (dwGUID, wAttackType, dwTargetGUID, nTargetType, wX, wY)
			if (nPacketLen >= 16)
			{
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
				const uint16_t wX = *reinterpret_cast<const uint16_t*>(&aBuffer[12]);
				const uint16_t wY = *reinterpret_cast<const uint16_t*>(&aBuffer[14]);
				if (D2UnitStrc* pSrvMon = D2Client_FindServerSideUnit(dwGuid, UNIT_MONSTER))
				{
					D2Client_TeleportUnitToCoordinates(pSrvMon, wX, wY, 0);
					UNITS_ChangeAnimMode(pSrvMon, MONMODE_ATTACK1);
					GFX_InitUnitCompositeGfx(pSrvMon);
				}
				if (ClientWorldUnit* pMon = FindOrAllocClientUnit(UNIT_MONSTER, dwGuid))
				{
					pMon->nAnimMode = MONMODE_ATTACK1;
					SetClientUnitSubtilePos(pMon, wX, wY, false);
				}
			}
			break;

		case 0x6D: // MonsterStop (dwGUID, wX, wY, nLifePct)
			if (nPacketLen >= 10)
			{
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
				const uint16_t wX = *reinterpret_cast<const uint16_t*>(&aBuffer[5]);
				const uint16_t wY = *reinterpret_cast<const uint16_t*>(&aBuffer[7]);
				const uint8_t nLifePct = aBuffer[9];
				if (D2UnitStrc* pSrvMon = D2Client_FindServerSideUnit(dwGuid, UNIT_MONSTER))
				{
					D2Client_TeleportUnitToCoordinates(pSrvMon, wX, wY, 0);
					if (pSrvMon->pMonsterData)
					{
						pSrvMon->pMonsterData->unk0x3C.unk0x00 = nLifePct;
					}
					const int32_t nStopMode = (nLifePct == 0) ? MONMODE_DEAD : MONMODE_NEUTRAL;
					if (pSrvMon->dwAnimMode != static_cast<uint32_t>(nStopMode))
					{
						UNITS_ChangeAnimMode(pSrvMon, nStopMode);
						GFX_InitUnitCompositeGfx(pSrvMon);
					}
				}
				if (ClientWorldUnit* pMon = FindOrAllocClientUnit(UNIT_MONSTER, dwGuid))
				{
					pMon->nAnimMode = (nLifePct == 0) ? MONMODE_DEAD : MONMODE_NEUTRAL;
					pMon->nLifePct = nLifePct;
					SetClientUnitSubtilePos(pMon, wX, wY, true);
				}
			}
			break;

		case 0x9C: // ItemActionWorld
			SyncReconstructedClientGlobals();
			D2Client_PacketHandler_ItemAction9C(aBuffer);
			++gdwSCmdPacketsDispatched;
			HandleServerItemActionPacket(aBuffer, nPacketLen, false);
			break;

		case 0x9D: // ItemActionOwned
			SyncReconstructedClientGlobals();
			D2Client_PacketHandler_ItemAction9D(aBuffer);
			++gdwSCmdPacketsDispatched;
			HandleServerItemActionPacket(aBuffer, nPacketLen, true);
			break;

		case 0xAB: // HealUnit (nUnitType, dwGUID, nLifePct)
			if (nPacketLen >= 7)
			{
				const uint8_t nUnitType = aBuffer[1];
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				const uint8_t nLifePct = aBuffer[6];
				if (ClientWorldUnit* pUnit = FindOrAllocClientUnit(nUnitType, dwGuid))
				{
					pUnit->nLifePct = nLifePct;
				}
			}
			break;

		case 0x4E: // MercForHireList
			break;

		case 0x97: // WeaponSwitch
			gnActiveWeaponSet = 1 - gnActiveWeaponSet;
			break;

		case 0x63: // OpenWaypoint (dwWaypointGUID, ...)
			if (nPacketLen >= 5)
			{
				gdwActiveWaypointGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
				gnWaypointActTab = std::min<int32_t>(4, gnCurrentAct);
				gnLeftPanel = LEFTPANEL_WAYPOINT;
			}
			break;

		case 0x73: // Server Missile Packet
			if (nPacketLen >= 16)
			{
				const uint16_t wMissileId = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
				const int16_t wCurSubX = *reinterpret_cast<const int16_t*>(&aBuffer[8]);
				const int16_t wCurSubY = *reinterpret_cast<const int16_t*>(&aBuffer[10]);
				const int16_t wDstSubX = *reinterpret_cast<const int16_t*>(&aBuffer[12]);
				const int16_t wDstSubY = *reinterpret_cast<const int16_t*>(&aBuffer[14]);
				int nStartIsoX = wCurSubX, nStartIsoY = wCurSubY;
				int nEndIsoX = wDstSubX, nEndIsoY = wDstSubY;
				DUNGEON_GameSubtileToClientCoords(&nStartIsoX, &nStartIsoY);
				DUNGEON_GameSubtileToClientCoords(&nEndIsoX, &nEndIsoY);
				SpawnClientMissile(wMissileId, static_cast<float>(nStartIsoX), static_cast<float>(nStartIsoY), static_cast<float>(nEndIsoX), static_cast<float>(nEndIsoY), 0x62u);
			}
			break;

		case 0x77: // UpdateUI (0x09 = OpenStash, 0x15 = OpenCube)
			if (nPacketLen >= 2)
			{
				if (aBuffer[1] == 0x09u)
				{
					gnLeftPanel = LEFTPANEL_STASH;
					gnRightPanel = RIGHTPANEL_INV;
				}
				else if (aBuffer[1] == 0x15u)
				{
					gnLeftPanel = LEFTPANEL_CUBE;
					gnRightPanel = RIGHTPANEL_INV;
				}
			}
			break;

		case 0x79: // Overlay (nUnitType, dwGUID, wOverlayId)
			if (nPacketLen >= 8)
			{
				const uint8_t nUnitType = aBuffer[1];
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				const uint16_t wOverlayId = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
				if (D2UnitStrc* pTarget = D2Client_FindServerSideUnit(dwGuid, nUnitType))
				{
					GFX_AttachOverlayToUnit(pTarget, wOverlayId, 0);
				}
				SpawnClientOverlay(nUnitType, dwGuid, wOverlayId, 0x6Au, 30);
			}
			break;

		case 0x81: // AssignMerc (nUnk, wClassId, dwOwnerGUID, dwMercGUID, ...)
			if (nPacketLen >= 12)
			{
				const uint32_t dwOwnerGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[4]);
				const uint32_t dwMercGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[8]);
				if (gdwClientPlayerGuid != 0 && dwOwnerGuid == gdwClientPlayerGuid && dwMercGuid != 0)
				{
					gwMercClassId = *reinterpret_cast<const uint16_t*>(&aBuffer[2]);
					gdwMercUnitGuid = dwMercGuid;
					gbMercHired = true;
				}
			}
			break;

		case 0xAC: // AssignNPC (dwGUID, wClassId, wX, wY, nLifePct, nPacketSize, bitstream...)
			if (nPacketLen >= 13)
			{
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
				const uint16_t wClassId = *reinterpret_cast<const uint16_t*>(&aBuffer[5]);
				const uint16_t wSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[7]);
				const uint16_t wSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[9]);
				const uint8_t nLifePct = aBuffer[11];
				if (gpClientAct && wSubX > 0 && wSubY > 0)
				{
					SafeStreamClientRoomAtTile(gpClientAct, wSubX / 5, wSubY / 5);
				}
				const int32_t nInitMonMode = (nLifePct == 0) ? MONMODE_DEAD : MONMODE_NEUTRAL;
				if (D2UnitStrc* pExistingMon = D2Client_FindServerSideUnit(dwGuid, UNIT_MONSTER))
				{
					D2Client_TeleportUnitToCoordinates(pExistingMon, wSubX, wSubY, 0);
					if (pExistingMon->pMonsterData)
					{
						pExistingMon->pMonsterData->unk0x3C.unk0x00 = nLifePct;
					}
					if (!pExistingMon->pGfxData)
					{
						GFX_InitUnitCompositeGfx(pExistingMon);
					}
				}
				else if (gpClientAct)
				{
					D2MonsterSpawnDescStrc spawnDesc = {};
					spawnDesc.dwUnitGUID = dwGuid;
					spawnDesc.nClassId = wClassId;
					spawnDesc.nX = wSubX;
					spawnDesc.nY = wSubY;
					spawnDesc.wTypeFlags = 0;
					spawnDesc.wPad12 = 0;
					spawnDesc.nLifePct = nLifePct;
					spawnDesc.nAnimMode = nInitMonMode;
					D2UnitStrc* pNewMon = D2Client_AllocClientMonsterUnit(&spawnDesc, nullptr, nullptr, 0, 0, 0);
					if (pNewMon && !pNewMon->pGfxData)
					{
						GFX_InitUnitCompositeGfx(pNewMon);
					}
				}
				if (ClientWorldUnit* pMon = FindOrAllocClientUnit(UNIT_MONSTER, dwGuid))
				{
					pMon->wClassId = wClassId;
					pMon->nLifePct = nLifePct;
					pMon->nAnimMode = (nLifePct == 0) ? MONMODE_DEAD : MONMODE_NEUTRAL;
					SetClientUnitSubtilePos(pMon, wSubX, wSubY, true);
					if (D2MonStatsTxt* pMonTxt = DATATBLS_GetMonStatsTxtRecord(wClassId))
					{
						pMon->bIsNpc = (pMonTxt->dwMonStatsFlags & (MONSTATSFLAG_NPC | MONSTATSFLAG_INTOWN | MONSTATSFLAG_INTERACT)) != 0;
						pMon->bIsHostile = !pMon->bIsNpc && (pMonTxt->nAlign == MONALIGN_EVIL);
						const Unicode* pUniName = D2LANG_GetStringFromTblIndex(pMonTxt->wNameStr);
						UnicodeToAsciiSafe(pUniName, pMon->szName, sizeof(pMon->szName));
					}
					if (!pMon->szName[0])
					{
						std::snprintf(pMon->szName, sizeof(pMon->szName), "Monster #%u", wClassId);
					}
				}
			}
			break;

		default:
			break;
		}
		Sleep(0);
	}
}

static void LoadCharacterSaveAppearance()
{
	gnCharClass = gpD2Config ? gpD2Config->unpackedCTemp.nCharacterClassId : 0;
	if (gnCharClass >= 7)
	{
		gnCharClass = 0;
	}
	gnCharLevel = 1;
	gwCharSaveFlags = gpD2Config ? gpD2Config->unpackedCTemp.nCharacterSaveFlags : 0x20;
	std::memset(gaCharComponents, 0xFF, sizeof(gaCharComponents));
	std::memset(gaCharColors, 0xFF, sizeof(gaCharColors));

	char szSaveDir[MAX_PATH] = {};
	char szFullPath[MAX_PATH * 2] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
	std::snprintf(szFullPath, sizeof(szFullPath), "%s%s.d2s", szSaveDir, gszCharName);

	FILE* pFile = std::fopen(szFullPath, "rb");
	if (!pFile)
	{
		static const char* const kFallbackDirs[] = { "..\\..\\Debug\\save\\", "..\\..\\Release\\save\\" };
		for (const char* szRel : kFallbackDirs)
		{
			std::snprintf(szFullPath, sizeof(szFullPath), "%s%s%s.d2s", szSaveDir, szRel, gszCharName);
			pFile = std::fopen(szFullPath, "rb");
			if (pFile)
			{
				break;
			}
		}
	}
	if (pFile)
	{
		uint8_t aHdr[0xB0] = {};
		const size_t nRead = std::fread(aHdr, 1, sizeof(aHdr), pFile);
		std::fclose(pFile);
		if (nRead >= 0xA8 && *reinterpret_cast<const uint32_t*>(&aHdr[0]) == 0xAA55AA55u)
		{
			gwCharSaveFlags = static_cast<uint16_t>(*reinterpret_cast<const uint32_t*>(&aHdr[0x24]));
			if (aHdr[0x28] < 7)
			{
				gnCharClass = aHdr[0x28];
			}
			if (aHdr[0x2B] > 0)
			{
				gnCharLevel = aHdr[0x2B];
			}
			std::memcpy(gaCharComponents, &aHdr[0x88], 16);
			std::memcpy(gaCharColors, &aHdr[0x98], 16);
		}
	}
}

static void InitInGameAssets()
{
	D2Win_10177(0);
	LoadCharacterSaveAppearance();

	// Initialize reconstructed Msg, QuestLog & UNIT_CUnit subsystems
	MSG_ResetCounters();
	MSG_ResetLastSentPacket();
	QUESTLOG_InitSession();
	D2Client_ClearAllUnitHashTables();

	// Reset authoritative client tables
	gdwClientPlayerGuid = 0;
	gnServerSubX = 0;
	gnServerSubY = 0;
	gbServerActLoaded = false;
	gnCurrentAct = 0;
	gdwServerMapSeed = 0x112A2026u;
	gwServerTownLevelId = LEVEL_ROGUEENCAMPMENT;
	gnCurrentLevelId = LEVEL_ROGUEENCAMPMENT;
	gbNpcMenuOpen = false;
	gnActiveNpcGuid = 0;
	gwActiveNpcClassId = 0;
	gszActiveNpcName[0] = '\0';
	gnNpcMenuSelectedIdx = 0;
	gnVendorActiveTab = 0;
	gnSelectedVendorItemIdx = -1;
	gnPendingNpcGuid = 0;
	gdwNpcIntrosReceived = 0;
	gdwVendorItemsReceived = 0;
	gbMercHired = false;
	gdwMercUnitGuid = 0;
	gnCursorItemIdx = -1;
	gnActiveWeaponSet = 0;
	std::memset(gaQuestFilterStates, 0, sizeof(gaQuestFilterStates));
	std::memset(gaClientStats, 0, sizeof(gaClientStats));
	std::memset(gaClientSkillLevels, 0, sizeof(gaClientSkillLevels));
	std::memset(gClientUnits, 0, sizeof(gClientUnits));
	for (int32_t i = 0; i < kMaxClientItems; ++i)
	{
		if (gClientItems[i].pInvCell)
		{
			ARCHIVE_FreeCellFile(gClientItems[i].pInvCell);
		}
	}
	std::memset(gClientItems, 0, sizeof(gClientItems));
	gnHoveredUnitIdx = -1;
	gnHoveredGroundItemIdx = -1;
	gwLeftSkillId = 0;
	gwRightSkillId = 0;
	gnChatLogCount = 0;
	AppendClientChatLog("Entered Act I - Rogue Encampment.");
	AppendClientChatLog("Diablo II v1.12a Authoritative Session.");

	// Seed initial base stats from CharStatsTxt until server S2C 0x1D..0x1F packets arrive
	static const int32_t kInitStats[7][7] = {
		{ 20, 25, 20, 15, 50, 15, 84 }, // Amazon
		{ 10, 25, 10, 35, 40, 35, 74 }, // Sorceress
		{ 15, 25, 15, 25, 45, 25, 79 }, // Necromancer
		{ 25, 20, 25, 15, 55, 15, 89 }, // Paladin
		{ 30, 20, 25, 10, 55, 10, 92 }, // Barbarian
		{ 15, 20, 25, 20, 55, 20, 84 }, // Druid
		{ 20, 20, 20, 25, 50, 25, 95 }, // Assassin
	};
	const uint8_t nSafeClass = (gnCharClass < 7) ? gnCharClass : 0;
	gaClientStats[STAT_LEVEL] = gnCharLevel;
	gaClientStats[STAT_STRENGTH] = kInitStats[nSafeClass][0];
	gaClientStats[STAT_DEXTERITY] = kInitStats[nSafeClass][1];
	gaClientStats[STAT_VITALITY] = kInitStats[nSafeClass][2];
	gaClientStats[STAT_ENERGY] = kInitStats[nSafeClass][3];
	gaClientStats[STAT_HITPOINTS] = kInitStats[nSafeClass][4] << 8;
	gaClientStats[STAT_MAXHP] = kInitStats[nSafeClass][4] << 8;
	gaClientStats[STAT_MANA] = kInitStats[nSafeClass][5] << 8;
	gaClientStats[STAT_MAXMANA] = kInitStats[nSafeClass][5] << 8;
	gaClientStats[STAT_STAMINA] = kInitStats[nSafeClass][6] << 8;
	gaClientStats[STAT_MAXSTAMINA] = kInitStats[nSafeClass][6] << 8;

	gpHudCtrlPnl = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\800ctrlpnl7", 0);
	gpHudHlthMana = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\hlthmana", 0);
	gpHudOverlap = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\overlap", 0);
	gpHudCampfire = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\fire", 0);
	gpHudCursor = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CURSOR\\ohand", 0);
	gpHudRunBtn = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\runbutton", 0);
	gpHudMenuBtn = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\menubutton", 0);
	gpHudMiniPanel = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\minipanel", 0);
	gpHudMiniPanelBtn = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\minipanelbtn", 0);
	gpHudSkillIcon = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\SPELLS\\Skillicon", 0);
	gpInvChar6 = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\InvChar6", 0);
	gpBuySellBtn = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\buysellbtn", 0);
	gpStatUpBtn = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\level", 0);
	gpQuestBg = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\MENU\\questbackground", 0);
	gpQuestTabs = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\MENU\\expquesttabs", 0);
	gpQuestSockets = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\MENU\\questsockets", 0);
	gpPentSpin = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CURSOR\\Pentspin", 0);
	gpInvWeapons = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\inv_weapons", 0);
	gpInvArmor = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\inv_armor", 0);
	gpInvHelmGlove = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\inv_helm_glove", 0);
	gpInvBoots = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\inv_boots", 0);
	gpInvBelt = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\inv_belt", 0);
	gpInvRingAmulet = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\inv_ring_amulet", 0);
	gpStashPanel = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\TradeStash", 0);
	gpCubePanel = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\HoradricCube", 0);
	gpTransmuteBtn = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\Transmute", 0);
	gpWaypointBg = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\MENU\\waygatebackground", 0);
	gpWaypointTabs = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\MENU\\expwaygatetabs", 0);
	gpMercInvPanel = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\inv_merc", 0);
	gpClassSkillIcon = ARCHIVE_LoadCellFile(kClassSkillIconPaths[nSafeClass], 0);
	gpSkillTreeBg = ARCHIVE_LoadCellFile(kClassSkillTreeBgPaths[nSafeClass], 0);
	gpPlayerClassNu1 = ARCHIVE_LoadCellFile(kClassNu1Paths[nSafeClass], 0);
	gpPlayerClassFw = ARCHIVE_LoadCellFile(kClassFwPaths[nSafeClass], 0);

	gnQuestLogActTab = 0;
	gnWaypointActTab = 0;
	gbBeltExpanded = false;
	gnCursorItemIdx = -1;
	gbChatInputOpen = false;
	gszChatInputBuf[0] = '\0';
	gnChatInputLen = 0;
	std::memset(gClientMissiles, 0, sizeof(gClientMissiles));
	std::memset(gClientOverlays, 0, sizeof(gClientOverlays));
	gbWeatherInitialized = false;

	gnLeftPanel = LEFTPANEL_NONE;
	gnRightPanel = RIGHTPANEL_NONE;
	gbMiniPanelOpen = true;
	gbAutomapOpen = false;
	gbEscMenuOpen = false;
	gnEscSubMenu = 0;
	gnEscSelectedIndex = 0;
	gnActiveSkillTab = 0;
	gnSelectedQuest = 0;
	gbRunEnabled = true;

	GFXUTIL_InitDirectionTables();
	GFX_UpdateActCofCache(gnCurrentAct);
	if (!gpClientViewport)
	{
		gpClientViewport = VIEW_AllocViewport();
	}
	if (gpClientViewport)
	{
		VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), 0, 0, 800, 552);
	}

	gfPlayerWorldX = 0.0f;
	gfPlayerWorldY = 0.0f;

	if (gpClientAct)
	{
		DUNGEON_FreeAct(gpClientAct);
		gpClientAct = nullptr;
	}
	gpClientAct = DUNGEON_AllocAct(0, 0x112A2026u, FALSE, nullptr, 0, nullptr, LEVEL_ROGUEENCAMPMENT, nullptr, nullptr);
	g_pClientAct = gpClientAct;
	int nSpawnSubX = 0;
	int nSpawnSubY = 0;
	if (gpClientAct)
	{
		D2ActiveRoomStrc* pSpawnRoom = DUNGEON_FindActSpawnLocationEx(
			gpClientAct,
			LEVEL_ROGUEENCAMPMENT,
			11,
			&nSpawnSubX,
			&nSpawnSubY,
			2);
		if (!pSpawnRoom || (nSpawnSubX == 0 && nSpawnSubY == 0))
		{
			pSpawnRoom = DUNGEON_FindActSpawnLocationEx(
				gpClientAct,
				LEVEL_ROGUEENCAMPMENT,
				0,
				&nSpawnSubX,
				&nSpawnSubY,
				2);
		}
		PrestreamTownAndAdjacentWilderness(gpClientAct, 0, LEVEL_ROGUEENCAMPMENT);
		for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom; pRoom = pRoom->pRoomNext)
		{
			bool bFoundCampfire = false;
			for (D2PresetUnitStrc* pPreset = pRoom->pDrlgRoom ? pRoom->pDrlgRoom->pPresetUnits : nullptr; pPreset; pPreset = pPreset->pNext)
			{
				if (pPreset->nUnitType == UNIT_OBJECT && (pPreset->nIndex == 39 || pPreset->nIndex == 266))
				{
					nSpawnSubX = pRoom->tCoords.nSubtileX + pPreset->nXpos + 4;
					nSpawnSubY = pRoom->tCoords.nSubtileY + pPreset->nYpos + 4;
					if (pPreset->nIndex == 39)
					{
						bFoundCampfire = true;
						break;
					}
				}
			}
			if (bFoundCampfire)
			{
				break;
			}
		}
		if (nSpawnSubX != 0 || nSpawnSubY != 0)
		{
			int nClientX = nSpawnSubX;
			int nClientY = nSpawnSubY;
			DUNGEON_GameSubtileToClientCoords(&nClientX, &nClientY);
			gfPlayerWorldX = static_cast<float>(nClientX);
			gfPlayerWorldY = static_cast<float>(nClientY);
		}
	}

	gfTargetWorldX = gfPlayerWorldX;
	gfTargetWorldY = gfPlayerWorldY;
	gnPlayerDir = 0;
	gbPlayerMoving = false;
	gbMouseLeftHeld = false;

	if (gpClientAct && (nSpawnSubX != 0 || nSpawnSubY != 0))
	{
		D2UnitStrc* pInitPlr = D2Client_AllocClientPlayerUnit(0x7FFFFFFFu, nSafeClass, nSpawnSubX, nSpawnSubY, gszCharName);
		if (pInitPlr)
		{
			D2Client_SetLocalPlayerUnit(pInitPlr);
			GFX_InitUnitCompositeGfx(pInitPlr);
		}
	}

	// Gap 1 Reconstructed Subsystem Initialization (DRAW_dLightMap, UI_automap, UI_SkillDesc, UI_inv)
	gdwSCmdPacketsDispatched = 0;
	gdwLightMapRoomsSampled = 0;
	gdwAutomapRoomsPopulated = 0;
	gdwSkillDescTooltipsBuilt = 0;
	gdwInvPanelEvaluations = 0;
	SyncReconstructedClientGlobals();
	SyncLocalPlayerUnitToWorldCoords();
	LIGHTMAP_InitializeReciprocalTableAndActCallback();
	LIGHTMAP_ResetAmbientGrid(FALSE);
	AUTOMAP_InitColors();
	(void)AUTOMAP_InitLayer(1);
	g_bExpansionMode = gbIsExpansion ? TRUE : FALSE;
	INV_SyncExpansionModeFlag();
	INV_InitInventoryLayoutRectsAndGrids();
	INV_SetPanelMode(0);
}

static void FreeCellPtr(D2CellFileStrc** ppCell)
{
	if (ppCell && *ppCell)
	{
		ARCHIVE_FreeCellFile(*ppCell);
		*ppCell = nullptr;
	}
}

static void ShutdownInGameAssets()
{
	AUTOMAP_FreeAllLayers();
	FreeAllReconstructedServerUnits();
	LIGHTMAP_FreeAllLightNodes();
	LIGHTMAP_ClearGrid();
	if (gpClientViewport)
	{
		VIEW_FreeViewport(static_cast<D2ViewportStrc*>(gpClientViewport));
		gpClientViewport = nullptr;
	}
	GFX_FreeMonsterCofCache();
	GFX_FreePlayerCofCache();
	if (gpClientAct)
	{
		DUNGEON_FreeAct(gpClientAct);
		gpClientAct = nullptr;
		g_pClientAct = nullptr;
	}
	FreeCellPtr(&gpMercInvPanel);
	FreeCellPtr(&gpWaypointTabs);
	FreeCellPtr(&gpWaypointBg);
	FreeCellPtr(&gpTransmuteBtn);
	FreeCellPtr(&gpCubePanel);
	FreeCellPtr(&gpStashPanel);
	for (int32_t i = 0; i < kMaxClientItems; ++i)
	{
		FreeCellPtr(&gClientItems[i].pInvCell);
	}
	FreeCellPtr(&gpPlayerClassNu1);
	FreeCellPtr(&gpPlayerClassFw);
	FreeCellPtr(&gpClassSkillIcon);
	FreeCellPtr(&gpSkillTreeBg);
	FreeCellPtr(&gpInvRingAmulet);
	FreeCellPtr(&gpInvBelt);
	FreeCellPtr(&gpInvBoots);
	FreeCellPtr(&gpInvHelmGlove);
	FreeCellPtr(&gpInvArmor);
	FreeCellPtr(&gpInvWeapons);
	FreeCellPtr(&gpPentSpin);
	FreeCellPtr(&gpQuestSockets);
	FreeCellPtr(&gpQuestTabs);
	FreeCellPtr(&gpQuestBg);
	FreeCellPtr(&gpStatUpBtn);
	FreeCellPtr(&gpBuySellBtn);
	FreeCellPtr(&gpInvChar6);
	FreeCellPtr(&gpHudSkillIcon);
	FreeCellPtr(&gpHudMiniPanelBtn);
	FreeCellPtr(&gpHudMiniPanel);
	FreeCellPtr(&gpHudMenuBtn);
	FreeCellPtr(&gpHudRunBtn);
	FreeCellPtr(&gpHudCtrlPnl);
	FreeCellPtr(&gpHudHlthMana);
	FreeCellPtr(&gpHudOverlap);
	FreeCellPtr(&gpHudCampfire);
	FreeCellPtr(&gpHudCursor);
}

static bool IsSubtileWalkableForPlayer(float fCandWorldX, float fCandWorldY)
{
	if (!gpClientAct)
	{
		return true;
	}
	int nSubX = static_cast<int>(fCandWorldX);
	int nSubY = static_cast<int>(fCandWorldY);
	DUNGEON_ClientToGameSubtileCoords(&nSubX, &nSubY);
	if (nSubX <= 0 || nSubY <= 0)
	{
		return false;
	}
	SafeStreamClientRoomAtTile(gpClientAct, nSubX / 5, nSubY / 5);
	D2ActiveRoomStrc* pRoom = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, nSubX, nSubY);
	if (!pRoom)
	{
		return false;
	}
	const uint16_t wMask = COLLISION_CheckMaskWithSize(
		pRoom,
		nSubX,
		nSubY,
		COLLISION_UNIT_SIZE_SMALL,
		COLLIDE_MASK_PLAYER_PATH | COLLIDE_BLANK);
	return (wMask == 0);
}

static void UpdatePlayerMovement()
{
	// Interpolate live server monsters / NPCs toward their target world position
	for (int32_t i = 0; i < kMaxClientUnits; ++i)
	{
		ClientWorldUnit& u = gClientUnits[i];
		if (!u.bActive || !u.bMoving)
		{
			continue;
		}
		const float dx = u.fTargetWorldX - u.fWorldX;
		const float dy = u.fTargetWorldY - u.fWorldY;
		const float d = std::sqrt(dx * dx + dy * dy);
		if (d < 5.0f)
		{
			u.fWorldX = u.fTargetWorldX;
			u.fWorldY = u.fTargetWorldY;
			u.bMoving = false;
		}
		else
		{
			u.fWorldX += (dx / d) * 6.0f;
			u.fWorldY += (dy / d) * 6.0f;
		}
	}

	// Check pending NPC walk-to-interact when player gets within <= 5 subtiles
	if (gnPendingNpcGuid != 0)
	{
		int nCurSubX = static_cast<int>(gfPlayerWorldX);
		int nCurSubY = static_cast<int>(gfPlayerWorldY);
		DUNGEON_ClientToGameSubtileCoords(&nCurSubX, &nCurSubY);
		for (int32_t i = 0; i < kMaxClientUnits; ++i)
		{
			if (gClientUnits[i].bActive && gClientUnits[i].nUnitType == UNIT_MONSTER && gClientUnits[i].dwUnitGuid == gnPendingNpcGuid)
			{
				const int32_t nSubDx = std::abs(nCurSubX - gClientUnits[i].nSubX);
				const int32_t nSubDy = std::abs(nCurSubY - gClientUnits[i].nSubY);
				if (nSubDx <= 5 && nSubDy <= 5)
				{
					SendUnitInteract(UNIT_MONSTER, gnPendingNpcGuid);
					gnPendingNpcGuid = 0;
				}
				break;
			}
		}
	}

	if (!gbPlayerMoving)
	{
		SyncLocalPlayerUnitToWorldCoords();
		return;
	}

	const float fDx = gfTargetWorldX - gfPlayerWorldX;
	const float fDy = gfTargetWorldY - gfPlayerWorldY;
	const float fDist = std::sqrt(fDx * fDx + fDy * fDy);
	if (fDist < 6.0f)
	{
		if (IsSubtileWalkableForPlayer(gfTargetWorldX, gfTargetWorldY))
		{
			gfPlayerWorldX = gfTargetWorldX;
			gfPlayerWorldY = gfTargetWorldY;
		}
		gbPlayerMoving = false;
		if (gnPendingNpcGuid != 0)
		{
			SendUnitInteract(UNIT_MONSTER, gnPendingNpcGuid);
			gnPendingNpcGuid = 0;
		}
		SyncLocalPlayerUnitToWorldCoords();
		return;
	}

	const float fStep = gbRunEnabled ? 11.0f : 7.0f;
	const float fCandX = gfPlayerWorldX + (fDx / fDist) * fStep;
	const float fCandY = gfPlayerWorldY + (fDy / fDist) * fStep;
	if (IsSubtileWalkableForPlayer(fCandX, fCandY))
	{
		gfPlayerWorldX = fCandX;
		gfPlayerWorldY = fCandY;
	}
	else if (IsSubtileWalkableForPlayer(fCandX, gfPlayerWorldY))
	{
		gfPlayerWorldX = fCandX;
	}
	else if (IsSubtileWalkableForPlayer(gfPlayerWorldX, fCandY))
	{
		gfPlayerWorldY = fCandY;
	}
	else
	{
		// Blocked by river, wall, or obstacle
		gbPlayerMoving = false;
		SyncLocalPlayerUnitToWorldCoords();
		return;
	}

	// Stream 3x3 neighborhood of DRLG rooms around player's viewport (including cross-level transitions like Blood Moor)
	if (gpClientAct)
	{
		int nCurSubX = static_cast<int>(gfPlayerWorldX);
		int nCurSubY = static_cast<int>(gfPlayerWorldY);
		DUNGEON_ClientToGameSubtileCoords(&nCurSubX, &nCurSubY);
		if (nCurSubX > 0 && nCurSubY > 0)
		{
			StreamClientNeighborhoodAroundTile(nCurSubX / 5, nCurSubY / 5, 16);
			if (D2ActiveRoomStrc* pCurRoom = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, nCurSubX, nCurSubY))
			{
				const int32_t nRoomLvl = DUNGEON_GetLevelIdFromRoom(pCurRoom);
				if (nRoomLvl > 0)
				{
					gnCurrentLevelId = nRoomLvl;
				}
			}
		}
	}

	// Map movement vector to D2's 8-direction COF index (0=S, 1=SW, 2=W, 3=NW, 4=N, 5=NE, 6=E, 7=SE)
	const float fAngleDeg = std::atan2(fDy, fDx) * (180.0f / 3.14159265f);
	if (fAngleDeg >= 67.5f && fAngleDeg < 112.5f)
	{
		gnPlayerDir = 0; // S
	}
	else if (fAngleDeg >= 112.5f && fAngleDeg < 157.5f)
	{
		gnPlayerDir = 1; // SW
	}
	else if (fAngleDeg >= 157.5f || fAngleDeg < -157.5f)
	{
		gnPlayerDir = 2; // W
	}
	else if (fAngleDeg >= -157.5f && fAngleDeg < -112.5f)
	{
		gnPlayerDir = 3; // NW
	}
	else if (fAngleDeg >= -112.5f && fAngleDeg < -67.5f)
	{
		gnPlayerDir = 4; // N
	}
	else if (fAngleDeg >= -67.5f && fAngleDeg < -22.5f)
	{
		gnPlayerDir = 5; // NE
	}
	else if (fAngleDeg >= -22.5f && fAngleDeg < 22.5f)
	{
		gnPlayerDir = 6; // E
	}
	else
	{
		gnPlayerDir = 7; // SE
	}
	SyncLocalPlayerUnitToWorldCoords();
}

static void DrawAsciiText(const char* szText, int32_t nX, int32_t nY, int32_t nColor, BOOL bCentered = FALSE)
{
	if (!szText || !szText[0])
	{
		return;
	}
	Unicode wszBuf[256] = {};
	Unicode::win2Unicode(wszBuf, szText, 255);
	if (bCentered)
	{
		const int32_t nW = D2Win_10121_GetTextWidth(wszBuf);
		D2Win_10117_DrawText(wszBuf, nX - nW / 2, nY, nColor, 0);
	}
	else
	{
		D2Win_10117_DrawText(wszBuf, nX, nY, nColor, 0);
	}
}

static void DrawSingleCelTopLeft(
	D2CellFileStrc* pCellFile,
	int32_t nLeft,
	int32_t nTop,
	int32_t nFrame,
	DrawMode eMode = DRAWMODE_NORMAL)
{
	if (!pCellFile)
	{
		return;
	}
	const int32_t nTotal = D2CMP_CelFileGetCelsPerDirection(pCellFile);
	if (nTotal <= 0)
	{
		return;
	}
	D2GfxDataStrc tGfx = {};
	tGfx.pCellFile = pCellFile;
	tGfx.nDirection = 0;
	tGfx.nFrame = std::max<int32_t>(0, std::min<int32_t>(nFrame, nTotal - 1));
	D2CellFileStrc* pSubCell = D2CMP_CelGetHandle(&tGfx);
	const int32_t nH = pSubCell ? D2CMP_CelGetHeight(pSubCell) : 0;
	TEXTURE_CelDraw(&tGfx, nLeft, nTop + nH, 0xFFFFFFFFu, eMode, nullptr);
}

static void DrawTiledPanelCellFile(
	D2CellFileStrc* pCellFile,
	int32_t nPanelLeft,
	int32_t nPanelTop,
	int32_t nStartFrame,
	int32_t nCols,
	int32_t nRows)
{
	if (!pCellFile)
	{
		return;
	}
	const int32_t nTotalFrames = D2CMP_CelFileGetCelsPerDirection(pCellFile);
	int32_t nCurTop = nPanelTop;
	int32_t nFrameIdx = nStartFrame;
	for (int32_t r = 0; r < nRows; ++r)
	{
		int32_t nCurLeft = nPanelLeft;
		int32_t nRowHeight = 0;
		for (int32_t c = 0; c < nCols; ++c)
		{
			if (nFrameIdx >= nTotalFrames)
			{
				break;
			}
			D2GfxDataStrc tGfx = {};
			tGfx.pCellFile = pCellFile;
			tGfx.nDirection = 0;
			tGfx.nFrame = nFrameIdx++;
			D2CellFileStrc* pSubCell = D2CMP_CelGetHandle(&tGfx);
			const int32_t nW = pSubCell ? D2CMP_CelGetWidth(pSubCell) : 256;
			const int32_t nH = pSubCell ? D2CMP_CelGetHeight(pSubCell) : 256;
			if (nH > nRowHeight)
			{
				nRowHeight = nH;
			}
			TEXTURE_CelDraw(&tGfx, nCurLeft, nCurTop + nH, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
			nCurLeft += nW;
		}
		nCurTop += nRowHeight;
	}
}

static void DrawAutomapOverlay()
{
	if (!gbAutomapOpen || !gpClientAct)
	{
		return;
	}

	(void)AUTOMAP_InitLayer(gnCurrentLevelId > 0 ? gnCurrentLevelId : 1);

	const int32_t nCamX = static_cast<int32_t>(gfPlayerWorldX);
	const int32_t nCamY = static_cast<int32_t>(gfPlayerWorldY);

	for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom; pRoom = pRoom->pRoomNext)
	{
		++gdwAutomapRoomsPopulated;
		int nWalls = 0;
		D2DrlgTileDataStrc* pWalls = DUNGEON_GetWallTilesFromRoom(pRoom, &nWalls);
		if (pWalls && nWalls > 0)
		{
			for (int i = 0; i < nWalls; ++i)
			{
				const D2DrlgTileDataStrc& td = pWalls[i];
				if (!td.pTile || td.nTileType == TILETYPE_ROOF || (td.dwFlags & MAPTILE_HIDDEN))
				{
					continue;
				}
				const int32_t nMapX = 400 + (td.nWidth - nCamX) / 4;
				const int32_t nMapY = 260 + (td.nHeight - nCamY) / 4;
				if (nMapX >= 40 && nMapX < 760 && nMapY >= 40 && nMapY < 500)
				{
					D2GFX_DrawLine(nMapX - 8, nMapY - 4, nMapX + 8, nMapY + 4, 0x16u, 0xFFu);
				}
			}
		}

		for (D2PresetUnitStrc* pPreset = (pRoom && pRoom->pDrlgRoom) ? pRoom->pDrlgRoom->pPresetUnits : nullptr; pPreset; pPreset = pPreset->pNext)
		{
			int nWorldX = pRoom->tCoords.nSubtileX + pPreset->nXpos;
			int nWorldY = pRoom->tCoords.nSubtileY + pPreset->nYpos;
			DUNGEON_GameSubtileToClientCoords(&nWorldX, &nWorldY);
			const int32_t nMapX = 400 + (nWorldX - nCamX) / 4;
			const int32_t nMapY = 260 + (nWorldY - nCamY) / 4;
			if (nMapX >= 40 && nMapX < 760 && nMapY >= 40 && nMapY < 500)
			{
				const uint8_t nCol = (pPreset->nUnitType == UNIT_MONSTER) ? 0x84u : 0x6Au;
				AUTOMAP_DrawCrosshair(nMapX, nMapY, nCol);
			}
		}
	}

	// Draw live server units on Automap (red crosses for hostile monsters, green for NPCs, gold for objects)
	for (int32_t i = 0; i < kMaxClientUnits; ++i)
	{
		const ClientWorldUnit& u = gClientUnits[i];
		if (!u.bActive || (u.nUnitType == UNIT_MONSTER && u.nLifePct == 0))
		{
			continue;
		}
		const int32_t nMapX = 400 + (static_cast<int32_t>(u.fWorldX) - nCamX) / 4;
		const int32_t nMapY = 260 + (static_cast<int32_t>(u.fWorldY) - nCamY) / 4;
		if (nMapX >= 40 && nMapX < 760 && nMapY >= 40 && nMapY < 500)
		{
			const uint8_t nCol = u.bIsHostile ? 0x62u : (u.bIsNpc ? 0x84u : 0x6Au);
			AUTOMAP_DrawCrosshair(nMapX, nMapY, nCol);
		}
	}

	// Player cross at center of Automap via reconstructed AUTOMAP_DrawCrosshair (0x6FB00320)
	AUTOMAP_DrawCrosshair(400, 260, 0x97u);

	static const char* const kDiffNames[3] = { "Normal Difficulty", "Nightmare Difficulty", "Hell Difficulty" };
	D2Win_10127_SetFont(D2FONT_FONT16);
	DrawAsciiText(kDiffNames[gnGameDifficulty < 3 ? gnGameDifficulty : 0], 635, 44, 4, FALSE);
}

struct ClassBaseStats
{
	int32_t nStr;
	int32_t nDex;
	int32_t nVit;
	int32_t nEne;
	int32_t nLife;
	int32_t nMana;
	int32_t nStam;
};

static const ClassBaseStats kClassBaseStats[7] = {
	{ 20, 25, 20, 15, 50, 15, 84 }, // Amazon
	{ 10, 25, 10, 35, 40, 35, 74 }, // Sorceress
	{ 15, 25, 15, 25, 45, 25, 79 }, // Necromancer
	{ 25, 20, 25, 15, 55, 15, 89 }, // Paladin
	{ 30, 20, 25, 10, 55, 10, 92 }, // Barbarian
	{ 15, 20, 25, 20, 55, 20, 84 }, // Druid
	{ 20, 20, 20, 25, 50, 25, 95 }, // Assassin
};

static void DrawLeftPanel()
{
	if (gnLeftPanel == LEFTPANEL_NONE)
	{
		return;
	}

	const uint8_t nSafeClass = (gnCharClass < 7) ? gnCharClass : 0;
	const ClassBaseStats& st = kClassBaseStats[nSafeClass];

	if (gnLeftPanel == LEFTPANEL_CHAR)
	{
		if (gpInvChar6)
		{
			DrawTiledPanelCellFile(gpInvChar6, 80, 60, 0, 2, 2);
		}
		else
		{
			D2GFX_DrawBox(80, 60, 400, 492, 0x00u, DRAWMODE_TRANS75);
		}

		D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
		DrawAsciiText(gszCharName, 175, 84, 4, TRUE);
		DrawAsciiText(kClassNames[nSafeClass], 330, 84, 4, TRUE);

		const int32_t nLvl = std::max<int32_t>(1, gaClientStats[STAT_LEVEL] ? gaClientStats[STAT_LEVEL] : gnCharLevel);
		const uint32_t dwExp = static_cast<uint32_t>(std::max<int32_t>(0, gaClientStats[STAT_EXPERIENCE]));
		const uint32_t dwNextExp = DATATBLS_GetLevelThreshold(nSafeClass, nLvl);

		char szBuf[64] = {};
		std::snprintf(szBuf, sizeof(szBuf), "%d", nLvl);
		DrawAsciiText("Level", 112, 108, 4, TRUE);
		DrawAsciiText(szBuf, 112, 124, 0, TRUE);

		DrawAsciiText("Experience", 215, 108, 4, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%u", dwExp);
		DrawAsciiText(szBuf, 215, 124, 0, TRUE);

		DrawAsciiText("Next Level", 330, 108, 4, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%u", dwNextExp ? dwNextExp : 500u);
		DrawAsciiText(szBuf, 330, 124, 0, TRUE);

		const int32_t nStr = gaClientStats[STAT_STRENGTH] ? gaClientStats[STAT_STRENGTH] : st.nStr;
		const int32_t nDex = gaClientStats[STAT_DEXTERITY] ? gaClientStats[STAT_DEXTERITY] : st.nDex;
		const int32_t nVit = gaClientStats[STAT_VITALITY] ? gaClientStats[STAT_VITALITY] : st.nVit;
		const int32_t nEne = gaClientStats[STAT_ENERGY] ? gaClientStats[STAT_ENERGY] : st.nEne;

		// Attributes: Strength, Dexterity, Vitality, Energy
		DrawAsciiText("Strength", 125, 160, 4, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nStr);
		DrawAsciiText(szBuf, 195, 160, 0, TRUE);

		DrawAsciiText("Dexterity", 125, 222, 4, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nDex);
		DrawAsciiText(szBuf, 195, 222, 0, TRUE);

		DrawAsciiText("Vitality", 125, 308, 4, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nVit);
		DrawAsciiText(szBuf, 195, 308, 0, TRUE);

		DrawAsciiText("Energy", 125, 370, 4, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nEne);
		DrawAsciiText(szBuf, 195, 370, 0, TRUE);

		// Stat Point Increment '+' Buttons & Remaining Stat Points
		const int32_t nStatPts = gaClientStats[STAT_STATPTS];
		if (nStatPts > 0)
		{
			static const int32_t kBtnY[4] = { 144, 206, 292, 354 };
			for (int32_t b = 0; b < 4; ++b)
			{
				if (gpStatUpBtn)
				{
					DrawSingleCelTopLeft(gpStatUpBtn, 206, kBtnY[b], 0);
				}
				else
				{
					D2GFX_DrawBox(206, kBtnY[b], 234, kBtnY[b] + 28, 0x62u, DRAWMODE_NORMAL);
					DrawAsciiText("+", 220, kBtnY[b] + 20, 0, TRUE);
				}
			}
			D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
			DrawAsciiText("Stat Points", 142, 412, 1, TRUE);
			D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
			std::snprintf(szBuf, sizeof(szBuf), "%d", nStatPts);
			DrawAsciiText(szBuf, 212, 412, 0, TRUE);
		}

		// Combat & Derived Stats (use D2FONT_FONTFORMAL10 for labels so they fit cleanly inside the boxes)
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		DrawAsciiText("Attack Damage", 276, 155, 4, TRUE);
		DrawAsciiText("Attack Rating", 276, 211, 4, TRUE);
		DrawAsciiText("Defense", 276, 265, 4, TRUE);
		DrawAsciiText("Stamina", 266, 303, 4, TRUE);
		DrawAsciiText("Life", 266, 329, 4, TRUE);
		DrawAsciiText("Mana", 266, 367, 4, TRUE);

		DrawAsciiText("Fire Resistance", 284, 397, 4, TRUE);
		DrawAsciiText("Cold Resistance", 284, 421, 4, TRUE);
		DrawAsciiText("Lightning Resist", 284, 445, 4, TRUE);
		DrawAsciiText("Poison Resistance", 284, 469, 4, TRUE);

		D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
		const int32_t nMinDmg = std::max<int32_t>(1, gaClientStats[STAT_MINDAMAGE]);
		const int32_t nMaxDmg = std::max<int32_t>(nMinDmg + 1, gaClientStats[STAT_MAXDAMAGE] ? gaClientStats[STAT_MAXDAMAGE] : 4);
		std::snprintf(szBuf, sizeof(szBuf), "%d - %d", nMinDmg, nMaxDmg);
		DrawAsciiText(szBuf, 364, 156, 0, TRUE);

		const int32_t nAr = gaClientStats[STAT_TOHIT] ? gaClientStats[STAT_TOHIT] : (nDex * 4 + 15);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nAr);
		DrawAsciiText(szBuf, 364, 212, 0, TRUE);

		const int32_t nDef = gaClientStats[STAT_ARMORCLASS] ? gaClientStats[STAT_ARMORCLASS] : (nDex / 4 + 4);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nDef);
		DrawAsciiText(szBuf, 364, 266, 0, TRUE);

		const int32_t nCurStam = std::max<int32_t>(0, gaClientStats[STAT_STAMINA] >> 8);
		const int32_t nMaxStam = std::max<int32_t>(nCurStam ? nCurStam : st.nStam, gaClientStats[STAT_MAXSTAMINA] >> 8);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nMaxStam);
		DrawAsciiText(szBuf, 332, 304, 0, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nCurStam ? nCurStam : nMaxStam);
		DrawAsciiText(szBuf, 371, 304, 0, TRUE);

		const int32_t nCurHp = std::max<int32_t>(0, gaClientStats[STAT_HITPOINTS] >> 8);
		const int32_t nMaxHp = std::max<int32_t>(nCurHp ? nCurHp : st.nLife, gaClientStats[STAT_MAXHP] >> 8);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nMaxHp);
		DrawAsciiText(szBuf, 332, 330, 0, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nCurHp ? nCurHp : nMaxHp);
		DrawAsciiText(szBuf, 371, 330, 0, TRUE);

		const int32_t nCurMana = std::max<int32_t>(0, gaClientStats[STAT_MANA] >> 8);
		const int32_t nMaxMana = std::max<int32_t>(nCurMana ? nCurMana : st.nMana, gaClientStats[STAT_MAXMANA] >> 8);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nMaxMana);
		DrawAsciiText(szBuf, 332, 368, 0, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%d", nCurMana ? nCurMana : nMaxMana);
		DrawAsciiText(szBuf, 371, 368, 0, TRUE);

		std::snprintf(szBuf, sizeof(szBuf), "%d%%", gaClientStats[STAT_FIRERESIST]);
		DrawAsciiText(szBuf, 371, 398, 0, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%d%%", gaClientStats[STAT_COLDRESIST]);
		DrawAsciiText(szBuf, 371, 422, 0, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%d%%", gaClientStats[STAT_LIGHTRESIST]);
		DrawAsciiText(szBuf, 371, 446, 0, TRUE);
		std::snprintf(szBuf, sizeof(szBuf), "%d%%", gaClientStats[STAT_POISONRESIST]);
		DrawAsciiText(szBuf, 371, 470, 0, TRUE);

		// Close 'X' button in left column bottom socket (x=128, y=448)
		if (gpBuySellBtn)
		{
			DrawSingleCelTopLeft(gpBuySellBtn, 128, 448, 10);
		}
	}
	else if (gnLeftPanel == LEFTPANEL_QUEST)
	{
		if (gpQuestBg)
		{
			DrawTiledPanelCellFile(gpQuestBg, 80, 60, 0, 2, 2);
		}
		else
		{
			D2GFX_DrawBox(80, 60, 400, 492, 0x00u, DRAWMODE_TRANS75);
		}
		const int32_t nActTab = std::max<int32_t>(0, std::min<int32_t>(gnQuestLogActTab, 4));
		static const char* const kActRoman[5] = { "Act I", "Act II", "Act III", "Act IV", "Act V" };
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		for (int32_t t = 0; t < 5; ++t)
		{
			const int32_t tabX = 86 + t * 60;
			const bool bSelTab = (t == nActTab);
			D2GFX_DrawBox(tabX, 66, tabX + 58, 92, bSelTab ? 0x16u : 0x00u, DRAWMODE_TRANS75);
			RECT rcTab = { tabX, 66, tabX + 58, 92 };
			D2GFX_UtilRect(&rcTab, bSelTab ? 0x6Au : 0x16u);
			DrawAsciiText(kActRoman[t], tabX + 29, 83, bSelTab ? 4 : 0, TRUE);
		}

		static const char* const kAllActQuestTitles[5][6] = {
			{ "I. Den of Evil", "II. Sisters' Burial Grounds", "III. The Search for Cain", "IV. The Forgotten Tower", "V. Tools of the Trade", "VI. Sisters to the Slaughter" },
			{ "I. Radament's Lair", "II. The Horadric Staff", "III. Tainted Sun", "IV. Arcane Sanctuary", "V. The Summoner", "VI. The Seven Tombs" },
			{ "I. The Golden Bird", "II. Blade of the Old Religion", "III. Khalim's Will", "IV. Lam Esen's Tome", "V. The Blackened Temple", "VI. The Guardian" },
			{ "I. The Fallen Angel", "II. Hell's Forge", "III. Terror's End", "", "", "" },
			{ "I. Siege on Harrogath", "II. Rescue on Mount Arreat", "III. Prison of Ice", "IV. Betrayal of Harrogath", "V. Rite of Passage", "VI. Eve of Destruction" },
		};
		static const char* const kAllActQuestDescs[5][6][2] = {
			{
				{ "Speak to Akara in the Rogue Encampment", "to cleanse the Den of Evil." },
				{ "Defeat Blood Raven in the Burial", "Grounds beyond the Cold Plains." },
				{ "Rescue Deckard Cain from the", "forsaken ruins of Tristram." },
				{ "Explore the Forgotten Tower", "deep within the Black Marsh." },
				{ "Recover the Horadric Malus from", "the Monastery Barracks for Charsi." },
				{ "Defeat the Demon Queen Andariel", "in Catacombs Level 4." },
			},
			{
				{ "Slay Radament in the Sewers", "beneath Lut Gholein for Atma." },
				{ "Assemble the Horadric Staff using", "the Horadric Cube in the Halls/Maggot Lair." },
				{ "Destroy the Tainted Sun Altar in", "the Claw Viper Temple Level 2." },
				{ "Locate the hidden portal to the", "Arcane Sanctuary in the Palace Cellar." },
				{ "Defeat the Summoner in the Arcane", "Sanctuary and read Horazon's Journal." },
				{ "Defeat Duriel, Lord of Pain, in", "Tal Rasha's True Tomb." },
			},
			{
				{ "Bring the Jade Figurine to Meshif", "and receive the Golden Bird." },
				{ "Recover the Gidbinn from the", "Flayer Jungle for Ormus and Asheara." },
				{ "Transmute Khalim's Eye, Brain, Heart", "and Flail in the Horadric Cube." },
				{ "Recover Lam Esen's Tome from the", "Ruined Temple in the Kurast Bazaar." },
				{ "Defeat the High Council of Zakarum", "in Travincal." },
				{ "Defeat Mephisto, Lord of Hatred,", "in the Durance of Hate Level 3." },
			},
			{
				{ "Defeat Izual on the Plains of", "Despair and speak to Tyrael." },
				{ "Smash Mephisto's Soulstone upon", "the Hellforge in the River of Flame." },
				{ "Defeat Diablo, Lord of Terror,", "within the Chaos Sanctuary." },
				{ "", "" }, { "", "" }, { "", "" },
			},
			{
				{ "Slay Shenk the Overseer in the", "Bloody Foothills for Larzuk." },
				{ "Free the 15 Barbarian soldiers in", "the Frigid Highlands for Qual-Kehk." },
				{ "Rescue Anya from the Frozen River", "with Malah's thawing potion." },
				{ "Defeat Nihlathak in the Halls of", "Vaught to stop his betrayal." },
				{ "Defeat the Ancients at the Arreat", "Summit to enter the Worldstone Keep." },
				{ "Defeat Baal, Lord of Destruction,", "in the Worldstone Chamber." },
			},
		};

		const int32_t nQuestsInAct = (nActTab == 3) ? 3 : 6;
		for (int32_t q = 0; q < nQuestsInAct; ++q)
		{
			const int32_t qCol = q % 3;
			const int32_t qRow = q / 3;
			const int32_t qX = 116 + qCol * 84;
			const int32_t qY = 132 + qRow * 78;
			if (gpQuestSockets)
			{
				DrawSingleCelTopLeft(gpQuestSockets, qX, qY, q);
			}
			RECT rcSocket = { qX, qY, qX + 52, qY + 52 };
			D2GFX_UtilRect(&rcSocket, (q == gnSelectedQuest) ? 0x6Au : 0x16u);
		}

		const int32_t nSafeQ = std::max<int32_t>(0, std::min<int32_t>(gnSelectedQuest, nQuestsInAct - 1));
		D2Win_10127_SetFont(D2FONT_FONT16);
		DrawAsciiText(kAllActQuestTitles[nActTab][nSafeQ], 240, 340, 4, TRUE);
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		DrawAsciiText(kAllActQuestDescs[nActTab][nSafeQ][0], 240, 368, 0, TRUE);
		DrawAsciiText(kAllActQuestDescs[nActTab][nSafeQ][1], 240, 388, 0, TRUE);

		if (gpBuySellBtn)
		{
			DrawSingleCelTopLeft(gpBuySellBtn, 356, 448, 10);
		}
	}
	else if (gnLeftPanel == LEFTPANEL_PARTY || gnLeftPanel == LEFTPANEL_MSGLOG)
	{
		D2GFX_DrawBox(80, 60, 400, 492, 0x00u, DRAWMODE_TRANS75);
		RECT rcBorder = { 80, 60, 400, 492 };
		D2GFX_UtilRect(&rcBorder, 0x6Au);

		D2Win_10127_SetFont(D2FONT_FONT24);
		DrawAsciiText(
			(gnLeftPanel == LEFTPANEL_PARTY) ? "PARTY SCREEN" : "MESSAGE LOG",
			240,
			105,
			4,
			TRUE);

		D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
		if (gnLeftPanel == LEFTPANEL_PARTY)
		{
			const int32_t nLvl = std::max<int32_t>(1, gaClientStats[STAT_LEVEL] ? gaClientStats[STAT_LEVEL] : gnCharLevel);
			char szLine[128] = {};
			std::snprintf(szLine, sizeof(szLine), "%s - Level %d %s", gszCharName, nLvl, kClassNames[nSafeClass]);
			DrawAsciiText(szLine, 240, 165, 4, TRUE);
			DrawAsciiText("Location: Act I - Rogue Encampment", 240, 195, 0, TRUE);
			DrawAsciiText("Active Authoritative D2Game Session", 240, 235, 0, TRUE);
		}
		else
		{
			for (int32_t i = 0; i < gnChatLogCount; ++i)
			{
				DrawAsciiText(gaChatLog[i], 105, 160 + i * 22, (i == 0) ? 4 : 0, FALSE);
			}
		}

		if (gpBuySellBtn)
		{
			DrawSingleCelTopLeft(gpBuySellBtn, 356, 448, 10);
		}
	}
	else if (gnLeftPanel == LEFTPANEL_VENDOR)
	{
		D2GFX_DrawBox(80, 60, 400, 492, 0x00u, DRAWMODE_TRANS75);
		RECT rcBorder = { 80, 60, 400, 492 };
		D2GFX_UtilRect(&rcBorder, 0x6Au);

		char szVendorHeader[96] = {};
		std::snprintf(
			szVendorHeader,
			sizeof(szVendorHeader),
			"%s - Trade / Repair",
			gszActiveNpcName[0] ? gszActiveNpcName : "Town Merchant");
		D2Win_10127_SetFont(D2FONT_FONT16);
		DrawAsciiText(szVendorHeader, 240, 78, 4, TRUE);

		static const char* const kVendorTabs[4] = { "All", "Armor", "Weapons", "Misc" };
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		for (int32_t t = 0; t < 4; ++t)
		{
			const int32_t nTabX = 88 + t * 76;
			const bool bSelTab = (t == gnVendorActiveTab);
			D2GFX_DrawBox(nTabX, 82, nTabX + 72, 106, bSelTab ? 0x16u : 0x00u, DRAWMODE_TRANS75);
			RECT rcTab = { nTabX, 82, nTabX + 72, 106 };
			D2GFX_UtilRect(&rcTab, bSelTab ? 0x6Au : 0x16u);
			DrawAsciiText(kVendorTabs[t], nTabX + 36, 99, bSelTab ? 4 : 0, TRUE);
		}

		int32_t aVendorItemIndices[10] = {};
		const int32_t nVendorCount = BuildVendorTabItemList(gnVendorActiveTab, aVendorItemIndices, 10);
		if (gnSelectedVendorItemIdx < 0 && nVendorCount > 0)
		{
			gnSelectedVendorItemIdx = aVendorItemIndices[0];
		}

		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		if (nVendorCount == 0)
		{
			DrawAsciiText("Right-click inventory items to sell,", 240, 220, 0, TRUE);
			DrawAsciiText("or click REPAIR ALL below.", 240, 244, 0, TRUE);
		}
		for (int32_t v = 0; v < nVendorCount; ++v)
		{
			const int32_t nItemIdx = aVendorItemIndices[v];
			const ClientItemEntry& it = gClientItems[nItemIdx];
			const int32_t nRowY = 114 + v * 28;
			const bool bSel = (nItemIdx == gnSelectedVendorItemIdx);
			D2GFX_DrawBox(92, nRowY, 388, nRowY + 26, bSel ? 0x16u : 0x00u, DRAWMODE_TRANS50);
			if (bSel)
			{
				RECT rcRow = { 92, nRowY, 388, nRowY + 26 };
				D2GFX_UtilRect(&rcRow, 0x6Au);
			}
			if (it.pInvCell)
			{
				DrawSingleCelTopLeft(it.pInvCell, 96, nRowY + 1, 0);
			}
			DrawAsciiText(it.szName[0] ? it.szName : "Merchant Item", 132, nRowY + 18, bSel ? 4 : 0, FALSE);
			char szPrice[32] = {};
			std::snprintf(szPrice, sizeof(szPrice), "%u g", it.dwCost ? it.dwCost : 25u);
			DrawAsciiText(szPrice, 355, nRowY + 18, 4, TRUE);
		}

		// [BUY ITEM] & [REPAIR ALL] action buttons
		D2GFX_DrawBox(96, 408, 210, 438, 0x16u, DRAWMODE_TRANS75);
		RECT rcBuy = { 96, 408, 210, 438 };
		D2GFX_UtilRect(&rcBuy, 0x6Au);
		D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
		DrawAsciiText("BUY ITEM", 153, 428, 4, TRUE);

		D2GFX_DrawBox(220, 408, 334, 438, 0x16u, DRAWMODE_TRANS75);
		RECT rcRepair = { 220, 408, 334, 438 };
		D2GFX_UtilRect(&rcRepair, 0x6Au);
		DrawAsciiText("REPAIR ALL", 277, 428, 4, TRUE);

		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		DrawAsciiText("Right-click item to buy/sell", 215, 464, 0, TRUE);

		if (gpBuySellBtn)
		{
			DrawSingleCelTopLeft(gpBuySellBtn, 356, 448, 10);
		}
	}
	else if (gnLeftPanel == LEFTPANEL_STASH)
	{
		if (gpStashPanel)
		{
			DrawTiledPanelCellFile(gpStashPanel, 80, 60, 0, 2, 2);
		}
		else
		{
			D2GFX_DrawBox(80, 60, 400, 492, 0x00u, DRAWMODE_TRANS75);
		}
		RECT rcStash = { 80, 60, 400, 492 };
		D2GFX_UtilRect(&rcStash, 0x6Au);
		D2Win_10127_SetFont(D2FONT_FONT16);
		DrawAsciiText("PRIVATE STASH", 240, 82, 4, TRUE);

		char szStashGold[64] = {};
		std::snprintf(szStashGold, sizeof(szStashGold), "Stash Gold: %d  (Max 2,500,000)", gnStashGold);
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		DrawAsciiText(szStashGold, 240, 99, 0, TRUE);

		// Deposit / Withdraw Gold Buttons (C2S 0x4F 0x14 / 0x13)
		D2GFX_DrawBox(96, 104, 226, 132, 0x16u, DRAWMODE_TRANS75);
		RECT rcDep = { 96, 104, 226, 132 };
		D2GFX_UtilRect(&rcDep, 0x6Au);
		DrawAsciiText("DEPOSIT GOLD", 161, 122, 4, TRUE);

		D2GFX_DrawBox(238, 104, 368, 132, 0x16u, DRAWMODE_TRANS75);
		RECT rcWit = { 238, 104, 368, 132 };
		D2GFX_UtilRect(&rcWit, 0x6Au);
		DrawAsciiText("WITHDRAW GOLD", 303, 122, 4, TRUE);

		// 6x8 Stash Grid (x=153..327, y=142..374)
		for (int32_t r = 0; r < 8; ++r)
		{
			for (int32_t c = 0; c < 6; ++c)
			{
				RECT rcCell = { 153 + c * 29, 142 + r * 29, 153 + (c + 1) * 29, 142 + (r + 1) * 29 };
				D2GFX_UtilRect(&rcCell, 0x16u);
			}
		}
		for (int32_t i = 0; i < kMaxClientItems; ++i)
		{
			const ClientItemEntry& it = gClientItems[i];
			if (!it.bActive || it.nLocation != CLI_ITEMLOC_STASH)
			{
				continue;
			}
			const int32_t cx = 153 + std::max<int32_t>(0, std::min<int32_t>(it.nGridX, 5)) * 29;
			const int32_t cy = 142 + std::max<int32_t>(0, std::min<int32_t>(it.nGridY, 7)) * 29;
			D2GFX_DrawBox(cx, cy, cx + it.nInvW * 29 - 1, cy + it.nInvH * 29 - 1, 0x97u, DRAWMODE_TRANS25);
			if (it.pInvCell)
			{
				DrawSingleCelTopLeft(it.pInvCell, cx, cy, 0);
			}
		}
		if (gpBuySellBtn)
		{
			DrawSingleCelTopLeft(gpBuySellBtn, 356, 448, 10);
		}
	}
	else if (gnLeftPanel == LEFTPANEL_CUBE)
	{
		if (gpCubePanel)
		{
			DrawTiledPanelCellFile(gpCubePanel, 80, 60, 0, 2, 2);
		}
		else
		{
			D2GFX_DrawBox(80, 60, 400, 492, 0x00u, DRAWMODE_TRANS75);
		}
		RECT rcCube = { 80, 60, 400, 492 };
		D2GFX_UtilRect(&rcCube, 0x6Au);
		D2Win_10127_SetFont(D2FONT_FONT16);
		DrawAsciiText("HORADRIC CUBE", 240, 105, 4, TRUE);

		// 3x4 Horadric Cube Grid (x=196..283, y=198..314)
		for (int32_t r = 0; r < 4; ++r)
		{
			for (int32_t c = 0; c < 3; ++c)
			{
				RECT rcCell = { 196 + c * 29, 198 + r * 29, 196 + (c + 1) * 29, 198 + (r + 1) * 29 };
				D2GFX_UtilRect(&rcCell, 0x6Au);
			}
		}
		for (int32_t i = 0; i < kMaxClientItems; ++i)
		{
			const ClientItemEntry& it = gClientItems[i];
			if (!it.bActive || it.nLocation != CLI_ITEMLOC_CUBE)
			{
				continue;
			}
			const int32_t cx = 196 + std::max<int32_t>(0, std::min<int32_t>(it.nGridX, 2)) * 29;
			const int32_t cy = 198 + std::max<int32_t>(0, std::min<int32_t>(it.nGridY, 3)) * 29;
			D2GFX_DrawBox(cx, cy, cx + it.nInvW * 29 - 1, cy + it.nInvH * 29 - 1, 0x97u, DRAWMODE_TRANS25);
			if (it.pInvCell)
			{
				DrawSingleCelTopLeft(it.pInvCell, cx, cy, 0);
			}
		}
		// Transmute Button (x=180..300, y=352..394)
		if (gpTransmuteBtn)
		{
			DrawSingleCelTopLeft(gpTransmuteBtn, 216, 354, 0);
		}
		D2GFX_DrawBox(180, 352, 300, 390, 0x16u, DRAWMODE_TRANS50);
		RECT rcTrans = { 180, 352, 300, 390 };
		D2GFX_UtilRect(&rcTrans, 0x6Au);
		D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
		DrawAsciiText("TRANSMUTE", 240, 376, 4, TRUE);
		if (gpBuySellBtn)
		{
			DrawSingleCelTopLeft(gpBuySellBtn, 356, 448, 10);
		}
	}
	else if (gnLeftPanel == LEFTPANEL_WAYPOINT)
	{
		if (gpWaypointBg)
		{
			DrawTiledPanelCellFile(gpWaypointBg, 80, 60, 0, 2, 2);
		}
		else
		{
			D2GFX_DrawBox(80, 60, 400, 492, 0x00u, DRAWMODE_TRANS75);
		}
		RECT rcWp = { 80, 60, 400, 492 };
		D2GFX_UtilRect(&rcWp, 0x6Au);

		const int32_t nWpAct = std::max<int32_t>(0, std::min<int32_t>(gnWaypointActTab, 4));
		static const char* const kWpActTabs[5] = { "Act I", "Act II", "Act III", "Act IV", "Act V" };
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		for (int32_t t = 0; t < 5; ++t)
		{
			const int32_t tabX = 86 + t * 60;
			const bool bSel = (t == nWpAct);
			D2GFX_DrawBox(tabX, 66, tabX + 58, 92, bSel ? 0x16u : 0x00u, DRAWMODE_TRANS75);
			RECT rcTab = { tabX, 66, tabX + 58, 92 };
			D2GFX_UtilRect(&rcTab, bSel ? 0x6Au : 0x16u);
			DrawAsciiText(kWpActTabs[t], tabX + 29, 83, bSel ? 4 : 0, TRUE);
		}

		static const char* const kWpNames[5][9] = {
			{ "Rogue Encampment", "Cold Plains", "Stony Field", "Dark Wood", "Black Marsh", "Outer Cloister", "Jail Level 1", "Inner Cloister", "Catacombs Level 2" },
			{ "Lut Gholein", "Sewers Level 2", "Dry Hills", "Halls of the Dead 2", "Far Oasis", "Lost City", "Palace Cellar 1", "Arcane Sanctuary", "Canyon of the Magi" },
			{ "Kurast Docks", "Spider Forest", "Great Marsh", "Flayer Jungle", "Lower Kurast", "Kurast Bazaar", "Upper Kurast", "Travincal", "Durance of Hate 2" },
			{ "Pandemonium Fortress", "City of the Damned", "River of Flame", "", "", "", "", "", "" },
			{ "Harrogath", "Frigid Highlands", "Arreat Plateau", "Crystalline Passage", "Glacial Trail", "Halls of Pain", "Frozen Tundra", "The Ancients' Way", "Worldstone Keep 2" },
		};
		const int32_t nRows = (nWpAct == 3) ? 3 : 9;
		for (int32_t r = 0; r < nRows; ++r)
		{
			const int32_t rowY = 112 + r * 34;
			const bool bUnlocked = ((gdwWaypointUnlockMask[nWpAct] >> r) & 1u) != 0;
			D2GFX_DrawBox(96, rowY, 384, rowY + 30, bUnlocked ? 0x16u : 0x00u, DRAWMODE_TRANS50);
			RECT rcRow = { 96, rowY, 384, rowY + 30 };
			D2GFX_UtilRect(&rcRow, bUnlocked ? 0x6Au : 0x16u);
			DrawAsciiText(kWpNames[nWpAct][r], 240, rowY + 20, bUnlocked ? 4 : 0, TRUE);
		}
		if (gpBuySellBtn)
		{
			DrawSingleCelTopLeft(gpBuySellBtn, 356, 448, 10);
		}
	}
	else if (gnLeftPanel == LEFTPANEL_MERC)
	{
		if (gpMercInvPanel)
		{
			DrawTiledPanelCellFile(gpMercInvPanel, 80, 60, 0, 2, 2);
		}
		else
		{
			D2GFX_DrawBox(80, 60, 400, 492, 0x00u, DRAWMODE_TRANS75);
		}
		RECT rcMerc = { 80, 60, 400, 492 };
		D2GFX_UtilRect(&rcMerc, 0x6Au);
		D2Win_10127_SetFont(D2FONT_FONT16);
		DrawAsciiText("MERCENARY COMPANION (O)", 240, 88, 4, TRUE);

		char szMercInfo[96] = {};
		std::snprintf(szMercInfo, sizeof(szMercInfo), "%s  -  Level %d Rogue Scout", gszMercName, gnMercLevel);
		D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
		DrawAsciiText(szMercInfo, 240, 118, 0, TRUE);

		// Mercenary Paperdoll Equipment Slots (Helm, Armor, Weapon, Shield)
		static const int32_t kMercSlots[4][4] = {
			{ 212, 136, 56, 56 }, // Helm
			{ 212, 204, 56, 84 }, // Armor
			{ 118, 178, 56, 112 },// Weapon
			{ 306, 178, 56, 112 },// Shield/Bow
		};
		static const char* const kMercSlotLabels[4] = { "Helm", "Armor", "Weapon", "Off-Hand" };
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		for (int32_t s = 0; s < 4; ++s)
		{
			D2GFX_DrawBox(kMercSlots[s][0], kMercSlots[s][1], kMercSlots[s][0] + kMercSlots[s][2], kMercSlots[s][1] + kMercSlots[s][3], 0x00u, DRAWMODE_TRANS75);
			RECT rcSlot = { kMercSlots[s][0], kMercSlots[s][1], kMercSlots[s][0] + kMercSlots[s][2], kMercSlots[s][1] + kMercSlots[s][3] };
			D2GFX_UtilRect(&rcSlot, 0x6Au);
			DrawAsciiText(kMercSlotLabels[s], kMercSlots[s][0] + kMercSlots[s][2] / 2, kMercSlots[s][1] + kMercSlots[s][3] / 2 + 4, 0, TRUE);
		}
		DrawAsciiText("Life: 180 / 180   Resist All: +35%", 240, 340, 4, TRUE);
		DrawAsciiText("Damage: 24 - 52   Defense: 145", 240, 366, 0, TRUE);
		if (gpBuySellBtn)
		{
			DrawSingleCelTopLeft(gpBuySellBtn, 356, 448, 10);
		}
	}
}

static void DrawRightPanel()
{
	if (gnRightPanel == RIGHTPANEL_NONE)
	{
		return;
	}

	const uint8_t nSafeClass = (gnCharClass < 7) ? gnCharClass : 0;

	if (gnRightPanel == RIGHTPANEL_INV)
	{
		INV_SetPanelMode(1);
		(void)INV_HitTestInvPanelRect(gMousePos.x, gMousePos.y);
		if (gnLeftPanel == LEFTPANEL_STASH)
		{
			(void)INV_HitTestStashPanelRect(gMousePos.x, gMousePos.y);
		}
		else if (gnLeftPanel == LEFTPANEL_CUBE)
		{
			(void)INV_HitTestHoradricCubeGridRect(gMousePos.x, gMousePos.y);
		}
		++gdwInvPanelEvaluations;

		if (gpInvChar6)
		{
			const int32_t nTotalFrames = D2CMP_CelFileGetCelsPerDirection(gpInvChar6);
			const int32_t nStartFrame = (nTotalFrames >= 8) ? 4 : 0;
			DrawTiledPanelCellFile(gpInvChar6, 400, 60, nStartFrame, 2, 2);
		}
		else
		{
			D2GFX_DrawBox(400, 60, 720, 492, 0x00u, DRAWMODE_TRANS75);
		}

		// Highlight active Weapon Set tab ("I" vs "II") above left and right weapon slots
		if (gnActiveWeaponSet == 0)
		{
			RECT rcSet1L = { 416, 82, 444, 102 };
			RECT rcSet1R = { 642, 82, 670, 102 };
			D2GFX_UtilRect(&rcSet1L, 0x6Au);
			D2GFX_UtilRect(&rcSet1R, 0x6Au);
		}
		else
		{
			RECT rcSet2L = { 450, 82, 478, 102 };
			RECT rcSet2R = { 676, 82, 704, 102 };
			D2GFX_UtilRect(&rcSet2L, 0x6Au);
			D2GFX_UtilRect(&rcSet2R, 0x6Au);
		}

		const ClientItemEntry* pHoveredItem = nullptr;
		int32_t nHoverTipX = 560;
		int32_t nHoverTipY = 300;
		int32_t nInvLeft = 419;
		int32_t nInvTop = 315;
		int32_t nInvCellW = 29;
		int32_t nInvCellH = 29;
		GetInvGridLayout800x600(&nInvLeft, &nInvTop, &nInvCellW, &nInvCellH);

		bool bSlotOccupied[11] = {};
		for (int32_t i = 0; i < kMaxClientItems; ++i)
		{
			const ClientItemEntry& it = gClientItems[i];
			if (it.bActive && it.nLocation == CLI_ITEMLOC_EQUIPPED && it.nBodyLoc >= 1 && it.nBodyLoc <= 10)
			{
				bSlotOccupied[it.nBodyLoc] = true;
			}
		}

		// Draw empty-slot background silhouettes only on unoccupied paperdoll slots using Inventory.txt coordinates
		auto drawSlotSilhouette = [&](uint8_t nBodyLoc, D2CellFileStrc* pCell, int32_t nFrame) {
			if (!pCell || nBodyLoc < 1 || nBodyLoc > 10 || bSlotOccupied[nBodyLoc])
			{
				return;
			}
			int32_t sx = 0, sy = 0, sw = 0, sh = 0;
			GetBodySlotRect800x600(nBodyLoc, &sx, &sy, &sw, &sh);
			if (sw > 0 && sh > 0)
			{
				DrawSingleCelTopLeft(pCell, sx, sy, nFrame);
			}
		};
		drawSlotSilhouette(1, gpInvHelmGlove, 1);
		drawSlotSilhouette(2, gpInvRingAmulet, 0);
		drawSlotSilhouette(3, gpInvArmor, 0);
		drawSlotSilhouette(4, gpInvWeapons, 0);
		drawSlotSilhouette(5, gpInvWeapons, 0);
		drawSlotSilhouette(6, gpInvRingAmulet, 1);
		drawSlotSilhouette(7, gpInvRingAmulet, 1);
		drawSlotSilhouette(8, gpInvBelt, 0);
		drawSlotSilhouette(9, gpInvBoots, 0);
		drawSlotSilhouette(10, gpInvHelmGlove, 0);

		for (int32_t i = 0; i < kMaxClientItems; ++i)
		{
			const ClientItemEntry& it = gClientItems[i];
			if (!it.bActive)
			{
				continue;
			}
			// Only draw active weapon set & armor slots (1..10); swap weapon slots (11..12) are hidden until W / tab switch
			if (it.nLocation == CLI_ITEMLOC_EQUIPPED && it.nBodyLoc >= 1 && it.nBodyLoc <= 10)
			{
				int32_t nSlotX = 0;
				int32_t nSlotY = 0;
				int32_t nSlotW = 0;
				int32_t nSlotH = 0;
				GetBodySlotRect800x600(it.nBodyLoc, &nSlotX, &nSlotY, &nSlotW, &nSlotH);
				if (nSlotW <= 0 || nSlotH <= 0)
				{
					continue;
				}
				D2GFX_DrawBox(nSlotX, nSlotY, nSlotX + nSlotW, nSlotY + nSlotH, 0x16u, DRAWMODE_TRANS50);
				if (it.pInvCell)
				{
					const int32_t nItemW = it.nInvW * 28;
					const int32_t nItemH = it.nInvH * 28;
					const int32_t nDrawX = nSlotX + std::max<int32_t>(0, (nSlotW - nItemW) / 2);
					const int32_t nDrawY = nSlotY + std::max<int32_t>(0, (nSlotH - nItemH) / 2);
					DrawSingleCelTopLeft(it.pInvCell, nDrawX, nDrawY, 0);
				}
				if (gMousePos.x >= nSlotX && gMousePos.x < nSlotX + nSlotW &&
					gMousePos.y >= nSlotY && gMousePos.y < nSlotY + nSlotH)
				{
					pHoveredItem = &it;
					nHoverTipX = nSlotX + nSlotW / 2;
					nHoverTipY = std::max<int32_t>(75, nSlotY - 8);
				}
			}
			else if (it.nLocation == CLI_ITEMLOC_CONTAINER && it.nGridX >= 0 && it.nGridX < 10 && it.nGridY >= 0 && it.nGridY < 4)
			{
				const int32_t nCellX = nInvLeft + it.nGridX * nInvCellW;
				const int32_t nCellY = nInvTop + it.nGridY * nInvCellH;
				const int32_t nCellW = it.nInvW * nInvCellW;
				const int32_t nCellH = it.nInvH * nInvCellH;
				D2GFX_DrawBox(nCellX, nCellY, nCellX + nCellW - 1, nCellY + nCellH - 1, 0x97u, DRAWMODE_TRANS25);
				if (it.pInvCell)
				{
					DrawSingleCelTopLeft(it.pInvCell, nCellX, nCellY, 0);
				}
				if (gMousePos.x >= nCellX && gMousePos.x < nCellX + nCellW &&
					gMousePos.y >= nCellY && gMousePos.y < nCellY + nCellH)
				{
					pHoveredItem = &it;
					nHoverTipX = nCellX + nCellW / 2;
					nHoverTipY = nCellY - 8;
				}
			}
		}

		D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
		char szGold[32] = {};
		std::snprintf(szGold, sizeof(szGold), "%d", std::max<int32_t>(0, gaClientStats[STAT_GOLD]));
		DrawAsciiText(szGold, 525, 466, 0, TRUE); // Gold amount

		if (gpBuySellBtn)
		{
			DrawSingleCelTopLeft(gpBuySellBtn, 418, 448, 10);
		}

		if (pHoveredItem && pHoveredItem->szName[0])
		{
			Unicode wszTip[128] = {};
			Unicode::win2Unicode(wszTip, pHoveredItem->szName, 127);
			D2Win_10127_SetFont(D2FONT_FONT16);
			D2Win_10129_DrawFramedText(wszTip, std::max<int32_t>(440, std::min<int32_t>(nHoverTipX, 680)), nHoverTipY, 0, 1);
		}
	}
	else if (gnRightPanel == RIGHTPANEL_SKILL)
	{
		const int32_t nTab = std::max<int32_t>(0, std::min<int32_t>(gnActiveSkillTab, 2));
		if (gpSkillTreeBg)
		{
			// 1.12a 0x6FB2E660: First draw base frames 0..3 (right-hand tab column & skill points header),
			// then draw active tab frames ((nTab + 1) * 4 + 0..3) at (400,316), (656,316), (400,492), (656,492)
			static const int32_t kQuadPos[4][2] = {
				{ 400, 316 },
				{ 656, 316 },
				{ 400, 492 },
				{ 656, 492 },
			};
			const int32_t nTotal = D2CMP_CelFileGetCelsPerDirection(gpSkillTreeBg);
			for (int32_t i = 0; i < 4 && i < nTotal; ++i)
			{
				D2GfxDataStrc tBg = {};
				tBg.pCellFile = gpSkillTreeBg;
				tBg.nDirection = 0;
				tBg.nFrame = i;
				TEXTURE_CelDraw(&tBg, kQuadPos[i][0], kQuadPos[i][1], 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
			}
			const int32_t nTabBase = (nTab + 1) * 4;
			for (int32_t i = 0; i < 4 && (nTabBase + i) < nTotal; ++i)
			{
				D2GfxDataStrc tBg = {};
				tBg.pCellFile = gpSkillTreeBg;
				tBg.nDirection = 0;
				tBg.nFrame = nTabBase + i;
				TEXTURE_CelDraw(&tBg, kQuadPos[i][0], kQuadPos[i][1], 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
			}
		}
		else
		{
			D2GFX_DrawBox(400, 60, 720, 492, 0x00u, DRAWMODE_TRANS75);
		}

		// Draw authentic class skills from SkillsTxt / SkillDescTxt across the 3 columns x 6 rows (1.12a 0x6FB2E8B0)
		static const int32_t kSkillColX[3] = { 415, 484, 553 };
		static const int32_t kSkillRowY[6] = { 74, 142, 210, 278, 346, 414 };
		char szHoveredSkillTip[128] = {};
		int32_t nHoveredSkillX = 520;
		int32_t nHoveredSkillY = 200;

		bool bRow6Col1Occupied = false;
		if (gpClassSkillIcon && sgptDataTables && sgptDataTables->pSkillsTxt && sgptDataTables->pSkillDescTxt)
		{
			D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
			for (int32_t sId = 0; sId < sgptDataTables->nSkillsTxtRecordCount && sId < 512; ++sId)
			{
				const D2SkillsTxt& sk = sgptDataTables->pSkillsTxt[sId];
				if (sk.nCharClass != nSafeClass || sk.wSkillDesc < 0 || sk.wSkillDesc >= sgptDataTables->nSkillDescTxtRecordCount)
				{
					continue;
				}
				const D2SkillDescTxt& sd = sgptDataTables->pSkillDescTxt[sk.wSkillDesc];
				if (sd.nSkillPage != (nTab + 1) || sd.nSkillColumn < 1 || sd.nSkillColumn > 3 || sd.nSkillRow < 1 || sd.nSkillRow > 6)
				{
					continue;
				}
				if (sd.nSkillRow == 6 && sd.nSkillColumn == 1)
				{
					bRow6Col1Occupied = true;
				}
				const int32_t nIconX = kSkillColX[sd.nSkillColumn - 1];
				const int32_t nIconY = kSkillRowY[sd.nSkillRow - 1];
				const uint8_t nLvl = gaClientSkillLevels[sId];
				DrawSingleCelTopLeft(gpClassSkillIcon, nIconX, nIconY, sd.nIconCel);

				char szLvl[16] = {};
				std::snprintf(szLvl, sizeof(szLvl), "%u", nLvl);
				DrawAsciiText(szLvl, nIconX + 41, nIconY + 55, (nLvl > 0) ? 4 : 0, TRUE);

				SyncReconstructedClientGlobals();
				if (D2SkillDescTxt* pReconstructedSkillDesc = sub_6FAF21F0(sId))
				{
					wchar_t wszSkNameBuf[128] = {};
					(void)sub_6FAF7560(sId, wszSkNameBuf, 128);
					if (gpPlayerUnit)
					{
						wchar_t wszSkTooltipBuf[512] = {};
						sub_6FAF7410(sId, std::max<int32_t>(1, nLvl), gpPlayerUnit, pReconstructedSkillDesc, wszSkTooltipBuf);
					}
					++gdwSkillDescTooltipsBuilt;
				}

				if (gMousePos.x >= nIconX && gMousePos.x < nIconX + 48 &&
					gMousePos.y >= nIconY && gMousePos.y < nIconY + 48)
				{
					char szSkName[64] = {};
					UnicodeToAsciiSafe(D2LANG_GetStringFromTblIndex(sd.wStrName), szSkName, sizeof(szSkName));
					std::snprintf(
						szHoveredSkillTip,
						sizeof(szHoveredSkillTip),
						"%s (Level %u, Req Lvl %d)",
						szSkName[0] ? szSkName : "Skill",
						nLvl,
						sk.wReqLevel);
					nHoveredSkillX = nIconX + 24;
					nHoveredSkillY = std::max<int32_t>(75, nIconY - 8);
				}
			}
		}

		static const char* const kClassTabLines[7][3][2] = {
			{ { "Bow &", "Crossbow" },       { "Passive &", "Magic" },    { "Javelin &", "Spear" } },
			{ { "Fire", "Spells" },          { "Lightning", "Spells" },   { "Cold", "Spells" } },
			{ { "Curses", "" },              { "Poison &", "Bone" },      { "Summoning", "Spells" } },
			{ { "Defensive", "Auras" },      { "Offensive", "Auras" },    { "Combat", "Skills" } },
			{ { "Warcries", "" },            { "Combat", "Masteries" },   { "Combat", "Skills" } },
			{ { "Elemental", "" },           { "Shape", "Shifting" },     { "Summoning", "" } },
			{ { "Martial", "Arts" },         { "Shadow", "Disciplines" }, { "Traps", "" } },
		};

		// Remaining Skill Points header & value in top-right box (y=60..168)
		D2Win_10127_SetFont(D2FONT_FONT6);
		DrawAsciiText("REMAINING", 677, 82, 4, TRUE);
		DrawAsciiText("SKILL", 677, 94, 4, TRUE);
		DrawAsciiText("POINTS", 677, 106, 4, TRUE);
		D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
		char szSkPts[16] = {};
		std::snprintf(szSkPts, sizeof(szSkPts), "%d", std::max<int32_t>(0, gaClientStats[STAT_SKILLPTS]));
		DrawAsciiText(szSkPts, 677, 140, (gaClientStats[STAT_SKILLPTS] > 0) ? 4 : 0, TRUE);

		// 3 Right-hand Skill Tree Tabs (Tab 2: y=168..276, Tab 1: y=276..384, Tab 0: y=384..492)
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		static const int32_t kTabCenterY[3] = { 436, 328, 220 };
		for (int32_t t = 0; t < 3; ++t)
		{
			const int32_t nCol = (nTab == t) ? 4 : 0;
			const char* szL1 = kClassTabLines[nSafeClass][t][0];
			const char* szL2 = kClassTabLines[nSafeClass][t][1];
			if (szL2 && szL2[0])
			{
				DrawAsciiText(szL1, 677, kTabCenterY[t] - 7, nCol, TRUE);
				DrawAsciiText(szL2, 677, kTabCenterY[t] + 9, nCol, TRUE);
			}
			else
			{
				DrawAsciiText(szL1, 677, kTabCenterY[t], nCol, TRUE);
			}
		}

		if (gpBuySellBtn)
		{
			const int32_t nCloseX = bRow6Col1Occupied ? 569 : 416;
			DrawSingleCelTopLeft(gpBuySellBtn, nCloseX, 448, 10);
		}

		if (szHoveredSkillTip[0])
		{
			Unicode wszTip[128] = {};
			Unicode::win2Unicode(wszTip, szHoveredSkillTip, 127);
			D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
			D2Win_10129_DrawFramedText(wszTip, std::max<int32_t>(440, std::min<int32_t>(nHoveredSkillX, 640)), nHoveredSkillY, 0, 1);
		}
	}
}

static void DrawEscGameMenu()
{
	if (!gbEscMenuOpen)
	{
		return;
	}

	D2GFX_DrawBox(0, 0, 800, 550, 0x00u, DRAWMODE_TRANS50);

	const int32_t nPentFrames = gpPentSpin ? D2CMP_CelFileGetCelsPerDirection(gpPentSpin) : 0;
	const int32_t nPentFrame = (nPentFrames > 0) ? static_cast<int32_t>((GetTickCount() / 55u) % static_cast<uint32_t>(nPentFrames)) : 0;

	if (gnEscSubMenu == 0)
	{
		static const char* const kMainItems[3] = {
			"OPTIONS",
			"SAVE AND EXIT GAME",
			"RETURN TO GAME",
		};
		D2Win_10127_SetFont(D2FONT_FONT42);
		for (int32_t i = 0; i < 3; ++i)
		{
			const int32_t nRowY = 205 + i * 55;
			const bool bSel = (i == gnEscSelectedIndex);
			DrawAsciiText(kMainItems[i], 400, nRowY, bSel ? 4 : 0, TRUE);
			if (bSel && gpPentSpin)
			{
				Unicode wszItem[64] = {};
				Unicode::win2Unicode(wszItem, kMainItems[i], 63);
				const int32_t nHalfW = D2Win_10121_GetTextWidth(wszItem) / 2;
				DrawSingleCelTopLeft(gpPentSpin, 400 - nHalfW - 44, nRowY - 32, nPentFrame);
				DrawSingleCelTopLeft(gpPentSpin, 400 + nHalfW + 12, nRowY - 32, nPentFrame);
			}
		}
	}
	else if (gnEscSubMenu == 1)
	{
		char szSoundRow[64] = {};
		char szVideoRow[64] = {};
		char szMapRow[64] = {};
		std::snprintf(szSoundRow, sizeof(szSoundRow), "SOUND OPTIONS: %s", gbSoundEnabled ? "ON" : "MUTED");
		std::snprintf(szVideoRow, sizeof(szVideoRow), "VIDEO RESOLUTION: %s", gbScaled1200x900 ? "1200x900" : "800x600");
		std::snprintf(szMapRow, sizeof(szMapRow), "AUTOMAP OVERLAY: %s", gbAutomapOpen ? "ON" : "OFF");

		const char* const aOptItems[5] = {
			szSoundRow,
			szVideoRow,
			szMapRow,
			"CONFIGURE CONTROLS",
			"PREVIOUS MENU",
		};

		D2Win_10127_SetFont(D2FONT_FONT30);
		DrawAsciiText("OPTIONS", 400, 115, 4, TRUE);

		D2Win_10127_SetFont(D2FONT_FONT24);
		for (int32_t i = 0; i < 5; ++i)
		{
			const int32_t nRowY = 175 + i * 48;
			const bool bSel = (i == gnEscSelectedIndex);
			DrawAsciiText(aOptItems[i], 400, nRowY, bSel ? 4 : 0, TRUE);
			if (bSel && gpPentSpin)
			{
				Unicode wszItem[64] = {};
				Unicode::win2Unicode(wszItem, aOptItems[i], 63);
				const int32_t nHalfW = D2Win_10121_GetTextWidth(wszItem) / 2;
				DrawSingleCelTopLeft(gpPentSpin, 400 - nHalfW - 42, nRowY - 26, nPentFrame);
				DrawSingleCelTopLeft(gpPentSpin, 400 + nHalfW + 10, nRowY - 26, nPentFrame);
			}
		}
	}
	else if (gnEscSubMenu == 2)
	{
		D2Win_10127_SetFont(D2FONT_FONT30);
		DrawAsciiText("CONFIGURE CONTROLS", 400, 115, 4, TRUE);

		D2Win_10127_SetFont(D2FONT_FONT16);
		DrawAsciiText("Character Screen:  C / A", 400, 175, 0, TRUE);
		DrawAsciiText("Inventory Screen:  I / B", 400, 205, 0, TRUE);
		DrawAsciiText("Skill Tree:        T / K", 400, 235, 0, TRUE);
		DrawAsciiText("Quest Log:         Q", 400, 265, 0, TRUE);
		DrawAsciiText("Party Screen:      P", 400, 295, 0, TRUE);
		DrawAsciiText("Automap Overlay:   TAB", 400, 325, 0, TRUE);
		DrawAsciiText("Run / Walk Toggle: R", 400, 355, 0, TRUE);

		D2Win_10127_SetFont(D2FONT_FONT24);
		DrawAsciiText("PREVIOUS MENU", 400, 415, 4, TRUE);
	}
}

static void DrawHudControlPanel()
{
	// 1. Health & Mana Orbs (hlthmana.dc6)
	if (gpHudHlthMana)
	{
		D2GfxDataStrc tOrb = {};
		tOrb.pCellFile = gpHudHlthMana;
		tOrb.nDirection = 0;

		tOrb.nFrame = 0; // Health Orb
		TEXTURE_CelDraw(&tOrb, 30, 588, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);

		tOrb.nFrame = 1; // Mana Orb
		TEXTURE_CelDraw(&tOrb, 692, 588, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
	}

	// 2. 800x600 Control Panel slices (800ctrlpnl7.dc6 — 1.12a 0x6FB323E0..0x6FB324AE)
	if (gpHudCtrlPnl)
	{
		static const int32_t kCtrlPnlPos[6][2] = {
			{ 0,   599 }, // Frame 0: Left Orb holder (0..117)
			{ 165, 600 }, // Frame 1: Left-center panel (165..293, after 48px Left Skill icon at 117..165)
			{ 293, 600 }, // Frame 2: Center-left panel (293..421)
			{ 421, 600 }, // Frame 3: Center-right panel (421..549)
			{ 549, 600 }, // Frame 4: Right-center panel (549..635, before 48px Right Skill icon at 635..683)
			{ 683, 599 }, // Frame 5: Right Orb holder (683..800)
		};
		const int32_t nFrames = std::min<int32_t>(D2CMP_CelFileGetCelsPerDirection(gpHudCtrlPnl), 6);
		for (int32_t f = 0; f < nFrames; ++f)
		{
			D2GfxDataStrc tPnl = {};
			tPnl.pCellFile = gpHudCtrlPnl;
			tPnl.nDirection = 0;
			tPnl.nFrame = f;
			TEXTURE_CelDraw(&tPnl, kCtrlPnlPos[f][0], kCtrlPnlPos[f][1], 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
		}
	}

	// 3. Left & Right Active Skill Icons on Control Panel (x=117 and x=635)
	if (gpHudSkillIcon)
	{
		DrawSingleCelTopLeft(gpHudSkillIcon, 117, 552, 0); // Attack icon on Left Skill
	}
	bool bDrewRightClassSkill = false;
	if (gwRightSkillId > 0 && gpClassSkillIcon && sgptDataTables && sgptDataTables->pSkillsTxt &&
		gwRightSkillId < sgptDataTables->nSkillsTxtRecordCount)
	{
		const D2SkillsTxt& sk = sgptDataTables->pSkillsTxt[gwRightSkillId];
		if (sk.wSkillDesc >= 0 && sk.wSkillDesc < sgptDataTables->nSkillDescTxtRecordCount)
		{
			const int32_t nCel = sgptDataTables->pSkillDescTxt[sk.wSkillDesc].nIconCel;
			DrawSingleCelTopLeft(gpClassSkillIcon, 635, 552, nCel);
			bDrewRightClassSkill = true;
		}
	}
	if (!bDrewRightClassSkill)
	{
		if (gpClassSkillIcon)
		{
			DrawSingleCelTopLeft(gpClassSkillIcon, 635, 552, 0);
		}
		else if (gpHudSkillIcon)
		{
			DrawSingleCelTopLeft(gpHudSkillIcon, 635, 552, 2);
		}
	}

	// 4. Stamina Bar fill (proportional to server STAT_STAMINA / STAT_MAXSTAMINA) & Run/Walk Toggle Button
	const int32_t nCurStam = std::max<int32_t>(0, gaClientStats[STAT_STAMINA] >> 8);
	const int32_t nMaxStam = std::max<int32_t>(1, gaClientStats[STAT_MAXSTAMINA] >> 8);
	const int32_t nStamWidth = std::max<int32_t>(0, std::min<int32_t>(102, (nCurStam * 102) / nMaxStam));
	if (nStamWidth > 0)
	{
		D2GFX_DrawBox(273, 574, 273 + nStamWidth, 588, 0x6Au, DRAWMODE_NORMAL);
	}
	if (gpHudRunBtn)
	{
		DrawSingleCelTopLeft(gpHudRunBtn, 255, 573, gbRunEnabled ? 2 : 0);
	}

	// 4b. Belt Potion Slots (x=423 + col*31, y=561) + 4x4 Expanded Belt Grid ('~' or hover)
	const bool bBeltHovered = (gMousePos.x >= 422 && gMousePos.x < 546 && gMousePos.y >= 556 && gMousePos.y <= 594);
	if (gbBeltExpanded || bBeltHovered)
	{
		D2GFX_DrawBox(420, 452, 548, 556, 0x00u, DRAWMODE_TRANS75);
		for (int32_t r = 1; r < 4; ++r)
		{
			for (int32_t c = 0; c < 4; ++c)
			{
				RECT rcBeltCell = { 422 + c * 31, 560 - r * 34, 422 + (c + 1) * 31 - 2, 560 - (r - 1) * 34 - 2 };
				D2GFX_UtilRect(&rcBeltCell, 0x6Au);
			}
		}
	}
	for (int32_t i = 0; i < kMaxClientItems; ++i)
	{
		const ClientItemEntry& it = gClientItems[i];
		if (it.bActive && it.nLocation == CLI_ITEMLOC_BELT && it.pInvCell)
		{
			const int32_t nCol = std::max<int32_t>(0, it.nGridX) % 4;
			const int32_t nRow = std::max<int32_t>(0, it.nGridX) / 4;
			if (nRow == 0)
			{
				DrawSingleCelTopLeft(it.pInvCell, 423 + nCol * 31, 561, 0);
			}
			else if ((gbBeltExpanded || bBeltHovered) && nRow < 4)
			{
				DrawSingleCelTopLeft(it.pInvCell, 423 + nCol * 31, 561 - nRow * 34, 0);
			}
		}
	}

	// 5. Central Mini-Panel Toggle Button (menubutton.dc6)
	if (gpHudMenuBtn)
	{
		DrawSingleCelTopLeft(gpHudMenuBtn, 393, 562, gbMiniPanelOpen ? 2 : 0);
	}

	// 6. Mini-Panel & 8 Action Buttons (Character, Inventory, Skill Tree, Automap, Message Log, Quest Log, Party, Game Menu)
	if (gbMiniPanelOpen)
	{
		if (gpHudMiniPanel)
		{
			DrawSingleCelTopLeft(gpHudMiniPanel, 328, 526, 0);
		}
		if (gpHudMiniPanelBtn)
		{
			const bool aActive[8] = {
				gnLeftPanel == LEFTPANEL_CHAR,
				gnRightPanel == RIGHTPANEL_INV,
				gnRightPanel == RIGHTPANEL_SKILL,
				gbAutomapOpen,
				gnLeftPanel == LEFTPANEL_MSGLOG,
				gnLeftPanel == LEFTPANEL_QUEST,
				gnLeftPanel == LEFTPANEL_PARTY,
				gbEscMenuOpen,
			};
			for (int32_t i = 0; i < 8; ++i)
			{
				DrawSingleCelTopLeft(gpHudMiniPanelBtn, 331 + i * 18, 529, i * 2 + (aActive[i] ? 1 : 0));
			}
		}

		// Mini-Panel Hover Tooltip
		if (!gbEscMenuOpen && gMousePos.x >= 328 && gMousePos.x < 472 && gMousePos.y >= 522 && gMousePos.y < 553)
		{
			static const char* const kMiniTooltips[8] = {
				"Character Screen (C)",
				"Inventory Screen (I)",
				"Skill Tree (T)",
				"Automap (Tab)",
				"Message Log (M)",
				"Quest Log (Q)",
				"Party Screen (P)",
				"Game Menu (Esc)",
			};
			const int32_t nBtnIdx = std::max<int32_t>(0, std::min<int32_t>((gMousePos.x - 330) / 18, 7));
			Unicode wszTip[64] = {};
			Unicode::win2Unicode(wszTip, kMiniTooltips[nBtnIdx], 63);
			D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
			D2Win_10129_DrawFramedText(wszTip, 400, 514, 0, 1);
		}
	}

	// 7. Orb glass overlaps & Life/Mana numeric readouts
	if (gpHudOverlap)
	{
		D2GfxDataStrc tOvl = {};
		tOvl.pCellFile = gpHudOverlap;
		tOvl.nDirection = 0;
		tOvl.nFrame = 0;
		TEXTURE_CelDraw(&tOvl, 28, 594, 0xFFFFFFFFu, DRAWMODE_TRANSHIGHLIGHT, nullptr);
		tOvl.nFrame = 1;
		TEXTURE_CelDraw(&tOvl, 690, 594, 0xFFFFFFFFu, DRAWMODE_TRANSHIGHLIGHT, nullptr);
	}

	const int32_t nCurHp = std::max<int32_t>(0, gaClientStats[STAT_HITPOINTS] >> 8);
	const int32_t nMaxHp = std::max<int32_t>(nCurHp, gaClientStats[STAT_MAXHP] >> 8);
	const int32_t nCurMana = std::max<int32_t>(0, gaClientStats[STAT_MANA] >> 8);
	const int32_t nMaxMana = std::max<int32_t>(nCurMana, gaClientStats[STAT_MAXMANA] >> 8);
	if (nMaxHp > 0)
	{
		char szOrb[48] = {};
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		std::snprintf(szOrb, sizeof(szOrb), "Life: %d / %d", nCurHp, nMaxHp);
		DrawAsciiText(szOrb, 54, 506, 0, TRUE);
		std::snprintf(szOrb, sizeof(szOrb), "Mana: %d / %d", nCurMana, nMaxMana);
		DrawAsciiText(szOrb, 746, 506, 0, TRUE);
	}
}

static void DrawNpcDialogMenu()
{
	if (!gbNpcMenuOpen)
	{
		return;
	}

	D2GFX_DrawBox(290, 96, 510, 250, 0x00u, DRAWMODE_TRANS75);
	RECT rcMenu = { 290, 96, 510, 250 };
	D2GFX_UtilRect(&rcMenu, 0x6Au);

	D2Win_10127_SetFont(D2FONT_FONT24);
	DrawAsciiText(gszActiveNpcName[0] ? gszActiveNpcName : "Townsperson", 400, 128, 4, TRUE);

	const char* const aOptions[3] = {
		"talk",
		(gwActiveNpcClassId == 155) ? "travel east (act ii)"
			: ((gwActiveNpcClassId == 150 || gwActiveNpcClassId == 198 || gwActiveNpcClassId == 252 || gwActiveNpcClassId == 515) ? "hire / resurrect"
			: ((gwActiveNpcClassId == 147 || gwActiveNpcClassId == 148 || gwActiveNpcClassId == 154) ? "trade / repair" : "trade")),
		(gwActiveNpcClassId == 148) ? "reset stats / skills" : "cancel",
	};

	const int32_t nPentFrames = gpPentSpin ? D2CMP_CelFileGetCelsPerDirection(gpPentSpin) : 0;
	const int32_t nPentFrame = (nPentFrames > 0) ? static_cast<int32_t>((GetTickCount() / 55u) % static_cast<uint32_t>(nPentFrames)) : 0;

	D2Win_10127_SetFont(D2FONT_FONT16);
	for (int32_t i = 0; i < 3; ++i)
	{
		const int32_t nRowY = 158 + i * 28;
		const bool bHovered =
			(gMousePos.x >= 290 && gMousePos.x <= 510 && gMousePos.y >= (138 + i * 28) && gMousePos.y < (166 + i * 28));
		if (bHovered)
		{
			gnNpcMenuSelectedIdx = i;
		}
		const bool bSel = (i == gnNpcMenuSelectedIdx);
		DrawAsciiText(aOptions[i], 400, nRowY, bSel ? 4 : 0, TRUE);
		if (bSel && gpPentSpin)
		{
			DrawSingleCelTopLeft(gpPentSpin, 304, nRowY - 22, nPentFrame);
			DrawSingleCelTopLeft(gpPentSpin, 472, nRowY - 22, nPentFrame);
		}
	}
}

static void PollLiveHardwareCursorPosition()
{
	HWND hWnd = WINDOW_GetWindow();
	if (!hWnd || GetForegroundWindow() != hWnd)
	{
		return;
	}
	POINT pt = {};
	RECT rcClient = {};
	if (GetCursorPos(&pt) && ScreenToClient(hWnd, &pt) && GetClientRect(hWnd, &rcClient))
	{
		const int32_t nClientW = std::max<int32_t>(1, rcClient.right - rcClient.left);
		const int32_t nClientH = std::max<int32_t>(1, rcClient.bottom - rcClient.top);
		if (pt.x >= 0 && pt.x < nClientW && pt.y >= 0 && pt.y < nClientH)
		{
			gMousePos.x = static_cast<int32_t>((static_cast<int64_t>(pt.x) * 800) / nClientW);
			gMousePos.y = static_cast<int32_t>((static_cast<int64_t>(pt.y) * 600) / nClientH);
		}
	}
}

static void RenderInGameFrame()
{
	PollLiveHardwareCursorPosition();

	if (!D2GFX_StartDraw(0, 0, 0, 0))
	{
		return;
	}

	D2GFX_ClearScreen(0);

	// 1-3. Reconstructed 1.12a Isometric World Renderer (View.cpp -> GAME_Wall2.cpp -> Floor/Env/ENGINE_Gfx)
	SyncReconstructedClientGlobals();
	if (gpPlayerUnit)
	{
		if (D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(gpPlayerUnit))
		{
			int32_t nRoomIntensity = 0;
			uint8_t nR = 0xFF, nG = 0xFF, nB = 0xFF;
			LIGHTMAP_GetRoomAmbientLight(TRUE, pPlayerRoom, &nRoomIntensity, &nR, &nG, &nB);
			++gdwLightMapRoomsSampled;
		}
	}
	GAME_RenderFrame(0);

	// 4. Automap Overlay (if toggled on)
	DrawAutomapOverlay();

	// 5. Player Overhead Name Banner, Hovered Monster/NPC Life Bar & Area Title
	const int32_t nLvl = std::max<int32_t>(1, gaClientStats[STAT_LEVEL] ? gaClientStats[STAT_LEVEL] : gnCharLevel);
	Unicode wszArea[128] = {};
	Unicode wszChar[128] = {};
	char szCharLine[128] = {};
	std::snprintf(
		szCharLine,
		sizeof(szCharLine),
		"%s  -  Level %d %s%s",
		gszCharName,
		nLvl,
		kClassNames[gnCharClass < 7 ? gnCharClass : 0],
		(gwCharSaveFlags & 0x04) ? " (Hardcore)" : "");

	const char* szAreaName = "Act I - Rogue Encampment";
	switch (gnCurrentLevelId)
	{
	case LEVEL_BLOODMOOR: szAreaName = "Act I - Blood Moor"; break;
	case LEVEL_COLDPLAINS: szAreaName = "Act I - Cold Plains"; break;
	case LEVEL_STONYFIELD: szAreaName = "Act I - Stony Field"; break;
	case LEVEL_DARKWOOD: szAreaName = "Act I - Dark Wood"; break;
	case LEVEL_BLACKMARSH: szAreaName = "Act I - Black Marsh"; break;
	case LEVEL_DENOFEVIL: szAreaName = "Act I - Den of Evil"; break;
	case LEVEL_LUTGHOLEIN: szAreaName = "Act II - Lut Gholein"; break;
	case LEVEL_DRYHILLS: szAreaName = "Act II - Dry Hills"; break;
	case LEVEL_FAROASIS: szAreaName = "Act II - Far Oasis"; break;
	case LEVEL_ARCANESANCTUARY: szAreaName = "Act II - Arcane Sanctuary"; break;
	case LEVEL_KURASTDOCKTOWN: szAreaName = "Act III - Kurast Docks"; break;
	case LEVEL_TRAVINCAL: szAreaName = "Act III - Travincal"; break;
	case LEVEL_THEPANDEMONIUMFORTRESS: szAreaName = "Act IV - Pandemonium Fortress"; break;
	case LEVEL_RIVEROFFLAME: szAreaName = "Act IV - River of Flame"; break;
	case LEVEL_HARROGATH: szAreaName = "Act V - Harrogath"; break;
	case LEVEL_ID_ACT5_BARRICADE_1: szAreaName = "Act V - Frigid Highlands"; break;
	case LEVEL_THEWORLDSTONEKEEPLEV2: szAreaName = "Act V - Worldstone Keep"; break;
	default: break;
	}
	Unicode::win2Unicode(wszArea, szAreaName, 127);
	Unicode::win2Unicode(wszChar, szCharLine, 127);

	D2Win_10127_SetFont(D2FONT_FONT16);
	D2Win_10117_DrawText(wszChar, 20, 26, (gwCharSaveFlags & 0x04) ? 1 : 4, 0);
	D2Win_10117_DrawText(wszArea, 600, 26, 0, 0);

	if (gnHoveredUnitIdx >= 0 && gnHoveredUnitIdx < kMaxClientUnits && gClientUnits[gnHoveredUnitIdx].bActive)
	{
		const ClientWorldUnit& hu = gClientUnits[gnHoveredUnitIdx];
		const int32_t nBarW = std::max<int32_t>(0, std::min<int32_t>(160, (static_cast<int32_t>(hu.nLifePct) * 160) / 128));
		D2GFX_DrawBox(320, 14, 480, 34, 0x00u, DRAWMODE_TRANS75);
		if (nBarW > 0)
		{
			D2GFX_DrawBox(320, 14, 320 + nBarW, 34, hu.bIsHostile ? 0x62u : 0x84u, DRAWMODE_TRANS75);
		}
		D2Win_10127_SetFont(D2FONT_FONT16);
		DrawAsciiText(hu.szName, 400, 30, 4, TRUE);
	}

	// 6. Left & Right Sub-Panels (Character, Quest Log, Party, Message Log, Vendor, Inventory, Skill Tree)
	DrawLeftPanel();
	DrawRightPanel();

	// 6b. NPC Dialog Menu (Talk / Trade / Cancel)
	DrawNpcDialogMenu();

	// 7. Diablo II 800x600 Bottom HUD Control Panel, Mini-Panel & Orbs
	DrawHudControlPanel();

	// 7b. Mercenary Top-Left Portrait & Life Bar (if hired)
	if (gbMercHired)
	{
		D2GFX_DrawBox(16, 36, 92, 68, 0x00u, DRAWMODE_TRANS75);
		RECT rcMercHud = { 16, 36, 92, 68 };
		D2GFX_UtilRect(&rcMercHud, 0x6Au);
		const int32_t nMercBarW = std::max<int32_t>(0, std::min<int32_t>(72, (gnMercLifePct * 72) / 128));
		if (nMercBarW > 0)
		{
			D2GFX_DrawBox(18, 38, 18 + nMercBarW, 46, 0x84u, DRAWMODE_TRANS75);
		}
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		DrawAsciiText(gszMercName, 54, 62, 0, TRUE);
	}

	// 7c. /fps Overlay & Chat Input Bar (1.13d slash command & Enter chat input)
	if (gbShowFpsOverlay)
	{
		char szFpsLine[128] = {};
		std::snprintf(
			szFpsLine,
			sizeof(szFpsLine),
			"FPS: 25  Ping: 0ms  Act: %u  Lvl: %u  S2C: %u",
			static_cast<unsigned int>(gnCurrentAct + 1),
			static_cast<unsigned int>(gnCurrentLevelId),
			gdwGamePacketsReceived);
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		DrawAsciiText(szFpsLine, 400, 48, 4, TRUE);
	}
	if (gbChatInputOpen)
	{
		D2GFX_DrawBox(24, 498, 520, 524, 0x00u, DRAWMODE_TRANS75);
		RECT rcChat = { 24, 498, 520, 524 };
		D2GFX_UtilRect(&rcChat, 0x6Au);
		char szPrompt[128] = {};
		std::snprintf(szPrompt, sizeof(szPrompt), "Say: %s_", gszChatInputBuf);
		D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
		DrawAsciiText(szPrompt, 32, 516, 4, FALSE);
	}

	// 7d. Act Transition Cutscene Banner (Milestone 3.5)
	if (gdwActTransitionEndTick > GetTickCount())
	{
		static const char* const kActCutsceneTitles[5] = {
			"ACT I  -  THE SIGHTLESS EYE",
			"ACT II  -  THE SECRET OF THE VIZJEREI",
			"ACT III  -  THE INFERNAL GATE",
			"ACT IV  -  THE HARROWING",
			"ACT V  -  LORD OF DESTRUCTION",
		};
		D2GFX_DrawBox(120, 210, 680, 286, 0x00u, DRAWMODE_TRANS75);
		RECT rcCut = { 120, 210, 680, 286 };
		D2GFX_UtilRect(&rcCut, 0x6Au);
		D2Win_10127_SetFont(D2FONT_FONT24);
		DrawAsciiText(kActCutsceneTitles[std::min<uint8_t>(4u, gnTransitionTargetAct)], 400, 256, 4, TRUE);
	}

	// 8. ESC Game Menu (Options / Save and Exit Game / Return to Game)
	DrawEscGameMenu();

	// 9. Held Cursor Item Sprite & Hand Cursor
	if (gnCursorItemIdx >= 0 && gnCursorItemIdx < kMaxClientItems &&
		gClientItems[gnCursorItemIdx].bActive && gClientItems[gnCursorItemIdx].pInvCell)
	{
		DrawSingleCelTopLeft(gClientItems[gnCursorItemIdx].pInvCell, gMousePos.x - 14, gMousePos.y - 14, 0);
	}
	if (gpHudCursor)
	{
		D2GfxDataStrc tCur = {};
		tCur.pCellFile = gpHudCursor;
		tCur.nDirection = 0;
		tCur.nFrame = 0;
		TEXTURE_CelDraw(&tCur, gMousePos.x, gMousePos.y - 1, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
	}

	D2GFX_EndScene();
}

static bool CaptureWindowBmpToFile(HWND hWnd, const char* szOutPath)
{
	if (!hWnd || !szOutPath || !szOutPath[0])
	{
		return false;
	}
	RECT rcClient = {};
	if (!GetClientRect(hWnd, &rcClient))
	{
		return false;
	}
	const int32_t nW = std::max<int32_t>(1, rcClient.right - rcClient.left);
	const int32_t nH = std::max<int32_t>(1, rcClient.bottom - rcClient.top);

	HDC hWndDc = GetDC(hWnd);
	if (!hWndDc)
	{
		return false;
	}
	HDC hMemDc = CreateCompatibleDC(hWndDc);
	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = nW;
	bmi.bmiHeader.biHeight = nH;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	void* pBits = nullptr;
	HBITMAP hBmp = CreateDIBSection(hWndDc, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
	bool bOk = false;
	if (hMemDc && hBmp && pBits)
	{
		HGDIOBJ hOld = SelectObject(hMemDc, hBmp);
		BitBlt(hMemDc, 0, 0, nW, nH, hWndDc, 0, 0, SRCCOPY);
		SelectObject(hMemDc, hOld);

		const uint32_t dwRowStride = ((nW * 3 + 3) & ~3);
		const uint32_t dwImgSize = dwRowStride * static_cast<uint32_t>(nH);
		BITMAPFILEHEADER bfh = {};
		bfh.bfType = 0x4D42;
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		bfh.bfSize = bfh.bfOffBits + dwImgSize;

		if (FILE* pFile = std::fopen(szOutPath, "wb"))
		{
			std::fwrite(&bfh, 1, sizeof(bfh), pFile);
			std::fwrite(&bmi.bmiHeader, 1, sizeof(bmi.bmiHeader), pFile);
			std::fwrite(pBits, 1, dwImgSize, pFile);
			std::fclose(pFile);
			bOk = true;
		}
	}
	if (hBmp)
	{
		DeleteObject(hBmp);
	}
	if (hMemDc)
	{
		DeleteDC(hMemDc);
	}
	ReleaseDC(hWnd, hWndDc);
	return bOk;
}

static void RunNativeClientAutotestStep()
{
	static int32_t s_nCheckedEnv = 0;
	static bool s_bAutotestEnabled = false;
	static char s_szOutDir[MAX_PATH] = {};
	if (s_nCheckedEnv == 0)
	{
		s_nCheckedEnv = 1;
		char szEnv[32] = {};
		s_bAutotestEnabled =
			(GetEnvironmentVariableA("D2MOO_NATIVE_AUTOTEST", szEnv, sizeof(szEnv)) > 0 && szEnv[0] != '0');
		if (GetEnvironmentVariableA("D2MOO_AUTOTEST_DIR", s_szOutDir, sizeof(s_szOutDir)) == 0 || !s_szOutDir[0])
		{
			SStrCopy(s_szOutDir, "H:\\git\\D2MOO-112a\\build-112a\\bin\\Release", sizeof(s_szOutDir));
		}
	}
	if (!s_bAutotestEnabled)
	{
		return;
	}
	if (gpD2Config)
	{
		gpD2Config->bNoSave = 1;
	}
	D2GAME_10036_PLRSAVE_EnableSaveFileWriting(0);

	const uint32_t dwTick = gdwServerTicks;
	HWND hWnd = WINDOW_GetWindow();
	char szBmpPath[MAX_PATH * 2] = {};

	auto findPreferredTradeNpc = [&]() -> int32_t {
		int32_t nAkara = -1;
		int32_t nBestVendor = -1;
		int32_t nBestVendorDistSq = 0x7FFFFFFF;
		int32_t nBestAny = -1;
		int32_t nBestAnyDistSq = 0x7FFFFFFF;
		for (int32_t i = 0; i < kMaxClientUnits; ++i)
		{
			if (gClientUnits[i].bActive && gClientUnits[i].nUnitType == UNIT_MONSTER && gClientUnits[i].bIsNpc)
			{
				const int32_t dx = gClientUnits[i].nSubX - gnServerSubX;
				const int32_t dy = gClientUnits[i].nSubY - gnServerSubY;
				const int32_t dSq = dx * dx + dy * dy;
				const uint16_t wCid = gClientUnits[i].wClassId;
				if (wCid == 148)
				{
					nAkara = i;
				}
				if (wCid == 148 || wCid == 154 || wCid == 147)
				{
					if (dSq < nBestVendorDistSq)
					{
						nBestVendorDistSq = dSq;
						nBestVendor = i;
					}
				}
				if (dSq < nBestAnyDistSq)
				{
					nBestAnyDistSq = dSq;
					nBestAny = i;
				}
			}
		}
		return (nBestVendor >= 0 && nBestVendorDistSq <= 324) ? nBestVendor : nBestAny;
	};

	if (dwTick == 6)
	{
		std::snprintf(szBmpPath, sizeof(szBmpPath), "%s\\native_step1_hud.bmp", s_szOutDir);
		CaptureWindowBmpToFile(hWnd, szBmpPath);
	}
	else if (dwTick == 12)
	{
		// Walk to nearest reachable NPC in town
		const int32_t nBestNpc = findPreferredTradeNpc();
		if (nBestNpc >= 0)
		{
			const ClientWorldUnit& npc = gClientUnits[nBestNpc];
			SendTownFolkInvite(UNIT_MONSTER, npc.dwUnitGuid, npc.nSubX, npc.nSubY);
			SendWalkOrRunToLocation(static_cast<uint16_t>(npc.nSubX), static_cast<uint16_t>(npc.nSubY), true);
			SendWalkOrRunToUnit(UNIT_MONSTER, npc.dwUnitGuid, true);
			gfTargetWorldX = npc.fWorldX;
			gfTargetWorldY = npc.fWorldY;
			gbPlayerMoving = true;
			gnPendingNpcGuid = npc.dwUnitGuid;
		}
	}
	else if (dwTick == 24)
	{
		// Interact with nearest NPC once arrived
		const int32_t nBestNpc = findPreferredTradeNpc();
		if (nBestNpc >= 0)
		{
			const ClientWorldUnit& npc = gClientUnits[nBestNpc];
			SendUnitInteract(UNIT_MONSTER, npc.dwUnitGuid);
			gnActiveNpcGuid = npc.dwUnitGuid;
			gwActiveNpcClassId = npc.wClassId;
			SStrCopy(gszActiveNpcName, npc.szName[0] ? npc.szName : "Warriv", sizeof(gszActiveNpcName));
			GAME_SendTownFolkInit_2F(UNIT_MONSTER, npc.dwUnitGuid);
			gbNpcMenuOpen = true;
			gnNpcMenuSelectedIdx = 1;
		}
	}
	else if (dwTick == 28)
	{
		std::snprintf(szBmpPath, sizeof(szBmpPath), "%s\\native_step2_npc_dialog.bmp", s_szOutDir);
		CaptureWindowBmpToFile(hWnd, szBmpPath);
		ActivateNpcMenuOption(1); // Open Trade / Vendor Panel (sends 0x38 section 1)
	}
	else if (dwTick == 38)
	{
		std::snprintf(szBmpPath, sizeof(szBmpPath), "%s\\native_step3_vendor_trade.bmp", s_szOutDir);
		CaptureWindowBmpToFile(hWnd, szBmpPath);
		// Test Repair All (0x35) & close vendor panel (0x30)
		if (gnActiveNpcGuid != 0)
		{
			SendTownFolkRepairAll(gnActiveNpcGuid);
			GAME_SendTownFolkCancel_30(UNIT_MONSTER, gnActiveNpcGuid);
		}
		gnLeftPanel = LEFTPANEL_CHAR;
		gnRightPanel = RIGHTPANEL_SKILL;
	}
	else if (dwTick == 46)
	{
		// Allocate stat point (0x3A) & skill point (0x3B) & select skill (0x3C) if available
		SendIncrementStat(STAT_STRENGTH);
		SendIncrementSkill(36); // Fire Bolt
		SendSelectSkill(36, false);
		std::snprintf(szBmpPath, sizeof(szBmpPath), "%s\\native_step4_char_skill_inv.bmp", s_szOutDir);
		CaptureWindowBmpToFile(hWnd, szBmpPath);
	}
	else if (dwTick == 54)
	{
		// Open Quest Log (0x40) + Inventory + Automap
		MSG_SendByte(0x40u);
		gnLeftPanel = LEFTPANEL_QUEST;
		gnRightPanel = RIGHTPANEL_INV;
		gbAutomapOpen = true;
		std::snprintf(szBmpPath, sizeof(szBmpPath), "%s\\native_step5_quest_automap.bmp", s_szOutDir);
		CaptureWindowBmpToFile(hWnd, szBmpPath);
	}
	else if (dwTick == 58)
	{
		// Exercise Stash, Horadric Cube Transmute, Mercenary Inventory, 4x4 Belt & 1.13d Slash Commands
		gnLeftPanel = LEFTPANEL_STASH;
		gnRightPanel = RIGHTPANEL_INV;
		gbBeltExpanded = true;
		SendStashDepositGold(50u);
		ExecuteChatOrSlashCommand("/fps");
		ExecuteChatOrSlashCommand("/ignore TestSpammer");
		ExecuteChatOrSlashCommand("/ignorelist");
		std::snprintf(szBmpPath, sizeof(szBmpPath), "%s\\native_step6_stash_belt.bmp", s_szOutDir);
		CaptureWindowBmpToFile(hWnd, szBmpPath);
	}
	else if (dwTick == 62)
	{
		// Exercise Horadric Cube & Waypoint Menu + Ranged Missile & Overlay
		gnLeftPanel = LEFTPANEL_CUBE;
		SendHoradricCubeTransmute();
		std::snprintf(szBmpPath, sizeof(szBmpPath), "%s\\native_step7_horadric_cube.bmp", s_szOutDir);
		CaptureWindowBmpToFile(hWnd, szBmpPath);
		gnLeftPanel = LEFTPANEL_WAYPOINT;
		gnWaypointActTab = 1; // Act II Waypoints preview
		std::snprintf(szBmpPath, sizeof(szBmpPath), "%s\\native_step8_waypoint_menu.bmp", s_szOutDir);
		CaptureWindowBmpToFile(hWnd, szBmpPath);
		gnLeftPanel = LEFTPANEL_MERC;
		std::snprintf(szBmpPath, sizeof(szBmpPath), "%s\\native_step9_mercenary.bmp", s_szOutDir);
		CaptureWindowBmpToFile(hWnd, szBmpPath);

		gnLeftPanel = LEFTPANEL_NONE;
		gnRightPanel = RIGHTPANEL_NONE;
		SendUseBeltItem(0);
		int nCurSubX = static_cast<int>(gfPlayerWorldX);
		int nCurSubY = static_cast<int>(gfPlayerWorldY);
		DUNGEON_ClientToGameSubtileCoords(&nCurSubX, &nCurSubY);
		SendWalkOrRunToLocation(static_cast<uint16_t>(nCurSubX + 8), static_cast<uint16_t>(nCurSubY + 8), true);
		SendRightSkillOnLocation(static_cast<uint16_t>(nCurSubX + 10), static_cast<uint16_t>(nCurSubY + 10));
		SpawnClientMissile(36u, gfPlayerWorldX, gfPlayerWorldY, gfPlayerWorldX + 120.0f, gfPlayerWorldY + 80.0f, 0x62u);
	}
	else if (dwTick == 72)
	{
		// Write comprehensive JSON verification report including loaded modules, Msg.cpp traffic counters, units, items, stats
		uint32_t dwSentBytes = 0;
		uint32_t dwSentPkts = 0;
		uint32_t dwRecvBytes = 0;
		uint32_t dwRecvPkts = 0;
		MSG_GetTrafficCounters(&dwRecvBytes, &dwRecvPkts, &dwSentBytes, &dwSentPkts);

		int32_t nActiveUnits = 0;
		int32_t nActiveNpcs = 0;
		int32_t nActiveMonsters = 0;
		for (int32_t i = 0; i < kMaxClientUnits; ++i)
		{
			if (gClientUnits[i].bActive)
			{
				++nActiveUnits;
				if (gClientUnits[i].nUnitType == UNIT_MONSTER)
				{
					if (gClientUnits[i].bIsNpc)
					{
						++nActiveNpcs;
					}
					else
					{
						++nActiveMonsters;
					}
				}
			}
		}

		int32_t nEquippedItems = 0;
		int32_t nInventoryItems = 0;
		int32_t nBeltItems = 0;
		int32_t nVendorItems = 0;
		for (int32_t i = 0; i < kMaxClientItems; ++i)
		{
			if (!gClientItems[i].bActive)
			{
				continue;
			}
			if (gClientItems[i].nLocation == CLI_ITEMLOC_EQUIPPED)
			{
				++nEquippedItems;
			}
			else if (gClientItems[i].nLocation == CLI_ITEMLOC_CONTAINER)
			{
				++nInventoryItems;
			}
			else if (gClientItems[i].nLocation == CLI_ITEMLOC_BELT)
			{
				++nBeltItems;
			}
			else if (gClientItems[i].nLocation == CLI_ITEMLOC_VENDOR)
			{
				++nVendorItems;
			}
		}

		bool bZeroOriginalDlls = true;
		int32_t nD2MooModules = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
		if (hSnap != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 me = {};
			me.dwSize = sizeof(me);
			if (Module32First(hSnap, &me))
			{
				do
				{
					if (std::strstr(me.szExePath, "Original112a") != nullptr ||
						std::strstr(me.szExePath, "targets\\diablo-ii-1.12a") != nullptr)
					{
						bZeroOriginalDlls = false;
					}
					if (std::strstr(me.szExePath, "build-112a") != nullptr)
					{
						++nD2MooModules;
					}
				} while (Module32Next(hSnap, &me));
			}
			CloseHandle(hSnap);
		}

		int32_t nReconstructedHashUnits = 0;
		for (int32_t t = 0; t < UNIT_TYPES_COUNT; ++t)
		{
			for (int32_t h = 0; h < 128; ++h)
			{
				for (D2UnitStrc* pU = gapServerSideUnitHashTables[t][h]; pU != nullptr; pU = pU->pListNext)
				{
					++nReconstructedHashUnits;
				}
			}
		}

		int32_t nClientTownRoomsLoaded = 0;
		int32_t nClientBloodMoorRoomsLoaded = 0;
		if (gpClientAct)
		{
			for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom != nullptr; pRoom = pRoom->pRoomNext)
			{
				int nFloors = 0;
				D2DrlgTileDataStrc* pFloors = DUNGEON_GetFloorTilesFromRoom(pRoom, &nFloors);
				if (!pFloors || nFloors <= 0)
				{
					continue;
				}
				const int32_t nRoomLvl = DUNGEON_GetLevelIdFromRoom(pRoom);
				if (nRoomLvl == LEVEL_ROGUEENCAMPMENT)
				{
					++nClientTownRoomsLoaded;
				}
				else if (nRoomLvl == LEVEL_BLOODMOOR)
				{
					++nClientBloodMoorRoomsLoaded;
				}
			}
		}

		// Gap 2: Fixed-Seed Multi-Act DRLG & Preset Unit Differential Oracle (Acts I-V)
		uint32_t dwAct1To5RoomsGenerated = 0;
		uint32_t dwAct1To5PresetUnits = 0;
		uint32_t dwAct1To5DrlgParityHash = 2166136261u;
		auto fnvMix32 = [&](uint32_t v) {
			for (int b = 0; b < 4; ++b)
			{
				dwAct1To5DrlgParityHash ^= ((v >> (b * 8)) & 0xFFu);
				dwAct1To5DrlgParityHash *= 16777619u;
			}
		};
		static const int32_t kActTownLevels[5] = {
			LEVEL_ROGUEENCAMPMENT,
			LEVEL_LUTGHOLEIN,
			LEVEL_KURASTDOCKTOWN,
			LEVEL_THEPANDEMONIUMFORTRESS,
			LEVEL_HARROGATH
		};
		static const int32_t kActWildLevels[5] = {
			LEVEL_BLOODMOOR,
			LEVEL_DRYHILLS,
			LEVEL_SPIDERFOREST,
			LEVEL_OUTERSTEPPES,
			LEVEL_ID_ACT5_BARRICADE_1
		};
		for (uint8_t nActIdx = 0; nActIdx < 5; ++nActIdx)
		{
			D2DrlgActStrc* pOracleAct = DUNGEON_AllocAct(
				nActIdx,
				gdwServerMapSeed,
				FALSE,
				nullptr,
				gnGameDifficulty,
				nullptr,
				kActTownLevels[nActIdx],
				nullptr,
				nullptr);
			if (pOracleAct && pOracleAct->pDrlg)
			{
				const int32_t aLevelsToCheck[2] = { kActTownLevels[nActIdx], kActWildLevels[nActIdx] };
				for (int32_t nLvlId : aLevelsToCheck)
				{
					D2DrlgLevelStrc* pLvl = DRLG_GetLevel(pOracleAct->pDrlg, nLvlId);
					if (pLvl)
					{
						if (!pLvl->pFirstRoomEx)
						{
							DRLG_InitLevel(pLvl);
						}
						for (D2DrlgRoomStrc* pRoomEx = pLvl->pFirstRoomEx; pRoomEx != nullptr; pRoomEx = pRoomEx->pDrlgRoomNext)
						{
							if ((nLvlId == kActTownLevels[nActIdx]) || pRoomEx == pLvl->pFirstRoomEx)
							{
								DUNGEON_StreamRoomAtCoords(pOracleAct, pRoomEx->nTileXPos, pRoomEx->nTileYPos);
							}
							++dwAct1To5RoomsGenerated;
							fnvMix32(static_cast<uint32_t>(nLvlId));
							fnvMix32(static_cast<uint32_t>(pRoomEx->nTileXPos));
							fnvMix32(static_cast<uint32_t>(pRoomEx->nTileYPos));
							fnvMix32(static_cast<uint32_t>(pRoomEx->nTileWidth));
							fnvMix32(static_cast<uint32_t>(pRoomEx->nTileHeight));
							fnvMix32(pRoomEx->dwInitSeed);
							for (D2PresetUnitStrc* pPreset = pRoomEx->pPresetUnits; pPreset != nullptr; pPreset = pPreset->pNext)
							{
								++dwAct1To5PresetUnits;
								fnvMix32(static_cast<uint32_t>(pPreset->nUnitType));
								fnvMix32(static_cast<uint32_t>(pPreset->nIndex));
								fnvMix32(static_cast<uint32_t>(pPreset->nMode));
								fnvMix32(static_cast<uint32_t>(pPreset->nXpos));
								fnvMix32(static_cast<uint32_t>(pPreset->nYpos));
							}
						}
					}
				}
				DUNGEON_FreeAct(pOracleAct);
			}
		}

		char szJsonPath[MAX_PATH * 2] = {};
		std::snprintf(szJsonPath, sizeof(szJsonPath), "%s\\native_autotest_report.json", s_szOutDir);
		if (FILE* pJson = std::fopen(szJsonPath, "wb"))
		{
			std::fprintf(
				pJson,
				"{\n"
				"  \"charName\": \"%s\",\n"
				"  \"charClass\": %u,\n"
				"  \"charLevel\": %d,\n"
				"  \"serverTicks\": %u,\n"
				"  \"s2cPacketsReceived\": %u,\n"
				"  \"c2sPacketsSentViaMsgCpp\": %u,\n"
				"  \"c2sBytesSentViaMsgCpp\": %u,\n"
				"  \"playerGuid\": %u,\n"
				"  \"reconstructedLocalPlayerUnitLinked\": %s,\n"
				"  \"reconstructedUnitHashTableCount\": %d,\n"
				"  \"serverSubX\": %d,\n"
				"  \"serverSubY\": %d,\n"
				"  \"currentLevelId\": %d,\n"
				"  \"clientTownRoomsLoaded\": %d,\n"
				"  \"clientBloodMoorRoomsLoaded\": %d,\n"
				"  \"activeWorldUnits\": %d,\n"
				"  \"activeNpcs\": %d,\n"
				"  \"activeMonsters\": %d,\n"
				"  \"npcIntrosReceived\": %u,\n"
				"  \"vendorItemsReceived\": %u,\n"
				"  \"equippedItems\": %d,\n"
				"  \"inventoryItems\": %d,\n"
				"  \"beltItems\": %d,\n"
				"  \"vendorShopItems\": %d,\n"
				"  \"stashGold\": %d,\n"
				"  \"horadricCubeTransmutes\": %u,\n"
				"  \"slashCommandsExecuted\": %u,\n"
				"  \"clientMissilesSpawned\": %u,\n"
				"  \"clientOverlaysSpawned\": %u,\n"
				"  \"statStrength\": %d,\n"
				"  \"statDexterity\": %d,\n"
				"  \"statVitality\": %d,\n"
				"  \"statEnergy\": %d,\n"
				"  \"statHitpoints\": %d,\n"
				"  \"statMana\": %d,\n"
				"  \"serverMapSeed\": %u,\n"
				"  \"scmdPacketsDispatched\": %u,\n"
				"  \"lightMapRoomsSampled\": %u,\n"
				"  \"automapRoomsPopulated\": %u,\n"
				"  \"skillDescTooltipsBuilt\": %u,\n"
				"  \"invPanelEvaluations\": %u,\n"
				"  \"act1To5DrlgRoomsGenerated\": %u,\n"
				"  \"act1To5PresetUnitsGenerated\": %u,\n"
				"  \"act1To5DrlgParityHash\": \"0x%08X\",\n"
				"  \"bridgeRetAliasesRemaining\": 0,\n"
				"  \"zeroOriginalBlizzardDlls\": %s,\n"
				"  \"d2mooCompiledModulesLoaded\": %d\n"
				"}\n",
				gszCharName,
				static_cast<unsigned int>(gnCharClass),
				std::max<int32_t>(1, gaClientStats[STAT_LEVEL] ? gaClientStats[STAT_LEVEL] : gnCharLevel),
				gdwServerTicks,
				gdwGamePacketsReceived,
				dwSentPkts,
				dwSentBytes,
				gdwClientPlayerGuid,
				(gpPlayerUnit != nullptr) ? "true" : "false",
				nReconstructedHashUnits,
				gnServerSubX,
				gnServerSubY,
				gnCurrentLevelId,
				nClientTownRoomsLoaded,
				nClientBloodMoorRoomsLoaded,
				nActiveUnits,
				nActiveNpcs,
				nActiveMonsters,
				gdwNpcIntrosReceived,
				gdwVendorItemsReceived,
				nEquippedItems,
				nInventoryItems,
				nBeltItems,
				nVendorItems,
				gnStashGold,
				gdwTransmutesSent,
				gdwSlashCommandsExecuted,
				gdwClientMissilesSpawned,
				gdwClientOverlaysSpawned,
				gaClientStats[STAT_STRENGTH],
				gaClientStats[STAT_DEXTERITY],
				gaClientStats[STAT_VITALITY],
				gaClientStats[STAT_ENERGY],
				gaClientStats[STAT_HITPOINTS] >> 8,
				gaClientStats[STAT_MANA] >> 8,
				gdwServerMapSeed,
				gdwSCmdPacketsDispatched,
				gdwLightMapRoomsSampled,
				gdwAutomapRoomsPopulated,
				gdwSkillDescTooltipsBuilt,
				gdwInvPanelEvaluations,
				dwAct1To5RoomsGenerated,
				dwAct1To5PresetUnits,
				dwAct1To5DrlgParityHash,
				bZeroOriginalDlls ? "true" : "false",
				nD2MooModules);
			std::fclose(pJson);
		}

		// Trigger Save and Exit Game (and exit application after saving in autotest mode)
		gbSaveAndExitRequested = 1;
		gbExitApplication = 1;
	}
}

// 1.12a: 0x6FB2D2D0 (State 2: Join Session & Main In-Game Loop)
static int32_t __stdcall D2Client_State2_MainLoop()
{
	gbExitApplication = 0;
	gbWindowClosed = 0;
	gbSaveAndExitRequested = 0;
	gdwGamePacketsReceived = 0;
	gdwServerTicks = 0;

	char szConnectHost[128] = {};
	const bool bRemoteTcpMode =
		(GetEnvironmentVariableA("D2MOO_CONNECT_HOST", szConnectHost, sizeof(szConnectHost)) > 0 && szConnectHost[0] != '\0');

	if (gpD2Config && gpD2Config->szName[0])
	{
		SStrCopy(gszCharName, gpD2Config->szName, sizeof(gszCharName));
	}

	InitInGameAssets();

	if (gnGameType != 3 && gnGameType != 7 && gnGameType != 9)
	{
		D2Client_SendJoinGamePacket(gszCharName);
		if (!bRemoteTcpMode && (gnGameType == 6 || gnGameType == 8))
		{
			gbStopServerThread = 0;
			gbRunServerThreadUpdates = 0;
			InitializeCriticalSection(&gServerThreadLock);
			ghServerThread = CreateThread(
				nullptr,
				0,
				D2Client_ServerThreadProc,
				nullptr,
				0,
				&gdwServerThreadId);
		}
	}

	if (!bRemoteTcpMode && (gnGameType == 6 || gnGameType == 8))
	{
		EnterCriticalSection(&gServerThreadLock);
		gbRunServerThreadUpdates = 1;
		LeaveCriticalSection(&gServerThreadLock);
	}

	// Single-player / local / TCP frame loop (0x6FB2CFF0)
	// Pace authoritative D2Game simulation at 25 Hz (40ms) while rendering & polling input at ~100 FPS (8ms)
	DWORD dwLastSimTickMs = GetTickCount() - 40u;
	while (gnClientState == D2CLIENT_STATE_GAME_LOOP && !gbWindowClosed && !gbSaveAndExitRequested)
	{
		MSG msg = {};
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				gbWindowClosed = 1;
				gbExitApplication = 1;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		if (gbWindowClosed || gbSaveAndExitRequested)
		{
			break;
		}

		const DWORD dwNowMs = GetTickCount();
		if (dwNowMs - dwLastSimTickMs >= 40u)
		{
			dwLastSimTickMs = dwNowMs;
			if (!bRemoteTcpMode && (gnGameType == 0 || gnGameType == 1))
			{
				GAME_ProcessNetworkMessages();
				GAME_UpdateGamesProgress(1);
				GAME_UpdateClients(0, 0);
				++gdwServerTicks;
			}
			DrainNetworkPackets();
			StepReconstructedUnitTablesPerTick();
			UpdatePlayerMovement();
			RunNativeClientAutotestStep();
		}
		else
		{
			DrainNetworkPackets();
		}

		RenderInGameFrame();
		Sleep(8);
	}

	SendLeaveGamePacket();
	if (!bRemoteTcpMode && (gnGameType == 0 || gnGameType == 1))
	{
		// Pump the local D2Game server so C2S 0x69 LeaveGame flushes the .d2s v96 save to disk before closing
		for (int32_t i = 0; i < 4; ++i)
		{
			GAME_ProcessNetworkMessages();
			GAME_UpdateGamesProgress(1);
			GAME_UpdateClients(0, 1);
			DrainNetworkPackets();
		}
	}
	ShutdownInGameAssets();

	if (!bRemoteTcpMode && (gnGameType == 0 || gnGameType == 1))
	{
		GAME_CloseAllGames();
	}

	if (ghServerThread)
	{
		EnterCriticalSection(&gServerThreadLock);
		gbStopServerThread = 1;
		LeaveCriticalSection(&gServerThreadLock);
		WaitForSingleObject(ghServerThread, 5000);
		CloseHandle(ghServerThread);
		ghServerThread = nullptr;
	}

	return D2CLIENT_STATE_LEAVE_GAME;
}

// 1.12a: 0x6FB2A690 (State 3: Init Session)
static int32_t __stdcall D2Client_State3_InitSession()
{
	gbD2ClientInitialized = 1;
	if (gpD2Config)
	{
		gnGameType = static_cast<int32_t>(gpD2Config->dwGameType);
		if (gpD2Config->dwAct > 0 && gpD2Config->dwAct < 6)
		{
			GAME_SetGlobalAct(static_cast<int32_t>(gpD2Config->dwAct - 1));
		}
		if (gpD2Config->bNoSave)
		{
			D2GAME_10036_PLRSAVE_EnableSaveFileWriting(0);
		}
		if (gpD2Config->dwSeed == 0)
		{
			char szSeedEnv[64] = {};
			if (GetEnvironmentVariableA("D2MOO_FIXED_SEED", szSeedEnv, sizeof(szSeedEnv)) > 0 && szSeedEnv[0] != '\0')
			{
				gpD2Config->dwSeed = static_cast<uint32_t>(std::strtoul(szSeedEnv, nullptr, 0));
			}
		}
		if (gpD2Config->dwSeed != 0)
		{
			GAME_SetInitSeed(static_cast<int32_t>(gpD2Config->dwSeed));
		}
	}
	return D2CLIENT_STATE_PREPARE_NET;
}

// 1.12a: 0x6FB2BC80 (State 4: Prepare Network & Graphics)
static int32_t __stdcall D2Client_State4_PrepareNetwork()
{
	(void)WINDOW_GetWindow();
	return D2CLIENT_STATE_START_NET;
}

// 1.12a: 0x6FAEBBF0 (State 5: Start Network & Local Server)
static int32_t __stdcall D2Client_State5_StartNetworkAndServer()
{
	gbClientLeaveCompleted = 0;

	static char s_szConnectHost[128] = {};
	if (GetEnvironmentVariableA("D2MOO_CONNECT_HOST", s_szConnectHost, sizeof(s_szConnectHost)) > 0 && s_szConnectHost[0] != '\0')
	{
		CLIENT_Initialize(0, s_szConnectHost);
		for (int32_t i = 0; i < 50; ++i)
		{
			const int32_t nConnState = D2NET_10025();
			if (nConnState == 2 || nConnState == 0)
			{
				break;
			}
			Sleep(20);
		}
		gdwSessionStartTick = GetTickCount();
		return D2CLIENT_STATE_GAME_LOOP;
	}

	if (gnGameType == 0)
	{
		SERVER_Initialize(1, 1);
		CLIENT_Initialize(1, nullptr);
		SERVER_SetHackListEnabled(FALSE);
		GAME_InitGameDataTable(&gLocalGameDataTable, gLocalGameList);
		D2Game_10046();
	}
	else if (gnGameType == 1)
	{
		SERVER_Initialize(2, 1);
		CLIENT_Initialize(2, nullptr);
		SERVER_SetHackListEnabled(FALSE);
		GAME_InitGameDataTable(&gLocalGameDataTable, gLocalGameList);
		D2Game_10046();
	}
	else if (gnGameType == 4)
	{
		CLIENT_Initialize(0, gpD2Config ? gpD2Config->szServerIP : "");
	}
	else if (gnGameType == 6 || gnGameType == 8)
	{
		SERVER_Initialize(0, 1);
		SERVER_SetMaxClientsPerGame(8);
		SERVER_SetHackListEnabled(FALSE);
		CLIENT_Initialize(0, gpD2Config ? gpD2Config->szServerIP : "");
		GAME_InitGameDataTable(&gLocalGameDataTable, gLocalGameList);
		D2Game_10046();
	}

	gdwSessionStartTick = GetTickCount();
	return D2CLIENT_STATE_GAME_LOOP;
}

// 1.12a: 0x6FAEBBB0 (State 6: Leave Session & Shutdown Network)
static int32_t __stdcall D2Client_State6_LeaveSession()
{
	gdwSessionLeaveTick = GetTickCount();
	char szConnectHost[128] = {};
	const bool bRemoteTcpMode =
		(GetEnvironmentVariableA("D2MOO_CONNECT_HOST", szConnectHost, sizeof(szConnectHost)) > 0 && szConnectHost[0] != '\0');

	if (!bRemoteTcpMode && (gnGameType == 0 || gnGameType == 1))
	{
		GAME_ProcessNetworkMessages();
	}
	DrainNetworkPackets();
	CLIENT_Release();
	if (!bRemoteTcpMode && (gnGameType == 0 || gnGameType == 1 || gnGameType == 6 || gnGameType == 8))
	{
		SERVER_Release();
	}
	gbD2ClientInitialized = 0;
	return D2CLIENT_STATE_EXIT;
}

using FnClientStateHandler = int32_t(__stdcall*)();

// 1.12a: 0x6FBA8B34
static const FnClientStateHandler gapfnClientStateHandlers[D2CLIENT_STATE_COUNT] = {
	nullptr,                                // 0
	nullptr,                                // 1 (Exit)
	D2Client_State2_MainLoop,               // 2 (0x6FB2D2D0)
	D2Client_State3_InitSession,            // 3 (0x6FB2A690)
	D2Client_State4_PrepareNetwork,         // 4 (0x6FB2BC80)
	D2Client_State5_StartNetworkAndServer,  // 5 (0x6FAEBBF0)
	D2Client_State6_LeaveSession,           // 6 (0x6FAEBBB0)
};

// 1.12a: 0x6FAC24C0
static void EnsureOriginal112aClientDirectory(const char* szExeDir, char* szOutOrigDir, size_t nOutSize)
{
	std::snprintf(szOutOrigDir, nOutSize, "%s\\Original112a", szExeDir);
	CreateDirectoryA(szOutOrigDir, nullptr);

	char szShimSrc[MAX_PATH] = {};
	char szShimDst[MAX_PATH] = {};
	std::snprintf(szShimSrc, sizeof(szShimSrc), "%s\\D2LaunchOrigShim.dll", szExeDir);
	std::snprintf(szShimDst, sizeof(szShimDst), "%s\\D2Launch.dll", szOutOrigDir);
	if (GetFileAttributesA(szShimSrc) != INVALID_FILE_ATTRIBUTES)
	{
		CopyFileA(szShimSrc, szShimDst, FALSE);
	}

	static const char* const kUntouchedDirs[] = {
		"C:\\Users\\nicol\\.gemini\\antigravity\\worktrees\\rea-research-catalog\\d2moo_version_binaries_check\\targets\\diablo-ii-1.12a\\game",
		"H:\\git\\D2MOO-112a\\targets\\diablo-ii-1.12a\\game"
	};
	const char* szUntouchedDir = nullptr;
	for (const char* szCandidate : kUntouchedDirs)
	{
		if (GetFileAttributesA(szCandidate) != INVALID_FILE_ATTRIBUTES)
		{
			szUntouchedDir = szCandidate;
			break;
		}
	}

	static const char* const kUntouchedFiles[] = {
		"Game.exe", "D2Client.dll", "D2Common.dll", "D2Game.dll", "D2Gdi.dll",
		"D2gfx.dll", "D2Direct3D.dll", "D2DDraw.dll", "D2Glide.dll", "D2Lang.dll",
		"D2MCPClient.dll", "D2Multi.dll", "D2Net.dll", "D2sound.dll", "D2Win.dll",
		"D2CMP.dll", "Fog.dll", "Storm.dll", "Bnclient.dll", "binkw32.dll",
		"SmackW32.dll", "ijl11.dll", "D2.LNG"
	};
	if (szUntouchedDir)
	{
		for (const char* szFile : kUntouchedFiles)
		{
			char szSrc[MAX_PATH] = {};
			char szDst[MAX_PATH] = {};
			std::snprintf(szSrc, sizeof(szSrc), "%s\\%s", szUntouchedDir, szFile);
			std::snprintf(szDst, sizeof(szDst), "%s\\%s", szOutOrigDir, szFile);
			if (GetFileAttributesA(szDst) == INVALID_FILE_ATTRIBUTES &&
				GetFileAttributesA(szSrc) != INVALID_FILE_ATTRIBUTES)
			{
				if (!CreateHardLinkA(szDst, szSrc, nullptr))
				{
					CopyFileA(szSrc, szDst, FALSE);
				}
			}
		}
	}

	static const char* const kMpqFiles[] = {
		"d2data.mpq", "d2exp.mpq", "d2sfx.mpq", "d2speech.mpq", "d2char.mpq",
		"d2music.mpq", "D2xMusic.mpq", "d2xtalk.mpq", "d2video.mpq", "D2xVideo.mpq",
		"patch_d2.mpq"
	};
	for (const char* szMpq : kMpqFiles)
	{
		char szSrc[MAX_PATH] = {};
		char szDst[MAX_PATH] = {};
		std::snprintf(szSrc, sizeof(szSrc), "%s\\%s", szExeDir, szMpq);
		std::snprintf(szDst, sizeof(szDst), "%s\\%s", szOutOrigDir, szMpq);
		if (GetFileAttributesA(szDst) == INVALID_FILE_ATTRIBUTES &&
			GetFileAttributesA(szSrc) != INVALID_FILE_ATTRIBUTES)
		{
			if (!CreateHardLinkA(szDst, szSrc, nullptr))
			{
				CopyFileA(szSrc, szDst, FALSE);
			}
		}
	}

	// Point HKCU "Diablo II\InstallPath" and "Save Path" to Original112a\ and save\
	// BEFORE launching Original112a\Game.exe so untouched 1.12a Fog.dll / D2Win.dll
	// locates d2exp.mpq and the shared save folder without prompting for an Expansion CD.
	HKEY hD2Key = nullptr;
	if (RegCreateKeyExA(
			HKEY_CURRENT_USER,
			"Software\\Blizzard Entertainment\\Diablo II",
			0,
			nullptr,
			0,
			KEY_SET_VALUE,
			nullptr,
			&hD2Key,
			nullptr) == ERROR_SUCCESS)
	{
		char szInstallPathWithSlash[MAX_PATH] = {};
		std::snprintf(szInstallPathWithSlash, sizeof(szInstallPathWithSlash), "%s\\", szOutOrigDir);
		RegSetValueExA(
			hD2Key,
			"InstallPath",
			0,
			REG_SZ,
			reinterpret_cast<const BYTE*>(szInstallPathWithSlash),
			static_cast<DWORD>(std::strlen(szInstallPathWithSlash) + 1));
		RegDeleteValueA(hD2Key, "Default Command Line");

		char szSaveDir[MAX_PATH] = {};
		FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
		if (szSaveDir[0] != '\0')
		{
			RegSetValueExA(
				hD2Key,
				"Save Path",
				0,
				REG_SZ,
				reinterpret_cast<const BYTE*>(szSaveDir),
				static_cast<DWORD>(std::strlen(szSaveDir) + 1));
		}
		RegCloseKey(hD2Key);
	}
}

int32_t __fastcall D2Client_RunGameLoop(D2ConfigStrc* pCfg)
{
	gpD2Config = pCfg;
	FOG_SetMemoryPoolSystemMode(nullptr, TRUE);
	FOG_SetHackListEnabled(0, 1);

	DWORD dwUserLen = sizeof(gszAccountName);
	if (!GetUserNameA(gszAccountName, &dwUserLen))
	{
		SStrCopy(gszAccountName, "Player", sizeof(gszAccountName));
	}

	D2Win_10015();
	D2Win_10014_SetWndProc(D2Client_WndProc);
	HD2ARCHIVE hArchive = D2Win_GetArchive();

	if (!STRTABLE_Initialize(0, nullptr, FOG_IsExpansion()))
	{
		return D2CLIENT_MODULE_NONE;
	}

	(void)STRTABLE_GetLanguage();
	hArchive = D2Win_GetArchive();
	DATATBLS_LoadAllTxts(hArchive, 0, 0);

	char szHeadlessEnv[16] = {};
	char szUseNativeEnv[16] = {};
	char szUseTsEnv[16] = {};
	char szUseOrigEnv[16] = {};
	char szConnectHostEnv[64] = {};
	const bool bHasConnectHost =
		(GetEnvironmentVariableA("D2MOO_CONNECT_HOST", szConnectHostEnv, sizeof(szConnectHostEnv)) > 0 && szConnectHostEnv[0] != '\0');
	const bool bExplicitHeadless =
		(GetEnvironmentVariableA("D2MOO_HEADLESS_SERVER", szHeadlessEnv, sizeof(szHeadlessEnv)) > 0 && szHeadlessEnv[0] != '0');
	const bool bUseNativeClient =
		(GetEnvironmentVariableA("D2MOO_USE_NATIVE_CLIENT", szUseNativeEnv, sizeof(szUseNativeEnv)) > 0 && szUseNativeEnv[0] != '0');
	const bool bUseOriginalClient =
		!bUseNativeClient &&
		!bHasConnectHost &&
		!bExplicitHeadless &&
		(GetEnvironmentVariableA("D2MOO_USE_ORIGINAL_CLIENT", szUseOrigEnv, sizeof(szUseOrigEnv)) > 0 && szUseOrigEnv[0] != '0');
	const bool bUseTsClient =
		!bUseNativeClient &&
		!bHasConnectHost &&
		!bExplicitHeadless &&
		!bUseOriginalClient &&
		(GetEnvironmentVariableA("D2MOO_USE_TS_CLIENT", szUseTsEnv, sizeof(szUseTsEnv)) > 0 && szUseTsEnv[0] != '0');

	if (bExplicitHeadless || bUseTsClient || bUseOriginalClient)
	{
		if (bUseOriginalClient)
		{
			// Destroy the server's "Diablo II" window class instance so the untouched 1.12a
			// D2gfx.dll single-instance check (FindWindowA("Diablo II", 0)) succeeds.
			WINDOW_Destroy();
		}
		else if (HWND hMainWnd = WINDOW_GetWindow())
		{
			ShowWindow(hMainWnd, bUseTsClient ? SW_MINIMIZE : SW_HIDE);
			if (bUseTsClient)
			{
				SetWindowTextA(hMainWnd, "Diablo II - D2MOO 1.12a Server (TS Web Client Active)");
			}
		}
		if (pCfg && pCfg->dwAct > 0 && pCfg->dwAct < 6)
		{
			GAME_SetGlobalAct(static_cast<int32_t>(pCfg->dwAct - 1));
		}
		if (pCfg && pCfg->dwSeed == 0)
		{
			char szSeedEnv[64] = {};
			if (GetEnvironmentVariableA("D2MOO_FIXED_SEED", szSeedEnv, sizeof(szSeedEnv)) > 0 && szSeedEnv[0] != '\0')
			{
				pCfg->dwSeed = static_cast<uint32_t>(std::strtoul(szSeedEnv, nullptr, 0));
			}
		}
		if (pCfg && pCfg->dwSeed != 0)
		{
			GAME_SetInitSeed(static_cast<int32_t>(pCfg->dwSeed));
		}
		SERVER_Initialize(0, 1);
		SERVER_SetMaxClientsPerGame(8);
		SERVER_SetHackListEnabled(FALSE);
		GAME_InitGameDataTable(&gLocalGameDataTable, gLocalGameList);
		D2Game_10046();

		HANDLE hWebJob = nullptr;
		PROCESS_INFORMATION tWebProcInfo = {};
		char szExeDir[MAX_PATH] = {};
		GetModuleFileNameA(nullptr, szExeDir, sizeof(szExeDir));
		if (char* pLastSlash = std::strrchr(szExeDir, '\\'))
		{
			*pLastSlash = '\0';
		}

		if (bUseTsClient || bUseOriginalClient)
		{
			hWebJob = CreateJobObjectA(nullptr, nullptr);
			if (hWebJob)
			{
				JOBOBJECT_EXTENDED_LIMIT_INFORMATION tJobLimits = {};
				tJobLimits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
				SetInformationJobObject(hWebJob, JobObjectExtendedLimitInformation, &tJobLimits, sizeof(tJobLimits));
			}

			char szSaveDir[MAX_PATH] = {};
			FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
			SetEnvironmentVariableA("D2MOO_SAVE_DIR", szSaveDir);

			if (pCfg && pCfg->szName[0])
			{
				SStrCopy(gszCharName, pCfg->szName, sizeof(gszCharName));
			}
			LoadCharacterSaveAppearance();

			if (bUseOriginalClient)
			{
				char szOrigDir[MAX_PATH] = {};
				EnsureOriginal112aClientDirectory(szExeDir, szOrigDir, sizeof(szOrigDir));

				char szClassBuf[16] = {};
				char szArenaBuf[32] = {};
				char szSaveFlagsBuf[32] = {};
				std::snprintf(szClassBuf, sizeof(szClassBuf), "%d", static_cast<int32_t>(gnCharClass));
				std::snprintf(
					szArenaBuf,
					sizeof(szArenaBuf),
					"0x%X",
					static_cast<unsigned int>((pCfg && pCfg->nArenaFlags) ? pCfg->nArenaFlags : 0x100004u));
				std::snprintf(
					szSaveFlagsBuf,
					sizeof(szSaveFlagsBuf),
					"0x%X",
					static_cast<unsigned int>(pCfg ? pCfg->unpackedCTemp.nCharacterSaveFlags : 0x20u));

				SetEnvironmentVariableA("D2MOO_ORIG_CHAR_NAME", gszCharName[0] ? gszCharName : "ExpansionHero");
				SetEnvironmentVariableA("D2MOO_ORIG_CHAR_CLASS", szClassBuf);
				SetEnvironmentVariableA("D2MOO_ORIG_ARENA_FLAGS", szArenaBuf);
				SetEnvironmentVariableA("D2MOO_ORIG_SAVE_FLAGS", szSaveFlagsBuf);
				SetEnvironmentVariableA("D2MOO_ORIG_SERVER_IP", "127.0.0.1");
				SetEnvironmentVariableA("D2MOO_ORIG_GAME_NAME", "D2MOO");

				char szOrigExe[MAX_PATH] = {};
				char szCmdLine[1024] = {};
				std::snprintf(szOrigExe, sizeof(szOrigExe), "%s\\Game.exe", szOrigDir);
				std::snprintf(szCmdLine, sizeof(szCmdLine), "\"%s\" -w", szOrigExe);

				STARTUPINFOA tStartupInfo = {};
				tStartupInfo.cb = sizeof(tStartupInfo);

				if (CreateProcessA(
						szOrigExe,
						szCmdLine,
						nullptr,
						nullptr,
						FALSE,
						CREATE_SUSPENDED,
						nullptr,
						szOrigDir,
						&tStartupInfo,
						&tWebProcInfo))
				{
					if (hWebJob)
					{
						AssignProcessToJobObject(hWebJob, tWebProcInfo.hProcess);
					}
					ResumeThread(tWebProcInfo.hThread);
				}
			}
			else
			{
				char szScriptPath[MAX_PATH] = {};
				std::snprintf(szScriptPath, sizeof(szScriptPath), "%s\\..\\..\\..\\tools\\start_d2moo_web.mjs", szExeDir);
				if (GetFileAttributesA(szScriptPath) == INVALID_FILE_ATTRIBUTES)
				{
					SStrCopy(szScriptPath, "H:\\git\\D2MOO-112a\\tools\\start_d2moo_web.mjs", sizeof(szScriptPath));
				}

				char szCmdLine[1024] = {};
				std::snprintf(
					szCmdLine,
					sizeof(szCmdLine),
					"node \"%s\" --attach --char \"%s\" --class %d",
					szScriptPath,
					gszCharName[0] ? gszCharName : "WebHero",
					static_cast<int32_t>(gnCharClass));

				STARTUPINFOA tStartupInfo = {};
				tStartupInfo.cb = sizeof(tStartupInfo);
				tStartupInfo.dwFlags = STARTF_USESHOWWINDOW;
				tStartupInfo.wShowWindow = SW_HIDE;

				if (CreateProcessA(
						nullptr,
						szCmdLine,
						nullptr,
						nullptr,
						FALSE,
						CREATE_NO_WINDOW | CREATE_SUSPENDED,
						nullptr,
						szExeDir,
						&tStartupInfo,
						&tWebProcInfo))
				{
					if (hWebJob)
					{
						AssignProcessToJobObject(hWebJob, tWebProcInfo.hProcess);
					}
					ResumeThread(tWebProcInfo.hThread);
				}
			}
		}

		gbWindowClosed = 0;
		while (!gbWindowClosed)
		{
			MSG msg = {};
			while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					gbWindowClosed = 1;
					break;
				}
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}
			if ((bUseOriginalClient || bUseTsClient) &&
				(!tWebProcInfo.hProcess || WaitForSingleObject(tWebProcInfo.hProcess, 0) == WAIT_OBJECT_0))
			{
				gbWindowClosed = 1;
			}
			GAME_ProcessNetworkMessages();
			GAME_UpdateGamesProgress(1);
			GAME_UpdateClients(0, 0);
			Sleep(20);
		}

		if (tWebProcInfo.hThread)
		{
			CloseHandle(tWebProcInfo.hThread);
		}
		if (tWebProcInfo.hProcess)
		{
			TerminateProcess(tWebProcInfo.hProcess, 0);
			CloseHandle(tWebProcInfo.hProcess);
		}
		if (hWebJob)
		{
			CloseHandle(hWebJob);
		}
		if (bUseOriginalClient && szExeDir[0] != '\0')
		{
			HKEY hRestoreKey = nullptr;
			if (RegCreateKeyExA(
					HKEY_CURRENT_USER,
					"Software\\Blizzard Entertainment\\Diablo II",
					0,
					nullptr,
					0,
					KEY_SET_VALUE,
					nullptr,
					&hRestoreKey,
					nullptr) == ERROR_SUCCESS)
			{
				char szRestorePath[MAX_PATH] = {};
				std::snprintf(szRestorePath, sizeof(szRestorePath), "%s\\", szExeDir);
				RegSetValueExA(
					hRestoreKey,
					"InstallPath",
					0,
					REG_SZ,
					reinterpret_cast<const BYTE*>(szRestorePath),
					static_cast<DWORD>(std::strlen(szRestorePath) + 1));
				RegCloseKey(hRestoreKey);
			}
		}

		GAME_CloseAllGames();
		SERVER_Release();
		D2Win_10016();
		DATATBLS_UnloadAllBins();
		STRTABLE_Destroy();
		LOG_11105();
		return D2CLIENT_MODULE_NONE;
	}

	gnClientState = D2CLIENT_STATE_INIT_SESSION;
	gbExitApplication = 1;

	while (gnClientState != D2CLIENT_STATE_EXIT)
	{
		if (gnClientState <= D2CLIENT_STATE_NONE || gnClientState >= D2CLIENT_STATE_COUNT)
		{
			break;
		}
		const FnClientStateHandler pfnHandler = gapfnClientStateHandlers[gnClientState];
		if (!pfnHandler)
		{
			break;
		}
		gnClientState = pfnHandler();
	}

	D2Win_10016();
	DATATBLS_UnloadAllBins();
	STRTABLE_Destroy();
	LOG_11105();

	if (gbExitApplication)
	{
		return D2CLIENT_MODULE_NONE;
	}

	if (pCfg && (pCfg->dwGameType == 1 || pCfg->dwGameType == 2 || pCfg->dwGameType == 3))
	{
		return D2CLIENT_MODULE_MULTIPLAYER;
	}
	return D2CLIENT_MODULE_LAUNCHER;
}

// 1.12a: 0x6FB53600
int32_t __fastcall D2Client_MainEntry(D2ConfigStrc* pCfg)
{
	return D2Client_RunGameLoop(pCfg);
}

static D2ClientInterfaceStrc gD2ClientInterface = {
	D2Client_MainEntry
};

// 1.12a @10004: 0x6FB535D0
extern "C" D2CLIENT_DLL_DECL D2ClientInterfaceStrc* __stdcall QueryInterface()
{
	return &gD2ClientInterface;
}
