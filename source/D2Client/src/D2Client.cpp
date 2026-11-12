#include "D2Client.h"
#include "reconstructed/D2ClientReconstructed.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <vector>
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
#include <DataTbls/BeltsTbls.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/LevelsTbls.h>
#include <DataTbls/ItemsTbls.h>
#include <DataTbls/MonsterTbls.h>
#include <DataTbls/ObjectsTbls.h>
#include <DataTbls/SkillsTbls.h>
#include <D2BitManip.h>
#include <D2QuestRecord.h>
#include <D2Skills.h>
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

// ============================================================================
// Reconstructed 1.12a D2Client Module Entry Points & Cross-Module Globals
// ============================================================================

// Msg.cpp (C2S packet encoders & traffic counters)
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

// UNIT_CUnit.cpp & UNIT_Item.cpp
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
D2UnitStrc* __stdcall D2Client_GetLocalPlayerUnit();
void __fastcall D2Client_SetLocalPlayerUnit(D2UnitStrc* pPlayerUnit);
D2UnitStrc* __fastcall D2Client_FindServerSideUnit(uint32_t dwUnitGUID, uint32_t nUnitType);
D2UnitStrc* __fastcall D2Client_FindClientSideUnit(uint32_t dwUnitGUID, uint32_t nUnitType);
D2UnitStrc* __fastcall D2Client_RegisterUnitInHashTable(D2UnitStrc* pUnit);
void __fastcall D2Client_PacketHandler_ItemAction9C(const uint8_t* pPacket);
void __fastcall D2Client_PacketHandler_ItemAction9D(const uint8_t* pPacket);

// View.cpp, ENGINE_Gfx.cpp, DRAW_dLightMap.cpp
struct D2ViewportStrc;
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
void __stdcall LIGHTMAP_InitializeReciprocalTableAndActCallback();
void __stdcall LIGHTMAP_GetRoomAmbientLight(
	BOOL bCheckSpecialRoomLight,
	D2ActiveRoomStrc* pRoom,
	int32_t* pOutIntensity,
	uint8_t* pOutRed,
	uint8_t* pOutGreen,
	uint8_t* pOutBlue);
void __stdcall LIGHTMAP_ResetAmbientGrid(BOOL bClearToZero);

// SCmd.cpp (S2C Packet Dispatch Table)
typedef void(__fastcall* SCMD_PacketHandlerFn)(uint8_t* pPacket);
typedef void(__fastcall* SCMD_UnitPacketHandlerFn)(D2UnitStrc* pUnit, uint8_t* pPacket);
struct D2SCmdTableStrc
{
	SCMD_PacketHandlerFn     pfHandler;
	int32_t                  nPacketSize;
	SCMD_UnitPacketHandlerFn pfUnitHandler;
};
extern const D2SCmdTableStrc sgptSCmdTable[0xAF];
extern D2UnitStrc* gpClientPlayer;
void __fastcall SCMD_FreeUnitPacketQueue(D2UnitStrc* pUnit);

// UI_ui.cpp, UI_inv.cpp, UI_QuestLog.cpp, UI_automap.cpp, Sound_SoundHdr.cpp (CHARSTATS), GAME_Game.cpp (SKILLTREE)
struct D2WinMsgStrc
{
	HWND     hWnd;
	UINT     nMessage;
	WPARAM   wParam;
	union
	{
		LPARAM lParam;
		struct
		{
			uint16_t nX;
			uint16_t nY;
		};
	};
	uint32_t dwCommand;
	uint32_t dwKeyFlags;
	BOOL     bHandled;
	LRESULT  lResult;
};

struct D2GSPacketSrv5DStrc;
struct D2AutomapLayerStrc;
struct D2MsgStrc;

void __stdcall UI_InitAllPanels();
void __stdcall UI_ReleaseAllAssets();
void __fastcall UI_RenderHUD();
int32_t __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t bMoveMouse);
void __fastcall UI_ToggleMiniPanel();
void __fastcall PANEL_OnRButtonDown(D2MsgStrc* pMsg);

void __stdcall INV_SyncExpansionModeFlag();
void __stdcall INV_InitInventoryLayoutRectsAndGrids();
void __fastcall INV_SetPanelMode(int32_t nMode);
void __stdcall INV_OnInventoryPanelMouseDown(D2WinMsgStrc* pMsg);
void __stdcall INV_OnInventoryPanelMouseUp(D2WinMsgStrc* pMsg);
void __stdcall INV_OnStashPanelMouseUp(D2WinMsgStrc* pMsg);
void __stdcall INV_OnHoradricCubeMouseUp(D2WinMsgStrc* pMsg);
void __stdcall INV_OnMercInventoryMouseUp(D2WinMsgStrc* pMsg);

void __stdcall QUESTLOG_InitSession(void);
void __stdcall QUESTLOG_TogglePanel(int32_t bCenterCursor);
int32_t __stdcall QUESTLOG_ClosePanel(int32_t bCenterCursor);
void __stdcall QUESTLOG_OnPanelMouseDown(void* pMsg);
void __stdcall QUESTLOG_OnPanelMouseUp(void* pMsg);
void __fastcall QUESTLOG_UpdateServerQuestStateArray(const uint8_t* pStateBytes);
void __stdcall QUESTLOG_HandleQuestAlertPacket(const D2GSPacketSrv5DStrc* pPacket);

int32_t __fastcall CHARSTATS_HandleMouseDown(void* pMsg);
int32_t __fastcall CHARSTATS_HandleMouseUp(void* pMsg);

void __stdcall AUTOMAP_Initialize(void);
void __stdcall AUTOMAP_FreeAllLayers(void);
D2AutomapLayerStrc* __fastcall AUTOMAP_InitLayer(int32_t nLayerNo);
void __stdcall AUTOMAP_InitTownLayer(int32_t nLevelId, int32_t nVariant, int32_t nTileX, int32_t nTileY);
void __stdcall AUTOMAP_RevealRoomFull(D2ActiveRoomStrc* pRoom);
void __stdcall AUTOMAP_UpdateReveal(void);
void __fastcall AUTOMAP_CenterView(int32_t bForceCenter);
void __fastcall PALSHIFT_InitTables(void);
void __fastcall PALSHIFT_FreeTables(void);
void __fastcall PALSHIFT_ResetAndPreloadDefaultMonsters(void);

// Additional Cross-Module Reconstructed State Globals
extern void* g_pClientAct;
extern D2UnitStrc* g_pPlayerUnit;
extern D2SkillDescTxt* g_pSkillDescTxt_6FDD1C44;
extern int32_t g_nSkillDescTxtRecordCount_6FDD1C4C;
extern int32_t gdwActiveInteractNpcGUID;
extern int32_t g_bExpansionChar;
extern BOOL g_bExpansionMode;
extern int32_t g_nViewShiftState;
extern BOOL g_bEscMenuActive;
extern BOOL g_bSkillTreeOrSelectOpen;
extern int32_t g_nSkillTreeTab_6FBA79FC;
extern int32_t g_nEquippedBeltRows;

// ============================================================================
// Session & Client Loop State (Zero Shadow World/Item/Stat/UI Arrays)
// ============================================================================
static D2ConfigStrc* gpD2Config = nullptr;                     // 0x6FBCB908
static int32_t gbD2ClientInitialized = 0;                      // 0x6FBCD75C
static const uint8_t gaD2ClientBytes[3] = { 0xFF, 0x40, 0x30 };// 0x6FBB1678
static int32_t gnClientState = D2CLIENT_STATE_INIT_SESSION;    // 0x6FBA8B30
static int32_t gbExitApplication = 1;                          // 0x6FBA8B70
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

static bool gbRunEnabled = true;
static bool gbSoundEnabled = true;
static bool gbScaled1200x900 = true;
static bool gbChatInputOpen = false;
static char gszChatInputBuf[96] = {};
static int32_t gnChatInputLen = 0;
static bool gbShowFpsOverlay = false;

static float gfPlayerWorldX = 0.0f;
static float gfPlayerWorldY = 0.0f;
static float gfTargetWorldX = 0.0f;
static float gfTargetWorldY = 0.0f;
static bool gbPlayerMoving = false;
static bool gbMouseLeftHeld = false;
static POINT gMousePos = { 400, 300 };
static std::vector<POINT> g_vInteractivePath;
static size_t g_uInteractivePathIdx = 0;

static uint32_t gdwClientPlayerGuid = 0;
static int32_t gnServerSubX = 0;
static int32_t gnServerSubY = 0;
static int32_t gnAuthServerSubX = 0;
static int32_t gnAuthServerSubY = 0;
static bool gbServerActLoaded = false;
static uint8_t gnCurrentAct = 0;
static uint32_t gdwServerMapSeed = 0x112A2026u;
static uint16_t gwServerTownLevelId = LEVEL_ROGUEENCAMPMENT;
static uint16_t gnCurrentLevelId = LEVEL_ROGUEENCAMPMENT;
static int32_t gnCurrentAutomapLayerNo = -1;

// Honest Telemetry Counters (populated strictly from real S2C packets & reconstructed calls)
static uint32_t gdwServerTicks = 0;
static uint32_t gdwGamePacketsReceived = 0;
static uint32_t gdwSCmdPacketsDispatched = 0;
static uint32_t gdwInlinePacketsHandled = 0;
static uint32_t gdwNpcIntrosReceived = 0;
static uint32_t gdwVendorItemsReceived = 0;
static uint32_t gdwAutotestVendorShopItems = 0;
static uint32_t gdwTransmutesSent = 0;
static uint32_t gdwSlashCommandsExecuted = 0;
static uint32_t gdwAutomapRoomsPopulated = 0;

// ============================================================================
// Reconstructed State Synchronization & Helper Utilities
// ============================================================================
static int32_t ComputeEquippedBeltRows(D2UnitStrc* pPlayer)
{
	if (!pPlayer || !pPlayer->pInventory)
	{
		return 1;
	}
	for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pPlayer->pInventory);
		 pItem != nullptr;
		 pItem = INVENTORY_GetNextItem(pItem))
	{
		if (INVENTORY_UnitIsItem(pItem) &&
			pItem->dwAnimMode == IMODE_EQUIP &&
			ITEMS_GetBodyLocation(pItem) == BODYLOC_BELT)
		{
			D2ItemsTxt* pTxt = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
			if (pTxt)
			{
				D2BeltsTxt tBeltTxt = {};
				DATATBLS_GetBeltsTxtRecord(pTxt->nBelt, 1, &tBeltTxt);
				if (tBeltTxt.nBoxes >= 4)
				{
					return std::max<int32_t>(1, std::min<int32_t>(4, tBeltTxt.nBoxes / 4));
				}
			}
			return 2;
		}
	}
	return 1;
}

static void SyncReconstructedClientGlobals()
{
	gpClientPlayer = gpPlayerUnit;
	g_pPlayerUnit = gpPlayerUnit;
	g_pClientAct = gpClientAct;
	g_bExpansionChar = gbIsExpansion;
	g_bExpansionMode = (gbIsExpansion != 0) ? TRUE : FALSE;
	g_nViewShiftState = gnViewportShiftMode;
	g_bEscMenuActive = (gaPanelStates[D2PANEL_ESCMENU] != 0) ? TRUE : FALSE;
	g_bSkillTreeOrSelectOpen =
		(gaPanelStates[D2PANEL_SKILLTREE] != 0 || gaPanelStates[D2PANEL_SKILLSELECT] != 0) ? TRUE : FALSE;
	gnMouseX = gMousePos.x;
	gnMouseY = gMousePos.y;
	gdwActiveInteractNpcGUID = g_nInteractingNpcGUID;
	gdwClientGameTick = gdwServerTicks;

	if (sgptDataTables)
	{
		g_pSkillDescTxt_6FDD1C44 = sgptDataTables->pSkillDescTxt;
		g_nSkillDescTxtRecordCount_6FDD1C4C = sgptDataTables->nSkillDescTxtRecordCount;
	}

	if (gpPlayerUnit)
	{
		const int32_t nComputedRows = ComputeEquippedBeltRows(gpPlayerUnit);
		if (nComputedRows > g_nEquippedBeltRows)
		{
			g_nEquippedBeltRows = nComputedRows;
		}
		if (D2ActiveRoomStrc* pRoom = UNITS_GetRoom(gpPlayerUnit))
		{
			const int32_t nLvlId = DUNGEON_GetLevelIdFromRoom(pRoom);
			if (nLvlId > 0)
			{
				gnCurrentLevelId = static_cast<uint16_t>(nLvlId);
				D2LevelDefBin* pLvlDef = DATATBLS_GetLevelDefRecord(nLvlId);
				const int32_t nLayerNo = (pLvlDef && pLvlDef->dwLayer > 0)
					? static_cast<int32_t>(pLvlDef->dwLayer)
					: nLvlId;
				if (nLayerNo != gnCurrentAutomapLayerNo)
				{
					gnCurrentAutomapLayerNo = nLayerNo;
					AUTOMAP_InitLayer(nLayerNo);
				}
			}
		}
	}
}

static void SeedDefaultPlayerCharStats(D2UnitStrc* pPlrUnit, uint8_t nClassId)
{
	if (!pPlrUnit || !sgptDataTables || !sgptDataTables->pCharStatsTxt)
	{
		return;
	}
	const int32_t nSafeCls = (nClassId < 7 && nClassId < sgptDataTables->nCharStatsTxtRecordCount) ? nClassId : 0;
	D2CharStatsTxt* pCharStats = &sgptDataTables->pCharStatsTxt[nSafeCls];
	if (STATLIST_UnitGetStatValue(pPlrUnit, STAT_LEVEL, 0) <= 0)
	{
		STATLIST_SetUnitStat(pPlrUnit, STAT_LEVEL, std::max<int32_t>(1, gnCharLevel), 0);
	}
	if (STATLIST_UnitGetStatValue(pPlrUnit, STAT_STRENGTH, 0) <= 0)
	{
		STATLIST_SetUnitStat(pPlrUnit, STAT_STRENGTH, pCharStats->nStr, 0);
		STATLIST_SetUnitStat(pPlrUnit, STAT_DEXTERITY, pCharStats->nDex, 0);
		STATLIST_SetUnitStat(pPlrUnit, STAT_ENERGY, pCharStats->nInt, 0);
		STATLIST_SetUnitStat(pPlrUnit, STAT_VITALITY, pCharStats->nVit, 0);
	}
	const int32_t nBaseMaxHp = (static_cast<int32_t>(pCharStats->nVit) + static_cast<int32_t>(pCharStats->nLifeAdd)) << 8;
	const int32_t nBaseMaxMp = static_cast<int32_t>(pCharStats->nInt) << 8;
	const int32_t nBaseMaxStam = static_cast<int32_t>(pCharStats->nStamina) << 8;
	if (STATLIST_UnitGetStatValue(pPlrUnit, STAT_MAXHP, 0) <= 0)
	{
		STATLIST_SetUnitStat(pPlrUnit, STAT_MAXHP, nBaseMaxHp, 0);
		STATLIST_SetUnitStat(pPlrUnit, STAT_HITPOINTS, nBaseMaxHp, 0);
	}
	if (STATLIST_UnitGetStatValue(pPlrUnit, STAT_MAXMANA, 0) <= 0)
	{
		STATLIST_SetUnitStat(pPlrUnit, STAT_MAXMANA, nBaseMaxMp, 0);
		STATLIST_SetUnitStat(pPlrUnit, STAT_MANA, nBaseMaxMp, 0);
	}
	if (STATLIST_UnitGetStatValue(pPlrUnit, STAT_MAXSTAMINA, 0) <= 0)
	{
		STATLIST_SetUnitStat(pPlrUnit, STAT_MAXSTAMINA, nBaseMaxStam, 0);
		STATLIST_SetUnitStat(pPlrUnit, STAT_STAMINA, nBaseMaxStam, 0);
	}
	if (pPlrUnit->pSkills && !UNITS_GetLeftSkill(pPlrUnit))
	{
		SKILLS_AddSkill(pPlrUnit, 0); // Attack
		SKILLS_SetLeftActiveSkill(pPlrUnit, 0, 0xFFFFFFFFu);
		SKILLS_SetRightActiveSkill(pPlrUnit, 0, 0xFFFFFFFFu);
	}
}

// ============================================================================
// Client DRLG Room & Outdoor Wilderness Border Streaming
// ============================================================================
static void SpawnClientRoomPresetUnits(D2ActiveRoomStrc* pRoom)
{
	if (!pRoom || !pRoom->pDrlgRoom)
	{
		return;
	}
	RECT rcSub = {};
	DUNGEON_GetSubtileRect(pRoom, &rcSub);
	for (D2PresetUnitStrc* pPreset = DUNGEON_GetPresetUnitsFromRoom(pRoom); pPreset != nullptr; pPreset = pPreset->pNext)
	{
		if (pPreset->nUnitType != UNIT_OBJECT)
		{
			continue;
		}
		const int32_t nObjX = rcSub.left + pPreset->nXpos;
		const int32_t nObjY = rcSub.top + pPreset->nYpos;
		const uint32_t dwSyntheticGuid =
			0x40000000u ^
			(static_cast<uint32_t>(nObjX & 0x7FFF) << 15) ^
			static_cast<uint32_t>(nObjY & 0x7FFF);
		if (!D2Client_FindServerSideUnit(dwSyntheticGuid, UNIT_OBJECT))
		{
			D2UnitStrc* pObj = D2Client_AllocClientObjectUnit(
				UNIT_OBJECT,
				pPreset->nIndex,
				dwSyntheticGuid,
				nObjX,
				nObjY,
				pPreset->nMode,
				0,
				0);
			if (pObj)
			{
				GFX_InitUnitCompositeGfx(pObj);
			}
		}
	}
}

extern "C" D2ActiveRoomStrc* __fastcall D2Client_SafeStreamRoomAtTile(int32_t nTileX, int32_t nTileY, int32_t nExplicitLevelId)
{
	if (!gpClientAct || !gpClientAct->pDrlg)
	{
		return nullptr;
	}
	if (D2ActiveRoomStrc* pExisting = DUNGEON_FindRoomByTileCoordinates(gpClientAct, nTileX, nTileY))
	{
		return pExisting;
	}

	D2DrlgStrc* pDrlg = gpClientAct->pDrlg;
	D2DrlgRoomStrc* pMatchedRoomEx = nullptr;
	int32_t nMatchedLevelId = 0;

	if (nExplicitLevelId > 0)
	{
		if (D2DrlgLevelStrc* pExplicitLvl = DRLG_GetLevel(pDrlg, nExplicitLevelId))
		{
			if (!pExplicitLvl->pFirstRoomEx)
			{
				DRLG_InitLevel(pExplicitLvl);
			}
			for (D2DrlgRoomStrc* pRoomEx = pExplicitLvl->pFirstRoomEx; pRoomEx != nullptr; pRoomEx = pRoomEx->pDrlgRoomNext)
			{
				if (pRoomEx->nTileWidth > 0 && pRoomEx->nTileHeight > 0 &&
					nTileX >= pRoomEx->nTileXPos && nTileX < pRoomEx->nTileXPos + pRoomEx->nTileWidth &&
					nTileY >= pRoomEx->nTileYPos && nTileY < pRoomEx->nTileYPos + pRoomEx->nTileHeight)
				{
					pMatchedRoomEx = pRoomEx;
					nMatchedLevelId = nExplicitLevelId;
					break;
				}
			}
		}
	}

	if (!pMatchedRoomEx)
	{
		// Check all valid DRLG levels; never query DRLG_GetLevel(pDrlg, 0) for out-of-bounds void tiles
		for (D2DrlgLevelStrc* pLvl = pDrlg->pLevel; pLvl != nullptr; pLvl = pLvl->pNextLevel)
		{
			if (pLvl->nLevelId <= 0 || pLvl->nWidth <= 0 || pLvl->nHeight <= 0)
			{
				continue;
			}
			if (nTileX >= pLvl->nPosX && nTileX < pLvl->nPosX + pLvl->nWidth &&
				nTileY >= pLvl->nPosY && nTileY < pLvl->nPosY + pLvl->nHeight)
			{
				if (!pLvl->pFirstRoomEx)
				{
					DRLG_InitLevel(pLvl);
				}
				for (D2DrlgRoomStrc* pRoomEx = pLvl->pFirstRoomEx; pRoomEx != nullptr; pRoomEx = pRoomEx->pDrlgRoomNext)
				{
					if (pRoomEx->nTileWidth > 0 && pRoomEx->nTileHeight > 0 &&
						nTileX >= pRoomEx->nTileXPos && nTileX < pRoomEx->nTileXPos + pRoomEx->nTileWidth &&
						nTileY >= pRoomEx->nTileYPos && nTileY < pRoomEx->nTileYPos + pRoomEx->nTileHeight)
					{
						pMatchedRoomEx = pRoomEx;
						nMatchedLevelId = pLvl->nLevelId;
						break;
					}
				}
				if (pMatchedRoomEx)
				{
					break;
				}
			}
		}
	}

	if (!pMatchedRoomEx || nMatchedLevelId <= 0)
	{
		return nullptr;
	}

	D2ActiveRoomStrc* pPlayerRoom = gpPlayerUnit ? UNITS_GetRoom(gpPlayerUnit) : nullptr;
	DUNGEON_SetClientIsInSight(gpClientAct, nMatchedLevelId, pMatchedRoomEx->nTileXPos, pMatchedRoomEx->nTileYPos, pPlayerRoom);
	D2ActiveRoomStrc* pStreamed = pMatchedRoomEx->pRoom;
	if (!pStreamed)
	{
		pStreamed = DUNGEON_FindRoomByTileCoordinates(gpClientAct, pMatchedRoomEx->nTileXPos, pMatchedRoomEx->nTileYPos);
	}
	if (pStreamed)
	{
		SpawnClientRoomPresetUnits(pStreamed);
		AUTOMAP_RevealRoomFull(pStreamed);
		++gdwAutomapRoomsPopulated;
	}
	return pStreamed;
}

static inline D2ActiveRoomStrc* SafeStreamClientRoomAtTile(int32_t nTileX, int32_t nTileY, int32_t nExplicitLevelId = 0)
{
	return D2Client_SafeStreamRoomAtTile(nTileX, nTileY, nExplicitLevelId);
}

static void StreamClientNeighborhoodAroundTile(int32_t nCenterTileX, int32_t nCenterTileY, int32_t nRadiusTiles)
{
	if (!gpClientAct)
	{
		return;
	}
	// Step by 4 tiles (rooms are >= 8x8 tiles) so no outdoor wilderness room can fall between sample points
	for (int32_t dy = -nRadiusTiles; dy <= nRadiusTiles; dy += 4)
	{
		for (int32_t dx = -nRadiusTiles; dx <= nRadiusTiles; dx += 4)
		{
			SafeStreamClientRoomAtTile(nCenterTileX + dx, nCenterTileY + dy);
		}
	}
}

static int32_t GetAdjacentWildernessLevelForTown(int32_t nTownLevelId)
{
	switch (nTownLevelId)
	{
	case LEVEL_ROGUEENCAMPMENT:        return LEVEL_BLOODMOOR;
	case LEVEL_LUTGHOLEIN:             return LEVEL_ROCKYWASTE;
	case LEVEL_KURASTDOCKTOWN:         return LEVEL_SPIDERFOREST;
	case LEVEL_THEPANDEMONIUMFORTRESS: return LEVEL_OUTERSTEPPES;
	case LEVEL_HARROGATH:              return LEVEL_BLOODYFOOTHILLS;
	default:                           return 0;
	}
}

static void PrestreamTownAndAdjacentWilderness(int32_t nTownLevelId)
{
	if (!gpClientAct || !gpClientAct->pDrlg || nTownLevelId <= 0)
	{
		return;
	}

	D2DrlgLevelStrc* pTownLvl = DRLG_GetLevel(gpClientAct->pDrlg, nTownLevelId);
	if (!pTownLvl)
	{
		return;
	}
	if (!pTownLvl->pFirstRoomEx)
	{
		DRLG_InitLevel(pTownLvl);
	}

	const int32_t nWildLevelId = GetAdjacentWildernessLevelForTown(nTownLevelId);
	D2DrlgLevelStrc* pWildLvl = (nWildLevelId > 0) ? DRLG_GetLevel(gpClientAct->pDrlg, nWildLevelId) : nullptr;
	if (pWildLvl && !pWildLvl->pFirstRoomEx)
	{
		DRLG_InitLevel(pWildLvl);
	}

	D2LevelDefBin* pLvlDef = DATATBLS_GetLevelDefRecord(nTownLevelId);
	const int32_t nLayerNo = (pLvlDef && pLvlDef->dwLayer > 0)
		? static_cast<int32_t>(pLvlDef->dwLayer)
		: nTownLevelId;
	gnCurrentAutomapLayerNo = nLayerNo;
	AUTOMAP_InitLayer(nLayerNo);

	// 1. Stream all town rooms and their cross-level DRLG neighbor rooms (ppRoomsNear)
	for (D2DrlgRoomStrc* pRoomEx = pTownLvl->pFirstRoomEx; pRoomEx != nullptr; pRoomEx = pRoomEx->pDrlgRoomNext)
	{
		SafeStreamClientRoomAtTile(pRoomEx->nTileXPos, pRoomEx->nTileYPos, nTownLevelId);
		if (pRoomEx->ppRoomsNear && pRoomEx->nRoomsNear > 0)
		{
			for (int32_t i = 0; i < pRoomEx->nRoomsNear; ++i)
			{
				if (D2DrlgRoomStrc* pNear = pRoomEx->ppRoomsNear[i])
				{
					const int32_t nNearLvl = pNear->pLevel ? pNear->pLevel->nLevelId : 0;
					SafeStreamClientRoomAtTile(pNear->nTileXPos, pNear->nTileYPos, nNearLvl);
				}
			}
		}
	}

	// 2. Pre-stream adjacent wilderness rooms within 28 tiles of the town boundary so the town exit bridge is seamless
	if (pWildLvl && pWildLvl->pFirstRoomEx)
	{
		const int32_t nTownMinX = pTownLvl->nPosX - 28;
		const int32_t nTownMaxX = pTownLvl->nPosX + pTownLvl->nWidth + 28;
		const int32_t nTownMinY = pTownLvl->nPosY - 28;
		const int32_t nTownMaxY = pTownLvl->nPosY + pTownLvl->nHeight + 28;
		for (D2DrlgRoomStrc* pWildRoom = pWildLvl->pFirstRoomEx; pWildRoom != nullptr; pWildRoom = pWildRoom->pDrlgRoomNext)
		{
			const int32_t rx0 = pWildRoom->nTileXPos;
			const int32_t ry0 = pWildRoom->nTileYPos;
			const int32_t rx1 = rx0 + pWildRoom->nTileWidth;
			const int32_t ry1 = ry0 + pWildRoom->nTileHeight;
			if (rx1 >= nTownMinX && rx0 <= nTownMaxX && ry1 >= nTownMinY && ry0 <= nTownMaxY)
			{
				SafeStreamClientRoomAtTile(rx0, ry0, nWildLevelId);
			}
		}
	}
}

static void RebuildClientActWorld(uint8_t nActNo, uint32_t dwMapSeed, int32_t nTownLevelId)
{
	static const int32_t kDefaultTownLevels[5] = {
		LEVEL_ROGUEENCAMPMENT,
		LEVEL_LUTGHOLEIN,
		LEVEL_KURASTDOCKTOWN,
		LEVEL_THEPANDEMONIUMFORTRESS,
		LEVEL_HARROGATH
	};
	if (nActNo >= 5)
	{
		nActNo = 0;
	}
	if (nTownLevelId <= 0)
	{
		nTownLevelId = kDefaultTownLevels[nActNo];
	}

	// Unlink existing client units from old rooms via UNITROOM_RemoveUnitFromRoom before freeing gpClientAct
	for (int32_t t = 0; t < UNIT_TYPES_COUNT; ++t)
	{
		for (int32_t b = 0; b < 128; ++b)
		{
			D2UnitStrc* pU = gapServerSideUnitHashTables[t][b];
			while (pU != nullptr)
			{
				D2UnitStrc* pNext = pU->pListNext;
				UNITROOM_RemoveUnitFromRoom(pU);
				if (t == UNIT_PLAYER || t == UNIT_MONSTER || t == UNIT_MISSILE)
				{
					if (pU->pDynamicPath)
					{
						pU->pDynamicPath->pPreviousRoom = nullptr;
					}
				}
				if (t == UNIT_OBJECT && pU->dwUnitId >= 0x7F800000u && pU->dwUnitId < 0x7FFFFFFFu)
				{
					D2Client_RemoveServerSideUnit(UNIT_OBJECT, pU->dwUnitId);
				}
				pU = pNext;
			}
		}
	}

	for (int32_t t = 0; t < UNIT_TYPES_COUNT; ++t)
	{
		for (int32_t b = 0; b < 128; ++b)
		{
			for (D2UnitStrc* pU = gapClientSideUnitHashTables[t][b]; pU != nullptr; pU = pU->pListNext)
			{
				UNITROOM_RemoveUnitFromRoom(pU);
			}
		}
	}

	if (gpClientAct)
	{
		DUNGEON_FreeAct(gpClientAct);
		gpClientAct = nullptr;
	}

	gpClientAct = DUNGEON_AllocAct(
		nActNo,
		dwMapSeed ? dwMapSeed : 0x112A2026u,
		TRUE,
		nullptr,
		gnClientDifficulty,
		nullptr,
		nTownLevelId,
		AUTOMAP_RevealRoomFull,
		AUTOMAP_InitTownLayer);
	g_pClientAct = gpClientAct;
	GFX_UpdateActCofCache(nActNo);
	LIGHTMAP_InitializeReciprocalTableAndActCallback();
	LIGHTMAP_ResetAmbientGrid(FALSE);

	PrestreamTownAndAdjacentWilderness(nTownLevelId);

	// Re-anchor gpPlayerUnit into the newly streamed act
	if (gpPlayerUnit && gpClientAct)
	{
		gpPlayerUnit->nAct = nActNo;
		gpPlayerUnit->pDrlgAct = gpClientAct;
		if (gnServerSubX > 0 && gnServerSubY > 0)
		{
			StreamClientNeighborhoodAroundTile(gnServerSubX / 5, gnServerSubY / 5, 24);
			D2Client_TeleportUnitToCoordinates(gpPlayerUnit, gnServerSubX, gnServerSubY, 0);
		}
	}
	SyncReconstructedClientGlobals();
}

// ============================================================================
// Exports & Coordinate Helpers
// ============================================================================
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

static void SubtileToWorldIso(int32_t nSubX, int32_t nSubY, float* pIsoX, float* pIsoY)
{
	int nX = nSubX;
	int nY = nSubY;
	DUNGEON_GameSubtileToClientCoords(&nX, &nY);
	if (pIsoX) *pIsoX = static_cast<float>(nX);
	if (pIsoY) *pIsoY = static_cast<float>(nY);
}

static void ScreenPointToSubtile(int32_t nScreenX, int32_t nScreenY, int32_t* pSubX, int32_t* pSubY)
{
	const int32_t nCenterX = (gnViewportWidth > 0 ? gnViewportWidth / 2 : 400) + gnViewportShiftOffsetX;
	const int32_t nCenterY = (gnViewportHeight > 0 ? gnViewportHeight / 2 : 276);
	int nIsoX = static_cast<int>(gfPlayerWorldX) + (nScreenX - nCenterX);
	int nIsoY = static_cast<int>(gfPlayerWorldY) + (nScreenY - nCenterY);
	DUNGEON_ClientToGameSubtileCoords(&nIsoX, &nIsoY);
	if (pSubX) *pSubX = nIsoX;
	if (pSubY) *pSubY = nIsoY;
}

static bool WorldUnitToScreenPos(D2UnitStrc* pUnit, int32_t* pOutSx, int32_t* pOutSy)
{
	if (!pUnit || !gpPlayerUnit)
	{
		return false;
	}
	int nUx = UNITS_GetXPosition(pUnit);
	int nUy = UNITS_GetYPosition(pUnit);
	if (nUx <= 0 && nUy <= 0)
	{
		return false;
	}
	DUNGEON_GameSubtileToClientCoords(&nUx, &nUy);
	const int32_t nCenterX = (gnViewportWidth > 0 ? gnViewportWidth / 2 : 400) + gnViewportShiftOffsetX;
	const int32_t nCenterY = (gnViewportHeight > 0 ? gnViewportHeight / 2 : 276);
	const int32_t nSx = nCenterX + (nUx - static_cast<int32_t>(gfPlayerWorldX));
	const int32_t nSy = nCenterY + (nUy - static_cast<int32_t>(gfPlayerWorldY));
	if (pOutSx) *pOutSx = nSx;
	if (pOutSy) *pOutSy = nSy;
	return (nSx >= -64 && nSx <= gnScreenWidth + 64 && nSy >= -64 && nSy <= gnScreenHeight + 64);
}

static D2UnitStrc* FindHoveredWorldUnitAtScreenPos(int32_t nMouseX, int32_t nMouseY)
{
	if (!gpPlayerUnit || nMouseY >= gnScreenHeight - 48)
	{
		return nullptr;
	}
	if (gnViewportShiftMode == 3)
	{
		return nullptr;
	}
	if (gnViewportShiftMode == 1 && nMouseX >= gnScreenWidth / 2)
	{
		return nullptr;
	}
	if (gnViewportShiftMode == 2 && nMouseX <= gnScreenWidth / 2)
	{
		return nullptr;
	}

	D2UnitStrc* pBest = nullptr;
	int32_t nBestDistSq = 48 * 48;
	const uint32_t aTypes[3] = { UNIT_MONSTER, UNIT_OBJECT, UNIT_ITEM };
	for (uint32_t nType : aTypes)
	{
		for (int32_t b = 0; b < 128; ++b)
		{
			for (D2UnitStrc* pUnit = gapServerSideUnitHashTables[nType][b]; pUnit != nullptr; pUnit = pUnit->pListNext)
			{
				if (nType == UNIT_MONSTER &&
					(pUnit->dwAnimMode == MONMODE_DEAD || pUnit->dwAnimMode == MONMODE_DEATH))
				{
					continue;
				}
				if (nType == UNIT_ITEM &&
					pUnit->dwAnimMode != IMODE_ONGROUND && pUnit->dwAnimMode != IMODE_DROPPING)
				{
					continue;
				}
				int32_t nSx = 0, nSy = 0;
				if (!WorldUnitToScreenPos(pUnit, &nSx, &nSy))
				{
					continue;
				}
				const int32_t nHitY = (nType == UNIT_MONSTER) ? (nSy - 28) : (nSy - 10);
				const int32_t dx = nMouseX - nSx;
				const int32_t dy = nMouseY - nHitY;
				const int32_t d2 = dx * dx + dy * dy;
				if (d2 < nBestDistSq)
				{
					nBestDistSq = d2;
					pBest = pUnit;
				}
			}
		}
	}
	return pBest;
}

static bool IsTownNpcMonster(D2UnitStrc* pUnit)
{
	if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
	{
		return false;
	}
	D2MonStatsTxt* pMonTxt = DATATBLS_GetMonStatsTxtRecord(pUnit->dwClassId);
	if (!pMonTxt)
	{
		return false;
	}
	return (pMonTxt->dwMonStatsFlags & (1u << 9)) != 0 ||  // MONSTATSFLAG_NPC
		   (pMonTxt->dwMonStatsFlags & (1u << 8)) != 0 ||  // MONSTATSFLAG_INTOWN
		   (pMonTxt->dwMonStatsFlags & (1u << 24)) != 0;   // MONSTATSFLAG_INVENTORY
}

// ============================================================================
// Reconstructed C2S Packet Senders (Msg.cpp)
// ============================================================================
static void SendLeaveGamePacket()
{
	MSG_SendLeaveGame_69();
}

static void SendWalkOrRunToLocation(uint16_t wSubX, uint16_t wSubY)
{
	MSG_SendByteWordWord(gbRunEnabled ? 0x03u : 0x01u, wSubX, wSubY);
}

static void SendWalkOrRunToUnit(uint32_t dwUnitType, uint32_t dwUnitGuid)
{
	MSG_SendByte2Dwords(gbRunEnabled ? 0x04u : 0x02u, dwUnitType, dwUnitGuid);
}

static void SendRightSkillLocation(uint16_t wSubX, uint16_t wSubY)
{
	MSG_SendByteWordWord(0x0Cu, wSubX, wSubY);
}

static void SendUnitInteract(uint32_t dwUnitType, uint32_t dwUnitGuid)
{
	MSG_SendByte2Dwords(0x13u, dwUnitType, dwUnitGuid);
}

static void SendChatOrOverheadMessage(const char* szMessage, bool bOverhead)
{
	if (!szMessage || !szMessage[0])
	{
		return;
	}
	uint8_t aPacket[288] = {};
	aPacket[0] = bOverhead ? 0x14u : 0x15u;
	aPacket[1] = bOverhead ? 0x00u : 0x01u;
	aPacket[2] = 0x00u;
	const size_t nMsgLen = std::min<size_t>(250u, std::strlen(szMessage));
	std::memcpy(&aPacket[3], szMessage, nMsgLen);
	aPacket[3 + nMsgLen] = '\0';
	aPacket[3 + nMsgLen + 1] = '\0';
	aPacket[3 + nMsgLen + 2] = '\0';
	MSG_SendPacket(aPacket, 3 + nMsgLen + 3);
}

static void SendPickupGroundItem(uint32_t dwItemGuid, bool bToCursor)
{
	MSG_SendByte3Dwords(0x16u, UNIT_ITEM, dwItemGuid, bToCursor ? 1u : 0u);
}

static void SendUseBeltItem(uint32_t dwItemGuid, bool bShiftToMerc)
{
	int32_t nSubX = gnServerSubX;
	int32_t nSubY = gnServerSubY;
	if (gpPlayerUnit)
	{
		nSubX = UNITS_GetXPosition(gpPlayerUnit);
		nSubY = UNITS_GetYPosition(gpPlayerUnit);
	}
	MSG_SendByte3Dwords(
		0x26u,
		dwItemGuid,
		bShiftToMerc ? 1u : 0u,
		static_cast<uint32_t>((nSubX & 0xFFFF) | ((nSubY & 0xFFFF) << 16)));
}

static void SendTownFolkCancel(uint32_t dwNpcGuid)
{
	GAME_SendTownFolkCancel_30(UNIT_MONSTER, dwNpcGuid);
}

static void SendTownFolkMenuSelect(uint32_t dwMenuType, uint32_t dwNpcGuid, uint32_t dwData)
{
	MSG_SendByte3Dwords(0x38u, dwMenuType, dwNpcGuid, dwData);
}

static void SendAssignStatPoint(uint16_t wStatId)
{
	MSG_SendByteWord(0x3Au, wStatId);
}

static void SendWaypointSelect(uint32_t dwWaypointGuid, uint32_t dwDestLevelId)
{
	MSG_SendByte2Dwords(0x49u, dwWaypointGuid, dwDestLevelId);
}

static void SendUIButtonAction(uint16_t wActionId, uint16_t wParamLow, uint16_t wParamHigh)
{
	MSG_SendByte3Words(0x4Fu, wActionId, wParamLow, wParamHigh);
}

static void SendWeaponSwitch()
{
	MSG_SendByte(0x60u);
}

static void CloseActiveNpcSession()
{
	if (g_nInteractingNpcGUID != 0)
	{
		SendTownFolkCancel(static_cast<uint32_t>(g_nInteractingNpcGUID));
	}
	UI_SetPanelState(D2PANEL_NPCMENU, 1, 0);
	UI_SetPanelState(D2PANEL_NPCSHOP, 1, 0);
	g_bInteractingWithNpc = 0;
	g_nInteractingNpcGUID = 0;
	g_nInteractingNpcClassId = 0;
	gdwActiveInteractNpcGUID = 0;
}

static void OpenNpcInteractionSession(D2UnitStrc* pNpc)
{
	if (!pNpc)
	{
		return;
	}
	g_pTradeNpcUnit = pNpc;
	g_bInteractingWithNpc = 1;
	g_nInteractingNpcGUID = static_cast<int32_t>(pNpc->dwUnitId);
	g_nInteractingNpcClassId = static_cast<int32_t>(pNpc->dwClassId);
	gdwActiveInteractNpcGUID = g_nInteractingNpcGUID;

	SendUnitInteract(UNIT_MONSTER, pNpc->dwUnitId);
	GAME_SendTownFolkInit_2F(UNIT_MONSTER, pNpc->dwUnitId);
	UI_SetPanelState(D2PANEL_NPCMENU, 0, 0);
}

// ============================================================================
// S2C Packet Length Determination & Authoritative Reconstructed Dispatch
// ============================================================================
static int32_t GetServerPacketLength112a(const uint8_t* pBuf, int32_t nAvail)
{
	if (!pBuf || nAvail <= 0)
	{
		return 0;
	}
	const uint8_t op = pBuf[0];
	switch (op)
	{
	case 0x26: // Chat message (variable NUL-terminated name + message)
	{
		if (nAvail < 12) return nAvail;
		int32_t pos = 10;
		while (pos < nAvail && pBuf[pos] != 0) ++pos;
		++pos;
		while (pos < nAvail && pBuf[pos] != 0) ++pos;
		return std::min<int32_t>(nAvail, pos + 1);
	}
	case 0x94: // BaseSkillLevels: [0x94][count:u8][playerGuid:u32] + count * 3
		return (nAvail >= 2) ? std::min<int32_t>(nAvail, 6 + static_cast<int32_t>(pBuf[1]) * 3) : nAvail;
	case 0x9C: // ItemActionWorld: [0x9C][action][pktLen]...
	case 0x9D: // ItemActionOwned: [0x9D][action][pktLen]...
		return (nAvail >= 3 && pBuf[2] >= 3) ? std::min<int32_t>(nAvail, static_cast<int32_t>(pBuf[2])) : nAvail;
	case 0xAA: // StateInfo: [0xAA][unitType][guid:u32][pktLen]...
		return (nAvail >= 7 && pBuf[6] >= 7) ? std::min<int32_t>(nAvail, static_cast<int32_t>(pBuf[6])) : nAvail;
	case 0xAC: // AssignNPC: [0xAC][guid:u32][classId:u16][x:u16][y:u16][life:u8][pktLen:u8]...
		return (nAvail >= 13 && pBuf[12] >= 13) ? std::min<int32_t>(nAvail, static_cast<int32_t>(pBuf[12])) : nAvail;
	case 0xAE: // Warden: [0xAE][wPayloadLen]...
		return (nAvail >= 3) ? std::min<int32_t>(nAvail, 3 + *reinterpret_cast<const uint16_t*>(&pBuf[1])) : nAvail;
	case 0xAF: // CompressionInfo greeting: [0xAF][bCompression]
		return std::min<int32_t>(nAvail, 2);
	default:
		break;
	}

	if (op < 0xAF && sgptSCmdTable[op].nPacketSize > 0)
	{
		return std::min<int32_t>(nAvail, sgptSCmdTable[op].nPacketSize);
	}
	return nAvail;
}

static void HandleServerPacket112a(uint8_t* aBuffer, int32_t nPacketLen)
{
	if (!aBuffer || nPacketLen <= 0)
	{
		return;
	}

	const uint8_t nOpcode = aBuffer[0];
	++gdwGamePacketsReceived;
	SyncReconstructedClientGlobals();

	switch (nOpcode)
	{
	case 0x01: // GameLogonReceipt (8 bytes)
	{
		if (nPacketLen >= 8)
		{
			gnClientDifficulty = aBuffer[1];
			gbIsExpansion = (aBuffer[6] != 0) ? 1 : 0;
			if (sgptSCmdTable[0x01].pfHandler)
			{
				sgptSCmdTable[0x01].pfHandler(aBuffer);
				++gdwSCmdPacketsDispatched;
			}
			INV_SyncExpansionModeFlag();
			SyncReconstructedClientGlobals();
		}
		break;
	}
	case 0x02: // GameLogonSuccess (1 byte)
	{
		if (sgptSCmdTable[0x02].pfHandler)
		{
			sgptSCmdTable[0x02].pfHandler(aBuffer);
			++gdwSCmdPacketsDispatched;
		}
		else
		{
			MSG_SendJoinActComplete_6B();
		}
		break;
	}
	case 0x03: // LoadAct (12 bytes): [0x03][act][mapId:u32][townLevelId:u16][unk:u32]
	{
		if (nPacketLen >= 12)
		{
			const uint8_t nNewAct = aBuffer[1];
			const uint32_t dwNewSeed = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
			const uint16_t wTownLvl = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
			const bool bNeedRebuild =
				!gbServerActLoaded ||
				!gpClientAct ||
				nNewAct != gnCurrentAct ||
				dwNewSeed != gdwServerMapSeed ||
				wTownLvl != gwServerTownLevelId;

			gnCurrentAct = nNewAct;
			gdwServerMapSeed = dwNewSeed;
			gwServerTownLevelId = wTownLvl;
			gnCurrentLevelId = wTownLvl;
			gbServerActLoaded = true;

			if (bNeedRebuild)
			{
				RebuildClientActWorld(gnCurrentAct, gdwServerMapSeed, gwServerTownLevelId);
			}
			++gdwInlinePacketsHandled;
		}
		break;
	}
	case 0x05: // UnloadComplete -> LeaveGame ack
	case 0x06: // GameExitSuccess
	case 0xB0: // Connection Terminated
	{
		if (nOpcode < 0xAF && sgptSCmdTable[nOpcode].pfHandler)
		{
			sgptSCmdTable[nOpcode].pfHandler(aBuffer);
			++gdwSCmdPacketsDispatched;
		}
		gbClientLeaveCompleted = 1;
		break;
	}
	case 0x07: // AddRoom (6 bytes): [0x07][tileX:u16][tileY:u16][levelId:u8]
	{
		if (nPacketLen >= 6)
		{
			const int32_t nTileX = *reinterpret_cast<const uint16_t*>(&aBuffer[1]);
			const int32_t nTileY = *reinterpret_cast<const uint16_t*>(&aBuffer[3]);
			const int32_t nLevelId = aBuffer[5];
			SafeStreamClientRoomAtTile(nTileX, nTileY, nLevelId);
			++gdwInlinePacketsHandled;
		}
		break;
	}
	case 0x0A: // RemoveObject (6 bytes): [0x0A][unitType][guid:u32]
	{
		if (nPacketLen >= 6 && sgptSCmdTable[0x0A].pfHandler)
		{
			const uint8_t nType = aBuffer[1];
			const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
			// Do not delete the local player unit if server sends visibility cull
			if (!(nType == UNIT_PLAYER && gpPlayerUnit && dwGuid == gpPlayerUnit->dwUnitId))
			{
				sgptSCmdTable[0x0A].pfHandler(aBuffer);
				++gdwSCmdPacketsDispatched;
			}
		}
		break;
	}
	case 0x0D: // PlayerStop (13 bytes): [0x0D][type][guid:u32][hitClass][x:u16][y:u16][unk][lifePct]
	{
		if (nPacketLen >= 13)
		{
			const uint8_t nType = aBuffer[1];
			const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
			const int32_t nCurX = *reinterpret_cast<const uint16_t*>(&aBuffer[7]);
			const int32_t nCurY = *reinterpret_cast<const uint16_t*>(&aBuffer[9]);
			if (nType == UNIT_PLAYER && gpPlayerUnit && dwGuid == gpPlayerUnit->dwUnitId && nCurX > 0 && nCurY > 0)
			{
				gnServerSubX = nCurX;
				gnServerSubY = nCurY;
				gnAuthServerSubX = nCurX;
				gnAuthServerSubY = nCurY;
				SubtileToWorldIso(nCurX, nCurY, &gfPlayerWorldX, &gfPlayerWorldY);
				gfTargetWorldX = gfPlayerWorldX;
				gfTargetWorldY = gfPlayerWorldY;
				gbPlayerMoving = false;
				g_vInteractivePath.clear();
				g_uInteractivePathIdx = 0;
				StreamClientNeighborhoodAroundTile(nCurX / 5, nCurY / 5, 24);
				D2Client_TeleportUnitToCoordinates(gpPlayerUnit, nCurX, nCurY, 0);
				UNITS_ChangeAnimMode(gpPlayerUnit, DUNGEON_IsTownLevelId(gnCurrentLevelId) ? PLRMODE_TOWNNEUTRAL : PLRMODE_NEUTRAL);
				if (sgptSCmdTable[0x0D].pfUnitHandler)
				{
					sgptSCmdTable[0x0D].pfUnitHandler(gpPlayerUnit, aBuffer);
					++gdwSCmdPacketsDispatched;
				}
				SyncReconstructedClientGlobals();
			}
		}
		break;
	}
	case 0x0F: // PlayerMove (16 bytes): [0x0F][type][guid:u32][moveType][tx:u16][ty:u16][0][cx:u16][cy:u16]
	{
		if (nPacketLen >= 16)
		{
			const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
			const int32_t nTargetX = *reinterpret_cast<const uint16_t*>(&aBuffer[7]);
			const int32_t nTargetY = *reinterpret_cast<const uint16_t*>(&aBuffer[9]);
			const int32_t nCurX = *reinterpret_cast<const uint16_t*>(&aBuffer[12]);
			const int32_t nCurY = *reinterpret_cast<const uint16_t*>(&aBuffer[14]);
			if (gpPlayerUnit && dwGuid == gpPlayerUnit->dwUnitId)
			{
				if (nCurX > 0 && nCurY > 0)
				{
					gnServerSubX = nCurX;
					gnServerSubY = nCurY;
					gnAuthServerSubX = nCurX;
					gnAuthServerSubY = nCurY;
					StreamClientNeighborhoodAroundTile(nCurX / 5, nCurY / 5, 24);
				}
				if (nTargetX > 0 && nTargetY > 0)
				{
					StreamClientNeighborhoodAroundTile(nTargetX / 5, nTargetY / 5, 24);
					SubtileToWorldIso(nTargetX, nTargetY, &gfTargetWorldX, &gfTargetWorldY);
					gbPlayerMoving = true;
				}
				if (sgptSCmdTable[0x0F].pfUnitHandler)
				{
					sgptSCmdTable[0x0F].pfUnitHandler(gpPlayerUnit, aBuffer);
				}
				SyncReconstructedClientGlobals();
			}
			++gdwSCmdPacketsDispatched;
		}
		break;
	}
	case 0x10: // PlayerToTarget (16 bytes): [0x10][type][guid:u32][moveType][targetType][targetGuid:u32][cx:u16][cy:u16]
	{
		if (nPacketLen >= 16)
		{
			const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
			const uint8_t nTargetType = aBuffer[7];
			const uint32_t dwTargetGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[8]);
			const int32_t nCurX = *reinterpret_cast<const uint16_t*>(&aBuffer[12]);
			const int32_t nCurY = *reinterpret_cast<const uint16_t*>(&aBuffer[14]);
			if (gpPlayerUnit && dwGuid == gpPlayerUnit->dwUnitId)
			{
				if (nCurX > 0 && nCurY > 0)
				{
					gnServerSubX = nCurX;
					gnServerSubY = nCurY;
					gnAuthServerSubX = nCurX;
					gnAuthServerSubY = nCurY;
					StreamClientNeighborhoodAroundTile(nCurX / 5, nCurY / 5, 24);
				}
				if (D2UnitStrc* pTarget = D2Client_FindServerSideUnit(dwTargetGuid, nTargetType))
				{
					const int32_t tx = UNITS_GetXPosition(pTarget);
					const int32_t ty = UNITS_GetYPosition(pTarget);
					if (tx > 0 && ty > 0)
					{
						SubtileToWorldIso(tx, ty, &gfTargetWorldX, &gfTargetWorldY);
						gbPlayerMoving = true;
					}
				}
				if (sgptSCmdTable[0x10].pfUnitHandler)
				{
					sgptSCmdTable[0x10].pfUnitHandler(gpPlayerUnit, aBuffer);
				}
				SyncReconstructedClientGlobals();
			}
			++gdwSCmdPacketsDispatched;
		}
		break;
	}
	case 0x15: // ReassignPlayer (11 bytes): [0x15][unitType][guid:u32][x:u16][y:u16][bFlash]
	{
		if (nPacketLen >= 11)
		{
			const uint8_t nUnitType = aBuffer[1];
			const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
			const int32_t nSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
			const int32_t nSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[8]);
			if (nUnitType == UNIT_PLAYER && nSubX > 0 && nSubY > 0)
			{
				StreamClientNeighborhoodAroundTile(nSubX / 5, nSubY / 5, 24);
				if (!gpPlayerUnit || dwGuid == gpPlayerUnit->dwUnitId || gdwClientPlayerGuid == 0)
				{
					gdwClientPlayerGuid = dwGuid;
					gnServerSubX = nSubX;
					gnServerSubY = nSubY;
					gnAuthServerSubX = nSubX;
					gnAuthServerSubY = nSubY;
					SubtileToWorldIso(nSubX, nSubY, &gfPlayerWorldX, &gfPlayerWorldY);
					gfTargetWorldX = gfPlayerWorldX;
					gfTargetWorldY = gfPlayerWorldY;
					gbPlayerMoving = false;
					if (gpPlayerUnit)
					{
						D2Client_TeleportUnitToCoordinates(gpPlayerUnit, nSubX, nSubY, aBuffer[10]);
					}
				}
			}
			if (sgptSCmdTable[0x15].pfHandler)
			{
				sgptSCmdTable[0x15].pfHandler(aBuffer);
				++gdwSCmdPacketsDispatched;
			}
			SyncReconstructedClientGlobals();
		}
		break;
	}
	case 0x27: // NpcInfo (40 bytes)
	{
		if (nPacketLen >= 40)
		{
			++gdwNpcIntrosReceived;
			if (sgptSCmdTable[0x27].pfHandler)
			{
				sgptSCmdTable[0x27].pfHandler(aBuffer);
				++gdwSCmdPacketsDispatched;
			}
		}
		break;
	}
	case 0x51: // AssignObject (14 bytes): [0x51][unitType][guid:u32][classId:u16][x:u16][y:u16][mode][interactType]
	{
		if (nPacketLen >= 14)
		{
			const uint8_t nType = aBuffer[1];
			const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
			const uint16_t wClassId = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
			const int32_t nSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[8]);
			const int32_t nSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[10]);
			const uint8_t nMode = aBuffer[12];
			const uint8_t nInteract = aBuffer[13];

			SafeStreamClientRoomAtTile(nSubX / 5, nSubY / 5);
			D2UnitStrc* pObj = D2Client_FindServerSideUnit(dwGuid, nType);
			if (!pObj)
			{
				pObj = D2Client_AllocClientObjectUnit(nType, wClassId, dwGuid, nSubX, nSubY, nMode, nInteract, 0);
			}
			else
			{
				UNITS_ChangeAnimMode(pObj, nMode);
				D2Client_TeleportUnitToCoordinates(pObj, nSubX, nSubY, 0);
			}
			if (pObj)
			{
				GFX_InitUnitCompositeGfx(pObj);
			}
			++gdwInlinePacketsHandled;
		}
		break;
	}
	case 0x59: // AssignPlayer (26 bytes): [0x59][guid:u32][classId:u8][szName:16][x:u16][y:u16]
	{
		if (nPacketLen >= 26)
		{
			const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
			const uint8_t nClassId = aBuffer[5];
			const int32_t nSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[22]);
			const int32_t nSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[24]);
			char szAssignedName[17] = {};
			std::memcpy(szAssignedName, &aBuffer[6], 16);

			const bool bIsLocal =
				(gdwClientPlayerGuid == 0) ||
				(dwGuid == gdwClientPlayerGuid) ||
				(szAssignedName[0] != '\0' && _stricmp(szAssignedName, gszCharName) == 0);

			if (nSubX > 0 && nSubY > 0)
			{
				StreamClientNeighborhoodAroundTile(nSubX / 5, nSubY / 5, 24);
			}

			if (bIsLocal)
			{
				gdwClientPlayerGuid = dwGuid;
				if (nClassId < 7)
				{
					gnCharClass = nClassId;
				}
				if (nSubX > 0 && nSubY > 0)
				{
					gnServerSubX = nSubX;
					gnServerSubY = nSubY;
					gnAuthServerSubX = nSubX;
					gnAuthServerSubY = nSubY;
					SubtileToWorldIso(nSubX, nSubY, &gfPlayerWorldX, &gfPlayerWorldY);
					gfTargetWorldX = gfPlayerWorldX;
					gfTargetWorldY = gfPlayerWorldY;
					gbPlayerMoving = false;
				}
				if (gpPlayerUnit && gpPlayerUnit->dwUnitId == 0x7FFFFFFFu && dwGuid != 0x7FFFFFFFu)
				{
					D2Client_RemoveServerSideUnit(UNIT_PLAYER, 0x7FFFFFFFu);
					D2Client_SetLocalPlayerUnit(nullptr);
				}
			}

			D2UnitStrc* pPlrUnit = D2Client_FindServerSideUnit(dwGuid, UNIT_PLAYER);
			if (!pPlrUnit)
			{
				pPlrUnit = D2Client_AllocClientPlayerUnit(
					dwGuid,
					nClassId < 7 ? nClassId : gnCharClass,
					nSubX,
					nSubY,
					szAssignedName[0] ? szAssignedName : gszCharName);
			}
			else if (nSubX > 0 && nSubY > 0)
			{
				D2Client_TeleportUnitToCoordinates(pPlrUnit, nSubX, nSubY, 0);
			}

			if (pPlrUnit)
			{
				if (bIsLocal)
				{
					D2Client_SetLocalPlayerUnit(pPlrUnit);
					SeedDefaultPlayerCharStats(pPlrUnit, gnCharClass);
				}
				GFX_InitUnitCompositeGfx(pPlrUnit);
			}
			SyncReconstructedClientGlobals();
			++gdwInlinePacketsHandled;
		}
		break;
	}
	case 0x67: // MonsterMove (16 bytes)
	case 0x68: // MonsterMoveToTarget (21 bytes)
	case 0x69: // MonsterState (12 bytes)
	case 0x6A: // MonsterUnk6A (12 bytes)
	case 0x6B: // MonsterAction (16 bytes)
	case 0x6C: // MonsterAttack (16 bytes)
	case 0x6D: // MonsterStop (10 bytes)
	{
		if (nPacketLen >= 10)
		{
			const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
			D2UnitStrc* pMon = D2Client_FindServerSideUnit(dwGuid, UNIT_MONSTER);
			if (pMon)
			{
				int32_t nSubX = 0;
				int32_t nSubY = 0;
				if (nOpcode == 0x67 || nOpcode == 0x68)
				{
					nSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
					nSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[8]);
				}
				else if (nOpcode == 0x69)
				{
					const uint8_t nState = aBuffer[5];
					nSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
					nSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[8]);
					if (nState == MONMODE_DEAD || nState == MONMODE_DEATH)
					{
						UNITS_ChangeAnimMode(pMon, MONMODE_DEAD);
					}
				}
				else if (nOpcode == 0x6C && nPacketLen >= 16)
				{
					nSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[12]);
					nSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[14]);
				}
				else if (nOpcode == 0x6D)
				{
					nSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[5]);
					nSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[7]);
					STATLIST_SetUnitStat(pMon, STAT_MAXHP, 128 << 8, 0);
					STATLIST_SetUnitStat(pMon, STAT_HITPOINTS, static_cast<int32_t>(aBuffer[9]) << 8, 0);
				}

				if (nSubX > 0 && nSubY > 0)
				{
					SafeStreamClientRoomAtTile(nSubX / 5, nSubY / 5);
					D2Client_TeleportUnitToCoordinates(pMon, nSubX, nSubY, 0);
				}
				if (sgptSCmdTable[nOpcode].pfUnitHandler)
				{
					sgptSCmdTable[nOpcode].pfUnitHandler(pMon, aBuffer);
				}
				++gdwSCmdPacketsDispatched;
			}
		}
		break;
	}
	case 0x95: // HPMPUpdate (13 bytes): 8b hdr + 15b hp + 15b mp + 15b stam + 16b x + 16b y + ...
	case 0x96: // WalkVerify (9 bytes):  8b hdr + 15b stam + 16b x + 16b y + ...
	{
		const int32_t nMinLen = (nOpcode == 0x95) ? 13 : 9;
		if (nPacketLen >= nMinLen)
		{
			if (sgptSCmdTable[nOpcode].pfHandler)
			{
				sgptSCmdTable[nOpcode].pfHandler(aBuffer);
				++gdwSCmdPacketsDispatched;
			}
			D2BitBufferStrc bitBuf = {};
			BITMANIP_Initialize(&bitBuf, aBuffer, nMinLen);
			BITMANIP_Read(&bitBuf, 8);
			if (nOpcode == 0x95)
			{
				BITMANIP_Read(&bitBuf, 15);
				BITMANIP_Read(&bitBuf, 15);
			}
			BITMANIP_Read(&bitBuf, 15);
			const int32_t nServerX = BITMANIP_Read(&bitBuf, 16);
			const int32_t nServerY = BITMANIP_Read(&bitBuf, 16);
			if (nServerX > 0 && nServerY > 0)
			{
				const int32_t nDriftX = std::abs(gnServerSubX - nServerX);
				const int32_t nDriftY = std::abs(gnServerSubY - nServerY);
				gnServerSubX = nServerX;
				gnServerSubY = nServerY;
				gnAuthServerSubX = nServerX;
				gnAuthServerSubY = nServerY;
				StreamClientNeighborhoodAroundTile(nServerX / 5, nServerY / 5, 24);
				if (gpPlayerUnit && (nDriftX > 4 || nDriftY > 4))
				{
					SubtileToWorldIso(nServerX, nServerY, &gfPlayerWorldX, &gfPlayerWorldY);
					D2Client_TeleportUnitToCoordinates(gpPlayerUnit, nServerX, nServerY, 0);
				}
				SyncReconstructedClientGlobals();
			}
		}
		break;
	}
	case 0x9C: // ItemActionWorld -> UNIT_Item.cpp
	{
		if (nPacketLen >= 8)
		{
			if (aBuffer[1] == 0x0B) // ITEM_ACTION_ADD_TO_SHOP
			{
				++gdwVendorItemsReceived;
			}
			D2Client_PacketHandler_ItemAction9C(aBuffer);
			++gdwSCmdPacketsDispatched;
			SyncReconstructedClientGlobals();
		}
		break;
	}
	case 0x9D: // ItemActionOwned -> UNIT_Item.cpp
	{
		if (nPacketLen >= 13)
		{
			if (aBuffer[1] == 0x0B)
			{
				++gdwVendorItemsReceived;
			}
			D2Client_PacketHandler_ItemAction9D(aBuffer);
			++gdwSCmdPacketsDispatched;
			SyncReconstructedClientGlobals();
		}
		break;
	}
	case 0xAC: // AssignNPC / Monster Spawn -> UNIT_CUnit.cpp
	{
		if (nPacketLen >= 13)
		{
			const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
			const uint16_t wClassId = *reinterpret_cast<const uint16_t*>(&aBuffer[5]);
			const int32_t nSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[7]);
			const int32_t nSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[9]);
			const uint8_t nLife = aBuffer[11];

			SafeStreamClientRoomAtTile(nSubX / 5, nSubY / 5);
			D2UnitStrc* pMon = D2Client_FindServerSideUnit(dwGuid, UNIT_MONSTER);
			if (!pMon)
			{
				D2MonsterSpawnDescStrc tDesc = {};
				tDesc.dwUnitGUID = dwGuid;
				tDesc.nClassId = wClassId;
				tDesc.nX = nSubX;
				tDesc.nY = nSubY;
				tDesc.nLifePct = nLife;
				tDesc.nAnimMode = (nLife == 0) ? MONMODE_DEAD : MONMODE_NEUTRAL;
				pMon = D2Client_AllocClientMonsterUnit(&tDesc, nullptr, nullptr, 0, 0, 0);
			}
			else
			{
				D2Client_TeleportUnitToCoordinates(pMon, nSubX, nSubY, 0);
			}
			if (pMon)
			{
				STATLIST_SetUnitStat(pMon, STAT_MAXHP, 128 << 8, 0);
				STATLIST_SetUnitStat(pMon, STAT_HITPOINTS, static_cast<int32_t>(nLife) << 8, 0);
				GFX_InitUnitCompositeGfx(pMon);
			}
			++gdwInlinePacketsHandled;
		}
		break;
	}
	case 0xAF: // D2GS TCP CompressionInfo Greeting
	{
		gbClientLeaveCompleted = 1;
		break;
	}
	default:
	{
		if (nOpcode < 0xAF && sgptSCmdTable[nOpcode].pfHandler)
		{
			sgptSCmdTable[nOpcode].pfHandler(aBuffer);
			++gdwSCmdPacketsDispatched;
			SyncReconstructedClientGlobals();
		}
		break;
	}
	}
}

static void DrainNetworkPackets()
{
	uint8_t aBuffer[2048] = {};
	for (int32_t nSafety = 0; nSafety < 512; ++nSafety)
	{
		int32_t nBytes = CLIENT_DequeueGamePacket(aBuffer, sizeof(aBuffer));
		if (nBytes <= 0)
		{
			nBytes = CLIENT_DequeueSystemPacket(aBuffer, sizeof(aBuffer));
		}
		if (nBytes <= 0)
		{
			break;
		}
		int32_t nOffset = 0;
		while (nOffset < nBytes)
		{
			const int32_t nSingleLen = GetServerPacketLength112a(&aBuffer[nOffset], nBytes - nOffset);
			if (nSingleLen <= 0)
			{
				break;
			}
			HandleServerPacket112a(&aBuffer[nOffset], nSingleLen);
			nOffset += nSingleLen;
		}
	}
}

// ============================================================================
// Join Game Packet & Server Thread
// ============================================================================
// 1.12a: 0x6FB294E0
void __fastcall D2Client_SendJoinGamePacket(const char* szCharName)
{
	char szConnectHost[128] = {};
	if (GetEnvironmentVariableA("D2MOO_CONNECT_HOST", szConnectHost, sizeof(szConnectHost)) > 0 && szConnectHost[0])
	{
		for (int32_t w = 0; w < 25 && !gbClientLeaveCompleted; ++w)
		{
			DrainNetworkPackets();
			if (gbClientLeaveCompleted)
			{
				break;
			}
			Sleep(20);
		}
		gbClientLeaveCompleted = 0;

		uint8_t aPkt68[37] = {};
		aPkt68[0] = 0x68u;
		*reinterpret_cast<uint32_t*>(&aPkt68[1]) = 1u;
		*reinterpret_cast<uint16_t*>(&aPkt68[5]) = 1u;
		aPkt68[7] = gnCharClass;
		*reinterpret_cast<uint32_t*>(&aPkt68[8]) = 0x0Cu;
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
	return 0;
}

// ============================================================================
// Slash Commands & Window Input Routing -> Reconstructed UI Modules
// ============================================================================
static void ExecuteChatOrSlashCommand(const char* szRawText)
{
	if (!szRawText || !szRawText[0])
	{
		return;
	}
	if (szRawText[0] == '!')
	{
		SendChatOrOverheadMessage(szRawText + 1, true);
		return;
	}
	if (szRawText[0] == '/')
	{
		++gdwSlashCommandsExecuted;
		if (_stricmp(szRawText, "/fps") == 0)
		{
			gbShowFpsOverlay = !gbShowFpsOverlay;
			return;
		}
		if (_strnicmp(szRawText, "/players", 8) == 0)
		{
			int nPlayers = std::atoi(szRawText + 8);
			if (nPlayers >= 1 && nPlayers <= 8)
			{
				SERVER_SetMaxClientsPerGame(nPlayers);
			}
			SendChatOrOverheadMessage(szRawText, false);
			return;
		}
		if (_stricmp(szRawText, "/nopickup") == 0)
		{
			SendChatOrOverheadMessage(szRawText, false);
			return;
		}
	}
	SendChatOrOverheadMessage(szRawText, false);
}

static bool HandleNpcDialogMouseDown(int32_t mx, int32_t my)
{
	if (!gaPanelStates[D2PANEL_NPCMENU] || gaPanelStates[D2PANEL_NPCSHOP])
	{
		return false;
	}
	D2UnitStrc* pNpc = g_pTradeNpcUnit;
	if (!pNpc && g_nInteractingNpcGUID != 0)
	{
		pNpc = D2Client_FindServerSideUnit(static_cast<uint32_t>(g_nInteractingNpcGUID), UNIT_MONSTER);
	}
	int32_t nBoxCenterX = (gnScreenWidth > 0 ? gnScreenWidth : 800) / 2;
	int32_t nBoxTopY = 120;
	if (pNpc)
	{
		int32_t nSx = 0, nSy = 0;
		if (WorldUnitToScreenPos(pNpc, &nSx, &nSy))
		{
			nBoxCenterX = std::max<int32_t>(120, std::min<int32_t>((gnScreenWidth > 0 ? gnScreenWidth : 800) - 120, nSx));
			nBoxTopY = (nSy > 190) ? (nSy - 165) : 40;
		}
	}
	const int32_t nCls = pNpc ? static_cast<int32_t>(pNpc->dwClassId) : g_nInteractingNpcClassId;
	const bool bHasTrade = (nCls == 148 || nCls == 154 || nCls == 147 || nCls == 150 || nCls == 155);
	const int32_t nOptCount = bHasTrade ? 3 : 2;
	const int32_t nBoxH = 44 + nOptCount * 22;

	if (mx < nBoxCenterX - 96 || mx > nBoxCenterX + 96 || my < nBoxTopY || my > nBoxTopY + nBoxH)
	{
		CloseActiveNpcSession();
		return true;
	}

	for (int32_t i = 0; i < nOptCount; ++i)
	{
		const int32_t nRowY = nBoxTopY + 46 + i * 22;
		if (my >= nRowY - 14 && my <= nRowY + 6)
		{
			const uint32_t dwNpcGuid = pNpc ? pNpc->dwUnitId : static_cast<uint32_t>(g_nInteractingNpcGUID);
			if (i == 0) // Talk / Gossip
			{
				SendTownFolkMenuSelect(0u, dwNpcGuid, 0u);
				UI_SetPanelState(D2PANEL_NPCMENU, 1, 0);
			}
			else if (i == 1 && bHasTrade)
			{
				if (nCls == 147) // Gamble
				{
					SendTownFolkMenuSelect(2u, dwNpcGuid, 0u);
				}
				else
				{
					SendTownFolkMenuSelect(1u, dwNpcGuid, 0u);
				}
				UI_SetPanelState(D2PANEL_NPCMENU, 1, 0);
				INV_SetPanelMode(1);
				UI_SetPanelState(D2PANEL_NPCSHOP, 0, 0);
			}
			else // Cancel
			{
				CloseActiveNpcSession();
			}
			return true;
		}
	}
	return true;
}

static bool HandleEscMenuMouseDown(int32_t mx, int32_t my)
{
	if (!gaPanelStates[D2PANEL_ESCMENU])
	{
		return false;
	}
	const int32_t nW = (gnScreenWidth > 0) ? gnScreenWidth : 800;
	const int32_t nH = (gnScreenHeight > 0) ? gnScreenHeight : 600;
	const int32_t nCenterX = nW / 2;
	const int32_t nBaseY = (nH / 2) - 60;
	for (int32_t i = 0; i < 3; ++i)
	{
		const int32_t nRowY = nBaseY + i * 46;
		if (my >= nRowY - 28 && my <= nRowY + 8 && mx >= nCenterX - 180 && mx <= nCenterX + 180)
		{
			if (i == 0)
			{
				gbSoundEnabled = !gbSoundEnabled;
			}
			else if (i == 1)
			{
				UI_SetPanelState(D2PANEL_ESCMENU, 1, 0);
				gbSaveAndExitRequested = 1;
			}
			else if (i == 2)
			{
				UI_SetPanelState(D2PANEL_ESCMENU, 1, 0);
			}
			return true;
		}
	}
	return true;
}

static bool HandleHudBottomBarMouseDown(int32_t mx, int32_t my)
{
	const int32_t nW = (gnScreenWidth > 0) ? gnScreenWidth : 800;
	const int32_t nH = (gnScreenHeight > 0) ? gnScreenHeight : 600;
	const int32_t nBarBaseX = (nW / 2) - 400;

	// Mini-panel pop-up buttons (Char, Inv, SkillTree, Automap, QuestLog, EscMenu)
	if (gaPanelStates[D2PANEL_MINIPANEL] != 0)
	{
		const int32_t nMiniX = (nW / 2) - 72;
		const int32_t nMiniY = nH - 72;
		if (mx >= nMiniX && mx < nMiniX + 144 && my >= nMiniY && my < nMiniY + 24)
		{
			const int32_t nBtn = (mx - nMiniX) / 24;
			switch (nBtn)
			{
			case 0: UI_SetPanelState(D2PANEL_CHAR, 2, 0); break;
			case 1: UI_SetPanelState(D2PANEL_INVENTORY, 2, 0); break;
			case 2: UI_SetPanelState(D2PANEL_SKILLTREE, 2, 0); break;
			case 3: UI_SetPanelState(D2PANEL_AUTOMAP, 2, 0); break;
			case 4: QUESTLOG_TogglePanel(0); UI_SetPanelState(D2PANEL_QUEST, gaPanelStates[D2PANEL_QUEST] ? 1 : 0, 0); break;
			case 5: UI_SetPanelState(D2PANEL_ESCMENU, 2, 0); break;
			default: break;
			}
			return true;
		}
	}

	if (my < nH - 48)
	{
		return false;
	}

	// Run/Walk button on bottom bar
	if (mx >= nBarBaseX + 252 && mx <= nBarBaseX + 276 && my >= nH - 40)
	{
		gbRunEnabled = !gbRunEnabled;
		SyncReconstructedClientGlobals();
		return true;
	}
	// Mini-panel toggle caret button
	if (mx >= (nW / 2) - 12 && mx <= (nW / 2) + 12 && my >= nH - 42)
	{
		UI_ToggleMiniPanel();
		return true;
	}
	// Belt slots 1..4 on bottom bar
	if (mx >= nBarBaseX + 422 && mx < nBarBaseX + 550 && gpPlayerUnit && gpPlayerUnit->pInventory)
	{
		const int32_t nCol = std::max<int32_t>(0, std::min<int32_t>(3, (mx - (nBarBaseX + 422)) / 32));
		if (D2UnitStrc* pBeltItem = INVENTORY_GetItemFromBeltSlot(gpPlayerUnit->pInventory, nCol))
		{
			SendUseBeltItem(pBeltItem->dwUnitId, (GetKeyState(VK_SHIFT) & 0x8000) != 0);
		}
		else
		{
			UI_SetPanelState(D2PANEL_BELT, 2, 0);
		}
		return true;
	}
	return true;
}

static bool ComputeWalkableSubtilePath(
	int32_t rawStartX,
	int32_t rawStartY,
	int32_t rawGoalX,
	int32_t rawGoalY,
	std::vector<POINT>& outWaypoints);

static LRESULT CALLBACK D2Client_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		gbWindowClosed = 1;
		gbExitApplication = 1;
		return 0;

	case WM_MOUSEMOVE:
	{
		// D2Win_MAINWINDOW_WndProc_6F8AD9B0 already scales physical client coords into 800x600 logical coords
		const int32_t logX = static_cast<int16_t>(LOWORD(lParam));
		const int32_t logY = static_cast<int16_t>(HIWORD(lParam));
		gMousePos.x = std::max<int32_t>(0, std::min<int32_t>(799, logX));
		gMousePos.y = std::max<int32_t>(0, std::min<int32_t>(599, logY));
		SyncReconstructedClientGlobals();
		return 0;
	}

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	{
		const int32_t logX = static_cast<int16_t>(LOWORD(lParam));
		const int32_t logY = static_cast<int16_t>(HIWORD(lParam));
		gMousePos.x = std::max<int32_t>(0, std::min<int32_t>(799, logX));
		gMousePos.y = std::max<int32_t>(0, std::min<int32_t>(599, logY));
		SyncReconstructedClientGlobals();

		const int32_t mx = gMousePos.x;
		const int32_t my = gMousePos.y;
		const bool bLeftClick = (uMsg == WM_LBUTTONDOWN);

		D2WinMsgStrc tMsg = {};
		tMsg.hWnd = hWnd;
		tMsg.nMessage = uMsg;
		tMsg.wParam = wParam;
		tMsg.nX = static_cast<uint16_t>(mx);
		tMsg.nY = static_cast<uint16_t>(my);

		if (bLeftClick && HandleEscMenuMouseDown(mx, my))
		{
			return 0;
		}
		if (bLeftClick && HandleNpcDialogMouseDown(mx, my))
		{
			return 0;
		}
		if (bLeftClick && HandleHudBottomBarMouseDown(mx, my))
		{
			return 0;
		}
		if (!bLeftClick && mx >= 418 && mx <= 550 && (my >= 552 || gaPanelStates[D2PANEL_BELT]))
		{
			PANEL_OnRButtonDown(reinterpret_cast<D2MsgStrc*>(&tMsg));
			if (tMsg.dwKeyFlags != 0 || tMsg.bHandled != 0)
			{
				return 0;
			}
		}

		// Left-side panels (0..400)
		if (mx < 400 && my < 552)
		{
			if (gaPanelStates[D2PANEL_CHAR])
			{
				CHARSTATS_HandleMouseDown(&tMsg);
				return 0;
			}
			if (gaPanelStates[D2PANEL_QUEST])
			{
				QUESTLOG_OnPanelMouseDown(&tMsg);
				return 0;
			}
			if (gaPanelStates[D2PANEL_WAYPOINT])
			{
				const int32_t nBaseX = 80;
				const int32_t nBaseY = 60;
				if (mx >= nBaseX + 260 && mx <= nBaseX + 308 && my >= nBaseY + 382 && my <= nBaseY + 424)
				{
					UI_SetPanelState(D2PANEL_WAYPOINT, 1, 0);
					return 0;
				}
				const int32_t nMaxActs = gbIsExpansion ? 5 : 4;
				if (my >= nBaseY + 10 && my <= nBaseY + 46 && mx >= nBaseX + 4 && mx < nBaseX + 4 + nMaxActs * 62)
				{
					g_nSelectedWaypointTab = std::max<int32_t>(0, std::min<int32_t>(nMaxActs - 1, (mx - (nBaseX + 4)) / 62));
					return 0;
				}
				static const uint32_t kActWpLevels[5][9] = {
					{ 1, 3, 4, 5, 6, 27, 29, 32, 35 },
					{ 40, 48, 42, 57, 43, 44, 52, 74, 46 },
					{ 75, 76, 77, 78, 79, 80, 81, 83, 101 },
					{ 103, 106, 107, 0, 0, 0, 0, 0, 0 },
					{ 109, 111, 112, 113, 115, 123, 117, 118, 129 }
				};
				static const int32_t kActWpCounts[5]  = { 9, 9, 9, 3, 9 };
				static const int32_t kActWpBitBase[5] = { 0, 9, 18, 27, 30 };
				const int32_t nActIdx = std::max<int32_t>(0, std::min<int32_t>(nMaxActs - 1, g_nSelectedWaypointTab));
				for (int32_t i = 0; i < kActWpCounts[nActIdx]; ++i)
				{
					const int32_t nRowY = nBaseY + 92 + i * 34;
					if (my >= nRowY - 6 && my <= nRowY + 22 && mx >= nBaseX + 40 && mx <= nBaseX + 280)
					{
						const int32_t nWpBit = kActWpBitBase[nActIdx] + i;
						const bool bUnlocked = ((g_qwUnlockedWaypointsMask >> nWpBit) & 1ULL) != 0ULL || (nActIdx == 0 && i == 0);
						if (bUnlocked)
						{
							SendWaypointSelect(g_dwActiveWaypointGuid, kActWpLevels[nActIdx][i]);
							UI_SetPanelState(D2PANEL_WAYPOINT, 1, 0);
						}
						return 0;
					}
				}
				return 0;
			}
			if (gaPanelStates[D2PANEL_NPCSHOP] || gaPanelStates[D2PANEL_STASH] ||
				gaPanelStates[D2PANEL_CUBE] || gaPanelStates[D2PANEL_MERCINV])
			{
				if (bLeftClick && gaPanelStates[D2PANEL_CUBE] && mx >= 218 && mx <= 278 && my >= 298 && my <= 356)
				{
					SendUIButtonAction(0x18u, 0u, 0u); // Horadric Cube Transmute (Panel\miniconvert at 224,352)
					++gdwTransmutesSent;
					return 0;
				}
				INV_OnInventoryPanelMouseDown(&tMsg);
				return 0;
			}
		}

		// Right-side panels (400..800)
		if (mx >= 400 && my < 552)
		{
			if (gaPanelStates[D2PANEL_INVENTORY])
			{
				INV_OnInventoryPanelMouseDown(&tMsg);
				return 0;
			}
			if (gaPanelStates[D2PANEL_SKILLTREE])
			{
				if (mx >= 708 && my >= 88 && my < 430)
				{
					g_nSkillTreeTab_6FBA79FC = std::max<int32_t>(0, std::min<int32_t>(2, (my - 88) / 110));
				}
				else if (mx >= 426 && mx <= 462 && my >= 442 && my <= 478)
				{
					UI_SetPanelState(D2PANEL_SKILLTREE, 1, 0);
				}
				return 0;
			}
		}

		// World viewport click -> unit interaction or collision-avoiding BFS walk / cast
		if (bLeftClick)
		{
			gbMouseLeftHeld = true;
			if (D2UnitStrc* pHovered = FindHoveredWorldUnitAtScreenPos(mx, my))
			{
				g_vInteractivePath.clear();
				g_uInteractivePathIdx = 0;
				if (pHovered->dwUnitType == UNIT_ITEM)
				{
					SendWalkOrRunToUnit(UNIT_ITEM, pHovered->dwUnitId);
					SendPickupGroundItem(pHovered->dwUnitId, false);
					return 0;
				}
				if (pHovered->dwUnitType == UNIT_OBJECT)
				{
					SendWalkOrRunToUnit(UNIT_OBJECT, pHovered->dwUnitId);
					SendUnitInteract(UNIT_OBJECT, pHovered->dwUnitId);
					return 0;
				}
				if (pHovered->dwUnitType == UNIT_MONSTER)
				{
					if (IsTownNpcMonster(pHovered))
					{
						SendWalkOrRunToUnit(UNIT_MONSTER, pHovered->dwUnitId);
						OpenNpcInteractionSession(pHovered);
					}
					else
					{
						UNITMODE_SendActionPacket(0x06u, 0, 0, UNIT_MONSTER, pHovered->dwUnitId);
					}
					return 0;
				}
			}

			int32_t nClickSubX = 0, nClickSubY = 0;
			ScreenPointToSubtile(mx, my, &nClickSubX, &nClickSubY);
			if (nClickSubX > 0 && nClickSubY > 0)
			{
				const int32_t nStartX = (gnServerSubX > 0) ? gnServerSubX : gnAuthServerSubX;
				const int32_t nStartY = (gnServerSubY > 0) ? gnServerSubY : gnAuthServerSubY;
				g_vInteractivePath.clear();
				g_uInteractivePathIdx = 0;
				int32_t nStepX = nClickSubX;
				int32_t nStepY = nClickSubY;
				if (nStartX > 0 && nStartY > 0 && ComputeWalkableSubtilePath(nStartX, nStartY, nClickSubX, nClickSubY, g_vInteractivePath) && !g_vInteractivePath.empty())
				{
					nStepX = g_vInteractivePath[0].x;
					nStepY = g_vInteractivePath[0].y;
				}
				StreamClientNeighborhoodAroundTile(nStepX / 5, nStepY / 5, 24);
				SubtileToWorldIso(nStepX, nStepY, &gfTargetWorldX, &gfTargetWorldY);
				gbPlayerMoving = true;
				SendWalkOrRunToLocation(static_cast<uint16_t>(nStepX), static_cast<uint16_t>(nStepY));
			}
		}
		else
		{
			int32_t nCastSubX = 0, nCastSubY = 0;
			ScreenPointToSubtile(mx, my, &nCastSubX, &nCastSubY);
			if (nCastSubX > 0 && nCastSubY > 0)
			{
				SendRightSkillLocation(static_cast<uint16_t>(nCastSubX), static_cast<uint16_t>(nCastSubY));
			}
		}
		return 0;
	}

	case WM_LBUTTONUP:
	{
		gbMouseLeftHeld = false;
		D2WinMsgStrc tMsg = {};
		tMsg.hWnd = hWnd;
		tMsg.nMessage = uMsg;
		tMsg.wParam = wParam;
		tMsg.nX = static_cast<uint16_t>(gMousePos.x);
		tMsg.nY = static_cast<uint16_t>(gMousePos.y);
		if (gaPanelStates[D2PANEL_CHAR])
		{
			CHARSTATS_HandleMouseUp(&tMsg);
		}
		if (gaPanelStates[D2PANEL_QUEST])
		{
			QUESTLOG_OnPanelMouseUp(&tMsg);
		}
		if (gaPanelStates[D2PANEL_INVENTORY])
		{
			INV_OnInventoryPanelMouseUp(&tMsg);
		}
		if (gaPanelStates[D2PANEL_STASH])
		{
			INV_OnStashPanelMouseUp(&tMsg);
		}
		if (gaPanelStates[D2PANEL_CUBE])
		{
			INV_OnHoradricCubeMouseUp(&tMsg);
		}
		if (gaPanelStates[D2PANEL_MERCINV])
		{
			INV_OnMercInventoryMouseUp(&tMsg);
		}
		return 0;
	}

	case WM_CHAR:
	{
		if (gbChatInputOpen)
		{
			const char ch = static_cast<char>(wParam);
			if (ch == '\r' || ch == '\n')
			{
				gszChatInputBuf[gnChatInputLen] = '\0';
				ExecuteChatOrSlashCommand(gszChatInputBuf);
				gnChatInputLen = 0;
				gszChatInputBuf[0] = '\0';
				gbChatInputOpen = false;
			}
			else if (ch == '\b')
			{
				if (gnChatInputLen > 0)
				{
					gszChatInputBuf[--gnChatInputLen] = '\0';
				}
			}
			else if (ch >= 32 && ch < 127 && gnChatInputLen < static_cast<int32_t>(sizeof(gszChatInputBuf)) - 1)
			{
				gszChatInputBuf[gnChatInputLen++] = ch;
				gszChatInputBuf[gnChatInputLen] = '\0';
			}
			return 0;
		}
		break;
	}

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		if (wParam == VK_MENU)
		{
			UI_SetPanelState(D2PANEL_SHOWITEMS, 0, 0);
			return 0;
		}
		if (gbChatInputOpen)
		{
			if (wParam == VK_ESCAPE)
			{
				gbChatInputOpen = false;
				gnChatInputLen = 0;
				gszChatInputBuf[0] = '\0';
			}
			return 0;
		}
		switch (wParam)
		{
		case VK_RETURN:
			gbChatInputOpen = true;
			gnChatInputLen = 0;
			gszChatInputBuf[0] = '\0';
			return 0;
		case VK_ESCAPE:
			if (gaPanelStates[D2PANEL_NPCMENU] || gaPanelStates[D2PANEL_NPCSHOP])
			{
				CloseActiveNpcSession();
			}
			else if (gnViewportShiftMode != 0 || gaPanelStates[D2PANEL_PARTY] || gaPanelStates[D2PANEL_BELT])
			{
				for (int32_t p = 1; p < D2PANEL_COUNT; ++p)
				{
					if (p != D2PANEL_AUTOMAP && p != D2PANEL_MINIPANEL && gaPanelStates[p] != 0)
					{
						UI_SetPanelState(p, 1, 0);
					}
				}
			}
			else
			{
				UI_SetPanelState(D2PANEL_ESCMENU, 2, 0);
			}
			SyncReconstructedClientGlobals();
			return 0;
		case 'I':
		case 'B':
			UI_SetPanelState(D2PANEL_INVENTORY, 2, 0);
			return 0;
		case 'C':
		case 'A':
			UI_SetPanelState(D2PANEL_CHAR, 2, 0);
			return 0;
		case 'T':
			UI_SetPanelState(D2PANEL_SKILLTREE, 2, 0);
			return 0;
		case 'Q':
			QUESTLOG_TogglePanel(0);
			UI_SetPanelState(D2PANEL_QUEST, gaPanelStates[D2PANEL_QUEST] ? 1 : 0, 0);
			return 0;
		case 'P':
			UI_SetPanelState(D2PANEL_PARTY, 2, 0);
			return 0;
		case 'O':
			UI_SetPanelState(D2PANEL_MERCINV, 2, 0);
			return 0;
		case 'R':
			gbRunEnabled = !gbRunEnabled;
			SyncReconstructedClientGlobals();
			return 0;
		case 'W':
			SendWeaponSwitch();
			return 0;
		case VK_OEM_3: // '~' Belt rows toggle
			UI_SetPanelState(D2PANEL_BELT, 2, 0);
			return 0;
		case '1':
		case '2':
		case '3':
		case '4':
			if (gpPlayerUnit && gpPlayerUnit->pInventory)
			{
				const int32_t nSlot = static_cast<int32_t>(wParam - '1');
				if (D2UnitStrc* pPot = INVENTORY_GetItemFromBeltSlot(gpPlayerUnit->pInventory, nSlot))
				{
					SendUseBeltItem(pPot->dwUnitId, (GetKeyState(VK_SHIFT) & 0x8000) != 0);
				}
			}
			return 0;
		case VK_TAB:
			UI_SetPanelState(D2PANEL_AUTOMAP, 2, 0);
			if (gaPanelStates[D2PANEL_AUTOMAP])
			{
				AUTOMAP_CenterView(1);
			}
			return 0;
		case VK_F11:
			gbScaled1200x900 = !gbScaled1200x900;
			if (HWND hMainWnd = WINDOW_GetWindow())
			{
				RECT rc = { 0, 0, gbScaled1200x900 ? 1200 : 800, gbScaled1200x900 ? 900 : 600 };
				AdjustWindowRectEx(&rc, GetWindowLongA(hMainWnd, GWL_STYLE), FALSE, GetWindowLongA(hMainWnd, GWL_EXSTYLE));
				SetWindowPos(hMainWnd, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
			return 0;
		default:
			break;
		}
		break;
	}

	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam == VK_MENU)
		{
			UI_SetPanelState(D2PANEL_SHOWITEMS, 1, 0);
			return 0;
		}
		break;

	default:
		break;
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

// ============================================================================
// Client Session Lifecycle (InitInGameAssets / ShutdownInGameAssets / Tick / Render)
// ============================================================================
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
	LoadCharacterSaveAppearance();
	gbIsExpansion = (FOG_IsExpansion() || (gwCharSaveFlags & 0x20u) != 0) ? 1 : 0;
	gnClientDifficulty = gpD2Config ? gpD2Config->nArenaDifficulty : 0;

	// Reset reconstructed session state & packet counters
	MSG_ResetCounters();
	MSG_ResetLastSentPacket();
	D2Client_ClearAllUnitHashTables();
	D2Client_SetLocalPlayerUnit(nullptr);
	GFXUTIL_InitDirectionTables();

	if (!g_pPlayerQuestRecord)
	{
		g_pPlayerQuestRecord = QUESTRECORD_AllocRecord(nullptr);
	}
	if (!g_pGameQuestRecord)
	{
		g_pGameQuestRecord = QUESTRECORD_AllocRecord(nullptr);
	}
	QUESTLOG_InitSession();

	// Set 800x600 resolution & palette for reconstructed 1.12a UI/World modules
	WINDOW_Resize(D2GAMERES_800x600, FALSE);
	D2Client_SyncResolutionMetrics(800, 600);
	D2Win_10014_SetWndProc(D2Client_WndProc);
	if (HWND hMainWnd = WINDOW_GetWindow())
	{
		DWORD dwStyle = GetWindowLongA(hMainWnd, GWL_STYLE);
		dwStyle |= (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
		SetWindowLongA(hMainWnd, GWL_STYLE, dwStyle);
		const int32_t nInitW = gbScaled1200x900 ? 1200 : 800;
		const int32_t nInitH = gbScaled1200x900 ? 900 : 600;
		RECT rcDesired = { 0, 0, nInitW, nInitH };
		AdjustWindowRectEx(&rcDesired, dwStyle, FALSE, GetWindowLongA(hMainWnd, GWL_EXSTYLE));
		const int32_t nOuterW = rcDesired.right - rcDesired.left;
		const int32_t nOuterH = rcDesired.bottom - rcDesired.top;
		const int32_t nScreenW = GetSystemMetrics(SM_CXSCREEN);
		const int32_t nScreenH = GetSystemMetrics(SM_CYSCREEN);
		SetWindowPos(
			hMainWnd,
			nullptr,
			std::max<int32_t>(0, (nScreenW - nOuterW) / 2),
			std::max<int32_t>(0, (nScreenH - nOuterH) / 2),
			nOuterW,
			nOuterH,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}
	D2Win_10177(0);

	if (!gpClientViewport)
	{
		gpClientViewport = VIEW_AllocViewport();
	}
	if (gpClientViewport)
	{
		VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), 0, 0, 800, 552);
	}

	std::memset(gaPanelStates, 0, sizeof(gaPanelStates));
	gnPanelCount = D2PANEL_COUNT;
	gnViewportShiftMode = 0;
	gnViewportShiftOffsetX = 0;
	g_nEquippedBeltRows = 1;
	g_bInteractingWithNpc = 0;
	g_nInteractingNpcGUID = 0;
	g_nInteractingNpcClassId = 0;
	g_pTradeNpcUnit = nullptr;
	gnCurrentAutomapLayerNo = -1;

	gdwClientPlayerGuid = 0;
	gnServerSubX = 0;
	gnServerSubY = 0;
	gnAuthServerSubX = 0;
	gnAuthServerSubY = 0;
	gbServerActLoaded = false;
	gnCurrentAct = 0;
	gdwServerMapSeed = (gpD2Config && gpD2Config->dwSeed != 0) ? gpD2Config->dwSeed : 0x112A2026u;
	gwServerTownLevelId = LEVEL_ROGUEENCAMPMENT;
	gnCurrentLevelId = LEVEL_ROGUEENCAMPMENT;

	gdwSCmdPacketsDispatched = 0;
	gdwInlinePacketsHandled = 0;
	gdwNpcIntrosReceived = 0;
	gdwVendorItemsReceived = 0;
	gdwAutotestVendorShopItems = 0;
	gdwTransmutesSent = 0;
	gdwSlashCommandsExecuted = 0;
	gdwAutomapRoomsPopulated = 0;

	INV_SyncExpansionModeFlag();
	PALSHIFT_InitTables();
	PALSHIFT_ResetAndPreloadDefaultMonsters();
	UI_InitAllPanels();

	// Pre-allocate initial Act 1 world and bootstrap player unit until S2C 0x03/0x59 arrive
	RebuildClientActWorld(0, gdwServerMapSeed, LEVEL_ROGUEENCAMPMENT);

	int32_t nBootSubX = 0;
	int32_t nBootSubY = 0;
	if (gpClientAct)
	{
		if (D2ActiveRoomStrc* pFirstRoom = DUNGEON_GetRoomFromAct(gpClientAct))
		{
			RECT rcFirst = {};
			DUNGEON_GetSubtileRect(pFirstRoom, &rcFirst);
			nBootSubX = (rcFirst.left + rcFirst.right) / 2;
			nBootSubY = (rcFirst.top + rcFirst.bottom) / 2;
		}
	}
	if (nBootSubX > 0 && nBootSubY > 0)
	{
		gnServerSubX = nBootSubX;
		gnServerSubY = nBootSubY;
		SubtileToWorldIso(nBootSubX, nBootSubY, &gfPlayerWorldX, &gfPlayerWorldY);
		gfTargetWorldX = gfPlayerWorldX;
		gfTargetWorldY = gfPlayerWorldY;

		D2UnitStrc* pBootPlr = D2Client_AllocClientPlayerUnit(0x7FFFFFFFu, gnCharClass, nBootSubX, nBootSubY, gszCharName);
		if (pBootPlr)
		{
			D2Client_SetLocalPlayerUnit(pBootPlr);
			SeedDefaultPlayerCharStats(pBootPlr, gnCharClass);
			GFX_InitUnitCompositeGfx(pBootPlr);
		}
	}
	SyncReconstructedClientGlobals();
}

static void ShutdownInGameAssets()
{
	CloseActiveNpcSession();
	UI_ReleaseAllAssets();
	D2Client_ClearItemInvCellCache();

	D2Client_FreeAllUnits();
	D2Client_SetLocalPlayerUnit(nullptr);
	D2Client_ClearAllUnitHashTables();
	GFX_FreeMonsterCofCache();
	GFX_FreePlayerCofCache();
	PALSHIFT_FreeTables();

	if (gpClientViewport)
	{
		VIEW_FreeViewport(static_cast<D2ViewportStrc*>(gpClientViewport));
		gpClientViewport = nullptr;
	}
	if (gpClientAct)
	{
		DUNGEON_FreeAct(gpClientAct);
		gpClientAct = nullptr;
	}
	if (g_pPlayerQuestRecord)
	{
		QUESTRECORD_FreeRecord(nullptr, g_pPlayerQuestRecord);
		g_pPlayerQuestRecord = nullptr;
	}
	if (g_pGameQuestRecord)
	{
		QUESTRECORD_FreeRecord(nullptr, g_pGameQuestRecord);
		g_pGameQuestRecord = nullptr;
	}
	SyncReconstructedClientGlobals();
}

static void StepReconstructedUnitTablesPerTick()
{
	SyncReconstructedClientGlobals();
	D2Client_UpdateAllUnitsPerTick();
	AUTOMAP_UpdateReveal();
}

static void UpdatePlayerMovement()
{
	if (gbMouseLeftHeld && gMousePos.y < 552 && !gaPanelStates[D2PANEL_ESCMENU] && !gaPanelStates[D2PANEL_NPCMENU])
	{
		const bool bOverLeftPanel = (gnViewportShiftMode == 2 || gnViewportShiftMode == 3) && gMousePos.x < 400;
		const bool bOverRightPanel = (gnViewportShiftMode == 1 || gnViewportShiftMode == 3) && gMousePos.x >= 400;
		if (!bOverLeftPanel && !bOverRightPanel)
		{
			int32_t nMoveSubX = 0, nMoveSubY = 0;
			ScreenPointToSubtile(gMousePos.x, gMousePos.y, &nMoveSubX, &nMoveSubY);
			if (nMoveSubX > 0 && nMoveSubY > 0 && g_vInteractivePath.size() <= 1)
			{
				SubtileToWorldIso(nMoveSubX, nMoveSubY, &gfTargetWorldX, &gfTargetWorldY);
				gbPlayerMoving = true;
				if ((gdwServerTicks % 4u) == 0u)
				{
					StreamClientNeighborhoodAroundTile(nMoveSubX / 5, nMoveSubY / 5, 24);
					SendWalkOrRunToLocation(static_cast<uint16_t>(nMoveSubX), static_cast<uint16_t>(nMoveSubY));
				}
			}
		}
	}

	if (!gbPlayerMoving)
	{
		return;
	}

	const float dx = gfTargetWorldX - gfPlayerWorldX;
	const float dy = gfTargetWorldY - gfPlayerWorldY;
	const float fDist = std::sqrt(dx * dx + dy * dy);
	const float fSpeed = gbRunEnabled ? 8.8f : 5.6f;

	float fNextWorldX = gfPlayerWorldX;
	float fNextWorldY = gfPlayerWorldY;
	bool bReachedTarget = false;
	if (fDist <= fSpeed)
	{
		fNextWorldX = gfTargetWorldX;
		fNextWorldY = gfTargetWorldY;
		bReachedTarget = true;
	}
	else
	{
		fNextWorldX += (dx / fDist) * fSpeed;
		fNextWorldY += (dy / fDist) * fSpeed;
	}

	int nCurSubX = static_cast<int>(fNextWorldX);
	int nCurSubY = static_cast<int>(fNextWorldY);
	DUNGEON_ClientToGameSubtileCoords(&nCurSubX, &nCurSubY);
	if (nCurSubX > 0 && nCurSubY > 0 && gpPlayerUnit)
	{
		D2ActiveRoomStrc* pCandRoom = SafeStreamClientRoomAtTile(nCurSubX / 5, nCurSubY / 5);
		const bool bBlocked = !pCandRoom ||
			COLLISION_CheckMask(pCandRoom, nCurSubX, nCurSubY, COLLIDE_WALL | COLLIDE_NOPLAYER | COLLIDE_OBJECT | COLLIDE_BLANK);
		if (bBlocked && (nCurSubX != gnServerSubX || nCurSubY != gnServerSubY))
		{
			gbPlayerMoving = false;
			g_vInteractivePath.clear();
			g_uInteractivePathIdx = 0;
			nCurSubX = gnServerSubX;
			nCurSubY = gnServerSubY;
		}
		else
		{
			gfPlayerWorldX = fNextWorldX;
			gfPlayerWorldY = fNextWorldY;
			if (bReachedTarget)
			{
				if (!g_vInteractivePath.empty() && g_uInteractivePathIdx + 1 < g_vInteractivePath.size())
				{
					++g_uInteractivePathIdx;
					const POINT ptNext = g_vInteractivePath[g_uInteractivePathIdx];
					StreamClientNeighborhoodAroundTile(ptNext.x / 5, ptNext.y / 5, 24);
					SubtileToWorldIso(ptNext.x, ptNext.y, &gfTargetWorldX, &gfTargetWorldY);
					gbPlayerMoving = true;
					SendWalkOrRunToLocation(static_cast<uint16_t>(ptNext.x), static_cast<uint16_t>(ptNext.y));
				}
				else
				{
					g_vInteractivePath.clear();
					g_uInteractivePathIdx = 0;
					gbPlayerMoving = false;
				}
			}
			const bool bTileChanged = (nCurSubX / 5 != gnServerSubX / 5) || (nCurSubY / 5 != gnServerSubY / 5);
			gnServerSubX = nCurSubX;
			gnServerSubY = nCurSubY;
			if (bTileChanged)
			{
				StreamClientNeighborhoodAroundTile(nCurSubX / 5, nCurSubY / 5, 24);
			}
			D2Client_TeleportUnitToCoordinates(gpPlayerUnit, nCurSubX, nCurSubY, 0);
		}
		const int32_t nIdleMode = DUNGEON_IsTownLevelId(gnCurrentLevelId) ? PLRMODE_TOWNNEUTRAL : PLRMODE_NEUTRAL;
		const int32_t nWalkMode = DUNGEON_IsTownLevelId(gnCurrentLevelId) ? PLRMODE_TOWNWALK : PLRMODE_WALK;
		UNITS_ChangeAnimMode(gpPlayerUnit, gbPlayerMoving ? (gbRunEnabled ? PLRMODE_RUN : nWalkMode) : nIdleMode);
		SyncReconstructedClientGlobals();
	}
}

static bool gbAutotestModeActive = false;

static void PollLiveHardwareCursorPosition()
{
	if (gbAutotestModeActive)
	{
		return;
	}
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
	SyncReconstructedClientGlobals();

	// 1. Reconstructed 1.12a Isometric World Renderer (View.cpp -> GAME_Wall2 / Floor / Env / ENGINE_Gfx)
	GAME_RenderFrame(0);

	// 2. Reconstructed 1.12a UI & HUD Renderer (UI_ui.cpp -> Automap, Orbs, ControlBar, Panels, Shop, Tooltips, Cursor)
	UI_RenderHUD();

	// 3. Optional /fps Telemetry Line & Active Chat Input Box
	if (gbShowFpsOverlay)
	{
		char szFps[128] = {};
		Unicode wszFps[128] = {};
		std::snprintf(
			szFps,
			sizeof(szFps),
			"FPS: 60  Ping: %ums  Seed: 0x%08X  Level: %u",
			gdwClientPing,
			gdwServerMapSeed,
			static_cast<unsigned int>(gnCurrentLevelId));
		Unicode::win2Unicode(wszFps, szFps, 127);
		D2Win_10127_SetFont(D2FONT_FONT16);
		D2Win_10117_DrawText(wszFps, 18, 22, 3, 0);
	}

	if (gbChatInputOpen)
	{
		char szPrompt[128] = {};
		Unicode wszPrompt[128] = {};
		std::snprintf(szPrompt, sizeof(szPrompt), "Say: %s_", gszChatInputBuf);
		Unicode::win2Unicode(wszPrompt, szPrompt, 127);
		D2GFX_DrawBox(24, 514, 480, 540, 0x00u, DRAWMODE_TRANS75);
		D2Win_10127_SetFont(D2FONT_FONT16);
		D2Win_10117_DrawText(wszPrompt, 32, 532, 0, 0);
	}

	D2GFX_EndScene();
}

// ============================================================================
// Native Client 10-Step Autotest & Honest Diagnostic Report
// ============================================================================
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
	if (hBmp) DeleteObject(hBmp);
	if (hMemDc) DeleteDC(hMemDc);
	ReleaseDC(hWnd, hWndDc);
	return bOk;
}

static D2UnitStrc* FindAutotestTownVendorNpc()
{
	D2UnitStrc* pBestPrimary = nullptr;
	int64_t nBestPrimaryDistSq = 0x7FFFFFFFFFFFFFFFLL;
	D2UnitStrc* pBestFallback = nullptr;
	int64_t nBestFallbackDistSq = 0x7FFFFFFFFFFFFFFFLL;
	for (int32_t b = 0; b < 128; ++b)
	{
		for (D2UnitStrc* pU = gapServerSideUnitHashTables[UNIT_MONSTER][b]; pU != nullptr; pU = pU->pListNext)
		{
			const uint32_t nCls = pU->dwClassId;
			if (nCls != 148 && nCls != 154 && nCls != 147) // Akara, Charsi, or Gheed
			{
				continue;
			}
			const int64_t dx = static_cast<int64_t>(UNITS_GetXPosition(pU)) - gnServerSubX;
			const int64_t dy = static_cast<int64_t>(UNITS_GetYPosition(pU)) - gnServerSubY;
			const int64_t d2 = dx * dx + dy * dy;
			if (nCls == 148 || nCls == 154)
			{
				if (d2 < nBestPrimaryDistSq)
				{
					nBestPrimaryDistSq = d2;
					pBestPrimary = pU;
				}
			}
			else if (d2 < nBestFallbackDistSq)
			{
				nBestFallbackDistSq = d2;
				pBestFallback = pU;
			}
		}
	}
	return pBestPrimary ? pBestPrimary : pBestFallback;
}

static bool IsSubtileWalkableForBfs(int32_t sx, int32_t sy, bool bRequireClearMargin)
{
	if (sx <= 0 || sy <= 0 || !gpClientAct)
	{
		return false;
	}
	D2ActiveRoomStrc* pRoom = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, sx, sy);
	if (!pRoom)
	{
		pRoom = SafeStreamClientRoomAtTile(sx / 5, sy / 5);
	}
	if (!pRoom || COLLISION_CheckMask(pRoom, sx, sy, COLLIDE_WALL | COLLIDE_NOPLAYER | COLLIDE_OBJECT | COLLIDE_DOOR | COLLIDE_BLANK))
	{
		return false;
	}
	if (bRequireClearMargin)
	{
		static const int32_t kOffs[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
		for (int32_t i = 0; i < 4; ++i)
		{
			const int32_t nx = sx + kOffs[i][0];
			const int32_t ny = sy + kOffs[i][1];
			D2ActiveRoomStrc* pAdj = DUNGEON_FindRoomBySubtileCoordinates(gpClientAct, nx, ny);
			if (!pAdj)
			{
				pAdj = SafeStreamClientRoomAtTile(nx / 5, ny / 5);
			}
			if (!pAdj || COLLISION_CheckMask(pAdj, nx, ny, COLLIDE_WALL | COLLIDE_NOPLAYER | COLLIDE_OBJECT | COLLIDE_BLANK))
			{
				return false;
			}
		}
	}
	return true;
}

static void FindNearestWalkableSubtile(int32_t nBaseX, int32_t nBaseY, int32_t nMaxRadius, int32_t* pOutX, int32_t* pOutY)
{
	*pOutX = nBaseX;
	*pOutY = nBaseY;
	for (int32_t pass = 0; pass < 2; ++pass)
	{
		const bool bMargin = (pass == 0);
		for (int32_t r = 0; r <= nMaxRadius; r += 1)
		{
			for (int32_t dy = -r; dy <= r; ++dy)
			{
				for (int32_t dx = -r; dx <= r; ++dx)
				{
					if (std::abs(dx) != r && std::abs(dy) != r)
					{
						continue;
					}
					const int32_t sx = nBaseX + dx;
					const int32_t sy = nBaseY + dy;
					if (IsSubtileWalkableForBfs(sx, sy, bMargin))
					{
						*pOutX = sx;
						*pOutY = sy;
						return;
					}
				}
			}
		}
	}
}

static bool ComputeWalkableSubtilePath(
	int32_t rawStartX,
	int32_t rawStartY,
	int32_t rawGoalX,
	int32_t rawGoalY,
	std::vector<POINT>& outWaypoints)
{
	outWaypoints.clear();
	if (!gpClientAct || rawStartX <= 0 || rawStartY <= 0 || rawGoalX <= 0 || rawGoalY <= 0)
	{
		return false;
	}

	PrestreamTownAndAdjacentWilderness(gwServerTownLevelId);
	StreamClientNeighborhoodAroundTile(rawStartX / 5, rawStartY / 5, 24);
	StreamClientNeighborhoodAroundTile(rawGoalX / 5, rawGoalY / 5, 28);

	int32_t startX = rawStartX, startY = rawStartY;
	int32_t goalX = rawGoalX, goalY = rawGoalY;
	FindNearestWalkableSubtile(rawStartX, rawStartY, 10, &startX, &startY);
	FindNearestWalkableSubtile(rawGoalX, rawGoalY, 10, &goalX, &goalY);

	const int32_t minX = (std::min(startX, goalX) - 180) & ~1;
	const int32_t maxX = (std::max(startX, goalX) + 180) & ~1;
	const int32_t minY = (std::min(startY, goalY) - 180) & ~1;
	const int32_t maxY = (std::max(startY, goalY) + 180) & ~1;

	const int32_t gridW = (maxX - minX) / 2 + 1;
	const int32_t gridH = (maxY - minY) / 2 + 1;
	if (gridW <= 2 || gridH <= 2 || gridW * gridH > 262144)
	{
		outWaypoints.push_back({ goalX, goalY });
		return true;
	}

	const int32_t sgx = (startX - minX) / 2;
	const int32_t sgy = (startY - minY) / 2;
	const int32_t ggx = (goalX - minX) / 2;
	const int32_t ggy = (goalY - minY) / 2;
	const int32_t startIdx = sgy * gridW + sgx;
	const int32_t goalIdx = ggy * gridW + ggx;

	static const int32_t kDirs[8][2] = {
		{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 },
		{ 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }
	};

	std::vector<int32_t> vParent;
	int32_t reachedIdx = -1;
	for (int32_t pass = 0; pass < 2 && reachedIdx < 0; ++pass)
	{
		const bool bMargin = (pass == 0);
		vParent.assign(static_cast<size_t>(gridW * gridH), -1);
		std::queue<int32_t> q;
		vParent[startIdx] = startIdx;
		q.push(startIdx);

		int32_t bestFallbackIdx = startIdx;
		int64_t bestFallbackDistSq = 0x7FFFFFFFFFFFFFFFLL;

		while (!q.empty())
		{
			const int32_t cur = q.front();
			q.pop();
			const int32_t cx = cur % gridW;
			const int32_t cy = cur / gridW;
			const int64_t ddx = static_cast<int64_t>(cx - ggx);
			const int64_t ddy = static_cast<int64_t>(cy - ggy);
			const int64_t d2 = ddx * ddx + ddy * ddy;
			if (d2 < bestFallbackDistSq)
			{
				bestFallbackDistSq = d2;
				bestFallbackIdx = cur;
			}
			if (d2 <= 1)
			{
				reachedIdx = cur;
				break;
			}
			const int32_t curSubX = minX + cx * 2;
			const int32_t curSubY = minY + cy * 2;
			for (int32_t d = 0; d < 8; ++d)
			{
				const int32_t nx = cx + kDirs[d][0];
				const int32_t ny = cy + kDirs[d][1];
				if (nx < 0 || nx >= gridW || ny < 0 || ny >= gridH)
				{
					continue;
				}
				const int32_t nIdx = ny * gridW + nx;
				if (vParent[nIdx] != -1)
				{
					continue;
				}
				const int32_t nextSubX = minX + nx * 2;
				const int32_t nextSubY = minY + ny * 2;
				const int32_t midSubX = curSubX + kDirs[d][0];
				const int32_t midSubY = curSubY + kDirs[d][1];
				if (!IsSubtileWalkableForBfs(midSubX, midSubY, false) ||
					!IsSubtileWalkableForBfs(nextSubX, nextSubY, bMargin))
				{
					continue;
				}
				vParent[nIdx] = cur;
				q.push(nIdx);
			}
		}
		if (reachedIdx < 0 && pass == 1 && bestFallbackDistSq <= 9)
		{
			reachedIdx = bestFallbackIdx;
		}
		(void)goalIdx;
	}

	if (reachedIdx < 0)
	{
		outWaypoints.push_back({ goalX, goalY });
		return false;
	}

	std::vector<POINT> vDense;
	int32_t trace = reachedIdx;
	for (int32_t guard = 0; guard < gridW * gridH; ++guard)
	{
		const int32_t tx = minX + (trace % gridW) * 2;
		const int32_t ty = minY + (trace / gridW) * 2;
		vDense.push_back({ tx, ty });
		if (trace == startIdx)
		{
			break;
		}
		trace = vParent[trace];
	}
	std::reverse(vDense.begin(), vDense.end());

	POINT ptLastEmitted = { startX, startY };
	for (size_t i = 1; i < vDense.size(); ++i)
	{
		const double dx = static_cast<double>(vDense[i].x - ptLastEmitted.x);
		const double dy = static_cast<double>(vDense[i].y - ptLastEmitted.y);
		if (std::sqrt(dx * dx + dy * dy) >= 7.0)
		{
			outWaypoints.push_back(vDense[i]);
			ptLastEmitted = vDense[i];
		}
	}
	if (outWaypoints.empty() || outWaypoints.back().x != goalX || outWaypoints.back().y != goalY)
	{
		outWaypoints.push_back({ goalX, goalY });
	}
	return true;
}

static bool IsCorridorClearOfWalls(int32_t bx, int32_t by, int32_t nx, int32_t ny, int32_t dMin, int32_t dMax)
{
	for (int32_t d = dMin; d <= dMax; d += 2)
	{
		const int32_t sx = bx + nx * d;
		const int32_t sy = by + ny * d;
		D2ActiveRoomStrc* pRoom = SafeStreamClientRoomAtTile(sx / 5, sy / 5);
		if (!pRoom || COLLISION_CheckMask(pRoom, sx, sy, COLLIDE_WALL))
		{
			return false;
		}
	}
	return true;
}

static bool ComputeAutotestBloodMoorBridgeRoute(POINT aOutWaypoints[4])
{
	if (!gpClientAct || !gpClientAct->pDrlg)
	{
		return false;
	}
	PrestreamTownAndAdjacentWilderness(LEVEL_ROGUEENCAMPMENT);

	D2DrlgLevelStrc* pTownLvl = DRLG_GetLevel(gpClientAct->pDrlg, LEVEL_ROGUEENCAMPMENT);
	if (!pTownLvl)
	{
		return false;
	}

	const int32_t nTownCenterX = (pTownLvl->nPosX + pTownLvl->nWidth / 2) * 5;
	const int32_t nTownCenterY = (pTownLvl->nPosY + pTownLvl->nHeight / 2) * 5;
	int32_t wp0X = nTownCenterX, wp0Y = nTownCenterY;
	FindNearestWalkableSubtile(nTownCenterX, nTownCenterY, 24, &wp0X, &wp0Y);
	aOutWaypoints[0] = { wp0X, wp0Y };

	int32_t nBestBridgeX = 0;
	int32_t nBestBridgeY = 0;
	int32_t nBestNormX = 0;
	int32_t nBestNormY = 0;
	int32_t nBestClearScore = -1;

	for (D2ActiveRoomStrc* pTownRoom = DUNGEON_GetRoomFromAct(gpClientAct);
		 pTownRoom != nullptr;
		 pTownRoom = pTownRoom->pRoomNext)
	{
		if (DUNGEON_GetLevelIdFromRoom(pTownRoom) != LEVEL_ROGUEENCAMPMENT)
		{
			continue;
		}
		RECT rcT = {};
		DUNGEON_GetSubtileRect(pTownRoom, &rcT);

		for (D2ActiveRoomStrc* pMoorRoom = DUNGEON_GetRoomFromAct(gpClientAct);
			 pMoorRoom != nullptr;
			 pMoorRoom = pMoorRoom->pRoomNext)
		{
			if (DUNGEON_GetLevelIdFromRoom(pMoorRoom) != LEVEL_BLOODMOOR)
			{
				continue;
			}
			RECT rcM = {};
			DUNGEON_GetSubtileRect(pMoorRoom, &rcM);

			int32_t nx = 0, ny = 0;
			int32_t segStart = 0, segEnd = 0, fixedCoord = 0;
			bool bVerticalEdge = false;

			if (rcT.right == rcM.left && std::min(rcT.bottom, rcM.bottom) > std::max(rcT.top, rcM.top))
			{
				nx = 1; ny = 0; bVerticalEdge = true; fixedCoord = rcT.right;
				segStart = std::max(rcT.top, rcM.top) + 2;
				segEnd = std::min(rcT.bottom, rcM.bottom) - 2;
			}
			else if (rcT.left == rcM.right && std::min(rcT.bottom, rcM.bottom) > std::max(rcT.top, rcM.top))
			{
				nx = -1; ny = 0; bVerticalEdge = true; fixedCoord = rcT.left;
				segStart = std::max(rcT.top, rcM.top) + 2;
				segEnd = std::min(rcT.bottom, rcM.bottom) - 2;
			}
			else if (rcT.bottom == rcM.top && std::min(rcT.right, rcM.right) > std::max(rcT.left, rcM.left))
			{
				nx = 0; ny = 1; bVerticalEdge = false; fixedCoord = rcT.bottom;
				segStart = std::max(rcT.left, rcM.left) + 2;
				segEnd = std::min(rcT.right, rcM.right) - 2;
			}
			else if (rcT.top == rcM.bottom && std::min(rcT.right, rcM.right) > std::max(rcT.left, rcM.left))
			{
				nx = 0; ny = -1; bVerticalEdge = false; fixedCoord = rcT.top;
				segStart = std::max(rcT.left, rcM.left) + 2;
				segEnd = std::min(rcT.right, rcM.right) - 2;
			}
			else
			{
				continue;
			}

			for (int32_t t = segStart; t <= segEnd; ++t)
			{
				const int32_t bx = bVerticalEdge ? fixedCoord : t;
				const int32_t by = bVerticalEdge ? t : fixedCoord;
				if (!IsCorridorClearOfWalls(bx, by, nx, ny, -16, 20))
				{
					continue;
				}
				const int32_t px = -ny;
				const int32_t py = nx;
				int32_t nClearScore = 1;
				for (int32_t lat = 1; lat <= 4; ++lat)
				{
					if (IsCorridorClearOfWalls(bx + px * lat, by + py * lat, nx, ny, -10, 12))
					{
						++nClearScore;
					}
					if (IsCorridorClearOfWalls(bx - px * lat, by - py * lat, nx, ny, -10, 12))
					{
						++nClearScore;
					}
				}
				if (nClearScore > nBestClearScore)
				{
					nBestClearScore = nClearScore;
					nBestBridgeX = bx;
					nBestBridgeY = by;
					nBestNormX = nx;
					nBestNormY = ny;
				}
			}
		}
	}

	if (nBestClearScore < 0)
	{
		return false;
	}

	int32_t wp1X = nBestBridgeX - nBestNormX * 28;
	int32_t wp1Y = nBestBridgeY - nBestNormY * 28;
	FindNearestWalkableSubtile(wp1X, wp1Y, 12, &wp1X, &wp1Y);

	const int32_t wp2X = nBestBridgeX - nBestNormX * 8;
	const int32_t wp2Y = nBestBridgeY - nBestNormY * 8;

	int32_t wp3X = nBestBridgeX + nBestNormX * 20;
	int32_t wp3Y = nBestBridgeY + nBestNormY * 20;
	StreamClientNeighborhoodAroundTile(wp3X / 5, wp3Y / 5, 28);
	FindNearestWalkableSubtile(wp3X, wp3Y, 10, &wp3X, &wp3Y);

	aOutWaypoints[1] = { wp1X, wp1Y };
	aOutWaypoints[2] = { wp2X, wp2Y };
	aOutWaypoints[3] = { wp3X, wp3Y };
	return true;
}

static void SendAutotestMoveToWaypoint(int32_t wpX, int32_t wpY)
{
	if (wpX <= 0 || wpY <= 0)
	{
		return;
	}
	gbRunEnabled = 1;
	StreamClientNeighborhoodAroundTile(wpX / 5, wpY / 5, 24);
	SubtileToWorldIso(wpX, wpY, &gfTargetWorldX, &gfTargetWorldY);
	gbPlayerMoving = true;
	MSG_ResetLastSentPacket();
	SendWalkOrRunToLocation(static_cast<uint16_t>(wpX), static_cast<uint16_t>(wpY));
}

static void CaptureAutotestFrameBmp(const char* szOutDir, const char* szFileName)
{
	RenderInGameFrame();
	if (HWND hWnd = WINDOW_GetWindow())
	{
		char szBmpPath[MAX_PATH * 2] = {};
		std::snprintf(szBmpPath, sizeof(szBmpPath), "%s\\%s", szOutDir, szFileName);
		CaptureWindowBmpToFile(hWnd, szBmpPath);
	}
}

static void SimulateClientLeftClick800x600(int32_t x800, int32_t y600)
{
	HWND hWnd = WINDOW_GetWindow();
	if (!hWnd)
	{
		return;
	}
	RECT rcClient = {};
	GetClientRect(hWnd, &rcClient);
	const int32_t nClientW = std::max<int32_t>(1, rcClient.right - rcClient.left);
	const int32_t nClientH = std::max<int32_t>(1, rcClient.bottom - rcClient.top);
	const int32_t rawX = (x800 * nClientW + 400) / 800;
	const int32_t rawY = (y600 * nClientH + 300) / 600;
	const LPARAM lParam = MAKELPARAM(rawX, rawY);
	SendMessageA(hWnd, WM_MOUSEMOVE, 0, lParam);
	SendMessageA(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
	SendMessageA(hWnd, WM_LBUTTONUP, 0, lParam);
}

static void WriteNativeAutotestReportJson(const char* szOutDir)
{
	int32_t nHashUnits = 0;
	int32_t nActiveNpcs = 0;
	int32_t nActiveMonsters = 0;
	for (int32_t t = 0; t < UNIT_TYPES_COUNT; ++t)
	{
		for (int32_t b = 0; b < 128; ++b)
		{
			for (D2UnitStrc* pU = gapServerSideUnitHashTables[t][b]; pU != nullptr; pU = pU->pListNext)
			{
				++nHashUnits;
				if (t == UNIT_MONSTER)
				{
					if (IsTownNpcMonster(pU)) ++nActiveNpcs;
					else ++nActiveMonsters;
				}
			}
		}
	}

	int32_t nEquippedItems = 0;
	int32_t nInventoryItems = 0;
	int32_t nBeltItems = 0;
	if (gpPlayerUnit && gpPlayerUnit->pInventory)
	{
		for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(gpPlayerUnit->pInventory);
			 pItem != nullptr;
			 pItem = INVENTORY_GetNextItem(pItem))
		{
			if (!INVENTORY_UnitIsItem(pItem))
			{
				continue;
			}
			if (pItem->dwAnimMode == IMODE_EQUIP)
			{
				++nEquippedItems;
			}
			else if (pItem->dwAnimMode == IMODE_INBELT)
			{
				++nBeltItems;
			}
			else if (pItem->dwAnimMode == IMODE_STORED)
			{
				++nInventoryItems;
			}
		}
	}

	int32_t nClientTownRoomsLoaded = 0;
	int32_t nClientBloodMoorRoomsLoaded = 0;
	if (gpClientAct)
	{
		for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct);
			 pRoom != nullptr;
			 pRoom = pRoom->pRoomNext)
		{
			const int32_t nRoomLvl = DUNGEON_GetLevelIdFromRoom(pRoom);
			if (nRoomLvl == LEVEL_ROGUEENCAMPMENT) ++nClientTownRoomsLoaded;
			else if (nRoomLvl == LEVEL_BLOODMOOR) ++nClientBloodMoorRoomsLoaded;
		}
	}

	const int32_t nCharLvl = gpPlayerUnit ? std::max<int32_t>(1, STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_LEVEL, 0)) : gnCharLevel;
	const int32_t nStatStr = gpPlayerUnit ? STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_STRENGTH, 0) : 0;
	const int32_t nStatDex = gpPlayerUnit ? STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_DEXTERITY, 0) : 0;
	const int32_t nStatVit = gpPlayerUnit ? STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_VITALITY, 0) : 0;
	const int32_t nStatEne = gpPlayerUnit ? STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_ENERGY, 0) : 0;
	const int32_t nStatHp  = gpPlayerUnit ? (STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_HITPOINTS, 0) >> 8) : 0;
	const int32_t nStatMp  = gpPlayerUnit ? (STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_MANA, 0) >> 8) : 0;
	const int32_t nGoldBank = gpPlayerUnit ? STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_GOLDBANK, 0) : 0;

	uint32_t dwBytesRecv = 0, dwPktsRecv = 0, dwSentBytes = 0, dwSentPkts = 0;
	MSG_GetTrafficCounters(&dwBytesRecv, &dwPktsRecv, &dwSentBytes, &dwSentPkts);

	bool bZeroOriginalDlls = true;
	bool bStorm112Loaded = false;
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
				if (_stricmp(me.szModule, "Storm112.dll") == 0)
				{
					bStorm112Loaded = true;
				}
				if (_stricmp(me.szModule, "D2Client.dll") == 0 ||
					_stricmp(me.szModule, "D2Common.dll") == 0 ||
					_stricmp(me.szModule, "D2Game.dll") == 0 ||
					_stricmp(me.szModule, "D2Win.dll") == 0 ||
					_stricmp(me.szModule, "D2gfx.dll") == 0 ||
					_stricmp(me.szModule, "D2Gdi.dll") == 0 ||
					_stricmp(me.szModule, "D2CMP.dll") == 0 ||
					_stricmp(me.szModule, "D2Lang.dll") == 0 ||
					_stricmp(me.szModule, "D2Net.dll") == 0 ||
					_stricmp(me.szModule, "D2sound.dll") == 0 ||
					_stricmp(me.szModule, "Fog.dll") == 0 ||
					_stricmp(me.szModule, "Storm.dll") == 0 ||
					_stricmp(me.szModule, "D2Launch.dll") == 0 ||
					_stricmp(me.szModule, "D2Multi.dll") == 0 ||
					_stricmp(me.szModule, "D2MCPClient.dll") == 0)
				{
					++nD2MooModules;
				}
			} while (Module32Next(hSnap, &me));
		}
		CloseHandle(hSnap);
	}

	char szJsonPath[MAX_PATH * 2] = {};
	std::snprintf(szJsonPath, sizeof(szJsonPath), "%s\\native_autotest_report.json", szOutDir);
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
			"  \"vendorShopItems\": %u,\n"
			"  \"stashGold\": %d,\n"
			"  \"horadricCubeTransmutes\": %u,\n"
			"  \"slashCommandsExecuted\": %u,\n"
			"  \"statStrength\": %d,\n"
			"  \"statDexterity\": %d,\n"
			"  \"statVitality\": %d,\n"
			"  \"statEnergy\": %d,\n"
			"  \"statHitpoints\": %d,\n"
			"  \"statMana\": %d,\n"
			"  \"serverMapSeed\": %u,\n"
			"  \"scmdPacketsDispatched\": %u,\n"
			"  \"inlineBootstrapPacketsHandled\": %u,\n"
			"  \"automapRoomsPopulated\": %u,\n"
			"  \"zeroOriginalBlizzardDlls\": %s,\n"
			"  \"storm112BackendLoaded\": %s,\n"
			"  \"d2mooCompiledModulesLoaded\": %d\n"
			"}\n",
			gszCharName,
			static_cast<unsigned int>(gnCharClass),
			nCharLvl,
			gdwServerTicks,
			gdwGamePacketsReceived,
			dwSentPkts,
			dwSentBytes,
			gdwClientPlayerGuid,
			(gpPlayerUnit != nullptr) ? "true" : "false",
			nHashUnits,
			gnServerSubX,
			gnServerSubY,
			static_cast<int32_t>(gnCurrentLevelId),
			nClientTownRoomsLoaded,
			nClientBloodMoorRoomsLoaded,
			nHashUnits,
			nActiveNpcs,
			nActiveMonsters,
			gdwNpcIntrosReceived,
			gdwVendorItemsReceived,
			nEquippedItems,
			nInventoryItems,
			nBeltItems,
			gdwAutotestVendorShopItems,
			nGoldBank,
			gdwTransmutesSent,
			gdwSlashCommandsExecuted,
			nStatStr,
			nStatDex,
			nStatVit,
			nStatEne,
			nStatHp,
			nStatMp,
			gdwServerMapSeed,
			gdwSCmdPacketsDispatched,
			gdwInlinePacketsHandled,
			gdwAutomapRoomsPopulated,
			bZeroOriginalDlls ? "true" : "false",
			bStorm112Loaded ? "true" : "false",
			nD2MooModules);
		std::fclose(pJson);
	}
}

static void RunNativeClientAutotestStep()
{
	static int32_t s_nCheckedEnv = 0;
	static bool s_bAutotestEnabled = false;
	static char s_szOutDir[MAX_PATH] = {};
	static int32_t s_nPhase = 0;
	static uint32_t s_dwPhaseTick = 0;
	static uint32_t s_dwStepSentTick = 0;
	static uint32_t s_dwTargetVendorGuid = 0;
	static std::vector<POINT> s_vActivePath;
	static size_t s_uPathIdx = 0;
	static int32_t s_nInteractSubStage = 0;
	static POINT s_aMoorRoute[4] = {};
	static bool s_bHasMoorRoute = false;

	if (!s_nCheckedEnv)
	{
		s_nCheckedEnv = 1;
		char szEnv[32] = {};
		const char* szCmdLine = GetCommandLineA();
		if ((GetEnvironmentVariableA("D2MOO_NATIVE_AUTOTEST", szEnv, sizeof(szEnv)) > 0 && szEnv[0] != '0') ||
			(szCmdLine && std::strstr(szCmdLine, "--autotest-native") != nullptr))
		{
			s_bAutotestEnabled = true;
			gbAutotestModeActive = true;
			if (GetEnvironmentVariableA("D2MOO_AUTOTEST_DIR", s_szOutDir, sizeof(s_szOutDir)) == 0 || s_szOutDir[0] == '\0')
			{
				GetModuleFileNameA(nullptr, s_szOutDir, sizeof(s_szOutDir));
				if (char* pSlash = std::strrchr(s_szOutDir, '\\'))
				{
					*pSlash = '\0';
				}
			}
			else
			{
				CreateDirectoryA(s_szOutDir, nullptr);
			}
		}
	}
	if (!s_bAutotestEnabled)
	{
		return;
	}

	switch (s_nPhase)
	{
	case 0: // Step 1: Rogue Encampment + Reconstructed HUD
	{
		if (gdwServerTicks >= 6u && gpPlayerUnit)
		{
			gbRunEnabled = 1;
			ExecuteChatOrSlashCommand("/fps");
			CaptureAutotestFrameBmp(s_szOutDir, "native_step1_hud.bmp");
			s_vActivePath.clear();
			s_uPathIdx = 0;
			s_nInteractSubStage = 0;
			if (D2UnitStrc* pVendor = FindAutotestTownVendorNpc())
			{
				s_dwTargetVendorGuid = pVendor->dwUnitId;
				int32_t nGoalX = UNITS_GetXPosition(pVendor);
				int32_t nGoalY = UNITS_GetYPosition(pVendor);
				FindNearestWalkableSubtile(nGoalX, nGoalY, 6, &nGoalX, &nGoalY);
				const int32_t nStartX = (gnAuthServerSubX > 0) ? gnAuthServerSubX : gnServerSubX;
				const int32_t nStartY = (gnAuthServerSubY > 0) ? gnAuthServerSubY : gnServerSubY;
				ComputeWalkableSubtilePath(nStartX, nStartY, nGoalX, nGoalY, s_vActivePath);
			}
			s_dwPhaseTick = gdwServerTicks;
			s_dwStepSentTick = 0;
			s_nPhase = 1;
		}
		break;
	}
	case 1: // Collision-free BFS navigation to town vendor -> C2S 0x04 -> C2S 0x13 -> S2C 0x27 NpcInfo
	{
		D2UnitStrc* pVendor = (s_dwTargetVendorGuid != 0)
			? D2Client_FindServerSideUnit(s_dwTargetVendorGuid, UNIT_MONSTER)
			: FindAutotestTownVendorNpc();
		if (pVendor)
		{
			if (s_dwTargetVendorGuid == 0)
			{
				s_dwTargetVendorGuid = pVendor->dwUnitId;
				int32_t nGoalX = UNITS_GetXPosition(pVendor);
				int32_t nGoalY = UNITS_GetYPosition(pVendor);
				FindNearestWalkableSubtile(nGoalX, nGoalY, 6, &nGoalX, &nGoalY);
				const int32_t nStartX = (gnAuthServerSubX > 0) ? gnAuthServerSubX : gnServerSubX;
				const int32_t nStartY = (gnAuthServerSubY > 0) ? gnAuthServerSubY : gnServerSubY;
				ComputeWalkableSubtilePath(nStartX, nStartY, nGoalX, nGoalY, s_vActivePath);
				s_uPathIdx = 0;
				s_dwStepSentTick = 0;
			}

			if (gdwNpcIntrosReceived > 0 || (gdwServerTicks - s_dwPhaseTick) >= 150u)
			{
				gbPlayerMoving = false;
				OpenNpcInteractionSession(pVendor);
				s_dwPhaseTick = gdwServerTicks;
				s_nPhase = 2;
				break;
			}

			if (s_uPathIdx < s_vActivePath.size())
			{
				if (s_dwStepSentTick == 0)
				{
					SendAutotestMoveToWaypoint(s_vActivePath[s_uPathIdx].x, s_vActivePath[s_uPathIdx].y);
					s_dwStepSentTick = gdwServerTicks;
				}
				else if ((gdwServerTicks - s_dwStepSentTick) >= 15u)
				{
					++s_uPathIdx;
					s_dwStepSentTick = 0;
				}
			}
			else
			{
				// Reached walkable tile adjacent to vendor: close final 1-2 subtiles via 0x04, then send clean 0x13
				if (s_nInteractSubStage == 0)
				{
					MSG_ResetLastSentPacket();
					SendWalkOrRunToUnit(UNIT_MONSTER, s_dwTargetVendorGuid);
					s_dwStepSentTick = gdwServerTicks;
					s_nInteractSubStage = 1;
				}
				else if (s_nInteractSubStage == 1 && (gdwServerTicks - s_dwStepSentTick) >= 6u)
				{
					gbPlayerMoving = false;
					MSG_ResetLastSentPacket();
					SendUnitInteract(UNIT_MONSTER, s_dwTargetVendorGuid);
					s_dwStepSentTick = gdwServerTicks;
					s_nInteractSubStage = 2;
				}
				else if (s_nInteractSubStage == 2 && (gdwServerTicks - s_dwStepSentTick) >= 8u)
				{
					MSG_ResetLastSentPacket();
					SendUnitInteract(UNIT_MONSTER, s_dwTargetVendorGuid);
					s_dwStepSentTick = gdwServerTicks;
				}
			}
		}
		else if ((gdwServerTicks - s_dwPhaseTick) >= 20u)
		{
			s_dwPhaseTick = gdwServerTicks;
			s_nPhase = 2;
		}
		break;
	}
	case 2: // Step 2: Capture NPC Dialog Menu, then request Trade inventory (C2S 0x38)
	{
		if ((gdwServerTicks - s_dwPhaseTick) >= 3u)
		{
			UI_SetPanelState(D2PANEL_NPCMENU, 0, 0);
			CaptureAutotestFrameBmp(s_szOutDir, "native_step2_npc_dialog.bmp");
			if (s_dwTargetVendorGuid != 0)
			{
				D2UnitStrc* pVendor = D2Client_FindServerSideUnit(s_dwTargetVendorGuid, UNIT_MONSTER);
				const uint32_t dwMenuAction = (pVendor && pVendor->dwClassId == 147) ? 2u : 1u;
				MSG_ResetLastSentPacket();
				SendTownFolkMenuSelect(dwMenuAction, s_dwTargetVendorGuid, 0u);
			}
			UI_SetPanelState(D2PANEL_NPCMENU, 1, 0);
			INV_SetPanelMode(1);
			UI_SetPanelState(D2PANEL_NPCSHOP, 0, 0);
			s_dwPhaseTick = gdwServerTicks;
			s_nPhase = 3;
		}
		break;
	}
	case 3: // Step 3: Wait for S2C 0x9C shop stream, count pVendor->pInventory items & capture Vendor Shop
	{
		if ((gdwVendorItemsReceived >= 15u && (gdwServerTicks - s_dwPhaseTick) >= 4u) ||
			(gdwServerTicks - s_dwPhaseTick) >= 20u)
		{
			D2UnitStrc* pVendor = g_pTradeNpcUnit;
			if (!pVendor && s_dwTargetVendorGuid != 0)
			{
				pVendor = D2Client_FindServerSideUnit(s_dwTargetVendorGuid, UNIT_MONSTER);
			}
			uint32_t dwShopCount = 0;
			if (pVendor && pVendor->pInventory)
			{
				for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pVendor->pInventory);
					 pItem != nullptr;
					 pItem = INVENTORY_GetNextItem(pItem))
				{
					if (INVENTORY_UnitIsItem(pItem))
					{
						++dwShopCount;
					}
				}
			}
			gdwAutotestVendorShopItems = dwShopCount;
			SimulateClientLeftClick800x600(120, 102); // Click Armor tab in NPC Shop via D2Client_WndProc
			CaptureAutotestFrameBmp(s_szOutDir, "native_step3_vendor_trade.bmp");
			CloseActiveNpcSession();
			s_dwPhaseTick = gdwServerTicks;
			s_nPhase = 4;
		}
		break;
	}
	case 4: // Step 4: Character Stats + Skill Tree
	{
		if ((gdwServerTicks - s_dwPhaseTick) >= 2u)
		{
			UI_SetPanelState(D2PANEL_CHAR, 0, 0);
			UI_SetPanelState(D2PANEL_SKILLTREE, 0, 0);
			CaptureAutotestFrameBmp(s_szOutDir, "native_step4_char_skill_inv.bmp");
			s_dwPhaseTick = gdwServerTicks;
			s_nPhase = 5;
		}
		break;
	}
	case 5: // Step 5: Quest Log + Inventory + Automap Overlay
	{
		if ((gdwServerTicks - s_dwPhaseTick) >= 2u)
		{
			UI_SetPanelState(D2PANEL_SKILLTREE, 1, 0);
			UI_SetPanelState(D2PANEL_CHAR, 1, 0);
			QUESTLOG_TogglePanel(0);
			UI_SetPanelState(D2PANEL_QUEST, 0, 0);
			UI_SetPanelState(D2PANEL_INVENTORY, 0, 0);
			UI_SetPanelState(D2PANEL_AUTOMAP, 0, 0);
			AUTOMAP_CenterView(1);
			CaptureAutotestFrameBmp(s_szOutDir, "native_step5_quest_automap.bmp");
			s_dwPhaseTick = gdwServerTicks;
			s_nPhase = 6;
		}
		break;
	}
	case 6: // Step 6: Stash + Belt Popup + Inventory
	{
		if ((gdwServerTicks - s_dwPhaseTick) >= 2u)
		{
			UI_SetPanelState(D2PANEL_AUTOMAP, 1, 0);
			QUESTLOG_ClosePanel(0);
			UI_SetPanelState(D2PANEL_QUEST, 1, 0);
			INV_SetPanelMode(11);
			UI_SetPanelState(D2PANEL_STASH, 0, 0);
			UI_SetPanelState(D2PANEL_INVENTORY, 0, 0);
			UI_SetPanelState(D2PANEL_BELT, 0, 0);
			CaptureAutotestFrameBmp(s_szOutDir, "native_step6_stash_belt.bmp");
			s_dwPhaseTick = gdwServerTicks;
			s_nPhase = 7;
		}
		break;
	}
	case 7: // Step 7: Horadric Cube + Interactive Transmute Button Click
	{
		if ((gdwServerTicks - s_dwPhaseTick) >= 2u)
		{
			UI_SetPanelState(D2PANEL_BELT, 1, 0);
			UI_SetPanelState(D2PANEL_STASH, 1, 0);
			INV_SetPanelMode(12);
			UI_SetPanelState(D2PANEL_CUBE, 0, 0);
			UI_SetPanelState(D2PANEL_INVENTORY, 0, 0);
			SimulateClientLeftClick800x600(248, 326); // Click Panel\miniconvert button via D2Client_WndProc
			CaptureAutotestFrameBmp(s_szOutDir, "native_step7_horadric_cube.bmp");
			s_dwPhaseTick = gdwServerTicks;
			s_nPhase = 8;
		}
		break;
	}
	case 8: // Step 8: Waypoint Panel
	{
		if ((gdwServerTicks - s_dwPhaseTick) >= 2u)
		{
			UI_SetPanelState(D2PANEL_CUBE, 1, 0);
			UI_SetPanelState(D2PANEL_WAYPOINT, 0, 0);
			CaptureAutotestFrameBmp(s_szOutDir, "native_step8_waypoint_menu.bmp");
			s_dwPhaseTick = gdwServerTicks;
			s_nPhase = 9;
		}
		break;
	}
	case 9: // Step 9: Mercenary Inventory Panel -> Close Panels & Compute Blood Moor BFS Route
	{
		if ((gdwServerTicks - s_dwPhaseTick) >= 2u)
		{
			UI_SetPanelState(D2PANEL_WAYPOINT, 1, 0);
			UI_SetPanelState(D2PANEL_MERCINV, 0, 0);
			UI_SetPanelState(D2PANEL_INVENTORY, 0, 0);
			CaptureAutotestFrameBmp(s_szOutDir, "native_step9_mercenary.bmp");

			for (int32_t p = 1; p < D2PANEL_COUNT; ++p)
			{
				if (gaPanelStates[p] != 0)
				{
					UI_SetPanelState(p, 1, 0);
				}
			}
			INV_SetPanelMode(0);

			s_vActivePath.clear();
			s_uPathIdx = 0;
			s_dwStepSentTick = 0;
			s_bHasMoorRoute = ComputeAutotestBloodMoorBridgeRoute(s_aMoorRoute);
			if (s_bHasMoorRoute)
			{
				const int32_t nStartX = (gnAuthServerSubX > 0) ? gnAuthServerSubX : gnServerSubX;
				const int32_t nStartY = (gnAuthServerSubY > 0) ? gnAuthServerSubY : gnServerSubY;
				ComputeWalkableSubtilePath(nStartX, nStartY, s_aMoorRoute[3].x, s_aMoorRoute[3].y, s_vActivePath);
			}
			s_dwPhaseTick = gdwServerTicks;
			s_nPhase = 10;
		}
		break;
	}
	case 10: // Step 10a: Run out of Rogue Encampment across the bridge into Blood Moor (Level 2)
	{
		if (s_bHasMoorRoute && !s_vActivePath.empty())
		{
			if (s_uPathIdx < s_vActivePath.size())
			{
				if (s_dwStepSentTick == 0)
				{
					SendAutotestMoveToWaypoint(s_vActivePath[s_uPathIdx].x, s_vActivePath[s_uPathIdx].y);
					s_dwStepSentTick = gdwServerTicks;
				}
				else if ((gdwServerTicks - s_dwStepSentTick) >= 15u)
				{
					++s_uPathIdx;
					s_dwStepSentTick = 0;
				}
			}
			else
			{
				const int32_t nCheckX = (gnAuthServerSubX > 0) ? gnAuthServerSubX : gnServerSubX;
				const int32_t nCheckY = (gnAuthServerSubY > 0) ? gnAuthServerSubY : gnServerSubY;
				D2ActiveRoomStrc* pAuthRoom = SafeStreamClientRoomAtTile(nCheckX / 5, nCheckY / 5);
				const int32_t nAuthLevel = pAuthRoom ? DUNGEON_GetLevelIdFromRoom(pAuthRoom) : 0;

				if ((gnCurrentLevelId == LEVEL_BLOODMOOR && nAuthLevel == LEVEL_BLOODMOOR) ||
					gdwServerTicks >= 490u)
				{
					s_dwPhaseTick = gdwServerTicks;
					s_nPhase = 11;
					break;
				}

				// Recompute short recovery path from latest server position if WalkVerify snapped behind
				ComputeWalkableSubtilePath(nCheckX, nCheckY, s_aMoorRoute[3].x, s_aMoorRoute[3].y, s_vActivePath);
				s_uPathIdx = 0;
				s_dwStepSentTick = 0;
			}

			if (gdwServerTicks >= 490u)
			{
				s_dwPhaseTick = gdwServerTicks;
				s_nPhase = 11;
			}
		}
		else
		{
			s_dwPhaseTick = gdwServerTicks;
			s_nPhase = 11;
		}
		break;
	}
	case 11: // Step 10b: Settle in Blood Moor, capture screenshot, write honest JSON report, and exit cleanly
	{
		if ((gdwServerTicks - s_dwPhaseTick) >= 5u)
		{
			gbPlayerMoving = false;
			if (gpPlayerUnit)
			{
				UNITS_ChangeAnimMode(gpPlayerUnit, DUNGEON_IsTownLevelId(gnCurrentLevelId) ? PLRMODE_TOWNNEUTRAL : PLRMODE_NEUTRAL);
			}
			SyncReconstructedClientGlobals();
			CaptureAutotestFrameBmp(s_szOutDir, "native_step10_blood_moor.bmp");
			WriteNativeAutotestReportJson(s_szOutDir);
			s_nPhase = 12;
			gbSaveAndExitRequested = 1;
			gbExitApplication = 1;
		}
		break;
	}
	default:
		break;
	}
}

// ============================================================================
// D2Client State Machine (States 2..6) & Main Entry
// ============================================================================
// 1.12a: 0x6FB2D2D0 (State 2: Join Session & Main In-Game Loop)
static int32_t __stdcall D2Client_State2_MainLoop()
{
	gbExitApplication = 0;
	gbWindowClosed = 0;
	gbSaveAndExitRequested = 0;
	gdwGamePacketsReceived = 0;
	gdwSCmdPacketsDispatched = 0;
	gdwInlinePacketsHandled = 0;
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
		bool bAdvancedTick = false;
		if (!bRemoteTcpMode && (gnGameType == 0 || gnGameType == 1))
		{
			GAME_ProcessNetworkMessages();
			if (GAME_UpdateGamesProgress(1))
			{
				GAME_UpdateClients(0, 0);
				++gdwServerTicks;
				bAdvancedTick = true;
			}
		}
		else if (dwNowMs - dwLastSimTickMs >= 40u)
		{
			dwLastSimTickMs = dwNowMs;
			++gdwServerTicks;
			bAdvancedTick = true;
		}

		DrainNetworkPackets();
		if (bAdvancedTick)
		{
			StepReconstructedUnitTablesPerTick();
			UpdatePlayerMovement();
			RunNativeClientAutotestStep();
		}

		RenderInGameFrame();
		Sleep(4);
	}

	SendLeaveGamePacket();
	if (!bRemoteTcpMode && (gnGameType == 0 || gnGameType == 1))
	{
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

		if (tWebProcInfo.hThread) CloseHandle(tWebProcInfo.hThread);
		if (tWebProcInfo.hProcess)
		{
			TerminateProcess(tWebProcInfo.hProcess, 0);
			CloseHandle(tWebProcInfo.hProcess);
		}
		if (hWebJob) CloseHandle(hWebJob);

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
