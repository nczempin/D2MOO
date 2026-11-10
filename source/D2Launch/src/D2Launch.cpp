#include "D2Launch.h"

#include <Windows.h>
#include <cstdio>
#include <cstring>
#include <ctime>

#include <Fog.h>
#include <Storm.h>
#include <D2Lang.h>
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
#include <D2WinComp.h>
#include <D2WinFont.h>
#include <D2WinPalette.h>
#include <Window.h>

// 1.12a Font+Color style descriptors (0x6FA60BD8..0x6FA60C10)
struct D2FontStyleStrc
{
	Font eFont;
	int32_t nColor;
};

static D2FontStyleStrc gFontStyle42 = { D2FONT_FONT42, 0 };             // 0x6FA60BD8
static D2FontStyleStrc gFontStyle30 = { D2FONT_FONT30, 0 };             // 0x6FA60BE0
static D2FontStyleStrc gFontStyle24 = { D2FONT_FONT24, 0 };             // 0x6FA60BE8
static D2FontStyleStrc gFontStyle16 = { D2FONT_FONT16, 0 };             // 0x6FA60BF0
static D2FontStyleStrc gFontStyleFormal12 = { D2FONT_FONTFORMAL12, 0 }; // 0x6FA60C08

// 1.12a Globals
static D2ConfigStrc* gpD2Config = nullptr;               // 0x6FA65ACC
static int32_t gdwNextModule = D2LAUNCH_MODULE_NONE;     // 0x6FA65AE0
static int32_t gnLaunchMode = 0;                         // 0x6FA65AE4 (0=SinglePlayer, 1=Realm, 2=TCPIP, 3=OpenBNet)
static D2SaveHeaderStrc gD2SaveHeader = {};              // 0x6FA652D0

// Front-End & Character Select/Create CellFiles (0x6FA654B8..0x6FA65604)
static D2CellFileStrc* gpCursorCellFile = nullptr;       // 0x6FA654C0
static D2CellFileStrc* gpBgTitleScreen = nullptr;        // 0x6FA654BC
static D2CellFileStrc* gpBgGameSelect = nullptr;         // 0x6FA654B8
static D2CellFileStrc* gpBgCharSelect = nullptr;         // 0x6FA65504
static D2CellFileStrc* gpBgCharCreate = nullptr;         // 0x6FA65508
static D2CellFileStrc* gpBtnWideBlank = nullptr;         // 0x6FA654DC
static D2CellFileStrc* gpBtnMediumBlank = nullptr;       // 0x6FA654E0
static D2CellFileStrc* gpBtnMediumSelBlank = nullptr;    // 0x6FA655F8
static D2CellFileStrc* gpBtnTallBlank = nullptr;         // 0x6FA654E4
static D2CellFileStrc* gpBtnCancelBlank = nullptr;       // 0x6FA654E8
static D2CellFileStrc* gpBtnClickBox = nullptr;          // 0x6FA65604
static D2CellFileStrc* gpImgTextBox = nullptr;           // 0x6FA65510
static D2CellFileStrc* gpImgTextBox2 = nullptr;          // 0x6FA654F4
static D2CellFileStrc* gpCharSelectBox = nullptr;        // 0x6FA655FC
static D2CellFileStrc* gpCharSelectBoxGrey = nullptr;    // 0x6FA65600
static D2CellFileStrc* gpLogoBlackLeft = nullptr;        // 0x6FA654C4
static D2CellFileStrc* gpLogoFireLeft = nullptr;         // 0x6FA654C8
static D2CellFileStrc* gpLogoBlackRight = nullptr;       // 0x6FA654CC
static D2CellFileStrc* gpLogoFireRight = nullptr;        // 0x6FA654D0
static D2CellFileStrc* gpCampfire = nullptr;             // 0x6FA654D4

// 7 Character Classes around the Campfire (0=Amazon, 1=Sorceress, 2=Necromancer, 3=Paladin, 4=Barbarian, 5=Druid, 6=Assassin)
static D2CellFileStrc* gpClassNu1[7] = {};               // 0x6FA65514..0x6FA6552C
static D2CellFileStrc* gpClassNu2[7] = {};               // 0x6FA65530..0x6FA65548
static D2CellFileStrc* gpClassNu3[7] = {};               // 0x6FA6554C..0x6FA65564
static D2CellFileStrc* gpClassNu3s[7] = {};              // 0x6FA65568..0x6FA65580
static D2CellFileStrc* gpClassFw[7] = {};                // 0x6FA65584..0x6FA6559C
static D2CellFileStrc* gpClassFws[7] = {};               // 0x6FA655A0..0x6FA655B8
static D2CellFileStrc* gpClassBw[7] = {};                // 0x6FA655BC..0x6FA655D4
static D2CellFileStrc* gpClassBws[7] = {};               // 0x6FA655D8..0x6FA655F0

// 1.12a: 0x6FA60E50 / 0x6FA60EA0
static D2AnimatedImageDescriptor gAnimLogoLeft[5] = {
	{ &gpLogoBlackLeft, &gpLogoFireLeft, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackLeft, &gpLogoFireLeft, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackLeft, &gpLogoFireLeft, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackLeft, &gpLogoFireLeft, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackLeft, &gpLogoFireLeft, DRAWMODE_MODULATE, 1 },
};
static D2AnimatedImageDescriptor gAnimLogoRight[5] = {
	{ &gpLogoBlackRight, &gpLogoFireRight, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackRight, &gpLogoFireRight, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackRight, &gpLogoFireRight, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackRight, &gpLogoFireRight, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackRight, &gpLogoFireRight, DRAWMODE_MODULATE, 1 },
};

// 1.12a: 0x6FA60C20..0x6FA60E40 (5-state animation descriptors per class: nu1, nu2, fw, nu3, bw)
static D2AnimatedImageDescriptor gAnimCharClass[7][5] = {
	// 0: Amazon (0x6FA60C20)
	{
		{ &gpClassNu1[0], nullptr,          DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[0], nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[0],  &gpClassFws[0],   DRAWMODE_MODULATE, 1 },
		{ &gpClassNu3[0], &gpClassNu3s[0],  DRAWMODE_MODULATE, 1 },
		{ &gpClassBw[0],  nullptr,          DRAWMODE_NORMAL,   1 },
	},
	// 1: Sorceress (0x6FA60C70)
	{
		{ &gpClassNu1[1], nullptr,          DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[1], nullptr,          DRAWMODE_NORMAL,   2 },
		{ &gpClassFw[1],  &gpClassFws[1],   DRAWMODE_MODULATE, 1 },
		{ &gpClassNu3[1], &gpClassNu3s[1],  DRAWMODE_MODULATE, 1 },
		{ &gpClassBw[1],  &gpClassBws[1],   DRAWMODE_MODULATE, 1 },
	},
	// 2: Necromancer (0x6FA60CC0)
	{
		{ &gpClassNu1[2], nullptr,          DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[2], nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[2],  &gpClassFws[2],   DRAWMODE_MODULATE, 1 },
		{ &gpClassNu3[2], &gpClassNu3s[2],  DRAWMODE_MODULATE, 1 },
		{ &gpClassBw[2],  &gpClassBws[2],   DRAWMODE_MODULATE, 1 },
	},
	// 3: Paladin (0x6FA60D10)
	{
		{ &gpClassNu1[3], nullptr,          DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[3], nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[3],  &gpClassFws[3],   DRAWMODE_MODULATE, 1 },
		{ &gpClassNu3[3], nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassBw[3],  nullptr,          DRAWMODE_NORMAL,   1 },
	},
	// 4: Barbarian (0x6FA60D60)
	{
		{ &gpClassNu1[4], nullptr,          DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[4], nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[4],  &gpClassFws[4],   DRAWMODE_NORMAL,   1 },
		{ &gpClassNu3[4], nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassBw[4],  nullptr,          DRAWMODE_NORMAL,   1 },
	},
	// 5: Druid (0x6FA60DB0)
	{
		{ &gpClassNu1[5], nullptr,          DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[5], nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[5],  nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassNu3[5], nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassBw[5],  nullptr,          DRAWMODE_NORMAL,   1 },
	},
	// 6: Assassin (0x6FA60E00)
	{
		{ &gpClassNu1[6], nullptr,          DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[6], nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[6],  nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassNu3[6], nullptr,          DRAWMODE_NORMAL,   1 },
		{ &gpClassBw[6],  nullptr,          DRAWMODE_NORMAL,   1 },
	},
};

static const short kClassNameTblIds[7] = { 4011, 4010, 4009, 4008, 4007, 22518, 22519 };
static const short kClassDescTblIds[7] = { 5128, 5131, 5129, 5132, 5130, 22520, 22521 };
static const char* const kClassFallbackNames[7] = {
	"Amazon", "Sorceress", "Necromancer", "Paladin", "Barbarian", "Druid", "Assassin"
};

static constexpr int kMaxLaunchControls = 96;
static D2WinControlStrc* gapLaunchControls[kMaxLaunchControls] = {};
static D2LaunchCharNodeStrc* gpCharListHead = nullptr;
static D2LaunchCharNodeStrc* gpSelectedChar = nullptr;

// Character Select screen state (8 visible slots: 4 rows x 2 columns)
static D2WinTextBoxStrc* gpCharSelectTitleBox = nullptr;
static D2WinImageStrc* gpCharSlotFrameImg[8] = {};
static D2WinTextBoxStrc* gpCharSlotTextBox[8] = {};
static D2CompositeUnitStrc* gpCharSlotCompUnit[8] = {};
static D2LaunchCharNodeStrc* gpCharSlotNode[8] = {};
static D2WinButtonStrc* gpBtnConvertExp = nullptr;
static D2WinButtonStrc* gpBtnDeleteChar = nullptr;
static D2WinButtonStrc* gpBtnCharSelectOk = nullptr;
static D2WinButtonStrc* gpBtnClientModeToggle = nullptr;
// 0 = TS Web Client, 1 = Original 1.12a Binary Client, 2 = Native C++ Client
static int32_t gnClientMode = 2;
static bool gbClientModeInitialized = false;
static int32_t gnSelectedSlotIndex = -1;
static DWORD gdwLastSlotClickTick = 0;

static void SyncClientModeEnv()
{
	if (!gbClientModeInitialized)
	{
		char szNativeEnv[16] = {};
		char szOrigEnv[16] = {};
		char szTsEnv[16] = {};
		char szAutotestEnv[16] = {};
		const char* szCmdLine = GetCommandLineA();
		if ((GetEnvironmentVariableA("D2MOO_USE_NATIVE_CLIENT", szNativeEnv, sizeof(szNativeEnv)) > 0 && szNativeEnv[0] != '0') ||
			(GetEnvironmentVariableA("D2MOO_NATIVE_AUTOTEST", szAutotestEnv, sizeof(szAutotestEnv)) > 0 && szAutotestEnv[0] != '0') ||
			(szCmdLine && std::strstr(szCmdLine, "--autotest-native") != nullptr))
		{
			gnClientMode = 2;
		}
		else if (GetEnvironmentVariableA("D2MOO_USE_ORIGINAL_CLIENT", szOrigEnv, sizeof(szOrigEnv)) > 0 && szOrigEnv[0] != '0')
		{
			gnClientMode = 1;
		}
		else if (GetEnvironmentVariableA("D2MOO_USE_TS_CLIENT", szTsEnv, sizeof(szTsEnv)) > 0 && szTsEnv[0] != '0')
		{
			gnClientMode = 0;
		}
		else
		{
			gnClientMode = 2;
		}
		gbClientModeInitialized = true;
	}

	SetEnvironmentVariableA("D2MOO_USE_TS_CLIENT", (gnClientMode == 0) ? "1" : "0");
	SetEnvironmentVariableA("D2MOO_USE_ORIGINAL_CLIENT", (gnClientMode == 1) ? "1" : "0");
	SetEnvironmentVariableA("D2MOO_USE_NATIVE_CLIENT", (gnClientMode == 2) ? "1" : "0");
}

static void RefreshClientModeButtonText()
{
	if (!gpBtnClientModeToggle)
	{
		return;
	}
	const char* szLabel = "CLIENT: TS WEB";
	if (gnClientMode == 1)
	{
		szLabel = "CLIENT: ORIGINAL 1.12A";
	}
	else if (gnClientMode == 2)
	{
		szLabel = "CLIENT: NATIVE C++";
	}
	Unicode wszLabel[64] = {};
	Unicode::win2Unicode(wszLabel, szLabel, 64);
	BUTTON_SetText(gpBtnClientModeToggle, D2FONT_FONTEXOCET10, wszLabel, 0);
}

static int __stdcall D2LAUNCH_OnToggleClientMode(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	gnClientMode = (gnClientMode + 1) % 3;
	SyncClientModeEnv();
	RefreshClientModeButtonText();
	return 1;
}

// Character Create screen state
static int32_t gnSelectedCreateClass = -1;
static D2WinAnimImageStrc* gpCreateClassAnim[7] = {};
static D2WinTextBoxStrc* gpCreateTopTitleBox = nullptr;
static D2WinTextBoxStrc* gpCreateClassNameBox = nullptr;
static D2WinTextBoxStrc* gpCreateClassDescBox = nullptr;
static D2WinTextBoxStrc* gpCreateNameLabelBox = nullptr;
static D2WinImageStrc* gpCreateNameFrameImg = nullptr;
static D2WinEditBoxStrc* gpCreateNameEditBox = nullptr;
static D2WinButtonStrc* gpCreateExpCheckBox = nullptr;
static D2WinTextBoxStrc* gpCreateExpLabelBox = nullptr;
static D2WinButtonStrc* gpCreateHcCheckBox = nullptr;
static D2WinTextBoxStrc* gpCreateHcLabelBox = nullptr;
static D2WinButtonStrc* gpCreateOkBtn = nullptr;

// 1.12a: 0x6FA51670 (Load Front-End Palette)
static void D2LAUNCH_SetSkyPalette()
{
	D2Win_10028("DATA\\GLOBAL\\palette\\sky\\pal.dat", "DATA\\GLOBAL\\palette\\sky\\pal.pl2");
}

static void D2LAUNCH_SetFeCharPalette()
{
	D2Win_10028("DATA\\GLOBAL\\palette\\fechar\\pal.dat", "DATA\\GLOBAL\\palette\\fechar\\pal.pl2");
}

static D2WinControlStrc* RegisterControl(D2WinControlStrc* pCtrl)
{
	if (!pCtrl)
	{
		return nullptr;
	}
	for (int i = 0; i < kMaxLaunchControls; ++i)
	{
		if (gapLaunchControls[i] == nullptr)
		{
			gapLaunchControls[i] = pCtrl;
			return pCtrl;
		}
	}
	return pCtrl;
}

static void SetControlVisibleAndEnabled(D2WinControlStrc* pCtrl, int bVisible)
{
	if (!pCtrl)
	{
		return;
	}
	D2Win_10025_CONTROL_ToggleFirstFlag(pCtrl, bVisible);
	D2Win_10027_CONTROL_ToggleThirdFlag(pCtrl, bVisible);
}

// 1.12a: 0x6FA54830
void __fastcall D2LAUNCH_DestroyAllControls()
{
	D2Win_10146();
	for (int i = 0; i < kMaxLaunchControls; ++i)
	{
		if (gapLaunchControls[i] != nullptr)
		{
			CONTROL_Destroy(&gapLaunchControls[i]);
			gapLaunchControls[i] = nullptr;
		}
	}
	gpCharSelectTitleBox = nullptr;
	std::memset(gpCharSlotFrameImg, 0, sizeof(gpCharSlotFrameImg));
	std::memset(gpCharSlotTextBox, 0, sizeof(gpCharSlotTextBox));
	std::memset(gpCharSlotCompUnit, 0, sizeof(gpCharSlotCompUnit));
	std::memset(gpCharSlotNode, 0, sizeof(gpCharSlotNode));
	gpBtnConvertExp = nullptr;
	gpBtnDeleteChar = nullptr;
	gpBtnCharSelectOk = nullptr;
	gpBtnClientModeToggle = nullptr;
	gnSelectedSlotIndex = -1;

	std::memset(gpCreateClassAnim, 0, sizeof(gpCreateClassAnim));
	gpCreateTopTitleBox = nullptr;
	gpCreateClassNameBox = nullptr;
	gpCreateClassDescBox = nullptr;
	gpCreateNameLabelBox = nullptr;
	gpCreateNameFrameImg = nullptr;
	gpCreateNameEditBox = nullptr;
	gpCreateExpCheckBox = nullptr;
	gpCreateExpLabelBox = nullptr;
	gpCreateHcCheckBox = nullptr;
	gpCreateHcLabelBox = nullptr;
	gpCreateOkBtn = nullptr;

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
		bExpansion ? "DATA\\GLOBAL\\UI\\FrontEnd\\trademarkscreenEXP" : "DATA\\GLOBAL\\UI\\FrontEnd\\trademark",
		0);
	gpBgGameSelect = ARCHIVE_LoadCellFile(
		bExpansion ? "DATA\\GLOBAL\\UI\\FrontEnd\\gameselectscreenEXP" : "DATA\\GLOBAL\\UI\\FrontEnd\\TitleScreen",
		0);
	gpBgCharSelect = ARCHIVE_LoadCellFile(
		bExpansion ? "DATA\\GLOBAL\\UI\\CharSelect\\characterselectscreenEXP" : "DATA\\GLOBAL\\UI\\CharSelect\\charselectbckg",
		0);
	gpBgCharCreate = ARCHIVE_LoadCellFile(
		bExpansion ? "DATA\\GLOBAL\\UI\\FrontEnd\\charactercreationscreenEXP" : "DATA\\GLOBAL\\UI\\CharSelect\\CharacterCreate",
		0);

	gpBtnWideBlank = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\WideButtonBlank", 0);
	gpBtnMediumBlank = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\MediumButtonBlank", 0);
	gpBtnMediumSelBlank = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\MediumSelButtonBlank", 0);
	gpBtnTallBlank = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\TallButtonBlank", 0);
	gpBtnCancelBlank = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\CancelButtonBlank", 0);
	gpBtnClickBox = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\ClickBox", 0);
	gpImgTextBox = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\textbox", 0);
	gpImgTextBox2 = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\textbox2", 0);
	gpCharSelectBox = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\charselectbox", 0);
	gpCharSelectBoxGrey = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\charselectboxgrey", 0);

	gpLogoBlackLeft = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2LogoBlackLeft", 0);
	gpLogoFireLeft = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2LogoFireLeft", 0);
	gpLogoBlackRight = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2LogoBlackRight", 0);
	gpLogoFireRight = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2LogoFireRight", 0);
	gpCampfire = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\fire", 0);

	// 0: Amazon
	gpClassNu1[0] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amnu1", 0);
	gpClassNu2[0] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amnu2", 0);
	gpClassNu3[0] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amnu3", 0);
	gpClassNu3s[0] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amnu3s", 0);
	gpClassFw[0] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amfw", 0);
	gpClassFws[0] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amfws", 0);
	gpClassBw[0] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\ambw", 0);

	// 1: Sorceress
	gpClassNu1[1] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sonu1", 0);
	gpClassNu2[1] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sonu2", 0);
	gpClassNu3[1] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sonu3", 0);
	gpClassNu3s[1] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sonu3s", 0);
	gpClassFw[1] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sofw", 0);
	gpClassFws[1] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sofws", 0);
	gpClassBw[1] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sobw", 0);
	gpClassBws[1] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sobws", 0);

	// 2: Necromancer
	gpClassNu1[2] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nenu1", 0);
	gpClassNu2[2] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nenu2", 0);
	gpClassNu3[2] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nenu3", 0);
	gpClassNu3s[2] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nenu3s", 0);
	gpClassFw[2] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nefw", 0);
	gpClassFws[2] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nefws", 0);
	gpClassBw[2] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nebw", 0);
	gpClassBws[2] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nebws", 0);

	// 3: Paladin
	gpClassNu1[3] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\panu1", 0);
	gpClassNu2[3] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\panu2", 0);
	gpClassNu3[3] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\panu3", 0);
	gpClassFw[3] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\pafw", 0);
	gpClassFws[3] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\pafws", 0);
	gpClassBw[3] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\pabw", 0);

	// 4: Barbarian
	gpClassNu1[4] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\banu1", 0);
	gpClassNu2[4] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\banu2", 0);
	gpClassNu3[4] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\banu3", 0);
	gpClassFw[4] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\bafw", 0);
	gpClassFws[4] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\bafws", 0);
	gpClassBw[4] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\babw", 0);

	if (bExpansion)
	{
		// 5: Druid
		gpClassNu1[5] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dznu1", 0);
		gpClassNu2[5] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dznu2", 0);
		gpClassNu3[5] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dznu3", 0);
		gpClassFw[5] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dzfw", 0);
		gpClassBw[5] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dzbw", 0);

		// 6: Assassin
		gpClassNu1[6] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asnu1", 0);
		gpClassNu2[6] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asnu2", 0);
		gpClassNu3[6] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asnu3", 0);
		gpClassFw[6] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asfw", 0);
		gpClassBw[6] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asbw", 0);
	}
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
	FreeCellPtr(&gpBgCharSelect);
	FreeCellPtr(&gpBgCharCreate);
	FreeCellPtr(&gpBtnWideBlank);
	FreeCellPtr(&gpBtnMediumBlank);
	FreeCellPtr(&gpBtnMediumSelBlank);
	FreeCellPtr(&gpBtnTallBlank);
	FreeCellPtr(&gpBtnCancelBlank);
	FreeCellPtr(&gpBtnClickBox);
	FreeCellPtr(&gpImgTextBox);
	FreeCellPtr(&gpImgTextBox2);
	FreeCellPtr(&gpCharSelectBox);
	FreeCellPtr(&gpCharSelectBoxGrey);
	FreeCellPtr(&gpLogoBlackLeft);
	FreeCellPtr(&gpLogoFireLeft);
	FreeCellPtr(&gpLogoBlackRight);
	FreeCellPtr(&gpLogoFireRight);
	FreeCellPtr(&gpCampfire);

	for (int i = 0; i < 7; ++i)
	{
		FreeCellPtr(&gpClassNu1[i]);
		FreeCellPtr(&gpClassNu2[i]);
		FreeCellPtr(&gpClassNu3[i]);
		FreeCellPtr(&gpClassNu3s[i]);
		FreeCellPtr(&gpClassFw[i]);
		FreeCellPtr(&gpClassFws[i]);
		FreeCellPtr(&gpClassBw[i]);
		FreeCellPtr(&gpClassBws[i]);
	}
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

static void EncodePreviewInfo(
	D2CharacterPreviewInfoStrc* pPreviewInfo,
	uint8_t nCharClass,
	uint8_t nCharLevel,
	uint16_t wSaveFlags,
	const uint8_t* pComponents,
	const uint8_t* pColors)
{
	if (!pPreviewInfo)
	{
		return;
	}
	uint8_t* pOut = reinterpret_cast<uint8_t*>(pPreviewInfo);
	FOG_Encode14BitsToString(reinterpret_cast<uint16_t*>(pOut), 0x0C);
	pOut += 2;
	for (int i = 0; i < 11; ++i)
	{
		*pOut++ = pComponents ? pComponents[i] : 0xFF;
	}
	*pOut++ = static_cast<uint8_t>(nCharClass + 1);
	for (int i = 0; i < 11; ++i)
	{
		*pOut++ = pColors ? pColors[i] : 0xFF;
	}
	*pOut++ = nCharLevel ? nCharLevel : 1;
	FOG_Encode14BitsToString(reinterpret_cast<uint16_t*>(pOut), wSaveFlags);
	pOut += 2;
	FOG_Encode14BitsToString(reinterpret_cast<uint16_t*>(pOut), 0);
	pOut += 2;
	pOut[0] = 0xFF;
	pOut[1] = 0xFF;
	pOut[2] = 0xFF;
	pOut[3] = 0;
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
	std::memset(gD2SaveHeader.dwSkillHotkeys, 0xFF, sizeof(gD2SaveHeader.dwSkillHotkeys));
	gD2SaveHeader.dwLeftSkill1 = 0xFFFFFFFFu;
	gD2SaveHeader.dwRightSkill1 = 0xFFFFFFFFu;
	gD2SaveHeader.dwLeftSkill2 = 0xFFFFFFFFu;
	gD2SaveHeader.dwRightSkill2 = 0xFFFFFFFFu;
	std::memcpy(gD2SaveHeader.nAppearanceComponents, kDefaultAppearance, sizeof(gD2SaveHeader.nAppearanceComponents));
	std::memset(gD2SaveHeader.nAppearanceColors, 0xFF, sizeof(gD2SaveHeader.nAppearanceColors));

	EncodePreviewInfo(
		pPreviewInfo,
		nCharClass,
		1,
		static_cast<uint16_t>(dwFlags),
		gD2SaveHeader.nAppearanceComponents,
		gD2SaveHeader.nAppearanceColors);
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

// 1.12a: 0x6FA50E80
static int __stdcall D2LAUNCH_EditBoxCharFilter(D2WinEditBoxStrc* pEditBox, int32_t nPos, uint8_t* pChar)
{
	if (!pChar)
	{
		return 0;
	}
	const uint8_t cCh = *pChar;
	if ((cCh >= 'A' && cCh <= 'Z') || (cCh >= 'a' && cCh <= 'z'))
	{
		return 1;
	}
	if (cCh == '-' || cCh == '_')
	{
		if (nPos == 0)
		{
			return 0;
		}
		if (pEditBox)
		{
			const Unicode* pwszText = D2Win_10077_EDITBOX_GetText(pEditBox);
			if (pwszText)
			{
				for (const Unicode* p = pwszText; *p; ++p)
				{
					if (*p == '-' || *p == '_')
					{
						return 0;
					}
				}
			}
		}
		return 1;
	}
	return 0;
}

// 1.12a: 0x6FA53080
static bool D2LAUNCH_IsCharNameValid(const char* szName)
{
	if (!szName)
	{
		return false;
	}
	const size_t nLen = std::strlen(szName);
	if (nLen < 2 || nLen >= 16)
	{
		return false;
	}
	if (szName[0] == '-' || szName[0] == '_' || szName[nLen - 1] == '-' || szName[nLen - 1] == '_')
	{
		return false;
	}
	int nHyphenCount = 0;
	for (size_t i = 0; i < nLen; ++i)
	{
		const char c = szName[i];
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		{
			continue;
		}
		if (c == '-' || c == '_')
		{
			++nHyphenCount;
			if (nHyphenCount >= 2)
			{
				return false;
			}
			continue;
		}
		return false;
	}
	return true;
}

// 1.12a: 0x6FA53100
static void __stdcall D2LAUNCH_OnNameTextChanged(int32_t nUnused)
{
	(void)nUnused;
	char szTypedName[32] = {};
	if (gpCreateNameEditBox)
	{
		const Unicode* pwszText = D2Win_10077_EDITBOX_GetText(gpCreateNameEditBox);
		if (pwszText && pwszText[0])
		{
			Unicode::unicode2Win(szTypedName, pwszText, sizeof(szTypedName));
		}
	}
	const bool bValid = D2LAUNCH_IsCharNameValid(szTypedName);
	if (gpCreateOkBtn)
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(&gpCreateOkBtn->controlHeader, bValid ? 1 : 0);
	}
}

// 1.12a: 0x6FA59B90
static int __stdcall D2LAUNCH_OnCreateCharConfirm(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (!gpD2Config || gnSelectedCreateClass < 0 || gnSelectedCreateClass >= 7)
	{
		return 0;
	}
	if (!gpCreateOkBtn || !(gpCreateOkBtn->controlHeader.dwFlags & 1))
	{
		return 0;
	}

	char szTypedName[32] = {};
	if (gpCreateNameEditBox)
	{
		const Unicode* pwszText = D2Win_10077_EDITBOX_GetText(gpCreateNameEditBox);
		if (pwszText && pwszText[0])
		{
			Unicode::unicode2Win(szTypedName, pwszText, sizeof(szTypedName));
		}
	}

	if (!D2LAUNCH_IsCharNameValid(szTypedName))
	{
		return 0;
	}

	SStrCopy(gpD2Config->szName, szTypedName, sizeof(gpD2Config->szName));

	uint16_t wSaveFlags = 0;
	if (FOG_IsExpansion())
	{
		if (!gpCreateExpCheckBox || BUTTON_IsPressed(gpCreateExpCheckBox))
		{
			wSaveFlags |= 0x20u;
		}
	}
	if (gpCreateHcCheckBox && BUTTON_IsPressed(gpCreateHcCheckBox))
	{
		wSaveFlags |= 0x04u;
	}
	if (gnSelectedCreateClass == 5 || gnSelectedCreateClass == 6)
	{
		wSaveFlags |= 0x20u;
	}

	gpD2Config->unpackedCTemp.nCharacterSaveFlags = wSaveFlags;
	gpD2Config->unpackedCTemp.nCharacterClassId = static_cast<uint8_t>(gnSelectedCreateClass);

	D2LAUNCH_InitNewSaveAndPreview(
		&gpD2Config->tCharPreviewInfo,
		gpD2Config->unpackedCTemp.nCharacterClassId,
		gpD2Config->szName,
		gpD2Config->unpackedCTemp.nCharacterSaveFlags);
	D2LAUNCH_WriteNewSaveFile(gpD2Config->szName);

	uint32_t dwArenaFlags = (wSaveFlags & 0x04) ? 0x804u : 0x004u;
	if (wSaveFlags & 0x20)
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

static int __stdcall D2LAUNCH_OnCreateCancel(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyAllControls();
	if (D2LAUNCH_HasSavedCharacters())
	{
		D2LAUNCH_EnterCharSelect();
	}
	else
	{
		D2LAUNCH_EnterMainMenu(0);
	}
	return 1;
}

static int __stdcall D2LAUNCH_OnCharSelectOk(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (gpSelectedChar && gpD2Config)
	{
		gpD2Config->tCharPreviewInfo = gpSelectedChar->tPreview;
		gpD2Config->unpackedCTemp.nCharacterClassId = gpSelectedChar->nCharClass;
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

static void RefreshCharSelectUI();

static int __stdcall D2LAUNCH_OnCharSelectConvertExpansion(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (!gpSelectedChar || (gpSelectedChar->wSaveFlags & 0x20))
	{
		return 0;
	}

	char szSaveDir[MAX_PATH] = {};
	char szFullPath[MAX_PATH * 2] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
	std::snprintf(szFullPath, sizeof(szFullPath), "%s%s.d2s", szSaveDir, gpSelectedChar->szCharName);

	FILE* pFile = std::fopen(szFullPath, "r+b");
	if (pFile)
	{
		std::fseek(pFile, 0, SEEK_END);
		const long nSize = std::ftell(pFile);
		std::fseek(pFile, 0, SEEK_SET);
		if (nSize >= static_cast<long>(sizeof(D2SaveHeaderStrc)))
		{
			uint8_t* pBuf = static_cast<uint8_t*>(D2_CALLOC(nSize));
			if (pBuf)
			{
				if (std::fread(pBuf, 1, nSize, pFile) == static_cast<size_t>(nSize))
				{
					D2SaveHeaderStrc* pHdr = reinterpret_cast<D2SaveHeaderStrc*>(pBuf);
					pHdr->dwSaveFlags |= 0x20u;
					pHdr->dwChecksum = 0;
					pHdr->dwChecksum = FOG_ComputeChecksum(pBuf, nSize);
					std::fseek(pFile, 0, SEEK_SET);
					std::fwrite(pBuf, 1, nSize, pFile);
					gpSelectedChar->wSaveFlags |= 0x20u;
					EncodePreviewInfo(
						&gpSelectedChar->tPreview,
						gpSelectedChar->nCharClass,
						gpSelectedChar->nCharLevel,
						gpSelectedChar->wSaveFlags,
						gpSelectedChar->nAppearanceComponents,
						gpSelectedChar->nAppearanceColors);
				}
				D2_FREE(pBuf);
			}
		}
		std::fclose(pFile);
	}

	RefreshCharSelectUI();
	return 1;
}

static int __stdcall D2LAUNCH_OnCharSelectDelete(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (!gpSelectedChar)
	{
		return 0;
	}

	char szSaveDir[MAX_PATH] = {};
	char szBasePath[MAX_PATH * 2] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));

	static const char* const kSaveExts[] = { ".d2s", ".key", ".ma0", ".ma1", ".ma2", ".ma3", ".map" };
	for (const char* szExt : kSaveExts)
	{
		std::snprintf(szBasePath, sizeof(szBasePath), "%s%s%s", szSaveDir, gpSelectedChar->szCharName, szExt);
		DeleteFileA(szBasePath);
	}

	D2LAUNCH_DestroyAllControls();
	if (D2LAUNCH_HasSavedCharacters())
	{
		D2LAUNCH_EnterCharSelect();
	}
	else
	{
		D2LAUNCH_EnterMainMenu(0);
	}
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

static D2WinControlStrc* CreateBackgroundImageControl(
	D2CellFileStrc** ppBgCellFile,
	int(__stdcall* pfClick)(SMSGHANDLER_PARAMS*))
{
	D2WinControlInitStrc tImg = {};
	tImg.nType = D2WIN_IMAGE;
	tImg.nX = 0;
	tImg.nY = 599;
	tImg.nWidth = 800;
	tImg.nHeight = 600;
	tImg.ppCellFile = ppBgCellFile;
	tImg.field_20 = pfClick;
	tImg.field_28 = reinterpret_cast<void*>(pfClick);
	return RegisterControl(CONTROL_Create(&tImg));
}

static void CreateBurningLogoControls()
{
	D2WinControlInitStrc tLogoLeft = {};
	tLogoLeft.nType = D2WIN_ANIMIMAGE;
	tLogoLeft.nX = 400;
	tLogoLeft.nY = 120;
	tLogoLeft.nWidth = 181;
	tLogoLeft.nHeight = 170;
	tLogoLeft.field_24 = gAnimLogoLeft;
	RegisterControl(CONTROL_Create(&tLogoLeft));

	D2WinControlInitStrc tLogoRight = {};
	tLogoRight.nType = D2WIN_ANIMIMAGE;
	tLogoRight.nX = 400;
	tLogoRight.nY = 120;
	tLogoRight.nWidth = 188;
	tLogoRight.nHeight = 177;
	tLogoRight.field_24 = gAnimLogoRight;
	RegisterControl(CONTROL_Create(&tLogoRight));
}

// 1.12a: 0x6FA58150
void __stdcall D2LAUNCH_EnterLogoScreen()
{
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_SetSkyPalette();
	CreateBackgroundImageControl(&gpBgTitleScreen, nullptr);
	CreateBurningLogoControls();

	D2WinControlInitStrc tBtnInit = {};
	tBtnInit.nType = D2WIN_BUTTON;
	tBtnInit.nX = 0;
	tBtnInit.nY = 599;
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
	D2LAUNCH_SetSkyPalette();
	CreateBackgroundImageControl(&gpBgGameSelect, nullptr);
	CreateBurningLogoControls();

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

	// Battle.net (5107) — disabled for offline/single-player build
	tBtn.nY = 330;
	tBtn.nStringId = 5107;
	tBtn.field_20 = nullptr;
	if (D2WinControlStrc* pBnetBtn = RegisterControl(CONTROL_Create(&tBtn)))
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(pBnetBtn, 0);
	}

	// Other Multiplayer (5108) — disabled for offline/single-player build
	tBtn.nY = 370;
	tBtn.nStringId = 5108;
	tBtn.field_20 = nullptr;
	if (D2WinControlStrc* pMultiBtn = RegisterControl(CONTROL_Create(&tBtn)))
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(pMultiBtn, 0);
	}

	// Client Mode Toggle: TS Web Client vs. Native C++ Client
	SyncClientModeEnv();
	tBtn.nY = 435;
	tBtn.nStringId = 0;
	tBtn.field_20 = D2LAUNCH_OnToggleClientMode;
	gpBtnClientModeToggle = reinterpret_cast<D2WinButtonStrc*>(RegisterControl(CONTROL_Create(&tBtn)));
	RefreshClientModeButtonText();

	// Exit Diablo II (5109)
	tBtn.nY = 500;
	tBtn.nStringId = 5109;
	tBtn.field_20 = D2LAUNCH_OnExitClick;
	RegisterControl(CONTROL_Create(&tBtn));
}

static void EnumerateSavedCharactersFromDir(const char* szSaveDir, D2LaunchCharNodeStrc** ppTail)
{
	char szPattern[MAX_PATH * 2] = {};
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
				dwRead >= 0xA8 &&
				tHdr.dwHeaderMagic == 0xAA55AA55 &&
				tHdr.dwVersion >= 0x47 &&
				tHdr.dwVersion <= 0x60)
			{
				char szCandidateName[16] = {};
				tHdr.szName[15] = '\0';
				if (tHdr.szName[0])
				{
					SStrCopy(szCandidateName, tHdr.szName, sizeof(szCandidateName));
				}
				else
				{
					SStrCopy(szCandidateName, tFind.cFileName, sizeof(szCandidateName));
					if (char* pDot = std::strrchr(szCandidateName, '.'))
					{
						*pDot = '\0';
					}
				}

				bool bAlreadyListed = false;
				for (D2LaunchCharNodeStrc* pCur = gpCharListHead; pCur; pCur = pCur->pNext)
				{
					if (_stricmp(pCur->szCharName, szCandidateName) == 0)
					{
						bAlreadyListed = true;
						break;
					}
				}

				if (!bAlreadyListed)
				{
					D2LaunchCharNodeStrc* pNode = static_cast<D2LaunchCharNodeStrc*>(D2_CALLOC(sizeof(D2LaunchCharNodeStrc)));
					if (pNode)
					{
						SStrCopy(pNode->szCharName, szCandidateName, sizeof(pNode->szCharName));
						pNode->wSaveFlags = static_cast<uint16_t>(tHdr.dwSaveFlags);
						pNode->nCharClass = (tHdr.nCharClass < 7) ? tHdr.nCharClass : 0;
						pNode->nCharLevel = tHdr.nCharLevel ? tHdr.nCharLevel : 1;
						std::memcpy(pNode->nAppearanceComponents, tHdr.nAppearanceComponents, 16);
						std::memcpy(pNode->nAppearanceColors, tHdr.nAppearanceColors, 16);

						EncodePreviewInfo(
							&pNode->tPreview,
							pNode->nCharClass,
							pNode->nCharLevel,
							pNode->wSaveFlags,
							pNode->nAppearanceComponents,
							pNode->nAppearanceColors);

						pNode->pNext = nullptr;
						if (!gpCharListHead)
						{
							gpCharListHead = pNode;
							*ppTail = pNode;
						}
						else
						{
							(*ppTail)->pNext = pNode;
							*ppTail = pNode;
						}
						if (!gpSelectedChar)
						{
							gpSelectedChar = pNode;
						}
					}
				}
			}
			FOG_CloseFile(hFile);
		}
	} while (FindNextFileA(hFind, &tFind));

	FindClose(hFind);
}

static void EnumerateSavedCharacters()
{
	FreeCharList();

	char szSaveDir[MAX_PATH] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));

	D2LaunchCharNodeStrc* pTail = nullptr;
	EnumerateSavedCharactersFromDir(szSaveDir, &pTail);

	static const char* const kFallbackDirs[] = { "..\\..\\Debug\\save\\", "..\\..\\Release\\save\\" };
	for (const char* szRel : kFallbackDirs)
	{
		char szAltDir[MAX_PATH * 2] = {};
		std::snprintf(szAltDir, sizeof(szAltDir), "%s%s", szSaveDir, szRel);
		EnumerateSavedCharactersFromDir(szAltDir, &pTail);
	}
}

// 1.12a: 0x6FA4CF70 (Populate character slot text & selection visuals)
static void PopulateSlotTextBox(int nSlotIndex, D2LaunchCharNodeStrc* pNode)
{
	D2WinTextBoxStrc* pTextBox = gpCharSlotTextBox[nSlotIndex];
	if (!pTextBox)
	{
		return;
	}
	D2Win_10047(pTextBox);
	if (!pNode)
	{
		return;
	}

	// Line 1: Character Name (color 1 = Red for Hardcore, 4 = Gold for Softcore)
	const int32_t nNameColor = (pNode->wSaveFlags & 0x04) ? 1 : 4;
	D2Win_10046(pTextBox, pNode->szCharName, nNameColor);

	// Line 2: "Level X <Class>" (color 0 = White)
	const uint8_t nClass = (pNode->nCharClass < 7) ? pNode->nCharClass : 0;
	char szClassName[64] = {};
	const Unicode* pwszClass = D2LANG_GetStringFromTblIndex(kClassNameTblIds[nClass]);
	if (pwszClass && pwszClass[0])
	{
		Unicode::unicode2Win(szClassName, pwszClass, sizeof(szClassName));
	}
	if (!szClassName[0])
	{
		SStrCopy(szClassName, kClassFallbackNames[nClass], sizeof(szClassName));
	}

	char szLevelLine[96] = {};
	std::snprintf(szLevelLine, sizeof(szLevelLine), "Level %u %s", pNode->nCharLevel, szClassName);
	D2Win_10046(pTextBox, szLevelLine, 0);

	// Line 3: "EXPANSION CHARACTER" (color 2 = Green) if Expansion flag (0x20) is set
	if (pNode->wSaveFlags & 0x20)
	{
		const Unicode* pwszExp = D2LANG_GetStringFromTblIndex(22731);
		if (pwszExp && pwszExp[0])
		{
			D2Win_10051(pTextBox, pwszExp, 2, 0, 0);
		}
		else
		{
			D2Win_10046(pTextBox, "EXPANSION CHARACTER", 2);
		}
	}
}

static void RefreshCharSelectUI()
{
	if (gpCharSelectTitleBox)
	{
		D2Win_10047(gpCharSelectTitleBox);
		if (gpSelectedChar)
		{
			D2Win_10046(gpCharSelectTitleBox, gpSelectedChar->szCharName, 4);
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		const bool bHasChar = (gpCharSlotNode[i] != nullptr);
		const bool bIsSelected = bHasChar && (gpCharSlotNode[i] == gpSelectedChar);

		if (gpCharSlotFrameImg[i])
		{
			SetControlVisibleAndEnabled(&gpCharSlotFrameImg[i]->controlHeader, bHasChar ? 1 : 0);
			IMAGE_SetCellFile(
				gpCharSlotFrameImg[i],
				bIsSelected ? gpCharSelectBox : gpCharSelectBoxGrey,
				nullptr);
		}

		if (gpCharSlotTextBox[i])
		{
			PopulateSlotTextBox(i, gpCharSlotNode[i]);
		}

		if (gpCharSlotCompUnit[i])
		{
			D2Win_10156(gpCharSlotCompUnit[i], bIsSelected ? 5 : 1);
		}
	}

	if (gpBtnConvertExp)
	{
		const bool bCanConvert = FOG_IsExpansion() && gpSelectedChar && !(gpSelectedChar->wSaveFlags & 0x20);
		D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnConvertExp->controlHeader, bCanConvert ? 1 : 0);
	}
	if (gpBtnDeleteChar)
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnDeleteChar->controlHeader, gpSelectedChar ? 1 : 0);
	}
	if (gpBtnCharSelectOk)
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnCharSelectOk->controlHeader, gpSelectedChar ? 1 : 0);
	}
}

static int SelectCharSlot(int nSlotIndex)
{
	if (nSlotIndex < 0 || nSlotIndex >= 8 || !gpCharSlotNode[nSlotIndex])
	{
		return 0;
	}

	const DWORD dwNow = GetTickCount();
	if (gnSelectedSlotIndex == nSlotIndex && (dwNow - gdwLastSlotClickTick) <= 450)
	{
		return D2LAUNCH_OnCharSelectOk(nullptr);
	}

	gnSelectedSlotIndex = nSlotIndex;
	gdwLastSlotClickTick = dwNow;
	gpSelectedChar = gpCharSlotNode[nSlotIndex];
	RefreshCharSelectUI();
	return 1;
}

#define DEFINE_CHAR_SLOT_CLICK(idx) \
	static int __stdcall D2LAUNCH_OnCharSlotClick_##idx(SMSGHANDLER_PARAMS* pMsg) \
	{ \
		(void)pMsg; \
		return SelectCharSlot(idx); \
	}

DEFINE_CHAR_SLOT_CLICK(0)
DEFINE_CHAR_SLOT_CLICK(1)
DEFINE_CHAR_SLOT_CLICK(2)
DEFINE_CHAR_SLOT_CLICK(3)
DEFINE_CHAR_SLOT_CLICK(4)
DEFINE_CHAR_SLOT_CLICK(5)
DEFINE_CHAR_SLOT_CLICK(6)
DEFINE_CHAR_SLOT_CLICK(7)

static int(__stdcall* const gapfnCharSlotClick[8])(SMSGHANDLER_PARAMS*) = {
	D2LAUNCH_OnCharSlotClick_0,
	D2LAUNCH_OnCharSlotClick_1,
	D2LAUNCH_OnCharSlotClick_2,
	D2LAUNCH_OnCharSlotClick_3,
	D2LAUNCH_OnCharSlotClick_4,
	D2LAUNCH_OnCharSlotClick_5,
	D2LAUNCH_OnCharSlotClick_6,
	D2LAUNCH_OnCharSlotClick_7,
};

// 1.12a: 0x6FA4EAE0
BOOL __stdcall D2LAUNCH_EnterCharSelect()
{
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_SetSkyPalette();
	CreateBackgroundImageControl(&gpBgCharSelect, nullptr);
	EnumerateSavedCharacters();

	// Top Selected Character Name Banner (0x6FA62C78)
	{
		D2WinControlInitStrc tTitle = {};
		tTitle.nType = D2WIN_TEXTBOX;
		tTitle.nX = 85;
		tTitle.nY = 78;
		tTitle.nWidth = 466;
		tTitle.nHeight = 42;
		tTitle.field_14 = 7;
		tTitle.nStringId = 2;
		tTitle.field_28 = &gFontStyle42;
		tTitle.field_2C = reinterpret_cast<void*>(0x22);
		gpCharSelectTitleBox = reinterpret_cast<D2WinTextBoxStrc*>(RegisterControl(CONTROL_Create(&tTitle)));
	}

	// 8 Character Slots (4 rows x 2 columns: 0x6FA62978 & 0x6FA627F8)
	D2LaunchCharNodeStrc* pCurNode = gpCharListHead;
	gnSelectedSlotIndex = -1;

	for (int i = 0; i < 8; ++i)
	{
		const int nCol = i % 2;
		const int nRow = i / 2;
		gpCharSlotNode[i] = pCurNode;
		if (pCurNode)
		{
			if (pCurNode == gpSelectedChar || gnSelectedSlotIndex == -1)
			{
				gpSelectedChar = pCurNode;
				gnSelectedSlotIndex = i;
			}
			pCurNode = pCurNode->pNext;
		}

		// Slot Frame Image (0x6FA62978 + i * 0x30)
		D2WinControlInitStrc tImg = {};
		tImg.nType = D2WIN_IMAGE;
		tImg.nX = nCol ? 307 : 35;
		tImg.nY = 178 + nRow * 93;
		tImg.nWidth = 272;
		tImg.nHeight = 92;
		tImg.ppCellFile = (gpCharSlotNode[i] && gpCharSlotNode[i] == gpSelectedChar)
			? &gpCharSelectBox
			: &gpCharSelectBoxGrey;
		gpCharSlotFrameImg[i] = reinterpret_cast<D2WinImageStrc*>(RegisterControl(CONTROL_Create(&tImg)));
		if (gpCharSlotFrameImg[i])
		{
			IMAGE_ForceNormalDrawMode(gpCharSlotFrameImg[i], 1);
		}

		// Slot Text Box (0x6FA627F8 + i * 0x30: field_14 = 0x4C = 76px left offset for paperdoll)
		D2WinControlInitStrc tBox = {};
		tBox.nType = D2WIN_TEXTBOX;
		tBox.nX = nCol ? 309 : 37;
		tBox.nY = 178 + nRow * 93;
		tBox.nWidth = 200;
		tBox.nHeight = 92;
		tBox.field_14 = 0x4C;
		tBox.nStringId = 3;
		tBox.field_20 = gapfnCharSlotClick[i];
		tBox.field_28 = &gFontStyle16;
		tBox.field_2C = reinterpret_cast<void*>(0x20);
		gpCharSlotTextBox[i] = reinterpret_cast<D2WinTextBoxStrc*>(RegisterControl(CONTROL_Create(&tBox)));

		// Paperdoll Composite Unit (0x6FA4E050 / 0x6FA4CF70)
		if (D2LaunchCharNodeStrc* pSlotChar = gpCharSlotNode[i])
		{
			D2CompositeUnitStrc* pComp = D2Win_10144(
				pSlotChar->nCharClass,
				5,
				pSlotChar->nAppearanceComponents,
				pSlotChar->nAppearanceColors);
			gpCharSlotCompUnit[i] = pComp;
			if (pComp)
			{
				D2Win_10149(pComp, 0);
				D2Win_10151(pComp, 0);
				D2Win_10143(pComp, 5);
				D2Win_10152(pComp, nCol ? 344 : 72);
				D2Win_10153(pComp, 165 + nRow * 93);
				D2Win_10156(pComp, (pSlotChar == gpSelectedChar) ? 5 : 1);
			}
		}
	}

	// Two-line Tall Buttons (0x6FA62CA8, 0x6FA62CD8, 0x6FA62D08)
	{
		D2WinControlInitStrc tTallBtn = {};
		tTallBtn.nType = D2WIN_BUTTON;
		tTallBtn.nY = 528;
		tTallBtn.nWidth = 168;
		tTallBtn.nHeight = 60;
		tTallBtn.ppCellFile = &gpBtnTallBlank;
		tTallBtn.field_2C = reinterpret_cast<void*>(0x40);

		// Create New Character (10832 "CREATE NEW" + 21796 "CHARACTER")
		tTallBtn.nX = 33;
		tTallBtn.nStringId = 10832;
		tTallBtn.field_20 = D2LAUNCH_OnCharSelectCreateNew;
		D2WinButtonStrc* pBtnCreate = reinterpret_cast<D2WinButtonStrc*>(RegisterControl(CONTROL_Create(&tTallBtn)));
		if (pBtnCreate)
		{
			BUTTON_SetStringId(pBtnCreate, 21796);
		}

		// Convert to Expansion (22732 "CONVERT TO" + 22730 "EXPANSION")
		if (FOG_IsExpansion())
		{
			tTallBtn.nX = 233;
			tTallBtn.nStringId = 22732;
			tTallBtn.field_20 = D2LAUNCH_OnCharSelectConvertExpansion;
			gpBtnConvertExp = reinterpret_cast<D2WinButtonStrc*>(RegisterControl(CONTROL_Create(&tTallBtn)));
			if (gpBtnConvertExp)
			{
				BUTTON_SetStringId(gpBtnConvertExp, 22730);
			}
		}

		// Delete Character (5272 "DELETE" + 21796 "CHARACTER")
		tTallBtn.nX = 433;
		tTallBtn.nStringId = 5272;
		tTallBtn.field_20 = D2LAUNCH_OnCharSelectDelete;
		gpBtnDeleteChar = reinterpret_cast<D2WinButtonStrc*>(RegisterControl(CONTROL_Create(&tTallBtn)));
		if (gpBtnDeleteChar)
		{
			BUTTON_SetStringId(gpBtnDeleteChar, 21796);
		}
	}

	// Bottom EXIT & OK Medium Buttons (0x6FA62D38, 0x6FA62D68)
	{
		D2WinControlInitStrc tMedBtn = {};
		tMedBtn.nType = D2WIN_BUTTON;
		tMedBtn.nY = 572;
		tMedBtn.nWidth = 128;
		tMedBtn.nHeight = 35;
		tMedBtn.ppCellFile = &gpBtnMediumBlank;

		// EXIT (5101)
		tMedBtn.nX = 33;
		tMedBtn.field_14 = VK_ESCAPE;
		tMedBtn.nStringId = 5101;
		tMedBtn.field_20 = D2LAUNCH_OnCancelToMainMenu;
		RegisterControl(CONTROL_Create(&tMedBtn));

		// OK (5102)
		tMedBtn.nX = 627;
		tMedBtn.field_14 = VK_RETURN;
		tMedBtn.nStringId = 5102;
		tMedBtn.field_20 = D2LAUNCH_OnCharSelectOk;
		gpBtnCharSelectOk = reinterpret_cast<D2WinButtonStrc*>(RegisterControl(CONTROL_Create(&tMedBtn)));

		// Client Mode Toggle on Character Select (centered between EXIT and OK)
		SyncClientModeEnv();
		D2WinControlInitStrc tToggleBtn = {};
		tToggleBtn.nType = D2WIN_BUTTON;
		tToggleBtn.nX = 216;
		tToggleBtn.nY = 572;
		tToggleBtn.nWidth = 272;
		tToggleBtn.nHeight = 35;
		tToggleBtn.ppCellFile = &gpBtnWideBlank;
		tToggleBtn.nStringId = 0;
		tToggleBtn.field_20 = D2LAUNCH_OnToggleClientMode;
		gpBtnClientModeToggle = reinterpret_cast<D2WinButtonStrc*>(RegisterControl(CONTROL_Create(&tToggleBtn)));
		RefreshClientModeButtonText();
	}

	RefreshCharSelectUI();
	return TRUE;
}

// Character Create Helpers (0x6FA4FC90, 0x6FA52C00)
static void UpdateCreateClassText(int nClassId)
{
	if (!gpCreateClassNameBox || !gpCreateClassDescBox)
	{
		return;
	}
	D2Win_10047(gpCreateClassNameBox);
	D2Win_10047(gpCreateClassDescBox);

	if (nClassId >= 0 && nClassId < 7)
	{
		const Unicode* pwszName = D2LANG_GetStringFromTblIndex(kClassNameTblIds[nClassId]);
		if (pwszName && pwszName[0])
		{
			D2Win_10051(gpCreateClassNameBox, pwszName, 4, 0, 0);
		}
		else
		{
			D2Win_10046(gpCreateClassNameBox, kClassFallbackNames[nClassId], 4);
		}

		const Unicode* pwszDesc = D2LANG_GetStringFromTblIndex(kClassDescTblIds[nClassId]);
		if (pwszDesc && pwszDesc[0])
		{
			D2Win_10051(gpCreateClassDescBox, pwszDesc, 0, 0, 0);
		}
	}
}

static void SetCreateNameControlsVisible(int bVisible, int nClassId)
{
	SetControlVisibleAndEnabled(reinterpret_cast<D2WinControlStrc*>(gpCreateNameLabelBox), bVisible);
	SetControlVisibleAndEnabled(reinterpret_cast<D2WinControlStrc*>(gpCreateNameFrameImg), bVisible);
	SetControlVisibleAndEnabled(reinterpret_cast<D2WinControlStrc*>(gpCreateNameEditBox), bVisible);
	SetControlVisibleAndEnabled(reinterpret_cast<D2WinControlStrc*>(gpCreateHcCheckBox), bVisible);
	SetControlVisibleAndEnabled(reinterpret_cast<D2WinControlStrc*>(gpCreateHcLabelBox), bVisible);
	SetControlVisibleAndEnabled(reinterpret_cast<D2WinControlStrc*>(gpCreateOkBtn), bVisible);

	// Druid (5) and Assassin (6) are always Expansion characters; hide Expansion toggle for them
	const int bShowExpToggle = bVisible && FOG_IsExpansion() && (nClassId != 5 && nClassId != 6);
	SetControlVisibleAndEnabled(reinterpret_cast<D2WinControlStrc*>(gpCreateExpCheckBox), bShowExpToggle);
	SetControlVisibleAndEnabled(reinterpret_cast<D2WinControlStrc*>(gpCreateExpLabelBox), bShowExpToggle);

	if (bVisible && gpCreateNameEditBox)
	{
		D2Win_10012(gpCreateNameEditBox);
		D2LAUNCH_OnNameTextChanged(0);
	}
	else
	{
		if (gpCreateNameEditBox)
		{
			D2Win_10076(gpCreateNameEditBox, "");
		}
		D2Win_10012(nullptr);
	}
}

static int HandleCreateClassClick(int nClassId)
{
	if (nClassId < 0 || nClassId >= 7)
	{
		return 0;
	}

	if (gnSelectedCreateClass == nClassId)
	{
		// Clicking the already-selected class steps it back to the campfire
		gnSelectedCreateClass = -1;
		UpdateCreateClassText(nClassId);
		SetCreateNameControlsVisible(0, -1);
		return 1;
	}

	// Step back any previously selected class
	if (gnSelectedCreateClass >= 0 && gnSelectedCreateClass < 7)
	{
		D2WinAnimImageStrc* pPrev = gpCreateClassAnim[gnSelectedCreateClass];
		if (pPrev)
		{
			if (pPrev->nAnimType == 2)
			{
				pPrev->nAnimType = 3;
			}
			D2Win_10104(pPrev);
		}
	}

	gnSelectedCreateClass = nClassId;
	if (gpCreateClassAnim[nClassId] && gpCreateClassAnim[nClassId]->nAnimType == 0)
	{
		gpCreateClassAnim[nClassId]->nAnimType = 1;
	}
	UpdateCreateClassText(nClassId);
	SetCreateNameControlsVisible(1, nClassId);
	return 1;
}

static void HandleCreateClassHover(int nClassId, SMSGHANDLER_PARAMS* pMsg)
{
	if (gnSelectedCreateClass != -1)
	{
		return;
	}
	if (pMsg && pMsg->wParam)
	{
		UpdateCreateClassText(nClassId);
	}
	else
	{
		UpdateCreateClassText(-1);
	}
}

static int __stdcall D2LAUNCH_OnCreateBgClick(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (gnSelectedCreateClass >= 0 && gnSelectedCreateClass < 7)
	{
		D2WinAnimImageStrc* pPrev = gpCreateClassAnim[gnSelectedCreateClass];
		if (pPrev)
		{
			if (pPrev->nAnimType == 2)
			{
				pPrev->nAnimType = 3;
			}
			D2Win_10104(pPrev);
		}
		gnSelectedCreateClass = -1;
		UpdateCreateClassText(-1);
		SetCreateNameControlsVisible(0, -1);
	}
	return 1;
}

#define DEFINE_CREATE_CLASS_CALLBACKS(cls) \
	static int __stdcall D2LAUNCH_OnCreateClassClick_##cls(SMSGHANDLER_PARAMS* pMsg) \
	{ \
		(void)pMsg; \
		return HandleCreateClassClick(cls); \
	} \
	static void __stdcall D2LAUNCH_OnCreateClassHover_##cls(SMSGHANDLER_PARAMS* pMsg) \
	{ \
		HandleCreateClassHover(cls, pMsg); \
	}

DEFINE_CREATE_CLASS_CALLBACKS(0)
DEFINE_CREATE_CLASS_CALLBACKS(1)
DEFINE_CREATE_CLASS_CALLBACKS(2)
DEFINE_CREATE_CLASS_CALLBACKS(3)
DEFINE_CREATE_CLASS_CALLBACKS(4)
DEFINE_CREATE_CLASS_CALLBACKS(5)
DEFINE_CREATE_CLASS_CALLBACKS(6)

// 1.12a: 0x6FA55980
BOOL __stdcall D2LAUNCH_EnterCharCreate()
{
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_SetFeCharPalette();
	gnSelectedCreateClass = -1;

	// Background (0x6FA63068)
	CreateBackgroundImageControl(&gpBgCharCreate, nullptr);

	// Top Title "Select Hero Class" (0x6FA62FD8, string 5127)
	{
		D2WinControlInitStrc tTop = {};
		tTop.nType = D2WIN_TEXTBOX;
		tTop.nX = 0;
		tTop.nY = 80;
		tTop.nWidth = 800;
		tTop.nHeight = 50;
		tTop.field_28 = &gFontStyle30;
		tTop.field_2C = reinterpret_cast<void*>(2);
		gpCreateTopTitleBox = reinterpret_cast<D2WinTextBoxStrc*>(RegisterControl(CONTROL_Create(&tTop)));
		if (gpCreateTopTitleBox)
		{
			const Unicode* pwszSelectHero = D2LANG_GetStringFromTblIndex(5127);
			if (pwszSelectHero && pwszSelectHero[0])
			{
				D2Win_10051(gpCreateTopTitleBox, pwszSelectHero, 4, 0, 0);
			}
			else
			{
				D2Win_10046(gpCreateTopTitleBox, "Select Hero Class", 4);
			}
		}
	}

	// Class Name & Description TextBoxes (0x6FA63428, 0x6FA63458)
	{
		D2WinControlInitStrc tClassName = {};
		tClassName.nType = D2WIN_TEXTBOX;
		tClassName.nX = 0;
		tClassName.nY = 180;
		tClassName.nWidth = 800;
		tClassName.nHeight = 100;
		tClassName.field_28 = &gFontStyle30;
		tClassName.field_2C = reinterpret_cast<void*>(2);
		gpCreateClassNameBox = reinterpret_cast<D2WinTextBoxStrc*>(RegisterControl(CONTROL_Create(&tClassName)));

		D2WinControlInitStrc tClassDesc = {};
		tClassDesc.nType = D2WIN_TEXTBOX;
		tClassDesc.nX = 250;
		tClassDesc.nY = 210;
		tClassDesc.nWidth = 300;
		tClassDesc.nHeight = 100;
		tClassDesc.field_28 = &gFontStyle16;
		tClassDesc.field_2C = reinterpret_cast<void*>(2);
		gpCreateClassDescBox = reinterpret_cast<D2WinTextBoxStrc*>(RegisterControl(CONTROL_Create(&tClassDesc)));
	}

	// 7 Animated Character Classes in 1.12a Z-order (back-to-front around the campfire)
	struct ClassSpawnDesc
	{
		int nClassId;
		int32_t nX;
		int32_t nY;
		int(__stdcall* pfClick)(SMSGHANDLER_PARAMS*);
		void(__stdcall* pfHover)(SMSGHANDLER_PARAMS*);
	};
	static const ClassSpawnDesc kClassOrder[] = {
		{ 4, 400, 330, D2LAUNCH_OnCreateClassClick_4, D2LAUNCH_OnCreateClassHover_4 }, // Barbarian
		{ 1, 626, 353, D2LAUNCH_OnCreateClassClick_1, D2LAUNCH_OnCreateClassHover_1 }, // Sorceress
		{ 3, 521, 339, D2LAUNCH_OnCreateClassClick_3, D2LAUNCH_OnCreateClassHover_3 }, // Paladin
		{ 2, 301, 333, D2LAUNCH_OnCreateClassClick_2, D2LAUNCH_OnCreateClassHover_2 }, // Necromancer
		{ 6, 232, 364, D2LAUNCH_OnCreateClassClick_6, D2LAUNCH_OnCreateClassHover_6 }, // Assassin
		{ 0, 100, 337, D2LAUNCH_OnCreateClassClick_0, D2LAUNCH_OnCreateClassHover_0 }, // Amazon
		{ 5, 720, 370, D2LAUNCH_OnCreateClassClick_5, D2LAUNCH_OnCreateClassHover_5 }, // Druid
	};

	const BOOL bExpansion = FOG_IsExpansion();
	for (const ClassSpawnDesc& desc : kClassOrder)
	{
		if (!bExpansion && (desc.nClassId == 5 || desc.nClassId == 6))
		{
			continue;
		}
		D2WinControlInitStrc tAnim = {};
		tAnim.nType = D2WIN_ANIMIMAGE;
		tAnim.nX = desc.nX;
		tAnim.nY = desc.nY;
		tAnim.nWidth = 88;
		tAnim.nHeight = 184;
		tAnim.field_20 = desc.pfClick;
		tAnim.field_24 = gAnimCharClass[desc.nClassId];
		tAnim.field_28 = reinterpret_cast<void*>(desc.pfHover);
		gpCreateClassAnim[desc.nClassId] = reinterpret_cast<D2WinAnimImageStrc*>(RegisterControl(CONTROL_Create(&tAnim)));
	}

	// Campfire (0x6FA63098 - drawn in front of back-row feet with DRAWMODE_MODULATE)
	{
		D2WinControlInitStrc tFire = {};
		tFire.nType = D2WIN_ANIMIMAGE;
		tFire.nX = 345;
		tFire.nY = 454;
		tFire.nWidth = 110;
		tFire.nHeight = 127;
		tFire.field_14 = 1;
		tFire.nStringId = DRAWMODE_MODULATE;
		tFire.ppCellFile = &gpCampfire;
		RegisterControl(CONTROL_Create(&tFire));
	}

	// Character Name Label, Frame Image, EditBox, and Checkboxes (0x6FA634B8..0x6FA636F8)
	{
		D2WinControlInitStrc tNameLbl = {};
		tNameLbl.nType = D2WIN_TEXTBOX;
		tNameLbl.nX = 321;
		tNameLbl.nY = 512;
		tNameLbl.nWidth = 200;
		tNameLbl.nHeight = 32;
		tNameLbl.field_28 = &gFontStyle16;
		gpCreateNameLabelBox = reinterpret_cast<D2WinTextBoxStrc*>(RegisterControl(CONTROL_Create(&tNameLbl)));
		if (gpCreateNameLabelBox)
		{
			const Unicode* pwszCharName = D2LANG_GetStringFromTblIndex(5125);
			if (pwszCharName && pwszCharName[0])
			{
				D2Win_10051(gpCreateNameLabelBox, pwszCharName, 4, 0, 0);
			}
			else
			{
				D2Win_10046(gpCreateNameLabelBox, "Character Name", 4);
			}
		}

		D2WinControlInitStrc tNameImg = {};
		tNameImg.nType = D2WIN_IMAGE;
		tNameImg.nX = 319;
		tNameImg.nY = 519;
		tNameImg.nWidth = 169;
		tNameImg.nHeight = 26;
		tNameImg.ppCellFile = &gpImgTextBox;
		gpCreateNameFrameImg = reinterpret_cast<D2WinImageStrc*>(RegisterControl(CONTROL_Create(&tNameImg)));

		D2WinControlInitStrc tEdit = {};
		tEdit.nType = D2WIN_EDITBOX;
		tEdit.nX = 318;
		tEdit.nY = 510;
		tEdit.nWidth = 157;
		tEdit.nHeight = 16;
		tEdit.field_14 = 8;
		tEdit.nStringId = 4;
		tEdit.field_20 = nullptr;
		tEdit.field_24 = reinterpret_cast<void*>(D2LAUNCH_OnCreateCharConfirm);
		tEdit.field_28 = &gFontStyleFormal12;
		gpCreateNameEditBox = reinterpret_cast<D2WinEditBoxStrc*>(RegisterControl(CONTROL_Create(&tEdit)));
		if (gpCreateNameEditBox)
		{
			D2Win_10081(gpCreateNameEditBox, 16);
			D2Win_10072(gpCreateNameEditBox, reinterpret_cast<int32_t>(D2LAUNCH_EditBoxCharFilter));
			D2Win_10073(gpCreateNameEditBox, D2LAUNCH_OnNameTextChanged);
		}

		// Expansion Character Checkbox & Label (0x6FA636F8, 0x6FA636C8)
		if (bExpansion)
		{
			D2WinControlInitStrc tExpChk = {};
			tExpChk.nType = D2WIN_BUTTON;
			tExpChk.nX = 319;
			tExpChk.nY = 540;
			tExpChk.nWidth = 15;
			tExpChk.nHeight = 16;
			tExpChk.ppCellFile = &gpBtnClickBox;
			tExpChk.field_2C = reinterpret_cast<void*>(BUTTONFLAG_IS_TOGGLE);
			gpCreateExpCheckBox = reinterpret_cast<D2WinButtonStrc*>(RegisterControl(CONTROL_Create(&tExpChk)));
			if (gpCreateExpCheckBox)
			{
				BUTTON_SetIsPressed(gpCreateExpCheckBox, 1);
			}

			D2WinControlInitStrc tExpLbl = {};
			tExpLbl.nType = D2WIN_TEXTBOX;
			tExpLbl.nX = 340;
			tExpLbl.nY = 540;
			tExpLbl.nWidth = 200;
			tExpLbl.nHeight = 16;
			tExpLbl.field_28 = &gFontStyle16;
			gpCreateExpLabelBox = reinterpret_cast<D2WinTextBoxStrc*>(RegisterControl(CONTROL_Create(&tExpLbl)));
			if (gpCreateExpLabelBox)
			{
				const Unicode* pwszExp = D2LANG_GetStringFromTblIndex(22731);
				if (pwszExp && pwszExp[0])
				{
					D2Win_10051(gpCreateExpLabelBox, pwszExp, 4, 0, 0);
				}
				else
				{
					D2Win_10046(gpCreateExpLabelBox, "EXPANSION CHARACTER", 4);
				}
			}
		}

		// Hardcore Checkbox & Label (0x6FA635D8, 0x6FA635A8)
		{
			D2WinControlInitStrc tHcChk = {};
			tHcChk.nType = D2WIN_BUTTON;
			tHcChk.nX = 319;
			tHcChk.nY = 560;
			tHcChk.nWidth = 15;
			tHcChk.nHeight = 16;
			tHcChk.ppCellFile = &gpBtnClickBox;
			tHcChk.field_2C = reinterpret_cast<void*>(BUTTONFLAG_IS_TOGGLE);
			gpCreateHcCheckBox = reinterpret_cast<D2WinButtonStrc*>(RegisterControl(CONTROL_Create(&tHcChk)));

			D2WinControlInitStrc tHcLbl = {};
			tHcLbl.nType = D2WIN_TEXTBOX;
			tHcLbl.nX = 340;
			tHcLbl.nY = 560;
			tHcLbl.nWidth = 200;
			tHcLbl.nHeight = 16;
			tHcLbl.field_28 = &gFontStyle16;
			gpCreateHcLabelBox = reinterpret_cast<D2WinTextBoxStrc*>(RegisterControl(CONTROL_Create(&tHcLbl)));
			if (gpCreateHcLabelBox)
			{
				const Unicode* pwszHc = D2LANG_GetStringFromTblIndex(5126);
				if (pwszHc && pwszHc[0])
				{
					D2Win_10051(gpCreateHcLabelBox, pwszHc, 4, 0, 0);
				}
				else
				{
					D2Win_10046(gpCreateHcLabelBox, "HARDCORE", 4);
				}
			}
		}
	}

	// Bottom EXIT & OK Medium Buttons (0x6FA63008, 0x6FA63038)
	{
		D2WinControlInitStrc tMedBtn = {};
		tMedBtn.nType = D2WIN_BUTTON;
		tMedBtn.nY = 572;
		tMedBtn.nWidth = 128;
		tMedBtn.nHeight = 35;
		tMedBtn.ppCellFile = &gpBtnMediumSelBlank;

		// EXIT (5101)
		tMedBtn.nX = 33;
		tMedBtn.field_14 = VK_ESCAPE;
		tMedBtn.nStringId = 5101;
		tMedBtn.field_20 = D2LAUNCH_OnCreateCancel;
		RegisterControl(CONTROL_Create(&tMedBtn));

		// OK (5102)
		tMedBtn.nX = 627;
		tMedBtn.field_14 = VK_RETURN;
		tMedBtn.nStringId = 5102;
		tMedBtn.field_20 = D2LAUNCH_OnCreateCharConfirm;
		gpCreateOkBtn = reinterpret_cast<D2WinButtonStrc*>(RegisterControl(CONTROL_Create(&tMedBtn)));
	}

	SetCreateNameControlsVisible(0, -1);
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

	STRTABLE_Initialize(0, nullptr, FOG_IsExpansion());

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
	SyncClientModeEnv();

	bool bAutoExitSession = false;
	char szAutoChar[32] = {};
	DWORD nAutoCharLen = GetEnvironmentVariableA("D2MOO_AUTO_CHAR", szAutoChar, sizeof(szAutoChar));
	if (nAutoCharLen == 0 || szAutoChar[0] == '\0')
	{
		char szAutotest[16] = {};
		const char* szCmdLine = GetCommandLineA();
		if ((GetEnvironmentVariableA("D2MOO_NATIVE_AUTOTEST", szAutotest, sizeof(szAutotest)) > 0 && szAutotest[0] == '1') ||
		    (szCmdLine && strstr(szCmdLine, "--autotest-native") != nullptr))
		{
			SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
			SetEnvironmentVariableA("D2MOO_NATIVE_AUTOTEST", "1");
			SetEnvironmentVariableA("D2MOO_USE_NATIVE_CLIENT", "1");
			SetEnvironmentVariableA("D2MOO_USE_TS_CLIENT", "0");
			strcpy_s(szAutoChar, "ExpansionHero");
			nAutoCharLen = static_cast<DWORD>(strlen(szAutoChar));
		}
	}
	if (nAutoCharLen > 0 && szAutoChar[0] != '\0')
	{
		if (pCfg->bShownLogo)
		{
			// Returned from D2Client after an auto-entered session: exit cleanly
			gdwNextModule = D2LAUNCH_MODULE_NONE;
			bAutoExitSession = true;
		}
		else
		{
			pCfg->bShownLogo = 1;
			gnLaunchMode = 0;
			EnumerateSavedCharacters();
			D2LaunchCharNodeStrc* pMatch = nullptr;
			for (D2LaunchCharNodeStrc* pNode = gpCharListHead; pNode != nullptr; pNode = pNode->pNext)
			{
				if (_stricmp(pNode->szCharName, szAutoChar) == 0)
				{
					pMatch = pNode;
					break;
				}
			}

			if (pMatch)
			{
				gpSelectedChar = pMatch;
				pCfg->tCharPreviewInfo = pMatch->tPreview;
				pCfg->unpackedCTemp.nCharacterClassId = pMatch->nCharClass;
				D2LAUNCH_EnterSelectedCharacter(pMatch->szCharName, "", pMatch->wSaveFlags);
			}
			else
			{
				char szAutoClass[16] = {};
				uint8_t nClass = 1; // Default: Sorceress
				if (GetEnvironmentVariableA("D2MOO_AUTO_CLASS", szAutoClass, sizeof(szAutoClass)) > 0)
				{
					const int nParsed = std::atoi(szAutoClass);
					if (nParsed >= 0 && nParsed < 7)
					{
						nClass = static_cast<uint8_t>(nParsed);
					}
				}
				const uint16_t wSaveFlags = FOG_IsExpansion() ? 0x20u : 0x00u;
				SStrCopy(pCfg->szName, szAutoChar, sizeof(pCfg->szName));
				pCfg->unpackedCTemp.nCharacterSaveFlags = wSaveFlags;
				pCfg->unpackedCTemp.nCharacterClassId = nClass;
				D2LAUNCH_InitNewSaveAndPreview(
					&pCfg->tCharPreviewInfo,
					nClass,
					pCfg->szName,
					wSaveFlags);
				D2LAUNCH_WriteNewSaveFile(pCfg->szName);
				pCfg->nArenaFlags = (wSaveFlags & 0x20u) ? 0x100004u : 0x000004u;
				pCfg->nLaunchType = LAUNCHTYPE_LOCAL;
				pCfg->dwGameType = 0;
				gdwNextModule = D2LAUNCH_MODULE_CLIENT;
			}
		}
	}
	else if (pCfg->bSkipToBNet && !pCfg->bShownLogo)
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
		if (D2LAUNCH_HasSavedCharacters())
		{
			D2LAUNCH_EnterCharSelect();
		}
		else
		{
			D2LAUNCH_EnterCharCreate();
		}
	}
	else
	{
		D2LAUNCH_EnterMainMenu(0);
	}

	if (gdwNextModule == D2LAUNCH_MODULE_NONE && !bAutoExitSession)
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
