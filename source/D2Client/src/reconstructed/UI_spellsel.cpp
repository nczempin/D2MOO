// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Slice:
//   - ..\Source\D2Client\UI\spellsel.cpp (0x6FB074E0..0x6FB094BF)
//   - ..\Source\D2Client\UNIT\Player.cpp (0x6FB094C0..0x6FB099AF)
// ImageBase: 0x6FAB0000
// ============================================================================

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <D2BasicTypes.h>
#include <D2DataTbls.h>
#include <DataTbls/SkillsTbls.h>
#include <Units/Units.h>
#include <Path/Path.h>
#include <D2Inventory.h>
#include <D2Items.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <D2CMP.h>
#include <D2Lang.h>
#include <D2Unicode.h>
#include <Texture.h>
#include <Window.h>
#include <D2WinFont.h>
#include <Fog.h>
#include <D2BitManip.h>
#include <Storm.h>

// ============================================================================
// Local & Engine Structures
// ============================================================================

#pragma pack(push, 1)

#ifndef D2_GFX_DATA_STRC_DEFINED
#define D2_GFX_DATA_STRC_DEFINED
struct D2GfxCellStrc;

struct D2GfxDataStrc
{
    uint8_t fState;                         // +0x00
    uint8_t nComponent;                     // +0x01
    uint8_t unk0x02;                        // +0x02
    uint8_t unk0x03;                        // +0x03
    int32_t nUnitType;                      // +0x04
    int32_t nClassId;                       // +0x08
    int32_t nMode;                          // +0x0C
    int32_t nOverlayId;                     // +0x10
    uint32_t dwName[5];                     // +0x14..+0x27
    char* pName;                            // +0x28
    uint32_t dwFlags;                       // +0x2C
    int32_t nMaxDirections;                 // +0x30
    D2GfxCellStrc* pCurrentCell;            // +0x34
    uint32_t nDirection;                    // +0x38
    D2CellFileStrc* pCellFile;              // +0x3C
    uint32_t nFrame;                        // +0x40
    int32_t nMaxFrames;                     // +0x44
};
static_assert(sizeof(D2GfxDataStrc) == 0x48, "D2GfxDataStrc must be 0x48 bytes");
#endif

// Client unit graphics container stored in pUnit->pGfxData (ENGINE/Gfx.cpp, 0x3C bytes)
struct D2UnitCofLinkStrc;
struct D2OverlayStrc;

struct D2UnitGfxDataStrc
{
    uint16_t aLayerFlags[20];               // +0x00..+0x27 (low byte cleared on player death)
    D2UnitCofLinkStrc* pCofList;            // +0x28
    D2OverlayStrc* pOverlayList;            // +0x2C
    void* pParticleList;                    // +0x30
    int32_t nHeightIndex;                   // +0x34
    int32_t nDefaultHeightIndex;            // +0x38
};
static_assert(sizeof(D2UnitGfxDataStrc) == 0x3C, "D2UnitGfxDataStrc must be 0x3C bytes");

// Class-specific skill icon table entry (0x22 bytes, 7 entries at 0x6FB947F0..0x6FB948DD)
struct D2ClassSkillIconStrc
{
    D2CellFileStrc* pCellFile;              // +0x00
    char szPath[30];                        // +0x04
};
static_assert(sizeof(D2ClassSkillIconStrc) == 0x22, "D2ClassSkillIconStrc must be 0x22 bytes");

// Speedbar popup grid state (0x3C5 bytes at 0x6FBC21D8..0x6FBC259C)
struct D2SpeedBarStateStrc
{
    int32_t anPosX[60];                     // +0x000 (0x6FBC21D8..0x6FBC22C7)
    int32_t anPosY[60];                     // +0x0F0 (0x6FBC22C8..0x6FBC23B7)
    int32_t anSkillId[60];                  // +0x1E0 (0x6FBC23B8..0x6FBC24A7)
    D2UnitGUID adwOwnerGUID[60];            // +0x2D0 (0x6FBC24A8..0x6FBC2597)
    uint8_t nCount;                         // +0x3C0 (0x6FBC2598)
    int32_t nLastTint;                      // +0x3C1 (0x6FBC2599..0x6FBC259C)
};
static_assert(sizeof(D2SpeedBarStateStrc) == 0x3C5, "D2SpeedBarStateStrc must be 0x3C5 bytes");

// 10-byte packed key binding entry in gKeyBindings (0x6FBB11A0..0x6FBB1613)
struct D2KeyBindingStrc
{
    int32_t nCommandId;                     // +0x00
    uint16_t wKey;                          // +0x04 (0xFFFF = unbound)
    int32_t bIsPrimary;                     // +0x06 (1 = primary, 0 = secondary)
};
static_assert(sizeof(D2KeyBindingStrc) == 0x0A, "D2KeyBindingStrc must be 0x0A bytes");

// Storm window message dispatch parameter block (0x20 bytes)
struct D2StormMsgStrc
{
    HWND hWnd;                              // +0x00
    uint32_t uMessage;                      // +0x04
    WPARAM wParam;                          // +0x08
    LPARAM lParam;                          // +0x0C
    uint32_t dwCommand;                     // +0x10
    uint32_t dwKeyFlags;                    // +0x14
    int32_t bHandled;                       // +0x18
    int32_t dwReturn;                       // +0x1C
};
static_assert(sizeof(D2StormMsgStrc) == 0x20, "D2StormMsgStrc must be 0x20 bytes");

// Storm message handler registration table entry (0x0C bytes)
struct D2MsgHandlerRegStrc
{
    uint32_t dwMsgCategory;                 // +0x00 (0 = Window message)
    uint32_t dwMsgCode;                     // +0x04 (WM_LBUTTONDOWN = 0x201)
    BOOL (__stdcall* pfnHandler)(D2StormMsgStrc* pMsg); // +0x08
};
static_assert(sizeof(D2MsgHandlerRegStrc) == 0x0C, "D2MsgHandlerRegStrc must be 0x0C bytes");

// C2S 0x3C (C2S_SELECTSKILL) 9-byte packet
struct D2C2SSelectSkillPacketStrc
{
    uint8_t nOpcode;                        // +0x00 (0x3C)
    uint32_t dwSkillIdAndSide;              // +0x01 (bit 31 = left hand)
    D2UnitGUID dwOwnerGUID;                 // +0x05 (-1 = native skill)
};
static_assert(sizeof(D2C2SSelectSkillPacketStrc) == 0x09, "D2C2SSelectSkillPacketStrc must be 9 bytes");

// C2S 0x51 (C2S_ASSIGNSKILLHOTKEY) 9-byte packet
struct D2C2SAssignSkillHotkeyPacketStrc
{
    uint8_t nOpcode;                        // +0x00 (0x51)
    uint16_t wSkillIdAndSide;               // +0x01 (bit 15 = left hand, 0xFFFF = unbind)
    uint16_t wHotkeySlot;                   // +0x03 (0..15)
    D2UnitGUID dwOwnerGUID;                 // +0x05 (-1 = native skill)
};
static_assert(sizeof(D2C2SAssignSkillHotkeyPacketStrc) == 0x09, "D2C2SAssignSkillHotkeyPacketStrc must be 9 bytes");

// Local player input / mouse action state block (UNIT/Player.cpp, 0x20 bytes)
enum D2PlayerActionFlags : uint32_t
{
    PLRACTIONFLAG_LEFTMOUSE   = 0x01,
    PLRACTIONFLAG_RIGHTMOUSE  = 0x02,
    PLRACTIONFLAG_BUTTONDOWN  = 0x04,
    PLRACTIONFLAG_BUTTONHELD  = 0x08,
    PLRACTIONFLAG_BUTTONUP    = 0x10,
    PLRACTIONFLAG_STANDSTILL  = 0x20,
    PLRACTIONFLAG_RUN         = 0x40,
};

struct D2PlayerActionStrc
{
    uint32_t dwFlags;                       // +0x00 (D2PlayerActionFlags)
    D2UnitStrc* pPlayerUnit;                // +0x04
    D2UnitStrc* pTargetUnit;                // +0x08
    int32_t nTargetX;                       // +0x0C
    int32_t nTargetY;                       // +0x10
    int32_t nActionParam;                   // +0x14
    int32_t nActionState;                   // +0x18
    D2SkillStrc* pSkill;                    // +0x1C
};
static_assert(sizeof(D2PlayerActionStrc) == 0x20, "D2PlayerActionStrc must be 0x20 bytes");

#pragma pack(pop)

// ============================================================================
// External Imports & D2Client Cross-Module Declarations
// ============================================================================

inline const char* FOG_GetErrorLocation(int32_t nLine) { (void)nLine; return FOG_GetCallerReturnAddress(); }

struct D2LightNodeStrc;

// 0x6FAB1D40 (Engine/DataTbls inline helper)
D2SkillDescTxt* __fastcall D2Client_GetSkillDescTxtRecord(int32_t nSkillId);
// 0x6FAF91D0 (UI/SkillDesc.cpp)
void __stdcall SKILLDESC_DrawActiveSkillTooltip(
    D2UnitStrc* pPlayerUnit,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nCharges,
    int32_t nColor,
    int32_t nLeftX,
    int32_t nBottomY);
// 0x6FB07470 (SKILLS/Skills.cpp / UI/spellsel.cpp)
BOOL __fastcall D2Client_IsSkillHotkeySlotUsable(int32_t nHotkeySlot);
// 0x6FB0E500 (DRAW/dLightMap.cpp)
void __fastcall LIGHTMAP_DestroyLightNode(D2LightNodeStrc* pLight);
// 0x6FB0E680 (DRAW/dLightMap.cpp)
int32_t __stdcall LIGHTMAP_ApproxDistanceB(int32_t nDx, int32_t nDy);
// 0x6FB1D0A0 (UI/smsg.cpp)
void __fastcall SMSG_UnregisterCallbacks(int32_t nCount, HWND hWnd, const D2MsgHandlerRegStrc* pTable);
// 0x6FB291A0 (GAME/Msg.cpp)
int32_t __fastcall MSG_SendPacket(void* pPacketData, size_t nSize);
// 0x6FB29F80 (GAME/Game.cpp)
void __stdcall GAME_ResetMouseButtonStates(void);
// 0x6FB3C170 (UI/ui.cpp)
D2CellFileStrc* __fastcall UI_LoadPanelCelAsset(const char* szRelPath);
// 0x6FB3EF00 (UI/ui.cpp)
int32_t __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t nArg);
// 0x6FB407D0 (ENGINE/Gfx.cpp)
void __fastcall GFX_FreeDeadUnitCofCache(D2UnitStrc* pUnit);
// 0x6FB50F30 (UI/CmdTbl.cpp)
const Unicode* __fastcall CMDTBL_GetShortBindingNameString(int32_t nCommandId, int32_t bIsPrimary);

// Forward declaration for SMsg table registration
BOOL __stdcall D2Client_HandleSpeedBarMouseClick(D2StormMsgStrc* pMsg);

// ============================================================================
// Globals (.rdata / .data / .bss)
// ============================================================================

// 0x6FB80323
static const char s_szEmptyHaltMsg_6FB80323[] = "";
// 0x6FB807A0
static const char s_szArchiveFile_6FB807A0[] = "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP";
// 0x6FB813D0
static const D2MsgHandlerRegStrc g_tSpeedBarMsgReg_6FB813D0 = { 0, WM_LBUTTONDOWN, D2Client_HandleSpeedBarMouseClick };
// 0x6FB85244
static const char s_szPlayerFile_6FB85244[] = "..\\Source\\D2Client\\UNIT\\Player.cpp";
// 0x6FB85268
static const char s_szSpellSelFile_6FB85268[] = "..\\Source\\D2Client\\UI\\spellsel.cpp";
// 0x6FB8528C
static const char s_szGeneralSkillIconPath_6FB8528C[] = "Spells\\Skillicon";

// 0x6FB8C6E0
// [aliased] gnScreenWidth_6FB8C6E0
// 0x6FB8C6E4
// [aliased] gnScreenHeight_6FB8C6E4
// 0x6FB8F368
// [aliased] gnPanelCount_6FB8F368

// 0x6FB947F0..0x6FB948DD (7 class skill icon descriptors, stride 0x22)
static D2ClassSkillIconStrc g_aClassSkillIcons_6FB947F0[7] =
{
    { nullptr, "Spells\\AmSkillicon" }, // [0] Amazon      (0x6FB947F0)
    { nullptr, "Spells\\SoSkillicon" }, // [1] Sorceress   (0x6FB94812)
    { nullptr, "Spells\\NeSkillicon" }, // [2] Necromancer (0x6FB94834)
    { nullptr, "Spells\\PaSkillicon" }, // [3] Paladin     (0x6FB94856)
    { nullptr, "Spells\\BaSkillicon" }, // [4] Barbarian   (0x6FB94878)
    { nullptr, "Spells\\DrSkillicon" }, // [5] Druid       (0x6FB9489A)
    { nullptr, "Spells\\AsSkillicon" }, // [6] Assassin    (0x6FB948BC)
};

// 0x6FBB11A0..0x6FBB1613 (114 key binding entries)
// [aliased] gKeyBindings_6FBB11A0
// 0x6FBB1634
// [aliased] gnMouseY_6FBB1634
// 0x6FBB1638
// [aliased] gnMouseX_6FBB1638
// 0x6FBB2B58..0x6FBB2BEF (38 panel open states; index 3 at 0x6FBB2B64 = D2PANEL_SKILLSELECT)
// [aliased] gaPanelStates_6FBB2B58
// 0x6FBB32B0
// [aliased] gdwClientGameTick_6FBB32B0
// 0x6FBB3314
// [aliased] gdwSkillDelayEndFrame_6FBB3314
// 0x6FBB33D4
// [aliased] gnLocalPlayerCharClass_6FBB33D4

// 0x6FBC21D8..0x6FBC259C
static D2SpeedBarStateStrc g_tSpeedBar_6FBC21D8;
// 0x6FBC25A0..0x6FBC25DF
int32_t g_anSkillHotkeyId_6FBC25A0[16];
// 0x6FBC25E0
static int32_t g_nCurrentSkillHotkeyIdx_6FBC25E0;
// 0x6FBC25E4..0x6FBC25F3 (rows 1..4 Y positions recorded by jump table 0x6FB09130)
static int32_t g_anSpeedBarRowPosY_6FBC25E4[4];
// 0x6FBC2600..0x6FBC263F
static int32_t g_anSkillHotkeyUnk600_6FBC2600[16];
// 0x6FBC2640..0x6FBC267F
D2UnitGUID g_adwSkillHotkeyOwnerGUID_6FBC2640[16];
// 0x6FBC2680..0x6FBC268F
static int32_t g_anSkillHotkeyUnk680_6FBC2680[4];
// 0x6FBC2690..0x6FBC26CF
int32_t g_abSkillHotkeyLeft_6FBC2690[16];

// 0x6FBCBCE4
// [aliased] gwRightHandWeaponClass_6FBCBCE4
// 0x6FBCBCE8
// [aliased] gwLeftHandWeaponClass_6FBCBCE8
// 0x6FBCC1D8
extern int32_t g_bActionDispatchedThisFrame;
#define g_bPlayerMouseActionLock_6FBCC1D8 g_bActionDispatchedThisFrame
// 0x6FBCC1DC
static D2CellFileStrc* gpGeneralSkillIconCel_6FBCC1DC = nullptr;
// 0x6FBCC1E0
static BOOL gbSpeedBarLeftSide_6FBCC1E0 = FALSE;
// 0x6FBCC3D0
extern D2UnitStrc* gpPlayerUnit_6FBCC3D0;

// 0x6FBD365C
static int32_t g_nHeldMouseTargetState_6FBD365C = 0;
// 0x6FBD3660
static uint32_t g_dwLastStepMoveTick_6FBD3660 = 0;


extern D2KeyBindingStrc gKeyBindings[114];
extern int32_t  gaPanelStates[38];
extern uint32_t gdwClientGameTick;
extern uint32_t gdwSkillCooldownEndFrame_6FBB3314;
extern uint8_t  gnLocalPlayerCharClass;
extern int32_t  gnMouseX;
extern int32_t  gnMouseY;
extern int32_t  gnRegisteredPanelCount;
extern int32_t  gnScreenHeight;
extern int32_t  gnScreenWidth;
extern void __stdcall GAME_ResetAllMouseStates(void);
struct D2SMsgRegEntryStrc;
extern void __fastcall WALL2_UnregisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries);
extern void __fastcall SKILLDESC_DrawActiveSkillTooltip(int32_t nSkillId, int32_t nSkillLevel, D2SkillStrc* pSkill, int32_t bLeftHand, int32_t nCenterX, int32_t nBottomY);

#define gKeyBindings_6FBB11A0          gKeyBindings
#define gaPanelStates_6FBB2B58         gaPanelStates
#define gdwClientGameTick_6FBB32B0     gdwClientGameTick
#define gdwSkillDelayEndFrame_6FBB3314 gdwSkillCooldownEndFrame_6FBB3314
#define gnLocalPlayerCharClass_6FBB33D4 gnLocalPlayerCharClass
#define gnMouseX_6FBB1638              gnMouseX
#define gnMouseY_6FBB1634              gnMouseY
#define gnPanelCount_6FB8F368          gnRegisteredPanelCount
#define gnScreenHeight_6FB8C6E4        gnScreenHeight
#define gnScreenWidth_6FB8C6E0         gnScreenWidth

uint16_t gwLeftHandWeaponClass_6FBCBCE8  = 0;
uint16_t gwRightHandWeaponClass_6FBCBCE4 = 0;

// ============================================================================
// PART 1: UI/spellsel.cpp (0x6FB074E0..0x6FB094BF)
// ============================================================================

// 1.12a: 0x6FB074E0 (spans 0x6FB074E0..0x6FB07520, straddling slice start 0x6FB07500)
int32_t __fastcall D2Client_FindQuantitySkillSlot0(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (pUnit)
    {
        for (D2SkillStrc* pSkill = UNITS_GetStartSkill(pUnit); pSkill != nullptr; pSkill = SKILLS_GetNextSkill(pSkill))
        {
            D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
            if (pSkillsTxt && SKILLS_GetQuantity(pSkill) > 0 && pSkillsTxt->nSkillId == nSkillId)
            {
                break;
            }
        }
    }
    return 0;
}

// 1.12a: 0x6FB07530
int32_t __fastcall D2Client_FindQuantitySkillSlot1(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (pUnit)
    {
        for (D2SkillStrc* pSkill = UNITS_GetStartSkill(pUnit); pSkill != nullptr; pSkill = SKILLS_GetNextSkill(pSkill))
        {
            D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
            if (pSkillsTxt && SKILLS_GetQuantity(pSkill) > 0 && pSkillsTxt->nSkillId == nSkillId)
            {
                break;
            }
        }
    }
    return 0;
}

// 1.12a: 0x6FB07580
int32_t __fastcall D2Client_FindQuantitySkillSlot2(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (pUnit)
    {
        for (D2SkillStrc* pSkill = UNITS_GetStartSkill(pUnit); pSkill != nullptr; pSkill = SKILLS_GetNextSkill(pSkill))
        {
            D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
            if (pSkillsTxt && SKILLS_GetQuantity(pSkill) > 0 && pSkillsTxt->nSkillId == nSkillId)
            {
                break;
            }
        }
    }
    return 0;
}

// 1.12a: 0x6FB075D0
int32_t __fastcall D2Client_FindQuantitySkillSlot3(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (pUnit)
    {
        for (D2SkillStrc* pSkill = UNITS_GetStartSkill(pUnit); pSkill != nullptr; pSkill = SKILLS_GetNextSkill(pSkill))
        {
            D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
            if (pSkillsTxt && SKILLS_GetQuantity(pSkill) > 0 && pSkillsTxt->nSkillId == nSkillId)
            {
                break;
            }
        }
    }
    return 0;
}

// 1.12a: 0x6FB07620
int32_t __stdcall D2Client_AddSpeedBarSkillEntry(
    int32_t nPosX,
    int32_t nPosY,
    D2SkillStrc* pSkill,
    BOOL bLeftSide,
    int32_t nIconTint)
{
    const uint8_t nIdx = g_tSpeedBar_6FBC21D8.nCount;
    g_tSpeedBar_6FBC21D8.anPosX[nIdx] = nPosX;
    g_tSpeedBar_6FBC21D8.anPosY[nIdx] = nPosY;
    g_tSpeedBar_6FBC21D8.anSkillId[nIdx] = SKILLS_GetSkillIdFromSkill(pSkill, s_szSpellSelFile_6FB85268, 573);
    SKILLS_GetSkillInfo(pSkill, &g_tSpeedBar_6FBC21D8.adwOwnerGUID[nIdx], nullptr, nullptr, nullptr);
    g_tSpeedBar_6FBC21D8.nLastTint = nIconTint;
    g_tSpeedBar_6FBC21D8.nCount = static_cast<uint8_t>(nIdx + 1);

    return bLeftSide ? (nPosX + 48) : (nPosX - 48);
}

// 1.12a: 0x6FB076B0
void __fastcall D2Client_SetSpeedBarSide(BOOL bLeftSide)
{
    gbSpeedBarLeftSide_6FBCC1E0 = bLeftSide;
}

// 1.12a: 0x6FB076C0
void __stdcall D2Client_DrawSkillIconQuantityOverlay(int32_t nQuantity, int32_t nX, int32_t nY)
{
    Unicode wszQuantity[16];
    if (nQuantity <= 0)
    {
        return;
    }

    char szQuantity[16];
    _itoa(nQuantity, szQuantity, 10);
    Unicode::toUnicode(wszQuantity, szQuantity, 16);

    int32_t nLen = Unicode::strlen(wszQuantity);
    if (D2Win_10121_GetTextWidth(wszQuantity) > 40)
    {
        Unicode* pTail = &wszQuantity[nLen];
        do
        {
            *pTail = Unicode(0);
            --pTail;
        } while (D2Win_10121_GetTextWidth(wszQuantity) > 40);
    }

    D2Win_10121_GetTextWidth(wszQuantity);
    D2Win_10117_DrawText(wszQuantity, nX + 4, nY - 1, 3, FALSE);
}

// 1.12a: 0x6FB07770
int32_t __fastcall D2Client_GetSkillIconTintColor(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
    const int32_t nUseState = SKILLS_GetUseState(pUnit, pSkill);
    if (nUseState == SKILLUSTATE_USABLE)
    {
        if (pUnit == gpPlayerUnit_6FBCC3D0 && gdwClientGameTick_6FBB32B0 < gdwSkillDelayEndFrame_6FBB3314)
        {
            return 1;
        }
        return 0;
    }

    if (nUseState == SKILLUSTATE_AURA)
    {
        return 4;
    }
    return 1;
}

// 1.12a: 0x6FB077B0
BOOL __stdcall D2Client_PtInSkillIconRect(int32_t nLeftX, int32_t nBottomY, int32_t nWidth, int32_t nHeight)
{
    const int32_t nMouseX = gnMouseX_6FBB1638;
    if (nMouseX < nLeftX || nMouseX > nLeftX + nWidth)
    {
        return FALSE;
    }

    const int32_t nMouseY = gnMouseY_6FBB1634;
    if (nMouseY < nBottomY - nHeight || nMouseY > nBottomY)
    {
        return FALSE;
    }
    return TRUE;
}

// 1.12a: 0x6FB077F0
void __stdcall D2Client_FreeSkillIconCellFiles(void)
{
    if (gpGeneralSkillIconCel_6FBCC1DC)
    {
        D2CMP_CelFileFreeHardware(gpGeneralSkillIconCel_6FBCC1DC);
        FOG_Free(gpGeneralSkillIconCel_6FBCC1DC, s_szArchiveFile_6FB807A0, 122, 0);
        gpGeneralSkillIconCel_6FBCC1DC = nullptr;
    }

    D2CellFileStrc* pLocalClassCel = g_aClassSkillIcons_6FB947F0[gnLocalPlayerCharClass_6FBB33D4].pCellFile;
    if (pLocalClassCel)
    {
        D2CMP_CelFileFreeHardware(pLocalClassCel);
        FOG_Free(pLocalClassCel, s_szArchiveFile_6FB807A0, 122, 0);
        g_aClassSkillIcons_6FB947F0[gnLocalPlayerCharClass_6FBB33D4].pCellFile = nullptr;
    }

    for (int32_t i = 0; i < 7; ++i)
    {
        D2CellFileStrc* pCel = g_aClassSkillIcons_6FB947F0[i].pCellFile;
        if (pCel)
        {
            D2CMP_CelFileFreeHardware(pCel);
            FOG_Free(pCel, s_szArchiveFile_6FB807A0, 122, 0);
            g_aClassSkillIcons_6FB947F0[i].pCellFile = nullptr;
        }
    }
}

// 1.12a: 0x6FB078A0
void __fastcall D2Client_AssignSkillHotkey(int32_t nHotkeySlot, int32_t nSpeedBarIdx)
{
    if (nHotkeySlot >= 16 || g_anSkillHotkeyId_6FBC25A0[nHotkeySlot] != -1)
    {
        return;
    }

    const int32_t nSkillId = g_tSpeedBar_6FBC21D8.anSkillId[nSpeedBarIdx];
    g_anSkillHotkeyId_6FBC25A0[nHotkeySlot] = nSkillId;
    g_abSkillHotkeyLeft_6FBC2690[nHotkeySlot] = (gbSpeedBarLeftSide_6FBCC1E0 != FALSE) ? 1 : 0;

    const D2UnitGUID dwOwnerGUID = g_tSpeedBar_6FBC21D8.adwOwnerGUID[nSpeedBarIdx];
    g_adwSkillHotkeyOwnerGUID_6FBC2640[nHotkeySlot] = dwOwnerGUID;

    const uint16_t wSideMask = g_abSkillHotkeyLeft_6FBC2690[nHotkeySlot] ? 0x8000u : 0u;
    D2C2SAssignSkillHotkeyPacketStrc tPacket;
    tPacket.nOpcode = 0x51;
    tPacket.wSkillIdAndSide = static_cast<uint16_t>(nSkillId + wSideMask);
    tPacket.wHotkeySlot = static_cast<uint16_t>(nHotkeySlot);
    tPacket.dwOwnerGUID = dwOwnerGUID;
    MSG_SendPacket(&tPacket, sizeof(tPacket));
}

// 1.12a: 0x6FB07930
void __fastcall D2Client_SelectActiveSkill(int32_t nHotkeySlot)
{
    if (g_anSkillHotkeyId_6FBC25A0[nHotkeySlot] == -1)
    {
        return;
    }

    if (!D2Client_IsSkillHotkeySlotUsable(nHotkeySlot))
    {
        return;
    }

    const int32_t bLeftHand = g_abSkillHotkeyLeft_6FBC2690[nHotkeySlot];
    const int32_t nSkillId = g_anSkillHotkeyId_6FBC25A0[nHotkeySlot];
    const D2UnitGUID dwOwnerGUID = g_adwSkillHotkeyOwnerGUID_6FBC2640[nHotkeySlot];
    D2UnitStrc* const pPlayer = gpPlayerUnit_6FBCC3D0;

    g_nCurrentSkillHotkeyIdx_6FBC25E0 = nHotkeySlot;

    D2C2SSelectSkillPacketStrc tPacket;
    tPacket.nOpcode = 0x3C;
    tPacket.dwSkillIdAndSide = static_cast<uint32_t>(nSkillId) | (bLeftHand ? 0x80000000u : 0u);
    tPacket.dwOwnerGUID = dwOwnerGUID;
    MSG_SendPacket(&tPacket, sizeof(tPacket));

    if (bLeftHand)
    {
        SKILLS_SetLeftActiveSkill(pPlayer, nSkillId, dwOwnerGUID);
    }
    else
    {
        SKILLS_SetRightActiveSkill(pPlayer, nSkillId, dwOwnerGUID);
    }
}

// 1.12a: 0x6FB079E0
void __stdcall D2Client_BindSkillHotkeyFromSpeedBarOrActive(int32_t nHotkeySlot)
{
    int32_t nMouseX = gnMouseX_6FBB1638;
    int32_t nMouseY = gnMouseY_6FBB1634;

    for (int32_t i = 0; i < 60; ++i)
    {
        const int32_t nIconX = g_tSpeedBar_6FBC21D8.anPosX[i];
        const int32_t nIconY = g_tSpeedBar_6FBC21D8.anPosY[i];
        if (nMouseX < nIconX || nMouseX > nIconX + 48 || nMouseY < nIconY - 48 || nMouseY > nIconY)
        {
            continue;
        }

        if (g_anSkillHotkeyId_6FBC25A0[nHotkeySlot] != -1)
        {
            D2C2SAssignSkillHotkeyPacketStrc tUnbindPrev;
            tUnbindPrev.nOpcode = 0x51;
            tUnbindPrev.wSkillIdAndSide = 0xFFFFu;
            tUnbindPrev.wHotkeySlot = static_cast<uint16_t>(nHotkeySlot);
            tUnbindPrev.dwOwnerGUID = static_cast<D2UnitGUID>(-1);
            MSG_SendPacket(&tUnbindPrev, sizeof(tUnbindPrev));
        }

        const int32_t nSkillId = g_tSpeedBar_6FBC21D8.anSkillId[i];
        g_anSkillHotkeyId_6FBC25A0[nHotkeySlot] = nSkillId;
        g_abSkillHotkeyLeft_6FBC2690[nHotkeySlot] = (gbSpeedBarLeftSide_6FBCC1E0 != FALSE) ? 1 : 0;

        const D2UnitGUID dwOwnerGUID = g_tSpeedBar_6FBC21D8.adwOwnerGUID[i];
        g_adwSkillHotkeyOwnerGUID_6FBC2640[nHotkeySlot] = dwOwnerGUID;

        const uint16_t wSideMask = g_abSkillHotkeyLeft_6FBC2690[nHotkeySlot] ? 0x8000u : 0u;
        D2C2SAssignSkillHotkeyPacketStrc tBindNew;
        tBindNew.nOpcode = 0x51;
        tBindNew.wSkillIdAndSide = static_cast<uint16_t>(nSkillId + wSideMask);
        tBindNew.wHotkeySlot = static_cast<uint16_t>(nHotkeySlot);
        tBindNew.dwOwnerGUID = dwOwnerGUID;
        MSG_SendPacket(&tBindNew, sizeof(tBindNew));

        for (int32_t nOther = 0; nOther < 16; ++nOther)
        {
            if (nOther != nHotkeySlot
                && g_anSkillHotkeyId_6FBC25A0[nOther] == g_anSkillHotkeyId_6FBC25A0[nHotkeySlot]
                && g_adwSkillHotkeyOwnerGUID_6FBC2640[nOther] == g_adwSkillHotkeyOwnerGUID_6FBC2640[nHotkeySlot])
            {
                g_anSkillHotkeyId_6FBC25A0[nOther] = -1;
                g_abSkillHotkeyLeft_6FBC2690[nOther] = -1;
                g_adwSkillHotkeyOwnerGUID_6FBC2640[nOther] = static_cast<D2UnitGUID>(-1);

                D2C2SAssignSkillHotkeyPacketStrc tUnbindDup;
                tUnbindDup.nOpcode = 0x51;
                tUnbindDup.wSkillIdAndSide = 0xFFFFu;
                tUnbindDup.wHotkeySlot = static_cast<uint16_t>(nOther);
                tUnbindDup.dwOwnerGUID = static_cast<D2UnitGUID>(-1);
                MSG_SendPacket(&tUnbindDup, sizeof(tUnbindDup));
            }
        }

        nMouseX = gnMouseX_6FBB1638;
        nMouseY = gnMouseY_6FBB1634;
    }
}

// 1.12a: 0x6FB07B80
void __stdcall D2Client_ClearDepletedItemSkillHotkeys(D2SkillsTxt* pSkillsTxt, int32_t nQuantity)
{
    for (int32_t i = 0; i < 60; ++i)
    {
        if (g_tSpeedBar_6FBC21D8.anSkillId[i] != pSkillsTxt->nSkillId)
        {
            continue;
        }

        for (int32_t nSlot = 0; nSlot < 16; ++nSlot)
        {
            if (g_anSkillHotkeyId_6FBC25A0[nSlot] == g_tSpeedBar_6FBC21D8.anSkillId[i] && nQuantity <= 0)
            {
                g_anSkillHotkeyId_6FBC25A0[nSlot] = -1;
                g_abSkillHotkeyLeft_6FBC2690[nSlot] = -1;
                g_adwSkillHotkeyOwnerGUID_6FBC2640[nSlot] = static_cast<D2UnitGUID>(-1);

                D2C2SAssignSkillHotkeyPacketStrc tUnbind;
                tUnbind.nOpcode = 0x51;
                tUnbind.wSkillIdAndSide = 0xFFFFu;
                tUnbind.wHotkeySlot = static_cast<uint16_t>(nSlot);
                tUnbind.dwOwnerGUID = static_cast<D2UnitGUID>(-1);
                MSG_SendPacket(&tUnbind, sizeof(tUnbind));
            }
        }
    }
}

// 1.12a: 0x6FB07C20
void __stdcall D2Client_CycleSkillHotkeys(int32_t nDirectionDelta)
{
    D2UnitStrc* const pPlayer = gpPlayerUnit_6FBCC3D0;
    if (!pPlayer || pPlayer->dwAnimMode == 17)
    {
        return;
    }

    const int32_t nStep = (nDirectionDelta > 0) ? 1 : -1;
    int32_t nRemaining = (nDirectionDelta < 0) ? -nDirectionDelta : nDirectionDelta;
    if (nRemaining == 0)
    {
        return;
    }

    int32_t nBestSlot = -1;
    int32_t nIdx = g_nCurrentSkillHotkeyIdx_6FBC25E0 + nStep;

    for (int32_t nChecked = 0; nChecked < 16; ++nChecked)
    {
        if (nIdx < 0)
        {
            nIdx = 15;
        }
        else if (nIdx >= 16)
        {
            nIdx = 0;
        }

        if (g_anSkillHotkeyId_6FBC25A0[nIdx] != -1
            && g_abSkillHotkeyLeft_6FBC2690[nIdx] == 0
            && D2Client_IsSkillHotkeySlotUsable(nIdx))
        {
            nBestSlot = nIdx;
            --nRemaining;
            if (nRemaining == 0)
            {
                break;
            }
        }

        nIdx += nStep;
    }

    if (nBestSlot != -1)
    {
        D2Client_SelectActiveSkill(nBestSlot);
    }
}

// 1.12a: 0x6FB07CC0
void __stdcall D2Client_DrawSkillButtonTooltip(
    D2SkillsTxt* pSkillsTxt,
    int32_t nIconTint,
    int32_t nQuantity,
    int32_t nLeftX,
    int32_t nBottomY)
{
    const int32_t nMouseX = gnMouseX_6FBB1638;
    const int32_t nMouseY = gnMouseY_6FBB1634;

    int32_t nTextColor = 1;
    if (nMouseX >= nLeftX && nMouseX <= nLeftX + 48 && nMouseY >= nBottomY - 48 && nMouseY <= nBottomY)
    {
        nTextColor = (nIconTint == 1) ? 1 : 0;
    }

    if (nMouseX < nLeftX || nMouseX > nLeftX + 48 || nMouseY < nBottomY - 48 || nMouseY > nBottomY)
    {
        return;
    }

    const Unicode* const pNewline = D2LANG_GetStringFromTblIndex(0x0F9E);
    Unicode wszTooltip[256];

    const int32_t nSkillId = pSkillsTxt->nSkillId;
    D2SkillDescTxt* pSkillDesc = D2Client_GetSkillDescTxtRecord(nSkillId);
    const uint16_t wStrName = pSkillDesc ? pSkillDesc->wStrName : 0x1506u;

    pSkillDesc = D2Client_GetSkillDescTxtRecord(nSkillId);
    const uint16_t wStrShort = pSkillDesc ? pSkillDesc->wStrShort : 0x1506u;

    Unicode::strcpy(wszTooltip, D2LANG_GetStringFromTblIndex(static_cast<short>(wStrShort)));
    Unicode::strcat(wszTooltip, pNewline);
    Unicode::strcat(wszTooltip, D2LANG_GetStringFromTblIndex(0x105C));
    Unicode::strcat(wszTooltip, D2LANG_GetStringFromTblIndex(0x0F9D));

    Unicode wszQty[10];
    char szQty[16];
    wsprintfA(szQty, "%i", nQuantity);
    Unicode::toUnicode(wszQty, szQty, 9);

    Unicode::strcat(wszTooltip, wszQty);
    Unicode::strcat(wszTooltip, pNewline);
    Unicode::strcat(wszTooltip, D2LANG_GetStringFromTblIndex(static_cast<short>(wStrName)));

    D2Win_10129_DrawFramedText(wszTooltip, nLeftX, nBottomY - 50, nTextColor, 1);
}

// 1.12a: 0x6FB07E80
void __stdcall D2Client_AutoAssignDefaultSkillHotkeys(void)
{
    D2UnitStrc* const pPlayer = gpPlayerUnit_6FBCC3D0;
    if (!UNITS_GetStartSkill(pPlayer))
    {
        FOG_DisplayHalt(s_szEmptyHaltMsg_6FB80323, FOG_GetErrorLocation(1316), 0);
        exit(-1);
    }

    int32_t nHotkeySlot = 0;
    for (int32_t i = 0; i < 60; ++i)
    {
        D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pPlayer, g_tSpeedBar_6FBC21D8.anSkillId[i]);
        if (!pSkill)
        {
            continue;
        }

        D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
        if (!pSkillsTxt)
        {
            continue;
        }

        if ((pSkillsTxt->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_PASSIVE]) != 0)
        {
            continue;
        }

        if (nHotkeySlot <= 15)
        {
            D2Client_AssignSkillHotkey(nHotkeySlot, i);
            ++nHotkeySlot;
        }
    }
}

// 1.12a: 0x6FB07F10
void __stdcall D2Client_CheckDepletedQuantitySkills(void)
{
    D2SkillStrc* pSkill = UNITS_GetStartSkill(gpPlayerUnit_6FBCC3D0);
    if (!pSkill)
    {
        FOG_DisplayHalt(s_szEmptyHaltMsg_6FB80323, FOG_GetErrorLocation(1149), 0);
        exit(-1);
    }

    do
    {
        D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
        if ((pSkillsTxt->dwFlags[1] & gdwBitMasks[SKILLSFLAGINDEX2_SCROLL]) != 0)
        {
            const int32_t nQty = SKILLS_GetQuantity(pSkill);
            D2Client_ClearDepletedItemSkillHotkeys(pSkillsTxt, nQty);
        }
        pSkill = SKILLS_GetNextSkill(pSkill);
    } while (pSkill != nullptr);
}

// 1.12a: 0x6FB07F80
void __stdcall D2Client_DrawSkillHotkeyBindingLabel(
    int32_t nCommandId,
    int32_t nRightX,
    int32_t nBottomY,
    int32_t nColor)
{
    int32_t nBindIdx = -1;
    for (int32_t i = 0; i < 114; ++i)
    {
        if (gKeyBindings_6FBB11A0[i].nCommandId == nCommandId && gKeyBindings_6FBB11A0[i].bIsPrimary == 1)
        {
            nBindIdx = i;
            break;
        }
    }

    if (nBindIdx < 0 || gKeyBindings_6FBB11A0[nBindIdx].wKey == 0xFFFFu)
    {
        return;
    }

    // 0x6FBD36C8 (initialized once via guard bit at 0x6FBD36E8)
    static Unicode s_wszTruncatedBinding_6FBD36C8[16];

    const Unicode* pLabel = CMDTBL_GetShortBindingNameString(nCommandId, 1);
    int32_t nTextWidth = D2Win_10121_GetTextWidth(pLabel);
    if (nTextWidth > 40)
    {
        Unicode::strncpy(s_wszTruncatedBinding_6FBD36C8, pLabel, 16);
        pLabel = s_wszTruncatedBinding_6FBD36C8;
        Unicode* pEnd = &s_wszTruncatedBinding_6FBD36C8[Unicode::strlen(s_wszTruncatedBinding_6FBD36C8)];
        do
        {
            --pEnd;
            *pEnd = Unicode(0);
            nTextWidth = D2Win_10121_GetTextWidth(s_wszTruncatedBinding_6FBD36C8);
        } while (nTextWidth > 40);
    }

    if (nTextWidth > 13)
    {
        nRightX += (13 - nTextWidth);
    }

    D2Win_10117_DrawText(pLabel, nRightX, nBottomY, nColor, FALSE);
}

// 1.12a: 0x6FB080B0
D2CellFileStrc* __fastcall D2Client_GetSkillIconCellFile(D2SkillsTxt* pSkillsTxt)
{
    if (!pSkillsTxt)
    {
        return g_aClassSkillIcons_6FB947F0[gnLocalPlayerCharClass_6FBB33D4].pCellFile;
    }

    const int32_t nClassId = SKILLS_GetClassIdFromSkillId(pSkillsTxt->nSkillId);
    if (nClassId >= 0 && nClassId < 7)
    {
        if (!g_aClassSkillIcons_6FB947F0[nClassId].pCellFile)
        {
            g_aClassSkillIcons_6FB947F0[nClassId].pCellFile = UI_LoadPanelCelAsset(g_aClassSkillIcons_6FB947F0[nClassId].szPath);
        }
        return g_aClassSkillIcons_6FB947F0[nClassId].pCellFile;
    }

    return gpGeneralSkillIconCel_6FBCC1DC;
}

// 1.12a: 0x6FB08110
void __stdcall D2Client_InitSpeedBarAndSkillIcons(void)
{
    gpGeneralSkillIconCel_6FBCC1DC = UI_LoadPanelCelAsset(s_szGeneralSkillIconPath_6FB8528C);

    const uint8_t nLocalClass = gnLocalPlayerCharClass_6FBB33D4;
    D2CellFileStrc* pLocalClassCel = UI_LoadPanelCelAsset(g_aClassSkillIcons_6FB947F0[nLocalClass].szPath);
    g_aClassSkillIcons_6FB947F0[gnLocalPlayerCharClass_6FBB33D4].pCellFile = pLocalClassCel;

    for (int32_t i = 0; i < 16; ++i)
    {
        g_anSkillHotkeyUnk600_6FBC2600[i] = -1;
        g_abSkillHotkeyLeft_6FBC2690[i] = -1;
        g_adwSkillHotkeyOwnerGUID_6FBC2640[i] = static_cast<D2UnitGUID>(-1);
        g_anSkillHotkeyId_6FBC25A0[i] = -1;
    }

    memset(&g_tSpeedBar_6FBC21D8, 0, sizeof(g_tSpeedBar_6FBC21D8));
    for (int32_t i = 0; i < 60; ++i)
    {
        g_tSpeedBar_6FBC21D8.adwOwnerGUID[i] = static_cast<D2UnitGUID>(-1);
    }

    for (int32_t i = 0; i < 4; ++i)
    {
        g_anSkillHotkeyUnk680_6FBC2680[i] = 0;
    }
}

// 1.12a: 0x6FB08300
void __fastcall D2Client_DrawRightSkillHotkeyLabel(
    int32_t nIconX,
    int32_t nIconY,
    D2SkillsTxt* pSkillsTxt,
    D2UnitGUID dwOwnerGUID)
{
    static const int32_t s_anHotkeyCommandIds[16] =
    {
        0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
        0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35
    };

    for (int32_t i = 0; i < 16; ++i)
    {
        const int32_t nBoundSkillId = g_anSkillHotkeyId_6FBC25A0[i];
        if (nBoundSkillId == pSkillsTxt->nSkillId
            && g_adwSkillHotkeyOwnerGUID_6FBC2640[i] == dwOwnerGUID
            && nBoundSkillId != -1
            && g_abSkillHotkeyLeft_6FBC2690[i] == 0)
        {
            D2Client_DrawSkillHotkeyBindingLabel(s_anHotkeyCommandIds[i], nIconX + 34, nIconY - 35, 4);
        }
    }
}

// 1.12a: 0x6FB086D0
void __fastcall D2Client_DrawLeftSkillHotkeyLabel(
    int32_t nIconX,
    int32_t nIconY,
    D2SkillsTxt* pSkillsTxt,
    D2UnitGUID dwOwnerGUID)
{
    static const int32_t s_anHotkeyCommandIds[16] =
    {
        0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
        0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35
    };

    for (int32_t i = 0; i < 16; ++i)
    {
        const int32_t nBoundSkillId = g_anSkillHotkeyId_6FBC25A0[i];
        if (nBoundSkillId == pSkillsTxt->nSkillId
            && g_adwSkillHotkeyOwnerGUID_6FBC2640[i] == dwOwnerGUID
            && nBoundSkillId != -1
            && g_abSkillHotkeyLeft_6FBC2690[i] == 1)
        {
            D2Client_DrawSkillHotkeyBindingLabel(s_anHotkeyCommandIds[i], nIconX + 34, nIconY - 35, 4);
        }
    }
}

// 1.12a: 0x6FB08AA0
void __stdcall D2Client_DrawSpeedBarSkillIcon(
    int32_t nX,
    D2SkillsTxt* pSkillsTxt,
    D2UnitGUID dwOwnerGUID,
    int32_t nQuantityOrCharges,
    int32_t nIconTint,
    int32_t nY,
    BOOL bLeftSide)
{
    D2CellFileStrc* pCellFile = nullptr;
    const int32_t nClassId = SKILLS_GetClassIdFromSkillId(pSkillsTxt->nSkillId);
    if (nClassId >= 0 && nClassId < 7)
    {
        if (!g_aClassSkillIcons_6FB947F0[nClassId].pCellFile)
        {
            g_aClassSkillIcons_6FB947F0[nClassId].pCellFile = UI_LoadPanelCelAsset(g_aClassSkillIcons_6FB947F0[nClassId].szPath);
        }
        pCellFile = g_aClassSkillIcons_6FB947F0[nClassId].pCellFile;
    }
    else
    {
        pCellFile = gpGeneralSkillIconCel_6FBCC1DC;
    }

    D2SkillDescTxt* pSkillDesc = D2Client_GetSkillDescTxtRecord(pSkillsTxt->nSkillId);
    if (!pSkillDesc)
    {
        return;
    }

    const uint32_t nIconCel = pSkillDesc->nIconCel;
    if (gnMouseX_6FBB1638 >= nX && gnMouseX_6FBB1638 <= nX + 48
        && gnMouseY_6FBB1634 >= nY - 48 && gnMouseY_6FBB1634 <= nY)
    {
        switch (nIconTint)
        {
        case 0:
            nIconTint = 0;
            break;
        case 1:
            nIconTint = 1;
            break;
        case 4:
            nIconTint = 4;
            break;
        default:
            nIconTint = 1;
            break;
        }
    }

    D2GfxDataStrc tGfxData = {};
    tGfxData.pCellFile = pCellFile;
    tGfxData.nDirection = 0;
    tGfxData.nFrame = nIconCel;
    TEXTURE_CelDrawColor(&tGfxData, nX, nY, static_cast<uint32_t>(-1), static_cast<DrawMode>(5), nIconTint);

    if (bLeftSide)
    {
        D2Client_DrawLeftSkillHotkeyLabel(nX, nY, pSkillsTxt, dwOwnerGUID);
    }
    else
    {
        D2Client_DrawRightSkillHotkeyLabel(nX, nY, pSkillsTxt, dwOwnerGUID);
    }

    D2Client_DrawSkillIconQuantityOverlay(nQuantityOrCharges, nX, nY);
}

// 1.12a: 0x6FB08BF0
void __stdcall D2Client_DrawActiveSkillButtonIcon(
    int32_t nX,
    int32_t nIconTint,
    D2SkillsTxt* pSkillsTxt,
    D2CellFileStrc* pCellFile,
    D2UnitGUID dwOwnerGUID,
    int32_t nQuantityOrCharges,
    int32_t nY,
    BOOL bLeftSide)
{
    D2SkillDescTxt* pSkillDesc = D2Client_GetSkillDescTxtRecord(pSkillsTxt->nSkillId);
    if (!pSkillDesc)
    {
        return;
    }

    const uint32_t nIconCel = pSkillDesc->nIconCel;
    if (gnMouseX_6FBB1638 >= nX && gnMouseX_6FBB1638 <= nX + 48
        && gnMouseY_6FBB1634 >= nY - 48 && gnMouseY_6FBB1634 <= nY)
    {
        switch (nIconTint)
        {
        case 0:
            nIconTint = 0;
            break;
        case 1:
            nIconTint = 1;
            break;
        case 4:
            nIconTint = 4;
            break;
        default:
            nIconTint = 1;
            break;
        }
    }

    D2GfxDataStrc tGfxData = {};
    tGfxData.pCellFile = pCellFile;
    tGfxData.nDirection = 0;
    tGfxData.nFrame = nIconCel;
    TEXTURE_CelDrawColor(&tGfxData, nX, nY, static_cast<uint32_t>(-1), static_cast<DrawMode>(5), nIconTint);

    if (bLeftSide)
    {
        D2Client_DrawLeftSkillHotkeyLabel(nX, nY, pSkillsTxt, dwOwnerGUID);
    }
    else
    {
        D2Client_DrawRightSkillHotkeyLabel(nX, nY, pSkillsTxt, dwOwnerGUID);
    }

    D2UnitStrc* const pPlayer = gpPlayerUnit_6FBCC3D0;
    if (pPlayer && pPlayer->pInventory)
    {
        D2InventoryStrc* const pInventory = pPlayer->pInventory;
        if (pSkillsTxt->nSkillId == 2) // Throw
        {
            D2UnitStrc* pWeapon = INVENTORY_GetLeftHandWeapon(pInventory);
            if (pWeapon && ITEMS_CheckIfStackable(pWeapon) && ITEMS_CheckIfSocketable(pWeapon))
            {
                nQuantityOrCharges = STATLIST_UnitGetStatValue(pWeapon, STAT_QUANTITY, 0);
            }
        }
        else if (pSkillsTxt->nSkillId == 4) // Left Hand Throw
        {
            D2UnitStrc* pPrimary = INVENTORY_GetLeftHandWeapon(pInventory);
            D2UnitStrc* pRightArm = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM);
            D2UnitStrc* pLeftArm = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM);
            D2UnitStrc* pOffHand = (pPrimary == pRightArm) ? pLeftArm : pRightArm;

            if (pOffHand
                && ITEMS_CheckItemTypeId(pOffHand, ITEMTYPE_WEAPON)
                && ITEMS_CheckIfStackable(pOffHand)
                && ITEMS_CheckIfSocketable(pOffHand))
            {
                nQuantityOrCharges = STATLIST_UnitGetStatValue(pOffHand, STAT_QUANTITY, 0);
            }
        }
    }

    D2Client_DrawSkillIconQuantityOverlay(nQuantityOrCharges, nX, nY);
}

// 1.12a: 0x6FB08D80
void __stdcall D2Client_DrawActiveSkillButtonTooltip(
    int32_t nX,
    int32_t nY,
    D2SkillsTxt* pSkillsTxt,
    int32_t nIconTint,
    int32_t nSkillLevel,
    int32_t nCharges)
{
    const int32_t nMouseX = gnMouseX_6FBB1638;
    const int32_t nMouseY = gnMouseY_6FBB1634;

    int32_t nColor = 1;
    if (nMouseX >= nX && nMouseX <= nX + 48 && nMouseY >= nY - 48 && nMouseY <= nY)
    {
        nColor = (nIconTint == 1) ? 1 : 0;
    }

    if (nMouseX >= nX && nMouseX <= nX + 48 && nMouseY >= nY - 48 && nMouseY <= nY)
    {
        SKILLDESC_DrawActiveSkillTooltip(
            gpPlayerUnit_6FBCC3D0,
            pSkillsTxt->nSkillId,
            nSkillLevel,
            nCharges,
            nColor,
            nX,
            nY);
    }
}

// 1.12a: 0x6FB08E00 (uses 4-case switch jump table at 0x6FB09130)
void __stdcall D2Client_LayoutAndRenderSpeedBarPopup(
    D2UnitStrc* pUnit,
    int32_t nStartX,
    BOOL bLeftSide,
    BOOL bDrawTooltipsOnly,
    BOOL bLayoutOnlyNoDraw)
{
    int32_t nCurY = gnScreenHeight_6FB8C6E4 - 86;
    const int32_t nStepX = bLeftSide ? 48 : -48;
    uint8_t abUsedListPool[256] = {};
    int32_t nPrevRow = 0;

    for (int32_t nRow = 0; nRow < 5; ++nRow)
    {
        int32_t nCurX = nStartX;
        D2SkillStrc* pNextSkill = nullptr;

        for (D2SkillStrc* pSkill = UNITS_GetStartSkill(pUnit); pSkill != nullptr; pSkill = pNextSkill)
        {
            pNextSkill = SKILLS_GetNextSkill(pSkill);
            D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
            if (!pSkillsTxt)
            {
                continue;
            }

            const int32_t nSkillId = pSkillsTxt->nSkillId;
            if (nSkillId < 0 || !sgptDataTables || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
            {
                continue;
            }

            D2SkillsTxt* pTableSkill = &sgptDataTables->pSkillsTxt[nSkillId];
            if (!pTableSkill)
            {
                continue;
            }

            const int32_t nSkillDescId = static_cast<int16_t>(pTableSkill->wSkillDesc);
            if (nSkillDescId < 0 || nSkillDescId >= sgptDataTables->nSkillDescTxtRecordCount)
            {
                continue;
            }

            D2SkillDescTxt* pSkillDesc = &sgptDataTables->pSkillDescTxt[nSkillDescId];
            if (!pSkillDesc)
            {
                continue;
            }

            if (bLeftSide && (pSkillsTxt->dwFlags[0] & SKILLSFLAG_LEFTSKILL) == 0)
            {
                continue;
            }

            if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_PASSIVE) != 0)
            {
                continue;
            }

            if ((SKILLS_GetFlags(pSkill) & SKFLAG_NEEDCORPSE) != 0)
            {
                continue;
            }

            D2UnitGUID dwOwnerGUID = static_cast<D2UnitGUID>(-1);
            int32_t nCharges = 0;
            BOOL bIsItemOrScrollRow = SKILLS_GetSkillInfo(pSkill, &dwOwnerGUID, nullptr, nullptr, &nCharges);

            int32_t nQuantity = 0;
            if (!bLeftSide && !bIsItemOrScrollRow)
            {
                nQuantity = SKILLS_GetQuantity(pSkill);
                if (nQuantity > 0 && (pSkillsTxt->dwFlags[1] & SKILLSFLAG2_SCROLL) != 0)
                {
                    bIsItemOrScrollRow = TRUE;
                }
            }

            if (nRow < 4)
            {
                if (bIsItemOrScrollRow || static_cast<int8_t>(pSkillDesc->nListRow) != nRow)
                {
                    continue;
                }
            }
            else
            {
                if (!bIsItemOrScrollRow)
                {
                    continue;
                }
            }

            int32_t nIconTint = 0;
            const int32_t nUseState = SKILLS_GetUseState(pUnit, pSkill);
            if (nUseState == SKILLUSTATE_USABLE)
            {
                if (pUnit == gpPlayerUnit_6FBCC3D0 && gdwClientGameTick_6FBB32B0 < gdwSkillDelayEndFrame_6FBB3314)
                {
                    nIconTint = 1;
                }
            }
            else if (nUseState == SKILLUSTATE_AURA)
            {
                nIconTint = 4;
            }
            else
            {
                nIconTint = 1;
            }

            if (nPrevRow < nRow)
            {
                nCurY -= 48;
                nPrevRow = nRow;
            }

            const uint8_t nListPool = pSkillDesc->nListPool;
            if (nListPool > 0)
            {
                if (abUsedListPool[nListPool] > 0)
                {
                    continue;
                }
                abUsedListPool[nListPool] = 1;
            }

            if (!bDrawTooltipsOnly)
            {
                if (!bLayoutOnlyNoDraw)
                {
                    D2Client_DrawSpeedBarSkillIcon(nCurX, pSkillsTxt, dwOwnerGUID, nCharges, nIconTint, nCurY, bLeftSide);
                }
                nCurX = D2Client_AddSpeedBarSkillEntry(nCurX, nCurY, pSkill, bLeftSide, nIconTint);
            }
            else
            {
                if (nQuantity == 0)
                {
                    const int32_t nSkillLvl = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
                    D2Client_DrawActiveSkillButtonTooltip(nCurX, nCurY, pSkillsTxt, nIconTint, nSkillLvl, nCharges);
                }
                else
                {
                    D2Client_DrawSkillButtonTooltip(pSkillsTxt, nIconTint, nQuantity, nCurX, nCurY);
                }
                nCurX += nStepX;
            }

            // Switch jump table at 0x6FB09130 (cases 1..4)
            switch (nRow)
            {
            case 1:
                g_anSpeedBarRowPosY_6FBC25E4[0] = nCurY;
                break;
            case 2:
                g_anSpeedBarRowPosY_6FBC25E4[1] = nCurY;
                break;
            case 3:
                g_anSpeedBarRowPosY_6FBC25E4[2] = nCurY;
                break;
            case 4:
                g_anSpeedBarRowPosY_6FBC25E4[3] = nCurY;
                break;
            default:
                break;
            }
        }
    }
}

// 1.12a: 0x6FB09140
void __stdcall D2Client_DrawRightSpeedBarTooltips(void)
{
    if (!gbSpeedBarLeftSide_6FBCC1E0)
    {
        D2Client_LayoutAndRenderSpeedBarPopup(
            gpPlayerUnit_6FBCC3D0,
            gnScreenWidth_6FB8C6E0 - 128,
            FALSE,
            TRUE,
            FALSE);
    }
}

// 1.12a: 0x6FB09170
void __stdcall D2Client_DrawLeftSpeedBarTooltips(void)
{
    if (gbSpeedBarLeftSide_6FBCC1E0)
    {
        D2Client_LayoutAndRenderSpeedBarPopup(
            gpPlayerUnit_6FBCC3D0,
            80,
            TRUE,
            TRUE,
            FALSE);
    }
}

// 1.12a: 0x6FB09190
void __stdcall D2Client_DrawRightSpeedBarIcons(BOOL bLayoutOnlyNoDraw)
{
    if (!gbSpeedBarLeftSide_6FBCC1E0)
    {
        g_tSpeedBar_6FBC21D8.nCount = 0;
        D2Client_LayoutAndRenderSpeedBarPopup(
            gpPlayerUnit_6FBCC3D0,
            gnScreenWidth_6FB8C6E0 - 128,
            FALSE,
            FALSE,
            bLayoutOnlyNoDraw);
    }
}

// 1.12a: 0x6FB091D0
void __stdcall D2Client_DrawLeftSpeedBarIcons(void)
{
    if (gbSpeedBarLeftSide_6FBCC1E0)
    {
        g_tSpeedBar_6FBC21D8.nCount = 0;
        D2Client_LayoutAndRenderSpeedBarPopup(
            gpPlayerUnit_6FBCC3D0,
            80,
            TRUE,
            FALSE,
            FALSE);
    }
}

// 1.12a: 0x6FB09200
void __stdcall D2Client_RenderSpeedBarPopup(void)
{
    memset(&g_tSpeedBar_6FBC21D8, 0, sizeof(g_tSpeedBar_6FBC21D8));
    for (int32_t i = 0; i < 60; ++i)
    {
        g_tSpeedBar_6FBC21D8.adwOwnerGUID[i] = static_cast<D2UnitGUID>(-1);
    }

    D2Client_DrawLeftSpeedBarIcons();
    D2Client_DrawRightSpeedBarIcons(FALSE);
    D2Client_DrawLeftSpeedBarTooltips();
    D2Client_DrawRightSpeedBarTooltips();
}

// 1.12a: 0x6FB092C0
BOOL __stdcall D2Client_HandleSpeedBarMouseClick(D2StormMsgStrc* pMsg)
{
    if (!gpPlayerUnit_6FBCC3D0)
    {
        FOG_DisplayHalt(s_szEmptyHaltMsg_6FB80323, FOG_GetErrorLocation(954), 0);
        exit(-1);
    }

    const int32_t nInitialMouseY = gnMouseY_6FBB1634;
    BOOL bSelectedSkill = FALSE;

    for (int32_t i = 0; i < 60; ++i)
    {
        const int32_t nIconX = g_tSpeedBar_6FBC21D8.anPosX[i];
        const int32_t nIconY = g_tSpeedBar_6FBC21D8.anPosY[i];
        const int32_t nMouseX = gnMouseX_6FBB1638;
        const int32_t nMouseY = gnMouseY_6FBB1634;

        if (nMouseX < nIconX || nMouseX > nIconX + 48 || nMouseY < nIconY - 48 || nMouseY > nIconY)
        {
            continue;
        }

        const int32_t nSkillId = g_tSpeedBar_6FBC21D8.anSkillId[i];
        const D2UnitGUID dwOwnerGUID = g_tSpeedBar_6FBC21D8.adwOwnerGUID[i];
        D2SkillStrc* pSkill = SKILLS_GetSkillById(gpPlayerUnit_6FBCC3D0, nSkillId, dwOwnerGUID);
        if (!pSkill)
        {
            continue;
        }

        if (SKILLS_GetOwnerGUIDFromSkill(pSkill) != static_cast<D2UnitGUID>(-1))
        {
            int32_t nCharges = 0;
            if (SKILLS_GetSkillInfo(pSkill, nullptr, nullptr, nullptr, &nCharges) && nCharges <= 0)
            {
                const int32_t bWasOpen = gaPanelStates_6FBB2B58[3];
                if (bWasOpen)
                {
                    FOG_IsExpansion();
                    if (gnPanelCount_6FB8F368 != 38)
                    {
                        FOG_DisplayHalt(s_szEmptyHaltMsg_6FB80323, FOG_GetErrorLocation(5001), 0);
                        exit(-1);
                    }
                    gaPanelStates_6FBB2B58[3] = 0;
                    SMSG_UnregisterCallbacks(1, WINDOW_GetWindow(), &g_tSpeedBarMsgReg_6FB813D0);
                }

                pMsg->dwReturn = 0;
                pMsg->bHandled = 1;
                SMsgBreakHandlerChain(pMsg);
                return FALSE;
            }
        }

        D2C2SSelectSkillPacketStrc tPacket;
        tPacket.nOpcode = 0x3C;
        tPacket.dwSkillIdAndSide = static_cast<uint32_t>(nSkillId) | (gbSpeedBarLeftSide_6FBCC1E0 ? 0x80000000u : 0u);
        tPacket.dwOwnerGUID = dwOwnerGUID;
        MSG_SendPacket(&tPacket, sizeof(tPacket));

        const int32_t bWasOpen = gaPanelStates_6FBB2B58[3];
        FOG_IsExpansion();
        if (gnPanelCount_6FB8F368 != 38)
        {
            FOG_DisplayHalt(s_szEmptyHaltMsg_6FB80323, FOG_GetErrorLocation(5001), 0);
            exit(-1);
        }

        gaPanelStates_6FBB2B58[3] = 0;
        if (bWasOpen)
        {
            SMSG_UnregisterCallbacks(1, WINDOW_GetWindow(), &g_tSpeedBarMsgReg_6FB813D0);
        }

        gbSpeedBarLeftSide_6FBCC1E0 = FALSE;
        bSelectedSkill = TRUE;
        GAME_ResetMouseButtonStates();
    }

    if (nInitialMouseY <= gnScreenHeight_6FB8C6E4 - 49 && !bSelectedSkill && gaPanelStates_6FBB2B58[3] != 0)
    {
        UI_SetPanelState(3, 1, 0);
    }

    pMsg->dwReturn = 0;
    pMsg->bHandled = 1;
    SMsgBreakHandlerChain(pMsg);
    return FALSE;
}

// ============================================================================
// PART 2: Early UNIT/Player.cpp (0x6FB094C0..0x6FB099AF)
// ============================================================================

// 1.12a: 0x6FB094C0
BOOL __fastcall D2Client_GetLocalPlayerSaveGfxInfo(uint8_t* pColorOut, uint32_t nBufferSize)
{
    D2UnitStrc* const pPlayer = gpPlayerUnit_6FBCC3D0;
    uint8_t abUnusedComponents[16];

    if (nBufferSize < 16 || !pPlayer)
    {
        return FALSE;
    }

    memset(pColorOut, 0xFF, 16);
    INVENTORY_GetItemSaveGfxInfo(pPlayer, abUnusedComponents, pColorOut);
    return TRUE;
}

// 1.12a: 0x6FB09500
BOOL __fastcall D2Client_GetUnitSaveGfxInfo(uint8_t* pComponentsOut, uint32_t nBufferSize)
{
    D2UnitStrc* const pPlayer = gpPlayerUnit_6FBCC3D0;
    uint8_t abUnusedColors[16];

    if (nBufferSize < 16 || !pPlayer)
    {
        return FALSE;
    }

    memset(pComponentsOut, 0xFF, 16);
    INVENTORY_GetItemSaveGfxInfo(pPlayer, pComponentsOut, abUnusedColors);
    return TRUE;
}

// 1.12a: 0x6FB09540
uint8_t __stdcall D2Client_GetPlayerEquippedItemComponent(D2UnitStrc* pPlayerUnit, D2UnitStrc* pItem)
{
    const uint8_t nBodyLoc = ITEMS_GetBodyLocation(pItem);
    if (nBodyLoc == BODYLOC_RARM || nBodyLoc == BODYLOC_LARM)
    {
        D2InventoryStrc* const pInventory = pPlayerUnit->pInventory;
        D2UnitStrc* const pRightArmItem = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM);
        D2UnitStrc* const pLeftArmItem = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM);

        if (pRightArmItem
            && pLeftArmItem
            && !ITEMS_CheckWeaponIfTwoHanded(pRightArmItem)
            && !ITEMS_CheckWeaponIfTwoHanded(pLeftArmItem)
            && pItem != INVENTORY_GetLeftHandWeapon(pInventory))
        {
            return 6; // Off-hand dual-wield component (COMP_LH)
        }
    }

    return ITEMS_GetComponent(pItem);
}

// 1.12a: 0x6FB095C0
void __stdcall D2Client_ClearPlayerMouseActionLock(void)
{
    g_bPlayerMouseActionLock_6FBCC1D8 = 0;
}

// 1.12a: 0x6FB095D0
BOOL __stdcall D2Client_IsPlayerSkillWithinRange(
    int32_t* pOutRange,
    D2SkillStrc* pSkill,
    D2UnitStrc* pTargetUnit,
    int32_t* pOutDistance)
{
    D2UnitStrc* const pPlayer = gpPlayerUnit_6FBCC3D0;
    if (!pPlayer || !pSkill || !pOutDistance)
    {
        return FALSE;
    }

    D2SkillsTxt* const pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
    if (!pSkillsTxt)
    {
        return FALSE;
    }

    const int32_t nSkillLevel = SKILLS_GetSkillLevel(pPlayer, pSkill, 1);
    *pOutRange = SKILLS_EvaluateSkillFormula(pPlayer, pSkillsTxt->dwAuraRangeCalc, pSkillsTxt->nSkillId, nSkillLevel);

    const int32_t nDistance = UNITS_GetDistanceToOtherUnit(pPlayer, pTargetUnit);
    *pOutDistance = nDistance;
    return (nDistance <= *pOutRange) ? TRUE : FALSE;
}

// 1.12a: 0x6FB09650
BOOL __fastcall D2Client_ValidatePlayerInputAction(D2PlayerActionStrc* pAction)
{
    const uint32_t dwFlags = pAction->dwFlags;
    if ((dwFlags & PLRACTIONFLAG_BUTTONUP) != 0)
    {
        g_nHeldMouseTargetState_6FBD365C = 0;
        return FALSE;
    }

    if ((dwFlags & PLRACTIONFLAG_BUTTONDOWN) != 0)
    {
        g_nHeldMouseTargetState_6FBD365C = (pAction->pTargetUnit != nullptr) ? 2 : 1;
        return TRUE;
    }

    if ((dwFlags & PLRACTIONFLAG_BUTTONHELD) != 0)
    {
        if (g_nHeldMouseTargetState_6FBD365C == 0)
        {
            return FALSE;
        }

        if (pAction->pTargetUnit != nullptr)
        {
            const int32_t nTargetType = static_cast<int32_t>(pAction->pTargetUnit->dwUnitType);
            if (nTargetType < UNIT_PLAYER || (nTargetType > UNIT_MONSTER && nTargetType != UNIT_TILE))
            {
                return FALSE;
            }

            if (pAction->pPlayerUnit != nullptr)
            {
                const uint32_t dwMode = pAction->pPlayerUnit->dwAnimMode;
                if (dwMode == 2 || dwMode == 3 || dwMode == 6)
                {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

// 1.12a: 0x6FB096C0
BOOL __fastcall D2Client_CanUseSkillInCurrentRoom(D2SkillStrc* pSkill)
{
    if (!pSkill)
    {
        return FALSE;
    }

    const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, s_szPlayerFile_6FB85244, 1882);
    D2SkillsTxt* pSkillsTxt = nullptr;
    if (nSkillId >= 0 && sgptDataTables && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
    {
        pSkillsTxt = &sgptDataTables->pSkillsTxt[nSkillId];
    }

    if ((pSkillsTxt->dwFlags[0] & SKILLSFLAG_INTOWN) != 0)
    {
        return TRUE;
    }
    return FALSE;
}

// 1.12a: 0x6FB09710
BOOL __stdcall D2Client_ComputePlayerStepMoveTarget(D2UnitStrc* pUnit, D2PlayerActionStrc* pAction)
{
    int32_t nUnitX = 0;
    const int32_t nTypeX = static_cast<int32_t>(pUnit->dwUnitType);
    if (nTypeX == UNIT_OBJECT || (nTypeX > UNIT_MISSILE && nTypeX <= UNIT_TILE))
    {
        nUnitX = pUnit->pStaticPath->nXPos;
    }
    else if (pUnit->pDynamicPath)
    {
        nUnitX = PATH_GetXPosition(pUnit->pDynamicPath);
    }

    int32_t nUnitY = 0;
    const int32_t nTypeY = static_cast<int32_t>(pUnit->dwUnitType);
    if (nTypeY == UNIT_OBJECT || (nTypeY > UNIT_MISSILE && nTypeY <= UNIT_TILE))
    {
        nUnitY = pUnit->pStaticPath->nYPos;
    }
    else if (pUnit->pDynamicPath)
    {
        nUnitY = PATH_GetYPosition(pUnit->pDynamicPath);
    }

    int32_t nDeltaX = pAction->nTargetX - nUnitX;
    int32_t nDeltaY = pAction->nTargetY - nUnitY;

    const int32_t nAbsX = (nDeltaX < 0) ? -nDeltaX : nDeltaX;
    if (nAbsX >= 256)
    {
        return FALSE;
    }

    const int32_t nAbsY = (nDeltaY < 0) ? -nDeltaY : nDeltaY;
    if (nAbsY >= 256)
    {
        return FALSE;
    }

    if (nAbsX < 1 && nAbsY < 1)
    {
        return FALSE;
    }

    int32_t nDist = LIGHTMAP_ApproxDistanceB(nDeltaX, nDeltaY);
    if (nDist <= 1)
    {
        nDist = 1;
    }

    const uint32_t dwFlags = pAction->dwFlags;
    int32_t nMinDist = 3;
    if ((dwFlags & PLRACTIONFLAG_BUTTONDOWN) != 0 && (dwFlags & PLRACTIONFLAG_RUN) != 0)
    {
        nMinDist = 5;
    }
    else if ((dwFlags & PLRACTIONFLAG_BUTTONHELD) != 0)
    {
        nMinDist = static_cast<int32_t>(((dwFlags >> 6) & 1u) | 4u);
    }
    else if ((dwFlags & PLRACTIONFLAG_BUTTONUP) != 0)
    {
        nMinDist = 0;
    }

    if (nDist < nMinDist)
    {
        const double dScale = static_cast<double>(nMinDist) / static_cast<double>(nDist);
        nDeltaX = static_cast<int32_t>(static_cast<double>(nDeltaX) * dScale);
        nDeltaY = static_cast<int32_t>(static_cast<double>(nDeltaY) * dScale);
    }

    pAction->nTargetX = nUnitX + nDeltaX;
    pAction->nTargetY = nUnitY + nDeltaY;

    const uint32_t dwCurTick = gdwClientGameTick_6FBB32B0;
    if (pUnit->dwUnitType == UNIT_PLAYER && (dwFlags & PLRACTIONFLAG_BUTTONHELD) != 0)
    {
        const uint32_t dwMode = pUnit->dwAnimMode;
        if ((dwMode == 2 || dwMode == 3 || dwMode == 6)
            && (dwCurTick - g_dwLastStepMoveTick_6FBD3660) < 7u)
        {
            return FALSE;
        }
    }

    g_dwLastStepMoveTick_6FBD3660 = dwCurTick;
    return TRUE;
}

// 1.12a: 0x6FB098F0
void __fastcall D2Client_OnPlayerDeathCleanup(D2UnitStrc* pPlayerUnit)
{
    gwRightHandWeaponClass_6FBCBCE4 = 0;
    gwLeftHandWeaponClass_6FBCBCE8 = 0;

    PATH_SetUnitDeadCollision(pPlayerUnit, FALSE);
    if (pPlayerUnit)
    {
        pPlayerUnit->dwFlags = (pPlayerUnit->dwFlags & ~UNITFLAG_CANBEATTACKED) | UNITFLAG_ISDEAD;
    }

    if (!STATES_CheckState(pPlayerUnit, STATE_PLAYERBODY) && pPlayerUnit)
    {
        pPlayerUnit->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    GFX_FreeDeadUnitCofCache(pPlayerUnit);

    D2UnitGfxDataStrc* const pGfxData = pPlayerUnit ? reinterpret_cast<D2UnitGfxDataStrc*>(pPlayerUnit->pGfxData) : nullptr;
    if (!pGfxData)
    {
        FOG_DisplayHalt(s_szEmptyHaltMsg_6FB80323, FOG_GetErrorLocation(3002), 0);
        exit(-1);
    }

    for (int32_t i = 0; i < 20; ++i)
    {
        pGfxData->aLayerFlags[i] &= 0xFF00u;
    }

    STATLIST_SetUnitStat(pPlayerUnit, STAT_HITPOINTS, 0, 0);
}

// 1.12a: 0x6FB09990
void __fastcall D2Client_FreeUnitHoverText(D2UnitStrc* pUnit)
{
    D2LightNodeStrc* const pLight = static_cast<D2LightNodeStrc*>(D2Common_10958(pUnit, nullptr));
    if (pLight)
    {
        LIGHTMAP_DestroyLightNode(pLight);
    }
}

D2SkillDescTxt* __fastcall D2Client_GetSkillDescTxtRecord(int32_t nSkillDescId)
{
    if (!sgptDataTables || nSkillDescId <= 0 || nSkillDescId >= sgptDataTables->nSkillDescTxtRecordCount) return nullptr;
    return &sgptDataTables->pSkillDescTxt[nSkillDescId];
}
int32_t __fastcall D2Client_IsSkillHotkeySlotUsable(int32_t nSlot) { return (nSlot >= 0 && nSlot < 16) ? 1 : 0; }
void __stdcall GAME_ResetMouseButtonStates(void) { GAME_ResetAllMouseStates(); }
void __fastcall GFX_FreeDeadUnitCofCache(D2UnitStrc* pUnit) { (void)pUnit; }
void __fastcall SMSG_UnregisterCallbacks(int32_t nCount, HWND hWnd, const D2MsgHandlerRegStrc* pEntries)
{
    WALL2_UnregisterMsgHandlerTable(static_cast<uint32_t>(nCount), hWnd, reinterpret_cast<const D2SMsgRegEntryStrc*>(pEntries));
}
void __stdcall SKILLDESC_DrawActiveSkillTooltip(
    D2UnitStrc* pPlayerUnit,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nCharges,
    int32_t nColor,
    int32_t nLeftX,
    int32_t nBottomY)
{
    (void)nCharges; (void)nColor;
    D2SkillStrc* pSkill = pPlayerUnit ? SKILLS_GetSkillById(pPlayerUnit, nSkillId, static_cast<D2UnitGUID>(-1)) : nullptr;
    SKILLDESC_DrawActiveSkillTooltip(nSkillId, nSkillLevel, pSkill, (nLeftX < 400) ? 1 : 0, nLeftX + 24, nBottomY);
}
void __fastcall D2Client_AssignSkillHotkey(int32_t nSlot, int32_t nSkillId, int32_t bLeft, uint32_t dwItemGUID)
{
    if (nSlot < 0 || nSlot >= 16)
    {
        return;
    }
    g_anSkillHotkeyId_6FBC25A0[nSlot] = nSkillId;
    g_abSkillHotkeyLeft_6FBC2690[nSlot] = (nSkillId == -1) ? -1 : (bLeft ? 1 : 0);
    g_adwSkillHotkeyOwnerGUID_6FBC2640[nSlot] = dwItemGUID;
}
