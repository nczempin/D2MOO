#pragma once

#include <D2Dll.h>
#include <D2BasicTypes.h>
#include <Fog.h>
#include <cstdint>
#include <mmsystem.h>
#include <dsound.h>

// 1.10f Image Base: 0x6F980000
// 1.12a Image Base: 0x6F9B0000

#ifdef D2_VERSION_110F
constexpr int D2SoundImageBase = 0x6F980000;
#elif defined(D2_VERSION_112A)
constexpr int D2SoundImageBase = 0x6F9B0000;
#endif

#pragma pack(push, 1)

enum D2SoundSampleFlags : uint32_t
{
	D2SOUNDSMP_FLAG_STEREO      = 0x01,
	D2SOUNDSMP_FLAG_3D          = 0x02,
	D2SOUNDSMP_FLAG_NOOCCLUSION = 0x04,
};

struct D2SoundSmpStrc // sizeof 0x60
{
	IDirectSoundBuffer* pDSBuffer;         // 0x00
	uint32_t dwBufferSize;                 // 0x04
	uint32_t dwHalfBufferSize;             // 0x08
	IDirectSound3DBuffer* p3DBuffer;       // 0x0C
	IKsPropertySet* pPropertySet;          // 0x10
	const uint8_t* pWaveData;              // 0x14
	uint32_t dwWaveSize;                   // 0x18
	BOOL bPlaying;                         // 0x1C
	uint32_t dwPlayCursorTotal;            // 0x20
	uint32_t dwWriteOffsetTotal;           // 0x24
	int32_t nNextHalfToFill;               // 0x28
	BOOL bEndOfDataReached;                // 0x2C
	uint32_t dwSingleShotEndOffset;        // 0x30
	uint32_t dwFlags;                      // 0x34
	int32_t nVolume;                       // 0x38
	float fOcclusion;                      // 0x3C
	BOOL bLooping;                         // 0x40
	uint32_t dwLoopStart;                  // 0x44
	HSFILE hFile;                          // 0x48
	BOOL bFading;                          // 0x4C
	int32_t nFadeStartVol;                 // 0x50
	int32_t nFadeEndVol;                   // 0x54
	uint32_t dwFadeStartTick;              // 0x58
	uint32_t dwFadeEndTick;                // 0x5C
};
static_assert(sizeof(D2SoundSmpStrc) == 0x60, "D2SoundSmpStrc size must be 0x60");

struct D2SoundFastEntryStrc // sizeof 0x60
{
	char szFileName[0x50];                 // 0x00
	uint8_t* pWaveBuffer;                  // 0x50
	uint32_t dwWaveSize;                   // 0x54
	uint32_t dwFlags;                      // 0x58
	D2SoundFastEntryStrc* pNext;           // 0x5C
};
static_assert(sizeof(D2SoundFastEntryStrc) == 0x60, "D2SoundFastEntryStrc size must be 0x60");

struct D2SoundListNodeStrc // sizeof 0x08
{
	void* pItem;                           // 0x00
	D2SoundListNodeStrc* pNext;            // 0x04
};
static_assert(sizeof(D2SoundListNodeStrc) == 0x08, "D2SoundListNodeStrc size must be 0x08");

struct D2SoundEaxEnvStrc // sizeof 0x34
{
	uint32_t dwEnvironment;                // 0x00
	float fEnvSize;                        // 0x04
	float fEnvDiffusion;                   // 0x08
	int32_t lRoom;                         // 0x0C
	int32_t lRoomHF;                       // 0x10
	float fDecayTime;                      // 0x14
	float fDecayHFRatio;                   // 0x18
	int32_t lReflections;                  // 0x1C
	float fReflectionsDelay;               // 0x20
	int32_t lReverb;                       // 0x24
	float fReverbDelay;                    // 0x28
	float fRoomRolloffFactor;              // 0x2C
	float fAirAbsorptionHF;                // 0x30
};
static_assert(sizeof(D2SoundEaxEnvStrc) == 0x34, "D2SoundEaxEnvStrc size must be 0x34");

struct D2SoundMusicTrackStrc // sizeof 0x08
{
	const char* szFileName;                // 0x00
	BOOL bPlayed;                          // 0x04
};
static_assert(sizeof(D2SoundMusicTrackStrc) == 0x08, "D2SoundMusicTrackStrc size must be 0x08");

#pragma pack(pop)

// 1.12a @10000: 0x67C0
D2FUNC_DLL(D2SOUND, SmpHasStream, BOOL, __fastcall, (D2SoundSmpStrc* pSmp), 0x67C0)
// 1.12a @10001: 0x7B50
D2FUNC_DLL(D2SOUND, GetNPCSpeech, int32_t, __fastcall, (), 0x7B50)
// 1.10f @10001: 0x21C0 | 1.12a @10002: 0x87A0
D2FUNC_DLL(D2SOUND, CloseSoundSystem, void, __fastcall, (), 0x21C0)
// 1.12a @10003: 0x83E0
D2FUNC_DLL(D2SOUND, GetUnknownZero, int32_t, __fastcall, (), 0x83E0)
// 1.12a @10004: 0x9730
D2FUNC_DLL(D2SOUND, IsMusicEnabled, BOOL, __fastcall, (), 0x9730)
// 1.12a @10005: 0x7480
D2FUNC_DLL(D2SOUND, SmpSetMasterVolume, void, __fastcall, (int32_t nVolume), 0x7480)
// 1.10f @10034: 0x4D60 | 1.12a @10006: 0x9890
D2FUNC_DLL(D2SOUND, EnableMusic, void, __fastcall, (int32_t bEnable), 0x4D60)
// 1.12a @10007: 0x7C10
D2FUNC_DLL(D2SOUND, GetSoundMixer, int32_t, __fastcall, (), 0x7C10)
// 1.12a @10008: 0x6E20
D2FUNC_DLL(D2SOUND, SmpSetVolume, void, __fastcall, (D2SoundSmpStrc* pSmp, int32_t nVolume), 0x6E20)
// 1.12a @10009: 0x6920
D2FUNC_DLL(D2SOUND, SmpGetVolume, int32_t, __fastcall, (D2SoundSmpStrc* pSmp), 0x6920)
// 1.12a @10010: 0x7E90
D2FUNC_DLL(D2SOUND, FastShutdown, void, __fastcall, (), 0x7E90)
// 1.12a @10011: 0x93F0
D2FUNC_DLL(D2SOUND, ListClear, void, __fastcall, (D2SoundListNodeStrc** ppHead), 0x93F0)
// 1.12a @10012: 0x6850
D2FUNC_DLL(D2SOUND, SmpGetStreamPos, uint32_t, __fastcall, (D2SoundSmpStrc* pSmp), 0x6850)
// 1.10f @10070: 0x2BD0 | 1.12a @10013: 0x7B00
D2FUNC_DLL(D2SOUND, SetOptionsMusic, int32_t, __fastcall, (int32_t bOptionsMusic), 0x2BD0)
// 1.12a @10014: 0x6B60
D2FUNC_DLL(D2SOUND, SmpGetMasterVolume, int32_t, __fastcall, (), 0x6B60)
// 1.12a @10015: 0x91E0
D2FUNC_DLL(D2SOUND, ListPrepend, void, __fastcall, (D2SoundListNodeStrc** ppHead, void* pItem), 0x91E0)
// 1.12a @10016: 0x6FA0
D2FUNC_DLL(D2SOUND, SmpDestroy, void, __fastcall, (D2SoundSmpStrc* pSmp), 0x6FA0)
// 1.12a @10017: 0x7B80
D2FUNC_DLL(D2SOUND, GetPositionalBias, int32_t, __fastcall, (), 0x7B80)
// 1.12a @10018: 0x6980
D2FUNC_DLL(D2SOUND, SmpStop, void, __fastcall, (D2SoundSmpStrc* pSmp), 0x6980)
// 1.12a @10019: 0x7650
D2FUNC_DLL(D2SOUND, SmpReset, void, __fastcall, (D2SoundSmpStrc* pSmp), 0x7650)
// 1.12a @10020: 0x9110
D2FUNC_DLL(D2SOUND, ListAppend, void, __fastcall, (D2SoundListNodeStrc** ppHead, void* pItem), 0x9110)
// 1.12a @10021: 0x9620
D2FUNC_DLL(D2SOUND, UtilCalc2DVolumeAndPan, void, __fastcall, (int32_t* pOutVolume, int32_t* pOutPan, float fX, float fY, float fZ), 0x9620)
// 1.12a @10022: 0x6C00
D2FUNC_DLL(D2SOUND, SmpSetReverbMix, void, __fastcall, (D2SoundSmpStrc* pSmp, float fReverbMix), 0x6C00)
// 1.12a @10023: 0x7BE0
D2FUNC_DLL(D2SOUND, GetMasterVolume, int32_t, __fastcall, (), 0x7BE0)
// 1.10f @10069: 0x2BC0 | 1.12a @10024: 0x7B20
D2FUNC_DLL(D2SOUND, GetOptionsMusic, int32_t, __fastcall, (), 0x2BC0)
// 1.12a @10025: 0x7E30
D2FUNC_DLL(D2SOUND, FastFadeAll, void, __fastcall, (), 0x7E30)
// 1.12a @10026: 0x7B30
D2FUNC_DLL(D2SOUND, SetNPCSpeech, int32_t, __fastcall, (int32_t nNPCSpeech), 0x7B30)
// 1.12a @10027: 0x6F30
D2FUNC_DLL(D2SOUND, SmpPlay, BOOL, __fastcall, (D2SoundSmpStrc* pSmp), 0x6F30)
// 1.12a @10028: 0x8540
D2FUNC_DLL(D2SOUND, SetEaxEnvironment, void, __fastcall, (const D2SoundEaxEnvStrc* pEnv), 0x8540)
// 1.12a @10029: 0x8050
D2FUNC_DLL(D2SOUND, FastCacheSound, void, __fastcall, (const char* szFile), 0x8050)
// 1.12a @10030: 0x7C20
D2FUNC_DLL(D2SOUND, LoadConfigFromRegistry, void, __fastcall, (), 0x7C20)
// 1.12a @10031: 0x9300
D2FUNC_DLL(D2SOUND, UtilRatioToMillibels, float, __fastcall, (float fMin, float fMax, float fVal), 0x9300)
// 1.10f @10000: 0x10E0 | 1.12a @10032: 0x8D80
#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 13
D2FUNC_DLL(D2SOUND, OpenSoundSystem, void, __fastcall, (BOOL bExpansion, BOOL bSoundBackground), 0x0000)
#else
D2FUNC_DLL(D2SOUND, OpenSoundSystem, void, __fastcall, (BOOL bExpansion), 0x10E0)
#endif
// 1.12a @10033: 0x6660
D2FUNC_DLL(D2SOUND, SmpNoop1, void, __fastcall, (), 0x6660)
// 1.12a @10034: 0x6CE0
D2FUNC_DLL(D2SOUND, SmpSetOcclusion, void, __fastcall, (D2SoundSmpStrc* pSmp, float fOcclusion), 0x6CE0)
// 1.10f @10048: 0x2B30 | 1.12a @10035: 0x7BB0
D2FUNC_DLL(D2SOUND, GetMusicVolume, int32_t, __fastcall, (), 0x2B30)
// 1.12a @10036: 0x68D0
D2FUNC_DLL(D2SOUND, SmpSetPosition3D, void, __fastcall, (D2SoundSmpStrc* pSmp, float fX, float fY, float fZ), 0x68D0)
// 1.10f @10065: 0x49D0 | 1.12a @10037: 0x98C0
D2FUNC_DLL(D2SOUND, FadeMusic, void, __fastcall, (), 0x49D0)
// 1.12a @10038: 0x7B60
D2FUNC_DLL(D2SOUND, SetPositionalBias, int32_t, __fastcall, (int32_t nBias), 0x7B60)
// 1.10f @10049: 0x2B40 | 1.12a @10039: 0x7B90
D2FUNC_DLL(D2SOUND, SetMusicVolume, int32_t, __fastcall, (int32_t nVolume), 0x2B40)
// 1.12a @10040: 0x6B80
D2FUNC_DLL(D2SOUND, SmpFade, void, __fastcall, (D2SoundSmpStrc* pSmp, int32_t nTargetVol, uint32_t dwDurationMs), 0x6B80)
// 1.12a @10041: 0x6820
D2FUNC_DLL(D2SOUND, SmpSetLooping, void, __fastcall, (D2SoundSmpStrc* pSmp, BOOL bLooping), 0x6820)
// 1.12a @10042: 0x6670
D2FUNC_DLL(D2SOUND, SmpNoop2, void, __fastcall, (), 0x6670)
// 1.12a @10043: 0x6EC0
D2FUNC_DLL(D2SOUND, SmpIsBufferPlaying, BOOL, __fastcall, (D2SoundSmpStrc* pSmp), 0x6EC0)
// 1.12a @10044: 0x7510
D2FUNC_DLL(D2SOUND, SmpFadeMasterVolume, void, __fastcall, (int32_t nDurationMs), 0x7510)
// 1.12a @10045: 0x92A0
D2FUNC_DLL(D2SOUND, UtilMillibelsToLinear, float, __fastcall, (float fMillibels), 0x92A0)
// 1.12a @10046: 0x8410
D2FUNC_DLL(D2SOUND, GetDirectSound, IDirectSound*, __fastcall, (), 0x8410)
// 1.12a @10047: 0x9170
D2FUNC_DLL(D2SOUND, ListContains, BOOL, __fastcall, (D2SoundListNodeStrc* pHead, void* pItem), 0x9170)
// 1.12a @10048: 0x98F0
D2FUNC_DLL(D2SOUND, StopMusic, void, __fastcall, (), 0x98F0)
// 1.12a @10049: 0x7740
D2FUNC_DLL(D2SOUND, SmpCreate, D2SoundSmpStrc*, __fastcall, (uint32_t dwFlags), 0x7740)
// 1.12a @10050: 0x9740
D2FUNC_DLL(D2SOUND, IsMusicPlaying, BOOL, __fastcall, (), 0x9740)
// 1.10f @10066: 0x2990 | 1.12a @10051: 0x7D40
D2FUNC_DLL(D2SOUND, FastIsInitialized, int32_t, __fastcall, (), 0x2990)
// 1.12a @10052: 0x83F0
D2FUNC_DLL(D2SOUND, IsEaxEnabled, BOOL, __fastcall, (), 0x83F0)
// 1.12a @10053: 0x7D50
D2FUNC_DLL(D2SOUND, FastStopAll, void, __fastcall, (), 0x7D50)
// 1.12a @10054: 0x6D80
D2FUNC_DLL(D2SOUND, SmpSetPan, void, __fastcall, (D2SoundSmpStrc* pSmp, int32_t nPan), 0x6D80)
// 1.12a @10055: 0x7BF0
D2FUNC_DLL(D2SOUND, SetSoundMixer, int32_t, __fastcall, (int32_t nMixer), 0x7BF0)
// 1.10f @10039: 0x2800 | 1.12a @10056: 0x7F30
D2FUNC_DLL(D2SOUND, FastPlaySound, void, __fastcall, (const char* szFile), 0x2800)
// 1.12a @10057: 0x9160
D2FUNC_DLL(D2SOUND, ListCount, int32_t, __fastcall, (D2SoundListNodeStrc* pHead), 0x9160)
// 1.12a @10058: 0x7580
D2FUNC_DLL(D2SOUND, SmpLoadWave, BOOL, __fastcall, (D2SoundSmpStrc* pSmp, const uint8_t* pWaveBuffer, uint32_t dwBufferSize), 0x7580)
// 1.12a @10059: 0x7DD0
D2FUNC_DLL(D2SOUND, FastIsAnyPlaying, BOOL, __fastcall, (), 0x7DD0)
// 1.12a @10060: 0x8400
D2FUNC_DLL(D2SOUND, Is3DSoundEnabled, BOOL, __fastcall, (), 0x8400)
// 1.12a @10061: 0x66F0
D2FUNC_DLL(D2SOUND, SmpOpenStream, BOOL, __fastcall, (D2SoundSmpStrc* pSmp, const char* szFile, int32_t nVolumeScale, BOOL bLoop), 0x66F0)
// 1.12a @10062: 0x67F0
D2FUNC_DLL(D2SOUND, SmpSetLoopStart, void, __fastcall, (D2SoundSmpStrc* pSmp, uint32_t dwLoopStart), 0x67F0)
// 1.12a @10063: 0x99B0
D2FUNC_DLL(D2SOUND, PlayNextMusicTrack, BOOL, __fastcall, (), 0x99B0)
// 1.12a @10064: 0x9190
D2FUNC_DLL(D2SOUND, ListRemove, void, __fastcall, (D2SoundListNodeStrc** ppHead, void* pItem), 0x9190)
// 1.12a @10065: 0x7BC0
D2FUNC_DLL(D2SOUND, SetMasterVolume, int32_t, __fastcall, (int32_t nVolume), 0x7BC0)
// 1.12a @10066: 0x9920
D2FUNC_DLL(D2SOUND, PlayMusicFile, BOOL, __fastcall, (const char* szFile, BOOL bLoop), 0x9920)
// 1.12a @10067: 0x6950
D2FUNC_DLL(D2SOUND, SmpIsPlaying, BOOL, __fastcall, (D2SoundSmpStrc* pSmp), 0x6950)
// 1.12a @10068: 0x9450
D2FUNC_DLL(D2SOUND, UtilParseWaveData, const uint8_t*, __fastcall, (const uint8_t* pWaveBuffer, uint32_t* pInOutSize, WAVEFORMATEX* pOutFormat), 0x9450)
// 1.12a @10069: 0x6680
D2FUNC_DLL(D2SOUND, SmpCloseStream, void, __fastcall, (D2SoundSmpStrc* pSmp), 0x6680)
// 1.12a @10070: 0x80F0
D2FUNC_DLL(D2SOUND, FastInit, void, __fastcall, (), 0x80F0)

// Compatibility aliases for 1.10f ordinal-named call sites in D2Win
#define D2SOUND_10034 D2SOUND_EnableMusic
#define D2SOUND_10039 D2SOUND_FastPlaySound
inline void* __fastcall D2SOUND_10048() { return reinterpret_cast<void*>(static_cast<intptr_t>(D2SOUND_GetMusicVolume())); }
inline void __fastcall D2SOUND_10049(void* pVol) { D2SOUND_SetMusicVolume(static_cast<int32_t>(reinterpret_cast<intptr_t>(pVol))); }
#define D2SOUND_10065 D2SOUND_FadeMusic
#define D2SOUND_10066 D2SOUND_FastIsInitialized
#define D2SOUND_10069 D2SOUND_GetOptionsMusic
#define D2SOUND_10070 D2SOUND_SetOptionsMusic

#ifdef __cplusplus
template <typename T>
inline void __fastcall D2SOUND_ListPrepend(T** ppHead, void* pItem) { D2SOUND_ListPrepend(reinterpret_cast<D2SoundListNodeStrc**>(ppHead), pItem); }
template <typename T>
inline void __fastcall D2SOUND_ListRemove(T** ppHead, void* pItem) { D2SOUND_ListRemove(reinterpret_cast<D2SoundListNodeStrc**>(ppHead), pItem); }
template <typename T, typename CmpFn>
inline void __fastcall D2SOUND_ListSort(T** ppHead, CmpFn pfCompare) { (void)ppHead; (void)pfCompare; }
inline uint32_t __fastcall D2SOUND_SmpGetStreamPos(void* pSmp) { return D2SOUND_SmpGetStreamPos(static_cast<D2SoundSmpStrc*>(pSmp)); }
inline void __fastcall D2SOUND_SmpReleaseStream(void* pSmp) { D2SOUND_SmpDestroy(static_cast<D2SoundSmpStrc*>(pSmp)); }
inline int32_t __fastcall D2SOUND_GetMaxVoices() { return 32; }
#endif
