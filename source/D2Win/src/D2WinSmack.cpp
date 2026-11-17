#include "D2WinSmack.h"

#include <algorithm>

#include <D2Gfx.h>
#include <Fog.h>

#include <D2BitManip.h>

// D2Gfx
#include <Window.h>

#include "D2WinMain.h"


char gszSmackFileName[260];    // 1.12a: 0x6F9A8CC0
int gnSmackVersion;            // 1.12a: 0x6F9A8DC4
int32_t gbSmackReloadPending;  // 1.10f: 0x6F96A8D8 | 1.12a: 0x6F9A8DC8
int32_t dword_6F96A8DC;        // 1.10f: 0x6F96A8DC (shared with D2WinTextBox)


// 1.10f: D2Win.0x6F8AF710 (#10097) | 1.12a: D2Win.0x6F8F7460 (#10187)
D2WinSmackStrc* __fastcall SMACK_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t(__stdcall* a5)(SMSGHANDLER_PARAMS*))
{
	D2WinSmackStrc* pSmack = D2_CALLOC_STRC(D2WinSmackStrc);

	pSmack->controlHeader.nType = D2WIN_SMACK;
	pSmack->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pSmack->controlHeader.field_34 = a5;
	pSmack->controlHeader.nImageX = nX;
	pSmack->controlHeader.nImageY = nY;
	pSmack->controlHeader.nWidth = nWidth;
	pSmack->controlHeader.nHeight = nHeight;
	pSmack->controlHeader.pfDraw = SMACK_Draw;
	pSmack->controlHeader.pfHandleMouseDown = SMACK_HandleMouseDown;
	pSmack->nSmackX = nX + (nWidth - 468) / 2;
	pSmack->nSmackY = nY + (nHeight - 60) / 2;

	CONTROL_AppendToList(&pSmack->controlHeader);

	return pSmack;
}

// 1.10f: D2Win.0x6F8AF7E0 (#10099) | 1.12a: D2Win.0x6F8F7320 (#10013)
int32_t __stdcall SMACK_Decode(D2WinSmackStrc* pSmack, const char* szFileName, int32_t nVersion, int32_t nUnused, int32_t nUnused2)
{
	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	strncpy_s(pSmack->szFileName, szFileName, std::size(pSmack->szFileName));
	WINDOW_CloseSmacker(&pSmack->nSmackerContext);
	WINDOW_DecodeSmacker(pSmack->szFileName, (uint8_t*)&pSmack->nSmackerContext, nVersion);
	strncpy_s(gszSmackFileName, szFileName, std::size(gszSmackFileName));
	gnSmackVersion = nVersion;
	return 1;
}

// 1.10f: D2Win.0x6F8AF860 (#10101) | 1.12a: D2Win.0x6F8F72D0 (#10044)
int32_t __stdcall SMACK_DeleteFile(D2WinSmackStrc* pSmack)
{
	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	WINDOW_CloseSmacker(&pSmack->nSmackerContext);
	if (!gbSmackReloadPending)
	{
		DeleteFileA(pSmack->szFileName);
	}

	return 1;
}

// 1.10f: D2Win.0x6F8AF8B0 (#10100) | 1.12a: D2Win.0x6F8F7280 (#10120)
int32_t __stdcall SMACK_SetCoordinates(D2WinSmackStrc* pSmack, int32_t nX, int32_t nY)
{
	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	pSmack->controlHeader.nImageX = nX;
	pSmack->controlHeader.nImageY = nY;
	pSmack->nSmackX = nX;
	pSmack->nSmackY = nY;
	return 1;
}

// 1.10f: D2Win.0x6F8AF900 (#10098) | 1.12a: D2Win.0x6F8F7420 (#10055)
int32_t __fastcall SMACK_Destroy(D2WinSmackStrc* pSmack)
{
	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	SMACK_DeleteFile(pSmack);

	return CONTROL_RemoveFromList(&pSmack->controlHeader);
}

// 1.10f: D2Win.0x6F8AF950 (#10184) | 1.12a: D2Win.0x6F8F7270 (#10071)
void __stdcall D2Win_10184()
{
	gbSmackReloadPending = 1;
}

// 1.10f: D2Win.0x6F8AF960 | 1.12a: D2Win.0x6F8F73A0
int32_t __fastcall SMACK_Draw(D2WinControlStrc* pControl)
{
	D2WinSmackStrc* pSmack = (D2WinSmackStrc*)pControl;

	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	if (!(pSmack->controlHeader.dwFlags & gdwBitMasks[2]) || !(pSmack->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	if (gbSmackReloadPending)
	{
		if (gszSmackFileName[0])
		{
			SMACK_Decode(pSmack, gszSmackFileName, gnSmackVersion, 0, 0);
		}

		gbSmackReloadPending = 0;
	}

	WINDOW_PlaySmacker(&pSmack->nSmackerContext);
	return 1;
}

// 1.10f: D2Win.0x6F8AFA40 | 1.12a: D2Win.0x6F8F7210
int32_t __stdcall SMACK_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinSmackStrc* pSmack = (D2WinSmackStrc*)pMsg->hWindow;

	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	if (!(pSmack->controlHeader.dwFlags & gdwBitMasks[2]) || !(pSmack->controlHeader.dwFlags & gdwBitMasks[0]) || !pSmack->controlHeader.field_34)
	{
		return 0;
	}

	return pSmack->controlHeader.field_34(pMsg);
}

// 1.10f: D2Win.0x6F8AFA90 (#10208)
void __fastcall D2Win_10208(int32_t a1)
{
	dword_6F96A8DC = a1;
}
