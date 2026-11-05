#pragma once

#include <cstdint>
#include <Windows.h>
#include <D2Dll.h>
#include <D2Gfx.h>

#pragma pack(push, 1)

struct D2SmackStrc
{
	uint32_t dwVersion;			//0x00
	uint32_t nWidth;			//0x04
	uint32_t nHeight;			//0x08
	uint32_t nFrames;			//0x0C
	uint8_t pad0x10[0x58];		//0x10
	uint32_t bNewPalette;		//0x68
	uint8_t aPalette[772];		//0x6C
	uint8_t pad0x370[4];		//0x370
	uint32_t nFrameNum;			//0x374
};

struct D2SmackerContextStrc
{
	D2SmackStrc* pSmack;		//0x00
	char cMode;					//0x04
	uint8_t pad0x05[3];			//0x05
	int32_t nXPos;				//0x08
	int32_t nYPos;				//0x0C
};

#pragma pack(pop)

//1.12a: D2Gdi.0x6F876B60 (#10000)
D2GDI_DLL_DECL D2GraphicsInterfaceStrc* __fastcall D2GDI_GetGraphicsInterface();
//1.12a: D2Gdi.0x6F875EB0 (#10001)
D2GDI_DLL_DECL void __fastcall D2GDI_GetPalette(HPALETTE* pPalette, RGBQUAD** ppColorTable);
//1.12a: D2Gdi.0x6F875E50 (#10002)
D2GDI_DLL_DECL void __fastcall D2GDI_GetSurfaceInfo(uint8_t** ppBits, int32_t* pHeight, int32_t* pWidth);
//1.12a: D2Gdi.0x6F875E40 (#10003)
D2GDI_DLL_DECL const D2GfxHelperStrc* __fastcall D2GDI_GetHelpers();
