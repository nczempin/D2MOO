#include "D2Launch.h"

#include <Windows.h>
#include <cstdio>
#include <cstring>
#include <ctime>

#include <Fog.h>
#include <Storm.h>
#include <D2StrTable.h>
#include <D2Gfx.h>
#include <D2Sound.h>
#include <D2MCPClient.h>
#include <D2WinMain.h>
#include <D2WinArchive.h>
#include <D2WinControlHeader.h>
#include <D2WinButton.h>
#include <D2WinEditBox.h>
#include <D2WinTextBox.h>
#include <D2WinImage.h>
#include <D2WinAnimImage.h>
#include <D2WinFont.h>
#include <D2WinPalette.h>
#include <Window.h>

// 1.12a Globals
static D2ConfigStrc* gpD2Config = nullptr;               // 0x6FA65ACC
static int32_t gdwNextModule = D2LAUNCH_MODULE_NONE;     // 0x6FA65AE0
static int32_t gnLaunchMode = 0;                         // 0x6FA65AE4 (0=SinglePlayer, 1=Realm, 2=TCPIP, 3=OpenBNet)
static D2SaveHeaderStrc gD2SaveHeader = {};              // 0x6FA652D0

static D2CellFileStrc* gpCursorCellFile = nullptr;       // 0x6FA654C0
static D2CellFileStrc* gpBgTitleScreen = nullptr;        // 0x6FA654BC
static D2CellFileStrc* gpBgGameSelect = nullptr;         // 0x6FA654B8
static D2CellFileStrc* gpBtnWideBlank = nullptr;         // 0x6FA654DC
static D2CellFileStrc* gpBtnMediumBlank = nullptr;       // 0x6FA654E0
static D2CellFileStrc* gpBtnTallBlank = nullptr;         // 0x6FA654E4
static D2CellFileStrc* gpBtnCancelBlank = nullptr;       // 0x6FA654E8
static D2CellFileStrc* gpLogoBlackLeft = nullptr;        // 0x6FA654C4
static D2CellFileStrc* gpLogoFireLeft = nullptr;         // 0x6FA654C8
static D2CellFileStrc* gpLogoBlackRight = nullptr;       // 0x6FA654CC
static D2CellFileStrc* gpLogoFireRight = nullptr;        // 0x6FA654D0

static constexpr int kMaxLaunchControls = 64;
static D2WinControlStrc* gapLaunchControls[kMaxLaunchControls] = {};
static D2LaunchCharNodeStrc* gpCharListHead = nullptr;
static D2LaunchCharNodeStrc* gpSelectedChar = nullptr;
static uint8_t gnSelectedCreateClass = 0;

static void RegisterControl(D2WinControlStrc* pCtrl)
{
	if (!pCtrl)
	{
		return;
	}
	for (int i = 0; i < kMaxLaunchControls; ++i)
	{
		if (gapLaunchControls[i] == nullptr)
		{
			gapLaunchControls[i] = pCtrl;
			return;
		}
	}
}

// 1.12a: 0x6FA54830
void __fastcall D2LAUNCH_DestroyAllControls()
{
	for (int i = 0; i < kMaxLaunchControls; ++i)
	{
		if (gapLaunchControls[i] != nullptr)
		{
			CONTROL_Destroy(&gapLaunchControls[i]);
			gapLaunchControls[i] = nullptr;
		}
	}
	BACKGROUND_SetCellFile(nullptr);
}

static void FreeCharList()
{
	D2LaunchCharNodeStrc* pCur = gpCharListHead;
	while (pCur)
	{
		D2LaunchCharNodeStrc* pNext = pCur->pNext;
		D2_FREE(pCur);
		pCur = pNext;
	}
	gpCharListHead = nullptr;
	gpSelectedChar = nullptr;
}

// 1.12a: 0x6FA51A90
void __fastcall D2LAUNCH_LoadSprites(BOOL bReload)
{
	if (!bReload)
	{
		gpCursorCellFile = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CURSOR\\ohand", 0);
	}

	const BOOL bExpansion = FOG_IsExpansion();
	gpBgTitleScreen = ARCHIVE_LoadCellFile(
		bExpansion ? "DATA\\GLOBAL\\UI\\FrontEnd\\ExpansionTitleScreen" : "DATA\\GLOBAL\\UI\\FrontEnd\\TitleScreen",
		4);
	gpBgGameSelect = ARCHIVE_LoadCellFile(
		bExpansion ? "DATA\\GLOBAL\\UI\\FrontEnd\\gameselectscreenEXP" : "DATA\\GLOBAL\\UI\\FrontEnd\\gameselectscreen",
		4);
	gpBtnWideBlank = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\WideButtonBlank", 0);
	gpBtnMediumBlank = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\MediumButtonBlank", 0);
	gpBtnTallBlank = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\TallButtonBlank", 0);
	gpBtnCancelBlank = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\CancelButtonBlank", 0);
	gpLogoBlackLeft = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2LogoBlackLeft", 0);
	gpLogoFireLeft = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2LogoFireLeft", 0);
	gpLogoBlackRight = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2LogoBlackRight", 0);
	gpLogoFireRight = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2LogoFireRight", 0);
}

static void FreeCellPtr(D2CellFileStrc** ppCellFile)
{
	if (ppCellFile && *ppCellFile)
	{
		ARCHIVE_FreeCellFile(*ppCellFile);
		*ppCellFile = nullptr;
	}
}

// 1.12a: 0x6FA51700
void __fastcall D2LAUNCH_FreeSprites(BOOL bReload)
{
	if (!bReload)
	{
		CURSOR_SetCellFile(nullptr);
		FreeCellPtr(&gpCursorCellFile);
	}
	FreeCellPtr(&gpBgTitleScreen);
	FreeCellPtr(&gpBgGameSelect);
	FreeCellPtr(&gpBtnWideBlank);
	FreeCellPtr(&gpBtnMediumBlank);
	FreeCellPtr(&gpBtnTallBlank);
	FreeCellPtr(&gpBtnCancelBlank);
	FreeCellPtr(&gpLogoBlackLeft);
	FreeCellPtr(&gpLogoFireLeft);
	FreeCellPtr(&gpLogoBlackRight);
	FreeCellPtr(&gpLogoFireRight);
}

// 1.12a: 0x6FA50CC0
static int __cdecl D2LAUNCH_OnCloseWindow()
{
	gdwNextModule = D2LAUNCH_MODULE_NONE;
	return 1;
}

// 1.12a: 0x6FA50AC0
static void __stdcall D2LAUNCH_IdleCallback(DWORD dwUnused)
{
	(void)dwUnused;
}

// 1.12a: 0x6FA50A50
BOOL __stdcall D2LAUNCH_HasSavedCharacters()
{
	char szSaveDir[MAX_PATH] = {};
	char szPattern[MAX_PATH * 2] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
	std::snprintf(szPattern, sizeof(szPattern), "%s*.d2s", szSaveDir);

	WIN32_FIND_DATAA tFindData = {};
	HANDLE hFind = FindFirstFileA(szPattern, &tFindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	FindClose(hFind);
	return TRUE;
}

// 1.12a: 0x6FA5A2D0
void __fastcall D2LAUNCH_InitNewSaveAndPreview(
	D2CharacterPreviewInfoStrc* pPreviewInfo,
	uint8_t nCharClass,
	const char* szCharName,
	uint16_t wSaveFlags)
{
	static const uint8_t kDefaultAppearance[16] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};

	std::memset(&gD2SaveHeader, 0, sizeof(gD2SaveHeader));
	gD2SaveHeader.dwHeaderMagic = 0xAA55AA55;
	gD2SaveHeader.dwVersion = 0x60;
	gD2SaveHeader.dwFileSize = sizeof(D2SaveHeaderStrc);
	SStrCopy(gD2SaveHeader.szName, szCharName ? szCharName : "", sizeof(gD2SaveHeader.szName));

	uint32_t dwFlags = static_cast<uint32_t>(wSaveFlags) | 0x01u;
	if (nCharClass == 5 || nCharClass == 6)
	{
		dwFlags |= 0x20u;
	}
	gD2SaveHeader.dwSaveFlags = dwFlags;
	gD2SaveHeader.nCharClass = nCharClass;
	gD2SaveHeader.nStatsCount = 0x10;
	gD2SaveHeader.nSkillsCount = 0x1E;
	gD2SaveHeader.nCharLevel = 1;

	const uint32_t dwNow = static_cast<uint32_t>(std::time(nullptr));
	gD2SaveHeader.dwCreateTime = dwNow;
	gD2SaveHeader.dwLastTime = dwNow;
	std::memcpy(gD2SaveHeader.nAppearanceComponents, kDefaultAppearance, sizeof(gD2SaveHeader.nAppearanceComponents));
	std::memset(gD2SaveHeader.nAppearanceColors, 0xFF, sizeof(gD2SaveHeader.nAppearanceColors));

	if (pPreviewInfo)
	{
		uint8_t* pOut = reinterpret_cast<uint8_t*>(pPreviewInfo);
		FOG_Encode14BitsToString(reinterpret_cast<uint16_t*>(pOut), 0x0C);
		pOut += 2;
		for (int i = 0; i < 11; ++i)
		{
			*pOut++ = kDefaultAppearance[i];
		}
		*pOut++ = static_cast<uint8_t>(nCharClass + 1);
		std::memset(pOut, 0xFF, 11);
		pOut += 11;
		*pOut++ = 1;
		FOG_Encode14BitsToString(reinterpret_cast<uint16_t*>(pOut), static_cast<uint16_t>(dwFlags));
		pOut += 2;
		FOG_Encode14BitsToString(reinterpret_cast<uint16_t*>(pOut), 0);
		pOut += 2;
		pOut[0] = 0xFF;
		pOut[1] = 0xFF;
		pOut[2] = 0xFF;
		pOut[3] = 0;
	}
}

// 1.12a: 0x6FA5A210
BOOL __stdcall D2LAUNCH_WriteNewSaveFile(const char* szCharName)
{
	if (!szCharName || !szCharName[0])
	{
		return FALSE;
	}

	char szSaveDir[0x400] = {};
	char szFilePath[0x200] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
	std::snprintf(szFilePath, sizeof(szFilePath), "%s%s.d2s", szSaveDir, szCharName);

	HANDLE hFile = FOG_CreateFileA(
		szFilePath,
		GENERIC_WRITE,
		0,
		nullptr,
		CREATE_ALWAYS,
		FILE_FLAG_SEQUENTIAL_SCAN,
		nullptr);
	if (!hFile || hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	gD2SaveHeader.dwChecksum = FOG_ComputeChecksum(&gD2SaveHeader, sizeof(gD2SaveHeader));
	DWORD dwBytesWritten = 0;
	FOG_WriteFile(hFile, &gD2SaveHeader, sizeof(gD2SaveHeader), &dwBytesWritten, nullptr);
	FOG_CloseFile(hFile);
	return TRUE;
}

// 1.12a: 0x6FA594F0
static BOOL __stdcall D2LAUNCH_EnterSelectedCharacter(
	const char* szCharName,
	const char* szRealmName,
	uint16_t wSaveFlags)
{
	if (!gpD2Config || !szCharName)
	{
		return FALSE;
	}

	SStrCopy(gpD2Config->szName, szCharName, sizeof(gpD2Config->szName));
	SStrCopy(gpD2Config->szRealm, szRealmName ? szRealmName : "", sizeof(gpD2Config->szRealm));
	gpD2Config->unpackedCTemp.nCharacterSaveFlags = wSaveFlags;

	if (gnLaunchMode == 0)
	{
		gpD2Config->nLaunchType = LAUNCHTYPE_LOCAL;
		gpD2Config->dwGameType = 0;
	}
	else if (gnLaunchMode == 2)
	{
		gpD2Config->nLaunchType = LAUNCHTYPE_TCPIP;
	}
	else if (gnLaunchMode == 3)
	{
		gpD2Config->nLaunchType = LAUNCHTYPE_OPENBNET;
		gpD2Config->nArenaFlags = (wSaveFlags & 0x04) ? 0x804u : 0x004u;
		if (wSaveFlags & 0x20)
		{
			gpD2Config->nArenaFlags |= 0x100000u;
		}
		gdwNextModule = D2LAUNCH_MODULE_MULTIPLAYER;
		D2Win_10009();
		return TRUE;
	}

	uint32_t dwArenaFlags = (wSaveFlags & 0x04) ? 0x804u : 0x004u;
	if (wSaveFlags & 0x20)
	{
		dwArenaFlags |= 0x100000u;
	}
	gpD2Config->nArenaFlags = dwArenaFlags;
	gdwNextModule = D2LAUNCH_MODULE_CLIENT;
	D2Win_10009();
	return TRUE;
}

// 1.12a: 0x6FA59B90
static int __stdcall D2LAUNCH_OnCreateCharConfirm(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (!gpD2Config)
	{
		return 0;
	}

	if (!gpD2Config->szName[0])
	{
		SStrCopy(gpD2Config->szName, "Hero", sizeof(gpD2Config->szName));
	}

	if (FOG_IsExpansion())
	{
		gpD2Config->unpackedCTemp.nCharacterSaveFlags |= 0x20;
	}
	gpD2Config->unpackedCTemp.nCharacterClassId = gnSelectedCreateClass;

	D2LAUNCH_InitNewSaveAndPreview(
		&gpD2Config->tCharPreviewInfo,
		gpD2Config->unpackedCTemp.nCharacterClassId,
		gpD2Config->szName,
		gpD2Config->unpackedCTemp.nCharacterSaveFlags);
	D2LAUNCH_WriteNewSaveFile(gpD2Config->szName);

	const uint16_t wFlags = gpD2Config->unpackedCTemp.nCharacterSaveFlags;
	uint32_t dwArenaFlags = (wFlags & 0x04) ? 0x804u : 0x004u;
	if (wFlags & 0x20)
	{
		dwArenaFlags |= 0x100000u;
	}
	gpD2Config->nArenaFlags = dwArenaFlags;

	if (gnLaunchMode == 3)
	{
		gpD2Config->nLaunchType = LAUNCHTYPE_OPENBNET;
		gdwNextModule = D2LAUNCH_MODULE_MULTIPLAYER;
	}
	else if (gnLaunchMode == 2)
	{
		gpD2Config->nLaunchType = LAUNCHTYPE_TCPIP;
		gdwNextModule = D2LAUNCH_MODULE_CLIENT;
	}
	else
	{
		gpD2Config->nLaunchType = LAUNCHTYPE_LOCAL;
		gpD2Config->dwGameType = 0;
		gdwNextModule = D2LAUNCH_MODULE_CLIENT;
	}

	D2Win_10009();
	return 1;
}

static int __stdcall D2LAUNCH_OnCancelToMainMenu(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_EnterMainMenu(0);
	return 1;
}

static int __stdcall D2LAUNCH_OnCharSelectOk(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (gpSelectedChar && gpD2Config)
	{
		gpD2Config->tCharPreviewInfo = gpSelectedChar->tPreview;
		D2LAUNCH_EnterSelectedCharacter(gpSelectedChar->szCharName, "", gpSelectedChar->wSaveFlags);
		return 1;
	}
	return 0;
}

static int __stdcall D2LAUNCH_OnCharSelectCreateNew(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_EnterCharCreate();
	return 1;
}

// 1.12a: 0x6FA59A70
static int __stdcall D2LAUNCH_OnSinglePlayerClick(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	gnLaunchMode = 0;
	D2LAUNCH_DestroyAllControls();
	if (D2LAUNCH_HasSavedCharacters())
	{
		D2LAUNCH_EnterCharSelect();
	}
	else
	{
		D2LAUNCH_EnterCharCreate();
	}
	return 1;
}

// 1.12a: 0x6FA58230
static int __stdcall D2LAUNCH_OnBattleNetClick(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	gnLaunchMode = 1;
	if (gpD2Config)
	{
		gpD2Config->nLaunchType = LAUNCHTYPE_REALM;
	}
	gdwNextModule = D2LAUNCH_MODULE_MULTIPLAYER;
	D2Win_10009();
	return 1;
}

// 1.12a: 0x6FA56820
static int __stdcall D2LAUNCH_OnOtherMultiplayerClick(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	gnLaunchMode = 2;
	D2LAUNCH_DestroyAllControls();
	if (D2LAUNCH_HasSavedCharacters())
	{
		D2LAUNCH_EnterCharSelect();
	}
	else
	{
		D2LAUNCH_EnterCharCreate();
	}
	return 1;
}

// 1.12a: 0x6FA506D0
static int __stdcall D2LAUNCH_OnExitClick(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	gdwNextModule = D2LAUNCH_MODULE_NONE;
	D2SOUND_FastPlaySound("data\\global\\sfx\\cursor\\button.wav");
	D2Win_10009();
	return 1;
}

static int __stdcall D2LAUNCH_OnLogoClick(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (gpD2Config)
	{
		gpD2Config->bShownLogo = 1;
	}
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_EnterMainMenu(0);
	return 1;
}

// 1.12a: 0x6FA58150
void __stdcall D2LAUNCH_EnterLogoScreen()
{
	D2LAUNCH_DestroyAllControls();
	BACKGROUND_SetCellFile(gpBgTitleScreen);

	D2WinControlInitStrc tBtnInit = {};
	tBtnInit.nType = D2WIN_BUTTON;
	tBtnInit.nX = 0;
	tBtnInit.nY = 0;
	tBtnInit.nWidth = 800;
	tBtnInit.nHeight = 600;
	tBtnInit.field_14 = 0x20;
	tBtnInit.nStringId = 0;
	tBtnInit.ppCellFile = nullptr;
	tBtnInit.field_20 = D2LAUNCH_OnLogoClick;
	RegisterControl(CONTROL_Create(&tBtnInit));
}

// 1.12a: 0x6FA568C0
void __stdcall D2LAUNCH_EnterMainMenu(int32_t nUnused)
{
	(void)nUnused;
	D2LAUNCH_DestroyAllControls();
	BACKGROUND_SetCellFile(gpBgGameSelect);

	D2WinControlInitStrc tBtn = {};
	tBtn.nType = D2WIN_BUTTON;
	tBtn.nX = 264;
	tBtn.nWidth = 272;
	tBtn.nHeight = 35;
	tBtn.ppCellFile = &gpBtnWideBlank;

	// Single Player (5106)
	tBtn.nY = 290;
	tBtn.nStringId = 5106;
	tBtn.field_20 = D2LAUNCH_OnSinglePlayerClick;
	RegisterControl(CONTROL_Create(&tBtn));

	// Battle.net (5107)
	tBtn.nY = 330;
	tBtn.nStringId = 5107;
	tBtn.field_20 = D2LAUNCH_OnBattleNetClick;
	RegisterControl(CONTROL_Create(&tBtn));

	// Other Multiplayer (5108)
	tBtn.nY = 370;
	tBtn.nStringId = 5108;
	tBtn.field_20 = D2LAUNCH_OnOtherMultiplayerClick;
	RegisterControl(CONTROL_Create(&tBtn));

	// Exit Diablo II (5109)
	tBtn.nY = 500;
	tBtn.nStringId = 5109;
	tBtn.field_20 = D2LAUNCH_OnExitClick;
	RegisterControl(CONTROL_Create(&tBtn));
}

static void EnumerateSavedCharacters()
{
	FreeCharList();

	char szSaveDir[MAX_PATH] = {};
	char szPattern[MAX_PATH * 2] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
	std::snprintf(szPattern, sizeof(szPattern), "%s*.d2s", szSaveDir);

	WIN32_FIND_DATAA tFind = {};
	HANDLE hFind = FindFirstFileA(szPattern, &tFind);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		char szFullPath[MAX_PATH * 2] = {};
		std::snprintf(szFullPath, sizeof(szFullPath), "%s%s", szSaveDir, tFind.cFileName);
		HANDLE hFile = FOG_CreateFileA(
			szFullPath,
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			nullptr);
		if (hFile && hFile != INVALID_HANDLE_VALUE)
		{
			D2SaveHeaderStrc tHdr = {};
			DWORD dwRead = 0;
			if (FOG_ReadFile(hFile, &tHdr, sizeof(tHdr), &dwRead, nullptr) &&
				dwRead >= 0x88 &&
				tHdr.dwHeaderMagic == 0xAA55AA55 &&
				tHdr.dwVersion >= 0x47 &&
				tHdr.dwVersion <= 0x60)
			{
				D2LaunchCharNodeStrc* pNode = static_cast<D2LaunchCharNodeStrc*>(D2_CALLOC(sizeof(D2LaunchCharNodeStrc)));
				if (pNode)
				{
					SStrCopy(pNode->szCharName, tHdr.szName, sizeof(pNode->szCharName));
					pNode->wSaveFlags = static_cast<uint16_t>(tHdr.dwSaveFlags);
					pNode->nCharClass = tHdr.nCharClass;
					pNode->nCharLevel = tHdr.nCharLevel ? tHdr.nCharLevel : 1;
					D2LAUNCH_InitNewSaveAndPreview(
						&pNode->tPreview,
						pNode->nCharClass,
						pNode->szCharName,
						pNode->wSaveFlags);
					pNode->tPreview.nLevel = pNode->nCharLevel;
					pNode->pNext = gpCharListHead;
					gpCharListHead = pNode;
					if (!gpSelectedChar)
					{
						gpSelectedChar = pNode;
					}
				}
			}
			FOG_CloseFile(hFile);
		}
	} while (FindNextFileA(hFind, &tFind));

	FindClose(hFind);
}

// 1.12a: 0x6FA4EAE0
BOOL __stdcall D2LAUNCH_EnterCharSelect()
{
	D2LAUNCH_DestroyAllControls();
	BACKGROUND_SetCellFile(gpBgGameSelect);
	EnumerateSavedCharacters();

	D2WinControlInitStrc tBtn = {};
	tBtn.nType = D2WIN_BUTTON;

	// Create New Character
	tBtn.nX = 33;
	tBtn.nY = 528;
	tBtn.nWidth = 168;
	tBtn.nHeight = 60;
	tBtn.nStringId = 5266;
	tBtn.ppCellFile = &gpBtnTallBlank;
	tBtn.field_20 = D2LAUNCH_OnCharSelectCreateNew;
	RegisterControl(CONTROL_Create(&tBtn));

	// Exit / Cancel
	tBtn.nX = 33;
	tBtn.nY = 568;
	tBtn.nWidth = 168;
	tBtn.nHeight = 35;
	tBtn.nStringId = 5103;
	tBtn.ppCellFile = &gpBtnMediumBlank;
	tBtn.field_20 = D2LAUNCH_OnCancelToMainMenu;
	RegisterControl(CONTROL_Create(&tBtn));

	// OK
	tBtn.nX = 609;
	tBtn.nY = 568;
	tBtn.nWidth = 168;
	tBtn.nHeight = 35;
	tBtn.nStringId = 5102;
	tBtn.ppCellFile = &gpBtnMediumBlank;
	tBtn.field_20 = D2LAUNCH_OnCharSelectOk;
	RegisterControl(CONTROL_Create(&tBtn));

	return TRUE;
}

// 1.12a: 0x6FA55980
BOOL __stdcall D2LAUNCH_EnterCharCreate()
{
	D2LAUNCH_DestroyAllControls();
	BACKGROUND_SetCellFile(gpBgGameSelect);
	gnSelectedCreateClass = 0;

	D2WinControlInitStrc tBtn = {};
	tBtn.nType = D2WIN_BUTTON;

	// Exit / Cancel
	tBtn.nX = 33;
	tBtn.nY = 568;
	tBtn.nWidth = 168;
	tBtn.nHeight = 35;
	tBtn.nStringId = 5103;
	tBtn.ppCellFile = &gpBtnMediumBlank;
	tBtn.field_20 = D2LAUNCH_OnCancelToMainMenu;
	RegisterControl(CONTROL_Create(&tBtn));

	// OK (Create Character)
	tBtn.nX = 609;
	tBtn.nY = 568;
	tBtn.nWidth = 168;
	tBtn.nHeight = 35;
	tBtn.nStringId = 5102;
	tBtn.ppCellFile = &gpBtnMediumBlank;
	tBtn.field_20 = D2LAUNCH_OnCreateCharConfirm;
	RegisterControl(CONTROL_Create(&tBtn));

	return TRUE;
}

// 1.12a: 0x6FA58730
int32_t __fastcall D2LAUNCH_MainEntry(D2ConfigStrc* pCfg)
{
	if (!pCfg)
	{
		return D2LAUNCH_MODULE_NONE;
	}

	gpD2Config = pCfg;
	gdwNextModule = D2LAUNCH_MODULE_NONE;

	D2Win_10015();
	(void)D2Win_GetArchive();

	char szFolder[16] = {};
	STRTABLE_Initialize(0, szFolder, FOG_IsExpansion());

	D2LAUNCH_LoadSprites(FALSE);
	CURSOR_SetCellFile(gpCursorCellFile);

	pCfg->bUnk035A = 0;
	pCfg->nComponents[0] = 0;
	pCfg->dwGameType = 0;
	pCfg->szGamePassword[0] = '\0';
	if (gnLaunchMode != 2)
	{
		pCfg->szServerIP[0] = '\0';
	}

	D2SOUND_FastInit();
	D2SOUND_FastCacheSound("data\\global\\sfx\\cursor\\button.wav");

	D2Win_10005(D2GAMERES_800x600);

	if (pCfg->bSkipToBNet && !pCfg->bShownLogo)
	{
		pCfg->bShownLogo = 1;
		pCfg->nLaunchType = LAUNCHTYPE_REALM;
		gdwNextModule = D2LAUNCH_MODULE_MULTIPLAYER;
	}
	else if (!pCfg->bShownLogo)
	{
		D2LAUNCH_EnterLogoScreen();
	}
	else if (pCfg->nLaunchType == LAUNCHTYPE_LOCAL)
	{
		gnLaunchMode = 0;
		D2LAUNCH_EnterCharSelect();
	}
	else
	{
		D2LAUNCH_EnterMainMenu(0);
	}

	if (gdwNextModule == D2LAUNCH_MODULE_NONE)
	{
		D2Win_10010(D2LAUNCH_OnCloseWindow);
		D2Win_10008(D2LAUNCH_IdleCallback);
	}

	D2SOUND_FastFadeAll();
	D2SOUND_FastShutdown();
	FreeCharList();
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_FreeSprites(FALSE);
	STRTABLE_Destroy();
	D2Win_10016();

	return gdwNextModule;
}

static D2LaunchInterfaceStrc gD2LaunchInterface = {
	D2LAUNCH_MainEntry
};

// 1.12a @1: 0x6FA5A420
extern "C" D2LAUNCH_DLL_DECL D2LaunchInterfaceStrc* __stdcall QueryInterface()
{
	return &gD2LaunchInterface;
}
