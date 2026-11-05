#include "D2Client.h"

#include <cstring>

#include <Fog.h>
#include <Storm.h>
#include <D2StrTable.h>
#include <D2Gfx.h>
#include <Window.h>
#include <D2CMP.h>
#include <D2Sound.h>
#include <D2WinMain.h>
#include <D2WinArchive.h>
#include <D2WinFont.h>
#include <D2WinPalette.h>
#include <D2DataTbls.h>
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
static CRITICAL_SECTION gServerThreadLock = {};                // 0x6FBB327C
static HANDLE ghServerThread = nullptr;                        // 0x6FBB33D8
static DWORD gdwServerThreadId = 0;                            // 0x6FBB340C
static volatile int32_t gbStopServerThread = 0;                // 0x6FBCC038
static volatile int32_t gbRunServerThreadUpdates = 0;          // 0x6FBCC03C
static D2GameDataTableStrc gLocalGameDataTable = {};           // 0x6FBD0E98
static uint32_t gLocalGameList[8] = {};                        // 0x6FBCC018

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
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			gbSaveAndExitRequested = 1;
			gbExitApplication = 0;
			gnClientState = D2CLIENT_STATE_LEAVE_GAME;
			return 0;
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

static void RenderInGameFrame()
{
	if (!D2GFX_StartDraw(0, 0, 0, 0))
	{
		return;
	}

	D2GFX_ClearScreen(0);

	// Draw ground / viewport frame and HUD status
	D2GFX_DrawBoxAlpha(0, 0, 800, 600, 0x10, 255);
	D2GFX_DrawBoxAlpha(0, 552, 800, 600, 0x08, 255);

	Unicode wszTitle[128] = {};
	Unicode wszChar[128] = {};
	Unicode wszHint[128] = {};
	char szCharLine[128] = {};
	std::snprintf(
		szCharLine,
		sizeof(szCharLine),
		"Character: %s  |  Act I - Rogue Encampment  |  Tick: %u  |  Packets: %u",
		gszCharName,
		gdwServerTicks,
		gdwGamePacketsReceived);

	Unicode::win2Unicode(wszTitle, "Diablo II: Lord of Destruction (v1.12a - D2MOO In-Game Session)", 127);
	Unicode::win2Unicode(wszChar, szCharLine, 127);
	Unicode::win2Unicode(wszHint, "Press ESC to Save and Exit Game", 127);

	D2Win_10127_SetFont(D2FONT_FONT16);
	D2Win_10117_DrawText(wszTitle, 20, 40, 4, 0);
	D2Win_10117_DrawText(wszChar, 20, 80, 0, 0);
	D2Win_10117_DrawText(wszHint, 20, 580, 9, 0);

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
