#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <algorithm>

#include <D2Sound.h>
#include <D2Gfx.h>
#include <Window.h>
#include <Fog.h>
#include <Storm.h>
#include <Archive.h>

// EAX 1.0 / 2.0 Property Set GUIDs from 1.12a .rdata (0x6F9C01B4..0x6F9C0208)
static const GUID kIID_IKsPropertySet =
	{ 0x31EFAC30, 0x515C, 0x11D0, { 0xA9, 0xAA, 0x00, 0xAA, 0x00, 0x61, 0xBE, 0x93 } };
static const GUID kIID_IDirectSound3DBuffer =
	{ 0x279AFA86, 0x4981, 0x11CE, { 0xA5, 0x21, 0x00, 0x20, 0xAF, 0x0B, 0xE5, 0x60 } };
static const GUID kDSPROPSETID_EAX10_ListenerProperties =
	{ 0x4A4E6FC1, 0xC341, 0x11D1, { 0xB7, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };
static const GUID kDSPROPSETID_EAX10_BufferProperties =
	{ 0x4A4E6FC0, 0xC341, 0x11D1, { 0xB7, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };
static const GUID kDSPROPSETID_EAX20_ListenerProperties =
	{ 0x0306A6A8, 0xB224, 0x11D2, { 0x99, 0xE5, 0x00, 0x00, 0xE8, 0xD8, 0xC7, 0x22 } };
static const GUID kDSPROPSETID_EAX20_BufferProperties =
	{ 0x0306A6A7, 0xB224, 0x11D2, { 0x99, 0xE5, 0x00, 0x00, 0xE8, 0xD8, 0xC7, 0x22 } };

// Classic & Expansion Music Track Tables (0x6F9C58F0..0x6F9C5978)
static D2SoundMusicTrackStrc gClassicMusicTracks[8] = {
	{ "data\\global\\music\\common\\options.wav", FALSE },
	{ "data\\global\\music\\act1\\caves.wav", FALSE },
	{ "data\\global\\music\\act1\\monastery.wav", FALSE },
	{ "data\\global\\music\\act1\\crypt.wav", FALSE },
	{ "data\\global\\music\\act2\\harem.wav", FALSE },
	{ "data\\global\\music\\act2\\tombs.wav", FALSE },
	{ "data\\global\\music\\act3\\spider.wav", FALSE },
	{ "data\\global\\music\\act3\\kurastsewer.wav", FALSE },
};
static int32_t gnClassicMusicTrackCount = 8; // 0x6F9C5930

static D2SoundMusicTrackStrc gExpansionMusicTracks[8] = {
	{ "data\\global\\music\\introedit.wav", FALSE },
	{ "data\\global\\music\\act5\\icecaves.wav", FALSE },
	{ "data\\global\\music\\act5\\xtemple.wav", FALSE },
	{ "data\\global\\music\\act2\\desert.wav", FALSE },
	{ "data\\global\\music\\act2\\sewer.wav", FALSE },
	{ "data\\global\\music\\act3\\kurast.wav", FALSE },
	{ "data\\global\\music\\act3\\kurastsewer.wav", FALSE },
	{ "data\\global\\music\\act4\\diablo.wav", FALSE },
};
static int32_t gnExpansionMusicTrackCount = 8; // 0x6F9C5978

// Sound System Globals (0x6F9C597C..0x6F9C65D0)
static BOOL gbReuseBuffers = TRUE;                        // 0x6F9C597C
static int32_t gnCurrentMasterVolume = 0xFF;              // 0x6F9C5980
static BOOL gbMusicTracksUseExpansion = FALSE;            // 0x6F9C649C
static BOOL gbMusicTracksInitialized = FALSE;             // 0x6F9C64A0
static D2SoundSmpStrc* gpMusicSample = nullptr;           // 0x6F9C64A8
static IKsPropertySet* gpListenerPropertySet = nullptr;   // 0x6F9C64AC
static DSCAPS gDSCaps = {};                               // 0x6F9C64B0
static IDirectSound3DBuffer* gpListener3DBuffer = nullptr;// 0x6F9C6510
static IDirectSoundBuffer* gpListenerDSBuffer = nullptr;  // 0x6F9C6514
static IDirectSoundBuffer* gpPrimaryDSBuffer = nullptr;   // 0x6F9C6518
static CRITICAL_SECTION gcsSound = {};                    // 0x6F9C651C
static CRITICAL_SECTION gcsFastSound = {};                // 0x6F9C6534
static D2SoundSmpStrc* gpFastChannels[8] = {};            // 0x6F9C654C
static int32_t gnSoundMixer = 0;                          // 0x6F9C656C
static int32_t gnMasterVolume = 100;                      // 0x6F9C6570
static int32_t gnMusicVolume = 50;                        // 0x6F9C6574
static int32_t gnPositionalBias = 50;                     // 0x6F9C6578
static int32_t gnNPCSpeech = 2;                           // 0x6F9C657C
static int32_t gbOptionsMusic = 1;                        // 0x6F9C6580
static BOOL gbMusicEnabled = FALSE;                       // 0x6F9C6584
static BOOL gbPlayFirstMusicTrack = FALSE;                // 0x6F9C6588
static BOOL gbSoundSystemOpened = FALSE;                  // 0x6F9C658C
static IDirectSound* gpDirectSound = nullptr;             // 0x6F9C6590
static BOOL gb3DSoundEnabled = FALSE;                     // 0x6F9C6594
static BOOL gbEaxEnabled = FALSE;                         // 0x6F9C6598
static int32_t gnEaxVersion = 0;                          // 0x6F9C659C
static BOOL gbEaxEnvironmentOff = FALSE;                  // 0x6F9C65A0
static BOOL gbSoundCSInitialized = FALSE;                 // 0x6F9C65A4
static BOOL gbDriverEnumerated = FALSE;                   // 0x6F9C65A8
static BOOL gbDisable3DSoundDueToDriver = FALSE;          // 0x6F9C65AC
static BOOL gbHasEmu10k1Vxd = FALSE;                      // 0x6F9C65B0
static BOOL gbHasCrystalOrEs197xVxd = FALSE;              // 0x6F9C65B4
static BOOL gbIsExpansion = FALSE;                        // 0x6F9C65B8
static BOOL gbFastInitialized = FALSE;                    // 0x6F9C65BC
static D2SoundFastEntryStrc* gpFastSoundList = nullptr;   // 0x6F9C65C0
static D2SoundListNodeStrc* gpSampleListHead = nullptr;   // 0x6F9C65C4
static HANDLE ghMixerEvent = nullptr;                     // 0x6F9C65C8
static HANDLE ghMixerThread = nullptr;                    // 0x6F9C65CC
static BOOL gbWindowIconic = FALSE;                       // 0x6F9C65D0

// Forward declarations of internal helpers
static void D2SOUND_SmpReleaseInterfacesAndZero(D2SoundSmpStrc* pSmp);
static BOOL D2SOUND_SmpFillBufferWithSilence(IDirectSoundBuffer* pDSBuffer, uint32_t dwOffset, uint32_t dwBytes);
static void D2SOUND_StopMixerThread();
static BOOL D2SOUND_SmpCreateBuffers(D2SoundSmpStrc* pSmp, uint32_t dwFlags);
static BOOL D2SOUND_SmpRestoreIfLost(D2SoundSmpStrc* pSmp);
static BOOL D2SOUND_SmpWriteWaveChunk(D2SoundSmpStrc* pSmp, uint32_t dwWaveOffset, uint32_t dwBufferOffset, uint32_t dwBytesToWrite);
static DWORD WINAPI D2SOUND_MixerThreadProc(LPVOID lpParameter);
static void D2SOUND_StartMixerThread();
static const char* D2SOUND_GetDirectSoundErrorString(HRESULT hr);
static BOOL D2SOUND_InitEaxListenerRoomOff(int32_t nEaxVer);
static void D2SOUND_ReleaseListenerInterfaces();
static BOOL CALLBACK D2SOUND_DSEnumCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext);
static void D2SOUND_InitDirectSoundAttempt();
static void D2SOUND_InitWaveFormat(WAVEFORMATEX* pFormat, uint16_t nChannels);
static const uint8_t* D2SOUND_FindRiffChunk(const uint8_t* pData, uint32_t* pInOutSize, uint32_t dwFourCC, uint32_t* pOutChunkSize);
static BOOL D2SOUND_GetVxdFileVersion(const char* szFilePath, uint32_t* pOutMajor, uint32_t* pOutMinor, uint32_t* pOutBuild);
static void D2SOUND_ResetMusicTracksPlayed();
static void D2SOUND_InitMusicSystem();

// 1.12a: 0x6F9B6660 (@10033)
void __fastcall D2SOUND_SmpNoop1()
{
}

// 1.12a: 0x6F9B6670 (@10042)
void __fastcall D2SOUND_SmpNoop2()
{
}

// 1.12a: 0x6F9B6680 (@10069)
void __fastcall D2SOUND_SmpCloseStream(D2SoundSmpStrc* pSmp)
{
	D2_ASSERT(pSmp != nullptr && pSmp->pDSBuffer != nullptr);
	EnterCriticalSection(&gcsSound);
	if (pSmp->hFile != nullptr)
	{
		SFileDdaEnd(pSmp->hFile);
		pSmp->pDSBuffer->Stop();
		FOG_FCloseFile(pSmp->hFile);
		pSmp->hFile = nullptr;
		pSmp->bPlaying = FALSE;
	}
	LeaveCriticalSection(&gcsSound);
}

// 1.12a: 0x6F9B66F0 (@10061)
BOOL __fastcall D2SOUND_SmpOpenStream(D2SoundSmpStrc* pSmp, const char* szFile, int32_t nVolumeScale, BOOL bLoop)
{
	D2_ASSERT(pSmp != nullptr);
	HSFILE hFile = nullptr;
	if (!FOG_FOpenFile(szFile, &hFile))
	{
		return FALSE;
	}

	EnterCriticalSection(&gcsSound);
	const uint32_t dwDdaFlags = bLoop ? 0x40000u : 0u;
	if (!SFileDdaBeginEx(
		hFile,
		static_cast<signed int>(pSmp->dwBufferSize),
		dwDdaFlags,
		nVolumeScale * 4,
		0x7FFFFFFF,
		0x7FFFFFFF,
		reinterpret_cast<int>(pSmp->pDSBuffer)))
	{
		FOG_FCloseFile(hFile);
		LeaveCriticalSection(&gcsSound);
		return FALSE;
	}

	pSmp->hFile = hFile;
	pSmp->bPlaying = TRUE;
	pSmp->dwSingleShotEndOffset = 0xFFFFFFFFu;
	LeaveCriticalSection(&gcsSound);
	return TRUE;
}

// 1.12a: 0x6F9B67C0 (@10000)
BOOL __fastcall D2SOUND_SmpHasStream(D2SoundSmpStrc* pSmp)
{
	D2_ASSERT(pSmp != nullptr);
	return pSmp->hFile != nullptr;
}

// 1.12a: 0x6F9B67F0 (@10062)
void __fastcall D2SOUND_SmpSetLoopStart(D2SoundSmpStrc* pSmp, uint32_t dwLoopStart)
{
	D2_ASSERT(pSmp != nullptr);
	pSmp->dwLoopStart = dwLoopStart;
}

// 1.12a: 0x6F9B6820 (@10041)
void __fastcall D2SOUND_SmpSetLooping(D2SoundSmpStrc* pSmp, BOOL bLooping)
{
	D2_ASSERT(pSmp != nullptr);
	pSmp->bLooping = bLooping;
}

// 1.12a: 0x6F9B6850 (@10012)
uint32_t __fastcall D2SOUND_SmpGetStreamPos(D2SoundSmpStrc* pSmp)
{
	D2_ASSERT(pSmp != nullptr);
	EnterCriticalSection(&gcsSound);
	if (pSmp->hFile != nullptr)
	{
		uint32_t dwPos = 0;
		uint32_t dwMaxPos = 0;
		if (SFileDdaGetPos(reinterpret_cast<int>(pSmp->hFile), reinterpret_cast<int>(&dwPos), reinterpret_cast<int>(&dwMaxPos)))
		{
			LeaveCriticalSection(&gcsSound);
			return dwPos >> 2;
		}
	}
	LeaveCriticalSection(&gcsSound);
	return 0;
}

// 1.12a: 0x6F9B68D0 (@10036)
void __fastcall D2SOUND_SmpSetPosition3D(D2SoundSmpStrc* pSmp, float fX, float fY, float fZ)
{
	D2_ASSERT(pSmp != nullptr && pSmp->pDSBuffer != nullptr);
	if ((pSmp->dwFlags & D2SOUNDSMP_FLAG_3D) != 0 && pSmp->p3DBuffer != nullptr)
	{
		pSmp->p3DBuffer->SetPosition(fX, fY, fZ, DS3D_IMMEDIATE);
	}
}

// 1.12a: 0x6F9B6920 (@10009)
int32_t __fastcall D2SOUND_SmpGetVolume(D2SoundSmpStrc* pSmp)
{
	D2_ASSERT(pSmp != nullptr);
	return pSmp->nVolume;
}

// 1.12a: 0x6F9B6950 (@10067)
BOOL __fastcall D2SOUND_SmpIsPlaying(D2SoundSmpStrc* pSmp)
{
	D2_ASSERT(pSmp != nullptr && pSmp->pDSBuffer != nullptr);
	return pSmp->bPlaying;
}

// 1.12a: 0x6F9B6980 (@10018)
void __fastcall D2SOUND_SmpStop(D2SoundSmpStrc* pSmp)
{
	D2_ASSERT(pSmp != nullptr && pSmp->pDSBuffer != nullptr);
	EnterCriticalSection(&gcsSound);
	if (pSmp->hFile != nullptr)
	{
		D2SOUND_SmpCloseStream(pSmp);
	}
	pSmp->pDSBuffer->Stop();
	pSmp->bPlaying = FALSE;
	LeaveCriticalSection(&gcsSound);
}

// 1.12a: 0x6F9B69F0 (Internal)
static void D2SOUND_SmpReleaseInterfacesAndZero(D2SoundSmpStrc* pSmp)
{
	D2SOUND_SmpStop(pSmp);
	if (pSmp->pPropertySet != nullptr)
	{
		pSmp->pPropertySet->Release();
		pSmp->pPropertySet = nullptr;
	}
	if (pSmp->p3DBuffer != nullptr)
	{
		pSmp->p3DBuffer->Release();
		pSmp->p3DBuffer = nullptr;
	}
	if (pSmp->pDSBuffer != nullptr)
	{
		pSmp->pDSBuffer->Release();
		pSmp->pDSBuffer = nullptr;
	}
	memset(pSmp, 0, sizeof(D2SoundSmpStrc));
}

// 1.12a: 0x6F9B6A40 (Internal)
static BOOL D2SOUND_SmpFillBufferWithSilence(IDirectSoundBuffer* pDSBuffer, uint32_t dwOffset, uint32_t dwBytes)
{
	D2_ASSERT(pDSBuffer != nullptr);
	EnterCriticalSection(&gcsSound);
	LPVOID pvAudioPtr1 = nullptr;
	DWORD dwAudioBytes1 = 0;
	HRESULT hr = pDSBuffer->Lock(dwOffset, dwBytes, &pvAudioPtr1, &dwAudioBytes1, nullptr, nullptr, 0);
	if (FAILED(hr))
	{
		LeaveCriticalSection(&gcsSound);
		return FALSE;
	}
	memset(pvAudioPtr1, 0, dwAudioBytes1);
	pDSBuffer->Unlock(pvAudioPtr1, dwAudioBytes1, nullptr, 0);
	LeaveCriticalSection(&gcsSound);
	return TRUE;
}

// 1.12a: 0x6F9B6AF0 (Internal)
static void D2SOUND_StopMixerThread()
{
	if (ghMixerThread != nullptr && ghMixerEvent != nullptr)
	{
		SetEvent(ghMixerEvent);
		WaitForSingleObject(ghMixerThread, INFINITE);
		CloseHandle(ghMixerEvent);
		CloseHandle(ghMixerThread);
		ghMixerEvent = nullptr;
		ghMixerThread = nullptr;
	}
}

// 1.12a: 0x6F9B6B60 (@10014)
int32_t __fastcall D2SOUND_SmpGetMasterVolume()
{
	return gpDirectSound != nullptr ? gnCurrentMasterVolume : 0;
}

// 1.12a: 0x6F9B6B80 (@10040)
void __fastcall D2SOUND_SmpFade(D2SoundSmpStrc* pSmp, int32_t nTargetVol, uint32_t dwDurationMs)
{
	D2_ASSERT(pSmp != nullptr);
	if (gpDirectSound != nullptr)
	{
		EnterCriticalSection(&gcsSound);
		pSmp->bFading = TRUE;
		pSmp->nFadeStartVol = pSmp->nVolume;
		pSmp->nFadeEndVol = nTargetVol;
		const uint32_t dwNow = GetTickCount();
		pSmp->dwFadeStartTick = dwNow;
		pSmp->dwFadeEndTick = dwNow + dwDurationMs;
		LeaveCriticalSection(&gcsSound);
	}
}

// 1.12a: 0x6F9B6C00 (@10022)
void __fastcall D2SOUND_SmpSetReverbMix(D2SoundSmpStrc* pSmp, float fReverbMix)
{
	D2_ASSERT(pSmp != nullptr);
	if (gbEaxEnvironmentOff)
	{
		fReverbMix = 0.0f;
	}
	if (pSmp->pPropertySet == nullptr)
	{
		return;
	}
	if (gnEaxVersion == 1)
	{
		if (fReverbMix < 0.0f)
		{
			fReverbMix = -1.0f;
		}
		pSmp->pPropertySet->Set(kDSPROPSETID_EAX10_BufferProperties, 1, nullptr, 0, &fReverbMix, sizeof(float));
	}
	else
	{
		int32_t lMillibels = static_cast<int32_t>(D2SOUND_UtilRatioToMillibels(0.0f, 1.0f, fReverbMix));
		pSmp->pPropertySet->Set(kDSPROPSETID_EAX20_BufferProperties, 5, nullptr, 0, &lMillibels, sizeof(int32_t));
		pSmp->pPropertySet->Set(kDSPROPSETID_EAX20_BufferProperties, 4, nullptr, 0, &lMillibels, sizeof(int32_t));
	}
}

// 1.12a: 0x6F9B6CE0 (@10034)
void __fastcall D2SOUND_SmpSetOcclusion(D2SoundSmpStrc* pSmp, float fOcclusion)
{
	D2_ASSERT(pSmp != nullptr);
	pSmp->fOcclusion = fOcclusion;
	if (pSmp->pPropertySet != nullptr && gnEaxVersion == 2)
	{
		if ((pSmp->dwFlags & D2SOUNDSMP_FLAG_NOOCCLUSION) != 0 || fOcclusion >= 0.0f)
		{
			int32_t lOcclusion = static_cast<int32_t>(D2SOUND_UtilRatioToMillibels(0.0f, 1.0f, 1.0f - fOcclusion));
			pSmp->pPropertySet->Set(kDSPROPSETID_EAX20_BufferProperties, 7, nullptr, 0, &lOcclusion, sizeof(int32_t));
		}
	}
}

// 1.12a: 0x6F9B6D80 (@10054)
void __fastcall D2SOUND_SmpSetPan(D2SoundSmpStrc* pSmp, int32_t nPan)
{
	D2_ASSERT(pSmp != nullptr);
	LONG lPanMillibels = 0;
	if (nPan > 128)
	{
		lPanMillibels = static_cast<LONG>(-D2SOUND_UtilRatioToMillibels(0.0f, 127.0f, static_cast<float>(255 - nPan)));
	}
	else if (nPan < 128)
	{
		lPanMillibels = static_cast<LONG>(D2SOUND_UtilRatioToMillibels(0.0f, 127.0f, static_cast<float>(nPan)));
	}
	pSmp->pDSBuffer->SetPan(lPanMillibels);
}

// 1.12a: 0x6F9B6E20 (@10008)
void __fastcall D2SOUND_SmpSetVolume(D2SoundSmpStrc* pSmp, int32_t nVolume)
{
	D2_ASSERT(pSmp != nullptr && pSmp->pDSBuffer != nullptr);
	pSmp->nVolume = nVolume;
	int32_t nScaled = (gnCurrentMasterVolume * nVolume) / 255;
	if ((pSmp->dwFlags & D2SOUNDSMP_FLAG_NOOCCLUSION) == 0 || gnEaxVersion != 2)
	{
		nScaled = static_cast<int32_t>((1.0 - static_cast<double>(pSmp->fOcclusion)) * static_cast<double>(nScaled));
	}
	LONG lVolMillibels = static_cast<LONG>(D2SOUND_UtilRatioToMillibels(0.0f, 255.0f, static_cast<float>(nScaled)));
	pSmp->pDSBuffer->SetVolume(lVolMillibels);
}

// 1.12a: 0x6F9B6EC0 (@10043)
BOOL __fastcall D2SOUND_SmpIsBufferPlaying(D2SoundSmpStrc* pSmp)
{
	D2_ASSERT(pSmp != nullptr && pSmp->pDSBuffer != nullptr);
	EnterCriticalSection(&gcsSound);
	DWORD dwStatus = 0;
	HRESULT hr = pSmp->pDSBuffer->GetStatus(&dwStatus);
	LeaveCriticalSection(&gcsSound);
	if (FAILED(hr))
	{
		return FALSE;
	}
	return (dwStatus & DSBSTATUS_PLAYING) != 0;
}

// 1.12a: 0x6F9B6F30 (@10027)
BOOL __fastcall D2SOUND_SmpPlay(D2SoundSmpStrc* pSmp)
{
	D2_ASSERT(pSmp != nullptr);
	EnterCriticalSection(&gcsSound);
	HRESULT hr = pSmp->pDSBuffer->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(hr))
	{
		LeaveCriticalSection(&gcsSound);
		return FALSE;
	}
	pSmp->bPlaying = TRUE;
	LeaveCriticalSection(&gcsSound);
	return TRUE;
}

// 1.12a: 0x6F9B6FA0 (@10016)
void __fastcall D2SOUND_SmpDestroy(D2SoundSmpStrc* pSmp)
{
	D2_ASSERT(pSmp != nullptr);
	EnterCriticalSection(&gcsSound);
	D2SOUND_SmpReleaseInterfacesAndZero(pSmp);
	D2SOUND_ListRemove(&gpSampleListHead, pSmp);
	LeaveCriticalSection(&gcsSound);
	D2_FREE(pSmp);
}

// 1.12a: 0x6F9B7090 (Internal)
static BOOL D2SOUND_SmpCreateBuffers(D2SoundSmpStrc* pSmp, uint32_t dwFlags)
{
	WAVEFORMATEX wfx = {};
	const uint16_t nChannels = (dwFlags & D2SOUNDSMP_FLAG_STEREO) ? 2 : 1;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = nChannels;
	wfx.nSamplesPerSec = 22050;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = nChannels * 2;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	const uint32_t dwBufferSize = (dwFlags & D2SOUNDSMP_FLAG_STEREO) ? 0x40000u : 0x20000u;
	const BOOL bIs3D = (dwFlags & D2SOUNDSMP_FLAG_3D) != 0;

	DSBUFFERDESC dsbd = {};
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = bIs3D ? (DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2)
	                     : (DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2);
	dsbd.dwBufferBytes = dwBufferSize;
	dsbd.lpwfxFormat = &wfx;

	if (gpDirectSound == nullptr || FAILED(gpDirectSound->CreateSoundBuffer(&dsbd, &pSmp->pDSBuffer, nullptr)))
	{
		return FALSE;
	}

	if (bIs3D)
	{
		if (FAILED(pSmp->pDSBuffer->QueryInterface(kIID_IDirectSound3DBuffer, reinterpret_cast<LPVOID*>(&pSmp->p3DBuffer))))
		{
			pSmp->pDSBuffer->Release();
			pSmp->pDSBuffer = nullptr;
			return FALSE;
		}

		if (gbEaxEnabled)
		{
			if (FAILED(pSmp->p3DBuffer->QueryInterface(kIID_IKsPropertySet, reinterpret_cast<LPVOID*>(&pSmp->pPropertySet))))
			{
				pSmp->p3DBuffer->Release();
				pSmp->p3DBuffer = nullptr;
				pSmp->pDSBuffer->Release();
				pSmp->pDSBuffer = nullptr;
				return FALSE;
			}

			ULONG ulSupport = 0;
			const GUID& propGuid = (gnEaxVersion == 1) ? kDSPROPSETID_EAX10_BufferProperties : kDSPROPSETID_EAX20_BufferProperties;
			HRESULT hr = pSmp->pPropertySet->QuerySupport(propGuid, 1, &ulSupport);
			if (FAILED(hr) || (ulSupport & 3) != 3)
			{
				pSmp->pPropertySet->Release();
				pSmp->pPropertySet = nullptr;
				pSmp->p3DBuffer->Release();
				pSmp->p3DBuffer = nullptr;
				pSmp->pDSBuffer->Release();
				pSmp->pDSBuffer = nullptr;
				return FALSE;
			}
		}
	}

	pSmp->dwBufferSize = dwBufferSize;
	pSmp->dwHalfBufferSize = dwBufferSize >> 1;
	pSmp->dwFlags = dwFlags;
	pSmp->dwSingleShotEndOffset = 0xFFFFFFFFu;
	return TRUE;
}

// 1.12a: 0x6F9B7230 (Internal)
static BOOL D2SOUND_SmpRestoreIfLost(D2SoundSmpStrc* pSmp)
{
	D2_ASSERT(pSmp->pDSBuffer != nullptr);
	EnterCriticalSection(&gcsSound);
	DWORD dwStatus = 0;
	if (FAILED(pSmp->pDSBuffer->GetStatus(&dwStatus)))
	{
		LeaveCriticalSection(&gcsSound);
		return FALSE;
	}

	if ((dwStatus & DSBSTATUS_BUFFERLOST) != 0)
	{
		for (;;)
		{
			HRESULT hr = pSmp->pDSBuffer->Restore();
			if (hr == DSERR_BUFFERLOST)
			{
				Sleep(10);
				continue;
			}
			if (SUCCEEDED(hr))
			{
				break;
			}
		}
		D2SOUND_SmpStop(pSmp);
	}
	LeaveCriticalSection(&gcsSound);
	return TRUE;
}

// 1.12a: 0x6F9B72D0 (Internal)
static BOOL D2SOUND_SmpWriteWaveChunk(D2SoundSmpStrc* pSmp, uint32_t dwWaveOffset, uint32_t dwBufferOffset, uint32_t dwBytesToWrite)
{
	D2_ASSERT(pSmp != nullptr && pSmp->pDSBuffer != nullptr && pSmp->pWaveData != nullptr && dwBytesToWrite != 0);
	EnterCriticalSection(&gcsSound);

	LPVOID pvAudioPtr1 = nullptr;
	DWORD dwAudioBytes1 = 0;
	if (FAILED(pSmp->pDSBuffer->Lock(dwBufferOffset, dwBytesToWrite, &pvAudioPtr1, &dwAudioBytes1, nullptr, nullptr, 0)))
	{
		LeaveCriticalSection(&gcsSound);
		return FALSE;
	}

	uint8_t* pDst = static_cast<uint8_t*>(pvAudioPtr1);
	uint32_t dwRemaining = dwBytesToWrite;
	uint32_t dwCurBufPos = dwBufferOffset;

	for (;;)
	{
		if (dwWaveOffset >= pSmp->dwWaveSize)
		{
			const uint32_t dwLoopLen = pSmp->dwWaveSize - pSmp->dwLoopStart;
			dwWaveOffset = pSmp->dwLoopStart + ((dwWaveOffset - pSmp->dwLoopStart) % dwLoopLen);
		}

		uint32_t dwChunk = pSmp->dwWaveSize - dwWaveOffset;
		if (dwRemaining < dwChunk)
		{
			dwChunk = dwRemaining;
		}

		memcpy(pDst, pSmp->pWaveData + dwWaveOffset, dwChunk);
		pSmp->dwWriteOffsetTotal += dwChunk;
		pDst += dwChunk;
		dwRemaining -= dwChunk;
		dwCurBufPos += dwChunk;
		dwWaveOffset += dwChunk;

		if (pSmp->bLooping)
		{
			if (dwRemaining != 0)
			{
				continue;
			}
			break;
		}
		else
		{
			if (dwRemaining != 0)
			{
				if (pSmp->dwSingleShotEndOffset != 0xFFFFFFFFu)
				{
					if (dwCurBufPos >= pSmp->dwSingleShotEndOffset)
					{
						break;
					}
					dwRemaining = pSmp->dwSingleShotEndOffset - dwCurBufPos;
				}
				if (dwRemaining != 0)
				{
					memset(pDst, 0, dwRemaining);
				}
			}
			break;
		}
	}

	pSmp->pDSBuffer->Unlock(pvAudioPtr1, dwAudioBytes1, nullptr, 0);
	if (!pSmp->bLooping && pSmp->dwWriteOffsetTotal >= pSmp->dwWaveSize)
	{
		pSmp->bEndOfDataReached = TRUE;
	}

	LeaveCriticalSection(&gcsSound);
	return TRUE;
}

// 1.12a: 0x6F9B7480 (@10005)
void __fastcall D2SOUND_SmpSetMasterVolume(int32_t nVolume)
{
	if (gpDirectSound != nullptr && nVolume != gnCurrentMasterVolume)
	{
		D2_ASSERT(nVolume >= 0 && nVolume <= 255);
		EnterCriticalSection(&gcsSound);
		gnCurrentMasterVolume = nVolume;
		for (D2SoundListNodeStrc* pNode = gpSampleListHead; pNode != nullptr; pNode = pNode->pNext)
		{
			D2SoundSmpStrc* pSmp = static_cast<D2SoundSmpStrc*>(pNode->pItem);
			D2_ASSERT(pSmp != nullptr);
			D2SOUND_SmpSetVolume(pSmp, pSmp->nVolume);
		}
		LeaveCriticalSection(&gcsSound);
	}
}

// 1.12a: 0x6F9B7510 (@10044)
void __fastcall D2SOUND_SmpFadeMasterVolume(int32_t nDurationMs)
{
	if (gpDirectSound == nullptr)
	{
		return;
	}
	D2_ASSERT(nDurationMs > 0);
	int32_t nVol = gnCurrentMasterVolume;
	const uint32_t dwSteps = std::max<uint32_t>(1u, static_cast<uint32_t>(nDurationMs) / 20u);
	const int32_t nStepDelta = static_cast<int32_t>(static_cast<uint32_t>(nVol) / dwSteps);
	while (nVol != 0)
	{
		Sleep(20);
		if (static_cast<uint32_t>(nVol) > static_cast<uint32_t>(nStepDelta) && nStepDelta > 0)
		{
			nVol -= nStepDelta;
		}
		else
		{
			nVol = 0;
		}
		D2SOUND_SmpSetMasterVolume(nVol);
	}
}

// 1.12a: 0x6F9B7580 (@10058)
BOOL __fastcall D2SOUND_SmpLoadWave(D2SoundSmpStrc* pSmp, const uint8_t* pWaveBuffer, uint32_t dwBufferSize)
{
	D2_ASSERT(pSmp != nullptr && pSmp->pDSBuffer != nullptr && pWaveBuffer != nullptr && dwBufferSize != 0);
	EnterCriticalSection(&gcsSound);
	WAVEFORMATEX wfx = {};
	uint32_t dwWaveDataSize = dwBufferSize;
	const uint8_t* pWaveData = D2SOUND_UtilParseWaveData(pWaveBuffer, &dwWaveDataSize, &wfx);
	D2_ASSERT(pSmp->dwLoopStart < dwWaveDataSize);

	pSmp->pWaveData = pWaveData;
	pSmp->dwWaveSize = dwWaveDataSize;
	D2SOUND_SmpWriteWaveChunk(pSmp, 0, 0, pSmp->dwBufferSize);
	pSmp->nNextHalfToFill = 1;
	if (!pSmp->bLooping && pSmp->dwWaveSize < pSmp->dwBufferSize)
	{
		pSmp->dwSingleShotEndOffset = pSmp->dwWaveSize;
	}
	else
	{
		pSmp->dwSingleShotEndOffset = 0xFFFFFFFFu;
	}
	LeaveCriticalSection(&gcsSound);
	return TRUE;
}

// 1.12a: 0x6F9B7650 (@10019)
void __fastcall D2SOUND_SmpReset(D2SoundSmpStrc* pSmp)
{
	D2_ASSERT(pSmp != nullptr && pSmp->pDSBuffer != nullptr);
	D2_ASSERT(pSmp->hFile == nullptr);
	EnterCriticalSection(&gcsSound);
	if (gbReuseBuffers)
	{
		pSmp->pWaveData = nullptr;
		pSmp->dwWaveSize = 0;
		pSmp->dwPlayCursorTotal = 0;
		pSmp->dwWriteOffsetTotal = 0;
		pSmp->bEndOfDataReached = FALSE;
		pSmp->bLooping = FALSE;
		pSmp->dwLoopStart = 0;
	}
	else
	{
		const uint32_t dwFlags = pSmp->dwFlags;
		D2SOUND_SmpReleaseInterfacesAndZero(pSmp);
		D2SOUND_SmpCreateBuffers(pSmp, dwFlags);
	}

	pSmp->pDSBuffer->SetCurrentPosition(0);
	if (pSmp->p3DBuffer != nullptr)
	{
		pSmp->p3DBuffer->SetMaxDistance(100.0f, DS3D_IMMEDIATE);
		pSmp->p3DBuffer->SetMinDistance(2.0f, DS3D_IMMEDIATE);
	}
	if ((pSmp->dwFlags & D2SOUNDSMP_FLAG_3D) != 0)
	{
		D2SOUND_SmpSetPosition3D(pSmp, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		pSmp->pDSBuffer->SetPan(0);
	}
	D2SOUND_SmpSetOcclusion(pSmp, 0.0f);
	D2SOUND_SmpSetVolume(pSmp, 255);
	LeaveCriticalSection(&gcsSound);
}

// 1.12a: 0x6F9B7740 (@10049)
D2SoundSmpStrc* __fastcall D2SOUND_SmpCreate(uint32_t dwFlags)
{
	if ((dwFlags & D2SOUNDSMP_FLAG_3D) != 0)
	{
		if (!gb3DSoundEnabled || (dwFlags & D2SOUNDSMP_FLAG_STEREO) != 0)
		{
			return nullptr;
		}
	}

	D2SoundSmpStrc* pSmp = D2_CALLOC_STRC(D2SoundSmpStrc);
	if (!D2SOUND_SmpCreateBuffers(pSmp, dwFlags))
	{
		D2_FREE(pSmp);
		return nullptr;
	}

	EnterCriticalSection(&gcsSound);
	D2SOUND_ListPrepend(&gpSampleListHead, pSmp);
	LeaveCriticalSection(&gcsSound);
	return pSmp;
}

// 1.12a: 0x6F9B7800 (Internal)
static DWORD WINAPI D2SOUND_MixerThreadProc(LPVOID /*lpParameter*/)
{
	while (WaitForSingleObject(ghMixerEvent, 50) == WAIT_TIMEOUT)
	{
		EnterCriticalSection(&gcsSound);
		const BOOL bPrevIconic = gbWindowIconic;
		gbWindowIconic = IsIconic(WINDOW_GetWindow());

		if (gbMusicEnabled && (gpMusicSample == nullptr || gpMusicSample->hFile == nullptr))
		{
			D2SOUND_PlayNextMusicTrack();
		}

		for (D2SoundListNodeStrc* pNode = gpSampleListHead; pNode != nullptr; pNode = pNode->pNext)
		{
			D2SoundSmpStrc* pSmp = static_cast<D2SoundSmpStrc*>(pNode->pItem);
			D2SOUND_SmpRestoreIfLost(pSmp);
			if (gbWindowIconic != bPrevIconic)
			{
				D2SOUND_SmpSetVolume(pSmp, pSmp->nVolume);
			}

			if (pSmp->bFading)
			{
				const uint32_t dwNow = GetTickCount();
				if (dwNow >= pSmp->dwFadeStartTick && dwNow <= pSmp->dwFadeEndTick && pSmp->dwFadeEndTick > pSmp->dwFadeStartTick)
				{
					const int32_t nCurVol = pSmp->nFadeStartVol +
						static_cast<int32_t>((static_cast<int64_t>(pSmp->nFadeEndVol - pSmp->nFadeStartVol) *
						                      static_cast<int64_t>(dwNow - pSmp->dwFadeStartTick)) /
						                     static_cast<int64_t>(pSmp->dwFadeEndTick - pSmp->dwFadeStartTick));
					D2SOUND_SmpSetVolume(pSmp, nCurVol);
				}
				else
				{
					D2SOUND_SmpSetVolume(pSmp, pSmp->nFadeEndVol);
					if (pSmp->nFadeEndVol == 0)
					{
						D2SOUND_SmpStop(pSmp);
					}
					pSmp->bFading = FALSE;
				}
			}

			if (pSmp->hFile != nullptr)
			{
				uint32_t dwPos = 0;
				uint32_t dwMaxPos = 0;
				if (!SFileDdaGetPos(reinterpret_cast<int>(pSmp->hFile), reinterpret_cast<int>(&dwPos), reinterpret_cast<int>(&dwMaxPos)))
				{
					D2SOUND_SmpStop(pSmp);
				}
			}
			else if (pSmp->bPlaying)
			{
				DWORD dwPlayCursor = 0;
				DWORD dwWriteCursor = 0;
				if (SUCCEEDED(pSmp->pDSBuffer->GetCurrentPosition(&dwPlayCursor, &dwWriteCursor)))
				{
					uint32_t dwTotalPlay = pSmp->dwPlayCursorTotal;
					const uint32_t dwCurMod = dwTotalPlay & (pSmp->dwBufferSize - 1);
					dwTotalPlay -= dwCurMod;
					if (dwPlayCursor < dwCurMod)
					{
						dwTotalPlay += pSmp->dwBufferSize;
					}
					dwTotalPlay += dwPlayCursor;
					pSmp->dwPlayCursorTotal = dwTotalPlay;

					if (!pSmp->bEndOfDataReached)
					{
						const uint32_t dwWriteTotal = pSmp->dwWriteOffsetTotal;
						if ((dwWriteTotal - dwTotalPlay) < pSmp->dwHalfBufferSize || dwTotalPlay > dwWriteTotal)
						{
							const uint32_t dwBufOffset = dwWriteTotal & (pSmp->dwBufferSize - 1);
							D2SOUND_SmpWriteWaveChunk(pSmp, dwWriteTotal, dwBufOffset, pSmp->dwHalfBufferSize);
							pSmp->dwSingleShotEndOffset = 0xFFFFFFFFu;
							pSmp->nNextHalfToFill = (dwBufOffset != 0) ? 1 : 0;
						}
					}
					else if (!pSmp->bLooping)
					{
						if (dwPlayCursor < pSmp->dwHalfBufferSize && pSmp->nNextHalfToFill == 0)
						{
							D2SOUND_SmpFillBufferWithSilence(pSmp->pDSBuffer, pSmp->dwHalfBufferSize, pSmp->dwHalfBufferSize);
							pSmp->nNextHalfToFill = 1;
						}
						if (dwPlayCursor >= pSmp->dwHalfBufferSize && pSmp->nNextHalfToFill == 1)
						{
							D2SOUND_SmpFillBufferWithSilence(pSmp->pDSBuffer, 0, pSmp->dwHalfBufferSize);
							pSmp->nNextHalfToFill = 0;
						}
						if (pSmp->dwPlayCursorTotal > pSmp->dwWaveSize)
						{
							pSmp->pDSBuffer->Stop();
							pSmp->bPlaying = FALSE;
						}
					}
				}
			}
		}

		LeaveCriticalSection(&gcsSound);
	}
	return 0;
}

// 1.12a: 0x6F9B7AA0 (Internal)
static void D2SOUND_StartMixerThread()
{
	D2_ASSERT(ghMixerThread == nullptr);
	ghMixerEvent = CreateEventA(nullptr, TRUE, FALSE, nullptr);
	DWORD dwThreadId = 0;
	ghMixerThread = CreateThread(nullptr, 0, D2SOUND_MixerThreadProc, nullptr, 0, &dwThreadId);
}

// 1.12a: 0x6F9B7B00 (@10013 | 1.10f @10070)
int32_t __fastcall D2SOUND_SetOptionsMusic(int32_t bOptionsMusic)
{
	SRegSaveValue("Diablo II", "Options Music", 0, static_cast<DWORD>(bOptionsMusic));
	gbOptionsMusic = bOptionsMusic;
	return bOptionsMusic;
}

// 1.12a: 0x6F9B7B20 (@10024 | 1.10f @10069)
int32_t __fastcall D2SOUND_GetOptionsMusic()
{
	return gbOptionsMusic;
}

// 1.12a: 0x6F9B7B30 (@10026)
int32_t __fastcall D2SOUND_SetNPCSpeech(int32_t nNPCSpeech)
{
	SRegSaveValue("Diablo II", "NPC Speech", 0, static_cast<DWORD>(nNPCSpeech));
	gnNPCSpeech = nNPCSpeech;
	return nNPCSpeech;
}

// 1.12a: 0x6F9B7B50 (@10001)
int32_t __fastcall D2SOUND_GetNPCSpeech()
{
	return gnNPCSpeech;
}

// 1.12a: 0x6F9B7B60 (@10038)
int32_t __fastcall D2SOUND_SetPositionalBias(int32_t nBias)
{
	SRegSaveValue("Diablo II", "Positional Bias", 0, static_cast<DWORD>(nBias));
	gnPositionalBias = nBias;
	return nBias;
}

// 1.12a: 0x6F9B7B80 (@10017)
int32_t __fastcall D2SOUND_GetPositionalBias()
{
	return gnPositionalBias;
}

// 1.12a: 0x6F9B7B90 (@10039 | 1.10f @10049)
int32_t __fastcall D2SOUND_SetMusicVolume(int32_t nVolume)
{
	SRegSaveValue("Diablo II", "Music Volume", 0, static_cast<DWORD>(nVolume));
	gnMusicVolume = nVolume;
	return nVolume;
}

// 1.12a: 0x6F9B7BB0 (@10035 | 1.10f @10048)
int32_t __fastcall D2SOUND_GetMusicVolume()
{
	return gnMusicVolume;
}

// 1.12a: 0x6F9B7BC0 (@10065)
int32_t __fastcall D2SOUND_SetMasterVolume(int32_t nVolume)
{
	SRegSaveValue("Diablo II", "Master Volume", 0, static_cast<DWORD>(nVolume));
	gnMasterVolume = nVolume;
	return nVolume;
}

// 1.12a: 0x6F9B7BE0 (@10023)
int32_t __fastcall D2SOUND_GetMasterVolume()
{
	return gnMasterVolume;
}

// 1.12a: 0x6F9B7BF0 (@10055)
int32_t __fastcall D2SOUND_SetSoundMixer(int32_t nMixer)
{
	SRegSaveValue("Diablo II", "Sound Mixer", 0, static_cast<DWORD>(nMixer));
	gnSoundMixer = nMixer;
	return nMixer;
}

// 1.12a: 0x6F9B7C10 (@10007)
int32_t __fastcall D2SOUND_GetSoundMixer()
{
	return gnSoundMixer;
}

// 1.12a: 0x6F9B7C20 (@10030)
void __fastcall D2SOUND_LoadConfigFromRegistry()
{
	gnSoundMixer = 0;
	gnMasterVolume = 100;
	gnMusicVolume = 50;
	gnPositionalBias = 50;
	gnNPCSpeech = 2;
	gbOptionsMusic = 1;

	DWORD dwVal = 0;
	if (SRegLoadValue("Diablo II", "Sound Mixer", 0, &dwVal) && dwVal < 3)
	{
		gnSoundMixer = static_cast<int32_t>(dwVal);
	}
	if (SRegLoadValue("Diablo II", "Master Volume", 0, &dwVal) && dwVal <= 100)
	{
		gnMasterVolume = static_cast<int32_t>(dwVal);
	}
	if (SRegLoadValue("Diablo II", "Music Volume", 0, &dwVal) && dwVal <= 100)
	{
		gnMusicVolume = static_cast<int32_t>(dwVal);
	}
	if (SRegLoadValue("Diablo II", "Positional Bias", 0, &dwVal) && dwVal <= 100)
	{
		gnPositionalBias = static_cast<int32_t>(dwVal);
	}
	if (SRegLoadValue("Diablo II", "NPC Speech", 0, &dwVal) && dwVal < 3)
	{
		gnNPCSpeech = static_cast<int32_t>(dwVal);
	}
	if (SRegLoadValue("Diablo II", "Options Music", 0, &dwVal) && dwVal <= 1)
	{
		gbOptionsMusic = static_cast<int32_t>(dwVal);
	}
}

// 1.12a: 0x6F9B7D40 (@10051 | 1.10f @10066)
int32_t __fastcall D2SOUND_FastIsInitialized()
{
	return gbFastInitialized;
}

// 1.12a: 0x6F9B7D50 (@10053)
void __fastcall D2SOUND_FastStopAll()
{
	if (!gbFastInitialized)
	{
		return;
	}
	for (int i = 0; i < 8; ++i)
	{
		D2SOUND_SmpStop(gpFastChannels[i]);
	}
}

// 1.12a: 0x6F9B7DD0 (@10059)
BOOL __fastcall D2SOUND_FastIsAnyPlaying()
{
	if (!gbFastInitialized)
	{
		return FALSE;
	}
	for (int i = 0; i < 8; ++i)
	{
		if (D2SOUND_SmpIsPlaying(gpFastChannels[i]))
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 1.12a: 0x6F9B7E30 (@10025)
void __fastcall D2SOUND_FastFadeAll()
{
	if (!gbFastInitialized)
	{
		return;
	}
	for (int i = 0; i < 8; ++i)
	{
		if (D2SOUND_SmpIsPlaying(gpFastChannels[i]))
		{
			D2SOUND_SmpFade(gpFastChannels[i], 0, 200);
		}
	}
}

// 1.12a: 0x6F9B7E90 (@10010)
void __fastcall D2SOUND_FastShutdown()
{
	if (!gbFastInitialized)
	{
		return;
	}
	EnterCriticalSection(&gcsFastSound);
	D2SOUND_FastStopAll();
	for (int i = 0; i < 8; ++i)
	{
		D2SOUND_SmpDestroy(gpFastChannels[i]);
		gpFastChannels[i] = nullptr;
	}
	D2SoundFastEntryStrc* pEntry = gpFastSoundList;
	while (pEntry != nullptr)
	{
		D2SoundFastEntryStrc* pNext = pEntry->pNext;
		if (pEntry->pWaveBuffer != nullptr)
		{
			D2_FREE(pEntry->pWaveBuffer);
		}
		D2_FREE(pEntry);
		gpFastSoundList = pNext;
		pEntry = pNext;
	}
	gbFastInitialized = FALSE;
	DeleteCriticalSection(&gcsFastSound);
}

// 1.12a: 0x6F9B7F30 (@10056 | 1.10f @10039)
void __fastcall D2SOUND_FastPlaySound(const char* szFile)
{
	if (!gbFastInitialized)
	{
		return;
	}
	EnterCriticalSection(&gcsFastSound);
	D2SoundFastEntryStrc* pFound = nullptr;
	for (D2SoundFastEntryStrc* pCur = gpFastSoundList; pCur != nullptr; pCur = pCur->pNext)
	{
		if (strcmp(pCur->szFileName, szFile) == 0)
		{
			pFound = pCur;
			break;
		}
	}
	if (pFound == nullptr)
	{
		LeaveCriticalSection(&gcsFastSound);
		return;
	}

	int nChannel = 0;
	for (; nChannel < 8; ++nChannel)
	{
		D2SoundSmpStrc* pSmp = gpFastChannels[nChannel];
		if (pSmp->dwFlags == pFound->dwFlags && !D2SOUND_SmpIsPlaying(pSmp))
		{
			break;
		}
	}
	if (nChannel < 8)
	{
		D2SoundSmpStrc* pSmp = gpFastChannels[nChannel];
		D2SOUND_SmpReset(pSmp);
		D2SOUND_SmpLoadWave(pSmp, pFound->pWaveBuffer, pFound->dwWaveSize);
		D2SOUND_SmpSetVolume(pSmp, 255);
		D2SOUND_SmpPlay(pSmp);
	}
	LeaveCriticalSection(&gcsFastSound);
}

// 1.12a: 0x6F9B8050 (@10029)
void __fastcall D2SOUND_FastCacheSound(const char* szFile)
{
	if (!gbFastInitialized)
	{
		return;
	}
	size_t dwBytesRead = 0;
	uint8_t* pWaveBuf = static_cast<uint8_t*>(ARCHIVE_ALLOC_BUFFER_AND_READ_FILE_TO_IT(nullptr, szFile, &dwBytesRead));
	if (pWaveBuf == nullptr)
	{
		return;
	}
	WAVEFORMATEX wfx = {};
	uint32_t dwSize = static_cast<uint32_t>(dwBytesRead);
	D2SOUND_UtilParseWaveData(pWaveBuf, &dwSize, &wfx);
	if (wfx.nSamplesPerSec == 22050)
	{
		D2SoundFastEntryStrc* pEntry = D2_ALLOC_STRC(D2SoundFastEntryStrc);
		pEntry->pNext = gpFastSoundList;
		gpFastSoundList = pEntry;
		pEntry->pWaveBuffer = pWaveBuf;
		pEntry->dwWaveSize = static_cast<uint32_t>(dwBytesRead);
		pEntry->dwFlags = (wfx.nChannels == 2) ? D2SOUNDSMP_FLAG_STEREO : 0;
		strcpy(pEntry->szFileName, szFile);
	}
}

// 1.12a: 0x6F9B80F0 (@10070)
void __fastcall D2SOUND_FastInit()
{
	if (gpDirectSound == nullptr || gbFastInitialized)
	{
		return;
	}
	InitializeCriticalSection(&gcsFastSound);
	EnterCriticalSection(&gcsFastSound);
	D2SOUND_SmpSetMasterVolume(255);
	for (int i = 0; i < 8; ++i)
	{
		gpFastChannels[i] = D2SOUND_SmpCreate((i < 4) ? D2SOUNDSMP_FLAG_STEREO : 0);
	}
	gbFastInitialized = TRUE;
	LeaveCriticalSection(&gcsFastSound);
}

// 1.12a: 0x6F9B81E0 (Internal)
static const char* D2SOUND_GetDirectSoundErrorString(HRESULT hr)
{
	switch (hr)
	{
	case DS_OK: return "DS_OK";
	case DSERR_ALLOCATED: return "DSERR_ALLOCATED";
	case DSERR_CONTROLUNAVAIL: return "DSERR_CONTROLUNAVAIL";
	case DSERR_INVALIDPARAM: return "DSERR_INVALIDPARAM";
	case DSERR_INVALIDCALL: return "DSERR_INVALIDCALL";
	case DSERR_GENERIC: return "DSERR_GENERIC";
	case DSERR_PRIOLEVELNEEDED: return "DSERR_PRIOLEVELNEEDED";
	case DSERR_OUTOFMEMORY: return "DSERR_OUTOFMEMORY";
	case DSERR_BADFORMAT: return "DSERR_BADFORMAT";
	case DSERR_UNSUPPORTED: return "DSERR_UNSUPPORTED";
	case DSERR_NODRIVER: return "DSERR_NODRIVER";
	case DSERR_ALREADYINITIALIZED: return "DSERR_ALREADYINITIALIZED";
	case DSERR_NOAGGREGATION: return "DSERR_NOAGGREGATION";
	case DSERR_BUFFERLOST: return "DSERR_BUFFERLOST";
	case DSERR_OTHERAPPHASPRIO: return "DSERR_OTHERAPPHASPRIO";
	case DSERR_UNINITIALIZED: return "DSERR_UNINITIALIZED";
	case DSERR_NOINTERFACE: return "DSERR_NOINTERFACE";
	default: return nullptr;
	}
}

// 1.12a: 0x6F9B8320 (Internal)
static BOOL D2SOUND_InitEaxListenerRoomOff(int32_t nEaxVer)
{
	if (gpListenerPropertySet == nullptr)
	{
		return FALSE;
	}
	if (nEaxVer == 1)
	{
		float fZero = 0.0f;
		if (FAILED(gpListenerPropertySet->Set(kDSPROPSETID_EAX10_ListenerProperties, 2, nullptr, 0, &fZero, sizeof(float))))
		{
			return FALSE;
		}
	}
	else
	{
		int32_t lMinus10000 = -10000;
		if (FAILED(gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 2, nullptr, 0, &lMinus10000, sizeof(int32_t))))
		{
			return FALSE;
		}
		if (FAILED(gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 3, nullptr, 0, &lMinus10000, sizeof(int32_t))))
		{
			return FALSE;
		}
	}
	gbEaxEnvironmentOff = TRUE;
	return TRUE;
}

// 1.12a: 0x6F9B83E0 (@10003)
int32_t __fastcall D2SOUND_GetUnknownZero()
{
	return 0;
}

// 1.12a: 0x6F9B83F0 (@10052)
BOOL __fastcall D2SOUND_IsEaxEnabled()
{
	return gbEaxEnabled;
}

// 1.12a: 0x6F9B8400 (@10060)
BOOL __fastcall D2SOUND_Is3DSoundEnabled()
{
	return gb3DSoundEnabled;
}

// 1.12a: 0x6F9B8410 (@10046)
IDirectSound* __fastcall D2SOUND_GetDirectSound()
{
	return gpDirectSound;
}

// 1.12a: 0x6F9B8420 (Internal)
static void D2SOUND_ReleaseListenerInterfaces()
{
	if (gpListenerPropertySet != nullptr)
	{
		gpListenerPropertySet->Release();
		gpListenerPropertySet = nullptr;
	}
	if (gpListener3DBuffer != nullptr)
	{
		gpListener3DBuffer->Release();
		gpListener3DBuffer = nullptr;
	}
	if (gpListenerDSBuffer != nullptr)
	{
		gpListenerDSBuffer->Release();
		gpListenerDSBuffer = nullptr;
	}
}

// 1.12a: 0x6F9B8480 (Internal)
static BOOL CALLBACK D2SOUND_DSEnumCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID /*lpContext*/)
{
	if (lpGuid != nullptr && !gbDriverEnumerated)
	{
		if (strcmp(lpcstrModule, "EMU10K1.VXD") == 0)
		{
			gbHasEmu10k1Vxd = TRUE;
		}
		if (strcmp(lpcstrModule, "es197x.vxd") == 0)
		{
			gbHasCrystalOrEs197xVxd = TRUE;
		}
		if (strcmp(lpcstrDescription, "Crystal SoundFusion(tm)") == 0)
		{
			gbHasCrystalOrEs197xVxd = TRUE;
		}
		if (strcmp(lpcstrModule, "YDSXG.VXD") == 0)
		{
			gbReuseBuffers = FALSE;
		}
		if (strcmp(lpcstrModule, "RIPTIDE.VXD") == 0)
		{
			gbReuseBuffers = FALSE;
		}
		gbDriverEnumerated = TRUE;
	}
	return TRUE;
}

// 1.12a: 0x6F9B8540 (@10028)
void __fastcall D2SOUND_SetEaxEnvironment(const D2SoundEaxEnvStrc* pEnv)
{
	if (!gbEaxEnabled || gpListenerPropertySet == nullptr || pEnv == nullptr)
	{
		return;
	}
	if (gnEaxVersion == 1)
	{
		float fRoomLinear = D2SOUND_UtilMillibelsToLinear(static_cast<float>(pEnv->lRoom));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX10_ListenerProperties, 1, nullptr, 0, const_cast<uint32_t*>(&pEnv->dwEnvironment), sizeof(uint32_t));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX10_ListenerProperties, 2, nullptr, 0, &fRoomLinear, sizeof(float));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX10_ListenerProperties, 3, nullptr, 0, const_cast<float*>(&pEnv->fDecayTime), sizeof(float));
	}
	if (gnEaxVersion == 2)
	{
		uint32_t dwFlags = 0x20;
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 11, nullptr, 0, const_cast<uint32_t*>(&pEnv->dwEnvironment), sizeof(uint32_t));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 15, nullptr, 0, &dwFlags, sizeof(uint32_t));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 11, nullptr, 0, const_cast<uint32_t*>(&pEnv->dwEnvironment), sizeof(uint32_t));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 12, nullptr, 0, const_cast<float*>(&pEnv->fEnvSize), sizeof(float));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 13, nullptr, 0, const_cast<float*>(&pEnv->fEnvDiffusion), sizeof(float));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 2, nullptr, 0, const_cast<int32_t*>(&pEnv->lRoom), sizeof(int32_t));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 3, nullptr, 0, const_cast<int32_t*>(&pEnv->lRoomHF), sizeof(int32_t));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 5, nullptr, 0, const_cast<float*>(&pEnv->fDecayTime), sizeof(float));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 6, nullptr, 0, const_cast<float*>(&pEnv->fDecayHFRatio), sizeof(float));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 7, nullptr, 0, const_cast<int32_t*>(&pEnv->lReflections), sizeof(int32_t));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 8, nullptr, 0, const_cast<float*>(&pEnv->fReflectionsDelay), sizeof(float));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 9, nullptr, 0, const_cast<int32_t*>(&pEnv->lReverb), sizeof(int32_t));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 10, nullptr, 0, const_cast<float*>(&pEnv->fReverbDelay), sizeof(float));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 4, nullptr, 0, const_cast<float*>(&pEnv->fRoomRolloffFactor), sizeof(float));
		gpListenerPropertySet->Set(kDSPROPSETID_EAX20_ListenerProperties, 14, nullptr, 0, const_cast<float*>(&pEnv->fAirAbsorptionHF), sizeof(float));
	}
	gbEaxEnvironmentOff = (pEnv->lRoom == -10000);
}

// 1.12a: 0x6F9B87A0 (@10002 | 1.10f @10001)
void __fastcall D2SOUND_CloseSoundSystem()
{
	gbSoundSystemOpened = FALSE;
	D2SOUND_SmpSetMasterVolume(0);

	if (gpMusicSample != nullptr)
	{
		D2SOUND_SmpDestroy(gpMusicSample);
		gpMusicSample = nullptr;
	}
	SFileDdaDestroy();
	D2SOUND_StopMixerThread();
	D2SOUND_ReleaseListenerInterfaces();

	if (gpPrimaryDSBuffer != nullptr)
	{
		gpPrimaryDSBuffer->Release();
		gpPrimaryDSBuffer = nullptr;
	}
	if (gpDirectSound != nullptr)
	{
		gpDirectSound->Release();
		gpDirectSound = nullptr;
	}
	if (gbSoundCSInitialized)
	{
		DeleteCriticalSection(&gcsSound);
		gbSoundCSInitialized = FALSE;
	}
}

// 1.12a: 0x6F9B8910 (Internal)
static void D2SOUND_InitDirectSoundAttempt()
{
	if (gb3DSoundEnabled && gbDisable3DSoundDueToDriver)
	{
		return;
	}
	D2_ASSERT(!gbSoundCSInitialized);
	InitializeCriticalSection(&gcsSound);
	gbSoundCSInitialized = TRUE;

	HRESULT hr = DirectSoundCreate(nullptr, &gpDirectSound, nullptr);
	if (FAILED(hr))
	{
		FOG_Trace("DirectSound error: %s returned on DirectSoundCreate", D2SOUND_GetDirectSoundErrorString(hr));
		D2SOUND_CloseSoundSystem();
		return;
	}

	hr = gpDirectSound->SetCooperativeLevel(WINDOW_GetWindow(), DSSCL_PRIORITY);
	if (FAILED(hr))
	{
		FOG_Trace("DirectSound error: %s returned on SetCooperativeLevel", D2SOUND_GetDirectSoundErrorString(hr));
		D2SOUND_CloseSoundSystem();
		return;
	}

	DSBUFFERDESC dsbd = {};
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = gb3DSoundEnabled ? (DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D) : DSBCAPS_PRIMARYBUFFER;
	hr = gpDirectSound->CreateSoundBuffer(&dsbd, &gpPrimaryDSBuffer, nullptr);
	if (FAILED(hr))
	{
		FOG_Trace("........DirectSound progress: %s returned on failed CreateSoundBuffer", D2SOUND_GetDirectSoundErrorString(hr));
		D2SOUND_CloseSoundSystem();
		return;
	}

	memset(&gDSCaps, 0, sizeof(DSCAPS));
	gDSCaps.dwSize = sizeof(DSCAPS);
	hr = gpDirectSound->GetCaps(&gDSCaps);
	if (FAILED(hr))
	{
		FOG_Trace("........DirectSound progress: %s returned on failed GetCaps", D2SOUND_GetDirectSoundErrorString(hr));
		D2SOUND_CloseSoundSystem();
		return;
	}

	if (gb3DSoundEnabled && gDSCaps.dwMaxHw3DAllBuffers < 16)
	{
		FOG_Trace("........DirectSound progress: %d buffers available is less than min needed %d buffers for 3d sound",
		          gDSCaps.dwMaxHw3DAllBuffers, 16);
		D2SOUND_CloseSoundSystem();
		return;
	}

	WAVEFORMATEX wfx = {};
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 22050;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = 4;
	wfx.nAvgBytesPerSec = 88200;
	hr = gpPrimaryDSBuffer->SetFormat(&wfx);
	if (FAILED(hr))
	{
		FOG_Trace("........DirectSound progress: %s returned on failed SetFormat", D2SOUND_GetDirectSoundErrorString(hr));
		gpPrimaryDSBuffer->Release();
		gpPrimaryDSBuffer = nullptr;
		D2SOUND_CloseSoundSystem();
		return;
	}

	gnEaxVersion = 0;
	if (gbEaxEnabled)
	{
		WAVEFORMATEX wfxMono = {};
		D2SOUND_InitWaveFormat(&wfxMono, 1);
		DSBUFFERDESC dsbd3d = {};
		dsbd3d.dwSize = sizeof(DSBUFFERDESC);
		dsbd3d.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_STATIC;
		dsbd3d.dwBufferBytes = 0x400;
		dsbd3d.lpwfxFormat = &wfxMono;

		ULONG ulSupport = 0;
		if (SUCCEEDED(gpDirectSound->CreateSoundBuffer(&dsbd3d, &gpListenerDSBuffer, nullptr)) &&
		    SUCCEEDED(gpListenerDSBuffer->QueryInterface(kIID_IDirectSound3DBuffer, reinterpret_cast<LPVOID*>(&gpListener3DBuffer))) &&
		    SUCCEEDED(gpListener3DBuffer->QueryInterface(kIID_IKsPropertySet, reinterpret_cast<LPVOID*>(&gpListenerPropertySet))) &&
		    SUCCEEDED(gpListenerPropertySet->QuerySupport(kDSPROPSETID_EAX20_ListenerProperties, 1, &ulSupport)) &&
		    (ulSupport & 3) == 3 &&
		    D2SOUND_InitEaxListenerRoomOff(2))
		{
			gnEaxVersion = 2;
		}
		else
		{
			D2SOUND_ReleaseListenerInterfaces();
			ulSupport = 0;
			if (SUCCEEDED(gpDirectSound->CreateSoundBuffer(&dsbd3d, &gpListenerDSBuffer, nullptr)) &&
			    SUCCEEDED(gpListenerDSBuffer->QueryInterface(kIID_IDirectSound3DBuffer, reinterpret_cast<LPVOID*>(&gpListener3DBuffer))) &&
			    SUCCEEDED(gpListener3DBuffer->QueryInterface(kIID_IKsPropertySet, reinterpret_cast<LPVOID*>(&gpListenerPropertySet))) &&
			    SUCCEEDED(gpListenerPropertySet->QuerySupport(kDSPROPSETID_EAX10_ListenerProperties, 0, &ulSupport)) &&
			    (ulSupport & 3) == 3 &&
			    D2SOUND_InitEaxListenerRoomOff(1))
			{
				gnEaxVersion = 1;
			}
			else
			{
				D2SOUND_ReleaseListenerInterfaces();
				FOG_Trace("........DirectSound progress: Unsupported EAX or EAX emulation");
				D2SOUND_CloseSoundSystem();
				return;
			}
		}
	}

	DSBCAPS dsbcaps = {};
	dsbcaps.dwSize = sizeof(DSBCAPS);
	gpPrimaryDSBuffer->GetCaps(&dsbcaps);
	D2SOUND_SmpFillBufferWithSilence(gpPrimaryDSBuffer, 0, dsbcaps.dwBufferBytes);
	gpPrimaryDSBuffer->Play(0, 0, DSBPLAY_LOOPING);

	D2SOUND_StartMixerThread();
	D2SOUND_InitMusicSystem();
	D2SOUND_LoadConfigFromRegistry();
	D2SOUND_SmpSetMasterVolume(255);
	gbSoundSystemOpened = TRUE;
}

// 1.12a: 0x6F9B8D80 (@10032 | 1.10f @10000)
#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 13
void __fastcall D2SOUND_OpenSoundSystem(BOOL bExpansion, BOOL /*bSoundBackground*/)
#else
void __fastcall D2SOUND_OpenSoundSystem(BOOL bExpansion)
#endif
{
	gbIsExpansion = bExpansion;
	gbDriverEnumerated = FALSE;
	DirectSoundEnumerateA(D2SOUND_DSEnumCallback, nullptr);

	char szPath[MAX_PATH] = {};
	uint32_t dwMajor = 0, dwMinor = 0, dwBuild = 0;
	if (gbHasEmu10k1Vxd)
	{
		GetWindowsDirectoryA(szPath, MAX_PATH);
		strcat(szPath, "\\SYSTEM\\EMU10K1.VXD");
		if (D2SOUND_GetVxdFileVersion(szPath, &dwBuild, &dwMajor, &dwMinor) && dwMajor == 4)
		{
			if ((dwMinor == 6 && dwBuild >= 0x26E && dwBuild <= 0x2C6) ||
			    (dwMinor == 11 && dwBuild >= 0x274 && dwBuild <= 0x275))
			{
				gbDisable3DSoundDueToDriver = TRUE;
			}
		}
	}
	if (gbHasCrystalOrEs197xVxd)
	{
		GetWindowsDirectoryA(szPath, MAX_PATH);
		strcat(szPath, "\\SYSTEM\\CRLDS3D.VXD");
		if (D2SOUND_GetVxdFileVersion(szPath, &dwBuild, &dwMajor, &dwMinor))
		{
			if (dwMajor < 4 || (dwMajor == 4 && (dwMinor < 12 || (dwMinor == 12 && dwBuild < 0x40B))))
			{
				gbDisable3DSoundDueToDriver = TRUE;
			}
		}
	}

	if (!gbSoundSystemOpened)
	{
		gb3DSoundEnabled = TRUE;
		gbEaxEnabled = TRUE;
		D2SOUND_InitDirectSoundAttempt();
		if (!gbSoundSystemOpened)
		{
			gb3DSoundEnabled = TRUE;
			gbEaxEnabled = FALSE;
			D2SOUND_InitDirectSoundAttempt();
			if (!gbSoundSystemOpened)
			{
				gb3DSoundEnabled = FALSE;
				gbEaxEnabled = FALSE;
				D2SOUND_InitDirectSoundAttempt();
				if (!gbSoundSystemOpened)
				{
					FOG_Trace("\tCouldn't initialize DirectSound");
				}
			}
		}
	}
}

// 1.12a: 0x6F9B9110 (@10020)
void __fastcall D2SOUND_ListAppend(D2SoundListNodeStrc** ppHead, void* pItem)
{
	D2SoundListNodeStrc* pNode = D2_CALLOC_STRC(D2SoundListNodeStrc);
	pNode->pItem = pItem;
	if (*ppHead == nullptr)
	{
		*ppHead = pNode;
		return;
	}
	D2SoundListNodeStrc* pCur = *ppHead;
	while (pCur->pNext != nullptr)
	{
		pCur = pCur->pNext;
	}
	pCur->pNext = pNode;
}

// 1.12a: 0x6F9B9160 (@10057)
int32_t __fastcall D2SOUND_ListCount(D2SoundListNodeStrc* pHead)
{
	int32_t nCount = 0;
	for (D2SoundListNodeStrc* pCur = pHead; pCur != nullptr; pCur = pCur->pNext)
	{
		++nCount;
	}
	return nCount;
}

// 1.12a: 0x6F9B9170 (@10047)
BOOL __fastcall D2SOUND_ListContains(D2SoundListNodeStrc* pHead, void* pItem)
{
	for (D2SoundListNodeStrc* pCur = pHead; pCur != nullptr; pCur = pCur->pNext)
	{
		if (pCur->pItem == pItem)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 1.12a: 0x6F9B9190 (@10064)
void __fastcall D2SOUND_ListRemove(D2SoundListNodeStrc** ppHead, void* pItem)
{
	D2SoundListNodeStrc* pPrev = nullptr;
	for (D2SoundListNodeStrc* pCur = *ppHead; pCur != nullptr; pCur = pCur->pNext)
	{
		if (pCur->pItem == pItem)
		{
			if (pCur == *ppHead)
			{
				*ppHead = pCur->pNext;
			}
			if (pPrev != nullptr)
			{
				pPrev->pNext = pCur->pNext;
			}
			D2_FREE(pCur);
			return;
		}
		pPrev = pCur;
	}
}

// 1.12a: 0x6F9B91E0 (@10015)
void __fastcall D2SOUND_ListPrepend(D2SoundListNodeStrc** ppHead, void* pItem)
{
	D2SoundListNodeStrc* pNode = D2_CALLOC_STRC(D2SoundListNodeStrc);
	pNode->pItem = pItem;
	pNode->pNext = *ppHead;
	*ppHead = pNode;
}

// 1.12a: 0x6F9B9220 (Internal)
static void D2SOUND_InitWaveFormat(WAVEFORMATEX* pFormat, uint16_t nChannels)
{
	memset(pFormat, 0, sizeof(WAVEFORMATEX));
	pFormat->wFormatTag = WAVE_FORMAT_PCM;
	pFormat->nChannels = nChannels;
	pFormat->nSamplesPerSec = 22050;
	pFormat->wBitsPerSample = 16;
	pFormat->nBlockAlign = nChannels * 2;
	pFormat->nAvgBytesPerSec = static_cast<DWORD>(pFormat->nBlockAlign) * 22050u;
}

// 1.12a: 0x6F9B9260 (Internal)
static const uint8_t* D2SOUND_FindRiffChunk(const uint8_t* pData, uint32_t* pInOutSize, uint32_t dwFourCC, uint32_t* pOutChunkSize)
{
	while (*pInOutSize >= 8)
	{
		*pInOutSize -= 8;
		const uint32_t dwChunkId = *reinterpret_cast<const uint32_t*>(pData);
		const uint32_t dwChunkSize = *reinterpret_cast<const uint32_t*>(pData + 4);
		pData += 8;
		if (dwChunkId == dwFourCC)
		{
			*pOutChunkSize = dwChunkSize;
			return pData;
		}
		if (*pInOutSize < dwChunkSize)
		{
			break;
		}
		*pInOutSize -= dwChunkSize;
		pData += dwChunkSize;
	}
	return nullptr;
}

// 1.12a: 0x6F9B92A0 (@10045)
float __fastcall D2SOUND_UtilMillibelsToLinear(float fMillibels)
{
	if (fMillibels + 0.0001f >= 0.0f)
	{
		return 1.0f;
	}
	if (fMillibels - 0.0001f <= -10000.0f)
	{
		return 0.0f;
	}
	return static_cast<float>(std::pow(10.0, static_cast<double>(fMillibels * 0.0005f)));
}

// 1.12a: 0x6F9B9300 (@10031)
float __fastcall D2SOUND_UtilRatioToMillibels(float fMin, float fMax, float fVal)
{
	if (fMin + 0.0001f >= fVal)
	{
		return -10000.0f;
	}
	if (fMax - 0.0001f <= fVal)
	{
		return 0.0f;
	}
	const double dSpan = static_cast<double>(fMax) + static_cast<double>((fMin < 0.0f) ? fMin : fMax);
	const double dRatio = dSpan / static_cast<double>(fVal);
	return static_cast<float>(std::log10(dRatio) * -2000.0);
}

// 1.12a: 0x6F9B93F0 (@10011)
void __fastcall D2SOUND_ListClear(D2SoundListNodeStrc** ppHead)
{
	while (*ppHead != nullptr)
	{
		D2SOUND_ListRemove(ppHead, (*ppHead)->pItem);
	}
}

// 1.12a: 0x6F9B9450 (@10068)
const uint8_t* __fastcall D2SOUND_UtilParseWaveData(const uint8_t* pWaveBuffer, uint32_t* pInOutSize, WAVEFORMATEX* pOutFormat)
{
	if (*pInOutSize < 0x20)
	{
		return nullptr;
	}
	*pInOutSize -= 12;
	if (*reinterpret_cast<const uint32_t*>(pWaveBuffer) != 0x46464952u || // 'RIFF'
	    *reinterpret_cast<const uint32_t*>(pWaveBuffer + 8) != 0x45564157u) // 'WAVE'
	{
		return nullptr;
	}

	uint32_t dwFmtSize = 0;
	const uint8_t* pFmt = D2SOUND_FindRiffChunk(pWaveBuffer + 12, pInOutSize, 0x20746D66u, &dwFmtSize); // 'fmt '
	if (pFmt == nullptr || dwFmtSize < 16 || *pInOutSize < dwFmtSize)
	{
		return nullptr;
	}

	*pInOutSize -= dwFmtSize;
	memcpy(pOutFormat, pFmt, 16);
	pOutFormat->cbSize = 0;

	uint32_t dwDataSize = 0;
	const uint8_t* pData = D2SOUND_FindRiffChunk(pFmt + dwFmtSize, pInOutSize, 0x61746164u, &dwDataSize); // 'data'
	if (pData == nullptr || *pInOutSize < dwDataSize)
	{
		return nullptr;
	}

	*pInOutSize = dwDataSize;
	return pData;
}

// 1.12a: 0x6F9B9520 (Internal)
static BOOL D2SOUND_GetVxdFileVersion(const char* szFilePath, uint32_t* pOutBuild, uint32_t* pOutMajor, uint32_t* pOutMinor)
{
	DWORD dwHandle = 0;
	DWORD dwVerSize = GetFileVersionInfoSizeA(szFilePath, &dwHandle);
	if (dwVerSize == 0)
	{
		return FALSE;
	}
	void* pVerData = D2_ALLOC(dwVerSize);
	if (!GetFileVersionInfoA(szFilePath, dwHandle, dwVerSize, pVerData))
	{
		D2_FREE(pVerData);
		return FALSE;
	}
	VS_FIXEDFILEINFO* pFixedInfo = nullptr;
	UINT uLen = 0;
	if (!VerQueryValueA(pVerData, "\\", reinterpret_cast<LPVOID*>(&pFixedInfo), &uLen) || pFixedInfo == nullptr)
	{
		D2_FREE(pVerData);
		return FALSE;
	}
	*pOutBuild = HIWORD(pFixedInfo->dwFileVersionLS);
	*pOutMajor = pFixedInfo->dwFileVersionMS & 0xFFFFu;
	*pOutMinor = pFixedInfo->dwFileVersionLS & 0xFFFFu;
	D2_FREE(pVerData);
	return TRUE;
}

// 1.12a: 0x6F9B9620 (@10021)
void __fastcall D2SOUND_UtilCalc2DVolumeAndPan(int32_t* pOutVolume, int32_t* pOutPan, float fX, float fY, float fZ)
{
	float fDist = std::sqrt(fX * fX + fY * fY + fZ * fZ);
	float fDbDrop = 0.0f;
	if (fDist >= 100.0f)
	{
		fDist = 100.0f;
		fDbDrop = std::log2(fDist * 0.5f) * 6.0f;
	}
	else if (fDist > 2.0f)
	{
		fDbDrop = std::log2(fDist * 0.5f) * 6.0f;
	}

	const double dAtten = std::pow(10.0, static_cast<double>(fDbDrop * 0.05f));
	int32_t nVol = static_cast<int32_t>(255.0 / dAtten);
	nVol = std::min(std::max(nVol, 0), 255);

	int32_t nPan = static_cast<int32_t>(fY * 127.0f * 0.8f + 128.0f);
	nPan = std::min(std::max(nPan, 0), 255);

	*pOutVolume = nVol;
	*pOutPan = nPan;
}

// 1.12a: 0x6F9B9730 (@10004)
BOOL __fastcall D2SOUND_IsMusicEnabled()
{
	return gbMusicEnabled;
}

// 1.12a: 0x6F9B9740 (@10050)
BOOL __fastcall D2SOUND_IsMusicPlaying()
{
	return gpMusicSample != nullptr && gpMusicSample->hFile != nullptr;
}

// 1.12a: 0x6F9B97D0 (Internal)
static void D2SOUND_ResetMusicTracksPlayed()
{
	if (!gbMusicTracksInitialized)
	{
		gbMusicTracksInitialized = TRUE;
		gbMusicTracksUseExpansion = gbIsExpansion;
	}
	D2_ASSERT(gnClassicMusicTrackCount == gnExpansionMusicTrackCount);
	D2SoundMusicTrackStrc* pTracks = gbMusicTracksUseExpansion ? gExpansionMusicTracks : gClassicMusicTracks;
	for (int32_t i = 0; i < gnClassicMusicTrackCount; ++i)
	{
		pTracks[i].bPlayed = FALSE;
	}
}

// 1.12a: 0x6F9B9890 (@10006 | 1.10f @10034)
void __fastcall D2SOUND_EnableMusic(int32_t bEnable)
{
	if (bEnable && !gbMusicEnabled)
	{
		D2SOUND_ResetMusicTracksPlayed();
		gbPlayFirstMusicTrack = TRUE;
	}
	gbMusicEnabled = bEnable;
}

// 1.12a: 0x6F9B98C0 (@10037 | 1.10f @10065)
void __fastcall D2SOUND_FadeMusic()
{
	if (gpMusicSample != nullptr && gpMusicSample->hFile != nullptr)
	{
		gbMusicEnabled = FALSE;
		D2SOUND_SmpFade(gpMusicSample, 0, 200);
	}
}

// 1.12a: 0x6F9B98F0 (@10048)
void __fastcall D2SOUND_StopMusic()
{
	if (gpMusicSample != nullptr)
	{
		gbMusicEnabled = FALSE;
		if (gpMusicSample->hFile != nullptr)
		{
			D2SOUND_SmpStop(gpMusicSample);
		}
	}
}

// 1.12a: 0x6F9B9920 (@10066)
BOOL __fastcall D2SOUND_PlayMusicFile(const char* szFile, BOOL bLoop)
{
	if (gpMusicSample == nullptr || gpDirectSound == nullptr || gnCurrentMasterVolume == 0 ||
	    gpMusicSample->hFile != nullptr || szFile == nullptr)
	{
		return FALSE;
	}
	D2SOUND_SmpReset(gpMusicSample);
	D2SOUND_SmpSetVolume(gpMusicSample, 110);
	return D2SOUND_SmpOpenStream(gpMusicSample, szFile, 0, bLoop);
}

// 1.12a: 0x6F9B9990 (Internal)
static void D2SOUND_InitMusicSystem()
{
	SFileDdaInitialize(gpDirectSound);
	gpMusicSample = D2SOUND_SmpCreate(D2SOUNDSMP_FLAG_STEREO);
}

// 1.12a: 0x6F9B99B0 (@10063)
BOOL __fastcall D2SOUND_PlayNextMusicTrack()
{
	EnterCriticalSection(&gcsSound);
	if (gpMusicSample == nullptr || gpDirectSound == nullptr || gnCurrentMasterVolume == 0 || gpMusicSample->hFile != nullptr)
	{
		LeaveCriticalSection(&gcsSound);
		return FALSE;
	}

	BOOL bResetTried = FALSE;
	for (;;)
	{
		if (!gbMusicTracksInitialized)
		{
			gbMusicTracksInitialized = TRUE;
			gbMusicTracksUseExpansion = gbIsExpansion;
		}
		D2_ASSERT(gnClassicMusicTrackCount == gnExpansionMusicTrackCount);
		D2SoundMusicTrackStrc* pTracks = gbMusicTracksUseExpansion ? gExpansionMusicTracks : gClassicMusicTracks;

		bool bAnyUnplayed = false;
		for (int32_t i = 0; i < gnClassicMusicTrackCount; ++i)
		{
			if (!pTracks[i].bPlayed)
			{
				bAnyUnplayed = true;
				break;
			}
		}

		if (!bAnyUnplayed)
		{
			if (bResetTried)
			{
				gbMusicEnabled = FALSE;
				LeaveCriticalSection(&gcsSound);
				return FALSE;
			}
			D2SOUND_ResetMusicTracksPlayed();
			bResetTried = TRUE;
		}

		int32_t nIdx = 0;
		if (gbPlayFirstMusicTrack)
		{
			gbPlayFirstMusicTrack = FALSE;
			nIdx = 0;
		}
		else
		{
			nIdx = std::rand() % gnClassicMusicTrackCount;
			while (pTracks[nIdx].bPlayed)
			{
				nIdx = (nIdx + 1) % gnClassicMusicTrackCount;
			}
		}

		pTracks[nIdx].bPlayed = TRUE;
		if (D2SOUND_PlayMusicFile(pTracks[nIdx].szFileName, FALSE))
		{
			LeaveCriticalSection(&gcsSound);
			return TRUE;
		}
	}
}
