// =============================================================================
// D2Client.dll 1.12a (ImageBase = 0x6FAB0000)
// Reconstructed Translation Unit: GAME/Msg.cpp (0x6FB29160 .. 0x6FB29CD0)
// Plus Coupled Local Input / Movement / Action Senders in:
//   - UNIT/UnitMode.cpp (0x6FAF0DC0 .. 0x6FAF1910)
//   - UNIT/Player.cpp   (0x6FB0B050 .. 0x6FB0CE80)
//   - GAME/Game.cpp     (0x6FB305A0, 0x6FB31FE0)
//
// Source Assertion Path: "..\Source\D2Client\GAME\Msg.cpp" (0x6FB87874)
// Cross-referenced against:
//   - H:/git/D2MOO-112a/source/D2Common/include/Path/Path.h
//   - H:/git/D2MOO-112a/source/D2Common/include/D2Collision.h
//   - H:/git/D2MOO-112a/source/D2Common/include/Units/Units.h
//   - H:/git/D2MOO-112a/source/D2Net/include/Client.h
// =============================================================================

#include <stdint.h>
#include <string.h>
#include <windows.h>

#include <Fog.h>
#include <Units/Units.h>
#include <Path/Path.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <D2Skills.h>
#include <D2StatList.h>
#include <D2States.h>
#include <D2DataTbls.h>

typedef D2ActiveRoomStrc D2RoomStrc;

// =============================================================================
// External D2Net / Fog / D2Common / D2Lang Imports
// =============================================================================
// D2Net.#10005 / #10036: int32_t __stdcall CLIENT_Send(int32_t nQueue, const void* pPacket, int32_t nSize)
extern "C" int32_t __stdcall CLIENT_Send(int32_t nQueue, const void* pPacket, int32_t nSize);
// D2Net.#10023: int32_t __stdcall D2NET_10023_GetSendQueueBacklog()
extern "C" int32_t __stdcall D2NET_10023_GetSendQueueBacklog(void);
// D2Lang.#10009: int32_t __fastcall Unicode::win2utf8(char* pDest, const wchar_t* pSrc, int32_t nDestSize)
extern "C" int32_t __fastcall Unicode_win2utf8(char* pDest, const wchar_t* pSrc, int32_t nDestSize);

// =============================================================================
// C->S Opcode Enum (0x01 .. 0x6D)
// =============================================================================
enum D2C2SPacketOpcodes : uint8_t
{
    C2S_WALKTOLOCATION          = 0x01, // 5 bytes: [0x01, uint16_t wX, uint16_t wY]
    C2S_WALKTOUNIT              = 0x02, // 9 bytes: [0x02, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_RUNTOLOCATION           = 0x03, // 5 bytes: [0x03, uint16_t wX, uint16_t wY]
    C2S_RUNTOUNIT               = 0x04, // 9 bytes: [0x04, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_LEFTSKILLONLOCATION     = 0x05, // 5 bytes: [0x05, uint16_t wX, uint16_t wY]
    C2S_LEFTSKILLONUNIT         = 0x06, // 9 bytes: [0x06, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_LEFTSKILLONUNITEX       = 0x07, // 9 bytes: [0x07, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_LEFTSKILLONLOCATIONEX   = 0x08, // 5 bytes: [0x08, uint16_t wX, uint16_t wY]
    C2S_LEFTSKILLONUNITREPEAT   = 0x09, // 9 bytes: [0x09, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_LEFTSKILLONUNITREPEATEX = 0x0A, // 9 bytes: [0x0A, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_SKILLUNKNOWN_0B         = 0x0B, // 1 byte:  [0x0B]
    C2S_RIGHTSKILLONLOCATION    = 0x0C, // 5 bytes: [0x0C, uint16_t wX, uint16_t wY]
    C2S_RIGHTSKILLONUNIT        = 0x0D, // 9 bytes: [0x0D, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_RIGHTSKILLONUNITEX      = 0x0E, // 9 bytes: [0x0E, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_RIGHTSKILLONLOCATIONEX  = 0x0F, // 5 bytes: [0x0F, uint16_t wX, uint16_t wY]
    C2S_RIGHTSKILLONUNITREPEAT  = 0x10, // 9 bytes: [0x10, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_RIGHTSKILLONUNITREPEATEX= 0x11, // 9 bytes: [0x11, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_SKILLUNKNOWN_12         = 0x12, // 1 byte:  [0x12]
    C2S_UNITINTERACT            = 0x13, // 9 bytes: [0x13, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_OVERHEADCHAT            = 0x14, // varlen:  [0x14, 0, 0, szMsg..., 0, 0, 0]
    C2S_CHATMESSAGE             = 0x15, // varlen:  [0x15, uint8_t nType, 0, szMsg..., 0, szTarget..., 0, 0]
    C2S_PICKUPITEM              = 0x16, // 13 bytes:[0x16, uint32_t dwUnitType(4), uint32_t dwItemGUID, uint32_t bToCursor]
    C2S_DROPITEM                = 0x17, // 5 bytes: [0x17, uint32_t dwItemGUID]
    C2S_INSERTITEMTOBUFFER      = 0x18, // 17 bytes:[0x18, uint32_t dwItemGUID, uint32_t nX, uint32_t nY, uint32_t nBuffer]
    C2S_REMOVEITEMFROMBUFFER    = 0x19, // 5 bytes: [0x19, uint32_t dwItemGUID]
    C2S_EQUIPITEM               = 0x1A, // 9 bytes: [0x1A, uint32_t dwItemGUID, uint16_t wBodyLoc, uint16_t wPad]
    C2S_SWAPEQUIPPEDITEM        = 0x1B, // 9 bytes: [0x1B, uint32_t dwItemGUID, uint16_t wBodyLoc, uint16_t wPad]
    C2S_UNEQUIPITEM             = 0x1C, // 3 bytes: [0x1C, uint16_t wBodyLoc]
    C2S_SWAPCURSOREQUIPPEDITEM  = 0x1D, // 9 bytes: [0x1D, uint32_t dwItemGUID, uint16_t wBodyLoc, uint16_t wPad]
    C2S_SWAPBELTITEM            = 0x23, // 9 bytes: [0x23, uint32_t dwItemGUID, uint16_t wBeltSlot, uint16_t wPad]
    C2S_REMOVEBELTITEM          = 0x24, // 5 bytes: [0x24, uint32_t dwItemGUID]
    C2S_SWAPCURSORBELTITEM      = 0x25, // 9 bytes: [0x25, uint32_t dwOldItemGUID, uint32_t dwNewItemGUID]
    C2S_USEBELTITEM             = 0x26, // 13 bytes:[0x26, uint32_t dwItemGUID, uint32_t bShiftToMerc, uint32_t dwZero]
    C2S_IDENTIFYITEM            = 0x27, // 9 bytes: [0x27, uint32_t dwItemGUID, uint32_t dwScrollGUID]
    C2S_SOCKETITEM              = 0x28, // 9 bytes: [0x28, uint32_t dwSocketableGUID, uint32_t dwTargetGUID]
    C2S_SCROLLTOTOME            = 0x29, // 9 bytes: [0x29, uint32_t dwScrollGUID, uint32_t dwTomeGUID]
    C2S_ITEMTOCUBE              = 0x2A, // 9 bytes: [0x2A, uint32_t dwItemGUID, uint32_t dwCubeGUID]
    C2S_TOWNFOLKINIT            = 0x2F, // 9 bytes: [0x2F, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_TOWNFOLKCANCEL          = 0x30, // 9 bytes: [0x30, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_QUESTMESSAGE            = 0x31, // 9 bytes: [0x31, uint32_t dwNpcGUID, uint32_t dwMessageId]
    C2S_TOWNFOLKBUY             = 0x32, // 17 bytes:[0x32, uint32_t dwNpcGUID, uint32_t dwItemGUID, uint32_t dwTab, uint32_t dwCost]
    C2S_TOWNFOLKSELL            = 0x33, // 17 bytes:[0x33, uint32_t dwNpcGUID, uint32_t dwItemGUID, uint32_t dwMode, uint32_t dwCost]
    C2S_CAINIDENTIFY            = 0x34, // 5 bytes: [0x34, uint32_t dwNpcGUID]
    C2S_TOWNFOLKREPAIR          = 0x35, // 17 bytes:[0x35, uint32_t dwNpcGUID, uint32_t dwItemGUID, uint32_t dwTab, uint32_t dwFlags]
    C2S_HIREMERC                = 0x36, // 9 bytes: [0x36, uint32_t dwNpcGUID, uint32_t dwMercId]
    C2S_GAMBLEIDENTIFY          = 0x37, // 5 bytes: [0x37, uint32_t dwItemGUID]
    C2S_TOWNFOLKMENUSELECTIONS  = 0x38, // 13 bytes:[0x38, uint32_t dwAction, uint32_t dwNpcGUID, uint32_t dwParam]
    C2S_INCREMENTSTAT           = 0x3A, // 3 bytes: [0x3A, uint16_t wStatId]
    C2S_INCREMENTSKILL          = 0x3B, // 3 bytes: [0x3B, uint16_t wSkillId]
    C2S_SELECTSKILL             = 0x3C, // 9 bytes: [0x3C, uint16_t wSkillId, uint16_t wLeftFlag, uint32_t dwItemGUID]
    C2S_ACTIVATEITEM            = 0x3F, // 3 bytes: [0x3F, uint16_t wSoundId]
    C2S_QUESTLOG                = 0x40, // 1 byte:  [0x40]
    C2S_RESURRECT               = 0x41, // 1 byte:  [0x41]
    C2S_STAFFINTOORIFICE        = 0x44, // 17 bytes:[0x44, uint32_t dwOrificeType, uint32_t dwOrificeGUID, uint32_t dwStaffGUID, uint32_t dwState]
    C2S_MERCINTERACT            = 0x4B, // 9 bytes: [0x4B, uint32_t dwUnitType, uint32_t dwUnitGUID]
    C2S_WAYPOINT                = 0x49, // 9 bytes: [0x49, uint32_t dwWaypointGUID, uint32_t dwDestLevelId]
    C2S_TRADEBUTTON             = 0x4F, // 7 bytes: [0x4F, uint16_t wAction, uint16_t wParamLow, uint16_t wParamHigh]
    C2S_DROPGOLD                = 0x50, // 9 bytes: [0x50, uint32_t dwPlayerGUID, uint32_t dwGoldAmount]
    C2S_ASSIGNHOTKEY            = 0x51, // 9 bytes: [0x51, uint16_t wSkillId, uint16_t wSlotFlags, uint32_t dwItemGUID]
    C2S_TOWNFOLKINVITE          = 0x59, // 17 bytes:[0x59, uint32_t dwUnitType, uint32_t dwUnitGUID, uint32_t dwX, uint32_t dwY]
    C2S_PARTYACTION             = 0x5D, // 7 bytes: [0x5D, uint8_t nSubAction, uint8_t bFlag, uint32_t dwTargetPlayerGUID]
    C2S_PARTYINVITE             = 0x5E, // 6 bytes: [0x5E, uint8_t nAction, uint32_t dwTargetPlayerGUID]
    C2S_UPDATEPOSITION          = 0x5F, // 5 bytes: [0x5F, uint16_t wX, uint16_t wY]
    C2S_SWITCHWEAPONS           = 0x60, // 1 byte:  [0x60]
    C2S_MERCITEM                = 0x61, // 3 bytes: [0x61, uint16_t wBodyLoc]
    C2S_RESURRECTMERC           = 0x62, // 5 bytes: [0x62, uint32_t dwNpcGUID]
    C2S_BELTSHIFTCLICK          = 0x63, // 5 bytes: [0x63, uint32_t dwItemGUID]
    C2S_WARDENRESPONSE          = 0x66, // varlen:  [0x66, uint16_t wPayloadLen, uint8_t abPayload[]]
    C2S_CREATEGAME              = 0x67, // 29 bytes
    C2S_JOINGAME                = 0x68, // 37 bytes
    C2S_LEAVEGAME               = 0x69, // 1 byte:  [0x69]
    C2S_JOINACTCOMPLETE         = 0x6B, // 1 byte:  [0x6B]
    C2S_UPLOADSAVECHUNK         = 0x6C, // varlen:  [0x6C, uint8_t nChunkLen, uint32_t dwTotalSaveSize, uint8_t abChunk[]]
    C2S_PING                    = 0x6D, // 13 bytes:[0x6D, uint32_t dwTickCount, uint32_t dwDelayDiff, uint32_t dwZero]
};

// =============================================================================
// Client Packet & Input Globals in D2Client.dll (.data / .bss)
// =============================================================================
static int32_t  g_bSinglePlayerUploadDone = 0;         // 0x6FB87870 (initialized to 1)
static uint32_t g_dwBytesSent             = 0;         // 0x6FBCBCCC
static uint32_t g_dwPacketsSent           = 0;         // 0x6FBCDCD4
static uint32_t g_dwLastSendTick          = 0;         // 0x6FBD3430
static int32_t  g_bSaveUploadComplete     = 0;         // 0x6FBD3434
static uint8_t  g_abLastSentPacket[0x200] = { 0 };     // 0x6FBD3438
static uint32_t g_dwLastPingTick          = 0;         // 0x6FBB5DD4
static uint32_t g_dwPingTimestamp         = 0;         // 0x6FBCBC9C
static uint32_t g_dwGameStartTick         = 0;         // 0x6FBCC1E4

// Player / Input Globals
extern D2UnitStrc*  gpClientPlayer;                    // 0x6FBCC3D0
extern D2DrlgActStrc* gpClientAct;                     // 0x6FBCC02C
extern int32_t      gbAlwaysRun;                       // 0x6FBCC054
extern int32_t      gbRunKeyHeld;                      // 0x6FBCC050
extern int32_t      gbRunLock;                         // 0x6FBCC04C
extern int32_t      gnClientActionFrame;               // 0x6FBCC2BC
extern int32_t      gnMouseScreenX;                    // 0x6FBCC3C8
extern int32_t      gnMouseScreenY;                    // 0x6FBCC3CC
extern int32_t      gbLeftMouseHold;                   // 0x6FBCC2AC
extern int32_t      gbStandStillHeld;                  // 0x6FBCC1D4

// Forward declarations for internal D2Client helpers
D2UnitStrc* __fastcall UNIT_GetClientPlayer(void);                                              // 0x6FB0B3E0
D2UnitStrc* __fastcall UNIT_FindClientUnit(int32_t nUnitGUID, int32_t nUnitType);               // 0x6FACE550
int32_t __fastcall UNITMODE_DispatchClientEvent(D2UnitStrc* pUnit, int32_t nEvent, int32_t nX, int32_t nY, D2UnitStrc* pTarget, int32_t a6, int32_t a7); // 0x6FAF1450
void __fastcall UNITMODE_QueuePendingPlayerAction(D2UnitStrc* pUnit, int32_t nEvent, int32_t nX, int32_t nY, D2UnitStrc* pTarget); // 0x6FAF1D90

// =============================================================================
// SECTION 1: GAME/Msg.cpp (0x6FB29160 .. 0x6FB29CD0)
// =============================================================================

// 0x6FB29160
void __fastcall MSG_ResetCounters(void)
{
    g_dwBytesSent = 0;
    g_dwPacketsSent = 0;
}

// 0x6FB29170
void __fastcall MSG_GetTrafficCounters(uint32_t* pBytesReceived, uint32_t* pPacketsReceived, uint32_t* pBytesSent, uint32_t* pPacketsSent)
{
    extern uint32_t g_dwBytesReceived;   // 0x6FBCBE50
    extern uint32_t g_dwPacketsReceived; // 0x6FBCBC90

    *pBytesReceived = g_dwBytesReceived;
    *pPacketsReceived = g_dwPacketsReceived;
    *pBytesSent = g_dwBytesSent;
    *pPacketsSent = g_dwPacketsSent;
}

// 0x6FB291A0
// Core throttled C->S packet sender.
// Copies the packet onto a 512-byte stack buffer, applies duplicate packet
// suppression within a 50ms or 200ms window depending on opcode class,
// and forwards the buffer to D2Net.#10036 (CLIENT_Send).
int32_t __fastcall MSG_SendPacket(void* pPacketData, size_t nSize)
{
    uint8_t abStackCopy[0x200];

    if (nSize >= 0x200)
    {
        FOG_DisplayAssert("len < MAX_CMD_SIZE", "..\\Source\\D2Client\\GAME\\Msg.cpp", 215);
        exit(-1);
    }

    memcpy(abStackCopy, pPacketData, nSize);

    const uint8_t bOpcode = abStackCopy[0];
    uint32_t dwThrottleWindowMs = 0;

    // Determine duplicate-suppression throttle window by opcode:
    // - 0x06, 0x07, 0x09, 0x0A, 0x0D, 0x0E (unit-targeted skill attacks): 50ms (0x32)
    // - 0x12 .. 0x39 (item manipulation, NPC interactions, chat): 0ms (never suppressed)
    // - All other opcodes (0x01..0x05, 0x08, 0x0B, 0x0C, 0x0F..0x11, >= 0x3A): 200ms (0xC8)
    if (bOpcode <= C2S_SKILLUNKNOWN_12)
    {
        switch (bOpcode)
        {
        case C2S_LEFTSKILLONUNIT:         // 0x06
        case C2S_LEFTSKILLONUNITEX:       // 0x07
        case C2S_LEFTSKILLONUNITREPEAT:   // 0x09
        case C2S_LEFTSKILLONUNITREPEATEX: // 0x0A
        case C2S_RIGHTSKILLONUNIT:        // 0x0D
        case C2S_RIGHTSKILLONUNITEX:      // 0x0E
            dwThrottleWindowMs = 50;
            break;

        case C2S_SKILLUNKNOWN_12:         // 0x12
            dwThrottleWindowMs = 0;
            break;

        default:                          // 0x01..0x05, 0x08, 0x0B, 0x0C, 0x0F..0x11
            dwThrottleWindowMs = 200;
            break;
        }
    }
    else if (bOpcode >= C2S_INCREMENTSTAT) // >= 0x3A
    {
        if (bOpcode == C2S_INCREMENTSTAT ||
            bOpcode == C2S_INCREMENTSKILL ||
            bOpcode == C2S_SELECTSKILL ||
            bOpcode == C2S_QUESTLOG ||
            bOpcode == C2S_TOWNFOLKINVITE ||
            bOpcode == C2S_SWITCHWEAPONS ||
            bOpcode == C2S_BELTSHIFTCLICK)
        {
            dwThrottleWindowMs = 0;
        }
        else
        {
            dwThrottleWindowMs = 200;
        }
    }
    else
    {
        // 0x13 (C2S_UNITINTERACT) .. 0x39: no duplicate suppression in MSG_SendPacket
        dwThrottleWindowMs = 0;
    }

    if (dwThrottleWindowMs != 0)
    {
        const uint32_t dwNow = GetTickCount();
        if (dwNow - g_dwLastSendTick < dwThrottleWindowMs)
        {
            if (memcmp(g_abLastSentPacket, pPacketData, nSize) == 0)
            {
                // Identical packet sent within throttle window: suppress
                return 0;
            }
        }
    }

    g_dwLastSendTick = GetTickCount();
    memcpy(g_abLastSentPacket, pPacketData, nSize);

    const int32_t nSent = CLIENT_Send(1, abStackCopy, static_cast<int32_t>(nSize));
    if (nSent != 0)
    {
        g_dwBytesSent += (nSent > 1) ? static_cast<uint32_t>(nSent) : static_cast<uint32_t>(nSize);
        g_dwPacketsSent += 1;
    }
    return nSent;
}

// 0x6FB292F0
void __fastcall MSG_ResetLastSentPacket(void)
{
    g_abLastSentPacket[0] = 0;
    g_dwLastSendTick = GetTickCount();
    g_bSinglePlayerUploadDone = 1;
}

// 0x6FB293F0
int32_t __fastcall MSG_Send6A(void)
{
    uint8_t bPacket = 0x6A;
    const int32_t nSent = CLIENT_Send(0, &bPacket, 1);
    if (nSent != 0)
    {
        g_dwBytesSent += nSent;
        g_dwPacketsSent += 1;
    }
    return nSent;
}

// 0x6FB29420
// Sends 0x69 (C2S_LEAVEGAME). Waits up to 2000ms for the send queue to flush first.
int32_t __fastcall MSG_SendLeaveGame_69(void)
{
    const uint32_t dwStart = GetTickCount();
    while (D2NET_10023_GetSendQueueBacklog() != 0)
    {
        if (GetTickCount() - dwStart >= 2000)
        {
            break;
        }
        Sleep(10);
    }

    uint8_t bPacket = C2S_LEAVEGAME;
    const int32_t nSent = CLIENT_Send(0, &bPacket, 1);
    if (nSent != 0)
    {
        g_dwBytesSent += nSent;
        g_dwPacketsSent += 1;
    }

    const uint32_t dwFlushStart = GetTickCount();
    while (D2NET_10023_GetSendQueueBacklog() != 0)
    {
        if (GetTickCount() - dwFlushStart >= 2000)
        {
            break;
        }
        Sleep(10);
    }
    return nSent;
}

// 0x6FB294B0
// Sends 0x6B (C2S_JOINACTCOMPLETE) in response to S->C 0x02.
int32_t __fastcall MSG_SendJoinActComplete_6B(void)
{
    uint8_t bPacket = C2S_JOINACTCOMPLETE;
    const int32_t nSent = CLIENT_Send(0, &bPacket, 1);
    if (nSent != 0)
    {
        g_dwBytesSent += nSent;
        g_dwPacketsSent += 1;
    }
    return nSent;
}

// 0x6FB294E0
// Sends 0x67 (C2S_CREATEGAME) - 29 bytes.
#pragma pack(push, 1)
struct D2C2SCreateGameStrc
{
    uint8_t  nOpcode;        // +0x00 (0x67)
    char     szGameName[16]; // +0x01
    uint8_t  nGameType;      // +0x11
    uint8_t  nCharClass;     // +0x12
    uint8_t  nTemplate;      // +0x13
    uint8_t  nDifficulty;    // +0x14
    char     szCharName[16]; // +0x15
    uint16_t wFlags1;        // +0x25
    uint32_t dwArenaFlags;   // +0x27
    uint8_t  nUnk2B;         // +0x2B
    uint8_t  nUnk2C;         // +0x2C
    uint8_t  nLocale;        // +0x2D
};
#pragma pack(pop)

int32_t __fastcall MSG_SendCreateGame_67(
    const char* szGameName,
    uint8_t nGameType,
    uint8_t nCharClass,
    uint8_t nTemplate,
    uint8_t nDifficulty,
    const char* szCharName,
    uint16_t wFlags1,
    uint32_t dwArenaFlags,
    uint8_t nUnk2B,
    uint8_t nUnk2C,
    uint8_t nLocale)
{
    uint8_t abPacket[0x2E] = { 0 };
    abPacket[0] = C2S_CREATEGAME;
    strncpy(reinterpret_cast<char*>(&abPacket[1]), szGameName, 15);
    abPacket[16] = 0;
    abPacket[0x11] = nGameType;
    abPacket[0x12] = nCharClass;
    abPacket[0x13] = nTemplate;
    abPacket[0x14] = nDifficulty;
    if (szCharName != nullptr)
    {
        strncpy(reinterpret_cast<char*>(&abPacket[0x15]), szCharName, 15);
        abPacket[0x24] = 0;
    }
    *reinterpret_cast<uint16_t*>(&abPacket[0x25]) = wFlags1;
    *reinterpret_cast<uint32_t*>(&abPacket[0x27]) = dwArenaFlags;
    abPacket[0x2B] = nUnk2B;
    abPacket[0x2C] = nUnk2C;
    abPacket[0x2D] = nLocale;

    const int32_t nSent = CLIENT_Send(0, abPacket, 0x1D);
    if (nSent != 0)
    {
        g_dwBytesSent += nSent;
        g_dwPacketsSent += 1;
    }
    return nSent;
}

// 0x6FB295F0
// Sends 0x66 (C2S_WARDENRESPONSE).
int32_t __fastcall MSG_SendWardenResponse_66(uint16_t wPayloadSize, const void* pPayload)
{
    const size_t nTotalSize = static_cast<size_t>(wPayloadSize) + 3;
    uint8_t* pPacket = static_cast<uint8_t*>(FOG_AllocClientMemory(nTotalSize, "..\\Source\\D2Client\\GAME\\Msg.cpp", 460, 0));
    pPacket[0] = C2S_WARDENRESPONSE;
    *reinterpret_cast<uint16_t*>(&pPacket[1]) = wPayloadSize;
    memcpy(&pPacket[3], pPayload, wPayloadSize);

    const int32_t nSent = CLIENT_Send(1, pPacket, static_cast<int32_t>(nTotalSize));
    if (nSent != 0)
    {
        g_dwBytesSent += nSent;
        g_dwPacketsSent += 1;
    }
    FOG_FreeClientMemory(pPacket, "..\\Source\\D2Client\\GAME\\Msg.cpp", 467, 0);
    return nSent;
}

// 0x6FB29670
// Sends 0x14 (C2S_OVERHEADCHAT) or 0x15 (C2S_CHATMESSAGE).
int32_t __fastcall MSG_SendChat_14_15(uint8_t bOpcode, uint8_t nChatType, const wchar_t* wszMessage, const char* szTargetName)
{
    uint8_t abPacket[0x120] = { 0 };
    abPacket[0] = bOpcode;
    abPacket[1] = nChatType;

    const int32_t nUtf8Len = Unicode_win2utf8(reinterpret_cast<char*>(&abPacket[3]), wszMessage, 260);
    size_t nNameLen = 0;
    if (szTargetName != nullptr)
    {
        nNameLen = strlen(szTargetName);
        strcpy(reinterpret_cast<char*>(&abPacket[3 + nUtf8Len + 1]), szTargetName);
    }

    return MSG_SendPacket(abPacket, nUtf8Len + nNameLen + 6);
}

// 0x6FB296E0
// Sends 0x5E (C2S_PARTYINVITE) - 6 bytes: [0x5E, nButtonAction, dwTargetPlayerGUID].
int32_t __fastcall MSG_SendPartyInvite_5E(uint8_t nButtonAction, uint32_t dwTargetPlayerGUID)
{
#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint8_t  nAction;
        uint32_t dwPlayerGUID;
    } pkt;
#pragma pack(pop)

    pkt.bOpcode = C2S_PARTYINVITE;
    pkt.nAction = nButtonAction;
    pkt.dwPlayerGUID = dwTargetPlayerGUID;
    return MSG_SendPacket(&pkt, sizeof(pkt));
}

// 0x6FB29720
// Sends 0x5D (C2S_PARTYACTION) - 7 bytes: [0x5D, nSubAction, bFlag, dwTargetPlayerGUID].
int32_t __fastcall MSG_SendPartyAction_5D(uint8_t nSubAction, uint8_t bFlag, uint32_t dwTargetPlayerGUID)
{
#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint8_t  nSubAction;
        uint8_t  bFlag;
        uint32_t dwPlayerGUID;
    } pkt;
#pragma pack(pop)

    pkt.bOpcode = C2S_PARTYACTION;
    pkt.nSubAction = nSubAction;
    pkt.bFlag = bFlag;
    pkt.dwPlayerGUID = dwTargetPlayerGUID;
    return MSG_SendPacket(&pkt, sizeof(pkt));
}

// 0x6FB29770
// Generic 17-byte packet builder: [uint8_t bOpcode, uint32_t dw1, uint32_t dw2, uint32_t dw3, uint32_t dw4].
// Used by:
//   - 0x18 (C2S_INSERTITEMTOBUFFER)
//   - 0x32 (C2S_TOWNFOLKBUY)
//   - 0x33 (C2S_TOWNFOLKSELL)
//   - 0x35 (C2S_TOWNFOLKREPAIR)
//   - 0x44 (C2S_STAFFINTOORIFICE)
//   - 0x59 (C2S_TOWNFOLKINVITE)
int32_t __fastcall MSG_SendByte4Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2, uint32_t dw3, uint32_t dw4)
{
#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint32_t dw1;
        uint32_t dw2;
        uint32_t dw3;
        uint32_t dw4;
    } pkt;
#pragma pack(pop)

    pkt.bOpcode = bOpcode;
    pkt.dw1 = dw1;
    pkt.dw2 = dw2;
    pkt.dw3 = dw3;
    pkt.dw4 = dw4;
    return MSG_SendPacket(&pkt, sizeof(pkt));
}

// 0x6FB297B0
// Generic 13-byte packet builder: [uint8_t bOpcode, uint32_t dw1, uint32_t dw2, uint32_t dw3].
// Used by:
//   - 0x16 (C2S_PICKUPITEM: [0x16, UNIT_ITEM(4), dwItemGUID, bToCursor])
//   - 0x26 (C2S_USEBELTITEM: [0x26, dwItemGUID, bShiftToMerc, 0])
//   - 0x38 (C2S_TOWNFOLKMENUSELECTIONS: [0x38, dwAction, dwNpcGUID, dwParam])
int32_t __fastcall MSG_SendByte3Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2, uint32_t dw3)
{
#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint32_t dw1;
        uint32_t dw2;
        uint32_t dw3;
    } pkt;
#pragma pack(pop)

    pkt.bOpcode = bOpcode;
    pkt.dw1 = dw1;
    pkt.dw2 = dw2;
    pkt.dw3 = dw3;
    return MSG_SendPacket(&pkt, sizeof(pkt));
}

// 0x6FB297E0
// Generic 9-byte packet builder: [uint8_t bOpcode, uint32_t dw1, uint32_t dw2].
// Used by:
//   - 0x2F (C2S_TOWNFOLKINIT:   [0x2F, dwUnitType, dwUnitGUID])
//   - 0x30 (C2S_TOWNFOLKCANCEL: [0x30, dwUnitType, dwUnitGUID])
//   - 0x25, 0x27, 0x28, 0x29, 0x2A, 0x31, 0x36, 0x49, 0x4B, 0x50
int32_t __fastcall MSG_SendByte2Dwords(uint8_t bOpcode, uint32_t dw1, uint32_t dw2)
{
#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint32_t dw1;
        uint32_t dw2;
    } pkt;
#pragma pack(pop)

    pkt.bOpcode = bOpcode;
    pkt.dw1 = dw1;
    pkt.dw2 = dw2;
    return MSG_SendPacket(&pkt, sizeof(pkt));
}

// 0x6FB29810
// Generic 5-byte packet builder: [uint8_t bOpcode, uint32_t dw1].
// Used by:
//   - 0x17 (C2S_DROPITEM)
//   - 0x19 (C2S_REMOVEITEMFROMBUFFER)
//   - 0x24 (C2S_REMOVEBELTITEM)
//   - 0x34 (C2S_CAINIDENTIFY)
//   - 0x37 (C2S_GAMBLEIDENTIFY)
//   - 0x62 (C2S_RESURRECTMERC)
//   - 0x63 (C2S_BELTSHIFTCLICK)
int32_t __fastcall MSG_SendByteDword(uint8_t bOpcode, uint32_t dw1)
{
#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint32_t dw1;
    } pkt;
#pragma pack(pop)

    pkt.bOpcode = bOpcode;
    pkt.dw1 = dw1;
    return MSG_SendPacket(&pkt, sizeof(pkt));
}

// 0x6FB29830
// Generic 9-byte packet builder: [uint8_t bOpcode, uint16_t w1, uint16_t w2, uint16_t w3, uint16_t w4].
// Used by:
//   - 0x1A, 0x1B, 0x1D, 0x23 (Equip/Swap item with bodyloc)
//   - 0x3C (C2S_SELECTSKILL: [0x3C, wSkillId, wLeftFlag, wItemGUIDLow, wItemGUIDHigh])
//   - 0x51 (C2S_ASSIGNHOTKEY)
int32_t __fastcall MSG_SendByte4Words(uint8_t bOpcode, uint16_t w1, uint16_t w2, uint16_t w3, uint16_t w4)
{
#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint16_t w1;
        uint16_t w2;
        uint16_t w3;
        uint16_t w4;
    } pkt;
#pragma pack(pop)

    pkt.bOpcode = bOpcode;
    pkt.w1 = w1;
    pkt.w2 = w2;
    pkt.w3 = w3;
    pkt.w4 = w4;
    return MSG_SendPacket(&pkt, sizeof(pkt));
}

// 0x6FB29870
// Generic 7-byte packet builder: [uint8_t bOpcode, uint16_t w1, uint16_t w2, uint16_t w3].
// Used by:
//   - 0x4F (C2S_TRADEBUTTON)
int32_t __fastcall MSG_SendByte3Words(uint8_t bOpcode, uint16_t w1, uint16_t w2, uint16_t w3)
{
#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint16_t w1;
        uint16_t w2;
        uint16_t w3;
    } pkt;
#pragma pack(pop)

    pkt.bOpcode = bOpcode;
    pkt.w1 = w1;
    pkt.w2 = w2;
    pkt.w3 = w3;
    return MSG_SendPacket(&pkt, sizeof(pkt));
}

// 0x6FB298B0
// Generic 5-byte packet builder: [uint8_t bOpcode, uint16_t w1, uint16_t w2].
// Used by:
//   - 0x01 (C2S_WALKTOLOCATION:       [0x01, wX, wY])
//   - 0x03 (C2S_RUNTOLOCATION:        [0x03, wX, wY])
//   - 0x05 (C2S_LEFTSKILLONLOCATION:  [0x05, wX, wY])
//   - 0x08 (C2S_LEFTSKILLONLOCATIONEX:[0x08, wX, wY])
//   - 0x0C (C2S_RIGHTSKILLONLOCATION: [0x0C, wX, wY])
//   - 0x0F (C2S_RIGHTSKILLONLOCATIONEX:[0x0F, wX, wY])
//   - 0x5F (C2S_UPDATEPOSITION:       [0x5F, wX, wY])
int32_t __fastcall MSG_SendByteWordWord(uint8_t bOpcode, uint16_t w1, uint16_t w2)
{
#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint16_t w1;
        uint16_t w2;
    } pkt;
#pragma pack(pop)

    pkt.bOpcode = bOpcode;
    pkt.w1 = w1;
    pkt.w2 = w2;
    return MSG_SendPacket(&pkt, sizeof(pkt));
}

// 0x6FB298E0
// Generic 3-byte packet builder: [uint8_t bOpcode, uint16_t w1].
// Used by:
//   - 0x1C (C2S_UNEQUIPITEM)
//   - 0x3A (C2S_INCREMENTSTAT)
//   - 0x3B (C2S_INCREMENTSKILL)
//   - 0x3F (C2S_ACTIVATEITEM)
//   - 0x61 (C2S_MERCITEM)
int32_t __fastcall MSG_SendByteWord(uint8_t bOpcode, uint16_t w1)
{
#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint16_t w1;
    } pkt;
#pragma pack(pop)

    pkt.bOpcode = bOpcode;
    pkt.w1 = w1;
    return MSG_SendPacket(&pkt, sizeof(pkt));
}

// 0x6FB29900
// Generic 1-byte packet builder: [uint8_t bOpcode].
// Used by:
//   - 0x40 (C2S_QUESTLOG)
//   - 0x41 (C2S_RESURRECT)
//   - 0x60 (C2S_SWITCHWEAPONS)
int32_t __fastcall MSG_SendByte(uint8_t bOpcode)
{
    uint8_t bPacket = bOpcode;
    return MSG_SendPacket(&bPacket, 1);
}

// 0x6FB29920
// Sends 0x68 (C2S_JOINGAME) - 37 bytes.
int32_t __fastcall MSG_SendJoinGame_68(
    uint32_t dwServerHash,
    uint16_t wServerToken,
    uint8_t nCharClass,
    uint32_t dwClientVersion,
    uint32_t dwArenaFlags,
    uint8_t nLocale,
    const char* szCharName)
{
    uint8_t abPacket[0x25] = { 0 };
    abPacket[0] = C2S_JOINGAME;
    *reinterpret_cast<uint32_t*>(&abPacket[0x01]) = dwServerHash;
    *reinterpret_cast<uint16_t*>(&abPacket[0x05]) = wServerToken;
    abPacket[0x07] = nCharClass;
    *reinterpret_cast<uint32_t*>(&abPacket[0x08]) = dwClientVersion;
    *reinterpret_cast<uint32_t*>(&abPacket[0x0C]) = (dwArenaFlags & 0x00800800) != 0 ? 0xED5DCC50u : 0x2185EDFEu;
    *reinterpret_cast<uint32_t*>(&abPacket[0x10]) = 0;
    abPacket[0x14] = nLocale;
    if (szCharName != nullptr)
    {
        strncpy(reinterpret_cast<char*>(&abPacket[0x15]), szCharName, 15);
        abPacket[0x24] = 0;
    }

    const int32_t nSent = CLIENT_Send(0, abPacket, 0x25);
    if (nSent != 0)
    {
        g_dwBytesSent += nSent;
        g_dwPacketsSent += 1;
    }
    return nSent;
}

// 0x6FB299D0
// Periodic keepalive ping sender: sends 0x6D (C2S_PING, 13 bytes) every 5000ms.
void __fastcall MSG_SendPing_6D(void)
{
    const uint32_t dwNow = GetTickCount();
    if (dwNow - g_dwLastPingTick < 5000)
    {
        return;
    }

#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint32_t dwTickCount;
        uint32_t dwPongDelta;
        uint32_t dwZero;
    } pkt;
#pragma pack(pop)

    pkt.bOpcode = C2S_PING;
    pkt.dwTickCount = GetTickCount();
    pkt.dwPongDelta = pkt.dwTickCount - g_dwPingTimestamp;
    pkt.dwZero = 0;

    const int32_t nSent = CLIENT_Send(0, &pkt, sizeof(pkt));
    if (nSent != 0)
    {
        g_dwBytesSent += nSent;
        g_dwPacketsSent += 1;
    }
    g_dwLastPingTick = dwNow;
}

// 0x6FB29B50
// Uploads local single-player / open character `.d2s` save file in 250-byte chunks
// using C->S opcode 0x6C (C2S_UPLOADSAVECHUNK).
int32_t __fastcall MSG_UploadSaveFile_6C(void* pSaveBuffer, uint32_t dwSaveSize)
{
#pragma pack(push, 1)
    struct
    {
        uint8_t  bOpcode;
        uint8_t  nChunkBytes;
        uint32_t dwTotalSaveSize;
        uint8_t  abChunk[256];
    } pkt;
#pragma pack(pop)

    uint8_t* pCursor = static_cast<uint8_t*>(pSaveBuffer);
    uint32_t dwRemaining = dwSaveSize;

    pkt.bOpcode = C2S_UPLOADSAVECHUNK;

    while (dwRemaining > 255)
    {
        pkt.nChunkBytes = 255;
        pkt.dwTotalSaveSize = dwSaveSize;
        memcpy(pkt.abChunk, pCursor, 255);
        pCursor += 255;
        dwRemaining -= 255;

        const int32_t nSent = CLIENT_Send(0, &pkt, 0x106);
        if (nSent != 0)
        {
            g_dwBytesSent += nSent;
            g_dwPacketsSent += 1;
        }
        Sleep(20);
    }

    pkt.nChunkBytes = static_cast<uint8_t>(dwRemaining);
    pkt.dwTotalSaveSize = dwSaveSize;
    memcpy(pkt.abChunk, pCursor, dwRemaining);

    const int32_t nSent = CLIENT_Send(0, &pkt, static_cast<int32_t>(dwRemaining + 7));
    if (nSent != 0)
    {
        g_dwBytesSent += nSent;
        g_dwPacketsSent += 1;
    }
    g_bSaveUploadComplete = 1;
    return 1;
}

// =============================================================================
// SECTION 2: Local Input & Movement C->S Dispatchers (UNIT/UnitMode.cpp & UNIT/Player.cpp)
// =============================================================================

// 0x6FAF0DC0 (UNIT/UnitMode.cpp)
// Central action packet dispatcher for opcodes 0x01 .. 0x11:
//   - Location actions (0x01 Walk, 0x03 Run, 0x05 LeftSkill, 0x08 ShiftLeftSkill,
//     0x0C RightSkill, 0x0F ShiftRightSkill) -> MSG_SendByteWordWord(bOpcode, wX, wY)
//   - Unit-targeted actions (0x02 WalkToUnit, 0x04 RunToUnit, 0x06/0x07/0x09/0x0A LeftSkillOnUnit,
//     0x0D/0x0E/0x10/0x11 RightSkillOnUnit) -> MSG_SendPacket([bOpcode, dwUnitType, dwUnitGUID], 9)
int32_t __fastcall UNITMODE_SendActionPacket(
    uint8_t bOpcode,
    uint16_t wTargetX,
    uint16_t wTargetY,
    uint32_t dwTargetUnitType,
    uint32_t dwTargetUnitGUID)
{
    if (bOpcode < C2S_WALKTOLOCATION || bOpcode > C2S_RIGHTSKILLONUNITREPEATEX)
    {
        return 0;
    }

    switch (bOpcode)
    {
    case C2S_WALKTOLOCATION:          // 0x01
    case C2S_RUNTOLOCATION:           // 0x03
    case C2S_LEFTSKILLONLOCATION:     // 0x05
    case C2S_LEFTSKILLONLOCATIONEX:   // 0x08
    case C2S_RIGHTSKILLONLOCATION:    // 0x0C
    case C2S_RIGHTSKILLONLOCATIONEX:  // 0x0F
        return MSG_SendByteWordWord(bOpcode, wTargetX, wTargetY);

    case C2S_WALKTOUNIT:              // 0x02
    case C2S_RUNTOUNIT:               // 0x04
    case C2S_LEFTSKILLONUNIT:         // 0x06
    case C2S_LEFTSKILLONUNITEX:       // 0x07
    case C2S_LEFTSKILLONUNITREPEAT:   // 0x09
    case C2S_LEFTSKILLONUNITREPEATEX: // 0x0A
    case C2S_RIGHTSKILLONUNIT:        // 0x0D
    case C2S_RIGHTSKILLONUNITEX:      // 0x0E
    case C2S_RIGHTSKILLONUNITREPEAT:  // 0x10
    case C2S_RIGHTSKILLONUNITREPEATEX:// 0x11
    {
#pragma pack(push, 1)
        struct
        {
            uint8_t  bOpcode;
            uint32_t dwUnitType;
            uint32_t dwUnitGUID;
        } pkt;
#pragma pack(pop)

        pkt.bOpcode = bOpcode;
        pkt.dwUnitType = dwTargetUnitType;
        pkt.dwUnitGUID = dwTargetUnitGUID;
        return MSG_SendPacket(&pkt, sizeof(pkt));
    }

    case C2S_SKILLUNKNOWN_0B:         // 0x0B
    default:
        return 0;
    }
}

// 0x6FAF14C0 (UNIT/UnitMode.cpp)
// Executes a local client movement/skill state transition via UNITMODE_DispatchClientEvent
// and sends the corresponding C->S action packet (0x01..0x11) if the transition succeeds.
int32_t __fastcall UNITMODE_StartClientAction(
    D2UnitStrc* pPlayer,
    int32_t nClientEvent,
    uint8_t bOpcode,
    int32_t nTargetX,
    int32_t nTargetY,
    D2UnitStrc* pTargetUnit,
    int32_t a7,
    int32_t a8)
{
    uint32_t dwUnitType = 0;
    uint32_t dwUnitGUID = 0;

    if (pTargetUnit != nullptr)
    {
        dwUnitType = pTargetUnit->dwUnitType;
        dwUnitGUID = pTargetUnit->dwUnitId;
    }

    if (!UNITMODE_DispatchClientEvent(pPlayer, nClientEvent, nTargetX, nTargetY, pTargetUnit, a7, a8))
    {
        return 0;
    }

    // Skill cast events (19..22) increment client stat 0x148 (328)
    if (nClientEvent == 19 || nClientEvent == 20 || nClientEvent == 21 || nClientEvent == 22)
    {
        STATLIST_AddUnitStat(pPlayer, 0x148, 1, 0);
    }

    UNITMODE_SendActionPacket(
        bOpcode,
        static_cast<uint16_t>(nTargetX),
        static_cast<uint16_t>(nTargetY),
        dwUnitType,
        dwUnitGUID);

    return 1;
}

// 0x6FAF1740 (UNIT/UnitMode.cpp)
// Moves the local player toward an interacted unit (WalkToUnit 0x02 or RunToUnit 0x04).
// Uses D2Common.#10142 (PATH_SetTargetUnit) via UNITMODE_DispatchClientEvent.
int32_t __fastcall UNITMODE_MoveToTargetUnit(
    D2UnitStrc* pPlayer,
    int32_t nTargetX,
    int32_t nTargetY,
    uint32_t dwTargetUnitType,
    uint32_t dwTargetUnitGUID)
{
    D2UnitStrc* pTargetUnit = UNIT_FindClientUnit(dwTargetUnitGUID, dwTargetUnitType);
    if (pTargetUnit == nullptr)
    {
        return 0;
    }

    bool bShouldRun = false;
    if (gbRunLock != 0)
    {
        if (gbRunKeyHeld == 0 && gbAlwaysRun != 0)
        {
            bShouldRun = true;
        }
    }
    else if (gbRunKeyHeld != 0 || gbAlwaysRun != 0)
    {
        bShouldRun = true;
    }

    // Cannot run if stamina is drained and Freeze/Stamina states prohibit running
    if (bShouldRun)
    {
        if (STATLIST_UnitGetStatValue(pPlayer, STAT_STAMINA, 0) == 0 &&
            !STATES_CheckState(pPlayer, STATE_STAMINA))
        {
            bShouldRun = false;
        }
        if (STATES_CheckState(pPlayer, STATE_FREEZE))
        {
            bShouldRun = false;
        }
    }

    const int32_t nClientEvent = bShouldRun ? 0x18 : 0x17; // 0x18 = RunToUnit, 0x17 = WalkToUnit
    const uint8_t bOpcode = bShouldRun ? C2S_RUNTOUNIT : C2S_WALKTOUNIT;

    if (UNITMODE_DispatchClientEvent(pPlayer, nClientEvent, nTargetX, nTargetY, pTargetUnit, 1, 1))
    {
#pragma pack(push, 1)
        struct
        {
            uint8_t  bOpcode;
            uint32_t dwUnitType;
            uint32_t dwUnitGUID;
        } pkt;
#pragma pack(pop)

        pkt.bOpcode = bOpcode;
        pkt.dwUnitType = dwTargetUnitType;
        pkt.dwUnitGUID = dwTargetUnitGUID;
        MSG_SendPacket(&pkt, sizeof(pkt));
        return 1;
    }

    // If currently mid-action, queue the movement in pPlayerData (+0x150..+0x15C)
    UNITMODE_QueuePendingPlayerAction(pPlayer, nClientEvent, nTargetX, nTargetY, pTargetUnit);
    return 0;
}

// 0x6FAF1910 (UNIT/UnitMode.cpp)
// Sends C->S 0x13 (C2S_UNITINTERACT) or 0x16 (C2S_PICKUPITEM) when the player
// reaches interaction range of a targeted monster/NPC, object, warp tile, or ground item.
void __fastcall UNITMODE_SendUnitInteract_13(D2UnitStrc* pPlayer, uint32_t dwTargetUnitType, uint32_t dwTargetUnitGUID)
{
    D2UnitStrc* pTarget = UNIT_FindClientUnit(dwTargetUnitGUID, dwTargetUnitType);
    if (pTarget == nullptr)
    {
        return;
    }

    // Clear path target unit and face the target coordinates
    // D2Common.#10142: PATH_SetTargetUnit
    PATH_SetTargetUnit(pPlayer->pDynamicPath, nullptr);

    const int32_t nTargetX = UNITS_GetClientCoordX(pTarget);
    const int32_t nTargetY = UNITS_GetClientCoordY(pTarget);
    // D2Common.#10742: UNITS_UpdateDirectionAndSpeed
    UNITS_UpdateDirectionAndSpeed(pPlayer, nTargetX, nTargetY);

    switch (dwTargetUnitType)
    {
    case UNIT_MONSTER: // 1: NPC dialog / dead monster rummage
    case UNIT_OBJECT:  // 2: Chest, shrine, portal, waypoint, quest object
    {
        if (dwTargetUnitType == UNIT_OBJECT)
        {
            // Throttle repeated shrine/object clicks to once every 200ms (0xC8) per object
            const uint32_t dwNow = GetTickCount();
            uint32_t* pLastInteractTick = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(pTarget) + 0xD4);
            if (*pLastInteractTick != 0 && (dwNow - *pLastInteractTick < 200))
            {
                return;
            }
            *pLastInteractTick = dwNow;
        }

#pragma pack(push, 1)
        struct
        {
            uint8_t  bOpcode;
            uint32_t dwUnitType;
            uint32_t dwUnitGUID;
        } pkt;
#pragma pack(pop)

        pkt.bOpcode = C2S_UNITINTERACT; // 0x13
        pkt.dwUnitType = dwTargetUnitType;
        pkt.dwUnitGUID = dwTargetUnitGUID;
        MSG_SendPacket(&pkt, sizeof(pkt));
        break;
    }

    case UNIT_ITEM: // 4: Ground item pickup -> C2S_PICKUPITEM (0x16)
    {
        // [0x16, UNIT_ITEM(4), dwItemGUID, bToCursor(0)]
        MSG_SendByte3Dwords(C2S_PICKUPITEM, UNIT_ITEM, dwTargetUnitGUID, 0);
        break;
    }

    case UNIT_TILE: // 5: Warp / level transition tile -> C2S_UNITINTERACT (0x13)
    {
#pragma pack(push, 1)
        struct
        {
            uint8_t  bOpcode;
            uint32_t dwUnitType;
            uint32_t dwUnitGUID;
        } pkt;
#pragma pack(pop)

        pkt.bOpcode = C2S_UNITINTERACT; // 0x13
        pkt.dwUnitType = UNIT_TILE;
        pkt.dwUnitGUID = dwTargetUnitGUID;
        MSG_SendPacket(&pkt, sizeof(pkt));
        break;
    }

    default:
        break;
    }
}

// 0x6FAF0EB0 (UNIT/UnitMode.cpp)
// Position drift reconciliation (called from S->C 0x0C..0x10 and 0x95/0x96).
// Uses D2Common.#10186 (PATH_ComputeSquaredDistance) and D2Common.#10504 (COLLISION_GetFreeCoordinates).
// If the local player has drifted and needs to report their position back to the server,
// sends C->S 0x5F (C2S_UPDATEPOSITION: [0x5F, wX, wY]).
int32_t __fastcall UNITMODE_CheckPositionDrift(D2UnitStrc* pUnit, int32_t nServerX, int32_t nServerY)
{
    if (pUnit == nullptr || (nServerX == 0 && nServerY == 0))
    {
        return 0;
    }

    const int32_t nClientX = UNITS_GetClientCoordX(pUnit);
    const int32_t nClientY = UNITS_GetClientCoordY(pUnit);

    // D2Common.#10186: PATH_ComputeSquaredDistance
    const int32_t nDist = PATH_ComputeSquaredDistance(nClientX, nClientY, nServerX, nServerY);
    if (nDist <= 13)
    {
        if (nDist < 3)
        {
            return 0;
        }

        // For local player in neutral mode, send C->S 0x5F position sync
        if (pUnit == UNIT_GetClientPlayer() && pUnit->dwAnimMode == PLRMODE_NEUTRAL)
        {
            MSG_SendByteWordWord(
                C2S_UPDATEPOSITION,
                static_cast<uint16_t>(nClientX),
                static_cast<uint16_t>(nClientY));
        }
        return 0;
    }

    return 1;
}

// 0x6FB0C530 (UNIT/Player.cpp)
// Resolves wall/obstacle collisions for local walk/run target coordinates before
// dispatching pathfinding and C->S 0x01/0x03 packets.
// Calls:
//   - D2Common.#10363 (PATH_GetUnitCollisionPattern)
//   - D2Common.#10807 (COLLISION_CheckMaskWithPattern) with mask 0xC01
//   - D2Common.#10504 (COLLISION_GetFreeCoordinates)
void __fastcall PLAYER_ResolveWallCollision(D2UnitStrc* pPlayer, D2CoordStrc* pTargetCoord)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pPlayer);
    if (pRoom == nullptr)
    {
        return;
    }

    const uint32_t nCollisionPattern = PATH_GetUnitCollisionPattern(pPlayer);
    if (COLLISION_CheckMaskWithPattern(pRoom, pTargetCoord->nX, pTargetCoord->nY, nCollisionPattern, 0x0C01))
    {
        COLLISION_GetFreeCoordinates(pRoom, pTargetCoord, UNITS_GetUnitSizeX(pPlayer), 0x0C01, 0);
    }
}

// 0x6FB0BB10 (UNIT/Player.cpp)
// Checks interaction range and line-of-sight collision between the local player
// and a clicked target unit (NPC, object, item, warp), inviting NPCs to move closer
// via C->S 0x59 (C2S_TOWNFOLKINVITE) and engaging interaction via C->S 0x13 (C2S_UNITINTERACT).
// Calls:
//   - D2Common.#10448 (UNITS_IsObjectInInteractRange)
//   - D2Common.#10302 (UNITS_TestCollisionBetweenInteractingUnits) with mask 0x0804
//   - D2Common.#10397 (UNITS_GetDistanceToOtherUnit)
void __fastcall PLAYER_InteractWithUnit(D2UnitStrc* pPlayer, D2UnitStrc* pTargetUnit)
{
    if (pPlayer == nullptr || pTargetUnit == nullptr)
    {
        return;
    }

    // If interacting with a friendly town NPC, send C->S 0x59 (C2S_TOWNFOLKINVITE)
    // so the server halts the NPC's patrol walk and faces the player.
    if (pTargetUnit->dwUnitType == UNIT_MONSTER &&
        STATLIST_GetUnitAlignment(pTargetUnit) == UNIT_ALIGNMENT_GOOD)
    {
        const int32_t nPlayerX = UNITS_GetClientCoordX(pPlayer);
        const int32_t nPlayerY = UNITS_GetClientCoordY(pPlayer);
        MSG_SendByte4Dwords(
            C2S_TOWNFOLKINVITE,
            pTargetUnit->dwUnitType,
            pTargetUnit->dwUnitId,
            static_cast<uint32_t>(nPlayerX),
            static_cast<uint32_t>(nPlayerY));
    }

    // Check if already within interaction range and unblocked by walls (mask 0x0804)
    bool bInRange = false;
    if (pTargetUnit->dwUnitType == UNIT_OBJECT)
    {
        bInRange = (UNITS_IsObjectInInteractRange(pPlayer, pTargetUnit) != 0) &&
                   (UNITS_TestCollisionBetweenInteractingUnits(pTargetUnit, pPlayer, 0x0804) == 0);
    }
    else
    {
        bInRange = (UNITS_GetDistanceToOtherUnit(pPlayer, pTargetUnit) <= 2) &&
                   (UNITS_TestCollisionBetweenInteractingUnits(pPlayer, pTargetUnit, 0x0804) == 0);
    }

    if (bInRange)
    {
        UNITMODE_SendUnitInteract_13(pPlayer, pTargetUnit->dwUnitType, pTargetUnit->dwUnitId);
    }
    else
    {
        const int32_t nTargetX = UNITS_GetClientCoordX(pTargetUnit);
        const int32_t nTargetY = UNITS_GetClientCoordY(pTargetUnit);
        UNITMODE_MoveToTargetUnit(pPlayer, nTargetX, nTargetY, pTargetUnit->dwUnitType, pTargetUnit->dwUnitId);
    }
}

// 0x6FB0B050 (UNIT/Player.cpp)
// Dispatches a left-hand or right-hand skill cast at a location or target unit,
// choosing the exact C->S opcode:
//   - Left skill on location:  0x05 (C2S_LEFTSKILLONLOCATION)  or 0x08 (Shift/Repeat)
//   - Left skill on unit:      0x06 (C2S_LEFTSKILLONUNIT)      or 0x07 / 0x09 / 0x0A
//   - Right skill on location: 0x0C (C2S_RIGHTSKILLONLOCATION) or 0x0F (Shift/Repeat)
//   - Right skill on unit:     0x0D (C2S_RIGHTSKILLONUNIT)     or 0x0E / 0x10 / 0x11
int32_t __fastcall PLAYER_ExecuteSkillCast(
    D2UnitStrc* pPlayer,
    int32_t bIsLeftSkill,
    int32_t bIsRepeatOrShift,
    int32_t bIsMovingSkill,
    int32_t nTargetX,
    int32_t nTargetY,
    D2UnitStrc* pTargetUnit)
{
    uint8_t bOpcode = 0;
    int32_t nClientEvent = 0;

    if (pTargetUnit == nullptr)
    {
        nClientEvent = bIsLeftSkill ? 19 : 21; // LeftSkillOnLocation (19) / RightSkillOnLocation (21)
        if (bIsLeftSkill)
        {
            bOpcode = bIsRepeatOrShift ? C2S_LEFTSKILLONLOCATIONEX : C2S_LEFTSKILLONLOCATION; // 0x08 : 0x05
        }
        else
        {
            bOpcode = bIsRepeatOrShift ? C2S_RIGHTSKILLONLOCATIONEX : C2S_RIGHTSKILLONLOCATION; // 0x0F : 0x0C
        }
    }
    else
    {
        nClientEvent = bIsLeftSkill ? 20 : 22; // LeftSkillOnUnit (20) / RightSkillOnUnit (22)
        if (bIsLeftSkill)
        {
            if (bIsRepeatOrShift)
            {
                bOpcode = bIsMovingSkill ? C2S_LEFTSKILLONUNITREPEATEX : C2S_LEFTSKILLONUNITREPEAT; // 0x0A : 0x09
            }
            else
            {
                bOpcode = bIsMovingSkill ? C2S_LEFTSKILLONUNITEX : C2S_LEFTSKILLONUNIT; // 0x07 : 0x06
            }
        }
        else
        {
            if (bIsRepeatOrShift)
            {
                bOpcode = bIsMovingSkill ? C2S_RIGHTSKILLONUNITREPEATEX : C2S_RIGHTSKILLONUNITREPEAT; // 0x11 : 0x10
            }
            else
            {
                bOpcode = bIsMovingSkill ? C2S_RIGHTSKILLONUNITEX : C2S_RIGHTSKILLONUNIT; // 0x0E : 0x0D
            }
        }
    }

    return UNITMODE_StartClientAction(pPlayer, nClientEvent, bOpcode, nTargetX, nTargetY, pTargetUnit, 1, 1);
}

// =============================================================================
// SECTION 3: TownFolkInit (0x2F) & TownFolkCancel (0x30) in GAME/Game.cpp
// =============================================================================

// 0x6FB32015 (inside GAME_StartNpcDialog at 0x6FB31FE0 in GAME/Game.cpp)
// Sends C->S 0x2F (C2S_TOWNFOLKINIT: [0x2F, dwNpcUnitType, dwNpcUnitGUID]) when
// the player enters conversation range with an NPC.
int32_t __fastcall GAME_SendTownFolkInit_2F(uint32_t dwNpcUnitType, uint32_t dwNpcUnitGUID)
{
    return MSG_SendByte2Dwords(C2S_TOWNFOLKINIT, dwNpcUnitType, dwNpcUnitGUID);
}

// 0x6FB305BB (inside GAME_CloseNpcMenu at 0x6FB305A0 in GAME/Game.cpp)
// Sends C->S 0x30 (C2S_TOWNFOLKCANCEL: [0x30, dwNpcUnitType, dwNpcUnitGUID]) when
// the player closes the NPC dialog/trade menu.
int32_t __fastcall GAME_SendTownFolkCancel_30(uint32_t dwNpcUnitType, uint32_t dwNpcUnitGUID)
{
    return MSG_SendByte2Dwords(C2S_TOWNFOLKCANCEL, dwNpcUnitType, dwNpcUnitGUID);
}
