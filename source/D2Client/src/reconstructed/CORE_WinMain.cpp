// =============================================================================
// Diablo II 1.12a - D2Client.dll Reconstructed Source
// Slices:
//   1. Engine.h & DataTable / Storm Intrusive List Helpers (0x6FAB1000..0x6FAB1D9F)
//   2. UI/waygate.cpp                                      (0x6FAC09D0..0x6FAC1FC0)
//   3. CORE/WINMAIN.CPP, Quest/Boss FX, Weather/Env,       (0x6FAC1FE0..0x6FAC7CD8)
//      Ambient Critters/DRLG Room Init, Smacker Cutscenes
//   4. DLL Exports, DllMain, & QueryInterface              (0x6FB4E940..0x6FB53650)
//
// ImageBase: 0x6FAB0000 (32-bit MSVC x86)
// =============================================================================

#ifndef D2_VERSION_MAJOR
#define D2_VERSION_112A 1
#define D2_VERSION_MAJOR 1
#define D2_VERSION_MINOR 12
#define D2_VERSION_PATCH 'A'
#endif

#ifndef D2CLIENT_DLL_DECL
#define D2CLIENT_DLL_DECL __declspec(dllexport)
#endif
#ifndef D2COMMON_DLL_DECL
#define D2COMMON_DLL_DECL __declspec(dllimport)
#endif
#ifndef FOG_DLL_DECL
#define FOG_DLL_DECL __declspec(dllimport)
#endif
#ifndef STORM_DLL_DECL
#define STORM_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2GFX_DLL_DECL
#define D2GFX_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2WIN_DLL_DECL
#define D2WIN_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2LANG_DLL_DECL
#define D2LANG_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2CMP_DLL_DECL
#define D2CMP_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2SOUND_DLL_DECL
#define D2SOUND_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2NET_DLL_DECL
#define D2NET_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2GAME_DLL_DECL
#define D2GAME_DLL_DECL __declspec(dllimport)
#endif
#ifndef D2MCPCLIENT_DLL_DECL
#define D2MCPCLIENT_DLL_DECL __declspec(dllimport)
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <D2BasicTypes.h>
#include <D2Config.h>
#include <D2Constants.h>
#include <D2Dll.h>
#include <Fog.h>
#include <Storm.h>
#include <D2DataTbls.h>
#include <DataTbls/ItemsTbls.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/LevelsTbls.h>
#include <DataTbls/MissilesIds.h>
#include <DataTbls/MissilesTbls.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/MonsterTbls.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/SkillsTbls.h>
#include <Units/Missile.h>
#include <Units/Monster.h>
#include <Units/UnitFinds.h>
#include <Units/UnitRoom.h>
#include <Units/Units.h>
#include <Path/Path.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Environment.h>
#include <D2Inventory.h>
#include <D2Log.h>
#include <D2Monsters.h>
#include <D2QuestRecord.h>
#include <D2Seed.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2Waypoints.h>
#include <UselessOrdinals.h>
#include <D2CMP.h>
#include <D2Gfx.h>
#include <DrawMode.h>
#include <Scale.h>
#include <Texture.h>
#include <Window.h>
#include <D2Lang.h>
#include <D2StrTable.h>
#include <D2Unicode.h>
#include <D2WinArchive.h>
#include <D2WinFont.h>
#include <D2WinMain.h>
#include <D2WinPalette.h>

static_assert(sizeof(void*) == 4, "D2Client 1.12a requires 32-bit x86 compilation");

// =============================================================================
// Internal Client & Engine Struct Definitions
// =============================================================================

#pragma pack(push, 1)

// Storm TSLink / TSExplicitList intrusive doubly-linked list node & header
struct D2StormLinkStrc
{
    D2StormLinkStrc* pNext;     // +0x00 (positive object ptr, nullptr if unlinked, or negative ~ptr if terminator)
    intptr_t         nPrevLink; // +0x04 (<0: ~addr of predecessor pNext field; >=0: byte delta relative to pNext->nPrevLink)
};

struct D2StormListStrc
{
    intptr_t        nLinkOffset;  // +0x00
    D2StormLinkStrc tTerminator;  // +0x04 (+0x04: pLastOrTerm, +0x08: pHead)
};

// Chat / Quest Unicode text buffers initialized by Engine.h inline constructors
struct D2ChatLineRowStrc
{
    uint8_t pad00[7];        // +0x00
    Unicode wszText[300];    // +0x07 (0x258 bytes)
    uint8_t pad25F[11];      // +0x25F (total stride = 0x26A)
};
static_assert(sizeof(D2ChatLineRowStrc) == 0x26A, "D2ChatLineRowStrc size must be 0x26A");

struct D2ChatLineGridStrc
{
    uint8_t           hdr00[0x118]; // +0x00
    D2ChatLineRowStrc aRows[6];     // +0x118
};

struct D2QuestSpeechBufferStrc
{
    Unicode aPages[6][1024]; // +0x0000 (6 * 0x800 = 0x3000 bytes)
};

static_assert(sizeof(D2GfxDataStrc) == 0x48, "D2GfxDataStrc size must be 0x48");

// Client unit ballistic height / vertical Z arc data attached at ((D2ClientUnitGfxStrc*)pUnit->pGfxData)->pBallistic
struct D2ClientBallisticHeightStrc
{
    uint32_t dwFlags;   // +0x00 (bit 0x20: ballistic Z active)
    int32_t  nOffsetX;  // +0x04
    int32_t  nOffsetY;  // +0x08
    int32_t  nOffsetZ;  // +0x0C (12-bit fixed point)
    int32_t  nVelX;     // +0x10
    int32_t  nVelY;     // +0x14
    int32_t  nVelZ;     // +0x18 (12-bit fixed point)
    int32_t  nUnk1C[3]; // +0x1C..+0x27
    int32_t  nAccelX;   // +0x28
    int32_t  nAccelY;   // +0x2C
    int32_t  nAccelZ;   // +0x30
};

struct D2ClientUnitGfxStrc
{
    uint8_t                      pad00[0x30]; // +0x00
    D2ClientBallisticHeightStrc* pBallistic;  // +0x30
};

// Fog TDataList pool header (FOG ordinals #10060..#10065)
struct D2FogDataListStrc
{
    uint8_t pad00[0x110];     // +0x000
    int32_t nMaxAllocatedIdx; // +0x110
    int32_t nActiveCount;     // +0x114
    int32_t nCapacity;        // +0x118
    void*   pElements;        // +0x11C
};

// Environment rain/snow particle (stride = 0x28 = 40 bytes)
struct D2EnvParticleStrc
{
    uint16_t wSlotSeq;      // +0x00 (non-zero when slot active)
    uint16_t wPad02;        // +0x02
    int32_t  nX;            // +0x04
    int32_t  nY;            // +0x08
    int32_t  nEndY;         // +0x0C
    int32_t  nLengthOrSize; // +0x10 (snowflake size 0..7 or rain line length)
    int32_t  nSpeed;        // +0x14
    int32_t  nPhase;        // +0x18 (0..511 sine phase for snowflake drift)
    int32_t  bLanded;       // +0x1C
    int32_t  nMeltFrames;   // +0x20
    uint8_t  nColor;        // +0x24
    uint8_t  nAlpha;        // +0x25
    uint8_t  pad26[2];      // +0x26
};
static_assert(sizeof(D2EnvParticleStrc) == 0x28, "D2EnvParticleStrc size must be 0x28");

// Environment rain splash / swamp bubble overlay particle (stride = 0x18 = 24 bytes)
struct D2EnvOverlayParticleStrc
{
    uint16_t wSlotSeq;      // +0x00 (non-zero when slot active)
    uint16_t wPad02;        // +0x02
    int32_t  nX;            // +0x04
    int32_t  nY;            // +0x08
    int32_t  nCellVariant;  // +0x0C (0..3 index into Rain1..4 or bubble1..4)
    int32_t  nFrame;        // +0x10
    int32_t  nFrameTicks;   // +0x14
};
static_assert(sizeof(D2EnvOverlayParticleStrc) == 0x18, "D2EnvOverlayParticleStrc size must be 0x18");

// Window / Storm UI message dispatch payload
struct D2WinMsgStrc
{
    HWND     hWnd;       // +0x00
    UINT     nMessage;   // +0x04
    WPARAM   wParam;     // +0x08
    union
    {
        LPARAM lParam;   // +0x0C
        struct
        {
            uint16_t nX; // +0x0C
            uint16_t nY; // +0x0E
        };
    };
    uint32_t dwCommand;  // +0x10
    uint32_t dwKeyFlags; // +0x14
    BOOL     bHandled;   // +0x18
    LRESULT  lResult;    // +0x1C
};

struct D2WaypointPanelMsgEntryStrc
{
    int32_t nMsgCategory;                         // +0x00
    uint32_t uMessage;                            // +0x04
    void (__stdcall* pfnHandler)(D2WinMsgStrc*);  // +0x08
};

struct D2PanelDescriptorStrc
{
    uint8_t pad00[0x48];       // +0x00
    int32_t nCutsceneCloseMode;// +0x48
    int32_t nUnk4C;            // +0x4C
    int32_t nWaypointCloseMode;// +0x50
};

// S->C 0x63 Open Waypoint Menu packet
struct D2GSPacketSrv63Strc
{
    uint8_t           nHeader;      // +0x00 (0x63)
    uint32_t          dwWaygateId;  // +0x01
    D2WaypointDataStrc tWaypoints;  // +0x05
};

// C->S 0x49 Waypoint Menu Interact / Cancel packet (9 bytes)
struct D2GSPacketClt49Strc
{
    uint8_t  nHeader;      // +0x00 (0x49)
    uint32_t dwWaygateId;  // +0x01
    uint32_t dwDestLevel;  // +0x05 (0 = close/cancel)
};
static_assert(sizeof(D2GSPacketClt49Strc) == 9, "D2GSPacketClt49Strc size must be 9");

// Client monster movement command block passed to UNITMODE_StartClientMonsterMode (0x6FAE48D0)
struct D2ClientMonsterMoveCmdStrc
{
    union
    {
        int32_t nTargetX;     // +0x00
        int32_t nTargetMode;  // +0x00
    };
    union
    {
        int32_t  nTargetY;    // +0x04
        uint32_t dwTargetGUID;// +0x04
    };
    int32_t nUnk08;           // +0x08
    int32_t nPathType;        // +0x0C
    int32_t nOverridePathType;// +0x10
    int32_t nMaxDistance;     // +0x14
    int32_t nUnk18;           // +0x18
};
static_assert(sizeof(D2ClientMonsterMoveCmdStrc) == 0x1C, "D2ClientMonsterMoveCmdStrc size must be 0x1C");

// Client monster spawn descriptor passed to UNIT_AllocClientMonster (0x6FAD0EB0)
struct D2MonsterSpawnDescStrc
{
    uint32_t dwUnitGUID;  // +0x00
    int32_t  nClassId;    // +0x04
    int32_t  nX;          // +0x08
    int32_t  nY;          // +0x0C
    uint16_t wTypeFlags;  // +0x10
    uint16_t wPad12;      // +0x12
    int32_t  nLifePct;    // +0x14
    int32_t  nAnimMode;   // +0x18
};
static_assert(sizeof(D2MonsterSpawnDescStrc) == 0x1C, "D2MonsterSpawnDescStrc size must be 0x1C");

// Smacker/Bink cutscene descriptor table entry (12 bytes at 0x6FBA8A68)
struct D2CutsceneEntryStrc
{
    int32_t     nCutsceneId;     // +0x00
    const char* szFileFormat;    // +0x04
    uint8_t     bLocalizedPath;  // +0x08
    uint8_t     bResolutionFmt;  // +0x09
    uint16_t    wPad0A;          // +0x0A
};
static_assert(sizeof(D2CutsceneEntryStrc) == 0x0C, "D2CutsceneEntryStrc size must be 0x0C");

// D2Client DLL Interface returned by QueryInterface (@10004, 0x6FB535D0)
struct D2ClientInterfaceStrc
{
    int32_t (__fastcall* pfMainEntry)(D2ConfigStrc* pConfig); // +0x00 (0x6FB53600)
};

// Storm SCmd callback payload passed to command-line switch handlers
struct D2StormCmdCallbackArgsStrc
{
    uint32_t    dwArgFlags; // +0x00
    int32_t     nArgId;     // +0x04
    const char* szSwitch;   // +0x08
    uint32_t    dwUnk0C[3]; // +0x0C..+0x17
    const char* szArgument; // +0x18
};

// Storm command-line switch registration descriptor (16 bytes per entry at 0x6FB83510)
struct D2StormCmdArgDescStrc
{
    uint32_t    dwFlags;    // +0x00
    int32_t     nArgId;     // +0x04 (-1)
    const char* szSwitch;   // +0x08
    BOOL (__stdcall* pfnCallback)(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra); // +0x0C
};
static_assert(sizeof(D2StormCmdArgDescStrc) == 0x10, "D2StormCmdArgDescStrc size must be 0x10");

// Client monster AI delay & state overlay on D2MonsterDataStrc (+0x30 and +0x34)
struct D2ClientMonsterAiStateStrc
{
    int32_t nAiDelayFrames; // maps to D2MonsterDataStrc + 0x30 (pMonInteract union on client)
    int32_t bHasSeenPlayer; // maps to D2MonsterDataStrc + 0x34 (unk0x34 on client)
};

#pragma pack(pop)

inline D2ClientMonsterAiStateStrc* ENGINE_GetClientMonsterAiState(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
    {
        return nullptr;
    }
    return reinterpret_cast<D2ClientMonsterAiStateStrc*>(&pUnit->pMonsterData->pMonInteract);
}

// =============================================================================
// External Imports (Fog, Storm, D2Win, D2Lang, D2Game, CRT)
// =============================================================================

inline D2FogDataListStrc* FOG_10060_DataListCreate(const char* szName, int32_t nCapacity, int32_t nElementSize)
{
    return reinterpret_cast<D2FogDataListStrc*>(DATATBLS_CreateDataArray(szName, nCapacity, nElementSize));
}
inline void FOG_10061_DataListDestroy(D2FogDataListStrc* pList)
{
    DATATBLS_DestroyDataArray(reinterpret_cast<D2FogDataArrayStrc*>(pList));
}
inline void FOG_10062_DataListInit(D2FogDataListStrc* pList)
{
    DATATBLS_InitDataArray(reinterpret_cast<D2FogDataArrayStrc*>(pList));
}
inline void FOG_10063_DataListReset(D2FogDataListStrc* pList)
{
    DATATBLS_ResetDataArray(reinterpret_cast<D2FogDataArrayStrc*>(pList));
}
inline int32_t FOG_10064_DataListAllocHandle(D2FogDataListStrc* pList)
{
    return DATATBLS_AllocDataArrayElement(reinterpret_cast<D2FogDataArrayStrc*>(pList));
}
inline void FOG_10065_DataListFreeHandle(D2FogDataListStrc* pList, int32_t nHandle)
{
    DATATBLS_FreeDataArrayElement(reinterpret_cast<D2FogDataArrayStrc*>(pList), nHandle);
}
inline void FOG_10101(int32_t bPoolSystem)
{
    FOG_CompactMemoryPoolSystem(reinterpret_cast<void*>(static_cast<intptr_t>(bPoolSystem)));
}
inline int32_t FOG_10191_IsHighResVideoSupported(void)
{
    return FOG_IsSlowLegacyCPU();
}
inline const char* Fog_10265(void)
{
    return FOG_GetCallerReturnAddress();
}

// D2Game.dll (C++ export in D2MOO)
void __stdcall GAME_SetInitSeed(int32_t nSeed);

// =============================================================================
// Cross-Module D2Client Function Declarations (Outside Slice VA Range)
// =============================================================================

struct D2ViewportStrc;

// 1.12a: 0x6FAB2D40 (custom: eax = nMonsterId, edx = nFlagIndex -> standard __fastcall)
extern BOOL __fastcall MONSTER_CheckMonStatsFlag(int32_t nMonsterId, int32_t nFlagIndex);

// 1.12a: 0x6FAC0800 (CORE/ARCHIVE.CPP shutdown)
extern void __stdcall ARCHIVE_ShutdownClientArchives(void);
// 1.12a: 0x6FAC0830 (CORE/ARCHIVE.CPP startup callback in gInitCallbacks_6FB83508)
extern BOOL __stdcall ARCHIVE_InitClientStartupCallback(void);
// 1.12a: 0x6FAC0860 (CORE/ARCHIVE.CPP initialize client MPQ archives)
extern BOOL __stdcall ARCHIVE_InitializeClientArchives(void);
// 1.12a: 0x6FAC0980 (UI/waygate.cpp: custom ebx = nLocalX, edi = nLocalY -> standard __fastcall)
extern int32_t __fastcall WAYGATE_HitTestEntry(int32_t nLocalX, int32_t nLocalY);

// 1.12a: 0x6FACA1F0 (UI/npcmenu.cpp: custom edi = dwNpcGUID -> standard __fastcall)
extern void __fastcall NPCMENU_CancelActiveInteraction(uint32_t dwNpcGUID);

// 1.12a: 0x6FACEAC0 (UNIT/CUnit.cpp: custom eax = pUnit -> standard __fastcall)
extern BOOL __fastcall UNIT_IsUnitDeadOrCorpse(D2UnitStrc* pUnit);
// 1.12a: 0x6FACF710 (UNIT/CUnit.cpp: custom eax = pUnit -> standard __fastcall)
extern D2UnitStrc* __fastcall UNIT_GetMorphedSourceUnit(D2UnitStrc* pUnit);
// 1.12a: 0x6FAD0620 (UNIT/CUnit.cpp: custom ecx = ppHashTable, edx = nHashIdx, [esp+4] = dwGUID, [esp+8] = nUnitType)
extern void __fastcall UNIT_RemoveClientSideUnitFromHash(D2UnitStrc** ppHashTable, uint32_t dwHashIdx, uint32_t dwUnitGUID, int32_t nUnitType);
// 1.12a: 0x6FAD0EB0 (UNIT/CUnit.cpp: __stdcall 6 args)
extern D2UnitStrc* __stdcall UNIT_AllocClientMonsterUnit(const D2MonsterSpawnDescStrc* pSpawnDesc, const uint8_t* pCompInfo, const uint8_t* pMonUMods, uint32_t dwNameSeedOrAsync, uint16_t wBossHcIdx, int32_t nIsClientAsync);
// 1.12a: 0x6FAD1320 (UNIT/CUnit.cpp: custom ecx = nClassId, edx = nX, [esp+4] = nY, [esp+8] = nUnitType, [esp+12] = nAnimMode)
extern D2UnitStrc* __fastcall UNIT_CreateClientUnitAtCoords(int32_t nClassId, int32_t nX, int32_t nY, int32_t nUnitType, int32_t nAnimMode);
// 1.12a: 0x6FAD1940 (MISSILE: custom edx = pOwner, [esp+4] = nMissileId, [esp+8] = nX, [esp+12] = nY, [esp+16] = nSkill, [esp+20] = nSkillLevel)
extern D2UnitStrc* __fastcall MISSILE_SpawnClientEffectAtCoords(int32_t nUnusedEcx, D2UnitStrc* pOwner, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkill, int32_t nSkillLevel);
// 1.12a: 0x6FAD4B20 (MISSILE: __fastcall ecx = D2MissileStrc*)
extern D2UnitStrc* __fastcall MISSILE_CreateClientMissile(D2MissileStrc* pMissileParams);

// 1.12a: 0x6FAE1AD0 (SOUND: stop all ambient/music channels before cutscene)
extern void __stdcall SOUND_StopAllForCutscene(void);
// 1.12a: 0x6FAE1FA0 (SOUND: custom ebx = nSoundId, [esp+4] = pUnit, [esp+8] = nTicksOrVol, [esp+12] = bPreload, [esp+16] = nFlags)
extern void* __fastcall SOUND_PlayClientSoundEx(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nVolumeOrDelay, int32_t bPreload, int32_t nFlags);
// 1.12a: 0x6FAE20D0 (SOUND: __fastcall ecx = pSoundChannel, [esp+4] = nRelX, [esp+8] = nRelY, [esp+12] = bWorldCoords)
extern void __fastcall SOUND_SetChannelPositionOffset(void* pChannel, int32_t nRelX, int32_t nRelY, int32_t bWorldCoords);
// 1.12a: 0x6FAE2E80 (UNIT/Monster.cpp: custom eax = pUnit, ecx = pSeed -> standard __fastcall)
extern int32_t* __fastcall MONSTER_GetClientIdleChatterState(D2UnitStrc* pUnit, D2SeedStrc* pSeed);
// 1.12a: 0x6FAE48D0 (UNIT/UnitMode.cpp: __fastcall ecx = nTargetMode, edx = pUnit, [esp+4] = pMoveCmd)
extern void __fastcall UNITMODE_StartClientMonsterMode(int32_t nTargetMode, D2UnitStrc* pUnit, D2ClientMonsterMoveCmdStrc* pMoveCmd);
// 1.12a: 0x6FAEB9F0 (CLEANUP: release remaining client registries on exit)
extern void __stdcall CLIENT_CleanupRegistryState(void);
// 1.12a: 0x6FAEBBB0, 0x6FAEBBF0 (Client state handlers 5 & 4)
extern int32_t __stdcall CLIENT_StateHandler_StartNet_6FAEBBB0(void);
extern int32_t __stdcall CLIENT_StateHandler_PrepareNet_6FAEBBF0(void);
// 1.12a: 0x6FAEF1B0 (UI/inv.cpp: __fastcall ecx = bReinitLayout)
extern void __fastcall INV_OnResolutionModeChanged(int32_t bReinitLayout);
// 1.12a: 0x6FAFD350, 0x6FAFD4A0, 0x6FAFD5E0 (Archive disk prompt callbacks)
extern BOOL __stdcall ARCHIVE_PromptClassicVideoDisc(void);
extern BOOL __stdcall ARCHIVE_PromptExpansionDiscB(void);
extern BOOL __stdcall ARCHIVE_PromptExpansionDiscA(void);
// 1.12a: 0x6FAFE710 (QUEST: __stdcall nQuestSoundId, nArg2, nArg3)
extern void __stdcall QUEST_TriggerNihlathakTauntSound(int32_t nSoundId, int32_t nArg2, int32_t nArg3);

// 1.12a: 0x6FB03450 (UI/inv.cpp: check if cursor holds an item)
extern D2UnitStrc* __stdcall INV_GetCursorItem(void);
// 1.12a: 0x6FB0DFF0 (VIEW: custom eax = nViewShiftState -> standard __fastcall)
extern void __fastcall VIEW_ApplyResolutionViewShift(int32_t nViewShiftState);
// 1.12a: 0x6FB0F8F0 (DRAW/dLightMap.cpp: custom eax = bCheckSpecial, [esp+4] = pRoom, [esp+8..20] = out intensity/R/G/B)
extern void __fastcall LIGHTMAP_QueryBaseAmbientRGB(BOOL bCheckSpecial, D2ActiveRoomStrc* pRoom, int32_t* pOutIntensity, uint8_t* pOutRed, uint8_t* pOutGreen, uint8_t* pOutBlue);

// 1.12a: 0x6FB1D0A0 (MSG: custom eax = nLayer, [esp+4] = hWnd, [esp+8] = pMsgTable)
extern void __fastcall MSG_UnregisterPanelHandlers(int32_t nLayer, HWND hWnd, const D2WaypointPanelMsgEntryStrc* pTable);
// 1.12a: 0x6FB1D100 (MSG: custom eax = nLayer, [esp+4] = hWnd, [esp+8] = pMsgTable)
extern void __fastcall MSG_RegisterPanelHandlers(int32_t nLayer, HWND hWnd, const D2WaypointPanelMsgEntryStrc* pTable);
// 1.12a: 0x6FB1D200 (Window procedure registered with D2Win_10014_SetWndProc)
extern LRESULT CALLBACK MSG_MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 1.12a: 0x6FB268E0 (Generate/fetch initial game seed into gdInitSeed_6FBB5378)
extern void __stdcall GAME_InitRandomSeed(void);
// 1.12a: 0x6FB291A0 (GAME/Msg.cpp: custom ebx = nSize, [esp+4] = pPacketData)
extern int32_t __fastcall MSG_SendRawClientPacket(size_t nSize, const void* pPacketData);
// 1.12a: 0x6FB297E0 (GAME/Msg.cpp: custom al = bOpcode, ecx = dw1, edx = dw2)
extern int32_t __fastcall MSG_SendByte2Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2);
// 1.12a: 0x6FB29900 (GAME/Msg.cpp: custom al = bOpcode)
extern int32_t __fastcall MSG_SendByte(uint8_t bOpcode);
// 1.12a: 0x6FB299D0 (Cutscene frame network keepalive callback)
extern BOOL __stdcall GAME_CutsceneKeepAliveCallback(void);
// 1.12a: 0x6FB2A000..0x6FB2A480 (Command-line switch table initializers in gCmdLineInitFns_6FB90528)
typedef void (__fastcall* D2CmdSwitchInitFn)(D2ConfigStrc* pContext);
// 1.12a: 0x6FB2A690, 0x6FB2BC80, 0x6FB2D2D0 (Client state handlers 2, 3, 1/6)
extern int32_t __stdcall CLIENT_StateHandler_GameLoop_6FB2A690(void);
extern int32_t __stdcall CLIENT_StateHandler_InitSession_6FB2BC80(void);
extern int32_t __stdcall CLIENT_StateHandler_ExitOrLeave_6FB2D2D0(void);
// 1.12a: 0x6FB2AB00 (Post-cutscene act transition / credits trigger)
extern void __stdcall GAME_OnCutsceneCompleted(void);
// 1.12a: 0x6FB2E0F0 (Prepare client UI/palette before video cutscene)
extern void __stdcall GAME_PrepareForCutscene(void);

// 1.12a: Internal UI widget destructors invoked by Engine.h scalar deleting destructors
extern void __stdcall  UI_DestroyWidgetG_6FB34B30(void* pWidget); // 0x6FB34B30
extern void __fastcall UI_DestroyWidgetH_6FB347D0(void* pWidget); // 0x6FB347D0
extern void __stdcall  UI_DestroyWidgetE_6FB4B740(void* pWidget); // 0x6FB4B740
extern void __fastcall UI_DestroyWidgetF_6FB4B3C0(void* pWidget); // 0x6FB4B3C0
extern void __stdcall  UI_DestroyWidgetC_6FB68440(void* pWidget); // 0x6FB68440
extern void __fastcall UI_DestroyWidgetD_6FB682F0(void* pWidget); // 0x6FB682F0
extern void __stdcall  UI_DestroyWidgetA_6FB6CC90(void* pWidget); // 0x6FB6CC90
extern void __fastcall UI_DestroyWidgetB_6FB6C8A0(void* pWidget); // 0x6FB6C8A0

// 1.12a: 0x6FB3AA10 (UI/ui.cpp: refresh open panel layout & cursor state)
extern void __stdcall UI_OnPanelClosedRefresh(void);
// 1.12a: 0x6FB3EF00 (UI/ui.cpp: __fastcall ecx = nPanelId, edx = nAction, [esp+4] = bMoveMouse)
extern int32_t __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t bMoveMouse);
// 1.12a: 0x6FB3F800 (UI/ui.cpp: update HUD layout for new resolution)
extern void __stdcall UI_OnResolutionModeChanged(void);

// 1.12a: 0x6FB41F40 (SOUND: custom ebx = nMusicOrSoundId, [esp+4] = bFlag)
extern void __fastcall SOUND_TriggerBossEventAudio(int32_t nAudioId, int32_t bFlag);
// 1.12a: 0x6FB430E0 (SOUND: custom eax = nSoundId, [esp+4..28] = pUnit, nChannel, a4, a5, a6, a7)
extern void __fastcall SOUND_PlayPositionalEventSound(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nGroup, int32_t a4, int32_t a5, int32_t a6, int32_t a7);
// 1.12a: 0x6FB4ECD0 (CURSOR: custom ebx = nNewMouseX, edi = nNewMouseY -> standard __fastcall)
extern void __fastcall UI_SetCursorPosition(int32_t nMouseX, int32_t nMouseY);

// 1.12a: 0x6FB6A990 (VIEW: __fastcall ecx = nViewportWidth, edx = nViewportTop, eax = nViewportHeight, esi = pViewport, [esp+4] = nViewportLeft)
extern void __fastcall VIEW_SetViewportBounds(D2ViewportStrc* pViewport, int32_t nLeft, int32_t nTop, int32_t nRight, int32_t nBottom);
// 1.12a: 0x6FB6EED0 (ARCHIVE: custom eax = szPath, [esp+4] = pOutParam, [esp+8] = nFlags)
extern D2CellFileStrc* __fastcall ARCHIVE_LoadClientUncompressedCellFile(const char* szPath, int32_t* pOutUnused, int32_t nFlags);
// 1.12a: 0x6FB6EF70 (ARCHIVE: custom eax = szPath, [esp+4] = nPaletteMode)
extern D2CellFileStrc* __fastcall ARCHIVE_LoadClientCellFile(const char* szPath, int32_t nPaletteMode);

// 1.12a: 0x6FB75280 (MISSILE: custom eax = pUnit -> initialize Shenk death catapult missile)
extern void __fastcall MISSILE_InitShenkCatapultProjectile(D2UnitStrc* pUnit);
// 1.12a: 0x6FB755B0 (MISSILE: custom eax = pMissile, ecx = nRangeOrVel, edx = nArg2, [esp+4] = nArg3)
extern void __fastcall MISSILE_InitBallisticVelocity(D2UnitStrc* pMissile, int32_t nVelParam, int32_t nArg2, int32_t nArg3);
// 1.12a: 0x6FB75730 (MISSILE: __stdcall pMissile -> finalize chicken/bird flight frames)
extern void __stdcall MISSILE_FinalizeCritterFlight(D2UnitStrc* pMissile);
// 1.12a: 0x6FB759E0 (SCREENSHAKE: reset screen shake state before cutscene)
extern void __stdcall VIEW_ResetScreenShake(void);
// 1.12a: 0x6FB75A30 (SCREENSHAKE: __stdcall nStringTblIdx, nArg2, nDuration, nMagnitude, nArg5, nArg6)
extern void __stdcall VIEW_StartBossQuakeAndTaunt(int32_t nTauntStrIdx, int32_t nArg2, int32_t nDurationFrames, int32_t nArg4, int32_t nArg5, int32_t nArg6);

// =============================================================================
// Forward Declarations for Internal Functions Needed by Tables
// =============================================================================

BOOL    __stdcall  WINMAIN_CmdArg_Help_6FAC2190(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra);
BOOL    __stdcall  WINMAIN_CmdArg_SetDefaultCmdLine_6FAC2120(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra);
BOOL    __stdcall  WINMAIN_CmdArg_Mode_6FAC22A0(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra);
BOOL    __stdcall  WINMAIN_CmdArg_PrecommitMemory_6FAC2140(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra);
BOOL    __stdcall  WINMAIN_CmdArg_ResetWindowClose_6FAC2370(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra);
BOOL    __stdcall  WINMAIN_CmdArg_SeedInit_6FAC2480(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra);
void    __stdcall  WAYGATE_HandleMouseDown(D2WinMsgStrc* pMsg);
void    __stdcall  WAYGATE_HandleMouseUp(D2WinMsgStrc* pMsg);
void    __stdcall  WAYGATE_HandleKeyInput(D2WinMsgStrc* pMsg);
int32_t __fastcall D2ClientInterface_Entry(D2ConfigStrc* pConfig);

void __stdcall QUESTFX_StartCairnStonesPortalSequence(void);
void __stdcall QUESTFX_TriggerCountessChestEffect(void);
void __stdcall QUESTFX_TriggerIzualDeathEffect(void);
void __stdcall QUESTFX_TriggerHoradricStaffOrificeBeam(void);
void __stdcall QUESTFX_TriggerDiabloSealBossAmbient(void);
void __stdcall QUESTFX_StartServerShutdownCountdown90s(void);
void __stdcall QUESTFX_TriggerForgeHammerShatterEffect(void);
void __stdcall QUESTFX_TriggerTowerTomeEffect(void);
void __stdcall QUESTFX_TriggerDurielWallOpenEffect(void);
void __stdcall QUESTFX_EnableNihlathakTauntFlag(void);

// =============================================================================
// 1.12a Global Variables, Read-Only Constants, & Tables
// =============================================================================

// --- String Constants (.rdata) ---
static const char gszEmptyLiteral_6FB80323[]        = "";                                      // 0x6FB80323
static const char gszDeleteTag_6FB80328[]           = "delete";                                // 0x6FB80328
static const char gszDiabloII_6FB803FC[]            = "Diablo II";                             // 0x6FB803FC
static const char gszDataGlobal_6FB80424[]          = "DATA\\GLOBAL";                          // 0x6FB80424
static const char gszArchiveCpp_6FB807A0[]          = "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP"; // 0x6FB807A0
static const char gszEngineH_6FB83A20[]             = "..\\Source\\D2Client/Engine.h";         // 0x6FB83A20
static const char gszDataLocal_6FB83C98[]           = "DATA\\LOCAL";                           // 0x6FB83C98
static const char gszBuySellBtnFmt_6FB84300[]       = "%s\\ui\\panel\\buysellbtn";             // 0x6FB84300
static const char gszStrFmt_6FB84638[]              = "%s";                                    // 0x6FB84638
static const char gszVideoGlobalFmt_6FB86D20[]      = "%s\\video\\%s";                         // 0x6FB86D20
static const char gszVideoLocalFmt_6FB86D2C[]       = "%s\\video\\%s\\%s";                     // 0x6FB86D2C
static const char gszD2GameDll_6FB86D3C[]           = "d2game.dll";                            // 0x6FB86D3C
static const char gszRes640x146_6FB86D48[]          = "640x146";                               // 0x6FB86D48
static const char gszRes640x292_6FB86D50[]          = "640x292";                               // 0x6FB86D50
static const char gszLangChi_6FB86D58[]             = "CHI";                                   // 0x6FB86D58
static const char gszLangEng_6FB86D5C[]             = "ENG";                                   // 0x6FB86D5C
static const char gszIpAddrFmt_6FB86D60[]           = "%d.%d.%d.%d";                           // 0x6FB86D60
static const char gszAuxBnetKey_6FB86D6C[]          = "Aux Battle.net";                        // 0x6FB86D6C
static const char gszBubble4Fmt_6FB86EA8[]          = "%s\\UncompOverlays\\bubble4";           // 0x6FB86EA8
static const char gszBubble3Fmt_6FB86EC4[]          = "%s\\UncompOverlays\\bubble3";           // 0x6FB86EC4
static const char gszBubble2Fmt_6FB86EE0[]          = "%s\\UncompOverlays\\bubble2";           // 0x6FB86EE0
static const char gszBubble1Fmt_6FB86EFC[]          = "%s\\UncompOverlays\\bubble1";           // 0x6FB86EFC
static const char gszRain4Fmt_6FB86F18[]            = "%s\\UncompOverlays\\Rain4";             // 0x6FB86F18
static const char gszRain3Fmt_6FB86F30[]            = "%s\\UncompOverlays\\Rain3";             // 0x6FB86F30
static const char gszRain2Fmt_6FB86F48[]            = "%s\\UncompOverlays\\Rain2";             // 0x6FB86F48
static const char gszRain1Fmt_6FB86F60[]            = "%s\\UncompOverlays\\Rain1";             // 0x6FB86F60
static const char gszEnvBubbles_6FB86F78[]          = "Environment Bubbles";                   // 0x6FB86F78
static const char gszEnvSplashes_6FB86F8C[]         = "Environment Splashes";                  // 0x6FB86F8C
static const char gszEnvParticles_6FB86FA4[]        = "Environment Particles";                 // 0x6FB86FA4
static const char gszNoName_6FB86FFC[]              = "noname";                                // 0x6FB86FFC
static const char gszDefaultCmdLine_6FB87004[]      = "Default Command Line";                  // 0x6FB87004
static const char gszWinMainCpp_6FB8701C[]          = "..\\Source\\D2Client\\CORE\\WINMAIN.CPP"; // 0x6FB8701C
static const char gszRseisUser_6FB87040[]           = "rseis";                                 // 0x6FB87040
static const char gszCmdHelpText_6FB87050[]         =                                          // 0x6FB87050
    "Available command line options:\n"
    "  -set[default command line]\t\tSet new default command line string\n"
    "  -lng[lngfile]\t\t\tUse \"lngfile\" for language resources\n";
static const char gszWaygateIconsFmt_6FB87114[]     = "%s\\ui\\menu\\waygateicons";            // 0x6FB87114
static const char gszWaygateTabsFmt_6FB8712C[]      = "%s\\ui\\menu\\waygatetabs";             // 0x6FB8712C
static const char gszExpWaygateTabsFmt_6FB87144[]   = "%s\\ui\\menu\\expwaygatetabs";          // 0x6FB87144
static const char gszWaygateBgFmt_6FB87160[]        = "%s\\ui\\menu\\waygatebackground";       // 0x6FB87160

// --- Waypoint Panel Message Dispatch Table (0x6FB815D8) ---
static const D2WaypointPanelMsgEntryStrc gWaypointPanelMsgTable_6FB815D8[6] = // 0x6FB815D8
{
    { 0, WM_LBUTTONDOWN, WAYGATE_HandleMouseDown },
    { 0, WM_LBUTTONUP,   WAYGATE_HandleMouseUp   },
    { 0, WM_RBUTTONDOWN, WAYGATE_HandleMouseDown },
    { 0, WM_RBUTTONUP,   WAYGATE_HandleMouseUp   },
    { 0, WM_CHAR,        WAYGATE_HandleKeyInput  },
    { 0, 0,              nullptr                 },
};

// --- Command-Line Startup Callbacks & Switch Table (0x6FB83508..0x6FB8358F) ---
static BOOL (__stdcall* const gStartupInitCallbacks_6FB83508[2])(void) = // 0x6FB83508
{
    ARCHIVE_InitClientStartupCallback,
    nullptr,
};

static const D2StormCmdArgDescStrc gStormCmdArgList_6FB83510[8] = // 0x6FB83510
{
    { 0x00000000u, -1, "?",    WINMAIN_CmdArg_Help_6FAC2190             },
    { 0x00000000u, -1, "h",    WINMAIN_CmdArg_Help_6FAC2190             },
    { 0x00000000u, -1, "help", WINMAIN_CmdArg_Help_6FAC2190             },
    { 0x00020000u, -1, "set",  WINMAIN_CmdArg_SetDefaultCmdLine_6FAC2120},
    { 0x00020000u, -1, "mode", WINMAIN_CmdArg_Mode_6FAC22A0             },
    { 0x00000000u, -1, "c",    WINMAIN_CmdArg_PrecommitMemory_6FAC2140  },
    { 0x00000000u, -1, "r",    WINMAIN_CmdArg_ResetWindowClose_6FAC2370 },
    { 0x00000000u, -1, "p",    WINMAIN_CmdArg_SeedInit_6FAC2480         },
};

// --- Snowflake Cross-Line Offset Table (0x6FB86E10..0x6FB86E9F) ---
// Size s (0..7) draws row [s] and row [s + 1] (0x6FB86E20 == &gSnowflakeLines_6FB86E10[1])
static const int32_t gSnowflakeLines_6FB86E10[9][4] = // 0x6FB86E10 (row 1 at 0x6FB86E20)
{
    {  0,  0,  0,  1 }, // [0] 0x6FB86E10
    { -1, -1,  0,  0 }, // [1] 0x6FB86E20
    {  0, -1,  0,  1 }, // [2] 0x6FB86E30
    {  1,  0, -1,  1 }, // [3] 0x6FB86E40
    {  1,  0,  0,  1 }, // [4] 0x6FB86E50
    {  2, -1, -1,  1 }, // [5] 0x6FB86E60
    {  1,  1, -2, -1 }, // [6] 0x6FB86E70
    {  2,  1, -2, -1 }, // [7] 0x6FB86E80
    {  2, -1, -2,  2 }, // [8] 0x6FB86E90
};

// --- Snow Alpha by Period of Day (0x6FB86EA0..0x6FB86EA7) ---
// Indexed at [nPeriodOfDay * 2 + 1] (0x6FB86EA1): Day=200(0xC8), Dusk=160(0xA0), Night=80(0x50), Dawn=160(0xA0)
static const uint8_t gSnowPeriodAlphaTable_6FB86EA0[8] = // 0x6FB86EA0 (odd bytes at 0x6FB86EA1)
{
    0x50, 0xC8, 0x40, 0xA0, 0x20, 0x50, 0x40, 0xA0
};

// --- Floating-Point Constants (.rdata) ---
static const float gfTwoPow32_6FB8751C         = 4294967296.0f; // 0x6FB8751C (0x4F800000)
static const float gfPortalPulseScale_6FB87534 = 80.0f;         // 0x6FB87534 (0x42A00000)
static const float gfRainSpeedScale_6FB87578   = -15.0f;        // 0x6FB87578 (0xC1700000)
static const float gfRainLengthScale_6FB8757C  = -8.0f;         // 0x6FB8757C (0xC1000000)
static const float gfSnowSpeedScale_6FB87580   = -28.0f;        // 0x6FB87580 (0xC1E00000)
static const float gfRainBaseSpeedMul_6FB87588 = 0.85f;         // 0x6FB87588 (0x3F59999A)
static const float gfRainGustSpeedMul_6FB8758C = 0.15f;         // 0x6FB8758C (0x3E19999A)
static const float gfMinSnowCosSpeed_6FB87590  = 0.25f;         // 0x6FB87590 (0x3E800000)
static const float gfInv256_6FB87598           = 0.00390625f;   // 0x6FB87598 (0x3B800000, 1.0f / 256.0f)

// --- Initialized Data (.data) ---
// [aliased] gnScreenWidth_6FB8C6E0
// [aliased] gnScreenHeight_6FB8C6E4
extern int32_t gnScreenPitch_6FB8C6E8;            // 0x6FB8C6E8 (default 640)

static D2ClientInterfaceStrc gD2ClientInterface_6FB8D020 = // 0x6FB8D020
{
    D2ClientInterface_Entry
};

extern D2PanelDescriptorStrc* gpPanelDescriptors_6FB8F2D0[38]; // 0x6FB8F2D0
extern int32_t                gnPanelDescriptorCount_6FB8F368; // 0x6FB8F368 (initialized to 0x26 = 38)
extern int32_t                g_nLeftMouseButtonState;         // 0x6FB90354
extern int32_t                g_nRightMouseButtonState;        // 0x6FB90358
extern int32_t                g_bConfigApplyResult;            // 0x6FB90524
extern const D2CmdSwitchInitFn g_apfnApplyConfigCallbacks[49];  // 0x6FB90528
#define gnInteractSnapDistX_6FB90354    g_nLeftMouseButtonState
#define gnInteractSnapDistY_6FB90358    g_nRightMouseButtonState
#define gbCmdLineInitOk_6FB90524        g_bConfigApplyResult
#define gpfnCmdLineSwitchInits_6FB90528 g_apfnApplyConfigCallbacks
// [aliased] gdwNextClientCritterGUID_6FBA6A80

// Cutscene table (0x6FBA8A68, 8 entries)
static const D2CutsceneEntryStrc gCutsceneTable_6FBA8A68[8] = // 0x6FBA8A68
{
    { 0, "d2_promo.bik",    0, 0, 0 }, // [0] 0x6FBA8A68
    { 1, "d2intro%s.bik",   1, 1, 0 }, // [1] 0x6FBA8A74
    { 2, "ACT02START%s.BIK",1, 1, 0 }, // [2] 0x6FBA8A80
    { 3, "ACT03START%s.BIK",1, 1, 0 }, // [3] 0x6FBA8A8C
    { 4, "ACT04START%s.BIK",1, 1, 0 }, // [4] 0x6FBA8A98
    { 5, "ACT04END%s.BIK",  1, 1, 0 }, // [5] 0x6FBA8AA4
    { 6, "D2X_INTRO_%s.BIK",1, 1, 0 }, // [6] 0x6FBA8AB0 (szFileFormat @ 0x6FBA8AB4, bLocalizedPath @ 0x6FBA8AB8)
    { 7, "D2X_OUT_%s.BIK",  1, 1, 0 }, // [7] 0x6FBA8ABC
};
static int32_t gnCutsceneCount_6FBA8AC8          = 8;  // 0x6FBA8AC8
static BOOL    gbLightningPlayThunder_6FBA8ACC   = 1;  // 0x6FBA8ACC
int32_t  gnTristramPortalFadeTick_6FBA8AD0 = -1; // 0x6FBA8AD0
static int32_t gnDiabloSealFadeTick_6FBA8AD4     = -1; // 0x6FBA8AD4
uint32_t gdwDiabloUnitGUID_6FBA8AD8       = 0xFFFFFFFFu; // 0x6FBA8AD8

// Quest / Boss Client Event Callback Table (0x6FBA8AE0, 20 entries: indices 0..19)
static void (__stdcall* const gpfnQuestClientEventTable_6FBA8AE0[20])(void) = // 0x6FBA8AE0
{
    QUESTFX_StartCairnStonesPortalSequence, // [0]  0x6FAC2970
    QUESTFX_TriggerCountessChestEffect,     // [1]  0x6FAC3230
    nullptr,                                // [2]
    QUESTFX_TriggerIzualDeathEffect,        // [3]  0x6FAC3980
    nullptr,                                // [4]
    nullptr,                                // [5]
    QUESTFX_TriggerHoradricStaffOrificeBeam,// [6]  0x6FAC3180
    nullptr,                                // [7]
    nullptr,                                // [8]
    nullptr,                                // [9]
    nullptr,                                // [10]
    nullptr,                                // [11]
    QUESTFX_TriggerDiabloSealBossAmbient,   // [12] 0x6FAC3920
    QUESTFX_StartServerShutdownCountdown90s,// [13] 0x6FAC2950
    QUESTFX_TriggerForgeHammerShatterEffect,// [14] 0x6FAC3880
    nullptr,                                // [15]
    QUESTFX_TriggerTowerTomeEffect,         // [16] 0x6FAC30A0
    QUESTFX_TriggerDurielWallOpenEffect,    // [17] 0x6FAC3020
    nullptr,                                // [18]
    QUESTFX_EnableNihlathakTauntFlag,       // [19] 0x6FAC2940
};

static int32_t gnCurrentClientState_6FBA8B30 = 3; // 0x6FBA8B30
static int32_t (__stdcall* const gpfnClientStateHandlers_6FBA8B34[7])(void) = // 0x6FBA8B34
{
    nullptr,                                 // [0] 0x6FBA8B34: D2CLIENT_STATE_NONE
    nullptr,                                 // [1] 0x6FBA8B38: D2CLIENT_STATE_EXIT (exits loop before indexing)
    CLIENT_StateHandler_ExitOrLeave_6FB2D2D0,// [2] 0x6FBA8B3C: 0x6FB2D2D0 GAME_MainLoop
    CLIENT_StateHandler_GameLoop_6FB2A690,   // [3] 0x6FBA8B40: 0x6FB2A690 GAME_WaitJoinScreen (initial state 3)
    CLIENT_StateHandler_InitSession_6FB2BC80,// [4] 0x6FBA8B44: 0x6FB2BC80 GAME_LeaveGameCleanupState
    CLIENT_StateHandler_PrepareNet_6FAEBBF0, // [5] 0x6FBA8B48: 0x6FAEBBF0 NET_InitializeSessionByGameType
    CLIENT_StateHandler_StartNet_6FAEBBB0,   // [6] 0x6FBA8B4C: 0x6FAEBBB0 NET_ShutdownClientAndServer
};

static const char* const gszClientModeNames_6FBA8B50[7] = // 0x6FBA8B50
{
    nullptr,
    nullptr,
    "Game",
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const char* gszEmptyConfigFallback_6FBA8B6C = gszEmptyLiteral_6FB80323; // 0x6FBA8B6C
BOOL               gbExitApplication_6FBA8B70      = 1;                        // 0x6FBA8B70

// Waypoint Act Table (0x6FBA8B74, 5 acts x 11 bytes = 55 bytes)
static D2WaypointActTableStrc gWaypointActTable_6FBA8B74[5] = // 0x6FBA8B74
{
    { false,   1,  38, -1, 0 }, // Act I
    { false,  40,  74, -1, 0 }, // Act II
    { false,  75, 102, -1, 0 }, // Act III
    { false, 103, 107, -1, 0 }, // Act IV
    { false, 109, 136, -1, 0 }, // Act V
};
static int32_t gnWaypointActCount_6FBA8BAC = 5; // 0x6FBA8BAC

// Waypoint UI Row Coordinates Table (0x6FBA8BB0, 9 rows x 24 bytes)
static const D2WaypointCoordTableStrc gWaypointCoordTable_6FBA8BB0[9] = // 0x6FBA8BB0
{
    { 17,  89, 80,  84, 17,  60 },
    { 17, 125, 80, 119, 17,  96 },
    { 17, 161, 80, 154, 17, 132 },
    { 17, 197, 80, 189, 17, 168 },
    { 17, 234, 80, 224, 17, 205 },
    { 17, 270, 80, 259, 17, 241 },
    { 17, 306, 80, 294, 17, 277 },
    { 17, 342, 80, 330, 17, 313 },
    { 17, 378, 80, 364, 17, 349 },
};

// --- BSS / Cross-Module State Globals (0x6FBB1634..0x6FBD36F0) ---
// [aliased] gnMouseY_6FBB1634
// [aliased] gnMouseX_6FBB1638
extern uint8_t              gabServerWorldFlags_6FBB1678[37];     // 0x6FBB1678 (0x6FBB1679 == [1] Cow King / Tristram flag)
// [aliased] gabPanelOpenStates_6FBB2B58
extern BOOL                 gbCutsceneSavedPanelFlag_6FBB2BA0;    // 0x6FBB2BA0
// [aliased] gbWaypointMenuOpen_6FBB2BA8
// [aliased] gdwClientGameFrame_6FBB32B0
// [aliased] gbIsExpansionChar_6FBB330C
extern uint32_t             gdwSkillCooldownEndFrame_6FBB3314;    // 0x6FBB3314
// [aliased] gszPlayerAccountName_6FBB33E0
extern int32_t              gnInitGameSeed_6FBB5378;              // 0x6FBB5378
// [aliased] gbEscMenuOpen_6FBB5490
extern BOOL                 gbPlayingExpansionVideo_6FBC2790;     // 0x6FBC2790
// [aliased] gapClientMonsterHashTable_6FBC9F60
// [aliased] gapServerMonsterHashTable_6FBCAB60
// [aliased] gdwActiveNpcGUID_6FBCB625
// [aliased] gbActiveNpcInteract_6FBCB629
// [aliased] gpClientQuestFlags_6FBCB643
extern int32_t              ganUIBlockingStates_6FBCB65F[7];      // 0x6FBCB65F..0x6FBCB677
extern int32_t              gnMenuStateValue_6FBCB6AC;            // 0x6FBCB6AC

// Smacker Cutscene State Globals (0x6FBCB6CC..0x6FBCB6D0)
uint8_t                     gbCutsceneFromMainMenu_6FBCB6CC = 0;  // 0x6FBCB6CC
static uint8_t              gbCutsceneActive_6FBCB6CD       = 0;  // 0x6FBCB6CD
uint8_t                     gbHighResCutscene_6FBCB6CE      = 0;  // 0x6FBCB6CE
uint8_t                     gbInCutscenePlayback_6FBCB6CF   = 0;  // 0x6FBCB6CF
uint8_t                     gbChainAct5IntroVideo_6FBCB6D0  = 0;  // 0x6FBCB6D0

// Client Ambient Critter / Decoy / Player Slot Array (0x6FBCB6D4..0x6FBCB6F0)
D2UnitStrc*                 gapClientDecoyTargets_6FBCB6D4[8] = {}; // 0x6FBCB6D4

// Environment Weather, Rain, Snow, Lightning & Bubble State (0x6FBCB6F4..0x6FBCB7C0)
static int32_t              ganBubbleCelFrameCounts_6FBCB6F4[4] = {}; // 0x6FBCB6F4
static D2CellFileStrc*      gapBubbleCellFiles_6FBCB704[4]      = {}; // 0x6FBCB704..0x6FBCB710
static uint8_t              gabRainColorDawnDusk_6FBCB714[12]   = {}; // 0x6FBCB714
static int32_t              ganWeatherPhaseDurationRange_6FBCB720[4] = {}; // 0x6FBCB720..0x6FBCB72C
static int32_t              gnLightningTimerFrames_6FBCB730     = 0;  // 0x6FBCB730
static BOOL                 gbLightningDoubleFlash_6FBCB734     = 0;  // 0x6FBCB734
static int32_t              ganWeatherPhaseDurationMin_6FBCB738[4] = {}; // 0x6FBCB738..0x6FBCB744
static uint8_t              gabRainColorDay_6FBCB748[12]        = {}; // 0x6FBCB748
static uint8_t              gabSnowColorNormal_6FBCB754[12]     = {}; // 0x6FBCB754
static int32_t              gnWindAngleRangeSnow_6FBCB760       = 0;  // 0x6FBCB760
static int32_t              gnWindAngleRangeRain_6FBCB764       = 0;  // 0x6FBCB764
static float                gfCurrentRainIntensity_6FBCB768     = 0.0f; // 0x6FBCB768
static uint8_t              gabRainColorNight_6FBCB76C[12]      = {}; // 0x6FBCB76C
static D2CellFileStrc*      gapRainSplashCellFiles_6FBCB778[4]  = {}; // 0x6FBCB778..0x6FBCB784
static int32_t              gnMaxTargetParticles_6FBCB788       = 0;  // 0x6FBCB788
static int32_t              gnCurrentWindAngle_6FBCB78C         = 0;  // 0x6FBCB78C
static int32_t              ganRainSplashFrameCounts_6FBCB790[4]= {}; // 0x6FBCB790
static int32_t              gnWindAngleMinSnow_6FBCB7A0         = 0;  // 0x6FBCB7A0
static int32_t              gnWindAngleMinRain_6FBCB7A4         = 0;  // 0x6FBCB7A4
static int32_t              gnCurrentTargetParticles_6FBCB7A8   = 0;  // 0x6FBCB7A8
static int32_t              gnWindChangeTimer_6FBCB7AC          = 0;  // 0x6FBCB7AC
static int32_t              gnLightningPendingTrigger_6FBCB7B0  = 0;  // 0x6FBCB7B0
static uint8_t              gabSnowColorBright_6FBCB7B4[12]     = {}; // 0x6FBCB7B4
static int32_t              gnTargetWindAngle_6FBCB7C0          = 0;  // 0x6FBCB7C0

// Ambient Light Cosine Fade State (0x6FBCB7C4..0x6FBCB7EC, 11 dwords = 0x2C bytes)
static BOOL                 gbAmbientFadeActive_6FBCB7C4        = 0;  // 0x6FBCB7C4
static int32_t              gnAmbientFadeIntensity_6FBCB7C8     = 0;  // 0x6FBCB7C8
static uint8_t              gnAmbientFadeRed_6FBCB7CC           = 0;  // 0x6FBCB7CC
static uint8_t              gnAmbientFadeGreen_6FBCB7CD         = 0;  // 0x6FBCB7CD
static uint8_t              gnAmbientFadeBlue_6FBCB7CE          = 0;  // 0x6FBCB7CE
static uint8_t              gnAmbientFadePad_6FBCB7CF           = 0;  // 0x6FBCB7CF
static int32_t              gnAmbientFadeInFrames_6FBCB7D0      = 0;  // 0x6FBCB7D0
static int32_t              gnAmbientFadeOutFrames_6FBCB7D4     = 0;  // 0x6FBCB7D4
static int32_t              gnAmbientFadeHoldFrames_6FBCB7D8    = 0;  // 0x6FBCB7D8
static int32_t              gnAmbientFadeTotalFrames_6FBCB7DC   = 0;  // 0x6FBCB7DC
static int32_t              gnAmbientFadeCurrentFrame_6FBCB7E0  = 0;  // 0x6FBCB7E0
static int32_t              gnAmbientFadeCosAngle_6FBCB7E4      = 0;  // 0x6FBCB7E4
static int32_t              gnAmbientFadeLevelId_6FBCB7E8       = 0;  // 0x6FBCB7E8
static int32_t              gbAmbientFadeLocked_6FBCB7EC        = 0;  // 0x6FBCB7EC

// Client Module Path & State Queue (0x6FBCB7F0..0x6FBCB908)
static char                 gszClientModuleFileName_6FBCB7F0[MAX_PATH] = {}; // 0x6FBCB7F0
int32_t                     ganClientStateQueue_6FBCB8F4[5]     = {}; // 0x6FBCB8F4..0x6FBCB904
D2ConfigStrc*        gpD2Config_6FBCB908                 = nullptr; // 0x6FBCB908

// Waypoint UI Menu State (0x6FBCB90C..0x6FBCB961)
static D2WaypointTableStrc  gaWaypointEntries_6FBCB90C[9]       = {}; // 0x6FBCB90C..0x6FBCB938
static uint8_t              gbWaypointClosePressed_6FBCB93C     = 0;  // 0x6FBCB93C
static int32_t              gnWaypointClickedRow_6FBCB93D       = -1; // 0x6FBCB93D
static D2CellFileStrc*      gpWaygateBgCell_6FBCB941            = nullptr; // 0x6FBCB941
static D2CellFileStrc*      gpWaygateTabsCell_6FBCB945          = nullptr; // 0x6FBCB945
static D2CellFileStrc*      gpWaygateIconsCell_6FBCB949         = nullptr; // 0x6FBCB949
static uint32_t             gdwActiveWaygateUnitId_6FBCB94D     = 0;  // 0x6FBCB94D
static D2WaypointDataStrc*  gpPlayerWaypointData_6FBCB951       = nullptr; // 0x6FBCB951
static uint8_t              gbWaypointCancelSent_6FBCB955       = 0;  // 0x6FBCB955
static int32_t              gnSelectedWaypointActTab_6FBCB956   = 0;  // 0x6FBCB956
static int32_t              gnWaypointEntryCount_6FBCB95A       = 0;  // 0x6FBCB95A
static int32_t              gnActiveDestWaypointCount_6FBCB95E  = 0;  // 0x6FBCB95E

extern int32_t              gbIs800x600Mode_6FBCB980;            // 0x6FBCB980
static HINSTANCE            ghD2ClientInstance_6FBCBCC0         = nullptr; // 0x6FBCBCC0
extern int32_t              gnServerWorldStateInit_6FBCBCE0;     // 0x6FBCBCE0
// [aliased] gnUiPanelOffsetX_6FBCBD28
// [aliased] gnUiPanelOffsetY_6FBCBD2C
extern D2CellFileStrc*      gpBuySellBtnCell_6FBCBD4C;           // 0x6FBCBD4C
// [aliased] gbMissingExpansionGameDll_6FBCC010
// [aliased] gbExpansionCutsceneError_6FBCC014
// [aliased] gpClientDrlgAct_6FBCC01C
// [aliased] gpMainViewport_6FBCC028
// [aliased] gnAutoWalkTargetX_6FBCC040
// [aliased] gnAutoWalkTargetY_6FBCC044
// [aliased] gnAutoWalkStateA_6FBCC05C
// [aliased] gnAutoWalkStateB_6FBCC060
// [aliased] gnCameraScrollDeltaX_6FBCC06C
// [aliased] gnCameraScrollDeltaY_6FBCC070
// [aliased] gbCmdFlagR_6FBCC08C
// [aliased] gbCmdFlagP_6FBCC094
// [aliased] gnPendingMouseActionA_6FBCC118
// [aliased] gnPendingMouseActionB_6FBCC11C
// [aliased] gnViewShiftMode_6FBCC1D0
// [aliased] gnViewShiftOffsetX_6FBCC1D4
extern int32_t              gnCutsceneFadeClear_6FBCC34C;        // 0x6FBCC34C
extern uint32_t             gdwLightningFlashAlpha_6FBCC388;     // 0x6FBCC388
// [aliased] gpClientPlayer_6FBCC3D0

// Environment Pools & Weather Control Globals (0x6FBCC3E0..0x6FBCC408)
D2FogDataListStrc*          gpEnvSplashesPool_6FBCC3E0          = nullptr; // 0x6FBCC3E0
D2FogDataListStrc*          gpEnvBubblesPool_6FBCC3E4           = nullptr; // 0x6FBCC3E4
D2FogDataListStrc*          gpEnvParticlesPool_6FBCC3E8         = nullptr; // 0x6FBCC3E8
static BOOL                 gbLightningEnabled_6FBCC3EC         = 0;       // 0x6FBCC3EC
static uint32_t             gdwLastEnvUpdateFrame_6FBCC3F0      = 0;       // 0x6FBCC3F0
int32_t                     gnEnvUnusedSetting_6FBCC3F4         = 0;       // 0x6FBCC3F4
static BOOL                 gbIsSnowing_6FBCC3F8                = 0;       // 0x6FBCC3F8
static BOOL                 gbLargeSnowflakes_6FBCC3FC          = 0;       // 0x6FBCC3FC
static BOOL                 gbForceConstantWeather_6FBCC400     = 0;       // 0x6FBCC400
static BOOL                 gbWeatherDisabledCheat_6FBCC404     = 0;       // 0x6FBCC404
static int32_t              gnWeatherCyclePhase_6FBCC408        = 0;       // 0x6FBCC408 (0=dry, 1=ramp up, 2=peak, 3=ramp down)

// Quest / Exit / Startup Session Globals (0x6FBCC414..0x6FBCC438)
static uint32_t             gdwTriggeredQuestEventsMask_6FBCC414= 0;       // 0x6FBCC414
BOOL                        gbTristramPortalOpened_6FBCC418     = 0;       // 0x6FBCC418
BOOL                        gbDiabloSealTintActive_6FBCC41C     = 0;       // 0x6FBCC41C
int32_t                     gnExitCountdownEndSyncTime_6FBCC420 = 0;       // 0x6FBCC420
BOOL                        gbNihlathakTauntReady_6FBCC424      = 0;       // 0x6FBCC424
static HINSTANCE            ghClientModule_6FBCC428             = nullptr; // 0x6FBCC428
static int32_t              gnNextLauncherModuleId_6FBCC42C     = 0;       // 0x6FBCC42C
static BOOL                 gbCmdHelpShown_6FBCC434             = 0;       // 0x6FBCC434
int32_t                     gnClientStateQueueCount_6FBCC438    = 0;       // 0x6FBCC438

// [aliased] gnWorldViewportHeight_6FBCD524
// [aliased] gnWorldViewportWidth_6FBCD528

static BOOL                 gbPrevLevelHadMud_6FBD36A0          = 0;       // 0x6FBD36A0
static BOOL                 gbPrevLevelHadRain_6FBD36A4         = 0;       // 0x6FBD36A4
static int32_t              gnWeatherPhaseTotalTicks_6FBD36A8   = 0;       // 0x6FBD36A8
static int32_t              gnWeatherPhaseRemTicks_6FBD36AC     = 0;       // 0x6FBD36AC
static int32_t              gnSnowDriftTickCounter_6FBD36B0     = 0;       // 0x6FBD36B0
static BOOL                 gbEnvWindInitialized_6FBD36B4       = 0;       // 0x6FBD36B4
static BOOL                 gbNihlathakTauntPlayed_6FBD36B8     = 0;       // 0x6FBD36B8

// Helper matching D2Client's assertion/halt pattern:
// push LINE; call FOG_10265; push eax; push ""; call FOG_DisplayHalt; add esp, 0Ch; push -1; call exit
static __declspec(noreturn) void ENGINE_FatalHalt(int32_t nLine)
{
    const char* szFile = Fog_10265();
    FOG_DisplayHalt(gszEmptyLiteral_6FB80323, szFile, nLine);
    exit(-1);
}


// Canonical cross-TU global aliases by 1.12a VA
extern D2UnitStrc*    gpPlayerUnit;                          // 0x6FBCC3D0
extern D2DrlgActStrc* gpClientAct;                           // 0x6FBCC01C
extern void*          gpClientViewport;                      // 0x6FBCC028
extern int32_t        gaPanelStates[38];                     // 0x6FBB2B58
extern D2UnitStrc*    gapClientSideUnitHashTables[6][128];   // 0x6FBC9D60
extern D2UnitStrc*    gapServerSideUnitHashTables[6][128];   // 0x6FBCA960
extern int32_t        gnScreenWidth;                         // 0x6FB8C6E0
extern int32_t        gnScreenHeight;                        // 0x6FB8C6E4
extern int32_t        gnMouseX;                              // 0x6FBB1638
extern int32_t        gnMouseY;                              // 0x6FBB1634
extern uint32_t       gdwClientGameTick;                     // 0x6FBB32B0
extern uint32_t       gdwNextClientUnitGUID;                 // 0x6FBA6A80
extern int32_t        gbIsExpansion;                         // 0x6FBB330C
extern int32_t        gnOpenPanelMode;                       // 0x6FBCC1D0
extern int32_t        gnViewportShiftOffsetX;                // 0x6FBCC1D4
extern int32_t        gnViewportHeight;                      // 0x6FBCD524
extern int32_t        gnViewportWidth;                       // 0x6FBCD528
extern int32_t        g_nPanelOffsetX;                       // 0x6FBCBD28
extern int32_t        g_nPanelOffsetY;                       // 0x6FBCBD2C
extern int32_t        gnSelectedTargetA;                     // 0x6FBCC118
extern int32_t        gnSelectedTargetB;                     // 0x6FBCC11C
extern D2BitBufferStrc* g_pPlayerQuestRecord;                // 0x6FBCB643
extern int32_t        g_bInteractingWithNpc;                 // 0x6FBCB629
extern int32_t        g_nInteractingNpcGUID;                 // 0x6FBCB625
extern int32_t        g_bLeftMouseHeld;                      // 0x6FBCC040
extern int32_t        g_bRightMouseHeld;                     // 0x6FBCC044
extern int32_t        g_bLeftMouseRepeating;                 // 0x6FBCC05C
extern int32_t        g_bRightMouseRepeating;                // 0x6FBCC060
extern int32_t        g_nCameraDeltaX;                       // 0x6FBCC06C
extern int32_t        g_nCameraDeltaY;                       // 0x6FBCC070
extern int32_t        g_bPlayExpansionEpilogueMovie;         // 0x6FBCC010
extern int32_t        g_bShowExpansionEndGameScreen;         // 0x6FBCC014
extern char           g_szConfigCharName[16];                // 0x6FBB33E0
extern int32_t        gbRecordingInputActive;                // 0x6FBCC08C
extern int32_t        gbRecordPlaybackActive;                // 0x6FBCC094

#define gpClientPlayer_6FBCC3D0          gpPlayerUnit
#define gpClientDrlgAct_6FBCC01C         gpClientAct
#define gpMainViewport_6FBCC028          (*reinterpret_cast<D2ViewportStrc**>(&gpClientViewport))
#define gabPanelOpenStates_6FBB2B58      gaPanelStates
#define gapClientMonsterHashTable_6FBC9F60 (gapClientSideUnitHashTables[1])
#define gapServerMonsterHashTable_6FBCAB60 (gapServerSideUnitHashTables[1])
#define gnScreenWidth_6FB8C6E0           gnScreenWidth
#define gnScreenHeight_6FB8C6E4          gnScreenHeight
#define gnMouseX_6FBB1638                gnMouseX
#define gnMouseY_6FBB1634                gnMouseY
#define gdwClientGameFrame_6FBB32B0      gdwClientGameTick
#define gdwNextClientCritterGUID_6FBA6A80 gdwNextClientUnitGUID
#define gbIsExpansionChar_6FBB330C       gbIsExpansion
#define gnViewShiftMode_6FBCC1D0         gnOpenPanelMode
#define gnViewShiftOffsetX_6FBCC1D4      gnViewportShiftOffsetX
#define gnWorldViewportHeight_6FBCD524   gnViewportHeight
#define gnWorldViewportWidth_6FBCD528    gnViewportWidth
#define gnUiPanelOffsetX_6FBCBD28        g_nPanelOffsetX
#define gnUiPanelOffsetY_6FBCBD2C        g_nPanelOffsetY
#define gnPendingMouseActionA_6FBCC118   gnSelectedTargetA
#define gnPendingMouseActionB_6FBCC11C   gnSelectedTargetB
#define gpClientQuestFlags_6FBCB643      g_pPlayerQuestRecord
#define gbActiveNpcInteract_6FBCB629     g_bInteractingWithNpc
#define gdwActiveNpcGUID_6FBCB625        (*reinterpret_cast<uint32_t*>(&g_nInteractingNpcGUID))
#define gnAutoWalkTargetX_6FBCC040       g_bLeftMouseHeld
#define gnAutoWalkTargetY_6FBCC044       g_bRightMouseHeld
#define gnAutoWalkStateA_6FBCC05C        g_bLeftMouseRepeating
#define gnAutoWalkStateB_6FBCC060        g_bRightMouseRepeating
#define gnCameraScrollDeltaX_6FBCC06C    g_nCameraDeltaX
#define gnCameraScrollDeltaY_6FBCC070    g_nCameraDeltaY
#define gbMissingExpansionGameDll_6FBCC010 g_bPlayExpansionEpilogueMovie
#define gbExpansionCutsceneError_6FBCC014  g_bShowExpansionEndGameScreen
#define gszPlayerAccountName_6FBB33E0    g_szConfigCharName
#define gbCmdFlagR_6FBCC08C              gbRecordingInputActive
#define gbCmdFlagP_6FBCC094              gbRecordPlaybackActive
#define gbEscMenuOpen_6FBB5490           (gaPanelStates[9])
#define gbWaypointMenuOpen_6FBB2BA8      (gaPanelStates[16])


// CORE_WinMain.cpp-owned 1.12a globals (.data / .bss)
uint8_t  gabServerWorldFlags_6FBB1678[37] = {};
int32_t  gnServerWorldStateInit_6FBCBCE0  = 0;
int32_t  ganUIBlockingStates_6FBCB65F[7]  = {};
BOOL     gbCutsceneSavedPanelFlag_6FBB2BA0= FALSE;
int32_t  gbIs800x600Mode_6FBCB980         = 1;
BOOL     gbPlayingExpansionVideo_6FBC2790 = FALSE;
uint32_t gdwLightningFlashAlpha_6FBCC388  = 0;
uint32_t gdwSkillCooldownEndFrame_6FBB3314= 0;
int32_t  gnCutsceneFadeClear_6FBCC34C     = 0;
int32_t  gnInitGameSeed_6FBB5378          = 0;
int32_t  gnMenuStateValue_6FBCB6AC        = 0;
int32_t  gnPanelDescriptorCount_6FB8F368  = 38;
int32_t  gnScreenPitch_6FB8C6E8           = 800;
D2CellFileStrc* gpBuySellBtnCell_6FBCBD4C = nullptr;
static D2PanelDescriptorStrc s_tDefaultPanelDesc = {};
D2PanelDescriptorStrc* gpPanelDescriptors_6FB8F2D0[38] = {
    &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc,
    &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc,
    &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc,
    &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc,
    &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc,
    &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc,
    &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc,
    &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc,
    &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc, &s_tDefaultPanelDesc,
    &s_tDefaultPanelDesc, &s_tDefaultPanelDesc
};

// =============================================================================
// SECTION 1: Engine.h & DataTable / Storm List / RTTI Helpers
// VA Range: 0x6FAB1000 .. 0x6FAB1D9F (51 functions)
// =============================================================================

// 1.12a: 0x6FAB1000 (__cdecl: [esp+4] = pMem)
void __cdecl ENGINE_MemFreeDelete(void* pMem)
{
    if (pMem != nullptr)
    {
        SMemFree(pMem, gszDeleteTag_6FB80328, -1, 0);
    }
}

// 1.12a: 0x6FAB1020 (custom: ecx = pSeed, eax = nMax -> __fastcall)
int32_t __fastcall ENGINE_RollRandomNumber(D2SeedStrc* pSeed, int32_t nMax)
{
    if (nMax <= 0)
    {
        return 0;
    }

    const uint64_t uProduct = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
    pSeed->nLowSeed  = static_cast<uint32_t>(uProduct);
    pSeed->nHighSeed = static_cast<uint32_t>(uProduct >> 32);

    const uint32_t uMax = static_cast<uint32_t>(nMax);
    if ((uMax & (uMax - 1u)) == 0)
    {
        return static_cast<int32_t>(pSeed->nLowSeed & (uMax - 1u));
    }
    return static_cast<int32_t>(pSeed->nLowSeed % uMax);
}

// 1.12a: 0x6FAB1070 (custom: ecx = pSeed, esi = nMax -> __fastcall)
int32_t __fastcall ENGINE_RollRandomNumberMax(D2SeedStrc* pSeed, int32_t nMax)
{
    if (nMax <= 0)
    {
        return 0;
    }

    const uint64_t uProduct = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
    pSeed->nLowSeed  = static_cast<uint32_t>(uProduct);
    pSeed->nHighSeed = static_cast<uint32_t>(uProduct >> 32);

    const uint32_t uMax = static_cast<uint32_t>(nMax);
    if ((uMax & (uMax - 1u)) == 0)
    {
        return static_cast<int32_t>(pSeed->nLowSeed & (uMax - 1u));
    }
    return static_cast<int32_t>(pSeed->nLowSeed % uMax);
}

// 1.12a: 0x6FAB10C0 (custom: eax = pUnit, ecx = dwFlag, edx = bSet -> __fastcall)
void __fastcall ENGINE_SetUnitFlagState(D2UnitStrc* pUnit, uint32_t dwFlag, BOOL bSet)
{
    if (pUnit == nullptr)
    {
        return;
    }

    if (bSet)
    {
        pUnit->dwFlags |= dwFlag;
    }
    else
    {
        pUnit->dwFlags &= ~dwFlag;
    }
}

// 1.12a: 0x6FAB1160 (custom: ecx = pElements, eax = nCount, ebx = pfnCtor, [esp+4] = nElemStride -> __fastcall)
void __fastcall ENGINE_ConstructElementArray(void* pElements, int32_t nCount, void (__thiscall* pfnDefaultCtor)(void*), size_t nElementStride)
{
    if (nCount <= 0)
    {
        return;
    }

    uint8_t* pCursor = static_cast<uint8_t*>(pElements);
    for (int32_t i = 0; i < nCount; ++i)
    {
        pfnDefaultCtor(pCursor);
        pCursor += nElementStride;
    }
}

static void __fastcall ENGINE_CallUnicodeDefaultCtor(void* pElement)
{
    Unicode* pUni = static_cast<Unicode*>(pElement);
    *pUni = Unicode(0);
}

// 1.12a: 0x6FAB10F0 (__stdcall: [esp+4] = pGrid; returns pGrid)
D2ChatLineGridStrc* __stdcall ENGINE_InitChatLineGrid(D2ChatLineGridStrc* pGrid)
{
    for (int32_t i = 0; i < 6; ++i)
    {
        Unicode* pRowChars = reinterpret_cast<Unicode*>(reinterpret_cast<uint8_t*>(&pGrid->aRows[i]));
        for (int32_t j = 0; j < 300; ++j)
        {
            ENGINE_CallUnicodeDefaultCtor(&pRowChars[j]);
        }
    }
    return pGrid;
}

// 1.12a: 0x6FAB1130 (__fastcall: ecx = pRow; returns pRow)
D2ChatLineRowStrc* __fastcall ENGINE_ConstructChatLineRow(D2ChatLineRowStrc* pRow)
{
    for (int32_t i = 0; i < 300; ++i)
    {
        ENGINE_CallUnicodeDefaultCtor(&pRow->wszText[i]);
    }
    return pRow;
}

// 1.12a: 0x6FAB1180 (__stdcall: no-op stub)
void __stdcall ENGINE_Noop_6FAB1180(void)
{
}

// Internal helper matching Storm TSLink::Unlink instruction sequence in 0x6FAB1190..0x6FAB138E
static inline void ENGINE_UnlinkSingleStormLink(D2StormLinkStrc* pLink)
{
    D2StormLinkStrc* const pNext = pLink->pNext;
    if (pNext == nullptr)
    {
        return;
    }

    const intptr_t nPrevLink = pLink->nPrevLink;
    D2StormLinkStrc** ppPrevNextField = nullptr;
    if (nPrevLink < 0)
    {
        ppPrevNextField = reinterpret_cast<D2StormLinkStrc**>(~static_cast<uintptr_t>(nPrevLink));
    }
    else
    {
        const uintptr_t uPredAddr = reinterpret_cast<uintptr_t>(pLink) - static_cast<uintptr_t>(pNext->nPrevLink) + static_cast<uintptr_t>(nPrevLink);
        ppPrevNextField = reinterpret_cast<D2StormLinkStrc**>(uPredAddr);
    }

    *ppPrevNextField = pNext;
    pLink->pNext->nPrevLink = pLink->nPrevLink;
    pLink->pNext = nullptr;
    pLink->nPrevLink = 0;
}

// 1.12a: 0x6FAB1350 (custom: ebx = pList -> __fastcall)
void __fastcall ENGINE_UnlinkAllStormListNodes(D2StormListStrc* pList)
{
    while (reinterpret_cast<intptr_t>(pList->tTerminator.pNext) > 0)
    {
        D2StormLinkStrc* pHead = reinterpret_cast<D2StormLinkStrc*>(pList->tTerminator.pNext);
        if (pHead->pNext != nullptr)
        {
            ENGINE_UnlinkSingleStormLink(pHead);
        }
    }
}

// 1.12a: 0x6FAB1190 (__stdcall: [esp+4] = pList; Storm TSExplicitList destructor)
void __stdcall ENGINE_DestroyStormList(D2StormListStrc* pList)
{
    ENGINE_UnlinkAllStormListNodes(pList);
    ENGINE_UnlinkSingleStormLink(&pList->tTerminator);
}

// 1.12a: 0x6FAB1210 (custom: eax = pList, ecx = pRefLink, [esp+4] = pNodeLink, [esp+8] = nInsertMode -> __fastcall)
void __fastcall ENGINE_LinkStormNode(D2StormListStrc* pList, D2StormLinkStrc* pRefLink, D2StormLinkStrc* pNodeLink, int32_t nInsertMode)
{
    D2StormLinkStrc* const pLink = (pNodeLink != nullptr) ? pNodeLink : &pList->tTerminator;
    ENGINE_UnlinkSingleStormLink(pLink);

    D2StormLinkStrc* const pTarget = (pRefLink != nullptr) ? pRefLink : &pList->tTerminator;
    if (nInsertMode == 1)
    {
        pLink->pNext = pTarget;
        pLink->nPrevLink = pTarget->nPrevLink;
        const intptr_t nOldPrev = pTarget->nPrevLink;
        if (nOldPrev < 0)
        {
            *reinterpret_cast<D2StormLinkStrc**>(~static_cast<uintptr_t>(nOldPrev)) = pLink;
            pTarget->nPrevLink = reinterpret_cast<intptr_t>(pNodeLink);
        }
        else
        {
            intptr_t nOffset = pList->nLinkOffset;
            if (nOffset < 0)
            {
                nOffset = static_cast<intptr_t>(reinterpret_cast<uintptr_t>(pTarget) - static_cast<uintptr_t>(pTarget->pNext->nPrevLink));
            }
            *reinterpret_cast<D2StormLinkStrc**>(static_cast<uintptr_t>(nOldPrev + nOffset)) = pLink;
            pTarget->nPrevLink = reinterpret_cast<intptr_t>(pNodeLink);
        }
    }
    else
    {
        D2StormLinkStrc* const pAfter = pTarget->pNext;
        pLink->pNext = pAfter;
        pLink->nPrevLink = pAfter->nPrevLink;
        pAfter->nPrevLink = reinterpret_cast<intptr_t>(pNodeLink);
        pTarget->pNext = pLink;
    }
}

// 1.12a: 0x6FAB12B0 (__stdcall: [esp+4] = pLink; Storm TSLinkedNode destructor)
void __stdcall ENGINE_DestroyStormLinkNode(D2StormLinkStrc* pLink)
{
    ENGINE_UnlinkSingleStormLink(pLink);
    ENGINE_UnlinkSingleStormLink(pLink);
}

// 1.12a: 0x6FAB1390 (custom: eax = nMonsterId -> __fastcall)
D2MonStatsTxt* __fastcall ENGINE_GetMonStatsTxtRecord(int32_t nMonsterId)
{
    if (nMonsterId >= 0 && nMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
    {
        return &sgptDataTables->pMonStatsTxt[nMonsterId];
    }
    return nullptr;
}

// 1.12a: 0x6FAB13C0 (__stdcall)
int32_t __stdcall ENGINE_GetMonStatsTxtRecordCount(void)
{
    return sgptDataTables->nMonStatsTxtRecordCount;
}

// 1.12a: 0x6FAB13D0 (custom: eax = nMonsterId -> __fastcall)
D2MonStats2Txt* __fastcall ENGINE_GetMonStats2TxtRecordFromMonsterId(int32_t nMonsterId)
{
    if (nMonsterId >= 0 && nMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
    {
        const int32_t nMonStatsEx = sgptDataTables->pMonStatsTxt[nMonsterId].wMonStatsEx;
        if (nMonStatsEx >= 0 && nMonStatsEx < sgptDataTables->nMonStats2TxtRecordCount)
        {
            return &sgptDataTables->pMonStats2Txt[nMonStatsEx];
        }
    }
    return nullptr;
}

// 1.12a: 0x6FAB1420 (__thiscall / __fastcall: ecx = pObj, [esp+4] = uFlags)
void* __fastcall ENGINE_ScalarDeletingDtor_WidgetA(void* pObj, int32_t nUnusedEdx, uint32_t uFlags)
{
    (void)nUnusedEdx;
    UI_DestroyWidgetA_6FB6CC90(pObj);
    if ((uFlags & 1u) != 0 && pObj != nullptr)
    {
        SMemFree(pObj, gszDeleteTag_6FB80328, -1, 0);
    }
    return pObj;
}

// 1.12a: 0x6FAB1450 (__thiscall / __fastcall: ecx = pObj, [esp+4] = uFlags)
void* __fastcall ENGINE_ScalarDeletingDtor_WidgetB(void* pObj, int32_t nUnusedEdx, uint32_t uFlags)
{
    (void)nUnusedEdx;
    UI_DestroyWidgetB_6FB6C8A0(pObj);
    if ((uFlags & 1u) != 0 && pObj != nullptr)
    {
        SMemFree(pObj, gszDeleteTag_6FB80328, -1, 0);
    }
    return pObj;
}

// 1.12a: 0x6FAB1480 (custom: eax = nMissileId -> __fastcall)
D2MissilesTxt* __fastcall ENGINE_GetMissilesTxtRecord(int32_t nMissileId)
{
    if (nMissileId >= 0 && nMissileId < sgptDataTables->nMissilesTxtRecordCount)
    {
        return &sgptDataTables->pMissilesTxt[nMissileId];
    }
    return nullptr;
}

// 1.12a: 0x6FAB14B0 (custom: ecx = pSeed, edi = nMin, esi = nRange -> __fastcall)
int32_t __fastcall ENGINE_RollRandomRange(D2SeedStrc* pSeed, int32_t nMin, int32_t nRange)
{
    if (nRange <= 0)
    {
        return nMin;
    }

    const uint64_t uProduct = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
    pSeed->nLowSeed  = static_cast<uint32_t>(uProduct);
    pSeed->nHighSeed = static_cast<uint32_t>(uProduct >> 32);

    const uint32_t uRange = static_cast<uint32_t>(nRange);
    uint32_t uRoll = 0;
    if ((uRange & (uRange - 1u)) == 0)
    {
        uRoll = pSeed->nLowSeed & (uRange - 1u);
    }
    else
    {
        uRoll = pSeed->nLowSeed % uRange;
    }
    return nMin + static_cast<int32_t>(uRoll);
}

// 1.12a: 0x6FAB1500 (custom: eax = nSkillId -> __fastcall)
D2SkillsTxt* __fastcall ENGINE_GetSkillsTxtRecord(int32_t nSkillId)
{
    if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
    {
        return &sgptDataTables->pSkillsTxt[nSkillId];
    }
    return nullptr;
}

// 1.12a: 0x6FAB1530 (custom: eax = nSkillId -> __fastcall)
int32_t __fastcall ENGINE_GetSkillParam1(int32_t nSkillId)
{
    D2SkillsTxt* const pSkillsTxt = ENGINE_GetSkillsTxtRecord(nSkillId);
    return (pSkillsTxt != nullptr) ? pSkillsTxt->dwParam[0] : 0;
}

// 1.12a: 0x6FAB1560 (custom: eax = nSkillId -> __fastcall)
int32_t __fastcall ENGINE_GetSkillParam2(int32_t nSkillId)
{
    D2SkillsTxt* const pSkillsTxt = ENGINE_GetSkillsTxtRecord(nSkillId);
    return (pSkillsTxt != nullptr) ? pSkillsTxt->dwParam[1] : 0;
}

// 1.12a: 0x6FAB1590 (__fastcall: ecx = pUnit)
int32_t __fastcall ENGINE_GetUnitX(D2UnitStrc* pUnit)
{
    const uint32_t dwType = pUnit->dwUnitType;
    if (dwType == UNIT_OBJECT || (dwType > UNIT_MISSILE && dwType <= UNIT_TILE))
    {
        return pUnit->pStaticPath->nXPos;
    }
    return (pUnit->pDynamicPath != nullptr) ? PATH_GetXPosition(pUnit->pDynamicPath) : 0;
}

// 1.12a: 0x6FAB15C0 (__fastcall: ecx = pUnit)
int32_t __fastcall ENGINE_GetUnitY(D2UnitStrc* pUnit)
{
    const uint32_t dwType = pUnit->dwUnitType;
    if (dwType == UNIT_OBJECT || (dwType > UNIT_MISSILE && dwType <= UNIT_TILE))
    {
        return pUnit->pStaticPath->nYPos;
    }
    return (pUnit->pDynamicPath != nullptr) ? PATH_GetYPosition(pUnit->pDynamicPath) : 0;
}

// 1.12a: 0x6FAB15F0 (__thiscall / __fastcall: ecx = pObj, [esp+4] = uFlags)
void* __fastcall ENGINE_ScalarDeletingDtor_WidgetC(void* pObj, int32_t nUnusedEdx, uint32_t uFlags)
{
    (void)nUnusedEdx;
    UI_DestroyWidgetC_6FB68440(pObj);
    if ((uFlags & 1u) != 0 && pObj != nullptr)
    {
        SMemFree(pObj, gszDeleteTag_6FB80328, -1, 0);
    }
    return pObj;
}

// 1.12a: 0x6FAB1620 (__thiscall / __fastcall: ecx = pObj, [esp+4] = uFlags)
void* __fastcall ENGINE_ScalarDeletingDtor_WidgetD(void* pObj, int32_t nUnusedEdx, uint32_t uFlags)
{
    (void)nUnusedEdx;
    UI_DestroyWidgetD_6FB682F0(pObj);
    if ((uFlags & 1u) != 0 && pObj != nullptr)
    {
        SMemFree(pObj, gszDeleteTag_6FB80328, -1, 0);
    }
    return pObj;
}

// 1.12a: 0x6FAB1650 (custom: eax = pUnit, cl = nComponent -> __fastcall)
uint8_t __fastcall ENGINE_GetMonsterComponent(D2UnitStrc* pUnit, uint8_t nComponent)
{
    if (pUnit != nullptr && pUnit->dwUnitType == UNIT_MONSTER && nComponent < 16 && pUnit->pMonsterData != nullptr)
    {
        return pUnit->pMonsterData->nComponent[nComponent];
    }
    return 0;
}

// 1.12a: 0x6FAB1670 (custom: eax = nMonsterId, edx = nFlagBit -> __fastcall)
uint32_t __fastcall ENGINE_CheckMonStats2Flag(int32_t nMonsterId, int32_t nFlagBit)
{
    D2MonStats2Txt* const pMonStats2Txt = ENGINE_GetMonStats2TxtRecordFromMonsterId(nMonsterId);
    if (pMonStats2Txt == nullptr)
    {
        return 0;
    }
    return static_cast<uint32_t>(pMonStats2Txt->nFlags[nFlagBit >> 3]) & gdwBitMasks[nFlagBit & 7];
}

// 1.12a: 0x6FAB16E0 (custom: eax = nSetItemId -> __fastcall)
D2SetItemsTxt* __fastcall ENGINE_GetSetItemsTxtRecord(int32_t nSetItemId)
{
    if (nSetItemId >= 0 && nSetItemId < sgptDataTables->nSetItemsTxtRecordCount)
    {
        return &sgptDataTables->pSetItemsTxt[nSetItemId];
    }
    return nullptr;
}

// 1.12a: 0x6FAB1710 (custom: eax = nUniqueItemId -> __fastcall)
D2UniqueItemsTxt* __fastcall ENGINE_GetUniqueItemsTxtRecord(int32_t nUniqueItemId)
{
    if (nUniqueItemId >= 0 && nUniqueItemId < sgptDataTables->nUniqueItemsTxtRecordCount)
    {
        return &sgptDataTables->pUniqueItemsTxt[nUniqueItemId];
    }
    return nullptr;
}

// 1.12a: 0x6FAB1740 (custom: esi = pUnit -> __fastcall)
int32_t __fastcall ENGINE_GetUnitEffectiveType(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr)
    {
        return UNIT_TYPES_COUNT; // 6
    }

    if (((pUnit->dwFlagEx >> 3) & 1u) == 0)
    {
        return static_cast<int32_t>(pUnit->dwUnitType);
    }

    int32_t nType  = static_cast<int32_t>(pUnit->dwUnitType);
    int32_t nClass = static_cast<int32_t>(pUnit->dwClassId);
    int32_t nMode  = UNITS_GetAnimOrSeqMode(pUnit);
    D2COMMON_11013_ConvertMode(pUnit, &nType, &nClass, &nMode, gszEngineH_6FB83A20, 0xF2);
    return nType;
}

// 1.12a: 0x6FAB17B0 (custom: esi = pUnit -> __fastcall)
int32_t __fastcall ENGINE_GetUnitEffectiveAnimMode(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr || ((pUnit->dwFlagEx >> 3) & 1u) == 0)
    {
        return UNITS_GetAnimOrSeqMode(pUnit);
    }

    int32_t nType  = static_cast<int32_t>(pUnit->dwUnitType);
    int32_t nClass = static_cast<int32_t>(pUnit->dwClassId);
    int32_t nMode  = UNITS_GetAnimOrSeqMode(pUnit);
    D2COMMON_11013_ConvertMode(pUnit, &nType, &nClass, &nMode, gszEngineH_6FB83A20, 0x104);
    return nMode;
}

// 1.12a: 0x6FAB1810 (custom: esi = pUnit -> __fastcall)
int32_t __fastcall ENGINE_GetUnitEffectiveClassId(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr)
    {
        return -1;
    }

    if (((pUnit->dwFlagEx >> 3) & 1u) == 0)
    {
        return static_cast<int32_t>(pUnit->dwClassId);
    }

    int32_t nType  = static_cast<int32_t>(pUnit->dwUnitType);
    int32_t nClass = static_cast<int32_t>(pUnit->dwClassId);
    int32_t nMode  = UNITS_GetAnimOrSeqMode(pUnit);
    D2COMMON_11013_ConvertMode(pUnit, &nType, &nClass, &nMode, gszEngineH_6FB83A20, 0x116);
    return nClass;
}

// 1.12a: 0x6FAB1880 (custom: edi = pUnit, esi = pbOutMorphed -> __fastcall)
D2InventoryStrc* __fastcall ENGINE_GetUnitEffectiveInventory(D2UnitStrc* pUnit, BOOL* pbOutMorphed)
{
    if (pUnit != nullptr && ((pUnit->dwFlagEx >> 3) & 1u) != 0)
    {
        if (STATES_CheckStateMaskOnUnit(pUnit, 0x25))
        {
            D2UnitStrc* const pSourceUnit = UNIT_GetMorphedSourceUnit(pUnit);
            if (pSourceUnit != nullptr)
            {
                if (pbOutMorphed != nullptr)
                {
                    *pbOutMorphed = TRUE;
                }
                return pSourceUnit->pInventory;
            }
        }
    }

    if (pbOutMorphed != nullptr)
    {
        *pbOutMorphed = FALSE;
    }
    return pUnit->pInventory;
}

// 1.12a: 0x6FAB18D0 (custom: edx = nValue, eax = nMul, ecx = nDiv -> __fastcall)
int32_t __fastcall ENGINE_SafeMulDiv(int32_t nValue, int32_t nMul, int32_t nDiv)
{
    if (nDiv == 0)
    {
        return 0;
    }

    if (nValue > 0x100000)
    {
        if (nDiv <= (nValue >> 4))
        {
            return (nValue / nDiv) * nMul;
        }
        return static_cast<int32_t>((static_cast<int64_t>(nValue) * static_cast<int64_t>(nMul)) / static_cast<int64_t>(nDiv));
    }

    if (nMul > 0x10000)
    {
        if (nDiv <= (nMul >> 4))
        {
            return (nMul / nDiv) * nValue;
        }
        return static_cast<int32_t>((static_cast<int64_t>(nValue) * static_cast<int64_t>(nMul)) / static_cast<int64_t>(nDiv));
    }

    return (nValue * nMul) / nDiv;
}

// 1.12a: 0x6FAB1940 (custom: eax = nClassId -> __fastcall)
D2CharStatsTxt* __fastcall ENGINE_GetCharStatsTxtRecord(int32_t nClassId)
{
    if (nClassId >= 0 && nClassId < sgptDataTables->nCharStatsTxtRecordCount)
    {
        return &sgptDataTables->pCharStatsTxt[nClassId];
    }
    return nullptr;
}

// 1.12a: 0x6FAB1970 (custom: eax = nSetId -> __fastcall)
D2SetsTxt* __fastcall ENGINE_GetSetsTxtRecord(int32_t nSetId)
{
    if (nSetId >= 0 && nSetId < sgptDataTables->nSetsTxtRecordCount)
    {
        return &sgptDataTables->pSetsTxt[nSetId];
    }
    return nullptr;
}

// 1.12a: 0x6FAB19A0 (custom: eax = pUnit -> __fastcall)
int32_t __fastcall ENGINE_GetMonsterBaseId(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return -1;
    }

    const int32_t nClassId = static_cast<int32_t>(pUnit->dwClassId);
    D2MonStatsTxt* const pMonStatsTxt = ENGINE_GetMonStatsTxtRecord(nClassId);
    if (pMonStatsTxt != nullptr)
    {
        return pMonStatsTxt->nBaseId;
    }
    return nClassId;
}

// 1.12a: 0x6FAB19E0 (custom: eax = nMonsterId -> __fastcall)
int32_t __fastcall ENGINE_ValidateMonsterId(int32_t nMonsterId)
{
    if (nMonsterId >= 0 && nMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
    {
        return nMonsterId;
    }
    return -1;
}

// 1.12a: 0x6FAB1A00 (custom: eax = pRoom, ebx = nSubtileX, edi = nSubtileY -> __fastcall)
D2ActiveRoomStrc* __fastcall ENGINE_FindRoomAtSubtileCoords(D2ActiveRoomStrc* pRoom, int32_t nSubtileX, int32_t nSubtileY)
{
    if (pRoom == nullptr)
    {
        return nullptr;
    }

    if (nSubtileX >= pRoom->tCoords.nSubtileX &&
        nSubtileX < pRoom->tCoords.nSubtileX + pRoom->tCoords.nSubtileWidth &&
        nSubtileY >= pRoom->tCoords.nSubtileY &&
        nSubtileY < pRoom->tCoords.nSubtileY + pRoom->tCoords.nSubtileHeight)
    {
        return pRoom;
    }

    D2ActiveRoomStrc** ppAdjacentRooms = nullptr;
    int32_t nNumAdjacentRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppAdjacentRooms, &nNumAdjacentRooms);

    for (int32_t i = 0; i < nNumAdjacentRooms; ++i)
    {
        D2ActiveRoomStrc* const pAdj = ppAdjacentRooms[i];
        if (pAdj != nullptr &&
            nSubtileX >= pAdj->tCoords.nSubtileX &&
            nSubtileX < pAdj->tCoords.nSubtileX + pAdj->tCoords.nSubtileWidth &&
            nSubtileY >= pAdj->tCoords.nSubtileY &&
            nSubtileY < pAdj->tCoords.nSubtileY + pAdj->tCoords.nSubtileHeight)
        {
            return pAdj;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FAB1AA0 (custom: eax = nSkillId -> __fastcall)
int32_t __fastcall ENGINE_GetSkillMaxLevel(int32_t nSkillId)
{
    if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
    {
        const int32_t nMaxLvl = sgptDataTables->pSkillsTxt[nSkillId].wMaxLvl;
        if (nMaxLvl > 0)
        {
            return nMaxLvl;
        }
    }
    return 20;
}

// 1.12a: 0x6FAB1AE0 (custom: eax = nStateId -> __fastcall)
D2StatesTxt* __fastcall ENGINE_GetStatesTxtRecord(int32_t nStateId)
{
    if (nStateId >= 0 && nStateId < sgptDataTables->nStatesTxtRecordCount)
    {
        return &sgptDataTables->pStatesTxt[nStateId];
    }
    return nullptr;
}

// 1.12a: 0x6FAB1B10 (__thiscall / __fastcall: ecx = pObj, [esp+4] = uFlags)
void* __fastcall ENGINE_ScalarDeletingDtor_WidgetE(void* pObj, int32_t nUnusedEdx, uint32_t uFlags)
{
    (void)nUnusedEdx;
    UI_DestroyWidgetE_6FB4B740(pObj);
    if ((uFlags & 1u) != 0 && pObj != nullptr)
    {
        SMemFree(pObj, gszDeleteTag_6FB80328, -1, 0);
    }
    return pObj;
}

// 1.12a: 0x6FAB1B40 (__fastcall: ecx = pBuf; constructs 6 pages x 1024 Unicode chars)
D2QuestSpeechBufferStrc* __fastcall ENGINE_InitQuestSpeechBuffer(D2QuestSpeechBufferStrc* pBuf)
{
    for (int32_t nPage = 0; nPage < 6; ++nPage)
    {
        for (int32_t i = 0; i < 1024; ++i)
        {
            ENGINE_CallUnicodeDefaultCtor(&pBuf->aPages[nPage][i]);
        }
    }
    return pBuf;
}

// 1.12a: 0x6FAB1C10 (__thiscall / __fastcall: ecx = pObj, [esp+4] = uFlags)
void* __fastcall ENGINE_ScalarDeletingDtor_WidgetF(void* pObj, int32_t nUnusedEdx, uint32_t uFlags)
{
    (void)nUnusedEdx;
    UI_DestroyWidgetF_6FB4B3C0(pObj);
    if ((uFlags & 1u) != 0 && pObj != nullptr)
    {
        SMemFree(pObj, gszDeleteTag_6FB80328, -1, 0);
    }
    return pObj;
}

// 1.12a: 0x6FAB1C40 (custom: eax = nMonsterId, edx = nMode -> __fastcall)
uint32_t __fastcall ENGINE_CheckMonStats2ModeFlag(int32_t nMonsterId, int32_t nMode)
{
    D2MonStats2Txt* const pMonStats2Txt = ENGINE_GetMonStats2TxtRecordFromMonsterId(nMonsterId);
    if (pMonStats2Txt == nullptr)
    {
        return 0;
    }
    return static_cast<uint32_t>(pMonStats2Txt->nModeFlags[nMode >> 3]) & gdwBitMasks[nMode & 7];
}

// 1.12a: 0x6FAB1CB0 (custom: eax = nMonType -> __fastcall)
D2MonTypeTxt* __fastcall ENGINE_GetMonTypeTxtRecord(int32_t nMonType)
{
    // Note: Original 1.12a Engine.h checks nMonPropTxtRecordCount (+0xC34) before indexing pMonTypeTxt (+0xC3C)
    if (nMonType >= 0 && nMonType < sgptDataTables->nMonPropTxtRecordCount)
    {
        return &sgptDataTables->pMonTypeTxt[nMonType];
    }
    return sgptDataTables->pMonTypeTxt;
}

// 1.12a: 0x6FAB1CE0 (__thiscall / __fastcall: ecx = pObj, [esp+4] = uFlags)
void* __fastcall ENGINE_ScalarDeletingDtor_WidgetG(void* pObj, int32_t nUnusedEdx, uint32_t uFlags)
{
    (void)nUnusedEdx;
    UI_DestroyWidgetG_6FB34B30(pObj);
    if ((uFlags & 1u) != 0 && pObj != nullptr)
    {
        SMemFree(pObj, gszDeleteTag_6FB80328, -1, 0);
    }
    return pObj;
}

// 1.12a: 0x6FAB1D10 (__thiscall / __fastcall: ecx = pObj, [esp+4] = uFlags)
void* __fastcall ENGINE_ScalarDeletingDtor_WidgetH(void* pObj, int32_t nUnusedEdx, uint32_t uFlags)
{
    (void)nUnusedEdx;
    UI_DestroyWidgetH_6FB347D0(pObj);
    if ((uFlags & 1u) != 0 && pObj != nullptr)
    {
        SMemFree(pObj, gszDeleteTag_6FB80328, -1, 0);
    }
    return pObj;
}

// 1.12a: 0x6FAB1D40 (custom: eax = nSkillId -> __fastcall)
D2SkillDescTxt* __fastcall ENGINE_GetSkillDescTxtRecordFromSkillId(int32_t nSkillId)
{
    D2SkillsTxt* const pSkillsTxt = ENGINE_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxt != nullptr)
    {
        const int32_t nSkillDesc = pSkillsTxt->wSkillDesc;
        if (nSkillDesc >= 0 && nSkillDesc < sgptDataTables->nSkillDescTxtRecordCount)
        {
            return &sgptDataTables->pSkillDescTxt[nSkillDesc];
        }
    }
    return nullptr;
}

// 1.12a: 0x6FAB1D90 (custom: eax = pCriticalSection -> __fastcall)
void __fastcall ENGINE_DeleteCriticalSection(LPCRITICAL_SECTION pCriticalSection)
{
    DeleteCriticalSection(pCriticalSection);
}

// =============================================================================
// SECTION 2: UI/waygate.cpp
// VA Range: 0x6FAC09D0 .. 0x6FAC1FC0 (15 functions)
// =============================================================================

// 1.12a: 0x6FAC09D0 (custom: eax = nLocalX, ecx = nLocalY -> __fastcall)
uint8_t __fastcall WAYGATE_IsCursorInCloseButton(int32_t nLocalX, int32_t nLocalY)
{
    if (nLocalX >= 0x111 && nLocalX <= 0x134 && nLocalY >= 0x183 && nLocalY <= 0x1A4)
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FAC0A00 (__stdcall)
void __stdcall WAYGATE_BuildActWaypointTable(void)
{
    gnWaypointEntryCount_6FBCB95A      = 0;
    gnActiveDestWaypointCount_6FBCB95E = 0;

    D2ActiveRoomStrc* const pPlayerRoom = UNITS_GetRoom(gpClientPlayer_6FBCC3D0);
    const int32_t nPlayerLevelId = DUNGEON_GetLevelIdFromRoom(pPlayerRoom);

    for (int32_t i = 0; i < 9; ++i)
    {
        gaWaypointEntries_6FBCB90C[i].nLevelId   = 0;
        gaWaypointEntries_6FBCB90C[i].bActivated = 0;
    }

    const uint32_t uAct = static_cast<uint32_t>(gnSelectedWaypointActTab_6FBCB956);
    if (uAct > 5u)
    {
        gbWaypointCancelSent_6FBCB955 = 0;
        return;
    }

    D2WaypointActTableStrc& rActInfo = gWaypointActTable_6FBA8B74[uAct];
    if (!rActInfo.bTableInitialized)
    {
        rActInfo.bTableInitialized = true;
        for (int32_t nLvl = rActInfo.nStartLevelId; nLvl <= rActInfo.nEndLevelId; ++nLvl)
        {
            short wWpNo = 0;
            if (WAYPOINTS_GetWaypointNoFromLevelId(nLvl, &wWpNo))
            {
                const uint8_t uWpNo = static_cast<uint8_t>(wWpNo);
                D2WaypointActTableStrc& rCurAct = gWaypointActTable_6FBA8B74[gnSelectedWaypointActTab_6FBCB956];
                if (static_cast<uint8_t>(rCurAct.nWpNo) == 0xFFu)
                {
                    rCurAct.nWpNo = static_cast<char>(uWpNo);
                }
                if (uWpNo > static_cast<uint8_t>(rCurAct.nLastWpNo))
                {
                    rCurAct.nLastWpNo = static_cast<char>(uWpNo);
                }
            }
        }
    }

    const int32_t nFirstWp = static_cast<uint8_t>(gWaypointActTable_6FBA8B74[uAct].nWpNo);
    for (int32_t nWp = nFirstWp; nWp <= static_cast<int32_t>(static_cast<uint8_t>(gWaypointActTable_6FBA8B74[gnSelectedWaypointActTab_6FBCB956].nLastWpNo)); ++nWp)
    {
        int32_t nLevelId = 0;
        if (WAYPOINTS_IsActivated(gpPlayerWaypointData_6FBCB951, nWp))
        {
            if (WAYPOINTS_GetLevelIdFromWaypointNo(nWp, &nLevelId))
            {
                const int32_t nIdx = gnWaypointEntryCount_6FBCB95A;
                gaWaypointEntries_6FBCB90C[nIdx].nLevelId = nLevelId;
                if (nLevelId == nPlayerLevelId)
                {
                    gaWaypointEntries_6FBCB90C[nIdx].bActivated = 0;
                }
                else
                {
                    gaWaypointEntries_6FBCB90C[nIdx].bActivated = 1;
                    ++gnActiveDestWaypointCount_6FBCB95E;
                }
                gnWaypointEntryCount_6FBCB95A = nIdx + 1;
            }
        }
        else
        {
            if (WAYPOINTS_GetLevelIdFromWaypointNo(nWp, &nLevelId))
            {
                const int32_t nIdx = gnWaypointEntryCount_6FBCB95A;
                gaWaypointEntries_6FBCB90C[nIdx].nLevelId   = nLevelId;
                gaWaypointEntries_6FBCB90C[nIdx].bActivated = 0;
                gnWaypointEntryCount_6FBCB95A = nIdx + 1;
            }
        }
    }
}

// 1.12a: 0x6FAC0B90 (custom: eax = nRequestedAct -> __fastcall)
void __fastcall WAYGATE_SelectValidActTab(int32_t nRequestedAct)
{
    int32_t nAct = (nRequestedAct < gnWaypointActCount_6FBA8BAC) ? nRequestedAct : 0;

    switch (nAct)
    {
    case 4:
        if (!QUESTRECORD_GetQuestState(gpClientQuestFlags_6FBCB643, 28, 0))
        {
            nAct = 3;
        }
        // fallthrough
    case 3:
        if (!QUESTRECORD_GetQuestState(gpClientQuestFlags_6FBCB643, 23, 0))
        {
            nAct = 2;
        }
        // fallthrough
    case 2:
        if (!QUESTRECORD_GetQuestState(gpClientQuestFlags_6FBCB643, 15, 0))
        {
            nAct = 1;
        }
        // fallthrough
    case 1:
        if (!QUESTRECORD_GetQuestState(gpClientQuestFlags_6FBCB643, 7, 0))
        {
            nAct = 0;
        }
        break;
    default:
        break;
    }

    gnSelectedWaypointActTab_6FBCB956 = nAct;
}

// 1.12a: 0x6FAC0C70 (__stdcall)
void __stdcall WAYGATE_FreeCellFiles(void)
{
    if (gpWaygateBgCell_6FBCB941 != nullptr)
    {
        D2CMP_CelFileFreeHardware(gpWaygateBgCell_6FBCB941);
        FOG_Free(gpWaygateBgCell_6FBCB941, gszArchiveCpp_6FB807A0, 0x7A, 0);
        gpWaygateBgCell_6FBCB941 = nullptr;
    }
    if (gpWaygateTabsCell_6FBCB945 != nullptr)
    {
        D2CMP_CelFileFreeHardware(gpWaygateTabsCell_6FBCB945);
        FOG_Free(gpWaygateTabsCell_6FBCB945, gszArchiveCpp_6FB807A0, 0x7A, 0);
        gpWaygateTabsCell_6FBCB945 = nullptr;
    }
    if (gpWaygateIconsCell_6FBCB949 != nullptr)
    {
        D2CMP_CelFileFreeHardware(gpWaygateIconsCell_6FBCB949);
        FOG_Free(gpWaygateIconsCell_6FBCB949, gszArchiveCpp_6FB807A0, 0x7A, 0);
        gpWaygateIconsCell_6FBCB949 = nullptr;
    }
}

// 1.12a: 0x6FAC0D00 (custom: esi = nLocalX, edi = nLocalY -> __fastcall)
int32_t __fastcall WAYGATE_HitTestActTab(int32_t nLocalX, int32_t nLocalY)
{
    if (FOG_IsExpansion() && gbIsExpansionChar_6FBB330C)
    {
        if (nLocalY > 0x1E || nLocalX > 0x140)
        {
            return -1;
        }
        if (nLocalX < 0x40)
        {
            return 0;
        }
        if (nLocalX < 0x80)
        {
            return 1;
        }
        if (nLocalX < 0xC0)
        {
            return 2;
        }
        return (nLocalX >= 0x100) ? 4 : 3;
    }

    if (nLocalY > 0x1E || nLocalX > 0x140)
    {
        return -1;
    }
    if (nLocalX < 0x50)
    {
        return 0;
    }
    if (nLocalX < 0xA0)
    {
        return 1;
    }
    return (nLocalX >= 0xF0) ? 3 : 2;
}

// 1.12a: 0x6FAC0D80 (__stdcall)
void __stdcall WAYGATE_InitForCurrentPlayerAct(void)
{
    if (gpClientPlayer_6FBCC3D0 != nullptr)
    {
        D2ActiveRoomStrc* const pRoom = UNITS_GetRoom(gpClientPlayer_6FBCC3D0);
        if (pRoom != nullptr)
        {
            const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
            if (nLevelId != 0)
            {
                const uint8_t nAct = DRLG_GetActNoFromLevelId(nLevelId);
                WAYGATE_SelectValidActTab(nAct);
                WAYGATE_BuildActWaypointTable();
                return;
            }
        }
    }

    gnSelectedWaypointActTab_6FBCB956 = 0;
    WAYGATE_BuildActWaypointTable();
}

// 1.12a: 0x6FAC0DC0 (__stdcall)
void __stdcall WAYGATE_SendCancelPacket(void)
{
    if (!gbWaypointCancelSent_6FBCB955 && gpClientPlayer_6FBCC3D0 != nullptr)
    {
        if (UNITS_GetRoom(gpClientPlayer_6FBCC3D0) != nullptr)
        {
            D2GSPacketClt49Strc tPacket = {};
            tPacket.nHeader     = 0x49;
            tPacket.dwWaygateId = gdwActiveWaygateUnitId_6FBCB94D;
            tPacket.dwDestLevel = 0;
            MSG_SendRawClientPacket(sizeof(tPacket), &tPacket);
            gbWaypointCancelSent_6FBCB955 = 1;
        }
    }
}

// 1.12a: 0x6FAC0E20 (__stdcall)
void __stdcall WAYGATE_EnsureCellFilesLoaded(void)
{
    char szPath[260] = {};

    if (gpWaygateBgCell_6FBCB941 == nullptr)
    {
        std::sprintf(szPath, gszWaygateBgFmt_6FB87160, gszDataGlobal_6FB80424);
        gpWaygateBgCell_6FBCB941 = ARCHIVE_LoadClientCellFile(szPath, 0);
    }

    if (gpWaygateTabsCell_6FBCB945 == nullptr)
    {
        if (FOG_IsExpansion() && gbIsExpansionChar_6FBB330C)
        {
            std::sprintf(szPath, gszExpWaygateTabsFmt_6FB87144, gszDataGlobal_6FB80424);
        }
        else
        {
            std::sprintf(szPath, gszWaygateTabsFmt_6FB8712C, gszDataGlobal_6FB80424);
        }
        gpWaygateTabsCell_6FBCB945 = ARCHIVE_LoadClientCellFile(szPath, 0);
    }

    if (gpWaygateIconsCell_6FBCB949 == nullptr)
    {
        std::sprintf(szPath, gszWaygateIconsFmt_6FB87114, gszDataGlobal_6FB80424);
        gpWaygateIconsCell_6FBCB949 = ARCHIVE_LoadClientCellFile(szPath, 0);
    }
}

// 1.12a: 0x6FAC0EE0 (__stdcall; thunk to WAYGATE_SendCancelPacket)
void __stdcall WAYGATE_OnMenuCloseNotify(void)
{
    WAYGATE_SendCancelPacket();
}

// 1.12a: 0x6FAC0EF0 (__stdcall)
void __stdcall WAYGATE_ShutdownWaypointSession(void)
{
    WAYGATE_SendCancelPacket();
    if (gpPlayerWaypointData_6FBCB951 != nullptr)
    {
        WAYPOINTS_FreeWaypointData(nullptr, gpPlayerWaypointData_6FBCB951);
        gpPlayerWaypointData_6FBCB951 = nullptr;
    }
    WAYGATE_FreeCellFiles();
}

// Internal helper matching the repeated Waypoint Menu close + viewport restore sequence
static void WAYGATE_ClosePanelAndRestoreViewport(BOOL bRecenterRightCursor)
{
    const BOOL bWasOpen = gbWaypointMenuOpen_6FBB2BA8;
    FOG_IsExpansion();
    if (gnPanelDescriptorCount_6FB8F368 != 0x26)
    {
        ENGINE_FatalHalt(0x1389);
    }

    gbWaypointMenuOpen_6FBB2BA8 = FALSE;
    if (bWasOpen)
    {
        MSG_UnregisterPanelHandlers(5, WINDOW_GetWindow(), gWaypointPanelMsgTable_6FB815D8);
        WAYGATE_SendCancelPacket();
        UI_OnPanelClosedRefresh();
    }

    const int32_t nOldMouseX = gnMouseX_6FBB1638;
    const int32_t nOldMouseY = gnMouseY_6FBB1634;

    if (!gabPanelOpenStates_6FBB2B58[1] && !gabPanelOpenStates_6FBB2B58[4])
    {
        gnViewShiftOffsetX_6FBCC1D4 = 0;
        gnViewShiftMode_6FBCC1D0    = 0;
        D2GFX_SetViewMatrixAdjust(0);
        VIEW_SetViewportBounds(gpMainViewport_6FBCC028, 0, 0, gnWorldViewportWidth_6FBCD528, gnWorldViewportHeight_6FBCD524);

        if (bRecenterRightCursor && nOldMouseX > gnScreenWidth_6FB8C6E0 / 2)
        {
            UI_SetCursorPosition(nOldMouseX - (gnScreenWidth_6FB8C6E0 / 4), nOldMouseY);
        }
    }
    else
    {
        const int32_t nQuarterScreen = -(gnScreenWidth_6FB8C6E0 / 4);
        gnViewShiftOffsetX_6FBCC1D4 = nQuarterScreen;
        gnViewShiftMode_6FBCC1D0    = 1;
        D2GFX_SetViewMatrixAdjust(nQuarterScreen);

        const int32_t nQuarterVp = gnWorldViewportWidth_6FBCD528 / 4;
        VIEW_SetViewportBounds(
            gpMainViewport_6FBCC028,
            -nQuarterVp,
            0,
            gnWorldViewportWidth_6FBCD528 - nQuarterVp,
            gnWorldViewportHeight_6FBCD524);
    }
}

// 1.12a: 0x6FAC0F20 (__stdcall: [esp+4] = pMsg)
void __stdcall WAYGATE_HandleMouseDown(D2WinMsgStrc* pMsg)
{
    int32_t nMouseX = pMsg->nX;
    int32_t nMouseY = pMsg->nY;

    if (INV_GetCursorItem() != nullptr)
    {
        return;
    }

    if (nMouseX > gnScreenWidth_6FB8C6E0 / 2 || nMouseY > gnScreenHeight_6FB8C6E4 - 49)
    {
        WAYGATE_SendCancelPacket();
        WAYGATE_ClosePanelAndRestoreViewport(TRUE);
        return;
    }

    const int32_t nLocalX = nMouseX - gnUiPanelOffsetX_6FBCBD28;
    const int32_t nLocalY = nMouseY + gnUiPanelOffsetY_6FBCBD2C;

    const int32_t nHitTab = WAYGATE_HitTestActTab(nLocalX, nLocalY);
    if (nHitTab != -1)
    {
        if (nHitTab != gnSelectedWaypointActTab_6FBCB956)
        {
            SOUND_PlayClientSoundEx(6, nullptr, 0, 0, 0);
        }
        WAYGATE_SelectValidActTab(nHitTab);
        WAYGATE_BuildActWaypointTable();
        pMsg->bHandled = TRUE;
        pMsg->lResult  = 0;
        SMsgBreakHandlerChain(pMsg);
    }

    gnWaypointClickedRow_6FBCB93D = -1;
    if (WAYGATE_IsCursorInCloseButton(nLocalX, nLocalY))
    {
        gbWaypointClosePressed_6FBCB93C = 1;
        SOUND_PlayClientSoundEx(4, nullptr, 0, 0, 0);
    }
    else
    {
        const int32_t nHitRow = WAYGATE_HitTestEntry(nLocalX, nLocalY);
        gnWaypointClickedRow_6FBCB93D = nHitRow;
        if (nHitRow != -1)
        {
            SOUND_PlayClientSoundEx(4, nullptr, 0, 0, 0);
        }
    }

    if (pMsg->nX <= gnScreenWidth_6FB8C6E0 && pMsg->nY <= gnScreenHeight_6FB8C6E4 - 49)
    {
        pMsg->bHandled = TRUE;
        pMsg->lResult  = 0;
        SMsgBreakHandlerChain(pMsg);
    }
}

// 1.12a: 0x6FAC1180 (__stdcall: [esp+4] = pMsg)
void __stdcall WAYGATE_HandleMouseUp(D2WinMsgStrc* pMsg)
{
    const int32_t nMouseX = pMsg->nX;
    const int32_t nMouseY = pMsg->nY;

    if (INV_GetCursorItem() != nullptr)
    {
        return;
    }

    if (nMouseX > gnScreenWidth_6FB8C6E0 || nMouseY > gnScreenHeight_6FB8C6E4 - 49)
    {
        gbWaypointClosePressed_6FBCB93C = 0;
        gnWaypointClickedRow_6FBCB93D   = -1;
        return;
    }

    const int32_t nLocalX = nMouseX - gnUiPanelOffsetX_6FBCBD28;
    const int32_t nLocalY = nMouseY + gnUiPanelOffsetY_6FBCBD2C;

    if (gbWaypointClosePressed_6FBCB93C)
    {
        if (WAYGATE_IsCursorInCloseButton(nLocalX, nLocalY))
        {
            WAYGATE_ClosePanelAndRestoreViewport(FALSE);
            WAYGATE_SendCancelPacket();
        }
    }
    else if (gnWaypointClickedRow_6FBCB93D != -1)
    {
        const int32_t nHitRow = WAYGATE_HitTestEntry(nLocalX, nLocalY);
        if (nHitRow == gnWaypointClickedRow_6FBCB93D)
        {
            pMsg->bHandled = TRUE;
            pMsg->lResult  = 0;
            SMsgBreakHandlerChain(pMsg);

            const uint32_t dwDestLevel = static_cast<uint32_t>(gaWaypointEntries_6FBCB90C[gnWaypointClickedRow_6FBCB93D].nLevelId);
            MSG_SendByte2Dwords(0x49, gdwActiveWaygateUnitId_6FBCB94D, dwDestLevel);
            gbWaypointCancelSent_6FBCB955   = 1;
            WAYGATE_SendCancelPacket();
            gbWaypointClosePressed_6FBCB93C = 0;
            gnWaypointClickedRow_6FBCB93D   = -1;

            WAYGATE_ClosePanelAndRestoreViewport(FALSE);
            SOUND_PlayClientSoundEx(0x8B7, nullptr, 0, 0, 0);

            pMsg->bHandled = TRUE;
            pMsg->lResult  = 0;
            SMsgBreakHandlerChain(pMsg);
            return;
        }
    }

    gbWaypointClosePressed_6FBCB93C = 0;
    gnWaypointClickedRow_6FBCB93D   = -1;
    pMsg->bHandled = TRUE;
    pMsg->lResult  = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FAC1460 (__stdcall: [esp+4] = pPacket; S->C 0x63 Waypoint Menu Open)
void __stdcall WAYGATE_HandleOpenWaypointPacket(const D2GSPacketSrv63Strc* pPacket)
{
    const BOOL bWasOpen = gbWaypointMenuOpen_6FBB2BA8;
    FOG_IsExpansion();
    if (gnPanelDescriptorCount_6FB8F368 != 0x26)
    {
        ENGINE_FatalHalt(0x1389);
    }

    if (gbEscMenuOpen_6FBB5490)
    {
        return;
    }

    for (int32_t i = 0; i < gnPanelDescriptorCount_6FB8F368; ++i)
    {
        if (gabPanelOpenStates_6FBB2B58[i] == 1)
        {
            const int32_t nCloseMode = gpPanelDescriptors_6FB8F2D0[i]->nWaypointCloseMode;
            switch (nCloseMode)
            {
            case 0:
                break;
            case 1:
                UI_SetPanelState(i, 1, 0);
                break;
            case 2:
            case 3:
                return;
            case 4:
                if (gbActiveNpcInteract_6FBCB629)
                {
                    NPCMENU_CancelActiveInteraction(gdwActiveNpcGUID_6FBCB625);
                }
                break;
            default:
                return;
            }
        }
    }

    if (gpClientPlayer_6FBCC3D0 == nullptr ||
        (gpClientPlayer_6FBCC3D0->dwUnitType == UNIT_PLAYER && !UNIT_IsUnitDeadOrCorpse(gpClientPlayer_6FBCC3D0)))
    {
        gbWaypointMenuOpen_6FBB2BA8 = TRUE;
    }

    const BOOL bNowOpen = gbWaypointMenuOpen_6FBB2BA8;
    if (!bWasOpen)
    {
        if (bNowOpen)
        {
            MSG_RegisterPanelHandlers(5, WINDOW_GetWindow(), gWaypointPanelMsgTable_6FB815D8);
        }
    }
    else if (!bNowOpen)
    {
        MSG_UnregisterPanelHandlers(5, WINDOW_GetWindow(), gWaypointPanelMsgTable_6FB815D8);
        WAYGATE_SendCancelPacket();
        UI_OnPanelClosedRefresh();
    }

    const int32_t nOldMouseX = gnMouseX_6FBB1638;
    const int32_t nOldMouseY = gnMouseY_6FBB1634;

    if (!gabPanelOpenStates_6FBB2B58[1] && !gabPanelOpenStates_6FBB2B58[4])
    {
        if (bNowOpen)
        {
            const int32_t nQuarterScreen = gnScreenWidth_6FB8C6E0 / 4;
            gnViewShiftOffsetX_6FBCC1D4 = nQuarterScreen;
            gnViewShiftMode_6FBCC1D0    = 2;
            D2GFX_SetViewMatrixAdjust(nQuarterScreen);

            const int32_t nQuarterVp = gnWorldViewportWidth_6FBCD528 / 4;
            VIEW_SetViewportBounds(
                gpMainViewport_6FBCC028,
                nQuarterVp,
                0,
                gnWorldViewportWidth_6FBCD528 + nQuarterVp,
                gnWorldViewportHeight_6FBCD524);

            if (nOldMouseX < gnScreenWidth_6FB8C6E0 - nQuarterScreen)
            {
                UI_SetCursorPosition(nOldMouseX + nQuarterScreen, nOldMouseY);
            }
        }
        else
        {
            gnViewShiftOffsetX_6FBCC1D4 = 0;
            gnViewShiftMode_6FBCC1D0    = 0;
            D2GFX_SetViewMatrixAdjust(0);
            VIEW_SetViewportBounds(gpMainViewport_6FBCC028, 0, 0, gnWorldViewportWidth_6FBCD528, gnWorldViewportHeight_6FBCD524);

            if (nOldMouseX > gnScreenWidth_6FB8C6E0 / 2)
            {
                UI_SetCursorPosition(nOldMouseX - (gnScreenWidth_6FB8C6E0 / 4), nOldMouseY);
            }
        }
    }
    else
    {
        if (bNowOpen)
        {
            gnViewShiftOffsetX_6FBCC1D4 = 0;
            gnViewShiftMode_6FBCC1D0    = 3;
            D2GFX_SetViewMatrixAdjust(0);
            VIEW_SetViewportBounds(gpMainViewport_6FBCC028, 0, 0, gnWorldViewportWidth_6FBCD528, gnWorldViewportHeight_6FBCD524);
        }
        else
        {
            const int32_t nQuarterScreen = -(gnScreenWidth_6FB8C6E0 / 4);
            gnViewShiftOffsetX_6FBCC1D4 = nQuarterScreen;
            gnViewShiftMode_6FBCC1D0    = 1;
            D2GFX_SetViewMatrixAdjust(nQuarterScreen);

            const int32_t nQuarterVp = gnWorldViewportWidth_6FBCD528 / 4;
            VIEW_SetViewportBounds(
                gpMainViewport_6FBCC028,
                -nQuarterVp,
                0,
                gnWorldViewportWidth_6FBCD528 - nQuarterVp,
                gnWorldViewportHeight_6FBCD524);
        }
    }

    gnInteractSnapDistX_6FB90354 = 16;
    gnInteractSnapDistY_6FB90358 = 16;
    gnAutoWalkTargetX_6FBCC040   = 0;
    gnAutoWalkTargetY_6FBCC044   = 0;
    gnAutoWalkStateA_6FBCC05C    = 0;
    gnAutoWalkStateB_6FBCC060    = 0;
    gnPendingMouseActionB_6FBCC11C = 0;
    gnPendingMouseActionA_6FBCC118 = 0;

    WAYGATE_InitForCurrentPlayerAct();

    gdwActiveWaygateUnitId_6FBCB94D = pPacket->dwWaygateId;
    WAYPOINTS_CopyAndValidateWaypointData(gpPlayerWaypointData_6FBCB951, const_cast<D2WaypointDataStrc*>(&pPacket->tWaypoints));
    gbWaypointCancelSent_6FBCB955 = 0;
    WAYGATE_BuildActWaypointTable();
}

// 1.12a: 0x6FAC1790 (__stdcall: [esp+4] = pMsg)
void __stdcall WAYGATE_HandleKeyInput(D2WinMsgStrc* pMsg)
{
    if (pMsg->nMessage == WM_CHAR && pMsg->wParam == VK_TAB)
    {
        return;
    }
    if (pMsg->nMessage == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
    {
        return;
    }

    WAYGATE_SendCancelPacket();
    WAYGATE_ClosePanelAndRestoreViewport(TRUE);

    pMsg->bHandled = TRUE;
    pMsg->lResult  = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FAC1900 (__stdcall)
void __stdcall WAYGATE_DrawMenu(void)
{
    WAYGATE_EnsureCellFilesLoaded();

    if (gpClientPlayer_6FBCC3D0 == nullptr || gpClientPlayer_6FBCC3D0->dwAnimMode == PLRMODE_DEAD)
    {
        WAYGATE_ClosePanelAndRestoreViewport(FALSE);
        WAYGATE_SendCancelPacket();
    }

    D2Win_10127_SetFont(D2FONT_FONT16);

    D2GfxDataStrc tGfxData = {};
    tGfxData.pCellFile  = gpWaygateBgCell_6FBCB941;
    tGfxData.nDirection = 0;

    // 4-quadrant Waypoint background frame
    tGfxData.nFrame = 0;
    TEXTURE_CelDraw(&tGfxData, gnUiPanelOffsetX_6FBCBD28, gnUiPanelOffsetY_6FBCBD2C + gnScreenHeight_6FB8C6E4 - 224, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
    tGfxData.nFrame = 1;
    TEXTURE_CelDraw(&tGfxData, gnUiPanelOffsetX_6FBCBD28 + 256, gnUiPanelOffsetY_6FBCBD2C + gnScreenHeight_6FB8C6E4 - 224, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
    tGfxData.nFrame = 2;
    TEXTURE_CelDraw(&tGfxData, gnUiPanelOffsetX_6FBCBD28, gnUiPanelOffsetY_6FBCBD2C + gnScreenHeight_6FB8C6E4 - 48, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
    tGfxData.nFrame = 3;
    TEXTURE_CelDraw(&tGfxData, gnUiPanelOffsetX_6FBCBD28 + 256, gnUiPanelOffsetY_6FBCBD2C + gnScreenHeight_6FB8C6E4 - 48, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);

    const int32_t nLocalMouseX = gnMouseX_6FBB1638 - gnUiPanelOffsetX_6FBCBD28;
    const int32_t nLocalMouseY = gnMouseY_6FBB1634 + gnUiPanelOffsetY_6FBCBD2C;

    tGfxData.pCellFile = gpWaygateTabsCell_6FBCB945;

    int32_t anTabX[5]    = {};
    int32_t nTabCount    = 4;
    int32_t nExpYAdjust  = 0;
    if (FOG_IsExpansion() && gbIsExpansionChar_6FBB330C)
    {
        nExpYAdjust = 1;
        nTabCount   = 5;
        anTabX[0]   = 5;
        anTabX[1]   = 0x43;
        anTabX[2]   = 0x81;
        anTabX[3]   = 0xBF;
        anTabX[4]   = 0xFD;
    }
    else
    {
        nExpYAdjust = 0;
        nTabCount   = 4;
        anTabX[0]   = 3;
        anTabX[1]   = 0x51;
        anTabX[2]   = 0x9F;
        anTabX[3]   = 0xED;
    }

    for (int32_t nTab = 0; nTab < nTabCount; ++nTab)
    {
        if (nTab == gnSelectedWaypointActTab_6FBCB956)
        {
            tGfxData.nFrame = nTab * 2;
            TEXTURE_CelDraw(&tGfxData, gnUiPanelOffsetX_6FBCBD28 + anTabX[nTab], nExpYAdjust - gnUiPanelOffsetY_6FBCBD2C + 0x21, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
        }
        else
        {
            bool bTabUnlocked = true;
            switch (nTab)
            {
            case 1:
                bTabUnlocked = (QUESTRECORD_GetQuestState(gpClientQuestFlags_6FBCB643, 7, 0) != 0);
                break;
            case 2:
                bTabUnlocked = (QUESTRECORD_GetQuestState(gpClientQuestFlags_6FBCB643, 15, 0) != 0);
                break;
            case 3:
                bTabUnlocked = (QUESTRECORD_GetQuestState(gpClientQuestFlags_6FBCB643, 23, 0) != 0);
                break;
            case 4:
            {
                const bool bQ26 = (QUESTRECORD_GetQuestState(gpClientQuestFlags_6FBCB643, 26, 0) != 0);
                bTabUnlocked = (FOG_IsExpansion() && gbIsExpansionChar_6FBB330C && bQ26);
                break;
            }
            default:
                break;
            }

            if (bTabUnlocked)
            {
                tGfxData.nFrame = nTab * 2 + 1;
                TEXTURE_CelDraw(&tGfxData, gnUiPanelOffsetX_6FBCBD28 + anTabX[nTab], nExpYAdjust - gnUiPanelOffsetY_6FBCBD2C + 0x21, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
            }
        }
    }

    // Close button
    if (gpBuySellBtnCell_6FBCBD4C == nullptr)
    {
        char szBtnPath[260] = {};
        wsprintfA(szBtnPath, gszBuySellBtnFmt_6FB84300, gszDataGlobal_6FB80424);
        gpBuySellBtnCell_6FBCBD4C = ARCHIVE_LoadClientCellFile(szBtnPath, 0);
    }
    tGfxData.pCellFile = gpBuySellBtnCell_6FBCBD4C;
    tGfxData.nFrame    = 10 + (gbWaypointClosePressed_6FBCB93C != 0 ? 1 : 0);
    TEXTURE_CelDraw(&tGfxData, gnUiPanelOffsetX_6FBCBD28 + 0x111, 0x1A1 - gnUiPanelOffsetY_6FBCBD2C, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);

    // Waypoint rows
    const int32_t nCurrentLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(gpClientPlayer_6FBCC3D0));
    tGfxData.pCellFile = gpWaygateIconsCell_6FBCB949;

    for (int32_t i = 0; i < gnWaypointEntryCount_6FBCB95A; ++i)
    {
        const D2WaypointTableStrc& rEntry      = gaWaypointEntries_6FBCB90C[i];
        const D2WaypointCoordTableStrc& rCoord = gWaypointCoordTable_6FBA8BB0[i];

        D2C_StringColorCodes eTextColor = STRCOLOR_GREY; // 5
        if (rEntry.bActivated)
        {
            if (rEntry.nLevelId == nCurrentLevelId)
            {
                tGfxData.nFrame = (gnWaypointClickedRow_6FBCB93D == i) ? 1 : 0;
            }
            else
            {
                tGfxData.nFrame = 3 + ((gnWaypointClickedRow_6FBCB93D == i) ? 1 : 0);
            }
            TEXTURE_CelDraw(&tGfxData, gnUiPanelOffsetX_6FBCBD28 + rCoord.nX, rCoord.nY - gnUiPanelOffsetY_6FBCBD2C, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
            eTextColor = STRCOLOR_WHITE; // 0
        }

        if (!gbWaypointClosePressed_6FBCB93C && gnWaypointClickedRow_6FBCB93D != -1 && gnWaypointClickedRow_6FBCB93D == i)
        {
            eTextColor = STRCOLOR_BLUE; // 3
        }

        if (rEntry.nLevelId == nCurrentLevelId)
        {
            eTextColor      = STRCOLOR_BLUE; // 3
            tGfxData.nFrame = 0;
            TEXTURE_CelDraw(&tGfxData, gnUiPanelOffsetX_6FBCBD28 + rCoord.nX, rCoord.nY - gnUiPanelOffsetY_6FBCBD2C, 0xFFFFFFFFu, DRAWMODE_NORMAL, nullptr);
        }

        const Unicode* wszLevelName = nullptr;
        D2LevelsTxt* const pLevelsTxt = DATATBLS_GetLevelsTxtRecord(rEntry.nLevelId);
        if (pLevelsTxt != nullptr)
        {
            if (rEntry.nLevelId == LEVEL_DENOFEVIL) // 8
            {
                if (gnServerWorldStateInit_6FBCBCE0 == 0)
                {
                    ENGINE_FatalHalt(0x60);
                }
                wszLevelName = D2LANG_GetStringFromTblIndex(gabServerWorldFlags_6FBB1678[1] ? 0x13B8 : 0x13B7);
            }
            else
            {
                wszLevelName = pLevelsTxt->wszLevelName;
            }
        }
        else
        {
            wszLevelName = D2LANG_GetStringFromTblIndex(0x150D);
        }

        D2Win_10117_DrawText(
            wszLevelName,
            gnUiPanelOffsetX_6FBCBD28 + rCoord.nTextX,
            rCoord.nTextY - gnUiPanelOffsetY_6FBCBD2C,
            eTextColor,
            0);
    }

    // Close button hover tooltip ("cancel" / 0x1022)
    if (WAYGATE_IsCursorInCloseButton(nLocalMouseX, nLocalMouseY))
    {
        const Unicode* const wszCancel = D2LANG_GetStringFromTblIndex(0x1022);
        const int32_t nHalfWidth = D2Win_10121_GetTextWidth(wszCancel) / 2;
        D2GFX_DrawBox(
            gnUiPanelOffsetX_6FBCBD28 - nHalfWidth + 0x11F,
            0x172 - gnUiPanelOffsetY_6FBCBD2C,
            gnUiPanelOffsetX_6FBCBD28 + nHalfWidth + 0x126,
            0x183 - gnUiPanelOffsetY_6FBCBD2C,
            0,
            DRAWMODE_TRANS50);
        D2Win_10117_DrawText(
            wszCancel,
            gnUiPanelOffsetX_6FBCBD28 - nHalfWidth + 0x124,
            0x181 - gnUiPanelOffsetY_6FBCBD2C,
            STRCOLOR_WHITE,
            0);
    }

    // Header title ("Select a Waypoint" 0xF96 or "No Waypoints Activated" 0xF97)
    const Unicode* const wszHeader = D2LANG_GetStringFromTblIndex(gnActiveDestWaypointCount_6FBCB95E != 0 ? 0x0F96 : 0x0F97);
    const int32_t nHeaderHalfWidth = D2Win_10121_GetTextWidth(wszHeader) / 2;
    D2Win_10117_DrawText(
        wszHeader,
        gnUiPanelOffsetX_6FBCBD28 - nHeaderHalfWidth + 0xA0,
        0x30 - gnUiPanelOffsetY_6FBCBD2C,
        STRCOLOR_WHITE,
        0);
}


// =============================================================================
// SECTION 3: CORE/WINMAIN.CPP - Config Getters, Command-Line, & Main Loop
// VA Range: 0x6FAC1FE0 .. 0x6FAC26E9 (26 functions)
// =============================================================================

// 1.12a: 0x6FAC1FE0 (__stdcall)
BOOL __stdcall WINMAIN_GetExitAppFlag(void)
{
    return gbExitApplication_6FBA8B70;
}

// 1.12a: 0x6FAC1FF0 (custom: eax = bExit -> __fastcall)
void __fastcall WINMAIN_SetExitAppFlag(BOOL bExit)
{
    gbExitApplication_6FBA8B70 = bExit;
}

// 1.12a: 0x6FAC2000 (__stdcall)
void __stdcall WINMAIN_RequestClientExitState(void)
{
    gnCurrentClientState_6FBA8B30 = 1;
}

// 1.12a: 0x6FAC2010 (__stdcall)
void __stdcall WINMAIN_SetCharSaveFlag08(void)
{
    gpD2Config_6FBCB908->unpackedCTemp.nCharacterSaveFlags |= 0x0008u;
}

// 1.12a: 0x6FAC2020 (__stdcall)
uint32_t __stdcall WINMAIN_CheckCharSaveFlag04(void)
{
    return static_cast<uint32_t>(gpD2Config_6FBCB908->unpackedCTemp.nCharacterSaveFlags & 0x0004u);
}

// 1.12a: 0x6FAC2030 (__stdcall)
const char* __stdcall WINMAIN_GetServerIpIfNonEmpty(void)
{
    if (gpD2Config_6FBCB908 != nullptr && std::strlen(gpD2Config_6FBCB908->szServerIP) >= 1)
    {
        return gpD2Config_6FBCB908->szServerIP;
    }
    return nullptr;
}

// 1.12a: 0x6FAC2060 (__stdcall)
const char* __stdcall WINMAIN_GetGameStatstring(void)
{
    if (gpD2Config_6FBCB908 != nullptr)
    {
        return gpD2Config_6FBCB908->szGameStatstring;
    }
    return gszEmptyConfigFallback_6FBA8B6C;
}

// 1.12a: 0x6FAC2080 (__stdcall)
const char* __stdcall WINMAIN_GetGamePasswordIfNonEmpty(void)
{
    if (gpD2Config_6FBCB908 != nullptr && std::strlen(gpD2Config_6FBCB908->szGamePassword) >= 1)
    {
        return gpD2Config_6FBCB908->szGamePassword;
    }
    return nullptr;
}

// 1.12a: 0x6FAC20B0 (__stdcall)
const char* __stdcall WINMAIN_GetGameNameIfNonEmpty(void)
{
    if (gpD2Config_6FBCB908 != nullptr && std::strlen(gpD2Config_6FBCB908->szGameName) >= 1)
    {
        return gpD2Config_6FBCB908->szGameName;
    }
    return nullptr;
}

// 1.12a: 0x6FAC20E0 (__stdcall)
BnClientInterface* __stdcall WINMAIN_GetBnClientInterface(void)
{
    return gpD2Config_6FBCB908->pComInterface;
}

// 1.12a: 0x6FAC20F0 (__stdcall)
D2ConfigStrc* __stdcall WINMAIN_GetConfig(void)
{
    return gpD2Config_6FBCB908;
}

#ifdef D2CLIENT_PURE_WINMAIN_EXPORTS
// 1.12a: 0x6FAC2100 (__stdcall; Export @10003 D2Client_10003)
extern "C" D2CLIENT_DLL_DECL void __stdcall D2Client_10003(void)
{
    const HWND hWnd = WINDOW_GetWindow();
    if (hWnd != nullptr)
    {
        SendMessageA(hWnd, WM_ACTIVATEAPP, 0, 0);
    }
    WINDOW_ShowAll();
}
#endif

// 1.12a: 0x6FAC2120 (__stdcall: [esp+4] = pArgs, [esp+8] = szExtra)
BOOL __stdcall WINMAIN_CmdArg_SetDefaultCmdLine_6FAC2120(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra)
{
    (void)szExtra;
    return SRegSaveString(gszDiabloII_6FB803FC, gszDefaultCmdLine_6FB87004, 0, pArgs->szArgument);
}

// 1.12a: 0x6FAC2140 (__stdcall: [esp+4] = pArgs, [esp+8] = szExtra)
BOOL __stdcall WINMAIN_CmdArg_PrecommitMemory_6FAC2140(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra)
{
    (void)pArgs;
    (void)szExtra;
    constexpr int32_t kPrecommitBytes = 0x06000000; // 96 MB
    uint8_t* const pBuf = static_cast<uint8_t*>(FOG_Alloc(kPrecommitBytes, gszWinMainCpp_6FB8701C, 0x11D, 0));
    uint8_t* const pEnd = pBuf + kPrecommitBytes;
    for (uint8_t* pPage = pBuf; pPage < pEnd; pPage += 0x1000)
    {
        *pPage = 0;
    }
    FOG_Free(pBuf, gszWinMainCpp_6FB8701C, 0x120, 0);
    return TRUE;
}

// 1.12a: 0x6FAC2190 (__stdcall: [esp+4] = pArgs, [esp+8] = szExtra)
BOOL __stdcall WINMAIN_CmdArg_Help_6FAC2190(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra)
{
    (void)pArgs;
    (void)szExtra;
    if (!gbCmdHelpShown_6FBCC434)
    {
        gbCmdHelpShown_6FBCC434 = TRUE;
        DWORD dwUserLen = 15;
        char szUserName[16] = {};
        GetUserNameA(szUserName, &dwUserLen);
        if (SStrCmp(szUserName, gszRseisUser_6FB87040, 0x7FFFFFFFu) != 0)
        {
            MessageBoxA(nullptr, gszCmdHelpText_6FB87050, gszDiabloII_6FB803FC, MB_OK);
        }
    }
    return FALSE;
}

// 1.12a: 0x6FAC2200 (__stdcall)
int32_t __stdcall WINMAIN_PopClientState(void)
{
    if (gnClientStateQueueCount_6FBCC438 == 0)
    {
        ENGINE_FatalHalt(0xB0);
    }

    const int32_t nPopped = ganClientStateQueue_6FBCB8F4[0];
    const int32_t nItem1  = ganClientStateQueue_6FBCB8F4[1];
    const int32_t nItem2  = ganClientStateQueue_6FBCB8F4[2];
    const int32_t nItem3  = ganClientStateQueue_6FBCB8F4[3];

    ganClientStateQueue_6FBCB8F4[0] = nItem1;
    ganClientStateQueue_6FBCB8F4[1] = nItem2;
    ganClientStateQueue_6FBCB8F4[2] = nItem3;
    --gnClientStateQueueCount_6FBCC438;

    return nPopped;
}

// 1.12a: 0x6FAC2260 (__stdcall: [esp+4] = nState)
void __stdcall WINMAIN_PushClientState(int32_t nState)
{
    if (gnClientStateQueueCount_6FBCC438 >= 5)
    {
        ENGINE_FatalHalt(0xA6);
    }

    ganClientStateQueue_6FBCB8F4[gnClientStateQueueCount_6FBCC438] = nState;
    ++gnClientStateQueueCount_6FBCC438;
}

// 1.12a: 0x6FAC22A0 (__stdcall: [esp+4] = pArgs, [esp+8] = szExtra)
BOOL __stdcall WINMAIN_CmdArg_Mode_6FAC22A0(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra)
{
    (void)szExtra;
    for (int32_t i = 0; i < 7; ++i)
    {
        const char* const szModeName = gszClientModeNames_6FBA8B50[i];
        if (szModeName != nullptr && _stricmp(szModeName, pArgs->szArgument) == 0)
        {
            gnCurrentClientState_6FBA8B30 = i;
            return TRUE;
        }
    }

    if (!gbCmdHelpShown_6FBCC434)
    {
        gbCmdHelpShown_6FBCC434 = TRUE;
        DWORD dwUserLen = 15;
        char szUserName[16] = {};
        GetUserNameA(szUserName, &dwUserLen);
        if (SStrCmp(szUserName, gszRseisUser_6FB87040, 0x7FFFFFFFu) != 0)
        {
            MessageBoxA(nullptr, gszCmdHelpText_6FB87050, gszDiabloII_6FB803FC, MB_OK);
        }
    }
    return FALSE;
}

// 1.12a: 0x6FAC2350 (__stdcall: [esp+4] = pCmdErr)
BOOL __stdcall WINMAIN_OnCmdErrorCallback(const void* pCmdErr)
{
    (void)pCmdErr;
    return WINMAIN_CmdArg_Help_6FAC2190(nullptr, nullptr);
}

// 1.12a: 0x6FAC2360 (__stdcall: [esp+4] = szExtraArg)
BOOL __stdcall WINMAIN_OnCmdExtraArgCallback(const char* szExtraArg)
{
    (void)szExtraArg;
    WINMAIN_CmdArg_Help_6FAC2190(nullptr, nullptr);
    return FALSE;
}

// 1.12a: 0x6FAC2370 (__stdcall: [esp+4] = pArgs, [esp+8] = szExtra)
BOOL __stdcall WINMAIN_CmdArg_ResetWindowClose_6FAC2370(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra)
{
    (void)pArgs;
    (void)szExtra;
    if (gbCmdFlagP_6FBCC094 != 0)
    {
        gbCmdFlagP_6FBCC094 = FALSE;
        return TRUE;
    }

    gbCmdFlagR_6FBCC08C = TRUE;
    return TRUE;
}

// 1.12a: 0x6FAC23A0 (custom: edi = pConfig -> __fastcall)
BOOL __fastcall WINMAIN_ProcessCommandLine(D2ConfigStrc* pConfig)
{
    SCmdRegisterArgList(reinterpret_cast<int>(gStormCmdArgList_6FB83510), 8);

    for (size_t i = 0; i < 1; ++i)
    {
        if (!gStartupInitCallbacks_6FB83508[i]())
        {
            return FALSE;
        }
    }

    gbCmdLineInitOk_6FB90524 = TRUE;
    for (size_t i = 0; i < 49; ++i)
    {
        if (gpfnCmdLineSwitchInits_6FB90528[i] != nullptr)
        {
            gpfnCmdLineSwitchInits_6FB90528[i](pConfig);
        }
    }

    if (!gbCmdLineInitOk_6FB90524)
    {
        return FALSE;
    }

    char szDefaultCmdLine[MAX_PATH] = {};
    if (SRegLoadString(gszDiabloII_6FB803FC, gszDefaultCmdLine_6FB87004, 0, szDefaultCmdLine, MAX_PATH))
    {
        SCmdProcess(
            reinterpret_cast<const char**>(szDefaultCmdLine),
            0,
            reinterpret_cast<int>(WINMAIN_OnCmdExtraArgCallback),
            reinterpret_cast<int>(WINMAIN_OnCmdErrorCallback));
    }

    DWORD dwNameSize = 15;
    if (gszPlayerAccountName_6FBB33E0[0] == '\0')
    {
        if (!GetUserNameA(gszPlayerAccountName_6FBB33E0, &dwNameSize))
        {
            wsprintfA(gszPlayerAccountName_6FBB33E0, gszStrFmt_6FB84638, gszNoName_6FB86FFC);
        }
    }
    return TRUE;
}

// 1.12a: 0x6FAC2480 (__stdcall: [esp+4] = pArgs, [esp+8] = szExtra)
BOOL __stdcall WINMAIN_CmdArg_SeedInit_6FAC2480(D2StormCmdCallbackArgsStrc* pArgs, const char* szExtra)
{
    (void)pArgs;
    (void)szExtra;
    if (gbCmdFlagR_6FBCC08C != 0)
    {
        gbCmdFlagR_6FBCC08C = FALSE;
        return TRUE;
    }

    gbCmdFlagP_6FBCC094 = TRUE;
    GAME_InitRandomSeed();
    GAME_SetInitSeed(gnInitGameSeed_6FBB5378);
    return TRUE;
}

// 1.12a: 0x6FAC24C0 (custom: eax = pConfig, esi = hModule -> __fastcall)
int32_t __fastcall WINMAIN_RunClient(D2ConfigStrc* pConfig, HINSTANCE hModule)
{
    gpD2Config_6FBCB908 = pConfig;
    FOG_10101(0);
    FOG_SetMemoryPoolSystemMode(nullptr, TRUE);

    ghClientModule_6FBCC428 = hModule;
    GetModuleFileNameA(hModule, gszClientModuleFileName_6FBCB7F0, MAX_PATH);

    if (!WINMAIN_ProcessCommandLine(pConfig))
    {
        return 0;
    }

    if (!ARCHIVE_InitializeClientArchives())
    {
        return 0;
    }

    D2Win_10015();
    D2Win_10014_SetWndProc(MSG_MainWindowProc);

    const BOOL bIsExpansion = FOG_IsExpansion();
    if (!STRTABLE_Initialize(reinterpret_cast<int>(D2Win_GetArchive()), nullptr, bIsExpansion))
    {
        return 0;
    }

    uint32_t dwLanguageInfo = 0;
    STRTABLE_GetLanguageInfo(&dwLanguageInfo, 0);

    const int32_t bIsOpenBnetOrTcp = (pConfig->dwGameType == 6) ? 1 : 0;
    DATATBLS_LoadAllTxts(D2Win_GetArchive(), 0, bIsOpenBnetOrTcp);

    int32_t nState = 3;
    gnCurrentClientState_6FBA8B30 = 3;
    gbExitApplication_6FBA8B70    = TRUE;

    do
    {
        nState = gpfnClientStateHandlers_6FBA8B34[nState]();
        gnCurrentClientState_6FBA8B30 = nState;

        if (nState >= 7)
        {
            ENGINE_FatalHalt(0x298);
        }
        if (nState <= 0)
        {
            ENGINE_FatalHalt(0x299);
        }
    } while (nState != 1);

    D2Win_10016();
    DATATBLS_UnloadAllBins();
    STRTABLE_Destroy();
    ARCHIVE_ShutdownClientArchives();
    LOG_11105();
    CLIENT_CleanupRegistryState();

    if (gbExitApplication_6FBA8B70 != 0)
    {
        gnNextLauncherModuleId_6FBCC42C = 0;
        return 0;
    }

    switch (pConfig->dwGameType)
    {
    case 2:
    case 3:
    case 6:
        gnNextLauncherModuleId_6FBCC42C = 3;
        return 3;
    default:
        gnNextLauncherModuleId_6FBCC42C = 4;
        return 4;
    }
}

// 1.12a: 0x6FAC2630 (custom: esi = nResolutionMode -> __fastcall)
void __fastcall WINMAIN_SetResolutionMode(int32_t nResolutionMode)
{
    if (nResolutionMode == D2GFX_GetResolutionMode())
    {
        return;
    }

    if (nResolutionMode == 0)
    {
        gnScreenWidth_6FB8C6E0  = 640;
        gnScreenHeight_6FB8C6E4 = 480;
        gbIs800x600Mode_6FBCB980 = 0;
    }
    else if (nResolutionMode == 2)
    {
        gnScreenWidth_6FB8C6E0  = 800;
        gnScreenHeight_6FB8C6E4 = 600;
        gbIs800x600Mode_6FBCB980 = 1;
    }

    gnWorldViewportWidth_6FBCD528  = gnScreenWidth_6FB8C6E0;
    gnWorldViewportHeight_6FBCD524 = gnScreenHeight_6FB8C6E4 - 40;
    gnScreenPitch_6FB8C6E8         = gnScreenWidth_6FB8C6E0;

    D2Win_10005(static_cast<D2GameResolutionMode>(nResolutionMode));
    VIEW_ApplyResolutionViewShift(gnViewShiftMode_6FBCC1D0);
    INV_OnResolutionModeChanged(1);
    UI_OnResolutionModeChanged();
}

// 1.12a: 0x6FAC26D0 (custom: esi = nSeconds -> __fastcall)
void __fastcall WINMAIN_SetExitGameCountdown(int32_t nSeconds)
{
    if (nSeconds > 0 && gnExitCountdownEndSyncTime_6FBCC420 == 0)
    {
        gnExitCountdownEndSyncTime_6FBCC420 = FOG_10055_GetSyncTime() + nSeconds;
    }
}

// =============================================================================
// SECTION 4: Quest, Act, Boss, & Ambient Light Fade Client Effects
// VA Range: 0x6FAC26F0 .. 0x6FAC3CBD (38 functions)
// =============================================================================

static void QUESTFX_ZeroAmbientFadeBlock(void)
{
    gbAmbientFadeActive_6FBCB7C4       = 0;
    gnAmbientFadeIntensity_6FBCB7C8    = 0;
    gnAmbientFadeRed_6FBCB7CC          = 0;
    gnAmbientFadeGreen_6FBCB7CD        = 0;
    gnAmbientFadeBlue_6FBCB7CE         = 0;
    gnAmbientFadePad_6FBCB7CF          = 0;
    gnAmbientFadeInFrames_6FBCB7D0     = 0;
    gnAmbientFadeOutFrames_6FBCB7D4    = 0;
    gnAmbientFadeHoldFrames_6FBCB7D8   = 0;
    gnAmbientFadeTotalFrames_6FBCB7DC  = 0;
    gnAmbientFadeCurrentFrame_6FBCB7E0 = 0;
    gnAmbientFadeCosAngle_6FBCB7E4     = 0;
    gnAmbientFadeLevelId_6FBCB7E8      = 0;
    gbAmbientFadeLocked_6FBCB7EC       = 0;
}

// 1.12a: 0x6FAC26F0 (__stdcall)
void __stdcall QUESTFX_ResetSessionState(void)
{
    gnTristramPortalFadeTick_6FBA8AD0 = -1;
    gdwDiabloUnitGUID_6FBA8AD8        = 0xFFFFFFFFu;
    QUESTFX_ZeroAmbientFadeBlock();
    gbTristramPortalOpened_6FBCC418     = 0;
    gbDiabloSealTintActive_6FBCC41C     = 0;
    gnExitCountdownEndSyncTime_6FBCC420 = 0;
    gbNihlathakTauntReady_6FBCC424      = 0;
}

// 1.12a: 0x6FAC2730 (__fastcall: ecx = pOutGreen, edx = pOutBlue, [esp+4] = pOutRed)
int32_t __fastcall QUESTFX_GetChaosSanctuarySealTint(uint8_t* pOutGreen, uint8_t* pOutBlue, uint8_t* pOutRed)
{
    if (gbDiabloSealTintActive_6FBCC41C != 0 && gnDiabloSealFadeTick_6FBA8AD4 == -1)
    {
        *pOutRed   = 0xFFu;
        *pOutGreen = 0x40u;
        *pOutBlue  = 0x30u;
        return 0xA0;
    }
    return 0;
}

// 1.12a: 0x6FAC2760 (custom: edx = pUnit -> __fastcall)
void __fastcall QUESTFX_SpawnRadamentDeathEffect(D2UnitStrc* pUnit)
{
    D2MissileStrc tMissile = {};
    tMissile.pOwner      = pUnit;
    tMissile.pOrigin     = pUnit;
    tMissile.nMissile    = 0x133;
    tMissile.nSkillLevel = 1;
    MISSILE_CreateClientMissile(&tMissile);
}

// 1.12a: 0x6FAC27A0 (custom: esi = pUnit -> __fastcall)
void __fastcall QUESTFX_SpawnAndarielDeathEffect(D2UnitStrc* pUnit)
{
    D2MissileStrc tMissile = {};
    tMissile.pOwner      = pUnit;
    tMissile.pOrigin     = pUnit;
    tMissile.nMissile    = 0x12B;
    tMissile.nSkillLevel = 1;
    D2UnitStrc* const pFireMis = MISSILE_CreateClientMissile(&tMissile);
    if (pFireMis != nullptr)
    {
        MISSILE_SetTargetX(pFireMis, 75);
        MISSILE_SetTargetY(pFireMis, 75);
    }

    std::memset(&tMissile, 0, sizeof(tMissile));
    tMissile.pOwner      = pUnit;
    tMissile.pOrigin     = pUnit;
    tMissile.nMissile    = 0x166;
    tMissile.nSkillLevel = 1;
    MISSILE_CreateClientMissile(&tMissile);

    std::memset(&tMissile, 0, sizeof(tMissile));
    tMissile.pOwner      = pUnit;
    tMissile.pOrigin     = pUnit;
    tMissile.nMissile    = 0x132;
    tMissile.nSkillLevel = 1;
    MISSILE_CreateClientMissile(&tMissile);
}

// 1.12a: 0x6FAC2850 (custom: esi = pUnit -> __fastcall)
void __fastcall QUESTFX_SpawnBloodRavenDeathEffect(D2UnitStrc* pUnit)
{
    D2MissileStrc tMissile = {};
    tMissile.pOwner      = pUnit;
    tMissile.pOrigin     = pUnit;
    tMissile.nMissile    = 0x11C;
    tMissile.nSkillLevel = 1;
    D2UnitStrc* const pFirstMis = MISSILE_CreateClientMissile(&tMissile);
    if (pFirstMis != nullptr)
    {
        MISSILE_SetTargetX(pFirstMis, 50);
    }

    std::memset(&tMissile, 0, sizeof(tMissile));
    tMissile.pOwner      = pUnit;
    tMissile.pOrigin     = pUnit;
    tMissile.nMissile    = 0x128;
    tMissile.nSkillLevel = 1;
    MISSILE_CreateClientMissile(&tMissile);

    tMissile.nMissile = 0x125;
    MISSILE_CreateClientMissile(&tMissile);
}

// 1.12a: 0x6FAC28E0 (__fastcall: ecx = nEventId)
void __fastcall QUESTFX_DispatchQuestClientEvent(int32_t nEventId)
{
    if (nEventId >= 32)
    {
        ENGINE_FatalHalt(0x1D9);
    }
    if (nEventId >= 20)
    {
        ENGINE_FatalHalt(0x1DA);
    }

    gdwTriggeredQuestEventsMask_6FBCC414 |= (1u << nEventId);
    void (__stdcall* const pfnEvent)(void) = gpfnQuestClientEventTable_6FBA8AE0[nEventId];
    if (pfnEvent != nullptr)
    {
        pfnEvent();
    }
}

// 1.12a: 0x6FAC2940 (__stdcall)
void __stdcall QUESTFX_EnableNihlathakTauntFlag(void)
{
    gbNihlathakTauntReady_6FBCC424 = TRUE;
}

// 1.12a: 0x6FAC2950 (__stdcall)
void __stdcall QUESTFX_StartServerShutdownCountdown90s(void)
{
    gnExitCountdownEndSyncTime_6FBCC420 = FOG_10055_GetSyncTime() + 90;
    gnDiabloSealFadeTick_6FBA8AD4       = 0;
}

// 1.12a: 0x6FAC2970 (__stdcall)
void __stdcall QUESTFX_StartCairnStonesPortalSequence(void)
{
    gnTristramPortalFadeTick_6FBA8AD0 = 0;
}

// 1.12a: 0x6FAC2980 (custom: eax = nFadeInFrames, edx = nFadeOutFrames, ebx = nHoldFrames, [esp+4] = nLevelId, [esp+8] = bForceOverride -> __fastcall)
void __fastcall QUESTFX_StartAmbientLightFade(int32_t nFadeInFrames, int32_t nFadeOutFrames, int32_t nHoldFrames, int32_t nLevelId, BOOL bForceOverride)
{
    if (gbAmbientFadeLocked_6FBCB7EC != 0 && !bForceOverride)
    {
        return;
    }

    QUESTFX_ZeroAmbientFadeBlock();
    gbAmbientFadeActive_6FBCB7C4 = TRUE;

    if (nFadeInFrames == 0)
    {
        ENGINE_FatalHalt(0x67);
    }

    gnAmbientFadeInFrames_6FBCB7D0   = nFadeInFrames;
    gnAmbientFadeHoldFrames_6FBCB7D8 = nHoldFrames;
    if (nFadeOutFrames == 0)
    {
        nFadeOutFrames = 25;
    }

    gnAmbientFadeOutFrames_6FBCB7D4   = nFadeOutFrames;
    gnAmbientFadeTotalFrames_6FBCB7DC = nFadeInFrames + nHoldFrames + nFadeOutFrames;
    gnAmbientFadeLevelId_6FBCB7E8     = nLevelId;
}

// 1.12a: 0x6FAC2A00 (__stdcall: [esp+4] = pOutRed, [esp+8] = pOutGreen, [esp+12] = pOutBlue)
int32_t __stdcall QUESTFX_GetTristramPortalTint(uint8_t* pOutRed, uint8_t* pOutGreen, uint8_t* pOutBlue)
{
    if (gbTristramPortalOpened_6FBCC418 != 0)
    {
        return 0;
    }

    if (gnServerWorldStateInit_6FBCBCE0 == 0)
    {
        ENGINE_FatalHalt(0x60);
    }

    if (gabServerWorldFlags_6FBB1678[1] == 0)
    {
        return 0;
    }

    *pOutRed   = 0xFFu;
    *pOutGreen = 0x40u;
    *pOutBlue  = 0x30u;

    const int32_t nTick = gnTristramPortalFadeTick_6FBA8AD0;
    if (nTick == -1)
    {
        return 0x50;
    }

    const int32_t nAngle = (nTick << 7) / 30;
    return static_cast<int32_t>(FOG_10083_Cos_LUT(static_cast<int16_t>(nAngle)) * gfPortalPulseScale_6FB87534);
}

// 1.12a: 0x6FAC2AA0 (__stdcall)
void __stdcall QUESTFX_TriggerShenkDeathEffect(void)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    D2ActiveRoomStrc* const pRoom = UNITS_GetRoom(pPlayer);
    D2UnitStrc* const pShenk = UNITFINDS_FindUnitInNeighboredRooms(pRoom, UNIT_MONSTER, 0x220);
    if (pShenk != nullptr)
    {
        const int32_t nY = ENGINE_GetUnitY(pShenk);
        const int32_t nX = ENGINE_GetUnitX(pShenk);
        MISSILE_SpawnClientEffectAtCoords(0, pPlayer, 0x271, nX, nY, 0, 1);
    }
}

// 1.12a: 0x6FAC2B30 (custom: eax = pUnit -> __fastcall)
void __fastcall QUESTFX_SetTrackedDiabloUnit(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr)
    {
        gdwDiabloUnitGUID_6FBA8AD8 = 0xFFFFFFFFu;
    }
    else
    {
        gdwDiabloUnitGUID_6FBA8AD8 = pUnit->dwUnitId;
    }
}

// 1.12a: 0x6FAC2B50 (custom: eax = pUnit -> __fastcall)
void __fastcall QUESTFX_SpawnRadamentSoulEffect_16B(D2UnitStrc* pUnit)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    const int32_t nY = ENGINE_GetUnitY(pUnit);
    const int32_t nX = ENGINE_GetUnitX(pUnit);
    if (pPlayer != nullptr)
    {
        D2MissileStrc tMissile = {};
        tMissile.dwFlags     = 1;
        tMissile.pOwner      = pPlayer;
        tMissile.nMissile    = 0x16B;
        tMissile.nX          = nX;
        tMissile.nY          = nY;
        tMissile.nSkill      = 0;
        tMissile.nSkillLevel = 1;
        MISSILE_CreateClientMissile(&tMissile);
    }
}

// 1.12a: 0x6FAC2C10 (custom: eax = pUnit -> __fastcall)
void __fastcall QUESTFX_SpawnSummonerDeathEffect_161(D2UnitStrc* pUnit)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    const int32_t nY = ENGINE_GetUnitY(pUnit);
    const int32_t nX = ENGINE_GetUnitX(pUnit);
    if (pPlayer != nullptr)
    {
        D2MissileStrc tMissile = {};
        tMissile.dwFlags     = 1;
        tMissile.pOwner      = pPlayer;
        tMissile.nMissile    = 0x161;
        tMissile.nX          = nX;
        tMissile.nY          = nY;
        tMissile.nSkill      = 0;
        tMissile.nSkillLevel = 1;
        MISSILE_CreateClientMissile(&tMissile);
    }
}

// 1.12a: 0x6FAC2CD0 (custom: eax = pUnit -> __fastcall)
void __fastcall QUESTFX_SpawnTalRashaTombEffect_15B(D2UnitStrc* pUnit)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    const int32_t nY = ENGINE_GetUnitY(pUnit);
    const int32_t nX = ENGINE_GetUnitX(pUnit);
    if (pPlayer != nullptr)
    {
        D2MissileStrc tMissile = {};
        tMissile.dwFlags     = 1;
        tMissile.pOwner      = pPlayer;
        tMissile.nMissile    = 0x15B;
        tMissile.nX          = nX;
        tMissile.nY          = nY;
        tMissile.nSkill      = 0;
        tMissile.nSkillLevel = 1;
        MISSILE_CreateClientMissile(&tMissile);
    }
}

// 1.12a: 0x6FAC2D90 (custom: ebx = pUnit -> __fastcall)
void __fastcall QUESTFX_SpawnMephistoSoulstoneEffect(D2UnitStrc* pUnit)
{
    D2ActiveRoomStrc* const pRoom = UNITS_GetRoom(pUnit);
    D2UnitStrc* const pPortalObj = UNITFINDS_FindUnitInNeighboredRooms(pRoom, UNIT_OBJECT, 0x173);

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (pPortalObj != nullptr)
    {
        nTargetX = ENGINE_GetUnitX(pPortalObj);
        nTargetY = ENGINE_GetUnitY(pPortalObj);
    }
    else
    {
        nTargetX = ENGINE_GetUnitX(pUnit) - 15;
        nTargetY = ENGINE_GetUnitY(pUnit) - 15;
    }

    D2MissileStrc tMissileA = {};
    tMissileA.dwFlags     = 0x420;
    tMissileA.pOwner      = pUnit;
    tMissileA.pOrigin     = pUnit;
    tMissileA.nMissile    = 0x123;
    tMissileA.nTargetX    = nTargetX;
    tMissileA.nTargetY    = nTargetY;
    tMissileA.nSkillLevel = 1;
    MISSILE_CreateClientMissile(&tMissileA);

    if (pUnit != nullptr)
    {
        D2MissileStrc tMissileB = {};
        tMissileB.dwFlags     = 1;
        tMissileB.pOwner      = pUnit;
        tMissileB.nMissile    = 0x14C;
        tMissileB.nX          = nTargetX;
        tMissileB.nY          = nTargetY;
        tMissileB.nSkill      = 0;
        tMissileB.nSkillLevel = 1;
        D2UnitStrc* const pMis = MISSILE_CreateClientMissile(&tMissileB);
        if (pMis != nullptr)
        {
            MISSILE_SetTargetY(pMis, 0);
        }
    }
}

// 1.12a: 0x6FAC2F20 (custom: ebx = pUnit -> __fastcall)
void __fastcall QUESTFX_SpawnAncientsStatueSpiritEffect(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr)
    {
        return;
    }

    int32_t nStatueObjectId = 0;
    switch (pUnit->dwClassId)
    {
    case 0x21C: // Talic
        nStatueObjectId = 0x1DB;
        break;
    case 0x21D: // Madawc
        nStatueObjectId = 0x1DC;
        break;
    case 0x21E: // Korlic
        nStatueObjectId = 0x1DA;
        break;
    default:
        return;
    }

    D2UnitStrc* const pStatue = UNITFINDS_FindUnitInNeighboredRooms(UNITS_GetRoom(pUnit), UNIT_OBJECT, nStatueObjectId);
    if (pStatue == nullptr)
    {
        return;
    }

    D2MissileStrc tMissile = {};
    tMissile.dwFlags     = 0x420;
    tMissile.pOwner      = pUnit;
    tMissile.pOrigin     = pUnit;
    tMissile.nMissile    = 0x21D;
    tMissile.nTargetX    = ENGINE_GetUnitX(pStatue);
    tMissile.nTargetY    = ENGINE_GetUnitY(pStatue);
    tMissile.nSkillLevel = 1;

    D2UnitStrc* const pMis = MISSILE_CreateClientMissile(&tMissile);
    if (pMis != nullptr)
    {
        MISSILE_SetTargetX(pMis, static_cast<int>(pStatue->dwUnitId));
    }
}

// 1.12a: 0x6FAC3020 (__stdcall)
void __stdcall QUESTFX_TriggerDurielWallOpenEffect(void)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        return;
    }

    D2ActiveRoomStrc* const pRoom = UNITS_GetRoom(pPlayer);
    if (pRoom == nullptr)
    {
        return;
    }

    D2UnitStrc* const pWallUnit = UNITFINDS_FindUnitInNeighboredRooms(pRoom, UNIT_MONSTER, 0x20E);
    if (pWallUnit != nullptr)
    {
        const int32_t nX = ENGINE_GetUnitX(pWallUnit);
        const int32_t nY = ENGINE_GetUnitY(pWallUnit);
        MISSILE_SpawnClientEffectAtCoords(0, pPlayer, 0x25E, nX, nY, 0, 1);
        MISSILE_SpawnClientEffectAtCoords(0, pPlayer, 0x25B, nX, nY, 0, 1);
        pWallUnit->dwFlagEx |= 0x00040000u;
    }
}

// 1.12a: 0x6FAC30A0 (__stdcall)
void __stdcall QUESTFX_TriggerTowerTomeEffect(void)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        return;
    }

    D2ActiveRoomStrc* const pRoom = UNITS_GetRoom(pPlayer);
    if (pRoom == nullptr)
    {
        return;
    }

    D2UnitStrc* const pTomeObj = UNITFINDS_FindUnitInNeighboredRooms(pRoom, UNIT_OBJECT, 0x1CC);
    if (pTomeObj != nullptr)
    {
        const int32_t nY = ENGINE_GetUnitY(pTomeObj);
        const int32_t nX = ENGINE_GetUnitX(pTomeObj);
        D2UnitStrc* const pMis = MISSILE_SpawnClientEffectAtCoords(0, pPlayer, 0x210, nX, nY, 0, 1);
        if (pMis != nullptr)
        {
            MISSILE_SetTargetX(pMis, 25);
        }
    }
}

// 1.12a: 0x6FAC3100 (custom: esi = pUnit -> __fastcall)
void __fastcall QUESTFX_SpawnBaalTentacleOrMinionEffect(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr)
    {
        return;
    }

    const int32_t nY = ENGINE_GetUnitY(pUnit);
    const int32_t nX = ENGINE_GetUnitX(pUnit);
    D2UnitStrc* const pMis = MISSILE_SpawnClientEffectAtCoords(0, pUnit, 0x25A, nX, nY, 0, 1);
    if (pMis != nullptr)
    {
        MISSILE_SetTargetX(pMis, 25);
    }
}

// 1.12a: 0x6FAC3180 (__stdcall)
void __stdcall QUESTFX_TriggerHoradricStaffOrificeBeam(void)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        return;
    }

    D2UnitStrc* const pOrifice = UNITFINDS_FindUnitInNeighboredRooms(UNITS_GetRoom(pPlayer), UNIT_OBJECT, 0x95);
    if (pOrifice != nullptr)
    {
        const int32_t nY = ENGINE_GetUnitY(pOrifice);
        const int32_t nX = ENGINE_GetUnitX(pOrifice);
        D2UnitStrc* const pMis = MISSILE_SpawnClientEffectAtCoords(0, pPlayer, 0x15E, nX, nY, 0, 1);
        if (pMis != nullptr)
        {
            MISSILE_SetTargetY(pMis, static_cast<int>(pOrifice->dwUnitId));
        }
    }
}

// 1.12a: 0x6FAC3230 (__stdcall)
void __stdcall QUESTFX_TriggerCountessChestEffect(void)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        return;
    }

    D2ActiveRoomStrc* const pRoom = UNITS_GetRoom(pPlayer);
    if (pRoom == nullptr)
    {
        return;
    }

    D2UnitStrc* const pChest = UNITFINDS_FindUnitInNeighboredRooms(pRoom, UNIT_OBJECT, 0x11);
    if (pChest != nullptr)
    {
        const int32_t nX = ENGINE_GetUnitX(pChest);
        const int32_t nY = ENGINE_GetUnitY(pChest);
        MISSILE_SpawnClientEffectAtCoords(0, pPlayer, 0x120, nX, nY, 0, 1);

        D2MissilesTxt* const pMisTxt = ENGINE_GetMissilesTxtRecord(0x120);
        const int32_t nFadeFrames    = pMisTxt->dwParam[0];
        const int32_t nTotalRange    = static_cast<int16_t>(pMisTxt->wRange);
        const int32_t nHoldFrames    = nTotalRange - (nFadeFrames * 2);
        QUESTFX_StartAmbientLightFade(nFadeFrames, nFadeFrames, nHoldFrames, 4, TRUE);
    }
}

// 1.12a: 0x6FAC32C0 (__stdcall)
void __stdcall QUESTFX_DrawExitCountdownOverlay(void)
{
    const int32_t nNow      = FOG_10055_GetSyncTime();
    const int32_t nDeadline = gnExitCountdownEndSyncTime_6FBCC420;
    if (nDeadline == 0 || nDeadline <= nNow)
    {
        return;
    }

    const uint32_t dwDiabloGUID = gdwDiabloUnitGUID_6FBA8AD8;
    D2UnitStrc* pDiablo = gapServerMonsterHashTable_6FBCAB60[dwDiabloGUID & 0x7Fu];
    while (pDiablo != nullptr && pDiablo->dwUnitId != dwDiabloGUID)
    {
        pDiablo = pDiablo->pListNext;
    }

    if (pDiablo != nullptr)
    {
        if (pDiablo->dwUnitType != UNIT_MONSTER)
        {
            ENGINE_FatalHalt(0x5A);
        }
        if (pDiablo->dwAnimMode != MONMODE_DEAD && (nDeadline - 70) > nNow)
        {
            return;
        }
    }

    Unicode wszCountdown[256] = {};
    const int32_t nSecondsLeft = gnExitCountdownEndSyncTime_6FBCC420 - nNow;
    const Unicode* const wszFmt = D2LANG_GetStringFromTblIndex(0x13E9);
    Unicode::sprintf(256, wszCountdown, wszFmt, nSecondsLeft);

    const Font ePrevFont     = D2Win_10127_SetFont(D2FONT_FONTINGAMECHAT);
    const int32_t nTextWidth = D2Win_10121_GetTextWidth(wszCountdown);
    const int32_t nDrawX     = (gnScreenWidth_6FB8C6E0 / 2) - (nTextWidth / 2);
    D2Win_10117_DrawText(wszCountdown, nDrawX, 375, STRCOLOR_YELLOW, 1);
    D2Win_10127_SetFont(ePrevFont);
}

// 1.12a: 0x6FAC33E0 (custom: eax = pRoom, ebx = pOutRed, edi = pOutGreen, esi = pOutBlue -> __fastcall)
int32_t __fastcall QUESTFX_GetLevelAmbientLightOverride(D2ActiveRoomStrc* pRoom, uint8_t* pOutRed, uint8_t* pOutGreen, uint8_t* pOutBlue)
{
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    if (nLevelId == 8)
    {
        return QUESTFX_GetTristramPortalTint(pOutRed, pOutGreen, pOutBlue);
    }

    if (nLevelId > 0x6A && nLevelId <= 0x6C)
    {
        return QUESTFX_GetChaosSanctuarySealTint(pOutGreen, pOutBlue, pOutRed);
    }

    *pOutRed   = gnAmbientFadeRed_6FBCB7CC;
    *pOutGreen = gnAmbientFadeGreen_6FBCB7CD;
    *pOutBlue  = gnAmbientFadeBlue_6FBCB7CE;
    return gnAmbientFadeIntensity_6FBCB7C8;
}

// 1.12a: 0x6FAC3430 (__stdcall; thunk to QUESTFX_DrawExitCountdownOverlay)
void __stdcall QUESTFX_DrawExitCountdownOverlayThunk(void)
{
    QUESTFX_DrawExitCountdownOverlay();
}

// 1.12a: 0x6FAC3440 (__stdcall)
void __stdcall QUESTFX_UpdateAmbientLightFade(void)
{
    int32_t nBaseIntensity = 0;
    D2ActiveRoomStrc* const pPlayerRoom = (gpClientPlayer_6FBCC3D0 != nullptr) ? UNITS_GetRoom(gpClientPlayer_6FBCC3D0) : nullptr;
    LIGHTMAP_QueryBaseAmbientRGB(
        FALSE,
        pPlayerRoom,
        &nBaseIntensity,
        &gnAmbientFadeRed_6FBCB7CC,
        &gnAmbientFadeGreen_6FBCB7CD,
        &gnAmbientFadeBlue_6FBCB7CE);

    const float fUnsignedIntensity = (nBaseIntensity < 0)
        ? (static_cast<float>(nBaseIntensity) + gfTwoPow32_6FB8751C)
        : static_cast<float>(nBaseIntensity);

    int32_t nCurFrame = gnAmbientFadeCurrentFrame_6FBCB7E0;
    if (nCurFrame < gnAmbientFadeInFrames_6FBCB7D0)
    {
        const float fCos = FOG_10083_Cos_LUT(static_cast<int16_t>(gnAmbientFadeCosAngle_6FBCB7E4));
        gnAmbientFadeIntensity_6FBCB7C8 = static_cast<int32_t>(fCos * fUnsignedIntensity);
        nCurFrame = gnAmbientFadeCurrentFrame_6FBCB7E0;
        gnAmbientFadeCosAngle_6FBCB7E4 = (nCurFrame << 7) / gnAmbientFadeInFrames_6FBCB7D0;
    }
    else if (nCurFrame >= (gnAmbientFadeTotalFrames_6FBCB7DC - gnAmbientFadeOutFrames_6FBCB7D4))
    {
        const float fCos = FOG_10083_Cos_LUT(static_cast<int16_t>(gnAmbientFadeCosAngle_6FBCB7E4));
        gnAmbientFadeIntensity_6FBCB7C8 = static_cast<int32_t>(fCos * fUnsignedIntensity);

        int32_t nFadeOutFrames = gnAmbientFadeOutFrames_6FBCB7D4;
        const int32_t nTotal   = gnAmbientFadeTotalFrames_6FBCB7DC;
        nCurFrame              = gnAmbientFadeCurrentFrame_6FBCB7E0;
        const int32_t nElapsedOut = nFadeOutFrames - nTotal + nCurFrame;
        if (nFadeOutFrames == 0)
        {
            nFadeOutFrames = 25;
            gnAmbientFadeOutFrames_6FBCB7D4 = 25;
        }
        gnAmbientFadeCosAngle_6FBCB7E4 = 128 - ((nElapsedOut << 7) / nFadeOutFrames);
    }
    else
    {
        gnAmbientFadeIntensity_6FBCB7C8 = 0;
    }

    ++nCurFrame;
    gnAmbientFadeCurrentFrame_6FBCB7E0 = nCurFrame;

    D2ActiveRoomStrc* const pRoomAfter = (gpClientPlayer_6FBCC3D0 != nullptr) ? UNITS_GetRoom(gpClientPlayer_6FBCC3D0) : nullptr;
    const int32_t nCurrentLevelId      = (pRoomAfter != nullptr) ? DUNGEON_GetLevelIdFromRoom(pRoomAfter) : 0;

    if (gnAmbientFadeCurrentFrame_6FBCB7E0 > gnAmbientFadeTotalFrames_6FBCB7DC ||
        nCurrentLevelId != gnAmbientFadeLevelId_6FBCB7E8)
    {
        QUESTFX_ZeroAmbientFadeBlock();
    }
}

// 1.12a: 0x6FAC35A0 (custom: eax = pRoom -> __fastcall)
void __fastcall QUESTFX_CheckNihlathakTempleTaunt(D2ActiveRoomStrc* pRoom)
{
    if (pRoom == nullptr)
    {
        return;
    }

    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    if (nLevelId >= 0x83 && nLevelId <= 0x84)
    {
        if (!gbNihlathakTauntPlayed_6FBD36B8 &&
            gbNihlathakTauntReady_6FBCC424 &&
            gpClientPlayer_6FBCC3D0 != nullptr)
        {
            QUEST_TriggerNihlathakTauntSound(0x27F, 0, 1);
            gbNihlathakTauntPlayed_6FBD36B8 = TRUE;
        }
    }
    else
    {
        gbNihlathakTauntPlayed_6FBD36B8 = FALSE;
    }
}

// 1.12a: 0x6FAC3600 (custom: eax = pMissileUnit -> __fastcall)
void __fastcall QUESTFX_InitShenkCatapultBallisticArc(D2UnitStrc* pMissileUnit)
{
    D2UnitStrc* const pMarkerMis = UNITFINDS_FindUnitInNeighboredRooms(UNITS_GetRoom(pMissileUnit), UNIT_MISSILE, 0x271);
    if (pMarkerMis == nullptr)
    {
        return;
    }

    MISSILE_InitShenkCatapultProjectile(pMissileUnit);

    if (pMissileUnit != nullptr && pMissileUnit->pGfxData != nullptr)
    {
        D2ClientBallisticHeightStrc* const pBallistic =
            reinterpret_cast<D2ClientUnitGfxStrc*>(pMissileUnit->pGfxData)->pBallistic;
        if (pBallistic != nullptr)
        {
            pBallistic->nOffsetX = 0;
            pBallistic->nOffsetY = 0;
            pBallistic->nOffsetZ = 0x96000;
            pBallistic->nVelX    = 0;
            pBallistic->nVelY    = 0;
            pBallistic->nVelZ    = -0x1000;
            pBallistic->nAccelX  = 0;
            pBallistic->nAccelY  = 0;
            pBallistic->nAccelZ  = 5;
            pBallistic->dwFlags &= ~0x20u;
        }
    }

    MISSILE_SetTargetY(pMarkerMis, 1);

    D2ActiveRoomStrc* const pPlayerRoom =
        (gpClientPlayer_6FBCC3D0 != nullptr) ? UNITS_GetRoom(gpClientPlayer_6FBCC3D0) : nullptr;
    QUESTFX_CheckNihlathakTempleTaunt(pPlayerRoom);
}

// 1.12a: 0x6FAC36D0 (custom: esi = pUnit -> __fastcall)
void __fastcall QUESTFX_SpawnDiabloDeathEffect(D2UnitStrc* pUnit)
{
    VIEW_StartBossQuakeAndTaunt(0x1253, 0, 250, 0, 0, 0);

    D2MissileStrc tMissile = {};
    tMissile.pOwner      = pUnit;
    tMissile.pOrigin     = pUnit;
    tMissile.nMissile    = 0x17B;
    tMissile.nSkillLevel = 1;
    MISSILE_CreateClientMissile(&tMissile);
}

// 1.12a: 0x6FAC3730 (custom: esi = pUnit -> __fastcall)
void __fastcall QUESTFX_SpawnBaalDeathOrPortalEffect(D2UnitStrc* pUnit)
{
    D2UnitStrc* const pPortalObj = UNITFINDS_FindUnitInNeighboredRooms(UNITS_GetRoom(pUnit), UNIT_OBJECT, 0x182);

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (pPortalObj != nullptr)
    {
        VIEW_StartBossQuakeAndTaunt(0x124F, 0, 350, 0, 0, 0);
        nTargetX = ENGINE_GetUnitX(pPortalObj);
        nTargetY = ENGINE_GetUnitY(pPortalObj);
    }
    else
    {
        nTargetX = ENGINE_GetUnitX(pUnit) - 15;
        nTargetY = ENGINE_GetUnitY(pUnit) - 15;
    }

    D2MissileStrc tMissile = {};
    tMissile.dwFlags     = 0x420;
    tMissile.pOwner      = pUnit;
    tMissile.pOrigin     = pUnit;
    tMissile.nMissile    = 0x170;
    tMissile.nTargetX    = nTargetX;
    tMissile.nTargetY    = nTargetY;
    tMissile.nSkillLevel = 1;

    D2UnitStrc* const pMis = MISSILE_CreateClientMissile(&tMissile);
    if (pMis != nullptr && pPortalObj != nullptr)
    {
        MISSILE_SetTargetX(pMis, static_cast<int>(pPortalObj->dwUnitId));
    }
}

// 1.12a: 0x6FAC3880 (__stdcall)
void __stdcall QUESTFX_TriggerForgeHammerShatterEffect(void)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        return;
    }

    D2ActiveRoomStrc* const pRoom = UNITS_GetRoom(pPlayer);
    if (pRoom == nullptr)
    {
        return;
    }

    D2UnitStrc* const pForge = UNITFINDS_FindUnitInNeighboredRooms(pRoom, UNIT_OBJECT, 0x178);
    if (pForge != nullptr)
    {
        VIEW_StartBossQuakeAndTaunt(0x1251, 0, 350, 0, 0, 0);
        SOUND_PlayClientSoundEx(0x23, pForge, 0, 0, 0);

        const int32_t nX = ENGINE_GetUnitX(pForge);
        const int32_t nY = ENGINE_GetUnitY(pForge);
        D2UnitStrc* const pMis = MISSILE_SpawnClientEffectAtCoords(0, pPlayer, 0x175, nX, nY, 0, 1);
        if (pMis != nullptr)
        {
            MISSILE_SetTargetX(pMis, static_cast<int>(pForge->dwUnitId));
            MISSILE_SetTargetY(pMis, 0);
        }
    }
}

// 1.12a: 0x6FAC3920 (__stdcall)
void __stdcall QUESTFX_TriggerDiabloSealBossAmbient(void)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    gbDiabloSealTintActive_6FBCC41C = TRUE;
    if (pPlayer == nullptr)
    {
        return;
    }

    D2ActiveRoomStrc* const pRoom = UNITS_GetRoom(pPlayer);
    if (pRoom != nullptr && DUNGEON_GetLevelIdFromRoom(pRoom) == 0x6C)
    {
        const int32_t nX = ENGINE_GetUnitX(pPlayer);
        const int32_t nY = ENGINE_GetUnitY(pPlayer);
        MISSILE_SpawnClientEffectAtCoords(0, pPlayer, 0x174, nX, nY, 0, 1);
        SOUND_TriggerBossEventAudio(0xF3, 1);
    }
}

// 1.12a: 0x6FAC3980 (__stdcall)
void __stdcall QUESTFX_TriggerIzualDeathEffect(void)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        return;
    }

    D2ActiveRoomStrc* const pRoom = UNITS_GetRoom(pPlayer);
    if (pRoom == nullptr)
    {
        return;
    }

    D2UnitStrc* const pIzualObj = UNITFINDS_FindUnitInNeighboredRooms(pRoom, UNIT_OBJECT, 0x98);
    if (pIzualObj == nullptr)
    {
        return;
    }

    VIEW_StartBossQuakeAndTaunt(0x1252, 0, 725, 0, 0, 0);

    const int32_t nX = ENGINE_GetUnitX(pIzualObj);
    const int32_t nY = ENGINE_GetUnitY(pIzualObj);
    D2UnitStrc* const pMis = MISSILE_SpawnClientEffectAtCoords(0, pPlayer, 0x152, nX, nY, 0, 1);
    if (pMis != nullptr)
    {
        MISSILE_InitBallisticVelocity(pMis, 40, 0, 0);
        if (pMis->pGfxData != nullptr)
        {
            D2ClientBallisticHeightStrc* const pBallistic =
                reinterpret_cast<D2ClientUnitGfxStrc*>(pMis->pGfxData)->pBallistic;
            if (pBallistic != nullptr)
            {
                pBallistic->nVelX    = 0;
                pBallistic->nVelY    = 0;
                pBallistic->nVelZ    = 0x200;
                pBallistic->nAccelX  = 0;
                pBallistic->nAccelY  = 0;
                pBallistic->nAccelZ  = 70;
                pBallistic->dwFlags |= 0x20u;
            }
        }
    }

    SOUND_PlayPositionalEventSound(0xB5, pIzualObj, 2, 0, 0, 0, 0);
    SOUND_TriggerBossEventAudio(0xD3, 1);
}

// 1.12a: 0x6FAC3A80 (__stdcall: [esp+4] = pRoom)
void __stdcall QUESTFX_SpawnTristramFiresInRoom(D2ActiveRoomStrc* pRoom)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        return;
    }

    D2MissileStrc tMissile = {};
    tMissile.dwFlags     = 0x2003;
    tMissile.pOwner      = pPlayer;
    tMissile.nMissile    = 0x11F;
    tMissile.nSkill      = 0;
    tMissile.nSkillLevel = 1;

    D2DrlgCoordsStrc tCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &tCoords);

    int32_t nSpawned  = 0;
    int32_t nAttempts = 0;
    while (nAttempts < 25)
    {
        const int32_t nRandX = tCoords.nSubtileX + ENGINE_RollRandomNumber(&pPlayer->pSeed, tCoords.nSubtileWidth);
        const int32_t nRandY = tCoords.nSubtileY + ENGINE_RollRandomNumber(&pPlayer->pSeed, tCoords.nSubtileHeight);
        tMissile.nX = nRandX;
        tMissile.nY = nRandY;

        if (COLLISION_CheckMask(pRoom, nRandX, nRandY, 5) == 0)
        {
            MISSILE_CreateClientMissile(&tMissile);
            ++nSpawned;
        }

        ++nAttempts;
        if (nSpawned >= 3)
        {
            break;
        }
    }
}

// 1.12a: 0x6FAC3BE0 (custom: esi = pRoom -> __fastcall)
void __fastcall QUESTFX_OnRoomAddedCheckTristramFires(D2ActiveRoomStrc* pRoom)
{
    if (pRoom == nullptr)
    {
        ENGINE_FatalHalt(0x344);
    }

    if (DUNGEON_GetLevelIdFromRoom(pRoom) == 8 && gbTristramPortalOpened_6FBCC418 != 0)
    {
        QUESTFX_SpawnTristramFiresInRoom(pRoom);
    }
}

// 1.12a: 0x6FAC3C20 (__stdcall)
void __stdcall QUESTFX_SpawnTristramFiresInAllActRooms(void)
{
    for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(gpClientDrlgAct_6FBCC01C);
         pRoom != nullptr;
         pRoom = pRoom->pRoomNext)
    {
        if (DUNGEON_GetLevelIdFromRoom(pRoom) == 8)
        {
            QUESTFX_SpawnTristramFiresInRoom(pRoom);
        }
    }
}

// 1.12a: 0x6FAC3C50 (__stdcall)
void __stdcall QUESTFX_UpdatePortalAndSealFades(void)
{
    if (gbAmbientFadeActive_6FBCB7C4 != 0)
    {
        QUESTFX_UpdateAmbientLightFade();
    }

    if (gbTristramPortalOpened_6FBCC418 == 0 && gnTristramPortalFadeTick_6FBA8AD0 != -1)
    {
        ++gnTristramPortalFadeTick_6FBA8AD0;
        if (gnTristramPortalFadeTick_6FBA8AD0 >= 30)
        {
            gbTristramPortalOpened_6FBCC418 = TRUE;
            QUESTFX_SpawnTristramFiresInAllActRooms();
        }
    }

    if (gbDiabloSealTintActive_6FBCC41C != 0 && gnDiabloSealFadeTick_6FBA8AD4 != -1)
    {
        ++gnDiabloSealFadeTick_6FBA8AD4;
        if (gnDiabloSealFadeTick_6FBA8AD4 >= 30)
        {
            gbDiabloSealTintActive_6FBCC41C = FALSE;
            gnDiabloSealFadeTick_6FBA8AD4   = -1;
        }
    }
}


// =============================================================================
// SECTION 5: Weather, Environment Particles, Rain, Snow, Lightning, & Bubbles
// VA Range: 0x6FAC3CC0 .. 0x6FAC5B4E (41 functions)
// =============================================================================

BOOL __fastcall ENV_InitLevelWeather(BOOL bForceReset);
void __stdcall  ENV_ConfigureAct5SnowParams(void);

// 1.12a: 0x6FAC3CC0 (__stdcall)
void __stdcall ENV_Noop_6FAC3CC0(void)
{
}

// 1.12a: 0x6FAC3CD0 (__stdcall)
void __stdcall ENV_Noop_6FAC3CD0(void)
{
}

// 1.12a: 0x6FAC3CE0 (__stdcall)
void __stdcall ENV_Noop_6FAC3CE0(void)
{
}

// 1.12a: 0x6FAC3CF0 (__stdcall)
void __stdcall ENV_Noop_6FAC3CF0(void)
{
}

// 1.12a: 0x6FAC3D00 (__stdcall)
void __stdcall ENV_Noop_6FAC3D00(void)
{
}

// 1.12a: 0x6FAC3D10 (__stdcall)
void __stdcall ENV_Noop_6FAC3D10(void)
{
}

// 1.12a: 0x6FAC3D20 (custom: eax = nSetting -> __fastcall)
void __fastcall ENV_SetUnusedSetting(int32_t nSetting)
{
    gnEnvUnusedSetting_6FBCC3F4 = nSetting;
}

// 1.12a: 0x6FAC3D30 (__stdcall)
float __stdcall ENV_GetCurrentRainIntensity(void)
{
    return gfCurrentRainIntensity_6FBCB768;
}

// 1.12a: 0x6FAC3D40 (__stdcall)
void __stdcall ENV_UpdateSwampBubbles(void)
{
    const BOOL bPerspective = D2GFX_CheckPerspective();
    D2FogDataListStrc* pPool = gpEnvBubblesPool_6FBCC3E4;
    D2EnvOverlayParticleStrc* const pElements = static_cast<D2EnvOverlayParticleStrc*>(pPool->pElements);

    for (int32_t i = 0; i <= pPool->nMaxAllocatedIdx; ++i)
    {
        D2EnvOverlayParticleStrc& rBubble = pElements[i];
        if (rBubble.wSlotSeq != 0)
        {
            if (!bPerspective)
            {
                rBubble.nX -= gnCameraScrollDeltaX_6FBCC06C;
                rBubble.nY -= gnCameraScrollDeltaY_6FBCC070;
            }

            --rBubble.nFrameTicks;
            if (rBubble.nFrameTicks <= 0)
            {
                ++rBubble.nFrame;
                rBubble.nFrameTicks = 2;
                if (rBubble.nFrame >= ganBubbleCelFrameCounts_6FBCB6F4[rBubble.nCellVariant])
                {
                    FOG_10065_DataListFreeHandle(pPool, i << 16);
                    pPool = gpEnvBubblesPool_6FBCC3E4;
                }
            }
        }
    }
}

// 1.12a: 0x6FAC3DD0 (__stdcall)
void __stdcall ENV_UpdateRainSplashes(void)
{
    const BOOL bPerspective = D2GFX_CheckPerspective();
    D2FogDataListStrc* pPool = gpEnvSplashesPool_6FBCC3E0;
    D2EnvOverlayParticleStrc* const pElements = static_cast<D2EnvOverlayParticleStrc*>(pPool->pElements);

    for (int32_t i = 0; i <= pPool->nMaxAllocatedIdx; ++i)
    {
        D2EnvOverlayParticleStrc& rSplash = pElements[i];
        if (rSplash.wSlotSeq != 0)
        {
            if (!bPerspective)
            {
                rSplash.nX -= gnCameraScrollDeltaX_6FBCC06C;
                rSplash.nY -= gnCameraScrollDeltaY_6FBCC070;
            }

            --rSplash.nFrameTicks;
            if (rSplash.nFrameTicks <= 0)
            {
                ++rSplash.nFrame;
                rSplash.nFrameTicks = 2;
                if (rSplash.nFrame >= ganRainSplashFrameCounts_6FBCB790[rSplash.nCellVariant])
                {
                    FOG_10065_DataListFreeHandle(pPool, i << 16);
                    pPool = gpEnvSplashesPool_6FBCC3E0;
                }
            }
        }
    }
}

// 1.12a: 0x6FAC3E60 (__stdcall: [esp+4] = bPlayThunder)
void __stdcall ENV_TriggerScriptedLightning(BOOL bPlayThunder)
{
    if (gbLightningEnabled_6FBCC3EC == 0)
    {
        gbLightningEnabled_6FBCC3EC        = TRUE;
        gnLightningPendingTrigger_6FBCB7B0 = 1;
        gbLightningDoubleFlash_6FBCB734    = FALSE;
        gbLightningPlayThunder_6FBA8ACC    = bPlayThunder;
    }
}

// 1.12a: 0x6FAC3E90 (__stdcall)
void __stdcall ENV_ResetParticlesPool(void)
{
    FOG_10063_DataListReset(gpEnvParticlesPool_6FBCC3E8);
    if (gpEnvParticlesPool_6FBCC3E8->nActiveCount != 0)
    {
        ENGINE_FatalHalt(0x15B);
    }
}

// 1.12a: 0x6FAC3ED0 (__stdcall)
int32_t __stdcall ENV_GetWeatherCyclePhase(void)
{
    return gnWeatherCyclePhase_6FBCC408;
}

// 1.12a: 0x6FAC3EE0 (__stdcall)
void __stdcall ENV_DestroyAllPools(void)
{
    FOG_10061_DataListDestroy(gpEnvParticlesPool_6FBCC3E8);
    FOG_10061_DataListDestroy(gpEnvSplashesPool_6FBCC3E0);
    FOG_10061_DataListDestroy(gpEnvBubblesPool_6FBCC3E4);
}

// 1.12a: 0x6FAC3F10 (__stdcall)
void __stdcall ENV_CreateAllPools(void)
{
    gpEnvParticlesPool_6FBCC3E8 = FOG_10060_DataListCreate(gszEnvParticles_6FB86FA4, 256, sizeof(D2EnvParticleStrc));
    FOG_10062_DataListInit(gpEnvParticlesPool_6FBCC3E8);
    gnCurrentTargetParticles_6FBCB7A8 = 0;

    gpEnvSplashesPool_6FBCC3E0 = FOG_10060_DataListCreate(gszEnvSplashes_6FB86F8C, 512, sizeof(D2EnvOverlayParticleStrc));
    FOG_10062_DataListInit(gpEnvSplashesPool_6FBCC3E0);

    gpEnvBubblesPool_6FBCC3E4 = FOG_10060_DataListCreate(gszEnvBubbles_6FB86F78, 128, sizeof(D2EnvOverlayParticleStrc));
    FOG_10062_DataListInit(gpEnvBubblesPool_6FBCC3E4);

    gnWindAngleMinRain_6FBCB7A4   = 42;
    gnWindAngleMinSnow_6FBCB7A0   = 42;
    gbLightningDoubleFlash_6FBCB734 = 0;
    gbLightningEnabled_6FBCC3EC     = 0;
    gdwLastEnvUpdateFrame_6FBCC3F0  = 0;
    gnWindAngleRangeRain_6FBCB764 = 170;
    gnWindAngleRangeSnow_6FBCB760 = 170;
}

// 1.12a: 0x6FAC3FB0 (__stdcall: [esp+4] = pPool, [esp+8] = ppCellFiles)
void __stdcall ENV_DrawOverlayParticlePool(D2FogDataListStrc* pPool, D2CellFileStrc* const* ppCellFiles)
{
    using CelDrawBoolFn = BOOL (__stdcall*)(D2GfxDataStrc*, int32_t, int32_t, uint32_t, DrawMode, uint8_t*);
    const CelDrawBoolFn pfnCelDraw = reinterpret_cast<CelDrawBoolFn>(TEXTURE_CelDraw);

    const BOOL bPerspective = D2GFX_CheckPerspective();
    const int32_t nClipMaxY = gnScreenHeight_6FB8C6E4 - 47;

    int32_t nClipMinX    = 0;
    int32_t nClipMaxX    = gnScreenWidth_6FB8C6E0;
    int32_t nPanelShiftX = 0;

    if (gnViewShiftMode_6FBCC1D0 == 1)
    {
        const int32_t nQuarter = gnScreenWidth_6FB8C6E0 / 4;
        nPanelShiftX = -nQuarter;
        nClipMaxX    = gnScreenWidth_6FB8C6E0 - (nQuarter * 2);
    }
    else if (gnViewShiftMode_6FBCC1D0 == 2)
    {
        const int32_t nQuarter = gnScreenWidth_6FB8C6E0 / 4;
        nPanelShiftX = nQuarter;
        nClipMinX    = nQuarter * 2;
    }

    D2GfxDataStrc tGfxData = {};
    D2EnvOverlayParticleStrc* const pElements = static_cast<D2EnvOverlayParticleStrc*>(pPool->pElements);

    for (int32_t i = 0; i <= pPool->nMaxAllocatedIdx; ++i)
    {
        const D2EnvOverlayParticleStrc& rElem = pElements[i];
        if (rElem.wSlotSeq == 0)
        {
            continue;
        }

        if (bPerspective)
        {
            BOOL bKeepAlive = FALSE;
            if (SCALE_SetPerspectiveScale(rElem.nX, rElem.nY))
            {
                int32_t nScreenX = 0;
                int32_t nScreenY = 0;
                SCALE_AdjustPerspectivePosition(rElem.nX, rElem.nY, 0, &nScreenX, &nScreenY);
                nScreenX += nPanelShiftX;

                if (nScreenX >= nClipMinX && nScreenX < nClipMaxX && nScreenY >= 0 && nScreenY < nClipMaxY)
                {
                    tGfxData.pCellFile = ppCellFiles[rElem.nCellVariant];
                    tGfxData.nFrame    = static_cast<uint32_t>(rElem.nFrame);
                    if (pfnCelDraw(&tGfxData, nScreenX, nScreenY, 0xFFFFFFFFu, DRAWMODE_MODULATE, nullptr))
                    {
                        bKeepAlive = TRUE;
                    }
                }
            }

            if (!bKeepAlive)
            {
                FOG_10065_DataListFreeHandle(pPool, i << 16);
            }
        }
        else
        {
            const int32_t nScreenX = rElem.nX + nPanelShiftX;
            const int32_t nScreenY = rElem.nY;
            if (nScreenX >= nClipMinX && nScreenX < nClipMaxX && nScreenY >= 0 && nScreenY < nClipMaxY)
            {
                tGfxData.pCellFile = ppCellFiles[rElem.nCellVariant];
                tGfxData.nFrame    = static_cast<uint32_t>(rElem.nFrame);
                TEXTURE_CelDraw(&tGfxData, nScreenX, nScreenY, 0xFFFFFFFFu, DRAWMODE_MODULATE, nullptr);
            }
        }
    }
}

// 1.12a: 0x6FAC4150 (custom: ebx = pbOutRain, edi = pbOutMud -> __fastcall)
void __fastcall ENV_QueryCurrentLevelRainAndMud(BOOL* pbOutRain, BOOL* pbOutMud)
{
    *pbOutRain = FALSE;
    *pbOutMud  = FALSE;

    if (gpClientPlayer_6FBCC3D0 != nullptr)
    {
        const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(gpClientPlayer_6FBCC3D0));
        *pbOutRain = (DATATBLS_GetRainFromLevelsTxt(nLevelId) != 0) ? TRUE : FALSE;
        *pbOutMud  = (DATATBLS_GetMudFromLevelsTxt(nLevelId) != 0) ? TRUE : FALSE;
    }
}

// 1.12a: 0x6FAC41A0 (__stdcall)
void __stdcall ENV_DrawLightningScreenFlash(void)
{
    int32_t nClipMinX = 0;
    int32_t nClipMaxX = gnScreenWidth_6FB8C6E0;
    const int32_t nClipMaxY = gnScreenHeight_6FB8C6E4 - 47;

    if (gnViewShiftMode_6FBCC1D0 == 1)
    {
        const int32_t nQuarter = gnScreenWidth_6FB8C6E0 / 4;
        nClipMaxX = gnScreenWidth_6FB8C6E0 - (nQuarter * 2);
    }
    else if (gnViewShiftMode_6FBCC1D0 == 2)
    {
        const int32_t nQuarter = gnScreenWidth_6FB8C6E0 / 4;
        nClipMinX = nQuarter * 2;
    }

    D2GFX_DrawBox(nClipMinX, 0, nClipMaxX, nClipMaxY, 0xFFu, DRAWMODE_NORMAL);
}

// 1.12a: 0x6FAC4210 (__stdcall)
void __stdcall ENV_DrawWeatherParticles(void)
{
    if (D2GFX_CheckLowQuality())
    {
        return;
    }

    int32_t nClipMinX = 0;
    int32_t nClipMaxX = gnScreenWidth_6FB8C6E0;
    const int32_t nClipMaxY = gnScreenHeight_6FB8C6E4 - 47;

    if (gnViewShiftMode_6FBCC1D0 == 1)
    {
        const int32_t nQuarter = gnScreenWidth_6FB8C6E0 / 4;
        nClipMaxX = gnScreenWidth_6FB8C6E0 - (nQuarter * 2);
    }
    else if (gnViewShiftMode_6FBCC1D0 == 2)
    {
        const int32_t nQuarter = gnScreenWidth_6FB8C6E0 / 4;
        nClipMinX = nQuarter * 2;
    }

    D2FogDataListStrc* const pPool = gpEnvParticlesPool_6FBCC3E8;
    const D2EnvParticleStrc* const pElements = static_cast<const D2EnvParticleStrc*>(pPool->pElements);

    for (int32_t i = 0; i <= pPool->nMaxAllocatedIdx; ++i)
    {
        const D2EnvParticleStrc& rPart = pElements[i];
        if (rPart.wSlotSeq == 0)
        {
            continue;
        }

        if (gbIsSnowing_6FBCC3F8 != 0)
        {
            if (rPart.nX >= nClipMinX && rPart.nX < nClipMaxX &&
                rPart.nY >= 0 && rPart.nY < nClipMaxY &&
                rPart.nLengthOrSize >= 0 && rPart.nLengthOrSize < 8)
            {
                const int32_t (&rLineA)[4] = gSnowflakeLines_6FB86E10[rPart.nLengthOrSize];
                D2GFX_DrawLine(
                    rPart.nX + rLineA[0],
                    rPart.nY + rLineA[1],
                    rPart.nX + rLineA[2],
                    rPart.nY + rLineA[3],
                    rPart.nColor,
                    rPart.nAlpha);

                const int32_t (&rLineB)[4] = gSnowflakeLines_6FB86E10[rPart.nLengthOrSize + 1];
                D2GFX_DrawLine(
                    rPart.nX + rLineB[0],
                    rPart.nY + rLineB[1],
                    rPart.nX + rLineB[2],
                    rPart.nY + rLineB[3],
                    rPart.nColor,
                    rPart.nAlpha);
            }
        }
        else if (rPart.bLanded != 0)
        {
            if (rPart.nX >= nClipMinX && rPart.nX < nClipMaxX &&
                rPart.nY >= 0 && rPart.nY < nClipMaxY)
            {
                D2GFX_DrawLine(rPart.nX, rPart.nY, rPart.nX, rPart.nY, rPart.nColor, rPart.nAlpha);
            }
        }
        else
        {
            int32_t nDeltaX = static_cast<int32_t>(FOG_10083_Cos_LUT(static_cast<int16_t>(gnCurrentWindAngle_6FBCB78C)) * static_cast<float>(rPart.nLengthOrSize));
            int32_t nDeltaY = static_cast<int32_t>(FOG_10084_Sin_LUT(static_cast<int16_t>(gnCurrentWindAngle_6FBCB78C)) * static_cast<float>(rPart.nLengthOrSize));

            const int32_t nRemY = rPart.nEndY - rPart.nY;
            if (nDeltaY > nRemY)
            {
                nDeltaX = (nRemY * nDeltaX) / nDeltaY;
                nDeltaY = nRemY;
            }

            const int32_t nEndX = rPart.nX + nDeltaX;
            const int32_t nEndY = rPart.nY + nDeltaY;

            const bool bStartVisible = (rPart.nX >= nClipMinX && rPart.nX < nClipMaxX && rPart.nY >= 0 && rPart.nY < nClipMaxY);
            const bool bEndVisible   = (nEndX >= nClipMinX && nEndX < nClipMaxX && nEndY >= 0 && nEndY < nClipMaxY);

            if (bStartVisible || bEndVisible)
            {
                D2GFX_DrawLine(rPart.nX, rPart.nY, nEndX, nEndY, rPart.nColor, rPart.nAlpha);
            }
        }
    }
}

// 1.12a: 0x6FAC4460 (__stdcall)
void __stdcall ENV_FreeOverlayCellFiles(void)
{
    for (int32_t i = 0; i < 4; ++i)
    {
        D2CellFileStrc* const pRainCell = gapRainSplashCellFiles_6FBCB778[i];
        if (pRainCell != nullptr)
        {
            D2CMP_CelFileFreeHardware(pRainCell);
            FOG_Free(pRainCell, gszArchiveCpp_6FB807A0, 0x7A, 0);
            gapRainSplashCellFiles_6FBCB778[i] = nullptr;
        }
    }

    for (int32_t i = 0; i < 4; ++i)
    {
        D2CellFileStrc* const pBubbleCell = gapBubbleCellFiles_6FBCB704[i];
        if (pBubbleCell != nullptr)
        {
            D2CMP_CelFileFreeHardware(pBubbleCell);
            FOG_Free(pBubbleCell, gszArchiveCpp_6FB807A0, 0x7A, 0);
            gapBubbleCellFiles_6FBCB704[i] = nullptr;
        }
    }
}

// 1.12a: 0x6FAC44E0 (__stdcall)
BOOL __stdcall ENV_CanSpawnRainSplashes(void)
{
    if (gbIsSnowing_6FBCC3F8 != 0)
    {
        return FALSE;
    }

    BOOL bRain = FALSE;
    if (gpClientPlayer_6FBCC3D0 != nullptr)
    {
        const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(gpClientPlayer_6FBCC3D0));
        bRain = (DATATBLS_GetRainFromLevelsTxt(nLevelId) != 0) ? TRUE : FALSE;
        DATATBLS_GetMudFromLevelsTxt(nLevelId);
    }
    return bRain;
}

// 1.12a: 0x6FAC4520 (__stdcall)
void __stdcall ENV_DrawSplashesAndBubbles(void)
{
    if (gpEnvSplashesPool_6FBCC3E0->nActiveCount != 0)
    {
        ENV_DrawOverlayParticlePool(gpEnvSplashesPool_6FBCC3E0, gapRainSplashCellFiles_6FBCB778);
    }
    if (gpEnvBubblesPool_6FBCC3E4->nActiveCount != 0)
    {
        ENV_DrawOverlayParticlePool(gpEnvBubblesPool_6FBCC3E4, gapBubbleCellFiles_6FBCB704);
    }
}

// 1.12a: 0x6FAC4560 (__stdcall)
BOOL __stdcall ENV_CanSpawnSwampBubbles(void)
{
    if (gpClientPlayer_6FBCC3D0 == nullptr)
    {
        return FALSE;
    }

    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(gpClientPlayer_6FBCC3D0));
    DATATBLS_GetRainFromLevelsTxt(nLevelId);
    return (DATATBLS_GetMudFromLevelsTxt(nLevelId) != 0) ? TRUE : FALSE;
}

// 1.12a: 0x6FAC4590 (custom: edi = nSize, ebx = pOutAlpha, [esp+4] = pOutColor -> __fastcall)
void __fastcall ENV_ComputeSnowflakeColorAndAlpha(int32_t nSize, uint8_t* pOutAlpha, uint8_t* pOutColor)
{
    const DisplayType eDispType = D2GFX_GetDisplayType();
    const BOOL bUseBrightTable = ((eDispType > 0 && eDispType <= 3) || eDispType == 6) ? FALSE : TRUE;

    D2ActiveRoomStrc* const pRoom = (gpClientPlayer_6FBCC3D0 != nullptr) ? UNITS_GetRoom(gpClientPlayer_6FBCC3D0) : nullptr;
    DUNGEON_GetLevelIdFromRoom(pRoom);

    const int32_t nPeriod = ENVIRONMENT_GetPeriodOfDayFromAct(gpClientDrlgAct_6FBCC01C, nullptr);
    if (nPeriod < 0 || nPeriod >= 4)
    {
        ENGINE_FatalHalt(0x356);
    }

    if (pOutAlpha != nullptr)
    {
        *pOutAlpha = gSnowPeriodAlphaTable_6FB86EA0[nPeriod * 2 + 1];
    }

    if (pOutColor != nullptr)
    {
        const int32_t nBaseIdx = (nSize * 12) / 8;
        if (bUseBrightTable)
        {
            *pOutColor = gabSnowColorBright_6FBCB7B4[nBaseIdx];
            return;
        }

        if (nPeriod == 1 || nPeriod == 3)
        {
            *pOutColor = gabSnowColorNormal_6FBCB754[nBaseIdx / 2];
        }
        else if (nPeriod == 2)
        {
            *pOutColor = gabSnowColorNormal_6FBCB754[nBaseIdx / 4];
        }
        else
        {
            *pOutColor = gabSnowColorNormal_6FBCB754[nBaseIdx];
        }
    }
}

// 1.12a: 0x6FAC4670 (__stdcall)
void __stdcall ENV_RenderWeatherOverlayAndLightning(void)
{
    D2UnitStrc* pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        ENGINE_FatalHalt(0x573);
    }

    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pPlayer));
    const BOOL bRain       = (DATATBLS_GetRainFromLevelsTxt(nLevelId) != 0) ? TRUE : FALSE;
    DATATBLS_GetMudFromLevelsTxt(nLevelId);

    pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        ENGINE_FatalHalt(0x573);
    }

    D2SeedStrc* const pSeed = &pPlayer->pSeed;
    if (gbLightningEnabled_6FBCC3EC != 0 && gnLightningPendingTrigger_6FBCB7B0 != 0)
    {
        --gnLightningPendingTrigger_6FBCB7B0;
        if (gnLightningPendingTrigger_6FBCB7B0 == 0)
        {
            if (gbLightningDoubleFlash_6FBCB734 == 0)
            {
                gnLightningTimerFrames_6FBCB730 = 3;
                gbLightningDoubleFlash_6FBCB734 = TRUE;
            }
            else
            {
                gnLightningTimerFrames_6FBCB730 = ENGINE_RollRandomRange(pSeed, 500, 1500);
                gbLightningDoubleFlash_6FBCB734 = FALSE;

                if (gbLightningPlayThunder_6FBA8ACC != 0)
                {
                    const int32_t nDelay = ENGINE_RollRandomRange(pSeed, 25, 50);
                    void* const pChannel = SOUND_PlayClientSoundEx(0xCA, nullptr, nDelay, 0, 0);
                    if (pChannel != nullptr)
                    {
                        const int32_t nOffY = ENGINE_RollRandomRange(pSeed, -200, 400);
                        const int32_t nOffX = ENGINE_RollRandomRange(pSeed, -200, 400);
                        SOUND_SetChannelPositionOffset(pChannel, nOffX, nOffY, 0);
                    }
                }
                else
                {
                    gbLightningPlayThunder_6FBA8ACC = TRUE;
                }
            }
        }

        if (gdwLightningFlashAlpha_6FBCC388 >= 10u)
        {
            ENV_DrawLightningScreenFlash();
            return;
        }
    }

    if (bRain && gpEnvParticlesPool_6FBCC3E8->nActiveCount != 0)
    {
        ENV_DrawWeatherParticles();
    }
}

// 1.12a: 0x6FAC47D0 (__stdcall: [esp+4] = pSeed)
void __stdcall ENV_SpawnWeatherParticle(D2SeedStrc* pSeed)
{
    const int32_t nHandle = FOG_10064_DataListAllocHandle(gpEnvParticlesPool_6FBCC3E8);
    if (nHandle == -1)
    {
        return;
    }

    const int32_t nSlotIdx = static_cast<int16_t>(nHandle >> 16);
    D2EnvParticleStrc* const pPart =
        &static_cast<D2EnvParticleStrc*>(gpEnvParticlesPool_6FBCC3E8->pElements)[nSlotIdx];

    pPart->bLanded = 0;
    pPart->nX      = ENGINE_RollRandomRange(pSeed, 0, gnScreenWidth_6FB8C6E0);

    const int32_t nEndYRange = gnScreenHeight_6FB8C6E4 - 87;
    pPart->nEndY = ENGINE_RollRandomRange(pSeed, 40, nEndYRange);
    pPart->nY    = ENGINE_RollRandomRange(pSeed, -20, pPart->nEndY + 20);

    const uint64_t uPhaseStep = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
    pSeed->nLowSeed  = static_cast<uint32_t>(uPhaseStep);
    pSeed->nHighSeed = static_cast<uint32_t>(uPhaseStep >> 32);
    pPart->nPhase    = static_cast<int32_t>(pSeed->nLowSeed & 0x1FFu);

    const float fDepthRatio = static_cast<float>(pPart->nEndY - 40) / static_cast<float>(nEndYRange);

    if (gbIsSnowing_6FBCC3F8 != 0)
    {
        pPart->nAlpha = 0xFFu;

        const uint64_t uSnowStep = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
        pSeed->nLowSeed  = static_cast<uint32_t>(uSnowStep);
        pSeed->nHighSeed = static_cast<uint32_t>(uSnowStep >> 32);

        pPart->nMeltFrames = 1;
        pPart->nSpeed      = 8 - static_cast<int32_t>(fDepthRatio * gfSnowSpeedScale_6FB87580);

        int32_t nSnowSize = ((pPart->nEndY - 40) * 7) / nEndYRange;
        if (gbLargeSnowflakes_6FBCC3FC != 0)
        {
            ++nSnowSize;
        }
        pPart->nLengthOrSize = nSnowSize;

        if (nSnowSize < 0 || nSnowSize >= 8)
        {
            ENGINE_FatalHalt(0x390);
        }

        ENV_ComputeSnowflakeColorAndAlpha(nSnowSize, &pPart->nAlpha, &pPart->nColor);
    }
    else
    {
        const uint64_t uRainStep = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
        pSeed->nLowSeed  = static_cast<uint32_t>(uRainStep);
        pSeed->nHighSeed = static_cast<uint32_t>(uRainStep >> 32);
        const uint32_t uColorIdx = pSeed->nLowSeed % 12u;

        const int32_t nPeriod = ENVIRONMENT_GetPeriodOfDayFromAct(gpClientDrlgAct_6FBCC01C, nullptr);
        switch (nPeriod)
        {
        case 0:
            pPart->nColor = gabRainColorDay_6FBCB748[uColorIdx];
            pPart->nAlpha = 0x7Fu;
            break;
        case 1:
        case 3:
            pPart->nColor = gabRainColorDawnDusk_6FBCB714[uColorIdx];
            pPart->nAlpha = 0xFFu;
            break;
        case 2:
            pPart->nColor = gabRainColorNight_6FBCB76C[uColorIdx];
            pPart->nAlpha = 0xFFu;
            break;
        default:
            break;
        }

        pPart->nLengthOrSize = 4 - static_cast<int32_t>(fDepthRatio * gfRainLengthScale_6FB8757C);
        pPart->nSpeed        = 15 - static_cast<int32_t>(fDepthRatio * gfRainSpeedScale_6FB87578);
        pPart->nMeltFrames   = 3;
    }
}

// 1.12a: 0x6FAC4A10 (__stdcall: [esp+4..16] = nScreenX, nScreenY, nPerspWorldX, nPerspWorldY)
void __stdcall ENV_SpawnSwampBubble(int32_t nScreenX, int32_t nScreenY, int32_t nPerspWorldX, int32_t nPerspWorldY)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        ENGINE_FatalHalt(0x2FC);
    }

    D2SeedStrc* const pSeed = &pPlayer->pSeed;
    const int32_t nHandle   = FOG_10064_DataListAllocHandle(gpEnvBubblesPool_6FBCC3E4);
    if (nHandle == -1)
    {
        return;
    }

    const int32_t nSlotIdx = static_cast<int16_t>(nHandle >> 16);
    D2EnvOverlayParticleStrc* const pBubble =
        &static_cast<D2EnvOverlayParticleStrc*>(gpEnvBubblesPool_6FBCC3E4->pElements)[nSlotIdx];

    if (D2GFX_CheckPerspective())
    {
        const uint64_t uStepX = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
        pSeed->nLowSeed  = static_cast<uint32_t>(uStepX);
        pSeed->nHighSeed = static_cast<uint32_t>(uStepX >> 32);
        const int32_t nJitterX = static_cast<int32_t>(pSeed->nLowSeed % 5u) - 2;
        pBubble->nX = nPerspWorldX + (nJitterX << 16);

        const uint64_t uStepY = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
        pSeed->nLowSeed  = static_cast<uint32_t>(uStepY);
        pSeed->nHighSeed = static_cast<uint32_t>(uStepY >> 32);
        const int32_t nJitterY = static_cast<int32_t>(pSeed->nLowSeed % 5u) - 2;
        pBubble->nY = nPerspWorldY + (nJitterY << 16);
    }
    else
    {
        const uint64_t uStepY = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
        pSeed->nLowSeed  = static_cast<uint32_t>(uStepY);
        pSeed->nHighSeed = static_cast<uint32_t>(uStepY >> 32);
        const int32_t nRandY = static_cast<int32_t>(pSeed->nLowSeed % 80u);

        int32_t nDistFromMid = nRandY - 40;
        if (nDistFromMid < 0)
        {
            nDistFromMid = -nDistFromMid;
        }
        const int32_t nHalfSpanX = 80 - (nDistFromMid * 2);
        pBubble->nX = nScreenX + ENGINE_RollRandomRange(pSeed, -nHalfSpanX, nHalfSpanX * 2);
        pBubble->nY = nScreenY + nRandY - 30;
    }

    pBubble->nCellVariant = 2;
    pBubble->nFrame       = 0;
    pBubble->nFrameTicks  = 2;
}

// 1.12a: 0x6FAC4B40 (__stdcall: [esp+4..16] = nScreenX, nScreenY, nPerspWorldX, nPerspWorldY)
void __stdcall ENV_SpawnRainSplash(int32_t nScreenX, int32_t nScreenY, int32_t nPerspWorldX, int32_t nPerspWorldY)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        ENGINE_FatalHalt(0x2D2);
    }

    D2SeedStrc* const pSeed = &pPlayer->pSeed;
    const int32_t nHandle   = FOG_10064_DataListAllocHandle(gpEnvSplashesPool_6FBCC3E0);
    if (nHandle == -1)
    {
        return;
    }

    const int32_t nSlotIdx = static_cast<int16_t>(nHandle >> 16);
    D2EnvOverlayParticleStrc* const pSplash =
        &static_cast<D2EnvOverlayParticleStrc*>(gpEnvSplashesPool_6FBCC3E0->pElements)[nSlotIdx];

    if (D2GFX_CheckPerspective())
    {
        const uint64_t uStepX = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
        pSeed->nLowSeed  = static_cast<uint32_t>(uStepX);
        pSeed->nHighSeed = static_cast<uint32_t>(uStepX >> 32);
        const int32_t nJitterX = static_cast<int32_t>(pSeed->nLowSeed % 5u);
        pSplash->nX = nPerspWorldX + (nJitterX << 16);

        const uint64_t uStepY = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
        pSeed->nLowSeed  = static_cast<uint32_t>(uStepY);
        pSeed->nHighSeed = static_cast<uint32_t>(uStepY >> 32);
        const int32_t nJitterY = static_cast<int32_t>(pSeed->nLowSeed % 5u);
        pSplash->nY = nPerspWorldY + (nJitterY << 16);
    }
    else
    {
        const uint64_t uStepY = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
        pSeed->nLowSeed  = static_cast<uint32_t>(uStepY);
        pSeed->nHighSeed = static_cast<uint32_t>(uStepY >> 32);
        const int32_t nRandY = static_cast<int32_t>(pSeed->nLowSeed % 80u);

        int32_t nDistFromMid = nRandY - 40;
        if (nDistFromMid < 0)
        {
            nDistFromMid = -nDistFromMid;
        }
        const int32_t nHalfSpanX = 80 - (nDistFromMid * 2);
        pSplash->nX = nScreenX + ENGINE_RollRandomRange(pSeed, -nHalfSpanX, nHalfSpanX * 2);
        pSplash->nY = nScreenY + nRandY;
    }

    const uint64_t uStepVar = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
    pSeed->nLowSeed  = static_cast<uint32_t>(uStepVar);
    pSeed->nHighSeed = static_cast<uint32_t>(uStepVar >> 32);

    const int32_t nVariant = static_cast<int32_t>(pSeed->nLowSeed & 1u) + 2;
    pSplash->nCellVariant  = nVariant;
    if (nVariant < 0 || nVariant >= 4)
    {
        ENGINE_FatalHalt(0x2EA);
    }

    pSplash->nFrame      = 0;
    pSplash->nFrameTicks = 2;
}

// 1.12a: 0x6FAC4CB0 (__stdcall)
void __stdcall ENV_InitRandomWindAndLightningTimers(void)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        gnWindChangeTimer_6FBCB7AC      = 312;
        gnLightningTimerFrames_6FBCB730 = 1250;
        gnCurrentWindAngle_6FBCB78C     = 127;
        gnTargetWindAngle_6FBCB7C0      = 127;
        return;
    }

    D2SeedStrc* const pSeed = &pPlayer->pSeed;
    gnWindChangeTimer_6FBCB7AC      = ENGINE_RollRandomRange(pSeed, 125, 375);
    gnLightningTimerFrames_6FBCB730 = ENGINE_RollRandomRange(pSeed, 500, 1500);
    const int32_t nInitialWind      = ENGINE_RollRandomRange(pSeed, 92, 71);
    gnCurrentWindAngle_6FBCB78C     = nInitialWind;
    gnTargetWindAngle_6FBCB7C0      = nInitialWind;
}

// 1.12a: 0x6FAC4D70 (__stdcall)
void __stdcall ENV_ConfigureAct5SnowParams(void)
{
    D2UnitStrc* const pPlayer   = gpClientPlayer_6FBCC3D0;
    D2ActiveRoomStrc* const pRoom = (pPlayer != nullptr) ? UNITS_GetRoom(pPlayer) : nullptr;
    const int32_t nLevelId      = DUNGEON_GetLevelIdFromRoom(pRoom);

    switch (nLevelId)
    {
    case 109: // Harrogath
    case 110: // Bloody Foothills
    case 111: // Frigid Highlands
    case 112: // Arreat Plateau
    case 121: // Nihlathak's Temple
    {
        gnWindAngleMinRain_6FBCB7A4   = 42;
        gnWindAngleMinSnow_6FBCB7A0   = 42;
        gnWindAngleRangeRain_6FBCB764 = 170;
        gnWindAngleRangeSnow_6FBCB760 = 170;
        const int32_t nCount = ENGINE_RollRandomRange(&pPlayer->pSeed, 32, 56);
        gnMaxTargetParticles_6FBCB788     = nCount;
        gnCurrentTargetParticles_6FBCB7A8 = nCount;
        break;
    }
    case 117: // Frozen Tundra
    {
        gnWindAngleRangeSnow_6FBCB760 = 56;
        gnWindAngleRangeRain_6FBCB764 = 56;
        gnWindAngleMinSnow_6FBCB7A0   = 170;
        gnWindAngleMinRain_6FBCB7A4   = 28;
        const int32_t nCount = ENGINE_RollRandomRange(&pPlayer->pSeed, 32, 112);
        gnMaxTargetParticles_6FBCB788     = nCount;
        gnCurrentTargetParticles_6FBCB7A8 = nCount;
        break;
    }
    case 120: // Arreat Summit
    {
        gnWindAngleMinSnow_6FBCB7A0       = 28;
        gnWindAngleRangeSnow_6FBCB760     = 28;
        gnWindAngleMinRain_6FBCB7A4       = 28;
        gnWindAngleRangeRain_6FBCB764     = 28;
        gnTargetWindAngle_6FBCB7C0        = 28;
        gnCurrentWindAngle_6FBCB78C       = 28;
        gnMaxTargetParticles_6FBCB788     = 256;
        gnCurrentTargetParticles_6FBCB7A8 = 256;
        break;
    }
    default:
    {
        gnCurrentTargetParticles_6FBCB7A8 = gnMaxTargetParticles_6FBCB788;
        if (gbIsSnowing_6FBCC3F8 != 0)
        {
            gbLightningEnabled_6FBCC3EC = FALSE;
        }
        else
        {
            const uint64_t uStep = static_cast<uint64_t>(pPlayer->pSeed.nLowSeed) * 0x6AC690C5ULL + pPlayer->pSeed.nHighSeed;
            pPlayer->pSeed.nLowSeed  = static_cast<uint32_t>(uStep);
            pPlayer->pSeed.nHighSeed = static_cast<uint32_t>(uStep >> 32);
            gbLightningEnabled_6FBCC3EC = FALSE;
        }
        break;
    }
    }
}

// 1.12a: 0x6FAC4F00 (custom: ebx = pSeed -> __fastcall)
void __fastcall ENV_SpawnParticlesAndStepWind(D2SeedStrc* pSeed)
{
    while (gpEnvParticlesPool_6FBCC3E8->nActiveCount < gnCurrentTargetParticles_6FBCB7A8)
    {
        ENV_SpawnWeatherParticle(pSeed);
    }

    const int32_t nSlewStep = (gbIsSnowing_6FBCC3F8 == 0) ? 2 : 1;
    if (gnTargetWindAngle_6FBCB7C0 < gnCurrentWindAngle_6FBCB78C)
    {
        const int32_t nNext = gnCurrentWindAngle_6FBCB78C - nSlewStep;
        gnCurrentWindAngle_6FBCB78C = (gnTargetWindAngle_6FBCB7C0 > nNext) ? gnTargetWindAngle_6FBCB7C0 : nNext;
    }
    else if (gnTargetWindAngle_6FBCB7C0 > gnCurrentWindAngle_6FBCB78C)
    {
        const int32_t nNext = gnCurrentWindAngle_6FBCB78C + nSlewStep;
        gnCurrentWindAngle_6FBCB78C = (gnTargetWindAngle_6FBCB7C0 < nNext) ? gnTargetWindAngle_6FBCB7C0 : nNext;
    }

    --gnWindChangeTimer_6FBCB7AC;
    if (gnWindChangeTimer_6FBCB7AC == 0)
    {
        if (gbIsSnowing_6FBCC3F8 != 0)
        {
            gnWindChangeTimer_6FBCB7AC  = ENGINE_RollRandomRange(pSeed, 250, 875);
            gnTargetWindAngle_6FBCB7C0  = ENGINE_RollRandomRange(pSeed, gnWindAngleMinSnow_6FBCB7A0, gnWindAngleRangeSnow_6FBCB760);
        }
        else
        {
            gnWindChangeTimer_6FBCB7AC  = ENGINE_RollRandomRange(pSeed, 125, 375);
            gnTargetWindAngle_6FBCB7C0  = ENGINE_RollRandomRange(pSeed, 92, 71);
        }
    }

    if (gbLightningEnabled_6FBCC3EC != 0)
    {
        --gnLightningTimerFrames_6FBCB730;
        if (gnLightningTimerFrames_6FBCB730 == 0)
        {
            gnLightningPendingTrigger_6FBCB7B0 = 1;
        }
    }
}

// 1.12a: 0x6FAC5030 (__stdcall: [esp+4] = pSeed)
void __stdcall ENV_StepWeatherParticles(D2SeedStrc* pSeed)
{
    float fSpeedMul = 1.0f;
    if (gbIsSnowing_6FBCC3F8 != 0)
    {
        fSpeedMul = std::fabs(FOG_10083_Cos_LUT(static_cast<int16_t>(gnCurrentWindAngle_6FBCB78C)));
        if (fSpeedMul < gfMinSnowCosSpeed_6FB87590)
        {
            fSpeedMul = gfMinSnowCosSpeed_6FB87590;
        }
    }
    else
    {
        fSpeedMul = gfCurrentRainIntensity_6FBCB768 * gfRainGustSpeedMul_6FB8758C + gfRainBaseSpeedMul_6FB87588;
    }

    D2FogDataListStrc* pPool = gpEnvParticlesPool_6FBCC3E8;
    D2EnvParticleStrc* const pElements = static_cast<D2EnvParticleStrc*>(pPool->pElements);

    for (int32_t i = 0; i <= pPool->nMaxAllocatedIdx; ++i)
    {
        D2EnvParticleStrc& rPart = pElements[i];
        if (rPart.wSlotSeq == 0)
        {
            continue;
        }

        const float fScaledSpeed = static_cast<float>(static_cast<int32_t>(static_cast<float>(rPart.nSpeed) * fSpeedMul));
        int32_t nStepX = static_cast<int32_t>(FOG_10083_Cos_LUT(static_cast<int16_t>(gnCurrentWindAngle_6FBCB78C)) * fScaledSpeed);
        const int32_t nStepY = static_cast<int32_t>(FOG_10084_Sin_LUT(static_cast<int16_t>(gnCurrentWindAngle_6FBCB78C)) * fScaledSpeed);

        rPart.nY += (nStepY - gnCameraScrollDeltaY_6FBCC070);
        if (rPart.nY > rPart.nEndY)
        {
            rPart.bLanded = 1;
        }

        if (rPart.bLanded != 0)
        {
            if (rPart.nMeltFrames == 0)
            {
                FOG_10065_DataListFreeHandle(gpEnvParticlesPool_6FBCC3E8, i << 16);
                if (gpEnvParticlesPool_6FBCC3E8->nActiveCount < gnCurrentTargetParticles_6FBCB7A8)
                {
                    ENV_SpawnWeatherParticle(pSeed);
                }
            }
            else
            {
                nStepX       = 0;
                rPart.nSpeed = 0;
                --rPart.nMeltFrames;
            }
        }

        if (rPart.wSlotSeq != 0)
        {
            if (gbIsSnowing_6FBCC3F8 != 0 && rPart.bLanded == 0)
            {
                const uint32_t uDriftAngle =
                    (((static_cast<uint32_t>(gnSnowDriftTickCounter_6FBD36B0) << 9) / 25u) +
                     static_cast<uint32_t>(rPart.nPhase)) & 0x1FFu;
                nStepX += static_cast<int32_t>(FOG_10084_Sin_LUT(static_cast<int16_t>(uDriftAngle)) * 2.0f);
            }

            int32_t nNewX = rPart.nX - gnCameraScrollDeltaX_6FBCC06C + nStepX;
            if (nNewX >= gnScreenWidth_6FB8C6E0)
            {
                nNewX -= gnScreenWidth_6FB8C6E0;
            }
            rPart.nX = nNewX;
            if (rPart.nX < 0)
            {
                rPart.nX += gnScreenWidth_6FB8C6E0;
            }
        }

        pPool = gpEnvParticlesPool_6FBCC3E8;
    }

    ++gnSnowDriftTickCounter_6FBD36B0;
}

// 1.12a: 0x6FAC51F0 (custom: eax = nPhase -> __fastcall)
void __fastcall ENV_SetWeatherCyclePhase(int32_t nPhase)
{
    if (nPhase < 0 || nPhase >= 4)
    {
        ENGINE_FatalHalt(0x12A);
    }

    if (gbWeatherDisabledCheat_6FBCC404 != 0 && nPhase != 0)
    {
        return;
    }

    if (gbForceConstantWeather_6FBCC400 != 0 && nPhase != 2)
    {
        return;
    }

    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    gnWeatherCyclePhase_6FBCC408 = nPhase;
    if (pPlayer == nullptr)
    {
        ENGINE_FatalHalt(0x137);
    }

    switch (nPhase)
    {
    case 0:
        gnCurrentTargetParticles_6FBCB7A8 = 0;
        break;
    case 1:
        ENV_InitLevelWeather(FALSE);
        gnMaxTargetParticles_6FBCB788 = ENGINE_RollRandomRange(&pPlayer->pSeed, 32, 224);
        break;
    case 2:
        ENV_ConfigureAct5SnowParams();
        break;
    case 3:
        gbLightningEnabled_6FBCC3EC = FALSE;
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FAC52C0 (__stdcall: [esp+4] = pSeed)
void __stdcall ENV_UpdateWeatherCycleMachine(D2SeedStrc* pSeed)
{
    int32_t nPhase = gnWeatherCyclePhase_6FBCC408;
    if (gbForceConstantWeather_6FBCC400 != 0 && nPhase != 2)
    {
        if (gbWeatherDisabledCheat_6FBCC404 != 0)
        {
            return;
        }

        gnWeatherCyclePhase_6FBCC408 = 2;
        if (gpClientPlayer_6FBCC3D0 == nullptr)
        {
            ENGINE_FatalHalt(0x137);
        }
        ENV_ConfigureAct5SnowParams();
        return;
    }

    if (gnWeatherPhaseRemTicks_6FBD36AC == 0)
    {
        ++nPhase;
        if (nPhase == 4)
        {
            nPhase = 0;
        }

        const int32_t nDuration = ENGINE_RollRandomRange(
            pSeed,
            ganWeatherPhaseDurationMin_6FBCB738[nPhase],
            ganWeatherPhaseDurationRange_6FBCB720[nPhase]);

        gnWeatherPhaseTotalTicks_6FBD36A8 = nDuration;
        gnWeatherPhaseRemTicks_6FBD36AC   = nDuration;
        ENV_SetWeatherCyclePhase(nPhase);
    }

    --gnWeatherPhaseRemTicks_6FBD36AC;
    const uint32_t uRem   = static_cast<uint32_t>(gnWeatherPhaseRemTicks_6FBD36AC);
    const uint32_t uTotal = static_cast<uint32_t>(gnWeatherPhaseTotalTicks_6FBD36A8);

    if (nPhase == 1)
    {
        gnCurrentTargetParticles_6FBCB7A8 =
            static_cast<int32_t>(((uTotal - uRem) * static_cast<uint32_t>(gnMaxTargetParticles_6FBCB788)) / uTotal);
    }
    else if (nPhase == 3)
    {
        gnCurrentTargetParticles_6FBCB7A8 =
            static_cast<int32_t>((static_cast<uint32_t>(gnMaxTargetParticles_6FBCB788) * uRem) / uTotal);
    }

    gfCurrentRainIntensity_6FBCB768 = static_cast<float>(gnCurrentTargetParticles_6FBCB7A8) * gfInv256_6FB87598;
}

// 1.12a: 0x6FAC53C0 (custom: eax = bConstant -> __fastcall)
void __fastcall ENV_SetForceConstantWeather(BOOL bConstant)
{
    gbForceConstantWeather_6FBCC400 = bConstant;
    if (bConstant != 0 && gbWeatherDisabledCheat_6FBCC404 == 0)
    {
        gnWeatherCyclePhase_6FBCC408 = 2;
        if (gpClientPlayer_6FBCC3D0 == nullptr)
        {
            ENGINE_FatalHalt(0x137);
        }
        ENV_ConfigureAct5SnowParams();
    }
}

// 1.12a: 0x6FAC5410 (__stdcall)
void __stdcall ENV_UpdateEnvironmentFrame(void)
{
    BOOL bRain = FALSE;
    BOOL bMud  = FALSE;
    if (gpClientPlayer_6FBCC3D0 != nullptr)
    {
        const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(gpClientPlayer_6FBCC3D0));
        bRain = (DATATBLS_GetRainFromLevelsTxt(nLevelId) != 0) ? TRUE : FALSE;
        bMud  = (DATATBLS_GetMudFromLevelsTxt(nLevelId) != 0) ? TRUE : FALSE;
    }

    if (!bRain && gbPrevLevelHadRain_6FBD36A4 != 0)
    {
        if (gpEnvParticlesPool_6FBCC3E8->nActiveCount != 0)
        {
            FOG_10063_DataListReset(gpEnvParticlesPool_6FBCC3E8);
        }
        if (gpEnvSplashesPool_6FBCC3E0->nActiveCount != 0)
        {
            FOG_10063_DataListReset(gpEnvSplashesPool_6FBCC3E0);
        }
    }

    if (!bMud && gbPrevLevelHadMud_6FBD36A0 != 0)
    {
        if (gpEnvBubblesPool_6FBCC3E4->nActiveCount != 0)
        {
            FOG_10063_DataListReset(gpEnvBubblesPool_6FBCC3E4);
        }
    }

    gbPrevLevelHadRain_6FBD36A4 = bRain;
    gbPrevLevelHadMud_6FBD36A0  = bMud;

    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        ENGINE_FatalHalt(0x547);
    }

    if (gdwClientGameFrame_6FBB32B0 > gdwLastEnvUpdateFrame_6FBCC3F0)
    {
        gdwLastEnvUpdateFrame_6FBCC3F0 = gdwClientGameFrame_6FBB32B0;
        D2SeedStrc* const pSeed        = &pPlayer->pSeed;

        if (bRain)
        {
            if (gpEnvParticlesPool_6FBCC3E8->nActiveCount != 0)
            {
                ENV_StepWeatherParticles(pSeed);
            }
            ENV_UpdateRainSplashes();
            ENV_UpdateWeatherCycleMachine(pSeed);
            ENV_SpawnParticlesAndStepWind(pSeed);
        }
        else
        {
            gfCurrentRainIntensity_6FBCB768 = 0.0f;
        }

        if (bMud)
        {
            ENV_UpdateSwampBubbles();
        }
    }
}

// 1.12a: 0x6FAC5540 (custom: esi = bSnowing, [esp+4] = bForceResetPool -> __fastcall)
void __fastcall ENV_SetSnowingState(BOOL bSnowing, BOOL bForceResetPool)
{
    if (gbLightningEnabled_6FBCC3EC != 0 && bSnowing != 0)
    {
        gbLightningEnabled_6FBCC3EC = FALSE;
    }

    if (gbForceConstantWeather_6FBCC400 != 0 && bSnowing == 0)
    {
        gbForceConstantWeather_6FBCC400 = FALSE;
    }

    if (bSnowing != gbIsSnowing_6FBCC3F8 || bForceResetPool != 0)
    {
        FOG_10063_DataListReset(gpEnvParticlesPool_6FBCC3E8);
        if (gpEnvParticlesPool_6FBCC3E8->nActiveCount != 0)
        {
            ENGINE_FatalHalt(0x15B);
        }
    }

    gbIsSnowing_6FBCC3F8 = bSnowing;
}

// 1.12a: 0x6FAC55C0 (__fastcall: ecx = bForceReset)
BOOL __fastcall ENV_InitLevelWeather(BOOL bForceReset)
{
    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    if (pPlayer == nullptr)
    {
        return FALSE;
    }

    D2ActiveRoomStrc* const pRoom = UNITS_GetRoom(pPlayer);
    if (pRoom == nullptr)
    {
        return FALSE;
    }

    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    if (DRLG_GetActNoFromLevelId(nLevelId) != ACT_V)
    {
        if (gbForceConstantWeather_6FBCC400 != 0)
        {
            gbForceConstantWeather_6FBCC400 = FALSE;
        }
        if (gbIsSnowing_6FBCC3F8 != 0)
        {
            FOG_10063_DataListReset(gpEnvParticlesPool_6FBCC3E8);
            if (gpEnvParticlesPool_6FBCC3E8->nActiveCount != 0)
            {
                ENGINE_FatalHalt(0x15B);
            }
        }
        gbIsSnowing_6FBCC3F8 = FALSE;
        ENV_InitRandomWindAndLightningTimers();
        return FALSE;
    }

    ENV_InitRandomWindAndLightningTimers();

    BOOL bConstantSnow       = FALSE;
    uint32_t uDryChancePct   = 0;
    switch (nLevelId)
    {
    case 109:
    case 110:
    case 111:
    case 112:
    case 121:
        gbLargeSnowflakes_6FBCC3FC = FALSE;
        uDryChancePct              = 25u;
        break;
    case 120:
        bConstantSnow              = TRUE;
        gbLargeSnowflakes_6FBCC3FC = TRUE;
        uDryChancePct              = 0u;
        break;
    default:
        break;
    }

    const uint64_t uStep = static_cast<uint64_t>(pPlayer->pSeed.nLowSeed) * 0x6AC690C5ULL + pPlayer->pSeed.nHighSeed;
    pPlayer->pSeed.nLowSeed  = static_cast<uint32_t>(uStep);
    pPlayer->pSeed.nHighSeed = static_cast<uint32_t>(uStep >> 32);

    if ((pPlayer->pSeed.nLowSeed % 100u) < uDryChancePct && bForceReset == 0)
    {
        if (gbForceConstantWeather_6FBCC400 != 0)
        {
            gbForceConstantWeather_6FBCC400 = FALSE;
        }
        if (gbIsSnowing_6FBCC3F8 != 0)
        {
            FOG_10063_DataListReset(gpEnvParticlesPool_6FBCC3E8);
            if (gpEnvParticlesPool_6FBCC3E8->nActiveCount != 0)
            {
                ENGINE_FatalHalt(0x15B);
            }
        }
        gbIsSnowing_6FBCC3F8 = FALSE;
        return FALSE;
    }

    ENV_SetForceConstantWeather(bConstantSnow);
    ENV_SetSnowingState(TRUE, bForceReset);
    return FALSE;
}

// 1.12a: 0x6FAC5760 (__stdcall)
void __stdcall ENV_ToggleWeatherCheat(void)
{
    const BOOL bDisable = (gbWeatherDisabledCheat_6FBCC404 == 0) ? TRUE : FALSE;
    gbWeatherDisabledCheat_6FBCC404 = bDisable;

    if (bDisable)
    {
        gbForceConstantWeather_6FBCC400 = FALSE;
        gnWeatherCyclePhase_6FBCC408    = 0;
        if (gpClientPlayer_6FBCC3D0 == nullptr)
        {
            ENGINE_FatalHalt(0x137);
        }
        gnCurrentTargetParticles_6FBCB7A8 = 0;
        ENV_ResetParticlesPool();
    }
    else
    {
        ENV_InitLevelWeather(TRUE);
    }
}

// 1.12a: 0x6FAC57F0 (custom: esi = bReinitWeather -> __fastcall)
void __fastcall ENV_OnLevelOrActTransition(BOOL bReinitWeather)
{
    FOG_10063_DataListReset(gpEnvSplashesPool_6FBCC3E0);
    FOG_10063_DataListReset(gpEnvBubblesPool_6FBCC3E4);
    if (bReinitWeather != 0)
    {
        ENV_InitLevelWeather(bReinitWeather);
    }
}

// 1.12a: 0x6FAC5820 (custom: eax = nLevelId -> __fastcall)
void __fastcall ENV_InitActWeatherPaletteAndOverlays(int32_t nLevelId)
{
    ENV_InitLevelWeather(TRUE);

    if (gbEnvWindInitialized_6FBD36B4 == 0)
    {
        ENV_InitRandomWindAndLightningTimers();
        gbEnvWindInitialized_6FBD36B4 = TRUE;
    }

    ganWeatherPhaseDurationMin_6FBCB738[0] = 7500;
    ganWeatherPhaseDurationMin_6FBCB738[1] = 250;
    ganWeatherPhaseDurationMin_6FBCB738[2] = 3000;
    ganWeatherPhaseDurationMin_6FBCB738[3] = 125;

    if (DRLG_GetActNoFromLevelId(nLevelId) == ACT_III)
    {
        ganWeatherPhaseDurationMin_6FBCB738[2] *= 3;
    }
    if (DRLG_GetActNoFromLevelId(nLevelId) == ACT_V)
    {
        ganWeatherPhaseDurationMin_6FBCB738[2] *= 3;
    }

    ganWeatherPhaseDurationRange_6FBCB720[0] = 7500;
    ganWeatherPhaseDurationRange_6FBCB720[1] = 250;
    ganWeatherPhaseDurationRange_6FBCB720[2] = 3000;
    ganWeatherPhaseDurationRange_6FBCB720[3] = 50;

    int32_t nStep80 = 0;
    int32_t nStep40 = 0;
    int32_t nStep24 = 0;
    for (int32_t i = 0; i < 12; ++i)
    {
        const int32_t nDayDrop  = nStep80 / 6;
        const uint8_t uDayBase  = static_cast<uint8_t>(98 - nDayDrop);
        gabRainColorDay_6FBCB748[i] = D2Win_10034_MixRGB(uDayBase, static_cast<uint8_t>(uDayBase + 25), uDayBase);

        const int32_t nDawnDrop = nStep40 / 6;
        const uint8_t uDawnBase = static_cast<uint8_t>(45 - nDawnDrop);
        gabRainColorDawnDusk_6FBCB714[i] = D2Win_10034_MixRGB(uDawnBase, static_cast<uint8_t>(uDawnBase + 10), uDawnBase);

        const int32_t nNightDrop = nStep24 / 6;
        const uint8_t uNightBase = static_cast<uint8_t>(25 - nNightDrop);
        gabRainColorNight_6FBCB76C[i] = D2Win_10034_MixRGB(uNightBase, static_cast<uint8_t>(uNightBase + 5), uNightBase);

        const uint8_t uSnowNorm = static_cast<uint8_t>(nDayDrop + 120);
        gabSnowColorNormal_6FBCB754[i] = D2Win_10034_MixRGB(uSnowNorm, uSnowNorm, uSnowNorm);

        const uint8_t uSnowBright = static_cast<uint8_t>(nDayDrop - 0x56);
        gabSnowColorBright_6FBCB7B4[i] = D2Win_10034_MixRGB(uSnowBright, uSnowBright, uSnowBright);

        nStep40 += 40;
        nStep80 += 80;
        nStep24 += 24;
    }

    char szOverlayPath[260] = {};
    int32_t nUnusedParam = 0;

    wsprintfA(szOverlayPath, gszRain1Fmt_6FB86F60, gszDataGlobal_6FB80424);
    gapRainSplashCellFiles_6FBCB778[0] = ARCHIVE_LoadClientUncompressedCellFile(szOverlayPath, &nUnusedParam, 0);

    wsprintfA(szOverlayPath, gszRain2Fmt_6FB86F48, gszDataGlobal_6FB80424);
    gapRainSplashCellFiles_6FBCB778[1] = ARCHIVE_LoadClientUncompressedCellFile(szOverlayPath, &nUnusedParam, 0);

    wsprintfA(szOverlayPath, gszRain3Fmt_6FB86F30, gszDataGlobal_6FB80424);
    gapRainSplashCellFiles_6FBCB778[2] = ARCHIVE_LoadClientUncompressedCellFile(szOverlayPath, &nUnusedParam, 0);

    wsprintfA(szOverlayPath, gszRain4Fmt_6FB86F18, gszDataGlobal_6FB80424);
    gapRainSplashCellFiles_6FBCB778[3] = ARCHIVE_LoadClientUncompressedCellFile(szOverlayPath, &nUnusedParam, 0);

    for (int32_t i = 0; i < 4; ++i)
    {
        ganRainSplashFrameCounts_6FBCB790[i] = D2CMP_CelFileGetCelsPerDirection(gapRainSplashCellFiles_6FBCB778[i]);
    }

    wsprintfA(szOverlayPath, gszBubble1Fmt_6FB86EFC, gszDataGlobal_6FB80424);
    gapBubbleCellFiles_6FBCB704[0] = ARCHIVE_LoadClientUncompressedCellFile(szOverlayPath, &nUnusedParam, 0);

    wsprintfA(szOverlayPath, gszBubble2Fmt_6FB86EE0, gszDataGlobal_6FB80424);
    gapBubbleCellFiles_6FBCB704[1] = ARCHIVE_LoadClientUncompressedCellFile(szOverlayPath, &nUnusedParam, 0);

    wsprintfA(szOverlayPath, gszBubble3Fmt_6FB86EC4, gszDataGlobal_6FB80424);
    gapBubbleCellFiles_6FBCB704[2] = ARCHIVE_LoadClientUncompressedCellFile(szOverlayPath, &nUnusedParam, 0);

    wsprintfA(szOverlayPath, gszBubble4Fmt_6FB86EA8, gszDataGlobal_6FB80424);
    gapBubbleCellFiles_6FBCB704[3] = ARCHIVE_LoadClientUncompressedCellFile(szOverlayPath, &nUnusedParam, 0);

    for (int32_t i = 0; i < 4; ++i)
    {
        ganBubbleCelFrameCounts_6FBCB6F4[i] = D2CMP_CelFileGetCelsPerDirection(gapBubbleCellFiles_6FBCB704[i]);
    }

    gfCurrentRainIntensity_6FBCB768 = 0.0f;
}


// =============================================================================
// SECTION 6: Client Decoys, Ambient Critter AI, & DRLG Room Critter Spawning
// VA Range: 0x6FAC5B50 .. 0x6FAC746E (28 functions)
// =============================================================================

using D2RoomUnitEnumFn = int32_t (__fastcall*)(D2UnitStrc* pSelf, D2UnitStrc* pCandidate, void* pCtx1, void* pCtx2);

// 1.12a: 0x6FAC5B50 (__stdcall)
void __stdcall CRITTER_ClearAllDecoyTargets(void)
{
    for (int32_t i = 0; i < 8; ++i)
    {
        gapClientDecoyTargets_6FBCB6D4[i] = nullptr;
    }
}

// 1.12a: 0x6FAC5B80 (custom: edx = pUnit -> __fastcall)
void __fastcall CRITTER_UnregisterDecoyTarget(int32_t nUnusedEcx, D2UnitStrc* pUnit)
{
    (void)nUnusedEcx;
    for (int32_t i = 0; i < 8; ++i)
    {
        if (gapClientDecoyTargets_6FBCB6D4[i] == pUnit)
        {
            gapClientDecoyTargets_6FBCB6D4[i] = nullptr;
            return;
        }
    }
}

// 1.12a: 0x6FAC5BA0 (__stdcall: [esp+4] = pUnit)
void __stdcall CRITTER_RegisterDecoyTarget(D2UnitStrc* pUnit)
{
    for (int32_t i = 0; i < 8; ++i)
    {
        if (gapClientDecoyTargets_6FBCB6D4[i] == nullptr)
        {
            gapClientDecoyTargets_6FBCB6D4[i] = pUnit;
            return;
        }
    }
}

// 1.12a: 0x6FAC5BD0 (custom: edi = pSelf, ebx = pfnCallback, [esp+4] = pContext1, [esp+8] = pContext2 -> __fastcall)
int32_t __fastcall CRITTER_ForEachUnitInAdjacentRooms(D2UnitStrc* pSelf, D2RoomUnitEnumFn pfnCallback, void* pContext1, void* pContext2)
{
    D2ActiveRoomStrc* const pRoom = UNITS_GetRoom(pSelf);
    if (pRoom == nullptr)
    {
        return 0;
    }

    D2ActiveRoomStrc** ppAdjacentRooms = nullptr;
    int32_t nNumRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppAdjacentRooms, &nNumRooms);

    for (uint32_t i = 0; i < static_cast<uint32_t>(nNumRooms); ++i)
    {
        D2ActiveRoomStrc* const pAdjRoom = ppAdjacentRooms[i];
        for (D2UnitStrc* pUnit = pAdjRoom->pUnitFirst; pUnit != nullptr; pUnit = pUnit->pRoomNext)
        {
            const int32_t nResult = pfnCallback(pSelf, pUnit, pContext1, pContext2);
            if (nResult != 0)
            {
                return nResult;
            }
        }
    }
    return 0;
}

// 1.12a: 0x6FAC5C60 (custom: eax = pUnitA, edi = pUnitB -> __fastcall)
BOOL __fastcall CRITTER_AreUnitsAtSameCoords(D2UnitStrc* pUnitA, D2UnitStrc* pUnitB)
{
    const int32_t nXA = ENGINE_GetUnitX(pUnitA);
    const int32_t nYA = ENGINE_GetUnitY(pUnitA);
    const int32_t nXB = ENGINE_GetUnitX(pUnitB);
    const int32_t nYB = ENGINE_GetUnitY(pUnitB);
    return (nXA == nXB && nYA == nYB) ? TRUE : FALSE;
}

// 1.12a: 0x6FAC5D30 (custom: edi = pnInOutMinDist, [esp+4] = pUnit -> __fastcall)
D2UnitStrc* __fastcall CRITTER_FindNearestActiveDecoy(int32_t* pnInOutMinDist, D2UnitStrc* pUnit)
{
    D2UnitStrc* pBestTarget = nullptr;
    for (int32_t i = 0; i < 8; ++i)
    {
        D2UnitStrc* const pCandidate = gapClientDecoyTargets_6FBCB6D4[i];
        if (pCandidate != nullptr &&
            pCandidate->dwAnimMode != 0 &&
            pCandidate->dwAnimMode != MONMODE_DEAD)
        {
            const int32_t nDist = UNITS_GetDistanceToOtherUnit(pUnit, pCandidate);
            if (nDist < *pnInOutMinDist)
            {
                pBestTarget     = pCandidate;
                *pnInOutMinDist = nDist;
            }
        }
    }
    return pBestTarget;
}

// 1.12a: 0x6FAC5D80 (__fastcall: ecx = pSelf, edx = pCandidate, [esp+4] = pCtxCorpse, [esp+8] = pCtxDist)
int32_t __fastcall CRITTER_CallbackFindCorpseInRange(D2UnitStrc* pSelf, D2UnitStrc* pCandidate, void* pCtxCorpse, void* pCtxDist)
{
    if (pCandidate == nullptr)
    {
        return 0;
    }

    const uint32_t dwType = pCandidate->dwUnitType;
    if (dwType != UNIT_PLAYER && dwType != UNIT_MONSTER)
    {
        return 0;
    }

    const uint32_t dwMode = pCandidate->dwAnimMode;
    if (dwType == UNIT_PLAYER)
    {
        if (dwMode != PLRMODE_DEATH && dwMode != PLRMODE_DEAD)
        {
            return 0;
        }
    }
    else
    {
        if (dwMode != MONMODE_DEATH && dwMode != MONMODE_DEAD)
        {
            return 0;
        }
    }

    const int32_t nDist = UNITS_GetDistanceToOtherUnit(pSelf, pCandidate);
    if (nDist >= 3 && nDist <= 25 &&
        UNITS_GetDistanceToOtherUnit(pCandidate, gpClientPlayer_6FBCC3D0) > 7 &&
        ENGINE_RollRandomNumber(&pSelf->pSeed, 2) != 0)
    {
        *static_cast<int32_t*>(pCtxDist)      = nDist;
        *static_cast<D2UnitStrc**>(pCtxCorpse) = pCandidate;
    }
    return 0;
}

// 1.12a: 0x6FAC5E00 (__fastcall: ecx = pSelf, edx = pCandidate, [esp+4] = pCtxThreat, [esp+8] = pCtxMinDist)
int32_t __fastcall CRITTER_CallbackFindNearestThreat(D2UnitStrc* pSelf, D2UnitStrc* pCandidate, void* pCtxThreat, void* pCtxMinDist)
{
    if (pSelf == pCandidate || pCandidate == nullptr)
    {
        return 0;
    }

    const uint32_t dwType = pCandidate->dwUnitType;
    if (dwType != UNIT_PLAYER)
    {
        if (dwType != UNIT_MONSTER)
        {
            return 0;
        }
        if (pCandidate->dwClassId == 0x95 || pCandidate->dwClassId == 0x22C)
        {
            return 0;
        }
    }

    if (UNIT_IsUnitDeadOrCorpse(pSelf))
    {
        return 0;
    }

    int32_t* const pnMinDist = static_cast<int32_t*>(pCtxMinDist);
    const int32_t nDist      = UNITS_GetDistanceToOtherUnit(pSelf, pCandidate);
    if (nDist < *pnMinDist)
    {
        *static_cast<D2UnitStrc**>(pCtxThreat) = pCandidate;
        *pnMinDist = nDist;
    }
    return 0;
}

// 1.12a: 0x6FAC5E60 (custom: esi = pCenterUnit, [esp+4] = nMonsterClassId, [esp+8] = nCount -> __fastcall)
void __fastcall CRITTER_SpawnAmbientCritterClusterNearUnit(D2UnitStrc* pCenterUnit, int32_t nMonsterClassId, int32_t nCount)
{
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pCenterUnit);
    D2MonStats2Txt* const pMonStats2Txt = ENGINE_GetMonStats2TxtRecordFromMonsterId(nMonsterClassId);
    if (pMonStats2Txt == nullptr)
    {
        return;
    }

    D2CoordStrc tCoords = {};
    UNITS_GetCoords(pCenterUnit, &tCoords);

    int32_t nAttempt = 0;
    do
    {
        const uint64_t uStepX = static_cast<uint64_t>(pCenterUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pCenterUnit->pSeed.nHighSeed;
        pCenterUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uStepX);
        pCenterUnit->pSeed.nHighSeed = static_cast<uint32_t>(uStepX >> 32);
        const int32_t nSpawnX = tCoords.nX + static_cast<int32_t>(pCenterUnit->pSeed.nLowSeed % 5u) - 2;

        const uint64_t uStepY = static_cast<uint64_t>(pCenterUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pCenterUnit->pSeed.nHighSeed;
        pCenterUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uStepY);
        pCenterUnit->pSeed.nHighSeed = static_cast<uint32_t>(uStepY >> 32);
        const int32_t nSpawnY = tCoords.nY + static_cast<int32_t>(pCenterUnit->pSeed.nLowSeed % 5u) - 2;

        pRoom = ENGINE_FindRoomAtSubtileCoords(pRoom, nSpawnX, nSpawnY);
        const int32_t nSizeX = static_cast<int8_t>(pMonStats2Txt->nSizeX);
        if (COLLISION_CheckMaskWithSize(pRoom, nSpawnX, nSpawnY, nSizeX, 0x3F11u) == 0)
        {
            UNIT_CreateClientUnitAtCoords(nMonsterClassId, nSpawnX, nSpawnY, UNIT_MONSTER, MONMODE_DEATH);
            --nCount;
        }

        if (nCount <= 0)
        {
            break;
        }
        ++nAttempt;
    } while (nAttempt < 10);
}

// 1.12a: 0x6FAC5F90 (custom: eax = pUnit, ecx = nFrames -> __fastcall)
void __fastcall CRITTER_SetAiDelayFramesClamped(D2UnitStrc* pUnit, int32_t nFrames)
{
    if (nFrames >= 200)
    {
        nFrames = 200;
    }

    D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
    if (pAiState != nullptr)
    {
        pAiState->nAiDelayFrames = nFrames;
    }
}

// 1.12a: 0x6FAC5FC0 (custom: eax = pUnit -> __fastcall)
void __fastcall CRITTER_UpdateBirdSpawnerMonster_23E(D2UnitStrc* pUnit)
{
    if (gpClientPlayer_6FBCC3D0 == nullptr)
    {
        D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
        if (pAiState != nullptr)
        {
            pAiState->nAiDelayFrames = 5;
        }
        return;
    }

    const uint64_t uChanceStep = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
    pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uChanceStep);
    pUnit->pSeed.nHighSeed = static_cast<uint32_t>(uChanceStep >> 32);
    if ((pUnit->pSeed.nLowSeed % 100u) >= 5u)
    {
        return;
    }

    const uint64_t uMisStep = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
    pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uMisStep);
    pUnit->pSeed.nHighSeed = static_cast<uint32_t>(uMisStep >> 32);
    const int32_t nMissileId = static_cast<int32_t>(pUnit->pSeed.nLowSeed & 3u) + 0x23A;

    const int32_t nBaseX = ENGINE_GetUnitX(pUnit);
    const uint64_t uStepX = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
    pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uStepX);
    pUnit->pSeed.nHighSeed = static_cast<uint32_t>(uStepX >> 32);
    const int32_t nSpawnX = nBaseX + static_cast<int32_t>(pUnit->pSeed.nLowSeed % 60u) - 30;

    const int32_t nBaseY = ENGINE_GetUnitY(pUnit);
    const uint64_t uStepY = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
    pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uStepY);
    pUnit->pSeed.nHighSeed = static_cast<uint32_t>(uStepY >> 32);
    const int32_t nSpawnY = nBaseY + static_cast<int32_t>(pUnit->pSeed.nLowSeed % 60u) - 30;

    D2UnitStrc* const pMis = MISSILE_SpawnClientEffectAtCoords(0, pUnit, nMissileId, nSpawnX, nSpawnY, 0, 1);
    if (pMis == nullptr)
    {
        return;
    }

    const uint64_t uDirStep = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
    pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uDirStep);
    pUnit->pSeed.nHighSeed = static_cast<uint32_t>(uDirStep >> 32);
    const BOOL bAscending = static_cast<BOOL>(pUnit->pSeed.nLowSeed & 1u);

    if (bAscending)
    {
        MISSILE_InitBallisticVelocity(pMis, -100, 0, 0);
        if (pMis->pGfxData != nullptr)
        {
            D2ClientBallisticHeightStrc* const pBallistic =
                reinterpret_cast<D2ClientUnitGfxStrc*>(pMis->pGfxData)->pBallistic;
            if (pBallistic != nullptr)
            {
                pBallistic->nVelX    = 0;
                pBallistic->nVelY    = 0;
                pBallistic->nVelZ    = 0x3000;
                pBallistic->nAccelX  = 0;
                pBallistic->nAccelY  = 0;
                pBallistic->nAccelZ  = 0x96000;
                pBallistic->dwFlags |= 0x20u;
            }
        }
    }
    else
    {
        MISSILE_InitBallisticVelocity(pMis, 300, 0, 0);
        if (pMis->pGfxData != nullptr)
        {
            D2ClientBallisticHeightStrc* const pBallistic =
                reinterpret_cast<D2ClientUnitGfxStrc*>(pMis->pGfxData)->pBallistic;
            if (pBallistic != nullptr)
            {
                pBallistic->nVelX    = 0;
                pBallistic->nVelY    = 0;
                pBallistic->nVelZ    = -0x3000;
                pBallistic->nAccelX  = 0;
                pBallistic->nAccelY  = 0;
                pBallistic->nAccelZ  = -0x32000;
                pBallistic->dwFlags &= ~0x20u;
            }
        }
    }

    MISSILE_FinalizeCritterFlight(pMis);
    MISSILE_SetTotalFrames(pMis, 60);
    MISSILE_SetCurrentFrame(pMis, 60);
    MISSILE_SetTargetX(pMis, 100);
    MISSILE_SetTargetY(pMis, 300);
}

// 1.12a: 0x6FAC6220 (custom: esi = pUnit -> __fastcall)
void __fastcall CRITTER_EnterIdleNeutralMode(D2UnitStrc* pUnit)
{
    D2ClientMonsterMoveCmdStrc tCmd = {};
    tCmd.nTargetX = ENGINE_GetUnitX(pUnit);
    tCmd.nTargetY = ENGINE_GetUnitY(pUnit);
    UNITMODE_StartClientMonsterMode(7, pUnit, &tCmd);
}

// 1.12a: 0x6FAC62B0 (custom: esi = pUnit -> __fastcall)
void __fastcall CRITTER_TriggerSquishDeathMode(D2UnitStrc* pUnit)
{
    D2ClientMonsterMoveCmdStrc tCmd = {};
    tCmd.nTargetX = ENGINE_GetUnitX(pUnit);
    tCmd.nTargetY = ENGINE_GetUnitY(pUnit);
    UNITMODE_StartClientMonsterMode(8, pUnit, &tCmd);
}

// 1.12a: 0x6FAC6340 (custom: eax = nTargetX, ecx = nTargetY, edi = pUnit, [esp+4] = nMode, [esp+8] = nOverridePathType -> __fastcall)
void __fastcall CRITTER_MoveMonsterToCoords(int32_t nTargetX, int32_t nTargetY, D2UnitStrc* pUnit, int32_t nMode, int32_t nOverridePathType)
{
    D2DynamicPathStrc* const pPath = pUnit->pDynamicPath;
    D2ClientMonsterMoveCmdStrc tCmd = {};
    tCmd.nTargetX          = nTargetX;
    tCmd.nTargetY          = nTargetY;
    tCmd.nUnk08            = 0;
    tCmd.nPathType         = PATH_GetType(pPath);
    tCmd.nOverridePathType = nOverridePathType;
    tCmd.nMaxDistance      = static_cast<int32_t>(static_cast<uint8_t>(PATH_GetMaxDistance(pPath)));
    UNITMODE_StartClientMonsterMode(nMode, pUnit, &tCmd);
}

// 1.12a: 0x6FAC6390 (custom: esi = pUnit, [esp+4] = uDist, [esp+8] = nMode, [esp+12] = nOverridePathType -> __fastcall)
void __fastcall CRITTER_WanderRandomDiagonalOffset(D2UnitStrc* pUnit, uint8_t uDist, int32_t nMode, int32_t nOverridePathType)
{
    const int32_t nOriginX = ENGINE_GetUnitX(pUnit);
    const int32_t nOriginY = ENGINE_GetUnitY(pUnit);

    int32_t nDeltaX = static_cast<int32_t>(uDist);
    int32_t nDeltaY = static_cast<int32_t>(uDist);

    const uint64_t uStepX = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
    pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uStepX);
    pUnit->pSeed.nHighSeed = static_cast<uint32_t>(uStepX >> 32);
    if ((pUnit->pSeed.nLowSeed & 1u) != 0)
    {
        nDeltaX = -nDeltaX;
    }

    const uint64_t uStepY = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
    pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uStepY);
    pUnit->pSeed.nHighSeed = static_cast<uint32_t>(uStepY >> 32);
    if ((pUnit->pSeed.nLowSeed & 1u) != 0)
    {
        nDeltaY = -nDeltaY;
    }

    D2DynamicPathStrc* const pPath = pUnit->pDynamicPath;
    D2ClientMonsterMoveCmdStrc tCmd = {};
    tCmd.nTargetX          = nOriginX + nDeltaX;
    tCmd.nTargetY          = nOriginY + nDeltaY;
    tCmd.nUnk08            = 0;
    tCmd.nPathType         = PATH_GetType(pPath);
    tCmd.nOverridePathType = nOverridePathType;
    tCmd.nMaxDistance      = static_cast<int32_t>(static_cast<uint8_t>(PATH_GetMaxDistance(pPath)));
    UNITMODE_StartClientMonsterMode(nMode, pUnit, &tCmd);
}

// 1.12a: 0x6FAC64B0 (custom: eax = pThreat, [esp+4] = pCritter, [esp+8] = uStepDist, [esp+12] = nMode, [esp+16] = nOverridePathType -> __fastcall)
BOOL __fastcall CRITTER_FleeAwayFromThreat(D2UnitStrc* pThreat, int32_t nUnusedEdx, D2UnitStrc* pCritter, uint8_t uStepDist, int32_t nMode, int32_t nOverridePathType)
{
    (void)nUnusedEdx;
    const uint32_t uCritterX = static_cast<uint32_t>(ENGINE_GetUnitX(pCritter));
    const uint32_t uCritterY = static_cast<uint32_t>(ENGINE_GetUnitY(pCritter));
    const uint32_t uThreatX  = static_cast<uint32_t>(ENGINE_GetUnitX(pThreat));
    const uint32_t uThreatY  = static_cast<uint32_t>(ENGINE_GetUnitY(pThreat));

    int32_t nSignX = 0;
    if (uThreatX > uCritterX)
    {
        nSignX = -1;
    }
    if (uThreatX < uCritterX)
    {
        nSignX = 1;
    }

    int32_t nSignY = 0;
    if (uThreatY > uCritterY)
    {
        nSignY = -1;
    }
    if (uThreatY < uCritterY)
    {
        nSignY = 1;
    }

    D2DynamicPathStrc* const pPath = pCritter->pDynamicPath;
    const uint32_t dwFootprintMask = PATH_GetFootprintCollisionMask(pPath);
    if (COLLISION_CheckMaskWithSize(
            UNITS_GetRoom(pCritter),
            static_cast<int32_t>(uCritterX) + nSignX,
            static_cast<int32_t>(uCritterY) + nSignY,
            1,
            dwFootprintMask) != 0)
    {
        return FALSE;
    }

    const int32_t nDist = static_cast<int32_t>(uStepDist);
    D2ClientMonsterMoveCmdStrc tCmd = {};
    tCmd.nTargetX          = static_cast<int32_t>(uCritterX) + (nSignX * nDist);
    tCmd.nTargetY          = static_cast<int32_t>(uCritterY) + (nSignY * nDist);
    tCmd.nUnk08            = 0;
    tCmd.nPathType         = PATH_GetType(pPath);
    tCmd.nOverridePathType = nOverridePathType;
    tCmd.nMaxDistance      = static_cast<int32_t>(static_cast<uint8_t>(PATH_GetMaxDistance(pPath)));
    UNITMODE_StartClientMonsterMode(nMode, pCritter, &tCmd);
    return TRUE;
}

// 1.12a: 0x6FAC6650 (custom: eax = pTarget, edi = pUnit, [esp+4] = nMode, [esp+8] = nOverridePathType -> __fastcall)
void __fastcall CRITTER_MoveMonsterTowardUnit(D2UnitStrc* pTarget, D2UnitStrc* pUnit, int32_t nMode, int32_t nOverridePathType)
{
    D2DynamicPathStrc* const pPath = pUnit->pDynamicPath;
    D2ClientMonsterMoveCmdStrc tCmd = {};
    tCmd.nTargetMode       = (pTarget != nullptr) ? static_cast<int32_t>(pTarget->dwUnitType) : UNIT_TYPES_COUNT;
    tCmd.dwTargetGUID      = (pTarget != nullptr) ? pTarget->dwUnitId : 0xFFFFFFFFu;
    tCmd.nUnk08            = 0;
    tCmd.nPathType         = PATH_GetType(pPath);
    tCmd.nOverridePathType = nOverridePathType;
    tCmd.nMaxDistance      = static_cast<int32_t>(static_cast<uint8_t>(PATH_GetMaxDistance(pPath)));
    UNITMODE_StartClientMonsterMode(nMode, pUnit, &tCmd);
}

// 1.12a: 0x6FAC66C0 (custom: eax = pUnit, ebx = pNearestTarget -> __fastcall)
void __fastcall CRITTER_UpdateZooMonsterAI(D2UnitStrc* pUnit, D2UnitStrc* pNearestTarget)
{
    const uint32_t dwMode = (pUnit != nullptr) ? pUnit->dwAnimMode : 0u;
    if (MONSTERS_IsDead(pUnit) || dwMode != MONMODE_NEUTRAL)
    {
        return;
    }

    const int32_t nClassId = (pUnit != nullptr) ? pUnit->dwClassId : -1;
    if (ENGINE_CheckMonStats2ModeFlag(nClassId, MONMODE_WALK) == 0)
    {
        D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
        if (pAiState != nullptr)
        {
            pAiState->nAiDelayFrames = 20;
        }
        CRITTER_EnterIdleNeutralMode(pUnit);
        return;
    }

    const uint64_t uStep = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
    pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uStep);
    pUnit->pSeed.nHighSeed = static_cast<uint32_t>(uStep >> 32);
    const uint32_t uRoll   = pUnit->pSeed.nLowSeed % 100u;

    if (uRoll < 40u)
    {
        D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
        if (pAiState != nullptr)
        {
            pAiState->nAiDelayFrames = 20;
        }
        CRITTER_EnterIdleNeutralMode(pUnit);
        return;
    }

    if (uRoll < 60u && pNearestTarget != nullptr)
    {
        CRITTER_MoveMonsterTowardUnit(pNearestTarget, pUnit, 0, 6);
        return;
    }

    if (uRoll < 80u && pNearestTarget != nullptr)
    {
        CRITTER_FleeAwayFromThreat(pNearestTarget, 0, pUnit, 8, MONMODE_NEUTRAL, 4);
        return;
    }

    CRITTER_WanderRandomDiagonalOffset(pUnit, 8, MONMODE_NEUTRAL, 4);
}

// 1.12a: 0x6FAC67B0 (custom: eax = pUnit -> __fastcall)
void __fastcall CRITTER_UpdateCamelOrHareAI_22C(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr || pUnit->dwAnimMode != MONMODE_NEUTRAL)
    {
        return;
    }

    const int32_t nY = ENGINE_GetUnitY(pUnit);
    const int32_t nX = ENGINE_GetUnitX(pUnit);
    if (COLLISION_CheckMask(UNITS_GetRoom(pUnit), nX, nY, 0x1C0u) != 0)
    {
        CRITTER_TriggerSquishDeathMode(pUnit);
        return;
    }

    D2UnitStrc* pThreat = nullptr;
    int32_t nMinDist    = 0x7FFFFFFF;
    CRITTER_ForEachUnitInAdjacentRooms(pUnit, CRITTER_CallbackFindNearestThreat, &pThreat, &nMinDist);
    if (pThreat == nullptr)
    {
        return;
    }

    D2DynamicPathStrc* const pPath = pUnit->pDynamicPath;
    PATH_SetFootprintCollisionMask(pPath, 0x3C01u);

    if (nMinDist < 6)
    {
        PATH_SetFootprintCollisionMask(pPath, 0x0C01u);
        if (!CRITTER_FleeAwayFromThreat(pThreat, 0, pUnit, 6, MONMODE_NEUTRAL, 120))
        {
            CRITTER_WanderRandomDiagonalOffset(pUnit, 6, MONMODE_NEUTRAL, 120);
        }
        return;
    }

    if (ENGINE_RollRandomNumber(&pUnit->pSeed, 100) < 30)
    {
        CRITTER_WanderRandomDiagonalOffset(pUnit, 2, MONMODE_NEUTRAL, 0);
        return;
    }

    D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
    if (pAiState != nullptr)
    {
        pAiState->nAiDelayFrames = 5;
    }
}

// 1.12a: 0x6FAC6900 (custom: eax = pTarget, ecx = pUnit, [esp+4] = uDist -> __fastcall)
void __fastcall CRITTER_UpdateScorpionOrSnakeAI_13E(D2UnitStrc* pTarget, D2UnitStrc* pUnit, uint32_t uDist)
{
    if (MONSTERS_IsDead(pUnit))
    {
        return;
    }

    if (CRITTER_AreUnitsAtSameCoords(pUnit, pTarget))
    {
        CRITTER_TriggerSquishDeathMode(pUnit);
        return;
    }

    if (pUnit == nullptr || pUnit->dwAnimMode != MONMODE_NEUTRAL)
    {
        return;
    }

    if (uDist <= 1u)
    {
        CRITTER_FleeAwayFromThreat(pTarget, 0, pUnit, 4, MONMODE_NEUTRAL, 0);
        return;
    }

    if (ENGINE_RollRandomNumber(&pUnit->pSeed, 100) < 75)
    {
        D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
        if (pAiState != nullptr)
        {
            pAiState->nAiDelayFrames = 32;
        }
        CRITTER_EnterIdleNeutralMode(pUnit);
        return;
    }

    CRITTER_WanderRandomDiagonalOffset(pUnit, 4, MONMODE_NEUTRAL, 0);
}

// 1.12a: 0x6FAC6990 (custom: eax = pTarget, ecx = pUnit, [esp+4] = uDist -> __fastcall)
void __fastcall CRITTER_UpdateCarrionBirdOrScavengerAI(D2UnitStrc* pTarget, D2UnitStrc* pUnit, uint32_t uDist)
{
    if (pUnit == nullptr)
    {
        return;
    }

    const uint32_t dwMode = pUnit->dwAnimMode;
    if (dwMode == MONMODE_DEAD || dwMode == MONMODE_DEATH)
    {
        return;
    }

    if (uDist < 2u && CRITTER_AreUnitsAtSameCoords(pUnit, pTarget))
    {
        CRITTER_TriggerSquishDeathMode(pUnit);
        return;
    }

    if (dwMode != MONMODE_NEUTRAL)
    {
        return;
    }

    if (uDist < 4u)
    {
        CRITTER_FleeAwayFromThreat(pTarget, 0, pUnit, 6, static_cast<int32_t>(dwMode), 0);
        return;
    }

    D2UnitStrc* pCorpse  = nullptr;
    int32_t nCorpseDist  = 0;
    if (uDist > 7u)
    {
        CRITTER_ForEachUnitInAdjacentRooms(pUnit, CRITTER_CallbackFindCorpseInRange, &pCorpse, &nCorpseDist);
    }

    int32_t* const pChatterState = MONSTER_GetClientIdleChatterState(pUnit, &pUnit->pSeed);
    if (pChatterState == nullptr)
    {
        D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
        if (pAiState != nullptr)
        {
            pAiState->nAiDelayFrames = 32;
        }
        CRITTER_EnterIdleNeutralMode(pUnit);
        return;
    }

    if (pCorpse != nullptr && uDist > 7u && *pChatterState == 0)
    {
        CRITTER_MoveMonsterTowardUnit(pCorpse, pUnit, 0, 6);
        *pChatterState = 1;
        return;
    }

    if (*pChatterState != 0)
    {
        if (ENGINE_RollRandomNumber(&pUnit->pSeed, 100) < 20)
        {
            *pChatterState = 0;
        }

        D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
        if (pAiState != nullptr)
        {
            pAiState->nAiDelayFrames = 32;
        }
        CRITTER_EnterIdleNeutralMode(pUnit);
        return;
    }

    if (ENGINE_RollRandomNumber(&pUnit->pSeed, 100) < 75)
    {
        D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
        if (pAiState != nullptr)
        {
            pAiState->nAiDelayFrames = 30;
        }
        CRITTER_EnterIdleNeutralMode(pUnit);
        return;
    }

    CRITTER_WanderRandomDiagonalOffset(pUnit, 5, MONMODE_NEUTRAL, 0);
}

// 1.12a: 0x6FAC6B10 (custom: eax = pUnit, [esp+4] = pTarget -> __fastcall)
void __fastcall CRITTER_UpdateMaggotOrBugAI_E3(D2UnitStrc* pUnit, int32_t nUnusedEdx, D2UnitStrc* pTarget)
{
    (void)nUnusedEdx;
    const uint32_t dwMode = (pUnit != nullptr) ? pUnit->dwAnimMode : 0u;
    if (MONSTERS_IsDead(pUnit))
    {
        return;
    }

    if (CRITTER_AreUnitsAtSameCoords(pUnit, pTarget))
    {
        CRITTER_TriggerSquishDeathMode(pUnit);
        return;
    }

    if (dwMode != MONMODE_NEUTRAL)
    {
        return;
    }

    const uint64_t uStep = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
    pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uStep);
    pUnit->pSeed.nHighSeed = static_cast<uint32_t>(uStep >> 32);

    if ((pUnit->pSeed.nLowSeed % 100u) < 40u)
    {
        D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
        if (pAiState != nullptr)
        {
            pAiState->nAiDelayFrames = 20;
        }
        CRITTER_EnterIdleNeutralMode(pUnit);
        return;
    }

    CRITTER_WanderRandomDiagonalOffset(pUnit, 4, MONMODE_NEUTRAL, 0);
}

// 1.12a: 0x6FAC6BB0 (custom: eax = pUnit -> __fastcall)
void __fastcall CRITTER_UpdateFlyingVultureOrParrotAI(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr || pUnit->dwAnimMode != MONMODE_NEUTRAL)
    {
        return;
    }

    D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
    if (pAiState == nullptr)
    {
        return;
    }

    D2UnitStrc* const pPlayer = gpClientPlayer_6FBCC3D0;
    const int32_t nDist = UNITS_GetDistanceToOtherUnit(pUnit, pPlayer);
    if (pAiState->bHasSeenPlayer != 0 && nDist > 25)
    {
        const uint32_t dwGUID = pUnit->dwUnitId;
        UNIT_RemoveClientSideUnitFromHash(gapClientMonsterHashTable_6FBC9F60, dwGUID & 0x7Fu, dwGUID, UNIT_MONSTER);
        return;
    }

    pAiState->bHasSeenPlayer = 1;
    D2SeedStrc* const pSeed  = &pUnit->pSeed;

    const uint64_t uAxisStep = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
    pSeed->nLowSeed  = static_cast<uint32_t>(uAxisStep);
    pSeed->nHighSeed = static_cast<uint32_t>(uAxisStep >> 32);

    int32_t nOffsetX = 0;
    int32_t nOffsetY = 0;
    if ((pSeed->nLowSeed & 1u) != 0)
    {
        nOffsetX = 40;
        nOffsetY = ENGINE_RollRandomNumber(pSeed, 40);
    }
    else
    {
        nOffsetX = ENGINE_RollRandomNumber(pSeed, 40);
        nOffsetY = 40;
    }

    const uint64_t uSignXStep = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
    pSeed->nLowSeed  = static_cast<uint32_t>(uSignXStep);
    pSeed->nHighSeed = static_cast<uint32_t>(uSignXStep >> 32);
    if ((pSeed->nLowSeed & 1u) != 0)
    {
        nOffsetX = -nOffsetX;
    }

    const uint64_t uSignYStep = static_cast<uint64_t>(pSeed->nLowSeed) * 0x6AC690C5ULL + pSeed->nHighSeed;
    pSeed->nLowSeed  = static_cast<uint32_t>(uSignYStep);
    pSeed->nHighSeed = static_cast<uint32_t>(uSignYStep >> 32);
    if ((pSeed->nLowSeed & 1u) != 0)
    {
        nOffsetY = -nOffsetY;
    }

    const int32_t nPivotX = ENGINE_GetUnitX(pPlayer) + nOffsetX;
    const int32_t nPivotY = ENGINE_GetUnitY(pPlayer) + nOffsetY;
    const int32_t nSelfX  = ENGINE_GetUnitX(pUnit);
    const int32_t nSelfY  = ENGINE_GetUnitY(pUnit);

    PATH_SetNewDistance(pUnit->pDynamicPath, 90);
    CRITTER_MoveMonsterToCoords((nPivotX * 3) - (nSelfX * 2), (nPivotY * 3) - (nSelfY * 2), pUnit, MONMODE_NEUTRAL, 0);
}

// 1.12a: 0x6FAC6DC0 (custom: eax = pUnit, [esp+4] = pTarget, [esp+8] = uDist -> __fastcall)
void __fastcall CRITTER_UpdateCrabOrSpiderAI_9F(D2UnitStrc* pUnit, int32_t nUnusedEdx, D2UnitStrc* pTarget, uint32_t uDist)
{
    (void)nUnusedEdx;
    if (pUnit == nullptr || pUnit->dwAnimMode != MONMODE_NEUTRAL)
    {
        return;
    }

    if (uDist < 5u)
    {
        const uint64_t uStep = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
        pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uStep);
        pUnit->pSeed.nHighSeed = static_cast<uint32_t>(uStep >> 32);

        if ((pUnit->pSeed.nLowSeed % 100u) < 66u)
        {
            CRITTER_FleeAwayFromThreat(pTarget, 0, pUnit, 8, MONMODE_NEUTRAL, 30);
        }
        else
        {
            CRITTER_WanderRandomDiagonalOffset(pUnit, 12, MONMODE_NEUTRAL, 30);
        }
        return;
    }

    CRITTER_WanderRandomDiagonalOffset(pUnit, 9, MONMODE_NEUTRAL, 0);
}

// 1.12a: 0x6FAC6E30 (custom: eax = pUnit -> __fastcall)
void __fastcall CRITTER_UpdateChickenAI(D2UnitStrc* pUnit)
{
    if (pUnit == nullptr || pUnit->dwAnimMode != MONMODE_NEUTRAL)
    {
        return;
    }

    D2UnitStrc* pThreat = nullptr;
    int32_t nMinDist    = 0x7FFFFFFF;
    CRITTER_ForEachUnitInAdjacentRooms(pUnit, CRITTER_CallbackFindNearestThreat, &pThreat, &nMinDist);
    if (pThreat == nullptr)
    {
        return;
    }

    D2DynamicPathStrc* const pPath = pUnit->pDynamicPath;
    PATH_SetFootprintCollisionMask(pPath, 0x3C01u);

    if (nMinDist < 4)
    {
        PATH_SetFootprintCollisionMask(pPath, 0x0C01u);
        if (!CRITTER_FleeAwayFromThreat(pThreat, 0, pUnit, 4, 12, 120))
        {
            CRITTER_WanderRandomDiagonalOffset(pUnit, 4, 12, 120);
        }
        return;
    }

    const uint64_t uStep = static_cast<uint64_t>(pUnit->pSeed.nLowSeed) * 0x6AC690C5ULL + pUnit->pSeed.nHighSeed;
    pUnit->pSeed.nLowSeed  = static_cast<uint32_t>(uStep);
    pUnit->pSeed.nHighSeed = static_cast<uint32_t>(uStep >> 32);

    if ((pUnit->pSeed.nLowSeed % 100u) < 30u)
    {
        CRITTER_WanderRandomDiagonalOffset(pUnit, 2, MONMODE_NEUTRAL, 0);
        return;
    }

    D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
    if (pAiState != nullptr)
    {
        pAiState->nAiDelayFrames = 5;
    }
}

// 1.12a: 0x6FAC6F20 (custom: eax = pUnit -> __fastcall)
void __fastcall CRITTER_UpdateAmbientMonsterAI(D2UnitStrc* pUnit)
{
    D2ClientMonsterAiStateStrc* const pAiState = ENGINE_GetClientMonsterAiState(pUnit);
    if (pAiState == nullptr)
    {
        return;
    }

    if (pAiState->nAiDelayFrames > 0)
    {
        --pAiState->nAiDelayFrames;
        return;
    }

    int32_t nTargetDist = 0x7FFFFFFF;
    D2UnitStrc* pTarget = CRITTER_FindNearestActiveDecoy(&nTargetDist, pUnit);
    if (pTarget == nullptr)
    {
        pTarget = gpClientPlayer_6FBCC3D0;
        if (pTarget == nullptr)
        {
            pAiState->nAiDelayFrames = 25;
            CRITTER_EnterIdleNeutralMode(pUnit);
            return;
        }
    }

    if (nTargetDist > 30)
    {
        CRITTER_SetAiDelayFramesClamped(pUnit, nTargetDist - 15);
        CRITTER_EnterIdleNeutralMode(pUnit);
        return;
    }

    const int32_t nClassId = pUnit->dwClassId;
    if (MONSTER_CheckMonStatsFlag(nClassId, MONSTATSFLAGINDEX_ZOO))
    {
        CRITTER_UpdateZooMonsterAI(pUnit, pTarget);
        return;
    }

    if (nClassId >= 0x116 && nClassId <= 0x11A)
    {
        return;
    }

    switch (nClassId)
    {
    case 149: // Chicken
    case 268:
        CRITTER_UpdateChickenAI(pUnit);
        break;
    case 151: // Rat / Scavenger
    case 269:
    case 283:
    case 339:
        CRITTER_UpdateCarrionBirdOrScavengerAI(pTarget, pUnit, static_cast<uint32_t>(nTargetDist));
        break;
    case 157: // Vulture / Parrot
    case 158:
    case 319:
        CRITTER_UpdateFlyingVultureOrParrotAI(pUnit);
        break;
    case 159: // Crab / Spider
        CRITTER_UpdateCrabOrSpiderAI_9F(pUnit, 0, pTarget, static_cast<uint32_t>(nTargetDist));
        break;
    case 227: // Maggot / Bug
        CRITTER_UpdateMaggotOrBugAI_E3(pUnit, 0, pTarget);
        break;
    case 318: // Scorpion / Snake
        CRITTER_UpdateScorpionOrSnakeAI_13E(pTarget, pUnit, static_cast<uint32_t>(nTargetDist));
        break;
    case 556: // Act 5 Hare / Snow Rabbit
        CRITTER_UpdateCamelOrHareAI_22C(pUnit);
        break;
    case 574: // Bird Spawner
        CRITTER_UpdateBirdSpawnerMonster_23E(pUnit);
        break;
    default:
        CRITTER_EnterIdleNeutralMode(pUnit);
        break;
    }
}

// 1.12a: 0x6FAC7150 (custom: esi = pRoomSeed, [esp+4] = pRoom, [esp+8] = nMonsterClassId -> __fastcall)
void __fastcall DRLG_SpawnSingleClientCritterInRoom(D2SeedStrc* pRoomSeed, int32_t nUnusedEdx, D2ActiveRoomStrc* pRoom, int32_t nMonsterClassId)
{
    (void)nUnusedEdx;
    D2DrlgCoordsStrc tCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &tCoords);

    const int32_t nMaxRandX = tCoords.nSubtileWidth - 1;
    const int32_t nMaxRandY = tCoords.nSubtileHeight - 1;

    D2MonStats2Txt* const pMonStats2Txt = ENGINE_GetMonStats2TxtRecordFromMonsterId(nMonsterClassId);
    if (pMonStats2Txt == nullptr)
    {
        return;
    }

    const int32_t nSizeX = static_cast<int8_t>(pMonStats2Txt->nSizeX);
    int32_t nSpawnX  = 0;
    int32_t nSpawnY  = 0;
    int32_t nAttempt = 0;

    while (nAttempt < 10)
    {
        nSpawnX = tCoords.nSubtileX + ENGINE_RollRandomNumber(pRoomSeed, nMaxRandX);
        nSpawnY = tCoords.nSubtileY + ENGINE_RollRandomNumber(pRoomSeed, nMaxRandY);

        if (COLLISION_CheckMaskWithSize(pRoom, nSpawnX, nSpawnY, nSizeX, 0x3F11u) == 0)
        {
            break;
        }
        ++nAttempt;
    }

    if (nAttempt < 10)
    {
        uint32_t dwCritterGUID = gdwNextClientCritterGUID_6FBA6A80 + 1u;
        if (dwCritterGUID == 0xFFFFFFFFu)
        {
            dwCritterGUID = 0u;
        }
        gdwNextClientCritterGUID_6FBA6A80 = dwCritterGUID;

        D2MonsterSpawnDescStrc tSpawnDesc = {};
        tSpawnDesc.dwUnitGUID = dwCritterGUID;
        tSpawnDesc.nClassId   = nMonsterClassId;
        tSpawnDesc.nX         = nSpawnX;
        tSpawnDesc.nY         = nSpawnY;
        tSpawnDesc.nAnimMode  = MONMODE_NEUTRAL;

        UNIT_AllocClientMonsterUnit(&tSpawnDesc, nullptr, nullptr, 0, 0, 1);
    }
}

// 1.12a: 0x6FAC7320 (__stdcall: [esp+4] = pRoom)
void __stdcall DRLG_InitRoomClientCritters(D2ActiveRoomStrc* pRoom)
{
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    if (nLevelId == 0)
    {
        return;
    }

    D2LevelsTxt* const pLevelsTxt = DATATBLS_GetLevelsTxtRecord(nLevelId);
    D2SeedStrc* const pRoomSeed   = (pRoom != nullptr) ? &pRoom->pSeed : nullptr;

    for (int32_t nSlot = 0; nSlot < 4; ++nSlot)
    {
        const int16_t nCritterId = static_cast<int16_t>(pLevelsTxt->wCMon[nSlot]);
        if (nCritterId < 0)
        {
            break;
        }

        const uint64_t uStep = static_cast<uint64_t>(pRoomSeed->nLowSeed) * 0x6AC690C5ULL + pRoomSeed->nHighSeed;
        pRoomSeed->nLowSeed  = static_cast<uint32_t>(uStep);
        pRoomSeed->nHighSeed = static_cast<uint32_t>(uStep >> 32);

        const uint32_t uPctRoll = pRoomSeed->nLowSeed % 100u;
        const int32_t nSpawnPct = static_cast<int16_t>(pLevelsTxt->wCPct[nSlot]);
        if (uPctRoll < static_cast<uint32_t>(nSpawnPct))
        {
            D2MonStatsTxt* const pMonStatsTxt = ENGINE_GetMonStatsTxtRecord(nCritterId);
            const int32_t nGrpRange = static_cast<int32_t>(pMonStatsTxt->nMaxGrp) - static_cast<int32_t>(pMonStatsTxt->nMinGrp);
            int32_t nSpawnCount     = static_cast<int32_t>(pMonStatsTxt->nMinGrp) + ENGINE_RollRandomNumber(pRoomSeed, nGrpRange);

            const int16_t nAmountMul = static_cast<int16_t>(pLevelsTxt->wCAmt[nSlot]);
            if (nAmountMul != 0)
            {
                nSpawnCount *= static_cast<int32_t>(nAmountMul);
            }

            for (int32_t j = 0; j < nSpawnCount; ++j)
            {
                DRLG_SpawnSingleClientCritterInRoom(pRoomSeed, 0, pRoom, nCritterId);
            }
        }
    }
}

// =============================================================================
// SECTION 7: Smacker/Bink Cutscene Playback & UI Blocking State Queries
// VA Range: 0x6FAC7480 .. 0x6FAC7CD8 (11 functions)
// =============================================================================

// 1.12a: 0x6FAC7480 (__stdcall)
uint8_t __stdcall CUTSCENE_IsInPlayback(void)
{
    return gbInCutscenePlayback_6FBCB6CF;
}

// 1.12a: 0x6FAC7490 (__stdcall)
void __stdcall CUTSCENE_RequestChainAct5IntroVideo(void)
{
    gbChainAct5IntroVideo_6FBCB6D0 = 1;
}

// 1.12a: 0x6FAC74A0 (__stdcall)
void __stdcall CUTSCENE_InitializeVideoState(void)
{
    gbCutsceneFromMainMenu_6FBCB6CC = 0;
    gbHighResCutscene_6FBCB6CE      = (FOG_10191_IsHighResVideoSupported() != 0) ? 1 : 0;
    gbChainAct5IntroVideo_6FBCB6D0  = 0;
    gbInCutscenePlayback_6FBCB6CF   = 0;
}

// 1.12a: 0x6FAC74D0 (__stdcall)
void __stdcall CUTSCENE_SetFromMainMenuFlag(void)
{
    gbCutsceneFromMainMenu_6FBCB6CC = 1;
}

// 1.12a: 0x6FAC74E0 (__fastcall: ecx = szFilePath)
BOOL __fastcall CUTSCENE_CheckMpqFileExists(const char* szFilePath)
{
    HANDLE hFile = nullptr;
    if (!SFileOpenFileEx(0, const_cast<char*>(szFilePath), 0, reinterpret_cast<int>(&hFile)))
    {
        return FALSE;
    }
    SFileCloseFile(hFile);
    return TRUE;
}

// 1.12a: 0x6FAC7510 (custom: al = uOctet -> __fastcall)
uint8_t __fastcall CUTSCENE_DecodeAuxBnetActUnlockRank(uint8_t uOctet)
{
    if ((uOctet & 0x01u) != 0)
    {
        return 7;
    }
    if ((uOctet & 0x80u) != 0)
    {
        return 6;
    }
    if ((uOctet & 0x10u) != 0)
    {
        return 5;
    }
    if ((uOctet & 0x08u) != 0)
    {
        return 4;
    }
    if ((uOctet & 0x40u) != 0)
    {
        return 3;
    }
    return ((uOctet & 0x04u) != 0) ? 2 : 1;
}

// 1.12a: 0x6FAC7540 (custom: esi = szOutPath, ebx = szFormat, edi = szVideoFileName -> __fastcall)
void __fastcall CUTSCENE_ResolveLocalizedVideoPath(char* szOutPath, const char* szFormat, const char* szVideoFileName)
{
    char szLangCode[8] = {};
    STRTABLE_GetLanguageInfo(szLangCode, 0);
    std::sprintf(szOutPath, szFormat, gszDataLocal_6FB83C98, szLangCode, szVideoFileName);

    HANDLE hFile = nullptr;
    if (SFileOpenFileEx(0, szOutPath, 0, reinterpret_cast<int>(&hFile)))
    {
        SFileCloseFile(hFile);
        return;
    }

    const char* const szFallbackLang = (STRTABLE_GetLanguage() != LANGUAGE_ENGLISH) ? gszLangEng_6FB86D5C : gszLangChi_6FB86D58;
    std::sprintf(szOutPath, szFormat, gszDataLocal_6FB83C98, szFallbackLang, szVideoFileName);
    if (SFileOpenFileEx(0, szOutPath, 0, reinterpret_cast<int>(&hFile)))
    {
        SFileCloseFile(hFile);
    }
}

// 1.12a: 0x6FAC7600 (custom: eax = nCutsceneIndex -> __fastcall)
void __fastcall CUTSCENE_UpdateAuxBnetRegistry(int32_t nCutsceneIndex)
{
    const int32_t nCutsceneId = gCutsceneTable_6FBA8A68[nCutsceneIndex].nCutsceneId;

    char szAuxIp[100] = {};
    int32_t nOctet0 = 0;
    int32_t nOctet1 = 0;
    int32_t nOctet2 = 0;
    int32_t nOctet3 = 0x22;

    uint8_t uSavedRank = 0x22u;
    if (SRegLoadString(gszDiabloII_6FB803FC, gszAuxBnetKey_6FB86D6C, 0, szAuxIp, 100))
    {
        std::sscanf(szAuxIp, gszIpAddrFmt_6FB86D60, &nOctet0, &nOctet1, &nOctet2, &nOctet3);
        uSavedRank = CUTSCENE_DecodeAuxBnetActUnlockRank(static_cast<uint8_t>(nOctet3));
    }

    uint8_t uCutsceneRank = 1u;
    switch (nCutsceneId)
    {
    case 2:
        uCutsceneRank = 2u;
        break;
    case 3:
        uCutsceneRank = 3u;
        break;
    case 4:
        uCutsceneRank = 4u;
        break;
    case 5:
    case 6:
        uCutsceneRank = 5u;
        break;
    case 7:
        uCutsceneRank = 7u;
        break;
    default:
        uCutsceneRank = 1u;
        break;
    }

    if (uCutsceneRank > uSavedRank)
    {
        int32_t nEncodedOctet = 0x22;
        switch (uCutsceneRank)
        {
        case 2:
            nEncodedOctet = 0x26;
            break;
        case 3:
            nEncodedOctet = 0x62;
            break;
        case 4:
            nEncodedOctet = 0x2A;
            break;
        case 5:
            nEncodedOctet = 0xB2;
            break;
        case 6:
            nEncodedOctet = 0x22;
            break;
        case 7:
            nEncodedOctet = 0x23;
            break;
        default:
            nEncodedOctet = 0x22;
            break;
        }

        std::sprintf(szAuxIp, gszIpAddrFmt_6FB86D60, 0xD8, 0x94, 0xF6, nEncodedOctet);
        SRegSaveString(gszDiabloII_6FB803FC, gszAuxBnetKey_6FB86D6C, 0, szAuxIp);
    }
}

// 1.12a: 0x6FAC7770 (__stdcall: [esp+4] = nCutsceneIndex)
void __stdcall CUTSCENE_PlayCutscene(int32_t nCutsceneIndex)
{
    using D2WinReadyForVideoFn = BOOL (__stdcall*)(void);
    const D2WinReadyForVideoFn pfnPrepareD2WinVideo = reinterpret_cast<D2WinReadyForVideoFn>(D2Win_10003);

    if (nCutsceneIndex >= gnCutsceneCount_6FBA8AC8)
    {
        ENGINE_FatalHalt(0xEF);
    }

    gbInCutscenePlayback_6FBCB6CF = 1;
    CUTSCENE_UpdateAuxBnetRegistry(nCutsceneIndex);

    if (gbCutsceneActive_6FBCB6CD != 0)
    {
        return;
    }

    if (nCutsceneIndex >= gnCutsceneCount_6FBA8AC8)
    {
        const uint8_t bEndCutscenePacket = 0x48u;
        MSG_SendRawClientPacket(sizeof(bEndCutscenePacket), &bEndCutscenePacket);
        gbInCutscenePlayback_6FBCB6CF = 0;
        return;
    }

    D2GameResolutionMode eVideoResMode = D2GAMERES_640x480; // 0
    char szVideoFileName[260] = {};
    const D2CutsceneEntryStrc& rEntry = gCutsceneTable_6FBA8A68[nCutsceneIndex];

    if (rEntry.bResolutionFmt != 0)
    {
        if (gbHighResCutscene_6FBCB6CE != 0)
        {
            std::sprintf(szVideoFileName, rEntry.szFileFormat, gszRes640x292_6FB86D50);
        }
        else
        {
            eVideoResMode = D2GAMERES_800x600; // 2
            std::sprintf(szVideoFileName, rEntry.szFileFormat, gszRes640x146_6FB86D48);
        }
    }
    else
    {
        std::strcpy(szVideoFileName, rEntry.szFileFormat);
    }

    GAME_PrepareForCutscene();
    SOUND_StopAllForCutscene();
    VIEW_ResetScreenShake();
    ARCHIVE_UnloadExpansionArchives();

    BOOL bVideoArchivesLoaded = FALSE;
    BOOL bSmackerOpened       = FALSE;
    BOOL bIsExpansionVideo    = FALSE;

    if (nCutsceneIndex == 7)
    {
        bIsExpansionVideo = TRUE;
        char szGameDllPath[MAX_PATH] = {};
        FOG_GetInstallPath(szGameDllPath, MAX_PATH);
        std::strcat(szGameDllPath, gszD2GameDll_6FB86D3C);

        const HANDLE hDllFile = CreateFileA(
            szGameDllPath,
            0,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);

        if (hDllFile == INVALID_HANDLE_VALUE)
        {
            gbMissingExpansionGameDll_6FBCC010 = 0;
            gbExpansionCutsceneError_6FBCC014  = 1;
            gbCutsceneFromMainMenu_6FBCB6CC    = 0;
            GAME_OnCutsceneCompleted();
            ARCHIVE_UnloadExpansionArchives();
            ARCHIVE_LoadExpansionArchives(ARCHIVE_PromptExpansionDiscA, ARCHIVE_PromptExpansionDiscB, nullptr, gpD2Config_6FBCB908);
            gbInCutscenePlayback_6FBCB6CF = 0;
            return;
        }
        CloseHandle(hDllFile);
    }
    else if (nCutsceneIndex == 6)
    {
        bIsExpansionVideo = TRUE;
    }

    gbPlayingExpansionVideo_6FBC2790 = bIsExpansionVideo;
    const ARCHIVE_ShowMessageFunctionPtr pfnPromptDisc =
        bIsExpansionVideo ? ARCHIVE_PromptExpansionDiscA : ARCHIVE_PromptClassicVideoDisc;

    char szFullVideoPath[260] = {};
    if (ARCHIVE_LoadVideoArchives(pfnPromptDisc, nullptr, bIsExpansionVideo))
    {
        bVideoArchivesLoaded = TRUE;
        if (rEntry.bLocalizedPath != 0)
        {
            CUTSCENE_ResolveLocalizedVideoPath(szFullVideoPath, gszVideoLocalFmt_6FB86D2C, szVideoFileName);
        }
        else
        {
            std::sprintf(szFullVideoPath, gszVideoGlobalFmt_6FB86D20, gszDataGlobal_6FB80424, szVideoFileName);
        }

        const BOOL bPrevSavedPanelFlag = gbCutsceneSavedPanelFlag_6FBB2BA0;
        gbCutsceneActive_6FBCB6CD      = 1;
        gnCutsceneFadeClear_6FBCC34C   = 0;

        FOG_IsExpansion();
        if (gnPanelDescriptorCount_6FB8F368 != 0x26)
        {
            ENGINE_FatalHalt(0x1389);
        }

        if (!gbEscMenuOpen_6FBB5490)
        {
            bool bAbortedPanelClose = false;
            for (int32_t i = 0; i < gnPanelDescriptorCount_6FB8F368; ++i)
            {
                if (gabPanelOpenStates_6FBB2B58[i] == 1)
                {
                    const int32_t nCloseMode = gpPanelDescriptors_6FB8F2D0[i]->nCutsceneCloseMode;
                    if (nCloseMode == 0)
                    {
                        continue;
                    }
                    if (nCloseMode == 1)
                    {
                        UI_SetPanelState(i, 1, 0);
                    }
                    else if (nCloseMode == 4)
                    {
                        if (gbActiveNpcInteract_6FBCB629)
                        {
                            NPCMENU_CancelActiveInteraction(gdwActiveNpcGUID_6FBCB625);
                        }
                    }
                    else
                    {
                        bAbortedPanelClose = true;
                        break;
                    }
                }
            }

            if (!bAbortedPanelClose)
            {
                if (gpClientPlayer_6FBCC3D0 == nullptr ||
                    (gpClientPlayer_6FBCC3D0->dwUnitType == UNIT_PLAYER && !UNIT_IsUnitDeadOrCorpse(gpClientPlayer_6FBCC3D0)))
                {
                    gbCutsceneSavedPanelFlag_6FBB2BA0 = TRUE;
                }

                if (bPrevSavedPanelFlag && !gbCutsceneSavedPanelFlag_6FBB2BA0)
                {
                    UI_OnPanelClosedRefresh();
                }
            }
        }

        if (pfnPrepareD2WinVideo())
        {
            void (__cdecl* const pfnFrameKeepAlive)(void) = (gbCutsceneFromMainMenu_6FBCB6CC != 0)
                ? nullptr
                : reinterpret_cast<void (__cdecl*)(void)>(GAME_CutsceneKeepAliveCallback);
            WINDOW_OpenSmackCutscene(szFullVideoPath, eVideoResMode, pfnFrameKeepAlive);
            bSmackerOpened = TRUE;
        }

        if (!(nCutsceneIndex == 5 && gbChainAct5IntroVideo_6FBCB6D0 != 0))
        {
            D2Win_10004(D2GFX_GetResolutionMode());
            const BOOL bWasSavedFlag = gbCutsceneSavedPanelFlag_6FBB2BA0;
            FOG_IsExpansion();
            if (gnPanelDescriptorCount_6FB8F368 != 0x26)
            {
                ENGINE_FatalHalt(0x1389);
            }
            gbCutsceneSavedPanelFlag_6FBB2BA0 = FALSE;
            if (bWasSavedFlag)
            {
                UI_OnPanelClosedRefresh();
            }
            gbCutsceneActive_6FBCB6CD = 0;
        }
    }

    if (gbChainAct5IntroVideo_6FBCB6D0 != 0)
    {
        if (bVideoArchivesLoaded)
        {
            ARCHIVE_UnloadExpansionArchives();
        }
        if (bSmackerOpened)
        {
            D2Win_10004(D2GFX_GetResolutionMode());
        }

        gbPlayingExpansionVideo_6FBC2790 = TRUE;
        if (ARCHIVE_LoadVideoArchives(ARCHIVE_PromptExpansionDiscA, nullptr, TRUE))
        {
            nCutsceneIndex = 6;
            const D2CutsceneEntryStrc& rAct5Entry = gCutsceneTable_6FBA8A68[6];
            if (gbHighResCutscene_6FBCB6CE != 0)
            {
                std::sprintf(szVideoFileName, rAct5Entry.szFileFormat, gszRes640x292_6FB86D50);
            }
            else
            {
                eVideoResMode = D2GAMERES_800x600;
                std::sprintf(szVideoFileName, rAct5Entry.szFileFormat, gszRes640x146_6FB86D48);
            }

            if (rAct5Entry.bLocalizedPath != 0)
            {
                CUTSCENE_ResolveLocalizedVideoPath(szFullVideoPath, gszVideoLocalFmt_6FB86D2C, szVideoFileName);
            }
            else
            {
                std::sprintf(szFullVideoPath, gszVideoGlobalFmt_6FB86D20, gszDataGlobal_6FB80424, szVideoFileName);
            }

            if (pfnPrepareD2WinVideo())
            {
                WINDOW_OpenSmackCutscene(
                    szFullVideoPath,
                    eVideoResMode,
                    reinterpret_cast<void (__cdecl*)(void)>(GAME_CutsceneKeepAliveCallback));
                CUTSCENE_UpdateAuxBnetRegistry(6);
            }
        }

        D2Win_10004(D2GFX_GetResolutionMode());
        UI_SetPanelState(18, 1, 0);
        gbCutsceneActive_6FBCB6CD      = 0;
        gbChainAct5IntroVideo_6FBCB6D0 = 0;
        D2Win_10177(ACT_V);
    }

    MSG_SendByte(0x48u);

    if (gbCutsceneFromMainMenu_6FBCB6CC != 0)
    {
        gbCutsceneFromMainMenu_6FBCB6CC = 0;
        GAME_OnCutsceneCompleted();
    }
    else if (nCutsceneIndex == 7)
    {
        gbMissingExpansionGameDll_6FBCC010 = 0;
        gbExpansionCutsceneError_6FBCC014  = 1;
        gbCutsceneFromMainMenu_6FBCB6CC    = 0;
        GAME_OnCutsceneCompleted();
    }

    ARCHIVE_UnloadExpansionArchives();
    ARCHIVE_LoadExpansionArchives(ARCHIVE_PromptExpansionDiscA, ARCHIVE_PromptExpansionDiscB, nullptr, gpD2Config_6FBCB908);
    gbInCutscenePlayback_6FBCB6CF = 0;
}

// 1.12a: 0x6FAC7C80 (__stdcall)
int32_t __stdcall WINMAIN_GetMenuStateValue(void)
{
    return gnMenuStateValue_6FBCB6AC;
}

// 1.12a: 0x6FAC7C90 (__stdcall)
BOOL __stdcall WINMAIN_IsAnyUIBlockingStateActive(void)
{
    for (int32_t i = 0; i < 7; ++i)
    {
        if (ganUIBlockingStates_6FBCB65F[i] != 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// =============================================================================
// SECTION 8: D2Client DLL Exports, DllMain, QueryInterface, & Skill State Helpers
// VA Range: 0x6FB4E940 .. 0x6FB53650 (9 functions)
// =============================================================================

#ifdef D2CLIENT_PURE_WINMAIN_EXPORTS
// 1.12a: 0x6FB4E940 (__stdcall; Export @10002 D2Client_10002)
extern "C" D2CLIENT_DLL_DECL uint8_t __stdcall D2Client_10002(int32_t nUnused, int32_t nIndex)
{
    (void)nUnused;
    if (gnServerWorldStateInit_6FBCBCE0 == 0)
    {
        ENGINE_FatalHalt(0x60);
    }
    if (nIndex >= 37)
    {
        ENGINE_FatalHalt(0x65);
    }
    return gabServerWorldFlags_6FBB1678[nIndex];
}

// 1.12a: 0x6FB4E980 (__stdcall; Export @10001 D2Client_10001)
extern "C" D2CLIENT_DLL_DECL int32_t __stdcall D2Client_10001(int32_t nUnused)
{
    (void)nUnused;
    return gnServerWorldStateInit_6FBCBCE0;
}

#endif // D2CLIENT_PURE_WINMAIN_EXPORTS

// 1.12a: 0x6FB4E990 (custom: eax = pPacket -> __fastcall)
void __fastcall GAME_HandleServerWorldFlagsPacket(const uint8_t* pPacket)
{
    std::memcpy(gabServerWorldFlags_6FBB1678, pPacket + 1, 37);
    gnServerWorldStateInit_6FBCBCE0 = 1;
}

#ifdef D2CLIENT_PURE_WINMAIN_EXPORTS
// 1.12a: 0x6FB535D0 (__stdcall; Export @10004 QueryInterface)
extern "C" D2CLIENT_DLL_DECL D2ClientInterfaceStrc* __stdcall QueryInterface(void)
{
    return &gD2ClientInterface_6FB8D020;
}

// 1.12a: 0x6FB535E0 (WINAPI / __stdcall)
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    (void)fdwReason;
    (void)lpvReserved;
    ghD2ClientInstance_6FBCBCC0 = hinstDLL;
    return TRUE;
}

#endif // D2CLIENT_PURE_WINMAIN_EXPORTS

// 1.12a: 0x6FB53600 (__fastcall: ecx = pConfig)
int32_t __fastcall D2ClientInterface_Entry(D2ConfigStrc* pConfig)
{
    return WINMAIN_RunClient(pConfig, ghD2ClientInstance_6FBCBCC0);
}

// 1.12a: 0x6FB53610 (custom: esi = pUnit, eax = pSkill -> __fastcall)
int32_t __fastcall WINMAIN_GetPlayerSkillUseState(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
    const int32_t nUseState = SKILLS_GetUseState(pUnit, pSkill);
    if (nUseState != 0)
    {
        return nUseState;
    }

    if (pUnit == gpClientPlayer_6FBCC3D0 && gdwClientGameFrame_6FBB32B0 < gdwSkillCooldownEndFrame_6FBB3314)
    {
        return 8;
    }
    return 0;
}

// 1.12a: 0x6FB53640 (__stdcall)
void __stdcall WINMAIN_ResetPlayerSkillCooldownTick(void)
{
    gdwSkillCooldownEndFrame_6FBB3314 = 0;
}

// 1.12a: 0x6FB53650 (__fastcall: ecx = pUnit)
void __fastcall WINMAIN_RefreshPassiveSkills(D2UnitStrc* pUnit)
{
    SKILLS_RefreshPassiveSkills(pUnit);
}


// =============================================================================
// Cross-TU VA-Mapped Adapters for CORE_WinMain.cpp
// =============================================================================
extern D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileDefault(const char* szFileName, int32_t nFileType);
extern D2CellFileStrc* __fastcall ARCHIVE_LoadCellFile(const char* szFileName, int32_t nFileType, int32_t bNoLocale);
extern BOOL __cdecl MODAL_RunOkCancelDialog(void);
extern int32_t __stdcall GAME_LeaveSinglePlayerServer(void);
extern int32_t __stdcall GAME_MainLoop(void);
extern int32_t __stdcall GAME_WaitJoinScreen(void);
extern int32_t __stdcall GAME_LeaveGameCleanupState(void);
extern void __stdcall NET_InitializeSessionByGameType(void);
extern void __stdcall NET_ShutdownClientAndServer(void);
extern void __fastcall MSG_SendPing_6D(void);
extern void __fastcall RECORD_LoadFromFile(void);
extern void __stdcall GAME_RequestExitGame(void);
extern void __fastcall AUTOMAP_CenterView(int32_t bForceCenter);
extern int32_t __fastcall MSG_SendPacket(void* pPacketData, size_t nSize);
extern void __cdecl sub_6FACA1F0(void);
extern int32_t __fastcall D2Client_PlaySound(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nTicksDelay, BOOL bLoop, uint32_t dwFlags);
extern void __fastcall D2Client_SetSoundChannelWorldCoords(int32_t nWorldX, uint32_t dwHandle, int32_t nWorldY);
extern void __stdcall SPINNER_Impl_Destructor(int32_t* pThis);
extern void __fastcall SPINNER_Proxy_Destructor(void* pThis);
extern void __fastcall UI_StartPanelCloseClickCooldown(void);
extern void __stdcall UI_CloseMercInteract(void);
extern void __fastcall CURSOR_WarpMousePosition(int32_t nNewMouseX, int32_t nNewMouseY);
extern void __fastcall D2Client_ExecuteMonsterModeTransition(D2UnitStrc* pUnit, int32_t nNewMode, int32_t nTargetX, int32_t nTargetY);
extern D2UnitStrc* __stdcall D2Client_AllocClientMonsterUnit(const D2MonsterSpawnDescStrc* pSpawnDesc, const uint8_t* pCompInfo, const uint8_t* pMonUMods, uint32_t dwNameSeedOrAsync, uint16_t wBossHcIdx, int32_t nExtra);
extern D2UnitStrc* __fastcall D2Client_CreateClientSideUnit(int32_t nX, int32_t nY, int32_t nClassId, uint32_t nUnitType, int32_t nMode);
extern D2UnitStrc* __fastcall D2Client_GetUnitOwner(D2UnitStrc* pUnit);
extern int __fastcall sub_6FACEAC0(D2UnitStrc* pUnit);
extern void __fastcall D2Client_RemoveUnitFromHashTable(D2UnitStrc** ppHashTable, uint32_t nBucket, uint32_t dwUnitGUID, uint32_t nUnitType);
extern void __fastcall VIEW_SetSkewMode(int32_t nSkewMode);
extern void __fastcall VIEW_SetViewportRect(D2ViewportStrc* pViewport, int32_t nLeft, int32_t nTop, int32_t nRight, int32_t nBottom);
extern D2UnitStrc* __fastcall D2Client_SpawnClientMissileAtCoords(D2UnitStrc* pOwner, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkillId, int32_t nSkillLevel);
extern void* __fastcall D2Client_GetMonsterDataField34Ptr(D2UnitStrc* pUnit);
extern LRESULT __stdcall WALL2_MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
struct D2SMsgRegEntryStrc;
extern void __fastcall WALL2_RegisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries);
extern void __fastcall WALL2_UnregisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries);
struct D2UnitLightViewStrc;
extern void __stdcall PARTICLE_UpdateUnitPhysics(D2UnitLightViewStrc* pUnit);
extern void __stdcall PARTICLE_SetPosition(D2UnitLightViewStrc* pUnit, int32_t nPosX, int32_t nPosY, int32_t nPosZ);
extern void __fastcall PARTICLE_AllocForUnit(D2UnitLightViewStrc* pUnit);
extern void __stdcall LIGHTMAP_GetEnvironmentLight(BOOL bCheckSpecial, D2ActiveRoomStrc* pRoom, int32_t* pOutInt, uint8_t* pOutR, uint8_t* pOutG, uint8_t* pOutB);

BOOL __stdcall ARCHIVE_InitClientStartupCallback(void) { return TRUE; }
BOOL __stdcall ARCHIVE_InitializeClientArchives(void) { return TRUE; }
void __stdcall ARCHIVE_ShutdownClientArchives(void) {}
D2CellFileStrc* __fastcall ARCHIVE_LoadClientCellFile(const char* szFileName, int32_t nFileType) { return ARCHIVE_LoadCellFileDefault(szFileName, nFileType); }
D2CellFileStrc* __fastcall ARCHIVE_LoadClientUncompressedCellFile(const char* szFileName, int32_t* pOutFileType, int32_t bNoLocale) { return ARCHIVE_LoadCellFile(szFileName, pOutFileType ? *pOutFileType : 0, bNoLocale); }
BOOL __stdcall ARCHIVE_PromptClassicVideoDisc(void) { return MODAL_RunOkCancelDialog(); }
BOOL __stdcall ARCHIVE_PromptExpansionDiscA(void) { return TRUE; }
BOOL __stdcall ARCHIVE_PromptExpansionDiscB(void) { return TRUE; }
void __stdcall CLIENT_CleanupRegistryState(void) { GAME_LeaveSinglePlayerServer(); }
int32_t __stdcall CLIENT_StateHandler_ExitOrLeave_6FB2D2D0(void) { return GAME_MainLoop(); }
int32_t __stdcall CLIENT_StateHandler_GameLoop_6FB2A690(void) { return GAME_WaitJoinScreen(); }
int32_t __stdcall CLIENT_StateHandler_InitSession_6FB2BC80(void) { return GAME_LeaveGameCleanupState(); }
int32_t __stdcall CLIENT_StateHandler_PrepareNet_6FAEBBF0(void) { NET_InitializeSessionByGameType(); return 1; }
int32_t __stdcall CLIENT_StateHandler_StartNet_6FAEBBB0(void) { NET_ShutdownClientAndServer(); return 1; }
BOOL __stdcall GAME_CutsceneKeepAliveCallback(void) { MSG_SendPing_6D(); return TRUE; }
void __stdcall GAME_InitRandomSeed(void) { RECORD_LoadFromFile(); }
void __stdcall GAME_OnCutsceneCompleted(void) { GAME_RequestExitGame(); }
void __stdcall GAME_PrepareForCutscene(void) {}
D2UnitStrc* __stdcall INV_GetCursorItem(void) { return (gpPlayerUnit && gpPlayerUnit->pInventory) ? INVENTORY_GetCursorItem(gpPlayerUnit->pInventory) : nullptr; }
void __fastcall INV_OnResolutionModeChanged(int32_t nMode) { (void)nMode; AUTOMAP_CenterView(1); }
void __fastcall LIGHTMAP_QueryBaseAmbientRGB(BOOL bCheckSpecial, D2ActiveRoomStrc* pRoom, int32_t* pOutIntensity, uint8_t* pOutRed, uint8_t* pOutGreen, uint8_t* pOutBlue)
{
    if (pOutIntensity) *pOutIntensity = 255;
    if (pOutRed) *pOutRed = 255;
    if (pOutGreen) *pOutGreen = 255;
    if (pOutBlue) *pOutBlue = 255;
    (void)bCheckSpecial; (void)pRoom;
}
void __stdcall MISSILE_FinalizeCritterFlight(D2UnitStrc* pUnit) { PARTICLE_UpdateUnitPhysics(reinterpret_cast<D2UnitLightViewStrc*>(pUnit)); }
void __fastcall MISSILE_InitBallisticVelocity(D2UnitStrc* pMissile, int32_t nVelParam, int32_t nArg2, int32_t nArg3) { PARTICLE_SetPosition(reinterpret_cast<D2UnitLightViewStrc*>(pMissile), nVelParam, nArg2, nArg3); }
void __fastcall MISSILE_InitShenkCatapultProjectile(D2UnitStrc* pMissile) { PARTICLE_AllocForUnit(reinterpret_cast<D2UnitLightViewStrc*>(pMissile)); }
D2UnitStrc* __fastcall MISSILE_SpawnClientEffectAtCoords(int32_t nUnusedEcx, D2UnitStrc* pOwner, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkill, int32_t nSkillLevel)
{
    (void)nUnusedEcx;
    return D2Client_SpawnClientMissileAtCoords(pOwner, nMissileId, nX, nY, nSkill, nSkillLevel);
}
BOOL __fastcall MONSTER_CheckMonStatsFlag(int32_t nMonsterId, int32_t nFlagIndex)
{
    if (nMonsterId < 0 || nFlagIndex < 0 || nFlagIndex >= 32 || !sgptDataTables || nMonsterId >= sgptDataTables->nMonStatsTxtRecordCount) return FALSE;
    return (sgptDataTables->pMonStatsTxt[nMonsterId].dwMonStatsFlags & gdwBitMasks[nFlagIndex]) != 0;
}
int32_t* __fastcall MONSTER_GetClientIdleChatterState(D2UnitStrc* pUnit, D2SeedStrc* pSeed) { (void)pSeed; return reinterpret_cast<int32_t*>(D2Client_GetMonsterDataField34Ptr(pUnit)); }
LRESULT CALLBACK MSG_MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return WALL2_MainWindowProc(hWnd, uMsg, wParam, lParam); }
void __fastcall MSG_RegisterPanelHandlers(int32_t nCount, HWND hWnd, const D2WaypointPanelMsgEntryStrc* pEntries) { WALL2_RegisterMsgHandlerTable(static_cast<uint32_t>(nCount), hWnd, reinterpret_cast<const D2SMsgRegEntryStrc*>(pEntries)); }
void __fastcall MSG_UnregisterPanelHandlers(int32_t nCount, HWND hWnd, const D2WaypointPanelMsgEntryStrc* pEntries) { WALL2_UnregisterMsgHandlerTable(static_cast<uint32_t>(nCount), hWnd, reinterpret_cast<const D2SMsgRegEntryStrc*>(pEntries)); }
int32_t __fastcall MSG_SendRawClientPacket(size_t nBytes, const void* pPacket) { return MSG_SendPacket(const_cast<void*>(pPacket), nBytes); }
void __fastcall NPCMENU_CancelActiveInteraction(uint32_t dwNpcGUID) { (void)dwNpcGUID; sub_6FACA1F0(); }
void __stdcall QUEST_TriggerNihlathakTauntSound(int32_t nSoundId, int32_t nArg2, int32_t nArg3) { (void)nArg2; (void)nArg3; D2Client_PlaySound(nSoundId, gpPlayerUnit, 0, FALSE, 0); }
void* __fastcall SOUND_PlayClientSoundEx(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nDelay, int32_t bLoop, int32_t dwFlags) { D2Client_PlaySound(nSoundId, pUnit, nDelay, bLoop, static_cast<uint32_t>(dwFlags)); return nullptr; }
void __fastcall SOUND_PlayPositionalEventSound(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t a5, int32_t a6, int32_t a7) { (void)nX; (void)nY; (void)a5; (void)a6; (void)a7; D2Client_PlaySound(nSoundId, pUnit ? pUnit : gpPlayerUnit, 0, FALSE, 0); }
void __fastcall SOUND_SetChannelPositionOffset(void* pChan, int32_t nX, int32_t nY, int32_t nZ) { (void)pChan; (void)nZ; D2Client_SetSoundChannelWorldCoords(nX, 0, nY); }
void __stdcall SOUND_StopAllForCutscene(void) {}
void __fastcall SOUND_TriggerBossEventAudio(int32_t nAudioId, int32_t bFlag) { (void)bFlag; D2Client_PlaySound(nAudioId, gpPlayerUnit, 0, FALSE, 0); }
void __stdcall UI_DestroyWidgetA_6FB6CC90(void* pWidget) { SPINNER_Impl_Destructor(reinterpret_cast<int32_t*>(pWidget)); }
void __fastcall UI_DestroyWidgetB_6FB6C8A0(void* pWidget) { SPINNER_Proxy_Destructor(pWidget); }
void __stdcall UI_DestroyWidgetC_6FB68440(void* pWidget) { (void)pWidget; }
void __fastcall UI_DestroyWidgetD_6FB682F0(void* pWidget) { (void)pWidget; }
void __stdcall UI_DestroyWidgetE_6FB4B740(void* pWidget) { (void)pWidget; }
void __fastcall UI_DestroyWidgetF_6FB4B3C0(void* pWidget) { (void)pWidget; }
void __stdcall UI_DestroyWidgetG_6FB34B30(void* pWidget) { (void)pWidget; }
void __fastcall UI_DestroyWidgetH_6FB347D0(void* pWidget) { (void)pWidget; }
void __stdcall UI_OnPanelClosedRefresh(void) { UI_StartPanelCloseClickCooldown(); }
void __stdcall UI_OnResolutionModeChanged(void) { UI_CloseMercInteract(); }
void __fastcall UI_SetCursorPosition(int32_t nX, int32_t nY) { CURSOR_WarpMousePosition(nX, nY); }
void __fastcall UNITMODE_StartClientMonsterMode(int32_t nMode, D2UnitStrc* pUnit, D2ClientMonsterMoveCmdStrc* pCmd)
{
    D2Client_ExecuteMonsterModeTransition(pUnit, nMode, pCmd ? pCmd->nTargetX : 0, pCmd ? pCmd->nTargetY : 0);
}
D2UnitStrc* __stdcall UNIT_AllocClientMonsterUnit(const D2MonsterSpawnDescStrc* pSpawnDesc, const uint8_t* pCompInfo, const uint8_t* pMonUMods, uint32_t dwNameSeedOrAsync, uint16_t wBossHcIdx, int32_t nIsClientAsync)
{
    return D2Client_AllocClientMonsterUnit(pSpawnDesc, pCompInfo, pMonUMods, dwNameSeedOrAsync, wBossHcIdx, nIsClientAsync);
}
D2UnitStrc* __fastcall UNIT_CreateClientUnitAtCoords(int32_t nClassId, int32_t nX, int32_t nY, int32_t nUnitType, int32_t nAnimMode)
{
    return D2Client_CreateClientSideUnit(nX, nY, nClassId, static_cast<uint32_t>(nUnitType), nAnimMode);
}
D2UnitStrc* __fastcall UNIT_GetMorphedSourceUnit(D2UnitStrc* pUnit) { return D2Client_GetUnitOwner(pUnit); }
BOOL __fastcall UNIT_IsUnitDeadOrCorpse(D2UnitStrc* pUnit) { return sub_6FACEAC0(pUnit); }
void __fastcall UNIT_RemoveClientSideUnitFromHash(D2UnitStrc** ppHashTable, uint32_t nBucket, uint32_t dwUnitId, int32_t nUnitType)
{
    D2Client_RemoveUnitFromHashTable(ppHashTable, static_cast<uint32_t>(nBucket), dwUnitId, static_cast<uint32_t>(nUnitType));
}
void __fastcall VIEW_ApplyResolutionViewShift(int32_t nMode) { VIEW_SetSkewMode(nMode); }
void __fastcall VIEW_SetViewportBounds(D2ViewportStrc* pView, int32_t l, int32_t t, int32_t r, int32_t b) { VIEW_SetViewportRect(pView, l, t, r, b); }
void __stdcall VIEW_StartBossQuakeAndTaunt(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; }
int32_t __fastcall WAYGATE_HitTestEntry(int32_t nLocalX, int32_t nLocalY)
{
    if (nLocalX < 36 || nLocalX > 284 || nLocalY < 0 || nLocalY >= 9 * 36) return -1;
    return nLocalY / 36;
}
// Provide ENGINE_FindRoomBySubtileCoords alias to 0x6FAB1A00 ENGINE_FindRoomAtSubtileCoords
D2ActiveRoomStrc* __fastcall ENGINE_FindRoomBySubtileCoords(D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY)
{
    return ENGINE_FindRoomAtSubtileCoords(pRoom, nX, nY);
}
