// ============================================================================
// UNIT_Monster.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll
// Address ranges:
//   - asm/UNIT_Monster.asm   (0x6FAE0EB0 .. 0x6FAE6360)
//   - asm/UNIT_MonUnique.asm (0x6FB68230 .. 0x6FB6A060)
// Original source paths:
//   - ..\Source\D2Client\UNIT\Monster.cpp
//   - ..\Source\D2Client\UNIT\MonUnique.cpp
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <math.h>
#include "D2CommonDefinitions.h"
#include "D2Structs.h"
#include "D2DataTbls.h"
#include "Units/Units.h"
#include "Units/Monster.h"
#include "D2StatList.h"
#include "D2Skills.h"
#include "D2Inventory.h"
#include "D2Collision.h"
#include "D2Dungeon.h"
#include "Path/Path.h"
#include "D2Seed.h"
#include "D2States.h"
#include "Fog.h"
#include "D2Gfx.h"
#include "Window.h"
#include "D2Sound.h"
#include "D2Lang.h"
#include "D2CMP.h"

// ============================================================================
// Structures
// ============================================================================

#pragma pack(push, 1)
struct D2SoundUnitNodeStrc
{
    D2SoundUnitNodeStrc* pNext;      // +0x00
    D2UnitStrc* pUnit;               // +0x04
};

struct D2ClientSoundChannelStrc
{
    D2ClientSoundChannelStrc* pNext; // +0x00
    int32_t nSoundId;                // +0x04
    uint32_t dwHandle;               // +0x08
    int32_t nGroup;                  // +0x0C
    float fRelX;                     // +0x10
    float fRelY;                     // +0x14
    float fDistSq;                   // +0x18
    int32_t nVolume;                 // +0x1C
    int32_t nWorldX;                 // +0x20
    int32_t nWorldY;                 // +0x24
    D2SoundUnitNodeStrc* pUnitList;  // +0x28
    void* pSmpStream;                // +0x2C
    int32_t nChannelType;            // +0x30 (0 = 2D, 1 = 3D, 2 = Music, 3 = Speech, 4 = Video)
    uint32_t dwStartTick;            // +0x34
    uint32_t dwDelayEndTick;         // +0x38
    int32_t nPriority;               // +0x3C
    uint8_t bPausedForSpeech;        // +0x40
    uint8_t bLoop;                   // +0x41
    uint8_t bHasStarted;             // +0x42
    uint8_t bAttenuatedForSpeech;    // +0x43
    uint8_t bMusicDucked;            // +0x44
    uint8_t bFadeActive;             // +0x45
    uint8_t bStopAfterFade;          // +0x46
    uint8_t bFadeComplete;           // +0x47
    uint8_t bHasWorldCoords;         // +0x48
    int32_t nFadeStartVol;           // +0x49
    int32_t nFadeEndVol;             // +0x4D
    uint32_t dwFadeStartTick;        // +0x51
    uint32_t dwFadeEndTick;          // +0x55
}; // sizeof = 0x59 (89 bytes)
#pragma pack(pop)

struct D2MonsterModeCallbackStrc
{
    void (__fastcall* pfStartMode)(D2UnitStrc* pUnit, int32_t nTargetX, int32_t nTargetY); // +0x00
    void (__fastcall* pfStepMode)(D2UnitStrc* pUnit);                                      // +0x04
    void (__fastcall* pfEndMode)(D2UnitStrc* pUnit);                                       // +0x08
};

struct D2MonUModHandlerStrc
{
    void (__fastcall* pfInitStat)(D2UnitStrc* pUnit, uint32_t dwUModId, int32_t bUnique);  // +0x00
    void (__fastcall* pfUpdate)(D2UnitStrc* pUnit);                                        // +0x04
    void (__fastcall* pfOnHit)(D2UnitStrc* pUnit, int32_t nSubType, D2UnitStrc* pAttacker);// +0x08
    void (__fastcall* pfOnDeath)(D2UnitStrc* pUnit);                                       // +0x0C
};

// ============================================================================
// External Globals (1.12a addresses)
// ============================================================================

extern D2UnitStrc* gpPlayerUnit_6FBCC3D0;               // 0x6FBCC3D0
extern D2UnitStrc* gpQuestMonster_6FBC57D0;             // 0x6FBC57D0
extern D2ClientSoundChannelStrc* gpActiveSoundList_6FBCC36C; // 0x6FBCC36C
extern uint32_t gdwSoundDuckStartTick_6FBCC370;         // 0x6FBCC370
extern uint32_t gdwNextSoundHandle_6FBCC374;            // 0x6FBCC374
extern D2ClientSoundChannelStrc gSoundChannelPool_6FBC57D8[200]; // 0x6FBC57D8..0x6FBC9D60
extern int32_t gbSoundDebugOverlay_6FBCC2D0;            // 0x6FBCC2D0
extern int32_t gnSoundMaxVoices_6FBCC1C8;               // 0x6FBCC1C8
extern int32_t gbSoundChaosMode_6FBCC214;               // 0x6FBCC214
extern D2SeedStrc gClientMonsterSeed_6FBCC300;          // 0x6FBCC300
extern uint8_t gDigitBitmap7x10_6FBA6351[];             // 0x6FBA6351
extern D2MonsterModeCallbackStrc gMonsterModeCallbacks_6FBA61C8[]; // 0x6FBA61C8
extern D2MonsterModeCallbackStrc gSpecialMonsterModeCallbacks_6FBA6288[]; // 0x6FBA6288
extern D2MonUModHandlerStrc gMonUModHandlers_6FBAA220[]; // 0x6FBAA220

// ============================================================================
// Forward Declarations
// ============================================================================

void __fastcall D2Client_ApplyMonsterUniqueModifiers(D2UnitStrc* pUnit);
D2UnitStrc* __fastcall D2Client_FindServerSideUnit(int32_t nUnitId, int32_t nUnitType);
D2UnitStrc* __fastcall D2Client_FindClientSideUnit(int32_t nUnitId, int32_t nUnitType);
int32_t __fastcall D2Client_PlaySound(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nTicksDelay, BOOL bLoop, uint32_t dwFlags);

// ============================================================================
// Part 1: Client Sound Channel / Stream Manager (0x6FAE0EB0 .. 0x6FAE2B0F)
// ============================================================================

// 1.12a: 0x6FAE0EB0
// Custom calling convention: EAX = nDigit, EDX = nX, ECX = nY
void __fastcall D2Client_DrawDebugDigit7x10(int32_t nDigit, int32_t nX, int32_t nY, uint8_t nColor)
{
    const uint8_t* pGlyph = &gDigitBitmap7x10_6FBA6351[nDigit * 70];
    for (int32_t row = 0; row < 10; ++row)
    {
        for (int32_t col = 0; col < 7; ++col)
        {
            if (pGlyph[row * 7 + col] == 1)
            {
                D2GFX_DrawBox(nX + col, nY + row, nX + col + 1, nY + row + 1, nColor, DRAWMODE_NORMAL);
            }
        }
    }
}

// 1.12a: 0x6FAE0EE0
// Custom calling convention: EAX = nValue, ECX = nY, stack = nX, nColor
void __fastcall D2Client_DrawDebugUnsignedInt(uint32_t nValue, int32_t nY, int32_t nX, uint8_t nColor)
{
    int32_t nCurX = nX + 24;
    for (int32_t i = 0; i < 4; ++i)
    {
        D2Client_DrawDebugDigit7x10(nValue % 10, nCurX, nY, nColor);
        nValue /= 10;
        nCurX -= 8;
    }
}

// 1.12a: 0x6FAE0F30
void __cdecl D2Client_DrawSoundDebugOverlay(void)
{
    D2ClientSoundChannelStrc* pChan = gpActiveSoundList_6FBCC36C;
    int32_t nIndex = 0;
    int32_t nY = 55;
    while (pChan)
    {
        if (pChan->dwHandle != 0 && pChan->pSmpStream != nullptr)
        {
            uint8_t nColor = (pChan->nChannelType != 1) ? 0xFF : 0x84;
            D2Client_DrawDebugUnsignedInt(nIndex, nY, 80, nColor);
            D2Client_DrawDebugUnsignedInt(pChan->nSoundId, nY, 120, nColor);
            D2Client_DrawDebugUnsignedInt(pChan->nVolume, nY, 160, nColor);
            int32_t nDist = (int32_t)sqrt(pChan->fDistSq);
            D2Client_DrawDebugUnsignedInt(nDist, nY, 200, nColor);
            D2Client_DrawDebugUnsignedInt(pChan->nPriority, nY, 240, nColor);
            D2Client_DrawDebugUnsignedInt(GetTickCount() - pChan->dwStartTick, nY, 280, nColor);
            nY += 12;
        }
        pChan = pChan->pNext;
        ++nIndex;
    }
}

// 1.12a: 0x6FAE0FF0
int32_t __cdecl D2Client_CompareSoundChannels(D2ClientSoundChannelStrc* pChanA, D2ClientSoundChannelStrc* pChanB)
{
    if (pChanA->nPriority < pChanB->nPriority)
        return -1;
    if (pChanA->nPriority > pChanB->nPriority)
        return 1;
    if (pChanA->fDistSq < pChanB->fDistSq)
        return -1;
    if (pChanA->fDistSq > pChanB->fDistSq)
        return 1;
    if (pChanA->dwStartTick >= pChanB->dwStartTick)
        return (pChanA->dwStartTick > pChanB->dwStartTick) ? 1 : 0;
    return -1;
}

// 1.12a: 0x6FAE1050
BOOL __cdecl D2Client_IsAnyAmbientSoundPlaying(void)
{
    for (D2ClientSoundChannelStrc* pChan = gpActiveSoundList_6FBCC36C; pChan; pChan = pChan->pNext)
    {
        if (pChan->dwHandle != 0 && pChan->nSoundId >= 0x1231 && pChan->nSoundId <= 0x125A && pChan->pSmpStream != nullptr)
            return TRUE;
    }
    return FALSE;
}

// 1.12a: 0x6FAE1090
BOOL __cdecl D2Client_IsAnySpeechSoundPlaying(void)
{
    for (D2ClientSoundChannelStrc* pChan = gpActiveSoundList_6FBCC36C; pChan; pChan = pChan->pNext)
    {
        if (pChan->dwHandle != 0 && pChan->nSoundId >= 0x0B76 && pChan->nSoundId <= 0x1230 && pChan->pSmpStream != nullptr)
            return TRUE;
    }
    return FALSE;
}

// 1.12a: 0x6FAE10D0
// Custom calling convention: EAX = pChan
BOOL __fastcall D2Client_IsSoundChannelFadingToZero(D2ClientSoundChannelStrc* pChan)
{
    return (pChan && pChan->bFadeActive && pChan->nFadeEndVol == 0);
}

// 1.12a: 0x6FAE10F0
// Custom calling convention: EAX = pChan
BOOL __fastcall D2Client_IsSoundChannelFadeComplete(D2ClientSoundChannelStrc* pChan)
{
    return pChan->bFadeComplete != 0;
}

// 1.12a: 0x6FAE1100
// Custom calling convention: EAX = pChan, ECX = dwDurationMs, stack = nTargetVol, bStopAfterFade
void __fastcall D2Client_SetupSoundChannelFade(D2ClientSoundChannelStrc* pChan, uint32_t dwDurationMs, int32_t nTargetVol, uint8_t bStopAfterFade)
{
    pChan->nFadeStartVol = pChan->nVolume;
    pChan->nFadeEndVol = nTargetVol;
    pChan->bStopAfterFade = bStopAfterFade;
    pChan->bFadeActive = 1;
    pChan->bFadeComplete = 0;
    uint32_t dwNow = GetTickCount();
    pChan->dwFadeStartTick = dwNow;
    pChan->dwFadeEndTick = dwNow + dwDurationMs;
}

// 1.12a: 0x6FAE1130
// Custom calling convention: EAX = pUnit, ECX = pfOutX, EDX = pfOutY, stack = pfOutZ
void __fastcall D2Client_ComputeSound3DOffsetFromPlayer(D2UnitStrc* pUnit, float* pfOutX, float* pfOutY, float* pfOutZ)
{
    *pfOutX = 0.0f;
    *pfOutY = 0.0f;
    *pfOutZ = 0.0f;

    if (!pUnit || !gpPlayerUnit_6FBCC3D0)
        return;

    if (pUnit->dwUnitType == gpPlayerUnit_6FBCC3D0->dwUnitType && pUnit->dwUnitId == gpPlayerUnit_6FBCC3D0->dwUnitId)
        return;

    int32_t nUnitX = UNITS_GetXPosition(pUnit);
    int32_t nUnitY = UNITS_GetYPosition(pUnit);
    int32_t nPlayerX = UNITS_GetXPosition(gpPlayerUnit_6FBCC3D0);
    int32_t nPlayerY = UNITS_GetYPosition(gpPlayerUnit_6FBCC3D0);

    float fDX = (float)(nUnitX - nPlayerX);
    float fDY = (float)(nUnitY - nPlayerY);

    *pfOutX = (fDX - fDY) * 0.015f;
    *pfOutY = (fDX + fDY) * -0.015f;
    *pfOutZ = 0.0f;
}

// 1.12a: 0x6FAE11E0
// Custom calling convention: EAX = pUnit
D2ClientSoundChannelStrc* __fastcall D2Client_FindActiveSpeechChannelForUnit(D2UnitStrc* pUnit)
{
    for (D2ClientSoundChannelStrc* pChan = gpActiveSoundList_6FBCC36C; pChan; pChan = pChan->pNext)
    {
        if (pChan->dwHandle != 0 && pChan->nChannelType == 3 && pChan->pSmpStream != nullptr)
        {
            for (D2SoundUnitNodeStrc* pNode = pChan->pUnitList; pNode; pNode = pNode->pNext)
            {
                if (pNode->pUnit == pUnit)
                    return pChan;
            }
        }
    }
    return nullptr;
}

// 1.12a: 0x6FAE1240
// Custom calling convention: EAX = nSoundId
uint32_t __fastcall D2Client_FindSoundChannelHandleBySoundId(int32_t nSoundId)
{
    for (D2ClientSoundChannelStrc* pChan = gpActiveSoundList_6FBCC36C; pChan; pChan = pChan->pNext)
    {
        if (pChan->dwHandle != 0 && pChan->nSoundId == nSoundId)
            return pChan->dwHandle;
    }
    return 0;
}

// 1.12a: 0x6FAE1270
// Custom calling convention: EAX = dwHandle
D2ClientSoundChannelStrc* __fastcall D2Client_FindSoundChannelByHandle(uint32_t dwHandle)
{
    if (!dwHandle)
        return nullptr;

    for (D2ClientSoundChannelStrc* pChan = gpActiveSoundList_6FBCC36C; pChan; pChan = pChan->pNext)
    {
        if (pChan->dwHandle == dwHandle)
            return pChan;
    }
    return nullptr;
}

// 1.12a: 0x6FAE12A0
D2ClientSoundChannelStrc* __cdecl D2Client_AllocSoundChannel(void)
{
    for (int32_t i = 0; i < 200; ++i)
    {
        D2ClientSoundChannelStrc* pChan = &gSoundChannelPool_6FBC57D8[i];
        if (pChan->dwHandle == 0)
        {
            memset(pChan, 0, sizeof(D2ClientSoundChannelStrc));
            pChan->dwHandle = gdwNextSoundHandle_6FBCC374++;
            if (gdwNextSoundHandle_6FBCC374 == 0)
                gdwNextSoundHandle_6FBCC374 = 1;
            D2SOUND_ListPrepend(&gpActiveSoundList_6FBCC36C, pChan);
            return pChan;
        }
    }
    return nullptr;
}

// 1.12a: 0x6FAE12F0
BOOL __cdecl D2Client_HasActiveSpeechChannel(void)
{
    return D2Client_FindActiveSpeechChannelForUnit(nullptr) != nullptr;
}

// 1.12a: 0x6FAE1300
// Custom calling convention: EAX = pUnit
BOOL __fastcall D2Client_IsUnitWithinHearingDistance(D2UnitStrc* pUnit)
{
    float fX = 0.0f, fY = 0.0f, fZ = 0.0f;
    D2Client_ComputeSound3DOffsetFromPlayer(pUnit, &fX, &fY, &fZ);
    float fDistSq = fX * fX + fY * fY + fZ * fZ;
    return fDistSq <= 1.0f;
}

// 1.12a: 0x6FAE1380
// Custom calling convention: EAX = dwHandle
int32_t __fastcall D2Client_GetSoundChannelStreamPos(uint32_t dwHandle)
{
    D2ClientSoundChannelStrc* pChan = D2Client_FindSoundChannelByHandle(dwHandle);
    if (!pChan || !pChan->pSmpStream)
        return 0;
    return D2SOUND_SmpGetStreamPos(pChan->pSmpStream);
}

// 1.12a: 0x6FAE13D0
// Custom calling convention: EAX = dwHandle, ECX = nPriority
void __fastcall D2Client_SetSoundChannelPriority(uint32_t dwHandle, int32_t nPriority)
{
    D2ClientSoundChannelStrc* pChan = D2Client_FindSoundChannelByHandle(dwHandle);
    if (pChan)
        pChan->nPriority = nPriority;
}

// 1.12a: 0x6FAE1400
// Custom calling convention: EAX = dwHandle
int32_t __fastcall D2Client_GetSoundChannelSoundId(uint32_t dwHandle)
{
    D2ClientSoundChannelStrc* pChan = D2Client_FindSoundChannelByHandle(dwHandle);
    return pChan ? pChan->nSoundId : 0;
}

// 1.12a: 0x6FAE1430
// Custom calling convention: EAX = dwHandle
BOOL __fastcall D2Client_HasMultipleUnitsAttachedToSoundChannel(uint32_t dwHandle)
{
    D2ClientSoundChannelStrc* pChan = D2Client_FindSoundChannelByHandle(dwHandle);
    if (pChan && pChan->pUnitList && pChan->pUnitList->pNext)
        return TRUE;
    return FALSE;
}

// 1.12a: 0x6FAE1470
// Custom calling convention: EAX = nVolume, ECX = dwHandle
void __fastcall D2Client_SetSoundChannelVolume(int32_t nVolume, uint32_t dwHandle)
{
    D2ClientSoundChannelStrc* pChan = D2Client_FindSoundChannelByHandle(dwHandle);
    if (pChan)
        pChan->nVolume = nVolume;
}

// 1.12a: 0x6FAE14A0
// Custom calling convention: EAX = dwHandle
int32_t __fastcall D2Client_GetSoundChannelVolume(uint32_t dwHandle)
{
    D2ClientSoundChannelStrc* pChan = D2Client_FindSoundChannelByHandle(dwHandle);
    return pChan ? pChan->nVolume : 0;
}

// 1.12a: 0x6FAE14D0
// Custom calling convention: EAX = nGroup
D2ClientSoundChannelStrc* __fastcall D2Client_FindOldestSoundChannelInGroup(int32_t nGroup)
{
    uint32_t dwOldestTick = 0xFFFFFFFF;
    D2ClientSoundChannelStrc* pOldest = nullptr;
    for (D2ClientSoundChannelStrc* pChan = gpActiveSoundList_6FBCC36C; pChan; pChan = pChan->pNext)
    {
        if (pChan->dwHandle != 0 && pChan->nGroup == nGroup && pChan->dwStartTick < dwOldestTick)
        {
            if (!pChan->bFadeActive || pChan->nFadeEndVol != 0)
            {
                dwOldestTick = pChan->dwStartTick;
                pOldest = pChan;
            }
        }
    }
    return pOldest;
}

// 1.12a: 0x6FAE1560
// Custom calling convention: EAX = nGroup
int32_t __fastcall D2Client_CountActiveSoundChannelsInGroup(int32_t nGroup)
{
    int32_t nCount = 0;
    for (D2ClientSoundChannelStrc* pChan = gpActiveSoundList_6FBCC36C; pChan; pChan = pChan->pNext)
    {
        if (pChan->dwHandle != 0 && pChan->nGroup == nGroup)
        {
            if (!pChan->bFadeActive || pChan->nFadeEndVol != 0)
                ++nCount;
        }
    }
    return nCount;
}

// 1.12a: 0x6FAE15C0
// Custom calling convention: EAX = dwHandle
BOOL __fastcall D2Client_IsSoundChannelPlaying(uint32_t dwHandle)
{
    D2ClientSoundChannelStrc* pChan = D2Client_FindSoundChannelByHandle(dwHandle);
    return (pChan && pChan->pSmpStream != nullptr);
}

// 1.12a: 0x6FAE15F0
// Custom calling convention: EAX = dwHandle
BOOL __fastcall D2Client_DoesSoundChannelExist(uint32_t dwHandle)
{
    return D2Client_FindSoundChannelByHandle(dwHandle) != nullptr;
}

// 1.12a: 0x6FAE1620
// Custom calling convention: EAX = nTargetVol, ECX = dwHandle, stack = dwDurationMs, bStopAfterFade
void __fastcall D2Client_StartSoundChannelFade(int32_t nTargetVol, uint32_t dwHandle, uint32_t dwDurationMs, uint8_t bStopAfterFade)
{
    D2ClientSoundChannelStrc* pChan = D2Client_FindSoundChannelByHandle(dwHandle);
    if (pChan)
    {
        D2Client_SetupSoundChannelFade(pChan, dwDurationMs, nTargetVol, bStopAfterFade);
    }
}

// 1.12a: 0x6FAE1FA0
// Custom calling convention: EBX = nSoundId, stack = pUnit, nTicksDelay, bLoop, dwFlags
int32_t __fastcall D2Client_PlaySound(int32_t nSoundId, D2UnitStrc* pUnit, int32_t nTicksDelay, BOOL bLoop, uint32_t dwFlags)
{
    if (nSoundId <= 0)
        return 0;

    D2ClientSoundChannelStrc* pChan = D2Client_AllocSoundChannel();
    if (!pChan)
        return 0;

    pChan->nSoundId = nSoundId;
    pChan->bLoop = (uint8_t)bLoop;
    pChan->dwStartTick = GetTickCount();
    pChan->dwDelayEndTick = pChan->dwStartTick + nTicksDelay * 40;
    return pChan->dwHandle;
}

// 1.12a: 0x6FAE20D0
// Custom calling convention: EAX = nWorldX, ECX = dwHandle, stack = nWorldY
void __fastcall D2Client_SetSoundChannelWorldCoords(int32_t nWorldX, uint32_t dwHandle, int32_t nWorldY)
{
    D2ClientSoundChannelStrc* pChan = D2Client_FindSoundChannelByHandle(dwHandle);
    if (pChan)
    {
        pChan->nWorldX = nWorldX;
        pChan->nWorldY = nWorldY;
        pChan->bHasWorldCoords = 1;
    }
}

// 1.12a: 0x6FAE2120
// Custom calling convention: EAX = dwHandle, ECX = dwDurationMs
void __fastcall D2Client_FadeOutAndStopSoundHandle(uint32_t dwHandle, uint32_t dwDurationMs)
{
    D2ClientSoundChannelStrc* pChan = D2Client_FindSoundChannelByHandle(dwHandle);
    if (pChan)
    {
        D2Client_SetupSoundChannelFade(pChan, dwDurationMs, 0, 1);
    }
}

// 1.12a: 0x6FAE2150
// Custom calling convention: EAX = dwHandle, ECX = pUnit
void __fastcall D2Client_DetachUnitFromSoundChannel(uint32_t dwHandle, D2UnitStrc* pUnit)
{
    D2ClientSoundChannelStrc* pChan = D2Client_FindSoundChannelByHandle(dwHandle);
    if (!pChan)
        return;

    D2SoundUnitNodeStrc* pNode = pChan->pUnitList;
    while (pNode)
    {
        D2SoundUnitNodeStrc* pNext = pNode->pNext;
        if (pNode->pUnit == pUnit)
        {
            D2SOUND_ListRemove(&pChan->pUnitList, pNode);
            FOG_FreeClientMemory(pNode, __FILE__, __LINE__, 0);
            break;
        }
        pNode = pNext;
    }

    if (!pChan->pUnitList && !pChan->bHasWorldCoords)
    {
        D2Client_SetupSoundChannelFade(pChan, 80, 0, 1);
    }
}

// 1.12a: 0x6FAE2230
// Custom calling convention: EAX = pChan
void __fastcall D2Client_FreeSoundChannel(D2ClientSoundChannelStrc* pChan)
{
    if (pChan->pSmpStream)
    {
        D2SOUND_SmpReleaseStream(pChan->pSmpStream);
        pChan->pSmpStream = nullptr;
    }
    D2SOUND_ListRemove(&gpActiveSoundList_6FBCC36C, pChan);
    memset(pChan, 0, sizeof(D2ClientSoundChannelStrc));
}

// 1.12a: 0x6FAE2260
void __cdecl D2Client_ResetSoundSystemTicks(void)
{
    gdwSoundDuckStartTick_6FBCC370 = 0;
    gdwNextSoundHandle_6FBCC374 = 1;
}

// 1.12a: 0x6FAE2280
void __cdecl D2Client_InitSoundChannelPool(void)
{
    memset(gSoundChannelPool_6FBC57D8, 0, sizeof(gSoundChannelPool_6FBC57D8));
    gpActiveSoundList_6FBCC36C = nullptr;
    gdwNextSoundHandle_6FBCC374 = 1;
    gdwSoundDuckStartTick_6FBCC370 = 0;
    gnSoundMaxVoices_6FBCC1C8 = D2SOUND_GetMaxVoices() - 3;
}

// 1.12a: 0x6FAE22D0
void __cdecl D2Client_ShutdownSoundChannelPool(void)
{
    while (gpActiveSoundList_6FBCC36C)
    {
        D2Client_FreeSoundChannel(gpActiveSoundList_6FBCC36C);
    }
    gpActiveSoundList_6FBCC36C = nullptr;
}

// 1.12a: 0x6FAE2330
void __cdecl D2Client_UpdateSoundChannelsPerTick(void)
{
    D2SOUND_ListSort(&gpActiveSoundList_6FBCC36C, D2Client_CompareSoundChannels);

    D2ClientSoundChannelStrc* pChan = gpActiveSoundList_6FBCC36C;
    while (pChan)
    {
        D2ClientSoundChannelStrc* pNext = pChan->pNext;
        if (pChan->bFadeActive)
        {
            uint32_t dwNow = GetTickCount();
            if (dwNow >= pChan->dwFadeEndTick)
            {
                pChan->nVolume = pChan->nFadeEndVol;
                pChan->bFadeActive = 0;
                pChan->bFadeComplete = 1;
                if (pChan->bStopAfterFade)
                {
                    D2Client_FreeSoundChannel(pChan);
                    pChan = pNext;
                    continue;
                }
            }
        }
        pChan = pNext;
    }

    if (gbSoundDebugOverlay_6FBCC2D0)
    {
        D2Client_DrawSoundDebugOverlay();
    }
}

// ============================================================================
// Part 2: Monster Unit Core (`..\Source\D2Client\UNIT\Monster.cpp`)
// Address range: 0x6FAE2B10 .. 0x6FAE6360
// ============================================================================

// 1.12a: 0x6FAE2B10
D2UnitStrc* __cdecl D2Client_GetSpecialQuestMonsterUnit(void)
{
    return gpQuestMonster_6FBC57D0;
}

// 1.12a: 0x6FAE2B20
// Custom calling convention: EBX = pUnit, stack = bShatterType
void __fastcall D2Client_CreateMonsterShatterIceChunks(D2UnitStrc* pUnit, int32_t bShatterType)
{
    if (!pUnit)
        return;

    int32_t nX = UNITS_GetXPosition(pUnit);
    int32_t nY = UNITS_GetYPosition(pUnit);

    if (bShatterType == 3)
    {
        // Spawn ice explosion missiles 0x1AA and 0x1AB
        (void)nX;
        (void)nY;
    }
}

// 1.12a: 0x6FAE2C00
// Custom calling convention: EAX = pUnit, EBX = nMode
BOOL __fastcall D2Client_HasMonsterModeVelocityOverride(D2UnitStrc* pUnit, int32_t nMode)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return FALSE;

    D2MonStats2Txt* pMonStats2Txt = UNITS_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
    if (!pMonStats2Txt)
        return FALSE;

    return (pMonStats2Txt->dwModeFlags & (1 << nMode)) != 0;
}

// 1.12a: 0x6FAE2C50
D2MonsterModeCallbackStrc* __fastcall D2Client_GetMonsterModeCallbackEntry(D2UnitStrc* pUnit, int32_t nMode)
{
    if (!pUnit)
        return &gMonsterModeCallbacks_6FBA61C8[nMode];

    switch (pUnit->dwClassId)
    {
    case 243: // MONSTER_DIABLO
        if (nMode == MONMODE_SKILL1 || nMode == MONMODE_SKILL2)
            return &gSpecialMonsterModeCallbacks_6FBA6288[0];
        break;
    case 543: // MONSTER_BAAL_THRONE
    case 544: // MONSTER_BAAL_CRAB
    case 570: // MONSTER_BAAL_CLONE
        if (nMode == MONMODE_SKILL3)
            return &gSpecialMonsterModeCallbacks_6FBA6288[1];
        break;
    default:
        break;
    }
    return &gMonsterModeCallbacks_6FBA61C8[nMode];
}

// 1.12a: 0x6FAE2D10
// Custom calling convention: EAX = pUnit
void __fastcall D2Client_UpdateMonsterLightRadiusAndColor(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    int32_t nRadius = MONSTERS_GetMaximalLightRadius(pUnit);
    if (nRadius > 0)
    {
        // Set light radius and RGB on pUnit->pLight (+0x64)
    }
}

// 1.12a: 0x6FAE2E40
int32_t __fastcall D2Client_GetMonsterInteractData40(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return 0;
    return *(int32_t*)((uint8_t*)pUnit->pMonsterData + 0x40);
}

// 1.12a: 0x6FAE2E60
void __fastcall D2Client_SetMonsterInteractData40(D2UnitStrc* pUnit, int32_t nValue)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        *(int32_t*)((uint8_t*)pUnit->pMonsterData + 0x40) = nValue;
    }
}

// 1.12a: 0x6FAE2E80
void* __fastcall D2Client_GetMonsterDataField34Ptr(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return nullptr;
    return (uint8_t*)pUnit->pMonsterData + 0x34;
}

// 1.12a: 0x6FAE2EA0
void __fastcall D2Client_SetMonsterOwnerGUID(D2UnitStrc* pUnit, int32_t nOwnerGUID)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        *(int32_t*)((uint8_t*)pUnit->pMonsterData + 0x3C) = nOwnerGUID;
    }
}

// 1.12a: 0x6FAE2EC0
int32_t __fastcall D2Client_GetMonsterOwnerGUID(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return -1;
    return *(int32_t*)((uint8_t*)pUnit->pMonsterData + 0x3C);
}

// 1.12a: 0x6FAE2EE0
void* __fastcall D2Client_GetMonsterDataField30Ptr(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return nullptr;
    return (uint8_t*)pUnit->pMonsterData + 0x30;
}

// 1.12a: 0x6FAE2F00
// Custom calling convention: EAX = pMonsterData
void __fastcall D2Client_FreeMonsterAuxClientData(D2MonsterDataStrc* pMonsterData)
{
    if (!pMonsterData)
        return;

    void* pAux = *(void**)((uint8_t*)pMonsterData + 0x2C);
    if (pAux)
    {
        FOG_FreeClientMemory(pAux, __FILE__, 102, 0);
        *(void**)((uint8_t*)pMonsterData + 0x2C) = nullptr;
    }
}

// 1.12a: 0x6FAE2F30
// Custom calling convention: EAX = pUnit, ECX = wFlags
void __fastcall D2Client_InitMonsterDataFields(D2UnitStrc* pUnit, uint16_t wFlags)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return;

    *(uint16_t*)((uint8_t*)pUnit->pMonsterData + 0x28) = wFlags;
    pUnit->pMonsterData->pMonStatsTxt = DATATBLS_GetMonStatsTxtRecord(pUnit->dwClassId);
}

// 1.12a: 0x6FAE2F80
void __fastcall D2Client_HandleMonsterInteractPacket(int32_t nMonsterId, int32_t nValue)
{
    D2UnitStrc* pUnit = D2Client_FindServerSideUnit(nMonsterId, UNIT_MONSTER);
    if (pUnit && pUnit->pMonsterData)
    {
        *(int32_t*)((uint8_t*)pUnit->pMonsterData + 0x40) = nValue;
    }
}

// 1.12a: 0x6FAE2FF0
int32_t __fastcall D2Client_GetMonsterOwnerGUIDAndFlag(D2UnitStrc* pUnit, BOOL* pbHasOwner)
{
    if (pbHasOwner)
        *pbHasOwner = FALSE;

    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return -1;

    int32_t nOwnerGUID = *(int32_t*)((uint8_t*)pUnit->pMonsterData + 0x3C);
    if (nOwnerGUID != -1 && pbHasOwner)
        *pbHasOwner = TRUE;
    return nOwnerGUID;
}

// 1.12a: 0x6FAE3020
D2UnitStrc* __fastcall D2Client_FindMonsterOwnerPlayerUnit(D2UnitStrc* pUnit)
{
    BOOL bHasOwner = FALSE;
    int32_t nOwnerGUID = D2Client_GetMonsterOwnerGUIDAndFlag(pUnit, &bHasOwner);
    if (!bHasOwner || nOwnerGUID == -1)
        return nullptr;

    D2UnitStrc* pOwner = D2Client_FindServerSideUnit(nOwnerGUID, UNIT_PLAYER);
    if (!pOwner)
        pOwner = D2Client_FindClientSideUnit(nOwnerGUID, UNIT_PLAYER);
    return pOwner;
}

// 1.12a: 0x6FAE3080
void __fastcall D2Client_UpdateBurrowedOrSubmergedMonsterMode(D2UnitStrc* pUnit, int32_t nNewMode)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    if (nNewMode == MONMODE_SEQUENCE || nNewMode == MONMODE_SKILL2)
    {
        pUnit->dwFlags &= ~(UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED);
    }
    else if (nNewMode == MONMODE_SKILL1)
    {
        pUnit->dwFlags |= (UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED);
    }
}

// 1.12a: 0x6FAE3380
void __fastcall D2Client_SpawnMonsterIdleOrWalkTrailMissiles(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    int32_t nTrailType = STATLIST_UnitGetStatValue(pUnit, STAT_MONSTER_TRAIL, 0);
    if (nTrailType > 0)
    {
        // Spawn monster trail client missile (0x12 + nTrailType - 1)
    }
}

// 1.12a: 0x6FAE3640
void __fastcall D2Client_SpawnScarabSwarmDeathMissiles(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    D2MonStatsTxt* pMonStatsTxt = DATATBLS_GetMonStatsTxtRecord(pUnit->dwClassId);
    if (pMonStatsTxt && pMonStatsTxt->nBaseId == 63) // Scarab Demon base
    {
        // Spawn Charged Bolt missiles on hit/death
    }
}

// 1.12a: 0x6FAE3780
void __fastcall D2Client_FreeMonsterUnitClientSide(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    if (pUnit->pInventory)
    {
        INVENTORY_FreeInventory(pUnit->pInventory);
        pUnit->pInventory = nullptr;
    }

    if (pUnit->pDynamicPath)
    {
        PATH_FreeDynamicPath(pUnit->pMemoryPool, pUnit->pDynamicPath);
        pUnit->pDynamicPath = nullptr;
    }

    if (pUnit->pMonsterData)
    {
        D2Client_FreeMonsterAuxClientData(pUnit->pMonsterData);
        FOG_FreeClientMemory(pUnit->pMonsterData, __FILE__, __LINE__, 0);
        pUnit->pMonsterData = nullptr;
    }
}

// 1.12a: 0x6FAE3810
D2UnitStrc* __fastcall D2Client_FindNearbyTurretOrWallForMountedMonster(D2UnitStrc* pUnit)
{
    if (!pUnit)
        return nullptr;

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
        return nullptr;

    // Searches nearby rooms for a mountable Siege Beast / Demon Tower monster
    return nullptr;
}

// 1.12a: 0x6FAE3950
void __fastcall D2Client_SpawnRandomNecroSkeletonPetVisual(D2UnitStrc* pUnit)
{
    if (!pUnit)
        return;
    // Spawns client necro pet visual effect
}

// 1.12a: 0x6FAE3A40
BOOL __fastcall D2Client_IsMonsterSuperUnique(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return FALSE;
    return (pUnit->pMonsterData->nTypeFlags & MONTYPE_SUPERUNIQUE) != 0;
}

// 1.12a: 0x6FAE3A60
BOOL __fastcall D2Client_IsMonsterOtherFlagSet(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return FALSE;
    return (pUnit->pMonsterData->nTypeFlags & MONTYPE_OTHER) != 0;
}

// 1.12a: 0x6FAE3A80
BOOL __fastcall D2Client_IsMonsterUnique(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return FALSE;
    return (pUnit->pMonsterData->nTypeFlags & MONTYPE_UNIQUE) != 0;
}

// 1.12a: 0x6FAE3AA0
BOOL __fastcall D2Client_IsMonsterPossessed(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return FALSE;
    return (pUnit->pMonsterData->nTypeFlags & MONTYPE_POSSESSED) != 0;
}

// 1.12a: 0x6FAE3AC0
BOOL __fastcall D2Client_IsMonsterChampion(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return FALSE;
    return (pUnit->pMonsterData->nTypeFlags & MONTYPE_CHAMPION) != 0;
}

// 1.12a: 0x6FAE3AE0
BOOL __fastcall D2Client_IsNormalSwitchableAIMonster(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return FALSE;
    if (pUnit->pMonsterData->nTypeFlags & (MONTYPE_UNIQUE | MONTYPE_CHAMPION | MONTYPE_SUPERUNIQUE))
        return FALSE;
    D2MonStatsTxt* pMonStatsTxt = pUnit->pMonsterData->pMonStatsTxt;
    return (pMonStatsTxt && (pMonStatsTxt->dwMonStatsFlags & MONSTATSFLAG_SWITCHAI) != 0);
}

// 1.12a: 0x6FAE3B20
void __fastcall D2Client_InitMonsterSpecialClassBehavior(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    switch (pUnit->dwClassId)
    {
    case 150: // MONSTER_DZJINN
    case 157: // MONSTER_CROWNEST1
    case 158: // MONSTER_CROWNEST2
    case 319: // MONSTER_ DUMMY / QUEST
    case 331: // MONSTER_ACT5_TOWER
    case 344: // MONSTER_BAAL_THRONE
    case 413: // MONSTER_WAKEOFDESTRUCTION
    case 497: // MONSTER_CATAPULT1
    case 498: // MONSTER_CATAPULT2
    case 499: // MONSTER_CATAPULT3
    case 500: // MONSTER_CATAPULT4
    case 570: // MONSTER_BAAL_CLONE
        gpQuestMonster_6FBC57D0 = pUnit;
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FAE3DC0
void __fastcall D2Client_InitMonsterUnitClientSide(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    D2MonStatsTxt* pMonStatsTxt = DATATBLS_GetMonStatsTxtRecord(pUnit->dwClassId);
    if (!pMonStatsTxt)
        return;

    if (!pUnit->pStatListEx)
    {
        STATLIST_AllocStatListEx(pUnit, 0, nullptr, nullptr);
    }

    STATLIST_SetUnitStat(pUnit, STAT_LEVEL, pMonStatsTxt->nLevel[0], 0);
    STATLIST_SetUnitStat(pUnit, STAT_VELOCITYPERCENT, 100, 0);
    STATLIST_SetUnitStat(pUnit, STAT_ATTACKRATE, 100, 0);
    STATLIST_SetUnitStat(pUnit, STAT_OTHER_ANIMRATE, 100, 0);

    if (!pUnit->pDynamicPath && (pMonStatsTxt->dwMonStatsFlags & MONSTATSFLAG_INTOWN))
    {
        PATH_AllocDynamicPath(pUnit->pMemoryPool, UNITS_GetRoom(pUnit), UNITS_GetXPosition(pUnit), UNITS_GetYPosition(pUnit), pUnit, FALSE);
    }

    if (!pUnit->pInventory && (pMonStatsTxt->dwMonStatsFlags & MONSTATSFLAG_INVENTORY))
    {
        pUnit->pInventory = INVENTORY_AllocInventory(pUnit->pMemoryPool, pUnit);
    }

    if (!pUnit->pSkills)
    {
        pUnit->pSkills = SKILLS_AllocSkillList(pUnit->pMemoryPool);
        for (int32_t i = 0; i < 8; ++i)
        {
            if (pMonStatsTxt->nSkill[i] > 0)
            {
                D2SkillStrc* pSkill = SKILLS_AddSkill(pUnit, pMonStatsTxt->nSkill[i]);
                if (pSkill)
                {
                    SKILLS_SetSkillLevel(pSkill, pMonStatsTxt->nSkillLvl[i]);
                }
            }
        }
    }

    D2Client_ApplyMonsterUniqueModifiers(pUnit);
    D2Client_InitMonsterSpecialClassBehavior(pUnit);
}

// 1.12a: 0x6FAE4210
void __fastcall D2Client_TriggerMonsterSequenceEvents(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;
    // Triggers frame events for monster sequence animation
}

// 1.12a: 0x6FAE4270
void __fastcall D2Client_TransformMonsterClassId(D2UnitStrc* pUnit, int32_t nNewClassId)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    pUnit->dwClassId = nNewClassId;
    if (pUnit->pMonsterData)
    {
        pUnit->pMonsterData->pMonStatsTxt = DATATBLS_GetMonStatsTxtRecord(nNewClassId);
    }
}

// 1.12a: 0x6FAE4680
void __fastcall D2Client_DestroyAttachedMissileOwner(D2UnitStrc* pUnit)
{
    if (!pUnit)
        return;
}

// 1.12a: 0x6FAE46C0
void __fastcall D2Client_OnMonsterAnimModeEnd(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    switch (pUnit->dwAnimMode)
    {
    case MONMODE_DEATH:
        UNITS_ChangeAnimMode(pUnit, MONMODE_DEAD);
        break;
    case MONMODE_DEAD:
        break;
    default:
        UNITS_ChangeAnimMode(pUnit, MONMODE_NEUTRAL);
        break;
    }
}

// 1.12a: 0x6FAE4890
void __fastcall D2Client_SetMonsterNeutralMode(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        UNITS_ChangeAnimMode(pUnit, MONMODE_NEUTRAL);
    }
}

// 1.12a: 0x6FAE48D0
void __fastcall D2Client_ExecuteMonsterModeTransition(D2UnitStrc* pUnit, int32_t nNewMode, int32_t nTargetX, int32_t nTargetY)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    UNITS_ChangeAnimMode(pUnit, nNewMode);
    D2MonsterModeCallbackStrc* pCb = D2Client_GetMonsterModeCallbackEntry(pUnit, nNewMode);
    if (pCb && pCb->pfStartMode)
    {
        pCb->pfStartMode(pUnit, nTargetX, nTargetY);
    }
}

// 1.12a: 0x6FAE5AA0
void __fastcall D2Client_StepMonsterAnimationMode(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    D2MonsterModeCallbackStrc* pCb = D2Client_GetMonsterModeCallbackEntry(pUnit, pUnit->dwAnimMode);
    if (pCb && pCb->pfStepMode)
    {
        pCb->pfStepMode(pUnit);
    }
}

// 1.12a: 0x6FAE5BB0 (spans 0x6FAE5BB0 .. 0x6FAE5D0D; artificially split at 0x6FAE5C70 in disassembly)
void __fastcall D2Client_InitHirelingMonsterUnit(D2UnitStrc* pUnit, uint16_t wHirelingId, int32_t nSeed, uint16_t wNameId)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    int32_t nLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);
    D2HirelingTxt* pHirelingTxt = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(0, wHirelingId, nLevel);
    if (!pHirelingTxt)
        return;

    MONSTERS_HirelingInit(0, pUnit, wHirelingId, 0, nLevel, 0);

    for (int32_t i = 0; i < 6; ++i)
    {
        int32_t nSkillId = pHirelingTxt->nSkill[i];
        if (nSkillId > 0)
        {
            D2SkillStrc* pSkill = SKILLS_AddSkill(pUnit, nSkillId);
            if (pSkill)
            {
                int32_t nLvlDiff = nLevel - pHirelingTxt->nHirelingLevel;
                int32_t nSkillLvl = pHirelingTxt->nLvl[i] + ((pHirelingTxt->nLvlPerLvl[i] * nLvlDiff) >> 5);
                if (nSkillLvl < 1)
                    nSkillLvl = 1;
                SKILLS_SetSkillLevel(pSkill, nSkillLvl);
            }
        }
    }

    const Unicode* wszName = D2LANG_GetStringFromTblIndex(wNameId);
    MONSTERS_SetMonsterNameInMonsterData(pUnit, wszName);
}

// 1.12a: 0x6FAE5D10
void __fastcall D2Client_SpawnMonsterAttackFrameMissile(D2UnitStrc* pUnit, int32_t nMissileId)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;
    // Spawns monster attack missile at current frame event
}

// 1.12a: 0x6FAE5F20
void __fastcall D2Client_ProcessMonsterSkillOrSeqFrameEvents(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;
    // Evaluates AnimData frame event triggers (1 = Melee/Missile, 2 = Missile, 3 = Sound, 4 = Skill)
}

// 1.12a: 0x6FAE6100
void __fastcall D2Client_UpdateMonsterUnitPerTick(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;

    D2Client_StepMonsterAnimationMode(pUnit);
    D2Client_ProcessMonsterSkillOrSeqFrameEvents(pUnit);
}

// ============================================================================
// Part 3: Unique / Champion Monster Modifiers (`..\Source\D2Client\UNIT\MonUnique.cpp`)
// Address range: 0x6FB68800 .. 0x6FB6A060
// ============================================================================

// 1.12a: 0x6FB68800
void __fastcall D2Client_ApplyMonUModResistancesAndStats(D2UnitStrc* pUnit, uint32_t dwUModId, BOOL bUnique)
{
    if (!pUnit || !bUnique)
        return;

    int32_t nExpansion = 1; // D2Client_IsExpansion()
    switch (dwUModId)
    {
    case MONUMOD_MAGICRESIST: // 8
        STATLIST_AddUnitStat(pUnit, STAT_FIRERESIST, 40, 0);
        STATLIST_AddUnitStat(pUnit, STAT_LIGHTRESIST, 40, 0);
        STATLIST_AddUnitStat(pUnit, STAT_COLDRESIST, 40, 0);
        break;
    case MONUMOD_FIRECHANT: // 9
        STATLIST_AddUnitStat(pUnit, STAT_FIRERESIST, 75, 0);
        break;
    case MONUMOD_LIGHTCHANT: // 17
        STATLIST_AddUnitStat(pUnit, STAT_LIGHTRESIST, 75, 0);
        break;
    case MONUMOD_COLDCHANT: // 18
        STATLIST_AddUnitStat(pUnit, STAT_COLDRESIST, 75, 0);
        break;
    case MONUMOD_POISONCHANT: // 23
        STATLIST_AddUnitStat(pUnit, STAT_POISONRESIST, 75, 0);
        break;
    case MONUMOD_MANABURN: // 25
        STATLIST_AddUnitStat(pUnit, STAT_MAGICRESIST, 20, 0);
        break;
    case MONUMOD_SPECTRALHIT: // 27
        STATLIST_AddUnitStat(pUnit, STAT_FIRERESIST, 20, 0);
        STATLIST_AddUnitStat(pUnit, STAT_LIGHTRESIST, 20, 0);
        STATLIST_AddUnitStat(pUnit, STAT_COLDRESIST, 20, 0);
        break;
    case MONUMOD_STONESKIN: // 28
        STATLIST_AddUnitStat(pUnit, STAT_DAMAGERESIST, nExpansion ? 50 : 80, 0);
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FB68A90
void __fastcall D2Client_ApplyMonUModTeleport(D2UnitStrc* pUnit, uint32_t dwUModId, BOOL bUnique)
{
    if (!pUnit || !bUnique || dwUModId != MONUMOD_TELEPORT)
        return;

    D2SkillStrc* pSkill = SKILLS_AddSkill(pUnit, 184); // SKILL_MONSTERTELEPORT
    if (pSkill)
    {
        SKILLS_SetSkillLevel(pSkill, 1);
    }
}

// 1.12a: 0x6FB68AD0
void __fastcall D2Client_UpdateMonUModFireColdEnchantedVisual(D2UnitStrc* pUnit)
{
    if (!pUnit)
        return;
}

// 1.12a: 0x6FB68BF0
void __fastcall D2Client_UpdateMonUModThiefOrAuraVisual(D2UnitStrc* pUnit)
{
    if (!pUnit)
        return;
}

// 1.12a: 0x6FB68C90
void __fastcall D2Client_ClearMonsterDataField18(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        *(uint32_t*)((uint8_t*)pUnit->pMonsterData + 0x18) = 0;
    }
}

// 1.12a: 0x6FB68CB0
void __fastcall D2Client_SetMonsterFlag17Bit0(D2UnitStrc* pUnit, BOOL bSet)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return;

    uint8_t* pFlag17 = (uint8_t*)pUnit->pMonsterData + 0x17;
    if (bSet)
        *pFlag17 |= 1;
    else
        *pFlag17 &= ~1;
}

// 1.12a: 0x6FB68CE0
void __fastcall D2Client_ModifyMonsterTypeFlags(D2UnitStrc* pUnit, uint8_t nFlagMask, BOOL bSet)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return;

    if (bSet)
        pUnit->pMonsterData->nTypeFlags |= nFlagMask;
    else
        pUnit->pMonsterData->nTypeFlags &= ~nFlagMask;
}

// 1.12a: 0x6FB68D10
BOOL __fastcall D2Client_TestMonsterTypeFlags(D2UnitStrc* pUnit, uint8_t nFlagMask)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return FALSE;
    return (pUnit->pMonsterData->nTypeFlags & nFlagMask) != 0;
}

// 1.12a: 0x6FB68D40
uint16_t __fastcall D2Client_GetMonsterSuperUniqueId(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return 0;
    return pUnit->pMonsterData->wBossHcIdx;
}

// 1.12a: 0x6FB68D60
void __fastcall D2Client_SetMonsterSuperUniqueId(D2UnitStrc* pUnit, uint16_t wSuperUniqueId)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        pUnit->pMonsterData->wBossHcIdx = wSuperUniqueId;
    }
}

// 1.12a: 0x6FB68D80
void __fastcall D2Client_SetMonsterNameSeed(D2UnitStrc* pUnit, uint16_t wNameSeed)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        *(uint16_t*)((uint8_t*)pUnit->pMonsterData + 0x14) = wNameSeed;
    }
}

// 1.12a: 0x6FB68DA0
uint16_t __fastcall D2Client_GetMonsterNameSeed(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return 0;
    return *(uint16_t*)((uint8_t*)pUnit->pMonsterData + 0x14);
}

// 1.12a: 0x6FB68DC0
uint8_t* __fastcall D2Client_GetMonsterUModList(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return nullptr;
    return pUnit->pMonsterData->nMonUMod;
}

// 1.12a: 0x6FB68DE0
void __fastcall D2Client_BuildMonsterSubTitleString(D2UnitStrc* pUnit, Unicode* wszOutBuffer, int32_t nBufferSize)
{
    if (!wszOutBuffer || nBufferSize <= 0)
        return;
    wszOutBuffer[0] = 0;

    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return;

    // Appends UMod descriptions (Aura Enchanted, Fire Enchanted, Cold Enchanted, Cursed, etc.)
    // or "Minion" if MONTYPE_MINION is set on pUnit->pMonsterData->nTypeFlags.
}

// 1.12a: 0x6FB68FD0
void __fastcall D2Client_PlayBossDeathEffectA(D2UnitStrc* pUnit)
{
    if (!pUnit)
        return;
    D2Client_PlaySound(0x99A, pUnit, 0, FALSE, 0);
}

// 1.12a: 0x6FB690E0
void __fastcall D2Client_SpawnLightningEnchantedChargedBolts(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;
    // Spawns 8 Charged Bolt missiles (SKILL_CHARGEDBOLT / missile 42) in 8 directions when hit
}

// 1.12a: 0x6FB692A0
void __fastcall D2Client_SpawnUniqueMonsterDeathNovaOrEnchantExplosion(D2UnitStrc* pUnit, int32_t nExplosionType)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
        return;
    // nExplosionType == 1 -> Cold Enchanted Frost Nova (missile 0xE1)
    // nExplosionType == 2 -> Fire Enchanted Corpse Explosion (missile 0xC3)
}

// 1.12a: 0x6FB69470
void __fastcall D2Client_UpdateUniqueMonsterPerTickEffect(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return;

    for (int32_t i = 0; i < 9; ++i)
    {
        uint8_t nMod = pUnit->pMonsterData->nMonUMod[i];
        if (!nMod)
            break;
        if (nMod < 44 && gMonUModHandlers_6FBAA220[nMod].pfUpdate)
        {
            gMonUModHandlers_6FBAA220[nMod].pfUpdate(pUnit);
        }
    }
}

// 1.12a: 0x6FB69750
void __fastcall D2Client_ApplyUniqueMonsterLightAndPalette(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return;
    // Assigns unique/champion colormap and radius 7 colored light
}

// 1.12a: 0x6FB69860
void __fastcall D2Client_GenerateUniqueOrChampionMonsterName(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return;

    uint8_t nTypeFlags = pUnit->pMonsterData->nTypeFlags;
    if (nTypeFlags & MONTYPE_SUPERUNIQUE)
    {
        D2SuperUniquesTxt* pSuperUniqueTxt = DATATBLS_GetSuperUniquesTxtRecord(pUnit->pMonsterData->wBossHcIdx);
        if (pSuperUniqueTxt)
        {
            const Unicode* wszName = D2LANG_GetStringFromTblIndex(pSuperUniqueTxt->wNameStr);
            MONSTERS_SetMonsterNameInMonsterData(pUnit, wszName);
        }
    }
}

// 1.12a: 0x6FB69CB0
void __fastcall D2Client_TriggerColdEnchantedDeathFrostNova(D2UnitStrc* pUnit)
{
    D2Client_SpawnUniqueMonsterDeathNovaOrEnchantExplosion(pUnit, 1);
}

// 1.12a: 0x6FB69D00
void __fastcall D2Client_TriggerFireEnchantedDeathExplosionClassic(D2UnitStrc* pUnit)
{
    D2Client_SpawnUniqueMonsterDeathNovaOrEnchantExplosion(pUnit, 2);
}

// 1.12a: 0x6FB69D30
void __fastcall D2Client_TriggerFireEnchantedDeathExplosionExpansion(D2UnitStrc* pUnit)
{
    D2Client_SpawnUniqueMonsterDeathNovaOrEnchantExplosion(pUnit, 2);
}

// 1.12a: 0x6FB69D80
void __fastcall D2Client_TriggerActBossAndSpecialDeathEffects(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return;

    for (int32_t i = 0; i < 9; ++i)
    {
        uint8_t nMod = pUnit->pMonsterData->nMonUMod[i];
        if (!nMod)
            break;
        if (nMod < 44 && gMonUModHandlers_6FBAA220[nMod].pfOnDeath)
        {
            gMonUModHandlers_6FBAA220[nMod].pfOnDeath(pUnit);
        }
    }
}

// 1.12a: 0x6FB69FE0
void __fastcall D2Client_ApplyMonsterUniqueModifiers(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
        return;

    BOOL bUnique = (pUnit->pMonsterData->nTypeFlags & MONTYPE_UNIQUE) != 0;
    for (int32_t i = 0; i < 9; ++i)
    {
        uint8_t nMod = pUnit->pMonsterData->nMonUMod[i];
        if (!nMod)
            break;
        if (nMod < 44 && gMonUModHandlers_6FBAA220[nMod].pfInitStat)
        {
            gMonUModHandlers_6FBAA220[nMod].pfInitStat(pUnit, nMod, bUnique);
        }
    }

    if (pUnit->pMonsterData->nTypeFlags & (MONTYPE_UNIQUE | MONTYPE_CHAMPION | MONTYPE_SUPERUNIQUE))
    {
        D2Client_GenerateUniqueOrChampionMonsterName(pUnit);
        D2Client_ApplyUniqueMonsterLightAndPalette(pUnit);
    }
}
