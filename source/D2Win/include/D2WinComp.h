#pragma once

#include <Windows.h>
#include <cstdint>

#include <D2Gfx.h>
#include <D2Unicode.h>

#pragma pack(push, 1)
struct D2CofDataStrc
{
	uint8_t unk0x00;
	uint8_t unk0x01;
	uint8_t unk0x02;
	uint8_t unk0x03;
	uint32_t unk0x04;
	uint32_t unk0x08;
	uint32_t unk0x0C;
	uint32_t unk0x10;
	uint32_t unk0x14;
	uint32_t unk0x18;
};

struct D2CofNodeStrc
{
	uint32_t unk0x00;
	uint32_t nMode;
	uint32_t unk0x08;
	uint32_t unk0x0C;
	uint32_t unk0x10;
	D2CofDataStrc* pCofData;
	D2CofNodeStrc* unk0x18;
	D2CofNodeStrc* unk0x1C;
};

struct D2CofInfoStrc
{
	D2CofNodeStrc* pCofNode;
	D2CofInfoStrc* pNext;
};

struct D2GfxInfoStrc
{
	D2CofInfoStrc* pCofInfo;
};

struct D2CompositeUnitStrc
{
	uint32_t dwFlags;
	uint8_t unk0x04;
	uint8_t unk0x05;
	uint8_t unk0x06;
	uint8_t unk0x07;
	uint32_t unk0x08;
	uint32_t unk0x0C;
	uint32_t unk0x10;
	int32_t nX;
	int32_t nY;
	int32_t nUnitIndex;
	int32_t nMode;
	int32_t nWClassTokenIndex;
	uint8_t unk0x28[16];
	uint32_t dwArmorType[16];
	uint32_t nItemType[16];
	D2GfxInfoStrc* pGfxInfo;
	D2CellFileStrc* pEmblemCellFile;
	uint8_t unk0xC0;
	uint8_t unk0xC1;
	uint8_t unk0xC2;
	uint8_t unk0xC3;
	D2CompositeUnitStrc* pNext;
	D2CompositeUnitStrc* unk0xC8;
	Unicode unk0xCC[64];
	Unicode unk0x14C[64];
	Unicode unk0x1CC[16];
	Unicode unk0x1EC[32];
	int32_t unk0x22C;
	uint32_t unk0x230;
};
#pragma pack(pop)

static_assert(sizeof(D2CompositeUnitStrc) == 0x234, "D2CompositeUnitStrc size must be 0x234");


void __stdcall sub_6F8A4C10();
void __stdcall D2Win_10157();
D2CompositeUnitStrc* __stdcall D2Win_10144(unsigned int nUnitIndex, int nMode, BYTE* pComponents, BYTE* pComponentsColors);
void __stdcall D2Win_10145(D2CompositeUnitStrc* pCompositeUnit, unsigned int nUnitIndex, int nMode, BYTE* a4, BYTE* a5);
void __stdcall D2Win_10146();
D2CompositeUnitStrc* __stdcall D2Win_10147();
D2CompositeUnitStrc* __stdcall D2Win_10148(D2CompositeUnitStrc* pCompositeUnit);
void __stdcall D2Win_10149(D2CompositeUnitStrc* pCompositeUnit, uint8_t a2);
uint8_t __stdcall D2Win_10150(D2CompositeUnitStrc* pCompositeUnit);
void __stdcall D2Win_10151(D2CompositeUnitStrc* pCompositeUnit, int a2);
void __stdcall D2Win_10152(D2CompositeUnitStrc* pCompositeUnit, int nX);
void __stdcall D2Win_10153(D2CompositeUnitStrc* pCompositeUnit, int nY);
void __stdcall D2Win_10154(D2CompositeUnitStrc* pCompositeUnit, const Unicode* a2, const Unicode* a3, const Unicode* a4, const Unicode* a5, int a6, int a7);
int __stdcall D2Win_10155(D2CompositeUnitStrc* pCompositeUnit);
void __stdcall D2Win_10156(D2CompositeUnitStrc* pCompositeUnit, int nFlags);
void __stdcall D2Win_10158(D2CompositeUnitStrc* pCompositeUnit, uint8_t a2, uint8_t a3, uint8_t a4);
void __stdcall D2Win_10159(D2CellFileStrc* pSquelchCelFile, D2CellFileStrc* pSelectedCelFile);
const char* __stdcall D2Win_10161(int a1, int a2, int a3, BOOL bSoftCore);
int __stdcall D2Win_10169(D2CompositeUnitStrc* pCompositeUnit);
BOOL __fastcall D2Win_10143(D2CompositeUnitStrc* pCompositeUnit, int nMode);
void __fastcall sub_6F8A15E0(D2CompositeUnitStrc* pCompositeUnit);
BOOL __fastcall sub_6F8A1890(D2CompositeUnitStrc* pCompositeUnit, DWORD dwGamma);
BOOL __fastcall sub_6F8A1D10(D2CompositeUnitStrc* pCompositeUnit);
BOOL __fastcall sub_6F8A2070(D2CompositeUnitStrc* pCompositeUnit);
void __stdcall D2Win_10142_CompUnitDestroy(D2CompositeUnitStrc* pCompositeUnit, int a2);
const char* __stdcall D2Win_10160_GetPlayerTitle(int a1, int a2, BOOL bSoftCore);
