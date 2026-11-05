#include "D2WinComp.h"

#include <algorithm>
#include <cstring>

#include <D2BitManip.h>
#include <D2Math.h>

#include <Fog.h>
#include <D2CMP.h>
#include <Archive.h>
#include <D2Lang.h>

#include <D2Gfx.h>
#include <Texture.h>

#include "D2WinArchive.h"
#include "D2WinControlHeader.h"
#include "D2WinFont.h"
#include "D2WinMain.h"
#include "D2WinPalette.h"


#pragma pack(push, 1)
struct D2ItemTypesTxt
{
	uint32_t dwCode;								//0x00
	int16_t nEquiv1;								//0x04
	int16_t nEquiv2;								//0x06
	uint8_t pad0x08[0xDC];							//0x08
};
static_assert(sizeof(D2ItemTypesTxt) == 0xE4, "D2ItemTypesTxt size must be 0xE4 (228) bytes");
#pragma pack(pop)

struct D2CompItemTableEntryStrc
{
	uint32_t dwItemCode;
	int32_t nWClass;
	int32_t nItemType;
};

struct D2CompWClassMapStrc
{
	uint32_t dwCode;
	int32_t nIndex;
};


D2CompositeUnitStrc* dword_6F8FD650;
D2CofNodeStrc* dword_6F8FD654;

uint8_t byte_6F8FD218[10];
uint8_t byte_6F8FD224[10];

char byte_6F8FD678[4];

D2CellFileStrc* ghSquelchCelFile_6F8FD65C;
int gnSquelchCelFileFrameCount_6F8FD660;
int dword_6F8FD664;
D2CellFileStrc* ghSelectedCelFile_6F8FD668;
int gnSelectedCelFileFrameCount_6F8FD66C;
int dword_6F8FD670;
uint32_t dword_6F8FD658;
int dword_6F8FD674;

int dword_6F8BCF94 = 1;
int dword_6F8FD230 = 1;
void* dword_6F8FD234;
void* dword_6F8FD238;
void* dword_6F8FD23C;
void* dword_6F8FD240;
D2ItemTypesTxt* gpItemTypesTxt_6F8FD244;
int unk_6F8FD248;

int dword_6F8C14F8;
uint8_t* dword_6F8C14FC;
uint8_t* dword_6F8C1500;
int dword_6F8C1504;
uint8_t* dword_6F8C1508;
int dword_6F8C150C;
uint8_t* dword_6F8C1510;
int dword_6F8C1514;
uint16_t* dword_6F8C1518;

D2CompItemTableEntryStrc stru_6F8C1520[255];
int32_t dword_6F8FD250[255];
uint8_t gPaletteShiftTables_6F8C2118[9][26880];

uint32_t dword_6F8BC040[25] =
{
	'  MA', '  OS', '  EN', '  AP', '  AB', '  ZD', '  IA', '  OR',
	'  HR', '  HR', '  1O', '  2O', '  3O', '  4O', '  DM', '  9O',
	'  YO', '  YO', '  5O', '  7O', '  8O', '  6O', '  0O', '  CT',
	'  OR',
};
uint32_t dword_6F8BC0A4 = 25;

uint32_t dword_6F8BC0A8[20] =
{
	'  TD', '  UN', '  LW', '  NR', '  HG', '  NT', '  WT', '  1A',
	'  2A', '  LB', '  CS', '  HT', '  KK', '  1S', '  2S', '  3S',
	'  4S', '  DD', '  HG', '  HG',
};
int gnPlayerModeTokenTblSize_6F8BC0F8 = 20;

uint32_t dword_6F8BC100[16] =
{
	'  DH', '  RT', '  GL', '  AR', '  AL', '  HR', '  HL', '  HS',
	'  1S', '  2S', '  3S', '  4S', '  5S', '  6S', '  7S', '  8S',
};

int dword_6F8BC154[15] =
{
	0,
	' hth',
	' th1',
	' th2',
	' sh1',
	' sh2',
	' wob',
	' wbx',
	' fts',
	' sj1',
	' tj1',
	' ss1',
	' ts1',
	' 1th',
	' 2th',
};
int dword_6F8BC190 = 15;

const D2CompWClassMapStrc stru_6F8BCF28[13] =
{
	{ ' wob', 1 },
	{ ' sh1', 2 },
	{ ' th1', 3 },
	{ ' fts', 4 },
	{ ' sh2', 5 },
	{ ' th2', 6 },
	{ ' wbx', 7 },
	{ ' sj1', 8 },
	{ ' tj1', 9 },
	{ ' ss1', 10 },
	{ ' ts1', 11 },
	{ ' 1th', 12 },
	{ ' 2th', 13 },
};
const int dword_6F8BCF90 = 13;

const int32_t dword_6F8BCF98[14] =
{
	1, 6, 4, 2, 8, 5, 3, 7, 9, 10, 11, 12, 13, 14,
};

const uint8_t byte_6F8BA370[7][64] =
{
	{ 0 },
	{ 0 },
	{ 0, 32 },
	{ 0, 16, 32, 48 },
	{ 0, 8, 16, 24, 32, 40, 48, 56 },
	{ 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60 },
	{ 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62 },
};

const D2CompItemTableEntryStrc stru_6F8BC1D8[255] =
{
	{ 0x00000000, 0, 1 }, { 0x2074696C, 0, 1 }, { 0x2064656D, 0, 1 }, { 0x20797668, 0, 1 },
	{ 0x20706163, 0, 37 }, { 0x20706B73, 0, 37 }, { 0x206D6C68, 0, 37 }, { 0x206C6866, 0, 37 },
	{ 0x206D6867, 0, 37 }, { 0x206E7263, 0, 37 }, { 0x206B736D, 0, 37 }, { 0x20657063, 0, 3 },
	{ 0x206B6C63, 0, 3 }, { 0x20626F72, 0, 3 }, { 0x20697571, 0, 3 }, { 0x2061656C, 0, 3 },
	{ 0x20616C68, 0, 3 }, { 0x20757473, 0, 3 }, { 0x20676E72, 0, 3 }, { 0x206C6373, 0, 3 },
	{ 0x206E6863, 0, 3 }, { 0x20737262, 0, 3 }, { 0x206C7073, 0, 3 }, { 0x20746C70, 0, 3 },
	{ 0x20646C66, 0, 3 }, { 0x20687467, 0, 3 }, { 0x206C7566, 0, 3 }, { 0x20637562, 0, 2 },
	{ 0x2067726C, 0, 2 }, { 0x2074696B, 0, 2 }, { 0x20776F74, 0, 2 }, { 0x206C676C, 0, 16 },
	{ 0x206C676D, 0, 16 }, { 0x206C6768, 0, 16 }, { 0x2074626C, 0, 15 }, { 0x2074626D, 0, 15 },
	{ 0x20746268, 0, 15 }, { 0x206C626C, 0, 19 }, { 0x206C626D, 0, 19 }, { 0x206C6268, 0, 19 },
	{ 0x206D6862, 0, 37 }, { 0x20687362, 0, 2 }, { 0x206B7073, 0, 2 }, { 0x20646E77, 4, 25 },
	{ 0x206E7779, 4, 25 }, { 0x206E7762, 4, 25 }, { 0x20626C63, 4, 29 }, { 0x20786168, 4, 43 },
	{ 0x20647373, 4, 30 }, { 0x206D6373, 4, 30 }, { 0x2063616D, 4, 36 }, { 0x20657861, 4, 28 },
	{ 0x20636C66, 4, 30 }, { 0x2064736C, 4, 30 }, { 0x20616C66, 4, 36 }, { 0x206D6468, 4, 31 },
	{ 0x206D6877, 4, 31 }, { 0x20737263, 4, 30 }, { 0x20736C66, 4, 30 }, { 0x206C7067, 2, 38 },
	{ 0x20737067, 2, 38 }, { 0x206C706F, 2, 38 }, { 0x2073706F, 2, 38 }, { 0x20726764, 2, 32 },
	{ 0x20333364, 2, 32 }, { 0x20333367, 2, 32 }, { 0x20726964, 2, 32 }, { 0x2076616A, 2, 33 },
	{ 0x20766C67, 2, 33 }, { 0x206C6970, 2, 33 }, { 0x206D6C63, 5, 30 }, { 0x20647367, 5, 30 },
	{ 0x20727073, 3, 33 }, { 0x20697274, 3, 33 }, { 0x206B6970, 3, 33 }, { 0x20727073, 3, 33 },
	{ 0x20776273, 6, 27 }, { 0x2077626C, 6, 27 }, { 0x20626273, 6, 27 }, { 0x2062626C, 6, 27 },
	{ 0x20747373, 8, 26 }, { 0x2074736C, 8, 26 }, { 0x20747363, 8, 26 }, { 0x20747362, 8, 26 },
	{ 0x2078616C, 8, 28 }, { 0x20796373, 8, 34 }, { 0x20786967, 8, 28 }, { 0x20787462, 8, 28 },
	{ 0x206C6168, 8, 34 }, { 0x2075616D, 8, 31 }, { 0x20786170, 8, 34 }, { 0x206C6168, 8, 34 },
	{ 0x20627868, 7, 35 }, { 0x2062786C, 7, 35 }, { 0x20786174, 4, 43 }, { 0x20637073, 4, 29 },
	{ 0x20726273, 4, 30 }, { 0x2074736D, 4, 36 }, { 0x20706373, 4, 24 }, { 0x206D7067, 2, 38 },
	{ 0x206D706F, 2, 38 }, { 0x20666B74, 2, 42 }, { 0x20646C62, 2, 32 }, { 0x20647362, 4, 30 },
	{ 0x20777362, 4, 30 }, { 0x20736832, 5, 30 }, { 0x206E7262, 3, 33 }, { 0x20776268, 6, 27 },
	{ 0x20776263, 6, 27 }, { 0x20627773, 6, 27 }, { 0x2062776C, 6, 27 }, { 0x20747377, 8, 26 },
	{ 0x20786162, 8, 28 }, { 0x20726162, 8, 34 }, { 0x20786167, 8, 28 }, { 0x20627872, 7, 35 },
	{ 0x2062786D, 7, 35 }, { 0x2074627A, 0, 1 }, { 0x20726161, 0, 3 }, { 0x2070746C, 0, 3 },
	{ 0x206C6D73, 0, 2 }, { 0x20737467, 0, 3 }, { 0x20767161, 0, 40 }, { 0x20767163, 0, 40 },
	{ 0x206C6276, 0, 19 }, { 0x206C6274, 0, 19 }, { 0x206C6776, 0, 16 }, { 0x206C6774, 0, 16 },
	{ 0x20746276, 0, 15 }, { 0x20746274, 0, 15 }, { 0x20626C66, 5, 30 }, { 0x2069706D, 4, 28 },
	{ 0x206C6162, 4, 43 }, { 0x2067656C, 4, 29 }, { 0x20746C71, 0, 3 }, { 0x20786132, 4, 28 },
	{ 0x20786177, 4, 28 }, { 0x206E7767, 4, 36 }, { 0x20616D67, 8, 36 }, { 0x20647377, 4, 30 },
	{ 0x20736967, 5, 30 }, { 0x20646C62, 2, 32 }, { 0x20666B62, 2, 32 }, { 0x20707373, 2, 33 },
	{ 0x20707374, 2, 33 }, { 0x20657073, 3, 33 }, { 0x20756F76, 8, 34 }, { 0x20637377, 8, 34 },
	{ 0x2069726B, 2, 32 }, { 0x20747073, 2, 33 }, { 0x20637367, 2, 24 }, { 0x20707377, 2, 24 },
	{ 0x20747368, 3, 26 }, { 0x2066736D, 3, 26 }, { 0x20686668, 4, 36 }, { 0x20616839, 4, 28 },
	{ 0x20786139, 4, 28 }, { 0x20613239, 4, 28 }, { 0x20706D39, 4, 28 }, { 0x20617739, 4, 28 },
	{ 0x20616C39, 8, 28 }, { 0x20616239, 8, 28 }, { 0x20746239, 8, 28 }, { 0x20616739, 8, 28 },
	{ 0x20696739, 8, 28 }, { 0x206E7739, 4, 25 }, { 0x20777939, 4, 25 }, { 0x20776239, 4, 25 },
	{ 0x20776739, 4, 25 }, { 0x206C6339, 4, 36 }, { 0x20637339, 4, 24 }, { 0x20737139, 4, 24 },
	{ 0x20737739, 4, 24 }, { 0x20707339, 4, 36 }, { 0x20616D39, 4, 36 }, { 0x20746D39, 4, 36 },
	{ 0x206C6639, 4, 36 }, { 0x20687739, 4, 36 }, { 0x20396D39, 8, 36 }, { 0x206D6739, 8, 36 },
	{ 0x20737339, 4, 30 }, { 0x206D7339, 4, 30 }, { 0x20627339, 4, 30 }, { 0x20636639, 4, 30 },
	{ 0x20726339, 4, 30 }, { 0x20736239, 4, 30 }, { 0x20736C39, 4, 30 }, { 0x20647739, 4, 30 },
	{ 0x20683239, 4, 30 }, { 0x206D6339, 4, 30 }, { 0x20736739, 4, 30 }, { 0x20396239, 4, 30 },
	{ 0x20626639, 4, 30 }, { 0x20646739, 4, 30 }, { 0x20676439, 2, 32 }, { 0x20696439, 2, 32 },
	{ 0x20726B39, 2, 32 }, { 0x206C6239, 2, 32 }, { 0x206B7439, 2, 42 }, { 0x20617439, 4, 43 },
	{ 0x206B6239, 2, 42 }, { 0x20386239, 4, 43 }, { 0x20616A39, 2, 33 }, { 0x20697039, 2, 33 },
	{ 0x20397339, 2, 33 }, { 0x206C6739, 2, 33 }, { 0x20737439, 2, 33 }, { 0x20727339, 3, 33 },
	{ 0x20727439, 3, 33 }, { 0x20726239, 3, 33 }, { 0x20747339, 3, 33 }, { 0x20397039, 3, 33 },
	{ 0x20376239, 8, 34 }, { 0x206F7639, 8, 34 }, { 0x20387339, 8, 34 }, { 0x20617039, 8, 34 },
	{ 0x20396839, 8, 34 }, { 0x20637739, 8, 34 }, { 0x20737338, 8, 26 }, { 0x20736C38, 8, 26 },
	{ 0x20736338, 8, 26 }, { 0x20736238, 8, 26 }, { 0x20737738, 8, 26 }, { 0x20627338, 6, 27 },
	{ 0x20626838, 6, 27 }, { 0x20626C38, 6, 27 }, { 0x20626338, 6, 27 }, { 0x20387338, 6, 27 },
	{ 0x20386C38, 6, 27 }, { 0x20777338, 6, 27 }, { 0x20776C38, 6, 27 }, { 0x20786C38, 7, 35 },
	{ 0x20786D38, 7, 35 }, { 0x20786838, 7, 35 }, { 0x20787238, 7, 35 }, { 0x20706178, 0, 37 },
	{ 0x20706B78, 0, 37 }, { 0x206D6C78, 0, 37 }, { 0x206C6878, 0, 37 }, { 0x206D6878, 0, 37 },
	{ 0x206E7278, 0, 37 }, { 0x206B7378, 0, 37 }, { 0x20697578, 0, 3 }, { 0x20616578, 0, 3 },
	{ 0x20616C78, 0, 3 }, { 0x20757478, 0, 3 }, { 0x20676E78, 0, 3 }, { 0x206C6378, 0, 3 },
	{ 0x206E6878, 0, 3 }, { 0x20737278, 0, 3 }, { 0x206C7078, 0, 3 }, { 0x20746C78, 0, 3 },
	{ 0x20646C78, 0, 3 }, { 0x20687478, 0, 3 }, { 0x206C7578, 0, 3 },
};


D2CofDataStrc* __fastcall sub_6F8A1250(D2GfxInfoStrc* pGfxInfo, unsigned int nClass, unsigned int nMode, int* a4, int* a5, int a6);

//D2Win.0x6F8A1000
void __fastcall sub_6F8A1000(D2CompositeUnitStrc* pCompositeUnit, D2GfxDataStrc* pGfxData)
{
	pGfxData->nUnitIndex = pCompositeUnit->nUnitIndex;
	pGfxData->nMode = pCompositeUnit->nMode;

	const int nComponent = pGfxData->nComponent;
	pGfxData->dwName[1] = dword_6F8BC100[nComponent];
	pGfxData->dwName[4] = dword_6F8BC154[pCompositeUnit->nWClassTokenIndex];
	pGfxData->dwName[0] = dword_6F8BC040[pCompositeUnit->nUnitIndex];
	if (pCompositeUnit->dwArmorType[nComponent])
	{
		pGfxData->dwName[2] = pCompositeUnit->dwArmorType[nComponent];
	}
	else
	{
		pGfxData->dwName[2] = ' til';
	}

	D2GfxInfoStrc* pGfxInfo = pCompositeUnit->pGfxInfo;
	D2_ASSERT(pGfxInfo);

	int v19 = 0;
	int v20 = 0;
	D2CofDataStrc* pCofData = sub_6F8A1250(pGfxInfo, pCompositeUnit->nUnitIndex, pCompositeUnit->nMode, &v20, &v19, dword_6F8BC154[pCompositeUnit->nWClassTokenIndex]);
	D2_ASSERT(pCofData);

	const uint8_t* pLayer = reinterpret_cast<const uint8_t*>(&pCofData[1]);
	for (int i = 0; i < pCofData->unk0x00; ++i)
	{
		D2_ASSERT(nComponent < 16);

		if (pLayer[0] == nComponent)
		{
			pGfxData->nUnitType = pGfxData->nUnitIndex >= 7;
			pGfxData->dwName[3] = dword_6F8BC0A8[pGfxData->nMode];

			if (!pGfxData->dwName[2])
			{
				FOG_DisplayAssert("[sGetCelContextInfo] This should not happen.\n", __FILE__, __LINE__);
				exit(-1);
			}

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
			memcpy(&pGfxData->dwName[4], szWClass, sizeof(uint32_t));
			break;
		}

		pLayer += 9;
	}

	D2_ASSERT(pGfxData->dwName[1]);
	D2_ASSERT(pGfxData->dwName[0]);
	D2_ASSERT(pGfxData->dwName[4]);

	if (!pGfxData->dwName[2])
	{
		pGfxData->dwName[2] = ' xxx';
	}

	if (!pGfxData->dwName[3])
	{
		pGfxData->dwName[3] = ' xxx';
	}
}

//D2Win.0x6F8A1250
D2CofDataStrc* __fastcall sub_6F8A1250(D2GfxInfoStrc* pGfxInfo, unsigned int nClass, unsigned int nMode, int* a4, int* a5, int a6)
{
	(void)a4;
	(void)a5;
	D2_ASSERT(pGfxInfo);

	D2CofInfoStrc* pCofInfo = pGfxInfo->pCofInfo;
	while (pCofInfo)
	{
		if (pCofInfo->pCofNode->nMode == static_cast<int>(nMode) && pCofInfo->pCofNode->unk0x0C == a6)
		{
			break;
		}
		pCofInfo = pCofInfo->pNext;
	}

	if (!pCofInfo)
	{
		char szPath[260] = {};
		D2_ASSERT(nClass < 25);
		D2_ASSERT(nMode < static_cast<unsigned int>(gnPlayerModeTokenTblSize_6F8BC0F8));

		const uint32_t dwClassToken = dword_6F8BC040[nClass];
		const uint32_t dwModeToken = dword_6F8BC0A8[nMode];

		auto TrimSpaces = [](uint32_t dwToken, char(&szOut)[5])
		{
			const char* pBytes = reinterpret_cast<const char*>(&dwToken);
			szOut[0] = (pBytes[0] == ' ') ? '\0' : pBytes[0];
			szOut[1] = (pBytes[1] == ' ') ? '\0' : pBytes[1];
			szOut[2] = (pBytes[2] == ' ') ? '\0' : pBytes[2];
			szOut[3] = '\0';
			szOut[4] = '\0';
		};

		char szClass[5] = {};
		char szMode[5] = {};
		char szWClass[5] = {};
		TrimSpaces(dwClassToken, szClass);
		TrimSpaces(dwModeToken, szMode);
		TrimSpaces(static_cast<uint32_t>(a6), szWClass);

		const char* szBaseDir = (nClass >= 7) ? "DATA\\GLOBAL\\MONSTERS" : "DATA\\GLOBAL\\CHARS";
		wsprintfA(szPath, "%s\\%s\\COF\\%s%s%s.COF", szBaseDir, szClass, szClass, szMode, szWClass);

		HSFILE hFile = nullptr;
		if (!FOG_FOpenFile(szPath, &hFile))
		{
			return nullptr;
		}
		FOG_FCloseFile(hFile);
		if (!szPath[0])
		{
			return nullptr;
		}

		pCofInfo = D2_CALLOC_STRC(D2CofInfoStrc);
		D2_ASSERT(pCofInfo);

		D2CofNodeStrc* pCofNode = dword_6F8FD654;
		while (pCofNode)
		{
			if (pCofNode->nMode == static_cast<uint32_t>(nMode) && pCofNode->unk0x08 == static_cast<uint32_t>(nClass) && pCofNode->unk0x0C == static_cast<uint32_t>(a6))
			{
				++pCofNode->unk0x00;
				break;
			}
			pCofNode = pCofNode->unk0x18;
		}

		if (!pCofNode)
		{
			pCofNode = D2_CALLOC_STRC(D2CofNodeStrc);
			D2_ASSERT(pCofNode);

			pCofNode->unk0x00 = 1;
			pCofNode->nMode = static_cast<uint32_t>(nMode);
			pCofNode->unk0x08 = static_cast<uint32_t>(nClass);
			pCofNode->unk0x0C = static_cast<uint32_t>(a6);
			pCofNode->unk0x18 = dword_6F8FD654;
			pCofNode->unk0x1C = nullptr;
			if (dword_6F8FD654)
			{
				dword_6F8FD654->unk0x1C = pCofNode;
			}
			dword_6F8FD654 = pCofNode;

			unsigned int nFileSize = 0;
			D2CofDataStrc* pCofData = static_cast<D2CofDataStrc*>(ARCHIVE_ALLOC_BUFFER_AND_READ_FILE_TO_IT(D2Win_GetArchive(), szPath, &nFileSize));
			D2_ASSERT(pCofData);
			D2_ASSERT(pCofData->unk0x03 == 20);
			pCofNode->unk0x10 = static_cast<uint32_t>(nFileSize);
			pCofNode->pCofData = pCofData;
		}

		D2_ASSERT(pCofNode);
		pCofInfo->pCofNode = pCofNode;
		pCofInfo->pNext = pGfxInfo->pCofInfo;
		pGfxInfo->pCofInfo = pCofInfo;
	}

	D2_ASSERT(pCofInfo->pCofNode->pCofData);
	return pCofInfo->pCofNode->pCofData;
}

//D2Win.0x6F8A15E0
void __fastcall sub_6F8A15E0(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	if (!(pCompositeUnit->dwFlags & gdwBitMasks[0]) || !(pCompositeUnit->dwFlags & gdwBitMasks[2]))
	{
		return;
	}

	const int32_t nXpos = pCompositeUnit->nX - 2;
	const int32_t nYpos = pCompositeUnit->nY;
	D2GfxInfoStrc* pGfxInfo = pCompositeUnit->pGfxInfo;
	D2_ASSERT(pGfxInfo);

	D2CofInfoStrc* pCofInfo = pGfxInfo->pCofInfo;
	if (!pCofInfo)
	{
		return;
	}

	while (pCofInfo->pCofNode->nMode != static_cast<uint32_t>(pCompositeUnit->nMode)
		|| pCofInfo->pCofNode->unk0x0C != static_cast<uint32_t>(dword_6F8BC154[pCompositeUnit->nWClassTokenIndex]))
	{
		pCofInfo = pCofInfo->pNext;
		if (!pCofInfo)
		{
			return;
		}
	}

	D2CofDataStrc* pCofData = pCofInfo->pCofNode->pCofData;
	D2_ASSERT(pCofData);

	const int nFramesPerDir = pCofData->unk0x01;
	const int nLayers = pCofData->unk0x00;
	const uint32_t nFrame = pCompositeUnit->unk0x08 >> 8;
	const uint8_t* pOrderTable = reinterpret_cast<const uint8_t*>(pCofData) + 28 + nFramesPerDir + nLayers * (pCompositeUnit->unk0x04 * nFramesPerDir + nFrame + 9);
	D2_ASSERT(pOrderTable != nullptr);

	if (nLayers <= 0)
	{
		return;
	}

	int32_t nMaxOffsetY = 0;
	int32_t nValidCount = 0;
	const uint8_t nDir = pCompositeUnit->unk0x04;
	for (int i = 0; i < nLayers; ++i)
	{
		const int nComponent = pOrderTable[i];
		D2_ASSERT(nComponent < 16);

		D2GfxDataStrc gfxData = {};
		gfxData.pCellFile = nullptr;
		gfxData.nComponent = static_cast<uint8_t>(nComponent);
		gfxData.nDirection = nDir;
		gfxData.nFrame = nFrame;
		sub_6F8A1000(pCompositeUnit, &gfxData);
		gfxData.fFlags |= 2u;
		if (D2CMP_SpriteValidate(&gfxData, 0, TRUE) && gfxData.pCurrentCell)
		{
			const int32_t nTestY = pCompositeUnit->nX + D2CMP_CelGetOffsetY(reinterpret_cast<D2CellFileStrc*>(gfxData.pCurrentCell));
			if (nTestY > nMaxOffsetY)
			{
				nMaxOffsetY = nTestY;
			}
			++nValidCount;
		}
	}

	if (nValidCount > 0)
	{
		for (int i = 0; i < nLayers; ++i)
		{
			const int nComponent = pOrderTable[i];
			D2_ASSERT(nComponent < 16);

			D2GfxDataStrc gfxData = {};
			gfxData.nComponent = static_cast<uint8_t>(nComponent);
			gfxData.nDirection = pCompositeUnit->unk0x04;
			gfxData.pCellFile = nullptr;
			gfxData.nFrame = nFrame;
			sub_6F8A1000(pCompositeUnit, &gfxData);
			gfxData.fFlags |= 2u;
			TEXTURE_CelDrawShadow(&gfxData, nXpos, nYpos);
		}
	}
}

//D2Win.0x6F8A1890
BOOL __fastcall sub_6F8A1890(D2CompositeUnitStrc* pCompositeUnit, DWORD dwGamma)
{
	D2_ASSERT(pCompositeUnit);

	if (!(pCompositeUnit->dwFlags & gdwBitMasks[0]))
	{
		return FALSE;
	}

	if (pCompositeUnit->dwFlags & gdwBitMasks[10])
	{
		D2_ASSERT(ghSelectedCelFile_6F8FD668);

		D2GfxDataStrc gfxData = {};
		gfxData.pCellFile = ghSelectedCelFile_6F8FD668;
		gfxData.nFrame = dword_6F8FD670;
		gfxData.nDirection = 0;
		TEXTURE_CelDraw(&gfxData, pCompositeUnit->nX + 5, pCompositeUnit->nY - 5, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
	}

	if (pCompositeUnit->pEmblemCellFile)
	{
		uint8_t pPalette[256] = {};
		pPalette[1] = pCompositeUnit->unk0xC0;
		pPalette[2] = pCompositeUnit->unk0xC1;

		D2GfxDataStrc gfxData = {};
		gfxData.pCellFile = pCompositeUnit->pEmblemCellFile;
		gfxData.nDirection = 0;
		gfxData.nFrame = 0;
		gfxData.nMaxDirections = 1;
		TEXTURE_CelDraw(&gfxData, pCompositeUnit->nX - 20, pCompositeUnit->nY - 60, 0xFFFFFFFFu, DRAWMODE_TRANSHIGHLIGHT, pPalette);
	}

	D2GfxInfoStrc* pGfxInfo = pCompositeUnit->pGfxInfo;
	D2_ASSERT(pGfxInfo);

	D2CofInfoStrc* pCofInfo = pGfxInfo->pCofInfo;
	if (!pCofInfo)
	{
		return FALSE;
	}

	while (pCofInfo->pCofNode->nMode != static_cast<uint32_t>(pCompositeUnit->nMode)
		|| pCofInfo->pCofNode->unk0x0C != static_cast<uint32_t>(dword_6F8BC154[pCompositeUnit->nWClassTokenIndex]))
	{
		pCofInfo = pCofInfo->pNext;
		if (!pCofInfo)
		{
			return FALSE;
		}
	}

	D2CofDataStrc* pCofData = pCofInfo->pCofNode->pCofData;
	D2_ASSERT(pCofData);

	const int nFramesPerDir = pCofData->unk0x01;
	const int nLayers = pCofData->unk0x00;
	const uint32_t nFrame = pCompositeUnit->unk0x08 >> 8;
	const uint8_t* pOrderTable = reinterpret_cast<const uint8_t*>(pCofData) + 28 + nFramesPerDir + nLayers * (pCompositeUnit->unk0x04 * nFramesPerDir + nFrame + 9);
	D2_ASSERT(pOrderTable != nullptr);

	DrawMode eDrawMode = DRAWMODE_TRANSHIGHLIGHT;
	if (((pCompositeUnit->dwFlags & gdwBitMasks[5]) && (pCompositeUnit->dwFlags & gdwBitMasks[4]))
		|| (pCompositeUnit->dwFlags & gdwBitMasks[13]))
	{
		eDrawMode = DRAWMODE_TRANS50;
	}

	for (int i = 0; i < nLayers; ++i)
	{
		const uint8_t nComponent = pOrderTable[i];
		D2_ASSERT(pCompositeUnit->unk0x04 < 0x40u);

		unsigned long dwDirRow = 0;
		if (pCompositeUnit->unk0x05)
		{
			_BitScanForward(&dwDirRow, pCompositeUnit->unk0x05);
		}
		const uint8_t nDir = byte_6F8BA370[dwDirRow][pCompositeUnit->unk0x04];
		D2_ASSERT(nDir <= 0x40u);

		D2GfxDataStrc gfxData = {};
		gfxData.nDirection = nDir;
		gfxData.pCellFile = nullptr;
		gfxData.nFrame = nFrame;
		gfxData.nComponent = nComponent;
		sub_6F8A1000(pCompositeUnit, &gfxData);
		gfxData.fFlags |= 2u;

		uint8_t* pPalette = nullptr;
		const uint8_t nColor = pCompositeUnit->unk0x28[nComponent];
		if (nColor != 0xFF)
		{
			int nTrans = nColor >> 5;
			const int nCol = nColor & 0x1F;
			if (nCol >= 0 && nCol < 21)
			{
				if (nTrans == 0)
				{
					nTrans = 8;
				}
				if (nTrans >= 0 && nTrans < 9 && (nTrans <= 2 || nTrans > 4))
				{
					pPalette = &gPaletteShiftTables_6F8C2118[nTrans][256 * nCol];
				}
			}
		}

		TEXTURE_CelDraw(&gfxData, pCompositeUnit->nX, pCompositeUnit->nY, dwGamma, eDrawMode, pPalette);
	}

	const uint32_t nNextFrame = pCompositeUnit->unk0x10 + pCompositeUnit->unk0x08;
	pCompositeUnit->unk0x08 = nNextFrame;
	if ((nNextFrame >> 8) >= pCompositeUnit->unk0x0C - 1)
	{
		pCompositeUnit->unk0x08 = 0;
	}

	const DWORD dwTickCount = GetTickCount();
	if (dwTickCount > dword_6F8FD658)
	{
		dword_6F8FD658 = dwTickCount + 66;
		++dword_6F8FD664;
		if (dword_6F8FD664 >= gnSquelchCelFileFrameCount_6F8FD660)
		{
			dword_6F8FD664 = 0;
		}
		++dword_6F8FD670;
		if (dword_6F8FD670 >= gnSelectedCelFileFrameCount_6F8FD66C)
		{
			dword_6F8FD670 = 0;
		}
	}

	if (pCompositeUnit->dwFlags & gdwBitMasks[9])
	{
		D2_ASSERT(ghSquelchCelFile_6F8FD65C != nullptr);

		D2GfxDataStrc gfxData = {};
		gfxData.pCellFile = ghSquelchCelFile_6F8FD65C;
		gfxData.nFrame = dword_6F8FD664;
		gfxData.nDirection = 0;
		TEXTURE_CelDraw(&gfxData, pCompositeUnit->nX, pCompositeUnit->nY + 10, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
	}

	return TRUE;
}

//D2Win.0x6F8A1D10
BOOL __fastcall sub_6F8A1D10(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	if (!(pCompositeUnit->dwFlags & gdwBitMasks[1]))
	{
		return FALSE;
	}

	int32_t nY = pCompositeUnit->nY + 6;
	Unicode wszText[256] = {};
	Unicode wszFullText[256] = {};

	int32_t nColor = 0;
	BOOL bDrawDropShadow = FALSE;
	if (pCompositeUnit->nUnitIndex == 20 || pCompositeUnit->nUnitIndex == 18)
	{
		bDrawDropShadow = TRUE;
		nColor = 3;
	}
	else if (pCompositeUnit->dwFlags & gdwBitMasks[5])
	{
		nColor = 1;
	}

	const uint32_t dwFlags = pCompositeUnit->dwFlags;
	if (dwFlags & gdwBitMasks[8])
	{
		nY += 15;
	}

	if ((dwFlags & gdwBitMasks[6]) || (dwFlags & gdwBitMasks[7]))
	{
		const int nTitleIdx = ((dwFlags >> 6) & 1) + 3 * ((dwFlags >> 5) & 1) + 2 * ((dwFlags >> 7) & 1);
		const char* szTitle = D2Win_10160_GetPlayerTitle(pCompositeUnit->nUnitIndex, nTitleIdx, (dwFlags >> 12) & 1);
		Unicode wszTitle[256] = {};
		Unicode::win2Unicode(wszTitle, szTitle, 255);
		Unicode::strcpy(wszFullText, wszTitle);
		Unicode::strcat(wszFullText, pCompositeUnit->unk0x14C);
	}
	else
	{
		const uint32_t dwProduct = pCompositeUnit->unk0x230;
		const Unicode* pSrcName = (dwProduct == 'D2DV' || dwProduct == 'D2ST' || dwProduct == 'D2XP')
			? pCompositeUnit->unk0x14C
			: pCompositeUnit->unk0xCC;
		Unicode::strcpy(wszFullText, pSrcName);
	}

	if (Unicode::strlen(pCompositeUnit->unk0x1CC))
	{
		Unicode wszBracket[256] = {};
		Unicode::win2Unicode(wszBracket, " {", 256);
		Unicode::strcat(wszFullText, wszBracket);
		Unicode::strcat(wszFullText, pCompositeUnit->unk0x1CC);
		Unicode::win2Unicode(wszBracket, "}", 256);
		Unicode::strcat(wszFullText, wszBracket);
	}

	D2Win_10127_SetFont(D2FONT_FONT16);

	char szAnsi[512] = {};
	Unicode::unicode2Win(szAnsi, wszFullText, sizeof(szAnsi));

	const int nLen = static_cast<int>(strlen(szAnsi));
	int nClipIndex = 0;
	BOOL bTruncated = FALSE;
	for (; nClipIndex < nLen; ++nClipIndex)
	{
		if (D2Win_10122(wszFullText, nClipIndex + 1) >= 120)
		{
			bTruncated = TRUE;
			break;
		}
	}

	szAnsi[nClipIndex] = '\0';
	if (bTruncated)
	{
		strcat(szAnsi, "...");
	}

	Unicode::win2Unicode(wszText, szAnsi, 255);

	int32_t nWidth = 0;
	int32_t nHeight = 0;
	D2Win_10131_GetTextDimensions(wszText, &nWidth, &nHeight);
	if (bDrawDropShadow)
	{
		D2Win_10117_DrawText(wszText, pCompositeUnit->nX - (nWidth >> 1) - 1, nY - 1, 6, 0);
	}
	D2Win_10117_DrawText(wszText, pCompositeUnit->nX - (nWidth >> 1), nY, nColor, 0);
	return TRUE;
}

//D2Win.0x6F8A2040
void __stdcall sub_6F8A2040(const Unicode* a1, int a2, int a3, void(__fastcall* a4)(const Unicode*))
{
	const Unicode* v4 = a1;
	for (int i = 0; i < a3; ++i)
	{
		a4(v4);
		v4 = (const Unicode*)((char*)v4 + a2);
	}
}

//D2Win.0x6F8A2070
BOOL __fastcall sub_6F8A2070(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	if (!(pCompositeUnit->dwFlags & gdwBitMasks[3]))
	{
		return FALSE;
	}

	RECT rc = {};
	SetRect(&rc, pCompositeUnit->nX - 30, pCompositeUnit->nY - 80, pCompositeUnit->nX + 30, pCompositeUnit->nY + 20);
	if (!CONTROL_MouseInsideRect(&rc))
	{
		return FALSE;
	}

	struct ExtraDataLineStrc
	{
		Unicode wszLine[256];
		int32_t nWidth;
		int32_t nHeight;
	};

	ExtraDataLineStrc lines[7] = {};
	int32_t nMaxWidth = 0;
	int32_t nTotalHeight = 0;
	int32_t nStrCount = 0;
	int32_t nFirstLineColor = 0;

	const int32_t nBaseX = pCompositeUnit->nX;
	const int32_t nYEnd = pCompositeUnit->nY - 80;

	D2Win_10127_SetFont(D2FONT_FONT8);

	auto AppendLine = [&](const Unicode* pStr)
	{
		D2_ASSERT(nStrCount < 7);
		Unicode::strcpy(lines[nStrCount].wszLine, pStr);
		D2Win_10131_GetTextDimensions(lines[nStrCount].wszLine, &lines[nStrCount].nWidth, &lines[nStrCount].nHeight);
		if (nMaxWidth <= lines[nStrCount].nWidth + 6)
		{
			nMaxWidth = lines[nStrCount].nWidth + 6;
		}
		nTotalHeight += lines[nStrCount].nHeight;
		++nStrCount;
	};

	auto AppendAsciiLine = [&](const char* szAscii)
	{
		Unicode wszTemp[256] = {};
		Unicode::win2Unicode(wszTemp, szAscii, 256);
		AppendLine(wszTemp);
	};

	if (pCompositeUnit->unk0x230 == 'D2XP')
	{
		AppendAsciiLine("Expansion");
	}

	bool bCheckRealm = false;
	switch (pCompositeUnit->nUnitIndex)
	{
	case 19:
		AppendAsciiLine("Moderator");
		bCheckRealm = true;
		break;
	case 20:
		AppendAsciiLine("Sysop");
		nFirstLineColor = 3;
		break;
	case 18:
		AppendAsciiLine("Blizzard Rep");
		nFirstLineColor = 3;
		break;
	case 23:
		AppendAsciiLine("Speaker");
		break;
	case 21:
		AppendAsciiLine("Referee");
		break;
	case 22:
		AppendAsciiLine("Chat");
		break;
	case 24:
		AppendAsciiLine("Open Character");
		break;
	default:
		{
			const uint32_t dwProduct = pCompositeUnit->unk0x230;
			if (dwProduct == 'D2DV' || dwProduct == 'D2XP' || dwProduct == 'D2ST')
			{
				bCheckRealm = true;
			}
			else
			{
				const char* szGameName = "Unknown";
				switch (dwProduct)
				{
				case 'WAR3':
					szGameName = "Warcraft III";
					break;
				case 'W3XP':
					szGameName = "Frozen Throne";
					break;
				case 'STAR':
				case 'JSTR':
					szGameName = "Starcraft";
					break;
				case 'W2BN':
					szGameName = "Warcraft II";
					break;
				case 'SSHR':
					szGameName = "Starcraft Shareware";
					break;
				case 'DTST':
				case 'DRTL':
					szGameName = "Diablo";
					break;
				case 'DSHR':
					szGameName = "Diablo Shareware";
					break;
				case 'SEXP':
					szGameName = "Brood War";
					break;
				default:
					break;
				}
				AppendAsciiLine(szGameName);
			}
		}
		break;
	}

	if (bCheckRealm && pCompositeUnit->unk0x22C > 0)
	{
		Unicode wszTemp[256] = {};
		Unicode::sprintf(256, wszTemp, D2LANG_GetStringFromTblIndex(5017), pCompositeUnit->unk0x22C);
		AppendLine(wszTemp);

		if (pCompositeUnit->dwFlags & gdwBitMasks[5])
		{
			AppendLine(D2LANG_GetStringFromTblIndex(5016));
		}

		Unicode::win2Unicode(wszTemp, "Realm: ", 256);
		Unicode::strcat(wszTemp, pCompositeUnit->unk0x1EC);
		AppendLine(wszTemp);
	}

	{
		Unicode wszTemp[256] = {};
		Unicode::win2Unicode(wszTemp, "Account Name: ", 256);
		Unicode::strcat(wszTemp, pCompositeUnit->unk0xCC);
		AppendLine(wszTemp);
	}

	const int32_t nBoxHeight = nTotalHeight + 6;
	int32_t nBoxX = nBaseX - (nMaxWidth >> 1);
	int32_t nShiftX = 0;
	if (nBoxX >= 0)
	{
		if (nBoxX + nMaxWidth > 800)
		{
			nShiftX = 800 - nBoxX - nMaxWidth;
			nBoxX = 800 - nMaxWidth;
		}
	}
	else
	{
		nShiftX = -nBoxX;
		nBoxX = 0;
	}

	const int32_t nBoxY = nYEnd - nBoxHeight;
	D2GFX_DrawBox(nBoxX, nBoxY, nBoxX + nMaxWidth, nYEnd, 0, DRAWMODE_TRANS50);

	int32_t nCurY = nBoxY + 3;
	const int32_t nCenterX = nBaseX + nShiftX;
	D2_ASSERT(nStrCount <= 7);

	for (int i = 0; i < nStrCount; ++i)
	{
		nCurY += lines[i].nHeight;
		const int32_t nColor = (i == 0) ? nFirstLineColor : 0;
		D2Win_10117_DrawText(lines[i].wszLine, nCenterX - (lines[i].nWidth >> 1), nCurY, nColor, 0);
	}

	return TRUE;
}

//D2Win.0x6F8A2A20 (#10142)
void __stdcall D2Win_10142_CompUnitDestroy(D2CompositeUnitStrc* pCompositeUnit, int a2)
{
	D2_ASSERT(pCompositeUnit);

	D2GfxInfoStrc* pGfxInfo = pCompositeUnit->pGfxInfo;

	D2_ASSERT(pGfxInfo);

	D2CofInfoStrc* pCofInfo = pGfxInfo->pCofInfo;
	while (pCofInfo)
	{
		D2CofNodeStrc* pRemCofNode = pCofInfo->pCofNode;
		D2CofInfoStrc* pNextCofInfo = pCofInfo->pNext;

		D2_ASSERT(pRemCofNode);

		if (dword_6F8FD654)
		{
			--pRemCofNode->unk0x00;
			if (pRemCofNode->unk0x00 <= 0)
			{
				D2CofNodeStrc* v8 = pRemCofNode->unk0x1C;
				D2CofNodeStrc* v9 = pRemCofNode->unk0x18;

				D2_FREE(pRemCofNode->pCofData);
				pRemCofNode->pCofData = nullptr;

				if (v8)
				{
					if (v9)
					{
						v8->unk0x18 = v9;
						v9->unk0x1C = v8;
					}
					else
					{
						v8->unk0x18 = nullptr;
					}
				}
				else
				{
					dword_6F8FD654 = v9;
					if (v9)
					{
						v9->unk0x1C = nullptr;
					}
				}
				D2_FREE(pRemCofNode);
			}
		}

		D2_FREE(pCofInfo);
		pCofInfo = pNextCofInfo;
	}

	pGfxInfo->pCofInfo = nullptr;
	D2_FREE(pGfxInfo);

	if (pCompositeUnit->pEmblemCellFile)
	{
		ARCHIVE_FreeCellFile(pCompositeUnit->pEmblemCellFile);
		pCompositeUnit->pEmblemCellFile = nullptr;
	}

	if (a2)
	{
		D2CompositeUnitStrc* v11 = pCompositeUnit->unk0xC8;
		D2CompositeUnitStrc* v12 = pCompositeUnit->pNext;
		if (v11)
		{
			v11->pNext = v12;
			if (v12)
			{
				v12->unk0xC8 = v11;
			}
		}
		else
		{
			dword_6F8FD650 = pCompositeUnit->pNext;
			if (v12)
			{
				v12->unk0xC8 = nullptr;
			}
		}
		D2_FREE(pCompositeUnit);
	}
}

//D2Win.0x6F8A2BD0 (#10154)
void __stdcall D2Win_10154(D2CompositeUnitStrc* pCompositeUnit, const Unicode* a2, const Unicode* a3, const Unicode* a4, const Unicode* a5, int a6, int a7)
{
	D2_ASSERT(pCompositeUnit);

	Unicode::strcpy(pCompositeUnit->unk0xCC, a2);
	Unicode::strcpy(pCompositeUnit->unk0x14C, a3);
	Unicode::strcpy(pCompositeUnit->unk0x1CC, a4);
	Unicode::strcpy(pCompositeUnit->unk0x1EC, a5);
	pCompositeUnit->unk0x22C = a6;
	pCompositeUnit->unk0x230 = a7;
}

//D2Win.0x6F8A2C50 (#10143)
BOOL __fastcall D2Win_10143(D2CompositeUnitStrc* pCompositeUnit, int nMode)
{
	D2_ASSERT(pCompositeUnit);

	for (D2CofInfoStrc* pCofInfo = pCompositeUnit->pGfxInfo->pCofInfo; pCofInfo; pCofInfo = pCofInfo->pNext)
	{
		if (pCofInfo->pCofNode->nMode == static_cast<uint32_t>(nMode))
		{
			D2CofDataStrc* pCofData = pCofInfo->pCofNode->pCofData;

			D2_ASSERT(pCofData);

			pCompositeUnit->nMode = nMode;
			pCompositeUnit->unk0x08 = 0;
			pCompositeUnit->unk0x0C = pCofData->unk0x01;
			pCompositeUnit->unk0x10 = pCofData->unk0x18;
			pCompositeUnit->unk0x05 = pCofData->unk0x02;

			return TRUE;
		}
	}

	return FALSE;
}

//D2Win.0x6F8A2CE0
void* __stdcall sub_6F8A2CE0(HD2ARCHIVE hArchive, const char* szName, D2BinFieldStrc* pBinField, int* pRecordCount, int nRecordSize)
{
	char szFilename[260] = {};

	if (dword_6F8BCF94)
	{
		wsprintfA(szFilename, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", szName, ".bin");
	}
	else if (_strcmpi(szName, "leveldefs"))
	{
		wsprintfA(szFilename, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", szName, ".txt");
	}
	else
	{
		wsprintfA(szFilename, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", "levels", ".txt");
	}

	unsigned int nFileSize = 0;
	void* pFileData = ARCHIVE_ALLOC_BUFFER_AND_READ_FILE_TO_IT(hArchive, szFilename, &nFileSize);

	D2_ASSERT(pFileData);

	void* pBinData = nullptr;
	int nRecordCount = 0;
	if (dword_6F8BCF94)
	{
		nRecordCount = *(int*)pFileData;
		pBinData = (char*)pFileData + 4;
	}
	else
	{
		D2BinFileStrc* pBinFile = FOG_CreateBinFile(pFileData, nFileSize);
		nRecordCount = FOG_GetRecordCountFromBinFile(pBinFile);
		pBinData = FOG_AllocPool(nullptr, nRecordSize * nRecordCount, __FILE__, __LINE__, 0);
		memset(pBinData, 0x00, nRecordSize * nRecordCount);

		FOG_10207(pBinFile, pBinField, pBinData, nRecordCount, nRecordSize);
		FOG_FreeBinFile(pBinFile);
	}

	if (pRecordCount)
	{
		*pRecordCount = nRecordCount;
	}

	return pBinData;
}

//D2Win.0x6F8A2E70
int __fastcall sub_6F8A2E70(HD2ARCHIVE hArchive)
{
	D2BinFieldStrc pItemFields[] =
	{
		{ "version", TXTFIELD_WORD, 0, 246, nullptr },
		{ "type", TXTFIELD_CODETOWORD, 0, 286, &dword_6F8FD240 },
		{ "code", TXTFIELD_RAW, 0, 128, nullptr },
		{ "alternategfx", TXTFIELD_RAW, 0, 144, nullptr },
		{ "spawnable", TXTFIELD_BYTE, 0, 307, nullptr },
		{ "level", TXTFIELD_BYTE, 0, 253, nullptr },
		{ "component", TXTFIELD_BYTE, 0, 277, nullptr },
		{ "wclass", TXTFIELD_RAW, 0, 192, nullptr },
		{ "2handedwclass", TXTFIELD_RAW, 0, 196, nullptr },
		{ "1or2handed", TXTFIELD_BYTE2, 0, 317, nullptr },
		{ "Transform", TXTFIELD_BYTE, 0, 321, nullptr },
		{ "end", TXTFIELD_NONE, 0, 0, nullptr },
	};

	uint8_t* pWeapons = static_cast<uint8_t*>(sub_6F8A2CE0(hArchive, "weapons", pItemFields, &dword_6F8C1504, 424));
	uint8_t* pArmor = static_cast<uint8_t*>(sub_6F8A2CE0(hArchive, "armor", pItemFields, &dword_6F8C150C, 424));
	uint8_t* pMisc = static_cast<uint8_t*>(sub_6F8A2CE0(hArchive, "misc", pItemFields, &dword_6F8C1514, 424));

	dword_6F8C14F8 = dword_6F8C1504 + dword_6F8C150C + dword_6F8C1514;
	uint8_t* pCombined = static_cast<uint8_t*>(FOG_AllocPool(nullptr, 424 * dword_6F8C14F8, __FILE__, __LINE__, 0));
	dword_6F8C14FC = pCombined;
	dword_6F8C1500 = pCombined;
	memcpy(pCombined, pWeapons, 424 * dword_6F8C1504);

	dword_6F8C1508 = pCombined + 424 * dword_6F8C1504;
	memcpy(dword_6F8C1508, pArmor, 424 * dword_6F8C150C);

	dword_6F8C1510 = dword_6F8C1508 + 424 * dword_6F8C150C;
	memcpy(dword_6F8C1510, pMisc, 424 * dword_6F8C1514);

	FOG_FreePool(nullptr, pWeapons - 4, __FILE__, __LINE__, 0);
	FOG_FreePool(nullptr, pArmor - 4, __FILE__, __LINE__, 0);
	FOG_FreePool(nullptr, pMisc - 4, __FILE__, __LINE__, 0);

	dword_6F8C1518 = static_cast<uint16_t*>(FOG_AllocPool(nullptr, 2 * dword_6F8C14F8, __FILE__, __LINE__, 0));
	memset(dword_6F8C1518, 0, 2 * dword_6F8C14F8);

	int nOldIdx = 0;
	for (int i = 0; i < dword_6F8C14F8; ++i)
	{
		if (*reinterpret_cast<uint16_t*>(dword_6F8C14FC + 424 * i + 246) == 0)
		{
			dword_6F8C1518[nOldIdx++] = static_cast<uint16_t>(i);
		}
	}

	dword_6F8FD23C = FOG_AllocLinker(__FILE__, __LINE__);
	for (int i = 0; i < dword_6F8C14F8; ++i)
	{
		FOG_10215(dword_6F8FD23C, *reinterpret_cast<int*>(dword_6F8C14FC + 424 * i + 128));
	}

	return dword_6F8C14F8;
}

//D2Win.0x6F8A32B0
BOOL __stdcall sub_6F8A32B0(int a1, int nItemType)
{
	if (!nItemType)
	{
		return TRUE;
	}

	if (a1)
	{
		int itemTypeStack[65] = {};

		itemTypeStack[1] = a1;
		int nStackIndex = 1;
		while (nStackIndex)
		{
			const int v5 = itemTypeStack[nStackIndex];
			--nStackIndex;

			if (nItemType == v5)
			{
				return TRUE;
			}

			D2ItemTypesTxt* pItemTypesTxtRecord = &gpItemTypesTxt_6F8FD244[v5];
			if (pItemTypesTxtRecord->nEquiv1)
			{
				++nStackIndex;
				itemTypeStack[nStackIndex] = pItemTypesTxtRecord->nEquiv1;

				if (pItemTypesTxtRecord->nEquiv2)
				{
					++nStackIndex;
					itemTypeStack[nStackIndex] = pItemTypesTxtRecord->nEquiv2;
				}
			}

			D2_ASSERT(nStackIndex < 64);
		}
	}

	return FALSE;
}

//D2Win.0x6F8A3360 (#10159)
void __stdcall D2Win_10159(D2CellFileStrc* pSquelchCelFile, D2CellFileStrc* pSelectedCelFile)
{
	ghSquelchCelFile_6F8FD65C = pSquelchCelFile;
	gnSquelchCelFileFrameCount_6F8FD660 = D2CMP_CelFileGetCelsPerDirection(pSquelchCelFile);
	ghSelectedCelFile_6F8FD668 = pSelectedCelFile;
	gnSelectedCelFileFrameCount_6F8FD66C = D2CMP_CelFileGetCelsPerDirection(pSelectedCelFile);
}

//D2Win.0x6F8A3570
int __fastcall sub_6F8A3570(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	if (static_cast<uint32_t>(pCompositeUnit->nUnitIndex) >= dword_6F8BC0A4 || static_cast<uint32_t>(pCompositeUnit->nMode) >= static_cast<unsigned int>(gnPlayerModeTokenTblSize_6F8BC0F8) || static_cast<uint32_t>(pCompositeUnit->nWClassTokenIndex) >= static_cast<unsigned int>(dword_6F8BC190))
	{
		pCompositeUnit->nUnitIndex = 7;
		pCompositeUnit->nMode = 5;
		pCompositeUnit->nWClassTokenIndex = 1;
	}

	D2GfxInfoStrc* pGfxInfo = pCompositeUnit->pGfxInfo;

	D2_ASSERT(pGfxInfo);

	int v4 = 0;

	if (sub_6F8A1250(pGfxInfo, pCompositeUnit->nUnitIndex, pCompositeUnit->nMode, &v4, &v4, dword_6F8BC154[pCompositeUnit->nWClassTokenIndex]))
	{
		return 1;
	}

	pCompositeUnit->nMode = 5;
	pCompositeUnit->nUnitIndex = 7;
	pCompositeUnit->nWClassTokenIndex = 1;

	D2CofDataStrc* pCurrCofData = sub_6F8A1250(pGfxInfo, 7u, 5u, &v4, &v4, dword_6F8BC154[1]);

	D2_ASSERT(pCurrCofData);
	return 1;
}

//D2Win.0x6F8A3680
int __cdecl sub_6F8A3680()
{
	if (dword_6F8FD674)
	{
		return dword_6F8FD674;
	}

	memset(stru_6F8C1520, 0, sizeof(stru_6F8C1520));
	dword_6F8FD674 = 1;

	D2BinFieldStrc pItemTypesFields[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &dword_6F8FD240 },
		{ "equiv1", TXTFIELD_CODETOWORD, 0, 4, &dword_6F8FD240 },
		{ "equiv2", TXTFIELD_CODETOWORD, 0, 6, &dword_6F8FD240 },
		{ "repair", TXTFIELD_BYTE, 0, 8, nullptr },
		{ "body", TXTFIELD_BYTE, 0, 9, nullptr },
		{ "shoots", TXTFIELD_CODETOWORD, 0, 12, &dword_6F8FD240 },
		{ "quiver", TXTFIELD_CODETOWORD, 0, 14, &dword_6F8FD240 },
		{ "throwable", TXTFIELD_BYTE, 0, 16, nullptr },
		{ "reload", TXTFIELD_BYTE, 0, 17, nullptr },
		{ "reequip", TXTFIELD_BYTE, 0, 18, nullptr },
		{ "autostack", TXTFIELD_BYTE, 0, 19, nullptr },
		{ "magic", TXTFIELD_BYTE, 0, 20, nullptr },
		{ "normal", TXTFIELD_BYTE, 0, 22, nullptr },
		{ "charm", TXTFIELD_BYTE, 0, 23, nullptr },
		{ "gem", TXTFIELD_BYTE, 0, 24, nullptr },
		{ "beltable", TXTFIELD_BYTE, 0, 25, nullptr },
		{ "costformula", TXTFIELD_BYTE, 0, 32, nullptr },
		{ "varinvgfx", TXTFIELD_BYTE, 0, 35, nullptr },
		{ "invgfx1", TXTFIELD_ASCII, 31, 36, nullptr },
		{ "invgfx2", TXTFIELD_ASCII, 31, 68, nullptr },
		{ "invgfx3", TXTFIELD_ASCII, 31, 100, nullptr },
		{ "invgfx4", TXTFIELD_ASCII, 31, 132, nullptr },
		{ "invgfx5", TXTFIELD_ASCII, 31, 164, nullptr },
		{ "end", TXTFIELD_NONE, 0, 0, nullptr },
	};

	dword_6F8FD240 = FOG_AllocLinker(__FILE__, __LINE__);
	gpItemTypesTxt_6F8FD244 = static_cast<D2ItemTypesTxt*>(sub_6F8A2CE0(D2Win_GetArchive(), "itemtypes", pItemTypesFields, &unk_6F8FD248, sizeof(D2ItemTypesTxt)));

	D2BinFieldStrc pHitClassFields[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &dword_6F8FD238 },
		{ "end", TXTFIELD_NONE, 0, 0, nullptr },
	};

	dword_6F8FD238 = FOG_AllocLinker(__FILE__, __LINE__);
	dword_6F8FD234 = sub_6F8A2CE0(D2Win_GetArchive(), "hitclass", pHitClassFields, nullptr, 4);
	sub_6F8A2E70(D2Win_GetArchive());

	stru_6F8C1520[1].dwItemCode = ' til';
	stru_6F8C1520[1].nWClass = dword_6F8BCF98[0];
	stru_6F8C1520[1].nItemType = 3;

	stru_6F8C1520[2].dwItemCode = ' dem';
	stru_6F8C1520[2].nWClass = dword_6F8BCF98[0];
	stru_6F8C1520[2].nItemType = 3;

	stru_6F8C1520[3].dwItemCode = ' yvh';
	stru_6F8C1520[3].nWClass = dword_6F8BCF98[0];
	stru_6F8C1520[3].nItemType = 3;

	int nNextIdx = 4;
	for (int i = 0; i < dword_6F8C14F8; ++i)
	{
		const uint8_t* pRow = dword_6F8C14FC + 424 * i;
		const uint32_t dwGfxCode = *reinterpret_cast<const uint32_t*>(pRow + 144)
			? *reinterpret_cast<const uint32_t*>(pRow + 144)
			: *reinterpret_cast<const uint32_t*>(pRow + 128);
		const uint16_t wItemType = *reinterpret_cast<const uint16_t*>(pRow + 286);

		bool bValid = (sub_6F8A32B0(wItemType, 45)
			|| sub_6F8A32B0(wItemType, 3)
			|| sub_6F8A32B0(wItemType, 51)
			|| sub_6F8A32B0(wItemType, 37))
			&& !sub_6F8A32B0(wItemType, 75);

		bool bAlreadyPresent = false;
		for (int j = 0; j < nNextIdx; ++j)
		{
			if (stru_6F8C1520[j].dwItemCode == dwGfxCode)
			{
				bAlreadyPresent = true;
				break;
			}
		}

		if (!bAlreadyPresent && bValid)
		{
			int nSlot = nNextIdx;
			while (nSlot < 255 && (
				(sub_6F8A32B0(stru_6F8BC1D8[nSlot].nItemType, 45) && sub_6F8A32B0(wItemType, 45))
				|| (sub_6F8A32B0(stru_6F8BC1D8[nSlot].nItemType, 50) && sub_6F8A32B0(wItemType, 50))
				|| stru_6F8C1520[nSlot].dwItemCode != 0))
			{
				++nSlot;
			}
			if (nSlot >= 255)
			{
				nSlot = nNextIdx;
			}

			stru_6F8C1520[nSlot].dwItemCode = dwGfxCode;
			stru_6F8C1520[nSlot].nItemType = wItemType;

			const uint32_t dwWClass1 = *reinterpret_cast<const uint32_t*>(pRow + 192);
			int nWClassIdx1 = 0;
			for (int k = 0; k < dword_6F8BCF90; ++k)
			{
				if (dwWClass1 == stru_6F8BCF28[k].dwCode)
				{
					nWClassIdx1 = stru_6F8BCF28[k].nIndex;
					break;
				}
			}
			stru_6F8C1520[nSlot].nWClass = dword_6F8BCF98[nWClassIdx1];

			const uint32_t dwWClass2 = *reinterpret_cast<const uint32_t*>(pRow + 196);
			int nWClassIdx2 = 0;
			for (int k = 0; k < dword_6F8BCF90; ++k)
			{
				if (dwWClass2 == stru_6F8BCF28[k].dwCode)
				{
					nWClassIdx2 = stru_6F8BCF28[k].nIndex;
					break;
				}
			}
			dword_6F8FD250[nSlot] = dword_6F8BCF98[nWClassIdx2];

			if (nNextIdx == nSlot)
			{
				++nNextIdx;
			}
		}
	}

	dword_6F8FD230 = 255;
	return dword_6F8C14F8;
}

//D2Win.0x6F8A3DC0
int __fastcall sub_6F8A3DC0(BYTE* a1, int a2)
{
	const uint8_t bRight = a1[5];
	const uint8_t bLeft = a1[6];
	const bool bDual = (bRight != 0xFF && bLeft != 0xFF);

	int v4 = 0;
	if (bRight != 0xFF)
	{
		if (bDual)
		{
			v4 = dword_6F8FD250[bRight];
		}
		else
		{
			v4 = stru_6F8C1520[bRight].nWClass;
			if (bLeft == 0xFF && a1[7] == 0xFF && dword_6F8FD250[bRight] != v4)
			{
				v4 = dword_6F8FD250[bRight];
			}
		}

		if (((v4 == 13 || v4 == 14) && a2 != 6) || sub_6F8A32B0(stru_6F8C1520[bRight].nItemType, 50))
		{
			v4 = stru_6F8BC1D8[bRight].nWClass;
		}
	}

	int v7 = 0;
	if (bLeft != 0xFF)
	{
		v7 = bDual ? dword_6F8FD250[bLeft] : stru_6F8C1520[bLeft].nWClass;
		if (((v7 == 13 || v7 == 14) && a2 != 6) || sub_6F8A32B0(stru_6F8C1520[bLeft].nItemType, 50))
		{
			v7 = stru_6F8BC1D8[bLeft].nWClass;
		}
	}

	if ((v4 == 13 || v4 == 14) && a2 != 6)
	{
		v4 = 0;
	}
	if ((v7 == 13 || v7 == 14) && a2 != 6)
	{
		v7 = 0;
	}

	if (v4)
	{
		if (v4 == v7 && (v4 == 6 || v4 == 7))
		{
			return v4;
		}
		if (v4 == 8)
		{
			return 8;
		}
		if (!v7)
		{
			return v4;
		}
		if (v4 == 4)
		{
			if (v7 == 4)
			{
				return 11;
			}
			if (v7 == 2)
			{
				return 9;
			}
		}
		else if (v4 == 2)
		{
			if (v7 == 4)
			{
				return 12;
			}
			if (v7 == 5)
			{
				return 11;
			}
			if (v7 == 2)
			{
				return 10;
			}
			return 0;
		}
		else if (v4 == 5 && v7 == 2)
		{
			return 11;
		}

		if (v4 == 5)
		{
			if (v7 == 4 || v7 == 5)
			{
				return 11;
			}
		}
		else if (v4 == 4)
		{
			if (v7 == 5)
			{
				return 11;
			}
		}
		else if (v4 == 13)
		{
			return (v7 == 13) ? 13 : 0;
		}
		else if (v4 == 14)
		{
			return (v7 == 14) ? 13 : 0;
		}
		else if (v4 == 1 && v7 == 1)
		{
			return 1;
		}
		return 0;
	}

	return v7 ? v7 : 1;
}

//D2Win.0x6F8A4020
void __fastcall sub_6F8A4020(int a1, BYTE* a2)
{
	auto RemapByType = [&](int nTargetType, bool bModulo3)
	{
		if (sub_6F8A32B0(stru_6F8BC1D8[*a2].nItemType, nTargetType))
		{
			for (int i = 0; i < dword_6F8FD230; ++i)
			{
				if (stru_6F8C1520[i].dwItemCode == stru_6F8BC1D8[*a2].dwItemCode)
				{
					*a2 = static_cast<BYTE>(i);
					return;
				}
			}
			*a2 = bModulo3 ? static_cast<BYTE>(*a2 % 3 + 1) : 1;
		}
		else if (!bModulo3)
		{
			*a2 = 1;
		}
	};

	if ((a1 == 1 || a1 == 2 || a1 == 3 || a1 == 4) && *a2 > 3u)
	{
		RemapByType(3, true);
		RemapByType(16, true);
		RemapByType(15, true);
		RemapByType(51, true);
	}

	if (a1 == 0 && !sub_6F8A32B0(stru_6F8C1520[*a2].nItemType, 37))
	{
		RemapByType(37, false);
	}
}

//D2Win.0x6F8A3390 (#10144)
D2CompositeUnitStrc* __stdcall D2Win_10144(unsigned int nUnitIndex, int nMode, BYTE* pComponents, BYTE* pComponentsColors)
{
	D2CompositeUnitStrc* pCompositeUnit = D2_CALLOC_STRC(D2CompositeUnitStrc);
	D2_ASSERT(pCompositeUnit);

	D2GfxInfoStrc* pGfxInfo = D2_CALLOC_STRC(D2GfxInfoStrc);
	D2_ASSERT(pGfxInfo);

	pCompositeUnit->pGfxInfo = pGfxInfo;
	sub_6F8A3680();

	pCompositeUnit->nUnitIndex = nUnitIndex;
	if (nUnitIndex < 7)
	{
		const int nWClass = sub_6F8A3DC0(pComponents, nUnitIndex);
		pCompositeUnit->nWClassTokenIndex = nWClass;
		if (!nWClass)
		{
			return nullptr;
		}
	}
	else
	{
		pCompositeUnit->nWClassTokenIndex = 1;
	}

	pCompositeUnit->nMode = nMode;
	pCompositeUnit->dwFlags |= gdwBitMasks[0] | gdwBitMasks[1] | gdwBitMasks[2];

	for (int i = 0; i < 16; ++i)
	{
		pCompositeUnit->nItemType[i] = 1;
		const BYTE nComp = pComponents[i];
		if (nComp && nComp < dword_6F8FD230 && nComp != 0xFF)
		{
			sub_6F8A4020(i, &pComponents[i]);
			pCompositeUnit->dwArmorType[i] = stru_6F8C1520[pComponents[i]].dwItemCode;
			pCompositeUnit->nItemType[i] = stru_6F8C1520[pComponents[i]].nItemType;
		}
		else
		{
			pCompositeUnit->dwArmorType[i] = 0;
		}
	}

	for (int i = 0; i < 16; ++i)
	{
		const BYTE nColor = pComponentsColors[i];
		pCompositeUnit->unk0x28[i] = (nColor != 0xFF) ? (nColor - 1) : 0xFF;
	}

	if (!sub_6F8A3570(pCompositeUnit) || D2Win_10143(pCompositeUnit, nMode))
	{
		pCompositeUnit->unk0xC8 = nullptr;
		pCompositeUnit->pNext = dword_6F8FD650;
		if (dword_6F8FD650)
		{
			dword_6F8FD650->unk0xC8 = pCompositeUnit;
		}
		dword_6F8FD650 = pCompositeUnit;
		return pCompositeUnit;
	}

	return nullptr;
}

//D2Win.0x6F8A4260 (#10145)
void __stdcall D2Win_10145(D2CompositeUnitStrc* pCompositeUnit, unsigned int nUnitIndex, int nMode, BYTE* a4, BYTE* a5)
{
	D2_ASSERT(pCompositeUnit);

	D2GfxInfoStrc* pGfxInfo = D2_CALLOC_STRC(D2GfxInfoStrc);
	D2_ASSERT(pGfxInfo);

	pCompositeUnit->pGfxInfo = pGfxInfo;
	pCompositeUnit->nUnitIndex = nUnitIndex;
	if (nUnitIndex < 7)
	{
		const int nWClass = sub_6F8A3DC0(a4, nUnitIndex);
		pCompositeUnit->nWClassTokenIndex = nWClass;
		if (!nWClass)
		{
			return;
		}
	}
	else
	{
		pCompositeUnit->nWClassTokenIndex = 1;
	}

	pCompositeUnit->nMode = nMode;
	pCompositeUnit->dwFlags |= gdwBitMasks[0] | gdwBitMasks[1] | gdwBitMasks[2];

	for (int i = 0; i < 16; ++i)
	{
		pCompositeUnit->nItemType[i] = 1;
		if (a4[i] && a4[i] < 255u)
		{
			sub_6F8A4020(i, &a4[i]);
			pCompositeUnit->dwArmorType[i] = stru_6F8C1520[a4[i]].dwItemCode;
			pCompositeUnit->nItemType[i] = stru_6F8C1520[a4[i]].nItemType;
		}
		else
		{
			pCompositeUnit->dwArmorType[i] = 0;
		}
	}

	for (int i = 0; i < 16; ++i)
	{
		const BYTE nColor = a5[i];
		pCompositeUnit->unk0x28[i] = (nColor != 0xFF) ? (nColor - 1) : 0xFF;
	}

	if (sub_6F8A3570(pCompositeUnit))
	{
		D2Win_10143(pCompositeUnit, nMode);
	}
}

//D2Win.0x6F8A43C0 (#10146)
void __stdcall D2Win_10146()
{
	D2CompositeUnitStrc* pCompositeUnit = dword_6F8FD650;
	while (pCompositeUnit)
	{
		D2CompositeUnitStrc* pNext = pCompositeUnit->pNext;
		D2Win_10142_CompUnitDestroy(pCompositeUnit, 0);
		D2_FREE(pCompositeUnit);
		pCompositeUnit = pNext;
	}
	dword_6F8FD650 = nullptr;
}

//D2Win.0x6F8A4400 (#10147)
D2CompositeUnitStrc* __stdcall D2Win_10147()
{
	return dword_6F8FD650;
}

//D2Win.0x6F8A4410 (#10148)
D2CompositeUnitStrc* __stdcall D2Win_10148(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	return pCompositeUnit->pNext;
}

//D2Win.0x6F8A4440 (#10149)
void __stdcall D2Win_10149(D2CompositeUnitStrc* pCompositeUnit, uint8_t a2)
{
	D2_ASSERT(pCompositeUnit);

	pCompositeUnit->unk0x04 = D2Clamp(a2, 0ui8, pCompositeUnit->unk0x05);
}

//D2Win.0x6F8A4490 (#10150)
uint8_t __stdcall D2Win_10150(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	return pCompositeUnit->unk0x05;
}

//D2Win.0x6F8A44C0 (#10169)
int __stdcall D2Win_10169(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	return pCompositeUnit->unk0x0C;
}

//D2Win.0x6F8A44F0 (#10151)
void __stdcall D2Win_10151(D2CompositeUnitStrc* pCompositeUnit, int a2)
{
	D2_ASSERT(pCompositeUnit);

	const int nMax = pCompositeUnit->unk0x0C << 8;
	pCompositeUnit->unk0x08 = D2Clamp(a2, 0, nMax);
}

//D2Win.0x6F8A4540 (#10152)
void __stdcall D2Win_10152(D2CompositeUnitStrc* pCompositeUnit, int nX)
{
	D2_ASSERT(pCompositeUnit);

	pCompositeUnit->nX = nX;
}

//D2Win.0x6F8A4570 (#10153)
void __stdcall D2Win_10153(D2CompositeUnitStrc* pCompositeUnit, int nY)
{
	D2_ASSERT(pCompositeUnit);

	pCompositeUnit->nY = nY;
}

//D2Win.0x6F8A45A0 (#10155)
int __stdcall D2Win_10155(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	return pCompositeUnit->dwFlags;
}

//D2Win.0x6F8A45D0 (#10156)
void __stdcall D2Win_10156(D2CompositeUnitStrc* pCompositeUnit, int nFlags)
{
	D2_ASSERT(pCompositeUnit);

	pCompositeUnit->dwFlags = nFlags;
}

//D2Win.0x6F8A4600 (#10158)
void __stdcall D2Win_10158(D2CompositeUnitStrc* pCompositeUnit, uint8_t a2, uint8_t a3, uint8_t a4)
{
	D2_ASSERT(pCompositeUnit);

	if (a2 < 1u || a2 > 32u)
	{
		return;
	}

	char szFile[512] = {};

	wsprintfA(szFile, "%s\\ui\\emblems\\icon%02dc", "DATA\\GLOBAL", a2 - 1);
	pCompositeUnit->pEmblemCellFile = ARCHIVE_LoadCellFile(szFile, 0);

	if (a3 >= 1u && a3 <= 10u)
	{
		pCompositeUnit->unk0xC0 = byte_6F8FD224[a3 - 1];
	}
	else
	{
		pCompositeUnit->unk0xC0 = 0;
	}

	if (a4 >= 1u && a4 <= 10u)
	{
		pCompositeUnit->unk0xC1 = byte_6F8FD218[a4 - 1];
	}
	else
	{
		pCompositeUnit->unk0xC1 = 0;
	}
}

//D2Win.0x6F8A4B10
int __fastcall sub_6F8A4B10(const char* szName, int nIndex)
{
	char szPath[256] = {};
	wsprintfA(szPath, "Data\\Global\\Items\\Palette\\%s.dat", szName);

	HSFILE hFile = nullptr;
	if (ARCHIVE_OpenFile(nullptr, szPath, &hFile, TRUE))
	{
		ARCHIVE_ReadFileToBuffer(nullptr, hFile, gPaletteShiftTables_6F8C2118[nIndex], 5376);
		ARCHIVE_CloseFile(nullptr, hFile);
		return 1;
	}

	FOG_DisplayWarning("hFile", __FILE__, __LINE__);
	memset(gPaletteShiftTables_6F8C2118[nIndex], 0, 5376);
	return 0;
}

//D2Win.0x6F8A46E0 (#10157)
void __stdcall D2Win_10157()
{
	sub_6F8A4B10("grey", 1);
	sub_6F8A4B10("grey2", 2);
	sub_6F8A4B10("gold", 3);
	sub_6F8A4B10("brown", 4);
	sub_6F8A4B10("greybrown", 5);
	sub_6F8A4B10("invgrey", 6);
	sub_6F8A4B10("invgrey2", 7);
	sub_6F8A4B10("invgreybrown", 8);
}

//D2Win.0x6F8A4C10
void __stdcall sub_6F8A4C10()
{
	constexpr uint8_t byte_6F8BC194[] =
	{
		0xBC, 0x00, 0x00,
		0xE0, 0x70, 0x20,
		0xFC, 0xD4, 0x5C,
		0xFC, 0xE4, 0xA4,
		0xF4, 0xF4, 0xF4,
		0xB8, 0xB8, 0xB8,
		0x74, 0x74, 0x74,
		0x4C, 0x6C, 0xAC,
		0x58, 0x90, 0x4C,
		0xB2, 0x7C, 0x58,
	};

	for (int32_t i = 0; i < std::size(byte_6F8FD224); ++i)
	{
		byte_6F8FD224[i] = D2Win_10034_MixRGB(byte_6F8BC194[3 * i], byte_6F8BC194[3 * i + 1], byte_6F8BC194[3 * i + 2]);
	}

	constexpr uint8_t byte_6F8BC1B4[] =
	{
		0x5C, 0x00, 0x00,
		0x90, 0x00, 0x00,
		0x30, 0x1C, 0x10,
		0x5C, 0x24, 0x08,
		0x04, 0x24, 0x10,
		0x24, 0x5C, 0x24,
		0x00, 0x00, 0x58,
		0x28, 0x28, 0x78,
		0x40, 0x40, 0x40,
		0x00, 0x00, 0x00,
	};

	for (int32_t i = 0; i < std::size(byte_6F8FD218); ++i)
	{
		byte_6F8FD218[i] = D2Win_10034_MixRGB(byte_6F8BC1B4[3 * i], byte_6F8BC1B4[3 * i + 1], byte_6F8BC1B4[3 * i + 2]);
	}
}

//D2Win.0x6F8A4C70 (#10161)
const char* __stdcall D2Win_10161(int a1, int a2, int a3, BOOL bSoftCore)
{
	int v4 = 0;

	if (bSoftCore)
	{
		if (a2 >= 5)
		{
			if (a2 < 10)
			{
				v4 = 1;
			}
			else if (a2 < 15)
			{
				v4 = 2;
			}
			else
			{
				v4 = 3;
			}
		}
	}
	else
	{
		if (a2 >= 4)
		{
			if (a2 < 8)
			{
				v4 = 1;
			}
			else if (a2 < 12)
			{
				v4 = 2;
			}
			else
			{
				v4 = 3;
			}
		}
	}

	if (a3 && v4)
	{
		v4 += 3;
	}

	return D2Win_10160_GetPlayerTitle(a1, v4, bSoftCore);
}

//D2Win.0x6F8A4CE0 (#10160)
const char* __stdcall D2Win_10160_GetPlayerTitle(int a1, int a2, BOOL bSoftCore)
{
	if (bSoftCore)
	{
		switch (a1)
		{
		case 0:
		case 1:
		case 6:
		case 9:
			switch (a2)
			{
			case 1:
				return "Slayer ";
			case 2:
				return "Champion ";
			case 3:
				return "Matriarch ";
			case 4:
				return "Destroyer ";
			case 5:
				return "Conqueror ";
			case 6:
				return "Guardian ";
			default:
				break;
			}
			break;

		default:
			switch (a2)
			{
			case 1:
				return "Slayer ";
			case 2:
				return "Champion ";
			case 3:
				return "Patriarch ";
			case 4:
				return "Destroyer ";
			case 5:
				return "Conqueror ";
			case 6:
				return "Guardian ";
			default:
				break;
			}
			break;
		}
	}
	else
	{
		switch (a1)
		{
		case 0:
		case 1:
		case 6:
		case 9:
			switch (a2)
			{
			case 1:
				return "Dame ";
			case 2:
				return "Lady ";
			case 3:
				return "Baroness ";
			case 4:
				return "Countess ";
			case 5:
				return "Duchess ";
			case 6:
				return "Queen ";
			default:
				break;
			}
			break;

		default:
			switch (a2)
			{
			case 1:
				return "Sir ";
			case 2:
				return "Lord ";
			case 3:
				return "Baron ";
			case 4:
				return "Count ";
			case 5:
				return "Duke ";
			case 6:
				return "King ";
			default:
				break;
			}
			break;
		}
	}

	return byte_6F8FD678;
}
