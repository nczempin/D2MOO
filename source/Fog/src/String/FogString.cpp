#include <Fog.h>
#include <FogTables.h>
#include <cctype>
#include <cstdarg>
#include <cstdio>

// 1.10f: 0x6FF5DD90 (#10018)
// 1.12a: 0x6FF60390 (#10018)
const char* __cdecl FOG_csprintf(char* szDest, const char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);
	vsprintf(szDest, szFormat, args);
	va_end(args);
	return szDest;
}

// 1.10f: 0x6FF51DE0 (#10082)
// 1.12a: 0x6FF5CEC0 (#10082)
void __fastcall FOG_10082_Noop()
{
}

// 1.10f: 0x6FF51DF0 (#10083)
// 1.12a: 0x6FF5CEA0 (#10083)
float __stdcall FOG_10083_Cos_LUT(int16_t index)
{
	return gfFogSinLut[(index + 128) & 0x1FF];
}

// 1.10f: 0x6FF51E10 (#10084)
// 1.12a: 0x6FF5CE80 (#10084)
float __stdcall FOG_10084_Sin_LUT(int16_t index)
{
	return gfFogSinLut[index & 0x1FF];
}

// 1.10f: 0x6FF53DB0 (#10137)
// 1.12a: 0x6FF64740 (#10137)
uint16_t __stdcall FOG_ComputeStringCRC16(const char* szString)
{
	uint16_t nCrc = 0xFFFF;
	for (uint8_t ch = static_cast<uint8_t>(std::tolower(static_cast<uint8_t>(*szString)));
		 ch != 0;
		 ++szString, ch = static_cast<uint8_t>(std::tolower(static_cast<uint8_t>(*szString))))
	{
		const uint8_t nIdx = static_cast<uint8_t>((ch ^ nCrc) & 0xFFu);
		nCrc = static_cast<uint16_t>((nCrc >> 2) ^ gwFogCrc16Lut[nIdx]);
	}
	return nCrc;
}

// 1.10f: 0x6FF51B10 (#10182)
// 1.12a: 0x6FF618B0 (#10182)
int __stdcall FOG_10182_Return(void* pArg)
{
	D2_MAYBE_UNUSED(pArg);
	return 0;
}

// 1.10f: 0x6FF57620 (#10183)
// 1.12a: 0x6FF618A0 (#10183)
int __stdcall FOG_10183_Return(void* pArg1, int nArg2)
{
	D2_MAYBE_UNUSED(pArg1);
	D2_MAYBE_UNUSED(nArg2);
	return 0;
}

// 1.10f: 0x6FF53940 (#10229)
// 1.12a: 0x6FF6B890 (#10229)
uint32_t __stdcall FOG_ComputeChecksum(void* pData, size_t dwSize)
{
	const uint8_t* pBytes = static_cast<const uint8_t*>(pData);
	int32_t nChecksum = 0;
	for (int32_t i = 0; i < static_cast<int32_t>(dwSize); ++i)
	{
		const int32_t nCarry = (nChecksum < 0) ? 1 : 0;
		nChecksum = (nChecksum * 2) + pBytes[i] + nCarry;
	}
	return static_cast<uint32_t>(nChecksum);
}

// 1.10f: 0x6FF53970 (#10230)
// 1.12a: 0x6FF6B860 (#10230)
uint32_t __stdcall FOG_PopCount(void* pData, size_t dwSize)
{
	const uint8_t* pCur = static_cast<const uint8_t*>(pData);
	const uint8_t* pEnd = pCur + dwSize;
	uint32_t nCount = 0;
	while (pCur < pEnd)
	{
		nCount += gdwFogPopCountLut[*pCur++];
	}
	return nCount;
}

// 1.10f: 0x6FF539A0 (#10252)
// 1.12a: 0x6FF6B7F0 (#10252)
uint32_t __stdcall FOG_LeadingZeroesCount(uint32_t nValue)
{
	if (nValue == 0)
	{
		return 0xFFFFFFFFu;
	}
	if ((nValue & 0xFFu) != 0)
	{
		return gdwFogLeadingZeroesLut[nValue & 0xFFu];
	}
	if (((nValue >> 8) & 0xFFu) != 0)
	{
		return gdwFogLeadingZeroesLut[(nValue >> 8) & 0xFFu] + 8;
	}
	const uint32_t nHigh = nValue >> 16;
	if ((nHigh & 0xFFu) != 0)
	{
		return gdwFogLeadingZeroesLut[nHigh & 0xFFu] + 16;
	}
	if (((nHigh >> 8) & 0xFFu) != 0)
	{
		return gdwFogLeadingZeroesLut[(nHigh >> 8) & 0xFFu] + 24;
	}
	return 0xFFFFFFFFu;
}
