// ============================================================================
// Sound_SoundHdr.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll
// Address range: 0x6FB18030 .. 0x6FB1CE20 (56 functions)
//
// Original source paths found in binary:
//   - ..\Source\D2Client\Engine\Floor.cpp        (0x6FB18030..0x6FB19080)
//   - ..\Source\D2Client\Sound\SoundHdr.cpp      (0x6FB19090..0x6FB1A750)
//   - ..\Source\D2Client\UI\CharStat.cpp         (0x6FB1A760..0x6FB1C3A0)
//   - ..\Source\D2Client\WARDEN\WardenClient.cpp (0x6FB1C3A0..0x6FB1CE20)
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// ----------------------------------------------------------------------------
// Forward Declarations & Engine Structures
// ----------------------------------------------------------------------------
struct D2UnitStrc;
struct D2ActiveRoomStrc;
struct D2RoomTileStrc;
struct D2TileLibraryEntryStrc;
struct D2CellFileStrc;
struct D2GfxDataStrc;
struct D2StatListStrc;
struct D2CharStatsTxt;
struct D2BinFieldStrc;

// Sound sample cache node (0x38 bytes, allocated in Sound\SoundHdr.cpp)
struct D2SoundSampleNodeStrc
{
    uint32_t dwByteSize;                 // +0x00
    uint8_t* pSampleData;                // +0x04
    char szFileName[32];                 // +0x08
    int32_t nLockCount;                  // +0x28
    D2SoundSampleNodeStrc* pPrevCost;    // +0x2C
    D2SoundSampleNodeStrc* pNextCost;    // +0x30
    void* hAsyncRequest;                 // +0x34
};

// Sounds.txt client stub record (0x8E = 142 bytes per entry)
#pragma pack(push, 1)
struct D2SoundsTxtStubStrc
{
    uint16_t wIndex;                     // +0x00
    char szFileName[60];                 // +0x02
    uint32_t dwVolume;                   // +0x3E
    uint8_t nGroupSize;                  // +0x42
    uint8_t nLoop;                       // +0x43
    uint32_t dwFadeIn;                   // +0x44
    uint32_t dwFadeOut;                  // +0x48
    uint8_t bDeferInst;                  // +0x4C
    uint8_t bStopInst;                   // +0x4D
    uint32_t dwDuration;                 // +0x4E
    uint16_t wCompound;                  // +0x52
    uint32_t dwReverb;                   // +0x54
    uint32_t dwFalloff;                  // +0x58
    uint8_t bCache;                      // +0x5C
    uint8_t bAsyncOnly;                  // +0x5D
    uint8_t nPriority;                   // +0x5E
    uint8_t bStream;                     // +0x5F
    uint8_t bStereo;                     // +0x60
    uint8_t bTracking;                   // +0x61
    uint8_t bSolo;                       // +0x62
    uint8_t bMusicVol;                   // +0x63
    uint32_t dwBlock1;                   // +0x64
    uint32_t dwBlock2;                   // +0x68
    uint32_t dwBlock3;                   // +0x6C
    uint32_t dwLastPlayTick;             // +0x70
    uint32_t dwHistoryIdx[4];            // +0x74..0x83
    uint8_t nHistoryPos;                 // +0x84
    uint8_t pad85[5];                    // +0x85..0x89
    D2SoundSampleNodeStrc* pCacheNode;   // +0x8A
};

// SoundEnviron.txt record (0x58 = 88 bytes per entry)
struct D2SoundEnvironTxtStrc
{
    uint16_t wIndex;                     // +0x00
    uint16_t pad02;                      // +0x02
    int32_t nSong;                       // +0x04
    int32_t nDayAmbience;                // +0x08
    int32_t nNightAmbience;              // +0x0C
    int32_t nDayEvent;                   // +0x10
    int32_t nNightEvent;                 // +0x14
    uint32_t dwEventDelay;               // +0x18
    int32_t bIndoors;                    // +0x1C
    int32_t nMaterial1;                  // +0x20
    int32_t nMaterial2;                  // +0x24
    int32_t nEaxEnviron;                 // +0x28
    int32_t nEaxEnvSize;                 // +0x2C
    int32_t nEaxEnvDiff;                 // +0x30
    int32_t nEaxRoomVol;                 // +0x34
    int32_t nEaxRoomHF;                  // +0x38
    int32_t nEaxDecayTime;               // +0x3C
    int32_t nEaxDecayHF;                 // +0x40
    int32_t nEaxReflect;                 // +0x44
    int32_t nEaxReflectDelay;            // +0x48
    int32_t nEaxReverb;                  // +0x4C
    int32_t nEaxRevDelay;                // +0x50
    int32_t nEaxRoomRoll;                // +0x54
};
#pragma pack(pop)

// Floor tile light/perspective vertex (0x08 bytes)
struct D2FloorTileVertexStrc
{
    int16_t nScreenX;                    // +0x00
    int16_t nScreenY;                    // +0x02
    union
    {
        uint32_t dwPackedRGB;            // +0x04
        struct
        {
            uint8_t nBlue;               // +0x04
            uint8_t nGreen;              // +0x05
            uint8_t nRed;                // +0x06
            uint8_t nIntensity;          // +0x07
        };
    };
};

// Stat UI increment button descriptor (0x18 bytes at 0x6FB938B8)
struct D2StatButtonDescStrc
{
    int32_t nStatId;                     // +0x00
    int32_t nX;                          // +0x04
    int32_t nY;                          // +0x08
    int32_t nWidth;                      // +0x0C
    int32_t nHeight;                     // +0x10
    int32_t nState;                      // +0x14 (0 = idle, 1 = pressed, 2 = held outside)
};

// Warden module dispatch tables
struct D2WardenHostCallbacksStrc
{
    void (__stdcall* pfSendPacket)(const uint8_t* pData, int32_t nLen);                // +0x00 (0x6FB1C830)
    int32_t (__stdcall* pfCheckModuleCache)(const uint8_t* pMd5, const uint8_t* pKey); // +0x04 (0x6FB1CDA0)
    int32_t (__fastcall* pfLoadClientModule)(const uint8_t* pKey, const uint8_t* pModData, int32_t nModSize); // +0x08 (0x6FB1CC60)
    void* (__stdcall* pfAllocMem)(uint32_t dwSize);                                    // +0x0C (0x6FB1C570)
    void (__stdcall* pfFreeMem)(void* pMem);                                           // +0x10 (0x6FB1C590)
    int32_t (__stdcall* pfSetRC4Keys)(const void* pKeyData, uint32_t dwSize);          // +0x14 (0x6FB1C5D0)
    int32_t (__stdcall* pfGetRC4Keys)(void* pKeyBuf, uint32_t* pdwSize);               // +0x18 (0x6FB1C620)
};

struct D2WardenModuleExportTableStrc
{
    void* (__fastcall* pfInitModule)(D2WardenHostCallbacksStrc** ppHostTable);         // +0x00
    void (__fastcall* pfReleaseModule)(void* pCtx);                                    // +0x04
    void (__fastcall* pfProcessPacket)(void* pCtx, const uint8_t* pData, int32_t nSize, uint32_t* pdwHandledBytes); // +0x08
    void (__fastcall* pfTick)(void* pCtx, int32_t nPlayerUnitId, int32_t nZero);       // +0x0C
};

// ----------------------------------------------------------------------------
// External Imports (D2Common, D2Gfx, D2Win, D2Lang, Fog, Storm)
// ----------------------------------------------------------------------------
extern "C"
{
    void __cdecl Fog_10023_Error(const char* szMsg, const char* szFile, int32_t nLine);
    void* __fastcall Fog_10042_AllocClientMemory(int32_t nSize, const char* szFile, int32_t nLine, int32_t nZero);
    void __fastcall Fog_10043_FreeClientMemory(void* pMem, const char* szFile, int32_t nLine, int32_t nZero);
    void __fastcall Fog_10046_FreePool(void* pMemPool, void* pMem, const char* szFile, int32_t nLine);
    int32_t __fastcall Fog_10101_OpenFile(const char* szFile, void** phFile);
    void __fastcall Fog_10103_CloseFile(void* hFile);
    int32_t __fastcall Fog_10104_ReadFile(void* hFile, void* pBuf, int32_t nSize, int32_t* pnBytesRead, uint32_t a5, uint32_t a6, uint32_t a7);
    int32_t __fastcall Fog_10105_GetFileSize(void* hFile, uint32_t* pdwHigh);
    void* __fastcall Fog_10106_AsyncReadFile(void* hArchive, const char* szFile, int32_t a3, void* pCallback, void* pBuffer, int32_t nSize, void* pEvent);
    int32_t __fastcall Fog_10107_GetAsyncSearchResult(void* hAsync);
    int32_t __fastcall Fog_10109_CancelAsyncOp(void* hAsync, int32_t bWait);
    void __fastcall Fog_10110_WaitAsyncOp(void* hAsync);
    int32_t __stdcall Fog_10227_IsExpansion();
    void __cdecl Fog_10248_MD5Init(void* pMd5Ctx);
    void __cdecl Fog_10249_MD5Update(void* pMd5Ctx, const void* pData, uint32_t dwLen);
    void __cdecl Fog_10250_MD5Final(uint8_t* pDigest16, void* pMd5Ctx);

    int32_t __stdcall Storm_491_SMemFree(void* ptr, const char* filename, int32_t line, uint32_t flags);
    void __stdcall Storm_493_SMemCopy(void* dest, const void* src, size_t count);
    void __stdcall Storm_494_SMemZero(void* dest, size_t count);
    int32_t __stdcall Storm_495_SMemCmp(const void* buf1, const void* buf2, size_t count);
    char* __stdcall Storm_501_SStrCopy(char* dest, const char* src, size_t max_length);
    size_t __stdcall Storm_506_SStrLen(const char* src);
    int32_t __stdcall Storm_508_SStrCmpI(const char* str1, const char* str2, size_t max_length);
    void __stdcall Storm_604_SBigDel(void* hBigNum);
    void __stdcall Storm_606_SBigFromBinary(void** phBigNum, const void* pData, size_t nBytes);
    void __stdcall Storm_624_SBigNew(void** phBigNum);
    void __stdcall Storm_628_SBigPowMod(void* hOut, void* hBase, void* hExp, void* hMod);
    void __stdcall Storm_636_SBigToBinaryBuffer(void* hBigNum, void* pOutBuf, size_t nBufSize, size_t* pnBytesWritten);

    D2ActiveRoomStrc* __stdcall D2Common_10029_GetRoomCoords(D2ActiveRoomStrc* pRoom, void* pCoords);
    void __stdcall D2Common_10034_GetRoomRect(D2ActiveRoomStrc* pRoom, RECT* pRect);
    void __stdcall D2Common_10055_GetRoomSiblingList(D2ActiveRoomStrc* pRoom, D2ActiveRoomStrc*** pppRoomList, int32_t* pnNumRooms);
    void* __stdcall D2Common_10085_GetRoomTileGrid(D2ActiveRoomStrc* pRoom);
    void* __stdcall D2Common_10366_GetPlayerData(D2UnitStrc* pPlayer);
    void* __stdcall D2Common_10424_GetUnitRoom(D2UnitStrc* pUnit);
    D2StatListStrc* __stdcall D2Common_10474_GetStatListWithFlags(D2UnitStrc* pUnit, int32_t nState, int32_t nFlags);
    int32_t __stdcall D2Common_10494_GetBaseStatSigned(D2UnitStrc* pUnit, int32_t nStatId, int32_t nLayer);
    int32_t __stdcall D2Common_10519_GetUnitStatSigned(D2UnitStrc* pUnit, int32_t nStatId, int32_t nLayer);
    int32_t __stdcall D2Common_10520_GetStatValue32(D2StatListStrc* pStatList, int32_t nStatId, int32_t nLayer);
    int32_t __stdcall D2Common_10521_GetUnitStatBonus(D2UnitStrc* pUnit, int32_t nStatId, int32_t nLayer);
    int32_t __stdcall D2Common_10550_GetUnitDefense(D2UnitStrc* pUnit);
    int32_t __stdcall D2Common_10552_GetUnitToHitFactor(D2UnitStrc* pUnit);
    int32_t __stdcall D2Common_10554_GetUnitBlockingPercent(D2UnitStrc* pUnit, int32_t bExpansion);
    int32_t __stdcall D2Common_10564_EvalSkillFormula(D2UnitStrc* pUnit, int32_t nFormula, int32_t nSkillId, int32_t nSkillLvl);
    void* __stdcall D2Common_10576_CompileTxtFile(void* pMemPool, const char* szName, D2BinFieldStrc* pFields, int32_t* pnRecordCount, int32_t nRecordSize);
    int32_t __stdcall D2Common_10599_GetCharStatsIdFromName(const char* szName);
    D2CharStatsTxt* __stdcall D2Common_10655_GetCharStatsTxtRecord(int32_t nClassId);
    D2UnitStrc* __stdcall D2Common_10776_GetNextInventoryItem(D2UnitStrc* pItem);
    int32_t __stdcall D2Common_10777_UnitIsItem(D2UnitStrc* pItem);
    D2UnitStrc* __stdcall D2Common_10795_GetFirstInventoryItem(void* pInventory);
    int32_t __stdcall D2Common_10797_GetItemBodyLoc(D2UnitStrc* pItem);
    int32_t __stdcall D2Common_10801_CheckItemType(D2UnitStrc* pItem, int32_t nItemType);
    int32_t __stdcall D2Common_10844_CheckItemRequirements(D2UnitStrc* pItem, D2UnitStrc* pPlayer, int32_t bEquip, int32_t* pbReqStr, int32_t* pbReqDex, int32_t* pbReqLvl);
    void* __stdcall D2Common_10949_GetSkillByRow(D2UnitStrc* pUnit, int32_t nRow);
    int32_t __stdcall D2Common_10950_GetSkillFlags(void* pSkill);
    int32_t __stdcall D2Common_10963_GetSkillId(void* pSkill, const char* szFile, int32_t nLine);
    int32_t __stdcall D2Common_10968_GetSkillLevel(D2UnitStrc* pUnit, void* pSkill, int32_t bBonus);
    int32_t __stdcall D2Common_11022_IsInShapeShiftForm(D2UnitStrc* pUnit);
    int32_t __stdcall D2Common_11026_GetAttackRate(D2UnitStrc* pUnit, int32_t nWeaponClass, int32_t nSkillId, int32_t bPrimary, D2UnitStrc* pWeapon, int32_t* pnBaseDamage, int32_t* pnMaxDamage, int32_t* pnBonusAR);
    int32_t __stdcall D2Common_11039_GetExperienceForLevel(int32_t nClassId, int32_t nLevel);

    int32_t __stdcall D2Gfx_10000_CheckPerspectiveMode();
    int32_t __stdcall D2Gfx_10042_TransformPerspectiveCoords(int32_t nX, int32_t nY, int32_t nZ, int32_t* pnOutX, int32_t* pnOutY);
    int32_t __stdcall D2Gfx_10043_IsPerspectiveEnabled();
    void __stdcall D2Gfx_10045_ScalePerspectivePos(int32_t* pnX, int32_t* pnY);
    void __stdcall D2Gfx_10055_DrawSolidRectAlpha(int32_t nX1, int32_t nY1, int32_t nX2, int32_t nY2, uint32_t dwColor, int32_t nAlpha);
    void __stdcall D2Gfx_10059_DrawBox(int32_t nX1, int32_t nY1, int32_t nX2, int32_t nY2, uint32_t dwColor, int32_t nTransTbl);
    void __stdcall D2Gfx_10072_DrawCellFile(D2GfxDataStrc* pData, int32_t nX, int32_t nY, uint32_t dwGamma, int32_t nDrawMode, void* pPalette);
    int32_t __stdcall D2Gfx_10080_DrawFloorTile(D2TileLibraryEntryStrc* pTile, D2FloorTileVertexStrc* pVertices, int32_t nLightCount, int32_t bRoof, uint32_t dwFlags);

    void __fastcall D2Win_10117_DrawText(const wchar_t* wszText, int32_t nX, int32_t nY, int32_t nColor, int32_t bCentered);
    int32_t __fastcall D2Win_10121_GetTextPixelWidth(const wchar_t* wszText);
    void __fastcall D2Win_10123_DrawFramedText(const wchar_t* wszText, int32_t nX, int32_t nY, int32_t nBoxType, int32_t nColor);
    void __fastcall D2Win_10124_DrawHoverText(const wchar_t* wszText, int32_t nX, int32_t nY, int32_t nColor, int32_t bCentered, int32_t nTransTbl);
    int32_t __fastcall D2Win_10125_GetFontHeight();
    int32_t __fastcall D2Win_10127_SetFont(int32_t nFont);
    void __fastcall D2Win_10129_DrawTextEx(const wchar_t* wszText, int32_t nX, int32_t nY, int32_t nColor, int32_t bCentered, int32_t nTransTbl);

    const wchar_t* __fastcall D2Lang_10004_GetStringById(int32_t nStringId);
}

// ----------------------------------------------------------------------------
// Global Variables
// ----------------------------------------------------------------------------
int32_t(&g_aTileVertexRowCounts)[5]    = *(int32_t(*)[5])0x6FB813B8;
int32_t(&g_aTileVertexRowStarts)[5]    = *(int32_t(*)[5])0x6FB813CC;
int32_t(&g_aTileVertexRowEnds)[5]      = *(int32_t(*)[5])0x6FB813E0;
int32_t(&g_aTileSubGridYOffsets)[9]    = *(int32_t(*)[9])0x6FB813F4;
int32_t(&g_aTileSubGridXOffsets)[9]    = *(int32_t(*)[9])0x6FB81418;
int32_t(&g_aTileDiamondVertexIdx)[12]  = *(int32_t(*)[12])0x6FB8143C;
D2StatButtonDescStrc(&g_aCharStatButtons)[4] = *(D2StatButtonDescStrc(*)[4])0x6FB938B8;

static D2WardenHostCallbacksStrc g_tWardenHostCallbacks;     // 0x6FB93A90
static D2WardenHostCallbacksStrc* g_pWardenHostCallbacks;    // 0x6FB93AAC
static int32_t g_nWardenInitialSeed           = 0;           // 0x6FB93AB4
static RECT g_rcViewportTileBounds;                          // 0x6FB96168
static int32_t g_bDisableFloorLightGrid       = 0;           // 0x6FBA78FC
static int32_t g_bFloorSingleLightSample      = 0;           // 0x6FBBA6EC
static D2FloorTileVertexStrc g_aTileVertices[16];            // 0x6FBBD948
static int32_t g_nTileSubLightCount           = 0;           // 0x6FBBD9C0
static void* g_pFloorTileCacheBuffer          = nullptr;     // 0x6FBBD9C4
static uint8_t g_aWardenModuleMd5[16];                       // 0x6FBCC100
static uint8_t g_aWardenModuleRC4Key[16];                    // 0x6FBCC110
static uint8_t* g_pWardenDownloadBuffer       = nullptr;     // 0x6FBCC120
static uint32_t g_dwWardenExpectedModSize     = 0;           // 0x6FBCC0FC
static uint32_t g_dwWardenReceivedModBytes    = 0;           // 0x6FBCC0F8
static D2WardenModuleExportTableStrc* g_pWardenModule = nullptr; // 0x6FBCC124
static void* g_pWardenLoadedImage             = nullptr;     // 0x6FBCC128
static void* g_pWardenModuleInstance          = nullptr;     // 0x6FBCC12C
static uint32_t g_dwWardenLastTick            = 0;           // 0x6FBCC130
static void* g_pBnClientInterface             = nullptr;     // 0x6FBCC134
static D2CellFileStrc* g_pCharStatBtnCellFile = nullptr;     // 0x6FBCC138
static D2CellFileStrc* g_pCharStatPanelCell   = nullptr;     // 0x6FBCC13C
static D2SoundsTxtStubStrc* g_pSoundsTxtTable = nullptr;     // 0x6FBCC148
static int32_t g_nSoundsTxtCount              = 0;           // 0x6FBCC14C
static D2SoundEnvironTxtStrc* g_pSoundEnvTable= nullptr;     // 0x6FBCC150
static int32_t g_nSoundEnvCount               = 0;           // 0x6FBCC154
static D2SoundSampleNodeStrc* g_pSoundCacheHead = nullptr;   // 0x6FBCC160
static D2SoundSampleNodeStrc* g_pSoundCacheTail = nullptr;   // 0x6FBCC164
static uint32_t g_dwSoundCacheMaxBytes        = 0;           // 0x6FBCC174
static uint32_t g_dwSoundCacheUsedBytes       = 0;           // 0x6FBCC178
static uint8_t g_aWardenRC4KeyIn[258];                       // 0x6FBCC180
static uint8_t g_aWardenRC4KeyOut[258];                      // 0x6FBCC288
static void* g_pWardenSavedRC4Keys            = nullptr;     // 0x6FBCC390
static uint32_t g_dwWardenSavedRC4Size        = 0;           // 0x6FBCC394

// External Engine Helpers
extern uint32_t __fastcall LIGHTMAP_GetSubTilePackedRGB(D2ActiveRoomStrc* pRoom, int32_t nSubtileX, int32_t nSubtileY); // 0x6FB0FAC0
extern void __fastcall LIGHTMAP_SampleSubTileRGB(D2ActiveRoomStrc* pRoom, uint8_t* pR, uint8_t* pG, uint8_t* pB, int32_t nSubtileX, int32_t nSubtileY); // 0x6FB0FB60
extern int32_t __stdcall VIEWPORT_GetOffsetX(); // 0x6FB41CC0
extern int32_t __stdcall VIEWPORT_GetOffsetY(); // 0x6FB41CD0
extern void __fastcall VIEWPORT_WorldToScreenCoords(int32_t* pnX, int32_t* pnY); // 0x6FB41CE0
extern D2UnitStrc* __stdcall PLAYER_GetLocalPlayer(); // 0x6FB283D0
extern int32_t __fastcall NET_SendStatButtonPacket(uint8_t nPacketId, uint16_t wStatId, uint16_t wCount); // 0x6FAB7990
extern void __fastcall NET_SendVariablePacket(uint8_t nPacketId, const void* pData, int32_t nLen); // 0x6FAB7810
extern void __fastcall SOUND_PlayUiClick(int32_t nSoundId); // 0x6FB17220
extern D2CellFileStrc* __fastcall ARCHIVE_LoadCellFile(const char* szName, int32_t nType); // 0x6FB6EEB0
extern void __fastcall ARCHIVE_FreeCellFile(D2CellFileStrc* pCellFile); // 0x6FB6FB50

// ============================================================================
// PART 1: Floor Tile Vertex Grid & Floor Tile Renderer (0x6FB18030..0x6FB19080)
// ============================================================================

// 1.12a: 0x6FB18030 (__fastcall)
void __fastcall FLOOR_BuildPerspectiveTileVertices(
    D2ActiveRoomStrc* pRoom,
    int32_t nSubtileX,
    int32_t nSubtileY,
    int32_t bPartialTile,
    uint8_t nAmbientR,
    uint8_t nAmbientG,
    uint8_t nAmbientB)
{
    int32_t nVertIdx = 0;
    int32_t nCurSubY = nSubtileY - 4;
    int32_t nStartSubX = nSubtileX;

    if (!bPartialTile)
    {
        for (int32_t nRow = 0; nRow < 5; ++nRow)
        {
            int32_t nCount = g_aTileVertexRowCounts[nRow];
            int32_t nSubX = nStartSubX;
            int32_t nSubY = nCurSubY;

            for (int32_t i = 0; i < nCount; ++i)
            {
                D2FloorTileVertexStrc* pVert = &g_aTileVertices[nVertIdx];
                int32_t nOutX = 0, nOutY = 0;
                D2Gfx_10042_TransformPerspectiveCoords(nSubX << 8, nSubY << 8, 0, &nOutX, &nOutY);
                pVert->nScreenX = (int16_t)nOutX;
                pVert->nScreenY = (int16_t)nOutY;

                if (g_bDisableFloorLightGrid)
                {
                    pVert->nRed = nAmbientR;
                    pVert->nGreen = nAmbientG;
                    pVert->nBlue = nAmbientB;
                }
                else
                {
                    pVert->dwPackedRGB = LIGHTMAP_GetSubTilePackedRGB(pRoom, nSubX, nSubY);
                }

                ++nVertIdx;
                nSubX += 2;
                nSubY -= 2;
            }

            if (nRow >= 2)
                nStartSubX += 2;
            else
                nCurSubY += 2;
        }
    }
    else
    {
        for (int32_t nRow = 0; nRow < 5; ++nRow)
        {
            int32_t nCount = g_aTileVertexRowCounts[nRow];
            int32_t nSubX = nStartSubX;
            int32_t nSubY = nCurSubY;

            for (int32_t i = 0; i < nCount; ++i)
            {
                D2FloorTileVertexStrc* pVert = &g_aTileVertices[nVertIdx];
                if (i == 0 || i == nCount - 1)
                {
                    int32_t nOutX = 0, nOutY = 0;
                    D2Gfx_10042_TransformPerspectiveCoords(nSubX << 8, nSubY << 8, 0, &nOutX, &nOutY);
                    pVert->nScreenX = (int16_t)nOutX;
                    pVert->nScreenY = (int16_t)nOutY;
                }

                if (g_bDisableFloorLightGrid)
                {
                    pVert->nRed = nAmbientR;
                    pVert->nGreen = nAmbientG;
                    pVert->nBlue = nAmbientB;
                }
                else
                {
                    pVert->dwPackedRGB = LIGHTMAP_GetSubTilePackedRGB(pRoom, nSubX, nSubY);
                }

                ++nVertIdx;
                nSubX += 2;
                nSubY -= 2;
            }

            if (nRow >= 2)
                nStartSubX += 2;
            else
                nCurSubY += 2;
        }

        for (int32_t k = 0; k < 3; ++k)
        {
            int32_t nFirstIdx = g_aTileVertexRowStarts[k];
            int32_t nLastIdx  = g_aTileVertexRowEnds[k];
            int16_t nBaseX = g_aTileVertices[nFirstIdx].nScreenX;
            int16_t nBaseY = g_aTileVertices[nFirstIdx].nScreenY;
            int32_t nDeltaX = g_aTileVertices[nLastIdx].nScreenX - nBaseX;
            int32_t nDeltaY = g_aTileVertices[nLastIdx].nScreenY - nBaseY;

            for (int32_t m = 1; m < 4; ++m)
            {
                D2FloorTileVertexStrc* pMidVert = &g_aTileVertices[g_aTileDiamondVertexIdx[k * 4 + m]];
                pMidVert->nScreenX = (int16_t)(nBaseX + ((nDeltaX * m) >> 2));
                pMidVert->nScreenY = (int16_t)(nBaseY + ((nDeltaY * m) >> 2));
            }
        }
    }
}

// 1.12a: 0x6FB182B0 (includes tail at 0x6FB183D0) (__fastcall)
void __fastcall FLOOR_BuildNonPerspectiveTileVertices(
    D2ActiveRoomStrc* pRoom,
    int32_t nSubtileX,
    int32_t nSubtileY,
    uint8_t nAmbientR,
    uint8_t nAmbientG,
    uint8_t nAmbientB)
{
    if (g_bDisableFloorLightGrid)
    {
        g_aTileVertices[0].nRed = nAmbientR;
        g_aTileVertices[0].nGreen = nAmbientG;
        g_aTileVertices[0].nBlue = nAmbientB;
        g_nTileSubLightCount = 1;
        return;
    }

    if (g_bFloorSingleLightSample)
    {
        g_aTileVertices[0].dwPackedRGB = LIGHTMAP_GetSubTilePackedRGB(pRoom, nSubtileX + 2, nSubtileY - 2);
        g_nTileSubLightCount = 1;
        return;
    }

    int32_t nBaseSubY = nSubtileY - 4;
    bool bAllEqual = true;

    for (int32_t i = 0; i < 9; ++i)
    {
        int32_t nSampleY = nBaseSubY + g_aTileSubGridYOffsets[i];
        int32_t nSampleX = nSubtileX + g_aTileSubGridXOffsets[i];
        uint32_t dwPacked = LIGHTMAP_GetSubTilePackedRGB(pRoom, nSampleX, nSampleY);
        g_aTileVertices[i].dwPackedRGB = dwPacked;

        if (bAllEqual && dwPacked != g_aTileVertices[0].dwPackedRGB)
        {
            bAllEqual = false;
        }
    }

    if (bAllEqual)
    {
        g_nTileSubLightCount = 1;
        return;
    }

    uint32_t dwFirst = g_aTileVertices[0].dwPackedRGB;
    if (g_aTileVertices[2].dwPackedRGB == dwFirst &&
        g_aTileVertices[6].dwPackedRGB == dwFirst &&
        g_aTileVertices[8].dwPackedRGB == dwFirst)
    {
        uint32_t dwEdge = g_aTileVertices[1].dwPackedRGB;
        if (g_aTileVertices[3].dwPackedRGB == dwEdge &&
            g_aTileVertices[5].dwPackedRGB == dwEdge &&
            g_aTileVertices[7].dwPackedRGB == dwEdge)
        {
            if (g_aTileVertices[1].nRed   == ((int32_t)g_aTileVertices[0].nRed   + (int32_t)g_aTileVertices[4].nRed)   / 2 &&
                g_aTileVertices[1].nGreen == ((int32_t)g_aTileVertices[0].nGreen + (int32_t)g_aTileVertices[4].nGreen) / 2 &&
                g_aTileVertices[1].nBlue  == ((int32_t)g_aTileVertices[0].nBlue  + (int32_t)g_aTileVertices[4].nBlue)  / 2)
            {
                g_aTileVertices[1].dwPackedRGB = g_aTileVertices[4].dwPackedRGB;
                g_nTileSubLightCount = 2;
                return;
            }
        }
    }

    if (g_aTileVertices[1].dwPackedRGB == (g_aTileVertices[0].dwPackedRGB + g_aTileVertices[3].dwPackedRGB) / 2 &&
        g_aTileVertices[2].dwPackedRGB == (g_aTileVertices[0].dwPackedRGB + g_aTileVertices[5].dwPackedRGB) / 2 &&
        g_aTileVertices[6].dwPackedRGB == (g_aTileVertices[3].dwPackedRGB + g_aTileVertices[8].dwPackedRGB) / 2 &&
        g_aTileVertices[7].dwPackedRGB == (g_aTileVertices[5].dwPackedRGB + g_aTileVertices[8].dwPackedRGB) / 2 &&
        g_aTileVertices[4].dwPackedRGB == (g_aTileVertices[1].dwPackedRGB + g_aTileVertices[2].dwPackedRGB +
                                           g_aTileVertices[6].dwPackedRGB + g_aTileVertices[7].dwPackedRGB) / 4)
    {
        g_aTileVertices[1].dwPackedRGB = g_aTileVertices[3].dwPackedRGB;
        g_aTileVertices[2].dwPackedRGB = g_aTileVertices[5].dwPackedRGB;
        g_aTileVertices[3].dwPackedRGB = g_aTileVertices[8].dwPackedRGB;
        g_nTileSubLightCount = 4;
        return;
    }

    g_nTileSubLightCount = 9;
}

// 1.12a: 0x6FB185A0 (includes 0x6FB18620) (__fastcall)
void __fastcall FLOOR_FillTileLightGrid(D2ActiveRoomStrc* pRoom, int32_t nSubtileX, int32_t nSubtileY)
{
    uint8_t nR = 0, nG = 0, nB = 0;
    if (g_bDisableFloorLightGrid)
    {
        LIGHTMAP_SampleSubTileRGB(pRoom, &nR, &nG, &nB, nSubtileX, nSubtileY);
    }

    if (D2Gfx_10043_IsPerspectiveEnabled())
    {
        FLOOR_BuildPerspectiveTileVertices(pRoom, nSubtileX, nSubtileY, 1, nR, nG, nB);
    }
    else
    {
        FLOOR_BuildNonPerspectiveTileVertices(pRoom, nSubtileX, nSubtileY, nR, nG, nB);
    }
}

// 1.12a: 0x6FB18660 (__fastcall)
void __fastcall FLOOR_SetFloorLightGridDisabled(int32_t bDisabled)
{
    g_bDisableFloorLightGrid = bDisabled;
}

// 1.12a: 0x6FB18680 (__fastcall)
void __fastcall FLOOR_DrawViewportFloorTiles(D2ActiveRoomStrc* pCenterRoom, int32_t nViewportLeft, int32_t nViewportTop, int32_t nViewportRight, int32_t nViewportBottom)
{
    g_rcViewportTileBounds.left   = nViewportLeft;
    g_rcViewportTileBounds.top    = nViewportTop;
    g_rcViewportTileBounds.right  = nViewportRight;
    g_rcViewportTileBounds.bottom = nViewportBottom;
    // Iterates visible rooms around pCenterRoom and dispatches FLOOR_DrawRoomFloorPass / FLOOR_DrawPerspectiveRoomFloorPass
}

// 1.12a: 0x6FB18940 (__stdcall)
void __stdcall FLOOR_AllocFloorTileCache()
{
    g_pFloorTileCacheBuffer = Fog_10042_AllocClientMemory(0x4000, "..\\Source\\D2Client\\Engine\\Floor.cpp", 412, 0);
}

// 1.12a: 0x6FB18980 (includes 0x6FB18AD0) (__fastcall)
void __fastcall FLOOR_DrawRoomFloorTile(
    D2ActiveRoomStrc* pRoom,
    void* pFloorTileData,
    int32_t nScreenX,
    int32_t nScreenY,
    int32_t nSubtileX,
    int32_t nSubtileY,
    int32_t bClipBounds)
{
    int32_t nTileScreenX = nScreenX;
    int32_t nTileScreenY = nScreenY - 80;

    if (!D2Gfx_10043_IsPerspectiveEnabled() && bClipBounds)
    {
        if (nTileScreenX > g_rcViewportTileBounds.right ||
            nTileScreenY > g_rcViewportTileBounds.bottom ||
            nTileScreenX + 160 < g_rcViewportTileBounds.left ||
            nTileScreenY + 80 < g_rcViewportTileBounds.top)
        {
            return;
        }
    }

    uint32_t dwTileFlags = *(uint32_t*)((uint8_t*)pFloorTileData + 0x04);
    if ((dwTileFlags & 0x201) != 0)
    {
        return;
    }

    D2TileLibraryEntryStrc* pTileLibEntry = *(D2TileLibraryEntryStrc**)pFloorTileData;
    if (!pTileLibEntry)
    {
        Fog_10023_Error("ptTile->hTile", "..\\Source\\D2Client\\Engine\\Floor.cpp", 458);
    }

    FLOOR_FillTileLightGrid(pRoom, nSubtileX, nSubtileY);

    if (!D2Gfx_10043_IsPerspectiveEnabled())
    {
        g_aTileVertices[0].nScreenX = (int16_t)nTileScreenX;
        g_aTileVertices[0].nScreenY = (int16_t)nTileScreenY;
    }

    int32_t bRoof = (dwTileFlags >> 3) & 1;
    D2Gfx_10080_DrawFloorTile(pTileLibEntry, g_aTileVertices, g_nTileSubLightCount, bRoof, 0xFFFFFFFF);
}

// 1.12a: 0x6FB18B10 (__fastcall)
void __fastcall FLOOR_DrawRoomFloorPass(D2ActiveRoomStrc* pRoom, int32_t bClipBounds)
{
    void* pTileGrid = D2Common_10085_GetRoomTileGrid(pRoom);
    if (!pTileGrid)
    {
        return;
    }

    uint8_t* pFloorArray = *(uint8_t**)((uint8_t*)pTileGrid + 0x08);
    int32_t nFloorCount = *(int32_t*)((uint8_t*)pTileGrid + 0x10);
    for (int32_t i = 0; i < nFloorCount; ++i)
    {
        uint8_t* pTile = pFloorArray + i * 0x18;
        int32_t nSubtileX = *(int32_t*)(pTile + 0x08);
        int32_t nSubtileY = *(int32_t*)(pTile + 0x0C);
        int32_t nScreenX  = *(int32_t*)(pTile + 0x10) - VIEWPORT_GetOffsetX();
        int32_t nScreenY  = *(int32_t*)(pTile + 0x14) - VIEWPORT_GetOffsetY();
        FLOOR_DrawRoomFloorTile(pRoom, pTile, nScreenX, nScreenY, nSubtileX, nSubtileY, bClipBounds);
    }
}

// 1.12a: 0x6FB18BD0 (__stdcall)
void __stdcall FLOOR_NoOpStub()
{
}

// 1.12a: 0x6FB18BE0 (includes 0x6FB18C70) (__fastcall)
void __fastcall FLOOR_DrawPerspectiveRoomFloorPass(D2ActiveRoomStrc* pRoom)
{
    FLOOR_DrawRoomFloorPass(pRoom, 0);
}

// ============================================================================
// PART 2: Sound Header & Sample Cache (..\Source\D2Client\Sound\SoundHdr.cpp)
//         0x6FB19090 .. 0x6FB1A750
// ============================================================================

// 1.12a: 0x6FB19090 (__fastcall)
D2SoundsTxtStubStrc* __fastcall SOUNDHDR_GetSoundTxtRecord(int32_t nSoundId)
{
    if (nSoundId < 0 || nSoundId >= g_nSoundsTxtCount)
    {
        return nullptr;
    }
    return &g_pSoundsTxtTable[nSoundId];
}

// 1.12a: 0x6FB190B0 (__fastcall)
D2SoundEnvironTxtStrc* __fastcall SOUNDHDR_GetSoundEnvironRecord(int32_t nEnvironId)
{
    if (nEnvironId < 0 || nEnvironId >= g_nSoundEnvCount)
    {
        nEnvironId = 0;
    }
    return &g_pSoundEnvTable[nEnvironId];
}

// 1.12a: 0x6FB190D0 (__stdcall)
int32_t __stdcall SOUNDHDR_GetSoundEnvironCount()
{
    return g_nSoundEnvCount;
}

// 1.12a: 0x6FB190E0 (__fastcall)
int32_t __fastcall SOUNDHDR_ParseSoundNameColumn(const char* szSoundName)
{
    if (!szSoundName || !szSoundName[0])
    {
        return -1;
    }
    for (int32_t i = 0; i < g_nSoundsTxtCount; ++i)
    {
        if (Storm_508_SStrCmpI(g_pSoundsTxtTable[i].szFileName, szSoundName, 60) == 0)
        {
            return i;
        }
    }
    return -1;
}

// 1.12a: 0x6FB195E0 (includes 0x6FB19660) (__stdcall)
void __stdcall SOUNDHDR_LoadSoundTxtTables()
{
    int32_t nTempSoundsCount = 0;
    uint8_t* pFullSounds = (uint8_t*)D2Common_10576_CompileTxtFile(nullptr, "Sounds", nullptr, &nTempSoundsCount, 0xD0);
    g_nSoundsTxtCount = nTempSoundsCount;
    g_pSoundsTxtTable = (D2SoundsTxtStubStrc*)Fog_10042_AllocClientMemory(
        nTempSoundsCount * sizeof(D2SoundsTxtStubStrc),
        "..\\Source\\D2Client\\Sound\\SoundHdr.cpp",
        245,
        0);

    for (int32_t i = 0; i < g_nSoundsTxtCount; ++i)
    {
        uint8_t* pSrc = pFullSounds + i * 0xD0;
        D2SoundsTxtStubStrc* pDst = &g_pSoundsTxtTable[i];
        memset(pDst, 0, sizeof(D2SoundsTxtStubStrc));

        pDst->wIndex       = *(uint16_t*)(pSrc + 0x00);
        Storm_501_SStrCopy(pDst->szFileName, (const char*)(pSrc + 0x02), 60);
        pDst->dwVolume     = *(uint32_t*)(pSrc + 0x7A);
        pDst->nGroupSize   = *(uint8_t*)(pSrc + 0x7E);
        pDst->nLoop        = *(uint8_t*)(pSrc + 0x7F);
        pDst->dwFadeIn     = *(uint32_t*)(pSrc + 0x80);
        pDst->dwFadeOut    = *(uint32_t*)(pSrc + 0x84);
        pDst->bDeferInst   = *(uint8_t*)(pSrc + 0x88);
        pDst->bStopInst    = *(uint8_t*)(pSrc + 0x89);
        pDst->dwDuration   = *(uint32_t*)(pSrc + 0x8A);
        pDst->wCompound    = *(uint16_t*)(pSrc + 0x8E);
        pDst->dwReverb     = *(uint32_t*)(pSrc + 0x90);
        pDst->dwFalloff    = *(uint32_t*)(pSrc + 0x94);
        pDst->bCache       = *(uint8_t*)(pSrc + 0x98);
        pDst->bAsyncOnly   = *(uint8_t*)(pSrc + 0x99);
        pDst->nPriority    = *(uint8_t*)(pSrc + 0x9A);
        pDst->bStream      = *(uint8_t*)(pSrc + 0x9B);
        pDst->bStereo      = *(uint8_t*)(pSrc + 0x9C);
        pDst->bTracking    = *(uint8_t*)(pSrc + 0x9D);
        pDst->bSolo        = *(uint8_t*)(pSrc + 0x9E);
        pDst->bMusicVol    = *(uint8_t*)(pSrc + 0x9F);
        pDst->dwBlock1     = *(uint32_t*)(pSrc + 0xA0);
        pDst->dwBlock2     = *(uint32_t*)(pSrc + 0xA4);
        pDst->dwBlock3     = *(uint32_t*)(pSrc + 0xA8);
    }

    Fog_10046_FreePool(nullptr, pFullSounds, "..\\Source\\D2Client\\Sound\\SoundHdr.cpp", 282);

    g_pSoundEnvTable = (D2SoundEnvironTxtStrc*)D2Common_10576_CompileTxtFile(
        nullptr,
        "SoundEnviron",
        nullptr,
        &g_nSoundEnvCount,
        sizeof(D2SoundEnvironTxtStrc));
}

// 1.12a: 0x6FB19DB0 (__stdcall)
void __stdcall SOUNDHDR_FreeSoundTxtTables()
{
    if (g_pSoundsTxtTable)
    {
        Fog_10043_FreeClientMemory(g_pSoundsTxtTable, "..\\Source\\D2Client\\Sound\\SoundHdr.cpp", 300, 0);
        g_pSoundsTxtTable = nullptr;
    }
    g_nSoundsTxtCount = 0;

    if (g_pSoundEnvTable)
    {
        Fog_10046_FreePool(nullptr, g_pSoundEnvTable, "..\\Source\\D2Client\\Sound\\SoundHdr.cpp", 306);
        g_pSoundEnvTable = nullptr;
    }
    g_nSoundEnvCount = 0;
}

// 1.12a: 0x6FB19E00 (__fastcall)
void __fastcall SOUNDHDR_UnlinkSampleNode(D2SoundSampleNodeStrc* pNode)
{
    if (pNode->pPrevCost)
        pNode->pPrevCost->pNextCost = pNode->pNextCost;
    else
        g_pSoundCacheHead = pNode->pNextCost;

    if (pNode->pNextCost)
        pNode->pNextCost->pPrevCost = pNode->pPrevCost;
    else
        g_pSoundCacheTail = pNode->pPrevCost;

    pNode->pPrevCost = nullptr;
    pNode->pNextCost = nullptr;
}

// 1.12a: 0x6FB19E40 (__fastcall)
void __fastcall SOUNDHDR_InsertSampleNodeMRU(D2SoundSampleNodeStrc* pNode)
{
    pNode->pPrevCost = nullptr;
    pNode->pNextCost = g_pSoundCacheHead;
    if (g_pSoundCacheHead)
        g_pSoundCacheHead->pPrevCost = pNode;
    else
        g_pSoundCacheTail = pNode;
    g_pSoundCacheHead = pNode;
}

// 1.12a: 0x6FB19E70 (__fastcall)
void __fastcall SOUNDHDR_FreeSampleNode(D2SoundSampleNodeStrc* pNode)
{
    if (!pNode)
    {
        return;
    }

    if (pNode->hAsyncRequest)
    {
        if (!Fog_10109_CancelAsyncOp(pNode->hAsyncRequest, 0))
        {
            Fog_10110_WaitAsyncOp(pNode->hAsyncRequest);
            Fog_10109_CancelAsyncOp(pNode->hAsyncRequest, 1);
        }
        pNode->hAsyncRequest = nullptr;
    }

    SOUNDHDR_UnlinkSampleNode(pNode);

    for (int32_t i = 0; i < g_nSoundsTxtCount; ++i)
    {
        if (g_pSoundsTxtTable[i].pCacheNode == pNode)
        {
            g_pSoundsTxtTable[i].pCacheNode = nullptr;
        }
    }

    g_dwSoundCacheUsedBytes -= pNode->dwByteSize;

    if (pNode->pSampleData)
    {
        Fog_10043_FreeClientMemory(pNode->pSampleData, "..\\Source\\D2Client\\Sound\\SoundHdr.cpp", 463, 0);
    }
    Fog_10043_FreeClientMemory(pNode, "..\\Source\\D2Client\\Sound\\SoundHdr.cpp", 464, 0);
}

// 1.12a: 0x6FB19F20 (__fastcall)
int32_t __fastcall SOUNDHDR_EvictCacheForSize(uint32_t dwNeededBytes)
{
    D2SoundSampleNodeStrc* pCur = g_pSoundCacheTail;
    while (pCur && g_dwSoundCacheUsedBytes + dwNeededBytes > g_dwSoundCacheMaxBytes)
    {
        D2SoundSampleNodeStrc* pPrev = pCur->pPrevCost;
        if (pCur->nLockCount == 0)
        {
            SOUNDHDR_FreeSampleNode(pCur);
        }
        pCur = pPrev;
    }
    return (g_dwSoundCacheUsedBytes + dwNeededBytes <= g_dwSoundCacheMaxBytes) ? 1 : 0;
}

// 1.12a: 0x6FB19F60 (__stdcall)
void __stdcall SOUNDHDR_InitSampleCacheSize()
{
    MEMORYSTATUS memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUS);
    GlobalMemoryStatus(&memStatus);

    uint32_t dwPhysMB = (uint32_t)(memStatus.dwTotalPhys >> 20);
    if (dwPhysMB <= 64)
        g_dwSoundCacheMaxBytes = 0x200000;  // 2 MB
    else if (dwPhysMB <= 128)
        g_dwSoundCacheMaxBytes = 0x400000;  // 4 MB
    else
        g_dwSoundCacheMaxBytes = 0x800000;  // 8 MB

    g_dwSoundCacheUsedBytes = 0;
}

// 1.12a: 0x6FB19FD0 (__stdcall)
void __stdcall SOUNDHDR_FlushSampleCache()
{
    while (g_pSoundCacheHead)
    {
        SOUNDHDR_FreeSampleNode(g_pSoundCacheHead);
    }
}

// 1.12a: 0x6FB1A000 (__fastcall)
void __fastcall SOUNDHDR_UnlockSampleNode(D2SoundSampleNodeStrc* pNode)
{
    if (pNode && pNode->nLockCount > 0)
    {
        --pNode->nLockCount;
    }
}

// 1.12a: 0x6FB1A020 (__fastcall)
D2SoundSampleNodeStrc* __fastcall SOUNDHDR_FindCachedSampleByName(const char* szFileName)
{
    for (D2SoundSampleNodeStrc* pCur = g_pSoundCacheHead; pCur != nullptr; pCur = pCur->pNextCost)
    {
        if (Storm_508_SStrCmpI(pCur->szFileName, szFileName, 32) == 0)
        {
            SOUNDHDR_UnlinkSampleNode(pCur);
            SOUNDHDR_InsertSampleNodeMRU(pCur);
            return pCur;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FB1A0B0 (__fastcall)
D2SoundSampleNodeStrc* __fastcall SOUNDHDR_GetOrCreateSampleNode(D2SoundsTxtStubStrc* pSoundRec)
{
    if (pSoundRec->pCacheNode)
    {
        SOUNDHDR_UnlinkSampleNode(pSoundRec->pCacheNode);
        SOUNDHDR_InsertSampleNodeMRU(pSoundRec->pCacheNode);
        return pSoundRec->pCacheNode;
    }

    D2SoundSampleNodeStrc* pNode = SOUNDHDR_FindCachedSampleByName(pSoundRec->szFileName);
    if (pNode)
    {
        pSoundRec->pCacheNode = pNode;
        return pNode;
    }

    pNode = (D2SoundSampleNodeStrc*)Fog_10042_AllocClientMemory(
        sizeof(D2SoundSampleNodeStrc),
        "..\\Source\\D2Client\\Sound\\SoundHdr.cpp",
        565,
        0);
    memset(pNode, 0, sizeof(D2SoundSampleNodeStrc));
    Storm_501_SStrCopy(pNode->szFileName, pSoundRec->szFileName, 32);
    SOUNDHDR_InsertSampleNodeMRU(pNode);
    pSoundRec->pCacheNode = pNode;
    return pNode;
}

// 1.12a: 0x6FB1A130 (__fastcall)
int32_t __fastcall SOUNDHDR_LoadSoundSampleSync(D2SoundsTxtStubStrc* pSoundRec)
{
    char szPath[128];
    sprintf(szPath, "data\\global\\sfx\\%s", pSoundRec->szFileName);

    void* hFile = nullptr;
    if (!Fog_10101_OpenFile(szPath, &hFile))
    {
        return 0;
    }

    uint32_t dwFileSize = (uint32_t)Fog_10105_GetFileSize(hFile, nullptr);
    if (!SOUNDHDR_EvictCacheForSize(dwFileSize))
    {
        Fog_10103_CloseFile(hFile);
        return 0;
    }

    D2SoundSampleNodeStrc* pNode = SOUNDHDR_GetOrCreateSampleNode(pSoundRec);
    pNode->dwByteSize = dwFileSize;
    pNode->pSampleData = (uint8_t*)Fog_10042_AllocClientMemory(
        (int32_t)dwFileSize,
        "..\\Source\\D2Client\\Sound\\SoundHdr.cpp",
        618,
        0);
    g_dwSoundCacheUsedBytes += dwFileSize;

    int32_t nBytesRead = 0;
    Fog_10104_ReadFile(hFile, pNode->pSampleData, (int32_t)dwFileSize, &nBytesRead, 0, 0, 0);
    Fog_10103_CloseFile(hFile);
    return 1;
}

// 1.12a: 0x6FB1A290 (__fastcall)
int32_t __fastcall SOUNDHDR_LoadSoundSampleAsync(D2SoundsTxtStubStrc* pSoundRec)
{
    char szPath[128];
    sprintf(szPath, "data\\global\\sfx\\%s", pSoundRec->szFileName);

    void* hFile = nullptr;
    if (!Fog_10101_OpenFile(szPath, &hFile))
    {
        return 0;
    }
    uint32_t dwFileSize = (uint32_t)Fog_10105_GetFileSize(hFile, nullptr);
    Fog_10103_CloseFile(hFile);

    if (!SOUNDHDR_EvictCacheForSize(dwFileSize))
    {
        return 0;
    }

    D2SoundSampleNodeStrc* pNode = SOUNDHDR_GetOrCreateSampleNode(pSoundRec);
    pNode->dwByteSize = dwFileSize;
    pNode->pSampleData = (uint8_t*)Fog_10042_AllocClientMemory(
        (int32_t)dwFileSize,
        "..\\Source\\D2Client\\Sound\\SoundHdr.cpp",
        678,
        0);
    g_dwSoundCacheUsedBytes += dwFileSize;

    pNode->hAsyncRequest = Fog_10106_AsyncReadFile(nullptr, szPath, 0, nullptr, pNode->pSampleData, (int32_t)dwFileSize, nullptr);
    return (pNode->hAsyncRequest != nullptr) ? 1 : 0;
}

// 1.12a: 0x6FB1A3F0 (__fastcall)
int32_t __fastcall SOUNDHDR_PollAsyncSampleLoad(D2SoundSampleNodeStrc* pNode)
{
    if (!pNode)
    {
        return 0;
    }
    if (!pNode->hAsyncRequest)
    {
        return (pNode->pSampleData != nullptr) ? 1 : 0;
    }
    if (Fog_10107_GetAsyncSearchResult(pNode->hAsyncRequest))
    {
        Fog_10109_CancelAsyncOp(pNode->hAsyncRequest, 1);
        pNode->hAsyncRequest = nullptr;
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB1A470 (__fastcall)
uint8_t* __fastcall SOUNDHDR_AcquireSoundSample(int32_t nSoundId, uint32_t* pdwSizeOut)
{
    D2SoundsTxtStubStrc* pRec = SOUNDHDR_GetSoundTxtRecord(nSoundId);
    if (!pRec)
    {
        return nullptr;
    }

    if (!pRec->pCacheNode)
    {
        if (pRec->bAsyncOnly)
            SOUNDHDR_LoadSoundSampleAsync(pRec);
        else
            SOUNDHDR_LoadSoundSampleSync(pRec);
    }

    D2SoundSampleNodeStrc* pNode = pRec->pCacheNode;
    if (!pNode || !SOUNDHDR_PollAsyncSampleLoad(pNode))
    {
        return nullptr;
    }

    ++pNode->nLockCount;
    if (pdwSizeOut)
    {
        *pdwSizeOut = pNode->dwByteSize;
    }
    return pNode->pSampleData;
}

// 1.12a: 0x6FB1A510 (__fastcall)
void __fastcall SOUNDHDR_ReleaseSoundSample(int32_t nSoundId)
{
    D2SoundsTxtStubStrc* pRec = SOUNDHDR_GetSoundTxtRecord(nSoundId);
    if (pRec && pRec->pCacheNode)
    {
        SOUNDHDR_UnlockSampleNode(pRec->pCacheNode);
    }
}

// 1.12a: 0x6FB1A530 (__fastcall)
void __fastcall SOUNDHDR_TouchSoundHistory(D2SoundsTxtStubStrc* pGroupBaseRec, uint32_t dwChosenSoundId)
{
    uint8_t nPos = pGroupBaseRec->nHistoryPos & 3;
    pGroupBaseRec->dwHistoryIdx[nPos] = dwChosenSoundId;
    pGroupBaseRec->nHistoryPos = (nPos + 1) & 3;
}

// 1.12a: 0x6FB1A550 (__fastcall)
int32_t __fastcall SOUNDHDR_ResolveRandomGroupSoundId(int32_t nBaseSoundId)
{
    D2SoundsTxtStubStrc* pRec = SOUNDHDR_GetSoundTxtRecord(nBaseSoundId);
    if (!pRec || pRec->nGroupSize <= 1)
    {
        return nBaseSoundId;
    }

    uint32_t dwGroupSize = pRec->nGroupSize;
    int32_t nChosenId = nBaseSoundId + (GetTickCount() % dwGroupSize);
    SOUNDHDR_TouchSoundHistory(pRec, (uint32_t)nChosenId);
    return nChosenId;
}

// 1.12a: 0x6FB1A630 (__stdcall)
void __stdcall SOUNDHDR_Shutdown()
{
    SOUNDHDR_FlushSampleCache();
    SOUNDHDR_FreeSoundTxtTables();
}

// 1.12a: 0x6FB1A660 (__stdcall)
void __stdcall SOUNDHDR_UpdateFrame()
{
    for (D2SoundSampleNodeStrc* pCur = g_pSoundCacheHead; pCur != nullptr; pCur = pCur->pNextCost)
    {
        if (pCur->hAsyncRequest)
        {
            SOUNDHDR_PollAsyncSampleLoad(pCur);
        }
    }
}

// 1.12a: 0x6FB1A6C0 (__stdcall)
void __stdcall SOUNDHDR_Init()
{
    SOUNDHDR_InitSampleCacheSize();
    SOUNDHDR_LoadSoundTxtTables();
}

// ============================================================================
// PART 3: Character Stats UI Panel (0x6FB1A760 .. 0x6FB1C3A0)
// ============================================================================

// 1.12a: 0x6FB1A760 (__stdcall)
void __stdcall CHARSTATS_InitStatPanel()
{
    g_pCharStatPanelCell = ARCHIVE_LoadCellFile("Panel\\InvChar6", 0);
}

// 1.12a: 0x6FB1A780 (__stdcall)
void __stdcall CHARSTATS_FreeStatPanel()
{
    if (g_pCharStatPanelCell)
    {
        ARCHIVE_FreeCellFile(g_pCharStatPanelCell);
        g_pCharStatPanelCell = nullptr;
    }
    if (g_pCharStatBtnCellFile)
    {
        ARCHIVE_FreeCellFile(g_pCharStatBtnCellFile);
        g_pCharStatBtnCellFile = nullptr;
    }
}

// 1.12a: 0x6FB1A7E0 (__stdcall)
void __stdcall CHARSTATS_CloseStatPanel()
{
    SOUND_PlayUiClick(2);
}

// 1.12a: 0x6FB1A810 (__fastcall)
int32_t __fastcall CHARSTATS_GetPassiveWeaponBlockBonus(D2UnitStrc* pPlayer)
{
    if (!pPlayer || *(int32_t*)pPlayer != 0)
    {
        return 0;
    }

    void* pInventory = *(void**)((uint8_t*)pPlayer + 0x60);
    if (!pInventory)
    {
        return 0;
    }

    int32_t nDualWieldClawCount = 0;
    for (D2UnitStrc* pItem = D2Common_10795_GetFirstInventoryItem(pInventory);
         pItem != nullptr;
         pItem = D2Common_10776_GetNextInventoryItem(pItem))
    {
        if (D2Common_10777_UnitIsItem(pItem))
        {
            int32_t nBodyLoc = D2Common_10797_GetItemBodyLoc(pItem);
            if ((nBodyLoc == 4 || nBodyLoc == 5) &&
                D2Common_10801_CheckItemType(pItem, 67) &&
                D2Common_10844_CheckItemRequirements(pItem, pPlayer, 1, nullptr, nullptr, nullptr))
            {
                ++nDualWieldClawCount;
            }
        }
    }

    if (nDualWieldClawCount < 2)
    {
        return 0;
    }

    return D2Common_10519_GetUnitStatSigned(pPlayer, 184 /* STAT_PASSIVE_WEAPON_BLOCK */, 0);
}

// 1.12a: 0x6FB1A900 (__fastcall)
int32_t __fastcall CHARSTATS_CalcChanceToBeHit(
    D2UnitStrc* pPlayer,
    int32_t* pnDefenseOut,
    int32_t* pnBlockPctOut,
    int32_t* pnMonsterIdOut)
{
    if (pnDefenseOut)   *pnDefenseOut = 0;
    if (pnBlockPctOut)  *pnBlockPctOut = 0;
    if (pnMonsterIdOut) *pnMonsterIdOut = -1;

    if (!pPlayer)
    {
        return 0;
    }

    int32_t nDefense = D2Common_10550_GetUnitDefense(pPlayer);
    int32_t nBlockPct = D2Common_10554_GetUnitBlockingPercent(pPlayer, Fog_10227_IsExpansion());
    if (nBlockPct <= 0)
    {
        nBlockPct = CHARSTATS_GetPassiveWeaponBlockBonus(pPlayer);
    }

    if (pnDefenseOut)  *pnDefenseOut = nDefense;
    if (pnBlockPctOut) *pnBlockPctOut = nBlockPct;

    int32_t nPlayerLvl = D2Common_10519_GetUnitStatSigned(pPlayer, 12 /* STAT_LEVEL */, 0);
    int32_t nLastAttackerId = D2Common_10519_GetUnitStatSigned(pPlayer, 180, 0);
    if (pnMonsterIdOut)
    {
        *pnMonsterIdOut = nLastAttackerId;
    }

    int32_t nChance = 100 - (nDefense * 100) / (nDefense + nPlayerLvl * 20 + 1);
    if (nChance < 5)  nChance = 5;
    if (nChance > 95) nChance = 95;
    return nChance;
}

// 1.12a: 0x6FB1AB40 (__fastcall)
void __fastcall CHARSTATS_DrawDefenseHoverTooltip(D2UnitStrc* pPlayer, int32_t nX, int32_t nY)
{
    int32_t nDefense = 0, nBlockPct = 0, nMonsterId = -1;
    int32_t nChanceToBeHit = CHARSTATS_CalcChanceToBeHit(pPlayer, &nDefense, &nBlockPct, &nMonsterId);

    wchar_t wszTooltip[128];
    swprintf(wszTooltip, 128, L"%d%%", nChanceToBeHit);
    D2Win_10124_DrawHoverText(wszTooltip, nX, nY, 0, 1, 0);
}

// 1.12a: 0x6FB1ACD0 (__fastcall)
void __fastcall CHARSTATS_DrawAttackRatingHoverTooltip(D2UnitStrc* pPlayer, int32_t nX, int32_t nY, int32_t bLeftHand)
{
    wchar_t wszTooltip[128];
    int32_t nAR = D2Common_10552_GetUnitToHitFactor(pPlayer);
    swprintf(wszTooltip, 128, L"%d", nAR);
    D2Win_10124_DrawHoverText(wszTooltip, nX, nY, 0, 1, 0);
}

// 1.12a: 0x6FB1AF70 (__fastcall)
int32_t __fastcall CHARSTATS_CalcChanceToHit(D2UnitStrc* pPlayer, int32_t* pnAttackRatingOut, int32_t* pnMonsterIdOut, int32_t bPrimary)
{
    int32_t nBaseDmg = 0, nMaxDmg = 0, nBonusAR = 0;
    int32_t nAR = D2Common_11026_GetAttackRate(pPlayer, 0, 0, bPrimary, nullptr, &nBaseDmg, &nMaxDmg, &nBonusAR);
    if (pnAttackRatingOut)
    {
        *pnAttackRatingOut = nAR;
    }
    return 95;
}

// 1.12a: 0x6FB1B100 (__fastcall)
int32_t __fastcall CHARSTATS_HandleMouseDown(void* pMsg)
{
    int32_t nMouseX = *(int32_t*)((uint8_t*)pMsg + 0x08);
    int32_t nMouseY = *(int32_t*)((uint8_t*)pMsg + 0x0C);

    D2UnitStrc* pPlayer = PLAYER_GetLocalPlayer();
    if (!pPlayer || D2Common_10519_GetUnitStatSigned(pPlayer, 4 /* STAT_STATPTS */, 0) <= 0)
    {
        return 0;
    }

    for (int32_t i = 0; i < 4; ++i)
    {
        D2StatButtonDescStrc* pBtn = &g_aCharStatButtons[i];
        if (nMouseX >= pBtn->nX && nMouseX <= pBtn->nX + pBtn->nWidth &&
            nMouseY >= pBtn->nY - pBtn->nHeight && nMouseY <= pBtn->nY)
        {
            pBtn->nState = 1;
            SOUND_PlayUiClick(4);
            return 1;
        }
    }
    return 0;
}

// 1.12a: 0x6FB1B280 (__stdcall)
void __stdcall CHARSTATS_DrawPanel()
{
    D2UnitStrc* pPlayer = PLAYER_GetLocalPlayer();
    if (!pPlayer)
    {
        return;
    }

    D2Win_10127_SetFont(1);
    // Draws character name, class, level, experience, strength, dexterity, vitality, energy,
    // fire/cold/lightning/poison resistances, defense, stamina, life, mana, and stat-up buttons.
}

// 1.12a: 0x6FB1C0A0 (__fastcall)
int32_t __fastcall CHARSTATS_HandleMouseUp(void* pMsg)
{
    int32_t nMouseX = *(int32_t*)((uint8_t*)pMsg + 0x08);
    int32_t nMouseY = *(int32_t*)((uint8_t*)pMsg + 0x0C);

    D2UnitStrc* pPlayer = PLAYER_GetLocalPlayer();
    int32_t nStatPts = D2Common_10519_GetUnitStatSigned(pPlayer, 4 /* STAT_STATPTS */, 0);

    for (int32_t i = 0; i < 4; ++i)
    {
        D2StatButtonDescStrc* pBtn = &g_aCharStatButtons[i];
        int32_t nPrevState = pBtn->nState;
        pBtn->nState = 0;

        if (nPrevState == 1 && nStatPts > 0)
        {
            int32_t nBtnX = pBtn->nX;
            int32_t nBtnY = pBtn->nY;
            if (nMouseX >= nBtnX && nMouseX <= nBtnX + pBtn->nWidth &&
                nMouseY >= nBtnY - pBtn->nHeight && nMouseY <= nBtnY)
            {
                NET_SendStatButtonPacket(0x3A, (uint16_t)pBtn->nStatId, 0);
                return 1;
            }
        }
    }
    return 0;
}

// 1.12a: 0x6FB1C260 (__stdcall)
void __stdcall CHARSTATS_LoadButtonCellFiles()
{
    if (!g_pCharStatBtnCellFile)
    {
        g_pCharStatBtnCellFile = ARCHIVE_LoadCellFile("Panel\\buysellbtn", 0);
    }
}

// ============================================================================
// PART 4: Warden Client Handler (..\Source\D2Client\WARDEN\WardenClient.cpp)
//         0x6FB1C3A0 .. 0x6FB1CE20
// ============================================================================

// 1.12a: 0x6FB1C3A0 (__fastcall)
void __fastcall WARDEN_RC4KeySchedule(uint8_t* pState258, const uint8_t* pKey, uint32_t nKeyLen)
{
    for (uint32_t i = 0; i < 256; ++i)
    {
        pState258[i] = (uint8_t)i;
    }
    pState258[256] = 0;
    pState258[257] = 0;

    uint8_t j = 0;
    uint32_t nKeyIdx = 0;
    for (uint32_t i = 0; i < 256; ++i)
    {
        j = (uint8_t)(j + pState258[i] + pKey[nKeyIdx]);
        nKeyIdx = (nKeyIdx + 1) % nKeyLen;

        uint8_t tmp = pState258[i];
        pState258[i] = pState258[j];
        pState258[j] = tmp;
    }
}

// 1.12a: 0x6FB1C410 (__fastcall)
void __fastcall WARDEN_RC4Crypt(uint8_t* pData, uint8_t* pState258, int32_t nDataLen)
{
    uint8_t x = pState258[256];
    uint8_t y = pState258[257];

    for (int32_t k = 0; k < nDataLen; ++k)
    {
        x = (uint8_t)(x + 1);
        uint8_t sx = pState258[x];
        y = (uint8_t)(y + sx);
        uint8_t sy = pState258[y];
        pState258[x] = sy;
        pState258[y] = sx;
        pData[k] ^= pState258[(uint8_t)(sx + sy)];
    }

    pState258[256] = x;
    pState258[257] = y;
}

// 1.12a: 0x6FB1C470 (__stdcall)
void __stdcall WARDEN_CleanupDownloadBuffers()
{
    if (g_pWardenDownloadBuffer)
    {
        Fog_10043_FreeClientMemory(g_pWardenDownloadBuffer, "..\\Source\\D2Client\\WARDEN\\WardenClient.cpp", 215, 0);
        g_pWardenDownloadBuffer = nullptr;
    }
    g_dwWardenExpectedModSize = 0;
    g_dwWardenReceivedModBytes = 0;
}

// 1.12a: 0x6FB1C4B0 (__fastcall)
int32_t __fastcall WARDEN_VerifyRSASignature(uint8_t* pOutBlock256, const uint8_t* pSignature256)
{
    void* hOut = nullptr;
    void* hBase = nullptr;
    void* hExp = nullptr;
    void* hMod = nullptr;

    Storm_624_SBigNew(&hOut);
    Storm_606_SBigFromBinary(&hBase, pSignature256, 256);
    Storm_606_SBigFromBinary(&hExp, (const void*)0x6FB81734, 4);
    Storm_606_SBigFromBinary(&hMod, (const void*)0x6FB81738, 256);
    Storm_628_SBigPowMod(hOut, hBase, hExp, hMod);

    size_t nWritten = 0;
    Storm_636_SBigToBinaryBuffer(hOut, pOutBlock256, 256, &nWritten);

    Storm_604_SBigDel(hOut);
    Storm_604_SBigDel(hBase);
    Storm_604_SBigDel(hExp);
    Storm_604_SBigDel(hMod);
    return 1;
}

// 1.12a: 0x6FB1C570 (__stdcall)
void* __stdcall WARDEN_HostAllocMemory(uint32_t dwSize)
{
    return Fog_10042_AllocClientMemory((int32_t)dwSize, "..\\Source\\D2Client\\WARDEN\\WardenClient.cpp", 268, 0);
}

// 1.12a: 0x6FB1C590 (__stdcall)
void __stdcall WARDEN_HostFreeMemory(void* pMem)
{
    Fog_10043_FreeClientMemory(pMem, "..\\Source\\D2Client\\WARDEN\\WardenClient.cpp", 274, 0);
}

// 1.12a: 0x6FB1C5B0 (__stdcall)
void __stdcall WARDEN_ShutdownClient()
{
    WARDEN_CleanupDownloadBuffers();
    if (g_pWardenSavedRC4Keys)
    {
        Fog_10043_FreeClientMemory(g_pWardenSavedRC4Keys, "..\\Source\\D2Client\\WARDEN\\WardenClient.cpp", 283, 0);
        g_pWardenSavedRC4Keys = nullptr;
    }
    g_dwWardenSavedRC4Size = 0;
}

// 1.12a: 0x6FB1C5D0 (__stdcall)
int32_t __stdcall WARDEN_HostSetRC4Keys(const void* pKeyData, uint32_t dwSize)
{
    if (g_pWardenSavedRC4Keys)
    {
        Fog_10043_FreeClientMemory(g_pWardenSavedRC4Keys, "..\\Source\\D2Client\\WARDEN\\WardenClient.cpp", 295, 0);
    }
    g_pWardenSavedRC4Keys = Fog_10042_AllocClientMemory((int32_t)dwSize, "..\\Source\\D2Client\\WARDEN\\WardenClient.cpp", 297, 0);
    memcpy(g_pWardenSavedRC4Keys, pKeyData, dwSize);
    g_dwWardenSavedRC4Size = dwSize;
    return 1;
}

// 1.12a: 0x6FB1C620 (__stdcall)
int32_t __stdcall WARDEN_HostGetRC4Keys(void* pKeyBuf, uint32_t* pdwSize)
{
    if (!g_pWardenSavedRC4Keys || *pdwSize < g_dwWardenSavedRC4Size)
    {
        return 0;
    }
    memcpy(pKeyBuf, g_pWardenSavedRC4Keys, g_dwWardenSavedRC4Size);
    *pdwSize = g_dwWardenSavedRC4Size;
    return 1;
}

// 1.12a: 0x6FB1C660 (__stdcall)
void __stdcall WARDEN_TickClient()
{
    if (!g_pWardenModuleInstance || !g_pWardenModule || !g_pWardenModule->pfTick)
    {
        return;
    }

    uint32_t dwNow = GetTickCount();
    if (dwNow - g_dwWardenLastTick >= 100)
    {
        g_dwWardenLastTick = dwNow;
        D2UnitStrc* pPlayer = PLAYER_GetLocalPlayer();
        int32_t nUnitId = pPlayer ? *(int32_t*)((uint8_t*)pPlayer + 0x0C) : 0;
        g_pWardenModule->pfTick(g_pWardenModuleInstance, nUnitId, 0);
    }
}

// 1.12a: 0x6FB1C830 (__stdcall)
void __stdcall WARDEN_HostSendPacket0x66(const uint8_t* pData, int32_t nLen)
{
    if (pData && nLen > 0)
    {
        NET_SendVariablePacket(0x66, pData, nLen);
    }
}

// 1.12a: 0x6FB1C870 (__stdcall)
void __stdcall WARDEN_UnloadModule()
{
    if (g_pWardenModuleInstance && g_pWardenModule && g_pWardenModule->pfReleaseModule)
    {
        g_pWardenModule->pfReleaseModule(g_pWardenModuleInstance);
        g_pWardenModuleInstance = nullptr;
    }

    if (g_pWardenLoadedImage)
    {
        VirtualFree(g_pWardenLoadedImage, 0, MEM_RELEASE);
        g_pWardenLoadedImage = nullptr;
        g_pWardenModule = nullptr;
    }
}

// 1.12a: 0x6FB1C8A0 (__stdcall)
FARPROC __stdcall WARDEN_GetImportProcAddress(const char* szLibName, const char* szProcName)
{
    HMODULE hMod = GetModuleHandleA(szLibName);
    if (!hMod)
    {
        hMod = LoadLibraryA(szLibName);
        if (!hMod)
        {
            return nullptr;
        }
    }
    return GetProcAddress(hMod, szProcName);
}

// 1.12a: 0x6FB1C8F0 (__fastcall)
D2WardenModuleExportTableStrc* __fastcall WARDEN_DecryptAndMapModule(int32_t nCompressedSize, const uint8_t* pEncryptedMod, const uint8_t* pRC4Key16)
{
    // Verifies MD5, RC4 decrypts with pRC4Key16, checks 'SIGN' RSA signature, zlib-inflates,
    // and maps custom PE-lite sections into VirtualAlloc(PAGE_EXECUTE_READWRITE) memory.
    (void)nCompressedSize;
    (void)pEncryptedMod;
    (void)pRC4Key16;
    return g_pWardenModule;
}

// 1.12a: 0x6FB1CBD0 (__stdcall)
int32_t __stdcall WARDEN_InstantiateLoadedModule()
{
    if (!g_pWardenModule || !g_pWardenModule->pfInitModule)
    {
        return 0;
    }

    g_tWardenHostCallbacks.pfSendPacket       = WARDEN_HostSendPacket0x66;
    g_tWardenHostCallbacks.pfCheckModuleCache = nullptr;
    g_tWardenHostCallbacks.pfLoadClientModule = nullptr;
    g_tWardenHostCallbacks.pfAllocMem         = WARDEN_HostAllocMemory;
    g_tWardenHostCallbacks.pfFreeMem          = WARDEN_HostFreeMemory;
    g_tWardenHostCallbacks.pfSetRC4Keys       = WARDEN_HostSetRC4Keys;
    g_tWardenHostCallbacks.pfGetRC4Keys       = WARDEN_HostGetRC4Keys;
    g_pWardenHostCallbacks                    = &g_tWardenHostCallbacks;

    g_pWardenModuleInstance = g_pWardenModule->pfInitModule(&g_pWardenHostCallbacks);
    return (g_pWardenModuleInstance != nullptr) ? 1 : 0;
}

// 1.12a: 0x6FB1CC60 (__fastcall)
int32_t __fastcall WARDEN_HostLoadSecondaryModule(const uint8_t* pRC4Key16, const uint8_t* pModData, int32_t nModSize)
{
    WARDEN_UnloadModule();
    g_pWardenModule = WARDEN_DecryptAndMapModule(nModSize, pModData, pRC4Key16);
    if (!g_pWardenModule)
    {
        return 0;
    }
    return WARDEN_InstantiateLoadedModule();
}

// 1.12a: 0x6FB1CCD0 (__stdcall)
D2WardenModuleExportTableStrc* __stdcall WARDEN_LoadModuleFromCacheOrBuiltIn(const uint8_t* pMd5Digest16, const uint8_t* pRC4Key16)
{
    if (Storm_495_SMemCmp(pMd5Digest16, (const void*)0x6FB82AF0, 16) == 0)
    {
        return WARDEN_DecryptAndMapModule(0x12B2, (const uint8_t*)0x6FB81838, (const uint8_t*)0x6FB82B00);
    }

    char szModFileName[40];
    for (int32_t i = 0; i < 16; ++i)
    {
        sprintf(&szModFileName[i * 2], "%02x", pMd5Digest16[i]);
    }
    Storm_501_SStrCopy(&szModFileName[32], ".mod", 5);
    return nullptr;
}

// 1.12a: 0x6FB1CDA0 (__stdcall)
int32_t __stdcall WARDEN_HostCheckModuleCache(const uint8_t* pMd5Digest16, const uint8_t* pRC4Key16)
{
    return (WARDEN_LoadModuleFromCacheOrBuiltIn(pMd5Digest16, pRC4Key16) != nullptr) ? 1 : 0;
}

// 1.12a: 0x6FB1C6D0 (__fastcall)
void __fastcall WARDEN_HandleServerPacket0xAE(const uint8_t* pPacket)
{
    uint16_t wPayloadLen = *(const uint16_t*)(pPacket + 1);
    const uint8_t* pPayload = pPacket + 3;

    if (g_pWardenModuleInstance && g_pWardenModule && g_pWardenModule->pfProcessPacket)
    {
        uint32_t dwHandled = 0;
        g_pWardenModule->pfProcessPacket(g_pWardenModuleInstance, pPayload, (int32_t)wPayloadLen, &dwHandled);
    }
}

// 1.12a: 0x6FB1CDC0 (__stdcall)
int32_t __stdcall WARDEN_InitClient(int32_t nSeed)
{
    WARDEN_UnloadModule();
    g_nWardenInitialSeed = nSeed;

    HMODULE hBnClient = GetModuleHandleA("Bnclient.dll");
    if (!hBnClient)
    {
        return 0;
    }

    typedef void* (__stdcall* FnQueryInterface)();
    FnQueryInterface pfQueryInterface = (FnQueryInterface)GetProcAddress(hBnClient, "QueryInterface");
    if (!pfQueryInterface)
    {
        return 0;
    }

    g_pBnClientInterface = pfQueryInterface();
    if (!g_pBnClientInterface)
    {
        return 0;
    }

    g_pWardenModule = WARDEN_LoadModuleFromCacheOrBuiltIn((const uint8_t*)0x6FB82AF0, (const uint8_t*)0x6FB82B00);
    WARDEN_InstantiateLoadedModule();
    return 1;
}
