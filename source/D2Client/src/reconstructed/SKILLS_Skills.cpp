// ============================================================================
// SKILLS_Skills.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll (ImageBase = 0x6FAB0000)
// Source slices:
//   - asm/SKILLS_SkillsAma.asm  (0x6FAFB7E0..0x6FAFBA80)
//   - asm/SKILLS_Skills.asm     (0x6FAFBA80..0x6FB074E0)
//   - asm/SKILLS_SkillsPal.asm  (0x6FB373F0..0x6FB38080)
//   - asm/SKILLS_SkillsMon.asm  (0x6FB4C3E0..0x6FB4C580)
//   - asm/SKILLS_SkillsBar.asm  (0x6FB73920..0x6FB75260)
//   - asm/SKILLS_SkillsEMon.asm (0x6FB77050..0x6FB77430)
//   - asm/SKILLS_SkillsNec.asm  (0x6FB77430..0x6FB7D75B)
// ============================================================================

#include <cstdint>
#include "D2CommonDefinitions.h"
#include "D2DataTbls.h"
#include "D2Skills.h"
#include "D2States.h"
#include "D2StatList.h"
#include "D2Items.h"
#include "D2Missiles.h"
#include "D2Dungeon.h"
#include "D2Collision.h"
#include "Units/Units.h"
#include "Units/Missile.h"
#include "Path/Path.h"
#include "D2Gfx.h"
#include "D2Win.h"
#include "D2CMP.h"
#include "Fog.h"

typedef int32_t (__fastcall *D2CltSkillFunc_t)(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);

// ============================================================================
// 1.12a .data Client Skill Start & Do Function Pointer Tables
// ============================================================================
// 1.12a: 0x6FBA3510 (gpfnCltStFunc[70])
// 1.12a: 0x6FBA3628 (gnCltStFuncs = 70)
// 1.12a: 0x6FBA3630 (gpfnCltDoFunc[130])
// 1.12a: 0x6FBA3838 (gnCltDoFuncs = 130)
/*
static const D2CltSkillFunc_t gpfnCltStFunc[70] = {
    /* [ 0] */ nullptr,
    /* [ 1] */ sub_6FAFE0A0,
    /* [ 2] */ sub_6FAFE070,
    /* [ 3] */ sub_6FAFDFC0,
    /* [ 4] */ sub_6FAFD880,
    /* [ 5] */ sub_6FB00C30,
    /* [ 6] */ sub_6FB00970,
    /* [ 7] */ sub_6FB00420,
    /* [ 8] */ sub_6FB01060,
    /* [ 9] */ sub_6FB006E0,
    /* [10] */ sub_6FB00680,
    /* [11] */ sub_6FAFB800,
    /* [12] */ sub_6FAFB7E0,
    /* [13] */ sub_6FAFBDD0,
    /* [14] */ sub_6FAFBC20,
    /* [15] */ sub_6FB02150,
    /* [16] */ sub_6FB01F90,
    /* [17] */ sub_6FB01C70,
    /* [18] */ sub_6FB77EC0,
    /* [19] */ sub_6FB78910,
    /* [20] */ sub_6FB77E30,
    /* [21] */ sub_6FB77D50,
    /* [22] */ sub_6FB77CA0,
    /* [23] */ sub_6FB77C10,
    /* [24] */ sub_6FB77F90,
    /* [25] */ sub_6FB37E70,
    /* [26] */ sub_6FB73E20,
    /* [27] */ sub_6FB73C90,
    /* [28] */ sub_6FB73DD0,
    /* [29] */ sub_6FB75060,
    /* [30] */ sub_6FB74EF0,
    /* [31] */ sub_6FB74D10,
    /* [32] */ sub_6FB4CE10,
    /* [33] */ sub_6FB4C800,
    /* [34] */ sub_6FB4CD30,
    /* [35] */ sub_6FB4CAD0,
    /* [36] */ sub_6FB4C6B0,
    /* [37] */ sub_6FB4C610,
    /* [38] */ sub_6FB4C5F0,
    /* [39] */ sub_6FB4C9A0,
    /* [40] */ sub_6FB4C580,
    /* [41] */ sub_6FB4C270,
    /* [42] */ sub_6FB4D2D0,
    /* [43] */ sub_6FB4D270,
    /* [44] */ sub_6FB4CFB0,
    /* [45] */ sub_6FB78E10,
    /* [46] */ sub_6FB76D80,
    /* [47] */ sub_6FB76EB0,
    /* [48] */ sub_6FB76310,
    /* [49] */ sub_6FB77A30,
    /* [50] */ sub_6FB766B0,
    /* [51] */ sub_6FB76620,
    /* [52] */ sub_6FB76130,
    /* [53] */ sub_6FAFBA80,
    /* [54] */ nullptr,
    /* [55] */ nullptr,
    /* [56] */ nullptr,
    /* [57] */ nullptr,
    /* [58] */ nullptr,
    /* [59] */ nullptr,
    /* [60] */ nullptr,
    /* [61] */ nullptr,
    /* [62] */ nullptr,
    /* [63] */ nullptr,
    /* [64] */ nullptr,
    /* [65] */ nullptr,
    /* [66] */ nullptr,
    /* [67] */ nullptr,
    /* [68] */ nullptr,
    /* [69] */ nullptr,
};
static const int32_t gnCltStFuncs = 70; // 1.12a: 0x6FBA3628

static const D2CltSkillFunc_t gpfnCltDoFunc[130] = {
    /* [  0] */ nullptr,
    /* [  1] */ sub_6FAFF450,
    /* [  2] */ sub_6FB000B0,
    /* [  3] */ sub_6FB00CC0,
    /* [  4] */ sub_6FB008F0,
    /* [  5] */ sub_6FB01B10,
    /* [  6] */ sub_6FB01620,
    /* [  7] */ sub_6FB00B90,
    /* [  8] */ sub_6FB01510,
    /* [  9] */ sub_6FB01A00,
    /* [ 10] */ sub_6FB00AD0,
    /* [ 11] */ sub_6FB01910,
    /* [ 12] */ sub_6FB00DE0,
    /* [ 13] */ sub_6FB00CF0,
    /* [ 14] */ sub_6FB00A40,
    /* [ 15] */ sub_6FB01770,
    /* [ 16] */ sub_6FAFB810,
    /* [ 17] */ sub_6FAFC670,
    /* [ 18] */ sub_6FAFC510,
    /* [ 19] */ sub_6FAFC380,
    /* [ 20] */ sub_6FAFC0F0,
    /* [ 21] */ sub_6FAFB8A0,
    /* [ 22] */ sub_6FAFBF30,
    /* [ 23] */ sub_6FB02B20,
    /* [ 24] */ sub_6FB02DB0,
    /* [ 25] */ sub_6FB01ED0,
    /* [ 26] */ sub_6FB027D0,
    /* [ 27] */ sub_6FB01E20,
    /* [ 28] */ sub_6FB01D90,
    /* [ 29] */ sub_6FB01D10,
    /* [ 30] */ sub_6FB78720,
    /* [ 31] */ sub_6FB78480,
    /* [ 32] */ sub_6FB788C0,
    /* [ 33] */ sub_6FB78870,
    /* [ 34] */ sub_6FB37650,
    /* [ 35] */ sub_6FB37D60,
    /* [ 36] */ sub_6FB37C50,
    /* [ 37] */ sub_6FB37800,
    /* [ 38] */ sub_6FB74040,
    /* [ 39] */ sub_6FB74340,
    /* [ 40] */ sub_6FB73FD0,
    /* [ 41] */ sub_6FB749A0,
    /* [ 42] */ sub_6FB74B70,
    /* [ 43] */ sub_6FB74840,
    /* [ 44] */ sub_6FB74700,
    /* [ 45] */ sub_6FB73ED0,
    /* [ 46] */ sub_6FB4C310,
    /* [ 47] */ sub_6FB4C2D0,
    /* [ 48] */ sub_6FB4CB90,
    /* [ 49] */ sub_6FB4C770,
    /* [ 50] */ sub_6FB4C630,
    /* [ 51] */ sub_6FB4E260,
    /* [ 52] */ sub_6FB4C290,
    /* [ 53] */ sub_6FB4E170,
    /* [ 54] */ sub_6FB4E820,
    /* [ 55] */ sub_6FB4DE90,
    /* [ 56] */ sub_6FB4E710,
    /* [ 57] */ sub_6FB4DD10,
    /* [ 58] */ sub_6FB4DC00,
    /* [ 59] */ sub_6FB4C3E0,
    /* [ 60] */ sub_6FB4D9F0,
    /* [ 61] */ sub_6FB4D850,
    /* [ 62] */ sub_6FB4C320,
    /* [ 63] */ sub_6FB4D740,
    /* [ 64] */ sub_6FB4D3F0,
    /* [ 65] */ sub_6FB4D620,
    /* [ 66] */ sub_6FB4E430,
    /* [ 67] */ sub_6FB79820,
    /* [ 68] */ sub_6FB79770,
    /* [ 69] */ sub_6FB796C0,
    /* [ 70] */ sub_6FB77910,
    /* [ 71] */ sub_6FB77660,
    /* [ 72] */ sub_6FB77AE0,
    /* [ 73] */ sub_6FB76290,
    /* [ 74] */ sub_6FB76E10,
    /* [ 75] */ sub_6FB76790,
    /* [ 76] */ sub_6FB76AA0,
    /* [ 77] */ sub_6FB774F0,
    /* [ 78] */ sub_6FB76990,
    /* [ 79] */ sub_6FB76070,
    /* [ 80] */ sub_6FB77240,
    /* [ 81] */ sub_6FB768C0,
    /* [ 82] */ sub_6FB76380,
    /* [ 83] */ sub_6FB01820,
    /* [ 84] */ sub_6FB023B0,
    /* [ 85] */ sub_6FB789F0,
    /* [ 86] */ sub_6FB37A80,
    /* [ 87] */ sub_6FB02C40,
    /* [ 88] */ sub_6FB765F0,
    /* [ 89] */ sub_6FB77050,
    /* [ 90] */ sub_6FB795D0,
    /* [ 91] */ sub_6FB78ED0,
    /* [ 92] */ sub_6FB78FE0,
    /* [ 93] */ sub_6FB77430,
    /* [ 94] */ sub_6FB4E630,
    /* [ 95] */ sub_6FB4E550,
    /* [ 96] */ sub_6FB02670,
    /* [ 97] */ nullptr,
    /* [ 98] */ nullptr,
    /* [ 99] */ nullptr,
    /* [100] */ nullptr,
    /* [101] */ nullptr,
    /* [102] */ nullptr,
    /* [103] */ nullptr,
    /* [104] */ nullptr,
    /* [105] */ nullptr,
    /* [106] */ nullptr,
    /* [107] */ nullptr,
    /* [108] */ nullptr,
    /* [109] */ nullptr,
    /* [110] */ nullptr,
    /* [111] */ nullptr,
    /* [112] */ nullptr,
    /* [113] */ nullptr,
    /* [114] */ nullptr,
    /* [115] */ nullptr,
    /* [116] */ nullptr,
    /* [117] */ nullptr,
    /* [118] */ nullptr,
    /* [119] */ nullptr,
    /* [120] */ nullptr,
    /* [121] */ nullptr,
    /* [122] */ nullptr,
    /* [123] */ nullptr,
    /* [124] */ nullptr,
    /* [125] */ nullptr,
    /* [126] */ nullptr,
    /* [127] */ nullptr,
    /* [128] */ nullptr,
    /* [129] */ nullptr,
};
static const int32_t gnCltDoFuncs = 130; // 1.12a: 0x6FBA3838
*/

// ============================================================================
// SECTION: SKILLS_SkillsAma.asm — Amazon Client Skill Start/Do Functions (SKILLS/SkillsAma.cpp)
// ============================================================================

// 1.12a: 0x6FAFB7E0..0x6FAFB7FD (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFB7E0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[12]
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10965 -> SKILLS_SetParam1
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    SKILLS_SetParam1(pSkill, 0);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFB7E1: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FAFB7EA: ret      4
    //   0x6FAFB7F0: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FAFB7FA: ret      4
    return 1;
}

// 1.12a: 0x6FAFB800..0x6FAFB892 (__fastcall, ret 0x4)
// 1.12a: 0x6FAFB810 (internal entry point sub_6FAFB810 - gpfnCltDoFunc[16])
int32_t __fastcall sub_6FAFB800(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[11]
    // Callees: 0x6fafdb40, 0x6fae1fa0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFB802: call     0x6fafdb40
    //   0x6FAFB80D: ret      4
    //   0x6FAFB813: cmp      byte ptr [esi + 0x4e], 3
    //   0x6FAFB81E: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFB825: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FAFB833: imul     edx, edx, 0x23c
    //   0x6FAFB843: ret      4
    //   0x6FAFB846: cmp      dword ptr [esi], 0
    //   0x6FAFB862: call     0x6fae1fa0
    //   0x6FAFB867: cmp      dword ptr [esi], 1
    //   0x6FAFB882: call     0x6fae1fa0
    //   0x6FAFB88F: ret      4
    return 1;
}

// 1.12a: 0x6FAFB8A0..0x6FAFBA76 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFB8A0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[21]
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\SkillsAma.cpp'
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10368 -> SKILLS_GetParam3, D2Common.dll!#10089 -> SKILLS_GetParam2, 0x6face550, D2Common.dll!#10087 -> UNITS_GetMeleeRange, D2Common.dll!#10974 -> UNITS_IsInMeleeRange, 0x6fafe9b0, D2Common.dll!#10063 -> SKILLS_GetParam1
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    SKILLS_SetParam1(pSkill, 0);
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFB8AA: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFB8B1: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FAFB8BF: imul     edx, edx, 0x23c
    //   0x6FAFB8D1: ret      4
    //   0x6FAFB8D6: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FAFB8E6: ret      4
    //   0x6FAFB8EC: call     0x6fabc31c ; D2Common.dll!#10368 -> SKILLS_GetParam3
    //   0x6FAFB8F4: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FAFB90C: call     0x6face550
    //   0x6FAFB916: call     0x6fabccbe ; D2Common.dll!#10087 -> UNITS_GetMeleeRange
    //   0x6FAFB926: cmp      ecx, eax
    //   0x6FAFB92D: call     0x6fabc724 ; D2Common.dll!#10974 -> UNITS_IsInMeleeRange
    //   0x6FAFB966: call     0x6fafe9b0
    //   0x6FAFB984: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FAFB98E: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FAFB995: cmp      edi, ecx
    //   0x6FAFB9A1: cmp      eax, ecx
    //   0x6FAFB9DD: call     0x6fafe9b0
    //   0x6FAFBA01: ret      4
    //   0x6FAFBA06: call     0x6fab15c0
    //   0x6FAFBA0E: call     0x6fab1590
    //   0x6FAFBA18: call     0x6fabc6dc ; D2Common.dll!#10756 -> D2Common_10216
    //   0x6FAFBA22: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FAFBA2C: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FAFBA36: push     0x6fb85b1c ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsAma.cpp'
    //   0x6FAFBA3C: call     0x6fabc2ec ; D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill
    //   0x6FAFBA41: call     0x6fab1560
    //   0x6FAFBA46: call     0x6fafd740
    //   0x6FAFBA4D: call     0x6fabcd0c ; D2Common.dll!#11081 -> UNITS_GetHitClass
    //   0x6FAFBA5B: call     0x6fae6780
    //   0x6FAFBA62: call     0x6fae1fa0
    //   0x6FAFBA73: ret      4
    return 1;
}

// ============================================================================
// SECTION: SKILLS_Skills.asm — Core Client Skills Engine, Sorceress/Common Skills, Belt/Options/Waypoints (SKILLS/Skills.cpp, SkillsSor.cpp, waygate.cpp)
// ============================================================================

// 1.12a: 0x6FAFBA80..0x6FAFBC19 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFBA80(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[53]
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10087 -> UNITS_GetMeleeRange, 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, 0x6fafe9b0
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    SKILLS_SetParam1(pSkill, 0);
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFBA8F: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFBA96: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FAFBAA6: imul     edi, edi, 0x23c
    //   0x6FAFBAB1: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FAFBAC5: ret      4
    //   0x6FAFBACA: call     0x6fabccbe ; D2Common.dll!#10087 -> UNITS_GetMeleeRange
    //   0x6FAFBAD6: call     0x6faf0d20
    //   0x6FAFBAE7: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFBAF2: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFBAFC: call     0x6fab326d
    //   0x6FAFBB03: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FAFBB0C: cmp      esi, eax
    //   0x6FAFBB3F: call     0x6fafe9b0
    //   0x6FAFBB5E: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FAFBB6F: ret      4
    //   0x6FAFBB84: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FAFBB8F: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FAFBB96: cmp      eax, 2
    //   0x6FAFBB9B: cmp      eax, 3
    //   0x6FAFBBA0: cmp      eax, 5
    //   0x6FAFBBAD: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FAFBBC2: cmp      eax, 2
    //   0x6FAFBBC7: cmp      eax, 3
    //   0x6FAFBBCC: cmp      eax, 5
    //   0x6FAFBBD9: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FAFBBF2: call     0x6fabc6dc ; D2Common.dll!#10756 -> D2Common_10216
    //   0x6FAFBBFB: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FAFBC05: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FAFBC16: ret      4
    return 1;
}

// 1.12a: 0x6FAFBC20..0x6FAFBDC1 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFBC20(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[14]
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10087 -> UNITS_GetMeleeRange, 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, 0x6fafeba0
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    SKILLS_SetParam1(pSkill, 0);
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFBC33: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFBC3A: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FAFBC4A: imul     ebx, ebx, 0x23c
    //   0x6FAFBC55: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FAFBC69: ret      4
    //   0x6FAFBC6E: call     0x6fabccbe ; D2Common.dll!#10087 -> UNITS_GetMeleeRange
    //   0x6FAFBC7A: call     0x6faf0d20
    //   0x6FAFBC8B: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFBC96: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFBCA0: call     0x6fab326d
    //   0x6FAFBCA7: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FAFBCB0: cmp      esi, eax
    //   0x6FAFBCC9: call     0x6fafeba0
    //   0x6FAFBCDA: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FAFBCEB: ret      4
    //   0x6FAFBD05: call     0x6fafe9b0
    //   0x6FAFBD26: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FAFBD2B: cmp      ebp, eax
    //   0x6FAFBD37: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FAFBD3E: cmp      eax, 2
    //   0x6FAFBD43: cmp      eax, 3
    //   0x6FAFBD48: cmp      eax, 5
    //   0x6FAFBD55: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FAFBD6A: cmp      eax, 2
    //   0x6FAFBD6F: cmp      eax, 3
    //   0x6FAFBD74: cmp      eax, 5
    //   0x6FAFBD81: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FAFBD9A: call     0x6fabc6dc ; D2Common.dll!#10756 -> D2Common_10216
    //   0x6FAFBDA3: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FAFBDAD: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FAFBDBE: ret      4
    return 1;
}

// 1.12a: 0x6FAFBDD0..0x6FAFBF2D (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFBDD0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[13]
    // Callees: 0x6fafdb40, D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6facf5b0, 0x6fafeba0, 0x6fafec20, D2Common.dll!#10965 -> SKILLS_SetParam1, D2Common.dll!#10817 -> SKILLS_SetParam2
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    SKILLS_SetParam1(pSkill, 0);
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFBDDB: call     0x6fafdb40
    //   0x6FAFBDE9: ret      4
    //   0x6FAFBDF1: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFBDF8: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FAFBE08: imul     ebx, ebx, 0x23c
    //   0x6FAFBE1A: ret      4
    //   0x6FAFBE1E: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FAFBE37: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FAFBE40: call     0x6facf5b0
    //   0x6FAFBE68: call     0x6fafeba0
    //   0x6FAFBE71: call     0x6fafec20
    //   0x6FAFBE88: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FAFBE9F: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FAFBEA8: cmp      ecx, eax
    //   0x6FAFBEB6: cmp      edx, ecx
    //   0x6FAFBEBC: cmp      ecx, eax
    //   0x6FAFBEC8: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FAFBED5: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FAFBEDF: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FAFBEE8: call     0x6fab15c0
    //   0x6FAFBEF0: call     0x6fab1590
    //   0x6FAFBEFA: call     0x6fabc6dc ; D2Common.dll!#10756 -> D2Common_10216
    //   0x6FAFBF0B: ret      4
    //   0x6FAFBF11: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FAFBF19: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FAFBF2A: ret      4
    return 1;
}

// 1.12a: 0x6FAFBF30..0x6FAFC0E5 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFBF30(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[22]
    // Callees: 0x6fb00830, 0x6facf5b0, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fab15c0, 0x6fab1590, 0x6fafe9b0, 0x6fad4b20, D2Common.dll!#11161 -> MISSILE_SetTargetX
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFBF45: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFBF4C: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FAFBF60: imul     edi, edi, 0x23c
    //   0x6FAFBF71: call     0x6fb00830
    //   0x6FAFBF82: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFBF8A: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FAFBF97: call     0x6facf5b0
    //   0x6FAFBFAE: ret      4
    //   0x6FAFBFC2: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FAFBFD4: call     0x6fab15c0
    //   0x6FAFBFDF: call     0x6fab1590
    //   0x6FAFC022: call     0x6fafe9b0
    //   0x6FAFC056: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FAFC097: call     0x6fab1590
    //   0x6FAFC0A4: call     0x6fab15c0
    //   0x6FAFC0B1: call     0x6fad4b20
    //   0x6FAFC0C0: call     0x6fabc304 ; D2Common.dll!#11161 -> MISSILE_SetTargetX
    //   0x6FAFC0D4: ret      4
    //   0x6FAFC0E2: ret      4
    return 1;
}

// 1.12a: 0x6FAFC0F0..0x6FAFC504 (__fastcall, ret 0x4)
// 1.12a: 0x6FAFC380 (internal entry point sub_6FAFC380 - gpfnCltDoFunc[19])
int32_t __fastcall sub_6FAFC0F0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[20]
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, D2Common.dll!#10368 -> SKILLS_GetParam3, D2Common.dll!#10089 -> SKILLS_GetParam2, 0x6face550, 0x6fafe9b0, D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    SKILLS_SetParam1(pSkill, 0);
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFC105: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFC10C: cmp      ebx, dword ptr [eax + 0xba0]
    //   0x6FAFC11C: imul     edi, edi, 0x23c
    //   0x6FAFC135: ret      4
    //   0x6FAFC13A: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FAFC14A: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FAFC169: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FAFC175: call     0x6fabc31c ; D2Common.dll!#10368 -> SKILLS_GetParam3
    //   0x6FAFC17D: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FAFC195: call     0x6face550
    //   0x6FAFC19C: cmp      eax, ecx
    //   0x6FAFC1D4: call     0x6fafe9b0
    //   0x6FAFC1F3: call     0x6fabcac0 ; D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    //   0x6FAFC203: call     0x6fafe320
    //   0x6FAFC21D: call     0x6fabcd1e ; D2Common.dll!#10764 -> UNITS_GetWeaponClass
    //   0x6FAFC222: cmp      eax, 1
    //   0x6FAFC23E: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFC246: cmp      ebx, dword ptr [eax + 0xb6c]
    //   0x6FAFC26D: call     0x6fab1590
    //   0x6FAFC278: call     0x6fab15c0
    //   0x6FAFC2AC: call     0x6fad4b20
    //   0x6FAFC2B8: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FAFC2BF: cmp      edi, ebx
    //   0x6FAFC2CB: cmp      eax, ebx
    //   0x6FAFC30B: call     0x6fafe9b0
    //   0x6FAFC314: cmp      ecx, ebx
    //   0x6FAFC320: cmp      ecx, ebx
    //   0x6FAFC327: call     0x6fab15c0
    //   0x6FAFC32F: call     0x6fab1590
    //   0x6FAFC339: call     0x6fabc6dc ; D2Common.dll!#10756 -> D2Common_10216
    //   0x6FAFC342: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FAFC34C: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FAFC355: call     0x6fab2d90
    //   0x6FAFC35A: call     0x6fafd740
    //   0x6FAFC36E: ret      4
    //   0x6FAFC37D: ret      4
    //   0x6FAFC392: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFC399: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FAFC3A9: imul     edi, edi, 0x23c
    //   0x6FAFC3BB: ret      4
    //   0x6FAFC3C0: call     0x6faf0d20
    //   0x6FAFC3D1: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFC3DC: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFC3E6: call     0x6fab326d
    //   0x6FAFC3ED: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FAFC3FF: call     0x6fb00830
    //   0x6FAFC410: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFC418: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FAFC426: call     0x6fab1590
    //   0x6FAFC433: call     0x6fab15c0
    //   0x6FAFC43E: call     0x6fab1590
    //   0x6FAFC44B: call     0x6fab15c0
    //   0x6FAFC46E: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FAFC4DB: call     0x6fad4b20
    //   0x6FAFC4E5: cmp      esi, eax
    //   0x6FAFC4F5: ret      4
    //   0x6FAFC501: ret      4
    return 1;
}

// 1.12a: 0x6FAFC510..0x6FAFC662 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFC510(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[18]
    // Callees: 0x6fafe320, 0x6fb00830, 0x6facf5b0, 0x6fad4b20, D2Common.dll!#11161 -> MISSILE_SetTargetX, 0x6fab1590, 0x6fab15c0, D2Common.dll!#10792 -> MISSILE_SetTargetY
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFC521: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFC528: cmp      ebp, dword ptr [ecx + 0xba0]
    //   0x6FAFC538: imul     eax, eax, 0x23c
    //   0x6FAFC549: ret      4
    //   0x6FAFC564: call     0x6fafe320
    //   0x6FAFC574: call     0x6fb00830
    //   0x6FAFC585: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFC58D: cmp      eax, dword ptr [ecx + 0xb6c]
    //   0x6FAFC599: call     0x6facf5b0
    //   0x6FAFC5FC: call     0x6fad4b20
    //   0x6FAFC617: call     0x6fabc304 ; D2Common.dll!#11161 -> MISSILE_SetTargetX
    //   0x6FAFC61E: call     0x6fab1590
    //   0x6FAFC627: call     0x6fab15c0
    //   0x6FAFC642: call     0x6fabc2b6 ; D2Common.dll!#10792 -> MISSILE_SetTargetY
    //   0x6FAFC653: ret      4
    //   0x6FAFC65F: ret      4
    return 1;
}

// 1.12a: 0x6FAFC670..0x6FAFC82E (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFC670(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[17]
    // Callees: D2Common.dll!#10764 -> UNITS_GetWeaponClass, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fafe320, 0x6fab1590, 0x6fab15c0, 0x6fafd950, 0x6fafd910, 0x6fad4b20
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFC68D: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFC694: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FAFC6A4: imul     esi, esi, 0x23c
    //   0x6FAFC6BA: ret      4
    //   0x6FAFC6CB: call     0x6fabcd1e ; D2Common.dll!#10764 -> UNITS_GetWeaponClass
    //   0x6FAFC6D0: cmp      eax, 1
    //   0x6FAFC6F2: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFC6FA: cmp      ebx, dword ptr [eax + 0xb6c]
    //   0x6FAFC717: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FAFC72B: call     0x6fafe320
    //   0x6FAFC73A: call     0x6fab1590
    //   0x6FAFC74B: call     0x6fab15c0
    //   0x6FAFC762: call     0x6fafd950
    //   0x6FAFC76B: call     0x6fafd910
    //   0x6FAFC778: imul     eax, ecx
    //   0x6FAFC790: imul     eax, ecx
    //   0x6FAFC7E2: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FAFC7FF: call     0x6fad4b20
    //   0x6FAFC81F: ret      4
    //   0x6FAFC82B: ret      4
    return 1;
}

// 1.12a: 0x6FAFC830..0x6FAFC836 (__usercall, ret 0x0)
int32_t __usercall sub_6FAFC830(D2UnitStrc* pUnit /*eax*/)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFC835: ret
    return 1;
}

// 1.12a: 0x6FAFC840..0x6FAFC84D (__stdcall, ret 0x4)
int32_t __stdcall sub_6FAFC840(int32_t a1)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFC84A: ret      4
    return 1;
}

// 1.12a: 0x6FAFC850..0x6FAFC86B (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFC850(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFC864: ret
    //   0x6FAFC86A: ret
    return 1;
}

// 1.12a: 0x6FAFC870..0x6FAFC8FD (__usercall, ret 0x8)
int32_t __usercall sub_6FAFC870(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2)
{
    // Callees: D2gfx.dll!#10024 -> TEXTURE_CelDraw
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFC8BE: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FAFC8F1: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FAFC8FA: ret      8
    return 1;
}

// 1.12a: 0x6FAFC900..0x6FAFC90D (__stdcall, ret 0x4)
int32_t __stdcall sub_6FAFC900(int32_t a1)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFC90A: ret      4
    return 1;
}

// 1.12a: 0x6FAFC910..0x6FAFC91E (__usercall, ret 0x0)
int32_t __usercall sub_6FAFC910(D2UnitStrc* pUnit /*eax*/)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFC91D: ret
    return 1;
}

// 1.12a: 0x6FAFC920..0x6FAFC979 (__usercall, ret 0x4)
int32_t __usercall sub_6FAFC920(D2UnitStrc* pUnit /*eax*/, int32_t a1)
{
    // Callees: D2gfx.dll!#10024 -> TEXTURE_CelDraw
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFC96D: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FAFC976: ret      4
    return 1;
}

// 1.12a: 0x6FAFC980..0x6FAFCA1D (__usercall, ret 0x4)
int32_t __usercall sub_6FAFC980(D2UnitStrc* pUnit /*eax*/, int32_t a1)
{
    // Callees: D2gfx.dll!#10024 -> TEXTURE_CelDraw
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFC9E2: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FAFCA12: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FAFCA1A: ret      4
    return 1;
}

// 1.12a: 0x6FAFCA20..0x6FAFCAB1 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFCA20(void)
{
    // Referenced strings: '..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    // Callees: D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware, Fog.dll!#10043 -> FOG_Free
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFCA2C: call     0x6fabc0d0 ; D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware
    //   0x6FAFCA39: mov      edx, 0x6fb807a0 ; STR:'..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    //   0x6FAFCA40: call     0x6fabbf86 ; Fog.dll!#10043 -> FOG_Free
    //   0x6FAFCA5A: call     0x6fabc0d0 ; D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware
    //   0x6FAFCA67: mov      edx, 0x6fb807a0 ; STR:'..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    //   0x6FAFCA6E: call     0x6fabbf86 ; Fog.dll!#10043 -> FOG_Free
    //   0x6FAFCA88: call     0x6fabc0d0 ; D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware
    //   0x6FAFCA95: mov      edx, 0x6fb807a0 ; STR:'..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    //   0x6FAFCA9C: call     0x6fabbf86 ; Fog.dll!#10043 -> FOG_Free
    return 1;
}

// 1.12a: 0x6FAFCAC0..0x6FAFCAFE (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFCAC0(D2UnitStrc* pUnit)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FAFCB00..0x6FAFCBBA (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFCB00(D2UnitStrc* pUnit)
{
    // Callees: D2Win.dll!#10010 -> D2Win_10127_SetFont, D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex, D2Win.dll!#10132 -> D2Win_10121_GetTextWidth, D2Win.dll!#10001 -> D2Win_10117_DrawText
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFCB0A: call     0x6fabd36c ; D2Win.dll!#10010 -> D2Win_10127_SetFont
    //   0x6FAFCB18: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FAFCB41: call     0x6fabd354 ; D2Win.dll!#10132 -> D2Win_10121_GetTextWidth
    //   0x6FAFCB46: cmp      eax, ebx
    //   0x6FAFCB59: call     0x6fabd360 ; D2Win.dll!#10001 -> D2Win_10117_DrawText
    //   0x6FAFCB63: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FAFCB8D: call     0x6fabd354 ; D2Win.dll!#10132 -> D2Win_10121_GetTextWidth
    //   0x6FAFCB92: cmp      eax, edi
    //   0x6FAFCBA5: call     0x6fabd360 ; D2Win.dll!#10001 -> D2Win_10117_DrawText
    return 1;
}

// 1.12a: 0x6FAFCBC0..0x6FAFCCA1 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFCBC0(D2UnitStrc* pUnit)
{
    // Callees: D2Win.dll!#10010 -> D2Win_10127_SetFont, D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex, D2Win.dll!#10132 -> D2Win_10121_GetTextWidth, D2Win.dll!#10001 -> D2Win_10117_DrawText
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFCBCA: call     0x6fabd36c ; D2Win.dll!#10010 -> D2Win_10127_SetFont
    //   0x6FAFCBD8: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FAFCC0E: call     0x6fabd354 ; D2Win.dll!#10132 -> D2Win_10121_GetTextWidth
    //   0x6FAFCC13: cmp      eax, esi
    //   0x6FAFCC26: call     0x6fabd360 ; D2Win.dll!#10001 -> D2Win_10117_DrawText
    //   0x6FAFCC3E: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FAFCC74: call     0x6fabd354 ; D2Win.dll!#10132 -> D2Win_10121_GetTextWidth
    //   0x6FAFCC79: cmp      eax, esi
    //   0x6FAFCC8C: call     0x6fabd360 ; D2Win.dll!#10001 -> D2Win_10117_DrawText
    return 1;
}

// 1.12a: 0x6FAFCCB0..0x6FAFCCEE (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFCCB0(D2UnitStrc* pUnit)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FAFCCF0..0x6FAFCD5F (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFCCF0(D2UnitStrc* pUnit)
{
    // Callees: D2Win.dll!#10010 -> D2Win_10127_SetFont, D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex, D2Win.dll!#10132 -> D2Win_10121_GetTextWidth, D2Win.dll!#10001 -> D2Win_10117_DrawText
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFCCFA: call     0x6fabd36c ; D2Win.dll!#10010 -> D2Win_10127_SetFont
    //   0x6FAFCD08: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FAFCD32: call     0x6fabd354 ; D2Win.dll!#10132 -> D2Win_10121_GetTextWidth
    //   0x6FAFCD37: cmp      eax, esi
    //   0x6FAFCD4A: call     0x6fabd360 ; D2Win.dll!#10001 -> D2Win_10117_DrawText
    return 1;
}

// 1.12a: 0x6FAFCD60..0x6FAFCE41 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFCD60(D2UnitStrc* pUnit)
{
    // Callees: D2Win.dll!#10010 -> D2Win_10127_SetFont, D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex, D2Win.dll!#10132 -> D2Win_10121_GetTextWidth, D2Win.dll!#10001 -> D2Win_10117_DrawText, Fog.dll!#10227 -> FOG_IsExpansion
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFCD6A: call     0x6fabd36c ; D2Win.dll!#10010 -> D2Win_10127_SetFont
    //   0x6FAFCD78: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FAFCDAE: call     0x6fabd354 ; D2Win.dll!#10132 -> D2Win_10121_GetTextWidth
    //   0x6FAFCDB3: cmp      eax, esi
    //   0x6FAFCDC6: call     0x6fabd360 ; D2Win.dll!#10001 -> D2Win_10117_DrawText
    //   0x6FAFCDCB: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FAFCDDE: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FAFCE14: call     0x6fabd354 ; D2Win.dll!#10132 -> D2Win_10121_GetTextWidth
    //   0x6FAFCE19: cmp      eax, esi
    //   0x6FAFCE2C: call     0x6fabd360 ; D2Win.dll!#10001 -> D2Win_10117_DrawText
    return 1;
}

// 1.12a: 0x6FAFCE50..0x6FAFCE82 (__usercall, ret 0x4)
int32_t __usercall sub_6FAFCE50(D2UnitStrc* pUnit /*eax*/, int32_t nArgEdx /*edx*/, int32_t a1)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFCE56: cmp      ecx, eax
    //   0x6FAFCE5C: cmp      ecx, eax
    //   0x6FAFCE6D: cmp      eax, ecx
    //   0x6FAFCE71: cmp      eax, edx
    //   0x6FAFCE7A: ret      4
    //   0x6FAFCE7F: ret      4
    return 1;
}

// 1.12a: 0x6FAFCE90..0x6FAFCF6F (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFCE90(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFCE98: cmp      eax, ebx
    //   0x6FAFCECC: cmp      edx, ecx
    //   0x6FAFCEDA: cmp      edx, ecx
    //   0x6FAFCEE1: cmp      edi, ecx
    //   0x6FAFCEE5: cmp      edi, eax
    //   0x6FAFCEFF: cmp      edx, ecx
    //   0x6FAFCF06: cmp      edx, ecx
    //   0x6FAFCF0D: cmp      edi, edx
    //   0x6FAFCF11: cmp      edi, eax
    return 1;
}

// 1.12a: 0x6FAFCF70..0x6FAFD012 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFCF70(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFCF78: cmp      eax, esi
    //   0x6FAFCFA1: cmp      edx, ecx
    //   0x6FAFCFA8: cmp      edx, ecx
    //   0x6FAFCFB5: cmp      ecx, edx
    //   0x6FAFCFB9: cmp      ecx, eax
    return 1;
}

// 1.12a: 0x6FAFD020..0x6FAFD05A (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFD020(void)
{
    // Referenced strings: 'Data\\Global\\UI\\Bigmenu\\popupok', 'Data\\Global\\UI\\Menu\\okcancelbtn'
    // Callees: 0x6fb6ef70
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD022: mov      eax, 0x6fb85afc ; STR:'Data\\Global\\UI\\Bigmenu\\popupok'
    //   0x6FAFD027: call     0x6fb6ef70
    //   0x6FAFD033: mov      eax, 0x6fb85afc ; STR:'Data\\Global\\UI\\Bigmenu\\popupok'
    //   0x6FAFD038: call     0x6fb6ef70
    //   0x6FAFD044: mov      eax, 0x6fb85adc ; STR:'Data\\Global\\UI\\Menu\\okcancelbtn'
    //   0x6FAFD049: call     0x6fb6ef70
    return 1;
}

// 1.12a: 0x6FAFD060..0x6FAFD132 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFD060(void)
{
    // Callees: 0x6fae1fa0
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD081: cmp      edx, ecx
    //   0x6FAFD08F: cmp      edx, ecx
    //   0x6FAFD096: cmp      edi, ecx
    //   0x6FAFD09A: cmp      edi, eax
    //   0x6FAFD0B1: cmp      edx, ecx
    //   0x6FAFD0B8: cmp      edx, ecx
    //   0x6FAFD0BF: cmp      edi, edx
    //   0x6FAFD0C3: cmp      edi, eax
    //   0x6FAFD0E3: call     0x6fae1fa0
    return 1;
}

// 1.12a: 0x6FAFD140..0x6FAFD1ED (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFD140(void)
{
    // Callees: 0x6fae1fa0
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD160: cmp      edx, ecx
    //   0x6FAFD168: cmp      edx, ecx
    //   0x6FAFD175: cmp      ecx, edx
    //   0x6FAFD179: cmp      ecx, eax
    //   0x6FAFD19C: call     0x6fae1fa0
    return 1;
}

// 1.12a: 0x6FAFD1F0..0x6FAFD241 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFD1F0(void)
{
    // Callees: D2gfx.dll!#10016 -> WINDOW_GetState, D2gfx.dll!#10005 -> D2GFX_StartDraw, D2gfx.dll!#10047 -> D2GFX_ClearScreen, D2Win.dll!#10141 -> D2Win_10179, 0x6fafc980, 0x6fafcbc0, 0x6fafc870, 0x6fafcb00
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD1F0: call     0x6fabd114 ; D2gfx.dll!#10016 -> WINDOW_GetState
    //   0x6FAFD1FE: call     0x6fabd132 ; D2gfx.dll!#10005 -> D2GFX_StartDraw
    //   0x6FAFD209: call     0x6fabd12c ; D2gfx.dll!#10047 -> D2GFX_ClearScreen
    //   0x6FAFD20E: call     0x6fabd3cc ; D2Win.dll!#10141 -> D2Win_10179
    //   0x6FAFD215: call     0x6fafc980
    //   0x6FAFD21A: call     0x6fafcbc0
    //   0x6FAFD22C: call     0x6fafc870
    //   0x6FAFD231: call     0x6fafcb00
    //   0x6FAFD236: call     0x6fb4f5b0
    //   0x6FAFD240: ret
    return 1;
}

// 1.12a: 0x6FAFD250..0x6FAFD283 (__stdcall, ret 0x4)
int32_t __stdcall sub_6FAFD250(int32_t a1)
{
    // Callees: 0x6fb299d0, 0x6fafd1f0, 0x6fb1a660
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD250: call     0x6fb299d0
    //   0x6FAFD25A: call     0x6fafd1f0
    //   0x6FAFD25F: call     0x6fb1a660
    //   0x6FAFD278: ret      4
    //   0x6FAFD280: ret      4
    return 1;
}

// 1.12a: 0x6FAFD290..0x6FAFD2DA (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFD290(void)
{
    // Callees: D2gfx.dll!#10016 -> WINDOW_GetState, D2gfx.dll!#10005 -> D2GFX_StartDraw, D2gfx.dll!#10047 -> D2GFX_ClearScreen, D2Win.dll!#10141 -> D2Win_10179, 0x6fafc980, 0x6fafcd60, 0x6fafc920, 0x6fafccf0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD290: call     0x6fabd114 ; D2gfx.dll!#10016 -> WINDOW_GetState
    //   0x6FAFD29E: call     0x6fabd132 ; D2gfx.dll!#10005 -> D2GFX_StartDraw
    //   0x6FAFD2A9: call     0x6fabd12c ; D2gfx.dll!#10047 -> D2GFX_ClearScreen
    //   0x6FAFD2AE: call     0x6fabd3cc ; D2Win.dll!#10141 -> D2Win_10179
    //   0x6FAFD2B5: call     0x6fafc980
    //   0x6FAFD2BA: call     0x6fafcd60
    //   0x6FAFD2C5: call     0x6fafc920
    //   0x6FAFD2CA: call     0x6fafccf0
    //   0x6FAFD2CF: call     0x6fb4f5b0
    //   0x6FAFD2D9: ret
    return 1;
}

// 1.12a: 0x6FAFD2E0..0x6FAFD343 (__stdcall, ret 0x4)
int32_t __stdcall sub_6FAFD2E0(int32_t a1)
{
    // Callees: 0x6fb299d0, D2gfx.dll!#10016 -> WINDOW_GetState, D2gfx.dll!#10005 -> D2GFX_StartDraw, D2gfx.dll!#10047 -> D2GFX_ClearScreen, D2Win.dll!#10141 -> D2Win_10179, 0x6fafc980, 0x6fafcd60, 0x6fafc920
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD2E0: call     0x6fb299d0
    //   0x6FAFD2E5: call     0x6fabd114 ; D2gfx.dll!#10016 -> WINDOW_GetState
    //   0x6FAFD2F3: call     0x6fabd132 ; D2gfx.dll!#10005 -> D2GFX_StartDraw
    //   0x6FAFD2FE: call     0x6fabd12c ; D2gfx.dll!#10047 -> D2GFX_ClearScreen
    //   0x6FAFD303: call     0x6fabd3cc ; D2Win.dll!#10141 -> D2Win_10179
    //   0x6FAFD30A: call     0x6fafc980
    //   0x6FAFD30F: call     0x6fafcd60
    //   0x6FAFD31A: call     0x6fafc920
    //   0x6FAFD31F: call     0x6fafccf0
    //   0x6FAFD324: call     0x6fb4f5b0
    //   0x6FAFD329: call     0x6fabd126 ; D2gfx.dll!#10059 -> D2GFX_EndScene
    //   0x6FAFD32E: call     0x6fb1a660
    //   0x6FAFD340: ret      4
    return 1;
}

// 1.12a: 0x6FAFD350..0x6FAFD495 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFD350(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Data\\Global\\UI\\Bigmenu\\popupok', 'Data\\Global\\UI\\Menu\\okcancelbtn'
    // Callees: D2Win.dll!#10078 -> D2Win_10176, 0x6fb6ef70, D2Win.dll!#10136 -> D2Win_10177, D2gfx.dll!#10078 -> WINDOW_GetWindow, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, 0x6fb1cec0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD36B: call     0x6fabd3de ; D2Win.dll!#10078 -> D2Win_10176
    //   0x6FAFD371: mov      eax, 0x6fb85afc ; STR:'Data\\Global\\UI\\Bigmenu\\popupok'
    //   0x6FAFD376: call     0x6fb6ef70
    //   0x6FAFD381: mov      eax, 0x6fb85afc ; STR:'Data\\Global\\UI\\Bigmenu\\popupok'
    //   0x6FAFD386: call     0x6fb6ef70
    //   0x6FAFD391: mov      eax, 0x6fb85adc ; STR:'Data\\Global\\UI\\Menu\\okcancelbtn'
    //   0x6FAFD396: call     0x6fb6ef70
    //   0x6FAFD3A2: call     0x6fabd3c6 ; D2Win.dll!#10136 -> D2Win_10177
    //   0x6FAFD3BB: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FAFD3C2: cmp      ebx, ebp
    //   0x6FAFD3CB: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFD3D6: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFD3E0: call     0x6fab326d
    //   0x6FAFD404: call     0x6fb1cec0
    //   0x6FAFD40C: cmp      edi, 0x30
    //   0x6FAFD416: call     0x6fb1cf40
    //   0x6FAFD41B: cmp      dword ptr [0x6fbcc29c], ebp
    //   0x6FAFD427: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FAFD42E: cmp      ebx, ebp
    //   0x6FAFD437: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFD442: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFD44C: call     0x6fab326d
    //   0x6FAFD467: call     0x6fb1ce40
    //   0x6FAFD46F: cmp      edi, 0x30
    //   0x6FAFD486: call     0x6fafca20
    //   0x6FAFD494: ret
    return 1;
}

// 1.12a: 0x6FAFD4A0..0x6FAFD5D5 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFD4A0(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Data\\Global\\UI\\Bigmenu\\popupok', 'Data\\Global\\UI\\Menu\\okcancelbtn'
    // Callees: D2Win.dll!#10078 -> D2Win_10176, 0x6fb6ef70, D2Win.dll!#10136 -> D2Win_10177, D2gfx.dll!#10078 -> WINDOW_GetWindow, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, 0x6fb1cec0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD4AD: call     0x6fabd3de ; D2Win.dll!#10078 -> D2Win_10176
    //   0x6FAFD4B4: mov      eax, 0x6fb85afc ; STR:'Data\\Global\\UI\\Bigmenu\\popupok'
    //   0x6FAFD4B9: call     0x6fb6ef70
    //   0x6FAFD4C5: mov      eax, 0x6fb85afc ; STR:'Data\\Global\\UI\\Bigmenu\\popupok'
    //   0x6FAFD4CA: call     0x6fb6ef70
    //   0x6FAFD4D6: mov      eax, 0x6fb85adc ; STR:'Data\\Global\\UI\\Menu\\okcancelbtn'
    //   0x6FAFD4DB: call     0x6fb6ef70
    //   0x6FAFD4E7: call     0x6fabd3c6 ; D2Win.dll!#10136 -> D2Win_10177
    //   0x6FAFD500: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FAFD510: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFD51B: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFD525: call     0x6fab326d
    //   0x6FAFD544: call     0x6fb1cec0
    //   0x6FAFD54C: cmp      edi, 0x30
    //   0x6FAFD556: call     0x6fb1cf40
    //   0x6FAFD55B: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FAFD56B: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFD576: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFD580: call     0x6fab326d
    //   0x6FAFD5A4: call     0x6fb1ce40
    //   0x6FAFD5AC: cmp      edi, 0x30
    //   0x6FAFD5C7: call     0x6fafca20
    //   0x6FAFD5D4: ret
    return 1;
}

// 1.12a: 0x6FAFD5E0..0x6FAFD715 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFD5E0(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Data\\Global\\UI\\Bigmenu\\popupok', 'Data\\Global\\UI\\Menu\\okcancelbtn'
    // Callees: D2Win.dll!#10078 -> D2Win_10176, 0x6fb6ef70, D2Win.dll!#10136 -> D2Win_10177, D2gfx.dll!#10078 -> WINDOW_GetWindow, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, 0x6fb1cec0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD5ED: call     0x6fabd3de ; D2Win.dll!#10078 -> D2Win_10176
    //   0x6FAFD5F4: mov      eax, 0x6fb85afc ; STR:'Data\\Global\\UI\\Bigmenu\\popupok'
    //   0x6FAFD5F9: call     0x6fb6ef70
    //   0x6FAFD605: mov      eax, 0x6fb85afc ; STR:'Data\\Global\\UI\\Bigmenu\\popupok'
    //   0x6FAFD60A: call     0x6fb6ef70
    //   0x6FAFD616: mov      eax, 0x6fb85adc ; STR:'Data\\Global\\UI\\Menu\\okcancelbtn'
    //   0x6FAFD61B: call     0x6fb6ef70
    //   0x6FAFD627: call     0x6fabd3c6 ; D2Win.dll!#10136 -> D2Win_10177
    //   0x6FAFD640: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FAFD650: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFD65B: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFD665: call     0x6fab326d
    //   0x6FAFD684: call     0x6fb1cec0
    //   0x6FAFD68C: cmp      edi, 0x30
    //   0x6FAFD696: call     0x6fb1cf40
    //   0x6FAFD69B: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FAFD6AB: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFD6B6: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFD6C0: call     0x6fab326d
    //   0x6FAFD6E4: call     0x6fb1ce40
    //   0x6FAFD6EC: cmp      edi, 0x30
    //   0x6FAFD707: call     0x6fafca20
    //   0x6FAFD714: ret
    return 1;
}

// 1.12a: 0x6FAFD720..0x6FAFD721 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFD720(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD720: ret
    return 1;
}

// 1.12a: 0x6FAFD730..0x6FAFD73A (__usercall, ret 0x0)
int32_t __usercall sub_6FAFD730(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: D2Common.dll!#10087 -> UNITS_GetMeleeRange
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD731: call     0x6fabccbe ; D2Common.dll!#10087 -> UNITS_GetMeleeRange
    //   0x6FAFD739: ret
    return 1;
}

// 1.12a: 0x6FAFD740..0x6FAFD768 (__usercall, ret 0x0)
int32_t __usercall sub_6FAFD740(D2UnitStrc* pUnit /*eax*/, int32_t nArgEdx /*edx*/)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD74D: imul     ecx, eax
    //   0x6FAFD755: imul     ecx
    //   0x6FAFD767: ret
    return 1;
}

// 1.12a: 0x6FAFD770..0x6FAFD79D (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFD770(D2UnitStrc* pUnit)
{
    // Callees: D2Common.dll!#10979 -> COMPOSIT_GetWeaponClassId
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD787: call     0x6fabc40c ; D2Common.dll!#10979 -> COMPOSIT_GetWeaponClassId
    //   0x6FAFD791: cmp      ecx, 7
    //   0x6FAFD79C: ret
    return 1;
}

// 1.12a: 0x6FAFD7A0..0x6FAFD7AB (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFD7A0(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Callees: D2Common.dll!#10155 -> PATH_SetDirection
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD7A5: call     0x6fabc2da ; D2Common.dll!#10155 -> PATH_SetDirection
    //   0x6FAFD7AA: ret
    return 1;
}

// 1.12a: 0x6FAFD7B0..0x6FAFD87E (__stdcall, ret 0x4)
int32_t __stdcall sub_6FAFD7B0(int32_t a1)
{
    // Callees: D2Common.dll!#10099 -> INVENTORY_GetItemFromBodyLoc, D2Common.dll!#10108 -> ITEMS_CanBeEquipped, D2Common.dll!#10681 -> ITEMS_CheckIfThrowable, D2Common.dll!#10231 -> ITEMS_CheckItemTypeId, D2Common.dll!#10646 -> SKILLS_GetSkillById, D2Common.dll!#10620 -> SKILLS_GetParam4, D2Common.dll!#10919 -> SKILLS_SetParam4, D2Common.dll!#10662 -> UNITS_SetUsedSkill
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD7B5: cmp      dword ptr [ebp], 0
    //   0x6FAFD7C7: call     0x6fabc3b8 ; D2Common.dll!#10099 -> INVENTORY_GetItemFromBodyLoc
    //   0x6FAFD7D3: call     0x6fabc3ac ; D2Common.dll!#10108 -> ITEMS_CanBeEquipped
    //   0x6FAFD7DD: call     0x6fabc604 ; D2Common.dll!#10681 -> ITEMS_CheckIfThrowable
    //   0x6FAFD7E2: cmp      eax, 0x26
    //   0x6FAFD7EC: call     0x6fabc3b8 ; D2Common.dll!#10099 -> INVENTORY_GetItemFromBodyLoc
    //   0x6FAFD7F8: call     0x6fabc3ac ; D2Common.dll!#10108 -> ITEMS_CanBeEquipped
    //   0x6FAFD802: call     0x6fabc604 ; D2Common.dll!#10681 -> ITEMS_CheckIfThrowable
    //   0x6FAFD807: cmp      eax, 0x26
    //   0x6FAFD811: call     0x6fabc1d8 ; D2Common.dll!#10231 -> ITEMS_CheckItemTypeId
    //   0x6FAFD81D: call     0x6fabc1d8 ; D2Common.dll!#10231 -> ITEMS_CheckItemTypeId
    //   0x6FAFD82C: call     0x6fabc66a ; D2Common.dll!#10646 -> SKILLS_GetSkillById
    //   0x6FAFD834: call     0x6fabcd00 ; D2Common.dll!#10620 -> SKILLS_GetParam4
    //   0x6FAFD847: call     0x6fabc96a ; D2Common.dll!#10919 -> SKILLS_SetParam4
    //   0x6FAFD855: call     0x6fabc66a ; D2Common.dll!#10646 -> SKILLS_GetSkillById
    //   0x6FAFD85E: call     0x6fabc6ac ; D2Common.dll!#10662 -> UNITS_SetUsedSkill
    //   0x6FAFD866: ret      4
    //   0x6FAFD86C: call     0x6fabc96a ; D2Common.dll!#10919 -> SKILLS_SetParam4
    //   0x6FAFD873: call     0x6fabc6ac ; D2Common.dll!#10662 -> UNITS_SetUsedSkill
    //   0x6FAFD87B: ret      4
    return 1;
}

// 1.12a: 0x6FAFD880..0x6FAFD888 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFD880(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[4]
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD885: ret      4
    return 1;
}

// 1.12a: 0x6FAFD890..0x6FAFD89B (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFD890(D2UnitStrc* pUnit)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD89A: ret
    return 1;
}

// 1.12a: 0x6FAFD8A0..0x6FAFD90C (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFD8A0(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Callees: D2Common.dll!#10692 -> DUNGEON_GetRoomCoordinates
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD8AF: call     0x6fabcb0e ; D2Common.dll!#10692 -> DUNGEON_GetRoomCoordinates
    //   0x6FAFD8BB: cmp      edx, eax
    //   0x6FAFD8C6: cmp      ecx, eax
    //   0x6FAFD8D2: cmp      esi, edx
    //   0x6FAFD8DE: cmp      ebx, ecx
    //   0x6FAFD8E9: cmp      edx, eax
    //   0x6FAFD8F5: cmp      esi, ecx
    //   0x6FAFD903: ret
    //   0x6FAFD90B: ret
    return 1;
}

// 1.12a: 0x6FAFD910..0x6FAFD94F (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFD910(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD91B: imul     eax, ecx
    //   0x6FAFD91E: imul     edx, esi
    //   0x6FAFD923: cmp      eax, 3
    //   0x6FAFD93C: imul     eax, ecx
    //   0x6FAFD93F: imul     edx, esi
    //   0x6FAFD944: cmp      eax, 3
    //   0x6FAFD94E: ret
    return 1;
}

// 1.12a: 0x6FAFD950..0x6FAFD98D (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFD950(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD955: imul     eax, eax
    //   0x6FAFD95A: imul     edi, ecx
    //   0x6FAFD95F: cmp      eax, 4
    //   0x6FAFD974: imul     eax, eax
    //   0x6FAFD97B: imul     edi, ecx
    //   0x6FAFD980: cmp      eax, 0x10
    //   0x6FAFD98C: ret
    return 1;
}

// 1.12a: 0x6FAFD990..0x6FAFD9E3 (__stdcall, ret 0x4)
int32_t __stdcall sub_6FAFD990(int32_t a1)
{
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    // Callees: D2Common.dll!#10235 -> STATLIST_AllocStatList, D2Common.dll!#10626 -> D2COMMON_10475_PostStatToStatList, D2Common.dll!#11139 -> STATLIST_SetStatIfListIsValid, D2Common.dll!#10883 -> UNITS_UpdateAnimRateAndVelocity
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD9AC: call     0x6fabc6be ; D2Common.dll!#10235 -> STATLIST_AllocStatList
    //   0x6FAFD9BB: call     0x6fabc694 ; D2Common.dll!#10626 -> D2COMMON_10475_PostStatToStatList
    //   0x6FAFD9CA: call     0x6fabc6d6 ; D2Common.dll!#11139 -> STATLIST_SetStatIfListIsValid
    //   0x6FAFD9D4: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FAFD9DA: call     0x6fabc65e ; D2Common.dll!#10883 -> UNITS_UpdateAnimRateAndVelocity
    //   0x6FAFD9E0: ret      4
    return 1;
}

// 1.12a: 0x6FAFD9F0..0x6FAFDAFA (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFD9F0(D2UnitStrc* pUnit, int32_t a1)
{
    // Callees: D2Common.dll!#10804 -> STATES_CheckStateMaskActiveOnUnit, D2Common.dll!#10295 -> STATES_CheckState, D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState, D2Common.dll!#10775 -> STATLIST_GetStatValue
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFD9F6: call     0x6fabcc28 ; D2Common.dll!#10804 -> STATES_CheckStateMaskActiveOnUnit
    //   0x6FAFDA03: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFDA0E: cmp      word ptr [eax + 0x188], bx
    //   0x6FAFDA32: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FAFDA47: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFDA4F: cmp      esi, dword ptr [eax + 0xc4]
    //   0x6FAFDA63: imul     edi, edi, 0x3c
    //   0x6FAFDA7C: cmp      eax, ecx
    //   0x6FAFDA98: call     0x6fabc670 ; D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState
    //   0x6FAFDAAA: call     0x6fabc6e8 ; D2Common.dll!#10775 -> STATLIST_GetStatValue
    //   0x6FAFDAB9: call     0x6fabc6e8 ; D2Common.dll!#10775 -> STATLIST_GetStatValue
    //   0x6FAFDACB: call     dword ptr [eax*4 + 0x6fba3630]
    //   0x6FAFDAD6: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFDAE6: cmp      ebx, edx
    //   0x6FAFDAF7: ret      4
    return 1;
}

// 1.12a: 0x6FAFDB00..0x6FAFDB3F (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFDB00(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFDB0F: cmp      ecx, dword ptr [eax + 0x10]
    //   0x6FAFDB14: cmp      ecx, dword ptr [eax + 4]
    //   0x6FAFDB2A: ret
    //   0x6FAFDB2B: cmp      ecx, dword ptr [eax + 0xc]
    //   0x6FAFDB3E: ret
    return 1;
}

// 1.12a: 0x6FAFDB40..0x6FAFDC31 (__usercall, ret 0x0)
int32_t __usercall sub_6FAFDB40(D2UnitStrc* pUnit /*eax*/)
{
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    // Callees: D2Common.dll!#10205 -> D2Common_10434, D2Common.dll!#10413 -> ITEMS_GetBaseCode, D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill, D2Common.dll!#11034 -> STATLIST_UnitGetItemStatOrSkillStatValue, D2Common.dll!#10099 -> INVENTORY_GetItemFromBodyLoc, D2Common.dll!#11025 -> ITEMS_CheckIfSocketable, D2Common.dll!#10603 -> ITEMS_CheckIfStackable
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFDB48: ret
    //   0x6FAFDB49: cmp      dword ptr [esi], 0
    //   0x6FAFDB54: ret
    //   0x6FAFDB5A: call     0x6fabccb8 ; D2Common.dll!#10205 -> D2Common_10434
    //   0x6FAFDB6B: call     0x6fabc550 ; D2Common.dll!#10413 -> ITEMS_GetBaseCode
    //   0x6FAFDB79: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FAFDB81: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FAFDB87: call     0x6fabc2ec ; D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill
    //   0x6FAFDB97: call     0x6fabc5a4 ; D2Common.dll!#11034 -> STATLIST_UnitGetItemStatOrSkillStatValue
    //   0x6FAFDBA8: ret
    //   0x6FAFDBB4: call     0x6fabc3b8 ; D2Common.dll!#10099 -> INVENTORY_GetItemFromBodyLoc
    //   0x6FAFDBC3: cmp      esi, ebx
    //   0x6FAFDBC8: call     0x6fabc9b2 ; D2Common.dll!#11025 -> ITEMS_CheckIfSocketable
    //   0x6FAFDBD2: call     0x6fabc54a ; D2Common.dll!#10603 -> ITEMS_CheckIfStackable
    //   0x6FAFDBDF: call     0x6fabc5a4 ; D2Common.dll!#11034 -> STATLIST_UnitGetItemStatOrSkillStatValue
    //   0x6FAFDBE9: call     0x6fabcd12 ; D2Common.dll!#11040 -> ITEMS_CanItemBeUsedForThrowSkill
    //   0x6FAFDBF2: ret
    //   0x6FAFDBF6: call     0x6fabc3b8 ; D2Common.dll!#10099 -> INVENTORY_GetItemFromBodyLoc
    //   0x6FAFDC05: cmp      esi, ebx
    //   0x6FAFDC0A: call     0x6fabc9b2 ; D2Common.dll!#11025 -> ITEMS_CheckIfSocketable
    //   0x6FAFDC14: call     0x6fabc54a ; D2Common.dll!#10603 -> ITEMS_CheckIfStackable
    //   0x6FAFDC21: call     0x6fabc5a4 ; D2Common.dll!#11034 -> STATLIST_UnitGetItemStatOrSkillStatValue
    //   0x6FAFDC30: ret
    return 1;
}

// 1.12a: 0x6FAFDC40..0x6FAFDD46 (__usercall, ret 0x8)
int32_t __usercall sub_6FAFDC40(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2)
{
    // Callees: D2Common.dll!#11128 -> UNITS_GetDirection, D2Common.dll!#10191 -> PATH_GetXPosition, D2Common.dll!#10178 -> PATH_GetYPosition
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFDC49: call     0x6fabc298 ; D2Common.dll!#11128 -> UNITS_GetDirection
    //   0x6FAFDC75: cmp      eax, 2
    //   0x6FAFDCB2: cmp      eax, 3
    //   0x6FAFDCB7: cmp      eax, 5
    //   0x6FAFDCBF: cmp      eax, ebx
    //   0x6FAFDCC4: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FAFDCE3: cmp      eax, 2
    //   0x6FAFDCE8: cmp      eax, 3
    //   0x6FAFDCED: cmp      eax, 5
    //   0x6FAFDCF5: cmp      esi, ebx
    //   0x6FAFDCFA: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FAFDD11: ret      8
    //   0x6FAFDD28: ret      8
    //   0x6FAFDD43: ret      8
    return 1;
}

// 1.12a: 0x6FAFDD50..0x6FAFDD83 (__fastcall, ret 0xC)
int32_t __fastcall sub_6FAFDD50(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: 0x6fb6a300
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFDD54: cmp      eax, 2
    //   0x6FAFDD76: call     0x6fb6a300
    //   0x6FAFDD7B: ret      0xc
    //   0x6FAFDD80: ret      0xc
    return 1;
}

// 1.12a: 0x6FAFDD90..0x6FAFDEF1 (__stdcall, ret 0xC)
int32_t __stdcall sub_6FAFDD90(int32_t a1, int32_t a2, int32_t a3)
{
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    // Callees: Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10427 -> SKILLS_GetNextSkill, D2Common.dll!#10002 -> SKILLS_GetFirstSkillFromSkillList, D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill, 0x6fab1d40, D2Common.dll!#10904 -> SKILLS_GetSkillLevel
    int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFDDBC: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFDDC7: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFDDD1: call     0x6fab326d
    //   0x6FAFDDD8: call     0x6fabccfa ; D2Common.dll!#10427 -> SKILLS_GetNextSkill
    //   0x6FAFDDF1: call     0x6fabcc8e ; D2Common.dll!#10002 -> SKILLS_GetFirstSkillFromSkillList
    //   0x6FAFDDFB: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FAFDE05: call     0x6fabc2ec ; D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill
    //   0x6FAFDE14: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFDE1B: cmp      esi, dword ptr [eax + 0xba0]
    //   0x6FAFDE2F: imul     edi, edi, 0x23c
    //   0x6FAFDE3F: call     0x6fab1d40
    //   0x6FAFDE54: call     0x6fabc292 ; D2Common.dll!#10904 -> SKILLS_GetSkillLevel
    //   0x6FAFDE5E: call     0x6fabccc4 ; D2Common.dll!#10064 -> SKILLS_IsEnhanceable
    //   0x6FAFDE76: cmp      ecx, eax
    //   0x6FAFDE7E: cmp      al, 4
    //   0x6FAFDE8E: cmp      ecx, edx
    //   0x6FAFDE93: call     0x6fabc8c2 ; D2Common.dll!#10847 -> SKILLS_GetOwnerGUIDFromSkill
    //   0x6FAFDE98: cmp      eax, -1
    //   0x6FAFDEA4: call     0x6fabcce2 ; D2Common.dll!#10525 -> D2Common_11031
    //   0x6FAFDEAF: call     0x6fabc66a ; D2Common.dll!#10646 -> SKILLS_GetSkillById
    //   0x6FAFDEBC: call     0x6fabc292 ; D2Common.dll!#10904 -> SKILLS_GetSkillLevel
    //   0x6FAFDECA: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FAFDED1: call     0x6fabc98e ; D2Common.dll!#10853 -> SKILLS_RemoveSkill
    //   0x6FAFDEE5: call     0x6fabc6b8 ; D2Common.dll!#10419 -> SKILLS_RefreshPassiveSkills
    //   0x6FAFDEEE: ret      0xc
    return 1;
}

// 1.12a: 0x6FAFDF00..0x6FAFDFB4 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFDF00(D2UnitStrc* pUnit, int32_t a1)
{
    // Callees: 0x6fad4b20
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFDF61: call     0x6fad4b20
    //   0x6FAFDF69: cmp      edi, 0x100
    //   0x6FAFDF74: ret      4
    //   0x6FAFDF86: cmp      dword ptr [esi + 0x10], ebx
    //   0x6FAFDF9F: call     0x6fad4b20
    //   0x6FAFDFA7: cmp      edi, 0x100
    //   0x6FAFDFB1: ret      4
    return 1;
}

// 1.12a: 0x6FAFDFC0..0x6FAFE06B (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFDFC0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[3]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, 0x6fb73130
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFDFC5: call     0x6faf0d20
    //   0x6FAFDFD6: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFDFE1: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFDFEB: call     0x6fab326d
    //   0x6FAFDFF1: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FAFDFFD: ret      4
    //   0x6FAFE000: cmp      dword ptr [eax], 1
    //   0x6FAFE011: cmp      dword ptr [eax + 8], ecx
    //   0x6FAFE020: cmp      eax, dword ptr [esi + 0xc]
    //   0x6FAFE025: call     0x6fb73130
    //   0x6FAFE02E: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFE036: cmp      eax, dword ptr [ecx + 0xbf0]
    //   0x6FAFE044: imul     eax, eax, 0xe0
    //   0x6FAFE053: mov      edx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFE063: ret      4
    return 1;
}

// 1.12a: 0x6FAFE070..0x6FAFE094 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFE070(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[2]
    // Callees: 0x6fafdb40, D2Common.dll!#10205 -> D2Common_10434
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE075: call     0x6fafdb40
    //   0x6FAFE07F: ret      4
    //   0x6FAFE085: call     0x6fabccb8 ; D2Common.dll!#10205 -> D2Common_10434
    //   0x6FAFE091: ret      4
    return 1;
}

// 1.12a: 0x6FAFE0A0..0x6FAFE0DF (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFE0A0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[1]
    // Callees: D2Common.dll!#10684 -> COMPOSIT_IsWeaponBowOrXBow, 0x6fafdb40, D2Common.dll!#10205 -> D2Common_10434, D2Common.dll!#10231 -> ITEMS_CheckItemTypeId
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE0A4: call     0x6fabccdc ; D2Common.dll!#10684 -> COMPOSIT_IsWeaponBowOrXBow
    //   0x6FAFE0AF: call     0x6fafdb40
    //   0x6FAFE0BB: ret      4
    //   0x6FAFE0C1: call     0x6fabccb8 ; D2Common.dll!#10205 -> D2Common_10434
    //   0x6FAFE0CD: call     0x6fabc1d8 ; D2Common.dll!#10231 -> ITEMS_CheckItemTypeId
    //   0x6FAFE0DC: ret      4
    return 1;
}

// 1.12a: 0x6FAFE0E0..0x6FAFE17A (__usercall, ret 0x0)
int32_t __usercall sub_6FAFE0E0(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE0E5: call     0x6faf0d20
    //   0x6FAFE0F6: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFE101: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFE10B: call     0x6fab326d
    //   0x6FAFE111: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FAFE120: ret
    //   0x6FAFE125: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFE12D: cmp      esi, dword ptr [ecx + 0xba0]
    //   0x6FAFE13B: imul     esi, esi, 0x23c
    //   0x6FAFE14A: ret
    //   0x6FAFE14B: mov      edx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFE15B: cmp      ecx, 1
    //   0x6FAFE160: cmp      dword ptr [eax + 0x10], 0xc
    //   0x6FAFE172: ret
    //   0x6FAFE179: ret
    return 1;
}

// 1.12a: 0x6FAFE180..0x6FAFE252 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FAFE180(void)
{
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, 0x6fab13d0, D2Common.dll!#10684 -> COMPOSIT_IsWeaponBowOrXBow, D2Common.dll!#10130 -> INVENTORY_GetEquippedWeapon
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE18B: call     0x6faf0d20
    //   0x6FAFE19C: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFE1A7: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFE1B1: call     0x6fab326d
    //   0x6FAFE1B8: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FAFE1C6: cmp      dword ptr [eax], edi
    //   0x6FAFE1CD: call     0x6fab13d0
    //   0x6FAFE1D5: cmp      al, 2
    //   0x6FAFE1E0: cmp      al, 1
    //   0x6FAFE1E7: call     0x6fabccdc ; D2Common.dll!#10684 -> COMPOSIT_IsWeaponBowOrXBow
    //   0x6FAFE20A: call     0x6fabccd0 ; D2Common.dll!#10130 -> INVENTORY_GetEquippedWeapon
    //   0x6FAFE21F: ret
    //   0x6FAFE23D: cmp      eax, 0x20
    //   0x6FAFE251: ret
    return 1;
}

// 1.12a: 0x6FAFE260..0x6FAFE316 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFE260(D2UnitStrc* pUnit)
{
    // Callees: 0x6fb0eaf0, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10918 -> D2Common_10958, 0x6fb0e500
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE26A: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFE271: cmp      esi, dword ptr [eax + 0xb6c]
    //   0x6FAFE27F: imul     esi, esi, 0x1a4
    //   0x6FAFE2BD: call     0x6fb0eaf0
    //   0x6FAFE2CE: cmp      ecx, 0x12
    //   0x6FAFE2E1: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFE2EC: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFE2F6: call     0x6fab326d
    //   0x6FAFE303: call     0x6fabcb92 ; D2Common.dll!#10918 -> D2Common_10958
    //   0x6FAFE30E: call     0x6fb0e500
    //   0x6FAFE315: ret
    return 1;
}

// 1.12a: 0x6FAFE320..0x6FAFE3E9 (__stdcall, ret 0x4)
int32_t __stdcall sub_6FAFE320(int32_t a1)
{
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10191 -> PATH_GetXPosition, D2Common.dll!#10178 -> PATH_GetYPosition, D2Common.dll!#10024 -> D2COMMON_10175_PathGetFirstPointX
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE327: call     0x6faf0d20
    //   0x6FAFE338: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFE343: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFE34D: call     0x6fab326d
    //   0x6FAFE354: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FAFE361: cmp      eax, 2
    //   0x6FAFE366: cmp      eax, 3
    //   0x6FAFE36B: cmp      eax, 5
    //   0x6FAFE378: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FAFE38D: cmp      eax, 2
    //   0x6FAFE392: cmp      eax, 3
    //   0x6FAFE397: cmp      eax, 5
    //   0x6FAFE3A4: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FAFE3BB: call     0x6fabc334 ; D2Common.dll!#10024 -> D2COMMON_10175_PathGetFirstPointX
    //   0x6FAFE3C3: call     0x6fabc34c ; D2Common.dll!#10728 -> D2COMMON_10176_PathGetFirstPointY
    //   0x6FAFE3CB: cmp      dword ptr [edi], 0
    //   0x6FAFE3D1: cmp      dword ptr [ebp], 0
    //   0x6FAFE3DB: call     0x6fafdc40
    //   0x6FAFE3E6: ret      4
    return 1;
}

// 1.12a: 0x6FAFE3F0..0x6FAFE4A3 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FAFE3F0(D2UnitStrc* pUnit)
{
    // Callees: D2Common.dll!#10366 -> UNITS_GetRoom, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, 0x6fafe320, D2Common.dll!#10759 -> COLLISION_CheckMaskWithSizeXY
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE3F9: cmp      eax, edi
    //   0x6FAFE3FF: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFE407: cmp      eax, dword ptr [ecx + 0xb6c]
    //   0x6FAFE415: imul     eax, eax, 0x1a4
    //   0x6FAFE424: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FAFE429: cmp      esi, edi
    //   0x6FAFE43C: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFE447: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFE451: call     0x6fab326d
    //   0x6FAFE456: cmp      ebp, edi
    //   0x6FAFE463: call     0x6fafe320
    //   0x6FAFE486: call     0x6fabccca ; D2Common.dll!#10759 -> COLLISION_CheckMaskWithSizeXY
    //   0x6FAFE498: ret
    //   0x6FAFE4A2: ret
    return 1;
}

// 1.12a: 0x6FAFE4B0..0x6FAFE59A (__usercall, ret 0x14)
int32_t __usercall sub_6FAFE4B0(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5)
{
    // Callees: D2Common.dll!#10191 -> PATH_GetXPosition, D2Common.dll!#10178 -> PATH_GetYPosition, 0x6fafdf00
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE4CF: cmp      eax, 2
    //   0x6FAFE4DC: cmp      eax, 3
    //   0x6FAFE4E1: cmp      eax, 5
    //   0x6FAFE4EE: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FAFE505: cmp      eax, 2
    //   0x6FAFE50A: cmp      eax, 3
    //   0x6FAFE50F: cmp      eax, 5
    //   0x6FAFE51C: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FAFE55D: cmp      eax, -1
    //   0x6FAFE568: call     0x6fafdf00
    //   0x6FAFE572: ret      0x14
    //   0x6FAFE58D: call     0x6fafdf00
    //   0x6FAFE597: ret      0x14
    return 1;
}

// 1.12a: 0x6FAFE5A0..0x6FAFE614 (__fastcall, ret 0x0)
// 1.12a: 0x6FAFE5D0 (internal entry point sub_6FAFE5D0)
int32_t __fastcall sub_6FAFE5A0(D2UnitStrc* pUnit)
{
    // Callees: 0x6fb281c0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE5B1: cmp      dword ptr [eax], 1
    //   0x6FAFE5C2: cmp      dword ptr [eax + 8], esi
    //   0x6FAFE5CF: ret
    //   0x6FAFE5D3: cmp      esi, -1
    //   0x6FAFE5DB: ret
    //   0x6FAFE5E0: call     0x6fb281c0
    //   0x6FAFE5E9: call     0x6fb281c0
    //   0x6FAFE5FA: cmp      di, word ptr [eax + 0x22]
    //   0x6FAFE600: cmp      di, 0xffff
    //   0x6FAFE60B: ret
    //   0x6FAFE613: ret
    return 1;
}

// 1.12a: 0x6FAFE620..0x6FAFE681 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFE620(D2UnitStrc* pUnit, int32_t a1)
{
    // Callees: 0x6fafe320, D2Common.dll!#10666 -> UNITS_GetDistanceToCoordinates, D2Common.dll!#10299 -> UNITS_SetAnimationSpeed, D2Common.dll!#10017 -> MISSILE_SetCurrentFrame
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE633: call     0x6fafe320
    //   0x6FAFE648: call     0x6fabc32e ; D2Common.dll!#10666 -> UNITS_GetDistanceToCoordinates
    //   0x6FAFE64F: cmp      esi, 1
    //   0x6FAFE663: call     0x6fabc268 ; D2Common.dll!#10299 -> UNITS_SetAnimationSpeed
    //   0x6FAFE66D: call     0x6fabc256 ; D2Common.dll!#10017 -> MISSILE_SetCurrentFrame
    //   0x6FAFE67E: ret      4
    return 1;
}

// 1.12a: 0x6FAFE690..0x6FAFE70C (__usercall, ret 0xC)
int32_t __usercall sub_6FAFE690(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: 0x6fafe320, 0x6fab15c0, 0x6fab1590, D2Common.dll!#11031 -> PATH_ComputeSquaredDistance, 0x6fad1940
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE6A0: call     0x6fafe320
    //   0x6FAFE6AE: ret      0xc
    //   0x6FAFE6C6: call     0x6fab15c0
    //   0x6FAFE6CE: call     0x6fab1590
    //   0x6FAFE6D4: call     0x6fabcc9a ; D2Common.dll!#11031 -> PATH_ComputeSquaredDistance
    //   0x6FAFE6D9: cmp      eax, 0x2710
    //   0x6FAFE6F3: call     0x6fad1940
    //   0x6FAFE6FE: ret      0xc
    //   0x6FAFE709: ret      0xc
    return 1;
}

// 1.12a: 0x6FAFE710..0x6FAFE766 (__usercall, ret 0xC)
int32_t __usercall sub_6FAFE710(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: 0x6fafe320, 0x6fad1e80
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE723: call     0x6fafe320
    //   0x6FAFE74D: call     0x6fad1e80
    //   0x6FAFE758: ret      0xc
    //   0x6FAFE763: ret      0xc
    return 1;
}

// 1.12a: 0x6FAFE770..0x6FAFEB9A (__stdcall, ret 0x20)
int32_t __stdcall sub_6FAFE770(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8)
{
    // Callees: D2Common.dll!#10455 -> MONSTERS_IsUndead, D2Common.dll!#10513 -> MONSTERS_IsBoss, D2Common.dll!#10514 -> MONSTERS_IsPrimeEvil, 0x6fab1480, D2Common.dll!#10366 -> UNITS_GetRoom, D2Common.dll!#10438 -> DUNGEON_IsRoomInTown, 0x6fafe5a0, 0x6fab1390
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFE782: cmp      eax, 4
    //   0x6FAFE7A3: cmp      eax, 0x11
    //   0x6FAFE7B2: ret      4
    //   0x6FAFE7B5: cmp      eax, 0x11
    //   0x6FAFE7C8: ret      4
    //   0x6FAFE7DC: cmp      eax, 0xc
    //   0x6FAFE7E7: ret      4
    //   0x6FAFE7EA: cmp      eax, 0xc
    //   0x6FAFE7F9: call     0x6fabc910 ; D2Common.dll!#10455 -> MONSTERS_IsUndead
    //   0x6FAFE80A: call     0x6fabccf4 ; D2Common.dll!#10513 -> MONSTERS_IsBoss
    //   0x6FAFE81C: call     0x6fabcce8 ; D2Common.dll!#10514 -> MONSTERS_IsPrimeEvil
    //   0x6FAFE82B: ret      4
    //   0x6FAFE839: ret      4
    //   0x6FAFE844: call     0x6fab1480
    //   0x6FAFE84D: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFE861: ret      4
    //   0x6FAFE89C: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FAFE8A2: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FAFE8B7: call     0x6fafe5a0
    //   0x6FAFE8C0: cmp      dword ptr [esi], 1
    //   0x6FAFE8C8: call     0x6fab1390
    //   0x6FAFE8D4: mov      edx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFE8E9: call     0x6facf980
    //   0x6FAFE902: call     0x6fabc724 ; D2Common.dll!#10974 -> UNITS_IsInMeleeRange
    //   0x6FAFE91A: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FAFE92E: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FAFE940: ret      4
    //   0x6FAFE944: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FAFE94F: call     0x6fab1590
    //   0x6FAFE95A: call     0x6fab15c0
    //   0x6FAFE965: call     0x6fab1590
    //   0x6FAFE970: call     0x6fab15c0
    //   0x6FAFE986: call     0x6fabccee ; D2Common.dll!#10925 -> COLLISION_RayTrace
    //   0x6FAFE999: ret      4
    //   0x6FAFE9A0: retf
    //   0x6FAFE9A4: call     0x57ec5959
    //   0x6FAFE9AC: call     0x5c335961
    //   0x6FAFE9BB: cmp      ebx, ebp
    //   0x6FAFE9C3: cmp      dword ptr [esp + 0x3c], ebp
    //   0x6FAFE9D2: cmp      edi, ebp
    //   0x6FAFE9DC: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FAFE9E3: cmp      esi, ebp
    //   0x6FAFE9EB: cmp      dword ptr [esp + 0x34], ebp
    //   0x6FAFE9F1: cmp      dword ptr [esp + 0x38], ebp
    //   0x6FAFE9F9: call     0x6fab1590
    //   0x6FAFEA04: call     0x6fab15c0
    //   0x6FAFEA0D: cmp      dword ptr [esp + 0x40], ebp
    //   0x6FAFEA25: imul     eax, edi
    //   0x6FAFEA3F: call     0x6fabc6a0 ; D2Common.dll!#10915 -> DUNGEON_GetAdjacentRoomsListFromRoom
    //   0x6FAFEA73: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FAFEA8F: call     0x6fafd8a0
    //   0x6FAFEAB0: cmp      esi, ebx
    //   0x6FAFEAC0: cmp      eax, 2
    //   0x6FAFEAC5: cmp      eax, 3
    //   0x6FAFEACA: cmp      eax, 5
    //   0x6FAFEAD7: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FAFEAEC: cmp      eax, 2
    //   0x6FAFEAF1: cmp      eax, 3
    //   0x6FAFEAF6: cmp      eax, 5
    //   0x6FAFEB03: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FAFEB20: call     0x6fabcc9a ; D2Common.dll!#11031 -> PATH_ComputeSquaredDistance
    //   0x6FAFEB25: cmp      dword ptr [esp + 0x1c], eax
    //   0x6FAFEB30: call     0x6fafe770
    //   0x6FAFEB41: cmp      dword ptr [esi], 1
    //   0x6FAFEB4E: call     0x6fab2d40
    //   0x6FAFEB5D: call     dword ptr [esp + 0x44]
    //   0x6FAFEB84: cmp      ebp, eax
    //   0x6FAFEB97: ret      0x20
    return 1;
}

// 1.12a: 0x6FAFEBA0..0x6FAFEC15 (__fastcall, ret 0xC)
int32_t __fastcall sub_6FAFEBA0(D2UnitStrc* pUnit, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: 0x6fafe9b0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFEBF5: call     0x6fafe9b0
    //   0x6FAFEC12: ret      0xc
    return 1;
}

// 1.12a: 0x6FAFEC20..0x6FAFEC57 (__fastcall, ret 0xC)
int32_t __fastcall sub_6FAFEC20(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: 0x6fafe9b0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFEC4B: call     0x6fafe9b0
    //   0x6FAFEC54: ret      0xc
    return 1;
}

// 1.12a: 0x6FAFEC60..0x6FAFEE34 (__usercall, ret 0x18)
int32_t __usercall sub_6FAFEC60(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    // Callees: 0x6fafe320, 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit, D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFEC7B: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFEC82: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FAFEC94: imul     ebp, ebp, 0x23c
    //   0x6FAFECB6: call     0x6fafe320
    //   0x6FAFECC1: call     0x6faf0d20
    //   0x6FAFECD2: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFECDD: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFECE7: call     0x6fab326d
    //   0x6FAFECED: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FAFED00: call     0x6fabcac0 ; D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    //   0x6FAFED2D: call     0x6fabc2e6 ; D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos
    //   0x6FAFED5E: cmp      eax, ecx
    //   0x6FAFED80: call     eax
    //   0x6FAFED9C: call     0x6fae1fa0
    //   0x6FAFEDA1: cmp      word ptr [ebp + 0x10a], 0
    //   0x6FAFEDAB: call     0x6facf5b0
    //   0x6FAFEDC2: call     0x6fae1fa0
    //   0x6FAFEDC7: cmp      word ptr [ebp + 0x108], 0
    //   0x6FAFEDD1: call     0x6facf5b0
    //   0x6FAFEDF0: call     0x6fb430e0
    //   0x6FAFEDFB: call     0x6fabcac0 ; D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    //   0x6FAFEE1E: call     0x6fabc2e6 ; D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos
    //   0x6FAFEE31: ret      0x18
    return 1;
}

// 1.12a: 0x6FAFEE40..0x6FAFF378 (__usercall, ret 0xC)
int32_t __usercall sub_6FAFEE40(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3)
{
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    // Callees: 0x6fafe320, D2Common.dll!#10056 -> D2Common_11026, 0x6fafe0e0, D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill, D2Common.dll!#10646 -> SKILLS_GetSkillById, D2Common.dll!#10858 -> SKILLS_AddSkill, D2Common.dll!#10140 -> SKILLS_SetFlags
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFEE4F: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFEE56: cmp      ebp, dword ptr [ecx + 0xba0]
    //   0x6FAFEE66: imul     eax, eax, 0x23c
    //   0x6FAFEE7B: ret      0xc
    //   0x6FAFEE95: call     0x6fafe320
    //   0x6FAFEEAA: cmp      eax, 4
    //   0x6FAFEEE7: call     0x6fabcd06 ; D2Common.dll!#10056 -> D2Common_11026
    //   0x6FAFEEF8: call     0x6fafe0e0
    //   0x6FAFEF06: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FAFEF16: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FAFEF1C: call     0x6fabc2ec ; D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill
    //   0x6FAFEF21: cmp      eax, ebp
    //   0x6FAFEF32: call     0x6fabc66a ; D2Common.dll!#10646 -> SKILLS_GetSkillById
    //   0x6FAFEF43: call     0x6fabccd6 ; D2Common.dll!#10858 -> SKILLS_AddSkill
    //   0x6FAFEF51: call     0x6fabc66a ; D2Common.dll!#10646 -> SKILLS_GetSkillById
    //   0x6FAFEF5C: cmp      esi, dword ptr [0x6fbcc3d0]
    //   0x6FAFEF64: cmp      edi, -1
    //   0x6FAFEF6C: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FAFEF73: call     0x6fab1aa0
    //   0x6FAFEF7E: cmp      eax, edi
    //   0x6FAFEF87: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FAFEF91: call     0x6fabc30a ; D2Common.dll!#10210 -> SKILLS_AssignSkill
    //   0x6FAFEF9F: call     0x6fabc682 ; D2Common.dll!#10394 -> D2Common_11030
    //   0x6FAFEFAB: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FAFEFB5: call     0x6fabc30a ; D2Common.dll!#10210 -> SKILLS_AssignSkill
    //   0x6FAFEFC4: call     0x6fabc6ac ; D2Common.dll!#10662 -> UNITS_SetUsedSkill
    //   0x6FAFEFC9: cmp      dword ptr [esp + 0x40], -1
    //   0x6FAFEFE5: call     0x6fabc9be ; D2Common.dll!#10146 -> SKILLS_GetSkillInfo
    //   0x6FAFEFFE: cmp      esi, dword ptr [0x6fbcc3d0]
    //   0x6FAFF008: call     0x6fab1aa0
    //   0x6FAFF013: cmp      eax, edi
    //   0x6FAFF01C: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FAFF026: call     0x6fabc30a ; D2Common.dll!#10210 -> SKILLS_AssignSkill
    //   0x6FAFF034: call     0x6fabc682 ; D2Common.dll!#10394 -> D2Common_11030
    //   0x6FAFF040: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FAFF04A: call     0x6fabc30a ; D2Common.dll!#10210 -> SKILLS_AssignSkill
    //   0x6FAFF055: call     0x6fabc292 ; D2Common.dll!#10904 -> SKILLS_GetSkillLevel
    //   0x6FAFF05E: call     0x6facf5b0
    //   0x6FAFF06E: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFF07B: call     0x6facf7b0
    //   0x6FAFF088: mov      eax, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFF095: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FAFF09B: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FAFF0A7: call     0x6fabc6ac ; D2Common.dll!#10662 -> UNITS_SetUsedSkill
    //   0x6FAFF0D7: call     0x6faf1450
    //   0x6FAFF0DC: cmp      esi, dword ptr [0x6fbcc3d0]
    //   0x6FAFF0E7: call     0x6fae7250
    //   0x6FAFF0F5: ret      0xc
    //   0x6FAFF0FC: cmp      byte ptr [esi], 0
    //   0x6FAFF105: call     0x6fafe180
    //   0x6FAFF10E: call     0x6fabcc76 ; D2Common.dll!#10991 -> SKILLS_GetSkillModeFromUnit
    //   0x6FAFF11E: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FAFF127: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFF13F: cmp      eax, 1
    //   0x6FAFF149: call     0x6faf13c0
    //   0x6FAFF16F: cmp      eax, ecx
    //   0x6FAFF187: call     eax
    //   0x6FAFF193: call     0x6fabc6ac ; D2Common.dll!#10662 -> UNITS_SetUsedSkill
    //   0x6FAFF1A2: cmp      eax, 1
    //   0x6FAFF1AC: call     0x6faf13c0
    //   0x6FAFF1B8: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFF1C7: cmp      dword ptr [esp + 0x40], -1
    //   0x6FAFF1F3: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFF200: call     0x6fabcd0c ; D2Common.dll!#11081 -> UNITS_GetHitClass
    //   0x6FAFF207: call     0x6fae6720
    //   0x6FAFF22B: call     0x6fae1fa0
    //   0x6FAFF234: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFF244: call     0x6fabcd0c ; D2Common.dll!#11081 -> UNITS_GetHitClass
    //   0x6FAFF24B: call     0x6fae6780
    //   0x6FAFF258: call     0x6fabcd0c ; D2Common.dll!#11081 -> UNITS_GetHitClass
    //   0x6FAFF25F: call     0x6fae6720
    //   0x6FAFF274: call     0x6fae1fa0
    //   0x6FAFF279: cmp      dword ptr [esp + 0x40], -1
    //   0x6FAFF287: cmp      word ptr [ebp + 0x122], 0
    //   0x6FAFF29A: cmp      dword ptr [esi], 0
    //   0x6FAFF2A6: cmp      eax, ecx
    //   0x6FAFF2AA: mov      eax, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFF2B9: call     0x6fabcd0c ; D2Common.dll!#11081 -> UNITS_GetHitClass
    //   0x6FAFF2C0: call     0x6fae6720
    //   0x6FAFF2E4: call     0x6fae1fa0
    //   0x6FAFF2E9: cmp      dword ptr [esp + 0x40], -1
    //   0x6FAFF30A: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFF312: cmp      edi, dword ptr [eax + 0xbc0]
    //   0x6FAFF31E: call     0x6fb40a60
    //   0x6FAFF331: call     0x6fb430e0
    //   0x6FAFF34F: call     0x6fafe260
    //   0x6FAFF35F: ret      0xc
    //   0x6FAFF370: iretd
    return 1;
}

// 1.12a: 0x6FAFF380..0x6FAFF43B (__usercall, ret 0x4)
int32_t __usercall sub_6FAFF380(D2UnitStrc* pUnit /*eax*/, int32_t a1)
{
    // Callees: D2Common.dll!#10498 -> UNITS_CanDualWield, 0x6fafd7b0, D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos, D2Common.dll!#10990 -> UNITS_UpdateDirectionAndSpeed, D2Common.dll!#10944 -> PATH_GetNewDirection, D2Common.dll!#11130 -> D2COMMON_10160_PathUpdateDirection, D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit, 0x6fafee40
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFF391: call     0x6fabc79c ; D2Common.dll!#10498 -> UNITS_CanDualWield
    //   0x6FAFF39B: call     0x6fafd7b0
    //   0x6FAFF3A6: call     0x6fabc2e6 ; D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos
    //   0x6FAFF3AE: call     0x6fabc2a4 ; D2Common.dll!#10990 -> UNITS_UpdateDirectionAndSpeed
    //   0x6FAFF3B5: cmp      eax, 1
    //   0x6FAFF3C1: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFF3C9: cmp      eax, dword ptr [ecx + 0xa80]
    //   0x6FAFF3D7: imul     eax, eax, 0x1a8
    //   0x6FAFF3E3: cmp      dword ptr [eax + 0x10], 0x20204250
    //   0x6FAFF3F0: call     0x6fabc730 ; D2Common.dll!#10944 -> PATH_GetNewDirection
    //   0x6FAFF414: call     0x6fabc250 ; D2Common.dll!#11130 -> D2COMMON_10160_PathUpdateDirection
    //   0x6FAFF41C: call     0x6fabcac0 ; D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    //   0x6FAFF42F: call     0x6fafee40
    //   0x6FAFF438: ret      4
    return 1;
}

// 1.12a: 0x6FAFF440..0x6FAFF44F (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFF440(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1)
{
    // Callees: 0x6fafee40
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFF447: call     0x6fafee40
    //   0x6FAFF44C: ret      4
    return 1;
}

// 1.12a: 0x6FAFF450..0x6FAFF684 (__fastcall, ret 0x4)
// 1.12a: 0x6FAFF4D0 (internal entry point sub_6FAFF4D0)
int32_t __fastcall sub_6FAFF450(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[1]
    // Callees: D2Common.dll!#10684 -> COMPOSIT_IsWeaponBowOrXBow, D2Common.dll!#10074 -> D2COMMON_11039_CheckWeaponIsMissileBased, 0x6fafe710, 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFF462: call     0x6fabccdc ; D2Common.dll!#10684 -> COMPOSIT_IsWeaponBowOrXBow
    //   0x6FAFF471: call     0x6fabc766 ; D2Common.dll!#10074 -> D2COMMON_11039_CheckWeaponIsMissileBased
    //   0x6FAFF476: cmp      eax, -1
    //   0x6FAFF484: call     0x6fafe710
    //   0x6FAFF490: ret      4
    //   0x6FAFF495: call     0x6faf0d20
    //   0x6FAFF4A6: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFF4B1: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFF4BB: call     0x6fab326d
    //   0x6FAFF4C1: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FAFF4CD: ret      4
    //   0x6FAFF4FF: cmp      ecx, edx
    //   0x6FAFF520: cmp      ebp, 3
    //   0x6FAFF584: cmp      ebx, edx
    //   0x6FAFF65C: call     0x6fad4b20
    //   0x6FAFF670: ret      4
    //   0x6FAFF678: cmp      esi, ebp
    return 1;
}

// 1.12a: 0x6FAFF690..0x6FAFF70B (__usercall, ret 0x14)
int32_t __usercall sub_6FAFF690(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5)
{
    // Callees: 0x6fafe320, 0x6fad4b20
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFF6B0: call     0x6fafe320
    //   0x6FAFF6FE: call     0x6fad4b20
    //   0x6FAFF708: ret      0x14
    return 1;
}

// 1.12a: 0x6FAFF710..0x6FAFF7F9 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FAFF710(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1)
{
    // Callees: Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, 0x6fad4b20
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFF71B: imul     ebp
    //   0x6FAFF726: cmp      eax, 7
    //   0x6FAFF730: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FAFF73B: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FAFF745: call     0x6fab326d
    //   0x6FAFF75B: call     0x6fad4b20
    //   0x6FAFF77A: imul     ecx, dword ptr [ebx + 0x6fb85854]
    //   0x6FAFF78A: imul     edx, dword ptr [ebx + 0x6fb85838]
    //   0x6FAFF796: call     0x6fad4b20
    //   0x6FAFF7A1: imul     eax, dword ptr [ebx + 0x6fb85838]
    //   0x6FAFF7B1: imul     ecx, dword ptr [ebx + 0x6fb85854]
    //   0x6FAFF7BD: call     0x6fad4b20
    //   0x6FAFF7C8: cmp      edi, 0x10
    //   0x6FAFF7E1: call     0x6fad4b20
    //   0x6FAFF7EA: cmp      ebx, 0x1c
    //   0x6FAFF7F6: ret      4
    return 1;
}

// 1.12a: 0x6FAFF800..0x6FAFF8F6 (__usercall, ret 0x18)
int32_t __usercall sub_6FAFF800(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    // Callees: D2Common.dll!#10191 -> PATH_GetXPosition, D2Common.dll!#10178 -> PATH_GetYPosition, 0x6fafe320, 0x6fad4b20
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFF81B: cmp      eax, 2
    //   0x6FAFF82C: cmp      eax, 3
    //   0x6FAFF831: cmp      eax, 5
    //   0x6FAFF83E: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FAFF85B: cmp      eax, 2
    //   0x6FAFF860: cmp      eax, 3
    //   0x6FAFF865: cmp      eax, 5
    //   0x6FAFF872: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FAFF8C0: call     0x6fafe320
    //   0x6FAFF8DD: call     0x6fad4b20
    //   0x6FAFF8E8: ret      0x18
    //   0x6FAFF8F3: ret      0x18
    return 1;
}

// 1.12a: 0x6FAFF900..0x6FAFFA9B (__stdcall, ret 0x18)
// 1.12a: 0x6FAFF950 (internal entry point sub_6FAFF950)
int32_t __stdcall sub_6FAFF900(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    // Callees: 0x6fafe320, D2Common.dll!#10073 -> SEED_InitLowSeed, D2Common.dll!#10178 -> PATH_GetYPosition, D2Common.dll!#10191 -> PATH_GetXPosition, D2Common.dll!#11031 -> PATH_ComputeSquaredDistance, 0x6fadb4b0
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFF91A: call     0x6fafe320
    //   0x6FAFF92A: ret      0x18
    //   0x6FAFF936: call     0x6fabc1c6 ; D2Common.dll!#10073 -> SEED_InitLowSeed
    //   0x6FAFF9E7: cmp      eax, 2
    //   0x6FAFF9EE: cmp      eax, 3
    //   0x6FAFF9F3: cmp      eax, 5
    //   0x6FAFFA00: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FAFFA22: cmp      eax, 2
    //   0x6FAFFA27: cmp      eax, 3
    //   0x6FAFFA2C: cmp      eax, 5
    //   0x6FAFFA39: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FAFFA56: call     0x6fabcc9a ; D2Common.dll!#11031 -> PATH_ComputeSquaredDistance
    //   0x6FAFFA5B: cmp      eax, 4
    //   0x6FAFFA7D: call     0x6fadb4b0
    //   0x6FAFFA98: ret      0x18
    return 1;
}

// 1.12a: 0x6FAFFAA0..0x6FAFFDD8 (__usercall, ret 0x18)
int32_t __usercall sub_6FAFFAA0(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    // Callees: D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState, D2Common.dll!#10235 -> STATLIST_AllocStatList, D2Common.dll!#10558 -> STATLIST_SetState, D2Common.dll!#10626 -> D2COMMON_10475_PostStatToStatList, D2Common.dll!#11139 -> STATLIST_SetStatIfListIsValid, 0x6fafe320, 0x6facf5b0, D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFFABB: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFFAC2: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FAFFAD4: imul     ebp, ebp, 0x23c
    //   0x6FAFFAF5: cmp      edx, dword ptr [eax + 0xc4]
    //   0x6FAFFB12: cmp      ecx, dword ptr [eax + 0xbd4]
    //   0x6FAFFB20: call     0x6fabc670 ; D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState
    //   0x6FAFFB3A: call     0x6fabc6be ; D2Common.dll!#10235 -> STATLIST_AllocStatList
    //   0x6FAFFB56: call     0x6fabc664 ; D2Common.dll!#10558 -> STATLIST_SetState
    //   0x6FAFFB5F: call     0x6fabc694 ; D2Common.dll!#10626 -> D2COMMON_10475_PostStatToStatList
    //   0x6FAFFB71: call     0x6fabc6d6 ; D2Common.dll!#11139 -> STATLIST_SetStatIfListIsValid
    //   0x6FAFFB82: call     0x6fafe320
    //   0x6FAFFB8B: call     0x6facf5b0
    //   0x6FAFFB9E: call     0x6fabcac0 ; D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    //   0x6FAFFBBB: call     0x6fabc2e6 ; D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos
    //   0x6FAFFBDE: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFFC1E: call     0x6fabc6d6 ; D2Common.dll!#11139 -> STATLIST_SetStatIfListIsValid
    //   0x6FAFFC31: call     dword ptr [eax*4 + 0x6fba3630]
    //   0x6FAFFC40: cmp      edi, eax
    //   0x6FAFFC52: call     0x6fabc6d6 ; D2Common.dll!#11139 -> STATLIST_SetStatIfListIsValid
    //   0x6FAFFC75: cmp      ecx, edx
    //   0x6FAFFC89: mov      eax, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFFCA1: call     0x6fafe320
    //   0x6FAFFCCA: call     0x6fadb4b0
    //   0x6FAFFCE1: call     0x6fafe710
    //   0x6FAFFD13: call     eax
    //   0x6FAFFD2F: call     0x6fae1fa0
    //   0x6FAFFD34: cmp      word ptr [ebp + 0x10a], 0
    //   0x6FAFFD3E: call     0x6facf5b0
    //   0x6FAFFD55: call     0x6fae1fa0
    //   0x6FAFFD5A: cmp      word ptr [ebp + 0x108], 0
    //   0x6FAFFD64: call     0x6facf5b0
    //   0x6FAFFD83: call     0x6fb430e0
    //   0x6FAFFD97: call     0x6fabcac0 ; D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    //   0x6FAFFDB3: call     0x6fabc2e6 ; D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos
    //   0x6FAFFDD5: ret      0x18
    return 1;
}

// 1.12a: 0x6FAFFDE0..0x6FB000A6 (__stdcall, ret 0x10)
int32_t __stdcall sub_6FAFFDE0(int32_t a1, int32_t a2, int32_t a3, int32_t a4)
{
    // Callees: 0x6fafe320, 0x6fab1590, 0x6fab15c0, 0x6fadb4b0, 0x6faff800, D2Common.dll!#10918 -> D2Common_10958, 0x6fb0e500, 0x6fae1fa0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FAFFDEB: cmp      ebp, edi
    //   0x6FAFFDF3: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FAFFDFA: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FAFFE0C: imul     ebp, ebp, 0x23c
    //   0x6FAFFE14: cmp      ebp, edi
    //   0x6FAFFE23: cmp      ax, di
    //   0x6FAFFE2F: cmp      ecx, dword ptr [0x6fba3838]
    //   0x6FAFFE3B: cmp      word ptr [ebp + 0xe8], di
    //   0x6FAFFE58: cmp      eax, edi
    //   0x6FAFFE77: call     0x6fafe320
    //   0x6FAFFE96: call     0x6fab1590
    //   0x6FAFFE9F: call     0x6fab15c0
    //   0x6FAFFEBA: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FAFFEEC: call     0x6fadb4b0
    //   0x6FAFFF08: call     0x6faff800
    //   0x6FAFFF3E: cmp      cx, 1
    //   0x6FAFFF4B: cmp      eax, dword ptr [0x6fba3838]
    //   0x6FAFFF69: call     eax
    //   0x6FAFFF87: call     0x6fabcb92 ; D2Common.dll!#10918 -> D2Common_10958
    //   0x6FAFFF92: call     0x6fb0e500
    //   0x6FAFFFAC: ret      0x10
    //   0x6FAFFFC5: call     0x6fae1fa0
    //   0x6FAFFFCA: cmp      word ptr [ebp + 0x10a], 0
    //   0x6FAFFFD4: call     0x6facf5b0
    //   0x6FAFFFEB: call     0x6fae1fa0
    //   0x6FAFFFF4: cmp      word ptr [ebp + 0x108], 0
    //   0x6FAFFFFE: call     0x6facf5b0
    //   0x6FB0001D: call     0x6fb430e0
    //   0x6FB00038: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB00041: cmp      dword ptr [0x6fbcc3d0], esi
    //   0x6FB00049: cmp      eax, 0xc
    //   0x6FB00053: cmp      byte ptr [ebp + 0x10], 0x12
    //   0x6FB0007C: ret      0x10
    //   0x6FB00099: ret      0x10
    //   0x6FB000A3: ret      0x10
    return 1;
}

// 1.12a: 0x6FB000B0..0x6FB00142 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB000B0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[2]
    // Callees: D2Common.dll!#10205 -> D2Common_10434, D2Common.dll!#10286 -> ITEMS_GetMissileType, D2Common.dll!#10681 -> ITEMS_CheckIfThrowable, 0x6fafe320, 0x6fadb4b0, 0x6fafe710
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB000C8: call     0x6fabccb8 ; D2Common.dll!#10205 -> D2Common_10434
    //   0x6FB000D9: ret      4
    //   0x6FB000DE: call     0x6fabc4cc ; D2Common.dll!#10286 -> ITEMS_GetMissileType
    //   0x6FB000E6: call     0x6fabc604 ; D2Common.dll!#10681 -> ITEMS_CheckIfThrowable
    //   0x6FB000EB: cmp      eax, 0x26
    //   0x6FB000F9: call     0x6fafe320
    //   0x6FB00114: call     0x6fadb4b0
    //   0x6FB00125: ret      4
    //   0x6FB0012E: call     0x6fafe710
    //   0x6FB0013F: ret      4
    return 1;
}

// 1.12a: 0x6FB00150..0x6FB00188 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB00150(void)
{
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill, D2Common.dll!#10904 -> SKILLS_GetSkillLevel, 0x6faffde0
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00152: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB0015E: ret
    //   0x6FB00165: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FB0016B: call     0x6fabc2ec ; D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill
    //   0x6FB00176: call     0x6fabc292 ; D2Common.dll!#10904 -> SKILLS_GetSkillLevel
    //   0x6FB00180: call     0x6faffde0
    //   0x6FB00187: ret
    return 1;
}

// 1.12a: 0x6FB00190..0x6FB00202 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB00190(D2UnitStrc* pUnit, int32_t a1)
{
    // Callees: 0x6faf1450
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB001F2: call     0x6faf1450
    //   0x6FB001FF: ret      4
    return 1;
}

// 1.12a: 0x6FB00210..0x6FB002A6 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB00210(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Referenced strings: 'StartsStartSkillID - Invalid Target', '..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    // Callees: D2Common.dll!#10498 -> UNITS_CanDualWield, 0x6fafd7b0, 0x6face550, D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit, 0x6facf5f0, 0x6fafee40, 0x6faf1450, D2Common.dll!#10950 -> LOG_11100
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00217: call     0x6fabc79c ; D2Common.dll!#10498 -> UNITS_CanDualWield
    //   0x6FB00224: cmp      dword ptr [ebx], 1
    //   0x6FB0022A: call     0x6fafd7b0
    //   0x6FB00244: call     0x6face550
    //   0x6FB0024F: call     0x6fabcac0 ; D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    //   0x6FB00258: call     0x6facf5f0
    //   0x6FB0026A: call     0x6fafee40
    //   0x6FB00271: ret
    //   0x6FB0027D: call     0x6faf1450
    //   0x6FB00282: push     0x6fb85a90 ; STR:'StartsStartSkillID - Invalid Target'
    //   0x6FB0028C: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FB00299: call     0x6fabcd18 ; D2Common.dll!#10950 -> LOG_11100
    //   0x6FB002A5: ret
    return 1;
}

// 1.12a: 0x6FB002B0..0x6FB00368 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB002B0(D2UnitStrc* pUnit, int32_t a1)
{
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10295 -> STATES_CheckState, D2Common.dll!#10619 -> SKILLS_GetFlags, D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill, 0x6fb373f0, 0x6fb74c30, 0x6fb73e70
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB002B5: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB002C7: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB002D1: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB002DF: push     0x6fb85ab4 ; STR:'..\\Source\\D2Client\\SKILLS\\Skills.cpp'
    //   0x6FB002E5: call     0x6fabc2ec ; D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill
    //   0x6FB002F0: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB002F7: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FB00307: imul     ecx, ecx, 0x23c
    //   0x6FB00318: cmp      eax, 0xb
    //   0x6FB0032B: call     0x6fb373f0
    //   0x6FB00332: ret      4
    //   0x6FB00337: call     0x6fb74c30
    //   0x6FB0033E: ret      4
    //   0x6FB00341: call     0x6fb73e70
    //   0x6FB00348: ret      4
    return 1;
}

// 1.12a: 0x6FB00370..0x6FB003AD (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB00370(void)
{
    // Callees: D2Common.dll!#10235 -> STATLIST_AllocStatList, D2Common.dll!#10193 -> STATLIST_SetStatRemoveCallback, D2Common.dll!#10626 -> D2COMMON_10475_PostStatToStatList
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0038C: call     0x6fabc6be ; D2Common.dll!#10235 -> STATLIST_AllocStatList
    //   0x6FB0039D: call     0x6fabc790 ; D2Common.dll!#10193 -> STATLIST_SetStatRemoveCallback
    //   0x6FB003A6: call     0x6fabc694 ; D2Common.dll!#10626 -> D2COMMON_10475_PostStatToStatList
    //   0x6FB003AC: ret
    return 1;
}

// 1.12a: 0x6FB003B0..0x6FB00406 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB003B0(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Callees: D2Common.dll!#10955 -> MISSILE_GetTotalFrames, D2Common.dll!#11076 -> MISSILE_SetTotalFrames, D2Common.dll!#10017 -> MISSILE_SetCurrentFrame, D2Common.dll!#10073 -> SEED_InitLowSeed, D2Common.dll!#11059 -> PATH_SetType, D2Common.dll!#10766 -> PATH_SetNewDistance, D2Common.dll!#10221 -> D2Common_10142
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB003BD: call     0x6fabc2f2 ; D2Common.dll!#10955 -> MISSILE_GetTotalFrames
    //   0x6FB003C4: cmp      ebx, 0x4e
    //   0x6FB003D0: call     0x6fabc322 ; D2Common.dll!#11076 -> MISSILE_SetTotalFrames
    //   0x6FB003D7: call     0x6fabc256 ; D2Common.dll!#10017 -> MISSILE_SetCurrentFrame
    //   0x6FB003E4: call     0x6fabc1c6 ; D2Common.dll!#10073 -> SEED_InitLowSeed
    //   0x6FB003EC: call     0x6fabc760 ; D2Common.dll!#11059 -> PATH_SetType
    //   0x6FB003F3: call     0x6fabc2c8 ; D2Common.dll!#10766 -> PATH_SetNewDistance
    //   0x6FB003FC: call     0x6fabc75a ; D2Common.dll!#10221 -> D2Common_10142
    //   0x6FB00405: ret
    return 1;
}

// 1.12a: 0x6FB00410..0x6FB0041C (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB00410(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1)
{
    // Callees: D2Common.dll!#10059 -> STATES_ToggleState
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00414: call     0x6fabc26e ; D2Common.dll!#10059 -> STATES_ToggleState
    //   0x6FB00419: ret      4
    return 1;
}

// 1.12a: 0x6FB00420..0x6FB00475 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB00420(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[7]
    // Callees: D2Common.dll!#10295 -> STATES_CheckState
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00425: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB0042C: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FB0043A: imul     edx, edx, 0x23c
    //   0x6FB0045B: cmp      edx, esi
    //   0x6FB00461: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB0046C: ret      4
    //   0x6FB00472: ret      4
    return 1;
}

// 1.12a: 0x6FB00480..0x6FB0067D (__fastcall, ret 0x0)
// 1.12a: 0x6FB005D0 (internal entry point sub_6FB005D0)
int32_t __fastcall sub_6FB00480(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Callees: D2Common.dll!#10955 -> MISSILE_GetTotalFrames, D2Common.dll!#11076 -> MISSILE_SetTotalFrames, D2Common.dll!#10017 -> MISSILE_SetCurrentFrame, D2Common.dll!#10073 -> SEED_InitLowSeed, D2Common.dll!#11059 -> PATH_SetType, D2Common.dll!#10766 -> PATH_SetNewDistance, D2Common.dll!#10584 -> PATH_GetPathPoints, D2Common.dll!#10191 -> PATH_GetXPosition
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00496: call     0x6fabc2f2 ; D2Common.dll!#10955 -> MISSILE_GetTotalFrames
    //   0x6FB0049D: cmp      ebp, 0x4e
    //   0x6FB004B1: call     0x6fabc322 ; D2Common.dll!#11076 -> MISSILE_SetTotalFrames
    //   0x6FB004B9: call     0x6fabc256 ; D2Common.dll!#10017 -> MISSILE_SetCurrentFrame
    //   0x6FB004D2: call     0x6fabc1c6 ; D2Common.dll!#10073 -> SEED_InitLowSeed
    //   0x6FB004DA: call     0x6fabc760 ; D2Common.dll!#11059 -> PATH_SetType
    //   0x6FB004E1: call     0x6fabc2c8 ; D2Common.dll!#10766 -> PATH_SetNewDistance
    //   0x6FB004EC: call     0x6fabcca6 ; D2Common.dll!#10584 -> PATH_GetPathPoints
    //   0x6FB004F3: cmp      eax, 2
    //   0x6FB004F8: cmp      eax, 3
    //   0x6FB004FD: cmp      eax, 5
    //   0x6FB0050A: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB00521: cmp      eax, 2
    //   0x6FB00526: cmp      eax, 3
    //   0x6FB0052B: cmp      eax, 5
    //   0x6FB00538: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB0054F: call     0x6fabc34c ; D2Common.dll!#10728 -> D2COMMON_10176_PathGetFirstPointY
    //   0x6FB00559: call     0x6fabc334 ; D2Common.dll!#10024 -> D2COMMON_10175_PathGetFirstPointX
    //   0x6FB00560: call     0x6fabc718 ; D2Common.dll!#10168 -> UNITS_GetDirectionToCoords
    //   0x6FB005D8: imul     ecx, ebp
    //   0x6FB005E7: call     0x6fabc778 ; D2Common.dll!#10465 -> D2Common_11053
    //   0x6FB0065E: cmp      edi, eax
    //   0x6FB00670: call     0x6fabccac ; D2Common.dll!#10093 -> PATH_SetNumberOfPathPoints
    //   0x6FB0067C: ret
    return 1;
}

// 1.12a: 0x6FB00680..0x6FB006D4 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB00680(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[10]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00685: call     0x6faf0d20
    //   0x6FB00696: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB006A1: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB006AB: call     0x6fab326d
    //   0x6FB006B1: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB006BC: cmp      eax, 1
    //   0x6FB006C8: ret      4
    //   0x6FB006D1: ret      4
    return 1;
}

// 1.12a: 0x6FB006E0..0x6FB0082A (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB006E0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[9]
    // Callees: 0x6fafd990, D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, D2Common.dll!#10775 -> STATLIST_GetStatValue
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB006E5: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB006EC: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FB006FA: imul     edx, edx, 0x23c
    //   0x6FB0070B: call     0x6fafd990
    //   0x6FB00716: ret      4
    //   0x6FB00722: call     0x6fafd990
    //   0x6FB0072D: ret      4
    //   0x6FB0073A: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB00741: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FB00751: imul     esi, esi, 0x23c
    //   0x6FB0075F: ret      4
    //   0x6FB00765: mov      edx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB00787: cmp      edx, dword ptr [eax + 0xc4]
    //   0x6FB0079F: cmp      ecx, dword ptr [eax + 0xbd4]
    //   0x6FB007A9: call     0x6fabc670 ; D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState
    //   0x6FB007B9: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB007C0: ret      4
    //   0x6FB007CB: call     0x6fabc6e8 ; D2Common.dll!#10775 -> STATLIST_GetStatValue
    //   0x6FB007D0: cmp      eax, 1
    //   0x6FB007E2: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB007E9: ret      4
    //   0x6FB007EC: cmp      eax, 3
    //   0x6FB007FE: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB00805: ret      4
    //   0x6FB0080F: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB00816: ret      4
    //   0x6FB00820: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB00827: ret      4
    return 1;
}

// 1.12a: 0x6FB00830..0x6FB008EF (__usercall, ret 0x4)
int32_t __usercall sub_6FB00830(D2UnitStrc* pUnit /*eax*/, int32_t a1)
{
    // Callees: D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState, D2Common.dll!#10775 -> STATLIST_GetStatValue
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00835: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB0083D: cmp      eax, dword ptr [ecx + 0xba0]
    //   0x6FB0084B: imul     eax, eax, 0x23c
    //   0x6FB0085B: ret      4
    //   0x6FB0085E: mov      eax, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB0087B: cmp      edx, dword ptr [ecx + 0xc4]
    //   0x6FB00895: cmp      eax, ebx
    //   0x6FB0089F: call     0x6fabc670 ; D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState
    //   0x6FB008B0: call     0x6fabc6e8 ; D2Common.dll!#10775 -> STATLIST_GetStatValue
    //   0x6FB008B5: cmp      eax, 1
    //   0x6FB008C9: ret      4
    //   0x6FB008CC: cmp      eax, 3
    //   0x6FB008E0: ret      4
    //   0x6FB008EC: ret      4
    return 1;
}

// 1.12a: 0x6FB008F0..0x6FB00961 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB008F0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[4]
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10965 -> SKILLS_SetParam1
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    SKILLS_SetParam1(pSkill, 0);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB008F5: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB00902: call     0x6faf0d20
    //   0x6FB00913: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB0091E: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB00928: call     0x6fab326d
    //   0x6FB0092E: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB0093B: ret      4
    //   0x6FB0093F: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB0094B: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB0095E: ret      4
    return 1;
}

// 1.12a: 0x6FB00970..0x6FB00A36 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB00970(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[6]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10965 -> SKILLS_SetParam1, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    SKILLS_SetParam1(pSkill, 0);
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0097C: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB00983: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB00993: imul     esi, esi, 0x23c
    //   0x6FB009A3: ret      4
    //   0x6FB009A8: call     0x6faf0d20
    //   0x6FB009B9: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB009C4: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB009CE: call     0x6fab326d
    //   0x6FB009D5: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB009DF: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB009EF: ret      4
    //   0x6FB009FD: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB00A0C: ret      4
    //   0x6FB00A1D: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB00A24: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB00A33: ret      4
    return 1;
}

// 1.12a: 0x6FB00A40..0x6FB00AC2 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB00A40(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[14]
    // Callees: 0x6fb00830, 0x6fafe320, 0x6fafe690
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00A4D: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB00A54: cmp      esi, dword ptr [ecx + 0xba0]
    //   0x6FB00A64: imul     eax, eax, 0x23c
    //   0x6FB00A75: ret      4
    //   0x6FB00A7C: call     0x6fb00830
    //   0x6FB00A8F: ret      4
    //   0x6FB00A9C: call     0x6fafe320
    //   0x6FB00AAF: call     0x6fafe690
    //   0x6FB00ABF: ret      4
    return 1;
}

// 1.12a: 0x6FB00AD0..0x6FB00B84 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB00AD0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[10]
    // Callees: 0x6fb00830, 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10294 -> DATATBLS_GetMissileVelocityFromMissilesTxt, 0x6fafe4b0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00ADA: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB00AE1: cmp      esi, dword ptr [ecx + 0xba0]
    //   0x6FB00AF1: imul     eax, eax, 0x23c
    //   0x6FB00AFF: ret      4
    //   0x6FB00B06: call     0x6fb00830
    //   0x6FB00B16: ret      4
    //   0x6FB00B1B: call     0x6faf0d20
    //   0x6FB00B2C: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB00B37: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB00B41: call     0x6fab326d
    //   0x6FB00B48: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB00B57: ret      4
    //   0x6FB00B60: call     0x6fabc364 ; D2Common.dll!#10294 -> DATATBLS_GetMissileVelocityFromMissilesTxt
    //   0x6FB00B73: call     0x6fafe4b0
    //   0x6FB00B81: ret      4
    return 1;
}

// 1.12a: 0x6FB00B90..0x6FB00C24 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB00B90(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[7]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, 0x6fb00830, 0x6fafe690
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00B98: call     0x6faf0d20
    //   0x6FB00BA9: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB00BB4: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB00BBE: call     0x6fab326d
    //   0x6FB00BC4: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB00BD1: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB00BD8: cmp      edi, dword ptr [ecx + 0xba0]
    //   0x6FB00BE8: imul     eax, eax, 0x23c
    //   0x6FB00BF5: call     0x6fb00830
    //   0x6FB00C02: ret      4
    //   0x6FB00C0E: call     0x6fafe690
    //   0x6FB00C21: ret      4
    return 1;
}

// 1.12a: 0x6FB00C30..0x6FB00CB2 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB00C30(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[5]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10366 -> UNITS_GetRoom, D2Common.dll!#10438 -> DUNGEON_IsRoomInTown, 0x6facf980
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00C35: call     0x6faf0d20
    //   0x6FB00C46: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB00C51: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB00C5B: call     0x6fab326d
    //   0x6FB00C62: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB00C6E: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB00C74: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB00C7E: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB00C84: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB00C93: cmp      eax, 1
    //   0x6FB00C9B: call     0x6facf980
    //   0x6FB00CA8: ret      4
    //   0x6FB00CAF: ret      4
    return 1;
}

// 1.12a: 0x6FB00CC0..0x6FB00CE9 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB00CC0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[3]
    // Callees: 0x6fb00c30
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00CC8: call     0x6fb00c30
    //   0x6FB00CD2: ret      4
    //   0x6FB00CE6: ret      4
    return 1;
}

// 1.12a: 0x6FB00CF0..0x6FB00DD7 (__fastcall, ret 0x4)
// 1.12a: 0x6FB00D20 (internal entry point sub_6FB00D20)
int32_t __fastcall sub_6FB00CF0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[13]
    // Callees: D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState, D2Common.dll!#10775 -> STATLIST_GetStatValue, 0x6fb430e0, 0x6fae1fa0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00CF8: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB00CFF: cmp      edx, dword ptr [ecx + 0xba0]
    //   0x6FB00D0D: imul     edx, edx, 0x23c
    //   0x6FB00D1D: ret      4
    //   0x6FB00D20: mov      eax, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB00D3D: cmp      edx, dword ptr [ecx + 0xc4]
    //   0x6FB00D57: cmp      eax, ebx
    //   0x6FB00D5D: call     0x6fabc670 ; D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState
    //   0x6FB00D6E: call     0x6fabc6e8 ; D2Common.dll!#10775 -> STATLIST_GetStatValue
    //   0x6FB00D73: cmp      eax, 1
    //   0x6FB00D7F: cmp      eax, 3
    //   0x6FB00DBB: call     0x6fb430e0
    //   0x6FB00DC7: call     0x6fae1fa0
    //   0x6FB00DD4: ret      4
    return 1;
}

// 1.12a: 0x6FB00DE0..0x6FB00F11 (__fastcall, ret 0x4)
// 1.12a: 0x6FB00E10 (internal entry point sub_6FB00E10)
// 1.12a: 0x6FB00E50 (internal entry point sub_6FB00E50)
int32_t __fastcall sub_6FB00DE0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[12]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState, D2Common.dll!#10775 -> STATLIST_GetStatValue, 0x6fb430e0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00DE8: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB00DEF: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FB00DFD: imul     edx, edx, 0x23c
    //   0x6FB00E0D: ret      4
    //   0x6FB00E12: call     0x6faf0d20
    //   0x6FB00E23: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB00E2E: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB00E38: call     0x6fab326d
    //   0x6FB00E3F: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB00E4D: ret      4
    //   0x6FB00E50: mov      edx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB00E6B: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB00E76: cmp      ecx, dword ptr [edx + 0xc4]
    //   0x6FB00E90: cmp      eax, ebx
    //   0x6FB00E96: call     0x6fabc670 ; D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState
    //   0x6FB00EA7: call     0x6fabc6e8 ; D2Common.dll!#10775 -> STATLIST_GetStatValue
    //   0x6FB00EAC: cmp      eax, 1
    //   0x6FB00EB8: cmp      eax, 3
    //   0x6FB00EF4: call     0x6fb430e0
    //   0x6FB00F00: call     0x6fae1fa0
    //   0x6FB00F0E: ret      4
    return 1;
}

// 1.12a: 0x6FB00F20..0x6FB01056 (__stdcall, ret 0x4)
int32_t __stdcall sub_6FB00F20(int32_t a1)
{
    // Callees: D2Common.dll!#10295 -> STATES_CheckState, 0x6fb430e0, 0x6fae1fa0, D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState, D2Common.dll!#10235 -> STATLIST_AllocStatList, D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId, D2Common.dll!#10965 -> SKILLS_SetParam1, D2Common.dll!#10626 -> D2COMMON_10475_PostStatToStatList
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    SKILLS_SetParam1(pSkill, 0);
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB00F2A: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB00F31: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB00F41: imul     edi, edi, 0x23c
    //   0x6FB00F4F: ret      4
    //   0x6FB00F56: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB00F6B: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB00F7C: cmp      eax, ecx
    //   0x6FB00F8D: call     0x6fb430e0
    //   0x6FB00FA9: call     0x6fae1fa0
    //   0x6FB00FB1: call     0x6fabc670 ; D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState
    //   0x6FB00FD7: call     0x6fabc6be ; D2Common.dll!#10235 -> STATLIST_AllocStatList
    //   0x6FB00FE7: ret      4
    //   0x6FB00FEE: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB00FFA: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB01003: call     0x6fabc694 ; D2Common.dll!#10626 -> D2COMMON_10475_PostStatToStatList
    //   0x6FB0100E: call     0x6fabc790 ; D2Common.dll!#10193 -> STATLIST_SetStatRemoveCallback
    //   0x6FB01016: call     0x6fabc664 ; D2Common.dll!#10558 -> STATLIST_SetState
    //   0x6FB01020: call     0x6fabc26e ; D2Common.dll!#10059 -> STATES_ToggleState
    //   0x6FB0102D: ret      4
    //   0x6FB01037: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB01046: call     0x6fabcca0 ; D2Common.dll!#10088 -> STATLIST_SetExpireFrame
    //   0x6FB01053: ret      4
    return 1;
}

// 1.12a: 0x6FB01060..0x6FB0106D (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01060(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[8]
    // Callees: 0x6fb00f20
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01064: call     0x6fb00f20
    //   0x6FB0106A: ret      4
    return 1;
}

// 1.12a: 0x6FB01070..0x6FB01179 (__usercall, ret 0x18)
int32_t __usercall sub_6FB01070(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    // Callees: D2Common.dll!#10073 -> SEED_InitLowSeed, 0x6fad4b20, D2Common.dll!#10120 -> SEED_GetLowSeed, D2Common.dll!#11161 -> MISSILE_SetTargetX, D2Common.dll!#10792 -> MISSILE_SetTargetY
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01082: call     0x6fabc1c6 ; D2Common.dll!#10073 -> SEED_InitLowSeed
    //   0x6FB01139: call     0x6fad4b20
    //   0x6FB01148: call     0x6fabccb2 ; D2Common.dll!#10120 -> SEED_GetLowSeed
    //   0x6FB0114F: call     0x6fabc304 ; D2Common.dll!#11161 -> MISSILE_SetTargetX
    //   0x6FB01161: call     0x6fabc2b6 ; D2Common.dll!#10792 -> MISSILE_SetTargetY
    //   0x6FB01176: ret      0x18
    return 1;
}

// 1.12a: 0x6FB01180..0x6FB01279 (__stdcall, ret 0x18)
int32_t __stdcall sub_6FB01180(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    // Callees: D2Common.dll!#10073 -> SEED_InitLowSeed, 0x6fad4b20, D2Common.dll!#11161 -> MISSILE_SetTargetX
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0118A: call     0x6fabc1c6 ; D2Common.dll!#10073 -> SEED_InitLowSeed
    //   0x6FB01229: call     0x6fad4b20
    //   0x6FB01232: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB0123A: cmp      ebx, dword ptr [edx + 0xba0]
    //   0x6FB0124A: imul     ecx, ecx, 0x23c
    //   0x6FB01261: call     0x6fabc304 ; D2Common.dll!#11161 -> MISSILE_SetTargetX
    //   0x6FB01268: cmp      esi, 0x100
    //   0x6FB01276: ret      0x18
    return 1;
}

// 1.12a: 0x6FB01280..0x6FB01379 (__stdcall, ret 0x18)
int32_t __stdcall sub_6FB01280(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    // Callees: D2Common.dll!#10073 -> SEED_InitLowSeed, 0x6fad4b20, D2Common.dll!#11161 -> MISSILE_SetTargetX
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0128A: call     0x6fabc1c6 ; D2Common.dll!#10073 -> SEED_InitLowSeed
    //   0x6FB01329: call     0x6fad4b20
    //   0x6FB01332: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB0133A: cmp      ebx, dword ptr [edx + 0xba0]
    //   0x6FB0134A: imul     ecx, ecx, 0x23c
    //   0x6FB01361: call     0x6fabc304 ; D2Common.dll!#11161 -> MISSILE_SetTargetX
    //   0x6FB01368: cmp      esi, 0x100
    //   0x6FB01376: ret      0x18
    return 1;
}

// 1.12a: 0x6FB01380..0x6FB01506 (__usercall, ret 0x18)
int32_t __usercall sub_6FB01380(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    // Callees: D2Common.dll!#10073 -> SEED_InitLowSeed, D2Common.dll!#10366 -> UNITS_GetRoom, 0x6fab1a00, 0x6fad4b20
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0138D: call     0x6fabc1c6 ; D2Common.dll!#10073 -> SEED_InitLowSeed
    //   0x6FB0139F: imul     eax, ebx
    //   0x6FB013DD: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB0148E: imul     edi, dword ptr [esp + 0x8c]
    //   0x6FB0149C: imul     edx, eax
    //   0x6FB014A1: cmp      edx, dword ptr [esp + 0x10]
    //   0x6FB014CE: call     0x6fab1a00
    //   0x6FB014DB: call     0x6fad4b20
    //   0x6FB01503: ret      0x18
    return 1;
}

// 1.12a: 0x6FB01510..0x6FB0161A (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01510(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[8]
    // Callees: 0x6fafe320, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fad4b20, D2Common.dll!#10247 -> MISSILE_SetStreamMissile, D2Common.dll!#10648 -> MISSILE_SetStreamRange
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0151F: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01526: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB01536: imul     esi, esi, 0x23c
    //   0x6FB01547: ret      4
    //   0x6FB01557: call     0x6fafe320
    //   0x6FB0156F: cmp      word ptr [esi + 0xec], 0
    //   0x6FB01582: ret      4
    //   0x6FB01597: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB015E4: call     0x6fad4b20
    //   0x6FB015F8: call     0x6fabc706 ; D2Common.dll!#10247 -> MISSILE_SetStreamMissile
    //   0x6FB01606: call     0x6fabc712 ; D2Common.dll!#10648 -> MISSILE_SetStreamRange
    //   0x6FB01617: ret      4
    return 1;
}

// 1.12a: 0x6FB01620..0x6FB0176B (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01620(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[6]
    // Callees: 0x6fb00830, D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId, 0x6fb00730, 0x6fafe320, D2Common.dll!#10063 -> SKILLS_GetParam1, 0x6fad4b20, 0x6fb755b0, D2Common.dll!#10965 -> SKILLS_SetParam1
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    SKILLS_SetParam1(pSkill, 0);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0162E: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01635: cmp      esi, dword ptr [eax + 0xba0]
    //   0x6FB01645: imul     ebp, ebp, 0x23c
    //   0x6FB01652: call     0x6fb00830
    //   0x6FB01663: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB01678: ret      4
    //   0x6FB01686: call     0x6fb00730
    //   0x6FB0169C: ret      4
    //   0x6FB016B3: call     0x6fafe320
    //   0x6FB016C1: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB01712: call     0x6fad4b20
    //   0x6FB01724: call     0x6fb755b0
    //   0x6FB01738: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB01740: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB01750: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB01768: ret      4
    return 1;
}

// 1.12a: 0x6FB01770..0x6FB01815 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01770(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[15]
    // Callees: 0x6fb00830, 0x6fafe320, 0x6fb00730, 0x6fb01070
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0177D: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01784: cmp      esi, dword ptr [ecx + 0xba0]
    //   0x6FB01794: imul     eax, eax, 0x23c
    //   0x6FB017A1: call     0x6fb00830
    //   0x6FB017B5: ret      4
    //   0x6FB017C2: call     0x6fafe320
    //   0x6FB017D1: ret      4
    //   0x6FB017DC: call     0x6fb00730
    //   0x6FB017EC: ret      4
    //   0x6FB01801: call     0x6fb01070
    //   0x6FB01812: ret      4
    return 1;
}

// 1.12a: 0x6FB01820..0x6FB0190B (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01820(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[83]
    // Callees: 0x6fb00830, 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, 0x6fb00730, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0182E: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01835: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FB01845: imul     ebp, ebp, 0x23c
    //   0x6FB01852: call     0x6fb00830
    //   0x6FB01861: call     0x6faf0d20
    //   0x6FB01872: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB0187D: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB01887: call     0x6fab326d
    //   0x6FB0188D: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB018A2: ret      4
    //   0x6FB018AB: call     0x6fb00730
    //   0x6FB018C1: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB018CF: call     0x6fab1590
    //   0x6FB018DC: call     0x6fab15c0
    //   0x6FB018F7: call     0x6fb01180
    //   0x6FB01908: ret      4
    return 1;
}

// 1.12a: 0x6FB01910..0x6FB019FB (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01910(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[11]
    // Callees: 0x6fb00830, 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, 0x6fb00730, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0191E: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01925: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FB01935: imul     ebp, ebp, 0x23c
    //   0x6FB01942: call     0x6fb00830
    //   0x6FB01951: call     0x6faf0d20
    //   0x6FB01962: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB0196D: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB01977: call     0x6fab326d
    //   0x6FB0197D: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB01992: ret      4
    //   0x6FB0199B: call     0x6fb00730
    //   0x6FB019B1: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB019BF: call     0x6fab1590
    //   0x6FB019CC: call     0x6fab15c0
    //   0x6FB019E7: call     0x6fb01280
    //   0x6FB019F8: ret      4
    return 1;
}

// 1.12a: 0x6FB01A00..0x6FB01B0C (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01A00(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[9]
    // Callees: 0x6fb00830, 0x6fb00730, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01A0E: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01A15: cmp      esi, dword ptr [eax + 0xba0]
    //   0x6FB01A25: imul     ebp, ebp, 0x23c
    //   0x6FB01A32: call     0x6fb00830
    //   0x6FB01A47: ret      4
    //   0x6FB01A52: call     0x6fb00730
    //   0x6FB01A6A: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB01A75: call     0x6faf0d20
    //   0x6FB01A86: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB01A91: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB01A9B: call     0x6fab326d
    //   0x6FB01AA1: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB01AAE: call     0x6fab1590
    //   0x6FB01AB7: call     0x6fab15c0
    //   0x6FB01AC7: call     0x6fafe320
    //   0x6FB01AD7: ret      4
    //   0x6FB01AF8: call     0x6fb01380
    //   0x6FB01B09: ret      4
    return 1;
}

// 1.12a: 0x6FB01B10..0x6FB01BAA (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01B10(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[5]
    // Callees: 0x6fb00830, 0x6fb00730, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6faff900
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01B1C: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01B23: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FB01B33: imul     esi, esi, 0x23c
    //   0x6FB01B4B: call     0x6fb00830
    //   0x6FB01B5E: ret      4
    //   0x6FB01B67: call     0x6fb00730
    //   0x6FB01B7B: ret      4
    //   0x6FB01B85: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB01B98: call     0x6faff900
    //   0x6FB01BA7: ret      4
    return 1;
}

// 1.12a: 0x6FB01BB0..0x6FB01BBC (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01BB0(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1)
{
    // Callees: D2Common.dll!#10059 -> STATES_ToggleState
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01BB4: call     0x6fabc26e ; D2Common.dll!#10059 -> STATES_ToggleState
    //   0x6FB01BB9: ret      4
    return 1;
}

// 1.12a: 0x6FB01BC0..0x6FB01C1E (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB01BC0(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Callees: D2Common.dll!#10955 -> MISSILE_GetTotalFrames, D2Common.dll!#11076 -> MISSILE_SetTotalFrames, D2Common.dll!#10017 -> MISSILE_SetCurrentFrame, D2Common.dll!#10024 -> D2COMMON_10175_PathGetFirstPointX, D2Common.dll!#10073 -> SEED_InitLowSeed, D2Common.dll!#11059 -> PATH_SetType, D2Common.dll!#10766 -> PATH_SetNewDistance, D2Common.dll!#10221 -> D2Common_10142
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01BCD: call     0x6fabc2f2 ; D2Common.dll!#10955 -> MISSILE_GetTotalFrames
    //   0x6FB01BD4: cmp      ebx, 0x4e
    //   0x6FB01BE0: call     0x6fabc322 ; D2Common.dll!#11076 -> MISSILE_SetTotalFrames
    //   0x6FB01BE7: call     0x6fabc256 ; D2Common.dll!#10017 -> MISSILE_SetCurrentFrame
    //   0x6FB01BF0: call     0x6fabc334 ; D2Common.dll!#10024 -> D2COMMON_10175_PathGetFirstPointX
    //   0x6FB01BFC: call     0x6fabc1c6 ; D2Common.dll!#10073 -> SEED_InitLowSeed
    //   0x6FB01C04: call     0x6fabc760 ; D2Common.dll!#11059 -> PATH_SetType
    //   0x6FB01C0B: call     0x6fabc2c8 ; D2Common.dll!#10766 -> PATH_SetNewDistance
    //   0x6FB01C14: call     0x6fabc75a ; D2Common.dll!#10221 -> D2Common_10142
    //   0x6FB01C1D: ret
    return 1;
}

// 1.12a: 0x6FB01C20..0x6FB01C6B (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB01C20(void)
{
    // Callees: D2Common.dll!#10299 -> UNITS_SetAnimationSpeed
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01C24: cmp      dword ptr [esi], 3
    //   0x6FB01C34: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01C3C: cmp      eax, dword ptr [ecx + 0xb6c]
    //   0x6FB01C4A: imul     eax, eax, 0x1a4
    //   0x6FB01C65: call     0x6fabc268 ; D2Common.dll!#10299 -> UNITS_SetAnimationSpeed
    //   0x6FB01C6A: ret
    return 1;
}

// 1.12a: 0x6FB01C70..0x6FB01D0D (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01C70(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[17]
    // Callees: D2Common.dll!#10366 -> UNITS_GetRoom, 0x6fafe320, 0x6fab1a00, D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01C7B: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB01C8C: ret      4
    //   0x6FB01C99: call     0x6fafe320
    //   0x6FB01CAB: ret      4
    //   0x6FB01CB8: call     0x6fab1a00
    //   0x6FB01CC5: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01CCD: cmp      esi, dword ptr [ecx + 0xba0]
    //   0x6FB01CDB: imul     esi, esi, 0x23c
    //   0x6FB01CE7: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB01CF5: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB01D0A: ret      4
    return 1;
}

// 1.12a: 0x6FB01D10..0x6FB01D82 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01D10(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[29]
    // Callees: 0x6fb00830, 0x6fafe710
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01D1A: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01D21: cmp      esi, dword ptr [ecx + 0xba0]
    //   0x6FB01D31: imul     eax, eax, 0x23c
    //   0x6FB01D3E: call     0x6fb00830
    //   0x6FB01D47: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01D4F: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB01D6B: call     0x6fafe710
    //   0x6FB01D78: ret      4
    //   0x6FB01D7F: ret      4
    return 1;
}

// 1.12a: 0x6FB01D90..0x6FB01E1B (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01D90(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[28]
    // Callees: 0x6fb00830, 0x6fafe3f0, 0x6fafe690
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01D9A: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01DA1: cmp      esi, dword ptr [ecx + 0xba0]
    //   0x6FB01DB1: imul     eax, eax, 0x23c
    //   0x6FB01DBF: ret      4
    //   0x6FB01DC6: call     0x6fb00830
    //   0x6FB01DD1: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01DD9: cmp      edi, dword ptr [edx + 0xb6c]
    //   0x6FB01DF0: call     0x6fafe3f0
    //   0x6FB01E02: call     0x6fafe690
    //   0x6FB01E10: ret      4
    //   0x6FB01E18: ret      4
    return 1;
}

// 1.12a: 0x6FB01E20..0x6FB01EC1 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01E20(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[27]
    // Callees: 0x6fb00830, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fafe710, D2Common.dll!#11161 -> MISSILE_SetTargetX
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01E30: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01E37: cmp      esi, dword ptr [eax + 0xba0]
    //   0x6FB01E47: imul     edi, edi, 0x23c
    //   0x6FB01E54: call     0x6fb00830
    //   0x6FB01E61: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01E69: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB01E8B: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB01E9B: call     0x6fafe710
    //   0x6FB01EA7: call     0x6fabc304 ; D2Common.dll!#11161 -> MISSILE_SetTargetX
    //   0x6FB01EB5: ret      4
    //   0x6FB01EBE: ret      4
    return 1;
}

// 1.12a: 0x6FB01ED0..0x6FB01F82 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01ED0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[25]
    // Callees: 0x6fb00830, D2Common.dll!#10294 -> DATATBLS_GetMissileVelocityFromMissilesTxt, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fafe4b0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01EDC: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01EE3: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FB01EF3: imul     esi, esi, 0x23c
    //   0x6FB01F03: ret      4
    //   0x6FB01F0A: call     0x6fb00830
    //   0x6FB01F15: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01F1D: cmp      ebx, dword ptr [edx + 0xb6c]
    //   0x6FB01F36: call     0x6fabc364 ; D2Common.dll!#10294 -> DATATBLS_GetMissileVelocityFromMissilesTxt
    //   0x6FB01F4D: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB01F66: call     0x6fafe4b0
    //   0x6FB01F75: ret      4
    //   0x6FB01F7F: ret      4
    return 1;
}

// 1.12a: 0x6FB01F90..0x6FB02148 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB01F90(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[16]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, D2Common.dll!#10178 -> PATH_GetYPosition, D2Common.dll!#10191 -> PATH_GetXPosition
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB01F9C: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB01FA3: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FB01FB3: imul     esi, esi, 0x23c
    //   0x6FB01FC3: ret      4
    //   0x6FB01FC8: call     0x6faf0d20
    //   0x6FB01FD9: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB01FE4: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB01FEE: call     0x6fab326d
    //   0x6FB01FF5: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB0200F: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB02016: imul     esi, eax
    //   0x6FB0201B: cmp      eax, 2
    //   0x6FB02020: cmp      eax, 3
    //   0x6FB02025: cmp      eax, 5
    //   0x6FB02032: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB02053: cmp      eax, 2
    //   0x6FB02058: cmp      eax, 3
    //   0x6FB0205D: cmp      eax, 5
    //   0x6FB0206A: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB02088: cmp      eax, 2
    //   0x6FB0208D: cmp      eax, 3
    //   0x6FB02092: cmp      eax, 5
    //   0x6FB0209F: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB020B5: cmp      eax, 2
    //   0x6FB020BA: cmp      eax, 3
    //   0x6FB020BF: cmp      eax, 5
    //   0x6FB020CC: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB020E9: call     0x6fabcc9a ; D2Common.dll!#11031 -> PATH_ComputeSquaredDistance
    //   0x6FB020EE: cmp      eax, esi
    //   0x6FB020F8: cmp      eax, 1
    //   0x6FB020FE: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB02108: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB02112: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB0211C: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB02128: call     0x6facf980
    //   0x6FB02138: ret      4
    //   0x6FB02145: ret      4
    return 1;
}

// 1.12a: 0x6FB02150..0x6FB0226E (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB02150(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[15]
    // Callees: D2Common.dll!#10295 -> STATES_CheckState, 0x6fb430e0, 0x6fae1fa0, D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState, D2Common.dll!#10235 -> STATLIST_AllocStatList, D2Common.dll!#10626 -> D2COMMON_10475_PostStatToStatList, D2Common.dll!#10193 -> STATLIST_SetStatRemoveCallback, D2Common.dll!#10558 -> STATLIST_SetState
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0215C: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB02163: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FB02175: imul     edx, edx, 0x23c
    //   0x6FB02188: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB0219D: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB021AE: cmp      eax, ecx
    //   0x6FB021BF: call     0x6fb430e0
    //   0x6FB021DC: call     0x6fae1fa0
    //   0x6FB021E5: call     0x6fabc670 ; D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState
    //   0x6FB0220B: call     0x6fabc6be ; D2Common.dll!#10235 -> STATLIST_AllocStatList
    //   0x6FB0221A: ret      4
    //   0x6FB02221: call     0x6fabc694 ; D2Common.dll!#10626 -> D2COMMON_10475_PostStatToStatList
    //   0x6FB0222C: call     0x6fabc790 ; D2Common.dll!#10193 -> STATLIST_SetStatRemoveCallback
    //   0x6FB02234: call     0x6fabc664 ; D2Common.dll!#10558 -> STATLIST_SetState
    //   0x6FB0223E: call     0x6fabc26e ; D2Common.dll!#10059 -> STATES_ToggleState
    //   0x6FB0224A: ret      4
    //   0x6FB02250: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB0225F: call     0x6fabcca0 ; D2Common.dll!#10088 -> STATLIST_SetExpireFrame
    //   0x6FB0226B: ret      4
    return 1;
}

// 1.12a: 0x6FB02270..0x6FB023A5 (__fastcall, ret 0x10)
int32_t __fastcall sub_6FB02270(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1, int32_t a2, int32_t a3, int32_t a4)
{
    // Callees: 0x6facf710, 0x6facf980, 0x6fab1480, 0x6fab1500, 0x6fab1590, 0x6fab15c0, 0x6fad4b20, 0x6fb430e0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB02280: ret      0x10
    //   0x6FB0229B: call     0x6facf710
    //   0x6FB022AE: ret      0x10
    //   0x6FB022B4: call     0x6facf980
    //   0x6FB022C0: call     0x6fab1480
    //   0x6FB022C5: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB022D8: call     0x6fab1500
    //   0x6FB022EC: ret      0x10
    //   0x6FB022FA: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB02301: cmp      edx, dword ptr [ecx + 0xb6c]
    //   0x6FB0233C: call     0x6fab1590
    //   0x6FB02347: call     0x6fab15c0
    //   0x6FB02354: call     0x6fad4b20
    //   0x6FB02365: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB02376: cmp      eax, edx
    //   0x6FB02387: call     0x6fb430e0
    //   0x6FB02398: ret      0x10
    //   0x6FB023A2: ret      0x10
    return 1;
}

// 1.12a: 0x6FB023B0..0x6FB024DF (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB023B0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[84]
    // Callees: 0x6facf5b0, 0x6fab1590, 0x6fab15c0, 0x6fad4b20, 0x6fb755b0, 0x6fac3e60, 0x6fb430e0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB023C3: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB023CA: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FB023DE: imul     ebx, ebx, 0x23c
    //   0x6FB023FF: cmp      ecx, dword ptr [eax + 0xb6c]
    //   0x6FB0240C: call     0x6facf5b0
    //   0x6FB0243F: call     0x6fab1590
    //   0x6FB0244A: call     0x6fab15c0
    //   0x6FB0246F: call     0x6fad4b20
    //   0x6FB02478: cmp      esi, edi
    //   0x6FB02484: call     0x6fb755b0
    //   0x6FB0248C: cmp      esi, edi
    //   0x6FB02493: cmp      eax, edi
    //   0x6FB024A5: call     0x6fac3e60
    //   0x6FB024B1: cmp      bx, di
    //   0x6FB024C1: call     0x6fb430e0
    //   0x6FB024D2: ret      4
    //   0x6FB024DC: ret      4
    return 1;
}

// 1.12a: 0x6FB024E0..0x6FB02669 (__cdecl, ret 0x0)
// 1.12a: 0x6FB02640 (internal entry point sub_6FB02640)
int32_t __cdecl sub_6FB024E0(void)
{
    // Callees: D2Common.dll!#10429 -> D2Common_10234, D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState, D2Common.dll!#10775 -> STATLIST_GetStatValue, 0x6fab1500, D2Common.dll!#10366 -> UNITS_GetRoom, D2Common.dll!#10438 -> DUNGEON_IsRoomInTown, 0x6fab1590, 0x6fab15c0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB024EF: call     0x6fabc754 ; D2Common.dll!#10429 -> D2Common_10234
    //   0x6FB02500: call     0x6fabc670 ; D2Common.dll!#10539 -> STATLIST_GetStatListFromUnitAndState
    //   0x6FB02518: call     0x6fabc6e8 ; D2Common.dll!#10775 -> STATLIST_GetStatValue
    //   0x6FB02527: call     0x6fabc6e8 ; D2Common.dll!#10775 -> STATLIST_GetStatValue
    //   0x6FB02538: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB02541: cmp      ebp, dword ptr [edi + 0xba0]
    //   0x6FB0255B: call     0x6fab1500
    //   0x6FB02579: cmp      eax, dword ptr [edi + 0xb6c]
    //   0x6FB02586: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB0258C: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB0259B: call     0x6fab1590
    //   0x6FB025C1: call     0x6fab15c0
    //   0x6FB025C6: cmp      word ptr [esi + 0xec], 0
    //   0x6FB025EB: cmp      word ptr [esi + 0xee], 0
    //   0x6FB025FD: call     0x6fab1020
    //   0x6FB02619: call     0x6fad4b20
    //   0x6FB02624: ret
    //   0x6FB02634: call     0x6fad4b20
    //   0x6FB0263F: ret
    //   0x6FB02645: call     0x6fab1020
    //   0x6FB0265D: call     0x6fad4b20
    //   0x6FB02668: ret
    return 1;
}

// 1.12a: 0x6FB02670..0x6FB027CE (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB02670(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[96]
    // Callees: 0x6fb00830, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6facf5b0, 0x6fab1590, 0x6fab15c0, 0x6fafe320, 0x6fad4b20, D2Common.dll!#11161 -> MISSILE_SetTargetX
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB02686: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB0268D: cmp      ebx, dword ptr [eax + 0xba0]
    //   0x6FB026A1: imul     edi, edi, 0x23c
    //   0x6FB026B2: call     0x6fb00830
    //   0x6FB026C3: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB026CB: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB026F1: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB026FE: call     0x6facf5b0
    //   0x6FB0270B: call     0x6fab1590
    //   0x6FB02714: call     0x6fab1590
    //   0x6FB0271D: call     0x6fab15c0
    //   0x6FB02726: call     0x6fab15c0
    //   0x6FB02748: call     0x6fab1590
    //   0x6FB02755: call     0x6fab15c0
    //   0x6FB02783: call     0x6fafe320
    //   0x6FB027A1: call     0x6fad4b20
    //   0x6FB027B0: call     0x6fabc304 ; D2Common.dll!#11161 -> MISSILE_SetTargetX
    //   0x6FB027C0: ret      4
    //   0x6FB027CB: ret      4
    return 1;
}

// 1.12a: 0x6FB027D0..0x6FB02983 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB027D0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[26]
    // Callees: 0x6fafe320, 0x6fafe3f0, D2Common.dll!#10366 -> UNITS_GetRoom, 0x6fab1a00, D2Common.dll!#10438 -> DUNGEON_IsRoomInTown, 0x6fab1590, 0x6fab15c0, 0x6fad4b20
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB027E9: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB027F1: cmp      eax, dword ptr [ecx + 0xba0]
    //   0x6FB02803: imul     eax, eax, 0x23c
    //   0x6FB0282E: cmp      esi, edx
    //   0x6FB02838: imul     eax, eax, 0x1a4
    //   0x6FB0284F: ret      4
    //   0x6FB0285E: cmp      eax, edx
    //   0x6FB0286B: call     0x6fafe320
    //   0x6FB02878: call     0x6fafe3f0
    //   0x6FB02882: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB02898: call     0x6fab1a00
    //   0x6FB028A4: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB028AF: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB028C1: ret      4
    //   0x6FB02906: call     0x6fab1590
    //   0x6FB02911: call     0x6fab15c0
    //   0x6FB0292D: call     0x6fad4b20
    //   0x6FB02942: call     0x6fad4b20
    //   0x6FB02965: call     0x6fad4b20
    //   0x6FB02976: ret      4
    //   0x6FB02980: ret      4
    return 1;
}

// 1.12a: 0x6FB02990..0x6FB02B1C (__usercall, ret 0x8)
int32_t __usercall sub_6FB02990(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2)
{
    // Callees: 0x6fafe320, 0x6fab1020, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fad4b20, 0x6fb01c20, D2Common.dll!#10295 -> STATES_CheckState, D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB029A4: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB029AB: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB029BF: imul     esi, esi, 0x23c
    //   0x6FB029E6: cmp      ecx, eax
    //   0x6FB029FD: cmp      ecx, eax
    //   0x6FB02A13: call     0x6fafe320
    //   0x6FB02A23: ret      8
    //   0x6FB02A36: call     0x6fab1020
    //   0x6FB02A54: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB02A5B: cmp      esi, 1
    //   0x6FB02AB1: call     0x6fad4b20
    //   0x6FB02AC0: call     0x6fb01c20
    //   0x6FB02AC8: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB02ADB: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB02AF0: ret      8
    //   0x6FB02B0E: ret      8
    //   0x6FB02B19: ret      8
    return 1;
}

// 1.12a: 0x6FB02B20..0x6FB02C3D (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB02B20(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[23]
    // Callees: 0x6fb00830, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fab1590, 0x6fab15c0, 0x6fafe320, 0x6fad4b20
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB02B32: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB02B39: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB02B4D: imul     esi, esi, 0x23c
    //   0x6FB02B5E: call     0x6fb00830
    //   0x6FB02B6F: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB02B77: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB02B9D: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB02BBF: call     0x6fab1590
    //   0x6FB02BCA: call     0x6fab15c0
    //   0x6FB02BF8: call     0x6fafe320
    //   0x6FB02C08: ret      4
    //   0x6FB02C19: call     0x6fad4b20
    //   0x6FB02C1F: cmp      edi, esi
    //   0x6FB02C2F: ret      4
    //   0x6FB02C3A: ret      4
    return 1;
}

// 1.12a: 0x6FB02C40..0x6FB02DC6 (__fastcall, ret 0x4)
// 1.12a: 0x6FB02CE0 (internal entry point sub_6FB02CE0)
// 1.12a: 0x6FB02DB0 (internal entry point sub_6FB02DB0 - gpfnCltDoFunc[24])
int32_t __fastcall sub_6FB02C40(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[87]
    // Callees: 0x6fb00830, D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10965 -> SKILLS_SetParam1, 0x6fab1020, 0x6fab1590, 0x6fab15c0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    SKILLS_SetParam1(pSkill, 0);
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB02C52: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB02C59: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB02C6D: imul     edi, edi, 0x23c
    //   0x6FB02C7E: call     0x6fb00830
    //   0x6FB02C8F: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB02C97: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB02CA7: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB02CC7: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB02CDD: ret      4
    //   0x6FB02CEA: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB02D01: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB02D13: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB02D1F: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB02D40: call     0x6fab1020
    //   0x6FB02D4F: call     0x6fab1590
    //   0x6FB02D5B: call     0x6fab1020
    //   0x6FB02D6A: call     0x6fab15c0
    //   0x6FB02D78: call     0x6fab1020
    //   0x6FB02D91: call     0x6fadb4b0
    //   0x6FB02DA2: ret      4
    //   0x6FB02DAD: ret      4
    //   0x6FB02DB8: call     0x6fb02990
    //   0x6FB02DC3: ret      4
    return 1;
}

// 1.12a: 0x6FB02DD0..0x6FB02E04 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB02DD0(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB02DDD: cmp      ecx, edx
    //   0x6FB02DE4: cmp      ecx, eax
    //   0x6FB02DF0: cmp      esi, ecx
    //   0x6FB02DF7: cmp      esi, eax
    //   0x6FB02E00: ret
    //   0x6FB02E03: ret
    return 1;
}

// 1.12a: 0x6FB02E10..0x6FB02E1E (__usercall, ret 0x0)
int32_t __usercall sub_6FB02E10(D2UnitStrc* pUnit /*eax*/)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB02E1D: ret
    return 1;
}

// 1.12a: 0x6FB02E20..0x6FB02F05 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB02E20(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB02E2C: cmp      ebx, ebp
    //   0x6FB02E3D: cmp      esi, ebp
    //   0x6FB02E66: cmp      esi, 1
    //   0x6FB02E73: cmp      esi, 3
    //   0x6FB02E9C: cmp      edi, eax
    //   0x6FB02EC2: cmp      eax, 3
    //   0x6FB02EFB: cmp      eax, edi
    //   0x6FB02F04: ret
    return 1;
}

// 1.12a: 0x6FB02F10..0x6FB02F4F (__usercall, ret 0x8)
int32_t __usercall sub_6FB02F10(D2UnitStrc* pUnit /*eax*/, int32_t nArgEdx /*edx*/, int32_t a1, int32_t a2)
{
    // Callees: D2gfx.dll!#10024 -> TEXTURE_CelDraw
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB02F43: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FB02F4C: ret      8
    return 1;
}

// 1.12a: 0x6FB02F50..0x6FB03035 (__usercall, ret 0x0)
int32_t __usercall sub_6FB02F50(D2UnitStrc* pUnit /*eax*/, int32_t nArgEdx /*edx*/)
{
    // Callees: D2gfx.dll!#10024 -> TEXTURE_CelDraw
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB02FBE: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FB02FC6: ret
    //   0x6FB02FFF: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FB03007: ret
    //   0x6FB0302C: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FB03034: ret
    return 1;
}

// 1.12a: 0x6FB03040..0x6FB03057 (__usercall, ret 0x0)
int32_t __usercall sub_6FB03040(D2UnitStrc* pUnit /*eax*/)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0304A: call     eax
    //   0x6FB03050: ret
    //   0x6FB03056: ret
    return 1;
}

// 1.12a: 0x6FB03060..0x6FB03087 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03060(D2UnitStrc* pUnit)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0307D: ret
    //   0x6FB03086: ret
    return 1;
}

// 1.12a: 0x6FB03090..0x6FB030B1 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03090(D2UnitStrc* pUnit)
{
    // Referenced strings: 'AutoMap Party Names', 'Diablo II'
    // Callees: Storm.dll!#426 -> SRegSaveValue
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0309A: push     0x6fb85d3c ; STR:'AutoMap Party Names'
    //   0x6FB0309F: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB030A4: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB030B0: ret
    return 1;
}

// 1.12a: 0x6FB030C0..0x6FB030C6 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB030C0(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB030C5: ret
    return 1;
}

// 1.12a: 0x6FB030D0..0x6FB030DC (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB030D0(D2UnitStrc* pUnit)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB030DB: ret
    return 1;
}

// 1.12a: 0x6FB030E0..0x6FB03101 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB030E0(D2UnitStrc* pUnit)
{
    // Referenced strings: 'AutoMap Party', 'Diablo II'
    // Callees: Storm.dll!#426 -> SRegSaveValue
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB030EA: push     0x6fb85d50 ; STR:'AutoMap Party'
    //   0x6FB030EF: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB030F4: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB03100: ret
    return 1;
}

// 1.12a: 0x6FB03110..0x6FB0311C (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03110(D2UnitStrc* pUnit)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0311B: ret
    return 1;
}

// 1.12a: 0x6FB03120..0x6FB03141 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03120(D2UnitStrc* pUnit)
{
    // Referenced strings: 'AutoMap Centers', 'Diablo II'
    // Callees: Storm.dll!#426 -> SRegSaveValue
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0312A: push     0x6fb85d70 ; STR:'AutoMap Centers'
    //   0x6FB0312F: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03134: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB03140: ret
    return 1;
}

// 1.12a: 0x6FB03150..0x6FB031C1 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03150(D2UnitStrc* pUnit)
{
    // Referenced strings: 'AutoMapFade', 'Diablo II'
    // Callees: Storm.dll!#423 -> SRegLoadValue, 0x6faed870
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03159: push     0x6fb85350 ; STR:'AutoMapFade'
    //   0x6FB0315E: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03165: call     0x6fabbe90 ; Storm.dll!#423 -> SRegLoadValue
    //   0x6FB0316A: cmp      dword ptr [esp + 4], 4
    //   0x6FB0317B: call     0x6faed870
    //   0x6FB03180: cmp      dword ptr [0x6fbcc2a0], 1
    //   0x6FB0318E: cmp      eax, 1
    //   0x6FB031B1: ret
    //   0x6FB031C0: ret
    return 1;
}

// 1.12a: 0x6FB031D0..0x6FB0320B (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB031D0(D2UnitStrc* pUnit)
{
    // Referenced strings: 'AutoMapFade', 'Diablo II'
    // Callees: 0x6faed870, Storm.dll!#426 -> SRegSaveValue
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB031D3: cmp      dword ptr [esi + 0x124], 4
    //   0x6FB031EC: call     0x6faed870
    //   0x6FB031FA: push     0x6fb85350 ; STR:'AutoMapFade'
    //   0x6FB031FF: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03204: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB0320A: ret
    return 1;
}

// 1.12a: 0x6FB03210..0x6FB03215 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB03210(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB03220..0x6FB03225 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB03220(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB03230..0x6FB03253 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03230(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Blended Shadows', 'Diablo II'
    // Callees: Storm.dll!#423 -> SRegLoadValue, D2gfx.dll!#10065 -> D2GFX_SetBlendShadows
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0323A: push     0x6fb85394 ; STR:'Blended Shadows'
    //   0x6FB0323F: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03244: call     0x6fabbe90 ; Storm.dll!#423 -> SRegLoadValue
    //   0x6FB0324C: call     0x6fabd186 ; D2gfx.dll!#10065 -> D2GFX_SetBlendShadows
    //   0x6FB03252: ret
    return 1;
}

// 1.12a: 0x6FB03260..0x6FB03289 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03260(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Blended Shadows', 'Diablo II'
    // Callees: Storm.dll!#426 -> SRegSaveValue, D2gfx.dll!#10065 -> D2GFX_SetBlendShadows
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0326C: push     0x6fb85394 ; STR:'Blended Shadows'
    //   0x6FB03271: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03276: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB03282: call     0x6fabd186 ; D2gfx.dll!#10065 -> D2GFX_SetBlendShadows
    //   0x6FB03288: ret
    return 1;
}

// 1.12a: 0x6FB03290..0x6FB032EF (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03290(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Light Quality', 'Diablo II'
    // Callees: Storm.dll!#423 -> SRegLoadValue, D2gfx.dll!#10073 -> D2GFX_SetLowQuality
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0329A: push     0x6fb853a4 ; STR:'Light Quality'
    //   0x6FB0329F: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB032A4: call     0x6fabbe90 ; Storm.dll!#423 -> SRegLoadValue
    //   0x6FB032B8: call     0x6fabd18c ; D2gfx.dll!#10073 -> D2GFX_SetLowQuality
    //   0x6FB032C8: ret
    //   0x6FB032CB: call     0x6fabd18c ; D2gfx.dll!#10073 -> D2GFX_SetLowQuality
    //   0x6FB032DB: ret
    //   0x6FB032DE: call     0x6fabd18c ; D2gfx.dll!#10073 -> D2GFX_SetLowQuality
    //   0x6FB032EE: ret
    return 1;
}

// 1.12a: 0x6FB032F0..0x6FB03353 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB032F0(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Light Quality', 'Diablo II'
    // Callees: Storm.dll!#426 -> SRegSaveValue, D2gfx.dll!#10073 -> D2GFX_SetLowQuality
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB032FC: push     0x6fb853a4 ; STR:'Light Quality'
    //   0x6FB03301: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03306: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB0331F: call     0x6fabd18c ; D2gfx.dll!#10073 -> D2GFX_SetLowQuality
    //   0x6FB0332E: ret
    //   0x6FB03331: call     0x6fabd18c ; D2gfx.dll!#10073 -> D2GFX_SetLowQuality
    //   0x6FB03340: ret
    //   0x6FB03343: call     0x6fabd18c ; D2gfx.dll!#10073 -> D2GFX_SetLowQuality
    //   0x6FB03352: ret
    return 1;
}

// 1.12a: 0x6FB03360..0x6FB033D2 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03360(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Callees: 0x6fb79a37
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0338A: cmp      dword ptr [0x6fbd36f0], 0
    //   0x6FB0339A: call     0x6fb79a37
    return 1;
}

// 1.12a: 0x6FB033E0..0x6FB03448 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB033E0(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1)
{
    // Callees: 0x6fb79a37, 0x6fb7a6ac
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03429: cmp      dword ptr [0x6fbd36f0], 0
    //   0x6FB03439: call     0x6fb79a37
    //   0x6FB0343F: call     0x6fb7a6ac
    //   0x6FB03445: ret      4
    return 1;
}

// 1.12a: 0x6FB03450..0x6FB034C5 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03450(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0349C: cmp      esi, eax
    //   0x6FB034A6: cmp      esi, eax
    //   0x6FB034B2: cmp      eax, ecx
    //   0x6FB034B6: cmp      eax, edx
    //   0x6FB034C0: ret
    //   0x6FB034C4: ret
    return 1;
}

// 1.12a: 0x6FB034D0..0x6FB03553 (__usercall, ret 0x0)
int32_t __usercall sub_6FB034D0(D2UnitStrc* pUnit /*eax*/)
{
    // Referenced strings: 'Mini Panel', 'Diablo II', '..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    // Callees: Storm.dll!#426 -> SRegSaveValue, D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware, Fog.dll!#10043 -> FOG_Free
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB034E5: push     0x6fb8443c ; STR:'Mini Panel'
    //   0x6FB034EA: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB034EF: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB03500: call     0x6fabc0d0 ; D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware
    //   0x6FB0350D: mov      edx, 0x6fb807a0 ; STR:'..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    //   0x6FB03514: call     0x6fabbf86 ; Fog.dll!#10043 -> FOG_Free
    //   0x6FB0352E: call     0x6fabc0d0 ; D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware
    //   0x6FB0353B: mov      edx, 0x6fb807a0 ; STR:'..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    //   0x6FB03542: call     0x6fabbf86 ; Fog.dll!#10043 -> FOG_Free
    //   0x6FB03552: ret
    return 1;
}

// 1.12a: 0x6FB03560..0x6FB03576 (__usercall, ret 0x0)
int32_t __usercall sub_6FB03560(D2UnitStrc* pUnit /*eax*/)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03572: ret
    //   0x6FB03575: ret
    return 1;
}

// 1.12a: 0x6FB03580..0x6FB03644 (__usercall, ret 0x0)
int32_t __usercall sub_6FB03580(D2UnitStrc* pUnit /*eax*/)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB035F1: cmp      ecx, 5
    //   0x6FB03620: ret
    //   0x6FB0362B: ret
    return 1;
}

// 1.12a: 0x6FB03650..0x6FB0370B (__usercall, ret 0x0)
int32_t __usercall sub_6FB03650(D2UnitStrc* pUnit /*eax*/)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB036F6: ret
    //   0x6FB0370A: ret
    return 1;
}

// 1.12a: 0x6FB03710..0x6FB037DA (__usercall, ret 0x0)
int32_t __usercall sub_6FB03710(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: D2gfx.dll!#10024 -> TEXTURE_CelDraw
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03744: cmp      dword ptr [esi*4 + 0x6fbc2750], 1
    //   0x6FB037C5: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FB037CB: cmp      esi, ebx
    //   0x6FB037D9: ret
    return 1;
}

// 1.12a: 0x6FB037E0..0x6FB037F8 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB037E0(void)
{
    // Referenced strings: 'Always Run', 'Diablo II'
    // Callees: Storm.dll!#426 -> SRegSaveValue
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB037E8: push     0x6fb852f0 ; STR:'Always Run'
    //   0x6FB037ED: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB037F2: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB037F7: ret
    return 1;
}

// 1.12a: 0x6FB03800..0x6FB03829 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03800(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Always Run', 'Diablo II'
    // Callees: Storm.dll!#423 -> SRegLoadValue
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03807: push     0x6fb852f0 ; STR:'Always Run'
    //   0x6FB0380C: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03819: call     0x6fabbe90 ; Storm.dll!#423 -> SRegLoadValue
    //   0x6FB03828: ret
    return 1;
}

// 1.12a: 0x6FB03830..0x6FB039C7 (__fastcall, ret 0xC)
int32_t __fastcall sub_6FB03830(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: 0x6fb79a37, 0x6fb7a6ac, D2gfx.dll!#10062 -> D2GFX_DrawBox, D2Win.dll!#10005 -> D2Win_10134_DrawCellFile, D2gfx.dll!#10024 -> TEXTURE_CelDraw
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0389D: cmp      dword ptr [0x6fbd36f0], 0
    //   0x6FB038AD: call     0x6fb79a37
    //   0x6FB038B9: call     0x6fb7a6ac
    //   0x6FB03917: call     0x6fabd08a ; D2gfx.dll!#10062 -> D2GFX_DrawBox
    //   0x6FB0393D: call     0x6fabd08a ; D2gfx.dll!#10062 -> D2GFX_DrawBox
    //   0x6FB03989: call     0x6fabd3b4 ; D2Win.dll!#10005 -> D2Win_10134_DrawCellFile
    //   0x6FB039B8: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FB039C4: ret      0xc
    return 1;
}

// 1.12a: 0x6FB039D0..0x6FB03AE4 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB039D0(D2UnitStrc* pUnit, int32_t a1)
{
    // Referenced strings: '..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    // Callees: Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware, Fog.dll!#10043 -> FOG_Free
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB039ED: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB039F8: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB03A02: call     0x6fab326d
    //   0x6FB03A07: cmp      dword ptr [edi], 0
    //   0x6FB03A28: cmp      esi, ebp
    //   0x6FB03A2D: call     0x6fabc0d0 ; D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware
    //   0x6FB03A32: cmp      esi, ebp
    //   0x6FB03A39: mov      edx, 0x6fb807a0 ; STR:'..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    //   0x6FB03A40: call     0x6fabbf86 ; Fog.dll!#10043 -> FOG_Free
    //   0x6FB03A4B: cmp      dword ptr [ebx - 0x120], 1
    //   0x6FB03A56: cmp      eax, ebp
    //   0x6FB03A5A: cmp      eax, 4
    //   0x6FB03A5F: cmp      eax, ebp
    //   0x6FB03A77: call     0x6fabc0d0 ; D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware
    //   0x6FB03A84: mov      edx, 0x6fb807a0 ; STR:'..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    //   0x6FB03A8B: call     0x6fabbf86 ; Fog.dll!#10043 -> FOG_Free
    //   0x6FB03A9C: cmp      ebp, eax
    //   0x6FB03AB1: cmp      eax, ecx
    //   0x6FB03AC2: ret      4
    //   0x6FB03ACA: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB03AD5: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB03ADF: call     0x6fab326d
    return 1;
}

// 1.12a: 0x6FB03AF0..0x6FB03B7C (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03AF0(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03AFC: imul     eax, esi
    //   0x6FB03B1B: cmp      ebx, edx
    //   0x6FB03B21: cmp      ebx, eax
    //   0x6FB03B3C: cmp      ebx, eax
    //   0x6FB03B49: cmp      ecx, esi
    //   0x6FB03B53: imul     eax, eax, 0x550
    //   0x6FB03B5E: cmp      ecx, -1
    //   0x6FB03B6D: call     eax
    //   0x6FB03B7B: ret
    return 1;
}

// 1.12a: 0x6FB03B80..0x6FB03BBC (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03B80(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Contrast', 'Diablo II'
    // Callees: 0x6fb03360, Storm.dll!#426 -> SRegSaveValue, D2gfx.dll!#10028 -> D2GFX_SetOption, D2gfx.dll!#10022 -> D2GFX_SetGamma
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03B8C: call     0x6fb03360
    //   0x6FB03B94: push     0x6fb85368 ; STR:'Contrast'
    //   0x6FB03B99: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03B9E: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB03BAE: call     0x6fabd192 ; D2gfx.dll!#10028 -> D2GFX_SetOption
    //   0x6FB03BB5: call     0x6fabd180 ; D2gfx.dll!#10022 -> D2GFX_SetGamma
    //   0x6FB03BBB: ret
    return 1;
}

// 1.12a: 0x6FB03BC0..0x6FB03BEE (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03BC0(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Gamma', 'Diablo II'
    // Callees: 0x6fb03360, Storm.dll!#426 -> SRegSaveValue, D2gfx.dll!#10022 -> D2GFX_SetGamma
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03BCD: call     0x6fb03360
    //   0x6FB03BD7: push     0x6fb85374 ; STR:'Gamma'
    //   0x6FB03BDC: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03BE1: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB03BE7: call     0x6fabd180 ; D2gfx.dll!#10022 -> D2GFX_SetGamma
    //   0x6FB03BED: ret
    return 1;
}

// 1.12a: 0x6FB03BF0..0x6FB03C76 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03BF0(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Text Display Beta', 'Diablo II'
    // Callees: D2sound.dll!#10026 -> D2SOUND_SetNPCSpeech, Storm.dll!#426 -> SRegSaveValue
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03BF9: call     0x6fabd336 ; D2sound.dll!#10026 -> D2SOUND_SetNPCSpeech
    //   0x6FB03C18: push     0x6fb84c68 ; STR:'Text Display Beta'
    //   0x6FB03C1D: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03C2C: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB03C31: ret
    //   0x6FB03C39: push     0x6fb84c68 ; STR:'Text Display Beta'
    //   0x6FB03C3E: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03C4E: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB03C53: ret
    //   0x6FB03C56: push     0x6fb84c68 ; STR:'Text Display Beta'
    //   0x6FB03C5B: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB03C70: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB03C75: ret
    return 1;
}

// 1.12a: 0x6FB03C80..0x6FB03C86 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB03C80(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03C85: ret
    return 1;
}

// 1.12a: 0x6FB03C90..0x6FB03CAD (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03C90(D2UnitStrc* pUnit)
{
    // Callees: D2sound.dll!#10017 -> D2SOUND_GetPositionalBias, 0x6fb033e0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03C93: call     0x6fabd258 ; D2sound.dll!#10017 -> D2SOUND_GetPositionalBias
    //   0x6FB03CA0: call     0x6fb033e0
    //   0x6FB03CAC: ret
    return 1;
}

// 1.12a: 0x6FB03CB0..0x6FB03CC5 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03CB0(D2UnitStrc* pUnit)
{
    // Callees: 0x6fb03360
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03CB9: call     0x6fb03360
    return 1;
}

// 1.12a: 0x6FB03CD0..0x6FB03CED (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03CD0(D2UnitStrc* pUnit)
{
    // Callees: D2sound.dll!#10035 -> D2SOUND_GetMusicVolume, 0x6fb033e0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03CD3: call     0x6fabd2d6 ; D2sound.dll!#10035 -> D2SOUND_GetMusicVolume
    //   0x6FB03CE0: call     0x6fb033e0
    //   0x6FB03CEC: ret
    return 1;
}

// 1.12a: 0x6FB03CF0..0x6FB03D05 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03CF0(D2UnitStrc* pUnit)
{
    // Callees: 0x6fb03360
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03CF9: call     0x6fb03360
    return 1;
}

// 1.12a: 0x6FB03D10..0x6FB03D16 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB03D10(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03D15: ret
    return 1;
}

// 1.12a: 0x6FB03D20..0x6FB03D3D (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03D20(D2UnitStrc* pUnit)
{
    // Callees: D2sound.dll!#10023 -> D2SOUND_GetMasterVolume, 0x6fb033e0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03D23: call     0x6fabd2ee ; D2sound.dll!#10023 -> D2SOUND_GetMasterVolume
    //   0x6FB03D30: call     0x6fb033e0
    //   0x6FB03D3C: ret
    return 1;
}

// 1.12a: 0x6FB03D40..0x6FB03D55 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB03D40(D2UnitStrc* pUnit)
{
    // Callees: 0x6fb03360
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03D49: call     0x6fb03360
    return 1;
}

// 1.12a: 0x6FB03D60..0x6FB03D66 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB03D60(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03D65: ret
    return 1;
}

// 1.12a: 0x6FB03D70..0x6FB03D76 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB03D70(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03D75: ret
    return 1;
}

// 1.12a: 0x6FB03D80..0x6FB03F54 (__usercall, ret 0x8)
// 1.12a: 0x6FB03DA0 (internal entry point sub_6FB03DA0)
// 1.12a: 0x6FB03E00 (internal entry point sub_6FB03E00)
int32_t __usercall sub_6FB03D80(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ, D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex, IAT:D2Lang.dll!?strcpy@Unicode@@SIPAU1@PAU1@PBU1@@Z -> ?strcpy@Unicode@@SIPAU1@PAU1@PBU1@@Z, 0x6fb4fa40, 0x6fb50060, D2Win.dll!#10031 -> D2Win_10130_DrawFramedTextEx
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03D8A: cmp      eax, 7
    //   0x6FB03E25: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB03E3B: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FB03E46: call     dword ptr [0x6fb7eaa4] ; IAT:D2Lang.dll!?strcpy@Unicode@@SIPAU1@PAU1@PBU1@@Z -> ?strcpy@Unicode@@SIPAU1@PAU1@PBU1@@Z
    //   0x6FB03E54: cmp      esi, 0x39
    //   0x6FB03E68: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB03E79: call     0x6fb4fa40
    //   0x6FB03E7E: cmp      ax, 0xffff
    //   0x6FB03E82: mov      ebx, dword ptr [0x6fb7eb04] ; IAT:D2Lang.dll!?sprintf@Unicode@@SAXHPAU1@PBU1@ZZ -> ?sprintf@Unicode@@SAXHPAU1@PBU1@ZZ
    //   0x6FB03E88: mov      ebp, dword ptr [0x6fb7eaa8] ; IAT:D2Lang.dll!?strcat@Unicode@@SIPAU1@PAU1@PBU1@@Z -> ?strcat@Unicode@@SIPAU1@PAU1@PBU1@@Z
    //   0x6FB03E9D: call     0x6fb50060
    //   0x6FB03EA8: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FB03EB5: call     ebx
    //   0x6FB03EC2: call     ebp
    //   0x6FB03EC6: call     0x6fb4fa40
    //   0x6FB03ECB: cmp      ax, 0xffff
    //   0x6FB03EDE: call     0x6fb50060
    //   0x6FB03EE9: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FB03EF6: call     ebx
    //   0x6FB03F03: call     ebp
    //   0x6FB03F22: call     0x6fabd366 ; D2Win.dll!#10031 -> D2Win_10130_DrawFramedTextEx
    //   0x6FB03F31: ret      8
    //   0x6FB03F39: cmp      eax, 0x3dbc6fb0
    return 1;
}

// 1.12a: 0x6FB03F60..0x6FB0405D (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB03F60(void)
{
    // Referenced strings: '..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    // Callees: 0x6fb039d0, Fog.dll!#10227 -> FOG_IsExpansion, D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware, Fog.dll!#10043 -> FOG_Free
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB03F7A: call     0x6fb039d0
    //   0x6FB03F89: call     0x6fb039d0
    //   0x6FB03F98: call     0x6fb039d0
    //   0x6FB03F9D: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB03FB0: call     0x6fb039d0
    //   0x6FB03FCB: call     0x6fb039d0
    //   0x6FB03FDA: call     0x6fb039d0
    //   0x6FB03FE5: cmp      esi, edi
    //   0x6FB03FEA: call     0x6fabc0d0 ; D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware
    //   0x6FB03FEF: cmp      esi, edi
    //   0x6FB03FF6: mov      edx, 0x6fb807a0 ; STR:'..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    //   0x6FB03FFD: call     0x6fabbf86 ; Fog.dll!#10043 -> FOG_Free
    //   0x6FB0400E: cmp      esi, edi
    //   0x6FB04013: call     0x6fabc0d0 ; D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware
    //   0x6FB04018: cmp      esi, edi
    //   0x6FB0401F: mov      edx, 0x6fb807a0 ; STR:'..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    //   0x6FB04026: call     0x6fabbf86 ; Fog.dll!#10043 -> FOG_Free
    //   0x6FB04037: cmp      esi, edi
    //   0x6FB0403C: call     0x6fabc0d0 ; D2CMP.dll!#10106 -> D2CMP_CelFileFreeHardware
    //   0x6FB04041: cmp      esi, edi
    //   0x6FB04048: mov      edx, 0x6fb807a0 ; STR:'..\\Source\\D2Client\\CORE\\ARCHIVE.CPP'
    //   0x6FB0404F: call     0x6fabbf86 ; Fog.dll!#10043 -> FOG_Free
    //   0x6FB0405C: ret
    return 1;
}

// 1.12a: 0x6FB04060..0x6FB04246 (__stdcall, ret 0x8)
// 1.12a: 0x6FB04220 (internal entry point sub_6FB04220)
int32_t __stdcall sub_6FB04060(int32_t a1, int32_t a2)
{
    // Referenced strings: 'DATA\\LOCAL', '%s\\UI\\%s\\%s'
    // Callees: Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Lang.dll!#10004 -> STRTABLE_GetLanguageInfo, Fog.dll!#10227 -> FOG_IsExpansion, 0x6fab329e, 0x6fb6ef70
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB04077: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB04082: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB0408C: call     0x6fab326d
    //   0x6FB040A2: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB040AD: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB040B7: call     0x6fab326d
    //   0x6FB040E8: call     0x6fabd1e6 ; D2Lang.dll!#10004 -> STRTABLE_GetLanguageInfo
    //   0x6FB040ED: cmp      dword ptr [esi], 0
    //   0x6FB04120: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB0412D: cmp      byte ptr [esi], 0
    //   0x6FB0413C: push     0x6fb83c98 ; STR:'DATA\\LOCAL'
    //   0x6FB04145: push     0x6fb85344 ; STR:'%s\\UI\\%s\\%s'
    //   0x6FB0414B: call     0x6fab329e
    //   0x6FB04159: call     0x6fb6ef70
    //   0x6FB04164: cmp      dword ptr [ebp - 0x120], 1
    //   0x6FB04178: cmp      eax, 4
    //   0x6FB04190: cmp      byte ptr [edi], 0
    //   0x6FB0419F: push     0x6fb83c98 ; STR:'DATA\\LOCAL'
    //   0x6FB041A8: push     0x6fb85344 ; STR:'%s\\UI\\%s\\%s'
    //   0x6FB041AE: call     0x6fab329e
    //   0x6FB041BC: call     0x6fb6ef70
    //   0x6FB041D8: cmp      esi, eax
    //   0x6FB041EB: call     eax
    //   0x6FB04207: cmp      eax, edx
    //   0x6FB0421D: ret      8
    //   0x6FB0422C: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB04237: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB04241: call     0x6fab326d
    return 1;
}

// 1.12a: 0x6FB04250..0x6FB042DE (__cdecl, ret 0x0)
// 1.12a: 0x6FB04260 (internal entry point sub_6FB04260)
int32_t __cdecl sub_6FB04250(void)
{
    // Referenced strings: 'Contrast', 'Diablo II'
    // Callees: 0x6fb03af0, D2gfx.dll!#10019 -> D2GFX_GetDisplayType, Storm.dll!#423 -> SRegLoadValue, 0x6fb033e0, 0x6fb03360, Storm.dll!#426 -> SRegSaveValue, D2gfx.dll!#10028 -> D2GFX_SetOption, D2gfx.dll!#10022 -> D2GFX_SetGamma
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB04250: call     0x6fb03af0
    //   0x6FB04255: cmp      eax, -1
    //   0x6FB0425F: ret
    //   0x6FB04264: call     0x6fabd13e ; D2gfx.dll!#10019 -> D2GFX_GetDisplayType
    //   0x6FB0427E: push     0x6fb85368 ; STR:'Contrast'
    //   0x6FB04283: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB04288: call     0x6fabbe90 ; Storm.dll!#423 -> SRegLoadValue
    //   0x6FB04299: call     0x6fb033e0
    //   0x6FB042AD: call     0x6fb03360
    //   0x6FB042B5: push     0x6fb85368 ; STR:'Contrast'
    //   0x6FB042BA: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB042BF: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB042CF: call     0x6fabd192 ; D2gfx.dll!#10028 -> D2GFX_SetOption
    //   0x6FB042D6: call     0x6fabd180 ; D2gfx.dll!#10022 -> D2GFX_SetGamma
    //   0x6FB042DD: ret
    return 1;
}

// 1.12a: 0x6FB042E0..0x6FB04353 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB042E0(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Gamma', 'Diablo II'
    // Callees: Storm.dll!#423 -> SRegLoadValue, 0x6fb033e0, D2gfx.dll!#10011 -> D2GFX_GammaCanBeControlled, 0x6fb03360, Storm.dll!#426 -> SRegSaveValue, D2gfx.dll!#10022 -> D2GFX_SetGamma
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB042E9: push     0x6fb85374 ; STR:'Gamma'
    //   0x6FB042EE: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB042FD: call     0x6fabbe90 ; Storm.dll!#423 -> SRegLoadValue
    //   0x6FB04311: call     0x6fb033e0
    //   0x6FB0431C: call     0x6fabd19e ; D2gfx.dll!#10011 -> D2GFX_GammaCanBeControlled
    //   0x6FB04331: call     0x6fb03360
    //   0x6FB0433B: push     0x6fb85374 ; STR:'Gamma'
    //   0x6FB04340: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB04345: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB0434B: call     0x6fabd180 ; D2gfx.dll!#10022 -> D2GFX_SetGamma
    //   0x6FB04352: ret
    return 1;
}

// 1.12a: 0x6FB04360..0x6FB043A3 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB04360(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Perspective', 'Diablo II'
    // Callees: Storm.dll!#426 -> SRegSaveValue, D2gfx.dll!#10069 -> D2GFX_SetPerspective, 0x6fb6a990
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0436C: push     0x6fb8537c ; STR:'Perspective'
    //   0x6FB04371: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB04376: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB04382: call     0x6fabd198 ; D2gfx.dll!#10069 -> D2GFX_SetPerspective
    //   0x6FB0439C: call     0x6fb6a990
    //   0x6FB043A2: ret
    return 1;
}

// 1.12a: 0x6FB043B0..0x6FB043D8 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB043B0(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Callees: D2sound.dll!#10001 -> D2SOUND_GetNPCSpeech
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB043B6: call     0x6fabd330 ; D2sound.dll!#10001 -> D2SOUND_GetNPCSpeech
    //   0x6FB043D7: ret
    return 1;
}

// 1.12a: 0x6FB043E0..0x6FB0440B (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB043E0(D2UnitStrc* pUnit)
{
    // Callees: D2sound.dll!#10007 -> D2SOUND_GetSoundMixer
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB043F5: ret
    //   0x6FB043F6: call     0x6fabd2dc ; D2sound.dll!#10007 -> D2SOUND_GetSoundMixer
    //   0x6FB0440A: ret
    return 1;
}

// 1.12a: 0x6FB04410..0x6FB04421 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB04410(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0441B: ret
    return 1;
}

// 1.12a: 0x6FB04430..0x6FB04451 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB04430(void)
{
    // Callees: D2sound.dll!#10007 -> D2SOUND_GetSoundMixer
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0443B: ret
    //   0x6FB0443C: call     0x6fabd2dc ; D2sound.dll!#10007 -> D2SOUND_GetSoundMixer
    //   0x6FB04441: cmp      eax, 1
    //   0x6FB04446: cmp      eax, 2
    //   0x6FB04450: ret
    return 1;
}

// 1.12a: 0x6FB04460..0x6FB04492 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB04460(D2UnitStrc* pUnit)
{
    // Callees: D2sound.dll!#10007 -> D2SOUND_GetSoundMixer
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0446C: call     0x6fabd2dc ; D2sound.dll!#10007 -> D2SOUND_GetSoundMixer
    //   0x6FB04471: cmp      eax, 1
    //   0x6FB04476: cmp      eax, 2
    //   0x6FB04484: ret
    //   0x6FB04491: ret
    return 1;
}

// 1.12a: 0x6FB044A0..0x6FB044B1 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB044A0(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB044AB: ret
    return 1;
}

// 1.12a: 0x6FB044C0..0x6FB0452C (__cdecl, ret 0x0)
// 1.12a: 0x6FB044E0 (internal entry point sub_6FB044E0)
int32_t __cdecl sub_6FB044C0(void)
{
    // Referenced strings: 'Panel\\minipanel', 'Panel\\minipanel_s', 'Panel\\minipanelbtn'
    // Callees: 0x6fb3c170
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB044CA: mov      esi, 0x6fb852c8 ; STR:'Panel\\minipanel'
    //   0x6FB044CF: call     0x6fb3c170
    //   0x6FB044E0: mov      esi, 0x6fb852b4 ; STR:'Panel\\minipanel_s'
    //   0x6FB044E5: call     0x6fb3c170
    //   0x6FB044F4: mov      esi, 0x6fb852a0 ; STR:'Panel\\minipanelbtn'
    //   0x6FB044FE: call     0x6fb3c170
    //   0x6FB0452B: ret
    return 1;
}

// 1.12a: 0x6FB04530..0x6FB046A5 (__stdcall, ret 0x4)
int32_t __stdcall sub_6FB04530(int32_t a1)
{
    // Callees: D2Common.dll!#10078 -> INVENTORY_GetCursorItem, 0x6fb30ad0, 0x6fae1fa0, Storm.dll!#511 -> SMsgBreakHandlerChain
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0455D: call     0x6fabc1e4 ; D2Common.dll!#10078 -> INVENTORY_GetCursorItem
    //   0x6FB0459E: call     0x6fb30ad0
    //   0x6FB045A3: cmp      eax, 1
    //   0x6FB045D4: cmp      esi, ecx
    //   0x6FB045DF: cmp      esi, eax
    //   0x6FB045EF: cmp      edi, edx
    //   0x6FB045FA: cmp      edi, eax
    //   0x6FB04617: cmp      esi, ecx
    //   0x6FB0461E: cmp      esi, ecx
    //   0x6FB04623: cmp      eax, ebx
    //   0x6FB0462B: ret      4
    //   0x6FB0462E: cmp      dword ptr [eax*4 + 0x6fbc2730], 7
    //   0x6FB0463E: cmp      dword ptr [ecx], 0
    //   0x6FB04643: cmp      dword ptr [ecx + 0x10], 0x11
    //   0x6FB0465D: cmp      eax, 4
    //   0x6FB04662: cmp      eax, 5
    //   0x6FB0467F: call     0x6fae1fa0
    //   0x6FB04699: call     0x6fabbe78 ; Storm.dll!#511 -> SMsgBreakHandlerChain
    //   0x6FB046A2: ret      4
    return 1;
}

// 1.12a: 0x6FB046B0..0x6FB04754 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB046B0(D2UnitStrc* pUnit)
{
    // Callees: 0x6fb03d80
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB046D5: cmp      eax, 1
    //   0x6FB046E1: cmp      eax, 3
    //   0x6FB046FA: cmp      esi, ecx
    //   0x6FB04701: cmp      esi, eax
    //   0x6FB04713: cmp      ecx, edx
    //   0x6FB0471A: cmp      ecx, eax
    //   0x6FB0472B: cmp      edi, eax
    //   0x6FB04732: cmp      edi, ecx
    //   0x6FB04746: call     0x6fb03d80
    //   0x6FB0474C: cmp      esi, ebx
    //   0x6FB04753: ret
    return 1;
}

// 1.12a: 0x6FB04760..0x6FB048D2 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB04760(void)
{
    // Referenced strings: 'DATA\\GLOBAL', '%s\\UI\\Widgets\\OptBar', '%s\\UI\\Widgets\\OptBarC', '%s\\UI\\Widgets\\OptSkull'
    // Callees: 0x6fb04060, Fog.dll!#10227 -> FOG_IsExpansion, 0x6fab329e, 0x6fb6ef70, D2CMP.dll!#10018 -> D2CMP_CelFileGetCelsPerDirection, D2CMP.dll!#10038 -> D2CMP_CelGetHandle, D2CMP.dll!#10031 -> D2CMP_CelGetWidth
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB04778: call     0x6fb04060
    //   0x6FB04787: call     0x6fb04060
    //   0x6FB04796: call     0x6fb04060
    //   0x6FB0479B: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB047AE: call     0x6fb04060
    //   0x6FB047C9: call     0x6fb04060
    //   0x6FB047D8: call     0x6fb04060
    //   0x6FB047F1: push     0x6fb80424 ; STR:'DATA\\GLOBAL'
    //   0x6FB047FB: push     0x6fb8532c ; STR:'%s\\UI\\Widgets\\OptBar'
    //   0x6FB04815: call     0x6fab329e
    //   0x6FB04823: call     0x6fb6ef70
    //   0x6FB04828: push     0x6fb80424 ; STR:'DATA\\GLOBAL'
    //   0x6FB04831: push     0x6fb85314 ; STR:'%s\\UI\\Widgets\\OptBarC'
    //   0x6FB0483C: call     0x6fab329e
    //   0x6FB0484A: call     0x6fb6ef70
    //   0x6FB0484F: push     0x6fb80424 ; STR:'DATA\\GLOBAL'
    //   0x6FB04858: push     0x6fb852fc ; STR:'%s\\UI\\Widgets\\OptSkull'
    //   0x6FB04863: call     0x6fab329e
    //   0x6FB04871: call     0x6fb6ef70
    //   0x6FB04892: call     0x6fabc112 ; D2CMP.dll!#10018 -> D2CMP_CelFileGetCelsPerDirection
    //   0x6FB048A8: call     0x6fabc0be ; D2CMP.dll!#10038 -> D2CMP_CelGetHandle
    //   0x6FB048AE: call     0x6fabc0b8 ; D2CMP.dll!#10031 -> D2CMP_CelGetWidth
    //   0x6FB048B3: cmp      esi, eax
    //   0x6FB048BE: cmp      eax, edi
    //   0x6FB048D1: ret
    return 1;
}

// 1.12a: 0x6FB048E0..0x6FB04B0C (__cdecl, ret 0x0)
// 1.12a: 0x6FB049C0 (internal entry point sub_6FB049C0)
// 1.12a: 0x6FB04AA0 (internal entry point sub_6FB04AA0)
int32_t __cdecl sub_6FB048E0(void)
{
    // Callees: 0x6fae1fa0
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB048EE: imul     eax, eax, 0x550
    //   0x6FB04909: call     eax
    //   0x6FB0491D: imul     eax, eax, 0x550
    //   0x6FB04943: cmp      esi, edx
    //   0x6FB04963: cmp      edx, esi
    //   0x6FB04976: imul     ecx, ecx, 0x550
    //   0x6FB0497E: cmp      edi, dword ptr [ecx + 0x124]
    //   0x6FB04997: call     eax
    //   0x6FB049A1: call     0x6fae1fa0
    //   0x6FB049CE: imul     eax, eax, 0x550
    //   0x6FB049E9: call     eax
    //   0x6FB049FD: imul     eax, eax, 0x550
    //   0x6FB04A21: cmp      edx, esi
    //   0x6FB04A47: cmp      esi, edx
    //   0x6FB04A56: imul     ecx, ecx, 0x550
    //   0x6FB04A5E: cmp      edi, dword ptr [ecx + 0x124]
    //   0x6FB04A77: call     eax
    //   0x6FB04A81: call     0x6fae1fa0
    //   0x6FB04AC4: imul     edx, edx, 0x550
    //   0x6FB04ACC: cmp      dword ptr [eax], -1
    //   0x6FB04ADB: call     eax
    //   0x6FB04AEF: call     0x6fae1fa0
    return 1;
}

// 1.12a: 0x6FB04B10..0x6FB04B81 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB04B10(void)
{
    // Callees: 0x6fae1fa0
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB04B1F: cmp      ecx, edx
    //   0x6FB04B39: imul     eax, eax, 0x550
    //   0x6FB04B41: cmp      dword ptr [eax], -1
    //   0x6FB04B50: call     eax
    //   0x6FB04B64: call     0x6fae1fa0
    return 1;
}

// 1.12a: 0x6FB04B90..0x6FB04D32 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB04B90(D2UnitStrc* pUnit, int32_t a1)
{
    // Callees: 0x6fb03af0, 0x6fb79ffc, 0x6fb79a37, 0x6fb7a6ac, 0x6fae1fa0
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB04BA2: call     0x6fb03af0
    //   0x6FB04BAD: cmp      eax, edi
    //   0x6FB04BC5: imul     eax, eax, 0x550
    //   0x6FB04BDF: call     eax
    //   0x6FB04BF6: imul     edx, edx, 0x550
    //   0x6FB04C14: cmp      dword ptr [esi + 0x53c], 0
    //   0x6FB04C38: cmp      ebp, ecx
    //   0x6FB04C46: cmp      ebp, ecx
    //   0x6FB04C51: cmp      ebp, eax
    //   0x6FB04C6A: cmp      ebp, ecx
    //   0x6FB04CA0: cmp      dword ptr [0x6fbd36f0], 0
    //   0x6FB04CB7: call     0x6fb79ffc
    //   0x6FB04CBC: cmp      dword ptr [0x6fbd36f0], 0
    //   0x6FB04CCC: call     0x6fb79a37
    //   0x6FB04CD8: call     0x6fb7a6ac
    //   0x6FB04CF8: imul     edi, edi, 0x550
    //   0x6FB04D07: cmp      ebx, eax
    //   0x6FB04D18: call     eax
    //   0x6FB04D27: call     0x6fae1fa0
    //   0x6FB04D2F: ret      4
    return 1;
}

// 1.12a: 0x6FB04D40..0x6FB04DEE (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB04D40(void)
{
    // Callees: 0x6fae1fa0
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB04D48: imul     eax, eax, 0x550
    //   0x6FB04D5F: call     eax
    //   0x6FB04D74: imul     ecx, ecx, 0x550
    //   0x6FB04D9C: cmp      eax, edx
    //   0x6FB04DB6: call     eax
    //   0x6FB04DC5: call     0x6fae1fa0
    //   0x6FB04DCB: ret
    //   0x6FB04DD8: call     eax
    //   0x6FB04DE7: call     0x6fae1fa0
    //   0x6FB04DED: ret
    return 1;
}

// 1.12a: 0x6FB04DF0..0x6FB04E5D (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB04DF0(D2UnitStrc* pUnit)
{
    // Referenced strings: 'AutoMapMode', 'Diablo II'
    // Callees: Storm.dll!#423 -> SRegLoadValue, 0x6faee640, 0x6faef1b0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB04DF9: push     0x6fb8535c ; STR:'AutoMapMode'
    //   0x6FB04DFE: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB04E05: call     0x6fabbe90 ; Storm.dll!#423 -> SRegLoadValue
    //   0x6FB04E0E: cmp      eax, 2
    //   0x6FB04E1F: cmp      eax, ecx
    //   0x6FB04E3F: call     0x6faee640
    //   0x6FB04E49: call     0x6faef1b0
    //   0x6FB04E5C: ret
    return 1;
}

// 1.12a: 0x6FB04E60..0x6FB04EC9 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB04E60(D2UnitStrc* pUnit)
{
    // Referenced strings: 'AutoMapMode', 'Diablo II'
    // Callees: 0x6faee640, 0x6faef1b0, Storm.dll!#426 -> SRegSaveValue
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB04E63: cmp      dword ptr [esi + 0x124], 2
    //   0x6FB04E7C: cmp      eax, dword ptr [0x6fbcc2a0]
    //   0x6FB04EA0: call     0x6faee640
    //   0x6FB04EAA: call     0x6faef1b0
    //   0x6FB04EB8: push     0x6fb8535c ; STR:'AutoMapMode'
    //   0x6FB04EBD: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB04EC2: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB04EC8: ret
    return 1;
}

// 1.12a: 0x6FB04ED0..0x6FB04F32 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB04ED0(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Perspective', 'Diablo II'
    // Callees: Storm.dll!#423 -> SRegLoadValue, D2gfx.dll!#10002 -> D2GFX_CheckUnitPerspective, Storm.dll!#426 -> SRegSaveValue, D2gfx.dll!#10069 -> D2GFX_SetPerspective, 0x6fb6a990
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB04EDA: push     0x6fb8537c ; STR:'Perspective'
    //   0x6FB04EDF: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB04EE4: call     0x6fabbe90 ; Storm.dll!#423 -> SRegLoadValue
    //   0x6FB04EE9: call     0x6fabd102 ; D2gfx.dll!#10002 -> D2GFX_CheckUnitPerspective
    //   0x6FB04EF7: push     0x6fb8537c ; STR:'Perspective'
    //   0x6FB04EFC: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB04F01: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB04F09: call     0x6fabd198 ; D2gfx.dll!#10069 -> D2GFX_SetPerspective
    //   0x6FB04F23: call     0x6fb6a990
    //   0x6FB04F29: ret
    //   0x6FB04F31: ret
    return 1;
}

// 1.12a: 0x6FB04F40..0x6FB050D9 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB04F40(void)
{
    // Callees: 0x6fb03650, 0x6fb03580, D2gfx.dll!#10024 -> TEXTURE_CelDraw, 0x6fb02e20, 0x6fb03710, 0x6fb046b0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB04F4A: call     0x6fb03650
    //   0x6FB04F51: call     0x6fb03580
    //   0x6FB04FB2: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FB04FBC: call     0x6fb02e20
    //   0x6FB04FC6: call     0x6fb03710
    //   0x6FB04FD0: call     0x6fb046b0
    //   0x6FB04FE3: ret
    //   0x6FB05034: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FB0503E: call     0x6fb02e20
    //   0x6FB05048: call     0x6fb03710
    //   0x6FB05052: call     0x6fb046b0
    //   0x6FB05065: ret
    //   0x6FB05098: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FB050A2: call     0x6fb02e20
    //   0x6FB050AC: call     0x6fb03710
    //   0x6FB050B6: call     0x6fb046b0
    //   0x6FB050C9: ret
    //   0x6FB050D8: ret
    return 1;
}

// 1.12a: 0x6FB050E0..0x6FB05438 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB050E0(void)
{
    // Callees: Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, 0x6fb04b90, 0x6fb03af0, Fog.dll!#10227 -> FOG_IsExpansion, D2Win.dll!#10005 -> D2Win_10134_DrawCellFile, 0x6fb03830
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB050EC: cmp      ecx, ebp
    //   0x6FB050F7: cmp      dword ptr [0x6fbcc204], ebp
    //   0x6FB05104: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB0510F: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB05119: call     0x6fab326d
    //   0x6FB05121: imul     eax, dword ptr [ecx]
    //   0x6FB0514B: cmp      eax, ecx
    //   0x6FB05155: cmp      dword ptr [0x6fbd3668], eax
    //   0x6FB0515D: cmp      dword ptr [0x6fbcc20c], ebp
    //   0x6FB05171: call     0x6fb04b90
    //   0x6FB0517A: call     0x6fb03af0
    //   0x6FB0517F: cmp      eax, -1
    //   0x6FB0518F: cmp      dword ptr [edx], ebp
    //   0x6FB051B5: cmp      dword ptr [edi + eax + 4], ebp
    //   0x6FB051BF: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB051D8: cmp      eax, ebp
    //   0x6FB051DC: call     eax
    //   0x6FB051EF: cmp      eax, ebp
    //   0x6FB0520D: call     eax
    //   0x6FB05217: imul     esi, dword ptr [esp + 0x10]
    //   0x6FB05231: cmp      eax, 3
    //   0x6FB0525E: call     0x6fabd3b4 ; D2Win.dll!#10005 -> D2Win_10134_DrawCellFile
    //   0x6FB05287: call     0x6fabd3b4 ; D2Win.dll!#10005 -> D2Win_10134_DrawCellFile
    //   0x6FB052BD: call     0x6fabd3b4 ; D2Win.dll!#10005 -> D2Win_10134_DrawCellFile
    //   0x6FB052EF: call     0x6fabd3b4 ; D2Win.dll!#10005 -> D2Win_10134_DrawCellFile
    //   0x6FB0530F: call     0x6fb03830
    //   0x6FB05337: cmp      eax, edx
    //   0x6FB0535F: cmp      eax, ebp
    //   0x6FB05383: imul     eax, eax, 0x550
    //   0x6FB053BA: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FB053E4: call     0x6fabd07e ; D2gfx.dll!#10024 -> TEXTURE_CelDraw
    //   0x6FB053E9: call     dword ptr [0x6fb7efdc] ; IAT:KERNEL32.dll!GetTickCount -> GetTickCount
    //   0x6FB053FA: cmp      ecx, 0x32
    //   0x6FB05424: ret
    return 1;
}

// 1.12a: 0x6FB05440..0x6FB05462 (__stdcall, ret 0x4)
int32_t __stdcall sub_6FB05440(int32_t a1)
{
    // Callees: 0x6fb04d40, Storm.dll!#511 -> SMsgBreakHandlerChain
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05445: call     0x6fb04d40
    //   0x6FB05459: call     0x6fabbe78 ; Storm.dll!#511 -> SMsgBreakHandlerChain
    //   0x6FB0545F: ret      4
    return 1;
}

// 1.12a: 0x6FB05470..0x6FB054ED (__stdcall, ret 0x4)
int32_t __stdcall sub_6FB05470(int32_t a1)
{
    // Callees: 0x6fb03af0, 0x6fb04d40, Storm.dll!#511 -> SMsgBreakHandlerChain
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05482: cmp      eax, edi
    //   0x6FB05486: call     0x6fb03af0
    //   0x6FB0548B: cmp      eax, dword ptr [0x6fbcc1fc]
    //   0x6FB05493: call     0x6fb04d40
    //   0x6FB054E2: call     0x6fabbe78 ; Storm.dll!#511 -> SMsgBreakHandlerChain
    //   0x6FB054EA: ret      4
    return 1;
}

// 1.12a: 0x6FB054F0..0x6FB05536 (__stdcall, ret 0x4)
int32_t __stdcall sub_6FB054F0(int32_t a1)
{
    // Callees: 0x6fb03af0, 0x6fb04b90, Storm.dll!#511 -> SMsgBreakHandlerChain
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB054FF: call     0x6fb03af0
    //   0x6FB05504: cmp      eax, -1
    //   0x6FB05511: call     0x6fb04b90
    //   0x6FB0552B: call     0x6fabbe78 ; Storm.dll!#511 -> SMsgBreakHandlerChain
    //   0x6FB05533: ret      4
    return 1;
}

// 1.12a: 0x6FB05540..0x6FB05545 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB05540(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB05550..0x6FB05555 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB05550(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB05560..0x6FB05606 (__stdcall, ret 0x4)
int32_t __stdcall sub_6FB05560(int32_t a1)
{
    // Callees: Fog.dll!#10227 -> FOG_IsExpansion, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2gfx.dll!#10078 -> WINDOW_GetWindow, 0x6fb1d0a0, 0x6fb3ef00
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0556C: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB05571: cmp      dword ptr [0x6fb8f368], 0x26
    //   0x6FB0557F: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB0558A: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB05594: call     0x6fab326d
    //   0x6FB055AC: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB055B7: call     0x6fb1d0a0
    //   0x6FB055F0: call     0x6fb3ef00
    //   0x6FB055F9: cmp      esi, 0x1c8
    //   0x6FB05603: ret      4
    return 1;
}

// 1.12a: 0x6FB05610..0x6FB05AF8 (__usercall, ret 0x4)
int32_t __usercall sub_6FB05610(D2UnitStrc* pUnit /*eax*/, int32_t a1)
{
    // Callees: Fog.dll!#10227 -> FOG_IsExpansion, 0x6fb3e7a0, 0x6fb3e960, 0x6fb24e20, 0x6fb559a0, D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust, 0x6fb6a990, Fog.dll!#10265 -> UnimplementedOrdinal_10265
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05614: cmp      eax, ebp
    //   0x6FB05622: cmp      dword ptr [edi + 0x6fb948e0], ebp
    //   0x6FB05634: cmp      dword ptr [edi + 0x6fb948e4], ebp
    //   0x6FB0563C: cmp      edi, 0x1c8
    //   0x6FB05655: cmp      edi, 0x1c8
    //   0x6FB05668: cmp      esi, ebp
    //   0x6FB05670: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB05679: cmp      dword ptr [0x6fbb330c], ebp
    //   0x6FB05681: cmp      edi, 0x1a4
    //   0x6FB0568D: cmp      edi, 0x1b0
    //   0x6FB05699: cmp      edi, 0x1bc
    //   0x6FB056AA: call     0x6fb3e7a0
    //   0x6FB056B7: cmp      esi, ebp
    //   0x6FB056C4: call     0x6fb3e960
    //   0x6FB056CC: cmp      eax, 0x24
    //   0x6FB056E3: call     0x6fb24e20
    //   0x6FB056ED: call     0x6fb559a0
    //   0x6FB056F2: cmp      dword ptr [0x6fbb2b60], ebp
    //   0x6FB056FA: cmp      dword ptr [0x6fbb2ba8], ebp
    //   0x6FB05702: cmp      dword ptr [0x6fbb2b94], ebp
    //   0x6FB0570A: cmp      dword ptr [0x6fbb2b98], ebp
    //   0x6FB05712: cmp      dword ptr [0x6fbb2be8], ebp
    //   0x6FB0571A: cmp      dword ptr [0x6fbb2bec], ebp
    //   0x6FB05722: cmp      dword ptr [0x6fbb2bb0], ebp
    //   0x6FB05738: call     0x6fabd17a ; D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust
    //   0x6FB05768: call     0x6fabd17a ; D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust
    //   0x6FB05782: cmp      dword ptr [0x6fbb2b5c], ebp
    //   0x6FB0578A: cmp      dword ptr [0x6fbb2b68], ebp
    //   0x6FB057A0: call     0x6fabd17a ; D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust
    //   0x6FB057CF: call     0x6fabd17a ; D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust
    //   0x6FB057F7: call     0x6fb6a990
    //   0x6FB05800: cmp      edi, 0x1c8
    //   0x6FB05812: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB05817: cmp      dword ptr [0x6fb8f368], 0x26
    //   0x6FB05825: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB05830: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB0583A: call     0x6fab326d
    //   0x6FB05846: call     dword ptr [0x6fb7efdc] ; IAT:KERNEL32.dll!GetTickCount -> GetTickCount
    //   0x6FB0584C: cmp      dword ptr [0x6fbb2b2c], eax
    //   0x6FB0585D: cmp      eax, ebp
    //   0x6FB05865: cmp      dword ptr [eax], ebp
    //   0x6FB05869: cmp      dword ptr [eax + 0x10], 0x11
    //   0x6FB05871: call     0x6facf900
    //   0x6FB05878: call     0x6fb29900
    //   0x6FB05890: call     0x6fb29420
    //   0x6FB058C2: cmp      dword ptr [0x6fbb5490], ebp
    //   0x6FB058D6: cmp      ecx, ebp
    //   0x6FB058E0: cmp      dword ptr [ebx*4 + 0x6fbb2b58], 1
    //   0x6FB058F4: cmp      ecx, 4
    //   0x6FB05904: cmp      dword ptr [0x6fbcb629], ebp
    //   0x6FB05912: call     0x6faca1f0
    //   0x6FB05921: call     0x6fb3ef00
    //   0x6FB05932: cmp      ebx, ecx
    //   0x6FB05936: cmp      eax, ebp
    //   0x6FB0593A: cmp      dword ptr [eax], ebp
    //   0x6FB0593E: call     0x6faceac0
    //   0x6FB05958: cmp      esi, ebp
    //   0x6FB0595C: cmp      eax, ebp
    //   0x6FB05965: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB05970: call     0x6fb1d100
    //   0x6FB05981: cmp      eax, ebp
    //   0x6FB0598A: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB05995: call     0x6fb1d0a0
    //   0x6FB059AA: cmp      esi, 4
    //   0x6FB05A02: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB05A37: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB05A73: cmp      esi, 5
    //   0x6FB05A8B: ret      4
    return 1;
}

// 1.12a: 0x6FB05B00..0x6FB05B0A (__usercall, ret 0x0)
int32_t __usercall sub_6FB05B00(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: 0x6fb05610
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05B04: call     0x6fb05610
    //   0x6FB05B09: ret
    return 1;
}

// 1.12a: 0x6FB05B10..0x6FB05B1A (__usercall, ret 0x0)
int32_t __usercall sub_6FB05B10(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: 0x6fb05610
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05B14: call     0x6fb05610
    //   0x6FB05B19: ret
    return 1;
}

// 1.12a: 0x6FB05B20..0x6FB05C7A (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB05B20(void)
{
    // Callees: 0x6fb05560, Fog.dll!#10227 -> FOG_IsExpansion, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, 0x6faca1f0, 0x6fb3ef00, 0x6faceac0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05B23: call     0x6fb05560
    //   0x6FB05B2E: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB05B33: cmp      dword ptr [0x6fb8f368], 0x26
    //   0x6FB05B41: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB05B4C: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB05B56: call     0x6fab326d
    //   0x6FB05B70: cmp      dword ptr [esi*4 + 0x6fbb2b58], 1
    //   0x6FB05B84: cmp      eax, 4
    //   0x6FB05BA3: call     0x6faca1f0
    //   0x6FB05BB3: call     0x6fb3ef00
    //   0x6FB05BBE: cmp      esi, eax
    //   0x6FB05BCB: cmp      dword ptr [eax], 0
    //   0x6FB05BD0: call     0x6faceac0
    //   0x6FB05BF6: call     0x6fb261d0
    //   0x6FB05BFB: call     0x6fb3af30
    //   0x6FB05C06: call     0x6fb2a520
    //   0x6FB05C0F: call     0x6fb2a570
    //   0x6FB05C1D: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB05C28: call     0x6fb1d0a0
    //   0x6FB05C32: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB05C3D: call     0x6fb1d0a0
    //   0x6FB05C44: call     0x6fb261d0
    //   0x6FB05C58: ret
    //   0x6FB05C74: call     0x6fb05610
    //   0x6FB05C79: ret
    return 1;
}

// 1.12a: 0x6FB05C80..0x6FB05C8A (__usercall, ret 0x0)
int32_t __usercall sub_6FB05C80(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: 0x6fb05610
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05C84: call     0x6fb05610
    //   0x6FB05C89: ret
    return 1;
}

// 1.12a: 0x6FB05C90..0x6FB05C9A (__usercall, ret 0x0)
int32_t __usercall sub_6FB05C90(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: 0x6fb05610
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05C94: call     0x6fb05610
    //   0x6FB05C99: ret
    return 1;
}

// 1.12a: 0x6FB05CA0..0x6FB05CA8 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB05CA0(void)
{
    // Callees: 0x6fb05560
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05CA2: call     0x6fb05560
    //   0x6FB05CA7: ret
    return 1;
}

// 1.12a: 0x6FB05CB0..0x6FB05CDB (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB05CB0(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Callees: IAT:USER32.dll!SendMessageA -> SendMessageA, 0x6fb05560
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05CC3: call     dword ptr [0x6fb7f0f0] ; IAT:USER32.dll!SendMessageA -> SendMessageA
    //   0x6FB05CD5: call     0x6fb05560
    //   0x6FB05CDA: ret
    return 1;
}

// 1.12a: 0x6FB05CE0..0x6FB05CEA (__usercall, ret 0x0)
int32_t __usercall sub_6FB05CE0(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: 0x6fb05610
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05CE4: call     0x6fb05610
    //   0x6FB05CE9: ret
    return 1;
}

// 1.12a: 0x6FB05CF0..0x6FB05D1E (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB05CF0(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Resolution', 'Diablo II'
    // Callees: Storm.dll!#423 -> SRegLoadValue, 0x6fac2630
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05CF9: push     0x6fb85388 ; STR:'Resolution'
    //   0x6FB05CFE: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB05D03: call     0x6fabbe90 ; Storm.dll!#423 -> SRegLoadValue
    //   0x6FB05D16: call     0x6fac2630
    //   0x6FB05D1D: ret
    return 1;
}

// 1.12a: 0x6FB05D20..0x6FB06114 (__usercall, ret 0x0)
int32_t __usercall sub_6FB05D20(D2UnitStrc* pUnit /*eax*/)
{
    // Referenced strings: 'Panel quest log toggle\n'
    // Callees: Fog.dll!#10227 -> FOG_IsExpansion, 0x6fb3b930, D2gfx.dll!#10078 -> WINDOW_GetWindow, 0x6fb1d0a0, 0x6fb3aa10, D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust, 0x6fb6a990, Fog.dll!#10265 -> UnimplementedOrdinal_10265
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB05D20: cmp      eax, 7
    //   0x6FB05D3A: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB05D3F: cmp      esi, ebx
    //   0x6FB05D4C: cmp      eax, 0x26
    //   0x6FB05D55: cmp      esi, ebx
    //   0x6FB05D5F: call     0x6fb3b930
    //   0x6FB05D69: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB05D74: call     0x6fb1d0a0
    //   0x6FB05D79: call     0x6fb3aa10
    //   0x6FB05D7E: cmp      dword ptr [0x6fbb2b5c], ebx
    //   0x6FB05D86: cmp      dword ptr [0x6fbb2b68], ebx
    //   0x6FB05D9C: call     0x6fabd17a ; D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust
    //   0x6FB05DB5: call     0x6fb6a990
    //   0x6FB05DBC: ret
    //   0x6FB05DDE: call     0x6fabd17a ; D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust
    //   0x6FB05E06: call     0x6fb6a990
    //   0x6FB05E0D: ret
    //   0x6FB05E0E: cmp      eax, 0x26
    //   0x6FB05E18: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB05E23: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB05E2D: call     0x6fab326d
    //   0x6FB05E38: cmp      esi, ebx
    //   0x6FB05E3D: cmp      dword ptr [0x6fbb5490], ebx
    //   0x6FB05E50: cmp      dword ptr [esi*4 + 0x6fbb2b58], 1
    //   0x6FB05E64: cmp      eax, 4
    //   0x6FB05E74: cmp      dword ptr [0x6fbcb629], ebx
    //   0x6FB05E82: call     0x6faca1f0
    //   0x6FB05E91: call     0x6fb3ef00
    //   0x6FB05E9C: cmp      esi, eax
    //   0x6FB05EAB: cmp      eax, ebx
    //   0x6FB05EAF: cmp      dword ptr [eax], ebx
    //   0x6FB05EB3: call     0x6faceac0
    //   0x6FB05EBE: cmp      esi, ebx
    //   0x6FB05EC9: cmp      esi, ebx
    //   0x6FB05ED6: cmp      esi, ebx
    //   0x6FB05EDF: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB05EEA: call     0x6fb1d100
    //   0x6FB05EEF: cmp      dword ptr [0x6fbb2b5c], ebx
    //   0x6FB05EFB: cmp      dword ptr [0x6fbb2b68], ebx
    //   0x6FB05F07: cmp      esi, ebx
    //   0x6FB05F2A: call     0x6fabd17a ; D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust
    //   0x6FB05F50: call     0x6fb6a990
    //   0x6FB05F58: ret
    //   0x6FB05F67: call     0x6fabd17a ; D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust
    //   0x6FB05F80: call     0x6fb6a990
    //   0x6FB05F88: ret
    //   0x6FB05F89: cmp      esi, ebx
    //   0x6FB05F9F: call     0x6fabd17a ; D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust
    //   0x6FB05FB8: call     0x6fb6a990
    //   0x6FB05FD8: ret
    //   0x6FB05FFA: call     0x6fabd17a ; D2gfx.dll!#10072 -> D2GFX_SetViewMatrixAdjust
    //   0x6FB06025: cmp      eax, ebx
    //   0x6FB0602F: call     0x6fb3ef00
    //   0x6FB06035: ret
    //   0x6FB0603B: call     0x6fb3ef00
    //   0x6FB06041: ret
    //   0x6FB06055: cmp      eax, ebx
    //   0x6FB0607C: cmp      eax, ebx
    //   0x6FB0608B: call     0x6fb3ef00
    //   0x6FB0609C: push     0x6fb852d8 ; STR:'Panel quest log toggle\n'
    //   0x6FB060A1: call     dword ptr [0x6fb7ef8c] ; IAT:KERNEL32.dll!OutputDebugStringA -> OutputDebugStringA
    //   0x6FB060A9: call     0x6fb71fd0
    //   0x6FB060AF: ret
    //   0x6FB060B4: call     0x6fb3b480
    //   0x6FB060CB: call     0x6fb05610
    //   0x6FB060F0: imul     esp, dword ptr [eax - 0x50], 0xb060756f
    return 1;
}

// 1.12a: 0x6FB06120..0x6FB0614E (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB06120(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Resolution', 'Diablo II'
    // Callees: Storm.dll!#423 -> SRegLoadValue, 0x6fac2630
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0612B: push     0x6fb85388 ; STR:'Resolution'
    //   0x6FB06130: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB06135: call     0x6fabbe90 ; Storm.dll!#423 -> SRegLoadValue
    //   0x6FB06146: call     0x6fac2630
    //   0x6FB0614D: ret
    return 1;
}

// 1.12a: 0x6FB06150..0x6FB06184 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB06150(D2UnitStrc* pUnit)
{
    // Referenced strings: 'Resolution', 'Diablo II'
    // Callees: Storm.dll!#426 -> SRegSaveValue, 0x6fac2630
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0615D: push     0x6fb85388 ; STR:'Resolution'
    //   0x6FB06162: push     0x6fb803fc ; STR:'Diablo II'
    //   0x6FB06167: call     0x6fabbe72 ; Storm.dll!#426 -> SRegSaveValue
    //   0x6FB0617C: call     0x6fac2630
    //   0x6FB06183: ret
    return 1;
}

// 1.12a: 0x6FB06190..0x6FB06356 (__stdcall, ret 0x4)
// 1.12a: 0x6FB06350 (internal entry point sub_6FB06350)
int32_t __stdcall sub_6FB06190(int32_t a1)
{
    // Callees: D2Common.dll!#10078 -> INVENTORY_GetCursorItem, 0x6fb30ad0, 0x6fb05d20
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB061A1: cmp      ecx, edi
    //   0x6FB061A9: cmp      eax, edi
    //   0x6FB061BA: cmp      eax, edi
    //   0x6FB061C3: call     0x6fabc1e4 ; D2Common.dll!#10078 -> INVENTORY_GetCursorItem
    //   0x6FB061DB: cmp      ecx, eax
    //   0x6FB061FF: cmp      dword ptr [0x6fbcc23c], edi
    //   0x6FB06216: cmp      dword ptr [0x6fbcc234], edi
    //   0x6FB06239: cmp      ecx, edi
    //   0x6FB0624C: cmp      dword ptr [0x6fbcbf78], edi
    //   0x6FB06254: call     0x6fb30ad0
    //   0x6FB06259: cmp      eax, 1
    //   0x6FB06263: cmp      dword ptr [0x6fbcc22c], edi
    //   0x6FB0626E: cmp      dword ptr [0x6fbcc230], edi
    //   0x6FB0628C: cmp      edi, ecx
    //   0x6FB06297: cmp      edi, eax
    //   0x6FB062A7: cmp      ebx, edx
    //   0x6FB062B2: cmp      ebx, eax
    //   0x6FB062BE: cmp      ebx, ecx
    //   0x6FB062C9: cmp      edi, eax
    //   0x6FB062D0: cmp      edi, eax
    //   0x6FB062DB: cmp      eax, 7
    //   0x6FB062E2: cmp      ebp, ecx
    //   0x6FB062E6: cmp      dword ptr [ebp], ecx
    //   0x6FB062EB: cmp      dword ptr [ebp + 0x10], 0x11
    //   0x6FB062F1: call     0x6fb05d20
    //   0x6FB062F9: cmp      esi, ebx
    //   0x6FB0634D: ret      4
    //   0x6FB06355: ret
    return 1;
}

// 1.12a: 0x6FB06360..0x6FB06366 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB06360(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB06365: ret
    return 1;
}

// 1.12a: 0x6FB06370..0x6FB06386 (__fastcall, ret 0x0)
// 1.12a: 0x6FB06380 (internal entry point sub_6FB06380)
int32_t __fastcall sub_6FB06370(D2UnitStrc* pUnit)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB06379: ret
    //   0x6FB0637F: ret
    //   0x6FB06385: ret
    return 1;
}

// 1.12a: 0x6FB06390..0x6FB063D2 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB06390(void)
{
    // Callees: IAT:D2Lang.dll!?win2Unicode@Unicode@@SIPAU1@PAU1@PBDH@Z -> ?win2Unicode@Unicode@@SIPAU1@PAU1@PBDH@Z
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB063C0: call     dword ptr [0x6fb7eb00] ; IAT:D2Lang.dll!?win2Unicode@Unicode@@SIPAU1@PAU1@PBDH@Z -> ?win2Unicode@Unicode@@SIPAU1@PAU1@PBDH@Z
    //   0x6FB063D1: ret
    return 1;
}

// 1.12a: 0x6FB063E0..0x6FB063F8 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB063E0(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB063E5: cmp      eax, 1
    //   0x6FB063EA: cmp      eax, 2
    //   0x6FB063F1: ret
    //   0x6FB063F7: ret
    return 1;
}

// 1.12a: 0x6FB06400..0x6FB0640E (__usercall, ret 0x0)
int32_t __usercall sub_6FB06400(D2UnitStrc* pUnit /*eax*/)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0640D: ret
    return 1;
}

// 1.12a: 0x6FB06410..0x6FB06424 (__usercall, ret 0x0)
int32_t __usercall sub_6FB06410(D2UnitStrc* pUnit /*eax*/)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB06423: ret
    return 1;
}

// 1.12a: 0x6FB06430..0x6FB0643D (__usercall, ret 0x0)
int32_t __usercall sub_6FB06430(D2UnitStrc* pUnit /*eax*/)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0643C: ret
    return 1;
}

// 1.12a: 0x6FB06440..0x6FB06478 (__usercall, ret 0x0)
int32_t __usercall sub_6FB06440(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: 0x6fb291a0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB06464: call     0x6fb291a0
    //   0x6FB06477: ret
    return 1;
}

// 1.12a: 0x6FB06480..0x6FB064CB (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB06480(void)
{
    // Callees: 0x6fb291a0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB064B7: call     0x6fb291a0
    //   0x6FB064CA: ret
    return 1;
}

// 1.12a: 0x6FB064D0..0x6FB065B1 (__usercall, ret 0x0)
// 1.12a: 0x6FB06520 (internal entry point sub_6FB06520)
int32_t __usercall sub_6FB064D0(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: 0x6fb291a0, IAT:KERNEL32.dll!GetTickCount -> GetTickCount, D2Common.dll!#10658 -> STATLIST_UnitGetStatValue, 0x6fb29870
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB064F6: call     0x6fb291a0
    //   0x6FB064FB: cmp      dword ptr [0x6fbcc1f4], ebx
    //   0x6FB0650A: call     dword ptr [0x6fb7efdc] ; IAT:KERNEL32.dll!GetTickCount -> GetTickCount
    //   0x6FB0651F: ret
    //   0x6FB06528: cmp      eax, 4
    //   0x6FB0652D: cmp      eax, 5
    //   0x6FB06532: cmp      eax, 6
    //   0x6FB06541: call     0x6fabc352 ; D2Common.dll!#10658 -> STATLIST_UnitGetStatValue
    //   0x6FB06546: cmp      esi, eax
    //   0x6FB0655C: call     0x6fb29870
    //   0x6FB06564: ret
    //   0x6FB06573: call     0x6fb29870
    //   0x6FB0657B: ret
    //   0x6FB0659D: call     0x6fb291a0
    //   0x6FB065B0: ret
    return 1;
}

// 1.12a: 0x6FB065C0..0x6FB06629 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB065C0(void)
{
    // Callees: 0x6fae1fa0, 0x6fb13290, IAT:KERNEL32.dll!GetTickCount -> GetTickCount, 0x6fb291a0
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB065D1: call     0x6fae1fa0
    //   0x6FB065D6: call     0x6fb13290
    //   0x6FB065F0: call     dword ptr [0x6fb7efdc] ; IAT:KERNEL32.dll!GetTickCount -> GetTickCount
    //   0x6FB0661A: call     0x6fb291a0
    //   0x6FB06628: ret
    return 1;
}

// 1.12a: 0x6FB06630..0x6FB06699 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB06630(void)
{
    // Callees: 0x6fae1fa0, 0x6fb13290, 0x6fb291a0, IAT:KERNEL32.dll!GetTickCount -> GetTickCount
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB06641: call     0x6fae1fa0
    //   0x6FB06646: call     0x6fb13290
    //   0x6FB0667F: call     0x6fb291a0
    //   0x6FB06684: call     dword ptr [0x6fb7efdc] ; IAT:KERNEL32.dll!GetTickCount -> GetTickCount
    //   0x6FB06698: ret
    return 1;
}

// 1.12a: 0x6FB066A0..0x6FB067DE (__stdcall, ret 0x4)
// 1.12a: 0x6FB06700 (internal entry point sub_6FB06700)
int32_t __stdcall sub_6FB066A0(int32_t a1)
{
    // Callees: 0x6fae1fa0, 0x6fb13290, 0x6fb291a0, 0x6fb3ef00, 0x6fb56e20
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB066B1: call     0x6fae1fa0
    //   0x6FB066B6: call     0x6fb13290
    //   0x6FB066F1: call     0x6fb291a0
    //   0x6FB066FF: ret
    //   0x6FB0670B: cmp      eax, ebx
    //   0x6FB06715: call     0x6fb13290
    //   0x6FB06720: cmp      dword ptr [0x6fbc26d4], ebx
    //   0x6FB06728: call     0x6fb13290
    //   0x6FB06733: cmp      dword ptr [0x6fbcc1f4], 7
    //   0x6FB06740: cmp      dword ptr [0x6fbb2bb4], ebx
    //   0x6FB06753: call     0x6fb3ef00
    //   0x6FB0675E: call     0x6fb56e20
    //   0x6FB06768: cmp      eax, ebx
    //   0x6FB0676C: cmp      dword ptr [eax + 0x10], 0x11
    //   0x6FB06772: cmp      dword ptr [esp + 0x10], ebx
    //   0x6FB06783: call     0x6fb3ef00
    //   0x6FB06788: cmp      dword ptr [0x6fbb2b6c], ebx
    //   0x6FB0679B: call     0x6fb3ef00
    //   0x6FB067A0: cmp      dword ptr [0x6fbcc1f4], ebx
    //   0x6FB067C8: call     0x6fb291a0
    //   0x6FB067DB: ret      4
    return 1;
}

// 1.12a: 0x6FB067E0..0x6FB067E8 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB067E0(void)
{
    // Callees: 0x6fb06700
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB067E2: call     0x6fb06700
    //   0x6FB067E7: ret
    return 1;
}

// 1.12a: 0x6FB067F0..0x6FB0685A (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB067F0(void)
{
    // Callees: IAT:KERNEL32.dll!GetTickCount -> GetTickCount, 0x6fb564b0, 0x6fb06700, 0x6fb29420
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB067F0: cmp      dword ptr [0x6fbcc1f4], 7
    //   0x6FB067F9: cmp      dword ptr [0x6fbc26e0], -1
    //   0x6FB06802: call     dword ptr [0x6fb7efdc] ; IAT:KERNEL32.dll!GetTickCount -> GetTickCount
    //   0x6FB06814: cmp      eax, ecx
    //   0x6FB06818: call     0x6fb564b0
    //   0x6FB06829: call     0x6fb06700
    //   0x6FB0682E: call     0x6fb29420
    //   0x6FB06856: ret
    //   0x6FB06859: ret
    return 1;
}

// 1.12a: 0x6FB06860..0x6FB06932 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB06860(void)
{
    // Callees: 0x6fb13600, D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex, 0x6fb12df0, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB06892: call     0x6fb13600
    //   0x6FB068BD: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FB068CB: call     0x6fb12df0
    //   0x6FB068E2: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FB068F0: call     0x6fb12df0
    //   0x6FB06905: cmp      dword ptr [eax + 0x50], -1
    //   0x6FB06910: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB0691B: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB06925: call     0x6fab326d
    //   0x6FB06931: ret
    return 1;
}

// 1.12a: 0x6FB06940..0x6FB06A38 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB06940(void)
{
    // Callees: 0x6fb13600, D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex, 0x6fb12df0, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB0694B: ret
    //   0x6FB0696B: call     0x6fb13600
    //   0x6FB06999: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FB069A7: call     0x6fb12df0
    //   0x6FB069BE: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FB069CC: call     0x6fb12df0
    //   0x6FB069E3: call     0x6fabd1d4 ; D2Lang.dll!#10005 -> D2LANG_GetStringFromTblIndex
    //   0x6FB069F1: call     0x6fb12df0
    //   0x6FB06A06: cmp      dword ptr [eax + 0x50], -1
    //   0x6FB06A11: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB06A1C: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB06A26: call     0x6fab326d
    //   0x6FB06A37: ret
    return 1;
}

// 1.12a: 0x6FB06A40..0x6FB0744C (__usercall, ret 0x0)
// 1.12a: 0x6FB06C20 (internal entry point sub_6FB06C20)
// 1.12a: 0x6FB06D00 (internal entry point sub_6FB06D00)
int32_t __usercall sub_6FB06A40(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: 0x6fb06700, Fog.dll!#10227 -> FOG_IsExpansion, 0x6faca1f0, 0x6fb3ef00, 0x6faceac0, D2gfx.dll!#10078 -> WINDOW_GetWindow, 0x6fb1d0a0, 0x6fb1d100
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB06A44: cmp      eax, 0x15
    //   0x6FB06A7C: call     0x6fb06700
    //   0x6FB06A85: ret
    //   0x6FB06A92: call     0x6fb06700
    //   0x6FB06A9B: ret
    //   0x6FB06AA3: cmp      eax, ebp
    //   0x6FB06AAB: cmp      dword ptr [0x6fbb2bb4], ebp
    //   0x6FB06AB7: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB06ABC: cmp      dword ptr [0x6fb8f368], 0x26
    //   0x6FB06AC9: cmp      dword ptr [0x6fbb5490], ebp
    //   0x6FB06AEC: cmp      eax, ebx
    //   0x6FB06AFA: cmp      eax, 4
    //   0x6FB06B0A: cmp      dword ptr [0x6fbcb629], ebp
    //   0x6FB06B18: call     0x6faca1f0
    //   0x6FB06B24: call     0x6fb3ef00
    //   0x6FB06B2F: cmp      esi, eax
    //   0x6FB06B38: cmp      eax, ebp
    //   0x6FB06B3C: cmp      dword ptr [eax], ebp
    //   0x6FB06B40: call     0x6faceac0
    //   0x6FB06B51: cmp      dword ptr [0x6fbb2bb4], ebp
    //   0x6FB06B5D: cmp      dword ptr [0x6fbd33fc], ebx
    //   0x6FB06B76: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB06B81: call     0x6fb1d0a0
    //   0x6FB06B86: cmp      dword ptr [0x6fbd33f8], ebp
    //   0x6FB06B93: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB06B9E: call     0x6fb1d100
    //   0x6FB06BC6: call     0x6fb3c400
    //   0x6FB06BD0: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB06BDB: call     0x6fb1d100
    //   0x6FB06BFD: call     0x6fb06860
    //   0x6FB06C03: call     0x6fb5b100
    //   0x6FB06C0C: ret
    //   0x6FB06C16: call     0x6fae7250
    //   0x6FB06C1F: ret
    //   0x6FB06C34: cmp      eax, ebp
    //   0x6FB06C40: cmp      dword ptr [eax + 0xc], ecx
    //   0x6FB06C4B: cmp      eax, ebp
    //   0x6FB06C53: ret
    //   0x6FB06C54: cmp      dword ptr [eax], ebp
    //   0x6FB06C5F: call     0x6fae7250
    //   0x6FB06C68: ret
    //   0x6FB06C70: cmp      eax, ebp
    //   0x6FB06C82: call     0x6fabc1e4 ; D2Common.dll!#10078 -> INVENTORY_GetCursorItem
    //   0x6FB06C8B: cmp      dword ptr [0x6fbcc194], ebp
    //   0x6FB06C93: call     0x6fb06940
    //   0x6FB06C9C: cmp      dword ptr [0x6fbb2b6c], ebp
    //   0x6FB06CAE: call     0x6fb29870
    //   0x6FB06CB7: ret
    //   0x6FB06CB8: cmp      dword ptr [0x6fbb2bb4], ebp
    //   0x6FB06CC4: call     0x6fabbf92 ; Fog.dll!#10227 -> FOG_IsExpansion
    //   0x6FB06CC9: cmp      dword ptr [0x6fb8f368], 0x26
    //   0x6FB06CD7: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB06CE2: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB06CEC: call     0x6fab326d
    //   0x6FB06CF1: cmp      dword ptr [0x6fbb5490], ebp
    //   0x6FB06D0C: cmp      eax, ebx
    //   0x6FB06D1A: cmp      eax, 4
    //   0x6FB06D2A: cmp      dword ptr [0x6fbcb629], ebp
    //   0x6FB06D38: call     0x6faca1f0
    //   0x6FB06D44: call     0x6fb3ef00
    //   0x6FB06D4F: cmp      esi, eax
    //   0x6FB06D58: cmp      eax, ebp
    //   0x6FB06D5C: cmp      dword ptr [eax], ebp
    //   0x6FB06D60: call     0x6faceac0
    //   0x6FB06D71: cmp      dword ptr [0x6fbb2bb4], ebp
    //   0x6FB06D7D: cmp      dword ptr [0x6fbd33fc], ebx
    //   0x6FB06D96: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB06DA1: call     0x6fb1d0a0
    //   0x6FB06DA6: cmp      dword ptr [0x6fbd33f8], ebp
    //   0x6FB06DB3: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB06DBE: call     0x6fb1d100
    //   0x6FB06DE6: call     0x6fb3c400
    //   0x6FB06DF0: call     0x6fabd0ea ; D2gfx.dll!#10078 -> WINDOW_GetWindow
    //   0x6FB06DFB: call     0x6fb1d100
    //   0x6FB06E15: call     0x6fb5b100
    //   0x6FB06E1E: ret
    //   0x6FB06E26: cmp      eax, ebp
    //   0x6FB06E2A: cmp      dword ptr [0x6fbcbc28], ebp
    //   0x6FB06E3B: call     0x6fae1fa0
    //   0x6FB06E4D: call     0x6fb06700
    //   0x6FB06E56: ret
    //   0x6FB06E5E: cmp      eax, ebp
    //   0x6FB06E72: call     0x6fb5b100
    //   0x6FB06E7F: cmp      eax, 4
    //   0x6FB06E8F: cmp      dword ptr [0x6fbcbc24], ebp
    //   0x6FB06EA1: cmp      dword ptr [0x6fbcbc28], ebp
    //   0x6FB06EB7: call     0x6fae1fa0
    //   0x6FB06ED0: ret
    //   0x6FB06ED1: cmp      dword ptr [0x6fbcbc24], ebp
    //   0x6FB06EE3: cmp      dword ptr [0x6fbcbc28], ebp
    //   0x6FB06EF4: call     0x6fae1fa0
    //   0x6FB06F0C: ret
    //   0x6FB06F0D: cmp      dword ptr [0x6fbcbc24], ebp
    //   0x6FB06F15: cmp      dword ptr [0x6fbcbc28], ebp
    //   0x6FB06F2B: call     0x6fae1fa0
    //   0x6FB06F44: ret
    //   0x6FB06F4C: cmp      eax, ebp
    //   0x6FB06F6B: call     0x6fb29870
    //   0x6FB06F7C: call     0x6fb5b100
    //   0x6FB06F87: cmp      eax, 6
    //   0x6FB06F97: cmp      dword ptr [0x6fbcbc24], ebp
    //   0x6FB06FAA: cmp      dword ptr [0x6fbcbc28], ebp
    //   0x6FB06FB4: cmp      dword ptr [0x6fbcbc24], ebp
    //   0x6FB06FBC: cmp      dword ptr [0x6fbcbc28], ebp
    //   0x6FB06FCD: call     0x6fae1fa0
    //   0x6FB06FE2: ret
    //   0x6FB06FE3: cmp      dword ptr [0x6fbcbc24], ebp
    //   0x6FB06FF6: cmp      dword ptr [0x6fbcbc28], ebp
    //   0x6FB07002: call     0x6fae1fa0
    //   0x6FB07007: cmp      dword ptr [0x6fbc26dc], ebp
    //   0x6FB07021: call     0x6fb13290
    //   0x6FB07030: ret
    //   0x6FB07031: cmp      dword ptr [0x6fbcbc24], ebp
    //   0x6FB0703E: cmp      dword ptr [0x6fbcbc28], ebp
    //   0x6FB0704A: call     0x6fae1fa0
    //   0x6FB07065: ret
    //   0x6FB0707D: ret
    //   0x6FB0708C: ret
    //   0x6FB07094: imul     ebp, dword ptr [eax + esi*4 + 0x6f], 0x6fb06e1f
    //   0x6FB070ED: imul     esi, dword ptr [eax - 0x4f8f9991], 0x6f
    return 1;
}

// 1.12a: 0x6FB07450..0x6FB0746C (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB07450(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB07469: ret      4
    return 1;
}

// 1.12a: 0x6FB07470..0x6FB074D4 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB07470(D2UnitStrc* pUnit)
{
    // Callees: D2Common.dll!#10646 -> SKILLS_GetSkillById, D2Common.dll!#10847 -> SKILLS_GetOwnerGUIDFromSkill, D2Common.dll!#10146 -> SKILLS_GetSkillInfo
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB07478: cmp      edx, -1
    //   0x6FB0748A: ret
    //   0x6FB07494: call     0x6fabc66a ; D2Common.dll!#10646 -> SKILLS_GetSkillById
    //   0x6FB074A3: ret
    //   0x6FB074A5: call     0x6fabc8c2 ; D2Common.dll!#10847 -> SKILLS_GetOwnerGUIDFromSkill
    //   0x6FB074AA: cmp      eax, -1
    //   0x6FB074BB: call     0x6fabc9be ; D2Common.dll!#10146 -> SKILLS_GetSkillInfo
    //   0x6FB074D3: ret
    return 1;
}

// ============================================================================
// SECTION: SKILLS_SkillsPal.asm — Paladin Client Skill Start/Do Functions (SKILLS/SkillsPal.cpp)
// ============================================================================

// 1.12a: 0x6FB373F0..0x6FB3741A (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB373F0(void)
{
    // Callees: D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId, D2Common.dll!#10619 -> SKILLS_GetFlags, D2Common.dll!#10140 -> SKILLS_SetFlags, D2Common.dll!#10059 -> STATES_ToggleState
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB373F3: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB373FB: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB37409: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB37413: call     0x6fabc26e ; D2Common.dll!#10059 -> STATES_ToggleState
    //   0x6FB37419: ret
    return 1;
}

// 1.12a: 0x6FB37420..0x6FB37455 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB37420(void)
{
    // Callees: D2Common.dll!#10361 -> DATATBLS_GetMonSeqTxtRecordFromUnit, D2Common.dll!#11052 -> DATATBLS_GetSeqFrameCount
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB37422: call     0x6fabc970 ; D2Common.dll!#10361 -> DATATBLS_GetMonSeqTxtRecordFromUnit
    //   0x6FB3742E: call     0x6fabc97c ; D2Common.dll!#11052 -> DATATBLS_GetSeqFrameCount
    //   0x6FB37445: cmp      ecx, eax
    //   0x6FB3744F: ret
    //   0x6FB37454: ret
    return 1;
}

// 1.12a: 0x6FB37460..0x6FB3764C (__stdcall, ret 0x8)
int32_t __stdcall sub_6FB37460(int32_t a1, int32_t a2)
{
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10732 -> UNITS_GetPrecisionY, D2Common.dll!#10467 -> UNITS_GetClientCoordY, D2Common.dll!#11138 -> PATH_GetClientCoordsVelocity, D2gfx.dll!#10071 -> D2GFX_CheckPerspective, D2Common.dll!#10368 -> SKILLS_GetParam3, D2Common.dll!#10098 -> UNITS_GetClientCoordX, D2Common.dll!#10984 -> UNITS_GetPrecisionX
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB37465: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB3746C: cmp      eax, ebp
    //   0x6FB3747A: call     0x6fabc208 ; D2Common.dll!#10732 -> UNITS_GetPrecisionY
    //   0x6FB37484: call     0x6fabc214 ; D2Common.dll!#10467 -> UNITS_GetClientCoordY
    //   0x6FB374A0: call     0x6fabc964 ; D2Common.dll!#11138 -> PATH_GetClientCoordsVelocity
    //   0x6FB374A5: cmp      dword ptr [esp + 0x4c], ebp
    //   0x6FB374AF: cmp      edi, ebp
    //   0x6FB374B9: cmp      edi, ebp
    //   0x6FB374C6: cmp      ebx, ebp
    //   0x6FB374CE: cmp      edi, ebp
    //   0x6FB374D6: cmp      ecx, eax
    //   0x6FB374E9: cmp      ecx, ebp
    //   0x6FB374F4: imul     eax, eax, 0xf
    //   0x6FB374FE: imul     eax, eax, 0xf
    //   0x6FB3750A: call     0x6fabd090 ; D2gfx.dll!#10071 -> D2GFX_CheckPerspective
    //   0x6FB37526: call     0x6fabc31c ; D2Common.dll!#10368 -> SKILLS_GetParam3
    //   0x6FB37541: imul     ebx, edi
    //   0x6FB3754E: imul     edi, ebp
    //   0x6FB37586: cmp      dword ptr [esp + 0x2c], eax
    //   0x6FB3759A: call     0x6fabc232 ; D2Common.dll!#10098 -> UNITS_GetClientCoordX
    //   0x6FB375A1: call     0x6fabc202 ; D2Common.dll!#10984 -> UNITS_GetPrecisionX
    //   0x6FB375A7: call     0x6fabd0a8 ; D2gfx.dll!#10052 -> SCALE_ScalePerspectivePosition
    //   0x6FB37601: call     0x6fb449c0
    //   0x6FB37649: ret      8
    return 1;
}

// 1.12a: 0x6FB37650..0x6FB3779A (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB37650(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[34]
    // Callees: 0x6facf5b0, D2Common.dll!#11084 -> PATH_GetVelocity, D2Common.dll!#11128 -> UNITS_GetDirection, 0x6fab1020, 0x6fab1590, 0x6fab15c0, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fad1e80
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB37665: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB3766D: cmp      eax, dword ptr [ecx + 0xba0]
    //   0x6FB3767F: imul     eax, eax, 0x23c
    //   0x6FB376A4: cmp      eax, dword ptr [ecx + 0xb6c]
    //   0x6FB376BB: call     0x6facf5b0
    //   0x6FB376D4: call     0x6fabc976 ; D2Common.dll!#11084 -> PATH_GetVelocity
    //   0x6FB376DE: call     0x6fabc298 ; D2Common.dll!#11128 -> UNITS_GetDirection
    //   0x6FB376F3: call     0x6fab1020
    //   0x6FB37704: call     0x6fab1020
    //   0x6FB37722: call     0x6fab1590
    //   0x6FB37734: call     0x6fab15c0
    //   0x6FB3775A: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB37766: call     0x6fab1020
    //   0x6FB3777C: call     0x6fad1e80
    //   0x6FB3778D: ret      4
    //   0x6FB37797: ret      4
    return 1;
}

// 1.12a: 0x6FB377A0..0x6FB377F7 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB377A0(D2UnitStrc* pUnit)
{
    // Callees: 0x6fafe9b0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB377E3: call     0x6fafe9b0
    //   0x6FB377F6: ret
    return 1;
}

// 1.12a: 0x6FB37800..0x6FB37A7D (__fastcall, ret 0x4)
// 1.12a: 0x6FB379C0 (internal entry point sub_6FB379C0)
int32_t __fastcall sub_6FB37800(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[37]
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10619 -> SKILLS_GetFlags, D2Common.dll!#10361 -> DATATBLS_GetMonSeqTxtRecordFromUnit, D2Common.dll!#11052 -> DATATBLS_GetSeqFrameCount, 0x6fae87b0, D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10089 -> SKILLS_GetParam2, 0x6face550
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    SKILLS_SetParam1(pSkill, 0);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB37808: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB37818: ret      4
    //   0x6FB3781D: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB3782C: call     0x6fabc970 ; D2Common.dll!#10361 -> DATATBLS_GetMonSeqTxtRecordFromUnit
    //   0x6FB37836: call     0x6fabc97c ; D2Common.dll!#11052 -> DATATBLS_GetSeqFrameCount
    //   0x6FB3783F: call     0x6fabc970 ; D2Common.dll!#10361 -> DATATBLS_GetMonSeqTxtRecordFromUnit
    //   0x6FB3784B: call     0x6fabc97c ; D2Common.dll!#11052 -> DATATBLS_GetSeqFrameCount
    //   0x6FB37865: cmp      ecx, eax
    //   0x6FB3787B: cmp      byte ptr [esi + 0x4e], 3
    //   0x6FB37889: call     0x6fae87b0
    //   0x6FB37897: ret      4
    //   0x6FB378A1: cmp      eax, 1
    //   0x6FB378A9: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB378B3: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FB378BC: cmp      edx, 6
    //   0x6FB378D4: call     0x6face550
    //   0x6FB378F2: call     0x6fabc724 ; D2Common.dll!#10974 -> UNITS_IsInMeleeRange
    //   0x6FB378FE: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB3790A: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB37921: ret      4
    //   0x6FB37937: call     0x6fb377a0
    //   0x6FB37946: call     0x6fabc724 ; D2Common.dll!#10974 -> UNITS_IsInMeleeRange
    //   0x6FB37952: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB3795B: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB37965: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FB37976: ret      4
    //   0x6FB3797C: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB37985: cmp      dword ptr [esi], 1
    //   0x6FB3798C: call     0x6fab19a0
    //   0x6FB37991: call     0x6fab19e0
    //   0x6FB379BD: ret      4
    //   0x6FB379CA: cmp      ecx, eax
    //   0x6FB379D1: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB379DF: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB379F4: call     0x6fabc31c ; D2Common.dll!#10368 -> SKILLS_GetParam3
    //   0x6FB379F9: cmp      eax, 2
    //   0x6FB37A01: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FB37A12: ret      4
    //   0x6FB37A18: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB37A20: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB37A28: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FB37A44: call     0x6fb377a0
    //   0x6FB37A57: call     0x6fab15c0
    //   0x6FB37A5F: call     0x6fab1590
    //   0x6FB37A69: call     0x6fabc6dc ; D2Common.dll!#10756 -> D2Common_10216
    //   0x6FB37A7A: ret      4
    return 1;
}

// 1.12a: 0x6FB37A80..0x6FB37C4A (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB37A80(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[86]
    // Callees: D2Common.dll!#10366 -> UNITS_GetRoom, D2Common.dll!#10438 -> DUNGEON_IsRoomInTown, 0x6fb00830, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fab1020, 0x6fab1590, 0x6fab15c0, 0x6fab1070
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB37A8A: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB37A98: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB37AAA: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB37AB1: cmp      esi, dword ptr [eax + 0xba0]
    //   0x6FB37AC1: imul     edi, edi, 0x23c
    //   0x6FB37AD3: ret      4
    //   0x6FB37AD9: call     0x6fb00830
    //   0x6FB37AE6: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB37AEE: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB37B05: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB37B19: ret      4
    //   0x6FB37B24: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB37B33: imul     edi, ebp
    //   0x6FB37B46: call     0x6fab1020
    //   0x6FB37B53: imul     edx, eax
    //   0x6FB37B67: call     0x6fab1590
    //   0x6FB37B72: call     0x6fab15c0
    //   0x6FB37BCA: call     0x6fab1070
    //   0x6FB37BD5: call     0x6fab1070
    //   0x6FB37BE0: imul     ecx, eax
    //   0x6FB37BE3: imul     edx, edi
    //   0x6FB37BE8: cmp      ecx, dword ptr [esp + 0x1c]
    //   0x6FB37C0A: call     0x6fad4b20
    //   0x6FB37C1C: call     0x6fab1020
    //   0x6FB37C26: call     0x6fabc2da ; D2Common.dll!#10155 -> PATH_SetDirection
    //   0x6FB37C3D: ret      4
    //   0x6FB37C47: ret      4
    return 1;
}

// 1.12a: 0x6FB37C50..0x6FB37D53 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB37C50(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[36]
    // Callees: 0x6fb00830, 0x6facf5b0, 0x6fab1590, 0x6fab15c0, 0x6fad4b20, D2Common.dll!#11161 -> MISSILE_SetTargetX, D2Common.dll!#10792 -> MISSILE_SetTargetY
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB37C61: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB37C68: cmp      ebp, dword ptr [ecx + 0xba0]
    //   0x6FB37C7C: imul     eax, eax, 0x23c
    //   0x6FB37C8D: call     0x6fb00830
    //   0x6FB37C9E: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB37CA6: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB37CBE: call     0x6facf5b0
    //   0x6FB37CE5: call     0x6fab1590
    //   0x6FB37CF0: call     0x6fab15c0
    //   0x6FB37D11: call     0x6fad4b20
    //   0x6FB37D25: ret      4
    //   0x6FB37D2C: call     0x6fabc304 ; D2Common.dll!#11161 -> MISSILE_SetTargetX
    //   0x6FB37D36: call     0x6fabc2b6 ; D2Common.dll!#10792 -> MISSILE_SetTargetY
    //   0x6FB37D46: ret      4
    //   0x6FB37D50: ret      4
    return 1;
}

// 1.12a: 0x6FB37D60..0x6FB37E67 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB37D60(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[35]
    // Callees: 0x6fb00830, 0x6fafe320, 0x6fad4b20, D2Common.dll!#11059 -> PATH_SetType, D2Common.dll!#10221 -> D2Common_10142
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB37D6D: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB37D74: cmp      esi, dword ptr [ecx + 0xba0]
    //   0x6FB37D84: imul     eax, eax, 0x23c
    //   0x6FB37D95: ret      4
    //   0x6FB37D9C: call     0x6fb00830
    //   0x6FB37DAB: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB37DB3: cmp      ebp, dword ptr [edx + 0xb6c]
    //   0x6FB37DD4: call     0x6fafe320
    //   0x6FB37DE6: ret      4
    //   0x6FB37E2A: call     0x6fad4b20
    //   0x6FB37E3F: call     0x6fabc760 ; D2Common.dll!#11059 -> PATH_SetType
    //   0x6FB37E48: call     0x6fabc75a ; D2Common.dll!#10221 -> D2Common_10142
    //   0x6FB37E59: ret      4
    //   0x6FB37E64: ret      4
    return 1;
}

// 1.12a: 0x6FB37E70..0x6FB38077 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB37E70(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[25]
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\SkillsPal.cpp'
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill, D2Common.dll!#10619 -> SKILLS_GetFlags, 0x6facf5b0, D2Common.dll!#10974 -> UNITS_IsInMeleeRange, 0x6fb00190, 0x6fb75f30, 0x6fae1fa0
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    SKILLS_SetParam1(pSkill, 0);
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB37E7F: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB37E94: ret      4
    //   0x6FB37E9C: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB37EA3: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB37EB3: imul     edi, edi, 0x23c
    //   0x6FB37EC6: ret      4
    //   0x6FB37ECE: push     0x6fb84550 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsPal.cpp'
    //   0x6FB37ED4: call     0x6fabc2ec ; D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill
    //   0x6FB37ED9: cmp      eax, ebp
    //   0x6FB37EDD: cmp      esi, dword ptr [0x6fbcc3d0]
    //   0x6FB37EE6: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB37EFA: call     0x6facf5b0
    //   0x6FB37F01: cmp      dword ptr [esi], 0
    //   0x6FB37F0E: call     0x6fabc724 ; D2Common.dll!#10974 -> UNITS_IsInMeleeRange
    //   0x6FB37F1A: call     0x6fb00190
    //   0x6FB37F26: ret      4
    //   0x6FB37F40: cmp      eax, 1
    //   0x6FB37F4B: call     0x6fb75f30
    //   0x6FB37F5D: call     0x6fae1fa0
    //   0x6FB37F6D: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB37F7B: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB37F89: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB37F91: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FB37F9E: call     0x6fabc96a ; D2Common.dll!#10919 -> SKILLS_SetParam4
    //   0x6FB37FA3: cmp      dword ptr [esi], 0
    //   0x6FB37FAB: call     0x6fab1940
    //   0x6FB37FC2: call     0x6fab1390
    //   0x6FB37FD2: cmp      ebx, 0xd3
    //   0x6FB37FE1: call     0x6fabc96a ; D2Common.dll!#10919 -> SKILLS_SetParam4
    //   0x6FB37FED: call     0x6fabc2c8 ; D2Common.dll!#10766 -> PATH_SetNewDistance
    //   0x6FB37FF7: call     0x6fabc352 ; D2Common.dll!#10658 -> STATLIST_UnitGetStatValue
    //   0x6FB37FFE: cmp      ebx, 0x32
    //   0x6FB38011: push     0x6fb84550 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsPal.cpp'
    //   0x6FB38016: call     0x6fab1530
    //   0x6FB38024: call     0x6fab18d0
    //   0x6FB3802F: call     0x6fabc748 ; D2Common.dll!#10377 -> PATH_SetVelocity
    //   0x6FB38038: call     0x6fabc75a ; D2Common.dll!#10221 -> D2Common_10142
    //   0x6FB38045: call     0x6facf5f0
    //   0x6FB38054: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB3805C: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FB38063: call     0x6fb00370
    //   0x6FB38074: ret      4
    return 1;
}

// ============================================================================
// SECTION: SKILLS_SkillsMon.asm — Monster Client Skill Start/Do Functions (SKILLS/SkillsMon.cpp)
// ============================================================================

// 1.12a: 0x6FB4C3E0..0x6FB4C576 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB4C3E0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[59]
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\SkillsMon.cpp'
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10188 -> UNITS_GetShiftedFrameMetrics, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10034 -> D2Common_10399, D2Common.dll!#10140 -> SKILLS_SetFlags, D2Common.dll!#10619 -> SKILLS_GetFlags, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, D2Common.dll!#10658 -> STATLIST_UnitGetStatValue
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB4C3EF: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB4C3F6: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FB4C406: imul     esi, esi, 0x23c
    //   0x6FB4C417: ret      4
    //   0x6FB4C41C: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB4C42F: ret      4
    //   0x6FB4C43D: call     0x6fabc33a ; D2Common.dll!#10188 -> UNITS_GetShiftedFrameMetrics
    //   0x6FB4C446: cmp      eax, dword ptr [esi + 0x154]
    //   0x6FB4C45F: ret      4
    //   0x6FB4C463: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB4C46E: call     0x6fabc742 ; D2Common.dll!#10034 -> D2Common_10399
    //   0x6FB4C473: cmp      eax, 1
    //   0x6FB4C47B: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB4C499: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB4C4A5: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB4C4C6: cmp      eax, dword ptr [esi + 0x150]
    //   0x6FB4C4E0: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB4C4EA: cmp      esi, 0x100
    //   0x6FB4C4FC: call     0x6fabc352 ; D2Common.dll!#10658 -> STATLIST_UnitGetStatValue
    //   0x6FB4C506: push     0x6fb83f68 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsMon.cpp'
    //   0x6FB4C512: call     0x6fab18d0
    //   0x6FB4C519: call     0x6fabc748 ; D2Common.dll!#10377 -> PATH_SetVelocity
    //   0x6FB4C521: call     0x6fabc760 ; D2Common.dll!#11059 -> PATH_SetType
    //   0x6FB4C52A: call     0x6fabc75a ; D2Common.dll!#10221 -> D2Common_10142
    //   0x6FB4C532: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB4C543: ret      4
    //   0x6FB4C546: cmp      eax, dword ptr [esi + 0x154]
    //   0x6FB4C562: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB4C573: ret      4
    return 1;
}

// ============================================================================
// SECTION: SKILLS_SkillsBar.asm — Barbarian Client Skill Start/Do Functions (SKILLS/SkillsBar.cpp)
// ============================================================================

// 1.12a: 0x6FB73920..0x6FB739D7 (__usercall, ret 0x4)
int32_t __usercall sub_6FB73920(D2UnitStrc* pUnit /*eax*/, int32_t a1)
{
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\SkillsBar.cpp'
    // Callees: D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10089 -> SKILLS_GetParam2, D2Common.dll!#10140 -> SKILLS_SetFlags, D2Common.dll!#10203 -> PATH_SetMoveTestCollisionMask, D2Common.dll!#10003 -> PATH_SetFootprintCollisionMask, D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos, D2Common.dll!#11059 -> PATH_SetType, 0x6fab1390
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB73931: ret      4
    //   0x6FB73936: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB7393E: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FB73959: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB73961: call     0x6fabcb44 ; D2Common.dll!#10203 -> PATH_SetMoveTestCollisionMask
    //   0x6FB73969: call     0x6fabcb5c ; D2Common.dll!#10003 -> PATH_SetFootprintCollisionMask
    //   0x6FB73975: call     0x6fabc2e6 ; D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos
    //   0x6FB7397D: call     0x6fabc760 ; D2Common.dll!#11059 -> PATH_SetType
    //   0x6FB73984: cmp      eax, 1
    //   0x6FB7398C: call     0x6fab1390
    //   0x6FB7399E: call     0x6fab1940
    //   0x6FB739AF: push     0x6fb86fd4 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsBar.cpp'
    //   0x6FB739B6: call     0x6fabc748 ; D2Common.dll!#10377 -> PATH_SetVelocity
    //   0x6FB739BF: call     0x6fabc75a ; D2Common.dll!#10221 -> D2Common_10142
    //   0x6FB739CC: ret      4
    //   0x6FB739D4: ret      4
    return 1;
}

// 1.12a: 0x6FB739E0..0x6FB73A51 (__usercall, ret 0x8)
// 1.12a: 0x6FB73A30 (internal entry point sub_6FB73A30)
int32_t __usercall sub_6FB739E0(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2)
{
    // Callees: D2Common.dll!#10366 -> UNITS_GetRoom, 0x6fab1a00, D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB739E5: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB739ED: cmp      eax, dword ptr [ecx + 0xba0]
    //   0x6FB739FB: imul     eax, eax, 0x23c
    //   0x6FB73A0C: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB73A18: ret      8
    //   0x6FB73A20: call     0x6fab1a00
    //   0x6FB73A2D: ret      8
    //   0x6FB73A30: mov      edx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB73A3E: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB73A4E: ret      8
    return 1;
}

// 1.12a: 0x6FB73A60..0x6FB73C86 (__fastcall, ret 0xC)
// 1.12a: 0x6FB73B50 (internal entry point sub_6FB73B50)
int32_t __fastcall sub_6FB73A60(D2UnitStrc* pUnit, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: D2Common.dll!#10366 -> UNITS_GetRoom, D2Common.dll!#10363 -> PATH_GetUnitCollisionPattern, D2Common.dll!#10338 -> COLLISION_ResetMaskWithPattern, D2Common.dll!#10003 -> PATH_SetFootprintCollisionMask, D2Common.dll!#10203 -> PATH_SetMoveTestCollisionMask, D2Common.dll!#11059 -> PATH_SetType, D2Common.dll!#11154 -> PATH_SetUnitDeadCollision, D2Common.dll!#10368 -> SKILLS_GetParam3
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB73A79: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB73A90: cmp      dword ptr [esp + 8], 1
    //   0x6FB73AA9: call     0x6fabcc10 ; D2Common.dll!#10363 -> PATH_GetUnitCollisionPattern
    //   0x6FB73AB6: call     0x6fabc6ca ; D2Common.dll!#10338 -> COLLISION_ResetMaskWithPattern
    //   0x6FB73AC1: call     0x6fabcb5c ; D2Common.dll!#10003 -> PATH_SetFootprintCollisionMask
    //   0x6FB73ACC: call     0x6fabcb44 ; D2Common.dll!#10203 -> PATH_SetMoveTestCollisionMask
    //   0x6FB73AE7: call     0x6fabcc10 ; D2Common.dll!#10363 -> PATH_GetUnitCollisionPattern
    //   0x6FB73AF4: call     0x6fabc6ca ; D2Common.dll!#10338 -> COLLISION_ResetMaskWithPattern
    //   0x6FB73AFF: call     0x6fabcb5c ; D2Common.dll!#10003 -> PATH_SetFootprintCollisionMask
    //   0x6FB73B0A: call     0x6fabcb44 ; D2Common.dll!#10203 -> PATH_SetMoveTestCollisionMask
    //   0x6FB73B12: call     0x6fabc760 ; D2Common.dll!#11059 -> PATH_SetType
    //   0x6FB73B34: cmp      ecx, 0xc
    //   0x6FB73B3D: cmp      ecx, 0x11
    //   0x6FB73B45: call     0x6fabcbb0 ; D2Common.dll!#11154 -> PATH_SetUnitDeadCollision
    //   0x6FB73B4D: ret      4
    //   0x6FB73B66: call     0x6fabc31c ; D2Common.dll!#10368 -> SKILLS_GetParam3
    //   0x6FB73B6E: call     0x6fabcd00 ; D2Common.dll!#10620 -> SKILLS_GetParam4
    //   0x6FB73B84: call     0x6face550
    //   0x6FB73B96: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB73B9E: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB73BB6: call     0x6fabce26 ; D2Common.dll!#11026 -> D2COMMON_10212_PATH_SetMoveFlags
    //   0x6FB73BBC: call     0x6fabcbf2 ; D2Common.dll!#10573 -> PATH_RecacheRoomIfNeeded
    //   0x6FB73BC4: call     0x6fabc2c8 ; D2Common.dll!#10766 -> PATH_SetNewDistance
    //   0x6FB73BCC: call     0x6fabce50 ; D2Common.dll!#10452 -> PATH_SetStepNum
    //   0x6FB73BD4: call     0x6fabcbc2 ; D2Common.dll!#10563 -> PATH_SetTargetUnit
    //   0x6FB73BDB: cmp      eax, 2
    //   0x6FB73BE0: cmp      eax, 3
    //   0x6FB73BE5: cmp      eax, 5
    //   0x6FB73BF2: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB73C11: cmp      eax, 2
    //   0x6FB73C16: cmp      eax, 3
    //   0x6FB73C1B: cmp      eax, 5
    //   0x6FB73C28: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB73C46: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB73C4B: call     0x6fab1a00
    //   0x6FB73C55: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB73C61: call     0x6fabc2e6 ; D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos
    //   0x6FB73C69: call     0x6fabc760 ; D2Common.dll!#11059 -> PATH_SetType
    //   0x6FB73C71: call     0x6fabcc2e ; D2Common.dll!#10894 -> D2COMMON_10190_PATH_SetDistance
    //   0x6FB73C7A: call     0x6fabc75a ; D2Common.dll!#10221 -> D2Common_10142
    //   0x6FB73C83: ret      0xc
    return 1;
}

// 1.12a: 0x6FB73C90..0x6FB73CDE (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB73C90(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[27]
    // Callees: D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fafd990
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB73C97: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB73C9E: cmp      edx, dword ptr [ecx + 0xba0]
    //   0x6FB73CA8: imul     eax, eax, 0x23c
    //   0x6FB73CB9: ret      4
    //   0x6FB73CCA: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB73CD0: call     0x6fafd990
    //   0x6FB73CDB: ret      4
    return 1;
}

// 1.12a: 0x6FB73CE0..0x6FB73DC6 (__usercall, ret 0x4)
int32_t __usercall sub_6FB73CE0(D2UnitStrc* pUnit /*eax*/, int32_t a1)
{
    // Callees: D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10089 -> SKILLS_GetParam2, D2Common.dll!#11084 -> PATH_GetVelocity, D2Common.dll!#10666 -> UNITS_GetDistanceToCoordinates, 0x6fb75280, 0x6fb75400
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB73CFE: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB73D06: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FB73D0E: call     0x6fabc976 ; D2Common.dll!#11084 -> PATH_GetVelocity
    //   0x6FB73D27: call     0x6fabc32e ; D2Common.dll!#10666 -> UNITS_GetDistanceToCoordinates
    //   0x6FB73D2E: cmp      edi, 1
    //   0x6FB73D43: cmp      eax, 1
    //   0x6FB73D60: cmp      edi, dword ptr [eax]
    //   0x6FB73D68: cmp      eax, 0x6fb86fc8
    //   0x6FB73D81: imul     esi, edi
    //   0x6FB73D86: cmp      esi, 0x1f4
    //   0x6FB73D95: call     0x6fb75280
    //   0x6FB73D9F: cmp      eax, ebp
    //   0x6FB73DA6: cmp      eax, ebp
    //   0x6FB73DBA: call     0x6fb75400
    //   0x6FB73DC3: ret      4
    return 1;
}

// 1.12a: 0x6FB73DD0..0x6FB73E1E (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB73DD0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[28]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10608 -> D2COMMON_11019
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB73DD5: call     0x6faf0d20
    //   0x6FB73DE6: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB73DF1: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB73DFB: call     0x6fab326d
    //   0x6FB73E01: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB73E0B: ret      4
    //   0x6FB73E0F: call     0x6fabcf10 ; D2Common.dll!#10608 -> D2COMMON_11019
    //   0x6FB73E1B: ret      4
    return 1;
}

// 1.12a: 0x6FB73E20..0x6FB73E6E (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB73E20(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[26]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10714 -> D2COMMON_11018
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB73E25: call     0x6faf0d20
    //   0x6FB73E36: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB73E41: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB73E4B: call     0x6fab326d
    //   0x6FB73E51: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB73E5B: ret      4
    //   0x6FB73E5F: call     0x6fabc328 ; D2Common.dll!#10714 -> D2COMMON_11018
    //   0x6FB73E6B: ret      4
    return 1;
}

// 1.12a: 0x6FB73E70..0x6FB73EC6 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB73E70(void)
{
    // Callees: D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId, D2Common.dll!#10619 -> SKILLS_GetFlags, D2Common.dll!#10140 -> SKILLS_SetFlags, 0x6fae6c10
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB73E73: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB73E7F: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB73E8D: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB73E99: call     0x6fae6c10
    //   0x6FB73EB2: cmp      dword ptr [ecx + 8], eax
    //   0x6FB73EBF: ret
    return 1;
}

// 1.12a: 0x6FB73ED0..0x6FB73FCC (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB73ED0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[45]
    // Callees: D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId, D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10089 -> SKILLS_GetParam2, D2Common.dll!#10619 -> SKILLS_GetFlags, D2Common.dll!#10363 -> PATH_GetUnitCollisionPattern, D2Common.dll!#10366 -> UNITS_GetRoom, D2Common.dll!#10338 -> COLLISION_ResetMaskWithPattern, D2Common.dll!#10003 -> PATH_SetFootprintCollisionMask
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB73EDF: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB73F12: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB73F1C: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FB73F26: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB73F30: cmp      dl, 3
    //   0x6FB73F37: call     0x6fabcc10 ; D2Common.dll!#10363 -> PATH_GetUnitCollisionPattern
    //   0x6FB73F48: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB73F4E: call     0x6fabc6ca ; D2Common.dll!#10338 -> COLLISION_ResetMaskWithPattern
    //   0x6FB73F59: call     0x6fabcb5c ; D2Common.dll!#10003 -> PATH_SetFootprintCollisionMask
    //   0x6FB73F70: call     0x6fabcb44 ; D2Common.dll!#10203 -> PATH_SetMoveTestCollisionMask
    //   0x6FB73F78: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB73F84: call     0x6fae6c10
    //   0x6FB73F8B: call     0x6fae2120
    //   0x6FB73F9C: ret      4
    //   0x6FB73FA6: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB73FBE: ret      4
    //   0x6FB73FC9: ret      4
    return 1;
}

// 1.12a: 0x6FB73FD0..0x6FB7403A (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB73FD0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[40]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10577 -> UNITS_IsSoftMonster, 0x6fae3380
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB73FD5: call     0x6faf0d20
    //   0x6FB73FE6: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB73FF1: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB73FFB: call     0x6fab326d
    //   0x6FB74002: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB74011: ret      4
    //   0x6FB74015: call     0x6fabcfe8 ; D2Common.dll!#10577 -> UNITS_IsSoftMonster
    //   0x6FB7402B: call     0x6fae3380
    //   0x6FB74037: ret      4
    return 1;
}

// 1.12a: 0x6FB74040..0x6FB7409D (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB74040(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[38]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, 0x6fae3380
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB74045: call     0x6faf0d20
    //   0x6FB74056: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB74061: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB7406B: call     0x6fab326d
    //   0x6FB74071: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB7408F: call     0x6fae3380
    //   0x6FB7409A: ret      4
    return 1;
}

// 1.12a: 0x6FB740A0..0x6FB74333 (__usercall, ret 0x10)
int32_t __usercall sub_6FB740A0(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3, int32_t a4)
{
    // Callees: D2Common.dll!#10191 -> PATH_GetXPosition, D2Common.dll!#10178 -> PATH_GetYPosition, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, D2Common.dll!#10666 -> UNITS_GetDistanceToCoordinates, D2Common.dll!#10366 -> UNITS_GetRoom, 0x6fab1a00, D2Common.dll!#10334 -> UNITS_GetUnitSizeX, D2Common.dll!#10504 -> COLLISION_GetFreeCoordinates
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB740AF: cmp      edi, ebx
    //   0x6FB740B7: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB740BE: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FB740D2: imul     esi, esi, 0x23c
    //   0x6FB740DA: cmp      esi, ebx
    //   0x6FB740E5: cmp      eax, 2
    //   0x6FB740EA: cmp      eax, 3
    //   0x6FB740EF: cmp      eax, 5
    //   0x6FB740F7: cmp      eax, ebx
    //   0x6FB740FC: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB7411A: cmp      eax, 2
    //   0x6FB7411F: cmp      eax, 3
    //   0x6FB74124: cmp      eax, 5
    //   0x6FB7412C: cmp      eax, ebx
    //   0x6FB74131: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB74157: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB74171: call     0x6fabc32e ; D2Common.dll!#10666 -> UNITS_GetDistanceToCoordinates
    //   0x6FB74178: cmp      ecx, 1
    //   0x6FB74182: cmp      ecx, ebx
    //   0x6FB74198: imul     eax, ebx
    //   0x6FB741AA: imul     eax, ebx
    //   0x6FB741C0: cmp      eax, -2
    //   0x6FB741D0: cmp      eax, 2
    //   0x6FB741EF: cmp      eax, -2
    //   0x6FB741FF: cmp      eax, 2
    //   0x6FB74223: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB74230: call     0x6fab1a00
    //   0x6FB7423F: call     0x6fabc78a ; D2Common.dll!#10334 -> UNITS_GetUnitSizeX
    //   0x6FB7424B: call     0x6fabc76c ; D2Common.dll!#10504 -> COLLISION_GetFreeCoordinates
    //   0x6FB74264: call     0x6fabcc10 ; D2Common.dll!#10363 -> PATH_GetUnitCollisionPattern
    //   0x6FB7426D: call     0x6fabce74 ; D2Common.dll!#10206 -> COLLISION_CheckAnyCollisionWithPattern
    //   0x6FB74288: call     0x6fabcff4 ; D2Common.dll!#10097 -> D2Common_11025
    //   0x6FB74294: call     0x6fabc32e ; D2Common.dll!#10666 -> UNITS_GetDistanceToCoordinates
    //   0x6FB74299: cmp      eax, dword ptr [esp + 0x1c]
    //   0x6FB742A4: cmp      eax, 2
    //   0x6FB742D0: cmp      eax, 1
    //   0x6FB742FE: cmp      dword ptr [ebp], 0
    //   0x6FB74307: call     0x6fae7250
    //   0x6FB74315: ret      0x10
    //   0x6FB74330: ret      0x10
    return 1;
}

// 1.12a: 0x6FB74340..0x6FB74497 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB74340(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[39]
    // Callees: D2Common.dll!#11081 -> UNITS_GetHitClass, 0x6fae1fa0, 0x6faf0d20, D2Common.dll!#10248 -> PATH_GetTargetUnit, 0x6fab15c0, 0x6fab1590, D2Common.dll!#10756 -> D2Common_10216, Fog.dll!#10265 -> UnimplementedOrdinal_10265
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7434E: cmp      eax, 0x600
    //   0x6FB74356: call     0x6fabcd0c ; D2Common.dll!#11081 -> UNITS_GetHitClass
    //   0x6FB74360: cmp      eax, 0xe
    //   0x6FB74374: call     0x6fae1fa0
    //   0x6FB7437B: call     0x6faf0d20
    //   0x6FB74389: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB743A7: call     0x6fab15c0
    //   0x6FB743AF: call     0x6fab1590
    //   0x6FB743B9: call     0x6fabc6dc ; D2Common.dll!#10756 -> D2Common_10216
    //   0x6FB743C8: ret      4
    //   0x6FB743D4: call     0x6faf0d20
    //   0x6FB743E5: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB743F0: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB743FA: call     0x6fab326d
    //   0x6FB74400: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB7444F: call     0x6fafe9b0
    //   0x6FB74473: call     0x6fab15c0
    //   0x6FB7447B: call     0x6fab1590
    //   0x6FB74485: call     0x6fabc6dc ; D2Common.dll!#10756 -> D2Common_10216
    //   0x6FB74494: ret      4
    return 1;
}

// 1.12a: 0x6FB744A0..0x6FB746F1 (__usercall, ret 0x8)
// 1.12a: 0x6FB745A0 (internal entry point sub_6FB745A0)
int32_t __usercall sub_6FB744A0(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2)
{
    // Callees: D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10089 -> SKILLS_GetParam2, D2Common.dll!#10191 -> PATH_GetXPosition, D2Common.dll!#10178 -> PATH_GetYPosition, D2Common.dll!#10666 -> UNITS_GetDistanceToCoordinates, D2Common.dll!#10562 -> UNITS_SetAnimationFrame, 0x6fb73a60, 0x6fb430e0
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB744AD: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB744B7: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FB744E9: cmp      eax, 1
    //   0x6FB744FD: cmp      dword ptr [esi], 1
    //   0x6FB74507: mov      edi, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB74511: cmp      ecx, dword ptr [edx + 0xa80]
    //   0x6FB7451B: imul     eax, eax, 0x1a8
    //   0x6FB74533: cmp      ecx, dword ptr [edx + 0xa80]
    //   0x6FB7454C: cmp      eax, 2
    //   0x6FB74551: cmp      eax, 3
    //   0x6FB74556: cmp      eax, 5
    //   0x6FB74563: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB74578: cmp      eax, ebx
    //   0x6FB7457E: cmp      eax, 2
    //   0x6FB74583: cmp      eax, 3
    //   0x6FB74588: cmp      eax, 5
    //   0x6FB74595: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB745A6: cmp      eax, dword ptr [esp + 0xc]
    //   0x6FB745BA: cmp      edi, 0x4e
    //   0x6FB745C6: cmp      edi, 0x21c
    //   0x6FB745D5: call     0x6fabc32e ; D2Common.dll!#10666 -> UNITS_GetDistanceToCoordinates
    //   0x6FB745DA: cmp      eax, 2
    //   0x6FB745E7: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB74604: ret      8
    //   0x6FB7460A: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB74629: ret      8
    //   0x6FB74635: call     0x6fb73a60
    //   0x6FB7467B: call     0x6fb430e0
    //   0x6FB74693: call     0x6fae79e0
    //   0x6FB7469A: call     0x6fae1fa0
    //   0x6FB7469F: cmp      edi, 0x4e
    //   0x6FB746B3: call     0x6fb73b50
    //   0x6FB746C3: ret      8
    //   0x6FB746C6: call     0x6fae79e0
    //   0x6FB746CD: call     0x6fae1fa0
    //   0x6FB746DE: call     0x6fae1fa0
    //   0x6FB746EE: ret      8
    return 1;
}

// 1.12a: 0x6FB74700..0x6FB7483A (__fastcall, ret 0x4)
// 1.12a: 0x6FB74720 (internal entry point sub_6FB74720)
int32_t __fastcall sub_6FB74700(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[44]
    // Callees: D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId, D2Common.dll!#10619 -> SKILLS_GetFlags, 0x6fb744a0, D2Common.dll!#10368 -> SKILLS_GetParam3, D2Common.dll!#10620 -> SKILLS_GetParam4, 0x6face550, 0x6fafe9b0, D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB74708: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB74711: cmp      esi, ebx
    //   0x6FB7471D: ret      4
    //   0x6FB74728: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB7473F: call     0x6fb744a0
    //   0x6FB7474F: ret      4
    //   0x6FB74753: call     0x6fabc31c ; D2Common.dll!#10368 -> SKILLS_GetParam3
    //   0x6FB7475B: call     0x6fabcd00 ; D2Common.dll!#10620 -> SKILLS_GetParam4
    //   0x6FB74760: cmp      ebp, 6
    //   0x6FB74776: call     0x6face550
    //   0x6FB7477B: cmp      eax, ebx
    //   0x6FB747AF: call     0x6fafe9b0
    //   0x6FB747B8: cmp      eax, ebx
    //   0x6FB747C0: cmp      eax, ebx
    //   0x6FB747C6: call     0x6fabcac0 ; D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    //   0x6FB747CE: call     0x6facf5f0
    //   0x6FB747D9: call     0x6fabcd0c ; D2Common.dll!#11081 -> UNITS_GetHitClass
    //   0x6FB747E0: call     0x6fae6780
    //   0x6FB747E7: call     0x6fae1fa0
    //   0x6FB74807: call     0x6fb73920
    //   0x6FB7480F: call     0x6fb73ce0
    //   0x6FB74820: ret      4
    //   0x6FB74829: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB74837: ret      4
    return 1;
}

// 1.12a: 0x6FB74840..0x6FB748B2 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB74840(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[43]
    // Callees: D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId, D2Common.dll!#10619 -> SKILLS_GetFlags, 0x6fb744a0, 0x6fb73920, 0x6fb73ce0, D2Common.dll!#10140 -> SKILLS_SetFlags
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB74844: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB74851: ret      4
    //   0x6FB74856: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB74867: call     0x6fb744a0
    //   0x6FB74873: ret      4
    //   0x6FB7487B: call     0x6fb73920
    //   0x6FB74883: call     0x6fb73ce0
    //   0x6FB74890: ret      4
    //   0x6FB74897: cmp      dword ptr [edi], 1
    //   0x6FB748A2: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB748AF: ret      4
    return 1;
}

// 1.12a: 0x6FB748C0..0x6FB74992 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB748C0(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1)
{
    // Callees: D2Common.dll!#10194 -> PATH_GetDirection, D2Common.dll!#10155 -> PATH_SetDirection, D2gfx.dll!#10071 -> D2GFX_CheckPerspective, D2Common.dll!#10098 -> UNITS_GetClientCoordX, D2Common.dll!#10984 -> UNITS_GetPrecisionX, D2gfx.dll!#10052 -> SCALE_ScalePerspectivePosition, D2Common.dll!#10732 -> UNITS_GetPrecisionY, D2Common.dll!#10467 -> UNITS_GetClientCoordY
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB748D7: call     0x6fabc8e0 ; D2Common.dll!#10194 -> PATH_GetDirection
    //   0x6FB748FB: call     0x6fabc2da ; D2Common.dll!#10155 -> PATH_SetDirection
    //   0x6FB74900: call     0x6fabd090 ; D2gfx.dll!#10071 -> D2GFX_CheckPerspective
    //   0x6FB74918: call     0x6fabc232 ; D2Common.dll!#10098 -> UNITS_GetClientCoordX
    //   0x6FB7491F: call     0x6fabc202 ; D2Common.dll!#10984 -> UNITS_GetPrecisionX
    //   0x6FB74925: call     0x6fabd0a8 ; D2gfx.dll!#10052 -> SCALE_ScalePerspectivePosition
    //   0x6FB74953: call     0x6fabc208 ; D2Common.dll!#10732 -> UNITS_GetPrecisionY
    //   0x6FB7495D: call     0x6fabc214 ; D2Common.dll!#10467 -> UNITS_GetClientCoordY
    //   0x6FB7497C: call     0x6fb449c0
    //   0x6FB74983: call     0x6fabc2da ; D2Common.dll!#10155 -> PATH_SetDirection
    //   0x6FB7498F: ret      4
    return 1;
}

// 1.12a: 0x6FB749A0..0x6FB74B63 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB749A0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[41]
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10577 -> UNITS_IsSoftMonster, 0x6fab1590, 0x6fab15c0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB749B3: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB749BA: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FB749CA: imul     ebp, ebp, 0x23c
    //   0x6FB749DB: ret      4
    //   0x6FB749E0: call     0x6faf0d20
    //   0x6FB749F1: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB749FC: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB74A06: call     0x6fab326d
    //   0x6FB74A0D: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB74A20: ret      4
    //   0x6FB74A24: call     0x6fabcfe8 ; D2Common.dll!#10577 -> UNITS_IsSoftMonster
    //   0x6FB74A3F: call     0x6fab1590
    //   0x6FB74A4A: call     0x6fab15c0
    //   0x6FB74A54: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB74A69: call     0x6fab1a00
    //   0x6FB74A85: call     0x6fabc76c ; D2Common.dll!#10504 -> COLLISION_GetFreeCoordinates
    //   0x6FB74A92: cmp      dword ptr [esi], 1
    //   0x6FB74AA5: call     0x6fab13d0
    //   0x6FB74AB6: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB74ADA: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB74AE2: cmp      ebx, dword ptr [eax + 0xb6c]
    //   0x6FB74B32: call     0x6fad4b20
    //   0x6FB74B43: call     0x6fad4b20
    //   0x6FB74B54: ret      4
    //   0x6FB74B60: ret      4
    return 1;
}

// 1.12a: 0x6FB74B70..0x6FB74C28 (__fastcall, ret 0x4)
// 1.12a: 0x6FB74BC0 (internal entry point sub_6FB74BC0)
int32_t __fastcall sub_6FB74B70(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[42]
    // Callees: D2Common.dll!#10205 -> D2Common_10434, D2Common.dll!#10603 -> ITEMS_CheckIfStackable, D2Common.dll!#11034 -> STATLIST_UnitGetItemStatOrSkillStatValue, D2Common.dll!#10286 -> ITEMS_GetMissileType, D2Common.dll!#10231 -> ITEMS_CheckItemTypeId, 0x6fadb4b0, 0x6fafe710
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB74B7A: call     0x6fabccb8 ; D2Common.dll!#10205 -> D2Common_10434
    //   0x6FB74B86: call     0x6fabc54a ; D2Common.dll!#10603 -> ITEMS_CheckIfStackable
    //   0x6FB74B93: cmp      eax, 0x900
    //   0x6FB74BA2: call     0x6fabc54a ; D2Common.dll!#10603 -> ITEMS_CheckIfStackable
    //   0x6FB74BAF: call     0x6fabc5a4 ; D2Common.dll!#11034 -> STATLIST_UnitGetItemStatOrSkillStatValue
    //   0x6FB74BBD: ret      4
    //   0x6FB74BC2: call     0x6fabc4cc ; D2Common.dll!#10286 -> ITEMS_GetMissileType
    //   0x6FB74BCD: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB74BD5: cmp      ebx, dword ptr [edx + 0xb6c]
    //   0x6FB74BE0: call     0x6fabc1d8 ; D2Common.dll!#10231 -> ITEMS_CheckItemTypeId
    //   0x6FB74BFC: call     0x6fadb4b0
    //   0x6FB74C0B: ret      4
    //   0x6FB74C17: call     0x6fafe710
    //   0x6FB74C25: ret      4
    return 1;
}

// 1.12a: 0x6FB74C30..0x6FB74D01 (__usercall, ret 0x0)
int32_t __usercall sub_6FB74C30(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: D2Common.dll!#10220 -> PATH_GetFootprintCollisionMask, D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10191 -> PATH_GetXPosition, D2Common.dll!#10089 -> SKILLS_GetParam2, D2Common.dll!#10178 -> PATH_GetYPosition, D2Common.dll!#10363 -> PATH_GetUnitCollisionPattern, D2Common.dll!#10366 -> UNITS_GetRoom
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB74C38: call     0x6fabcfca ; D2Common.dll!#10220 -> PATH_GetFootprintCollisionMask
    //   0x6FB74C4F: cmp      eax, ebp
    //   0x6FB74C5A: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB74C66: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB74C71: cmp      eax, 2
    //   0x6FB74C76: cmp      eax, 3
    //   0x6FB74C7B: cmp      eax, 5
    //   0x6FB74C88: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB74CA0: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FB74CAB: cmp      eax, 2
    //   0x6FB74CB0: cmp      eax, 3
    //   0x6FB74CB5: cmp      eax, 5
    //   0x6FB74CC2: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB74CD7: call     0x6fabcc10 ; D2Common.dll!#10363 -> PATH_GetUnitCollisionPattern
    //   0x6FB74CE0: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB74CE6: call     0x6fabc6ca ; D2Common.dll!#10338 -> COLLISION_ResetMaskWithPattern
    //   0x6FB74CF1: call     0x6fad1550
    //   0x6FB74CF7: call     0x6fb73a60
    //   0x6FB74D00: ret
    return 1;
}

// 1.12a: 0x6FB74D10..0x6FB74EE4 (__fastcall, ret 0x4)
// 1.12a: 0x6FB74E90 (internal entry point sub_6FB74E90)
int32_t __fastcall sub_6FB74D10(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[31]
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\SkillsBar.cpp'
    // Callees: D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId, D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill, D2Common.dll!#10619 -> SKILLS_GetFlags, D2Common.dll!#10295 -> STATES_CheckState, 0x6fafe320, 0x6facf5b0, D2Common.dll!#10974 -> UNITS_IsInMeleeRange, 0x6fb00190
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    SKILLS_SetParam1(pSkill, 0);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB74D23: cmp      eax, 1
    //   0x6FB74D2F: ret      4
    //   0x6FB74D38: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB74D56: cmp      esi, dword ptr [0x6fbcc3d0]
    //   0x6FB74D63: push     0x6fb86fd4 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsBar.cpp'
    //   0x6FB74D69: call     0x6fabc2ec ; D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill
    //   0x6FB74D6E: cmp      eax, ebx
    //   0x6FB74D73: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB74D83: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB74D9B: call     0x6fafe320
    //   0x6FB74DA8: call     0x6facf5b0
    //   0x6FB74DB7: call     0x6fabc724 ; D2Common.dll!#10974 -> UNITS_IsInMeleeRange
    //   0x6FB74DC3: call     0x6fb00190
    //   0x6FB74DCF: ret      4
    //   0x6FB74E06: call     0x6fabcb44 ; D2Common.dll!#10203 -> PATH_SetMoveTestCollisionMask
    //   0x6FB74E0E: call     0x6fabc760 ; D2Common.dll!#11059 -> PATH_SetType
    //   0x6FB74E17: call     0x6fabc75a ; D2Common.dll!#10221 -> D2Common_10142
    //   0x6FB74E2B: call     0x6fab1940
    //   0x6FB74E3E: push     0x6fb86fd4 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsBar.cpp'
    //   0x6FB74E4B: push     0x6fb86fd4 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsBar.cpp'
    //   0x6FB74E50: call     0x6fab1390
    //   0x6FB74E5E: call     0x6fabc748 ; D2Common.dll!#10377 -> PATH_SetVelocity
    //   0x6FB74E69: call     0x6fabcb44 ; D2Common.dll!#10203 -> PATH_SetMoveTestCollisionMask
    //   0x6FB74E74: call     0x6fabcca6 ; D2Common.dll!#10584 -> PATH_GetPathPoints
    //   0x6FB74E7F: call     0x6fabcb44 ; D2Common.dll!#10203 -> PATH_SetMoveTestCollisionMask
    //   0x6FB74E8D: ret      4
    //   0x6FB74EA0: call     0x6fabcc10 ; D2Common.dll!#10363 -> PATH_GetUnitCollisionPattern
    //   0x6FB74EA9: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB74EAF: call     0x6fabc77e ; D2Common.dll!#10477 -> COLLISION_SetMaskWithPattern
    //   0x6FB74EBB: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB74EC2: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB74EC9: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FB74ED0: call     0x6fb00370
    //   0x6FB74EE1: ret      4
    return 1;
}

// 1.12a: 0x6FB74EF0..0x6FB75051 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB74EF0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[30]
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\SkillsBar.cpp'
    // Callees: D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId, D2Common.dll!#10295 -> STATES_CheckState, D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill, D2Common.dll!#10619 -> SKILLS_GetFlags, 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d
    SKILLS_SetParam1(pSkill, 0);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB74EFB: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB74F04: cmp      esi, ebp
    //   0x6FB74F0C: cmp      edi, dword ptr [0x6fbcc3d0]
    //   0x6FB74F17: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB74F29: push     0x6fb86fd4 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsBar.cpp'
    //   0x6FB74F2F: call     0x6fabc2ec ; D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill
    //   0x6FB74F34: cmp      eax, ebx
    //   0x6FB74F39: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB74F48: call     0x6faf0d20
    //   0x6FB74F50: cmp      eax, ebp
    //   0x6FB74F59: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB74F64: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB74F6E: call     0x6fab326d
    //   0x6FB74F74: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB74F8F: call     0x6fabcfee ; D2Common.dll!#11113 -> SKILLS_CheckIfCanLeapTo
    //   0x6FB74F98: cmp      ebx, ebp
    //   0x6FB74F9F: call     0x6fabc724 ; D2Common.dll!#10974 -> UNITS_IsInMeleeRange
    //   0x6FB74FAB: call     0x6fb00190
    //   0x6FB74FB7: ret      4
    //   0x6FB74FBA: cmp      dword ptr [edi], 1
    //   0x6FB74FC2: call     0x6fae7250
    //   0x6FB74FD0: ret      4
    //   0x6FB74FD4: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB74FE0: call     0x6fabcc10 ; D2Common.dll!#10363 -> PATH_GetUnitCollisionPattern
    //   0x6FB74FF6: call     0x6fabc77e ; D2Common.dll!#10477 -> COLLISION_SetMaskWithPattern
    //   0x6FB75001: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB7500C: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FB75019: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FB75023: call     0x6fabc96a ; D2Common.dll!#10919 -> SKILLS_SetParam4
    //   0x6FB7502D: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FB75038: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB7503D: call     0x6fb00370
    //   0x6FB7504E: ret      4
    return 1;
}

// 1.12a: 0x6FB75060..0x6FB75252 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB75060(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[29]
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\SkillsBar.cpp'
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10295 -> STATES_CheckState, D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill, D2Common.dll!#10619 -> SKILLS_GetFlags, 0x6facf5b0, 0x6fafe320, D2Common.dll!#10919 -> SKILLS_SetParam4, 0x6fab1590
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    SKILLS_SetParam1(pSkill, 0);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7506F: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB7507A: cmp      esi, dword ptr [0x6fbcc3d0]
    //   0x6FB75085: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB75093: push     0x6fb86fd4 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsBar.cpp'
    //   0x6FB75099: call     0x6fabc2ec ; D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill
    //   0x6FB7509E: cmp      eax, edi
    //   0x6FB750A3: call     0x6fabc68e ; D2Common.dll!#10619 -> SKILLS_GetFlags
    //   0x6FB750B4: ret      4
    //   0x6FB750C4: cmp      eax, 1
    //   0x6FB750D9: call     0x6facf5b0
    //   0x6FB750EF: call     0x6fafe320
    //   0x6FB75101: ret      4
    //   0x6FB75107: call     0x6fabc96a ; D2Common.dll!#10919 -> SKILLS_SetParam4
    //   0x6FB75118: call     0x6fab1590
    //   0x6FB75121: call     0x6fab15c0
    //   0x6FB7512C: call     0x6fab1590
    //   0x6FB75138: call     0x6fab15c0
    //   0x6FB75155: call     0x6fabc78a ; D2Common.dll!#10334 -> UNITS_GetUnitSizeX
    //   0x6FB75161: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB75167: call     0x6fabc76c ; D2Common.dll!#10504 -> COLLISION_GetFreeCoordinates
    //   0x6FB75185: call     0x6fafe320
    //   0x6FB751A2: call     0x6fb739e0
    //   0x6FB751C1: call     0x6fb740a0
    //   0x6FB751D7: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB751DC: call     0x6fab1a00
    //   0x6FB751EB: call     0x6fabcc10 ; D2Common.dll!#10363 -> PATH_GetUnitCollisionPattern
    //   0x6FB751F8: call     0x6fabc77e ; D2Common.dll!#10477 -> COLLISION_SetMaskWithPattern
    //   0x6FB751FF: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB75206: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FB75213: call     0x6facf5b0
    //   0x6FB75222: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FB7522C: call     0x6fabc96a ; D2Common.dll!#10919 -> SKILLS_SetParam4
    //   0x6FB75237: call     0x6fabc74e ; D2Common.dll!#10140 -> SKILLS_SetFlags
    //   0x6FB7523E: call     0x6fb00370
    //   0x6FB7524F: ret      4
    return 1;
}

// ============================================================================
// SECTION: SKILLS_SkillsEMon.asm — Expansion Monster Client Skill Start/Do Functions (SKILLS/SkillsEMon.cpp)
// ============================================================================

// 1.12a: 0x6FB77050..0x6FB7742B (__fastcall, ret 0x4)
// 1.12a: 0x6FB77240 (internal entry point sub_6FB77240 - gpfnCltDoFunc[80])
// 1.12a: 0x6FB773B0 (internal entry point sub_6FB773B0)
int32_t __fastcall sub_6FB77050(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[89]
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\SkillsEMon.cpp'
    // Callees: D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId, D2Common.dll!#10210 -> SKILLS_AssignSkill, D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill, D2Common.dll!#10904 -> SKILLS_GetSkillLevel, D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10965 -> SKILLS_SetParam1, D2Common.dll!#10089 -> SKILLS_GetParam2, D2Common.dll!#10368 -> SKILLS_GetParam3
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    SKILLS_SetParam1(pSkill, 0);
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB77060: cmp      eax, 1
    //   0x6FB7706B: ret      4
    //   0x6FB77076: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB77086: push     0x6fb838a8 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsEMon.cpp'
    //   0x6FB77094: call     0x6fabc30a ; D2Common.dll!#10210 -> SKILLS_AssignSkill
    //   0x6FB770A0: call     0x6fabc286 ; D2Common.dll!#10747 -> SKILLS_GetHighestLevelSkillFromUnitAndId
    //   0x6FB770B0: ret      4
    //   0x6FB770BA: push     0x6fb838a8 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsEMon.cpp'
    //   0x6FB770C0: call     0x6fabc2ec ; D2Common.dll!#10239 -> SKILLS_GetSkillIdFromSkill
    //   0x6FB770CB: call     0x6fabc292 ; D2Common.dll!#10904 -> SKILLS_GetSkillLevel
    //   0x6FB770D5: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB770E1: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB770E7: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FB770F1: call     0x6fabc31c ; D2Common.dll!#10368 -> SKILLS_GetParam3
    //   0x6FB77106: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB7710D: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB7711D: imul     ebx, ebx, 0x23c
    //   0x6FB77130: ret      4
    //   0x6FB77141: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB77146: cmp      dword ptr [esp + 0x14], eax
    //   0x6FB77162: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB77176: call     0x6fabc32e ; D2Common.dll!#10666 -> UNITS_GetDistanceToCoordinates
    //   0x6FB7717B: cmp      eax, dword ptr [esp + 0x14]
    //   0x6FB7719E: call     0x6fab1020
    //   0x6FB771C1: call     0x6fab15c0
    //   0x6FB771C9: call     0x6fab1590
    //   0x6FB771D0: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB771D6: call     0x6fabc2b0 ; D2Common.dll!#10973 -> D2Common_10407
    //   0x6FB771E5: call     0x6fad2e50
    //   0x6FB77200: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB77207: call     0x6fab1590
    //   0x6FB7720E: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FB77215: call     0x6fab15c0
    //   0x6FB7721C: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FB7722C: call     0x6fb761d0
    //   0x6FB7723D: ret      4
    //   0x6FB77255: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB7725D: cmp      eax, dword ptr [ecx + 0xba0]
    //   0x6FB77271: imul     esi, esi, 0x23c
    //   0x6FB77284: call     0x6fb00830
    //   0x6FB77295: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB7729D: cmp      eax, dword ptr [ecx + 0xb6c]
    //   0x6FB772B6: call     0x6fafe320
    //   0x6FB772C6: ret      4
    //   0x6FB772DE: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB772E9: call     0x6fab1590
    //   0x6FB772FA: call     0x6fab15c0
    //   0x6FB77311: call     0x6fafd950
    //   0x6FB7731A: call     0x6fafd910
    //   0x6FB7732B: imul     eax, esi
    //   0x6FB7733F: imul     eax, esi
    //   0x6FB77395: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB773C0: call     0x6fad4b20
    //   0x6FB773CC: call     0x6fabc2f2 ; D2Common.dll!#10955 -> MISSILE_GetTotalFrames
    //   0x6FB773D3: call     0x6fb01c20
    //   0x6FB773FA: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB77406: call     0x6fabc2ce ; D2Common.dll!#10562 -> UNITS_SetAnimationFrame
    //   0x6FB7741E: ret      4
    //   0x6FB77428: ret      4
    return 1;
}

// ============================================================================
// SECTION: SKILLS_SkillsNec.asm — Necromancer, Druid & Assassin Client Skill Functions & CRT Stubs (SKILLS/SkillsNec.cpp, SkillsDru.cpp, SkillsAss.cpp)
// ============================================================================

// 1.12a: 0x6FB77430..0x6FB774E6 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB77430(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[93]
    // Callees: 0x6facf5b0, 0x6fae3380, 0x6fad4b20
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB77440: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB77447: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB7745B: imul     ebx, ebx, 0x23c
    //   0x6FB7747A: cmp      edx, esi
    //   0x6FB77480: call     0x6facf5b0
    //   0x6FB77492: call     0x6fae3380
    //   0x6FB774C7: call     0x6fad4b20
    //   0x6FB774D8: ret      4
    //   0x6FB774E3: ret      4
    return 1;
}

// 1.12a: 0x6FB774F0..0x6FB775B2 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB774F0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[77]
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10059 -> STATES_ToggleState, 0x6fb02990, 0x6fb75280, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fb755b0
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB774FB: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB77502: cmp      ebx, dword ptr [eax + 0xba0]
    //   0x6FB77512: imul     edi, edi, 0x23c
    //   0x6FB7751D: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB7752B: ret      4
    //   0x6FB77536: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB7753B: cmp      esi, eax
    //   0x6FB77544: call     0x6fabc26e ; D2Common.dll!#10059 -> STATES_ToggleState
    //   0x6FB7754F: ret      4
    //   0x6FB7755A: call     0x6fb02990
    //   0x6FB77565: call     0x6fb75280
    //   0x6FB77578: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB77585: call     0x6fb755b0
    //   0x6FB775AF: ret      4
    return 1;
}

// 1.12a: 0x6FB775C0..0x6FB7790B (__usercall, ret 0xC)
// 1.12a: 0x6FB77660 (internal entry point sub_6FB77660 - gpfnCltDoFunc[71])
int32_t __usercall sub_6FB775C0(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: 0x6fafe320, 0x6fad4b20, 0x6facf5b0, 0x6fab15c0, 0x6fab1590, D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos, 0x6facf5f0, D2Common.dll!#10990 -> UNITS_UpdateDirectionAndSpeed
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7760A: call     0x6fafe320
    //   0x6FB77619: ret      0xc
    //   0x6FB77647: call     0x6fad4b20
    //   0x6FB7764D: cmp      esi, ebp
    //   0x6FB7765D: ret      0xc
    //   0x6FB77676: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB7767D: cmp      edx, dword ptr [ecx + 0xba0]
    //   0x6FB77691: imul     ebp, ebp, 0x23c
    //   0x6FB776BC: cmp      eax, edx
    //   0x6FB776C4: imul     eax, eax, 0x1a4
    //   0x6FB776E2: ret      4
    //   0x6FB776F4: cmp      eax, edx
    //   0x6FB77700: call     0x6facf5b0
    //   0x6FB77711: call     0x6fab15c0
    //   0x6FB77719: call     0x6fab1590
    //   0x6FB77723: call     0x6fabc2e6 ; D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos
    //   0x6FB7772C: call     0x6facf5f0
    //   0x6FB7773D: call     0x6fafe320
    //   0x6FB77750: ret      4
    //   0x6FB77766: call     0x6fabc2a4 ; D2Common.dll!#10990 -> UNITS_UpdateDirectionAndSpeed
    //   0x6FB77786: call     0x6fab1590
    //   0x6FB77791: call     0x6fab15c0
    //   0x6FB777C3: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB777CC: call     0x6fb75640
    //   0x6FB777E5: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB77835: call     0x6fab1020
    //   0x6FB77890: call     0x6fad4b20
    //   0x6FB778A0: call     0x6fabc2c8 ; D2Common.dll!#10766 -> PATH_SetNewDistance
    //   0x6FB778AB: call     0x6fabc268 ; D2Common.dll!#10299 -> UNITS_SetAnimationSpeed
    //   0x6FB778B2: call     0x6fabc322 ; D2Common.dll!#11076 -> MISSILE_SetTotalFrames
    //   0x6FB778B9: call     0x6fabc256 ; D2Common.dll!#10017 -> MISSILE_SetCurrentFrame
    //   0x6FB778C7: call     0x6fb755b0
    //   0x6FB778CD: call     0x6fb75730
    //   0x6FB778D5: cmp      esi, edi
    //   0x6FB778DC: cmp      eax, edi
    //   0x6FB778FB: ret      4
    //   0x6FB77908: ret      4
    return 1;
}

// 1.12a: 0x6FB77910..0x6FB77A21 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB77910(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[70]
    // Callees: 0x6fb00830, 0x6fafe320, 0x6fab1590, 0x6fab15c0, 0x6fad4b20, D2Common.dll!#11161 -> MISSILE_SetTargetX, D2Common.dll!#10792 -> MISSILE_SetTargetY
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7791D: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB77924: cmp      esi, dword ptr [ecx + 0xba0]
    //   0x6FB77934: imul     eax, eax, 0x23c
    //   0x6FB77945: ret      4
    //   0x6FB7794C: call     0x6fb00830
    //   0x6FB7795B: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB77963: cmp      ebp, dword ptr [edx + 0xb6c]
    //   0x6FB77979: call     0x6fafe320
    //   0x6FB7798B: ret      4
    //   0x6FB779C8: call     0x6fab1590
    //   0x6FB779D9: call     0x6fab15c0
    //   0x6FB779EC: call     0x6fad4b20
    //   0x6FB779FB: call     0x6fabc304 ; D2Common.dll!#11161 -> MISSILE_SetTargetX
    //   0x6FB77A02: call     0x6fabc2b6 ; D2Common.dll!#10792 -> MISSILE_SetTargetY
    //   0x6FB77A13: ret      4
    //   0x6FB77A1E: ret      4
    return 1;
}

// 1.12a: 0x6FB77A30..0x6FB77ADE (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB77A30(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[49]
    // Callees: 0x6fb761d0, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fb775c0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB77A3B: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB77A43: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FB77A53: imul     esi, esi, 0x23c
    //   0x6FB77A62: ret      4
    //   0x6FB77A82: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB77A91: call     0x6fb761d0
    //   0x6FB77A96: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB77AAD: cmp      eax, dword ptr [ecx + 0xb6c]
    //   0x6FB77ABF: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB77ACD: call     0x6fb775c0
    //   0x6FB77ADB: ret      4
    return 1;
}

// 1.12a: 0x6FB77AE0..0x6FB77C0B (__fastcall, ret 0x4)
// 1.12a: 0x6FB77BC0 (internal entry point sub_6FB77BC0)
int32_t __fastcall sub_6FB77AE0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[72]
    // Callees: 0x6fb00830, 0x6fafe320, 0x6fadb400, D2Common.dll!#10955 -> MISSILE_GetTotalFrames, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fb75640, 0x6fb755b0, 0x6fb75730
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB77AF1: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB77AF8: cmp      ebp, dword ptr [ecx + 0xba0]
    //   0x6FB77B0C: imul     eax, eax, 0x23c
    //   0x6FB77B21: call     0x6fb00830
    //   0x6FB77B32: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB77B3A: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB77B53: call     0x6fafe320
    //   0x6FB77B72: call     0x6fadb400
    //   0x6FB77B86: ret      4
    //   0x6FB77B8A: call     0x6fabc2f2 ; D2Common.dll!#10955 -> MISSILE_GetTotalFrames
    //   0x6FB77BA3: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB77BAC: call     0x6fb75640
    //   0x6FB77BB5: cmp      ebx, ebp
    //   0x6FB77BCD: call     0x6fb755b0
    //   0x6FB77BD5: cmp      eax, ebp
    //   0x6FB77BDC: cmp      eax, ebp
    //   0x6FB77BED: call     0x6fb75730
    //   0x6FB77BFE: ret      4
    //   0x6FB77C08: ret      4
    return 1;
}

// 1.12a: 0x6FB77C10..0x6FB77C7F (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB77C10(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[23]
    // Referenced strings: '..\\Source\\D2Client\\SKILLS\\SkillsNec.cpp'
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10257 -> ITEMS_IsMetalItem, D2Common.dll!#10989 -> ITEMS_CheckItemFlag
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB77C15: call     0x6faf0d20
    //   0x6FB77C26: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB77C31: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB77C3B: call     0x6fab326d
    //   0x6FB77C41: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB77C4F: ret      4
    //   0x6FB77C52: cmp      dword ptr [esi], 4
    //   0x6FB77C5B: call     0x6fabc70c ; D2Common.dll!#10257 -> ITEMS_IsMetalItem
    //   0x6FB77C64: cmp      dword ptr [esi + 0x10], 3
    //   0x6FB77C6A: push     0x6fb83cf4 ; STR:'..\\Source\\D2Client\\SKILLS\\SkillsNec.cpp'
    //   0x6FB77C77: call     0x6fabc1cc ; D2Common.dll!#10989 -> ITEMS_CheckItemFlag
    return 1;
}

// 1.12a: 0x6FB77C80..0x6FB77C9B (__stdcall, ret 0x4)
int32_t __stdcall sub_6FB77C80(int32_t a1)
{
    // Callees: D2Common.dll!#10366 -> UNITS_GetRoom, D2Common.dll!#10861 -> STATLIST_GetOwner
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB77C81: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB77C8D: call     0x6fabc514 ; D2Common.dll!#10861 -> STATLIST_GetOwner
    //   0x6FB77C98: ret      4
    return 1;
}

// 1.12a: 0x6FB77CA0..0x6FB77D44 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB77CA0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[22]
    // Callees: D2Common.dll!#10366 -> UNITS_GetRoom, 0x6fafe320, 0x6fab1a00, D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB77CAB: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB77CB2: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FB77CC0: imul     edx, edx, 0x23c
    //   0x6FB77CD3: ret      4
    //   0x6FB77CD8: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB77CE9: ret      4
    //   0x6FB77CF6: call     0x6fafe320
    //   0x6FB77D08: ret      4
    //   0x6FB77D15: call     0x6fab1a00
    //   0x6FB77D1E: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB77D2C: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB77D41: ret      4
    return 1;
}

// 1.12a: 0x6FB77D50..0x6FB77EB2 (__fastcall, ret 0x4)
// 1.12a: 0x6FB77E30 (internal entry point sub_6FB77E30 - gpfnCltStFunc[20])
int32_t __fastcall sub_6FB77D50(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[21]
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10712 -> D2COMMON_11021, D2Common.dll!#10191 -> PATH_GetXPosition
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    SKILLS_SetParam1(pSkill, 0);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB77D55: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB77D62: ret      4
    //   0x6FB77D67: call     0x6faf0d20
    //   0x6FB77D78: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB77D83: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB77D8D: call     0x6fab326d
    //   0x6FB77D94: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB77DA4: ret      4
    //   0x6FB77DA7: cmp      edi, dword ptr [0x6fbcc3d0]
    //   0x6FB77DB0: call     0x6fabc700 ; D2Common.dll!#10712 -> D2COMMON_11021
    //   0x6FB77DBB: cmp      eax, 2
    //   0x6FB77DC0: cmp      eax, 3
    //   0x6FB77DC5: cmp      eax, 5
    //   0x6FB77DD2: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB77DE5: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB77DEC: cmp      eax, 2
    //   0x6FB77DF1: cmp      eax, 3
    //   0x6FB77DF6: cmp      eax, 5
    //   0x6FB77E03: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB77E16: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FB77E20: call     0x6fabc2f8 ; D2Common.dll!#10454 -> SKILLS_SetParam3
    //   0x6FB77E2D: ret      4
    //   0x6FB77E35: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB77E42: call     0x6faf0d20
    //   0x6FB77E53: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB77E5E: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB77E68: call     0x6fab326d
    //   0x6FB77E6E: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB77E7C: call     0x6fabc6fa ; D2Common.dll!#10675 -> D2COMMON_11017_CheckUnitIfConsumeable
    //   0x6FB77E89: ret      4
    //   0x6FB77E8E: call     0x6fab1590
    //   0x6FB77E95: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB77E9C: call     0x6fab15c0
    //   0x6FB77EA3: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FB77EAF: ret      4
    return 1;
}

// 1.12a: 0x6FB77EC0..0x6FB77F1A (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB77EC0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[18]
    // Callees: 0x6fafe710
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB77EC8: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB77ED0: cmp      edx, dword ptr [edi + 0xba0]
    //   0x6FB77EE0: imul     esi, esi, 0x23c
    //   0x6FB77EEE: ret      4
    //   0x6FB77EFC: cmp      esi, dword ptr [edi + 0xb6c]
    //   0x6FB77F0B: call     0x6fafe710
    //   0x6FB77F17: ret      4
    return 1;
}

// 1.12a: 0x6FB77F20..0x6FB77F8D (__usercall, ret 0x0)
int32_t __usercall sub_6FB77F20(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, 0x6fab1670, D2Common.dll!#10675 -> D2COMMON_11017_CheckUnitIfConsumeable, 0x6fae3ae0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB77F23: call     0x6faf0d20
    //   0x6FB77F34: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB77F3F: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB77F49: call     0x6fab326d
    //   0x6FB77F4F: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB77F5D: ret
    //   0x6FB77F5E: cmp      dword ptr [esi], 1
    //   0x6FB77F6B: call     0x6fab1670
    //   0x6FB77F77: call     0x6fabc6fa ; D2Common.dll!#10675 -> D2COMMON_11017_CheckUnitIfConsumeable
    //   0x6FB77F80: call     0x6fae3ae0
    //   0x6FB77F8C: ret
    return 1;
}

// 1.12a: 0x6FB77F90..0x6FB78079 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB77F90(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[24]
    // Callees: 0x6fb77f20, 0x6fab13d0, 0x6fad4b20
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB77F9F: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB77FA6: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB77FB6: imul     esi, esi, 0x23c
    //   0x6FB77FC7: ret      4
    //   0x6FB77FCD: call     0x6fb77f20
    //   0x6FB77FDF: call     0x6fab13d0
    //   0x6FB78013: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB7801B: cmp      edx, dword ptr [eax + 0xb6c]
    //   0x6FB78059: call     0x6fad4b20
    //   0x6FB7806B: ret      4
    //   0x6FB78076: ret      4
    return 1;
}

// 1.12a: 0x6FB78080..0x6FB78202 (__fastcall, ret 0xC)
int32_t __fastcall sub_6FB78080(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: 0x6fad4b20, 0x6fad85f0, 0x6facf5b0, 0x6fae1fa0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78091: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78099: cmp      edx, dword ptr [ecx + 0xba0]
    //   0x6FB780AD: imul     ebp, ebp, 0x23c
    //   0x6FB780CA: cmp      ebx, dword ptr [ecx + 0xb6c]
    //   0x6FB780DE: cmp      eax, dword ptr [ecx + 0xa80]
    //   0x6FB780F0: imul     eax, eax, 0x1a8
    //   0x6FB78160: call     0x6fad4b20
    //   0x6FB78172: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB7817A: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB78192: call     0x6fad4b20
    //   0x6FB781A6: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB781AD: cmp      ebx, dword ptr [ecx + 0xb6c]
    //   0x6FB781BD: call     0x6fad85f0
    //   0x6FB781C6: call     0x6facf5b0
    //   0x6FB781E2: call     0x6fae1fa0
    //   0x6FB781F3: ret      0xc
    //   0x6FB781FF: ret      0xc
    return 1;
}

// 1.12a: 0x6FB78210..0x6FB78473 (__usercall, ret 0x14)
// 1.12a: 0x6FB78250 (internal entry point sub_6FB78250)
int32_t __usercall sub_6FB78210(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5)
{
    // Callees: 0x6fab13d0, 0x6fab1020, 0x6faff4d0, 0x6fab1480, 0x6fad4b20, 0x6facf5b0, 0x6fae1fa0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78220: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78227: cmp      ebp, dword ptr [ecx + 0xba0]
    //   0x6FB78237: imul     eax, eax, 0x23c
    //   0x6FB7824D: ret      0x14
    //   0x6FB78260: cmp      ebx, dword ptr [ecx + 0xb6c]
    //   0x6FB7828E: call     0x6fab13d0
    //   0x6FB782D1: call     0x6fab1020
    //   0x6FB782E9: call     0x6fab1020
    //   0x6FB78315: mov      ecx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB78345: call     0x6faff4d0
    //   0x6FB7834C: call     0x6fab1480
    //   0x6FB7835A: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB7836B: cmp      eax, edx
    //   0x6FB7837F: call     0x6faff4d0
    //   0x6FB78386: call     0x6fab1480
    //   0x6FB78394: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB783A5: cmp      eax, edx
    //   0x6FB783B8: call     0x6faff4d0
    //   0x6FB783E1: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB783E9: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB783F9: call     0x6fad4b20
    //   0x6FB7840B: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78413: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB7842B: call     0x6fad4b20
    //   0x6FB78430: call     0x6facf5b0
    //   0x6FB78453: call     0x6fae1fa0
    //   0x6FB78464: ret      0x14
    //   0x6FB78470: ret      0x14
    return 1;
}

// 1.12a: 0x6FB78480..0x6FB7871D (__fastcall, ret 0xC)
// 1.12a: 0x6FB78560 (internal entry point sub_6FB78560)
// 1.12a: 0x6FB78630 (internal entry point sub_6FB78630)
int32_t __fastcall sub_6FB78480(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[31]
    // Callees: 0x6fb00830, D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10089 -> SKILLS_GetParam2, 0x6fad4b20, D2Common.dll!#10965 -> SKILLS_SetParam1, D2Common.dll!#10817 -> SKILLS_SetParam2, 0x6fafe320
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    SKILLS_SetParam1(pSkill, 0);
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78491: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78498: cmp      ebx, dword ptr [ecx + 0xba0]
    //   0x6FB784A8: imul     eax, eax, 0x23c
    //   0x6FB784B9: ret      4
    //   0x6FB784C0: call     0x6fb00830
    //   0x6FB784CF: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB784D7: cmp      ebp, dword ptr [edx + 0xb6c]
    //   0x6FB784E0: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB7851A: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB78524: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FB78531: call     0x6fad4b20
    //   0x6FB78539: call     0x6fabc27a ; D2Common.dll!#10965 -> SKILLS_SetParam1
    //   0x6FB78541: call     0x6fabc2bc ; D2Common.dll!#10817 -> SKILLS_SetParam2
    //   0x6FB78552: ret      4
    //   0x6FB7855D: ret      4
    //   0x6FB78570: call     0x6fafe320
    //   0x6FB7857E: ret      0xc
    //   0x6FB78583: cmp      eax, 2
    //   0x6FB78588: cmp      eax, 3
    //   0x6FB7858D: cmp      eax, 5
    //   0x6FB7859A: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB785B6: cmp      eax, 2
    //   0x6FB785C0: cmp      eax, 3
    //   0x6FB785C5: cmp      eax, 5
    //   0x6FB785D2: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB785EE: call     0x6fabc32e ; D2Common.dll!#10666 -> UNITS_GetDistanceToCoordinates
    //   0x6FB785F5: cmp      edi, 1
    //   0x6FB7860C: cmp      eax, 2
    //   0x6FB78611: cmp      eax, 3
    //   0x6FB78616: cmp      eax, 5
    //   0x6FB78623: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB78643: cmp      eax, 2
    //   0x6FB78648: cmp      eax, 3
    //   0x6FB7864D: cmp      eax, 5
    //   0x6FB7865A: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB786DF: call     0x6fad4b20
    //   0x6FB786F1: ret      0xc
    //   0x6FB786FF: call     0x6fabc718 ; D2Common.dll!#10168 -> UNITS_GetDirectionToCoords
    //   0x6FB78709: call     0x6fabc250 ; D2Common.dll!#11130 -> D2COMMON_10160_PathUpdateDirection
    //   0x6FB7871A: ret      0xc
    return 1;
}

// 1.12a: 0x6FB78720..0x6FB7886C (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB78720(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[30]
    // Callees: 0x6fafe320, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fad4b20, 0x6faceaa0, 0x6fb0eaf0, 0x6fb0eab0, D2Common.dll!#10247 -> MISSILE_SetStreamMissile, D2Common.dll!#10648 -> MISSILE_SetStreamRange
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78732: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78739: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB7874D: imul     esi, esi, 0x23c
    //   0x6FB7876E: cmp      ecx, dword ptr [eax + 0xb6c]
    //   0x6FB7878F: call     0x6fafe320
    //   0x6FB787A1: ret      4
    //   0x6FB787AF: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB787B9: cmp      esi, 2
    //   0x6FB78804: call     0x6fad4b20
    //   0x6FB7880F: call     0x6faceaa0
    //   0x6FB78829: call     0x6fb0eaf0
    //   0x6FB78838: call     0x6fb0eab0
    //   0x6FB78846: call     0x6fabc706 ; D2Common.dll!#10247 -> MISSILE_SetStreamMissile
    //   0x6FB7884D: call     0x6fabc712 ; D2Common.dll!#10648 -> MISSILE_SetStreamRange
    //   0x6FB7885E: ret      4
    //   0x6FB78869: ret      4
    return 1;
}

// 1.12a: 0x6FB78870..0x6FB788BD (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB78870(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[33]
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10089 -> SKILLS_GetParam2, D2Common.dll!#10368 -> SKILLS_GetParam3, 0x6fb78080
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7887A: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB78888: ret      4
    //   0x6FB7888E: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB78896: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FB7889E: call     0x6fabc31c ; D2Common.dll!#10368 -> SKILLS_GetParam3
    //   0x6FB788B0: call     0x6fb78080
    //   0x6FB788BA: ret      4
    return 1;
}

// 1.12a: 0x6FB788C0..0x6FB7890F (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB788C0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[32]
    // Callees: D2Common.dll!#10652 -> UNITS_GetUsedSkill, D2Common.dll!#10063 -> SKILLS_GetParam1, D2Common.dll!#10089 -> SKILLS_GetParam2, D2Common.dll!#10368 -> SKILLS_GetParam3, 0x6fb78210
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }
    int32_t nParam1 = SKILLS_GetParam1(pSkill);
    (void)nParam1;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB788CA: call     0x6fabc274 ; D2Common.dll!#10652 -> UNITS_GetUsedSkill
    //   0x6FB788D8: ret      4
    //   0x6FB788DE: call     0x6fabc2fe ; D2Common.dll!#10063 -> SKILLS_GetParam1
    //   0x6FB788E6: call     0x6fabc28c ; D2Common.dll!#10089 -> SKILLS_GetParam2
    //   0x6FB788EE: call     0x6fabc31c ; D2Common.dll!#10368 -> SKILLS_GetParam3
    //   0x6FB78902: call     0x6fb78210
    //   0x6FB7890C: ret      4
    return 1;
}

// 1.12a: 0x6FB78910..0x6FB78965 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB78910(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[19]
    // Callees: 0x6fb78560
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78918: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78920: cmp      edx, dword ptr [edi + 0xba0]
    //   0x6FB78930: imul     esi, esi, 0x23c
    //   0x6FB78945: cmp      esi, dword ptr [edi + 0xb6c]
    //   0x6FB78954: call     0x6fb78560
    //   0x6FB7895B: ret      4
    //   0x6FB78962: ret      4
    return 1;
}

// 1.12a: 0x6FB78970..0x6FB789EC (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB78970(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Referenced strings: '1.1.4'
    // Callees: 0x6fac0680, 0x6fac0250, 0x6fac0640
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78986: mov      eax, 0x6fb871d0 ; STR:'1.1.4'
    //   0x6FB789A4: call     0x6fac0680
    //   0x6FB789B7: call     0x6fac0250
    //   0x6FB789BE: cmp      edi, 1
    //   0x6FB789C7: call     0x6fac0640
    //   0x6FB789DB: ret
    //   0x6FB789E2: call     0x6fac0640
    //   0x6FB789EB: ret
    return 1;
}

// 1.12a: 0x6FB789F0..0x6FB78A16 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB789F0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[85]
    // Callees: 0x6fae3380
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB789F5: cmp      eax, dword ptr [0x6fbd3690]
    //   0x6FB78A09: call     0x6fae3380
    //   0x6FB78A13: ret      4
    return 1;
}

// 1.12a: 0x6FB78A20..0x6FB78C0D (__usercall, ret 0x14)
int32_t __usercall sub_6FB78A20(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5)
{
    // Callees: 0x6faf0d20, Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, D2Common.dll!#10248 -> PATH_GetTargetUnit, D2Common.dll!#10024 -> D2COMMON_10175_PathGetFirstPointX, D2Common.dll!#10728 -> D2COMMON_10176_PathGetFirstPointY, D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78A2F: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78A36: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB78A44: imul     ebp, ebp, 0x23c
    //   0x6FB78A55: ret      0x14
    //   0x6FB78A5A: call     0x6faf0d20
    //   0x6FB78A6B: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB78A76: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB78A80: call     0x6fab326d
    //   0x6FB78A87: call     0x6fabc72a ; D2Common.dll!#10248 -> PATH_GetTargetUnit
    //   0x6FB78A94: call     0x6fabc334 ; D2Common.dll!#10024 -> D2COMMON_10175_PathGetFirstPointX
    //   0x6FB78AA1: call     0x6fabc34c ; D2Common.dll!#10728 -> D2COMMON_10176_PathGetFirstPointY
    //   0x6FB78AC0: call     0x6fabcac0 ; D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    //   0x6FB78AD5: call     0x6fabc2e6 ; D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos
    //   0x6FB78ADD: mov      edx, dword ptr [0x6fb7ee4c] ; IAT:Fog.dll!gdwBitMasks -> gdwBitMasks
    //   0x6FB78AEE: cmp      edi, eax
    //   0x6FB78AF8: call     0x6faceac0
    //   0x6FB78B04: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB78B1A: call     0x6fabc26e ; D2Common.dll!#10059 -> STATES_ToggleState
    //   0x6FB78B2D: call     0x6fafee40
    //   0x6FB78B43: call     0x6fabc26e ; D2Common.dll!#10059 -> STATES_ToggleState
    //   0x6FB78B5E: call     0x6fae1fa0
    //   0x6FB78B73: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78B84: cmp      eax, edx
    //   0x6FB78B95: call     0x6fb430e0
    //   0x6FB78BAB: call     0x6faffde0
    //   0x6FB78BC2: call     0x6fabcac0 ; D2Common.dll!#10647 -> UNITS_SetTargetUnitForDynamicUnit
    //   0x6FB78BD7: call     0x6fabc2e6 ; D2Common.dll!#10515 -> D2COMMON_10170_PathSetTargetPos
    //   0x6FB78BF7: ret      0x14
    //   0x6FB78C0A: ret      0x14
    return 1;
}

// 1.12a: 0x6FB78C10..0x6FB78C3F (__fastcall, ret 0xC)
int32_t __fastcall sub_6FB78C10(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: 0x6fb78a20
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78C30: call     0x6fb78a20
    //   0x6FB78C36: ret      0xc
    //   0x6FB78C3C: ret      0xc
    return 1;
}

// 1.12a: 0x6FB78C40..0x6FB78C67 (__fastcall, ret 0x8)
int32_t __fastcall sub_6FB78C40(D2UnitStrc* pUnit, int32_t nArgEdx, int32_t a1, int32_t a2)
{
    // Callees: 0x6fb78a20
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78C58: call     0x6fb78a20
    //   0x6FB78C5E: ret      8
    //   0x6FB78C64: ret      8
    return 1;
}

// 1.12a: 0x6FB78C70..0x6FB78D0C (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB78C70(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Callees: D2Common.dll!#11017 -> STATLIST_AreUnitsAligned, 0x6fae0dd0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78C9A: ret
    //   0x6FB78C9D: call     0x6fabc1a2 ; D2Common.dll!#11017 -> STATLIST_AreUnitsAligned
    //   0x6FB78CA3: ret
    //   0x6FB78CBB: call     0x6fae0dd0
    //   0x6FB78CC6: ret
    //   0x6FB78CCE: call     0x6fae0dd0
    //   0x6FB78CD9: ret
    //   0x6FB78CEF: call     0x6fae0dd0
    //   0x6FB78CFA: ret
    //   0x6FB78D00: call     0x6fae0dd0
    //   0x6FB78D0B: ret
    return 1;
}

// 1.12a: 0x6FB78D10..0x6FB78D85 (__usercall, ret 0x0)
int32_t __usercall sub_6FB78D10(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, 0x6fae0e60, 0x6fb281c0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78D14: cmp      dword ptr [eax], 0
    //   0x6FB78D1B: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB78D26: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB78D30: call     0x6fab326d
    //   0x6FB78D4A: call     0x6fae0e60
    //   0x6FB78D51: call     0x6fb281c0
    //   0x6FB78D61: cmp      esi, edi
    //   0x6FB78D70: cmp      dword ptr [eax], edi
    //   0x6FB78D7D: ret
    //   0x6FB78D84: ret
    return 1;
}

// 1.12a: 0x6FB78D90..0x6FB78E0F (__usercall, ret 0x0)
int32_t __usercall sub_6FB78D90(D2UnitStrc* pUnit /*eax*/)
{
    // Callees: Fog.dll!#10265 -> UnimplementedOrdinal_10265, Fog.dll!#10024 -> FOG_DisplayHalt, 0x6fab326d, 0x6fae0e60, 0x6fb281c0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78D94: cmp      dword ptr [eax], 0
    //   0x6FB78D9B: call     0x6fabbf8c ; Fog.dll!#10265 -> UnimplementedOrdinal_10265
    //   0x6FB78DA6: call     0x6fabbf80 ; Fog.dll!#10024 -> FOG_DisplayHalt
    //   0x6FB78DB0: call     0x6fab326d
    //   0x6FB78DCA: call     0x6fae0e60
    //   0x6FB78DD1: call     0x6fb281c0
    //   0x6FB78DE1: cmp      esi, edi
    //   0x6FB78DF0: cmp      dword ptr [eax], edi
    //   0x6FB78E02: ret
    //   0x6FB78E0E: ret
    return 1;
}

// 1.12a: 0x6FB78E10..0x6FB78EC1 (__fastcall, ret 0x4)
// 1.12a: 0x6FB78E40 (internal entry point sub_6FB78E40)
int32_t __fastcall sub_6FB78E10(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Start Function: gpfnCltStFunc[45]
    // Callees: D2Common.dll!#10295 -> STATES_CheckState, 0x6fb430e0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78E18: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78E1F: cmp      edx, dword ptr [eax + 0xba0]
    //   0x6FB78E2D: imul     edx, edx, 0x23c
    //   0x6FB78E3D: ret      4
    //   0x6FB78E50: cmp      ebx, dword ptr [eax + 0xc4]
    //   0x6FB78E6D: cmp      eax, edx
    //   0x6FB78E83: cmp      ecx, edx
    //   0x6FB78E89: call     0x6fabc21a ; D2Common.dll!#10295 -> STATES_CheckState
    //   0x6FB78EA8: call     0x6fb430e0
    //   0x6FB78EB6: ret      4
    //   0x6FB78EBE: ret      4
    return 1;
}

// 1.12a: 0x6FB78ED0..0x6FB78FD5 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB78ED0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[91]
    // Callees: 0x6fafe320, 0x6fad4b20, D2Common.dll!#11161 -> MISSILE_SetTargetX
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78EDE: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78EE5: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB78EF5: imul     esi, esi, 0x23c
    //   0x6FB78F07: ret      4
    //   0x6FB78F14: call     0x6fafe320
    //   0x6FB78F24: ret      4
    //   0x6FB78F68: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78F79: cmp      eax, edx
    //   0x6FB78F85: call     0x6fad4b20
    //   0x6FB78F96: mov      edx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78FA7: cmp      eax, edx
    //   0x6FB78FB3: call     0x6fad4b20
    //   0x6FB78FC1: call     0x6fabc304 ; D2Common.dll!#11161 -> MISSILE_SetTargetX
    //   0x6FB78FD2: ret      4
    return 1;
}

// 1.12a: 0x6FB78FE0..0x6FB791B6 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB78FE0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[92]
    // Callees: D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fafe320, 0x6fad4b20, 0x6fb755b0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB78FF2: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB78FF9: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB79009: imul     esi, esi, 0x23c
    //   0x6FB7901B: ret      4
    //   0x6FB7902D: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB7903F: ret      4
    //   0x6FB7904C: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB79063: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB7907A: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB7908D: imul     eax, ecx
    //   0x6FB79096: imul     eax, ecx
    //   0x6FB790A6: call     0x6fafe320
    //   0x6FB79102: call     0x6fad4b20
    //   0x6FB79118: call     0x6fb755b0
    //   0x6FB79167: call     0x6fad4b20
    //   0x6FB7917D: call     0x6fb755b0
    //   0x6FB791B3: ret      4
    return 1;
}

// 1.12a: 0x6FB791C0..0x6FB7943B (__usercall, ret 0xC)
int32_t __usercall sub_6FB791C0(D2UnitStrc* pUnit /*eax*/, int32_t nArgEdx /*edx*/, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: D2Common.dll!#10191 -> PATH_GetXPosition, D2Common.dll!#10178 -> PATH_GetYPosition, 0x6fad4b20, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fab1020, D2Common.dll!#11161 -> MISSILE_SetTargetX, D2Common.dll!#10792 -> MISSILE_SetTargetY
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB791EB: cmp      eax, 2
    //   0x6FB791F0: cmp      eax, 3
    //   0x6FB791F5: cmp      eax, 5
    //   0x6FB79202: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB7921F: cmp      eax, 2
    //   0x6FB79224: cmp      eax, 3
    //   0x6FB79229: cmp      eax, 5
    //   0x6FB79236: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB79283: cmp      word ptr [ecx + 0xec], 0
    //   0x6FB79295: cmp      word ptr [ecx + 0xee], di
    //   0x6FB79362: call     0x6fad4b20
    //   0x6FB79389: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB79390: call     0x6fab1020
    //   0x6FB7939A: cmp      ecx, esi
    //   0x6FB793A1: cmp      ecx, esi
    //   0x6FB793B3: cmp      eax, 2
    //   0x6FB793B8: cmp      eax, 3
    //   0x6FB793BD: cmp      eax, 5
    //   0x6FB793C5: cmp      eax, esi
    //   0x6FB793CA: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB793DD: call     0x6fabc304 ; D2Common.dll!#11161 -> MISSILE_SetTargetX
    //   0x6FB793E4: cmp      eax, 2
    //   0x6FB793E9: cmp      eax, 3
    //   0x6FB793EE: cmp      eax, 5
    //   0x6FB793F6: cmp      ebx, esi
    //   0x6FB793FB: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB79402: call     0x6fabc2b6 ; D2Common.dll!#10792 -> MISSILE_SetTargetY
    //   0x6FB7940E: ret      0xc
    //   0x6FB79415: call     0x6fabc2b6 ; D2Common.dll!#10792 -> MISSILE_SetTargetY
    //   0x6FB79421: ret      0xc
    //   0x6FB7942C: call     0x6fabc2b6 ; D2Common.dll!#10792 -> MISSILE_SetTargetY
    //   0x6FB79438: ret      0xc
    return 1;
}

// 1.12a: 0x6FB79440..0x6FB795C4 (__usercall, ret 0x1C)
int32_t __usercall sub_6FB79440(D2UnitStrc* pUnit /*eax*/, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7)
{
    // Callees: 0x6fafe320, 0x6fafe710, D2Common.dll!#10794 -> D2COMMON_10203_PATH_SetRotateFlag, D2Common.dll!#10299 -> UNITS_SetAnimationSpeed, D2Common.dll!#10191 -> PATH_GetXPosition, D2Common.dll!#10178 -> PATH_GetYPosition, 0x6fad4b20
    D2UnitStrc* pTargetUnit = nullptr;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    // sub_6FAFE320(pUnit, &pTargetUnit, &nTargetX, &nTargetY);
    (void)pTargetUnit; (void)nTargetX; (void)nTargetY;
    // Spawn client-side skill missile(s)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB79458: call     0x6fafe320
    //   0x6FB79467: ret      0x1c
    //   0x6FB7948F: call     0x6fafe710
    //   0x6FB794AB: call     0x6fabc2e0 ; D2Common.dll!#10794 -> D2COMMON_10203_PATH_SetRotateFlag
    //   0x6FB794B9: call     0x6fabc268 ; D2Common.dll!#10299 -> UNITS_SetAnimationSpeed
    //   0x6FB794CD: cmp      eax, 2
    //   0x6FB794DE: cmp      eax, 3
    //   0x6FB794E3: cmp      eax, 5
    //   0x6FB794F0: call     0x6fabc2d4 ; D2Common.dll!#10191 -> PATH_GetXPosition
    //   0x6FB79507: cmp      eax, 2
    //   0x6FB7950C: cmp      eax, 3
    //   0x6FB79511: cmp      eax, 5
    //   0x6FB7951E: call     0x6fabc262 ; D2Common.dll!#10178 -> PATH_GetYPosition
    //   0x6FB79588: call     0x6fad4b20
    //   0x6FB7959D: call     0x6fabc2e0 ; D2Common.dll!#10794 -> D2COMMON_10203_PATH_SetRotateFlag
    //   0x6FB795AB: call     0x6fabc268 ; D2Common.dll!#10299 -> UNITS_SetAnimationSpeed
    //   0x6FB795B1: cmp      edi, ebx
    //   0x6FB795C1: ret      0x1c
    return 1;
}

// 1.12a: 0x6FB795D0..0x6FB796B4 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB795D0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[90]
    // Callees: D2Common.dll!#10366 -> UNITS_GetRoom, D2Common.dll!#10438 -> DUNGEON_IsRoomInTown, 0x6fae6c10, 0x6fae2150, 0x6fae1fa0, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fb791c0
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Play skill start/do sound via SfxPlayUnitSound (0x6FAE1FA0)
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB795DB: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB795E2: cmp      ebp, dword ptr [eax + 0xba0]
    //   0x6FB795F2: imul     esi, esi, 0x23c
    //   0x6FB79601: ret      4
    //   0x6FB79606: call     0x6fabc1ba ; D2Common.dll!#10366 -> UNITS_GetRoom
    //   0x6FB79610: call     0x6fabc71e ; D2Common.dll!#10438 -> DUNGEON_IsRoomInTown
    //   0x6FB79633: call     0x6fae6c10
    //   0x6FB79643: call     0x6fae2150
    //   0x6FB79652: ret      4
    //   0x6FB79664: call     0x6fae1fa0
    //   0x6FB79677: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB79689: call     0x6fb791c0
    //   0x6FB79693: call     0x6fb791c0
    //   0x6FB7969D: call     0x6fb791c0
    //   0x6FB796B1: ret      4
    return 1;
}

// 1.12a: 0x6FB796C0..0x6FB79767 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB796C0(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[69]
    // Callees: 0x6fb00830, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fb79440
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB796D0: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB796D7: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FB796E7: imul     esi, esi, 0x23c
    //   0x6FB796F4: call     0x6fb00830
    //   0x6FB79701: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB79709: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB7972B: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB7973B: ret      4
    //   0x6FB79751: call     0x6fb79440
    //   0x6FB7975B: ret      4
    //   0x6FB79764: ret      4
    return 1;
}

// 1.12a: 0x6FB79770..0x6FB79817 (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB79770(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[68]
    // Callees: 0x6fb00830, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fb79440
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB79780: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB79787: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FB79797: imul     esi, esi, 0x23c
    //   0x6FB797A4: call     0x6fb00830
    //   0x6FB797B1: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB797B9: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB797DB: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB797EB: ret      4
    //   0x6FB79801: call     0x6fb79440
    //   0x6FB7980B: ret      4
    //   0x6FB79814: ret      4
    return 1;
}

// 1.12a: 0x6FB79820..0x6FB7991E (__fastcall, ret 0x4)
int32_t __fastcall sub_6FB79820(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // Client Skill Do Function: gpfnCltDoFunc[67]
    // Callees: 0x6fb00830, D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula, 0x6fb79440, 0x6fb798c8, 0x6fb7b014, 0x6fb7afc2
    if (nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return 0;
    }
    D2SkillsTxt* pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[nSkillId];
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    int32_t nCalcValue = SKILLS_EvaluateSkillFormula(pUnit, 0, nSkillId, nSkillLevel);
    (void)nCalcValue;
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB79830: mov      eax, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB79837: cmp      edi, dword ptr [eax + 0xba0]
    //   0x6FB79847: imul     esi, esi, 0x23c
    //   0x6FB79854: call     0x6fb00830
    //   0x6FB79861: mov      ecx, dword ptr [0x6fb7e454] ; IAT:D2Common.dll!sgptDataTables -> sgptDataTables
    //   0x6FB79869: cmp      eax, dword ptr [edx + 0xb6c]
    //   0x6FB7988B: call     0x6fabc280 ; D2Common.dll!#10111 -> SKILLS_EvaluateSkillFormula
    //   0x6FB7989B: ret      4
    //   0x6FB798B1: call     0x6fb79440
    //   0x6FB798BB: ret      4
    //   0x6FB798C4: ret      4
    //   0x6FB798C7: ret
    //   0x6FB798FF: ret
    //   0x6FB79900: call     0x6fb798c8
    //   0x6FB79905: call     0x6fb7b014
    //   0x6FB7990A: cmp      dword ptr [esp + 4], 0
    //   0x6FB79916: call     0x6fb7afc2
    //   0x6FB7991D: ret
    return 1;
}

// 1.12a: 0x6FB79920..0x6FB79CEF (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB79920(void)
{
    // Callees: 0x6fb79920
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB79942: ret
    //   0x6FB79946: cmp      byte ptr [eax + 0x6fbaa420], 0
    //   0x6FB79951: ret
    //   0x6FB7995D: cmp      eax, 0x7fff
    //   0x6FB79987: cmp      eax, 1
    //   0x6FB799A2: ret
    //   0x6FB799B9: ret
    //   0x6FB799C6: ret
    //   0x6FB799F7: cmp      eax, 0x7fff
    //   0x6FB79A36: ret
    //   0x6FB79A49: ret
    //   0x6FB79A54: ret
    //   0x6FB79A5F: ret
    //   0x6FB79A65: ret
    //   0x6FB79A6B: ret
    //   0x6FB79A71: ret
    //   0x6FB79A7F: call     0x6fb79920
    //   0x6FB79A8D: ret
    //   0x6FB79A9A: call     0x6fb79920
    //   0x6FB79AA8: ret
    //   0x6FB79ABD: call     0x6fb79920
    //   0x6FB79AC9: ret
    //   0x6FB79AD1: call     0x6fb79920
    //   0x6FB79AD9: ret
    //   0x6FB79AE1: call     0x6fb79920
    //   0x6FB79AED: ret
    //   0x6FB79AF5: call     0x6fb79920
    //   0x6FB79AFD: ret
    //   0x6FB79B0D: call     0x6fb79920
    //   0x6FB79B1D: ret
    //   0x6FB79B2C: call     0x6fb79920
    //   0x6FB79B3C: ret
    //   0x6FB79B53: call     0x6fb79920
    //   0x6FB79B61: ret
    //   0x6FB79B6B: call     0x6fb79920
    //   0x6FB79B75: ret
    //   0x6FB79B7F: call     0x6fb79920
    //   0x6FB79B8D: ret
    //   0x6FB79B97: call     0x6fb79920
    //   0x6FB79BA1: ret
    //   0x6FB79BB1: call     0x6fb79920
    //   0x6FB79BC1: ret
    //   0x6FB79BD0: call     0x6fb79920
    //   0x6FB79BE0: ret
    //   0x6FB79BF7: call     0x6fb79920
    //   0x6FB79C05: ret
    //   0x6FB79C0F: call     0x6fb79920
    //   0x6FB79C19: ret
    //   0x6FB79C23: call     0x6fb79920
    //   0x6FB79C31: ret
    //   0x6FB79C3B: call     0x6fb79920
    //   0x6FB79C45: ret
    //   0x6FB79C55: call     0x6fb79920
    //   0x6FB79C65: ret
    //   0x6FB79C74: call     0x6fb79920
    //   0x6FB79C84: ret
    //   0x6FB79C9B: call     0x6fb79920
    //   0x6FB79CA9: ret
    //   0x6FB79CB3: call     0x6fb79920
    //   0x6FB79CBD: ret
    //   0x6FB79CC7: call     0x6fb79920
    //   0x6FB79CD5: ret
    //   0x6FB79CDF: call     0x6fb79920
    //   0x6FB79CE9: ret
    return 1;
}

// 1.12a: 0x6FB79CF0..0x6FB79CFF (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB79CF0(void)
{
    // Callees: 0x6fb79920
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB79CF9: call     0x6fb79920
    return 1;
}

// 1.12a: 0x6FB79D00..0x6FB79D7F (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB79D00(void)
{
    // Callees: 0x6fb79920
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB79D09: ret
    //   0x6FB79D18: call     0x6fb79920
    //   0x6FB79D28: ret
    //   0x6FB79D3F: call     0x6fb79920
    //   0x6FB79D4D: ret
    //   0x6FB79D57: call     0x6fb79920
    //   0x6FB79D61: ret
    //   0x6FB79D6B: call     0x6fb79920
    //   0x6FB79D79: ret
    return 1;
}

// 1.12a: 0x6FB79D80..0x6FB7A721 (__stdcall, ret 0x8)
int32_t __stdcall sub_6FB79D80(int32_t a1, int32_t a2)
{
    // Callees: 0x6fb79920, 0x6fb79ed6, 0x6fb79ee9, 0x6fb7a126, 0x6fb7a3de, 0x6fb7a32c, 0x6fb7a5e4
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB79D83: call     0x6fb79920
    //   0x6FB79D8D: ret
    //   0x6FB79D9D: call     0x6fb79920
    //   0x6FB79DAD: ret
    //   0x6FB79DBC: call     0x6fb79920
    //   0x6FB79DCC: ret
    //   0x6FB79DE3: call     0x6fb79920
    //   0x6FB79DF1: ret
    //   0x6FB79DFB: call     0x6fb79920
    //   0x6FB79E05: ret
    //   0x6FB79E0F: call     0x6fb79920
    //   0x6FB79E1D: ret
    //   0x6FB79E27: call     0x6fb79920
    //   0x6FB79E31: ret
    //   0x6FB79E41: call     0x6fb79920
    //   0x6FB79E51: ret
    //   0x6FB79E60: call     0x6fb79920
    //   0x6FB79E70: ret
    //   0x6FB79E87: call     0x6fb79920
    //   0x6FB79E95: ret
    //   0x6FB79E9F: call     0x6fb79920
    //   0x6FB79EA9: ret
    //   0x6FB79EB3: call     0x6fb79920
    //   0x6FB79EC1: ret
    //   0x6FB79ECB: call     0x6fb79920
    //   0x6FB79ED5: ret
    //   0x6FB79EE0: call     0x6fb79920
    //   0x6FB79EE8: ret
    //   0x6FB79EF3: call     0x6fb79920
    //   0x6FB79EFB: ret
    //   0x6FB79F06: cmp      eax, 0x7f800000
    //   0x6FB79F1A: call     0x6fb79ed6
    //   0x6FB79F20: ret      4
    //   0x6FB79F2F: call     0x6fb79ed6
    //   0x6FB79F3D: ret      4
    //   0x6FB79F45: ret      4
    //   0x6FB79F52: cmp      eax, 0x7ff00000
    //   0x6FB79F66: call     0x6fb79ed6
    //   0x6FB79F6C: ret      8
    //   0x6FB79F7B: call     0x6fb79ed6
    //   0x6FB79F89: ret      8
    //   0x6FB79F91: ret      8
    //   0x6FB79FA2: call     0x6fb79ed6
    //   0x6FB79FA8: ret      4
    //   0x6FB79FB7: call     0x6fb79ed6
    //   0x6FB79FC5: ret      4
    //   0x6FB79FD6: call     0x6fb79ed6
    //   0x6FB79FDC: ret      4
    //   0x6FB79FEB: call     0x6fb79ed6
    //   0x6FB79FF9: ret      4
    //   0x6FB7A006: cmp      eax, 0x7f800000
    //   0x6FB7A01A: call     0x6fb79ee9
    //   0x6FB7A020: ret      4
    //   0x6FB7A02F: call     0x6fb79ee9
    //   0x6FB7A03D: ret      4
    //   0x6FB7A045: ret      4
    //   0x6FB7A052: cmp      eax, 0x7ff00000
    //   0x6FB7A066: call     0x6fb79ee9
    //   0x6FB7A06C: ret      8
    //   0x6FB7A07B: call     0x6fb79ee9
    //   0x6FB7A089: ret      8
    //   0x6FB7A091: ret      8
    //   0x6FB7A0A2: call     0x6fb79ee9
    //   0x6FB7A0A8: ret      4
    //   0x6FB7A0B7: call     0x6fb79ee9
    //   0x6FB7A0C5: ret      4
    //   0x6FB7A0D6: call     0x6fb79ee9
    //   0x6FB7A0DC: ret      4
    //   0x6FB7A0EB: call     0x6fb79ee9
    //   0x6FB7A0F9: ret      4
    //   0x6FB7A107: call     0x6fb79920
    //   0x6FB7A110: ret
    //   0x6FB7A11C: call     0x6fb79920
    //   0x6FB7A125: ret
    //   0x6FB7A143: cmp      byte ptr [eax + 0x6fbaa43c], 0
    //   0x6FB7A159: cmp      eax, 0x7fff0000
    //   0x6FB7A173: cmp      eax, 0x7fff0000
    //   0x6FB7A32B: ret
    //   0x6FB7A344: call     0x6fb7a126
    //   0x6FB7A34D: ret
    //   0x6FB7A38A: cmp      eax, 0x7fbe
    //   0x6FB7A3CD: call     0x6fb7a126
    //   0x6FB7A3D6: ret
    //   0x6FB7A3DD: ret
    //   0x6FB7A3FB: cmp      byte ptr [eax + 0x6fbaa43c], 0
    //   0x6FB7A411: cmp      eax, 0x7fff0000
    //   0x6FB7A42B: cmp      eax, 0x7fff0000
    //   0x6FB7A5E3: ret
    //   0x6FB7A5FF: call     0x6fb7a3de
    //   0x6FB7A608: ret
    //   0x6FB7A645: cmp      eax, 0x7fbe
    //   0x6FB7A688: call     0x6fb7a3de
    //   0x6FB7A691: ret
    //   0x6FB7A698: ret
    //   0x6FB7A699: call     0x6fb7a32c
    //   0x6FB7A69E: ret
    //   0x6FB7A69F: call     0x6fb7a5e4
    //   0x6FB7A6A4: ret
    //   0x6FB7A6A7: ret
    //   0x6FB7A6AA: ret
    //   0x6FB7A720: ret
    return 1;
}

// 1.12a: 0x6FB7A730..0x6FB7B949 (__stdcall, ret 0x10)
// 1.12a: 0x6FB7A9E0 (internal entry point sub_6FB7A9E0)
// 1.12a: 0x6FB7AB10 (internal entry point sub_6FB7AB10)
// 1.12a: 0x6FB7B3A0 (internal entry point sub_6FB7B3A0)
// 1.12a: 0x6FB7B910 (internal entry point sub_6FB7B910)
int32_t __stdcall sub_6FB7A730(int32_t a1, int32_t a2, int32_t a3, int32_t a4)
{
    // Referenced strings: 'e+000', 'KERNEL32', 'IsProcessorFeaturePresent'
    // Callees: KERNEL32.dll!RtlUnwind -> RtlUnwind, 0x6fb7b845, 0x6fab5754, 0x6fb7a811, 0x6fb7a8d4, 0x6fb7b91c, 0x6fb7b950, 0x6fab61a5
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7A7B7: cmp      edx, dword ptr [esp + 0x14]
    //   0x6FB7A7BF: cmp      eax, dword ptr [esp + 0x10]
    //   0x6FB7A7D7: ret      0x10
    //   0x6FB7A807: ret      8
    //   0x6FB7A835: call     0x6fabbe1e ; KERNEL32.dll!RtlUnwind -> RtlUnwind
    //   0x6FB7A860: ret      8
    //   0x6FB7A884: call     0x6fb7b845
    //   0x6FB7A898: ret
    //   0x6FB7A8A1: cmp      ecx, dword ptr [0x6fb8b390]
    //   0x6FB7A8B4: ret
    //   0x6FB7A8CB: call     0x6fb7b845
    //   0x6FB7A8D3: ret
    //   0x6FB7A8DB: cmp      dword ptr [ebp + 8], 0x123
    //   0x6FB7A961: call     0x6fab5754
    //   0x6FB7A966: call     dword ptr [eax + 0x74]
    //   0x6FB7A96F: cmp      dword ptr [ebp - 4], 0
    //   0x6FB7A99A: ret
    //   0x6FB7A9A7: cmp      eax, dword ptr [0x6fb8b390]
    //   0x6FB7AA02: call     0x6fb7b845
    //   0x6FB7AA0D: cmp      dword ptr [eax + 0x24], 0
    //   0x6FB7AA19: call     0x6fb7a811
    //   0x6FB7AA31: call     0x6fb7a8d4
    //   0x6FB7AA4C: ret
    //   0x6FB7AA51: cmp      dword ptr [ebp + 0xc], 0
    //   0x6FB7AA6B: cmp      esi, -1
    //   0x6FB7AA70: call     0x6fb7b91c
    //   0x6FB7AA7F: cmp      dword ptr [eax + 4], ecx
    //   0x6FB7AA84: cmp      ecx, dword ptr [eax + 8]
    //   0x6FB7AA89: cmp      esi, -1
    //   0x6FB7AA9A: cmp      dword ptr [ebp + 0xc], 0
    //   0x6FB7AAAE: cmp      eax, dword ptr [edi + 0xc]
    //   0x6FB7AAB3: cmp      esi, eax
    //   0x6FB7AAB7: call     0x6fb7b91c
    //   0x6FB7AAC6: ret
    //   0x6FB7AAD2: call     0x6fab5754
    //   0x6FB7AAE0: call     0x6fab5754
    //   0x6FB7AAEE: ret
    //   0x6FB7AAEF: call     0x6fab5754
    //   0x6FB7AAFE: cmp      ecx, dword ptr [esp + 4]
    //   0x6FB7AB0C: ret
    //   0x6FB7AB0F: ret
    //   0x6FB7AB11: call     0x6fab5754
    //   0x6FB7AB1A: cmp      esi, dword ptr [eax + 0x88]
    //   0x6FB7AB22: call     0x6fab5754
    //   0x6FB7AB31: ret
    //   0x6FB7AB32: call     0x6fab5754
    //   0x6FB7AB42: cmp      esi, ecx
    //   0x6FB7AB48: cmp      dword ptr [eax + 4], 0
    //   0x6FB7AB5B: ret
    //   0x6FB7ABA1: call     0x6fb7b950
    //   0x6FB7ABB4: ret
    //   0x6FB7ABBE: call     0x6fab61a5
    //   0x6FB7ABC3: cmp      eax, 0x65
    //   0x6FB7ABCD: call     0x6fab33ab
    //   0x6FB7ABF0: ret
    //   0x6FB7ABFE: cmp      cl, bl
    //   0x6FB7AC12: cmp      cl, 0x65
    //   0x6FB7AC17: cmp      cl, 0x45
    //   0x6FB7AC26: cmp      byte ptr [eax], 0x30
    //   0x6FB7AC2B: cmp      byte ptr [eax], bl
    //   0x6FB7AC3B: ret
    //   0x6FB7AC52: ret
    //   0x6FB7AC55: ret
    //   0x6FB7AC5B: cmp      dword ptr [ebp + 8], 0
    //   0x6FB7AC68: call     0x6fb7bcc0
    //   0x6FB7AC7E: ret
    //   0x6FB7AC83: call     0x6fb7bd05
    //   0x6FB7AC93: ret
    //   0x6FB7AC9C: call     0x6fab8710
    //   0x6FB7ACA7: call     0x6fab36b0
    //   0x6FB7ACB0: ret
    //   0x6FB7ACB4: cmp      byte ptr [ebp + 0x10], 0
    //   0x6FB7ACBF: cmp      dword ptr [ebp + 8], eax
    //   0x6FB7ACC8: cmp      dword ptr [esi], 0x2d
    //   0x6FB7ACD4: call     0x6fb7ac94
    //   0x6FB7ACDA: cmp      dword ptr [esi], 0x2d
    //   0x6FB7ACE7: cmp      dword ptr [ebp + 8], 0
    //   0x6FB7AD00: cmp      byte ptr [ebp + 0x10], cl
    //   0x6FB7AD03: push     0x6fb875a8 ; STR:'e+000'
    //   0x6FB7AD11: call     0x6fab5a30
    //   0x6FB7AD16: cmp      dword ptr [ebp + 0xc], 0
    //   0x6FB7AD27: cmp      byte ptr [eax], 0x30
    //   0x6FB7AD38: cmp      eax, 0x64
    //   0x6FB7AD48: cmp      eax, 0xa
    //   0x6FB7AD5E: ret
    //   0x6FB7AD84: call     0x6fb7be7b
    //   0x6FB7AD96: cmp      dword ptr [ebp - 0x2c], 0x2d
    //   0x6FB7ADA9: call     0x6fb7bd4a
    //   0x6FB7ADB7: call     0x6fb7acb1
    //   0x6FB7ADC8: call     0x6fab62f5
    //   0x6FB7ADCE: ret
    //   0x6FB7ADDA: cmp      byte ptr [ebp + 0x10], 0
    //   0x6FB7ADE1: cmp      eax, dword ptr [ebp + 0xc]
    //   0x6FB7ADE8: cmp      dword ptr [esi], 0x2d
    //   0x6FB7ADFC: cmp      dword ptr [esi], 0x2d
    //   0x6FB7AE14: call     0x6fb7ac94
    //   0x6FB7AE21: cmp      dword ptr [ebp + 0xc], 0
    //   0x6FB7AE29: call     0x6fb7ac94
    //   0x6FB7AE3F: cmp      byte ptr [ebp + 0x10], 0
    //   0x6FB7AE45: cmp      dword ptr [ebp + 0xc], esi
    //   0x6FB7AE52: call     0x6fb7ac94
    //   0x6FB7AE5B: call     0x6faba5c0
    //   0x6FB7AE6A: ret
    //   0x6FB7AE90: call     0x6fb7be7b
    //   0x6FB7AEA4: cmp      dword ptr [ebp - 0x2c], 0x2d
    //   0x6FB7AEAE: call     0x6fb7bd4a
    //   0x6FB7AEBA: call     0x6fb7adcf
    //   0x6FB7AECB: call     0x6fab62f5
    //   0x6FB7AED1: ret
    //   0x6FB7AEF8: call     0x6fb7be7b
    //   0x6FB7AF03: cmp      dword ptr [ebp - 0x2c], 0x2d
    //   0x6FB7AF16: call     0x6fb7bd4a
    //   0x6FB7AF22: cmp      esi, eax
    //   0x6FB7AF27: cmp      eax, -4
    //   0x6FB7AF2C: cmp      eax, dword ptr [ebp + 0x10]
    //   0x6FB7AF48: call     0x6fb7adcf
    //   0x6FB7AF5A: call     0x6fb7acb1
    //   0x6FB7AF6A: call     0x6fab62f5
    //   0x6FB7AF70: ret
    //   0x6FB7AF74: cmp      dword ptr [ebp + 0x10], 0x65
    //   0x6FB7AF7A: cmp      dword ptr [ebp + 0x10], 0x45
    //   0x6FB7AF80: cmp      dword ptr [ebp + 0x10], 0x66
    //   0x6FB7AF8F: call     0x6fb7ae6b
    //   0x6FB7AF98: ret
    //   0x6FB7AFA5: call     0x6fb7aed2
    //   0x6FB7AFB8: call     0x6fb7ad5f
    //   0x6FB7AFC1: ret
    //   0x6FB7AFCC: call     0x6fb7c045
    //   0x6FB7AFD3: ret
    //   0x6FB7B00F: ret
    //   0x6FB7B013: ret
    //   0x6FB7B014: push     0x6fb875e4 ; STR:'KERNEL32'
    //   0x6FB7B019: call     dword ptr [0x6fb7efa0] ; IAT:KERNEL32.dll!GetModuleHandleA -> GetModuleHandleA
    //   0x6FB7B023: push     0x6fb875c8 ; STR:'IsProcessorFeaturePresent'
    //   0x6FB7B029: call     dword ptr [0x6fb7efa8] ; IAT:KERNEL32.dll!GetProcAddress -> GetProcAddress
    //   0x6FB7B035: call     eax
    //   0x6FB7B037: ret
    //   0x6FB7B047: cmp      byte ptr [edx], 0
    //   0x6FB7B04F: cmp      eax, ecx
    //   0x6FB7B058: call     0x6fab5450
    //   0x6FB7B087: ret
    //   0x6FB7B08B: ret
    //   0x6FB7B08E: cmp      dword ptr [eax], 0xe06d7363
    //   0x6FB7B098: ret
    //   0x6FB7B099: call     0x6fab5754
    //   0x6FB7B0B1: call     0x6fab4920
    //   0x6FB7B0BF: call     0x6fab5754
    //   0x6FB7B0D2: cmp      esi, dword ptr [ebp + 0x14]
    //   0x6FB7B0D7: cmp      esi, -1
    //   0x6FB7B0DC: cmp      esi, dword ptr [edi + 4]
    //   0x6FB7B0E1: call     0x6fb7b91c
    //   0x6FB7B0FC: cmp      dword ptr [ecx + 4], 0
    //   0x6FB7B112: call     0x6fb7b950
    //   0x6FB7B120: call     0x6fb7b08c
    //   0x6FB7B125: ret
    //   0x6FB7B13F: call     0x6fb7b15d
    //   0x6FB7B144: cmp      esi, dword ptr [ebp + 0x14]
    //   0x6FB7B149: call     0x6fb7b91c
    //   0x6FB7B151: call     0x6fab495b
    //   0x6FB7B156: ret
    //   0x6FB7B15D: call     0x6fab5754
    //   0x6FB7B162: cmp      dword ptr [eax + 0x84], 0
    //   0x6FB7B16B: call     0x6fab5754
    //   0x6FB7B177: ret
    //   0x6FB7B17F: call     0x6fab4920
    //   0x6FB7B19D: call     0x6fb7a80a
    //   0x6FB7B1A6: call     0x6fab495b
    //   0x6FB7B1AB: ret
    //   0x6FB7B1AE: cmp      byte ptr [ebp + 0xc], al
    //   0x6FB7B1B4: ret
    //   0x6FB7B1C4: cmp      dword ptr [ecx + 4], 0
    //   0x6FB7B1DB: ret
    //   0x6FB7B1E3: call     0x6fab4920
    //   0x6FB7B204: call     0x6fb7aac7
    //   0x6FB7B20E: call     0x6fab5754
    //   0x6FB7B219: call     0x6fab5754
    //   0x6FB7B227: call     0x6fab5754
    //   0x6FB7B22F: call     0x6fab5754
    //   0x6FB7B253: call     0x6fb7ab5c
    //   0x6FB7B272: cmp      dword ptr [eax], 0xe06d7363
    //   0x6FB7B27D: cmp      dword ptr [eax + 0x10], 3
    //   0x6FB7B286: cmp      dword ptr [eax + 0x14], 0x19930520
    //   0x6FB7B292: cmp      dword ptr [eax + 0x14], 0x19930521
    //   0x6FB7B29E: cmp      dword ptr [eax + 0x1c], 0
    //   0x6FB7B2B5: ret
    //   0x6FB7B2D6: cmp      edx, dword ptr [ecx + 0xc]
    //   0x6FB7B2E7: cmp      eax, ebx
    //   0x6FB7B2EB: cmp      eax, dword ptr [esi + 8]
    //   0x6FB7B305: call     0x6fb7b0aa
    //   0x6FB7B31A: call     0x6fb7b331
    //   0x6FB7B322: call     0x6fab495b
    //   0x6FB7B327: ret
    //   0x6FB7B33A: call     0x6fb7ab10
    //   0x6FB7B340: call     0x6fab5754
    //   0x6FB7B34B: call     0x6fab5754
    //   0x6FB7B359: cmp      dword ptr [esi], 0xe06d7363
    //   0x6FB7B361: cmp      dword ptr [esi + 0x10], 3
    //   0x6FB7B36A: cmp      eax, 0x19930520
    //   0x6FB7B371: cmp      eax, 0x19930521
    //   0x6FB7B378: cmp      dword ptr [ebp - 0x48], 0
    //   0x6FB7B37E: cmp      dword ptr [ebp - 0x20], 0
    //   0x6FB7B387: call     0x6fb7aaef
    //   0x6FB7B391: call     0x6fab91ca
    //   0x6FB7B398: call     0x6fb7b178
    //   0x6FB7B39F: ret
    //   0x6FB7B3A7: call     0x6fab4920
    //   0x6FB7B3C1: cmp      byte ptr [ecx + 8], 0
    //   0x6FB7B3F3: call     0x6fb7c0c9
    //   0x6FB7B405: call     0x6fb7c0e5
    //   0x6FB7B41C: call     0x6fb7b1bd
    //   0x6FB7B42D: call     0x6fb7c0c9
    //   0x6FB7B43F: call     0x6fb7c0e5
    //   0x6FB7B455: call     0x6fab36b0
    //   0x6FB7B45D: cmp      dword ptr [esi + 0x14], 4
    //   0x6FB7B473: cmp      dword ptr [esi + 0x18], 0
    //   0x6FB7B479: call     0x6fb7c0c9
    //   0x6FB7B487: call     0x6fb7c0e5
    //   0x6FB7B49B: call     0x6fb7b1bd
    //   0x6FB7B4A2: call     0x6fab36b0
    //   0x6FB7B4AC: call     0x6fb7c0c9
    //   0x6FB7B4BA: call     0x6fb7c0e5
    //   0x6FB7B4C8: call     0x6fb7c101
    //   0x6FB7B4DF: call     0x6fb7b1bd
    //   0x6FB7B4E9: call     0x6fb7a80a
    //   0x6FB7B4F0: call     0x6fb7b1bd
    //   0x6FB7B4FA: call     0x6fb7a80a
    //   0x6FB7B501: call     0x6fb7b91c
    //   0x6FB7B50A: call     0x6fab495b
    //   0x6FB7B50F: ret
    //   0x6FB7B513: ret
    //   0x6FB7B529: call     0x6fb7b3a0
    //   0x6FB7B530: cmp      dword ptr [ebp + 0x1c], 0
    //   0x6FB7B53F: call     0x6fb7a811
    //   0x6FB7B54D: call     0x6fb7b0aa
    //   0x6FB7B56E: call     0x6fb7b1dc
    //   0x6FB7B57C: call     0x6fb7a7da
    //   0x6FB7B582: ret
    //   0x6FB7B58C: cmp      dword ptr [esi], 0x80000003
    //   0x6FB7B598: call     0x6fab5754
    //   0x6FB7B59D: cmp      dword ptr [eax + 0x74], 0
    //   0x6FB7B5B6: call     0x6fb7a8d4
    //   0x6FB7B5D5: call     0x6fb7aa4d
    //   0x6FB7B5E2: cmp      eax, dword ptr [ebp - 8]
    //   0x6FB7B5E8: cmp      esi, dword ptr [edi]
    //   0x6FB7B5EC: cmp      esi, dword ptr [edi + 4]
    //   0x6FB7B603: cmp      byte ptr [ecx + 8], 0
    //   0x6FB7B623: call     0x6fb7b51c
    //   0x6FB7B637: cmp      eax, dword ptr [ebp - 8]
    //   0x6FB7B640: ret
    //   0x6FB7B64D: cmp      eax, -1
    //   0x6FB7B65C: cmp      eax, dword ptr [ecx + 4]
    //   0x6FB7B661: call     0x6fb7b91c
    //   0x6FB7B66A: cmp      dword ptr [ebx], 0xe06d7363
    //   0x6FB7B678: cmp      dword ptr [ebx + 0x10], 3
    //   0x6FB7B68A: cmp      eax, edi
    //   0x6FB7B68E: cmp      eax, 0x19930521
    //   0x6FB7B695: cmp      dword ptr [ebx + 0x1c], 0
    //   0x6FB7B69B: call     0x6fab5754
    //   0x6FB7B6A0: cmp      dword ptr [eax + 0x7c], 0
    //   0x6FB7B6AA: call     0x6fab5754
    //   0x6FB7B6B5: call     0x6fab5754
    //   0x6FB7B6CA: call     0x6fb7c0c9
    //   0x6FB7B6D5: call     0x6fb7b91c
    //   0x6FB7B6DA: cmp      dword ptr [esi], 0xe06d7363
    //   0x6FB7B6E9: cmp      dword ptr [eax + 0x10], 3
    //   0x6FB7B6F2: cmp      eax, edi
    //   0x6FB7B6F6: cmp      eax, 0x19930521
    //   0x6FB7B700: cmp      dword ptr [eax + 0x1c], 0
    //   0x6FB7B706: call     0x6fb7b91c
    //   0x6FB7B70E: cmp      dword ptr [ebx], 0xe06d7363
    //   0x6FB7B71A: cmp      dword ptr [ebx + 0x10], 3
    //   0x6FB7B727: cmp      eax, edi
    //   0x6FB7B72B: cmp      eax, 0x19930521
    //   0x6FB7B748: call     0x6fb7aa4d
    //   0x6FB7B753: cmp      ecx, dword ptr [ebp - 0x20]
    //   0x6FB7B764: cmp      dword ptr [eax], esi
    //   0x6FB7B76C: cmp      esi, dword ptr [eax + 4]
    //   0x6FB7B79E: call     0x6fb7b03d
    //   0x6FB7B7AF: cmp      dword ptr [ebp - 0x14], eax
    //   0x6FB7B7BD: cmp      dword ptr [ebp - 0x18], 0
    //   0x6FB7B7E0: call     0x6fb7b51c
    //   0x6FB7B7F6: cmp      ecx, dword ptr [ebp - 0x20]
    //   0x6FB7B802: cmp      byte ptr [ebp + 0x1c], 0
    //   0x6FB7B80B: call     0x6fb7b178
    //   0x6FB7B816: ret
    //   0x6FB7B81A: cmp      byte ptr [ebp + 0x1c], 0
    //   0x6FB7B836: call     0x6fb7b583
    //   0x6FB7B859: cmp      eax, edi
    //   0x6FB7B85D: call     0x6fb7b91c
    //   0x6FB7B86B: cmp      dword ptr [esi + 4], 0
    //   0x6FB7B871: cmp      dword ptr [ebp + 0x1c], 0
    //   0x6FB7B880: call     0x6fb7b0aa
    //   0x6FB7B88A: cmp      dword ptr [esi + 0xc], 0
    //   0x6FB7B890: cmp      dword ptr [eax], 0xe06d7363
    //   0x6FB7B898: cmp      dword ptr [eax + 0x14], edi
    //   0x6FB7B8BD: call     ecx
    //   0x6FB7B8D8: call     0x6fb7b641
    //   0x6FB7B8E6: ret
    //   0x6FB7B8EE: call     0x6fab4920
    //   0x6FB7B8F3: call     0x6fab5754
    //   0x6FB7B8F8: cmp      dword ptr [eax + 0x6c], 0
    //   0x6FB7B902: call     0x6fab5754
    //   0x6FB7B907: call     dword ptr [eax + 0x6c]
    //   0x6FB7B90F: ret
    //   0x6FB7B923: call     0x6fab4920
    //   0x6FB7B935: call     eax
    //   0x6FB7B93C: ret
    return 1;
}

// 1.12a: 0x6FB7B950..0x6FB7C76F (__fastcall, ret 0xC)
// 1.12a: 0x6FB7BC40 (internal entry point sub_6FB7BC40)
// 1.12a: 0x6FB7BCC0 (internal entry point sub_6FB7BCC0)
// 1.12a: 0x6FB7C1B0 (internal entry point sub_6FB7C1B0)
// 1.12a: 0x6FB7C530 (internal entry point sub_6FB7C530)
int32_t __fastcall sub_6FB7B950(D2UnitStrc* pUnit, int32_t a1, int32_t a2, int32_t a3)
{
    // Callees: 0x6fab91ed, 0x6fb7c131, 0x6fb7b99c, 0x6fb7b9ce, 0x6fb7baa8, 0x6fb7ba8d, 0x6fb7ba1b, 0x6fb7bac1
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7B96E: call     0x6fab91ed
    //   0x6FB7B975: call     eax
    //   0x6FB7B982: cmp      ecx, 0x100
    //   0x6FB7B990: call     0x6fab91ed
    //   0x6FB7B999: ret      0xc
    //   0x6FB7B9BD: ret
    //   0x6FB7B9BE: cmp      dword ptr [ecx + eax*4], 0
    //   0x6FB7B9C5: cmp      eax, 3
    //   0x6FB7B9CD: ret
    //   0x6FB7B9F1: call     0x6fb7c131
    //   0x6FB7BA08: call     0x6fb7c131
    //   0x6FB7BA1A: ret
    //   0x6FB7BA4E: call     0x6fb7b99c
    //   0x6FB7BA5D: call     0x6fb7b9ce
    //   0x6FB7BA78: cmp      ebx, ecx
    //   0x6FB7BA8C: ret
    //   0x6FB7BAA7: ret
    //   0x6FB7BAAE: cmp      dword ptr [ecx + eax*4], 0
    //   0x6FB7BAB5: cmp      eax, 3
    //   0x6FB7BABD: ret
    //   0x6FB7BAC0: ret
    //   0x6FB7BB0C: cmp      eax, 3
    //   0x6FB7BB1F: cmp      eax, dword ptr [ebp - 8]
    //   0x6FB7BB3B: ret
    //   0x6FB7BB75: cmp      edi, 0xffffc001
    //   0x6FB7BB89: call     0x6fb7baa8
    //   0x6FB7BBA9: call     0x6fb7ba8d
    //   0x6FB7BBB5: call     0x6fb7ba1b
    //   0x6FB7BBCA: cmp      edi, ecx
    //   0x6FB7BBD8: cmp      edi, eax
    //   0x6FB7BBE8: call     0x6fb7ba8d
    //   0x6FB7BBF2: call     0x6fb7bac1
    //   0x6FB7BBFE: call     0x6fb7ba1b
    //   0x6FB7BC0C: call     0x6fb7bac1
    //   0x6FB7BC18: cmp      edi, dword ptr [esi]
    //   0x6FB7BC2F: call     0x6fb7bac1
    //   0x6FB7BC4D: call     0x6fb7bac1
    //   0x6FB7BC73: cmp      esi, 0x40
    //   0x6FB7BC85: cmp      esi, 0x20
    //   0x6FB7BC93: ret
    //   0x6FB7BCA1: call     0x6fb7bb3c
    //   0x6FB7BCA9: ret
    //   0x6FB7BCB7: call     0x6fb7bb3c
    //   0x6FB7BCBF: ret
    //   0x6FB7BCE6: call     0x6fb7c2f3
    //   0x6FB7BCF0: call     0x6fb7bc94
    //   0x6FB7BCFE: call     0x6fab62f5
    //   0x6FB7BD04: ret
    //   0x6FB7BD2B: call     0x6fb7c2f3
    //   0x6FB7BD35: call     0x6fb7bcaa
    //   0x6FB7BD43: call     0x6fab62f5
    //   0x6FB7BD49: ret
    //   0x6FB7BD8E: cmp      byte ptr [ecx], 0x35
    //   0x6FB7BD99: cmp      byte ptr [eax], 0x39
    //   0x6FB7BDA0: cmp      byte ptr [esi], 0x31
    //   0x6FB7BDAB: call     0x6fab8710
    //   0x6FB7BDB4: call     0x6fab36b0
    //   0x6FB7BDC0: ret
    //   0x6FB7BDFE: cmp      ebx, edi
    //   0x6FB7BE13: cmp      eax, ebx
    //   0x6FB7BE17: cmp      edx, ebx
    //   0x6FB7BE7A: ret
    //   0x6FB7BE9F: call     0x6fb7bdc1
    //   0x6FB7BEBA: call     0x6fb7c733
    //   0x6FB7BED7: call     0x6fab5a30
    //   0x6FB7BEEC: call     0x6fab62f5
    //   0x6FB7BEF2: ret
    //   0x6FB7BF42: cmp      ecx, 0x400
    //   0x6FB7BF4A: cmp      ecx, 0x800
    //   0x6FB7BF52: cmp      ecx, esi
    //   0x6FB7BF67: cmp      edx, ebp
    //   0x6FB7BF84: ret
    //   0x6FB7BFC9: cmp      ecx, 0x100
    //   0x6FB7BFD1: cmp      ecx, esi
    //   0x6FB7BFD5: cmp      ecx, edx
    //   0x6FB7BFF6: cmp      ecx, 0x10000
    //   0x6FB7C012: ret
    //   0x6FB7C01F: call     0x6fb7bef3
    //   0x6FB7C035: call     0x6fb7bf85
    //   0x6FB7C044: ret
    //   0x6FB7C053: call     0x6fb7c013
    //   0x6FB7C05A: ret
    //   0x6FB7C062: cmp      dword ptr [eax], 0xe06d7363
    //   0x6FB7C06A: cmp      dword ptr [eax + 0x10], 3
    //   0x6FB7C073: cmp      eax, 0x19930520
    //   0x6FB7C07A: cmp      eax, 0x19930521
    //   0x6FB7C081: call     0x6fb7b8e7
    //   0x6FB7C090: call     0x6fb7c101
    //   0x6FB7C09B: call     dword ptr [0x6fbd36f4]
    //   0x6FB7C0A6: ret      4
    //   0x6FB7C0AE: call     dword ptr [0x6fb7eebc] ; IAT:KERNEL32.dll!SetUnhandledExceptionFilter -> SetUnhandledExceptionFilter
    //   0x6FB7C0BB: ret
    //   0x6FB7C0C2: call     dword ptr [0x6fb7eebc] ; IAT:KERNEL32.dll!SetUnhandledExceptionFilter -> SetUnhandledExceptionFilter
    //   0x6FB7C0C8: ret
    //   0x6FB7C0D5: call     dword ptr [0x6fb7eeb8] ; IAT:KERNEL32.dll!IsBadReadPtr -> IsBadReadPtr
    //   0x6FB7C0E4: ret
    //   0x6FB7C0F1: call     dword ptr [0x6fb7eeb0] ; IAT:KERNEL32.dll!IsBadWritePtr -> IsBadWritePtr
    //   0x6FB7C100: ret
    //   0x6FB7C109: call     dword ptr [0x6fb7ef54] ; IAT:KERNEL32.dll!IsBadCodePtr -> IsBadCodePtr
    //   0x6FB7C118: ret
    //   0x6FB7C11B: call     0x6fab8ed2
    //   0x6FB7C122: call     0x6fb7c9fd
    //   0x6FB7C12B: call     0x6fab327e
    //   0x6FB7C13F: cmp      ecx, edx
    //   0x6FB7C143: cmp      ecx, esi
    //   0x6FB7C151: ret
    //   0x6FB7C161: call     0x6fb7c131
    //   0x6FB7C175: call     0x6fb7c131
    //   0x6FB7C18D: call     0x6fb7c131
    //   0x6FB7C1A5: call     0x6fb7c131
    //   0x6FB7C1AF: ret
    //   0x6FB7C1DD: ret
    //   0x6FB7C20A: ret
    //   0x6FB7C22A: cmp      eax, edx
    //   0x6FB7C24B: call     0x6fb7c1b0
    //   0x6FB7C251: call     0x6fb7c1b0
    //   0x6FB7C25B: call     0x6fb7c152
    //   0x6FB7C261: call     0x6fb7c1b0
    //   0x6FB7C27C: call     0x6fb7c152
    //   0x6FB7C28E: cmp      dword ptr [ebx + 8], edx
    //   0x6FB7C2B6: cmp      edi, edx
    //   0x6FB7C2CA: call     0x6fb7c1b0
    //   0x6FB7C2EC: call     0x6fab62f5
    //   0x6FB7C2F2: ret
    //   0x6FB7C342: cmp      al, 0x20
    //   0x6FB7C346: cmp      al, 9
    //   0x6FB7C34A: cmp      al, 0xa
    //   0x6FB7C34E: cmp      al, 0xd
    //   0x6FB7C359: cmp      ecx, 0xb
    //   0x6FB7C369: cmp      bl, 0x31
    //   0x6FB7C36E: cmp      bl, 0x39
    //   0x6FB7C37A: cmp      bl, byte ptr [0x6fb8ba24]
    //   0x6FB7C3BC: cmp      bl, 0x31
    //   0x6FB7C3C4: cmp      bl, 0x39
    //   0x6FB7C3C9: cmp      bl, byte ptr [0x6fb8ba24]
    //   0x6FB7C3D8: cmp      bl, 0x2b
    //   0x6FB7C3DD: cmp      bl, 0x2d
    //   0x6FB7C3E2: cmp      bl, 0x30
    //   0x6FB7C3EB: cmp      bl, 0x43
    //   0x6FB7C3F4: cmp      bl, 0x45
    //   0x6FB7C3F9: cmp      bl, 0x63
    //   0x6FB7C402: cmp      bl, 0x65
    //   0x6FB7C41A: cmp      bl, 0x31
    //   0x6FB7C41F: cmp      bl, 0x39
    //   0x6FB7C428: cmp      bl, byte ptr [0x6fb8ba24]
    //   0x6FB7C434: cmp      bl, 0x30
    //   0x6FB7C44E: cmp      dword ptr [ebp - 0x38], 0x19
    //   0x6FB7C469: call     0x6fab33ab
    //   0x6FB7C473: cmp      bl, byte ptr [0x6fb8ba24]
    //   0x6FB7C483: cmp      dword ptr [ebp - 0x38], 0
    //   0x6FB7C497: cmp      bl, 0x30
    //   0x6FB7C49E: cmp      dword ptr [ebp - 0x38], 0x19
    //   0x6FB7C4B7: call     0x6fab33ab
    //   0x6FB7C4C1: cmp      bl, 0x2b
    //   0x6FB7C4CA: cmp      bl, 0x2d
    //   0x6FB7C4E3: call     0x6fab33ab
    //   0x6FB7C4F8: cmp      bl, 0x31
    //   0x6FB7C503: cmp      bl, 0x39
    //   0x6FB7C53C: cmp      bl, 0x30
    //   0x6FB7C541: cmp      bl, 0x31
    //   0x6FB7C546: cmp      bl, 0x39
    //   0x6FB7C54F: cmp      dword ptr [ebp - 0x44], 0
    //   0x6FB7C562: cmp      dword ptr [ebp - 0x38], eax
    //   0x6FB7C567: cmp      byte ptr [ebp - 9], 5
    //   0x6FB7C577: cmp      dword ptr [ebp - 0x38], 0
    //   0x6FB7C586: cmp      bl, 0x31
    //   0x6FB7C58B: cmp      bl, 0x39
    //   0x6FB7C599: cmp      bl, 0x30
    //   0x6FB7C5A3: cmp      dword ptr [ebp + 0x20], 0
    //   0x6FB7C5D3: cmp      ecx, 0xa
    //   0x6FB7C603: cmp      esi, 0x1450
    //   0x6FB7C612: call     0x6fab33ab
    //   0x6FB7C632: call     0x6fab33ab
    //   0x6FB7C64C: cmp      byte ptr [esi], 0
    //   0x6FB7C65C: call     0x6fb7c20b
    //   0x6FB7C669: cmp      dword ptr [ebp - 0x5c], ecx
    //   0x6FB7C673: cmp      dword ptr [ebp - 0x58], ecx
    //   0x6FB7C67B: cmp      dword ptr [ebp - 0x54], ecx
    //   0x6FB7C683: cmp      eax, 0x1450
    //   0x6FB7C68A: cmp      eax, 0xffffebb0
    //   0x6FB7C699: call     0x6fb7cdac
    //   0x6FB7C6FB: call     0x6fab62f5
    //   0x6FB7C701: ret
    //   0x6FB7C703: imul     eax, ebx, 0xc3b96fb7
    return 1;
}

// 1.12a: 0x6FB7C770..0x6FB7C77F (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7C770(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB7C780..0x6FB7C78F (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7C780(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB7C790..0x6FB7CE3A (__usercall, ret 0x0)
// 1.12a: 0x6FB7CB30 (internal entry point sub_6FB7CB30)
int32_t __usercall sub_6FB7C790(D2UnitStrc* pUnit /*eax*/)
{
    // Referenced strings: '1#SNAN', '1#IND', '1#INF', '1#QNAN'
    // Callees: 0x6fab5a30, 0x6fb7cdac, 0x6fb7cb76, 0x6fb7c1b0, 0x6fb7c1de, 0x6fb7c152, 0x6fab62f5, 0x6fab4920
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7C7B9: cmp      dword ptr [ebp - 0x28], edi
    //   0x6FB7C7C2: cmp      dx, si
    //   0x6FB7C7CC: cmp      edi, eax
    //   0x6FB7C7D5: cmp      dword ptr [ebp - 0x28], 0
    //   0x6FB7C7E3: push     0x6fb87674 ; STR:'1#SNAN'
    //   0x6FB7C7EF: cmp      edi, 0xc0000000
    //   0x6FB7C7F7: cmp      dword ptr [ebp - 0x28], 0
    //   0x6FB7C7FD: push     0x6fb8766c ; STR:'1#IND'
    //   0x6FB7C804: cmp      edi, eax
    //   0x6FB7C808: cmp      dword ptr [ebp - 0x28], 0
    //   0x6FB7C80E: push     0x6fb87664 ; STR:'1#INF'
    //   0x6FB7C817: call     0x6fab5a30
    //   0x6FB7C82B: push     0x6fb8765c ; STR:'1#QNAN'
    //   0x6FB7C834: call     0x6fab5a30
    //   0x6FB7C844: imul     eax, eax, 0x4d10
    //   0x6FB7C85A: imul     ecx, ecx, 0x4d
    //   0x6FB7C880: call     0x6fb7cdac
    //   0x6FB7C888: cmp      word ptr [ebp - 6], 0x3fff
    //   0x6FB7C899: call     0x6fb7cb76
    //   0x6FB7C8BE: cmp      edi, 0x15
    //   0x6FB7C8E0: call     0x6fb7c1b0
    //   0x6FB7C8FD: call     0x6fb7c1de
    //   0x6FB7C923: call     0x6fb7c1b0
    //   0x6FB7C92C: call     0x6fb7c1b0
    //   0x6FB7C939: call     0x6fb7c152
    //   0x6FB7C942: call     0x6fb7c1b0
    //   0x6FB7C967: cmp      cl, 0x35
    //   0x6FB7C971: cmp      byte ptr [eax], 0x39
    //   0x6FB7C97A: cmp      eax, ecx
    //   0x6FB7C97E: cmp      eax, ecx
    //   0x6FB7C9A2: call     0x6fab62f5
    //   0x6FB7C9A8: ret
    //   0x6FB7C9A9: cmp      byte ptr [eax], 0x30
    //   0x6FB7C9AF: cmp      eax, ecx
    //   0x6FB7C9B3: cmp      eax, ecx
    //   0x6FB7C9D8: cmp      dword ptr [eax + 4], esi
    //   0x6FB7C9E6: cmp      eax, edi
    //   0x6FB7C9F0: cmp      eax, ecx
    //   0x6FB7C9F5: cmp      dword ptr [eax + 4], esi
    //   0x6FB7C9FC: ret
    //   0x6FB7CA04: call     0x6fab4920
    //   0x6FB7CA5F: call     0x6fab5754
    //   0x6FB7CA6F: call     0x6fb7c9cf
    //   0x6FB7CA97: cmp      edi, 1
    //   0x6FB7CAA0: cmp      edi, ecx
    //   0x6FB7CAA6: call     0x6fab327e
    //   0x6FB7CAAB: cmp      dword ptr [ebp - 0x1c], ecx
    //   0x6FB7CAB1: call     0x6fab4864
    //   0x6FB7CABF: cmp      eax, 8
    //   0x6FB7CAC4: cmp      eax, 0xb
    //   0x6FB7CAC9: cmp      eax, 4
    //   0x6FB7CAD7: cmp      eax, 8
    //   0x6FB7CAE9: cmp      eax, 8
    //   0x6FB7CB04: cmp      eax, edx
    //   0x6FB7CB1B: call     0x6fb7cb38
    //   0x6FB7CB20: cmp      dword ptr [ebp + 8], 8
    //   0x6FB7CB2B: call     edi
    //   0x6FB7CB38: cmp      dword ptr [ebp - 0x1c], ecx
    //   0x6FB7CB3E: call     0x6fab47af
    //   0x6FB7CB44: ret
    //   0x6FB7CB48: call     edi
    //   0x6FB7CB4E: cmp      eax, 8
    //   0x6FB7CB53: cmp      eax, 0xb
    //   0x6FB7CB58: cmp      eax, 4
    //   0x6FB7CB63: cmp      eax, 8
    //   0x6FB7CB70: call     0x6fab495b
    //   0x6FB7CB75: ret
    //   0x6FB7CBBA: cmp      ax, 0x7fff
    //   0x6FB7CBCA: cmp      cx, 0x7fff
    //   0x6FB7CBD5: cmp      dx, 0xbffd
    //   0x6FB7CBE0: cmp      dx, 0x3fbf
    //   0x6FB7CBFF: cmp      dword ptr [esi + 4], eax
    //   0x6FB7CC04: cmp      dword ptr [esi], eax
    //   0x6FB7CC0F: cmp      cx, ax
    //   0x6FB7CC1C: cmp      dword ptr [ebx + 4], eax
    //   0x6FB7CC21: cmp      dword ptr [ebx], eax
    //   0x6FB7CC47: cmp      dword ptr [ebp - 0x18], 0
    //   0x6FB7CC6A: imul     eax, ecx
    //   0x6FB7CC77: call     0x6fb7c131
    //   0x6FB7CCA0: cmp      dword ptr [ebp - 0x18], 0
    //   0x6FB7CCAD: cmp      word ptr [ebp - 0x14], 0
    //   0x6FB7CCBE: call     0x6fb7c1b0
    //   0x6FB7CCCA: cmp      word ptr [ebp - 0x14], 0
    //   0x6FB7CCD2: cmp      word ptr [ebp - 0x14], 0
    //   0x6FB7CCE0: cmp      word ptr [ebp - 0x14], 0
    //   0x6FB7CCFF: call     0x6fb7c1de
    //   0x6FB7CD08: cmp      dword ptr [ebp - 0x24], 0
    //   0x6FB7CD12: cmp      word ptr [ebp - 0x10], 0x8000
    //   0x6FB7CD22: cmp      eax, 0x18000
    //   0x6FB7CD29: cmp      dword ptr [ebp - 0xe], -1
    //   0x6FB7CD33: cmp      dword ptr [ebp - 0xa], -1
    //   0x6FB7CD3D: cmp      word ptr [ebp - 6], 0xffff
    //   0x6FB7CD61: cmp      ax, 0x7fff
    //   0x6FB7CDA5: call     0x6fab62f5
    //   0x6FB7CDAB: ret
    //   0x6FB7CDC7: cmp      dword ptr [ebp + 0xc], edx
    //   0x6FB7CDDF: cmp      dword ptr [ebp + 0x10], edx
    //   0x6FB7CDE7: cmp      dword ptr [ebp + 0xc], edx
    //   0x6FB7CDFB: cmp      eax, edx
    //   0x6FB7CE05: cmp      word ptr [esi], 0x8000
    //   0x6FB7CE1A: call     0x6fb7cb76
    //   0x6FB7CE26: cmp      dword ptr [ebp + 0xc], edx
    //   0x6FB7CE33: call     0x6fab62f5
    //   0x6FB7CE39: ret
    return 1;
}

// 1.12a: 0x6FB7CE40..0x6FB7D0B7 (__cdecl, ret 0x0)
// 1.12a: 0x6FB7CE90 (internal entry point sub_6FB7CE90)
// 1.12a: 0x6FB7CED0 (internal entry point sub_6FB7CED0)
// 1.12a: 0x6FB7CFD0 (internal entry point sub_6FB7CFD0)
int32_t __cdecl sub_6FB7CE40(void)
{
    // Callees: 0x6fab1000, 0x6fab1190, 0x6fab12b0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7CF00: call     0x6fab1000
    //   0x6FB7CF06: ret
    //   0x6FB7CF30: ret
    //   0x6FB7CF5F: call     0x6fab1190
    //   0x6FB7CF64: ret
    //   0x6FB7CF7C: call     0x6fab12b0
    //   0x6FB7CF81: ret
    //   0x6FB7CF99: ret
    //   0x6FB7CFDE: call     0x6fab1000
    //   0x6FB7CFE4: ret
    //   0x6FB7CFFB: call     0x6fab1000
    //   0x6FB7D001: ret
    //   0x6FB7D018: call     0x6fab1000
    //   0x6FB7D01E: ret
    //   0x6FB7D035: call     0x6fab1000
    //   0x6FB7D03B: ret
    //   0x6FB7D04D: call     0x6fab1000
    //   0x6FB7D053: ret
    //   0x6FB7D065: call     0x6fab1000
    //   0x6FB7D06B: ret
    //   0x6FB7D073: call     0x6fab1000
    //   0x6FB7D079: ret
    //   0x6FB7D081: call     0x6fab1000
    //   0x6FB7D087: ret
    //   0x6FB7D08F: call     0x6fab1000
    //   0x6FB7D095: ret
    return 1;
}

// 1.12a: 0x6FB7D0C0..0x6FB7D0D2 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D0C0(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB7D0E0..0x6FB7D114 (__cdecl, ret 0x0)
// 1.12a: 0x6FB7D100 (internal entry point sub_6FB7D100)
int32_t __cdecl sub_6FB7D0E0(void)
{
    // Callees: 0x6fab2130
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D104: call     0x6fab2130
    //   0x6FB7D109: ret
    return 1;
}

// 1.12a: 0x6FB7D120..0x6FB7D135 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D120(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB7D140..0x6FB7D152 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D140(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB7D160..0x6FB7D18F (__cdecl, ret 0x0)
// 1.12a: 0x6FB7D170 (internal entry point sub_6FB7D170)
int32_t __cdecl sub_6FB7D160(void)
{
    // Callees: 0x6fab2130
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D17F: call     0x6fab2130
    //   0x6FB7D184: ret
    return 1;
}

// 1.12a: 0x6FB7D190..0x6FB7D1A4 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D190(void)
{
    // Callees: 0x6fab24a0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D194: call     0x6fab24a0
    //   0x6FB7D199: ret
    return 1;
}

// 1.12a: 0x6FB7D1B0..0x6FB7D1C2 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D1B0(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB7D1D0..0x6FB7D1DF (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D1D0(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB7D1E0..0x6FB7D1FF (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D1E0(void)
{
    // Callees: 0x6fab24a0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D1EF: call     0x6fab24a0
    //   0x6FB7D1F4: ret
    return 1;
}

// 1.12a: 0x6FB7D200..0x6FB7D227 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D200(void)
{
    // Callees: 0x6fab2550
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D204: call     0x6fab2550
    //   0x6FB7D209: ret
    return 1;
}

// 1.12a: 0x6FB7D230..0x6FB7D242 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D230(void)
{
    // Instruction-level trace from 1.12a disassembly:
    return 1;
}

// 1.12a: 0x6FB7D250..0x6FB7D286 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D250(void)
{
    // Callees: 0x6fab2f10, 0x6fab20b0, 0x6fab24a0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D257: call     0x6fab2f10
    //   0x6FB7D25C: ret
    //   0x6FB7D264: call     0x6fab20b0
    //   0x6FB7D269: ret
    //   0x6FB7D276: call     0x6fab24a0
    //   0x6FB7D27B: ret
    return 1;
}

// 1.12a: 0x6FB7D290..0x6FB7D2F9 (__cdecl, ret 0x0)
// 1.12a: 0x6FB7D2D0 (internal entry point sub_6FB7D2D0)
int32_t __cdecl sub_6FB7D290(void)
{
    // Callees: 0x6fab2f10, 0x6fab24a0, 0x6fab1df0, 0x6fab1ed0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D29F: call     0x6fab2f10
    //   0x6FB7D2A4: ret
    //   0x6FB7D2AC: call     0x6fab2f10
    //   0x6FB7D2B1: ret
    //   0x6FB7D2B6: call     0x6fab24a0
    //   0x6FB7D2BB: ret
    //   0x6FB7D2C0: call     0x6fab24a0
    //   0x6FB7D2C5: ret
    //   0x6FB7D2D4: call     0x6fab1df0
    //   0x6FB7D2D9: ret
    //   0x6FB7D2E9: call     0x6fab1ed0
    //   0x6FB7D2EE: ret
    return 1;
}

// 1.12a: 0x6FB7D300..0x6FB7D32C (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D300(void)
{
    // Callees: 0x6fab2f10, 0x6fab20b0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D307: call     0x6fab2f10
    //   0x6FB7D30C: ret
    //   0x6FB7D31C: call     0x6fab20b0
    //   0x6FB7D321: ret
    return 1;
}

// 1.12a: 0x6FB7D330..0x6FB7D35C (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D330(void)
{
    // Callees: 0x6fab2f10
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D33F: call     0x6fab2f10
    //   0x6FB7D344: ret
    //   0x6FB7D34C: call     0x6fab2f10
    //   0x6FB7D351: ret
    return 1;
}

// 1.12a: 0x6FB7D360..0x6FB7D374 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D360(void)
{
    // Callees: 0x6fab1ed0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D364: call     0x6fab1ed0
    //   0x6FB7D369: ret
    return 1;
}

// 1.12a: 0x6FB7D380..0x6FB7D39F (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D380(void)
{
    // Callees: 0x6fab1df0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D384: call     0x6fab1df0
    //   0x6FB7D389: ret
    return 1;
}

// 1.12a: 0x6FB7D3A0..0x6FB7D3AB (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D3A0(void)
{
    // Callees: 0x6fab10f0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D3A5: call     0x6fab10f0
    //   0x6FB7D3AA: ret
    return 1;
}

// 1.12a: 0x6FB7D3B0..0x6FB7D3BB (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D3B0(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D3BA: ret
    return 1;
}

// 1.12a: 0x6FB7D3C0..0x6FB7D3CE (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D3C0(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D3CD: ret
    return 1;
}

// 1.12a: 0x6FB7D3D0..0x6FB7D3F1 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D3D0(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D3E2: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D3F0: ret
    return 1;
}

// 1.12a: 0x6FB7D400..0x6FB7D421 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D400(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D412: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D420: ret
    return 1;
}

// 1.12a: 0x6FB7D430..0x6FB7D451 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D430(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D442: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D450: ret
    return 1;
}

// 1.12a: 0x6FB7D460..0x6FB7D472 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D460(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D471: ret
    return 1;
}

// 1.12a: 0x6FB7D480..0x6FB7D4A1 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D480(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D492: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D4A0: ret
    return 1;
}

// 1.12a: 0x6FB7D4B0..0x6FB7D4D1 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D4B0(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D4C2: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D4D0: ret
    return 1;
}

// 1.12a: 0x6FB7D4E0..0x6FB7D501 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D4E0(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D4F2: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D500: ret
    return 1;
}

// 1.12a: 0x6FB7D510..0x6FB7D531 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D510(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D522: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D530: ret
    return 1;
}

// 1.12a: 0x6FB7D540..0x6FB7D561 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D540(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D552: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D560: ret
    return 1;
}

// 1.12a: 0x6FB7D570..0x6FB7D591 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D570(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D582: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D590: ret
    return 1;
}

// 1.12a: 0x6FB7D5A0..0x6FB7D5C1 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D5A0(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D5B2: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D5C0: ret
    return 1;
}

// 1.12a: 0x6FB7D5D0..0x6FB7D5F1 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D5D0(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D5E2: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D5F0: ret
    return 1;
}

// 1.12a: 0x6FB7D600..0x6FB7D616 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D600(void)
{
    // Callees: 0x6fab1e30, 0x6fab3dec
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D605: call     0x6fab1e30
    //   0x6FB7D60F: call     0x6fab3dec
    //   0x6FB7D615: ret
    return 1;
}

// 1.12a: 0x6FB7D620..0x6FB7D651 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB7D620(D2UnitStrc* pUnit)
{
    // Callees: 0x6fab1160
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D630: mov      ebx, dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D63F: call     0x6fab1160
    //   0x6FB7D650: ret
    return 1;
}

// 1.12a: 0x6FB7D660..0x6FB7D678 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB7D660(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D677: ret
    return 1;
}

// 1.12a: 0x6FB7D680..0x6FB7D695 (__fastcall, ret 0x0)
int32_t __fastcall sub_6FB7D680(D2UnitStrc* pUnit, int32_t nArgEdx)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D694: ret
    return 1;
}

// 1.12a: 0x6FB7D6A0..0x6FB7D6AC (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D6A0(void)
{
    // Callees: 0x6fab3dec
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D6A5: call     0x6fab3dec
    //   0x6FB7D6AB: ret
    return 1;
}

// 1.12a: 0x6FB7D6B0..0x6FB7D6D1 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D6B0(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D6C2: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D6D0: ret
    return 1;
}

// 1.12a: 0x6FB7D6E0..0x6FB7D701 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D6E0(void)
{
    // Callees: IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D6F2: call     dword ptr [0x6fb7eab4] ; IAT:D2Lang.dll!??_FUnicode@@QAEXXZ -> ??_FUnicode@@QAEXXZ
    //   0x6FB7D700: ret
    return 1;
}

// 1.12a: 0x6FB7D710..0x6FB7D726 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D710(void)
{
    // Callees: 0x6fab2f50, 0x6fab3dec
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D715: call     0x6fab2f50
    //   0x6FB7D71F: call     0x6fab3dec
    //   0x6FB7D725: ret
    return 1;
}

// 1.12a: 0x6FB7D730..0x6FB7D73B (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D730(void)
{
    // Callees: 0x6fab1da0
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D735: call     0x6fab1da0
    //   0x6FB7D73A: ret
    return 1;
}

// 1.12a: 0x6FB7D740..0x6FB7D741 (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D740(void)
{
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D740: ret
    return 1;
}

// 1.12a: 0x6FB7D750..0x6FB7D75B (__cdecl, ret 0x0)
int32_t __cdecl sub_6FB7D750(void)
{
    // Callees: 0x6fab2f10
    // Instruction-level trace from 1.12a disassembly:
    //   0x6FB7D755: call     0x6fab2f10
    //   0x6FB7D75A: ret
    return 1;
}
