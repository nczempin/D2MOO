#pragma once

#include <D2BasicTypes.h>
#include <D2Config.h>
#include <D2Dll.h>
#include <D2Unicode.h>
#include <Font.h>
#include <cstdint>

struct D2CellFileStrc;
struct D2CompositeUnitStrc;
struct D2WinControlInitStrc;
struct D2WinControlStrc;
struct D2WinButtonStrc;
struct D2WinEditBoxStrc;
struct D2WinImageStrc;
struct D2WinTextBoxStrc;
struct SMSGHANDLER_PARAMS;

// 1.12a Image Base: 0x6FA40000
constexpr int D2LaunchImageBase = 0x6FA40000;

#pragma pack(push, 1)

enum D2LaunchModuleId : int32_t
{
	D2LAUNCH_MODULE_NONE        = 0,
	D2LAUNCH_MODULE_CLIENT      = 1,
	D2LAUNCH_MODULE_SERVER      = 2,
	D2LAUNCH_MODULE_MULTIPLAYER = 3,
	D2LAUNCH_MODULE_LAUNCHER    = 4,
	D2LAUNCH_MODULE_EXPAND      = 5,
};

struct D2LaunchInterfaceStrc
{
	int32_t(__fastcall* pfMainEntry)(D2ConfigStrc* pCfg);
};

// 1.12a Font+Color style descriptor (0x6FA60BD8..0x6FA60C18)
struct D2FontStyleStrc
{
	Font eFont;
	int32_t nColor;
};

// 1.12a TextBox scrollbar track descriptor (0x6FA60F2C)
struct D2LaunchScrollTrackStrc
{
	int32_t nX;
	int32_t nY;
	int32_t nHeight;
};

// 1.10+ (v96 = 0x60) 335-byte (0x14F) .d2s character save header at 0x6FA652D0
struct D2SaveHeaderStrc
{
	uint32_t dwHeaderMagic;                // 0x00 (0xAA55AA55)
	uint32_t dwVersion;                    // 0x04 (0x60 = 96 for 1.10+)
	uint32_t dwFileSize;                   // 0x08 (0x14F = 335 for newly created character)
	uint32_t dwChecksum;                   // 0x0C (FOG_ComputeChecksum)
	uint32_t dwWeaponSet;                  // 0x10
	char szName[16];                       // 0x14
	uint32_t dwSaveFlags;                  // 0x24 (0x01=NewChar, 0x04=Hardcore, 0x08=Dead, 0x20=Expansion, bits 8..12=Progression)
	uint8_t nCharClass;                    // 0x28 (D2C_PlayerClasses 0..6)
	uint8_t nStatsCount;                   // 0x29 (0x10)
	uint8_t nSkillsCount;                  // 0x2A (0x1E)
	uint8_t nCharLevel;                    // 0x2B (1..99)
	uint32_t dwCreateTime;                 // 0x2C
	uint32_t dwLastTime;                   // 0x30
	uint32_t dwPlayTime;                   // 0x34
	uint32_t dwSkillHotkeys[16];           // 0x38
	uint32_t dwLeftSkill1;                 // 0x78
	uint32_t dwRightSkill1;                // 0x7C
	uint32_t dwLeftSkill2;                 // 0x80
	uint32_t dwRightSkill2;                // 0x84
	uint8_t nAppearanceComponents[16];     // 0x88
	uint8_t nAppearanceColors[16];         // 0x98
	uint8_t nTownDifficulty[3];            // 0xA8
	uint32_t dwMapSeed;                    // 0xAB
	uint16_t wMercDead;                    // 0xAF
	uint32_t dwMercSeed;                   // 0xB1
	uint16_t wMercName;                    // 0xB5
	uint16_t wMercType;                    // 0xB7
	uint32_t dwMercExp;                    // 0xB9
	uint8_t aReserved[0x92];               // 0xBD..0x14E
};
static_assert(sizeof(D2SaveHeaderStrc) == 0x14F, "D2SaveHeaderStrc must be 0x14F (335) bytes");

// 1.12a Character Select node (0x6FA4E050, allocated in CharSel.cpp)
struct D2LaunchCharNodeStrc
{
	char szCharName[16];
	Unicode wszRealmName[64];
	D2CharacterPreviewInfoStrc tPreview;
	uint16_t wPreviewVersion;
	uint16_t wSaveFlags;
	uint16_t wGuildFlags;
	uint8_t nCharClass;
	uint8_t nCharLevel;
	uint8_t nAppearanceComponents[16];
	uint8_t nAppearanceColors[16];
	char szGuildTag[4];
	uint8_t nGuildEmblemBgColor;
	uint8_t nGuildEmblemFgColor;
	uint8_t nGuildEmblemType;
	uint8_t nPad;
	D2CompositeUnitStrc* pCompUnit;
	uint32_t dwLastWriteLow;
	uint32_t dwLastWriteHigh;
	int32_t nExpireSeconds;
	D2LaunchCharNodeStrc* pNext;
};

#pragma pack(pop)

// Shared 1.12a Font Styles (0x6FA60BD8..0x6FA60C18)
extern D2FontStyleStrc gFontStyle42;           // 0x6FA60BD8
extern D2FontStyleStrc gFontStyle30;           // 0x6FA60BE0
extern D2FontStyleStrc gFontStyle24;           // 0x6FA60BE8
extern D2FontStyleStrc gFontStyle16;           // 0x6FA60BF0
extern D2FontStyleStrc gFontStyle8Gold;        // 0x6FA60BF8
extern D2FontStyleStrc gFontStyleFormal10Red;  // 0x6FA60C00
extern D2FontStyleStrc gFontStyleFormal12;     // 0x6FA60C08
extern D2FontStyleStrc gFontStyleFormal10;     // 0x6FA60C10
extern D2FontStyleStrc gFontStyleFormal11;     // 0x6FA60C18

// Shared 1.12a Globals
extern D2ConfigStrc* gpD2Config;               // 0x6FA65ACC
extern int32_t gdwNextModule;                  // 0x6FA65AE0
extern int32_t gnLaunchMode;                   // 0x6FA65AE4 (0=SinglePlayer, 1=Realm, 2=TCPIP, 3=OpenBNet)
extern int32_t gbModalOpen;                    // 0x6FA65E94
extern int32_t gdwAllowHardcore;               // 0x6FA65E98
extern D2SaveHeaderStrc gD2SaveHeader;         // 0x6FA652D0

// Shared 1.12a CellFile Pointers (0x6FA65BF4..0x6FA65CD0)
extern D2CellFileStrc* gpBtnTallBlank;         // 0x6FA65C2C
extern D2CellFileStrc* gpBgCharSelectExp;      // 0x6FA65C30
extern D2CellFileStrc* gpBtnWideBlank;         // 0x6FA65C34
extern D2CellFileStrc* gpBtnMediumBlank;       // 0x6FA65C38
extern D2CellFileStrc* gpBtnShortBlank;        // 0x6FA65C3C
extern D2CellFileStrc* gpImgTextBox2;          // 0x6FA65C44
extern D2CellFileStrc* gpBgCharSelectClassic;  // 0x6FA65C64
extern D2CellFileStrc* gpDifficultyLevels;     // 0x6FA65C68
extern D2CellFileStrc* gpCharSelectBox;        // 0x6FA65C74
extern D2CellFileStrc* gpCharSelectBoxGrey;    // 0x6FA65C78
extern D2CellFileStrc* gpPopUpOKCancel;        // 0x6FA65C7C
extern D2CellFileStrc* gpPopUpOKCancel2;       // 0x6FA65C80
extern D2CellFileStrc* gpSelectCharArrows;     // 0x6FA65C84
extern D2CellFileStrc* gpCreateNewCharImage;   // 0x6FA65C90
extern D2CellFileStrc* gpPopUpOK;              // 0x6FA65C94
extern D2CellFileStrc* gpPopUpOK2;             // 0x6FA65CA8
extern D2CellFileStrc* gpBtnOkCancelBlank;     // 0x6FA65CAC
extern D2CellFileStrc* gpBtnCancelBlank;       // 0x6FA65CB0
extern D2CellFileStrc* gpJoinGameScrollBars;   // 0x6FA65CC8

// 1.12a @1: 0x6FA5A420
extern "C" D2LAUNCH_DLL_DECL D2LaunchInterfaceStrc* __stdcall QueryInterface();

// MainMenus.cpp Procedures (0x6FA4FC90..0x6FA5B350)
// 1.12a: 0x6FA4FC90
void __stdcall D2LAUNCH_LoadCampfireSprites();
// 1.12a: 0x6FA505C0
void __fastcall D2LAUNCH_SetConfigCharacterClass(uint8_t nCharClass);
// 1.12a: 0x6FA50A50
BOOL __stdcall D2LAUNCH_HasSavedCharacters();
// 1.12a: 0x6FA51590
D2WinControlStrc* __fastcall D2LAUNCH_RegisterScreenControl(D2WinControlStrc* pCtrl);
D2WinControlStrc* __fastcall D2LAUNCH_CreateScreenControl(D2WinControlInitStrc* pInit);
// 1.12a: 0x6FA515E0
int __stdcall D2LAUNCH_DestroyModalControls();
D2WinControlStrc* __fastcall D2LAUNCH_CreateModalControl(D2WinControlInitStrc* pInit);
// 1.12a: 0x6FA51670
void __fastcall D2LAUNCH_SetPalette(const char* szDatRelPath, const char* szPl2RelPath);
// 1.12a: 0x6FA51700
void __fastcall D2LAUNCH_FreeSprites(BOOL bReload);
// 1.12a: 0x6FA51A90
void __fastcall D2LAUNCH_LoadSprites(BOOL bReload);
// 1.12a: 0x6FA540B0 / 0x6FA541B0
void __stdcall D2LAUNCH_ShowModalAlert(const Unicode* wszMessage, void(__stdcall* pfOnOk)(int32_t), int32_t nColor);
// 1.12a: 0x6FA54830
void __fastcall D2LAUNCH_DestroyAllControls();
// 1.12a: 0x6FA558E0
int __stdcall D2LAUNCH_EnterOtherMultiplayer(SMSGHANDLER_PARAMS* pMsg);
// 1.12a: 0x6FA55980
BOOL __stdcall D2LAUNCH_EnterCharCreate();
// 1.12a: 0x6FA56630
void __stdcall D2LAUNCH_EnterTcpIpMenu();
// 1.12a: 0x6FA568C0
void __stdcall D2LAUNCH_EnterMainMenu(int32_t nUnused);
// 1.12a: 0x6FA56BF0
void __stdcall D2LAUNCH_ShowTrademarkScreen();
// 1.12a: 0x6FA57380
int __stdcall D2LAUNCH_EnterCinematicsScreen(SMSGHANDLER_PARAMS* pMsg);
// 1.12a: 0x6FA57690
int __stdcall D2LAUNCH_EnterCreditsScreen(SMSGHANDLER_PARAMS* pMsg);
// 1.12a: 0x6FA58150
void __stdcall D2LAUNCH_EnterLogoScreen();
// 1.12a: 0x6FA58730
int32_t __fastcall D2LAUNCH_MainEntry(D2ConfigStrc* pCfg);
// 1.12a: 0x6FA594F0
BOOL __stdcall D2LAUNCH_EnterSelectedCharacter(const char* szCharName, const char* szRealmName, uint8_t nCharClass, uint16_t wSaveFlags);
// 1.12a: 0x6FA59D50
BOOL __fastcall D2LAUNCH_ConvertSaveToExpansion(const char* szCharName);
// 1.12a: 0x6FA59EB0
BOOL __fastcall D2LAUNCH_ReadSavePreview(D2CharacterPreviewInfoStrc* pPreviewOut, const char* szCharName);
// 1.12a: 0x6FA5A0C0
BOOL __fastcall D2LAUNCH_DeleteSaveFiles(const char* szCharName);
// 1.12a: 0x6FA5A210
BOOL __stdcall D2LAUNCH_WriteNewSaveFile(const char* szCharName);
// 1.12a: 0x6FA5A2D0
void __fastcall D2LAUNCH_InitNewSaveAndPreview(D2CharacterPreviewInfoStrc* pPreviewInfo, uint8_t nCharClass, const char* szCharName, uint16_t wSaveFlags);
// Dev helper (opt-in via D2MOO_DEV_CLIENT_TOGGLE=1)
void D2LAUNCH_SyncClientModeEnv();
void D2LAUNCH_MaybeCreateDevClientToggle(int32_t nX, int32_t nY);

// CharSel.cpp Procedures (0x6FA4B540..0x6FA4FBF0)
// 1.12a: 0x6FA4B7B0
const Unicode* __fastcall D2LAUNCH_GetClassDisplayName(uint8_t nCharClass);
// 1.12a: 0x6FA4C7D0
void __stdcall D2LAUNCH_FreeCharList();
// 1.12a: 0x6FA4E050
BOOL __stdcall D2LAUNCH_AddCharNode(
	const char* szCharName,
	const char* szRealmTitle,
	const D2CharacterPreviewInfoStrc* pPreview,
	int32_t nExpireSeconds,
	uint32_t dwLastWriteLow,
	uint32_t dwLastWriteHigh);
// 1.12a: 0x6FA4E5D0
BOOL __stdcall D2LAUNCH_EnumerateLocalSavedCharacters();
// 1.12a: 0x6FA4EAE0
BOOL __stdcall D2LAUNCH_EnterCharSelectExpansion();
// 1.12a: 0x6FA4EF60
BOOL __stdcall D2LAUNCH_EnterCharSelect();
D2LaunchCharNodeStrc* D2LAUNCH_GetCharListHead();
