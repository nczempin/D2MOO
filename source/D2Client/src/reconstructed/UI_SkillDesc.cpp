// ============================================================================
// UI_SkillDesc.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll
// Address ranges:
//   - 0x6FAF2060 .. 0x6FAFB7E0 (asm/UI_SkillDesc.asm, 114 functions)
//   - 0x6FB074E0 .. 0x6FB076B0 (asm/UI_spellsel.asm, 5 functions)
// Original source: ..\Source\D2Client\UI\SkillDesc.cpp
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Units/Units.h>
#include <Units/UnitRoom.h>
#include <Units/Missile.h>
#include <Path/Path.h>
#include <Path/PathMisc.h>
#include <Path/Step.h>
#include <UselessOrdinals.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgPreset.h>
#include <D2Environment.h>
#include <D2Inventory.h>
#include <D2Items.h>
#include <D2Monsters.h>
#include <D2Seed.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <D2DataTbls.h>
#include <D2Gfx.h>
#include <D2Lang.h>
#include <D2Sound.h>
#include <D2WinMain.h>
#include <D2WinFont.h>
#include <D2WinImage.h>
#include <D2WinTextBox.h>
#include <Fog.h>
#include <Storm.h>

#pragma pack(push, 1)

struct D2ElementalDamageStatStrc
{
    int32_t nElemType;                  // 0x00
    int32_t nStatMinId;                 // 0x04
    int32_t nStatMaxId;                 // 0x08
};

struct D2CharPagTextBoxStrc
{
    int32_t nXLeft;                     // 0x00
    int32_t nYBase;                     // 0x04
    int32_t nXRight;                    // 0x08
};

#pragma pack(pop)

// API compatibility shims for 1.12a UI_SkillDesc
static inline const wchar_t* D2LANG_GetStringByTblIndexW(uint16_t nIndex)
{
    const Unicode* pStr = D2LANG_GetStringByTblIndex(nIndex);
    return pStr ? reinterpret_cast<const wchar_t*>(pStr) : L"";
}
#define D2LANG_GetStringByTblIndex D2LANG_GetStringByTblIndexW

#define SKILLS_GetElementalMinDamage SKILLS_GetMinElemDamage
#define SKILLS_GetElementalMaxDamage SKILLS_GetMaxElemDamage
#define ITEMS_CheckWeaponIfThrow ITEMS_CheckIfThrowable
#define MISSILE_GetElemDamageLength MISSILE_GetElementalLength

static inline int32_t DATATBLS_GetStatesTxtRecordCount(void)
{
    return sgptDataTables ? sgptDataTables->nStatesTxtRecordCount : 0;
}

static inline int32_t MISSILE_CalculateMasteryBonus(int32_t nMissileId, D2UnitStrc* pUnit, int32_t nSkillLevel)
{
    return MISSILE_GetMinDamage(nullptr, pUnit, nMissileId, nSkillLevel);
}

static inline int32_t MISSILE_GetRange(D2UnitStrc* pMissile, D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillLevel)
{
    return MISSILE_EvaluateMissileFormula(pMissile, pUnit, 0, nMissileId, nSkillLevel);
}

static inline D2SkillStrc* SKILLS_GetFirstSkillFromSkillList(void* pSkillList)
{
    return ::SKILLS_GetFirstSkillFromSkillList(reinterpret_cast<D2SkillListStrc*>(pSkillList));
}

static inline int32_t SKILLS_GetProgressiveSkillId(D2UnitStrc* pUnit, int32_t nSkillId)
{
    (void)pUnit;
    if (!sgptDataTables || nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
        return -1;
    return sgptDataTables->pSkillsTxt[nSkillId].wSummon;
}

static inline BOOL SKILLS_CalculateGetLeftHandSwing(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pOutLevel)
{
    (void)pUnit;
    (void)nSkillId;
    if (pOutLevel) *pOutLevel = nSkillLevel;
    return nSkillLevel > 0;
}

inline int32_t D2WIN_GetTextWidth(const wchar_t* wszText)
{
    return D2Win_10121_GetTextWidth(reinterpret_cast<const Unicode*>(wszText));
}

inline void D2WIN_DrawText(const wchar_t* wszText, int32_t nX, int32_t nY, int32_t nColor, int32_t bCentered)
{
    D2Win_10117_DrawText(reinterpret_cast<const Unicode*>(wszText), nX, nY, nColor, bCentered);
}

inline Font D2WIN_SetFont(int32_t nFont)
{
    return D2Win_10127_SetFont(static_cast<Font>(nFont));
}

static inline void D2WIN_WordWrap(wchar_t* wszText, int32_t nMaxWidth)
{
    (void)wszText;
    (void)nMaxWidth;
}

inline void D2WIN_DrawFramedText(const wchar_t* wszText, int32_t nX, int32_t nY, int32_t nColor, int32_t nAlign)
{
    D2Win_10129_DrawFramedText(reinterpret_cast<const Unicode*>(wszText), nX, nY, nColor, nAlign);
}

// External 1.12a D2Client helpers referenced by UI_SkillDesc
D2SkillsTxt* __fastcall sub_6FAFF7E0(int32_t nSkillId);
D2UnitStrc* __fastcall sub_6FB57830(D2InventoryStrc* pInventory);
uint8_t __cdecl sub_6FAFF090(void);
D2UnitStrc* __cdecl sub_6FAFF3D0(void);
int32_t __cdecl sub_6FAFE930(void);
int32_t __fastcall sub_6FAFCE90(D2UnitStrc* pUnit, int32_t nType, D2UnitStrc* pWeapon);
int32_t __fastcall sub_6FAFCD70(D2UnitStrc* pUnit, int32_t nType, D2UnitStrc* pWeapon);
int32_t __fastcall sub_6FAFCC20(D2UnitStrc* pUnit, int32_t nType, D2UnitStrc* pWeapon);
const wchar_t* __fastcall sub_6FB200F0(int32_t nColor);
void __fastcall sub_6FB201A0(wchar_t* wszOut, const wchar_t* wszStrA, int32_t nVal, const wchar_t* wszStrB);
void __fastcall sub_6FB201D0(wchar_t* wszOut, const wchar_t* wszStrA, int32_t nVal, const wchar_t* wszStrB);
void __fastcall sub_6FB20200(wchar_t* wszOut, const wchar_t* wszStrA, const wchar_t* wszMid, const wchar_t* wszStrB);
void __fastcall sub_6FB20280(const wchar_t* wszStrA, wchar_t* wszOut, int32_t nVal);
void __fastcall sub_6FB202B0(const wchar_t* wszStrA, wchar_t* wszOut, int32_t nVal);
void __fastcall sub_6FB202E0(const wchar_t* wszStrA, wchar_t* wszOut, const wchar_t* wszVal);
void __fastcall sub_6FB20310(wchar_t* wszOut, int32_t nVal, const wchar_t* wszStr);
void __fastcall sub_6FB20340(wchar_t* wszOut, int32_t nVal, const wchar_t* wszStr);

// Internal forward declarations for UI_SkillDesc & UI_spellsel functions
void __cdecl sub_6FAF2060(void);
BOOL __fastcall sub_6FAF2090(D2SkillStrc* pSkill, D2UnitStrc* pUnit);
void __fastcall sub_6FAF20D0(D2UnitStrc* pUnit, D2StatListStrc** ppSavedStatList, D2UnitStrc* pLeftWeapon);
void __fastcall sub_6FAF2170(D2StatListStrc* pSavedStatList, D2UnitStrc* pUnit, D2UnitStrc* pLeftWeapon);
D2SkillDescTxt* __fastcall sub_6FAF21F0(int32_t nSkillId);
int32_t __fastcall sub_6FAF2210(D2UnitStrc* pWeapon, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);
int32_t __fastcall sub_6FAF2310(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);
int32_t __fastcall sub_6FAF2430(D2UnitStrc* pWeapon, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);
D2SkillDescTxt* __fastcall sub_6FAF2540(uint16_t wSkillDesc);
void __fastcall sub_6FAF2560(int32_t nVal, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB);
void __fastcall sub_6FAF2660(int32_t nMin, int32_t nMax, wchar_t* wszOut);
void __fastcall sub_6FAF26C0(int32_t nMin, int32_t nMax, wchar_t* wszOut);
void __fastcall sub_6FAF2720(int32_t nMin, int32_t nMax, wchar_t* wszOut);
void __fastcall sub_6FAF2760(int32_t nMin, int32_t nMax, wchar_t* wszOut);
void __fastcall sub_6FAF27A0(const wchar_t* wszPrefix, wchar_t* wszOut, int32_t nMin, int32_t nMax);
void __fastcall sub_6FAF2840(uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, int32_t nVal);
void __fastcall sub_6FAF2950(uint16_t wStrA, wchar_t* wszOut, uint16_t wStrB, int32_t nVal);
void __fastcall sub_6FAF29B0(uint16_t wStrA, wchar_t* wszOut, uint16_t wStrB, int32_t nVal);
void __fastcall sub_6FAF2A10(int32_t nMin, int32_t nMax, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB);
void __fastcall sub_6FAF2AB0(int32_t nMin, int32_t nMax, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB);
void __fastcall sub_6FAF2B50(int32_t nMin, int32_t nMax, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB);
void __fastcall sub_6FAF2BF0(int32_t nMin, int32_t nMax, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB);
void __fastcall sub_6FAF2C90(int32_t nMin, int32_t nMax, uint16_t wStr, wchar_t* wszOut);
void __fastcall sub_6FAF2CE0(int32_t nMin, int32_t nMax, uint16_t wStr, wchar_t* wszOut);
void __fastcall sub_6FAF2D30(int32_t nMin, int32_t nMax, uint16_t wStr, wchar_t* wszOut);
void __fastcall sub_6FAF2D80(int32_t nMin, int32_t nMax, uint16_t wStr, wchar_t* wszOut);
int32_t __fastcall sub_6FAF2DD0(int32_t nMonType, int32_t nMonLevel, D2MonStatInitStrc* pMonStatInit);
int32_t __fastcall sub_6FAF2E30(int32_t nSkillId, D2UnitStrc* pUnit, int32_t nSkillLevel, D2MonStatInitStrc* pMonStatInit, int32_t* pHpOut);
void __fastcall sub_6FAF2F00(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, wchar_t* wszOut);
void __fastcall sub_6FAF3090(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, wchar_t* wszOut);
void __fastcall sub_6FAF3240(wchar_t* wszText, int32_t nColor, int32_t nRow, int32_t nCol);
void __fastcall sub_6FAF3300(int32_t nCol, int32_t nRow, D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nColor);
int32_t __fastcall sub_6FAF3410(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);
void __fastcall sub_6FAF3450(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, int32_t nLineIdx, wchar_t* wszOut);
void __fastcall sub_6FAF3500(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, int32_t nLineIdx, wchar_t* wszOut);
int32_t __fastcall sub_6FAF35C0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);
int32_t __fastcall sub_6FAF3600(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t* pAR, int32_t* pColor);
void __fastcall sub_6FAF3690(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, int32_t nLineIdx, wchar_t* wszOut);
void __fastcall sub_6FAF3750(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF3790(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF37D0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF3820(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF3870(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF38D0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF3920(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF3980(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF3A30(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut);
void __fastcall sub_6FAF3A70(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut);
void __fastcall sub_6FAF3AB0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut);
void __fastcall sub_6FAF3B00(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut);
void __fastcall sub_6FAF3B50(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF3C20(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF3CF0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF3EC0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF40E0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
int32_t __fastcall sub_6FAF4290(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor);
void __fastcall sub_6FAF4330(int32_t nSkillId, int32_t nSkillLevel, uint16_t wMissileId, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF43E0(int32_t nSkillId, int32_t nSkillLevel, uint16_t wMissileId, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF44A0(int32_t nSkillId, int32_t nSkillLevel, uint16_t wMissileId, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF4560(int32_t nSkillId, int32_t nSkillLevel, uint16_t wMissileId, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF4620(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF4680(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF46E0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF4750(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF4810(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF48D0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF49B0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF4AA0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF4B90(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut);
void __fastcall sub_6FAF4BF0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut);
void __fastcall sub_6FAF4C50(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF4D70(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF4DC0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF4EA0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF4FE0(D2UnitStrc* pUnit, int32_t nRow, D2SkillStrc* pSkill);
void __fastcall sub_6FAF5050(int32_t nSkillId, D2UnitStrc* pUnit, int32_t nSkillLevel, D2UnitStrc* pWeapon, int32_t* pElemBonus, int32_t* pMinElem, int32_t* pMaxElem, int32_t* pElemType, int32_t nOverrideElemType);
int32_t __fastcall sub_6FAF51A0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);
int32_t __fastcall sub_6FAF51F0(D2UnitStrc* pUnit, int32_t* pAR, int32_t* pColor);
void __fastcall sub_6FAF5220(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut);
void __fastcall sub_6FAF5330(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF54E0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB);
void __fastcall sub_6FAF5530(D2SkillDescTxt* pSkillDesc, int32_t nSkillLevel, D2UnitStrc* pUnit, int32_t nLineIdx, wchar_t* wszOut);
void __fastcall sub_6FAF5690(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, int32_t nLineIdx, wchar_t* wszOut);
void __fastcall sub_6FAF58C0(D2UnitStrc* pUnit, int32_t* pMinOut, int32_t* pMaxOut, int32_t* pElemTypeOut, int32_t nOverrideElemType);
void __fastcall sub_6FAF5A90(D2UnitStrc* pUnit, int32_t* pMinOut, int32_t* pMaxOut, int32_t* pElemTypeOut, int32_t nOverrideElemType);
uint16_t __stdcall sub_6FAF5BD0(int32_t nSkillId);
uint16_t __stdcall sub_6FAF5BF0(int32_t nSkillId);
uint16_t __stdcall sub_6FAF5C10(int32_t nSkillId);
uint16_t __stdcall sub_6FAF5C30(int32_t nSkillId);
uint16_t __stdcall sub_6FAF5C50(int32_t nSkillId);
void __fastcall sub_6FAF5C70(D2UnitStrc* pUnit, int32_t nRow, D2SkillStrc* pSkill);
int32_t __fastcall sub_6FAF5DC0(int32_t nSkillId, D2UnitStrc* pUnit, int32_t nSkillLevel, D2UnitStrc* pWeapon, int32_t* pPhysBonus, int32_t* pMinPhys, int32_t* pMaxPhys, int32_t nApplyPenalty);
int32_t __fastcall sub_6FAF61B0(int32_t nSkillId, D2UnitStrc* pUnit, int32_t nSkillLevel, D2UnitStrc* pWeapon, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nApplyPenalty, int32_t nElemType);
int32_t __fastcall sub_6FAF6280(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF63E0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF6510(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF66F0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF6800(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF6A20(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF6CF0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);
void __fastcall sub_6FAF7410(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, wchar_t* wszOut);
int32_t __stdcall sub_6FAF7560(int32_t nSkillId, wchar_t* wszOut, uint32_t nMaxChars);
void __fastcall sub_6FAF75A0(D2UnitStrc* pUnit, int32_t nRow);
void __fastcall sub_6FAF7600(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, int32_t nLineIdx, wchar_t* wszOut);
int32_t __fastcall sub_6FAF8B30(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF8D60(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
void __cdecl sub_6FAF91D0(void);
void __cdecl sub_6FAF9490(void);
void __fastcall sub_6FAF9A90(int32_t nSkillId, int32_t nHoverIdx);
int32_t __fastcall sub_6FAFA030(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA1E0(D2UnitStrc* pUnit, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor);
int32_t __fastcall sub_6FAFA210(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA260(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA580(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
void __fastcall sub_6FAFA620(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, int32_t* pMinOut, int32_t* pMaxOut);
int32_t __fastcall sub_6FAFA8A0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA930(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA9F0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFAB40(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFAD40(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFAD90(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFAFB0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFB080(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFB1B0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFB2E0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFB3B0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFB480(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
BOOL __stdcall sub_6FB074E0(D2UnitStrc* pUnit, int32_t nSkillId);
BOOL __stdcall sub_6FB07530(D2UnitStrc* pUnit, int32_t nSkillId);
BOOL __stdcall sub_6FB07580(D2UnitStrc* pUnit, int32_t nSkillId);
BOOL __stdcall sub_6FB075D0(D2UnitStrc* pUnit, int32_t nSkillId);
void __cdecl sub_6FB07620(void);

typedef int32_t (__fastcall *CharPagDmgCallback_t)(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
typedef int32_t (__fastcall *CharPagARCallback_t)(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);

// ----------------------------------------------------------------------------
// External Data & Helpers
// ----------------------------------------------------------------------------
extern int32_t g_nScreenOffsetX_6FBA79A0;                           // 0x6FBA79A0
extern int32_t g_nSkillTreeTab_6FBA79FC;                            // 0x6FBA79FC
extern int32_t g_nSkillHoverY_6FBBA6CC;                             // 0x6FBBA6CC
extern int32_t g_nSkillHoverX_6FBBA6D0;                             // 0x6FBBA6D0
extern int32_t g_nHoveredSkillId_6FBBA6E8;                          // 0x6FBBA6E8
extern int32_t g_bSkillHoverIsRight_6FBC21C0;                       // 0x6FBC21C0
extern int32_t g_nSpeedBarCount_6FBC21C8;                           // 0x6FBC21C8
extern int32_t g_nSpeedBarHoverIdx_6FBC21CC;                        // 0x6FBC21CC
extern int32_t g_nSpeedBarSkillIds_6FBC21D8[60];                    // 0x6FBC21D8
extern int32_t g_nSpeedBarSkillFlags_6FBC22C8[60];                  // 0x6FBC22C8
extern int32_t g_nSpeedBarSkillLevels_6FBC23B8[60];                 // 0x6FBC23B8
extern int32_t g_nSpeedBarSkillY_6FBC24A8[60];                      // 0x6FBC24A8
extern int32_t g_nSpeedBarSkillX_6FBC2598[60];                      // 0x6FBC2598
extern D2SkillDescTxt* g_pSkillDescTxt_6FDD1C44;                    // 0x6FDD1C44 (*0x6FB7D5FC)
extern int32_t g_nSkillDescTxtRecordCount_6FDD1C4C;                 // 0x6FDD1C4C (*0x6FB7D61C)

// 1.12a: 0x6FB85B68 (Elemental damage stat table)
static const D2ElementalDamageStatStrc g_aElemDamageStats_6FB85B68[5] = {
    {   1,  9,  3 }, // Physical / Magic
    {   3, 48, 49 }, // Fire (STATS_FIREMINDAM, STATS_FIREMAXDAM)
    { 329, 50, 51 }, // Lightning (STATS_LIGHTMINDAM, STATS_LIGHTMAXDAM)
    { 330, 54, 55 }, // Cold (STATS_COLDMINDAM, STATS_COLDMAXDAM)
    { 331, 52, 53 }, // Poison (STATS_POISONMINDAM, STATS_POISONMAXDAM)
};

// 1.12a: 0x6FBA3980 (Character screen text box coordinates [3 rows][3 cols])
static const D2CharPagTextBoxStrc g_aCharPagTextBoxes_6FBA3980[3][3] = {
    { { 162,  93, 258 }, { 162, 101, 258 }, { 263,  98, 307 } },
    { { 162, 160, 270 }, { 162, 163, 270 }, { 270, 160, 310 } },
    { { 162, 117, 258 }, { 162, 125, 258 }, { 263, 122, 307 } },
};

// Forward declarations for table entries
int32_t __fastcall sub_6FAFB3B0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF6A20(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA930(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA8A0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFB1B0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFB480(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFAB40(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF66F0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFAFB0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF6510(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFAD90(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF63E0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA210(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF8D60(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF6280(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF8B30(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA030(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFAD40(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA9F0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFB2E0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAF6800(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA580(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFA260(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);
int32_t __fastcall sub_6FAFB080(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType);

int32_t __fastcall sub_6FAF35C0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);
int32_t __fastcall sub_6FAF6CF0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);
int32_t __fastcall sub_6FAF3410(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);
int32_t __fastcall sub_6FAF51A0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);
int32_t __fastcall sub_6FAF2310(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor);

// 1.12a: 0x6FBA38A8 (Character screen Damage function table, 36 entries)
static const CharPagDmgCallback_t g_apfnCharPagDmg_6FBA38A8[36] = {
    nullptr,
    sub_6FAFB3B0, // 1
    sub_6FAF6A20, // 2
    sub_6FAFA930, // 3
    sub_6FAFA8A0, // 4
    sub_6FAFB1B0, // 5
    sub_6FAFB480, // 6
    sub_6FAFAB40, // 7
    sub_6FAF66F0, // 8
    sub_6FAFAFB0, // 9
    sub_6FAF6510, // 10
    sub_6FAFAD90, // 11
    sub_6FAF63E0, // 12
    sub_6FAFA210, // 13
    sub_6FAF8D60, // 14
    sub_6FAF6280, // 15
    sub_6FAF8B30, // 16
    sub_6FAFA030, // 17
    sub_6FAFAD40, // 18
    sub_6FAFA9F0, // 19
    sub_6FAFB2E0, // 20
    sub_6FAF6800, // 21
    sub_6FAFA580, // 22
    sub_6FAFA260, // 23
    sub_6FAFB080, // 24
};

// 1.12a: 0x6FBA3938 (Character screen Attack Rating function table, 18 entries)
static const CharPagARCallback_t g_apfnCharPagAR_6FBA3938[18] = {
    nullptr,
    sub_6FAF35C0, // 1
    sub_6FAF6CF0, // 2
    sub_6FAF3410, // 3
    sub_6FAF51A0, // 4
    sub_6FAF2310, // 5
};

// ============================================================================
// Reconstructed Functions: UI_SkillDesc.asm (0x6FAF2060 .. 0x6FAFB7E0)
// ============================================================================

// 1.12a: 0x6FAF2060
// Calling convention: __cdecl
void __cdecl sub_6FAF2060(void)
{
    LARGE_INTEGER PerformanceCount;
    QueryPerformanceCounter(&PerformanceCount);
}

// 1.12a: 0x6FAF2090
// Calling convention: __fastcall (EAX = pSkill, EDX = pUnit)
BOOL __fastcall sub_6FAF2090(D2SkillStrc* pSkill, D2UnitStrc* pUnit)
{
    if (!pUnit || !pSkill)
        return FALSE;
    return SKILLS_GetSkillLevel(pUnit, pSkill, 1) != 0
        && SKILLS_GetSkillLevel(pUnit, pSkill, 0) == 0;
}

// 1.12a: 0x6FAF20D0
// Calling convention: __fastcall (EAX = pUnit, ECX = ppSavedStatList, [esp+4] = pLeftWeapon)
void __fastcall sub_6FAF20D0(D2UnitStrc* pUnit, D2StatListStrc** ppSavedStatList, D2UnitStrc* pLeftWeapon)
{
    *ppSavedStatList = nullptr;
    if (!pUnit || !UNITS_CanDualWield(pUnit))
        return;

    D2InventoryStrc* pInventory = *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60);
    if (!pInventory || !INVENTORY_GetLeftHandWeapon(pInventory))
        return;

    D2UnitStrc* pWeapon = pLeftWeapon ? pLeftWeapon : INVENTORY_GetLeftHandWeapon(pInventory);
    int32_t nBodyLoc = (ITEMS_GetBodyLocation(pWeapon) == 4) ? 5 : 4;
    D2UnitStrc* pOtherWeapon = INVENTORY_GetItemFromBodyLoc(pInventory, nBodyLoc);
    if (pOtherWeapon && pOtherWeapon != pWeapon && ITEMS_CanBeEquipped(pOtherWeapon))
    {
        *ppSavedStatList = STATLIST_GetStatListFromUnitStateAndFlag(pOtherWeapon, 0, 0x40);
        if (*ppSavedStatList)
            STATLIST_MergeStatLists(pUnit, pWeapon, 1);
    }
}

// 1.12a: 0x6FAF2170
// Calling convention: __fastcall (ECX = pSavedStatList, EDX = pUnit, [esp+4] = pLeftWeapon)
void __fastcall sub_6FAF2170(D2StatListStrc* pSavedStatList, D2UnitStrc* pUnit, D2UnitStrc* pLeftWeapon)
{
    if (!pUnit || !UNITS_CanDualWield(pUnit))
        return;

    D2InventoryStrc* pInventory = *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60);
    if (!pInventory)
        return;

    D2UnitStrc* pWeapon = pLeftWeapon ? pLeftWeapon : INVENTORY_GetLeftHandWeapon(pInventory);
    if (!pWeapon)
        return;

    int32_t nBodyLoc = (ITEMS_GetBodyLocation(pWeapon) == 4) ? 5 : 4;
    D2UnitStrc* pOtherWeapon = INVENTORY_GetItemFromBodyLoc(pInventory, nBodyLoc);
    if (pOtherWeapon && pOtherWeapon != pWeapon && pSavedStatList)
    {
        STATLIST_MergeStatLists(pUnit, pOtherWeapon, 1);
    }
}

// 1.12a: 0x6FAF21F0
// Calling convention: __fastcall (ECX = nSkillId)
D2SkillDescTxt* __fastcall sub_6FAF21F0(int32_t nSkillId)
{
    D2SkillsTxt* pSkillsTxt = sub_6FAFF7E0(nSkillId);
    if (!pSkillsTxt)
        return nullptr;
    uint16_t wSkillDesc = pSkillsTxt->wSkillDesc;
    if (wSkillDesc <= 0 || wSkillDesc >= g_nSkillDescTxtRecordCount_6FDD1C4C)
        return nullptr;
    return &g_pSkillDescTxt_6FDD1C44[wSkillDesc];
}

// 1.12a: 0x6FAF2210
// Calling convention: __usercall (EDX = pUnit, ECX = pWeapon, [esp+4] = nSkillId, [esp+8] = nSkillLevel, [esp+12] = pAR, [esp+16] = pColor)
int32_t __fastcall sub_6FAF2210(D2UnitStrc* pWeapon, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor)
{
    *pAR = 0;
    *pColor = 0;
    if (!pUnit)
        return 0;

    D2StatListStrc* pSavedStatList = nullptr;
    sub_6FAF20D0(pUnit, &pSavedStatList, pWeapon);

    int32_t nBaseAR = UNITS_GetAttackRate(pUnit);
    int32_t nBonusAR = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 0x13, 0);
    int32_t nPctAR = STATLIST_UnitGetStatValue(pUnit, 0x77, 0) + SKILLS_GetWeaponMasteryBonus(pUnit, pWeapon, 0, 0);

    if (nSkillId >= 0 && nSkillLevel > 0)
    {
        uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
        if (pSkillsTxt && pSkillsTxt[0x19] == 0)
            nPctAR += SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel);
    }

    sub_6FAF2170(pSavedStatList, pUnit, pWeapon);

    int32_t nTotalAR = nBaseAR + nBonusAR + (nBaseAR + nBonusAR) * nPctAR / 100;
    *pAR = nTotalAR;
    if (STATES_CheckStateMaskAttBlueOnUnit(pUnit) || nTotalAR > nBaseAR)
        *pColor = 3;
    if (STATES_CheckStateMaskAttRedOnUnit(pUnit) || nTotalAR < nBaseAR)
        *pColor = 1;
    return 1;
}

// 1.12a: 0x6FAF2310
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF2310(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor)
{
    *pAR = 0;
    *pColor = 0;
    if (!pUnit)
        return 0;

    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    D2StatListStrc* pSavedStatList = nullptr;
    sub_6FAF20D0(pUnit, &pSavedStatList, pWeapon);

    int32_t nBaseAR = UNITS_GetAttackRate(pUnit);
    int32_t nBonusAR = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 0x13, 0);
    int32_t nPctAR = STATLIST_UnitGetStatValue(pUnit, 0x77, 0) + SKILLS_GetWeaponMasteryBonus(pUnit, pWeapon, 0, 0);

    uint8_t* pSkillsTxt = (nSkillId >= 0) ? (uint8_t*)sub_6FAFF7E0(nSkillId) : nullptr;
    if (pSkillsTxt && nSkillLevel > 0 && pSkillsTxt[0x19] == 0)
        nPctAR += SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel);

    if (pSkillsTxt)
    {
        for (int32_t i = 0; i < 5; ++i)
        {
            if (*(int16_t*)(pSkillsTxt + 0x92 + i * 2) == 0x77)
                nPctAR += SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x9C + i * 4), nSkillId, nSkillLevel);
        }
    }

    sub_6FAF2170(pSavedStatList, pUnit, pWeapon);

    int32_t nTotalAR = nBaseAR + nBonusAR + (nBaseAR + nBonusAR) * nPctAR / 100;
    *pAR = nTotalAR;
    if (STATES_CheckStateMaskAttBlueOnUnit(pUnit) || nTotalAR > nBaseAR)
        *pColor = 3;
    if (STATES_CheckStateMaskAttRedOnUnit(pUnit) || nTotalAR < nBaseAR)
        *pColor = 1;
    return 1;
}

// 1.12a: 0x6FAF2430
// Calling convention: __usercall (EDX = pUnit, ECX = pWeapon, [esp+4] = nSkillId, [esp+8] = nSkillLevel, [esp+12] = pAR, [esp+16] = pColor)
int32_t __fastcall sub_6FAF2430(D2UnitStrc* pWeapon, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor)
{
    *pAR = 0;
    *pColor = 0;
    if (!pUnit)
        return 0;

    D2StatListStrc* pSavedStatList = nullptr;
    sub_6FAF20D0(pUnit, &pSavedStatList, pWeapon);

    int32_t nBaseAR = UNITS_GetAttackRate(pUnit);
    int32_t nBonusAR = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 0x13, 0);
    int32_t nPctAR = STATLIST_UnitGetStatValue(pUnit, 0x77, 0);
    if (pWeapon && ITEMS_CheckWeaponIfThrow(pWeapon))
        nPctAR += STATLIST_UnitGetStatValue(pUnit, 0x89, 0);

    if (nSkillId >= 0 && nSkillLevel > 0)
    {
        uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
        if (pSkillsTxt && pSkillsTxt[0x19] == 0)
            nPctAR += SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel);
    }

    sub_6FAF2170(pSavedStatList, pUnit, pWeapon);

    int32_t nTotalAR = nBaseAR + nBonusAR + (nBaseAR + nBonusAR) * nPctAR / 100;
    *pAR = nTotalAR;
    if (STATES_CheckStateMaskAttBlueOnUnit(pUnit) || nTotalAR > nBaseAR)
        *pColor = 3;
    if (STATES_CheckStateMaskAttRedOnUnit(pUnit) || nTotalAR < nBaseAR)
        *pColor = 1;
    return 1;
}

// 1.12a: 0x6FAF2540
// Calling convention: __fastcall (ECX = wSkillDesc)
D2SkillDescTxt* __fastcall sub_6FAF2540(uint16_t wSkillDesc)
{
    if (wSkillDesc <= 0 || wSkillDesc >= g_nSkillDescTxtRecordCount_6FDD1C4C)
        return nullptr;
    return &g_pSkillDescTxt_6FDD1C44[wSkillDesc];
}

// 1.12a: 0x6FAF2560
// Calling convention: __fastcall (ECX = nVal, EDX = wszOut, [esp+4] = wStrA, [esp+8] = wStrB)
void __fastcall sub_6FAF2560(int32_t nVal, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB)
{
    wchar_t wszNum[32];
    swprintf(wszNum, L"%d.%d", nVal / 25, ((nVal % 25) * 10) / 25);
    if (wStrB)
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(wStrA), wszNum, D2LANG_GetStringByTblIndex(wStrB));
    else
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszOut, wszNum);
}

// 1.12a: 0x6FAF2660
// Calling convention: __usercall (EAX = wszOut, ECX = nMin, EDX = nMax)
void __fastcall sub_6FAF2660(int32_t nMin, int32_t nMax, wchar_t* wszOut)
{
    if (nMin >= nMax)
        swprintf(wszOut, L"+%d.%d", nMin >> 8, ((nMin & 0xFF) * 10) >> 8);
    else
        swprintf(wszOut, L"+%d.%d-%d.%d", nMin >> 8, ((nMin & 0xFF) * 10) >> 8, nMax >> 8, ((nMax & 0xFF) * 10) >> 8);
}

// 1.12a: 0x6FAF26C0
// Calling convention: __usercall (EAX = wszOut, ECX = nMin, EDX = nMax)
void __fastcall sub_6FAF26C0(int32_t nMin, int32_t nMax, wchar_t* wszOut)
{
    if (nMin >= nMax)
        swprintf(wszOut, L"%d.%d", nMin >> 8, ((nMin & 0xFF) * 10) >> 8);
    else
        swprintf(wszOut, L"%d.%d-%d.%d", nMin >> 8, ((nMin & 0xFF) * 10) >> 8, nMax >> 8, ((nMax & 0xFF) * 10) >> 8);
}

// 1.12a: 0x6FAF2720
// Calling convention: __usercall (EAX = wszOut, ECX = nMin, EDX = nMax)
void __fastcall sub_6FAF2720(int32_t nMin, int32_t nMax, wchar_t* wszOut)
{
    if (nMin >= nMax)
        swprintf(wszOut, L"+%d", nMin >> 8);
    else
        swprintf(wszOut, L"+%d-%d", nMin >> 8, nMax >> 8);
}

// 1.12a: 0x6FAF2760
// Calling convention: __usercall (EAX = wszOut, ECX = nMin, EDX = nMax)
void __fastcall sub_6FAF2760(int32_t nMin, int32_t nMax, wchar_t* wszOut)
{
    if (nMin >= nMax)
        swprintf(wszOut, L"%d", nMin >> 8);
    else
        swprintf(wszOut, L"%d-%d", nMin >> 8, nMax >> 8);
}

// 1.12a: 0x6FAF27A0
// Calling convention: __fastcall (ECX = wszPrefix, EDX = wszOut, [esp+4] = nMin, [esp+8] = nMax)
void __fastcall sub_6FAF27A0(const wchar_t* wszPrefix, wchar_t* wszOut, int32_t nMin, int32_t nMax)
{
    int32_t nMinVal = nMin >> 8;
    int32_t nMaxVal = nMax >> 8;
    if (nMinVal >= nMaxVal)
    {
        if (nMinVal >= 100000)
            swprintf(wszOut, L"%s%dK", wszPrefix, nMinVal / 1000);
        else
            swprintf(wszOut, L"%s%d", wszPrefix, nMinVal);
    }
    else
    {
        if (nMaxVal >= 10000)
            swprintf(wszOut, L"%s%dK-%dK", wszPrefix, (nMinVal < 1000) ? 1 : (nMinVal / 1000), nMaxVal / 1000);
        else
            swprintf(wszOut, L"%s%d-%d", wszPrefix, nMinVal, nMaxVal);
    }
}

// 1.12a: 0x6FAF2840
// Calling convention: __fastcall (ECX = wStrA, EDX = wStrB, [esp+4] = wszOut, [esp+8] = nVal)
void __fastcall sub_6FAF2840(uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, int32_t nVal)
{
    int32_t nYards = nVal * 2;
    wchar_t wszNum[32];
    swprintf(wszNum, L"%d.%d", nYards / 3, ((nYards % 3) * 10) / 3);
    if (wStrB)
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(wStrA), wszNum, D2LANG_GetStringByTblIndex(wStrB));
    else
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszOut, wszNum);
}

// 1.12a: 0x6FAF2950
// Calling convention: __usercall (EAX = wStrB, ECX = wStrA, EDX = wszOut, [esp+4] = nVal)
void __fastcall sub_6FAF2950(uint16_t wStrA, wchar_t* wszOut, uint16_t wStrB, int32_t nVal)
{
    if (wStrB)
        sub_6FB201D0(wszOut, D2LANG_GetStringByTblIndex(wStrA), nVal, D2LANG_GetStringByTblIndex(wStrB));
    else
        sub_6FB202B0(D2LANG_GetStringByTblIndex(wStrA), wszOut, nVal);
}

// 1.12a: 0x6FAF29B0
// Calling convention: __usercall (EAX = wStrB, ECX = wStrA, EDX = wszOut, [esp+4] = nVal)
void __fastcall sub_6FAF29B0(uint16_t wStrA, wchar_t* wszOut, uint16_t wStrB, int32_t nVal)
{
    if (wStrB)
        sub_6FB201A0(wszOut, D2LANG_GetStringByTblIndex(wStrA), nVal, D2LANG_GetStringByTblIndex(wStrB));
    else
        sub_6FB20280(D2LANG_GetStringByTblIndex(wStrA), wszOut, nVal);
}

// 1.12a: 0x6FAF2A10
// Calling convention: __usercall (EAX = wszOut, ECX = nMin, EDX = nMax, [esp+4] = wStrA, [esp+8] = wStrB)
void __fastcall sub_6FAF2A10(int32_t nMin, int32_t nMax, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB)
{
    wchar_t wszRange[32];
    sub_6FAF2760(nMin, nMax, wszRange);
    if (wStrB)
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(wStrA), wszRange, D2LANG_GetStringByTblIndex(wStrB));
    else
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszOut, wszRange);
}

// 1.12a: 0x6FAF2AB0
// Calling convention: __usercall (EAX = wszOut, ECX = nMin, EDX = nMax, [esp+4] = wStrA, [esp+8] = wStrB)
void __fastcall sub_6FAF2AB0(int32_t nMin, int32_t nMax, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB)
{
    wchar_t wszRange[32];
    sub_6FAF2720(nMin, nMax, wszRange);
    if (wStrB)
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(wStrA), wszRange, D2LANG_GetStringByTblIndex(wStrB));
    else
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszOut, wszRange);
}

// 1.12a: 0x6FAF2B50
// Calling convention: __usercall (EAX = wszOut, ECX = nMin, EDX = nMax, [esp+4] = wStrA, [esp+8] = wStrB)
void __fastcall sub_6FAF2B50(int32_t nMin, int32_t nMax, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB)
{
    wchar_t wszRange[32];
    sub_6FAF2660(nMin, nMax, wszRange);
    if (wStrB)
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(wStrA), wszRange, D2LANG_GetStringByTblIndex(wStrB));
    else
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszOut, wszRange);
}

// 1.12a: 0x6FAF2BF0
// Calling convention: __usercall (EAX = wszOut, ECX = nMin, EDX = nMax, [esp+4] = wStrA, [esp+8] = wStrB)
void __fastcall sub_6FAF2BF0(int32_t nMin, int32_t nMax, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB)
{
    wchar_t wszRange[32];
    sub_6FAF26C0(nMin, nMax, wszRange);
    if (wStrB)
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(wStrA), wszRange, D2LANG_GetStringByTblIndex(wStrB));
    else
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszOut, wszRange);
}

// 1.12a: 0x6FAF2C90
// Calling convention: __usercall (EAX = wStr, ECX = nMin, EDX = nMax, [esp+4] = wszOut)
void __fastcall sub_6FAF2C90(int32_t nMin, int32_t nMax, uint16_t wStr, wchar_t* wszOut)
{
    wchar_t wszRange[32];
    sub_6FAF2760(nMin, nMax, wszRange);
    sub_6FB202E0(wszRange, wszOut, D2LANG_GetStringByTblIndex(wStr));
}

// 1.12a: 0x6FAF2CE0
// Calling convention: __usercall (EAX = wStr, ECX = nMin, EDX = nMax, [esp+4] = wszOut)
void __fastcall sub_6FAF2CE0(int32_t nMin, int32_t nMax, uint16_t wStr, wchar_t* wszOut)
{
    wchar_t wszRange[32];
    sub_6FAF2720(nMin, nMax, wszRange);
    sub_6FB202E0(wszRange, wszOut, D2LANG_GetStringByTblIndex(wStr));
}

// 1.12a: 0x6FAF2D30
// Calling convention: __usercall (EAX = wStr, ECX = nMin, EDX = nMax, [esp+4] = wszOut)
void __fastcall sub_6FAF2D30(int32_t nMin, int32_t nMax, uint16_t wStr, wchar_t* wszOut)
{
    wchar_t wszRange[32];
    sub_6FAF2660(nMin, nMax, wszRange);
    sub_6FB202E0(wszRange, wszOut, D2LANG_GetStringByTblIndex(wStr));
}

// 1.12a: 0x6FAF2D80
// Calling convention: __usercall (EAX = wStr, ECX = nMin, EDX = nMax, [esp+4] = wszOut)
void __fastcall sub_6FAF2D80(int32_t nMin, int32_t nMax, uint16_t wStr, wchar_t* wszOut)
{
    wchar_t wszRange[32];
    sub_6FAF26C0(nMin, nMax, wszRange);
    sub_6FB202E0(wszRange, wszOut, D2LANG_GetStringByTblIndex(wStr));
}

// 1.12a: 0x6FAF2DD0
// Calling convention: __usercall (ECX = nMonType, EDX = nMonLevel, [esp+4] = pMonStatInit)
int32_t __fastcall sub_6FAF2DD0(int32_t nMonType, int32_t nMonLevel, D2MonStatInitStrc* pMonStatInit)
{
    uint8_t nDifficulty = sub_6FAFF090();
    int32_t nGameType = 0;
    D2UnitStrc* pPlayer = sub_6FAFF3D0();
    if (pPlayer)
        nGameType = sub_6FAFE930() || (STATLIST_GetUnitAlignment(pPlayer) != 0);
    return DATATBLS_CalculateMonsterStatsByLevel(nMonType, nGameType, nDifficulty, nMonLevel, 0x3F, pMonStatInit);
}

// 1.12a: 0x6FAF2E30
// Calling convention: __usercall (EDX = pUnit, ECX = nSkillId, [esp+4] = nSkillLevel, [esp+8] = pMonStatInit, [esp+12] = pHpOut)
int32_t __fastcall sub_6FAF2E30(int32_t nSkillId, D2UnitStrc* pUnit, int32_t nSkillLevel, D2MonStatInitStrc* pMonStatInit, int32_t* pHpOut)
{
    int32_t nSummonId = SKILLS_GetProgressiveSkillId(pUnit, nSkillId);
    if (nSummonId < 0)
        return 0;

    int32_t nMonLevel = 0;
    if (!SKILLS_CalculateGetLeftHandSwing(pUnit, nSkillId, nSkillLevel, &nMonLevel))
        return 0;

    if (!sub_6FAF2DD0(nSummonId, nMonLevel, pMonStatInit))
        return 0;

    int32_t nHp = (pMonStatInit->nMinHP + pMonStatInit->nMaxHP) / 2;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        for (int32_t i = 0; i < 5; ++i)
        {
            if (*(int16_t*)(pSkillsTxt + 0x92 + i * 2) == 0x4D)
                nHp += nHp * SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x9C + i * 4), nSkillId, nSkillLevel) / 100;
        }
    }
    *pHpOut = nHp;
    return 1;
}

// 1.12a: 0x6FAF2F00
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wszOut)
void __fastcall sub_6FAF2F00(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, wchar_t* wszOut)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (!pSkillsTxt || !pSkillDesc)
        return;

    int32_t nMin = 0, nMax = 0;
    if (*(uint32_t*)(pSkillsTxt + 0x1A8))
        nMin = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x1A8), nSkillId, nSkillLevel) << 8;
    else
        nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);

    if (*(uint32_t*)(pSkillsTxt + 0x1AC))
        nMax = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x1AC), nSkillId, nSkillLevel) << 8;
    else
        nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);

    if (pSkillDesc->dwDamCalc[0])
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDamCalc[0], nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (pSkillDesc->dwDamCalc[1])
    {
        int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDamCalc[1], nSkillId, nSkillLevel) << 8;
        nMin += nBonus;
        nMax += nBonus;
    }

    int32_t nLen = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);
    if (pSkillsTxt[0x17C] == 5 && nLen > 0)
    {
        nMin *= nLen;
        nMax *= nLen;
    }

    if (nMax < nMin)
        nMax = nMin;
    sub_6FAF2760(nMin, nMax, wszOut);
}

// 1.12a: 0x6FAF3090
// Calling convention: __fastcall (ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pUnit, [esp+8] = wszOut)
void __fastcall sub_6FAF3090(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, wchar_t* wszOut)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (!pSkillsTxt || !pSkillDesc)
        return;

    int32_t nMin = 0, nMax = 0;
    if (*(uint32_t*)(pSkillsTxt + 0x1A8))
        nMin = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x1A8), nSkillId, nSkillLevel) << 8;
    else
        nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);

    if (*(uint32_t*)(pSkillsTxt + 0x1AC))
        nMax = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x1AC), nSkillId, nSkillLevel) << 8;
    else
        nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);

    if (pSkillDesc->dwDamCalc[0])
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDamCalc[0], nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (pSkillDesc->dwDamCalc[1])
    {
        int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDamCalc[1], nSkillId, nSkillLevel) << 8;
        nMin += nBonus;
        nMax += nBonus;
    }

    int32_t nLen = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);
    if (pSkillsTxt[0x17C] == 5 && nLen > 0)
    {
        nMin *= nLen;
        nMax *= nLen;
    }

    nMin = (nMin * 25) >> 8;
    nMax = (nMax * 25) >> 8;
    if (nMax < nMin)
        nMax = nMin;

    if (nMin == nMax)
        swprintf(wszOut, L"%d", nMin);
    else
        swprintf(wszOut, L"%d-%d", nMin, nMax);
}

// 1.12a: 0x6FAF3240
// Calling convention: __usercall (ECX = wszText, EDX = nColor, [esp+4] = nRow, [esp+8] = nCol)
void __fastcall sub_6FAF3240(wchar_t* wszText, int32_t nColor, int32_t nRow, int32_t nCol)
{
    const D2CharPagTextBoxStrc* pBox = &g_aCharPagTextBoxes_6FBA3980[nRow][nCol];
    int32_t nXLeft = pBox->nXLeft - g_nScreenOffsetX_6FBA79A0;
    int32_t nXRight = pBox->nXRight - g_nScreenOffsetX_6FBA79A0;
    int32_t nY = pBox->nYBase;

    wchar_t wszBuf[512];
    wcscpy(wszBuf, wszText);
    wchar_t* pNewline = wcschr(wszBuf, L'\n');
    if (pNewline)
    {
        *pNewline = L'\0';
        int32_t nWidth1 = D2WIN_GetTextWidth(wszBuf);
        D2WIN_DrawText(wszBuf, nXLeft + ((nXRight - nXLeft - nWidth1) >> 1), nY - 6, nColor, 0);
        int32_t nWidth2 = D2WIN_GetTextWidth(pNewline + 1);
        D2WIN_DrawText(pNewline + 1, nXLeft + ((nXRight - nXLeft - nWidth2) >> 1), nY + 6, nColor, 0);
    }
    else
    {
        int32_t nWidth = D2WIN_GetTextWidth(wszBuf);
        D2WIN_DrawText(wszBuf, nXLeft + ((nXRight - nXLeft - nWidth) >> 1), nY, nColor, 0);
    }
}

// 1.12a: 0x6FAF3300
// Calling convention: __usercall (EDX = nRow, ECX = nCol, [esp+4] = pUnit, [esp+8] = pSkill, [esp+12] = nColor)
void __fastcall sub_6FAF3300(int32_t nCol, int32_t nRow, D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nColor)
{
    int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (!pSkillDesc)
        return;

    uint16_t wStr = pSkillDesc->wStrAlt ? pSkillDesc->wStrAlt : pSkillDesc->wStrName;
    if (!wStr)
        return;

    const wchar_t* wszName = D2LANG_GetStringByTblIndex(wStr);
    const D2CharPagTextBoxStrc* pBox = &g_aCharPagTextBoxes_6FBA3980[nRow][nCol];
    int32_t nMaxWidth = pBox->nXRight - pBox->nXLeft;

    D2WIN_SetFont(6);
    if (wcschr(wszName, L'\n') || D2WIN_GetTextWidth(wszName) <= nMaxWidth)
    {
        sub_6FAF3240((wchar_t*)wszName, nColor, nRow, nCol);
    }
    else
    {
        wchar_t wszWrapped[512];
        wcscpy(wszWrapped, wszName);
        D2WIN_WordWrap(wszWrapped, nMaxWidth);
        sub_6FAF3240(wszWrapped, nColor, nRow, nCol);
    }
}

// 1.12a: 0x6FAF3410
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF3410(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor)
{
    *pAR = 0;
    *pColor = 0;
    if (!pUnit)
        return 0;

    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF2430(pWeapon, pUnit, nSkillId, nSkillLevel, pAR, pColor);
}

// 1.12a: 0x6FAF3450
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pSkillDesc, [esp+8] = nLineIdx, [esp+12] = wszOut)
void __fastcall sub_6FAF3450(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, int32_t nLineIdx, wchar_t* wszOut)
{
    int32_t nElemType = pSkillDesc->nPrgDamElem[nLineIdx];
    int32_t nMin = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwProgDmgMin[nLineIdx], nSkillId, nSkillLevel);
    int32_t nMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwProgDmgMax[nLineIdx], nSkillId, nSkillLevel);

    for (int32_t i = 0; i < 5; ++i)
    {
        if (g_aElemDamageStats_6FB85B68[i].nElemType == nElemType)
        {
            int32_t nPct = STATLIST_UnitGetStatValue(pUnit, g_aElemDamageStats_6FB85B68[i].nStatMinId, 0);
            if (nPct)
            {
                nMin += nMin * nPct / 100;
                nMax += nMax * nPct / 100;
            }
            break;
        }
    }

    sub_6FAF2A10(nMin, nMax, wszOut, pSkillDesc->wDescTextA[nLineIdx], pSkillDesc->wDescTextB[nLineIdx]);
}

// 1.12a: 0x6FAF3500
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pSkillDesc, [esp+8] = nLineIdx, [esp+12] = wszOut)
void __fastcall sub_6FAF3500(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, int32_t nLineIdx, wchar_t* wszOut)
{
    int32_t nElemType = pSkillDesc->nPrgDamElem[nLineIdx];
    int32_t nMin = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwProgDmgMin[nLineIdx], nSkillId, nSkillLevel) * 25;
    int32_t nMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwProgDmgMax[nLineIdx], nSkillId, nSkillLevel) * 25;

    for (int32_t i = 0; i < 5; ++i)
    {
        if (g_aElemDamageStats_6FB85B68[i].nElemType == nElemType)
        {
            int32_t nPct = STATLIST_UnitGetStatValue(pUnit, g_aElemDamageStats_6FB85B68[i].nStatMinId, 0);
            if (nPct)
            {
                nMin += nMin * nPct / 100;
                nMax += nMax * nPct / 100;
            }
            break;
        }
    }

    sub_6FAF2A10(nMin, nMax, wszOut, pSkillDesc->wDescTextA[nLineIdx], pSkillDesc->wDescTextB[nLineIdx]);
}

// 1.12a: 0x6FAF35C0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF35C0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor)
{
    *pAR = 0;
    *pColor = 0;
    if (!pUnit)
        return 0;

    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF2210(pWeapon, pUnit, nSkillId, nSkillLevel, pAR, pColor);
}

// 1.12a: 0x6FAF3600
// Calling convention: __fastcall (ECX = pUnit, EDX = pSkill, [esp+4] = pAR, [esp+8] = pColor)
int32_t __fastcall sub_6FAF3600(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t* pAR, int32_t* pColor)
{
    *pAR = 0;
    *pColor = 0;
    if (!pUnit || !pSkill)
        return 0;

    int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (!pSkillDesc || pSkillDesc->wDescAtt == 0 || pSkillDesc->wDescAtt >= 18)
        return sub_6FAF35C0(pUnit, nSkillId, nSkillLevel, pAR, pColor);

    CharPagARCallback_t pfnAR = g_apfnCharPagAR_6FBA3938[pSkillDesc->wDescAtt];
    if (!pfnAR)
        return 0;
    return pfnAR(pUnit, nSkillId, nSkillLevel, pAR, pColor);
}

// 1.12a: 0x6FAF3690
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pSkillDesc, [esp+8] = nLineIdx, [esp+12] = wszOut)
void __fastcall sub_6FAF3690(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, int32_t nLineIdx, wchar_t* wszOut)
{
    int32_t nElemType = pSkillDesc->nPrgDamElem[nLineIdx];
    int32_t nMin = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwProgDmgMin[nLineIdx], nSkillId, nSkillLevel) / 3;
    int32_t nMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwProgDmgMax[nLineIdx], nSkillId, nSkillLevel) / 3;

    for (int32_t i = 0; i < 5; ++i)
    {
        if (g_aElemDamageStats_6FB85B68[i].nElemType == nElemType)
        {
            int32_t nPct = STATLIST_UnitGetStatValue(pUnit, g_aElemDamageStats_6FB85B68[i].nStatMinId, 0);
            if (nPct)
            {
                nMin += nMin * nPct / 100;
                nMax += nMax * nPct / 100;
            }
            break;
        }
    }

    sub_6FAF2BF0(nMin, nMax, wszOut, pSkillDesc->wDescTextA[nLineIdx], pSkillDesc->wDescTextB[nLineIdx]);
}

// 1.12a: 0x6FAF3750
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF3750(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    sub_6FAF2AB0(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF3790
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF3790(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    sub_6FAF2A10(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF37D0
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF37D0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
    sub_6FAF2A10(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF3820
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF3820(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
    sub_6FAF2B50(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF3870
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF3870(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1) * 25 / 2;
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1) * 25 / 2;
    sub_6FAF2B50(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF38D0
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF38D0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
    sub_6FAF2BF0(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF3920
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF3920(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1) * 25 / 3;
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1) * 25 / 3;
    sub_6FAF2A10(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF3980
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut, [esp+20] = dwCalcA, [esp+24] = dwCalcB)
void __fastcall sub_6FAF3980(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    if (dwCalcA)
    {
        int32_t nMult = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nMult)
        {
            nMin *= nMult;
            nMax *= nMult;
        }
    }
    if (dwCalcB)
    {
        int32_t nDiv = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel);
        if (nDiv)
        {
            nMin /= nDiv;
            nMax /= nDiv;
        }
    }
    sub_6FAF2BF0(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF3A30
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStr, [esp+12] = wszOut)
void __fastcall sub_6FAF3A30(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    sub_6FAF2CE0(nMin, nMax, wStr, wszOut);
}

// 1.12a: 0x6FAF3A70
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStr, [esp+12] = wszOut)
void __fastcall sub_6FAF3A70(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    sub_6FAF2C90(nMin, nMax, wStr, wszOut);
}

// 1.12a: 0x6FAF3AB0
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStr, [esp+12] = wszOut)
void __fastcall sub_6FAF3AB0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
    sub_6FAF2D30(nMin, nMax, wStr, wszOut);
}

// 1.12a: 0x6FAF3B00
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStr, [esp+12] = wszOut)
void __fastcall sub_6FAF3B00(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
    sub_6FAF2D80(nMin, nMax, wStr, wszOut);
}

// 1.12a: 0x6FAF3B50
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut, [esp+20] = dwCalcA, [esp+24] = dwCalcB)
void __fastcall sub_6FAF3B50(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    if (dwCalcA)
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (dwCalcB)
    {
        int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel) << 8;
        nMin += nBonus;
        nMax += nBonus;
    }
    sub_6FAF2AB0(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF3C20
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut, [esp+20] = dwCalcA, [esp+24] = dwCalcB)
void __fastcall sub_6FAF3C20(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    if (dwCalcA)
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (dwCalcB)
    {
        int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel) << 8;
        nMin += nBonus;
        nMax += nBonus;
    }
    sub_6FAF2A10(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF3CF0
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut, [esp+20] = dwCalcA, [esp+24] = dwCalcB)
void __fastcall sub_6FAF3CF0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nLen = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);
    if (dwCalcA)
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (dwCalcB)
    {
        int32_t nLenPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel);
        if (nLenPct)
            nLen += nLen * nLenPct / 100;
    }
    if (nLen > 0)
    {
        nMin *= nLen;
        nMax *= nLen;
    }
    else
    {
        nMin *= 25;
        nMax *= 25;
        nLen = 25;
    }

    wchar_t wszRange[32];
    if (nMin < 256 || nMax < 256)
        sub_6FAF2660(nMin, nMax, wszRange);
    else
        sub_6FAF2720((nMin + 128) & ~0xFF, (nMax + 128) & ~0xFF, wszRange);

    if (wStrB)
    {
        wchar_t wszTemp[256];
        sub_6FB202B0(D2LANG_GetStringByTblIndex(wStrB), wszTemp, nLen / 25);
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(wStrA), wszRange, wszTemp);
    }
    else
    {
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszOut, wszRange);
    }
}

// 1.12a: 0x6FAF3EC0
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut, [esp+20] = dwCalcA, [esp+24] = dwCalcB)
void __fastcall sub_6FAF3EC0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nLen = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);
    if (dwCalcA)
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (dwCalcB)
    {
        int32_t nLenPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel);
        if (nLenPct)
            nLen += nLen * nLenPct / 100;
    }
    if (nLen > 0)
    {
        nMin *= nLen;
        nMax *= nLen;
    }
    else
    {
        nMin *= 25;
        nMax *= 25;
        nLen = 25;
    }

    wchar_t wszRange[32];
    if (nMin < 256 || nMax < 256)
        sub_6FAF26C0(nMin, nMax, wszRange);
    else
        sub_6FAF2760((nMin + 128) & ~0xFF, (nMax + 128) & ~0xFF, wszRange);

    if (wStrB)
    {
        wchar_t wszTemp[256];
        if (nLen % 25)
        {
            wchar_t wszSec[32];
            swprintf(wszSec, L"%d.%d", nLen / 25, ((nLen % 25) * 10) / 25);
            sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrB), wszTemp, wszSec);
        }
        else
        {
            sub_6FB202B0(D2LANG_GetStringByTblIndex(wStrB), wszTemp, nLen / 25);
        }
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(wStrA), wszRange, wszTemp);
    }
    else
    {
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszOut, wszRange);
    }
}

// 1.12a: 0x6FAF40E0
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut, [esp+20] = dwCalcA, [esp+24] = dwCalcB)
void __fastcall sub_6FAF40E0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nLen = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);
    if (dwCalcA)
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (dwCalcB)
    {
        int32_t nLenPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel);
        if (nLenPct)
            nLen += nLen * nLenPct / 100;
    }
    nMin *= 25;
    nMax *= 25;
    if (nLen <= 0)
        nLen = 25;

    wchar_t wszRange[32];
    if (nMin < 256 || nMax < 256)
        sub_6FAF26C0(nMin, nMax, wszRange);
    else
        sub_6FAF2760((nMin + 128) & ~0xFF, (nMax + 128) & ~0xFF, wszRange);

    if (wStrB)
    {
        wchar_t wszTemp[256];
        sub_6FB202B0(D2LANG_GetStringByTblIndex(wStrB), wszTemp, nLen / 25);
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(wStrA), wszRange, wszTemp);
    }
    else
    {
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszOut, wszRange);
    }
}

// 1.12a: 0x6FAF4290
// Calling convention: __fastcall (ECX = pUnit, EDX = pSkill, [esp+4] = pMinDmg, [esp+8] = pMaxDmg, [esp+12] = pColor)
int32_t __fastcall sub_6FAF4290(D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor)
{
    *pMinDmg = 0;
    *pMaxDmg = 0;
    *pColor = 0;
    if (!pUnit || !pSkill)
        return 0;

    int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (!pSkillDesc || pSkillDesc->wDescDam >= 36)
        return 0;

    CharPagDmgCallback_t pfnDmg = g_apfnCharPagDmg_6FBA38A8[pSkillDesc->wDescDam];
    if (!pfnDmg)
        return 0;

    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    int32_t nElemType = pSkillsTxt ? pSkillsTxt[0x17C] : 0;
    return pfnDmg(pUnit, nSkillId, nSkillLevel, pMinDmg, pMaxDmg, pColor, nElemType);
}

// 1.12a: 0x6FAF4330
// Calling convention: __usercall (EAX = wMissileId, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF4330(int32_t nSkillId, int32_t nSkillLevel, uint16_t wMissileId, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt && *(int16_t*)(pSkillsTxt + 0x24) == wMissileId)
    {
        nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    else
    {
        nMin = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMinElemDamage(0, pUnit, wMissileId, nSkillLevel);
        nMax = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMaxElemDamage(0, pUnit, wMissileId, nSkillLevel);
    }
    sub_6FAF2A10(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF43E0
// Calling convention: __usercall (EAX = wMissileId, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF43E0(int32_t nSkillId, int32_t nSkillLevel, uint16_t wMissileId, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt && *(int16_t*)(pSkillsTxt + 0x24) == wMissileId)
    {
        nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    else
    {
        nMin = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMinElemDamage(0, pUnit, wMissileId, nSkillLevel);
        nMax = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMaxElemDamage(0, pUnit, wMissileId, nSkillLevel);
    }
    sub_6FAF2A10(nMin * 25, nMax * 25, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF44A0
// Calling convention: __usercall (EAX = wMissileId, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF44A0(int32_t nSkillId, int32_t nSkillLevel, uint16_t wMissileId, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt && *(int16_t*)(pSkillsTxt + 0x24) == wMissileId)
    {
        nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    else
    {
        nMin = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMinElemDamage(0, pUnit, wMissileId, nSkillLevel);
        nMax = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMaxElemDamage(0, pUnit, wMissileId, nSkillLevel);
    }
    sub_6FAF2A10(nMin * 25 / 3, nMax * 25 / 3, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF4560
// Calling convention: __usercall (EAX = wMissileId, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF4560(int32_t nSkillId, int32_t nSkillLevel, uint16_t wMissileId, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt && *(int16_t*)(pSkillsTxt + 0x24) == wMissileId)
    {
        nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    else
    {
        nMin = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMinElemDamage(0, pUnit, wMissileId, nSkillLevel);
        nMax = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMaxElemDamage(0, pUnit, wMissileId, nSkillLevel);
    }
    sub_6FAF2BF0(nMin * 25 / 3, nMax * 25 / 3, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF4620
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut)
void __fastcall sub_6FAF4620(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    sub_6FAF2A10(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF4680
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut)
void __fastcall sub_6FAF4680(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    sub_6FAF2AB0(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF46E0
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut)
void __fastcall sub_6FAF46E0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
        nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1) * 25;
    }
    sub_6FAF2A10(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF4750
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut, [esp+16] = dwCalcA, [esp+20] = dwCalcB)
void __fastcall sub_6FAF4750(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    if (dwCalcA)
    {
        int32_t nMult = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nMult)
        {
            nMin *= nMult;
            nMax *= nMult;
        }
    }
    if (dwCalcB)
    {
        int32_t nDiv = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel);
        if (nDiv)
        {
            nMin /= nDiv;
            nMax /= nDiv;
        }
    }
    sub_6FAF2A10(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF4810
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut, [esp+16] = dwCalcA, [esp+20] = dwCalcB)
void __fastcall sub_6FAF4810(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    if (dwCalcA)
    {
        int32_t nMult = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nMult)
        {
            nMin *= nMult;
            nMax *= nMult;
        }
    }
    if (dwCalcB)
    {
        int32_t nDiv = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel);
        if (nDiv)
        {
            nMin /= nDiv;
            nMax /= nDiv;
        }
    }
    sub_6FAF2BF0(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF48D0
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut, [esp+16] = dwCalcA, [esp+20] = dwCalcB)
void __fastcall sub_6FAF48D0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    if (dwCalcA)
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (dwCalcB)
    {
        int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel) << 8;
        nMin += nBonus;
        nMax += nBonus;
    }
    sub_6FAF2A10(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF49B0
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut, [esp+16] = dwCalcA, [esp+20] = dwCalcB)
void __fastcall sub_6FAF49B0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    if (dwCalcA)
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (dwCalcB)
    {
        int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel) << 8;
        nMin += nBonus;
        nMax += nBonus;
    }
    sub_6FAF2A10(nMin * 25 / 3, nMax * 25 / 3, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF4AA0
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut, [esp+16] = dwCalcA, [esp+20] = dwCalcB)
void __fastcall sub_6FAF4AA0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    if (dwCalcA)
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (dwCalcB)
    {
        int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel) << 8;
        nMin += nBonus;
        nMax += nBonus;
    }
    sub_6FAF2BF0(nMin * 25 / 3, nMax * 25 / 3, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF4B90
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStr, [esp+8] = wszOut)
void __fastcall sub_6FAF4B90(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    sub_6FAF2C90(nMin, nMax, wStr, wszOut);
}

// 1.12a: 0x6FAF4BF0
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStr, [esp+8] = wszOut)
void __fastcall sub_6FAF4BF0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStr, wchar_t* wszOut)
{
    int32_t nMin = 0, nMax = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
        nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
    }
    sub_6FAF2CE0(nMin, nMax, wStr, wszOut);
}

// 1.12a: 0x6FAF4C50
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut, [esp+16] = dwCalcA, [esp+20] = dwCalcB)
void __fastcall sub_6FAF4C50(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1) * 3 / 4
                 + SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1) * 3 / 4
                 + SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    if (dwCalcA)
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (dwCalcB)
    {
        int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel) << 8;
        nMin += nBonus;
        nMax += nBonus;
    }
    sub_6FAF2AB0(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF4D70
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut)
void __fastcall sub_6FAF4D70(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nMin = 0, nMax = 0, nColor = 0;
    sub_6FAFB1B0(pUnit, nSkillId, nSkillLevel, &nMin, &nMax, &nColor, 0);
    sub_6FAF2A10(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF4DC0
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut, [esp+16] = dwCalcA, [esp+20] = dwCalcB)
void __fastcall sub_6FAF4DC0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = 0, nMax = 0, nColor = 0;
    sub_6FAFB080(pUnit, nSkillId, nSkillLevel, &nMin, &nMax, &nColor, 0);
    if (dwCalcA)
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (dwCalcB)
    {
        int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel) << 8;
        nMin += nBonus;
        nMax += nBonus;
    }
    sub_6FAF2A10(nMin, nMax, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF4EA0
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut)
void __fastcall sub_6FAF4EA0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    D2MonStatInitStrc monStatInit = {};
    int32_t nHp = 0;
    if (!sub_6FAF2E30(nSkillId, pUnit, nSkillLevel, &monStatInit, &nHp))
        return;

    int32_t nBonusPct = 0;
    int32_t nSkillMasteryLvl = SKILLS_GetSkillLevel(pUnit, SKILLS_GetSkillById(pUnit, 70, -1), 1);
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        int32_t nBaseBonus = *(int32_t*)(pSkillsTxt + 0x16C);
        if (nBaseBonus > 0 && nSkillLevel > 1)
            nBonusPct += (nSkillLevel - 1) * nBaseBonus;
    }
    if (nSkillMasteryLvl > 0)
    {
        uint8_t* pMasteryTxt = (uint8_t*)sub_6FAFF7E0(70);
        if (pMasteryTxt)
            nBonusPct += SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pMasteryTxt + 0xA0), 70, nSkillMasteryLvl);
    }

    int32_t nMin = monStatInit.nA1MinD;
    int32_t nMax = monStatInit.nA1MaxD;
    if (nBonusPct > 0)
    {
        nMin += nMin * nBonusPct / 100;
        nMax += nMax * nBonusPct / 100;
    }
    sub_6FAF2A10(nMin << 8, nMax << 8, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF4FE0
// Calling convention: __usercall (EDX = nRow, ECX = pUnit, [esp+4] = pSkill)
void __fastcall sub_6FAF4FE0(D2UnitStrc* pUnit, int32_t nRow, D2SkillStrc* pSkill)
{
    int32_t nAR = 0, nColor = 0;
    if (sub_6FAF3600(pUnit, pSkill, &nAR, &nColor))
    {
        sub_6FAF3300(0, nRow, pUnit, pSkill, 0);
        D2WIN_SetFont(1);
        wchar_t wszAR[64];
        swprintf(wszAR, L"%d", nAR);
        sub_6FAF3240(wszAR, nColor, nRow, 2);
    }
    else
    {
        sub_6FAF3300(1, nRow, pUnit, pSkill, 0);
    }
}

// 1.12a: 0x6FAF5050
// Calling convention: __usercall (EDX = pUnit, ECX = nSkillId, [esp+4] = nSkillLevel, [esp+8] = pWeapon, [esp+12] = pElemBonus, [esp+16] = pMinElem, [esp+20] = pMaxElem, [esp+24] = pElemType, [esp+28] = nOverrideElemType)
void __fastcall sub_6FAF5050(int32_t nSkillId, D2UnitStrc* pUnit, int32_t nSkillLevel, D2UnitStrc* pWeapon, int32_t* pElemBonus, int32_t* pMinElem, int32_t* pMaxElem, int32_t* pElemType, int32_t nOverrideElemType)
{
    D2StatListStrc* pSavedStatList = nullptr;
    sub_6FAF20D0(pUnit, &pSavedStatList, pWeapon);

    *pElemBonus = 0;
    *pMinElem = 0;
    *pMaxElem = 0;
    if (pElemType)
        *pElemType = 0;

    if (pUnit && nSkillId >= 0 && nSkillLevel > 0)
    {
        uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
        if (pSkillsTxt)
        {
            *pMinElem = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
            *pMaxElem = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
            if (*pMaxElem < *pMinElem)
                *pMaxElem = *pMinElem;
            if (pElemType)
                *pElemType = pSkillsTxt[0x17C];
        }
    }

    int32_t nElemMin = 0, nElemMax = 0;
    sub_6FAF58C0(pUnit, &nElemMin, &nElemMax, pElemType, nOverrideElemType);
    *pMinElem += nElemMin;
    *pMaxElem += nElemMax;

    int32_t nEnchMin = 0, nEnchMax = 0;
    sub_6FAF5A90(pUnit, &nEnchMin, &nEnchMax, pElemType, nOverrideElemType);
    *pMinElem += nEnchMin;
    *pMaxElem += nEnchMax;
    *pElemBonus = nEnchMax;

    sub_6FAF2170(pSavedStatList, pUnit, pWeapon);
}

// 1.12a: 0x6FAF51A0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF51A0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor)
{
    *pAR = 0;
    *pColor = 0;
    if (!pUnit || !UNITS_CanDualWield(pUnit))
        return 0;

    int32_t nAR1 = 0, nAR2 = 0;
    D2InventoryStrc* pInventory = *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60);
    sub_6FAF2210(INVENTORY_GetLeftHandWeapon(pInventory), pUnit, nSkillId, nSkillLevel, &nAR1, pColor);
    sub_6FAF2210(INVENTORY_GetSecondaryWeapon(pInventory), pUnit, nSkillId, nSkillLevel, &nAR2, pColor);
    *pAR = (nAR1 + nAR2) / 2;
    return 1;
}

// 1.12a: 0x6FAF51F0
// Calling convention: __fastcall (ECX = pUnit, EDX = pAR, [esp+4] = pColor)
int32_t __fastcall sub_6FAF51F0(D2UnitStrc* pUnit, int32_t* pAR, int32_t* pColor)
{
    if (!pUnit || !UNITS_CanDualWield(pUnit))
        return 0;

    D2UnitStrc* pSecondaryWeapon = INVENTORY_GetSecondaryWeapon(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF2210(pSecondaryWeapon, pUnit, 0, 1, pAR, pColor);
}

// 1.12a: 0x6FAF5220
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut)
void __fastcall sub_6FAF5220(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    int32_t nSummonId = SKILLS_GetProgressiveSkillId(pUnit, nSkillId);
    if (nSummonId < 0)
        return;

    int32_t nMonLevel = 0;
    if (!SKILLS_CalculateGetLeftHandSwing(pUnit, nSkillId, nSkillLevel, &nMonLevel))
        return;

    D2MonStatInitStrc monStatInit = {};
    if (!sub_6FAF2DD0(nSummonId, nMonLevel, &monStatInit))
        return;

    int32_t nMin = monStatInit.nA1MinD;
    int32_t nMax = monStatInit.nA1MaxD;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        for (int32_t i = 0; i < 5; ++i)
        {
            if (*(int16_t*)(pSkillsTxt + 0x92 + i * 2) == 0x19)
            {
                int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x9C + i * 4), nSkillId, nSkillLevel);
                nMin += nMin * nPct / 100;
                nMax += nMax * nPct / 100;
            }
        }
    }
    sub_6FAF2A10(nMin << 8, nMax << 8, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF5330
// Calling convention: __usercall (EAX = nSkillId, ECX = pUnit, [esp+4] = nSkillLevel, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = wszOut, [esp+20] = dwCalcA, [esp+24] = dwCalcB)
void __fastcall sub_6FAF5330(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nBaseMin = STATLIST_UnitGetStatValue(pUnit, 0x17, 0);
    int32_t nBaseMax = STATLIST_UnitGetStatValue(pUnit, 0x18, 0);
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
    {
        nBaseMin += SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 0) >> 8;
        nBaseMax += SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 0) >> 8;
    }

    D2UnitStrc* pShield = nullptr;
    if (pUnit && *(uint32_t*)pUnit == 0)
    {
        INVENTORY_GetEquippedShield(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60), &pShield);
        if (pShield)
        {
            uint8_t* pItemTxt = (uint8_t*)DATATBLS_GetItemsTxtRecord(*(uint32_t*)((uint8_t*)pShield + 4));
            if (pItemTxt)
            {
                nBaseMin += pItemTxt[0x132];
                nBaseMax += pItemTxt[0x133];
            }
        }
    }

    int32_t nBonusPct = 0;
    if (dwCalcA)
        nBonusPct += SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel);
    if (dwCalcB)
        nBonusPct += SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel);
    if (STATES_CheckState(pUnit, 0x95))
    {
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, 0x95);
        if (pStatList)
            nBonusPct += STATLIST_GetStatValue(pStatList, 0x19, 0);
    }
    nBonusPct += STATLIST_UnitGetStatValue(pUnit, 0, 0);

    int32_t nMin = nBaseMin * (nBonusPct + 100) / 100;
    int32_t nMax = nBaseMax * (nBonusPct + 100) / 100;
    if (nMin < 1)
        nMin = 1;
    if (nMax <= nMin)
        nMax = nMin + 1;
    sub_6FAF2A10(nMin << 8, nMax << 8, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF54E0
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = wszOut, [esp+16] = dwCalcA, [esp+20] = dwCalcB)
void __fastcall sub_6FAF54E0(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut, uint32_t dwCalcA, uint32_t dwCalcB)
{
    int32_t nMin = 0, nMax = 0;
    sub_6FAFA620(nSkillId, nSkillLevel, pUnit, &nMin, &nMax);
    sub_6FAF2A10(nMin << 8, nMax << 8, wszOut, wStrA, wStrB);
}

// 1.12a: 0x6FAF5530
// Calling convention: __usercall (EDX = nSkillLevel, ECX = pSkillDesc, [esp+4] = pUnit, [esp+8] = nLineIdx, [esp+12] = wszOut)
void __fastcall sub_6FAF5530(D2SkillDescTxt* pSkillDesc, int32_t nSkillLevel, D2UnitStrc* pUnit, int32_t nLineIdx, wchar_t* wszOut)
{
    uint16_t wMissileId = pSkillDesc->wDescMissile[nLineIdx];
    int32_t nMin = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMinElemDamage(0, pUnit, wMissileId, nSkillLevel);
    int32_t nMax = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMaxElemDamage(0, pUnit, wMissileId, nSkillLevel);
    int32_t nLen = MISSILE_GetElemDamageLength(0, pUnit, wMissileId, nSkillLevel);
    if (nLen > 0)
    {
        nMin *= nLen;
        nMax *= nLen;
    }

    wchar_t wszRange[32];
    sub_6FAF2760(nMin, nMax, wszRange);
    if (pSkillDesc->wDescTextB[nLineIdx])
    {
        wchar_t wszTemp[256];
        sub_6FB202B0(D2LANG_GetStringByTblIndex(pSkillDesc->wDescTextB[nLineIdx]), wszTemp, nLen / 25);
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(pSkillDesc->wDescTextA[nLineIdx]), wszRange, wszTemp);
    }
    else
    {
        sub_6FB202E0(D2LANG_GetStringByTblIndex(pSkillDesc->wDescTextA[nLineIdx]), wszOut, wszRange);
    }
}

// 1.12a: 0x6FAF5690
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pSkillDesc, [esp+8] = nLineIdx, [esp+12] = wszOut)
void __fastcall sub_6FAF5690(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, int32_t nLineIdx, wchar_t* wszOut)
{
    uint16_t wMissileId = pSkillDesc->wDescMissile[nLineIdx];
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt && *(int16_t*)(pSkillsTxt + 0x24) == wMissileId)
    {
        sub_6FAF3EC0(nSkillId, nSkillLevel, pUnit, pSkillDesc->wDescTextA[nLineIdx], pSkillDesc->wDescTextB[nLineIdx], wszOut, pSkillDesc->dwDescCalcA[nLineIdx], pSkillDesc->dwDescCalcB[nLineIdx]);
        return;
    }

    int32_t nMin = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMinElemDamage(0, pUnit, wMissileId, nSkillLevel);
    int32_t nMax = MISSILE_CalculateMasteryBonus(wMissileId, pUnit, nSkillLevel) + MISSILE_GetMaxElemDamage(0, pUnit, wMissileId, nSkillLevel);
    int32_t nLen = MISSILE_GetElemDamageLength(0, pUnit, wMissileId, nSkillLevel);
    if (pSkillDesc->dwDescCalcA[nLineIdx])
    {
        int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDescCalcA[nLineIdx], nSkillId, nSkillLevel);
        if (nPct)
        {
            nMin += nMin * nPct / 100;
            nMax += nMax * nPct / 100;
        }
    }
    if (pSkillDesc->dwDescCalcB[nLineIdx])
    {
        int32_t nLenPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDescCalcB[nLineIdx], nSkillId, nSkillLevel);
        if (nLenPct)
            nLen += nLen * nLenPct / 100;
    }
    if (nLen > 0)
    {
        nMin *= nLen;
        nMax *= nLen;
    }
    else
    {
        nMin *= 25;
        nMax *= 25;
        nLen = 25;
    }

    wchar_t wszRange[32];
    if (nMin < 256 || nMax < 256)
        sub_6FAF26C0(nMin, nMax, wszRange);
    else
        sub_6FAF2760((nMin + 128) & ~0xFF, (nMax + 128) & ~0xFF, wszRange);

    if (pSkillDesc->wDescTextB[nLineIdx])
    {
        wchar_t wszTemp[256];
        sub_6FB202B0(D2LANG_GetStringByTblIndex(pSkillDesc->wDescTextB[nLineIdx]), wszTemp, nLen / 25);
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(pSkillDesc->wDescTextA[nLineIdx]), wszRange, wszTemp);
    }
    else
    {
        sub_6FB202E0(D2LANG_GetStringByTblIndex(pSkillDesc->wDescTextA[nLineIdx]), wszOut, wszRange);
    }
}

// 1.12a: 0x6FAF58C0
// Calling convention: __usercall (EDX = pUnit, ECX = pMinOut, [esp+4] = pMaxOut, [esp+8] = pElemTypeOut, [esp+12] = nOverrideElemType)
void __fastcall sub_6FAF58C0(D2UnitStrc* pUnit, int32_t* pMinOut, int32_t* pMaxOut, int32_t* pElemTypeOut, int32_t nOverrideElemType)
{
    int32_t nTotalMin = 0;
    int32_t nTotalMax = 0;
    int32_t nDominantElem = 0;
    int32_t nDominantMax = 0;

    if (nOverrideElemType > 0)
    {
        nDominantElem = nOverrideElemType;
        nDominantMax = 0x7FFFFFFF;
    }
    else if (pElemTypeOut && *pElemTypeOut > 0)
    {
        nDominantElem = *pElemTypeOut;
        nDominantMax = *pMaxOut;
    }

    int32_t nFireMin = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 48, 0) << 8;
    int32_t nFireMax = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 49, 0) << 8;
    if (nFireMax > 0)
    {
        int32_t nPct = STATLIST_UnitGetStatValue(pUnit, 329, 0);
        if (nPct > 0)
        {
            nFireMin += nFireMin * nPct / 100;
            nFireMax += nFireMax * nPct / 100;
        }
        nTotalMin += nFireMin;
        nTotalMax += nFireMax;
        if (nFireMax > nDominantMax)
        {
            nDominantElem = 1;
            nDominantMax = nFireMax;
        }
    }

    int32_t nLtngMin = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 50, 0) << 8;
    int32_t nLtngMax = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 51, 0) << 8;
    if (nLtngMax > 0)
    {
        int32_t nPct = STATLIST_UnitGetStatValue(pUnit, 330, 0);
        if (nPct > 0)
        {
            nLtngMin += nLtngMin * nPct / 100;
            nLtngMax += nLtngMax * nPct / 100;
        }
        nTotalMin += nLtngMin;
        nTotalMax += nLtngMax;
        if (nLtngMax > nDominantMax)
        {
            nDominantElem = 2;
            nDominantMax = nLtngMax;
        }
    }

    int32_t nMagMin = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 52, 0) << 8;
    int32_t nMagMax = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 53, 0) << 8;
    if (nMagMax > 0)
    {
        nTotalMin += nMagMin;
        nTotalMax += nMagMax;
        if (nMagMax > nDominantMax)
        {
            nDominantElem = 3;
            nDominantMax = nMagMax;
        }
    }

    int32_t nColdMin = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 54, 0) << 8;
    int32_t nColdMax = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 55, 0) << 8;
    if (nColdMax > 0)
    {
        int32_t nPct = STATLIST_UnitGetStatValue(pUnit, 331, 0);
        if (nPct > 0)
        {
            nColdMin += nColdMin * nPct / 100;
            nColdMax += nColdMax * nPct / 100;
        }
        nTotalMin += nColdMin;
        nTotalMax += nColdMax;
        if (nColdMax > nDominantMax)
        {
            nDominantElem = 4;
            nDominantMax = nColdMax;
        }
    }

    int32_t nPoisMin = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 57, 0);
    int32_t nPoisMax = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 58, 0);
    int32_t nPoisLen = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 59, 0);
    int32_t nPoisCount = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, 326, 0);
    if (nPoisMax > 0 && nPoisLen > 0)
    {
        int32_t nPct = STATLIST_UnitGetStatValue(pUnit, 332, 0);
        if (nPct > 0)
        {
            nPoisMin += nPoisMin * nPct / 100;
            nPoisMax += nPoisMax * nPct / 100;
        }
        if (nPoisCount > 1)
            nPoisLen /= nPoisCount;
        int32_t nPoisTotalMin = (nPoisMin * nPoisLen + 128) >> 8 << 8;
        int32_t nPoisTotalMax = (nPoisMax * nPoisLen + 128) >> 8 << 8;
        nTotalMin += nPoisTotalMin;
        nTotalMax += nPoisTotalMax;
        if (nPoisTotalMax > nDominantMax)
        {
            nDominantElem = 5;
            nDominantMax = nPoisTotalMax;
        }
    }

    *pMinOut = nTotalMin;
    *pMaxOut = nTotalMax;
    if (pElemTypeOut)
        *pElemTypeOut = nDominantElem;
}

// 1.12a: 0x6FAF5A90
// Calling convention: __usercall (EAX = pUnit, ECX = pMinOut, [esp+4] = pMaxOut, [esp+8] = pElemTypeOut, [esp+12] = nOverrideElemType)
void __fastcall sub_6FAF5A90(D2UnitStrc* pUnit, int32_t* pMinOut, int32_t* pMaxOut, int32_t* pElemTypeOut, int32_t nOverrideElemType)
{
    *pMinOut = 0;
    *pMaxOut = 0;
    if (!pUnit)
        return;

    int32_t nDominantElem = 0;
    int32_t nDominantMax = 0;
    if (nOverrideElemType > 0)
    {
        nDominantElem = nOverrideElemType;
        nDominantMax = 0x7FFFFFFF;
    }
    else if (pElemTypeOut && *pElemTypeOut > 0)
    {
        nDominantElem = *pElemTypeOut;
        nDominantMax = *pMaxOut;
    }

    int32_t nStateCount = DATATBLS_GetStatesTxtRecordCount();
    for (int32_t nStateId = 0; nStateId < nStateCount; ++nStateId)
    {
        if (STATES_CheckState(pUnit, nStateId))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nStateId);
            if (pStatList)
            {
                for (int32_t i = 1; i < 5; ++i)
                {
                    int32_t nMin = STATLIST_GetStatValue(pStatList, g_aElemDamageStats_6FB85B68[i].nStatMinId, 0) << 8;
                    int32_t nMax = STATLIST_GetStatValue(pStatList, g_aElemDamageStats_6FB85B68[i].nStatMaxId, 0) << 8;
                    if (nMax > 0)
                    {
                        if (nMin > nMax)
                            nMax = nMin;
                        int32_t nPct = STATLIST_UnitGetStatValue(pUnit, g_aElemDamageStats_6FB85B68[i].nElemType, 0);
                        if (nPct)
                        {
                            nMin += nMin * nPct / 100;
                            nMax += nMax * nPct / 100;
                        }
                        *pMinOut += nMin;
                        *pMaxOut += nMax;
                        if (nMax > nDominantMax)
                        {
                            nDominantElem = g_aElemDamageStats_6FB85B68[i].nElemType;
                            nDominantMax = nMax;
                        }
                    }
                }

                int32_t nPoisMin = STATLIST_GetStatValue(pStatList, 57, 0);
                int32_t nPoisMax = STATLIST_GetStatValue(pStatList, 58, 0);
                int32_t nPoisLen = STATLIST_GetStatValue(pStatList, 59, 0);
                if (nPoisMax > 0 && nPoisLen > 0)
                {
                    if (nPoisMin > nPoisMax)
                        nPoisMax = nPoisMin;
                    int32_t nPct = STATLIST_UnitGetStatValue(pUnit, 332, 0);
                    if (nPct)
                    {
                        nPoisMin += nPoisMin * nPct / 100;
                        nPoisMax += nPoisMax * nPct / 100;
                    }
                    int32_t nPoisTotalMin = (nPoisMin * nPoisLen) & ~0xFF;
                    int32_t nPoisTotalMax = (nPoisMax * nPoisLen) & ~0xFF;
                    *pMinOut += nPoisTotalMin;
                    *pMaxOut += nPoisTotalMax;
                    if (nPoisTotalMax > nDominantMax)
                    {
                        nDominantElem = 5;
                        nDominantMax = nPoisTotalMax;
                    }
                }
            }
        }
    }

    if (pElemTypeOut)
        *pElemTypeOut = nDominantElem;
}

// 1.12a: 0x6FAF5BD0
// Calling convention: __stdcall
uint16_t __stdcall sub_6FAF5BD0(int32_t nSkillId)
{
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    return pSkillDesc ? pSkillDesc->wStrName : 0;
}

// 1.12a: 0x6FAF5BF0
// Calling convention: __stdcall
uint16_t __stdcall sub_6FAF5BF0(int32_t nSkillId)
{
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    return pSkillDesc ? pSkillDesc->wStrShort : 0;
}

// 1.12a: 0x6FAF5C10
// Calling convention: __stdcall
uint16_t __stdcall sub_6FAF5C10(int32_t nSkillId)
{
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    return pSkillDesc ? pSkillDesc->wStrLong : 0;
}

// 1.12a: 0x6FAF5C30
// Calling convention: __stdcall
uint16_t __stdcall sub_6FAF5C30(int32_t nSkillId)
{
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    return pSkillDesc ? pSkillDesc->wStrAlt : 0;
}

// 1.12a: 0x6FAF5C50
// Calling convention: __stdcall
uint16_t __stdcall sub_6FAF5C50(int32_t nSkillId)
{
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    return pSkillDesc ? pSkillDesc->wStrMana : 0;
}

// 1.12a: 0x6FAF5C70
// Calling convention: __usercall (EDX = nRow, ECX = pUnit, [esp+4] = pSkill)
void __fastcall sub_6FAF5C70(D2UnitStrc* pUnit, int32_t nRow, D2SkillStrc* pSkill)
{
    int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (!pSkillDesc)
        return;

    int32_t nMin = 0, nMax = 0, nColor = 0;
    int32_t nResult = sub_6FAF4290(pUnit, pSkill, &nMin, &nMax, &nColor);
    sub_6FAF3300((nResult == 0) ? 1 : 0, nRow, pUnit, pSkill, 0);
    if (!nResult)
        return;

    D2WIN_SetFont(6);
    wchar_t wszDmg[64];
    wszDmg[0] = L'\0';
    if (nResult == 2)
    {
        wchar_t wszPrefix[8] = L"";
        if (pSkillDesc->wDescDam == 6 || pSkillDesc->wDescDam == 7)
            wcscpy(wszPrefix, L"+");
        sub_6FAF27A0(wszPrefix, wszDmg, nMin, nMax);
    }
    else if (nResult == 3)
    {
        sub_6FAF3090(nSkillId, nSkillLevel, pUnit, wszDmg);
    }

    int32_t nWidth = D2WIN_GetTextWidth(wszDmg);
    const D2CharPagTextBoxStrc* pBox = &g_aCharPagTextBoxes_6FBA3980[nRow][2];
    if (nWidth > (pBox->nXRight - pBox->nXLeft))
        D2WIN_SetFont(0);
    sub_6FAF3240(wszDmg, nColor, nRow, 2);
    D2WIN_SetFont(1);
}

// 1.12a: 0x6FAF5DC0
// Calling convention: __usercall (ECX = nSkillId, [esp+4] = pUnit, [esp+8] = nSkillLevel, [esp+12] = pWeapon, [esp+16] = pPhysBonus, [esp+20] = pMinPhys, [esp+24] = pMaxPhys, [esp+28] = nApplyPenalty)
int32_t __fastcall sub_6FAF5DC0(int32_t nSkillId, D2UnitStrc* pUnit, int32_t nSkillLevel, D2UnitStrc* pWeapon, int32_t* pPhysBonus, int32_t* pMinPhys, int32_t* pMaxPhys, int32_t nApplyPenalty)
{
    D2StatListStrc* pSavedStatList = nullptr;
    sub_6FAF20D0(pUnit, &pSavedStatList, pWeapon);

    *pPhysBonus = 0;
    *pMinPhys = 256;
    *pMaxPhys = 512;
    if (!pUnit)
        return 0;

    int32_t nBaseMin = sub_6FAFCE90(pUnit, 0, pWeapon);
    int32_t nBaseMax = sub_6FAFCD70(pUnit, 0, pWeapon);
    if (nBaseMin < 256)
        nBaseMin = 256;
    if (nBaseMax <= nBaseMin)
        nBaseMax = nBaseMin + 256;

    uint8_t* pSkillsTxt = (nSkillId >= 0) ? (uint8_t*)sub_6FAFF7E0(nSkillId) : nullptr;
    int32_t nBonusMin = 0, nBonusMax = 0;
    if (pSkillsTxt && nSkillLevel > 0)
    {
        if (pSkillsTxt[0x155])
        {
            nBonusMin += SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 0);
            nBonusMax += SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 0);
        }
        int32_t nSkillPct = 0;
        for (int32_t i = 0; i < 6; ++i)
        {
            int16_t nStatId = *(int16_t*)(pSkillsTxt + 0x3A + i * 2);
            if (nStatId == 0x19)
                nSkillPct += SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x50 + i * 4), nSkillId, nSkillLevel);
            else if (nStatId == 0x15)
                nBonusMin += SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x50 + i * 4), nSkillId, nSkillLevel) << 8;
            else if (nStatId == 0x16)
                nBonusMax += SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x50 + i * 4), nSkillId, nSkillLevel) << 8;
        }
        if (pSkillsTxt[0x04] & 0x20)
            nSkillPct += SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x148), nSkillId, nSkillLevel);

        int32_t nElemConvPct = 0;
        if (pSkillsTxt[0x17C] != 0 && *(uint32_t*)(pSkillsTxt + 0x1A0) != 0)
            nElemConvPct = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x1A0), nSkillId, nSkillLevel);

        if (nElemConvPct < 100)
            *pPhysBonus += nSkillPct;
    }

    int32_t nUnitPct = STATLIST_UnitGetStatValue(pUnit, 0x19, 0) + SKILLS_GetWeaponMasteryBonus(pUnit, pWeapon, 0, 1);
    *pPhysBonus += nUnitPct;

    int32_t nWeapBonusPct = sub_6FAFCC20(pUnit, 0, pWeapon);
    if (*pPhysBonus + nWeapBonusPct < -90)
        nWeapBonusPct = -90 - *pPhysBonus;

    int32_t nTotalMin = nBaseMin + nBonusMin;
    int32_t nTotalMax = nBaseMax + nBonusMax;
    nTotalMin += (int32_t)(((int64_t)nTotalMin * (*pPhysBonus + nWeapBonusPct)) / 100);
    nTotalMax += (int32_t)(((int64_t)nTotalMax * (*pPhysBonus + nWeapBonusPct)) / 100);

    if (pSkillsTxt && nApplyPenalty)
    {
        int32_t nSrcDam = pSkillsTxt[0x156];
        if (!nSrcDam)
            nSrcDam = 128;
        nTotalMin = (int32_t)(((int64_t)nTotalMin * nSrcDam) >> 7);
        nTotalMax = (int32_t)(((int64_t)nTotalMax * nSrcDam) >> 7);
    }

    if (pWeapon && ITEMS_CheckWeaponIfThrow(pWeapon) && STATES_CheckState(pUnit, 0x80))
    {
        D2SkillStrc* pDoubleThrow = SKILLS_GetSkillById(pUnit, 140, -1);
        if (pDoubleThrow)
        {
            uint8_t* pDT = (uint8_t*)sub_6FAFF7E0(140);
            if (pDT)
            {
                int32_t nDTPct = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pDT + 0x13C), 140, SKILLS_GetSkillLevel(pUnit, pDoubleThrow, 1));
                if (nDTPct)
                {
                    nTotalMin += nTotalMin * nDTPct / 100;
                    nTotalMax += nTotalMax * nDTPct / 100;
                    *pPhysBonus += nDTPct;
                }
            }
        }
    }

    if (nTotalMin < 0)
        nTotalMin = 0;
    if (nTotalMax < nTotalMin)
        nTotalMax = nTotalMin;

    *pMinPhys = nTotalMin;
    *pMaxPhys = nTotalMax;
    sub_6FAF2170(pSavedStatList, pUnit, pWeapon);
    return 1;
}

// 1.12a: 0x6FAF61B0
// Calling convention: __usercall (ECX = nSkillId, [esp+4] = pUnit, [esp+8] = nSkillLevel, [esp+12] = pWeapon, [esp+16] = pMinDmg, [esp+20] = pMaxDmg, [esp+24] = pColor, [esp+28] = nApplyPenalty, [esp+32] = nElemType)
int32_t __fastcall sub_6FAF61B0(int32_t nSkillId, D2UnitStrc* pUnit, int32_t nSkillLevel, D2UnitStrc* pWeapon, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nApplyPenalty, int32_t nElemType)
{
    *pMinDmg = 0;
    *pMaxDmg = 0;
    *pColor = 0;

    int32_t nPhysBonus = 0, nMinPhys = 0, nMaxPhys = 0;
    sub_6FAF5DC0(nSkillId, pUnit, nSkillLevel, pWeapon, &nPhysBonus, &nMinPhys, &nMaxPhys, nApplyPenalty);

    int32_t nElemBonus = 0, nMinElem = 0, nMaxElem = 0, nDomElem = nElemType;
    sub_6FAF5050(nSkillId, pUnit, nSkillLevel, pWeapon, &nElemBonus, &nMinElem, &nMaxElem, &nDomElem, nElemType);

    *pMinDmg = nMinPhys + nMinElem;
    *pMaxDmg = nMaxPhys + nMaxElem;
    if (*pMinDmg < 256)
        *pMinDmg = 256;
    if (*pMaxDmg <= *pMinDmg)
        *pMaxDmg = *pMinDmg + 256;

    if (STATES_CheckStateMaskDamBlueOnUnit(pUnit) || nPhysBonus > 0 || nElemBonus > 0)
        *pColor = 3;
    if (STATES_CheckStateMaskDamRedOnUnit(pUnit) || nPhysBonus < 0)
        *pColor = 1;
    return 2;
}

// 1.12a: 0x6FAF6280
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF6280(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (!pSkillsTxt)
        return 0;

    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    int32_t nPhysBonus = 0, nMinPhys = 0, nMaxPhys = 0;
    sub_6FAF5DC0(nSkillId, pUnit, nSkillLevel, pWeapon, &nPhysBonus, &nMinPhys, &nMaxPhys, 0);

    int32_t nElemBonus = 0, nMinElem = 0, nMaxElem = 0;
    sub_6FAF5050(-1, pUnit, 0, pWeapon, &nElemBonus, &nMinElem, &nMaxElem, nullptr, nElemType);

    int32_t nConvPct = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x13C), nSkillId, nSkillLevel);
    if (nConvPct > 0)
    {
        nMinPhys += nMinPhys * nConvPct / 100;
        nMaxPhys += nMaxPhys * nConvPct / 100;
        nPhysBonus += nConvPct;
    }

    *pMinDmg = nMinPhys + nMinElem;
    *pMaxDmg = nMaxPhys + nMaxElem;
    if (*pMinDmg < 256)
        *pMinDmg = 256;
    if (*pMaxDmg <= *pMinDmg)
        *pMaxDmg = *pMinDmg + 256;

    if (STATES_CheckStateMaskDamBlueOnUnit(pUnit) || nPhysBonus > 0 || nElemBonus > 0)
        *pColor = 3;
    if (STATES_CheckStateMaskDamRedOnUnit(pUnit) || nPhysBonus < 0)
        *pColor = 1;
    return 2;
}

// 1.12a: 0x6FAF63E0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF63E0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (!pSkillsTxt)
        return 0;

    int32_t nSkillPct = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x148), nSkillId, nSkillLevel);
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    int32_t nPhysBonus = 0, nMinPhys = 0, nMaxPhys = 0;
    sub_6FAF5DC0(0, pUnit, 1, pWeapon, &nPhysBonus, &nMinPhys, &nMaxPhys, 0);

    int32_t nElemBonus = 0, nMinElem = 0, nMaxElem = 0;
    sub_6FAF5050(-1, pUnit, 0, pWeapon, &nElemBonus, &nMinElem, &nMaxElem, nullptr, nElemType);

    nMinPhys = (nMinPhys + nMinPhys * nSkillPct / 100) * 3 / 4;
    nMaxPhys = (nMaxPhys + nMaxPhys * nSkillPct / 100) * 3 / 4;
    nMinElem = nMinElem * 3 / 4;
    nMaxElem = nMaxElem * 3 / 4;

    *pMinDmg = nMinPhys + nMinElem;
    *pMaxDmg = nMaxPhys + nMaxElem;
    if (*pMinDmg < 256)
        *pMinDmg = 256;
    if (*pMaxDmg <= *pMinDmg)
        *pMaxDmg = *pMinDmg + 256;

    if (STATES_CheckStateMaskDamBlueOnUnit(pUnit) || nPhysBonus > 0 || nElemBonus > 0)
        *pColor = 3;
    if (STATES_CheckStateMaskDamRedOnUnit(pUnit) || nPhysBonus < 0)
        *pColor = 1;
    return 2;
}

// 1.12a: 0x6FAF6510
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF6510(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (!pSkillsTxt)
        return 0;

    int32_t nSkillPct = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x148), nSkillId, nSkillLevel);
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    int32_t nPhysBonus = 0, nMinPhys = 0, nMaxPhys = 0;
    sub_6FAF5DC0(0, pUnit, 1, pWeapon, &nPhysBonus, &nMinPhys, &nMaxPhys, 0);

    int32_t nElemBonus = 0, nMinElem = 0, nMaxElem = 0;
    sub_6FAF5050(-1, pUnit, 0, pWeapon, &nElemBonus, &nMinElem, &nMaxElem, nullptr, nElemType);

    nMinPhys = (nMinPhys + nMinPhys * nSkillPct / 100) * 3 / 4 + SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
    nMaxPhys = (nMaxPhys + nMaxPhys * nSkillPct / 100) * 3 / 4 + SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
    nMinElem = nMinElem * 3 / 4 + SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    nMaxElem = nMaxElem * 3 / 4 + SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);

    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (pSkillDesc)
    {
        if (pSkillDesc->dwDamCalc[0])
        {
            int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDamCalc[0], nSkillId, nSkillLevel);
            if (nPct)
            {
                nMinElem += nMinElem * nPct / 100;
                nMaxElem += nMaxElem * nPct / 100;
            }
        }
        if (pSkillDesc->dwDamCalc[1])
        {
            int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDamCalc[1], nSkillId, nSkillLevel) << 8;
            nMinElem += nBonus;
            nMaxElem += nBonus;
        }
    }

    *pMinDmg = nMinPhys + nMinElem;
    *pMaxDmg = nMaxPhys + nMaxElem;
    if (*pMinDmg < 256)
        *pMinDmg = 256;
    if (*pMaxDmg <= *pMinDmg)
        *pMaxDmg = *pMinDmg + 256;

    if (STATES_CheckStateMaskDamBlueOnUnit(pUnit) || nPhysBonus > 0 || nElemBonus > 0)
        *pColor = 3;
    if (STATES_CheckStateMaskDamRedOnUnit(pUnit) || nPhysBonus < 0)
        *pColor = 1;
    return 2;
}

// 1.12a: 0x6FAF66F0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF66F0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (!pSkillsTxt)
        return 0;

    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    int32_t nPhysBonus = 0, nMinPhys = 0, nMaxPhys = 0;
    sub_6FAF5DC0(0, pUnit, 1, pWeapon, &nPhysBonus, &nMinPhys, &nMaxPhys, 0);

    int32_t nElemBonus = 0, nMinElem = 0, nMaxElem = 0;
    sub_6FAF5050(nSkillId, pUnit, nSkillLevel, pWeapon, &nElemBonus, &nMinElem, &nMaxElem, nullptr, nElemType);

    nMinPhys = nMinPhys * 3 / 4;
    nMaxPhys = nMaxPhys * 3 / 4;
    nMinElem += nMinPhys;
    nMaxElem += nMaxPhys;

    *pMinDmg = nMinElem;
    *pMaxDmg = nMaxElem;
    if (*pMinDmg < 256)
        *pMinDmg = 256;
    if (*pMaxDmg <= *pMinDmg)
        *pMaxDmg = *pMinDmg + 256;

    if (STATES_CheckStateMaskDamBlueOnUnit(pUnit) || nPhysBonus > 0 || nElemBonus > 0)
        *pColor = 3;
    if (STATES_CheckStateMaskDamRedOnUnit(pUnit) || nPhysBonus < 0)
        *pColor = 1;
    return 2;
}

// 1.12a: 0x6FAF6800
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF6800(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (!pSkillsTxt)
        return 0;

    int32_t nSkillPct = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x148), nSkillId, nSkillLevel);
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    int32_t nPhysBonus = 0, nMinPhys = 0, nMaxPhys = 0;
    sub_6FAF5DC0(0, pUnit, 1, pWeapon, &nPhysBonus, &nMinPhys, &nMaxPhys, 0);

    int32_t nElemBonus = 0, nMinElem = 0, nMaxElem = 0;
    sub_6FAF5050(-1, pUnit, 0, pWeapon, &nElemBonus, &nMinElem, &nMaxElem, nullptr, nElemType);

    nMinPhys = (nMinPhys + nMinPhys * nSkillPct / 100) * 3 / 4;
    nMaxPhys = (nMaxPhys + nMaxPhys * nSkillPct / 100) * 3 / 4;
    nMinElem = nMinElem * 3 / 4 + SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    nMaxElem = nMaxElem * 3 / 4 + SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);

    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (pSkillDesc)
    {
        if (pSkillDesc->dwDamCalc[0])
        {
            int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDamCalc[0], nSkillId, nSkillLevel);
            if (nPct)
            {
                nMinElem += nMinElem * nPct / 100;
                nMaxElem += nMaxElem * nPct / 100;
            }
        }
        if (pSkillDesc->dwDamCalc[1])
        {
            int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDamCalc[1], nSkillId, nSkillLevel) << 8;
            nMinElem += nBonus;
            nMaxElem += nBonus;
        }
    }

    *pMinDmg = nMinPhys + nMinElem;
    *pMaxDmg = nMaxPhys + nMaxElem;
    if (*pMinDmg < 256)
        *pMinDmg = 256;
    if (*pMaxDmg <= *pMinDmg)
        *pMaxDmg = *pMinDmg + 256;

    if (STATES_CheckStateMaskDamBlueOnUnit(pUnit) || nPhysBonus > 0 || nElemBonus > 0)
        *pColor = 3;
    if (STATES_CheckStateMaskDamRedOnUnit(pUnit) || nPhysBonus < 0)
        *pColor = 1;
    return 2;
}

// 1.12a: 0x6FAF6A20
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF6A20(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (!pSkillsTxt)
        return 0;

    D2InventoryStrc* pInventory = *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60);
    D2UnitStrc* pWeapon = sub_6FB57830(pInventory);
    if (!pWeapon || !ITEMS_CheckWeaponIfThrow(pWeapon))
        return sub_6FAF61B0(0, pUnit, 1, pWeapon, pMinDmg, pMaxDmg, pColor, 1, nElemType);

    D2StatListStrc* pSavedStatList = nullptr;
    sub_6FAF20D0(pUnit, &pSavedStatList, pWeapon);

    *pMinDmg = 256;
    *pMaxDmg = 512;
    *pColor = 0;

    int32_t nBaseMin = sub_6FAFCE90(pUnit, 2, pWeapon);
    int32_t nBaseMax = sub_6FAFCD70(pUnit, 2, pWeapon);
    if (nBaseMin < 256)
        nBaseMin = 256;
    if (nBaseMax <= nBaseMin)
        nBaseMax = nBaseMin + 256;

    int32_t nBonusMin = 0, nBonusMax = 0, nSkillPct = 0;
    if (nSkillLevel > 0)
    {
        if (pSkillsTxt[0x155])
        {
            nBonusMin += SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 0);
            nBonusMax += SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 0);
        }
        for (int32_t i = 0; i < 6; ++i)
        {
            int16_t nStatId = *(int16_t*)(pSkillsTxt + 0x3A + i * 2);
            if (nStatId == 0x19)
                nSkillPct += SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x50 + i * 4), nSkillId, nSkillLevel);
            else if (nStatId == 0x15)
                nBonusMin += SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x50 + i * 4), nSkillId, nSkillLevel) << 8;
            else if (nStatId == 0x16)
                nBonusMax += SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x50 + i * 4), nSkillId, nSkillLevel) << 8;
        }
        if (pSkillsTxt[0x04] & 0x20)
            nSkillPct += SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x148), nSkillId, nSkillLevel);
    }

    int32_t nPhysBonus = nSkillPct + STATLIST_UnitGetStatValue(pUnit, 0x19, 0) + STATLIST_UnitGetStatValue(pUnit, 0x88, 0);
    int32_t nWeapBonusPct = sub_6FAFCC20(pUnit, 2, pWeapon);
    if (nPhysBonus + nWeapBonusPct < -90)
        nWeapBonusPct = -90 - nPhysBonus;

    int32_t nMinPhys = nBaseMin + nBonusMin;
    int32_t nMaxPhys = nBaseMax + nBonusMax;
    nMinPhys += (int32_t)(((int64_t)nMinPhys * (nPhysBonus + nWeapBonusPct)) / 100);
    nMaxPhys += (int32_t)(((int64_t)nMaxPhys * (nPhysBonus + nWeapBonusPct)) / 100);

    sub_6FAF2170(pSavedStatList, pUnit, pWeapon);

    int32_t nElemBonus = 0, nMinElem = 0, nMaxElem = 0;
    sub_6FAF5050(nSkillId, pUnit, nSkillLevel, pWeapon, &nElemBonus, &nMinElem, &nMaxElem, nullptr, nElemType);

    *pMinDmg = nMinPhys + nMinElem;
    *pMaxDmg = nMaxPhys + nMaxElem;
    if (*pMinDmg < 256)
        *pMinDmg = 256;
    if (*pMaxDmg <= *pMinDmg)
        *pMaxDmg = *pMinDmg + 256;

    if (STATES_CheckStateMaskDamBlueOnUnit(pUnit) || nPhysBonus > 0 || nElemBonus > 0)
        *pColor = 3;
    if (STATES_CheckStateMaskDamRedOnUnit(pUnit) || nPhysBonus < 0)
        *pColor = 1;
    return 2;
}

// 1.12a: 0x6FAF6CF0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF6CF0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pAR, int32_t* pColor)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (!pSkillsTxt)
        return 0;

    D2InventoryStrc* pInventory = *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60);
    D2UnitStrc* pLeftWeapon = INVENTORY_GetLeftHandWeapon(pInventory);
    D2UnitStrc* pRightWeapon = INVENTORY_GetSecondaryWeapon(pInventory);

    int32_t bLeftThrow = pLeftWeapon && ITEMS_CheckWeaponIfThrow(pLeftWeapon);
    int32_t bRightThrow = pRightWeapon && ITEMS_CheckWeaponIfThrow(pRightWeapon);
    if (!bLeftThrow || !bRightThrow)
        return sub_6FAF35C0(pUnit, 0, 1, pAR, pColor);

    int32_t nAR1 = 0, nAR2 = 0;
    sub_6FAF2430(pLeftWeapon, pUnit, nSkillId, nSkillLevel, &nAR1, pColor);
    sub_6FAF2430(pRightWeapon, pUnit, nSkillId, nSkillLevel, &nAR2, pColor);
    *pAR = (nAR1 + nAR2) / 2;
    return 1;
}

// 1.12a: 0x6FAF7410
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pSkillDesc, [esp+8] = wszOut)
void __fastcall sub_6FAF7410(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, wchar_t* wszOut)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (!pSkillsTxt)
        return;

    int32_t nManaShift = *(int16_t*)(pSkillsTxt + 0x160);
    int32_t nBaseMana = *(int16_t*)(pSkillsTxt + 0x15E);
    int32_t nLvlMana = (nSkillLevel > 1) ? ((nSkillLevel - 1) * *(int16_t*)(pSkillsTxt + 0x162)) : 0;
    int32_t nManaFixed = (nBaseMana + nLvlMana) << nManaShift;
    int32_t nMinMana = *(int16_t*)(pSkillsTxt + 0x15C) << 8;
    if (nManaFixed < nMinMana)
        nManaFixed = nMinMana;

    if (nManaFixed <= 0 && pSkillDesc->wStrMana == 0)
        return;

    uint16_t wStrMana = pSkillDesc->wStrMana ? pSkillDesc->wStrMana : 0x0DD5;
    wcscpy(wszOut, D2LANG_GetStringByTblIndex(wStrMana));

    if (*(int16_t*)(pSkillsTxt + 0x160) >= 8 || (nManaFixed & 0xFF) == 0)
    {
        wchar_t wszNum[32];
        swprintf(wszNum, L"%d", nManaFixed >> 8);
        wcscat(wszOut, wszNum);
    }
    else
    {
        wchar_t wszNum[32];
        swprintf(wszNum, L"%d.%d", nManaFixed >> 8, ((nManaFixed & 0xFF) * 10) >> 8);
        wcscat(wszOut, wszNum);
    }
}

// 1.12a: 0x6FAF7560
// Calling convention: __stdcall
int32_t __stdcall sub_6FAF7560(int32_t nSkillId, wchar_t* wszOut, uint32_t nMaxChars)
{
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (!pSkillDesc || !pSkillDesc->wStrName)
        return 0;

    wcsncpy(wszOut, D2LANG_GetStringByTblIndex(pSkillDesc->wStrName), nMaxChars);
    wszOut[nMaxChars - 1] = L'\0';
    return 1;
}

// 1.12a: 0x6FAF75A0
// Calling convention: __usercall (EDX = nRow, ECX = pUnit)
void __fastcall sub_6FAF75A0(D2UnitStrc* pUnit, int32_t nRow)
{
    if (!pUnit || !UNITS_CanDualWield(pUnit))
        return;

    int32_t nMin = 0, nMax = 0, nColor = 0;
    if (!sub_6FAFA1E0(pUnit, &nMin, &nMax, &nColor))
        return;

    wchar_t wszDmg[64];
    sub_6FAF27A0(L"", wszDmg, nMin, nMax);
    D2WIN_SetFont(6);
    const D2CharPagTextBoxStrc* pBox = &g_aCharPagTextBoxes_6FBA3980[nRow][2];
    if (D2WIN_GetTextWidth(wszDmg) > (pBox->nXRight - pBox->nXLeft))
        D2WIN_SetFont(0);
    sub_6FAF3240(wszDmg, nColor, nRow, 2);
    D2WIN_SetFont(1);
}

// 1.12a: 0x6FAF7600
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pSkillDesc, [esp+8] = nLineIdx, [esp+12] = wszOut)
// Jump table at 0x6FAF89FC (75 cases: descline 1..75)
void __fastcall sub_6FAF7600(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, D2SkillDescTxt* pSkillDesc, int32_t nLineIdx, wchar_t* wszOut)
{
    uint8_t nDescLine = pSkillDesc->nDescLine[nLineIdx];
    uint16_t wStrA = pSkillDesc->wDescTextA[nLineIdx];
    uint16_t wStrB = pSkillDesc->wDescTextB[nLineIdx];
    uint32_t dwCalcA = pSkillDesc->dwDescCalcA[nLineIdx];
    uint32_t dwCalcB = pSkillDesc->dwDescCalcB[nLineIdx];

    int32_t nValA = dwCalcA ? SKILLS_EvaluateSkillFormula(pUnit, dwCalcA, nSkillId, nSkillLevel) : 0;
    int32_t nValB = dwCalcB ? SKILLS_EvaluateSkillFormula(pUnit, dwCalcB, nSkillId, nSkillLevel) : 0;

    switch (nDescLine)
    {
    case 1:
        sub_6FAF7410(nSkillId, nSkillLevel, pUnit, pSkillDesc, wszOut);
        break;
    case 2:
        sub_6FAF29B0(wStrA, wszOut, wStrB, nValA);
        break;
    case 3:
        sub_6FAF2950(wStrA, wszOut, wStrB, nValA);
        break;
    case 4:
        sub_6FB20280(D2LANG_GetStringByTblIndex(wStrA), wszOut, nValA);
        break;
    case 5:
        sub_6FB202B0(D2LANG_GetStringByTblIndex(wStrA), wszOut, nValA);
        break;
    case 6:
        sub_6FB20310(wszOut, nValA, D2LANG_GetStringByTblIndex(wStrA));
        break;
    case 7:
        sub_6FB20340(wszOut, nValA, D2LANG_GetStringByTblIndex(wStrA));
        break;
    case 8:
    {
        uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
        int32_t nToHit = pSkillsTxt ? SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel) : 0;
        sub_6FAF29B0(0x0D9E, wszOut, 0x0DC9, nToHit);
        break;
    }
    case 9:
    {
        uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
        if (pSkillsTxt)
        {
            int32_t nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
            int32_t nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1);
            sub_6FAF2AB0(nMin, nMax, wszOut, 0x0DC7, wStrB);
        }
        break;
    }
    case 10:
        sub_6FAF3750(nSkillId, nSkillLevel, pUnit, 0x0DC7, wStrB, wszOut);
        break;
    case 11:
        sub_6FAF3790(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut);
        break;
    case 12:
    {
        int32_t nElemLen = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);
        sub_6FAF2560(nElemLen, wszOut, wStrA, wStrB);
        break;
    }
    case 13:
    {
        D2MonStatInitStrc monStatInit = {};
        int32_t nHp = 0;
        if (sub_6FAF2E30(nSkillId, pUnit, nSkillLevel, &monStatInit, &nHp))
            sub_6FB202B0(D2LANG_GetStringByTblIndex(0x0DDC), wszOut, nHp);
        break;
    }
    case 14:
        sub_6FAF3CF0(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut, dwCalcA, dwCalcB);
        break;
    case 15:
        sub_6FAF2560(nValA, wszOut, wStrA, wStrB);
        break;
    case 16:
        sub_6FAF2560(nValA * 25, wszOut, wStrA, wStrB);
        break;
    case 17:
        sub_6FAF2AB0(nValA << 8, nValB << 8, wszOut, wStrA, wStrB);
        break;
    case 18:
        wcscpy(wszOut, D2LANG_GetStringByTblIndex(wStrA));
        break;
    case 19:
        sub_6FAF2840(wStrA, wStrB, wszOut, nValA);
        break;
    case 20:
        sub_6FAF29B0(wStrA, wszOut, 0x0DC9, nValA);
        break;
    case 21:
        sub_6FAF2950(wStrA, wszOut, 0x0DC9, nValA);
        break;
    case 22:
        sub_6FAF3870(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut);
        break;
    case 23:
        sub_6FAF2560(nValA, wszOut, wStrA, 0x0DC8);
        break;
    case 24:
        sub_6FAF3C20(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut, dwCalcA, dwCalcB);
        break;
    case 25:
        sub_6FAF3B50(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut, dwCalcA, dwCalcB);
        break;
    case 26:
        sub_6FAF3820(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut);
        break;
    case 27:
        sub_6FAF38D0(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut);
        break;
    case 28:
        sub_6FAF2840(0x0D9A, 0x0DCB, wszOut, 2);
        break;
    case 29:
        sub_6FAF2840(wStrB, 0x0DCB, wszOut, nValA);
        break;
    case 30:
    {
        uint16_t wMissileId = pSkillDesc->wDescMissile[nLineIdx];
        int32_t nRange = MISSILE_GetRange(0, pUnit, wMissileId, nSkillLevel);
        sub_6FAF2560(nRange, wszOut, wStrA, wStrB);
        break;
    }
    case 31:
        sub_6FAF2560(nValA, wszOut, wStrA, 0x0DC8);
        break;
    case 32:
    {
        wchar_t wszBase[256];
        sub_6FAF29B0(wStrA, wszBase, 0, nValA);
        sub_6FB201D0(wszOut, wszBase, nValB, D2LANG_GetStringByTblIndex(wStrB));
        break;
    }
    case 33:
        sub_6FAF4EA0(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut);
        break;
    case 34:
        sub_6FAF5220(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut);
        break;
    case 35:
        sub_6FAF2A10(nValA << 8, nValB << 8, wszOut, wStrA, wStrB);
        break;
    case 36:
        sub_6FB202B0(D2LANG_GetStringByTblIndex((nValA == 1) ? wStrA : wStrB), wszOut, nValA);
        break;
    case 37:
        sub_6FAF2840(wStrA, wStrB, wszOut, nValA / 2);
        break;
    case 38:
        sub_6FAF2A10(nValA << 8, nValB << 8, wszOut, wStrA, 0x0DC9);
        break;
    case 39:
        sub_6FAF5330(pUnit, nSkillId, nSkillLevel, wStrA, wStrB, wszOut, dwCalcA, dwCalcB);
        break;
    case 40:
    {
        wchar_t wszColor[16];
        swprintf(wszColor, L"%s", sub_6FB200F0(nValA));
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszOut, wszColor);
        if (wStrB)
            wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrB));
        break;
    }
    case 41:
        sub_6FAF54E0(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut, dwCalcA, dwCalcB);
        break;
    case 42:
    {
        wchar_t wszVal[64];
        swprintf(wszVal, L"+%d.%d", nValA, nValB);
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(wStrA), wszVal, D2LANG_GetStringByTblIndex(wStrB));
        break;
    }
    case 43:
        sub_6FAF2BF0((nValA << 8) / 10, (nValB << 8) / 10, wszOut, wStrA, wStrB);
        break;
    case 44:
        sub_6FAF2BF0((nValA << 8) / 256, (nValB << 8) / 256, wszOut, wStrA, wStrB);
        break;
    case 45:
    {
        int32_t nManaSteal = (nValA * 25) >> 8;
        sub_6FAF2950(wStrA, wszOut, wStrB, nManaSteal);
        break;
    }
    case 46:
        wcscpy(wszOut, D2LANG_GetStringByTblIndex(wStrA));
        break;
    case 47:
        sub_6FAF2A10(nValA, nValB, wszOut, wStrA, wStrB);
        break;
    case 48:
        sub_6FAF2BF0(nValA, nValB, wszOut, wStrA, wStrB);
        break;
    case 49:
        if (sub_6FB074E0(pUnit, nSkillId))
            sub_6FAF4620(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut);
        break;
    case 50:
        if (sub_6FB07530(pUnit, nSkillId))
            sub_6FAF4330(nSkillId, nSkillLevel, pSkillDesc->wDescMissile[0], pUnit, wStrA, wStrB, wszOut);
        break;
    case 51:
        if (sub_6FB07580(pUnit, nSkillId))
            sub_6FAF2950(wStrA, wszOut, wStrB, nValA);
        break;
    case 52:
        if (sub_6FB075D0(pUnit, nSkillId))
            sub_6FAF2AB0(nValA << 8, nValB << 8, wszOut, wStrA, wStrB);
        break;
    case 53:
        sub_6FAF4D70(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut);
        break;
    case 54:
        sub_6FAF4DC0(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut, dwCalcA, dwCalcB);
        break;
    case 55:
        sub_6FAF4C50(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut, dwCalcA, dwCalcB);
        break;
    case 56:
        sub_6FAF3450(nSkillId, nSkillLevel, pUnit, pSkillDesc, nLineIdx, wszOut);
        break;
    case 57:
        sub_6FAF5530(pSkillDesc, nSkillLevel, pUnit, nLineIdx, wszOut);
        break;
    case 58:
        sub_6FAF3500(nSkillId, nSkillLevel, pUnit, pSkillDesc, nLineIdx, wszOut);
        break;
    case 59:
        sub_6FAF3690(nSkillId, nSkillLevel, pUnit, pSkillDesc, nLineIdx, wszOut);
        break;
    case 60:
        sub_6FAF5690(nSkillId, nSkillLevel, pUnit, pSkillDesc, nLineIdx, wszOut);
        break;
    case 61:
        sub_6FAF3EC0(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut, dwCalcA, dwCalcB);
        break;
    case 62:
        sub_6FAF40E0(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut, dwCalcA, dwCalcB);
        break;
    case 63:
    {
        const wchar_t* wszSkillName = D2LANG_GetStringByTblIndex(wStrB ? wStrB : sub_6FAF5BD0(nValB));
        wchar_t wszTemp[256];
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszTemp, wszSkillName);
        sub_6FB20280(wszTemp, wszOut, nValA);
        break;
    }
    case 64:
        sub_6FAF4750(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut, dwCalcA, dwCalcB);
        break;
    case 65:
        sub_6FAF4810(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut, dwCalcA, dwCalcB);
        break;
    case 66:
    {
        wchar_t wszTemp[256];
        sub_6FB202B0(D2LANG_GetStringByTblIndex(wStrA), wszTemp, nValA);
        sub_6FB202B0(wszTemp, wszOut, nValB);
        break;
    }
    case 67:
        sub_6FAF29B0(wStrA, wszOut, wStrB, nValA);
        break;
    case 68:
        sub_6FAF2950(wStrA, wszOut, wStrB, nValA);
        break;
    case 69:
    {
        const wchar_t* wszRef = D2LANG_GetStringByTblIndex(wStrB);
        wchar_t wszTemp[256];
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszTemp, wszRef);
        sub_6FB202B0(wszTemp, wszOut, nValA);
        break;
    }
    case 70:
    {
        const wchar_t* wszRef = D2LANG_GetStringByTblIndex(wStrB);
        wchar_t wszTemp[256];
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszTemp, wszRef);
        sub_6FB20280(wszTemp, wszOut, nValA);
        break;
    }
    case 71:
    {
        const wchar_t* wszRef = D2LANG_GetStringByTblIndex(wStrB ? wStrB : sub_6FAF5BD0(nValB));
        wchar_t wszTemp[256];
        sub_6FB202E0(D2LANG_GetStringByTblIndex(wStrA), wszTemp, wszRef);
        sub_6FB202B0(wszTemp, wszOut, nValA);
        break;
    }
    case 72:
    {
        wchar_t wszRatio[64];
        swprintf(wszRatio, L"%d/%d", nValA, nValB);
        sub_6FB20200(wszOut, D2LANG_GetStringByTblIndex(wStrA), wszRatio, D2LANG_GetStringByTblIndex(wStrB));
        break;
    }
    case 73:
    {
        wchar_t wszRatio[64];
        swprintf(wszRatio, L"%d/%d", nValA, nValB);
        sub_6FB202E0(wszRatio, wszOut, D2LANG_GetStringByTblIndex(wStrA));
        break;
    }
    case 74:
        sub_6FAF4620(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut);
        break;
    case 75:
        sub_6FAF4680(nSkillId, nSkillLevel, pUnit, wStrA, wStrB, wszOut);
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FAF8B30
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF8B30(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (!pSkillsTxt)
        return 0;

    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    int32_t nPhysBonus = 0, nMinPhys = 0, nMaxPhys = 0;
    sub_6FAF5DC0(0, pUnit, 1, pWeapon, &nPhysBonus, &nMinPhys, &nMaxPhys, 0);

    int32_t nElemBonus = 0, nMinElem = 0, nMaxElem = 0;
    sub_6FAF5050(nSkillId, pUnit, nSkillLevel, pWeapon, &nElemBonus, &nMinElem, &nMaxElem, nullptr, nElemType);

    nMinPhys = nMinPhys * 3 / 4;
    nMaxPhys = nMaxPhys * 3 / 4;
    *pMinDmg = nMinPhys + nMinElem;
    *pMaxDmg = nMaxPhys + nMaxElem;
    if (*pMinDmg < 256)
        *pMinDmg = 256;
    if (*pMaxDmg <= *pMinDmg)
        *pMaxDmg = *pMinDmg + 256;

    if (STATES_CheckStateMaskDamBlueOnUnit(pUnit) || nPhysBonus > 0 || nElemBonus > 0)
        *pColor = 3;
    if (STATES_CheckStateMaskDamRedOnUnit(pUnit) || nPhysBonus < 0)
        *pColor = 1;
    return 2;
}

// 1.12a: 0x6FAF8D60
// Calling convention: __fastcall
int32_t __fastcall sub_6FAF8D60(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (!pSkillsTxt)
        return 0;

    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    int32_t nPhysBonus = 0, nMinPhys = 0, nMaxPhys = 0;
    sub_6FAF5DC0(nSkillId, pUnit, nSkillLevel, pWeapon, &nPhysBonus, &nMinPhys, &nMaxPhys, 0);

    int32_t nElemBonus = 0, nMinElem = 0, nMaxElem = 0;
    sub_6FAF5050(nSkillId, pUnit, nSkillLevel, pWeapon, &nElemBonus, &nMinElem, &nMaxElem, nullptr, nElemType);

    int32_t nBonusPct = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x13C), nSkillId, nSkillLevel);
    if (nBonusPct > 0)
    {
        nMinPhys += nMinPhys * nBonusPct / 100;
        nMaxPhys += nMaxPhys * nBonusPct / 100;
    }

    *pMinDmg = nMinPhys + nMinElem;
    *pMaxDmg = nMaxPhys + nMaxElem;
    if (*pMinDmg < 256)
        *pMinDmg = 256;
    if (*pMaxDmg <= *pMinDmg)
        *pMaxDmg = *pMinDmg + 256;

    if (STATES_CheckStateMaskDamBlueOnUnit(pUnit) || nPhysBonus > 0 || nElemBonus > 0)
        *pColor = 3;
    if (STATES_CheckStateMaskDamRedOnUnit(pUnit) || nPhysBonus < 0)
        *pColor = 1;
    return 2;
}

// 1.12a: 0x6FAF91D0
// Calling convention: __cdecl
void __cdecl sub_6FAF91D0(void)
{
    D2UnitStrc* pPlayer = sub_6FAFF3D0();
    int32_t nSkillId = g_nHoveredSkillId_6FBBA6E8;
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (!pPlayer || !pSkillDesc)
        return;

    wchar_t wszLines[32][256] = {};
    int32_t nColors[32] = {};
    int32_t nLineCount = 0;

    for (int32_t i = 0; i < 6; ++i)
    {
        int32_t nIdx = 9 - i;
        if (pSkillDesc->nDescLine[nIdx])
        {
            sub_6FAF7600(nSkillId, 1, pPlayer, pSkillDesc, nIdx, wszLines[nLineCount]);
            if (wszLines[nLineCount][0])
                nLineCount++;
        }
    }

    if (nLineCount > 0)
        nLineCount++;

    for (int32_t i = 0; i < 6; ++i)
    {
        int32_t nIdx = 5 - i;
        if (pSkillDesc->nDescLine[nIdx])
        {
            sub_6FAF7600(nSkillId, 1, pPlayer, pSkillDesc, nIdx, wszLines[nLineCount]);
            if (wszLines[nLineCount][0])
                nLineCount++;
        }
    }

    wcscpy(wszLines[nLineCount++], D2LANG_GetStringByTblIndex(0x0DD6));
    nLineCount++;

    if (pSkillDesc->wStrLong)
    {
        wchar_t wszLong[512];
        wcscpy(wszLong, D2LANG_GetStringByTblIndex(pSkillDesc->wStrLong));
        wchar_t* pCur = wszLong;
        while (pCur && *pCur)
        {
            wchar_t* pNext = wcschr(pCur, L'\n');
            if (pNext)
                *pNext++ = L'\0';
            wcscpy(wszLines[nLineCount++], pCur);
            pCur = pNext;
        }
    }

    if (pSkillDesc->wStrName)
    {
        nColors[nLineCount] = 1;
        wcscpy(wszLines[nLineCount++], D2LANG_GetStringByTblIndex(pSkillDesc->wStrName));
    }

    wchar_t wszFull[2048] = {};
    for (int32_t i = nLineCount - 1; i >= 0; --i)
    {
        wcscat(wszFull, wszLines[i]);
        wcscat(wszFull, L"\n");
    }

    D2WIN_SetFont(1);
    D2WIN_DrawFramedText(wszFull, g_nSkillHoverX_6FBBA6D0, g_nSkillHoverY_6FBBA6CC, 0, (g_nSkillTreeTab_6FBA79FC == 3) ? 2 : 0);
}

// 1.12a: 0x6FAF9490
// Calling convention: __cdecl
void __cdecl sub_6FAF9490(void)
{
    D2UnitStrc* pPlayer = sub_6FAFF3D0();
    int32_t nSkillId = g_nHoveredSkillId_6FBBA6E8;
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (!pPlayer || !pSkillDesc)
        return;

    D2SkillStrc* pSkill = SKILLS_GetSkillById(pPlayer, nSkillId, -1);
    int32_t nSkillLevel = pSkill ? SKILLS_GetSkillLevel(pPlayer, pSkill, 1) : 0;

    wchar_t wszLines[48][256] = {};
    int32_t nColors[48] = {};
    int32_t nLineCount = 0;

    for (int32_t nIdx = 16; nIdx >= 10; --nIdx)
    {
        if (pSkillDesc->nDescLine[nIdx])
        {
            sub_6FAF7600(nSkillId, nSkillLevel > 0 ? nSkillLevel : 1, pPlayer, pSkillDesc, nIdx, wszLines[nLineCount]);
            if (wszLines[nLineCount][0])
            {
                if (pSkillDesc->nDescLine[nIdx] == 40)
                    nColors[nLineCount] = SKILLS_EvaluateSkillFormula(pPlayer, pSkillDesc->dwDescCalcA[nIdx], nSkillId, nSkillLevel > 0 ? nSkillLevel : 1);
                nLineCount++;
            }
        }
    }

    if (nLineCount > 0)
        nLineCount++;

    int32_t nNextLvl = nSkillLevel + 1;
    for (int32_t nIdx = 5; nIdx >= 0; --nIdx)
    {
        if (pSkillDesc->nDescLine[nIdx])
        {
            sub_6FAF7600(nSkillId, nNextLvl, pPlayer, pSkillDesc, nIdx, wszLines[nLineCount]);
            if (wszLines[nLineCount][0])
                nLineCount++;
        }
    }

    wcscpy(wszLines[nLineCount++], D2LANG_GetStringByTblIndex(nSkillLevel > 0 ? 0x0DD7 : 0x0DD6));
    nLineCount++;

    if (nSkillLevel > 0)
    {
        for (int32_t nIdx = 5; nIdx >= 0; --nIdx)
        {
            if (pSkillDesc->nDescLine[nIdx])
            {
                sub_6FAF7600(nSkillId, nSkillLevel, pPlayer, pSkillDesc, nIdx, wszLines[nLineCount]);
                if (wszLines[nLineCount][0])
                    nLineCount++;
            }
        }
        sub_6FB202B0(D2LANG_GetStringByTblIndex(0x0DD4), wszLines[nLineCount++], nSkillLevel);
        nLineCount++;
    }

    for (int32_t nIdx = 9; nIdx >= 6; --nIdx)
    {
        if (pSkillDesc->nDescLine[nIdx])
        {
            sub_6FAF7600(nSkillId, nSkillLevel > 0 ? nSkillLevel : 1, pPlayer, pSkillDesc, nIdx, wszLines[nLineCount]);
            if (wszLines[nLineCount][0])
                nLineCount++;
        }
    }

    if (pSkillDesc->wStrLong)
    {
        wchar_t wszLong[512];
        wcscpy(wszLong, D2LANG_GetStringByTblIndex(pSkillDesc->wStrLong));
        wchar_t* pCur = wszLong;
        while (pCur && *pCur)
        {
            wchar_t* pNext = wcschr(pCur, L'\n');
            if (pNext)
                *pNext++ = L'\0';
            wcscpy(wszLines[nLineCount++], pCur);
            pCur = pNext;
        }
    }

    if (pSkillDesc->wStrName)
    {
        nColors[nLineCount] = 3;
        wcscpy(wszLines[nLineCount++], D2LANG_GetStringByTblIndex(pSkillDesc->wStrName));
    }

    wchar_t wszFull[4096] = {};
    for (int32_t i = nLineCount - 1; i >= 0; --i)
    {
        wcscat(wszFull, wszLines[i]);
        wcscat(wszFull, L"\n");
    }

    D2WIN_SetFont(1);
    D2WIN_DrawFramedText(wszFull, g_nSkillHoverX_6FBBA6D0, g_nSkillHoverY_6FBBA6CC, 0, (g_nSkillTreeTab_6FBA79FC == 3) ? 2 : 0);
}

// 1.12a: 0x6FAF9A90
// Calling convention: __fastcall (ECX = nSkillId, EDX = nHoverIdx)
void __fastcall sub_6FAF9A90(int32_t nSkillId, int32_t nHoverIdx)
{
    D2UnitStrc* pPlayer = sub_6FAFF3D0();
    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (!pPlayer || !pSkillDesc)
        return;

    int32_t nSkillLevel = g_nSpeedBarSkillLevels_6FBC23B8[nHoverIdx];
    int32_t nSkillFlags = g_nSpeedBarSkillFlags_6FBC22C8[nHoverIdx];
    D2SkillStrc* pSkill = SKILLS_GetSkillById(pPlayer, nSkillId, nSkillFlags);

    wchar_t wszLines[32][256] = {};
    int32_t nLineCount = 0;

    for (int32_t nIdx = 5; nIdx >= 0; --nIdx)
    {
        if (pSkillDesc->nDescLine[nIdx])
        {
            sub_6FAF7600(nSkillId, nSkillLevel, pPlayer, pSkillDesc, nIdx, wszLines[nLineCount]);
            if (wszLines[nLineCount][0])
                nLineCount++;
        }
    }

    for (int32_t nIdx = 9; nIdx >= 6; --nIdx)
    {
        if (pSkillDesc->nDescLine[nIdx])
        {
            sub_6FAF7600(nSkillId, nSkillLevel, pPlayer, pSkillDesc, nIdx, wszLines[nLineCount]);
            if (wszLines[nLineCount][0])
                nLineCount++;
        }
    }

    if (pSkill)
    {
        int32_t nCharges = 0, nMaxCharges = 0;
        if (SKILLS_GetSkillInfo(pSkill, nullptr, nullptr, &nMaxCharges, &nCharges) && nMaxCharges > 0)
        {
            wchar_t wszChargeLine[128];
            sub_6FB202B0(D2LANG_GetStringByTblIndex(0x0D7A), wszChargeLine, nCharges);
            sub_6FB202B0(wszChargeLine, wszLines[nLineCount++], nMaxCharges);
        }
        else if (nSkillLevel > 0)
        {
            sub_6FB202B0(D2LANG_GetStringByTblIndex(0x0DD4), wszLines[nLineCount++], nSkillLevel);
        }
    }

    if (pSkillDesc->wStrShort)
    {
        wchar_t wszShort[512];
        wcscpy(wszShort, D2LANG_GetStringByTblIndex(pSkillDesc->wStrShort));
        wchar_t* pCur = wszShort;
        while (pCur && *pCur)
        {
            wchar_t* pNext = wcschr(pCur, L'\n');
            if (pNext)
                *pNext++ = L'\0';
            wcscpy(wszLines[nLineCount++], pCur);
            pCur = pNext;
        }
    }

    if (pSkillDesc->wStrName)
        wcscpy(wszLines[nLineCount++], D2LANG_GetStringByTblIndex(pSkillDesc->wStrName));

    wchar_t wszFull[2048] = {};
    for (int32_t i = nLineCount - 1; i >= 0; --i)
    {
        wcscat(wszFull, wszLines[i]);
        wcscat(wszFull, L"\n");
    }

    D2WIN_SetFont(1);
    int32_t nX = g_nSpeedBarSkillX_6FBC2598[nHoverIdx];
    int32_t nY = g_nSpeedBarSkillY_6FBC24A8[nHoverIdx] - 48;
    D2WIN_DrawFramedText(wszFull, nX, nY, 0, g_bSkillHoverIsRight_6FBC21C0 ? 2 : 0);
}

// 1.12a: 0x6FAFA030
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFA030(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF61B0(nSkillId, pUnit, nSkillLevel, pWeapon, pMinDmg, pMaxDmg, pColor, 1, nElemType);
}

// 1.12a: 0x6FAFA1E0
// Calling convention: __fastcall (ECX = pUnit, EDX = pMinDmg, [esp+4] = pMaxDmg, [esp+8] = pColor)
int32_t __fastcall sub_6FAFA1E0(D2UnitStrc* pUnit, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor)
{
    if (!pUnit || !UNITS_CanDualWield(pUnit))
        return 0;

    D2UnitStrc* pSecondaryWeapon = INVENTORY_GetSecondaryWeapon(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF61B0(0, pUnit, 1, pSecondaryWeapon, pMinDmg, pMaxDmg, pColor, 0, 0);
}

// 1.12a: 0x6FAFA210
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFA210(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF61B0(nSkillId, pUnit, nSkillLevel, pWeapon, pMinDmg, pMaxDmg, pColor, 1, nElemType);
}

// 1.12a: 0x6FAFA260
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFA260(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF61B0(nSkillId, pUnit, nSkillLevel, pWeapon, pMinDmg, pMaxDmg, pColor, 0, nElemType);
}

// 1.12a: 0x6FAFA580
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFA580(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    *pMinDmg = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    *pMaxDmg = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    *pColor = 0;
    return 2;
}

// 1.12a: 0x6FAFA620
// Calling convention: __usercall (EAX = pUnit, ECX = nSkillId, EDX = nSkillLevel, [esp+4] = pMinOut, [esp+8] = pMaxOut)
void __fastcall sub_6FAFA620(int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pUnit, int32_t* pMinOut, int32_t* pMaxOut)
{
    int32_t nStr = STATLIST_UnitGetStatValue(pUnit, 0, 0);
    int32_t nDex = STATLIST_UnitGetStatValue(pUnit, 2, 0);
    int32_t nBaseMin = (nDex + nStr - 20) / 4;
    int32_t nBaseMax = (nDex + nStr - 20) / 3;
    if (nBaseMin < 1)
        nBaseMin = 1;
    if (nBaseMax < 1)
        nBaseMax = 1;

    int32_t nBootMin = 0, nBootMax = 0;
    if (pUnit && *(uint32_t*)pUnit == 0 && *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60))
    {
        D2UnitStrc* pBoots = INVENTORY_GetItemFromBodyLoc(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60), 9);
        if (pBoots)
        {
            uint8_t* pItemTxt = (uint8_t*)DATATBLS_GetItemsTxtRecord(*(uint32_t*)((uint8_t*)pBoots + 4));
            if (pItemTxt)
            {
                nBootMin = pItemTxt[0x132];
                nBootMax = pItemTxt[0x133];
            }
        }
    }

    int32_t nSkillPct = 0;
    uint8_t* pSkillsTxt = (uint8_t*)sub_6FAFF7E0(nSkillId);
    if (pSkillsTxt)
        nSkillPct = SKILLS_EvaluateSkillFormula(pUnit, *(uint32_t*)(pSkillsTxt + 0x13C), nSkillId, nSkillLevel);

    int32_t nStatBonusPct = STATLIST_UnitGetStatValue(pUnit, 0x19, 0);
    int32_t nMin = nBaseMin + nBaseMin * (nStr * 120 / 100 + nStatBonusPct + nSkillPct - 20) / 100
                 + nBootMin + nBootMin * (nStr * 120 / 100 + nStatBonusPct + nSkillPct) / 100;
    int32_t nMax = nBaseMax + nBaseMax * (nStr * 120 / 100 + nStatBonusPct + nSkillPct - 20) / 100
                 + nBootMax + nBootMax * (nStr * 120 / 100 + nStatBonusPct + nSkillPct) / 100;

    if (nMin < 1)
        nMin = 1;
    if (nMax <= nMin)
        nMax = nMin + 1;

    *pMinOut = nMin;
    *pMaxOut = nMax;
}

// 1.12a: 0x6FAFA8A0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFA8A0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2InventoryStrc* pInventory = *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60);
    D2UnitStrc* pLeftWeapon = INVENTORY_GetLeftHandWeapon(pInventory);
    (void)pLeftWeapon;
    return sub_6FAF6A20(pUnit, nSkillId, nSkillLevel, pMinDmg, pMaxDmg, pColor, nElemType);
}

// 1.12a: 0x6FAFA930
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFA930(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    sub_6FAF61B0(0, pUnit, 1, pWeapon, pMinDmg, pMaxDmg, pColor, 1, nElemType);
    *pMinDmg += SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    *pMaxDmg += SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    return 2;
}

// 1.12a: 0x6FAFA9F0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFA9F0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    int32_t nMinKick = 0, nMaxKick = 0;
    sub_6FAFA620(nSkillId, nSkillLevel, pUnit, &nMinKick, &nMaxKick);
    *pMinDmg = nMinKick << 8;
    *pMaxDmg = nMaxKick << 8;
    *pColor = 0;
    return 2;
}

// 1.12a: 0x6FAFAB40
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFAB40(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF61B0(nSkillId, pUnit, nSkillLevel, pWeapon, pMinDmg, pMaxDmg, pColor, 0, nElemType);
}

// 1.12a: 0x6FAFAD40
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFAD40(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    *pMinDmg = 0;
    *pMaxDmg = 0;
    *pColor = 0;
    return 3;
}

// 1.12a: 0x6FAFAD90
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFAD90(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF61B0(nSkillId, pUnit, nSkillLevel, pWeapon, pMinDmg, pMaxDmg, pColor, 0, nElemType);
}

// 1.12a: 0x6FAFAFB0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFAFB0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF61B0(nSkillId, pUnit, nSkillLevel, pWeapon, pMinDmg, pMaxDmg, pColor, 0, nElemType);
}

// 1.12a: 0x6FAFB080
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFB080(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF61B0(nSkillId, pUnit, nSkillLevel, pWeapon, pMinDmg, pMaxDmg, pColor, 0, nElemType);
}

// 1.12a: 0x6FAFB1B0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFB1B0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    *pMinDmg = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    *pMaxDmg = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    *pColor = 0;

    D2SkillDescTxt* pSkillDesc = sub_6FAF21F0(nSkillId);
    if (pSkillDesc)
    {
        if (pSkillDesc->dwDamCalc[0])
        {
            int32_t nPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDamCalc[0], nSkillId, nSkillLevel);
            if (nPct)
            {
                *pMinDmg += *pMinDmg * nPct / 100;
                *pMaxDmg += *pMaxDmg * nPct / 100;
            }
        }
        if (pSkillDesc->dwDamCalc[1])
        {
            int32_t nBonus = SKILLS_EvaluateSkillFormula(pUnit, pSkillDesc->dwDamCalc[1], nSkillId, nSkillLevel) << 8;
            *pMinDmg += nBonus;
            *pMaxDmg += nBonus;
        }
    }
    return 2;
}

// 1.12a: 0x6FAFB2E0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFB2E0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2InventoryStrc* pInventory = *(D2InventoryStrc**)((uint8_t*)pUnit + 0x60);
    D2UnitStrc* pLeftWeapon = INVENTORY_GetLeftHandWeapon(pInventory);
    return sub_6FAF61B0(nSkillId, pUnit, nSkillLevel, pLeftWeapon, pMinDmg, pMaxDmg, pColor, 0, nElemType);
}

// 1.12a: 0x6FAFB3B0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFB3B0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF61B0(nSkillId, pUnit, nSkillLevel, pWeapon, pMinDmg, pMaxDmg, pColor, 0, nElemType);
}

// 1.12a: 0x6FAFB480
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFB480(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF61B0(nSkillId, pUnit, nSkillLevel, pWeapon, pMinDmg, pMaxDmg, pColor, 0, nElemType);
}

// ============================================================================
// Reconstructed Functions: UI_spellsel.asm (0x6FB074E0 .. 0x6FB076B0)
// ============================================================================

// 1.12a: 0x6FB074E0
// Calling convention: __stdcall
BOOL __stdcall sub_6FB074E0(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (!pUnit)
        return FALSE;
    for (D2SkillStrc* pSkill = SKILLS_GetFirstSkillFromSkillList(*(void**)((uint8_t*)pUnit + 0xA8)); pSkill; pSkill = SKILLS_GetNextSkill(pSkill))
    {
        if (SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__) == nSkillId && SKILLS_GetSkillLevel(pUnit, pSkill, 1) > 0)
            return TRUE;
    }
    return FALSE;
}

// 1.12a: 0x6FB07530
// Calling convention: __stdcall
BOOL __stdcall sub_6FB07530(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (!pUnit)
        return FALSE;
    for (D2SkillStrc* pSkill = SKILLS_GetFirstSkillFromSkillList(*(void**)((uint8_t*)pUnit + 0xA8)); pSkill; pSkill = SKILLS_GetNextSkill(pSkill))
    {
        if (SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__) == nSkillId && SKILLS_GetSkillLevel(pUnit, pSkill, 1) > 0)
            return TRUE;
    }
    return FALSE;
}

// 1.12a: 0x6FB07580
// Calling convention: __stdcall
BOOL __stdcall sub_6FB07580(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (!pUnit)
        return FALSE;
    for (D2SkillStrc* pSkill = SKILLS_GetFirstSkillFromSkillList(*(void**)((uint8_t*)pUnit + 0xA8)); pSkill; pSkill = SKILLS_GetNextSkill(pSkill))
    {
        if (SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__) == nSkillId && SKILLS_GetSkillLevel(pUnit, pSkill, 1) > 0)
            return TRUE;
    }
    return FALSE;
}

// 1.12a: 0x6FB075D0
// Calling convention: __stdcall
BOOL __stdcall sub_6FB075D0(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (!pUnit)
        return FALSE;
    for (D2SkillStrc* pSkill = SKILLS_GetFirstSkillFromSkillList(*(void**)((uint8_t*)pUnit + 0xA8)); pSkill; pSkill = SKILLS_GetNextSkill(pSkill))
    {
        if (SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__) == nSkillId && SKILLS_GetSkillLevel(pUnit, pSkill, 1) > 0)
            return TRUE;
    }
    return FALSE;
}

// 1.12a: 0x6FB07620
// Calling convention: __cdecl
void __cdecl sub_6FB07620(void)
{
    int32_t nIdx = g_nSpeedBarHoverIdx_6FBC21CC;
    if (nIdx >= 0 && nIdx < g_nSpeedBarCount_6FBC21C8)
        sub_6FAF9A90(g_nSpeedBarSkillIds_6FBC21D8[nIdx], nIdx);
}

// ============================================================================
// Additional Unicode & Damage Helpers from UI_SkillDesc.asm
// ============================================================================
#if 0
// 1.12a: 0x6FAF2590
// Calling convention: __usercall (EAX = nMaxFrac, ECX = nMaxInt, EDX = nMinFrac, ESI = nMinInt, [esp+4] = pOutUnicode, [esp+8] = bPlus)
void __fastcall sub_6FAF2590(int32_t nMaxInt, int32_t nMinFrac, int32_t nMaxFrac, int32_t nMinInt, wchar_t* pOutUnicode, int32_t bPlus)
{
    if (bPlus)
        swprintf(pOutUnicode, L"+%d.%d-%d.%d", nMinInt, nMinFrac, nMaxInt, nMaxFrac);
    else
        swprintf(pOutUnicode, L"%d.%d-%d.%d", nMinInt, nMinFrac, nMaxInt, nMaxFrac);
}

// 1.12a: 0x6FAF25E0
// Calling convention: __usercall (EAX = nMax, ECX = bPlus, EDX = bAllowK, ESI = nMin, [esp+4] = pOutUnicode)
void __fastcall sub_6FAF25E0(int32_t bPlus, int32_t bAllowK, int32_t nMax, int32_t nMin, wchar_t* pOutUnicode)
{
    const wchar_t* wszPrefix = bPlus ? L"+" : L"";
    if (bAllowK && nMin >= 10000)
        swprintf(pOutUnicode, L"%s%dK-%dK", wszPrefix, (nMin + 500) / 1000, (nMax + 500) / 1000);
    else if (bAllowK && nMax >= 10000)
        swprintf(pOutUnicode, L"%s%d-%dK", wszPrefix, nMin, (nMax + 500) / 1000);
    else
        swprintf(pOutUnicode, L"%s%d-%d", wszPrefix, nMin, nMax);
}

// 1.12a: 0x6FAF26D0
// Calling convention: __usercall (EAX = nInt, ECX = nFrac, EDX = bPlus, [esp+4] = pOutUnicode)
int32_t __fastcall sub_6FAF26D0(int32_t nFrac, int32_t bPlus, int32_t nInt, wchar_t* pOutUnicode)
{
    if (bPlus && nInt >= 0)
        swprintf(pOutUnicode, L"+%d.%d", nInt, nFrac);
    else
        swprintf(pOutUnicode, L"%d.%d", nInt, nFrac);
    return 1;
}

// 1.12a: 0x6FAF2770
// Calling convention: __usercall (EAX = nElemType)
int32_t __fastcall sub_6FAF2770(int32_t nElemType)
{
    switch (nElemType)
    {
    case 1: return 1;
    case 2: return 9;
    case 3: return 3;
    case 4: return 3;
    case 5: return 2;
    default: return 0;
    }
}

// 1.12a: 0x6FAF27B0
// Calling convention: __usercall (EAX = nElemType)
uint16_t __fastcall sub_6FAF27B0(int32_t nElemType)
{
    switch (nElemType)
    {
    case 1: return 0x10A1;
    case 2: return 0x10A3;
    case 3: return 0x10A2;
    case 4: return 0x10C3;
    case 5: return 0x10A4;
    case 6: return 0x0DC4;
    default: return 0;
    }
}

// 1.12a: 0x6FAF2800
// Calling convention: __usercall (EBX = pUnit)
int32_t __fastcall sub_6FAF2800(D2UnitStrc* pUnit)
{
    extern int32_t g_bHasClassSkills_6FBD3670;
    if (g_bHasClassSkills_6FBD3670)
        return 1;
    if (!pUnit)
        return 0;
    g_bHasClassSkills_6FBD3670 = 1;
    return 1;
}

// 1.12a: 0x6FAF2930
// Calling convention: __usercall (EBX = wszOut, [esp+4] = wStrId, [esp+8] = nValue)
int32_t __stdcall sub_6FAF2930(uint16_t wStrId, int32_t nValue, wchar_t* wszOut)
{
    if (wStrId == 0x1506)
        return 0;
    wchar_t wszTemp[256];
    swprintf(wszTemp, D2LANG_GetStringByTblIndex(wStrId), nValue);
    wcscat(wszOut, wszTemp);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF2AA0
// Calling convention: __usercall (EDI = wszOut, [esp+4] = wStrA, [esp+8] = wStrB)
int32_t __stdcall sub_6FAF2AA0(uint16_t wStrA, uint16_t wStrB, wchar_t* wszOut)
{
    if (wStrA == 0x1506 || wStrB == 0x1506)
        return 0;
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrA));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9D));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9B));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrB));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF2B40
// Calling convention: __usercall (EDI = wszOut, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = nNum, [esp+16] = nDenom, [esp+20] = bPlus)
int32_t __stdcall sub_6FAF2B40(uint16_t wStrA, uint16_t wStrB, int32_t nNum, int32_t nDenom, int32_t bPlus, wchar_t* wszOut)
{
    if (wStrA == 0x1506 || nNum == 0 || nDenom <= 0)
        return 0;
    wchar_t wszRatio[64];
    swprintf(wszRatio, bPlus ? L"+%d/%d " : L"%d/%d ", nNum, nDenom);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrA));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9D));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9B));
    wcscat(wszOut, wszRatio);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrB));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF2C60
// Calling convention: __usercall (EBX = wStrA, EDI = wszOut, [esp+4] = wStrB, [esp+8] = nInt, [esp+12] = nFrac, [esp+16] = bPlus, [esp+20] = bPercent)
int32_t __stdcall sub_6FAF2C60(uint16_t wStrA, uint16_t wStrB, int32_t nInt, int32_t nFrac, int32_t bPlus, int32_t bPercent, wchar_t* wszOut)
{
    if (wStrA == 0x1506 || (nInt == 0 && nFrac == 0))
        return 0;
    wchar_t wszNum[32];
    sub_6FAF26D0(nFrac, bPlus, nInt, wszNum);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrA));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9D));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9B));
    wcscat(wszOut, wszNum);
    if (bPercent)
        wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0FA1));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9B));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrB));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF2D50
// Calling convention: __usercall (EDI = wszOut, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = nVal, [esp+16] = bPlus, [esp+20] = bPercent)
int32_t __stdcall sub_6FAF2D50(uint16_t wStrA, uint16_t wStrB, int32_t nVal, int32_t bPlus, int32_t bPercent, wchar_t* wszOut)
{
    if (!nVal)
        return 0;
    wchar_t wszNum[32];
    swprintf(wszNum, (bPlus && nVal >= 0) ? L"+%d" : L"%d", nVal);
    if (wStrA != 0x1506)
    {
        wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrA));
        wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9D));
        wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9B));
    }
    wcscat(wszOut, wszNum);
    if (bPercent)
        wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0FA1));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9B));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrB));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF2E60
// Calling convention: __usercall (EBX = wszOut, [esp+4] = nMin, [esp+8] = nMax, [esp+12] = wStrElem)
int32_t __stdcall sub_6FAF2E60(int32_t nMin, int32_t nMax, uint16_t wStrElem, wchar_t* wszOut)
{
    if (nMin == 0 && nMax == 0)
        return 0;
    wchar_t wszRange[32];
    if (nMin == nMax)
        swprintf(wszRange, L"%d", nMin);
    else
        swprintf(wszRange, L"%d-%d", nMin, nMax);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x10E2));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrElem));
    wcscat(wszOut, wszRange);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x10BE));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF2F40
// Calling convention: __usercall (EAX = nSkillLevel, EBX = nPctBonus, [esp+4] = pUnit, [esp+8] = nSkillId, [esp+12] = wszOut, [esp+16] = nFlatBonus)
int32_t __stdcall sub_6FAF2F40(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t nPctBonus, int32_t nFlatBonus, wchar_t* wszOut)
{
    int32_t nMin = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 0) >> 8;
    int32_t nMax = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 0) >> 8;
    nMin += nMin * nPctBonus / 100 + nFlatBonus;
    nMax += nMax * nPctBonus / 100 + nFlatBonus;
    if (nMin == 0 && nMax == 0)
        return 0;
    wchar_t wszRange[32];
    if (nMin == nMax)
        swprintf(wszRange, (nMin >= 0) ? L"+%d" : L"%d", nMin);
    else
        swprintf(wszRange, L"%d-%d", nMin, nMax);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x10A0));
    wcscat(wszOut, wszRange);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF3160
// Calling convention: __usercall (EAX = wStrCustom, [esp+4] = wszOut, [esp+8] = nMin, [esp+12] = nMax, [esp+16] = nElemType)
int32_t __stdcall sub_6FAF3160(uint16_t wStrCustom, wchar_t* wszOut, int32_t nMin, int32_t nMax, int32_t nElemType)
{
    uint16_t wStr = wStrCustom ? wStrCustom : sub_6FAF27B0(nElemType);
    if (!wStr)
        return 0;
    wchar_t wszRange[32];
    if (nMin == nMax)
        swprintf(wszRange, L"%d", nMin);
    else
        swprintf(wszRange, L"%d-%d", nMin, nMax);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStr));
    wcscat(wszOut, wszRange);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF3440
// Calling convention: __usercall (EAX = pUnit, [esp+4..40] = dual-wield callback wrapper args)
int32_t __stdcall sub_6FAF3440(D2UnitStrc* pUnit, int32_t* pVal1A, int32_t* pVal1B, int32_t* pVal1C, int32_t* pVal2A, int32_t* pVal2B, int32_t nArg1, int32_t nArg2, void* pfnCallback, int32_t nArg3, int32_t nArg4)
{
    if (!pUnit || !*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60) || !UNITS_CanDualWield(pUnit))
        return 0;
    return 1;
}

// 1.12a: 0x6FAF38F0
// Calling convention: __usercall (EBX = wszOut, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = nVal, [esp+16] = bPlus)
int32_t __stdcall sub_6FAF38F0(uint16_t wStrA, uint16_t wStrB, int32_t nVal, int32_t bPlus, wchar_t* wszOut)
{
    if (!nVal)
        return 0;
    wchar_t wszNum[32];
    swprintf(wszNum, (bPlus && nVal >= 0) ? L"+%d" : L"%d", nVal);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrA));
    wcscat(wszOut, wszNum);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrB));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF39B0
// Calling convention: __usercall (EBX = nVal, [esp+4] = wszOut, [esp+8] = wStrSingular, [esp+12] = wStrPlural, [esp+16] = bPlus)
int32_t __stdcall sub_6FAF39B0(int32_t nVal, wchar_t* wszOut, uint16_t wStrSingular, uint16_t wStrPlural, int32_t bPlus)
{
    if (!nVal)
        return 0;
    wchar_t wszNum[32];
    swprintf(wszNum, (bPlus && nVal >= 0) ? L"+%d" : L"%d", nVal);
    wcscat(wszOut, wszNum);
    wcscat(wszOut, D2LANG_GetStringByTblIndex((nVal == 1) ? wStrSingular : wStrPlural));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF3B30
// Calling convention: __usercall (EBX = wszOut, [esp+4] = wStr, [esp+8] = nVal, [esp+12] = bPlus)
int32_t __stdcall sub_6FAF3B30(uint16_t wStr, int32_t nVal, int32_t bPlus, wchar_t* wszOut)
{
    if (!nVal)
        return 0;
    wchar_t wszNum[32];
    swprintf(wszNum, (bPlus && nVal >= 0) ? L"+%d" : L"%d", nVal);
    wcscat(wszOut, wszNum);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStr));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF3BE0
// Calling convention: __usercall (EBX = wszOut, [esp+4] = wStr, [esp+8] = nVal, [esp+12] = bPlus)
int32_t __stdcall sub_6FAF3BE0(uint16_t wStr, int32_t nVal, int32_t bPlus, wchar_t* wszOut)
{
    if (!nVal)
        return 0;
    wchar_t wszNum[32];
    swprintf(wszNum, (bPlus && nVal >= 0) ? L"+%d" : L"%d", nVal);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStr));
    wcscat(wszOut, wszNum);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF3C90
// Calling convention: __usercall (EBX = wszOut, [esp+4] = wStr, [esp+8] = wszArg)
int32_t __stdcall sub_6FAF3C90(uint16_t wStr, const wchar_t* wszArg, wchar_t* wszOut)
{
    if (wStr == 0x1506)
    {
        wcscat(wszOut, D2LANG_GetStringByTblIndex(0x1506));
        wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
        return 1;
    }
    wchar_t wszTemp[256];
    swprintf(wszTemp, D2LANG_GetStringByTblIndex(wStr), wszArg);
    wcscat(wszTemp, D2LANG_GetStringByTblIndex(0x0F9E));
    wcscat(wszOut, wszTemp);
    return 1;
}

// 1.12a: 0x6FAF3D40
// Calling convention: __usercall (EBX = nFrames, [esp+4] = wszOut, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = nMinFixed, [esp+20] = nMaxFixed)
int32_t __stdcall sub_6FAF3D40(int32_t nFrames, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB, int32_t nMinFixed, int32_t nMaxFixed)
{
    int32_t nMinInt = nMinFixed / 256;
    int32_t nMinFrac = ((nMinFixed % 256) * nFrames) / 256;
    int32_t nMaxInt = nMaxFixed / 256;
    int32_t nMaxFrac = ((nMaxFixed % 256) * nFrames) / 256;
    if (nMinInt == 0 && nMaxInt == 0 && nMinFrac == 0 && nMaxFrac == 0)
        return 0;
    wchar_t wszRange[32];
    if (nMinFrac == 0 && nMaxFrac == 0)
        swprintf(wszRange, L"%d-%d", nMinInt, nMaxInt);
    else
        swprintf(wszRange, L"%d.%d-%d.%d", nMinInt, nMinFrac, nMaxInt, nMaxFrac);
    if (wStrA != 0x1506)
        wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrA));
    wcscat(wszOut, wszRange);
    if (wStrB != 0x1506)
        wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrB));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF3E90
// Calling convention: __usercall (EBX = wszOut, [esp+4] = wStrA, [esp+8] = nMinFrames, [esp+12] = nMaxFrames)
int32_t __stdcall sub_6FAF3E90(uint16_t wStrA, int32_t nMinFrames, int32_t nMaxFrames, wchar_t* wszOut)
{
    int32_t nMinSec = nMinFrames / 25;
    int32_t nMinDec = ((nMinFrames % 25) * 10) / 25;
    int32_t nMaxSec = nMaxFrames / 25;
    int32_t nMaxDec = ((nMaxFrames % 25) * 10) / 25;
    if (nMinSec == 0 && nMinDec == 0 && nMaxSec == 0)
        return 0;
    wchar_t wszRange[32];
    if (nMinDec == 0 && nMaxDec == 0)
        swprintf(wszRange, L"%d-%d", nMinSec, nMaxSec);
    else
        swprintf(wszRange, L"%d.%d-%d.%d", nMinSec, nMinDec, nMaxSec, nMaxDec);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrA));
    wcscat(wszOut, wszRange);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x10AC));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF3FA0
// Calling convention: __usercall (EAX = bPlus, [esp+4] = wszOut, [esp+8] = wStrA, [esp+12] = nFrames)
int32_t __stdcall sub_6FAF3FA0(int32_t bPlus, wchar_t* wszOut, uint16_t wStrA, int32_t nFrames)
{
    int32_t nSec = nFrames / 25;
    int32_t nDec = ((nFrames % 25) * 10) / 25;
    if (nSec == 0 && nDec == 0)
        return 0;
    wchar_t wszTime[32];
    if (nDec != 0)
        sub_6FAF26D0(nDec, bPlus, nSec, wszTime);
    else
        sub_6FAF2720(nSec, bPlus, wszTime);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrA));
    wcscat(wszOut, wszTime);
    wcscat(wszOut, D2LANG_GetStringByTblIndex((nSec == 1 && nDec == 0) ? 0x10AB : 0x10AC));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF4080
// Calling convention: __usercall (EAX = nMax, ECX = bPlus, EDX = wszOut, [esp+4] = nMin)
int32_t __fastcall sub_6FAF4080(int32_t bPlus, wchar_t* wszOut, int32_t nMax, int32_t nMin)
{
    if (nMin == nMax)
        sub_6FAF2720(nMin, bPlus, wszOut);
    else
        sub_6FAF25E0(bPlus, 1, nMax, nMin, wszOut);
    return 1;
}

// 1.12a: 0x6FAF40B0
// Calling convention: __usercall (EAX = nInt, ECX = nFrac, EDX = bPlus, ESI = wszOut)
void __fastcall sub_6FAF40B0(int32_t nFrac, int32_t bPlus, int32_t nInt, wchar_t* wszOut)
{
    if (nInt == 0 && nFrac == 0)
        return;
    if (nFrac != 0)
        sub_6FAF26D0(nFrac, bPlus, nInt, wszOut);
    else
        sub_6FAF2720(nInt, bPlus, wszOut);
}

// 1.12a: 0x6FAF40D0
// Calling convention: __usercall (EAX = pSkillsTxt, EBX = nSkillLevel, [esp+4] = pUnit, [esp+8] = pMinOut, [esp+12] = pMaxOut, [esp+16] = pColorOut)
void __stdcall sub_6FAF40D0(uint8_t* pSkillsTxt, int32_t nSkillLevel, D2UnitStrc* pUnit, int32_t* pMinOut, int32_t* pMaxOut, int32_t* pColorOut)
{
    int16_t nSkillId = *(int16_t*)pSkillsTxt;
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1) >> 8;
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1) >> 8;
    if (pSkillsTxt[0x1DC] == 5)
    {
        int32_t nLen = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);
        nMin = (nMin * nLen) >> 8;
        nMax = (nMax * nLen) >> 8;
        if (pColorOut)
            *pColorOut = 2;
    }
    else if (pColorOut)
    {
        *pColorOut = sub_6FAF2770(pSkillsTxt[0x1DC]);
    }
    *pMinOut += nMin;
    *pMaxOut += nMax;
}

// 1.12a: 0x6FAF41B0
// Calling convention: __usercall (ECX = pSkillsTxt, EBX = pMaxInOut, [esp+4] = pMinInOut)
void __fastcall sub_6FAF41B0(uint8_t* pSkillsTxt, int32_t* pMaxInOut, int32_t* pMinInOut)
{
    D2UnitStrc* pPlayer = sub_6FAFF3D0();
    if (pSkillsTxt && pSkillsTxt[0x1DC] == 2)
    {
        int32_t nLtngMastery = STATLIST_UnitGetStatValue(pPlayer, 330, 0);
        if (nLtngMastery)
        {
            *pMinInOut += *pMinInOut * nLtngMastery / 100;
            *pMaxInOut += *pMaxInOut * nLtngMastery / 100;
        }
    }
}

// 1.12a: 0x6FAF4210
// Calling convention: __usercall (ECX = pSkillsTxt, EBX = pMaxInOut, [esp+4] = pMinInOut)
void __fastcall sub_6FAF4210(uint8_t* pSkillsTxt, int32_t* pMaxInOut, int32_t* pMinInOut)
{
    D2UnitStrc* pPlayer = sub_6FAFF3D0();
    if (pSkillsTxt && pSkillsTxt[0x1DC] == 1)
    {
        int32_t nFireMastery = STATLIST_UnitGetStatValue(pPlayer, 329, 0);
        if (nFireMastery)
        {
            *pMinInOut += *pMinInOut * nFireMastery / 100;
            *pMaxInOut += *pMaxInOut * nFireMastery / 100;
        }
    }
}

// 1.12a: 0x6FAF4270
// Calling convention: __usercall (ESI = nValue)
int32_t __fastcall sub_6FAF4270(int32_t nValue)
{
    uint8_t* pDiffTxt = (uint8_t*)DATATBLS_GetDifficultyLevelsTxtRecord(sub_6FAFF090());
    int32_t nDiv = pDiffTxt ? *(int32_t*)(pDiffTxt + 0x1C) : 0;
    return (nDiv > 0) ? (nValue / nDiv) : nValue;
}

// 1.12a: 0x6FAF4350
// Calling convention: __stdcall
int32_t __stdcall sub_6FAF4350(D2UnitStrc* pUnit, int32_t nSkillId, uint8_t* pSkillsTxt, int32_t nSkillLevel, wchar_t* wszOut)
{
    int16_t nMonId = *(int16_t*)(pSkillsTxt + 0xBC);
    D2MonStatInitStrc monStatInit = {};
    if (nMonId < 0 || !sub_6FAF2DD0(nMonId, 1, &monStatInit))
        return 0;
    return sub_6FAF3160(0x10A0, wszOut, monStatInit.nA1MinD, monStatInit.nA1MaxD, 0);
}

// 1.12a: 0x6FAF46B0
// Calling convention: __stdcall
int32_t __stdcall sub_6FAF46B0(wchar_t* wszOut, uint8_t* pSkillsTxt, int32_t nBonusMin, int32_t nBonusMax)
{
    int16_t nMonId = *(int16_t*)(pSkillsTxt + 0xBC);
    D2MonStatInitStrc monStatInit = {};
    if (nMonId < 0 || !sub_6FAF2DD0(nMonId, 1, &monStatInit))
        return 0;
    return sub_6FAF3160(0, wszOut, monStatInit.nA1MinD + nBonusMin, monStatInit.nA1MaxD + nBonusMax, pSkillsTxt[0x1DC]);
}

// 1.12a: 0x6FAF4840
// Calling convention: __stdcall
int32_t __stdcall sub_6FAF4840(D2UnitStrc* pUnit, wchar_t* wszOut, int32_t nSkillId, uint8_t* pSkillsTxt, int32_t nSkillLevel)
{
    int16_t nMonId = *(int16_t*)(pSkillsTxt + 0xBC);
    D2MonStatInitStrc monStatInit = {};
    if (nMonId < 0 || !sub_6FAF2DD0(nMonId, 1, &monStatInit))
        return 0;
    return sub_6FAF3160(0x10A0, wszOut, monStatInit.nA1MinD, monStatInit.nA1MaxD, 0);
}

// 1.12a: 0x6FAF4AC0
// Calling convention: __usercall (EAX = pUnit, EDI = nSkillId, ESI = nSkillLevel, [esp+4] = wszOut, [esp+8] = pSkillsTxt)
int32_t __stdcall sub_6FAF4AC0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, wchar_t* wszOut, uint8_t* pSkillsTxt)
{
    int32_t nMin = (SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1) * 25) >> 8;
    int32_t nMax = (SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1) * 25) >> 8;
    uint16_t wStrElem = sub_6FAF27B0(pSkillsTxt[0x1DC]);
    return sub_6FAF2E60(nMin, nMax, wStrElem ? wStrElem : 0x10A1, wszOut);
}

// 1.12a: 0x6FAF4BA0
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, [esp+4] = pUnit, [esp+8] = wszOut, [esp+12] = pSkillsTxt)
int32_t __stdcall sub_6FAF4BA0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, wchar_t* wszOut, uint8_t* pSkillsTxt)
{
    int32_t nLen = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 0);
    int32_t nMin = (SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1) * nLen) >> 8;
    int32_t nMax = (SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1) * nLen) >> 8;
    sub_6FAF3FA0(0, wszOut, 0x2B0E, nLen);
    return sub_6FAF3160(0, wszOut, nMin, nMax, pSkillsTxt[0x1DC]);
}

// 1.12a: 0x6FAF4CC0
// Calling convention: __usercall (EAX = nSkillId, EDI = nSkillLevel, [esp+4] = pUnit, [esp+8] = wszOut)
int32_t __stdcall sub_6FAF4CC0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, wchar_t* wszOut)
{
    int32_t nMin = (SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1) * 75) >> 8;
    int32_t nMax = (SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1) * 75) >> 8;
    return sub_6FAF2E60(nMin, nMax, 0x10A1, wszOut);
}

// 1.12a: 0x6FAF4D10
// Calling convention: __usercall (EAX = nSkillLevel, ECX = pUnit, ESI = nMissileId, [esp+4] = wszOut, [esp+8] = pSkillsTxt)
int32_t __stdcall sub_6FAF4D10(D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillLevel, wchar_t* wszOut, uint8_t* pSkillsTxt)
{
    int32_t nMin = MISSILE_GetMinElemDamage(0, pUnit, nMissileId, nSkillLevel) * 75;
    int32_t nMax = MISSILE_GetMaxElemDamage(0, pUnit, nMissileId, nSkillLevel) * 75;
    sub_6FAF4210(pSkillsTxt, &nMax, &nMin);
    sub_6FAF41B0(pSkillsTxt, &nMax, &nMin);
    uint8_t nElemType = MISSILE_GetElemTypeFromMissileId(nMissileId);
    uint16_t wStrElem = sub_6FAF27B0(nElemType);
    return sub_6FAF2E60(nMin >> 8, nMax >> 8, wStrElem ? wStrElem : 0x10A1, wszOut);
}

// 1.12a: 0x6FAF4E30
// Calling convention: __usercall (EAX = nSkillLevel, ECX = nSkillId, EDX = pUnit, [esp+4] = wszOut)
int32_t __stdcall sub_6FAF4E30(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, wchar_t* wszOut)
{
    int32_t nToHit = SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel);
    if (!nToHit)
        return 0;
    return sub_6FAF38F0(0x10A6, 0x10B3, nToHit, 1, wszOut);
}

// 1.12a: 0x6FAF4E60
// Calling convention: __usercall (EAX = nSkillLevel, EBX = nMissileId, [esp+4] = pUnit, [esp+8] = wszOut, [esp+12] = wStrCustom)
int32_t __stdcall sub_6FAF4E60(D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSkillLevel, wchar_t* wszOut, uint16_t wStrCustom)
{
    int32_t nMin = MISSILE_GetMinElemDamage(0, pUnit, nMissileId, nSkillLevel) >> 8;
    int32_t nMax = MISSILE_GetMaxElemDamage(0, pUnit, nMissileId, nSkillLevel) >> 8;
    if (nMin == 0 && nMax == 0)
        return 0;
    uint8_t nElemType = MISSILE_GetElemTypeFromMissileId(nMissileId);
    return sub_6FAF3160(wStrCustom, wszOut, nMin, nMax, nElemType);
}

// 1.12a: 0x6FAF4EC0
// Calling convention: __usercall (EBX = nSkillId, EDI = nSkillLevel, [esp+4] = pUnit, [esp+8] = wszOut, [esp+12] = pSkillsTxt, [esp+16] = wStrCustom)
int32_t __stdcall sub_6FAF4EC0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, wchar_t* wszOut, uint8_t* pSkillsTxt, uint16_t wStrCustom)
{
    int32_t nMin = SKILLS_GetElementalMinDamage(pUnit, nSkillId, nSkillLevel, 1);
    int32_t nMax = SKILLS_GetElementalMaxDamage(pUnit, nSkillId, nSkillLevel, 1);
    if (nMin == 0 && nMax == 0)
        return 0;
    return sub_6FAF3160(wStrCustom, wszOut, nMin >> 8, nMax >> 8, pSkillsTxt[0x1DC]);
}

// 1.12a: 0x6FAF5280
// Calling convention: __usercall (EAX = nSubTiles, [esp+4] = wszOut, [esp+8] = wStrPrefix)
int32_t __stdcall sub_6FAF5280(int32_t nSubTiles, wchar_t* wszOut, uint16_t wStrPrefix)
{
    int32_t nYardsX10 = (nSubTiles * 10) / 3;
    int32_t nYardsInt = nYardsX10 / 10;
    int32_t nYardsDec = nYardsX10 % 10;
    if (nYardsInt < 1)
        nYardsInt = 1;
    wchar_t wszNum[32];
    if (nYardsDec != 0)
        swprintf(wszNum, L"%d.%d", nYardsInt, nYardsDec);
    else
        swprintf(wszNum, L"%d", nYardsInt);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrPrefix));
    wcscat(wszOut, wszNum);
    wcscat(wszOut, D2LANG_GetStringByTblIndex((nYardsInt == 1) ? 0x10C0 : 0x10B6));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF5370
// Calling convention: __usercall (EAX = nSubTiles, [esp+4] = wszOut, [esp+8] = wStrPrefix)
int32_t __stdcall sub_6FAF5370(int32_t nSubTiles, wchar_t* wszOut, uint16_t wStrPrefix)
{
    int32_t nYardsX10 = (nSubTiles * 20) / 3;
    int32_t nYardsInt = nYardsX10 / 10;
    int32_t nYardsDec = nYardsX10 % 10;
    if (nYardsInt < 1)
        nYardsInt = 1;
    wchar_t wszNum[32];
    if (nYardsDec != 0)
        swprintf(wszNum, L"%d.%d", nYardsInt, nYardsDec);
    else
        swprintf(wszNum, L"%d", nYardsInt);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrPrefix));
    wcscat(wszOut, wszNum);
    wcscat(wszOut, D2LANG_GetStringByTblIndex((nYardsInt == 1) ? 0x10C0 : 0x10B6));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF5460
// Calling convention: __usercall (EAX = wszOut, [esp+4] = wStrPrefix, [esp+8] = nMin, [esp+12] = nMax, [esp+16] = bPlus, [esp+20] = bSpace)
int32_t __stdcall sub_6FAF5460(wchar_t* wszOut, uint16_t wStrPrefix, int32_t nMin, int32_t nMax, int32_t bPlus, int32_t bSpace)
{
    if (nMin == nMax)
        return sub_6FAF3BE0(wStrPrefix, nMin, bPlus, wszOut);
    wchar_t wszRange[32];
    swprintf(wszRange, bPlus ? L"+%d-%d" : L"%d-%d", nMin, nMax);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrPrefix));
    if (bSpace)
        wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9B));
    wcscat(wszOut, wszRange);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF5540
// Calling convention: __usercall (EAX = wszOut, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = nMin, [esp+16] = nMax, [esp+20] = bPlus)
int32_t __stdcall sub_6FAF5540(wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB, int32_t nMin, int32_t nMax, int32_t bPlus)
{
    if (nMin == nMax)
        return sub_6FAF38F0(wStrA, wStrB, nMin, 0, wszOut);
    wchar_t wszRange[32];
    swprintf(wszRange, bPlus ? L"+%d-%d" : L"%d-%d", nMin, nMax);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrA));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrB));
    wcscat(wszOut, wszRange);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF5620
// Calling convention: __usercall (EAX = wszOut, [esp+4] = wStrA, [esp+8] = wStrB, [esp+12] = nMin, [esp+16] = nMax, [esp+20] = bPlus)
int32_t __stdcall sub_6FAF5620(wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB, int32_t nMin, int32_t nMax, int32_t bPlus)
{
    if (nMin == nMax)
        return sub_6FAF38F0(wStrA, wStrB, nMin, 0, wszOut);
    wchar_t wszRange[32];
    swprintf(wszRange, bPlus ? L"+%d-%d" : L"%d-%d", nMin, nMax);
    if (wStrA != 0x1506)
        wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrA));
    wcscat(wszOut, wszRange);
    if (wStrB != 0x1506)
        wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrB));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF5710
// Calling convention: __usercall (EBX = nFrac, [esp+4] = wszOut, [esp+8] = wStrPrefix, [esp+12] = nInt, [esp+16] = bPlus)
int32_t __stdcall sub_6FAF5710(int32_t nFrac, wchar_t* wszOut, uint16_t wStrPrefix, int32_t nInt, int32_t bPlus)
{
    if (nInt == 0 && nFrac <= 0)
        return 0;
    wchar_t wszNum[32];
    if (nFrac != 0)
        sub_6FAF26D0(nFrac, bPlus, nInt, wszNum);
    else
        sub_6FAF2720(nInt, bPlus, wszNum);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrPrefix));
    wcscat(wszOut, wszNum);
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF57B0
// Calling convention: __usercall (EBX = nFixedVal, [esp+4] = wszOut, [esp+8] = wStrA, [esp+12] = wStrB, [esp+16] = bPlus, [esp+20] = nScale)
int32_t __stdcall sub_6FAF57B0(int32_t nFixedVal, wchar_t* wszOut, uint16_t wStrA, uint16_t wStrB, int32_t bPlus, int32_t nScale)
{
    if (!nFixedVal)
        return 0;
    int32_t nInt = nFixedVal / 256;
    int32_t nFrac = ((nFixedVal % 256) * nScale) / 256;
    if (nInt == 0 && nFrac == 0)
        return 0;
    wchar_t wszNum[32];
    sub_6FAF40B0(nFrac, bPlus, nInt, wszNum);
    if (wStrA != 0x1506)
        wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrA));
    wcscat(wszOut, wszNum);
    if (wStrB != 0x1506)
        wcscat(wszOut, D2LANG_GetStringByTblIndex(wStrB));
    wcscat(wszOut, D2LANG_GetStringByTblIndex(0x0F9E));
    return 1;
}

// 1.12a: 0x6FAF58B0
// Calling convention: __usercall (ECX = pSkillDesc)
uint16_t __fastcall sub_6FAF58B0(D2SkillDescTxt* pSkillDesc)
{
    return pSkillDesc ? pSkillDesc->wStrMana : 0;
}

// 1.12a: 0x6FAF6A80
// Calling convention: __usercall (EAX = pSkillsTxt, ECX = pUnit, [esp+4..36] = dual-wield damage display args)
void __stdcall sub_6FAF6A80(uint8_t* pSkillsTxt, D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nSkillLevel, int32_t nArg4, int32_t nArg5, int32_t nXLeft, int32_t nYBase, int32_t nXRight, int32_t bAddElem)
{
    if (!pUnit || !pSkillsTxt || !nSkillLevel)
        return;
}

// 1.12a: 0x6FAF6D70
// Calling convention: __stdcall
int32_t __stdcall sub_6FAF6D70(D2UnitStrc* pUnit, int32_t* pMinOut, int32_t* pMaxOut, int32_t* pColorOut, D2SkillStrc* pSkill)
{
    for (int32_t i = 0; i < 4; ++i)
    {
        int32_t nMin = STATLIST_UnitGetStatValue(pUnit, g_aElemDamageStats_6FB85B68[i + 1].nStatMinId, 0);
        int32_t nMax = STATLIST_UnitGetStatValue(pUnit, g_aElemDamageStats_6FB85B68[i + 1].nStatMaxId, 0);
        int32_t nPctStat = g_aElemDamageStats_6FB85B68[i + 1].nElemType;
        if (nPctStat > 3)
        {
            int32_t nPct = STATLIST_UnitGetStatValue(pUnit, nPctStat, 0);
            if (nPct)
            {
                nMin += nMin * nPct / 100;
                nMax += nMax * nPct / 100;
            }
        }
        if (nMin > nMax)
            nMin = nMax;
        *pMinOut += nMin;
        *pMaxOut += nMax;
        if (nMin || nMax)
            *pColorOut = g_aElemDamageStats_6FB85B68[i].nElemType;
    }
    if (STATES_CheckStateMaskDamBlueOnUnit(pUnit))
        *pColorOut = 3;
    if (STATES_CheckStateMaskDamRedOnUnit(pUnit))
        *pColorOut = 1;
    return 1;
}

// 1.12a: 0x6FAF7160
// Calling convention: __usercall (EAX = nBonusPct, ECX = pUnit, [esp+4..32] = weapon damage calculation args)
int32_t __stdcall sub_6FAF7160(int32_t nBonusPct, D2UnitStrc* pUnit, int32_t* pMinOut, int32_t* pMaxOut, int32_t* pColorOut, D2SkillStrc* pSkill, D2UnitStrc* pWeapon, int32_t nSrcDam, int32_t bTwoHand)
{
    if (!pUnit || !*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60))
        return 0;
    return 1;
}

// 1.12a: 0x6FAF8E70
// Calling convention: __usercall (ECX = pUnit, EDX = pMinOut, [esp+4] = pMaxOut, [esp+8..28] = throw damage args)
int32_t __stdcall sub_6FAF8E70(D2UnitStrc* pUnit, int32_t* pMinOut, int32_t* pMaxOut, int32_t* pColorOut, D2SkillStrc* pSkill, int32_t nBonusPct, D2UnitStrc* pWeapon)
{
    *pMinOut = 0;
    *pMaxOut = 0;
    if (!pUnit)
        return 0;
    return 1;
}

// 1.12a: 0x6FAF9180
// Calling convention: __usercall (ECX = pUnit, EDX = pMinOut, [esp+4..28] = weapon + elemental damage wrapper)
int32_t __stdcall sub_6FAF9180(D2UnitStrc* pUnit, int32_t* pMinOut, int32_t* pMaxOut, int32_t* pColorOut, D2SkillStrc* pSkill, int32_t nBonusPct, D2UnitStrc* pWeapon)
{
    sub_6FAF7160(nBonusPct, pUnit, pMinOut, pMaxOut, pColorOut, pSkill, pWeapon, 0, 0);
    return sub_6FAF6D70(pUnit, pMinOut, pMaxOut, pColorOut, pSkill);
}

// 1.12a: 0x6FAFACB0
// Calling convention: __fastcall
int32_t __fastcall sub_6FAFACB0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pMinDmg, int32_t* pMaxDmg, int32_t* pColor, int32_t nElemType)
{
    D2UnitStrc* pWeapon = sub_6FB57830(*(D2InventoryStrc**)((uint8_t*)pUnit + 0x60));
    return sub_6FAF61B0(nSkillId, pUnit, nSkillLevel, pWeapon, pMinDmg, pMaxDmg, pColor, 0, nElemType);
}
#endif
