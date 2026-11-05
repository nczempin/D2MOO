#include "CmnSubtile.h"

#include <algorithm>

#include <D2Math.h>
#include <Fog.h>
#include <Storm.h>

#include "DisplayType.h"
#include "D2Gfx.h"


extern int32_t gnViewMatrixAdjust_6FA8D254;
extern DisplayType gnDisplayType;


constexpr int32_t GOURAUD_SHIFT = 24;

PALETTEENTRY** dword_6FA81448;

PALETTEENTRY* gpPaletteTable_6FA81460[72];
D2PaletteStrc stru_6FA81580[49];

uint8_t* gCmnInfo_pBuffer_6FA8144C;
int32_t dword_6FA8D244;
int32_t dword_6FA84694;
int32_t dword_6FA84688;
int32_t dword_6FA8D23C;
int32_t dword_6FA8D24C;
int32_t dword_6FA8D240;
int32_t dword_6FA8468C;
int32_t dword_6FA846A0;
int32_t dword_6FA8D220;
int32_t dword_6FA8D224;
int32_t dword_6FA84698;
int32_t dword_6FA846A8[732];
int32_t dword_6FA8D228;
int32_t dword_6FA8D22C;
int32_t dword_6FA8D230;
int32_t dword_6FA85218;
int32_t dword_6FA8D238;
int32_t dword_6FA8D248;
int32_t dword_6FA8D234;
int32_t dword_6FA84690;
int32_t dword_6FA8469C;

int32_t dword_6FA8D250;

int32_t dword_6FA81450;
int32_t dword_6FA81454;
int32_t dword_6FA81458;

int32_t dword_6FA84680;
int32_t dword_6FA7E040 = -1;

uint8_t byte_6FA85220[32][32][32];


// TODO: Names
constexpr int32_t dword_6FA7E064[] =
{
    14, 12, 10, 8, 6, 4, 2, 0, 2, 4, 6, 8, 10, 12, 14
};

constexpr int32_t dword_6FA7E0A0[] =
{
    4, 8, 12, 16, 20, 24, 28, 32, 28, 24, 20, 16, 12, 8, 4
};

constexpr int32_t dword_6FA7E0DC[] =
{
    0, 4, 12, 24, 40, 60, 84, 112, 144, 172, 196, 216, 232, 244, 252
};



//D2Gfx.0x6FA71000
void __fastcall D2GFX_InitGouraudCache_6FA71000()
{
    DGFX_InitGouraudCache_6FA72570();
    gCmnInfo_pBuffer_6FA8144C = nullptr;
}

//D2Gfx.0x6FA71010
void __fastcall D2GFX_FillYBufferTable_6FA71010(uint8_t* a1, int32_t a2, int32_t a3, int32_t a4)
{
    gCmnInfo_pBuffer_6FA8144C = a1;
    dword_6FA81450 = a2;
    dword_6FA81454 = a3;
    dword_6FA81458 = a4;

    if (a2 != dword_6FA84680)
    {
        dword_6FA81450 = a2;
        dword_6FA84680 = a2;
        sub_6FA72700();
    }

    if (gnViewMatrixAdjust_6FA8D254 != dword_6FA7E040)
    {
        sub_6FA72630(a2, a3);
    }
}

//D2Gfx.0x6FA71070
void __fastcall sub_6FA71070(D2PaletteTableStrc* pPaletteTable)
{
    memcpy(gpPaletteTable_6FA81460, pPaletteTable, sizeof(gpPaletteTable_6FA81460));
    dword_6FA81448 = gpPaletteTable_6FA81460;
    for (int32_t i = 0; i < std::size(stru_6FA81580); ++i)
    {
        memcpy(&stru_6FA81580[i], gpPaletteTable_6FA81460[i + 3], sizeof(D2PaletteStrc));
    }
}

//D2Gfx.0x6FA710C0
int32_t __fastcall D2GFX_SetContrastAndGamma_6FA710C0()
{
    DWORD nContrast = gnDisplayType != DISPLAYTYPE_GLIDE ? 100 : 0;
    SRegLoadValue("Diablo II", "Contrast", 0, &nContrast);
    D2GFX_SetOption(11, nContrast);

    DWORD nGamma = 155;
    SRegLoadValue("Diablo II", "Gamma", 0, &nGamma);
    gGfxSettings.nGamma = nGamma;
    return D2GFX_SetGamma(nGamma);
}

//D2Gfx.0x6FA71130
void __fastcall sub_6FA71130(uint8_t* a1, uint8_t* a2, uint8_t* a3)
{
    // Unrolled in original dll
    for (int i = 0; i < 15; ++i)
    {
        uint8_t* v5 = &a1[dword_6FA7E0DC[i]];
        uint8_t* v6 = &a2[dword_6FA7E064[i]];
        const int nValues = dword_6FA7E0A0[i];
        for (int j = nValues - 1; j >= 0; --j)
        {
            v6[j] = a3[v5[j]];
        }
        a2 += dword_6FA81450;
    }
}

//D2Gfx.0x6FA71340
void __fastcall sub_6FA71340(uint8_t* a1, uint8_t* a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    uint8_t* v6 = a2;

    for (int32_t i = 0; i < std::size(dword_6FA7E064); ++i)
    {
        const int32_t v8 = dword_6FA7E064[i];
        uint8_t* v9 = (uint8_t*)((char*)byte_6FA85220 + 32 * ((a5 >> 7) + 32 * (a3 >> 7)) + v8);
        uint8_t* v10 = &a1[dword_6FA7E0DC[i]];

        // Unrolled in orginal dll
        const int nValues = dword_6FA7E0A0[i];
        for (int j = nValues-1; j >= 0; j--)
        {
            v6[v8 + j] = stru_6FA81580[v9[j]].unk0x00[v10[j]];
        }

        v6 += dword_6FA81450;
        a3 += a4;
        a5 += a6;
    }
}

//D2Gfx.0x6FA71720
void __fastcall sub_6FA71720(uint8_t* a1, uint8_t* a2, uint8_t* a3, int32_t a4)
{
    // TODO: Names
    uint8_t* v4 = a2;
    while (a4)
    {
        uint8_t v7 = a1[0];
        uint8_t v8 = a1[1];

        a1 += 2;

        if (v7 || v8)
        {
            uint8_t* v9 = &v4[v7];
            // Unrolled in original dll
            for (int j = v8 - 1; j >= 0; j--)
            {
                v9[j] = a3[a1[j]];
            }

            v4 = &v9[v8];
            a1 += v8;
        }
        else
        {
            a2 += dword_6FA81450;
            --a4;
            v4 = a2;
        }
    }
}

//D2Gfx.0x6FA71970
void __fastcall sub_6FA71970(uint8_t* a1, uint8_t* a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, uint8_t a8)
{
    // TODO: Names
    uint8_t* v8 = a1;
    uint8_t* v9 = a2;
    uint8_t* v10 = (uint8_t*)((char*)byte_6FA85220 + 32 * ((a5 >> a8) + 32 * (a3 >> a8)));
    
    while (a7)
    {
        const uint8_t v11 = v8[0];
        const uint8_t v12 = v8[1];
        v8 += 2;
        
        if (v11 || v12)
        {
            uint8_t* v13 = &v9[v11];
            uint8_t* v14 = &v10[v11];
            // Unrolled in original dll
            for (int i = v12 - 1; i >= 0; i--)
            {
                v13[i] = stru_6FA81580[v14[i]].unk0x00[v8[i]];
            }

            v9 = &v13[v12];
            v8 += v12;
            v10 = &v14[v12];
        }
        else
        {
            --a7;
            a3 += a4;
            a5 += a6;
            a2 += dword_6FA81450;
            v9 = a2;
            v10 = (BYTE*)((char*)byte_6FA85220 + 32 * ((a5 >> a8) + 32 * (a3 >> a8)));
        }
    }
}

//D2Gfx.0x6FA71D90) --------------------------------------------------------
void __fastcall sub_6FA71D90(int32_t a1, int32_t a2, uint8_t* a3, uint8_t* a4, uint8_t* a5, int32_t a6)
{
    BYTE* v6; // ebp@1
    BYTE* v7; // eax@1
    int v8; // ecx@2
    BYTE* v9; // edx@2
    int v10; // ebx@3
    BYTE* v11; // edx@3
    int v12; // edi@3
    int v13; // ebx@3
    BYTE* v14; // eax@7
    int v15; // edi@8
    int v16; // ebp@8
    int v17; // edi@10
    int v18; // ecx@10
    int v19; // ecx@10
    int v20; // ebx@12
    BYTE* v21; // eax@13
    BYTE* v22; // edx@13
    int v23; // [sp+10h] [bp-10h]@1
    int v24; // [sp+14h] [bp-Ch]@2
    int v25; // [sp+18h] [bp-8h]@1
    BYTE* v26; // [sp+1Ch] [bp-4h]@8
    BYTE* a3a; // [sp+24h] [bp+4h]@8

    v25 = a1;
    v6 = a4;
    v7 = a4;
    v23 = 0;
    v8 = a2;
    v9 = a3;
    v24 = v8;
    while (v23 < a6)
    {
        v10 = *v9;
        v11 = v9 + 1;
        v12 = v10;
        v13 = *v11;
        v9 = v11 + 1;

        if (v12 || v13)
        {
            if (v8 >= dword_6FA8D240)
                return;

            v14 = &v7[v12];
            if (v8 < dword_6FA84690)
            {
                v7 = &v14[v13];
                v9 += v13;
            }
            else
            {
                v15 = v25 + v14 - v6;
                a3a = &v9[v13];
                v16 = dword_6FA8D220 - v15;
                v26 = &v14[v13];

                if (dword_6FA8D220 - v15 <= 0)
                    v16 = 0;

                v17 = dword_6FA8469C - v15;
                v19 = v17 & ((v17 <= 0) - 1);

                if (v19 < v13)
                    v13 = v19;

                v20 = v13 - v16;
                if (v20 > 0)
                {
                    v21 = &v14[v16];
                    v22 = &v9[v16];
                    // Unrolled in original dll
                    for (int i = v20 - 1; i >= 0; i--)
                    {
                        v21[i] = a5[v22[i]];
                    }
                }
                v9 = a3a;
                v7 = v26;
                v8 = v24;
                v6 = a4;
            }
        }
        else
        {
            ++v8;
            v6 += dword_6FA81450;
            ++v23;
            v24 = v8;
            a4 = v6;
            v7 = v6;
        }
    }
}

//D2Gfx.0x6FA72090) --------------------------------------------------------
void __fastcall sub_6FA72090(int32_t a1, int32_t a2, uint8_t* a3, uint8_t* a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8, int32_t a9, uint8_t a10)
{
    char v10; // cl@1
    BYTE* v11; // ebp@1
    BYTE* v12; // esi@1
    BYTE* v13; // eax@1
    BYTE* v14; // edx@2
    int v15; // ebx@3
    BYTE* v16; // edx@3
    int v17; // ebx@3
    BYTE* v18; // esi@7
    BYTE* v19; // eax@7
    int v20; // edi@8
    int v21; // ebp@8
    int v22; // ecx@10
    int v23; // ecx@12
    BYTE* v24; // esi@13
    BYTE* v25; // edx@13
    int v26; // [sp+10h] [bp-14h]@2
    int v27; // [sp+14h] [bp-10h]@1
    int v28; // [sp+18h] [bp-Ch]@1
    BYTE* v29; // [sp+20h] [bp-4h]@8
    int a3a; // [sp+28h] [bp+4h]@3
    BYTE* a3b; // [sp+28h] [bp+4h]@8

    v28 = a1;
    v10 = a10;
    v11 = a4;
    v27 = 0;
    v12 = a4;
    v13 = (BYTE*)((char*)byte_6FA85220 + 32 * ((a7 >> a10) + 32 * (a5 >> a10)));
    v26 = a2;
    v14 = a3;
    while (v27 < a9)
    {
        v15 = *v14;
        v16 = v14 + 1;
        a3a = v15;
        v17 = *v16;
        v14 = v16 + 1;
        if (a3a || v17)
        {
            if (v26 >= dword_6FA8D240)
                return;
            v18 = &v12[a3a];
            v19 = &v13[a3a];
            if (v26 < dword_6FA84690)
            {
                v12 = &v18[v17];
                v13 = &v19[v17];
                v14 += v17;
            }
            else
            {
                v29 = &v18[v17];
                v20 = v28 + v18 - v11;
                v21 = dword_6FA8D220 - v20;
                a3b = &v14[v17];
                if (dword_6FA8D220 - v20 <= 0)
                    v21 = 0;
                v22 = ((dword_6FA8469C - v20 <= 0) - 1) & (dword_6FA8469C - v20);
                if (v22 >= v17)
                    v22 = v17;
                v23 = v22 - v21;
                if (v23 > 0)
                {
                    v24 = &v18[v21];
                    v25 = &v14[v21];
                    for (int i = v23 - 1; i >= 0; i--)
                    {
                        v24[i] = stru_6FA81580[v19[i]].unk0x00[v25[i]];
                    }
                }

                v14 = a3b;
                v12 = v29;
                v10 = a10;
                v11 = a4;
                v13 = &v19[v17];
            }
        }
        else
        {
            ++v26;
            v11 += dword_6FA81450;
            a5 += a6;
            a7 += a8;
            ++v27;
            a4 = v11;
            v12 = v11;
            v13 = (BYTE*)((char*)byte_6FA85220 + 32 * ((a7 >> v10) + 32 * (a5 >> v10)));
        }
    }
}

//D2Gfx.0x6FA72570
void __fastcall DGFX_InitGouraudCache_6FA72570()
{
    // TODO: Check, Names
    int32_t v1 = 0;
    int32_t v10 = 0;
    for (int32_t i = 0; i < 32; ++i)
    {
        int32_t nVal = i << GOURAUD_SHIFT;
        const int32_t nOriginalValue = nVal;
        for (int32_t j = 0; j < 32; ++j)
        {
            for (int32_t k = 0; k < 32; ++k)
            {
                nVal = D2Clamp(nVal, 0, 0x1F000000);
                byte_6FA85220[i][j][k] = (nVal >> GOURAUD_SHIFT) & 0XFF;
                nVal += v1 >> 5;
            }

            D2_ASSERT((nVal >> GOURAUD_SHIFT) == j);

            v1 += (1 << GOURAUD_SHIFT);
            nVal = nOriginalValue;
        }

        v10 -= (1 << GOURAUD_SHIFT);
        v1 = v10;
    }
}

//D2Gfx.0x6FA72630
void __fastcall sub_6FA72630(int32_t a1, int32_t a2)
{
    //TODO: Names
    int32_t v2 = 0;
    if (gnViewMatrixAdjust_6FA8D254 == 1)
    {
        a1 -= a1 / 2;
    }
    else if (gnViewMatrixAdjust_6FA8D254 == 2)
    {
        v2 = a1 / 2;
    }

    dword_6FA8D244 = v2 - 32;
    dword_6FA84694 = a2 - 63;
    dword_6FA84688 = v2 - 32;
    dword_6FA8D23C = a2 - 47;
    dword_6FA8D24C = a2 - 47;
    dword_6FA8D240 = a2 - 47;
    dword_6FA8468C = v2;
    dword_6FA846A0 = v2;
    dword_6FA8D220 = v2;
    dword_6FA8D224 = -16;
    dword_6FA84698 = a1;
    dword_6FA8D228 = 0;
    dword_6FA8D22C = a1 - 32;
    dword_6FA8D230 = -32;
    dword_6FA85218 = a1;
    dword_6FA8D238 = 0;
    dword_6FA8D248 = a2 - 79;
    dword_6FA8D234 = a1 - 32;
    dword_6FA84690 = 0;
    dword_6FA8469C = a1;
}

//D2Gfx.0x6FA72700
void __fastcall sub_6FA72700()
{
    int32_t v2 = -32 * dword_6FA81450;
    for (int32_t i = 0; i < std::size(dword_6FA846A8); ++i)
    {
        dword_6FA846A8[i] = v2;
        v2 += dword_6FA81450;
    }
}

//D2Gfx.0x6FA72730) --------------------------------------------------------
void __fastcall D2GFX_DrawVisTile_6FA72730(uint8_t* a1, int32_t nX, int32_t nY)
{
    if (nY >= dword_6FA8D230 && nY < dword_6FA8D24C && nX >= dword_6FA84688 && nX < dword_6FA85218)
    {
        D2_ASSERT(gCmnInfo_pBuffer_6FA8144C);

        uint8_t* pLine = &gCmnInfo_pBuffer_6FA8144C[dword_6FA846A8[nY + 32]] + nX;
        uint8_t* pDst = pLine;
        const uint8_t* pSrc = a1;

        if (nY < dword_6FA8D238 || nY >= dword_6FA8D248 || nX < dword_6FA846A0 || nX >= dword_6FA8D234)
        {
            int32_t nCurY = nY;
            for (int32_t nRow = 0; nRow < 32; )
            {
                const int32_t nSkip = pSrc[0];
                const int32_t nCount = pSrc[1];
                pSrc += 2;

                if (nSkip || nCount)
                {
                    if (nCurY >= dword_6FA8D240)
                    {
                        return;
                    }
                    pDst += nSkip;
                    if (nCurY >= dword_6FA84690)
                    {
                        const int32_t nCurX = nX + (int32_t)(pDst - pLine);
                        int32_t nStart = dword_6FA8D220 - nCurX;
                        if (nStart <= 0)
                        {
                            nStart = 0;
                        }
                        int32_t nEnd = dword_6FA8469C - nCurX;
                        if (nEnd <= 0)
                        {
                            nEnd = 0;
                        }
                        if (nEnd > nCount)
                        {
                            nEnd = nCount;
                        }
                        const int32_t nCopyLen = nEnd - nStart;
                        if (nCopyLen > 0)
                        {
                            memcpy(pDst + nStart, pSrc + nStart, nCopyLen);
                        }
                    }
                    pDst += nCount;
                    pSrc += nCount;
                }
                else
                {
                    ++nCurY;
                    pLine += dword_6FA81450;
                    pDst = pLine;
                    ++nRow;
                }
            }
        }
        else
        {
            for (int32_t nRow = 32; nRow > 0; )
            {
                const int32_t nSkip = pSrc[0];
                const int32_t nCount = pSrc[1];
                pSrc += 2;

                if (nSkip || nCount)
                {
                    pDst += nSkip;
                    memcpy(pDst, pSrc, nCount);
                    pDst += nCount;
                    pSrc += nCount;
                }
                else
                {
                    pLine += dword_6FA81450;
                    pDst = pLine;
                    --nRow;
                }
            }
        }
    }
}

//D2Gfx.0x6FA72900
void __fastcall sub_6FA72900(uint8_t* a1, int32_t nX, int32_t nY, uint8_t a4)
{
    if (nY < dword_6FA8D230 || nY >= dword_6FA8D24C || nX < dword_6FA84688 || nX >= dword_6FA85218)
    {
        return;
    }

    uint8_t* pBuffer = &gCmnInfo_pBuffer_6FA8144C[dword_6FA846A8[nY + 32]] + nX;

    D2_ASSERT(gCmnInfo_pBuffer_6FA8144C);

    if (nY < dword_6FA8D238 || nY >= dword_6FA8D248 || nX < dword_6FA846A0 || nX >= dword_6FA8D234)
    {
        sub_6FA71D90(nX, nY, a1, pBuffer, stru_6FA81580[(uint32_t)a4 >> 3].unk0x00, 32);
    }
    else
    {
        sub_6FA71720(a1, pBuffer, stru_6FA81580[(uint32_t)a4 >> 3].unk0x00, 32);
    }
}

//D2Gfx.0x6FA729D0
void __fastcall TILE_TileDrawLit_6FA729D0(uint8_t* a1, int32_t nX, int32_t nY, int32_t* pIntensities)
{
    if (dword_6FA8D250)
    {
        if ((*(uint8_t*)pIntensities & 0xF8) == 0xF8)
        {
            D2GFX_DrawVisTile_6FA72730(a1, nX, nY);
        }
        else
        {
            sub_6FA72900(a1, nX, nY, *(uint8_t*)pIntensities);
        }
    }
    else
    {
        // TODO: Names
        const int32_t v6 = 32 * pIntensities[0];
        const int32_t v7 = pIntensities[3] - pIntensities[0];
        const int32_t v8 = pIntensities[2] - pIntensities[1];
        const int32_t v9 = 32 * pIntensities[1];

        if (gGfxSettings.bLowQuality || std::abs(pIntensities[1] - pIntensities[0]) + std::abs(v7) + std::abs(v8) < 10)
        {
            if ((v6 & 0xFFFFFF00) == 0x1F00)
            {
                D2GFX_DrawVisTile_6FA72730(a1, nX, nY);
            }
            else
            {
                sub_6FA72900(a1, nX, nY, *(uint8_t*)pIntensities);
            }
        }
        else
        {
            if (nY >= dword_6FA8D230 && nY < dword_6FA8D24C && nX >= dword_6FA84688 && nX < dword_6FA85218)
            {
                D2_ASSERT(gCmnInfo_pBuffer_6FA8144C);

                uint8_t* pBuffer = &gCmnInfo_pBuffer_6FA8144C[dword_6FA846A8[nY + 32]] + nX;
                if (nY < dword_6FA8D238 || nY >= dword_6FA8D248 || nX < dword_6FA846A0 || nX >= dword_6FA8D234)
                {
                    sub_6FA72090(nX, nY, a1, pBuffer, v6, v7, v9, v8, 32, 8);
                }
                else
                {
                    sub_6FA71970(a1, pBuffer, v6, v7, v9, v8, 32, 8);
                }
            }
        }
    }
}

//D2Gfx.0x6FA72B80) --------------------------------------------------------
void __fastcall D2GFX_DrawBlendedVisTile_6FA72B80(uint8_t* a1, int32_t nX, int32_t nY, uint8_t nAlpha)
{
    const uint8_t* pTransPalette = nullptr;
    if (nAlpha < 0xC0u)
    {
        if (nAlpha < 0x80u)
        {
            if (nAlpha < 0x40u)
            {
                return;
            }
            pTransPalette = (const uint8_t*)dword_6FA81448[2];
        }
        else
        {
            pTransPalette = (const uint8_t*)dword_6FA81448[1];
        }
    }
    else
    {
        pTransPalette = (const uint8_t*)dword_6FA81448[0];
    }

    if (nY >= dword_6FA8D230 && nY < dword_6FA8D24C && nX >= dword_6FA84688 && nX < dword_6FA85218)
    {
        D2_ASSERT(gCmnInfo_pBuffer_6FA8144C);

        uint8_t* pLine = &gCmnInfo_pBuffer_6FA8144C[dword_6FA846A8[nY + 32]] + nX;
        uint8_t* pDst = pLine;
        const uint8_t* pSrc = a1;

        if (nY < dword_6FA8D238 || nY >= dword_6FA8D248 || nX < dword_6FA846A0 || nX >= dword_6FA8D234)
        {
            int32_t nCurY = nY;
            for (int32_t nRow = 0; nRow < 32; )
            {
                const int32_t nSkip = pSrc[0];
                const int32_t nCount = pSrc[1];
                pSrc += 2;

                if (nSkip || nCount)
                {
                    if (nCurY >= dword_6FA8D240)
                    {
                        return;
                    }
                    pDst += nSkip;
                    if (nCurY >= dword_6FA84690)
                    {
                        const int32_t nCurX = nX + (int32_t)(pDst - pLine);
                        int32_t nStart = dword_6FA8D220 - nCurX;
                        if (nStart <= 0)
                        {
                            nStart = 0;
                        }
                        int32_t nEnd = dword_6FA8469C - nCurX;
                        if (nEnd <= 0)
                        {
                            nEnd = 0;
                        }
                        if (nEnd > nCount)
                        {
                            nEnd = nCount;
                        }
                        const int32_t nCopyLen = nEnd - nStart;
                        for (int32_t i = 0; i < nCopyLen; ++i)
                        {
                            const uint8_t srcVal = pSrc[nStart + i];
                            const uint8_t dstVal = pDst[nStart + i];
                            pDst[nStart + i] = pTransPalette[((uint32_t)dstVal << 8) + srcVal];
                        }
                    }
                    pDst += nCount;
                    pSrc += nCount;
                }
                else
                {
                    ++nCurY;
                    pLine += dword_6FA81450;
                    pDst = pLine;
                    ++nRow;
                }
            }
        }
        else
        {
            for (int32_t nRow = 32; nRow > 0; )
            {
                const int32_t nSkip = pSrc[0];
                const int32_t nCount = pSrc[1];
                pSrc += 2;

                if (nSkip || nCount)
                {
                    pDst += nSkip;
                    for (int32_t i = 0; i < nCount; ++i)
                    {
                        const uint8_t srcVal = pSrc[i];
                        const uint8_t dstVal = pDst[i];
                        pDst[i] = pTransPalette[((uint32_t)dstVal << 8) + srcVal];
                    }
                    pDst += nCount;
                    pSrc += nCount;
                }
                else
                {
                    pLine += dword_6FA81450;
                    pDst = pLine;
                    --nRow;
                }
            }
        }
    }
}

//D2Gfx.0x6FA72DC0) --------------------------------------------------------
void __fastcall D2GFX_DrawRoofTile_6FA72DC0(uint8_t* a1, int32_t nX, int32_t nY, int32_t* pIntensities, uint8_t nAlpha)
{
    const uint8_t* pTransPalette = nullptr;
    if (nAlpha < 0xC0u)
    {
        if (nAlpha < 0x80u)
        {
            if (nAlpha < 0x40u)
            {
                return;
            }
            pTransPalette = (const uint8_t*)dword_6FA81448[2];
        }
        else
        {
            pTransPalette = (const uint8_t*)dword_6FA81448[1];
        }
    }
    else
    {
        pTransPalette = (const uint8_t*)dword_6FA81448[0];
    }

    const int32_t nDelta0 = pIntensities[3] - pIntensities[0];
    const int32_t nDelta1 = pIntensities[2] - pIntensities[1];
    int32_t nInt0 = 32 * pIntensities[0];
    int32_t nInt1 = 32 * pIntensities[1];

    if (nY >= dword_6FA8D230 && nY < dword_6FA8D24C && nX >= dword_6FA84688 && nX < dword_6FA85218)
    {
        D2_ASSERT(gCmnInfo_pBuffer_6FA8144C);

        uint8_t* pLine = &gCmnInfo_pBuffer_6FA8144C[dword_6FA846A8[nY + 32]] + nX;
        uint8_t* pDst = pLine;
        const uint8_t* pSrc = a1;
        const uint8_t* pGouraud = &byte_6FA85220[nInt0 >> 8][nInt1 >> 8][0];

        if (nY >= dword_6FA8D238 && nY < dword_6FA8D248 && nX >= dword_6FA846A0 && nX < dword_6FA8D234)
        {
            for (int32_t nRow = 32; nRow > 0; )
            {
                const int32_t nSkip = pSrc[0];
                const int32_t nCount = pSrc[1];
                pSrc += 2;

                if (nSkip || nCount)
                {
                    pDst += nSkip;
                    pGouraud += nSkip;
                    for (int32_t i = 0; i < nCount; ++i)
                    {
                        const uint8_t litVal = stru_6FA81580[pGouraud[i]].unk0x00[pSrc[i]];
                        const uint8_t dstVal = pDst[i];
                        pDst[i] = pTransPalette[((uint32_t)litVal << 8) + dstVal];
                    }
                    pDst += nCount;
                    pGouraud += nCount;
                    pSrc += nCount;
                }
                else
                {
                    --nRow;
                    nInt0 += nDelta0;
                    nInt1 += nDelta1;
                    pLine += dword_6FA81450;
                    pDst = pLine;
                    pGouraud = &byte_6FA85220[nInt0 >> 8][nInt1 >> 8][0];
                }
            }
        }
        else
        {
            int32_t nCurY = nY;
            for (int32_t nRow = 0; nRow < 32; )
            {
                const int32_t nSkip = pSrc[0];
                const int32_t nCount = pSrc[1];
                pSrc += 2;

                if (nSkip || nCount)
                {
                    if (nCurY >= dword_6FA8D240)
                    {
                        return;
                    }
                    pDst += nSkip;
                    pGouraud += nSkip;
                    if (nCurY >= dword_6FA84690)
                    {
                        const int32_t nCurX = nX + (int32_t)(pDst - pLine);
                        int32_t nStart = dword_6FA8D220 - nCurX;
                        if (nStart <= 0)
                        {
                            nStart = 0;
                        }
                        int32_t nEnd = dword_6FA8469C - nCurX;
                        if (nEnd <= 0)
                        {
                            nEnd = 0;
                        }
                        if (nEnd > nCount)
                        {
                            nEnd = nCount;
                        }
                        const int32_t nCopyLen = nEnd - nStart;
                        for (int32_t i = 0; i < nCopyLen; ++i)
                        {
                            const uint8_t litVal = stru_6FA81580[pGouraud[nStart + i]].unk0x00[pSrc[nStart + i]];
                            const uint8_t dstVal = pDst[nStart + i];
                            pDst[nStart + i] = pTransPalette[((uint32_t)litVal << 8) + dstVal];
                        }
                    }
                    pDst += nCount;
                    pGouraud += nCount;
                    pSrc += nCount;
                }
                else
                {
                    nInt0 += nDelta0;
                    nInt1 += nDelta1;
                    ++nRow;
                    ++nCurY;
                    pLine += dword_6FA81450;
                    pDst = pLine;
                    pGouraud = &byte_6FA85220[nInt0 >> 8][nInt1 >> 8][0];
                }
            }
        }
    }
}

//D2Gfx.0x6FA73130
void __fastcall sub_6FA73130(D2TileLibraryBlockStrc* a1, int32_t a2, int32_t a3, uint8_t* a4, uint8_t a5)
{
    D2PaletteStrc* pPalette = &stru_6FA81580[(uint32_t)a5 >> 3];

    if (a1->nFormat & 4)
    {
        sub_6FA71D90(a2, a3, a1->dwOffset_pData, a4, pPalette->unk0x00, 15);
    }
    else
    {
        const int32_t nEnd = std::min(dword_6FA8D240 - a3, 15);
        for (int32_t nStart = std::max(dword_6FA84690 - a3, 0); nStart < nEnd; ++nStart)
        {
            const int32_t nOffset = a2 + dword_6FA7E064[nStart];
            const int32_t nIndex = std::max(dword_6FA8D220 - nOffset, 0);
            const int32_t nSize = D2Clamp(dword_6FA8469C - nOffset, 0, dword_6FA7E0A0[nStart]) - nIndex;
            if (nSize > 0)
            {
                uint8_t* pCurrent = &a4[nStart * dword_6FA81450 + dword_6FA7E064[nStart] + nIndex];
                uint8_t* pStart = &a1->dwOffset_pData[dword_6FA7E0DC[nStart] + nIndex];
                uint8_t* pEnd = &pStart[nSize];
                while (pStart < pEnd)
                {
                    *pCurrent = pPalette->unk0x00[*pStart];
                    ++pCurrent;
                    ++pStart;
                }
            }
        }
    }
}

//D2Gfx.0x6FA73270
void __fastcall sub_6FA73270(D2TileLibraryBlockStrc* a1, int32_t a2, int32_t a3, uint8_t* a4, int32_t* a5)
{
    // TODO: Names
    const int32_t v7 = a5[3] - a5[0];
    const int32_t v9 = a5[2] - a5[1];
    int32_t v11 = 16 * a5[0];
    int32_t v12 = 16 * a5[1];

    if (a1->nFormat & 4)
    {
        sub_6FA72090(a2, a3, a1->dwOffset_pData, a4, v11, v7, v12, v9, 15, 7);
    }
    else
    {
        const int32_t nEnd = std::min(dword_6FA8D240 - a3, 15);
        for (int32_t nStart = std::max(dword_6FA84690 - a3, 0); nStart < nEnd; ++nStart)
        {
            const int32_t nOffset = a2 + dword_6FA7E064[nStart];
            const int32_t nIndex = std::max(dword_6FA8D220 - nOffset, 0);
            const int32_t nSize = D2Clamp(dword_6FA8469C - nOffset, 0, dword_6FA7E0A0[nStart]) - nIndex;
            if (nSize > 0)
            {
                uint8_t* pIndex = (uint8_t*)((char*)byte_6FA85220 + 32 * ((v12 >> 7) + 32 * (v11 >> 7)) + dword_6FA7E064[nStart]);
                uint8_t* pCurrent = &a4[nStart * dword_6FA81450 + dword_6FA7E064[nStart] + nIndex];
                uint8_t* pStart = &a1->dwOffset_pData[dword_6FA7E0DC[nStart] + nIndex];
                uint8_t* pEnd = &pStart[nSize];
                while (pStart < pEnd)
                {
                    *pCurrent = stru_6FA81580[*pIndex].unk0x00[*pStart];
                    ++pCurrent;
                    ++pStart;
                    ++pIndex;
                }
            }

            v11 += v7;
            v12 += v9;
        }
    }
}

//D2Gfx.0x6FA73410) --------------------------------------------------------
void __fastcall D2GFX_FloorTileDraw_6FA73410(D2TileLibraryEntryStrc* pTileLibraryEntry, int32_t nX, int32_t nY, D2GfxLightExStrc* pLightEx)
{
    uint8_t* pBaseDst = gCmnInfo_pBuffer_6FA8144C + nY * dword_6FA81450 + nX;
    for (int32_t nBlockIdx = 0; nBlockIdx < pTileLibraryEntry->nBlocks; ++nBlockIdx)
    {
        D2TileLibraryBlockStrc* pBlock = &pTileLibraryEntry->pBlock[nBlockIdx];
        if (!(pBlock->nFormat & 1))
        {
            continue;
        }

        const int32_t nBlockX = nX + pBlock->nPosX;
        const int32_t nBlockY = nY + pBlock->nPosY;
        if (nBlockY < dword_6FA8D224 || nBlockY >= dword_6FA8D23C || nBlockX < dword_6FA8D244 || nBlockX >= dword_6FA84698)
        {
            continue;
        }

        uint8_t* pBlockDst = pBaseDst + pBlock->nPosX + dword_6FA81450 * pBlock->nPosY;
        BOOL bFlatLit = FALSE;
        uint8_t nFlatIntensity = 0;
        int32_t aIntensities[4] = {};

        if (dword_6FA8D250)
        {
            bFlatLit = TRUE;
            nFlatIntensity = pLightEx[pBlock->nGridX + 8 * pBlock->nGridY + 9].nIntensity;
        }
        else
        {
            D2GfxLightExStrc* pCellLight = &pLightEx[pBlock->nGridX + 8 * pBlock->nGridY];
            const int32_t nI10 = pCellLight[10].nIntensity;
            const int32_t nI11 = pCellLight[11].nIntensity;
            const int32_t nI17 = pCellLight[17].nIntensity;
            const int32_t nI09 = pCellLight[9].nIntensity;
            const int32_t nI18 = pCellLight[18].nIntensity;
            const int32_t nI19 = pCellLight[19].nIntensity;

            const int32_t nDiff1 = std::abs(nI10 - nI09);
            const int32_t nDiff2 = std::abs(nI17 - nI09);
            const int32_t nDiff3 = std::abs(nI18 - nI10);

            if (!gGfxSettings.bLowQuality && (nDiff1 + nDiff2 + nDiff3 >= 10))
            {
                aIntensities[0] = (pCellLight[8].nIntensity + nI09 + pCellLight[16].nIntensity + nI17) >> 2;
                aIntensities[1] = (nI09 + pCellLight[2].nIntensity + nI10 + pCellLight[1].nIntensity) >> 2;
                aIntensities[2] = (nI10 + nI11 + nI19 + nI17) >> 2;
                aIntensities[3] = (nI17 + pCellLight[26].nIntensity + nI18 + pCellLight[25].nIntensity) >> 2;
                bFlatLit = FALSE;
            }
            else
            {
                bFlatLit = TRUE;
                nFlatIntensity = (uint8_t)nI09;
            }
        }

        if (nBlockY < dword_6FA8D228 || nBlockY >= dword_6FA84694 || nBlockX < dword_6FA8468C || nBlockX >= dword_6FA8D22C)
        {
            if (bFlatLit)
            {
                sub_6FA73130(pBlock, nBlockX, nBlockY, pBlockDst, nFlatIntensity);
            }
            else
            {
                sub_6FA73270(pBlock, nBlockX, nBlockY, pBlockDst, aIntensities);
            }
        }
        else
        {
            if (bFlatLit)
            {
                uint8_t* pPal = stru_6FA81580[(uint32_t)nFlatIntensity >> 3].unk0x00;
                if (pBlock->nFormat & 4)
                {
                    sub_6FA71720(pBlock->dwOffset_pData, pBlockDst, pPal, 15);
                }
                else
                {
                    sub_6FA71130(pBlock->dwOffset_pData, pBlockDst, pPal);
                }
            }
            else
            {
                if (pBlock->nFormat & 4)
                {
                    sub_6FA71970(pBlock->dwOffset_pData, pBlockDst, 16 * aIntensities[0], aIntensities[3] - aIntensities[0], 16 * aIntensities[1], aIntensities[2] - aIntensities[1], 15, 7);
                }
                else
                {
                    sub_6FA71340(pBlock->dwOffset_pData, pBlockDst, 16 * aIntensities[0], aIntensities[3] - aIntensities[0], 16 * aIntensities[1], aIntensities[2] - aIntensities[1]);
                }
            }
        }
    }
}
