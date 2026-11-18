// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Translation Unit: UI_options_minipanel.cpp
// Sources:
//   - ..\Source\D2Client\UI\minipanel.cpp & ..\Source\D2Client\UI\options.cpp (0x6FB02DD0..0x6FB0634F)
//   - ..\Source\D2Client\UI\plrtrade.cpp  (Player Trade & Prompt Dialogs, 0x6FB06350..0x6FB0744F)
//   - ..\Source\D2Client\UI\spellsel.cpp  (Speedbar / Skill Check Head, 0x6FB07450..0x6FB07520)
// ImageBase: 0x6FAB0000
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "D2ClientReconstructed.h"
#include <D2Unicode.h>

// ============================================================================
// Forward Declarations & Typed Engine Structures (Zero Raw Offset Casts)
// ============================================================================
struct D2ActiveRoomStrc;
typedef D2ActiveRoomStrc D2RoomStrc;
struct D2CellFileStrc;
struct D2InventoryStrc;
struct D2SkillListStrc;
struct D2SkillStrc;
struct D2StatListExStrc;
struct D2ViewportStrc;
struct D2PlayerDataStrc;
struct D2HoverTextStrc;
struct D2CombatStrc;
struct D2AnimDataRecordStrc;
struct D2AnimSeqTxt;
struct D2GfxLightStrc;
struct D2UnitEventStrc;
struct D2EventTimerStrc;
struct D2UnitPacketListStrc;
struct D2DynamicPathStrc;

typedef uint32_t D2UnitGUID;

#pragma pack(push, 1)
struct D2SeedStrc
{
    uint32_t dwLowSeed;                         // +0x00
    uint32_t dwHighSeed;                        // +0x04
};

struct D2UnitStrc
{
    uint32_t dwUnitType;                        // +0x00
    int32_t dwClassId;                          // +0x04
    void* pMemoryPool;                          // +0x08
    D2UnitGUID dwUnitId;                        // +0x0C
    uint32_t dwAnimMode;                        // +0x10
    D2PlayerDataStrc* pPlayerData;              // +0x14
    uint8_t nAct;                               // +0x18
    uint8_t unk0x19[3];                         // +0x19
    D2DrlgActStrc* pDrlgAct;                    // +0x1C
    D2SeedStrc pSeed;                           // +0x20
    uint32_t dwInitSeed;                        // +0x28
    D2DynamicPathStrc* pDynamicPath;            // +0x2C
    D2AnimSeqTxt* pAnimSeq;                     // +0x30
    uint32_t dwSeqFrameCount;                   // +0x34
    int32_t dwSeqFrame;                         // +0x38
    uint32_t dwSeqSpeed;                        // +0x3C
    uint32_t dwSeqMode;                         // +0x40
    int32_t nSeqCurrentFramePrecise;            // +0x44
    int32_t dwFrameCountPrecise;                // +0x48
    int16_t wAnimSpeed;                         // +0x4C
    uint8_t nActionFrame;                       // +0x4E
    uint8_t unk0x4F;                            // +0x4F
    const D2AnimDataRecordStrc* pAnimData;      // +0x50
    D2GfxDataStrc* pGfxData;                    // +0x54
    D2GfxDataStrc* pGfxDataCopy;                // +0x58
    D2StatListExStrc* pStatListEx;              // +0x5C
    D2InventoryStrc* pInventory;                // +0x60
    D2GfxLightStrc* pLight;                     // +0x64
    uint32_t dwStartLight;                      // +0x68
    int32_t nPaletteIndex;                      // +0x6C
    BOOL bUnitSfx;                              // +0x70
    uint32_t dwSfxMode;                         // +0x74
    void* pUnitSfxData;                         // +0x78
    uint32_t dwSfxTicks;                        // +0x7C
    uint32_t dwSfxAsyncTicks;                   // +0x80
    uint32_t dwSfxStepTicks;                    // +0x84
    BOOL bHasActiveSound;                       // +0x88
    uint16_t nLastClickX;                       // +0x8C
    uint16_t nLastClickY;                       // +0x8E
    D2UnitEventStrc* pCltTimerList;             // +0x90
    uint32_t dwOwnerType;                       // +0x94
    D2UnitGUID dwOwnerGUID;                     // +0x98
    uint32_t dwKillerType;                      // +0x9C
    D2UnitGUID dwKillerGUID;                    // +0xA0
    D2HoverTextStrc* pHoverText;                // +0xA4
    D2SkillListStrc* pSkills;                   // +0xA8
    D2CombatStrc* pCombat;                      // +0xAC
    uint32_t dwLastHitClass;                    // +0xB0
    uint32_t dwClientInteracted;                // +0xB4
    uint32_t dwDropItemCode;                    // +0xB8
    uint32_t unk0xBC[2];                        // +0xBC
    uint32_t dwFlags;                           // +0xC4
    uint32_t dwFlagEx;                          // +0xC8
    void* pUnused0xCC;                          // +0xCC
    uint32_t dwNodeIndex;                       // +0xD0
    uint32_t dwTickCount;                       // +0xD4
    void* pPacketList;                          // +0xD8
    D2EventTimerStrc* pTimer;                   // +0xDC
    D2UnitStrc* pChangeNextUnit;                // +0xE0
    D2UnitStrc* pListNext;                      // +0xE4
    D2UnitStrc* pRoomNext;                      // +0xE8
    D2UnitPacketListStrc* pMsgFirst;            // +0xEC
    D2UnitPacketListStrc* pMsgLast;             // +0xF0
};
static_assert(sizeof(D2UnitStrc) == 0xF4, "1.12a D2UnitStrc must be 0xF4 bytes");

struct D2SkillsTxt
{
    int16_t nSkillId;                           // +0x00
    uint16_t wFlags;                            // +0x02
};

struct D2ClientGameDataStrc
{
    uint8_t pad000[0x1EA];                      // +0x000
    uint16_t wCharFlags;                        // +0x1EA (bit 2 = Hardcore, bit 3 = Hardcore Dead, bit 5 = Expansion)
};

struct D2CltTradeButtonPacketStrc
{
    uint8_t bOpcode;                            // +0x00 (0x4F)
    uint16_t wSubCmd;                           // +0x01 (2 = Cancel, 3 = AcceptPrompt, 4 = CheckAccept, 7 = UncheckAccept, 8 = GoldOffer)
    uint16_t wGoldHigh;                         // +0x03
    uint16_t wGoldLow;                          // +0x05
};
static_assert(sizeof(D2CltTradeButtonPacketStrc) == 0x07, "C2S 0x4F packet must be 7 bytes");

struct D2SrvTradePartnerPacketStrc
{
    uint8_t bOpcode;                            // +0x00
    char szPartnerName[16];                     // +0x01
    uint32_t dwPartnerGUID;                     // +0x11
};

struct D2SrvTradeActionPacketStrc
{
    uint8_t bOpcode;                            // +0x00 (0x77)
    uint8_t nAction;                            // +0x01 (0..21)
};
#pragma pack(pop)

struct D2WinMsgStrc
{
    HWND hWnd;                                  // +0x00
    UINT uMsg;                                  // +0x04
    WPARAM wParam;                              // +0x08
    union
    {
        LPARAM lParam;                          // +0x0C
        struct
        {
            uint16_t nX;                        // +0x0C
            uint16_t nY;                        // +0x0E
        };
    };
    uint32_t dwCommand;                         // +0x10
    uint32_t dwKeyFlags;                        // +0x14
    int32_t bHandled;                           // +0x18
    LRESULT lResult;                            // +0x1C
};
static_assert(sizeof(D2WinMsgStrc) == 0x20, "1.12a D2WinMsgStrc must be 0x20 bytes");

struct D2SMsgRegEntryStrc
{
    uint32_t nDispatchType;                     // +0x00 (0 = WM_*, 3 = VK_*)
    uint32_t dwMsgOrKey;                        // +0x04
    void (__stdcall* pfnHandler)(D2WinMsgStrc* pMsg); // +0x08
};

struct D2PanelRuleRowStrc
{
    int32_t aActionOnOpen[D2PANEL_COUNT];       // +0x00..+0x97 (0=keep, 1=close, 2/3=block, 4=closeNpc)
};

// Dialog structures matching UI\dialog.cpp (0x6FB12960..0x6FB14190)
struct D2DialogWidgetNodeStrc;

struct D2DialogLineStrc
{
    uint16_t wszText[120];                      // +0x00
    int32_t nLineHeight;                        // +0xF0
    int32_t nTextOffsetX;                       // +0xF4
    int32_t nTextWidth;                         // +0xF8
    int32_t nUnkFC;                             // +0xFC
    int32_t nFont;                              // +0x100
    int32_t nColor;                             // +0x104
    int (__cdecl* pfnAction)(void);             // +0x108
    int32_t bSelectable;                        // +0x10C
};
static_assert(sizeof(D2DialogLineStrc) == 0x110, "1.12a D2DialogLineStrc must be 0x110 bytes");

struct D2DialogStrc
{
    uint32_t dwCreationTick;                    // +0x00
    uint32_t dwUnk04;                           // +0x04
    D2CellFileStrc* pBackgroundCel;             // +0x08
    int32_t bHasBackgroundCel;                  // +0x0C
    int32_t bMousePressedInDialog;              // +0x10
    int32_t bUseNonSelectableHeightAnchor;      // +0x14
    int32_t bBlockUnhandledKeys;                // +0x18
    int32_t bCloseOnRightClick;                 // +0x1C
    int32_t bIgnoreMenuHotkeys;                 // +0x20
    int32_t nAnchorX;                           // +0x24
    int32_t nAnchorY;                           // +0x28
    int32_t nPosX;                              // +0x2C
    int32_t nPosY;                              // +0x30
    int32_t nWidth;                             // +0x34
    int32_t nHeight;                            // +0x38
    int32_t nUnk3C;                             // +0x3C
    int32_t nUnk40;                             // +0x40
    int32_t nSelectedIndex;                     // +0x44
    int32_t nHoveredIndex;                      // +0x48
    int32_t nSelectableLineCount;               // +0x4C
    int32_t nLineCount;                         // +0x50
    uint32_t dwFrameCount;                      // +0x54
    int32_t bAutoCalcBounds;                    // +0x58
    int32_t nSelectionCursorMode;               // +0x5C
    int (__cdecl* pfnCancel)(void);             // +0x60
    int (__cdecl* pfnOutsideClick)(void);       // +0x64
    D2DialogLineStrc aLines[10];                // +0x68..+0xB07
    D2DialogWidgetNodeStrc* pWidgetList;        // +0xB08
    D2DialogStrc* pPrevDialog;                  // +0xB0C
};
static_assert(sizeof(D2DialogStrc) == 0xB10, "1.12a D2DialogStrc must be 0xB10 bytes");

// Esc Options Menu Header (0x18 bytes) & Item (0x550 bytes)
struct D2OptionsMenuHeaderStrc
{
    int32_t nItemCount;                         // +0x00
    int32_t nLineStepY;                         // +0x04
    int32_t nTextOffsetY;                       // +0x08
    int32_t nPentagramOffsetY;                  // +0x0C
    int32_t nSliderOffsetY;                     // +0x10
    int32_t dwReserved14;                       // +0x14
};
static_assert(sizeof(D2OptionsMenuHeaderStrc) == 0x18, "1.12a D2OptionsMenuHeaderStrc must be 0x18 bytes");

struct D2OptionsMenuItemStrc
{
    int32_t nItemType;                          // +0x000 (-1 = title, 0 = button, 1 = choice toggle, 2 = slider)
    int32_t bExpansionOnly;                     // +0x004
    int32_t nRenderedBaseY;                     // +0x008
    char szLabelCelName[0x104];                 // +0x00C..+0x10F
    int32_t (__cdecl* pfnCanUse)(void);         // +0x110
    void (__fastcall* pfnOnChange)(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // +0x114
    void (__fastcall* pfnOnLoad)(D2OptionsMenuItemStrc* pItem);   // +0x118
    void (__fastcall* pfnOnUpdate)(D2OptionsMenuItemStrc* pItem); // +0x11C
    int32_t nChoiceCount;                       // +0x120
    int32_t nCurrentState;                      // +0x124
    int32_t bUseCompactSliderBar;               // +0x128 (0 = OptBar, 1 = OptBarC)
    char aszChoiceCelNames[4][0x104];           // +0x12C..+0x53B
    D2CellFileStrc* pLabelCel;                  // +0x53C
    D2CellFileStrc* apChoiceCels[4];            // +0x540..+0x54F
};
static_assert(sizeof(D2OptionsMenuItemStrc) == 0x550, "1.12a D2OptionsMenuItemStrc must be 0x550 bytes");

struct D2EscPanelSaveEntryStrc
{
    int32_t bCloseOnEsc;                        // +0x00
    int32_t bRestoreOnReturn;                   // +0x04
    int32_t bSavedState;                        // +0x08
};
static_assert(sizeof(D2EscPanelSaveEntryStrc) == 0x0C, "1.12a D2EscPanelSaveEntryStrc must be 0x0C bytes");

enum D2UnitTypeConst : uint32_t
{
    UNIT_PLAYER_TYPE = 0,
};

enum D2PlayerModeConst : uint32_t
{
    PLRMODE_DEAD_CONST = 17,
};

enum D2StatConst : int32_t
{
    STAT_GOLD_CONST = 14,
};

// ============================================================================
// External Imports (Storm, Fog, D2CMP, D2Common, D2Gfx, D2Lang, D2Sound, D2Win)
// ============================================================================
enum DisplayType : int32_t;
enum DrawMode : int32_t;
extern "C" {
    // Storm.dll (C exports)
    BOOL __stdcall SRegLoadValue(const char* szSection, const char* szKey, uint8_t nFlags, int32_t* pValue); // Storm!#423 (0x6FABBE90)
    BOOL __stdcall SRegSaveValue(const char* szSection, const char* szKey, uint8_t nFlags, uint32_t dwValue); // Storm!#426 (0x6FABBE72)
    int32_t __stdcall SMsgBreakHandlerChain(void* pMsg); // Storm!#511 (0x6FABBE78)
}

// Fog.dll, D2CMP.dll, D2Common.dll, D2Gfx.dll, D2Lang.dll, D2Sound.dll, D2Win.dll (C++ exports in D2MOO)
void __cdecl FOG_DisplayHalt(const char* szFmt, const char* szFile, int32_t nLine);
void __fastcall FOG_Free(void* pMem, const char* szFile, int32_t nLine, int32_t nZero);
const char* __cdecl FOG_GetCallerReturnAddress();
int32_t __fastcall FOG_IsExpansion(void);

int32_t __stdcall D2CMP_CelGetWidth(D2CellFileStrc* pCelHandle);
D2CellFileStrc* __stdcall D2CMP_CelGetHandle(D2GfxDataStrc* pGfxData);
int32_t __stdcall D2CMP_CelFileFreeHardware(D2CellFileStrc* pCellFile);
int32_t __stdcall D2CMP_CelFileGetCelsPerDirection(D2CellFileStrc* pCelFile);

int32_t __stdcall DUNGEON_GetLevelIdFromRoom(D2ActiveRoomStrc* pRoom);
BOOL __stdcall DUNGEON_IsTownLevelId(int32_t nLevelId);
D2ActiveRoomStrc* __stdcall UNITS_GetRoom(D2UnitStrc* pUnit);
inline D2ActiveRoomStrc* UNITS_GetRoom(const D2UnitStrc* pUnit) { return UNITS_GetRoom(const_cast<D2UnitStrc*>(pUnit)); }
D2SkillStrc* __stdcall UNITS_GetStartSkill(D2UnitStrc* pUnit);
D2UnitStrc* __stdcall INVENTORY_GetCursorItem(D2InventoryStrc* pInv);
int32_t __stdcall STATLIST_UnitGetStatValue(const D2UnitStrc* pUnit, int32_t nStatId, uint16_t nLayer);
D2SkillStrc* __fastcall SKILLS_GetSkillById(D2UnitStrc* pUnit, int32_t nSkillId, D2UnitGUID nOwnerGUID);
D2UnitGUID __stdcall SKILLS_GetOwnerGUIDFromSkill(D2SkillStrc* pSkill);
D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecordFromSkill(D2SkillStrc* pSkill);
BOOL __stdcall SKILLS_GetSkillInfo(D2SkillStrc* pSkill, D2UnitGUID* pOwnerGUID, int32_t* pSkillId, int32_t* pSkillLevel, int32_t* pCharges);
int32_t __stdcall SKILLS_GetQuantity(D2SkillStrc* pSkill);
D2SkillStrc* __stdcall SKILLS_GetNextSkill(D2SkillStrc* pSkill);

void __stdcall TEXTURE_CelDraw(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, uint32_t dwGamma, DrawMode eDrawMode, uint8_t* pPalette);
inline void TEXTURE_CelDraw(D2GfxDataStrc* pData, int32_t nX, int32_t nY, uint32_t dwGamma, int32_t nDrawMode, uint8_t* pPalette) { TEXTURE_CelDraw(pData, nX, nY, dwGamma, static_cast<DrawMode>(nDrawMode), pPalette); }
void __stdcall D2GFX_DrawBox(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, DrawMode eDrawMode);
inline void D2GFX_DrawBox(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, int32_t nDrawMode) { D2GFX_DrawBox(nXStart, nYStart, nXEnd, nYEnd, dwColor, static_cast<DrawMode>(nDrawMode)); }
HWND __stdcall WINDOW_GetWindow(void);
int32_t __stdcall D2GFX_CheckUnitPerspective(void);
DisplayType __stdcall D2GFX_GetDisplayType(void);
void __fastcall D2GFX_SetViewMatrixAdjust(int32_t nAdjust);
int32_t __stdcall D2GFX_SetGamma(uint32_t dwGamma);
void __stdcall D2GFX_SetBlendShadows(int32_t bBlendedShadows);
void __stdcall D2GFX_SetLowQuality(int32_t bLowQuality);
int32_t __fastcall D2GFX_SetOption(int32_t nOption, int32_t nValue);
void __stdcall D2GFX_SetPerspective(int32_t bPerspective);
int32_t __stdcall D2GFX_GammaCanBeControlled(void);

void __stdcall STRTABLE_GetLanguageInfo(void* pOut, int nMode);
inline Unicode* D2LANG_UnicodeStrCpy(Unicode* pDst, const Unicode* pSrc) { return Unicode::strcpy(pDst, pSrc); }
inline Unicode* D2LANG_UnicodeStrCat(Unicode* pDst, const Unicode* pSrc) { return Unicode::strcat(pDst, pSrc); }
inline Unicode* D2LANG_Win2Unicode(Unicode* pDst, const char* szSrc, int32_t nMaxChars) { return Unicode::win2Unicode(pDst, szSrc, nMaxChars); }
#define D2LANG_UnicodeSPrintf Unicode::sprintf

int32_t __fastcall D2SOUND_GetPositionalBias(void);
BOOL __fastcall D2SOUND_IsEaxEnabled(void);
BOOL __fastcall D2SOUND_Is3DSoundEnabled(void);
int32_t __fastcall D2SOUND_GetMusicVolume(void);
int32_t __fastcall D2SOUND_GetSoundMixer(void);
int32_t __fastcall D2SOUND_GetMasterVolume(void);
int32_t __fastcall D2SOUND_SetPositionalBias(int32_t nBias);
int32_t __fastcall D2SOUND_SetMasterVolume(int32_t nVolume);
int32_t __fastcall D2SOUND_GetNPCSpeech(void);
int32_t __fastcall D2SOUND_SetNPCSpeech(int32_t nNPCSpeech);
int32_t __fastcall D2SOUND_SetMusicVolume(int32_t nVolume);

void __fastcall D2Win_10129_DrawFramedText(const Unicode* wszText, int nX, int nY, int nTextColor, int nAlign);
inline void D2Win_10130_DrawFramedTextEx(const Unicode* wszText, int32_t nX, int32_t nY, int32_t nColor, int32_t nAlign) { D2Win_10129_DrawFramedText(wszText, nX, nY, nColor, nAlign); }
void __fastcall D2Win_10134_DrawCellFile(D2CellFileStrc* pCellFile, int nX, int nY, int nDiv, DrawMode eDrawMode, int nColor);
inline void D2Win_10134_DrawCellFile(D2CellFileStrc* pCellFile, int32_t nX, int32_t nY, int32_t nAlign, int32_t nDrawMode, int32_t nColor) { D2Win_10134_DrawCellFile(pCellFile, nX, nY, nAlign, static_cast<DrawMode>(nDrawMode), nColor); }

static void OPTIONS_HaltAtLine(int32_t nLine)
{
    FOG_DisplayHalt("%s", FOG_GetCallerReturnAddress(), nLine);
    exit(-1);
}

// ============================================================================
// Cross-TU D2Client Function Prototypes (Exact 1.12a Addresses)
// ============================================================================
extern int32_t __fastcall GAME_SetResolutionMode(int32_t nResolutionMode); // 0x6FAC2630
extern void __cdecl NPCMENU_CloseActiveMenu(void); // 0x6FACA1F0
extern int32_t __fastcall CUNIT_HasActiveInteract(D2UnitStrc* pUnit); // 0x6FACEAC0
extern void __fastcall UNIT_FreeUnitGfxData(D2UnitStrc* pUnit); // 0x6FACF900
extern void __stdcall SOUND_PlaySoundEffect(uint32_t nSoundId, D2UnitStrc* pUnit, int32_t nX, int32_t nY, BOOL bGlobal); // 0x6FAE1FA0
extern void __stdcall SOUND_PlayUnitSpeech(D2UnitStrc* pUnit, uint32_t nSpeechId); // 0x6FAE7250
extern int32_t __fastcall AUTOMAP_SetFadeMode(int32_t nFadeMode); // 0x6FAED870
extern void __stdcall AUTOMAP_LoadSprites(void); // 0x6FAEE640
extern void __fastcall AUTOMAP_CenterView(int32_t bForceCenter); // 0x6FAEF1B0

extern void __stdcall DIALOG_AddLine(
    D2DialogStrc* pDialog,
    int32_t nColor,
    const Unicode* wszText,
    int32_t nLineHeight,
    int32_t nUnkFC,
    int32_t nFont,
    int (__cdecl* pfnAction)(void),
    int32_t bSelectable); // 0x6FB12DF0
extern void __fastcall DIALOG_Draw(void* pDialog); // 0x6FB12F40
extern void __cdecl DIALOG_CloseActiveDialog(void); // 0x6FB13290
extern D2DialogStrc* __stdcall DIALOG_CreateDynamic(
    int32_t nAnchorX,
    int32_t nAnchorY,
    int (__cdecl* pfnCancel)(void),
    int (__cdecl* pfnOutsideClick)(void),
    int32_t bBlockUnhandledKeys,
    int32_t bUseNonSelectableHeightAnchor,
    int32_t bAutoCalcBounds,
    int32_t nWidth,
    int32_t nHeight,
    int32_t bCloseOnRightClick,
    int32_t bIgnoreMenuHotkeys); // 0x6FB13600

extern void __fastcall WALL2_UnregisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries); // 0x6FB1D0A0
extern void __fastcall WALL2_RegisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries); // 0x6FB1D100

extern void __stdcall TEXT_ClearChatAndCfgControls(void); // 0x6FB24E20
extern void __stdcall CFGCONTROLS_SetOpenState(BOOL bOpen); // 0x6FB261D0

extern int32_t __fastcall MSG_SendPacket(void* pPacketData, size_t nSize); // 0x6FB291A0
extern int32_t __fastcall MSG_SendLeaveGame_69(void); // 0x6FB29420
extern int32_t __fastcall MSG_SendByte3Words(uint8_t bOpcode, uint16_t w1, uint16_t w2, uint16_t w3); // 0x6FB29870
extern int32_t __fastcall MSG_SendByte(uint8_t bOpcode); // 0x6FB29900

extern int32_t __fastcall GAME_HasMouseWheelB(void); // 0x6FB2A520
extern int32_t __fastcall GAME_HasMouseWheelA(void); // 0x6FB2A570

extern int32_t __cdecl SOUNDCHANNEL_ToggleEAX(void); // 0x6FB2E4C0
extern int32_t __cdecl SOUNDCHANNEL_Toggle3DSound(void); // 0x6FB2E530
extern int32_t __cdecl CTRLPNL_GetEquippedBeltRows(void); // 0x6FB30AD0

extern void __fastcall UI_StartPanelCloseClickCooldown(void); // 0x6FB3AA10
extern void __fastcall UI_RegisterDefaultWindowHandlers(void); // 0x6FB3AF30
extern void __stdcall UI_CloseAllPanels(void); // 0x6FB3B480
extern void __fastcall UI_CloseGoldDialog(void); // 0x6FB3B930
extern D2CellFileStrc* __fastcall UI_LoadGlobalUICellFile(const char* szName); // 0x6FB3C170
extern void __fastcall UI_UpdatePanelHotkeysInMsgTable(void); // 0x6FB3C400
extern int32_t __fastcall UI_PreparePanelOpen(int32_t nAction, int32_t nPanelId); // 0x6FB3E7A0
extern void __fastcall UI_OnPanelClosed(int32_t nPanelId); // 0x6FB3E960
extern int32_t __fastcall UI_SetPanelState(int32_t nPanelId, int32_t nAction, int32_t bMoveMouse); // 0x6FB3EF00

extern uint16_t __fastcall CMDTBL_GetBoundKey(int32_t nCommandId, int32_t bIsPrimary); // 0x6FB4FA40
extern const Unicode* __fastcall CMDTBL_GetKeyNameString(int32_t nCommandId, int32_t bIsPrimary); // 0x6FB50060

extern void __stdcall INV_ClearHoveredItemState(void); // 0x6FB559A0
extern void __stdcall INV_CancelCursorModeOrSend4C(void); // 0x6FB564B0
extern void __stdcall INV_FreeShopTabCells(void); // 0x6FB56E20
extern void __stdcall INV_OpenTradePanel(BOOL bOpen); // 0x6FB5B100
extern void __stdcall INV_ToggleMercenaryInventoryPanel(void); // 0x6FB61ED0
extern void __stdcall INV_CloseMercenaryInventoryPanel(void); // 0x6FB62180
extern void __stdcall INV_OpenInventoryPanel(void); // 0x6FB62300

extern void __fastcall VIEW_SetViewportRect(D2ViewportStrc* pViewport, int32_t nLeft, int32_t nTop, int32_t nRight, int32_t nBottom); // 0x6FB6A990
extern void __stdcall QUESTLOG_TogglePanel(int32_t bCenterCursor); // 0x6FB71FD0

// ============================================================================
// External Cross-TU Globals (Annotated with Exact 1.12a VAs)
// ============================================================================
// 0x6FB813DC
extern const D2SMsgRegEntryStrc g_aCharStatMsgHandlers[2];
// 0x6FB81488
extern const D2SMsgRegEntryStrc g_aCfgControlsMsgHandlers[13];
// 0x6FB81524
extern const D2SMsgRegEntryStrc g_aCfgControlsWheelHandlers[1];
// 0x6FB81644
extern const int32_t g_anTradeAllowedPanelHotkeys[6];
// 0x6FB8D74C
extern const D2SMsgRegEntryStrc g_aCmdTblKeyHandlers[4];
// 0x6FB8D77C
extern const D2SMsgRegEntryStrc g_aCmdTblKeyUpOnlyHandlers[2];
// 0x6FB8DAB0
extern const D2SMsgRegEntryStrc g_aTradePanelMsgHandlers[13];

struct D2PanelDescriptorStrc;
extern BOOL gbExitApplication_6FBA8B70;
#define gbExitAppFlag_6FBA8B70 gbExitApplication_6FBA8B70
extern int32_t g_bWindowClosedExit;
extern D2PanelDescriptorStrc* gpPanelDescriptors_6FB8F2D0[38];
#define g_bExitAppFlag    gbExitAppFlag_6FBA8B70
#define g_nRightMouseHoldY g_bWindowClosedExit
#define g_apUiPanelTable  (*reinterpret_cast<D2PanelRuleRowStrc* (*)[38]>(&gpPanelDescriptors_6FB8F2D0))

// 0x6FB8F2D0
// [aliased] g_apUiPanelTable
// 0x6FB8F36C
extern int32_t g_bAllowPanelToggleSound;
// 0x6FB8FD94
extern int32_t g_nEquippedBeltType;
// 0x6FB90354
extern int32_t g_nMouseRepeatDelayCounterB;
// 0x6FB90358
extern int32_t g_nMouseRepeatDelayCounterA;
// 0x6FB905EC
extern int32_t gbNpcSpeechEnabled;

// 0x6FBA3B98
extern int32_t gnAutomapDivisor;
// 0x6FBA3B9C
extern int32_t gbAutomapCenters;
// 0x6FBA3BA0
extern int32_t gbAutomapShowParty;
// 0x6FBA3BA4
extern int32_t gbAutomapShowPartyNames;
// 0x6FBA6778
extern int32_t g_bForceMediumLightQuality;
// 0x6FBA8B70
// [aliased] g_bExitAppFlag

// 0x6FBB0C38
extern int32_t g_bTradePanelInitPending;
// 0x6FBB1644
extern D2UnitStrc* gpCursorLockedItem;
// 0x6FBB1664
extern int32_t gnCursorAnimId;
// 0x6FBB1668
extern int32_t gnCursorFrameFixed8;
// 0x6FBB2B2C
extern DWORD g_dwPanelCloseCooldownTick;
// 0x6FBB2B3C
extern int32_t g_bPanelTooltipSuppressed;
// 0x6FBB330C
extern int32_t gbIsExpansionClient;
// 0x6FBB5490
extern int32_t gbGoldDialogOpen;
// 0x6FBB54DE
extern int32_t g_bTextDisplayBeta;

// 0x6FBC25A0
extern int32_t g_anSkillHotkeyId[16];
// 0x6FBC2640
extern uint32_t g_adwSkillHotkeyOwner[16];
// 0x6FBC2690
extern int32_t g_abSkillHotkeyLeft[16];

// 0x6FBCB625
extern int32_t g_nInteractedNpcGUID;
// 0x6FBCB629
extern BOOL g_bInteractedWithNpc;
// 0x6FBCB908
struct D2ConfigStrc;
extern D2ConfigStrc* gpD2Config_6FBCB908;
#define g_pGameData reinterpret_cast<D2ClientGameDataStrc*>(gpD2Config_6FBCB908)
// 0x6FBCBC24
extern BOOL g_bTradeGoldBtnPressed;
// 0x6FBCBC28
extern BOOL g_bTradePartnerGoldHighlight;
// 0x6FBCBC3C
extern BOOL g_bTradeGoldTransferDisabled;
// 0x6FBCBCC4
extern int32_t gbCmdTblInputEnabled;
// 0x6FBCBD48
extern D2CellFileStrc* g_pPentSpinCel;
// 0x6FBCBF78
extern BOOL g_bWeaponSwapActive;
// 0x6FBCBFD8
extern int32_t g_bSoundSubsystemInitialized;

// 0x6FBCC000
extern void* g_pClientPoolSystem;
// 0x6FBCC004
extern int32_t gbActLoadedFlag;
// 0x6FBCC008
extern int32_t g_bExitGameFlag;
// 0x6FBCC040
extern int32_t g_bLeftMouseHeld;
// 0x6FBCC044
extern int32_t g_bLeftMouseRepeating;
// 0x6FBCC050
extern int32_t gbAlwaysRun;
// 0x6FBCC05C
extern int32_t g_bRightMouseHeld;
// 0x6FBCC060
extern int32_t g_bRightMouseRepeating;
// 0x6FBCC068
// [aliased] g_nRightMouseHoldY
// 0x6FBCC118
extern int32_t gnTooltipUnitType;
// 0x6FBCC11C
extern int32_t gnTooltipUnitGUID;
// 0x6FBCC194
extern int32_t g_bStashOrCubeBusy;
// 0x6FBCC2A0
extern int32_t gnAutomapMode;
// 0x6FBCC2F8
extern int32_t gnAutomapFadeMode;
// 0x6FBCC2FC
extern int32_t gnAutomapSavedFadeMode;

// 0x6FBD33F8
extern int32_t gbCmdTblAltKeyHandlersRegistered;
// 0x6FBD33FC
extern int32_t gbCmdTblKeyHandlersRegistered;

// ============================================================================
// Forward Declarations of Local Callbacks & Handlers (for Menu Tables)
// ============================================================================
void __fastcall OPTIONS_SyncAutomapPartyNames(D2OptionsMenuItemStrc* pItem); // 0x6FB03060
void __fastcall OPTIONS_OnChangeAutomapPartyNames(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB03090
int32_t __cdecl OPTIONS_CanUseAutomapPartyNames(void); // 0x6FB030C0
void __fastcall OPTIONS_SyncAutomapParty(D2OptionsMenuItemStrc* pItem); // 0x6FB030D0
void __fastcall OPTIONS_OnChangeAutomapParty(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB030E0
void __fastcall OPTIONS_SyncAutomapCenters(D2OptionsMenuItemStrc* pItem); // 0x6FB03110
void __fastcall OPTIONS_OnChangeAutomapCenters(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB03120
void __fastcall OPTIONS_SyncAutomapFade(D2OptionsMenuItemStrc* pItem); // 0x6FB03150
void __fastcall OPTIONS_OnChangeAutomapFade(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB031D0
int32_t __cdecl OPTIONS_CanUseGamma(void); // 0x6FB03210
int32_t __cdecl OPTIONS_CanUsePerspective(void); // 0x6FB03220
void __fastcall OPTIONS_InitBlendShadows(D2OptionsMenuItemStrc* pItem); // 0x6FB03230
void __fastcall OPTIONS_OnChangeBlendShadows(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB03260
void __fastcall OPTIONS_InitLightQuality(D2OptionsMenuItemStrc* pItem); // 0x6FB03290
void __fastcall OPTIONS_OnChangeLightQuality(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB032F0
void __fastcall OPTIONS_OnChangeContrast(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB03B80
void __fastcall OPTIONS_OnChangeGamma(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB03BC0
void __fastcall OPTIONS_OnChangeNPCSpeech(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB03BF0
int32_t __cdecl OPTIONS_CanUseNPCSpeech(void); // 0x6FB03C80
void __fastcall OPTIONS_InitPositionalBias(D2OptionsMenuItemStrc* pItem); // 0x6FB03C90
void __fastcall OPTIONS_OnChangePositionalBias(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB03CB0
void __fastcall OPTIONS_InitMusicVolume(D2OptionsMenuItemStrc* pItem); // 0x6FB03CD0
void __fastcall OPTIONS_OnChangeMusicVolume(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB03CF0
int32_t __cdecl OPTIONS_CanUseMusicVolume(void); // 0x6FB03D10
void __fastcall OPTIONS_InitMasterVolume(D2OptionsMenuItemStrc* pItem); // 0x6FB03D20
void __fastcall OPTIONS_OnChangeMasterVolume(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB03D40
int32_t __cdecl OPTIONS_CanUseMasterVolume(void); // 0x6FB03D60
int32_t __cdecl OPTIONS_CanUseSoundOptionsMenu(void); // 0x6FB03D70
void __fastcall OPTIONS_InitContrastSlider(D2OptionsMenuItemStrc* pItem); // 0x6FB04260
void __fastcall OPTIONS_InitGammaSlider(D2OptionsMenuItemStrc* pItem); // 0x6FB042E0
void __fastcall OPTIONS_OnChangePerspective(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB04360
void __fastcall OPTIONS_InitNPCSpeech(D2OptionsMenuItemStrc* pItem); // 0x6FB043B0
void __fastcall OPTIONS_SyncEAXState(D2OptionsMenuItemStrc* pItem); // 0x6FB043E0
int32_t __cdecl OPTIONS_CanUseEAX(void); // 0x6FB04410
int32_t __cdecl OPTIONS_CanUsePositionalBias(void); // 0x6FB04430
void __fastcall OPTIONS_Sync3DSoundState(D2OptionsMenuItemStrc* pItem); // 0x6FB04460
int32_t __cdecl OPTIONS_CanUse3DSound(void); // 0x6FB044A0

void __stdcall OPTIONS_OnRightArrowKey(D2WinMsgStrc* pMsg); // 0x6FB048E0
void __stdcall OPTIONS_OnLeftArrowKey(D2WinMsgStrc* pMsg); // 0x6FB049C0
void __stdcall OPTIONS_OnUpArrowKey(D2WinMsgStrc* pMsg); // 0x6FB04AA0
void __stdcall OPTIONS_OnDownArrowKey(D2WinMsgStrc* pMsg); // 0x6FB04B10

void __fastcall OPTIONS_InitAutomapMode(D2OptionsMenuItemStrc* pItem); // 0x6FB04DF0
void __fastcall OPTIONS_OnChangeAutomapMode(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB04E60
void __fastcall OPTIONS_InitPerspective(D2OptionsMenuItemStrc* pItem); // 0x6FB04ED0

void __stdcall OPTIONS_OnReturnKey(D2WinMsgStrc* pMsg); // 0x6FB05440
void __stdcall OPTIONS_OnLButtonUp(D2WinMsgStrc* pMsg); // 0x6FB05470
void __stdcall OPTIONS_OnLButtonDown(D2WinMsgStrc* pMsg); // 0x6FB054F0
void __fastcall OPTIONS_OnChangeEAX(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB05540
void __fastcall OPTIONS_OnChange3DSound(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB05550
void __stdcall UI_CloseEscOptionsMenu(int32_t bRestorePanels); // 0x6FB05560
void __fastcall UI_OpenEscOptionsMenu(int32_t bSaveAndClosePanels, int32_t nMenuPage); // 0x6FB05610
void __fastcall OPTIONS_OnSelectSubMenuPrevious(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB05B00
void __fastcall OPTIONS_OnSelectOptionsPrevious(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB05B10
void __fastcall OPTIONS_OnSelectConfigureControls(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB05B20
void __fastcall OPTIONS_OnSelectAutomapOptions(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB05C70
void __fastcall OPTIONS_OnSelectVideoOptions(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB05C80
void __fastcall OPTIONS_OnSelectSoundOptions(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB05C90
void __fastcall OPTIONS_OnSelectReturnToGame(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB05CA0
void __fastcall OPTIONS_OnSelectSaveAndExitGame(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB05CB0
void __fastcall OPTIONS_OnSelectOptionsSubMenu(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB05CE0
void __fastcall OPTIONS_InitResolution(D2OptionsMenuItemStrc* pItem); // 0x6FB06120
void __fastcall OPTIONS_OnChangeResolution(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg); // 0x6FB06150

// ============================================================================
// Module-Owned Tables & Globals (Exact 1.12a Addresses)
// ============================================================================

// 0x6FB81430
static const D2SMsgRegEntryStrc s_aEscOptionsMsgHandlers[7] = {
    { 0, WM_LBUTTONDOWN, OPTIONS_OnLButtonDown   },
    { 0, WM_LBUTTONUP,   OPTIONS_OnLButtonUp     },
    { 3, VK_DOWN,        OPTIONS_OnDownArrowKey  },
    { 3, VK_UP,          OPTIONS_OnUpArrowKey    },
    { 3, VK_LEFT,        OPTIONS_OnLeftArrowKey  },
    { 3, VK_RIGHT,       OPTIONS_OnRightArrowKey },
    { 3, VK_RETURN,      OPTIONS_OnReturnKey     },
};

// 0x6FB948E0 (38 entries of 12 bytes; index 21 bSavedState is at 0x6FB949E4)
static D2EscPanelSaveEntryStrc g_aEscPanelSaveTable[D2PANEL_COUNT] = {
    /* [ 0] */ { 0, 0, 0 },
    /* [ 1] */ { 1, 0, 0 },
    /* [ 2] */ { 1, 0, 0 },
    /* [ 3] */ { 1, 0, 0 },
    /* [ 4] */ { 1, 0, 0 },
    /* [ 5] */ { 1, 0, 0 },
    /* [ 6] */ { 1, 1, 0 },
    /* [ 7] */ { 1, 1, 0 },
    /* [ 8] */ { 1, 0, 0 },
    /* [ 9] */ { 0, 0, 0 },
    /* [10] */ { 1, 1, 0 },
    /* [11] */ { 1, 0, 0 },
    /* [12] */ { 1, 0, 0 },
    /* [13] */ { 1, 0, 0 },
    /* [14] */ { 1, 0, 0 },
    /* [15] */ { 1, 0, 0 },
    /* [16] */ { 1, 0, 0 },
    /* [17] */ { 1, 1, 0 },
    /* [18] */ { 1, 0, 0 },
    /* [19] */ { 1, 0, 0 },
    /* [20] */ { 1, 0, 0 },
    /* [21] */ { 1, 1, 0 }, // D2PANEL_MINIPANEL (bSavedState @ 0x6FB949E4)
    /* [22] */ { 1, 0, 0 },
    /* [23] */ { 1, 0, 0 },
    /* [24] */ { 1, 0, 0 },
    /* [25] */ { 1, 0, 0 },
    /* [26] */ { 1, 0, 0 },
    /* [27] */ { 1, 0, 0 },
    /* [28] */ { 1, 0, 0 },
    /* [29] */ { 1, 0, 0 },
    /* [30] */ { 1, 0, 0 },
    /* [31] */ { 1, 0, 0 },
    /* [32] */ { 1, 0, 0 },
    /* [33] */ { 1, 0, 0 },
    /* [34] */ { 1, 0, 0 },
    /* [35] */ { 1, 1, 0 },
    /* [36] */ { 1, 0, 0 },
    /* [37] */ { 1, 0, 0 },
};

// 0x6FB94AA8
static D2OptionsMenuHeaderStrc g_tMainEscMenuHeader = { 3, 50, 39, 51, 0, 0 };
// 0x6FB94AC0
static D2OptionsMenuItemStrc g_aMainEscMenuItems[3] = {
    { 0, 0, 0, "Options",      nullptr, OPTIONS_OnSelectOptionsSubMenu,  nullptr, nullptr, 0, 0, 0, { "", "", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
    { 0, 0, 0, "Exit",         nullptr, OPTIONS_OnSelectSaveAndExitGame, nullptr, nullptr, 0, 0, 0, { "", "", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
    { 0, 0, 0, "ReturnToGame", nullptr, OPTIONS_OnSelectReturnToGame,    nullptr, nullptr, 0, 0, 0, { "", "", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
};

// 0x6FB95AB0
static D2OptionsMenuHeaderStrc g_tOptionsSubMenuHeader = { 5, 50, 39, 51, 0, 0 };
// 0x6FB95AC8
static D2OptionsMenuItemStrc g_aOptionsSubMenuItems[5] = {
    { 0, 0, 0, "SoundOptions",   OPTIONS_CanUseSoundOptionsMenu, OPTIONS_OnSelectSoundOptions,      nullptr, nullptr, 0, 0, 0, { "", "", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
    { 0, 0, 0, "VideoOptions",   nullptr,                        OPTIONS_OnSelectVideoOptions,      nullptr, nullptr, 0, 0, 0, { "", "", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
    { 0, 0, 0, "AutoMapOptions", nullptr,                        OPTIONS_OnSelectAutomapOptions,    nullptr, nullptr, 0, 0, 0, { "", "", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
    { 0, 0, 0, "CfgOptions",     nullptr,                        OPTIONS_OnSelectConfigureControls, nullptr, nullptr, 0, 0, 0, { "", "", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
    { 0, 0, 0, "Previous",       nullptr,                        OPTIONS_OnSelectOptionsPrevious,   nullptr, nullptr, 0, 0, 0, { "", "", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
};

// 0x6FB97558
static D2OptionsMenuHeaderStrc g_tSoundMenuHeader = { 8, 45, 34, 49, 36, 0 };
// 0x6FB97570
static D2OptionsMenuItemStrc g_aSoundMenuItems[8] = {
    { -1, 0, 0, "SoundOptions", nullptr,                        nullptr,                        nullptr,                    nullptr,                  0, 0, 0, { "", "", "", "" },                          nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  2, 0, 0, "Sound",        OPTIONS_CanUseMasterVolume,     OPTIONS_OnChangeMasterVolume,   OPTIONS_InitMasterVolume,   nullptr,                 21, 0, 0, { "", "", "", "" },                          nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  2, 0, 0, "Music",        OPTIONS_CanUseMusicVolume,      OPTIONS_OnChangeMusicVolume,    OPTIONS_InitMusicVolume,    nullptr,                 21, 0, 0, { "", "", "", "" },                          nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "3DSound",      OPTIONS_CanUse3DSound,          OPTIONS_OnChange3DSound,        OPTIONS_Sync3DSoundState,   OPTIONS_Sync3DSoundState, 2, 1, 0, { "SmallOff", "SmallOn", "", "" },           nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "EAX",          OPTIONS_CanUseEAX,              OPTIONS_OnChangeEAX,            OPTIONS_SyncEAXState,       OPTIONS_SyncEAXState,     2, 1, 0, { "SmallOff", "SmallOn", "", "" },           nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  2, 0, 0, "3DBias",       OPTIONS_CanUsePositionalBias,   OPTIONS_OnChangePositionalBias, OPTIONS_InitPositionalBias, nullptr,                 21, 0, 1, { "", "", "", "" },                          nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "NPCSpeech",    OPTIONS_CanUseNPCSpeech,        OPTIONS_OnChangeNPCSpeech,      OPTIONS_InitNPCSpeech,      nullptr,                  3, 2, 0, { "AudioOnly", "TextOnly", "AudioText", "" },nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  0, 0, 0, "SPrevious",    nullptr,                        OPTIONS_OnSelectSubMenuPrevious,nullptr,                    nullptr,                  0, 0, 0, { "", "", "", "" },                          nullptr, { nullptr, nullptr, nullptr, nullptr } },
};

// 0x6FB99FF0
static D2OptionsMenuHeaderStrc g_tVideoMenuClassicHeader = { 7, 45, 34, 49, 36, 0 };
// 0x6FB9A008
static D2OptionsMenuItemStrc g_aVideoMenuClassicItems[7] = {
    { -1, 0, 0, "VideoOptions", nullptr,                    nullptr,                        nullptr,                    nullptr,   0,  0, 0, { "", "", "", "" },                nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "LightQuality", nullptr,                    OPTIONS_OnChangeLightQuality,   OPTIONS_InitLightQuality,   nullptr,   3,  2, 0, { "Low", "Medium", "High", "" },   nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "BlendShadow",  nullptr,                    OPTIONS_OnChangeBlendShadows,   OPTIONS_InitBlendShadows,   nullptr,   2,  1, 0, { "SmallOff", "SmallOn", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "Perspective",  OPTIONS_CanUsePerspective,  OPTIONS_OnChangePerspective,    OPTIONS_InitPerspective,    nullptr,   2,  1, 0, { "SmallOff", "SmallOn", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  2, 0, 0, "Gamma",        OPTIONS_CanUseGamma,        OPTIONS_OnChangeGamma,          OPTIONS_InitGammaSlider,    nullptr,  21,  0, 1, { "", "", "", "" },                nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  2, 0, 0, "Contrast",     nullptr,                    OPTIONS_OnChangeContrast,       OPTIONS_InitContrastSlider, nullptr, 100, 50, 1, { "", "", "", "" },                nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  0, 0, 0, "SPrevious",    nullptr,                    OPTIONS_OnSelectSubMenuPrevious,nullptr,                    nullptr,   0,  0, 0, { "", "", "", "" },                nullptr, { nullptr, nullptr, nullptr, nullptr } },
};

// 0x6FB9C538
static D2OptionsMenuHeaderStrc g_tVideoMenuExpHeader = { 8, 45, 34, 49, 36, 0 };
// 0x6FB9C550
static D2OptionsMenuItemStrc g_aVideoMenuExpItems[8] = {
    { -1, 0, 0, "VideoOptions", nullptr,                    nullptr,                        nullptr,                    nullptr,   0,  0, 0, { "", "", "", "" },                nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 1, 0, "Resolution",   nullptr,                    OPTIONS_OnChangeResolution,     OPTIONS_InitResolution,     nullptr,   2,  0, 0, { "640x480", "800x600", "", "" },  nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "LightQuality", nullptr,                    OPTIONS_OnChangeLightQuality,   OPTIONS_InitLightQuality,   nullptr,   3,  2, 0, { "Low", "Medium", "High", "" },   nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "BlendShadow",  nullptr,                    OPTIONS_OnChangeBlendShadows,   OPTIONS_InitBlendShadows,   nullptr,   2,  1, 0, { "SmallOff", "SmallOn", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "Perspective",  OPTIONS_CanUsePerspective,  OPTIONS_OnChangePerspective,    OPTIONS_InitPerspective,    nullptr,   2,  1, 0, { "SmallOff", "SmallOn", "", "" }, nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  2, 0, 0, "Gamma",        OPTIONS_CanUseGamma,        OPTIONS_OnChangeGamma,          OPTIONS_InitGammaSlider,    nullptr,  21,  0, 1, { "", "", "", "" },                nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  2, 0, 0, "Contrast",     nullptr,                    OPTIONS_OnChangeContrast,       OPTIONS_InitContrastSlider, nullptr, 100, 50, 1, { "", "", "", "" },                nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  0, 0, 0, "SPrevious",    nullptr,                    OPTIONS_OnSelectSubMenuPrevious,nullptr,                    nullptr,   0,  0, 0, { "", "", "", "" },                nullptr, { nullptr, nullptr, nullptr, nullptr } },
};

// 0x6FB9EFD0
static D2OptionsMenuHeaderStrc g_tAutomapMenuClassicHeader = { 6, 45, 34, 49, 36, 0 };
// 0x6FB9EFE8
static D2OptionsMenuItemStrc g_aAutomapMenuClassicItems[6] = {
    { -1, 0, 0, "AutoMapOptions",    nullptr,                           nullptr,                           nullptr,                        nullptr,                        0, 0, 0, { "", "", "", "" },                                 nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "AutoMapFade",       nullptr,                           OPTIONS_OnChangeAutomapFade,       OPTIONS_SyncAutomapFade,        OPTIONS_SyncAutomapFade,        4, 0, 0, { "SmallNo", "Center", "Everything", "Auto" },      nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "AutoMapCenter",     nullptr,                           OPTIONS_OnChangeAutomapCenters,    OPTIONS_SyncAutomapCenters,     OPTIONS_SyncAutomapCenters,     2, 1, 0, { "SmallNo", "SmallYes", "", "" },                  nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "AutoMapParty",      nullptr,                           OPTIONS_OnChangeAutomapParty,      OPTIONS_SyncAutomapParty,       OPTIONS_SyncAutomapParty,       2, 1, 0, { "SmallNo", "SmallYes", "", "" },                  nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "AutoMapPartyNames", OPTIONS_CanUseAutomapPartyNames,   OPTIONS_OnChangeAutomapPartyNames, OPTIONS_SyncAutomapPartyNames,  OPTIONS_SyncAutomapPartyNames,  2, 1, 0, { "SmallNo", "SmallYes", "", "" },                  nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  0, 0, 0, "SPrevious",         nullptr,                           OPTIONS_OnSelectSubMenuPrevious,   nullptr,                        nullptr,                        0, 0, 0, { "", "", "", "" },                                 nullptr, { nullptr, nullptr, nullptr, nullptr } },
};

// 0x6FBA0FC8
static D2OptionsMenuHeaderStrc g_tAutomapMenuExpHeader = { 7, 45, 34, 49, 36, 0 };
// 0x6FBA0FE0
static D2OptionsMenuItemStrc g_aAutomapMenuExpItems[7] = {
    { -1, 0, 0, "AutoMapOptions",    nullptr,                           nullptr,                           nullptr,                        nullptr,                        0, 0, 0, { "", "", "", "" },                                 nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 1, 0, "AutoMapMode",       nullptr,                           OPTIONS_OnChangeAutomapMode,       OPTIONS_InitAutomapMode,        nullptr,                        2, 0, 0, { "Full", "Mini", "", "" },                         nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "AutoMapFade",       nullptr,                           OPTIONS_OnChangeAutomapFade,       OPTIONS_SyncAutomapFade,        OPTIONS_SyncAutomapFade,        4, 0, 0, { "SmallNo", "Center", "Everything", "Auto" },      nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "AutoMapCenter",     nullptr,                           OPTIONS_OnChangeAutomapCenters,    OPTIONS_SyncAutomapCenters,     OPTIONS_SyncAutomapCenters,     2, 1, 0, { "SmallNo", "SmallYes", "", "" },                  nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "AutoMapParty",      nullptr,                           OPTIONS_OnChangeAutomapParty,      OPTIONS_SyncAutomapParty,       OPTIONS_SyncAutomapParty,       2, 1, 0, { "SmallNo", "SmallYes", "", "" },                  nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  1, 0, 0, "AutoMapPartyNames", OPTIONS_CanUseAutomapPartyNames,   OPTIONS_OnChangeAutomapPartyNames, OPTIONS_SyncAutomapPartyNames,  OPTIONS_SyncAutomapPartyNames,  2, 1, 0, { "SmallNo", "SmallYes", "", "" },                  nullptr, { nullptr, nullptr, nullptr, nullptr } },
    {  0, 0, 0, "SPrevious",         nullptr,                           OPTIONS_OnSelectSubMenuPrevious,   nullptr,                        nullptr,                        0, 0, 0, { "", "", "", "" },                                 nullptr, { nullptr, nullptr, nullptr, nullptr } },
};

// Player Trade & Prompt Dialog State Globals (0x6FBC26D0..0x6FBC26EC, 0x6FBCC1F4..0x6FBCC1FB, 0x6FBD334C)
// 0x6FBC26D0
static uint32_t g_dwTradePartnerGUID = 0;
// 0x6FBC26D4
static D2DialogStrc* g_pIncomingTradePromptDialog = nullptr;
// 0x6FBC26D8
int32_t g_nTradePartnerGold = 0; // Local player's offered gold in trade (referenced as g_nTradePartnerGold by UI_inv.cpp)
// 0x6FBC26DC
static D2DialogStrc* g_pOutgoingTradePromptDialog = nullptr;
// 0x6FBC26E0
static uint32_t g_dwTradeCancelTimeoutStartTick = 0xFFFFFFFFu;
// 0x6FBC26E4
static uint8_t g_bTradeCancelPacketSent = 0;
// 0x6FBC26E8
int32_t g_nTradePlayerGold = 0; // Partner player's offered gold in trade (referenced as g_nTradePlayerGold by UI_inv.cpp)
// 0x6FBCC1F4
static int32_t g_nPlayerTradeState = 0;
// 0x6FBCC1F8
static uint8_t g_bTradeAcceptBlocked = 0;
// 0x6FBD334C
Unicode g_wszTradePartnerName[24] = {};

// Mini-Panel Button Arrays & State Globals (0x6FBC26F0..0x6FBC278F, 0x6FBCC224..0x6FBCC243)
// 0x6FBC26F0
static int32_t g_aMiniPanelBtnX[8] = {};
// 0x6FBC2710
static int32_t g_aMiniPanelBtnY[8] = {};
// 0x6FBC2730
static int32_t g_aMiniPanelBtnAction[8] = {};
// 0x6FBC2750
static int32_t g_aMiniPanelBtnPressed[8] = {};
// 0x6FBC2770
static int32_t g_aMiniPanelBtnFrame[8] = {};

// Esc Options Menu & Mini-Panel Runtime Globals (0x6FBCC1FC..0x6FBCC240, 0x6FBD3400..0x6FBD3668)
// 0x6FBCC1FC
static int32_t g_nSelectedOptionsItemIdx = 0;
// 0x6FBCC200
static D2OptionsMenuHeaderStrc* g_pActiveOptionsHeader = nullptr;
// 0x6FBCC204
static D2OptionsMenuItemStrc* g_pActiveOptionsItems = nullptr;
// 0x6FBCC208
static int32_t g_nOptionsPentagramFrame = 0;
// 0x6FBCC20C
static int32_t g_bOptionsMouseButtonDown = 0;
// 0x6FBCC210
static int32_t g_bOptionsSliderDragging = 0;
// 0x6FBCC214
static D2CellFileStrc* g_pOptBarCel = nullptr;
// 0x6FBCC218
static D2CellFileStrc* g_pOptBarCompactCel = nullptr;
// 0x6FBCC21C
static D2CellFileStrc* g_pOptSkullCel = nullptr;
// 0x6FBCC220
static int32_t g_nMaxPentagramWidth = 0;
// 0x6FBCC224
static D2CellFileStrc* g_pMiniPanelBgCel = nullptr;
// 0x6FBCC228
static D2CellFileStrc* g_pMiniPanelBtnCel = nullptr;
// 0x6FBCC22C
static int32_t g_bMiniPanelShiftLeftForRightPanel = 0;
// 0x6FBCC230
static int32_t g_bMiniPanelShiftRightForLeftPanel = 0;
// 0x6FBCC234
static int32_t g_bMiniPanelHiddenByPanels = 0;
// 0x6FBCC238
static int32_t g_bMiniPanelTogglePressed = 0;
// 0x6FBCC23C
static int32_t g_bMiniPanelSinglePlayer = 0;
// 0x6FBCC240
static int32_t g_bMiniPanelButtonDown = 0;

// 0x6FBD3400
static DWORD g_dwLastOptionsPentagramTick = 0;
// 0x6FBD3664
static int32_t g_nLastOptionsMouseY = 0;
// 0x6FBD3668
static int32_t g_nLastOptionsMouseX = 0;

// ============================================================================
// PART 1: UI\minipanel.cpp & UI\options.cpp (0x6FB02DD0..0x6FB0634F)
// ============================================================================

// 1.12a: 0x6FB02DD0
int32_t __fastcall MINIPANEL_HitTestPoint(int32_t nMouseX, int32_t nMouseY)
{
    int32_t nHalfW = gnScreenWidth / 2;
    if (nMouseX <= nHalfW - 0x4A || nMouseX >= nHalfW + 0x47)
    {
        return 0;
    }
    if (nMouseY >= gnScreenHeight - 0x32 || nMouseY <= gnScreenHeight - 0x45)
    {
        return 0;
    }
    return 1;
}

// 1.12a: 0x6FB02DD0 (Cross-TU alias referenced by UI_npcmenu.cpp & D2ClientLinkBridge.cpp)
int32_t __fastcall D2Client_HitTestMiniPanel(int32_t nMouseX)
{
    return MINIPANEL_HitTestPoint(nMouseX, gnMouseY);
}

// 1.12a: 0x6FB02E10
int32_t __cdecl MINIPANEL_IsToggleButtonHeld(void)
{
    return g_bMiniPanelTogglePressed != 0 ? 1 : 0;
}

// 1.12a: 0x6FB02E20 (epilogue 0x6FB02F00..0x6FB02F04)
void __fastcall MINIPANEL_UpdateButtonLayout(int32_t nDockMode)
{
    const int32_t bSinglePlayer = g_bMiniPanelSinglePlayer;
    const int32_t nBtnCount = (bSinglePlayer == 0) ? 8 : 7;
    if (nDockMode == 0)
    {
        return;
    }

    const int32_t nHalfW = gnScreenWidth / 2;
    int32_t nCurX = (bSinglePlayer == 0) ? (nHalfW - 0x54) : (nHalfW - 0x4A);
    const int32_t nBtnY = gnScreenHeight - 0x32;

    if (nDockMode == 1)
    {
        nCurX = nHalfW - 0xCA;
    }
    else if (nDockMode == 3)
    {
        nCurX = (bSinglePlayer != 0) ? (nHalfW + 0x39) : (nHalfW + 0x23);
    }

    g_aMiniPanelBtnAction[0] = 0;
    g_aMiniPanelBtnFrame[0]  = 0;
    g_aMiniPanelBtnX[0]      = nCurX;
    g_aMiniPanelBtnY[0]      = nBtnY;
    nCurX += 21;

    int32_t nActionSkip = 0;
    int32_t nFrameSkip  = 0;
    for (int32_t i = 1; i < nBtnCount; ++i)
    {
        g_aMiniPanelBtnX[i] = nCurX;
        g_aMiniPanelBtnY[i] = nBtnY;
        if (bSinglePlayer != 0)
        {
            if (i == 3)
            {
                nFrameSkip += 2;
                nActionSkip += 1;
            }
            g_aMiniPanelBtnAction[i] = i + nActionSkip;
            g_aMiniPanelBtnFrame[i]  = (i * 2) + nFrameSkip;
        }
        else
        {
            g_aMiniPanelBtnAction[i] = i;
            g_aMiniPanelBtnFrame[i]  = i * 2;
        }
        nCurX += 21;
    }
}

// 1.12a: 0x6FB02F10
void __fastcall MINIPANEL_DrawButtonCell(int32_t nUnusedEcx, int32_t nY, int32_t nFrame, int32_t nX)
{
    (void)nUnusedEcx;
    D2GfxDataStrc tGfx = {};
    tGfx.pCellFile = g_pMiniPanelBtnCel;
    tGfx.nFrame    = (uint32_t)nFrame;
    TEXTURE_CelDraw(&tGfx, nX, nY, 0xFFFFFFFFu, 5, nullptr);
}

// 1.12a: 0x6FB02F50
void __fastcall MINIPANEL_DrawBackgroundByDockMode(int32_t nUnusedEcx, int32_t nDockMode)
{
    (void)nUnusedEcx;
    if (nDockMode == 0)
    {
        return;
    }

    D2GfxDataStrc tGfx = {};
    tGfx.pCellFile = g_pMiniPanelBgCel;

    const int32_t nHalfW = gnScreenWidth / 2;
    const int32_t nY = gnScreenHeight - 0x2F;
    if (nDockMode == 1)
    {
        TEXTURE_CelDraw(&tGfx, nHalfW - 0xCD, nY, 0xFFFFFFFFu, 5, nullptr);
    }
    else if (nDockMode == 2)
    {
        const int32_t nBaseX = (g_bMiniPanelSinglePlayer != 0) ? (nHalfW - 0x4A) : (nHalfW - 0x54);
        TEXTURE_CelDraw(&tGfx, nBaseX - 3, nY, 0xFFFFFFFFu, 5, nullptr);
    }
    else if (nDockMode == 3)
    {
        const int32_t nBaseX = (g_bMiniPanelSinglePlayer != 0) ? (nHalfW + 0x38) : (nHalfW + 0x23);
        TEXTURE_CelDraw(&tGfx, nBaseX - 2, nY, 0xFFFFFFFFu, 5, nullptr);
    }
}

// 1.12a: 0x6FB03040
int32_t __fastcall OPTIONS_IsMenuItemEnabled(const D2OptionsMenuItemStrc* pItem)
{
    if (pItem->pfnCanUse != nullptr && pItem->pfnCanUse() == 0)
    {
        return 0;
    }
    return 1;
}

// 1.12a: 0x6FB03060
void __fastcall OPTIONS_SyncAutomapPartyNames(D2OptionsMenuItemStrc* pItem)
{
    if (gbAutomapShowParty != 0 && gbAutomapShowPartyNames != 0)
    {
        pItem->nCurrentState = 1;
    }
    else
    {
        pItem->nCurrentState = 0;
    }
}

// 1.12a: 0x6FB03090
void __fastcall OPTIONS_OnChangeAutomapPartyNames(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    const int32_t nState = pItem->nCurrentState;
    SRegSaveValue("Diablo II", "AutoMap Party Names", 0, (uint32_t)nState);
    gbAutomapShowPartyNames = nState;
}

// 1.12a: 0x6FB030C0
int32_t __cdecl OPTIONS_CanUseAutomapPartyNames(void)
{
    return gbAutomapShowParty;
}

// 1.12a: 0x6FB030D0
void __fastcall OPTIONS_SyncAutomapParty(D2OptionsMenuItemStrc* pItem)
{
    pItem->nCurrentState = gbAutomapShowParty;
}

// 1.12a: 0x6FB030E0
void __fastcall OPTIONS_OnChangeAutomapParty(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    const int32_t nState = pItem->nCurrentState;
    SRegSaveValue("Diablo II", "AutoMap Party", 0, (uint32_t)nState);
    gbAutomapShowParty = nState;
}

// 1.12a: 0x6FB03110
void __fastcall OPTIONS_SyncAutomapCenters(D2OptionsMenuItemStrc* pItem)
{
    pItem->nCurrentState = gbAutomapCenters;
}

// 1.12a: 0x6FB03120
void __fastcall OPTIONS_OnChangeAutomapCenters(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    const int32_t nState = pItem->nCurrentState;
    SRegSaveValue("Diablo II", "AutoMap Centers", 0, (uint32_t)nState);
    gbAutomapCenters = nState;
}

// 1.12a: 0x6FB03150
void __fastcall OPTIONS_SyncAutomapFade(D2OptionsMenuItemStrc* pItem)
{
    int32_t nRegFade = 0;
    SRegLoadValue("Diablo II", "AutoMapFade", 0, &nRegFade);
    if ((uint32_t)nRegFade > 4u)
    {
        nRegFade = 0;
        AUTOMAP_SetFadeMode(0);
    }

    if (gnAutomapMode == 1)
    {
        int32_t nFadeMode = gnAutomapFadeMode;
        if (nFadeMode == 1)
        {
            nFadeMode = (gnAutomapSavedFadeMode == 0) ? 2 : 0;
            gnAutomapFadeMode = nFadeMode;
        }
        pItem->nCurrentState = nFadeMode;
        return;
    }

    pItem->nCurrentState = gnAutomapFadeMode;
}

// 1.12a: 0x6FB031D0
void __fastcall OPTIONS_OnChangeAutomapFade(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    if ((uint32_t)pItem->nCurrentState > 4u)
    {
        pItem->nCurrentState = 0;
    }
    AUTOMAP_SetFadeMode(pItem->nCurrentState);
    SRegSaveValue("Diablo II", "AutoMapFade", 0, (uint32_t)pItem->nCurrentState);
}

// 1.12a: 0x6FB03210
int32_t __cdecl OPTIONS_CanUseGamma(void)
{
    return D2GFX_GammaCanBeControlled();
}

// 1.12a: 0x6FB03220
int32_t __cdecl OPTIONS_CanUsePerspective(void)
{
    return D2GFX_CheckUnitPerspective();
}

// 1.12a: 0x6FB03230
void __fastcall OPTIONS_InitBlendShadows(D2OptionsMenuItemStrc* pItem)
{
    SRegLoadValue("Diablo II", "Blended Shadows", 0, &pItem->nCurrentState);
    D2GFX_SetBlendShadows(pItem->nCurrentState);
}

// 1.12a: 0x6FB03260
void __fastcall OPTIONS_OnChangeBlendShadows(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    SRegSaveValue("Diablo II", "Blended Shadows", 0, (uint32_t)pItem->nCurrentState);
    D2GFX_SetBlendShadows(pItem->nCurrentState);
}

// 1.12a: 0x6FB03290
void __fastcall OPTIONS_InitLightQuality(D2OptionsMenuItemStrc* pItem)
{
    SRegLoadValue("Diablo II", "Light Quality", 0, &pItem->nCurrentState);
    switch (pItem->nCurrentState)
    {
    case 0:
        D2GFX_SetLowQuality(1);
        g_bForceMediumLightQuality = 0;
        break;
    case 1:
        D2GFX_SetLowQuality(0);
        g_bForceMediumLightQuality = 0;
        break;
    case 2:
        D2GFX_SetLowQuality(0);
        g_bForceMediumLightQuality = 1;
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FB032F0
void __fastcall OPTIONS_OnChangeLightQuality(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    SRegSaveValue("Diablo II", "Light Quality", 0, (uint32_t)pItem->nCurrentState);
    switch (pItem->nCurrentState)
    {
    case 0:
        D2GFX_SetLowQuality(1);
        g_bForceMediumLightQuality = 0;
        break;
    case 1:
        D2GFX_SetLowQuality(0);
        g_bForceMediumLightQuality = 0;
        break;
    case 2:
        D2GFX_SetLowQuality(0);
        g_bForceMediumLightQuality = 1;
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FB03360
int32_t __fastcall OPTIONS_SliderStateToRangeValue(int32_t nMinVal, const D2OptionsMenuItemStrc* pItem, int32_t nMaxVal)
{
    const double fSpan    = (double)(uint32_t)(nMaxVal - nMinVal);
    const double fMaxStep = (double)(uint32_t)(pItem->nChoiceCount - 1);
    const double fCurStep = (double)(uint32_t)pItem->nCurrentState;
    return (int32_t)((fSpan / fMaxStep) * fCurStep + (double)(uint32_t)nMinVal);
}

// 1.12a: 0x6FB033E0
int32_t __fastcall OPTIONS_RangeValueToSliderState(int32_t nMaxVal, int32_t nMinVal, int32_t nValue, const D2OptionsMenuItemStrc* pItem)
{
    const double fMaxStep = (double)(uint32_t)(pItem->nChoiceCount - 1);
    const double fOffset  = (double)(uint32_t)(nValue - nMinVal + 1);
    const double fSpan    = (double)(uint32_t)(nMaxVal - nMinVal);
    return (int32_t)((fMaxStep * fOffset) / fSpan);
}

// 1.12a: 0x6FB03450
int32_t __cdecl MINIPANEL_HitTestRightDockedRect(void)
{
    if (gaPanelStates[20] == 0 || gaPanelStates[D2PANEL_MINIPANEL] == 0)
    {
        return 0;
    }

    const int32_t bSinglePlayer = g_bMiniPanelSinglePlayer;
    const int32_t nHalfW = gnScreenWidth / 2;
    const int32_t nLeftX = (bSinglePlayer != 0) ? (nHalfW + 0x38) : (nHalfW + 0x23);
    const int32_t nBottomY = gnScreenHeight - 0x32;
    const int32_t nBtnCount = (bSinglePlayer != 0) ? 7 : 8;

    if (gnMouseX <= nLeftX || gnMouseX >= nLeftX + (nBtnCount * 20))
    {
        return 0;
    }
    if (gnMouseY <= nBottomY - 0x1A || gnMouseY >= nBottomY)
    {
        return 0;
    }
    return 1;
}

// 1.12a: 0x6FB034D0
void __cdecl MINIPANEL_FreeAssetsAndSaveRegistry(void)
{
    const uint32_t dwRegHidden = (g_aEscPanelSaveTable[D2PANEL_MINIPANEL].bSavedState == 0) ? 1u : 0u;
    SRegSaveValue("Diablo II", "Mini Panel", 1, dwRegHidden);

    if (g_pMiniPanelBgCel != nullptr)
    {
        D2CMP_CelFileFreeHardware(g_pMiniPanelBgCel);
        FOG_Free(g_pMiniPanelBgCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
        g_pMiniPanelBgCel = nullptr;
    }

    if (g_pMiniPanelBtnCel != nullptr)
    {
        D2CMP_CelFileFreeHardware(g_pMiniPanelBtnCel);
        FOG_Free(g_pMiniPanelBtnCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
        g_pMiniPanelBtnCel = nullptr;
    }
}

// 1.12a: 0x6FB03560
int32_t __fastcall MINIPANEL_CanOpenMiniPanel(int32_t bRequestedOpen)
{
    if (bRequestedOpen != 0 && gaPanelStates[D2PANEL_TRADEPROMPT] == 0)
    {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB03580 (with jump table at 0x6FB0362C)
int32_t __cdecl MINIPANEL_CheckRightSideObstructed(void)
{
    int32_t bRightBlocked = 0;
    if (gaPanelStates[D2PANEL_TRADEPROMPT] != 0 ||
        gaPanelStates[D2PANEL_STASH] != 0 ||
        gaPanelStates[28] != 0 ||
        gaPanelStates[D2PANEL_CUBE] != 0 ||
        gaPanelStates[D2PANEL_NPCSHOP] != 0 ||
        gaPanelStates[D2PANEL_MSGLOG] != 0 ||
        gaPanelStates[27] != 0 ||
        gaPanelStates[D2PANEL_SKILLSELECT] != 0)
    {
        bRightBlocked = 1;
    }

    if (gaPanelStates[D2PANEL_BELT] != 0 || g_bWeaponSwapActive != 0)
    {
        // Belt type 2 = unequipped (1 row); all other belt types (0, 1, 3, 4, 5, >5) have >= 2 rows and overlap
        if ((uint32_t)g_nEquippedBeltType > 5u || g_nEquippedBeltType != 2)
        {
            bRightBlocked = 1;
        }
    }

    if (gaPanelStates[D2PANEL_INVENTORY] != 0 || gaPanelStates[D2PANEL_SKILLTREE] != 0)
    {
        g_bMiniPanelShiftLeftForRightPanel = 1;
        return 1;
    }

    g_bMiniPanelShiftLeftForRightPanel = 0;
    return bRightBlocked;
}

// 1.12a: 0x6FB03650
int32_t __cdecl MINIPANEL_CheckLeftSideObstructed(void)
{
    const int32_t bMercInvOpen = gaPanelStates[D2PANEL_MERCINV];
    if (gaPanelStates[D2PANEL_TRADEPROMPT] != 0 ||
        gaPanelStates[D2PANEL_STASH] != 0 ||
        gaPanelStates[D2PANEL_CUBE] != 0 ||
        gaPanelStates[D2PANEL_NPCSHOP] != 0 ||
        gaPanelStates[28] != 0 ||
        gaPanelStates[29] != 0 ||
        gaPanelStates[D2PANEL_MSGLOG] != 0 ||
        gaPanelStates[D2PANEL_SKILLSELECT] != 0 ||
        gaPanelStates[D2PANEL_WAYPOINT] != 0 ||
        gaPanelStates[D2PANEL_PARTY] != 0 ||
        bMercInvOpen != 0 ||
        gaPanelStates[D2PANEL_CHAR] != 0 ||
        gaPanelStates[D2PANEL_QUEST] != 0 ||
        gaPanelStates[20] != 0 ||
        gaPanelStates[27] != 0)
    {
        g_bMiniPanelShiftRightForLeftPanel = 1;
        return 1;
    }

    g_bMiniPanelShiftRightForLeftPanel = 0;
    return 0;
}

// 1.12a: 0x6FB03710
void __fastcall MINIPANEL_DrawButtons(int32_t bDraw)
{
    if (bDraw == 0)
    {
        return;
    }

    const int32_t nBtnCount = (g_bMiniPanelSinglePlayer != 0) ? 7 : 8;
    for (int32_t i = 0; i < nBtnCount; ++i)
    {
        D2GfxDataStrc tGfx = {};
        tGfx.pCellFile = g_pMiniPanelBtnCel;
        tGfx.nFrame    = (uint32_t)((g_aMiniPanelBtnPressed[i] == 1) ? (g_aMiniPanelBtnFrame[i] + 1) : g_aMiniPanelBtnFrame[i]);
        TEXTURE_CelDraw(&tGfx, g_aMiniPanelBtnX[i], g_aMiniPanelBtnY[i], 0xFFFFFFFFu, 5, nullptr);
    }
}

// 1.12a: 0x6FB037E0
void __cdecl OPTIONS_SaveAlwaysRunSetting(void)
{
    SRegSaveValue("Diablo II", "Always Run", 0, (uint32_t)gbAlwaysRun);
}

// 1.12a: 0x6FB03800
void __cdecl OPTIONS_LoadAlwaysRunSetting(void)
{
    int32_t bVal = 0;
    SRegLoadValue("Diablo II", "Always Run", 0, &bVal);
    gbAlwaysRun = bVal;
}

// 1.12a: 0x6FB03830
void __fastcall OPTIONS_DrawSliderBar(int32_t nItemBaseY, int32_t nUnusedEdx, const D2OptionsMenuItemStrc* pItem, int32_t nDrawMode, int32_t bCenteredBar)
{
    (void)nUnusedEdx;
    const int32_t nBarBottomY = g_pActiveOptionsHeader->nSliderOffsetY + nItemBaseY;
    const int32_t nHalfW = gnScreenWidth / 2;
    const int32_t nSkullBaseX = (bCenteredBar != 0) ? (nHalfW - 0x91) : (nHalfW - 0x3C);

    const double fCurStep = (double)(uint32_t)pItem->nCurrentState;
    const double fMaxStep = (double)(uint32_t)(pItem->nChoiceCount - 1);
    const int32_t nNegOffset = (int32_t)((fCurStep / fMaxStep) * -265.0);

    const int32_t nSkullX = nSkullBaseX - nNegOffset;
    const int32_t nFillWidth = (nSkullX - nSkullBaseX) + 12;
    const int32_t nBoxTopY = nBarBottomY - 30;
    const int32_t nSkullY = nBarBottomY - ((pItem->bUseCompactSliderBar == 0) ? 1 : 0) - 1;
    const int32_t nBoxLeftX = nHalfW + ((bCenteredBar != 0) ? -0x90 : -0x3B);
    const int32_t nSplitX = nBoxLeftX + nFillWidth;

    const int32_t nLeftTransMode = (pItem->bUseCompactSliderBar == 0) ? 2 : 1;
    D2GFX_DrawBox(nBoxLeftX, nBoxTopY, nSplitX, nBoxTopY + 30, 0, nLeftTransMode);

    const int32_t nRightTransMode = (pItem->bUseCompactSliderBar != 0) ? 1 : 0;
    D2GFX_DrawBox(nSplitX, nBoxTopY, nBoxLeftX + 0x121, nBoxTopY + 30, 0, nRightTransMode);

    const int32_t nBarDrawX = (bCenteredBar != 0) ? (gnScreenWidth / 2) : ((gnScreenWidth / 2) + 0xE6);
    const int32_t nBarAlign = (bCenteredBar != 0) ? 1 : 2;
    D2CellFileStrc* pBarCel = (pItem->bUseCompactSliderBar != 0) ? g_pOptBarCompactCel : g_pOptBarCel;
    D2Win_10134_DrawCellFile(pBarCel, nBarDrawX, nBarBottomY, nBarAlign, nDrawMode, -1);

    D2GfxDataStrc tSkullGfx = {};
    tSkullGfx.pCellFile = g_pOptSkullCel;
    TEXTURE_CelDraw(&tSkullGfx, nSkullX, nSkullY, 0xFFFFFFFFu, nDrawMode, nullptr);
}

// 1.12a: 0x6FB039D0
void __fastcall OPTIONS_FreeMenuTableAssets(D2OptionsMenuItemStrc* pItems, int32_t nUnusedEdx, const D2OptionsMenuHeaderStrc* pHeader)
{
    (void)nUnusedEdx;
    if (pHeader == nullptr)
    {
        OPTIONS_HaltAtLine(0x5BA);
    }
    if (pItems == nullptr)
    {
        OPTIONS_HaltAtLine(0x5BB);
    }

    for (int32_t i = 0; i < pHeader->nItemCount; ++i)
    {
        D2OptionsMenuItemStrc& rItem = pItems[i];
        if (rItem.pLabelCel != nullptr)
        {
            D2CMP_CelFileFreeHardware(rItem.pLabelCel);
            FOG_Free(rItem.pLabelCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
            rItem.pLabelCel = nullptr;
        }

        if (rItem.nItemType == 1)
        {
            const uint32_t uChoiceCount = (uint32_t)rItem.nChoiceCount;
            if (uChoiceCount == 0 || uChoiceCount > 4u)
            {
                OPTIONS_HaltAtLine(0x5D8);
            }
            for (uint32_t j = 0; j < uChoiceCount; ++j)
            {
                if (rItem.apChoiceCels[j] != nullptr)
                {
                    D2CMP_CelFileFreeHardware(rItem.apChoiceCels[j]);
                    FOG_Free(rItem.apChoiceCels[j], "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
                    rItem.apChoiceCels[j] = nullptr;
                }
            }
        }
    }
}

// 1.12a: 0x6FB03AF0
int32_t __fastcall OPTIONS_HitTestMenuRowY(int32_t nMouseY)
{
    const D2OptionsMenuHeaderStrc* pHeader = g_pActiveOptionsHeader;
    const int32_t nItemCount = pHeader->nItemCount;
    const int32_t nHalfMenuH = (nItemCount * pHeader->nLineStepY) / 2;
    const int32_t nCenterY = (gnScreenHeight - 0x50) / 2;

    if (nMouseY <= nCenterY - nHalfMenuH || nMouseY >= nCenterY + nHalfMenuH)
    {
        return -1;
    }

    int32_t nHitIdx = -1;
    const D2OptionsMenuItemStrc* pItems = g_pActiveOptionsItems;
    for (int32_t i = 0; i < nItemCount; ++i)
    {
        const int32_t nRowY = pItems[i].nRenderedBaseY;
        if (nRowY != 0 && nMouseY >= nRowY + nItemCount)
        {
            nHitIdx = i;
        }
    }

    if (nHitIdx >= 0)
    {
        const D2OptionsMenuItemStrc& rHit = pItems[nHitIdx];
        if (rHit.nItemType == -1)
        {
            nHitIdx = -1;
        }
        else if (rHit.pfnCanUse != nullptr && rHit.pfnCanUse() == 0)
        {
            nHitIdx = -1;
        }
    }

    return nHitIdx;
}

// 1.12a: 0x6FB03B80
void __fastcall OPTIONS_OnChangeContrast(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    const int32_t nContrast = OPTIONS_SliderStateToRangeValue(0, pItem, 100);
    SRegSaveValue("Diablo II", "Contrast", 0, (uint32_t)nContrast);
    D2GFX_SetOption(11, pItem->nCurrentState);
    D2GFX_SetGamma(0);
}

// 1.12a: 0x6FB03BC0
void __fastcall OPTIONS_OnChangeGamma(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    const int32_t nGamma = OPTIONS_SliderStateToRangeValue(55, pItem, 255);
    SRegSaveValue("Diablo II", "Gamma", 0, (uint32_t)nGamma);
    D2GFX_SetGamma((uint32_t)nGamma);
}

// 1.12a: 0x6FB03BF0
void __fastcall OPTIONS_OnChangeNPCSpeech(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    D2SOUND_SetNPCSpeech(pItem->nCurrentState);
    switch (pItem->nCurrentState)
    {
    case 0:
        gbNpcSpeechEnabled = 1;
        g_bTextDisplayBeta = 0;
        SRegSaveValue("Diablo II", "Text Display Beta", 0, 0);
        break;
    case 1:
        gbNpcSpeechEnabled = 0;
        g_bTextDisplayBeta = 1;
        SRegSaveValue("Diablo II", "Text Display Beta", 0, 1);
        break;
    case 2:
        gbNpcSpeechEnabled = 1;
        g_bTextDisplayBeta = 1;
        SRegSaveValue("Diablo II", "Text Display Beta", 0, 1);
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FB03C80
int32_t __cdecl OPTIONS_CanUseNPCSpeech(void)
{
    return g_bSoundSubsystemInitialized;
}

// 1.12a: 0x6FB03C90
void __fastcall OPTIONS_InitPositionalBias(D2OptionsMenuItemStrc* pItem)
{
    const int32_t nBias = D2SOUND_GetPositionalBias();
    pItem->nCurrentState = OPTIONS_RangeValueToSliderState(100, 0, nBias, pItem);
}

// 1.12a: 0x6FB03CB0
void __fastcall OPTIONS_OnChangePositionalBias(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    const int32_t nBias = OPTIONS_SliderStateToRangeValue(0, pItem, 100);
    D2SOUND_SetPositionalBias(nBias);
}

// 1.12a: 0x6FB03CD0
void __fastcall OPTIONS_InitMusicVolume(D2OptionsMenuItemStrc* pItem)
{
    const int32_t nVol = D2SOUND_GetMusicVolume();
    pItem->nCurrentState = OPTIONS_RangeValueToSliderState(100, 0, nVol, pItem);
}

// 1.12a: 0x6FB03CF0
void __fastcall OPTIONS_OnChangeMusicVolume(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    const int32_t nVol = OPTIONS_SliderStateToRangeValue(0, pItem, 100);
    D2SOUND_SetMusicVolume(nVol);
}

// 1.12a: 0x6FB03D10
int32_t __cdecl OPTIONS_CanUseMusicVolume(void)
{
    return g_bSoundSubsystemInitialized;
}

// 1.12a: 0x6FB03D20
void __fastcall OPTIONS_InitMasterVolume(D2OptionsMenuItemStrc* pItem)
{
    const int32_t nVol = D2SOUND_GetMasterVolume();
    pItem->nCurrentState = OPTIONS_RangeValueToSliderState(100, 0, nVol, pItem);
}

// 1.12a: 0x6FB03D40
void __fastcall OPTIONS_OnChangeMasterVolume(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    const int32_t nVol = OPTIONS_SliderStateToRangeValue(0, pItem, 100);
    D2SOUND_SetMasterVolume(nVol);
}

// 1.12a: 0x6FB03D60
int32_t __cdecl OPTIONS_CanUseMasterVolume(void)
{
    return g_bSoundSubsystemInitialized;
}

// 1.12a: 0x6FB03D70
int32_t __cdecl OPTIONS_CanUseSoundOptionsMenu(void)
{
    return g_bSoundSubsystemInitialized;
}

// 1.12a: 0x6FB03D80 (with jump table at 0x6FB03F34)
void __fastcall MINIPANEL_DrawButtonTooltip(int32_t nBtnAction, int32_t nUnusedEdx, int32_t nBtnX, int32_t nBtnY)
{
    (void)nUnusedEdx;
    if ((uint32_t)nBtnAction > 7u)
    {
        return;
    }

    uint16_t wStringId = 0x1050;
    int32_t bShowHotkey = 0;
    int32_t nCommandId = 57;

    switch (nBtnAction)
    {
    case 0:
        wStringId = 0x1049; // Character Screen
        bShowHotkey = 1;
        nCommandId = 0;
        break;
    case 1:
        wStringId = 0x104A; // Inventory Screen
        bShowHotkey = 1;
        nCommandId = 1;
        break;
    case 2:
        wStringId = 0x104B; // Skill Tree
        bShowHotkey = 1;
        nCommandId = 12;
        break;
    case 3:
        wStringId = 0x104C; // Party Screen
        bShowHotkey = 1;
        nCommandId = 2;
        break;
    case 4:
        wStringId = 0x104D; // Automap
        bShowHotkey = 1;
        nCommandId = 7;
        break;
    case 5:
        wStringId = 0x104E; // Message Log
        bShowHotkey = 1;
        nCommandId = 3;
        break;
    case 6:
        wStringId = 0x104F; // Quest Log
        bShowHotkey = 1;
        nCommandId = 4;
        break;
    case 7:
        wStringId = 0x1050; // Game Menu
        bShowHotkey = 0;
        nCommandId = 57;
        break;
    default:
        return;
    }

    Unicode wszTooltip[200] = {};
    D2LANG_UnicodeStrCpy(wszTooltip, D2LANG_GetStringFromTblIndex(wStringId));

    if (bShowHotkey != 0 && nCommandId != 57)
    {
        if (CMDTBL_GetBoundKey(nCommandId, 1) != 0xFFFFu)
        {
            Unicode wszKeySuffix[30] = {};
            const Unicode* wszKeyName = CMDTBL_GetKeyNameString(nCommandId, 1);
            D2LANG_UnicodeSPrintf(30, wszKeySuffix, D2LANG_GetStringFromTblIndex(0x1052), wszKeyName);
            D2LANG_UnicodeStrCat(wszTooltip, wszKeySuffix);
        }
        if (CMDTBL_GetBoundKey(nCommandId, 0) != 0xFFFFu)
        {
            Unicode wszKeySuffix[30] = {};
            const Unicode* wszKeyName = CMDTBL_GetKeyNameString(nCommandId, 0);
            D2LANG_UnicodeSPrintf(30, wszKeySuffix, D2LANG_GetStringFromTblIndex(0x1052), wszKeyName);
            D2LANG_UnicodeStrCat(wszTooltip, wszKeySuffix);
        }
    }

    D2Win_10130_DrawFramedTextEx(wszTooltip, nBtnX - 3, nBtnY - 26, 0, 1);
}

// 1.12a: 0x6FB03F60
void __cdecl OPTIONS_FreeAllMenuAssets(void)
{
    g_pActiveOptionsHeader = nullptr;
    g_pActiveOptionsItems  = nullptr;

    OPTIONS_FreeMenuTableAssets(g_aSoundMenuItems, 0, &g_tSoundMenuHeader);
    OPTIONS_FreeMenuTableAssets(g_aOptionsSubMenuItems, 0, &g_tOptionsSubMenuHeader);
    OPTIONS_FreeMenuTableAssets(g_aMainEscMenuItems, 0, &g_tMainEscMenuHeader);

    if (FOG_IsExpansion() != 0)
    {
        OPTIONS_FreeMenuTableAssets(g_aVideoMenuExpItems, 0, &g_tVideoMenuExpHeader);
        OPTIONS_FreeMenuTableAssets(g_aAutomapMenuExpItems, 0, &g_tAutomapMenuExpHeader);
    }
    else
    {
        OPTIONS_FreeMenuTableAssets(g_aVideoMenuClassicItems, 0, &g_tVideoMenuClassicHeader);
        OPTIONS_FreeMenuTableAssets(g_aAutomapMenuClassicItems, 0, &g_tAutomapMenuClassicHeader);
    }

    if (g_pOptBarCel != nullptr)
    {
        D2CMP_CelFileFreeHardware(g_pOptBarCel);
        FOG_Free(g_pOptBarCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
        g_pOptBarCel = nullptr;
    }

    if (g_pOptBarCompactCel != nullptr)
    {
        D2CMP_CelFileFreeHardware(g_pOptBarCompactCel);
        FOG_Free(g_pOptBarCompactCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
        g_pOptBarCompactCel = nullptr;
    }

    if (g_pOptSkullCel != nullptr)
    {
        D2CMP_CelFileFreeHardware(g_pOptSkullCel);
        FOG_Free(g_pOptSkullCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
        g_pOptSkullCel = nullptr;
    }
}

// 1.12a: 0x6FB04060
void __stdcall OPTIONS_LoadMenuTableAssets(const D2OptionsMenuHeaderStrc* pHeader, D2OptionsMenuItemStrc* pItems)
{
    if (pHeader == nullptr)
    {
        OPTIONS_HaltAtLine(0x524);
    }
    if (pItems == nullptr)
    {
        OPTIONS_HaltAtLine(0x525);
    }

    char szPath[260] = {};
    char szLangCode[4] = {};
    STRTABLE_GetLanguageInfo(szLangCode, 0);

    for (int32_t i = 0; i < pHeader->nItemCount; ++i)
    {
        D2OptionsMenuItemStrc& rItem = pItems[i];
        if (rItem.bExpansionOnly == 0 || FOG_IsExpansion() != 0)
        {
            if (rItem.szLabelCelName[0] != '\0')
            {
                snprintf(szPath, sizeof(szPath), "%s\\UI\\%s\\%s", "DATA\\LOCAL", szLangCode, rItem.szLabelCelName);
                rItem.pLabelCel = ARCHIVE_LoadCellFileDefault(szPath, 0);

                if (rItem.nItemType == 1)
                {
                    const uint32_t uChoiceCount = (uint32_t)rItem.nChoiceCount;
                    if (uChoiceCount == 0 || uChoiceCount > 4u)
                    {
                        OPTIONS_HaltAtLine(0x561);
                    }
                    for (uint32_t j = 0; j < uChoiceCount; ++j)
                    {
                        if (rItem.aszChoiceCelNames[j][0] == '\0')
                        {
                            OPTIONS_HaltAtLine(0x564);
                        }
                        snprintf(szPath, sizeof(szPath), "%s\\UI\\%s\\%s", "DATA\\LOCAL", szLangCode, rItem.aszChoiceCelNames[j]);
                        rItem.apChoiceCels[j] = ARCHIVE_LoadCellFileDefault(szPath, 0);
                    }
                }
            }

            if (rItem.pfnOnLoad != nullptr)
            {
                rItem.pfnOnLoad(&rItem);
            }
        }
    }
}

// 1.12a: 0x6FB04250
void __fastcall OPTIONS_SelectHoveredItemByY(int32_t nMouseY)
{
    const int32_t nHitIdx = OPTIONS_HitTestMenuRowY(nMouseY);
    if (nHitIdx != -1)
    {
        g_nSelectedOptionsItemIdx = nHitIdx;
    }
}

// 1.12a: 0x6FB04260
void __fastcall OPTIONS_InitContrastSlider(D2OptionsMenuItemStrc* pItem)
{
    int32_t nContrast = (D2GFX_GetDisplayType() != 4) ? 100 : 0;
    SRegLoadValue("Diablo II", "Contrast", 0, &nContrast);
    pItem->nCurrentState = OPTIONS_RangeValueToSliderState(100, 0, nContrast, pItem);

    const int32_t nSavedContrast = OPTIONS_SliderStateToRangeValue(0, pItem, 100);
    SRegSaveValue("Diablo II", "Contrast", 0, (uint32_t)nSavedContrast);
    D2GFX_SetOption(11, pItem->nCurrentState);
    D2GFX_SetGamma(0);
}

// 1.12a: 0x6FB042E0
void __fastcall OPTIONS_InitGammaSlider(D2OptionsMenuItemStrc* pItem)
{
    int32_t nGamma = 155;
    SRegLoadValue("Diablo II", "Gamma", 0, &nGamma);
    pItem->nCurrentState = OPTIONS_RangeValueToSliderState(255, 55, nGamma, pItem);

    if (D2GFX_GammaCanBeControlled() != 0)
    {
        const int32_t nSavedGamma = OPTIONS_SliderStateToRangeValue(55, pItem, 255);
        SRegSaveValue("Diablo II", "Gamma", 0, (uint32_t)nSavedGamma);
        D2GFX_SetGamma((uint32_t)nSavedGamma);
    }
}

// 1.12a: 0x6FB04360
void __fastcall OPTIONS_OnChangePerspective(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    SRegSaveValue("Diablo II", "Perspective", 0, (uint32_t)pItem->nCurrentState);
    D2GFX_SetPerspective(pItem->nCurrentState);
    VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), 0, 0, gnViewportWidth, gnViewportHeight);
}

// 1.12a: 0x6FB043B0
void __fastcall OPTIONS_InitNPCSpeech(D2OptionsMenuItemStrc* pItem)
{
    pItem->nCurrentState = D2SOUND_GetNPCSpeech();
    if (g_bSoundSubsystemInitialized != 0)
    {
        OPTIONS_OnChangeNPCSpeech(pItem, nullptr);
    }
}

// 1.12a: 0x6FB043E0
void __fastcall OPTIONS_SyncEAXState(D2OptionsMenuItemStrc* pItem)
{
    if (g_bSoundSubsystemInitialized == 0)
    {
        pItem->nCurrentState = 0;
        return;
    }
    pItem->nCurrentState = (D2SOUND_GetSoundMixer() == 2) ? 1 : 0;
}

// 1.12a: 0x6FB04410
int32_t __cdecl OPTIONS_CanUseEAX(void)
{
    if (g_bSoundSubsystemInitialized == 0)
    {
        return 0;
    }
    return D2SOUND_IsEaxEnabled();
}

// 1.12a: 0x6FB04430
int32_t __cdecl OPTIONS_CanUsePositionalBias(void)
{
    if (g_bSoundSubsystemInitialized == 0)
    {
        return 0;
    }
    const int32_t nMixer = D2SOUND_GetSoundMixer();
    return (nMixer == 1 || nMixer == 2) ? 1 : 0;
}

// 1.12a: 0x6FB04460
void __fastcall OPTIONS_Sync3DSoundState(D2OptionsMenuItemStrc* pItem)
{
    if (g_bSoundSubsystemInitialized != 0)
    {
        const int32_t nMixer = D2SOUND_GetSoundMixer();
        if (nMixer == 1 || nMixer == 2)
        {
            pItem->nCurrentState = 1;
            return;
        }
    }
    pItem->nCurrentState = 0;
}

// 1.12a: 0x6FB044A0
int32_t __cdecl OPTIONS_CanUse3DSound(void)
{
    if (g_bSoundSubsystemInitialized == 0)
    {
        return 0;
    }
    return D2SOUND_Is3DSoundEnabled();
}

// 1.12a: 0x6FB044C0
void __cdecl MINIPANEL_LoadAssets(void)
{
    if (gnGameType != 0)
    {
        g_pMiniPanelBgCel = UI_LoadGlobalUICellFile("Panel\\minipanel");
        g_bMiniPanelSinglePlayer = 0;
    }
    else
    {
        g_pMiniPanelBgCel = UI_LoadGlobalUICellFile("Panel\\minipanel_s");
        g_bMiniPanelSinglePlayer = 1;
    }

    g_pMiniPanelBtnCel = UI_LoadGlobalUICellFile("Panel\\minipanelbtn");

    const int32_t nBtnCount = (g_bMiniPanelSinglePlayer != 0) ? 7 : 8;
    for (int32_t i = 0; i < nBtnCount; ++i)
    {
        g_aMiniPanelBtnPressed[i] = 0;
    }
}

// 1.12a: 0x6FB04530
void __stdcall MINIPANEL_OnLButtonDown(D2WinMsgStrc* pMsg)
{
    const int32_t nBtnCount = (g_bMiniPanelSinglePlayer != 0) ? 7 : 8;
    if (gpPlayerUnit == nullptr ||
        gpPlayerUnit->pInventory == nullptr ||
        INVENTORY_GetCursorItem(gpPlayerUnit->pInventory) != nullptr ||
        g_bMiniPanelHiddenByPanels != 0)
    {
        return;
    }

    const int32_t nMouseX = (int32_t)pMsg->nX;
    const int32_t nMouseY = (int32_t)pMsg->nY;
    int32_t nShiftX = 0;

    if (g_bWeaponSwapActive != 0 && CTRLPNL_GetEquippedBeltRows() > 1)
    {
        nShiftX = -0x76;
    }
    if (g_bMiniPanelShiftLeftForRightPanel != 0)
    {
        nShiftX -= 0x76;
    }
    if (g_bMiniPanelShiftRightForLeftPanel != 0)
    {
        nShiftX += 0x77;
    }

    const int32_t nCenterX = (gnScreenWidth / 2) + nShiftX;
    if (nMouseX <= nCenterX - 0x54 || nMouseX >= nCenterX + 0x59)
    {
        return;
    }
    if (nMouseY <= gnScreenHeight - 0x45 || nMouseY >= gnScreenHeight - 0x2F)
    {
        return;
    }

    for (int32_t i = 0; i < nBtnCount; ++i)
    {
        if (nMouseX > g_aMiniPanelBtnX[i] && nMouseX < g_aMiniPanelBtnX[i] + 20)
        {
            if (g_aMiniPanelBtnAction[i] == 7)
            {
                g_aMiniPanelBtnPressed[i] = 1;
            }
            else if (gpPlayerUnit->dwUnitType == UNIT_PLAYER_TYPE && gpPlayerUnit->dwAnimMode != PLRMODE_DEAD_CONST)
            {
                const int32_t nMinAllowedWhenEscOpen = (gnGameType != 0) ? 5 : 4;
                if (gaPanelStates[D2PANEL_ESCMENU] == 0 || i >= nMinAllowedWhenEscOpen)
                {
                    g_aMiniPanelBtnPressed[i] = 1;
                }
            }

            SOUND_PlaySoundEffect(4, nullptr, 0, 0, 0);
            g_bMiniPanelButtonDown = 1;
            pMsg->bHandled = 1;
            pMsg->lResult  = 0;
            SMsgBreakHandlerChain(pMsg);
            return;
        }
    }
}

// 1.12a: 0x6FB046B0
void __fastcall MINIPANEL_RenderHoveredTooltip(int32_t nDockMode)
{
    const int32_t nBtnCount = (g_bMiniPanelSinglePlayer != 0) ? 7 : 8;
    if (nDockMode == 0)
    {
        return;
    }

    int32_t nShiftX = 0;
    if (nDockMode == 1)
    {
        nShiftX = -0x76;
    }
    else if (nDockMode == 3)
    {
        nShiftX = 0x77;
    }

    const int32_t nCenterX = (gnScreenWidth / 2) + nShiftX;
    if (gnMouseX <= nCenterX - 0x54 || gnMouseX >= nCenterX + 0x59)
    {
        return;
    }
    if (gnMouseY <= gnScreenHeight - 0x45 || gnMouseY >= gnScreenHeight - 0x2F)
    {
        return;
    }

    for (int32_t i = 0; i < nBtnCount; ++i)
    {
        if (gnMouseX > g_aMiniPanelBtnX[i] && gnMouseX < g_aMiniPanelBtnX[i] + 20)
        {
            MINIPANEL_DrawButtonTooltip(g_aMiniPanelBtnAction[i], 0, g_aMiniPanelBtnX[i], g_aMiniPanelBtnY[i]);
        }
    }
}

// 1.12a: 0x6FB04760
void __cdecl OPTIONS_LoadAllMenuAssets(void)
{
    OPTIONS_LoadMenuTableAssets(&g_tMainEscMenuHeader, g_aMainEscMenuItems);
    OPTIONS_LoadMenuTableAssets(&g_tOptionsSubMenuHeader, g_aOptionsSubMenuItems);
    OPTIONS_LoadMenuTableAssets(&g_tSoundMenuHeader, g_aSoundMenuItems);

    if (FOG_IsExpansion() != 0)
    {
        OPTIONS_LoadMenuTableAssets(&g_tVideoMenuExpHeader, g_aVideoMenuExpItems);
        OPTIONS_LoadMenuTableAssets(&g_tAutomapMenuExpHeader, g_aAutomapMenuExpItems);
    }
    else
    {
        OPTIONS_LoadMenuTableAssets(&g_tVideoMenuClassicHeader, g_aVideoMenuClassicItems);
        OPTIONS_LoadMenuTableAssets(&g_tAutomapMenuClassicHeader, g_aAutomapMenuClassicItems);
    }

    g_pActiveOptionsHeader = &g_tMainEscMenuHeader;
    g_pActiveOptionsItems  = g_aMainEscMenuItems;

    char szPath[260] = {};
    snprintf(szPath, sizeof(szPath), "%s\\UI\\Widgets\\OptBar", "DATA\\GLOBAL");
    g_pOptBarCel = ARCHIVE_LoadCellFileDefault(szPath, 0);

    snprintf(szPath, sizeof(szPath), "%s\\UI\\Widgets\\OptBarC", "DATA\\GLOBAL");
    g_pOptBarCompactCel = ARCHIVE_LoadCellFileDefault(szPath, 0);

    snprintf(szPath, sizeof(szPath), "%s\\UI\\Widgets\\OptSkull", "DATA\\GLOBAL");
    g_pOptSkullCel = ARCHIVE_LoadCellFileDefault(szPath, 0);

    D2GfxDataStrc tPentGfx = {};
    tPentGfx.pCellFile = g_pPentSpinCel;
    const uint32_t uFrames = (uint32_t)D2CMP_CelFileGetCelsPerDirection(g_pPentSpinCel);
    int32_t nMaxWidth = 0;
    for (uint32_t f = 0; f < uFrames; ++f)
    {
        tPentGfx.nFrame = f;
        D2CellFileStrc* pCelHandle = D2CMP_CelGetHandle(&tPentGfx);
        const int32_t nW = D2CMP_CelGetWidth(pCelHandle);
        if (nW > nMaxWidth)
        {
            nMaxWidth = nW;
        }
    }
    g_nMaxPentagramWidth = nMaxWidth;
}

// 1.12a: 0x6FB048E0
void __stdcall OPTIONS_OnRightArrowKey(D2WinMsgStrc* pMsg)
{
    int32_t nIdx = g_nSelectedOptionsItemIdx;
    D2OptionsMenuItemStrc* pItem = &g_pActiveOptionsItems[nIdx];
    const int32_t nOldState = pItem->nCurrentState;

    if (pItem->pfnCanUse == nullptr || pItem->pfnCanUse() != 0)
    {
        nIdx = g_nSelectedOptionsItemIdx;
        pItem = &g_pActiveOptionsItems[nIdx];
        if (pItem->nItemType == 1)
        {
            const uint32_t uNext = (uint32_t)(pItem->nCurrentState + 1);
            pItem->nCurrentState = (uNext > (uint32_t)(pItem->nChoiceCount - 1)) ? 0 : (int32_t)uNext;
        }
        else if (pItem->nItemType == 2)
        {
            const uint32_t uNext = (uint32_t)(pItem->nCurrentState + 1);
            const uint32_t uMax  = (uint32_t)(pItem->nChoiceCount - 1);
            pItem->nCurrentState = (uNext > uMax) ? (int32_t)uMax : (int32_t)uNext;
        }
    }

    pItem = &g_pActiveOptionsItems[nIdx];
    if (pItem->nCurrentState != nOldState)
    {
        if (pItem->pfnOnChange != nullptr)
        {
            pItem->pfnOnChange(pItem, nullptr);
        }
        SOUND_PlaySoundEffect(1, nullptr, 0, 0, 0);
    }

    pMsg->bHandled = 1;
    pMsg->lResult  = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB049C0
void __stdcall OPTIONS_OnLeftArrowKey(D2WinMsgStrc* pMsg)
{
    int32_t nIdx = g_nSelectedOptionsItemIdx;
    D2OptionsMenuItemStrc* pItem = &g_pActiveOptionsItems[nIdx];
    const int32_t nOldState = pItem->nCurrentState;

    if (pItem->pfnCanUse == nullptr || pItem->pfnCanUse() != 0)
    {
        nIdx = g_nSelectedOptionsItemIdx;
        pItem = &g_pActiveOptionsItems[nIdx];
        if (pItem->nItemType == 1)
        {
            const uint32_t uPrev = (uint32_t)(pItem->nCurrentState - 1);
            const uint32_t uMax  = (uint32_t)(pItem->nChoiceCount - 1);
            pItem->nCurrentState = (uPrev > uMax) ? (int32_t)uMax : (int32_t)uPrev;
        }
        else if (pItem->nItemType == 2)
        {
            const uint32_t uPrev = (uint32_t)(pItem->nCurrentState - 1);
            const uint32_t uMax  = (uint32_t)(pItem->nChoiceCount - 1);
            pItem->nCurrentState = (uPrev > uMax) ? 0 : (int32_t)uPrev;
        }
    }

    pItem = &g_pActiveOptionsItems[nIdx];
    if (pItem->nCurrentState != nOldState)
    {
        if (pItem->pfnOnChange != nullptr)
        {
            pItem->pfnOnChange(pItem, nullptr);
        }
        SOUND_PlaySoundEffect(1, nullptr, 0, 0, 0);
    }

    pMsg->bHandled = 1;
    pMsg->lResult  = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB04AA0
void __stdcall OPTIONS_OnUpArrowKey(D2WinMsgStrc* pMsg)
{
    for (;;)
    {
        int32_t nIdx = g_nSelectedOptionsItemIdx - 1;
        g_nSelectedOptionsItemIdx = nIdx;
        if (nIdx < 0)
        {
            nIdx = g_pActiveOptionsHeader->nItemCount - 1;
            g_nSelectedOptionsItemIdx = nIdx;
        }

        const D2OptionsMenuItemStrc& rItem = g_pActiveOptionsItems[nIdx];
        if (rItem.nItemType == -1)
        {
            continue;
        }
        if (rItem.pfnCanUse != nullptr && rItem.pfnCanUse() == 0)
        {
            continue;
        }
        break;
    }

    SOUND_PlaySoundEffect(1, nullptr, 0, 0, 0);
    pMsg->bHandled = 1;
    pMsg->lResult  = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB04B10
void __stdcall OPTIONS_OnDownArrowKey(D2WinMsgStrc* pMsg)
{
    for (;;)
    {
        int32_t nIdx = g_nSelectedOptionsItemIdx + 1;
        g_nSelectedOptionsItemIdx = nIdx;
        if (nIdx > g_pActiveOptionsHeader->nItemCount - 1)
        {
            nIdx = 0;
            g_nSelectedOptionsItemIdx = 0;
        }

        const D2OptionsMenuItemStrc& rItem = g_pActiveOptionsItems[nIdx];
        if (rItem.nItemType == -1)
        {
            continue;
        }
        if (rItem.pfnCanUse != nullptr && rItem.pfnCanUse() == 0)
        {
            continue;
        }
        break;
    }

    SOUND_PlaySoundEffect(1, nullptr, 0, 0, 0);
    pMsg->bHandled = 1;
    pMsg->lResult  = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB04B90
void __fastcall OPTIONS_UpdateSliderDragAtMouse(int32_t nMouseY, int32_t nUnusedEdx, int32_t nMouseX)
{
    (void)nUnusedEdx;
    if (g_bOptionsSliderDragging == 0)
    {
        const int32_t nHitIdx = OPTIONS_HitTestMenuRowY(nMouseY);
        if (nHitIdx != g_nSelectedOptionsItemIdx)
        {
            return;
        }
    }

    int32_t nIdx = g_nSelectedOptionsItemIdx;
    D2OptionsMenuItemStrc* pItem = &g_pActiveOptionsItems[nIdx];
    const int32_t nOldState = pItem->nCurrentState;

    if (pItem->pfnCanUse == nullptr || pItem->pfnCanUse() != 0)
    {
        nIdx = g_nSelectedOptionsItemIdx;
        pItem = &g_pActiveOptionsItems[nIdx];
        if (pItem->nItemType == 2)
        {
            const int32_t nHalfW = gnScreenWidth / 2;
            const int32_t nBoxLeftX   = (pItem->pLabelCel != nullptr) ? (nHalfW - 0x3B) : (nHalfW - 0x90);
            const int32_t nTrackBaseX = (pItem->pLabelCel != nullptr) ? (nHalfW - 0x3C) : (nHalfW - 0x91);

            if (g_bOptionsSliderDragging != 0 || (nMouseX > nBoxLeftX && nMouseX < nBoxLeftX + 0x121))
            {
                const int32_t nTrackMinX = nTrackBaseX + 12;
                if (nMouseX < nTrackMinX)
                {
                    pItem->nCurrentState = 0;
                }
                else if (nMouseX > nTrackMinX + 265)
                {
                    pItem->nCurrentState = pItem->nChoiceCount - 1;
                }
                else
                {
                    const double fDeltaX  = (double)(nMouseX - nTrackMinX);
                    const double fMaxStep = (double)(uint32_t)(pItem->nChoiceCount - 1);
                    const int32_t nTwice  = (int32_t)((fDeltaX / (132.5 / fMaxStep)) + 1.0);
                    pItem->nCurrentState  = nTwice / 2;
                }
                g_bOptionsSliderDragging = 1;
            }
        }
    }

    pItem = &g_pActiveOptionsItems[nIdx];
    if (pItem->nCurrentState != nOldState)
    {
        if (pItem->pfnOnChange != nullptr)
        {
            pItem->pfnOnChange(pItem, nullptr);
        }
        SOUND_PlaySoundEffect(1, nullptr, 0, 0, 0);
    }
}

// 1.12a: 0x6FB04D40
void __fastcall OPTIONS_ActivateSelectedItem(D2WinMsgStrc* pMsg)
{
    int32_t nIdx = g_nSelectedOptionsItemIdx;
    D2OptionsMenuItemStrc* pItem = &g_pActiveOptionsItems[nIdx];
    if (pItem->pfnCanUse != nullptr && pItem->pfnCanUse() == 0)
    {
        return;
    }

    nIdx = g_nSelectedOptionsItemIdx;
    pItem = &g_pActiveOptionsItems[nIdx];
    if (pItem->nItemType == 0)
    {
        if (pItem->pfnOnChange != nullptr)
        {
            pItem->pfnOnChange(pItem, pMsg);
            SOUND_PlaySoundEffect(2, nullptr, 0, 0, 0);
        }
    }
    else if (pItem->nItemType == 1)
    {
        const uint32_t uNext = (uint32_t)(pItem->nCurrentState + 1);
        pItem->nCurrentState = (uNext > (uint32_t)(pItem->nChoiceCount - 1)) ? 0 : (int32_t)uNext;
        if (pItem->pfnOnChange != nullptr)
        {
            pItem->pfnOnChange(pItem, pMsg);
        }
        SOUND_PlaySoundEffect(1, nullptr, 0, 0, 0);
    }
}

// 1.12a: 0x6FB04DF0
void __fastcall OPTIONS_InitAutomapMode(D2OptionsMenuItemStrc* pItem)
{
    int32_t nMode = 0;
    SRegLoadValue("Diablo II", "AutoMapMode", 0, &nMode);
    if ((uint32_t)nMode > 2u)
    {
        nMode = 0;
    }

    if (nMode != gnAutomapMode)
    {
        gnAutomapMode    = nMode;
        gnAutomapDivisor = (nMode == 1) ? 20 : 10;
        AUTOMAP_LoadSprites();
        AUTOMAP_CenterView(1);
    }
    pItem->nCurrentState = gnAutomapMode;
}

// 1.12a: 0x6FB04E60
void __fastcall OPTIONS_OnChangeAutomapMode(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    if ((uint32_t)pItem->nCurrentState > 2u)
    {
        pItem->nCurrentState = 0;
    }

    const int32_t nMode = pItem->nCurrentState;
    if (nMode != gnAutomapMode)
    {
        gnAutomapMode    = nMode;
        gnAutomapDivisor = (nMode == 1) ? 20 : 10;
        AUTOMAP_LoadSprites();
        AUTOMAP_CenterView(1);
    }
    SRegSaveValue("Diablo II", "AutoMapMode", 0, (uint32_t)pItem->nCurrentState);
}

// 1.12a: 0x6FB04ED0
void __fastcall OPTIONS_InitPerspective(D2OptionsMenuItemStrc* pItem)
{
    SRegLoadValue("Diablo II", "Perspective", 0, &pItem->nCurrentState);
    if (D2GFX_CheckUnitPerspective() != 0)
    {
        SRegSaveValue("Diablo II", "Perspective", 0, (uint32_t)pItem->nCurrentState);
        D2GFX_SetPerspective(pItem->nCurrentState);
        VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), 0, 0, gnViewportWidth, gnViewportHeight);
    }
    else
    {
        pItem->nCurrentState = 0;
    }
}

// 1.12a: 0x6FB04F40
void __cdecl MINIPANEL_Render(void)
{
    const int32_t bLeftBlocked  = MINIPANEL_CheckLeftSideObstructed();
    const int32_t bRightBlocked = MINIPANEL_CheckRightSideObstructed();

    if (bLeftBlocked != 0)
    {
        if (bRightBlocked != 0)
        {
            g_bMiniPanelHiddenByPanels = 1;
            return;
        }

        D2GfxDataStrc tGfx = {};
        tGfx.pCellFile = g_pMiniPanelBgCel;
        const int32_t nHalfW = gnScreenWidth / 2;
        const int32_t nX = ((g_bMiniPanelSinglePlayer != 0) ? (nHalfW + 0x38) : (nHalfW + 0x23)) - 2;
        TEXTURE_CelDraw(&tGfx, nX, gnScreenHeight - 0x2F, 0xFFFFFFFFu, 5, nullptr);

        MINIPANEL_UpdateButtonLayout(3);
        MINIPANEL_DrawButtons(1);
        MINIPANEL_RenderHoveredTooltip(3);
        g_bMiniPanelHiddenByPanels = 0;
        return;
    }

    if (bRightBlocked == 0)
    {
        D2GfxDataStrc tGfx = {};
        tGfx.pCellFile = g_pMiniPanelBgCel;
        const int32_t nHalfW = gnScreenWidth / 2;
        const int32_t nX = ((g_bMiniPanelSinglePlayer != 0) ? (nHalfW - 0x4A) : (nHalfW - 0x54)) - 3;
        TEXTURE_CelDraw(&tGfx, nX, gnScreenHeight - 0x2F, 0xFFFFFFFFu, 5, nullptr);

        MINIPANEL_UpdateButtonLayout(2);
        MINIPANEL_DrawButtons(1);
        MINIPANEL_RenderHoveredTooltip(2);
        g_bMiniPanelHiddenByPanels = 0;
        return;
    }

    D2GfxDataStrc tGfx = {};
    tGfx.pCellFile = g_pMiniPanelBgCel;
    TEXTURE_CelDraw(&tGfx, (gnScreenWidth / 2) - 0xCD, gnScreenHeight - 0x2F, 0xFFFFFFFFu, 5, nullptr);

    MINIPANEL_UpdateButtonLayout(1);
    MINIPANEL_DrawButtons(1);
    MINIPANEL_RenderHoveredTooltip(1);
    g_bMiniPanelHiddenByPanels = 0;
}

// 1.12a: 0x6FB050E0 (with jump table at 0x6FB05428)
void __cdecl OPTIONS_RenderEscMenu(void)
{
    if (g_pActiveOptionsHeader == nullptr)
    {
        OPTIONS_HaltAtLine(0x6BE);
    }
    if (g_pActiveOptionsItems == nullptr)
    {
        OPTIONS_HaltAtLine(0x6BF);
    }

    const int32_t nHalfMenuH = (g_pActiveOptionsHeader->nItemCount * g_pActiveOptionsHeader->nLineStepY) / 2;
    const int32_t nMenuTopY  = ((gnScreenHeight - 0x50) / 2) - nHalfMenuH;

    if (g_nLastOptionsMouseY != gnMouseY || g_nLastOptionsMouseX != gnMouseX)
    {
        g_nLastOptionsMouseY = gnMouseY;
        g_nLastOptionsMouseX = gnMouseX;
        if (g_bOptionsMouseButtonDown != 0)
        {
            OPTIONS_UpdateSliderDragAtMouse(gnMouseY, 0, gnMouseX);
        }
        else
        {
            const int32_t nHitIdx = OPTIONS_HitTestMenuRowY(gnMouseY);
            if (nHitIdx != -1)
            {
                g_nSelectedOptionsItemIdx = nHitIdx;
            }
        }
    }

    int32_t nVisibleRow = 0;
    for (int32_t i = 0; i < g_pActiveOptionsHeader->nItemCount; ++i)
    {
        D2OptionsMenuItemStrc& rItem = g_pActiveOptionsItems[i];
        rItem.nRenderedBaseY = 0;
        if (rItem.bExpansionOnly != 0 && FOG_IsExpansion() == 0)
        {
            continue;
        }

        const int32_t bEnabled  = (rItem.pfnCanUse == nullptr || rItem.pfnCanUse() != 0) ? 1 : 0;
        const int32_t nDrawMode = (bEnabled * 4) + 1;
        if (rItem.pfnOnUpdate != nullptr)
        {
            rItem.pfnOnUpdate(&rItem);
        }

        const int32_t nItemBaseY = nMenuTopY + (nVisibleRow * g_pActiveOptionsHeader->nLineStepY);
        const int32_t nTextDrawY = nItemBaseY + g_pActiveOptionsHeader->nTextOffsetY;
        rItem.nRenderedBaseY = nItemBaseY;

        switch (rItem.nItemType)
        {
        case -1:
        case 0:
            D2Win_10134_DrawCellFile(rItem.pLabelCel, gnScreenWidth / 2, nTextDrawY, 1, nDrawMode, -1);
            break;

        case 1:
            D2Win_10134_DrawCellFile(rItem.pLabelCel, (gnScreenWidth / 2) - 0xE6, nTextDrawY, 0, nDrawMode, -1);
            D2Win_10134_DrawCellFile(rItem.apChoiceCels[rItem.nCurrentState], (gnScreenWidth / 2) + 0xE6, nTextDrawY, 2, nDrawMode, -1);
            break;

        case 2:
            if (rItem.pLabelCel != nullptr)
            {
                D2Win_10134_DrawCellFile(rItem.pLabelCel, (gnScreenWidth / 2) - 0xE6, nTextDrawY, 0, nDrawMode, -1);
            }
            OPTIONS_DrawSliderBar(nItemBaseY, 0, &rItem, nDrawMode, (rItem.pLabelCel == nullptr) ? 1 : 0);
            break;

        default:
            break;
        }

        ++nVisibleRow;
    }

    D2GfxDataStrc tPentGfx = {};
    tPentGfx.pCellFile = g_pPentSpinCel;
    tPentGfx.nFrame    = (uint32_t)((g_nOptionsPentagramFrame != 0) ? (8 - g_nOptionsPentagramFrame) : 0);

    const int32_t nPentY = g_pActiveOptionsItems[g_nSelectedOptionsItemIdx].nRenderedBaseY + g_pActiveOptionsHeader->nPentagramOffsetY;
    const int32_t nHalfW = gnScreenWidth / 2;
    TEXTURE_CelDraw(&tPentGfx, nHalfW - g_nMaxPentagramWidth - 0xF9, nPentY, 0xFFFFFFFFu, 5, nullptr);

    tPentGfx.nFrame = (uint32_t)g_nOptionsPentagramFrame;
    TEXTURE_CelDraw(&tPentGfx, nHalfW + 0xF9, nPentY, 0xFFFFFFFFu, 5, nullptr);

    const DWORD dwNow = GetTickCount();
    if (dwNow - g_dwLastOptionsPentagramTick > 50u)
    {
        g_dwLastOptionsPentagramTick = dwNow;
        g_nOptionsPentagramFrame = (g_nOptionsPentagramFrame + 1) & 7;
    }
}

// 1.12a: 0x6FB05440
void __stdcall OPTIONS_OnReturnKey(D2WinMsgStrc* pMsg)
{
    OPTIONS_ActivateSelectedItem(pMsg);
    pMsg->bHandled = 1;
    pMsg->lResult  = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB05470
void __stdcall OPTIONS_OnLButtonUp(D2WinMsgStrc* pMsg)
{
    if (g_bOptionsMouseButtonDown != 0)
    {
        const int32_t nHitIdx = OPTIONS_HitTestMenuRowY((int32_t)pMsg->nY);
        if (nHitIdx == g_nSelectedOptionsItemIdx)
        {
            OPTIONS_ActivateSelectedItem(pMsg);
        }
    }

    g_nMouseRepeatDelayCounterA = 16;
    g_nMouseRepeatDelayCounterB = 16;
    g_bLeftMouseHeld            = 0;
    g_bLeftMouseRepeating       = 0;
    g_bRightMouseHeld           = 0;
    g_bRightMouseRepeating      = 0;
    gnTooltipUnitGUID           = 0;
    gnTooltipUnitType           = 0;
    g_bOptionsMouseButtonDown   = 0;
    g_bOptionsSliderDragging    = 0;

    pMsg->bHandled = 1;
    pMsg->lResult  = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB054F0
void __stdcall OPTIONS_OnLButtonDown(D2WinMsgStrc* pMsg)
{
    const int32_t nHitIdx = OPTIONS_HitTestMenuRowY((int32_t)pMsg->nY);
    if (nHitIdx != -1)
    {
        g_nSelectedOptionsItemIdx = nHitIdx;
    }

    OPTIONS_UpdateSliderDragAtMouse((int32_t)pMsg->nY, 0, (int32_t)pMsg->nX);
    g_bOptionsMouseButtonDown = 1;

    pMsg->bHandled = 1;
    pMsg->lResult  = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB05540
void __fastcall OPTIONS_OnChangeEAX(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pItem;
    (void)pMsg;
    SOUNDCHANNEL_ToggleEAX();
}

// 1.12a: 0x6FB05550
void __fastcall OPTIONS_OnChange3DSound(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pItem;
    (void)pMsg;
    SOUNDCHANNEL_Toggle3DSound();
}

// 1.12a: 0x6FB05560
void __stdcall UI_CloseEscOptionsMenu(int32_t bRestorePanels)
{
    const int32_t nPrevEscState = gaPanelStates[D2PANEL_ESCMENU];
    if (nPrevEscState != 0)
    {
        FOG_IsExpansion();
        if (gnPanelCount != D2PANEL_COUNT)
        {
            OPTIONS_HaltAtLine(0x1389);
        }

        gaPanelStates[D2PANEL_ESCMENU] = 0;
        WALL2_UnregisterMsgHandlerTable(7, WINDOW_GetWindow(), s_aEscOptionsMsgHandlers);
        gnTooltipUnitGUID = 0;
        gnTooltipUnitType = 0;
    }

    if (bRestorePanels != 0)
    {
        for (int32_t i = 0; i < D2PANEL_COUNT; ++i)
        {
            if (g_aEscPanelSaveTable[i].bSavedState != 0)
            {
                UI_SetPanelState(i, 0, 0);
            }
        }
    }
}

static void OPTIONS_ApplyPostCloseViewportAdjustment(int32_t nClosedPanelId)
{
    switch (nClosedPanelId)
    {
    case 1:  // D2PANEL_INVENTORY (clears hovered item first, then checks left panels)
        INV_ClearHoveredItemState();
        // fall through to case 4
    case 4:  // D2PANEL_SKILLTREE
        if (gaPanelStates[D2PANEL_CHAR] == 0 &&
            gaPanelStates[20] == 0 &&
            gaPanelStates[D2PANEL_QUEST] == 0 &&
            gaPanelStates[D2PANEL_WAYPOINT] == 0 &&
            gaPanelStates[D2PANEL_MERCINV] == 0 &&
            gaPanelStates[37] == 0 &&
            gaPanelStates[D2PANEL_PARTY] == 0)
        {
            gnViewportShiftOffsetX = 0;
            gnViewportShiftMode    = 0;
            D2GFX_SetViewMatrixAdjust(0);
            VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), 0, 0, gnViewportWidth, gnViewportHeight);
        }
        else
        {
            const int32_t nShiftScreen = gnScreenWidth / 4;
            gnViewportShiftOffsetX = nShiftScreen;
            gnViewportShiftMode    = 2;
            D2GFX_SetViewMatrixAdjust(nShiftScreen);
            const int32_t nShiftView = gnViewportWidth / 4;
            VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), nShiftView, 0, gnViewportWidth + nShiftView, gnViewportHeight);
        }
        break;

    case 2:  // D2PANEL_CHAR
    case 15: // D2PANEL_QUEST
    case 16: // D2PANEL_WAYPOINT
    case 20:
    case 22: // D2PANEL_PARTY
    case 36: // D2PANEL_MERCINV
    case 37:
        if (gaPanelStates[D2PANEL_INVENTORY] == 0 && gaPanelStates[D2PANEL_SKILLTREE] == 0)
        {
            gnViewportShiftOffsetX = 0;
            gnViewportShiftMode    = 0;
            D2GFX_SetViewMatrixAdjust(0);
            VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), 0, 0, gnViewportWidth, gnViewportHeight);
        }
        else
        {
            const int32_t nShiftScreen = -(gnScreenWidth / 4);
            gnViewportShiftOffsetX = nShiftScreen;
            gnViewportShiftMode    = 1;
            D2GFX_SetViewMatrixAdjust(nShiftScreen);
            const int32_t nShiftView = gnViewportWidth / 4;
            VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), -nShiftView, 0, gnViewportWidth - nShiftView, gnViewportHeight);
        }
        break;

    case 11: // D2PANEL_CFGCONTROLS
        TEXT_ClearChatAndCfgControls();
        break;

    case 12: // D2PANEL_NPCSHOP
    case 14: // D2PANEL_MODITEM
    case 25: // D2PANEL_STASH
    case 26: // D2PANEL_CUBE
    case 27:
    case 28:
    case 29:
    case 32:
        gnViewportShiftOffsetX = 0;
        gnViewportShiftMode    = 0;
        D2GFX_SetViewMatrixAdjust(0);
        VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), 0, 0, gnViewportWidth, gnViewportHeight);
        break;

    default:
        break;
    }
}

// 1.12a: 0x6FB05610 (with jump tables at 0x6FB05A90, 0x6FB05AA8, 0x6FB05AD0, 0x6FB05AE4)
void __fastcall UI_OpenEscOptionsMenu(int32_t bSaveAndClosePanels, int32_t nMenuPage)
{
    if (bSaveAndClosePanels != 0)
    {
        for (int32_t i = 0; i < D2PANEL_COUNT; ++i)
        {
            g_aEscPanelSaveTable[i].bSavedState = 0;
            if (g_aEscPanelSaveTable[i].bCloseOnEsc == 0)
            {
                continue;
            }
            if (g_aEscPanelSaveTable[i].bRestoreOnReturn != 0)
            {
                g_aEscPanelSaveTable[i].bSavedState = gaPanelStates[i];
            }

            const int32_t nWasOpen = gaPanelStates[i];
            if (nWasOpen == 0)
            {
                continue;
            }

            if ((FOG_IsExpansion() == 0 || gbIsExpansionClient == 0) &&
                (i == 35 || i == D2PANEL_MERCINV || i == 37))
            {
                continue;
            }

            if (UI_PreparePanelOpen(1, i) == 0)
            {
                continue;
            }

            gaPanelStates[i] = 0;
            if (nWasOpen != 0)
            {
                UI_OnPanelClosed(i);
            }
            OPTIONS_ApplyPostCloseViewportAdjustment(i);
        }
    }

    const int32_t nPrevEscOpen = gaPanelStates[D2PANEL_ESCMENU];
    FOG_IsExpansion();
    if (gnPanelCount != D2PANEL_COUNT)
    {
        OPTIONS_HaltAtLine(0x1389);
    }

    if (g_dwPanelCloseCooldownTick <= GetTickCount() && gpPlayerUnit != nullptr)
    {
        if (gpPlayerUnit->dwUnitType == UNIT_PLAYER_TYPE && gpPlayerUnit->dwAnimMode == PLRMODE_DEAD_CONST)
        {
            UNIT_FreeUnitGfxData(gpPlayerUnit);
            MSG_SendByte(0x41);
            if ((g_pGameData->wCharFlags & 0x04u) != 0)
            {
                MSG_SendLeaveGame_69();
                g_pGameData->wCharFlags |= 0x08u;
                gbActLoadedFlag    = 0;
                g_pClientPoolSystem = nullptr;
                g_bExitAppFlag     = 0;
                g_bExitGameFlag    = 1;
            }
        }
        else if (gbGoldDialogOpen == 0)
        {
            bool bBlocked = false;
            for (int32_t i = 0; i < gnPanelCount; ++i)
            {
                if (gaPanelStates[i] == 1)
                {
                    const int32_t nRule = g_apUiPanelTable[i]->aActionOnOpen[D2PANEL_ESCMENU];
                    if (nRule == 1)
                    {
                        UI_SetPanelState(i, 1, 0);
                    }
                    else if (nRule == 4)
                    {
                        if (g_bInteractedWithNpc != 0)
                        {
                            NPCMENU_CloseActiveMenu();
                        }
                    }
                    else if (nRule != 0)
                    {
                        bBlocked = true;
                        break;
                    }
                }
            }

            if (!bBlocked)
            {
                if (gpPlayerUnit == nullptr ||
                    (gpPlayerUnit->dwUnitType == UNIT_PLAYER_TYPE && CUNIT_HasActiveInteract(gpPlayerUnit) == 0))
                {
                    gaPanelStates[D2PANEL_ESCMENU] = 1;
                }

                const int32_t nNewEscOpen = gaPanelStates[D2PANEL_ESCMENU];
                if (nPrevEscOpen == 0)
                {
                    if (nNewEscOpen != 0)
                    {
                        WALL2_RegisterMsgHandlerTable(7, WINDOW_GetWindow(), s_aEscOptionsMsgHandlers);
                        gnTooltipUnitGUID = 1;
                        gnTooltipUnitType = 0;
                    }
                }
                else if (nNewEscOpen == 0)
                {
                    WALL2_UnregisterMsgHandlerTable(7, WINDOW_GetWindow(), s_aEscOptionsMsgHandlers);
                    gnTooltipUnitGUID = 0;
                    gnTooltipUnitType = 0;
                }
            }
        }
    }

    switch (nMenuPage)
    {
    case 0:
        g_pActiveOptionsHeader = &g_tMainEscMenuHeader;
        g_pActiveOptionsItems  = g_aMainEscMenuItems;
        break;
    case 1:
        g_pActiveOptionsHeader = &g_tOptionsSubMenuHeader;
        g_pActiveOptionsItems  = g_aOptionsSubMenuItems;
        break;
    case 2:
        g_pActiveOptionsHeader = &g_tSoundMenuHeader;
        g_pActiveOptionsItems  = g_aSoundMenuItems;
        break;
    case 3:
        if (FOG_IsExpansion() != 0)
        {
            g_pActiveOptionsHeader = &g_tVideoMenuExpHeader;
            g_pActiveOptionsItems  = g_aVideoMenuExpItems;
        }
        else
        {
            g_pActiveOptionsHeader = &g_tVideoMenuClassicHeader;
            g_pActiveOptionsItems  = g_aVideoMenuClassicItems;
        }
        break;
    case 4:
        if (FOG_IsExpansion() != 0)
        {
            g_pActiveOptionsHeader = &g_tAutomapMenuExpHeader;
            g_pActiveOptionsItems  = g_aAutomapMenuExpItems;
        }
        else
        {
            g_pActiveOptionsHeader = &g_tAutomapMenuClassicHeader;
            g_pActiveOptionsItems  = g_aAutomapMenuClassicItems;
        }
        break;
    default:
        break;
    }

    g_nSelectedOptionsItemIdx = g_pActiveOptionsHeader->nItemCount - 1;
    if (nMenuPage == 5)
    {
        g_nSelectedOptionsItemIdx = 1;
    }
}

// 1.12a: 0x6FB05B00
void __fastcall OPTIONS_OnSelectSubMenuPrevious(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pItem;
    (void)pMsg;
    UI_OpenEscOptionsMenu(0, 1);
}

// 1.12a: 0x6FB05B10
void __fastcall OPTIONS_OnSelectOptionsPrevious(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pItem;
    (void)pMsg;
    UI_OpenEscOptionsMenu(0, 0);
}

// 1.12a: 0x6FB05B20 (with jump table at 0x6FB05C5C)
void __fastcall OPTIONS_OnSelectConfigureControls(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pItem;
    (void)pMsg;
    UI_CloseEscOptionsMenu(0);

    const int32_t nPrevCfgOpen = gaPanelStates[D2PANEL_CFGCONTROLS];
    FOG_IsExpansion();
    if (gnPanelCount != D2PANEL_COUNT)
    {
        OPTIONS_HaltAtLine(0x1389);
    }

    if (gbGoldDialogOpen != 0)
    {
        return;
    }

    for (int32_t i = 0; i < gnPanelCount; ++i)
    {
        if (gaPanelStates[i] == 1)
        {
            const int32_t nRule = g_apUiPanelTable[i]->aActionOnOpen[D2PANEL_CFGCONTROLS];
            if (nRule == 1)
            {
                UI_SetPanelState(i, 1, 0);
            }
            else if (nRule == 4)
            {
                if (g_bInteractedWithNpc != 0)
                {
                    NPCMENU_CloseActiveMenu();
                }
            }
            else if (nRule != 0)
            {
                return;
            }
        }
    }

    if (gpPlayerUnit == nullptr ||
        (gpPlayerUnit->dwUnitType == UNIT_PLAYER_TYPE && CUNIT_HasActiveInteract(gpPlayerUnit) == 0))
    {
        gaPanelStates[D2PANEL_CFGCONTROLS] = 1;
    }

    const int32_t nNewCfgOpen = gaPanelStates[D2PANEL_CFGCONTROLS];
    if (nPrevCfgOpen == 0)
    {
        if (nNewCfgOpen == 0)
        {
            TEXT_ClearChatAndCfgControls();
            return;
        }
        CFGCONTROLS_SetOpenState(TRUE);
        UI_RegisterDefaultWindowHandlers();
    }
    else
    {
        if (nNewCfgOpen != 0)
        {
            return;
        }
        if (GAME_HasMouseWheelB() != 0 || GAME_HasMouseWheelA() != 0)
        {
            WALL2_UnregisterMsgHandlerTable(1, WINDOW_GetWindow(), g_aCfgControlsWheelHandlers);
        }
        WALL2_UnregisterMsgHandlerTable(13, WINDOW_GetWindow(), g_aCfgControlsMsgHandlers);
        CFGCONTROLS_SetOpenState(FALSE);
    }

    if (nNewCfgOpen == 0)
    {
        TEXT_ClearChatAndCfgControls();
    }
}

// 1.12a: 0x6FB05C70
void __fastcall OPTIONS_OnSelectAutomapOptions(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pItem;
    (void)pMsg;
    UI_OpenEscOptionsMenu(0, 4);
}

// 1.12a: 0x6FB05C80
void __fastcall OPTIONS_OnSelectVideoOptions(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pItem;
    (void)pMsg;
    UI_OpenEscOptionsMenu(0, 3);
}

// 1.12a: 0x6FB05C90
void __fastcall OPTIONS_OnSelectSoundOptions(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pItem;
    (void)pMsg;
    UI_OpenEscOptionsMenu(0, 2);
}

// 1.12a: 0x6FB05CA0
void __fastcall OPTIONS_OnSelectReturnToGame(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pItem;
    (void)pMsg;
    UI_CloseEscOptionsMenu(1);
}

// 1.12a: 0x6FB05CB0
void __fastcall OPTIONS_OnSelectSaveAndExitGame(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pItem;
    const HWND hWnd = pMsg ? pMsg->hWnd : WINDOW_GetWindow();
    g_bExitAppFlag = 0;
    SendMessageA(hWnd, WM_CLOSE, 0, 0);
    g_nRightMouseHoldY = 1;
    UI_CloseEscOptionsMenu(1);
}

// 1.12a: 0x6FB05CE0
void __fastcall OPTIONS_OnSelectOptionsSubMenu(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pItem;
    (void)pMsg;
    UI_OpenEscOptionsMenu(0, 1);
}

// 1.12a: 0x6FB05CF0
void __cdecl OPTIONS_ApplyRegistryResolution(void)
{
    int32_t nRes = 0;
    SRegLoadValue("Diablo II", "Resolution", 0, &nRes);
    GAME_SetResolutionMode((nRes == 1) ? 2 : 0);
}

// 1.12a: 0x6FB05D20 (with jump tables at 0x6FB060E0, 0x6FB06100)
void __fastcall MINIPANEL_ExecuteButtonAction(int32_t nBtnAction)
{
    if ((uint32_t)nBtnAction > 7u)
    {
        return;
    }

    switch (nBtnAction)
    {
    case 0: // Toggle Character Screen
    {
        int32_t nCharOpen = gaPanelStates[D2PANEL_CHAR];
        FOG_IsExpansion();
        if (nCharOpen != 0)
        {
            if (gnPanelCount != D2PANEL_COUNT)
            {
                OPTIONS_HaltAtLine(0x1389);
            }
            gaPanelStates[D2PANEL_CHAR] = 0;
            UI_CloseGoldDialog();
            WALL2_UnregisterMsgHandlerTable(2, WINDOW_GetWindow(), g_aCharStatMsgHandlers);
            UI_StartPanelCloseClickCooldown();

            if (gaPanelStates[D2PANEL_INVENTORY] == 0 && gaPanelStates[D2PANEL_SKILLTREE] == 0)
            {
                gnViewportShiftOffsetX = 0;
                gnViewportShiftMode    = 0;
                D2GFX_SetViewMatrixAdjust(0);
                VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), 0, 0, gnViewportWidth, gnViewportHeight);
            }
            else
            {
                const int32_t nShiftScreen = -(gnScreenWidth / 4);
                gnViewportShiftOffsetX = nShiftScreen;
                gnViewportShiftMode    = 1;
                D2GFX_SetViewMatrixAdjust(nShiftScreen);
                const int32_t nShiftView = gnViewportWidth / 4;
                VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), -nShiftView, 0, gnViewportWidth - nShiftView, gnViewportHeight);
            }
            return;
        }

        if (gnPanelCount != D2PANEL_COUNT)
        {
            OPTIONS_HaltAtLine(0x1389);
        }

        nCharOpen = gaPanelStates[D2PANEL_CHAR];
        if (nCharOpen == 0)
        {
            if (gbGoldDialogOpen != 0)
            {
                return;
            }
            for (int32_t i = 0; i < gnPanelCount; ++i)
            {
                if (gaPanelStates[i] == 1)
                {
                    const int32_t nRule = g_apUiPanelTable[i]->aActionOnOpen[D2PANEL_CHAR];
                    if (nRule == 1)
                    {
                        UI_SetPanelState(i, 1, 0);
                    }
                    else if (nRule == 4)
                    {
                        if (g_bInteractedWithNpc != 0)
                        {
                            NPCMENU_CloseActiveMenu();
                        }
                    }
                    else if (nRule != 0)
                    {
                        return;
                    }
                }
            }
            nCharOpen = gaPanelStates[D2PANEL_CHAR];
        }

        if (gpPlayerUnit == nullptr ||
            (gpPlayerUnit->dwUnitType == UNIT_PLAYER_TYPE && CUNIT_HasActiveInteract(gpPlayerUnit) == 0))
        {
            nCharOpen = (nCharOpen == 0) ? 1 : 0;
            gaPanelStates[D2PANEL_CHAR] = nCharOpen;
        }

        if (nCharOpen != 0)
        {
            WALL2_RegisterMsgHandlerTable(2, WINDOW_GetWindow(), g_aCharStatMsgHandlers);
        }

        if (gaPanelStates[D2PANEL_INVENTORY] == 0 && gaPanelStates[D2PANEL_SKILLTREE] == 0)
        {
            if (nCharOpen != 0)
            {
                const int32_t nShiftScreen = gnScreenWidth / 4;
                gnViewportShiftOffsetX = nShiftScreen;
                gnViewportShiftMode    = 2;
                D2GFX_SetViewMatrixAdjust(nShiftScreen);
                const int32_t nShiftView = gnViewportWidth / 4;
                VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), nShiftView, 0, gnViewportWidth + nShiftView, gnViewportHeight);
            }
            else
            {
                gnViewportShiftOffsetX = 0;
                gnViewportShiftMode    = 0;
                D2GFX_SetViewMatrixAdjust(0);
                VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), 0, 0, gnViewportWidth, gnViewportHeight);
            }
        }
        else
        {
            if (nCharOpen != 0)
            {
                gnViewportShiftOffsetX = 0;
                gnViewportShiftMode    = 3;
                D2GFX_SetViewMatrixAdjust(0);
                VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), 0, 0, gnViewportWidth, gnViewportHeight);
                g_bLeftMouseHeld       = 0;
                g_bLeftMouseRepeating  = 0;
                g_bRightMouseHeld      = 0;
                g_bRightMouseRepeating = 0;
            }
            else
            {
                const int32_t nShiftScreen = -(gnScreenWidth / 4);
                gnViewportShiftOffsetX = nShiftScreen;
                gnViewportShiftMode    = 1;
                D2GFX_SetViewMatrixAdjust(nShiftScreen);
                const int32_t nShiftView = gnViewportWidth / 4;
                VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), -nShiftView, 0, gnViewportWidth - nShiftView, gnViewportHeight);
            }
        }
        break;
    }

    case 1: // Toggle Inventory
        UI_SetPanelState(D2PANEL_INVENTORY, (gaPanelStates[D2PANEL_INVENTORY] != 0) ? 1 : 2, 0);
        break;

    case 2: // Toggle Skill Tree
        UI_SetPanelState(D2PANEL_SKILLTREE, (gaPanelStates[D2PANEL_SKILLTREE] != 0) ? 1 : 2, 0);
        break;

    case 3: // Toggle Party Screen (Multiplayer only)
        if (gnGameType != 0)
        {
            UI_SetPanelState(D2PANEL_PARTY, (gaPanelStates[D2PANEL_PARTY] != 0) ? 1 : 2, 0);
        }
        break;

    case 4: // Toggle Automap
        UI_SetPanelState(D2PANEL_AUTOMAP, (gaPanelStates[D2PANEL_AUTOMAP] != 0) ? 1 : 2, 0);
        break;

    case 5: // Toggle Message Log
        if (gaPanelStates[D2PANEL_BELT] != 0)
        {
            UI_SetPanelState(D2PANEL_BELT, 1, 0);
        }
        UI_SetPanelState(D2PANEL_MSGLOG, (gaPanelStates[D2PANEL_MSGLOG] != 0) ? 1 : 2, 0);
        break;

    case 6: // Toggle Quest Log
        OutputDebugStringA("Panel quest log toggle\n");
        QUESTLOG_TogglePanel(0);
        break;

    case 7: // Open Esc Options Menu
        UI_CloseAllPanels();
        g_bAllowPanelToggleSound   = 0;
        g_dwPanelCloseCooldownTick = 0;
        UI_OpenEscOptionsMenu(1, 0);
        g_bAllowPanelToggleSound   = 1;
        UI_StartPanelCloseClickCooldown();
        break;

    default:
        break;
    }
}

// 1.12a: 0x6FB06120
void __fastcall OPTIONS_InitResolution(D2OptionsMenuItemStrc* pItem)
{
    SRegLoadValue("Diablo II", "Resolution", 0, &pItem->nCurrentState);
    GAME_SetResolutionMode((pItem->nCurrentState == 1) ? 2 : 0);
}

// 1.12a: 0x6FB06150
void __fastcall OPTIONS_OnChangeResolution(D2OptionsMenuItemStrc* pItem, D2WinMsgStrc* pMsg)
{
    (void)pMsg;
    SRegSaveValue("Diablo II", "Resolution", 0, (uint32_t)pItem->nCurrentState);
    GAME_SetResolutionMode((pItem->nCurrentState == 1) ? 2 : 0);
}

// 1.12a: 0x6FB06190
void __stdcall MINIPANEL_OnLButtonUp(D2WinMsgStrc* pMsg)
{
    if (g_bMiniPanelButtonDown == 0)
    {
        return;
    }
    g_bMiniPanelButtonDown = 0;

    if (gpPlayerUnit == nullptr ||
        gpPlayerUnit->pInventory == nullptr ||
        INVENTORY_GetCursorItem(gpPlayerUnit->pInventory) != nullptr)
    {
        return;
    }

    if (gnCursorState != 7)
    {
        gpCursorLockedItem = nullptr;
        gnCursorState      = 1;
        gnCursorAnimId     = 5;
        gnCursorFrameFixed8 = 0;
    }

    const int32_t nBtnCount = (g_bMiniPanelSinglePlayer != 0) ? 7 : 8;
    if (g_bMiniPanelHiddenByPanels != 0)
    {
        g_bMiniPanelTogglePressed = 0;
        return;
    }

    const int32_t nMouseX = (int32_t)pMsg->nX;
    const int32_t nMouseY = (int32_t)pMsg->nY;
    for (int32_t i = 0; i < nBtnCount; ++i)
    {
        g_aMiniPanelBtnPressed[i] = 0;
    }

    int32_t nShiftX = 0;
    if (g_bWeaponSwapActive != 0 && CTRLPNL_GetEquippedBeltRows() > 1)
    {
        nShiftX = -0x76;
    }
    if (g_bMiniPanelShiftLeftForRightPanel != 0)
    {
        nShiftX -= 0x76;
    }
    if (g_bMiniPanelShiftRightForLeftPanel != 0)
    {
        nShiftX += 0x77;
    }

    const int32_t nCenterX = (gnScreenWidth / 2) + nShiftX;
    if (nMouseX <= nCenterX - 0x54 ||
        nMouseX >= nCenterX + 0x59 ||
        nMouseY <= gnScreenHeight - 0x45 ||
        nMouseY >= gnScreenHeight - 0x2F)
    {
        pMsg->bHandled = 0;
        g_bMiniPanelTogglePressed = 0;
        return;
    }

    for (int32_t i = 0; i < nBtnCount; ++i)
    {
        if (nMouseX > g_aMiniPanelBtnX[i] && nMouseX < g_aMiniPanelBtnX[i] + 20)
        {
            const int32_t nAction = g_aMiniPanelBtnAction[i];
            if (nAction == 7 ||
                (gpPlayerUnit != nullptr &&
                 gpPlayerUnit->dwUnitType == UNIT_PLAYER_TYPE &&
                 gpPlayerUnit->dwAnimMode != PLRMODE_DEAD_CONST))
            {
                MINIPANEL_ExecuteButtonAction(nAction);
            }
        }
    }

    g_bLeftMouseHeld       = 0;
    g_bLeftMouseRepeating  = 0;
    g_bRightMouseHeld      = 0;
    g_bRightMouseRepeating = 0;
    pMsg->bHandled         = 1;
    pMsg->lResult          = 0;
    SMsgBreakHandlerChain(pMsg);
}

// ============================================================================
// PART 2: UI\plrtrade.cpp — Player Trade Session & Trade Prompt Dialogs
//         (0x6FB06350..0x6FB0744F)
// ============================================================================

// 1.12a: 0x6FB06350
int32_t __cdecl PLRTRADE_GetLocalGoldOffer(void)
{
    return g_nTradePartnerGold;
}

// 1.12a: 0x6FB06360
int32_t __cdecl PLRTRADE_GetPartnerGoldOffer(void)
{
    return g_nTradePlayerGold;
}

// 1.12a: 0x6FB06370
void __fastcall PLRTRADE_SetGoldOffer(uint8_t bIsLocalOffer, int32_t nUnusedEdx, int32_t nGoldAmount)
{
    (void)nUnusedEdx;
    if (bIsLocalOffer != 0)
    {
        g_nTradePartnerGold = nGoldAmount;
    }
    else
    {
        g_nTradePlayerGold = nGoldAmount;
    }
}

// 1.12a: 0x6FB06380
Unicode* __cdecl PLRTRADE_GetPartnerName(void)
{
    return g_wszTradePartnerName;
}

// 1.12a: 0x6FB06390
void __fastcall PLRTRADE_OnRecvPartnerInfoPacket(const D2SrvTradePartnerPacketStrc* pPacket)
{
    char szPartnerName[16] = {};
    memcpy(szPartnerName, pPacket->szPartnerName, sizeof(szPartnerName));
    szPartnerName[15] = '\0';

    D2LANG_Win2Unicode(g_wszTradePartnerName, szPartnerName, 16);
    g_dwTradePartnerGUID = pPacket->dwPartnerGUID;
}

// 1.12a: 0x6FB063E0
int32_t __cdecl PLRTRADE_IsPromptDialogActive(void)
{
    return (g_nPlayerTradeState == 1 || g_nPlayerTradeState == 2) ? 1 : 0;
}

// 1.12a: 0x6FB06400
int32_t __cdecl PLRTRADE_IsAcceptCheckAllowed(void)
{
    return (g_bTradeAcceptBlocked == 0) ? 1 : 0;
}

// 1.12a: 0x6FB06410
void __cdecl PLRTRADE_BlockAcceptAndClearGold(void)
{
    g_nTradePlayerGold    = 0;
    g_nTradePartnerGold   = 0;
    g_bTradeAcceptBlocked = 1;
}

// 1.12a: 0x6FB06430
void __cdecl PLRTRADE_ResetTradeState(void)
{
    g_nPlayerTradeState      = 0;
    g_bTradeCancelPacketSent = 0;
}

// 1.12a: 0x6FB06440
void __cdecl PLRTRADE_SendUncheckAcceptPacket(void)
{
    D2CltTradeButtonPacketStrc tPkt = {};
    tPkt.bOpcode = 0x4F;
    tPkt.wSubCmd = 7;
    MSG_SendPacket(&tPkt, sizeof(tPkt));
    g_nPlayerTradeState = 3;
}

// 1.12a: 0x6FB06480
void __cdecl PLRTRADE_SendCheckAcceptPacket(void)
{
    const uint32_t dwGold = (uint32_t)g_nTradePartnerGold;
    g_nPlayerTradeState = 7;

    D2CltTradeButtonPacketStrc tPkt = {};
    tPkt.bOpcode   = 0x4F;
    tPkt.wSubCmd   = 4;
    tPkt.wGoldHigh = (uint16_t)(dwGold >> 16);
    tPkt.wGoldLow  = (uint16_t)(dwGold & 0xFFFFu);
    MSG_SendPacket(&tPkt, sizeof(tPkt));

    g_dwTradeCancelTimeoutStartTick = 0xFFFFFFFFu;
}

// 1.12a: 0x6FB064D0
void __cdecl PLRTRADE_SendCancelTradePacket(void)
{
    D2CltTradeButtonPacketStrc tPkt = {};
    tPkt.bOpcode = 0x4F;
    tPkt.wSubCmd = 2;
    MSG_SendPacket(&tPkt, sizeof(tPkt));

    if (g_nPlayerTradeState != 7)
    {
        g_bTradeCancelPacketSent        = 1;
        g_dwTradeCancelTimeoutStartTick = GetTickCount();
        g_nPlayerTradeState             = 7;
    }
}

// 1.12a: 0x6FB06520
void __fastcall PLRTRADE_SubmitGoldOffer(uint32_t dwRequestedGold)
{
    if (g_nPlayerTradeState == 4 || g_nPlayerTradeState == 5 || g_nPlayerTradeState == 6)
    {
        D2CltTradeButtonPacketStrc tPkt = {};
        tPkt.bOpcode = 0x4F;
        tPkt.wSubCmd = 7;
        MSG_SendPacket(&tPkt, sizeof(tPkt));
        g_nPlayerTradeState = 3;
        return;
    }

    const uint32_t dwMaxGold = (uint32_t)STATLIST_UnitGetStatValue(gpPlayerUnit, STAT_GOLD_CONST, 0);
    const uint32_t dwClamped = (dwRequestedGold < dwMaxGold) ? dwRequestedGold : dwMaxGold;
    g_nTradePartnerGold = (int32_t)dwClamped;

    MSG_SendByte3Words(0x4F, 8, (uint16_t)(dwClamped >> 16), (uint16_t)(dwClamped & 0xFFFFu));
}

// 1.12a: 0x6FB065C0
int __cdecl PLRTRADE_OnCancelOutgoingPrompt(void)
{
    SOUND_PlaySoundEffect(2, nullptr, 0, 0, 0);
    DIALOG_CloseActiveDialog();

    g_pOutgoingTradePromptDialog    = nullptr;
    g_nPlayerTradeState             = 7;
    g_dwTradeCancelTimeoutStartTick = GetTickCount();

    D2CltTradeButtonPacketStrc tPkt = {};
    tPkt.bOpcode = 0x4F;
    tPkt.wSubCmd = 2;
    MSG_SendPacket(&tPkt, sizeof(tPkt));
    return 1;
}

// 1.12a: 0x6FB06630
int __cdecl PLRTRADE_OnDeclineIncomingPrompt(void)
{
    SOUND_PlaySoundEffect(2, nullptr, 0, 0, 0);
    DIALOG_CloseActiveDialog();

    g_pIncomingTradePromptDialog = nullptr;
    g_nPlayerTradeState          = 7;

    D2CltTradeButtonPacketStrc tPkt = {};
    tPkt.bOpcode = 0x4F;
    tPkt.wSubCmd = 2;
    MSG_SendPacket(&tPkt, sizeof(tPkt));

    g_dwTradeCancelTimeoutStartTick = GetTickCount();
    return 1;
}

// 1.12a: 0x6FB066A0
int __cdecl PLRTRADE_OnAcceptIncomingPrompt(void)
{
    SOUND_PlaySoundEffect(2, nullptr, 0, 0, 0);
    DIALOG_CloseActiveDialog();

    g_nPlayerTradeState          = 3;
    g_pIncomingTradePromptDialog = nullptr;

    D2CltTradeButtonPacketStrc tPkt = {};
    tPkt.bOpcode = 0x4F;
    tPkt.wSubCmd = 3;
    MSG_SendPacket(&tPkt, sizeof(tPkt));
    return 1;
}

// 1.12a: 0x6FB06700
void __stdcall PLRTRADE_CloseTradeSession(BOOL bReopenInventory)
{
    g_bTradeAcceptBlocked = 0;

    if (g_pOutgoingTradePromptDialog != nullptr)
    {
        DIALOG_CloseActiveDialog();
        g_pOutgoingTradePromptDialog = nullptr;
    }
    if (g_pIncomingTradePromptDialog != nullptr)
    {
        DIALOG_CloseActiveDialog();
        g_pIncomingTradePromptDialog = nullptr;
    }

    if (g_nPlayerTradeState == 7)
    {
        return;
    }

    if (gaPanelStates[D2PANEL_TRADEPROMPT] != 0)
    {
        UI_SetPanelState(D2PANEL_TRADEPROMPT, 1, 0);
        g_bTradeGoldTransferDisabled = 0;
        INV_FreeShopTabCells();

        if (gpPlayerUnit != nullptr &&
            gpPlayerUnit->dwAnimMode != PLRMODE_DEAD_CONST &&
            bReopenInventory != 0)
        {
            UI_SetPanelState(D2PANEL_INVENTORY, 2, 0);
        }

        if (gaPanelStates[D2PANEL_CHAT] != 0)
        {
            UI_SetPanelState(D2PANEL_CHAT, 1, 0);
        }
    }

    if (g_nPlayerTradeState != 0)
    {
        D2CltTradeButtonPacketStrc tPkt = {};
        tPkt.bOpcode = 0x4F;
        tPkt.wSubCmd = 2;
        MSG_SendPacket(&tPkt, sizeof(tPkt));
        g_nPlayerTradeState = 0;
    }
}

// 1.12a: 0x6FB067E0
void __cdecl PLRTRADE_CloseTradeSessionNoReopen(void)
{
    PLRTRADE_CloseTradeSession(FALSE);
}

// 1.12a: 0x6FB067F0
int32_t __cdecl PLRTRADE_CheckCancelTimeout(void)
{
    if (g_nPlayerTradeState != 7)
    {
        return 0;
    }

    if (g_dwTradeCancelTimeoutStartTick != 0xFFFFFFFFu)
    {
        const DWORD dwNow = GetTickCount();
        if (dwNow > g_dwTradeCancelTimeoutStartTick + 30000u)
        {
            INV_CancelCursorModeOrSend4C();
            g_nPlayerTradeState = 0;
            PLRTRADE_CloseTradeSession(FALSE);
            MSG_SendLeaveGame_69();
            g_bExitAppFlag     = 0;
            g_nRightMouseHoldY = 1;
            g_bExitGameFlag    = 1;
        }
    }
    return 1;
}

// 1.12a: 0x6FB06860
void __cdecl PLRTRADE_CreateOutgoingPromptDialog(void)
{
    if (g_pOutgoingTradePromptDialog != nullptr)
    {
        OPTIONS_HaltAtLine(0x13D);
    }

    D2DialogStrc* pDialog = DIALOG_CreateDynamic(
        320,
        100,
        PLRTRADE_OnCancelOutgoingPrompt,
        PLRTRADE_OnCancelOutgoingPrompt,
        0,
        1,
        1,
        0,
        0,
        0,
        1);
    g_pOutgoingTradePromptDialog = pDialog;
    if (pDialog == nullptr)
    {
        OPTIONS_HaltAtLine(0x529);
    }

    pDialog->nSelectionCursorMode = 1;
    DIALOG_AddLine(g_pOutgoingTradePromptDialog, 0, D2LANG_GetStringFromTblIndex(0x0FCB), 15, 0, 1, nullptr, 0);
    DIALOG_AddLine(g_pOutgoingTradePromptDialog, 0, D2LANG_GetStringFromTblIndex(0x0D48), 15, 0, 1, PLRTRADE_OnCancelOutgoingPrompt, 1);

    pDialog = g_pOutgoingTradePromptDialog;
    if (pDialog == nullptr)
    {
        OPTIONS_HaltAtLine(0x4F2);
    }
    if (pDialog->nLineCount <= -1)
    {
        OPTIONS_HaltAtLine(0x4F8);
    }
    pDialog->nSelectedIndex = -1;
}

// 1.12a: 0x6FB06940
int32_t __cdecl PLRTRADE_CreateIncomingPromptDialog(void)
{
    if (g_pIncomingTradePromptDialog != nullptr)
    {
        return 0;
    }

    D2DialogStrc* pDialog = DIALOG_CreateDynamic(
        320,
        100,
        PLRTRADE_OnDeclineIncomingPrompt,
        PLRTRADE_OnDeclineIncomingPrompt,
        0,
        1,
        1,
        0,
        0,
        0,
        1);
    g_pIncomingTradePromptDialog = pDialog;
    if (pDialog == nullptr)
    {
        OPTIONS_HaltAtLine(0x529);
    }

    pDialog->nSelectionCursorMode = 1;
    DIALOG_AddLine(g_pIncomingTradePromptDialog, 0, D2LANG_GetStringFromTblIndex(0x0FCA), 15, 0, 1, nullptr, 0);
    DIALOG_AddLine(g_pIncomingTradePromptDialog, 0, D2LANG_GetStringFromTblIndex(0x0D49), 15, 0, 1, PLRTRADE_OnAcceptIncomingPrompt, 1);
    DIALOG_AddLine(g_pIncomingTradePromptDialog, 0, D2LANG_GetStringFromTblIndex(0x0D48), 15, 0, 1, PLRTRADE_OnDeclineIncomingPrompt, 1);

    pDialog = g_pIncomingTradePromptDialog;
    if (pDialog == nullptr)
    {
        OPTIONS_HaltAtLine(0x4F2);
    }
    if (pDialog->nLineCount <= -1)
    {
        OPTIONS_HaltAtLine(0x4F8);
    }
    pDialog->nSelectedIndex = -1;
    return 1;
}

static bool PLRTRADE_OpenTradePromptPanelAndHooks(void)
{
    FOG_IsExpansion();
    if (gnPanelCount != D2PANEL_COUNT)
    {
        OPTIONS_HaltAtLine(0x1389);
    }
    if (gbGoldDialogOpen != 0)
    {
        return false;
    }

    for (int32_t i = 0; i < gnPanelCount; ++i)
    {
        if (gaPanelStates[i] == 1)
        {
            const int32_t nRule = g_apUiPanelTable[i]->aActionOnOpen[D2PANEL_TRADEPROMPT];
            if (nRule == 1)
            {
                UI_SetPanelState(i, 1, 0);
            }
            else if (nRule == 4)
            {
                if (g_bInteractedWithNpc != 0)
                {
                    NPCMENU_CloseActiveMenu();
                }
            }
            else if (nRule != 0)
            {
                return false;
            }
        }
    }

    if (gpPlayerUnit == nullptr ||
        (gpPlayerUnit->dwUnitType == UNIT_PLAYER_TYPE && CUNIT_HasActiveInteract(gpPlayerUnit) == 0))
    {
        gaPanelStates[D2PANEL_TRADEPROMPT] = 1;
    }

    if (gaPanelStates[D2PANEL_TRADEPROMPT] != 0)
    {
        gbCmdTblInputEnabled = 0;
        if (gbCmdTblKeyHandlersRegistered == 1)
        {
            gbCmdTblKeyHandlersRegistered = 0;
            WALL2_UnregisterMsgHandlerTable(4, WINDOW_GetWindow(), g_aCmdTblKeyHandlers);
            if (gbCmdTblAltKeyHandlersRegistered == 0)
            {
                WALL2_RegisterMsgHandlerTable(2, WINDOW_GetWindow(), g_aCmdTblKeyUpOnlyHandlers);
                gbCmdTblAltKeyHandlersRegistered = 1;
            }
        }

        g_nTradePlayerGold        = 0;
        g_nTradePartnerGold       = 0;
        g_bTradeAcceptBlocked     = 1;
        g_bTradePanelInitPending  = 0;
        UI_UpdatePanelHotkeysInMsgTable();
        WALL2_RegisterMsgHandlerTable(13, WINDOW_GetWindow(), g_aTradePanelMsgHandlers);
    }
    return true;
}

// 1.12a: 0x6FB06A40 (with jump tables at 0x6FB07090, 0x6FB070E8, 0x6FB070FC, 0x6FB07110, 0x6FB07124)
void __fastcall PLRTRADE_OnRecvSrvTradeActionPacket(const D2SrvTradeActionPacketStrc* pPacket)
{
    const uint32_t uAction = (uint32_t)pPacket->nAction;
    if (uAction > 21u)
    {
        return;
    }

    switch (uAction)
    {
    case 0: // Outgoing trade request initiated ("Waiting for other player...")
        if (g_nPlayerTradeState != 0)
        {
            PLRTRADE_SendCancelTradePacket();
            return;
        }
        if (gaPanelStates[D2PANEL_TRADEPROMPT] == 0)
        {
            if (!PLRTRADE_OpenTradePromptPanelAndHooks())
            {
                PLRTRADE_SendCancelTradePacket();
                return;
            }
            g_bTradeGoldTransferDisabled    = 0;
            g_dwTradeCancelTimeoutStartTick = 0xFFFFFFFFu;
        }
        g_nPlayerTradeState = 1;
        PLRTRADE_CreateOutgoingPromptDialog();
        INV_OpenTradePanel(FALSE);
        break;

    case 1: // Incoming trade request received ("Other player wants to trade")
        if (g_nPlayerTradeState != 0)
        {
            PLRTRADE_SendCancelTradePacket();
            return;
        }
        if (INVENTORY_GetCursorItem(gpPlayerUnit->pInventory) != nullptr ||
            g_bStashOrCubeBusy != 0 ||
            PLRTRADE_CreateIncomingPromptDialog() == 0 ||
            gaPanelStates[D2PANEL_CHAT] != 0)
        {
            MSG_SendByte3Words(0x4F, 2, 0, 0);
            return;
        }
        if (gaPanelStates[D2PANEL_TRADEPROMPT] == 0)
        {
            if (!PLRTRADE_OpenTradePromptPanelAndHooks())
            {
                PLRTRADE_SendCancelTradePacket();
                return;
            }
            g_dwTradeCancelTimeoutStartTick = 0xFFFFFFFFu;
        }
        g_nPlayerTradeState = 2;
        INV_OpenTradePanel(FALSE);
        break;

    case 2: // Partner declined or cancelled trade
        if (g_bTradeGoldBtnPressed != 0 || g_bTradePartnerGoldHighlight != 0)
        {
            SOUND_PlaySoundEffect(3, nullptr, 0, 0, 0);
        }
        g_bTradeGoldBtnPressed       = 0;
        g_bTradePartnerGoldHighlight = 0;
        PLRTRADE_CloseTradeSession(FALSE);
        break;

    case 5: // Partner clicked Accept Trade button
        if (gaPanelStates[D2PANEL_TRADEPROMPT] == 0)
        {
            return;
        }
        g_dwTradeCancelTimeoutStartTick = 0xFFFFFFFFu;
        INV_OpenTradePanel(TRUE);
        switch (g_nPlayerTradeState)
        {
        case 3:
            g_nPlayerTradeState = 5;
            SOUND_PlaySoundEffect((g_bTradeGoldBtnPressed != 0 || g_bTradePartnerGoldHighlight != 0) ? 3u : 1u, nullptr, 0, 0, 0);
            g_bTradeGoldBtnPressed       = 0;
            g_bTradePartnerGoldHighlight = 1;
            break;
        case 4:
            g_nPlayerTradeState = 6;
            if (g_bTradeGoldBtnPressed == 0 || g_bTradePartnerGoldHighlight == 0)
            {
                SOUND_PlaySoundEffect(1, nullptr, 0, 0, 0);
            }
            g_bTradeGoldBtnPressed       = 1;
            g_bTradePartnerGoldHighlight = 1;
            break;
        case 5:
            SOUND_PlaySoundEffect((g_bTradeGoldBtnPressed != 0 || g_bTradePartnerGoldHighlight != 0) ? 3u : 1u, nullptr, 0, 0, 0);
            g_bTradeGoldBtnPressed       = 0;
            g_bTradePartnerGoldHighlight = 1;
            break;
        case 6:
        case 7:
            break;
        default:
            PLRTRADE_SendCancelTradePacket();
            break;
        }
        break;

    case 6: // Trade window opened or Accept unchecked
    {
        if (gaPanelStates[D2PANEL_TRADEPROMPT] == 0)
        {
            return;
        }
        const uint32_t dwGold = (uint32_t)g_nTradePartnerGold;
        MSG_SendByte3Words(0x4F, 8, (uint16_t)(dwGold >> 16), (uint16_t)(dwGold & 0xFFFFu));
        g_dwTradeCancelTimeoutStartTick = 0xFFFFFFFFu;
        INV_OpenTradePanel(TRUE);

        switch (g_nPlayerTradeState)
        {
        case 1:
            g_nTradePlayerGold = 0;
            if (g_bTradeGoldBtnPressed != 0 || g_bTradePartnerGoldHighlight != 0)
            {
                SOUND_PlaySoundEffect(3, nullptr, 0, 0, 0);
            }
            g_bTradeGoldBtnPressed       = 0;
            g_bTradePartnerGoldHighlight = 0;
            g_nPlayerTradeState          = 3;
            if (g_pOutgoingTradePromptDialog != nullptr)
            {
                DIALOG_CloseActiveDialog();
                g_pOutgoingTradePromptDialog = nullptr;
            }
            break;

        case 3:
            if (g_bTradeGoldBtnPressed != 0 || g_bTradePartnerGoldHighlight != 0)
            {
                SOUND_PlaySoundEffect(3, nullptr, 0, 0, 0);
            }
            g_bTradeGoldBtnPressed       = 0;
            g_bTradePartnerGoldHighlight = 0;
            break;

        case 4:
        case 5:
        case 6:
        case 7:
            g_nPlayerTradeState = 3;
            if (g_bTradeGoldBtnPressed != 0 || g_bTradePartnerGoldHighlight != 0)
            {
                SOUND_PlaySoundEffect(3, nullptr, 0, 0, 0);
            }
            g_bTradeGoldBtnPressed       = 0;
            g_bTradePartnerGoldHighlight = 0;
            break;

        default:
            PLRTRADE_SendCancelTradePacket();
            break;
        }
        break;
    }

    case 9: // Local player lacks inventory space
        SOUND_PlayUnitSpeech(gpPlayerUnit, 23);
        break;

    case 10: // Trade partner lacks inventory space
    {
        D2UnitStrc* pUnit = gapServerSideUnitHashTables[UNIT_PLAYER_TYPE][g_dwTradePartnerGUID & 0x7Fu];
        while (pUnit != nullptr && pUnit->dwUnitId != g_dwTradePartnerGUID)
        {
            pUnit = pUnit->pListNext;
        }
        if (pUnit != nullptr)
        {
            if (pUnit->dwUnitType != UNIT_PLAYER_TYPE)
            {
                OPTIONS_HaltAtLine(0x5A);
            }
            SOUND_PlayUnitSpeech(pUnit, 23);
        }
        break;
    }

    case 12: // Close trade without reopening inventory
        g_nPlayerTradeState = 0;
        PLRTRADE_CloseTradeSession(FALSE);
        break;

    case 13: // Trade completed — close trade and reopen inventory
        g_nPlayerTradeState = 0;
        PLRTRADE_CloseTradeSession(TRUE);
        break;

    case 14: // Disable accept check button (red check cooldown)
        g_bTradeGoldTransferDisabled = 1;
        break;

    case 15: // Enable accept check button
        g_bTradeGoldTransferDisabled = 0;
        break;

    case 16: // Open stash / inventory panel
        INV_OpenInventoryPanel();
        break;

    case 17: // Close mercenary inventory panel
        INV_CloseMercenaryInventoryPanel();
        break;

    case 21: // Toggle mercenary inventory panel
        INV_ToggleMercenaryInventoryPanel();
        break;

    default:
        break;
    }
}

// 1.12a: 0x6FB07140
void __cdecl PLRTRADE_RenderAndUpdate(void)
{
    const int32_t nCurMouseX = gnMouseX;
    const int32_t nCurMouseY = gnMouseY;
    int32_t nState = g_nPlayerTradeState;

    if (nState > 2 && g_bPanelTooltipSuppressed == 0)
    {
        int32_t nPanelOffX = g_nPanelOffsetX;
        int32_t nPanelOffY = g_nPanelOffsetY;

        if (nCurMouseX >= nPanelOffX + 0x111 &&
            nCurMouseX <= nPanelOffX + 0x133 &&
            nCurMouseY >= 0xA8 - nPanelOffY &&
            nCurMouseY <= 0xCA - nPanelOffY)
        {
            D2Win_10130_DrawFramedTextEx(
                D2LANG_GetStringFromTblIndex(0x1023),
                nPanelOffX + 0xEF,
                0xA7 - nPanelOffY,
                0,
                0);
            nState     = g_nPlayerTradeState;
            nPanelOffX = g_nPanelOffsetX;
            nPanelOffY = g_nPanelOffsetY;
        }

        if (nCurMouseX >= nPanelOffX + 0xDD &&
            nCurMouseX <= nPanelOffX + 0xFF &&
            nCurMouseY >= 0x17F - nPanelOffY &&
            nCurMouseY <= 0x1A1 - nPanelOffY)
        {
            D2Win_10130_DrawFramedTextEx(
                D2LANG_GetStringFromTblIndex(0x1023),
                nPanelOffX + 0xBB,
                0x17E - nPanelOffY,
                0,
                0);
            nState     = g_nPlayerTradeState;
            nPanelOffX = g_nPanelOffsetX;
            nPanelOffY = g_nPanelOffsetY;
        }

        if (nCurMouseX >= nPanelOffX + 0x111 &&
            nCurMouseX <= nPanelOffX + 0x133 &&
            nCurMouseY >= 0x17F - nPanelOffY &&
            nCurMouseY <= 0x1A1 - nPanelOffY)
        {
            D2Win_10130_DrawFramedTextEx(
                D2LANG_GetStringFromTblIndex(0x1022),
                nPanelOffX + 0x107,
                0x17E - nPanelOffY,
                0,
                0);
            nState = g_nPlayerTradeState;
        }
    }

    if (gpPlayerUnit != nullptr)
    {
        D2RoomStrc* pRoom = UNITS_GetRoom(gpPlayerUnit);
        bool bPlayerDeadOrLeftTown = false;
        if (gpPlayerUnit == nullptr || gpPlayerUnit->dwAnimMode == PLRMODE_DEAD_CONST)
        {
            bPlayerDeadOrLeftTown = true;
        }
        else if (pRoom != nullptr && DUNGEON_IsTownLevelId(DUNGEON_GetLevelIdFromRoom(pRoom)) == 0)
        {
            bPlayerDeadOrLeftTown = true;
        }

        if (bPlayerDeadOrLeftTown)
        {
            if (gaPanelStates[D2PANEL_TRADEPROMPT] == 0)
            {
                return;
            }

            D2CltTradeButtonPacketStrc tPkt = {};
            tPkt.bOpcode = 0x4F;
            tPkt.wSubCmd = 2;
            MSG_SendPacket(&tPkt, sizeof(tPkt));

            const int32_t nWasOpen = gaPanelStates[D2PANEL_TRADEPROMPT];
            FOG_IsExpansion();
            if (gnPanelCount != D2PANEL_COUNT)
            {
                OPTIONS_HaltAtLine(0x1389);
            }

            gaPanelStates[D2PANEL_TRADEPROMPT] = 0;
            if (nWasOpen != 0 && PLRTRADE_CheckCancelTimeout() == 0)
            {
                gnViewportShiftOffsetX = 0;
                gnViewportShiftMode    = 0;
                D2GFX_SetViewMatrixAdjust(0);
                VIEW_SetViewportRect(static_cast<D2ViewportStrc*>(gpClientViewport), 0, 0, gnViewportWidth, gnViewportHeight);
                UI_CloseGoldDialog();
                WALL2_UnregisterMsgHandlerTable(13, WINDOW_GetWindow(), g_aTradePanelMsgHandlers);
                UI_StartPanelCloseClickCooldown();
                PLRTRADE_CloseTradeSession(FALSE);

                gbCmdTblInputEnabled = 1;
                if (gbCmdTblKeyHandlersRegistered == 0)
                {
                    gbCmdTblKeyHandlersRegistered = 1;
                    if (gbCmdTblAltKeyHandlersRegistered != 0)
                    {
                        WALL2_UnregisterMsgHandlerTable(2, WINDOW_GetWindow(), g_aCmdTblKeyUpOnlyHandlers);
                        gbCmdTblAltKeyHandlersRegistered = 0;
                    }
                    WALL2_RegisterMsgHandlerTable(4, WINDOW_GetWindow(), g_aCmdTblKeyHandlers);
                }
            }

            g_bTradeGoldTransferDisabled = 0;
            INV_FreeShopTabCells();
            g_nPlayerTradeState = 0;
            return;
        }

        nState = g_nPlayerTradeState;
    }

    if (nState >= 3 && nState <= 6)
    {
        return;
    }
    if (nState == 1 && g_pOutgoingTradePromptDialog != nullptr)
    {
        DIALOG_Draw(g_pOutgoingTradePromptDialog);
    }
    else if (nState == 2 && g_pIncomingTradePromptDialog != nullptr)
    {
        DIALOG_Draw(g_pIncomingTradePromptDialog);
    }
}

// ============================================================================
// PART 3: UI\spellsel.cpp Head — Skill Hotkey Slot & Charge Queries
//         (0x6FB07450..0x6FB07520)
// ============================================================================

// 1.12a: 0x6FB07450
void __fastcall SPELLSEL_SetSkillHotkeySlot(int32_t nSkillId, int32_t bLeftHand, int32_t nSlotIdx, uint32_t dwOwnerGUID)
{
    g_anSkillHotkeyId[nSlotIdx]    = nSkillId;
    g_abSkillHotkeyLeft[nSlotIdx]  = bLeftHand;
    g_adwSkillHotkeyOwner[nSlotIdx] = dwOwnerGUID;
}

// 1.12a: 0x6FB07470
int32_t __fastcall SPELLSEL_IsSkillHotkeySlotUsable(int32_t nSlotIdx)
{
    const int32_t nSkillId = g_anSkillHotkeyId[nSlotIdx];
    if (nSkillId == -1 || gpPlayerUnit == nullptr)
    {
        return 0;
    }

    D2SkillStrc* pSkill = SKILLS_GetSkillById(gpPlayerUnit, nSkillId, g_adwSkillHotkeyOwner[nSlotIdx]);
    if (pSkill == nullptr)
    {
        return 0;
    }

    if (SKILLS_GetOwnerGUIDFromSkill(pSkill) != 0xFFFFFFFFu)
    {
        int32_t nCharges = 0;
        if (!SKILLS_GetSkillInfo(pSkill, nullptr, nullptr, nullptr, &nCharges) || nCharges <= 0)
        {
            return 0;
        }
    }
    return 1;
}

// 1.12a: 0x6FB074E0..0x6FB07520
int32_t __fastcall SPELLSEL_CheckUnitSkillQuantity(D2UnitStrc* pUnit, int32_t nSkillId)
{
    if (pUnit == nullptr)
    {
        return 0;
    }

    for (D2SkillStrc* pSkill = UNITS_GetStartSkill(pUnit); pSkill != nullptr; pSkill = SKILLS_GetNextSkill(pSkill))
    {
        const D2SkillsTxt* pSkillsTxt = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
        if (pSkillsTxt != nullptr && SKILLS_GetQuantity(pSkill) > 0)
        {
            if ((int32_t)pSkillsTxt->nSkillId == nSkillId)
            {
                break;
            }
        }
    }
    return 0;
}


// Cross-TU VA-Mapped Globals & Adapters for UI_options.cpp
const D2SMsgRegEntryStrc g_aCfgControlsMsgHandlers[13] = {};
const D2SMsgRegEntryStrc g_aCfgControlsWheelHandlers[1] = {};
const D2SMsgRegEntryStrc g_aCharStatMsgHandlers[2] = {};
const D2SMsgRegEntryStrc g_aCmdTblKeyHandlers[4] = {};
const D2SMsgRegEntryStrc g_aCmdTblKeyUpOnlyHandlers[2] = {};
const D2SMsgRegEntryStrc g_aTradePanelMsgHandlers[13] = {};
int32_t g_bAllowPanelToggleSound   = 1;
int32_t g_bPanelTooltipSuppressed  = 0;
int32_t g_bSoundSubsystemInitialized = 1;
int32_t g_bStashOrCubeBusy         = 0;
int32_t g_bTextDisplayBeta         = 0;
int32_t g_bTradePanelInitPending   = 0;
int32_t g_nEquippedBeltType        = 0;
int32_t g_nMouseRepeatDelayCounterA = 16;
int32_t g_nMouseRepeatDelayCounterB = 16;
void*   g_pClientPoolSystem        = nullptr;

extern void __stdcall sub_6FB261D0(BOOL bInit);
extern int __fastcall sub_6FACEAC0(D2UnitStrc* pUnit);
extern void __cdecl sub_6FACA1F0(void);
extern void __stdcall sub_6FB24E20(void);

void __stdcall CFGCONTROLS_SetOpenState(BOOL bInit) { sub_6FB261D0(bInit); }
int32_t __cdecl CTRLPNL_GetEquippedBeltRows(void) { return 2; }
BOOL __fastcall CUNIT_HasActiveInteract(D2UnitStrc* pUnit) { return sub_6FACEAC0(pUnit) == 0; }
void __cdecl NPCMENU_CloseActiveMenu(void) { sub_6FACA1F0(); }
int32_t __cdecl SOUNDCHANNEL_Toggle3DSound(void) { return 0; }
int32_t __cdecl SOUNDCHANNEL_ToggleEAX(void) { return 0; }
void __stdcall TEXT_ClearChatAndCfgControls(void) { sub_6FB24E20(); }
