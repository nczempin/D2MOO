#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <vector>

#include <Storm.h>
#include <storm/List.hpp>

namespace
{
// ============================================================================
// 1. Storm Global Options (#301, #303, #304)
// ============================================================================
uint32_t g_StormOpt1 = 0;
uint32_t g_StormOpt2 = 0x4000;
uint32_t g_StormOpt3 = 0;
uint32_t g_StormOpt4 = 0;
uint32_t g_StormOpt5 = 0;

// ============================================================================
// 2. Storm Window Message Dispatcher (#412, #414..#419, #511..#515, #517..#518)
// ============================================================================
enum StormEvtType : uint32_t
{
	SEVT_TYPE_MSG     = 0x534D5347u, // 'SMSG'
	SEVT_TYPE_CMD     = 0x534D5348u, // 'SMSH'
	SEVT_TYPE_KEYDOWN = 0x534D5349u, // 'SMSI'
	SEVT_TYPE_KEYUP   = 0x534D534Au, // 'SMSJ'
};

struct StormMsgEntry
{
	uint32_t nId = 0;
	uint32_t nSeq = 0;
	SMSGHANDLER pfnHandler = nullptr;
};

struct StormMsgWindowLayer
{
	std::vector<StormMsgEntry> entries;
};

struct StormMsgWindowStack
{
	uint32_t nType = 0;
	HWND hWnd = nullptr;
	std::vector<StormMsgWindowLayer> layers; // back() is active layer
};

std::recursive_mutex g_SMsgMutex;
std::vector<StormMsgWindowStack> g_SMsgStacks;
std::vector<SMSGHANDLER_PARAMS*> g_SMsgBreakList;
uint32_t g_SMsgNextSeq = 1;
HWND g_SMsgDefaultWindow = nullptr;

StormMsgWindowStack* FindMsgStack(uint32_t nType, HWND hWnd, bool bCreate)
{
	for (auto& st : g_SMsgStacks)
	{
		if (st.nType == nType && st.hWnd == hWnd)
		{
			if (st.layers.empty() && bCreate)
			{
				st.layers.emplace_back();
			}
			return &st;
		}
	}
	if (!bCreate)
	{
		return nullptr;
	}
	StormMsgWindowStack created;
	created.nType = nType;
	created.hWnd = hWnd;
	created.layers.emplace_back();
	g_SMsgStacks.push_back(std::move(created));
	return &g_SMsgStacks.back();
}

BOOL RegisterMsgInternal(uint32_t nType, HWND hWnd, uint32_t nId, SMSGHANDLER pHandler)
{
	if (!pHandler)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	std::lock_guard<std::recursive_mutex> lock(g_SMsgMutex);
	StormMsgWindowStack* pStack = FindMsgStack(nType, hWnd, true);
	StormMsgEntry entry;
	entry.nId = nId;
	entry.nSeq = g_SMsgNextSeq++;
	entry.pfnHandler = pHandler;
	pStack->layers.back().entries.push_back(entry);
	return TRUE;
}

BOOL UnregisterMsgInternal(uint32_t nType, HWND hWnd, uint32_t nId, SMSGHANDLER pHandler)
{
	std::lock_guard<std::recursive_mutex> lock(g_SMsgMutex);
	StormMsgWindowStack* pStack = FindMsgStack(nType, hWnd, false);
	if (!pStack || pStack->layers.empty())
	{
		return FALSE;
	}
	auto& vec = pStack->layers.back().entries;
	BOOL bRemoved = FALSE;
	for (auto it = vec.begin(); it != vec.end();)
	{
		if (it->nId == nId && (!pHandler || it->pfnHandler == pHandler))
		{
			it = vec.erase(it);
			bRemoved = TRUE;
		}
		else
		{
			++it;
		}
	}
	return bRemoved;
}

bool ConsumeBreakRequestLocked(SMSGHANDLER_PARAMS* pParams)
{
	for (auto it = g_SMsgBreakList.begin(); it != g_SMsgBreakList.end(); ++it)
	{
		if (*it == pParams)
		{
			g_SMsgBreakList.erase(it);
			return true;
		}
	}
	return false;
}

BOOL DispatchEvtChain(uint32_t nType, HWND hWnd, uint32_t nId, SMSGHANDLER_PARAMS* pParams)
{
	BOOL bCalledAny = FALSE;
	uint32_t nUpperSeq = 0xFFFFFFFFu;

	for (;;)
	{
		SMSGHANDLER pfnToCall = nullptr;
		{
			std::lock_guard<std::recursive_mutex> lock(g_SMsgMutex);
			if (ConsumeBreakRequestLocked(pParams))
			{
				break;
			}
			StormMsgWindowStack* pStack = FindMsgStack(nType, hWnd, false);
			if (!pStack || pStack->layers.empty())
			{
				break;
			}
			const auto& vec = pStack->layers.back().entries;
			uint32_t nBestSeq = 0;
			for (const auto& e : vec)
			{
				if (e.nId == nId && e.nSeq < nUpperSeq && e.nSeq >= nBestSeq && e.pfnHandler != nullptr)
				{
					nBestSeq = e.nSeq;
					pfnToCall = e.pfnHandler;
				}
			}
			if (!pfnToCall)
			{
				break;
			}
			nUpperSeq = nBestSeq;
		}

		bCalledAny = TRUE;
		pfnToCall(pParams);
	}

	{
		std::lock_guard<std::recursive_mutex> lock(g_SMsgMutex);
		while (ConsumeBreakRequestLocked(pParams))
		{
		}
	}
	return bCalledAny;
}

void CleanupWindowMsgStacks(HWND hWnd)
{
	std::lock_guard<std::recursive_mutex> lock(g_SMsgMutex);
	for (auto it = g_SMsgStacks.begin(); it != g_SMsgStacks.end();)
	{
		if (it->hWnd == hWnd)
		{
			it = g_SMsgStacks.erase(it);
		}
		else
		{
			++it;
		}
	}
}

// ============================================================================
// 3. Storm Compression Codecs (#552 SCompDecompress)
//    - 0x08: PKWARE DCL Explode (1.12a: 0x6FBFEC10)
//    - 0x02: RFC 1950/1951 Zlib Inflate
//    - 0x01: Storm Adaptive Huffman (1.12a: 0x6FC13790)
//    - 0x80: Storm IMA ADPCM Stereo (1.12a: 0x6FC12CF0)
//    - 0x40: Storm IMA ADPCM Mono (1.12a: 0x6FC12D80)
// ============================================================================

// --- 3A. Storm IMA ADPCM (1.12a: 0x6FC12740) ---
const int32_t g_AdpcmIndexTable[32] = {
	-1, 0, -1, 4, -1, 2, -1, 6, -1, 1, -1, 5, -1, 3, -1, 7,
	-1, 1, -1, 5, -1, 3, -1, 7, -1, 2, -1, 4, -1, 6, -1, 8
};

const uint32_t g_AdpcmStepTable[89] = {
	7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31,
	34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143,
	157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658,
	724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
	3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
	15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};

uint32_t StormDecompressAdpcm(uint8_t* pOut, uint32_t cbMaxOut, const uint8_t* pIn, uint32_t cbIn, uint32_t nChannels)
{
	if (cbIn < 2 + nChannels * 2 || nChannels == 0 || nChannels > 2)
	{
		return 0;
	}

	const uint8_t nBitShift = static_cast<uint8_t>((pIn[1] > 0) ? (pIn[1] - 1) : 0); // Wait: in 0x6FC12740: *(byte*)(param_2 + 1) & 0x1f directly!
	const uint8_t nShift = static_cast<uint8_t>(pIn[1] & 0x1Fu);
	(void)nBitShift;

	int32_t stepIndices[2] = { 0x2C, 0x2C };
	int32_t predicted[2] = { 0, 0 };

	const uint8_t* pbSrc = pIn + 2;
	const uint8_t* const pbEnd = pIn + cbIn;
	int16_t* pwOut = reinterpret_cast<int16_t*>(pOut);
	int16_t* const pwStart = pwOut;
	uint32_t cbRemOut = cbMaxOut;

	for (uint32_t ch = 0; ch < nChannels; ++ch)
	{
		int16_t s0 = 0;
		std::memcpy(&s0, pbSrc, sizeof(int16_t));
		pbSrc += 2;
		predicted[ch] = static_cast<int32_t>(s0);
		if (cbRemOut < 2)
		{
			return static_cast<uint32_t>(reinterpret_cast<uint8_t*>(pwOut) - reinterpret_cast<uint8_t*>(pwStart));
		}
		*pwOut++ = s0;
		cbRemOut -= 2;
	}

	uint32_t chIdx = nChannels - 1;
	while (pbSrc < pbEnd)
	{
		if (nChannels == 2)
		{
			chIdx = (chIdx == 0) ? 1u : 0u;
		}
		const uint8_t bCode = *pbSrc++;
		if ((bCode & 0x80u) != 0)
		{
			const uint32_t cmd = bCode & 0x7Fu;
			if (cmd == 0)
			{
				if (stepIndices[chIdx] != 0)
				{
					stepIndices[chIdx] -= 1;
				}
				if (cbRemOut < 2)
				{
					break;
				}
				*pwOut++ = static_cast<int16_t>(predicted[chIdx]);
				cbRemOut -= 2;
			}
			else if (cmd == 1)
			{
				stepIndices[chIdx] += 8;
				if (stepIndices[chIdx] > 0x58)
				{
					stepIndices[chIdx] = 0x58;
				}
				if (nChannels == 2)
				{
					chIdx = (chIdx == 0) ? 1u : 0u;
				}
			}
			else if (cmd == 2)
			{
				stepIndices[chIdx] -= 8;
				if (stepIndices[chIdx] < 0)
				{
					stepIndices[chIdx] = 0;
				}
				if (nChannels == 2)
				{
					chIdx = (chIdx == 0) ? 1u : 0u;
				}
			}
			continue;
		}

		const uint32_t stepSize = g_AdpcmStepTable[stepIndices[chIdx]];
		uint32_t delta = stepSize >> nShift;
		if (bCode & 0x01u) delta += stepSize;
		if (bCode & 0x02u) delta += (stepSize >> 1);
		if (bCode & 0x04u) delta += (stepSize >> 2);
		if (bCode & 0x08u) delta += (stepSize >> 3);
		if (bCode & 0x10u) delta += (stepSize >> 4);
		if (bCode & 0x20u) delta += (stepSize >> 5);

		int32_t nextSample = predicted[chIdx];
		if ((bCode & 0x40u) == 0)
		{
			nextSample += static_cast<int32_t>(delta);
			if (nextSample > 32767) nextSample = 32767;
		}
		else
		{
			nextSample -= static_cast<int32_t>(delta);
			if (nextSample < -32768) nextSample = -32768;
		}
		predicted[chIdx] = nextSample;

		if (cbRemOut < 2)
		{
			break;
		}
		*pwOut++ = static_cast<int16_t>(nextSample);
		cbRemOut -= 2;

		int32_t nextIndex = stepIndices[chIdx] + g_AdpcmIndexTable[bCode & 0x1Fu];
		if (nextIndex < 0) nextIndex = 0;
		else if (nextIndex > 0x58) nextIndex = 0x58;
		stepIndices[chIdx] = nextIndex;
	}

	return static_cast<uint32_t>(reinterpret_cast<uint8_t*>(pwOut) - reinterpret_cast<uint8_t*>(pwStart));
}

// --- 3B. Storm Adaptive Huffman (1.12a: 0x6FC13020..0x6FC13790) ---
const uint8_t g_StormHuffWeightTables[9][258] = {
	// Table 0
	{
		 10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   2,   0,   0,
	},
	// Table 1
	{
		 84,  22,  22,  13,  12,   8,   6,   5,   6,   5,   6,   3,   4,   4,   3,   5,  14,  11,
		 20,  19,  19,   9,  11,   6,   5,   4,   3,   2,   3,   2,   2,   2,  13,   7,   9,   6,
		  6,   4,   3,   2,   4,   3,   3,   3,   3,   3,   2,   2,   9,   6,   4,   4,   4,   4,
		  3,   2,   3,   2,   2,   2,   2,   3,   2,   4,   8,   3,   4,   7,   9,   5,   3,   3,
		  3,   3,   2,   2,   2,   3,   2,   2,   3,   2,   2,   2,   2,   2,   2,   2,   2,   1,
		  1,   1,   2,   1,   2,   2,   6,  10,   8,   8,   6,   7,   4,   3,   4,   4,   2,   2,
		  4,   2,   3,   3,   4,   3,   7,   7,   9,   6,   4,   3,   3,   2,   1,   2,   2,   2,
		  2,   2,  10,   2,   2,   3,   2,   2,   1,   1,   2,   2,   2,   6,   3,   5,   2,   3,
		  2,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   3,   1,   1,   1,   2,   1,
		  1,   1,   1,   1,   1,   2,   4,   4,   4,   7,   9,   8,  12,   2,   1,   1,   1,   1,
		  1,   1,   1,   1,   1,   1,   1,   1,   2,   1,   1,   3,   4,   1,   2,   4,   5,   1,
		  1,   1,   1,   1,   1,   1,   2,   1,   1,   1,   4,   1,   1,   1,   1,   1,   2,   1,
		  1,   1,   1,   1,   1,   1,   1,   1,   2,   1,   1,   1,   1,   1,   1,   1,   3,   1,
		  1,   1,   1,   1,   1,   1,   2,   1,   1,   1,   1,   1,   1,   2,   2,   1,   1,   2,
		  2,   2,   6,  75,   0,   0,
	},
	// Table 2
	{
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,  39,   0,   0,  35,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255,   1,   1,   1,
		  1,   1,   1,   1,   2,   2,   1,   1,   6,  14,  16,   4,   6,   8,   5,   4,   4,   3,
		  3,   2,   2,   3,   3,   1,   1,   2,   1,   1,   1,   4,   2,   4,   2,   2,   2,   1,
		  1,   4,   1,   1,   2,   3,   3,   2,   3,   1,   3,   6,   4,   1,   1,   1,   1,   1,
		  1,   2,   1,   2,   1,   1,   1,  41,   7,  22,  18,  64,  10,  10,  17,  37,   1,   3,
		 23,  16,  38,  42,  16,   1,  35,  35,  47,  16,   6,   7,   2,   9,   1,   1,   1,   1,
		  1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
	},
	// Table 3
	{
		255,  11,   7,   5,  11,   2,   2,   2,   6,   2,   2,   1,   4,   2,   1,   3,   9,   1,
		  1,   1,   3,   4,   1,   1,   2,   1,   1,   1,   2,   1,   1,   1,   5,   1,   1,   1,
		 13,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   1,   1,   3,   1,   1,
		  1,   1,   1,   1,   1,   2,   1,   1,   1,   1,  10,   4,   2,   1,   6,   3,   2,   1,
		  1,   1,   1,   1,   3,   1,   1,   1,   5,   2,   3,   4,   3,   3,   3,   2,   1,   1,
		  1,   2,   1,   2,   3,   3,   1,   3,   1,   1,   2,   5,   1,   1,   4,   3,   5,   1,
		  3,   1,   3,   3,   2,   1,   4,   3,  10,   6,   1,   1,   1,   1,   1,   1,   1,   1,
		  1,   1,   2,   2,   1,  10,   2,   5,   1,   1,   2,   7,   2,  23,   1,   5,   1,   1,
		 14,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
		  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
		  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   6,   2,   1,   4,   5,   1,
		  1,   2,   1,   1,   1,   1,   2,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
		  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   7,   1,
		  1,   2,   1,   1,   1,   1,   2,   1,   1,   1,   1,   1,   1,   1,   2,   1,   1,   1,
		  1,   1,   1,  17,   0,   0,
	},
	// Table 4
	{
		255, 251, 152, 154, 132, 133,  99, 100,  62,  62,  34,  34,  19,  19,  24,  23,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
	},
	// Table 5
	{
		255, 241, 157, 158, 154, 155, 154, 151, 147, 147, 140, 142, 134, 136, 128, 130, 124, 124,
		114, 115, 105, 107,  95,  96,  85,  86,  74,  75,  64,  65,  55,  55,  47,  47,  39,  39,
		 33,  33,  27,  28,  23,  23,  19,  19,  16,  16,  13,  13,  11,  11,   9,   9,   8,   8,
		  7,   7,   6,   5,   5,   4,   4,   4,  25,  24,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
	},
	// Table 6
	{
		195, 203, 245,  65, 255, 123, 247,  33,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 191, 204, 242,  64, 253, 124, 247,  34,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0, 122,  70,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
	},
	// Table 7
	{
		195, 217, 239,  61, 249, 124, 233,  30, 253, 171, 241,  44, 252,  91, 254,  23,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 189, 217, 236,  61, 245, 125, 232,  29,
		251, 174, 240,  44, 251,  92, 255,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0, 112, 108,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
	},
	// Table 8
	{
		186, 197, 218,  51, 227, 109, 216,  24, 229, 148, 218,  35, 223,  74, 209,  16, 238, 175,
		228,  44, 234,  90, 222,  21, 244, 135, 233,  33, 246,  67, 252,  18,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 176, 199, 216,  51, 227, 107, 214,  24,
		231, 149, 216,  35, 219,  73, 208,  17, 233, 178, 226,  43, 232,  92, 221,  21, 241, 135,
		231,  32, 247,  68, 255,  19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,  95, 158,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
	},
};

struct StormHuffNode : TSLinkedNode<StormHuffNode>
{
	uint32_t nSymbol = 0;
	uint32_t nWeight = 0;
	StormHuffNode* pParent = nullptr;
	StormHuffNode* pChild0 = nullptr;
};

struct StormHuffQuickLink
{
	uint32_t nValidSeq = 0;
	uint32_t nCodeBits = 0;
	uintptr_t uSymOrNode = 0;
};

struct StormHuffBitStream
{
	const uint8_t* pSrc = nullptr;
	const uint8_t* pEnd = nullptr;
	uint32_t bitBuf = 0;
	uint32_t bitCount = 0;

	uint16_t ReadU16Safe()
	{
		uint16_t v = 0;
		if (pSrc < pEnd)
		{
			v = pSrc[0];
			if (pSrc + 1 < pEnd)
			{
				v |= static_cast<uint16_t>(pSrc[1]) << 8;
			}
			pSrc = (pSrc + 2 <= pEnd) ? (pSrc + 2) : pEnd;
		}
		return v;
	}

	uint32_t ReadU32Safe()
	{
		uint32_t v = 0;
		for (uint32_t i = 0; i < 4 && pSrc < pEnd; ++i)
		{
			v |= static_cast<uint32_t>(*pSrc++) << (i * 8);
		}
		return v;
	}
};

struct StormHuffTree
{
	uint32_t bAdaptive = 0;
	uint32_t nCacheSeq = 1;
	StormHuffNode nodes[0x203];
	STORM_LIST(StormHuffNode) freeList;
	STORM_LIST(StormHuffNode) activeList;
	uint32_t nUsedNodes = 0;
	StormHuffNode* symbolLeaves[0x102] = {};
	StormHuffQuickLink quickLinks[0x80] = {};

	StormHuffNode* AllocNode(uint32_t linkMode)
	{
		StormHuffNode* pNode = freeList.Head();
		if (!pNode)
		{
			if (nUsedNodes >= 0x203)
			{
				return nullptr;
			}
			pNode = &nodes[nUsedNodes++];
		}
		activeList.LinkNode(pNode, linkMode, nullptr);
		return pNode;
	}

	void UpdateWeights(StormHuffNode* pNode)
	{
		while (pNode != nullptr)
		{
			const uint32_t newWeight = pNode->nWeight + 1;
			pNode->nWeight = newWeight;

			StormHuffNode* pScan = pNode;
			StormHuffNode* pAfter = nullptr;
			for (;;)
			{
				StormHuffNode* pNext = activeList.Next(pScan);
				if (!pNext)
				{
					pAfter = nullptr;
					break;
				}
				if (newWeight <= pNext->nWeight)
				{
					pAfter = pNext;
					break;
				}
				pScan = pNext;
			}

			if (pScan != pNode)
			{
				activeList.LinkNode(pScan, 2, pNode);
				activeList.LinkNode(pNode, 2, pAfter);

				StormHuffNode* pScanParentChild0 = pScan->pParent ? pScan->pParent->pChild0 : nullptr;
				if (pNode->pParent && pNode->pParent->pChild0 == pNode)
				{
					pNode->pParent->pChild0 = pScan;
				}
				if (pScanParentChild0 == pScan && pScan->pParent)
				{
					pScan->pParent->pChild0 = pNode;
				}
				std::swap(pNode->pParent, pScan->pParent);
				++nCacheSeq;
			}
			pNode = pNode->pParent;
		}
	}

	void BuildInitialTree(uint8_t nCompType)
	{
		while (StormHuffNode* pHead = activeList.Head())
		{
			activeList.UnlinkNode(pHead);
			freeList.LinkToHead(pHead);
		}
		std::memset(symbolLeaves, 0, sizeof(symbolLeaves));
		if (nCompType >= 9)
		{
			nCompType = 0;
		}

		uint32_t maxWeightSeen = 0;
		for (uint32_t sym = 0; sym < 0x100; ++sym)
		{
			const uint8_t w = g_StormHuffWeightTables[nCompType][sym];
			if (w != 0)
			{
				StormHuffNode* pNode = AllocNode(2); // LinkToTail
				pNode->pParent = nullptr;
				pNode->pChild0 = nullptr;
				symbolLeaves[sym] = pNode;
				pNode->nSymbol = sym;
				pNode->nWeight = w;
				if (w < maxWeightSeen)
				{
					StormHuffNode* pPos = activeList.Head();
					while (pPos && w > pPos->nWeight)
					{
						pPos = activeList.Next(pPos);
					}
					activeList.LinkNode(pNode, 2, pPos);
				}
				else
				{
					maxWeightSeen = w;
				}
			}
		}

		for (uint32_t sym = 0x100; sym < 0x102; ++sym)
		{
			StormHuffNode* pNode = AllocNode(1); // LinkToHead (lightest = weight 1)
			pNode->pParent = nullptr;
			pNode->pChild0 = nullptr;
			symbolLeaves[sym] = pNode;
			pNode->nSymbol = sym;
			pNode->nWeight = 1;
		}

		StormHuffNode* pChild0 = activeList.Head();
		while (pChild0)
		{
			StormHuffNode* pChild1 = activeList.Next(pChild0);
			if (!pChild1)
			{
				break;
			}
			StormHuffNode* pParent = AllocNode(2); // LinkToTail
			pParent->pChild0 = nullptr;
			pParent->pParent = nullptr;
			pParent->nWeight = pChild0->nWeight + pChild1->nWeight;
			pParent->pChild0 = pChild0;
			pChild0->pParent = pParent;
			pChild1->pParent = pParent;

			if (pParent->nWeight < maxWeightSeen)
			{
				StormHuffNode* pPos = activeList.Next(pChild1);
				while (pPos && pParent->nWeight > pPos->nWeight)
				{
					pPos = activeList.Next(pPos);
				}
				activeList.LinkNode(pParent, 2, pPos);
			}
			else
			{
				maxWeightSeen = pParent->nWeight;
			}
			pChild0 = activeList.Next(pChild1);
		}
		nCacheSeq = 1;
	}

	void InsertEscapedSymbol(uint32_t sym)
	{
		StormHuffNode* pLightest = activeList.Head();
		if (!pLightest)
		{
			return;
		}

		StormHuffNode* pNewInternal = AllocNode(1);
		pNewInternal->pParent = pLightest;
		pNewInternal->pChild0 = nullptr;
		pNewInternal->nSymbol = pLightest->nSymbol;
		pNewInternal->nWeight = pLightest->nWeight;
		if (pNewInternal->nSymbol < 0x102)
		{
			symbolLeaves[pNewInternal->nSymbol] = pNewInternal;
		}

		StormHuffNode* pNewLeaf = AllocNode(1);
		pNewLeaf->pChild0 = nullptr;
		pNewLeaf->nWeight = 0;
		pNewLeaf->pParent = pLightest;
		pNewLeaf->nSymbol = sym;
		if (sym < 0x102)
		{
			symbolLeaves[sym] = pNewLeaf;
		}
		pLightest->pChild0 = pNewLeaf;
		UpdateWeights(pNewLeaf);
	}

	uint32_t DecodeSymbol(StormHuffBitStream& bs)
	{
		if (bs.bitCount < 8)
		{
			const uint16_t w = bs.ReadU16Safe();
			bs.bitBuf |= static_cast<uint32_t>(w) << (bs.bitCount & 0x1Fu);
			bs.bitCount += 16;
		}

		const uint32_t peek7 = bs.bitBuf & 0x7Fu;
		StormHuffQuickLink& ql = quickLinks[peek7];
		const bool bCacheMiss = (ql.nValidSeq != nCacheSeq);
		StormHuffNode* pNode = nullptr;

		if (!bCacheMiss)
		{
			if (ql.nCodeBits < 8)
			{
				bs.bitBuf >>= (ql.nCodeBits & 0x1Fu);
				bs.bitCount -= ql.nCodeBits;
				return static_cast<uint32_t>(ql.uSymOrNode);
			}
			bs.bitBuf >>= 7;
			bs.bitCount -= 7;
			pNode = reinterpret_cast<StormHuffNode*>(ql.uSymOrNode);
		}
		else
		{
			pNode = activeList.Tail();
		}

		if (!pNode)
		{
			return 0x100;
		}

		uint32_t nBitsWalked = 0;
		StormHuffNode* pNodeAt7 = nullptr;
		while (pNode->pChild0 != nullptr)
		{
			const uint32_t bit = bs.bitBuf & 1u;
			pNode = pNode->pChild0;
			bs.bitBuf >>= 1;
			bs.bitCount -= 1;
			if (bs.bitCount == 0)
			{
				bs.bitBuf = bs.ReadU32Safe();
				bs.bitCount = 32;
			}
			if (bit != 0)
			{
				pNode = activeList.Next(pNode);
				if (!pNode)
				{
					return 0x100;
				}
			}
			++nBitsWalked;
			if (nBitsWalked == 7)
			{
				pNodeAt7 = pNode;
			}
		}

		if (bCacheMiss)
		{
			if (nBitsWalked > 7)
			{
				ql.nValidSeq = nCacheSeq;
				ql.nCodeBits = nBitsWalked;
				ql.uSymOrNode = reinterpret_cast<uintptr_t>(pNodeAt7);
			}
			else
			{
				const uint32_t step = 1u << (nBitsWalked & 0x1Fu);
				const uint32_t mask = (nBitsWalked == 0) ? 0u : ((1u << nBitsWalked) - 1u);
				for (uint32_t idx = peek7 & mask; idx < 0x80; idx += step)
				{
					quickLinks[idx].nValidSeq = nCacheSeq;
					quickLinks[idx].nCodeBits = nBitsWalked;
					quickLinks[idx].uSymOrNode = pNode->nSymbol;
				}
			}
		}
		return pNode->nSymbol;
	}
};

uint32_t StormDecompressHuffman(uint8_t* pOut, uint32_t cbMaxOut, const uint8_t* pIn, uint32_t cbIn)
{
	if (cbMaxOut == 0 || cbIn == 0)
	{
		return 0;
	}

	auto pTree = std::make_unique<StormHuffTree>();
	StormHuffBitStream bs;
	bs.pSrc = pIn;
	bs.pEnd = pIn + cbIn;
	bs.bitBuf = bs.ReadU32Safe();
	bs.bitCount = 32;

	if (bs.bitCount < 9)
	{
		const uint16_t w = bs.ReadU16Safe();
		bs.bitBuf |= static_cast<uint32_t>(w) << (bs.bitCount & 0x1Fu);
		bs.bitCount += 16;
	}
	const uint8_t nCompType = static_cast<uint8_t>(bs.bitBuf & 0xFFu);
	bs.bitBuf >>= 8;
	bs.bitCount -= 8;

	pTree->BuildInitialTree(nCompType);
	pTree->bAdaptive = (nCompType == 0) ? 1u : 0u;

	uint8_t* pbDst = pOut;
	uint32_t cbRem = cbMaxOut;
	for (;;)
	{
		uint32_t sym = pTree->DecodeSymbol(bs);
		if (sym == 0x101)
		{
			if (bs.bitCount < 9)
			{
				const uint16_t w = bs.ReadU16Safe();
				bs.bitBuf |= static_cast<uint32_t>(w) << (bs.bitCount & 0x1Fu);
				bs.bitCount += 16;
			}
			sym = bs.bitBuf & 0xFFu;
			bs.bitBuf >>= 8;
			bs.bitCount -= 8;
			pTree->InsertEscapedSymbol(sym);
			if (pTree->bAdaptive == 0 && sym < 0x102 && pTree->symbolLeaves[sym])
			{
				pTree->UpdateWeights(pTree->symbolLeaves[sym]);
			}
		}
		if (sym == 0x100)
		{
			break;
		}
		*pbDst++ = static_cast<uint8_t>(sym);
		if (--cbRem == 0)
		{
			break;
		}
		if (pTree->bAdaptive != 0 && sym < 0x102 && pTree->symbolLeaves[sym])
		{
			pTree->UpdateWeights(pTree->symbolLeaves[sym]);
		}
	}
	return static_cast<uint32_t>(pbDst - pOut);
}

// --- 3C. PKWARE Data Compression Library Explode (1.12a: 0x6FBFEC10) ---
const uint8_t g_PkDistBits[64] = {
	2, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

const uint8_t g_PkDistCode[64] = {
	3, 13, 5, 25, 9, 17, 1, 62, 30, 46, 14, 54, 22, 38, 6, 58,
	26, 42, 10, 50, 18, 34, 66, 2, 124, 60, 92, 28, 108, 44, 76, 12,
	116, 52, 84, 20, 100, 36, 68, 4, 120, 56, 88, 24, 104, 40, 72, 8,
	240, 112, 176, 48, 208, 80, 144, 16, 224, 96, 160, 32, 192, 64, 128, 0
};

const uint8_t g_PkExLenBits[16] = {
	0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8
};

const uint16_t g_PkLenBase[16] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 14, 22, 38, 70, 134, 262
};

const uint8_t g_PkLenBits[16] = {
	3, 2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7
};

const uint8_t g_PkLenCode[16] = {
	5, 3, 1, 6, 10, 2, 12, 20, 4, 24, 8, 48, 16, 32, 64, 0
};

const uint8_t g_PkChBitsAsc[256] = {
	11, 12, 12, 12, 12, 12, 12, 12, 12, 8, 7, 12, 12, 7, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12,
	4, 10, 8, 12, 10, 12, 10, 8, 7, 7, 8, 9, 7, 6, 7, 8,
	7, 6, 7, 7, 7, 7, 8, 7, 7, 8, 8, 12, 11, 7, 9, 11,
	12, 6, 7, 6, 6, 5, 7, 8, 8, 6, 11, 9, 6, 7, 6, 6,
	7, 11, 6, 6, 6, 7, 9, 8, 9, 9, 11, 8, 11, 9, 12, 8,
	12, 5, 6, 6, 6, 5, 6, 6, 6, 5, 11, 7, 5, 6, 5, 5,
	6, 10, 5, 5, 5, 5, 8, 7, 8, 8, 10, 11, 11, 12, 12, 12,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	13, 12, 13, 13, 13, 12, 13, 13, 13, 12, 13, 13, 13, 13, 12, 13,
	13, 13, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13
};

const uint16_t g_PkChCodeAsc[256] = {
	1168, 4064, 2016, 3040, 992, 3552, 1504, 2528, 480, 184, 98, 3808, 1760, 34, 2784, 736,
	3296, 1248, 2272, 224, 3936, 1888, 2912, 864, 3424, 1376, 4672, 2400, 352, 3680, 1632, 2656,
	15, 592, 56, 608, 80, 3168, 912, 216, 66, 2, 88, 432, 124, 41, 60, 152,
	92, 9, 28, 108, 44, 76, 24, 12, 116, 232, 104, 1120, 144, 52, 176, 1808,
	2144, 49, 84, 17, 33, 23, 20, 168, 40, 1, 784, 304, 62, 100, 30, 46,
	36, 1296, 14, 54, 22, 68, 48, 200, 464, 208, 272, 72, 1552, 336, 96, 136,
	4000, 7, 38, 6, 58, 27, 26, 42, 10, 11, 528, 4, 19, 50, 3, 29,
	18, 400, 13, 21, 5, 25, 8, 120, 240, 112, 656, 1040, 16, 1952, 2976, 928,
	576, 7232, 3136, 5184, 1088, 6208, 2112, 4160, 64, 8064, 3968, 6016, 1920, 7040, 2944, 4992,
	896, 7552, 3456, 5504, 1408, 6528, 2432, 4480, 384, 7808, 3712, 5760, 1664, 6784, 2688, 4736,
	640, 7296, 3200, 5248, 1152, 6272, 2176, 4224, 128, 7936, 3840, 5888, 1792, 6912, 2816, 4864,
	3488, 1440, 2464, 416, 3744, 1696, 2720, 672, 3232, 1184, 2208, 160, 3872, 1824, 2848, 800,
	3360, 1312, 2336, 288, 3616, 1568, 2592, 544, 3104, 1056, 2080, 32, 4032, 1984, 3008, 960,
	3520, 1472, 2496, 448, 3776, 1728, 2752, 704, 3264, 1216, 2240, 192, 3904, 1856, 2880, 832,
	768, 3392, 7424, 3328, 5376, 1344, 1280, 6400, 2304, 2368, 4352, 256, 7680, 3584, 320, 5632,
	1536, 6656, 3648, 1600, 2624, 2560, 4608, 512, 7168, 3072, 5120, 1024, 6144, 2048, 4096, 0
};

struct PkExplodeState
{
	uint32_t ctype = 0;
	uint32_t outputPos = 0x1000;
	uint32_t dsizeBits = 0;
	uint32_t dsizeMask = 0;
	uint32_t bitBuf = 0;
	uint32_t extraBits = 0;
	const uint8_t* pIn = nullptr;
	uint32_t inPos = 0;
	uint32_t inEnd = 0;
	uint8_t* pOut = nullptr;
	uint32_t outPos = 0;
	uint32_t outMax = 0;

	uint8_t outBuf[0x2200] = {};
	uint8_t distPosCode[0x100] = {};
	uint8_t lenCodeLut[0x100] = {};
	uint8_t chCodeAsc1[0x100] = {};
	uint8_t chCodeAsc2[0x100] = {};
	uint8_t chCodeAsc3[0x80] = {};
	uint8_t chCodeAsc4[0x100] = {};
	uint8_t chBitsAsc[0x100] = {};

	static void GenDecodeTabs(int32_t count, const uint8_t* pBits, const uint8_t* pCode, uint8_t* pLut)
	{
		for (int32_t i = count - 1; i >= 0; --i)
		{
			const uint32_t step = 1u << (pBits[i] & 0x1Fu);
			for (uint32_t idx = pCode[i]; idx < 0x100; idx += step)
			{
				pLut[idx] = static_cast<uint8_t>(i);
			}
		}
	}

	void GenAscTabs()
	{
		std::memcpy(chBitsAsc, g_PkChBitsAsc, 256);
		for (int32_t i = 255; i >= 0; --i)
		{
			uint8_t bits = chBitsAsc[i];
			const uint16_t code = g_PkChCodeAsc[i];
			if (bits <= 8)
			{
				const uint32_t step = 1u << (bits & 0x1Fu);
				for (uint32_t idx = code; idx < 0x100; idx += step)
				{
					chCodeAsc1[idx] = static_cast<uint8_t>(i);
				}
			}
			else if ((code & 0xFFu) == 0)
			{
				bits = static_cast<uint8_t>(bits - 8);
				chBitsAsc[i] = bits;
				const uint32_t step = 1u << (bits & 0x1Fu);
				for (uint32_t idx = (code >> 8); idx < 0x100; idx += step)
				{
					chCodeAsc4[idx] = static_cast<uint8_t>(i);
				}
			}
			else
			{
				chCodeAsc1[code & 0xFFu] = 0xFFu;
				if ((code & 0x3Fu) == 0)
				{
					bits = static_cast<uint8_t>(bits - 6);
					chBitsAsc[i] = bits;
					const uint32_t step = 1u << (bits & 0x1Fu);
					for (uint32_t idx = (code >> 6); idx < 0x80; idx += step)
					{
						chCodeAsc3[idx] = static_cast<uint8_t>(i);
					}
				}
				else
				{
					bits = static_cast<uint8_t>(bits - 4);
					chBitsAsc[i] = bits;
					const uint32_t step = 1u << (bits & 0x1Fu);
					for (uint32_t idx = (code >> 4); idx < 0x100; idx += step)
					{
						chCodeAsc2[idx] = static_cast<uint8_t>(i);
					}
				}
			}
		}
	}

	bool WasteBits(uint32_t nBits)
	{
		if (nBits <= extraBits)
		{
			extraBits -= nBits;
			bitBuf >>= (nBits & 0x1Fu);
			return false;
		}
		bitBuf >>= (extraBits & 0x1Fu);
		if (inPos >= inEnd)
		{
			return true;
		}
		const uint8_t nextByte = pIn[inPos++];
		bitBuf |= static_cast<uint32_t>(nextByte) << 8;
		bitBuf >>= ((nBits - extraBits) & 0x1Fu);
		extraBits = (extraBits - nBits) + 8;
		return false;
	}

	uint32_t DecodeLit()
	{
		if ((bitBuf & 1u) == 0)
		{
			if (WasteBits(1)) return 0x306;
			if (ctype == 0)
			{
				const uint32_t ch = bitBuf & 0xFFu;
				if (WasteBits(8)) return 0x306;
				return ch;
			}
			uint32_t ch = bitBuf & 0xFFu;
			if (ch == 0)
			{
				if (WasteBits(8)) return 0x306;
				ch = chCodeAsc4[bitBuf & 0xFFu];
			}
			else
			{
				ch = chCodeAsc1[ch];
				if (ch == 0xFFu)
				{
					if ((bitBuf & 0x3Fu) == 0)
					{
						if (WasteBits(6)) return 0x306;
						ch = chCodeAsc3[bitBuf & 0x7Fu];
					}
					else
					{
						if (WasteBits(4)) return 0x306;
						ch = chCodeAsc2[bitBuf & 0xFFu];
					}
				}
			}
			if (WasteBits(chBitsAsc[ch])) return 0x306;
			return ch;
		}

		if (WasteBits(1)) return 0x306;
		uint32_t lenIdx = lenCodeLut[bitBuf & 0xFFu];
		if (WasteBits(g_PkLenBits[lenIdx])) return 0x306;
		const uint8_t exBits = g_PkExLenBits[lenIdx];
		if (exBits != 0)
		{
			const uint32_t exVal = bitBuf & ((1u << exBits) - 1u);
			if (WasteBits(exBits) && (lenIdx + exVal != 0x10Eu))
			{
				return 0x306;
			}
			lenIdx = g_PkLenBase[lenIdx] + exVal;
		}
		return lenIdx + 0x100u;
	}

	uint32_t DecodeDist(uint32_t repLen)
	{
		uint32_t distCode = distPosCode[bitBuf & 0xFFu];
		if (WasteBits(g_PkDistBits[distCode])) return 0;
		if (repLen == 2)
		{
			distCode = (distCode << 2) | (bitBuf & 3u);
			if (WasteBits(2)) return 0;
		}
		else
		{
			distCode = (distCode << dsizeBits) | (bitBuf & dsizeMask);
			if (WasteBits(dsizeBits)) return 0;
		}
		return distCode + 1u;
	}

	void FlushOutput(const uint8_t* pSrc, uint32_t nBytes)
	{
		if (outPos < outMax && nBytes > 0)
		{
			const uint32_t nCopy = std::min(nBytes, outMax - outPos);
			std::memcpy(pOut + outPos, pSrc, nCopy);
			outPos += nCopy;
		}
	}
};

uint32_t StormDecompressPkware(uint8_t* pOut, uint32_t cbMaxOut, const uint8_t* pIn, uint32_t cbIn)
{
	if (cbIn < 4 || cbMaxOut == 0)
	{
		return 0;
	}

	auto st = std::make_unique<PkExplodeState>();
	st->ctype = pIn[0];
	st->dsizeBits = pIn[1];
	st->bitBuf = pIn[2];
	st->extraBits = 0;
	st->pIn = pIn;
	st->inPos = 3;
	st->inEnd = cbIn;
	st->pOut = pOut;
	st->outPos = 0;
	st->outMax = cbMaxOut;

	if (st->dsizeBits < 4 || st->dsizeBits > 6 || (st->ctype != 0 && st->ctype != 1))
	{
		return 0;
	}
	st->dsizeMask = 0xFFFFu >> (16u - st->dsizeBits);
	if (st->ctype == 1)
	{
		st->GenAscTabs();
	}
	PkExplodeState::GenDecodeTabs(16, g_PkLenBits, g_PkLenCode, st->lenCodeLut);
	PkExplodeState::GenDecodeTabs(64, g_PkDistBits, g_PkDistCode, st->distPosCode);

	st->outputPos = 0x1000;
	for (;;)
	{
		const uint32_t lit = st->DecodeLit();
		if (lit >= 0x305u)
		{
			st->FlushOutput(st->outBuf + 0x1000, st->outputPos - 0x1000);
			break;
		}
		if (lit < 0x100u)
		{
			st->outBuf[st->outputPos++] = static_cast<uint8_t>(lit);
		}
		else
		{
			const uint32_t repLen = lit - 0xFEu;
			const uint32_t dist = st->DecodeDist(repLen);
			if (dist == 0 || dist > st->outputPos)
			{
				st->FlushOutput(st->outBuf + 0x1000, st->outputPos - 0x1000);
				break;
			}
			uint8_t* pbDst = st->outBuf + st->outputPos;
			const uint8_t* pbSrc = pbDst - dist;
			st->outputPos += repLen;
			for (uint32_t i = 0; i < repLen; ++i)
			{
				pbDst[i] = pbSrc[i];
			}
		}

		if (st->outputPos >= 0x2000u)
		{
			st->FlushOutput(st->outBuf + 0x1000, 0x1000);
			std::memmove(st->outBuf, st->outBuf + 0x1000, st->outputPos - 0x1000);
			st->outputPos -= 0x1000;
		}
	}
	return st->outPos;
}

// --- 3D. RFC 1950/1951 Zlib Inflate (for MPQ_COMPRESSION_ZLIB = 0x02) ---
struct ZlibHuffTable
{
	uint16_t counts[16] = {};
	uint16_t symbols[288] = {};

	bool Build(const uint8_t* lengths, uint32_t numSyms)
	{
		std::memset(counts, 0, sizeof(counts));
		for (uint32_t i = 0; i < numSyms; ++i)
		{
			if (lengths[i] > 15) return false;
			counts[lengths[i]]++;
		}
		counts[0] = 0;
		uint16_t offsets[16] = {};
		uint16_t sum = 0;
		for (uint32_t bits = 1; bits <= 15; ++bits)
		{
			offsets[bits] = sum;
			sum = static_cast<uint16_t>(sum + counts[bits]);
		}
		for (uint32_t i = 0; i < numSyms; ++i)
		{
			const uint8_t len = lengths[i];
			if (len != 0)
			{
				symbols[offsets[len]++] = static_cast<uint16_t>(i);
			}
		}
		return true;
	}
};

struct ZlibBitReader
{
	const uint8_t* pIn = nullptr;
	uint32_t inPos = 0;
	uint32_t inEnd = 0;
	uint32_t bitBuf = 0;
	uint32_t bitCnt = 0;

	uint32_t GetBits(uint32_t n)
	{
		while (bitCnt < n)
		{
			const uint8_t b = (inPos < inEnd) ? pIn[inPos++] : 0;
			bitBuf |= static_cast<uint32_t>(b) << bitCnt;
			bitCnt += 8;
		}
		const uint32_t val = (n == 0) ? 0u : (bitBuf & ((1u << n) - 1u));
		bitBuf >>= n;
		bitCnt -= n;
		return val;
	}

	int32_t Decode(const ZlibHuffTable& ht)
	{
		int32_t code = 0;
		int32_t first = 0;
		int32_t index = 0;
		for (uint32_t len = 1; len <= 15; ++len)
		{
			code |= static_cast<int32_t>(GetBits(1));
			const int32_t count = ht.counts[len];
			if (code - count < first)
			{
				return ht.symbols[index + (code - first)];
			}
			index += count;
			first += count;
			first <<= 1;
			code <<= 1;
		}
		return -1;
	}
};

uint32_t StormDecompressZlib(uint8_t* pOut, uint32_t cbMaxOut, const uint8_t* pIn, uint32_t cbIn)
{
	if (cbIn < 2 || cbMaxOut == 0)
	{
		return 0;
	}

	uint32_t startOffset = 0;
	// Check for RFC 1950 2-byte zlib header (CMF/FLG)
	if ((pIn[0] & 0x0Fu) == 8 && ((static_cast<uint32_t>(pIn[0]) * 256u + pIn[1]) % 31u) == 0)
	{
		startOffset = 2;
		if ((pIn[1] & 0x20u) != 0)
		{
			startOffset += 4;
		}
	}
	if (startOffset >= cbIn)
	{
		return 0;
	}

	ZlibBitReader br;
	br.pIn = pIn;
	br.inPos = startOffset;
	br.inEnd = cbIn;

	static const uint16_t s_LenBase[29] = {
		3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258
	};
	static const uint8_t s_LenExtra[29] = {
		0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0
	};
	static const uint16_t s_DistBase[30] = {
		1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577
	};
	static const uint8_t s_DistExtra[30] = {
		0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13
	};
	static const uint8_t s_CodeOrder[19] = {
		16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15
	};

	uint32_t outPos = 0;
	uint32_t bFinal = 0;
	do
	{
		bFinal = br.GetBits(1);
		const uint32_t bType = br.GetBits(2);
		if (bType == 0)
		{
			br.bitBuf = 0;
			br.bitCnt = 0;
			if (br.inPos + 4 > br.inEnd) break;
			const uint16_t blkLen = static_cast<uint16_t>(br.pIn[br.inPos] | (br.pIn[br.inPos + 1] << 8));
			br.inPos += 4;
			const uint32_t nCopy = std::min<uint32_t>(blkLen, std::min(cbMaxOut - outPos, br.inEnd - br.inPos));
			std::memcpy(pOut + outPos, br.pIn + br.inPos, nCopy);
			outPos += nCopy;
			br.inPos += nCopy;
		}
		else if (bType == 1 || bType == 2)
		{
			ZlibHuffTable litLenTab, distTab;
			if (bType == 1)
			{
				uint8_t lengths[288] = {};
				for (uint32_t i = 0; i <= 143; ++i) lengths[i] = 8;
				for (uint32_t i = 144; i <= 255; ++i) lengths[i] = 9;
				for (uint32_t i = 256; i <= 279; ++i) lengths[i] = 7;
				for (uint32_t i = 280; i <= 287; ++i) lengths[i] = 8;
				litLenTab.Build(lengths, 288);
				uint8_t dlengths[32];
				std::memset(dlengths, 5, sizeof(dlengths));
				distTab.Build(dlengths, 32);
			}
			else
			{
				const uint32_t hlit = br.GetBits(5) + 257;
				const uint32_t hdist = br.GetBits(5) + 1;
				const uint32_t hclen = br.GetBits(4) + 4;
				uint8_t codeLengths[19] = {};
				for (uint32_t i = 0; i < hclen; ++i)
				{
					codeLengths[s_CodeOrder[i]] = static_cast<uint8_t>(br.GetBits(3));
				}
				ZlibHuffTable codeTab;
				if (!codeTab.Build(codeLengths, 19)) break;

				uint8_t allLengths[320] = {};
				uint32_t idx = 0;
				const uint32_t totalCodes = hlit + hdist;
				while (idx < totalCodes)
				{
					const int32_t sym = br.Decode(codeTab);
					if (sym < 0) return outPos;
					if (sym < 16)
					{
						allLengths[idx++] = static_cast<uint8_t>(sym);
					}
					else if (sym == 16)
					{
						const uint8_t prev = (idx > 0) ? allLengths[idx - 1] : 0;
						uint32_t rep = br.GetBits(2) + 3;
						while (rep-- && idx < totalCodes) allLengths[idx++] = prev;
					}
					else if (sym == 17)
					{
						uint32_t rep = br.GetBits(3) + 3;
						while (rep-- && idx < totalCodes) allLengths[idx++] = 0;
					}
					else
					{
						uint32_t rep = br.GetBits(7) + 11;
						while (rep-- && idx < totalCodes) allLengths[idx++] = 0;
					}
				}
				if (!litLenTab.Build(allLengths, hlit) || !distTab.Build(allLengths + hlit, hdist))
				{
					break;
				}
			}

			for (;;)
			{
				const int32_t sym = br.Decode(litLenTab);
				if (sym < 0 || sym == 256) break;
				if (sym < 256)
				{
					if (outPos >= cbMaxOut) break;
					pOut[outPos++] = static_cast<uint8_t>(sym);
				}
				else
				{
					const uint32_t lenIdx = static_cast<uint32_t>(sym - 257);
					if (lenIdx >= 29) break;
					const uint32_t matchLen = s_LenBase[lenIdx] + br.GetBits(s_LenExtra[lenIdx]);
					const int32_t distSym = br.Decode(distTab);
					if (distSym < 0 || distSym >= 30) break;
					const uint32_t matchDist = s_DistBase[distSym] + br.GetBits(s_DistExtra[distSym]);
					if (matchDist == 0 || matchDist > outPos) break;
					const uint32_t nCopy = std::min(matchLen, cbMaxOut - outPos);
					for (uint32_t i = 0; i < nCopy; ++i)
					{
						pOut[outPos + i] = pOut[outPos - matchDist + i];
					}
					outPos += nCopy;
				}
			}
		}
		else
		{
			break;
		}
	} while (!bFinal && outPos < cbMaxOut);

	return outPos;
}

// ============================================================================
// 4. MPQ v1 Crypt Table, Archive Table, and File Engine (#252..#289)
// ============================================================================
uint32_t g_MpqCryptTable[0x500] = {};
bool g_bMpqCryptInitialized = false;

void EnsureMpqCryptTable()
{
	if (g_bMpqCryptInitialized)
	{
		return;
	}
	uint32_t seed = 0x00100001u;
	for (uint32_t index1 = 0; index1 < 0x100u; ++index1)
	{
		uint32_t index2 = index1;
		for (int i = 0; i < 5; ++i, index2 += 0x100u)
		{
			seed = (seed * 125u + 3u) % 0x2AAAABu;
			const uint32_t temp1 = (seed & 0xFFFFu) << 16;
			seed = (seed * 125u + 3u) % 0x2AAAABu;
			const uint32_t temp2 = (seed & 0xFFFFu);
			g_MpqCryptTable[index2] = temp1 | temp2;
		}
	}
	g_bMpqCryptInitialized = true;
}

uint32_t MpqHashString(const char* szFileName, uint32_t dwHashType)
{
	EnsureMpqCryptTable();
	uint32_t seed1 = 0x7FED7FEDu;
	uint32_t seed2 = 0xEEEEEEEEu;
	while (*szFileName != '\0')
	{
		uint8_t ch = static_cast<uint8_t>(*szFileName++);
		if (ch >= 'a' && ch <= 'z')
		{
			ch = static_cast<uint8_t>(ch - ('a' - 'A'));
		}
		else if (ch == '/')
		{
			ch = '\\';
		}
		seed1 = g_MpqCryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3u;
	}
	return seed1;
}

void MpqDecryptBlock(void* pData, uint32_t cbBytes, uint32_t dwKey)
{
	EnsureMpqCryptTable();
	uint32_t* pdwData = static_cast<uint32_t*>(pData);
	uint32_t seed2 = 0xEEEEEEEEu;
	const uint32_t nDwords = cbBytes >> 2;
	for (uint32_t i = 0; i < nDwords; ++i)
	{
		seed2 += g_MpqCryptTable[0x400u + (dwKey & 0xFFu)];
		const uint32_t plain = pdwData[i] ^ (dwKey + seed2);
		dwKey = ((~dwKey << 0x15) + 0x11111111u) | (dwKey >> 0x0B);
		seed2 = plain + seed2 + (seed2 << 5) + 3u;
		pdwData[i] = plain;
	}
}

#pragma pack(push, 1)
struct MpqHeaderV1
{
	uint32_t dwID;
	uint32_t dwHeaderSize;
	uint32_t dwArchiveSize;
	uint16_t wFormatVersion;
	uint16_t wBlockSizeShift;
	uint32_t dwHashTablePos;
	uint32_t dwBlockTablePos;
	uint32_t dwHashTableEntries;
	uint32_t dwBlockTableEntries;
};

struct MpqHashEntry
{
	uint32_t dwName1;
	uint32_t dwName2;
	uint16_t lcLocale;
	uint16_t wPlatform;
	uint32_t dwBlockIndex;
};

struct MpqBlockEntry
{
	uint32_t dwFilePos;
	uint32_t dwCSize;
	uint32_t dwFSize;
	uint32_t dwFlags;
};
#pragma pack(pop)

enum MpqFileFlags : uint32_t
{
	MPQ_FLAG_IMPLODE     = 0x00000100u,
	MPQ_FLAG_COMPRESS    = 0x00000200u,
	MPQ_FLAG_ENCRYPTED   = 0x00010000u,
	MPQ_FLAG_FIX_KEY     = 0x00020000u,
	MPQ_FLAG_SINGLE_UNIT = 0x01000000u,
	MPQ_FLAG_SECTOR_CRC  = 0x04000000u,
	MPQ_FLAG_EXISTS      = 0x80000000u,
};

struct StormArchiveRecord
{
	uint32_t dwMagic = 0x53415243u; // 'SARC'
	char szFileName[MAX_PATH] = {};
	HANDLE hDiskFile = INVALID_HANDLE_VALUE;
	int32_t nPriority = 0;
	uint32_t dwArchiveOffset = 0;
	uint32_t dwSectorSize = 0x1000;
	MpqHeaderV1 header = {};
	std::vector<MpqHashEntry> hashTable;
	std::vector<MpqBlockEntry> blockTable;
};

struct StormFileRecord
{
	uint32_t dwMagic = 0x5346494Cu; // 'SFIL'
	char szFileName[MAX_PATH] = {};
	HANDLE hDirectFile = INVALID_HANDLE_VALUE;
	StormArchiveRecord* pArchive = nullptr;
	uint32_t dwBlockIndex = 0;
	MpqBlockEntry block = {};
	uint32_t dwFileKey = 0;
	uint32_t dwFilePos = 0;
	std::vector<uint32_t> sectorOffsets;
	bool bSectorOffsetsLoaded = false;

	uint32_t cachedSectorIndex = 0xFFFFFFFFu;
	std::vector<uint8_t> cachedSectorData;
	std::vector<uint8_t> singleUnitData;
	bool bSingleUnitLoaded = false;
};

std::recursive_mutex g_MpqMutex;
std::vector<StormArchiveRecord*> g_OpenArchives;
char g_szBasePath[MAX_PATH] = {};
uint32_t g_dwDirectAccessFlags = 0;
LCID g_lcFileLocale = 0;

bool ReadDiskAt(HANDLE hFile, uint64_t offset, void* pBuf, uint32_t cbToRead, uint32_t* pcbRead = nullptr)
{
	LARGE_INTEGER liPos = {};
	liPos.QuadPart = static_cast<LONGLONG>(offset);
	if (!SetFilePointerEx(hFile, liPos, nullptr, FILE_BEGIN))
	{
		return false;
	}
	DWORD dwRead = 0;
	if (!ReadFile(hFile, pBuf, cbToRead, &dwRead, nullptr))
	{
		return false;
	}
	if (pcbRead)
	{
		*pcbRead = dwRead;
	}
	return dwRead == cbToRead;
}

uint32_t FindMpqBlockIndex(const StormArchiveRecord* pArc, const char* szFileName)
{
	if (!pArc || pArc->hashTable.empty() || !szFileName || !szFileName[0])
	{
		return 0xFFFFFFFFu;
	}
	const uint32_t nHashSize = static_cast<uint32_t>(pArc->hashTable.size());
	const uint32_t dwStart = MpqHashString(szFileName, 0) & (nHashSize - 1u);
	const uint32_t dwName1 = MpqHashString(szFileName, 1);
	const uint32_t dwName2 = MpqHashString(szFileName, 2);

	uint32_t dwNeutralFallback = 0xFFFFFFFFu;
	for (uint32_t step = 0; step < nHashSize; ++step)
	{
		const uint32_t idx = (dwStart + step) & (nHashSize - 1u);
		const MpqHashEntry& he = pArc->hashTable[idx];
		if (he.dwBlockIndex == 0xFFFFFFFFu)
		{
			break;
		}
		if (he.dwBlockIndex == 0xFFFFFFFEu)
		{
			continue;
		}
		if (he.dwName1 == dwName1 && he.dwName2 == dwName2 && he.dwBlockIndex < pArc->blockTable.size())
		{
			if (pArc->blockTable[he.dwBlockIndex].dwFlags & MPQ_FLAG_EXISTS)
			{
				if (he.lcLocale == static_cast<uint16_t>(g_lcFileLocale))
				{
					return he.dwBlockIndex;
				}
				if (dwNeutralFallback == 0xFFFFFFFFu)
				{
					dwNeutralFallback = he.dwBlockIndex;
				}
			}
		}
	}
	return dwNeutralFallback;
}

const char* GetPlainFileName(const char* szPath)
{
	const char* pPlain = szPath;
	for (const char* p = szPath; *p != '\0'; ++p)
	{
		if (*p == '\\' || *p == '/' || *p == ':')
		{
			pPlain = p + 1;
		}
	}
	return pPlain;
}

bool DecompressMpqChunk(
	uint32_t dwBlockFlags,
	uint8_t* pDst,
	uint32_t cbUncompressed,
	const uint8_t* pSrc,
	uint32_t cbCompressed)
{
	if (cbCompressed == cbUncompressed)
	{
		std::memcpy(pDst, pSrc, cbUncompressed);
		return true;
	}
	if ((dwBlockFlags & MPQ_FLAG_IMPLODE) != 0 && (dwBlockFlags & MPQ_FLAG_COMPRESS) == 0)
	{
		const uint32_t cbOut = StormDecompressPkware(pDst, cbUncompressed, pSrc, cbCompressed);
		return cbOut == cbUncompressed;
	}
	if ((dwBlockFlags & MPQ_FLAG_COMPRESS) != 0)
	{
		int nOutSize = static_cast<int>(cbUncompressed);
		if (!SCompDecompress(pDst, &nOutSize, pSrc, cbCompressed))
		{
			return false;
		}
		return static_cast<uint32_t>(nOutSize) == cbUncompressed;
	}
	return false;
}

bool EnsureSectorOffsetsLoaded(StormFileRecord* pFile)
{
	if (pFile->bSectorOffsetsLoaded)
	{
		return true;
	}
	pFile->bSectorOffsetsLoaded = true;
	if ((pFile->block.dwFlags & (MPQ_FLAG_COMPRESS | MPQ_FLAG_IMPLODE)) == 0 ||
		(pFile->block.dwFlags & MPQ_FLAG_SINGLE_UNIT) != 0 ||
		pFile->block.dwFSize == 0)
	{
		return true;
	}

	const uint32_t nSectors = (pFile->block.dwFSize + pFile->pArchive->dwSectorSize - 1u) / pFile->pArchive->dwSectorSize;
	const uint32_t nTableEntries = nSectors + ((pFile->block.dwFlags & MPQ_FLAG_SECTOR_CRC) ? 2u : 1u);
	pFile->sectorOffsets.resize(nTableEntries);

	const uint64_t diskOffset = static_cast<uint64_t>(pFile->pArchive->dwArchiveOffset) + pFile->block.dwFilePos;
	if (!ReadDiskAt(pFile->pArchive->hDiskFile, diskOffset, pFile->sectorOffsets.data(), nTableEntries * sizeof(uint32_t)))
	{
		pFile->sectorOffsets.clear();
		return false;
	}
	if (pFile->block.dwFlags & MPQ_FLAG_ENCRYPTED)
	{
		MpqDecryptBlock(pFile->sectorOffsets.data(), nTableEntries * sizeof(uint32_t), pFile->dwFileKey - 1u);
	}
	return true;
}

bool LoadMpqSector(StormFileRecord* pFile, uint32_t nSectorIndex)
{
	if (pFile->cachedSectorIndex == nSectorIndex && !pFile->cachedSectorData.empty())
	{
		return true;
	}
	const uint32_t dwSectorSize = pFile->pArchive->dwSectorSize;
	const uint32_t dwSectorStartByte = nSectorIndex * dwSectorSize;
	if (dwSectorStartByte >= pFile->block.dwFSize)
	{
		return false;
	}
	const uint32_t cbRawSector = std::min(dwSectorSize, pFile->block.dwFSize - dwSectorStartByte);

	uint32_t dwRelOffset = dwSectorStartByte;
	uint32_t cbStoredSector = cbRawSector;
	if ((pFile->block.dwFlags & (MPQ_FLAG_COMPRESS | MPQ_FLAG_IMPLODE)) != 0)
	{
		if (!EnsureSectorOffsetsLoaded(pFile) || nSectorIndex + 1 >= pFile->sectorOffsets.size())
		{
			return false;
		}
		dwRelOffset = pFile->sectorOffsets[nSectorIndex];
		if (pFile->sectorOffsets[nSectorIndex + 1] < dwRelOffset)
		{
			return false;
		}
		cbStoredSector = pFile->sectorOffsets[nSectorIndex + 1] - dwRelOffset;
	}

	std::vector<uint8_t> rawBuf(cbStoredSector);
	const uint64_t diskPos = static_cast<uint64_t>(pFile->pArchive->dwArchiveOffset) + pFile->block.dwFilePos + dwRelOffset;
	if (!ReadDiskAt(pFile->pArchive->hDiskFile, diskPos, rawBuf.data(), cbStoredSector))
	{
		return false;
	}
	if (pFile->block.dwFlags & MPQ_FLAG_ENCRYPTED)
	{
		MpqDecryptBlock(rawBuf.data(), cbStoredSector, pFile->dwFileKey + nSectorIndex);
	}

	pFile->cachedSectorData.resize(cbRawSector);
	if ((pFile->block.dwFlags & (MPQ_FLAG_COMPRESS | MPQ_FLAG_IMPLODE)) != 0 && cbStoredSector < cbRawSector)
	{
		if (!DecompressMpqChunk(pFile->block.dwFlags, pFile->cachedSectorData.data(), cbRawSector, rawBuf.data(), cbStoredSector))
		{
			pFile->cachedSectorIndex = 0xFFFFFFFFu;
			return false;
		}
	}
	else
	{
		std::memcpy(pFile->cachedSectorData.data(), rawBuf.data(), cbRawSector);
	}

	pFile->cachedSectorIndex = nSectorIndex;
	return true;
}

bool ReadFromStormFileLocked(StormFileRecord* pFile, uint32_t dwStartOffset, uint8_t* pDst, uint32_t cbToRead, uint32_t* pcbActualRead)
{
	if (pcbActualRead)
	{
		*pcbActualRead = 0;
	}
	if (!pFile)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return false;
	}
	if (cbToRead == 0)
	{
		return true;
	}

	if (pFile->hDirectFile != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER liSize = {};
		if (!GetFileSizeEx(pFile->hDirectFile, &liSize))
		{
			return false;
		}
		const uint32_t dwTotal = static_cast<uint32_t>(liSize.QuadPart);
		if (dwStartOffset >= dwTotal)
		{
			SetLastError(ERROR_HANDLE_EOF);
			return false;
		}
		const uint32_t cbAvail = std::min(cbToRead, dwTotal - dwStartOffset);
		uint32_t cbGot = 0;
		LARGE_INTEGER liPos = {};
		liPos.QuadPart = dwStartOffset;
		if (!SetFilePointerEx(pFile->hDirectFile, liPos, nullptr, FILE_BEGIN))
		{
			return false;
		}
		DWORD dwRead = 0;
		if (!ReadFile(pFile->hDirectFile, pDst, cbAvail, &dwRead, nullptr))
		{
			return false;
		}
		cbGot = dwRead;
		if (pcbActualRead)
		{
			*pcbActualRead = cbGot;
		}
		if (cbGot != cbToRead)
		{
			SetLastError(ERROR_HANDLE_EOF);
			return false;
		}
		return true;
	}

	if (!pFile->pArchive)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return false;
	}
	if (dwStartOffset >= pFile->block.dwFSize)
	{
		SetLastError(ERROR_HANDLE_EOF);
		return false;
	}

	const uint32_t cbAvail = std::min(cbToRead, pFile->block.dwFSize - dwStartOffset);
	if (pFile->block.dwFlags & MPQ_FLAG_SINGLE_UNIT)
	{
		if (!pFile->bSingleUnitLoaded)
		{
			std::vector<uint8_t> compBuf(pFile->block.dwCSize);
			const uint64_t diskPos = static_cast<uint64_t>(pFile->pArchive->dwArchiveOffset) + pFile->block.dwFilePos;
			if (!ReadDiskAt(pFile->pArchive->hDiskFile, diskPos, compBuf.data(), pFile->block.dwCSize))
			{
				return false;
			}
			if (pFile->block.dwFlags & MPQ_FLAG_ENCRYPTED)
			{
				MpqDecryptBlock(compBuf.data(), pFile->block.dwCSize, pFile->dwFileKey);
			}
			pFile->singleUnitData.resize(pFile->block.dwFSize);
			if (!DecompressMpqChunk(
					pFile->block.dwFlags,
					pFile->singleUnitData.data(),
					pFile->block.dwFSize,
					compBuf.data(),
					pFile->block.dwCSize))
			{
				pFile->singleUnitData.clear();
				return false;
			}
			pFile->bSingleUnitLoaded = true;
		}
		std::memcpy(pDst, pFile->singleUnitData.data() + dwStartOffset, cbAvail);
		if (pcbActualRead)
		{
			*pcbActualRead = cbAvail;
		}
		if (cbAvail != cbToRead)
		{
			SetLastError(ERROR_HANDLE_EOF);
			return false;
		}
		return true;
	}

	const uint32_t dwSectorSize = pFile->pArchive->dwSectorSize;
	uint32_t dwCurPos = dwStartOffset;
	uint32_t dwCopied = 0;
	while (dwCopied < cbAvail)
	{
		const uint32_t nSecIdx = dwCurPos / dwSectorSize;
		const uint32_t nSecOff = dwCurPos % dwSectorSize;
		if (!LoadMpqSector(pFile, nSecIdx))
		{
			return false;
		}
		const uint32_t nChunk = std::min(cbAvail - dwCopied, static_cast<uint32_t>(pFile->cachedSectorData.size()) - nSecOff);
		std::memcpy(pDst + dwCopied, pFile->cachedSectorData.data() + nSecOff, nChunk);
		dwCopied += nChunk;
		dwCurPos += nChunk;
	}

	if (pcbActualRead)
	{
		*pcbActualRead = dwCopied;
	}
	if (dwCopied != cbToRead)
	{
		SetLastError(ERROR_HANDLE_EOF);
		return false;
	}
	return true;
}

bool TryOpenLocalFile(const char* szFileName, HANDLE* phDiskOut)
{
	*phDiskOut = INVALID_HANDLE_VALUE;
	char szCandidate[MAX_PATH] = {};

	auto tryPath = [&](const char* szTry) -> bool {
		const DWORD dwAttr = GetFileAttributesA(szTry);
		if (dwAttr == INVALID_FILE_ATTRIBUTES || (dwAttr & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{
			return false;
		}
		HANDLE h = CreateFileA(szTry, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
		if (h == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		*phDiskOut = h;
		return true;
	};

	if (szFileName[0] == '\\' || std::strstr(szFileName, ":\\") || std::strstr(szFileName, "\\\\"))
	{
		return tryPath(szFileName);
	}
	if (g_szBasePath[0] != '\0')
	{
		std::snprintf(szCandidate, sizeof(szCandidate), "%s\\%s", g_szBasePath, szFileName);
		if (tryPath(szCandidate))
		{
			return true;
		}
	}
	return tryPath(szFileName);
}

// ============================================================================
// 5. Storm DirectSound DDA Streaming Audio (#254..#261)
// ============================================================================
struct StormDdaStream
{
	HANDLE hFile = nullptr;
	IDirectSoundBuffer* pDSBuffer = nullptr;
	bool bOwnsBuffer = false;
	bool bLoop = false;
	bool bFinished = false;
	uint8_t nSilenceByte = 0;
	uint32_t dwBufferSize = 0;
	uint32_t dwDataOffset = 0;
	uint32_t dwDataSize = 0;
	uint32_t dwFileReadRel = 0;     // bytes read into ring buffer (mod dwDataSize if looping)
	uint32_t dwNextWriteCursor = 0; // byte offset in pDSBuffer to write next
	uint32_t dwLastPlayCursor = 0;
	uint64_t uTotalWrittenBytes = 0;
	uint64_t uTotalPlayedBytes = 0;
	int32_t nVolume = 0;
	int32_t nPan = 0;
};

std::recursive_mutex g_DdaMutex;
IDirectSound* g_pDdaDirectSound = nullptr;
std::vector<StormDdaStream*> g_DdaStreams;
HANDLE g_hDdaThread = nullptr;
HANDLE g_hDdaWakeEvent = nullptr;
volatile bool g_bDdaStopThread = false;

void FillDdaBufferRegion(StormDdaStream* pStream, uint32_t dwWriteOffset, uint32_t cbToWrite)
{
	if (!pStream || !pStream->pDSBuffer || cbToWrite == 0)
	{
		return;
	}

	LPVOID pPtr1 = nullptr;
	DWORD dwBytes1 = 0;
	LPVOID pPtr2 = nullptr;
	DWORD dwBytes2 = 0;
	HRESULT hr = pStream->pDSBuffer->Lock(dwWriteOffset, cbToWrite, &pPtr1, &dwBytes1, &pPtr2, &dwBytes2, 0);
	if (hr == DSERR_BUFFERLOST)
	{
		pStream->pDSBuffer->Restore();
		hr = pStream->pDSBuffer->Lock(dwWriteOffset, cbToWrite, &pPtr1, &dwBytes1, &pPtr2, &dwBytes2, 0);
	}
	if (FAILED(hr))
	{
		return;
	}

	auto fillSpan = [&](uint8_t* pDst, uint32_t cbSpan) {
		uint32_t cbDone = 0;
		while (cbDone < cbSpan)
		{
			if (pStream->dwFileReadRel >= pStream->dwDataSize)
			{
				if (pStream->bLoop && pStream->dwDataSize > 0)
				{
					pStream->dwFileReadRel = 0;
				}
				else
				{
					std::memset(pDst + cbDone, pStream->nSilenceByte, cbSpan - cbDone);
					cbDone = cbSpan;
					break;
				}
			}

			const uint32_t cbChunk = std::min(cbSpan - cbDone, pStream->dwDataSize - pStream->dwFileReadRel);
			SFileSetFilePointer(reinterpret_cast<int>(pStream->hFile), static_cast<LONG>(pStream->dwDataOffset + pStream->dwFileReadRel), 0, FILE_BEGIN);
			DWORD dwGot = 0;
			if (!SFileReadFile(reinterpret_cast<int>(pStream->hFile), reinterpret_cast<int>(pDst + cbDone), static_cast<int>(cbChunk), reinterpret_cast<int>(&dwGot), 0) || dwGot == 0)
			{
				std::memset(pDst + cbDone, pStream->nSilenceByte, cbSpan - cbDone);
				pStream->dwFileReadRel = pStream->dwDataSize;
				cbDone = cbSpan;
				break;
			}
			pStream->dwFileReadRel += dwGot;
			pStream->uTotalWrittenBytes += dwGot;
			cbDone += dwGot;
		}
	};

	if (pPtr1 && dwBytes1 > 0)
	{
		fillSpan(static_cast<uint8_t*>(pPtr1), dwBytes1);
	}
	if (pPtr2 && dwBytes2 > 0)
	{
		fillSpan(static_cast<uint8_t*>(pPtr2), dwBytes2);
	}
	pStream->pDSBuffer->Unlock(pPtr1, dwBytes1, pPtr2, dwBytes2);
	pStream->dwNextWriteCursor = (dwWriteOffset + cbToWrite) % pStream->dwBufferSize;
}

void PumpDdaStreamLocked(StormDdaStream* pStream)
{
	if (!pStream || !pStream->pDSBuffer || pStream->bFinished || pStream->dwBufferSize == 0)
	{
		return;
	}

	DWORD dwPlay = 0;
	DWORD dwWrite = 0;
	if (FAILED(pStream->pDSBuffer->GetCurrentPosition(&dwPlay, &dwWrite)))
	{
		return;
	}

	const uint32_t dwAdvanced = (dwPlay >= pStream->dwLastPlayCursor)
		? (dwPlay - pStream->dwLastPlayCursor)
		: (pStream->dwBufferSize - pStream->dwLastPlayCursor + dwPlay);
	pStream->dwLastPlayCursor = dwPlay;
	pStream->uTotalPlayedBytes += dwAdvanced;

	if (!pStream->bLoop && pStream->uTotalPlayedBytes >= pStream->uTotalWrittenBytes && pStream->dwFileReadRel >= pStream->dwDataSize)
	{
		pStream->pDSBuffer->Stop();
		pStream->bFinished = true;
		return;
	}

	const uint32_t dwAvailToFill = (dwPlay >= pStream->dwNextWriteCursor)
		? (dwPlay - pStream->dwNextWriteCursor)
		: (pStream->dwBufferSize - pStream->dwNextWriteCursor + dwPlay);

	const uint32_t kMinChunk = std::min<uint32_t>(0x4000u, pStream->dwBufferSize / 4u);
	if (dwAvailToFill >= std::max<uint32_t>(kMinChunk, 512u))
	{
		FillDdaBufferRegion(pStream, pStream->dwNextWriteCursor, dwAvailToFill);
	}
}

DWORD WINAPI StormDdaWorkerProc(LPVOID)
{
	while (!g_bDdaStopThread)
	{
		if (g_hDdaWakeEvent)
		{
			WaitForSingleObject(g_hDdaWakeEvent, 40);
		}
		else
		{
			Sleep(40);
		}
		if (g_bDdaStopThread)
		{
			break;
		}
		std::lock_guard<std::recursive_mutex> lock(g_DdaMutex);
		for (StormDdaStream* pStream : g_DdaStreams)
		{
			PumpDdaStreamLocked(pStream);
		}
	}
	return 0;
}

void EnsureDdaThreadLocked()
{
	if (!g_hDdaWakeEvent)
	{
		g_hDdaWakeEvent = CreateEventA(nullptr, FALSE, FALSE, nullptr);
	}
	if (!g_hDdaThread)
	{
		g_bDdaStopThread = false;
		DWORD dwTid = 0;
		g_hDdaThread = CreateThread(nullptr, 0, StormDdaWorkerProc, nullptr, 0, &dwTid);
		if (g_hDdaThread)
		{
			SetThreadPriority(g_hDdaThread, THREAD_PRIORITY_ABOVE_NORMAL);
		}
	}
}

bool FindWavChunk(HANDLE hFile, uint32_t dwTargetFourCC, uint32_t* pdwChunkSize, uint32_t* pdwChunkDataPos)
{
	SFileSetFilePointer(reinterpret_cast<int>(hFile), 12, 0, FILE_BEGIN);
	const int nTotalSize = SFileGetFileSize(reinterpret_cast<int>(hFile), nullptr);
	if (nTotalSize <= 12)
	{
		return false;
	}
	uint32_t dwCurOffset = 12;
	while (dwCurOffset + 8 <= static_cast<uint32_t>(nTotalSize))
	{
		uint32_t hdr[2] = {};
		DWORD dwGot = 0;
		SFileSetFilePointer(reinterpret_cast<int>(hFile), static_cast<LONG>(dwCurOffset), 0, FILE_BEGIN);
		if (!SFileReadFile(reinterpret_cast<int>(hFile), reinterpret_cast<int>(hdr), 8, reinterpret_cast<int>(&dwGot), 0) || dwGot != 8)
		{
			return false;
		}
		if (hdr[0] == dwTargetFourCC)
		{
			*pdwChunkSize = hdr[1];
			*pdwChunkDataPos = dwCurOffset + 8;
			return true;
		}
		dwCurOffset += 8 + ((hdr[1] + 1u) & ~1u);
	}
	return false;
}
} // namespace

// ============================================================================
// Exported Storm Functions
// ============================================================================

// Storm.#301
BOOL __cdecl StormDestroy()
{
	SFileDdaDestroy();
	SFileDestroy();
	return TRUE;
}

// Storm.#303
BOOL __stdcall StormGetOption(int type, int optval, size_t optlen)
{
	uint32_t* pOut = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(optval)));
	if (!pOut || optlen != sizeof(uint32_t))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	switch (type)
	{
	case 1: *pOut = g_StormOpt1; break;
	case 2: *pOut = g_StormOpt2; break;
	case 3: *pOut = g_StormOpt3; break;
	case 4: *pOut = g_StormOpt4; break;
	case 5: *pOut = g_StormOpt5; break;
	default:
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	SetLastError(ERROR_SUCCESS);
	return TRUE;
}

// Storm.#304 (1.12a: 0x6FC123C0)
BOOL __stdcall StormSetOption(int type, int optval, size_t optlen)
{
	const uint32_t* pVal = reinterpret_cast<const uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(optval)));
	if (!pVal || optlen != sizeof(uint32_t))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	switch (type)
	{
	case 1:
		g_StormOpt1 = *pVal;
		break;
	case 2:
		if (*pVal == 0 || ((*pVal & (*pVal - 1u)) != 0))
		{
			SetLastError(ERROR_INVALID_PARAMETER);
			return FALSE;
		}
		g_StormOpt2 = *pVal;
		break;
	case 3:
		g_StormOpt3 = *pVal;
		break;
	case 4:
		g_StormOpt4 = *pVal;
		break;
	case 5:
		g_StormOpt5 = *pVal;
		break;
	default:
		break;
	}
	SetLastError(ERROR_SUCCESS);
	return TRUE;
}

// Storm.#412 (1.12a: 0x6FC149B0)
BOOL __stdcall SMsgDispatchMessage(HWND hWnd, DWORD messageID, WPARAM wParam, LPARAM lParam, BOOL* bUseResult, LRESULT* pResult)
{
	if (bUseResult) *bUseResult = FALSE;
	if (pResult) *pResult = 0;

	SMSGHANDLER_PARAMS params = {};
	params.hWindow = hWnd;
	params.nMessage = messageID;
	params.wParam = wParam;
	params.lParam = lParam;
	params.nNotifyCode = (messageID == WM_COMMAND) ? static_cast<UINT>(HIWORD(wParam)) : 0u;
	params.pExtra = nullptr;
	params.bUseResult = FALSE;
	params.lResult = 0;

	HWND hCurTarget = hWnd;
	for (;;)
	{
		DispatchEvtChain(SEVT_TYPE_MSG, hCurTarget, messageID, &params);
		if (messageID == WM_COMMAND)
		{
			DispatchEvtChain(SEVT_TYPE_CMD, hCurTarget, static_cast<uint32_t>(LOWORD(wParam)), &params);
		}
		else if (messageID == WM_KEYDOWN)
		{
			DispatchEvtChain(SEVT_TYPE_KEYDOWN, hCurTarget, static_cast<uint32_t>(wParam), &params);
		}
		else if (messageID == WM_KEYUP)
		{
			DispatchEvtChain(SEVT_TYPE_KEYUP, hCurTarget, static_cast<uint32_t>(wParam), &params);
		}

		if (hCurTarget == nullptr || hCurTarget != g_SMsgDefaultWindow)
		{
			if (messageID == WM_NCDESTROY)
			{
				CleanupWindowMsgStacks(hWnd);
			}
			if (bUseResult) *bUseResult = params.bUseResult;
			if (pResult) *pResult = params.lResult;
			return TRUE;
		}
		hCurTarget = nullptr;
	}
}

// Storm.#414
BOOL __stdcall SMsgRegisterCommand(HWND hWnd, uint32_t nId, SMSGHANDLER pHandler)
{
	return RegisterMsgInternal(SEVT_TYPE_CMD, hWnd, nId, pHandler);
}

// Storm.#415
BOOL __stdcall SMsgRegisterKeyDown(HWND hWnd, uint32_t nId, SMSGHANDLER pHandler)
{
	return RegisterMsgInternal(SEVT_TYPE_KEYDOWN, hWnd, nId, pHandler);
}

// Storm.#416
BOOL __stdcall SMsgRegisterKeyUp(HWND hWnd, uint32_t nId, SMSGHANDLER pHandler)
{
	return RegisterMsgInternal(SEVT_TYPE_KEYUP, hWnd, nId, pHandler);
}

// Storm.#417
BOOL __stdcall SMsgRegisterMessage(HWND hWnd, uint32_t nId, SMSGHANDLER pHandler)
{
	return RegisterMsgInternal(SEVT_TYPE_MSG, hWnd, nId, pHandler);
}

// Storm.#418 (1.12a: 0x6FC148D0)
BOOL __stdcall SMsgPopRegisterState(int type)
{
	HWND hWnd = reinterpret_cast<HWND>(static_cast<intptr_t>(type));
	std::lock_guard<std::recursive_mutex> lock(g_SMsgMutex);
	static const uint32_t kTypes[4] = { SEVT_TYPE_CMD, SEVT_TYPE_KEYDOWN, SEVT_TYPE_KEYUP, SEVT_TYPE_MSG };
	for (uint32_t t : kTypes)
	{
		if (StormMsgWindowStack* pStack = FindMsgStack(t, hWnd, false))
		{
			if (!pStack->layers.empty())
			{
				pStack->layers.pop_back();
			}
		}
	}
	return TRUE;
}

// Storm.#419 (1.12a: 0x6FC14D50)
BOOL __stdcall SMsgPushRegisterState(int type)
{
	HWND hWnd = reinterpret_cast<HWND>(static_cast<intptr_t>(type));
	std::lock_guard<std::recursive_mutex> lock(g_SMsgMutex);
	static const uint32_t kTypes[4] = { SEVT_TYPE_CMD, SEVT_TYPE_KEYDOWN, SEVT_TYPE_KEYUP, SEVT_TYPE_MSG };
	for (uint32_t t : kTypes)
	{
		StormMsgWindowStack* pStack = FindMsgStack(t, hWnd, true);
		pStack->layers.emplace_back();
	}
	return TRUE;
}

// Storm.#511 (1.12a: 0x6FC14AE0)
BOOL __stdcall SMsgBreakHandlerChain(SMSGHANDLER_PARAMS* pData)
{
	if (!pData)
	{
		return FALSE;
	}
	std::lock_guard<std::recursive_mutex> lock(g_SMsgMutex);
	g_SMsgBreakList.push_back(pData);
	return TRUE;
}

// Storm.#512
BOOL __stdcall SMsgUnregisterCommand(HWND hWnd, uint32_t nId, SMSGHANDLER pHandler)
{
	return UnregisterMsgInternal(SEVT_TYPE_CMD, hWnd, nId, pHandler);
}

// Storm.#513
BOOL __stdcall SMsgUnregisterKeyDown(HWND hWnd, uint32_t nId, SMSGHANDLER pHandler)
{
	return UnregisterMsgInternal(SEVT_TYPE_KEYDOWN, hWnd, nId, pHandler);
}

// Storm.#514
BOOL __stdcall SMsgUnregisterKeyUp(HWND hWnd, uint32_t nId, SMSGHANDLER pHandler)
{
	return UnregisterMsgInternal(SEVT_TYPE_KEYUP, hWnd, nId, pHandler);
}

// Storm.#515
BOOL __stdcall SMsgUnregisterMessage(HWND hWnd, uint32_t nId, SMSGHANDLER pHandler)
{
	return UnregisterMsgInternal(SEVT_TYPE_MSG, hWnd, nId, pHandler);
}

// Storm.#517
BOOL __stdcall SMsgSetDefaultWindow(int a1)
{
	g_SMsgDefaultWindow = reinterpret_cast<HWND>(static_cast<intptr_t>(a1));
	return TRUE;
}

// Storm.#518
HWND __cdecl SMsgGetDefaultWindow()
{
	return g_SMsgDefaultWindow;
}

// Storm.#321 (1.12a: 0x6FC27200)
BOOL __stdcall SBmpDecodeImage(
	int nImageType,
	signed int* pSrcBuffer,
	unsigned int dwSrcSize,
	int pPaletteAddr,
	int pPixelBufferAddr,
	int nPixelBufferSize,
	int pWidthAddr,
	int pHeightAddr,
	int pBppAddr)
{
	int32_t* pWidth = reinterpret_cast<int32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(pWidthAddr)));
	int32_t* pHeight = reinterpret_cast<int32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(pHeightAddr)));
	int32_t* pBpp = reinterpret_cast<int32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(pBppAddr)));
	PALETTEENTRY* pPalette = reinterpret_cast<PALETTEENTRY*>(static_cast<uintptr_t>(static_cast<uint32_t>(pPaletteAddr)));
	uint8_t* pDstPixels = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(pPixelBufferAddr)));

	if (pWidth) *pWidth = 0;
	if (pHeight) *pHeight = 0;
	if (pBpp) *pBpp = 0;

	const uint8_t* pbSrc = reinterpret_cast<const uint8_t*>(pSrcBuffer);
	if (!pbSrc || dwSrcSize < 2 || (pDstPixels != nullptr && nPixelBufferSize <= 0))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	if (nImageType == 0)
	{
		const uint16_t magic = static_cast<uint16_t>(pbSrc[0] | (pbSrc[1] << 8));
		if (magic == 0x050Au)
		{
			nImageType = 2; // PCX
		}
		else if (magic == 0x4D42u)
		{
			nImageType = 1; // BMP ('BM')
		}
		else
		{
			nImageType = 3; // TGA
		}
	}

	if (nImageType == 2) // PCX (1.12a: 0x6FC26790)
	{
		if (dwSrcSize < 128)
		{
			return FALSE;
		}
		const uint16_t xMin = static_cast<uint16_t>(pbSrc[4] | (pbSrc[5] << 8));
		const uint16_t yMin = static_cast<uint16_t>(pbSrc[6] | (pbSrc[7] << 8));
		const uint16_t xMax = static_cast<uint16_t>(pbSrc[8] | (pbSrc[9] << 8));
		const uint16_t yMax = static_cast<uint16_t>(pbSrc[10] | (pbSrc[11] << 8));
		const uint8_t bitsPerPlane = pbSrc[3];
		const uint8_t numPlanes = pbSrc[65];
		const uint16_t bytesPerLine = static_cast<uint16_t>(pbSrc[66] | (pbSrc[67] << 8));

		const int32_t width = static_cast<int32_t>(xMax) - static_cast<int32_t>(xMin) + 1;
		const int32_t height = static_cast<int32_t>(yMax) - static_cast<int32_t>(yMin) + 1;
		const int32_t bpp = static_cast<int32_t>(bitsPerPlane) * static_cast<int32_t>(numPlanes);
		if (width <= 0 || height <= 0 || bytesPerLine == 0 || numPlanes == 0)
		{
			return FALSE;
		}
		if (pWidth) *pWidth = width;
		if (pHeight) *pHeight = height;
		if (pBpp) *pBpp = bpp;

		if (pDstPixels)
		{
			const uint32_t rleEnd = (bpp == 8 && dwSrcSize >= 128 + 769) ? (dwSrcSize - 769u) : dwSrcSize;
			uint32_t srcPos = 128;
			const int32_t rowStride = width * static_cast<int32_t>(numPlanes);
			uint8_t* pRow = pDstPixels;
			uint8_t* const pDstEnd = pDstPixels + nPixelBufferSize;
			uint32_t x = 0;
			int32_t plane = static_cast<int32_t>(numPlanes) - 1;

			while (srcPos < rleEnd && pRow + x < pDstEnd)
			{
				uint8_t val = pbSrc[srcPos++];
				uint32_t count = 1;
				if (val >= 0xC0u)
				{
					count = val & 0x3Fu;
					if (srcPos >= rleEnd) break;
					val = pbSrc[srcPos++];
				}
				while (count > 0 && pRow < pDstEnd)
				{
					if (x < static_cast<uint32_t>(width))
					{
						uint8_t* pPx = (numPlanes == 1) ? (pRow + x) : (pRow + x * numPlanes + plane);
						if (pPx < pDstEnd)
						{
							*pPx = val;
						}
					}
					++x;
					if (x >= bytesPerLine)
					{
						x = 0;
						--plane;
						if (plane < 0)
						{
							plane = static_cast<int32_t>(numPlanes) - 1;
							pRow += rowStride;
						}
					}
					--count;
				}
			}
		}

		if (pPalette && bpp == 8 && dwSrcSize >= 768)
		{
			const uint8_t* pPalSrc = pbSrc + dwSrcSize - 768;
			for (int i = 0; i < 256; ++i)
			{
				pPalette[i].peRed = pPalSrc[i * 3 + 0];
				pPalette[i].peGreen = pPalSrc[i * 3 + 1];
				pPalette[i].peBlue = pPalSrc[i * 3 + 2];
				pPalette[i].peFlags = 0;
			}
		}
		return TRUE;
	}

	if (nImageType == 1) // BMP (1.12a: 0x6FC269C0)
	{
		if (dwSrcSize < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
		{
			return FALSE;
		}
		const auto* pBf = reinterpret_cast<const BITMAPFILEHEADER*>(pbSrc);
		const auto* pBi = reinterpret_cast<const BITMAPINFOHEADER*>(pbSrc + sizeof(BITMAPFILEHEADER));
		const int32_t width = pBi->biWidth;
		const int32_t absHeight = (pBi->biHeight < 0) ? -pBi->biHeight : pBi->biHeight;
		const int32_t bpp = pBi->biBitCount;
		if (width <= 0 || absHeight <= 0)
		{
			return FALSE;
		}
		if (pWidth) *pWidth = width;
		if (pHeight) *pHeight = absHeight;
		if (pBpp) *pBpp = bpp;

		if (pPalette && bpp <= 8)
		{
			const uint32_t nColors = (pBi->biClrUsed != 0 && pBi->biClrUsed <= 256) ? pBi->biClrUsed : (1u << bpp);
			const auto* pRgb = reinterpret_cast<const RGBQUAD*>(pbSrc + sizeof(BITMAPFILEHEADER) + pBi->biSize);
			for (uint32_t i = 0; i < std::min<uint32_t>(nColors, 256u); ++i)
			{
				pPalette[i].peRed = pRgb[i].rgbRed;
				pPalette[i].peGreen = pRgb[i].rgbGreen;
				pPalette[i].peBlue = pRgb[i].rgbBlue;
				pPalette[i].peFlags = 0;
			}
		}

		if (pDstPixels && pBf->bfOffBits < dwSrcSize)
		{
			const uint32_t unpaddedRow = static_cast<uint32_t>((width * bpp + 7) / 8);
			const uint32_t srcStride = (unpaddedRow + 3u) & ~3u;
			const uint8_t* pBits = pbSrc + pBf->bfOffBits;
			const bool bBottomUp = (pBi->biHeight > 0);
			uint32_t dstOff = 0;
			for (int32_t y = 0; y < absHeight && dstOff < static_cast<uint32_t>(nPixelBufferSize); ++y)
			{
				const int32_t srcRow = bBottomUp ? (absHeight - 1 - y) : y;
				if (pBf->bfOffBits + static_cast<uint32_t>(srcRow) * srcStride >= dwSrcSize)
				{
					break;
				}
				const uint32_t nCopy = std::min(unpaddedRow, static_cast<uint32_t>(nPixelBufferSize) - dstOff);
				std::memcpy(pDstPixels + dstOff, pBits + static_cast<uint32_t>(srcRow) * srcStride, nCopy);
				dstOff += nCopy;
			}
		}
		return TRUE;
	}

	return FALSE;
}

// Storm.#552 (1.12a: 0x6FC138E0)
BOOL __stdcall SCompDecompress(void* pOutBuffer, int* pcbOutSize, const void* pInBuffer, unsigned int cbInSize)
{
	if (!pOutBuffer || !pcbOutSize || *pcbOutSize <= 0 || !pInBuffer || cbInSize == 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	const uint32_t cbTargetMax = static_cast<uint32_t>(*pcbOutSize);
	if (cbInSize == cbTargetMax)
	{
		if (pOutBuffer != pInBuffer)
		{
			std::memcpy(pOutBuffer, pInBuffer, cbInSize);
		}
		return TRUE;
	}

	const uint8_t* pbIn = static_cast<const uint8_t*>(pInBuffer);
	const uint8_t fMask = pbIn[0];
	const uint8_t* pCurIn = pbIn + 1;
	uint32_t cbCurIn = cbInSize - 1;

	// Supported compression bits: 0x08 (PKWARE), 0x02 (zlib), 0x01 (Huffman), 0x80 (ADPCM stereo), 0x40 (ADPCM mono)
	if ((fMask & ~(0x08u | 0x02u | 0x01u | 0x80u | 0x40u)) != 0)
	{
		return FALSE;
	}

	struct CodecStage
	{
		uint8_t bit;
	};
	static const CodecStage kOrder[5] = { { 0x08u }, { 0x02u }, { 0x01u }, { 0x80u }, { 0x40u } };

	std::vector<uint8_t> bufA;
	std::vector<uint8_t> bufB;
	bool bToggle = false;

	for (const auto& st : kOrder)
	{
		if ((fMask & st.bit) == 0)
		{
			continue;
		}
		std::vector<uint8_t>& dstVec = bToggle ? bufB : bufA;
		dstVec.resize(cbTargetMax);
		uint32_t cbProduced = 0;
		switch (st.bit)
		{
		case 0x08u:
			cbProduced = StormDecompressPkware(dstVec.data(), cbTargetMax, pCurIn, cbCurIn);
			break;
		case 0x02u:
			cbProduced = StormDecompressZlib(dstVec.data(), cbTargetMax, pCurIn, cbCurIn);
			break;
		case 0x01u:
			cbProduced = StormDecompressHuffman(dstVec.data(), cbTargetMax, pCurIn, cbCurIn);
			break;
		case 0x80u:
			cbProduced = StormDecompressAdpcm(dstVec.data(), cbTargetMax, pCurIn, cbCurIn, 2);
			break;
		case 0x40u:
			cbProduced = StormDecompressAdpcm(dstVec.data(), cbTargetMax, pCurIn, cbCurIn, 1);
			break;
		default:
			break;
		}
		pCurIn = dstVec.data();
		cbCurIn = cbProduced;
		bToggle = !bToggle;
	}

	std::memcpy(pOutBuffer, pCurIn, std::min(cbCurIn, cbTargetMax));
	*pcbOutSize = static_cast<int>(cbCurIn);
	return TRUE;
}

// ============================================================================
// SFile* Archive & File APIs (#252..#289)
// ============================================================================

// Storm.#252
BOOL __stdcall SFileCloseArchive(HSARCHIVE hArchive)
{
	auto* pArc = reinterpret_cast<StormArchiveRecord*>(hArchive);
	if (!pArc || pArc->dwMagic != 0x53415243u)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	std::lock_guard<std::recursive_mutex> lock(g_MpqMutex);
	auto it = std::find(g_OpenArchives.begin(), g_OpenArchives.end(), pArc);
	if (it != g_OpenArchives.end())
	{
		g_OpenArchives.erase(it);
	}
	if (pArc->hDiskFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(pArc->hDiskFile);
		pArc->hDiskFile = INVALID_HANDLE_VALUE;
	}
	pArc->dwMagic = 0;
	delete pArc;
	return TRUE;
}

// Storm.#253
BOOL __stdcall SFileCloseFile(HANDLE hFile)
{
	auto* pFile = reinterpret_cast<StormFileRecord*>(hFile);
	if (!pFile || pFile->dwMagic != 0x5346494Cu)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	std::lock_guard<std::recursive_mutex> lock(g_MpqMutex);
	if (pFile->hDirectFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(pFile->hDirectFile);
		pFile->hDirectFile = INVALID_HANDLE_VALUE;
	}
	pFile->dwMagic = 0;
	delete pFile;
	return TRUE;
}

// Storm.#262
BOOL __cdecl SFileDestroy()
{
	std::lock_guard<std::recursive_mutex> lock(g_MpqMutex);
	while (!g_OpenArchives.empty())
	{
		StormArchiveRecord* pArc = g_OpenArchives.back();
		g_OpenArchives.pop_back();
		if (pArc->hDiskFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(pArc->hDiskFile);
		}
		pArc->dwMagic = 0;
		delete pArc;
	}
	return TRUE;
}

// Storm.#263
BOOL __stdcall SFileEnableDirectAccess(HANDLE hFile)
{
	g_dwDirectAccessFlags = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(hFile));
	return TRUE;
}

// Storm.#264
BOOL __stdcall SFileGetFileArchive(HANDLE hFile, HANDLE archive)
{
	auto* pFile = reinterpret_cast<StormFileRecord*>(hFile);
	auto* phOut = reinterpret_cast<HSARCHIVE*>(archive);
	if (!pFile || pFile->dwMagic != 0x5346494Cu || !phOut)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	*phOut = reinterpret_cast<HSARCHIVE>(pFile->pArchive);
	return TRUE;
}

// Storm.#265
int __stdcall SFileGetFileSize(int hFileInt, LPDWORD lpFileSizeHigh)
{
	if (lpFileSizeHigh)
	{
		*lpFileSizeHigh = 0;
	}
	auto* pFile = reinterpret_cast<StormFileRecord*>(static_cast<uintptr_t>(static_cast<uint32_t>(hFileInt)));
	if (!pFile || pFile->dwMagic != 0x5346494Cu)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return -1;
	}
	std::lock_guard<std::recursive_mutex> lock(g_MpqMutex);
	if (pFile->hDirectFile != INVALID_HANDLE_VALUE)
	{
		return static_cast<int>(GetFileSize(pFile->hDirectFile, lpFileSizeHigh));
	}
	return static_cast<int>(pFile->block.dwFSize);
}

// Storm.#266 (1.12a: 0x6FC04110)
BOOL __stdcall SFileOpenArchive(char* szName, int nPriority, int nFlags, HSARCHIVE* phArchive)
{
	(void)nFlags;
	if (phArchive)
	{
		*phArchive = nullptr;
	}
	if (!szName || !szName[0] || !phArchive)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	std::lock_guard<std::recursive_mutex> lock(g_MpqMutex);
	EnsureMpqCryptTable();

	HANDLE hDisk = INVALID_HANDLE_VALUE;
	char szResolved[MAX_PATH] = {};
	std::snprintf(szResolved, sizeof(szResolved), "%s", szName);
	if (!TryOpenLocalFile(szResolved, &hDisk))
	{
		SetLastError(ERROR_FILE_NOT_FOUND);
		return FALSE;
	}

	LARGE_INTEGER liFileSize = {};
	if (!GetFileSizeEx(hDisk, &liFileSize) || liFileSize.QuadPart < sizeof(MpqHeaderV1))
	{
		CloseHandle(hDisk);
		SetLastError(ERROR_BAD_FORMAT);
		return FALSE;
	}

	MpqHeaderV1 hdr = {};
	uint32_t dwArcOffset = 0;
	bool bFoundHdr = false;
	const uint64_t uMaxScan = static_cast<uint64_t>(liFileSize.QuadPart) - sizeof(MpqHeaderV1);
	while (static_cast<uint64_t>(dwArcOffset) <= uMaxScan)
	{
		if (!ReadDiskAt(hDisk, dwArcOffset, &hdr, sizeof(hdr)))
		{
			break;
		}
		if (hdr.dwID == 0x1A51504Du && hdr.dwHeaderSize >= 0x20u && hdr.wBlockSizeShift < 16u)
		{
			bFoundHdr = true;
			break;
		}
		dwArcOffset += 0x200u;
	}

	if (!bFoundHdr || hdr.dwHashTableEntries == 0 || (hdr.dwHashTableEntries & (hdr.dwHashTableEntries - 1u)) != 0)
	{
		CloseHandle(hDisk);
		SetLastError(ERROR_BAD_FORMAT);
		return FALSE;
	}

	auto* pArc = new StormArchiveRecord();
	std::snprintf(pArc->szFileName, sizeof(pArc->szFileName), "%s", szResolved);
	pArc->hDiskFile = hDisk;
	pArc->nPriority = nPriority;
	pArc->dwArchiveOffset = dwArcOffset;
	pArc->dwSectorSize = 0x200u << hdr.wBlockSizeShift;
	pArc->header = hdr;

	pArc->hashTable.resize(hdr.dwHashTableEntries);
	const uint32_t cbHashBytes = hdr.dwHashTableEntries * sizeof(MpqHashEntry);
	if (!ReadDiskAt(hDisk, static_cast<uint64_t>(dwArcOffset) + hdr.dwHashTablePos, pArc->hashTable.data(), cbHashBytes))
	{
		CloseHandle(hDisk);
		delete pArc;
		return FALSE;
	}
	MpqDecryptBlock(pArc->hashTable.data(), cbHashBytes, MpqHashString("(hash table)", 3));

	pArc->blockTable.resize(hdr.dwBlockTableEntries);
	const uint32_t cbBlockBytes = hdr.dwBlockTableEntries * sizeof(MpqBlockEntry);
	if (cbBlockBytes > 0)
	{
		if (!ReadDiskAt(hDisk, static_cast<uint64_t>(dwArcOffset) + hdr.dwBlockTablePos, pArc->blockTable.data(), cbBlockBytes))
		{
			CloseHandle(hDisk);
			delete pArc;
			return FALSE;
		}
		MpqDecryptBlock(pArc->blockTable.data(), cbBlockBytes, MpqHashString("(block table)", 3));
	}

	// Insert before the first archive whose priority <= nPriority (matching 0x6FC045BD)
	auto insertPos = g_OpenArchives.begin();
	while (insertPos != g_OpenArchives.end() && (*insertPos)->nPriority > nPriority)
	{
		++insertPos;
	}
	g_OpenArchives.insert(insertPos, pArc);

	*phArchive = reinterpret_cast<HSARCHIVE>(pArc);
	return TRUE;
}

// Storm.#268 (1.12a: 0x6FC01DC0)
int __stdcall SFileOpenFileEx(int hArchiveInt, char* szFileName, char nSearchScope, int phFileAddr)
{
	auto** pphOut = reinterpret_cast<StormFileRecord**>(static_cast<uintptr_t>(static_cast<uint32_t>(phFileAddr)));
	if (pphOut)
	{
		*pphOut = nullptr;
	}
	if (!szFileName || !szFileName[0] || !pphOut)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	std::lock_guard<std::recursive_mutex> lock(g_MpqMutex);
	EnsureMpqCryptTable();

	const uint8_t uFlags = static_cast<uint8_t>(nSearchScope);
	if ((uFlags & 0x03u) != 0)
	{
		HANDLE hDirect = INVALID_HANDLE_VALUE;
		if (TryOpenLocalFile(szFileName, &hDirect))
		{
			auto* pRec = new StormFileRecord();
			std::snprintf(pRec->szFileName, sizeof(pRec->szFileName), "%s", szFileName);
			pRec->hDirectFile = hDirect;
			*pphOut = pRec;
			return 1;
		}
		if (uFlags == 0xFFFFFFFFu)
		{
			SetLastError(ERROR_FILE_NOT_FOUND);
			return 0;
		}
	}

	auto* pExplicitArc = reinterpret_cast<StormArchiveRecord*>(static_cast<uintptr_t>(static_cast<uint32_t>(hArchiveInt)));
	StormArchiveRecord* pFoundArc = nullptr;
	uint32_t dwBlockIdx = 0xFFFFFFFFu;

	if (pExplicitArc != nullptr)
	{
		if (pExplicitArc->dwMagic == 0x53415243u)
		{
			dwBlockIdx = FindMpqBlockIndex(pExplicitArc, szFileName);
			if (dwBlockIdx != 0xFFFFFFFFu)
			{
				pFoundArc = pExplicitArc;
			}
		}
	}
	else
	{
		for (StormArchiveRecord* pArc : g_OpenArchives)
		{
			dwBlockIdx = FindMpqBlockIndex(pArc, szFileName);
			if (dwBlockIdx != 0xFFFFFFFFu)
			{
				pFoundArc = pArc;
				break;
			}
		}
	}

	if (!pFoundArc || dwBlockIdx == 0xFFFFFFFFu)
	{
		SetLastError(ERROR_FILE_NOT_FOUND);
		return 0;
	}

	const MpqBlockEntry& blk = pFoundArc->blockTable[dwBlockIdx];
	auto* pFile = new StormFileRecord();
	std::snprintf(pFile->szFileName, sizeof(pFile->szFileName), "%s", szFileName);
	pFile->pArchive = pFoundArc;
	pFile->dwBlockIndex = dwBlockIdx;
	pFile->block = blk;
	pFile->dwFilePos = 0;

	if (blk.dwFlags & MPQ_FLAG_ENCRYPTED)
	{
		uint32_t key = MpqHashString(GetPlainFileName(szFileName), 3);
		if (blk.dwFlags & MPQ_FLAG_FIX_KEY)
		{
			key = (key + blk.dwFilePos) ^ blk.dwFSize;
		}
		pFile->dwFileKey = key;
	}

	*pphOut = pFile;
	return 1;
}

// Storm.#267 (1.12a: 0x6FC02200)
int __stdcall SFileOpenFile(char* szFileName, int phFileAddr)
{
	return SFileOpenFileEx(0, szFileName, static_cast<char>(g_dwDirectAccessFlags & 0xFFu), phFileAddr);
}

// Storm.#289 (1.12a: 0x6FC03610 — async/overlapped read, misnamed SFileFileExistsEx in 1.10f Storm.h)
int __stdcall SFileFileExistsEx(
	int hFileInt,
	int pBufferAddr,
	int nToRead,
	int pnBytesReadAddr,
	LONG lpOverlappedAddr,
	int nAsyncPriority,
	int pAsyncCallbackAddr)
{
	(void)nAsyncPriority;
	auto* pFile = reinterpret_cast<StormFileRecord*>(static_cast<uintptr_t>(static_cast<uint32_t>(hFileInt)));
	uint8_t* pBuf = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(pBufferAddr)));
	uint32_t* pcbRead = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(pnBytesReadAddr)));
	OVERLAPPED* pOv = reinterpret_cast<OVERLAPPED*>(static_cast<uintptr_t>(static_cast<uint32_t>(lpOverlappedAddr)));

	struct StormAsyncLoadCallback
	{
		DWORD(__stdcall* pfnCallback)(void* pFileData, size_t nDataLength, void* pCallbackData);
	};
	auto* pAsyncCb = reinterpret_cast<StormAsyncLoadCallback*>(static_cast<uintptr_t>(static_cast<uint32_t>(pAsyncCallbackAddr)));

	if (pcbRead)
	{
		*pcbRead = 0;
	}
	if (!pBuf || nToRead < 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (nToRead == 0)
	{
		if (pOv && pOv->hEvent)
		{
			SetEvent(pOv->hEvent);
		}
		return 1;
	}

	uint32_t dwActual = 0;
	bool bOk = false;
	{
		std::lock_guard<std::recursive_mutex> lock(g_MpqMutex);
		if (!pFile || pFile->dwMagic != 0x5346494Cu)
		{
			SetLastError(ERROR_INVALID_HANDLE);
			return 0;
		}
		const uint32_t dwStartOffset = pOv ? static_cast<uint32_t>(pOv->Offset) : pFile->dwFilePos;
		bOk = ReadFromStormFileLocked(pFile, dwStartOffset, pBuf, static_cast<uint32_t>(nToRead), &dwActual);
		if (!pOv)
		{
			pFile->dwFilePos += dwActual;
		}
	}

	if (pcbRead)
	{
		*pcbRead = dwActual;
	}
	if (pAsyncCb && pAsyncCb->pfnCallback)
	{
		pAsyncCb->pfnCallback(pBuf, dwActual, pAsyncCb);
	}
	if (pOv && pOv->hEvent)
	{
		SetEvent(pOv->hEvent);
	}
	return bOk ? 1 : 0;
}

// Storm.#287
int __stdcall SFileReadFileEx(int hFileInt, int pBufferAddr, int nToRead, int pnBytesReadAddr, LONG lpOverlappedAddr, int pAsyncCallbackAddr)
{
	return SFileFileExistsEx(hFileInt, pBufferAddr, nToRead, pnBytesReadAddr, lpOverlappedAddr, 0, pAsyncCallbackAddr);
}

// Storm.#269 (1.12a: 0x6FC03C40)
int __stdcall SFileReadFile(int hFileInt, int pBufferAddr, int nToRead, int pnBytesReadAddr, LONG lpOverlappedAddr)
{
	return SFileFileExistsEx(hFileInt, pBufferAddr, nToRead, pnBytesReadAddr, lpOverlappedAddr, 0, 0);
}

// Storm.#270
BOOL __stdcall SFileSetBasePath(char* path)
{
	if (!path)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	std::lock_guard<std::recursive_mutex> lock(g_MpqMutex);
	std::snprintf(g_szBasePath, sizeof(g_szBasePath), "%s", path);
	size_t nLen = std::strlen(g_szBasePath);
	while (nLen > 0 && (g_szBasePath[nLen - 1] == '\\' || g_szBasePath[nLen - 1] == '/'))
	{
		g_szBasePath[--nLen] = '\0';
	}
	return TRUE;
}

// Storm.#271
int __stdcall SFileSetFilePointer(int hFileInt, LONG lDistanceToMove, int lpDistanceToMoveHighAddr, DWORD dwMoveMethod)
{
	auto* pHigh = reinterpret_cast<LONG*>(static_cast<uintptr_t>(static_cast<uint32_t>(lpDistanceToMoveHighAddr)));
	auto* pFile = reinterpret_cast<StormFileRecord*>(static_cast<uintptr_t>(static_cast<uint32_t>(hFileInt)));
	if (!pFile || pFile->dwMagic != 0x5346494Cu)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return -1;
	}

	std::lock_guard<std::recursive_mutex> lock(g_MpqMutex);
	if (pFile->hDirectFile != INVALID_HANDLE_VALUE)
	{
		return static_cast<int>(SetFilePointer(pFile->hDirectFile, lDistanceToMove, pHigh, dwMoveMethod));
	}

	int64_t delta = lDistanceToMove;
	if (pHigh)
	{
		delta = (static_cast<int64_t>(*pHigh) << 32) | static_cast<uint32_t>(lDistanceToMove);
	}

	int64_t target = 0;
	switch (dwMoveMethod)
	{
	case FILE_BEGIN:
		target = delta;
		break;
	case FILE_CURRENT:
		target = static_cast<int64_t>(pFile->dwFilePos) + delta;
		break;
	case FILE_END:
		target = static_cast<int64_t>(pFile->block.dwFSize) + delta;
		break;
	default:
		SetLastError(ERROR_INVALID_PARAMETER);
		return -1;
	}

	if (target < 0)
	{
		SetLastError(ERROR_NEGATIVE_SEEK);
		return -1;
	}
	if (target > static_cast<int64_t>(pFile->block.dwFSize))
	{
		target = pFile->block.dwFSize;
	}
	pFile->dwFilePos = static_cast<uint32_t>(target);
	if (pHigh)
	{
		*pHigh = 0;
	}
	return static_cast<int>(pFile->dwFilePos);
}

// Storm.#272
LCID __stdcall SFileSetLocale(LCID lcLocale)
{
	const LCID prev = g_lcFileLocale;
	g_lcFileLocale = lcLocale;
	return prev;
}

// Storm.#273
BOOL __stdcall SFileGetBasePath(char* result, int maxSize)
{
	if (!result || maxSize <= 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	std::lock_guard<std::recursive_mutex> lock(g_MpqMutex);
	std::snprintf(result, static_cast<size_t>(maxSize), "%s", g_szBasePath);
	return TRUE;
}

// Storm.#274
BOOL __stdcall SFileSetIoErrorMode(int a1, int a2)
{
	(void)a1;
	(void)a2;
	return TRUE;
}

// Storm.#275
BOOL __stdcall SFileGetArchiveName(HANDLE hArchive, char* name, int length)
{
	auto* pArc = reinterpret_cast<StormArchiveRecord*>(hArchive);
	if (!pArc || pArc->dwMagic != 0x53415243u || !name || length <= 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	std::snprintf(name, static_cast<size_t>(length), "%s", pArc->szFileName);
	return TRUE;
}

// Storm.#276
BOOL __stdcall SFileGetFileName(HANDLE hFile, char* buffer, int length)
{
	auto* pFile = reinterpret_cast<StormFileRecord*>(hFile);
	if (!pFile || pFile->dwMagic != 0x5346494Cu || !buffer || length <= 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	std::snprintf(buffer, static_cast<size_t>(length), "%s", pFile->szFileName);
	return TRUE;
}

// Storm.#282
void __stdcall SFilePrioritizeRequest(int a1, int a2)
{
	(void)a1;
	(void)a2;
}

// Storm.#283
void __stdcall SFileCancelRequest(int a1)
{
	(void)a1;
}

// Storm.#284
int __stdcall SFileSetAsyncBudget(int a1)
{
	(void)a1;
	return 1;
}

// Storm.#285
void __stdcall SFileSetDataChunkSize(int a1)
{
	if (a1 > 0 && (a1 & (a1 - 1)) == 0)
	{
		g_StormOpt2 = static_cast<uint32_t>(a1);
	}
}

// Storm.#286
int __stdcall SFileEnableSeekOptimization(int a1)
{
	(void)a1;
	return 1;
}

// Storm.#288
DWORD __stdcall SFileFileExists(HANDLE hFile)
{
	const char* szName = reinterpret_cast<const char*>(hFile);
	if (!szName || !szName[0])
	{
		return 0;
	}
	HANDLE hOpened = nullptr;
	if (SFileOpenFile(const_cast<char*>(szName), reinterpret_cast<int>(&hOpened)) && hOpened)
	{
		SFileCloseFile(hOpened);
		return 1;
	}
	return 0;
}

// ============================================================================
// SFileDda* Streaming Audio APIs (#254..#261)
// ============================================================================

// Storm.#260
BOOL __stdcall SFileDdaInitialize(HANDLE directsound)
{
	std::lock_guard<std::recursive_mutex> lock(g_DdaMutex);
	g_pDdaDirectSound = reinterpret_cast<IDirectSound*>(directsound);
	return TRUE;
}

// Storm.#256
BOOL __cdecl SFileDdaDestroy()
{
	HANDLE hThreadToJoin = nullptr;
	{
		std::lock_guard<std::recursive_mutex> lock(g_DdaMutex);
		g_bDdaStopThread = true;
		if (g_hDdaWakeEvent)
		{
			SetEvent(g_hDdaWakeEvent);
		}
		hThreadToJoin = g_hDdaThread;
		g_hDdaThread = nullptr;
	}
	if (hThreadToJoin)
	{
		WaitForSingleObject(hThreadToJoin, 2000);
		CloseHandle(hThreadToJoin);
	}
	std::lock_guard<std::recursive_mutex> lock(g_DdaMutex);
	if (g_hDdaWakeEvent)
	{
		CloseHandle(g_hDdaWakeEvent);
		g_hDdaWakeEvent = nullptr;
	}
	while (!g_DdaStreams.empty())
	{
		StormDdaStream* pSt = g_DdaStreams.back();
		g_DdaStreams.pop_back();
		if (pSt->pDSBuffer)
		{
			pSt->pDSBuffer->Stop();
			if (pSt->bOwnsBuffer)
			{
				pSt->pDSBuffer->Release();
			}
		}
		delete pSt;
	}
	g_pDdaDirectSound = nullptr;
	return TRUE;
}

// Storm.#257
BOOL __stdcall SFileDdaEnd(HANDLE hFile)
{
	if (!hFile)
	{
		SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	std::lock_guard<std::recursive_mutex> lock(g_DdaMutex);
	for (auto it = g_DdaStreams.begin(); it != g_DdaStreams.end(); ++it)
	{
		StormDdaStream* pSt = *it;
		if (pSt->hFile == hFile)
		{
			if (pSt->pDSBuffer)
			{
				pSt->pDSBuffer->Stop();
				if (pSt->bOwnsBuffer)
				{
					pSt->pDSBuffer->Release();
				}
			}
			g_DdaStreams.erase(it);
			delete pSt;
			return TRUE;
		}
	}
	return FALSE;
}

// Storm.#255 (1.12a: 0x6FC046B0)
int __stdcall SFileDdaBeginEx(
	HANDLE hFile,
	signed int nBufferSize,
	unsigned int dwFlags,
	unsigned __int32 dwStartOffset,
	signed int nVolume,
	signed int nPan,
	int pDSBufferAddr)
{
	if (!hFile || nBufferSize <= 0 || !g_pDdaDirectSound)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	SFileDdaEnd(hFile);

	uint32_t riffHdr[3] = {};
	DWORD dwGot = 0;
	SFileSetFilePointer(reinterpret_cast<int>(hFile), 0, 0, FILE_BEGIN);
	if (!SFileReadFile(reinterpret_cast<int>(hFile), reinterpret_cast<int>(riffHdr), 12, reinterpret_cast<int>(&dwGot), 0) ||
		dwGot != 12 || riffHdr[0] != 0x46464952u || riffHdr[2] != 0x45564157u)
	{
		SetLastError(ERROR_BAD_FORMAT);
		return 0;
	}

	uint32_t dwFmtSize = 0;
	uint32_t dwFmtPos = 0;
	if (!FindWavChunk(hFile, 0x20746D66u, &dwFmtSize, &dwFmtPos) || dwFmtSize < 16)
	{
		SetLastError(ERROR_BAD_FORMAT);
		return 0;
	}

	WAVEFORMATEX wfx = {};
	SFileSetFilePointer(reinterpret_cast<int>(hFile), static_cast<LONG>(dwFmtPos), 0, FILE_BEGIN);
	if (!SFileReadFile(reinterpret_cast<int>(hFile), reinterpret_cast<int>(&wfx), 16, reinterpret_cast<int>(&dwGot), 0) || dwGot != 16)
	{
		SetLastError(ERROR_BAD_FORMAT);
		return 0;
	}
	wfx.cbSize = 0;

	uint32_t dwDataSize = 0;
	uint32_t dwDataPos = 0;
	if (!FindWavChunk(hFile, 0x61746164u, &dwDataSize, &dwDataPos) || dwDataSize == 0)
	{
		SetLastError(ERROR_BAD_FORMAT);
		return 0;
	}

	IDirectSoundBuffer* pDSB = reinterpret_cast<IDirectSoundBuffer*>(static_cast<uintptr_t>(static_cast<uint32_t>(pDSBufferAddr)));
	bool bOwnsBuffer = false;
	uint32_t dwActualBufSize = static_cast<uint32_t>(nBufferSize);
	if (!pDSB)
	{
		DSBUFFERDESC dsbd = {};
		dsbd.dwSize = sizeof(dsbd);
		dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_GETCURRENTPOSITION2;
		dsbd.dwBufferBytes = dwActualBufSize;
		dsbd.lpwfxFormat = &wfx;
		if (FAILED(g_pDdaDirectSound->CreateSoundBuffer(&dsbd, &pDSB, nullptr)) || !pDSB)
		{
			return 0;
		}
		bOwnsBuffer = true;
		if (nVolume != 0 && nVolume != 0x7FFFFFFF)
		{
			pDSB->SetVolume(nVolume);
		}
		if (nPan != 0 && nPan != 0x7FFFFFFF)
		{
			pDSB->SetPan(nPan);
		}
	}
	else
	{
		DSBCAPS caps = {};
		caps.dwSize = sizeof(caps);
		if (SUCCEEDED(pDSB->GetCaps(&caps)) && caps.dwBufferBytes > 0)
		{
			dwActualBufSize = caps.dwBufferBytes;
		}
	}

	auto* pStream = new StormDdaStream();
	pStream->hFile = hFile;
	pStream->pDSBuffer = pDSB;
	pStream->bOwnsBuffer = bOwnsBuffer;
	pStream->bLoop = (dwFlags & 0x40000u) != 0;
	pStream->nSilenceByte = (wfx.wBitsPerSample == 8) ? 0x80u : 0x00u;
	pStream->dwBufferSize = dwActualBufSize;
	pStream->dwDataOffset = dwDataPos;
	pStream->dwDataSize = dwDataSize;
	pStream->dwFileReadRel = (dwStartOffset != 0) ? (dwStartOffset % dwDataSize) : 0u;
	pStream->nVolume = nVolume;
	pStream->nPan = nPan;

	std::lock_guard<std::recursive_mutex> lock(g_DdaMutex);
	pDSB->Stop();
	pDSB->SetCurrentPosition(0);
	FillDdaBufferRegion(pStream, 0, dwActualBufSize);
	pStream->dwLastPlayCursor = 0;
	pDSB->Play(0, 0, DSBPLAY_LOOPING);

	g_DdaStreams.push_back(pStream);
	EnsureDdaThreadLocked();
	if (g_hDdaWakeEvent)
	{
		SetEvent(g_hDdaWakeEvent);
	}
	return 1;
}

// Storm.#254
int __stdcall SFileDdaBegin(HANDLE hFile, int nBufferSize, int dwFlags)
{
	return SFileDdaBeginEx(hFile, nBufferSize, static_cast<unsigned int>(dwFlags), 0, 0x7FFFFFFF, 0x7FFFFFFF, 0);
}

// Storm.#258 (1.12a: 0x6FC006A0)
BOOL __stdcall SFileDdaGetPos(int hFileInt, int pdwPosAddr, int pdwMaxPosAddr)
{
	HANDLE hFile = reinterpret_cast<HANDLE>(static_cast<uintptr_t>(static_cast<uint32_t>(hFileInt)));
	uint32_t* pdwPos = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(pdwPosAddr)));
	uint32_t* pdwMaxPos = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(pdwMaxPosAddr)));

	if (pdwPos) *pdwPos = 0;
	if (pdwMaxPos) *pdwMaxPos = 0;

	std::lock_guard<std::recursive_mutex> lock(g_DdaMutex);
	for (StormDdaStream* pSt : g_DdaStreams)
	{
		if (pSt->hFile == hFile)
		{
			PumpDdaStreamLocked(pSt);
			if (pdwMaxPos)
			{
				*pdwMaxPos = pSt->dwDataSize;
			}
			if (pSt->bFinished)
			{
				return FALSE;
			}
			if (pdwPos)
			{
				*pdwPos = (pSt->dwDataSize > 0)
					? static_cast<uint32_t>(pSt->uTotalPlayedBytes % pSt->dwDataSize)
					: 0u;
			}
			return TRUE;
		}
	}
	return FALSE;
}

// Storm.#259
BOOL __stdcall SFileDdaGetVolume(int hFileInt, int plVolumeAddr, int plPanAddr)
{
	HANDLE hFile = reinterpret_cast<HANDLE>(static_cast<uintptr_t>(static_cast<uint32_t>(hFileInt)));
	int32_t* plVol = reinterpret_cast<int32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(plVolumeAddr)));
	int32_t* plPan = reinterpret_cast<int32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(plPanAddr)));

	std::lock_guard<std::recursive_mutex> lock(g_DdaMutex);
	for (StormDdaStream* pSt : g_DdaStreams)
	{
		if (pSt->hFile == hFile)
		{
			if (plVol) *plVol = pSt->nVolume;
			if (plPan) *plPan = pSt->nPan;
			return TRUE;
		}
	}
	return FALSE;
}

// Storm.#261
BOOL __stdcall SFileDdaSetVolume(HANDLE hFile, signed int volume, signed int pan)
{
	std::lock_guard<std::recursive_mutex> lock(g_DdaMutex);
	for (StormDdaStream* pSt : g_DdaStreams)
	{
		if (pSt->hFile == hFile)
		{
			if (volume != 0x7FFFFFFF && pSt->pDSBuffer)
			{
				pSt->nVolume = volume;
				pSt->pDSBuffer->SetVolume(volume);
			}
			if (pan != 0x7FFFFFFF && pSt->pDSBuffer)
			{
				pSt->nPan = pan;
				pSt->pDSBuffer->SetPan(pan);
			}
			return TRUE;
		}
	}
	return FALSE;
}
