#include "D2Glide.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <GL/gl.h>

#include <Fog.h>
#include <Storm.h>
#include <D2CMP.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#pragma pack(push, 1)
struct D2GlideSmackStrc
{
	uint32_t dwVersion;			//0x00
	uint32_t nWidth;			//0x04
	uint32_t nHeight;			//0x08
	uint32_t nFrames;			//0x0C
	uint8_t pad0x10[0x58];		//0x10
	uint32_t bNewPalette;		//0x68
	uint8_t aPalette[772];		//0x6C
	uint8_t pad0x370[4];		//0x370
	uint32_t nFrameNum;			//0x374
};

struct D2GlideSmackerContextStrc
{
	D2GlideSmackStrc* pSmack;	//0x00
	char cMode;					//0x04
	uint8_t pad0x05[3];			//0x05
	int32_t nXPos;				//0x08
	int32_t nYPos;				//0x0C
};
#pragma pack(pop)

using SmackOpenFn = D2GlideSmackStrc* (__stdcall*)(const char* szFile, uint32_t dwFlags, int32_t nExtraBuf);
using SmackToBufferFn = void(__stdcall*)(D2GlideSmackStrc* pSmack, uint32_t nLeft, uint32_t nTop, uint32_t nPitch, uint32_t nDestHeight, void* pBuf, uint32_t dwFlags);
using SmackWaitFn = uint32_t(__stdcall*)(D2GlideSmackStrc* pSmack);
using SmackDoFrameFn = uint32_t(__stdcall*)(D2GlideSmackStrc* pSmack);
using SmackNextFrameFn = void(__stdcall*)(D2GlideSmackStrc* pSmack);
using SmackCloseFn = void(__stdcall*)(D2GlideSmackStrc* pSmack);
using WglSwapIntervalExtFn = BOOL(WINAPI*)(int interval);

static constexpr int32_t kDiamondRowOffset[15] = { 14, 12, 10, 8, 6, 4, 2, 0, 2, 4, 6, 8, 10, 12, 14 };
static constexpr int32_t kDiamondRowWidth[15]  = { 4, 8, 12, 16, 20, 24, 28, 32, 28, 24, 20, 16, 12, 8, 4 };
static constexpr int32_t kDiamondRowSrcIdx[15] = { 0, 4, 12, 24, 40, 60, 84, 112, 144, 172, 196, 216, 232, 244, 252 };

static constexpr float kPerspFocal = 1100.0f;
static constexpr float kPerspTiltX = 0.16f;
static constexpr float kPerspTiltY = 0.09f;

struct D2GlideCachedTileStrc
{
	const D2TileLibraryEntryStrc* pTileKey;
	const D2TileLibraryBlockStrc* pBlockKey;
	int32_t nBlocks;
	int32_t nType;
	int32_t nStyle;
	int32_t nSequence;
	int32_t bIsFloorDiamond;
	int32_t nMinX;
	int32_t nMinY;
	int32_t nWidth;
	int32_t nHeight;
	uint8_t* pIndices;
	uint8_t* pMask;
};

static constexpr uint32_t kTileCacheSlotCount = 2048; // Power of two
static D2GlideCachedTileStrc gTileCache[kTileCacheSlotCount] = {};

static HWND ghWnd = nullptr;
static HDC ghGlDC = nullptr;
static HGLRC ghGlRC = nullptr;
static GLuint gGlFrameTex = 0;
static int32_t gnGlTexWidth = 0;
static int32_t gnGlTexHeight = 0;
static BOOL gbWindowedMode = TRUE;

static int32_t gnSurfaceWidth = 0;
static int32_t gnSurfaceHeight = 0;
static int32_t gnCropLeft = 0;
static int32_t gnCropRight = 0;
static int32_t gnWindowState = 0;
static int32_t gnGlobalScaleFactor = 256;
static int32_t gnPanelViewMode = 0; // 0 = Full, 1 = RightPanelOpen(LeftHalf), 2 = LeftPanelOpen(RightHalf)
static int32_t gnGammaValue = 155;
static int32_t gnContrastValue = 100;
static uint8_t gnAmbientRed = 0xFF;
static uint8_t gnAmbientGreen = 0xFF;
static uint8_t gnAmbientBlue = 0xFF;

static BOOL gbInUnitPerspective = FALSE;
static int32_t gnUnitPerspX = 0;
static int32_t gnUnitPerspY = 0;

static uint32_t* gpFrameRGBA = nullptr;    // RGBA8 little-endian: [R, G, B, 255]
static uint8_t* gpCompatSurface8 = nullptr; // 8-bit compatibility surface for GetSurfaceInfo
static uint32_t* gpGdiFallbackBGRA = nullptr;
static HPALETTE ghPalette = nullptr;
static HFONT ghFont = nullptr;
static RGBQUAD gColorTable[256] = {};
static PALETTEENTRY gPaletteEntries[256] = {};
static uint8_t gGammaLut[256] = {};
static uint32_t gGammaColorRGBA[256] = {};

static D2PaletteTableStrc gPaletteTables = {};
static D2PaletteTableStrc* gpPaletteTables = nullptr;
static D2GfxSettingsStrc* gpGfxSettings = nullptr;
static const D2GfxHelperStrc* gpGfxHelpers = nullptr;
static CRITICAL_SECTION* gpCriticalSection = nullptr;

static uint8_t gSmackPaletteMap[256] = {};
static uint8_t* gpSmackBuffer = nullptr;
static BOOL gbSmackPaletteInitialized = FALSE;

static uint8_t* gpScratchCellIdx = nullptr;
static uint8_t* gpScratchCellMask = nullptr;
static size_t guScratchCellCap = 0;

static inline uint32_t PackRGBA(uint8_t r, uint8_t g, uint8_t b)
{
	return 0xFF000000u | (static_cast<uint32_t>(b) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(r);
}

static void RebuildGammaTables()
{
	const int32_t clampedGamma = std::max<int32_t>(55, std::min<int32_t>(300, gnGammaValue > 0 ? gnGammaValue : 155));
	const double exponent = 155.0 / static_cast<double>(clampedGamma);
	gGammaLut[0] = 0;
	for (int32_t i = 1; i < 256; ++i)
	{
		const double norm = static_cast<double>(i) / 255.0;
		const double mapped = std::pow(norm, exponent) * 255.0;
		const int32_t iv = static_cast<int32_t>(mapped + 0.5);
		gGammaLut[i] = static_cast<uint8_t>(std::max<int32_t>(0, std::min<int32_t>(255, iv)));
	}

	for (int32_t i = 0; i < 256; ++i)
	{
		const uint8_t r = gGammaLut[gPaletteEntries[i].peRed];
		const uint8_t g = gGammaLut[gPaletteEntries[i].peGreen];
		const uint8_t b = gGammaLut[gPaletteEntries[i].peBlue];
		gGammaColorRGBA[i] = PackRGBA(r, g, b);
	}
}

static inline bool IsPerspectiveActive()
{
	return gpGfxSettings != nullptr && gpGfxSettings->bPerspectiveCapable != 0 && gpGfxSettings->bPerspectiveEnabled != 0;
}

static inline void GetActiveViewportGeometry(float* pCenterX, float* pCenterY, int32_t* pClipLeft, int32_t* pClipRight, int32_t* pClipBottom)
{
	const int32_t w = (gnSurfaceWidth > 0) ? gnSurfaceWidth : 800;
	const int32_t h = (gnSurfaceHeight > 0) ? gnSurfaceHeight : 600;
	const int32_t vpBottom = (h > 48) ? (h - 47) : h;

	int32_t clipL = 0;
	int32_t clipR = w;
	float cx = static_cast<float>(w) * 0.5f;
	if (gnPanelViewMode == 1)
	{
		clipL = 0;
		clipR = w / 2;
		cx = static_cast<float>(w) * 0.25f;
	}
	else if (gnPanelViewMode == 2)
	{
		clipL = w / 2;
		clipR = w;
		cx = static_cast<float>(w) * 0.75f;
	}
	const float cy = (h > 48) ? (static_cast<float>(h - 48) * 0.5f) : (static_cast<float>(h) * 0.5f);

	if (pCenterX) *pCenterX = cx;
	if (pCenterY) *pCenterY = cy;
	if (pClipLeft) *pClipLeft = clipL;
	if (pClipRight) *pClipRight = clipR;
	if (pClipBottom) *pClipBottom = vpBottom;
}

static inline void ProjectGroundPoint(float isoX, float isoY, float cx, float cy, float* pOutX, float* pOutY, float* pOutScale = nullptr)
{
	const float dy = std::max(-900.0f, std::min(700.0f, isoY - cy));
	const float scale = kPerspFocal / (kPerspFocal - dy);
	const float dyOut = (kPerspFocal * dy) / (kPerspFocal - 0.5f * dy);
	const float dxOut = (isoX - cx) * scale;
	*pOutX = cx + dxOut;
	*pOutY = cy + dyOut;
	if (pOutScale)
	{
		*pOutScale = scale;
	}
}

static inline void ProjectElevatedPoint(float isoX, float groundY, float heightPx, float cx, float cy, float* pOutX, float* pOutY)
{
	float gx = 0.0f;
	float gy = 0.0f;
	float scale = 1.0f;
	ProjectGroundPoint(isoX, groundY, cx, cy, &gx, &gy, &scale);
	if (heightPx == 0.0f)
	{
		*pOutX = gx;
		*pOutY = gy;
		return;
	}
	const float tiltX = heightPx * scale * kPerspTiltX * ((gx - cx) * (1.0f / 400.0f));
	const float tiltY = heightPx * scale * kPerspTiltY * ((gy - cy) * (1.0f / 276.0f));
	*pOutX = gx + tiltX;
	*pOutY = gy - heightPx * scale + tiltY;
}

static inline void InverseGroundScanline(float screenY, float cy, float* pOutIsoY, float* pOutInvScaleX)
{
	const float dyOut = screenY - cy;
	const float denom = std::max(128.0f, kPerspFocal + 0.5f * dyOut);
	const float dy = (kPerspFocal * dyOut) / denom;
	*pOutIsoY = cy + dy;
	*pOutInvScaleX = (kPerspFocal - dy) * (1.0f / kPerspFocal);
}

static inline void InverseElevatedScanline(float screenY, float heightPx, float cy, float* pOutGroundY, float* pOutInvFactorX)
{
	if (heightPx <= 0.0f)
	{
		InverseGroundScanline(screenY, cy, pOutGroundY, pOutInvFactorX);
		return;
	}
	float dy = std::max(-900.0f, std::min(700.0f, (screenY + heightPx) - cy));
	for (int32_t iter = 0; iter < 4; ++iter)
	{
		const float denomA = kPerspFocal - 0.5f * dy;
		const float denomB = kPerspFocal - dy;
		const float dyG = (kPerspFocal * dy) / denomA;
		const float scale = kPerspFocal / denomB;
		const float tiltFactorY = 1.0f - kPerspTiltY * (dyG * (1.0f / 276.0f));
		const float fVal = (cy + dyG) - heightPx * scale * tiltFactorY - screenY;

		const float dDyG = (kPerspFocal * kPerspFocal) / (denomA * denomA);
		const float dScale = kPerspFocal / (denomB * denomB);
		const float dF = dDyG - heightPx * (dScale * tiltFactorY - scale * kPerspTiltY * (dDyG * (1.0f / 276.0f)));
		if (std::fabs(dF) > 1e-4f)
		{
			dy = std::max(-900.0f, std::min(700.0f, dy - fVal / dF));
		}
	}
	const float scale = kPerspFocal / (kPerspFocal - dy);
	const float invScale = (kPerspFocal - dy) * (1.0f / kPerspFocal);
	const float tiltHoriz = 1.0f + heightPx * scale * (kPerspTiltX * (1.0f / 400.0f));
	*pOutGroundY = cy + dy;
	*pOutInvFactorX = invScale / std::max(0.25f, tiltHoriz);
}

static inline uint32_t ModulateTexelRGB(uint8_t palIdx, uint32_t mulR, uint32_t mulG, uint32_t mulB)
{
	if (palIdx == 0)
	{
		return 0xFF000000u;
	}
	const uint32_t baseR = gPaletteEntries[palIdx].peRed;
	const uint32_t baseG = gPaletteEntries[palIdx].peGreen;
	const uint32_t baseB = gPaletteEntries[palIdx].peBlue;
	const uint8_t outR = gGammaLut[std::min<uint32_t>(255u, (baseR * mulR) >> 8)];
	const uint8_t outG = gGammaLut[std::min<uint32_t>(255u, (baseG * mulG) >> 8)];
	const uint8_t outB = gGammaLut[std::min<uint32_t>(255u, (baseB * mulB) >> 8)];
	return PackRGBA(outR, outG, outB);
}

static inline uint32_t BlendPixelRGBA(uint32_t dst, uint32_t src, DrawMode eDrawMode)
{
	const uint32_t sr = src & 0xFFu;
	const uint32_t sg = (src >> 8) & 0xFFu;
	const uint32_t sb = (src >> 16) & 0xFFu;

	switch (eDrawMode)
	{
	case DRAWMODE_NORMAL:
		return src;
	case DRAWMODE_TRANS25:
	{
		const uint32_t dr = dst & 0xFFu;
		const uint32_t dg = (dst >> 8) & 0xFFu;
		const uint32_t db = (dst >> 16) & 0xFFu;
		return PackRGBA(
			static_cast<uint8_t>((sr + dr * 3u) >> 2),
			static_cast<uint8_t>((sg + dg * 3u) >> 2),
			static_cast<uint8_t>((sb + db * 3u) >> 2));
	}
	case DRAWMODE_TRANS50:
	{
		const uint32_t dr = dst & 0xFFu;
		const uint32_t dg = (dst >> 8) & 0xFFu;
		const uint32_t db = (dst >> 16) & 0xFFu;
		return PackRGBA(
			static_cast<uint8_t>((sr + dr) >> 1),
			static_cast<uint8_t>((sg + dg) >> 1),
			static_cast<uint8_t>((sb + db) >> 1));
	}
	case DRAWMODE_TRANS75:
	{
		const uint32_t dr = dst & 0xFFu;
		const uint32_t dg = (dst >> 8) & 0xFFu;
		const uint32_t db = (dst >> 16) & 0xFFu;
		return PackRGBA(
			static_cast<uint8_t>((sr * 3u + dr) >> 2),
			static_cast<uint8_t>((sg * 3u + dg) >> 2),
			static_cast<uint8_t>((sb * 3u + db) >> 2));
	}
	case DRAWMODE_MODULATE:
	{
		const uint32_t dr = dst & 0xFFu;
		const uint32_t dg = (dst >> 8) & 0xFFu;
		const uint32_t db = (dst >> 16) & 0xFFu;
		return PackRGBA(
			static_cast<uint8_t>(std::min<uint32_t>(255u, dr + sr)),
			static_cast<uint8_t>(std::min<uint32_t>(255u, dg + sg)),
			static_cast<uint8_t>(std::min<uint32_t>(255u, db + sb)));
	}
	case DRAWMODE_BURN:
	{
		const uint32_t dr = dst & 0xFFu;
		const uint32_t dg = (dst >> 8) & 0xFFu;
		const uint32_t db = (dst >> 16) & 0xFFu;
		return PackRGBA(
			static_cast<uint8_t>(std::min<uint32_t>(255u, dr + (sr >> 1))),
			static_cast<uint8_t>(std::min<uint32_t>(255u, dg + (sg >> 1))),
			static_cast<uint8_t>(std::min<uint32_t>(255u, db + (sb >> 1))));
	}
	case DRAWMODE_TRANSHIGHLIGHT:
		return PackRGBA(
			static_cast<uint8_t>(std::min<uint32_t>(255u, sr + 44u)),
			static_cast<uint8_t>(std::min<uint32_t>(255u, sg + 44u)),
			static_cast<uint8_t>(std::min<uint32_t>(255u, sb + 44u)));
	default:
		return src;
	}
}

static inline uint32_t BlendAlphaRGBA(uint32_t dst, uint32_t src, uint8_t nAlpha)
{
	if (nAlpha >= 0xF8u)
	{
		return src;
	}
	if (nAlpha == 0)
	{
		return dst;
	}
	const uint32_t a = nAlpha;
	const uint32_t invA = 256u - a;
	const uint32_t sr = src & 0xFFu;
	const uint32_t sg = (src >> 8) & 0xFFu;
	const uint32_t sb = (src >> 16) & 0xFFu;
	const uint32_t dr = dst & 0xFFu;
	const uint32_t dg = (dst >> 8) & 0xFFu;
	const uint32_t db = (dst >> 16) & 0xFFu;
	return PackRGBA(
		static_cast<uint8_t>((sr * a + dr * invA) >> 8),
		static_cast<uint8_t>((sg * a + dg * invA) >> 8),
		static_cast<uint8_t>((sb * a + db * invA) >> 8));
}

static void ClearDecodedTileCache()
{
	for (uint32_t i = 0; i < kTileCacheSlotCount; ++i)
	{
		if (gTileCache[i].pIndices)
		{
			std::free(gTileCache[i].pIndices);
			gTileCache[i].pIndices = nullptr;
		}
		if (gTileCache[i].pMask)
		{
			std::free(gTileCache[i].pMask);
			gTileCache[i].pMask = nullptr;
		}
		gTileCache[i] = {};
	}
}

static void FreeTileCacheSlot(D2GlideCachedTileStrc* pSlot)
{
	if (pSlot->pIndices)
	{
		std::free(pSlot->pIndices);
		pSlot->pIndices = nullptr;
	}
	if (pSlot->pMask)
	{
		std::free(pSlot->pMask);
		pSlot->pMask = nullptr;
	}
	*pSlot = {};
}

static const D2GlideCachedTileStrc* GetOrDecodeTile(const D2TileLibraryEntryStrc* pTile, bool bFloorDiamond)
{
	if (!pTile || pTile->nBlocks <= 0 || !pTile->pBlock)
	{
		return nullptr;
	}

	const uintptr_t rawPtr = reinterpret_cast<uintptr_t>(pTile);
	uint32_t h = static_cast<uint32_t>((rawPtr >> 4) ^ (rawPtr >> 13) ^ (bFloorDiamond ? 0x9E37u : 0x517Cu));
	D2GlideCachedTileStrc* pSlot = &gTileCache[h & (kTileCacheSlotCount - 1)];

	const int32_t nFloorFlag = bFloorDiamond ? 1 : 0;
	if (pSlot->pTileKey == pTile &&
		pSlot->pBlockKey == pTile->pBlock &&
		pSlot->nBlocks == pTile->nBlocks &&
		pSlot->nType == pTile->nType &&
		pSlot->nStyle == pTile->nStyle &&
		pSlot->nSequence == pTile->nSequence &&
		pSlot->bIsFloorDiamond == nFloorFlag &&
		pSlot->pIndices != nullptr &&
		pSlot->pMask != nullptr)
	{
		return pSlot;
	}

	FreeTileCacheSlot(pSlot);

	if (bFloorDiamond)
	{
		constexpr int32_t kW = 160;
		constexpr int32_t kH = 80;
		uint8_t* pIdx = static_cast<uint8_t*>(std::calloc(kW * kH, 1));
		uint8_t* pMask = static_cast<uint8_t*>(std::calloc(kW * kH, 1));
		if (!pIdx || !pMask)
		{
			std::free(pIdx);
			std::free(pMask);
			return nullptr;
		}

		for (int32_t b = 0; b < pTile->nBlocks; ++b)
		{
			const D2TileLibraryBlockStrc& blk = pTile->pBlock[b];
			if (!(blk.nFormat & 1) || !blk.dwOffset_pData)
			{
				continue;
			}
			const int32_t bx = blk.nPosX;
			const int32_t by = blk.nPosY;
			const uint8_t* pSrc = blk.dwOffset_pData;

			if (blk.nFormat & 4)
			{
				int32_t row = 0;
				int32_t curX = 0;
				while (row < 15)
				{
					const uint8_t nSkip = pSrc[0];
					const uint8_t nCount = pSrc[1];
					pSrc += 2;
					if (nSkip || nCount)
					{
						curX += nSkip;
						const int32_t dy = by + row;
						if (dy >= 0 && dy < kH)
						{
							for (uint8_t i = 0; i < nCount; ++i)
							{
								const int32_t dx = bx + curX + i;
								if (dx >= 0 && dx < kW)
								{
									pIdx[dy * kW + dx] = pSrc[i];
									pMask[dy * kW + dx] = 1;
								}
							}
						}
						curX += nCount;
						pSrc += nCount;
					}
					else
					{
						++row;
						curX = 0;
					}
				}
			}
			else
			{
				for (int32_t row = 0; row < 15; ++row)
				{
					const int32_t dy = by + row;
					if (dy < 0 || dy >= kH)
					{
						continue;
					}
					const int32_t xOff = kDiamondRowOffset[row];
					const int32_t width = kDiamondRowWidth[row];
					const uint8_t* pRowSrc = pSrc + kDiamondRowSrcIdx[row];
					for (int32_t i = 0; i < width; ++i)
					{
						const int32_t dx = bx + xOff + i;
						if (dx >= 0 && dx < kW)
						{
							pIdx[dy * kW + dx] = pRowSrc[i];
							pMask[dy * kW + dx] = 1;
						}
					}
				}
			}
		}

		pSlot->pTileKey = pTile;
		pSlot->pBlockKey = pTile->pBlock;
		pSlot->nBlocks = pTile->nBlocks;
		pSlot->nType = pTile->nType;
		pSlot->nStyle = pTile->nStyle;
		pSlot->nSequence = pTile->nSequence;
		pSlot->bIsFloorDiamond = 1;
		pSlot->nMinX = 0;
		pSlot->nMinY = 0;
		pSlot->nWidth = kW;
		pSlot->nHeight = kH;
		pSlot->pIndices = pIdx;
		pSlot->pMask = pMask;
		return pSlot;
	}

	int32_t minX = 0x7FFFFFFF;
	int32_t minY = 0x7FFFFFFF;
	int32_t maxX = -0x7FFFFFFF;
	int32_t maxY = -0x7FFFFFFF;
	for (int32_t b = 0; b < pTile->nBlocks; ++b)
	{
		const D2TileLibraryBlockStrc& blk = pTile->pBlock[b];
		if (!blk.dwOffset_pData)
		{
			continue;
		}
		minX = std::min<int32_t>(minX, blk.nPosX);
		minY = std::min<int32_t>(minY, blk.nPosY);
		maxX = std::max<int32_t>(maxX, blk.nPosX + 32);
		maxY = std::max<int32_t>(maxY, blk.nPosY + 32);
	}
	if (minX >= maxX || minY >= maxY)
	{
		return nullptr;
	}
	const int32_t w = std::min<int32_t>(512, maxX - minX);
	const int32_t hgt = std::min<int32_t>(512, maxY - minY);

	uint8_t* pIdx = static_cast<uint8_t*>(std::calloc(static_cast<size_t>(w) * static_cast<size_t>(hgt), 1));
	uint8_t* pMask = static_cast<uint8_t*>(std::calloc(static_cast<size_t>(w) * static_cast<size_t>(hgt), 1));
	if (!pIdx || !pMask)
	{
		std::free(pIdx);
		std::free(pMask);
		return nullptr;
	}

	for (int32_t b = 0; b < pTile->nBlocks; ++b)
	{
		const D2TileLibraryBlockStrc& blk = pTile->pBlock[b];
		if (!blk.dwOffset_pData)
		{
			continue;
		}
		const int32_t baseX = blk.nPosX - minX;
		const int32_t baseY = blk.nPosY - minY;
		const uint8_t* pSrc = blk.dwOffset_pData;

		if (blk.nFormat & 1)
		{
			if (blk.nFormat & 4)
			{
				int32_t row = 0;
				int32_t curX = 0;
				while (row < 15)
				{
					const uint8_t nSkip = pSrc[0];
					const uint8_t nCount = pSrc[1];
					pSrc += 2;
					if (nSkip || nCount)
					{
						curX += nSkip;
						const int32_t dy = baseY + row;
						if (dy >= 0 && dy < hgt)
						{
							for (uint8_t i = 0; i < nCount; ++i)
							{
								const int32_t dx = baseX + curX + i;
								if (dx >= 0 && dx < w)
								{
									pIdx[dy * w + dx] = pSrc[i];
									pMask[dy * w + dx] = 1;
								}
							}
						}
						curX += nCount;
						pSrc += nCount;
					}
					else
					{
						++row;
						curX = 0;
					}
				}
			}
			else
			{
				for (int32_t row = 0; row < 15; ++row)
				{
					const int32_t dy = baseY + row;
					if (dy < 0 || dy >= hgt)
					{
						continue;
					}
					const int32_t xOff = kDiamondRowOffset[row];
					const int32_t width = kDiamondRowWidth[row];
					const uint8_t* pRowSrc = pSrc + kDiamondRowSrcIdx[row];
					for (int32_t i = 0; i < width; ++i)
					{
						const int32_t dx = baseX + xOff + i;
						if (dx >= 0 && dx < w)
						{
							pIdx[dy * w + dx] = pRowSrc[i];
							pMask[dy * w + dx] = 1;
						}
					}
				}
			}
		}
		else
		{
			int32_t row = 0;
			int32_t curX = 0;
			while (row < 32)
			{
				const uint8_t nSkip = pSrc[0];
				const uint8_t nCount = pSrc[1];
				pSrc += 2;
				if (nSkip || nCount)
				{
					curX += nSkip;
					const int32_t dy = baseY + row;
					if (dy >= 0 && dy < hgt)
					{
						for (uint8_t i = 0; i < nCount; ++i)
						{
							const int32_t dx = baseX + curX + i;
							if (dx >= 0 && dx < w)
							{
								pIdx[dy * w + dx] = pSrc[i];
								pMask[dy * w + dx] = 1;
							}
						}
					}
					curX += nCount;
					pSrc += nCount;
				}
				else
				{
					++row;
					curX = 0;
				}
			}
		}
	}

	pSlot->pTileKey = pTile;
	pSlot->pBlockKey = pTile->pBlock;
	pSlot->nBlocks = pTile->nBlocks;
	pSlot->nType = pTile->nType;
	pSlot->nStyle = pTile->nStyle;
	pSlot->nSequence = pTile->nSequence;
	pSlot->bIsFloorDiamond = 0;
	pSlot->nMinX = minX;
	pSlot->nMinY = minY;
	pSlot->nWidth = w;
	pSlot->nHeight = hgt;
	pSlot->pIndices = pIdx;
	pSlot->pMask = pMask;
	return pSlot;
}

static inline float ComputeWallGroundY(int32_t nTileType, int32_t nTileBaseX, int32_t nTileBaseY, float isoX)
{
	const float relX = std::max(0.0f, std::min(160.0f, isoX - static_cast<float>(nTileBaseX)));
	const float baseY = static_cast<float>(nTileBaseY);
	switch (nTileType)
	{
	case TILETYPE_WALL_LEFT:
	case TILETYPE_WALL_LEFT_DOOR:
	case TILETYPE_WALL_LEFT_EXIT:
		return (relX <= 80.0f)
			? (baseY - 40.0f - 0.5f * relX)
			: (baseY - 80.0f + 0.5f * (relX - 80.0f));
	case TILETYPE_WALL_RIGHT:
	case TILETYPE_WALL_RIGHT_DOOR:
	case TILETYPE_WALL_RIGHT_EXIT:
		return (relX >= 80.0f)
			? (baseY - 80.0f + 0.5f * (relX - 80.0f))
			: (baseY - 40.0f - 0.5f * relX);
	case TILETYPE_WALL_TOP_CORNER_RIGHT:
	case TILETYPE_WALL_TOP_CORNER_LEFT:
	case TILETYPE_WALL_TOP_RIGHT:
		return baseY - 80.0f + 0.5f * std::fabs(relX - 80.0f);
	case TILETYPE_WALL_BOTTOM_LEFT:
		return baseY - 0.5f * (80.0f - std::min(80.0f, relX));
	case TILETYPE_WALL_BOTTOM_RIGHT:
		return baseY - 0.5f * std::max(0.0f, relX - 80.0f);
	default:
		return baseY - 40.0f;
	}
}

struct QuadVert
{
	float x;
	float y;
	float u;
	float v;
};

static void RasterizeTexturedQuad(
	const uint8_t* pIndices,
	const uint8_t* pMask,
	int32_t texStride,
	int32_t texW,
	int32_t texH,
	const QuadVert verts[4],
	uint32_t mulR,
	uint32_t mulG,
	uint32_t mulB,
	uint8_t nAlpha,
	int32_t clipLeft,
	int32_t clipTop,
	int32_t clipRight,
	int32_t clipBottom)
{
	float minYf = verts[0].y;
	float maxYf = verts[0].y;
	for (int32_t i = 1; i < 4; ++i)
	{
		minYf = std::min(minYf, verts[i].y);
		maxYf = std::max(maxYf, verts[i].y);
	}

	const int32_t yStart = std::max(clipTop, static_cast<int32_t>(std::ceil(minYf - 0.5f)));
	const int32_t yEnd = std::min(clipBottom, static_cast<int32_t>(std::ceil(maxYf - 0.5f)));
	if (yStart >= yEnd)
	{
		return;
	}

	for (int32_t y = yStart; y < yEnd; ++y)
	{
		const float sampleY = static_cast<float>(y) + 0.5f;
		float leftX = 1e9f, leftU = 0.0f, leftV = 0.0f;
		float rightX = -1e9f, rightU = 0.0f, rightV = 0.0f;
		int32_t hits = 0;

		for (int32_t e = 0; e < 4; ++e)
		{
			const QuadVert& a = verts[e];
			const QuadVert& b = verts[(e + 1) & 3];
			if ((a.y <= sampleY && b.y > sampleY) || (b.y <= sampleY && a.y > sampleY))
			{
				const float t = (sampleY - a.y) / (b.y - a.y);
				const float ix = a.x + t * (b.x - a.x);
				const float iu = a.u + t * (b.u - a.u);
				const float iv = a.v + t * (b.v - a.v);
				if (ix < leftX)
				{
					leftX = ix;
					leftU = iu;
					leftV = iv;
				}
				if (ix > rightX)
				{
					rightX = ix;
					rightU = iu;
					rightV = iv;
				}
				++hits;
			}
		}

		if (hits < 2 || rightX <= leftX)
		{
			continue;
		}

		int32_t xStart = static_cast<int32_t>(std::ceil(leftX - 0.5f));
		int32_t xEnd = static_cast<int32_t>(std::ceil(rightX - 0.5f));
		if (xEnd <= clipLeft || xStart >= clipRight)
		{
			continue;
		}

		const float invSpan = 1.0f / std::max(0.001f, rightX - leftX);
		const float duDx = (rightU - leftU) * invSpan;
		const float dvDx = (rightV - leftV) * invSpan;

		if (xStart < clipLeft)
		{
			xStart = clipLeft;
		}
		if (xEnd > clipRight)
		{
			xEnd = clipRight;
		}
		if (xStart >= xEnd)
		{
			continue;
		}

		const float initStep = (static_cast<float>(xStart) + 0.5f) - leftX;
		int32_t uFp = static_cast<int32_t>((leftU + initStep * duDx) * 65536.0f);
		int32_t vFp = static_cast<int32_t>((leftV + initStep * dvDx) * 65536.0f);
		const int32_t duFp = static_cast<int32_t>(duDx * 65536.0f);
		const int32_t dvFp = static_cast<int32_t>(dvDx * 65536.0f);

		uint32_t* pDstRow = gpFrameRGBA + y * gnSurfaceWidth;
		for (int32_t x = xStart; x < xEnd; ++x)
		{
			const int32_t tu = std::max<int32_t>(0, std::min<int32_t>(texW - 1, uFp >> 16));
			const int32_t tv = std::max<int32_t>(0, std::min<int32_t>(texH - 1, vFp >> 16));
			const int32_t off = tv * texStride + tu;
			if (pMask[off])
			{
				const uint32_t lit = ModulateTexelRGB(pIndices[off], mulR, mulG, mulB);
				pDstRow[x] = (nAlpha >= 0xF8u) ? lit : BlendAlphaRGBA(pDstRow[x], lit, nAlpha);
			}
			uFp += duFp;
			vFp += dvFp;
		}
	}
}

static bool EnsureScratchCellCapacity(size_t needed)
{
	if (needed <= guScratchCellCap && gpScratchCellIdx && gpScratchCellMask)
	{
		return true;
	}
	const size_t newCap = std::max<size_t>(needed, 512u * 512u);
	uint8_t* pNewIdx = static_cast<uint8_t*>(std::realloc(gpScratchCellIdx, newCap));
	uint8_t* pNewMask = static_cast<uint8_t*>(std::realloc(gpScratchCellMask, newCap));
	if (!pNewIdx || !pNewMask)
	{
		return false;
	}
	gpScratchCellIdx = pNewIdx;
	gpScratchCellMask = pNewMask;
	guScratchCellCap = newCap;
	return true;
}

static inline const uint8_t* GetCellPixels(const D2GfxCellStrc* pCell)
{
	return pCell ? (reinterpret_cast<const uint8_t*>(pCell) + 0x20) : nullptr;
}

static bool DecodeCellToScratch(const D2GfxCellStrc* pCell)
{
	if (!pCell || pCell->dwLength == 0 || pCell->dwWidth == 0 || pCell->dwHeight == 0)
	{
		return false;
	}
	const int32_t w = static_cast<int32_t>(pCell->dwWidth);
	const int32_t h = static_cast<int32_t>(pCell->dwHeight);
	if (w <= 0 || h <= 0 || w > 2048 || h > 2048)
	{
		return false;
	}
	const size_t total = static_cast<size_t>(w) * static_cast<size_t>(h);
	if (!EnsureScratchCellCapacity(total))
	{
		return false;
	}
	std::memset(gpScratchCellMask, 0, total);

	const bool bTopDown = (pCell->bFlip & 1) != 0;
	int32_t curY = bTopDown ? 0 : (h - 1);
	const int32_t stepY = bTopDown ? 1 : -1;
	int32_t curX = 0;

	const uint8_t* pSrc = GetCellPixels(pCell);
	const uint8_t* pEnd = pSrc + pCell->dwLength;
	int32_t lines = 0;

	while (pSrc < pEnd && lines < h)
	{
		const uint8_t ctrl = *pSrc++;
		if (ctrl == 0x80)
		{
			curX = 0;
			curY += stepY;
			++lines;
		}
		else if (ctrl > 0x80)
		{
			curX += (ctrl & 0x7F);
		}
		else
		{
			const int32_t count = ctrl;
			if (curY >= 0 && curY < h)
			{
				uint8_t* pRowIdx = gpScratchCellIdx + curY * w;
				uint8_t* pRowMask = gpScratchCellMask + curY * w;
				for (int32_t i = 0; i < count && pSrc < pEnd; ++i)
				{
					const uint8_t px = *pSrc++;
					if (curX >= 0 && curX < w)
					{
						pRowIdx[curX] = px;
						pRowMask[curX] = 1;
					}
					++curX;
				}
			}
			else
			{
				pSrc += count;
				curX += count;
			}
		}
	}
	return true;
}

static void DrawCell2D(
	const D2GfxCellStrc* pCell,
	int32_t nPosX,
	int32_t nPosY,
	int32_t nClipLeft,
	int32_t nClipRight,
	int32_t nClipTop,
	int32_t nClipBottom,
	int32_t nSkipLines,
	int32_t nDrawLines,
	const uint8_t* pPaletteRemap,
	uint32_t dwGamma,
	DrawMode eDrawMode)
{
	if (!pCell || pCell->dwLength == 0 || !gpFrameRGBA)
	{
		return;
	}

	const int32_t w = static_cast<int32_t>(pCell->dwWidth);
	const int32_t h = static_cast<int32_t>(pCell->dwHeight);
	if (w <= 0 || h <= 0)
	{
		return;
	}

	const int32_t startX = nPosX + pCell->nXOffset;
	const int32_t bottomY = nPosY + pCell->nYOffset;
	const int32_t skip = std::max<int32_t>(0, nSkipLines);
	const int32_t toDraw = (nDrawLines > 0 && nDrawLines <= h - skip) ? nDrawLines : (h - skip);
	if (toDraw <= 0 || startX >= nClipRight || startX + w <= nClipLeft)
	{
		return;
	}

	const uint8_t* pDarken = nullptr;
	if (gpPaletteTables && (dwGamma & 0xFFu) != 0xFFu)
	{
		pDarken = static_cast<const uint8_t*>(gpPaletteTables->shadowLightGammaPalettes[(dwGamma & 0xFFu) >> 3]);
	}

	const bool bTopDown = (pCell->bFlip & 1) != 0;
	const int32_t stepY = bTopDown ? 1 : -1;
	int32_t curY = bTopDown ? (bottomY - h + 1 + skip) : (bottomY - skip);

	const uint8_t* pSrc = GetCellPixels(pCell);
	const uint8_t* pEnd = pSrc + pCell->dwLength;
	for (int32_t s = 0; s < skip && pSrc < pEnd;)
	{
		const uint8_t ctrl = *pSrc++;
		if (ctrl == 0x80)
		{
			++s;
		}
		else if (ctrl < 0x80)
		{
			pSrc += ctrl;
		}
	}

	int32_t curX = startX;
	int32_t drawn = 0;
	while (pSrc < pEnd && drawn < toDraw)
	{
		const uint8_t ctrl = *pSrc++;
		if (ctrl == 0x80)
		{
			curX = startX;
			curY += stepY;
			++drawn;
		}
		else if (ctrl > 0x80)
		{
			curX += (ctrl & 0x7F);
		}
		else
		{
			const int32_t count = ctrl;
			if (curY >= nClipTop && curY < nClipBottom)
			{
				uint32_t* pDstRow = gpFrameRGBA + curY * gnSurfaceWidth;
				for (int32_t i = 0; i < count && pSrc < pEnd; ++i)
				{
					uint8_t px = *pSrc++;
					if (curX >= nClipLeft && curX < nClipRight)
					{
						if (pPaletteRemap)
						{
							px = pPaletteRemap[px];
						}
						if (pDarken)
						{
							px = pDarken[px];
						}
						const uint32_t srcRGBA = gGammaColorRGBA[px];
						pDstRow[curX] = BlendPixelRGBA(pDstRow[curX], srcRGBA, eDrawMode);
					}
					++curX;
				}
			}
			else
			{
				pSrc += count;
				curX += count;
			}
		}
	}
}

static void DrawCellPerspectiveUnit(
	const D2GfxCellStrc* pCell,
	int32_t nAnchorX,
	int32_t nAnchorY,
	const uint8_t* pPaletteRemap,
	uint32_t dwGamma,
	DrawMode eDrawMode)
{
	if (!DecodeCellToScratch(pCell) || !gpFrameRGBA)
	{
		return;
	}

	float cx = 400.0f, cy = 276.0f;
	int32_t clipL = 0, clipR = gnSurfaceWidth, clipB = gnSurfaceHeight;
	GetActiveViewportGeometry(&cx, &cy, &clipL, &clipR, &clipB);

	float anchorScreenX = 0.0f, anchorScreenY = 0.0f, scale = 1.0f;
	ProjectGroundPoint(static_cast<float>(nAnchorX), static_cast<float>(nAnchorY), cx, cy, &anchorScreenX, &anchorScreenY, &scale);

	const int32_t w = static_cast<int32_t>(pCell->dwWidth);
	const int32_t h = static_cast<int32_t>(pCell->dwHeight);
	const float u0 = static_cast<float>(pCell->nXOffset);
	const float hTop = static_cast<float>(-(pCell->nYOffset - h + 1));
	const float hBot = static_cast<float>(-pCell->nYOffset);

	const float tiltFactorX = kPerspTiltX * ((anchorScreenX - cx) * (1.0f / 400.0f));
	const float tiltFactorY = 1.0f - kPerspTiltY * ((anchorScreenY - cy) * (1.0f / 276.0f));
	const float effScaleY = std::max(0.25f, scale * tiltFactorY);

	const float yTopScreen = anchorScreenY - hTop * effScaleY;
	const float yBotScreen = anchorScreenY - hBot * effScaleY + 1.0f;

	const int32_t yStart = std::max<int32_t>(0, static_cast<int32_t>(std::floor(std::min(yTopScreen, yBotScreen))));
	const int32_t yEnd = std::min<int32_t>(clipB, static_cast<int32_t>(std::ceil(std::max(yTopScreen, yBotScreen))));
	if (yStart >= yEnd)
	{
		return;
	}

	const uint8_t* pDarken = nullptr;
	if (gpPaletteTables && (dwGamma & 0xFFu) != 0xFFu)
	{
		pDarken = static_cast<const uint8_t*>(gpPaletteTables->shadowLightGammaPalettes[(dwGamma & 0xFFu) >> 3]);
	}

	const float invScaleX = 1.0f / std::max(0.25f, scale);
	const float invEffScaleY = 1.0f / effScaleY;

	for (int32_t sy = yStart; sy < yEnd; ++sy)
	{
		const float curH = (anchorScreenY - (static_cast<float>(sy) + 0.5f)) * invEffScaleY;
		const int32_t srcY = static_cast<int32_t>(std::floor(hTop - curH + 0.5f));
		if (srcY < 0 || srcY >= h)
		{
			continue;
		}

		const float rowShiftX = curH * scale * tiltFactorX;
		const float xLeftScreen = anchorScreenX + rowShiftX + u0 * scale;
		const float xRightScreen = xLeftScreen + static_cast<float>(w) * scale;

		const int32_t xStart = std::max<int32_t>(clipL, static_cast<int32_t>(std::ceil(xLeftScreen - 0.5f)));
		const int32_t xEnd = std::min<int32_t>(clipR, static_cast<int32_t>(std::ceil(xRightScreen - 0.5f)));
		if (xStart >= xEnd)
		{
			continue;
		}

		const uint8_t* pRowIdx = gpScratchCellIdx + srcY * w;
		const uint8_t* pRowMask = gpScratchCellMask + srcY * w;
		uint32_t* pDstRow = gpFrameRGBA + sy * gnSurfaceWidth;

		int32_t uFp = static_cast<int32_t>(((static_cast<float>(xStart) + 0.5f - xLeftScreen) * invScaleX) * 65536.0f);
		const int32_t duFp = static_cast<int32_t>(invScaleX * 65536.0f);

		for (int32_t sx = xStart; sx < xEnd; ++sx)
		{
			const int32_t srcX = uFp >> 16;
			if (static_cast<uint32_t>(srcX) < static_cast<uint32_t>(w) && pRowMask[srcX])
			{
				uint8_t px = pRowIdx[srcX];
				if (pPaletteRemap)
				{
					px = pPaletteRemap[px];
				}
				if (pDarken)
				{
					px = pDarken[px];
				}
				pDstRow[sx] = BlendPixelRGBA(pDstRow[sx], gGammaColorRGBA[px], eDrawMode);
			}
			uFp += duFp;
		}
	}
}

static HMODULE D2GLIDE_GetSmackModule()
{
	HMODULE hMod = GetModuleHandleA("smackw32.dll");
	if (!hMod)
	{
		hMod = LoadLibraryA("smackw32.dll");
	}
	return hMod;
}

static void D2GLIDE_DestroyGlContext()
{
	if (ghGlRC)
	{
		if (ghGlDC)
		{
			wglMakeCurrent(ghGlDC, ghGlRC);
		}
		if (gGlFrameTex != 0)
		{
			glDeleteTextures(1, &gGlFrameTex);
			gGlFrameTex = 0;
		}
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(ghGlRC);
		ghGlRC = nullptr;
	}
	if (ghGlDC && ghWnd)
	{
		ReleaseDC(ghWnd, ghGlDC);
		ghGlDC = nullptr;
	}
	gnGlTexWidth = 0;
	gnGlTexHeight = 0;
}

static void D2GLIDE_EnsureGlContextAndTexture()
{
	if (!ghWnd || gnSurfaceWidth <= 0 || gnSurfaceHeight <= 0)
	{
		return;
	}

	if (!ghGlDC)
	{
		ghGlDC = GetDC(ghWnd);
	}
	if (!ghGlDC)
	{
		return;
	}

	if (!ghGlRC)
	{
		if (GetPixelFormat(ghGlDC) == 0)
		{
			PIXELFORMATDESCRIPTOR pfd = {};
			pfd.nSize = sizeof(pfd);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cAlphaBits = 8;
			pfd.iLayerType = PFD_MAIN_PLANE;
			const int pf = ChoosePixelFormat(ghGlDC, &pfd);
			if (pf != 0)
			{
				SetPixelFormat(ghGlDC, pf, &pfd);
			}
		}
		ghGlRC = wglCreateContext(ghGlDC);
	}

	if (!ghGlRC || !wglMakeCurrent(ghGlDC, ghGlRC))
	{
		return;
	}

	if (auto pfSwapInterval = reinterpret_cast<WglSwapIntervalExtFn>(wglGetProcAddress("wglSwapIntervalEXT")))
	{
		pfSwapInterval((gpGfxSettings && gpGfxSettings->bVSync) ? 1 : 0);
	}

	if (gGlFrameTex == 0 || gnGlTexWidth != gnSurfaceWidth || gnGlTexHeight != gnSurfaceHeight)
	{
		if (gGlFrameTex == 0)
		{
			glGenTextures(1, &gGlFrameTex);
		}
		glBindTexture(GL_TEXTURE_2D, gGlFrameTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gnSurfaceWidth, gnSurfaceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		gnGlTexWidth = gnSurfaceWidth;
		gnGlTexHeight = gnSurfaceHeight;
	}
}

static void D2GLIDE_AllocateBuffers(D2GameResolutionMode nResolutionMode)
{
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
	gnCropLeft = 0;
	gnCropRight = nWidth;

	const size_t nPixels = static_cast<size_t>(nWidth) * static_cast<size_t>(nHeight);
	gpFrameRGBA = static_cast<uint32_t*>(std::realloc(gpFrameRGBA, nPixels * sizeof(uint32_t)));
	gpCompatSurface8 = static_cast<uint8_t*>(std::realloc(gpCompatSurface8, nPixels));
	gpGdiFallbackBGRA = static_cast<uint32_t*>(std::realloc(gpGdiFallbackBGRA, nPixels * sizeof(uint32_t)));

	if (gpFrameRGBA)
	{
		for (size_t i = 0; i < nPixels; ++i)
		{
			gpFrameRGBA[i] = 0xFF000000u;
		}
	}
	if (gpCompatSurface8)
	{
		std::memset(gpCompatSurface8, 0, nPixels);
	}
	if (gpGfxHelpers && gpGfxHelpers->pfFillYBufferTable)
	{
		gpGfxHelpers->pfFillYBufferTable(gpCompatSurface8, gnSurfaceWidth, gnSurfaceHeight, 0);
	}
}

//1.12a: D2Glide (#10003)
const D2GfxHelperStrc* __fastcall D2GLIDE_GetHelpers()
{
	return gpGfxHelpers;
}

//1.12a: D2Glide (#10002)
void __fastcall D2GLIDE_GetSurfaceInfo(uint8_t** ppBits, int32_t* pHeight, int32_t* pWidth)
{
	if (ppBits) *ppBits = gpCompatSurface8;
	if (pHeight) *pHeight = gnSurfaceHeight;
	if (pWidth) *pWidth = gnSurfaceWidth;
}

//1.12a: D2Glide (#10001)
void __fastcall D2GLIDE_GetPalette(HPALETTE* pPalette, RGBQUAD** ppColorTable)
{
	if (pPalette) *pPalette = ghPalette;
	if (ppColorTable) *ppColorTable = gColorTable;
}

static BOOL __fastcall D2GLIDE_Detect(HINSTANCE hInstance)
{
	(void)hInstance;
	return TRUE;
}

static BOOL __fastcall D2GLIDE_Init(D2GfxSettingsStrc* pSettings, const D2GfxHelperStrc* pHelpers)
{
	gpGfxSettings = pSettings;
	gpGfxHelpers = pHelpers;
	if (gpGfxSettings)
	{
		gpGfxSettings->bPerspectiveCapable = TRUE;
		if (gpGfxSettings->nGamma > 0)
		{
			gnGammaValue = gpGfxSettings->nGamma;
		}
	}
	RebuildGammaTables();
	gpCriticalSection = static_cast<CRITICAL_SECTION*>(D2_ALLOC(sizeof(CRITICAL_SECTION)));
	InitializeCriticalSection(gpCriticalSection);
	return TRUE;
}

static BOOL __fastcall D2GLIDE_Close()
{
	D2GLIDE_DestroyGlContext();
	ClearDecodedTileCache();

	std::free(gpFrameRGBA);
	gpFrameRGBA = nullptr;
	std::free(gpCompatSurface8);
	gpCompatSurface8 = nullptr;
	std::free(gpGdiFallbackBGRA);
	gpGdiFallbackBGRA = nullptr;
	std::free(gpScratchCellIdx);
	gpScratchCellIdx = nullptr;
	std::free(gpScratchCellMask);
	gpScratchCellMask = nullptr;
	guScratchCellCap = 0;

	if (gpCriticalSection)
	{
		DeleteCriticalSection(gpCriticalSection);
		D2_FREE(gpCriticalSection);
		gpCriticalSection = nullptr;
	}
	return TRUE;
}

static BOOL __fastcall D2GLIDE_CreateSurface(HWND hWnd, D2GameResolutionMode nResolutionMode)
{
	ghWnd = hWnd;
	D2GLIDE_AllocateBuffers(nResolutionMode);

	uint8_t logPalBuf[sizeof(LOGPALETTE) + 256 * sizeof(PALETTEENTRY)] = {};
	LOGPALETTE* pLogPal = reinterpret_cast<LOGPALETTE*>(logPalBuf);
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = 256;
	ghPalette = CreatePalette(pLogPal);

	ghFont = CreateFontA(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	D2GLIDE_EnsureGlContextAndTexture();
	return TRUE;
}

static BOOL __fastcall D2GLIDE_CloseSurface()
{
	D2GLIDE_DestroyGlContext();
	if (ghFont)
	{
		DeleteObject(ghFont);
		ghFont = nullptr;
	}
	if (ghPalette)
	{
		DeleteObject(ghPalette);
		ghPalette = nullptr;
	}
	if (gpGfxHelpers && gpGfxHelpers->pfFillYBufferTable)
	{
		gpGfxHelpers->pfFillYBufferTable(nullptr, 0, 0, 0);
	}
	return TRUE;
}

static void __fastcall D2GLIDE_PauseSurface(HWND hWnd, D2GameResolutionMode nResolutionMode, int32_t nWindowState)
{
	(void)hWnd;
	(void)nResolutionMode;
	if (gpCriticalSection)
	{
		EnterCriticalSection(gpCriticalSection);
		gnWindowState = nWindowState;
		LeaveCriticalSection(gpCriticalSection);
	}
}

static BOOL __fastcall D2GLIDE_StartDraw(int32_t bClear, uint8_t nRed, uint8_t nGreen, uint8_t nBlue)
{
	if (gnWindowState != 0 || !gpFrameRGBA)
	{
		return FALSE;
	}
	if (gpCriticalSection)
	{
		EnterCriticalSection(gpCriticalSection);
	}
	gbInUnitPerspective = FALSE;
	if (bClear)
	{
		const int32_t clearH = (gnSurfaceHeight > 47) ? (gnSurfaceHeight - 47) : gnSurfaceHeight;
		const uint32_t clearCol = PackRGBA(nRed, nGreen, nBlue);
		const size_t count = static_cast<size_t>(clearH) * static_cast<size_t>(gnSurfaceWidth);
		for (size_t i = 0; i < count; ++i)
		{
			gpFrameRGBA[i] = clearCol;
		}
	}
	return TRUE;
}

static BOOL __fastcall D2GLIDE_EndDraw()
{
	gbInUnitPerspective = FALSE;
	if (gpCriticalSection)
	{
		LeaveCriticalSection(gpCriticalSection);
	}
	return TRUE;
}

static BOOL __fastcall D2GLIDE_Blit()
{
	if (!ghWnd || !gpFrameRGBA || gnSurfaceWidth <= 0 || gnSurfaceHeight <= 0)
	{
		return FALSE;
	}

	RECT rcClient = {};
	GetClientRect(ghWnd, &rcClient);
	const int32_t dstW = std::max<int32_t>(1, rcClient.right - rcClient.left);
	const int32_t dstH = std::max<int32_t>(1, rcClient.bottom - rcClient.top);

	if (!ghGlRC)
	{
		D2GLIDE_EnsureGlContextAndTexture();
	}

	if (ghGlDC && ghGlRC && wglMakeCurrent(ghGlDC, ghGlRC))
	{
		if (gGlFrameTex == 0 || gnGlTexWidth != gnSurfaceWidth || gnGlTexHeight != gnSurfaceHeight)
		{
			D2GLIDE_EnsureGlContextAndTexture();
		}

		glViewport(0, 0, dstW, dstH);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, gGlFrameTex);
		const GLint filter = (dstW == gnSurfaceWidth && dstH == gnSurfaceHeight) ? GL_NEAREST : GL_LINEAR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, gnSurfaceWidth, gnSurfaceHeight, GL_RGBA, GL_UNSIGNED_BYTE, gpFrameRGBA);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 1.0f);
		glEnd();

		SwapBuffers(ghGlDC);
		return TRUE;
	}

	// Clean GDI StretchDIBits fallback if WGL is unavailable
	if (gpGdiFallbackBGRA)
	{
		const size_t total = static_cast<size_t>(gnSurfaceWidth) * static_cast<size_t>(gnSurfaceHeight);
		for (size_t i = 0; i < total; ++i)
		{
			const uint32_t px = gpFrameRGBA[i];
			gpGdiFallbackBGRA[i] = 0xFF000000u | ((px & 0xFFu) << 16) | (px & 0xFF00u) | ((px >> 16) & 0xFFu);
		}
		BITMAPINFO bmi = {};
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = gnSurfaceWidth;
		bmi.bmiHeader.biHeight = -gnSurfaceHeight;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;

		HDC hDC = GetDC(ghWnd);
		if (hDC)
		{
			SetStretchBltMode(hDC, HALFTONE);
			StretchDIBits(hDC, 0, 0, dstW, dstH, 0, 0, gnSurfaceWidth, gnSurfaceHeight, gpGdiFallbackBGRA, &bmi, DIB_RGB_COLORS, SRCCOPY);
			ReleaseDC(ghWnd, hDC);
		}
	}
	return TRUE;
}

static BOOL __fastcall D2GLIDE_ChangeRes(HWND hWnd, D2GameResolutionMode nResolutionMode)
{
	ghWnd = hWnd;
	D2GLIDE_AllocateBuffers(nResolutionMode);
	D2GLIDE_EnsureGlContextAndTexture();
	return TRUE;
}

static BOOL __fastcall D2GLIDE_GetBackBuffer(uint8_t* pBuffer)
{
	if (!pBuffer || !gpFrameRGBA)
	{
		return FALSE;
	}
	const size_t total = static_cast<size_t>(gnSurfaceWidth) * static_cast<size_t>(gnSurfaceHeight);
	uint8_t* pDst = pBuffer;
	for (size_t i = 0; i < total; ++i)
	{
		const uint32_t px = gpFrameRGBA[i];
		pDst[0] = static_cast<uint8_t>(px & 0xFFu);
		pDst[1] = static_cast<uint8_t>((px >> 8) & 0xFFu);
		pDst[2] = static_cast<uint8_t>((px >> 16) & 0xFFu);
		pDst += 3;
	}
	return TRUE;
}

static BOOL __fastcall D2GLIDE_ActivateWindow()
{
	return TRUE;
}

static BOOL __fastcall D2GLIDE_SetOption(int32_t nOption, int32_t nValue)
{
	switch (nOption)
	{
	case 8:
		gbWindowedMode = (nValue != 0);
		return TRUE;
	case 11:
		gnContrastValue = nValue;
		return TRUE;
	case 12:
		gnPanelViewMode = nValue;
		return TRUE;
	default:
		return FALSE;
	}
}

static BOOL __fastcall D2GLIDE_PlayCutscene()
{
	return FALSE;
}

static void __fastcall D2GLIDE_OpenSmackCutscene(const char* szFile, D2GameResolutionMode nResolutionMode, void* pfFrame)
{
	(void)szFile;
	(void)nResolutionMode;
	(void)pfFrame;
}

static BOOL __fastcall D2GLIDE_CheckCutScene()
{
	return FALSE;
}

static void D2GLIDE_BlitSmackerFrame(D2GlideSmackerContextStrc* pContext, int32_t nWidth, int32_t nHeight)
{
	if (!pContext || !gpSmackBuffer || !gpFrameRGBA)
	{
		return;
	}
	const int32_t startY = pContext->nYPos - nHeight;
	const int32_t startX = pContext->nXPos;
	const uint8_t* pSrcRow = gpSmackBuffer;
	for (int32_t y = 0; y < nHeight; ++y)
	{
		const int32_t dy = startY + y;
		if (dy >= 0 && dy < gnSurfaceHeight)
		{
			uint32_t* pDstRow = gpFrameRGBA + dy * gnSurfaceWidth;
			for (int32_t x = 0; x < nWidth; ++x)
			{
				const int32_t dx = startX + x;
				if (dx >= 0 && dx < gnSurfaceWidth)
				{
					const uint8_t mappedIdx = gSmackPaletteMap[pSrcRow[x]];
					pDstRow[dx] = gGammaColorRGBA[mappedIdx];
				}
			}
		}
		pSrcRow += nWidth;
	}
}

static void __fastcall D2GLIDE_CloseSmacker(void* pContextVoid)
{
	D2GlideSmackerContextStrc* pContext = static_cast<D2GlideSmackerContextStrc*>(pContextVoid);
	if (gpSmackBuffer)
	{
		D2_FREE(gpSmackBuffer);
		gpSmackBuffer = nullptr;
	}
	if (pContext && pContext->cMode == 's' && pContext->pSmack)
	{
		if (HMODULE hMod = D2GLIDE_GetSmackModule())
		{
			if (auto pfSmackClose = reinterpret_cast<SmackCloseFn>(GetProcAddress(hMod, "_SmackClose@4")))
			{
				pfSmackClose(pContext->pSmack);
			}
		}
		pContext->pSmack = nullptr;
	}
	if (pContext)
	{
		pContext->cMode = 0;
	}
}

static void __fastcall D2GLIDE_PlaySmacker(void* pContextVoid)
{
	D2GlideSmackerContextStrc* pContext = static_cast<D2GlideSmackerContextStrc*>(pContextVoid);
	if (!pContext)
	{
		return;
	}
	if (pContext->cMode == 'p')
	{
		if (gnWindowState == 0)
		{
			D2GLIDE_BlitSmackerFrame(pContext, 468, 60);
		}
		return;
	}

	if (pContext->cMode == 's' && pContext->pSmack)
	{
		D2GlideSmackStrc* pSmack = pContext->pSmack;
		HMODULE hMod = D2GLIDE_GetSmackModule();
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
				D2GLIDE_CloseSmacker(pContext);
			}
			else if (pfSmackNextFrame)
			{
				pfSmackNextFrame(pSmack);
			}
		}

		if (gnWindowState == 0 && gbSmackPaletteInitialized)
		{
			D2GLIDE_BlitSmackerFrame(pContext, static_cast<int32_t>(pSmack->nWidth), static_cast<int32_t>(pSmack->nHeight));
		}
	}
}

static void __fastcall D2GLIDE_DecodeSmacker(const char* szSmacker, uint8_t* pBuffer, int32_t nVersion)
{
	D2GlideSmackerContextStrc* pContext = reinterpret_cast<D2GlideSmackerContextStrc*>(pBuffer);
	if (!pContext)
	{
		return;
	}
	const char cMode = static_cast<char>(nVersion);
	pContext->cMode = cMode;

	if (cMode == 's')
	{
		if (!pContext->pSmack)
		{
			HMODULE hMod = D2GLIDE_GetSmackModule();
			auto pfSmackOpen = hMod ? reinterpret_cast<SmackOpenFn>(GetProcAddress(hMod, "_SmackOpen@12")) : nullptr;
			auto pfSmackToBuffer = hMod ? reinterpret_cast<SmackToBufferFn>(GetProcAddress(hMod, "_SmackToBuffer@28")) : nullptr;

			D2GlideSmackStrc* pSmack = pfSmackOpen ? pfSmackOpen(szSmacker, 0xFE000, -1) : nullptr;
			pContext->pSmack = pSmack;
			if (!pSmack)
			{
				D2GLIDE_CloseSmacker(pContext);
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
			D2GLIDE_CloseSmacker(pContext);
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
			D2GLIDE_CloseSmacker(pContext);
			return;
		}

		for (int32_t i = 0; i < 256; ++i)
		{
			gSmackPaletteMap[i] = static_cast<uint8_t>(D2CMP_GetNearestPaletteIndex(gPaletteEntries, 256, bmpPalette[i].peRed, bmpPalette[i].peGreen, bmpPalette[i].peBlue));
		}
	}
}

static void* __fastcall D2GLIDE_GetCacheResults()
{
	return nullptr;
}

static BOOL __fastcall D2GLIDE_GetDimensions(int32_t* pWidth, int32_t* pHeight)
{
	if (pWidth) *pWidth = gnSurfaceWidth;
	if (pHeight) *pHeight = gnSurfaceHeight;
	return TRUE;
}

static void __fastcall D2GLIDE_SetGlobalScale(int32_t nScaleFactor)
{
	gnGlobalScaleFactor = (nScaleFactor > 0) ? nScaleFactor : 256;
}

static BOOL __fastcall D2GLIDE_SetGamma(int32_t nGamma)
{
	if (nGamma > 0)
	{
		gnGammaValue = nGamma;
		RebuildGammaTables();
	}
	return TRUE;
}

static int32_t __fastcall D2GLIDE_GammaCanBeControlled()
{
	return 1;
}

static BOOL __fastcall D2GLIDE_UpdatePerspective(int32_t nScaleX, int32_t nScaleY)
{
	gnUnitPerspX = nScaleX;
	gnUnitPerspY = nScaleY;
	gbInUnitPerspective = IsPerspectiveActive() ? TRUE : FALSE;
	return TRUE;
}

static void __fastcall D2GLIDE_PerspectiveTransform(int32_t nPosX, int32_t nPosY, int32_t nBias, int32_t* pXAdjust, int32_t* pYAdjust)
{
	if (!IsPerspectiveActive())
	{
		if (pXAdjust) *pXAdjust = nPosX;
		if (pYAdjust) *pYAdjust = nPosY - nBias;
		return;
	}
	float cx = 400.0f, cy = 276.0f;
	GetActiveViewportGeometry(&cx, &cy, nullptr, nullptr, nullptr);
	float ox = 0.0f, oy = 0.0f;
	ProjectElevatedPoint(static_cast<float>(nPosX), static_cast<float>(nPosY), static_cast<float>(nBias), cx, cy, &ox, &oy);
	if (pXAdjust) *pXAdjust = static_cast<int32_t>(std::floor(ox + 0.5f));
	if (pYAdjust) *pYAdjust = static_cast<int32_t>(std::floor(oy + 0.5f));
}

static void __fastcall D2GLIDE_PerspectiveTransformScale(int32_t nPosX, int32_t nPosY, int32_t nAngle, int32_t* pXAdjust, int32_t* pYAdjust, int32_t bOrder)
{
	(void)nAngle;
	if (!IsPerspectiveActive())
	{
		if (pXAdjust) *pXAdjust = nPosX;
		if (pYAdjust) *pYAdjust = nPosY;
		return;
	}
	float cx = 400.0f, cy = 276.0f;
	GetActiveViewportGeometry(&cx, &cy, nullptr, nullptr, nullptr);

	if (bOrder != 0)
	{
		float isoY = 0.0f, invScaleX = 1.0f;
		InverseGroundScanline(static_cast<float>(nPosY), cy, &isoY, &invScaleX);
		const float isoX = cx + (static_cast<float>(nPosX) - cx) * invScaleX;
		if (pXAdjust) *pXAdjust = static_cast<int32_t>(std::floor(isoX + 0.5f));
		if (pYAdjust) *pYAdjust = static_cast<int32_t>(std::floor(isoY + 0.5f));
	}
	else
	{
		float ox = 0.0f, oy = 0.0f;
		ProjectGroundPoint(static_cast<float>(nPosX), static_cast<float>(nPosY), cx, cy, &ox, &oy);
		if (pXAdjust) *pXAdjust = static_cast<int32_t>(std::floor(ox + 0.5f));
		if (pYAdjust) *pYAdjust = static_cast<int32_t>(std::floor(oy + 0.5f));
	}
}

static void __fastcall D2GLIDE_PerspectiveClearScale()
{
	gbInUnitPerspective = FALSE;
}

static void __fastcall D2GLIDE_SetPalette(LPPALETTEENTRY pPalette)
{
	if (!pPalette)
	{
		return;
	}
	for (int32_t i = 0; i < 256; ++i)
	{
		gColorTable[i].rgbRed = pPalette[i].peRed;
		gColorTable[i].rgbGreen = pPalette[i].peGreen;
		gColorTable[i].rgbBlue = pPalette[i].peBlue;
		gColorTable[i].rgbReserved = 0xFF;
	}
	if (ghPalette)
	{
		SetPaletteEntries(ghPalette, 0, 256, pPalette);
	}
	std::memcpy(gPaletteEntries, pPalette, sizeof(gPaletteEntries));
	RebuildGammaTables();
}

static void __fastcall D2GLIDE_SetPaletteTables(D2PaletteTableStrc* pPaletteTables)
{
	if (pPaletteTables)
	{
		std::memcpy(&gPaletteTables, pPaletteTables, 0x48 * sizeof(uint32_t));
		gpPaletteTables = &gPaletteTables;
	}
}

static void __fastcall D2GLIDE_SetAmbientColor(uint8_t nRed, uint8_t nGreen, uint8_t nBlue)
{
	gnAmbientRed = nRed;
	gnAmbientGreen = nGreen;
	gnAmbientBlue = nBlue;
}

static int32_t __fastcall D2GLIDE_FloorTileDraw(
	D2TileLibraryEntryStrc* pTile,
	D2GfxLightExStrc* pLight,
	int32_t nPosX,
	int32_t nPosY,
	int32_t nWorldXpos,
	int32_t nWorldYpos,
	uint8_t nAlpha,
	int32_t nScreenPanels,
	void* pTileData)
{
	(void)nWorldXpos;
	(void)nWorldYpos;
	(void)pTileData;

	if (!gpFrameRGBA || !D2CMP_TileValidate(pTile, TRUE, FALSE))
	{
		return 0;
	}

	const D2GlideCachedTileStrc* pCached = GetOrDecodeTile(pTile, true);
	if (!pCached)
	{
		return 0;
	}

	int32_t x0 = nPosX - 80;
	if (nScreenPanels == 1)
	{
		x0 -= gnSurfaceWidth / 4;
	}
	else if (nScreenPanels == 2)
	{
		x0 += gnSurfaceWidth / 4;
	}
	const int32_t y0 = nPosY;

	uint32_t mulR = 256u;
	uint32_t mulG = 256u;
	uint32_t mulB = 256u;
	if (pLight)
	{
		const D2GfxLightExStrc& sample = pLight[9];
		const uint32_t inten = sample.nIntensity ? sample.nIntensity : 0xFFu;
		const uint32_t lr = (sample.nRed || sample.nGreen || sample.nBlue) ? sample.nRed : 0xFFu;
		const uint32_t lg = (sample.nRed || sample.nGreen || sample.nBlue) ? sample.nGreen : 0xFFu;
		const uint32_t lb = (sample.nRed || sample.nGreen || sample.nBlue) ? sample.nBlue : 0xFFu;
		mulR = std::min<uint32_t>(320u, ((lr * inten) >> 8) + 1u);
		mulG = std::min<uint32_t>(320u, ((lg * inten) >> 8) + 1u);
		mulB = std::min<uint32_t>(320u, ((lb * inten) >> 8) + 1u);
	}

	float cx = 400.0f, cy = 276.0f;
	int32_t clipL = 0, clipR = gnSurfaceWidth, clipB = gnSurfaceHeight;
	GetActiveViewportGeometry(&cx, &cy, &clipL, &clipR, &clipB);

	if (!IsPerspectiveActive())
	{
		const int32_t startY = std::max<int32_t>(0, y0);
		const int32_t endY = std::min<int32_t>(clipB, y0 + 80);
		const int32_t startX = std::max<int32_t>(clipL, x0);
		const int32_t endX = std::min<int32_t>(clipR, x0 + 160);
		for (int32_t sy = startY; sy < endY; ++sy)
		{
			const int32_t tv = sy - y0;
			const uint8_t* pRowIdx = pCached->pIndices + tv * 160;
			const uint8_t* pRowMask = pCached->pMask + tv * 160;
			uint32_t* pDstRow = gpFrameRGBA + sy * gnSurfaceWidth;
			for (int32_t sx = startX; sx < endX; ++sx)
			{
				const int32_t tu = sx - x0;
				if (pRowMask[tu])
				{
					const uint32_t lit = ModulateTexelRGB(pRowIdx[tu], mulR, mulG, mulB);
					pDstRow[sx] = (nAlpha >= 0xF8u) ? lit : BlendAlphaRGBA(pDstRow[sx], lit, nAlpha);
				}
			}
		}
		return 1;
	}

	const float roofH = (pTile->nType == TILETYPE_ROOF && pTile->nRoofHeight > 0)
		? static_cast<float>(pTile->nRoofHeight)
		: 0.0f;
	const float groundTopY = static_cast<float>(y0) + roofH;

	float cX[4] = {}, cY[4] = {};
	ProjectElevatedPoint(static_cast<float>(x0), groundTopY, roofH, cx, cy, &cX[0], &cY[0]);
	ProjectElevatedPoint(static_cast<float>(x0 + 160), groundTopY, roofH, cx, cy, &cX[1], &cY[1]);
	ProjectElevatedPoint(static_cast<float>(x0 + 160), groundTopY + 80.0f, roofH, cx, cy, &cX[2], &cY[2]);
	ProjectElevatedPoint(static_cast<float>(x0), groundTopY + 80.0f, roofH, cx, cy, &cX[3], &cY[3]);

	const float minXf = std::min(std::min(cX[0], cX[1]), std::min(cX[2], cX[3]));
	const float maxXf = std::max(std::max(cX[0], cX[1]), std::max(cX[2], cX[3]));
	const float minYf = std::min(std::min(cY[0], cY[1]), std::min(cY[2], cY[3]));
	const float maxYf = std::max(std::max(cY[0], cY[1]), std::max(cY[2], cY[3]));

	const int32_t syStart = std::max<int32_t>(0, static_cast<int32_t>(std::floor(minYf)) - 1);
	const int32_t syEnd = std::min<int32_t>(clipB, static_cast<int32_t>(std::ceil(maxYf)) + 2);
	const int32_t sxStart = std::max<int32_t>(clipL, static_cast<int32_t>(std::floor(minXf)) - 1);
	const int32_t sxEnd = std::min<int32_t>(clipR, static_cast<int32_t>(std::ceil(maxXf)) + 2);
	if (syStart >= syEnd || sxStart >= sxEnd)
	{
		return 1;
	}

	const int32_t baseGroundTopY = y0 + static_cast<int32_t>(roofH);
	for (int32_t sy = syStart; sy < syEnd; ++sy)
	{
		float isoY = 0.0f;
		float invFactorX = 1.0f;
		InverseElevatedScanline(static_cast<float>(sy) + 0.5f, roofH, cy, &isoY, &invFactorX);
		const int32_t iy = static_cast<int32_t>(std::floor(isoY));
		const int32_t tv = iy - baseGroundTopY;
		if (static_cast<uint32_t>(tv) >= 80u)
		{
			continue;
		}

		const uint8_t* pRowIdx = pCached->pIndices + tv * 160;
		const uint8_t* pRowMask = pCached->pMask + tv * 160;
		uint32_t* pDstRow = gpFrameRGBA + sy * gnSurfaceWidth;

		const float startIsoX = cx + ((static_cast<float>(sxStart) + 0.5f) - cx) * invFactorX;
		int32_t isoXFp = static_cast<int32_t>(std::floor(startIsoX * 65536.0f));
		const int32_t stepXFp = static_cast<int32_t>(invFactorX * 65536.0f);

		for (int32_t sx = sxStart; sx < sxEnd; ++sx)
		{
			const int32_t ix = isoXFp >> 16;
			const int32_t tu = ix - x0;
			if (static_cast<uint32_t>(tu) < 160u && pRowMask[tu])
			{
				const uint32_t lit = ModulateTexelRGB(pRowIdx[tu], mulR, mulG, mulB);
				pDstRow[sx] = (nAlpha >= 0xF8u) ? lit : BlendAlphaRGBA(pDstRow[sx], lit, nAlpha);
			}
			isoXFp += stepXFp;
		}
	}

	return 1;
}

static int32_t DrawWallOrRoofTileInternal(
	D2TileLibraryEntryStrc* pTile,
	int32_t nPosX,
	int32_t nPosY,
	D2GfxLightStrc* pLight,
	int32_t nScreenPanels,
	uint8_t nAlpha)
{
	if (!gpFrameRGBA || nAlpha == 0 || !D2CMP_TileValidate(pTile, TRUE, FALSE))
	{
		return 0;
	}

	if (pTile->nType == TILETYPE_ROOF)
	{
		const D2GlideCachedTileStrc* pRoof = GetOrDecodeTile(pTile, false);
		if (!pRoof)
		{
			return 0;
		}
		float cx = 400.0f, cy = 276.0f;
		int32_t clipL = 0, clipR = gnSurfaceWidth, clipB = gnSurfaceHeight;
		GetActiveViewportGeometry(&cx, &cy, &clipL, &clipR, &clipB);
		if (nScreenPanels == 1) clipR = std::min(clipR, gnSurfaceWidth / 2);
		else if (nScreenPanels == 2) clipL = std::max(clipL, gnSurfaceWidth / 2);

		uint32_t mulR = 256u, mulG = 256u, mulB = 256u;
		if (pLight)
		{
			const uint32_t inten = pLight->nIntensity ? pLight->nIntensity : 0xFFu;
			const uint32_t lr = (pLight->nRed || pLight->nGreen || pLight->nBlue) ? pLight->nRed : 0xFFu;
			const uint32_t lg = (pLight->nRed || pLight->nGreen || pLight->nBlue) ? pLight->nGreen : 0xFFu;
			const uint32_t lb = (pLight->nRed || pLight->nGreen || pLight->nBlue) ? pLight->nBlue : 0xFFu;
			mulR = std::min<uint32_t>(320u, ((lr * inten) >> 8) + 1u);
			mulG = std::min<uint32_t>(320u, ((lg * inten) >> 8) + 1u);
			mulB = std::min<uint32_t>(320u, ((lb * inten) >> 8) + 1u);
		}

		const int32_t baseIsoX = nPosX + pRoof->nMinX;
		const int32_t baseIsoY = nPosY + pRoof->nMinY;
		if (!IsPerspectiveActive())
		{
			const int32_t syStart = std::max<int32_t>(0, baseIsoY);
			const int32_t syEnd = std::min<int32_t>(clipB, baseIsoY + pRoof->nHeight);
			const int32_t sxStart = std::max<int32_t>(clipL, baseIsoX);
			const int32_t sxEnd = std::min<int32_t>(clipR, baseIsoX + pRoof->nWidth);
			for (int32_t sy = syStart; sy < syEnd; ++sy)
			{
				const int32_t tv = sy - baseIsoY;
				const uint8_t* pRowIdx = pRoof->pIndices + tv * pRoof->nWidth;
				const uint8_t* pRowMask = pRoof->pMask + tv * pRoof->nWidth;
				uint32_t* pDstRow = gpFrameRGBA + sy * gnSurfaceWidth;
				for (int32_t sx = sxStart; sx < sxEnd; ++sx)
				{
					const int32_t tu = sx - baseIsoX;
					if (pRowMask[tu])
					{
						const uint32_t lit = ModulateTexelRGB(pRowIdx[tu], mulR, mulG, mulB);
						pDstRow[sx] = BlendAlphaRGBA(pDstRow[sx], lit, nAlpha);
					}
				}
			}
			return 1;
		}

		const float roofH = static_cast<float>(pTile->nRoofHeight);
		const float groundTopY = static_cast<float>(baseIsoY) + roofH;
		QuadVert q[4] = {};
		ProjectElevatedPoint(static_cast<float>(baseIsoX), groundTopY, roofH, cx, cy, &q[0].x, &q[0].y);
		ProjectElevatedPoint(static_cast<float>(baseIsoX + pRoof->nWidth), groundTopY, roofH, cx, cy, &q[1].x, &q[1].y);
		ProjectElevatedPoint(static_cast<float>(baseIsoX + pRoof->nWidth), groundTopY + static_cast<float>(pRoof->nHeight), roofH, cx, cy, &q[2].x, &q[2].y);
		ProjectElevatedPoint(static_cast<float>(baseIsoX), groundTopY + static_cast<float>(pRoof->nHeight), roofH, cx, cy, &q[3].x, &q[3].y);
		q[0].u = 0.0f; q[0].v = 0.0f;
		q[1].u = static_cast<float>(pRoof->nWidth); q[1].v = 0.0f;
		q[2].u = static_cast<float>(pRoof->nWidth); q[2].v = static_cast<float>(pRoof->nHeight);
		q[3].u = 0.0f; q[3].v = static_cast<float>(pRoof->nHeight);
		RasterizeTexturedQuad(pRoof->pIndices, pRoof->pMask, pRoof->nWidth, pRoof->nWidth, pRoof->nHeight, q, mulR, mulG, mulB, nAlpha, clipL, 0, clipR, clipB);
		return 1;
	}

	const D2GlideCachedTileStrc* pWall = GetOrDecodeTile(pTile, false);
	if (!pWall)
	{
		return 0;
	}

	float cx = 400.0f, cy = 276.0f;
	int32_t clipL = 0, clipR = gnSurfaceWidth, clipB = gnSurfaceHeight;
	GetActiveViewportGeometry(&cx, &cy, &clipL, &clipR, &clipB);
	if (nScreenPanels == 1) clipR = std::min(clipR, gnSurfaceWidth / 2);
	else if (nScreenPanels == 2) clipL = std::max(clipL, gnSurfaceWidth / 2);

	uint32_t mulR = 256u, mulG = 256u, mulB = 256u;
	if (pLight)
	{
		const uint32_t inten = pLight->nIntensity ? pLight->nIntensity : 0xFFu;
		const uint32_t lr = (pLight->nRed || pLight->nGreen || pLight->nBlue) ? pLight->nRed : 0xFFu;
		const uint32_t lg = (pLight->nRed || pLight->nGreen || pLight->nBlue) ? pLight->nGreen : 0xFFu;
		const uint32_t lb = (pLight->nRed || pLight->nGreen || pLight->nBlue) ? pLight->nBlue : 0xFFu;
		mulR = std::min<uint32_t>(320u, ((lr * inten) >> 8) + 1u);
		mulG = std::min<uint32_t>(320u, ((lg * inten) >> 8) + 1u);
		mulB = std::min<uint32_t>(320u, ((lb * inten) >> 8) + 1u);
	}

	const int32_t baseIsoX = nPosX + pWall->nMinX;
	const int32_t baseIsoY = nPosY + pWall->nMinY;

	if (!IsPerspectiveActive())
	{
		const int32_t syStart = std::max<int32_t>(0, baseIsoY);
		const int32_t syEnd = std::min<int32_t>(clipB, baseIsoY + pWall->nHeight);
		const int32_t sxStart = std::max<int32_t>(clipL, baseIsoX);
		const int32_t sxEnd = std::min<int32_t>(clipR, baseIsoX + pWall->nWidth);
		for (int32_t sy = syStart; sy < syEnd; ++sy)
		{
			const int32_t tv = sy - baseIsoY;
			const uint8_t* pRowIdx = pWall->pIndices + tv * pWall->nWidth;
			const uint8_t* pRowMask = pWall->pMask + tv * pWall->nWidth;
			uint32_t* pDstRow = gpFrameRGBA + sy * gnSurfaceWidth;
			for (int32_t sx = sxStart; sx < sxEnd; ++sx)
			{
				const int32_t tu = sx - baseIsoX;
				if (pRowMask[tu])
				{
					const uint32_t lit = ModulateTexelRGB(pRowIdx[tu], mulR, mulG, mulB);
					pDstRow[sx] = (nAlpha >= 0xF8u) ? lit : BlendAlphaRGBA(pDstRow[sx], lit, nAlpha);
				}
			}
		}
		return 1;
	}

	const bool bLowerCliffWall = (pTile->nType >= TILETYPE_LEFT_WALL_DOWN);
	constexpr int32_t kStripW = 32;
	for (int32_t stripU = 0; stripU < pWall->nWidth; stripU += kStripW)
	{
		const int32_t curW = std::min<int32_t>(kStripW, pWall->nWidth - stripU);
		const float xL = static_cast<float>(baseIsoX + stripU);
		const float xR = static_cast<float>(baseIsoX + stripU + curW);
		const float yTopIso = static_cast<float>(baseIsoY);
		const float yBotIso = static_cast<float>(baseIsoY + pWall->nHeight);

		QuadVert q[4] = {};
		if (bLowerCliffWall)
		{
			ProjectGroundPoint(xL, yTopIso, cx, cy, &q[0].x, &q[0].y);
			ProjectGroundPoint(xR, yTopIso, cx, cy, &q[1].x, &q[1].y);
			ProjectGroundPoint(xR, yBotIso, cx, cy, &q[2].x, &q[2].y);
			ProjectGroundPoint(xL, yBotIso, cx, cy, &q[3].x, &q[3].y);
		}
		else
		{
			const float gYL = ComputeWallGroundY(pTile->nType, nPosX, nPosY, xL);
			const float gYR = ComputeWallGroundY(pTile->nType, nPosX, nPosY, xR);
			ProjectElevatedPoint(xL, gYL, gYL - yTopIso, cx, cy, &q[0].x, &q[0].y);
			ProjectElevatedPoint(xR, gYR, gYR - yTopIso, cx, cy, &q[1].x, &q[1].y);
			ProjectElevatedPoint(xR, gYR, gYR - yBotIso, cx, cy, &q[2].x, &q[2].y);
			ProjectElevatedPoint(xL, gYL, gYL - yBotIso, cx, cy, &q[3].x, &q[3].y);
		}
		q[0].u = static_cast<float>(stripU);          q[0].v = 0.0f;
		q[1].u = static_cast<float>(stripU + curW);   q[1].v = 0.0f;
		q[2].u = static_cast<float>(stripU + curW);   q[2].v = static_cast<float>(pWall->nHeight);
		q[3].u = static_cast<float>(stripU);          q[3].v = static_cast<float>(pWall->nHeight);

		RasterizeTexturedQuad(pWall->pIndices, pWall->pMask, pWall->nWidth, pWall->nWidth, pWall->nHeight, q, mulR, mulG, mulB, nAlpha, clipL, 0, clipR, clipB);
	}

	return 1;
}

static int32_t __fastcall D2GLIDE_TileDrawLit(D2TileLibraryEntryStrc* pTile, int32_t nPosX, int32_t nPosY, D2GfxLightStrc* pLight, int32_t nScreenPanels)
{
	return DrawWallOrRoofTileInternal(pTile, nPosX, nPosY, pLight, nScreenPanels, 0xFFu);
}

static int32_t __fastcall D2GLIDE_TileDrawTrans(D2TileLibraryEntryStrc* pTile, int32_t nPosX, int32_t nPosY, D2GfxLightStrc* pLight, int32_t nScreenPanels, uint8_t nAlpha)
{
	return DrawWallOrRoofTileInternal(pTile, nPosX, nPosY, pLight, nScreenPanels, nAlpha);
}

static int32_t __fastcall D2GLIDE_ShadowTileDraw(D2TileLibraryEntryStrc* pTile, int32_t nPosX, int32_t nPosY, DrawMode eDrawMode, int32_t nScreenPanels)
{
	(void)eDrawMode;
	if (!gpFrameRGBA || !D2CMP_TileValidate(pTile, TRUE, FALSE))
	{
		return 0;
	}
	const D2GlideCachedTileStrc* pShadow = GetOrDecodeTile(pTile, false);
	if (!pShadow)
	{
		return 0;
	}

	float cx = 400.0f, cy = 276.0f;
	int32_t clipL = 0, clipR = gnSurfaceWidth, clipB = gnSurfaceHeight;
	GetActiveViewportGeometry(&cx, &cy, &clipL, &clipR, &clipB);
	if (nScreenPanels == 1) clipR = std::min(clipR, gnSurfaceWidth / 2);
	else if (nScreenPanels == 2) clipL = std::max(clipL, gnSurfaceWidth / 2);

	const int32_t baseX = nPosX + pShadow->nMinX;
	const int32_t baseY = nPosY + pShadow->nMinY;

	if (!IsPerspectiveActive())
	{
		const int32_t syStart = std::max<int32_t>(0, baseY);
		const int32_t syEnd = std::min<int32_t>(clipB, baseY + pShadow->nHeight);
		const int32_t sxStart = std::max<int32_t>(clipL, baseX);
		const int32_t sxEnd = std::min<int32_t>(clipR, baseX + pShadow->nWidth);
		for (int32_t sy = syStart; sy < syEnd; ++sy)
		{
			const uint8_t* pRowMask = pShadow->pMask + (sy - baseY) * pShadow->nWidth;
			uint32_t* pDstRow = gpFrameRGBA + sy * gnSurfaceWidth;
			for (int32_t sx = sxStart; sx < sxEnd; ++sx)
			{
				if (pRowMask[sx - baseX])
				{
					const uint32_t d = pDstRow[sx];
					pDstRow[sx] = PackRGBA(
						static_cast<uint8_t>(((d & 0xFFu) * 3u) >> 2),
						static_cast<uint8_t>((((d >> 8) & 0xFFu) * 3u) >> 2),
						static_cast<uint8_t>((((d >> 16) & 0xFFu) * 3u) >> 2));
				}
			}
		}
		return 1;
	}

	float cX[4] = {}, cY[4] = {};
	ProjectGroundPoint(static_cast<float>(baseX), static_cast<float>(baseY), cx, cy, &cX[0], &cY[0]);
	ProjectGroundPoint(static_cast<float>(baseX + pShadow->nWidth), static_cast<float>(baseY), cx, cy, &cX[1], &cY[1]);
	ProjectGroundPoint(static_cast<float>(baseX + pShadow->nWidth), static_cast<float>(baseY + pShadow->nHeight), cx, cy, &cX[2], &cY[2]);
	ProjectGroundPoint(static_cast<float>(baseX), static_cast<float>(baseY + pShadow->nHeight), cx, cy, &cX[3], &cY[3]);

	const float minXf = std::min(std::min(cX[0], cX[1]), std::min(cX[2], cX[3]));
	const float maxXf = std::max(std::max(cX[0], cX[1]), std::max(cX[2], cX[3]));
	const float minYf = std::min(std::min(cY[0], cY[1]), std::min(cY[2], cY[3]));
	const float maxYf = std::max(std::max(cY[0], cY[1]), std::max(cY[2], cY[3]));

	const int32_t syStart = std::max<int32_t>(0, static_cast<int32_t>(std::floor(minYf)) - 1);
	const int32_t syEnd = std::min<int32_t>(clipB, static_cast<int32_t>(std::ceil(maxYf)) + 2);
	const int32_t sxStart = std::max<int32_t>(clipL, static_cast<int32_t>(std::floor(minXf)) - 1);
	const int32_t sxEnd = std::min<int32_t>(clipR, static_cast<int32_t>(std::ceil(maxXf)) + 2);

	for (int32_t sy = syStart; sy < syEnd; ++sy)
	{
		float isoY = 0.0f, invScaleX = 1.0f;
		InverseGroundScanline(static_cast<float>(sy) + 0.5f, cy, &isoY, &invScaleX);
		const int32_t tv = static_cast<int32_t>(std::floor(isoY)) - baseY;
		if (static_cast<uint32_t>(tv) >= static_cast<uint32_t>(pShadow->nHeight))
		{
			continue;
		}
		const uint8_t* pRowMask = pShadow->pMask + tv * pShadow->nWidth;
		uint32_t* pDstRow = gpFrameRGBA + sy * gnSurfaceWidth;
		const float startIsoX = cx + ((static_cast<float>(sxStart) + 0.5f) - cx) * invScaleX;
		int32_t isoXFp = static_cast<int32_t>(std::floor(startIsoX * 65536.0f));
		const int32_t stepXFp = static_cast<int32_t>(invScaleX * 65536.0f);

		for (int32_t sx = sxStart; sx < sxEnd; ++sx)
		{
			const int32_t tu = (isoXFp >> 16) - baseX;
			if (static_cast<uint32_t>(tu) < static_cast<uint32_t>(pShadow->nWidth) && pRowMask[tu])
			{
				const uint32_t d = pDstRow[sx];
				pDstRow[sx] = PackRGBA(
					static_cast<uint8_t>(((d & 0xFFu) * 3u) >> 2),
					static_cast<uint8_t>((((d >> 8) & 0xFFu) * 3u) >> 2),
					static_cast<uint8_t>((((d >> 16) & 0xFFu) * 3u) >> 2));
			}
			isoXFp += stepXFp;
		}
	}
	return 1;
}

static void __fastcall D2GLIDE_CelDraw(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, DrawMode eDrawMode, uint8_t* pPalette)
{
	if (!pData || !D2CMP_SpriteValidate(pData, 0, TRUE) || !pData->pCurrentCell)
	{
		return;
	}
	if (gbInUnitPerspective && IsPerspectiveActive())
	{
		DrawCellPerspectiveUnit(pData->pCurrentCell, nPosX, nPosY, pPalette, dwGamma, eDrawMode);
		return;
	}
	DrawCell2D(pData->pCurrentCell, nPosX, nPosY, gnCropLeft, gnCropRight, 0, gnSurfaceHeight, 0, 0, pPalette, dwGamma, eDrawMode);
}

static void __fastcall D2GLIDE_CelFlatSpriteDraw(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, DrawMode eDrawMode, int32_t nScreenMode, uint8_t* pPalette)
{
	(void)nScreenMode;
	if (!pData || !D2CMP_SpriteValidate(pData, 0, TRUE) || !pData->pCurrentCell)
	{
		return;
	}
	DrawCell2D(pData->pCurrentCell, nPosX, nPosY, gnCropLeft, gnCropRight, 0, gnSurfaceHeight, 0, 0, pPalette, dwGamma, eDrawMode);
}

static void __fastcall D2GLIDE_CelDrawColor(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, DrawMode eDrawMode, int32_t nGlobalPaletteShift)
{
	if (!pData || !D2CMP_SpriteValidate(pData, 0, TRUE) || !pData->pCurrentCell)
	{
		return;
	}
	const uint8_t* pShift = nullptr;
	if (gpPaletteTables && nGlobalPaletteShift > 0 && nGlobalPaletteShift < 13)
	{
		pShift = static_cast<const uint8_t*>(gpPaletteTables->standardShiftPalettes[nGlobalPaletteShift]);
	}
	if (gbInUnitPerspective && IsPerspectiveActive())
	{
		DrawCellPerspectiveUnit(pData->pCurrentCell, nPosX, nPosY, pShift, dwGamma, eDrawMode);
		return;
	}
	DrawCell2D(pData->pCurrentCell, nPosX, nPosY, gnCropLeft, gnCropRight, 0, gnSurfaceHeight, 0, 0, pShift, dwGamma, eDrawMode);
}

static void __fastcall D2GLIDE_CelDrawEx(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, int32_t nSkipLines, int32_t nDrawLines, DrawMode eDrawMode)
{
	if (!pData || !D2CMP_SpriteValidate(pData, 0, TRUE) || !pData->pCurrentCell)
	{
		return;
	}
	DrawCell2D(pData->pCurrentCell, nPosX, nPosY, gnCropLeft, gnCropRight, 0, gnSurfaceHeight, nSkipLines, nDrawLines, nullptr, 0xFFFFFFFFu, eDrawMode);
}

static void __fastcall D2GLIDE_CelDrawShadow(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY)
{
	if (!pData || !gpFrameRGBA || !D2CMP_SpriteValidate(pData, 0, TRUE) || !pData->pCurrentCell)
	{
		return;
	}

	const D2GfxCellStrc* pCell = pData->pCurrentCell;
	if (pCell->dwLength == 0)
	{
		return;
	}
	const int32_t nCellH = static_cast<int32_t>(pCell->dwHeight);
	const int32_t nStartX = nPosX + pCell->nXOffset + (pCell->nYOffset >> 1);
	const int32_t nBottomY = nPosY + (pCell->nYOffset >> 1);

	float cx = 400.0f, cy = 276.0f;
	int32_t clipL = 0, clipR = gnSurfaceWidth, clipB = gnSurfaceHeight;
	GetActiveViewportGeometry(&cx, &cy, &clipL, &clipR, &clipB);
	const bool bPersp = IsPerspectiveActive();

	const uint8_t* pSrc = GetCellPixels(pCell);
	const uint8_t* pEnd = pSrc + pCell->dwLength;
	int32_t srcY = 0;
	int32_t curY = nBottomY;
	int32_t rowStartX = nStartX;
	int32_t curX = rowStartX;

	while (pSrc < pEnd && srcY < nCellH)
	{
		const uint8_t ctrl = *pSrc++;
		if (ctrl == 0x80)
		{
			++srcY;
			if ((srcY & 1) == 0)
			{
				--curY;
				--rowStartX;
			}
			curX = rowStartX;
		}
		else if (ctrl > 0x80)
		{
			curX += (ctrl & 0x7F);
		}
		else
		{
			if ((srcY & 1) == 0)
			{
				if (!bPersp)
				{
					if (curY >= 0 && curY < clipB)
					{
						uint32_t* pDstRow = gpFrameRGBA + curY * gnSurfaceWidth;
						for (uint8_t i = 0; i < ctrl && pSrc < pEnd; ++i)
						{
							++pSrc;
							if (curX >= clipL && curX < clipR)
							{
								const uint32_t d = pDstRow[curX];
								pDstRow[curX] = PackRGBA(
									static_cast<uint8_t>(((d & 0xFFu) * 5u) >> 3),
									static_cast<uint8_t>((((d >> 8) & 0xFFu) * 5u) >> 3),
									static_cast<uint8_t>((((d >> 16) & 0xFFu) * 5u) >> 3));
							}
							++curX;
						}
					}
					else
					{
						pSrc += ctrl;
						curX += ctrl;
					}
				}
				else
				{
					float sx0 = 0.0f, sy0 = 0.0f, sc = 1.0f;
					ProjectGroundPoint(static_cast<float>(curX), static_cast<float>(curY), cx, cy, &sx0, &sy0, &sc);
					const float sx1 = sx0 + static_cast<float>(ctrl) * sc;
					const int32_t py0 = static_cast<int32_t>(std::floor(sy0));
					const int32_t py1 = (sc > 1.05f) ? (py0 + 1) : py0;
					const int32_t pxStart = std::max<int32_t>(clipL, static_cast<int32_t>(std::floor(sx0)));
					const int32_t pxEnd = std::min<int32_t>(clipR, static_cast<int32_t>(std::ceil(sx1)));
					for (int32_t py = py0; py <= py1; ++py)
					{
						if (py >= 0 && py < clipB)
						{
							uint32_t* pDstRow = gpFrameRGBA + py * gnSurfaceWidth;
							for (int32_t px = pxStart; px < pxEnd; ++px)
							{
								const uint32_t d = pDstRow[px];
								pDstRow[px] = PackRGBA(
									static_cast<uint8_t>(((d & 0xFFu) * 5u) >> 3),
									static_cast<uint8_t>((((d >> 8) & 0xFFu) * 5u) >> 3),
									static_cast<uint8_t>((((d >> 16) & 0xFFu) * 5u) >> 3));
							}
						}
					}
					pSrc += ctrl;
					curX += ctrl;
				}
			}
			else
			{
				pSrc += ctrl;
				curX += ctrl;
			}
		}
	}
}

static void __fastcall D2GLIDE_CelDrawHilight(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint8_t nPaletteIndex)
{
	if (!pData || !gpFrameRGBA || !D2CMP_SpriteValidate(pData, 0, TRUE) || !pData->pCurrentCell)
	{
		return;
	}
	if (!DecodeCellToScratch(pData->pCurrentCell))
	{
		return;
	}

	const D2GfxCellStrc* pCell = pData->pCurrentCell;
	const int32_t w = static_cast<int32_t>(pCell->dwWidth);
	const int32_t h = static_cast<int32_t>(pCell->dwHeight);
	const uint32_t outlineCol = gGammaColorRGBA[nPaletteIndex];

	float cx = 400.0f, cy = 276.0f;
	int32_t clipL = 0, clipR = gnSurfaceWidth, clipB = gnSurfaceHeight;
	GetActiveViewportGeometry(&cx, &cy, &clipL, &clipR, &clipB);
	const bool bPersp = gbInUnitPerspective && IsPerspectiveActive();

	float anchorScreenX = 0.0f, anchorScreenY = 0.0f, scale = 1.0f;
	if (bPersp)
	{
		ProjectGroundPoint(static_cast<float>(nPosX), static_cast<float>(nPosY), cx, cy, &anchorScreenX, &anchorScreenY, &scale);
	}
	const float tiltX = kPerspTiltX * ((anchorScreenX - cx) * (1.0f / 400.0f));
	const float effScaleY = scale * (1.0f - kPerspTiltY * ((anchorScreenY - cy) * (1.0f / 276.0f)));
	const int32_t startX2D = nPosX + pCell->nXOffset;
	const int32_t topY2D = nPosY + pCell->nYOffset - h + 1;

	for (int32_t y = 0; y < h; ++y)
	{
		const uint8_t* pRowMask = gpScratchCellMask + y * w;
		for (int32_t x = 0; x < w; ++x)
		{
			if (!pRowMask[x])
			{
				continue;
			}
			const bool bEdge =
				(x == 0 || !pRowMask[x - 1]) ||
				(x == w - 1 || !pRowMask[x + 1]) ||
				(y == 0 || !gpScratchCellMask[(y - 1) * w + x]) ||
				(y == h - 1 || !gpScratchCellMask[(y + 1) * w + x]);
			if (!bEdge)
			{
				continue;
			}
			int32_t px = startX2D + x;
			int32_t py = topY2D + y;
			if (bPersp)
			{
				const float u = static_cast<float>(pCell->nXOffset + x);
				const float hPx = static_cast<float>(-(pCell->nYOffset - h + 1 + y));
				px = static_cast<int32_t>(std::floor(anchorScreenX + u * scale + hPx * scale * tiltX + 0.5f));
				py = static_cast<int32_t>(std::floor(anchorScreenY - hPx * effScaleY + 0.5f));
			}
			if (px >= clipL && px < clipR && py >= 0 && py < clipB)
			{
				gpFrameRGBA[py * gnSurfaceWidth + px] = outlineCol;
			}
		}
	}
}

static void __fastcall D2GLIDE_CelDrawClipped(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, void* pCropRectVoid, DrawMode eDrawMode)
{
	if (!pData || !pCropRectVoid || !D2CMP_SpriteValidate(pData, 0, TRUE) || !pData->pCurrentCell)
	{
		return;
	}
	const RECT* pCrop = static_cast<const RECT*>(pCropRectVoid);
	const int32_t clipLeft = std::max<int32_t>(0, pCrop->left);
	const int32_t clipRight = std::min<int32_t>(gnSurfaceWidth, pCrop->top);
	const int32_t clipTop = std::max<int32_t>(0, pCrop->right);
	const int32_t clipBottom = std::min<int32_t>(gnSurfaceHeight, pCrop->bottom);
	if (clipLeft >= clipRight || clipTop >= clipBottom)
	{
		return;
	}
	DrawCell2D(pData->pCurrentCell, nPosX, nPosY, clipLeft, clipRight, clipTop, clipBottom, 0, 0, nullptr, 0xFFFFFFFFu, eDrawMode);
}

static void __fastcall D2GLIDE_DrawLine(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, uint8_t nAlpha)
{
	if (!gpFrameRGBA)
	{
		return;
	}
	const uint32_t col = gGammaColorRGBA[dwColor & 0xFFu];
	int32_t dx = nXEnd - nXStart;
	int32_t dy = nYEnd - nYStart;
	const int32_t stepX = (dx < 0) ? -1 : 1;
	const int32_t stepY = (dy < 0) ? -1 : 1;
	dx = std::abs(dx);
	dy = std::abs(dy);

	int32_t x = nXStart;
	int32_t y = nYStart;
	if (x >= 0 && x < gnSurfaceWidth && y >= 0 && y < gnSurfaceHeight)
	{
		gpFrameRGBA[y * gnSurfaceWidth + x] = BlendAlphaRGBA(gpFrameRGBA[y * gnSurfaceWidth + x], col, nAlpha);
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
				gpFrameRGBA[y * gnSurfaceWidth + x] = BlendAlphaRGBA(gpFrameRGBA[y * gnSurfaceWidth + x], col, nAlpha);
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
				gpFrameRGBA[y * gnSurfaceWidth + x] = BlendAlphaRGBA(gpFrameRGBA[y * gnSurfaceWidth + x], col, nAlpha);
			}
		}
	}
}

static void __fastcall D2GLIDE_UtilFilledRect(RECT* pRect, uint8_t nPaletteIndex)
{
	if (!pRect || !gpFrameRGBA)
	{
		return;
	}
	const int32_t x0 = std::max<int32_t>(0, pRect->left);
	const int32_t x1 = std::min<int32_t>(gnSurfaceWidth, pRect->right);
	const int32_t y0 = std::max<int32_t>(0, pRect->top);
	const int32_t y1 = std::min<int32_t>(gnSurfaceHeight, pRect->bottom);
	if (x0 >= x1 || y0 >= y1)
	{
		return;
	}
	const uint32_t col = gGammaColorRGBA[nPaletteIndex];
	for (int32_t y = y0; y < y1; ++y)
	{
		uint32_t* pRow = gpFrameRGBA + y * gnSurfaceWidth;
		for (int32_t x = x0; x < x1; ++x)
		{
			pRow[x] = col;
		}
	}
}

static void __fastcall D2GLIDE_UtilPoint(POINT* pPoint, uint8_t nSize, uint8_t nPaletteIndex)
{
	if (!pPoint || !nSize)
	{
		return;
	}
	const int32_t r = nSize - 1;
	RECT rc = { pPoint->x - r, pPoint->y - r, pPoint->x + r + 1, pPoint->y + r + 1 };
	D2GLIDE_UtilFilledRect(&rc, nPaletteIndex);
}

static void __fastcall D2GLIDE_UtilRect(RECT* pRect, uint8_t nPaletteIndex)
{
	if (!pRect || pRect->left == pRect->right || pRect->top == pRect->bottom)
	{
		return;
	}
	D2GLIDE_DrawLine(pRect->left, pRect->top, pRect->right, pRect->top, nPaletteIndex, 0xFFu);
	D2GLIDE_DrawLine(pRect->right, pRect->top, pRect->right, pRect->bottom, nPaletteIndex, 0xFFu);
	D2GLIDE_DrawLine(pRect->right, pRect->bottom, pRect->left, pRect->bottom, nPaletteIndex, 0xFFu);
	D2GLIDE_DrawLine(pRect->left, pRect->bottom, pRect->left, pRect->top, nPaletteIndex, 0xFFu);
}

static void __fastcall D2GLIDE_UtilDiamond(RECT* pRect, uint8_t nPaletteIndex)
{
	if (!pRect)
	{
		return;
	}
	const int32_t w = pRect->right - pRect->left;
	const int32_t h = pRect->bottom - pRect->top;
	if (w <= 0 || h <= 0)
	{
		return;
	}
	const int32_t mx = pRect->left + (w >> 1);
	const int32_t my = pRect->top + (h >> 1);
	D2GLIDE_DrawLine(mx, pRect->top, pRect->right, my, nPaletteIndex, 0xFFu);
	D2GLIDE_DrawLine(pRect->right, my, mx, pRect->bottom, nPaletteIndex, 0xFFu);
	D2GLIDE_DrawLine(mx, pRect->bottom, pRect->left, my, nPaletteIndex, 0xFFu);
	D2GLIDE_DrawLine(pRect->left, my, mx, pRect->top, nPaletteIndex, 0xFFu);
}

static void __fastcall D2GLIDE_DrawBox(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, DrawMode eDrawMode)
{
	if (!gpFrameRGBA)
	{
		return;
	}
	const int32_t x0 = std::max<int32_t>(0, std::min<int32_t>(gnSurfaceWidth, nXStart));
	const int32_t x1 = std::max<int32_t>(0, std::min<int32_t>(gnSurfaceWidth, nXEnd));
	const int32_t y0 = std::max<int32_t>(0, std::min<int32_t>(gnSurfaceHeight, nYStart));
	const int32_t y1 = std::max<int32_t>(0, std::min<int32_t>(gnSurfaceHeight, nYEnd));
	if (x0 >= x1 || y0 >= y1)
	{
		return;
	}
	const uint32_t srcCol = gGammaColorRGBA[dwColor & 0xFFu];
	for (int32_t y = y0; y < y1; ++y)
	{
		uint32_t* pRow = gpFrameRGBA + y * gnSurfaceWidth;
		for (int32_t x = x0; x < x1; ++x)
		{
			pRow[x] = BlendPixelRGBA(pRow[x], srcCol, eDrawMode);
		}
	}
}

static void __fastcall D2GLIDE_DrawBoxAlpha(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, uint8_t nAlpha)
{
	if (!gpFrameRGBA || nAlpha == 0)
	{
		return;
	}
	const int32_t x0 = std::max<int32_t>(0, std::min<int32_t>(gnSurfaceWidth, nXStart));
	const int32_t x1 = std::max<int32_t>(0, std::min<int32_t>(gnSurfaceWidth, nXEnd));
	const int32_t y0 = std::max<int32_t>(0, std::min<int32_t>(gnSurfaceHeight, nYStart));
	const int32_t y1 = std::max<int32_t>(0, std::min<int32_t>(gnSurfaceHeight, nYEnd));
	if (x0 >= x1 || y0 >= y1)
	{
		return;
	}
	const uint32_t srcCol = gGammaColorRGBA[dwColor & 0xFFu];
	for (int32_t y = y0; y < y1; ++y)
	{
		uint32_t* pRow = gpFrameRGBA + y * gnSurfaceWidth;
		for (int32_t x = x0; x < x1; ++x)
		{
			pRow[x] = BlendAlphaRGBA(pRow[x], srcCol, nAlpha);
		}
	}
}

static void __fastcall D2GLIDE_ClearScreen(int32_t bPartial)
{
	if (!gpFrameRGBA)
	{
		return;
	}
	const int32_t h = (bPartial && gnSurfaceHeight > 47) ? (gnSurfaceHeight - 47) : gnSurfaceHeight;
	const size_t total = static_cast<size_t>(h) * static_cast<size_t>(gnSurfaceWidth);
	for (size_t i = 0; i < total; ++i)
	{
		gpFrameRGBA[i] = 0xFF000000u;
	}
}

static void __fastcall D2GLIDE_OutputString(int32_t nPosX, int32_t nPosY, const char* szFormat, va_list va)
{
	if (!gpFrameRGBA || !szFormat)
	{
		return;
	}
	char szBuffer[512] = {};
	wvsprintfA(szBuffer, szFormat, va);
	(void)nPosX;
	(void)nPosY;
}

static void __fastcall D2GLIDE_DebugDraw(uint32_t* pLight, uint32_t* pPlayerLight, int32_t nPosX, int32_t nPosY)
{
	(void)pLight;
	(void)pPlayerLight;
	(void)nPosX;
	(void)nPosY;
}

static void __fastcall D2GLIDE_Distort(int32_t nPosX, int32_t nPosY)
{
	(void)nPosX;
	(void)nPosY;
}

static void __fastcall D2GLIDE_ClearCaches()
{
	ClearDecodedTileCache();
}

static D2GraphicsInterfaceStrc gGlideGraphicsInterface =
{
	D2GLIDE_Detect,
	D2GLIDE_Init,
	D2GLIDE_Close,
	D2GLIDE_CreateSurface,
	D2GLIDE_CloseSurface,
	D2GLIDE_PauseSurface,
	D2GLIDE_StartDraw,
	D2GLIDE_EndDraw,
	D2GLIDE_Blit,
	D2GLIDE_ChangeRes,
	D2GLIDE_GetBackBuffer,
	D2GLIDE_ActivateWindow,
	D2GLIDE_SetOption,
	D2GLIDE_PlayCutscene,
	D2GLIDE_OpenSmackCutscene,
	D2GLIDE_CheckCutScene,
	D2GLIDE_DecodeSmacker,
	D2GLIDE_PlaySmacker,
	D2GLIDE_CloseSmacker,
	D2GLIDE_GetCacheResults,
	D2GLIDE_GetDimensions,
	D2GLIDE_SetGlobalScale,
	D2GLIDE_SetGamma,
	D2GLIDE_GammaCanBeControlled,
	D2GLIDE_UpdatePerspective,
	D2GLIDE_PerspectiveTransform,
	D2GLIDE_PerspectiveTransformScale,
	D2GLIDE_PerspectiveClearScale,
	D2GLIDE_SetPalette,
	D2GLIDE_SetPaletteTables,
	D2GLIDE_SetAmbientColor,
	D2GLIDE_FloorTileDraw,
	D2GLIDE_CelFlatSpriteDraw,
	D2GLIDE_CelDraw,
	D2GLIDE_CelDrawColor,
	D2GLIDE_CelDrawEx,
	D2GLIDE_CelDrawShadow,
	D2GLIDE_CelDrawHilight,
	D2GLIDE_CelDrawClipped,
	D2GLIDE_TileDrawLit,
	D2GLIDE_TileDrawTrans,
	D2GLIDE_ShadowTileDraw,
	D2GLIDE_UtilDiamond,
	D2GLIDE_UtilRect,
	D2GLIDE_UtilFilledRect,
	D2GLIDE_UtilPoint,
	D2GLIDE_DrawBox,
	D2GLIDE_DrawBoxAlpha,
	D2GLIDE_DrawLine,
	D2GLIDE_ClearScreen,
	D2GLIDE_OutputString,
	D2GLIDE_DebugDraw,
	D2GLIDE_Distort,
	D2GLIDE_ClearCaches,
};

//1.12a: D2Glide (#10000)
D2GraphicsInterfaceStrc* __fastcall D2GLIDE_GetGraphicsInterface()
{
	return &gGlideGraphicsInterface;
}
