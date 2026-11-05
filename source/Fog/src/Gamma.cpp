#include <Fog.h>

#include <cstring>

// 1.12a: 0x6FF7F020 (256 bytes)
static uint8_t gbFogHuffmanCodeLengths[256] = {
	0x01, 0x04, 0x06, 0x07, 0x07, 0x06, 0x07, 0x07, 0x07, 0x08, 0x07, 0x08, 0x08, 0x07, 0x09, 0x08,
	0x08, 0x08, 0x07, 0x06, 0x06, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x08, 0x07, 0x08,
	0x08, 0x0A, 0x0A, 0x0A, 0x0A, 0x09, 0x0A, 0x0A, 0x09, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0B,
	0x09, 0x09, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x09, 0x0A, 0x0A, 0x0A, 0x0A,
	0x09, 0x0A, 0x0A, 0x09, 0x0A, 0x09, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x09, 0x09, 0x0A, 0x0B,
	0x09, 0x08, 0x09, 0x0A, 0x0B, 0x09, 0x09, 0x0A, 0x09, 0x0A, 0x0A, 0x0A, 0x0B, 0x0A, 0x0A, 0x0A,
	0x0A, 0x0A, 0x09, 0x09, 0x09, 0x0A, 0x0A, 0x07, 0x07, 0x07, 0x0A, 0x09, 0x08, 0x07, 0x0A, 0x0A,
	0x0A, 0x0B, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0B, 0x0A, 0x0A, 0x0A, 0x0A,
	0x07, 0x0A, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0A,
	0x09, 0x0B, 0x0B, 0x0A, 0x0B, 0x09, 0x09, 0x09, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
	0x0A, 0x0B, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
	0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
	0x0A, 0x0B, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0A, 0x0B,
	0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
	0x0A, 0x0B, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
	0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x06,
};

// 1.12a: 0x6FF7F120 (16 dwords)
static const uint32_t gdwFogHuffmanBitMasks[16] = {
	0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F,
	0x000000FF, 0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF, 0x00001FFF, 0x00003FFF, 0x00007FFF,
};

static uint8_t* gpFogHuffmanDecodeTable[256] = {};
static uint8_t gbFogHuffmanDecodeArena[768] = {};
static uint8_t gbFogHuffmanCodes[256] = {};
static uint64_t gqwFogHuffmanByteCounts[256] = {};
static uint64_t gqwFogRawBytesTotal = 0;
static uint64_t gqwFogCompressedBytesTotal = 0;
static uint64_t gqwFogHeaderBytesTotal = 0;

//1.10f: Fog.dll 0x6FF5CC40 (#10218)
//1.12a: Fog.dll 0x6FF6C690 (#10218)
int __fastcall FOG_10218()
{
	std::memset(gqwFogHuffmanByteCounts, 0, sizeof(gqwFogHuffmanByteCounts));
	gqwFogRawBytesTotal = 0;
	gqwFogCompressedBytesTotal = 0;
	gqwFogHeaderBytesTotal = 0;
	return FOG_10219(gbFogHuffmanCodeLengths);
}

//1.10f: Fog.dll 0x6FF5C9E0 (#10219)
//1.12a: Fog.dll 0x6FF6C110 (#10219)
int __fastcall FOG_10219(uint8_t* pCodeLengths)
{
	uint32_t nLengthCounts[16] = {};
	for (int i = 0; i < 256; ++i)
	{
		const uint8_t nLen = pCodeLengths[i];
		if (nLen == 0 || nLen >= 16)
		{
			return 0;
		}
		nLengthCounts[nLen]++;
	}

	uint32_t nRunning = 0;
	for (int nLen = 15; nLen >= 1; --nLen)
	{
		const uint32_t nCount = nLengthCounts[nLen];
		nLengthCounts[nLen] = nRunning;
		nRunning += nCount;
	}
	nLengthCounts[0] = nRunning;

	uint8_t sortedSymbols[256] = {};
	for (int nSym = 0; nSym < 256; ++nSym)
	{
		const uint8_t nLen = pCodeLengths[nSym];
		sortedSymbols[nLengthCounts[nLen]++] = static_cast<uint8_t>(nSym);
	}

	uint8_t sortedLengths[256] = {};
	for (int i = 0; i < 256; ++i)
	{
		sortedLengths[i] = pCodeLengths[sortedSymbols[i]];
		gbFogHuffmanCodeLengths[i] = pCodeLengths[i];
	}

	gbFogHuffmanCodes[sortedSymbols[0]] = 0;
	for (int i = 0; i < 255; ++i)
	{
		const int nPrevCode = gbFogHuffmanCodes[sortedSymbols[i]];
		const int nShift = sortedLengths[i] - sortedLengths[i + 1];
		gbFogHuffmanCodes[sortedSymbols[i + 1]] = static_cast<uint8_t>((nPrevCode + 1) >> nShift);
	}

	uint8_t* pArena = gbFogHuffmanDecodeArena;
	for (int idx = 255; idx >= 0; --idx)
	{
		const uint8_t nLen = sortedLengths[idx];
		const uint8_t nSym = sortedSymbols[idx];
		if (nLen <= 8)
		{
			const uint32_t dwPrefix = static_cast<uint32_t>(gbFogHuffmanCodes[nSym]) << (8 - nLen);
			const int nCount = 1 << (8 - nLen);
			uint8_t* pEntry = pArena;
			pEntry[0] = 0;
			pEntry[1] = nSym;
			pArena += 2;
			for (int j = 0; j < nCount; ++j)
			{
				gpFogHuffmanDecodeTable[dwPrefix + j] = pEntry;
			}
		}
		else
		{
			const uint8_t nExtraBits = static_cast<uint8_t>(nLen - 8);
			const uint32_t dwCode = gbFogHuffmanCodes[nSym];
			const uint32_t dwPrefix = dwCode >> nExtraBits;
			const int nSubSize = 1 << nExtraBits;
			if ((dwCode & (nSubSize - 1)) == 0)
			{
				uint8_t* pEntry = pArena;
				gpFogHuffmanDecodeTable[dwPrefix] = pEntry;
				*pArena++ = nExtraBits;

				int nSubIdx = 0;
				const uint8_t* pSymCursor = &sortedSymbols[idx];
				while (nSubIdx < nSubSize)
				{
					const uint8_t nCurLen = sortedLengths[idx + nSubIdx];
					int nStep = 1 << (nLen - nCurLen);
					if (nStep != 0)
					{
						const uint8_t nCurSym = *pSymCursor;
						pArena += nStep;
						while (nStep-- > 0)
						{
							pEntry[1 + nSubIdx++] = nCurSym;
						}
					}
					++pSymCursor;
				}
			}
		}
	}

	return 1;
}

//1.10f: Fog.dll 0x6FF5CC90 (#10222)
//1.12a: Fog.dll 0x6FF6C050 (#10222)
int __fastcall FOG_10222(const uint8_t* pPacket, int nSize)
{
	uint32_t dwTotalBits = 0;
	const uint32_t dwSize = static_cast<uint32_t>(nSize);
	for (uint32_t i = 0; i < dwSize; ++i)
	{
		const uint8_t b = pPacket[i];
		gqwFogHuffmanByteCounts[b]++;
		dwTotalBits += gbFogHuffmanCodeLengths[b];
	}

	uint32_t dwCompBytes = ((dwTotalBits + 7) >> 3) + 1;
	uint32_t dwHdrBytes = 1;
	if (dwCompBytes >= 0xF0)
	{
		dwCompBytes++;
		dwHdrBytes = 2;
	}

	gqwFogRawBytesTotal += dwSize;
	gqwFogCompressedBytesTotal += dwCompBytes;
	gqwFogHeaderBytesTotal += dwHdrBytes;
	return static_cast<int>(dwCompBytes);
}

//1.10f: Fog.dll 0x6FF5CD50 (#10223)
//1.12a: Fog.dll 0x6FF6BFA0 (#10223)
int __fastcall FOG_10223(uint8_t* pDest, int nDestSize, const uint8_t* pSrc, int nSrcSize)
{
	uint8_t* pOut = pDest;
	int nRemainingDest = nDestSize;
	uint8_t bCurByte = 0;
	int nBitsAvail = 8;

	if (nSrcSize != 0)
	{
		int nRemainingSrc = nSrcSize;
		const uint8_t* pIn = pSrc;
		do
		{
			--nRemainingSrc;
			const uint8_t bSym = *pIn++;
			int nCodeLen = gbFogHuffmanCodeLengths[bSym];
			const uint8_t bCode = gbFogHuffmanCodes[bSym];

			while (nCodeLen >= nBitsAvail)
			{
				nCodeLen -= nBitsAvail;
				if (nRemainingDest-- == 0)
				{
					return 0;
				}
				const uint8_t bPart = static_cast<uint8_t>((bCode >> nCodeLen) | bCurByte);
				*pOut++ = bPart;
				bCurByte = 0;
				nBitsAvail = 8;
			}

			if (nCodeLen != 0)
			{
				nBitsAvail -= nCodeLen;
				bCurByte |= static_cast<uint8_t>(bCode << nBitsAvail);
			}
		} while (nRemainingSrc != 0);

		if (nBitsAvail < 8)
		{
			if (nRemainingDest == 0)
			{
				return 0;
			}
			*pOut++ = bCurByte;
		}
	}

	return static_cast<int>(pOut - pDest);
}

//1.10f: Fog.dll 0x6FF5CE00 (#10224)
//1.12a: Fog.dll 0x6FF6BEE0 (#10224)
int __fastcall FOG_10224(char* pDest, int nDestSize, uint8_t* pSrc, int nSrcSize)
{
	char* pOut = pDest;
	int nRemainingDest = nDestSize;
	const uint8_t* pIn = pSrc;
	uint32_t dwRemainingSrc = static_cast<uint32_t>(nSrcSize);
	uint32_t dwBitBuffer = 0;
	int nFreeBits = 32;

	for (;;)
	{
		while (nFreeBits >= 8 && dwRemainingSrc > 0)
		{
			nFreeBits -= 8;
			dwBitBuffer |= static_cast<uint32_t>(*pIn++) << nFreeBits;
			--dwRemainingSrc;
		}

		const uint8_t* pDecodeEntry = gpFogHuffmanDecodeTable[dwBitBuffer >> 24];
		const uint32_t dwExtraBits = pDecodeEntry[0];
		const uint32_t dwSubIdx = (dwBitBuffer >> (24 - dwExtraBits)) & gdwFogHuffmanBitMasks[dwExtraBits];
		const uint8_t bSym = pDecodeEntry[1 + dwSubIdx];
		const int nCodeLen = gbFogHuffmanCodeLengths[bSym];

		nFreeBits += nCodeLen;
		if (nFreeBits > 32)
		{
			return static_cast<int>(pOut - pDest);
		}

		if (nRemainingDest-- == 0)
		{
			return 0;
		}

		*pOut++ = static_cast<char>(bSym);
		dwBitBuffer <<= nCodeLen;
	}
}
