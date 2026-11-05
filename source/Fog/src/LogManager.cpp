#include <Fog.h>
#include <Storm.h>
#include <Windows.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>

static char gszLogPrefix[MAX_PATH] = "D2";

static void LogMgr_WriteFormatted(const char* szFileSubName, BOOL bIncludeDate, BOOL bNewline, const char* szFormat, va_list args)
{
	D2_MAYBE_UNUSED(szFileSubName);
	D2_MAYBE_UNUSED(bIncludeDate);
	if (!szFormat)
	{
		return;
	}

	char szBuffer[4096];
	_vsnprintf(szBuffer, sizeof(szBuffer) - 2, szFormat, args);
	szBuffer[sizeof(szBuffer) - 2] = '\0';

	if (bNewline)
	{
		const size_t nLen = strlen(szBuffer);
		if (nLen == 0 || szBuffer[nLen - 1] != '\n')
		{
			szBuffer[nLen] = '\n';
			szBuffer[nLen + 1] = '\0';
		}
	}

	OutputDebugStringA(szBuffer);
}

// 1.10f: Fog.0x6FF5E1A0 (#10021)
// 1.12a: Fog.0x6FF60610 (#10021)
int __fastcall FOG_SetLogPrefix(const char* szLogPrefix)
{
	if (szLogPrefix != nullptr)
	{
		SStrCopy(gszLogPrefix, szLogPrefix, sizeof(gszLogPrefix));
	}
	return 1;
}

// 1.10f: Fog.0x6FF620A0 (#10029)
// 1.12a: Fog.0x6FF699A0 (#10029)
void __cdecl FOG_Trace(const char* szFormat, ...)
{
	D2_ASSERT(szFormat != nullptr);
	va_list args;
	va_start(args, szFormat);
	LogMgr_WriteFormatted("", TRUE, TRUE, szFormat, args);
	va_end(args);
}

// 1.10f: Fog.0x6FF620E0 (#10030)
// 1.12a: Fog.0x6FF69940 (#10030)
void __cdecl FOG_TraceF(const char* pFileSubName, const char* szFormat, ...)
{
	D2_ASSERT(szFormat != nullptr);
	va_list args;
	va_start(args, szFormat);
	LogMgr_WriteFormatted(pFileSubName ? pFileSubName : "", TRUE, TRUE, szFormat, args);
	va_end(args);
}

// 1.10f: Fog.0x6FF62180 (#10031)
// 1.12a: Fog.0x6FF698E0 (#10031)
void __cdecl FOG_TraceAppend(const char* szFormat, ...)
{
	D2_ASSERT(szFormat != nullptr);
	va_list args;
	va_start(args, szFormat);
	LogMgr_WriteFormatted("", FALSE, FALSE, szFormat, args);
	va_end(args);
}

// 1.10f: Fog.0x6FF62210 (#10032)
// 1.12a: Fog.0x6FF691F0 (#10032)
void __cdecl FOG_TraceMemory(const char* pFileSubName, const char* szMessage, void* pMemory, size_t nByteCount, BOOL bLogOffset, BOOL bNoCopy, BOOL bNoASCII)
{
	D2_MAYBE_UNUSED(pMemory);
	D2_MAYBE_UNUSED(bLogOffset);
	D2_MAYBE_UNUSED(bNoCopy);
	D2_MAYBE_UNUSED(bNoASCII);
	FOG_TraceF(pFileSubName, "%s (%u bytes)", szMessage ? szMessage : "", static_cast<unsigned int>(nByteCount));
}
