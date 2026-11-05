#pragma once

#include <D2BasicTypes.h>
#include <D2Config.h>
#include <D2Dll.h>
#include <cstdint>

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

// 1.10+ (v96 = 0x60) 335-byte (0x14F) .d2s character save header at 0x6FA652D0
struct D2SaveHeaderStrc
{
	uint32_t dwHeaderMagic;                // 0x00 (0xAA55AA55)
	uint32_t dwVersion;                    // 0x04 (0x60 = 96 for 1.10+)
	uint32_t dwFileSize;                   // 0x08 (0x14F = 335 for newly created character)
	uint32_t dwChecksum;                   // 0x0C (FOG_ComputeChecksum)
	uint32_t dwWeaponSet;                  // 0x10
	char szName[16];                       // 0x14
	uint32_t dwSaveFlags;                  // 0x24 (0x01=NewChar, 0x04=Hardcore, 0x20=Expansion)
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

struct D2LaunchCharNodeStrc
{
	char szCharName[16];
	D2CharacterPreviewInfoStrc tPreview;
	uint16_t wSaveFlags;
	uint8_t nCharClass;
	uint8_t nCharLevel;
	D2LaunchCharNodeStrc* pNext;
};

#pragma pack(pop)

// 1.12a @1: 0x6FA5A420
extern "C" D2LAUNCH_DLL_DECL D2LaunchInterfaceStrc* __stdcall QueryInterface();

// 1.12a: 0x6FA58730
int32_t __fastcall D2LAUNCH_MainEntry(D2ConfigStrc* pCfg);
// 1.12a: 0x6FA51A90
void __fastcall D2LAUNCH_LoadSprites(BOOL bReload);
// 1.12a: 0x6FA51700
void __fastcall D2LAUNCH_FreeSprites(BOOL bReload);
// 1.12a: 0x6FA54830
void __fastcall D2LAUNCH_DestroyAllControls();
// 1.12a: 0x6FA50A50
BOOL __stdcall D2LAUNCH_HasSavedCharacters();
// 1.12a: 0x6FA5A2D0
void __fastcall D2LAUNCH_InitNewSaveAndPreview(D2CharacterPreviewInfoStrc* pPreviewInfo, uint8_t nCharClass, const char* szCharName, uint16_t wSaveFlags);
// 1.12a: 0x6FA5A210
BOOL __stdcall D2LAUNCH_WriteNewSaveFile(const char* szCharName);
// 1.12a: 0x6FA58150
void __stdcall D2LAUNCH_EnterLogoScreen();
// 1.12a: 0x6FA568C0
void __stdcall D2LAUNCH_EnterMainMenu(int32_t nUnused);
// 1.12a: 0x6FA4EAE0
BOOL __stdcall D2LAUNCH_EnterCharSelect();
// 1.12a: 0x6FA55980
BOOL __stdcall D2LAUNCH_EnterCharCreate();
