#include "D2WinEditBox.h"

#include <algorithm>

#include <D2Math.h>

#include "D2CMP.h"
#include "D2Gfx.h"
#include "D2Lang.h"
#include "Fog.h"

#include <Texture.h>
#include <Window.h>

#include <D2BitManip.h>

#include "D2WinFont.h"
#include "D2WinMain.h"
#include "D2WinPalette.h"

#pragma warning (disable : 28159)


D2WinEditBoxStrc* dword_6F8FD8B0;
int dword_6F8FD8B4;
static char gszClipboardDest_6F8FD8B8[512];
static HGLOBAL ghClipboardMem_6F8FDAC0;


//D2Win.0x6F8A6C80 (#10070)
D2WinEditBoxStrc* __fastcall D2Win_10070_EDITBOX_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t a5, int32_t a6, D2CellFileStrc* pCellFile, int32_t a8, int32_t(__stdcall* a9)(SMSGHANDLER_PARAMS*), int32_t nEditBoxFlags, int32_t a11)
{
	D2WinEditBoxStrc* pEditBox = D2_CALLOC_STRC(D2WinEditBoxStrc);

	pEditBox->controlHeader.nType = D2WIN_EDITBOX;
	pEditBox->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pEditBox->controlHeader.nImageX = nX;
	pEditBox->controlHeader.nImageY = nY;
	pEditBox->controlHeader.nWidth = nWidth;
	pEditBox->controlHeader.nHeight = nHeight;
	pEditBox->controlHeader.pCellFile = pCellFile;
	pEditBox->field_40 = a5;
	pEditBox->field_44 = a6;
	pEditBox->field_48 = 256;
	pEditBox->field_54 = -1;

	if (!(nEditBoxFlags & 8))
	{
		pEditBox->controlHeader.pfDraw = sub_6F8A82D0;
		pEditBox->controlHeader.pfHandleMouseDown = sub_6F8A9120;
	}
	else
	{
		pEditBox->controlHeader.pfDraw = sub_6F8A86C0;
		pEditBox->controlHeader.pfHandleMouseDown = sub_6F8A97D0;
	}

	pEditBox->controlHeader.pfHandleMouseUp = EDITBOX_HandleMouseUp;
	pEditBox->field_264 = a8;
	pEditBox->controlHeader.field_34 = a9;
	pEditBox->dwEditBoxFlags = nEditBoxFlags;

	if (a11)
	{
		pEditBox->eFont = *(Font*)a11;
		pEditBox->nTextColor = *(DWORD*)(a11 + 4);
	}
	else
	{
		pEditBox->eFont = D2FONT_FONT16;
		pEditBox->nTextColor = 0;
	}

	CONTROL_AppendToList(&pEditBox->controlHeader);

	return pEditBox;
}

//D2Win.0x6F8A6DC0 (#10071)
int32_t __fastcall D2Win_10071_EDITBOX_Destroy(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	return CONTROL_RemoveFromList(&pEditBox->controlHeader);
}

//D2Win.0x6F8A6E10 (#10072)
void __fastcall D2Win_10072(D2WinEditBoxStrc* pEditBox, int32_t a2)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_268 = a2;
}

//D2Win.0x6F8A6E60 (#10073)
void __fastcall D2Win_10073(D2WinEditBoxStrc* pEditBox, void(__stdcall* a2)(int32_t))
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_26C = a2;
}

//D2Win.0x6F8A6EB0 (#10083)
void __fastcall D2Win_10083(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_54 = -1;
	pEditBox->nTextLength = 0;
}

//D2Win.0x6F8A6F10 (#10082)
int32_t __fastcall D2Win_10082(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_54 = 0;
	pEditBox->nTextLength = Unicode::strlen(pEditBox->wszText);

	return pEditBox->nTextLength;
}

//D2Win.0x6F8A6F70 (#10074)
int32_t __fastcall D2Win_10074(D2WinEditBoxStrc* pEditBox, uint8_t a2)
{
	uint8_t nChar = a2;
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	const uint32_t dwFlags = pEditBox->controlHeader.dwFlags;
	if (!(dwFlags & gdwBitMasks[2]) || !(dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	if (a2 == 9 && (pEditBox->dwEditBoxFlags & 4))
	{
		if (pEditBox->field_268)
		{
			++dword_6F8FE254;
			reinterpret_cast<void(__stdcall*)(D2WinEditBoxStrc*, int32_t, uint8_t*)>(pEditBox->field_268)(pEditBox, pEditBox->field_25C, &nChar);
			--dword_6F8FE254;
		}
		return 0;
	}

	const bool bPrintable = (D2Win_10128(a2) != 0);
	if (!bPrintable
		&& (!(pEditBox->dwEditBoxFlags & 0x20) || a2 != 255)
		&& (!(pEditBox->dwEditBoxFlags & 8) || nChar != 13)
		&& (!(pEditBox->dwEditBoxFlags & 0x10) || nChar != 14)
		&& nChar != 22 && nChar != 3 && nChar != 24 && nChar != 1)
	{
		return 0;
	}

	if (pEditBox->field_268)
	{
		const int nCallbackRes = reinterpret_cast<int(__stdcall*)(D2WinEditBoxStrc*, int32_t, uint8_t*)>(pEditBox->field_268)(pEditBox, pEditBox->field_25C, &nChar);
		if (!nCallbackRes && nChar != 22 && nChar != 3 && nChar != 24 && nChar != 1)
		{
			return 0;
		}
	}

	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (pEditBox->field_54 != -1 && pEditBox->field_54 != pEditBox->nTextLength && nChar != 3 && nChar != 24 && nChar != 1)
	{
		sub_6F8A7970(pEditBox);
	}

	const int32_t nCurLen = Unicode::strlen(pEditBox->wszText);
	Unicode wszWorking[512] = {};
	Unicode::strcpy(wszWorking, pEditBox->wszText);
	Unicode* pCursor = &wszWorking[pEditBox->field_25C];

	switch (nChar)
	{
	case 22: // Ctrl+V (Paste)
		{
			const char* szClip = "";
			if (IsClipboardFormatAvailable(CF_TEXT) && OpenClipboard(WINDOW_GetWindow()))
			{
				HANDLE hClipData = GetClipboardData(CF_TEXT);
				if (hClipData)
				{
					const char* pLocked = static_cast<const char*>(GlobalLock(hClipData));
					if (pLocked)
					{
						strncpy(gszClipboardDest_6F8FD8B8, pLocked, sizeof(gszClipboardDest_6F8FD8B8) - 1);
						gszClipboardDest_6F8FD8B8[sizeof(gszClipboardDest_6F8FD8B8) - 1] = '\0';
						GlobalUnlock(hClipData);
					}
				}
				CloseClipboard();
				szClip = gszClipboardDest_6F8FD8B8;
			}

			const int32_t nClipLen = static_cast<int32_t>(strlen(szClip));
			if (nCurLen + nClipLen > pEditBox->field_48 - 1)
			{
				return 0;
			}

			if (pEditBox->field_268)
			{
				for (int32_t i = 0; i < nClipLen; ++i)
				{
					uint8_t ch = static_cast<uint8_t>(szClip[i]);
					if (!D2Win_10128(ch)
						|| !reinterpret_cast<int(__stdcall*)(D2WinEditBoxStrc*, int32_t, uint8_t*)>(pEditBox->field_268)(pEditBox, pEditBox->field_25C + i, &ch))
					{
						return 0;
					}
				}
			}

			int32_t nSelStart = pEditBox->field_25C;
			int32_t nSelEnd = pEditBox->field_25C;
			if (pEditBox->field_54 != -1)
			{
				nSelStart = std::min(pEditBox->field_54, pEditBox->nTextLength);
				nSelEnd = std::max(pEditBox->field_54, pEditBox->nTextLength);
			}

			char szFiltered[400] = {};
			if (pEditBox->dwEditBoxFlags & 8)
			{
				strncpy(szFiltered, szClip, sizeof(szFiltered) - 1);
			}
			else
			{
				char* pDst = szFiltered;
				for (const char* pSrc = szClip; *pSrc && (pDst - szFiltered) < static_cast<ptrdiff_t>(sizeof(szFiltered) - 1); ++pSrc)
				{
					if (*pSrc != '\r' && *pSrc != '\n')
					{
						*pDst++ = *pSrc;
					}
				}
				*pDst = '\0';
			}

			Unicode wszPrefix[256] = {};
			Unicode wszMid[256] = {};
			Unicode wszSuffix[256] = {};
			Unicode::strncpy(wszPrefix, wszWorking, nSelStart);
			Unicode::strcpy(wszSuffix, &wszWorking[nSelEnd]);
			Unicode::win2Unicode(wszMid, szFiltered, static_cast<int>(strlen(szFiltered)) + 1);
			Unicode::strcpy(wszWorking, wszPrefix);
			Unicode::strcat(wszWorking, wszMid);
			Unicode::strcat(wszWorking, wszSuffix);

			pEditBox->field_54 = -1;
			pEditBox->nTextLength = 0;
			pCursor = &wszWorking[nSelStart + strlen(szFiltered)];
		}
		break;

	case 3:  // Ctrl+C (Copy)
	case 24: // Ctrl+X (Cut)
		if (pEditBox->field_54 != -1 && pEditBox->field_54 != pEditBox->nTextLength)
		{
			const int32_t nSelStart = std::min(pEditBox->field_54, pEditBox->nTextLength);
			const int32_t nSelEnd = std::max(pEditBox->field_54, pEditBox->nTextLength);
			Unicode wszSel[256] = {};
			Unicode::strncpy(wszSel, &wszWorking[nSelStart], nSelEnd - nSelStart);

			char szOut[400] = {};
			Unicode::unicode2Win(szOut, wszSel, sizeof(szOut));

			if (ghClipboardMem_6F8FDAC0)
			{
				GlobalFree(ghClipboardMem_6F8FDAC0);
				ghClipboardMem_6F8FDAC0 = nullptr;
			}

			if (OpenClipboard(WINDOW_GetWindow()))
			{
				EmptyClipboard();
				const size_t nAllocSize = strlen(szOut) + 1;
				ghClipboardMem_6F8FDAC0 = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nAllocSize);
				if (ghClipboardMem_6F8FDAC0)
				{
					void* pLocked = GlobalLock(ghClipboardMem_6F8FDAC0);
					if (pLocked)
					{
						memcpy(pLocked, szOut, nAllocSize);
						GlobalUnlock(ghClipboardMem_6F8FDAC0);
						SetClipboardData(CF_TEXT, ghClipboardMem_6F8FDAC0);
					}
				}
				CloseClipboard();
			}

			if (nChar == 24)
			{
				sub_6F8A7970(pEditBox);
				Unicode::strcpy(wszWorking, pEditBox->wszText);
				pCursor = &wszWorking[nSelStart];
			}
		}
		break;

	case 1: // Ctrl+A (Select All)
		pEditBox->field_54 = 0;
		pEditBox->nTextLength = Unicode::strlen(pEditBox->wszText);
		break;

	default:
		if (nCurLen >= pEditBox->field_48 - 1)
		{
			return 0;
		}
		if (nCurLen < 255)
		{
			char szTail[512] = {};
			Unicode::unicode2Win(szTail, pCursor, sizeof(szTail));

			char szInsert[516] = {};
			if (nChar == 13)
			{
				szInsert[0] = '\r';
				szInsert[1] = '\n';
				strncpy(&szInsert[2], szTail, sizeof(szInsert) - 3);
				Unicode::win2Unicode(pCursor, szInsert, 256);
				pCursor += 2;
			}
			else
			{
				szInsert[0] = static_cast<char>(nChar);
				strncpy(&szInsert[1], szTail, sizeof(szInsert) - 2);
				Unicode::win2Unicode(pCursor, szInsert, 256);
				pCursor += 1;
			}

			pEditBox->field_54 = -1;
			pEditBox->nTextLength = 0;
		}
		break;
	}

	const int32_t nMaxHeight = pEditBox->controlHeader.nHeight - 2 * pEditBox->field_44;
	if (pEditBox->dwEditBoxFlags & 8)
	{
		D2Win_10127_SetFont(pEditBox->eFont);
		const int32_t nFontHeight = D2Win_10125();
		const int32_t nStartY = pEditBox->field_44 + pEditBox->controlHeader.nImageY - pEditBox->controlHeader.nHeight + nFontHeight;
		int32_t nCurY = nStartY;

		Unicode wszUnderscore[2] = {};
		Unicode::win2Unicode(wszUnderscore, "_", 2);
		const int32_t nUnderscoreW = D2Win_10122(wszUnderscore, 2);

		char szAnsi[512] = {};
		Unicode::unicode2Win(szAnsi, wszWorking, sizeof(szAnsi));
		const char* pLineStart = szAnsi;
		const char* pScan = szAnsi;
		const int32_t nMaxWidth = pEditBox->controlHeader.nWidth - 2 * pEditBox->field_40;

		while (*pScan)
		{
			int32_t nCol = static_cast<int32_t>(pScan - pLineStart);
			bool bWrapped = false;
			char szLineBuf[256] = {};
			Unicode wszLineBuf[256] = {};

			while (*pScan && *pScan != '\r' && *pScan != '\n')
			{
				szLineBuf[nCol] = *pScan;
				Unicode::win2Unicode(wszLineBuf, szLineBuf, 256);
				if (nUnderscoreW + D2Win_10122(wszLineBuf, nCol) > nMaxWidth)
				{
					bWrapped = true;
					break;
				}
				++pScan;
				++nCol;
			}

			if (bWrapped)
			{
				nCurY += nFontHeight;
			}

			if (*pScan == '\r')
			{
				if (pScan[1] == '\n')
				{
					while (*pScan == '\r' && pScan[1] == '\n')
					{
						pScan += 2;
						nCurY += nFontHeight;
					}
				}
				else
				{
					while (*pScan == '\r')
					{
						++pScan;
						nCurY += nFontHeight;
					}
				}
			}
			pLineStart = pScan;
		}

		if (nCurY - nStartY >= nMaxHeight)
		{
			return 1;
		}
	}

	Unicode::strcpy(pEditBox->wszText, wszWorking);
	pEditBox->field_25C = static_cast<int32_t>(pCursor - wszWorking);
	sub_6F8A98A0(pEditBox);
	if (pEditBox->field_26C)
	{
		pEditBox->field_26C(0);
	}
	return 1;
}

//D2Win.0x6F8A7970
void __fastcall sub_6F8A7970(D2WinEditBoxStrc* pEditBox)
{
	// TODO: Names
	Unicode v11[256] = {};
	Unicode v12[256] = {};

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	int32_t v6 = pEditBox->nTextLength;
	int32_t v7 = pEditBox->field_54;
	int32_t v8 = pEditBox->field_54;

	int32_t v9 = pEditBox->nTextLength;

	if (v7 >= v6)
	{
		v8 = pEditBox->nTextLength;
		if (v7 > v6)
		{
			v9 = pEditBox->field_54;
		}
	}

	if (v8)
	{
		Unicode::strncpy(v11, pEditBox->wszText, v8);
	}
	else
	{
		v11[0] = 0;
	}

	Unicode::strcpy(v12, &pEditBox->wszText[v9]);
	Unicode::strcpy(pEditBox->wszText, v11);
	Unicode::strcat(pEditBox->wszText, v12);

	int32_t v10 = Unicode::strlen(pEditBox->wszText);
	if (v8 > v10)
	{
		pEditBox->field_25C = v10;
	}
	else
	{
		pEditBox->field_25C = v8;
	}

	D2Win_10083(pEditBox);
}

//D2Win.0x6F8A7AB0
int32_t __fastcall sub_6F8A7AB0(D2WinEditBoxStrc* pEditBox, char a2)
{
	char nKey = a2;
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	const uint32_t dwFlags = pEditBox->controlHeader.dwFlags;
	if (!(dwFlags & gdwBitMasks[2]) || !(dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	BOOL bDeletedSelection = FALSE;
	BOOL bTextChanged = FALSE;
	Unicode* pCursor = &pEditBox->wszText[pEditBox->field_25C];
	const BOOL bShiftDown = (GetKeyState(VK_SHIFT) & 0x8000) != 0;

	switch (nKey)
	{
	case VK_BACK:
	case VK_DELETE:
		if (pEditBox->field_54 != -1 && pEditBox->field_54 != pEditBox->nTextLength)
		{
			sub_6F8A7970(pEditBox);
			bDeletedSelection = TRUE;
			bTextChanged = TRUE;
			pCursor = &pEditBox->wszText[pEditBox->field_25C];
		}
		break;

	case VK_END:
	case VK_HOME:
	case VK_LEFT:
	case VK_UP:
	case VK_RIGHT:
	case VK_DOWN:
		if (bShiftDown)
		{
			if (pEditBox->field_54 == -1)
			{
				pEditBox->field_54 = pEditBox->field_25C;
				pEditBox->nTextLength = pEditBox->field_25C;
			}
		}
		else
		{
			D2Win_10083(pEditBox);
		}
		break;

	default:
		break;
	}

	auto FinishCursorUpdate = [&]() -> int32_t
	{
		pEditBox->field_25C = static_cast<int32_t>(pCursor - pEditBox->wszText);
		sub_6F8A98A0(pEditBox);
		if (bTextChanged && pEditBox->field_268)
		{
			if (!reinterpret_cast<int(__stdcall*)(D2WinEditBoxStrc*, int32_t, char*)>(pEditBox->field_268)(pEditBox, pEditBox->field_25C, &nKey)
				&& nKey != VK_BACK && nKey != VK_DELETE)
			{
				return 0;
			}
		}
		if (pEditBox->field_26C)
		{
			pEditBox->field_26C(0);
		}
		return 1;
	};

	switch (nKey)
	{
	case VK_HOME:
		pCursor = pEditBox->wszText;
		if (bShiftDown)
		{
			pEditBox->nTextLength = 0;
		}
		return FinishCursorUpdate();

	case VK_END:
		while (*pCursor)
		{
			++pCursor;
		}
		if (bShiftDown && pEditBox->field_54 != -1)
		{
			pEditBox->nTextLength = static_cast<int32_t>(pCursor - pEditBox->wszText);
		}
		return FinishCursorUpdate();

	case VK_UP:
		if (pEditBox->dwEditBoxFlags & 8)
		{
			int32_t nCurX = 0;
			int32_t nCurY = 0;
			sub_6F8A8D60(pEditBox, &nCurX, &nCurY);
			SMSGHANDLER_PARAMS msg = {};
			msg.hWindow = reinterpret_cast<HWND>(pEditBox);
			msg.nMessage = 0;
			sub_6F8A9410(&msg, nCurX, nCurY - D2Win_10125());
			pCursor = &pEditBox->wszText[pEditBox->field_25C];
		}
		else
		{
			pCursor = pEditBox->wszText;
			if (bShiftDown)
			{
				pEditBox->nTextLength = 0;
			}
		}
		return FinishCursorUpdate();

	case VK_DOWN:
		if (pEditBox->dwEditBoxFlags & 8)
		{
			int32_t nCurX = 0;
			int32_t nCurY = 0;
			sub_6F8A8D60(pEditBox, &nCurX, &nCurY);
			SMSGHANDLER_PARAMS msg = {};
			msg.hWindow = reinterpret_cast<HWND>(pEditBox);
			msg.nMessage = 0;
			sub_6F8A9410(&msg, nCurX, nCurY + D2Win_10125());
			pCursor = &pEditBox->wszText[pEditBox->field_25C];
		}
		else
		{
			while (*pCursor)
			{
				++pCursor;
			}
			if (bShiftDown)
			{
				pEditBox->nTextLength = static_cast<int32_t>(pCursor - pEditBox->wszText);
			}
		}
		return FinishCursorUpdate();

	case VK_LEFT:
		--pCursor;
		if (pCursor >= pEditBox->wszText && *pCursor == '\n')
		{
			--pCursor;
		}
		if (pCursor >= pEditBox->wszText)
		{
			if (bShiftDown && pEditBox->field_54 != -1)
			{
				pEditBox->nTextLength = static_cast<int32_t>(pCursor - pEditBox->wszText);
			}
		}
		else
		{
			pCursor = pEditBox->wszText;
		}
		return FinishCursorUpdate();

	case VK_RIGHT:
		if (*pCursor)
		{
			const Unicode chNext = pCursor[1];
			++pCursor;
			if (chNext == '\n')
			{
				++pCursor;
			}
			if (bShiftDown && pEditBox->field_54 != -1)
			{
				pEditBox->nTextLength = static_cast<int32_t>(pCursor - pEditBox->wszText);
			}
		}
		return FinishCursorUpdate();

	case VK_BACK:
		if (pCursor > pEditBox->wszText && !bDeletedSelection)
		{
			--pCursor;
			const size_t nMoveBytes = sizeof(Unicode) * (Unicode::strlen(pCursor + 1) + 1);
			memmove(pCursor, pCursor + 1, nMoveBytes);
			if (pCursor > pEditBox->wszText && *(pCursor - 1) == '\r')
			{
				--pCursor;
				memmove(pCursor, pCursor + 1, nMoveBytes);
			}
			bTextChanged = TRUE;
		}
		return FinishCursorUpdate();

	case VK_DELETE:
		if (*pCursor && !bDeletedSelection)
		{
			const size_t nCount = Unicode::strlen(pCursor + 1) + 1;
			memmove(pCursor, pCursor + 1, sizeof(Unicode) * nCount);
			if (*pCursor == '\n')
			{
				memmove(pCursor, pCursor + 1, sizeof(Unicode) * (nCount - 1));
			}
			bTextChanged = TRUE;
		}
		return FinishCursorUpdate();

	case VK_RETURN:
		if (!pEditBox->field_264)
		{
			return FinishCursorUpdate();
		}
		{
			const uint32_t bKeepAlive = pEditBox->dwEditBoxFlags & 2;
			++dword_6F8FE254;
			reinterpret_cast<void(__stdcall*)(Unicode*)>(pEditBox->field_264)(pEditBox->wszText);
			--dword_6F8FE254;
			if (!bKeepAlive)
			{
				return 1;
			}
			pCursor = &pEditBox->wszText[pEditBox->field_25C];
			return FinishCursorUpdate();
		}

	case VK_ESCAPE:
		if (pEditBox->field_264)
		{
			++dword_6F8FE254;
			reinterpret_cast<void(__stdcall*)(DWORD)>(pEditBox->field_264)(0);
			--dword_6F8FE254;
		}
		return 1;

	case VK_TAB:
		{
			D2WinEditBoxStrc* pTarget = (GetKeyState(VK_SHIFT) < 0) ? pEditBox->pPrevious : pEditBox->pNext;
			if (!pTarget)
			{
				return 1;
			}
			while (pTarget && pTarget != pEditBox)
			{
				const uint32_t dwTargetFlags = pTarget->controlHeader.dwFlags;
				if ((dwTargetFlags & gdwBitMasks[2]) && (dwTargetFlags & gdwBitMasks[0]))
				{
					break;
				}
				pTarget = (GetKeyState(VK_SHIFT) < 0) ? pTarget->pPrevious : pTarget->pNext;
			}
			D2Win_10012(pTarget);
			D2_ASSERT(pTarget);
			D2_ASSERT(pTarget->controlHeader.nType == D2WIN_EDITBOX);
			pTarget->field_54 = 0;
			pTarget->nTextLength = Unicode::strlen(pTarget->wszText);
			return 1;
		}

	case VK_F1:
		return 0;

	default:
		return FinishCursorUpdate();
	}
}

//D2Win.0x6F8A80A0 (#10081)
void __fastcall D2Win_10081(D2WinEditBoxStrc* pEditBox, int32_t a2)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_48 = a2;
}

//D2Win.0x6F8A80F0 (#10076)
int32_t __fastcall D2Win_10076(D2WinEditBoxStrc* pEditBox, const char* szText)
{
	Unicode wszText[256] = {};

	Unicode::win2Unicode(wszText, szText, 256);
	return D2Win_10075(pEditBox, wszText);
}

//D2Win.0x6F8A8140 (#10075)
int32_t __fastcall D2Win_10075(D2WinEditBoxStrc* pEditBox, const Unicode* pText)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_25C = Unicode::strlen(pText);
	if (pEditBox->field_25C)
	{
		Unicode::strcpy(pEditBox->wszText, pText);
	}
	else
	{
		pEditBox->wszText[0] = 0;
	}

	sub_6F8A98A0(pEditBox);

	if (pEditBox->field_26C)
	{
		pEditBox->field_26C(0);
	}

	return 1;
}

//D2Win.0x6F8A81E0 (#10077)
const Unicode* __fastcall D2Win_10077_EDITBOX_GetText(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	return pEditBox->wszText;
}

//D2Win.0x6F8A8230 (#10080)
void __fastcall D2Win_10080_EDITBOX_SetNextEditBox(D2WinEditBoxStrc* pEditBox, D2WinEditBoxStrc* pNext)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	D2_ASSERT(pNext);
	D2_ASSERT(pNext->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->pNext = pNext;
	pNext->pPrevious = pEditBox;
}

//D2Win.0x6F8A82D0
int32_t __fastcall sub_6F8A82D0(D2WinControlStrc* pControl)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pControl;

	//TODO: Names
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (pEditBox->controlHeader.dwFlags & gdwBitMasks[2] && pEditBox->controlHeader.dwFlags & gdwBitMasks[0])
	{
		if (!D2Win_IsMouseButtonPressed())
		{
			pEditBox->field_280 = 0;
		}

		if (pEditBox->field_280)
		{
			SMSGHANDLER_PARAMS msg = {};
			msg.hWindow = (HWND)pEditBox;
			msg.nMessage = WM_LBUTTONDOWN;
			sub_6F8A9120(&msg);
		}

		if (pEditBox->controlHeader.pCellFile)
		{
			D2GfxDataStrc gfxData = {};
			gfxData.pCellFile = pEditBox->controlHeader.pCellFile;
			gfxData.nDirection = 0;
			gfxData.nFrame = 0;
			TEXTURE_CelDraw(&gfxData, pEditBox->controlHeader.nImageX, pEditBox->controlHeader.nImageY - 1, -1, DRAWMODE_NORMAL, 0);
		}

		D2Win_10127_SetFont(pEditBox->eFont);

		int32_t nX = pEditBox->controlHeader.nImageX + pEditBox->field_40;
		const int32_t nY = pEditBox->field_44 - pEditBox->controlHeader.nHeight + pEditBox->controlHeader.nImageY + 16;

		Unicode v48[300] = {};
		Unicode::strncpy(v48, &pEditBox->wszText[pEditBox->field_4C], pEditBox->field_50 - pEditBox->field_4C + 1);

		const int32_t v50 = Unicode::strlen(v48);
		if (pEditBox->dwEditBoxFlags & 1)
		{
			Unicode v41[2] = {};
			Unicode::win2Unicode(v41, "*", 2);
			for (int32_t i = 0; i < v50; ++i)
			{
				v48[i] = v41[0];
			}
			//if (v50 > 0)
			//{
			//	LOWORD(pEditBox2) = v41[0];
			//	pEditBox3 = pEditBox2 << 16;
			//	LOWORD(pEditBox3) = v41[0];
			//	pEditBox4 = v50 & 1;
			//	pEditBox5 = v50 >> 1;
			//	memset32(v48, pEditBox3, v50 >> 1);
			//	pEditBox6 = &v48[2 * pEditBox5];
			//	for (i = pEditBox4; i; --i)
			//	{
			//		*pEditBox6 = pEditBox3;
			//		++pEditBox6;
			//	}
			//}
		}

		if (pEditBox->field_54 == -1 || pEditBox->field_54 == pEditBox->nTextLength)
		{
			D2Win_10117_DrawText(v48, nX, nY, pEditBox->nTextColor, 0);
		}
		else
		{
			Unicode v49[256] = {};
			Unicode wszText[256] = {};
			Unicode v51[256] = {};

			int32_t v27 = pEditBox->field_54;
			if (pEditBox->field_54 >= pEditBox->nTextLength)
			{
				v27 = pEditBox->nTextLength;
			}

			int32_t v29 = 0;
			if (v29 > v27 - pEditBox->field_4C)
			{
				v29 = v27 - pEditBox->field_4C;
			}

			int32_t v25 = pEditBox->nTextLength;
			if (pEditBox->field_54 > v25)
			{
				v25 = pEditBox->field_54;
			}

			int32_t v30 = 0;
			if (v25 - pEditBox->field_4C - v29 > 0)
			{
				v30 = v25 - pEditBox->field_4C - v29;
			}

			Unicode::strncpy(wszText, v48, v29);
			Unicode::strncpy(v49, &v48[v29], v30);
			Unicode::strcpy(v51, &v48[v29 + v30]);

			int32_t nWidth1 = 0;
			int32_t nWidth2 = 0;
			int32_t nUnused = 0;
			D2Win_10131_GetTextDimensions(wszText, &nWidth1, &nUnused);
			D2Win_10131_GetTextDimensions(v49, &nWidth2, &nUnused);
			D2Win_10117_DrawText(wszText, nX, nY, pEditBox->nTextColor, 0);
			D2Win_10133(v49, nX + nWidth1, nY, D2Win_10034_MixRGB(0x40u, 0x40u, 0x40u), DRAWMODE_NORMAL, pEditBox->nTextColor);
			D2Win_10117_DrawText(v51, nX + nWidth1 + nWidth2, nY, pEditBox->nTextColor, 0);
		}

		if (D2Win_10023() == pEditBox && GetTickCount() / 250 & 1)
		{
			Unicode v41[2] = {};
			Unicode::win2Unicode(v41, "_", 2);
			if (pEditBox->field_25C != pEditBox->field_4C)
			{
				nX += D2Win_10122(v48, pEditBox->field_25C - pEditBox->field_4C);
			}

			D2Win_10117_DrawText(v41, nX, nY, pEditBox->nTextColor, 0);
		}
	}
	return 1;
}

//D2Win.0x6F8A86C0
int32_t __fastcall sub_6F8A86C0(D2WinControlStrc* pControl)
{
	D2WinEditBoxStrc* pEditBox = reinterpret_cast<D2WinEditBoxStrc*>(pControl);

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (!(pEditBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pEditBox->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 1;
	}

	if (!D2Win_IsMouseButtonPressed())
	{
		pEditBox->field_280 = 0;
	}

	if (pEditBox->field_280)
	{
		SMSGHANDLER_PARAMS msg = {};
		msg.hWindow = reinterpret_cast<HWND>(pEditBox);
		msg.nMessage = WM_LBUTTONDOWN;
		sub_6F8A9410(&msg, gMousePosition_6F8FE234.x - pEditBox->field_40 - pEditBox->controlHeader.nImageX, gMousePosition_6F8FE234.y + pEditBox->controlHeader.nHeight - pEditBox->field_44 - pEditBox->controlHeader.nImageY);
	}

	D2Win_10127_SetFont(pEditBox->eFont);

	const int32_t nX = pEditBox->field_40 + pEditBox->controlHeader.nImageX;
	const int32_t nFontHeight = D2Win_10125();
	const int32_t nStartY = pEditBox->field_44 + pEditBox->controlHeader.nImageY - pEditBox->controlHeader.nHeight + nFontHeight;
	int32_t nCurY = nStartY;

	Unicode wszUnderscore[2] = {};
	Unicode::win2Unicode(wszUnderscore, "_", 2);
	const int32_t nUnderscoreW = D2Win_10122(wszUnderscore, 2);

	const BOOL bDrawCursor = (D2Win_10023() == pEditBox && ((GetTickCount() / 250) & 1));
	BOOL bCursorDrawn = FALSE;

	char szAnsi[256] = {};
	Unicode::unicode2Win(szAnsi, pEditBox->wszText, sizeof(szAnsi));

	const char* pLineStart = szAnsi;
	const char* pScan = szAnsi;
	const int32_t nMaxWidth = pEditBox->controlHeader.nWidth - 2 * pEditBox->field_40;
	const int32_t nMaxHeight = pEditBox->controlHeader.nHeight - 2 * pEditBox->field_44;

	if (!szAnsi[0])
	{
		if (bDrawCursor)
		{
			D2Win_10117_DrawText(wszUnderscore, nX, nStartY, pEditBox->nTextColor, 0);
		}
		return 1;
	}

	while (*pScan)
	{
		if (nCurY - nStartY >= nMaxHeight)
		{
			return 1;
		}

		int32_t nCount = static_cast<int32_t>(pScan - pLineStart);
		char szLineBuf[256] = {};
		Unicode wszLineBuf[256] = {};
		BOOL bWrapped = FALSE;

		while (*pScan && *pScan != '\r' && *pScan != '\n')
		{
			szLineBuf[nCount] = *pScan;
			Unicode::win2Unicode(wszLineBuf, szLineBuf, 256);
			if (nUnderscoreW + D2Win_10122(wszLineBuf, nCount) > nMaxWidth)
			{
				bWrapped = TRUE;
				break;
			}
			++pScan;
			++nCount;
		}

		const int32_t nLineStartOffset = static_cast<int32_t>(pLineStart - szAnsi);
		const int32_t nLineEndOffset = static_cast<int32_t>(pScan - szAnsi);

		if (pEditBox->field_54 != -1 && pEditBox->field_54 != pEditBox->nTextLength)
		{
			const int32_t nSelStart = std::min(pEditBox->field_54, pEditBox->nTextLength);
			const int32_t nSelEnd = std::max(pEditBox->field_54, pEditBox->nTextLength);

			if (nSelStart < nLineStartOffset && nSelEnd >= nLineEndOffset)
			{
				strncpy(szLineBuf, pLineStart, nCount);
				szLineBuf[nLineEndOffset - nLineStartOffset] = '\0';
				Unicode::win2Unicode(wszLineBuf, szLineBuf, 256);
				int32_t nW = 0;
				int32_t nH = 0;
				D2Win_10131_GetTextDimensions(wszLineBuf, &nW, &nH);
				D2GFX_DrawBox(nX, nCurY - nFontHeight, nX + nW, nCurY, D2Win_10034_MixRGB(0x40u, 0x40u, 0x40u), DRAWMODE_NORMAL);
			}
			else if ((nSelStart >= nLineStartOffset && nSelStart < nLineEndOffset)
				|| (nSelEnd >= nLineStartOffset && nSelEnd < nLineEndOffset))
			{
				const int32_t nClampStart = std::max(nSelStart, nLineStartOffset);
				const int32_t nClampEnd = std::min(nSelEnd, nLineEndOffset);
				const int32_t nPreLen = nClampStart - nLineStartOffset;
				int32_t nHighlightX = nX;
				if (nPreLen > 0)
				{
					strncpy(szLineBuf, pLineStart, nPreLen);
					szLineBuf[nPreLen] = '\0';
					Unicode::win2Unicode(wszLineBuf, szLineBuf, 256);
					int32_t nW = 0;
					int32_t nH = 0;
					D2Win_10131_GetTextDimensions(wszLineBuf, &nW, &nH);
					nHighlightX += nW;
				}

				const int32_t nSelLen = nClampEnd - nClampStart;
				strncpy(szLineBuf, pLineStart + nPreLen, nSelLen);
				szLineBuf[nSelLen] = '\0';
				Unicode::win2Unicode(wszLineBuf, szLineBuf, 256);
				int32_t nW = 0;
				int32_t nH = 0;
				D2Win_10131_GetTextDimensions(wszLineBuf, &nW, &nH);
				D2GFX_DrawBox(nHighlightX, nCurY - nFontHeight, nHighlightX + nW, nCurY, D2Win_10034_MixRGB(0x40u, 0x40u, 0x40u), DRAWMODE_NORMAL);
			}
		}

		strncpy(szLineBuf, pLineStart, nCount);
		szLineBuf[nLineEndOffset - nLineStartOffset] = '\0';
		Unicode::win2Unicode(wszLineBuf, szLineBuf, 256);
		D2Win_10117_DrawText(wszLineBuf, nX, nCurY, pEditBox->nTextColor, 0);

		if (bDrawCursor && !bCursorDrawn)
		{
			const int32_t nRelCursor = pEditBox->field_25C - nLineStartOffset;
			if (nRelCursor >= 0 && nRelCursor <= nCount)
			{
				const int32_t nCursorX = (nRelCursor > 0) ? (nX + D2Win_10122(wszLineBuf, nRelCursor)) : nX;
				D2Win_10117_DrawText(wszUnderscore, nCursorX, nCurY, pEditBox->nTextColor, 0);
				bCursorDrawn = TRUE;
			}
		}

		if (bWrapped)
		{
			nCurY += nFontHeight;
		}

		if (*pScan == '\r')
		{
			if (pScan[1] == '\n')
			{
				int32_t nOffset = static_cast<int32_t>(pScan - szAnsi);
				while (*pScan == '\r' && pScan[1] == '\n')
				{
					pScan += 2;
					nOffset += 2;
					nCurY += nFontHeight;
					if (bDrawCursor && !bCursorDrawn && pEditBox->field_25C == nOffset && (nCurY - nStartY) < nMaxHeight)
					{
						D2Win_10117_DrawText(wszUnderscore, nX, nCurY, pEditBox->nTextColor, 0);
					}
				}
			}
			else
			{
				int32_t nOffset = static_cast<int32_t>(pScan - szAnsi);
				while (*pScan == '\r')
				{
					++pScan;
					++nOffset;
					nCurY += nFontHeight;
					if (bDrawCursor && !bCursorDrawn && pEditBox->field_25C == nOffset && (nCurY - nStartY) < nMaxHeight)
					{
						D2Win_10117_DrawText(wszUnderscore, nX, nCurY, pEditBox->nTextColor, 0);
					}
				}
			}
		}

		pLineStart = pScan;
	}

	return 1;
}

//D2Win.0x6F8A8D60
void __fastcall sub_6F8A8D60(D2WinEditBoxStrc* pEditBox, int32_t* a2, int32_t* a3)
{
	//Unicode v48[300] = {};

	//TODO: Names
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	D2Win_10127_SetFont(pEditBox->eFont);

	const int32_t v37 = pEditBox->controlHeader.nImageX + pEditBox->field_40;
	const int32_t v6 = D2Win_10125();
	const int32_t v38 = pEditBox->field_44 + pEditBox->controlHeader.nImageY - pEditBox->controlHeader.nHeight + v6;
	int32_t v7 = v38;
	//Unicode::strcpy(v48, pEditBox->wszText);

	Unicode a1[2] = {};
	Unicode::win2Unicode(a1, "_", 2);

	const int32_t v43 = D2Win_10122(a1, 2);

	char v45[256] = {};
	Unicode::unicode2Win(v45, pEditBox->wszText, ARRAY_SIZE(v45));
	const char* v10 = v45;
	const char* v32 = v45;
	const int32_t v44 = pEditBox->controlHeader.nWidth - 2 * pEditBox->field_40;
	const int32_t v39 = pEditBox->controlHeader.nHeight - 2 * pEditBox->field_44;

	*a2 = -1;
	*a3 = -1;

	while (*v10)
	{
		if (v7 - v38 >= v39)
		{
			break;
		}

		int32_t v16 = v10 - v32;
		char v46[256] = {};
		Unicode v47[256] = {};
		BOOL v41 = 0;
		while (*v10 && *v10 != '\r' && *v10 != '\n')
		{
			v46[v16] = *v10;
			Unicode::win2Unicode(v47, v46, 256);
			if (v43 + D2Win_10122(v47, v16) > v44)
			{
				v41 = 1;
				break;
			}

			++v10;
			++v16;
		}

		const int32_t v19 = (int32_t)&v45[pEditBox->field_25C - (int32_t)v32];
		if (*a2 == -1 && v19 >= 0 && v19 <= v16)
		{
			*a2 = v37;
			if (v19)
			{
				Unicode::win2Unicode(v47, v46, 256);
				*a2 += D2Win_10122(v47, v19);
			}
			*a3 = v7;
		}

		if (v41)
		{
			v7 += v6;
		}

		if (*v10 == '\r')
		{
			if (v10[1] == '\n')
			{
				int32_t v22 = v10 - v45;
				int32_t v23 = v7 - v38;
				do
				{
					if (v10[1] != '\n')
					{
						break;
					}
					v10 += 2;
					v22 += 2;
					v7 += v6;
					v23 += v6;
					if (*a2 == -1 && pEditBox->field_25C == v22 && v23 < v39)
					{
						*a2 = v37;
						*a3 = v7;
					}
				}
				while (*v10 == '\r');
			}
			else
			{
				int32_t v24 = v10 - v45;
				int32_t v25 = v7 - v38;
				do
				{
					++v10;
					++v24;
					v7 += v6;
					v25 += v6;
					if (*a2 == -1 && pEditBox->field_25C == v24 && v25 < v39)
					{
						*a2 = v37;
						*a3 = v7;
					}
				}
				while (*v10 == '\r');
			}
		}
		v32 = v10;
	}

	if (*a2 != -1)
	{
		*a2 -= pEditBox->controlHeader.nImageX - 1;
		*a3 += pEditBox->controlHeader.nHeight - pEditBox->controlHeader.nImageY - 1;

		*a2 = D2Clamp(*a2, pEditBox->field_40, pEditBox->controlHeader.nWidth - pEditBox->field_40 - 1);
		*a3 = D2Clamp(*a2, pEditBox->field_44, pEditBox->controlHeader.nHeight - pEditBox->field_44 - 1);
	}
}

//D2Win.0x6F8A9120
int32_t __stdcall sub_6F8A9120(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (!(pEditBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pEditBox->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	D2Win_10012(pEditBox);

	const int32_t nX = gMousePosition_6F8FE234.x - pEditBox->field_40 - pEditBox->controlHeader.nImageX;
	const int32_t nY = pEditBox->controlHeader.nHeight - pEditBox->field_44 - pEditBox->controlHeader.nImageY + gMousePosition_6F8FE234.y;

	const bool bInside = nX >= -pEditBox->field_40 && nX < pEditBox->controlHeader.nWidth - pEditBox->field_40 && nY > -pEditBox->field_44 && nY < pEditBox->controlHeader.nHeight - pEditBox->field_44;
	const uint32_t dwTickCount = GetTickCount();

	if (bInside && !pEditBox->field_280 && pEditBox == dword_6F8FD8B0 && dwTickCount - dword_6F8FD8B4 < 300)
	{
		D2Win_10082(pEditBox);
	}
	else
	{
		if (pEditBox->field_280 || bInside)
		{
			D2Win_10127_SetFont(pEditBox->eFont);

			if (pEditBox->field_280)
			{
				if (nX < -pEditBox->field_40)
				{
					if (pEditBox->field_25C - 1 < 0)
					{
						pEditBox->field_25C = 0;
					}
					else
					{
						--pEditBox->field_25C;
					}

					sub_6F8A98A0(pEditBox);
				}
				else if (nX >= pEditBox->controlHeader.nWidth - pEditBox->field_40)
				{
					const int nLength = Unicode::strlen(pEditBox->wszText) - 1;
					if (pEditBox->field_25C + 1 > nLength)
					{
						pEditBox->field_25C = nLength;
					}
					else
					{
						++pEditBox->field_25C;
					}

					sub_6F8A98A0(pEditBox);
				}
			}

			int nCounter = 1;
			while (nX > D2Win_10122(&pEditBox->wszText[pEditBox->field_4C], nCounter))
			{
				if (nCounter > Unicode::strlen(&pEditBox->wszText[pEditBox->field_4C]))
				{
					break;
				}
				++nCounter;
			}

			const int nOldValue = pEditBox->field_25C;
			pEditBox->field_25C = pEditBox->field_4C + nCounter - 1;

			if (GetKeyState(VK_SHIFT) & 0x8000 || pEditBox->field_280)
			{
				if (pEditBox->field_54 == -1)
				{
					pEditBox->field_54 = nOldValue;
				}

				pEditBox->nTextLength = pEditBox->field_25C;
			}
			else
			{
				D2Win_10083(pEditBox);
			}
		}

		if (pMsg->nMessage == WM_LBUTTONDOWN)
		{
			pEditBox->field_280 = 1;
		}
	}

	dword_6F8FD8B0 = pEditBox;
	dword_6F8FD8B4 = dwTickCount;

	if (pEditBox->controlHeader.field_34)
	{
		return pEditBox->controlHeader.field_34(pMsg);
	}

	return 0;
}

//D2Win.0x6F8A93A0
int32_t __stdcall EDITBOX_HandleMouseUp(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (pEditBox->field_280)
	{
		pEditBox->field_280 = 0;
		return 1;
	}

	return 0;
}

//D2Win.0x6F8A9410
int32_t __fastcall sub_6F8A9410(SMSGHANDLER_PARAMS* pMsg, int32_t nX, int32_t nY)
{
	//TODO: Names
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (!(pEditBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pEditBox->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	D2Win_10012(pEditBox);

	if (pEditBox->field_280)
	{
		nX = D2Clamp(nX, pEditBox->field_40, pEditBox->controlHeader.nWidth - pEditBox->field_40 - 1);
		nY = D2Clamp(nY, pEditBox->field_44, pEditBox->controlHeader.nHeight - pEditBox->field_44 - 1);
	}

	if (nX >= -pEditBox->field_40 && nX < pEditBox->controlHeader.nWidth - pEditBox->field_40 && nY >= -pEditBox->field_44 && nY < pEditBox->controlHeader.nHeight - pEditBox->field_44)
	{
		D2Win_10127_SetFont(pEditBox->eFont);

		Unicode wszText[2] = {};
		Unicode::win2Unicode(wszText, "_", 2);

		const int32_t v36 = D2Win_10121_GetTextWidth(wszText);

		char v38[256] = {};
		Unicode::unicode2Win(v38, pEditBox->wszText, ARRAY_SIZE(v38));
		const char* v30 = v38;
		const char* v13 = v38;
		int32_t v26 = 0;
		const int32_t v28 = D2Win_10125();
		int32_t v31 = 0;
		int32_t v35 = -1;

		while (*v13)
		{
			char v39[256] = {};
			Unicode v40[256] = {};

			int32_t v34 = 0;
			int32_t v18 = v13 - v30;
			while (*v13 && *v13 != '\r' && *v13 != '\n')
			{
				v39[v18] = *v13;
				Unicode::win2Unicode(v40, v39, 256);
				if (v36 + D2Win_10122(v40, v18) > pEditBox->controlHeader.nWidth - 2 * pEditBox->field_40)
				{
					v34 = 1;
					break;
				}
				++v13;
				++v18;
			}

			if (nY >= v26 && nY < v28 + v26)
			{
				int32_t v21 = 1;
				Unicode::win2Unicode(v40, v39, 256);
				while (nX > D2Win_10122(v40, v21))
				{
					if (v21 > v18)
					{
						break;
					}
					++v21;
				}
				v35 = pEditBox->field_25C;
				v31 = 1;
				pEditBox->field_25C = (int32_t)&v30[v21 - (int32_t)v38 - 1];
			}

			if (!v34)
			{
				if (*v13 == '\r')
				{
					if (v13[1] == '\n')
					{
						v13 += 2;
					}
					else
					{
						++v13;
					}
					v26 += v28;
				}
			}
			else
			{
				v26 += v28;
			}

			v30 = v13;
		}

		int32_t v23 = v35;
		if (!v31)
		{
			v23 = pEditBox->field_25C;
			pEditBox->field_25C = Unicode::strlen(pEditBox->wszText);
		}

		if ((GetKeyState(VK_SHIFT) & 0x8000 || pEditBox->field_280) && v23 != -1)
		{
			if (pEditBox->field_54 == -1)
			{
				pEditBox->field_54 = v23;
			}
			pEditBox->nTextLength = pEditBox->field_25C;
		}
		else
		{
			D2Win_10083(pEditBox);
		}
	}

	if (pMsg->nMessage == WM_LBUTTONDOWN)
	{
		pEditBox->field_280 = 1;
	}

	if (pEditBox->controlHeader.field_34)
	{
		return pEditBox->controlHeader.field_34(pMsg);
	}

	return 0;
}

//D2Win.0x6F8A97D0
int32_t __stdcall sub_6F8A97D0(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	return sub_6F8A9410(pMsg, gMousePosition_6F8FE234.x - pEditBox->field_40 - pEditBox->controlHeader.nImageX, gMousePosition_6F8FE234.y + pEditBox->controlHeader.nHeight - pEditBox->field_44 - pEditBox->controlHeader.nImageY);
}

//D2Win.0x6F8A9850 (#10078)
bool __stdcall D2Win_10078_IsNumber(int32_t nUnused1, int32_t nUnused2, char* szChar)
{
	return *szChar >= '0' && *szChar <= '9';
}

//D2Win.0x6F8A9870 (#10079)
bool __stdcall D2Win_10079_IsLetter(int nUnused1, int nUnused2, char* szChar)
{
	return *szChar >= 'a' && *szChar <= 'z' || *szChar >= 'A' && *szChar <= 'Z';
}

//D2Win.0x6F8A98A0
void __fastcall sub_6F8A98A0(D2WinEditBoxStrc* pEditBox)
{
	//TODO: Names
	D2Win_10127_SetFont(pEditBox->eFont);

	const int32_t v2 = Unicode::strlen(pEditBox->wszText);

	Unicode wszText[300] = {};
	if (pEditBox->dwEditBoxFlags & EDITBOX_PASSWORD)
	{
		Unicode wszStar[2] = {};
		Unicode::win2Unicode(wszStar, "*", 2);
		if (v2 > 0)
		{
			for (int32_t i = 0; i < v2; ++i)
			{
				wszText[i] = wszStar[0];
			}

			//LOWORD(v7) = wszStar;
			//v8 = v7 << 16;
			//LOWORD(v8) = wszStar;
			//memset32(wszText, v8, (unsigned int)v2 >> 1);
			//v9 = &wszText[2 * ((unsigned int)v2 >> 1)];
			//for (int i = v2 & 1; i; --i)
			//{
			//	*v9 = v8;
			//	++v9;
			//}
		}
	}
	else
	{
		Unicode::strcpy(wszText, pEditBox->wszText);
	}

	Unicode wszDash[2] = {};
	Unicode::win2Unicode(wszDash, "_", 2);

	const int32_t nDashWidth = D2Win_10121_GetTextWidth(wszDash);

	int32_t nTextWidth = D2Win_10121_GetTextWidth(wszText);
	if (pEditBox->field_25C == v2)
	{
		nTextWidth += nDashWidth;
	}

	if (nTextWidth <= pEditBox->controlHeader.nWidth - 2 * pEditBox->field_40)
	{
		pEditBox->field_4C = 0;
		pEditBox->field_50 = v2 - 1;
	}
	else
	{
		int32_t v16 = pEditBox->controlHeader.nWidth - 2 * pEditBox->field_40;
		if (pEditBox->field_25C == v2)
		{
			v16 -= nDashWidth;
		}

		if (pEditBox->field_25C > pEditBox->field_50)
		{
			pEditBox->field_50 = pEditBox->field_25C;
			pEditBox->field_4C = pEditBox->field_25C - 1;
			while (pEditBox->field_4C)
			{
				if (D2Win_10122(&wszText[pEditBox->field_4C], pEditBox->field_50 - pEditBox->field_4C + 1) >= v16)
				{
					break;
				}

				--pEditBox->field_4C;
			}

			++pEditBox->field_4C;
		}
		else if (pEditBox->field_25C > pEditBox->field_4C + 1)
		{
			if (D2Win_10121_GetTextWidth(&wszText[pEditBox->field_4C]) >= v16)
			{
				pEditBox->field_50 = pEditBox->field_4C + 1;
				while (pEditBox->field_50 < v2)
				{
					if (D2Win_10122(&wszText[pEditBox->field_4C], pEditBox->field_50 - pEditBox->field_4C + 1) >= v16)
					{
						break;
					}

					++pEditBox->field_50;
				}

				--pEditBox->field_50;
			}
			else
			{
				pEditBox->field_50 = v2 - 1;
				pEditBox->field_4C = v2 - 2;
				while (pEditBox->field_4C)
				{
					if (D2Win_10121_GetTextWidth(&wszText[pEditBox->field_4C]) >= v16)
					{
						break;
					}

					--pEditBox->field_4C;
				}

				++pEditBox->field_4C;
			}
		}
		else
		{
			const int32_t v19 = std::max(pEditBox->field_25C - 1, 0);

			pEditBox->field_4C = v19;
			pEditBox->field_50 = v19 + 1;
			while (pEditBox->field_50 < v2)
			{
				if (D2Win_10122(&wszText[pEditBox->field_4C], pEditBox->field_50 - pEditBox->field_4C + 1) >= v16)
				{
					break;
				}

				++pEditBox->field_50;
			}

			--pEditBox->field_50;
		}
	}
}
