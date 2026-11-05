#include <Fog.h>

// 1.10f: 0x6FF51B20 | 1.12a: 0x6FF659F0 (#10085)
unsigned __stdcall FOG_Decode14BitsFromString(uint16_t* p2Characters)
{
	const uint8_t* pBytes = reinterpret_cast<const uint8_t*>(p2Characters);
	return ((static_cast<unsigned>(pBytes[1]) & 0x7Fu) << 7) + (static_cast<unsigned>(pBytes[0]) & 0x7Fu);
}

// 1.10f: 0x6FF51B40 | 1.12a: 0x6FF659D0 (#10086)
void __stdcall FOG_Encode14BitsToString(uint16_t* p2Characters, uint32_t value)
{
	uint8_t* pBytes = reinterpret_cast<uint8_t*>(p2Characters);
	pBytes[0] = static_cast<uint8_t>(value | 0x80u);
	pBytes[1] = static_cast<uint8_t>((value >> 7) | 0x80u);
}

// 1.10f: 0x6FF51B60 | 1.12a: 0x6FF65990 (#10087)
unsigned __stdcall FOG_Decode32BitsFromString(uint32_t* p4Characters)
{
	const uint8_t* pBytes = reinterpret_cast<const uint8_t*>(p4Characters);
	uint32_t nAcc = static_cast<uint32_t>(pBytes[3]) * 0xFFu + static_cast<uint32_t>(pBytes[2]);
	nAcc = nAcc * 0xFFu + static_cast<uint32_t>(pBytes[1]);
	nAcc = nAcc * 0xFFu + static_cast<uint32_t>(pBytes[0]) - 0x00FE0200u;
	return nAcc;
}

// 1.10f: 0x6FF51BA0 | 1.12a: 0x6FF65930 (#10088)
void __stdcall FOG_Encode32BitsToString(uint32_t* p4Characters, uint32_t value)
{
	uint8_t* pBytes = reinterpret_cast<uint8_t*>(p4Characters);
	pBytes[0] = static_cast<uint8_t>((value % 0xFFu) + 1u);
	pBytes[1] = static_cast<uint8_t>(((value / 0xFFu) % 0xFFu) + 1u);
	pBytes[2] = static_cast<uint8_t>(((value / 0xFE01u) % 0xFFu) + 1u);
	pBytes[3] = static_cast<uint8_t>(((value / 0xFD02FFu) % 0xFFu) + 1u);
}
