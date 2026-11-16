#ifndef STORM_STRING_HPP
#define STORM_STRING_HPP

#include <D2Dll.h>
#include <cstdint>
#include <cstdlib>

#define STORM_MAX_PATH 260
#define STORM_MAX_STR 0x7FFFFFFF

STORM_DLL_DECL size_t __stdcall SStrCopy(char* dest, const char* source, size_t destsize); // Storm.#501
STORM_DLL_DECL void __stdcall SStrTokenize(const char** string, char* buffer, size_t bufferchars, const char* delimiter, int32_t* quoted); // Storm.#504
STORM_DLL_DECL size_t __stdcall SStrLen(const char* string); // Storm.#506

#define SStrDupA SStrDup
STORM_DLL_DECL void* __stdcall SStrDup(int a1); // Storm.#507

STORM_DLL_DECL int32_t __stdcall SStrCmp(const char* Str1, const char* Str2, size_t MaxCount); // Storm.#508
STORM_DLL_DECL int32_t __stdcall SStrCmpI(const char* Str1, const char* Str, size_t MaxCount); // Storm.#509
STORM_DLL_DECL char* __stdcall SStrUpper(char* Str); // Storm.#510
STORM_DLL_DECL const char* __stdcall SStrChr(const char* string, char c); // Storm.#571
STORM_DLL_DECL const char* __stdcall SStrChrR(const char* string, char c); // Storm.#572

inline char* SStrChr(char* string, char search) { return const_cast<char*>(SStrChr(static_cast<const char*>(string), search)); }
inline char* SStrChrR(char* string, char search) { return const_cast<char*>(SStrChrR(static_cast<const char*>(string), search)); }

STORM_DLL_DECL int __stdcall SStrLower(char* Str); // Storm.#579
STORM_DLL_DECL int __cdecl SStrPrintf(char* DstBuf, size_t MaxCount, const char* Format, ...); // Storm.#578
STORM_DLL_DECL float __stdcall SStrToFloat(const char* string); // Storm.#574
STORM_DLL_DECL int32_t __stdcall SStrToInt(const char* string); // Storm.#575

inline uint32_t SStrHashHT(const char* string)
{
	if (!string) return 0;
	uint32_t hash = 0;
	while (*string)
	{
		uint8_t ch = static_cast<uint8_t>(*string++);
		if (ch >= 'a' && ch <= 'z') ch = static_cast<uint8_t>(ch - 0x20);
		if (ch == '/') ch = '\\';
		hash = (hash * 33u) ^ ch;
	}
	return hash ? hash : 1u;
}

#endif
