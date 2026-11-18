#include "D2Launch.h"

#include <Windows.h>
#include <cstdio>
#include <cstring>

#include <Fog.h>
#include <Storm.h>
#include <D2Lang.h>
#include <D2StrTable.h>
#include <D2Sound.h>
#include <D2WinMain.h>
#include <D2WinControlHeader.h>
#include <D2WinButton.h>
#include <D2WinTextBox.h>
#include <D2WinImage.h>
#include <D2WinComp.h>
#include <D2WinFont.h>
#include <D2WinSmack.h>

// ============================================================================
// Forward Declarations of Internal 1.12a Callbacks (0x6FA4B540..0x6FA52370)
// ============================================================================
static int __stdcall D2LAUNCH_OnCharSlotClick(SMSGHANDLER_PARAMS* pMsg);         // 0x6FA4FAD0
static int __stdcall D2LAUNCH_OnCharSelectKeyDown(SMSGHANDLER_PARAMS* pMsg);     // 0x6FA4DF40
static int __stdcall D2LAUNCH_OnCharSelectExit(SMSGHANDLER_PARAMS* pMsg);        // 0x6FA4FA40
static int __stdcall D2LAUNCH_OnCharSelectOk(SMSGHANDLER_PARAMS* pMsg);          // 0x6FA4F850
static int __stdcall D2LAUNCH_OnCharSelectCreateNew(SMSGHANDLER_PARAMS* pMsg);   // 0x6FA4FA90
static int __stdcall D2LAUNCH_OnCharSelectConvertModal(SMSGHANDLER_PARAMS* pMsg);// 0x6FA4BF00
static int __stdcall D2LAUNCH_OnCharSelectDeleteModal(SMSGHANDLER_PARAMS* pMsg); // 0x6FA4CF60 -> 0x6FA4BD80
static int __stdcall D2LAUNCH_OnDeleteModalNo(SMSGHANDLER_PARAMS* pMsg);         // 0x6FA4BD40
static int __stdcall D2LAUNCH_OnDeleteModalYes(SMSGHANDLER_PARAMS* pMsg);        // 0x6FA4F4F0
static int __stdcall D2LAUNCH_OnConvertModalNo(SMSGHANDLER_PARAMS* pMsg);        // 0x6FA4BEB0
static int __stdcall D2LAUNCH_OnConvertModalYes(SMSGHANDLER_PARAMS* pMsg);       // 0x6FA4F5E0
static int __stdcall D2LAUNCH_OnDifficultyNormal(SMSGHANDLER_PARAMS* pMsg);      // 0x6FA4FBD0
static int __stdcall D2LAUNCH_OnDifficultyNightmare(SMSGHANDLER_PARAMS* pMsg);   // 0x6FA4FBC0
static int __stdcall D2LAUNCH_OnDifficultyHell(SMSGHANDLER_PARAMS* pMsg);        // 0x6FA4FBB0
static int __stdcall D2LAUNCH_OnDifficultyCancel(SMSGHANDLER_PARAMS* pMsg);      // 0x6FA52370
static void __stdcall D2LAUNCH_OnCharScroll(SMSGHANDLER_PARAMS* pMsg);           // 0x6FA4DBC0

// ============================================================================
// 1.12a .data Control Descriptors for Character Select (0x6FA627F8..0x6FA62FA8)
// ============================================================================
static D2LaunchScrollTrackStrc gCharScrollTrack = { 585, 457, 363 }; // 0x6FA60F2C

// 8 Main Character Slot TextBoxes (0x6FA627F8..0x6FA62948: field_14 = 0x4C = 76px paperdoll offset)
static D2WinControlInitStrc gCtrlCharSlotText[8] = {
	{ D2WIN_TEXTBOX, 37,  178, 200, 92, 0x4C, 0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA627F8
	{ D2WIN_TEXTBOX, 309, 178, 200, 92, 0x4C, 0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA62828
	{ D2WIN_TEXTBOX, 37,  271, 200, 92, 0x4C, 0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA62858
	{ D2WIN_TEXTBOX, 309, 271, 200, 92, 0x4C, 0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA62888
	{ D2WIN_TEXTBOX, 37,  364, 200, 92, 0x4C, 0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA628B8
	{ D2WIN_TEXTBOX, 309, 364, 200, 92, 0x4C, 0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA628E8
	{ D2WIN_TEXTBOX, 37,  457, 200, 92, 0x4C, 0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA62918
	{ D2WIN_TEXTBOX, 309, 457, 200, 92, 0x4C, 0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA62948
};

// 8 Right-Hand Clickable Slot TextBoxes (0x6FA62978..0x6FA62AC8: extends slot click target to full 272px width)
static D2WinControlInitStrc gCtrlCharSlotRightHit[8] = {
	{ D2WIN_TEXTBOX, 237, 178, 72,  93, 0,    0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA62978
	{ D2WIN_TEXTBOX, 509, 178, 72,  93, 0,    0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA629A8
	{ D2WIN_TEXTBOX, 237, 271, 72,  93, 0,    0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA629D8
	{ D2WIN_TEXTBOX, 509, 271, 72,  93, 0,    0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA62A08
	{ D2WIN_TEXTBOX, 237, 364, 72,  93, 0,    0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA62A38
	{ D2WIN_TEXTBOX, 509, 364, 72,  93, 0,    0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA62A68
	{ D2WIN_TEXTBOX, 237, 457, 72,  93, 0,    0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA62A98
	{ D2WIN_TEXTBOX, 509, 457, 72,  93, 0,    0, nullptr, D2LAUNCH_OnCharSlotClick, nullptr, &gFontStyle16, reinterpret_cast<void*>(0x20) }, // 0x6FA62AC8
};

// Gold Selection Highlight Frame & Top Name Banner (0x6FA62B58, 0x6FA62C78)
static D2WinControlInitStrc gCtrlCharHighlight   = { D2WIN_IMAGE,   37,  178, 272, 94,  0,    0, &gpCharSelectBox, nullptr, nullptr, reinterpret_cast<void*>(D2LAUNCH_OnCharSelectKeyDown), nullptr }; // 0x6FA62B58
static D2WinControlInitStrc gCtrlCharTopBanner   = { D2WIN_TEXTBOX, 85,  78,  466, 42,  7,    2, nullptr,          nullptr, nullptr, &gFontStyle42, reinterpret_cast<void*>(0x22) }; // 0x6FA62C78

// Character Select Backdrop, Buttons, and Scrollbar (0x6FA62D68..0x6FA62E88)
static D2WinControlInitStrc gCtrlCharSelectBg    = { D2WIN_IMAGE,   0,   599, 800, 600, 0,    0,     &gpBgCharSelectExp, nullptr,                          nullptr, nullptr, nullptr }; // 0x6FA62D68
static D2WinControlInitStrc gCtrlCharSelectExit  = { D2WIN_BUTTON,  33,  571, 128, 35,  VK_ESCAPE, 5101, &gpBtnMediumBlank, D2LAUNCH_OnCharSelectExit,     nullptr, nullptr, nullptr }; // 0x6FA62D98
static D2WinControlInitStrc gCtrlCharSelectOk    = { D2WIN_BUTTON,  629, 571, 128, 35,  VK_RETURN, 5102, &gpBtnMediumBlank, D2LAUNCH_OnCharSelectOk,       nullptr, nullptr, nullptr }; // 0x6FA62DC8
static D2WinControlInitStrc gCtrlCharCreateBtn   = { D2WIN_BUTTON,  33,  528, 168, 60,  0,    10832, &gpBtnTallBlank,        D2LAUNCH_OnCharSelectCreateNew,    nullptr, nullptr, reinterpret_cast<void*>(0x40) }; // 0x6FA62DF8
static D2WinControlInitStrc gCtrlCharConvertBtn  = { D2WIN_BUTTON,  233, 528, 168, 60,  0,    22732, &gpBtnTallBlank,        D2LAUNCH_OnCharSelectConvertModal, nullptr, nullptr, reinterpret_cast<void*>(0x40) }; // 0x6FA62E28
static D2WinControlInitStrc gCtrlCharDeleteBtn   = { D2WIN_BUTTON,  433, 528, 168, 60,  0,    5272,  &gpBtnTallBlank,        D2LAUNCH_OnCharSelectDeleteModal,  nullptr, nullptr, reinterpret_cast<void*>(0x40) }; // 0x6FA62E58
static D2WinControlInitStrc gCtrlCharScrollBar   = { D2WIN_TEXTBOX, 564, 457, 34,  371, 0,    0,     &gpJoinGameScrollBars,  nullptr,                           &gCharScrollTrack, &gFontStyleFormal12, reinterpret_cast<void*>(5) }; // 0x6FA62E88

// Delete Character Confirmation Modal (0x6FA62BA8..0x6FA62C48)
static D2WinControlInitStrc gCtrlDeletePopupBg   = { D2WIN_IMAGE,   268, 350, 264, 176, 0,    0,     &gpPopUpOKCancel2,  nullptr,                  nullptr, nullptr, nullptr }; // 0x6FA62BA8
static D2WinControlInitStrc gCtrlDeleteBtnNo     = { D2WIN_BUTTON,  281, 337, 96,  32,  VK_ESCAPE, 5167, &gpBtnCancelBlank, D2LAUNCH_OnDeleteModalNo, nullptr, nullptr, nullptr }; // 0x6FA62BD8
static D2WinControlInitStrc gCtrlDeleteBtnYes    = { D2WIN_BUTTON,  421, 337, 96,  32,  0,    5166,  &gpBtnCancelBlank,  D2LAUNCH_OnDeleteModalYes,nullptr, nullptr, nullptr }; // 0x6FA62C08
static D2WinControlInitStrc gCtrlDeletePrompt    = { D2WIN_TEXTBOX, 268, 320, 264, 120, 20,   20,    nullptr,            nullptr,                  nullptr, &gFontStyleFormal12, reinterpret_cast<void*>(6) }; // 0x6FA62C38

// Convert to Expansion Confirmation Modal (0x6FA62CB8..0x6FA62D38)
static D2WinControlInitStrc gCtrlConvertPopupBg  = { D2WIN_IMAGE,   268, 350, 264, 176, 0,    0,     &gpPopUpOKCancel2,  nullptr,                   nullptr, nullptr, nullptr }; // 0x6FA62CB8
static D2WinControlInitStrc gCtrlConvertBtnNo    = { D2WIN_BUTTON,  281, 337, 96,  32,  VK_ESCAPE, 5167, &gpBtnCancelBlank, D2LAUNCH_OnConvertModalNo, nullptr, nullptr, nullptr }; // 0x6FA62CE8
static D2WinControlInitStrc gCtrlConvertBtnYes   = { D2WIN_BUTTON,  421, 337, 96,  32,  0,    5166,  &gpBtnCancelBlank,  D2LAUNCH_OnConvertModalYes,nullptr, nullptr, nullptr }; // 0x6FA62D18
static D2WinControlInitStrc gCtrlConvertPrompt   = { D2WIN_TEXTBOX, 268, 310, 264, 120, 20,   20,    nullptr,            nullptr,                   nullptr, &gFontStyle8Gold, reinterpret_cast<void*>(6) }; // 0x6FA62D38

// Difficulty Selection Modal (0x6FA62EB8..0x6FA62FA8)
static D2WinControlInitStrc gCtrlDiffNormalBtn   = { D2WIN_BUTTON,  264, 297, 272, 35,  0,    10018, &gpBtnWideBlank,    D2LAUNCH_OnDifficultyNormal,    nullptr, nullptr, nullptr }; // 0x6FA62EB8
static D2WinControlInitStrc gCtrlDiffNightmBtn   = { D2WIN_BUTTON,  264, 340, 272, 35,  0,    10017, &gpBtnWideBlank,    D2LAUNCH_OnDifficultyNightmare, nullptr, nullptr, nullptr }; // 0x6FA62EE8
static D2WinControlInitStrc gCtrlDiffHellBtn     = { D2WIN_BUTTON,  264, 383, 272, 35,  0,    10016, &gpBtnWideBlank,    D2LAUNCH_OnDifficultyHell,      nullptr, nullptr, nullptr }; // 0x6FA62F18
static D2WinControlInitStrc gCtrlDiffTitle       = { D2WIN_TEXTBOX, 264, 260, 272, 35,  0,    0,     nullptr,            nullptr,                        nullptr, &gFontStyle24, reinterpret_cast<void*>(2) }; // 0x6FA62F48
static D2WinControlInitStrc gCtrlDiffPopupBg     = { D2WIN_IMAGE,   237, 400, 326, 200, 0,    0,     &gpDifficultyLevels,nullptr,                        nullptr, nullptr, nullptr }; // 0x6FA62F78
static D2WinControlInitStrc gCtrlDiffEscBtn      = { D2WIN_BUTTON,  0,   0,   0,   0,   VK_ESCAPE, 0, &gpBtnWideBlank,   D2LAUNCH_OnDifficultyCancel,    nullptr, nullptr, nullptr }; // 0x6FA62FA8

// ============================================================================
// 1.12a Character Select Globals (0x6FA64DB4..0x6FA65ED4)
// ============================================================================
static int32_t gnSelectedCharIndex                 = 0;       // 0x6FA64DB4
static int32_t gnVisibleSlotCount                  = 8;       // 0x6FA64DC0
static int32_t gnSlotRowY[8]                       = {};      // 0x6FA656E8
static D2WinTextBoxStrc* gpCharSlotTextBox[8]      = {};      // 0x6FA6570C
static D2WinTextBoxStrc* gpCharSlotRightBox[8]     = {};      // 0x6FA65830
static D2WinTextBoxStrc* gpCharSelectTitleBox      = nullptr; // 0x6FA65850
static D2WinButtonStrc* gpBtnCharSelectExit        = nullptr; // 0x6FA65958
static D2WinButtonStrc* gpBtnCharSelectOk          = nullptr; // 0x6FA656E4
static D2WinButtonStrc* gpBtnCreateNewChar         = nullptr; // 0x6FA65A84
static D2WinButtonStrc* gpBtnConvertExp            = nullptr; // 0x6FA65970
static D2WinButtonStrc* gpBtnDeleteChar            = nullptr; // 0x6FA656E0
static D2WinTextBoxStrc* gpCharScrollBox           = nullptr; // 0x6FA65A78
static D2WinImageStrc* gpCharHighlightImg          = nullptr; // 0x6FA65A88
static D2LaunchCharNodeStrc* gpCharListHead        = nullptr; // 0x6FA65EB0
static int32_t gnCharCount                         = 0;       // 0x6FA65EB8
static int32_t gnCharListScrollOffset              = 0;       // 0x6FA65EBC
static int32_t gnPrevScrollStep                    = 0;       // 0x6FA65EC0
static DWORD gdwLastSlotClickTick                  = 0;       // 0x6FA6595C

D2LaunchCharNodeStrc* D2LAUNCH_GetCharListHead()
{
	return gpCharListHead;
}

static D2LaunchCharNodeStrc* D2LAUNCH_GetCharNodeByIndex(int32_t nIndex)
{
	if (nIndex < 0)
	{
		return nullptr;
	}
	int32_t i = 0;
	for (D2LaunchCharNodeStrc* pCur = gpCharListHead; pCur != nullptr; pCur = pCur->pNext, ++i)
	{
		if (i == nIndex)
		{
			return pCur;
		}
	}
	return nullptr;
}

// 1.12a: 0x6FA4B7B0
const Unicode* __fastcall D2LAUNCH_GetClassDisplayName(uint8_t nCharClass)
{
	static const int16_t kClassNameIds[7] = { 0x0FAB, 0x0FAA, 0x0FA9, 0x0FA8, 0x0FA7, 0x2771, 0x2772 };
	if (nCharClass < 7)
	{
		return D2LANG_GetStringFromTblIndex(kClassNameIds[nCharClass]);
	}
	return D2LANG_GetStringFromTblIndex(0x0FAB);
}

// 1.12a: 0x6FA4C7D0
void __stdcall D2LAUNCH_FreeCharList()
{
	D2Win_10146();
	D2LaunchCharNodeStrc* pCur = gpCharListHead;
	while (pCur)
	{
		D2LaunchCharNodeStrc* pNext = pCur->pNext;
		pCur->pCompUnit = nullptr;
		D2_FREE(pCur);
		pCur = pNext;
	}
	gpCharListHead = nullptr;
	gnCharCount = 0;
}

// 1.12a: 0x6FA4E050
BOOL __stdcall D2LAUNCH_AddCharNode(
	const char* szCharName,
	const char* szRealmTitle,
	const D2CharacterPreviewInfoStrc* pPreview,
	int32_t nExpireSeconds,
	uint32_t dwLastWriteLow,
	uint32_t dwLastWriteHigh)
{
	if (!szCharName || !pPreview)
	{
		return FALSE;
	}

	D2LaunchCharNodeStrc* pNode = D2_CALLOC_STRC(D2LaunchCharNodeStrc);
	if (!pNode)
	{
		return FALSE;
	}

	SStrCopy(pNode->szCharName, szCharName, sizeof(pNode->szCharName));
	if (szRealmTitle && szRealmTitle[0])
	{
		Unicode::win2Unicode(pNode->wszRealmName, szRealmTitle, 64);
	}
	pNode->tPreview = *pPreview;
	pNode->nExpireSeconds = nExpireSeconds;
	pNode->dwLastWriteLow = dwLastWriteLow;
	pNode->dwLastWriteHigh = dwLastWriteHigh;

	const uint8_t* pRaw = reinterpret_cast<const uint8_t*>(pPreview);
	pNode->wPreviewVersion = static_cast<uint16_t>(FOG_Decode14BitsFromString(const_cast<uint16_t*>(reinterpret_cast<const uint16_t*>(pRaw))));
	pRaw += 2;
	for (int i = 0; i < 11; ++i)
	{
		pNode->nAppearanceComponents[i] = *pRaw++;
	}
	for (int i = 11; i < 16; ++i)
	{
		pNode->nAppearanceComponents[i] = 0xFF;
	}
	pNode->nCharClass = static_cast<uint8_t>((*pRaw++) - 1);
	for (int i = 0; i < 11; ++i)
	{
		pNode->nAppearanceColors[i] = *pRaw++;
	}
	for (int i = 11; i < 16; ++i)
	{
		pNode->nAppearanceColors[i] = 0xFF;
	}
	pNode->nCharLevel = *pRaw++;
	pNode->wSaveFlags = static_cast<uint16_t>(FOG_Decode14BitsFromString(const_cast<uint16_t*>(reinterpret_cast<const uint16_t*>(pRaw))));
	pRaw += 2;
	pNode->wGuildFlags = static_cast<uint16_t>(FOG_Decode14BitsFromString(const_cast<uint16_t*>(reinterpret_cast<const uint16_t*>(pRaw))));
	pRaw += 2;
	pNode->nGuildEmblemBgColor = (pRaw[0] == 0xFF) ? 0 : pRaw[0];
	pNode->nGuildEmblemFgColor = (pRaw[1] == 0xFF) ? 0 : pRaw[1];
	pNode->nGuildEmblemType    = (pRaw[2] == 0xFF) ? 0 : pRaw[2];
	std::memcpy(pNode->szGuildTag, pRaw + 3, 3);
	pNode->szGuildTag[3] = '\0';

	// Determine avatar unit index & stance (0x6FA4E243..0x6FA4E2A2: alive HC stands in PLRMODE_NEUTRAL=1, dead HC shows hooded ghost!)
	unsigned int nAvatarUnitIdx = pNode->nCharClass;
	int nStanceMode = 5; // PLRMODE_TNEUTRAL
	if (pNode->wSaveFlags & 0x04u)
	{
		if (pNode->wSaveFlags & 0x08u)
		{
			nAvatarUnitIdx = 8;
			nStanceMode = 5;
		}
		else if (FOG_IsExpansion() || pNode->nCharClass < 5)
		{
			nStanceMode = 1; // PLRMODE_NEUTRAL (Hardcore combat stance)
		}
	}

	D2CompositeUnitStrc* pComp = D2Win_10144(
		nAvatarUnitIdx,
		nStanceMode,
		pNode->nAppearanceComponents,
		pNode->nAppearanceColors);
	if (!pComp)
	{
		std::memset(pNode->nAppearanceComponents, 0, sizeof(pNode->nAppearanceComponents));
		std::memset(pNode->nAppearanceColors, 0xFF, sizeof(pNode->nAppearanceColors));
		pNode->nAppearanceComponents[1] = 1;
		pComp = D2Win_10144(7, 5, pNode->nAppearanceComponents, pNode->nAppearanceColors);
	}
	pNode->pCompUnit = pComp;
	if (pComp)
	{
		D2Win_10149(pComp, 0);
		D2Win_10151(pComp, 0);
		D2Win_10143(pComp, 5);
		D2Win_10156(pComp, 0);
	}

	// Insert into gpCharListHead sorted by LastWriteTime descending in Expansion (0x6FA4E364..0x6FA4E466)
	if (!gpCharListHead)
	{
		gpCharListHead = pNode;
	}
	else if (FOG_IsExpansion())
	{
		D2LaunchCharNodeStrc* pPrev = nullptr;
		D2LaunchCharNodeStrc* pCur = gpCharListHead;
		while (pCur)
		{
			if (dwLastWriteHigh > pCur->dwLastWriteHigh ||
				(dwLastWriteHigh == pCur->dwLastWriteHigh && dwLastWriteLow > pCur->dwLastWriteLow))
			{
				break;
			}
			pPrev = pCur;
			pCur = pCur->pNext;
		}
		if (!pPrev)
		{
			pNode->pNext = gpCharListHead;
			gpCharListHead = pNode;
		}
		else
		{
			pNode->pNext = pPrev->pNext;
			pPrev->pNext = pNode;
		}
	}
	else
	{
		D2LaunchCharNodeStrc* pTail = gpCharListHead;
		while (pTail->pNext)
		{
			pTail = pTail->pNext;
		}
		pTail->pNext = pNode;
	}

	++gnCharCount;

	// Unlock AllowHardcore if any save has beaten Normal or is Hardcore (0x6FA4E3E5..0x6FA4E433)
	const uint8_t nProg = static_cast<uint8_t>((pNode->wSaveFlags >> 8) & 0x1Fu);
	const uint8_t nNormalBeat = (pNode->wSaveFlags & 0x20u) ? 5 : 4;
	if (nProg >= nNormalBeat || (pNode->wSaveFlags & 0x04u) != 0)
	{
		if (!gdwAllowHardcore)
		{
			SRegSaveValue("Diablo II", "AllowHardcore", 0, 1);
		}
		gdwAllowHardcore = 1;
	}

	return TRUE;
}

// 1.12a: 0x6FA4E5D0
BOOL __stdcall D2LAUNCH_EnumerateLocalSavedCharacters()
{
	D2LAUNCH_FreeCharList();

	char szSaveDir[0x400] = {};
	char szPattern[0x600] = {};
	FOG_GetSavePath(szSaveDir, sizeof(szSaveDir));
	std::snprintf(szPattern, sizeof(szPattern), "%s*.d2s", szSaveDir);

	WIN32_FIND_DATAA tFind = {};
	HANDLE hFind = FindFirstFileA(szPattern, &tFind);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return TRUE;
	}

	do
	{
		char szBaseName[64] = {};
		size_t k = 0;
		while (tFind.cFileName[k] != '\0' && tFind.cFileName[k] != '.' && k + 1 < sizeof(szBaseName))
		{
			szBaseName[k] = tFind.cFileName[k];
			++k;
		}
		szBaseName[k] = '\0';

		const size_t nNameLen = std::strlen(szBaseName);
		if (nNameLen >= 2 && nNameLen < 16 &&
			szBaseName[nNameLen - 1] != '-' && szBaseName[0] != '_')
		{
			D2CharacterPreviewInfoStrc tPreview = {};
			if (D2LAUNCH_ReadSavePreview(&tPreview, szBaseName))
			{
				D2LAUNCH_AddCharNode(
					szBaseName,
					nullptr,
					&tPreview,
					0,
					tFind.ftLastWriteTime.dwLowDateTime,
					tFind.ftLastWriteTime.dwHighDateTime);
			}
		}
	} while (FindNextFileA(hFind, &tFind));

	FindClose(hFind);
	return TRUE;
}

// 1.12a: 0x6FA4C870 / 0x6FA4CF70
static void D2LAUNCH_PopulateCharSelectSlots()
{
	for (int i = 0; i < 8; ++i)
	{
		if (gpCharSlotTextBox[i])
		{
			D2Win_10047(gpCharSlotTextBox[i]);
		}
	}

	// Update Top Selected Character Name Banner (0x6FA62C78)
	if (gpCharSelectTitleBox)
	{
		D2Win_10047(gpCharSelectTitleBox);
		if (D2LaunchCharNodeStrc* pSel = D2LAUNCH_GetCharNodeByIndex(gnSelectedCharIndex))
		{
			D2Win_10046(gpCharSelectTitleBox, pSel->szCharName, 4);
		}
	}

	int32_t nIdx = 0;
	for (D2LaunchCharNodeStrc* pNode = gpCharListHead; pNode != nullptr; pNode = pNode->pNext, ++nIdx)
	{
		const int32_t nSlot = nIdx - gnCharListScrollOffset;
		if (nSlot < 0 || nSlot >= gnVisibleSlotCount)
		{
			if (pNode->pCompUnit)
			{
				D2Win_10156(pNode->pCompUnit, 0);
			}
			continue;
		}

		const int32_t nCol = nSlot & 1;
		const int32_t nRow = nSlot / 2;
		const int32_t nBoxX = nCol ? 309 : 37;
		const int32_t nBoxY = 178 + nRow * 93;

		if (pNode->pCompUnit)
		{
			D2Win_10152(pNode->pCompUnit, nBoxX + 30);
			D2Win_10153(pNode->pCompUnit, nBoxY - 13);
			D2Win_10156(pNode->pCompUnit, (nIdx == gnSelectedCharIndex) ? 5 : 1);
		}

		D2WinTextBoxStrc* pSlotBox = gpCharSlotTextBox[nSlot];
		if (!pSlotBox)
		{
			continue;
		}

		D2Win_10208(1);

		const bool bHardcore  = (pNode->wSaveFlags & 0x04u) != 0;
		const bool bExpansion = (pNode->wSaveFlags & 0x20u) != 0;
		const int32_t nNameColor = bHardcore ? 1 : 4;
		const int32_t nProgression = (pNode->wSaveFlags >> 8) & 0x1F;

		// Line 0 (optional): Character Title ("Slayer", "Champion", "Patriarch", "Matriarch", "Destroyer", "Conqueror", "Guardian")
		const char* szTitle = D2Win_10161(pNode->nCharClass, nProgression, bHardcore ? 1 : 0, bExpansion ? 1 : 0);
		if (szTitle && szTitle[0])
		{
			D2Win_10046(pSlotBox, szTitle, nNameColor);
		}

		// Line 1: Character Name
		D2Win_10046(pSlotBox, pNode->szCharName, nNameColor);

		// Line 2: Localized "Level %d <Class>" (0x0FD9 + 0x6FA4B7B0)
		Unicode wszLevelFmt[128] = {};
		const Unicode* wszLvlWord = D2LANG_GetStringFromTblIndex(0x0FD9);
		const Unicode* wszClsWord = D2LAUNCH_GetClassDisplayName(pNode->nCharClass);
		if (wszLvlWord && wszClsWord)
		{
			char szLvlWord[32] = {};
			char szClsWord[64] = {};
			char szCombined[128] = {};
			Unicode::unicode2Win(szLvlWord, wszLvlWord, sizeof(szLvlWord));
			Unicode::unicode2Win(szClsWord, wszClsWord, sizeof(szClsWord));
			std::snprintf(szCombined, sizeof(szCombined), "%s %u %s", szLvlWord, pNode->nCharLevel, szClsWord);
			Unicode::win2Unicode(wszLevelFmt, szCombined, 128);
			D2Win_10051(pSlotBox, wszLevelFmt, 0, 0, 0);
		}

		// Line 3: "EXPANSION CHARACTER" (0x58CB = 22731, green = 2)
		if (bExpansion)
		{
			if (const Unicode* wszExp = D2LANG_GetStringFromTblIndex(0x58CB))
			{
				D2Win_10051(pSlotBox, wszExp, 2, 0, 0);
			}
		}

		D2Win_10208(0);
	}
}

// 1.12a: 0x6FA4B540
static void __stdcall D2LAUNCH_StepCharScrollBar(int32_t nDeltaSteps)
{
	if (!gpCharScrollBox)
	{
		return;
	}
	const int32_t nCur = D2Win_10192(gpCharScrollBox);
	D2Win_10193(gpCharScrollBox, nCur + nDeltaSteps);
	gnPrevScrollStep = D2Win_10192(gpCharScrollBox);
}

// 1.12a: 0x6FA4B560 / 0x6FA4C230
static void D2LAUNCH_UpdateCharSelectButtonsAndHighlight()
{
	if (gnCharCount == 0)
	{
		gnSelectedCharIndex = -1;
	}

	D2LaunchCharNodeStrc* pSel = D2LAUNCH_GetCharNodeByIndex(gnSelectedCharIndex);
	const int32_t nRelSlot = gnSelectedCharIndex - gnCharListScrollOffset;

	if (gpCharHighlightImg)
	{
		if (pSel && nRelSlot >= 0 && nRelSlot < gnVisibleSlotCount)
		{
			const int32_t nHighlightX = (nRelSlot & 1) ? 309 : 37;
			const int32_t nHighlightY = gnSlotRowY[nRelSlot];
			IMAGE_SetCoordinates(gpCharHighlightImg, nHighlightX, nHighlightY);
			D2Win_10027_CONTROL_ToggleThirdFlag(&gpCharHighlightImg->controlHeader, 1);
		}
		else
		{
			D2Win_10027_CONTROL_ToggleThirdFlag(&gpCharHighlightImg->controlHeader, 0);
		}
	}

	if (gpBtnCharSelectExit)
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnCharSelectExit->controlHeader, 1);
	}
	if (gpBtnCreateNewChar)
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnCreateNewChar->controlHeader, 1);
	}

	if (!pSel)
	{
		if (gpBtnCharSelectOk) D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnCharSelectOk->controlHeader, 0);
		if (gpBtnDeleteChar)   D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnDeleteChar->controlHeader, 0);
		if (gpBtnConvertExp)   D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnConvertExp->controlHeader, 0);
		return;
	}

	if (gpBtnCharSelectOk)
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnCharSelectOk->controlHeader, 1);
	}
	if (gpBtnDeleteChar)
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnDeleteChar->controlHeader, 1);
	}
	if (gpBtnConvertExp)
	{
		const BOOL bCanConvert = (FOG_IsExpansion() && !(pSel->wSaveFlags & 0x20u)) ? 1 : 0;
		D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnConvertExp->controlHeader, bCanConvert);
	}
}

// 1.12a: 0x6FA4DBC0
static void __stdcall D2LAUNCH_OnCharScroll(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (!gpCharScrollBox)
	{
		return;
	}
	const int32_t nNewStep = D2Win_10192(gpCharScrollBox);
	if (nNewStep == gnPrevScrollStep)
	{
		return;
	}

	const int32_t nDelta = nNewStep - gnPrevScrollStep;
	gnCharListScrollOffset += nDelta * 2;
	if (gnCharListScrollOffset < 0)
	{
		gnCharListScrollOffset = 0;
	}
	if (gnCharCount > 0 && gnCharListScrollOffset >= gnCharCount)
	{
		gnCharListScrollOffset = (gnCharCount - 1) & ~1;
	}
	gnPrevScrollStep = nNewStep;

	D2LAUNCH_PopulateCharSelectSlots();
	D2LAUNCH_UpdateCharSelectButtonsAndHighlight();
}

static void D2LAUNCH_EnsureSelectedCharScrolledIntoView()
{
	if (gnSelectedCharIndex < 0 || gnCharCount <= 0)
	{
		return;
	}
	if (gnSelectedCharIndex < gnCharListScrollOffset)
	{
		const int32_t nNewOffset = gnSelectedCharIndex & ~1;
		const int32_t nStepDelta = (nNewOffset - gnCharListScrollOffset) / 2;
		gnCharListScrollOffset = nNewOffset;
		D2LAUNCH_StepCharScrollBar(nStepDelta);
	}
	else if (gnSelectedCharIndex >= gnCharListScrollOffset + gnVisibleSlotCount)
	{
		const int32_t nRowStart = gnSelectedCharIndex & ~1;
		const int32_t nNewOffset = nRowStart - (gnVisibleSlotCount - 2);
		const int32_t nStepDelta = (nNewOffset - gnCharListScrollOffset) / 2;
		gnCharListScrollOffset = nNewOffset;
		D2LAUNCH_StepCharScrollBar(nStepDelta);
	}
}

// 1.12a: 0x6FA4D980 / 0x6FA4DF40
static int __stdcall D2LAUNCH_OnCharSelectKeyDown(SMSGHANDLER_PARAMS* pMsg)
{
	if (!pMsg || gbModalOpen || gnCharCount <= 0)
	{
		return 0;
	}
	if (pMsg->nMessage == WM_CHAR)
	{
		return 0;
	}

	int32_t nTarget = gnSelectedCharIndex;
	switch (pMsg->wParam)
	{
	case VK_DOWN:
		if (nTarget + 2 < gnCharCount) nTarget += 2;
		break;
	case VK_UP:
		if (nTarget - 2 >= 0) nTarget -= 2;
		break;
	case VK_RIGHT:
		if ((nTarget & 1) == 0 && nTarget + 1 < gnCharCount) ++nTarget;
		break;
	case VK_LEFT:
		if ((nTarget & 1) == 1 && nTarget - 1 >= 0) --nTarget;
		break;
	case VK_NEXT: // PageDown
		nTarget += gnVisibleSlotCount;
		if (nTarget >= gnCharCount) nTarget = gnCharCount - 1;
		break;
	case VK_PRIOR: // PageUp
		nTarget -= gnVisibleSlotCount;
		if (nTarget < 0) nTarget = 0;
		break;
	case VK_HOME:
		nTarget = 0;
		break;
	case VK_END:
		nTarget = gnCharCount - 1;
		break;
	default:
		return 0;
	}

	if (nTarget != gnSelectedCharIndex)
	{
		gnSelectedCharIndex = nTarget;
		D2LAUNCH_EnsureSelectedCharScrolledIntoView();
		D2LAUNCH_PopulateCharSelectSlots();
		D2LAUNCH_UpdateCharSelectButtonsAndHighlight();
	}
	return 1;
}

// ============================================================================
// Character Select Modals: Delete, Convert to Expansion, Difficulty (0x6FA4BD40..0x6FA4F850)
// ============================================================================
static void D2LAUNCH_SetUnderlyingSelectButtonsEnabled(int bEnabled)
{
	if (gpBtnCharSelectOk)   D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnCharSelectOk->controlHeader, bEnabled);
	if (gpBtnCharSelectExit) D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnCharSelectExit->controlHeader, bEnabled);
	if (gpBtnCreateNewChar)  D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnCreateNewChar->controlHeader, bEnabled);
	if (gpBtnDeleteChar)     D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnDeleteChar->controlHeader, bEnabled);
	if (gpBtnConvertExp)     D2Win_10025_CONTROL_ToggleFirstFlag(&gpBtnConvertExp->controlHeader, bEnabled);
}

// 1.12a: 0x6FA4BD40
static int __stdcall D2LAUNCH_OnDeleteModalNo(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyModalControls();
	D2LAUNCH_UpdateCharSelectButtonsAndHighlight();
	return 1;
}

// 1.12a: 0x6FA4F4F0
static int __stdcall D2LAUNCH_OnDeleteModalYes(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyModalControls();

	D2LaunchCharNodeStrc* pSel = D2LAUNCH_GetCharNodeByIndex(gnSelectedCharIndex);
	if (!pSel)
	{
		D2LAUNCH_UpdateCharSelectButtonsAndHighlight();
		return 1;
	}

	D2LAUNCH_DeleteSaveFiles(pSel->szCharName);

	if (!D2LAUNCH_HasSavedCharacters())
	{
		D2LAUNCH_FreeCharList();
		if (gnLaunchMode == 2)
		{
			D2LAUNCH_EnterTcpIpMenu();
		}
		else
		{
			D2LAUNCH_EnterMainMenu(0);
		}
		return 1;
	}

	D2LAUNCH_EnumerateLocalSavedCharacters();
	if (gnSelectedCharIndex >= gnCharCount)
	{
		gnSelectedCharIndex = (gnCharCount > 0) ? (gnCharCount - 1) : 0;
	}
	if (gpCharScrollBox)
	{
		const int32_t nExtra = gnCharCount - gnVisibleSlotCount;
		const int32_t nMaxSteps = (nExtra > 0) ? ((nExtra + 1) / 2) : 0;
		D2Win_10191(gpCharScrollBox, nMaxSteps);
	}
	D2LAUNCH_EnsureSelectedCharScrolledIntoView();
	D2LAUNCH_PopulateCharSelectSlots();
	D2LAUNCH_UpdateCharSelectButtonsAndHighlight();
	return 1;
}

// 1.12a: 0x6FA4CF60 -> 0x6FA4BD80
static int __stdcall D2LAUNCH_OnCharSelectDeleteModal(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (gbModalOpen || !D2LAUNCH_GetCharNodeByIndex(gnSelectedCharIndex))
	{
		return 0;
	}

	D2LAUNCH_DestroyModalControls();
	gbModalOpen = 1;
	D2LAUNCH_SetUnderlyingSelectButtonsEnabled(0);

	if (D2WinControlStrc* pBg = D2LAUNCH_CreateModalControl(&gCtrlDeletePopupBg))
	{
		D2Win_10033(pBg, 1);
	}
	if (D2WinControlStrc* pNo = D2LAUNCH_CreateModalControl(&gCtrlDeleteBtnNo))
	{
		D2Win_10031(pNo);
	}
	D2LAUNCH_CreateModalControl(&gCtrlDeleteBtnYes);
	if (D2WinTextBoxStrc* pPrompt = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateModalControl(&gCtrlDeletePrompt)))
	{
		D2Win_10051(pPrompt, D2LANG_GetStringFromTblIndex(0x142B), 4, 0, 0);
	}
	return 1;
}

// 1.12a: 0x6FA4BEB0
static int __stdcall D2LAUNCH_OnConvertModalNo(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyModalControls();
	D2LAUNCH_UpdateCharSelectButtonsAndHighlight();
	return 1;
}

// 1.12a: 0x6FA4F5E0
static int __stdcall D2LAUNCH_OnConvertModalYes(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyModalControls();

	D2LaunchCharNodeStrc* pSel = D2LAUNCH_GetCharNodeByIndex(gnSelectedCharIndex);
	if (pSel && !(pSel->wSaveFlags & 0x20u))
	{
		if (D2LAUNCH_ConvertSaveToExpansion(pSel->szCharName))
		{
			pSel->wSaveFlags |= 0x20u;
			D2LAUNCH_ReadSavePreview(&pSel->tPreview, pSel->szCharName);
		}
	}

	D2LAUNCH_PopulateCharSelectSlots();
	D2LAUNCH_UpdateCharSelectButtonsAndHighlight();
	return 1;
}

// 1.12a: 0x6FA4BF00
static int __stdcall D2LAUNCH_OnCharSelectConvertModal(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LaunchCharNodeStrc* pSel = D2LAUNCH_GetCharNodeByIndex(gnSelectedCharIndex);
	if (gbModalOpen || !pSel || (pSel->wSaveFlags & 0x20u))
	{
		return 0;
	}

	D2LAUNCH_DestroyModalControls();
	gbModalOpen = 1;
	D2LAUNCH_SetUnderlyingSelectButtonsEnabled(0);

	if (D2WinControlStrc* pBg = D2LAUNCH_CreateModalControl(&gCtrlConvertPopupBg))
	{
		D2Win_10033(pBg, 1);
	}
	if (D2WinControlStrc* pNo = D2LAUNCH_CreateModalControl(&gCtrlConvertBtnNo))
	{
		D2Win_10031(pNo);
	}
	D2LAUNCH_CreateModalControl(&gCtrlConvertBtnYes);
	if (D2WinTextBoxStrc* pPrompt = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateModalControl(&gCtrlConvertPrompt)))
	{
		static const Unicode kBlank[2] = { Unicode(' '), Unicode(0) };
		D2Win_10051(pPrompt, D2LANG_GetStringFromTblIndex(0x58CE), 4, 0, 0);
		D2Win_10051(pPrompt, kBlank, 4, 0, 0);
		D2Win_10051(pPrompt, D2LANG_GetStringFromTblIndex(0x58CF), 4, 0, 0);
		D2Win_10051(pPrompt, kBlank, 4, 0, 0);
		D2Win_10051(pPrompt, D2LANG_GetStringFromTblIndex(0x58D0), 4, 0, 0);
	}
	return 1;
}

// 1.12a: 0x6FA52370
static int __stdcall D2LAUNCH_OnDifficultyCancel(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_DestroyModalControls();
	D2LAUNCH_UpdateCharSelectButtonsAndHighlight();
	return 1;
}

// 1.12a: 0x6FA4F7B0
static int D2LAUNCH_ConfirmDifficultyAndEnter(uint8_t nDifficulty)
{
	D2LaunchCharNodeStrc* pSel = D2LAUNCH_GetCharNodeByIndex(gnSelectedCharIndex);
	if (!pSel || !gpD2Config)
	{
		return 0;
	}

	gpD2Config->nArenaDifficulty = nDifficulty;
	gpD2Config->tCharPreviewInfo = pSel->tPreview;
	D2LAUNCH_DestroyModalControls();

	char szRealm[64] = {};
	if (pSel->wszRealmName[0])
	{
		Unicode::unicode2Win(szRealm, pSel->wszRealmName, sizeof(szRealm));
	}
	D2LAUNCH_EnterSelectedCharacter(pSel->szCharName, szRealm, pSel->nCharClass, pSel->wSaveFlags);
	return 1;
}

static int __stdcall D2LAUNCH_OnDifficultyNormal(SMSGHANDLER_PARAMS* pMsg)    { (void)pMsg; return D2LAUNCH_ConfirmDifficultyAndEnter(0); } // 0x6FA4FBD0
static int __stdcall D2LAUNCH_OnDifficultyNightmare(SMSGHANDLER_PARAMS* pMsg) { (void)pMsg; return D2LAUNCH_ConfirmDifficultyAndEnter(1); } // 0x6FA4FBC0
static int __stdcall D2LAUNCH_OnDifficultyHell(SMSGHANDLER_PARAMS* pMsg)      { (void)pMsg; return D2LAUNCH_ConfirmDifficultyAndEnter(2); } // 0x6FA4FBB0

// 1.12a: 0x6FA4C070
static void D2LAUNCH_OpenDifficultySelectModal(BOOL bEnableHell)
{
	D2LAUNCH_DestroyModalControls();
	gbModalOpen = 1;
	D2LAUNCH_SetUnderlyingSelectButtonsEnabled(0);

	if (D2WinControlStrc* pBg = D2LAUNCH_CreateModalControl(&gCtrlDiffPopupBg))
	{
		D2Win_10033(pBg, 1);
	}
	if (D2WinTextBoxStrc* pTitle = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateModalControl(&gCtrlDiffTitle)))
	{
		D2Win_10051(pTitle, D2LANG_GetStringFromTblIndex(0x2723), 7, 0, 0);
	}
	D2LAUNCH_CreateModalControl(&gCtrlDiffNormalBtn);
	D2LAUNCH_CreateModalControl(&gCtrlDiffNightmBtn);
	if (D2WinControlStrc* pHell = D2LAUNCH_CreateModalControl(&gCtrlDiffHellBtn))
	{
		D2Win_10025_CONTROL_ToggleFirstFlag(pHell, bEnableHell ? 1 : 0);
	}
	if (D2WinControlStrc* pEsc = D2LAUNCH_CreateModalControl(&gCtrlDiffEscBtn))
	{
		D2Win_10027_CONTROL_ToggleThirdFlag(pEsc, 0);
		D2Win_10031(pEsc);
	}
}

// 1.12a: 0x6FA4F850
static int __stdcall D2LAUNCH_OnCharSelectOk(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	if (gbModalOpen || !gpD2Config)
	{
		return 1;
	}

	D2LaunchCharNodeStrc* pSel = D2LAUNCH_GetCharNodeByIndex(gnSelectedCharIndex);
	if (!pSel)
	{
		return 0;
	}

	// Dead Hardcore character check (0x6FA4F8D2: (wSaveFlags & 0x0C) == 0x0C -> string 0x14B8 = 5304)
	if ((pSel->wSaveFlags & 0x0Cu) == 0x0Cu)
	{
		D2LAUNCH_ShowModalAlert(D2LANG_GetStringFromTblIndex(0x14B8), nullptr, 1);
		return 1;
	}

	gpD2Config->nArenaDifficulty = 0;

	// Difficulty Selection Popup for Single Player or TCP/IP Host (0x6FA4F90E..0x6FA4F96B)
	if (gnLaunchMode == 0 || (gnLaunchMode == 2 && gpD2Config->dwGameType == 8))
	{
		const uint8_t nProg = static_cast<uint8_t>((pSel->wSaveFlags >> 8) & 0x1Fu);
		const bool bExp = (pSel->wSaveFlags & 0x20u) != 0;
		const uint8_t nReqNightmare = bExp ? 5 : 4;
		const uint8_t nReqHell      = bExp ? 10 : 8;
		if (nProg >= nReqNightmare)
		{
			D2LAUNCH_OpenDifficultySelectModal(nProg >= nReqHell ? TRUE : FALSE);
			return 1;
		}
	}

	gpD2Config->tCharPreviewInfo = pSel->tPreview;
	char szRealm[64] = {};
	if (pSel->wszRealmName[0])
	{
		Unicode::unicode2Win(szRealm, pSel->wszRealmName, sizeof(szRealm));
	}
	D2LAUNCH_EnterSelectedCharacter(pSel->szCharName, szRealm, pSel->nCharClass, pSel->wSaveFlags);
	return 1;
}

// 1.12a: 0x6FA4FA40
static int __stdcall D2LAUNCH_OnCharSelectExit(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_FreeCharList();
	if (gnLaunchMode == 2)
	{
		D2LAUNCH_EnterTcpIpMenu();
	}
	else
	{
		D2LAUNCH_EnterMainMenu(0);
	}
	return 1;
}

// 1.12a: 0x6FA4FA90
static int __stdcall D2LAUNCH_OnCharSelectCreateNew(SMSGHANDLER_PARAMS* pMsg)
{
	(void)pMsg;
	D2LAUNCH_FreeCharList();
	D2LAUNCH_EnterCharCreate();
	return 1;
}

// 1.12a: 0x6FA4FAD0
static int __stdcall D2LAUNCH_OnCharSlotClick(SMSGHANDLER_PARAMS* pMsg)
{
	if (!pMsg || gbModalOpen)
	{
		return 0;
	}

	D2WinControlStrc* pClicked = reinterpret_cast<D2WinControlStrc*>(pMsg->hWindow);
	int32_t nSlot = -1;
	for (int i = 0; i < 8; ++i)
	{
		if ((gpCharSlotTextBox[i] && &gpCharSlotTextBox[i]->controlHeader == pClicked) ||
			(gpCharSlotRightBox[i] && &gpCharSlotRightBox[i]->controlHeader == pClicked))
		{
			nSlot = i;
			break;
		}
	}
	if (nSlot < 0)
	{
		return 0;
	}

	const int32_t nCharIdx = gnCharListScrollOffset + nSlot;
	if (!D2LAUNCH_GetCharNodeByIndex(nCharIdx))
	{
		return 0;
	}

	const DWORD dwNow = GetTickCount();
	if (gnSelectedCharIndex == nCharIdx && (dwNow - gdwLastSlotClickTick) <= GetDoubleClickTime())
	{
		gdwLastSlotClickTick = 0;
		return D2LAUNCH_OnCharSelectOk(nullptr);
	}

	gnSelectedCharIndex = nCharIdx;
	gdwLastSlotClickTick = dwNow;
	D2LAUNCH_PopulateCharSelectSlots();
	D2LAUNCH_UpdateCharSelectButtonsAndHighlight();
	return 1;
}

// ============================================================================
// Character Select Screen Entry (0x6FA4EAE0, 0x6FA4EF60)
// ============================================================================
// 1.12a: 0x6FA4EAE0
BOOL __stdcall D2LAUNCH_EnterCharSelectExpansion()
{
	D2LAUNCH_DestroyAllControls();
	D2LAUNCH_SetPalette("palette\\sky\\pal.dat", "palette\\sky\\pal.pl2");

	DWORD dwRegHc = 0;
	SRegLoadValue("Diablo II", "AllowHardcore", 0, &dwRegHc);
	gdwAllowHardcore = (dwRegHc != 0) ? 1 : 0;
#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 13
	gdwAllowHardcore = 1;
#endif

	gnCharCount = 0;
	gnCharListScrollOffset = 0;
	gnSelectedCharIndex = 0;
	gnPrevScrollStep = 0;
	gdwLastSlotClickTick = 0;
	if (gpD2Config)
	{
		gpD2Config->nArenaDifficulty = 0;
	}

	D2LAUNCH_CreateScreenControl(&gCtrlCharSelectBg);
	gpBtnCharSelectExit = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCharSelectExit));
	gpBtnCharSelectOk   = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCharSelectOk));
	gpBtnCreateNewChar  = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCharCreateBtn));
	gpBtnConvertExp     = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCharConvertBtn));
	gpBtnDeleteChar     = reinterpret_cast<D2WinButtonStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCharDeleteBtn));
	gpCharSelectTitleBox= reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCharTopBanner));

	if (gpBtnCreateNewChar) BUTTON_SetStringId(gpBtnCreateNewChar, 0x5524); // 21796 "CHARACTER"
	if (gpBtnConvertExp)    BUTTON_SetStringId(gpBtnConvertExp, 0x58CA);    // 22730 "EXPANSION"
	if (gpBtnDeleteChar)    BUTTON_SetStringId(gpBtnDeleteChar, 0x5524);    // 21796 "CHARACTER"

	if (!FOG_IsExpansion() && gpBtnConvertExp)
	{
		D2Win_10027_CONTROL_ToggleThirdFlag(&gpBtnConvertExp->controlHeader, 0);
	}

	gpCharScrollBox = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCharScrollBar));
	if (gpCharScrollBox)
	{
		D2Win_10194(gpCharScrollBox, nullptr);
		D2Win_10195(gpCharScrollBox, D2LAUNCH_OnCharScroll);
	}

	gnVisibleSlotCount = 8;
	for (int i = 0; i < 8; ++i)
	{
		gpCharSlotRightBox[i] = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCharSlotRightHit[i]));
		if (gpCharSlotRightBox[i])
		{
			D2Win_10045(gpCharSlotRightBox[i], " ");
		}

		D2Win_10208(1);
		gpCharSlotTextBox[i] = reinterpret_cast<D2WinTextBoxStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCharSlotText[i]));
		D2Win_10208(0);

		gnSlotRowY[i] = 178 + (i / 2) * 93;
	}

	gpCharHighlightImg = reinterpret_cast<D2WinImageStrc*>(D2LAUNCH_CreateScreenControl(&gCtrlCharHighlight));
	D2LAUNCH_MaybeCreateDevClientToggle(216, 571);

	D2LAUNCH_EnumerateLocalSavedCharacters();

	if (gpCharScrollBox)
	{
		const int32_t nExtra = gnCharCount - gnVisibleSlotCount;
		const int32_t nMaxSteps = (nExtra > 0) ? ((nExtra + 1) / 2) : 0;
		D2Win_10191(gpCharScrollBox, nMaxSteps);
	}

	D2LAUNCH_PopulateCharSelectSlots();
	D2LAUNCH_UpdateCharSelectButtonsAndHighlight();
	return TRUE;
}

// 1.12a: 0x6FA4EF60
BOOL __stdcall D2LAUNCH_EnterCharSelect()
{
	return D2LAUNCH_EnterCharSelectExpansion();
}
