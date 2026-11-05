#include <Fog.h>
#include <FogTables.h>
#include <D2BitManip.h>
#include <Storm.h>

#include <cstring>

static const char* gszUnknownLinkName = "???";

// 1.12a: 0x6FF64720
static void __fastcall Excel_ToLowerInPlace(char* szText, int nMaxLen)
{
	char* pEnd = szText + nMaxLen;
	while (*szText != '\0' && szText < pEnd)
	{
		*szText = static_cast<char>(gbFogCharLowerTable[static_cast<uint8_t>(*szText)]);
		++szText;
	}
}

// 1.12a: 0x6FF67280
static const char* __fastcall Excel_FindLinkNameByIndex(D2TxtLinkNodeStrc* pNode, int nLinkIndex)
{
	if (!pNode)
	{
		return gszUnknownLinkName;
	}

	if (pNode->nLinkIndex == nLinkIndex)
	{
		return pNode->szText;
	}

	if (pNode->pPrevious)
	{
		const char* szResult = Excel_FindLinkNameByIndex(pNode->pPrevious, nLinkIndex);
		if (szResult != gszUnknownLinkName)
		{
			return szResult;
		}
	}

	if (pNode->pNext)
	{
		const char* szResult = Excel_FindLinkNameByIndex(pNode->pNext, nLinkIndex);
		if (szResult != gszUnknownLinkName)
		{
			return szResult;
		}
	}

	return gszUnknownLinkName;
}

// 1.12a: 0x6FF674B0
static void Excel_InsertLinkNode(D2TxtLinkNodeStrc** ppRoot, D2TxtLinkNodeStrc* pNewNode)
{
	D2TxtLinkNodeStrc** ppCurr = ppRoot;
	while (*ppCurr != nullptr)
	{
		D2TxtLinkNodeStrc* pCurr = *ppCurr;
		const int nCmp = strcmp(pNewNode->szText, pCurr->szText);
		if (nCmp < 0)
		{
			ppCurr = &pCurr->pPrevious;
		}
		else if (nCmp > 0)
		{
			ppCurr = &pCurr->pNext;
		}
		else
		{
			D2_FREE(pNewNode);
			return;
		}
	}
	*ppCurr = pNewNode;
}

// 1.12a: 0x6FF67530
static void __fastcall Excel_FreeLinkNodes(D2TxtLinkNodeStrc* pNode)
{
	if (pNode->pPrevious)
	{
		Excel_FreeLinkNodes(pNode->pPrevious);
	}
	if (pNode->pNext)
	{
		Excel_FreeLinkNodes(pNode->pNext);
	}
	D2_FREE(pNode);
}

// 1.12a: 0x6FF67CC0
static int Excel_GetOrAddLinkNode(D2TxtLinkStrc* pLinker, const char* szText)
{
	D2_ASSERT(pLinker != nullptr);
	D2_ASSERT(pLinker->pTbl == nullptr);

	const int nExisting = FOG_GetRowFromTxt(pLinker, const_cast<char*>(szText), 0);
	if (nExisting >= 0)
	{
		return nExisting;
	}

	D2TxtLinkNodeStrc* pNode = D2_CALLOC_STRC(D2TxtLinkNodeStrc);
	SStrCopy(pNode->szText, szText, sizeof(pNode->szText));
	Excel_ToLowerInPlace(pNode->szText, sizeof(pNode->szText));
	pNode->nLinkIndex = pLinker->nRecords;
	Excel_InsertLinkNode(&pLinker->pFirstNode, pNode);
	return pLinker->nRecords++;
}

// 1.12a: 0x6FF67880
static int __stdcall Excel_MapColumnsToFields(int16_t* pColFieldMap, D2BinFileStrc* pBinFile, D2BinFieldStrc* pBinFields)
{
	for (D2BinFieldStrc* pField = pBinFields; pField->nFieldType != TXTFIELD_NONE; ++pField)
	{
		if (pField->nFieldType == TXTFIELD_UNKNOWN1 && pField->nFieldLength > 1)
		{
			for (int32_t i = 1; i < pField->nFieldLength; ++i)
			{
				++pField;
				D2_ASSERT(pField->nFieldType == TXTFIELD_UNKNOWN1);
			}
		}
	}

	D2UnkExcelStrc* apBuckets[256];
	SMemZero(apBuckets, sizeof(apBuckets));

	D2UnkExcelStrc aNodes[280];
	uint32_t nFieldCount = 0;

	for (D2BinFieldStrc* pField = pBinFields; pField->nFieldType != TXTFIELD_NONE; ++pField)
	{
		D2_ASSERT(nFieldCount < 280);
		D2UnkExcelStrc* pNode = &aNodes[nFieldCount++];
		const uint32_t dwHash = SStrHash(pField->szFieldName, 0, 0);
		const uint32_t nBucket = ((dwHash >> 8) ^ dwHash ^ (((dwHash >> 8) ^ dwHash) >> 16)) & 0xFF;
		pNode->dwHash = dwHash;
		pNode->pBinField = pField;
		pNode->pNext = apBuckets[nBucket];
		apBuckets[nBucket] = pNode;
	}

	int nMappedCount = 0;
	const char* szColName = reinterpret_cast<const char*>(pBinFile->pDataBuffer);
	for (int32_t nCol = 0; nCol < pBinFile->nCellCount; ++nCol)
	{
		const uint32_t dwHash = SStrHash(szColName, 0, 0);
		const uint32_t nBucket = (dwHash ^ (dwHash >> 8) ^ (dwHash >> 16) ^ (dwHash >> 24)) & 0xFF;

		D2UnkExcelStrc* pMatch = nullptr;
		for (D2UnkExcelStrc* pCurr = apBuckets[nBucket]; pCurr != nullptr; pCurr = pCurr->pNext)
		{
			if (pCurr->dwHash == dwHash && SStrCmpI(szColName, pCurr->pBinField->szFieldName, 0x7FFFFFFF) == 0)
			{
				pMatch = pCurr;
				break;
			}
		}

		if (pMatch)
		{
			pMatch->dwHash = 0;
			pColFieldMap[nMappedCount++] = static_cast<int16_t>(pMatch->pBinField - pBinFields);
		}
		else
		{
			pColFieldMap[nMappedCount++] = 0x7FFF;
		}

		while (*szColName != '\0')
		{
			++szColName;
		}
		++szColName;
	}

	for (uint32_t i = 0; i < nFieldCount; ++i)
	{
		if (aNodes[i].dwHash != 0)
		{
			pColFieldMap[nMappedCount++] = static_cast<int16_t>(aNodes[i].pBinField - pBinFields);
		}
	}

	return nMappedCount;
}

static uint32_t Excel_ReadFourCC(const char*& szCell)
{
	char szRaw[4] = { 0, 0, 0, 0 };
	int nIdx = 0;
	while (*szCell != '\0')
	{
		if (nIdx < 4)
		{
			szRaw[nIdx++] = *szCell;
		}
		++szCell;
	}
	++szCell;

	uint8_t aCode[4];
	if (szRaw[0] == '\0')
	{
		aCode[0] = ' ';
		aCode[1] = ' ';
		aCode[2] = ' ';
		aCode[3] = ' ';
	}
	else
	{
		aCode[0] = static_cast<uint8_t>(szRaw[0]);
		if (szRaw[1] == '\0')
		{
			aCode[1] = ' ';
			aCode[2] = ' ';
			aCode[3] = ' ';
		}
		else
		{
			aCode[1] = static_cast<uint8_t>(szRaw[1]);
			if (szRaw[2] == '\0')
			{
				aCode[2] = ' ';
				aCode[3] = ' ';
			}
			else
			{
				aCode[2] = static_cast<uint8_t>(szRaw[2]);
				aCode[3] = (szRaw[3] != '\0') ? static_cast<uint8_t>(szRaw[3]) : ' ';
			}
		}
	}

	uint32_t dwCode = 0;
	memcpy(&dwCode, aCode, sizeof(dwCode));
	return dwCode;
}

// 1.10f: Fog.0x6FF5AA60 (#10207)
// 1.12a: Fog.0x6FF67EF0 (#10207)
void __stdcall FOG_10207(D2BinFileStrc* pBinFile, D2BinFieldStrc* pBinField, void* pTxt, int nRecordCount, int nRecordSize)
{
	D2_ASSERT(static_cast<uint32_t>(nRecordCount) > 0);
	D2_ASSERT(static_cast<uint32_t>(nRecordSize) > 0);

	int16_t aColFieldMap[280];
	const int nTotalMapped = Excel_MapColumnsToFields(aColFieldMap, pBinFile, pBinField);

	BOOL bHasLinkerPass = FALSE;
	for (int32_t nCol = 0; nCol < pBinFile->nCellCount; ++nCol)
	{
		const int16_t nFieldIdx = aColFieldMap[nCol];
		if (nFieldIdx == 0x7FFF)
		{
			continue;
		}
		switch (pBinField[nFieldIdx].nFieldType)
		{
		case TXTFIELD_ASCIITOCODE:
		case TXTFIELD_UNKNOWN4:
		case TXTFIELD_UNKNOWN5:
		case TXTFIELD_UNKNOWN6:
		case TXTFIELD_NAMETOINDEX:
		case TXTFIELD_NAMETOINDEX2:
			bHasLinkerPass = TRUE;
			break;
		default:
			break;
		}
	}

	if (bHasLinkerPass && pBinFile->nRecordCount > 0)
	{
		const char* szCell = reinterpret_cast<const char*>(pBinFile->pData);
		uint8_t* pRecord = static_cast<uint8_t*>(pTxt);

		for (int32_t nRow = 0; nRow < pBinFile->nRecordCount; ++nRow)
		{
			for (int32_t nCol = 0; nCol < pBinFile->nCellCount; ++nCol)
			{
				const int16_t nFieldIdx = aColFieldMap[nCol];
				if (nFieldIdx == 0x7FFF)
				{
					while (*szCell != '\0')
					{
						++szCell;
					}
					++szCell;
					continue;
				}

				D2BinFieldStrc* pField = &pBinField[nFieldIdx];
				uint8_t* pDest = pRecord + pField->nFieldOffset;

				switch (pField->nFieldType)
				{
				case TXTFIELD_ASCIITOCODE:
				case TXTFIELD_UNKNOWN4:
				case TXTFIELD_UNKNOWN5:
				{
					const uint32_t dwCode = Excel_ReadFourCC(szCell);
					const int nLinkIdx = FOG_10215(*reinterpret_cast<void**>(pField->pLinkField), static_cast<int>(dwCode));
					if (pField->nFieldType == TXTFIELD_ASCIITOCODE)
					{
						*reinterpret_cast<uint32_t*>(pDest) = dwCode;
					}
					else if (pField->nFieldType == TXTFIELD_UNKNOWN4)
					{
						D2_ASSERT(nLinkIdx <= 0xFF);
						*pDest = static_cast<uint8_t>(dwCode);
					}
					else if (pField->nFieldType == TXTFIELD_UNKNOWN5)
					{
						D2_ASSERT(nLinkIdx <= 0xFFFF);
						*reinterpret_cast<uint16_t*>(pDest) = static_cast<uint16_t>(dwCode);
					}
					break;
				}
				case TXTFIELD_UNKNOWN6:
				{
					uint8_t* pEnd = pDest + pField->nFieldLength - 1;
					uint8_t* pWrite = pDest;
					while (*szCell != '\0' && pWrite < pEnd)
					{
						*pWrite++ = static_cast<uint8_t>(*szCell++);
					}
					*pWrite = '\0';
					while (*szCell != '\0')
					{
						++szCell;
					}
					++szCell;
					FOG_10216_AddRecordToLinkingTable(*reinterpret_cast<void**>(pField->pLinkField), reinterpret_cast<const char*>(pDest));
					break;
				}
				case TXTFIELD_NAMETOINDEX:
				case TXTFIELD_NAMETOINDEX2:
				{
					char szName[32];
					int nPos = 0;
					while (*szCell != '\0' && nPos < 31)
					{
						szName[nPos++] = *szCell++;
					}
					szName[nPos] = '\0';
					while (*szCell != '\0')
					{
						++szCell;
					}
					++szCell;

					const int nIdx = Excel_GetOrAddLinkNode(*reinterpret_cast<D2TxtLinkStrc**>(pField->pLinkField), szName);
					if (pField->nFieldType == TXTFIELD_NAMETOINDEX)
					{
						*reinterpret_cast<uint16_t*>(pDest) = static_cast<uint16_t>(nIdx);
					}
					else
					{
						*reinterpret_cast<uint32_t*>(pDest) = static_cast<uint32_t>(nIdx);
					}
					break;
				}
				default:
					while (*szCell != '\0')
					{
						++szCell;
					}
					++szCell;
					break;
				}
			}

			D2_ASSERT(*szCell == '\n');
			++szCell;
			pRecord += nRecordSize;
		}
	}

	if (pBinFile->nRecordCount <= 0)
	{
		return;
	}

	const char* szCell = reinterpret_cast<const char*>(pBinFile->pData);
	uint8_t* pRecord = static_cast<uint8_t*>(pTxt);
	using CustomFieldCallback = void(__fastcall*)(char* szText, void* pRecord, int nFieldOffset, int nFieldLength, int nRow, int nCol);

	for (int32_t nRow = 0; nRow < pBinFile->nRecordCount; ++nRow)
	{
		for (int32_t nCol = 0; nCol < pBinFile->nCellCount; ++nCol)
		{
			const int16_t nFieldIdx = aColFieldMap[nCol];
			if (nFieldIdx == 0x7FFF)
			{
				while (*szCell != '\0')
				{
					++szCell;
				}
				++szCell;
				continue;
			}

			D2BinFieldStrc* pField = &pBinField[nFieldIdx];
			uint8_t* pDest = pRecord + pField->nFieldOffset;

			switch (pField->nFieldType)
			{
			case TXTFIELD_ASCII:
			case TXTFIELD_BYTE2:
			{
				uint8_t* pEnd = pDest + pField->nFieldLength;
				uint8_t* pWrite = pDest;
				while (*szCell != '\0' && pWrite < pEnd)
				{
					*pWrite++ = static_cast<uint8_t>(*szCell++);
				}
				while (*szCell != '\0')
				{
					++szCell;
				}
				++szCell;
				*pWrite = '\0';
				break;
			}
			case TXTFIELD_DWORD:
			case TXTFIELD_WORD:
			case TXTFIELD_BYTE:
			case TXTFIELD_UNKNOWN1:
			case TXTFIELD_UNKNOWN2:
			case TXTFIELD_DWORD2:
			case TXTFIELD_BIT:
			{
				const char* szStart = szCell;
				if (*szCell == '-')
				{
					++szCell;
				}
				int32_t nValue = 0;
				while (*szCell != '\0')
				{
					nValue = nValue * 10 + (*szCell - '0');
					++szCell;
				}
				if (*szStart == '-')
				{
					nValue = -nValue;
				}
				++szCell;

				switch (pField->nFieldType)
				{
				case TXTFIELD_DWORD:
				case TXTFIELD_DWORD2:
					*reinterpret_cast<int32_t*>(pDest) = nValue;
					break;
				case TXTFIELD_WORD:
					*reinterpret_cast<int16_t*>(pDest) = static_cast<int16_t>(nValue);
					break;
				case TXTFIELD_BYTE:
				case TXTFIELD_UNKNOWN2:
					*reinterpret_cast<int8_t*>(pDest) = static_cast<int8_t>(nValue);
					break;
				case TXTFIELD_UNKNOWN1:
					if (szStart + 1 != szCell)
					{
						*reinterpret_cast<int8_t*>(pDest) = static_cast<int8_t>(nValue);
					}
					break;
				case TXTFIELD_BIT:
				{
					const int32_t nBit = pField->nFieldLength;
					uint8_t* pByte = pDest + (nBit >> 3);
					const uint32_t nBitInByte = nBit & 7;
					if (nValue != 0)
					{
						*pByte |= static_cast<uint8_t>(gdwBitMasks[nBitInByte]);
					}
					else
					{
						*pByte &= static_cast<uint8_t>(gdwInvBitMasks[nBitInByte]);
					}
					break;
				}
				default:
					break;
				}
				break;
			}
			case TXTFIELD_RAW:
			{
				const uint32_t dwCode = Excel_ReadFourCC(szCell);
				memcpy(pDest, &dwCode, sizeof(dwCode));
				break;
			}
			case TXTFIELD_ASCIITOCODE:
			case TXTFIELD_UNKNOWN4:
			case TXTFIELD_UNKNOWN5:
			case TXTFIELD_UNKNOWN6:
			case TXTFIELD_NAMETOINDEX:
			case TXTFIELD_NAMETOINDEX2:
			{
				while (*szCell != '\0')
				{
					++szCell;
				}
				++szCell;
				break;
			}
			case TXTFIELD_UNKNOWN3:
			case TXTFIELD_CODETOBYTE:
			case TXTFIELD_CODETOWORD:
			{
				const uint32_t dwCode = Excel_ReadFourCC(szCell);
				const int nIdx = FOG_GetLinkIndex(*reinterpret_cast<void**>(pField->pLinkField), dwCode, FALSE);
				if (pField->nFieldType == TXTFIELD_UNKNOWN3)
				{
					*reinterpret_cast<int32_t*>(pDest) = nIdx;
				}
				else if (pField->nFieldType == TXTFIELD_CODETOBYTE)
				{
					*reinterpret_cast<int8_t*>(pDest) = static_cast<int8_t>(nIdx);
				}
				else if (pField->nFieldType == TXTFIELD_CODETOWORD)
				{
					*reinterpret_cast<int16_t*>(pDest) = static_cast<int16_t>(nIdx);
				}
				break;
			}
			case TXTFIELD_NAMETODWORD:
			case TXTFIELD_NAMETOWORD:
			case TXTFIELD_NAMETOWORD2:
			{
				char szName[32];
				int nPos = 0;
				while (*szCell != '\0' && nPos < 31)
				{
					szName[nPos++] = *szCell++;
				}
				szName[nPos] = '\0';
				while (*szCell != '\0')
				{
					++szCell;
				}
				++szCell;

				const int nIdx = FOG_GetRowFromTxt(*reinterpret_cast<void**>(pField->pLinkField), szName, 1);
				if (pField->nFieldType == TXTFIELD_NAMETODWORD)
				{
					*reinterpret_cast<int32_t*>(pDest) = nIdx;
				}
				else if (pField->nFieldType == TXTFIELD_NAMETOWORD)
				{
					*reinterpret_cast<int16_t*>(pDest) = static_cast<int16_t>(nIdx);
				}
				else if (pField->nFieldType == TXTFIELD_NAMETOWORD2)
				{
					*reinterpret_cast<int8_t*>(pDest) = static_cast<int8_t>(nIdx);
				}
				break;
			}
			case TXTFIELD_KEYTOWORD:
			{
				char szKey[256];
				int nPos = 0;
				while (*szCell != '\0' && nPos < 255)
				{
					szKey[nPos++] = *szCell++;
				}
				szKey[nPos] = '\0';
				while (*szCell != '\0')
				{
					++szCell;
				}
				++szCell;

				if (pField->pLinkField)
				{
					using KeyToWordCallback = int16_t(__fastcall*)(const char*);
					*reinterpret_cast<int16_t*>(pDest) = reinterpret_cast<KeyToWordCallback>(pField->pLinkField)(szKey);
				}
				break;
			}
			case TXTFIELD_CUSTOMLINK:
			case TXTFIELD_UNKNOWN7:
			case TXTFIELD_CALCTODWORD:
			{
				char szCustom[256];
				int nPos = 0;
				while (*szCell != '\0' && nPos < 255)
				{
					szCustom[nPos++] = *szCell++;
				}
				szCustom[nPos] = '\0';
				while (*szCell != '\0')
				{
					++szCell;
				}
				++szCell;

				if (pField->pLinkField)
				{
					reinterpret_cast<CustomFieldCallback>(pField->pLinkField)(
						szCustom, pRecord, pField->nFieldOffset, pField->nFieldLength, nRow, nCol);
				}
				break;
			}
			default:
				while (*szCell != '\0')
				{
					++szCell;
				}
				++szCell;
				break;
			}
		}

		for (int nUnmatched = pBinFile->nCellCount; nUnmatched < nTotalMapped; ++nUnmatched)
		{
			D2BinFieldStrc* pField = &pBinField[aColFieldMap[nUnmatched]];
			uint8_t* pDest = pRecord + pField->nFieldOffset;

			switch (pField->nFieldType)
			{
			case TXTFIELD_ASCII:
			case TXTFIELD_BYTE:
			case TXTFIELD_UNKNOWN1:
			case TXTFIELD_UNKNOWN2:
			case TXTFIELD_BYTE2:
			case TXTFIELD_UNKNOWN4:
			case TXTFIELD_UNKNOWN6:
				*pDest = 0;
				break;
			case TXTFIELD_DWORD:
			case TXTFIELD_DWORD2:
			case TXTFIELD_RAW:
			case TXTFIELD_ASCIITOCODE:
			case TXTFIELD_NAMETOINDEX2:
				*reinterpret_cast<uint32_t*>(pDest) = 0;
				break;
			case TXTFIELD_WORD:
			case TXTFIELD_UNKNOWN5:
			case TXTFIELD_NAMETOINDEX:
			case TXTFIELD_KEYTOWORD:
				*reinterpret_cast<uint16_t*>(pDest) = 0;
				break;
			case TXTFIELD_UNKNOWN3:
			case TXTFIELD_NAMETODWORD:
				*reinterpret_cast<int32_t*>(pDest) = -1;
				break;
			case TXTFIELD_CODETOBYTE:
			case TXTFIELD_NAMETOWORD2:
				*reinterpret_cast<int8_t*>(pDest) = -1;
				break;
			case TXTFIELD_CODETOWORD:
			case TXTFIELD_NAMETOWORD:
				*reinterpret_cast<int16_t*>(pDest) = -1;
				break;
			case TXTFIELD_CUSTOMLINK:
			case TXTFIELD_UNKNOWN7:
			case TXTFIELD_CALCTODWORD:
				if (pField->pLinkField)
				{
					reinterpret_cast<CustomFieldCallback>(pField->pLinkField)(
						nullptr, pRecord, pField->nFieldOffset, pField->nFieldLength, nRow, nUnmatched);
				}
				break;
			default:
				break;
			}
		}

		D2_ASSERT(*szCell == '\n');
		++szCell;
		pRecord += nRecordSize;
	}
}

// 1.10f: Fog.0x6FF5A8B0 (#10208)
// 1.12a: Fog.0x6FF67B00 (#10208)
D2BinFileStrc* __stdcall FOG_CreateBinFile(void* pDataBuffer, int nBufferSize)
{
	uint8_t* pCur = static_cast<uint8_t*>(pDataBuffer);
	uint8_t* pEnd = pCur + nBufferSize;

	D2BinFileStrc* pBinFile = D2_ALLOC_STRC(D2BinFileStrc);
	pBinFile->pDataBuffer = pCur;
	pBinFile->pData = nullptr;
	pBinFile->nRecordCount = 0;
	pBinFile->nCellCount = 0;

	bool bFoundHeaderEnd = false;
	while (pCur < pEnd)
	{
		const uint8_t ch = *pCur++;
		if (ch == '\t')
		{
			pCur[-1] = '\0';
			++pBinFile->nCellCount;
		}
		else if (ch == '\r')
		{
			pCur[-1] = '\0';
			D2_ASSERT(*pCur == '\n');
			++pCur;
			++pBinFile->nCellCount;
			bFoundHeaderEnd = true;
			break;
		}
	}

	if (!bFoundHeaderEnd || pCur >= pEnd)
	{
		D2_FREE(pBinFile->pDataBuffer);
		D2_FREE(pBinFile);
		return nullptr;
	}

	D2_ASSERT(pBinFile->nCellCount <= 280);
	pBinFile->pData = pCur;

	while (pCur < pEnd)
	{
		uint8_t* pLineStart = pCur;
		while (pCur < pEnd)
		{
			const uint8_t ch = *pCur++;
			if (ch == '\t')
			{
				pCur[-1] = '\0';
			}
			else if (ch == '\r')
			{
				pCur[-1] = '\0';
				D2_ASSERT(*pCur == '\n');
				++pCur;
				++pBinFile->nRecordCount;
				break;
			}
		}

		if (SStrCmp(reinterpret_cast<const char*>(pLineStart), "Expansion", 10) == 0)
		{
			--pBinFile->nRecordCount;
			const ptrdiff_t nRemaining = pEnd - pCur;
			SMemCopy(pLineStart, pCur, nRemaining);
			pEnd -= (pCur - pLineStart);
			pCur = pLineStart;
		}
	}

	return pBinFile;
}

// 1.10f: Fog.0x6FF5AA10 (#10209)
// 1.12a: Fog.0x6FF67570 (#10209)
void __stdcall FOG_FreeBinFile(D2BinFileStrc* pBinFile)
{
	D2_FREE(pBinFile->pDataBuffer);
	D2_FREE(pBinFile);
}

// 1.10f: Fog.0x6FF5AA50 (#10210)
// 1.12a: Fog.0x6FF67410 (#10210)
int __stdcall FOG_GetRecordCountFromBinFile(D2BinFileStrc* pBinFile)
{
	return pBinFile->nRecordCount;
}

// 1.10f: Fog.0x6FF5B720 (#10211)
// 1.12a: Fog.0x6FF67830 (#10211)
void* __stdcall FOG_AllocLinker(const char* szFile, int nLine)
{
	D2TxtLinkStrc* pLinker = static_cast<D2TxtLinkStrc*>(FOG_AllocPool(nullptr, sizeof(D2TxtLinkStrc), szFile, nLine, 0));
	pLinker->nRecords = 0;
	pLinker->nAllocatedCells = 0;
	pLinker->pTbl = nullptr;
	pLinker->pFirstNode = nullptr;
	return pLinker;
}

// 1.10f: Fog.0x6FF5B750 (#10212)
// 1.12a: Fog.0x6FF675B0 (#10212)
void __stdcall FOG_FreeLinker(void* pLinker)
{
	if (!pLinker)
	{
		return;
	}

	D2TxtLinkStrc* pTxtLinker = static_cast<D2TxtLinkStrc*>(pLinker);
	if (pTxtLinker->pFirstNode)
	{
		Excel_FreeLinkNodes(pTxtLinker->pFirstNode);
	}
	if (pTxtLinker->pTbl)
	{
		D2_FREE(pTxtLinker->pTbl);
	}
	D2_FREE(pTxtLinker);
}

// 1.10f: Fog.0x6FF5B810 (#10213)
// 1.12a: Fog.0x6FF67390 (#10213)
int __stdcall FOG_GetLinkIndex(void* pLink, uint32_t dwCode, BOOL bLogError)
{
	D2_MAYBE_UNUSED(bLogError);
	D2TxtLinkStrc* pTxtLinker = static_cast<D2TxtLinkStrc*>(pLink);
	int nLow = 0;
	int nHigh = pTxtLinker->nRecords;
	D2TxtLinkTblStrc* pTbl = pTxtLinker->pTbl;

	while (nLow < nHigh)
	{
		const int nMid = nLow + (nHigh - nLow) / 2;
		const uint32_t dwMidCode = pTbl[nMid].dwCode;
		if (dwCode > dwMidCode)
		{
			nLow = nMid + 1;
		}
		else if (dwCode < dwMidCode)
		{
			nHigh = nMid;
		}
		else
		{
			return pTbl[nMid].nLinkIndex;
		}
	}

	return -1;
}

// 1.10f: Fog.0x6FF5B8F0 (#10214)
// 1.12a: Fog.0x6FF672D0 (#10214)
int __stdcall FOG_GetStringFromLinkIndex(void* pLinker, int nIndex, char* szString)
{
	D2TxtLinkStrc* pTxtLinker = static_cast<D2TxtLinkStrc*>(pLinker);
	if (!pTxtLinker->pTbl || nIndex < 0 || nIndex >= pTxtLinker->nRecords)
	{
		return 0;
	}

	for (int32_t i = 0; i < pTxtLinker->nRecords; ++i)
	{
		if (pTxtLinker->pTbl[i].nLinkIndex == nIndex)
		{
			const char* szCode = pTxtLinker->pTbl[i].szCode;
			szString[0] = (szCode[0] != ' ') ? szCode[0] : '\0';
			szString[1] = (szCode[1] != ' ') ? szCode[1] : '\0';
			szString[2] = (szCode[2] != ' ') ? szCode[2] : '\0';
			szString[3] = (szCode[3] != ' ') ? szCode[3] : '\0';
			szString[4] = '\0';
			return 1;
		}
	}

	return 0;
}

// 1.10f: Fog.0x6FF5B990 (#10215)
// 1.12a: Fog.0x6FF67DC0 (#10215)
int __stdcall FOG_10215(void* pBin, int a2)
{
	D2TxtLinkStrc* pTxtLinker = static_cast<D2TxtLinkStrc*>(pBin);
	D2_ASSERT(pTxtLinker != nullptr);
	D2_ASSERT(pTxtLinker->pFirstNode == nullptr);

	if (pTxtLinker->nAllocatedCells <= pTxtLinker->nRecords)
	{
		const int32_t nOldAllocated = pTxtLinker->nAllocatedCells;
		const int32_t nNewAllocated = nOldAllocated + 64;
		pTxtLinker->pTbl = static_cast<D2TxtLinkTblStrc*>(
			FOG_ReallocPool(nullptr, pTxtLinker->pTbl, nNewAllocated * sizeof(D2TxtLinkTblStrc), __FILE__, __LINE__, 0));
		memset(&pTxtLinker->pTbl[nOldAllocated], 0, 64 * sizeof(D2TxtLinkTblStrc));
		pTxtLinker->nAllocatedCells = nNewAllocated;
	}

	uint32_t dwCode = static_cast<uint32_t>(a2);
	int nLow = 0;
	int nHigh = pTxtLinker->nRecords;

	while (nLow < nHigh)
	{
		const int nMid = nLow + (nHigh - nLow) / 2;
		const uint32_t dwMidCode = pTxtLinker->pTbl[nMid].dwCode;
		if (dwCode > dwMidCode)
		{
			nLow = nMid + 1;
		}
		else if (dwCode < dwMidCode)
		{
			nHigh = nMid;
		}
		else
		{
			++dwCode;
			nLow = 0;
			nHigh = pTxtLinker->nRecords;
		}
	}

	if (nLow < pTxtLinker->nRecords)
	{
		memmove(
			&pTxtLinker->pTbl[nLow + 1],
			&pTxtLinker->pTbl[nLow],
			static_cast<size_t>(pTxtLinker->nRecords - nLow) * sizeof(D2TxtLinkTblStrc));
	}

	pTxtLinker->pTbl[nLow].dwCode = dwCode;
	pTxtLinker->pTbl[nLow].nLinkIndex = pTxtLinker->nRecords;
	return pTxtLinker->nRecords++;
}

// 1.10f: Fog.0x6FF5BD80 (#10216)
// 1.12a: Fog.0x6FF67630 (#10216)
int __stdcall FOG_10216_AddRecordToLinkingTable(void* pBin, const char* a2)
{
	D2TxtLinkStrc* pTxtLinker = static_cast<D2TxtLinkStrc*>(pBin);
	D2_ASSERT(pTxtLinker != nullptr);
	D2_ASSERT(pTxtLinker->pTbl == nullptr);

	D2TxtLinkNodeStrc* pNode = D2_CALLOC_STRC(D2TxtLinkNodeStrc);
	SStrCopy(pNode->szText, a2, sizeof(pNode->szText));
	Excel_ToLowerInPlace(pNode->szText, sizeof(pNode->szText));
	pNode->nLinkIndex = pTxtLinker->nRecords;
	Excel_InsertLinkNode(&pTxtLinker->pFirstNode, pNode);
	return pTxtLinker->nRecords++;
}

// 1.10f: Fog.0x6FF5BC20 (#10217)
// 1.12a: Fog.0x6FF67740 (#10217)
int __stdcall FOG_GetRowFromTxt(void* pBin, char* szText, int nColumn)
{
	D2_MAYBE_UNUSED(nColumn);
	char szLower[32];
	SStrCopy(szLower, szText, sizeof(szLower));
	Excel_ToLowerInPlace(szLower, sizeof(szLower));

	D2TxtLinkStrc* pTxtLinker = static_cast<D2TxtLinkStrc*>(pBin);
	if (pTxtLinker->nRecords != 0)
	{
		D2_ASSERT(pTxtLinker->pFirstNode != nullptr);
	}

	D2TxtLinkNodeStrc* pCurr = pTxtLinker->pFirstNode;
	while (pCurr != nullptr)
	{
		const int nCmp = strcmp(szLower, pCurr->szText);
		if (nCmp == 0)
		{
			return pCurr->nLinkIndex;
		}
		pCurr = (nCmp < 0) ? pCurr->pPrevious : pCurr->pNext;
	}

	return -1;
}

// 1.10f: Fog.0x6FF5BB20 (#10255)
// 1.12a: Fog.0x6FF67470 (#10255)
char* __stdcall FOG_10255(void* pLinker, int nId, int a3)
{
	D2TxtLinkStrc* pTxtLinker = static_cast<D2TxtLinkStrc*>(pLinker);
	if (!pTxtLinker || nId < 0 || nId >= pTxtLinker->nRecords)
	{
		return const_cast<char*>(gszUnknownLinkName);
	}

	const char* szResult = Excel_FindLinkNameByIndex(pTxtLinker->pFirstNode, nId);
	if ((!szResult || szResult == gszUnknownLinkName) && a3 != 0)
	{
		return const_cast<char*>(gszUnknownLinkName);
	}

	return const_cast<char*>(szResult);
}
