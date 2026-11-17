#pragma once

#include "D2WinControlHeader.h"


#pragma pack(push, 1)
struct D2WinSmackStrc
{
	D2WinControlStrc controlHeader;					//0x00
	char szFileName[256];							//0x40
	int32_t nSmackerContext;						//0x140
	int32_t nSmackerFlags;							//0x144
	int32_t nSmackX;								//0x148
	int32_t nSmackY;								//0x14C
};
#pragma pack(pop)


// 1.10f: D2Win.0x6F8AF710 (#10097) | 1.12a: D2Win.0x6F8F7460 (#10187)
D2WinSmackStrc* __fastcall SMACK_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t(__stdcall* a5)(SMSGHANDLER_PARAMS*));
// 1.10f: D2Win.0x6F8AF7E0 (#10099) | 1.12a: D2Win.0x6F8F7320 (#10013)
int __stdcall SMACK_Decode(D2WinSmackStrc* pSmack, const char* szFileName, int nVersion, int nUnused, int nUnused2);
// 1.10f: D2Win.0x6F8AF860 (#10101) | 1.12a: D2Win.0x6F8F72D0 (#10044)
int __stdcall SMACK_DeleteFile(D2WinSmackStrc* pSmack);
// 1.10f: D2Win.0x6F8AF8B0 (#10100) | 1.12a: D2Win.0x6F8F7280 (#10120)
int __stdcall SMACK_SetCoordinates(D2WinSmackStrc* pSmack, int nX, int nY);
// 1.10f: D2Win.0x6F8AF900 (#10098) | 1.12a: D2Win.0x6F8F7420 (#10055)
int __fastcall SMACK_Destroy(D2WinSmackStrc* pSmack);
// 1.10f: D2Win.0x6F8AF950 (#10184) | 1.12a: D2Win.0x6F8F7270 (#10071)
void __stdcall D2Win_10184();
// 1.10f: D2Win.0x6F8AF960 | 1.12a: D2Win.0x6F8F73A0
int32_t __fastcall SMACK_Draw(D2WinControlStrc* pControl);
// 1.10f: D2Win.0x6F8AFA40 | 1.12a: D2Win.0x6F8F7210
int __stdcall SMACK_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg);
// 1.10f: D2Win.0x6F8AFA90 (#10208)
void __fastcall D2Win_10208(int a1);
