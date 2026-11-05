#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <D2Dll.h>

#undef D2FUNC_DLL_NP
#define D2FUNC_DLL_NP D2FUNC_DLL_NP_STUB

#include <Storm.h>

// 1.12a: 0x6FC082F0 (Storm.#302)
HMODULE __cdecl StormGetInstance()
{
	return GetModuleHandleA("Storm.dll");
}

// 1.12a: 0x6FC24330 (Storm.#491)
int __stdcall SMemCopy(void* dest, const void* source, size_t size)
{
	if (!dest)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (!source && size != 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	std::memcpy(dest, source, size);
	return 1;
}

// 1.12a: 0x6FC24300 (Storm.#492)
int __stdcall SMemFill(void* location, size_t length, char fillWith)
{
	if (!location && length != 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (length != 0)
	{
		std::memset(location, static_cast<unsigned char>(fillWith), length);
	}
	return 1;
}

// 1.12a: 0x6FC242E0 (Storm.#493)
int __stdcall SMemMove(void* Dst, void* Src, size_t Size)
{
	if (!Dst)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (!Src && Size != 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	std::memmove(Dst, Src, Size);
	return 1;
}

// 1.12a: 0x6FC242C0 (Storm.#494)
int __stdcall SMemZero(void* location, size_t length)
{
	if (!location && length != 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (length != 0)
	{
		std::memset(location, 0, length);
	}
	return 1;
}

// 1.12a: 0x6FC24360 (Storm.#495)
int __stdcall SMemCmp(void* location1, void* location2, size_t size)
{
	if (!location1 || !location2)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	return std::memcmp(location1, location2, size);
}

// 1.12a: 0x6FC36758
static const uint32_t gdwStormHash32Table[16] = {
	0x486E26EEu, 0xDCAA16B3u, 0xE1918EEFu, 0x202DAFDBu,
	0x341C7DC7u, 0x1C365303u, 0x40EF2D37u, 0x65FD5E49u,
	0xD6057177u, 0x904ECE93u, 0x1C38024Fu, 0x98FD323Bu,
	0xE3061AE7u, 0xA39B0FA1u, 0x9797F25Fu, 0xE4444563u,
};

// 1.12a: 0x6FC05B50 (Storm.#502)
DWORD __stdcall SStrHash(const char* string, unsigned __int32 flags, unsigned __int32 Seed)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	if (Seed == 0)
	{
		Seed = 0x7FED7FEDu;
	}
	uint32_t dwAdd = 0xEEEEEEEEu;

	while (*string != '\0')
	{
		uint32_t c = static_cast<uint8_t>(*string++);
		if ((flags & 1) == 0)
		{
			if (c >= 'a' && c <= 'z')
			{
				c -= 0x20;
			}
			if (c == '/')
			{
				c = '\\';
			}
		}
		Seed = (Seed + dwAdd) ^ (gdwStormHash32Table[c >> 4] - gdwStormHash32Table[c & 0xF]);
		dwAdd = dwAdd * 33u + c + Seed + 3u;
	}

	return Seed ? Seed : 1u;
}

// 1.12a: 0x6FC05D40 (Storm.#503)
int __stdcall SStrNCat(char* base, const char* append, int max_length)
{
	if (!base || !append || max_length <= 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	const size_t nBaseLen = std::strlen(base);
	if (max_length == 0x7FFFFFFF)
	{
		std::strcat(base + nBaseLen, append);
		return static_cast<int>(nBaseLen + std::strlen(base + nBaseLen));
	}

	if (static_cast<int>(nBaseLen) < max_length - 1)
	{
		const size_t nRemain = static_cast<size_t>((max_length - 1) - static_cast<int>(nBaseLen));
		char* pDst = base + nBaseLen;
		size_t i = 0;
		while (i < nRemain && append[i] != '\0')
		{
			pDst[i] = append[i];
			++i;
		}
		pDst[i] = '\0';
		return static_cast<int>(nBaseLen + i);
	}

	base[max_length - 1] = '\0';
	return max_length - 1;
}

// 1.12a: 0x6FC06A40 (Storm.#505)
char* __stdcall SStrSearch(const char* string, char c, BOOL bReverse)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return nullptr;
	}
	return const_cast<char*>(bReverse ? SStrChrR(string, c) : SStrChr(string, c));
}

// 1.12a: 0x6FC06770 (Storm.#573)
double __stdcall SStrToDouble(const char* string)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0.0;
	}
	return std::atof(string);
}

// 1.12a: 0x6FC05C30 (Storm.#576)
unsigned int __stdcall SStrToUnsigned(const char* string)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	unsigned int result = 0;
	while (*string >= '0' && *string <= '9')
	{
		result = result * 10u + static_cast<unsigned int>(*string - '0');
		++string;
	}
	return result;
}

// 1.12a: 0x6FC064A0 (Storm.#577)
__int64 __stdcall SStrToInt64(const char* string)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	bool bNegative = false;
	if (*string == '-')
	{
		bNegative = true;
		++string;
	}
	else if (*string == '+')
	{
		++string;
	}
	uint64_t val = 0;
	while (*string >= '0' && *string <= '9')
	{
		val = val * 10u + static_cast<uint64_t>(*string - '0');
		++string;
	}
	return bNegative ? -static_cast<__int64>(val) : static_cast<__int64>(val);
}

// 1.12a: 0x6FC06940 (Storm.#581)
int __cdecl SStrVPrintf(char* DstBuf, size_t MaxCount, char* Format, va_list ArgList)
{
	if (!DstBuf || !Format || MaxCount == 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (MaxCount == 0x7FFFFFFF)
	{
		return std::vsprintf(DstBuf, Format, ArgList);
	}
	int nWritten = std::vsnprintf(DstBuf, MaxCount, Format, ArgList);
	DstBuf[MaxCount - 1] = '\0';
	if (nWritten < 0 || static_cast<size_t>(nWritten) >= MaxCount)
	{
		return static_cast<int>(MaxCount - 1);
	}
	return nWritten;
}

#define STORM_JENKINS_MIX(a, b, c) \
	do { \
		a -= b; a -= c; a ^= (c >> 13); \
		b -= c; b -= a; b ^= (a << 8); \
		c -= a; c -= b; c ^= (b >> 13); \
		a -= b; a -= c; a ^= (c >> 12); \
		b -= c; b -= a; b ^= (a << 16); \
		c -= a; c -= b; c ^= (b >> 5); \
		a -= b; a -= c; a ^= (c >> 3); \
		b -= c; b -= a; b ^= (a << 10); \
		c -= a; c -= b; c ^= (b >> 15); \
	} while (0)

// 1.12a: 0x6FC057B0
static uint32_t StormJenkinsHash(const uint8_t* k, uint32_t length, uint32_t initval)
{
	uint32_t a = 0x9E3779B9u;
	uint32_t b = 0x9E3779B9u;
	uint32_t c = initval;
	uint32_t len = length;

	while (len >= 12)
	{
		a += k[0] + (static_cast<uint32_t>(k[1]) << 8) + (static_cast<uint32_t>(k[2]) << 16) + (static_cast<uint32_t>(k[3]) << 24);
		b += k[4] + (static_cast<uint32_t>(k[5]) << 8) + (static_cast<uint32_t>(k[6]) << 16) + (static_cast<uint32_t>(k[7]) << 24);
		c += k[8] + (static_cast<uint32_t>(k[9]) << 8) + (static_cast<uint32_t>(k[10]) << 16) + (static_cast<uint32_t>(k[11]) << 24);
		STORM_JENKINS_MIX(a, b, c);
		k += 12;
		len -= 12;
	}

	c += length;
	switch (len)
	{
	case 11: c += static_cast<uint32_t>(k[10]) << 24;
	case 10: c += static_cast<uint32_t>(k[9]) << 16;
	case 9:  c += static_cast<uint32_t>(k[8]) << 8;
	case 8:  b += static_cast<uint32_t>(k[7]) << 24;
	case 7:  b += static_cast<uint32_t>(k[6]) << 16;
	case 6:  b += static_cast<uint32_t>(k[5]) << 8;
	case 5:  b += k[4];
	case 4:  a += static_cast<uint32_t>(k[3]) << 24;
	case 3:  a += static_cast<uint32_t>(k[2]) << 16;
	case 2:  a += static_cast<uint32_t>(k[1]) << 8;
	case 1:  a += k[0]; break;
	default: break;
	}
	STORM_JENKINS_MIX(a, b, c);
	return c;
}

// 1.12a: 0x6FC06270 (Storm.#582)
uint32_t __stdcall SHashString(const char* szStr)
{
	char szNormalized[1024] = {};
	uint32_t nLen = 0;
	if (szStr)
	{
		while (szStr[nLen] != '\0' && nLen < 1023)
		{
			char ch = szStr[nLen];
			if (ch >= 'a' && ch <= 'z')
			{
				ch -= 0x20;
			}
			else if (ch == '/')
			{
				ch = '\\';
			}
			szNormalized[nLen] = ch;
			++nLen;
		}
	}
	szNormalized[nLen] = '\0';
	return StormJenkinsHash(reinterpret_cast<const uint8_t*>(szNormalized), nLen, 0);
}

static void StormBuildRegSubKey(const char* keyname, uint32_t flags, char* szOut, size_t nOutSize)
{
	szOut[0] = '\0';
	if ((flags & SREG_ABSOLUTE) == 0)
	{
		SStrCopy(szOut, (flags & SREG_BATTLE_NET) ? "Software\\Battle.net\\" : "Software\\Blizzard Entertainment\\", nOutSize);
	}
	if (keyname)
	{
		SStrNCat(szOut, keyname, static_cast<int>(nOutSize));
	}
}

// 1.12a: 0x6FC0C960 (Storm.#427)
BOOL __stdcall SRegGetBaseKey(char flags, char* buffer, size_t bufferchars)
{
	if (!buffer || bufferchars == 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	StormBuildRegSubKey("", static_cast<uint8_t>(flags), buffer, bufferchars);
	return TRUE;
}

// 1.12a: 0x6FC0CA60 (Storm.#421)
BOOL __stdcall SRegLoadData(const char* keyname, const char* valuename, size_t nSize, LPBYTE lpData, uint32_t nFlags, LPDWORD lpcbData)
{
	if (!keyname || !valuename || (nSize > 0 && !lpData))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	char szSubKey[260] = {};
	StormBuildRegSubKey(keyname, nFlags, szSubKey, sizeof(szSubKey));

	HKEY hRoots[2] = {};
	int nRoots = 0;
	if ((nFlags & SREG_EXCLUDE_CURRENT_USER) == 0)
	{
		hRoots[nRoots++] = HKEY_CURRENT_USER;
	}
	if ((nFlags & SREG_EXCLUDE_LOCAL_MACHINE) == 0)
	{
		hRoots[nRoots++] = HKEY_LOCAL_MACHINE;
	}

	for (int i = 0; i < nRoots; ++i)
	{
		HKEY hKey = nullptr;
		if (RegOpenKeyExA(hRoots[i], szSubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			DWORD dwType = 0;
			DWORD cbData = static_cast<DWORD>(nSize);
			const LSTATUS status = RegQueryValueExA(hKey, valuename, nullptr, &dwType, lpData, &cbData);
			RegCloseKey(hKey);
			if (lpcbData)
			{
				*lpcbData = cbData;
			}
			if (status == ERROR_SUCCESS)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

// 1.12a: 0x6FC0CBC0 (Storm.#422)
BOOL __stdcall SRegLoadString(const char* keyname, const char* valuename, uint32_t nFlags, LPSTR pBuffer, size_t nBufferSize)
{
	if (!keyname || !valuename || !pBuffer || nBufferSize == 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	DWORD cbRead = 0;
	if (SRegLoadData(keyname, valuename, nBufferSize, reinterpret_cast<LPBYTE>(pBuffer), nFlags, &cbRead))
	{
		pBuffer[nBufferSize - 1] = '\0';
		return TRUE;
	}
	pBuffer[0] = '\0';
	return FALSE;
}

// 1.12a: 0x6FC0CCA0 (Storm.#423)
BOOL __stdcall SRegLoadValue(const char* keyname, const char* valuename, uint32_t nFlags, DWORD* value)
{
	if (!keyname || !valuename || !value)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	DWORD cbRead = 0;
	return SRegLoadData(keyname, valuename, sizeof(DWORD), reinterpret_cast<LPBYTE>(value), nFlags, &cbRead);
}

static BOOL StormRegSaveInternal(const char* keyname, const char* valuename, BYTE flags, DWORD dwType, const BYTE* lpData, DWORD cbData)
{
	if (!keyname || !valuename || !lpData)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	char szSubKey[260] = {};
	StormBuildRegSubKey(keyname, flags, szSubKey, sizeof(szSubKey));

	HKEY hRoot = (flags & SREG_EXCLUDE_CURRENT_USER) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
	HKEY hKey = nullptr;
	DWORD dwDisposition = 0;
	if (RegCreateKeyExA(hRoot, szSubKey, 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, &dwDisposition) != ERROR_SUCCESS)
	{
		return FALSE;
	}

	const LSTATUS status = RegSetValueExA(hKey, valuename, 0, dwType, lpData, cbData);
	RegCloseKey(hKey);
	return status == ERROR_SUCCESS;
}

// 1.12a: 0x6FC0CD50 (Storm.#424)
BOOL __stdcall SRegSaveData(char* keyname, char* valuename, BYTE flags, BYTE* lpData, DWORD cbData)
{
	return StormRegSaveInternal(keyname, valuename, flags, REG_BINARY, lpData, cbData);
}

// 1.12a: 0x6FC0CEC0 (Storm.#425)
BOOL __stdcall SRegSaveString(const char* keyname, const char* valuename, BYTE flags, const char* string)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	return StormRegSaveInternal(keyname, valuename, flags, REG_SZ, reinterpret_cast<const BYTE*>(string), static_cast<DWORD>(std::strlen(string) + 1));
}

// 1.12a: 0x6FC0CFA0 (Storm.#426)
BOOL __stdcall SRegSaveValue(const char* keyname, const char* valuename, BYTE flags, DWORD result)
{
	return StormRegSaveInternal(keyname, valuename, flags, REG_DWORD, reinterpret_cast<const BYTE*>(&result), sizeof(DWORD));
}

// 1.12a: 0x6FC0CB30 (Storm.#428)
BOOL __stdcall SRegDeleteValue(char* keyname, char* valuename, HKEY phkResult)
{
	if (!keyname || !valuename)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	const uint32_t nFlags = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(phkResult) & 0xFFu);
	char szSubKey[260] = {};
	StormBuildRegSubKey(keyname, nFlags, szSubKey, sizeof(szSubKey));

	HKEY hRoot = (nFlags & SREG_EXCLUDE_CURRENT_USER) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
	HKEY hKey = nullptr;
	if (RegOpenKeyExA(hRoot, szSubKey, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	const LSTATUS status = RegDeleteValueA(hKey, valuename);
	RegCloseKey(hKey);
	return status == ERROR_SUCCESS;
}

// 1.12a: 0x6FC354F8
static const uint8_t TrailingBytesForUTF8[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5,
};

// 1.12a: 0x6FC354E0
static const uint32_t OffsetsFromUTF8[6] = {
	0x00000000u, 0x00003080u, 0x000E2080u,
	0x03C82080u, 0xFA082080u, 0x82082080u,
};

// 1.12a: 0x6FC14110 (Storm.#904)
int __stdcall SUniConvertUTF8to16(wchar_t* pBuffer, int nBufferCount, const char* szSrc, int nSrcCount, int* pOutLength, int a6)
{
	int* pBytesConsumed = reinterpret_cast<int*>(static_cast<uintptr_t>(a6));
	if (pBytesConsumed)
	{
		*pBytesConsumed = 0;
	}
	if (!pOutLength)
	{
		return -1;
	}
	*pOutLength = 0;
	if (!pBuffer || nBufferCount < 0 || !szSrc || nSrcCount < 0)
	{
		return -1;
	}

	const uint8_t* source = reinterpret_cast<const uint8_t*>(szSrc);
	const uint8_t* const sourceStart = source;
	const uint8_t* const sourceEnd = source + nSrcCount;
	wchar_t* target = pBuffer;
	wchar_t* const targetStart = target;
	wchar_t* const targetEnd = target + nBufferCount;

	int result = 0;
	while (source < sourceEnd && *source != 0)
	{
		uint32_t ch = 0;
		const uint16_t extraBytesToRead = TrailingBytesForUTF8[*source];
		if (source + extraBytesToRead >= sourceEnd)
		{
			result = 1;
			break;
		}
		bool bNullInMiddle = false;
		for (uint16_t i = 1; i <= extraBytesToRead; ++i)
		{
			if (source[i] == 0)
			{
				bNullInMiddle = true;
				break;
			}
		}
		if (bNullInMiddle)
		{
			result = 1;
			break;
		}

		switch (extraBytesToRead)
		{
		case 5: ch += *source++; ch <<= 6;
		case 4: ch += *source++; ch <<= 6;
		case 3: ch += *source++; ch <<= 6;
		case 2: ch += *source++; ch <<= 6;
		case 1: ch += *source++; ch <<= 6;
		case 0: ch += *source++; break;
		}
		ch -= OffsetsFromUTF8[extraBytesToRead];

		if (target >= targetEnd)
		{
			result = 2;
			break;
		}

		if (ch <= 0xFFFFu)
		{
			*target++ = static_cast<wchar_t>(ch);
		}
		else if (ch > 0x10FFFFu)
		{
			*target++ = static_cast<wchar_t>(0xFFFDu);
		}
		else
		{
			if (target + 1 >= targetEnd)
			{
				result = 2;
				break;
			}
			ch -= 0x0010000u;
			*target++ = static_cast<wchar_t>((ch >> 10) + 0xD800u);
			*target++ = static_cast<wchar_t>((ch & 0x3FFu) + 0xDC00u);
		}
	}

	if (result == 0)
	{
		if (source < sourceEnd && *source == 0)
		{
			if (target < targetEnd)
			{
				++source;
				*target = L'\0';
			}
			else
			{
				result = 2;
			}
		}
	}

	if (pBytesConsumed)
	{
		*pBytesConsumed = static_cast<int>(source - sourceStart);
	}
	*pOutLength = static_cast<int>(target - targetStart);
	return result;
}
