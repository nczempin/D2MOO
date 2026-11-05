#include "D2Client.h"

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
static D2CellFileStrc* gpPlayerClassNu1 = nullptr;
static D2CellFileStrc* gpPlayerClassFw = nullptr;
static D2CompositeUnitStrc* gpPlayerCompIdle = nullptr;
static D2CompositeUnitStrc* gpPlayerCompWalk = nullptr;
static D2DrlgActStrc* gpClientAct = nullptr;

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
static POINT gMousePos = { 400, 300 };

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

static void SetMoveTargetFromScreenPoint(int32_t nScreenX, int32_t nScreenY)
{
	if (nScreenY >= 530)
	{
		return;
	}
	const float fDx = static_cast<float>(nScreenX - 400);
	const float fDy = static_cast<float>(nScreenY - 280);
	gfTargetWorldX = gfPlayerWorldX + fDx;
	gfTargetWorldY = gfPlayerWorldY + fDy;
	gbPlayerMoving = true;
}

// 1.12a: 0x6FB1D200
static LRESULT __stdcall D2Client_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		gbWindowClosed = 1;
		gbExitApplication = 1;
		gnClientState = D2CLIENT_STATE_LEAVE_GAME;
		return 0;
	case WM_MOUSEMOVE:
		gMousePos.x = static_cast<int16_t>(LOWORD(lParam));
		gMousePos.y = static_cast<int16_t>(HIWORD(lParam));
		if (gbMouseLeftHeld)
		{
			SetMoveTargetFromScreenPoint(gMousePos.x, gMousePos.y);
		}
		return 0;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		gMousePos.x = static_cast<int16_t>(LOWORD(lParam));
		gMousePos.y = static_cast<int16_t>(HIWORD(lParam));
		gbMouseLeftHeld = (uMsg == WM_LBUTTONDOWN);
		SetMoveTargetFromScreenPoint(gMousePos.x, gMousePos.y);
		return 0;
	case WM_LBUTTONUP:
		gbMouseLeftHeld = false;
		return 0;
	case WM_KEYUP:
		if (wParam == VK_SNAPSHOT)
		{
			D2Win_10168_WINMAIN_CreateScreenshot();
			return 0;
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			gbSaveAndExitRequested = 1;
			gbExitApplication = 0;
			gnClientState = D2CLIENT_STATE_LEAVE_GAME;
			return 0;
		}
		else if (wParam == VK_LEFT || wParam == 'A')
		{
			gfTargetWorldX = gfPlayerWorldX - 120.0f;
			gfTargetWorldY = gfPlayerWorldY;
			gbPlayerMoving = true;
		}
		else if (wParam == VK_RIGHT || wParam == 'D')
		{
			gfTargetWorldX = gfPlayerWorldX + 120.0f;
			gfTargetWorldY = gfPlayerWorldY;
			gbPlayerMoving = true;
		}
		else if (wParam == VK_UP || wParam == 'W')
		{
			gfTargetWorldX = gfPlayerWorldX;
			gfTargetWorldY = gfPlayerWorldY - 90.0f;
			gbPlayerMoving = true;
		}
		else if (wParam == VK_DOWN || wParam == 'S')
		{
			gfTargetWorldX = gfPlayerWorldX;
			gfTargetWorldY = gfPlayerWorldY + 90.0f;
			gbPlayerMoving = true;
		}
		break;
	default:
		break;
	}
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

// 1.12a: 0x6FB294E0
void __fastcall D2Client_SendJoinGamePacket(const char* szCharName)
{
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
	while (CLIENT_DequeueGamePacket(aBuffer, sizeof(aBuffer)) != -1)
	{
		++gdwGamePacketsReceived;
		if (aBuffer[0] == 0x02) // D2GS_ACTINITDONE -> send D2CLTSYS_JOINACT (0x6B)
		{
			const uint8_t pktJoinAct = 0x6Bu;
			CLIENT_Send(0, &pktJoinAct, 1);
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

	gpHudCtrlPnl = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\800ctrlpnl7", 0);
	gpHudHlthMana = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\hlthmana", 0);
	gpHudOverlap = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\PANEL\\overlap", 0);
	gpHudCampfire = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\fire", 0);
	gpHudCursor = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CURSOR\\ohand", 0);
	const uint8_t nSafeClass = (gnCharClass < 7) ? gnCharClass : 0;
	gpPlayerClassNu1 = ARCHIVE_LoadCellFile(kClassNu1Paths[nSafeClass], 0);
	gpPlayerClassFw = ARCHIVE_LoadCellFile(kClassFwPaths[nSafeClass], 0);

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
	D2Win_10146();
	gpPlayerCompIdle = nullptr;
	gpPlayerCompWalk = nullptr;
	FreeCellPtr(&gpPlayerClassNu1);
	FreeCellPtr(&gpPlayerClassFw);
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

	const float fStep = 7.0f;
	gfPlayerWorldX += (fDx / fDist) * fStep;
	gfPlayerWorldY += (fDy / fDist) * fStep;

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

static void DrawPresetUnitsInRoom(D2ActiveRoomStrc* pRoom, int32_t nCamX, int32_t nCamY)
{
	static const uint32_t kNpcCompOrder[7] = {
		'  GL', '  RT', '  DH', '  AR', '  AL', '  HR', '  HL'
	};
	const DWORD dwTick = GetTickCount();

	for (D2PresetUnitStrc* pPreset = (pRoom && pRoom->pDrlgRoom) ? pRoom->pDrlgRoom->pPresetUnits : nullptr; pPreset; pPreset = pPreset->pNext)
	{
		int nWorldX = pRoom->tCoords.nSubtileX + pPreset->nXpos;
		int nWorldY = pRoom->tCoords.nSubtileY + pPreset->nYpos;
		DUNGEON_GameSubtileToClientCoords(&nWorldX, &nWorldY);

		const int32_t nScreenX = nWorldX - nCamX + 400;
		const int32_t nScreenY = nWorldY - nCamY + 280;
		if (nScreenX < -160 || nScreenX > 960 || nScreenY < -160 || nScreenY > 760)
		{
			continue;
		}

		if (pPreset->nUnitType == UNIT_OBJECT)
		{
			D2ObjectsTxt* pObjTxt = DATATBLS_GetObjectsTxtRecord(pPreset->nIndex);
			if (!pObjTxt || !pObjTxt->nDraw || !pObjTxt->szToken[0])
			{
				continue;
			}
			const uint32_t dwTok = PackThreeCharToken(pObjTxt->szToken);
			const bool bUseOperating = (pPreset->nMode == 1 || pObjTxt->dwFrameCnt[1] > 0);
			const uint32_t dwMode = bUseOperating ? PackThreeCharToken("ON") : PackThreeCharToken("NU");

			D2GfxDataStrc tObjGfx = {};
			tObjGfx.nUnitType = UNIT_OBJECT;
			tObjGfx.nUnitIndex = pPreset->nIndex;
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
				tFlameGfx.nUnitIndex = pPreset->nIndex;
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
		else if (pPreset->nUnitType == UNIT_MONSTER)
		{
			D2MonStatsTxt* pMonTxt = DATATBLS_GetMonStatsTxtRecord(pPreset->nIndex);
			if (!pMonTxt || !pMonTxt->dwCode)
			{
				continue;
			}
			for (int pass = 0; pass < 2; ++pass)
			{
				for (int c = 0; c < 7; ++c)
				{
					D2GfxDataStrc tMonGfx = {};
					tMonGfx.nUnitType = UNIT_MONSTER;
					tMonGfx.nUnitIndex = pPreset->nIndex;
					tMonGfx.nDirection = 0;
					tMonGfx.dwName[0] = pMonTxt->dwCode;
					tMonGfx.dwName[1] = kNpcCompOrder[c];
					tMonGfx.dwName[2] = ' til';
					tMonGfx.dwName[3] = '  UN';
					tMonGfx.dwName[4] = ' hth';
					if (D2CMP_SpriteValidate(&tMonGfx, 0, TRUE))
					{
						tMonGfx.nFrame = (tMonGfx.nMaxFrames > 0) ? ((dwTick / 70u) % static_cast<uint32_t>(tMonGfx.nMaxFrames)) : 0;
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

		// Pass 4: Preset town objects (campfire, torches, stash, waypoint) and NPCs
		for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientAct); pRoom; pRoom = pRoom->pRoomNext)
		{
			DrawPresetUnitsInRoom(pRoom, nCamX, nCamY);
		}
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

static void DrawHudControlPanel()
{
	// Health & Mana Orbs (hlthmana.dc6)
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

	// 800x600 Control Panel slices (800ctrlpnl7.dc6)
	if (gpHudCtrlPnl)
	{
		const int32_t nFrames = D2CMP_CelFileGetCelsPerDirection(gpHudCtrlPnl);
		int32_t nCurX = 0;
		for (int32_t f = 0; f < nFrames; ++f)
		{
			D2GfxDataStrc tPnl = {};
			tPnl.pCellFile = gpHudCtrlPnl;
			tPnl.nDirection = 0;
			tPnl.nFrame = f;
			D2CellFileStrc* pCell = D2CMP_CelGetHandle(&tPnl);
			const int32_t nW = pCell ? D2CMP_CelGetWidth(pCell) : 128;
			const int32_t nY = (f == 0 || f == nFrames - 1) ? 600 : 599;
			TEXTURE_CelDraw(&tPnl, nCurX, nY, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
			nCurX += nW;
		}
	}

	// Orb glass overlaps
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
}

static void RenderInGameFrame()
{
	UpdatePlayerMovement();

	if (!D2GFX_StartDraw(0, 0, 0, 0))
	{
		return;
	}

	D2GFX_ClearScreen(0);

	// 1. Rogue Encampment Ground, Shadows, Lower Walls & Preset Units
	DrawRogueEncampmentGround(false);

	// 2. Player Composite Character at screen center (400, 280)
	D2CompositeUnitStrc* pActiveComp = (gbPlayerMoving && gpPlayerCompWalk) ? gpPlayerCompWalk : gpPlayerCompIdle;
	DrawCompositeCharacter(pActiveComp, 400, 280, gnPlayerDir);

	// 3. Rogue Encampment Upper Walls & Roofs
	DrawRogueEncampmentGround(true);

	// 3. Player Overhead Name Banner & Area Title
	Unicode wszArea[128] = {};
	Unicode wszChar[128] = {};
	Unicode wszHint[128] = {};
	char szCharLine[128] = {};
	std::snprintf(
		szCharLine,
		sizeof(szCharLine),
		"%s  -  Level %u %s%s",
		gszCharName,
		gnCharLevel,
		kClassNames[gnCharClass < 7 ? gnCharClass : 0],
		(gwCharSaveFlags & 0x04) ? " (Hardcore)" : "");

	Unicode::win2Unicode(wszArea, "Act I - Rogue Encampment", 127);
	Unicode::win2Unicode(wszChar, szCharLine, 127);
	Unicode::win2Unicode(wszHint, "Left-Click / WASD: Move   |   ESC: Save and Exit Game", 127);

	D2Win_10127_SetFont(D2FONT_FONT16);
	D2Win_10117_DrawText(wszChar, 20, 26, (gwCharSaveFlags & 0x04) ? 1 : 4, 0);
	D2Win_10117_DrawText(wszArea, 600, 26, 0, 0);
	D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
	D2Win_10117_DrawText(wszHint, 230, 535, 0, 0);

	// 4. Diablo II 800x600 Bottom HUD Control Panel & Orbs
	DrawHudControlPanel();

	// 5. Hand Cursor
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

	if (gpD2Config && gpD2Config->szName[0])
	{
		SStrCopy(gszCharName, gpD2Config->szName, sizeof(gszCharName));
	}

	InitInGameAssets();

	if (gnGameType != 3 && gnGameType != 7 && gnGameType != 9)
	{
		D2Client_SendJoinGamePacket(gszCharName);
		if (gnGameType == 6 || gnGameType == 8)
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

	if (gnGameType == 6 || gnGameType == 8)
	{
		EnterCriticalSection(&gServerThreadLock);
		gbRunServerThreadUpdates = 1;
		LeaveCriticalSection(&gServerThreadLock);
	}

	// Single-player / local frame loop (0x6FB2CFF0)
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

		if (gnGameType == 0 || gnGameType == 1)
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

	ShutdownInGameAssets();

	if (gnGameType == 0 || gnGameType == 1)
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
	if (gnGameType == 0 || gnGameType == 1)
	{
		GAME_ProcessNetworkMessages();
	}
	DrainNetworkPackets();
	CLIENT_Release();
	if (gnGameType == 0 || gnGameType == 1 || gnGameType == 6 || gnGameType == 8)
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
