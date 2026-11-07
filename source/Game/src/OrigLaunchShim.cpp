#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <D2Config.h>

static bool gbLaunchedOnce = false;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	(void)hinstDLL;
	(void)fdwReason;
	(void)lpvReserved;
	return TRUE;
}

static int32_t __fastcall D2LAUNCH_OrigShimMainEntry(D2ConfigStrc* pCfg)
{
	if (!pCfg || gbLaunchedOnce)
	{
		return 0; // MODULE_NONE -> exit Original112a/Game.exe cleanly after leaving game
	}
	gbLaunchedOnce = true;

	char szCharName[24] = "ExpansionHero";
	char szClassEnv[16] = "0";
	char szArenaEnv[32] = "0x100004";
	char szSaveFlagsEnv[32] = "0x20";
	char szServerIp[24] = "127.0.0.1";
	char szGameName[24] = "D2MOO";
	char szSaveDir[MAX_PATH] = {};

	GetEnvironmentVariableA("D2MOO_ORIG_CHAR_NAME", szCharName, sizeof(szCharName));
	GetEnvironmentVariableA("D2MOO_ORIG_CHAR_CLASS", szClassEnv, sizeof(szClassEnv));
	GetEnvironmentVariableA("D2MOO_ORIG_ARENA_FLAGS", szArenaEnv, sizeof(szArenaEnv));
	GetEnvironmentVariableA("D2MOO_ORIG_SAVE_FLAGS", szSaveFlagsEnv, sizeof(szSaveFlagsEnv));
	GetEnvironmentVariableA("D2MOO_ORIG_SERVER_IP", szServerIp, sizeof(szServerIp));
	GetEnvironmentVariableA("D2MOO_ORIG_GAME_NAME", szGameName, sizeof(szGameName));

	if (GetEnvironmentVariableA("D2MOO_SAVE_DIR", szSaveDir, sizeof(szSaveDir)) > 0 && szSaveDir[0] != '\0')
	{
		HKEY hKey = nullptr;
		if (RegCreateKeyExA(
				HKEY_CURRENT_USER,
				"Software\\Blizzard Entertainment\\Diablo II",
				0,
				nullptr,
				0,
				KEY_SET_VALUE,
				nullptr,
				&hKey,
				nullptr) == ERROR_SUCCESS)
		{
			RegSetValueExA(
				hKey,
				"Save Path",
				0,
				REG_SZ,
				reinterpret_cast<const BYTE*>(szSaveDir),
				static_cast<DWORD>(std::strlen(szSaveDir) + 1));
			RegDeleteValueA(hKey, "Default Command Line");
			RegCloseKey(hKey);
		}
	}

	const int nCharClass = std::atoi(szClassEnv);
	const uint32_t dwArenaFlags = static_cast<uint32_t>(std::strtoul(szArenaEnv, nullptr, 0));
	const uint16_t wSaveFlags = static_cast<uint16_t>(std::strtoul(szSaveFlagsEnv, nullptr, 0));

	pCfg->bIsExpansion = 1;
	pCfg->bWindow = 1;
	pCfg->dwGameType = 5; // TCP/IP client connect to 127.0.0.1:4000 without starting a local server

	std::memset(pCfg->szGameName, 0, sizeof(pCfg->szGameName));
	std::strncpy(pCfg->szGameName, szGameName, sizeof(pCfg->szGameName) - 1);

	std::memset(pCfg->szServerIP, 0, sizeof(pCfg->szServerIP));
	std::strncpy(pCfg->szServerIP, szServerIp, sizeof(pCfg->szServerIP) - 1);

	std::memset(pCfg->szName, 0, sizeof(pCfg->szName));
	std::strncpy(pCfg->szName, szCharName, sizeof(pCfg->szName) - 1);

	pCfg->bAmazon = (nCharClass == 0) ? 1 : 0;
	pCfg->bSorceress = (nCharClass == 1) ? 1 : 0;
	pCfg->bNecromancer = (nCharClass == 2) ? 1 : 0;
	pCfg->bPaladin = (nCharClass == 3) ? 1 : 0;
	pCfg->bBarbarian = (nCharClass == 4) ? 1 : 0;
	pCfg->bDruid = (nCharClass == 5) ? 1 : 0;
	pCfg->bAssassin = (nCharClass == 6) ? 1 : 0;

	pCfg->unpackedCTemp.nUnk = static_cast<uint8_t>(nCharClass);
	pCfg->unpackedCTemp.nCharacterClassId = static_cast<uint8_t>(nCharClass);
	pCfg->unpackedCTemp.nCharacterSaveFlags = wSaveFlags;
	pCfg->nArenaFlags = dwArenaFlags ? dwArenaFlags : 0x100004u;
	pCfg->nLaunchType = LAUNCHTYPE_TCPIP;
	pCfg->bShownLogo = 1;

	return 1; // MODULE_CLIENT -> load Original112a/D2Client.dll
}

struct D2LaunchShimInterfaceStrc
{
	int32_t(__fastcall* pfMainEntry)(D2ConfigStrc* pCfg);
};

static D2LaunchShimInterfaceStrc gOrigLaunchShimInterface = {
	D2LAUNCH_OrigShimMainEntry
};

extern "C" __declspec(dllexport) D2LaunchShimInterfaceStrc* __stdcall QueryInterface()
{
	return &gOrigLaunchShimInterface;
}
