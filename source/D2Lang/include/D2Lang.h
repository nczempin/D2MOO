#pragma once

#include <D2Dll.h>
#include <cstdint>

#pragma pack(1)

struct D2LANG_DLL_DECL Unicode;

struct D2TblHeaderStrc
{
	uint16_t nCRC;								//0x00
	uint16_t nNumElements;						//0x02
	int32_t nHashTableSize;						//0x04
	uint8_t nVersion;							//0x08
	uint32_t dwIndexStart;						//0x09
	uint32_t dwNumLoops;						//0x0D
	uint32_t dwFileSize;						//0x11
};

struct D2TblNodeStrc
{
	uint8_t bUsed;								//0x00
	uint16_t nIndexNo;							//0x01
	uint32_t dwHashValue;						//0x03
	uint32_t dwStringOffset;					//0x07
	uint32_t dwKeyOffset;						//0x0B
	uint16_t nNameLen;							//0x0F
};

#pragma pack()

//1.10f: D2Lang.0x6FC13BC0 (#10003)
//1.12a: D2Lang.0x6FC0A5D0 (#10010)
D2LANG_DLL_DECL const Unicode* __fastcall D2LANG_GetStringByReferenceString(char* string);

//1.10f: D2Lang.0x6FC13740 (#10004)
//1.12a: D2Lang.0x6FC09310 (#10005)
//1.14d: Game.0x524A30
D2LANG_DLL_DECL const Unicode* __fastcall D2LANG_GetStringFromTblIndex(short index);

//1.10f: D2Lang.0x6FC13960 (#10013)
//1.12a: D2Lang.0x6FC0A3D0 (#10007)
D2LANG_DLL_DECL short __stdcall D2LANG_GetTblIndex(char* szReference, const Unicode** pUnicode);

