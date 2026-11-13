// ============================================================================
// ENGINE_Gfx.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll
// Address ranges:
//   0x6FB40550 .. 0x6FB44FE0  (..\Source\D2Client\ENGINE\Gfx.cpp)
//   0x6FB668E0 .. 0x6FB68230  (..\Source\D2Client\ENGINE\GfxUtil.cpp)
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "D2ClientReconstructed.h"

int __fastcall D2Client_PopulateUnitComponentGfxNames(D2UnitStrc* pUnit, D2GfxDataStrc* pOutGfxData, int nComponent, const char* szWeaponClass);
int __fastcall D2Client_PopulateNonCompositeGfxName(D2UnitStrc* pUnit, D2GfxDataStrc* pOutGfxData);
struct D2MonStats2Txt* __fastcall UNITS_GetMonStats2TxtRecordFromMonsterId(int nMonsterId);

// Forward declarations for D2MOO / engine types
struct D2UnitStrc;
struct D2ActiveRoomStrc;
struct D2GfxDataStrc;
struct D2GfxCellStrc;
struct D2CellFileStrc;
struct D2InventoryStrc;

void __cdecl FOG_DisplayWarning(const char* szExpr, const char* szFile, int nLine);

// ============================================================================
// External Imports (D2Common, D2Gfx, D2CMP, D2Win, Fog, Storm)
// ============================================================================
extern "C" {
    // Fog.dll
    void* __fastcall FOG_AllocClientMemory(int nSize, const char* szFile, int nLine, int nZero); // #10042
    void  __fastcall FOG_FreeClientMemory(void* pMem, const char* szFile, int nLine, int nZero); // #10043
    void  __cdecl    FOG_DisplayAssert(const char* szExpr, const char* szFile, int nLine);       // #10023
    void  __cdecl    FOG_DisplayHalt(const char* szExpr, const char* szFile, int nLine);         // #10024
    void  __cdecl    FOG_Trace(const char* szFmt, ...);                                          // #10029
    const char* __cdecl Fog_10265(int nErrorIndex);                                              // #10265

    // Storm.dll
    int   __stdcall  SStrCopy(char* szDst, const char* szSrc, int nMaxLen);                      // #501
    uint32_t __stdcall SStrHash(const char* szString, unsigned long dwFlags, unsigned long dwSeed); // #505
    int   __stdcall  SStrCmpI(const char* szStr1, const char* szStr2, size_t nSize);             // #508
    char* __stdcall  SStrChr(const char* szStr, char c, int bReverse);                           // #502

    // D2Win.dll
    uint32_t __fastcall D2Win_10014();                                                           // #10014 (GetArchiveHandle)

    // D2Gfx.dll
    int   __stdcall  WINDOW_GetState();                                                          // #10002
    void  __stdcall  D2Gfx_10012_CelDrawHilight(D2GfxDataStrc* pData, int nXPos, int nYPos, uint8_t nPaletteIndex); // #10012
    void  __stdcall  D2Gfx_10024_CelDraw(D2GfxDataStrc* pData, int nXPos, int nYPos, uint32_t dwGamma, int nDrawMode, uint8_t* pPalette); // #10024
    void  __stdcall  D2Gfx_10029_CelDrawEx(D2GfxDataStrc* pData, int nXPos, int nYPos, int nSkipLines, int nDrawLines, int nDrawMode); // #10029
    void  __stdcall  D2Gfx_10030_CelDrawShadow(D2GfxDataStrc* pData, int nXPos, int nYPos);      // #10030
    int   __stdcall  TEXTURE_CelFlatSpriteDraw(D2GfxDataStrc* pData, int nXPos, int nYPos, uint32_t dwGamma, int nDrawMode, int nScreenMode, uint8_t* pPalette); // #10061
    int   __stdcall  D2Gfx_10062_CheckPerspectivePosition(int nXPos, int nYPos);                 // #10062
    void  __stdcall  D2Gfx_10065_TransformPerspectiveCoords(int nXPos, int nYPos, int nAngle, int* pOutX, int* pOutY, int bOrder); // #10065
    int   __stdcall  D2Gfx_10069_AdjustPerspectivePosition(int nXPos, int nYPos, int nBais, int* pXAdjust, int* pYAdjust); // #10069
    void  __stdcall  D2Gfx_10075_DrawLine(int nXStart, int nYStart, int nXEnd, int nYEnd, uint8_t nColor, uint8_t nAlpha); // #10075

    // D2CMP.dll
    int   __stdcall  D2CMP_CelGetWidth(D2GfxCellStrc* pCell);                                    // #10008
    int   __stdcall  D2CMP_CelGetOffsetX(D2GfxCellStrc* pCell);                                  // #10011
    void  __stdcall  D2CMP_SpriteFreeAsyncLoads(D2GfxDataStrc* pData);                           // #10017
    int   __stdcall  D2CMP_SpriteGetFrameCount(D2GfxDataStrc* pData, int* pDirections, int* pFramesPerDir); // #10019
    int   __stdcall  D2CMP_SpriteValidate(D2GfxDataStrc* pData, int bAsync, int bForceLoad);     // #10020
    int   __stdcall  D2CMP_SpriteCacheGetAsyncLoadingCount();                                    // #10028
    int   __stdcall  D2CMP_CelGetHeight(D2GfxCellStrc* pCell);                                   // #10042
    int   __stdcall  D2CMP_CelGetOffsetY(D2GfxCellStrc* pCell);                                  // #10043
    int   __stdcall  D2CMP_CelGetHandle(D2GfxDataStrc* pData);                                   // #10054
    uint8_t* __stdcall D2CMP_GetPaletteRemapTable(int nDrawMode, int nTransIndex);               // #10065
    int   __stdcall  D2CMP_10087_GetLowQualityFlag();                                            // #10087

    // D2Common.dll
    void  __stdcall  ROOM_GetRoomCoordinates(D2ActiveRoomStrc* pRoom, void* pCoords);            // #10001
    D2UnitStrc* __stdcall DUNGEON_GetFirstUnitInRoom(D2ActiveRoomStrc* pRoom);                   // #10029
    D2UnitStrc* __stdcall INVENTORY_GetItemFromInventory(D2InventoryStrc* pInventory);           // #10035
    void  __stdcall  UNITS_UpdateDirectionAndSpeed(D2UnitStrc* pUnit, int nX, int nY);           // #10042
    int   __stdcall  UNITS_GetWeaponClass(D2UnitStrc* pUnit);                                    // #10045
    int   __stdcall  ITEMS_CheckIfSocketable(D2UnitStrc* pItem);                                 // #10099
    void  __stdcall  ROOM_GetAdjacentRoomsList(D2ActiveRoomStrc* pRoom, D2ActiveRoomStrc*** pppRoomList, int* pNumRooms); // #10145
    int   __stdcall  STATLIST_GetUnitAlignment(D2UnitStrc* pUnit);                               // #10150
    int   __stdcall  PATH_GetNumberOfPathPoints(void* pPath);                                    // #10164
    int   __stdcall  ITEMS_GetFileIndex(D2UnitStrc* pItem);                                      // #10170
    void* __stdcall  DATATBLS_GetPlayerCharTxtRecord(int nPlayerClass);                          // #10174
    void* __stdcall  DATATBLS_GetMonSeqTxtRecord(int nSeqId);                                    // #10186
    int   __stdcall  DUNGEON_GetLevelIdFromRoom(D2ActiveRoomStrc* pRoom);                        // #10188
    int   __stdcall  UNITS_GetCurrentLifePercentage(D2UnitStrc* pUnit);                          // #10256
    int   __stdcall  D2Common_10269_COMPOSIT(D2UnitStrc* pUnit, int* pOutWeaponClass, int nMode, int bForce, void* pInventory); // #10269
    void* __stdcall  DATATBLS_GetItemsTxtRecord(int nItemId);                                    // #10279
    int   __stdcall  ITEMS_GetBodyLocation(D2UnitStrc* pItem);                                   // #10283
    D2ActiveRoomStrc* __stdcall UNITS_GetRoom(D2UnitStrc* pUnit);                                // #10331
    int   __stdcall  STATES_CheckStateMaskOnUnit(D2UnitStrc* pUnit, int nStateMask);             // #10332
    int   __stdcall  ITEMS_GetColor(D2UnitStrc* pUnit, D2UnitStrc* pItem, int* pColorIndex, int nTransType); // #10346
    void* __stdcall  UNITS_GetObjectTxtRecordFromObject(D2UnitStrc* pObject);                    // #10373
    void* __stdcall  DATATBLS_GetObjectsTxtRecord(int nObjectId);                                // #10396
    int   __stdcall  UNITS_GetClientCoordX(D2UnitStrc* pUnit);                                   // #10420
    int   __stdcall  UNITS_GetClientCoordY(D2UnitStrc* pUnit);                                   // #10467
    int   __stdcall  UNITS_GetDir(D2UnitStrc* pUnit);                                            // #10483
    int   __stdcall  STATLIST_UnitGetStatValue(D2UnitStrc* pUnit, int nStatId, int nLayer);      // #10521
    void* __stdcall  DATATBLS_GetMonsterTxtRecord(int nMonsterId);                               // #10574
    void  __stdcall  MONSTERAI_GetMonsterComponents(D2UnitStrc* pUnit, uint8_t* pComponents);    // #10602
    void* __stdcall  DATATBLS_GetOverlaysTxtRecord(int nOverlayId);                              // #10665
    int   __stdcall  ITEMS_CheckItemFlag(D2UnitStrc* pItem, uint32_t dwFlag, int nLine, const char* szFile); // #10733
    D2UnitStrc* __stdcall INVENTORY_GetNextItem(D2UnitStrc* pItem);                              // #10772
    int   __stdcall  STATES_CheckStateMaskGreenOnUnit(D2UnitStrc* pUnit);                        // #10777
    int   __stdcall  D2Common_10862_GetMissileTrans(int nMissileId, uint8_t* pOutDrawMode, uint8_t* pOutTransIdx); // #10862
    void  __stdcall  D2Common_10885_COMPOSIT(D2UnitStrc* pUnit, char* szClass, char* szMode, int nUnitType, int nClassId, int nMode, int nWeaponClass, char* szWeapon); // #10885
    int   __stdcall  D2Common_10899_COMPOSIT_IsArmorComponent(int nComponent);                   // #10899
    void  __stdcall  D2Common_10924_COMPOSIT(int nWeaponClassId, char* szOutCode);               // #10924
    int   __stdcall  ITEMS_CheckNotSocketable(D2UnitStrc* pItem);                                // #10945
    int   __stdcall  D2Common_11012_CheckOverlayMask(D2UnitStrc* pUnit);                         // #11012
    int   __stdcall  STATES_CheckState(D2UnitStrc* pUnit, int nState);                           // #11017
    uint32_t* __stdcall STATES_GetListGfxFlags(D2UnitStrc* pUnit);                               // #11035
    int   __stdcall  D2Common_11037_GetStateColor(D2UnitStrc* pUnit, uint8_t* pOutIndex);        // #11037
    void  __stdcall  MONSTER_GetRightArmItemCode(int nMonsterId, int nComponentIndex, int nLevelId, char* szOutCode); // #11056
    int   __stdcall  MONSTER_HasComponents(D2UnitStrc* pUnit);                                   // #11092
    int   __stdcall  D2Common_11119_GetMissileDrawMode(int nMissileId, uint8_t* pOutDrawMode);   // #11119
    void  __stdcall  MONSTERS_GetCompInfo(void* pMonStatsTxt, int nComponent, uint8_t nCompIdx, uint8_t* pOutDrawMode, uint8_t* pOutTransIdx, uint8_t* pOutColor); // #11157
}

// ============================================================================
// Internal D2Client Cross-Module Helpers
// ============================================================================
void* __fastcall ARCHIVE_LoadFile(uint32_t hArchive, const char* szPath, DWORD* pdwSize, int bIsSystem, int bAllowFail); // 0x6FB6EF10
void  __stdcall  LIGHTMAP_GetViewportOffset(int* pOutX, int* pOutY); // 0x6FB3DF50
int   __stdcall  LIGHTMAP_GetViewportShiftX();                       // 0x6FB3E080
int   __stdcall  LIGHTMAP_GetViewportShiftY();                       // 0x6FB3E060
void  __fastcall LIGHTMAP_GetTileLight(D2ActiveRoomStrc* pRoom, int nX, int nY, uint32_t* pOutLight, int bPerspective); // 0x6FB0F4C0
int   __fastcall LIGHTMAP_AttachOverlayLight(D2UnitStrc* pUnit, int nOverlayId); // 0x6FB0EAF0
void  __fastcall PARTICLE_UpdateUnitParticles(D2UnitStrc* pUnit, int nOffsetY);  // 0x6FB75BA0
void  __fastcall PARTICLE_FreeList(void** ppParticleHead);                       // 0x6FB75530
D2UnitStrc* __stdcall PLAYER_GetLocalPlayer();                                   // 0x6FB283D0
int   __fastcall ROSTER_AreUnitsInSameParty(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2); // 0x6FB27980
D2UnitStrc* __fastcall CLIENT_FindUnit(int nUnitId, int nUnitType);              // 0x6FB161F0
int   __fastcall MISSILE_DrawSpecial(D2UnitStrc* pMissile);                      // 0x6FB5CE80
uint8_t* __fastcall PALSHIFT_GetMonsterPalette(int nMonsterId, int nPalIdx);     // 0x6FB6F5B0
uint8_t* __fastcall PALSHIFT_GetUnitPalette(int nColorIdx);                      // 0x6FB6F010
int   __fastcall GFXUTIL_EngineDirToCofDir(int nEngineDir, int nCofDirections);  // 0x6FB66950

// ============================================================================
// Structures
// ============================================================================

#pragma pack(push, 1)

// Raw .COF file header & layer records
struct D2CofLayerStrc
{
    uint8_t nComponent;       // +0x00 (0..15)
    uint8_t bShadow;          // +0x01
    uint8_t bSelectable;      // +0x02
    uint8_t nOverrideTranslvl;// +0x03
    uint8_t nNewTranslvl;     // +0x04
    char    szWeaponClass[4]; // +0x05 (3 chars + NUL)
};

struct D2CofFileStrc
{
    uint8_t nLayers;          // +0x00
    uint8_t nFramesPerDir;    // +0x01
    uint8_t nDirections;      // +0x02
    uint8_t nVersion;         // +0x03
    uint32_t dwUnknown04;     // +0x04
    int32_t nXMin;            // +0x08
    int32_t nXMax;            // +0x0C
    int32_t nYMin;            // +0x10
    int32_t nYMax;            // +0x14
    uint16_t wAnimRate;       // +0x18
    uint16_t wZeros;          // +0x1A
    // Followed at +0x1C by:
    //   D2CofLayerStrc aLayers[nLayers];           (9 bytes * nLayers)
    //   uint8_t        aFrameTriggers[nFramesPerDir];
    //   uint8_t        aPriority[nDirections * nFramesPerDir * nLayers];
};

// Runtime unpacked COF descriptor (0x30 bytes header + inline payload)
struct D2CofRecordStrc
{
    uint8_t nLayers;              // +0x00
    uint8_t nFramesPerDir;        // +0x01
    uint8_t nDirections;          // +0x02
    uint8_t nPad03;               // +0x03
    int32_t nXMin;                // +0x04
    int32_t nXMax;                // +0x08
    int32_t nYMin;                // +0x0C
    int32_t nYMax;                // +0x10
    int32_t nAnimRate;            // +0x14
    D2CofLayerStrc* pLayers;      // +0x18
    uint8_t* pFrameTriggers;      // +0x1C
    uint8_t* pLayerPriority;      // +0x20
    uint8_t  aCompToLayer[16];    // +0x24 .. +0x33 (0xFF if absent)
};

// COF hash bucket node (0x18 bytes header; packed nodes have D2CofRecordStrc at +0x18)
struct D2CofHashNodeStrc
{
    char     szCofName[8];        // +0x00 (7 chars + NUL)
    int32_t  nRefCount;           // +0x08
    int32_t  bDynamicAlloc;       // +0x0C (1 = dynamically allocated, 0 = inside packed .d2 buffer)
    D2CofRecordStrc* pCofRecord;  // +0x10
    D2CofHashNodeStrc* pNext;     // +0x14
};

// 512-bucket COF cache table (0x80C bytes)
struct D2CofCacheTableStrc
{
    void*    pPackedBuffer;       // +0x00 (from ARCHIVE_LoadFile)
    int32_t  nType;               // +0x04 (0 = monsters/objects, 1 = players)
    int32_t  nLoadedActIndex;     // +0x08 (-1 if unloaded)
    D2CofHashNodeStrc* pBuckets[512]; // +0x0C .. +0x80B
};

// Unit COF cache linked list node (0x14 bytes)
struct D2UnitCofLinkStrc
{
    int32_t  nMode;               // +0x00
    int32_t  nWeaponClass;        // +0x04
    D2CofHashNodeStrc* pCofNode;  // +0x08
    int32_t  nReserved0C;         // +0x0C
    D2UnitCofLinkStrc* pNext;     // +0x10
};

// D2GfxDataStrc (0x48 bytes, defined in D2ClientReconstructed.h)

// Unit Overlay node (0xA8 bytes, allocated in 0x6FB430E0)
struct D2OverlayStrc
{
    D2GfxDataStrc tGfxData;       // +0x00 .. +0x47
    int32_t  nOverlayId;          // +0x48
    int32_t  nFileIndex;          // +0x4C
    int32_t  bPreDraw;            // +0x50
    int32_t  nFrames;             // +0x54
    int32_t  nXOffset;            // +0x58
    int32_t  nYOffset;            // +0x5C
    int32_t  nHeightOffset;       // +0x60
    int32_t  nAnimRate;           // +0x64
    int32_t  nInitWait;           // +0x68
    uint32_t dwTrans;             // +0x6C
    int32_t  nCurFrameFp;         // +0x70 (fixed-point 8.8 frame counter)
    int32_t  nEndFrameFp;         // +0x74 (fixed-point 8.8 end frame)
    int32_t  nStepFp;             // +0x78
    uint8_t  nDrawMode;           // +0x7C
    uint8_t  nTransIndex;         // +0x7D
    uint8_t  bLoopWaitDone;       // +0x7E
    uint8_t  nPad7F;              // +0x7F
    uint32_t dwStartTick;         // +0x80
    int32_t  nDirType;            // +0x84 (0 = 1-dir, 1 = 4-dir, 2 = 8-dir, 3 = 16-dir)
    uint32_t dwUnitFlags;         // +0x88 (1 = active)
    int32_t  nVersion;            // +0x8C
    int32_t  nLightIndex;         // +0x90
    int32_t  b1ofN;               // +0x94
    int32_t  nReserved98;         // +0x98
    int32_t  nReserved9C;         // +0x9C
    D2OverlayStrc* pPrev;         // +0xA0
    D2OverlayStrc* pNext;         // +0xA4
};

// Unit Gfx Data container at pUnit->pGfxData (+0x54, 0x3C bytes, allocated in 0x6FB408A0)
struct D2UnitGfxDataStrc
{
    uint16_t aLayerFlags[20];     // +0x00 .. +0x27 (16 components + 4 extra slots)
    D2UnitCofLinkStrc* pCofList;  // +0x28
    D2OverlayStrc* pOverlayList;  // +0x2C
    void*    pParticleList;       // +0x30
    int32_t  nHeightIndex;        // +0x34 (initialized to 2)
    int32_t  nDefaultHeightIndex; // +0x38 (initialized to 2)
};

#pragma pack(pop)

// ============================================================================
// Global Variables (0x6FBB1B08 .. 0x6FBC21DC)
// ============================================================================

// ENGINE/Gfx.cpp globals
static D2CofCacheTableStrc g_MonsterCofCache;  // 0x6FBB1B08 (size 0x80C)
static D2CofCacheTableStrc g_PlayerCofCache;   // 0x6FBB2318 (size 0x80C, +0x04 = 1, +0x08 = -1)
static uint8_t  g_bHighlightGreen   = 0;       // 0x6FBB2B28
static uint8_t  g_bHighlightYellow  = 0;       // 0x6FBB2B29
static uint8_t  g_bHighlightRed     = 0;       // 0x6FBB2B2A
static uint8_t  g_bHighlightBlue    = 0;       // 0x6FBB2B2B
static DWORD    g_dwLastAsyncPrecacheTick = 0; // 0x6FBB2B2C

extern int32_t  g_nScreenWidth;                // 0x6FB8C6E0
extern int32_t  g_nScreenHeight;               // 0x6FB8C6E4
extern int32_t  g_bDrawUnitDebugBoxes;         // 0x6FBB58F8

// ENGINE/GfxUtil.cpp globals
static int32_t  g_aDirToCofDir32[32];          // 0x6FBC2130 (32-dir engine to COF dir lookup)
static int32_t  g_aDirToCofDir16[16];          // 0x6FBC21B0 (16-dir engine to COF dir lookup)
static int32_t  g_nCurrentActForCof = -1;      // 0x6FB9A7F0
static int32_t  g_nUnitHoverPaddingY = 0;      // 0x6FBC21CC
static D2GfxDataStrc g_tTempUnitGfxData;       // 0x6FBC2184 (and 0x6FBC21DC)


// ============================================================================
// PART 1: ENGINE/Gfx.cpp (0x6FB40550 .. 0x6FB44FE0)
// ============================================================================

// 1.12a: 0x6FB40550
// Unpacks raw .COF file data into a D2CofRecordStrc and initializes its 16 component-to-layer indices.
void __fastcall GFX_InitCofRecord(D2CofRecordStrc* pCofRecord, const D2CofFileStrc* pRawCof)
{
    pCofRecord->nLayers       = pRawCof->nLayers;
    pCofRecord->nFramesPerDir = pRawCof->nFramesPerDir;
    pCofRecord->nDirections   = pRawCof->nDirections;
    pCofRecord->nXMin         = pRawCof->nXMin;
    pCofRecord->nXMax         = pRawCof->nXMax;
    pCofRecord->nYMin         = pRawCof->nYMin;
    pCofRecord->nYMax         = pRawCof->nYMax;
    pCofRecord->nAnimRate     = (int32_t)pRawCof->wAnimRate;

    uint8_t* pCursor = (uint8_t*)pRawCof + 0x1C;
    pCofRecord->pLayers = (D2CofLayerStrc*)pCursor;

    pCursor += 9 * (uint32_t)pCofRecord->nLayers;
    pCofRecord->pFrameTriggers = pCursor;

    pCursor += (uint32_t)pCofRecord->nFramesPerDir;
    pCofRecord->pLayerPriority = pCursor;

    memset(pCofRecord->aCompToLayer, 0xFF, sizeof(pCofRecord->aCompToLayer));

    for (uint8_t i = 0; i < pCofRecord->nLayers; ++i) {
        uint8_t nComp = pCofRecord->pLayers[i].nComponent;
        if (nComp >= 16) {
            FOG_DisplayAssert("pLayer->bComp < NUM_COMPONENTS", "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 204);
            exit(-1);
        }
        pCofRecord->aCompToLayer[nComp] = i;
    }
}

// 1.12a: 0x6FB405E0
// Frees dynamically allocated COF nodes in a 512-bucket COF cache table.
void __fastcall GFX_PurgeCofCacheBuckets(D2CofCacheTableStrc* pTable, int bForceFreeAll)
{
    for (int i = 0; i < 512; ++i) {
        D2CofHashNodeStrc* pNode = pTable->pBuckets[i];
        D2CofHashNodeStrc* pPrev = NULL;
        while (pNode) {
            D2CofHashNodeStrc* pNext = pNode->pNext;
            if (pNode->bDynamicAlloc || bForceFreeAll) {
                if (bForceFreeAll && pNode->nRefCount != 0) {
                    pNode->nRefCount = 0;
                }
                if (pNode->nRefCount == 0) {
                    if (pPrev) {
                        pPrev->pNext = pNext;
                    } else {
                        pTable->pBuckets[i] = pNext;
                    }
                    if (pNode->pCofRecord) {
                        FOG_FreeClientMemory(pNode->pCofRecord, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 242, 0);
                    }
                    FOG_FreeClientMemory(pNode, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 243, 0);
                    pNode = pNext;
                    continue;
                }
            }
            pPrev = pNode;
            pNode = pNext;
        }
    }
}

// 1.12a: 0x6FB406A0
void __fastcall GFX_DecrementCofNodeRef(D2CofHashNodeStrc* pNode)
{
    if (!pNode) {
        return;
    }
    if (pNode->nRefCount > 0) {
        pNode->nRefCount--;
    }
}

// 1.12a: 0x6FB406D0
void __fastcall GFX_FreeUnitCofList(D2UnitGfxDataStrc* pGfxData)
{
    if (!pGfxData) {
        return;
    }
    D2UnitCofLinkStrc* pCur = pGfxData->pCofList;
    while (pCur) {
        D2UnitCofLinkStrc* pNext = pCur->pNext;
        D2CofHashNodeStrc* pNode = pCur->pCofNode;
        if (pNode && pNode->nRefCount > 0) {
            pNode->nRefCount--;
        }
        FOG_FreeClientMemory(pCur, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 370, 0);
        pCur = pNext;
    }
    pGfxData->pCofList = NULL;
}

D2CofRecordStrc* __fastcall GFX_FindUnitCofByMode(D2UnitStrc* pUnit, int nMode, int nWeaponClass);
void __fastcall GFX_UpdateUnitCofForCurrentMode(D2UnitStrc* pUnit);

// 1.12a: 0x6FB40740
D2CofRecordStrc* __fastcall GFX_GetUnitActiveCofRecord(D2UnitStrc* pUnit)
{
    if (!pUnit || *(uint32_t*)pUnit > 2) {
        return NULL;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return NULL;
    }
    int nMode = *(int*)((uint8_t*)pUnit + 0x10);
    int nWeaponClass = UNITS_GetWeaponClass(pUnit);
    D2CofRecordStrc* pModeCof = GFX_FindUnitCofByMode(pUnit, nMode, nWeaponClass);
    if (pModeCof) {
        return pModeCof;
    }
    GFX_UpdateUnitCofForCurrentMode(pUnit);
    pModeCof = GFX_FindUnitCofByMode(pUnit, nMode, nWeaponClass);
    if (pModeCof) {
        return pModeCof;
    }
    for (D2UnitCofLinkStrc* pCofLink = pGfxData->pCofList; pCofLink; pCofLink = pCofLink->pNext) {
        if (pCofLink->pCofNode && pCofLink->pCofNode->pCofRecord) {
            return pCofLink->pCofNode->pCofRecord;
        }
    }
    return NULL;
}

// 1.12a: 0x6FB40780
D2UnitGfxDataStrc* __fastcall GFX_GetUnitGfxData(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        return NULL;
    }
    return *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
}

// 1.12a: 0x6FB407A0
D2OverlayStrc* __fastcall GFX_GetUnitOverlayList(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        return NULL;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return NULL;
    }
    return pGfxData->pOverlayList;
}

// 1.12a: 0x6FB407C0
D2OverlayStrc** __fastcall GFX_GetUnitOverlayListPtr(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        return NULL;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return NULL;
    }
    return &pGfxData->pOverlayList;
}

// 1.12a: 0x6FB407E0
void __fastcall GFX_SetUnitComponentFlags(D2UnitStrc* pUnit, int nComponent, uint16_t wFlags, int bSet)
{
    if (!pUnit) {
        return;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return;
    }
    if (bSet) {
        pGfxData->aLayerFlags[nComponent] |= wFlags;
    } else {
        pGfxData->aLayerFlags[nComponent] &= ~wFlags;
    }
}

// 1.12a: 0x6FB40810
uint16_t __fastcall GFX_GetUnitComponentFlags(D2UnitStrc* pUnit, int nComponent, uint16_t wMask)
{
    if (!pUnit) {
        return 0;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return 0;
    }
    return pGfxData->aLayerFlags[nComponent] & wMask;
}

// 1.12a: 0x6FB40840
int __fastcall GFX_GetUnitHeightOffset(D2UnitStrc* pUnit)
{
    static const int s_aHeightOffsets[5] = { 0, -50, -75, -100, -100 }; // 0x6FB842D0
    if (pUnit) {
        D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
        if (pGfxData) {
            return s_aHeightOffsets[pGfxData->nHeightIndex];
        }
    }
    return s_aHeightOffsets[2];
}

// 1.12a: 0x6FB40860
void __fastcall GFX_SetUnitHeightIndex(D2UnitStrc* pUnit, int nHeightIndex)
{
    if (!pUnit) {
        return;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (pGfxData) {
        pGfxData->nHeightIndex = nHeightIndex;
    }
}

// 1.12a: 0x6FB40880
void __fastcall GFX_ResetUnitHeightIndex(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        return;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (pGfxData) {
        pGfxData->nHeightIndex = pGfxData->nDefaultHeightIndex;
    }
}

// 1.12a: 0x6FB408A0
D2UnitGfxDataStrc* __fastcall GFX_AllocUnitGfxData()
{
    D2UnitGfxDataStrc* pGfxData = (D2UnitGfxDataStrc*)FOG_AllocClientMemory(
        sizeof(D2UnitGfxDataStrc), "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 772, 0);
    memset(pGfxData, 0, sizeof(D2UnitGfxDataStrc));
    pGfxData->nHeightIndex = 2;
    pGfxData->nDefaultHeightIndex = 2;
    return pGfxData;
}

// 1.12a: 0x6FB408E0
void __fastcall GFX_FreeOverlayList(D2OverlayStrc** ppOverlayHead)
{
    if (!ppOverlayHead) {
        return;
    }
    D2OverlayStrc* pCur = *ppOverlayHead;
    while (pCur) {
        D2OverlayStrc* pNext = pCur->pNext;
        FOG_FreeClientMemory(pCur, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 1185, 0);
        pCur = pNext;
    }
    *ppOverlayHead = NULL;
}

// 1.12a: 0x6FB40920
void __fastcall GFX_FreeUnitOverlays(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        return;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return;
    }
    GFX_FreeOverlayList(&pGfxData->pOverlayList);
}

// 1.12a: 0x6FB40960
void __fastcall GFX_RemoveOverlayById(D2UnitStrc* pUnit, int nOverlayId)
{
    if (!pUnit) {
        return;
    }
    D2OverlayStrc** ppHead = GFX_GetUnitOverlayListPtr(pUnit);
    if (!ppHead) {
        return;
    }
    D2OverlayStrc* pCur = *ppHead;
    while (pCur) {
        D2OverlayStrc* pNext = pCur->pNext;
        if (pCur->nOverlayId == nOverlayId) {
            if (pCur->pPrev) {
                pCur->pPrev->pNext = pCur->pNext;
            }
            if (pCur->pNext) {
                pCur->pNext->pPrev = pCur->pPrev;
            }
            if (pCur == *ppHead) {
                *ppHead = pCur->pNext;
            }
            FOG_FreeClientMemory(pCur, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 1218, 0);
        }
        pCur = pNext;
    }
}

// 1.12a: 0x6FB409E0
void __fastcall GFX_UpdateUnitOverlaysAndParticles(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        return;
    }
    int nHeightOffset = GFX_GetUnitHeightOffset(pUnit);
    PARTICLE_UpdateUnitParticles(pUnit, nHeightOffset);

    D2OverlayStrc** ppHead = GFX_GetUnitOverlayListPtr(pUnit);
    if (!ppHead) {
        return;
    }

    D2OverlayStrc* pCur = *ppHead;
    while (pCur) {
        D2OverlayStrc* pNext = pCur->pNext;
        if (pCur->bLoopWaitDone) {
            int nNewFrameFp = pCur->nCurFrameFp + pCur->nStepFp;
            pCur->nCurFrameFp = nNewFrameFp;
            if (nNewFrameFp >= pCur->nEndFrameFp) {
                if (pCur->pPrev) {
                    pCur->pPrev->pNext = pCur->pNext;
                }
                if (pCur->pNext) {
                    pCur->pNext->pPrev = pCur->pPrev;
                }
                if (pCur == *ppHead) {
                    *ppHead = pCur->pNext;
                }
                FOG_FreeClientMemory(pCur, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 1363, 0);
            }
        } else if (pCur->nInitWait != 0) {
            if (GetTickCount() - pCur->dwStartTick > (DWORD)(40 * pCur->nInitWait)) {
                pCur->bLoopWaitDone = 1;
            }
        } else {
            pCur->bLoopWaitDone = 1;
        }
        pCur = pNext;
    }
}

// 1.12a: 0x6FB40AB0
int __fastcall GFX_GetOverlayLightIndex(D2UnitStrc* pUnit, int nOverlayId, int* pOutFrame, int* pOutTotalFrames)
{
    *pOutFrame = 0;
    *pOutTotalFrames = 0;
    if (!pUnit) {
        return 0;
    }
    D2OverlayStrc* pCur = GFX_GetUnitOverlayList(pUnit);
    while (pCur) {
        if (pCur->nOverlayId == nOverlayId) {
            *pOutFrame = pCur->nCurFrameFp >> 8;
            *pOutTotalFrames = pCur->nEndFrameFp >> 8;
            return pCur->nLightIndex;
        }
        pCur = pCur->pNext;
    }
    return 0;
}

// 1.12a: 0x6FB40B00
void __fastcall GFX_ScaleActiveUnitOverlays(D2UnitStrc* pUnit, int nPercent)
{
    if (!pUnit) {
        return;
    }
    D2OverlayStrc** ppHead = GFX_GetUnitOverlayListPtr(pUnit);
    if (!ppHead || !*ppHead) {
        return;
    }
    for (D2OverlayStrc* pCur = *ppHead; pCur; pCur = pCur->pNext) {
        if (pCur->bLoopWaitDone) {
            int nRate = pCur->nAnimRate + (nPercent * pCur->nAnimRate) / 100;
            if (nRate <= 0) {
                nRate = 1;
            }
            pCur->nStepFp = nRate;
        }
    }
}

// 1.12a: 0x6FB40B60
void __fastcall GFX_FreeUnitGfxData(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        return;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return;
    }
    GFX_FreeUnitCofList(pGfxData);
    GFX_FreeOverlayList(&pGfxData->pOverlayList);
    PARTICLE_FreeList(&pGfxData->pParticleList);
    FOG_FreeClientMemory(pGfxData, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 790, 0);
    *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54) = NULL;
}

// 1.12a: 0x6FB40BF0
void __fastcall GFX_ExtractBaseCofName(char* szOutBaseName, const char* szFullPath)
{
    const char* pSlash = strrchr(szFullPath, '\\');
    if (!pSlash) {
        pSlash = szFullPath - 1;
    }
    SStrCopy(szOutBaseName, pSlash + 1, 8);
    szOutBaseName[7] = '\0';
    char* pDot = strchr(szOutBaseName, '.');
    if (pDot) {
        *pDot = '\0';
    }
}

// 1.12a: 0x6FB40C40
uint32_t __fastcall GFX_HashCofName(const char* szCofName)
{
    if (!szCofName) {
        return 0;
    }
    char szBase[260];
    if (strchr(szCofName, '\\') || strchr(szCofName, '.')) {
        GFX_ExtractBaseCofName(szBase, szCofName);
        szCofName = szBase;
    }
    char szUpper[16] = {};
    for (int i = 0; i < 15 && szCofName[i]; ++i) {
        char c = szCofName[i];
        if (c >= 'a' && c <= 'z') {
            c = (char)(c - ('a' - 'A'));
        }
        szUpper[i] = c;
    }
    uint32_t h = SStrHash(szUpper, 1, 0);
    return (h ^ (h >> 8) ^ (h >> 16) ^ (h >> 24)) & 0x1FF;
}

// 1.12a: 0x6FB40CF0
void __cdecl GFX_FreeMonsterCofCache()
{
    GFX_PurgeCofCacheBuckets(&g_MonsterCofCache, 1);
    if (g_MonsterCofCache.pPackedBuffer) {
        FOG_FreeClientMemory(g_MonsterCofCache.pPackedBuffer, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 268, 0);
    }
    memset(&g_MonsterCofCache, 0, sizeof(g_MonsterCofCache));
    g_MonsterCofCache.nLoadedActIndex = -1;
}

// 1.12a: 0x6FB40D40
void __cdecl GFX_FreePlayerCofCache()
{
    GFX_PurgeCofCacheBuckets(&g_PlayerCofCache, 1);
    if (g_PlayerCofCache.pPackedBuffer) {
        FOG_FreeClientMemory(g_PlayerCofCache.pPackedBuffer, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 268, 0);
    }
    memset(&g_PlayerCofCache, 0, sizeof(g_PlayerCofCache));
    g_PlayerCofCache.nType = 1;
    g_PlayerCofCache.nLoadedActIndex = -1;
}

// 1.12a: 0x6FB40D90
// Computes unit selection highlight palette color based on alignment, mercenary status, and life percentage.
uint8_t __fastcall GFX_GetUnitHighlightPaletteColor(D2UnitStrc* pUnit)
{
    if (!g_bHighlightGreen) {
        uint8_t* pPal = D2CMP_GetPaletteRemapTable(3, 5);
        g_bHighlightGreen  = pPal[2];
        g_bHighlightYellow = pPal[7];
        g_bHighlightRed    = pPal[1];
        g_bHighlightBlue   = pPal[3];
    }

    int nAlign = STATLIST_GetUnitAlignment(pUnit);
    if (nAlign == 2) {
        return g_bHighlightBlue;
    }

    if (*(uint32_t*)pUnit == 1) { // UNIT_MONSTER
        void* pMonStats = DATATBLS_GetMonsterTxtRecord(*(uint32_t*)((uint8_t*)pUnit + 0x04));
        if (!pMonStats || (*(uint32_t*)((uint8_t*)pMonStats + 0x0C) & 0x0200u) != 0) {
            return g_bHighlightBlue;
        }
    }

    int nLifePct = UNITS_GetCurrentLifePercentage(pUnit);
    if (nLifePct > 66) {
        return g_bHighlightGreen;
    }
    if (nLifePct > 33) {
        return g_bHighlightYellow;
    }
    return g_bHighlightRed;
}

// 1.12a: 0x6FB40E40
int __fastcall GFX_ValidateUnitGfxData(D2UnitStrc* pUnit, D2GfxDataStrc* pGfxData)
{
    if (!pUnit) {
        return 0;
    }
    int nUnitType = *(int*)pUnit;
    int nClassId  = *(int*)((uint8_t*)pUnit + 0x04);
    memset(pGfxData, 0, sizeof(D2GfxDataStrc));
    pGfxData->nUnitType  = nUnitType;
    pGfxData->nClassId   = nClassId;
    pGfxData->nDirection = UNITS_GetDir(pUnit);
    pGfxData->nFrame     = (*(int*)((uint8_t*)pUnit + 0x44)) >> 8;

    switch (nUnitType) {
    case 0: // UNIT_PLAYER
    case 1: // UNIT_MONSTER
    case 2: // UNIT_OBJECT
    {
        if (nUnitType == 2) {
            void* pObjTxt = UNITS_GetObjectTxtRecordFromObject(pUnit);
            if (!pObjTxt) {
                return 0;
            }
            int nObjMode = *(int*)((uint8_t*)pUnit + 0x10);
            if (nObjMode < 0 || nObjMode >= 8 || !*(uint8_t*)((uint8_t*)pObjTxt + 0x13F + nObjMode)) {
                return 0;
            }
        }
        pGfxData->nMode = *(int*)((uint8_t*)pUnit + 0x10);
        D2Client_PopulateUnitComponentGfxNames(pUnit, pGfxData, 1, "hth");
        return D2CMP_SpriteValidate(pGfxData, 1, 0);
    }
    case 3: // UNIT_MISSILE
    {
        uint32_t dwFlags = *(uint32_t*)((uint8_t*)pUnit + 0xC4);
        if (dwFlags & 0x00010000) {
            return 0;
        }
        D2Client_PopulateNonCompositeGfxName(pUnit, pGfxData);
        return D2CMP_SpriteValidate(pGfxData, 1, 0);
    }
    case 4: // UNIT_ITEM
        pGfxData->nMode = *(int*)((uint8_t*)pUnit + 0x10);
        pGfxData->nClassId = ITEMS_GetFileIndex(pUnit);
        D2Client_PopulateNonCompositeGfxName(pUnit, pGfxData);
        return D2CMP_SpriteValidate(pGfxData, 1, 0);
    case 5: // UNIT_TILE
        return 0;
    default:
        FOG_DisplayHalt(Fog_10265(1408), "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 1106);
        exit(-1);
    }
}

// 1.12a: 0x6FB40F60
void __fastcall GFX_FreeUnitAsyncLoads(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        return;
    }
    int nUnitType = *(int*)pUnit;
    int nClassId  = *(int*)((uint8_t*)pUnit + 0x04);
    D2GfxDataStrc tGfxData;
    memset(&tGfxData, 0, sizeof(tGfxData));
    tGfxData.nUnitType = nUnitType;
    tGfxData.nClassId  = nClassId;

    switch (nUnitType) {
    case 0:
    case 1:
    case 2:
    {
        D2CofRecordStrc* pCof = GFX_GetUnitActiveCofRecord(pUnit);
        if (!pCof) {
            return;
        }
        tGfxData.nMode = *(int*)((uint8_t*)pUnit + 0x10);
        for (int i = 0; i < (int)pCof->nLayers; ++i) {
            tGfxData.nComponent = (uint8_t)i;
            D2CMP_SpriteFreeAsyncLoads(&tGfxData);
        }
        return;
    }
    case 3:
    {
        uint32_t dwFlags = *(uint32_t*)((uint8_t*)pUnit + 0xC4);
        if (!(dwFlags & 0x00010000)) {
            D2CMP_SpriteFreeAsyncLoads(&tGfxData);
        }
        return;
    }
    case 4:
        tGfxData.nMode = *(int*)((uint8_t*)pUnit + 0x10);
        tGfxData.nClassId = ITEMS_GetFileIndex(pUnit);
        D2CMP_SpriteFreeAsyncLoads(&tGfxData);
        return;
    case 5:
        return;
    default:
        FOG_DisplayHalt(Fog_10265(1408), "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 1166);
        exit(-1);
    }
}

// 1.12a: 0x6FB41090
// Populates a D2GfxDataStrc for an overlay and validates its sprite.
int __fastcall GFX_SetupOverlayGfxData(D2OverlayStrc* pOverlay, D2GfxDataStrc* pOutGfxData, int nDirection, int bForceLoad)
{
    static const int s_aOverlayDirShift4[8]  = { 0, 0, 1, 1, 2, 2, 3, 3 }; // 0x6FB842E4
    static const int s_aOverlayDirShift16[32] = {                          // 0x6FB84304
        0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7,
        8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15
    };

    memset(pOutGfxData, 0, sizeof(D2GfxDataStrc));
    pOutGfxData->nUnitType  = 6; // Overlay unit type
    pOutGfxData->nOverlayId = pOverlay->nFileIndex;
    if (void* pOverlayTxt = DATATBLS_GetOverlaysTxtRecord(pOverlay->nFileIndex)) {
        pOutGfxData->pName = (char*)pOverlayTxt + 0x02; // D2OverlaysTxt::szFilename
    }

    switch (pOverlay->nDirType) {
    case 0:
        pOutGfxData->nDirection = 0;
        break;
    case 1:
        pOutGfxData->nDirection = s_aOverlayDirShift4[nDirection & 7];
        break;
    case 2:
        pOutGfxData->nDirection = nDirection;
        break;
    case 3:
        pOutGfxData->nDirection = s_aOverlayDirShift16[nDirection & 31];
        break;
    default:
        pOutGfxData->nDirection = 0;
        break;
    }

    pOutGfxData->nFrame = pOverlay->nCurFrameFp >> 8;
    pOutGfxData->dwFlags = pOverlay->dwTrans;
    if (pOverlay->nVersion) {
        pOutGfxData->dwFlags |= 0x04;
    }
    return D2CMP_SpriteValidate(pOutGfxData, 1, bForceLoad);
}

// 1.12a: 0x6FB411D0
// Loads a packed .d2 COF archive file and populates a 512-bucket COF hash table.
// Retail binary record format in cmncof_a1..a5.d2 & chars_cof.d2 (verified at 0x6FB41231..0x6FB412A9):
//   +0x00: uint32_t pNext (0 on disk)
//   +0x04: uint32_t dwFlags (0 on disk)
//   +0x08: uint32_t dwCofSize (exact byte size of raw D2CofFileStrc at +0x18)
//   +0x0C: char     szCofName[8] (7 chars + NUL)
//   +0x14: uint32_t pRawCof (set at runtime to entry + 0x18)
//   +0x18: D2CofFileStrc (dwCofSize bytes)
int __fastcall GFX_LoadPackedCofArchive(D2CofCacheTableStrc* pTable, const char* szArchivePath)
{
    uint32_t hArchive = D2Win_10014();
    DWORD dwSize = 0;
    uint8_t* pBuffer = (uint8_t*)ARCHIVE_LoadFile(hArchive, szArchivePath, &dwSize, 1, 0);
    pTable->pPackedBuffer = pBuffer;
    if (!pBuffer) {
        return 0;
    }

    uint8_t* pEnd = pBuffer + dwSize;
    uint8_t* pCur = pBuffer;
    while (pCur + 0x18 <= pEnd) {
        uint32_t dwCofSize = *(const uint32_t*)(pCur + 0x08);
        if (dwCofSize < sizeof(D2CofFileStrc) || pCur + 0x18 + dwCofSize > pEnd) {
            break;
        }

        const char* szPackedName = (const char*)(pCur + 0x0C);
        const D2CofFileStrc* pRawCof = (const D2CofFileStrc*)(pCur + 0x18);
        if (pRawCof->nDirections <= 0x40 && szPackedName[0] != '\0') {
            D2CofHashNodeStrc* pNode = (D2CofHashNodeStrc*)FOG_AllocClientMemory(
                sizeof(D2CofHashNodeStrc), "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 295, 0);
            memset(pNode, 0, sizeof(D2CofHashNodeStrc));
            memcpy(pNode->szCofName, szPackedName, 8);
            pNode->szCofName[7] = '\0';
            pNode->nRefCount = 0;
            pNode->bDynamicAlloc = 0;

            D2CofRecordStrc* pCofRecord = (D2CofRecordStrc*)FOG_AllocClientMemory(
                sizeof(D2CofRecordStrc), "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 300, 0);
            memset(pCofRecord, 0, sizeof(D2CofRecordStrc));
            pNode->pCofRecord = pCofRecord;
            GFX_InitCofRecord(pCofRecord, pRawCof);

            uint32_t nBucket = GFX_HashCofName(pNode->szCofName);
            pNode->pNext = pTable->pBuckets[nBucket];
            pTable->pBuckets[nBucket] = pNode;
        }

        pCur += 0x18 + dwCofSize;
    }

    if (pCur != pEnd) {
        FOG_DisplayWarning("pbCurr == pbEnd", "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 306);
    }
    return 1;
}

// 1.12a: 0x6FB412E0
D2CofHashNodeStrc* __fastcall GFX_FindCofNodeInCache(D2CofCacheTableStrc* pTable, const char* szCofName)
{
    char szBase[260];
    if (strchr(szCofName, '\\') || strchr(szCofName, '.')) {
        GFX_ExtractBaseCofName(szBase, szCofName);
        szCofName = szBase;
    }
    uint32_t nBucket = GFX_HashCofName(szCofName);
    for (D2CofHashNodeStrc* pNode = pTable->pBuckets[nBucket]; pNode; pNode = pNode->pNext) {
        if (SStrCmpI(szCofName, pNode->szCofName, 8) == 0) {
            return pNode;
        }
    }
    return NULL;
}

// 1.12a: 0x6FB41350
void __cdecl GFX_BuildMonsterCofPath(char* szOutPath, const char* szToken, const char* szMode, const char* szWeaponClass)
{
    wsprintfA(szOutPath, "%s\\%s\\COF\\%s%s%s.COF", "DATA\\GLOBAL\\MONSTERS", szToken, szToken, szMode, szWeaponClass);
}

// 1.12a: 0x6FB41380
D2CofRecordStrc* __fastcall GFX_FindUnitCofByMode(D2UnitStrc* pUnit, int nMode, int nWeaponClass)
{
    if (!pUnit) {
        return NULL;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return NULL;
    }
    for (D2UnitCofLinkStrc* pLink = pGfxData->pCofList; pLink; pLink = pLink->pNext) {
        if (pLink->nMode == nMode && pLink->nWeaponClass == nWeaponClass) {
            if (!pLink->pCofNode) {
                return NULL;
            }
            return pLink->pCofNode->pCofRecord;
        }
    }
    return NULL;
}

// 1.12a: 0x6FB413E0
// Attaches or updates an overlay on a unit's overlay doubly-linked list.
void __fastcall GFX_InsertOverlay(D2UnitStrc* pUnit, D2OverlayStrc* pNewOverlay, int nOverlayId)
{
    if (!pNewOverlay) {
        FOG_DisplayAssert("ptOverlay != NULL", "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 1235);
        exit(-1);
    }
    if (!pUnit) {
        return;
    }
    D2OverlayStrc** ppHead = GFX_GetUnitOverlayListPtr(pUnit);
    if (!ppHead) {
        return;
    }

    D2OverlayStrc* pCur = *ppHead;
    if (pCur) {
        for (; pCur; pCur = pCur->pNext) {
            if (pCur->nOverlayId == nOverlayId) {
                pCur->nCurFrameFp = 0;
                pCur->bLoopWaitDone = (pNewOverlay->nInitWait == 0) ? 1 : 0;
                pCur->dwStartTick = GetTickCount();
                FOG_FreeClientMemory(pNewOverlay, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 1255, 0);
                return;
            }
        }
        pNewOverlay->pNext = *ppHead;
        (*ppHead)->pPrev = pNewOverlay;
    }
    *ppHead = pNewOverlay;
}

// 1.12a: 0x6FB41480
// Populates a D2GfxDataStrc for a unit's composite layer and validates the sprite.
int __fastcall GFX_SetupUnitComponentGfxData(
    D2UnitStrc* pUnit,
    D2GfxDataStrc* pOutGfxData,
    int nComponent,
    int nDirection,
    int nFrame,
    const char* szWeaponClass,
    int bAsync,
    int bForceLoad)
{
    if (!pUnit || nComponent >= 16) {
        return 0;
    }
    int nUnitType = *(int*)pUnit;
    int nClassId  = *(int*)((uint8_t*)pUnit + 0x04);
    memset(pOutGfxData, 0, sizeof(D2GfxDataStrc));
    pOutGfxData->nUnitType  = nUnitType;
    pOutGfxData->nClassId   = nClassId;
    pOutGfxData->nComponent = (uint8_t)nComponent;
    pOutGfxData->nMode      = *(int*)((uint8_t*)pUnit + 0x10);
    pOutGfxData->nDirection = nDirection;
    pOutGfxData->nFrame     = nFrame;
    if (!D2Client_PopulateUnitComponentGfxNames(pUnit, pOutGfxData, nComponent, szWeaponClass)) {
        return 0;
    }
    return D2CMP_SpriteValidate(pOutGfxData, bAsync, bForceLoad);
}

// 1.12a: 0x6FB41510
void __fastcall GFX_UpdateActCofCache(int nAct)
{
    static const char* const s_aszActCofArchives[5] = { // 0x6FB84254
        "DATA\\GLOBAL\\cmncof_a1.d2",
        "DATA\\GLOBAL\\cmncof_a2.d2",
        "DATA\\GLOBAL\\cmncof_a3.d2",
        "DATA\\GLOBAL\\cmncof_a4.d2",
        "DATA\\GLOBAL\\cmncof_a5.d2"
    };
    if (nAct != g_MonsterCofCache.nLoadedActIndex) {
        GFX_FreeMonsterCofCache();
        if (nAct >= 0) {
            GFX_LoadPackedCofArchive(&g_MonsterCofCache, s_aszActCofArchives[nAct]);
            g_MonsterCofCache.nLoadedActIndex = nAct;
        }
    }
    if (g_PlayerCofCache.nLoadedActIndex == -1) {
        GFX_LoadPackedCofArchive(&g_PlayerCofCache, "DATA\\GLOBAL\\chars_cof.d2");
        g_PlayerCofCache.nLoadedActIndex = 1;
    }
}

// 1.12a: 0x6FB415B0
// Resolves unit item/monster component palette remap table.
uint8_t* __fastcall GFX_GetUnitComponentPalette(
    D2UnitStrc* pUnit,
    int nComponent,
    uint8_t* pInOutDrawMode,
    uint8_t* pInOutTransIdx,
    int bDefaultTrans)
{
    uint8_t* pPalette = NULL;
    if (!pUnit) {
        return NULL;
    }
    if (!bDefaultTrans) {
        pPalette = D2CMP_GetPaletteRemapTable(*pInOutDrawMode, *pInOutTransIdx);
    }

    int nUnitType = *(int*)pUnit;
    if (nUnitType == 0) { // UNIT_PLAYER
        uint8_t nStateColor = 0;
        if (D2Common_11037_GetStateColor(pUnit, &nStateColor)) {
            return D2CMP_GetPaletteRemapTable(3, nStateColor);
        }
        D2InventoryStrc* pInv = *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60);
        if (pInv && D2Common_10899_COMPOSIT_IsArmorComponent(nComponent)) {
            for (D2UnitStrc* pItem = INVENTORY_GetItemFromInventory(pInv); pItem; pItem = INVENTORY_GetNextItem(pItem)) {
                if (*(int*)pItem == 4 && ITEMS_CheckNotSocketable(pItem)) {
                    int nBodyLoc = ITEMS_GetBodyLocation(pItem);
                    switch (nBodyLoc) {
                    case 1: // Head
                        if (nComponent == 0) {
                            int nColorIdx = 0;
                            uint8_t* pItemPal = (uint8_t*)ITEMS_GetColor(pUnit, pItem, &nColorIdx, 0);
                            if (pItemPal) return pItemPal;
                        }
                        break;
                    case 3: // Torso
                        if (nComponent == 1 || nComponent == 2 || nComponent == 3 ||
                            nComponent == 4 || nComponent == 7 || nComponent == 8) {
                            int nColorIdx = 0;
                            uint8_t* pItemPal = (uint8_t*)ITEMS_GetColor(pUnit, pItem, &nColorIdx, 1);
                            if (pItemPal) return pItemPal;
                        }
                        break;
                    case 4: // Right Arm
                    case 5: // Left Arm
                    {
                        void* pItemTxt = DATATBLS_GetItemsTxtRecord(*(int*)((uint8_t*)pItem + 0x04));
                        uint8_t nComp = pItemTxt ? *(uint8_t*)((uint8_t*)pItemTxt + 0x115) : 16;
                        if (nComp != 16) {
                            if (nComp == 5 || nComp == 6) {
                                int nWeapClass = 0;
                                D2Common_10269_COMPOSIT(pUnit, &nWeapClass, *(int*)((uint8_t*)pUnit + 0x10), 1, 0);
                                if (nWeapClass != 7 && nWeapClass != 6) {
                                    if (nBodyLoc == 4) nComp = 5;
                                    else if (nBodyLoc == 5) nComp = 6;
                                }
                            }
                            if (nComponent == (int)nComp) {
                                int nColorIdx = 0;
                                uint8_t* pItemPal = (uint8_t*)ITEMS_GetColor(pUnit, pItem, &nColorIdx, 0);
                                if (pItemPal) return pItemPal;
                            }
                        }
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
        }
    } else if (nUnitType == 1) { // UNIT_MONSTER
        uint8_t nStateColor = 0;
        if (D2Common_11037_GetStateColor(pUnit, &nStateColor)) {
            return D2CMP_GetPaletteRemapTable(3, nStateColor);
        }
        int nPalIdx = *(int*)((uint8_t*)pUnit + 0x6C);
        int nMonsterId = *(int*)((uint8_t*)pUnit + 0x04);
        void* pMonTxt = DATATBLS_GetMonsterTxtRecord(nMonsterId);
        uint8_t aMonComps[16];
        MONSTERAI_GetMonsterComponents(pUnit, aMonComps);
        uint8_t nDrawMode = *pInOutDrawMode;
        uint8_t nTransIdx = *pInOutTransIdx;
        uint8_t nCompColor = 0xFF;
        MONSTERS_GetCompInfo(pMonTxt, nComponent, aMonComps[nComponent], &nDrawMode, &nTransIdx, &nCompColor);
        *pInOutDrawMode = nDrawMode;
        *pInOutTransIdx = nTransIdx;

        if (nDrawMode != 3 || nTransIdx != 0) {
            return D2CMP_GetPaletteRemapTable(nDrawMode, nTransIdx);
        }
        if (nCompColor != 0xFF) {
            return D2CMP_GetPaletteRemapTable(3, nCompColor);
        }
        if (nPalIdx >= 2) {
            uint8_t* pMonPal = PALSHIFT_GetMonsterPalette(nMonsterId, nPalIdx);
            if (pMonPal) {
                return pMonPal;
            }
        }
    }
    return pPalette;
}

// 1.12a: 0x6FB417C0
// Loads or looks up a .COF file in the monster/player hash table.
D2CofHashNodeStrc* __fastcall GFX_LoadCofNode(const char* szFolder, const char* szToken, const char* szMode, const char* szWeaponClass, int bIsPlayer)
{
    char szFullPath[260];
    wsprintfA(szFullPath, "%s\\%s\\COF\\%s%s%s.COF", szFolder, szToken, szToken, szMode, szWeaponClass);

    D2CofCacheTableStrc* pTable = bIsPlayer ? &g_PlayerCofCache : &g_MonsterCofCache;
    D2CofHashNodeStrc* pNode = GFX_FindCofNodeInCache(pTable, szFullPath);
    if (pNode) {
        pNode->nRefCount++;
        return pNode;
    }

    uint32_t hArchive = D2Win_10014();
    DWORD dwSize = 0;
    D2CofFileStrc* pRawCof = (D2CofFileStrc*)ARCHIVE_LoadFile(hArchive, szFullPath, &dwSize, 1, 0);
    if (!pRawCof) {
        return NULL;
    }

    pNode = (D2CofHashNodeStrc*)FOG_AllocClientMemory(sizeof(D2CofHashNodeStrc), "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 435, 0);
    memset(pNode, 0, sizeof(D2CofHashNodeStrc));
    GFX_ExtractBaseCofName(pNode->szCofName, szFullPath);
    pNode->nRefCount = 1;
    pNode->bDynamicAlloc = 1;

    uint32_t dwUnpackedSize = sizeof(D2CofRecordStrc) + dwSize;
    D2CofRecordStrc* pCofRecord = (D2CofRecordStrc*)FOG_AllocClientMemory(dwUnpackedSize, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 448, 0);
    memset(pCofRecord, 0, sizeof(D2CofRecordStrc));
    pNode->pCofRecord = pCofRecord;
    D2CofFileStrc* pEmbeddedRawCof = (D2CofFileStrc*)((uint8_t*)pCofRecord + sizeof(D2CofRecordStrc));
    memcpy(pEmbeddedRawCof, pRawCof, dwSize);
    GFX_InitCofRecord(pCofRecord, pEmbeddedRawCof);

    FOG_FreeClientMemory(pRawCof, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 459, 0);

    uint32_t nBucket = GFX_HashCofName(pNode->szCofName);
    pNode->pNext = pTable->pBuckets[nBucket];
    pTable->pBuckets[nBucket] = pNode;
    return pNode;
}

// 1.12a: 0x6FB41910
// Resolves and caches the composite COF record for a unit's mode and weapon class.
void __fastcall GFX_CacheUnitCof(D2UnitStrc* pUnit, int nMode, int nWeaponClass)
{
    if (!pUnit || *(uint32_t*)pUnit > 2) {
        return;
    }
    int nUnitType = *(int*)pUnit;
    int nClassId  = *(int*)((uint8_t*)pUnit + 0x04);

    if (nUnitType == 2) { // UNIT_OBJECT
        void* pObjTxt = UNITS_GetObjectTxtRecordFromObject(pUnit);
        if (!pObjTxt || nMode < 0 || nMode >= 8 || !*(uint8_t*)((uint8_t*)pObjTxt + 0x13F + nMode)) {
            return;
        }
    }

    char szToken[4] = {0};
    char szMode[4]  = {0};
    char szWeap[4]  = {0};
    D2Common_10885_COMPOSIT(pUnit, szToken, szMode, nUnitType, nClassId, nMode, nWeaponClass, szWeap);

    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return;
    }

    for (D2UnitCofLinkStrc* pCur = pGfxData->pCofList; pCur; pCur = pCur->pNext) {
        if (pCur->nMode == nMode && pCur->nWeaponClass == nWeaponClass) {
            return;
        }
    }

    D2UnitCofLinkStrc* pNewLink = (D2UnitCofLinkStrc*)FOG_AllocClientMemory(
        sizeof(D2UnitCofLinkStrc), "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 541, 0);
    pNewLink->nMode = nMode;
    pNewLink->nWeaponClass = nWeaponClass;

    const char* szFolder = "DATA\\GLOBAL\\MONSTERS";
    int bIsPlayer = 0;
    if (nUnitType == 0) {
        szFolder  = "DATA\\GLOBAL\\CHARS";
        bIsPlayer = 1;
    } else if (nUnitType == 1) {
        szFolder = "DATA\\GLOBAL\\MONSTERS";
    } else if (nUnitType == 2) {
        szFolder = "DATA\\GLOBAL\\OBJECTS";
    } else {
        FOG_FreeClientMemory(pNewLink, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 559, 0);
        return;
    }

    pNewLink->pCofNode = GFX_LoadCofNode(szFolder, szToken, szMode, szWeap, bIsPlayer);
    pNewLink->pNext = pGfxData->pCofList;
    pGfxData->pCofList = pNewLink;
}

// 1.12a: 0x6FB41A90
void __fastcall GFX_UpdateUnitCofForCurrentMode(D2UnitStrc* pUnit)
{
    if (!pUnit || *(uint32_t*)pUnit > 2) {
        return;
    }
    int nMode = *(int*)((uint8_t*)pUnit + 0x10);
    int nWeaponClass = UNITS_GetWeaponClass(pUnit);
    GFX_CacheUnitCof(pUnit, nMode, nWeaponClass);
}

// 1.12a: 0x6FB41AC0
void __fastcall GFX_InitObjectCofLayers(D2UnitStrc* pUnit)
{
    if (!pUnit || *(int*)pUnit != 2) {
        return;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return;
    }
    GFX_FreeUnitCofList(pGfxData);
    void* pObjTxt = DATATBLS_GetObjectsTxtRecord(*(int*)((uint8_t*)pUnit + 0x04));
    if (pObjTxt) {
        for (int i = 0; i < 8; ++i) {
            if (*(uint8_t*)((uint8_t*)pObjTxt + 0x13F + i)) {
                GFX_CacheUnitCof(pUnit, i, 0);
            }
        }
    }
    D2CofRecordStrc* pCof = GFX_GetUnitActiveCofRecord(pUnit);
    if (pCof) {
        for (int i = 0; i < 16; ++i) {
            if (pCof->aCompToLayer[i] != 0xFF) {
                *((uint8_t*)pGfxData + i) = 1;
            }
        }
    }
}

// 1.12a: 0x6FB41BA0
void __fastcall GFX_InitMonsterCofLayers(D2UnitStrc* pUnit)
{
    if (!pUnit || *(int*)pUnit != 1) {
        return;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return;
    }
    GFX_FreeUnitCofList(pGfxData);
    int nWeaponClass = UNITS_GetWeaponClass(pUnit);
    void* pMonStats2 = UNITS_GetMonStats2TxtRecordFromMonsterId(*(int*)((uint8_t*)pUnit + 0x04));
    uint32_t dwModeFlags = pMonStats2 ? *(uint32_t*)((uint8_t*)pMonStats2 + 0xF0) : 0;
    for (int i = 0; i < 16; ++i) {
        if ((dwModeFlags & (1u << i)) != 0) {
            GFX_CacheUnitCof(pUnit, i, nWeaponClass);
        }
    }
    D2CofRecordStrc* pCof = GFX_GetUnitActiveCofRecord(pUnit);
    if (pCof) {
        for (int i = 0; i < 16; ++i) {
            if (pCof->aCompToLayer[i] != 0xFF) {
                *((uint8_t*)pGfxData + i) = 1;
            }
        }
    }
}

// 1.12a: 0x6FB41C60
// Updates player composite equipment layers and weapon class COF links.
void __fastcall GFX_UpdatePlayerCompositeEquipment(D2UnitStrc* pUnit, int bForce)
{
    if (!pUnit || *(int*)pUnit != 0) {
        return;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return;
    }
    int nWeaponClass = 0;
    D2Common_10269_COMPOSIT(pUnit, &nWeaponClass, *(int*)((uint8_t*)pUnit + 0x10), bForce, 0);
    GFX_FreeUnitCofList(pGfxData);
    for (int i = 0; i < 20; ++i) {
        if (i != 0 && i != 12 && i != 19) {
            GFX_CacheUnitCof(pUnit, i, nWeaponClass);
        }
    }
}

// 1.12a: 0x6FB41EA0
void __fastcall GFX_UpdateMonsterCompositeComponents(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        return;
    }
    if (!MONSTER_HasComponents(pUnit)) {
        GFX_InitMonsterCofLayers(pUnit);
        return;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return;
    }
    void* pMonStats2 = UNITS_GetMonStats2TxtRecordFromMonsterId(*(int*)((uint8_t*)pUnit + 0x04));
    if (!pMonStats2) {
        return;
    }
    GFX_FreeUnitCofList(pGfxData);
    int nWeaponClass = UNITS_GetWeaponClass(pUnit);
    uint32_t dwModeFlags = *(uint32_t*)((uint8_t*)pMonStats2 + 0xF0);
    for (int i = 0; i < 16; ++i) {
        if ((dwModeFlags & (1u << i)) != 0) {
            if (i == 0 || i == 12) {
                GFX_CacheUnitCof(pUnit, i, 0);
            } else {
                GFX_CacheUnitCof(pUnit, i, nWeaponClass);
            }
        }
    }
    D2CofRecordStrc* pCof = GFX_GetUnitActiveCofRecord(pUnit);
    if (pCof) {
        for (int i = 0; i < 16; ++i) {
            if (pCof->aCompToLayer[i] != 0xFF) {
                *((uint8_t*)pGfxData + i) = 1;
            }
        }
    }
    for (D2UnitCofLinkStrc* pCur = pGfxData->pCofList; pCur != nullptr; pCur = pCur->pNext) {
        if (pCur->pCofNode) {
            D2CofRecordStrc* pModeCof = pCur->pCofNode->pCofRecord;
            if (pModeCof) {
                for (int i = 0; i < 16; ++i) {
                    if (pModeCof->aCompToLayer[i] != 0xFF) {
                        *((uint8_t*)pGfxData + i) = 1;
                    }
                }
            }
        }
    }
}

// 1.12a: 0x6FB42000
void __fastcall GFX_InitUnitCompositeGfx(D2UnitStrc* pUnit)
{
    if (!pUnit || *(uint32_t*)pUnit > 2) {
        return;
    }
    if (*(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54) == NULL) {
        *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54) = GFX_AllocUnitGfxData();
    }
    int nUnitType = *(int*)pUnit;
    if (nUnitType == 0) {
        GFX_UpdatePlayerCompositeEquipment(pUnit, 1);
    } else if (nUnitType == 1) {
        if (MONSTER_HasComponents(pUnit)) {
            GFX_UpdateMonsterCompositeComponents(pUnit);
        } else {
            GFX_InitMonsterCofLayers(pUnit);
        }
    } else if (nUnitType == 2) {
        GFX_InitObjectCofLayers(pUnit);
    }
}

// 1.12a: 0x6FB420D0
// Returns current animation frame count and active COF record for a unit.
D2CofRecordStrc* __fastcall GFX_GetUnitFrameInfo(D2UnitStrc* pUnit, int* pOutFrameCount, int* pOutAnimRate)
{
    *pOutFrameCount = 1;
    *pOutAnimRate = 256;
    if (!pUnit || *(uint32_t*)pUnit > 2) {
        return NULL;
    }
    D2CofRecordStrc* pCof = GFX_FindUnitCofByMode(pUnit, *(int*)((uint8_t*)pUnit + 0x10), UNITS_GetWeaponClass(pUnit));
    if (!pCof) {
        GFX_UpdateUnitCofForCurrentMode(pUnit);
        pCof = GFX_FindUnitCofByMode(pUnit, *(int*)((uint8_t*)pUnit + 0x10), UNITS_GetWeaponClass(pUnit));
    }
    if (pCof) {
        *pOutFrameCount = pCof->nFramesPerDir;
        *pOutAnimRate   = pCof->nAnimRate;
    }
    return pCof;
}

// 1.12a: 0x6FB421B0
// Checks if a unit's COF bounding box intersects the screen viewport.
int __fastcall GFX_IsUnitInViewport(D2UnitStrc* pUnit)
{
    if (!pUnit) {
        return 0;
    }
    int nClientX = UNITS_GetClientCoordX(pUnit);
    int nClientY = UNITS_GetClientCoordY(pUnit);
    int nScreenX = nClientX - LIGHTMAP_GetViewportShiftX();
    int nScreenY = nClientY - LIGHTMAP_GetViewportShiftY();

    int nXMin = -64, nXMax = 64, nYMin = -128, nYMax = 32;
    D2CofRecordStrc* pCof = GFX_GetUnitActiveCofRecord(pUnit);
    if (pCof) {
        nXMin = pCof->nXMin;
        nXMax = pCof->nXMax;
        nYMin = pCof->nYMin;
        nYMax = pCof->nYMax;
    }

    if (nScreenX + nXMax < 0 || nScreenX + nXMin > g_nScreenWidth) {
        return 0;
    }
    if (nScreenY + nYMax < 0 || nScreenY + nYMin > g_nScreenHeight) {
        return 0;
    }
    return 1;
}

// 1.12a: 0x6FB42330
// Checks if a screen coordinate (nMouseX, nMouseY) intersects a unit's COF bounding box.
int __fastcall GFX_HitTestUnitCofBox(D2UnitStrc* pUnit, int nMouseX, int nMouseY, int nUnitScreenX, int nUnitScreenY)
{
    if (!pUnit) {
        return 0;
    }
    D2CofRecordStrc* pCof = GFX_GetUnitActiveCofRecord(pUnit);
    if (!pCof) {
        return 0;
    }
    int nLeft   = nUnitScreenX + pCof->nXMin - 16;
    int nRight  = nUnitScreenX + pCof->nXMax + 16;
    int nTop    = nUnitScreenY + pCof->nYMin - 16;
    int nBottom = nUnitScreenY + pCof->nYMax + 16;

    return (nMouseX >= nLeft && nMouseX <= nRight && nMouseY >= nTop && nMouseY <= nBottom) ? 1 : 0;
}

// 1.12a: 0x6FB42490
// Retrieves the bounding cell offset of a single-layer unit.
int __fastcall GFX_GetSingleLayerUnitCelOffset(D2UnitStrc* pUnit, int* pOutOffsetX, int* pOutOffsetY)
{
    *pOutOffsetX = 0;
    *pOutOffsetY = 0;
    D2GfxDataStrc tGfxData;
    if (!GFX_ValidateUnitGfxData(pUnit, &tGfxData)) {
        return 0;
    }
    if (!tGfxData.pCell) {
        return 0;
    }
    *pOutOffsetX = D2CMP_CelGetOffsetX(tGfxData.pCell);
    *pOutOffsetY = D2CMP_CelGetOffsetY(tGfxData.pCell);
    return 1;
}

// 1.12a: 0x6FB42640
// Draws selection highlight outline for a single-layer unit.
void __fastcall GFX_DrawSingleLayerUnitHighlight(D2UnitStrc* pUnit, int nXPos, int nYPos, uint8_t nPaletteIndex)
{
    D2GfxDataStrc tGfxData;
    if (GFX_ValidateUnitGfxData(pUnit, &tGfxData)) {
        D2Gfx_10012_CelDrawHilight(&tGfxData, nXPos, nYPos, nPaletteIndex);
    }
}

// 1.12a: 0x6FB42810
// Draws ground shadow for a single-layer unit.
void __fastcall GFX_DrawSingleLayerUnitShadow(D2UnitStrc* pUnit, int nXPos, int nYPos)
{
    D2GfxDataStrc tGfxData;
    if (GFX_ValidateUnitGfxData(pUnit, &tGfxData)) {
        D2Gfx_10030_CelDrawShadow(&tGfxData, nXPos, nYPos);
    }
}

// 1.12a: 0x6FB42A40
// Asynchronously pre-loads all COF layers of a unit.
int __fastcall GFX_AsyncPreloadUnitSprites(D2UnitStrc* pUnit, int bForceLoad)
{
    if (!pUnit) {
        return 0;
    }
    D2CofRecordStrc* pCof = GFX_GetUnitActiveCofRecord(pUnit);
    if (!pCof) {
        return 0;
    }
    D2GfxDataStrc tGfxData;
    for (int i = 0; i < (int)pCof->nLayers; ++i) {
        uint8_t nComp = pCof->pLayers[i].nComponent;
        GFX_SetupUnitComponentGfxData(pUnit, &tGfxData, nComp, 0, 0, pCof->pLayers[i].szWeaponClass, 1, bForceLoad);
    }
    return 1;
}

// 1.12a: 0x6FB42CC0
// Draws overlays attached to a unit (either pre-draw or post-draw pass).
void __fastcall GFX_DrawUnitOverlays(D2UnitStrc* pUnit, int nXPos, int nYPos, uint32_t dwGamma, int bPreDrawPass, int nScreenMode)
{
    if (!pUnit) {
        return;
    }
    D2OverlayStrc* pCur = GFX_GetUnitOverlayList(pUnit);
    if (!pCur) {
        return;
    }
    int nUnitDir = UNITS_GetDir(pUnit);
    for (; pCur; pCur = pCur->pNext) {
        if (pCur->bPreDraw == bPreDrawPass && pCur->bLoopWaitDone) {
            D2GfxDataStrc tGfxData;
            if (GFX_SetupOverlayGfxData(pCur, &tGfxData, nUnitDir, 0)) {
                int nDrawX = nXPos + pCur->nXOffset;
                int nDrawY = nYPos + pCur->nYOffset + pCur->nHeightOffset;
                uint8_t* pPal = D2CMP_GetPaletteRemapTable(pCur->nDrawMode, pCur->nTransIndex);
                if (nScreenMode) {
                    TEXTURE_CelFlatSpriteDraw(&tGfxData, nDrawX, nDrawY, dwGamma, pCur->nDrawMode, nScreenMode, pPal);
                } else {
                    D2Gfx_10024_CelDraw(&tGfxData, nDrawX, nDrawY, dwGamma, pCur->nDrawMode, pPal);
                }
            }
        }
    }
}

// 1.12a: 0x6FB42F70
// Draws selection highlight for active unit overlays.
void __fastcall GFX_DrawUnitOverlayHighlights(D2UnitStrc* pUnit, int nXPos, int nYPos, uint8_t nPaletteIndex)
{
    if (!pUnit) {
        return;
    }
    D2OverlayStrc* pCur = GFX_GetUnitOverlayList(pUnit);
    if (!pCur) {
        return;
    }
    int nUnitDir = UNITS_GetDir(pUnit);
    for (; pCur; pCur = pCur->pNext) {
        if (pCur->bLoopWaitDone) {
            D2GfxDataStrc tGfxData;
            if (GFX_SetupOverlayGfxData(pCur, &tGfxData, nUnitDir, 0)) {
                int nDrawX = nXPos + pCur->nXOffset;
                int nDrawY = nYPos + pCur->nYOffset + pCur->nHeightOffset;
                D2Gfx_10012_CelDrawHilight(&tGfxData, nDrawX, nDrawY, nPaletteIndex);
            }
        }
    }
}

// 1.12a: 0x6FB430E0
// Creates and attaches an overlay from Overlays.txt onto a unit.
void __fastcall GFX_AttachOverlayToUnit(D2UnitStrc* pUnit, int nOverlayId, int bPreDrawOverride)
{
    if (!pUnit || nOverlayId < 0) {
        return;
    }
    void* pOverlayTxt = DATATBLS_GetOverlaysTxtRecord(nOverlayId);
    if (!pOverlayTxt) {
        return;
    }
    D2OverlayStrc* pOverlay = (D2OverlayStrc*)FOG_AllocClientMemory(
        sizeof(D2OverlayStrc), "..\\Source\\D2Client\\ENGINE\\Gfx.cpp", 1300, 0);
    memset(pOverlay, 0, sizeof(D2OverlayStrc));

    pOverlay->nOverlayId   = nOverlayId;
    pOverlay->nFileIndex   = *(uint16_t*)((uint8_t*)pOverlayTxt + 0x00);
    pOverlay->nVersion     = *(uint16_t*)((uint8_t*)pOverlayTxt + 0x42);
    pOverlay->nFrames      = *(int*)((uint8_t*)pOverlayTxt + 0x44);
    pOverlay->bPreDraw     = bPreDrawOverride ? 1 : *(uint8_t*)((uint8_t*)pOverlayTxt + 0x48);
    pOverlay->nDirType     = *(uint8_t*)((uint8_t*)pOverlayTxt + 0x50);
    pOverlay->nXOffset     = *(int*)((uint8_t*)pOverlayTxt + 0x54);
    pOverlay->nYOffset     = *(int*)((uint8_t*)pOverlayTxt + 0x58);
    pOverlay->nAnimRate    = *(int*)((uint8_t*)pOverlayTxt + 0x6C);
    pOverlay->nInitWait    = *(int*)((uint8_t*)pOverlayTxt + 0x78);
    pOverlay->nDrawMode    = *(uint8_t*)((uint8_t*)pOverlayTxt + 0x7C);
    pOverlay->nTransIndex  = 0;
    pOverlay->nEndFrameFp  = pOverlay->nFrames << 8;
    pOverlay->nStepFp      = pOverlay->nAnimRate;
    pOverlay->dwStartTick  = GetTickCount();
    pOverlay->bLoopWaitDone = (pOverlay->nInitWait == 0) ? 1 : 0;
    pOverlay->nLightIndex  = LIGHTMAP_AttachOverlayLight(pUnit, nOverlayId);

    GFX_InsertOverlay(pUnit, pOverlay, nOverlayId);
}

// 1.12a: 0x6FB43380
// Renders a ground item unit (including gold pile amounts, ethereal translucency, and item color remap).
void __fastcall GFX_DrawItemUnit(D2UnitStrc* pItem, int nXPos, int nYPos, uint32_t dwGamma, int nScreenMode)
{
    if (!pItem) {
        return;
    }
    int nItemMode = *(int*)((uint8_t*)pItem + 0x10);
    if (nScreenMode != 0 || (nItemMode != 3 && nItemMode != 5)) {
        if (D2Client_DrawInvItemSprite(pItem, nXPos, nYPos, dwGamma, 5)) {
            return;
        }
    }
    D2GfxDataStrc tGfxData;
    if (!GFX_ValidateUnitGfxData(pItem, &tGfxData)) {
        return;
    }
    int nColorIdx = 0;
    uint8_t* pPal = (uint8_t*)ITEMS_GetColor(NULL, pItem, &nColorIdx, 0);
    int nDrawMode = 5;
    if (ITEMS_CheckItemFlag(pItem, 0x00400000, 1480, "..\\Source\\D2Client\\ENGINE\\Gfx.cpp")) { // Ethereal
        nDrawMode = 1;
    }
    GFX_DrawUnitOverlays(pItem, nXPos, nYPos, dwGamma, 1, nScreenMode);
    if (nScreenMode) {
        TEXTURE_CelFlatSpriteDraw(&tGfxData, nXPos, nYPos, dwGamma, nDrawMode, nScreenMode, pPal);
    } else {
        D2Gfx_10024_CelDraw(&tGfxData, nXPos, nYPos, dwGamma, nDrawMode, pPal);
    }
    GFX_DrawUnitOverlays(pItem, nXPos, nYPos, dwGamma, 0, nScreenMode);
}

// 1.12a: 0x6FB434E0
// Draws composite multi-layer unit selection highlight in COF priority order.
void __fastcall GFX_DrawCompositeUnitHighlight(D2UnitStrc* pUnit, int nXPos, int nYPos, uint8_t nPaletteIndex)
{
    if (!pUnit) {
        return;
    }
    D2CofRecordStrc* pCof = GFX_GetUnitActiveCofRecord(pUnit);
    if (!pCof) {
        return;
    }
    int nDir   = UNITS_GetDir(pUnit);
    int nFrame = (*(int*)((uint8_t*)pUnit + 0x44)) >> 8;
    if (nFrame < 0 || nFrame >= (int)pCof->nFramesPerDir) {
        nFrame = 0;
    }

    int nCofDir = GFXUTIL_EngineDirToCofDir(nDir, pCof->nDirections);

    uint8_t* pOrder = pCof->pLayerPriority + ((nCofDir * pCof->nFramesPerDir + nFrame) * pCof->nLayers);
    D2GfxDataStrc tGfxData;
    for (int i = 0; i < (int)pCof->nLayers; ++i) {
        uint8_t nComp = pOrder[i];
        uint8_t nLayerIdx = pCof->aCompToLayer[nComp & 15];
        if (nLayerIdx != 0xFF && pCof->pLayers[nLayerIdx].bSelectable) {
            if (GFX_SetupUnitComponentGfxData(pUnit, &tGfxData, nComp, nCofDir, nFrame, pCof->pLayers[nLayerIdx].szWeaponClass, 1, 0)) {
                D2Gfx_10012_CelDrawHilight(&tGfxData, nXPos, nYPos, nPaletteIndex);
            }
        }
    }
    GFX_DrawUnitOverlayHighlights(pUnit, nXPos, nYPos, nPaletteIndex);
}

// 1.12a: 0x6FB43860
// Draws composite multi-layer unit ground shadows in COF priority order.
void __fastcall GFX_DrawCompositeUnitShadow(D2UnitStrc* pUnit, int nXPos, int nYPos)
{
    if (!pUnit) {
        return;
    }
    D2CofRecordStrc* pCof = GFX_GetUnitActiveCofRecord(pUnit);
    if (!pCof) {
        return;
    }
    int nDir   = UNITS_GetDir(pUnit);
    int nFrame = (*(int*)((uint8_t*)pUnit + 0x44)) >> 8;
    if (nFrame < 0 || nFrame >= (int)pCof->nFramesPerDir) {
        nFrame = 0;
    }

    int nCofDir = GFXUTIL_EngineDirToCofDir(nDir, pCof->nDirections);

    uint8_t* pOrder = pCof->pLayerPriority + ((nCofDir * pCof->nFramesPerDir + nFrame) * pCof->nLayers);
    D2GfxDataStrc tGfxData;
    for (int i = 0; i < (int)pCof->nLayers; ++i) {
        uint8_t nComp = pOrder[i];
        uint8_t nLayerIdx = pCof->aCompToLayer[nComp & 15];
        if (nLayerIdx != 0xFF && pCof->pLayers[nLayerIdx].bShadow) {
            if (GFX_SetupUnitComponentGfxData(pUnit, &tGfxData, nComp, nCofDir, nFrame, pCof->pLayers[nLayerIdx].szWeaponClass, 1, 0)) {
                D2Gfx_10030_CelDrawShadow(&tGfxData, nXPos, nYPos);
            }
        }
    }
}

// 1.12a: 0x6FB44250
// Main composite multi-layer unit renderer (UNIT_PLAYER, UNIT_MONSTER, UNIT_OBJECT).
void __fastcall GFX_DrawCompositeUnit(D2UnitStrc* pUnit, int nXPos, int nYPos, uint32_t dwGamma, int nScreenMode)
{
    if (!pUnit) {
        return;
    }
    D2CofRecordStrc* pCof = GFX_GetUnitActiveCofRecord(pUnit);
    if (!pCof) {
        return;
    }

    GFX_DrawUnitOverlays(pUnit, nXPos, nYPos, dwGamma, 1, nScreenMode);

    int nDir   = UNITS_GetDir(pUnit);
    int nFrame = (*(int*)((uint8_t*)pUnit + 0x44)) >> 8;
    if (nFrame < 0 || nFrame >= (int)pCof->nFramesPerDir) {
        nFrame = 0;
    }

    int nCofDir = GFXUTIL_EngineDirToCofDir(nDir, pCof->nDirections);

    uint8_t* pOrder = pCof->pLayerPriority + ((nCofDir * pCof->nFramesPerDir + nFrame) * pCof->nLayers);
    D2GfxDataStrc tGfxData;

    for (int i = 0; i < (int)pCof->nLayers; ++i) {
        uint8_t nComp = pOrder[i];
        uint8_t nLayerIdx = pCof->aCompToLayer[nComp & 15];
        if (nLayerIdx == 0xFF) {
            continue;
        }
        const D2CofLayerStrc* pLayer = &pCof->pLayers[nLayerIdx];
        if (!GFX_SetupUnitComponentGfxData(pUnit, &tGfxData, nComp, nCofDir, nFrame, pLayer->szWeaponClass, 1, 0)) {
            continue;
        }

        uint8_t nDrawMode = pLayer->nOverrideTranslvl ? pLayer->nNewTranslvl : 5;
        uint8_t nTransIdx = 0;
        uint8_t* pPal = GFX_GetUnitComponentPalette(pUnit, nComp, &nDrawMode, &nTransIdx, 1);

        if (STATES_CheckStateMaskGreenOnUnit(pUnit)) {
            pPal = D2CMP_GetPaletteRemapTable(3, 12);
        }

        if (nScreenMode) {
            TEXTURE_CelFlatSpriteDraw(&tGfxData, nXPos, nYPos, dwGamma, nDrawMode, nScreenMode, pPal);
        } else {
            D2Gfx_10024_CelDraw(&tGfxData, nXPos, nYPos, dwGamma, nDrawMode, pPal);
        }
    }

    GFX_DrawUnitOverlays(pUnit, nXPos, nYPos, dwGamma, 0, nScreenMode);
}

// 1.12a: 0x6FB448F0
void __fastcall GFX_PrecacheRoomMonsters(D2ActiveRoomStrc* pRoom)
{
    if (!pRoom || D2CMP_SpriteCacheGetAsyncLoadingCount() >= 30) {
        return;
    }
    DWORD dwNow = GetTickCount();
    if (dwNow - g_dwLastAsyncPrecacheTick < 600) {
        return;
    }
    g_dwLastAsyncPrecacheTick = dwNow;

    for (D2UnitStrc* pUnit = DUNGEON_GetFirstUnitInRoom(pRoom); pUnit; pUnit = *(D2UnitStrc**)((uint8_t*)pUnit + 0xE8)) {
        if (*(int*)pUnit == 1) {
            GFX_AsyncPreloadUnitSprites(pUnit, 0);
        }
    }
}

// 1.12a: 0x6FB449C0
// Top-level unit drawing dispatcher (handles composite units, items, missiles, highlights, and debug boxes).
void __fastcall GFX_DrawUnit(D2UnitStrc* pUnit, uint32_t dwGamma, int nXPos, int nYPos, int bDrawHighlight, int nScreenMode)
{
    if (!pUnit) {
        return;
    }
    int nUnitType = *(int*)pUnit;
    if (bDrawHighlight) {
        uint8_t nHighlightColor = GFX_GetUnitHighlightPaletteColor(pUnit);
        if (nUnitType == 0 || nUnitType == 1 || nUnitType == 2) {
            GFX_DrawCompositeUnitHighlight(pUnit, nXPos, nYPos, nHighlightColor);
        } else {
            GFX_DrawSingleLayerUnitHighlight(pUnit, nXPos, nYPos, nHighlightColor);
        }
    }

    switch (nUnitType) {
    case 0: // UNIT_PLAYER
    case 1: // UNIT_MONSTER
    case 2: // UNIT_OBJECT
        GFX_DrawCompositeUnit(pUnit, nXPos, nYPos, dwGamma, nScreenMode);
        break;
    case 3: // UNIT_MISSILE
        if (!MISSILE_DrawSpecial(pUnit)) {
            D2GfxDataStrc tGfxData;
            if (GFX_ValidateUnitGfxData(pUnit, &tGfxData)) {
                uint8_t nDrawMode = 5;
                uint8_t nTransIdx = 0;
                D2Common_10862_GetMissileTrans(*(int*)((uint8_t*)pUnit + 0x04), &nDrawMode, &nTransIdx);
                uint8_t* pPal = D2CMP_GetPaletteRemapTable(nDrawMode, nTransIdx);
                D2Gfx_10024_CelDraw(&tGfxData, nXPos, nYPos, dwGamma, nDrawMode, pPal);
            }
        }
        break;
    case 4: // UNIT_ITEM
        GFX_DrawItemUnit(pUnit, nXPos, nYPos, dwGamma, nScreenMode);
        break;
    default:
        break;
    }
}


// ============================================================================
// PART 2: ENGINE/GfxUtil.cpp (0x6FB668E0 .. 0x6FB68230)
// ============================================================================

// 1.12a: 0x6FB668E0
// Initializes the 32-direction and 16-direction isometric-to-COF direction lookup tables.
void __cdecl GFXUTIL_InitDirectionTables()
{
    static const int s_aInit32[32] = {
        8,  9, 10, 11, 12, 13, 14, 15,
       16, 17, 18, 19, 20, 21, 22, 23,
       24, 25, 26, 27, 28, 29, 30, 31,
        0,  1,  2,  3,  4,  5,  6,  7
    };
    static const int s_aInit16[16] = {
        4,  5,  6,  7,  8,  9, 10, 11,
       12, 13, 14, 15,  0,  1,  2,  3
    };
    memcpy(g_aDirToCofDir32, s_aInit32, sizeof(g_aDirToCofDir32));
    memcpy(g_aDirToCofDir16, s_aInit16, sizeof(g_aDirToCofDir16));
}

// 1.12a: 0x6FB66950
// Converts an engine direction (0..63) to a COF sprite direction for the given direction count (1, 4, 8, 16, 32).
int __fastcall GFXUTIL_EngineDirToCofDir(int nEngineDir, int nCofDirections)
{
    switch (nCofDirections) {
    case 1:
        return 0;
    case 4:
        return g_aDirToCofDir16[(nEngineDir >> 2) & 15] >> 2;
    case 8:
        return g_aDirToCofDir16[(nEngineDir >> 2) & 15] >> 1;
    case 16:
        return g_aDirToCofDir16[(nEngineDir >> 2) & 15];
    case 32:
        return g_aDirToCofDir32[(nEngineDir >> 1) & 31];
    default:
        return 0;
    }
}

// 1.12a: 0x6FB679A0
// Populates a D2GfxDataStrc for a unit's specific component, resolving player/monster equipment codes and COF name.
int __fastcall GFXUTIL_PopulateComponentGfxData(
    D2UnitStrc* pUnit,
    D2CofRecordStrc* pCof,
    D2GfxDataStrc* pOutGfxData,
    int nComponent,
    int nDirection,
    int nFrame)
{
    if (!pUnit || !pCof || !pOutGfxData || nComponent >= 16) {
        return 0;
    }
    uint8_t nLayerIdx = pCof->aCompToLayer[nComponent];
    if (nLayerIdx == 0xFF) {
        return 0;
    }
    D2UnitGfxDataStrc* pGfxData = *(D2UnitGfxDataStrc**)((uint8_t*)pUnit + 0x54);
    if (!pGfxData) {
        return 0;
    }

    memset(pOutGfxData, 0, sizeof(D2GfxDataStrc));
    pOutGfxData->nUnitType      = *(int*)pUnit;
    pOutGfxData->nClassId       = *(int*)((uint8_t*)pUnit + 0x04);
    pOutGfxData->nComponent     = (uint8_t)nComponent;
    pOutGfxData->nMode          = *(int*)((uint8_t*)pUnit + 0x10);
    pOutGfxData->nDirection     = GFXUTIL_EngineDirToCofDir(nDirection, pCof->nDirections);
    pOutGfxData->nFrame         = nFrame;
    pOutGfxData->nMaxDirections = pCof->nDirections;
    pOutGfxData->nMaxFrames     = pCof->nFramesPerDir;

    return D2Client_PopulateUnitComponentGfxNames(pUnit, pOutGfxData, nComponent, pCof->pLayers[nLayerIdx].szWeaponClass);
}

// 1.12a: 0x6FB68130
// Computes the topmost Y screen coordinate across all valid COF layers of a unit for overhead hover text placement.
int __stdcall GFXUTIL_GetUnitTopExtentY(D2CofRecordStrc* pCof, D2UnitStrc* pUnit, int* pOutTopY, int nDirection, int nFrame)
{
    if (!pCof || !pUnit || !pOutTopY) {
        return 0;
    }
    int nValidLayers = 0;
    D2GfxDataStrc tGfxData;
    for (int i = 0; i < (int)pCof->nLayers; ++i) {
        uint8_t nComp = pCof->pLayers[i].nComponent;
        if (GFXUTIL_PopulateComponentGfxData(pUnit, pCof, &tGfxData, nComp, nDirection, nFrame)) {
            if (D2CMP_SpriteValidate(&tGfxData, 0, 1) && tGfxData.pCell) {
                int nCelOffsetY = D2CMP_CelGetOffsetY(tGfxData.pCell) - (g_nUnitHoverPaddingY - 8);
                int nExtentY = nCelOffsetY + UNITS_GetClientCoordY(pUnit);
                if (nExtentY > *pOutTopY) {
                    *pOutTopY = nExtentY;
                }
                nValidLayers++;
            }
        }
    }
    return nValidLayers;
}
