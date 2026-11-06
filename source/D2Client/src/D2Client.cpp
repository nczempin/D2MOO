#include "D2Client.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

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
#include <D2StatList.h>
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
static D2CompositeUnitStrc* gpPlayerCompIdle = nullptr;
static D2CompositeUnitStrc* gpPlayerCompWalk = nullptr;
static D2DrlgActStrc* gpClientAct = nullptr;

enum LeftPanelType : int32_t
{
	LEFTPANEL_NONE = 0,
	LEFTPANEL_CHAR = 1,
	LEFTPANEL_QUEST = 2,
	LEFTPANEL_PARTY = 3,
	LEFTPANEL_MSGLOG = 4,
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
static bool gbRunEnabled = true;
static bool gbSoundEnabled = true;
static bool gbScaled1200x900 = true;

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
};

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

// Authoritative C2S Packet Senders (1.12a Msg / CCmd)
static void SendWalkOrRunToLocation(int32_t nSubX, int32_t nSubY, bool bRun)
{
	uint8_t aPkt[5] = {};
	aPkt[0] = bRun ? 0x03u : 0x01u;
	*reinterpret_cast<uint16_t*>(&aPkt[1]) = static_cast<uint16_t>(nSubX);
	*reinterpret_cast<uint16_t*>(&aPkt[3]) = static_cast<uint16_t>(nSubY);
	CLIENT_Send(0, aPkt, sizeof(aPkt));
}

static void SendWalkOrRunToUnit(uint8_t nUnitType, uint32_t dwUnitGuid, bool bRun)
{
	uint8_t aPkt[9] = {};
	aPkt[0] = bRun ? 0x04u : 0x02u;
	*reinterpret_cast<uint32_t*>(&aPkt[1]) = static_cast<uint32_t>(nUnitType);
	*reinterpret_cast<uint32_t*>(&aPkt[5]) = dwUnitGuid;
	CLIENT_Send(0, aPkt, sizeof(aPkt));
}

static void SendUnitInteract(uint8_t nUnitType, uint32_t dwUnitGuid)
{
	uint8_t aPkt[9] = {};
	aPkt[0] = 0x13u;
	*reinterpret_cast<uint32_t*>(&aPkt[1]) = static_cast<uint32_t>(nUnitType);
	*reinterpret_cast<uint32_t*>(&aPkt[5]) = dwUnitGuid;
	CLIENT_Send(0, aPkt, sizeof(aPkt));
}

static void SendLeftSkillOnUnit(uint8_t nUnitType, uint32_t dwUnitGuid)
{
	uint8_t aPkt[9] = {};
	aPkt[0] = 0x06u;
	*reinterpret_cast<uint32_t*>(&aPkt[1]) = static_cast<uint32_t>(nUnitType);
	*reinterpret_cast<uint32_t*>(&aPkt[5]) = dwUnitGuid;
	CLIENT_Send(0, aPkt, sizeof(aPkt));
}

static void SendRightSkillOnLocation(int32_t nSubX, int32_t nSubY)
{
	uint8_t aPkt[5] = {};
	aPkt[0] = 0x0Cu;
	*reinterpret_cast<uint16_t*>(&aPkt[1]) = static_cast<uint16_t>(nSubX);
	*reinterpret_cast<uint16_t*>(&aPkt[3]) = static_cast<uint16_t>(nSubY);
	CLIENT_Send(0, aPkt, sizeof(aPkt));
}

static void SendRightSkillOnUnit(uint8_t nUnitType, uint32_t dwUnitGuid)
{
	uint8_t aPkt[9] = {};
	aPkt[0] = 0x0Du;
	*reinterpret_cast<uint32_t*>(&aPkt[1]) = static_cast<uint32_t>(nUnitType);
	*reinterpret_cast<uint32_t*>(&aPkt[5]) = dwUnitGuid;
	CLIENT_Send(0, aPkt, sizeof(aPkt));
}

static void SendPickupGroundItem(uint32_t dwItemGuid)
{
	uint8_t aPkt[13] = {};
	aPkt[0] = 0x16u;
	*reinterpret_cast<uint32_t*>(&aPkt[1]) = UNIT_ITEM;
	*reinterpret_cast<uint32_t*>(&aPkt[5]) = dwItemGuid;
	*reinterpret_cast<uint32_t*>(&aPkt[9]) = 0u; // 0 = directly into inventory
	CLIENT_Send(0, aPkt, sizeof(aPkt));
}

static void SendIncrementStat(uint16_t wStatId)
{
	uint8_t aPkt[3] = {};
	aPkt[0] = 0x3Au;
	*reinterpret_cast<uint16_t*>(&aPkt[1]) = wStatId;
	CLIENT_Send(0, aPkt, sizeof(aPkt));
}

static void SendIncrementSkill(uint16_t wSkillId)
{
	uint8_t aPkt[3] = {};
	aPkt[0] = 0x3Bu;
	*reinterpret_cast<uint16_t*>(&aPkt[1]) = wSkillId;
	CLIENT_Send(0, aPkt, sizeof(aPkt));
}

static void SendSelectSkill(uint16_t wSkillId, bool bLeftHand)
{
	uint8_t aPkt[9] = {};
	aPkt[0] = 0x3Cu;
	*reinterpret_cast<uint16_t*>(&aPkt[1]) = wSkillId;
	aPkt[3] = 0u;
	aPkt[4] = bLeftHand ? 0x80u : 0x00u;
	*reinterpret_cast<uint32_t*>(&aPkt[5]) = 0xFFFFFFFFu;
	CLIENT_Send(0, aPkt, sizeof(aPkt));
}

static void SendUseBeltItem(uint32_t dwItemGuid)
{
	uint8_t aPkt[13] = {};
	aPkt[0] = 0x26u;
	*reinterpret_cast<uint32_t*>(&aPkt[1]) = dwItemGuid;
	*reinterpret_cast<uint32_t*>(&aPkt[5]) = 0u;
	*reinterpret_cast<uint32_t*>(&aPkt[9]) = 0u;
	CLIENT_Send(0, aPkt, sizeof(aPkt));
}

static void SendLeaveGamePacket()
{
	const uint8_t pktLeave = 0x69u;
	CLIENT_Send(0, &pktLeave, 1);
}

static const char* const kClassNames[7] = {
	"Amazon", "Sorceress", "Necromancer", "Paladin", "Barbarian", "Druid", "Assassin"
};

static const uint32_t kClassTokens[7] = {
	'  MA', '  OS', '  EN', '  AP', '  AB', '  ZD', '  IA'
};
static const uint32_t kModeTokens[20] = {
	'  TD', '  UN', '  LW', '  NR', '  HG', '  NT', '  WT', '  1A',
	'  2A', '  LB', '  CS', '  HT', '  KK', '  1S', '  2S', '  3S',
	'  4S', '  DD', '  HG', '  HG'
};
static const uint32_t kCompTokens[16] = {
	'  DH', '  RT', '  GL', '  AR', '  AL', '  HR', '  HL', '  HS',
	'  1S', '  2S', '  3S', '  4S', '  5S', '  6S', '  7S', '  8S'
};
static const uint32_t kWClassTokens[15] = {
	0, ' hth', ' th1', ' th2', ' sh1', ' sh2', ' wob', ' wbx',
	' fts', ' sj1', ' tj1', ' ss1', ' ts1', ' 1th', ' 2th'
};
static const uint8_t kDirRemapTable[7][64] = {
	{ 0 },
	{ 0 },
	{ 0, 32 },
	{ 0, 16, 32, 48 },
	{ 0, 8, 16, 24, 32, 40, 48, 56 },
	{ 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60 },
	{ 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62 },
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
			SendLeftSkillOnUnit(UNIT_MONSTER, u.dwUnitGuid);
			D2SOUND_FastPlaySound("data\\global\\sfx\\combat\\weapon\\swing.wav");
		}
		else
		{
			SendWalkOrRunToUnit(u.nUnitType, u.dwUnitGuid, gbRunEnabled);
			SendUnitInteract(u.nUnitType, u.dwUnitGuid);
			if (u.nUnitType == UNIT_MONSTER && u.bIsNpc)
			{
				char szInteractMsg[128] = {};
				std::snprintf(szInteractMsg, sizeof(szInteractMsg), "%s: Greetings, %s! Your life and mana have been restored.", u.szName, gszCharName);
				AppendClientChatLog(szInteractMsg);
			}
		}
		return;
	}

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
	if (gbEscMenuOpen || nScreenY >= 524)
	{
		return;
	}
	UpdateHoveredWorldTarget(nScreenX, nScreenY);
	if (gnHoveredUnitIdx >= 0)
	{
		const ClientWorldUnit& u = gClientUnits[gnHoveredUnitIdx];
		SendRightSkillOnUnit(u.nUnitType, u.dwUnitGuid);
	}
	else
	{
		int32_t nSubX = 0;
		int32_t nSubY = 0;
		ScreenPointToSubtile(nScreenX, nScreenY, &nSubX, &nSubY);
		if (nSubX > 0 && nSubY > 0)
		{
			SendRightSkillOnLocation(nSubX, nSubY);
		}
	}
	D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
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

	// 4b. Belt Potion Slots on HUD (x=422..546, y=560..594)
	if (nX >= 422 && nX < 546 && nY >= 560 && nY <= 594)
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
	}

	// 5. Open Left Panel interactions (x=80..400, y=60..492)
	if (gnLeftPanel != LEFTPANEL_NONE && nX >= 80 && nX < 400 && nY >= 60 && nY < 492)
	{
		// Close 'X' button (checked at bottom-left socket, bottom-center, and bottom-right of Left Panel)
		if ((nX >= 124 && nX <= 168 && nY >= 444 && nY <= 484) ||
			(nX >= 204 && nX <= 244 && nY >= 444 && nY <= 484) ||
			(nX >= 350 && nX <= 392 && nY >= 444 && nY <= 484))
		{
			gnLeftPanel = LEFTPANEL_NONE;
			D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
			return true;
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
		// Quest Log 6 quest icon sockets (3 columns x 2 rows)
		if (gnLeftPanel == LEFTPANEL_QUEST)
		{
			for (int32_t q = 0; q < 6; ++q)
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

	return false;
}

// 1.12a: 0x6FB1D200
static LRESULT __stdcall D2Client_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
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
	case WM_KEYDOWN:
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
		else if (wParam == VK_TAB)
		{
			gbAutomapOpen = !gbAutomapOpen;
		}
		else if (wParam == 'R')
		{
			gbRunEnabled = !gbRunEnabled;
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

	// Actions that remove an item from a container/belt/body
	if (nAction == 0x01 || nAction == 0x05 || nAction == 0x08 || nAction == 0x0F || nAction == 0x12)
	{
		if (nIdx >= 0)
		{
			gClientItems[nIdx].bActive = false;
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

	if (nAction == 0x00 || nAction == 0x02 || nAction == 0x03 || tSave.nAnimMode == IMODE_ONGROUND || tSave.nAnimMode == IMODE_DROPPING)
	{
		it.nLocation = CLI_ITEMLOC_GROUND;
	}
	else if (nAction == 0x06 || nAction == 0x07 || nAction == 0x09 || tSave.nAnimMode == IMODE_EQUIP)
	{
		it.nLocation = CLI_ITEMLOC_EQUIPPED;
	}
	else if (nAction == 0x0E || nAction == 0x10 || tSave.nAnimMode == IMODE_INBELT)
	{
		it.nLocation = CLI_ITEMLOC_BELT;
	}
	else
	{
		it.nLocation = CLI_ITEMLOC_CONTAINER;
	}

	D2ItemsTxt* pItemsTxt = DATATBLS_GetItemsTxtRecord(tSave.nClassId);
	if (pItemsTxt)
	{
		it.nInvW = std::max<uint8_t>(1, pItemsTxt->nInvWidth);
		it.nInvH = std::max<uint8_t>(1, pItemsTxt->nInvHeight);
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
		case 0x01: // GameLogonReceipt
			if (nPacketLen >= 8)
			{
				gnGameDifficulty = aBuffer[1];
				gbIsExpansion = (aBuffer[6] != 0);
			}
			break;

		case 0x02: // LoadDone -> send C2S 0x6B (JoinActComplete)
		{
			const uint8_t pktJoinAct = 0x6Bu;
			CLIENT_Send(0, &pktJoinAct, 1);
			break;
		}

		case 0x03: // LoadAct (nAct, dwMapSeed, wTownLevelId, dwUnk)
			if (nPacketLen >= 12)
			{
				const uint8_t nAct = aBuffer[1];
				const uint32_t dwMapSeed = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				const uint16_t wLevelId = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
				if (!gbServerActLoaded || gnCurrentAct != nAct || gdwServerMapSeed != dwMapSeed || !gpClientAct)
				{
					if (gpClientAct)
					{
						DUNGEON_FreeAct(gpClientAct);
						gpClientAct = nullptr;
					}
					gnCurrentAct = nAct;
					gdwServerMapSeed = dwMapSeed;
					gwServerTownLevelId = wLevelId ? wLevelId : LEVEL_ROGUEENCAMPMENT;
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
					gbServerActLoaded = true;
				}
			}
			break;

		case 0x07: // AddRoom (wTileX, wTileY, nLevelId)
			if (nPacketLen >= 6 && gpClientAct)
			{
				const int16_t nTileX = *reinterpret_cast<const int16_t*>(&aBuffer[1]);
				const int16_t nTileY = *reinterpret_cast<const int16_t*>(&aBuffer[3]);
				DUNGEON_StreamRoomAtCoords(gpClientAct, nTileX, nTileY);
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
				if (ClientWorldUnit* pObj = FindOrAllocClientUnit(nUnitType, dwGuid))
				{
					pObj->nAnimMode = nMode;
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
				}
				else if (ClientWorldUnit* pUnit = FindOrAllocClientUnit(nUnitType, dwGuid))
				{
					SetClientUnitSubtilePos(pUnit, wSubX, wSubY, bSnap);
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
				}
			}
			break;

		case 0x19: // GoldByteAdd
			if (nPacketLen >= 2)
			{
				gaClientStats[STAT_GOLD] += aBuffer[1];
			}
			break;
		case 0x1A: // ExpByteAdd
			if (nPacketLen >= 2)
			{
				gaClientStats[STAT_EXPERIENCE] += aBuffer[1];
			}
			break;
		case 0x1B: // ExpWordAdd
			if (nPacketLen >= 3)
			{
				gaClientStats[STAT_EXPERIENCE] += *reinterpret_cast<const uint16_t*>(&aBuffer[1]);
			}
			break;
		case 0x1C: // ExpDwordSet
			if (nPacketLen >= 5)
			{
				gaClientStats[STAT_EXPERIENCE] = *reinterpret_cast<const int32_t*>(&aBuffer[1]);
			}
			break;
		case 0x1D: // StatByteSet
		case 0x1E: // StatWordSet
		case 0x1F: // StatDwordSet
		{
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

		case 0x51: // AssignObject (nUnitType, dwGUID, wObjectId, wX, wY, nAnimMode, nInteractType)
			if (nPacketLen >= 14)
			{
				const uint8_t nUnitType = aBuffer[1];
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[2]);
				const uint16_t wObjId = *reinterpret_cast<const uint16_t*>(&aBuffer[6]);
				const uint16_t wSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[8]);
				const uint16_t wSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[10]);
				const uint8_t nMode = aBuffer[12];
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

		case 0x59: // AssignPlayer (dwGUID, nCharClass, szName[16], wX, wY)
			if (nPacketLen >= 26)
			{
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
				const uint8_t nClass = aBuffer[5];
				const uint16_t wSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[22]);
				const uint16_t wSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[24]);
				if (gdwClientPlayerGuid == 0 || dwGuid == gdwClientPlayerGuid)
				{
					gdwClientPlayerGuid = dwGuid;
					if (nClass < 7)
					{
						gnCharClass = nClass;
					}
					if (aBuffer[6])
					{
						SStrCopy(gszCharName, reinterpret_cast<const char*>(&aBuffer[6]), sizeof(gszCharName));
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
				if (ClientWorldUnit* pMon = FindOrAllocClientUnit(UNIT_MONSTER, dwGuid))
				{
					pMon->nAnimMode = (nLifePct == 0) ? MONMODE_DEAD : MONMODE_NEUTRAL;
					pMon->nLifePct = nLifePct;
					SetClientUnitSubtilePos(pMon, wX, wY, true);
				}
			}
			break;

		case 0x9C: // ItemActionWorld
			HandleServerItemActionPacket(aBuffer, nPacketLen, false);
			break;

		case 0x9D: // ItemActionOwned
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

		case 0xAC: // AssignNPC (dwGUID, wClassId, wX, wY, nLifePct, nPacketSize, bitstream...)
			if (nPacketLen >= 13)
			{
				const uint32_t dwGuid = *reinterpret_cast<const uint32_t*>(&aBuffer[1]);
				const uint16_t wClassId = *reinterpret_cast<const uint16_t*>(&aBuffer[5]);
				const uint16_t wSubX = *reinterpret_cast<const uint16_t*>(&aBuffer[7]);
				const uint16_t wSubY = *reinterpret_cast<const uint16_t*>(&aBuffer[9]);
				const uint8_t nLifePct = aBuffer[11];
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

	// Reset authoritative client tables
	gdwClientPlayerGuid = 0;
	gnServerSubX = 0;
	gnServerSubY = 0;
	gbServerActLoaded = false;
	gnCurrentAct = 0;
	gdwServerMapSeed = 0x112A2026u;
	gwServerTownLevelId = LEVEL_ROGUEENCAMPMENT;
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
	gpClassSkillIcon = ARCHIVE_LoadCellFile(kClassSkillIconPaths[nSafeClass], 0);
	gpSkillTreeBg = ARCHIVE_LoadCellFile(kClassSkillTreeBgPaths[nSafeClass], 0);
	gpPlayerClassNu1 = ARCHIVE_LoadCellFile(kClassNu1Paths[nSafeClass], 0);
	gpPlayerClassFw = ARCHIVE_LoadCellFile(kClassFwPaths[nSafeClass], 0);

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

	D2Win_10146();
	gpPlayerCompIdle = D2Win_10144(gnCharClass, 5, gaCharComponents, gaCharColors);
	gpPlayerCompWalk = D2Win_10144(gnCharClass, 6, gaCharComponents, gaCharColors);

	gfPlayerWorldX = 0.0f;
	gfPlayerWorldY = 0.0f;

	if (gpClientAct)
	{
		DUNGEON_FreeAct(gpClientAct);
		gpClientAct = nullptr;
	}
	gpClientAct = DUNGEON_AllocAct(0, 0x112A2026u, FALSE, nullptr, 0, nullptr, LEVEL_ROGUEENCAMPMENT, nullptr, nullptr);
	if (gpClientAct)
	{
		int nSpawnSubX = 0;
		int nSpawnSubY = 0;
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
		if (D2DrlgLevelStrc* pTownLevel = DRLG_GetLevel(DUNGEON_GetDrlgFromAct(gpClientAct), LEVEL_ROGUEENCAMPMENT))
		{
			for (D2DrlgRoomStrc* pDrlgRoom = pTownLevel->pFirstRoomEx; pDrlgRoom; pDrlgRoom = pDrlgRoom->pDrlgRoomNext)
			{
				DUNGEON_StreamRoomAtCoords(gpClientAct, pDrlgRoom->nTileXPos, pDrlgRoom->nTileYPos);
			}
		}
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
	if (gpClientAct)
	{
		DUNGEON_FreeAct(gpClientAct);
		gpClientAct = nullptr;
	}
	for (int32_t i = 0; i < kMaxClientItems; ++i)
	{
		FreeCellPtr(&gClientItems[i].pInvCell);
	}
	D2Win_10146();
	gpPlayerCompIdle = nullptr;
	gpPlayerCompWalk = nullptr;
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

static void PopulateCompositeLayerGfxData(
	D2CompositeUnitStrc* pComp,
	D2CofDataStrc* pCofData,
	uint8_t nComponent,
	uint8_t nDir,
	uint32_t nFrame,
	D2GfxDataStrc* pGfxData)
{
	pGfxData->nUnitIndex = pComp->nUnitIndex;
	pGfxData->nMode = pComp->nMode;
	pGfxData->nComponent = nComponent;
	pGfxData->nDirection = nDir;
	pGfxData->nFrame = nFrame;
	pGfxData->dwName[0] = (pComp->nUnitIndex >= 0 && pComp->nUnitIndex < 7)
		? kClassTokens[pComp->nUnitIndex]
		: '  MA';
	pGfxData->dwName[1] = (nComponent < 16) ? kCompTokens[nComponent] : '  RT';
	pGfxData->dwName[2] = pComp->dwArmorType[nComponent] ? pComp->dwArmorType[nComponent] : ' til';
	pGfxData->dwName[3] = (pComp->nMode >= 0 && pComp->nMode < 20) ? kModeTokens[pComp->nMode] : '  NT';
	pGfxData->dwName[4] = (pComp->nWClassTokenIndex >= 0 && pComp->nWClassTokenIndex < 15)
		? kWClassTokens[pComp->nWClassTokenIndex]
		: ' hth';

	const uint8_t* pLayer = reinterpret_cast<const uint8_t*>(&pCofData[1]);
	for (int i = 0; i < pCofData->unk0x00; ++i)
	{
		if (pLayer[0] == nComponent)
		{
			char szWClass[4] = { ' ', ' ', ' ', ' ' };
			if (pLayer[5])
			{
				szWClass[0] = static_cast<char>(pLayer[5]);
				if (pLayer[6])
				{
					szWClass[1] = static_cast<char>(pLayer[6]);
					if (pLayer[7])
					{
						szWClass[2] = static_cast<char>(pLayer[7]);
					}
				}
			}
			std::memcpy(&pGfxData->dwName[4], szWClass, sizeof(uint32_t));
			break;
		}
		pLayer += 9;
	}
	pGfxData->fFlags |= 2u;
}

static void DrawCompositeCharacter(D2CompositeUnitStrc* pComp, int32_t nScreenX, int32_t nScreenY, uint8_t nDir8)
{
	if (!pComp || !pComp->pGfxInfo || !pComp->pGfxInfo->pCofInfo)
	{
		return;
	}

	D2CofInfoStrc* pCofInfo = pComp->pGfxInfo->pCofInfo;
	while (pCofInfo && pCofInfo->pCofNode->nMode != static_cast<uint32_t>(pComp->nMode))
	{
		pCofInfo = pCofInfo->pNext;
	}
	if (!pCofInfo || !pCofInfo->pCofNode || !pCofInfo->pCofNode->pCofData)
	{
		return;
	}

	D2CofDataStrc* pCofData = pCofInfo->pCofNode->pCofData;
	const int nFramesPerDir = pCofData->unk0x01;
	const int nLayers = pCofData->unk0x00;
	if (nFramesPerDir <= 0 || nLayers <= 0)
	{
		return;
	}

	const uint8_t nMaxDirs = pCofData->unk0x02 ? pCofData->unk0x02 : 8;
	const uint8_t nClampedDir = static_cast<uint8_t>(nDir8 % nMaxDirs);
	unsigned long dwDirRow = 0;
	if (nMaxDirs)
	{
		_BitScanForward(&dwDirRow, nMaxDirs);
	}
	const uint8_t nMappedDir = kDirRemapTable[dwDirRow < 7 ? dwDirRow : 3][nClampedDir];

	const uint32_t nFrame = (pComp->unk0x08 >> 8) % static_cast<uint32_t>(nFramesPerDir);
	const uint8_t* pOrderTable = reinterpret_cast<const uint8_t*>(pCofData) + 28 + nFramesPerDir +
		nLayers * (nClampedDir * nFramesPerDir + nFrame + 9);

	// Pass 1: Shadows
	for (int i = 0; i < nLayers; ++i)
	{
		const uint8_t nComponent = pOrderTable[i];
		if (nComponent >= 16)
		{
			continue;
		}
		D2GfxDataStrc gfxData = {};
		PopulateCompositeLayerGfxData(pComp, pCofData, nComponent, nMappedDir, nFrame, &gfxData);
		if (D2CMP_SpriteValidate(&gfxData, 0, TRUE))
		{
			TEXTURE_CelDrawShadow(&gfxData, nScreenX - 2, nScreenY);
		}
	}

	// Pass 2: Lit Sprite Layers
	bool bDrewAnyLayer = false;
	for (int i = 0; i < nLayers; ++i)
	{
		const uint8_t nComponent = pOrderTable[i];
		if (nComponent >= 16)
		{
			continue;
		}
		D2GfxDataStrc gfxData = {};
		PopulateCompositeLayerGfxData(pComp, pCofData, nComponent, nMappedDir, nFrame, &gfxData);
		if (D2CMP_SpriteValidate(&gfxData, 0, TRUE))
		{
			bDrewAnyLayer = true;
			TEXTURE_CelDraw(&gfxData, nScreenX, nScreenY, 0xFFFFFFFFu, DRAWMODE_TRANSHIGHLIGHT, nullptr);
		}
	}

	if (!bDrewAnyLayer)
	{
		D2CellFileStrc* pFallbackCell = (gbPlayerMoving && gpPlayerClassFw) ? gpPlayerClassFw : gpPlayerClassNu1;
		if (pFallbackCell)
		{
			const uint32_t nFrames = D2CMP_CelFileGetCelsPerDirection(pFallbackCell);
			D2GfxDataStrc tCharGfx = {};
			tCharGfx.pCellFile = pFallbackCell;
			tCharGfx.nDirection = 0;
			tCharGfx.nFrame = nFrames ? ((GetTickCount() / 55) % nFrames) : 0;
			TEXTURE_CelDrawShadow(&tCharGfx, nScreenX - 32, nScreenY + 35);
			TEXTURE_CelDraw(&tCharGfx, nScreenX - 30, nScreenY + 35, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
		}
	}

	const uint32_t nStep = pComp->unk0x10 ? pComp->unk0x10 : 128u;
	const uint32_t nNextFrame = pComp->unk0x08 + nStep;
	pComp->unk0x08 = ((nNextFrame >> 8) >= static_cast<uint32_t>(nFramesPerDir - 1)) ? 0 : nNextFrame;
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

	if (!gbPlayerMoving)
	{
		return;
	}

	const float fDx = gfTargetWorldX - gfPlayerWorldX;
	const float fDy = gfTargetWorldY - gfPlayerWorldY;
	const float fDist = std::sqrt(fDx * fDx + fDy * fDy);
	if (fDist < 6.0f)
	{
		gfPlayerWorldX = gfTargetWorldX;
		gfPlayerWorldY = gfTargetWorldY;
		gbPlayerMoving = false;
		return;
	}

	const float fStep = gbRunEnabled ? 11.0f : 7.0f;
	gfPlayerWorldX += (fDx / fDist) * fStep;
	gfPlayerWorldY += (fDy / fDist) * fStep;

	// Stream adjacent DRLG rooms around player's current subtile
	if (gpClientAct)
	{
		int nCurSubX = static_cast<int>(gfPlayerWorldX);
		int nCurSubY = static_cast<int>(gfPlayerWorldY);
		DUNGEON_ClientToGameSubtileCoords(&nCurSubX, &nCurSubY);
		if (nCurSubX > 0 && nCurSubY > 0)
		{
			DUNGEON_StreamRoomAtCoords(gpClientAct, nCurSubX / 5, nCurSubY / 5);
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
}

static uint32_t PackThreeCharToken(const char* szStr)
{
	char aBytes[4] = { ' ', ' ', ' ', ' ' };
	if (szStr && szStr[0])
	{
		aBytes[0] = szStr[0];
		if (szStr[1])
		{
			aBytes[1] = szStr[1];
			if (szStr[2])
			{
				aBytes[2] = szStr[2];
			}
		}
	}
	uint32_t dwOut = 0;
	std::memcpy(&dwOut, aBytes, sizeof(uint32_t));
	return dwOut;
}

static void DrawObjectByClassAndMode(int32_t nObjId, int32_t nMode, int32_t nScreenX, int32_t nScreenY, DWORD dwTick)
{
	D2ObjectsTxt* pObjTxt = DATATBLS_GetObjectsTxtRecord(nObjId);
	if (!pObjTxt || !pObjTxt->nDraw || !pObjTxt->szToken[0])
	{
		return;
	}
	const uint32_t dwTok = PackThreeCharToken(pObjTxt->szToken);
	const bool bUseOperating = (nMode == 1 || nMode == 2 || pObjTxt->dwFrameCnt[1] > 0);
	const uint32_t dwMode = bUseOperating ? PackThreeCharToken("ON") : PackThreeCharToken("NU");

	D2GfxDataStrc tObjGfx = {};
	tObjGfx.nUnitType = UNIT_OBJECT;
	tObjGfx.nUnitIndex = nObjId;
	tObjGfx.nDirection = 0;
	tObjGfx.dwName[0] = dwTok;
	tObjGfx.dwName[1] = '  RT';
	tObjGfx.dwName[2] = ' til';
	tObjGfx.dwName[3] = dwMode;
	tObjGfx.dwName[4] = ' hth';

	if (!D2CMP_SpriteValidate(&tObjGfx, 0, TRUE) && bUseOperating)
	{
		tObjGfx.dwName[3] = PackThreeCharToken("NU");
		D2CMP_SpriteValidate(&tObjGfx, 0, TRUE);
	}
	if (tObjGfx.pCurrentCell)
	{
		tObjGfx.nFrame = (tObjGfx.nMaxFrames > 0) ? ((dwTick / 55u) % static_cast<uint32_t>(tObjGfx.nMaxFrames)) : 0;
		if (D2CMP_SpriteValidate(&tObjGfx, 0, TRUE))
		{
			TEXTURE_CelDrawShadow(&tObjGfx, nScreenX - 2, nScreenY);
			TEXTURE_CelDraw(&tObjGfx, nScreenX, nScreenY, 0xFFFFFFFFu, DRAWMODE_TRANSHIGHLIGHT, nullptr);
		}
	}

	// Fire / Torch flame overlay layer (S1 / S2)
	for (int sIdx = 0; sIdx < 2; ++sIdx)
	{
		D2GfxDataStrc tFlameGfx = {};
		tFlameGfx.nUnitType = UNIT_OBJECT;
		tFlameGfx.nUnitIndex = nObjId;
		tFlameGfx.nDirection = 0;
		tFlameGfx.dwName[0] = dwTok;
		tFlameGfx.dwName[1] = (sIdx == 0) ? '  1S' : '  2S';
		tFlameGfx.dwName[2] = ' til';
		tFlameGfx.dwName[3] = dwMode;
		tFlameGfx.dwName[4] = ' hth';
		if (!D2CMP_SpriteValidate(&tFlameGfx, 0, TRUE) && bUseOperating)
		{
			tFlameGfx.dwName[3] = PackThreeCharToken("NU");
			D2CMP_SpriteValidate(&tFlameGfx, 0, TRUE);
		}
		if (tFlameGfx.pCurrentCell)
		{
			tFlameGfx.nFrame = (tFlameGfx.nMaxFrames > 0) ? ((dwTick / 45u) % static_cast<uint32_t>(tFlameGfx.nMaxFrames)) : 0;
			if (D2CMP_SpriteValidate(&tFlameGfx, 0, TRUE))
			{
				TEXTURE_CelDraw(&tFlameGfx, nScreenX, nScreenY, 0xFFFFFFFFu, DRAWMODE_MODULATE, nullptr);
			}
		}
	}
}

static void DrawMonsterByClassAndMode(int32_t nMonId, uint8_t nAnimMode, int32_t nScreenX, int32_t nScreenY, DWORD dwTick)
{
	static const uint32_t kNpcCompOrder[7] = {
		'  GL', '  RT', '  DH', '  AR', '  AL', '  HR', '  HL'
	};
	D2MonStatsTxt* pMonTxt = DATATBLS_GetMonStatsTxtRecord(nMonId);
	if (!pMonTxt || !pMonTxt->dwCode)
	{
		return;
	}
	uint32_t dwModeTok = '  UN';
	if (nAnimMode == MONMODE_WALK || nAnimMode == MONMODE_RUN)
	{
		dwModeTok = '  LW';
	}
	else if (nAnimMode == MONMODE_ATTACK1 || nAnimMode == MONMODE_ATTACK2)
	{
		dwModeTok = '  1A';
	}
	else if (nAnimMode == MONMODE_DEATH || nAnimMode == MONMODE_DEAD)
	{
		dwModeTok = '  DD';
	}

	for (int pass = 0; pass < 2; ++pass)
	{
		for (int c = 0; c < 7; ++c)
		{
			D2GfxDataStrc tMonGfx = {};
			tMonGfx.nUnitType = UNIT_MONSTER;
			tMonGfx.nUnitIndex = nMonId;
			tMonGfx.nDirection = 0;
			tMonGfx.dwName[0] = pMonTxt->dwCode;
			tMonGfx.dwName[1] = kNpcCompOrder[c];
			tMonGfx.dwName[2] = ' til';
			tMonGfx.dwName[3] = dwModeTok;
			tMonGfx.dwName[4] = ' hth';
			if (!D2CMP_SpriteValidate(&tMonGfx, 0, TRUE) && dwModeTok != '  UN')
			{
				tMonGfx.dwName[3] = '  UN';
				D2CMP_SpriteValidate(&tMonGfx, 0, TRUE);
			}
			if (tMonGfx.pCurrentCell)
			{
				tMonGfx.nFrame = (tMonGfx.nMaxFrames > 0)
					? ((nAnimMode == MONMODE_DEAD) ? static_cast<uint32_t>(tMonGfx.nMaxFrames - 1) : ((dwTick / 70u) % static_cast<uint32_t>(tMonGfx.nMaxFrames)))
					: 0;
				if (D2CMP_SpriteValidate(&tMonGfx, 0, TRUE))
				{
					if (pass == 0)
					{
						TEXTURE_CelDrawShadow(&tMonGfx, nScreenX - 2, nScreenY);
					}
					else
					{
						TEXTURE_CelDraw(&tMonGfx, nScreenX, nScreenY, 0xFFFFFFFFu, DRAWMODE_TRANSHIGHLIGHT, nullptr);
					}
				}
			}
		}
	}
}

static bool HasServerUnitNearSubtile(uint8_t nUnitType, uint16_t wClassId, int32_t nSubX, int32_t nSubY)
{
	for (int32_t i = 0; i < kMaxClientUnits; ++i)
	{
		const ClientWorldUnit& u = gClientUnits[i];
		if (!u.bActive || u.nUnitType != nUnitType || u.wClassId != wClassId)
		{
			continue;
		}
		if (std::abs(u.nSubX - nSubX) <= 3 && std::abs(u.nSubY - nSubY) <= 3)
		{
			return true;
		}
	}
	return false;
}

static void DrawPresetUnitsInRoom(D2ActiveRoomStrc* pRoom, int32_t nCamX, int32_t nCamY)
{
	const DWORD dwTick = GetTickCount();

	for (D2PresetUnitStrc* pPreset = (pRoom && pRoom->pDrlgRoom) ? pRoom->pDrlgRoom->pPresetUnits : nullptr; pPreset; pPreset = pPreset->pNext)
	{
		const int32_t nSubX = pRoom->tCoords.nSubtileX + pPreset->nXpos;
		const int32_t nSubY = pRoom->tCoords.nSubtileY + pPreset->nYpos;
		if (HasServerUnitNearSubtile(static_cast<uint8_t>(pPreset->nUnitType), static_cast<uint16_t>(pPreset->nIndex), nSubX, nSubY))
		{
			continue;
		}

		int nWorldX = nSubX;
		int nWorldY = nSubY;
		DUNGEON_GameSubtileToClientCoords(&nWorldX, &nWorldY);

		const int32_t nScreenX = nWorldX - nCamX + 400;
		const int32_t nScreenY = nWorldY - nCamY + 280;
		if (nScreenX < -160 || nScreenX > 960 || nScreenY < -160 || nScreenY > 760)
		{
			continue;
		}

		if (pPreset->nUnitType == UNIT_OBJECT)
		{
			DrawObjectByClassAndMode(pPreset->nIndex, pPreset->nMode, nScreenX, nScreenY, dwTick);
		}
		else if (pPreset->nUnitType == UNIT_MONSTER)
		{
			DrawMonsterByClassAndMode(pPreset->nIndex, MONMODE_NEUTRAL, nScreenX, nScreenY, dwTick);
		}
	}
}

static void DrawServerUnitsAndGroundItems(int32_t nCamX, int32_t nCamY)
{
	const DWORD dwTick = GetTickCount();

	// 1. Draw server-synced Ground Items (0x9C)
	for (int32_t i = 0; i < kMaxClientItems; ++i)
	{
		const ClientItemEntry& it = gClientItems[i];
		if (!it.bActive || it.nLocation != CLI_ITEMLOC_GROUND)
		{
			continue;
		}
		int nWorldX = it.nWorldSubX;
		int nWorldY = it.nWorldSubY;
		DUNGEON_GameSubtileToClientCoords(&nWorldX, &nWorldY);
		const int32_t nScreenX = nWorldX - nCamX + 400;
		const int32_t nScreenY = nWorldY - nCamY + 280;
		if (nScreenX < -120 || nScreenX > 920 || nScreenY < -120 || nScreenY > 720)
		{
			continue;
		}
		if (it.pInvCell)
		{
			D2GfxDataStrc tGfx = {};
			tGfx.pCellFile = it.pInvCell;
			tGfx.nDirection = 0;
			tGfx.nFrame = 0;
			TEXTURE_CelDraw(&tGfx, nScreenX - 14, nScreenY, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
		}
		else
		{
			D2GFX_DrawBox(nScreenX - 10, nScreenY - 12, nScreenX + 10, nScreenY, 0x6Au, DRAWMODE_NORMAL);
		}

		if (gbShowGroundItems || gnHoveredGroundItemIdx == i)
		{
			Unicode wszItem[64] = {};
			Unicode::win2Unicode(wszItem, it.szName, 63);
			D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
			D2Win_10129_DrawFramedText(wszItem, nScreenX, nScreenY - 24, 0, 1);
		}
	}

	// 2. Draw server-streamed World Units (0xAC Monsters/NPCs, 0x51 Objects)
	for (int32_t i = 0; i < kMaxClientUnits; ++i)
	{
		const ClientWorldUnit& u = gClientUnits[i];
		if (!u.bActive)
		{
			continue;
		}
		const int32_t nScreenX = static_cast<int32_t>(u.fWorldX) - nCamX + 400;
		const int32_t nScreenY = static_cast<int32_t>(u.fWorldY) - nCamY + 280;
		if (nScreenX < -160 || nScreenX > 960 || nScreenY < -160 || nScreenY > 760)
		{
			continue;
		}
		if (u.nUnitType == UNIT_OBJECT)
		{
			DrawObjectByClassAndMode(u.wClassId, u.nAnimMode, nScreenX, nScreenY, dwTick);
		}
		else if (u.nUnitType == UNIT_MONSTER)
		{
			DrawMonsterByClassAndMode(u.wClassId, u.nAnimMode, nScreenX, nScreenY, dwTick);
		}
	}
}

int32_t __stdcall TILE_ShadowTileDraw(D2TileLibraryEntryStrc* pTile, int32_t nXPos, int32_t nYPos, DrawMode eDrawMode, int32_t nScreenPanels);

static void DrawRogueEncampmentGround(bool bUpperLayerPass)
{
	if (!gpClientAct)
	{
		return;
	}

	const int32_t nCamX = static_cast<int32_t>(gfPlayerWorldX);
	const int32_t nCamY = static_cast<int32_t>(gfPlayerWorldY);

	D2GfxLightExStrc aFloorLightEx[64];
	std::memset(aFloorLightEx, 0xFF, sizeof(aFloorLightEx));

	D2GfxLightStrc aWallLight[8];
	std::memset(aWallLight, 0xFF, sizeof(aWallLight));

	if (!bUpperLayerPass)
	{
		// Pass 1: Floor tiles (ground level)
		for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom; pRoom = pRoom->pRoomNext)
		{
			int nFloors = 0;
			D2DrlgTileDataStrc* pFloors = DUNGEON_GetFloorTilesFromRoom(pRoom, &nFloors);
			if (!pFloors || nFloors <= 0)
			{
				continue;
			}
			for (int i = 0; i < nFloors; ++i)
			{
				const D2DrlgTileDataStrc& td = pFloors[i];
				if (!td.pTile || (td.dwFlags & MAPTILE_HIDDEN))
				{
					continue;
				}
				const int32_t nScreenX = td.nWidth + 80 - nCamX + 400;
				const int32_t nScreenY = td.nHeight - 80 - nCamY + 280;
				if (nScreenX < -160 || nScreenX > 960 || nScreenY < -160 || nScreenY > 760)
				{
					continue;
				}
				D2GFX_FloorTileDraw(td.pTile, aFloorLightEx, nScreenX, nScreenY, td.nWidth, td.nHeight, 0xFF, 0, nullptr);
			}
		}

		// Pass 2: Shadow tiles (stored in room roof/shadow tile list)
		for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom; pRoom = pRoom->pRoomNext)
		{
			int nShadows = 0;
			D2DrlgTileDataStrc* pShadows = DUNGEON_GetRoofTilesFromRoom(pRoom, &nShadows);
			if (!pShadows || nShadows <= 0)
			{
				continue;
			}
			for (int i = 0; i < nShadows; ++i)
			{
				const D2DrlgTileDataStrc& td = pShadows[i];
				if (!td.pTile || (td.dwFlags & MAPTILE_HIDDEN))
				{
					continue;
				}
				const int32_t nScreenX = td.nWidth - nCamX + 400;
				const int32_t nScreenY = td.nHeight - nCamY + 280;
				if (nScreenX < -160 || nScreenX > 960 || nScreenY < -160 || nScreenY > 760)
				{
					continue;
				}
				TILE_ShadowTileDraw(td.pTile, nScreenX, nScreenY, DRAWMODE_TRANSHIGHLIGHT, 0);
			}
		}

		// Pass 3: Lower walls (behind player) and background walls
		for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom; pRoom = pRoom->pRoomNext)
		{
			int nWalls = 0;
			D2DrlgTileDataStrc* pWalls = DUNGEON_GetWallTilesFromRoom(pRoom, &nWalls);
			if (!pWalls || nWalls <= 0)
			{
				continue;
			}
			for (int i = 0; i < nWalls; ++i)
			{
				const D2DrlgTileDataStrc& td = pWalls[i];
				if (!td.pTile || td.nTileType == TILETYPE_ROOF || (td.dwFlags & MAPTILE_HIDDEN))
				{
					continue;
				}
				const bool bLowerWall = (td.nTileType >= TILETYPE_LEFT_WALL_DOWN) || (td.nHeight <= nCamY);
				if (!bLowerWall)
				{
					continue;
				}
				const int32_t nScreenX = td.nWidth - nCamX + 400;
				const int32_t nScreenY = td.nHeight - nCamY + 280;
				if (nScreenX < -160 || nScreenX > 960 || nScreenY < -160 || nScreenY > 860)
				{
					continue;
				}
				TILE_TileDrawLit(td.pTile, nScreenX, nScreenY, aWallLight, 0);
			}
		}

		// Pass 4: Preset town objects (campfire, torches, stash, waypoint), live server units, and ground items
		for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom; pRoom = pRoom->pRoomNext)
		{
			DrawPresetUnitsInRoom(pRoom, nCamX, nCamY);
		}
		DrawServerUnitsAndGroundItems(nCamX, nCamY);
	}
	else
	{
		// Pass 5: Upper walls (in front of player) and Roofs
		for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom; pRoom = pRoom->pRoomNext)
		{
			int nWalls = 0;
			D2DrlgTileDataStrc* pWalls = DUNGEON_GetWallTilesFromRoom(pRoom, &nWalls);
			if (!pWalls || nWalls <= 0)
			{
				continue;
			}
			for (int i = 0; i < nWalls; ++i)
			{
				const D2DrlgTileDataStrc& td = pWalls[i];
				if (!td.pTile)
				{
					continue;
				}
				if (td.nTileType == TILETYPE_ROOF)
				{
					const int32_t nRoofH = D2CMP_TileGetRoofHeight(td.pTile);
					const int32_t nScreenX = td.nWidth + 80 - nCamX + 400;
					const int32_t nScreenY = td.nHeight - nRoofH - 80 - nCamY + 280;
					if (nScreenX < -160 || nScreenX > 960 || nScreenY < -160 || nScreenY > 860)
					{
						continue;
					}
					D2GFX_FloorTileDraw(td.pTile, aFloorLightEx, nScreenX, nScreenY, td.nWidth, td.nHeight, 0xFF, 0, nullptr);
					continue;
				}
				if (td.dwFlags & MAPTILE_HIDDEN)
				{
					continue;
				}
				const bool bLowerWall = (td.nTileType >= TILETYPE_LEFT_WALL_DOWN) || (td.nHeight <= nCamY);
				if (bLowerWall)
				{
					continue;
				}
				const int32_t nScreenX = td.nWidth - nCamX + 400;
				const int32_t nScreenY = td.nHeight - nCamY + 280;
				if (nScreenX < -160 || nScreenX > 960 || nScreenY < -160 || nScreenY > 860)
				{
					continue;
				}
				if (nScreenX > 280 && nScreenX < 520 && nScreenY > 240 && nScreenY < 420)
				{
					TILE_TileDrawTrans(td.pTile, nScreenX, nScreenY, aWallLight, 0, 0x80);
				}
				else
				{
					TILE_TileDrawLit(td.pTile, nScreenX, nScreenY, aWallLight, 0);
				}
			}
		}
	}
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

	const int32_t nCamX = static_cast<int32_t>(gfPlayerWorldX);
	const int32_t nCamY = static_cast<int32_t>(gfPlayerWorldY);

	for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom; pRoom = pRoom->pRoomNext)
	{
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
				D2GFX_DrawLine(nMapX - 3, nMapY, nMapX + 3, nMapY, nCol, 0xFFu);
				D2GFX_DrawLine(nMapX, nMapY - 3, nMapX, nMapY + 3, nCol, 0xFFu);
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
			D2GFX_DrawLine(nMapX - 3, nMapY, nMapX + 3, nMapY, nCol, 0xFFu);
			D2GFX_DrawLine(nMapX, nMapY - 3, nMapX, nMapY + 3, nCol, 0xFFu);
		}
	}

	// Player cross at center of Automap
	D2GFX_DrawLine(394, 260, 406, 260, 0x97u, 0xFFu);
	D2GFX_DrawLine(400, 254, 400, 266, 0x97u, 0xFFu);

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
		if (gpQuestTabs)
		{
			DrawSingleCelTopLeft(gpQuestTabs, 86, 66, 0);
		}

		static const char* const kAct1QuestTitles[6] = {
			"I. Den of Evil",
			"II. Sisters' Burial Grounds",
			"III. The Search for Cain",
			"IV. The Forgotten Tower",
			"V. Tools of the Trade",
			"VI. Sisters to the Slaughter",
		};
		static const char* const kAct1QuestDescs[6][2] = {
			{ "Speak to Akara in the Rogue Encampment", "to begin the Den of Evil quest." },
			{ "Defeat Blood Raven in the Burial", "Grounds beyond the Cold Plains." },
			{ "Search for Deckard Cain in the", "forsaken ruins of Tristram." },
			{ "Explore the Forgotten Tower", "deep within the Black Marsh." },
			{ "Recover the Horadric Malus from", "the Monastery Barracks for Charsi." },
			{ "Defeat the Demon Queen Andariel", "in the fourth level of the Catacombs." },
		};

		for (int32_t q = 0; q < 6; ++q)
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

		const int32_t nSafeQ = std::max<int32_t>(0, std::min<int32_t>(gnSelectedQuest, 5));
		D2Win_10127_SetFont(D2FONT_FONT16);
		DrawAsciiText(kAct1QuestTitles[nSafeQ], 240, 340, 4, TRUE);
		D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
		DrawAsciiText(kAct1QuestDescs[nSafeQ][0], 240, 368, 0, TRUE);
		DrawAsciiText(kAct1QuestDescs[nSafeQ][1], 240, 388, 0, TRUE);

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

		// Equipment slot silhouettes
		if (gpInvWeapons)
		{
			DrawSingleCelTopLeft(gpInvWeapons, 418, 108, 0);
			DrawSingleCelTopLeft(gpInvWeapons, 646, 108, 0);
		}
		if (gpInvArmor)
		{
			DrawSingleCelTopLeft(gpInvArmor, 532, 134, 0);
		}
		if (gpInvHelmGlove)
		{
			DrawSingleCelTopLeft(gpInvHelmGlove, 532, 66, 1);
			DrawSingleCelTopLeft(gpInvHelmGlove, 418, 230, 0);
		}
		if (gpInvBoots)
		{
			DrawSingleCelTopLeft(gpInvBoots, 646, 230, 0);
		}
		if (gpInvBelt)
		{
			DrawSingleCelTopLeft(gpInvBelt, 532, 258, 0);
		}
		if (gpInvRingAmulet)
		{
			DrawSingleCelTopLeft(gpInvRingAmulet, 604, 96, 0);
			DrawSingleCelTopLeft(gpInvRingAmulet, 492, 258, 1);
			DrawSingleCelTopLeft(gpInvRingAmulet, 604, 258, 1);
		}

		// Draw live server-synced items (0x9C / 0x9D) in Paperdoll Equipment Slots & 10x4 Inventory Grid
		static const int32_t kBodySlotRects[13][4] = {
			{ 0,   0,   0,  0 },  // 0: None
			{ 532, 66,  56, 56 }, // 1: HEAD
			{ 604, 96,  28, 28 }, // 2: NECK
			{ 532, 134, 56, 84 }, // 3: TORSO
			{ 418, 108, 56, 112 },// 4: RARM
			{ 646, 108, 56, 112 },// 5: LARM
			{ 492, 258, 28, 28 }, // 6: RRING
			{ 604, 258, 28, 28 }, // 7: LRING
			{ 532, 258, 56, 28 }, // 8: BELT
			{ 646, 230, 56, 56 }, // 9: FEET
			{ 418, 230, 56, 56 }, // 10: GLOVES
			{ 418, 108, 56, 112 },// 11: SWRARM
			{ 646, 108, 56, 112 },// 12: SWLARM
		};

		const ClientItemEntry* pHoveredItem = nullptr;
		int32_t nHoverTipX = 560;
		int32_t nHoverTipY = 300;

		for (int32_t i = 0; i < kMaxClientItems; ++i)
		{
			const ClientItemEntry& it = gClientItems[i];
			if (!it.bActive)
			{
				continue;
			}
			if (it.nLocation == CLI_ITEMLOC_EQUIPPED && it.nBodyLoc >= 1 && it.nBodyLoc <= 12)
			{
				const int32_t nSlotX = kBodySlotRects[it.nBodyLoc][0];
				const int32_t nSlotY = kBodySlotRects[it.nBodyLoc][1];
				const int32_t nSlotW = kBodySlotRects[it.nBodyLoc][2];
				const int32_t nSlotH = kBodySlotRects[it.nBodyLoc][3];
				D2GFX_DrawBox(nSlotX, nSlotY, nSlotX + nSlotW, nSlotY + nSlotH, 0x00u, DRAWMODE_TRANS75);
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
				const int32_t nCellX = 418 + it.nGridX * 29;
				const int32_t nCellY = 315 + it.nGridY * 29;
				const int32_t nCellW = it.nInvW * 29;
				const int32_t nCellH = it.nInvH * 29;
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

	// 4b. Belt Potion Slots (x=423 + slot*31, y=561)
	for (int32_t i = 0; i < kMaxClientItems; ++i)
	{
		const ClientItemEntry& it = gClientItems[i];
		if (it.bActive && it.nLocation == CLI_ITEMLOC_BELT && it.pInvCell)
		{
			const int32_t nBeltSlot = std::max<int32_t>(0, it.nGridX) % 4;
			DrawSingleCelTopLeft(it.pInvCell, 423 + nBeltSlot * 31, 561, 0);
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

static void RenderInGameFrame()
{
	UpdatePlayerMovement();

	if (!D2GFX_StartDraw(0, 0, 0, 0))
	{
		return;
	}

	D2GFX_ClearScreen(0);

	// 1. Rogue Encampment Ground, Shadows, Lower Walls, Preset Units & Authoritative Server Units/Items
	DrawRogueEncampmentGround(false);

	// 2. Player Composite Character at screen center (400, 280)
	D2CompositeUnitStrc* pActiveComp = (gbPlayerMoving && gpPlayerCompWalk) ? gpPlayerCompWalk : gpPlayerCompIdle;
	DrawCompositeCharacter(pActiveComp, 400, 280, gnPlayerDir);

	// 3. Rogue Encampment Upper Walls & Roofs
	DrawRogueEncampmentGround(true);

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

	Unicode::win2Unicode(wszArea, "Act I - Rogue Encampment", 127);
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

	// 6. Left & Right Sub-Panels (Character, Quest Log, Party, Message Log, Inventory, Skill Tree)
	DrawLeftPanel();
	DrawRightPanel();

	// 7. Diablo II 800x600 Bottom HUD Control Panel, Mini-Panel & Orbs
	DrawHudControlPanel();

	// 8. ESC Game Menu (Options / Save and Exit Game / Return to Game)
	DrawEscGameMenu();

	// 9. Hand Cursor
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

		if (!bRemoteTcpMode && (gnGameType == 0 || gnGameType == 1))
		{
			GAME_ProcessNetworkMessages();
			GAME_UpdateGamesProgress(1);
			GAME_UpdateClients(0, 0);
			++gdwServerTicks;
		}

		DrainNetworkPackets();
		RenderInGameFrame();
		Sleep(40);
	}

	SendLeaveGamePacket();
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
	char szUseTsEnv[16] = {};
	char szConnectHostEnv[64] = {};
	const bool bHasConnectHost =
		(GetEnvironmentVariableA("D2MOO_CONNECT_HOST", szConnectHostEnv, sizeof(szConnectHostEnv)) > 0 && szConnectHostEnv[0] != '\0');
	const bool bExplicitHeadless =
		(GetEnvironmentVariableA("D2MOO_HEADLESS_SERVER", szHeadlessEnv, sizeof(szHeadlessEnv)) > 0 && szHeadlessEnv[0] != '0');
	const bool bUseTsClient =
		!bHasConnectHost &&
		!bExplicitHeadless &&
		(GetEnvironmentVariableA("D2MOO_USE_TS_CLIENT", szUseTsEnv, sizeof(szUseTsEnv)) > 0 && szUseTsEnv[0] != '0');

	if (bExplicitHeadless || bUseTsClient)
	{
		if (HWND hMainWnd = WINDOW_GetWindow())
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
		if (bUseTsClient)
		{
			hWebJob = CreateJobObjectA(nullptr, nullptr);
			if (hWebJob)
			{
				JOBOBJECT_EXTENDED_LIMIT_INFORMATION tJobLimits = {};
				tJobLimits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
				SetInformationJobObject(hWebJob, JobObjectExtendedLimitInformation, &tJobLimits, sizeof(tJobLimits));
			}

			char szExeDir[MAX_PATH] = {};
			GetModuleFileNameA(nullptr, szExeDir, sizeof(szExeDir));
			if (char* pLastSlash = std::strrchr(szExeDir, '\\'))
			{
				*pLastSlash = '\0';
			}

			char szScriptPath[MAX_PATH] = {};
			std::snprintf(szScriptPath, sizeof(szScriptPath), "%s\\..\\..\\..\\tools\\start_d2moo_web.mjs", szExeDir);
			if (GetFileAttributesA(szScriptPath) == INVALID_FILE_ATTRIBUTES)
			{
				SStrCopy(szScriptPath, "H:\\git\\D2MOO-112a\\tools\\start_d2moo_web.mjs", sizeof(szScriptPath));
			}

			char szSaveDir[MAX_PATH] = {};
			FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
			SetEnvironmentVariableA("D2MOO_SAVE_DIR", szSaveDir);

			if (pCfg && pCfg->szName[0])
			{
				SStrCopy(gszCharName, pCfg->szName, sizeof(gszCharName));
			}
			LoadCharacterSaveAppearance();

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
