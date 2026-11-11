// =============================================================================
// D2Client.dll 1.12a (ImageBase = 0x6FAB0000)
// Reconstructed Translation Unit #28 of 55: UI/showitems.cpp
//
// Source Assertion Path: "..\Source\D2Client\UI\showitems.cpp" (0x6FB84BC4)
// .text Range : 0x6FB26E00..0x6FB27890 (0x00A90 bytes)
// Anchor Range: 0x6FB27180..0x6FB27890 (plus preceding tail functions 0x6FB26E00..0x6FB2717D)
//
// Responsibilities:
//   - ALT ground item label layout, screen clamping, and iterative AABB overlap avoidance
//   - Perspective and isometric screen projection of ground items across adjacent rooms
//   - Cursor hit-testing against ground item label boxes and item quality/socket/ethereal coloring
//   - Framed text box rendering via D2Win (#10082 / #10031)
//   - Preceding unit tail (0x6FB26E00..0x6FB2717D): NPC greeting/farewell/cycle audio lookup
// =============================================================================

#include <stdint.h>
#include <string.h>
#include <windows.h>

#include <D2BasicTypes.h>
#include <Units/Units.h>
#include <D2Items.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2Environment.h>
#include <D2Gfx.h>
#include <Scale.h>
#include <D2WinFont.h>
#include <D2WinPalette.h>
#include <Fog.h>
#include <D2Unicode.h>

#pragma pack(push, 1)

// 0x18-byte (24-byte) NPC greeting / farewell audio table entry (0x6FB905F0..0x6FB908A7)
struct D2NpcSpeechRecordStrc
{
    int32_t  nGreeting1;        // +0x00
    int32_t  nGreeting2;        // +0x04
    int32_t  nDayCycleBase;     // +0x08
    int32_t  nFarewell;         // +0x0C
    int32_t  nLastPlayedSound;  // +0x10
    uint32_t dwLastPlayedFrame; // +0x14
};
static_assert(sizeof(D2NpcSpeechRecordStrc) == 0x18, "D2NpcSpeechRecordStrc must be 0x18 bytes");

// 0x08-byte voice emote command to sound index mapping (0x6FB908A8)
struct D2VoiceEmoteMapStrc
{
    int32_t  nStrTblIndex;      // +0x00 (0x0DA0..0x0DAF)
    uint16_t wSoundCommandId;   // +0x04
    uint16_t wPadding;          // +0x06
};
static_assert(sizeof(D2VoiceEmoteMapStrc) == 0x08, "D2VoiceEmoteMapStrc must be 0x08 bytes");

// 0x120-byte (288-byte) ground item label entry in g_aGroundItemLabels[32] (0x6FBD0F00)
struct D2GroundItemLabelStrc
{
    int32_t     nLeft;          // +0x00
    int32_t     nTop;           // +0x04
    int32_t     nRight;         // +0x08
    int32_t     nBottom;        // +0x0C
    D2UnitStrc* pItem;          // +0x10
    Unicode     wszText[128];   // +0x14..+0x113 (256 bytes)
    int32_t     nBackPalette;   // +0x114
    int32_t     nTransLvl;      // +0x118
    int32_t     nTextColor;     // +0x11C
};
static_assert(sizeof(D2GroundItemLabelStrc) == 0x120, "D2GroundItemLabelStrc must be 0x120 bytes");

#pragma pack(pop)

// =============================================================================
// External Imports & D2Client Globals
// =============================================================================
extern "C" {
    // D2Win.dll!#10082
    void __fastcall D2Win_10132(const Unicode* pText, int32_t nLeft, int32_t nBottom, int32_t nBackColor, int32_t nTransLvl, int32_t nTextColor);
}

// D2Client internal helpers referenced by showitems.cpp
int32_t __fastcall SOUND_PickRandomVariant(int32_t nSoundId);                         // 0x6FB1A550
D2UnitStrc* __fastcall SELECT_GetSelectedUnit(void);                                  // 0x6FB1ECA0
void __fastcall SELECT_SetSelectedGroundItem(D2UnitStrc* pItem);                      // 0x6FB1F050
void __fastcall ITEM_BuildGroundLabelText(D2UnitStrc* pItem, Unicode* pOut, int32_t nMaxChars); // 0x6FB3C9D0

// Globals in D2Client.dll (.data / .bss)
extern void*       gpfnClientLoopHook;          // 0x6FBCC54C
extern void*       gpfnPrevClientLoopHook;      // 0x6FBB5364
extern int32_t     gbRecordPlaybackActive;      // 0x6FBCC090
extern int32_t     gbNpcSpeechEnabled;          // 0x6FB905EC (initialized to 1)
extern D2UnitStrc* gpClientPlayer;              // 0x6FBCC3D0
extern D2DrlgActStrc* gpClientAct;              // 0x6FBCC01C
extern uint32_t    gdwClientGameFrame;          // 0x6FBB32B0
extern int32_t     gnScreenWidth;               // 0x6FB8C6E0
extern int32_t     gnScreenHeight;              // 0x6FB8C6E4
extern int32_t     gnViewportShiftMode;         // 0x6FBCC1D0 (0=None, 1=LeftPanelOpen, 2=RightPanelOpen)
extern int32_t     gnViewportShiftOffsetX;      // 0x6FBCC1D4
extern int32_t     gnViewportAnchorX;           // 0x6FBC21D0
extern int32_t     gnViewportAnchorY;           // 0x6FBC21CC
extern int32_t     gnMouseScreenX;              // 0x6FBB1634
extern int32_t     gnMouseScreenY;              // 0x6FBB1638
extern int32_t     gnTooltipUnitType;           // 0x6FBCC118
extern int32_t     gnTooltipUnitGUID;           // 0x6FBCC11C

static int32_t               g_nGroundItemLabelCount    = 0;  // 0x6FBB3410
static D2GroundItemLabelStrc g_aGroundItemLabels[32]    = {}; // 0x6FBD0F00..0x6FBD32FF

// 0x6FB908A8: Voice emote lookup table
static const D2VoiceEmoteMapStrc g_aVoiceEmoteTable[] = {
    { 0x0DA0, 14, 0 },
    { 0x0DA1, 15, 0 },
    { 0x0DA2, 16, 0 },
    { 0x0DA3, 17, 0 },
    { 0x0DA4, 18, 0 },
    { 0x0DA5, 19, 0 },
    { 0x0DA6, 20, 0 },
    { 0x0DA7, 21, 0 },
    { 0x0DA8, 22, 0 },
    { 0x0DA9, 23, 0 },
    { 0x0DAA, 13, 0 },
    { 0x0DAB, 11, 0 },
    { 0x0DAC, 12, 0 },
    { 0x0DAD, 65, 0 },
    { 0x0DAE, 71, 0 },
    { 0x0DAF, 64, 0 },
    { 0x0000,  0, 0 },
};

// 0x6FB905F0..0x6FB908A7: Town NPC greeting / farewell audio table
static D2NpcSpeechRecordStrc g_aNpcSpeechRecords[29] = {
    { 3480, 3482, 3485, 3484, 0, 0 }, // [0]  0x6FB905F0: Akara (148)
    { 3523, 3525, 3528, 3527, 0, 0 }, // [1]  0x6FB90608: Alkor (254)
    { 3571, 3573, 3576, 3575, 0, 0 }, // [2]  0x6FB90620: Asheara (252)
    { 3613, 3615, 3618, 3617, 0, 0 }, // [3]  0x6FB90638: Atma (176)
    { 3644, 3647, 3649, 3644, 0, 0 }, // [4]  0x6FB90650: Cain (146, 244..246, 265, 520)
    { 3737, 3739, 3743, 3742, 0, 0 }, // [5]  0x6FB90668: Charsi (154)
    { 3776, 3779, 3782, 3781, 0, 0 }, // [6]  0x6FB90680: Drognan (177)
    { 3811, 3814, 3816, 3811, 0, 0 }, // [7]  0x6FB90698: Elzix (199)
    { 3844, 3847, 3850, 3849, 0, 0 }, // [8]  0x6FB906B0: Fara (178)
    { 3877, 3880, 3883, 3882, 0, 0 }, // [9]  0x6FB906C8: Geglash (200)
    { 3910, 3913, 3917, 3916, 0, 0 }, // [10] 0x6FB906E0: Gheed (147)
    { 3949, 3951, 3954, 3953, 0, 0 }, // [11] 0x6FB906F8: Greiz (198)
    { 3989, 3991,    0, 3989, 0, 0 }, // [12] 0x6FB90710: Halbu (257)
    { 3992, 3993, 3995, 3994, 0, 0 }, // [13] 0x6FB90728: Hratli (253)
    { 4043, 4045,    0, 4043, 0, 0 }, // [14] 0x6FB90740: Hadriel (405)
    { 4046, 4048,    0, 4046, 0, 0 }, // [15] 0x6FB90758: Izual
    { 4049, 4051, 4054, 4053, 0, 0 }, // [16] 0x6FB90770: Meshif Act 2 (201)
    { 4077, 4079, 4082, 4081, 0, 0 }, // [17] 0x6FB90788: Kashya (150)
    { 4118, 4120, 4123, 4122, 0, 0 }, // [18] 0x6FB907A0: Jerhyn (202)
    { 4150, 4152, 4155, 4154, 0, 0 }, // [19] 0x6FB907B8: Lysander (210)
    { 4217, 4219, 4221, 4220, 0, 0 }, // [20] 0x6FB907D0: Natalya (297)
    { 4248, 4250, 4252, 4251, 0, 0 }, // [21] 0x6FB907E8: Ormus (255)
    { 4303, 4305, 4306, 4303, 0, 0 }, // [22] 0x6FB90800: Tyrael (251, 521)
    {    0, 4320, 4324, 4323, 0, 0 }, // [23] 0x6FB90818: Warriv Act 1/2 (155, 175)
    {    0, 4382, 4386, 4385, 0, 0 }, // [24] 0x6FB90830: Drehya / Anya (512)
    {    0, 4563, 4567, 4566, 0, 0 }, // [25] 0x6FB90848: Qual-Kehk (515)
    {    0, 4489, 4493, 4492, 0, 0 }, // [26] 0x6FB90860: Malah (513)
    {    0, 4449, 4453, 4452, 0, 0 }, // [27] 0x6FB90878: Larzuk (511)
    {    0, 4533, 4537, 4536, 0, 0 }, // [28] 0x6FB90890: Nihlathak (514)
};

// =============================================================================
// SECTION 1: Preceding Unit Tail Functions (0x6FB26E00..0x6FB2717D)
// =============================================================================

extern void __stdcall RECORD_SaveAndInvokeExitCallback(int32_t a1, int32_t a2, int32_t a3);

// 1.12a: 0x6FB26E00 (__stdcall / void)
void __stdcall RECORD_InstallPlaybackHook(void)
{
    void* pPrevHook = gpfnClientLoopHook;
    gbRecordPlaybackActive = 1;
    gpfnClientLoopHook = reinterpret_cast<void*>(&RECORD_SaveAndInvokeExitCallback);
    gpfnPrevClientLoopHook = pPrevHook;
}

// 1.12a: 0x6FB26E20 (__usercall: eax = bEnabled)
void __fastcall NPCSPEECH_SetEnabled(int32_t bEnabled)
{
    gbNpcSpeechEnabled = bEnabled;
}

// 1.12a: 0x6FB26E30 (__stdcall)
int32_t __stdcall NPCSPEECH_IsEnabled(void)
{
    return gbNpcSpeechEnabled;
}

// 1.12a: 0x6FB26E40 (__fastcall: cx = wCommandId)
int32_t __fastcall NPCSPEECH_GetVoiceEmoteStringId(uint16_t wCommandId)
{
    if (!gbNpcSpeechEnabled)
    {
        return 0;
    }

    for (const D2VoiceEmoteMapStrc* pEntry = g_aVoiceEmoteTable; pEntry->nStrTblIndex != 0; ++pEntry)
    {
        if (pEntry->wSoundCommandId == wCommandId)
        {
            return pEntry->nStrTblIndex;
        }
    }
    return 0;
}

// 1.12a: 0x6FB26E70 (__usercall: eax = pNpcUnit)
D2NpcSpeechRecordStrc* __fastcall NPCSPEECH_GetRecordForUnit(D2UnitStrc* pNpcUnit)
{
    if (pNpcUnit == nullptr)
    {
        return nullptr;
    }

    const int32_t nClassId = pNpcUnit->dwClassId;
    switch (nClassId)
    {
    case 146: // MONSTER_CAIN1
    case 244: // MONSTER_CAIN2
    case 245: // MONSTER_CAIN3
    case 246: // MONSTER_CAIN4
    case 265: // MONSTER_CAIN5
    case 520: // MONSTER_CAIN6
        return &g_aNpcSpeechRecords[4];  // 0x6FB90650
    case 147: // MONSTER_GHEED
        return &g_aNpcSpeechRecords[10]; // 0x6FB906E0
    case 148: // MONSTER_AKARA
        return &g_aNpcSpeechRecords[0];  // 0x6FB905F0
    case 150: // MONSTER_KASHYA
        return &g_aNpcSpeechRecords[17]; // 0x6FB90788
    case 154: // MONSTER_CHARSI
        return &g_aNpcSpeechRecords[5];  // 0x6FB90668
    case 155: // MONSTER_WARRIV1
    case 175: // MONSTER_WARRIV2
        return &g_aNpcSpeechRecords[23]; // 0x6FB90818
    case 176: // MONSTER_ATMA
        return &g_aNpcSpeechRecords[3];  // 0x6FB90638
    case 177: // MONSTER_DROGNAN
        return &g_aNpcSpeechRecords[6];  // 0x6FB90680
    case 178: // MONSTER_FARA
        return &g_aNpcSpeechRecords[8];  // 0x6FB906B0
    case 198: // MONSTER_GREIZ
        return &g_aNpcSpeechRecords[11]; // 0x6FB906F8
    case 199: // MONSTER_ELZIX
        return &g_aNpcSpeechRecords[7];  // 0x6FB90698
    case 200: // MONSTER_GEGLASH
        return &g_aNpcSpeechRecords[9];  // 0x6FB906C8
    case 201: // MONSTER_MESHIF1
        return &g_aNpcSpeechRecords[16]; // 0x6FB90770
    case 202: // MONSTER_JERHYN
        return &g_aNpcSpeechRecords[18]; // 0x6FB907A0
    case 210: // MONSTER_LYSANDER
        return &g_aNpcSpeechRecords[19]; // 0x6FB907B8
    case 251: // MONSTER_TYRAEL1
    case 521: // MONSTER_TYRAEL2
        return &g_aNpcSpeechRecords[22]; // 0x6FB90800
    case 252: // MONSTER_ASHEARA
        return &g_aNpcSpeechRecords[2];  // 0x6FB90620
    case 253: // MONSTER_HRATLI
        return &g_aNpcSpeechRecords[13]; // 0x6FB90728
    case 254: // MONSTER_ALKOR
        return &g_aNpcSpeechRecords[1];  // 0x6FB90608
    case 255: // MONSTER_ORMUS
        return &g_aNpcSpeechRecords[21]; // 0x6FB907E8
    case 257: // MONSTER_HALBU
        return &g_aNpcSpeechRecords[12]; // 0x6FB90710
    case 297: // MONSTER_NATALYA
        return &g_aNpcSpeechRecords[20]; // 0x6FB907D0
    case 405: // MONSTER_HADRIEL
        return &g_aNpcSpeechRecords[14]; // 0x6FB90740
    case 511: // MONSTER_LARZUK
        return &g_aNpcSpeechRecords[27]; // 0x6FB90878
    case 512: // MONSTER_DREHYA
        return &g_aNpcSpeechRecords[24]; // 0x6FB90830
    case 513: // MONSTER_MALAH
        return &g_aNpcSpeechRecords[26]; // 0x6FB90860
    case 514: // MONSTER_NIHLATHAK
        return &g_aNpcSpeechRecords[28]; // 0x6FB90890
    case 515: // MONSTER_QUAL_KEHK
        return &g_aNpcSpeechRecords[25]; // 0x6FB90848
    default:
        return nullptr;
    }
}

// 1.12a: 0x6FB27080 (__userpurge: eax = pNpcUnit, [esp+4] = nSpeechMode)
// nSpeechMode: 0 = standard greeting, 1 = return greeting, 2 = farewell
int32_t __fastcall NPCSPEECH_PickGreetingSound(D2UnitStrc* pNpcUnit, int32_t nSpeechMode)
{
    D2NpcSpeechRecordStrc* pRec = NPCSPEECH_GetRecordForUnit(pNpcUnit);
    if (pRec == nullptr)
    {
        return 0;
    }

    if (nSpeechMode == 2)
    {
        return pRec->nFarewell;
    }

    D2UnitStrc* pPlayer = gpClientPlayer;
    int32_t nAttempt = 0;
    int32_t nPickedSound = 0;

    do
    {
        int32_t nBaseSoundId = pRec->nGreeting1;
        if (nSpeechMode == 0 && pRec->nGreeting2 != 0)
        {
            // Advance 64-bit LCG seed on pPlayer->dwSeed (+0x20)
            uint64_t uNext = static_cast<uint64_t>(pPlayer->dwInitSeed) * 0x6AC690C5ULL + pPlayer->dwEndSeed;
            pPlayer->dwInitSeed = static_cast<uint32_t>(uNext);
            pPlayer->dwEndSeed = static_cast<uint32_t>(uNext >> 32);
            if ((static_cast<uint32_t>(uNext) & 1) == 0)
            {
                nBaseSoundId = pRec->nGreeting2;
            }
        }

        if (pRec->nDayCycleBase != 0)
        {
            bool bUseCycleGreeting = (nBaseSoundId == 0);
            if (!bUseCycleGreeting)
            {
                uint64_t uNext = static_cast<uint64_t>(pPlayer->dwInitSeed) * 0x6AC690C5ULL + pPlayer->dwEndSeed;
                pPlayer->dwInitSeed = static_cast<uint32_t>(uNext);
                pPlayer->dwEndSeed = static_cast<uint32_t>(uNext >> 32);
                bUseCycleGreeting = ((static_cast<uint32_t>(uNext) % 3u) == 0);
            }

            if (bUseCycleGreeting)
            {
                // D2Common.#11140: ENVIRONMENT_GetCycleIndexFromAct
                const int32_t nCycle = ENVIRONMENT_GetCycleIndexFromAct(gpClientAct);
                if (nCycle == 1)
                {
                    nBaseSoundId = pRec->nDayCycleBase;
                }
                else if (nCycle > 1 && nCycle <= 3)
                {
                    nBaseSoundId = pRec->nDayCycleBase + 1;
                }
                else
                {
                    nBaseSoundId = pRec->nDayCycleBase + 2;
                }
                pPlayer = gpClientPlayer;
            }
        }

        nPickedSound = SOUND_PickRandomVariant(nBaseSoundId);
        if (nPickedSound != pRec->nLastPlayedSound)
        {
            break;
        }
        ++nAttempt;
    } while (nAttempt < 20);

    pRec->nLastPlayedSound = nPickedSound;
    pRec->dwLastPlayedFrame = gdwClientGameFrame;
    return nPickedSound;
}

// =============================================================================
// SECTION 2: Ground Item Label Layout & Rendering (UI/showitems.cpp)
// =============================================================================

// 1.12a: 0x6FB27180
// Calling convention: __userpurge (ecx = nMinX, [esp+4] = pLeft, [esp+8] = pTop, [esp+12] = pRight,
//                                  [esp+16] = pBottom, [esp+20] = nPass, [esp+24] = nMaxX)
// Clamps the proposed ground-item label rectangle to the active viewport and resolves AABB
// collisions against previously placed labels in g_aGroundItemLabels[0..g_nGroundItemLabelCount-1].
// Returns 1 if the rectangle is free of overlaps, or 0 if it was shifted and needs another pass.
int32_t __fastcall SHOWITEMS_ResolveLabelOverlap(
    int32_t  nMinX,
    int32_t* pLeft,
    int32_t* pTop,
    int32_t* pRight,
    int32_t* pBottom,
    int32_t  nPass,
    int32_t  nMaxX)
{
    const int32_t nMaxY = gnScreenHeight - 48;

    // Clamp left edge to nMinX
    if (*pLeft < nMinX)
    {
        *pRight += (nMinX - *pLeft);
        *pLeft = 6;
    }

    // Clamp top edge to 6
    if (*pTop < 6)
    {
        *pBottom += (6 - *pTop);
        *pTop = 6;
    }

    // Clamp right edge to nMaxX
    if (*pRight >= nMaxX)
    {
        const int32_t nExcessX = (*pRight - nMaxX) + 1;
        *pRight -= nExcessX;
        *pLeft -= nExcessX;
    }

    // Clamp bottom edge to nMaxY (gnScreenHeight - 48)
    if (*pBottom >= nMaxY)
    {
        const int32_t nExcessY = *pBottom - nMaxY;
        *pBottom -= nExcessY;
        *pTop -= nExcessY;
    }

    if (g_nGroundItemLabelCount >= 32)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\showitems.cpp", 71);
        exit(-1);
    }

    const int32_t nCurLeft = *pLeft;
    const int32_t nCurTop  = *pTop;

    for (int32_t i = 0; i < g_nGroundItemLabelCount; ++i)
    {
        const D2GroundItemLabelStrc& rOther = g_aGroundItemLabels[i];

        const bool bOverlapX = (nCurLeft <= rOther.nRight) && (*pRight >= rOther.nLeft);
        const bool bOverlapY = (nCurTop <= rOther.nBottom) && (*pBottom >= rOther.nTop);

        if (bOverlapX && bOverlapY)
        {
            if (nPass > 6)
            {
                // Passes 7..11: shift horizontally left or right of the colliding box
                const int32_t nHalfW = (*pRight - nCurLeft) / 2;
                int32_t nShiftX = 0;
                if (nHalfW != 0 && nCurLeft >= nMaxX)
                {
                    nShiftX = (rOther.nLeft - *pRight) - 1;
                }
                else
                {
                    nShiftX = (rOther.nRight - nCurLeft) + 1;
                }
                *pLeft = nCurLeft + nShiftX;
                *pRight += nShiftX;
                return 0;
            }
            else
            {
                // Passes 0..6: shift vertically above or below the colliding box
                const int32_t nHalfH = (*pBottom - nCurTop) / 2;
                int32_t nShiftY = 0;
                if (nHalfH != 0 && nCurTop <= nMaxY)
                {
                    nShiftY = (rOther.nTop - *pBottom) - 3;
                }
                else
                {
                    nShiftY = (rOther.nBottom - nCurTop) + 5;
                }
                *pTop = nCurTop + nShiftY;
                *pBottom += nShiftY;
                return 0;
            }
        }
    }

    return 1;
}

// 1.12a: 0x6FB27350 (__stdcall / void)
// Main entry point for `ALT` ground item label layout, hit-testing, and rendering.
void __stdcall SHOWITEMS_DrawGroundItemLabels(void)
{
    const BOOL bPerspective = D2GFX_CheckPerspective();
    if (gpClientPlayer == nullptr)
    {
        return;
    }

    D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(gpClientPlayer);
    if (pPlayerRoom == nullptr)
    {
        return;
    }

    D2UnitStrc* pPrevSelectedUnit = SELECT_GetSelectedUnit();
    const int32_t nMouseX = gnMouseScreenX;
    const int32_t nMouseY = gnMouseScreenY;

    D2ActiveRoomStrc** ppAdjacentRooms = nullptr;
    int32_t nNumRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pPlayerRoom, &ppAdjacentRooms, &nNumRooms);

    const uint8_t nHighlightBackColor = D2Win_10034_MixRGB(0x00, 0x40, 0x80);

    g_nGroundItemLabelCount = 0;
    int32_t bHoveredAnyLabel = 0;

    for (int32_t nRoomIdx = 0; nRoomIdx < nNumRooms; ++nRoomIdx)
    {
        D2ActiveRoomStrc* pRoom = ppAdjacentRooms[nRoomIdx];
        if (pRoom == nullptr)
        {
            continue;
        }

        for (D2UnitStrc* pUnit = pRoom->pUnitFirst; pUnit != nullptr; pUnit = pUnit->pRoomNext)
        {
            if ((pUnit->dwFlags & UNITFLAG_SELECTABLE) == 0)
            {
                continue;
            }
            if (pUnit->dwUnitType != UNIT_ITEM)
            {
                continue;
            }
            if (g_nGroundItemLabelCount >= 32)
            {
                continue;
            }

            D2GroundItemLabelStrc* pLabel = &g_aGroundItemLabels[g_nGroundItemLabelCount];
            pLabel->pItem = pUnit;

            int32_t nScreenX = 0;
            int32_t nScreenY = 0;

            if (bPerspective)
            {
                const int32_t nPrecX = UNITS_GetPrecisionX(pUnit);
                const int32_t nCoordX = UNITS_GetClientCoordX(pUnit);
                if (!SCALE_CheckPerspectivePosition(nPrecX, nCoordX))
                {
                    continue;
                }
                SCALE_AdjustPerspectivePosition(nPrecX, nCoordX, 0, &nScreenX, &nScreenY);

                if (gnViewportShiftMode == 1)
                {
                    nScreenX -= gnScreenWidth / 4;
                }
                else if (gnViewportShiftMode == 2)
                {
                    nScreenX += gnScreenWidth / 4;
                }
            }
            else
            {
                const int32_t nViewBaseX = gnViewportAnchorX - gnViewportShiftOffsetX;
                const int32_t nPrecY = UNITS_GetPrecisionY(pUnit);
                const int32_t nViewBaseY = gnViewportAnchorY - 8;
                nScreenX = nPrecY - nViewBaseX;
                nScreenY = UNITS_GetClientCoordY(pUnit) + (-16 - nViewBaseY);
            }

            // Compute horizontal viewport bounds based on open left/right panels
            int32_t nMinX = 0;
            int32_t nMaxX = gnScreenWidth;
            if (gnViewportShiftMode == 1)
            {
                nMaxX = (gnScreenWidth / 2) - 32;
            }
            else if (gnViewportShiftMode == 2)
            {
                nMinX = gnScreenWidth / 2;
            }

            if (nScreenX < nMinX || nScreenX > nMaxX || nScreenY < -8 || nScreenY > (gnScreenHeight - 16))
            {
                continue;
            }

            // Format localized item label string
            pLabel->wszText[0] = 0;
            ITEM_BuildGroundLabelText(pUnit, pLabel->wszText, 256);

            int32_t nTextWidth = 0;
            int32_t nTextHeight = 0;
            D2Win_10131_GetTextDimensions(pLabel->wszText, &nTextWidth, &nTextHeight);

            const int32_t nBoxWidth = nTextWidth + 8;
            int32_t nBoxLeft   = nScreenX - (nBoxWidth >> 1);
            int32_t nBoxRight  = nBoxLeft + nBoxWidth;
            int32_t nBoxBottom = nScreenY;
            int32_t nBoxTop    = (nScreenY - nTextHeight) + 4;

            // Up to 12 passes of AABB collision avoidance
            int32_t bPlaced = 0;
            for (int32_t nPass = 0; nPass < 12; ++nPass)
            {
                if (SHOWITEMS_ResolveLabelOverlap(
                        nMinX,
                        &nBoxLeft,
                        &nBoxTop,
                        &nBoxRight,
                        &nBoxBottom,
                        nPass,
                        nMaxX))
                {
                    bPlaced = 1;
                    break;
                }
            }

            if (!bPlaced || nBoxRight > nMaxX || nBoxLeft < nMinX)
            {
                continue;
            }

            pLabel->nLeft   = nBoxLeft;
            pLabel->nTop    = nBoxTop;
            pLabel->nRight  = nBoxRight;
            pLabel->nBottom = nBoxBottom;

            if (!bHoveredAnyLabel)
            {
                if (nMouseX >= nBoxLeft && nMouseX <= nBoxRight &&
                    nMouseY >= nBoxTop  && nMouseY <= (nBoxBottom + 4))
                {
                    pLabel->nTextColor   = 0;
                    pLabel->nBackPalette = nHighlightBackColor;
                    pLabel->nTransLvl    = 5;
                    SELECT_SetSelectedGroundItem(pUnit);
                    bHoveredAnyLabel = 1;
                    ++g_nGroundItemLabelCount;
                    continue;
                }

                if (pUnit == pPrevSelectedUnit)
                {
                    pLabel->nBackPalette = nHighlightBackColor;
                    pLabel->nTransLvl    = 5;
                    pLabel->nTextColor   = 0;
                    D2Win_10130_DrawFramedTextEx(nullptr, 0, 0, 0, 0, 0);
                    gnTooltipUnitGUID = 0;
                    gnTooltipUnitType = 0;
                    bHoveredAnyLabel = 1;
                    ++g_nGroundItemLabelCount;
                    continue;
                }
            }

            // Default (non-hovered) ground item label styling
            pLabel->nBackPalette = 0;
            pLabel->nTransLvl    = 1;

            int32_t nTextColor = 0; // White
            switch (ITEMS_GetItemQuality(pUnit))
            {
            case ITEMQUAL_MAGIC:    // 4
                nTextColor = 3;     // Blue
                break;
            case ITEMQUAL_SET:      // 5
                nTextColor = 2;     // Green
                break;
            case ITEMQUAL_RARE:     // 6
                nTextColor = 9;     // Yellow
                break;
            case ITEMQUAL_UNIQUE:   // 7
                nTextColor = 4;     // Gold
                break;
            case ITEMQUAL_CRAFT:    // 8
                nTextColor = 8;     // Orange
                break;
            case ITEMQUAL_TEMPERED: // 9
                nTextColor = 10;    // Dark Green
                break;
            default:
                if ((ITEMS_CheckItemFlag(pUnit, ITEMFLAG_SOCKETED, 262, "..\\Source\\D2Client\\UI\\showitems.cpp") &&
                     ITEMS_IsQuestItem(pUnit) &&
                     ITEMS_GetMaxSockets(pUnit) > 0) ||
                    ITEMS_CheckItemFlag(pUnit, ITEMFLAG_ETHEREAL, 268, "..\\Source\\D2Client\\UI\\showitems.cpp"))
                {
                    nTextColor = 5; // Grey
                }
                break;
            }

            D2ItemsTxt* pItemsTxt = DATATBLS_GetItemsTxtRecord(pUnit->dwClassId);
            if (pItemsTxt != nullptr && pItemsTxt->nQuest != 0 && pItemsTxt->dwCode != ' gel')
            {
                nTextColor = 4; // Quest items (except Wirt's Leg) use Gold
            }

            pLabel->nTextColor = nTextColor;
            ++g_nGroundItemLabelCount;
        }
    }

    // Draw all placed ground item labels
    for (int32_t i = 0; i < g_nGroundItemLabelCount; ++i)
    {
        const D2GroundItemLabelStrc& rLabel = g_aGroundItemLabels[i];
        D2Win_10132(
            rLabel.wszText,
            rLabel.nLeft,
            rLabel.nBottom,
            rLabel.nBackPalette,
            rLabel.nTransLvl,
            rLabel.nTextColor);
    }

    if (!bHoveredAnyLabel)
    {
        gnTooltipUnitGUID = 0;
        gnTooltipUnitType = 0;
        D2Win_10130_DrawFramedTextEx(nullptr, 0, 0, 0, 0, 0);
    }
}

// 1.12a: 0x6FB27870 (__stdcall)
float __stdcall SHOWITEMS_HypotSq(float fDx, float fDy)
{
    return (fDx * fDx) + (fDy * fDy);
}
