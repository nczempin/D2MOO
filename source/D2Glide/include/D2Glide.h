#pragma once

#include <cstdint>
#include <Windows.h>
#include <D2Dll.h>
#include <D2Gfx.h>

//1.12a: D2Glide (#10000)
D2GLIDE_DLL_DECL D2GraphicsInterfaceStrc* __fastcall D2GLIDE_GetGraphicsInterface();
//1.12a: D2Glide (#10001)
D2GLIDE_DLL_DECL void __fastcall D2GLIDE_GetPalette(HPALETTE* pPalette, RGBQUAD** ppColorTable);
//1.12a: D2Glide (#10002)
D2GLIDE_DLL_DECL void __fastcall D2GLIDE_GetSurfaceInfo(uint8_t** ppBits, int32_t* pHeight, int32_t* pWidth);
//1.12a: D2Glide (#10003)
D2GLIDE_DLL_DECL const D2GfxHelperStrc* __fastcall D2GLIDE_GetHelpers();
