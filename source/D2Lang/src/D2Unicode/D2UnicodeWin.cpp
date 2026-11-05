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

#include "D2Unicode.h"

#include <stdlib.h>
#include <string.h>

#include <Archive.h>
#include <Fog.h>

struct D2SysMapEntryStrc
{
	uint16_t wSysChar;
	uint16_t wUniChar;
};

static char sgSysMapName[40] = {};
static uint16_t sgNumSysMapEntries = 0;
static D2SysMapEntryStrc* sgpSysMapTable = nullptr;
static uint8_t sgNumLeadByteRanges = 0;
static uint8_t* sgpLeadByteRanges = nullptr;
static uint16_t* sgpSysMapIndices = nullptr;

// 1.12a: D2Lang.0x6FC081F0
static int __cdecl SysMapCompareUniChar(const void* a, const void* b)
{
	const D2SysMapEntryStrc* pA = (const D2SysMapEntryStrc*)a;
	const D2SysMapEntryStrc* pB = (const D2SysMapEntryStrc*)b;
	return (int)pA->wUniChar - (int)pB->wUniChar;
}

// 1.12a: D2Lang.0x6FC08210
static int __fastcall SysCharToUniChar(uint16_t wSysChar)
{
	if (wSysChar < 0x20)
	{
		return wSysChar;
	}
	int nHigh = (int)sgNumSysMapEntries - 1;
	int nLow = 0;
	if (nHigh < 0)
	{
		return -1;
	}
	if (wSysChar < sgpSysMapTable[sgpSysMapIndices[0]].wSysChar ||
		wSysChar > sgpSysMapTable[sgpSysMapIndices[nHigh]].wSysChar)
	{
		return -1;
	}
	int nMid = nHigh;
	if (wSysChar != sgpSysMapTable[sgpSysMapIndices[nHigh]].wSysChar)
	{
		nMid = nHigh / 2;
		do
		{
			if (wSysChar < sgpSysMapTable[sgpSysMapIndices[nMid]].wSysChar)
			{
				nHigh = nMid;
			}
			else
			{
				nLow = nMid;
			}
			nMid = (nHigh + nLow) / 2;
		} while (nHigh - nLow > 1);

		if (sgpSysMapTable[sgpSysMapIndices[nMid]].wSysChar != wSysChar)
		{
			return -1;
		}
	}
	if (nMid < 0)
	{
		return -1;
	}
	return sgpSysMapTable[sgpSysMapIndices[nMid]].wUniChar;
}

// 1.12a: D2Lang.0x6FC082A0
static int __fastcall UniCharToSysChar(uint16_t wUniChar)
{
	if (wUniChar < 0x20)
	{
		return wUniChar;
	}
	D2SysMapEntryStrc key = { 0, wUniChar };
	const D2SysMapEntryStrc* pFound = (const D2SysMapEntryStrc*)::bsearch(
		&key,
		sgpSysMapTable,
		sgNumSysMapEntries,
		sizeof(D2SysMapEntryStrc),
		SysMapCompareUniChar);
	if (!pFound)
	{
		return -1;
	}
	return pFound->wSysChar;
}

// 1.12a: D2Lang.0x6FC085D0
static BOOL __fastcall IsSingleByteSysChar(uint8_t ch)
{
	if (ch < 0x20)
	{
		return FALSE;
	}
	for (int i = 0; i < (int)sgNumLeadByteRanges; ++i)
	{
		if (ch >= sgpLeadByteRanges[i * 2] && ch <= sgpLeadByteRanges[i * 2 + 1])
		{
			return FALSE;
		}
	}
	return TRUE;
}

// 1.12a: D2Lang.0x6FC082F0 (#10032)
BOOL __fastcall Unicode::loadSysMap(HD2ARCHIVE hArchive, const char* szFilePath)
{
	uint8_t* pFile = (uint8_t*)ARCHIVE_ALLOC_BUFFER_AND_READ_FILE_TO_IT(hArchive, szFilePath, nullptr);
	if (::strncmp((const char*)pFile, "KAMAP\n", 5) != 0)
	{
		D2_FREE(pFile);
		return FALSE;
	}

	int nHeaderLen = 0;
	const uint8_t* pCursor = pFile + 6;
	BOOL bFoundEof = FALSE;
	while (nHeaderLen < 40)
	{
		if (pCursor[nHeaderLen] == 0x1A)
		{
			bFoundEof = TRUE;
			break;
		}
		++nHeaderLen;
	}
	if (!bFoundEof)
	{
		D2_FREE(pFile);
		return FALSE;
	}

	char szHeaderName[40] = {};
	::strncpy(szHeaderName, (const char*)pCursor, nHeaderLen + 1);
	szHeaderName[nHeaderLen] = '\0';
	::strtok(szHeaderName, "\n");

	pCursor += nHeaderLen + 5;
	const uint16_t* pNumEntries = (const uint16_t*)pCursor;
	pCursor += 2;
	const uint8_t* pNumLeadBytes = pCursor;
	pCursor += 1;

	if (sgpLeadByteRanges)
	{
		D2_FREE(sgpLeadByteRanges);
	}
	sgpLeadByteRanges = (uint8_t*)D2_ALLOC((size_t)(*pNumLeadBytes) * 2);
	if (!sgpLeadByteRanges)
	{
		D2_FREE(pFile);
		return FALSE;
	}
	::memcpy(sgpLeadByteRanges, pCursor, (size_t)(*pNumLeadBytes) * 2);
	pCursor += (size_t)(*pNumLeadBytes) * 2;

	if (sgpSysMapTable)
	{
		D2_FREE(sgpSysMapTable);
	}
	sgpSysMapTable = (D2SysMapEntryStrc*)D2_ALLOC((size_t)(*pNumEntries) * sizeof(D2SysMapEntryStrc));
	if (!sgpSysMapTable)
	{
		D2_FREE(pFile);
		return FALSE;
	}
	for (int i = 0; i < (int)(*pNumEntries); ++i)
	{
		sgpSysMapTable[i].wSysChar = pCursor[0];
		sgpSysMapTable[i].wUniChar = pCursor[2];
		pCursor += 4;
	}

	if (sgpSysMapIndices)
	{
		D2_FREE(sgpSysMapIndices);
	}
	sgpSysMapIndices = (uint16_t*)D2_ALLOC((size_t)(*pNumEntries) * sizeof(uint16_t));
	if (!sgpSysMapIndices)
	{
		D2_FREE(pFile);
		return FALSE;
	}
	for (int i = 0; i < (int)(*pNumEntries); ++i)
	{
		sgpSysMapIndices[i] = *(const uint16_t*)pCursor;
		pCursor += 2;
	}

	sgNumSysMapEntries = *pNumEntries;
	sgNumLeadByteRanges = *pNumLeadBytes;
	::strcpy(sgSysMapName, szHeaderName);
	D2_FREE(pFile);
	return TRUE;
}

// 1.12a: D2Lang.0x6FC08560 (#10058)
void __fastcall Unicode::unloadSysMap()
{
	if (sgpLeadByteRanges)
	{
		D2_FREE(sgpLeadByteRanges);
		sgpLeadByteRanges = nullptr;
	}
	if (sgpSysMapTable)
	{
		D2_FREE(sgpSysMapTable);
		sgpSysMapTable = nullptr;
	}
	if (sgpSysMapIndices)
	{
		D2_FREE(sgpSysMapIndices);
		sgpSysMapIndices = nullptr;
	}
}

// 1.12a: D2Lang.0x6FC08610 (#10056)
unsigned long __fastcall Unicode::unicodeWidth(const char* str, int count)
{
	if (!sgpSysMapIndices)
	{
		return (unsigned long)::strlen(str);
	}
	unsigned long width = 0;
	for (int i = 0; i < count && str[i] != '\0'; )
	{
		uint8_t ch = (uint8_t)str[i];
		if (IsSingleByteSysChar(ch))
		{
			i += 2;
		}
		else
		{
			i += 1;
		}
		++width;
	}
	return width;
}

// 1.12a: D2Lang.0x6FC086B0 (#10049)
unsigned long __fastcall Unicode::sysWidth(const Unicode* str, int count)
{
	if (!sgpSysMapTable)
	{
		return (unsigned long)Unicode::strlen(str);
	}
	unsigned long width = 0;
	for (int i = 0; i < count && str[i].ch != L'\0'; ++i)
	{
		int nSysChar = UniCharToSysChar(str[i].ch);
		if (nSysChar >= 0)
		{
			if ((nSysChar & 0xFF00) != 0 || IsSingleByteSysChar((uint8_t)nSysChar))
			{
				width += 2;
			}
			else
			{
				width += 1;
			}
		}
	}
	return width;
}

// 1.12a: D2Lang.0x6FC08760 (#10054)
char* __fastcall Unicode::unicode2Sys(char* dest, const Unicode* src, int count)
{
	if (!sgpSysMapTable)
	{
		return Unicode::unicode2Win(dest, src, count);
	}
	int i_dest = 0;
	for (; i_dest < count && src->ch != L'\0'; ++src)
	{
		int nSysChar = UniCharToSysChar(src->ch);
		if (nSysChar >= 0)
		{
			if ((nSysChar & 0xFF00) != 0 || IsSingleByteSysChar((uint8_t)nSysChar))
			{
				dest[i_dest++] = (char)((nSysChar >> 8) & 0xFF);
			}
			dest[i_dest++] = (char)(nSysChar & 0xFF);
		}
	}
	if (src->ch == L'\0' && i_dest < count)
	{
		dest[i_dest] = '\0';
	}
	return dest;
}

// 1.12a: D2Lang.0x6FC08840 (#10048)
Unicode* __fastcall Unicode::sys2Unicode(Unicode* dest, const char* src, int count)
{
	if (!sgpSysMapIndices)
	{
		return Unicode::win2Unicode(dest, src, count);
	}
	int i_dest = 0;
	while (i_dest < count && *src != '\0')
	{
		uint8_t ch = (uint8_t)*src;
		uint16_t wSysChar;
		if (IsSingleByteSysChar(ch))
		{
			wSysChar = ((uint16_t)ch << 8) | (uint8_t)src[1];
			src += 2;
		}
		else
		{
			wSysChar = ch;
			src += 1;
		}
		int nUniChar = SysCharToUniChar(wSysChar);
		if (nUniChar >= 0)
		{
			dest[i_dest++].ch = (uint16_t)nUniChar;
		}
	}
	if (*src == '\0' && i_dest < count)
	{
		dest[i_dest].ch = L'\0';
	}
	return dest;
}


/**
 * This implementation outputs the same binary from 1.00 and 1.10.
 *
 * 1.00: D2Lang.0x1000109B (#10053)
 * 1.10: D2Lang.0x6FC11C20 (#10055)
 */
char* __fastcall Unicode::unicode2Win(char* dest, const Unicode* src, int count)
{
	char* current_dest = dest;
	while (count > 1 && src->ch != L'\0')
	{
		const Unicode* current_src = src++;
		++current_dest;
		--count;

		*(current_dest - 1) = (char)current_src->ch;
	}

	*current_dest = '\0';
	return dest;
}

/**
 * This implementation outputs the same binary from 1.00.
 *
 * 1.00: D2Lang.0x10001122 (#10059)
 * 1.10: D2Lang.0x6FC11BD0 (#10062)
 */
Unicode* __fastcall Unicode::win2Unicode(Unicode* dest, const char* src, int count)
{
	int i;
	// Copy the source string as-is to the destination.
	for (i = 0; i < count; ++i)
	{
		unsigned char src_ch = src[i];
		if (src_ch == '\0')
		{
			break;
		}
		dest[i].ch = src_ch;
	}

	// Set the null-terminator.
	if (src[i] != '\0')
	{
		dest[i - 1].ch = L'\0';
		return dest;
	}

	dest[i].ch = L'\0';
	return dest;
}
