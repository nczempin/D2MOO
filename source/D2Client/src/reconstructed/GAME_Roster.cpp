// =============================================================================
// D2Client.dll 1.12a (ImageBase = 0x6FAB0000)
// Reconstructed Translation Units:
//   - GAME/Roster.cpp     (0x6FB27890 .. 0x6FB29160, 75 functions)
//   - GAME/RosterPets.cpp (0x6FB72F90 .. 0x6FB73920, 21 functions)
//   - GAME/Record.cpp     (0x6FB26790 .. 0x6FB26E00, 16 functions)
//
// Source Assertion Paths:
//   - "..\Source\D2Client\GAME\Roster.cpp"     (0x6FB84B98)
//   - "..\Source\D2Client\GAME\RosterPets.cpp" (0x6FB802FC)
//   - "..\Source\D2Client\GAME\Record.cpp"     (0x6FB84BF4)
//   - "..\Source\D2Client\UNIT\PlayerList.cpp" (0x6FB85ED0)
//
// Cross-referenced against:
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Units.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2StatList.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2DataTbls.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Dungeon.h
// =============================================================================

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#include <Fog.h>
#include <Storm.h>
#include <Units/Units.h>
#include <D2StatList.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <Window.h>

// =============================================================================
// External Imports & Internal Helpers
// =============================================================================
extern "C" void  __stdcall  GAME_SetInitSeed(int32_t nInitSeed); // D2Game.#10053

// Internal D2Client helpers
struct D2PlayerRelationListStrc;
D2PlayerRelationListStrc* __fastcall PLAYERLIST_AllocList(void);                                                      // 0x6FAE0C70
void    __fastcall PLAYERLIST_SetPartyStateForTarget(uint32_t dwLocalId, uint32_t dwTargetId, uint16_t wPartyState);   // 0x6FAE0D50
int32_t __fastcall PLAYERLIST_TestRelationFlag(uint32_t dwSourceId, uint32_t dwTargetId, uint32_t dwFlag);             // 0x6FAE0DD0
void    __fastcall PLAYERLIST_SetRelationFlag(uint32_t dwSourceId, uint32_t dwTargetId, uint32_t dwFlag, int32_t bSet);// 0x6FAE0E10
void    __fastcall PLAYERLIST_EnsureRelationEntry(uint32_t dwSourceId, uint32_t dwTargetId);                           // 0x6FAE0E60
void    __fastcall PARTYUI_MarkRosterDirty(void);                                                                      // 0x6FB12340
void    __fastcall MSG_SendClientPacket(size_t nBytes, const void* pPacket);                                           // 0x6FB291A0
void    __fastcall MSG_SendHostilePacket(uint32_t dwTargetUnitId, uint8_t nSubType, int32_t bHostile);                 // 0x6FB29720
void    __fastcall UNIT_UpdateMercenaryHostileOverlay(D2UnitStrc* pUnit, int32_t bFriendly);                           // 0x6FACEFA0
void    __fastcall UNIT_FreeUnitGfxData(D2UnitStrc* pUnit);                                                            // 0x6FACF900
void    __fastcall MONSTER_ReinitVisualSeed(D2UnitStrc* pUnit);                                                        // 0x6FAE2D10
void    __stdcall  MONSTER_SetInitPacketData(D2UnitStrc* pUnit, const void* pMonInitData);                             // 0x6FAE5BB0

// =============================================================================
// Data Structures: Roster, Party Groups, Pets, and Input Recording
// =============================================================================

struct D2PlayerRelationNodeStrc // 0x0C bytes (allocated in UNIT/PlayerList.cpp)
{
    uint32_t                  dwTargetUnitId;     // +0x00
    uint32_t                  dwRelationFlags;    // +0x04 (0x01=Loot, 0x02=Mute, 0x04=Squelch, 0x08=Hostile)
    D2PlayerRelationNodeStrc* pNext;              // +0x08
};

struct D2PlayerRelationListStrc // 0x04 bytes
{
    D2PlayerRelationNodeStrc* pHead;              // +0x00
};

struct D2RosterCorpseNodeStrc // 0x08 bytes
{
    uint32_t                dwCorpseUnitId;       // +0x00
    D2RosterCorpseNodeStrc* pNext;                // +0x04
};

struct D2RosterUnitStrc // 0x84 (132) bytes
{
    char                      szName[16];         // +0x00
    uint32_t                  dwUnitId;           // +0x10
    uint32_t                  dwPartyLife;        // +0x14 (0..100)
    int32_t                   nArenaScore;        // +0x18
    uint32_t                  dwClassId;          // +0x1C
    uint16_t                  wLevel;             // +0x20
    uint16_t                  wPartyId;           // +0x22 (0xFFFF = none)
    uint32_t                  dwLevelId;          // +0x24
    int32_t                   nXpos;              // +0x28
    int32_t                   nYpos;              // +0x2C
    uint32_t                  dwPartyFlags;       // +0x30 (1=unpartied, 2=invited, 4=has invite)
    D2PlayerRelationListStrc* pPvpRelationList;   // +0x34
    D2RosterCorpseNodeStrc*   pCorpseList;        // +0x38
    uint32_t                  dwQuestFlagsLow;    // +0x3C
    uint32_t                  dwQuestFlagsHigh;   // +0x40
    uint16_t                  wGuildEmblem;       // +0x44
    char                      szGuildTag[4];      // +0x46
    char                      szAccountName[28];  // +0x4A
    char                      szFormattedName[26];// +0x66
    D2RosterUnitStrc*         pNext;              // +0x80
};

struct D2RosterPartyGroupStrc // 0x0C (12) bytes
{
    uint16_t                wPartyId;             // +0x00
    uint16_t                wPad02;               // +0x02
    D2RosterUnitStrc*       pMembers;             // +0x04
    D2RosterPartyGroupStrc* pNext;                // +0x08
};

struct D2RosterPetCreateDataStrc // 0x0C bytes
{
    uint32_t dwMonSeed;                           // +0x00
    uint32_t dwNameIdx;                           // +0x04
    uint32_t dwSpecialFlag;                       // +0x08
};

struct D2RosterPetStrc // 0x34 (52) bytes
{
    int32_t          nClassId;                    // +0x00
    int32_t          nPetType;                    // +0x04 (7 = hireling)
    uint32_t         dwUnitId;                    // +0x08
    uint32_t         dwOwnerId;                   // +0x0C
    int32_t          nPetNameStringId;            // +0x10
    int32_t          nScreenIconX;                // +0x14
    int32_t          nScreenIconY;                // +0x18
    int32_t          nLifePercent;                // +0x1C (0..100)
    int32_t          bIsDeadHireling;             // +0x20
    uint32_t         dwMonSeed;                   // +0x24
    uint32_t         dwNameIdx;                   // +0x28
    uint32_t         dwSpecialFlag;               // +0x2C
    D2RosterPetStrc* pNext;                       // +0x30
};

struct D2RecordedEventStrc // 0x10 (16) bytes
{
    uint32_t dwGameTick;                          // +0x00
    uint32_t uMsg;                                // +0x04
    WPARAM   wParam;                              // +0x08
    LPARAM   lParam;                              // +0x0C
};

struct D2RecordChunkStrc // 0x1F4C (8,012) bytes
{
    D2RecordedEventStrc aEvents[500];             // +0x0000 .. +0x1F3F
    int32_t             nRecordedCount;           // +0x1F40
    int32_t             nPlaybackIndex;           // +0x1F44
    D2RecordChunkStrc*  pNext;                    // +0x1F48
};

struct D2RecordFileHeaderStrc // 0x14 (20) bytes at 0x6FBB5368
{
    uint32_t dwVersion;                           // +0x00 (must be 1)
    int32_t  nChunkCount;                         // +0x04
    int32_t  nTotalEventCount;                    // +0x08
    uint32_t dwEndGameTick;                       // +0x0C
    uint32_t dwInitSeed;                          // +0x10
};

// =============================================================================
// Module Globals (1.12a Addresses)
// =============================================================================
D2RosterUnitStrc*              gpRosterUnitList        = nullptr;                // 0x6FBCC080
static D2RosterUnitStrc*       gpRosterPendingList     = nullptr;                // 0x6FBCC084
static D2RosterPartyGroupStrc* gpRosterPartyGroupList  = nullptr;                // 0x6FBCC088
extern struct D2RosterPetEntryStrc* gpRosterPetList;                             // 0x6FBCB97C
#define gpRosterPetList (*reinterpret_cast<D2RosterPetStrc**>(&gpRosterPetList))

static D2RecordChunkStrc       g_tFirstRecordChunk     = { 0 };                  // 0x6FBB3418 (next at 0x6FBB5360)
static D2RecordChunkStrc*      gpCurrentRecordChunk    = &g_tFirstRecordChunk;   // 0x6FB923B0
static int32_t                 g_nChunkReadEvents      = 0;                      // 0x6FBB5358
static void*                   g_pfChainedExitCallback = nullptr;                // 0x6FBB5364
static D2RecordFileHeaderStrc  g_tRecordFileHeader     = { 0 };                  // 0x6FBB5368 .. 0x6FBB5378
static int32_t                 gbWindowCloseRequested  = 0;                      // 0x6FBCC08C
static int32_t                 gbRecordCaptureActive   = 0;                      // 0x6FBCC090
static int32_t                 gbSaveAndExitPending    = 0;                      // 0x6FBCC094
int32_t                        gbRecordPlaybackActive  = 0;                      // 0x6FBCC098

extern uint32_t                gdwClientGameTick;                                // 0x6FBB32B0
extern D2DrlgActStrc*          gpClientAct;                                      // 0x6FBCC01C
extern D2UnitStrc*             gpClientPlayer;                                   // 0x6FBCC3D0
extern uint32_t                gdwArenaFlags;                                    // 0x6FBA675C
extern D2UnitStrc*             gapServerSideUnitHashTables[6][128];              // 0x6FBCA960
#define g_apServerMonsterHashTable (gapServerSideUnitHashTables[1])

// Forward declarations
D2RosterUnitStrc* __fastcall ROSTER_FindPlayerByUnitId(uint32_t dwUnitId);                  // 0x6FB281C0
int32_t           __fastcall ROSTER_ArePlayersHostile(uint32_t dwUnitIdA, uint32_t dwUnitIdB); // 0x6FB287F0

// =============================================================================
// SECTION 1: Input Recording & Playback (GAME/Record.cpp, 0x6FB26790..0x6FB26E00)
// =============================================================================

// 1.12a: 0x6FB26790 (__fastcall)
void __fastcall RECORD_RequestWindowClose(void)
{
    if (gbSaveAndExitPending != 0)
    {
        gbSaveAndExitPending = 0;
        return;
    }
    gbWindowCloseRequested = 1;
}

// 1.12a: 0x6FB267B0 (__fastcall)
int32_t __fastcall RECORD_IsPlaybackActive(void)
{
    return gbRecordPlaybackActive;
}

// 1.12a: 0x6FB267C0 (__fastcall)
int32_t __fastcall RECORD_IsSaveAndExitPending(void)
{
    return gbSaveAndExitPending;
}

// 1.12a: 0x6FB267D0 (__fastcall)
void __fastcall RECORD_EnablePlayback(void)
{
    gbRecordPlaybackActive = 1;
}

// 1.12a: 0x6FB267E0 (__fastcall)
int32_t __fastcall RECORD_IsCaptureActive(void)
{
    return gbRecordCaptureActive;
}

// 1.12a: 0x6FB267F0 (__fastcall)
int32_t __fastcall RECORD_IsWindowCloseRequested(void)
{
    return gbWindowCloseRequested;
}

// 1.12a: 0x6FB26800 (__fastcall)
void __fastcall RECORD_FreeAllocatedChunks(void)
{
    D2RecordChunkStrc* pChunk = g_tFirstRecordChunk.pNext;
    while (pChunk != nullptr)
    {
        D2RecordChunkStrc* pNext = pChunk->pNext;
        FOG_Free(pChunk, "..\\Source\\D2Client\\GAME\\Record.cpp", 288, 0);
        pChunk = pNext;
    }
}

// 1.12a: 0x6FB26830 (__fastcall)
D2RecordChunkStrc* __fastcall RECORD_AllocNextChunk(void)
{
    D2RecordChunkStrc* pNewChunk = static_cast<D2RecordChunkStrc*>(
        FOG_Alloc(sizeof(D2RecordChunkStrc), "..\\Source\\D2Client\\GAME\\Record.cpp", 53, 0));

    pNewChunk->nRecordedCount = 0;
    pNewChunk->nPlaybackIndex = 0;
    pNewChunk->pNext = nullptr;

    gpCurrentRecordChunk->pNext = pNewChunk;
    gpCurrentRecordChunk = pNewChunk;
    return pNewChunk;
}

// 1.12a: 0x6FB26880 (usercall: eax = pEvent)
void __fastcall RECORD_DispatchRecordedEvent(const D2RecordedEventStrc* pEvent)
{
    if (pEvent->dwGameTick != gdwClientGameTick)
    {
        FOG_DisplayHalt("pEvent->dwGameTick == gdwClientGameTick", "..\\Source\\D2Client\\GAME\\Record.cpp", 298);
        exit(-1);
    }

    BOOL bHandled = FALSE;
    LRESULT lResult = 0;
    HWND hWnd = WINDOW_GetWindow();
    SMsgDispatchMessage(hWnd, pEvent->uMsg, pEvent->wParam, pEvent->lParam, &bHandled, &lResult);
}

// 1.12a: 0x6FB268E0 (__fastcall)
void __fastcall RECORD_LoadFromFile(void)
{
    HANDLE hFile = CreateFileA(
        "Record.dr1",
        GENERIC_READ,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    DWORD dwBytesRead = 0;
    uint32_t dwHeaderSize = 0;
    ReadFile(hFile, &dwHeaderSize, sizeof(dwHeaderSize), &dwBytesRead, nullptr);
    ReadFile(hFile, &g_tRecordFileHeader, sizeof(g_tRecordFileHeader), &dwBytesRead, nullptr);

    if (g_tRecordFileHeader.dwVersion != 1)
    {
        gbSaveAndExitPending = 0;
        gbRecordPlaybackActive = 0;
        CloseHandle(hFile);
        return;
    }

    for (int32_t i = 0; i < g_tRecordFileHeader.nChunkCount; ++i)
    {
        if (i != 0)
        {
            RECORD_AllocNextChunk();
        }

        int32_t nEventsToRead = g_tRecordFileHeader.nTotalEventCount;
        if (nEventsToRead <= 500)
        {
            g_nChunkReadEvents = nEventsToRead;
            if ((i + 1) != g_tRecordFileHeader.nChunkCount)
            {
                FOG_DisplayHalt("(i + 1) == g_tRecordFileHeader.nChunkCount", "..\\Source\\D2Client\\GAME\\Record.cpp", 256);
                exit(-1);
            }
        }
        else
        {
            nEventsToRead = 500;
            g_nChunkReadEvents = 500;
        }

        ReadFile(
            hFile,
            gpCurrentRecordChunk->aEvents,
            nEventsToRead * sizeof(D2RecordedEventStrc),
            &dwBytesRead,
            nullptr);
    }

    gpCurrentRecordChunk = &g_tFirstRecordChunk;
    CloseHandle(hFile);
}

// 1.12a: 0x6FB26A20 (__fastcall)
void __fastcall RECORD_SaveToFile(void)
{
    HANDLE hFile = CreateFileA(
        "Record.dr1",
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    int32_t nChunkCount = 1;
    D2RecordChunkStrc* pLastChunk = &g_tFirstRecordChunk;
    while (pLastChunk->pNext != nullptr)
    {
        ++nChunkCount;
        pLastChunk = pLastChunk->pNext;
    }

    D2RecordFileHeaderStrc tHeader = { 0 };
    tHeader.dwVersion = 1;
    tHeader.nChunkCount = nChunkCount;
    tHeader.nTotalEventCount = (nChunkCount - 1) * 500 + pLastChunk->nRecordedCount;
    tHeader.dwEndGameTick = gdwClientGameTick;
    tHeader.dwInitSeed = DUNGEON_GetInitSeedFromAct(gpClientAct);

    DWORD dwBytesWritten = 0;
    const uint32_t dwHeaderSize = sizeof(D2RecordFileHeaderStrc);
    WriteFile(hFile, &dwHeaderSize, sizeof(dwHeaderSize), &dwBytesWritten, nullptr);
    WriteFile(hFile, &tHeader, sizeof(tHeader), &dwBytesWritten, nullptr);

    for (D2RecordChunkStrc* pChunk = &g_tFirstRecordChunk; pChunk != nullptr; pChunk = pChunk->pNext)
    {
        WriteFile(
            hFile,
            pChunk->aEvents,
            pChunk->nRecordedCount * sizeof(D2RecordedEventStrc),
            &dwBytesWritten,
            nullptr);
    }

    CloseHandle(hFile);
}

// 1.12a: 0x6FB26B10 (__stdcall)
void __stdcall RECORD_AppendEvent(uint32_t uMsg, WPARAM wParam, LPARAM lParam)
{
    if (gpCurrentRecordChunk->nRecordedCount >= 500)
    {
        RECORD_AllocNextChunk();
    }

    const int32_t nIdx = gpCurrentRecordChunk->nRecordedCount++;
    D2RecordedEventStrc* pEvent = &gpCurrentRecordChunk->aEvents[nIdx];
    pEvent->dwGameTick = gdwClientGameTick;
    pEvent->uMsg = uMsg;
    pEvent->wParam = wParam;
    pEvent->lParam = lParam;
}

// 1.12a: 0x6FB26B70 (__fastcall)
void __fastcall RECORD_UpdatePlaybackTick(void)
{
    if (!gbRecordPlaybackActive || gpCurrentRecordChunk == nullptr)
    {
        return;
    }

    if (gdwClientGameTick >= g_tRecordFileHeader.dwEndGameTick)
    {
        gbRecordPlaybackActive = 0;
        return;
    }

    if (gpCurrentRecordChunk->nPlaybackIndex >= 500)
    {
        FOG_DisplayHalt("gpCurrentRecordChunk->nPlaybackIndex < 500", "..\\Source\\D2Client\\GAME\\Record.cpp", 393);
        exit(-1);
    }

    while (gpCurrentRecordChunk != nullptr)
    {
        const int32_t nIdx = gpCurrentRecordChunk->nPlaybackIndex;
        if (gpCurrentRecordChunk->pNext == nullptr && gpCurrentRecordChunk->nRecordedCount == nIdx)
        {
            break;
        }

        const D2RecordedEventStrc* pEvent = &gpCurrentRecordChunk->aEvents[nIdx];
        if (pEvent->dwGameTick > gdwClientGameTick)
        {
            break;
        }

        RECORD_DispatchRecordedEvent(pEvent);
        const int32_t nNextIdx = ++gpCurrentRecordChunk->nPlaybackIndex;

        if (gpCurrentRecordChunk->pNext == nullptr && gpCurrentRecordChunk->nRecordedCount == nNextIdx)
        {
            break;
        }

        if (nNextIdx >= 500)
        {
            gpCurrentRecordChunk = gpCurrentRecordChunk->pNext;
        }
    }
}

// 1.12a: 0x6FB26C50 (__stdcall)
void __stdcall RECORD_SaveAndInvokeExitCallback(int32_t a1, int32_t a2, int32_t a3)
{
    RECORD_SaveToFile();
    if (g_pfChainedExitCallback != nullptr)
    {
        reinterpret_cast<void(__stdcall*)(int32_t, int32_t, int32_t)>(g_pfChainedExitCallback)(a1, a2, a3);
    }
}

// 1.12a: 0x6FB26C70 (__fastcall)
void __fastcall RECORD_BeginPlaybackOrClearCloseFlag(void)
{
    if (gbWindowCloseRequested != 0)
    {
        gbWindowCloseRequested = 0;
        return;
    }

    gbSaveAndExitPending = 1;
    RECORD_LoadFromFile();
    GAME_SetInitSeed(g_tRecordFileHeader.dwInitSeed);
}

// 1.12a: 0x6FB26CA0 (usercall: eax = uMsg, esi = wParam, edx = lParam)
void __fastcall RECORD_ProcessWindowMessage(uint32_t uMsg, LPARAM lParam)
{
    if (!gbRecordCaptureActive)
    {
        return;
    }

    if (uMsg == WM_CLOSE || uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_CHAR ||
        uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP || uMsg == WM_COMMAND ||
        (uMsg >= WM_MOUSEMOVE && uMsg <= WM_RBUTTONDBLCLK))
    {
        RECORD_AppendEvent(uMsg, 0, lParam);
    }
}

// =============================================================================
// SECTION 2: Party & Player Roster Management (GAME/Roster.cpp, 0x6FB27890..0x6FB29160)
// =============================================================================

// 1.12a: 0x6FB27890 (usercall: eax = nValA, ecx = nValB, edi = nModulus)
int32_t __fastcall ROSTER_GetCircularWrappedDistance(int32_t nValA, int32_t nValB, int32_t nModulus)
{
    int32_t nModA = 0;
    int32_t nModB = 0;
    if ((nModulus & (nModulus - 1)) == 0)
    {
        nModA = nValA & (nModulus - 1);
        nModB = nValB & (nModulus - 1);
    }
    else
    {
        nModA = nValA % nModulus;
        nModB = nValB % nModulus;
    }

    int32_t nDiff1 = nModB - nModA;
    int32_t nAbs1  = (nDiff1 < 0) ? -nDiff1 : nDiff1;
    int32_t nAbs2  = (nDiff1 - nModulus < 0) ? -(nDiff1 - nModulus) : (nDiff1 - nModulus);
    int32_t nAbs3  = (nModA - nModB - nModulus < 0) ? -(nModA - nModB - nModulus) : (nModA - nModB - nModulus);

    int32_t nMin = (nAbs1 < nAbs2) ? nAbs1 : nAbs2;
    return (nMin < nAbs3) ? nMin : nAbs3;
}

// 1.12a: 0x6FB278E0 (__stdcall)
float __stdcall ROSTER_Hypot2D(float fDx, float fDy)
{
    return sqrtf(fDx * fDx + fDy * fDy);
}

// 1.12a: 0x6FB27900 (usercall: esi = nMaxExclusive)
uint32_t __fastcall ROSTER_RollPlayerRandomRange(int32_t nMaxExclusive)
{
    if (nMaxExclusive <= 0 || gpClientPlayer == nullptr)
    {
        return 0;
    }

    const uint64_t ullNext = static_cast<uint64_t>(gpClientPlayer->pSeed.nLowSeed) * 0x6AC690C5ULL + gpClientPlayer->pSeed.nHighSeed;
    gpClientPlayer->pSeed.nLowSeed  = static_cast<uint32_t>(ullNext);
    gpClientPlayer->pSeed.nHighSeed = static_cast<uint32_t>(ullNext >> 32);

    if ((nMaxExclusive & (nMaxExclusive - 1)) == 0)
    {
        return gpClientPlayer->pSeed.nLowSeed & static_cast<uint32_t>(nMaxExclusive - 1);
    }
    return gpClientPlayer->pSeed.nLowSeed % static_cast<uint32_t>(nMaxExclusive);
}

// 1.12a: 0x6FB27950 (usercall: edi = nHalfSpan)
int32_t __fastcall ROSTER_RollPlayerSymmetricOffset(int32_t nHalfSpan)
{
    return static_cast<int32_t>(ROSTER_RollPlayerRandomRange(nHalfSpan * 2 + 1)) - nHalfSpan;
}

// 1.12a: 0x6FB27960 (usercall: edi = nMin, eax = nMax)
int32_t __fastcall ROSTER_RollPlayerBoundedInt(int32_t nMin, int32_t nMax)
{
    return static_cast<int32_t>(ROSTER_RollPlayerRandomRange(nMax - nMin + 1)) + nMin;
}

// 1.12a: 0x6FB27970 (usercall: eax = pRosterUnit)
int32_t __fastcall ROSTER_IsMaleCharacterClass(const D2RosterUnitStrc* pRosterUnit)
{
    if (pRosterUnit == nullptr)
    {
        return 1;
    }
    const int32_t nClass = static_cast<int32_t>(pRosterUnit->dwClassId);
    if (nClass >= 0 && (nClass <= 1 || nClass == 6)) // Amazon (0), Sorceress (1), Assassin (6)
    {
        return 0;
    }
    return 1;
}

// 1.12a: 0x6FB27990 (__fastcall: ecx = dwQuestFlagCheck)
const char* __fastcall ROSTER_FindFormattedNameByQuestField(uint32_t dwQuestFieldVal)
{
    for (const D2RosterUnitStrc* pUnit = gpRosterUnitList; pUnit != nullptr; pUnit = pUnit->pNext)
    {
        if (pUnit->dwQuestFlagsLow == dwQuestFieldVal || pUnit->dwQuestFlagsHigh == dwQuestFieldVal)
        {
            return pUnit->szFormattedName;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FB279C0 (usercall: eax = pRosterUnit)
uint32_t __fastcall ROSTER_GetLevelId(const D2RosterUnitStrc* pRosterUnit)
{
    return pRosterUnit ? pRosterUnit->dwLevelId : 0;
}

// 1.12a: 0x6FB279D0 (usercall: eax = pRosterUnit, edx = pOutX, ecx = pOutY)
void __fastcall ROSTER_GetPlayerCoords(const D2RosterUnitStrc* pRosterUnit, int32_t* pOutX, int32_t* pOutY)
{
    if (pRosterUnit != nullptr)
    {
        *pOutX = pRosterUnit->nXpos;
        *pOutY = pRosterUnit->nYpos;
    }
    else
    {
        *pOutX = 0;
        *pOutY = 0;
    }
}

// 1.12a: 0x6FB279F0 (usercall: eax = pRosterUnit)
uint32_t __fastcall ROSTER_GetUnitId(const D2RosterUnitStrc* pRosterUnit)
{
    return pRosterUnit ? pRosterUnit->dwUnitId : 0xFFFFFFFFu;
}

// 1.12a: 0x6FB27A00 (usercall: eax = pRosterUnit)
uint32_t __fastcall ROSTER_GetPartyFlags(const D2RosterUnitStrc* pRosterUnit)
{
    return pRosterUnit ? pRosterUnit->dwPartyFlags : 1;
}

// 1.12a: 0x6FB27A10 (usercall: eax = pRosterUnit)
// Returns the player's `wPartyId` only if at least 2 players in the roster share that party ID;
// otherwise returns 0xFFFF.
uint16_t __fastcall ROSTER_GetActivePartyId(const D2RosterUnitStrc* pRosterUnit)
{
    if (pRosterUnit == nullptr || gpRosterUnitList == nullptr)
    {
        return 0xFFFFu;
    }

    const uint16_t wPartyId = pRosterUnit->wPartyId;
    int32_t nMemberCount = 0;
    for (const D2RosterUnitStrc* pIter = gpRosterUnitList; pIter != nullptr; pIter = pIter->pNext)
    {
        if (pIter->wPartyId == wPartyId)
        {
            ++nMemberCount;
        }
    }

    return (nMemberCount > 1) ? wPartyId : 0xFFFFu;
}

// 1.12a: 0x6FB27A40 (usercall: eax = pRosterUnit)
uint32_t __fastcall ROSTER_GetClassId(const D2RosterUnitStrc* pRosterUnit)
{
    return pRosterUnit ? pRosterUnit->dwClassId : 0;
}

// 1.12a: 0x6FB27A50 (usercall: eax = pRosterUnit)
uint16_t __fastcall ROSTER_GetCharacterLevel(const D2RosterUnitStrc* pRosterUnit)
{
    return pRosterUnit ? pRosterUnit->wLevel : 0;
}

// 1.12a: 0x6FB27A60 (usercall: eax = pRosterUnit)
int32_t __fastcall ROSTER_GetArenaScore(const D2RosterUnitStrc* pRosterUnit)
{
    return pRosterUnit ? pRosterUnit->nArenaScore : 0;
}

// 1.12a: 0x6FB27A70 (usercall: eax = pRosterUnit)
const char* __fastcall ROSTER_GetAccountName(const D2RosterUnitStrc* pRosterUnit)
{
    return pRosterUnit ? pRosterUnit->szAccountName : nullptr;
}

// 1.12a: 0x6FB27A80 (usercall: eax = pRosterUnit)
const char* __fastcall ROSTER_GetGuildTag(const D2RosterUnitStrc* pRosterUnit)
{
    return pRosterUnit ? pRosterUnit->szGuildTag : nullptr;
}

// 1.12a: 0x6FB27A90 (usercall: eax = pRosterUnit)
uint16_t __fastcall ROSTER_GetGuildEmblem(const D2RosterUnitStrc* pRosterUnit)
{
    return pRosterUnit ? pRosterUnit->wGuildEmblem : 0;
}

// 1.12a: 0x6FB27AA0 (usercall: esi = dwUnitId)
// Moves the roster entry matching `dwUnitId` to the head of `gpRosterUnitList`.
void __fastcall ROSTER_MovePlayerToFront(uint32_t dwUnitId)
{
    D2RosterUnitStrc* pHead = gpRosterUnitList;
    if (pHead == nullptr)
    {
        return;
    }

    D2RosterUnitStrc* pPrev = nullptr;
    for (D2RosterUnitStrc* pCurr = pHead; pCurr != nullptr; pCurr = pCurr->pNext)
    {
        if (pCurr->dwUnitId == dwUnitId && pPrev != nullptr)
        {
            pPrev->pNext = pCurr->pNext;
            pCurr->pNext = pHead;
            pHead = pCurr;
        }
        pPrev = pCurr;
    }
    gpRosterUnitList = pHead;
}

// 1.12a: 0x6FB27AF0 (__fastcall)
// Flattens all party member lists from `gpRosterPartyGroupList` back into `gpRosterUnitList`.
void __fastcall ROSTER_FlattenPartyGroupsToMainList(void)
{
    D2RosterUnitStrc* pTail = nullptr;
    for (D2RosterPartyGroupStrc* pGroup = gpRosterPartyGroupList; pGroup != nullptr; pGroup = pGroup->pNext)
    {
        if (pTail != nullptr)
        {
            pTail->pNext = pGroup->pMembers;
        }
        else
        {
            gpRosterUnitList = pGroup->pMembers;
            pTail = gpRosterUnitList;
        }

        pGroup->pMembers = nullptr;
        if (pTail != nullptr)
        {
            while (pTail->pNext != nullptr)
            {
                pTail = pTail->pNext;
            }
        }
    }
}

// 1.12a: 0x6FB27B50 (usercall: dx = wLocalPartyId, ebx = dwLocalUnitId)
// Moves the local player's party group to the head of `gpRosterPartyGroupList` and moves
// the local player to the head of that group's member list.
void __fastcall ROSTER_PromoteLocalPartyAndPlayerToFront(uint16_t wLocalPartyId, uint32_t dwLocalUnitId)
{
    if (wLocalPartyId == 0xFFFFu || gpRosterPartyGroupList == nullptr)
    {
        return;
    }

    D2RosterPartyGroupStrc* pPrevGroup = nullptr;
    D2RosterPartyGroupStrc* pGroup = gpRosterPartyGroupList;
    while (pGroup != nullptr && pGroup->wPartyId != wLocalPartyId)
    {
        pPrevGroup = pGroup;
        pGroup = pGroup->pNext;
    }

    if (pGroup == nullptr)
    {
        return;
    }

    if (pPrevGroup != nullptr)
    {
        pPrevGroup->pNext = pGroup->pNext;
        pGroup->pNext = gpRosterPartyGroupList;
        gpRosterPartyGroupList = pGroup;
    }

    D2RosterUnitStrc* pPrevMember = nullptr;
    D2RosterUnitStrc* pMember = pGroup->pMembers;
    while (pMember != nullptr && pMember->dwUnitId != dwLocalUnitId)
    {
        pPrevMember = pMember;
        pMember = pMember->pNext;
    }

    if (pMember != nullptr && pPrevMember != nullptr)
    {
        pPrevMember->pNext = pMember->pNext;
        pMember->pNext = pGroup->pMembers;
        pGroup->pMembers = pMember;
    }
}

// 1.12a: 0x6FB27BD0 (__fastcall)
// Insertion-sorts `gpRosterPartyGroupList` in ascending order by `wPartyId`.
void __fastcall ROSTER_SortPartyGroupsById(void)
{
    D2RosterPartyGroupStrc* pSortedHead = nullptr;
    D2RosterPartyGroupStrc* pCurr = gpRosterPartyGroupList;

    while (pCurr != nullptr)
    {
        D2RosterPartyGroupStrc* pNext = pCurr->pNext;
        D2RosterPartyGroupStrc* pPrev = nullptr;
        D2RosterPartyGroupStrc* pIter = pSortedHead;

        while (pIter != nullptr && pCurr->wPartyId >= pIter->wPartyId)
        {
            pPrev = pIter;
            pIter = pIter->pNext;
        }

        if (pPrev != nullptr)
        {
            pCurr->pNext = pPrev->pNext;
            pPrev->pNext = pCurr;
        }
        else
        {
            pCurr->pNext = pSortedHead;
            pSortedHead = pCurr;
        }

        pCurr = pNext;
    }

    gpRosterPartyGroupList = pSortedHead;
}

// 1.12a: 0x6FB27C30 (usercall: edi = pPartyGroup)
// Extracts all roster units with `wPartyId == pPartyGroup->wPartyId` from `gpRosterUnitList`
// and prepends them into `pPartyGroup->pMembers`.
void __fastcall ROSTER_CollectMembersForPartyGroup(D2RosterPartyGroupStrc* pPartyGroup)
{
    D2RosterUnitStrc* pPrev = nullptr;
    D2RosterUnitStrc* pCurr = gpRosterUnitList;

    while (pCurr != nullptr)
    {
        if (pCurr->wPartyId == pPartyGroup->wPartyId)
        {
            D2RosterUnitStrc* pNext = pCurr->pNext;
            if (pPrev != nullptr)
            {
                pPrev->pNext = pNext;
            }
            else
            {
                gpRosterUnitList = pNext;
            }
            pCurr->pNext = pPartyGroup->pMembers;
            pPartyGroup->pMembers = pCurr;
            pCurr = pNext;
        }
        else
        {
            pPrev = pCurr;
            pCurr = pCurr->pNext;
        }
    }
}

// 1.12a: 0x6FB27C90 (__fastcall)
void __fastcall ROSTER_FreePartyGroupList(void)
{
    D2RosterPartyGroupStrc* pGroup = gpRosterPartyGroupList;
    while (pGroup != nullptr)
    {
        D2RosterPartyGroupStrc* pNext = pGroup->pNext;
        FOG_Free(pGroup, "..\\Source\\D2Client\\GAME\\Roster.cpp", 556, 0);
        pGroup = pNext;
    }
    gpRosterPartyGroupList = nullptr;
}

// 1.12a: 0x6FB27CD0 (__fastcall: cx = wPartyId)
D2RosterPartyGroupStrc* __fastcall ROSTER_FindPartyGroup(uint16_t wPartyId)
{
    for (D2RosterPartyGroupStrc* pGroup = gpRosterPartyGroupList; pGroup != nullptr; pGroup = pGroup->pNext)
    {
        if (pGroup->wPartyId == wPartyId)
        {
            return pGroup;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FB27CF0 (usercall: eax = pListHead)
// Insertion-sorts a roster linked list in descending order by `nArenaScore` (+0x18).
D2RosterUnitStrc* __fastcall ROSTER_SortListByArenaScoreDesc(D2RosterUnitStrc* pListHead)
{
    D2RosterUnitStrc* pSortedHead = nullptr;
    D2RosterUnitStrc* pCurr = pListHead;

    while (pCurr != nullptr)
    {
        D2RosterUnitStrc* pNext = pCurr->pNext;
        D2RosterUnitStrc* pPrev = nullptr;
        D2RosterUnitStrc* pIter = pSortedHead;

        while (pIter != nullptr && pCurr->nArenaScore <= pIter->nArenaScore)
        {
            pPrev = pIter;
            pIter = pIter->pNext;
        }

        if (pPrev != nullptr)
        {
            pCurr->pNext = pPrev->pNext;
            pPrev->pNext = pCurr;
        }
        else
        {
            pCurr->pNext = pSortedHead;
            pSortedHead = pCurr;
        }

        pCurr = pNext;
    }

    return pSortedHead;
}

// 1.12a: 0x6FB27D50 (__stdcall)
// Insertion-sorts a roster linked list in ascending lexicographical order by `szName` (+0x00).
D2RosterUnitStrc* __stdcall ROSTER_SortListByNameAsc(D2RosterUnitStrc* pListHead)
{
    D2RosterUnitStrc* pSortedHead = nullptr;
    D2RosterUnitStrc* pCurr = pListHead;

    while (pCurr != nullptr)
    {
        D2RosterUnitStrc* pNext = pCurr->pNext;
        D2RosterUnitStrc* pPrev = nullptr;
        D2RosterUnitStrc* pIter = pSortedHead;

        while (pIter != nullptr && strcmp(pCurr->szName, pIter->szName) >= 0)
        {
            pPrev = pIter;
            pIter = pIter->pNext;
        }

        if (pPrev != nullptr)
        {
            pCurr->pNext = pPrev->pNext;
            pPrev->pNext = pCurr;
        }
        else
        {
            pCurr->pNext = pSortedHead;
            pSortedHead = pCurr;
        }

        pCurr = pNext;
    }

    return pSortedHead;
}

// 1.12a: 0x6FB27E10 (usercall: eax = pRosterUnit)
const char* __fastcall ROSTER_GetFormattedNameChecked(const D2RosterUnitStrc* pRosterUnit)
{
    if (pRosterUnit == nullptr)
    {
        FOG_DisplayHalt("pRosterUnit != NULL", "..\\Source\\D2Client\\GAME\\Roster.cpp", 369);
        exit(-1);
    }
    return pRosterUnit->szFormattedName;
}

// 1.12a: 0x6FB27E40 (usercall: eax = pCurrentRosterUnit)
D2RosterUnitStrc* __fastcall ROSTER_GetNextOrFirstUnit(D2RosterUnitStrc* pCurrentRosterUnit)
{
    return pCurrentRosterUnit ? pCurrentRosterUnit->pNext : gpRosterUnitList;
}

// 1.12a: 0x6FB27E60 (usercall: edi = pRosterUnit)
void __fastcall ROSTER_FreeCorpseList(D2RosterUnitStrc* pRosterUnit)
{
    D2RosterCorpseNodeStrc* pNode = pRosterUnit->pCorpseList;
    while (pNode != nullptr)
    {
        D2RosterCorpseNodeStrc* pNext = pNode->pNext;
        FOG_Free(pNode, "..\\Source\\D2Client\\GAME\\Roster.cpp", 269, 0);
        pNode = pNext;
    }
    pRosterUnit->pCorpseList = nullptr;
}

// 1.12a: 0x6FB27EA0 (usercall: eax = pRosterUnit, edi = szCharName, ebx = szGuildTag, stack args)
void __fastcall ROSTER_PopulateUnitFields(
    D2RosterUnitStrc* pRosterUnit,
    const char* szCharName,
    const char* szGuildTag,
    uint32_t dwUnitId,
    uint32_t dwClassId,
    uint16_t wLevel,
    uint16_t wPartyId,
    uint32_t dwPartyFlags,
    uint16_t wGuildEmblem,
    const char* szAccountName)
{
    strcpy(pRosterUnit->szName, szCharName);
    pRosterUnit->dwUnitId = dwUnitId;
    pRosterUnit->dwClassId = dwClassId;
    pRosterUnit->wLevel = wLevel;
    pRosterUnit->wPartyId = wPartyId;
    pRosterUnit->dwPartyFlags = dwPartyFlags;
    pRosterUnit->wGuildEmblem = wGuildEmblem;
    strcpy(pRosterUnit->szGuildTag, szGuildTag);
    strcpy(pRosterUnit->szAccountName, szAccountName);

    if (szGuildTag[0] != '\0')
    {
        sprintf(pRosterUnit->szFormattedName, "%s {%s}", szCharName, szGuildTag);
    }
    else
    {
        strcpy(pRosterUnit->szFormattedName, szCharName);
    }
}

// 1.12a: 0x6FB27F40 (usercall: ebx = dwUnitId, [esp+4] = szName)
D2RosterUnitStrc* __fastcall ROSTER_FindPlayerByUnitIdOrName(uint32_t dwUnitId, const char* szName)
{
    for (D2RosterUnitStrc* pUnit = gpRosterUnitList; pUnit != nullptr; pUnit = pUnit->pNext)
    {
        if (pUnit->dwUnitId == dwUnitId || strcmp(pUnit->szName, szName) == 0)
        {
            return pUnit;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FB27FB0 (usercall: eax = pRosterUnit, ecx = dwCorpseUnitId)
int32_t __fastcall ROSTER_HasCorpseId(const D2RosterUnitStrc* pRosterUnit, uint32_t dwCorpseUnitId)
{
    for (const D2RosterCorpseNodeStrc* pNode = pRosterUnit->pCorpseList; pNode != nullptr; pNode = pNode->pNext)
    {
        if (pNode->dwCorpseUnitId == dwCorpseUnitId)
        {
            return 1;
        }
    }
    return 0;
}

// 1.12a: 0x6FB27FD0 (__fastcall)
// Sorts each party group's member list (by arena score if arena flag 0x02 is set, else by name)
// and then sorts the party groups by party ID.
void __fastcall ROSTER_SortAllPartyGroups(void)
{
    for (D2RosterPartyGroupStrc* pGroup = gpRosterPartyGroupList; pGroup != nullptr; pGroup = pGroup->pNext)
    {
        if (gdwArenaFlags & 0x02)
        {
            pGroup->pMembers = ROSTER_SortListByArenaScoreDesc(pGroup->pMembers);
        }
        else
        {
            pGroup->pMembers = ROSTER_SortListByNameAsc(pGroup->pMembers);
        }
    }
    ROSTER_SortPartyGroupsById();
}

// 1.12a: 0x6FB28010 (usercall: si = wPartyId)
D2RosterPartyGroupStrc* __fastcall ROSTER_GetOrCreatePartyGroup(uint16_t wPartyId)
{
    for (D2RosterPartyGroupStrc* pGroup = gpRosterPartyGroupList; pGroup != nullptr; pGroup = pGroup->pNext)
    {
        if (pGroup->wPartyId == wPartyId)
        {
            return pGroup;
        }
    }

    D2RosterPartyGroupStrc* pNewGroup = static_cast<D2RosterPartyGroupStrc*>(
        FOG_Alloc(sizeof(D2RosterPartyGroupStrc), "..\\Source\\D2Client\\GAME\\Roster.cpp", 505, 0));
    pNewGroup->wPartyId = wPartyId;
    pNewGroup->wPad02 = 0;
    pNewGroup->pMembers = nullptr;
    pNewGroup->pNext = gpRosterPartyGroupList;
    gpRosterPartyGroupList = pNewGroup;
    return pNewGroup;
}

// 1.12a: 0x6FB28060 (__fastcall)
void __fastcall ROSTER_SortMainListByArenaScore(void)
{
    gpRosterUnitList = ROSTER_SortListByArenaScoreDesc(gpRosterUnitList);
}

// 1.12a: 0x6FB28070 (__fastcall)
void __fastcall ROSTER_SortMainListByName(void)
{
    gpRosterUnitList = ROSTER_SortListByNameAsc(gpRosterUnitList);
}

// 1.12a: 0x6FB28090 (__fastcall)
// Frees all roster units, their PvP relation lists, and corpse lists in both
// `gpRosterUnitList` and `gpRosterPendingList`.
void __fastcall ROSTER_FreeAllRosterLists(void)
{
    D2RosterUnitStrc* pLists[2] = { gpRosterUnitList, gpRosterPendingList };
    const int32_t aFreeLines[2] = { 330, 342 };

    for (int32_t nListIdx = 0; nListIdx < 2; ++nListIdx)
    {
        D2RosterUnitStrc* pUnit = pLists[nListIdx];
        while (pUnit != nullptr)
        {
            D2RosterUnitStrc* pNextUnit = pUnit->pNext;

            if (pUnit->pPvpRelationList != nullptr)
            {
                D2PlayerRelationNodeStrc* pRel = pUnit->pPvpRelationList->pHead;
                while (pRel != nullptr)
                {
                    D2PlayerRelationNodeStrc* pNextRel = pRel->pNext;
                    FOG_Free(pRel, "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 60, 0);
                    pRel = pNextRel;
                }
                FOG_Free(pUnit->pPvpRelationList, "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 64, 0);
                pUnit->pPvpRelationList = nullptr;
            }

            ROSTER_FreeCorpseList(pUnit);
            FOG_Free(pUnit, "..\\Source\\D2Client\\GAME\\Roster.cpp", aFreeLines[nListIdx], 0);
            pUnit = pNextUnit;
        }
    }

    gpRosterUnitList = nullptr;
    gpRosterPendingList = nullptr;
}

// 1.12a: 0x6FB281C0 (usercall: edx = dwUnitId)
// Finds a roster player entry by player unit GUID or by owned corpse unit GUID.
D2RosterUnitStrc* __fastcall ROSTER_FindPlayerByUnitId(uint32_t dwUnitId)
{
    if (dwUnitId == 0xFFFFFFFFu)
    {
        return nullptr;
    }

    for (D2RosterUnitStrc* pUnit = gpRosterUnitList; pUnit != nullptr; pUnit = pUnit->pNext)
    {
        if (pUnit->dwUnitId == dwUnitId)
        {
            return pUnit;
        }

        for (D2RosterCorpseNodeStrc* pCorpse = pUnit->pCorpseList; pCorpse != nullptr; pCorpse = pCorpse->pNext)
        {
            if (pCorpse->dwCorpseUnitId == dwUnitId)
            {
                return pUnit;
            }
        }
    }

    return nullptr;
}

// 1.12a: 0x6FB28200 (usercall: edx = dwUnitId)
uint32_t __fastcall ROSTER_GetRemotePartyMemberLife(uint32_t dwUnitId)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
    if (dwUnitId == dwLocalId || pRosterUnit == nullptr)
    {
        return 0;
    }
    return pRosterUnit->dwPartyLife;
}

// 1.12a: 0x6FB28230 (usercall: edx = dwUnitId, [esp+4] = nLifePercent)
void __fastcall ROSTER_SetPartyMemberLife(uint32_t dwUnitId, uint8_t nLifePercent)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    if (pRosterUnit != nullptr)
    {
        pRosterUnit->dwPartyLife = nLifePercent;
    }
}

// 1.12a: 0x6FB28250 (usercall: edx = dwUnitIdA, [esp+4] = dwUnitIdB)
int32_t __fastcall ROSTER_ArePlayersInSameParty(uint32_t dwUnitIdA, uint32_t dwUnitIdB)
{
    const D2RosterUnitStrc* pUnitA = ROSTER_FindPlayerByUnitId(dwUnitIdA);
    const D2RosterUnitStrc* pUnitB = ROSTER_FindPlayerByUnitId(dwUnitIdB);
    if (pUnitA != nullptr && pUnitB != nullptr &&
        pUnitA->wPartyId == pUnitB->wPartyId &&
        pUnitA->wPartyId != 0xFFFFu)
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB28290 (usercall: edx = dwPlayerUnitId, edi = dwCorpseUnitId)
void __fastcall ROSTER_RemovePlayerCorpse(uint32_t dwPlayerUnitId, uint32_t dwCorpseUnitId)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwPlayerUnitId);
    if (pRosterUnit == nullptr)
    {
        return;
    }

    D2RosterCorpseNodeStrc* pPrev = nullptr;
    for (D2RosterCorpseNodeStrc* pCurr = pRosterUnit->pCorpseList; pCurr != nullptr; pCurr = pCurr->pNext)
    {
        if (pCurr->dwCorpseUnitId == dwCorpseUnitId)
        {
            if (pPrev != nullptr)
            {
                pPrev->pNext = pCurr->pNext;
            }
            else
            {
                pRosterUnit->pCorpseList = pCurr->pNext;
            }
            FOG_Free(pCurr, "..\\Source\\D2Client\\GAME\\Roster.cpp", 1715, 0);
            return;
        }
        pPrev = pCurr;
    }
}

// 1.12a: 0x6FB282F0 (usercall: ebx = dwPlayerUnitId, [esp+4] = dwCorpseUnitId)
void __fastcall ROSTER_AddPlayerCorpse(uint32_t dwPlayerUnitId, uint32_t dwCorpseUnitId)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwPlayerUnitId);
    if (pRosterUnit == nullptr)
    {
        pRosterUnit = static_cast<D2RosterUnitStrc*>(
            FOG_Alloc(sizeof(D2RosterUnitStrc), "..\\Source\\D2Client\\GAME\\Roster.cpp", 1660, 0));
        if (pRosterUnit == nullptr)
        {
            FOG_DisplayHalt("pRosterUnit != NULL", "..\\Source\\D2Client\\GAME\\Roster.cpp", 1661);
            exit(-1);
        }
        memset(pRosterUnit, 0, sizeof(D2RosterUnitStrc));
        pRosterUnit->dwUnitId = dwPlayerUnitId;
        pRosterUnit->pNext = gpRosterPendingList;
        gpRosterPendingList = pRosterUnit;
    }

    for (D2RosterCorpseNodeStrc* pNode = pRosterUnit->pCorpseList; pNode != nullptr; pNode = pNode->pNext)
    {
        if (pNode->dwCorpseUnitId == dwCorpseUnitId)
        {
            return;
        }
    }

    D2RosterCorpseNodeStrc* pNewCorpse = static_cast<D2RosterCorpseNodeStrc*>(
        FOG_Alloc(sizeof(D2RosterCorpseNodeStrc), "..\\Source\\D2Client\\GAME\\Roster.cpp", 1677, 0));
    if (pNewCorpse == nullptr)
    {
        FOG_DisplayHalt("pNewCorpse != NULL", "..\\Source\\D2Client\\GAME\\Roster.cpp", 1678);
        exit(-1);
    }

    pNewCorpse->dwCorpseUnitId = dwCorpseUnitId;
    pNewCorpse->pNext = pRosterUnit->pCorpseList;
    pRosterUnit->pCorpseList = pNewCorpse;
}

// 1.12a: 0x6FB283B0 (usercall: edx = dwUnitId)
D2PlayerRelationListStrc* __fastcall ROSTER_GetPvpRelationList(uint32_t dwUnitId)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    return pRosterUnit ? pRosterUnit->pPvpRelationList : nullptr;
}

// 1.12a: 0x6FB283C0 (usercall: edx = dwUnitId, [esp+4] = dwQuestLow, [esp+8] = dwQuestHigh)
void __fastcall ROSTER_SetPlayerQuestFlags(uint32_t dwUnitId, uint32_t dwQuestLow, uint32_t dwQuestHigh)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    if (pRosterUnit != nullptr)
    {
        pRosterUnit->dwQuestFlagsLow = dwQuestLow;
        pRosterUnit->dwQuestFlagsHigh = dwQuestHigh;
    }
}

// 1.12a: 0x6FB283E0 (usercall: esi = pPlayerUnit)
const char* __fastcall ROSTER_GetPlayerUnitFormattedName(D2UnitStrc* pPlayerUnit)
{
    if (pPlayerUnit == nullptr)
    {
        FOG_DisplayHalt("pPlayerUnit != NULL", "..\\Source\\D2Client\\GAME\\Roster.cpp", 1489);
        exit(-1);
    }
    if (pPlayerUnit->dwUnitType != UNIT_PLAYER)
    {
        FOG_DisplayHalt("pPlayerUnit->dwUnitType == UNIT_PLAYER", "..\\Source\\D2Client\\GAME\\Roster.cpp", 1490);
        exit(-1);
    }

    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(pPlayerUnit->dwUnitId);
    if (pRosterUnit != nullptr)
    {
        return pRosterUnit->szFormattedName;
    }

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayerUnit);
    if (pPlayerData == nullptr)
    {
        FOG_DisplayHalt("pPlayerData != NULL", "..\\Source\\D2Client\\GAME\\Roster.cpp", 1499);
        exit(-1);
    }
    return pPlayerData->szName;
}

// 1.12a: 0x6FB28430 (usercall: edx = dwUnitId)
const char* __fastcall ROSTER_GetFormattedNameByUnitId(uint32_t dwUnitId)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    return pRosterUnit ? pRosterUnit->szFormattedName : nullptr;
}

// 1.12a: 0x6FB28440 (usercall: edx = dwUnitId)
uint32_t __fastcall ROSTER_GetPartyLifeByUnitId(uint32_t dwUnitId)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    return pRosterUnit ? pRosterUnit->dwPartyLife : 0;
}

// 1.12a: 0x6FB28450 (usercall: edx = dwUnitId, [esp+4] = nXpos, [esp+8] = nYpos)
void __fastcall ROSTER_SetPlayerCoordsByUnitId(uint32_t dwUnitId, int32_t nXpos, int32_t nYpos)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    if (pRosterUnit != nullptr)
    {
        pRosterUnit->nXpos = nXpos;
        pRosterUnit->nYpos = nYpos;
    }
}

// 1.12a: 0x6FB28470 (usercall: eax = pUnit)
uint16_t __fastcall ROSTER_GetUnitPartyId(const D2UnitStrc* pUnit)
{
    if (pUnit != nullptr)
    {
        const D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(pUnit->dwUnitId);
        if (pRosterUnit != nullptr)
        {
            return pRosterUnit->wPartyId;
        }
    }
    return 0xFFFFu;
}

// 1.12a: 0x6FB28490 (usercall: edx = dwUnitId)
D2RosterUnitStrc* __fastcall ROSTER_FindPlayerByUnitIdThunk(uint32_t dwUnitId)
{
    return ROSTER_FindPlayerByUnitId(dwUnitId);
}

// 1.12a: 0x6FB284A0 (__fastcall)
// Partitions `gpRosterUnitList` into party groups in `gpRosterPartyGroupList` by `wPartyId`.
void __fastcall ROSTER_BuildPartyGroupsFromMainList(void)
{
    while (gpRosterUnitList != nullptr)
    {
        const uint16_t wPartyId = gpRosterUnitList->wPartyId;
        D2RosterPartyGroupStrc* pGroup = ROSTER_GetOrCreatePartyGroup(wPartyId);
        ROSTER_CollectMembersForPartyGroup(pGroup);
    }
}

// 1.12a: 0x6FB28520 (__fastcall)
D2RosterUnitStrc* __fastcall ROSTER_GetLocalPlayerRosterEntryChecked(void)
{
    const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
    D2RosterUnitStrc* pLocalEntry = ROSTER_FindPlayerByUnitId(dwLocalId);
    if (pLocalEntry == nullptr)
    {
        FOG_DisplayHalt("pLocalEntry != NULL", "..\\Source\\D2Client\\GAME\\Roster.cpp", 114);
        exit(-1);
    }
    return pLocalEntry;
}

// 1.12a: 0x6FB28560 (usercall: eax = pPlayerUnit)
int32_t __fastcall ROSTER_HasOtherPartyMembers(const D2UnitStrc* pPlayerUnit)
{
    if (pPlayerUnit == nullptr || pPlayerUnit->dwUnitType != UNIT_PLAYER)
    {
        return 0;
    }

    const D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(pPlayerUnit->dwUnitId);
    if (pRosterUnit == nullptr || pRosterUnit->wPartyId == 0xFFFFu)
    {
        return 0;
    }

    for (const D2RosterUnitStrc* pIter = gpRosterUnitList; pIter != nullptr; pIter = pIter->pNext)
    {
        if (pIter->wPartyId == pRosterUnit->wPartyId && pIter->dwUnitId != pPlayerUnit->dwUnitId)
        {
            return 1;
        }
    }
    return 0;
}

// 1.12a: 0x6FB285B0 (usercall: eax = pRosterUnit)
int32_t __fastcall ROSTER_IsRosterUnitInLocalParty(const D2RosterUnitStrc* pRosterUnit)
{
    if (pRosterUnit == nullptr || pRosterUnit->wPartyId == 0xFFFFu)
    {
        return 0;
    }

    uint16_t wLocalPartyId = 0xFFFFu;
    if (gpClientPlayer != nullptr && gpClientPlayer->dwUnitType == UNIT_PLAYER)
    {
        const D2RosterUnitStrc* pLocalRoster = ROSTER_FindPlayerByUnitId(gpClientPlayer->dwUnitId);
        if (pLocalRoster != nullptr)
        {
            wLocalPartyId = pLocalRoster->wPartyId;
        }
    }

    return (wLocalPartyId == pRosterUnit->wPartyId) ? 1 : 0;
}

// 1.12a: 0x6FB28600 (__fastcall)
// Re-sorts the entire roster by party grouping, arena score / name, and promotes the local
// player and local party group to the top of the list.
void __fastcall ROSTER_ResortAllEntries(void)
{
    const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
    const D2RosterUnitStrc* pLocalEntry = ROSTER_FindPlayerByUnitId(dwLocalId);
    const uint16_t wLocalPartyId = pLocalEntry ? pLocalEntry->wPartyId : 0xFFFFu;

    ROSTER_BuildPartyGroupsFromMainList();
    ROSTER_SortAllPartyGroups();
    ROSTER_PromoteLocalPartyAndPlayerToFront(wLocalPartyId, dwLocalId);
    ROSTER_FlattenPartyGroupsToMainList();
    ROSTER_FreePartyGroupList();

    if (wLocalPartyId == 0xFFFFu)
    {
        ROSTER_MovePlayerToFront(dwLocalId);
    }
}

// 1.12a: 0x6FB28660 (__fastcall: ecx = pRosterUnit)
int32_t __fastcall ROSTER_CanLocalPlayerLootTarget(const D2RosterUnitStrc* pRosterUnit)
{
    if (pRosterUnit != nullptr && gpClientPlayer != nullptr)
    {
        if (PLAYERLIST_TestRelationFlag(gpClientPlayer->dwUnitId, pRosterUnit->dwUnitId, 0x01))
        {
            return 1;
        }
    }
    return 0;
}

// 1.12a: 0x6FB28690 (__fastcall: ecx = pRosterUnit)
int32_t __fastcall ROSTER_CanTargetLootLocalPlayer(const D2RosterUnitStrc* pRosterUnit)
{
    if (pRosterUnit != nullptr && gpClientPlayer != nullptr)
    {
        if (PLAYERLIST_TestRelationFlag(pRosterUnit->dwUnitId, gpClientPlayer->dwUnitId, 0x01))
        {
            return 1;
        }
    }
    return 0;
}

// 1.12a: 0x6FB286C0 (__fastcall: ecx = pRosterUnit)
int32_t __fastcall ROSTER_IsLocalPlayerHostileToTarget(const D2RosterUnitStrc* pRosterUnit)
{
    if (pRosterUnit != nullptr && gpClientPlayer != nullptr)
    {
        if (PLAYERLIST_TestRelationFlag(gpClientPlayer->dwUnitId, pRosterUnit->dwUnitId, 0x08))
        {
            return 1;
        }
    }
    return 0;
}

// 1.12a: 0x6FB286F0 (__fastcall: ecx = pRosterUnit)
int32_t __fastcall ROSTER_IsTargetHostileToLocalPlayer(const D2RosterUnitStrc* pRosterUnit)
{
    if (pRosterUnit != nullptr && gpClientPlayer != nullptr)
    {
        if (PLAYERLIST_TestRelationFlag(pRosterUnit->dwUnitId, gpClientPlayer->dwUnitId, 0x08))
        {
            return 1;
        }
    }
    return 0;
}

// 1.12a: 0x6FB28720 (__fastcall)
int32_t __fastcall ROSTER_IsAnyPlayerHostileToLocalPlayer(void)
{
    const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
    for (const D2RosterUnitStrc* pIter = gpRosterUnitList; pIter != nullptr; pIter = pIter->pNext)
    {
        if (pIter->dwUnitId == 0xFFFFFFFFu || pIter->dwUnitId == dwLocalId)
        {
            continue;
        }

        const D2RosterUnitStrc* pFound = ROSTER_FindPlayerByUnitId(pIter->dwUnitId);
        if (pFound != nullptr && pFound->pPvpRelationList != nullptr)
        {
            for (const D2PlayerRelationNodeStrc* pRel = pFound->pPvpRelationList->pHead; pRel != nullptr; pRel = pRel->pNext)
            {
                if (pRel->dwTargetUnitId == dwLocalId && (pRel->dwRelationFlags & 0x08))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// 1.12a: 0x6FB287C0 (usercall: eax = dwCollectorUnitId, edx = dwCorpseUnitId)
int32_t __fastcall ROSTER_CanPlayerLootCorpse(uint32_t dwCollectorUnitId, uint32_t dwCorpseUnitId)
{
    const D2RosterUnitStrc* pOwner = ROSTER_FindPlayerByUnitId(dwCorpseUnitId);
    if (pOwner == nullptr)
    {
        return 0;
    }
    if (dwCollectorUnitId == pOwner->dwUnitId)
    {
        return 1;
    }
    return PLAYERLIST_TestRelationFlag(pOwner->dwUnitId, dwCollectorUnitId, 0x01) ? 1 : 0;
}

// 1.12a: 0x6FB287F0 (usercall: edx = dwUnitIdA, [esp+4] = dwUnitIdB)
int32_t __fastcall ROSTER_ArePlayersHostile(uint32_t dwUnitIdA, uint32_t dwUnitIdB)
{
    const D2RosterUnitStrc* pUnitA = ROSTER_FindPlayerByUnitId(dwUnitIdA);
    const D2RosterUnitStrc* pUnitB = ROSTER_FindPlayerByUnitId(dwUnitIdB);
    if (pUnitA == nullptr || pUnitB == nullptr)
    {
        return 1;
    }

    if (pUnitA->wPartyId == pUnitB->wPartyId && pUnitA->wPartyId != 0xFFFFu)
    {
        return 0;
    }

    return PLAYERLIST_TestRelationFlag(pUnitA->dwUnitId, pUnitB->dwUnitId, 0x08) ? 1 : 0;
}

// 1.12a: 0x6FB28850 (usercall: eax = pTargetRosterUnit, bx = wRelationFlag)
void __fastcall ROSTER_ToggleLocalRelationFlag(D2RosterUnitStrc* pTargetRosterUnit, uint16_t wRelationFlag)
{
    const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
    if (pTargetRosterUnit == nullptr)
    {
        FOG_DisplayHalt("pTargetRosterUnit != NULL", "..\\Source\\D2Client\\GAME\\Roster.cpp", 1120);
        exit(-1);
    }

    PLAYERLIST_EnsureRelationEntry(dwLocalId, pTargetRosterUnit->dwUnitId);
    const int32_t bCurrentlySet = PLAYERLIST_TestRelationFlag(dwLocalId, pTargetRosterUnit->dwUnitId, wRelationFlag);
    PLAYERLIST_SetRelationFlag(dwLocalId, pTargetRosterUnit->dwUnitId, wRelationFlag, bCurrentlySet ? 0 : 1);
}

// Helper to check local player's relation flag towards `pTargetRosterUnit`
static int32_t ROSTER_CheckLocalRelationMask(const D2RosterUnitStrc* pTargetRosterUnit, uint32_t dwMask)
{
    if (pTargetRosterUnit == nullptr)
    {
        return 0;
    }
    const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
    const uint32_t dwTargetId = pTargetRosterUnit->dwUnitId;
    if (dwLocalId == dwTargetId)
    {
        return 0;
    }

    const D2RosterUnitStrc* pLocalEntry = ROSTER_FindPlayerByUnitId(dwLocalId);
    if (pLocalEntry == nullptr || pLocalEntry->pPvpRelationList == nullptr)
    {
        return 0;
    }

    for (const D2PlayerRelationNodeStrc* pRel = pLocalEntry->pPvpRelationList->pHead; pRel != nullptr; pRel = pRel->pNext)
    {
        if (pRel->dwTargetUnitId == dwTargetId)
        {
            return (pRel->dwRelationFlags & dwMask);
        }
    }
    return 0;
}

// 1.12a: 0x6FB288B0 (usercall: eax = pTargetRosterUnit)
int32_t __fastcall ROSTER_CheckLocalHostileToRosterUnit(const D2RosterUnitStrc* pTargetRosterUnit)
{
    return ROSTER_CheckLocalRelationMask(pTargetRosterUnit, 0x08) ? 1 : 0;
}

// 1.12a: 0x6FB28910 (usercall: eax = pTargetRosterUnit)
int32_t __fastcall ROSTER_CheckLocalSquelchFlag(const D2RosterUnitStrc* pTargetRosterUnit)
{
    return ROSTER_CheckLocalRelationMask(pTargetRosterUnit, 0x04);
}

// 1.12a: 0x6FB28960 (usercall: eax = pTargetRosterUnit)
int32_t __fastcall ROSTER_CheckLocalMuteFlag(const D2RosterUnitStrc* pTargetRosterUnit)
{
    return ROSTER_CheckLocalRelationMask(pTargetRosterUnit, 0x02);
}

// 1.12a: 0x6FB289B0 (usercall: eax = pTargetRosterUnit)
int32_t __fastcall ROSTER_CheckLocalLootPermissionFlag(const D2RosterUnitStrc* pTargetRosterUnit)
{
    return ROSTER_CheckLocalRelationMask(pTargetRosterUnit, 0x01);
}

// 1.12a: 0x6FB28A00 (usercall: edx = dwSenderUnitId)
int32_t __fastcall ROSTER_IsPlayerChatAllowed(uint32_t dwSenderUnitId)
{
    const D2RosterUnitStrc* pSender = ROSTER_FindPlayerByUnitId(dwSenderUnitId);
    if (pSender != nullptr)
    {
        return ROSTER_CheckLocalSquelchFlag(pSender) ? 0 : 1;
    }
    return 1;
}

#pragma pack(push, 1)
struct D2GSPacket5DStrc // 7 bytes: D2GS_PLAYERRELATION (0x5D)
{
    uint8_t  nHeader;       // +0x00 (0x5D)
    uint8_t  nRelationType; // +0x01 (1=Loot, 2=Mute, 3=Squelch, 4=Hostile)
    uint8_t  nAction;       // +0x02 (1=Set, 0=Clear)
    uint32_t dwTargetUnitId;// +0x03
};

struct D2GSPacket5EStrc // 6 bytes: D2GS_PARTY (0x5E)
{
    uint8_t  nHeader;       // +0x00 (0x5E)
    uint8_t  nAction;       // +0x01 (6=Invite, 7=Cancel, 8=Accept, 9=Leave)
    uint32_t dwTargetUnitId;// +0x02
};
#pragma pack(pop)

// 1.12a: 0x6FB28A20 (usercall: eax = pTargetRosterUnit)
void __fastcall ROSTER_ToggleSquelchAndSendPacket(D2RosterUnitStrc* pTargetRosterUnit)
{
    if (pTargetRosterUnit == nullptr)
    {
        return;
    }

    ROSTER_ToggleLocalRelationFlag(pTargetRosterUnit, 0x04);
    const int32_t bIsSquelched = ROSTER_CheckLocalSquelchFlag(pTargetRosterUnit);

    D2GSPacket5DStrc tPacket = { 0 };
    tPacket.nHeader = 0x5D;
    tPacket.nRelationType = 3;
    tPacket.nAction = bIsSquelched ? 1 : 0;
    tPacket.dwTargetUnitId = pTargetRosterUnit->dwUnitId;

    MSG_SendClientPacket(sizeof(tPacket), &tPacket);
}

// 1.12a: 0x6FB28AC0 (usercall: eax = pTargetRosterUnit)
void __fastcall ROSTER_ToggleMuteAndSendPacket(D2RosterUnitStrc* pTargetRosterUnit)
{
    if (pTargetRosterUnit == nullptr)
    {
        return;
    }

    ROSTER_ToggleLocalRelationFlag(pTargetRosterUnit, 0x02);
    const int32_t bIsMuted = ROSTER_CheckLocalMuteFlag(pTargetRosterUnit);

    D2GSPacket5DStrc tPacket = { 0 };
    tPacket.nHeader = 0x5D;
    tPacket.nRelationType = 2;
    tPacket.nAction = bIsMuted ? 1 : 0;
    tPacket.dwTargetUnitId = pTargetRosterUnit->dwUnitId;

    MSG_SendClientPacket(sizeof(tPacket), &tPacket);
}

// 1.12a: 0x6FB28B60 (usercall: eax = pTargetRosterUnit)
void __fastcall ROSTER_ToggleLootPermissionAndSendPacket(D2RosterUnitStrc* pTargetRosterUnit)
{
    if (pTargetRosterUnit == nullptr)
    {
        return;
    }

    ROSTER_ToggleLocalRelationFlag(pTargetRosterUnit, 0x01);
    const int32_t bCanLoot = ROSTER_CheckLocalLootPermissionFlag(pTargetRosterUnit);

    D2GSPacket5DStrc tPacket = { 0 };
    tPacket.nHeader = 0x5D;
    tPacket.nRelationType = 1;
    tPacket.nAction = bCanLoot ? 1 : 0;
    tPacket.dwTargetUnitId = pTargetRosterUnit->dwUnitId;

    MSG_SendClientPacket(sizeof(tPacket), &tPacket);
}

// 1.12a: 0x6FB28BF0 (usercall: eax = dwSourceUnitId, edi = dwTargetUnitId, [esp+4] = wRelationState)
void __fastcall ROSTER_OnRelationPacketReceived(uint32_t dwSourceUnitId, uint32_t dwTargetUnitId, uint16_t wRelationState)
{
    if (ROSTER_FindPlayerByUnitId(dwTargetUnitId) == nullptr)
    {
        return;
    }

    PLAYERLIST_EnsureRelationEntry(dwSourceUnitId, dwTargetUnitId);
    PLAYERLIST_SetPartyStateForTarget(dwSourceUnitId, dwTargetUnitId, wRelationState);
    PARTYUI_MarkRosterDirty();

    const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
    if (dwSourceUnitId == dwLocalId)
    {
        extern void __stdcall ROSTERPETS_UpdateGolemHostileState(uint32_t dwTargetPlayerId); // 0x6FB736B0
        ROSTERPETS_UpdateGolemHostileState(dwTargetUnitId);
    }
}

// 1.12a: 0x6FB28C40 (usercall: edx = dwUnitId, [esp+4] = nPartyFlags)
void __fastcall ROSTER_UpdatePlayerPartyFlags(uint32_t dwUnitId, uint8_t nPartyFlags)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    if (pRosterUnit != nullptr)
    {
        pRosterUnit->dwPartyFlags = nPartyFlags;
        PARTYUI_MarkRosterDirty();
    }
}

// 1.12a: 0x6FB28C60 (usercall: edx = dwUnitId, [esp+4] = wPartyId)
void __fastcall ROSTER_UpdatePlayerPartyId(uint32_t dwUnitId, uint16_t wPartyId)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    if (pRosterUnit != nullptr)
    {
        pRosterUnit->wPartyId = wPartyId;
        ROSTER_ResortAllEntries();
        PARTYUI_MarkRosterDirty();
    }
}

// 1.12a: 0x6FB28C80 (usercall: edx = dwUnitId, [esp+4] = dwPartyLife, [esp+8] = wLevelId)
void __fastcall ROSTER_UpdatePlayerLifeAndLevelId(uint32_t dwUnitId, uint32_t dwPartyLife, uint16_t wLevelId)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    if (pRosterUnit != nullptr)
    {
        pRosterUnit->dwPartyLife = dwPartyLife;
        pRosterUnit->dwLevelId = wLevelId;
        ROSTER_ResortAllEntries();
        PARTYUI_MarkRosterDirty();
    }
}

// 1.12a: 0x6FB28CB0 (__fastcall)
D2RosterUnitStrc* __fastcall ROSTER_LeaveLocalParty(void)
{
    const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
    D2RosterUnitStrc* pLocalEntry = ROSTER_FindPlayerByUnitId(dwLocalId);
    if (pLocalEntry == nullptr)
    {
        FOG_DisplayHalt("pLocalEntry != NULL", "..\\Source\\D2Client\\GAME\\Roster.cpp", 1251);
        exit(-1);
    }

    pLocalEntry->wPartyId = 0xFFFFu;
    pLocalEntry->dwPartyFlags = 1;
    ROSTER_ResortAllEntries();
    PARTYUI_MarkRosterDirty();
    return pLocalEntry;
}

// 1.12a: 0x6FB28D10 (usercall: eax = pTargetRosterUnit)
void __fastcall ROSTER_OnPartyButtonClicked(D2RosterUnitStrc* pTargetRosterUnit)
{
    if (pTargetRosterUnit == nullptr)
    {
        return;
    }

    uint8_t nPartyAction = 0;
    switch (pTargetRosterUnit->dwPartyFlags)
    {
    case 0:
    default:
        FOG_DisplayHalt("Invalid party button state", "..\\Source\\D2Client\\GAME\\Roster.cpp", 1213);
        exit(-1);
        break;
    case 1: // Unpartied -> Send Invite (6)
        pTargetRosterUnit->dwPartyFlags = 4;
        nPartyAction = 6;
        break;
    case 2: // Invited Local Player -> Accept Invite (8)
        pTargetRosterUnit->dwPartyFlags = 1;
        nPartyAction = 8;
        break;
    case 3: // In Party -> Leave Party (9)
        pTargetRosterUnit->dwPartyFlags = 1;
        nPartyAction = 9;
        break;
    case 4: // Invited by Local Player -> Cancel Invite (7)
        pTargetRosterUnit->dwPartyFlags = 0;
        nPartyAction = 7;
        break;
    }

    D2GSPacket5EStrc tPacket = { 0 };
    tPacket.nHeader = 0x5E;
    tPacket.nAction = nPartyAction;
    tPacket.dwTargetUnitId = pTargetRosterUnit->dwUnitId;

    MSG_SendClientPacket(sizeof(tPacket), &tPacket);
    ROSTER_ResortAllEntries();
    PARTYUI_MarkRosterDirty();
}

// 1.12a: 0x6FB28DD0 (usercall: edx = dwUnitId, si = wArenaScore)
void __fastcall ROSTER_SetPlayerArenaScore(uint32_t dwUnitId, int16_t nArenaScore)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    if (pRosterUnit != nullptr)
    {
        pRosterUnit->nArenaScore = static_cast<int32_t>(nArenaScore);
        ROSTER_ResortAllEntries();
        PARTYUI_MarkRosterDirty();
    }
}

// 1.12a: 0x6FB28E10 (__fastcall: ecx = dwUnitId)
void __fastcall ROSTER_RemovePlayerFromRoster(uint32_t dwUnitId)
{
    if (dwUnitId == 0xFFFFFFFFu)
    {
        FOG_DisplayHalt("dwUnitId != -1", "..\\Source\\D2Client\\GAME\\Roster.cpp", 286);
        exit(-1);
    }

    D2RosterUnitStrc* pPrev = nullptr;
    for (D2RosterUnitStrc* pCurr = gpRosterUnitList; pCurr != nullptr; pCurr = pCurr->pNext)
    {
        if (pCurr->dwUnitId == dwUnitId)
        {
            if (pPrev != nullptr)
            {
                pPrev->pNext = pCurr->pNext;
            }
            else
            {
                gpRosterUnitList = pCurr->pNext;
            }

            if (pCurr->pPvpRelationList != nullptr)
            {
                D2PlayerRelationNodeStrc* pRel = pCurr->pPvpRelationList->pHead;
                while (pRel != nullptr)
                {
                    D2PlayerRelationNodeStrc* pNextRel = pRel->pNext;
                    FOG_Free(pRel, "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 60, 0);
                    pRel = pNextRel;
                }
                FOG_Free(pCurr->pPvpRelationList, "..\\Source\\D2Client\\UNIT\\PlayerList.cpp", 64, 0);
                pCurr->pPvpRelationList = nullptr;
            }

            ROSTER_FreeCorpseList(pCurr);
            FOG_Free(pCurr, "..\\Source\\D2Client\\GAME\\Roster.cpp", 302, 0);
            break;
        }
        pPrev = pCurr;
    }

    ROSTER_ResortAllEntries();
    PARTYUI_MarkRosterDirty();
}

// 1.12a: 0x6FB28F10 (usercall: edx = dwUnitId, [esp+4] = wPartyId, [esp+8] = wLevel, [esp+12] = dwPartyFlags)
void __fastcall ROSTER_UpdatePlayerPartyAndLevel(
    uint32_t dwUnitId,
    uint16_t wPartyId,
    uint16_t wLevel,
    uint32_t dwPartyFlags)
{
    D2RosterUnitStrc* pRosterUnit = ROSTER_FindPlayerByUnitId(dwUnitId);
    if (pRosterUnit == nullptr || ROSTER_HasCorpseId(pRosterUnit, dwUnitId))
    {
        return;
    }

    pRosterUnit->wPartyId = wPartyId;
    pRosterUnit->wLevel = wLevel;
    pRosterUnit->dwPartyFlags = dwPartyFlags;

    extern void __fastcall ROSTERPETS_RefreshAllPetHostileOverlays(void); // 0x6FB73780
    ROSTERPETS_RefreshAllPetHostileOverlays();
    ROSTER_ResortAllEntries();
    PARTYUI_MarkRosterDirty();
}

// 1.12a: 0x6FB28F60 (usercall: eax = dwUnitId, stack args)
void __fastcall ROSTER_AddOrUpdatePlayer(
    uint32_t dwUnitId,
    const char* szCharName,
    uint32_t dwClassId,
    uint16_t wLevel,
    uint16_t wPartyId,
    uint32_t dwPartyFlags,
    uint16_t wGuildEmblem,
    const char* szAccountName,
    const char* szGuildTag)
{
    if (dwUnitId == 0xFFFFFFFFu)
    {
        FOG_DisplayHalt("dwUnitId != -1", "..\\Source\\D2Client\\GAME\\Roster.cpp", 176);
        exit(-1);
    }

    D2RosterUnitStrc* pEntry = ROSTER_FindPlayerByUnitIdOrName(dwUnitId, szCharName);
    if (pEntry != nullptr)
    {
        ROSTER_PopulateUnitFields(
            pEntry,
            szCharName,
            szGuildTag,
            dwUnitId,
            dwClassId,
            wLevel,
            wPartyId,
            dwPartyFlags,
            wGuildEmblem,
            szAccountName);
        ROSTER_ResortAllEntries();
        PARTYUI_MarkRosterDirty();
        return;
    }

    // Check pending list (e.g., corpses registered before the player join packet arrived)
    D2RosterUnitStrc* pPrevPending = nullptr;
    for (D2RosterUnitStrc* pIter = gpRosterPendingList; pIter != nullptr; pIter = pIter->pNext)
    {
        if (pIter->dwUnitId == dwUnitId)
        {
            pEntry = pIter;
            if (pPrevPending != nullptr)
            {
                pPrevPending->pNext = pEntry->pNext;
            }
            else
            {
                gpRosterPendingList = pEntry->pNext;
            }
            pEntry->pNext = nullptr;
            break;
        }
        pPrevPending = pIter;
    }

    if (pEntry == nullptr)
    {
        pEntry = static_cast<D2RosterUnitStrc*>(
            FOG_Alloc(sizeof(D2RosterUnitStrc), "..\\Source\\D2Client\\GAME\\Roster.cpp", 213, 0));
        memset(pEntry, 0, sizeof(D2RosterUnitStrc));
    }

    pEntry->pPvpRelationList = PLAYERLIST_AllocList();
    ROSTER_PopulateUnitFields(
        pEntry,
        szCharName,
        szGuildTag,
        dwUnitId,
        dwClassId,
        wLevel,
        wPartyId,
        dwPartyFlags,
        wGuildEmblem,
        szAccountName);

    pEntry->pNext = gpRosterUnitList;
    gpRosterUnitList = pEntry;

    ROSTER_ResortAllEntries();
    PARTYUI_MarkRosterDirty();
}

// 1.12a: 0x6FB290B0 (usercall: edi = pTargetRosterUnit)
void __fastcall ROSTER_ToggleHostilityWithPlayer(D2RosterUnitStrc* pTargetRosterUnit)
{
    if (pTargetRosterUnit == nullptr)
    {
        return;
    }

    const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
    const int32_t bAlreadyHostile = ROSTER_CheckLocalHostileToRosterUnit(pTargetRosterUnit);

    if (!bAlreadyHostile && pTargetRosterUnit->wLevel < 9)
    {
        return;
    }

    ROSTER_ToggleLocalRelationFlag(pTargetRosterUnit, 0x08);
    const int32_t bNowHostile = PLAYERLIST_TestRelationFlag(dwLocalId, pTargetRosterUnit->dwUnitId, 0x08);
    MSG_SendHostilePacket(pTargetRosterUnit->dwUnitId, 4, bNowHostile);

    if (PLAYERLIST_TestRelationFlag(dwLocalId, pTargetRosterUnit->dwUnitId, 0x08) &&
        pTargetRosterUnit->wPartyId != 0xFFFFu)
    {
        const D2RosterUnitStrc* pLocalEntry = ROSTER_GetLocalPlayerRosterEntryChecked();
        if (pLocalEntry->wPartyId == pTargetRosterUnit->wPartyId)
        {
            ROSTER_LeaveLocalParty();
        }
    }
}

// =============================================================================
// SECTION 3: Mercenary & Summon Pet Roster (GAME/RosterPets.cpp, 0x6FB72F90..0x6FB73920)
// =============================================================================

// 1.12a: 0x6FB73040 (__fastcall: ecx = dwPetUnitId)
int32_t __fastcall ROSTERPETS_GetPetLifePercent(uint32_t dwPetUnitId)
{
    for (const D2RosterPetStrc* pPet = gpRosterPetList; pPet != nullptr; pPet = pPet->pNext)
    {
        if (pPet->dwUnitId == dwPetUnitId)
        {
            return pPet->nLifePercent;
        }
    }
    return 0;
}

// 1.12a: 0x6FB73070 (usercall: edx = dwKeepUnitId)
// Removes any existing pet entry with `nClassId == 7` (hireling placeholder) whose `dwUnitId != dwKeepUnitId`.
void __fastcall ROSTERPETS_PruneOldHirelingPlaceholders(uint32_t dwKeepUnitId)
{
    D2RosterPetStrc* pPrev = nullptr;
    for (D2RosterPetStrc* pCurr = gpRosterPetList; pCurr != nullptr; pCurr = pCurr->pNext)
    {
        if (pCurr->nClassId == 7 && pCurr->dwUnitId != dwKeepUnitId)
        {
            if (pPrev != nullptr)
            {
                pPrev->pNext = pCurr->pNext;
            }
            else
            {
                gpRosterPetList = pCurr->pNext;
            }
            FOG_Free(pCurr, "..\\Source\\D2Client\\GAME\\RosterPets.cpp", 89, 0);
            return;
        }
        pPrev = pCurr;
    }
}

// 1.12a: 0x6FB730D0 (__fastcall: ecx = dwPetUnitId)
D2RosterPetStrc* __fastcall ROSTERPETS_FindPetByUnitId(uint32_t dwPetUnitId)
{
    for (D2RosterPetStrc* pPet = gpRosterPetList; pPet != nullptr; pPet = pPet->pNext)
    {
        if (pPet->dwUnitId == dwPetUnitId)
        {
            return pPet;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FB730F0 (__fastcall)
void __fastcall ROSTERPETS_FreeAllPets(void)
{
    D2RosterPetStrc* pPet = gpRosterPetList;
    while (pPet != nullptr)
    {
        D2RosterPetStrc* pNext = pPet->pNext;
        FOG_Free(pPet, "..\\Source\\D2Client\\GAME\\RosterPets.cpp", 47, 0);
        pPet = pNext;
    }
    gpRosterPetList = nullptr;
}

// 1.12a: 0x6FB73130 (__fastcall: ecx = dwPetUnitId)
int32_t __fastcall ROSTERPETS_GetPetType(uint32_t dwPetUnitId)
{
    const D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    return pPet ? pPet->nPetType : 0;
}

// 1.12a: 0x6FB73160 (__fastcall: ecx = dwPetUnitId)
uint32_t __fastcall ROSTERPETS_GetPetOwnerId(uint32_t dwPetUnitId)
{
    const D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    return pPet ? pPet->dwOwnerId : 0xFFFFFFFFu;
}

// 1.12a: 0x6FB73190 (__fastcall: ecx = dwPetUnitId)
int32_t __fastcall ROSTERPETS_GetPetClassId(uint32_t dwPetUnitId)
{
    const D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    return pPet ? pPet->nClassId : 0;
}

// 1.12a: 0x6FB731C0 (__fastcall: ecx = dwPetUnitId)
int32_t __fastcall ROSTERPETS_GetPetNameStringId(uint32_t dwPetUnitId)
{
    const D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    return pPet ? pPet->nPetNameStringId : 0;
}

// 1.12a: 0x6FB731F0 (usercall: eax = dwPetUnitId, edx = pOutIconX, ecx = pOutIconY)
void __fastcall ROSTERPETS_GetPetScreenIconCoords(uint32_t dwPetUnitId, int32_t* pOutIconX, int32_t* pOutIconY)
{
    const D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    if (pPet != nullptr)
    {
        *pOutIconX = pPet->nScreenIconX;
        *pOutIconY = pPet->nScreenIconY;
    }
    else
    {
        *pOutIconX = 0;
        *pOutIconY = 0;
    }
}

// Helper to resolve a server/client monster unit from `g_apServerMonsterHashTable` (`0x6FBCAB60`)
static D2UnitStrc* ROSTERPETS_FindMonsterUnitInHash(uint32_t dwUnitId)
{
    for (D2UnitStrc* pUnit = g_apServerMonsterHashTable[dwUnitId & 0x7Fu]; pUnit != nullptr; pUnit = pUnit->pListNext)
    {
        if (pUnit->dwUnitId == dwUnitId)
        {
            if (pUnit->dwUnitType != UNIT_MONSTER)
            {
                FOG_DisplayHalt("pUnit->dwUnitType == UNIT_MONSTER", "..\\Source\\D2Client\\GAME\\RosterPets.cpp", 90);
                exit(-1);
            }
            return pUnit;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FB73230 (usercall: ecx = dwPetUnitId, edi = nLifePercent, [esp+4] = wPetNameStringId)
void __fastcall ROSTERPETS_UpdatePetLifeAndName(uint32_t dwPetUnitId, int32_t nLifePercent, uint16_t wPetNameStringId)
{
    D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    if (pPet == nullptr)
    {
        return;
    }

    pPet->nPetNameStringId = wPetNameStringId;
    pPet->nLifePercent = nLifePercent;

    if (pPet->nPetType == 7) // Hireling
    {
        D2UnitStrc* pMercUnit = ROSTERPETS_FindMonsterUnitInHash(dwPetUnitId);
        if (pMercUnit != nullptr)
        {
            const int32_t nMaxLife = STATLIST_GetMaxLifeFromUnit(pMercUnit);
            const int32_t nScaledLife = (nMaxLife * nLifePercent) / 100;
            STATLIST_SetUnitStat(pMercUnit, STAT_HITPOINTS, nScaledLife, 0);
        }
    }
}

// 1.12a: 0x6FB732D0 (__fastcall: ecx = pOwnerUnit)
int32_t __fastcall ROSTERPETS_DoesPlayerHaveAnyPet(const D2UnitStrc* pOwnerUnit)
{
    const uint32_t dwOwnerId = pOwnerUnit ? pOwnerUnit->dwUnitId : 0xFFFFFFFFu;
    for (const D2RosterPetStrc* pPet = gpRosterPetList; pPet != nullptr; pPet = pPet->pNext)
    {
        if (pPet->dwOwnerId == dwOwnerId)
        {
            return 1;
        }
    }
    return 0;
}

// 1.12a: 0x6FB73300 (usercall: ecx = pOwnerUnit, esi = nPetType, edx = bIncludeDeadHireling)
uint32_t __fastcall ROSTERPETS_FindPlayerPetUnitIdByType(const D2UnitStrc* pOwnerUnit, int32_t nPetType, int32_t bIncludeDeadHireling)
{
    if (pOwnerUnit == nullptr)
    {
        return 0xFFFFFFFFu;
    }

    const uint32_t dwOwnerId = pOwnerUnit->dwUnitId;
    for (const D2RosterPetStrc* pPet = gpRosterPetList; pPet != nullptr; pPet = pPet->pNext)
    {
        if (pPet->nPetType == nPetType && pPet->dwOwnerId == dwOwnerId)
        {
            if (bIncludeDeadHireling || !pPet->bIsDeadHireling)
            {
                return pPet->dwUnitId;
            }
        }
    }
    return 0xFFFFFFFFu;
}

// 1.12a: 0x6FB73340 (usercall: eax = pOwnerUnit, esi = nPetType)
int32_t __fastcall ROSTERPETS_CountPlayerPetsByType(const D2UnitStrc* pOwnerUnit, int32_t nPetType)
{
    const uint32_t dwOwnerId = pOwnerUnit ? pOwnerUnit->dwUnitId : 0xFFFFFFFFu;
    int32_t nCount = 0;
    for (const D2RosterPetStrc* pPet = gpRosterPetList; pPet != nullptr; pPet = pPet->pNext)
    {
        if (pPet->nPetType == nPetType && pPet->dwOwnerId == dwOwnerId)
        {
            ++nCount;
        }
    }
    return nCount;
}

// 1.12a: 0x6FB73370 (__fastcall: ecx = dwPetUnitId)
const D2RosterPetCreateDataStrc* __fastcall ROSTERPETS_GetPetCreateData(uint32_t dwPetUnitId)
{
    const D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    return pPet ? reinterpret_cast<const D2RosterPetCreateDataStrc*>(&pPet->dwMonSeed) : nullptr;
}

// 1.12a: 0x6FB733A0 (usercall: edx = dwPetUnitId)
void __fastcall ROSTERPETS_RemoveOrMarkPetDead(uint32_t dwPetUnitId)
{
    D2RosterPetStrc* pPrev = nullptr;
    for (D2RosterPetStrc* pCurr = gpRosterPetList; pCurr != nullptr; pCurr = pCurr->pNext)
    {
        if (pCurr->dwUnitId == dwPetUnitId)
        {
            const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
            if (pCurr->nPetType == 7 && pCurr->dwOwnerId == dwLocalId)
            {
                pCurr->bIsDeadHireling = 1;
                return;
            }

            if (pPrev != nullptr)
            {
                pPrev->pNext = pCurr->pNext;
            }
            else
            {
                gpRosterPetList = pCurr->pNext;
            }
            FOG_Free(pCurr, "..\\Source\\D2Client\\GAME\\RosterPets.cpp", 209, 0);
            return;
        }
        pPrev = pCurr;
    }
}

// 1.12a: 0x6FB73420 (usercall: esi = nPetType, ebx = dwPetUnitId, [esp+4] = dwOwnerId, [esp+8] = nClassId)
void __fastcall ROSTERPETS_AddOrUpdateBasicPet(
    int32_t nPetType,
    uint32_t dwPetUnitId,
    uint32_t dwOwnerId,
    int32_t nClassId)
{
    if (nPetType == 7)
    {
        ROSTERPETS_PruneOldHirelingPlaceholders(dwPetUnitId);
    }

    D2RosterPetStrc* pExisting = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    if (pExisting != nullptr)
    {
        pExisting->nClassId = nClassId;
        pExisting->dwOwnerId = dwOwnerId;
        pExisting->nPetType = nPetType;
        pExisting->bIsDeadHireling = 0;
        return;
    }

    D2RosterPetStrc* pNewPet = static_cast<D2RosterPetStrc*>(
        FOG_Alloc(sizeof(D2RosterPetStrc), "..\\Source\\D2Client\\GAME\\RosterPets.cpp", 122, 0));
    memset(pNewPet, 0, sizeof(D2RosterPetStrc));

    pNewPet->nClassId = nClassId;
    pNewPet->nPetType = nPetType;
    pNewPet->dwUnitId = dwPetUnitId;
    pNewPet->dwOwnerId = dwOwnerId;
    pNewPet->nLifePercent = 100;
    pNewPet->pNext = gpRosterPetList;
    gpRosterPetList = pNewPet;
}

// 1.12a: 0x6FB734B0 (usercall: eax = dwPetUnitId)
// Returns automap/party affiliation state for a pet:
//   0 = unaligned/hostile, 1 = owned by local player, 2 = owned by party member, 3 = non-map pet.
int32_t __fastcall ROSTERPETS_GetPetPartyAffiliation(uint32_t dwPetUnitId)
{
    const D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    if (pPet == nullptr)
    {
        return 0;
    }

    const int32_t nPetType = pPet->nPetType;
    if (nPetType < 0 || nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        return 3;
    }

    const D2PetTypeTxt* pPetTypeTxt = &sgptDataTables->pPetTypeTxt[nPetType];
    if (pPetTypeTxt == nullptr || !(pPetTypeTxt->dwPetFlags & gdwBitMasks[4])) // 0x10 = automap draw flag
    {
        return 3;
    }

    const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
    if (pPet->dwOwnerId == dwLocalId)
    {
        return 1;
    }

    const D2RosterUnitStrc* pLocalRoster = ROSTER_FindPlayerByUnitId(dwLocalId);
    const uint16_t wLocalPartyId = ROSTER_GetActivePartyId(pLocalRoster);
    if (wLocalPartyId == 0xFFFFu)
    {
        return 0;
    }

    const D2RosterUnitStrc* pOwnerRoster = ROSTER_FindPlayerByUnitId(pPet->dwOwnerId);
    const uint16_t wOwnerPartyId = ROSTER_GetActivePartyId(pOwnerRoster);
    return (wLocalPartyId == wOwnerPartyId) ? 2 : 0;
}

// 1.12a: 0x6FB73560 (__fastcall: ecx = dwPetUnitId)
int32_t __fastcall ROSTERPETS_GetPetOwnerAffiliation(uint32_t dwPetUnitId)
{
    const D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    if (pPet == nullptr)
    {
        return 0;
    }

    const uint32_t dwLocalId = gpClientPlayer ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
    if (pPet->dwOwnerId == dwLocalId)
    {
        return 1;
    }

    const D2RosterUnitStrc* pLocalRoster = ROSTER_FindPlayerByUnitId(dwLocalId);
    const uint16_t wLocalPartyId = ROSTER_GetActivePartyId(pLocalRoster);
    if (wLocalPartyId == 0xFFFFu)
    {
        return 0;
    }

    const D2RosterUnitStrc* pOwnerRoster = ROSTER_FindPlayerByUnitId(pPet->dwOwnerId);
    const uint16_t wOwnerPartyId = ROSTER_GetActivePartyId(pOwnerRoster);
    return (wLocalPartyId == wOwnerPartyId) ? 2 : 0;
}

// 1.12a: 0x6FB735D0 (usercall: eax = pPetUnit)
int32_t __fastcall ROSTERPETS_IsPetHostileToLocalPlayer(const D2UnitStrc* pPetUnit)
{
    const uint32_t dwPetUnitId = pPetUnit ? pPetUnit->dwUnitId : 0xFFFFFFFFu;
    const D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    const uint32_t dwOwnerId = pPet ? pPet->dwOwnerId : 0xFFFFFFFFu;

    if (gpClientPlayer == nullptr || gpClientPlayer->dwUnitId == 0xFFFFFFFFu)
    {
        FOG_DisplayHalt("gpClientPlayer && gpClientPlayer->dwUnitId != -1", "..\\Source\\D2Client\\GAME\\RosterPets.cpp", 549);
        exit(-1);
    }

    if (dwOwnerId != 0xFFFFFFFFu)
    {
        return PLAYERLIST_TestRelationFlag(dwOwnerId, gpClientPlayer->dwUnitId, 0x08);
    }
    return 1;
}

// 1.12a: 0x6FB73650 (usercall: esi = pPetUnit, edi = pTargetUnit)
int32_t __fastcall ROSTERPETS_ArePetAndUnitHostile(D2UnitStrc* pPetUnit, D2UnitStrc* pTargetUnit)
{
    const uint32_t dwPetUnitId = pPetUnit ? pPetUnit->dwUnitId : 0xFFFFFFFFu;
    const D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);

    if (pPetUnit == nullptr || pTargetUnit == nullptr)
    {
        return 1;
    }

    if (pPet == nullptr)
    {
        if (pTargetUnit->dwUnitType == UNIT_MONSTER &&
            pPetUnit->dwUnitType == UNIT_MONSTER &&
            STATLIST_AreUnitsAligned(pTargetUnit, pPetUnit))
        {
            return 0;
        }
        return 1;
    }

    if (pPet->dwOwnerId == pTargetUnit->dwUnitId)
    {
        return 0;
    }
    return ROSTER_ArePlayersHostile(pTargetUnit->dwUnitId, pPet->dwOwnerId);
}

// 1.12a: 0x6FB736B0 (__stdcall)
void __stdcall ROSTERPETS_UpdateGolemHostileState(uint32_t dwOwnerPlayerId)
{
    for (const D2RosterPetStrc* pPet = gpRosterPetList; pPet != nullptr; pPet = pPet->pNext)
    {
        if (pPet->dwOwnerId == dwOwnerPlayerId && pPet->nClassId >= 363 && pPet->nClassId <= 364)
        {
            D2UnitStrc* pUnit = ROSTERPETS_FindMonsterUnitInHash(pPet->dwUnitId);
            if (pUnit != nullptr && pUnit->pMonsterData != nullptr)
            {
                const int32_t bFriendly = ROSTERPETS_IsPetHostileToLocalPlayer(pUnit) ? 0 : 1;
                uint32_t* pMonFields = reinterpret_cast<uint32_t*>(pUnit->pMonsterData);
                if (pMonFields[14] == pMonFields[13]) // +0x38 vs +0x34
                {
                    UNIT_UpdateMercenaryHostileOverlay(pUnit, bFriendly);
                }
                pMonFields[13] = static_cast<uint32_t>(bFriendly);
            }
        }
    }
}

// 1.12a: 0x6FB73780 (__fastcall)
void __fastcall ROSTERPETS_RefreshAllPetHostileOverlays(void)
{
    for (const D2RosterPetStrc* pPet = gpRosterPetList; pPet != nullptr; pPet = pPet->pNext)
    {
        if (pPet->nPetType == 4)
        {
            D2UnitStrc* pUnit = ROSTERPETS_FindMonsterUnitInHash(pPet->dwUnitId);
            if (pUnit != nullptr && pUnit->pMonsterData != nullptr)
            {
                const int32_t bFriendly = ROSTERPETS_ArePetAndUnitHostile(pUnit, gpClientPlayer) ? 0 : 1;
                uint32_t* pMonFields = reinterpret_cast<uint32_t*>(pUnit->pMonsterData);
                pMonFields[13] = static_cast<uint32_t>(bFriendly);
                pMonFields[14] = static_cast<uint32_t>(bFriendly);
            }
        }
    }
}

// 1.12a: 0x6FB73820 (usercall: eax = nPetType, ecx = dwPetUnitId, edi = pCreateData, [esp+4] = dwOwnerId, [esp+8] = nClassId)
void __fastcall ROSTERPETS_AddOrUpdateFullPet(
    int32_t nPetType,
    uint32_t dwPetUnitId,
    const D2RosterPetCreateDataStrc* pCreateData,
    uint32_t dwOwnerId,
    int32_t nClassId)
{
    if (nPetType == 7)
    {
        ROSTERPETS_PruneOldHirelingPlaceholders(dwPetUnitId);
    }

    ROSTERPETS_AddOrUpdateBasicPet(nPetType, dwPetUnitId, dwOwnerId, nClassId);
    D2RosterPetStrc* pPet = ROSTERPETS_FindPetByUnitId(dwPetUnitId);
    if (pPet == nullptr)
    {
        FOG_DisplayHalt("pPet != NULL", "..\\Source\\D2Client\\GAME\\RosterPets.cpp", 149);
        exit(-1);
    }

    pPet->dwUnitId = dwPetUnitId;
    pPet->dwOwnerId = dwOwnerId;
    pPet->nPetType = nPetType;
    pPet->dwMonSeed = pCreateData->dwMonSeed;
    pPet->dwNameIdx = pCreateData->dwNameIdx;
    pPet->dwSpecialFlag = pCreateData->dwSpecialFlag;

    if (nPetType == 7)
    {
        D2UnitStrc* pMercUnit = ROSTERPETS_FindMonsterUnitInHash(dwPetUnitId);
        if (pMercUnit != nullptr)
        {
            #pragma pack(push, 1)
            struct D2MercInitPacketDataStrc
            {
                uint16_t wNameIdx;
                uint16_t wPad;
                uint32_t dwMonSeed;
            } tInitData = { static_cast<uint16_t>(pCreateData->dwNameIdx), 0, pCreateData->dwMonSeed };
            #pragma pack(pop)

            MONSTER_SetInitPacketData(pMercUnit, &tInitData);
            UNIT_FreeUnitGfxData(pMercUnit);
            MONSTER_ReinitVisualSeed(pMercUnit);
            UNITS_ChangeAnimMode(pMercUnit, 1);
        }
    }
}
