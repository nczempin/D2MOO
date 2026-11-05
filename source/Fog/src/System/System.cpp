#include <Fog.h>
#include <Storm.h>
#include <System/System.h>
#include <Windows.h>
#include <ctime>
#include <cstring>

static BOOL gdwFogUseDirect = FALSE;
static BOOL gbExpansionChecked = FALSE;
static BOOL gbIsExpansion = FALSE;
static time_t gnSyncBaseEpoch = 0;
static DWORD gdwSyncBaseTick = 0;

// 1.10f: Fog.0x6FF5DC20 (#10050)
// 1.12a: Fog.0x6FF61550 (#10050)
void __fastcall FOG_10050_EnterCriticalSection(_Acquires_lock_(*_Curr_) CRITICAL_SECTION* pCriticalSection, int nLine)
{
	D2_MAYBE_UNUSED(nLine);
	EnterCriticalSection(pCriticalSection);
}

// 1.10f: Fog.0x6FF5A690 (#10055)
// 1.12a: Fog.0x6FF69F40 (#10055)
int32_t __fastcall FOG_10055_GetSyncTime()
{
	const DWORD dwNow = GetTickCount();
	if (gnSyncBaseEpoch == 0 || (dwNow - gdwSyncBaseTick) > 30000)
	{
		const int nPriority = GetThreadPriority(GetCurrentThread());
		if (nPriority != THREAD_PRIORITY_ERROR_RETURN)
		{
			SetThreadPriority(GetCurrentThread(), nPriority);
		}
		gnSyncBaseEpoch = time(nullptr);
		gdwSyncBaseTick = GetTickCount();
	}
	return static_cast<int32_t>(gnSyncBaseEpoch + (dwNow - gdwSyncBaseTick) / 1000);
}

// 1.10f: Fog.0x6FF61590 (#10101)
// 1.12a: Fog.0x6FF6EFF0 (#10101)
BOOL __fastcall FOG_MPQSetConfig(int dwDirectAccessFlags, int bEnableSeekOptimization)
{
	SFileSetBasePath(const_cast<char*>(""));
	SFileEnableDirectAccess(reinterpret_cast<HANDLE>(static_cast<uintptr_t>(dwDirectAccessFlags)));
	SFileSetIoErrorMode(0, 0);
	SFileEnableSeekOptimization(bEnableSeekOptimization);
	gdwFogUseDirect = dwDirectAccessFlags;

	int nOpt2 = 0x8000;
	StormSetOption(2, reinterpret_cast<int>(&nOpt2), sizeof(nOpt2));
	int nOpt1 = 1;
	StormSetOption(1, reinterpret_cast<int>(&nOpt1), sizeof(nOpt1));
	return StormSetOption(3, reinterpret_cast<int>(&nOpt1), sizeof(nOpt1));
}

// 1.10f: Fog.0x6FF61600 (#10102)
// 1.12a: Fog.0x6FF6EF40 (#10102)
BOOL __fastcall FOG_FOpenFile(const char* szFile, HSFILE* pFileHandle)
{
	return SFileOpenFile(const_cast<char*>(szFile), reinterpret_cast<int>(pFileHandle));
}

// 1.10f: Fog.0x6FF61610 (#10103)
// 1.12a: Fog.0x6FF6EF30 (#10103)
void __fastcall FOG_FCloseFile(HSFILE pFile)
{
	SFileCloseFile(reinterpret_cast<HANDLE>(pFile));
}

// 1.10f: Fog.0x6FF61620 (#10104)
// 1.12a: Fog.0x6FF6EF00 (#10104)
BOOL __fastcall FOG_FReadFile(HSFILE pFile, void* pBuffer, size_t nSize, int* nBytesRead, uint32_t a5, uint32_t a6, uint32_t a7)
{
	return SFileFileExistsEx(
		reinterpret_cast<int>(pFile),
		reinterpret_cast<int>(pBuffer),
		static_cast<int>(nSize),
		reinterpret_cast<int>(nBytesRead),
		static_cast<LONG>(a5),
		static_cast<int>(a7),
		static_cast<int>(a6));
}

// 1.10f: Fog.0x6FF61650 (#10105)
// 1.12a: Fog.0x6FF6EEF0 (#10105)
uint32_t __fastcall FOG_FGetFileSize(HSFILE pFileHandle, uint32_t* lpFileSizeHigh)
{
	return static_cast<uint32_t>(SFileGetFileSize(reinterpret_cast<int>(pFileHandle), reinterpret_cast<LPDWORD>(lpFileSizeHigh)));
}

// 1.10f: Fog.0x6FF61660 (#10106)
// 1.12a: Fog.0x6FF6EED0 (#10106)
size_t __fastcall FOG_FSetFilePointer(HSFILE hFile, int32_t lDistanceToMove, int32_t* lpDistanceToMoveHigh, uint32_t dwMoveMethod)
{
	return static_cast<size_t>(SFileSetFilePointer(
		reinterpret_cast<int>(hFile),
		lDistanceToMove,
		reinterpret_cast<int>(lpDistanceToMoveHigh),
		dwMoveMethod));
}

// 1.10f: Fog.0x6FF61680 (#10107)
// 1.12a: Fog.0x6FF6EEA0 (#10107)
HANDLE __fastcall FOG_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	return ::CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

// 1.10f: Fog.0x6FF616B0 (#10108)
// 1.12a: Fog.0x6FF6EE90 (#10108)
BOOL __fastcall FOG_DeleteFileA(LPCSTR lpFileName)
{
	return ::DeleteFileA(lpFileName);
}

// 1.10f: Fog.0x6FF616C0 (#10109)
// 1.12a: Fog.0x6FF6EE80 (#10109)
BOOL __fastcall FOG_CloseFile(HANDLE hFile)
{
	return ::CloseHandle(hFile);
}

// 1.10f: Fog.0x6FF61730 (#10114)
// 1.12a: Fog.0x6FF6ED40 (#10114)
BOOL __fastcall FOG_CreatePathHierarchy(char* szPath)
{
	if (!szPath || szPath[0] == '\0')
	{
		return FALSE;
	}

	char szDir[MAX_PATH];
	char* pSep = const_cast<char*>(SStrChr(static_cast<const char*>(szPath + 1), '\\'));
	while (pSep != nullptr)
	{
		const size_t nLen = static_cast<size_t>(pSep - szPath);
		if (nLen >= MAX_PATH)
		{
			return FALSE;
		}

		memcpy(szDir, szPath, nLen);
		szDir[nLen] = '\0';

		if (!CreateDirectoryA(szDir, nullptr))
		{
			const DWORD dwErr = GetLastError();
			if (dwErr != ERROR_ALREADY_EXISTS && dwErr != ERROR_ACCESS_DENIED)
			{
				return FALSE;
			}
			const DWORD dwAttr = GetFileAttributesA(szDir);
			if (dwAttr == INVALID_FILE_ATTRIBUTES || (dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				return FALSE;
			}
		}

		pSep = const_cast<char*>(SStrChr(static_cast<const char*>(pSep + 1), '\\'));
	}

	return TRUE;
}

// 1.10f: Fog.0x6FF61900 (#10115)
// 1.12a: Fog.0x6FF6F060 (#10115)
size_t __fastcall FOG_GetSavePath(char* pPathBuffer, size_t nBufferSize)
{
	if (!SRegLoadString("Diablo II", "Save Path", 0, pPathBuffer, static_cast<unsigned int>(nBufferSize)))
	{
		pPathBuffer[0] = '\0';
	}

	if (GetFileAttributesA(pPathBuffer) == INVALID_FILE_ATTRIBUTES)
	{
		pPathBuffer[0] = '\0';
	}

	if (pPathBuffer[0] == '\0')
	{
		FOG_GetInstallPath(pPathBuffer, nBufferSize);
		SStrNCat(pPathBuffer, "save\\", static_cast<int>(nBufferSize));
		SRegSaveString("Diablo II", "Save Path", 0, pPathBuffer);
	}

	if (nBufferSize != 0)
	{
		const size_t nLen = static_cast<size_t>(SStrLen(pPathBuffer));
		if (nLen == 0 || pPathBuffer[nLen - 1] != '\\')
		{
			if (nLen + 1 < nBufferSize)
			{
				pPathBuffer[nLen] = '\\';
				pPathBuffer[nLen + 1] = '\0';
			}
		}
	}

	return static_cast<size_t>(FOG_CreatePathHierarchy(pPathBuffer));
}

// 1.10f: Fog.0x6FF61870 (#10116)
// 1.12a: Fog.0x6FF6EF60 (#10116)
BOOL __fastcall FOG_GetInstallPath(char* pPathBuffer, size_t nBufferSize)
{
	if (!SRegLoadString("Diablo II", "InstallPath", 0, pPathBuffer, static_cast<unsigned int>(nBufferSize)))
	{
		pPathBuffer[0] = '\0';
	}

	if (GetFileAttributesA(pPathBuffer) == INVALID_FILE_ATTRIBUTES)
	{
		pPathBuffer[0] = '\0';
	}

	if (pPathBuffer[0] == '\0')
	{
		GetModuleFileNameA(nullptr, pPathBuffer, static_cast<DWORD>(nBufferSize));
		pPathBuffer[nBufferSize - 1] = '\0';
		char* pLastSlash = const_cast<char*>(SStrChrR(static_cast<const char*>(pPathBuffer), '\\'));
		if (pLastSlash)
		{
			pLastSlash[1] = '\0';
		}
		SRegSaveString("Diablo II", "InstallPath", 0, pPathBuffer);
	}

	if (nBufferSize != 0)
	{
		const size_t nLen = static_cast<size_t>(SStrLen(pPathBuffer));
		if (nLen == 0 || pPathBuffer[nLen - 1] != '\\')
		{
			if (nLen + 1 < nBufferSize)
			{
				pPathBuffer[nLen] = '\\';
				pPathBuffer[nLen + 1] = '\0';
			}
		}
	}

	return TRUE;
}

// 1.10f: Fog.0x6FF61A10 (#10117)
// 1.12a: Fog.0x6FF6ECC0 (#10117)
BOOL __fastcall FOG_UseDirect()
{
	return gdwFogUseDirect;
}

// 1.10f: Fog.0x6FF5A860 (#10176)
// 1.12a: Fog.0x6FF69FE0 (#10176)
BOOL __stdcall FOG_IsWindowsNT()
{
	OSVERSIONINFOA VersionInformation{};
	VersionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	return GetVersionExA(&VersionInformation) && VersionInformation.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

// 1.10f: Fog.0x6FF5D730 (#10227)
// 1.12a: Fog.0x6FF6F9A0 (#10227)
int __fastcall FOG_IsExpansion()
{
	if (!gbExpansionChecked)
	{
		gbExpansionChecked = TRUE;
		char szPath[MAX_PATH];
		FOG_GetInstallPath(szPath, sizeof(szPath));
		SStrNCat(szPath, "d2exp.mpq", sizeof(szPath));
		if (GetFileAttributesA(szPath) != INVALID_FILE_ATTRIBUTES)
		{
			gbIsExpansion = TRUE;
			return gbIsExpansion;
		}

		SStrCopy(szPath, "d2exp.mpq", sizeof(szPath));
		if (GetFileAttributesA(szPath) != INVALID_FILE_ATTRIBUTES)
		{
			gbIsExpansion = TRUE;
		}
	}
	return gbIsExpansion;
}
