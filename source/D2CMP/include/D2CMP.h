#pragma once

#include <D2Dll.h>
#include <D2BasicTypes.h>

//1.10f Image base: 0x6FDF0000
//1.12a Image base: 0x6FE10000

#ifdef D2_VERSION_110F
constexpr int D2CMPImageBase = 0x6FDF0000;
#else
constexpr int D2CMPImageBase = 0x6FE10000;
#endif

#pragma pack(1)

enum D2TileMaterialFlags : uint16_t 
{
	TILE_FLAGS_OTHER = 0x1,
	TILE_FLAGS_WATER = 0x2,
	TILE_FLAGS_WOOD_OBJ = 0x4, // Blocks reverberation
	TILE_FLAGS_ISTONE = 0x8,   // Indoor stone
	TILE_FLAGS_OSTONE = 0x10,  // Outdoor stone
	TILE_FLAGS_DIRT = 0x20,
	TILE_FLAGS_SAND = 0x40,
	TILE_FLAGS_WOOD = 0x80,
	TILE_FLAGS_LAVA = 0x100, // Special case: Always bright + animated
	TILE_FLAGS_SNOW = 0x400,
};

enum D2TileType
{
	TILETYPE_FLOOR = 0,                 // automap code: fl
	TILETYPE_WALL_LEFT = 1,             // automap code: wl
	TILETYPE_WALL_RIGHT = 2,            // automap code: wr
	TILETYPE_WALL_TOP_CORNER_RIGHT = 3, // automap code: wtlr
	TILETYPE_WALL_TOP_CORNER_LEFT = 4,  // automap code: wtll
	TILETYPE_WALL_TOP_RIGHT = 5,        // automap code: wtr
	TILETYPE_WALL_BOTTOM_LEFT = 6,      // automap code: wbl
	TILETYPE_WALL_BOTTOM_RIGHT = 7,     // automap code: wbr
	TILETYPE_WALL_LEFT_DOOR = 8,        // automap code: wld
	TILETYPE_WALL_RIGHT_DOOR = 9,       // automap code: wrd
	TILETYPE_WALL_LEFT_EXIT = 10,       // automap code: wle
	TILETYPE_WALL_RIGHT_EXIT = 11,      // automap code: wre
	TILETYPE_COLUMN = 12,               // automap code: co
	TILETYPE_SHADOW = 13,               // automap code: sh
	TILETYPE_TREE = 14,                 // automap code: tr
	TILETYPE_ROOF = 15,                 // automap code: rf
	TILETYPE_LEFT_WALL_DOWN = 16,       // automap code: ld
	TILETYPE_RIGHT_WALL_DOWN = 17,      // automap code: rd
	TILETYPE_FULL_WALL_DOWN = 18,       // automap code: fd
	TILETYPE_FRONT_WALL_DOWN = 19,      // automap code: fi
};

struct D2TileRecordStrc;

struct D2TileLibraryBlockStrc
{
	int16_t nPosX;							//0x00
	int16_t nPosY;							//0x02
	int16_t unk0x04;							//0x04
	uint8_t nGridX;							//0x06
	uint8_t nGridY;							//0x07
	uint16_t nFormat;							//0x08
	int32_t nLength;							//0x0A
	int16_t unk0x0E;							//0x0E
	uint8_t* dwOffset_pData;					//0x10
};

struct D2TileLibraryEntryStrc
{
	int32_t nLightDirection;					//0x00
	uint16_t nRoofHeight;						//0x04
	uint16_t nFlags;							//0x06 D2TileMaterialFlags
	int32_t nTotalHeight;						//0x08
	int32_t nWidth;								//0x0C
	int32_t nHeightToBottom;					//0x10
	int32_t nType;								//0x14 aka nOrientation
	int32_t nStyle;								//0x18 aka nIndex
	int32_t nSequence;							//0x1C aka nSubIndex
	int32_t nRarity_Frame;						//0x20 By default this is the rarity of the tile, for animated tiles this is the frame index.
	int32_t transparentColorRGB24;				//0x24
	uint8_t dwTileFlags[32];					//0x28 Collision info (25 subtile bytes + 7 padding)
	int32_t dwBlockOffset_pBlock;				//0x48
	int32_t nBlockSize;							//0x4C
	int32_t nBlocks;							//0x50
	D2TileLibraryBlockStrc* pBlock;				//0x54
	const char* szTileName;						//0x58
	void* pLRUCacheBlock;						//0x5C
};

struct D2TileStrc
{
	D2TileStrc* pNext;						//0x00
	uint16_t unk0x04;							//0x04
	uint16_t nFlags;							//0x06
	int32_t unk0x08;							//0x08
	int32_t nTile;								//0x0C
	int32_t unk0x10;							//0x10
	int32_t nAct;								//0x14
	int32_t nLevel;								//0x18
	int32_t nTileType;							//0x1C
	uint32_t unk0x20[7];						//0x20
	D2TileRecordStrc* pParent;				//0x3C
	uint32_t unk0x40[33];						//0x40
};

struct D2TileLibraryHashRefStrc
{
	D2TileLibraryEntryStrc* pTile;			//0x00
	D2TileLibraryHashRefStrc* pPrev;		//0x04
};

struct D2TileLibraryHashNodeStrc
{
	int32_t nStyle;							//0x00 aka nIndex;								
	int32_t nSequence;						//0x04 aka nSequence;							
	int32_t nType;							//0x08 aka nOrientation;						
	D2TileLibraryHashRefStrc* pRef;			//0x0C
	D2TileLibraryHashNodeStrc* pPrev;		//0x10
};

struct D2TileLibraryHashStrc
{
	D2TileLibraryHashNodeStrc* pNodes[128];	//0x00
};

struct D2TileLibraryHeaderStrc
{
	int32_t dwVersion;							//0x00
	int32_t dwFlags;							//0x04
	char szLibraryName[260];				//0x08
	int32_t nTiles;								//0x10C
	int32_t dwTileStart_pFirst;					//0x110
};

struct D2TileRecordStrc
{
	char szLibraryName[260];				//0x00
	void* pLibrary;							//0x104
	D2TileLibraryHashStrc* pHashBlock;		//0x108
	D2TileRecordStrc* pPrev;				//0x10C
};


struct D2CellFileStrc;
struct D2GfxCellStrc;
struct D2GfxDataStrc;

#pragma pack()

// Helper function
inline bool TileTypeIsAWallWithDoor(int32_t nTileType)
{
	return nTileType == TILETYPE_WALL_RIGHT_DOOR || nTileType == TILETYPE_WALL_LEFT_DOOR;
}

// Existing 30 exports (caller-compatible names and signatures)
D2FUNC_DLL(D2CMP, GetNearestPaletteIndex, int, __stdcall, (PALETTEENTRY* pPalette, int nPaletteSize, int nRed, int nGreen, int nBlue), 0xAB20)									//D2Cmp.#10004 (1.12a #10067)
D2FUNC_DLL(D2CMP, CelDraw, void, __stdcall, (D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, int32_t nCropLeft, uint8_t* pSurfaceBits, int32_t nSurfaceHeight, int32_t nCropRight, int32_t nSurfaceWidth, uint8_t* pDarkenTable, int32_t nDrawMode, void* pPaletteTable), 0x0)	//D2Cmp.#10012 (1.12a #10053)
D2FUNC_DLL(D2CMP, CelDrawEx, void, __stdcall, (D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, int32_t nCropLeft, int32_t nCropRight, uint8_t* pSurfaceBits, int32_t nSurfaceHeight, int32_t nSurfaceWidth, int32_t nUnused, int32_t nDrawMode, int32_t nDrawLines, int32_t nSkipLines, int32_t nUnused2), 0x0)	//D2Cmp.#10013 (1.12a #10093)
D2FUNC_DLL(D2CMP, CelDrawShadow, void, __stdcall, (D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, int32_t nCropLeft, int32_t nCropRight, uint8_t* pSurfaceBits, int32_t nSurfaceHeight, int32_t nSurfaceWidth, uint8_t* pShadowTable, int32_t nUnused), 0x0)	//D2Cmp.#10014 (1.12a #10079)
D2FUNC_DLL(D2CMP, SpriteValidate, BOOL, __stdcall, (D2GfxDataStrc* pData, int32_t nArg2, BOOL bLoad), 0x0)																			//D2Cmp.#10020 (1.12a #10020)
D2FUNC_DLL(D2CMP, CelFileNormalize, void, __stdcall, (D2CellFileStrc* pFile, D2CellFileStrc** ppOutFile, const char* szFile, int nLine, int nSpecVersion, int nUnused), 0x1EA0)		//D2Cmp.#10024 (1.12a #10006)
D2FUNC_DLL(D2CMP, CmpDoLensEffect, void, __stdcall, (uint8_t* pDest, int32_t nPitch), 0x0)																							//D2Cmp.#10025 (1.12a #10025)
D2FUNC_DLL(D2CMP, TileValidate, BOOL, __stdcall, (D2TileLibraryEntryStrc* pTile, BOOL bLoad, BOOL bAsync), 0x0)																		//D2Cmp.#10026 (1.12a #10026)
D2FUNC_DLL(D2CMP, CelFileFreeHardware, BOOL, __stdcall, (D2CellFileStrc* pFile), 0x2750)																				//D2Cmp.#10032 (1.12a #10106)
D2FUNC_DLL(D2CMP, CelGetHandle, D2CellFileStrc*, __stdcall, (D2GfxDataStrc* pData), 0x2540)																					//D2Cmp.#10036 (1.12a #10038)
D2FUNC_DLL(D2CMP, CelGetWidth, int, __stdcall, (D2CellFileStrc* pCelFile), 0x25E0)																						//D2Cmp.#10037 (1.12a #10031)
D2FUNC_DLL(D2CMP, CelGetHeight, int, __stdcall, (D2CellFileStrc* pCelFile), 0x2610)																						//D2Cmp.#10038 (1.12a #10096)
D2FUNC_DLL(D2CMP, CelGetOffsetX, int, __stdcall, (D2CellFileStrc* pCelFile), 0x2640)																					//D2Cmp.#10039 (1.12a #10008)
D2FUNC_DLL(D2CMP, CelGetOffsetY, int, __stdcall, (D2CellFileStrc* pCelFile), 0x2670)																					//D2Cmp.#10040 (1.12a #10043)
D2FUNC_DLL(D2CMP, CelFileGetCelsPerDirection, int, __stdcall, (D2CellFileStrc* pCelFile), 0x2700)																		//D2Cmp.#10046 (1.12a #10018)
D2FUNC_DLL(D2CMP, GetGfxFileExtension, const char*, __stdcall, (BOOL bAllowCompressed), 0xB930)																			//D2Cmp.#10051 (1.12a #10057)
D2FUNC_DLL(D2CMP, InitSpriteCache, void, __stdcall, (void* pMemPool, int dwSpriteCacheSize, int dwSize, unsigned int dwMemoryOverride), 0xB9F0)							//D2Cmp.#10052 (1.12a #10003)
D2FUNC_DLL(D2CMP, FlushSpriteCache, void, __stdcall, (BOOL bRealloc), 0xBBF0)																							//D2Cmp.#10053 (1.12a #10098)
D2FUNC_DLL(D2CMP, SetCompressedDataMode, void, __stdcall, (BOOL bAllowCompressedMode), 0xB9C0)																			//D2Cmp.#10054 (1.12a #10009)
D2FUNC_DLL(D2CMP, 10077_GetTileType, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFFF0)																//D2Cmp.#10077 (1.12a #10000)
D2FUNC_DLL(D2CMP, 10078_GetTileStyle, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFF30)																//D2Cmp.#10078 (1.12a #10005)
// Material flags
D2FUNC_DLL(D2CMP, 10079_GetTileFlags, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFF60)																//D2Cmp.#10079 (1.12a #10010)
D2FUNC_DLL(D2CMP, 10081_GetTileRarity, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFFC0)																//D2Cmp.#10081 (1.12a #10102)
D2FUNC_DLL(D2CMP, 10082_GetTileSequence, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFFF0)															//D2Cmp.#10082 (1.12a #10084)
D2FUNC_DLL(D2CMP, 10085_GetTileFlagArray, uint8_t*, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0x10080)													//D2Cmp.#10085 (1.12a #10060)
D2FUNC_DLL(D2CMP, 10087_LoadTileLibrarySlot, void, __stdcall, (D2TileLibraryHashStrc** ppTileLibraryHash, const char* szFileName), 0xFDE0)								//D2Cmp.#10087 (1.12a #10024)
D2FUNC_DLL(D2CMP, 10088_GetTiles, int, __stdcall, (D2TileLibraryHashStrc** ppTileLibraryHash, int nType, int nStyle, int nSequence, D2TileLibraryEntryStrc** pTileList, int nTileListSize), 0xFE70)//D2Cmp.#10088 (1.12a #10023)
D2FUNC_DLL(D2CMP, MixPalette, uint8_t*, __stdcall, (uint8_t nTrans, int nColor), 0xB760)																				//D2Cmp.#10098 (1.12a #10059)
D2FUNC_DLL(D2CMP, SpriteFreeAsyncLoads, void, __stdcall, (), 0xE000)																									//D2Cmp.#10099 (1.12a #10082)
D2FUNC_DLL(D2CMP, TileFreeAsyncLoads, void, __stdcall, (), 0xE860)																										//D2Cmp.#10102 (1.12a #10032)

// Additional 77 exports reconstructed from 1.12a D2CMP.dll
// CelCmp.cpp
D2FUNC_DLL(D2CMP, CelGetHardwareData, void*, __stdcall, (D2CellFileStrc* pCelFile), 0x95D0)																				//1.12a #10049
D2FUNC_DLL(D2CMP, CelGetHardwareFreeCallback, void*, __stdcall, (), 0x96D0)																								//1.12a #10052
D2FUNC_DLL(D2CMP, CelSetHardwareFreeCallback, void, __stdcall, (void (__fastcall* pfnCallback)(void*)), 0x96E0)															//1.12a #10017
D2FUNC_DLL(D2CMP, CelFileGetNumDirections, int32_t, __stdcall, (D2CellFileStrc* pCelFile), 0x9720)																		//1.12a #10035
D2FUNC_DLL(D2CMP, CelGetControlData, int32_t, __stdcall, (D2CellFileStrc* pCelFile), 0x9750)																			//1.12a #10021
D2FUNC_DLL(D2CMP, CelFileTransform, BOOL, __stdcall, (D2CellFileStrc* pCelFile, const uint8_t* pTransform), 0x9AB0)														//1.12a #10046
D2FUNC_DLL(D2CMP, CelFileFree, BOOL, __stdcall, (D2CellFileStrc* pCelFile), 0x9B20)																						//1.12a #10085
D2FUNC_DLL(D2CMP, CelFileFreeSerialized, BOOL, __stdcall, (D2CellFileStrc* pCelFile), 0x9B70)																			//1.12a #10099
D2FUNC_DLL(D2CMP, CelFileSerialize, void, __stdcall, (D2CellFileStrc* pCelFile, D2CellFileStrc** ppOutFile, int32_t* pOutSize), 0x9C80)									//1.12a #10047
D2FUNC_DLL(D2CMP, CelFileConvertOldToNew, BOOL, __stdcall, (void* pOldFile, D2CellFileStrc** ppOutFile, int32_t* pOutSize), 0x9DF0)										//1.12a #10062
D2FUNC_DLL(D2CMP, CelFileConvert24To8, BOOL, __stdcall, (D2CellFileStrc* pCelFile, D2CellFileStrc** ppOutFile, void* pfnConvert, int32_t dwTransColor), 0x9FD0)			//1.12a #10074
D2FUNC_DLL(D2CMP, CelFileHalve, BOOL, __stdcall, (D2CellFileStrc* pCelFile, D2CellFileStrc** ppOutFile), 0xA070)														//1.12a #10088
D2FUNC_DLL(D2CMP, CelGameDirToShadowGameDir, int32_t, __stdcall, (int32_t nGameDir, int32_t nNumDirs), 0xA0A0)															//1.12a #10078
D2FUNC_DLL(D2CMP, CelGameDirToNormalDir, int32_t, __stdcall, (int32_t nGameDir, int32_t nNumDirs), 0xA190)																//1.12a #10037
D2FUNC_DLL(D2CMP, CelConvertBlock, int32_t, __stdcall, (int32_t nSrcDirs, int32_t nDstDirs), 0xA280)																	//1.12a #10097
D2FUNC_DLL(D2CMP, CelConvertDir, int32_t, __stdcall, (int32_t nDir, int32_t nNumDirs, int32_t nSubDirs, BOOL bFlag), 0xA2B0)											//1.12a #10041
D2FUNC_DLL(D2CMP, CelGameDirToFileDir, int32_t, __stdcall, (int32_t nGameDir, int32_t nNumDirs), 0xA330)																//1.12a #10013
D2FUNC_DLL(D2CMP, CelNormalDirToFileDir, int32_t, __stdcall, (int32_t nNormalDir, int32_t nNumDirs), 0xA370)															//1.12a #10081
D2FUNC_DLL(D2CMP, CelGetRawData, BOOL, __stdcall, (D2CellFileStrc* pCelFile, int32_t nDir, int32_t nCell, void* pOutBuffer), 0xA470)									//1.12a #10068
D2FUNC_DLL(D2CMP, CelFileIteratePixels, BOOL, __stdcall, (D2CellFileStrc* pCelFile, void (__stdcall* pfnCallback)(int32_t)), 0xA4F0)									//1.12a #10075
D2FUNC_DLL(D2CMP, CelCompressCreateCelFile, BOOL, __stdcall, (int32_t hCompress, D2CellFileStrc** ppOutCelFile), 0xA570)												//1.12a #10095
D2FUNC_DLL(D2CMP, CelCompressDestroy, BOOL, __stdcall, (int32_t hCompress), 0xA660)																						//1.12a #10050
D2FUNC_DLL(D2CMP, CelCompressAddImage, BOOL, __stdcall, (int32_t hCompress, int32_t nDirection, int32_t nWidth, int32_t nHeight, int32_t nOffsetX, int32_t nOffsetY, int32_t nControlData, const uint8_t* pPixels), 0xA780)	//1.12a #10105
D2FUNC_DLL(D2CMP, CelCompressCreate, int32_t, __stdcall, (int32_t nType, int32_t nNumDirs, const int32_t* pTransColor, BOOL bIs24Bit), 0xA9A0)							//1.12a #10036

// Palette.cpp
D2FUNC_DLL(D2CMP, PaletteSetMask, void, __stdcall, (const uint8_t* pMask), 0xAAA0)																						//1.12a #10045
D2FUNC_DLL(D2CMP, PaletteFixBlack, void, __stdcall, (BOOL bFixBlack), 0xAAD0)																							//1.12a #10086
D2FUNC_DLL(D2CMP, Palette24BitTo8BitPixel, uint8_t, __stdcall, (uint8_t nRed, uint8_t nGreen, uint8_t nBlue), 0xAAE0)													//1.12a #10028
D2FUNC_DLL(D2CMP, PaletteTestBackground, void, __stdcall, (BOOL bTestBackground), 0xABC0)																				//1.12a #10014
D2FUNC_DLL(D2CMP, PaletteSetTransparency, void, __stdcall, (uint32_t dwColor), 0xAD30)																					//1.12a #10070
D2FUNC_DLL(D2CMP, GetFurthestColor, int32_t, __stdcall, (PALETTEENTRY* pPalette, int32_t nPaletteSize, int32_t nRed, int32_t nGreen, int32_t nBlue), 0xAD90)			//1.12a #10066
D2FUNC_DLL(D2CMP, DestroyD2Palette, BOOL, __stdcall, (void* pPalette), 0xAE70)																							//1.12a #10083
D2FUNC_DLL(D2CMP, Palette24BitTo8BitImage, void, __stdcall, (const uint8_t* pSrc, uint8_t* pDst, int32_t nPixels, void* pfnConvert), 0xB320)							//1.12a #10042
D2FUNC_DLL(D2CMP, PaletteInit, BOOL, __stdcall, (const char* szPaletteFile, int32_t nStartIdx, int32_t nEndIdx), 0xB3B0)												//1.12a #10080
D2FUNC_DLL(D2CMP, UpdateD2TransparencyTable, BOOL, __stdcall, (void* pPalette, int32_t nUnused), 0xC9F0)																//1.12a #10090
D2FUNC_DLL(D2CMP, CreateD2Palette, void*, __stdcall, (PALETTEENTRY* pPaletteEntries), 0xCA00)																			//1.12a #10055
D2FUNC_DLL(D2CMP, CreateD2PaletteByteStream, void*, __stdcall, (PALETTEENTRY* pPaletteEntries, uint32_t* pOutSize), 0xCAD0)												//1.12a #10058

// PalShift.cpp
D2FUNC_DLL(D2CMP, PalShiftSetAct, void, __stdcall, (int32_t nAct), 0xCB30)																								//1.12a #10019
D2FUNC_DLL(D2CMP, PalShiftInit, BOOL, __stdcall, (), 0xCD00)																											//1.12a #10063

// TileLib.cpp
D2FUNC_DLL(D2CMP, TileGetFilename, const char*, __stdcall, (D2TileLibraryEntryStrc* pTile), 0xD000)																		//1.12a #10092
D2FUNC_DLL(D2CMP, TileGetRoofHeight, uint16_t, __stdcall, (D2TileLibraryEntryStrc* pTile), 0xD030)																		//1.12a #10072
D2FUNC_DLL(D2CMP, TileGetHeightToBottom, int32_t, __stdcall, (D2TileLibraryEntryStrc* pTile), 0xD060)																	//1.12a #10100
D2FUNC_DLL(D2CMP, TileGetTotalHeight, int32_t, __stdcall, (D2TileLibraryEntryStrc* pTile), 0xD0C0)																		//1.12a #10034
D2FUNC_DLL(D2CMP, TileGetLightDirection, int32_t, __stdcall, (D2TileLibraryEntryStrc* pTile), 0xD0F0)																	//1.12a #10089
D2FUNC_DLL(D2CMP, TileGetWidth, int32_t, __stdcall, (D2TileLibraryEntryStrc* pTile), 0xD180)																			//1.12a #10033
D2FUNC_DLL(D2CMP, TileSetHardwareFreeCallback, void, __stdcall, (void (__fastcall* pfnCallback)(void*)), 0xD240)														//1.12a #10040

// Tilecmp.cpp
D2FUNC_DLL(D2CMP, TileListCreate, void*, __stdcall, (int32_t dwVersion, int32_t dwFlags), 0xF840)																		//1.12a #10103
D2FUNC_DLL(D2CMP, TileProjectIteratePixels, BOOL, __stdcall, (D2TileLibraryHeaderStrc* pHeader, void (__stdcall* pfnCallback)(int32_t)), 0xF990)						//1.12a #10056
D2FUNC_DLL(D2CMP, TileProjectNormalize, BOOL, __stdcall, (D2TileLibraryHeaderStrc* pHeader, D2TileLibraryHeaderStrc** ppOutHeader, const char* szLibraryName), 0xFB00)	//1.12a #10004
D2FUNC_DLL(D2CMP, TileProjectSerialize, BOOL, __stdcall, (D2TileLibraryHeaderStrc* pHeader, D2TileLibraryHeaderStrc** ppOutHeader, int32_t* pOutSize), 0xFC10)			//1.12a #10001
D2FUNC_DLL(D2CMP, TileAddRoof, BOOL, __stdcall, (void* pEntry, void* pPixels, int32_t nStride, int32_t nOffsetX, int32_t nOffsetY), 0x10040)							//1.12a #10101
D2FUNC_DLL(D2CMP, TileListAddTile, void*, __stdcall, (void* pList, int32_t nLightDir, int32_t nRoofHeight, int32_t nFlags, int32_t nType, int32_t nStyle, int32_t nSequence, int32_t nRarity, int32_t dwTransColor), 0x101A0)	//1.12a #10091
D2FUNC_DLL(D2CMP, TileProjectFree, BOOL, __stdcall, (D2TileLibraryHeaderStrc* pHeader, BOOL bKeepMemory), 0x10380)														//1.12a #10011
D2FUNC_DLL(D2CMP, TileListToTileProject, BOOL, __stdcall, (void* pList, D2TileLibraryHeaderStrc** ppOutHeader), 0x104D0)												//1.12a #10069
D2FUNC_DLL(D2CMP, TileListDestroy, BOOL, __stdcall, (void* pList), 0x10730)																								//1.12a #10012
D2FUNC_DLL(D2CMP, TileSetFloorImage, BOOL, __stdcall, (void* pEntry, void* pPixels), 0x108A0)																			//1.12a #10027
D2FUNC_DLL(D2CMP, TileProjectConvert24To8, BOOL, __stdcall, (D2TileLibraryHeaderStrc* pHeader, D2TileLibraryHeaderStrc** ppOutHeader, void* pfnConvert), 0x10900)		//1.12a #10044

// SpriteCache.cpp
D2FUNC_DLL(D2CMP, SpriteCacheIsLoaded, BOOL, __stdcall, (int32_t a1, int32_t a2, int32_t a3), 0x110E0)																	//1.12a #10077
D2FUNC_DLL(D2CMP, SpriteCacheGetAsyncLoadingCount, int32_t, __stdcall, (), 0x11170)																						//1.12a #10007
D2FUNC_DLL(D2CMP, SpriteCacheGetHeaderCacheSize, uint32_t, __stdcall, (), 0x11520)																						//1.12a #10094
D2FUNC_DLL(D2CMP, SpriteCacheGetHeaderCacheUsed, uint32_t, __stdcall, (), 0x11530)																						//1.12a #10076
D2FUNC_DLL(D2CMP, SpriteCacheNoop1, int32_t, __stdcall, (), 0x11540)																									//1.12a #10073
D2FUNC_DLL(D2CMP, SpriteCacheGetFrameCacheSize, uint32_t, __stdcall, (), 0x11550)																						//1.12a #10054
D2FUNC_DLL(D2CMP, SpriteCacheGetFrameCacheUsed, uint32_t, __stdcall, (), 0x11560)																						//1.12a #10022
D2FUNC_DLL(D2CMP, SpriteCacheNoop2, void, __stdcall, (), 0x12330)																										//1.12a #10029
D2FUNC_DLL(D2CMP, SpriteCacheBuildFilename, void, __stdcall, (char* szDest, const char* szSrc, size_t nMaxLen, BOOL bAllowCompressed), 0x12790)							//1.12a #10061
D2FUNC_DLL(D2CMP, SpriteCacheLoadInit, BOOL, __stdcall, (D2GfxDataStrc* pData), 0x131C0)																				//1.12a #10030

// FastCmp.cpp
D2FUNC_DLL(D2CMP, CelToggleDebugDrawMode, void, __stdcall, (), 0x143A0)																									//1.12a #10048
D2FUNC_DLL(D2CMP, FastCmpNoop1, int32_t, __stdcall, (), 0x16B20)																										//1.12a #10016
D2FUNC_DLL(D2CMP, FastCmpNoop2, int32_t, __stdcall, (int32_t a1, int32_t a2), 0x16B30)																					//1.12a #10065
D2FUNC_DLL(D2CMP, FastCmpNoop3, int32_t, __stdcall, (), 0x16B40)																										//1.12a #10087
D2FUNC_DLL(D2CMP, FastCmpNoop4, int32_t, __stdcall, (), 0x16B50)																										//1.12a #10002

// TileProjects.cpp
D2FUNC_DLL(D2CMP, TileCacheResetStats, void, __stdcall, (), 0x16B60)																									//1.12a #10039
D2FUNC_DLL(D2CMP, TileCacheGetStats, void, __stdcall, (int32_t* pMaxSize, int32_t* pMisses, int32_t* pHits, int32_t* pHitRate), 0x16B70)								//1.12a #10104
D2FUNC_DLL(D2CMP, InitTileCache, void, __stdcall, (void* pMemPool, int32_t nCacheSize), 0x16CB0)																		//1.12a #10015
D2FUNC_DLL(D2CMP, DestroyTileCache, void, __stdcall, (BOOL bFreeAll), 0x16E70)																							//1.12a #10064
D2FUNC_DLL(D2CMP, TileProjectsLoadLibrary, BOOL, __stdcall, (D2TileLibraryHashStrc** ppTileLibraryHash, const char* szFilename), 0x17020)								//1.12a #10071
D2FUNC_DLL(D2CMP, TileProjectsFreeAll, void, __stdcall, (), 0x17180)																									//1.12a #10051
