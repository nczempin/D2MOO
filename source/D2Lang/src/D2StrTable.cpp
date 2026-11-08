/**
 * D2MOO
 * Copyright (c) 2020-2022  The Phrozen Keep community
 *
 * This file belongs to D2MOO.
 * File authors: Mir Drualga
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "D2StrTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <Archive.h>
#include <D2Crc16.h>
#include <D2Constants.h>
#include <D2Lang.h>
#include <D2Unicode.h>
#include <Fog.h>

struct D2LanguageInfoStrc
{
	const char* szCode1;
	const char* szLangSubdir;
	const char* szFontSubdir;
	const char* szCode2;
	uint32_t dwLocaleTag;
	D2C_Language nLanguage;
};

// 1.12a: D2Lang.0x6FC0FFB0
static const D2LanguageInfoStrc sgLanguageTable[13] = {
	{ "ENU", "ENG", "ENG", "USA", 0x656E5553, LANGUAGE_ENGLISH },
	{ "ESP", "ESP", "ESP", "ESP", 0x65734553, LANGUAGE_SPANISH },
	{ "DEU", "DEU", "DEU", "DEU", 0x64654445, LANGUAGE_GERMAN },
	{ "FRA", "FRA", "FRA", "FRA", 0x66724652, LANGUAGE_FRENCH },
	{ "POR", "POR", "POR", "BRA", 0x70744252, LANGUAGE_PORTUGUESE },
	{ "ITA", "ITA", "ITA", "ITA", 0x69744954, LANGUAGE_ITALIAN },
	{ "JPN", "JPN", "ENG", "JPN", 0x6A614A41, LANGUAGE_JAPANESE },
	{ "KOR", "KOR", "ENG", "HAN", 0x6B6F4B52, LANGUAGE_KOREAN },
	{ "SIN", "SIN", "ENG", "SIN", 0x7A68434E, LANGUAGE_CHINESESIN },
	{ "CHI", "CHI", "ENG", "CHI", 0x7A685457, LANGUAGE_CHINESETWN },
	{ "POL", "POL", "POL", "POL", 0x706C504C, LANGUAGE_POLISH },
	{ "RUS", "RUS", "RUS", "RUS", 0x72755255, LANGUAGE_RUSSIAN },
	{ "ENU", "ENG", "ENG", "HAN", 0x6B6F4B52, LANGUAGE_ENGLISHKOR },
};

struct D2MissingStringNodeStrc
{
	Unicode* pUnicode;
	char* szKey;
	D2MissingStringNodeStrc* pNext;
};

static D2MissingStringNodeStrc* gpMissingStringList = nullptr;
static D2TblHeaderStrc* gpStringTable = nullptr;
static Unicode** gpStringTblArray = nullptr;
static Unicode** gpPatchStringTblArray = nullptr;
static Unicode** gpExpansionStringTblArray = nullptr;
static Unicode* gpStringTblBuffer = nullptr;
static Unicode* gpPatchStringTblBuffer = nullptr;
static Unicode* gpExpansionStringTblBuffer = nullptr;
static D2TblHeaderStrc* gpPatchStringTable = nullptr;
static D2TblHeaderStrc* gpExpansionStringTable = nullptr;
static int32_t gnTableRefCount = 0;
static Unicode sgEmptyUnicode = { 0 };
static Unicode sgShortUnitChars[4] = { L'K', L'M', L'B', L'T' };

/**
 * This variable is never set.
 *
 * 1.00: D2Lang.0x100175EC
 * 1.10: D2Lang.0x6FC20CDC
 * 1.13c: D2Lang.0x6FC10A8C
 * 1.14c: Game.0x00879A64
 */
static HD2ARCHIVE ghArchive = nullptr;

/**
 * 1.00: D2Lang.0x100175F4
 * 1.10: D2Lang.0x6FC20CE8
 * 1.13c: D2Lang.0x6FC10A20
 * 1.14c: Game.0x00879A78
 */
static bool gbTableLanguageInitialized = false;

/**
 * 1.00: D2Lang.0x100175F8
 * 1.10: D2Lang.0x6FC20CEC
 * 1.13c: D2Lang.0x6FC10A1C
 * 1.14c: Game.0x00879A74
 */
static D2C_Language gnTableLanguage = LANGUAGE_ENGLISH;

inline static void GroupCStrDigits(Unicode* pUnicode, const char* pcSrc, int nMaxLength)
{
	int nBufferLength = strlen(pcSrc);

	int nUnicodeLength = ((nBufferLength - 1) / 3) + nBufferLength;
	if (nBufferLength == 0 || nUnicodeLength <= 0 || nUnicodeLength >= nMaxLength - 1)
	{
		Unicode asteriskStr[2];
		Unicode::toUnicode(asteriskStr, "*", 2);
		Unicode::strcpy(pUnicode, asteriskStr);
		return;
	}

	pUnicode[nUnicodeLength] = '\0';
	int nDistanceToComma = 0;
	// Like C++ reverse iterators, i and j point to the index after the
	// insertion index.
	// i = destination index (naming is to preserve accurate assert msg)
	// j = source index
	for (int i = nUnicodeLength, j = nBufferLength; j > 0; --i, --j)
	{
		D2_ASSERT(i);

		pUnicode[i - 1] = pcSrc[j - 1];
		++nDistanceToComma;
		if (nDistanceToComma < 3)
		{
			continue;
		}

		// Inefficient: Calls strlen again.
		if (strlen(pcSrc) <= 3 || i - 1 <= 0)
		{
			continue;
		}

		nDistanceToComma = 0;
		pUnicode[i - 2] = L',';
		--i;
	}
}

/**
 * 1.00: D2Lang.0x100010B9 (#10007)
 * 1.10: D2Lang.0x6FC13DF0 (#10007)
 * 1.13c: D2Lang.0x6FC09290 (#10009)
 * 1.14c: Game.0x00522610
 * 1.14d: Game.0x00525150
 */
D2C_Language STRTABLE_GetLanguage()
{
	if (gbTableLanguageInitialized)
	{
		return gnTableLanguage;
	}
	gbTableLanguageInitialized = true;
	// Read locale from file.
	unsigned char* pBuffer = (unsigned char*) ARCHIVE_ALLOC_BUFFER_AND_READ_FILE_TO_IT(ghArchive, "data\\local\\use", nullptr);
	if (pBuffer)
	{
		gnTableLanguage = static_cast<D2C_Language>(*pBuffer);
		D2_FREE(pBuffer);
	}

	if (gnTableLanguage >= NUM_LANGUAGES)
	{
		gnTableLanguage = LANGUAGE_ENGLISH;
	}

	return gnTableLanguage;
}

/**
 * 1.00: D2Lang.0x100010AF (#10009)
 * 1.10: D2Lang.0x6FC140E0 (#10009)
 * 1.13c: D2Lang.0x6FC09BC0 (#10002)
 * 1.14c: Game.0x00523510
 */
void __stdcall STR_GroupIntDigits(Unicode* pUnicode, int nValue, int nMaxLength)
{
	D2_ASSERT(nMaxLength > 2);

	if (nValue < 0)
	{
		pUnicode[0] = L'-';
		++pUnicode;
		nMaxLength -= 1;
		nValue = -nValue;
	}

	char chBuffer[64];
	_itoa(nValue, chBuffer, 10);

	GroupCStrDigits(pUnicode, chBuffer, nMaxLength);
}

/**
 * 1.00: D2Lang.0x100010DC (#10010)
 * 1.10: D2Lang.0x6FC14210 (#10010)
 * 1.13c: D2Lang.0x6FC09A80 (#10006)
 * 1.14c: Game.0x005228A0
 */
void __stdcall STR_GroupUintDigits(Unicode* pUnicode, unsigned int dwValue, int nMaxLength)
{
	D2_ASSERT(nMaxLength > 2);

	char chBuffer[64];
	_ultoa(dwValue, chBuffer, 10);

	GroupCStrDigits(pUnicode, chBuffer, nMaxLength);
}

// 1.12a: D2Lang.0x6FC09800 (#10000)
void __stdcall STR_GroupInt64Digits(Unicode* pUnicode, int64_t nValue, int nMaxLength)
{
	D2_ASSERT(nMaxLength > 2);

	if (nValue < 0)
	{
		pUnicode[0] = L'-';
		++pUnicode;
		nMaxLength -= 1;
		nValue = -nValue;
	}

	char chBuffer[64];
	_i64toa(nValue, chBuffer, 10);

	GroupCStrDigits(pUnicode, chBuffer, nMaxLength);
}

// 1.12a: D2Lang.0x6FC09650 (#10006)
void __stdcall STR_ShortenUint64Digits(Unicode* pUnicode, uint64_t qwValue, int nMaxLength)
{
	D2_ASSERT(nMaxLength > 2);

	char chBuffer[64];
	_ui64toa(qwValue, chBuffer, 10);
	int nBufferLength = (int)strlen(chBuffer);
	if (nBufferLength > 0)
	{
		int nShift = ((nBufferLength - 1) / 3) - 1;
		if (nShift >= 4)
		{
			nShift = 4;
		}
		else if (nShift < 0)
		{
			nShift = 0;
		}

		nBufferLength -= nShift * 3;
		int nUnicodeLength = ((nBufferLength - 1) / 3) + nBufferLength;
		if (nUnicodeLength >= 1 && nUnicodeLength < nMaxLength - 2)
		{
			pUnicode[nUnicodeLength + 1].ch = L'\0';
			switch (nShift)
			{
			case 0:
				pUnicode[nUnicodeLength].ch = L'\0';
				break;
			case 1:
				pUnicode[nUnicodeLength].ch = sgShortUnitChars[0].ch;
				break;
			case 2:
				pUnicode[nUnicodeLength].ch = sgShortUnitChars[1].ch;
				break;
			case 3:
				pUnicode[nUnicodeLength].ch = sgShortUnitChars[2].ch;
				break;
			case 4:
				pUnicode[nUnicodeLength].ch = sgShortUnitChars[3].ch;
				break;
			default:
				break;
			}

			int nDistanceToComma = 0;
			for (int i = nUnicodeLength, j = nBufferLength; j > 0; --i, --j)
			{
				D2_ASSERT(i);
				pUnicode[i - 1].ch = (unsigned char)chBuffer[j - 1];
				++nDistanceToComma;
				if (nDistanceToComma >= 3)
				{
					nDistanceToComma = 0;
					if (i - 1 > 0)
					{
						--i;
						pUnicode[i - 1].ch = L',';
					}
				}
			}
			return;
		}
	}

	Unicode asteriskStr[2] = {};
	Unicode::toUnicode(asteriskStr, "*", 2);
	Unicode::strcpy(pUnicode, asteriskStr);
}

// 1.12a: D2Lang.0x6FC08C20
static D2C_Language LookupLanguageByCode1(const char* szCode)
{
	for (int i = 0; i < 13; ++i)
	{
		if (::_stricmp(szCode, sgLanguageTable[i].szCode1) == 0)
		{
			return sgLanguageTable[i].nLanguage;
		}
	}
	return LANGUAGE_ENGLISH;
}

// 1.12a: D2Lang.0x6FC08EB0 (#10001)
// 1.14c: Game.0x00522670
// 1.14d: Game.0x005251C0
D2C_Language __stdcall STRTABLE_GetSystemLanguage()
{
#if !defined(D2_VERSION_114D)
	OSVERSIONINFOA osvi = {};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	if (!GetVersionExA(&osvi))
	{
		return LANGUAGE_ENGLISH;
	}

	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		char szLang[8] = {};
		GetProfileStringA("Intl", "sLanguage", "ENG", szLang, 4);
		return LookupLanguageByCode1(szLang);
	}
	else if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
#endif
	{
		HKEY hKey = nullptr;
		if (RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\International", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			char szLang[256] = {};
			DWORD dwType = 0;
			DWORD dwSize = sizeof(szLang);
			if (RegQueryValueExA(hKey, "sLanguage", nullptr, &dwType, (LPBYTE)szLang, &dwSize) == ERROR_SUCCESS)
			{
				size_t nLen = strlen(szLang);
				if (nLen > 0 && nLen < 4)
				{
					RegCloseKey(hKey);
					return LookupLanguageByCode1(szLang);
				}
			}
			RegCloseKey(hKey);
		}
	}
	return LANGUAGE_ENGLISH;
}

// 1.12a: D2Lang.0x6FC09BC0 (#10004)
// 1.14c: Game.0x005227A0
// 1.14d: Game.0x00525260
void __stdcall STRTABLE_GetLanguageInfo(void* pOut, int nMode)
{
	D2C_Language nLang = STRTABLE_GetLanguage();
	D2_ASSERT(sgLanguageTable[0].nLanguage == LANGUAGE_ENGLISH);

	int nIdx = 0;
	if (nLang < NUM_LANGUAGES)
	{
		for (int i = 0; i < 13; ++i)
		{
			if (sgLanguageTable[i].nLanguage == nLang)
			{
				nIdx = i;
				break;
			}
		}
	}

	switch (nMode)
	{
	case 1:
		strcpy((char*)pOut, sgLanguageTable[nIdx].szFontSubdir);
		break;
	case 2:
		strcpy((char*)pOut, sgLanguageTable[nIdx].szCode2);
		break;
	case 3:
		*(uint32_t*)pOut = sgLanguageTable[nIdx].dwLocaleTag;
		break;
	default:
		strcpy((char*)pOut, sgLanguageTable[nIdx].szLangSubdir);
		break;
	}
}

// 1.12a: D2Lang.0x6FC08D10 (#10011)
uint32_t __stdcall STRTABLE_HashString(const char* szString, int nHashTableSize)
{
	uint32_t dwHash = 0;
	for (char ch = *szString; ch != '\0'; ch = *++szString)
	{
		dwHash = (dwHash << 4) + (int)ch;
		uint32_t dwHigh = dwHash & 0xF0000000u;
		if (dwHigh != 0)
		{
			dwHash = (dwHash ^ (dwHigh >> 24)) & 0x0FFFFFFFu;
		}
	}
	return dwHash % (uint32_t)nHashTableSize;
}

// 1.12a: D2Lang.0x6FC08D80
static const Unicode* __fastcall STRTABLE_GetNodeStringByIndex(D2TblHeaderStrc* pTbl, uint16_t nIndex, Unicode** ppArray)
{
	if (nIndex >= pTbl->nNumElements)
	{
		nIndex = 500;
	}
	uint16_t* pIndices = (uint16_t*)(pTbl + 1);
	D2TblNodeStrc* pNodes = (D2TblNodeStrc*)(pIndices + pTbl->nNumElements);
	int nNodeIdx = pIndices[nIndex];
	D2_ASSERT(nNodeIdx < pTbl->nHashTableSize);
	D2TblNodeStrc* pNode = &pNodes[nNodeIdx];
	D2_ASSERT((uint8_t*)pNode < (uint8_t*)pTbl + pTbl->dwFileSize);
	D2_ASSERT(pNode->bUsed == 1);
	const Unicode* pStr = ppArray[nNodeIdx];
	D2_ASSERT(pStr != nullptr);
	return pStr;
}

// 1.12a: D2Lang.0x6FC093A0
static D2TblHeaderStrc* __stdcall STRTABLE_LoadTblHeader(const char* szPath)
{
	D2TblHeaderStrc* pTbl = (D2TblHeaderStrc*)ARCHIVE_ALLOC_BUFFER_AND_READ_FILE_TO_IT(ghArchive, szPath, nullptr);
	uint16_t wCrc = 0;
	CRC16_CalculateChecksum(&wCrc, (const uint8_t*)pTbl, pTbl->dwIndexStart, pTbl->dwFileSize);
	D2_ASSERT(wCrc == pTbl->nCRC);

	uint16_t* pIndices = (uint16_t*)(pTbl + 1);
	D2TblNodeStrc* pNodes = (D2TblNodeStrc*)(pIndices + pTbl->nNumElements);
	for (int i = 0; i < pTbl->nHashTableSize; ++i)
	{
		D2_ASSERT(pNodes[i].dwStringOffset < pTbl->dwFileSize);
		D2_ASSERT(pNodes[i].dwKeyOffset < pTbl->dwFileSize);
	}
	return pTbl;
}

// 1.12a: D2Lang.0x6FC09450
static void STRTABLE_InitShortUnitChars()
{
	static const uint16_t kUnitIndices[4] = { 0x14D0, 0x14D1, 0x14D2, 0x14D3 };
	static const Unicode kDefaultUnits[4] = { L'K', L'M', L'B', L'T' };
	for (int i = 0; i < 4; ++i)
	{
		D2_ASSERT(gpStringTable != nullptr);
		const Unicode* pStr = STRTABLE_GetNodeStringByIndex(gpStringTable, kUnitIndices[i], gpStringTblArray);
		char szBuf[8] = {};
		Unicode::unicode2Win(szBuf, pStr, 5);
		if (strlen(szBuf) > 1)
		{
			sgShortUnitChars[i] = kDefaultUnits[i];
		}
		else
		{
			sgShortUnitChars[i].ch = (unsigned char)szBuf[0];
		}
	}
}

static void STRTABLE_PopulateUnicodeTable(D2TblHeaderStrc* pTbl, Unicode*** pppArray, Unicode** ppBuffer)
{
	int nCount = pTbl->nHashTableSize;
	Unicode** ppArray = (Unicode**)D2_CALLOC(nCount * sizeof(Unicode*));
	*pppArray = ppArray;

	uint16_t* pIndices = (uint16_t*)(pTbl + 1);
	D2TblNodeStrc* pNodes = (D2TblNodeStrc*)(pIndices + pTbl->nNumElements);

	size_t nTotalChars = 0;
	for (int i = 0; i < nCount; ++i)
	{
		const char* szStr = (const char*)pTbl + pNodes[i].dwKeyOffset;
		nTotalChars += Unicode::unicodeWidth(szStr, pNodes[i].nNameLen) + 1;
	}

	Unicode* pBuf = (Unicode*)D2_ALLOC(nTotalChars * sizeof(Unicode));
	*ppBuffer = pBuf;

	Unicode* pCursor = pBuf;
	for (int i = 0; i < nCount; ++i)
	{
		ppArray[i] = pCursor;
		const char* szStr = (const char*)pTbl + pNodes[i].dwKeyOffset;
		size_t nLen = Unicode::unicodeWidth(szStr, pNodes[i].nNameLen) + 1;
		Unicode::toUnicode(pCursor, szStr, (int)nLen);
		pCursor += nLen;
	}
}

// 1.12a: D2Lang.0x6FC09E60 (#10003)
BOOL __fastcall STRTABLE_Initialize(int nUnused, const char* szLanguage, BOOL bExpansion)
{
	D2_MAYBE_UNUSED(nUnused);
	if (gpStringTable != nullptr)
	{
		++gnTableRefCount;
		return TRUE;
	}

	char szLangCode[16] = {};
	if (szLanguage != nullptr && szLanguage[0] != '\0')
	{
		strcpy(szLangCode, szLanguage);
	}
	else
	{
		STRTABLE_GetLanguageInfo(szLangCode, 0);
	}

	char szFontSubdir[16] = {};
	if (::_stricmp(szLangCode, "JPN") == 0)
	{
		strcpy(szFontSubdir, "JPN");
	}
	else if (::_stricmp(szLangCode, "KOR") == 0)
	{
		strcpy(szFontSubdir, "KOR");
	}
	else if (::_stricmp(szLangCode, "POL") == 0)
	{
		strcpy(szFontSubdir, "LATIN2");
	}
	else if (::_stricmp(szLangCode, "RUS") == 0)
	{
		strcpy(szFontSubdir, "CYR");
	}
	else
	{
		strcpy(szFontSubdir, "LATIN");
	}

	char szMapPath[MAX_PATH] = {};
	sprintf(szMapPath, "data\\local\\font\\%s\\default.map", szFontSubdir);
	Unicode::loadSysMap(ghArchive, szMapPath);

	char szTblPath[MAX_PATH] = {};
	sprintf(szTblPath, "data\\local\\lng\\%s\\string.tbl", szLangCode);
	gpStringTable = STRTABLE_LoadTblHeader(szTblPath);
	if (!gpStringTable)
	{
		return FALSE;
	}
	++gnTableRefCount;
	STRTABLE_PopulateUnicodeTable(gpStringTable, &gpStringTblArray, &gpStringTblBuffer);

	sprintf(szTblPath, "data\\local\\lng\\%s\\patchstring.tbl", szLangCode);
	gpPatchStringTable = STRTABLE_LoadTblHeader(szTblPath);
	D2_ASSERT(gpPatchStringTable != nullptr);
	STRTABLE_PopulateUnicodeTable(gpPatchStringTable, &gpPatchStringTblArray, &gpPatchStringTblBuffer);

	if (bExpansion)
	{
		sprintf(szTblPath, "data\\local\\lng\\%s\\expansionstring.tbl", szLangCode);
		gpExpansionStringTable = STRTABLE_LoadTblHeader(szTblPath);
		D2_ASSERT(gpExpansionStringTable != nullptr);
		STRTABLE_PopulateUnicodeTable(gpExpansionStringTable, &gpExpansionStringTblArray, &gpExpansionStringTblBuffer);
	}

	STRTABLE_InitShortUnitChars();
	return TRUE;
}

// 1.12a: D2Lang.0x6FC09090 (#10009)
void __stdcall STRTABLE_Destroy()
{
	if (gpStringTable == nullptr || --gnTableRefCount != 0)
	{
		return;
	}

	if (gpStringTblArray != nullptr)
	{
		D2_FREE(gpStringTblBuffer);
		gpStringTblBuffer = nullptr;
		D2_FREE(gpStringTblArray);
		gpStringTblArray = nullptr;
	}
	D2_FREE(gpStringTable);
	gpStringTable = nullptr;

	if (gpPatchStringTblArray != nullptr)
	{
		D2_FREE(gpPatchStringTblArray);
		gpPatchStringTblArray = nullptr;
	}
	if (gpPatchStringTblBuffer != nullptr)
	{
		D2_FREE(gpPatchStringTblBuffer);
		gpPatchStringTblBuffer = nullptr;
	}
	D2_FREE(gpPatchStringTable);
	gpPatchStringTable = nullptr;

	if (gpExpansionStringTblArray != nullptr)
	{
		D2_FREE(gpExpansionStringTblArray);
		gpExpansionStringTblArray = nullptr;
	}
	if (gpExpansionStringTblBuffer != nullptr)
	{
		D2_FREE(gpExpansionStringTblBuffer);
		gpExpansionStringTblBuffer = nullptr;
	}
	if (gpExpansionStringTable != nullptr)
	{
		D2_FREE(gpExpansionStringTable);
		gpExpansionStringTable = nullptr;
	}

	D2MissingStringNodeStrc* pNode = gpMissingStringList;
	while (pNode != nullptr)
	{
		D2MissingStringNodeStrc* pNext = pNode->pNext;
		D2_FREE(pNode->pUnicode);
		D2_FREE(pNode->szKey);
		D2_FREE(pNode);
		pNode = pNext;
	}
	gpMissingStringList = nullptr;

	Unicode::unloadSysMap();
}

// 1.12a: D2Lang.0x6FC09310 (#10005)
const Unicode* __fastcall D2LANG_GetStringFromTblIndex(short index)
{
	uint16_t uIndex = (uint16_t)index;
	if (uIndex >= 20000)
	{
		if (gpExpansionStringTable == nullptr)
		{
			uIndex = 11078;
		}
		else
		{
			const Unicode* pStr = STRTABLE_GetNodeStringByIndex(gpExpansionStringTable, uIndex - 20000, gpExpansionStringTblArray);
			if (pStr != nullptr)
			{
				return pStr;
			}
		}
	}

	if (gpPatchStringTable != nullptr && uIndex >= 10000)
	{
		const Unicode* pStr = STRTABLE_GetNodeStringByIndex(gpPatchStringTable, uIndex - 10000, gpPatchStringTblArray);
		if (pStr != nullptr)
		{
			return pStr;
		}
	}

	D2_ASSERT(gpStringTable != nullptr);
	return STRTABLE_GetNodeStringByIndex(gpStringTable, uIndex, gpStringTblArray);
}

// 1.12a: D2Lang.0x6FC09220
static short __stdcall STRTABLE_FindInTable(D2TblHeaderStrc* pTbl, Unicode** ppArray, const char* szReference, const Unicode** ppUnicode)
{
	uint16_t* pIndices = (uint16_t*)(pTbl + 1);
	D2TblNodeStrc* pNodes = (D2TblNodeStrc*)(pIndices + pTbl->nNumElements);
	uint32_t dwHash = STRTABLE_HashString(szReference, pTbl->nHashTableSize);

	for (uint32_t i = 0; i < pTbl->dwNumLoops; ++i)
	{
		D2TblNodeStrc* pNode = &pNodes[dwHash];
		if (pNode->bUsed == 1)
		{
			const char* szKey = (const char*)pTbl + pNode->dwStringOffset;
			if (strcmp(szKey, szReference) == 0)
			{
				const Unicode* pStr = ppArray[dwHash];
				D2_ASSERT(pStr != nullptr);
				*ppUnicode = pStr;
				return (short)pNode->nIndexNo;
			}
		}
		dwHash = (dwHash + 1) % (uint32_t)pTbl->nHashTableSize;
	}
	return -1;
}

// 1.12a: D2Lang.0x6FC09CE0
static const Unicode* __fastcall STRTABLE_GetOrAddMissingString(const char* szReference)
{
	for (D2MissingStringNodeStrc* pCur = gpMissingStringList; pCur != nullptr; pCur = pCur->pNext)
	{
		if (strcmp(szReference, pCur->szKey) == 0)
		{
			return pCur->pUnicode;
		}
	}

	D2MissingStringNodeStrc* pNode = D2_ALLOC_STRC(D2MissingStringNodeStrc);
	pNode->pNext = gpMissingStringList;
	gpMissingStringList = pNode;

	size_t nRefLen = strlen(szReference);
	pNode->szKey = (char*)D2_ALLOC(nRefLen + 1);
	strcpy(pNode->szKey, szReference);

	static const char kMissingSuffix[] = " -not xlated call ken w";
	size_t nTotalLen = nRefLen + strlen(kMissingSuffix);
	char* szCombined = (char*)D2_ALLOC(nTotalLen + 1);
	strcpy(szCombined, szReference);
	strcat(szCombined, kMissingSuffix);

	unsigned long nUniLen = Unicode::unicodeWidth(szCombined, (int)nTotalLen);
	pNode->pUnicode = (Unicode*)D2_ALLOC((nUniLen + 1) * sizeof(Unicode));
	Unicode::toUnicode(pNode->pUnicode, szCombined, (int)nUniLen);
	D2_FREE(szCombined);

	return pNode->pUnicode;
}

// 1.12a: D2Lang.0x6FC0A3D0 (#10007)
short __stdcall D2LANG_GetTblIndex(char* szReference, const Unicode** pUnicode)
{
	D2_ASSERT(gpStringTable != nullptr);
	D2_ASSERT(gpPatchStringTable != nullptr);
	D2_ASSERT(szReference != nullptr);

	if (szReference[0] == '\0')
	{
		*pUnicode = &sgEmptyUnicode;
		return 0;
	}

	short nIdx = STRTABLE_FindInTable(gpPatchStringTable, gpPatchStringTblArray, szReference, pUnicode);
	if (nIdx != -1)
	{
		return nIdx + 10000;
	}

	if (gpExpansionStringTable != nullptr)
	{
		nIdx = STRTABLE_FindInTable(gpExpansionStringTable, gpExpansionStringTblArray, szReference, pUnicode);
		if (nIdx != -1)
		{
			return nIdx + 20000;
		}
	}

	nIdx = STRTABLE_FindInTable(gpStringTable, gpStringTblArray, szReference, pUnicode);
	if (nIdx != -1)
	{
		return nIdx;
	}

	*pUnicode = STRTABLE_GetOrAddMissingString(szReference);
	return 0;
}

// 1.12a: D2Lang.0x6FC0A5D0 (#10010)
const Unicode* __fastcall D2LANG_GetStringByReferenceString(char* string)
{
	const Unicode* pUnicode = nullptr;
	D2LANG_GetTblIndex(string, &pUnicode);
	return pUnicode;
}
