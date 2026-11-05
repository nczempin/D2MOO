#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cmath>

#include <D2Dll.h>
#include <D2CMP.h>
#include <D2CMPTables.h>
#include <Fog.h>
#include <Storm.h>

#pragma pack(push, 1)
struct D2PaletteTableStrc
{
	uint8_t* transPalettes[3];
	uint8_t* shadowLightGammaPalettes[49];
	uint8_t* standardShiftPalettes[13];
	uint8_t* pScreenPalette;
	uint8_t* pLuminancePalette;
	uint8_t* pDarkBlendPalette;
	uint8_t* pDarkenPalette;
	uint8_t* pStandardColorsPalette;
	uint8_t* unk0x118;
	uint8_t* unk0x11C;
	PALETTEENTRY datFilePalette[256];
};

struct D2GfxCellNodeStrc
{
	void* pHardwareData;		// 0x00
	uint32_t dwUnknown0x04;		// 0x04
};

struct D2GfxCellStrc
{
	BOOL bFlip;								//0x00
	uint32_t dwWidth;						//0x04
	uint32_t dwHeight;						//0x08
	int32_t nXOffset;						//0x0C
	int32_t nYOffset;						//0x10
	uint32_t unk0x14;						//0x14
	D2GfxCellNodeStrc* pCellNode;			//0x18
	uint32_t dwLength;						//0x1C
	uint8_t pPixels[1];						//0x20 (inline RLE pixel stream in normalized DC6)
};

struct D2CellFileStrc
{
	uint32_t dwVersion;						//0x00
	struct
	{
		uint16_t dwFlags;
		uint8_t mylastcol;
		uint8_t mytabno : 1;
	};										//0x04
	uint32_t eFormat;						//0x08
	uint32_t dwTermination;					//0x0C
	int32_t nDirections;					//0x10
	int32_t nFrames;						//0x14
	D2GfxCellStrc* pGfxCells[1];			//0x18
};

struct D2GfxDataStrc
{
#ifdef D2_VERSION_110F
	D2GfxCellStrc* pCurrentCell;			//0x00
	D2CellFileStrc* pCellFile;				//0x04
	uint32_t nFrame;						//0x08
	uint32_t nDirection;					//0x0C
	int32_t nMaxDirections;					//0x10
	int32_t nMaxFrames;						//0x14
	uint32_t fFlags;						//0x18
	uint8_t fState;							//0x1C
	union									//0x1D
	{
		uint8_t nComponent;
		uint8_t fItemFlags;
	};
	uint8_t unk0x1E;						//0x1E
	uint8_t unk0x1F;						//0x1F
	int32_t nUnitType;						//0x20
	int32_t nUnitIndex;						//0x24
	int32_t nMode;							//0x28
	int32_t nOverlay;						//0x2C
	union
	{
		struct
		{
			char szToken[4];				//0x30
			char szComponent[4];			//0x34
			char szArmorType[4];			//0x38
			char szMode[4];					//0x3C
			char szHitClass[4];				//0x40
		};
		uint32_t dwName[5];					//0x30
	};
	char* pName;							//0x44
#else
	uint8_t fState;							//0x00
	union									//0x01
	{
		uint8_t nComponent;
		uint8_t fItemFlags;
	};
	uint8_t unk0x02;						//0x02
	uint8_t unk0x03;						//0x03
	int32_t nUnitType;						//0x04
	int32_t nUnitIndex;						//0x08
	int32_t nMode;							//0x0C
	int32_t nOverlay;						//0x10
	union
	{
		struct
		{
			char szToken[4];				//0x14
			char szComponent[4];			//0x18
			char szArmorType[4];			//0x1C
			char szMode[4];					//0x20
			char szHitClass[4];				//0x24
		};
		uint32_t dwName[5];					//0x14
	};
	char* pName;							//0x28
	uint32_t fFlags;						//0x2C
	int32_t nMaxDirections;					//0x30
	D2GfxCellStrc* pCurrentCell;			//0x34
	uint32_t nDirection;					//0x38
	D2CellFileStrc* pCellFile;				//0x3C
	uint32_t nFrame;						//0x40
	int32_t nMaxFrames;						//0x44
#endif
};

struct D2DrawContextStrc
{
	int32_t nXPos;					// 0x00
	int32_t nYPos;					// 0x04
	int32_t nCropLeft;				// 0x08
	int32_t nCropRight;				// 0x0C
	uint8_t* pSurfaceBits;			// 0x10
	int32_t nSurfaceHeight;			// 0x14
	int32_t nSurfaceWidth;			// 0x18
	int32_t nDrawLines;				// 0x1C
	int32_t nSkipLines;				// 0x20
	uint8_t* pDarkenTable;			// 0x24
	int32_t nDrawMode;				// 0x28
	const uint8_t* pPaletteTable;	// 0x2C
	int32_t nUnused0x30;			// 0x30
};
#pragma pack(pop)

// ============================================================================
// Global State (.data / .bss in 1.12a D2CMP.dll)
// ============================================================================
static void(__fastcall* gpfnCelHardwareFreeCallback_6FE342C8)(void*) = nullptr;
static void(__fastcall* gpfnTileHardwareFreeCallback_6FE35CE8)(void*) = nullptr;

static BOOL gbFixBlack_6FE346F4 = FALSE;
static BOOL gbTestBackground_6FE342E4 = FALSE;
static uint8_t gBackgroundColorIndex_6FE346F8 = 0;
static uint8_t gPaletteMask_6FE342E8[256] = {};
static uint8_t gLoadedPaletteRGB_6FE343E8[256 * 3] = {};
static int32_t gnPaletteStart_6FE342DC = 0;
static int32_t gnPaletteEnd_6FE342E0 = 255;
static uint32_t gTransparencyColor_6FE318A8 = 0x0000FF00;

static BOOL gbPalShiftInitialized_6FE35AE4 = FALSE;
static int32_t gnPalShiftAct_6FE31AC4 = -1;
static uint8_t* gpColorMixPalettes_6FE359A0[13] = {};
static uint8_t* gpTransMixPalettes_6FE359E8[8] = {};
static uint8_t* gpNeutralMixPalette_6FE35AD8 = nullptr;

static BOOL gbCompressedDataMode_6FE37EF4 = TRUE;
static void* gpSpriteCacheMemPool_6FE3AB00 = nullptr;
static uint32_t gdwHeaderCacheSize_6FE3AB04 = 0;
static uint32_t gdwHeaderCacheUsed_6FE3AB08 = 0;
static uint32_t gdwFrameCacheSize_6FE3AB10 = 0;
static uint32_t gdwFrameCacheUsed_6FE3AB14 = 0;
static int32_t gnAsyncSpriteLoadCount_6FE3AAF8 = 0;

static BOOL gbDebugDrawMode_6FE3AB2C = FALSE;

static void* gpTileCacheMemPool_6FE3AB34 = nullptr;
static int32_t gnTileCacheMaxSize_6FE3AB38 = 0;
static int32_t gnTileCacheUsedSize_6FE3AB3C = 0;
static int32_t gnTileCacheHits_6FE3AB40 = 0;
static int32_t gnTileCacheMisses_6FE3AB44 = 0;
static D2TileRecordStrc* gpTileRecordList_6FE3AB48 = nullptr;

// ============================================================================
// FastCmp Internal Helpers (RLE / DC6 / Cell Draw / Lens)
// ============================================================================

// 1.12a: 0x6FE25B20
static BOOL __fastcall FastCmpTransformCell_6FE25B20(D2GfxCellStrc* pCell, const uint8_t* pTransform)
{
	if (!pCell || !pTransform)
	{
		return FALSE;
	}
	uint8_t* pCur = pCell->pPixels;
	uint8_t* pEnd = pCur + pCell->dwLength;
	while (pCur < pEnd)
	{
		uint8_t b = *pCur++;
		if (b < 0x80)
		{
			for (uint8_t i = 0; i < b && pCur < pEnd; ++i)
			{
				*pCur = pTransform[*pCur];
				++pCur;
			}
		}
	}
	return TRUE;
}

// 1.12a: 0x6FE261B0 - Draw DC6/RLE cell onto 8-bit software surface
static void __fastcall FastCmpDrawCell_6FE261B0(const D2DrawContextStrc* pCtx, const D2GfxCellStrc* pCell)
{
	if (!pCtx || !pCell || !pCtx->pSurfaceBits || !pCell->pPixels)
	{
		return;
	}

	const int32_t nCellW = (int32_t)pCell->dwWidth;
	const int32_t nCellH = (int32_t)pCell->dwHeight;
	if (nCellW <= 0 || nCellH <= 0)
	{
		return;
	}

	const int32_t nStartX = pCtx->nXPos + pCell->nXOffset;
	const int32_t nBottomY = pCtx->nYPos + pCell->nYOffset;
	const int32_t nClipLeft = (pCtx->nCropLeft > 0) ? pCtx->nCropLeft : 0;
	const int32_t nClipRight = (pCtx->nCropRight > 0 && pCtx->nCropRight <= pCtx->nSurfaceWidth) ? pCtx->nCropRight : pCtx->nSurfaceWidth;

	const int32_t nSkipLines = (pCtx->nSkipLines > 0) ? pCtx->nSkipLines : 0;
	const int32_t nDrawLines = (pCtx->nDrawLines > 0 && pCtx->nDrawLines <= nCellH - nSkipLines) ? pCtx->nDrawLines : (nCellH - nSkipLines);
	if (nDrawLines <= 0 || nStartX >= nClipRight || nStartX + nCellW <= nClipLeft)
	{
		return;
	}

	const bool bTopDown = (pCell->bFlip & 1) != 0;
	const int32_t nStepY = bTopDown ? 1 : -1;
	int32_t curY = bTopDown ? (nBottomY - nCellH + 1 + nSkipLines) : (nBottomY - nSkipLines);

	const uint8_t* pSrc = pCell->pPixels;
	const uint8_t* pSrcEnd = pSrc + pCell->dwLength;
	const uint8_t* pPal = pCtx->pPaletteTable;
	const uint8_t* pDarken = pCtx->pDarkenTable;
	const uint8_t* pTransTable = reinterpret_cast<const uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(pCtx->nDrawMode)));
	const bool bHasTransTable = (reinterpret_cast<uintptr_t>(pTransTable) >= 0x10000u);

	for (int32_t skipped = 0; skipped < nSkipLines && pSrc < pSrcEnd;)
	{
		uint8_t ctrl = *pSrc++;
		if (ctrl == 0x80)
		{
			++skipped;
		}
		else if (ctrl < 0x80)
		{
			pSrc += ctrl;
		}
	}

	int32_t curX = nStartX;
	int32_t linesDrawn = 0;

	while (pSrc < pSrcEnd && linesDrawn < nDrawLines)
	{
		uint8_t ctrl = *pSrc++;
		if (ctrl == 0x80)
		{
			curX = nStartX;
			curY += nStepY;
			++linesDrawn;
		}
		else if (ctrl > 0x80)
		{
			curX += (ctrl & 0x7F);
		}
		else
		{
			int32_t count = ctrl;
			if (curY >= 0 && curY < pCtx->nSurfaceHeight)
			{
				uint8_t* pRow = pCtx->pSurfaceBits + curY * pCtx->nSurfaceWidth;
				for (int32_t i = 0; i < count && pSrc < pSrcEnd; ++i)
				{
					uint8_t px = *pSrc++;
					if (curX >= nClipLeft && curX < nClipRight)
					{
						if (pPal)
						{
							px = pPal[px];
						}
						if (pDarken)
						{
							px = pDarken[px];
						}
						if (bHasTransTable)
						{
							px = pTransTable[(uint32_t(pRow[curX]) << 8) + px];
						}
						pRow[curX] = px;
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

// 1.12a: 0x6FE1A400 - Decode DC6 cell into raw width*height buffer
static BOOL __fastcall CodecDecompressCellToRaw_6FE1A400(const D2GfxCellStrc* pCell, uint8_t* pOutBuffer)
{
	if (!pCell || !pOutBuffer)
	{
		return FALSE;
	}
	const int32_t w = (int32_t)pCell->dwWidth;
	const int32_t h = (int32_t)pCell->dwHeight;
	memset(pOutBuffer, 0, (size_t)w * (size_t)h);

	const uint8_t* pSrc = pCell->pPixels;
	const uint8_t* pSrcEnd = pSrc + pCell->dwLength;
	const bool bTopDown = (pCell->bFlip & 1) != 0;
	const int32_t stepY = bTopDown ? 1 : -1;
	int32_t x = 0;
	int32_t y = bTopDown ? 0 : (h - 1);
	while (pSrc < pSrcEnd && y >= 0 && y < h)
	{
		uint8_t ctrl = *pSrc++;
		if (ctrl == 0x80)
		{
			x = 0;
			y += stepY;
		}
		else if (ctrl > 0x80)
		{
			x += (ctrl & 0x7F);
		}
		else
		{
			for (uint8_t i = 0; i < ctrl && pSrc < pSrcEnd; ++i)
			{
				if (x < w && y >= 0 && y < h)
				{
					pOutBuffer[y * w + x] = *pSrc;
				}
				++pSrc;
				++x;
			}
		}
	}
	return TRUE;
}

// ============================================================================
// CelCmp.cpp Exports (35 functions)
// ============================================================================

// 1.12a: 0x6FE195D0 (#10049)
D2CMP_DLL_DECL void* __stdcall D2CMP_CelGetHardwareData(D2CellFileStrc* pCelFile)
{
	D2_ASSERT(pCelFile != nullptr);
	D2GfxCellStrc* pCell = (D2GfxCellStrc*)pCelFile;
	if (!pCell->pCellNode)
	{
		pCell->pCellNode = D2_CALLOC_STRC(D2GfxCellNodeStrc);
	}
	return &pCell->pCellNode->pHardwareData;
}

// 1.12a: 0x6FE19600 (#10106, 1.10f #10032)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelFileFreeHardware(D2CellFileStrc* pFile)
{
	if (!pFile)
	{
		return TRUE;
	}
	const int32_t nTotalCells = pFile->nDirections * pFile->nFrames;
	if (pFile->dwTermination == 0xB00B1E5)
	{
		for (int32_t i = 0; i < nTotalCells; ++i)
		{
			D2GfxCellStrc* pCell = pFile->pGfxCells[i];
			if (pCell && pCell->pCellNode)
			{
				if (pCell->pCellNode->pHardwareData && gpfnCelHardwareFreeCallback_6FE342C8)
				{
					gpfnCelHardwareFreeCallback_6FE342C8(&pCell->pCellNode->pHardwareData);
				}
				D2_FREE(pCell->pCellNode);
				pCell->pCellNode = nullptr;
			}
		}
	}
	return TRUE;
}

// 1.12a: 0x6FE196D0 (#10052)
D2CMP_DLL_DECL void* __stdcall D2CMP_CelGetHardwareFreeCallback()
{
	return (void*)gpfnCelHardwareFreeCallback_6FE342C8;
}

// 1.12a: 0x6FE196E0 (#10017)
D2CMP_DLL_DECL void __stdcall D2CMP_CelSetHardwareFreeCallback(void(__fastcall* pfnCallback)(void*))
{
	gpfnCelHardwareFreeCallback_6FE342C8 = pfnCallback;
}

// 1.12a: 0x6FE196F0 (#10018, 1.10f #10046)
D2CMP_DLL_DECL int __stdcall D2CMP_CelFileGetCelsPerDirection(D2CellFileStrc* pCelFile)
{
	D2_ASSERT(pCelFile != nullptr);
	return pCelFile->nFrames;
}

// 1.12a: 0x6FE19720 (#10035)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_CelFileGetNumDirections(D2CellFileStrc* pCelFile)
{
	D2_ASSERT(pCelFile != nullptr);
	return pCelFile->nDirections;
}

// 1.12a: 0x6FE19750 (#10021)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_CelGetControlData(D2CellFileStrc* pCelFile)
{
	D2_ASSERT(pCelFile != nullptr);
	return ((D2GfxCellStrc*)pCelFile)->bFlip;
}

// 1.12a: 0x6FE19780 (#10043, 1.10f #10040)
D2CMP_DLL_DECL int __stdcall D2CMP_CelGetOffsetY(D2CellFileStrc* pCelFile)
{
	D2_ASSERT(pCelFile != nullptr);
	return ((D2GfxCellStrc*)pCelFile)->nYOffset;
}

// 1.12a: 0x6FE197B0 (#10008, 1.10f #10039)
D2CMP_DLL_DECL int __stdcall D2CMP_CelGetOffsetX(D2CellFileStrc* pCelFile)
{
	D2_ASSERT(pCelFile != nullptr);
	return ((D2GfxCellStrc*)pCelFile)->nXOffset;
}

// 1.12a: 0x6FE197E0 (#10096, 1.10f #10038)
D2CMP_DLL_DECL int __stdcall D2CMP_CelGetHeight(D2CellFileStrc* pCelFile)
{
	D2_ASSERT(pCelFile != nullptr);
	return (int)((D2GfxCellStrc*)pCelFile)->dwHeight;
}

// 1.12a: 0x6FE19810 (#10031, 1.10f #10037)
D2CMP_DLL_DECL int __stdcall D2CMP_CelGetWidth(D2CellFileStrc* pCelFile)
{
	D2_ASSERT(pCelFile != nullptr);
	return (int)((D2GfxCellStrc*)pCelFile)->dwWidth;
}

// 1.12a: 0x6FE1A3D0 (#10038, 1.10f #10036)
D2CMP_DLL_DECL D2CellFileStrc* __stdcall D2CMP_CelGetHandle(D2GfxDataStrc* pData)
{
	if (!pData)
	{
		return nullptr;
	}
	if (pData->pCellFile)
	{
		D2CellFileStrc* pCellFile = pData->pCellFile;
		int32_t nFileDir = D2CMP_CelGameDirToFileDir((int32_t)pData->nDirection, pCellFile->nDirections);
		int32_t nFrame = (pCellFile->nFrames > 0) ? ((int32_t)pData->nFrame % pCellFile->nFrames) : 0;
		int32_t idx = nFileDir * pCellFile->nFrames + nFrame;
		if (idx >= 0 && idx < pCellFile->nDirections * pCellFile->nFrames)
		{
			pData->pCurrentCell = pCellFile->pGfxCells[idx];
			return (D2CellFileStrc*)pData->pCurrentCell;
		}
		return nullptr;
	}
	if (pData->pCurrentCell)
	{
		return (D2CellFileStrc*)pData->pCurrentCell;
	}
	if (D2CMP_SpriteValidate(pData, 0, TRUE))
	{
		return (D2CellFileStrc*)pData->pCurrentCell;
	}
	return nullptr;
}

// 1.12a: 0x6FE19870 (#10093, 1.10f #10013)
D2CMP_DLL_DECL void __stdcall D2CMP_CelDrawEx(
	D2GfxDataStrc* pData,
	int32_t nXPos,
	int32_t nYPos,
	int32_t nCropLeft,
	int32_t nCropRight,
	uint8_t* pSurfaceBits,
	int32_t nSurfaceHeight,
	int32_t nSurfaceWidth,
	int32_t nUnused,
	int32_t nDrawMode,
	int32_t nDrawLines,
	int32_t nSkipLines,
	int32_t nUnused2)
{
	D2_MAYBE_UNUSED(nUnused);
	D2_MAYBE_UNUSED(nUnused2);
	if (!pData)
	{
		return;
	}
	if (!pData->pCurrentCell && !D2CMP_SpriteValidate(pData, 0, FALSE))
	{
		return;
	}
	D2GfxCellStrc* pCell = pData->pCurrentCell;
	if (!pCell)
	{
		return;
	}

	D2DrawContextStrc ctx = {};
	ctx.nXPos = nXPos;
	ctx.nYPos = nYPos;
	ctx.nCropLeft = nCropLeft;
	ctx.nCropRight = nCropRight;
	ctx.pSurfaceBits = pSurfaceBits;
	ctx.nSurfaceHeight = nSurfaceHeight;
	ctx.nSurfaceWidth = nSurfaceWidth;
	ctx.nSkipLines = nDrawLines;
	ctx.nDrawLines = nSkipLines;
	ctx.pDarkenTable = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(nUnused)));
	ctx.nDrawMode = nDrawMode;
	ctx.pPaletteTable = reinterpret_cast<const uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(nUnused2)));
	FastCmpDrawCell_6FE261B0(&ctx, pCell);
}

// 1.12a: 0x6FE19980 (#10053, 1.10f #10012)
D2CMP_DLL_DECL void __stdcall D2CMP_CelDraw(
	D2GfxDataStrc* pData,
	int32_t nXPos,
	int32_t nYPos,
	int32_t nCropLeft,
	uint8_t* pSurfaceBits,
	int32_t nSurfaceHeight,
	int32_t nCropRight,
	int32_t nSurfaceWidth,
	uint8_t* pDarkenTable,
	int32_t nDrawMode,
	void* pPaletteTable)
{
	if (!pData)
	{
		return;
	}
	if (!pData->pCurrentCell && !D2CMP_SpriteValidate(pData, 0, FALSE))
	{
		return;
	}
	D2GfxCellStrc* pCell = pData->pCurrentCell;
	if (!pCell)
	{
		return;
	}

	D2DrawContextStrc ctx = {};
	ctx.nXPos = nXPos;
	ctx.nYPos = nYPos;
	ctx.nCropLeft = nCropLeft;
	ctx.nCropRight = nCropRight;
	ctx.pSurfaceBits = pSurfaceBits;
	ctx.nSurfaceHeight = nSurfaceHeight;
	ctx.nSurfaceWidth = nSurfaceWidth;
	ctx.nDrawLines = (int32_t)pCell->dwHeight;
	ctx.nSkipLines = 0;
	ctx.pDarkenTable = pDarkenTable;
	ctx.nDrawMode = nDrawMode;
	ctx.pPaletteTable = (const uint8_t*)pPaletteTable;
	FastCmpDrawCell_6FE261B0(&ctx, pCell);
}

// 1.12a: 0x6FE1A8A0 (#10079, 1.10f #10014)
D2CMP_DLL_DECL void __stdcall D2CMP_CelDrawShadow(
	D2GfxDataStrc* pData,
	int32_t nXPos,
	int32_t nYPos,
	int32_t nCropLeft,
	int32_t nCropRight,
	uint8_t* pSurfaceBits,
	int32_t nSurfaceHeight,
	int32_t nSurfaceWidth,
	uint8_t* pShadowTable,
	int32_t nUnused)
{
	D2_MAYBE_UNUSED(nUnused);
	if (!pData)
	{
		return;
	}
	if (!pData->pCurrentCell && !D2CMP_SpriteValidate(pData, 0, FALSE))
	{
		return;
	}
	D2GfxCellStrc* pCell = pData->pCurrentCell;
	if (!pCell || !pSurfaceBits)
	{
		return;
	}

	// Isometric skew shadow rendering (bottom-up scanlines, each scanline pair shifts X by -1)
	const int32_t nCellH = (int32_t)pCell->dwHeight;
	const int32_t nStartX = nXPos + pCell->nXOffset + (pCell->nYOffset >> 1);
	const int32_t nBottomY = nYPos + (pCell->nYOffset >> 1);

	const uint8_t* pSrc = pCell->pPixels;
	const uint8_t* pSrcEnd = pSrc + pCell->dwLength;
	int32_t srcY = 0;
	int32_t curY = nBottomY;
	int32_t rowStartX = nStartX;
	int32_t curX = rowStartX;

	const int32_t nClipLeft = (nCropLeft > 0) ? nCropLeft : 0;
	const int32_t nClipRight = (nCropRight > 0 && nCropRight <= nSurfaceWidth) ? nCropRight : nSurfaceWidth;

	while (pSrc < pSrcEnd && srcY < nCellH)
	{
		uint8_t ctrl = *pSrc++;
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
			if ((srcY & 1) == 0 && curY >= 0 && curY < nSurfaceHeight)
			{
				uint8_t* pRow = pSurfaceBits + curY * nSurfaceWidth;
				for (uint8_t i = 0; i < ctrl && pSrc < pSrcEnd; ++i)
				{
					++pSrc;
					if (curX >= nClipLeft && curX < nClipRight)
					{
						pRow[curX] = pShadowTable ? pShadowTable[pRow[curX]] : 0;
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
	}
}

// 1.12a: 0x6FE19AB0 (#10046)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelFileTransform(D2CellFileStrc* pCelFile, const uint8_t* pTransform)
{
	if (!pCelFile || !pTransform)
	{
		return FALSE;
	}
	const int32_t nTotal = pCelFile->nDirections * pCelFile->nFrames;
	for (int32_t i = 0; i < nTotal; ++i)
	{
		if (pCelFile->pGfxCells[i])
		{
			FastCmpTransformCell_6FE25B20(pCelFile->pGfxCells[i], pTransform);
		}
	}
	return TRUE;
}

// 1.12a: 0x6FE19B20 (#10085)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelFileFree(D2CellFileStrc* pCelFile)
{
	if (!pCelFile)
	{
		return TRUE;
	}
	D2CMP_CelFileFreeHardware(pCelFile);
	const int32_t nTotal = pCelFile->nDirections * pCelFile->nFrames;
	for (int32_t i = 0; i < nTotal; ++i)
	{
		if (pCelFile->pGfxCells[i])
		{
			D2_FREE(pCelFile->pGfxCells[i]);
		}
	}
	D2_FREE(pCelFile);
	return TRUE;
}

// 1.12a: 0x6FE19B70 (#10099)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelFileFreeSerialized(D2CellFileStrc* pCelFile)
{
	if (!pCelFile)
	{
		return TRUE;
	}
	D2CMP_CelFileFreeHardware(pCelFile);
	D2_FREE(pCelFile);
	return TRUE;
}

// 1.12a: 0x6FE19BB0 (#10006, 1.10f #10024)
D2CMP_DLL_DECL void __stdcall D2CMP_CelFileNormalize(D2CellFileStrc* pFile, D2CellFileStrc** ppOutFile, const char* szFile, int nLine, int nSpecVersion, int nUnused)
{
	D2_MAYBE_UNUSED(szFile);
	D2_MAYBE_UNUSED(nLine);
	D2_MAYBE_UNUSED(nSpecVersion);
	D2_MAYBE_UNUSED(nUnused);
	if (!pFile || !ppOutFile)
	{
		return;
	}
	*ppOutFile = pFile;
	if (pFile->dwTermination == 0xB00B1E5)
	{
		return;
	}

	uint8_t* pBase = (uint8_t*)pFile;
	const int32_t nTotal = pFile->nDirections * pFile->nFrames;
	for (int32_t i = 0; i < nTotal; ++i)
	{
		uintptr_t offset = (uintptr_t)pFile->pGfxCells[i];
		D2GfxCellStrc* pCell = (D2GfxCellStrc*)(pBase + offset);
		pCell->pCellNode = nullptr;
		pFile->pGfxCells[i] = pCell;
	}
	pFile->dwTermination = 0xB00B1E5;
}

// 1.12a: 0x6FE19C80 (#10047)
D2CMP_DLL_DECL void __stdcall D2CMP_CelFileSerialize(D2CellFileStrc* pCelFile, D2CellFileStrc** ppOutFile, int32_t* pOutSize)
{
	if (!pCelFile || !ppOutFile || !pOutSize)
	{
		return;
	}
	const int32_t nTotal = pCelFile->nDirections * pCelFile->nFrames;
	int32_t nHeaderSize = 0x18 + nTotal * 4;
	int32_t nTotalSize = nHeaderSize;
	for (int32_t i = 0; i < nTotal; ++i)
	{
		if (pCelFile->pGfxCells[i])
		{
			nTotalSize += 0x20 + pCelFile->pGfxCells[i]->dwLength;
		}
	}

	uint8_t* pBuf = (uint8_t*)D2_CALLOC(nTotalSize);
	D2CellFileStrc* pOut = (D2CellFileStrc*)pBuf;
	memcpy(pOut, pCelFile, 0x18);
	pOut->dwTermination = 0xEEEEEEEF;

	int32_t curOffset = nHeaderSize;
	for (int32_t i = 0; i < nTotal; ++i)
	{
		D2GfxCellStrc* pSrcCell = pCelFile->pGfxCells[i];
		pOut->pGfxCells[i] = (D2GfxCellStrc*)(uintptr_t)curOffset;
		if (pSrcCell)
		{
			D2GfxCellStrc* pDstCell = (D2GfxCellStrc*)(pBuf + curOffset);
			memcpy(pDstCell, pSrcCell, 0x20);
			pDstCell->pCellNode = nullptr;
			if (pSrcCell->dwLength > 0)
			{
				memcpy(pDstCell->pPixels, pSrcCell->pPixels, pSrcCell->dwLength);
			}
			curOffset += 0x20 + pSrcCell->dwLength;
		}
	}
	*ppOutFile = pOut;
	*pOutSize = nTotalSize;
}

// 1.12a: 0x6FE19DF0 (#10062)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelFileConvertOldToNew(void* pOldFile, D2CellFileStrc** ppOutFile, int32_t* pOutSize)
{
	if (!pOldFile || !ppOutFile)
	{
		return FALSE;
	}
	*ppOutFile = (D2CellFileStrc*)pOldFile;
	if (pOutSize)
	{
		*pOutSize = 0;
	}
	return TRUE;
}

// 1.12a: 0x6FE19FD0 (#10074)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelFileConvert24To8(D2CellFileStrc* pCelFile, D2CellFileStrc** ppOutFile, void* pfnConvert, int32_t dwTransColor)
{
	D2_MAYBE_UNUSED(pfnConvert);
	D2_MAYBE_UNUSED(dwTransColor);
	if (!pCelFile || !ppOutFile)
	{
		return FALSE;
	}
	*ppOutFile = pCelFile;
	return TRUE;
}

// 1.12a: 0x6FE1A070 (#10088)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelFileHalve(D2CellFileStrc* pCelFile, D2CellFileStrc** ppOutFile)
{
	if (!pCelFile || !ppOutFile)
	{
		return FALSE;
	}
	*ppOutFile = pCelFile;
	return TRUE;
}

static int32_t MapNumDirsToTableRow(int32_t nNumDirs)
{
	switch (nNumDirs)
	{
	case 1:  return 1;
	case 2:  return 2;
	case 4:  return 3;
	case 8:  return 4;
	case 16: return 5;
	case 32: return 6;
	case 64: return 7;
	default: return 0;
	}
}

// 1.12a: 0x6FE1A0A0 (#10078)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_CelGameDirToShadowGameDir(int32_t nGameDir, int32_t nNumDirs)
{
	int32_t row = MapNumDirsToTableRow(nNumDirs);
	return gGameDirToShadowGameDirTable_6FE2F3E0[row][nGameDir & 63];
}

// 1.12a: 0x6FE1A190 (#10037)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_CelGameDirToNormalDir(int32_t nGameDir, int32_t nNumDirs)
{
	int32_t row = MapNumDirsToTableRow(nNumDirs);
	return gGameDirToNormalDirTable_6FE2EBE0[row][nGameDir & 63];
}

// 1.12a: 0x6FE1A280 (#10097)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_CelConvertBlock(int32_t nSrcDirs, int32_t nDstDirs)
{
	return D2CMP_CelConvertDir(nSrcDirs, nDstDirs, 1, FALSE);
}

// 1.12a: 0x6FE1A2B0 (#10041)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_CelConvertDir(int32_t nDir, int32_t nNumDirs, int32_t nSubDirs, BOOL bFlag)
{
	if (nNumDirs <= 0)
	{
		return 0;
	}
	int32_t nTotalDirs = nNumDirs * (nSubDirs > 0 ? nSubDirs : 1);
	int32_t idx = nDir % (nTotalDirs > 0 ? nTotalDirs : 1);
	if (bFlag)
	{
		return D2CMP_CelNormalDirToFileDir(idx, nNumDirs);
	}
	switch (nNumDirs)
	{
	case 4:  return gDirConvertTables_6FE2FBE0[0 + (idx & 3)];
	case 8:  return gDirConvertTables_6FE2FBE0[8 + (idx & 7)];
	case 16: return gDirConvertTables_6FE2FBE0[16 + (idx & 15)];
	case 32: return gDirConvertTables_6FE2FBE0[32 + (idx & 31)];
	default: return idx;
	}
}

// 1.12a: 0x6FE1A330 (#10013)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_CelGameDirToFileDir(int32_t nGameDir, int32_t nNumDirs)
{
	int32_t row = MapNumDirsToTableRow(nNumDirs);
	return gGameDirToFileDirTable_6FE2E3E0[row][nGameDir & 63];
}

// 1.12a: 0x6FE1A370 (#10081)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_CelNormalDirToFileDir(int32_t nNormalDir, int32_t nNumDirs)
{
	int32_t row = MapNumDirsToTableRow(nNumDirs);
	return gNormalDirToFileDirTable_6FE2D860[row][nNormalDir & 31];
}

// 1.12a: 0x6FE1A470 (#10068)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelGetRawData(D2CellFileStrc* pCelFile, int32_t nDir, int32_t nCell, void* pOutBuffer)
{
	if (!pCelFile || !pOutBuffer)
	{
		return FALSE;
	}
	int32_t idx = nDir * pCelFile->nFrames + nCell;
	if (idx < 0 || idx >= pCelFile->nDirections * pCelFile->nFrames)
	{
		return FALSE;
	}
	return CodecDecompressCellToRaw_6FE1A400(pCelFile->pGfxCells[idx], (uint8_t*)pOutBuffer);
}

// 1.12a: 0x6FE1A4F0 (#10075)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelFileIteratePixels(D2CellFileStrc* pCelFile, void(__stdcall* pfnCallback)(int32_t))
{
	if (!pCelFile || !pfnCallback)
	{
		return FALSE;
	}
	const int32_t nTotal = pCelFile->nDirections * pCelFile->nFrames;
	for (int32_t i = 0; i < nTotal; ++i)
	{
		D2GfxCellStrc* pCell = pCelFile->pGfxCells[i];
		if (!pCell || !pCell->pPixels)
		{
			continue;
		}
		const uint8_t* pCur = pCell->pPixels;
		const uint8_t* pEnd = pCur + pCell->dwLength;
		while (pCur < pEnd)
		{
			uint8_t b = *pCur++;
			if (b < 0x80)
			{
				for (uint8_t j = 0; j < b && pCur < pEnd; ++j)
				{
					pfnCallback(*pCur++);
				}
			}
		}
	}
	return TRUE;
}

// 1.12a: 0x6FE1A570 (#10095)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelCompressCreateCelFile(int32_t hCompress, D2CellFileStrc** ppOutCelFile)
{
	D2_MAYBE_UNUSED(hCompress);
	if (ppOutCelFile)
	{
		*ppOutCelFile = nullptr;
	}
	return FALSE;
}

// 1.12a: 0x6FE1A660 (#10050)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelCompressDestroy(int32_t hCompress)
{
	D2_MAYBE_UNUSED(hCompress);
	return TRUE;
}

// 1.12a: 0x6FE1A780 (#10105)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_CelCompressAddImage(
	int32_t hCompress,
	int32_t nDirection,
	int32_t nWidth,
	int32_t nHeight,
	int32_t nOffsetX,
	int32_t nOffsetY,
	int32_t nControlData,
	const uint8_t* pPixels)
{
	D2_MAYBE_UNUSED(hCompress);
	D2_MAYBE_UNUSED(nDirection);
	D2_MAYBE_UNUSED(nWidth);
	D2_MAYBE_UNUSED(nHeight);
	D2_MAYBE_UNUSED(nOffsetX);
	D2_MAYBE_UNUSED(nOffsetY);
	D2_MAYBE_UNUSED(nControlData);
	D2_MAYBE_UNUSED(pPixels);
	return TRUE;
}

// 1.12a: 0x6FE1A9A0 (#10036)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_CelCompressCreate(int32_t nType, int32_t nNumDirs, const int32_t* pTransColor, BOOL bIs24Bit)
{
	D2_MAYBE_UNUSED(nType);
	D2_MAYBE_UNUSED(nNumDirs);
	D2_MAYBE_UNUSED(pTransColor);
	D2_MAYBE_UNUSED(bIs24Bit);
	return 0;
}

// ============================================================================
// Palette.cpp Exports (13 functions)
// ============================================================================

// 1.12a: 0x6FE1AAA0 (#10045)
D2CMP_DLL_DECL void __stdcall D2CMP_PaletteSetMask(const uint8_t* pMask)
{
	if (pMask)
	{
		memcpy(gPaletteMask_6FE342E8, pMask, 256);
	}
	else
	{
		memset(gPaletteMask_6FE342E8, 1, 256);
	}
}

// 1.12a: 0x6FE1AAD0 (#10086)
D2CMP_DLL_DECL void __stdcall D2CMP_PaletteFixBlack(BOOL bFixBlack)
{
	gbFixBlack_6FE346F4 = bFixBlack;
}

// 1.12a: 0x6FE1AAE0 (#10028)
D2CMP_DLL_DECL uint8_t __stdcall D2CMP_Palette24BitTo8BitPixel(uint8_t nRed, uint8_t nGreen, uint8_t nBlue)
{
	uint32_t rgb = (uint32_t(nRed) << 16) | (uint32_t(nGreen) << 8) | uint32_t(nBlue);
	if (rgb == gTransparencyColor_6FE318A8)
	{
		return 0;
	}
	if (gbTestBackground_6FE342E4 && rgb == 0x00ADADAD)
	{
		return gBackgroundColorIndex_6FE346F8;
	}

	int32_t bestIdx = 1;
	float bestDist = 1.0e30f;
	for (int32_t i = gnPaletteStart_6FE342DC; i <= gnPaletteEnd_6FE342E0; ++i)
	{
		if (i == 0 && !gbFixBlack_6FE346F4)
		{
			continue;
		}
		if (!gPaletteMask_6FE342E8[i])
		{
			continue;
		}
		float dr = (float)((int32_t)nRed - (int32_t)gLoadedPaletteRGB_6FE343E8[i * 3 + 0]) * gLightWeightFactors_6FE2D2F8[0];
		float dg = (float)((int32_t)nGreen - (int32_t)gLoadedPaletteRGB_6FE343E8[i * 3 + 1]) * gLightWeightFactors_6FE2D2F8[1];
		float db = (float)((int32_t)nBlue - (int32_t)gLoadedPaletteRGB_6FE343E8[i * 3 + 2]) * gLightWeightFactors_6FE2D2F8[2];
		float dist = dr * dr + dg * dg + db * db;
		if (dist < bestDist)
		{
			bestDist = dist;
			bestIdx = i;
		}
	}
	return (uint8_t)bestIdx;
}

// 1.12a: 0x6FE1ABC0 (#10014)
D2CMP_DLL_DECL void __stdcall D2CMP_PaletteTestBackground(BOOL bTestBackground)
{
	gbTestBackground_6FE342E4 = bTestBackground;
	if (bTestBackground)
	{
		gBackgroundColorIndex_6FE346F8 = D2CMP_Palette24BitTo8BitPixel(0, 0, 0);
	}
}

// 1.12a: 0x6FE1AD30 (#10070)
D2CMP_DLL_DECL void __stdcall D2CMP_PaletteSetTransparency(uint32_t dwColor)
{
	gTransparencyColor_6FE318A8 = dwColor;
}

// 1.12a: 0x6FE1AD90 (#10066)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_GetFurthestColor(PALETTEENTRY* pPalette, int32_t nPaletteSize, int32_t nRed, int32_t nGreen, int32_t nBlue)
{
	if (!pPalette || nPaletteSize <= 0)
	{
		return 0;
	}
	int32_t bestIdx = 0;
	int32_t maxDist = -1;
	for (int32_t i = 0; i < nPaletteSize; ++i)
	{
		int32_t dr = nRed - (int32_t)pPalette[i].peRed;
		int32_t dg = nGreen - (int32_t)pPalette[i].peGreen;
		int32_t db = nBlue - (int32_t)pPalette[i].peBlue;
		int32_t dist = dr * dr + dg * dg + db * db;
		if (dist >= maxDist)
		{
			maxDist = dist;
			bestIdx = i;
		}
	}
	return bestIdx;
}

// 1.12a: 0x6FE1AE00 (#10067, 1.10f #10004)
D2CMP_DLL_DECL int __stdcall D2CMP_GetNearestPaletteIndex(PALETTEENTRY* pPalette, int nPaletteSize, int nRed, int nGreen, int nBlue)
{
	if (!pPalette || nPaletteSize <= 0)
	{
		return 0;
	}
	int bestIdx = 0;
	int minDist = 0x7FFFFFFF;
	for (int i = 0; i < nPaletteSize; ++i)
	{
		int dr = nRed - (int)pPalette[i].peRed;
		int dg = nGreen - (int)pPalette[i].peGreen;
		int db = nBlue - (int)pPalette[i].peBlue;
		int dist = dr * dr + dg * dg + db * db;
		if (dist <= minDist)
		{
			minDist = dist;
			bestIdx = i;
		}
	}
	return bestIdx;
}

// 1.12a: 0x6FE1AE70 (#10083)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_DestroyD2Palette(void* pPalette)
{
	if (!pPalette)
	{
		return FALSE;
	}
	D2PaletteTableStrc* pPal = (D2PaletteTableStrc*)pPalette;
	for (int i = 0; i < 3; ++i)
	{
		if (pPal->transPalettes[i]) D2_FREE(pPal->transPalettes[i]);
	}
	for (int i = 0; i < 49; ++i)
	{
		if (pPal->shadowLightGammaPalettes[i]) D2_FREE(pPal->shadowLightGammaPalettes[i]);
	}
	for (int i = 0; i < 13; ++i)
	{
		if (pPal->standardShiftPalettes[i]) D2_FREE(pPal->standardShiftPalettes[i]);
	}
	if (pPal->pScreenPalette) D2_FREE(pPal->pScreenPalette);
	if (pPal->pLuminancePalette) D2_FREE(pPal->pLuminancePalette);
	if (pPal->pDarkBlendPalette) D2_FREE(pPal->pDarkBlendPalette);
	if (pPal->pDarkenPalette) D2_FREE(pPal->pDarkenPalette);
	if (pPal->pStandardColorsPalette) D2_FREE(pPal->pStandardColorsPalette);
	if (pPal->unk0x118) D2_FREE(pPal->unk0x118);
	if (pPal->unk0x11C) D2_FREE(pPal->unk0x11C);
	D2_FREE(pPal);
	return TRUE;
}

// 1.12a: 0x6FE1B320 (#10042)
D2CMP_DLL_DECL void __stdcall D2CMP_Palette24BitTo8BitImage(const uint8_t* pSrc, uint8_t* pDst, int32_t nPixels, void* pfnConvert)
{
	if (!pSrc || !pDst || nPixels <= 0)
	{
		return;
	}
	typedef uint8_t(__stdcall* ConvertFn)(uint8_t, uint8_t, uint8_t);
	ConvertFn fn = pfnConvert ? (ConvertFn)pfnConvert : D2CMP_Palette24BitTo8BitPixel;
	for (int32_t i = 0; i < nPixels; ++i)
	{
		pDst[i] = fn(pSrc[i * 3 + 0], pSrc[i * 3 + 1], pSrc[i * 3 + 2]);
	}
}

// 1.12a: 0x6FE1B3B0 (#10080)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_PaletteInit(const char* szPaletteFile, int32_t nStartIdx, int32_t nEndIdx)
{
	memset(gPaletteMask_6FE342E8, 1, 256);
	gnPaletteStart_6FE342DC = (nStartIdx >= 0 && nStartIdx < 256) ? nStartIdx : 0;
	gnPaletteEnd_6FE342E0 = (nEndIdx >= 0 && nEndIdx < 256) ? nEndIdx : 255;

	if (szPaletteFile)
	{
		HSFILE hFile = nullptr;
		if (FOG_FOpenFile(szPaletteFile, &hFile))
		{
			int nRead = 0;
			FOG_FReadFile(hFile, gLoadedPaletteRGB_6FE343E8, 256 * 3, &nRead, 0, 0, 0);
			FOG_FCloseFile(hFile);
			return TRUE;
		}
	}
	return TRUE;
}

// 1.12a: 0x6FE1C9F0 (#10090)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_UpdateD2TransparencyTable(void* pPalette, int32_t nUnused)
{
	D2_MAYBE_UNUSED(pPalette);
	D2_MAYBE_UNUSED(nUnused);
	return TRUE;
}

// 1.12a: 0x6FE1CA00 (#10055)
D2CMP_DLL_DECL void* __stdcall D2CMP_CreateD2Palette(PALETTEENTRY* pPaletteEntries)
{
	D2PaletteTableStrc* pPal = D2_CALLOC_STRC(D2PaletteTableStrc);
	if (pPaletteEntries)
	{
		memcpy(pPal->datFilePalette, pPaletteEntries, sizeof(PALETTEENTRY) * 256);
	}
	return pPal;
}

// 1.12a: 0x6FE1CAD0 (#10058)
D2CMP_DLL_DECL void* __stdcall D2CMP_CreateD2PaletteByteStream(PALETTEENTRY* pPaletteEntries, uint32_t* pOutSize)
{
	void* pPal = D2CMP_CreateD2Palette(pPaletteEntries);
	if (pOutSize)
	{
		*pOutSize = sizeof(D2PaletteTableStrc);
	}
	return pPal;
}

// ============================================================================
// PalShift.cpp Exports (3 functions)
// ============================================================================

// 1.12a: 0x6FE1CD00 (#10063)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_PalShiftInit()
{
	if (gbPalShiftInitialized_6FE35AE4)
	{
		return TRUE;
	}
	if (!gpNeutralMixPalette_6FE35AD8)
	{
		gpNeutralMixPalette_6FE35AD8 = (uint8_t*)D2_ALLOC(256);
		for (int i = 0; i < 256; ++i)
		{
			gpNeutralMixPalette_6FE35AD8[i] = (uint8_t)i;
		}
	}
	gbPalShiftInitialized_6FE35AE4 = TRUE;
	return TRUE;
}

// 1.12a: 0x6FE1CB30 (#10019)
D2CMP_DLL_DECL void __stdcall D2CMP_PalShiftSetAct(int32_t nAct)
{
	if (!gbPalShiftInitialized_6FE35AE4)
	{
		D2CMP_PalShiftInit();
	}
	gnPalShiftAct_6FE31AC4 = nAct;
}

// 1.12a: 0x6FE1CAF0 (#10059, 1.10f #10098)
D2CMP_DLL_DECL uint8_t* __stdcall D2CMP_MixPalette(uint8_t nTrans, int nColor)
{
	if (!gbPalShiftInitialized_6FE35AE4)
	{
		D2CMP_PalShiftInit();
	}
	if (nColor > 0 && nColor <= 12 && gpColorMixPalettes_6FE359A0[nColor])
	{
		return gpColorMixPalettes_6FE359A0[nColor];
	}
	if (nTrans < 8 && gpTransMixPalettes_6FE359E8[nTrans])
	{
		return gpTransMixPalettes_6FE359E8[nTrans];
	}
	return gpNeutralMixPalette_6FE35AD8;
}

// ============================================================================
// TileLib.cpp Exports (15 functions)
// ============================================================================

// 1.12a: 0x6FE1D000 (#10092)
D2CMP_DLL_DECL const char* __stdcall D2CMP_TileGetFilename(D2TileLibraryEntryStrc* pTile)
{
	D2_ASSERT(pTile != nullptr);
	return pTile->szTileName;
}

// 1.12a: 0x6FE1D030 (#10072)
D2CMP_DLL_DECL uint16_t __stdcall D2CMP_TileGetRoofHeight(D2TileLibraryEntryStrc* pTile)
{
	D2_ASSERT(pTile != nullptr);
	return pTile->nRoofHeight;
}

// 1.12a: 0x6FE1D060 (#10100)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_TileGetHeightToBottom(D2TileLibraryEntryStrc* pTile)
{
	D2_ASSERT(pTile != nullptr);
	return pTile->nHeightToBottom;
}

// 1.12a: 0x6FE1D090 (#10060, 1.10f #10085)
D2CMP_DLL_DECL uint8_t* __stdcall D2CMP_10085_GetTileFlagArray(D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	D2_ASSERT(pTileLibraryEntry != nullptr);
	return pTileLibraryEntry->dwTileFlags;
}

// 1.12a: 0x6FE1D0C0 (#10034)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_TileGetTotalHeight(D2TileLibraryEntryStrc* pTile)
{
	D2_ASSERT(pTile != nullptr);
	return pTile->nTotalHeight;
}

// 1.12a: 0x6FE1D0F0 (#10089)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_TileGetLightDirection(D2TileLibraryEntryStrc* pTile)
{
	D2_ASSERT(pTile != nullptr);
	return pTile->nLightDirection;
}

// 1.12a: 0x6FE1D120 (#10084, 1.10f #10082)
D2CMP_DLL_DECL int __stdcall D2CMP_10082_GetTileSequence(D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	D2_ASSERT(pTileLibraryEntry != nullptr);
	return pTileLibraryEntry->nSequence;
}

// 1.12a: 0x6FE1D150 (#10102, 1.10f #10081)
D2CMP_DLL_DECL int __stdcall D2CMP_10081_GetTileRarity(D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	D2_ASSERT(pTileLibraryEntry != nullptr);
	return pTileLibraryEntry->nRarity_Frame;
}

// 1.12a: 0x6FE1D180 (#10033)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_TileGetWidth(D2TileLibraryEntryStrc* pTile)
{
	D2_ASSERT(pTile != nullptr);
	return pTile->nWidth;
}

// 1.12a: 0x6FE1D1B0 (#10010, 1.10f #10079)
D2CMP_DLL_DECL int __stdcall D2CMP_10079_GetTileFlags(D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	D2_ASSERT(pTileLibraryEntry != nullptr);
	return pTileLibraryEntry->nFlags;
}

// 1.12a: 0x6FE1D1E0 (#10005, 1.10f #10078)
D2CMP_DLL_DECL int __stdcall D2CMP_10078_GetTileStyle(D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	D2_ASSERT(pTileLibraryEntry != nullptr);
	return pTileLibraryEntry->nStyle;
}

// 1.12a: 0x6FE1D210 (#10000, 1.10f #10077)
D2CMP_DLL_DECL int __stdcall D2CMP_10077_GetTileType(D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	D2_ASSERT(pTileLibraryEntry != nullptr);
	return pTileLibraryEntry->nType;
}

// 1.12a: 0x6FE1D240 (#10040)
D2CMP_DLL_DECL void __stdcall D2CMP_TileSetHardwareFreeCallback(void(__fastcall* pfnCallback)(void*))
{
	gpfnTileHardwareFreeCallback_6FE35CE8 = pfnCallback;
}

// 1.12a: 0x6FE1D250 (#10023, 1.10f #10088)
D2CMP_DLL_DECL int __stdcall D2CMP_10088_GetTiles(
	D2TileLibraryHashStrc** ppTileLibraryHash,
	int nType,
	int nStyle,
	int nSequence,
	D2TileLibraryEntryStrc** pTileList,
	int nTileListSize)
{
	if (!ppTileLibraryHash || !pTileList || nTileListSize <= 0)
	{
		return 0;
	}
	int32_t hashIdx = ((nStyle << 5) + (nSequence << 2) + nType) & 0x7F;
	int nFound = 0;

	for (int slot = 0; slot < 32 && ppTileLibraryHash[slot] && nFound < nTileListSize; ++slot)
	{
		D2TileLibraryHashStrc* pHash = ppTileLibraryHash[slot];
		for (D2TileLibraryHashNodeStrc* pNode = pHash->pNodes[hashIdx]; pNode; pNode = pNode->pPrev)
		{
			if (pNode->nType == nType && pNode->nStyle == nStyle && pNode->nSequence == nSequence)
			{
				for (D2TileLibraryHashRefStrc* pRef = pNode->pRef; pRef && nFound < nTileListSize; pRef = pRef->pPrev)
				{
					pTileList[nFound++] = pRef->pTile;
				}
			}
		}
	}
	return nFound;
}

// 1.12a: 0x6FE1D330 (#10024, 1.10f #10087)
D2CMP_DLL_DECL void __stdcall D2CMP_10087_LoadTileLibrarySlot(D2TileLibraryHashStrc** ppTileLibraryHash, const char* szFileName)
{
	if (!ppTileLibraryHash || !szFileName)
	{
		return;
	}
	for (int slot = 0; slot < 32; ++slot)
	{
		if (!ppTileLibraryHash[slot])
		{
			D2CMP_TileProjectsLoadLibrary(&ppTileLibraryHash[slot], szFileName);
			return;
		}
	}
}

// ============================================================================
// Tilecmp.cpp Exports (11 functions)
// ============================================================================

// 1.12a: 0x6FE1F840 (#10103)
D2CMP_DLL_DECL void* __stdcall D2CMP_TileListCreate(int32_t dwVersion, int32_t dwFlags)
{
	D2TileLibraryHeaderStrc* pList = D2_CALLOC_STRC(D2TileLibraryHeaderStrc);
	pList->dwVersion = dwVersion;
	pList->dwFlags = dwFlags;
	return pList;
}

// 1.12a: 0x6FE1F990 (#10056)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_TileProjectIteratePixels(D2TileLibraryHeaderStrc* pHeader, void(__stdcall* pfnCallback)(int32_t))
{
	D2_MAYBE_UNUSED(pHeader);
	D2_MAYBE_UNUSED(pfnCallback);
	return TRUE;
}

// 1.12a: 0x6FE1FB00 (#10004)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_TileProjectNormalize(D2TileLibraryHeaderStrc* pHeader, D2TileLibraryHeaderStrc** ppOutHeader, const char* szLibraryName)
{
	if (!pHeader || !ppOutHeader)
	{
		return FALSE;
	}
	*ppOutHeader = pHeader;
	if (szLibraryName)
	{
		SStrCopy(pHeader->szLibraryName, szLibraryName, sizeof(pHeader->szLibraryName));
	}
	if (pHeader->dwFlags == 0xB00B1E5)
	{
		return TRUE;
	}

	uint8_t* pBase = (uint8_t*)pHeader;
	D2TileLibraryEntryStrc* pTiles = (D2TileLibraryEntryStrc*)(pBase + pHeader->dwTileStart_pFirst);
	pHeader->dwTileStart_pFirst = (int32_t)(uintptr_t)pTiles;
	for (int32_t i = 0; i < pHeader->nTiles; ++i)
	{
		D2TileLibraryEntryStrc* pTile = &pTiles[i];
		pTile->szTileName = pHeader->szLibraryName;
		pTile->pLRUCacheBlock = nullptr;
		if (pTile->dwBlockOffset_pBlock != 0 && pTile->nBlocks > 0)
		{
			D2TileLibraryBlockStrc* pBlocks = (D2TileLibraryBlockStrc*)(pBase + pTile->dwBlockOffset_pBlock);
			pTile->pBlock = pBlocks;
			for (int32_t b = 0; b < pTile->nBlocks; ++b)
			{
				pBlocks[b].dwOffset_pData = (uint8_t*)pBlocks + (uintptr_t)pBlocks[b].dwOffset_pData;
			}
		}
		else
		{
			pTile->pBlock = nullptr;
		}
	}
	pHeader->dwFlags = 0xB00B1E5;
	return TRUE;
}

// 1.12a: 0x6FE1FC10 (#10001)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_TileProjectSerialize(D2TileLibraryHeaderStrc* pHeader, D2TileLibraryHeaderStrc** ppOutHeader, int32_t* pOutSize)
{
	if (!pHeader || !ppOutHeader)
	{
		return FALSE;
	}
	*ppOutHeader = pHeader;
	if (pOutSize)
	{
		*pOutSize = sizeof(D2TileLibraryHeaderStrc);
	}
	return TRUE;
}

// 1.12a: 0x6FE20040 (#10101)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_TileAddRoof(void* pEntry, void* pPixels, int32_t nStride, int32_t nOffsetX, int32_t nOffsetY)
{
	D2_MAYBE_UNUSED(pEntry);
	D2_MAYBE_UNUSED(pPixels);
	D2_MAYBE_UNUSED(nStride);
	D2_MAYBE_UNUSED(nOffsetX);
	D2_MAYBE_UNUSED(nOffsetY);
	return TRUE;
}

// 1.12a: 0x6FE201A0 (#10091)
D2CMP_DLL_DECL void* __stdcall D2CMP_TileListAddTile(
	void* pList,
	int32_t nLightDir,
	int32_t nRoofHeight,
	int32_t nFlags,
	int32_t nType,
	int32_t nStyle,
	int32_t nSequence,
	int32_t nRarity,
	int32_t dwTransColor)
{
	D2_MAYBE_UNUSED(pList);
	D2TileLibraryEntryStrc* pTile = D2_CALLOC_STRC(D2TileLibraryEntryStrc);
	pTile->nLightDirection = nLightDir;
	pTile->nRoofHeight = (uint16_t)nRoofHeight;
	pTile->nFlags = (uint16_t)nFlags;
	pTile->nType = nType;
	pTile->nStyle = nStyle;
	pTile->nSequence = nSequence;
	pTile->nRarity_Frame = nRarity;
	pTile->transparentColorRGB24 = dwTransColor;
	return pTile;
}

// 1.12a: 0x6FE20380 (#10011)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_TileProjectFree(D2TileLibraryHeaderStrc* pHeader, BOOL bKeepMemory)
{
	if (!pHeader)
	{
		return TRUE;
	}
	if (!bKeepMemory)
	{
		D2_FREE(pHeader);
	}
	return TRUE;
}

// 1.12a: 0x6FE204D0 (#10069)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_TileListToTileProject(void* pList, D2TileLibraryHeaderStrc** ppOutHeader)
{
	if (!pList || !ppOutHeader)
	{
		return FALSE;
	}
	*ppOutHeader = (D2TileLibraryHeaderStrc*)pList;
	return TRUE;
}

// 1.12a: 0x6FE20730 (#10012)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_TileListDestroy(void* pList)
{
	if (pList)
	{
		D2_FREE(pList);
	}
	return TRUE;
}

// 1.12a: 0x6FE208A0 (#10027)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_TileSetFloorImage(void* pEntry, void* pPixels)
{
	D2_MAYBE_UNUSED(pEntry);
	D2_MAYBE_UNUSED(pPixels);
	return TRUE;
}

// 1.12a: 0x6FE20900 (#10044)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_TileProjectConvert24To8(D2TileLibraryHeaderStrc* pHeader, D2TileLibraryHeaderStrc** ppOutHeader, void* pfnConvert)
{
	D2_MAYBE_UNUSED(pfnConvert);
	if (!pHeader || !ppOutHeader)
	{
		return FALSE;
	}
	*ppOutHeader = pHeader;
	return TRUE;
}

// ============================================================================
// SpriteCache.cpp Exports (16 functions)
// ============================================================================

// 1.12a: 0x6FE21080 (#10082, 1.10f #10099)
D2CMP_DLL_DECL void __stdcall D2CMP_SpriteFreeAsyncLoads()
{
	gnAsyncSpriteLoadCount_6FE3AAF8 = 0;
}

// 1.12a: 0x6FE210E0 (#10077)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_SpriteCacheIsLoaded(int32_t a1, int32_t a2, int32_t a3)
{
	D2_MAYBE_UNUSED(a1);
	D2_MAYBE_UNUSED(a2);
	D2_MAYBE_UNUSED(a3);
	return TRUE;
}

// 1.12a: 0x6FE21170 (#10007)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_SpriteCacheGetAsyncLoadingCount()
{
	return gnAsyncSpriteLoadCount_6FE3AAF8;
}

// 1.12a: 0x6FE21520 (#10094)
D2CMP_DLL_DECL uint32_t __stdcall D2CMP_SpriteCacheGetHeaderCacheSize()
{
	return gdwHeaderCacheSize_6FE3AB04;
}

// 1.12a: 0x6FE21530 (#10076)
D2CMP_DLL_DECL uint32_t __stdcall D2CMP_SpriteCacheGetHeaderCacheUsed()
{
	return gdwHeaderCacheUsed_6FE3AB08;
}

// 1.12a: 0x6FE21540 (#10073)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_SpriteCacheNoop1()
{
	return 0;
}

// 1.12a: 0x6FE21550 (#10054)
D2CMP_DLL_DECL uint32_t __stdcall D2CMP_SpriteCacheGetFrameCacheSize()
{
	return gdwFrameCacheSize_6FE3AB10;
}

// 1.12a: 0x6FE21560 (#10022)
D2CMP_DLL_DECL uint32_t __stdcall D2CMP_SpriteCacheGetFrameCacheUsed()
{
	return gdwFrameCacheUsed_6FE3AB14;
}

// 1.12a: 0x6FE21D30 (#10009, 1.10f #10054)
D2CMP_DLL_DECL void __stdcall D2CMP_SetCompressedDataMode(BOOL bAllowCompressedMode)
{
	gbCompressedDataMode_6FE37EF4 = bAllowCompressedMode;
}

// 1.12a: 0x6FE21D60 (#10057, 1.10f #10051)
D2CMP_DLL_DECL const char* __stdcall D2CMP_GetGfxFileExtension(BOOL bAllowCompressed)
{
	if (bAllowCompressed && gbCompressedDataMode_6FE37EF4)
	{
		return ".dcc";
	}
	return ".dc6";
}

// 1.12a: 0x6FE22330 (#10029)
D2CMP_DLL_DECL void __stdcall D2CMP_SpriteCacheNoop2()
{
}

struct D2SpriteCacheEntryStrc
{
	char szKey[260];
	D2CellFileStrc* pCellFile;
	int32_t bSeparateCells;
	int32_t bNotFound;
	D2SpriteCacheEntryStrc* pNext;
};

static D2SpriteCacheEntryStrc* gpSpriteCacheBuckets[256] = {};

static const uint8_t kDccBitsTable[16] = {
	0, 1, 2, 4, 6, 8, 10, 12, 14, 16, 20, 24, 26, 28, 30, 32
};

struct DccBitStream
{
	const uint8_t* pData;
	uint32_t nByteLen;
	uint32_t nBitPos;

	uint32_t ReadBits(uint32_t nBits)
	{
		if (nBits == 0)
		{
			return 0;
		}
		uint32_t val = 0;
		for (uint32_t i = 0; i < nBits; ++i)
		{
			const uint32_t pos = nBitPos++;
			const uint32_t byteIdx = pos >> 3;
			if (byteIdx < nByteLen)
			{
				const uint32_t bit = (pData[byteIdx] >> (pos & 7u)) & 1u;
				val |= (bit << i);
			}
		}
		return val;
	}

	int32_t ReadSigned(uint32_t nBits)
	{
		if (nBits == 0)
		{
			return 0;
		}
		const uint32_t u = ReadBits(nBits);
		if (nBits < 32 && (u & (1u << (nBits - 1))))
		{
			return static_cast<int32_t>(u | (~0u << nBits));
		}
		return static_cast<int32_t>(u);
	}
};

struct DccFrameHeader
{
	uint32_t nVar0;
	int32_t nWidth;
	int32_t nHeight;
	int32_t nXOffset;
	int32_t nYOffset;
	uint32_t nOptionalBytes;
	uint32_t nCodedBytes;
	uint32_t bBottomUp;
	int32_t nXMin;
	int32_t nXMax;
	int32_t nYMin;
	int32_t nYMax;
};

struct DccDirCellState
{
	uint8_t* pLastPix;
	int32_t nLastW;
	int32_t nLastH;
	int32_t nLastCanvasX;
	int32_t nLastCanvasY;
	int32_t bVisited;
};

static D2GfxCellStrc* EncodeCanvasRectToGfxCell(
	const uint8_t* pCanvas,
	int32_t nCanvasStride,
	int32_t nRelX,
	int32_t nRelY,
	int32_t nWidth,
	int32_t nHeight,
	int32_t nXOffset,
	int32_t nYOffset,
	uint32_t bBottomUp)
{
	if (nWidth <= 0 || nHeight <= 0)
	{
		D2GfxCellStrc* pEmpty = (D2GfxCellStrc*)D2_CALLOC(0x24);
		pEmpty->dwWidth = 1;
		pEmpty->dwHeight = 1;
		pEmpty->nXOffset = nXOffset;
		pEmpty->nYOffset = nYOffset;
		pEmpty->dwLength = 1;
		pEmpty->pPixels[0] = 0x80;
		return pEmpty;
	}

	const size_t nMaxRle = static_cast<size_t>(nWidth * 2 + 4) * static_cast<size_t>(nHeight);
	uint8_t* pTempRle = (uint8_t*)D2_ALLOC(nMaxRle);
	size_t nRleLen = 0;

	for (int32_t rowIdx = 0; rowIdx < nHeight; ++rowIdx)
	{
		const int32_t srcY = bBottomUp ? (nRelY + rowIdx) : (nRelY + nHeight - 1 - rowIdx);
		const uint8_t* pRow = pCanvas + srcY * nCanvasStride + nRelX;

		int32_t x = 0;
		while (x < nWidth)
		{
			int32_t transStart = x;
			while (x < nWidth && pRow[x] == 0)
			{
				++x;
			}
			if (x >= nWidth)
			{
				break;
			}
			int32_t transCount = x - transStart;
			while (transCount > 0)
			{
				const int32_t chunk = (transCount > 0x7F) ? 0x7F : transCount;
				pTempRle[nRleLen++] = static_cast<uint8_t>(0x80 | chunk);
				transCount -= chunk;
			}

			int32_t opqStart = x;
			while (x < nWidth && pRow[x] != 0)
			{
				++x;
			}
			int32_t opqCount = x - opqStart;
			int32_t opqPos = opqStart;
			while (opqCount > 0)
			{
				const int32_t chunk = (opqCount > 0x7F) ? 0x7F : opqCount;
				pTempRle[nRleLen++] = static_cast<uint8_t>(chunk);
				memcpy(&pTempRle[nRleLen], &pRow[opqPos], static_cast<size_t>(chunk));
				nRleLen += static_cast<size_t>(chunk);
				opqPos += chunk;
				opqCount -= chunk;
			}
		}
		pTempRle[nRleLen++] = 0x80;
	}

	D2GfxCellStrc* pCell = (D2GfxCellStrc*)D2_CALLOC(0x20 + nRleLen);
	pCell->bFlip = bBottomUp ? 1 : 0;
	pCell->dwWidth = static_cast<uint32_t>(nWidth);
	pCell->dwHeight = static_cast<uint32_t>(nHeight);
	pCell->nXOffset = nXOffset;
	pCell->nYOffset = nYOffset;
	pCell->dwLength = static_cast<uint32_t>(nRleLen);
	if (nRleLen > 0)
	{
		memcpy(pCell->pPixels, pTempRle, nRleLen);
	}
	D2_FREE(pTempRle);
	return pCell;
}

// 1.12a: 0x6FE1EA00 - Decode one DCC direction stream into nFramesPerDir D2GfxCellStrc cells
static bool DecodeDccDirection(
	const uint8_t* pDirData,
	uint32_t nDirMaxBytes,
	int32_t nFramesPerDir,
	D2GfxCellStrc** ppOutCells)
{
	if (!pDirData || nDirMaxBytes < 8 || nFramesPerDir <= 0 || nFramesPerDir > 512)
	{
		return false;
	}

	DccBitStream bsMain = { pDirData, nDirMaxBytes, 0 };
	const uint32_t dwOutSizeCoded = bsMain.ReadBits(32);
	D2_MAYBE_UNUSED(dwOutSizeCoded);
	const uint32_t nCompressionFlags = bsMain.ReadBits(2);
	const uint32_t nVar0Bits = kDccBitsTable[bsMain.ReadBits(4)];
	const uint32_t nWidthBits = kDccBitsTable[bsMain.ReadBits(4)];
	const uint32_t nHeightBits = kDccBitsTable[bsMain.ReadBits(4)];
	const uint32_t nXOffsetBits = kDccBitsTable[bsMain.ReadBits(4)];
	const uint32_t nYOffsetBits = kDccBitsTable[bsMain.ReadBits(4)];
	const uint32_t nOptionalBytesBits = kDccBitsTable[bsMain.ReadBits(4)];
	const uint32_t nCodedBytesBits = kDccBitsTable[bsMain.ReadBits(4)];

	DccFrameHeader* aFrames = (DccFrameHeader*)D2_CALLOC(sizeof(DccFrameHeader) * static_cast<size_t>(nFramesPerDir));
	int32_t dirXMin = 0x7FFFFFFF;
	int32_t dirYMin = 0x7FFFFFFF;
	int32_t dirXMax = -0x7FFFFFFF;
	int32_t dirYMax = -0x7FFFFFFF;
	uint32_t totalOptionalBytes = 0;

	for (int32_t f = 0; f < nFramesPerDir; ++f)
	{
		DccFrameHeader& fh = aFrames[f];
		fh.nVar0 = bsMain.ReadBits(nVar0Bits);
		fh.nWidth = static_cast<int32_t>(bsMain.ReadBits(nWidthBits));
		fh.nHeight = static_cast<int32_t>(bsMain.ReadBits(nHeightBits));
		fh.nXOffset = bsMain.ReadSigned(nXOffsetBits);
		fh.nYOffset = bsMain.ReadSigned(nYOffsetBits);
		fh.nOptionalBytes = bsMain.ReadBits(nOptionalBytesBits);
		fh.nCodedBytes = bsMain.ReadBits(nCodedBytesBits);
		fh.bBottomUp = bsMain.ReadBits(1);
		totalOptionalBytes += fh.nOptionalBytes;

		const int32_t hOff = fh.bBottomUp ? 0 : fh.nHeight;
		fh.nXMin = fh.nXOffset;
		fh.nXMax = fh.nXMin + fh.nWidth;
		fh.nYMin = fh.nYOffset - hOff;
		fh.nYMax = fh.nYMin + fh.nHeight;

		if (fh.nWidth > 0 && fh.nHeight > 0)
		{
			if (fh.nXMin < dirXMin) dirXMin = fh.nXMin;
			if (fh.nXMax > dirXMax) dirXMax = fh.nXMax;
			if (fh.nYMin < dirYMin) dirYMin = fh.nYMin;
			if (fh.nYMax > dirYMax) dirYMax = fh.nYMax;
		}
	}

	if (totalOptionalBytes > 0)
	{
		bsMain.nBitPos = (bsMain.nBitPos + 7u) & ~7u;
		bsMain.nBitPos += totalOptionalBytes * 8u;
	}

	const uint32_t nEqualCellsBits = (nCompressionFlags & 2u) ? bsMain.ReadBits(20) : 0;
	const uint32_t nPixelMaskBits = bsMain.ReadBits(20);
	const uint32_t nEncodingTypeBits = (nCompressionFlags & 1u) ? bsMain.ReadBits(20) : 0;
	const uint32_t nRawPixelCodesBits = (nCompressionFlags & 1u) ? bsMain.ReadBits(20) : 0;

	uint8_t aPixelValues[256] = {};
	int nColors = 0;
	for (int i = 0; i < 256; ++i)
	{
		if (bsMain.ReadBits(1))
		{
			aPixelValues[nColors++] = static_cast<uint8_t>(i);
		}
	}

	const uint32_t posEqualCells = bsMain.nBitPos;
	const uint32_t posPixelMask = posEqualCells + nEqualCellsBits;
	const uint32_t posEncodingType = posPixelMask + nPixelMaskBits;
	const uint32_t posRawPixelCodes = posEncodingType + nEncodingTypeBits;
	const uint32_t posPixelCodeDisp = posRawPixelCodes + nRawPixelCodesBits;

	const int32_t dirW = (dirXMax > dirXMin) ? (dirXMax - dirXMin) : 0;
	const int32_t dirH = (dirYMax > dirYMin) ? (dirYMax - dirYMin) : 0;
	if (dirW <= 0 || dirH <= 0)
	{
		for (int32_t f = 0; f < nFramesPerDir; ++f)
		{
			ppOutCells[f] = EncodeCanvasRectToGfxCell(nullptr, 0, 0, 0, 0, 0, aFrames[f].nXOffset, aFrames[f].nYOffset, aFrames[f].bBottomUp);
		}
		D2_FREE(aFrames);
		return true;
	}

	const int32_t nCellsW = ((dirW + 3) >> 2) + 1;
	const int32_t nCellsH = ((dirH + 3) >> 2) + 1;
	const int32_t nTotalDirCells = nCellsW * nCellsH;

	// Count upper bound on frame cell visits across all frames
	int32_t nMaxCellVisits = 0;
	for (int32_t f = 0; f < nFramesPerDir; ++f)
	{
		if (aFrames[f].nWidth > 0 && aFrames[f].nHeight > 0)
		{
			nMaxCellVisits += ((aFrames[f].nWidth >> 2) + 2) * ((aFrames[f].nHeight >> 2) + 2);
		}
	}
	if (nMaxCellVisits <= 0)
	{
		nMaxCellVisits = 1;
	}

	uint8_t* pPixelBuffer = (uint8_t*)D2_CALLOC(static_cast<size_t>(nMaxCellVisits) * 4u);
	DccDirCellState* aDirCells = (DccDirCellState*)D2_CALLOC(sizeof(DccDirCellState) * static_cast<size_t>(nTotalDirCells));

	// Stage 1 (0x6FE1DF50): Decode 4-color palettes for each frame cell visit
	DccBitStream bsEqualCells = { pDirData, nDirMaxBytes, posEqualCells };
	DccBitStream bsPixelMask = { pDirData, nDirMaxBytes, posPixelMask };
	DccBitStream bsEncodingType = { pDirData, nDirMaxBytes, posEncodingType };
	DccBitStream bsRawPixelCodes = { pDirData, nDirMaxBytes, posRawPixelCodes };
	DccBitStream bsPixelCodeDisp = { pDirData, nDirMaxBytes, posPixelCodeDisp };

	uint8_t* pPixWrite = pPixelBuffer;
	for (int32_t f = 0; f < nFramesPerDir; ++f)
	{
		const DccFrameHeader& fh = aFrames[f];
		if (fh.nWidth <= 0 || fh.nHeight <= 0)
		{
			continue;
		}
		const int32_t relX = fh.nXMin - dirXMin;
		const int32_t relY = fh.nYMin - dirYMin;
		const int32_t cellX0 = relX >> 2;
		const int32_t cellY0 = relY >> 2;
		const int32_t wFirst = 4 - (relX & 3);
		const int32_t hFirst = 4 - (relY & 3);

		int32_t curY = 0;
		int32_t rowCellY = cellY0;
		int32_t cellH = (hFirst + 1 < fh.nHeight) ? hFirst : fh.nHeight;
		while (curY < fh.nHeight)
		{
			int32_t curX = 0;
			int32_t colCellX = cellX0;
			int32_t cellW = (wFirst + 1 < fh.nWidth) ? wFirst : fh.nWidth;
			while (curX < fh.nWidth)
			{
				const int32_t cellIdx = rowCellY * nCellsW + colCellX;
				DccDirCellState& dcell = aDirCells[cellIdx];
				uint8_t* oldPix = dcell.pLastPix;
				uint32_t nMask = 0x0Fu;

				bool bSkipEqual = false;
				if (oldPix != nullptr)
				{
					if ((nCompressionFlags & 2u) && bsEqualCells.ReadBits(1) != 0)
					{
						bSkipEqual = true;
					}
					else
					{
						nMask = bsPixelMask.ReadBits(4);
					}
				}

				if (!bSkipEqual)
				{
					uint8_t* curPix = pPixWrite;
					pPixWrite += 4;

					const uint32_t bRawEncoding = (nMask != 0 && (nCompressionFlags & 1u)) ? bsEncodingType.ReadBits(1) : 0;
					uint8_t curCode = 0;
					int edi = 4;
					while (edi > 0)
					{
						--edi;
						if (nMask & (1u << edi))
						{
							uint8_t bl = 0;
							if (bRawEncoding)
							{
								bl = static_cast<uint8_t>(bsRawPixelCodes.ReadBits(8));
								curPix[edi] = bl;
							}
							else
							{
								bl = curCode;
								uint32_t disp = 0;
								do
								{
									disp = bsPixelCodeDisp.ReadBits(4);
									bl = static_cast<uint8_t>(bl + disp);
								} while (disp == 15u);
								curPix[edi] = bl;
							}

							if (bl == curCode)
							{
								// Early stop: shift already-decoded masked slots (ecx > edi) into lowest masked slots (eax)
								int ecx = edi;
								int eax = -1;
								for (;;)
								{
									do { ++ecx; } while (ecx < 4 && !(nMask & (1u << ecx)));
									if (ecx >= 4)
									{
										break;
									}
									do { ++eax; } while (!(nMask & (1u << eax)));
									curPix[eax] = curPix[ecx];
								}
								for (;;)
								{
									do { ++eax; } while (eax < 4 && !(nMask & (1u << eax)));
									if (eax >= 4)
									{
										break;
									}
									curPix[eax] = 0;
								}
								while (edi > 0)
								{
									--edi;
									if (!(nMask & (1u << edi)))
									{
										curPix[edi] = oldPix ? oldPix[edi] : 0;
									}
								}
								break;
							}
							curCode = bl;
						}
						else
						{
							curPix[edi] = oldPix ? oldPix[edi] : 0;
						}
					}
					dcell.pLastPix = curPix;
				}

				curX += cellW;
				++colCellX;
				cellW = (curX + 5 < fh.nWidth) ? 4 : (fh.nWidth - curX);
			}
			curY += cellH;
			++rowCellY;
			cellH = (curY + 5 < fh.nHeight) ? 4 : (fh.nHeight - curY);
		}
	}

	// Translate all decoded color codes in pPixelBuffer through aPixelValues (0x6FE1EF65)
	const size_t nTotalPixBytes = static_cast<size_t>(pPixWrite - pPixelBuffer);
	for (size_t i = 0; i < nTotalPixBytes; ++i)
	{
		pPixelBuffer[i] = aPixelValues[pPixelBuffer[i]];
	}

	// Stage 2 (0x6FE1DB60): Fill direction canvas cell-by-cell and RLE-encode each frame
	uint8_t* pDirCanvas = (uint8_t*)D2_CALLOC(static_cast<size_t>(dirW) * static_cast<size_t>(dirH));
	memset(aDirCells, 0, sizeof(DccDirCellState) * static_cast<size_t>(nTotalDirCells));
	bsEqualCells.nBitPos = posEqualCells;
	const uint8_t* pPixRead = pPixelBuffer;

	for (int32_t f = 0; f < nFramesPerDir; ++f)
	{
		const DccFrameHeader& fh = aFrames[f];
		if (fh.nWidth <= 0 || fh.nHeight <= 0)
		{
			ppOutCells[f] = EncodeCanvasRectToGfxCell(nullptr, 0, 0, 0, 0, 0, fh.nXOffset, fh.nYOffset, fh.bBottomUp);
			continue;
		}
		const int32_t relX = fh.nXMin - dirXMin;
		const int32_t relY = fh.nYMin - dirYMin;
		const int32_t cellX0 = relX >> 2;
		const int32_t cellY0 = relY >> 2;
		const int32_t wFirst = 4 - (relX & 3);
		const int32_t hFirst = 4 - (relY & 3);

		int32_t curY = 0;
		int32_t rowCellY = cellY0;
		int32_t cellH = (hFirst + 1 < fh.nHeight) ? hFirst : fh.nHeight;
		while (curY < fh.nHeight)
		{
			int32_t curX = 0;
			int32_t colCellX = cellX0;
			int32_t cellW = (wFirst + 1 < fh.nWidth) ? wFirst : fh.nWidth;
			while (curX < fh.nWidth)
			{
				const int32_t cellIdx = rowCellY * nCellsW + colCellX;
				DccDirCellState& dcell = aDirCells[cellIdx];
				const int32_t canvasX = relX + curX;
				const int32_t canvasY = relY + curY;

				bool bEqualCell = false;
				if (dcell.bVisited && (nCompressionFlags & 2u))
				{
					bEqualCell = (bsEqualCells.ReadBits(1) != 0);
				}

				if (bEqualCell)
				{
					if (cellW != dcell.nLastW || cellH != dcell.nLastH)
					{
						for (int32_t y = 0; y < cellH; ++y)
						{
							memset(&pDirCanvas[(canvasY + y) * dirW + canvasX], 0, static_cast<size_t>(cellW));
						}
					}
					else if (canvasX != dcell.nLastCanvasX || canvasY != dcell.nLastCanvasY)
					{
						for (int32_t y = 0; y < cellH; ++y)
						{
							memcpy(
								&pDirCanvas[(canvasY + y) * dirW + canvasX],
								&pDirCanvas[(dcell.nLastCanvasY + y) * dirW + dcell.nLastCanvasX],
								static_cast<size_t>(cellW));
						}
					}
				}
				else
				{
					const uint8_t* pix = pPixRead;
					pPixRead += 4;

					uint32_t nBits = 0;
					if (pix[1] != pix[0])
					{
						nBits = (pix[2] != pix[1]) ? 2u : 1u;
					}

					if (nBits == 0)
					{
						const uint8_t c0 = pix[0];
						for (int32_t y = 0; y < cellH; ++y)
						{
							memset(&pDirCanvas[(canvasY + y) * dirW + canvasX], c0, static_cast<size_t>(cellW));
						}
					}
					else
					{
						for (int32_t y = 0; y < cellH; ++y)
						{
							uint8_t* pRow = &pDirCanvas[(canvasY + y) * dirW + canvasX];
							for (int32_t x = 0; x < cellW; ++x)
							{
								const uint32_t idx = bsPixelCodeDisp.ReadBits(nBits);
								pRow[x] = pix[idx & 3u];
							}
						}
					}
				}

				dcell.bVisited = true;
				dcell.nLastW = cellW;
				dcell.nLastH = cellH;
				dcell.nLastCanvasX = canvasX;
				dcell.nLastCanvasY = canvasY;

				curX += cellW;
				++colCellX;
				cellW = (curX + 5 < fh.nWidth) ? 4 : (fh.nWidth - curX);
			}
			curY += cellH;
			++rowCellY;
			cellH = (curY + 5 < fh.nHeight) ? 4 : (fh.nHeight - curY);
		}

		ppOutCells[f] = EncodeCanvasRectToGfxCell(
			pDirCanvas,
			dirW,
			relX,
			relY,
			fh.nWidth,
			fh.nHeight,
			fh.nXOffset,
			fh.nYOffset,
			fh.bBottomUp);
	}

	D2_FREE(pDirCanvas);
	D2_FREE(aDirCells);
	D2_FREE(pPixelBuffer);
	D2_FREE(aFrames);
	return true;
}

static D2CellFileStrc* DecodeDccToCellFile(const uint8_t* pFileBytes, uint32_t nFileSize)
{
	if (!pFileBytes || nFileSize < 19 || pFileBytes[0] != 0x74)
	{
		return nullptr;
	}
	const int32_t nDirections = pFileBytes[2];
	const int32_t nFramesPerDir = *reinterpret_cast<const int32_t*>(&pFileBytes[3]);
	if (nDirections <= 0 || nDirections > 64 || nFramesPerDir <= 0 || nFramesPerDir > 512)
	{
		return nullptr;
	}
	if (nFileSize < static_cast<uint32_t>(15 + 4 * nDirections))
	{
		return nullptr;
	}

	const int32_t nTotalCells = nDirections * nFramesPerDir;
	D2CellFileStrc* pCellFile = (D2CellFileStrc*)D2_CALLOC(sizeof(D2CellFileStrc) + sizeof(D2GfxCellStrc*) * static_cast<size_t>(nTotalCells));
	pCellFile->dwVersion = 6;
	pCellFile->dwTermination = 0xB00B1E5;
	pCellFile->nDirections = nDirections;
	pCellFile->nFrames = nFramesPerDir;

	const uint32_t* aDirOffsets = reinterpret_cast<const uint32_t*>(&pFileBytes[15]);
	for (int32_t d = 0; d < nDirections; ++d)
	{
		const uint32_t dirOff = aDirOffsets[d];
		if (dirOff >= nFileSize)
		{
			D2CMP_CelFileFree(pCellFile);
			return nullptr;
		}
		if (!DecodeDccDirection(pFileBytes + dirOff, nFileSize - dirOff, nFramesPerDir, &pCellFile->pGfxCells[d * nFramesPerDir]))
		{
			D2CMP_CelFileFree(pCellFile);
			return nullptr;
		}
	}
	return pCellFile;
}

static bool BuildSpriteBasePath(const D2GfxDataStrc* pData, char* szOutBase, size_t nMaxLen)
{
	if (!pData || !szOutBase || nMaxLen == 0)
	{
		return false;
	}
	szOutBase[0] = '\0';

	if (pData->pName && pData->pName[0])
	{
		if (_strnicmp(pData->pName, "DATA\\", 5) == 0)
		{
			SStrCopy(szOutBase, pData->pName, static_cast<int>(nMaxLen));
		}
		else if (pData->nUnitType == 3)
		{
			snprintf(szOutBase, nMaxLen, "DATA\\GLOBAL\\MISSILES\\%s", pData->pName);
		}
		else if (pData->nUnitType == 4)
		{
			snprintf(szOutBase, nMaxLen, "DATA\\GLOBAL\\ITEMS\\%s", pData->pName);
		}
		else if (pData->nUnitType == 5)
		{
			snprintf(szOutBase, nMaxLen, "DATA\\GLOBAL\\OVERLAYS\\%s", pData->pName);
		}
		else
		{
			SStrCopy(szOutBase, pData->pName, static_cast<int>(nMaxLen));
		}
		const size_t nLen = strlen(szOutBase);
		if (nLen > 4 && (_stricmp(szOutBase + nLen - 4, ".dcc") == 0 || _stricmp(szOutBase + nLen - 4, ".dc6") == 0))
		{
			szOutBase[nLen - 4] = '\0';
		}
		return szOutBase[0] != '\0';
	}

	if (pData->dwName[0] == 0)
	{
		return false;
	}

	auto TrimToken = [](uint32_t dwVal, char (&szOut)[5])
	{
		const char* p = reinterpret_cast<const char*>(&dwVal);
		int k = 0;
		for (int i = 0; i < 4; ++i)
		{
			if (p[i] != '\0' && p[i] != ' ')
			{
				szOut[k++] = p[i];
			}
		}
		szOut[k] = '\0';
	};

	char szTok[5] = {};
	char szComp[5] = {};
	char szArmor[5] = {};
	char szMode[5] = {};
	char szHitClass[5] = {};
	TrimToken(pData->dwName[0], szTok);
	TrimToken(pData->dwName[1], szComp);
	TrimToken(pData->dwName[2], szArmor);
	TrimToken(pData->dwName[3], szMode);
	TrimToken(pData->dwName[4], szHitClass);

	if (!szTok[0] || !szComp[0] || !szMode[0])
	{
		return false;
	}
	if (!szArmor[0])
	{
		strcpy(szArmor, "lit");
	}
	if (!szHitClass[0])
	{
		strcpy(szHitClass, "hth");
	}

	const char* szBaseDir = "DATA\\GLOBAL\\CHARS";
	if (pData->nUnitType == 2)
	{
		szBaseDir = "DATA\\GLOBAL\\OBJECTS";
	}
	else if (pData->nUnitType == 1 || pData->nUnitIndex >= 7)
	{
		szBaseDir = "DATA\\GLOBAL\\MONSTERS";
	}

	snprintf(
		szOutBase,
		nMaxLen,
		"%s\\%s\\%s\\%s%s%s%s%s",
		szBaseDir,
		szTok,
		szComp,
		szTok,
		szComp,
		szArmor,
		szMode,
		szHitClass);
	return true;
}

static D2CellFileStrc* LookupOrLoadCompositeSprite(const D2GfxDataStrc* pData)
{
	char szBase[260] = {};
	if (!BuildSpriteBasePath(pData, szBase, sizeof(szBase)))
	{
		return nullptr;
	}

	uint32_t dwHash = 2166136261u;
	for (const char* p = szBase; *p; ++p)
	{
		const uint8_t c = static_cast<uint8_t>((*p >= 'a' && *p <= 'z') ? (*p - 32) : *p);
		dwHash = (dwHash ^ c) * 16777619u;
	}
	const uint32_t bucket = dwHash & 0xFFu;

	for (D2SpriteCacheEntryStrc* pEntry = gpSpriteCacheBuckets[bucket]; pEntry; pEntry = pEntry->pNext)
	{
		if (SStrCmpI(pEntry->szKey, szBase, sizeof(pEntry->szKey)) == 0)
		{
			return pEntry->bNotFound ? nullptr : pEntry->pCellFile;
		}
	}

	D2CellFileStrc* pLoadedCellFile = nullptr;
	bool bSeparateCells = false;

	char szFullPath[268] = {};
	snprintf(szFullPath, sizeof(szFullPath), "%s.dcc", szBase);
	HSFILE hFile = nullptr;
	if (FOG_FOpenFile(szFullPath, &hFile))
	{
		const uint32_t dwSize = FOG_FGetFileSize(hFile, nullptr);
		if (dwSize >= 19)
		{
			uint8_t* pBuf = (uint8_t*)D2_ALLOC(dwSize);
			int nRead = 0;
			FOG_FReadFile(hFile, pBuf, dwSize, &nRead, 0, 0, 0);
			FOG_FCloseFile(hFile);
			pLoadedCellFile = DecodeDccToCellFile(pBuf, dwSize);
			D2_FREE(pBuf);
			bSeparateCells = (pLoadedCellFile != nullptr);
		}
		else
		{
			FOG_FCloseFile(hFile);
		}
	}

	if (!pLoadedCellFile)
	{
		snprintf(szFullPath, sizeof(szFullPath), "%s.dc6", szBase);
		if (FOG_FOpenFile(szFullPath, &hFile))
		{
			const uint32_t dwSize = FOG_FGetFileSize(hFile, nullptr);
			if (dwSize >= sizeof(D2CellFileStrc))
			{
				D2CellFileStrc* pRaw = (D2CellFileStrc*)D2_ALLOC(dwSize);
				int nRead = 0;
				FOG_FReadFile(hFile, pRaw, dwSize, &nRead, 0, 0, 0);
				FOG_FCloseFile(hFile);
				D2CMP_CelFileNormalize(pRaw, &pLoadedCellFile, __FILE__, __LINE__, -1, 0);
				bSeparateCells = false;
			}
			else
			{
				FOG_FCloseFile(hFile);
			}
		}
	}

	D2SpriteCacheEntryStrc* pNewEntry = D2_CALLOC_STRC(D2SpriteCacheEntryStrc);
	SStrCopy(pNewEntry->szKey, szBase, sizeof(pNewEntry->szKey));
	pNewEntry->pCellFile = pLoadedCellFile;
	pNewEntry->bSeparateCells = bSeparateCells;
	pNewEntry->bNotFound = (pLoadedCellFile == nullptr);
	pNewEntry->pNext = gpSpriteCacheBuckets[bucket];
	gpSpriteCacheBuckets[bucket] = pNewEntry;

	return pLoadedCellFile;
}

// 1.12a: 0x6FE22710 (#10098, 1.10f #10053)
D2CMP_DLL_DECL void __stdcall D2CMP_FlushSpriteCache(BOOL bRealloc)
{
	D2_MAYBE_UNUSED(bRealloc);
	D2CMP_SpriteFreeAsyncLoads();
	for (int i = 0; i < 256; ++i)
	{
		D2SpriteCacheEntryStrc* pEntry = gpSpriteCacheBuckets[i];
		gpSpriteCacheBuckets[i] = nullptr;
		while (pEntry)
		{
			D2SpriteCacheEntryStrc* pNext = pEntry->pNext;
			if (pEntry->pCellFile)
			{
				if (pEntry->bSeparateCells)
				{
					D2CMP_CelFileFree(pEntry->pCellFile);
				}
				else
				{
					D2CMP_CelFileFreeSerialized(pEntry->pCellFile);
				}
			}
			D2_FREE(pEntry);
			pEntry = pNext;
		}
	}
	gdwHeaderCacheUsed_6FE3AB08 = 0;
	gdwFrameCacheUsed_6FE3AB14 = 0;
}

// 1.12a: 0x6FE22790 (#10061)
D2CMP_DLL_DECL void __stdcall D2CMP_SpriteCacheBuildFilename(char* szDest, const char* szSrc, size_t nMaxLen, BOOL bAllowCompressed)
{
	if (!szDest || !szSrc || nMaxLen == 0)
	{
		return;
	}
	snprintf(szDest, nMaxLen, "%s%s", szSrc, D2CMP_GetGfxFileExtension(bAllowCompressed));
}

// 1.12a: 0x6FE229E0 (#10003, 1.10f #10052)
D2CMP_DLL_DECL void __stdcall D2CMP_InitSpriteCache(void* pMemPool, int dwSpriteCacheSize, int dwSize, unsigned int dwMemoryOverride)
{
	gpSpriteCacheMemPool_6FE3AB00 = pMemPool;
	uint32_t totalSize = dwMemoryOverride ? dwMemoryOverride : (uint32_t)dwSpriteCacheSize;
	gdwHeaderCacheSize_6FE3AB04 = (uint32_t)dwSize;
	gdwFrameCacheSize_6FE3AB10 = totalSize;
	gdwHeaderCacheUsed_6FE3AB08 = 0;
	gdwFrameCacheUsed_6FE3AB14 = 0;
}

// 1.12a: 0x6FE231C0 (#10030)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_SpriteCacheLoadInit(D2GfxDataStrc* pData)
{
	if (!pData)
	{
		return FALSE;
	}
	D2CellFileStrc* pCellFile = pData->pCellFile;
	if (!pCellFile)
	{
		pCellFile = LookupOrLoadCompositeSprite(pData);
	}
	if (pCellFile)
	{
		pData->nMaxDirections = pCellFile->nDirections;
		pData->nMaxFrames = pCellFile->nFrames;
		return TRUE;
	}
	return FALSE;
}

// 1.12a: 0x6FE23490 (#10020, 1.10f #10020)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_SpriteValidate(D2GfxDataStrc* pData, int32_t nArg2, BOOL bLoad)
{
	D2_MAYBE_UNUSED(nArg2);
	D2_MAYBE_UNUSED(bLoad);
	if (!pData)
	{
		return FALSE;
	}
	D2CellFileStrc* pCellFile = pData->pCellFile;
	if (!pCellFile)
	{
		pCellFile = LookupOrLoadCompositeSprite(pData);
		if (!pCellFile)
		{
			pData->pCurrentCell = nullptr;
			return FALSE;
		}
	}
	pData->nMaxDirections = pCellFile->nDirections;
	pData->nMaxFrames = pCellFile->nFrames;
	int32_t nFileDir = D2CMP_CelGameDirToFileDir((int32_t)pData->nDirection, pCellFile->nDirections);
	int32_t nFrame = (pCellFile->nFrames > 0) ? ((int32_t)pData->nFrame % pCellFile->nFrames) : 0;
	int32_t idx = nFileDir * pCellFile->nFrames + nFrame;
	if (idx >= 0 && idx < pCellFile->nDirections * pCellFile->nFrames)
	{
		pData->pCurrentCell = pCellFile->pGfxCells[idx];
		return pData->pCurrentCell != nullptr;
	}
	return FALSE;
}

// ============================================================================
// FastCmp.cpp Exports (6 functions)
// ============================================================================

// 1.12a: 0x6FE243A0 (#10048)
D2CMP_DLL_DECL void __stdcall D2CMP_CelToggleDebugDrawMode()
{
	gbDebugDrawMode_6FE3AB2C = !gbDebugDrawMode_6FE3AB2C;
}

// 1.12a: 0x6FE25B60 (#10025, 1.10f #10025) - Magnifying lens distortion effect
D2CMP_DLL_DECL void __stdcall D2CMP_CmpDoLensEffect(uint8_t* pDest, int32_t nPitch)
{
	if (!pDest || nPitch <= 0)
	{
		return;
	}
	// The 1.12a lens routine copies/magnifies a circular region in-place with pitch stride
	const int32_t nRadius = 32;
	for (int32_t y = -nRadius; y < nRadius; ++y)
	{
		for (int32_t x = -nRadius; x < nRadius; ++x)
		{
			if (x * x + y * y <= nRadius * nRadius)
			{
				int32_t srcX = x >> 1;
				int32_t srcY = y >> 1;
				pDest[(y + nRadius) * nPitch + (x + nRadius)] = pDest[(srcY + nRadius) * nPitch + (srcX + nRadius)];
			}
		}
	}
}

// 1.12a: 0x6FE26B20 (#10016)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_FastCmpNoop1()
{
	return 0;
}

// 1.12a: 0x6FE26B30 (#10065)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_FastCmpNoop2(int32_t a1, int32_t a2)
{
	D2_MAYBE_UNUSED(a1);
	D2_MAYBE_UNUSED(a2);
	return 0;
}

// 1.12a: 0x6FE26B40 (#10087)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_FastCmpNoop3()
{
	return 0;
}

// 1.12a: 0x6FE26B50 (#10002)
D2CMP_DLL_DECL int32_t __stdcall D2CMP_FastCmpNoop4()
{
	return 0;
}

// ============================================================================
// TileProjects.cpp Exports (8 functions)
// ============================================================================

// 1.12a: 0x6FE26B60 (#10039)
D2CMP_DLL_DECL void __stdcall D2CMP_TileCacheResetStats()
{
	gnTileCacheHits_6FE3AB40 = 0;
	gnTileCacheMisses_6FE3AB44 = 0;
}

// 1.12a: 0x6FE26B70 (#10104)
D2CMP_DLL_DECL void __stdcall D2CMP_TileCacheGetStats(int32_t* pMaxSize, int32_t* pMisses, int32_t* pHits, int32_t* pHitRate)
{
	if (pMaxSize) *pMaxSize = gnTileCacheMaxSize_6FE3AB38;
	if (pMisses) *pMisses = gnTileCacheMisses_6FE3AB44;
	if (pHits) *pHits = gnTileCacheHits_6FE3AB40;
	if (pHitRate)
	{
		int32_t total = gnTileCacheHits_6FE3AB40 + gnTileCacheMisses_6FE3AB44;
		*pHitRate = (total > 0) ? ((gnTileCacheHits_6FE3AB40 * 100) / total) : 0;
	}
}

// 1.12a: 0x6FE26CB0 (#10015)
D2CMP_DLL_DECL void __stdcall D2CMP_InitTileCache(void* pMemPool, int32_t nCacheSize)
{
	gpTileCacheMemPool_6FE3AB34 = pMemPool;
	gnTileCacheMaxSize_6FE3AB38 = nCacheSize;
	gnTileCacheUsedSize_6FE3AB3C = 0;
	gnTileCacheHits_6FE3AB40 = 0;
	gnTileCacheMisses_6FE3AB44 = 0;
}

// 1.12a: 0x6FE26D60 (#10032, 1.10f #10102)
D2CMP_DLL_DECL void __stdcall D2CMP_TileFreeAsyncLoads()
{
}

// 1.12a: 0x6FE26D90 (#10026, 1.10f #10026)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_TileValidate(D2TileLibraryEntryStrc* pTile, BOOL bLoad, BOOL bAsync)
{
	D2_MAYBE_UNUSED(bLoad);
	D2_MAYBE_UNUSED(bAsync);
	if (!pTile)
	{
		return FALSE;
	}
	if (pTile->pBlock != nullptr)
	{
		++gnTileCacheHits_6FE3AB40;
		return TRUE;
	}
	++gnTileCacheMisses_6FE3AB44;
	return FALSE;
}

// 1.12a: 0x6FE26E70 (#10064)
D2CMP_DLL_DECL void __stdcall D2CMP_DestroyTileCache(BOOL bFreeAll)
{
	if (bFreeAll)
	{
		D2CMP_TileProjectsFreeAll();
	}
	gnTileCacheUsedSize_6FE3AB3C = 0;
}

// 1.12a: 0x6FE27020 (#10071)
D2CMP_DLL_DECL BOOL __stdcall D2CMP_TileProjectsLoadLibrary(D2TileLibraryHashStrc** ppTileLibraryHash, const char* szFilename)
{
	if (!ppTileLibraryHash || !szFilename)
	{
		return FALSE;
	}

	// Check if already loaded in gpTileRecordList_6FE3AB48
	for (D2TileRecordStrc* pRec = gpTileRecordList_6FE3AB48; pRec; pRec = pRec->pPrev)
	{
		if (SStrCmpI(pRec->szLibraryName, szFilename, 260) == 0)
		{
			*ppTileLibraryHash = pRec->pHashBlock;
			return TRUE;
		}
	}

	HSFILE hFile = nullptr;
	if (!FOG_FOpenFile(szFilename, &hFile))
	{
		return FALSE;
	}
	uint32_t dwSize = FOG_FGetFileSize(hFile, nullptr);
	if (dwSize < sizeof(D2TileLibraryHeaderStrc))
	{
		FOG_FCloseFile(hFile);
		return FALSE;
	}

	D2TileLibraryHeaderStrc* pRawHeader = (D2TileLibraryHeaderStrc*)D2_ALLOC(dwSize);
	int nRead = 0;
	FOG_FReadFile(hFile, pRawHeader, dwSize, &nRead, 0, 0, 0);
	FOG_FCloseFile(hFile);

	D2TileLibraryHeaderStrc* pNormHeader = nullptr;
	D2CMP_TileProjectNormalize(pRawHeader, &pNormHeader, szFilename);

	D2TileLibraryHashStrc* pHash = D2_CALLOC_STRC(D2TileLibraryHashStrc);
	D2TileLibraryEntryStrc* pTiles = (D2TileLibraryEntryStrc*)(uintptr_t)pNormHeader->dwTileStart_pFirst;

	for (int32_t i = 0; i < pNormHeader->nTiles; ++i)
	{
		D2TileLibraryEntryStrc* pTile = &pTiles[i];
		int32_t hashIdx = ((pTile->nStyle << 5) + (pTile->nSequence << 2) + pTile->nType) & 0x7F;
		D2TileLibraryHashNodeStrc* pNode = pHash->pNodes[hashIdx];
		while (pNode && (pNode->nType != pTile->nType || pNode->nStyle != pTile->nStyle || pNode->nSequence != pTile->nSequence))
		{
			pNode = pNode->pPrev;
		}
		if (!pNode)
		{
			pNode = D2_CALLOC_STRC(D2TileLibraryHashNodeStrc);
			pNode->nType = pTile->nType;
			pNode->nStyle = pTile->nStyle;
			pNode->nSequence = pTile->nSequence;
			pNode->pPrev = pHash->pNodes[hashIdx];
			pHash->pNodes[hashIdx] = pNode;
		}
		D2TileLibraryHashRefStrc* pRef = D2_CALLOC_STRC(D2TileLibraryHashRefStrc);
		pRef->pTile = pTile;
		pRef->pPrev = pNode->pRef;
		pNode->pRef = pRef;
	}

	D2TileRecordStrc* pRec = D2_CALLOC_STRC(D2TileRecordStrc);
	SStrCopy(pRec->szLibraryName, szFilename, sizeof(pRec->szLibraryName));
	pRec->pLibrary = pNormHeader;
	pRec->pHashBlock = pHash;
	pRec->pPrev = gpTileRecordList_6FE3AB48;
	gpTileRecordList_6FE3AB48 = pRec;

	*ppTileLibraryHash = pHash;
	return TRUE;
}

// 1.12a: 0x6FE27180 (#10051)
D2CMP_DLL_DECL void __stdcall D2CMP_TileProjectsFreeAll()
{
	D2TileRecordStrc* pRec = gpTileRecordList_6FE3AB48;
	while (pRec)
	{
		D2TileRecordStrc* pPrevRec = pRec->pPrev;
		if (pRec->pHashBlock)
		{
			for (int i = 0; i < 128; ++i)
			{
				D2TileLibraryHashNodeStrc* pNode = pRec->pHashBlock->pNodes[i];
				while (pNode)
				{
					D2TileLibraryHashNodeStrc* pPrevNode = pNode->pPrev;
					D2TileLibraryHashRefStrc* pRef = pNode->pRef;
					while (pRef)
					{
						D2TileLibraryHashRefStrc* pPrevRef = pRef->pPrev;
						D2_FREE(pRef);
						pRef = pPrevRef;
					}
					D2_FREE(pNode);
					pNode = pPrevNode;
				}
			}
			D2_FREE(pRec->pHashBlock);
		}
		if (pRec->pLibrary)
		{
			D2_FREE(pRec->pLibrary);
		}
		D2_FREE(pRec);
		pRec = pPrevRec;
	}
	gpTileRecordList_6FE3AB48 = nullptr;
}
