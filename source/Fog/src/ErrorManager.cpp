#include <Fog.h>
#include <Storm.h>
#include <Winsock2.h>
#include <Windows.h>
#include <cstdio>
#include <cstring>

static char gszProgramName[MAX_PATH] = {};
static char gszVersion[MAX_PATH] = {};
static D2ExceptionCallback gpExceptionCallback = nullptr;
static BOOL gbErrorMgrInitialized = FALSE;
static BOOL gbIsHandlingError = FALSE;
static uint8_t gtFogSystemInfo[0x400] = {};

static void ErrorMgr_Report(int nCategory, const char* szFile, int nLine, const char* szFormat, const char* szMsg)
{
	gbIsHandlingError = TRUE;
	FOG_Trace("Error (%d) at %s:%d: %s (%s)", nCategory, szFile ? szFile : "", nLine, szMsg ? szMsg : "", szFormat ? szFormat : "");
	if (gpExceptionCallback != nullptr)
	{
		gpExceptionCallback();
	}
	if (nCategory != 2 && nCategory != 5 && nCategory != 6)
	{
		gbIsHandlingError = FALSE;
	}
}

// 1.10f: Fog.0x6FF5E1B0 (#10019)
// 1.12a: Fog.0x6FF615F0 (#10019)
void __fastcall FOG_InitErrorMgr(const char* szProgramName, D2ExceptionCallback pExceptionCallback, const char* szVersion, BOOL bLogToFile)
{
	D2_MAYBE_UNUSED(bLogToFile);
	if (szProgramName != nullptr)
	{
		SStrCopy(gszProgramName, szProgramName, sizeof(gszProgramName));
	}
	if (szVersion != nullptr)
	{
		SStrCopy(gszVersion, szVersion, sizeof(gszVersion));
	}
	gpExceptionCallback = pExceptionCallback;

	if (!gbErrorMgrInitialized)
	{
		gbErrorMgrInitialized = TRUE;
		FOG_CreateNewPoolSystem(nullptr, "Fog_MemorySystem", 0x40000, 0x8000);
	}
}

// 1.10f: Fog.0x6FF5DF20 (#10022)
// 1.12a: Fog.0x6FF60360 (#10022)
void* __cdecl FOG_GetSystemInfo()
{
	return gtFogSystemInfo;
}

// 1.10f: Fog.0x6FF5ED30 (#10023)
// 1.12a: Fog.0x6FF60EC0 (#10023)
void __cdecl FOG_DisplayAssert(const char* szMsg, const char* szFile, int nLine)
{
	WSACleanup();
	ErrorMgr_Report(5, szFile, nLine, "%s", szMsg);
}

// 1.10f: Fog.0x6FF5ED60 (#10024)
// 1.12a: Fog.0x6FF60E90 (#10024)
void __cdecl FOG_DisplayHalt(const char* szMsg, const char* szFile, int nLine)
{
	WSACleanup();
	ErrorMgr_Report(6, szFile, nLine, "%s", szMsg);
}

// 1.10f: Fog.0x6FF5ED90 (#10025)
// 1.12a: Fog.0x6FF60E60 (#10025)
void __cdecl FOG_DisplayWarning(const char* szMsg, const char* szFile, int nLine)
{
	WSACleanup();
	ErrorMgr_Report(7, szFile, nLine, "%s", szMsg);
}

// 1.10f: Fog.0x6FF5EDF0 (#10026)
// 1.12a: Fog.0x6FF60E00 (#10026)
void __cdecl FOG_DisplayError(int nCategory, const char* szMsg, const char* szFile, int nLine)
{
	WSACleanup();
	ErrorMgr_Report(nCategory, szFile, nLine, "%s", szMsg);
}

// 1.10f: Fog.0x6FF5FA10 (#10033)
// 1.12a: Fog.0x6FF6E9E0 (#10033)
void __fastcall FOG_CrashDeadlockDetected(HANDLE hThreadToWalk)
{
	FOG_TraceF("", "Deadlock detected (thread=%p)", hThreadToWalk);
	ErrorMgr_Report(1, __FILE__, __LINE__, "Deadlock detected", "");
}

// 1.10f: Fog.0x6FF51DE0 (#10034)
// 1.12a: Fog.0x6FF6EA20 (#10034)
void __fastcall FOG_CrashDumpThread(HANDLE hThreadToWalk)
{
	FOG_TraceF("", "Crash dump thread (thread=%p)", hThreadToWalk);
}

// 1.10f: Fog.0x6FF5F2A0 (#10039)
// 1.12a: Fog.0x6FF5FA50 (#10039)
BOOL __cdecl FOG_IsHandlingError()
{
	return gbIsHandlingError;
}

// 1.10f: Fog.0x6FF5F300 (#10233)
// 1.12a: Fog.0x6FF6CAE0 (#10233)
void __fastcall FOG_10233(const char* szLogPath, int nUnused)
{
	D2_MAYBE_UNUSED(szLogPath);
	D2_MAYBE_UNUSED(nUnused);
}
