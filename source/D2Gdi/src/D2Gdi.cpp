#include "D2Gdi.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

#include <Fog.h>
#include <Storm.h>
#include <D2CMP.h>

static const int32_t gDrawModeLookup[8] = { 2, 2, 2, 1, 1, 0, 1, 0 };
static COLORREF gTextColor = 0x00FFFFFF;
static COLORREF gTextBkColor = 0;

static int32_t gnCropLeft = 0;
static int32_t gnCropRight = 0;
static HFONT ghFont = nullptr;
static uint8_t gSmackPaletteMap[256] = {};
static uint8_t* gpSmackBuffer = nullptr;
static RGBQUAD gColorTable[256] = {};
static D2PaletteTableStrc* gpPaletteTables = nullptr;
static D2PaletteTableStrc gPaletteTables = {};
static HWND ghWnd = nullptr;
static PALETTEENTRY gPaletteEntries[256] = {};
static int32_t gnSurfaceWidth = 0;
static HPALETTE ghPalette = nullptr;
static D2GfxSettingsStrc* gpGfxSettings = nullptr;
static HBITMAP ghBitmap = nullptr;
static int32_t gnSurfaceHeight = 0;
static const D2GfxHelperStrc* gpGfxHelpers = nullptr;
static BOOL gbSmackPaletteInitialized = FALSE;
static uint8_t* gpSurfaceBits = nullptr;
static int32_t gnWindowState = 0;
static CRITICAL_SECTION* gpCriticalSection = nullptr;

// SmackW32 dynamic dispatch helpers
using SmackOpenFn = D2SmackStrc* (__stdcall*)(const char* szFile, uint32_t dwFlags, int32_t nExtraBuf);
using SmackToBufferFn = void(__stdcall*)(D2SmackStrc* pSmack, uint32_t nLeft, uint32_t nTop, uint32_t nPitch, uint32_t nDestHeight, void* pBuf, uint32_t dwFlags);
using SmackWaitFn = uint32_t(__stdcall*)(D2SmackStrc* pSmack);
using SmackDoFrameFn = uint32_t(__stdcall*)(D2SmackStrc* pSmack);
using SmackNextFrameFn = void(__stdcall*)(D2SmackStrc* pSmack);
using SmackCloseFn = void(__stdcall*)(D2SmackStrc* pSmack);

static HMODULE D2GDI_GetSmackModule()
{
	HMODULE hMod = GetModuleHandleA("smackw32.dll");
	if (!hMod)
	{
		hMod = LoadLibraryA("smackw32.dll");
	}
	return hMod;
}

//1.12a: D2Gdi.0x6F875E40 (#10003)
const D2GfxHelperStrc* __fastcall D2GDI_GetHelpers()
{
	return gpGfxHelpers;
}

//1.12a: D2Gdi.0x6F875E50 (#10002)
void __fastcall D2GDI_GetSurfaceInfo(uint8_t** ppBits, int32_t* pHeight, int32_t* pWidth)
{
	D2_ASSERT(ppBits != nullptr);
	D2_ASSERT(pHeight != nullptr);
	D2_ASSERT(pWidth != nullptr);

	*ppBits = gpSurfaceBits;
	*pHeight = gnSurfaceHeight;
	*pWidth = gnSurfaceWidth;
}

//1.12a: D2Gdi.0x6F875EB0 (#10001)
void __fastcall D2GDI_GetPalette(HPALETTE* pPalette, RGBQUAD** ppColorTable)
{
	D2_ASSERT(pPalette != nullptr);
	D2_ASSERT(ppColorTable != nullptr);

	*pPalette = ghPalette;
	*ppColorTable = gColorTable;
}

//1.12a: D2Gdi.0x6F875EF0
static BOOL __fastcall D2GDI_ReturnFalse0()
{
	return FALSE;
}

//1.12a: D2Gdi.0x6F875F00
static void* __fastcall D2GDI_GetCacheResults()
{
	return nullptr;
}

//1.12a: D2Gdi.0x6F875F10
static int32_t __fastcall D2GDI_GammaCanBeControlled()
{
	return 0;
}

//1.12a: D2Gdi.0x6F875F20
static void __fastcall D2GDI_UpdatePerspective(int32_t nScaleX, int32_t nScaleY)
{
}

//1.12a: D2Gdi.0x6F875F30
static void __fastcall D2GDI_PerspectiveTransformScale(int32_t nPosX, int32_t nPosY, int32_t nAngle, int32_t* pXAdjust, int32_t* pYAdjust, int32_t bOrder)
{
}

//1.12a: D2Gdi.0x6F875F40
static void __fastcall D2GDI_PerspectiveTransform(int32_t nPosX, int32_t nPosY, int32_t nBais, int32_t* pXAdjust, int32_t* pYAdjust)
{
}

//1.12a: D2Gdi.0x6F875F50
static void __fastcall D2GDI_PerspectiveClearScale()
{
}

//1.12a: D2Gdi.0x6F875F60
static void __fastcall D2GDI_OpenSmackCutscene(const char* szFile, D2GameResolutionMode nResolutionMode, void* pfFrame)
{
}

//1.12a: D2Gdi.0x6F875F70
static BOOL __fastcall D2GDI_CheckCutScene()
{
	return FALSE;
}

//1.12a: D2Gdi.0x6F875F80
static BOOL __fastcall D2GDI_SetGamma(int32_t nGamma)
{
	return FALSE;
}

//1.12a: D2Gdi.0x6F875F90
static void __fastcall D2GDI_SetGlobalScale(int32_t nScaleFactor)
{
}

//1.12a: D2Gdi.0x6F875FB0
static void __fastcall D2GDI_SetPaletteTables(D2PaletteTableStrc* pPaletteTables)
{
	memcpy(&gPaletteTables, pPaletteTables, 0x48 * sizeof(uint32_t));
	gpPaletteTables = &gPaletteTables;
}

//1.12a: D2Gdi.0x6F875FD0
static void __fastcall D2GDI_SetPalette(LPPALETTEENTRY pPalette)
{
	for (int32_t i = 0; i < 256; ++i)
	{
		gColorTable[i].rgbRed = pPalette[i].peRed;
		gColorTable[i].rgbGreen = pPalette[i].peGreen;
		gColorTable[i].rgbBlue = pPalette[i].peBlue;
	}
	SetPaletteEntries(ghPalette, 0, 256, pPalette);
	memcpy(gPaletteEntries, pPalette, sizeof(gPaletteEntries));
}

//1.12a: D2Gdi.0x6F876030
static BOOL __fastcall D2GDI_GetDimensions(int32_t* pWidth, int32_t* pHeight)
{
	*pWidth = gnSurfaceWidth;
	*pHeight = gnSurfaceHeight;
	return TRUE;
}

//1.12a: D2Gdi.0x6F876040
static BOOL __fastcall D2GDI_GetBackBuffer(uint8_t* pBuffer)
{
	uint8_t* pSrcRow = gpSurfaceBits;
	for (int32_t y = 0; y < gnSurfaceHeight; ++y)
	{
		uint8_t* pDst = pBuffer;
		const uint8_t* pSrc = pSrcRow;
		for (int32_t x = 0; x < gnSurfaceWidth; ++x)
		{
			const uint8_t idx = *pSrc++;
			pDst[0] = gColorTable[idx].rgbRed;
			pDst[1] = gColorTable[idx].rgbGreen;
			pDst[2] = gColorTable[idx].rgbBlue;
			pDst += 3;
		}
		pBuffer += gnSurfaceWidth * 3;
		pSrcRow += gnSurfaceWidth;
	}
	return TRUE;
}

//1.12a: D2Gdi.0x6F8760C0
static BOOL __fastcall D2GDI_Blit()
{
	HDC hDC = GetDC(ghWnd);
	HDC hMemDC = CreateCompatibleDC(hDC);
	SelectObject(hMemDC, ghBitmap);
	SetDIBColorTable(hMemDC, 0, 256, gColorTable);
	SelectPalette(hDC, ghPalette, FALSE);
	RealizePalette(hDC);
	RECT rcClient = {};
	GetClientRect(ghWnd, &rcClient);
	const int32_t nDstW = rcClient.right - rcClient.left;
	const int32_t nDstH = rcClient.bottom - rcClient.top;
	if (nDstW > 0 && nDstH > 0 && (nDstW != gnSurfaceWidth || nDstH != gnSurfaceHeight))
	{
		SetStretchBltMode(hDC, COLORONCOLOR);
		StretchBlt(hDC, 0, 0, nDstW, nDstH, hMemDC, 0, 0, gnSurfaceWidth, gnSurfaceHeight, SRCCOPY);
	}
	else
	{
		BitBlt(hDC, 0, 0, gnSurfaceWidth, gnSurfaceHeight, hMemDC, 0, 0, SRCCOPY);
	}
	GdiFlush();
	DeleteDC(hMemDC);
	ReleaseDC(ghWnd, hDC);
	return TRUE;
}

//1.12a: D2Gdi.0x6F876160
static BOOL __fastcall D2GDI_EndDraw()
{
	D2_ASSERT(gpCriticalSection != nullptr);
	LeaveCriticalSection(gpCriticalSection);
	return TRUE;
}

//1.12a: D2Gdi.0x6F8761A0
static void __fastcall D2GDI_PauseSurface(HWND hWnd, D2GameResolutionMode nResolutionMode, int32_t nWindowState)
{
	D2_ASSERT(gpCriticalSection != nullptr);
	EnterCriticalSection(gpCriticalSection);
	gnWindowState = nWindowState;
	LeaveCriticalSection(gpCriticalSection);
}

//1.12a: D2Gdi.0x6F8761F0
static BOOL __fastcall D2GDI_ActivateWindow()
{
	return FALSE;
}

//1.12a: D2Gdi.0x6F876200
static void __fastcall D2GDI_SetAmbientColor(uint8_t nRed, uint8_t nGreen, uint8_t nBlue)
{
}

//1.12a: D2Gdi.0x6F876210
static BOOL __fastcall D2GDI_SetOption(int32_t nOption, int32_t nValue)
{
	return FALSE;
}

//1.12a: D2Gdi.0x6F876250
static void D2GDI_CreateDIB(D2GameResolutionMode nResolutionMode)
{
	D2_ASSERT(gpSurfaceBits == nullptr);

	int32_t nWidth = 800;
	int32_t nHeight = 600;
	if (nResolutionMode == D2GAMERES_640x480)
	{
		nWidth = 640;
		nHeight = 480;
	}
	else if (nResolutionMode == D2GAMERES_1344x700)
	{
		nWidth = 1344;
		nHeight = 700;
	}

	gnSurfaceWidth = nWidth;
	gnSurfaceHeight = nHeight;

	uint8_t bmiBuf[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)] = {};
	BITMAPINFO* pBmi = reinterpret_cast<BITMAPINFO*>(bmiBuf);
	pBmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBmi->bmiHeader.biWidth = nWidth;
	pBmi->bmiHeader.biHeight = -nHeight;
	pBmi->bmiHeader.biPlanes = 1;
	pBmi->bmiHeader.biBitCount = 8;
	pBmi->bmiHeader.biCompression = BI_RGB;

	HDC hDC = GetDC(nullptr);
	ghBitmap = CreateDIBSection(hDC, pBmi, DIB_RGB_COLORS, reinterpret_cast<void**>(&gpSurfaceBits), nullptr, 0);
	ReleaseDC(nullptr, hDC);

	gpGfxHelpers->pfFillYBufferTable(gpSurfaceBits, gnSurfaceWidth, gnSurfaceHeight, 0);
}

//1.12a: D2Gdi.0x6F876340
static void __fastcall D2GDI_ClearCaches()
{
}

//1.12a: D2Gdi.0x6F876350
static BOOL __fastcall D2GDI_Close()
{
	if (gpCriticalSection)
	{
		DeleteCriticalSection(gpCriticalSection);
		D2_FREE(gpCriticalSection);
		gpCriticalSection = nullptr;
	}
	return TRUE;
}

//1.12a: D2Gdi.0x6F876390
static BOOL __fastcall D2GDI_Init(D2GfxSettingsStrc* pSettings, const D2GfxHelperStrc* pHelpers)
{
	gpGfxSettings = pSettings;
	gpGfxHelpers = pHelpers;
	gpCriticalSection = static_cast<CRITICAL_SECTION*>(D2_ALLOC(sizeof(CRITICAL_SECTION)));
	InitializeCriticalSection(gpCriticalSection);
	return TRUE;
}

//1.12a: D2Gdi.0x6F8763D0
static BOOL __fastcall D2GDI_Detect(HINSTANCE hInstance)
{
	return TRUE;
}

//1.12a: D2Gdi.0x6F876560
static BOOL __fastcall D2GDI_ChangeRes(HWND hWnd, D2GameResolutionMode nResolutionMode)
{
	DeleteObject(ghBitmap);
	gpSurfaceBits = nullptr;
	gpGfxHelpers->pfFillYBufferTable(nullptr, 0, 0, 0);
	D2GDI_CreateDIB(nResolutionMode);

	gnCropLeft = 0;
	if (nResolutionMode == D2GAMERES_640x480)
	{
		gnCropRight = 640;
	}
	else if (nResolutionMode == D2GAMERES_1344x700)
	{
		gnCropRight = 1344;
	}
	else
	{
		gnCropRight = 800;
	}
	return TRUE;
}

//1.12a: D2Gdi.0x6F8765D0
static BOOL __fastcall D2GDI_CloseSurface()
{
	DeleteObject(ghFont);
	DeleteObject(ghBitmap);
	gpSurfaceBits = nullptr;
	gpGfxHelpers->pfFillYBufferTable(nullptr, 0, 0, 0);
	return TRUE;
}

//1.12a: D2Gdi.0x6F876610
static BOOL __fastcall D2GDI_CreateSurface(HWND hWnd, D2GameResolutionMode nResolutionMode)
{
	ghWnd = hWnd;
	D2GDI_CreateDIB(nResolutionMode);
	memset(gPaletteEntries, 0, sizeof(gPaletteEntries));

	uint8_t logPalBuf[sizeof(LOGPALETTE) + 256 * sizeof(PALETTEENTRY)] = {};
	LOGPALETTE* pLogPal = reinterpret_cast<LOGPALETTE*>(logPalBuf);
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = 256;
	ghPalette = CreatePalette(pLogPal);

	ghFont = CreateFontA(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	gnCropLeft = 0;
	if (nResolutionMode == D2GAMERES_640x480)
	{
		gnCropRight = 640;
	}
	else if (nResolutionMode == D2GAMERES_1344x700)
	{
		gnCropRight = 1344;
	}
	else
	{
		gnCropRight = 800;
	}
	return TRUE;
}

//1.12a: D2Gdi.0x6F8766D0
static BOOL __fastcall D2GDI_StartDraw(int32_t bClear, uint8_t nRed, uint8_t nGreen, uint8_t nBlue)
{
	if (gnWindowState != 0)
	{
		return FALSE;
	}

	D2_ASSERT(gpCriticalSection != nullptr);
	EnterCriticalSection(gpCriticalSection);

	if (bClear)
	{
		memset(gpSurfaceBits, 0, (gnSurfaceHeight - 47) * gnSurfaceWidth);
	}
	return TRUE;
}

//1.12a: D2Gdi.0x6F876750
static void D2GDI_BlitSmackerFrame(D2SmackerContextStrc* pContext, int32_t nWidth, int32_t nHeight)
{
	uint8_t* pDstRow = gpSurfaceBits + (pContext->nYPos - nHeight) * gnSurfaceWidth + pContext->nXPos;
	const uint8_t* pSrcRow = gpSmackBuffer;
	for (int32_t y = 0; y < nHeight; ++y)
	{
		for (int32_t x = 0; x < nWidth; ++x)
		{
			pDstRow[x] = gSmackPaletteMap[pSrcRow[x]];
		}
		pDstRow += gnSurfaceWidth;
		pSrcRow += nWidth;
	}
}

//1.12a: D2Gdi.0x6F8767C0
static void __fastcall D2GDI_CloseSmacker(void* pContextVoid)
{
	D2SmackerContextStrc* pContext = static_cast<D2SmackerContextStrc*>(pContextVoid);
	if (gpSmackBuffer)
	{
		D2_FREE(gpSmackBuffer);
		gpSmackBuffer = nullptr;
	}
	if (pContext->cMode == 's' && pContext->pSmack)
	{
		if (HMODULE hMod = D2GDI_GetSmackModule())
		{
			if (auto pfSmackClose = reinterpret_cast<SmackCloseFn>(GetProcAddress(hMod, "_SmackClose@4")))
			{
				pfSmackClose(pContext->pSmack);
			}
		}
		pContext->pSmack = nullptr;
	}
	pContext->cMode = 0;
}

//1.12a: D2Gdi.0x6F876810
static void __fastcall D2GDI_PlaySmacker(void* pContextVoid)
{
	D2SmackerContextStrc* pContext = static_cast<D2SmackerContextStrc*>(pContextVoid);
	if (pContext->cMode == 'p')
	{
		if (gnWindowState == 0)
		{
			D2GDI_BlitSmackerFrame(pContext, 468, 60);
		}
		return;
	}

	if (pContext->cMode == 's' && pContext->pSmack)
	{
		D2SmackStrc* pSmack = pContext->pSmack;
		HMODULE hMod = D2GDI_GetSmackModule();
		if (!hMod)
		{
			return;
		}
		auto pfSmackWait = reinterpret_cast<SmackWaitFn>(GetProcAddress(hMod, "_SmackWait@4"));
		auto pfSmackDoFrame = reinterpret_cast<SmackDoFrameFn>(GetProcAddress(hMod, "_SmackDoFrame@4"));
		auto pfSmackNextFrame = reinterpret_cast<SmackNextFrameFn>(GetProcAddress(hMod, "_SmackNextFrame@4"));

		if (pfSmackWait && !pfSmackWait(pSmack))
		{
			if (!ghWnd)
			{
				return;
			}
			if (pSmack->bNewPalette)
			{
				const uint8_t* pPal = pSmack->aPalette;
				for (int32_t i = 0; i < 256; ++i)
				{
					gSmackPaletteMap[i] = static_cast<uint8_t>(D2CMP_GetNearestPaletteIndex(gPaletteEntries, 256, pPal[0], pPal[1], pPal[2]));
					pPal += 3;
				}
				gbSmackPaletteInitialized = TRUE;
			}
			if (pfSmackDoFrame)
			{
				pfSmackDoFrame(pSmack);
			}
			if (pSmack->nFrameNum == pSmack->nFrames)
			{
				D2GDI_CloseSmacker(pContext);
			}
			else if (pfSmackNextFrame)
			{
				pfSmackNextFrame(pSmack);
			}
		}

		if (gnWindowState == 0 && gbSmackPaletteInitialized)
		{
			D2GDI_BlitSmackerFrame(pContext, pSmack->nWidth, pSmack->nHeight);
		}
	}
}

//1.12a: D2Gdi.0x6F876950
static void __fastcall D2GDI_DecodeSmacker(const char* szSmacker, uint8_t* pBuffer, int32_t nVersion)
{
	D2SmackerContextStrc* pContext = reinterpret_cast<D2SmackerContextStrc*>(pBuffer);
	const char cMode = static_cast<char>(nVersion);
	pContext->cMode = cMode;

	if (cMode == 's')
	{
		if (!pContext->pSmack)
		{
			HMODULE hMod = D2GDI_GetSmackModule();
			auto pfSmackOpen = hMod ? reinterpret_cast<SmackOpenFn>(GetProcAddress(hMod, "_SmackOpen@12")) : nullptr;
			auto pfSmackToBuffer = hMod ? reinterpret_cast<SmackToBufferFn>(GetProcAddress(hMod, "_SmackToBuffer@28")) : nullptr;

			D2SmackStrc* pSmack = pfSmackOpen ? pfSmackOpen(szSmacker, 0xFE000, -1) : nullptr;
			pContext->pSmack = pSmack;
			if (!pSmack)
			{
				D2GDI_CloseSmacker(pContext);
				return;
			}

			const uint32_t nBufSize = pSmack->nWidth * pSmack->nHeight;
			gpSmackBuffer = static_cast<uint8_t*>(D2_CALLOC(nBufSize));
			if (pfSmackToBuffer)
			{
				pfSmackToBuffer(pSmack, 0, 0, pSmack->nWidth, pSmack->nHeight, gpSmackBuffer, 0);
			}
			gbSmackPaletteInitialized = FALSE;
		}
	}
	else if (cMode == 'p')
	{
		gpSmackBuffer = static_cast<uint8_t*>(D2_CALLOC(0x6DB0));
		void* pFileBuffer = nullptr;
		DWORD dwFileSize = 0;
		if (!SFileLoadFileEx(0, const_cast<char*>(szSmacker), reinterpret_cast<int>(&pFileBuffer), reinterpret_cast<int>(&dwFileSize), 0, 3, 0) || !pFileBuffer)
		{
			D2GDI_CloseSmacker(pContext);
			return;
		}

		PALETTEENTRY bmpPalette[256] = {};
		int32_t nWidth = 0;
		int32_t nHeight = 0;
		int32_t nBpp = 0;
		const BOOL bDecoded = SBmpDecodeImage(2, static_cast<int*>(pFileBuffer), dwFileSize, reinterpret_cast<int>(bmpPalette), reinterpret_cast<int>(gpSmackBuffer), 0x6DB0, reinterpret_cast<int>(&nWidth), reinterpret_cast<int>(&nHeight), reinterpret_cast<int>(&nBpp));
		SMemFree(pFileBuffer, __FILE__, __LINE__, 0);

		if (!bDecoded || nWidth != 468 || nHeight != 60 || nBpp != 8)
		{
			D2GDI_CloseSmacker(pContext);
			return;
		}

		for (int32_t i = 0; i < 256; ++i)
		{
			gSmackPaletteMap[i] = static_cast<uint8_t>(D2CMP_GetNearestPaletteIndex(gPaletteEntries, 256, bmpPalette[i].peRed, bmpPalette[i].peGreen, bmpPalette[i].peBlue));
		}
	}
}

//1.12a: D2Gdi.0x6F876BA0
static void __fastcall D2GDI_UtilFilledRect(RECT* pRect, uint8_t nPaletteIndex)
{
	D2_ASSERT(pRect->left <= pRect->right);
	D2_ASSERT(pRect->top <= pRect->bottom);

	if (pRect->right < 0 || pRect->left >= gnSurfaceWidth || pRect->bottom < 0 || pRect->top >= gnSurfaceHeight)
	{
		return;
	}

	if (pRect->left < 0) pRect->left = 0;
	if (pRect->right >= gnSurfaceWidth) pRect->right = gnSurfaceWidth - 1;
	if (pRect->top < 0) pRect->top = 0;
	if (pRect->bottom >= gnSurfaceHeight) pRect->bottom = gnSurfaceHeight - 1;

	const int32_t nWidth = pRect->right - pRect->left;
	const int32_t nHeight = pRect->bottom - pRect->top;
	if (nWidth == 0 || nHeight == 0)
	{
		return;
	}

	uint8_t* pDstRow = gpSurfaceBits + pRect->top * gnSurfaceWidth + pRect->left;
	for (int32_t y = 0; y < nHeight; ++y)
	{
		memset(pDstRow, nPaletteIndex, nWidth);
		pDstRow += gnSurfaceWidth;
	}
}

//1.12a: D2Gdi.0x6F877120
static void __fastcall D2GDI_DrawLine(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, uint8_t nAlpha)
{
	int32_t dx = nXEnd - nXStart;
	int32_t dy = nYEnd - nYStart;
	const int32_t stepX = (dx < 0) ? -1 : 1;
	const int32_t stepY = (dy < 0) ? -1 : 1;
	dx = std::abs(dx);
	dy = std::abs(dy);

	int32_t x = nXStart;
	int32_t y = nYStart;
	const uint8_t nColorByte = static_cast<uint8_t>(dwColor);

	if (x >= 0 && x < gnSurfaceWidth && y >= 0 && y < gnSurfaceHeight)
	{
		gpSurfaceBits[y * gnSurfaceWidth + x] = nColorByte;
	}

	int32_t err = 0;
	if (dx < dy)
	{
		for (int32_t i = 0; i < dy; ++i)
		{
			y += stepY;
			err += dx;
			if (err > dy)
			{
				x += stepX;
				err -= dy;
			}
			if (x >= 0 && x < gnSurfaceWidth && y >= 0 && y < gnSurfaceHeight)
			{
				gpSurfaceBits[y * gnSurfaceWidth + x] = nColorByte;
			}
		}
	}
	else if (dx > 0)
	{
		for (int32_t i = 0; i < dx; ++i)
		{
			x += stepX;
			err += dy;
			if (err > dx)
			{
				y += stepY;
				err -= dx;
			}
			if (x >= 0 && x < gnSurfaceWidth && y >= 0 && y < gnSurfaceHeight)
			{
				gpSurfaceBits[y * gnSurfaceWidth + x] = nColorByte;
			}
		}
	}
}

//1.12a: D2Gdi.0x6F876CC0
static void __fastcall D2GDI_DrawPointLine(const POINT* pEnd, const POINT* pStart, uint8_t nPaletteIndex)
{
	D2GDI_DrawLine(pStart->x, pStart->y, pEnd->x, pEnd->y, nPaletteIndex, 0xFF);
}

//1.12a: D2Gdi.0x6F876EB0
static int32_t D2GDI_CheckBlendedShadows()
{
	return gpGfxSettings->bBlendedShadows;
}

//1.12a: D2Gdi.0x6F876EF0
static void __fastcall D2GDI_ClearScreen(int32_t bPartial)
{
	const int32_t nHeight = bPartial ? (gnSurfaceHeight - 47) : gnSurfaceHeight;
	memset(gpSurfaceBits, 0, nHeight * gnSurfaceWidth);
}

//1.12a: D2Gdi.0x6F876F20
static void __fastcall D2GDI_UtilPoint(POINT* pPoint, uint8_t nSize, uint8_t nPaletteIndex)
{
	if (!nSize)
	{
		return;
	}
	const int32_t nRadius = nSize - 1;
	RECT rect = {};
	SetRect(&rect, pPoint->x - nRadius, pPoint->y - nRadius, pPoint->x + nRadius + 1, pPoint->y + nRadius + 1);
	D2GDI_UtilFilledRect(&rect, nPaletteIndex);
}

//1.12a: D2Gdi.0x6F876F70
static void __fastcall D2GDI_UtilRect(RECT* pRect, uint8_t nPaletteIndex)
{
	D2_ASSERT(pRect->left <= pRect->right);
	D2_ASSERT(pRect->top <= pRect->bottom);

	if (pRect->left == pRect->right || pRect->top == pRect->bottom)
	{
		return;
	}

	const POINT ptTopLeft = { pRect->left, pRect->top };
	const POINT ptTopRight = { pRect->right, pRect->top };
	const POINT ptBottomRight = { pRect->right, pRect->bottom };
	const POINT ptBottomLeft = { pRect->left, pRect->bottom };

	D2GDI_DrawPointLine(&ptTopRight, &ptTopLeft, nPaletteIndex);
	D2GDI_DrawPointLine(&ptBottomRight, &ptTopRight, nPaletteIndex);
	D2GDI_DrawPointLine(&ptBottomLeft, &ptBottomRight, nPaletteIndex);
	D2GDI_DrawPointLine(&ptTopLeft, &ptBottomLeft, nPaletteIndex);
}

//1.12a: D2Gdi.0x6F877040
static void __fastcall D2GDI_UtilDiamond(RECT* pRect, uint8_t nPaletteIndex)
{
	D2_ASSERT(pRect->left <= pRect->right);
	D2_ASSERT(pRect->top <= pRect->bottom);

	const int32_t nWidth = pRect->right - pRect->left;
	const int32_t nHeight = pRect->bottom - pRect->top;
	if (!nWidth || !nHeight)
	{
		return;
	}

	const int32_t nMidX = pRect->left + (nWidth >> 1);
	const int32_t nMidY = pRect->top + (nHeight >> 1);

	const POINT ptTop = { nMidX, pRect->top };
	const POINT ptRight = { pRect->right, nMidY };
	const POINT ptBottom = { nMidX, pRect->bottom };
	const POINT ptLeft = { pRect->left, nMidY };

	D2GDI_DrawPointLine(&ptRight, &ptTop, nPaletteIndex);
	D2GDI_DrawPointLine(&ptBottom, &ptRight, nPaletteIndex);
	D2GDI_DrawPointLine(&ptLeft, &ptBottom, nPaletteIndex);
	D2GDI_DrawPointLine(&ptTop, &ptLeft, nPaletteIndex);
}

//1.12a: D2Gdi.0x6F877A30
static void* __fastcall D2GDI_GetDrawPaletteTable(DrawMode eDrawMode, int32_t* pOutMode)
{
	*pOutMode = (static_cast<uint32_t>(eDrawMode) < 8) ? gDrawModeLookup[eDrawMode] : 0;
	switch (eDrawMode)
	{
	case DRAWMODE_TRANS25:
		return gpPaletteTables->transPalettes[2];
	case DRAWMODE_TRANS50:
		return gpPaletteTables->transPalettes[1];
	case DRAWMODE_TRANS75:
		return gpPaletteTables->transPalettes[0];
	case DRAWMODE_MODULATE:
		return gpPaletteTables->pScreenPalette;
	case DRAWMODE_BURN:
		return gpPaletteTables->pLuminancePalette;
	case DRAWMODE_NORMAL:
		return nullptr;
	case DRAWMODE_TRANSHIGHLIGHT:
		return gpPaletteTables->pDarkBlendPalette;
	default:
		return nullptr;
	}
}

//1.12a: D2Gdi.0x6F877290
static void __fastcall D2GDI_DrawBox(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, DrawMode eDrawMode)
{
	int32_t nBoxMode = 0;
	const uint8_t* pTable = static_cast<const uint8_t*>(D2GDI_GetDrawPaletteTable(eDrawMode, &nBoxMode));

	const int32_t maxX = gnSurfaceWidth - 1;
	const int32_t x0 = std::min(std::max(nXStart, 0), maxX);
	const int32_t x1 = std::min(std::max(nXEnd, 0), maxX);
	if (x0 == x1)
	{
		return;
	}

	const int32_t maxY = gnSurfaceHeight - 1;
	const int32_t y0 = std::min(std::max(nYStart, 0), maxY);
	const int32_t y1 = std::min(std::max(nYEnd, 0), maxY);
	if (y1 == y0)
	{
		return;
	}
	D2_ASSERT(y1 > y0);

	const int32_t nWidth = x1 - x0;
	const int32_t nHeight = y1 - y0;
	uint8_t* pRow = gpSurfaceBits + y0 * gnSurfaceWidth + x0;
	const uint8_t nColorByte = static_cast<uint8_t>(dwColor);

	if (nBoxMode == 1)
	{
		for (int32_t y = 0; y < nHeight; ++y)
		{
			for (int32_t x = 0; x < nWidth; ++x)
			{
				pRow[x] = pTable[pRow[x]];
			}
			pRow += gnSurfaceWidth;
		}
	}
	else if (nBoxMode == 2)
	{
		for (int32_t y = 0; y < nHeight; ++y)
		{
			for (int32_t x = 0; x < nWidth; ++x)
			{
				pRow[x] = pTable[(static_cast<uint32_t>(pRow[x]) << 8) + nColorByte];
			}
			pRow += gnSurfaceWidth;
		}
	}
	else
	{
		for (int32_t y = 0; y < nHeight; ++y)
		{
			memset(pRow, nColorByte, nWidth);
			pRow += gnSurfaceWidth;
		}
	}
}

//1.12a: D2Gdi.0x6F877430
static void __fastcall D2GDI_DrawBoxAlpha(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, uint8_t nAlpha)
{
	if (nAlpha >= 0xC0)
	{
		D2GDI_DrawBox(nXStart, nYStart, nXEnd, nYEnd, dwColor, DRAWMODE_NORMAL);
	}
	else if (nAlpha >= 0x80)
	{
		D2GDI_DrawBox(nXStart, nYStart, nXEnd, nYEnd, dwColor, DRAWMODE_TRANS75);
	}
	else if (nAlpha >= 0x40)
	{
		D2GDI_DrawBox(nXStart, nYStart, nXEnd, nYEnd, dwColor, DRAWMODE_TRANS50);
	}
	else if (nAlpha > 0)
	{
		D2GDI_DrawBox(nXStart, nYStart, nXEnd, nYEnd, dwColor, DRAWMODE_TRANS25);
	}
}

//1.12a: D2Gdi.0x6F877480
static int32_t __fastcall D2GDI_TileDrawLit(D2TileLibraryEntryStrc* pTile, int32_t nPosX, int32_t nPosY, D2GfxLightStrc* pLight, int32_t nScreenPanels)
{
	if (!D2CMP_TileValidate(pTile, TRUE, FALSE))
	{
		return 0;
	}

	int32_t minX = -32;
	int32_t maxX = gnSurfaceWidth;
	const int32_t maxY = gnSurfaceHeight + 32;
	if (nScreenPanels == 1)
	{
		maxX -= gnSurfaceWidth / 2;
	}
	else if (nScreenPanels == 2)
	{
		minX = gnSurfaceWidth / 2 - 32;
	}

	int32_t bDrawn = 0;
	const uint8_t* pLightBytes = reinterpret_cast<const uint8_t*>(pLight);
	for (int32_t i = 0; i < pTile->nBlocks; ++i)
	{
		D2TileLibraryBlockStrc* pBlock = &pTile->pBlock[i];
		const int32_t x = nPosX + pBlock->nPosX;
		const int32_t y = nPosY + pBlock->nPosY;
		if (x >= minX && x < maxX && y >= -32 && y < maxY)
		{
			const int32_t idx = pBlock->nPosX >> 5;
			int32_t lightVals[4] = {
				pLightBytes[idx * 4],
				pLightBytes[idx * 4 + 4],
				pLightBytes[idx * 4 + 4],
				pLightBytes[idx * 4]
			};
			bDrawn = 1;
			gpGfxHelpers->pfTileDrawLit(pBlock->dwOffset_pData, x, y, lightVals);
		}
	}
	return bDrawn;
}

//1.12a: D2Gdi.0x6F8775A0
static int32_t __fastcall D2GDI_TileDrawTrans(D2TileLibraryEntryStrc* pTile, int32_t nPosX, int32_t nPosY, D2GfxLightStrc* pLight, int32_t nScreenPanels, uint8_t nAlpha)
{
	if (!D2CMP_TileValidate(pTile, TRUE, FALSE))
	{
		return 0;
	}

	int32_t minX = -32;
	int32_t maxX = gnSurfaceWidth;
	const int32_t maxY = gnSurfaceHeight + 32;
	if (nScreenPanels == 1)
	{
		maxX -= gnSurfaceWidth / 2;
	}
	else if (nScreenPanels == 2)
	{
		minX = gnSurfaceWidth / 2 - 32;
	}

	int32_t bDrawn = 0;
	const uint8_t* pLightBytes = reinterpret_cast<const uint8_t*>(pLight);
	for (int32_t i = 0; i < pTile->nBlocks; ++i)
	{
		D2TileLibraryBlockStrc* pBlock = &pTile->pBlock[i];
		const int32_t x = nPosX + pBlock->nPosX;
		const int32_t y = nPosY + pBlock->nPosY;
		if (x >= minX && x < maxX && y >= -32 && y < maxY)
		{
			const int32_t idx = pBlock->nPosX >> 5;
			int32_t lightVals[4] = {
				pLightBytes[idx * 4],
				pLightBytes[idx * 4 + 4],
				pLightBytes[idx * 4 + 4],
				pLightBytes[idx * 4]
			};
			bDrawn = 1;
			gpGfxHelpers->pfDrawRoofTile(pBlock->dwOffset_pData, x, y, lightVals, nAlpha);
		}
	}
	return bDrawn;
}

//1.12a: D2Gdi.0x6F8776D0
static int32_t __fastcall D2GDI_ShadowTileDraw(D2TileLibraryEntryStrc* pTile, int32_t nPosX, int32_t nPosY, DrawMode eDrawMode, int32_t nScreenPanels)
{
	if (!D2CMP_TileValidate(pTile, TRUE, FALSE))
	{
		return 0;
	}

	int32_t minX = -32;
	int32_t maxX = gnSurfaceWidth;
	const int32_t maxY = gnSurfaceHeight + 32;
	if (nScreenPanels == 1)
	{
		maxX -= gnSurfaceWidth / 2;
	}
	else if (nScreenPanels == 2)
	{
		minX = gnSurfaceWidth / 2 - 32;
	}

	int32_t bDrawn = 0;
	for (int32_t i = 0; i < pTile->nBlocks; ++i)
	{
		D2TileLibraryBlockStrc* pBlock = &pTile->pBlock[i];
		const int32_t x = nPosX + pBlock->nPosX;
		const int32_t y = nPosY + pBlock->nPosY;
		if (x >= minX && x < maxX && y >= -32 && y < maxY)
		{
			bDrawn = 1;
			if (D2GDI_CheckBlendedShadows())
			{
				gpGfxHelpers->pfDrawBlendedVisTile(pBlock->dwOffset_pData, x, y, 0xC0);
			}
			else
			{
				gpGfxHelpers->pfDrawVisTile(pBlock->dwOffset_pData, x, y);
			}
		}
	}
	return bDrawn;
}

//1.12a: D2Gdi.0x6F8777F0
static void __fastcall D2GDI_DebugDraw(uint32_t* pLight, uint32_t* pPlayerLight, int32_t nPosX, int32_t nPosY)
{
}

//1.12a: D2Gdi.0x6F877800
static void __fastcall D2GDI_OutputString(int32_t nPosX, int32_t nPosY, const char* szFormat, va_list va)
{
	char szBuffer[512] = {};
	wvsprintfA(szBuffer, szFormat, va);

	HDC hDC = GetDC(nullptr);
	HDC hMemDC = CreateCompatibleDC(hDC);
	SelectObject(hMemDC, ghBitmap);
	SelectObject(hMemDC, ghFont);
	SetTextColor(hMemDC, gTextColor);
	SetBkColor(hMemDC, gTextBkColor);
	SetBkMode(hMemDC, TRANSPARENT);

	RECT rect = {};
	SetRect(&rect, nPosX, nPosY, gnSurfaceWidth - nPosX, gnSurfaceHeight - nPosY);
	DrawTextA(hMemDC, szBuffer, -1, &rect, DT_LEFT);

	DeleteDC(hMemDC);
	ReleaseDC(nullptr, hDC);
}

//1.12a: D2Gdi.0x6F877910
static void __fastcall D2GDI_Distort(int32_t nPosX, int32_t nPosY)
{
	D2_ASSERT(nPosX > 0 && nPosX < gnSurfaceWidth - 80);
	D2_ASSERT(nPosY > 0 && nPosY < gnSurfaceHeight - 80);

	D2CMP_CmpDoLensEffect(gpSurfaceBits + nPosY * gnSurfaceWidth + nPosX, gnSurfaceWidth);
}

//1.12a: D2Gdi.0x6F877970
static void __fastcall D2GDI_DebugFillBackBuffer(int32_t nPosX, int32_t nPosY)
{
}

//1.12a: D2Gdi.0x6F877AB0
static void __fastcall D2GDI_CelDrawShadow(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY)
{
	if (!D2CMP_SpriteValidate(pData, 0, TRUE))
	{
		return;
	}

	D2CellFileStrc* pCell = reinterpret_cast<D2CellFileStrc*>(pData->pCurrentCell);
	const int32_t nWidth = D2CMP_CelGetWidth(pCell);
	const int32_t nHeight = D2CMP_CelGetHeight(pCell);
	const int32_t nOffsetX = D2CMP_CelGetOffsetX(pCell);
	const int32_t nOffsetY = D2CMP_CelGetOffsetY(pCell);

	const int32_t xStart = nPosX - nHeight + nOffsetX;
	if (xStart >= gnSurfaceWidth || xStart + nWidth + nHeight < 0)
	{
		return;
	}

	const int32_t yEnd = nPosY + nOffsetY;
	if (yEnd < 0 || yEnd - (nHeight >> 1) >= gnSurfaceHeight)
	{
		return;
	}

	uint8_t* pShadowTable = gpGfxSettings->bBlendedShadows ? static_cast<uint8_t*>(gpPaletteTables->transPalettes[0]) : nullptr;
	D2CMP_CelDrawShadow(pData, nPosX, nPosY, gnCropLeft, gnCropRight, gpSurfaceBits, gnSurfaceHeight, gnSurfaceWidth, pShadowTable, 0);
}

//1.12a: D2Gdi.0x6F877B70
static void __fastcall D2GDI_CelDrawEx(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, int32_t nSkipLines, int32_t nDrawLines, DrawMode eDrawMode)
{
	if (!D2CMP_SpriteValidate(pData, 0, TRUE))
	{
		return;
	}

	D2CellFileStrc* pCell = reinterpret_cast<D2CellFileStrc*>(pData->pCurrentCell);
	const int32_t nWidth = D2CMP_CelGetWidth(pCell);
	const int32_t nHeight = D2CMP_CelGetHeight(pCell);
	const int32_t nOffsetX = D2CMP_CelGetOffsetX(pCell);
	const int32_t nOffsetY = D2CMP_CelGetOffsetY(pCell);

	const int32_t xStart = nPosX + nOffsetX;
	if (xStart >= gnSurfaceWidth || xStart + nWidth < 0)
	{
		return;
	}

	const int32_t yEnd = nPosY + nOffsetY;
	if (yEnd < 0 || yEnd - nHeight >= gnSurfaceHeight)
	{
		return;
	}

	int32_t nMode = 0;
	void* pTable = D2GDI_GetDrawPaletteTable(eDrawMode, &nMode);
	D2CMP_CelDrawEx(pData, nPosX, nPosY, gnCropLeft, gnCropRight, gpSurfaceBits, gnSurfaceHeight, gnSurfaceWidth, 0, reinterpret_cast<int32_t>(pTable), nSkipLines, nDrawLines, 0);
}

//1.12a: D2Gdi.0x6F877C20
static void __fastcall D2GDI_CelDrawColor(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, DrawMode eDrawMode, int32_t nGlobalPaletteShift)
{
	if (!D2CMP_SpriteValidate(pData, 0, TRUE))
	{
		return;
	}

	D2CellFileStrc* pCell = reinterpret_cast<D2CellFileStrc*>(pData->pCurrentCell);
	const int32_t nWidth = D2CMP_CelGetWidth(pCell);
	const int32_t nHeight = D2CMP_CelGetHeight(pCell);
	const int32_t nOffsetX = D2CMP_CelGetOffsetX(pCell);
	const int32_t nOffsetY = D2CMP_CelGetOffsetY(pCell);

	const int32_t xStart = nPosX + nOffsetX;
	if (xStart >= gnSurfaceWidth || xStart + nWidth < 0)
	{
		return;
	}

	const int32_t yEnd = nPosY + nOffsetY;
	if (yEnd < 0 || yEnd - nHeight >= gnSurfaceHeight)
	{
		return;
	}

	int32_t nMode = 0;
	void* pDrawTable = D2GDI_GetDrawPaletteTable(eDrawMode, &nMode);
	uint8_t* pDarkenTable = nullptr;
	if ((dwGamma & 0xFF) != 0xFF)
	{
		pDarkenTable = static_cast<uint8_t*>(gpPaletteTables->shadowLightGammaPalettes[(dwGamma & 0xFF) >> 3]);
	}
	if (eDrawMode == DRAWMODE_TRANSHIGHLIGHT)
	{
		pDarkenTable = gpPaletteTables->unk0x118;
	}

	void* pShiftTable = nullptr;
	if (nGlobalPaletteShift != 0)
	{
		pShiftTable = gpPaletteTables->standardShiftPalettes[nGlobalPaletteShift];
	}

	D2CMP_CelDraw(pData, nPosX, nPosY, gnCropLeft, gpSurfaceBits, gnSurfaceHeight, gnCropRight, gnSurfaceWidth, pDarkenTable, reinterpret_cast<int32_t>(pDrawTable), pShiftTable);
}

//1.12a: D2Gdi.0x6F877D30
static void __fastcall D2GDI_CelDraw(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, DrawMode eDrawMode, uint8_t* pPalette)
{
	if (!D2CMP_SpriteValidate(pData, 0, TRUE))
	{
		return;
	}

	D2CellFileStrc* pCell = reinterpret_cast<D2CellFileStrc*>(pData->pCurrentCell);
	const int32_t nWidth = D2CMP_CelGetWidth(pCell);
	const int32_t nHeight = D2CMP_CelGetHeight(pCell);
	const int32_t nOffsetX = D2CMP_CelGetOffsetX(pCell);
	const int32_t nOffsetY = D2CMP_CelGetOffsetY(pCell);

	const int32_t xStart = nPosX + nOffsetX;
	if (xStart >= gnSurfaceWidth || xStart + nWidth < 0)
	{
		return;
	}

	const int32_t yEnd = nPosY + nOffsetY;
	if (yEnd < 0 || yEnd - nHeight >= gnSurfaceHeight)
	{
		return;
	}

	int32_t nMode = 0;
	void* pDrawTable = D2GDI_GetDrawPaletteTable(eDrawMode, &nMode);
	uint8_t* pDarkenTable = nullptr;
	if ((dwGamma & 0xFF) != 0xFF)
	{
		pDarkenTable = static_cast<uint8_t*>(gpPaletteTables->shadowLightGammaPalettes[(dwGamma & 0xFF) >> 3]);
	}
	if (eDrawMode == DRAWMODE_TRANSHIGHLIGHT)
	{
		pDarkenTable = gpPaletteTables->unk0x118;
	}

	D2CMP_CelDraw(pData, nPosX, nPosY, gnCropLeft, gpSurfaceBits, gnSurfaceHeight, gnCropRight, gnSurfaceWidth, pDarkenTable, reinterpret_cast<int32_t>(pDrawTable), pPalette);
}

//1.12a: D2Gdi.0x6F877E20
static void __fastcall D2GDI_CelFlatSpriteDraw(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, DrawMode eDrawMode, int32_t nScreenMode, uint8_t* pPalette)
{
	D2GDI_CelDraw(pData, nPosX, nPosY, dwGamma, eDrawMode, pPalette);
}

//1.12a: D2Gdi.0x6F877E40
static BOOL D2GDI_ComputeClipRect(int32_t nPosX, int32_t nPosY, D2CellFileStrc* pCell, const RECT* pCropRect, RECT* pOutClip)
{
	const int32_t nWidth = D2CMP_CelGetWidth(pCell);
	const int32_t nHeight = D2CMP_CelGetHeight(pCell);

	const int32_t clipLeft = std::max(0L, pCropRect->left);
	const int32_t clipRight = std::min(static_cast<LONG>(gnSurfaceWidth), pCropRect->top);
	if (clipLeft > clipRight)
	{
		return TRUE;
	}

	const int32_t xStart = nPosX + D2CMP_CelGetOffsetX(pCell);
	if (xStart >= clipRight || xStart + nWidth < clipLeft)
	{
		return TRUE;
	}

	const int32_t clipTop = std::max(0L, pCropRect->right);
	const int32_t clipBottom = std::min(static_cast<LONG>(gnSurfaceHeight), pCropRect->bottom);
	if (clipTop > clipBottom)
	{
		return TRUE;
	}

	const int32_t yEnd = nPosY + D2CMP_CelGetOffsetY(pCell);
	if (yEnd < clipTop || yEnd - nHeight > clipBottom)
	{
		return TRUE;
	}

	pOutClip->left = clipLeft;
	pOutClip->top = clipRight;
	pOutClip->right = (yEnd > clipBottom) ? (yEnd - clipBottom - 1) : 0;
	if (yEnd - nHeight >= clipTop)
	{
		pOutClip->bottom = nHeight - pOutClip->right;
	}
	else
	{
		pOutClip->bottom = yEnd - pOutClip->right - clipTop + 1;
	}
	D2_ASSERT(pOutClip->bottom > 0);
	return FALSE;
}

//1.12a: D2Gdi.0x6F877F80
static void __fastcall D2GDI_CelDrawClipped(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, void* pCropRect, DrawMode eDrawMode)
{
	if (!D2CMP_SpriteValidate(pData, 0, TRUE))
	{
		return;
	}

	RECT clip = {};
	D2CellFileStrc* pCell = reinterpret_cast<D2CellFileStrc*>(pData->pCurrentCell);
	if (!D2GDI_ComputeClipRect(nPosX, nPosY, pCell, static_cast<const RECT*>(pCropRect), &clip))
	{
		int32_t nMode = 0;
		void* pTable = D2GDI_GetDrawPaletteTable(eDrawMode, &nMode);
		D2CMP_CelDrawEx(pData, nPosX, nPosY, clip.left, clip.top, gpSurfaceBits, gnSurfaceHeight, gnSurfaceWidth, 0, reinterpret_cast<int32_t>(pTable), clip.right, clip.bottom, 0);
	}
}

//1.12a: D2Gdi.0x6F878010
static int32_t __fastcall D2GDI_FloorTileDraw(D2TileLibraryEntryStrc* pTile, D2GfxLightExStrc* pLight, int32_t nPosX, int32_t nPosY, int32_t nWorldXpos, int32_t nWorldYpos, uint8_t nAlpha, int32_t nScreenPanels, void* pTileData)
{
	if (D2CMP_TileValidate(pTile, TRUE, FALSE))
	{
		int32_t x = nPosX - 80;
		if (nScreenPanels == 1)
		{
			x -= gnSurfaceWidth / 4;
		}
		else if (nScreenPanels == 2)
		{
			x += gnSurfaceWidth / 4;
		}
		gpGfxHelpers->pfFloorTileDraw(pTile, x, nPosY, pLight);
	}
	return 1;
}

//1.12a: D2Gdi.0x6F878070
static void __fastcall D2GDI_CelDrawHilight(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint8_t nPaletteIndex)
{
}

//1.12a: D2Gdi.0x6F87B928
static D2GraphicsInterfaceStrc gGdiGraphicsInterface =
{
	D2GDI_Detect,
	D2GDI_Init,
	D2GDI_Close,
	D2GDI_CreateSurface,
	D2GDI_CloseSurface,
	D2GDI_PauseSurface,
	D2GDI_StartDraw,
	D2GDI_EndDraw,
	D2GDI_Blit,
	D2GDI_ChangeRes,
	D2GDI_GetBackBuffer,
	D2GDI_ActivateWindow,
	D2GDI_SetOption,
	D2GDI_ReturnFalse0,
	D2GDI_OpenSmackCutscene,
	D2GDI_CheckCutScene,
	D2GDI_DecodeSmacker,
	D2GDI_PlaySmacker,
	D2GDI_CloseSmacker,
	D2GDI_GetCacheResults,
	D2GDI_GetDimensions,
	D2GDI_SetGlobalScale,
	D2GDI_SetGamma,
	D2GDI_GammaCanBeControlled,
	D2GDI_UpdatePerspective,
	D2GDI_PerspectiveTransform,
	D2GDI_PerspectiveTransformScale,
	D2GDI_PerspectiveClearScale,
	D2GDI_SetPalette,
	D2GDI_SetPaletteTables,
	D2GDI_SetAmbientColor,
	D2GDI_FloorTileDraw,
	D2GDI_CelFlatSpriteDraw,
	D2GDI_CelDraw,
	D2GDI_CelDrawColor,
	D2GDI_CelDrawEx,
	D2GDI_CelDrawShadow,
	D2GDI_CelDrawHilight,
	D2GDI_CelDrawClipped,
	D2GDI_TileDrawLit,
	D2GDI_TileDrawTrans,
	D2GDI_ShadowTileDraw,
	D2GDI_UtilDiamond,
	D2GDI_UtilRect,
	D2GDI_UtilFilledRect,
	D2GDI_UtilPoint,
	D2GDI_DrawBox,
	D2GDI_DrawBoxAlpha,
	D2GDI_DrawLine,
	D2GDI_ClearScreen,
	D2GDI_OutputString,
	D2GDI_DebugDraw,
	D2GDI_Distort,
	D2GDI_ClearCaches,
};

//1.12a: D2Gdi.0x6F876B60 (#10000)
D2GraphicsInterfaceStrc* __fastcall D2GDI_GetGraphicsInterface()
{
	return &gGdiGraphicsInterface;
}
