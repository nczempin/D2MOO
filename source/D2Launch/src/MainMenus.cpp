#include "D2Launch.h"

#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <Fog.h>
#include <Safesock.h>
#include <Storm.h>
#include <D2Lang.h>
#include <D2StrTable.h>
#include <D2Gfx.h>
#include <Window.h>
#include <D2Sound.h>
#include <Client.h>
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
#include <D2WinSmack.h>
#include <D2WinTimer.h>

// ============================================================================
// 1.12a .data Font+Color Style Descriptors (0x6FA60BD8..0x6FA60C18)
// ============================================================================
D2FontStyleStrc gFontStyle42          = { D2FONT_FONT42,       0 }; // 0x6FA60BD8
D2FontStyleStrc gFontStyle30          = { D2FONT_FONT30,       0 }; // 0x6FA60BE0
D2FontStyleStrc gFontStyle24          = { D2FONT_FONT24,       0 }; // 0x6FA60BE8
D2FontStyleStrc gFontStyle16          = { D2FONT_FONT16,       0 }; // 0x6FA60BF0
D2FontStyleStrc gFontStyle8Gold       = { D2FONT_FONT8,        4 }; // 0x6FA60BF8
D2FontStyleStrc gFontStyleFormal10Red = { D2FONT_FONTFORMAL10, 1 }; // 0x6FA60C00
D2FontStyleStrc gFontStyleFormal12    = { D2FONT_FONTFORMAL12, 0 }; // 0x6FA60C08
D2FontStyleStrc gFontStyleFormal10    = { D2FONT_FONTFORMAL10, 0 }; // 0x6FA60C10
D2FontStyleStrc gFontStyleFormal11    = { D2FONT_FONTFORMAL11, 0 }; // 0x6FA60C18

// ============================================================================
// 1.12a Globals (0x6FA64DA8..0x6FA65ED8)
// ============================================================================
D2ConfigStrc* gpD2Config       = nullptr;               // 0x6FA65ACC
int32_t gdwNextModule          = D2LAUNCH_MODULE_NONE;  // 0x6FA65AE0
int32_t gnLaunchMode           = 0;                     // 0x6FA65AE4 (0=SinglePlayer, 1=Realm, 2=TCPIP, 3=OpenBNet)
int32_t gbModalOpen            = 0;                     // 0x6FA65E94
int32_t gdwAllowHardcore       = 0;                     // 0x6FA65E98
D2SaveHeaderStrc gD2SaveHeader = {};                    // 0x6FA652D0

static constexpr int kMaxScreenControls = 64;
static constexpr int kMaxModalControls  = 64;
static D2WinControlStrc* gapScreenControls[kMaxScreenControls] = {}; // 0x6FA65448
static int32_t gnScreenControlCount = 0;                             // 0x6FA65E40
static D2WinControlStrc* gapModalControls[kMaxModalControls] = {};   // 0x6FA65550
static int32_t gnModalControlCount = 0;                              // 0x6FA65E44
static void(__stdcall* gpfModalAlertCallback)(int32_t) = nullptr;    // 0x6FA656BC

// ============================================================================
// 1.12a CellFile Pointers (0x6FA65BF4..0x6FA65DC0)
// ============================================================================
static D2CellFileStrc* gpImgBlizNo          = nullptr; // 0x6FA65BF4
static D2CellFileStrc* gpBgTitleScreen      = nullptr; // 0x6FA65BF8
static D2CellFileStrc* gpImgDiablo2Logo     = nullptr; // 0x6FA65C00
static D2CellFileStrc* gpLogoBlackLeft      = nullptr; // 0x6FA65C04
static D2CellFileStrc* gpLogoFireLeft       = nullptr; // 0x6FA65C08
static D2CellFileStrc* gpLogoBlackRight     = nullptr; // 0x6FA65C0C
static D2CellFileStrc* gpLogoFireRight      = nullptr; // 0x6FA65C10
static D2CellFileStrc* gpBgGameSelect       = nullptr; // 0x6FA65C14
static D2CellFileStrc* gpCursorCellFile     = nullptr; // 0x6FA65C18
static D2CellFileStrc* gpBgCredits          = nullptr; // 0x6FA65C1C
static D2CellFileStrc* gpBgCharCreate       = nullptr; // 0x6FA65C20
static D2CellFileStrc* gpCampfire           = nullptr; // 0x6FA65C24
static D2CellFileStrc* gpBtnRealmSelect     = nullptr; // 0x6FA65C28
D2CellFileStrc* gpBtnTallBlank              = nullptr; // 0x6FA65C2C
D2CellFileStrc* gpBgCharSelectExp           = nullptr; // 0x6FA65C30
D2CellFileStrc* gpBtnWideBlank              = nullptr; // 0x6FA65C34
D2CellFileStrc* gpBtnMediumBlank            = nullptr; // 0x6FA65C38
D2CellFileStrc* gpBtnShortBlank             = nullptr; // 0x6FA65C3C
static D2CellFileStrc* gpBtnMediumSelBlank  = nullptr; // 0x6FA65C40
D2CellFileStrc* gpImgTextBox2               = nullptr; // 0x6FA65C44
static D2CellFileStrc* gpBtn3WideBlank      = nullptr; // 0x6FA65C48
static D2CellFileStrc* gpImgEditTextBoxWide = nullptr; // 0x6FA65C4C
static D2CellFileStrc* gpBtnWideBlank02     = nullptr; // 0x6FA65C50
static D2CellFileStrc* gpBtnNarrowBlank     = nullptr; // 0x6FA65C54
static D2CellFileStrc* gpImgGWListBox       = nullptr; // 0x6FA65C58
static D2CellFileStrc* gpBgTermsOfUse       = nullptr; // 0x6FA65C60
D2CellFileStrc* gpBgCharSelectClassic       = nullptr; // 0x6FA65C64
D2CellFileStrc* gpDifficultyLevels          = nullptr; // 0x6FA65C68
static D2CellFileStrc* gpImgCinematics      = nullptr; // 0x6FA65C6C
static D2CellFileStrc* gpImgCinematicsExp   = nullptr; // 0x6FA65C70
D2CellFileStrc* gpCharSelectBox             = nullptr; // 0x6FA65C74
D2CellFileStrc* gpCharSelectBoxGrey         = nullptr; // 0x6FA65C78
D2CellFileStrc* gpPopUpOKCancel             = nullptr; // 0x6FA65C7C
D2CellFileStrc* gpPopUpOKCancel2            = nullptr; // 0x6FA65C80
D2CellFileStrc* gpSelectCharArrows          = nullptr; // 0x6FA65C84
D2CellFileStrc* gpCreateNewCharImage        = nullptr; // 0x6FA65C90
D2CellFileStrc* gpPopUpOK                   = nullptr; // 0x6FA65C94
static D2CellFileStrc* gpPopUp340x224       = nullptr; // 0x6FA65C98
static D2CellFileStrc* gpPopUpWide          = nullptr; // 0x6FA65C9C
static D2CellFileStrc* gpPopUpLargest       = nullptr; // 0x6FA65CA0
static D2CellFileStrc* gpTileableDialog     = nullptr; // 0x6FA65CA4
D2CellFileStrc* gpPopUpOK2                  = nullptr; // 0x6FA65CA8
D2CellFileStrc* gpBtnOkCancelBlank          = nullptr; // 0x6FA65CAC
D2CellFileStrc* gpBtnCancelBlank            = nullptr; // 0x6FA65CB0
static D2CellFileStrc* gpImgTextBox         = nullptr; // 0x6FA65CB8
static D2CellFileStrc* gpBtnClickBox        = nullptr; // 0x6FA65CBC
static D2CellFileStrc* gpImgClickBoxGrey    = nullptr; // 0x6FA65CC0
D2CellFileStrc* gpJoinGameScrollBars        = nullptr; // 0x6FA65CC8
static D2CellFileStrc* gpBgTcpIpScreen      = nullptr; // 0x6FA65CCC
static D2CellFileStrc* gpImgIPAddressBox    = nullptr; // 0x6FA65CD0

// 7 Character Classes around the Campfire (0=Amazon, 1=Sorceress, 2=Necromancer, 3=Paladin, 4=Barbarian, 5=Druid, 6=Assassin)
static D2CellFileStrc* gpClassNu1[7]  = {}; // 0x6FA65CDC..0x6FA65DC0
static D2CellFileStrc* gpClassNu2[7]  = {};
static D2CellFileStrc* gpClassNu3[7]  = {};
static D2CellFileStrc* gpClassNu3s[7] = {};
static D2CellFileStrc* gpClassFw[7]   = {};
static D2CellFileStrc* gpClassFws[7]  = {};
static D2CellFileStrc* gpClassBw[7]   = {};
static D2CellFileStrc* gpClassBws[7]  = {};

// ============================================================================
// 1.12a Animation Descriptors (0x6FA60C20..0x6FA60EA0)
// ============================================================================
static D2AnimatedImageDescriptor gAnimLogoLeft[5] = { // 0x6FA60E50
	{ &gpLogoBlackLeft, &gpLogoFireLeft, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackLeft, &gpLogoFireLeft, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackLeft, &gpLogoFireLeft, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackLeft, &gpLogoFireLeft, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackLeft, &gpLogoFireLeft, DRAWMODE_MODULATE, 1 },
};
static D2AnimatedImageDescriptor gAnimLogoRight[5] = { // 0x6FA60EA0
	{ &gpLogoBlackRight, &gpLogoFireRight, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackRight, &gpLogoFireRight, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackRight, &gpLogoFireRight, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackRight, &gpLogoFireRight, DRAWMODE_MODULATE, 1 },
	{ &gpLogoBlackRight, &gpLogoFireRight, DRAWMODE_MODULATE, 1 },
};

static D2AnimatedImageDescriptor gAnimCharClass[7][5] = {
	// 0: Amazon (0x6FA60C20)
	{
		{ &gpClassNu1[0], nullptr,         DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[0], nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[0],  nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassNu3[0], nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassBw[0],  nullptr,         DRAWMODE_NORMAL,   1 },
	},
	// 1: Sorceress (0x6FA60C70)
	{
		{ &gpClassNu1[1], nullptr,         DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[1], nullptr,         DRAWMODE_NORMAL,   2 },
		{ &gpClassFw[1],  &gpClassFws[1],  DRAWMODE_MODULATE, 1 },
		{ &gpClassNu3[1], &gpClassNu3s[1], DRAWMODE_MODULATE, 1 },
		{ &gpClassBw[1],  &gpClassBws[1],  DRAWMODE_MODULATE, 1 },
	},
	// 2: Necromancer (0x6FA60CC0)
	{
		{ &gpClassNu1[2], nullptr,         DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[2], nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[2],  &gpClassFws[2],  DRAWMODE_MODULATE, 1 },
		{ &gpClassNu3[2], &gpClassNu3s[2], DRAWMODE_MODULATE, 1 },
		{ &gpClassBw[2],  &gpClassBws[2],  DRAWMODE_MODULATE, 1 },
	},
	// 3: Paladin (0x6FA60D10)
	{
		{ &gpClassNu1[3], nullptr,         DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[3], nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[3],  &gpClassFws[3],  DRAWMODE_MODULATE, 1 },
		{ &gpClassNu3[3], nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassBw[3],  nullptr,         DRAWMODE_NORMAL,   1 },
	},
	// 4: Barbarian (0x6FA60D60)
	{
		{ &gpClassNu1[4], nullptr,         DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[4], nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[4],  &gpClassFws[4],  DRAWMODE_NORMAL,   1 },
		{ &gpClassNu3[4], nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassBw[4],  nullptr,         DRAWMODE_NORMAL,   1 },
	},
	// 5: Druid (0x6FA60DB0)
	{
		{ &gpClassNu1[5], nullptr,         DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[5], nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[5],  nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassNu3[5], nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassBw[5],  nullptr,         DRAWMODE_NORMAL,   1 },
	},
	// 6: Assassin (0x6FA60E00)
	{
		{ &gpClassNu1[6], nullptr,         DRAWMODE_NORMAL,   2 },
		{ &gpClassNu2[6], nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassFw[6],  nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassNu3[6], nullptr,         DRAWMODE_NORMAL,   1 },
		{ &gpClassBw[6],  nullptr,         DRAWMODE_NORMAL,   1 },
	},
};

// ============================================================================
// Forward Declarations of Internal 1.12a Callbacks
// ============================================================================
static int __stdcall D2LAUNCH_OnTrademarkDismiss(SMSGHANDLER_PARAMS* pMsg);   // 0x6FA57240
static int __stdcall D2LAUNCH_OnMainMenuCancel(SMSGHANDLER_PARAMS* pMsg);     // 0x6FA57210
static int __stdcall D2LAUNCH_OnSinglePlayerClick(SMSGHANDLER_PARAMS* pMsg);  // 0x6FA59A70
static int __stdcall D2LAUNCH_OnExitClick(SMSGHANDLER_PARAMS* pMsg);          // 0x6FA506D0
static int __stdcall D2LAUNCH_OnTcpIpMenuClick(SMSGHANDLER_PARAMS* pMsg);     // 0x6FA57320
static int __stdcall D2LAUNCH_OnTcpIpHostClick(SMSGHANDLER_PARAMS* pMsg);     // 0x6FA599E0
static int __stdcall D2LAUNCH_OnTcpIpJoinPopup(SMSGHANDLER_PARAMS* pMsg);     // 0x6FA524A0
static int __stdcall D2LAUNCH_OnTcpIpJoinCancel(SMSGHANDLER_PARAMS* pMsg);    // 0x6FA52400
static int __stdcall D2LAUNCH_OnTcpIpJoinConfirm(SMSGHANDLER_PARAMS* pMsg);   // 0x6FA59830
static int __stdcall D2LAUNCH_OnTcpIpHostHover(SMSGHANDLER_PARAMS* pMsg);     // 0x6FA51040
static int __stdcall D2LAUNCH_OnTcpIpJoinHover(SMSGHANDLER_PARAMS* pMsg);     // 0x6FA51000
static int __stdcall D2LAUNCH_OnTcpIpRefreshTimer(SMSGHANDLER_PARAMS* pMsg);  // 0x6FA53270
static int __stdcall D2LAUNCH_OnCreditsExit(SMSGHANDLER_PARAMS* pMsg);        // 0x6FA571B0
static int __stdcall D2LAUNCH_OnCreditsScrollTimer(SMSGHANDLER_PARAMS* pMsg); // 0x6FA57660
static int __stdcall D2LAUNCH_OnCinematicsCancel(SMSGHANDLER_PARAMS* pMsg);   // 0x6FA57340
static int __stdcall D2LAUNCH_OnCinematic1(SMSGHANDLER_PARAMS* pMsg);         // 0x6FA586E0
static int __stdcall D2LAUNCH_OnCinematic2(SMSGHANDLER_PARAMS* pMsg);         // 0x6FA58690
static int __stdcall D2LAUNCH_OnCinematic3(SMSGHANDLER_PARAMS* pMsg);         // 0x6FA58640
static int __stdcall D2LAUNCH_OnCinematic4(SMSGHANDLER_PARAMS* pMsg);         // 0x6FA585F0
static int __stdcall D2LAUNCH_OnCinematic5(SMSGHANDLER_PARAMS* pMsg);         // 0x6FA585A0
static int __stdcall D2LAUNCH_OnCinematic6(SMSGHANDLER_PARAMS* pMsg);         // 0x6FA58550
static int __stdcall D2LAUNCH_OnCinematic7(SMSGHANDLER_PARAMS* pMsg);         // 0x6FA58500
static int __stdcall D2LAUNCH_OnModalAlertOk(SMSGHANDLER_PARAMS* pMsg);       // 0x6FA51620
static int __stdcall D2LAUNCH_OnCreateCancel(SMSGHANDLER_PARAMS* pMsg);       // 0x6FA572C0
static int __stdcall D2LAUNCH_OnCreateOkClick(SMSGHANDLER_PARAMS* pMsg);      // 0x6FA59AB0
static int __stdcall D2LAUNCH_OnHardcoreCancel(SMSGHANDLER_PARAMS* pMsg);     // 0x6FA56180
static int __stdcall D2LAUNCH_OnHardcoreConfirm(SMSGHANDLER_PARAMS* pMsg);    // 0x6FA59B80
static int __stdcall D2LAUNCH_OnCampfireClassClick(SMSGHANDLER_PARAMS* pMsg); // 0x6FA52C00
static int __stdcall D2LAUNCH_OnCampfireHoverBarb(SMSGHANDLER_PARAMS* pMsg);  // 0x6FA50D60
static int __stdcall D2LAUNCH_OnCampfireHoverNecro(SMSGHANDLER_PARAMS* pMsg); // 0x6FA50D90
static int __stdcall D2LAUNCH_OnCampfireHoverAma(SMSGHANDLER_PARAMS* pMsg);   // 0x6FA50DC0
static int __stdcall D2LAUNCH_OnCampfireHoverSorc(SMSGHANDLER_PARAMS* pMsg);  // 0x6FA50DF0
static int __stdcall D2LAUNCH_OnCampfireHoverPal(SMSGHANDLER_PARAMS* pMsg);   // 0x6FA50E20
static int __stdcall D2LAUNCH_OnCampfireHoverDru(SMSGHANDLER_PARAMS* pMsg);   // 0x6FA52B20
static int __stdcall D2LAUNCH_OnCampfireHoverAssa(SMSGHANDLER_PARAMS* pMsg);  // 0x6FA52B60

// ============================================================================
// 1.12a .data Control Descriptors (0x6FA60F98..0x6FA643B8)
// ============================================================================
// Logo / Trademark Screen (0x6FA60F98..0x6FA61088)
static D2WinControlInitStrc gCtrlTrademarkBg    = { D2WIN_IMAGE,     0,   599, 800, 600, 0,    0, &gpBgTitleScreen, D2LAUNCH_OnTrademarkDismiss, nullptr, reinterpret_cast<void*>(D2LAUNCH_OnTrademarkDismiss), nullptr }; // 0x6FA60F98
static D2WinControlInitStrc gCtrlTrademarkTimer = { D2WIN_TIMER,     9,   0,   0,   0,   0,    0, nullptr,          D2LAUNCH_OnTrademarkDismiss, nullptr, nullptr, nullptr }; // 0x6FA60FC8
static D2WinControlInitStrc gCtrlCopyrightBox   = { D2WIN_TEXTBOX,   100, 580, 600, 80,  0,    0, nullptr,          nullptr,                     nullptr, &gFontStyleFormal12, reinterpret_cast<void*>(2) }; // 0x6FA60FF8
static D2WinControlInitStrc gCtrlBurningLogoL   = { D2WIN_ANIMIMAGE, 400, 120, 181, 170, 0,    0, nullptr,          nullptr,                     gAnimLogoLeft,  nullptr, nullptr }; // 0x6FA61058
static D2WinControlInitStrc gCtrlBurningLogoR   = { D2WIN_ANIMIMAGE, 400, 120, 188, 177, 0,    0, nullptr,          nullptr,                     gAnimLogoRight, nullptr, nullptr }; // 0x6FA61088
static D2WinControlInitStrc gCtrlMainMenuBg     = { D2WIN_IMAGE,     0,   599, 800, 600, 0,    0, &gpBgGameSelect,  nullptr,                     nullptr, nullptr, nullptr }; // 0x6FA610B8

// Expansion Main Menu (0x6FA612C8..0x6FA613E8 + 0x6FA64328)
static D2WinControlInitStrc gCtrlBtnSinglePlr   = { D2WIN_BUTTON,    264, 324, 272, 35,  0,    5106, &gpBtnWideBlank,   D2LAUNCH_OnSinglePlayerClick, nullptr, nullptr, nullptr }; // 0x6FA612C8
static D2WinControlInitStrc gCtrlBtnBattleNet   = { D2WIN_BUTTON,    264, 366, 272, 35,  0,    5107, &gpBtnWideBlank02, nullptr,                      nullptr, nullptr, nullptr }; // 0x6FA612F8
static D2WinControlInitStrc gCtrlBtnOtherMulti  = { D2WIN_BUTTON,    264, 433, 272, 35,  0,    5108, &gpBtnWideBlank,   D2LAUNCH_EnterOtherMultiplayer, nullptr, nullptr, nullptr }; // 0x6FA61328
static D2WinControlInitStrc gCtrlBtnExitGame    = { D2WIN_BUTTON,    264, 568, 272, 35,  VK_ESCAPE, 5109, &gpBtnWideBlank, D2LAUNCH_OnExitClick,      nullptr, nullptr, nullptr }; // 0x6FA61358
static D2WinControlInitStrc gCtrlBtnCredits     = { D2WIN_BUTTON,    264, 528, 135, 25,  0,    5110, &gpBtnShortBlank,  D2LAUNCH_EnterCreditsScreen,    nullptr, nullptr, nullptr }; // 0x6FA613E8
static D2WinControlInitStrc gCtrlBtnCinematics  = { D2WIN_BUTTON,    402, 528, 135, 25,  0,    5111, &gpBtnShortBlank,  D2LAUNCH_EnterCinematicsScreen, nullptr, nullptr, nullptr }; // 0x6FA613B8
static D2WinControlInitStrc gCtrlVersionBox     = { D2WIN_TEXTBOX,   0,   599, 200, 40,  0,    0,    nullptr,           nullptr,                      nullptr, &gFontStyleFormal12, nullptr }; // 0x6FA64328

// Other Multiplayer Menu (0x6FA61628..0x6FA61688)
static D2WinControlInitStrc gCtrlBtnOpenBNet    = { D2WIN_BUTTON,    264, 310, 272, 35,  0,    5115, &gpBtnWideBlank,   nullptr,                     nullptr, nullptr, nullptr }; // 0x6FA61628
static D2WinControlInitStrc gCtrlBtnTcpIpGame   = { D2WIN_BUTTON,    264, 350, 272, 35,  0,    5116, &gpBtnWideBlank,   D2LAUNCH_OnTcpIpMenuClick,   nullptr, nullptr, nullptr }; // 0x6FA61658
static D2WinControlInitStrc gCtrlBtnMultiCancel = { D2WIN_BUTTON,    264, 568, 272, 35,  VK_ESCAPE, 5103, &gpBtnWideBlank, D2LAUNCH_OnMainMenuCancel, nullptr, nullptr, nullptr }; // 0x6FA61688

// Credits Screen (0x6FA616E8..0x6FA61868)
static D2WinControlInitStrc gCtrlCreditsBg      = { D2WIN_IMAGE,     0,   599, 800, 600, 0,    0,    &gpBgCredits,      nullptr,                        nullptr, nullptr, nullptr }; // 0x6FA616E8
static D2WinControlInitStrc gCtrlCreditsExit    = { D2WIN_BUTTON,    33,  578, 128, 35,  VK_ESCAPE, 5101, &gpBtnMediumBlank, D2LAUNCH_OnCreditsExit,    nullptr, nullptr, nullptr }; // 0x6FA61718
static D2WinControlInitStrc gCtrlCreditsClLeft  = { D2WIN_TEXTBOX,   220, 560, 250, 470, 0,    0,    nullptr,           nullptr,                        nullptr, &gFontStyleFormal10, reinterpret_cast<void*>(0x90) }; // 0x6FA61748
static D2WinControlInitStrc gCtrlCreditsClRight = { D2WIN_TEXTBOX,   230, 560, 250, 470, 0,    0,    nullptr,           nullptr,                        nullptr, &gFontStyleFormal10, reinterpret_cast<void*>(0x80) }; // 0x6FA61778
static D2WinControlInitStrc gCtrlCreditsClHdr   = { D2WIN_TEXTBOX,   100, 560, 250, 470, 0,    0,    nullptr,           nullptr,                        nullptr, &gFontStyleFormal10Red, reinterpret_cast<void*>(0x82) }; // 0x6FA617A8
static D2WinControlInitStrc gCtrlCreditsTimer   = { D2WIN_TIMER,     50,  0,   0,   0,   0,    0,    nullptr,           D2LAUNCH_OnCreditsScrollTimer,  nullptr, nullptr, nullptr }; // 0x6FA617D8
static D2WinControlInitStrc gCtrlCreditsExpLeft = { D2WIN_TEXTBOX,   400, 615, 250, 610, 0,    0,    nullptr,           nullptr,                        nullptr, &gFontStyleFormal10, reinterpret_cast<void*>(0x90) }; // 0x6FA61808
static D2WinControlInitStrc gCtrlCreditsExpRght = { D2WIN_TEXTBOX,   410, 615, 250, 610, 0,    0,    nullptr,           nullptr,                        nullptr, &gFontStyleFormal10, reinterpret_cast<void*>(0x80) }; // 0x6FA61838
static D2WinControlInitStrc gCtrlCreditsExpHdr  = { D2WIN_TEXTBOX,   280, 615, 250, 610, 0,    0,    nullptr,           nullptr,                        nullptr, &gFontStyleFormal10Red, reinterpret_cast<void*>(0x82) }; // 0x6FA61868

// Cinematics Selection Screen (0x6FA61AA8..0x6FA61C58)
static D2WinControlInitStrc gCtrlCinematicsPanel= { D2WIN_IMAGE,     237, 505, 326, 427, 0,    0,     &gpImgCinematicsExp, nullptr,                   nullptr, nullptr, nullptr }; // 0x6FA61AA8
static D2WinControlInitStrc gCtrlCinematicsTitle= { D2WIN_TEXTBOX,   262, 153, 272, 35,  0,    0,     nullptr,             nullptr,                   nullptr, &gFontStyle30, reinterpret_cast<void*>(2) }; // 0x6FA61AD8
static D2WinControlInitStrc gCtrlCinematicBtn1  = { D2WIN_BUTTON,    264, 160, 272, 35,  0,    21797, &gpBtnWideBlank,     D2LAUNCH_OnCinematic1,     nullptr, nullptr, nullptr }; // 0x6FA61B08
static D2WinControlInitStrc gCtrlCinematicBtn2  = { D2WIN_BUTTON,    264, 205, 272, 35,  0,    21798, &gpBtnWideBlank,     D2LAUNCH_OnCinematic2,     nullptr, nullptr, nullptr }; // 0x6FA61B38
static D2WinControlInitStrc gCtrlCinematicBtn3  = { D2WIN_BUTTON,    264, 250, 272, 35,  0,    21799, &gpBtnWideBlank,     D2LAUNCH_OnCinematic3,     nullptr, nullptr, nullptr }; // 0x6FA61B68
static D2WinControlInitStrc gCtrlCinematicBtn4  = { D2WIN_BUTTON,    264, 295, 272, 35,  0,    21800, &gpBtnWideBlank,     D2LAUNCH_OnCinematic4,     nullptr, nullptr, nullptr }; // 0x6FA61B98
static D2WinControlInitStrc gCtrlCinematicBtn5  = { D2WIN_BUTTON,    264, 340, 272, 35,  0,    21801, &gpBtnWideBlank,     D2LAUNCH_OnCinematic5,     nullptr, nullptr, nullptr }; // 0x6FA61BC8
static D2WinControlInitStrc gCtrlCinematicBtn6  = { D2WIN_BUTTON,    264, 385, 272, 35,  0,    21802, &gpBtnWideBlank,     D2LAUNCH_OnCinematic6,     nullptr, nullptr, nullptr }; // 0x6FA61BF8
static D2WinControlInitStrc gCtrlCinematicBtn7  = { D2WIN_BUTTON,    264, 430, 272, 35,  0,    21803, &gpBtnWideBlank,     D2LAUNCH_OnCinematic7,     nullptr, nullptr, nullptr }; // 0x6FA61C28
static D2WinControlInitStrc gCtrlCinematicCancel= { D2WIN_BUTTON,    334, 488, 128, 35,  VK_ESCAPE, 5103, &gpBtnMediumBlank, D2LAUNCH_OnCinematicsCancel, nullptr, nullptr, nullptr }; // 0x6FA61C58

// Modal Alert Popup (0x6FA626D8..0x6FA62768)
static D2WinControlInitStrc gCtrlAlertPopupBg   = { D2WIN_IMAGE,     268, 350, 264, 176, 0,    0,    &gpPopUpOK2,       nullptr,                 nullptr, nullptr, nullptr }; // 0x6FA626D8
static D2WinControlInitStrc gCtrlAlertPopupOk   = { D2WIN_BUTTON,    351, 337, 96,  32,  VK_RETURN, 5102, &gpBtnOkCancelBlank, D2LAUNCH_OnModalAlertOk, nullptr, nullptr, nullptr }; // 0x6FA62708
static D2WinControlInitStrc gCtrlAlertPopupText = { D2WIN_TEXTBOX,   288, 292, 224, 100, 0,    0,    nullptr,           nullptr,                 nullptr, &gFontStyleFormal12, reinterpret_cast<void*>(6) }; // 0x6FA62768

// Campfire Character Create Screen (0x6FA62FD8..0x6FA63818)
static D2WinControlInitStrc gCtrlCreateTopTitle = { D2WIN_TEXTBOX,   0,   80,  800, 50,  0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyle30, reinterpret_cast<void*>(2) }; // 0x6FA62FD8
static D2WinControlInitStrc gCtrlCreateExit     = { D2WIN_BUTTON,    33,  572, 128, 35,  VK_ESCAPE, 5101, &gpBtnMediumSelBlank, D2LAUNCH_OnCreateCancel, nullptr, nullptr, nullptr }; // 0x6FA63008
static D2WinControlInitStrc gCtrlCreateOk       = { D2WIN_BUTTON,    627, 572, 128, 35,  VK_RETURN, 5102, &gpBtnMediumSelBlank, D2LAUNCH_OnCreateOkClick, nullptr, nullptr, nullptr }; // 0x6FA63038
static D2WinControlInitStrc gCtrlCreateBg       = { D2WIN_IMAGE,     0,   599, 800, 600, 0,    0,    &gpBgCharCreate,     nullptr,                      nullptr, nullptr, nullptr }; // 0x6FA63068
static D2WinControlInitStrc gCtrlCreateFireExp  = { D2WIN_ANIMIMAGE, 345, 470, 110, 127, 1,    DRAWMODE_MODULATE, &gpCampfire, nullptr,             nullptr, nullptr, nullptr }; // 0x6FA63098
static D2WinControlInitStrc gCtrlCreateFireCl   = { D2WIN_ANIMIMAGE, 345, 454, 110, 127, 1,    DRAWMODE_MODULATE, &gpCampfire, nullptr,             nullptr, nullptr, nullptr }; // 0x6FA630C8
static D2WinControlInitStrc gCtrlAnimBarb       = { D2WIN_ANIMIMAGE, 400, 330, 88,  184, 0,    0,    nullptr,             D2LAUNCH_OnCampfireClassClick, gAnimCharClass[4], reinterpret_cast<void*>(D2LAUNCH_OnCampfireHoverBarb), nullptr }; // 0x6FA63158
static D2WinControlInitStrc gCtrlAnimNecro      = { D2WIN_ANIMIMAGE, 301, 333, 88,  184, 0,    0,    nullptr,             D2LAUNCH_OnCampfireClassClick, gAnimCharClass[2], reinterpret_cast<void*>(D2LAUNCH_OnCampfireHoverNecro), nullptr }; // 0x6FA63128
static D2WinControlInitStrc gCtrlAnimAma        = { D2WIN_ANIMIMAGE, 100, 337, 88,  184, 0,    0,    nullptr,             D2LAUNCH_OnCampfireClassClick, gAnimCharClass[0], reinterpret_cast<void*>(D2LAUNCH_OnCampfireHoverAma), nullptr }; // 0x6FA63278
static D2WinControlInitStrc gCtrlAnimSorc       = { D2WIN_ANIMIMAGE, 626, 353, 88,  184, 0,    0,    nullptr,             D2LAUNCH_OnCampfireClassClick, gAnimCharClass[1], reinterpret_cast<void*>(D2LAUNCH_OnCampfireHoverSorc), nullptr }; // 0x6FA63218
static D2WinControlInitStrc gCtrlAnimPal        = { D2WIN_ANIMIMAGE, 521, 339, 88,  184, 0,    0,    nullptr,             D2LAUNCH_OnCampfireClassClick, gAnimCharClass[3], reinterpret_cast<void*>(D2LAUNCH_OnCampfireHoverPal), nullptr }; // 0x6FA63248
static D2WinControlInitStrc gCtrlAnimDruid      = { D2WIN_ANIMIMAGE, 720, 370, 88,  184, 0,    0,    nullptr,             D2LAUNCH_OnCampfireClassClick, gAnimCharClass[5], reinterpret_cast<void*>(D2LAUNCH_OnCampfireHoverDru), nullptr }; // 0x6FA63698
static D2WinControlInitStrc gCtrlAnimAssa       = { D2WIN_ANIMIMAGE, 232, 364, 88,  184, 0,    0,    nullptr,             D2LAUNCH_OnCampfireClassClick, gAnimCharClass[6], reinterpret_cast<void*>(D2LAUNCH_OnCampfireHoverAssa), nullptr }; // 0x6FA63668
static D2WinControlInitStrc gCtrlCreateHcLblExp = { D2WIN_TEXTBOX,   340, 560, 200, 16,  0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyle16, nullptr }; // 0x6FA632A8
static D2WinControlInitStrc gCtrlCreateHcChkExp = { D2WIN_BUTTON,    319, 560, 15,  16,  0,    0,    &gpBtnClickBox,      nullptr,                      nullptr, nullptr, reinterpret_cast<void*>(BUTTONFLAG_IS_TOGGLE) }; // 0x6FA632D8
static D2WinControlInitStrc gCtrlCreateClsName  = { D2WIN_TEXTBOX,   0,   180, 800, 100, 0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyle30, reinterpret_cast<void*>(2) }; // 0x6FA63428
static D2WinControlInitStrc gCtrlCreateClsDesc  = { D2WIN_TEXTBOX,   250, 210, 300, 100, 0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyle16, reinterpret_cast<void*>(2) }; // 0x6FA63458
static D2WinControlInitStrc gCtrlCreateNameImg  = { D2WIN_IMAGE,     319, 519, 169, 26,  0,    0,    &gpImgTextBox,       nullptr,                      nullptr, nullptr, nullptr }; // 0x6FA634B8
static D2WinControlInitStrc gCtrlCreateNameLbl  = { D2WIN_TEXTBOX,   321, 493, 200, 32,  0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyle16, nullptr }; // 0x6FA634E8
static D2WinControlInitStrc gCtrlCreateHcLblCl  = { D2WIN_TEXTBOX,   340, 540, 200, 16,  0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyle16, nullptr }; // 0x6FA63518
static D2WinControlInitStrc gCtrlCreateHcChkCl  = { D2WIN_BUTTON,    319, 540, 15,  16,  0,    0,    &gpBtnClickBox,      nullptr,                      nullptr, nullptr, reinterpret_cast<void*>(BUTTONFLAG_IS_TOGGLE) }; // 0x6FA63548
static D2WinControlInitStrc gCtrlCreateNameEdit = { D2WIN_EDITBOX,   318, 510, 157, 16,  8,    4,    nullptr,             nullptr,                      reinterpret_cast<void*>(D2LAUNCH_OnCreateOkClick), &gFontStyleFormal12, nullptr }; // 0x6FA63578
static D2WinControlInitStrc gCtrlCreateExpLbl   = { D2WIN_TEXTBOX,   340, 540, 200, 16,  0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyle16, nullptr }; // 0x6FA636C8
static D2WinControlInitStrc gCtrlCreateExpChk   = { D2WIN_BUTTON,    319, 540, 15,  16,  0,    0,    &gpBtnClickBox,      nullptr,                      nullptr, nullptr, reinterpret_cast<void*>(BUTTONFLAG_IS_TOGGLE) }; // 0x6FA636F8
static D2WinControlInitStrc gCtrlCreateExpGrey  = { D2WIN_IMAGE,     319, 540, 15,  16,  0,    0,    &gpImgClickBoxGrey,  nullptr,                      nullptr, nullptr, nullptr }; // 0x6FA63728

// Hardcore Creation Confirmation Modal (0x6FA63758..0x6FA63818)
static D2WinControlInitStrc gCtrlHcWarnBg       = { D2WIN_IMAGE,     268, 350, 264, 176, 0,    0,    &gpPopUpOKCancel,    nullptr,                      nullptr, nullptr, nullptr }; // 0x6FA63758
static D2WinControlInitStrc gCtrlHcWarnCancel   = { D2WIN_BUTTON,    281, 337, 96,  32,  VK_ESCAPE, 5103, &gpBtnOkCancelBlank, D2LAUNCH_OnHardcoreCancel,  nullptr, nullptr, nullptr }; // 0x6FA63788
static D2WinControlInitStrc gCtrlHcWarnOk       = { D2WIN_BUTTON,    421, 337, 96,  32,  VK_RETURN, 5102, &gpBtnOkCancelBlank, D2LAUNCH_OnHardcoreConfirm, nullptr, nullptr, nullptr }; // 0x6FA637B8
static D2WinControlInitStrc gCtrlHcWarnText     = { D2WIN_TEXTBOX,   288, 290, 224, 120, 0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyleFormal12, reinterpret_cast<void*>(6) }; // 0x6FA63818

// TCP/IP Screen & Join Game IP Modal (0x6FA63F98..0x6FA64268)
static D2WinControlInitStrc gCtrlTcpIpBg        = { D2WIN_IMAGE,     0,   599, 800, 600, 0,    0,    &gpBgTcpIpScreen,    nullptr,                      nullptr, nullptr, nullptr }; // 0x6FA63F98
static D2WinControlInitStrc gCtrlTcpIpTitle     = { D2WIN_TEXTBOX,   100, 76,  600, 40,  0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyle30, reinterpret_cast<void*>(2) }; // 0x6FA63FC8
static D2WinControlInitStrc gCtrlTcpIpYourIpLbl = { D2WIN_TEXTBOX,   270, 156, 272, 40,  0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyle16, reinterpret_cast<void*>(2) }; // 0x6FA63FF8
static D2WinControlInitStrc gCtrlTcpIpAddrBox   = { D2WIN_TEXTBOX,   265, 171, 272, 40,  0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyle16, reinterpret_cast<void*>(2) }; // 0x6FA64028
static D2WinControlInitStrc gCtrlTcpIpHostBtn   = { D2WIN_BUTTON,    265, 206, 272, 35,  0,    5118, &gpBtnWideBlank,     D2LAUNCH_OnTcpIpHostClick,    nullptr, reinterpret_cast<void*>(D2LAUNCH_OnTcpIpHostHover), nullptr }; // 0x6FA64058
static D2WinControlInitStrc gCtrlTcpIpJoinBtn   = { D2WIN_BUTTON,    265, 264, 272, 35,  0,    5119, &gpBtnWideBlank,     D2LAUNCH_OnTcpIpJoinPopup,    nullptr, reinterpret_cast<void*>(D2LAUNCH_OnTcpIpJoinHover), nullptr }; // 0x6FA64088
static D2WinControlInitStrc gCtrlTcpIpHostDesc  = { D2WIN_TEXTBOX,   305, 370, 200, 100, 0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyleFormal12, reinterpret_cast<void*>(2) }; // 0x6FA640B8
static D2WinControlInitStrc gCtrlTcpIpJoinDesc  = { D2WIN_TEXTBOX,   305, 370, 200, 100, 0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyleFormal12, reinterpret_cast<void*>(2) }; // 0x6FA640E8
static D2WinControlInitStrc gCtrlTcpIpCancelBtn = { D2WIN_BUTTON,    39,  571, 128, 35,  VK_ESCAPE, 5103, &gpBtnMediumBlank, D2LAUNCH_EnterOtherMultiplayer, nullptr, nullptr, nullptr }; // 0x6FA64118
static D2WinControlInitStrc gCtrlTcpIpTimer     = { D2WIN_TIMER,     5000,0,   0,   0,   0,    0,    nullptr,             D2LAUNCH_OnTcpIpRefreshTimer, nullptr, nullptr, nullptr }; // 0x6FA64148

static D2WinControlInitStrc gCtrlJoinIpPrompt   = { D2WIN_TEXTBOX,   300, 250, 200, 50,  0,    0,    nullptr,             nullptr,                      nullptr, &gFontStyle16, reinterpret_cast<void*>(2) }; // 0x6FA64178
static D2WinControlInitStrc gCtrlJoinIpEdit     = { D2WIN_EDITBOX,   300, 268, 213, 20,  0,    2,    nullptr,             nullptr,                      reinterpret_cast<void*>(D2LAUNCH_OnTcpIpJoinConfirm), &gFontStyleFormal12, nullptr }; // 0x6FA641A8
static D2WinControlInitStrc gCtrlJoinIpFrame    = { D2WIN_IMAGE,     291, 270, 218, 26,  0,    0,    &gpImgIPAddressBox,  nullptr,                      nullptr, nullptr, nullptr }; // 0x6FA641D8
static D2WinControlInitStrc gCtrlJoinIpPopupBg  = { D2WIN_IMAGE,     268, 350, 264, 176, 0,    0,    &gpPopUpOKCancel2,   nullptr,                      nullptr, nullptr, nullptr }; // 0x6FA64208
static D2WinControlInitStrc gCtrlJoinIpCancel   = { D2WIN_BUTTON,    281, 337, 96,  32,  VK_ESCAPE, 5103, &gpBtnOkCancelBlank, D2LAUNCH_OnTcpIpJoinCancel,  nullptr, nullptr, nullptr }; // 0x6FA64238
static D2WinControlInitStrc gCtrlJoinIpOk       = { D2WIN_BUTTON,    421, 337, 96,  32,  VK_RETURN, 5102, &gpBtnOkCancelBlank, D2LAUNCH_OnTcpIpJoinConfirm, nullptr, nullptr, nullptr }; // 0x6FA64268

// ============================================================================
// Screen-Specific State Pointers
// ============================================================================
// Credits State (0x6FA65688..0x6FA656A8)
static D2WinTextBoxStrc* gpCreditsLeftBox  = nullptr; // 0x6FA65688
static D2WinTextBoxStrc* gpCreditsRightBox = nullptr; // 0x6FA65694
static D2WinTextBoxStrc* gpCreditsHdrBox   = nullptr; // 0x6FA65690
static int32_t gnCreditsEndLine            = 0;       // 0x6FA656A8

// TCP/IP State (0x6FA65660..0x6FA656C0)
static D2WinTextBoxStrc* gpTcpIpAddrBox    = nullptr; // 0x6FA65660
static D2WinButtonStrc* gpTcpIpHostBtn     = nullptr; // 0x6FA656A0
static D2WinButtonStrc* gpTcpIpJoinBtn     = nullptr; // 0x6FA656A4
static D2WinTextBoxStrc* gpTcpIpHostDesc   = nullptr; // 0x6FA656B4
static D2WinTextBoxStrc* gpTcpIpJoinDesc   = nullptr; // 0x6FA656B0
static D2WinTimerStrc* gpTcpIpTimer        = nullptr; // 0x6FA656C0
static D2WinEditBoxStrc* gpJoinIpEditBox   = nullptr; // 0x6FA656C4
static int32_t gnTcpIpConnectResult        = 0;       // 0x6FA6569C

// Campfire Character Create State (0x6FA64DA8..0x6FA65AD4)
static int32_t gnHoveredCreateClass        = -1;      // 0x6FA64DA8
static int32_t gnSelectedCreateClass       = -1;      // 0x6FA64DAC
static D2WinAnimImageStrc* gpCampfireOrder[7] = {};   // 0x6FA6566C..0x6FA65684 (0=Barb, 1=Necro, 2=Ama, 3=Sorc, 4=Pal, 5=Druid, 6=Assa)
static D2WinTextBoxStrc* gpCreateClassNameBox = nullptr; // 0x6FA65AD0
static D2WinTextBoxStrc* gpCreateClassDescBox = nullptr; // 0x6FA65AD4
static D2WinImageStrc* gpCreateNameFrameImg   = nullptr; // 0x6FA65698
static D2WinTextBoxStrc* gpCreateNameLabelBox = nullptr; // 0x6FA656CC
static D2WinEditBoxStrc* gpCreateNameEditBox  = nullptr; // 0x6FA65548
static D2WinTextBoxStrc* gpCreateExpLabelBox  = nullptr; // 0x6FA65668
static D2WinButtonStrc* gpCreateExpCheckBox   = nullptr; // 0x6FA656C8
static D2WinImageStrc* gpCreateExpGreyImg     = nullptr; // 0x6FA656B4
static D2WinTextBoxStrc* gpCreateHcLabelBox   = nullptr; // 0x6FA656B0
static D2WinButtonStrc* gpCreateHcCheckBox    = nullptr; // 0x6FA656C4
static D2WinButtonStrc* gpCreateOkBtn         = nullptr; // 0x6FA65428

// Opt-in Dev Client Mode Toggle (D2MOO_DEV_CLIENT_TOGGLE=1)
static D2WinButtonStrc* gpBtnClientModeToggle = nullptr;
static int32_t gnClientMode = 2;
static bool gbClientModeInitialized = false;

void D2LAUNCH_SyncClientModeEnv()
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
	D2LAUNCH_SyncClientModeEnv();
	RefreshClientModeButtonText();
	return 1;
}

void D2LAUNCH_MaybeCreateDevClientToggle(int32_t nX, int32_t nY)
{
	D2LAUNCH_SyncClientModeEnv();
	char szDevToggle[16] = {};
	if (GetEnvironmentVariableA("D2MOO_DEV_CLIENT_TOGGLE", szDevToggle, sizeof(szDevToggle)) > 0 && szDevToggle[0] == '1')
	{
		D2WinControlInitStrc tToggle = {};
		tToggle.nType = D2WIN_BUTTON;
		tToggle.nX = nX;
		tToggle.nY = nY;
		tToggle.nWidth = 272;
		tToggle.nHeight = 35;
		tToggle.ppCellFile = &gpBtnWideBlank;
		tToggle.field_20 = D2LAUNCH_OnToggleClientMode;
		gpBtnClientModeToggle = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&tToggle));
		RefreshClientModeButtonText();
	}
}

// ============================================================================
// Control & Palette Management (0x6FA51590..0x6FA54830)
// ============================================================================
// 1.12a: 0x6FA51670
void __fastcall D2LAUNCH_SetPalette(const char* szDatRelPath, const char* szPl2RelPath)
{
	char szDatFull[MAX_PATH] = {};
	char szPl2Full[MAX_PATH] = {};
	std::snprintf(szDatFull, sizeof(szDatFull), "DATA\\GLOBAL\\%s", szDatRelPath);
	std::snprintf(szPl2Full, sizeof(szPl2Full), "DATA\\GLOBAL\\%s", szPl2RelPath);
	D2Win_10028(szDatFull, szPl2Full);
}

D2WinControlStrc* __fastcall D2LAUNCH_RegisterScreenControl(D2WinControlStrc* pCtrl)
{
	if (!pCtrl)
	{
		return nullptr;
	}
	if (gnScreenControlCount < kMaxScreenControls)
	{
		gapScreenControls[gnScreenControlCount++] = pCtrl;
	}
	return pCtrl;
}

// 1.12a: 0x6FA51590
D2WinControlStrc* __fastcall D2LAUNCH_CreateScreenControl(D2WinControlInitStrc* pInit)
{
	if (!pInit || gnScreenControlCount >= kMaxScreenControls)
	{
		return nullptr;
	}
	return D2LAUNCH_RegisterScreenControl(CONTROL_Create(pInit));
}

D2WinControlStrc* __fastcall D2LAUNCH_CreateModalControl(D2WinControlInitStrc* pInit)
{
	if (!pInit || gnModalControlCount >= kMaxModalControls)
	{
		return nullptr;
	}
	D2WinControlStrc* pCtrl = CONTROL_Create(pInit);
	if (pCtrl)
	{
		gapModalControls[gnModalControlCount++] = pCtrl;
		D2Win_10030_CONTROL_ToggleFourthFlag(pCtrl, 1);
	}
	return pCtrl;
}

// 1.12a: 0x6FA515E0
int __stdcall D2LAUNCH_DestroyModalControls()
{
	for (int32_t i = 0; i < gnModalControlCount; ++i)
	{
		if (gapModalControls[i] != nullptr)
		{
			CONTROL_Destroy(&gapModalControls[i]);
			gapModalControls[i] = nullptr;
		}
	}
	gnModalControlCount = 0;
	gbModalOpen = 0;
	gpJoinIpEditBox = nullptr;
	return 1;
}

// 1.12a: 0x6FA51620
static int __stdcall D2LAUNCH_OnModalAlertOk(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	void(__stdcall* pfCallback)(int32_t) = gpfModalAlertCallback;
	gpfModalAlertCallback = nullptr;
	D2LAUNCH_DestroyModalControls();
	if (pfCallback)
	{
		pfCallback(0);
	}
	return 1;
}

// 1.12a: 0x6FA540B0 / 0x6FA541B0
void __stdcall D2LAUNCH_ShowModalAlert(const Unicode* wszMessage, void(__stdcall* pfOnOk)(int32_t), int32_t nColor)
{
	D2LAUNCH_DestroyModalControls();
	gbModalOpen = 1;
	gpfModalAlertCallback = pfOnOk;

	D2WinControlStrc* pBg = D2LAUNCH_CreateModalControl(&gCtrlAlertPopupBg);
	if (pBg)
	{
		D2Win_10033(pBg, 1);
	}
	D2WinControlStrc* pOkBtn = D2LAUNCH_CreateModalControl(&gCtrlAlertPopupOk);
	if (pOkBtn)
	{
		D2Win_10031(pOkBtn);
	}
	D2WinTextBoxStrc* pTextBox = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateModalControl(&gCtrlAlertPopupText));
	if (pTextBox && wszMessage)
	{
		D2Win_10051(pTextBox, wszMessage, nColor, 0, 0);
	}
}

// 1.12a: 0x6FA54830
void __fastcall D2LAUNCH_DestroyAllControls()
{
	D2LAUNCH_DestroyModalControls();
	D2Win_10146();
	for (int32_t i = 0; i < gnScreenControlCount; ++i)
	{
		if (gapScreenControls[i] != nullptr)
		{
			CONTROL_Destroy(&gapScreenControls[i]);
			gapScreenControls[i] = nullptr;
		}
	}
	gnScreenControlCount = 0;

	gpCreditsLeftBox = nullptr;
	gpCreditsRightBox = nullptr;
	gpCreditsHdrBox = nullptr;
	gpTcpIpAddrBox = nullptr;
	gpTcpIpHostBtn = nullptr;
	gpTcpIpJoinBtn = nullptr;
	gpTcpIpHostDesc = nullptr;
	gpTcpIpJoinDesc = nullptr;
	gpTcpIpTimer = nullptr;

	std::memset(gpCampfireOrder, 0, sizeof(gpCampfireOrder));
	gpCreateClassNameBox = nullptr;
	gpCreateClassDescBox = nullptr;
	gpCreateNameFrameImg = nullptr;
	gpCreateNameLabelBox = nullptr;
	gpCreateNameEditBox = nullptr;
	gpCreateExpLabelBox = nullptr;
	gpCreateExpCheckBox = nullptr;
	gpCreateExpGreyImg = nullptr;
	gpCreateHcLabelBox = nullptr;
	gpCreateHcCheckBox = nullptr;
	gpCreateOkBtn = nullptr;
	gpBtnClientModeToggle = nullptr;

	BACKGROUND_SetCellFile(nullptr);
	D2LAUNCH_SetPalette("palette\\fechar\\pal.dat", "palette\\fechar\\pal.pl2");
}

// 1.12a: 0x6FA4FBE0
static void D2LAUNCH_ApplyLocaleLayoutAdjustments()
{
	if (STRTABLE_GetLanguage() == LANGUAGE_JAPANESE)
	{
		gCtrlTcpIpTitle.nY = 70;
		gCtrlJoinIpPrompt.nY = 238;
		gCtrlCreateNameLbl.nY = 506;
		gCtrlCreateExpLbl.nY = 555;
		gCtrlCreateHcLblCl.nY = 555;
		gCtrlCreateHcLblExp.nY = 575;
	}
}

// ============================================================================
// Sprite Loading & Freeing (0x6FA4FC90, 0x6FA51700, 0x6FA51A90)
// ============================================================================
static void FreeCellPtr(D2CellFileStrc** ppCellFile)
{
	if (ppCellFile && *ppCellFile)
	{
		ARCHIVE_FreeCellFile(*ppCellFile);
		*ppCellFile = nullptr;
	}
}

// 1.12a: 0x6FA51A90
void __fastcall D2LAUNCH_LoadSprites(BOOL bReload)
{
	const BOOL bExpansion = FOG_IsExpansion();
	if (!bReload)
	{
		gpCursorCellFile = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CURSOR\\ohand", 0);
	}

	gpImgBlizNo         = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\blizno", 0);
	gpBgTitleScreen     = ARCHIVE_LoadCellFile(bExpansion ? "DATA\\GLOBAL\\UI\\FrontEnd\\trademarkscreenEXP" : "DATA\\GLOBAL\\UI\\FrontEnd\\trademark", 0);
	gpImgDiablo2Logo    = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\Diablo2", 0);
	gpLogoBlackLeft     = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2logoBlackLeft", 0);
	gpLogoFireLeft      = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2logoFireLeft", 0);
	gpLogoBlackRight    = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2logoBlackRight", 0);
	gpLogoFireRight     = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\D2logoFireRight", 0);
	gpBgGameSelect      = ARCHIVE_LoadCellFile(bExpansion ? "DATA\\GLOBAL\\UI\\FrontEnd\\gameselectscreenEXP" : "DATA\\GLOBAL\\UI\\FrontEnd\\TitleScreen", 0);
	gpBgCredits         = ARCHIVE_LoadCellFile(bExpansion ? "DATA\\GLOBAL\\UI\\CharSelect\\creditsbckgexpand" : "DATA\\GLOBAL\\UI\\CharSelect\\creditsbckg", 0);

	gpBtnWideBlank      = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\WideButtonBlank", 0);
	gpBtnWideBlank02    = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\WideButtonBlank02", 0);
	gpBtnNarrowBlank    = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\NarrowButtonBlank", 0);
	gpImgGWListBox      = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\GWListBox", 0);
	gpBtnRealmSelect    = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\Realmselectbutton", 0);
	gpBtnTallBlank      = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\TallButtonBlank", 0);

	if (bExpansion)
	{
		gpBgCharSelectExp  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\characterselectscreenEXP", 0);
		gpImgCinematicsExp = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\CinematicsSelectionEXP", 0);
	}

	gpBtnMediumBlank     = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\MediumButtonBlank", 0);
	gpBtnShortBlank      = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\ShortButtonBlank", 0);
	gpImgTextBox2        = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\textbox2", 0);
	gpBtn3WideBlank      = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\3WideButtonBlank", 0);
	gpBgTermsOfUse       = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\TermsofUseBckg", 0);
	gpBgCharSelectClassic= ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\charselectbckg", 0);
	gpDifficultyLevels   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\DifficultyLevels", 0);
	gpImgCinematics      = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\CinematicsSelection", 0);
	gpCharSelectBox      = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\charselectbox", 0);
	gpCharSelectBoxGrey  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\charselectboxgrey", 0);
	gpCreateNewCharImage = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\CreateNewCharImage", 0);
	gpPopUpOK            = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\PopUpOK", 0);
	gpPopUp340x224       = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\PopUp_340x224", 0);
	gpPopUpWide          = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\PopUpWide", 0);
	gpPopUpLargest       = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\PopUpLargest", 0);
	gpTileableDialog     = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\TileableDialog", 0);
	gpPopUpOK2           = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\PopUpOK2", 0);
	gpPopUpOKCancel      = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\PopUpOKCancel", 0);
	gpPopUpOKCancel2     = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\PopUpOKCancel2", 0);
	gpBtnOkCancelBlank   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\OkCancelButtonBlank", 0);
	gpBtnCancelBlank     = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\CancelButtonBlank", 0);
	gpSelectCharArrows   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\CharSelect\\selectchararrows", 0);
	gpJoinGameScrollBars = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\BattleNet\\joingamescrollbars", 0);
	gpBgTcpIpScreen      = ARCHIVE_LoadCellFile(bExpansion ? "DATA\\GLOBAL\\UI\\FrontEnd\\tcpipscreen" : "DATA\\GLOBAL\\UI\\CharSelect\\tcpipbckg", 0);
	gpImgIPAddressBox    = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\IPAddressBox", 0);
	gpImgEditTextBoxWide = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\EditTextBoxWide", 0);
}

// 1.12a: 0x6FA4FC90
void __stdcall D2LAUNCH_LoadCampfireSprites()
{
	if (gpBgCharCreate != nullptr)
	{
		return;
	}

	const BOOL bExpansion = FOG_IsExpansion();
	gpBtnMediumSelBlank = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\MediumSelButtonBlank", 0);
	gpBgCharCreate      = ARCHIVE_LoadCellFile(bExpansion ? "DATA\\GLOBAL\\UI\\FrontEnd\\charactercreationscreenEXP" : "DATA\\GLOBAL\\UI\\CharSelect\\CharacterCreate", 0);
	gpCampfire          = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\fire", 0);
	gpImgTextBox        = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\textbox", 0);
	gpBtnClickBox       = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\clickbox", 0);
	gpImgClickBoxGrey   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\joingameclickboxgrey", 0);

	// 0: Amazon
	gpClassNu1[0]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amnu1", 0);
	gpClassNu2[0]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amnu2", 0);
	gpClassNu3[0]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amnu3", 0);
	gpClassFw[0]   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\amfw", 0);
	gpClassBw[0]   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\amazon\\ambw", 0);

	// 1: Sorceress
	gpClassNu1[1]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sonu1", 0);
	gpClassNu2[1]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sonu2", 0);
	gpClassNu3[1]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sonu3", 0);
	gpClassNu3s[1] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sonu3s", 0);
	gpClassFw[1]   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sofw", 0);
	gpClassFws[1]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sofws", 0);
	gpClassBw[1]   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sobw", 0);
	gpClassBws[1]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\sorceress\\sobws", 0);

	// 2: Necromancer
	gpClassNu1[2]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nenu1", 0);
	gpClassNu2[2]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nenu2", 0);
	gpClassNu3[2]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nenu3", 0);
	gpClassNu3s[2] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nenu3s", 0);
	gpClassFw[2]   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nefw", 0);
	gpClassFws[2]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nefws", 0);
	gpClassBw[2]   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nebw", 0);
	gpClassBws[2]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\necromancer\\nebws", 0);

	// 3: Paladin
	gpClassNu1[3]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\panu1", 0);
	gpClassNu2[3]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\panu2", 0);
	gpClassNu3[3]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\panu3", 0);
	gpClassFw[3]   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\pafw", 0);
	gpClassFws[3]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\pafws", 0);
	gpClassBw[3]   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\paladin\\pabw", 0);

	// 4: Barbarian
	gpClassNu1[4]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\banu1", 0);
	gpClassNu2[4]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\banu2", 0);
	gpClassNu3[4]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\banu3", 0);
	gpClassFw[4]   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\bafw", 0);
	gpClassFws[4]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\bafws", 0);
	gpClassBw[4]   = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\barbarian\\babw", 0);

	if (bExpansion)
	{
		// 5: Druid
		gpClassNu1[5] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dznu1", 0);
		gpClassNu2[5] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dznu2", 0);
		gpClassNu3[5] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dznu3", 0);
		gpClassFw[5]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dzfw", 0);
		gpClassBw[5]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\druid\\dzbw", 0);

		// 6: Assassin
		gpClassNu1[6] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asnu1", 0);
		gpClassNu2[6] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asnu2", 0);
		gpClassNu3[6] = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asnu3", 0);
		gpClassFw[6]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asfw", 0);
		gpClassBw[6]  = ARCHIVE_LoadCellFile("DATA\\GLOBAL\\UI\\FrontEnd\\assassin\\asbw", 0);
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

	FreeCellPtr(&gpImgBlizNo);
	FreeCellPtr(&gpBgTitleScreen);
	FreeCellPtr(&gpImgDiablo2Logo);
	FreeCellPtr(&gpLogoBlackLeft);
	FreeCellPtr(&gpLogoFireLeft);
	FreeCellPtr(&gpLogoBlackRight);
	FreeCellPtr(&gpLogoFireRight);
	FreeCellPtr(&gpBgGameSelect);
	FreeCellPtr(&gpBgCredits);
	FreeCellPtr(&gpBgCharCreate);
	FreeCellPtr(&gpCampfire);
	FreeCellPtr(&gpBtnRealmSelect);
	FreeCellPtr(&gpBtnTallBlank);
	FreeCellPtr(&gpBgCharSelectExp);
	FreeCellPtr(&gpBtnWideBlank);
	FreeCellPtr(&gpBtnMediumBlank);
	FreeCellPtr(&gpBtnShortBlank);
	FreeCellPtr(&gpBtnMediumSelBlank);
	FreeCellPtr(&gpImgTextBox2);
	FreeCellPtr(&gpBtn3WideBlank);
	FreeCellPtr(&gpImgEditTextBoxWide);
	FreeCellPtr(&gpBtnWideBlank02);
	FreeCellPtr(&gpBtnNarrowBlank);
	FreeCellPtr(&gpImgGWListBox);
	FreeCellPtr(&gpBgTermsOfUse);
	FreeCellPtr(&gpBgCharSelectClassic);
	FreeCellPtr(&gpDifficultyLevels);
	FreeCellPtr(&gpImgCinematics);
	FreeCellPtr(&gpImgCinematicsExp);
	FreeCellPtr(&gpCharSelectBox);
	FreeCellPtr(&gpCharSelectBoxGrey);
	FreeCellPtr(&gpPopUpOKCancel);
	FreeCellPtr(&gpPopUpOKCancel2);
	FreeCellPtr(&gpSelectCharArrows);
	FreeCellPtr(&gpCreateNewCharImage);
	FreeCellPtr(&gpPopUpOK);
	FreeCellPtr(&gpPopUp340x224);
	FreeCellPtr(&gpPopUpWide);
	FreeCellPtr(&gpPopUpLargest);
	FreeCellPtr(&gpTileableDialog);
	FreeCellPtr(&gpPopUpOK2);
	FreeCellPtr(&gpBtnOkCancelBlank);
	FreeCellPtr(&gpBtnCancelBlank);
	FreeCellPtr(&gpImgTextBox);
	FreeCellPtr(&gpBtnClickBox);
	FreeCellPtr(&gpImgClickBoxGrey);
	FreeCellPtr(&gpJoinGameScrollBars);
	FreeCellPtr(&gpBgTcpIpScreen);
	FreeCellPtr(&gpImgIPAddressBox);

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

// ============================================================================
// Save File & Character Config Procedures (0x6FA505C0..0x6FA5A2D0)
// ============================================================================
// 1.12a: 0x6FA505C0
void __fastcall D2LAUNCH_SetConfigCharacterClass(uint8_t nCharClass)
{
	if (!gpD2Config)
	{
		return;
	}
	gpD2Config->unpackedCTemp.nCharacterClassId = nCharClass;
	gpD2Config->bAmazon      = (nCharClass == 0) ? 1 : 0;
	gpD2Config->bSorceress   = (nCharClass == 1) ? 1 : 0;
	gpD2Config->bNecromancer = (nCharClass == 2) ? 1 : 0;
	gpD2Config->bPaladin     = (nCharClass == 3) ? 1 : 0;
	gpD2Config->bBarbarian   = (nCharClass == 4) ? 1 : 0;
#if D2_VERSION_EXPANSION
	gpD2Config->bDruid       = (nCharClass == 5) ? 1 : 0;
	gpD2Config->bAssassin    = (nCharClass == 6) ? 1 : 0;
#endif
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
	std::memset(pPreviewInfo, 0, sizeof(*pPreviewInfo));
	uint8_t* pOut = reinterpret_cast<uint8_t*>(pPreviewInfo);
	FOG_Encode14BitsToString(reinterpret_cast<uint16_t*>(pOut), 0x0C);
	pOut += 2;
	const bool bHasComp = pComponents && pComponents[0] != 0;
	for (int i = 0; i < 11; ++i)
	{
		*pOut++ = bHasComp ? pComponents[i] : 0xFF;
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

// 1.12a: 0x6FA59EB0
BOOL __fastcall D2LAUNCH_ReadSavePreview(D2CharacterPreviewInfoStrc* pPreviewOut, const char* szCharName)
{
	if (!pPreviewOut || !szCharName || !szCharName[0])
	{
		return FALSE;
	}

	char szSaveDir[0x400] = {};
	char szFilePath[0x600] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
	std::snprintf(szFilePath, sizeof(szFilePath), "%s%s.d2s", szSaveDir, szCharName);

	HANDLE hFile = FOG_CreateFileA(
		szFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		nullptr);
	if (!hFile || hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	uint8_t aBuf[0x200] = {};
	DWORD dwRead = 0;
	const BOOL bOk = FOG_ReadFile(hFile, aBuf, sizeof(aBuf), &dwRead, nullptr);
	FOG_CloseFile(hFile);
	if (!bOk || dwRead < 8)
	{
		return FALSE;
	}

	const uint32_t dwMagic = *reinterpret_cast<const uint32_t*>(aBuf + 0x00);
	const uint32_t dwVer   = *reinterpret_cast<const uint32_t*>(aBuf + 0x04);
	if (dwMagic != 0xAA55AA55u)
	{
		return FALSE;
	}

	uint8_t nClass = 0;
	uint8_t nLevel = 1;
	uint16_t wFlags = 0;
	const uint8_t* pComp = nullptr;
	const uint8_t* pColors = nullptr;

	if (dwVer >= 0x47u && dwVer <= 0x5Bu)
	{
		if (dwRead < 0x82u)
		{
			return FALSE;
		}
		wFlags  = static_cast<uint16_t>(*reinterpret_cast<const uint32_t*>(aBuf + 0x1Cu));
		nClass  = aBuf[0x26];
		nLevel  = aBuf[0x28];
		pComp   = aBuf + 0x2A;
		pColors = aBuf + 0x3A;
	}
	else if (dwVer > 0x5Bu && dwVer <= 0x60u)
	{
		if (dwRead < sizeof(D2SaveHeaderStrc))
		{
			return FALSE;
		}
		const D2SaveHeaderStrc* pHdr = reinterpret_cast<const D2SaveHeaderStrc*>(aBuf);
		wFlags  = static_cast<uint16_t>(pHdr->dwSaveFlags);
		nClass  = (pHdr->nCharClass < 7) ? pHdr->nCharClass : 0;
		nLevel  = pHdr->nCharLevel ? pHdr->nCharLevel : 1;
		pComp   = pHdr->nAppearanceComponents;
		pColors = pHdr->nAppearanceColors;
	}
	else
	{
		return FALSE;
	}

	if (!FOG_IsExpansion() && ((wFlags & 0x20u) != 0 || nClass >= 5))
	{
		return FALSE;
	}

	EncodePreviewInfo(pPreviewOut, nClass, nLevel, wFlags, pComp, pColors);
	return TRUE;
}

// 1.12a: 0x6FA59D50
BOOL __fastcall D2LAUNCH_ConvertSaveToExpansion(const char* szCharName)
{
	if (!szCharName || !szCharName[0])
	{
		return FALSE;
	}

	char szSaveDir[0x400] = {};
	char szFullPath[0x600] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
	std::snprintf(szFullPath, sizeof(szFullPath), "%s%s.d2s", szSaveDir, szCharName);

	FILE* pFile = std::fopen(szFullPath, "r+b");
	if (!pFile)
	{
		return FALSE;
	}

	BOOL bConverted = FALSE;
	std::fseek(pFile, 0, SEEK_END);
	const long nSize = std::ftell(pFile);
	std::fseek(pFile, 0, SEEK_SET);
	if (nSize >= 0x82)
	{
		uint8_t* pBuf = static_cast<uint8_t*>(D2_CALLOC(nSize));
		if (pBuf)
		{
			if (std::fread(pBuf, 1, nSize, pFile) == static_cast<size_t>(nSize))
			{
				const uint32_t dwVer = *reinterpret_cast<const uint32_t*>(pBuf + 0x04);
				if (dwVer <= 0x5Bu)
				{
					*reinterpret_cast<uint32_t*>(pBuf + 0x1Cu) |= 0x20u;
				}
				else
				{
					D2SaveHeaderStrc* pHdr = reinterpret_cast<D2SaveHeaderStrc*>(pBuf);
					pHdr->dwSaveFlags |= 0x20u;
					pHdr->dwChecksum = 0;
					pHdr->dwChecksum = FOG_ComputeChecksum(pBuf, nSize);
				}
				std::fseek(pFile, 0, SEEK_SET);
				std::fwrite(pBuf, 1, nSize, pFile);
				bConverted = TRUE;
			}
			D2_FREE(pBuf);
		}
	}
	std::fclose(pFile);
	return bConverted;
}

// 1.12a: 0x6FA5A0C0
BOOL __fastcall D2LAUNCH_DeleteSaveFiles(const char* szCharName)
{
	if (!szCharName || !szCharName[0])
	{
		return FALSE;
	}

	char szSaveDir[0x400] = {};
	char szPattern[0x600] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
	std::snprintf(szPattern, sizeof(szPattern), "%s%s.*", szSaveDir, szCharName);

	WIN32_FIND_DATAA tFind = {};
	HANDLE hFind = FindFirstFileA(szPattern, &tFind);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	do
	{
		char szTarget[0x600] = {};
		std::snprintf(szTarget, sizeof(szTarget), "%s%s", szSaveDir, tFind.cFileName);
		FOG_DeleteFileA(szTarget);
	} while (FindNextFileA(hFind, &tFind));

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
	gD2SaveHeader.dwHeaderMagic = 0xAA55AA55u;
	gD2SaveHeader.dwVersion = 0x60u;
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
	gD2SaveHeader.dwLeftSkill1  = 0xFFFFFFFFu;
	gD2SaveHeader.dwRightSkill1 = 0xFFFFFFFFu;
	gD2SaveHeader.dwLeftSkill2  = 0xFFFFFFFFu;
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
	char szFilePath[0x600] = {};
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

	gD2SaveHeader.dwChecksum = 0;
	gD2SaveHeader.dwChecksum = FOG_ComputeChecksum(&gD2SaveHeader, sizeof(gD2SaveHeader));
	DWORD dwBytesWritten = 0;
	FOG_WriteFile(hFile, &gD2SaveHeader, sizeof(gD2SaveHeader), &dwBytesWritten, nullptr);
	FOG_CloseFile(hFile);
	return TRUE;
}

// 1.12a: 0x6FA594F0
BOOL __stdcall D2LAUNCH_EnterSelectedCharacter(
	const char* szCharName,
	const char* szRealmName,
	uint8_t nCharClass,
	uint16_t wSaveFlags)
{
	if (!gpD2Config || !szCharName)
	{
		return FALSE;
	}

	SStrCopy(gpD2Config->szName, szCharName, sizeof(gpD2Config->szName));
	SStrCopy(gpD2Config->szRealm, szRealmName ? szRealmName : "", sizeof(gpD2Config->szRealm));
	gpD2Config->unpackedCTemp.nCharacterSaveFlags = wSaveFlags;
	D2LAUNCH_SetConfigCharacterClass(nCharClass);

	uint32_t dwArenaFlags = (wSaveFlags & 0x04u) ? 0x804u : 0x004u;
	if (wSaveFlags & 0x20u)
	{
		dwArenaFlags |= 0x100000u;
	}
	gpD2Config->nArenaFlags = dwArenaFlags;

	if (gnLaunchMode == 0)
	{
		gpD2Config->nLaunchType = LAUNCHTYPE_LOCAL;
		gpD2Config->dwGameType = 0;
		gdwNextModule = D2LAUNCH_MODULE_CLIENT;
	}
	else if (gnLaunchMode == 2)
	{
		gpD2Config->nLaunchType = LAUNCHTYPE_TCPIP;
		gdwNextModule = D2LAUNCH_MODULE_CLIENT;
	}
	else if (gnLaunchMode == 3)
	{
		gpD2Config->nLaunchType = LAUNCHTYPE_OPENBNET;
		gdwNextModule = D2LAUNCH_MODULE_MULTIPLAYER;
	}
	else
	{
		gpD2Config->nLaunchType = LAUNCHTYPE_REALM;
		gdwNextModule = D2LAUNCH_MODULE_MULTIPLAYER;
	}

	D2Win_10009();
	return TRUE;
}

// ============================================================================
// Logo / Trademark Screen & Main Menu (0x6FA56BF0, 0x6FA58150, 0x6FA5B260, 0x6FA568C0)
// ============================================================================
// 1.12a: 0x6FA57240
static int __stdcall D2LAUNCH_OnTrademarkDismiss(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (gpD2Config)
	{
		gpD2Config->bShownLogo = 1;
	}
	D2LAUNCH_EnterMainMenu(0);
	return 1;
}

// 1.12a: 0x6FA56BF0
void __stdcall D2LAUNCH_ShowTrademarkScreen()
{
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_CreateScreenControl(&gCtrlTrademarkBg);
	D2LAUNCH_CreateScreenControl(&gCtrlTrademarkTimer);
	D2LAUNCH_CreateScreenControl(&gCtrlBurningLogoL);
	D2LAUNCH_CreateScreenControl(&gCtrlBurningLogoR);
	D2LAUNCH_SetPalette("palette\\sky\\pal.dat", "palette\\sky\\pal.pl2");

	D2WinTextBoxStrc* pCopyright = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCopyrightBox));
	if (pCopyright)
	{
		const int16_t nLine1Id = FOG_IsExpansion() ? 0x557A : 0x13F0;
		if (const Unicode* wszLine1 = D2LANG_GetStringFromTblIndex(nLine1Id))
		{
			D2Win_10051(pCopyright, wszLine1, 4, 0, 0);
		}
		if (const Unicode* wszLine2 = D2LANG_GetStringFromTblIndex(0x13F1))
		{
			D2Win_10051(pCopyright, wszLine2, 4, 0, 0);
		}
	}

	D2Win_10029(0);
	if (D2SOUND_GetOptionsMusic())
	{
		D2SOUND_EnableMusic(1);
	}
}

// 1.12a: 0x6FA58150
void __stdcall D2LAUNCH_EnterLogoScreen()
{
	D2Win_10029(0);
	D2LAUNCH_DestroyAllControls();
	if (WINDOW_CheckCutScene())
	{
		D2Win_10003();
		WINDOW_OpenSmackCutscene("Data\\Local\\Video\\New_BLIZ640x480.bik", D2GAMERES_640x480, nullptr);
		WINDOW_OpenSmackCutscene("Data\\Local\\Video\\BlizNorth640x480.bik", D2GAMERES_640x480, nullptr);
		D2Win_10004(D2GAMERES_800x600);
	}
	D2LAUNCH_ShowTrademarkScreen();
}

// 1.12a: 0x6FA5B260
static void __stdcall D2LAUNCH_SetupMainMenuBackdrop()
{
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_CreateScreenControl(&gCtrlMainMenuBg);
	D2LAUNCH_CreateScreenControl(&gCtrlBurningLogoL);
	D2LAUNCH_CreateScreenControl(&gCtrlBurningLogoR);
	D2LAUNCH_SetPalette("palette\\sky\\pal.dat", "palette\\sky\\pal.pl2");
	D2LAUNCH_ApplyLocaleLayoutAdjustments();
}

// 1.12a: 0x6FA57210
static int __stdcall D2LAUNCH_OnMainMenuCancel(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_EnterMainMenu(0);
	return 1;
}

// 1.12a: 0x6FA59A70
static int __stdcall D2LAUNCH_OnSinglePlayerClick(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	gnLaunchMode = 0;
	if (D2LAUNCH_HasSavedCharacters())
	{
		D2SOUND_FastFadeAll();
		D2LAUNCH_DestroyAllControls();
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
	D2SOUND_SmpFadeMasterVolume(180);
	D2Win_10009();
	return 1;
}

// 1.12a: 0x6FA568C0
void __stdcall D2LAUNCH_EnterMainMenu(int32_t nUnused)
{
	(void)nUnused;
	D2LAUNCH_SetupMainMenuBackdrop();

	D2LAUNCH_CreateScreenControl(&gCtrlBtnSinglePlr);

	// Battle.net (5107) — closed realm service out-of-scope; keep authentic button disabled
	if (D2WinControlStrc* pBNet = D2LAUNCH_CreateScreenControl(&gCtrlBtnBattleNet))
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(pBNet, 0);
	}

	D2LAUNCH_CreateScreenControl(&gCtrlBtnOtherMulti);
	D2LAUNCH_CreateScreenControl(&gCtrlBtnCredits);
	D2LAUNCH_CreateScreenControl(&gCtrlBtnCinematics);
	D2LAUNCH_CreateScreenControl(&gCtrlBtnExitGame);

	if (D2WinTextBoxStrc* pVerBox = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlVersionBox)))
	{
#ifdef D2_VERSION_110F
		D2Win_10046(pVerBox, "v 1.10", 0);
#elif defined(D2_VERSION_113C)
		D2Win_10046(pVerBox, "v 1.13", 0);
#elif defined(D2_VERSION_113D)
		D2Win_10046(pVerBox, "v 1.13", 0);
#elif defined(D2_VERSION_114D)
		D2Win_10046(pVerBox, "v 1.14d", 0);
#else
		D2Win_10046(pVerBox, "v 1.12", 0);
#endif
	}

	D2LAUNCH_MaybeCreateDevClientToggle(264, 480);

	if (D2SOUND_GetOptionsMusic())
	{
		D2SOUND_EnableMusic(1);
	}
	else
	{
		D2SOUND_EnableMusic(0);
	}
}

// ============================================================================
// Other Multiplayer & TCP/IP Host / Join (0x6FA558E0, 0x6FA56630, 0x6FA524A0, 0x6FA59830)
// ============================================================================
// 1.12a: 0x6FA57320
static int __stdcall D2LAUNCH_OnTcpIpMenuClick(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	gnLaunchMode = 2;
	D2LAUNCH_EnterTcpIpMenu();
	return 1;
}

// 1.12a: 0x6FA558E0
int __stdcall D2LAUNCH_EnterOtherMultiplayer(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_SetupMainMenuBackdrop();

	// Open Battle.net (5115) — Battle.net gateway out-of-scope; keep authentic button disabled
	if (D2WinControlStrc* pOpenBNet = D2LAUNCH_CreateScreenControl(&gCtrlBtnOpenBNet))
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(pOpenBNet, 0);
	}

	D2LAUNCH_CreateScreenControl(&gCtrlBtnTcpIpGame);
	if (D2WinControlStrc* pCancel = D2LAUNCH_CreateScreenControl(&gCtrlBtnMultiCancel))
	{
		D2Win_10031(pCancel);
	}
	return 1;
}

// 1.12a: 0x6FA51040
static int __stdcall D2LAUNCH_OnTcpIpHostHover(SMSGHANDLER_PARAMS* pMsg)
{
	if (gpTcpIpHostDesc)
	{
		D2Win_10027_CONTROL_ToggleThirdFlag(&gpTcpIpHostDesc->controlHeader, (pMsg && pMsg->wParam) ? 1 : 0);
	}
	return 1;
}

// 1.12a: 0x6FA51000
static int __stdcall D2LAUNCH_OnTcpIpJoinHover(SMSGHANDLER_PARAMS* pMsg)
{
	if (gpTcpIpJoinDesc)
	{
		D2Win_10027_CONTROL_ToggleThirdFlag(&gpTcpIpJoinDesc->controlHeader, (pMsg && pMsg->wParam) ? 1 : 0);
	}
	return 1;
}

// 1.12a: 0x6FA51080
static void D2LAUNCH_UpdateTcpIpAddressDisplay()
{
	if (!gpTcpIpAddrBox)
	{
		return;
	}
	D2Win_10047(gpTcpIpAddrBox);
	const char* szHostIp = FOG_GetHostIPAddress();
	if (szHostIp && szHostIp[0])
	{
		D2Win_10046(gpTcpIpAddrBox, szHostIp, 0);
		if (gpTcpIpHostBtn)
		{
			D2Win_10025_CONTROL_ToggleFirstFlag(&gpTcpIpHostBtn->controlHeader, 1);
		}
		if (gpTcpIpJoinBtn)
		{
			D2Win_10025_CONTROL_ToggleFirstFlag(&gpTcpIpJoinBtn->controlHeader, 1);
		}
	}
	else
	{
		if (const Unicode* wszNoIp = D2LANG_GetStringFromTblIndex(0x1404))
		{
			D2Win_10051(gpTcpIpAddrBox, wszNoIp, 0, 0, 0);
		}
		if (gpTcpIpHostBtn)
		{
			D2Win_10025_CONTROL_ToggleFirstFlag(&gpTcpIpHostBtn->controlHeader, 0);
		}
		if (gpTcpIpJoinBtn)
		{
			D2Win_10025_CONTROL_ToggleFirstFlag(&gpTcpIpJoinBtn->controlHeader, 0);
		}
	}
}

// 1.12a: 0x6FA53270
static int __stdcall D2LAUNCH_OnTcpIpRefreshTimer(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_UpdateTcpIpAddressDisplay();
	return 1;
}

// 1.12a: 0x6FA599E0
static int __stdcall D2LAUNCH_OnTcpIpHostClick(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (!gpD2Config)
	{
		return 0;
	}
	gpD2Config->dwGameType = 8;
	const char* szHostIp = FOG_GetHostIPAddress();
	SStrCopy(gpD2Config->szServerIP, (szHostIp && szHostIp[0]) ? szHostIp : "127.0.0.1", sizeof(gpD2Config->szServerIP));
	gnLaunchMode = 2;
	gpD2Config->nArenaFlags = 4;

	if (D2LAUNCH_HasSavedCharacters())
	{
		D2SOUND_FastFadeAll();
		D2LAUNCH_DestroyAllControls();
		D2LAUNCH_EnterCharSelect();
	}
	else
	{
		D2LAUNCH_EnterCharCreate();
	}
	return 1;
}

// 1.12a: 0x6FA52400
static int __stdcall D2LAUNCH_OnTcpIpJoinCancel(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyModalControls();
	if (!gpTcpIpTimer)
	{
		gpTcpIpTimer = reinterpret_cast<D2WinTimerStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlTcpIpTimer));
		if (gpTcpIpTimer)
		{
			D2Win_10180(gpTcpIpTimer);
			D2Win_10182(gpTcpIpTimer);
		}
	}
	return 1;
}

// 1.12a: 0x6FA50650
static void __stdcall D2LAUNCH_TcpIpConnectPump(DWORD dwUnused)
{
	(void)dwUnused;
	const int32_t nState = D2NET_10017();
	if (nState == 0)
	{
		gnTcpIpConnectResult = 0;
		D2Win_10009();
	}
	else if (nState == 2)
	{
		gnTcpIpConnectResult = 1;
		D2Win_10009();
	}
}

static void __stdcall D2LAUNCH_ReturnToTcpIpMenuAfterError(int32_t nUnused)
{
	(void)nUnused;
	D2LAUNCH_EnterTcpIpMenu();
}

// 1.12a: 0x6FA59830
static int __stdcall D2LAUNCH_OnTcpIpJoinConfirm(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (!gpD2Config || !gpJoinIpEditBox)
	{
		return 0;
	}

	char szHostInput[64] = {};
	if (const Unicode* wszInput = D2Win_10077_EDITBOX_GetText(gpJoinIpEditBox))
	{
		Unicode::unicode2Win(szHostInput, wszInput, sizeof(szHostInput));
	}

	// Trim leading whitespace
	const char* pTrim = szHostInput;
	while (*pTrim == ' ' || *pTrim == '\t')
	{
		++pTrim;
	}
	if (!pTrim[0])
	{
		return 1;
	}

	SRegSaveString("Diablo II", "LastTcpIp", 0, pTrim);

	char szResolvedIp[36] = {};
	int a = 0, b = 0, c = 0, d = 0;
	if (std::sscanf(pTrim, "%d.%d.%d.%d", &a, &b, &c, &d) == 4 &&
		a >= 0 && a <= 255 && b >= 0 && b <= 255 && c >= 0 && c <= 255 && d >= 0 && d <= 255)
	{
		std::snprintf(szResolvedIp, sizeof(szResolvedIp), "%d.%d.%d.%d", a, b, c, d);
	}
	else
	{
		const char* szDns = FOG_ResolveHostIPAddress(pTrim);
		if (szDns && szDns[0])
		{
			SStrCopy(szResolvedIp, szDns, sizeof(szResolvedIp));
		}
	}

	D2LAUNCH_DestroyModalControls();
	if (!szResolvedIp[0])
	{
		D2LAUNCH_ShowModalAlert(D2LANG_GetStringFromTblIndex(0x145B), D2LAUNCH_ReturnToTcpIpMenuAfterError, 1);
		return 1;
	}

	SStrCopy(gpD2Config->szServerIP, szResolvedIp, sizeof(gpD2Config->szServerIP));

	gnTcpIpConnectResult = 0;
	CLIENT_Initialize(0, gpD2Config->szServerIP);
	D2Win_10008(D2LAUNCH_TcpIpConnectPump);
	CLIENT_Release();

	if (!gnTcpIpConnectResult)
	{
		D2LAUNCH_ShowModalAlert(D2LANG_GetStringFromTblIndex(0x145B), D2LAUNCH_ReturnToTcpIpMenuAfterError, 1);
		return 1;
	}

	gnLaunchMode = 2;
	gpD2Config->dwGameType = 9;
	gpD2Config->wJoinID = 1;

	if (D2LAUNCH_HasSavedCharacters())
	{
		D2SOUND_FastFadeAll();
		D2LAUNCH_DestroyAllControls();
		D2LAUNCH_EnterCharSelect();
	}
	else
	{
		D2LAUNCH_EnterCharCreate();
	}
	return 1;
}

// 1.12a: 0x6FA524A0
static int __stdcall D2LAUNCH_OnTcpIpJoinPopup(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (gpTcpIpTimer)
	{
		for (int32_t i = 0; i < gnScreenControlCount; ++i)
		{
			if (gapScreenControls[i] == &gpTcpIpTimer->controlHeader)
			{
				CONTROL_Destroy(&gapScreenControls[i]);
				gapScreenControls[i] = nullptr;
				break;
			}
		}
		gpTcpIpTimer = nullptr;
	}

	D2LAUNCH_DestroyModalControls();
	gbModalOpen = 1;

	if (D2WinControlStrc* pPopupBg = D2LAUNCH_CreateModalControl(&gCtrlJoinIpPopupBg))
	{
		D2Win_10033(pPopupBg, 0);
	}
	D2LAUNCH_CreateModalControl(&gCtrlJoinIpFrame);
	if (D2WinTextBoxStrc* pPrompt = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateModalControl(&gCtrlJoinIpPrompt)))
	{
		D2Win_10051(pPrompt, D2LANG_GetStringFromTblIndex(0x2B1F), 4, 0, 0);
	}

	gpJoinIpEditBox = reinterpret_cast<D2WinEditBoxStrc*>(D2LAUNCH_CreateModalControl(&gCtrlJoinIpEdit));
	if (gpJoinIpEditBox)
	{
		D2Win_10081(gpJoinIpEditBox, 32);
		char szLastIp[64] = {};
		if (SRegLoadString("Diablo II", "LastTcpIp", 0, szLastIp, sizeof(szLastIp)) && szLastIp[0])
		{
			D2Win_10076(gpJoinIpEditBox, szLastIp);
		}
		D2Win_10012(gpJoinIpEditBox);
		D2Win_10082(gpJoinIpEditBox);
	}

	if (D2WinControlStrc* pCancel = D2LAUNCH_CreateModalControl(&gCtrlJoinIpCancel))
	{
		D2Win_10031(pCancel);
	}
	D2LAUNCH_CreateModalControl(&gCtrlJoinIpOk);
	return 1;
}

// 1.12a: 0x6FA56630
void __stdcall D2LAUNCH_EnterTcpIpMenu()
{
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_ApplyLocaleLayoutAdjustments();
	D2LAUNCH_SetPalette("palette\\sky\\pal.dat", "palette\\sky\\pal.pl2");

	D2LAUNCH_CreateScreenControl(&gCtrlTcpIpBg);
	if (D2WinTextBoxStrc* pTitle = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlTcpIpTitle)))
	{
		D2Win_10051(pTitle, D2LANG_GetStringFromTblIndex(0x13FD), 4, 0, 0);
	}
	if (D2WinTextBoxStrc* pYourIp = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlTcpIpYourIpLbl)))
	{
		D2Win_10051(pYourIp, D2LANG_GetStringFromTblIndex(0x1401), 4, 0, 0);
	}

	gpTcpIpAddrBox  = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlTcpIpAddrBox));
	gpTcpIpHostBtn  = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlTcpIpHostBtn));
	gpTcpIpJoinBtn  = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlTcpIpJoinBtn));
	gpTcpIpHostDesc = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlTcpIpHostDesc));
	if (gpTcpIpHostDesc)
	{
		D2Win_10051(gpTcpIpHostDesc, D2LANG_GetStringFromTblIndex(0x1402), 0, 0, 0);
		D2Win_10027_CONTROL_ToggleThirdFlag(&gpTcpIpHostDesc->controlHeader, 0);
	}
	gpTcpIpJoinDesc = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlTcpIpJoinDesc));
	if (gpTcpIpJoinDesc)
	{
		D2Win_10051(gpTcpIpJoinDesc, D2LANG_GetStringFromTblIndex(0x1403), 0, 0, 0);
		D2Win_10027_CONTROL_ToggleThirdFlag(&gpTcpIpJoinDesc->controlHeader, 0);
	}

	if (D2WinControlStrc* pCancel = D2LAUNCH_CreateScreenControl(&gCtrlTcpIpCancelBtn))
	{
		D2Win_10031(pCancel);
	}

	gpTcpIpTimer = reinterpret_cast<D2WinTimerStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlTcpIpTimer));
	if (gpTcpIpTimer)
	{
		D2Win_10180(gpTcpIpTimer);
		D2Win_10182(gpTcpIpTimer);
	}

	D2LAUNCH_UpdateTcpIpAddressDisplay();
}

// ============================================================================
// Credits Screen (0x6FA508D0, 0x6FA53CA0, 0x6FA55610, 0x6FA57660, 0x6FA57690)
// ============================================================================
// 1.12a: 0x6FA571B0
static int __stdcall D2LAUNCH_OnCreditsExit(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_EnterMainMenu(0);
	return 1;
}

// 1.12a: 0x6FA57660
static int __stdcall D2LAUNCH_OnCreditsScrollTimer(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (!gpCreditsLeftBox)
	{
		return 1;
	}
	if (D2Win_10049(gpCreditsLeftBox) >= gnCreditsEndLine)
	{
		D2LAUNCH_EnterMainMenu(0);
	}
	return 1;
}

// 1.12a: 0x6FA508D0
static void D2LAUNCH_PadCreditsBoxesToMaxLines()
{
	if (!gpCreditsLeftBox || !gpCreditsRightBox || !gpCreditsHdrBox)
	{
		return;
	}
	static const Unicode kBlank[2] = { Unicode(' '), Unicode(0) };
	int32_t nMax = D2Win_10052_TEXTBOX_GetNumLines(gpCreditsLeftBox);
	const int32_t nRight = D2Win_10052_TEXTBOX_GetNumLines(gpCreditsRightBox);
	const int32_t nHdr   = D2Win_10052_TEXTBOX_GetNumLines(gpCreditsHdrBox);
	if (nRight > nMax) nMax = nRight;
	if (nHdr   > nMax) nMax = nHdr;

	while (D2Win_10052_TEXTBOX_GetNumLines(gpCreditsLeftBox) < nMax)
	{
		D2Win_10051(gpCreditsLeftBox, kBlank, 0, 0, 0);
	}
	while (D2Win_10052_TEXTBOX_GetNumLines(gpCreditsRightBox) < nMax)
	{
		D2Win_10051(gpCreditsRightBox, kBlank, 0, 0, 0);
	}
	while (D2Win_10052_TEXTBOX_GetNumLines(gpCreditsHdrBox) < nMax)
	{
		D2Win_10051(gpCreditsHdrBox, kBlank, 0, 0, 0);
	}
}

// 1.12a: 0x6FA53CA0
static Unicode* D2LAUNCH_LoadCreditsUnicodeText(size_t* pOutCharCount)
{
	if (pOutCharCount)
	{
		*pOutCharCount = 0;
	}

	char szLang[16] = {};
	STRTABLE_GetLanguageInfo(szLang, 0);
	if (!szLang[0])
	{
		SStrCopy(szLang, "ENG", sizeof(szLang));
	}

	const char* szFileName = FOG_IsExpansion() ? "ExpansionCredits.txt" : "Credits.txt";
	char szMpqPath[MAX_PATH] = {};
	std::snprintf(szMpqPath, sizeof(szMpqPath), "DATA\\LOCAL\\UI\\%s\\%s", szLang, szFileName);

	HSFILE hFile = nullptr;
	if (!FOG_FOpenFile(szMpqPath, &hFile) || !hFile)
	{
		std::snprintf(szMpqPath, sizeof(szMpqPath), "DATA\\LOCAL\\UI\\ENG\\%s", szFileName);
		if (!FOG_FOpenFile(szMpqPath, &hFile) || !hFile)
		{
			return nullptr;
		}
	}

	const uint32_t dwByteSize = FOG_FGetFileSize(hFile, nullptr);
	if (dwByteSize == 0)
	{
		FOG_FCloseFile(hFile);
		return nullptr;
	}

	uint8_t* pRaw = static_cast<uint8_t*>(D2_CALLOC(dwByteSize + 4));
	int nBytesRead = 0;
	FOG_FReadFile(hFile, pRaw, dwByteSize, &nBytesRead, 0, 0, 0);
	FOG_FCloseFile(hFile);

	if (nBytesRead <= 0)
	{
		D2_FREE(pRaw);
		return nullptr;
	}

	if (nBytesRead >= 2 && pRaw[0] == 0xFF && pRaw[1] == 0xFE)
	{
		const size_t nChars = static_cast<size_t>(nBytesRead - 2) / 2;
		Unicode* pOut = static_cast<Unicode*>(D2_CALLOC((nChars + 2) * sizeof(Unicode)));
		std::memcpy(pOut, pRaw + 2, nChars * sizeof(Unicode));
		D2_FREE(pRaw);
		if (pOutCharCount) *pOutCharCount = nChars;
		return pOut;
	}
	if (nBytesRead >= 2 && pRaw[0] == 0xFE && pRaw[1] == 0xFF)
	{
		const size_t nChars = static_cast<size_t>(nBytesRead - 2) / 2;
		Unicode* pOut = static_cast<Unicode*>(D2_CALLOC((nChars + 2) * sizeof(Unicode)));
		for (size_t i = 0; i < nChars; ++i)
		{
			const uint16_t w = (static_cast<uint16_t>(pRaw[2 + i * 2]) << 8) | pRaw[3 + i * 2];
			pOut[i] = Unicode(w);
		}
		D2_FREE(pRaw);
		if (pOutCharCount) *pOutCharCount = nChars;
		return pOut;
	}

	const size_t nChars = static_cast<size_t>(nBytesRead);
	Unicode* pOut = static_cast<Unicode*>(D2_CALLOC((nChars + 2) * sizeof(Unicode)));
	Unicode::win2Unicode(pOut, reinterpret_cast<const char*>(pRaw), static_cast<int>(nChars + 1));
	D2_FREE(pRaw);
	if (pOutCharCount) *pOutCharCount = nChars;
	return pOut;
}

// 1.12a: 0x6FA55610
static void D2LAUNCH_FormatCreditsLines()
{
	if (!gpCreditsLeftBox || !gpCreditsRightBox || !gpCreditsHdrBox)
	{
		return;
	}

	static const Unicode kBlank[2] = { Unicode(' '), Unicode(0) };
	const int32_t nInitPad = FOG_IsExpansion() ? 39 : 30;
	for (int32_t i = 0; i < nInitPad; ++i)
	{
		D2Win_10051(gpCreditsLeftBox, kBlank, 0, 0, 0);
	}
	D2LAUNCH_PadCreditsBoxesToMaxLines();

	size_t nTotalChars = 0;
	Unicode* pCreditsBuf = D2LAUNCH_LoadCreditsUnicodeText(&nTotalChars);
	if (pCreditsBuf)
	{
		size_t nPos = 0;
		bool bRightColNext = false;
		while (nPos < nTotalChars && pCreditsBuf[nPos] != Unicode(0))
		{
			size_t nLineStart = nPos;
			while (nPos < nTotalChars && pCreditsBuf[nPos] != Unicode(0) &&
				pCreditsBuf[nPos] != Unicode('\r') && pCreditsBuf[nPos] != Unicode('\n'))
			{
				++nPos;
			}
			const size_t nLineLen = nPos - nLineStart;
			if (nPos < nTotalChars && pCreditsBuf[nPos] == Unicode('\r')) ++nPos;
			if (nPos < nTotalChars && pCreditsBuf[nPos] == Unicode('\n')) ++nPos;

			Unicode wszLine[256] = {};
			const size_t nCopy = (nLineLen < 255) ? nLineLen : 255;
			if (nCopy > 0)
			{
				std::memcpy(wszLine, pCreditsBuf + nLineStart, nCopy * sizeof(Unicode));
			}
			wszLine[nCopy] = Unicode(0);

			if (nCopy == 0)
			{
				bRightColNext = false;
				D2LAUNCH_PadCreditsBoxesToMaxLines();
				D2Win_10051(gpCreditsHdrBox, kBlank, 0, 0, 0);
				D2LAUNCH_PadCreditsBoxesToMaxLines();
			}
			else if (wszLine[0] == Unicode('*'))
			{
				bRightColNext = false;
				D2LAUNCH_PadCreditsBoxesToMaxLines();
				const Unicode* pHdrText = (wszLine[1] != Unicode(0)) ? (wszLine + 1) : kBlank;
				D2Win_10051(gpCreditsHdrBox, pHdrText, 1, 0, 0);
				D2LAUNCH_PadCreditsBoxesToMaxLines();
			}
			else
			{
				// Check for comma-separated two-column entry ("LeftName,RightName")
				int nCommaIdx = -1;
				for (size_t k = 0; k < nCopy; ++k)
				{
					if (wszLine[k] == Unicode(','))
					{
						nCommaIdx = static_cast<int>(k);
						break;
					}
				}
				if (nCommaIdx >= 0)
				{
					wszLine[nCommaIdx] = Unicode(0);
					const Unicode* pLeft = (nCommaIdx > 0) ? wszLine : kBlank;
					const Unicode* pRight = wszLine + nCommaIdx + 1;
					while (*pRight == Unicode(' ')) ++pRight;
					if (*pRight == Unicode(0)) pRight = kBlank;
					D2LAUNCH_PadCreditsBoxesToMaxLines();
					D2Win_10051(gpCreditsLeftBox, pLeft, 0, 0, 0);
					D2Win_10051(gpCreditsRightBox, pRight, 0, 0, 0);
					D2LAUNCH_PadCreditsBoxesToMaxLines();
					bRightColNext = false;
				}
				else if (!bRightColNext)
				{
					D2LAUNCH_PadCreditsBoxesToMaxLines();
					D2Win_10051(gpCreditsLeftBox, wszLine, 0, 0, 0);
					bRightColNext = true;
				}
				else
				{
					D2Win_10051(gpCreditsRightBox, wszLine, 0, 0, 0);
					D2LAUNCH_PadCreditsBoxesToMaxLines();
					bRightColNext = false;
				}
			}
		}
		D2_FREE(pCreditsBuf);
	}

	D2LAUNCH_PadCreditsBoxesToMaxLines();
	for (int32_t i = 0; i < 6; ++i)
	{
		D2Win_10051(gpCreditsHdrBox, kBlank, 0, 0, 0);
	}
	Unicode wszTheEnd[32] = {};
	Unicode::win2Unicode(wszTheEnd, "The End", 32);
	D2Win_10051(gpCreditsHdrBox, wszTheEnd, 1, 0, 0);
	D2LAUNCH_PadCreditsBoxesToMaxLines();

	gnCreditsEndLine = D2Win_10052_TEXTBOX_GetNumLines(gpCreditsLeftBox);
	for (int32_t i = 0; i < nInitPad; ++i)
	{
		D2Win_10051(gpCreditsLeftBox, kBlank, 0, 0, 0);
	}
	D2LAUNCH_PadCreditsBoxesToMaxLines();
}

// 1.12a: 0x6FA57690
int __stdcall D2LAUNCH_EnterCreditsScreen(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_SetPalette("palette\\sky\\pal.dat", "palette\\sky\\pal.pl2");

	D2LAUNCH_CreateScreenControl(&gCtrlCreditsBg);
	if (D2WinControlStrc* pExit = D2LAUNCH_CreateScreenControl(&gCtrlCreditsExit))
	{
		D2Win_10031(pExit);
	}

	if (FOG_IsExpansion())
	{
		gpCreditsLeftBox  = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreditsExpLeft));
		gpCreditsRightBox = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreditsExpRght));
		gpCreditsHdrBox   = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreditsExpHdr));
	}
	else
	{
		gpCreditsLeftBox  = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreditsClLeft));
		gpCreditsRightBox = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreditsClRight));
		gpCreditsHdrBox   = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreditsClHdr));
	}

	if (D2WinTimerStrc* pTimer = reinterpret_cast<D2WinTimerStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreditsTimer)))
	{
		D2Win_10180(pTimer);
		D2Win_10182(pTimer);
	}

	D2LAUNCH_FormatCreditsLines();
	if (gpCreditsLeftBox)  D2Win_10053(gpCreditsLeftBox, 20);
	if (gpCreditsRightBox) D2Win_10053(gpCreditsRightBox, 20);
	if (gpCreditsHdrBox)   D2Win_10053(gpCreditsHdrBox, 20);
	return 1;
}

// ============================================================================
// Cinematics Selection Screen (0x6FA57380, 0x6FA552B0, 0x6FA58410..0x6FA586E0, 0x6FA57340)
// ============================================================================
// 1.12a: 0x6FA58410
static int D2LAUNCH_PlaySelectedCinematic(const char* szBaseVideoName, BOOL bExpansionDisc)
{
	(void)bExpansionDisc;
	if (!szBaseVideoName)
	{
		return 1;
	}
	if (WINDOW_CheckCutScene())
	{
		char szVideoPath[MAX_PATH] = {};
		std::snprintf(szVideoPath, sizeof(szVideoPath), "data\\local\\video\\%s640x480.bik", szBaseVideoName);
		D2Win_10003();
		WINDOW_OpenSmackCutscene(szVideoPath, D2GAMERES_640x480, nullptr);
		D2Win_10004(D2GAMERES_800x600);
	}
	return 1;
}

static int __stdcall D2LAUNCH_OnCinematic1(SMSGHANDLER_PARAMS* pMsg) { (void)pMsg; return D2LAUNCH_PlaySelectedCinematic("d2intro", FALSE); } // 0x6FA586E0
static int __stdcall D2LAUNCH_OnCinematic2(SMSGHANDLER_PARAMS* pMsg) { (void)pMsg; return D2LAUNCH_PlaySelectedCinematic("Act02start", FALSE); } // 0x6FA58690
static int __stdcall D2LAUNCH_OnCinematic3(SMSGHANDLER_PARAMS* pMsg) { (void)pMsg; return D2LAUNCH_PlaySelectedCinematic("Act03start", FALSE); } // 0x6FA58640
static int __stdcall D2LAUNCH_OnCinematic4(SMSGHANDLER_PARAMS* pMsg) { (void)pMsg; return D2LAUNCH_PlaySelectedCinematic("Act04start", FALSE); } // 0x6FA585F0
static int __stdcall D2LAUNCH_OnCinematic5(SMSGHANDLER_PARAMS* pMsg) { (void)pMsg; return D2LAUNCH_PlaySelectedCinematic("Act04end", FALSE); }   // 0x6FA585A0
static int __stdcall D2LAUNCH_OnCinematic6(SMSGHANDLER_PARAMS* pMsg) { (void)pMsg; return D2LAUNCH_PlaySelectedCinematic("d2x_intro", TRUE); }   // 0x6FA58550
static int __stdcall D2LAUNCH_OnCinematic7(SMSGHANDLER_PARAMS* pMsg) { (void)pMsg; return D2LAUNCH_PlaySelectedCinematic("d2x_out", TRUE); }     // 0x6FA58500

// 1.12a: 0x6FA57340
static int __stdcall D2LAUNCH_OnCinematicsCancel(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (D2SOUND_GetOptionsMusic())
	{
		D2SOUND_EnableMusic(1);
	}
	D2LAUNCH_EnterMainMenu(0);
	return 1;
}

// 1.12a: 0x6FA57380 / 0x6FA552B0
int __stdcall D2LAUNCH_EnterCinematicsScreen(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (D2SOUND_IsMusicPlaying())
	{
		D2SOUND_FadeMusic();
		D2SOUND_EnableMusic(0);
	}

	D2LAUNCH_SetupMainMenuBackdrop();

	// Parse "Aux Battle.net" cinematic unlock bitmask (0x6FA57395..0x6FA574C8)
	uint8_t nUnlockMask = 0x01; // Bit 0: Intro + LoD Intro always unlocked
	char szAuxReg[64] = {};
	if (SRegLoadString("Diablo II", "Aux Battle.net", 0, szAuxReg, sizeof(szAuxReg)) && szAuxReg[0])
	{
		const unsigned long ulParsed = std::strtoul(szAuxReg, nullptr, 10);
		if (ulParsed != 0)
		{
			nUnlockMask = 0x7Fu;
		}
	}
	// Also unlock acts based on highest local character progression
	D2LAUNCH_EnumerateLocalSavedCharacters();
	for (D2LaunchCharNodeStrc* pNode = D2LAUNCH_GetCharListHead(); pNode != nullptr; pNode = pNode->pNext)
	{
		const uint8_t nProg = static_cast<uint8_t>((pNode->wSaveFlags >> 8) & 0x1Fu);
		if (nProg >= 1) nUnlockMask |= 0x02u;
		if (nProg >= 2) nUnlockMask |= 0x04u;
		if (nProg >= 3) nUnlockMask |= 0x08u;
		if (nProg >= 4) nUnlockMask |= 0x10u;
		if ((pNode->wSaveFlags & 0x20u) && nProg >= 5) nUnlockMask |= 0x40u;
	}
	D2LAUNCH_FreeCharList();

	D2LAUNCH_CreateScreenControl(&gCtrlCinematicsPanel);
	if (D2WinTextBoxStrc* pTitle = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCinematicsTitle)))
	{
		D2Win_10051(pTitle, D2LANG_GetStringFromTblIndex(0x13FA), 4, 0, 0);
	}

	D2WinControlStrc* apBtns[7] = {
		D2LAUNCH_CreateScreenControl(&gCtrlCinematicBtn1),
		D2LAUNCH_CreateScreenControl(&gCtrlCinematicBtn2),
		D2LAUNCH_CreateScreenControl(&gCtrlCinematicBtn3),
		D2LAUNCH_CreateScreenControl(&gCtrlCinematicBtn4),
		D2LAUNCH_CreateScreenControl(&gCtrlCinematicBtn5),
		FOG_IsExpansion() ? D2LAUNCH_CreateScreenControl(&gCtrlCinematicBtn6) : nullptr,
		FOG_IsExpansion() ? D2LAUNCH_CreateScreenControl(&gCtrlCinematicBtn7) : nullptr,
	};

	const uint8_t aReqBit[7] = { 0x01u, 0x02u, 0x04u, 0x08u, 0x10u, 0x01u, 0x40u };
	for (int i = 0; i < 7; ++i)
	{
		if (apBtns[i])
		{
			D2Win_10025_CONTROL_ToggleFirstFlag(apBtns[i], (nUnlockMask & aReqBit[i]) ? 1 : 0);
		}
	}

	if (D2WinControlStrc* pCancel = D2LAUNCH_CreateScreenControl(&gCtrlCinematicCancel))
	{
		D2Win_10031(pCancel);
	}
	return 1;
}

// ============================================================================
// Campfire Character Create Screen (0x6FA50CD0..0x6FA59B80)
// ============================================================================
static const int16_t kCreateClassNameTblIds[7] = { 4011, 4010, 4009, 4008, 4007, 10097, 10098 };
static const int16_t kCreateClassDescTblIds[7] = { 5128, 5131, 5129, 5132, 5130, 22518, 22519 };
static const char* const kSelectClassWav[7] = {
	"data\\global\\sfx\\cursor\\intro\\amazon select.wav",
	"data\\global\\sfx\\cursor\\intro\\sorceress select.wav",
	"data\\global\\sfx\\cursor\\intro\\necromancer select.wav",
	"data\\global\\sfx\\cursor\\intro\\paladin select.wav",
	"data\\global\\sfx\\cursor\\intro\\barbarian select.wav",
	"data\\global\\sfx\\cursor\\intro\\druid select.wav",
	"data\\global\\sfx\\cursor\\intro\\assassin select.wav",
};
static const char* const kDeselectClassWav[7] = {
	"data\\global\\sfx\\cursor\\intro\\amazon deselect.wav",
	"data\\global\\sfx\\cursor\\intro\\sorceress deselect.wav",
	"data\\global\\sfx\\cursor\\intro\\necromancer deselect.wav",
	"data\\global\\sfx\\cursor\\intro\\paladin deselect.wav",
	"data\\global\\sfx\\cursor\\intro\\barbarian deselect.wav",
	"data\\global\\sfx\\cursor\\intro\\druid deselect.wav",
	"data\\global\\sfx\\cursor\\intro\\assassin deselect.wav",
};

// 1.12a: 0x6FA50CD0
static void D2LAUNCH_UpdateCreateClassBanner(int32_t nClassId)
{
	if (!gpCreateClassNameBox || !gpCreateClassDescBox)
	{
		return;
	}
	D2Win_10047(gpCreateClassNameBox);
	D2Win_10047(gpCreateClassDescBox);
	if (nClassId >= 0 && nClassId < 7)
	{
		if (const Unicode* wszName = D2LANG_GetStringFromTblIndex(kCreateClassNameTblIds[nClassId]))
		{
			D2Win_10051(gpCreateClassNameBox, wszName, 4, 0, 0);
		}
		if (const Unicode* wszDesc = D2LANG_GetStringFromTblIndex(kCreateClassDescTblIds[nClassId]))
		{
			D2Win_10051(gpCreateClassDescBox, wszDesc, 0, 0, 0);
		}
	}
}

static int D2LAUNCH_HandleCampfireHover(int32_t nClassId, SMSGHANDLER_PARAMS* pMsg)
{
	if (gnSelectedCreateClass != -1)
	{
		return 1;
	}
	if (pMsg && pMsg->wParam)
	{
		gnHoveredCreateClass = nClassId;
		D2LAUNCH_UpdateCreateClassBanner(nClassId);
	}
	else if (gnHoveredCreateClass == nClassId)
	{
		gnHoveredCreateClass = -1;
		D2LAUNCH_UpdateCreateClassBanner(-1);
	}
	return 1;
}

static int __stdcall D2LAUNCH_OnCampfireHoverBarb(SMSGHANDLER_PARAMS* pMsg)  { return D2LAUNCH_HandleCampfireHover(4, pMsg); } // 0x6FA50D60
static int __stdcall D2LAUNCH_OnCampfireHoverNecro(SMSGHANDLER_PARAMS* pMsg) { return D2LAUNCH_HandleCampfireHover(2, pMsg); } // 0x6FA50D90
static int __stdcall D2LAUNCH_OnCampfireHoverAma(SMSGHANDLER_PARAMS* pMsg)   { return D2LAUNCH_HandleCampfireHover(0, pMsg); } // 0x6FA50DC0
static int __stdcall D2LAUNCH_OnCampfireHoverSorc(SMSGHANDLER_PARAMS* pMsg)  { return D2LAUNCH_HandleCampfireHover(1, pMsg); } // 0x6FA50DF0
static int __stdcall D2LAUNCH_OnCampfireHoverPal(SMSGHANDLER_PARAMS* pMsg)   { return D2LAUNCH_HandleCampfireHover(3, pMsg); } // 0x6FA50E20
static int __stdcall D2LAUNCH_OnCampfireHoverDru(SMSGHANDLER_PARAMS* pMsg)   { return D2LAUNCH_HandleCampfireHover(5, pMsg); } // 0x6FA52B20
static int __stdcall D2LAUNCH_OnCampfireHoverAssa(SMSGHANDLER_PARAMS* pMsg)  { return D2LAUNCH_HandleCampfireHover(6, pMsg); } // 0x6FA52B60

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
			if (const Unicode* pwszText = D2Win_10077_EDITBOX_GetText(pEditBox))
			{
				for (const Unicode* p = pwszText; *p; ++p)
				{
					if (*p == Unicode('-') || *p == Unicode('_'))
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
			if (++nHyphenCount >= 2)
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
		if (const Unicode* pwszText = D2Win_10077_EDITBOX_GetText(gpCreateNameEditBox))
		{
			Unicode::unicode2Win(szTypedName, pwszText, sizeof(szTypedName));
		}
	}
	if (gpCreateOkBtn)
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(&gpCreateOkBtn->controlHeader, D2LAUNCH_IsCharNameValid(szTypedName) ? 1 : 0);
	}
}

static void D2LAUNCH_UpdateCreateControlsVisibility()
{
	const BOOL bSelected = (gnSelectedCreateClass >= 0 && gnSelectedCreateClass < 7) ? 1 : 0;

	if (gpCreateNameFrameImg) D2Win_10027_CONTROL_ToggleThirdFlag(&gpCreateNameFrameImg->controlHeader, bSelected);
	if (gpCreateNameLabelBox) D2Win_10027_CONTROL_ToggleThirdFlag(&gpCreateNameLabelBox->controlHeader, bSelected);
	if (gpCreateNameEditBox)  D2Win_10027_CONTROL_ToggleThirdFlag(&gpCreateNameEditBox->controlHeader, bSelected);
	if (gpCreateOkBtn)        D2Win_10027_CONTROL_ToggleThirdFlag(&gpCreateOkBtn->controlHeader, bSelected);

	const BOOL bShowHc = (bSelected && gdwAllowHardcore != 0) ? 1 : 0;
	if (gpCreateHcLabelBox)   D2Win_10027_CONTROL_ToggleThirdFlag(&gpCreateHcLabelBox->controlHeader, bShowHc);
	if (gpCreateHcCheckBox)   D2Win_10027_CONTROL_ToggleThirdFlag(&gpCreateHcCheckBox->controlHeader, bShowHc);

	if (FOG_IsExpansion())
	{
		const bool bExpOnlyClass = (gnSelectedCreateClass == 5 || gnSelectedCreateClass == 6);
		if (gpCreateExpLabelBox)
		{
			D2Win_10027_CONTROL_ToggleThirdFlag(&gpCreateExpLabelBox->controlHeader, bSelected);
		}
		if (gpCreateExpCheckBox)
		{
			D2Win_10027_CONTROL_ToggleThirdFlag(&gpCreateExpCheckBox->controlHeader, (bSelected && !bExpOnlyClass) ? 1 : 0);
			if (bSelected && !bExpOnlyClass)
			{
				BUTTON_SetIsPressed(gpCreateExpCheckBox, 1);
			}
		}
		if (gpCreateExpGreyImg)
		{
			D2Win_10027_CONTROL_ToggleThirdFlag(&gpCreateExpGreyImg->controlHeader, (bSelected && bExpOnlyClass) ? 1 : 0);
		}
	}

	if (bSelected && gpCreateNameEditBox)
	{
		D2Win_10012(gpCreateNameEditBox);
		D2LAUNCH_OnNameTextChanged(0);
	}
	else
	{
		D2Win_10012(nullptr);
	}
}

// 1.12a: 0x6FA52C00
static int __stdcall D2LAUNCH_OnCampfireClassClick(SMSGHANDLER_PARAMS* pMsg)
{
	if (!pMsg || !pMsg->hWindow)
	{
		return 0;
	}
	D2WinAnimImageStrc* pClicked = reinterpret_cast<D2WinAnimImageStrc*>(pMsg->hWindow);
	const int32_t nPrevAnimState = D2Win_10106(pClicked);

	const int nCount = FOG_IsExpansion() ? 7 : 5;
	for (int i = 0; i < nCount; ++i)
	{
		if (gpCampfireOrder[i] && !D2Win_10104(gpCampfireOrder[i]))
		{
			return 0;
		}
	}

	const int32_t nPrevClass = gnSelectedCreateClass;
	gnSelectedCreateClass = -1;
	if (nPrevClass >= 0 && nPrevClass < 7)
	{
		D2SOUND_FastFadeAll();
		D2SOUND_FastPlaySound(kDeselectClassWav[nPrevClass]);
	}

	static const int32_t kOrderToClass[7] = { 4, 2, 0, 1, 3, 5, 6 };
	int32_t nClickedClass = -1;
	for (int i = 0; i < nCount; ++i)
	{
		if (gpCampfireOrder[i] == pClicked)
		{
			nClickedClass = kOrderToClass[i];
			break;
		}
	}

	if (nClickedClass >= 0 && nClickedClass != nPrevClass)
	{
		gnSelectedCreateClass = nClickedClass;
		if (pClicked->nAnimType == 0)
		{
			pClicked->nAnimType = 1;
		}
		if (nPrevAnimState <= 1)
		{
			D2SOUND_FastPlaySound(kSelectClassWav[nClickedClass]);
		}
		D2LAUNCH_SetConfigCharacterClass(static_cast<uint8_t>(nClickedClass));
		D2LAUNCH_UpdateCreateClassBanner(nClickedClass);
	}
	else
	{
		D2LAUNCH_UpdateCreateClassBanner(-1);
	}

	D2LAUNCH_UpdateCreateControlsVisibility();
	return 1;
}

// 1.12a: 0x6FA572C0
static int __stdcall D2LAUNCH_OnCreateCancel(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2SOUND_FastFadeAll();
	if (D2LAUNCH_HasSavedCharacters())
	{
		D2LAUNCH_DestroyAllControls();
		D2LAUNCH_EnterCharSelect();
	}
	else if (gnLaunchMode == 2)
	{
		D2LAUNCH_EnterTcpIpMenu();
	}
	else
	{
		D2LAUNCH_EnterMainMenu(0);
	}
	return 1;
}

// 1.12a: 0x6FA59B80 / 0x6FA59B90
static int __stdcall D2LAUNCH_FinalizeCharacterCreation()
{
	if (!gpD2Config || gnSelectedCreateClass < 0 || gnSelectedCreateClass >= 7)
	{
		return 0;
	}

	uint16_t wSaveFlags = 0;
	if (FOG_IsExpansion())
	{
		if (gnSelectedCreateClass == 5 || gnSelectedCreateClass == 6 ||
			!gpCreateExpCheckBox || BUTTON_IsPressed(gpCreateExpCheckBox))
		{
			wSaveFlags |= 0x20u;
		}
	}
	if (gpCreateHcCheckBox && gdwAllowHardcore != 0 && BUTTON_IsPressed(gpCreateHcCheckBox))
	{
		wSaveFlags |= 0x04u;
	}

	gpD2Config->unpackedCTemp.nCharacterSaveFlags = wSaveFlags;
	D2LAUNCH_SetConfigCharacterClass(static_cast<uint8_t>(gnSelectedCreateClass));

	D2LAUNCH_InitNewSaveAndPreview(
		&gpD2Config->tCharPreviewInfo,
		gpD2Config->unpackedCTemp.nCharacterClassId,
		gpD2Config->szName,
		gpD2Config->unpackedCTemp.nCharacterSaveFlags);
	if (!D2LAUNCH_WriteNewSaveFile(gpD2Config->szName))
	{
		D2LAUNCH_ShowModalAlert(D2LANG_GetStringFromTblIndex(0x142C), nullptr, 1);
		return 1;
	}

	gpD2Config->nArenaDifficulty = 0;
	uint32_t dwArenaFlags = (wSaveFlags & 0x04u) ? 0x804u : 0x004u;
	if (wSaveFlags & 0x20u)
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

// 1.12a: 0x6FA56180
static int __stdcall D2LAUNCH_OnHardcoreCancel(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyModalControls();
	if (gpCreateNameEditBox)
	{
		D2Win_10012(gpCreateNameEditBox);
	}
	return 1;
}

// 1.12a: 0x6FA59B80
static int __stdcall D2LAUNCH_OnHardcoreConfirm(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyModalControls();
	return D2LAUNCH_FinalizeCharacterCreation();
}

// 1.12a: 0x6FA53110
static void D2LAUNCH_OpenHardcoreWarningModal()
{
	D2LAUNCH_DestroyModalControls();
	gbModalOpen = 1;

	if (D2WinControlStrc* pBg = D2LAUNCH_CreateModalControl(&gCtrlHcWarnBg))
	{
		D2Win_10033(pBg, 1);
	}
	if (D2WinControlStrc* pCancel = D2LAUNCH_CreateModalControl(&gCtrlHcWarnCancel))
	{
		D2Win_10031(pCancel);
	}
	D2LAUNCH_CreateModalControl(&gCtrlHcWarnOk);
	if (D2WinTextBoxStrc* pText = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateModalControl(&gCtrlHcWarnText)))
	{
		D2Win_10051(pText, D2LANG_GetStringFromTblIndex(0x14B7), 1, 0, 0);
	}
}

static void __stdcall D2LAUNCH_RefocusNameEditAfterAlert(int32_t nUnused)
{
	(void)nUnused;
	if (gpCreateNameEditBox)
	{
		D2Win_10012(gpCreateNameEditBox);
		D2Win_10082(gpCreateNameEditBox);
	}
}

// 1.12a: 0x6FA50510
static bool D2LAUNCH_DoesSaveAlreadyExist(const char* szCharName)
{
	char szSaveDir[0x400] = {};
	char szFilePath[0x600] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
	std::snprintf(szFilePath, sizeof(szFilePath), "%s%s.d2s", szSaveDir, szCharName);
	WIN32_FIND_DATAA tFind = {};
	HANDLE hFind = FindFirstFileA(szFilePath, &tFind);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	FindClose(hFind);
	return true;
}

// 1.12a: 0x6FA59AB0
static int __stdcall D2LAUNCH_OnCreateOkClick(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (gbModalOpen || !gpD2Config || gnSelectedCreateClass < 0 || gnSelectedCreateClass >= 7)
	{
		return 0;
	}
	if (!gpCreateOkBtn || !D2Win_10024_CONTROL_CheckFirstFlag(&gpCreateOkBtn->controlHeader))
	{
		return 0;
	}

	char szTypedName[32] = {};
	if (gpCreateNameEditBox)
	{
		if (const Unicode* pwszText = D2Win_10077_EDITBOX_GetText(gpCreateNameEditBox))
		{
			Unicode::unicode2Win(szTypedName, pwszText, sizeof(szTypedName));
		}
	}
	if (!D2LAUNCH_IsCharNameValid(szTypedName))
	{
		D2LAUNCH_ShowModalAlert(D2LANG_GetStringFromTblIndex(0x1491), D2LAUNCH_RefocusNameEditAfterAlert, 1);
		return 1;
	}

	// Disk free-space check (64-bit safe equivalent of 0x6FA59AFA >= 5000 bytes)
	char szSaveDir[0x400] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
	ULARGE_INTEGER uliFreeAvailable = {};
	if (GetDiskFreeSpaceExA(szSaveDir[0] ? szSaveDir : nullptr, &uliFreeAvailable, nullptr, nullptr) &&
		uliFreeAvailable.QuadPart < 5000ull)
	{
		D2LAUNCH_ShowModalAlert(D2LANG_GetStringFromTblIndex(0x142C), D2LAUNCH_RefocusNameEditAfterAlert, 1);
		return 1;
	}

	// Duplicate character check (0x6FA50510 / 0x6FA59240 -> string 0x142D = 5165)
	if (D2LAUNCH_DoesSaveAlreadyExist(szTypedName))
	{
		D2LAUNCH_ShowModalAlert(D2LANG_GetStringFromTblIndex(0x142D), D2LAUNCH_RefocusNameEditAfterAlert, 1);
		return 1;
	}

	SStrCopy(gpD2Config->szName, szTypedName, sizeof(gpD2Config->szName));

	if (gpCreateHcCheckBox && gdwAllowHardcore != 0 && BUTTON_IsPressed(gpCreateHcCheckBox))
	{
		D2LAUNCH_OpenHardcoreWarningModal();
		return 1;
	}

	return D2LAUNCH_FinalizeCharacterCreation();
}

// 1.12a: 0x6FA55980
BOOL __stdcall D2LAUNCH_EnterCharCreate()
{
	D2LAUNCH_LoadCampfireSprites();
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_ApplyLocaleLayoutAdjustments();

	if (gpD2Config)
	{
		gpD2Config->unpackedCTemp.nCharacterSaveFlags = 0;
	}
	gnHoveredCreateClass = -1;
	gnSelectedCreateClass = -1;

	DWORD dwRegHc = 0;
	if (SRegLoadValue("Diablo II", "AllowHardcore", 0, &dwRegHc) && dwRegHc != 0)
	{
		gdwAllowHardcore = 1;
	}
#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 13
	gdwAllowHardcore = 1;
#endif

	D2LAUNCH_SetPalette("palette\\fechar\\pal.dat", "palette\\fechar\\pal.pl2");

	D2LAUNCH_CreateScreenControl(&gCtrlCreateBg);
	gpCreateClassNameBox = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateClsName));
	gpCreateClassDescBox = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateClsDesc));

	if (D2WinTextBoxStrc* pTopTitle = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateTopTitle)))
	{
		D2Win_10051(pTopTitle, D2LANG_GetStringFromTblIndex(0x1407), 0, 0, 0);
	}

	if (D2WinControlStrc* pExit = D2LAUNCH_CreateScreenControl(&gCtrlCreateExit))
	{
		D2Win_10031(pExit);
	}

	gpCreateNameFrameImg = reinterpret_cast<D2WinImageStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateNameImg));
	gpCreateNameLabelBox = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateNameLbl));
	if (gpCreateNameLabelBox)
	{
		D2Win_10051(gpCreateNameLabelBox, D2LANG_GetStringFromTblIndex(0x1405), 4, 0, 0);
	}

	gpCreateNameEditBox = reinterpret_cast<D2WinEditBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateNameEdit));
	if (gpCreateNameEditBox)
	{
		D2Win_10081(gpCreateNameEditBox, 16);
		D2Win_10072(gpCreateNameEditBox, reinterpret_cast<int32_t>(D2LAUNCH_EditBoxCharFilter));
		D2Win_10073(gpCreateNameEditBox, D2LAUNCH_OnNameTextChanged);
	}

	const BOOL bExpansion = FOG_IsExpansion();
	if (bExpansion)
	{
		gpCreateExpLabelBox = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateExpLbl));
		if (gpCreateExpLabelBox)
		{
			D2Win_10051(gpCreateExpLabelBox, D2LANG_GetStringFromTblIndex(0x58CB), 4, 0, 0);
		}
		gpCreateExpCheckBox = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateExpChk));
		if (gpCreateExpCheckBox)
		{
			BUTTON_SetIsPressed(gpCreateExpCheckBox, 1);
		}
		gpCreateExpGreyImg = reinterpret_cast<D2WinImageStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateExpGrey));
		if (gpCreateExpGreyImg && gpImgClickBoxGrey)
		{
			IMAGE_SetFrame(gpCreateExpGreyImg, 1);
		}

		gpCreateHcLabelBox = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateHcLblExp));
		gpCreateHcCheckBox = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateHcChkExp));
	}
	else
	{
		gpCreateHcLabelBox = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateHcLblCl));
		gpCreateHcCheckBox = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateHcChkCl));
	}

	if (gpCreateHcLabelBox)
	{
		D2Win_10051(gpCreateHcLabelBox, D2LANG_GetStringFromTblIndex(0x1406), 4, 0, 0);
	}
	if (!gdwAllowHardcore)
	{
		if (gpCreateHcCheckBox) D2Win_10025_CONTROL_ToggleFirstFlag(&gpCreateHcCheckBox->controlHeader, 0);
		if (gpCreateHcLabelBox) D2Win_10025_CONTROL_ToggleFirstFlag(&gpCreateHcLabelBox->controlHeader, 0);
	}

	gpCreateOkBtn = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCreateOk));

	// 1.12a Z-order creation of Campfire heroes & fire (0x6FA55F4B..0x6FA56020)
	gpCampfireOrder[0] = reinterpret_cast<D2WinAnimImageStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlAnimBarb));
	gpCampfireOrder[3] = reinterpret_cast<D2WinAnimImageStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlAnimSorc));
	gpCampfireOrder[4] = reinterpret_cast<D2WinAnimImageStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlAnimPal));
	gpCampfireOrder[1] = reinterpret_cast<D2WinAnimImageStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlAnimNecro));
	if (bExpansion)
	{
		gpCampfireOrder[6] = reinterpret_cast<D2WinAnimImageStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlAnimAssa));
	}
	gpCampfireOrder[2] = reinterpret_cast<D2WinAnimImageStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlAnimAma));
	if (bExpansion)
	{
		gpCampfireOrder[5] = reinterpret_cast<D2WinAnimImageStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlAnimDruid));
	}
	D2LAUNCH_CreateScreenControl(bExpansion ? &gCtrlCreateFireExp : &gCtrlCreateFireCl);

	D2LAUNCH_UpdateCreateControlsVisibility();
	return TRUE;
}

// ============================================================================
// Main Entry & QueryInterface (0x6FA50AC0, 0x6FA50CC0, 0x6FA58730, 0x6FA5A420)
// ============================================================================
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
	if (!STRTABLE_Initialize(static_cast<int>(reinterpret_cast<intptr_t>(D2Win_GetArchive())), nullptr, FOG_IsExpansion()))
	{
		D2Win_10016();
		return D2LAUNCH_MODULE_NONE;
	}

	D2LAUNCH_LoadSprites(FALSE);
	CURSOR_SetCellFile(gpCursorCellFile);

	pCfg->szCurrentChannelName[0] = '\0';
	pCfg->szDefaultChannelName[0] = '\0';
	pCfg->szGameName[0] = '\0';
	pCfg->szGamePassword[0] = '\0';
	if (gnLaunchMode != 2)
	{
		pCfg->szServerIP[0] = '\0';
	}

	D2SOUND_FastInit();
	D2SOUND_FastCacheSound("data\\global\\sfx\\cursor\\button.wav");
	for (int i = 0; i < (FOG_IsExpansion() ? 7 : 5); ++i)
	{
		D2SOUND_FastCacheSound(kSelectClassWav[i]);
		D2SOUND_FastCacheSound(kDeselectClassWav[i]);
	}

	DWORD dwRegHc = 0;
	if (SRegLoadValue("Diablo II", "AllowHardcore", 0, &dwRegHc) && dwRegHc != 0)
	{
		gdwAllowHardcore = 1;
	}
#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 13
	gdwAllowHardcore = 1;
#endif

	D2LAUNCH_SyncClientModeEnv();

	// Headless / Automated Native Verification Fast-Path (D2MOO_NATIVE_AUTOTEST=1 / D2MOO_AUTO_CHAR)
	bool bAutoExitSession = false;
	char szAutoChar[32] = {};
	DWORD nAutoCharLen = GetEnvironmentVariableA("D2MOO_AUTO_CHAR", szAutoChar, sizeof(szAutoChar));
	if (nAutoCharLen == 0 || szAutoChar[0] == '\0')
	{
		char szAutotest[16] = {};
		const char* szCmdLine = GetCommandLineA();
		if ((GetEnvironmentVariableA("D2MOO_NATIVE_AUTOTEST", szAutotest, sizeof(szAutotest)) > 0 && szAutotest[0] == '1') ||
			(szCmdLine && std::strstr(szCmdLine, "--autotest-native") != nullptr))
		{
			SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
			SetEnvironmentVariableA("D2MOO_NATIVE_AUTOTEST", "1");
			SetEnvironmentVariableA("D2MOO_USE_NATIVE_CLIENT", "1");
			SetEnvironmentVariableA("D2MOO_USE_TS_CLIENT", "0");
			strcpy_s(szAutoChar, "ExpansionHero");
			nAutoCharLen = static_cast<DWORD>(std::strlen(szAutoChar));
		}
	}

	if (nAutoCharLen > 0 && szAutoChar[0] != '\0')
	{
		D2Win_10005(D2GAMERES_800x600);
		if (pCfg->bShownLogo)
		{
			gdwNextModule = D2LAUNCH_MODULE_NONE;
			bAutoExitSession = true;
		}
		else
		{
			pCfg->bShownLogo = 1;
			gnLaunchMode = 0;
			D2LAUNCH_EnumerateLocalSavedCharacters();
			D2LaunchCharNodeStrc* pMatch = nullptr;
			for (D2LaunchCharNodeStrc* pNode = D2LAUNCH_GetCharListHead(); pNode != nullptr; pNode = pNode->pNext)
			{
				if (_stricmp(pNode->szCharName, szAutoChar) == 0)
				{
					pMatch = pNode;
					break;
				}
			}

			if (pMatch)
			{
				pCfg->tCharPreviewInfo = pMatch->tPreview;
				pCfg->nArenaDifficulty = 0;
				D2LAUNCH_EnterSelectedCharacter(pMatch->szCharName, "", pMatch->nCharClass, pMatch->wSaveFlags);
			}
			else
			{
				char szAutoClass[16] = {};
				uint8_t nClass = 1;
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
				D2LAUNCH_SetConfigCharacterClass(nClass);
				D2LAUNCH_InitNewSaveAndPreview(&pCfg->tCharPreviewInfo, nClass, pCfg->szName, wSaveFlags);
				D2LAUNCH_WriteNewSaveFile(pCfg->szName);
				pCfg->nArenaDifficulty = 0;
				pCfg->nArenaFlags = (wSaveFlags & 0x20u) ? 0x100004u : 0x000004u;
				pCfg->nLaunchType = LAUNCHTYPE_LOCAL;
				pCfg->dwGameType = 0;
				gdwNextModule = D2LAUNCH_MODULE_CLIENT;
			}
			D2LAUNCH_FreeCharList();
		}
	}
	else if (pCfg->nLaunchType == LAUNCHTYPE_TCPIP)
	{
		gnLaunchMode = 2;
		D2Win_10005(D2GAMERES_800x600);
		D2LAUNCH_EnterTcpIpMenu();
	}
	else if (!pCfg->bShownLogo)
	{
		gnLaunchMode = 0;
		D2Win_10005(D2GAMERES_800x600);
		D2LAUNCH_EnterLogoScreen();
		pCfg->bShownLogo = 1;
	}
	else
	{
		gnLaunchMode = 0;
		D2Win_10005(D2GAMERES_800x600);
		D2LAUNCH_EnterMainMenu(0);
	}

	if (gdwNextModule == D2LAUNCH_MODULE_NONE && !bAutoExitSession)
	{
		D2Win_10010(D2LAUNCH_OnCloseWindow);
		D2Win_10008(D2LAUNCH_IdleCallback);
	}

	D2SOUND_FastFadeAll();
	while (D2SOUND_FastIsAnyPlaying())
	{
		Sleep(0);
	}
	D2SOUND_FastShutdown();
	D2LAUNCH_FreeCharList();
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_FreeSprites(FALSE);
	STRTABLE_Destroy();
	static_assert(sizeof(pCfg->_0082) >= 48, "D2ConfigStrc::_0082 must be at least 48 bytes so clearing the upper half never touches szName");
	std::memset(&pCfg->_0082[24], 0, 24);
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
