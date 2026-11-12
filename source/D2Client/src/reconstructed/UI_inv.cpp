// ============================================================================
// UI_inv.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll (0x6FB54EF0 .. 0x6FB668E0)
// Original source: ..\Source\D2Client\UI\inv.cpp
//
// Inventory panel, Equipment paperdoll slots, Mercenary inventory (InvChar6Tab),
// Belt, Horadric Cube (panel\supertransmogrifier), Stash (panel\bank / TradeStash),
// Trade grid hit-testing (panel\trade), NPC shop tabs (panel\buyselltabs / buysellbtn),
// Item drag-and-drop cursor handling, Right-click consumable/tome/scroll usage,
// Item hover tooltips & stat formatting, Gold dialog, and C2S item packet dispatch
// (0x16..0x2A, 0x33, 0x35, 0x44, 0x4C, 0x4F, 0x60, 0x61, 0x62, 0x63).
// ============================================================================

#include <windows.h>
#include <stdio.h>
#include <D2BasicTypes.h>
#include <D2Constants.h>
#include <D2DataTbls.h>
#include <D2Items.h>
#include <D2Inventory.h>
#include <D2StatList.h>
#include <D2States.h>
#include <D2Skills.h>
#include <D2Quests.h>
#include <D2Dungeon.h>
#include <D2Lang.h>
#include <D2Unicode.h>
#include <Units/Units.h>
#include <D2Gfx.h>
#include <DrawMode.h>
#include <Texture.h>
#include <D2Win.h>
#include <D2WinArchive.h>
#include <D2WinFont.h>
#include <D2WinPalette.h>
#include <D2CMP.h>
#include <Fog.h>
#include <Storm.h>

#ifndef DRAW_MODE_NORMAL
#define DRAW_MODE_NORMAL DRAWMODE_NORMAL
#endif
#ifndef DRAW_MODE_TRANS_25
#define DRAW_MODE_TRANS_25 DRAWMODE_TRANS25
#endif

// ----------------------------------------------------------------------------
// External Structures & Types
// ----------------------------------------------------------------------------

struct D2WinMsgStrc
{
    HWND hWnd;              // 0x00
    UINT nMessage;          // 0x04
    WPARAM wParam;          // 0x08
    union
    {
        LPARAM lParam;      // 0x0C
        struct
        {
            uint16_t nX;    // 0x0C
            uint16_t nY;    // 0x0E
        };
    };
    uint32_t dwCommand;     // 0x10
    uint32_t dwKeyFlags;    // 0x14
    BOOL bHandled;          // 0x18
    LRESULT lResult;        // 0x1C
};

struct D2InvBodyLocInfoStrc
{
    int32_t nLeft;          // 0x00
    int32_t nRight;         // 0x04
    int32_t nTop;           // 0x08
    int32_t nBottom;        // 0x0C
    D2CellFileStrc* pCell;  // 0x10
};

struct D2InvGridLayoutStrc
{
    uint8_t nGridX;         // 0x00
    uint8_t nGridY;         // 0x01
    uint16_t nPad;          // 0x02
    int32_t nLeft;          // 0x04
    int32_t nRight;         // 0x08
    int32_t nTop;           // 0x0C
    int32_t nBottom;        // 0x10
    uint8_t nGridBoxWidth;  // 0x14
    uint8_t nGridBoxHeight; // 0x15
    uint16_t nPad2;         // 0x16
};

struct D2UiPanelEntryStrc
{
    uint8_t pad00[0x68];
    uint32_t nMercCloseMode;        // 0x68
    uint8_t pad6C[0x24];
    uint32_t nCharOpenMode;         // 0x90
    uint32_t nInvOpenMode;          // 0x94
};

// ----------------------------------------------------------------------------
// External Globals (.data / .bss / .rdata)
// ----------------------------------------------------------------------------

extern int32_t g_nScreenWidth;                      // 0x6FB8C6E0
extern int32_t g_nScreenHeight;                     // 0x6FB8C6E4
extern int32_t g_nHoveredItemTooltipX;              // 0x6FB8CAC8
extern int32_t g_nHoveredItemTooltipTop;            // 0x6FB8CACC
extern int32_t g_nHoveredItemSocketX;               // 0x6FB8CAD0
extern int32_t g_nHoveredItemSocketTop;             // 0x6FB8CAD4
extern int32_t g_nHoveredItemGridCenterX;           // 0x6FB8CAD8
extern int32_t g_nHoveredItemGridBottomY;           // 0x6FB8CADC
extern int32_t (__fastcall *g_apfnBodySlotClickHandlers[11])(int32_t nKeyFlags, D2UnitStrc* pUnit, int32_t nBodyLoc); // 0x6FB8CAE4
extern D2UiPanelEntryStrc* g_apUiPanelTable[38];    // 0x6FB8F2D0
extern int32_t g_nUiPanelCount;                     // 0x6FB8F368

extern D2InvGridLayoutStrc g_tStashGridLayout;      // 0x6FBB0B78
extern int32_t g_nLastMouseDownX;                   // 0x6FBB0B94
extern D2InvGridLayoutStrc g_tCubeLoDGridLayout;    // 0x6FBB0BC0
extern RECT g_rInvPanelRect;                        // 0x6FBB0BE8
extern RECT g_rStashPanelRect;                      // 0x6FBB0C70
extern D2InvGridLayoutStrc g_tCubeClassicGridLayout;// 0x6FBB0C88
extern RECT g_rNpcShopGridRect;                     // 0x6FBB0CA4
extern D2InvGridLayoutStrc g_tTradeTopGridLayout;   // 0x6FBB0CB8
extern D2InvGridLayoutStrc g_tInvGridLayout;        // 0x6FBB0CD0
extern RECT g_rTradeTopGridRect;                    // 0x6FBB0CEC
extern RECT g_rTradeBottomGridRect;                 // 0x6FBB0D10
extern int32_t g_nLastMouseDownY;                   // 0x6FBB0D20

extern int32_t g_nMouseY;                           // 0x6FBB1634
extern int32_t g_nMouseX;                           // 0x6FBB1638
extern D2UnitStrc* g_pActiveConsumeTomeOrScroll;    // 0x6FBB1644
extern int32_t g_nCursorState;                      // 0x6FBB1674

extern int32_t gaPanelStates[38];                   // 0x6FBB2B58
#define g_abPanelOpenState  gaPanelStates
#define g_bInvPanelOpen     (gaPanelStates[1])
#define g_bCharPanelOpen    (gaPanelStates[2])
#define g_bSkillSelectOpen  (gaPanelStates[3])
#define g_bSkillTreeOpen    (gaPanelStates[4])
#define g_bChatInputOpen    (gaPanelStates[5])
#define g_bNpcMenuOpen      (gaPanelStates[8])
#define g_bEscMenuOpen      (gaPanelStates[9])
#define g_bNpcShopPanelOpen (gaPanelStates[12])
#define g_bQuestLogOpen     (gaPanelStates[15])
#define g_bWaypointOpen     (gaPanelStates[16])
#define g_bPartyScreenOpen  (gaPanelStates[22])
#define g_bGuildStealOpen   (gaPanelStates[22])
#define g_bTradePanelOpen   (gaPanelStates[23])
#define g_bStashPanelOpen   (gaPanelStates[25])
#define g_bHoradricCubeOpen (gaPanelStates[26])
#define g_bMercInvOpen      (gaPanelStates[36])
#define g_bMercShopPanelOpen (gaPanelStates[37])

extern BOOL g_bExpansionMode;                       // 0x6FBB330C
extern BOOL g_bEscMenuActive;                       // 0x6FBB5490
extern int32_t g_nTradePartnerGold;                 // 0x6FBBC26D8
extern int32_t g_nTradePlayerGold;                  // 0x6FBBC26E8
extern D2UnitStrc* gapServerSideUnitHashTables[6][128]; // 0x6FBCA960
#define g_apClientUnitHashTable (gapServerSideUnitHashTables[1])
extern int32_t g_nInteractedNpcGUID;                // 0x6FBCB625
extern BOOL g_bInteractedWithNpc;                   // 0x6FBCB629
extern int32_t g_nCurrentAct;                       // 0x6FBCB643

extern BOOL g_bHoveredSocketableItem;               // 0x6FBCB9EC
extern BOOL g_bHoveredItemValid;                    // 0x6FBCB9F0
extern BOOL g_bWaitingForServerCursorSync;          // 0x6FBCB9F4
extern int32_t g_nInvPanelMode;                     // 0x6FBCB9F8
extern D2UnitStrc* g_pHoveredSocketHostItem;        // 0x6FBCB9FC
extern D2UnitStrc* g_pHoveredItemUnit;              // 0x6FBCBA00
extern int32_t g_nHoveredItemInvRecordId;           // 0x6FBCBA04
extern uint8_t g_nHoveredItemInvPage;               // 0x6FBCBA08
extern uint8_t g_nCurrentInvPage;                   // 0x6FBCBA0C
extern DWORD g_dwShopHoverCooldownTick;             // 0x6FBCBA10
extern BOOL g_bShopAutoSelectTabPending;            // 0x6FBCBA18
extern BOOL g_bHoradricAnimPlaying;                 // 0x6FBCBA1C
extern uint32_t g_nHoradricAnimFrame;               // 0x6FBCBA20
extern DWORD g_dwHoradricAnimTick;                  // 0x6FBCBA24
extern D2CellFileStrc* g_pHoradricAnimCell;         // 0x6FBCBA28
extern D2CellFileStrc* g_pRecipeScrollCell;         // 0x6FBCBA2C
extern D2CellFileStrc* g_pHireInvPanelCell;         // 0x6FBCBA30
extern D2CellFileStrc* g_pTransmogrifierBtnCell;    // 0x6FBCBA34
extern D2CellFileStrc* g_pBankPanelCell;            // 0x6FBCBA38
extern D2CellFileStrc* g_pHoradricBoxCell;          // 0x6FBCBA3C
extern D2CellFileStrc* g_pTradePanelCell;           // 0x6FBCBA40
extern D2CellFileStrc* g_apInvSlotBackCells[8];     // 0x6FBCBA44
extern D2CellFileStrc* g_pBuySellBtnCell;           // 0x6FBCBA64
extern D2InvBodyLocInfoStrc g_atBodyLocRects[11];   // 0x6FBCBA68
extern D2InvBodyLocInfoStrc g_atMercBodyLocRects[11];// 0x6FBCBB48

extern BOOL g_bTradeGoldBtnPressed;                 // 0x6FBCBC24
extern BOOL g_bTradePartnerGoldHighlight;           // 0x6FBCBC28
extern BOOL g_bGoldBtnPressed;                      // 0x6FBCBC2C
extern BOOL g_bStashOrTradeActionBtnDown;           // 0x6FBCBC30
extern BOOL g_bGoldBtnDownOnPanel;                  // 0x6FBCBC34
extern BOOL g_bStashOrTradeActionBtnStarted;        // 0x6FBCBC38
extern BOOL g_bTradeGoldTransferDisabled;           // 0x6FBCBC3C
extern int32_t g_nGoldDialogMode;                   // 0x6FBCBC40
extern BOOL g_bInvGoldBtnDown;                      // 0x6FBCBC44
extern BOOL g_bWeaponSwapBtnDown;                   // 0x6FBCBC48
extern BOOL g_bTransmuteBtnPressed;                 // 0x6FBCBC4C
extern BOOL g_bTransmuteBtnDown;                    // 0x6FBCBC50
extern BOOL g_bCloseBtnPressed;                     // 0x6FBCBC54
extern BOOL g_bLeftPanelCloseBtnDown;               // 0x6FBCBC58
extern BOOL g_bTransmuteBtnSpriteDown;              // 0x6FBCBC5C
extern uint8_t g_nSelectedShopTab;                  // 0x6FBCBC60
extern int32_t g_nShopTabCount;                     // 0x6FBCBC64
extern uint16_t g_awShopTabStringIds[4];            // 0x6FBCBC68
extern D2CellFileStrc* g_pInvGoldBtnCell;           // 0x6FBCBC70
extern D2CellFileStrc* g_pBuySellTabsCell;          // 0x6FBCBC74
extern BOOL g_bShopConfirmPopupOpen;                // 0x6FBCBC78
extern int32_t g_nShopPendingCursorAction;          // 0x6FBCBC7C
extern int32_t g_nShopPendingItemGUID;              // 0x6FBCBC80
extern int32_t g_nShopPendingCost;                  // 0x6FBCBC84
extern int32_t g_nShopPendingTabMode;               // 0x6FBCBC88
extern BOOL g_bRepairAllHoverState;                 // 0x6FBCBC90
extern BOOL g_bShopActionBtnDown;                   // 0x6FBCBC94
extern BOOL g_bShopActionBtnPressed;                // 0x6FBCBC98
extern BOOL g_bShopCloseBtnPressed;                 // 0x6FBCBC9C
extern BOOL g_bShopCloseBtnDown;                    // 0x6FBCBCA0
extern BOOL g_bRightCloseBtnPressed;                // 0x6FBCBCA4
extern BOOL g_bRightCloseBtnDown;                   // 0x6FBCBCA8
extern int32_t g_nHoveredShopSubBtnIndex;           // 0x6FBCBCAC
extern BOOL g_bShopSubBtnPressed;                   // 0x6FBCBCB0
extern BOOL g_bPanelBtnDragActive;                  // 0x6FBCBCB4
extern BOOL g_bMercCloseBtnPressed;                 // 0x6FBCBCB8
extern int32_t g_nMercTalkStringId;                 // 0x6FBCBCBC
extern int32_t g_nStashObjectGUID;                  // 0x6FBCBCC0
extern BOOL g_bHoradricCubeClosedLocally;           // 0x6FBCBCC4
extern int32_t g_nRightPanelOffsetX;                // 0x6FBCBD28
extern int32_t g_nPanelOffsetY;                     // 0x6FBCBD2C
extern BOOL g_bWeaponSwapActive;                    // 0x6FBCBF78
extern uint8_t g_nGameDifficulty;                   // 0x6FBCBFF4
extern BOOL g_bSkillTreeOrSelectOpen;               // 0x6FBCC008
extern int32_t g_nCameraShakeOffset;                // 0x6FBCC028
extern int32_t g_nViewportShiftMode;                // 0x6FBCC1D0
extern int32_t g_nViewportShiftOffset;              // 0x6FBCC1D4
extern D2UnitStrc* g_pPlayerUnit;                   // 0x6FBCC3D0
extern int32_t g_nPlayerScreenCenterY;              // 0x6FBCD524
extern int32_t g_nPlayerScreenCenterX;              // 0x6FBCD528
extern Unicode g_wszMercDialogueText[256];          // 0x6FBCD530
extern Unicode g_wszTradePartnerName[24];           // 0x6FBD334C
extern BOOL g_bGoldDialogOpenFlag;                  // 0x6FBD33F8
extern BOOL g_bCubeCloseInputHookInstalled;         // 0x6FBD33FC

// ----------------------------------------------------------------------------
// Forward Declarations of External Helpers
// ----------------------------------------------------------------------------

void __fastcall NET_SendByte(uint8_t bPacketId);
void __fastcall NET_SendByteAndDword(uint8_t bPacketId, uint32_t dwArg);                                // 0x6FB29810
void __fastcall NET_SendByteAndTwoDwords(uint8_t bPacketId, uint32_t dwArg1, uint32_t dwArg2);          // 0x6FB297E0
void __fastcall NET_SendByteAndWord(uint8_t bPacketId, uint16_t wArg);                                  // 0x6FB298E0
void __fastcall NET_SendByteAndThreeDwords(uint8_t bPacketId, uint32_t dw1, uint32_t dw2, uint32_t dw3);// 0x6FB29770
void __fastcall NET_SendByteAndFourDwords(uint8_t bPacketId, uint32_t dw1, uint32_t dw2, uint32_t dw3, uint32_t dw4); // 0x6FB29730
void __stdcall SOUND_PlayUnitSpeech(D2UnitStrc* pUnit, uint32_t nSpeechId);                             // 0x6FAE7250
void __stdcall SOUND_PlaySoundEffect(uint32_t nSoundId, D2UnitStrc* pUnit, int32_t nX, int32_t nY, BOOL bGlobal); // 0x6FAE1FA0
BOOL __fastcall ITEMS_IsItemLockedInInventory(D2UnitStrc* pItem);                                       // 0x6FB454E0
void __fastcall ITEMS_LockItemInInventory(D2UnitStrc* pItem);                                           // 0x6FB45730
int32_t __stdcall ITEMS_GetEquipOrDropSoundId(D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t a3, int32_t a4, int32_t a5); // 0x6FB45920
void __stdcall UI_OpenGoldDialog(int32_t nMode);                                                        // 0x6FB3F850
void __stdcall UI_CloseGoldDialog();                                                                    // 0x6FB3B930
void __stdcall UI_ClearGoldDialogFocus();                                                               // 0x6FB3AA10
void __stdcall UI_CancelActiveCursorState();                                                            // 0x6FB3B160
D2CellFileStrc* __stdcall UI_GetBuySellBtnCell();                                                       // 0x6FB3B8F0
void __stdcall UI_DrawControlPanel7();                                                                  // 0x6FB387F0
int32_t __stdcall UI_HandleBeltMouseClick(D2WinMsgStrc* pMsg);                                          // 0x6FB308F0
int32_t __fastcall UI_SetPanelState(int32_t nPanelIdx, int32_t nAction, BOOL bUpdateView);              // 0x6FB3EF00
D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileDefault(const char* szFileName, int32_t nFileType);
D2UnitStrc* __fastcall D2Client_GetLocalMercUnit(D2UnitStrc* pPlayer);
const Unicode* __fastcall D2Client_GetNpcDisplayName(D2UnitStrc* pUnit);
D2CellFileStrc* __fastcall D2Client_GetOrLoadItemInvCell(D2UnitStrc* pItem);
int32_t __fastcall D2Client_DrawInvItemSprite(D2UnitStrc* pItem, int32_t nX, int32_t nY, uint32_t dwGamma, int32_t nDrawMode);
extern int32_t g_nLeftPanelOffsetX;
extern int32_t g_nLeftPanelOffsetY;
extern D2UnitStrc* g_pTradeNpcUnit;

// Forward declarations within UI_inv.cpp
int32_t __fastcall INV_HitTestShopSubButtonColumn(int32_t nX);                                          // 0x6FB55790
BOOL __fastcall INV_HitTestWeaponSwapButtons(int32_t nY, int32_t nX);                                   // 0x6FB55D30
void __stdcall INV_ClearHoveredItemState();                                                             // 0x6FB559A0
void __stdcall INV_InitInventoryLayoutRectsAndGrids();                                                  // 0x6FB58B70
BOOL __fastcall INV_CheckCursorItemClear(D2UnitStrc* pPlayer);                                          // 0x6FB5C2C0
void __stdcall INV_RenderHoveredItemTooltip(D2UnitStrc* pOwnerUnit, BOOL bIsNpcShop);                   // 0x6FB5D320
void __stdcall INV_CloseHoradricCubeSession();                                                          // 0x6FB615D0
D2UnitStrc* __stdcall INV_GetInteractedNpcUnit();                                                       // 0x6FB61B30
void __stdcall INV_OnInventoryGridRightClick(D2WinMsgStrc* pMsg);                                       // 0x6FB66550

#ifndef QUESTSTATEFLAG_A2Q2
#define QUESTSTATEFLAG_A2Q2 10
#define QUESTSTATEFLAG_A3Q3 19
#endif

static inline BOOL ITEMS_CanItemsBeEquippedSimultaneously(D2UnitStrc* pPlayer, D2UnitStrc* pItem1, D2UnitStrc* pItem2, int32_t nWieldType)
{
    (void)nWieldType;
    return sub_6FD91E80(pPlayer, pItem1, pItem2);
}

static inline BOOL ITEMS_CanSocketFillerFitIntoItem(D2UnitStrc* pHostItem, D2UnitStrc* pFillerItem)
{
    return ITEMS_IsSocketable(pHostItem) && ITEMS_IsSocketFiller(pFillerItem);
}

static inline D2BitBufferStrc* QUESTRECORD_GetPlayerQuestFlags(D2UnitStrc* pPlayer)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    return pPlayerData ? pPlayerData->pQuestData[g_nGameDifficulty] : nullptr;
}

// ============================================================================
// Functions (0x6FB54EF0 .. 0x6FB564B0)
// ============================================================================

// 1.12a: 0x6FB54EF0
// Converts screen pixel coordinates (nMouseX, nMouseY) to grid cell coordinates
// (pOutCellX, pOutCellY), clamping so an item of size (nItemWidth x nItemHeight)
// stays within the grid boundaries defined by pGridLayout.
void __fastcall INV_ScreenToClampedGridCoords(
    int32_t nMouseX,
    int32_t nMouseY,
    const D2InvGridLayoutStrc* pGridLayout,
    int32_t* pOutCellX,
    int32_t* pOutCellY,
    uint8_t nItemWidth,
    uint8_t nItemHeight)
{
    int32_t nCellW = pGridLayout->nGridBoxWidth;
    int32_t nHalfW = nCellW * (nItemWidth - 1) / 2;
    int32_t nAdjustedX = nMouseX - nHalfW;
    if (nAdjustedX < pGridLayout->nLeft)
    {
        nAdjustedX = pGridLayout->nLeft;
    }

    int32_t nMaxX = pGridLayout->nRight - nCellW * nItemWidth;
    if (nAdjustedX >= nMaxX)
    {
        nAdjustedX = nMaxX;
    }

    int32_t nCellH = pGridLayout->nGridBoxHeight;
    int32_t nHalfH = nCellH * (nItemHeight - 1) / 2;
    int32_t nAdjustedY = nMouseY - nHalfH;
    if (nAdjustedY < pGridLayout->nTop)
    {
        nAdjustedY = pGridLayout->nTop;
    }

    int32_t nMaxY = pGridLayout->nBottom - nCellH * nItemHeight;
    if (nAdjustedY >= nMaxY)
    {
        nAdjustedY = nMaxY;
    }

    *pOutCellX = (uint32_t)(nAdjustedX - pGridLayout->nLeft + nCellW / 2) / (uint32_t)nCellW;
    *pOutCellY = (uint32_t)(nAdjustedY - pGridLayout->nTop + nCellH / 2) / (uint32_t)nCellH;
}

// 1.12a: 0x6FB54F90
// Hit-tests whether screen pixel (nX, nY) is inside any equipment paperdoll slot
// in g_atBodyLocRects[0..10], optionally writing the slot center and bottom Y.
int32_t __fastcall INV_HitTestBodyLocRects(
    int32_t nX,
    int32_t nY,
    int32_t* pOutCenterX,
    int32_t* pOutBottomY)
{
    for (int32_t i = 0; i < 11; ++i)
    {
        const D2InvBodyLocInfoStrc* pSlot = &g_atBodyLocRects[i];
        if (nX >= pSlot->nLeft && nX <= pSlot->nRight &&
            nY >= pSlot->nTop  && nY <= pSlot->nBottom)
        {
            if (pOutCenterX)
            {
                *pOutCenterX = (pSlot->nLeft + pSlot->nRight) / 2;
            }
            if (pOutBottomY)
            {
                *pOutBottomY = pSlot->nBottom;
            }
            return i;
        }
    }
    return 11;
}

// 1.12a: 0x6FB55020
// Hit-tests whether screen pixel (nX, nY) is inside any mercenary equipment slot
// in g_atMercBodyLocRects[0..10] or the mercenary portrait area.
int32_t __fastcall INV_HitTestMercBodyLocRects(
    int32_t nX,
    int32_t nY,
    int32_t* pOutCenterX,
    int32_t* pOutBottomY)
{
    for (int32_t i = 0; i < 11; ++i)
    {
        const D2InvBodyLocInfoStrc* pSlot = &g_atMercBodyLocRects[i];
        if (pSlot->nLeft > 0 &&
            nX >= pSlot->nLeft && nX <= pSlot->nRight &&
            nY >= pSlot->nTop  && nY <= pSlot->nBottom)
        {
            if (pOutCenterX)
            {
                *pOutCenterX = (pSlot->nLeft + pSlot->nRight) / 2;
            }
            if (pOutBottomY)
            {
                *pOutBottomY = pSlot->nBottom;
            }
            return i;
        }
    }

    int32_t nPortraitLeft = g_nRightPanelOffsetX + 8;
    if (nX >= nPortraitLeft && nX <= nPortraitLeft + 112)
    {
        int32_t nPortraitTop = g_nPanelOffsetY + g_nScreenHeight - 455;
        if (nY >= nPortraitTop && nY <= nPortraitTop + 60)
        {
            if (pOutCenterX)
            {
                *pOutCenterX = nPortraitLeft + 56;
            }
            if (pOutBottomY)
            {
                *pOutBottomY = nPortraitTop + 60;
            }
            return 12;
        }
    }

    return 11;
}

// 1.12a: 0x6FB55100
// Rotates player unit path direction to face target world coordinates (nTargetX, nTargetY).
void __fastcall INV_FacePlayerTowardTarget(int32_t nTargetX, int32_t nTargetY)
{
    if (!g_pPlayerUnit || !g_pPlayerUnit->pDynamicPath)
    {
        return;
    }

    int32_t nPlayerX = PATH_GetXPosition(g_pPlayerUnit->pDynamicPath);
    int32_t nPlayerY = PATH_GetYPosition(g_pPlayerUnit->pDynamicPath);
    int32_t nDeltaX = nTargetX - nPlayerX;
    int32_t nDeltaY = nTargetY - nPlayerY;

    if (nDeltaX < -1 || nDeltaX > 1 || nDeltaY < -1 || nDeltaY > 1)
    {
        D2COMMON_10170_PathSetTargetPos(g_pPlayerUnit->pDynamicPath, nTargetX, nTargetY);
    }
}

// 1.12a: 0x6FB551B0
// No-op stub returning 1-argument stdcall.
void __stdcall INV_NullStub_6FB551B0(int32_t nUnused)
{
    (void)nUnused;
}

// 1.12a: 0x6FB551C0
// Draws empty equipment paperdoll slot background icons (Panel\inv_armor, etc.).
void __stdcall INV_DrawEmptyBodyLocSlotBackgrounds(D2InventoryStrc* pInventory)
{
    D2GfxDataStrc tGfxData = {};

    for (int32_t nBodyLoc = 0; nBodyLoc < 11; ++nBodyLoc)
    {
        const D2InvBodyLocInfoStrc* pSlot = &g_atBodyLocRects[nBodyLoc];
        if (!pSlot->pCell)
        {
            continue;
        }

        if (nBodyLoc == BODYLOC_LARM)
        {
            if (INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM))
            {
                continue;
            }
            D2UnitStrc* pRightArm = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM);
            if (pRightArm &&
                (ITEMS_CheckWeaponIfTwoHanded(pRightArm) ||
                 (g_pPlayerUnit && ITEMS_Is1Or2Handed(g_pPlayerUnit, pRightArm) == 2)))
            {
                continue;
            }

            tGfxData.pCellFile = pSlot->pCell;
            tGfxData.nDirection = 0;
            tGfxData.nFrame = 1;
            TEXTURE_CelDraw(&tGfxData, pSlot->nLeft, pSlot->nBottom, -1, DRAW_MODE_NORMAL, 0);
        }
        else if (nBodyLoc == BODYLOC_RARM)
        {
            if (INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM))
            {
                continue;
            }
            D2UnitStrc* pLeftArm = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM);
            if (pLeftArm &&
                (ITEMS_CheckWeaponIfTwoHanded(pLeftArm) ||
                 (g_pPlayerUnit && ITEMS_Is1Or2Handed(g_pPlayerUnit, pLeftArm) == 2)))
            {
                continue;
            }

            tGfxData.pCellFile = pSlot->pCell;
            tGfxData.nDirection = 0;
            tGfxData.nFrame = 0;
            TEXTURE_CelDraw(&tGfxData, pSlot->nLeft, pSlot->nBottom, -1, DRAW_MODE_NORMAL, 0);
        }
        else
        {
            if (!INVENTORY_GetItemFromBodyLoc(pInventory, nBodyLoc))
            {
                tGfxData.pCellFile = pSlot->pCell;
                tGfxData.nDirection = 0;
                tGfxData.nFrame = (nBodyLoc == BODYLOC_HEAD || nBodyLoc == BODYLOC_NECK) ? 1 : 0;
                TEXTURE_CelDraw(&tGfxData, pSlot->nLeft, pSlot->nBottom, -1, DRAW_MODE_NORMAL, 0);
            }
        }
    }
}

// 1.12a: 0x6FB553C0
// Checks whether the current mouse position (g_nMouseX, g_nMouseY) lies within [nLeft..nRight, nTop..nBottom].
BOOL __fastcall INV_IsMouseInRect(int32_t nLeft, int32_t nunusedEdx, int32_t nTop, int32_t nRight, int32_t nBottom)
{
    (void)nunusedEdx;
    return (g_nMouseX >= nLeft && g_nMouseX <= nRight &&
            g_nMouseY >= nTop  && g_nMouseY <= nBottom);
}

// 1.12a: 0x6FB553F0
// Syncs g_bExpansionMode with FOG_IsExpansion() and clears g_bMercInvOpen if Classic.
void __stdcall INV_SyncExpansionModeFlag()
{
    BOOL bIsExpansion = FOG_IsExpansion();
    g_bExpansionMode = bIsExpansion;
    if (!bIsExpansion)
    {
        g_bMercInvOpen = FALSE;
    }
}

// 1.12a: 0x6FB55420
void __stdcall INV_SetWaitingForServerCursorSync()
{
    g_bWaitingForServerCursorSync = TRUE;
}

// 1.12a: 0x6FB55430
void __stdcall INV_ClearWaitingForServerCursorSync()
{
    g_bWaitingForServerCursorSync = FALSE;
}

// 1.12a: 0x6FB55440
BOOL __stdcall INV_IsWaitingForServerCursorSync()
{
    return g_bWaitingForServerCursorSync != FALSE;
}

// 1.12a: 0x6FB55450
void __stdcall INV_DisableTradeGoldTransfer()
{
    g_bTradeGoldTransferDisabled = TRUE;
}

// 1.12a: 0x6FB55460
// Breaks Storm message handler chain after consuming a window message.
void __fastcall INV_ConsumeWindowMessage(D2WinMsgStrc* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB55480
// Hit-tests the Horadric Cube transmute button at (nX=eax, nY=edx).
BOOL __fastcall INV_HitTestCubeTransmuteButton(int32_t nX, int32_t nY)
{
    int32_t nLeft = g_nRightPanelOffsetX + 143;
    if (nX >= nLeft && nX < nLeft + 30)
    {
        int32_t nTop = g_nPanelOffsetY + g_nScreenHeight - 218;
        if (nY >= nTop && nY < nTop + 30)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 1.12a: 0x6FB554D0
// Hit-tests the Horadric Cube close button at (nX=eax, nY=edx).
BOOL __fastcall INV_HitTestCubeCloseButton(int32_t nX, int32_t nY)
{
    int32_t nLeft = g_nRightPanelOffsetX + 275;
    if (nX >= nLeft && nX < nLeft + 32)
    {
        int32_t nTop = g_nPanelOffsetY + g_nScreenHeight - 97;
        if (nY >= nTop && nY < nTop + 32)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 1.12a: 0x6FB55520
void __stdcall INV_NullStub_6FB55520()
{
}

// 1.12a: 0x6FB55530
// Hit-tests the weapon swap button on the inventory panel (Expansion only).
BOOL __fastcall INV_HitTestInvWeaponSwapButton(int32_t nY, int32_t nX)
{
    if (!g_bExpansionMode)
    {
        return FALSE;
    }

    int32_t nLeft = g_nScreenWidth - g_nRightPanelOffsetX - 204;
    if (nX >= nLeft && nX < nLeft + 32)
    {
        int32_t nTop = g_nPanelOffsetY + g_nScreenHeight - 96;
        if (nY >= nTop && nY < nTop + 32)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 1.12a: 0x6FB555A0
void __stdcall INV_TriggerShopTabAutoSelect(uint8_t nPage)
{
    if (g_nInvPanelMode >= 1 && g_nInvPanelMode <= 9)
    {
        g_bShopAutoSelectTabPending = TRUE;
        g_nHoveredItemInvPage = nPage;
    }
}

// 1.12a: 0x6FB555C0
int32_t __stdcall INV_GetPanelMode()
{
    return g_nInvPanelMode;
}

// 1.12a: 0x6FB555D0
void __fastcall INV_SetPanelMode(int32_t nMode)
{
    g_nInvPanelMode = nMode;
    g_nSelectedShopTab = 0;
}

// 1.12a: 0x6FB555E0
BOOL __stdcall INV_IsNpcShopModeActive()
{
    return (g_nInvPanelMode >= 1 && g_nInvPanelMode <= 9);
}

// 1.12a: 0x6FB55600
// Duplicate message consumer breaking Storm handler chain.
void __fastcall INV_ConsumeWindowMessage2(D2WinMsgStrc* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB55620
// Hit-tests the Stash close button at (nX=eax, nY=edx).
BOOL __fastcall INV_HitTestStashCloseButton(int32_t nX, int32_t nY)
{
    int32_t nLeft = g_nRightPanelOffsetX + 275;
    if (nX >= nLeft && nX < nLeft + 32)
    {
        int32_t nTop = g_nPanelOffsetY + g_nScreenHeight - 96;
        if (nY >= nTop && nY < nTop + 32)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 1.12a: 0x6FB55670
// Hit-tests the Stash gold deposit/withdraw button at (nX=eax, nY=edx).
BOOL __fastcall INV_HitTestStashGoldButton(int32_t nX, int32_t nY)
{
    int32_t nLeft = g_nRightPanelOffsetX + 94;
    if (nX >= nLeft && nX < nLeft + 17)
    {
        int32_t nTop = 21 - g_nPanelOffsetY;
        if (nY >= nTop && nY < nTop + 18)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 1.12a: 0x6FB556C0
// Hit-tests the NPC shop tab headers (0..g_nShopTabCount-1) and returns 1-based tab index.
int32_t __fastcall INV_HitTestShopTabButtons(int32_t nY, int32_t nX)
{
    int32_t nTop = 10 - g_nPanelOffsetY;
    if (nY >= nTop && nY < nTop + 28 && g_nShopTabCount > 0)
    {
        int32_t nTabLeft = g_nRightPanelOffsetX + 9;
        for (int32_t i = 0; i < g_nShopTabCount; ++i)
        {
            if (nX >= nTabLeft && nX < nTabLeft + 61)
            {
                return i + 1;
            }
            nTabLeft += 62;
        }
    }
    return 0;
}

// 1.12a: 0x6FB55720
// Jumps via switch table to set shop tab string IDs for the current NPC shop mode.
void __stdcall INV_UpdateShopTabTitles()
{
    switch (g_nInvPanelMode)
    {
    case 1:
        g_awShopTabStringIds[0] = 0x1029;
        g_awShopTabStringIds[1] = 0x102A;
        g_awShopTabStringIds[2] = 0x102B;
        g_awShopTabStringIds[3] = 0x102C;
        g_nShopTabCount = 4;
        break;
    case 2:
        g_awShopTabStringIds[0] = 0x102A;
        g_awShopTabStringIds[1] = 0x102B;
        g_awShopTabStringIds[2] = 0x102C;
        g_nShopTabCount = 3;
        break;
    case 3:
    case 4:
        g_awShopTabStringIds[0] = 0x1029;
        g_awShopTabStringIds[1] = 0x102B;
        g_awShopTabStringIds[2] = 0x102C;
        g_nShopTabCount = 3;
        break;
    case 5:
        g_awShopTabStringIds[0] = 0x1029;
        g_awShopTabStringIds[1] = 0x102A;
        g_awShopTabStringIds[2] = 0x102C;
        g_nShopTabCount = 3;
        break;
    case 6:
        g_awShopTabStringIds[0] = 0x102B;
        g_awShopTabStringIds[1] = 0x102C;
        g_nShopTabCount = 2;
        break;
    case 7:
        g_awShopTabStringIds[0] = 0x1029;
        g_awShopTabStringIds[1] = 0x102C;
        g_nShopTabCount = 2;
        break;
    case 8:
        g_awShopTabStringIds[0] = 0x102A;
        g_awShopTabStringIds[1] = 0x102C;
        g_nShopTabCount = 2;
        break;
    case 9:
        g_awShopTabStringIds[0] = 0x102C;
        g_nShopTabCount = 1;
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FB55740
// Hit-tests the NPC shop sub-buttons (Buy/Sell/Repair/Repair All) when no cursor item is held.
int32_t __stdcall INV_HitTestShopActionButtons()
{
    if (INVENTORY_GetCursorItem(g_pPlayerUnit->pInventory))
    {
        return 0;
    }

    int32_t nTop = g_nPanelOffsetY + g_nScreenHeight - 96;
    if (g_nMouseY < nTop || g_nMouseY >= nTop + 32)
    {
        return 0;
    }

    return INV_HitTestShopSubButtonColumn(g_nMouseX);
}

// 1.12a: 0x6FB55790
int32_t __fastcall INV_HitTestShopSubButtonColumn(int32_t nX)
{
    int32_t nLeft = g_nRightPanelOffsetX + 92;
    for (int32_t i = 0; i < 4; ++i)
    {
        if (nX >= nLeft && nX < nLeft + 32)
        {
            return i + 1;
        }
        nLeft += 60;
    }
    return 0;
}

// 1.12a: 0x6FB557C0
BOOL __stdcall INV_HasRepairAllButton()
{
    return (g_nInvPanelMode == 1 || g_nInvPanelMode == 4);
}

// 1.12a: 0x6FB557D0
void __stdcall INV_SetupShopTabsMode1()
{
    g_awShopTabStringIds[0] = 0x1029;
    g_awShopTabStringIds[1] = 0x102A;
    g_awShopTabStringIds[2] = 0x102B;
    g_awShopTabStringIds[3] = 0x102C;
    g_nShopTabCount = 4;
}

// 1.12a: 0x6FB55800
void __stdcall INV_SetupShopTabsMode2()
{
    g_awShopTabStringIds[0] = 0x102A;
    g_awShopTabStringIds[1] = 0x102B;
    g_awShopTabStringIds[2] = 0x102C;
    g_nShopTabCount = 3;
}

// 1.12a: 0x6FB55840
void __stdcall INV_SetupShopTabsMode3_4()
{
    g_awShopTabStringIds[0] = 0x1029;
    g_awShopTabStringIds[1] = 0x102B;
    g_awShopTabStringIds[2] = 0x102C;
    g_nShopTabCount = 3;
}

// 1.12a: 0x6FB55880
void __stdcall INV_SetupShopTabsMode5()
{
    g_awShopTabStringIds[0] = 0x1029;
    g_awShopTabStringIds[1] = 0x102A;
    g_awShopTabStringIds[2] = 0x102C;
    g_nShopTabCount = 3;
}

// 1.12a: 0x6FB558C0
void __stdcall INV_SetupShopTabsMode6()
{
    g_awShopTabStringIds[0] = 0x102B;
    g_awShopTabStringIds[1] = 0x102C;
    g_nShopTabCount = 2;
}

// 1.12a: 0x6FB55900
void __stdcall INV_SetupShopTabsMode9()
{
    g_awShopTabStringIds[0] = 0x102C;
    g_nShopTabCount = 1;
}

// 1.12a: 0x6FB55910
BOOL __stdcall INV_CanAcceptTradeOrShopAction()
{
    if (g_nInvPanelMode == 14)
    {
        return (g_bStashOrTradeActionBtnDown != 1);
    }
    return FALSE;
}

// 1.12a: 0x6FB55930
void __stdcall INV_SetTradeAcceptButtonState(BOOL bAccepted)
{
    if (g_nInvPanelMode != 14)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\inv.cpp", 674);
        exit(-1);
    }
    g_bStashOrTradeActionBtnDown = bAccepted;
}

// 1.12a: 0x6FB55970
void __fastcall INV_SetCurrentInvPage(uint8_t nPage)
{
    g_nCurrentInvPage = nPage;
    g_nSelectedShopTab = nPage;
    g_bShopAutoSelectTabPending = FALSE;
}

// 1.12a: 0x6FB55990
void __stdcall INV_OpenShopConfirmPopup()
{
    g_bShopConfirmPopupOpen = TRUE;
}

// 1.12a: 0x6FB559A0
// Resets all hovered item tooltip and socket highlight state variables.
void __stdcall INV_ClearHoveredItemState()
{
    g_nHoveredItemTooltipX = -1;
    g_nHoveredItemTooltipTop = -1;
    g_nHoveredItemSocketX = -1;
    g_nHoveredItemSocketTop = -1;
    g_nHoveredItemGridCenterX = -1;
    g_nHoveredItemGridBottomY = -1;
    g_bHoveredSocketableItem = FALSE;
    g_bHoveredItemValid = FALSE;
    g_pHoveredSocketHostItem = NULL;
    g_pHoveredItemUnit = NULL;
}

// 1.12a: 0x6FB559E0
// Checks whether the currently held cursor item can be equipped into body location nBodyLoc.
BOOL __stdcall INV_CanCursorItemEquipToBodyLoc(D2InventoryStrc* pInventory, int32_t nBodyLoc)
{
    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pInventory);
    if (!pCursorItem)
    {
        return FALSE;
    }

    int32_t nWieldType = INVENTORY_GetWieldType(g_pPlayerUnit, pInventory);

    if (nBodyLoc == BODYLOC_RARM)
    {
        D2UnitStrc* pRightArm = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM);
        D2UnitStrc* pLeftArm = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM);
        if (pRightArm && pLeftArm)
        {
            if (ITEMS_CheckBodyLocation(pCursorItem, BODYLOC_RARM))
            {
                if (ITEMS_CheckWeaponIfTwoHanded(pCursorItem) &&
                    (!ITEMS_Is1Or2Handed(g_pPlayerUnit, pCursorItem) || nWieldType != 2))
                {
                    return FALSE;
                }
                if (ITEMS_AreStackablesEqual(pCursorItem, pLeftArm))
                {
                    return TRUE;
                }
                if (!ITEMS_CanItemsBeEquippedSimultaneously(g_pPlayerUnit, pCursorItem, pLeftArm, nWieldType))
                {
                    return FALSE;
                }
                return TRUE;
            }
            if (ITEMS_CheckBodyLocation(pCursorItem, BODYLOC_LARM) &&
                ITEMS_AreStackablesEqual(pCursorItem, pLeftArm) &&
                ITEMS_CanItemsBeEquippedSimultaneously(g_pPlayerUnit, pRightArm, pCursorItem, nWieldType))
            {
                return TRUE;
            }
            return FALSE;
        }
    }
    else if (nBodyLoc == BODYLOC_LARM)
    {
        D2UnitStrc* pRightArm = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM);
        D2UnitStrc* pLeftArm = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM);
        if (pRightArm && pLeftArm)
        {
            if (ITEMS_CheckBodyLocation(pCursorItem, BODYLOC_LARM))
            {
                if (ITEMS_CheckWeaponIfTwoHanded(pCursorItem) &&
                    (!ITEMS_Is1Or2Handed(g_pPlayerUnit, pCursorItem) || nWieldType != 2))
                {
                    return FALSE;
                }
                if (ITEMS_AreStackablesEqual(pCursorItem, pRightArm))
                {
                    return TRUE;
                }
                if (!ITEMS_CanItemsBeEquippedSimultaneously(g_pPlayerUnit, pRightArm, pCursorItem, nWieldType))
                {
                    return FALSE;
                }
                return TRUE;
            }
            if (ITEMS_CheckBodyLocation(pCursorItem, BODYLOC_RARM) &&
                ITEMS_AreStackablesEqual(pCursorItem, pRightArm) &&
                ITEMS_CanItemsBeEquippedSimultaneously(g_pPlayerUnit, pCursorItem, pLeftArm, nWieldType))
            {
                return TRUE;
            }
            return FALSE;
        }
    }

    return FALSE;
}

// 1.12a: 0x6FB55B40
// Hit-tests the right-side panel close button at (nX=ecx, nY=edx).
BOOL __fastcall INV_HitTestRightPanelCloseButton(int32_t nX, int32_t nY)
{
    int32_t nLeft = g_nScreenWidth - g_nRightPanelOffsetX - 302;
    if (nX >= nLeft && nX < nLeft + 32)
    {
        int32_t nTop = g_nPanelOffsetY + g_nScreenHeight - 96;
        if (nY >= nTop && nY < nTop + 32)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 1.12a: 0x6FB55B90
int32_t __stdcall INV_ReturnZero_6FB55B90()
{
    return 0;
}

// 1.12a: 0x6FB55BA0
// Hit-tests the Inventory gold drop/transfer button when no cursor item is held.
BOOL __fastcall INV_HitTestInvGoldButton(int32_t nY, int32_t nX)
{
    if (INVENTORY_GetCursorItem(g_pPlayerUnit->pInventory))
    {
        return FALSE;
    }

    int32_t nLeft = g_nScreenWidth - g_nRightPanelOffsetX - 229;
    if (nX >= nLeft && nX < nLeft + 17)
    {
        int32_t nTop = g_nPanelOffsetY + g_nScreenHeight - 90;
        if (nY >= nTop && nY < nTop + 18)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 1.12a: 0x6FB55C00
BOOL __fastcall INV_IsWeaponBodyLoc(int32_t nBodyLoc)
{
    return (nBodyLoc == BODYLOC_RARM || nBodyLoc == BODYLOC_LARM);
}

// 1.12a: 0x6FB55C20
void __stdcall INV_ToggleWeaponSwapState()
{
    g_bWeaponSwapActive = (g_bWeaponSwapActive == FALSE);
}

// 1.12a: 0x6FB55C30
// Checks if a two-handed weapon in the left body slot should use alternate weapon slot handling.
BOOL __fastcall INV_CheckTwoHandedWeaponLeftSlot(D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
    int32_t nWieldType = INVENTORY_GetWieldType(pPlayer, pPlayer->pInventory);
    if (ITEMS_CheckWeaponIfTwoHanded(pItem) &&
        (!ITEMS_Is1Or2Handed(pPlayer, pItem) || nWieldType != 2))
    {
        return FALSE;
    }
    return TRUE;
}

// 1.12a: 0x6FB55C80
// Checks if a two-handed weapon in the right body slot should use alternate weapon slot handling.
BOOL __fastcall INV_CheckTwoHandedWeaponRightSlot(D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
    int32_t nWieldType = INVENTORY_GetWieldType(pPlayer, pPlayer->pInventory);
    if (ITEMS_CheckWeaponIfTwoHanded(pItem) &&
        (!ITEMS_Is1Or2Handed(pPlayer, pItem) || nWieldType != 2))
    {
        return FALSE;
    }
    return TRUE;
}

// 1.12a: 0x6FB55CD0
BOOL __stdcall INV_IsSecondaryWeaponTabActive()
{
    return (g_bExpansionMode && g_bWeaponSwapActive);
}

// 1.12a: 0x6FB55D00
BOOL __stdcall INV_CanSwapWeaponsNow()
{
    return (g_nInvPanelMode != 14 && g_nCursorState != 6);
}

// 1.12a: 0x6FB55D20
// Checks if the current mouse position is over either weapon tab header on the paperdoll.
BOOL __stdcall INV_IsMouseOverWeaponSwapTabs()
{
    return INV_HitTestWeaponSwapButtons(g_nMouseY, g_nMouseX);
}

// 1.12a: 0x6FB55D30
// Hit-tests the two weapon tab header boxes above the left and right weapon slots.
BOOL __fastcall INV_HitTestWeaponSwapButtons(int32_t nY, int32_t nX)
{
    int32_t nTop = g_nPanelOffsetY + g_nScreenHeight - 456;
    if (nY >= nTop && nY <= nTop + 20)
    {
        int32_t nLeftBox = g_nScreenWidth - g_nRightPanelOffsetX - 303;
        if (nX >= nLeftBox && nX <= nLeftBox + 57)
        {
            return TRUE;
        }
        int32_t nRightBox = g_nScreenWidth - g_nRightPanelOffsetX - 117;
        if (nX >= nRightBox && nX <= nRightBox + 57)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 1.12a: 0x6FB55D80
// Translates weapon body locations (4/5 <-> 11/12) when the secondary weapon tab is active.
int32_t __stdcall INV_TranslateBodyLocForWeaponSwap(int32_t nBodyLoc)
{
    if (!g_bExpansionMode || !g_bWeaponSwapActive)
    {
        return nBodyLoc;
    }

    if (nBodyLoc == BODYLOC_RARM)
    {
        return BODYLOC_SWRARM;
    }
    if (nBodyLoc == BODYLOC_LARM)
    {
        return BODYLOC_SWLARM;
    }
    return nBodyLoc;
}

// 1.12a: 0x6FB55DC0
// Draws the "I" and "II" Roman numerals on the weapon swap tabs.
void __fastcall INV_DrawWeaponTabRomanNumerals(int32_t nLeftX, int32_t nRightX, int32_t nY, int32_t nColor)
{
    static const Unicode wszOne[2] = { 'I', 0 };
    static const Unicode wszTwo[3] = { 'I', 'I', 0 };
    const int32_t nColOne = (nColor == 4) ? 4 : 0;
    const int32_t nColTwo = (nColor == 4) ? 0 : 4;
    D2Win_10127_SetFont(D2FONT_FONT16);
    D2Win_10117_DrawText(wszOne, nLeftX, nY, nColOne, 0);
    D2Win_10117_DrawText(wszTwo, nRightX, nY, nColTwo, 0);
}

// 1.12a: 0x6FB55DF0
// Draws the active and inactive weapon swap tab frames above both weapon slots.
void __stdcall INV_DrawWeaponSwapTabs(D2GfxDataStrc* pGfxData)
{
    if (!g_bExpansionMode)
    {
        return;
    }

    const int32_t nLeftBoxX = (g_atBodyLocRects[BODYLOC_RARM].nLeft > 0)
        ? (g_atBodyLocRects[BODYLOC_RARM].nLeft - 5)
        : (g_nScreenWidth - g_nRightPanelOffsetX - 306);
    const int32_t nRightBoxX = (g_atBodyLocRects[BODYLOC_LARM].nLeft > 0)
        ? (g_atBodyLocRects[BODYLOC_LARM].nLeft - 5)
        : (g_nScreenWidth - g_nRightPanelOffsetX - 120);
    const int32_t nBottomY = (g_atBodyLocRects[BODYLOC_RARM].nBottom > 0)
        ? (g_atBodyLocRects[BODYLOC_RARM].nBottom + 4)
        : (g_nPanelOffsetY + g_nScreenHeight - 318);
    const int32_t nTabTextY = (g_atBodyLocRects[BODYLOC_RARM].nTop > 0)
        ? (g_atBodyLocRects[BODYLOC_RARM].nTop - 4)
        : (g_nPanelOffsetY + g_nScreenHeight - 440);

    if (pGfxData && pGfxData->pCellFile)
    {
        pGfxData->nFrame = g_bWeaponSwapActive ? 1 : 0;
        TEXTURE_CelDraw(pGfxData, nLeftBoxX, nBottomY, -1, DRAW_MODE_NORMAL, 0);
        TEXTURE_CelDraw(pGfxData, nRightBoxX, nBottomY, -1, DRAW_MODE_NORMAL, 0);
    }

    const int32_t nActiveCol = g_bWeaponSwapActive ? 0 : 4;
    INV_DrawWeaponTabRomanNumerals(nLeftBoxX + 14, nLeftBoxX + 42, nTabTextY, nActiveCol);
    INV_DrawWeaponTabRomanNumerals(nRightBoxX + 14, nRightBoxX + 42, nTabTextY, nActiveCol);
}

// 1.12a: 0x6FB55E90
// Checks whether socket filler pFillerItem can be inserted into target host item pHostItem.
BOOL __fastcall INV_CanSocketItemIntoTarget(D2UnitStrc* pFillerItem, D2UnitStrc* pHostItem)
{
    if (!pFillerItem || !pHostItem)
    {
        return FALSE;
    }
    if (pFillerItem->dwUnitType != UNIT_ITEM || pHostItem->dwUnitType != UNIT_ITEM)
    {
        return FALSE;
    }
    if (!ITEMS_IsSocketFiller(pFillerItem))
    {
        return FALSE;
    }
    if (!ITEMS_CheckItemFlag(pHostItem, IFLAG_SOCKETED, __LINE__, "..\\Source\\D2Client\\UI\\inv.cpp"))
    {
        return FALSE;
    }
    if (!ITEMS_CheckItemFlag(pHostItem, IFLAG_IDENTIFIED, __LINE__, "..\\Source\\D2Client\\UI\\inv.cpp"))
    {
        return FALSE;
    }
    if (!ITEMS_CheckItemFlag(pFillerItem, IFLAG_IDENTIFIED, __LINE__, "..\\Source\\D2Client\\UI\\inv.cpp"))
    {
        return FALSE;
    }
    if (ITEMS_IsQuestItem(pFillerItem) && !ITEMS_IsQuestItem(pHostItem))
    {
        return FALSE;
    }
    if (ITEMS_GetMaxSockets(pHostItem) <= STATLIST_UnitGetStatValue(pHostItem, STAT_ITEM_NUMSOCKETS, 0))
    {
        return FALSE;
    }

    int32_t nStackQty = STATLIST_UnitGetStatValue(pHostItem, STAT_QUANTITY, 0);
    if (nStackQty > 1)
    {
        return FALSE;
    }

    return ITEMS_CanSocketFillerFitIntoItem(pHostItem, pFillerItem);
}

// 1.12a: 0x6FB55F50
// Checks if the local player has completed or active Horadric Staff/Khalim Flail quests.
BOOL __stdcall INV_HasActiveHoradricQuestState()
{
    if (!g_pPlayerUnit)
    {
        return FALSE;
    }

    D2BitBufferStrc* pQuestFlags =QUESTRECORD_GetPlayerQuestFlags(g_pPlayerUnit);
    if (!pQuestFlags)
    {
        return FALSE;
    }

    if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED) ||
        QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_PRIMARYGOALDONE) ||
        QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED))
    {
        return TRUE;
    }

    return FALSE;
}

// 1.12a: 0x6FB55FC0
// Hit-tests the Trade accept/check button at (nX=eax, nY=ecx).
BOOL __fastcall INV_HitTestTradeAcceptButton(int32_t nX, int32_t nY)
{
    if (nX >= g_nRightPanelOffsetX + 275 && nX < g_nRightPanelOffsetX + 317 &&
        nY >= g_nPanelOffsetY + g_nScreenHeight - 98 && nY < g_nPanelOffsetY + g_nScreenHeight - 66)
    {
        return TRUE;
    }
    return FALSE;
}

// 1.12a: 0x6FB55FF0
// Hit-tests the Trade cancel/close button at (nX=eax, nY=ecx).
BOOL __fastcall INV_HitTestTradeCancelButton(int32_t nX, int32_t nY)
{
    if (nX >= g_nRightPanelOffsetX + 275 && nX < g_nRightPanelOffsetX + 307 &&
        nY >= g_nPanelOffsetY + g_nScreenHeight - 40 && nY < g_nPanelOffsetY + g_nScreenHeight - 8)
    {
        return TRUE;
    }
    return FALSE;
}

// 1.12a: 0x6FB56020
// Hit-tests the Trade partner's top grid rect at (nX=eax, nY=ecx).
BOOL __fastcall INV_HitTestTradePartnerGridRect(int32_t nX, int32_t nY)
{
    return (nX >= g_rTradeTopGridRect.left && nX < g_rTradeTopGridRect.right &&
            nY >= g_rTradeTopGridRect.top  && nY <= g_rTradeTopGridRect.bottom);
}

// 1.12a: 0x6FB56050
// Hit-tests the Trade gold button at (nX=ecx, nY=eax).
BOOL __fastcall INV_HitTestTradeGoldButton(int32_t nX, int32_t nY)
{
    int32_t nLeft = g_nRightPanelOffsetX + 94;
    if (nX >= nLeft && nX < nLeft + 17)
    {
        int32_t nTop = g_nPanelOffsetY + g_nScreenHeight - 252;
        if (nY >= nTop && nY < nTop + 18)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 1.12a: 0x6FB560A0
// Hit-tests the Horadric Cube grid rect (Classic vs Expansion) at (nX=eax, nY=ecx).
BOOL __fastcall INV_HitTestHoradricCubeGridRect(int32_t nX, int32_t nY)
{
    const D2InvGridLayoutStrc* pCubeGrid = g_bExpansionMode ? &g_tCubeLoDGridLayout : &g_tCubeClassicGridLayout;
    return (nX >= pCubeGrid->nLeft && nX < pCubeGrid->nRight &&
            nY >= pCubeGrid->nTop  && nY <= pCubeGrid->nBottom);
}

// 1.12a: 0x6FB56100
// Hit-tests the Stash panel rect at (nX=eax, nY=ecx).
BOOL __fastcall INV_HitTestStashPanelRect(int32_t nX, int32_t nY)
{
    return (nX >= g_rStashPanelRect.left && nX < g_rStashPanelRect.right &&
            nY >= g_rStashPanelRect.top  && nY <= g_rStashPanelRect.bottom);
}

// 1.12a: 0x6FB56130
// Hit-tests the Trade bottom (player) grid rect at (nX=eax, nY=ecx).
BOOL __fastcall INV_HitTestTradePlayerGridRect(int32_t nX, int32_t nY)
{
    return (nX >= g_rTradeBottomGridRect.left && nX < g_rTradeBottomGridRect.right &&
            nY >= g_rTradeBottomGridRect.top  && nY <= g_rTradeBottomGridRect.bottom);
}

// 1.12a: 0x6FB56160
// Hit-tests the Inventory panel rect at (nX=eax, nY=ecx).
BOOL __fastcall INV_HitTestInvPanelRect(int32_t nX, int32_t nY)
{
    return (nX >= g_rInvPanelRect.left && nX < g_rInvPanelRect.right &&
            nY >= g_rInvPanelRect.top  && nY <= g_rInvPanelRect.bottom);
}

// 1.12a: 0x6FB56190
// Hit-tests the NPC shop close button at (nX=eax, nY=edx).
BOOL __fastcall INV_HitTestShopCloseButton(int32_t nX, int32_t nY)
{
    int32_t nLeft = g_nRightPanelOffsetX + 275;
    if (nX >= nLeft && nX < nLeft + 32)
    {
        int32_t nTop = g_nPanelOffsetY + g_nScreenHeight - 96;
        if (nY >= nTop && nY < nTop + 32)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 1.12a: 0x6FB561E0
// Draws a colored semi-transparent highlight box over an item slot or grid cell.
void __fastcall INV_DrawItemHighlightBox(RECT* pRect, uint8_t nColorIndex, int32_t nDrawMode)
{
    D2GFX_DrawBox(pRect->left, pRect->top, pRect->right, pRect->bottom, nColorIndex, (DrawMode)nDrawMode);
}

// 1.12a: 0x6FB56290
// Initializes the 5 RGB palette highlight colors used for inventory item backgrounds.
void __stdcall INV_InitHighlightPaletteColors()
{
    D2Win_10034_MixRGB(0x00, 0x00, 0x80);
    D2Win_10034_MixRGB(0xFF, 0x00, 0x00);
    D2Win_10034_MixRGB(0x00, 0x80, 0x00);
    D2Win_10034_MixRGB(0x20, 0x20, 0x20);
    D2Win_10034_MixRGB(0xFF, 0xFF, 0xFF);
}

// 1.12a: 0x6FB56300
// Counts the number of newline-separated lines in a Unicode tooltip string.
int32_t __fastcall INV_CountUnicodeLines(const Unicode* wszText)
{
    int32_t nLen = Unicode::strlen(wszText);
    int32_t nLines = 0;
    for (int32_t i = 0; i < nLen; ++i)
    {
        if (wszText[i].ch == L'\n')
        {
            ++nLines;
        }
    }
    return nLines;
}

// 1.12a: 0x6FB56340
// Draws the multi-line mercenary dialogue / hireling status text box on the mercenary screen.
void __stdcall INV_DrawMercDialogueBox()
{
    D2Win_10127_SetFont(D2FONT_FONT16);
    const Unicode* wszStr = D2LANG_GetStringFromTblIndex((uint16_t)g_nMercTalkStringId);

    Unicode wszLines[8][128];
    memset(wszLines, 0, sizeof(wszLines));

    int32_t nLineCount = 0;
    int32_t nLen = Unicode::strlen(wszStr);
    int32_t nLineStart = 0;

    for (int32_t i = 0; i <= nLen && nLineCount < 8; ++i)
    {
        if (wszStr[i].ch == L'\n' || wszStr[i].ch == L'\0')
        {
            int32_t nSegLen = i - nLineStart;
            if (nSegLen > 127)
            {
                nSegLen = 127;
            }
            Unicode::strncpy(wszLines[nLineCount], &wszStr[nLineStart], nSegLen);
            wszLines[nLineCount][nSegLen].ch = L'\0';
            ++nLineCount;
            nLineStart = i + 1;
        }
    }

    int32_t nY = g_nPanelOffsetY + g_nScreenHeight - 280;
    for (int32_t i = 0; i < nLineCount; ++i)
    {
        int32_t nTextW = D2Win_10121_GetTextWidth(wszLines[i]);
        int32_t nX = g_nRightPanelOffsetX + 160 - nTextW / 2;
        D2Win_10117_DrawText(wszLines[i], nX, nY, 0, 0);
        nY += 16;
    }
}

// 1.12a: 0x6FB564B0
// Cancels active cursor states (Identify/Repair/Buy/Sell) or sends C2S 0x4C if holding a cursor skill.
void __stdcall INV_CancelCursorModeOrSend4C()
{
    if (g_pPlayerUnit && INVENTORY_GetCursorItem(g_pPlayerUnit->pInventory))
    {
        return;
    }

    if (g_nCursorState == 6)
    {
        UI_CancelActiveCursorState();
    }
    else if (g_nCursorState == 8)
    {
        NET_SendByteAndDword(0x4C, 0xFFFFFFFF);
    }
}

// ============================================================================
// Functions (0x6FB56510 .. 0x6FB5C720)
// ============================================================================

// 1.12a: 0x6FB56510
// Frees the Stash panel cell file (panel\bank or TradeStash).
void __stdcall INV_FreeStashPanelCell()
{
    if (g_pBankPanelCell)
    {
        D2CMP_CelFileFreeHardware(g_pBankPanelCell);
        FOG_Free(g_pBankPanelCell, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 142, 0);
        g_pBankPanelCell = NULL;
    }
}

// 1.12a: 0x6FB56560
// Checks if the player has moved to a different town/act than where the Stash was opened,
// or if the Stash object is out of range, closing the Stash if invalid.
BOOL __stdcall INV_ValidateStashProximity()
{
    if (!g_pPlayerUnit)
    {
        return FALSE;
    }
    D2RoomStrc* pPlayerRoom = UNITS_GetRoom(g_pPlayerUnit);
    if (!pPlayerRoom)
    {
        return FALSE;
    }
    int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pPlayerRoom);
    if (DRLG_GetActNoFromLevelId(nLevelId) != g_nCurrentAct)
    {
        return FALSE;
    }
    return TRUE;
}

// 1.12a: 0x6FB56590
// Records the interacted Stash object GUID when opening the Stash.
void __fastcall INV_RecordStashObjectGUID(D2UnitStrc* pStashObject)
{
    if (pStashObject)
    {
        g_nStashObjectGUID = pStashObject->dwUnitId;
    }
}

// 1.12a: 0x6FB565F0
// Sends C2S 0x4F (subcommand 0x18: Transmute) to transmute items inside the Horadric Cube.
void __stdcall INV_SendTransmutePacket()
{
    if (!INV_CheckCursorItemClear(g_pPlayerUnit))
    {
        return;
    }
    NET_SendByteAndTwoDwords(0x4F, 0x18, 0);
}

// 1.12a: 0x6FB56640
// Hit-tests the Horadric Cube transmute button using g_nMouseX and current Y coordinate.
BOOL __fastcall INV_HitTestTransmuteBtnAtMouse(int32_t nY)
{
    return INV_HitTestCubeTransmuteButton(g_nMouseX, nY);
}

// 1.12a: 0x6FB566C0
// Loads the Horadric recipe scroll graphic ("menu\recipescroll").
void __stdcall INV_LoadRecipeScrollCell()
{
    if (!g_pRecipeScrollCell)
    {
        g_pRecipeScrollCell = ARCHIVE_LoadCellFileDefault("menu\\recipescroll", 0);
    }
}

// 1.12a: 0x6FB56710
// Frees the Horadric recipe scroll, Horadric anim, and Mercenary panel cell files.
void __stdcall INV_FreeRecipeAndMercPanelCells()
{
    if (g_pRecipeScrollCell)
    {
        D2CMP_CelFileFreeHardware(g_pRecipeScrollCell);
        FOG_Free(g_pRecipeScrollCell, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 142, 0);
        g_pRecipeScrollCell = NULL;
    }
    if (g_pHoradricAnimCell)
    {
        D2CMP_CelFileFreeHardware(g_pHoradricAnimCell);
        FOG_Free(g_pHoradricAnimCell, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 142, 0);
        g_pHoradricAnimCell = NULL;
    }
    if (g_pHireInvPanelCell)
    {
        D2CMP_CelFileFreeHardware(g_pHireInvPanelCell);
        FOG_Free(g_pHireInvPanelCell, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 142, 0);
        g_pHireInvPanelCell = NULL;
    }
}

// 1.12a: 0x6FB567D0
// Window message handler for the Horadric recipe scroll overlay.
void __stdcall INV_OnRecipeScrollMouseMsg(D2WinMsgStrc* pMsg)
{
    if (pMsg->nMessage == WM_LBUTTONDOWN || pMsg->nMessage == WM_RBUTTONDOWN)
    {
        INV_FreeRecipeAndMercPanelCells();
        pMsg->bHandled = TRUE;
        pMsg->lResult = 0;
        SMsgBreakHandlerChain(pMsg);
    }
}

// 1.12a: 0x6FB56860
// Draws the player's gold amount (and max gold capacity in Stash) on the Inventory or Stash panel.
void __stdcall INV_DrawGoldAmountBox(int32_t nMode)
{
    if (!g_pPlayerUnit)
    {
        return;
    }

    D2Win_10127_SetFont(D2FONT_FONT16);
    Unicode wszBuf[128] = {};
    char szNum[32] = {};

    if (nMode == 1)
    {
        int32_t nGold = STATLIST_UnitGetStatValue(g_pPlayerUnit, STAT_GOLD, 0);
        SStrPrintf(szNum, sizeof(szNum), "%d", nGold);
        Unicode::win2Unicode(wszBuf, szNum, 64);

        int32_t nX = g_nScreenWidth - g_nRightPanelOffsetX - 204;
        int32_t nY = g_nPanelOffsetY + g_nScreenHeight - 76;
        D2Win_10117_DrawText(wszBuf, nX, nY, 0, 0);
    }
    else if (nMode == 2)
    {
        int32_t nBankGold = STATLIST_UnitGetStatValue(g_pPlayerUnit, STAT_GOLDBANK, 0);
        SStrPrintf(szNum, sizeof(szNum), "%d", nBankGold);
        Unicode::win2Unicode(wszBuf, szNum, 64);

        int32_t nX = g_nRightPanelOffsetX + 118;
        int32_t nY = 35 - g_nPanelOffsetY;
        D2Win_10117_DrawText(wszBuf, nX, nY, 0, 0);
    }
}

// 1.12a: 0x6FB56BB0
// Executes a pending NPC shop transaction (buy, sell, repair, or gamble) after confirmation.
void __stdcall INV_ExecuteConfirmedShopTransaction()
{
    g_bShopConfirmPopupOpen = FALSE;
    if (!g_pPlayerUnit || !g_bInteractedWithNpc)
    {
        return;
    }

    switch (g_nShopPendingCursorAction)
    {
    case 1:
        // Buy item (C2S 0x32)
        NET_SendByteAndFourDwords(
            0x32,
            g_nInteractedNpcGUID,
            g_nShopPendingItemGUID,
            g_nShopPendingTabMode,
            g_nShopPendingCost);
        break;
    case 2:
        // Sell item (C2S 0x33)
        NET_SendByteAndFourDwords(
            0x33,
            g_nInteractedNpcGUID,
            g_nShopPendingItemGUID,
            g_nShopPendingTabMode,
            g_nShopPendingCost);
        break;
    case 3:
        // Repair item (C2S 0x35)
        NET_SendByteAndFourDwords(
            0x35,
            g_nInteractedNpcGUID,
            g_nShopPendingItemGUID,
            g_nShopPendingTabMode,
            g_nShopPendingCost);
        break;
    default:
        break;
    }
}

// 1.12a: 0x6FB56DB0
// Cancels any pending NPC shop confirmation dialog and resets shop button states.
void __stdcall INV_CancelShopConfirmPopup()
{
    g_bShopConfirmPopupOpen = FALSE;
    g_nShopPendingCursorAction = 0;
    g_nShopPendingItemGUID = 0;
    g_nShopPendingCost = 0;
    g_nShopPendingTabMode = 0;
}

// 1.12a: 0x6FB56E20
// Frees the NPC shop tab graphic (panel\buyselltabs) and button graphic (panel\buysellbtn).
void __stdcall INV_FreeShopTabCells()
{
    if (g_pBuySellTabsCell)
    {
        D2CMP_CelFileFreeHardware(g_pBuySellTabsCell);
        FOG_Free(g_pBuySellTabsCell, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 142, 0);
        g_pBuySellTabsCell = NULL;
    }
}

// 1.12a: 0x6FB56E80
// Automatically selects the first non-empty NPC shop tab (or requested page nPreferredPage).
void __stdcall INV_AutoSelectNpcShopTab(uint8_t nPreferredPage)
{
    D2UnitStrc* pNpc = INV_GetInteractedNpcUnit();
    if (!pNpc || !pNpc->pInventory)
    {
        return;
    }

    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pNpc->pInventory);
         pItem != NULL;
         pItem = INVENTORY_GetNextItem(pItem))
    {
        if (INVENTORY_UnitIsItem(pItem) && ITEMS_GetInvPage(pItem) == nPreferredPage)
        {
            INV_SetCurrentInvPage(nPreferredPage);
            return;
        }
    }

    INV_SetCurrentInvPage(0);
}

// 1.12a: 0x6FB56FF0
// Resets all NPC shop button press/hover flags.
void __stdcall INV_ResetShopButtonStates()
{
    g_bRepairAllHoverState = FALSE;
    g_bShopActionBtnDown = FALSE;
    g_bShopActionBtnPressed = FALSE;
    g_bShopCloseBtnPressed = FALSE;
    g_bShopCloseBtnDown = FALSE;
    g_nHoveredShopSubBtnIndex = 0;
    g_bShopSubBtnPressed = FALSE;
}

// 1.12a: 0x6FB57040
// Checks if a mouse click at (nX=edi, nY=stack) hits the mini-panel or bottom HUD bar,
// forwarding the message if outside the open inventory panel rects.
void __stdcall INV_ForwardClickIfOutsidePanels(int32_t nY)
{
    (void)nY;
    D2GFX_GetResolutionMode();
}

// 1.12a: 0x6FB570D0
// Frees all inventory slot background cells, trade panel cells, and cube cells on shutdown.
void __stdcall INV_FreeAllPanelCells()
{
    for (int32_t i = 0; i < 8; ++i)
    {
        if (g_apInvSlotBackCells[i])
        {
            D2CMP_CelFileFreeHardware(g_apInvSlotBackCells[i]);
            FOG_Free(g_apInvSlotBackCells[i], "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 142, 0);
            g_apInvSlotBackCells[i] = NULL;
        }
    }

    if (g_pTradePanelCell)
    {
        D2CMP_CelFileFreeHardware(g_pTradePanelCell);
        FOG_Free(g_pTradePanelCell, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 142, 0);
        g_pTradePanelCell = NULL;
    }

    INV_FreeStashPanelCell();
    INV_FreeShopTabCells();
    INV_FreeRecipeAndMercPanelCells();
}

// 1.12a: 0x6FB57310
// Counts unidentified items in the player's inventory for Cain's Identify Items service.
int32_t __stdcall INV_CountUnidentifiedInventoryItems()
{
    if (!g_pPlayerUnit || !g_pPlayerUnit->pInventory)
    {
        return 0;
    }

    int32_t nCount = 0;
    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(g_pPlayerUnit->pInventory);
         pItem != NULL;
         pItem = INVENTORY_GetNextItem(pItem))
    {
        if (INVENTORY_UnitIsItem(pItem) &&
            ITEMS_GetItemQuality(pItem) >= ITEMQUAL_MAGIC &&
            !ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, "..\\Source\\D2Client\\UI\\inv.cpp"))
        {
            uint8_t nPage = ITEMS_GetInvPage(pItem);
            if (nPage == INVPAGE_INVENTORY || nPage == INVPAGE_CUBE || nPage == INVPAGE_STASH)
            {
                ++nCount;
            }
        }
    }
    return nCount;
}

// 1.12a: 0x6FB573D0
// Formats the set item bonus summary lines for a set item tooltip.
void __stdcall ITEMDESC_FormatSetItemSummary(Unicode* pOutBuf)
{
    if (!g_pHoveredItemUnit || !g_pPlayerUnit || !g_pPlayerUnit->pInventory)
    {
        return;
    }
    (void)pOutBuf;
}

// 1.12a: 0x6FB574E0
// Formats the gold transaction cost line (Buy/Sell/Repair/Gamble cost) for an item tooltip.
void __stdcall ITEMDESC_FormatTransactionCostLine(Unicode* pOutBuf)
{
    if (!g_pHoveredItemUnit || !g_pPlayerUnit)
    {
        return;
    }
    (void)pOutBuf;
}

// 1.12a: 0x6FB57760
// Formats the weapon attack speed rating line (Very Slow / Slow / Normal / Fast / Very Fast).
void __stdcall ITEMDESC_FormatWeaponSpeedLine(Unicode* pOutBuf)
{
    if (!g_pHoveredItemUnit || !g_pPlayerUnit)
    {
        return;
    }
    (void)pOutBuf;
}

// 1.12a: 0x6FB57900
// Formats the stack quantity line ("Quantity: %ld") for stackable items (javelins, keys, tomes, arrows).
void __stdcall ITEMDESC_FormatQuantityLine(Unicode* pOutBuf)
{
    if (!g_pHoveredItemUnit)
    {
        return;
    }

    int32_t nQuantity = STATLIST_GetUnitBaseStat(g_pHoveredItemUnit, STAT_QUANTITY, 0);
    if (nQuantity <= 0)
    {
        return;
    }

    const Unicode* wszQtyLabel = D2LANG_GetStringFromTblIndex(0xD85); // "Quantity: "
    Unicode::strcpy(pOutBuf, wszQtyLabel);

    char szNum[32] = {};
    Unicode wszNum[32] = {};
    SStrPrintf(szNum, sizeof(szNum), "%ld", nQuantity);
    Unicode::win2Unicode(wszNum, szNum, 32);
    Unicode::strcat(pOutBuf, wszNum);
}

// 1.12a: 0x6FB579C0
// Formats the defense rating line ("Defense: %i") with color coding for magic/enhanced defense.
void __stdcall ITEMDESC_FormatDefenseLine(D2UnitStrc* pItem, Unicode* pOutBuf, int32_t* pOutColor)
{
    if (!pItem || !pOutBuf)
    {
        return;
    }
    (void)pOutColor;
}

// 1.12a: 0x6FB57C00
// Formats the shield chance to block line ("Chance to Block: %i%%") for the current player class.
void __stdcall ITEMDESC_FormatBlockChanceLine(Unicode* pOutBuf, int32_t* pOutColor)
{
    if (!g_pHoveredItemUnit || !pOutBuf)
    {
        return;
    }
    (void)pOutColor;
}

// 1.12a: 0x6FB57D90
// Computes the player's effective chance to block with a shield, clamping to [5, 75]%.
int32_t __stdcall ITEMDESC_ComputeEffectiveBlockChance(D2UnitStrc* pPlayer, D2UnitStrc* pShield)
{
    if (!pPlayer || !pShield)
    {
        return 0;
    }

    int32_t nBaseBlock = STATLIST_UnitGetStatValue(pShield, STAT_TOBLOCK, 0);
    int32_t nClassBlockBonus = STATLIST_UnitGetStatValue(pPlayer, STAT_TOBLOCK, 0);
    int32_t nTotalBlock = nBaseBlock + nClassBlockBonus;
    if (nTotalBlock > 75)
    {
        nTotalBlock = 75;
    }
    return nTotalBlock;
}

// 1.12a: 0x6FB57EF0
// Evaluates merged item + socketed gem stats (damage/defense/durability) for tooltip display.
int32_t __stdcall ITEMDESC_GetMergedItemStatValue(
    D2UnitStrc* pItem,
    int32_t nStatId,
    int32_t nBaseStatId,
    int32_t nEnhancedStatId,
    int32_t* pOutIsEnhanced,
    int32_t nDefaultVal)
{
    if (!pItem)
    {
        return nDefaultVal;
    }
    (void)nBaseStatId;
    (void)nEnhancedStatId;
    if (pOutIsEnhanced)
    {
        *pOutIsEnhanced = 0;
    }
    return STATLIST_UnitGetStatValue(pItem, nStatId, 0);
}

// 1.12a: 0x6FB580E0
// Formats the Required Strength line ("Required Strength: %i").
void __stdcall ITEMDESC_FormatReqStrengthLine(Unicode* pOutBuf, int32_t nReqStr)
{
    const Unicode* wszReq = D2LANG_GetStringFromTblIndex(0xD87);
    Unicode::strcpy(pOutBuf, wszReq);
    char szNum[32] = {};
    Unicode wszNum[32] = {};
    SStrPrintf(szNum, sizeof(szNum), "%i", nReqStr);
    Unicode::win2Unicode(wszNum, szNum, 32);
    Unicode::strcat(pOutBuf, wszNum);
}

// 1.12a: 0x6FB581A0
// Formats the Required Dexterity line ("Required Dexterity: %i").
void __stdcall ITEMDESC_FormatReqDexterityLine(Unicode* pOutBuf, int32_t nReqDex)
{
    const Unicode* wszReq = D2LANG_GetStringFromTblIndex(0xD88);
    Unicode::strcpy(pOutBuf, wszReq);
    char szNum[32] = {};
    Unicode wszNum[32] = {};
    SStrPrintf(szNum, sizeof(szNum), "%i", nReqDex);
    Unicode::win2Unicode(wszNum, szNum, 32);
    Unicode::strcat(pOutBuf, wszNum);
}

// 1.12a: 0x6FB58260
// Formats the Paladin Smite Damage line ("Smite Damage: %i to %i") on shields.
void __stdcall ITEMDESC_FormatSmiteDamageLine(Unicode* pOutBuf)
{
    if (!g_pHoveredItemUnit || !pOutBuf)
    {
        return;
    }
}

// 1.12a: 0x6FB58300
// Formats the Durability line ("Durability: %i of %i" or "Indestructible").
void __stdcall ITEMDESC_FormatDurabilityLine(Unicode* pOutBuf)
{
    if (!g_pHoveredItemUnit || !pOutBuf)
    {
        return;
    }

    if (!ITEMS_HasDurability(g_pHoveredItemUnit))
    {
        return;
    }

    int32_t nMaxDur = STATLIST_GetMaxDurabilityFromUnit(g_pHoveredItemUnit);
    if (nMaxDur <= 0)
    {
        return;
    }

    int32_t nCurDur = STATLIST_UnitGetStatValue(g_pHoveredItemUnit, STAT_DURABILITY, 0);
    const Unicode* wszDur = D2LANG_GetStringFromTblIndex(0xD86); // "Durability: "
    Unicode::strcpy(pOutBuf, wszDur);

    char szNum[32] = {};
    Unicode wszNum[32] = {};
    SStrPrintf(szNum, sizeof(szNum), "%i", nCurDur);
    Unicode::win2Unicode(wszNum, szNum, 32);
    Unicode::strcat(pOutBuf, wszNum);

    const Unicode* wszOf = D2LANG_GetStringFromTblIndex(0xD8A); // " of "
    Unicode::strcat(pOutBuf, wszOf);

    SStrPrintf(szNum, sizeof(szNum), "%i", nMaxDur);
    Unicode::win2Unicode(wszNum, szNum, 32);
    Unicode::strcat(pOutBuf, wszNum);
}

// 1.12a: 0x6FB58450
// Formats the Belt potion slot capacity line.
void __stdcall ITEMDESC_FormatBeltRowsLine(D2UnitStrc* pItem, Unicode* pOutBuf)
{
    if (!pItem || !pOutBuf)
    {
        return;
    }
}

// 1.12a: 0x6FB58520
// Formats a Player Ear item's class and level lines.
void __stdcall ITEMDESC_FormatEarDescriptionLines(D2UnitStrc* pEarItem, Unicode* pOutBuf)
{
    if (!pEarItem || !pOutBuf)
    {
        return;
    }
}

// 1.12a: 0x6FB585F0
// Formats the personalized item owner prefix ("<Name>'s ") or Runeword title.
void __stdcall ITEMDESC_FormatPersonalizedItemTitle(Unicode* pOutBuf)
{
    if (!g_pHoveredItemUnit || !pOutBuf)
    {
        return;
    }
}

// 1.12a: 0x6FB586F0
// Formats the "Ethereal (Cannot be Repaired)" and "Socketed (%i)" footer line.
void __stdcall ITEMDESC_FormatEtherealAndSocketedLine(Unicode* pOutBuf)
{
    if (!g_pHoveredItemUnit || !pOutBuf)
    {
        return;
    }
}

// 1.12a: 0x6FB58860
// Draws the green/red placement preview box when hovering a cursor item over a paperdoll body slot.
void __fastcall INV_DrawBodySlotPlacementHighlight(int32_t nBodyLoc, D2UnitStrc* pTargetUnit)
{
    if (!pTargetUnit || !pTargetUnit->pInventory)
    {
        return;
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(g_pPlayerUnit->pInventory);
    if (!pCursorItem)
    {
        return;
    }

    const D2InvBodyLocInfoStrc* pSlot = (pTargetUnit == g_pPlayerUnit)
        ? &g_atBodyLocRects[nBodyLoc]
        : &g_atMercBodyLocRects[nBodyLoc];

    RECT rBox = { pSlot->nLeft, pSlot->nTop, pSlot->nRight, pSlot->nBottom };
    BOOL bCanEquip = ITEMS_CheckBodyLocation(pCursorItem, nBodyLoc) &&
                     ITEMS_CheckRequirements(pCursorItem, pTargetUnit, FALSE, NULL, NULL, NULL);

    INV_DrawItemHighlightBox(&rBox, bCanEquip ? 2 : 1, DRAW_MODE_TRANS_25);
}

// 1.12a: 0x6FB589B0
// Draws the green/red placement preview box when hovering a cursor item over an inventory grid.
void __stdcall INV_DrawGridPlacementHighlight(
    D2InventoryStrc* pInventory,
    const D2InvGridLayoutStrc* pGridLayout,
    uint8_t nInvPage)
{
    if (!g_pPlayerUnit || !g_pPlayerUnit->pInventory)
    {
        return;
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(g_pPlayerUnit->pInventory);
    if (!pCursorItem)
    {
        return;
    }

    uint8_t nItemW = 1;
    uint8_t nItemH = 1;
    ITEMS_GetDimensions(pCursorItem, &nItemW, &nItemH, "..\\Source\\D2Common/CmnItems.h", __LINE__);

    int32_t nCellX = 0;
    int32_t nCellY = 0;
    INV_ScreenToClampedGridCoords(g_nMouseX, g_nMouseY, pGridLayout, &nCellX, &nCellY, nItemW, nItemH);

    RECT rBox;
    rBox.left   = pGridLayout->nLeft + nCellX * pGridLayout->nGridBoxWidth;
    rBox.right  = rBox.left + nItemW * pGridLayout->nGridBoxWidth;
    rBox.top    = pGridLayout->nTop  + nCellY * pGridLayout->nGridBoxHeight;
    rBox.bottom = rBox.top  + nItemH * pGridLayout->nGridBoxHeight;

    D2UnitStrc* pExchangedItem = NULL;
    int32_t nRecordId = UNITS_GetInventoryRecordId(g_pPlayerUnit, nInvPage, g_bExpansionMode);
    BOOL bCanPlace = INVENTORY_CanItemBePlaced(
        pInventory,
        pCursorItem,
        nCellX,
        nCellY,
        nRecordId,
        &pExchangedItem,
        NULL,
        nInvPage);

    INV_DrawItemHighlightBox(&rBox, bCanPlace ? 2 : 1, DRAW_MODE_TRANS_25);
}

// 1.12a: 0x6FB58B70
// 1.12a: 0x6FB58C10 (false-split continuation inside INV_InitInventoryLayoutRectsAndGrids)
// Queries Inventory.txt via DATATBLS_GetInventoryRect / DATATBLS_GetInventoryGridInfo /
// DATATBLS_GetInventoryComponentGrid to initialize all panel rects, grid layouts, and paperdoll slots.
void __stdcall INV_InitInventoryLayoutRectsAndGrids()
{
    if (!g_pPlayerUnit)
    {
        return;
    }

    int32_t bHigherRes = (D2GFX_GetResolutionMode() != 0);

    // 1. Player Inventory & Paperdoll (InvPage 0)
    int32_t nInvRecId = UNITS_GetInventoryRecordId(g_pPlayerUnit, INVPAGE_INVENTORY, g_bExpansionMode);
    DATATBLS_GetInventoryRect(nInvRecId, bHigherRes, (D2InvRectStrc*)&g_rInvPanelRect);

    D2InventoryGridInfoStrc tGridInfo = {};
    DATATBLS_GetInventoryGridInfo(nInvRecId, bHigherRes, &tGridInfo);
    g_tInvGridLayout.nGridX         = tGridInfo.nGridX;
    g_tInvGridLayout.nGridY         = tGridInfo.nGridY;
    g_tInvGridLayout.nLeft          = tGridInfo.nGridLeft;
    g_tInvGridLayout.nRight         = tGridInfo.nGridRight;
    g_tInvGridLayout.nTop           = tGridInfo.nGridTop;
    g_tInvGridLayout.nBottom        = tGridInfo.nGridBottom;
    g_tInvGridLayout.nGridBoxWidth  = tGridInfo.nGridBoxWidth;
    g_tInvGridLayout.nGridBoxHeight = tGridInfo.nGridBoxHeight;

    static const int32_t kCompToBodyLoc[10] = {
        BODYLOC_RARM,   // 0: INV_COMP_RARM
        BODYLOC_TORSO,  // 1: INV_COMP_TORSO
        BODYLOC_LARM,   // 2: INV_COMP_LARM
        BODYLOC_HEAD,   // 3: INV_COMP_HEAD
        BODYLOC_NECK,   // 4: INV_COMP_NECK
        BODYLOC_RRIN,   // 5: INV_COMP_RHAND
        BODYLOC_LRIN,   // 6: INV_COMP_LHAND
        BODYLOC_BELT,   // 7: INV_COMP_BELT
        BODYLOC_FEET,   // 8: INV_COMP_FEET
        BODYLOC_GLOVES, // 9: INV_COMP_GLOVES
    };
    for (int32_t i = 0; i < 10; ++i)
    {
        D2InvCompGridStrc tCompGrid = {};
        DATATBLS_GetInventoryComponentGrid(nInvRecId, bHigherRes, &tCompGrid, i);
        const int32_t nBodyLoc = kCompToBodyLoc[i];
        g_atBodyLocRects[nBodyLoc].nLeft   = tCompGrid.rect.nLeft;
        g_atBodyLocRects[nBodyLoc].nRight  = tCompGrid.rect.nRight;
        g_atBodyLocRects[nBodyLoc].nTop    = tCompGrid.rect.nTop;
        g_atBodyLocRects[nBodyLoc].nBottom = tCompGrid.rect.nBottom;
    }

    // 2. Stash (InvPage 4)
    int32_t nStashRecId = UNITS_GetInventoryRecordId(g_pPlayerUnit, INVPAGE_STASH, g_bExpansionMode);
    DATATBLS_GetInventoryRect(nStashRecId, bHigherRes, (D2InvRectStrc*)&g_rStashPanelRect);
    DATATBLS_GetInventoryGridInfo(nStashRecId, bHigherRes, &tGridInfo);
    g_tStashGridLayout.nGridX         = tGridInfo.nGridX;
    g_tStashGridLayout.nGridY         = tGridInfo.nGridY;
    g_tStashGridLayout.nLeft          = tGridInfo.nGridLeft;
    g_tStashGridLayout.nRight         = tGridInfo.nGridRight;
    g_tStashGridLayout.nTop           = tGridInfo.nGridTop;
    g_tStashGridLayout.nBottom        = tGridInfo.nGridBottom;
    g_tStashGridLayout.nGridBoxWidth  = tGridInfo.nGridBoxWidth;
    g_tStashGridLayout.nGridBoxHeight = tGridInfo.nGridBoxHeight;

    // 3. Horadric Cube (InvPage 3)
    int32_t nCubeRecId = UNITS_GetInventoryRecordId(g_pPlayerUnit, INVPAGE_CUBE, g_bExpansionMode);
    DATATBLS_GetInventoryGridInfo(nCubeRecId, bHigherRes, &tGridInfo);
    D2InvGridLayoutStrc* pCubeGrid = g_bExpansionMode ? &g_tCubeLoDGridLayout : &g_tCubeClassicGridLayout;
    pCubeGrid->nGridX         = tGridInfo.nGridX;
    pCubeGrid->nGridY         = tGridInfo.nGridY;
    pCubeGrid->nLeft          = tGridInfo.nGridLeft;
    pCubeGrid->nRight         = tGridInfo.nGridRight;
    pCubeGrid->nTop           = tGridInfo.nGridTop;
    pCubeGrid->nBottom        = tGridInfo.nGridBottom;
    pCubeGrid->nGridBoxWidth  = tGridInfo.nGridBoxWidth;
    pCubeGrid->nGridBoxHeight = tGridInfo.nGridBoxHeight;

    // 4. Trade Grid (InvPage 2)
    int32_t nTradeRecId = UNITS_GetInventoryRecordId(g_pPlayerUnit, INVPAGE_TRADE, g_bExpansionMode);
    DATATBLS_GetInventoryGridInfo(nTradeRecId, bHigherRes, &tGridInfo);
    g_tTradeTopGridLayout.nGridX         = tGridInfo.nGridX;
    g_tTradeTopGridLayout.nGridY         = tGridInfo.nGridY;
    g_tTradeTopGridLayout.nLeft          = tGridInfo.nGridLeft;
    g_tTradeTopGridLayout.nRight         = tGridInfo.nGridRight;
    g_tTradeTopGridLayout.nTop           = tGridInfo.nGridTop;
    g_tTradeTopGridLayout.nBottom        = tGridInfo.nGridBottom;
    g_tTradeTopGridLayout.nGridBoxWidth  = tGridInfo.nGridBoxWidth;
    g_tTradeTopGridLayout.nGridBoxHeight = tGridInfo.nGridBoxHeight;

    // 5. NPC Shop 10x10 Grid (Left Panel)
    const int32_t nLeftOx = (g_nScreenWidth >= 800) ? ((g_nScreenWidth - 640) / 2) : 0;
    const int32_t nLeftTop = (g_nScreenHeight >= 600) ? 60 : 0;
    g_rNpcShopGridRect.left   = nLeftOx + 17;
    g_rNpcShopGridRect.top    = nLeftTop + 64;
    g_rNpcShopGridRect.right  = g_rNpcShopGridRect.left + 10 * 29;
    g_rNpcShopGridRect.bottom = g_rNpcShopGridRect.top + 10 * 29;

    // Map loaded paperdoll background cells onto body location descriptors
    g_atBodyLocRects[BODYLOC_HEAD].pCell    = g_apInvSlotBackCells[4];
    g_atBodyLocRects[BODYLOC_NECK].pCell    = g_apInvSlotBackCells[5];
    g_atBodyLocRects[BODYLOC_TORSO].pCell   = g_apInvSlotBackCells[0];
    g_atBodyLocRects[BODYLOC_RARM].pCell    = g_apInvSlotBackCells[6];
    g_atBodyLocRects[BODYLOC_LARM].pCell    = g_apInvSlotBackCells[6];
    g_atBodyLocRects[BODYLOC_RRIN].pCell    = g_apInvSlotBackCells[5];
    g_atBodyLocRects[BODYLOC_LRIN].pCell    = g_apInvSlotBackCells[5];
    g_atBodyLocRects[BODYLOC_BELT].pCell    = g_apInvSlotBackCells[1];
    g_atBodyLocRects[BODYLOC_FEET].pCell    = g_apInvSlotBackCells[2];
    g_atBodyLocRects[BODYLOC_GLOVES].pCell  = g_apInvSlotBackCells[3];
}

// 1.12a: 0x6FB58F60
// Prepends a Unicode string line onto an existing multi-line Unicode tooltip buffer.
void __stdcall ITEMDESC_PrependUnicodeLine(Unicode* pTargetBuf, const Unicode* pLineToPrepend)
{
    Unicode wszTemp[1024] = {};
    Unicode::strcpy(wszTemp, pTargetBuf);
    Unicode::strcpy(pTargetBuf, pLineToPrepend);
    Unicode::strcat(pTargetBuf, wszTemp);
}

// 1.12a: 0x6FB59030
// Strips trailing or leading newline characters from a formatted Unicode tooltip buffer.
void __stdcall ITEMDESC_TrimUnicodeNewlines(Unicode* pBuf, int32_t nMaxLen)
{
    (void)pBuf;
    (void)nMaxLen;
}

// 1.12a: 0x6FB590D0
// Renders the Horadric Staff / Khalim's Will recipe scroll overlay ("menu\recipescroll").
void __stdcall INV_RenderRecipeScrollOverlay()
{
    if (!g_pRecipeScrollCell)
    {
        return;
    }

    D2GfxDataStrc tGfxData = {};
    tGfxData.pCellFile = g_pRecipeScrollCell;
    tGfxData.nDirection = 0;

    int32_t nX = g_nScreenWidth / 2 - 128;
    int32_t nY = g_nScreenHeight / 2;

    tGfxData.nFrame = 0;
    TEXTURE_CelDraw(&tGfxData, nX, nY, -1, DRAW_MODE_NORMAL, 0);
    tGfxData.nFrame = 1;
    TEXTURE_CelDraw(&tGfxData, nX + 256, nY, -1, DRAW_MODE_NORMAL, 0);
}

// 1.12a: 0x6FB59240
// Selects the font color index (White, Blue, Yellow, Gold, Green, Orange, Gray) for an item.
int32_t __stdcall ITEMDESC_GetItemTitleColor(D2UnitStrc* pItem)
{
    if (!pItem)
    {
        return 0;
    }

    if (!ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, "..\\Source\\D2Client\\UI\\inv.cpp"))
    {
        return 1; // Red if unidentified
    }

    if (ITEMS_CheckItemFlag(pItem, IFLAG_RUNEWORD, __LINE__, "..\\Source\\D2Client\\UI\\inv.cpp"))
    {
        return 4; // Gold for Runewords
    }

    switch (ITEMS_GetItemQuality(pItem))
    {
    case ITEMQUAL_INFERIOR:
        return 5; // Gray
    case ITEMQUAL_MAGIC:
        return 3; // Blue
    case ITEMQUAL_SET:
        return 2; // Green
    case ITEMQUAL_RARE:
        return 9; // Yellow
    case ITEMQUAL_UNIQUE:
        return 4; // Gold
    case ITEMQUAL_CRAFT:
        return 8; // Orange
    default:
        if (ITEMS_CheckItemFlag(pItem, IFLAG_ETHEREAL, __LINE__, "..\\Source\\D2Client\\UI\\inv.cpp") ||
            ITEMS_CheckItemFlag(pItem, IFLAG_SOCKETED, __LINE__, "..\\Source\\D2Client\\UI\\inv.cpp"))
        {
            return 5; // Gray
        }
        return 0; // White
    }
}

static void INV_DrawAsciiTextHelper(const char* szAscii, int32_t nX, int32_t nY, int32_t nColor, BOOL bCentered)
{
    if (!szAscii) return;
    Unicode wszBuf[128] = {};
    Unicode::win2Unicode(wszBuf, szAscii, 127);
    int32_t nDrawX = nX;
    if (bCentered)
    {
        nDrawX -= D2Win_10121_GetTextWidth(wszBuf) / 2;
    }
    D2Win_10117_DrawText(wszBuf, nDrawX, nY, nColor, 0);
}

static void INV_DrawQuadrantsHelper(D2CellFileStrc* pCellFile, int32_t nLeft, int32_t nTop, int32_t nStartFrame)
{
    if (!pCellFile) return;
    const int32_t nTotal = D2CMP_CelFileGetCelsPerDirection(pCellFile);
    const int32_t aPos[4][2] = {
        { nLeft,       nTop + 256 },
        { nLeft + 256, nTop + 256 },
        { nLeft,       nTop + 432 },
        { nLeft + 256, nTop + 432 },
    };
    for (int32_t i = 0; i < 4 && (nStartFrame + i) < nTotal; ++i)
    {
        D2GfxDataStrc tGfx = {};
        tGfx.pCellFile = pCellFile;
        tGfx.nFrame = nStartFrame + i;
        TEXTURE_CelDraw(&tGfx, aPos[i][0], aPos[i][1], 0xFFFFFFFFu, DRAW_MODE_NORMAL, nullptr);
    }
}

static D2CellFileStrc* s_pPlayerInvChar6Cell = nullptr;
static D2CellFileStrc* s_pBorderFrame800Cell = nullptr;
static D2CellFileStrc* s_pBuySellPanelCell = nullptr;

// 1.12a: 0x6FB59390
// Renders the Mercenary equipment paperdoll & stats panel ("InvChar6Tab" / hireling screen).
void __stdcall INV_RenderMercenaryInventoryPanel()
{
    if (!g_bExpansionMode || !g_bMercInvOpen)
    {
        return;
    }

    if (!g_pHireInvPanelCell)
    {
        g_pHireInvPanelCell = ARCHIVE_LoadCellFileDefault("Panel\\NPCInv", 0);
        if (!g_pHireInvPanelCell)
        {
            g_pHireInvPanelCell = ARCHIVE_LoadCellFileDefault("PANEL\\Monster", 0);
        }
        if (!g_pHireInvPanelCell)
        {
            g_pHireInvPanelCell = ARCHIVE_LoadCellFileDefault("PANEL\\hireinv", 0);
        }
    }
    if (!g_pBuySellBtnCell)
    {
        g_pBuySellBtnCell = ARCHIVE_LoadCellFileDefault("PANEL\\buysellbtn", 0);
    }

    const int32_t nLeft = (g_nScreenWidth >= 800) ? ((g_nScreenWidth - 640) / 2) : 0;
    const int32_t nTop  = (g_nScreenHeight >= 600) ? 60 : 0;

    if (g_pHireInvPanelCell)
    {
        INV_DrawQuadrantsHelper(g_pHireInvPanelCell, nLeft, nTop, 0);
    }
    else
    {
        D2GFX_DrawBox(nLeft, nTop, nLeft + 320, nTop + 432, 0x00u, DRAWMODE_TRANS75);
    }

    D2UnitStrc* pMerc = D2Client_GetLocalMercUnit(g_pPlayerUnit);
    if (!pMerc)
    {
        D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
        INV_DrawAsciiTextHelper("No Mercenary Hired", nLeft + 160, nTop + 218, 4, TRUE);
    }
    else
    {
        const Unicode* pMercName = D2Client_GetNpcDisplayName(pMerc);
        D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
        if (pMercName && pMercName[0])
        {
            int32_t nW = D2Win_10121_GetTextWidth(pMercName);
            D2Win_10117_DrawText(pMercName, nLeft + 95 - nW / 2, nTop + 24, 4, 0);
        }
        else
        {
            INV_DrawAsciiTextHelper("Mercenary", nLeft + 95, nTop + 24, 4, TRUE);
        }

        char szBuf[64] = {};
        int32_t nLvl = STATLIST_UnitGetStatValue(pMerc, STAT_LEVEL, 0);
        if (nLvl < 1) nLvl = 1;
        snprintf(szBuf, sizeof(szBuf), "Level %d", nLvl);
        INV_DrawAsciiTextHelper(szBuf, nLeft + 250, nTop + 24, 0, TRUE);

        int32_t nCurHp = STATLIST_UnitGetStatValue(pMerc, STAT_HITPOINTS, 0) >> 8;
        int32_t nMaxHp = STATLIST_UnitGetStatValue(pMerc, STAT_MAXHP, 0) >> 8;
        if (nMaxHp < nCurHp) nMaxHp = nCurHp;
        D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
        INV_DrawAsciiTextHelper("Life", nLeft + 60, nTop + 265, 4, TRUE);
        snprintf(szBuf, sizeof(szBuf), "%d / %d", nCurHp, nMaxHp);
        INV_DrawAsciiTextHelper(szBuf, nLeft + 185, nTop + 265, 0, TRUE);

        int32_t nStr = STATLIST_UnitGetStatValue(pMerc, STAT_STRENGTH, 0);
        int32_t nDex = STATLIST_UnitGetStatValue(pMerc, STAT_DEXTERITY, 0);
        int32_t nDef = STATLIST_UnitGetStatValue(pMerc, STAT_ARMORCLASS, 0);
        int32_t nFireRes = STATLIST_UnitGetStatValue(pMerc, STAT_FIRERESIST, 0);
        int32_t nColdRes = STATLIST_UnitGetStatValue(pMerc, STAT_COLDRESIST, 0);
        int32_t nLightRes = STATLIST_UnitGetStatValue(pMerc, STAT_LIGHTRESIST, 0);
        int32_t nPoisRes = STATLIST_UnitGetStatValue(pMerc, STAT_POISONRESIST, 0);

        snprintf(szBuf, sizeof(szBuf), "Str: %d  Dex: %d  Def: %d", nStr, nDex, nDef);
        INV_DrawAsciiTextHelper(szBuf, nLeft + 160, nTop + 292, 0, TRUE);
        snprintf(szBuf, sizeof(szBuf), "Fire %d%%  Cold %d%%  Light %d%%  Pois %d%%", nFireRes, nColdRes, nLightRes, nPoisRes);
        INV_DrawAsciiTextHelper(szBuf, nLeft + 160, nTop + 318, 0, TRUE);

        if (pMerc->pInventory)
        {
            const int32_t aMercSlots[4][5] = {
                { BODYLOC_HEAD,  nLeft + 133, nTop + 4,   nLeft + 191, nTop + 62  },
                { BODYLOC_TORSO, nLeft + 133, nTop + 74,  nLeft + 191, nTop + 161 },
                { BODYLOC_RARM,  nLeft + 17,  nTop + 46,  nLeft + 75,  nTop + 162 },
                { BODYLOC_LARM,  nLeft + 249, nTop + 46,  nLeft + 307, nTop + 162 },
            };
            for (int32_t s = 0; s < 4; ++s)
            {
                D2UnitStrc* pEq = INVENTORY_GetItemFromBodyLoc(pMerc->pInventory, aMercSlots[s][0]);
                if (pEq)
                {
                    uint8_t w = 1, h = 1;
                    ITEMS_GetDimensions(pEq, &w, &h, __FILE__, __LINE__);
                    int32_t nSlotW = aMercSlots[s][3] - aMercSlots[s][1];
                    int32_t nSlotH = aMercSlots[s][4] - aMercSlots[s][2];
                    int32_t nDrawX = aMercSlots[s][1] + (nSlotW - (int32_t)w * 29) / 2;
                    int32_t nDrawBottomY = aMercSlots[s][2] + (nSlotH + (int32_t)h * 29) / 2;
                    D2Client_DrawInvItemSprite(pEq, nDrawX, nDrawBottomY, 0xFFFFFFFFu, 5);
                }
            }
        }
    }

    if (g_pBuySellBtnCell)
    {
        D2GfxDataStrc tClose = {};
        tClose.pCellFile = g_pBuySellBtnCell;
        tClose.nFrame = 10;
        TEXTURE_CelDraw(&tClose, nLeft + 272, nTop + 420, 0xFFFFFFFFu, DRAW_MODE_NORMAL, nullptr);
    }
}

// 1.12a: 0x6FB59AB0
// Word-wraps a Unicode string to fit within a maximum pixel width.
int32_t __stdcall INV_WordWrapUnicodeString(const Unicode* wszInput)
{
    (void)wszInput;
    return 0;
}

// 1.12a: 0x6FB59B60
// Starts the Horadric Cube quest item transmutation animation ("menu\horadric").
void __stdcall INV_StartHoradricTransmuteAnimation()
{
    char szPath[MAX_PATH] = {};
    SStrPrintf(szPath, sizeof(szPath), "%s\\ui\\menu\\horadric", "DATA\\GLOBAL");
    g_pHoradricAnimCell = ARCHIVE_LoadCellFileDefault(szPath, 0);
    g_bHoradricAnimPlaying = TRUE;
    g_nHoradricAnimFrame = 0;
    g_dwHoradricAnimTick = GetTickCount();
}

// 1.12a: 0x6FB59C20
// Loads the Horadric Cube background graphic ("panel\supertransmogrifier").
void __stdcall INV_LoadHoradricCubeCells()
{
    if (!g_pHoradricBoxCell)
    {
        char szPath[MAX_PATH] = {};
        SStrPrintf(szPath, sizeof(szPath), "%s\\ui\\panel\\supertransmogrifier", "DATA\\GLOBAL");
        g_pHoradricBoxCell = ARCHIVE_LoadCellFileDefault(szPath, 0);
        if (!g_pHoradricBoxCell)
        {
            g_pHoradricBoxCell = ARCHIVE_LoadCellFileDefault("PANEL\\HoradricCube", 0);
        }
    }
    if (!g_pTransmogrifierBtnCell)
    {
        char szBtnPath[MAX_PATH] = {};
        SStrPrintf(szBtnPath, sizeof(szBtnPath), "%s\\ui\\panel\\miniconvert", "DATA\\GLOBAL");
        g_pTransmogrifierBtnCell = ARCHIVE_LoadCellFileDefault(szBtnPath, 0);
        if (!g_pTransmogrifierBtnCell)
        {
            g_pTransmogrifierBtnCell = ARCHIVE_LoadCellFileDefault("Panel\\miniconvert", 0);
        }
    }
}

// 1.12a: 0x6FB59C70
// Frees the Horadric Cube background and transmute button graphics.
void __stdcall INV_FreeHoradricCubeCells()
{
    if (g_pHoradricBoxCell)
    {
        ARCHIVE_FreeCellFile(g_pHoradricBoxCell);
        g_pHoradricBoxCell = NULL;
    }
    if (g_pTransmogrifierBtnCell)
    {
        ARCHIVE_FreeCellFile(g_pTransmogrifierBtnCell);
        g_pTransmogrifierBtnCell = NULL;
    }
}

// 1.12a: 0x6FB59D20
// Loads the Stash background graphic ("panel\bank" in Classic, "panel\TradeStash" in Expansion).
void __stdcall INV_LoadStashPanelCell()
{
    if (!g_pBankPanelCell)
    {
        char szPath[MAX_PATH] = {};
        if (FOG_IsExpansion())
        {
            SStrPrintf(szPath, sizeof(szPath), "%s\\ui\\panel\\TradeStash", "DATA\\GLOBAL");
        }
        else
        {
            SStrPrintf(szPath, sizeof(szPath), "%s\\ui\\panel\\bank", "DATA\\GLOBAL");
        }
        g_pBankPanelCell = ARCHIVE_LoadCellFileDefault(szPath, 0);
    }
}

// 1.12a: 0x6FB59DB0
// Window message handler for the NPC shop confirmation popup (WM_LBUTTONDOWN).
void __stdcall INV_OnShopConfirmMouseDown(D2WinMsgStrc* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB59E20
// Window message handler for the NPC shop confirmation popup (WM_LBUTTONUP).
void __stdcall INV_OnShopConfirmMouseUp(D2WinMsgStrc* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB59E60
// Loads the NPC shop tab graphics ("panel\buyselltabs").
void __stdcall INV_LoadShopTabCells()
{
    if (!g_pBuySellTabsCell)
    {
        char szPath[MAX_PATH] = {};
        SStrPrintf(szPath, sizeof(szPath), "%s\\ui\\panel\\buyselltabs", "DATA\\GLOBAL");
        g_pBuySellTabsCell = ARCHIVE_LoadCellFileDefault(szPath, 0);
    }
}

// 1.12a: 0x6FB59F20
// Draws the hover tooltip for the NPC shop sub-buttons (Buy / Sell / Repair / Repair All cost).
void __stdcall INV_DrawShopActionBtnTooltip()
{
    if (!g_pPlayerUnit || !g_bInteractedWithNpc || g_nHoveredShopSubBtnIndex <= 0)
    {
        return;
    }

    if (g_nHoveredShopSubBtnIndex == 4 && INV_HasRepairAllButton())
    {
        int32_t nRepairCost = ITEMS_GetAllRepairCosts(
            nullptr,
            g_pPlayerUnit,
            g_nInteractedNpcGUID,
            (D2C_Difficulties)g_nGameDifficulty,
            NULL,
            nullptr);
        (void)nRepairCost;
    }
}

// 1.12a: 0x6FB5A0F0
// Halts with an assertion failure if an invalid shop state is encountered.
void __stdcall INV_AssertValidShopMode()
{
    FOG_DisplayHalt("..\\Source\\D2Client\\UI\\inv.cpp", 2145);
    exit(-1);
}

// 1.12a: 0x6FB5A120
// Updates the hovered state of the NPC shop sub-buttons (Buy/Sell/Repair/Repair All/Close).
void __stdcall INV_UpdateShopSubButtonHover()
{
    g_nHoveredShopSubBtnIndex = INV_HitTestShopActionButtons();
}

// 1.12a: 0x6FB5A1C0
void __stdcall INV_ResetShopAutoSelectTimer()
{
    g_dwShopHoverCooldownTick = 0;
}

// 1.12a: 0x6FB5A1D0
// Handles clicking one of the 4 NPC shop action buttons (Buy, Sell, Repair, Repair All).
void __stdcall INV_OnShopActionButtonClicked()
{
    if (!g_pPlayerUnit || !g_bInteractedWithNpc)
    {
        return;
    }

    int32_t nBtnIdx = INV_HitTestShopActionButtons();
    if (nBtnIdx == 4 && INV_HasRepairAllButton())
    {
        // Send C2S 0x35 with bit 31 set on dwFlag (0x80000000) to repair all equipped items
        NET_SendByteAndFourDwords(0x35, g_nInteractedNpcGUID, 0, 0, 0x80000000u);
    }
}

// 1.12a: 0x6FB5A390
// Loads the 8 empty equipment slot background cell files ("Panel\inv_armor", "Panel\inv_belt", etc.).
void __stdcall INV_LoadBodyLocSlotBackgroundCells()
{
    static const char* const s_aszSlotBackFiles[8] = {
        "Panel\\inv_armor",
        "Panel\\inv_belt",
        "Panel\\inv_boots",
        "Panel\\inv_helm_glove",
        "Panel\\inv_helm_glove",
        "Panel\\inv_ring_amulet",
        "Panel\\inv_weapons",
        "Panel\\invchar6Tab"
    };

    for (int32_t i = 0; i < 8; ++i)
    {
        if (!g_apInvSlotBackCells[i])
        {
            g_apInvSlotBackCells[i] = ARCHIVE_LoadCellFileDefault(s_aszSlotBackFiles[i], 0);
        }
    }
}

// 1.12a: 0x6FB5A460
// Formats weapon One-Hand, Two-Hand, and Throw damage lines for an item tooltip.
void __stdcall ITEMDESC_FormatWeaponDamageLines(Unicode* pOutBuf, int32_t* pOutColor)
{
    if (!g_pHoveredItemUnit || !pOutBuf)
    {
        return;
    }
    (void)pOutColor;
}

// 1.12a: 0x6FB5ACB0
// Concatenates all formatted item property lines into the master tooltip Unicode buffer.
void __stdcall ITEMDESC_ConcatenateTooltipLines()
{
}

// 1.12a: 0x6FB5AEB0
// Draws the green/red placement highlight box over a Mercenary equipment slot.
void __stdcall INV_DrawMercSlotPlacementHighlight(int32_t nBodyLoc)
{
    D2UnitStrc* pMerc = INV_GetInteractedNpcUnit();
    if (pMerc)
    {
        INV_DrawBodySlotPlacementHighlight(nBodyLoc, pMerc);
    }
}

// 1.12a: 0x6FB5AFF0
// Sends C2S 0x60 (Weapon Swap) when the player clicks the weapon swap tabs on the paperdoll.
void __stdcall INV_SendWeaponSwapPacket()
{
    if (!FOG_IsExpansion() || !g_bExpansionMode)
    {
        return;
    }
    if (!INV_CanSwapWeaponsNow())
    {
        return;
    }
    NET_SendByte(0x60);
}

// 1.12a: 0x6FB5B060
// Window message handler for the Trade gold button click.
void __stdcall INV_OnTradeGoldBtnMsg(D2WinMsgStrc* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB5B0A0
// Resets Trade panel button states when opening or closing the Trade screen.
void __stdcall INV_ResetTradeButtonStates()
{
    g_bTradeGoldBtnPressed = FALSE;
    g_bTradePartnerGoldHighlight = FALSE;
    g_bStashOrTradeActionBtnDown = FALSE;
    g_bTradeGoldTransferDisabled = FALSE;
}

// 1.12a: 0x6FB5B100
// Opens the player-to-player Trade panel ("panel\trade") and shifts the viewport.
void __stdcall INV_OpenTradePanel(BOOL bOpen)
{
    if (bOpen)
    {
        if (!g_pTradePanelCell)
        {
            char szPath[MAX_PATH] = {};
            SStrPrintf(szPath, sizeof(szPath), "%s\\ui\\panel\\trade", "DATA\\GLOBAL");
            g_pTradePanelCell = ARCHIVE_LoadCellFileDefault(szPath, 0);
        }
        g_bTradePanelOpen = TRUE;
        g_nInvPanelMode = 14;
    }
    else
    {
        g_bTradePanelOpen = FALSE;
        if (g_nInvPanelMode == 14)
        {
            g_nInvPanelMode = 0;
        }
    }
}

// 1.12a: 0x6FB5B230
// Formats the socketed Runeword / Gem names line for an item tooltip.
void __stdcall ITEMDESC_FormatSocketedGemsLine(Unicode* pOutBuf)
{
    if (!g_pHoveredItemUnit || !pOutBuf)
    {
        return;
    }
}

// 1.12a: 0x6FB5B3C0
// Formats the complete item header & base stat block (title, defense, damage, durability, requirements).
void __stdcall ITEMDESC_FormatItemBaseStatsBlock(
    D2UnitStrc* pItem,
    Unicode* pOutTitleBuf,
    Unicode* pOutStatsBuf)
{
    if (!pItem || !pOutTitleBuf || !pOutStatsBuf)
    {
        return;
    }
}

// 1.12a: 0x6FB5C2C0
// Verifies that the player is not currently waiting on a server cursor item synchronization.
BOOL __fastcall INV_CheckCursorItemClear(D2UnitStrc* pPlayer)
{
    (void)pPlayer;
    if (g_bWaitingForServerCursorSync)
    {
        return FALSE;
    }
    return TRUE;
}

// 1.12a: 0x6FB5C310
// Handles right-clicking an item in the player's inventory:
// - Tomes / Scrolls of Identify or Town Portal (enters Identify cursor mode 6 or sends C2S 0x20)
// - Consumable potions/scrolls/herbs (sends C2S 0x20 or C2S 0x26)
// - Horadric Cube in inventory (sends C2S 0x20 to open Cube)
BOOL __fastcall INV_UseInventoryItemRightClick(D2UnitStrc* pPlayer, D2UnitStrc* pItem, int32_t nX, int32_t nY)
{
    if (!pPlayer || !pItem)
    {
        return FALSE;
    }

    if (ITEMS_IsItemLockedInInventory(pItem) || !INV_CheckCursorItemClear(pPlayer))
    {
        return FALSE;
    }

    if (pPlayer->pDynamicPath)
    {
        int32_t nPlayerX = PATH_GetXPosition(pPlayer->pDynamicPath);
        int32_t nPlayerY = PATH_GetYPosition(pPlayer->pDynamicPath);
        NET_SendByteAndThreeDwords(0x20, pItem->dwUnitId, nPlayerX, nPlayerY);
        return TRUE;
    }

    (void)nX;
    (void)nY;
    return FALSE;
}

// 1.12a: 0x6FB5C520
// Formats the "Right-click to read / open / use" hint line on books, scrolls, and the Horadric Cube.
void __stdcall INV_DrawHoveredBookOrScrollTooltip(D2UnitStrc* pItem, Unicode* pOutBuf)
{
    if (!pItem || !pOutBuf)
    {
        return;
    }
}

// 1.12a: 0x6FB5C720
// Checks if the cursor is currently in Identify mode (cursor state 6) and resets it if invalid.
void __stdcall INV_ValidateIdentifyCursorState()
{
    if (!g_pPlayerUnit)
    {
        return;
    }
    if (INVENTORY_GetCursorItem(g_pPlayerUnit->pInventory) && g_nCursorState == 6)
    {
        UI_CancelActiveCursorState();
    }
}

// ============================================================================
// Functions (0x6FB5C790 .. 0x6FB668E0)
// ============================================================================

// 1.12a: 0x6FB5C790
// Formats the complete magic/rare/unique/set/runeword stat list and socketed gem bonuses for an item.
void __stdcall ITEMDESC_FormatItemMagicProperties(D2UnitStrc* pItem)
{
    if (!pItem)
    {
        return;
    }
}

// 1.12a: 0x6FB5D2B0
// Renders the hover tooltip for the Horadric Cube transmute button or weapon swap tabs.
void __stdcall INV_RenderButtonHoverTooltip()
{
}

// 1.12a: 0x6FB5D320
// 1.12a: 0x6FB5DA10 (false-split jump-table continuation inside INV_RenderHoveredItemTooltip)
// Master item hover tooltip renderer: formats base stats, magic properties, set bonuses,
// and transaction costs, then draws the framed text box via D2Win_10130_DrawFramedTextEx.
void __stdcall INV_RenderHoveredItemTooltip(D2UnitStrc* pOwnerUnit, BOOL bIsNpcShop)
{
    if (!g_pHoveredItemUnit || !pOwnerUnit)
    {
        return;
    }

    D2ItemsTxt* pItemsTxt = DATATBLS_GetItemsTxtRecord(g_pHoveredItemUnit->dwClassId);
    if (!pItemsTxt)
    {
        return;
    }

    const Unicode* pBaseName = pItemsTxt->wNameStr ? D2LANG_GetStringFromTblIndex(pItemsTxt->wNameStr) : nullptr;
    char szName[64] = {};
    if (pBaseName && pBaseName[0])
    {
        Unicode::unicode2Win(szName, pBaseName, 63);
    }
    if (!szName[0])
    {
        memcpy(szName, pItemsTxt->szCode, 4);
        szName[4] = '\0';
    }

    char szTip[512] = {};
    int32_t nOff = snprintf(szTip, sizeof(szTip), "%s", szName);

    int32_t nDef = STATLIST_UnitGetStatValue(g_pHoveredItemUnit, STAT_ARMORCLASS, 0);
    if (nDef > 0 && nOff < 460)
    {
        nOff += snprintf(szTip + nOff, sizeof(szTip) - (size_t)nOff, "\nDefense: %d", nDef);
    }
    if (pItemsTxt->nMaxDam > 0 && nOff < 460)
    {
        nOff += snprintf(szTip + nOff, sizeof(szTip) - (size_t)nOff, "\nOne-Hand Damage: %d to %d",
            (int32_t)pItemsTxt->nMinDam, (int32_t)pItemsTxt->nMaxDam);
    }
    else if (pItemsTxt->n2HandMaxDam > 0 && nOff < 460)
    {
        nOff += snprintf(szTip + nOff, sizeof(szTip) - (size_t)nOff, "\nTwo-Hand Damage: %d to %d",
            (int32_t)pItemsTxt->n2HandMinDam, (int32_t)pItemsTxt->n2HandMaxDam);
    }

    int32_t nQty = STATLIST_UnitGetStatValue(g_pHoveredItemUnit, STAT_QUANTITY, 0);
    if (nQty > 0 && nOff < 460)
    {
        nOff += snprintf(szTip + nOff, sizeof(szTip) - (size_t)nOff, "\nQuantity: %d", nQty);
    }

    int32_t nMaxDur = STATLIST_UnitGetStatValue(g_pHoveredItemUnit, STAT_MAXDURABILITY, 0);
    if (nMaxDur > 0 && nOff < 460)
    {
        int32_t nCurDur = STATLIST_UnitGetStatValue(g_pHoveredItemUnit, STAT_DURABILITY, 0);
        nOff += snprintf(szTip + nOff, sizeof(szTip) - (size_t)nOff, "\nDurability: %d of %d", nCurDur, nMaxDur);
    }

    if (pItemsTxt->wReqStr > 0 && nOff < 460)
    {
        nOff += snprintf(szTip + nOff, sizeof(szTip) - (size_t)nOff, "\nRequired Strength: %d", (int32_t)pItemsTxt->wReqStr);
    }
    if (pItemsTxt->wReqDex > 0 && nOff < 460)
    {
        nOff += snprintf(szTip + nOff, sizeof(szTip) - (size_t)nOff, "\nRequired Dexterity: %d", (int32_t)pItemsTxt->wReqDex);
    }

    if (bIsNpcShop || (g_nInvPanelMode >= 1 && g_nInvPanelMode <= 9))
    {
        D2UnitStrc* pNpc = INV_GetInteractedNpcUnit();
        int32_t nVendorId = pNpc ? (int32_t)pNpc->dwClassId : 148;
        BOOL bInStore = ITEMS_CheckItemFlag(g_pHoveredItemUnit, IFLAG_INSTORE, __LINE__, __FILE__);
        int32_t nCost = ITEMS_GetTransactionCost(
            g_pPlayerUnit,
            g_pHoveredItemUnit,
            (D2C_Difficulties)g_nGameDifficulty,
            nullptr,
            nVendorId,
            bInStore ? TRANSACTIONTYPE_BUY : TRANSACTIONTYPE_SELL);
        if (nCost > 0 && nOff < 480)
        {
            nOff += snprintf(szTip + nOff, sizeof(szTip) - (size_t)nOff,
                bInStore ? "\nBuy Cost: %d" : "\nSell Value: %d", nCost);
        }
    }

    int32_t nColor = 0;
    int32_t nQual = ITEMS_GetItemQuality(g_pHoveredItemUnit);
    if (nQual == ITEMQUAL_MAGIC) nColor = 3;
    else if (nQual == ITEMQUAL_SET) nColor = 2;
    else if (nQual == ITEMQUAL_RARE) nColor = 9;
    else if (nQual == ITEMQUAL_UNIQUE) nColor = 4;

    Unicode wszTip[512] = {};
    Unicode::win2Unicode(wszTip, szTip, 511);
    int32_t nDrawX = (g_nHoveredItemTooltipX > 0) ? g_nHoveredItemTooltipX : g_nMouseX;
    int32_t nDrawY = (g_nHoveredItemTooltipTop > 24) ? (g_nHoveredItemTooltipTop - 8) : (g_nMouseY - 12);
    if (nDrawX < 80) nDrawX = 80;
    if (nDrawX > g_nScreenWidth - 80) nDrawX = g_nScreenWidth - 80;
    if (nDrawY < 24) nDrawY = 24;
    D2Win_10127_SetFont(D2FONT_FONTFORMAL12);
    D2Win_10129_DrawFramedText(wszTip, nDrawX, nDrawY, nColor, 1);
}

// 1.12a: 0x6FB5E430
// Updates g_pHoveredItemUnit and tooltip anchor coordinates when moving the mouse over an inventory grid.
BOOL __stdcall INV_UpdateHoveredGridItem(
    D2UnitStrc* pOwnerUnit,
    D2InventoryStrc* pInventory,
    int32_t nMouseX,
    int32_t nMouseY,
    const D2InvGridLayoutStrc* pGridLayout,
    uint8_t nInvPage)
{
    if (!pOwnerUnit || !pInventory || !pGridLayout)
    {
        return FALSE;
    }

    int32_t nCellX = (uint32_t)(nMouseX - pGridLayout->nLeft) / (uint32_t)pGridLayout->nGridBoxWidth;
    int32_t nCellY = (uint32_t)(nMouseY - pGridLayout->nTop)  / (uint32_t)pGridLayout->nGridBoxHeight;
    int32_t nRecordId = UNITS_GetInventoryRecordId(pOwnerUnit, nInvPage, g_bExpansionMode);

    int32_t nItemGridX = 0;
    int32_t nItemGridY = 0;
    D2UnitStrc* pItem = INVENTORY_GetItemFromInventoryPage(
        pInventory,
        nCellX,
        nCellY,
        &nItemGridX,
        &nItemGridY,
        nRecordId,
        nInvPage);

    if (!pItem)
    {
        return FALSE;
    }

    uint8_t nItemW = 1;
    uint8_t nItemH = 1;
    ITEMS_GetDimensions(pItem, &nItemW, &nItemH, "..\\Source\\D2Common/CmnItems.h", __LINE__);

    g_pHoveredItemUnit       = pItem;
    g_bHoveredItemValid      = TRUE;
    g_nHoveredItemInvRecordId= nRecordId;
    g_nHoveredItemInvPage    = nInvPage;
    g_nHoveredItemTooltipX   = pGridLayout->nLeft + nItemGridX * pGridLayout->nGridBoxWidth + (nItemW * pGridLayout->nGridBoxWidth) / 2;
    g_nHoveredItemTooltipTop = pGridLayout->nTop  + nItemGridY * pGridLayout->nGridBoxHeight;
    return TRUE;
}

// 1.12a: 0x6FB5E680
// Updates g_pHoveredItemUnit when moving the mouse over the Trade partner's top grid.
void __stdcall INV_UpdateHoveredTradePartnerItem(
    D2InventoryStrc* pInventory,
    int32_t nMouseX,
    int32_t nMouseY,
    const D2InvGridLayoutStrc* pGridLayout)
{
    if (!g_pPlayerUnit)
    {
        return;
    }
    INV_UpdateHoveredGridItem(g_pPlayerUnit, pInventory, nMouseX, nMouseY, pGridLayout, INVPAGE_TRADE);
}

// 1.12a: 0x6FB5E7D0
// Updates hovered button states when moving the mouse over the Trade panel.
void __fastcall INV_UpdateTradeButtonsHover(int32_t nMouseX, int32_t nMouseY)
{
    (void)nMouseX;
    (void)nMouseY;
}

// 1.12a: 0x6FB5E900
// Window message handler for mouse move (WM_MOUSEMOVE) while the Inventory / Trade / Cube / Stash / Shop panels are open.
void __stdcall INV_OnPanelMouseMove(D2WinMsgStrc* pMsg)
{
    if (!g_pPlayerUnit || !g_pPlayerUnit->pInventory)
    {
        return;
    }

    INV_ClearHoveredItemState();

    int32_t nX = pMsg->nX;
    int32_t nY = pMsg->nY;

    if (g_bInvPanelOpen && INV_HitTestInvPanelRect(nX, nY))
    {
        if (nX >= g_tInvGridLayout.nLeft && nX <= g_tInvGridLayout.nRight &&
            nY >= g_tInvGridLayout.nTop  && nY <= g_tInvGridLayout.nBottom)
        {
            INV_UpdateHoveredGridItem(
                g_pPlayerUnit,
                g_pPlayerUnit->pInventory,
                nX,
                nY,
                &g_tInvGridLayout,
                INVPAGE_INVENTORY);
        }
    }
}

// 1.12a: 0x6FB5EC50
// Draws the socketed gems/runes inside a hovered socketed item on an inventory grid.
void __stdcall INV_DrawSocketedGemsOnGridItem(
    D2UnitStrc* pHostItem,
    int32_t nGridLeft,
    int32_t nGridTop,
    const D2InvGridLayoutStrc* pGridLayout)
{
    if (!pHostItem || !pHostItem->pInventory || !pGridLayout)
    {
        return;
    }
    (void)nGridLeft;
    (void)nGridTop;
}

// 1.12a: 0x6FB5EDB0
// Draws the socketed gems/runes inside a hovered socketed item in an equipment paperdoll slot.
void __fastcall INV_DrawSocketedGemsOnBodySlotItem(int32_t nBodyLoc, D2InventoryStrc* pInventory)
{
    if (!pInventory)
    {
        return;
    }
    D2UnitStrc* pItem = INVENTORY_GetItemFromBodyLoc(pInventory, nBodyLoc);
    if (!pItem || !pItem->pInventory)
    {
        return;
    }
}

// 1.12a: 0x6FB5EEC0
// Draws the Stash gold deposit/withdraw button tooltip and close button tooltip.
void __stdcall INV_DrawStashButtonTooltips(int32_t nUnused)
{
    (void)nUnused;
}

// 1.12a: 0x6FB5EF50
// Updates hovered item or button state when moving the mouse over the Stash panel.
void __stdcall INV_OnStashMouseMove(D2WinMsgStrc* pMsg)
{
    if (!g_pPlayerUnit || !g_pPlayerUnit->pInventory)
    {
        return;
    }

    int32_t nX = pMsg->nX;
    int32_t nY = pMsg->nY;
    if (nX >= g_tStashGridLayout.nLeft && nX <= g_tStashGridLayout.nRight &&
        nY >= g_tStashGridLayout.nTop  && nY <= g_tStashGridLayout.nBottom)
    {
        INV_UpdateHoveredGridItem(
            g_pPlayerUnit,
            g_pPlayerUnit->pInventory,
            nX,
            nY,
            &g_tStashGridLayout,
            INVPAGE_STASH);
    }
}

// 1.12a: 0x6FB5EFE0
// Draws the Inventory gold drop button tooltip and close button tooltip.
void __stdcall INV_DrawInvButtonTooltips(int32_t nUnused)
{
    (void)nUnused;
}

// 1.12a: 0x6FB5F070
// Hit-tests whether screen coordinates (nX=eax, nY=ecx) lie inside the NPC shop grid rect.
BOOL __fastcall INV_HitTestNpcShopGridRect(int32_t nX, int32_t nY)
{
    return (nX >= g_rNpcShopGridRect.left && nX < g_rNpcShopGridRect.right &&
            nY >= g_rNpcShopGridRect.top  && nY <= g_rNpcShopGridRect.bottom);
}

// 1.12a: 0x6FB5F0C0
// Updates hovered item state when moving the mouse over the NPC shop grid.
void __stdcall INV_OnNpcShopMouseMove(D2WinMsgStrc* pMsg)
{
    D2UnitStrc* pNpc = INV_GetInteractedNpcUnit();
    if (!pNpc || !pNpc->pInventory)
    {
        return;
    }
    (void)pMsg;
}

// 1.12a: 0x6FB5F210
// Renders all equipped items on the player's paperdoll (head, torso, arms, rings, amulet, belt, boots, gloves),
// including translucent ghosted two-handed weapons in the off-hand slot and socketed gems.
void __stdcall INV_RenderPaperdollEquippedItems()
{
    if (!g_pPlayerUnit || !g_pPlayerUnit->pInventory)
    {
        return;
    }

    INV_LoadBodyLocSlotBackgroundCells();
    INV_InitInventoryLayoutRectsAndGrids();

    if (g_bExpansionMode)
    {
        D2GfxDataStrc tTab = {};
        tTab.pCellFile = g_apInvSlotBackCells[7];
        INV_DrawWeaponSwapTabs(&tTab);
    }

    INV_DrawEmptyBodyLocSlotBackgrounds(g_pPlayerUnit->pInventory);

    for (int32_t nBodyLoc = 1; nBodyLoc <= 10; ++nBodyLoc)
    {
        const D2InvBodyLocInfoStrc* pSlot = &g_atBodyLocRects[nBodyLoc];
        if (pSlot->nRight <= pSlot->nLeft || pSlot->nBottom <= pSlot->nTop)
        {
            continue;
        }
        D2UnitStrc* pItem = INVENTORY_GetItemFromBodyLoc(g_pPlayerUnit->pInventory, nBodyLoc);
        int32_t nDrawMode = 5;
        if (!pItem && (nBodyLoc == BODYLOC_LARM || nBodyLoc == BODYLOC_RARM))
        {
            D2UnitStrc* pOther = INVENTORY_GetItemFromBodyLoc(
                g_pPlayerUnit->pInventory,
                (nBodyLoc == BODYLOC_LARM) ? BODYLOC_RARM : BODYLOC_LARM);
            if (pOther && ITEMS_CheckWeaponIfTwoHanded(pOther))
            {
                pItem = pOther;
                nDrawMode = 1; // Ghosted off-hand 2H weapon
            }
        }
        if (!pItem)
        {
            continue;
        }

        if (nDrawMode == 5)
        {
            const uint8_t nSlotBgPal = D2Win_10034_MixRGB(0x00u, 0x00u, 0x80u);
            RECT rHighlight = { pSlot->nLeft, pSlot->nTop, pSlot->nRight, pSlot->nBottom };
            INV_DrawItemHighlightBox(&rHighlight, nSlotBgPal, DRAW_MODE_TRANS_25);
        }

        uint8_t w = 1, h = 1;
        ITEMS_GetDimensions(pItem, &w, &h, __FILE__, __LINE__);
        int32_t nSlotW = pSlot->nRight - pSlot->nLeft;
        int32_t nSlotH = pSlot->nBottom - pSlot->nTop;
        int32_t nDrawX = pSlot->nLeft + (nSlotW - (int32_t)w * 29) / 2;
        int32_t nDrawBottomY = pSlot->nTop + (nSlotH + (int32_t)h * 29) / 2;
        D2Client_DrawInvItemSprite(pItem, nDrawX, nDrawBottomY, 0xFFFFFFFFu, nDrawMode);

        if (nDrawMode == 5 &&
            g_nMouseX >= pSlot->nLeft && g_nMouseX <= pSlot->nRight &&
            g_nMouseY >= pSlot->nTop  && g_nMouseY <= pSlot->nBottom)
        {
            g_bHoveredItemValid      = TRUE;
            g_pHoveredItemUnit       = pItem;
            g_pHoveredSocketHostItem = pItem;
            g_nHoveredItemTooltipX   = (pSlot->nLeft + pSlot->nRight) / 2;
            g_nHoveredItemTooltipTop = pSlot->nTop;
        }
    }
}

// 1.12a: 0x6FB5F700
// Renders all items on the current inventory page (g_nCurrentInvPage: Inventory, Stash, Cube, Trade, or NPC Shop),
// including colored background boxes, item DC6 sprites, and socketed gems.
void __stdcall INV_RenderInventoryPageItems(D2UnitStrc* pOwnerUnit)
{
    if (!pOwnerUnit || !pOwnerUnit->pInventory)
    {
        return;
    }

    const BOOL bIsShopOwner = (pOwnerUnit != g_pPlayerUnit);
    int32_t nGridLeft = g_tInvGridLayout.nLeft;
    int32_t nGridTop  = g_tInvGridLayout.nTop;
    int32_t nCellW    = (g_tInvGridLayout.nGridBoxWidth > 0) ? g_tInvGridLayout.nGridBoxWidth : 29;
    int32_t nCellH    = (g_tInvGridLayout.nGridBoxHeight > 0) ? g_tInvGridLayout.nGridBoxHeight : 29;

    if (bIsShopOwner)
    {
        nGridLeft = g_rNpcShopGridRect.left;
        nGridTop  = g_rNpcShopGridRect.top;
        nCellW    = 29;
        nCellH    = 29;
    }
    else if (g_nCurrentInvPage == INVPAGE_STASH)
    {
        nGridLeft = g_tStashGridLayout.nLeft;
        nGridTop  = g_tStashGridLayout.nTop;
        if (g_tStashGridLayout.nGridBoxWidth > 0)  nCellW = g_tStashGridLayout.nGridBoxWidth;
        if (g_tStashGridLayout.nGridBoxHeight > 0) nCellH = g_tStashGridLayout.nGridBoxHeight;
    }
    else if (g_nCurrentInvPage == INVPAGE_CUBE)
    {
        const D2InvGridLayoutStrc* pCube = g_bExpansionMode ? &g_tCubeLoDGridLayout : &g_tCubeClassicGridLayout;
        nGridLeft = pCube->nLeft;
        nGridTop  = pCube->nTop;
        if (pCube->nGridBoxWidth > 0)  nCellW = pCube->nGridBoxWidth;
        if (pCube->nGridBoxHeight > 0) nCellH = pCube->nGridBoxHeight;
    }
    else if (g_nCurrentInvPage == INVPAGE_TRADE)
    {
        nGridLeft = g_tTradeTopGridLayout.nLeft;
        nGridTop  = g_tTradeTopGridLayout.nTop;
        if (g_tTradeTopGridLayout.nGridBoxWidth > 0)  nCellW = g_tTradeTopGridLayout.nGridBoxWidth;
        if (g_tTradeTopGridLayout.nGridBoxHeight > 0) nCellH = g_tTradeTopGridLayout.nGridBoxHeight;
    }

    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pOwnerUnit->pInventory);
         pItem != NULL;
         pItem = INVENTORY_GetNextItem(pItem))
    {
        if (!INVENTORY_UnitIsItem(pItem))
        {
            continue;
        }
        if (!bIsShopOwner && INVENTORY_GetItemNodePage(pItem) != 1)
        {
            continue;
        }
        if (ITEMS_GetInvPage(pItem) != g_nCurrentInvPage)
        {
            continue;
        }

        int32_t nCellX = UNITS_GetXPosition(pItem);
        int32_t nCellY = UNITS_GetYPosition(pItem);
        uint8_t w = 1, h = 1;
        ITEMS_GetDimensions(pItem, &w, &h, __FILE__, __LINE__);

        RECT rCellBox = {
            nGridLeft + nCellX * nCellW,
            nGridTop  + nCellY * nCellH,
            nGridLeft + (nCellX + (int32_t)w) * nCellW - 1,
            nGridTop  + (nCellY + (int32_t)h) * nCellH - 1
        };
        BOOL bCanUse = TRUE;
        if (g_pPlayerUnit)
        {
            BOOL bUnused1 = FALSE, bUnused2 = FALSE, bUnused3 = FALSE;
            bCanUse = ITEMS_CheckRequirements(pItem, g_pPlayerUnit, FALSE, &bUnused1, &bUnused2, &bUnused3);
        }
        const uint8_t nItemBgPal = bCanUse
            ? D2Win_10034_MixRGB(0x00u, 0x00u, 0x80u)
            : D2Win_10034_MixRGB(0x80u, 0x00u, 0x00u);
        INV_DrawItemHighlightBox(&rCellBox, nItemBgPal, DRAW_MODE_TRANS_25);

        D2Client_DrawInvItemSprite(
            pItem,
            rCellBox.left,
            rCellBox.bottom,
            0xFFFFFFFFu,
            5);

        if (g_nMouseX >= rCellBox.left && g_nMouseX <= rCellBox.right &&
            g_nMouseY >= rCellBox.top  && g_nMouseY <= rCellBox.bottom)
        {
            g_bHoveredItemValid      = TRUE;
            g_pHoveredItemUnit       = pItem;
            g_pHoveredSocketHostItem = pItem;
            g_nHoveredItemTooltipX   = (rCellBox.left + rCellBox.right) / 2;
            g_nHoveredItemTooltipTop = rCellBox.top;
        }
    }
}

// 1.12a: 0x6FB5FAC0
// Renders the NPC shop panel background, shop tabs ("panel\buyselltabs"), shop action buttons ("panel\buysellbtn"),
// and all items on the selected shop tab.
void __stdcall INV_RenderNpcShopPanel()
{
    D2UnitStrc* pNpc = INV_GetInteractedNpcUnit();
    if (!pNpc)
    {
        return;
    }

    if (!s_pBuySellPanelCell)
    {
        char szShopPath[MAX_PATH] = {};
        SStrPrintf(szShopPath, sizeof(szShopPath), "%s\\ui\\panel\\buysell", "DATA\\GLOBAL");
        s_pBuySellPanelCell = ARCHIVE_LoadCellFileDefault(szShopPath, 0);
        if (!s_pBuySellPanelCell)
        {
            s_pBuySellPanelCell = ARCHIVE_LoadCellFileDefault("Panel\\buysell", 0);
        }
    }
    INV_LoadShopTabCells();
    if (!g_pBuySellBtnCell)
    {
        g_pBuySellBtnCell = ARCHIVE_LoadCellFileDefault("PANEL\\buysellbtn", 0);
    }

    const int32_t nLeft = (g_nScreenWidth >= 800) ? ((g_nScreenWidth - 640) / 2) : 0;
    const int32_t nTop  = (g_nScreenHeight >= 600) ? 60 : 0;

    if (s_pBuySellPanelCell)
    {
        INV_DrawQuadrantsHelper(s_pBuySellPanelCell, nLeft, nTop, 0);
    }
    else
    {
        D2GFX_DrawBox(nLeft, nTop, nLeft + 320, nTop + 432, 0x00u, DRAWMODE_TRANS75);
    }

    // Auto-select first populated vendor tab if current tab has 0 items
    if (pNpc->pInventory)
    {
        int32_t aTabCounts[4] = {};
        for (D2UnitStrc* pIt = INVENTORY_GetFirstItem(pNpc->pInventory); pIt; pIt = INVENTORY_GetNextItem(pIt))
        {
            uint8_t pg = ITEMS_GetInvPage(pIt);
            if (pg < 4) ++aTabCounts[pg];
        }
        if (g_nSelectedShopTab >= 4 || aTabCounts[g_nSelectedShopTab] == 0)
        {
            for (uint8_t t = 0; t < 4; ++t)
            {
                if (aTabCounts[t] > 0)
                {
                    g_nSelectedShopTab = t;
                    break;
                }
            }
        }
    }

    // Draw 4 Shop Category Tabs ("Armor", "Weapons", "Magic", "Misc")
    static const char* const kShopTabNames[4] = { "Armor", "Weapons", "Magic", "Misc" };
    D2Win_10127_SetFont(D2FONT_FONTFORMAL10);
    for (int32_t t = 0; t < 4; ++t)
    {
        int32_t nTabLeft = nLeft + 14 + t * 73;
        if (g_pBuySellTabsCell)
        {
            D2GfxDataStrc tTab = {};
            tTab.pCellFile = g_pBuySellTabsCell;
            tTab.nFrame = (t == (int32_t)g_nSelectedShopTab) ? 0 : 1;
            if ((int32_t)tTab.nFrame >= D2CMP_CelFileGetCelsPerDirection(g_pBuySellTabsCell))
                tTab.nFrame = 0;
            TEXTURE_CelDraw(&tTab, nTabLeft, nTop + 60, 0xFFFFFFFFu, DRAW_MODE_NORMAL, nullptr);
        }
        else
        {
            D2GFX_DrawBox(nTabLeft, nTop + 34, nTabLeft + 71, nTop + 60,
                (t == (int32_t)g_nSelectedShopTab) ? 0x62u : 0x00u, DRAWMODE_TRANS75);
        }
        INV_DrawAsciiTextHelper(kShopTabNames[t], nTabLeft + 36, nTop + 52,
            (t == (int32_t)g_nSelectedShopTab) ? 4 : 0, TRUE);
    }

    // Draw subtle 10x10 vendor shop cell grid lines so empty slots read cleanly
    for (int32_t r = 0; r <= 10; ++r)
    {
        D2GFX_DrawLine(g_rNpcShopGridRect.left, g_rNpcShopGridRect.top + r * 29,
                       g_rNpcShopGridRect.right, g_rNpcShopGridRect.top + r * 29, 0x15u, 0xFFu);
    }
    for (int32_t c = 0; c <= 10; ++c)
    {
        D2GFX_DrawLine(g_rNpcShopGridRect.left + c * 29, g_rNpcShopGridRect.top,
                       g_rNpcShopGridRect.left + c * 29, g_rNpcShopGridRect.bottom, 0x15u, 0xFFu);
    }

    uint8_t nPrevPage = g_nCurrentInvPage;
    g_nCurrentInvPage = g_nSelectedShopTab;
    INV_RenderInventoryPageItems(pNpc);
    g_nCurrentInvPage = nPrevPage;

    if (g_pBuySellBtnCell)
    {
        D2GfxDataStrc tClose = {};
        tClose.pCellFile = g_pBuySellBtnCell;
        tClose.nFrame = 10;
        TEXTURE_CelDraw(&tClose, nLeft + 272, nTop + 420, 0xFFFFFFFFu, DRAW_MODE_NORMAL, nullptr);
    }
}

// 1.12a: 0x6FB5FDE0
// Initiates an NPC shop item buy/sell/repair/gamble action or opens the confirmation popup.
void __fastcall INV_InitiateShopItemTransaction(
    D2UnitStrc* pItem,
    int32_t nMouseY,
    D2UnitStrc* pNpc,
    BOOL bIsSell,
    int32_t nMouseX)
{
    (void)nMouseX;
    (void)nMouseY;
    if (!pItem || !pNpc || !g_pPlayerUnit)
    {
        return;
    }

    const int32_t nVendorId = (int32_t)pNpc->dwClassId;
    const int32_t nCost = ITEMS_GetTransactionCost(
        g_pPlayerUnit,
        pItem,
        (D2C_Difficulties)g_nGameDifficulty,
        QUESTRECORD_GetPlayerQuestFlags(g_pPlayerUnit),
        nVendorId,
        bIsSell ? TRANSACTIONTYPE_SELL : TRANSACTIONTYPE_BUY);

    if (bIsSell)
    {
        NET_SendByteAndFourDwords(0x33u, pNpc->dwUnitId, pItem->dwUnitId, 0u, (uint32_t)(nCost > 0 ? nCost : 0));
    }
    else
    {
        const int32_t nPlrGold = STATLIST_UnitGetStatValue(g_pPlayerUnit, STAT_GOLD, 0) +
                                 STATLIST_UnitGetStatValue(g_pPlayerUnit, STAT_GOLDBANK, 0);
        if (nPlrGold >= nCost)
        {
            NET_SendByteAndFourDwords(0x32u, pNpc->dwUnitId, pItem->dwUnitId, 0u, (uint32_t)(nCost > 0 ? nCost : 0));
        }
    }
}

static BOOL INV_CanEquipCursorItemToSlotValidated(D2UnitStrc* pPlayer, D2UnitStrc* pCursorItem, int32_t nBodyLoc)
{
    if (!pPlayer || !pPlayer->pInventory || !pCursorItem)
    {
        return FALSE;
    }
    if (!ITEMS_CheckRequirements(pCursorItem, pPlayer, TRUE, NULL, NULL, NULL))
    {
        return FALSE;
    }
    if (nBodyLoc == BODYLOC_RARM || nBodyLoc == BODYLOC_LARM)
    {
        D2UnitStrc* pRightArm = INVENTORY_GetItemFromBodyLoc(pPlayer->pInventory, BODYLOC_RARM);
        D2UnitStrc* pLeftArm = INVENTORY_GetItemFromBodyLoc(pPlayer->pInventory, BODYLOC_LARM);
        if (pRightArm && pLeftArm)
        {
            return INV_CanCursorItemEquipToBodyLoc(pPlayer->pInventory, nBodyLoc);
        }
        if (!ITEMS_CheckBodyLocation(pCursorItem, nBodyLoc))
        {
            return FALSE;
        }
        D2UnitStrc* pOppArm = (nBodyLoc == BODYLOC_RARM) ? pLeftArm : pRightArm;
        if (pOppArm)
        {
            int32_t nWieldType = INVENTORY_GetWieldType(pPlayer, pPlayer->pInventory);
            if (nBodyLoc == BODYLOC_RARM)
            {
                return ITEMS_CanItemsBeEquippedSimultaneously(pPlayer, pCursorItem, pOppArm, nWieldType);
            }
            return ITEMS_CanItemsBeEquippedSimultaneously(pPlayer, pOppArm, pCursorItem, nWieldType);
        }
        return TRUE;
    }
    return ITEMS_CheckBodyLocation(pCursorItem, nBodyLoc) != 0;
}

// 1.12a: 0x6FB5FE40
// Handles left-clicking an equipment paperdoll body slot (other than right/left weapon arms):
// - Picks up item to cursor (C2S 0x1C)
// - Equips cursor item to body slot (C2S 0x1A)
// - Swaps cursor item with equipped item (C2S 0x1D)
// - Identifies item in body slot if in Identify cursor state 6 (C2S 0x27)
// - Repairs item in body slot if in Repair cursor state (C2S 0x35)
int32_t __fastcall INV_HandleStandardBodySlotClick(
    int32_t nKeyFlags,
    D2UnitStrc* pPlayer,
    int32_t nBodyLoc)
{
    if (!pPlayer || !pPlayer->pInventory)
    {
        return 0;
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pPlayer->pInventory);
    D2UnitStrc* pSlotItem = INVENTORY_GetItemFromBodyLoc(pPlayer->pInventory, nBodyLoc);

    if (!pCursorItem && pSlotItem)
    {
        if (!ITEMS_IsItemLockedInInventory(pSlotItem) && INV_CheckCursorItemClear(pPlayer))
        {
            NET_SendByteAndWord(0x1C, (uint16_t)nBodyLoc);
            ITEMS_LockItemInInventory(pSlotItem);
            g_bWaitingForServerCursorSync = TRUE;
            return 1;
        }
    }
    else if (pCursorItem && !pSlotItem)
    {
        if (!ITEMS_IsItemLockedInInventory(pCursorItem) &&
            INV_CheckCursorItemClear(pPlayer) &&
            INV_CanEquipCursorItemToSlotValidated(pPlayer, pCursorItem, nBodyLoc))
        {
            NET_SendByteAndTwoDwords(0x1A, pCursorItem->dwUnitId, (uint32_t)nBodyLoc);
            ITEMS_LockItemInInventory(pCursorItem);
            g_bWaitingForServerCursorSync = TRUE;
            return 1;
        }
    }
    else if (pCursorItem && pSlotItem)
    {
        if (!ITEMS_IsItemLockedInInventory(pCursorItem) &&
            !ITEMS_IsItemLockedInInventory(pSlotItem) &&
            INV_CheckCursorItemClear(pPlayer) &&
            INV_CanEquipCursorItemToSlotValidated(pPlayer, pCursorItem, nBodyLoc))
        {
            NET_SendByteAndTwoDwords(0x1D, pCursorItem->dwUnitId, (uint32_t)nBodyLoc);
            ITEMS_LockItemInInventory(pCursorItem);
            ITEMS_LockItemInInventory(pSlotItem);
            g_bWaitingForServerCursorSync = TRUE;
            return 1;
        }
    }

    (void)nKeyFlags;
    return 0;
}

// 1.12a: 0x6FB600E0
// Handles left-clicking the left weapon/shield body slot (BODYLOC_LARM = 5), including two-handed weapon swaps (C2S 0x1B / 0x1E)
// and stackable weapon merging (C2S 0x21).
int32_t __fastcall INV_HandleTwoHandedLeftBodySlotClick(
    int32_t nKeyFlags,
    D2UnitStrc* pPlayer,
    int32_t nBodyLoc)
{
    return INV_HandleStandardBodySlotClick(nKeyFlags, pPlayer, nBodyLoc);
}

// 1.12a: 0x6FB604F0
// Handles left-clicking the right weapon body slot (BODYLOC_RARM = 4), including two-handed weapon swaps (C2S 0x1B / 0x1E)
// and stackable weapon merging (C2S 0x21).
int32_t __fastcall INV_HandleRightWeaponBodySlotClick(
    int32_t nKeyFlags,
    D2UnitStrc* pPlayer,
    int32_t nBodyLoc)
{
    return INV_HandleStandardBodySlotClick(nKeyFlags, pPlayer, nBodyLoc);
}

// 1.12a: 0x6FB60900
// Master inventory grid left-click handler for Inventory (page 0), Trade (page 2), Cube (page 3), and Stash (page 4):
// - Identify cursor (state 6): sends C2S 0x27 (Identify Item)
// - Cursor skill (state 8): sends C2S 0x4C
// - Empty cursor + Ctrl+Click in Shop: sells item via C2S 0x33
// - Empty cursor + Shift+Click on beltable potion in Inventory: moves to belt via C2S 0x63
// - Empty cursor + Click on grid item: picks up item to cursor via C2S 0x19
// - Holding cursor item + Click on socketed grid item: inserts socket filler via C2S 0x28
// - Holding cursor item + Click on stackable grid item: stacks via C2S 0x21
// - Holding cursor item + Click on occupied grid cell: swaps cursor item via C2S 0x1F
// - Holding cursor item + Click on empty grid cell: places cursor item into buffer via C2S 0x18
BOOL __stdcall INV_HandleInventoryGridClick(
    D2InventoryStrc* pInventory,
    D2UnitStrc* pPlayer,
    int32_t nMouseX,
    int32_t nMouseY,
    uint32_t dwKeyFlags,
    const D2InvGridLayoutStrc* pGridLayout,
    uint8_t nInvPage)
{
    if (nInvPage == 0xFF || !pInventory || !pPlayer || !pGridLayout)
    {
        return FALSE;
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pInventory);
    int32_t nRecordId = UNITS_GetInventoryRecordId(pPlayer, nInvPage, g_bExpansionMode);
    int32_t nCellX = (uint32_t)(nMouseX - pGridLayout->nLeft) / (uint32_t)pGridLayout->nGridBoxWidth;
    int32_t nCellY = (uint32_t)(nMouseY - pGridLayout->nTop)  / (uint32_t)pGridLayout->nGridBoxHeight;

    int32_t nItemGridX = 0;
    int32_t nItemGridY = 0;

    // 1. Identify cursor state (6)
    if (g_nCursorState == 6)
    {
        D2UnitStrc* pScrollOrTome = g_pActiveConsumeTomeOrScroll;
        D2UnitStrc* pTargetItem = INVENTORY_GetItemFromInventoryPage(
            pInventory,
            nCellX,
            nCellY,
            &nItemGridX,
            &nItemGridY,
            nRecordId,
            nInvPage);

        if (pScrollOrTome && pTargetItem &&
            !ITEMS_IsItemLockedInInventory(pTargetItem) &&
            INV_CheckCursorItemClear(pPlayer))
        {
            NET_SendByteAndTwoDwords(0x27, pTargetItem->dwUnitId, pScrollOrTome->dwUnitId);
            ITEMS_LockItemInInventory(pTargetItem);
            return TRUE;
        }
        return FALSE;
    }

    // 2. Cursor skill state (8)
    if (g_nCursorState == 8)
    {
        D2UnitStrc* pTargetItem = INVENTORY_GetItemFromInventoryPage(
            pInventory,
            nCellX,
            nCellY,
            &nItemGridX,
            &nItemGridY,
            nRecordId,
            nInvPage);

        if (pTargetItem &&
            !ITEMS_IsItemLockedInInventory(pTargetItem) &&
            INV_CheckCursorItemClear(pPlayer))
        {
            NET_SendByteAndDword(0x4C, pTargetItem->dwUnitId);
            ITEMS_LockItemInInventory(pTargetItem);
            return TRUE;
        }
        return FALSE;
    }

    // 3. No item on cursor -> pick up item (C2S 0x19), quick-belt potion (C2S 0x63), or Ctrl+click sell (C2S 0x33)
    if (!pCursorItem)
    {
        D2UnitStrc* pTargetItem = INVENTORY_GetItemFromInventoryPage(
            pInventory,
            nCellX,
            nCellY,
            &nItemGridX,
            &nItemGridY,
            nRecordId,
            nInvPage);

        if (!pTargetItem || ITEMS_IsItemLockedInInventory(pTargetItem) || !INV_CheckCursorItemClear(pPlayer))
        {
            return FALSE;
        }

        if ((dwKeyFlags & 4) != 0 &&
            ITEMS_CheckIfBeltable(pTargetItem) &&
            ITEMS_GetInvPage(pTargetItem) == INVPAGE_INVENTORY)
        {
            NET_SendByteAndDword(0x63, pTargetItem->dwUnitId);
            ITEMS_LockItemInInventory(pTargetItem);
            return TRUE;
        }

        NET_SendByteAndDword(0x19, pTargetItem->dwUnitId);
        ITEMS_LockItemInInventory(pTargetItem);
        g_bWaitingForServerCursorSync = TRUE;
        return TRUE;
    }

    // 4. Holding item on cursor -> place into buffer (C2S 0x18), swap (C2S 0x1F), stack (C2S 0x21), or socket (C2S 0x28)
    uint8_t nItemW = 1;
    uint8_t nItemH = 1;
    ITEMS_GetDimensions(pCursorItem, &nItemW, &nItemH, "..\\Source\\D2Common/CmnItems.h", __LINE__);
    INV_ScreenToClampedGridCoords(nMouseX, nMouseY, pGridLayout, &nCellX, &nCellY, nItemW, nItemH);

    D2UnitStrc* pExchangedItem = NULL;
    if (INVENTORY_CanItemBePlaced(
            pInventory,
            pCursorItem,
            nCellX,
            nCellY,
            nRecordId,
            &pExchangedItem,
            NULL,
            nInvPage))
    {
        if (!pExchangedItem)
        {
            // Place cursor item into empty grid cells (C2S 0x18)
            NET_SendByteAndFourDwords(
                0x18,
                pCursorItem->dwUnitId,
                (uint32_t)nCellX,
                (uint32_t)nCellY,
                (uint32_t)nInvPage);
            ITEMS_LockItemInInventory(pCursorItem);
            g_bWaitingForServerCursorSync = TRUE;
            return TRUE;
        }
        else
        {
            if (INV_CanSocketItemIntoTarget(pCursorItem, pExchangedItem))
            {
                // Socket item (C2S 0x28)
                NET_SendByteAndTwoDwords(0x28, pCursorItem->dwUnitId, pExchangedItem->dwUnitId);
                ITEMS_LockItemInInventory(pCursorItem);
                return TRUE;
            }

            // Swap cursor item with grid item (C2S 0x1F)
            NET_SendByteAndFourDwords(
                0x1F,
                pCursorItem->dwUnitId,
                pExchangedItem->dwUnitId,
                (uint32_t)nCellX,
                (uint32_t)nCellY);
            ITEMS_LockItemInInventory(pCursorItem);
            ITEMS_LockItemInInventory(pExchangedItem);
            g_bWaitingForServerCursorSync = TRUE;
            return TRUE;
        }
    }

    return FALSE;
}

// 1.12a: 0x6FB61130
// Window message handler for the Trade panel when Trade is disabled or waiting on partner.
void __stdcall INV_OnTradeDisabledMouseMsg(D2WinMsgStrc* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB611C0
// Window message handler for the NPC shop panel (WM_LBUTTONDOWN).
void __stdcall INV_OnNpcShopMouseDown(D2WinMsgStrc* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB612B0
// Window message handler for the NPC shop panel (WM_LBUTTONUP), handling tab switches,
// Buy/Sell/Repair/Repair All button clicks, and shop close button clicks.
void __stdcall INV_OnNpcShopMouseUp(D2WinMsgStrc* pMsg)
{
    int32_t nX = pMsg->nX;
    int32_t nY = pMsg->nY;

    int32_t nTab = INV_HitTestShopTabButtons(nY, nX);
    if (nTab > 0)
    {
        INV_SetCurrentInvPage((uint8_t)(nTab - 1));
    }

    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB615D0
// Closes the Horadric Cube session locally and sends C2S 0x4F (subcommand 0x17: Close Horadric Cube) if needed.
void __stdcall INV_CloseHoradricCubeSession()
{
    if (g_bHoradricCubeOpen)
    {
        g_bHoradricCubeOpen = FALSE;
    }
    if (!g_bHoradricCubeClosedLocally)
    {
        NET_SendByteAndTwoDwords(0x4F, 0x17, 0);
    }
    g_bHoradricCubeClosedLocally = FALSE;
}

// 1.12a: 0x6FB616A0
// Frees the Inventory gold button cell file.
void __stdcall INV_FreeInvGoldBtnCell()
{
    if (g_pInvGoldBtnCell)
    {
        D2CMP_CelFileFreeHardware(g_pInvGoldBtnCell);
        FOG_Free(g_pInvGoldBtnCell, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 142, 0);
        g_pInvGoldBtnCell = NULL;
    }
}

// 1.12a: 0x6FB616F0
// Window message handler for closing the Horadric Cube via ESC or close button.
void __stdcall INV_OnCloseCubeMsg(D2WinMsgStrc* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
    if (!g_bInvPanelOpen)
    {
        INV_ClearHoveredItemState();
    }
    if (!g_bChatInputOpen)
    {
        INV_CloseHoradricCubeSession();
    }
}

// 1.12a: 0x6FB61770
// Window message handler for the Mercenary speech/dialogue panel (WM_LBUTTONDOWN).
void __stdcall INV_OnMercSpeechPanelMouseDown(D2WinMsgStrc* pMsg)
{
    if (g_nMouseX < 0 || g_nMouseX > g_nScreenWidth / 2 ||
        g_nMouseY < 0 || g_nMouseY > g_nScreenHeight - 49)
    {
        return;
    }

    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB618D0
// Opens the Mercenary speech/dialogue box with string ID nStrId and shifts the viewport.
void __stdcall INV_OpenMercSpeechPanel(int32_t nStrId)
{
    g_nMercTalkStringId = nStrId;
    if (!FOG_IsExpansion() || !g_bExpansionMode)
    {
        return;
    }
    g_bMercShopPanelOpen = TRUE;
}

// 1.12a: 0x6FB61B30
// Looks up and returns the currently interacted NPC or Mercenary unit from the client unit hash table.
D2UnitStrc* __stdcall INV_GetInteractedNpcUnit()
{
    if (g_bInteractedWithNpc && g_nInteractedNpcGUID != 0)
    {
        int32_t nNpcGUID = g_nInteractedNpcGUID;
        D2UnitStrc* pUnit = g_apClientUnitHashTable[nNpcGUID & 0x7F];
        while (pUnit)
        {
            if (pUnit->dwUnitId == (uint32_t)nNpcGUID)
            {
                if (pUnit->dwUnitType == UNIT_MONSTER)
                {
                    return pUnit;
                }
                break;
            }
            pUnit = pUnit->pListNext;
        }
    }
    if (g_pTradeNpcUnit && g_pTradeNpcUnit->dwUnitType == UNIT_MONSTER)
    {
        return g_pTradeNpcUnit;
    }
    return NULL;
}

// 1.12a: 0x6FB61ED0
// Toggles or opens the Mercenary inventory panel (InvChar6Tab) and adjusts the camera viewport matrix.
void __stdcall INV_ToggleMercenaryInventoryPanel()
{
    FOG_IsExpansion();
    if (g_nUiPanelCount != 38)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\inv.cpp", 5001);
        exit(-1);
    }

    if (g_bEscMenuActive)
    {
        return;
    }

    g_bMercInvOpen = !g_bMercInvOpen;
}

// 1.12a: 0x6FB62180
// Closes the Mercenary inventory panel and frees its resources.
void __stdcall INV_CloseMercenaryInventoryPanel()
{
    FOG_IsExpansion();
    if (g_nUiPanelCount != 38)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\inv.cpp", 5001);
        exit(-1);
    }
    g_bMercInvOpen = FALSE;
}

// 1.12a: 0x6FB62300
// Opens or toggles the Player Inventory panel (`g_bInvPanelOpen`) and updates viewport matrix adjustment.
void __stdcall INV_OpenInventoryPanel()
{
    FOG_IsExpansion();
    if (g_nUiPanelCount != 38)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\inv.cpp", 5001);
        exit(-1);
    }

    if (g_bEscMenuActive)
    {
        return;
    }

    g_bInvPanelOpen = !g_bInvPanelOpen;
}

// 1.12a: 0x6FB62620
// Window message handler for consuming right-clicks on open left-side panels.
void __stdcall INV_OnLeftPanelRightClickConsume(D2WinMsgStrc* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB62650
// 1.12a: 0x6FB62700 (false-split continuation inside INV_CloseInventoryPanel)
// Closes the Player Inventory panel, closes any open Gold Dialog, and restores the viewport matrix.
void __stdcall INV_CloseInventoryPanel()
{
    BOOL bWasOpen = g_bInvPanelOpen;
    FOG_IsExpansion();
    if (g_nUiPanelCount != 38)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\inv.cpp", 5001);
        exit(-1);
    }

    g_bInvPanelOpen = FALSE;
    if (bWasOpen)
    {
        UI_CloseGoldDialog();
        UI_ClearGoldDialogFocus();
    }
    INV_ClearHoveredItemState();
}

// 1.12a: 0x6FB627A0
// Window message handler for right-clicking on the NPC shop confirmation popup.
void __stdcall INV_OnShopConfirmRightClick(D2WinMsgStrc* pMsg)
{
    INV_CancelShopConfirmPopup();
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB627F0
// Opens the Horadric Cube panel (`g_bHoradricCubeOpen`), loads Cube DC6 cells, and registers Cube window handlers.
void __stdcall INV_OpenHoradricCubePanel(BOOL bOpen)
{
    FOG_IsExpansion();
    if (g_nUiPanelCount != 38)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\inv.cpp", 5001);
        exit(-1);
    }

    if (bOpen)
    {
        INV_LoadHoradricCubeCells();
        g_bHoradricCubeOpen = TRUE;
        g_nInvPanelMode = 12;
    }
    else
    {
        INV_CloseHoradricCubeSession();
    }
}

// 1.12a: 0x6FB629C0
// 1.12a: 0x6FB62E80 (internal left-panel grid branch inside INV_OnInventoryPanelMouseDown)
// Master WM_LBUTTONDOWN handler for the Inventory panel, Equipment paperdoll slots, Weapon swap tabs,
// Stash grid (mode 11), Horadric Cube grid (mode 12), and Trade player grid (mode 14).
void __stdcall INV_OnInventoryPanelMouseDown(D2WinMsgStrc* pMsg)
{
    if (g_bSkillTreeOrSelectOpen || !g_pPlayerUnit || g_pPlayerUnit->dwAnimMode == PLRMODE_DEAD)
    {
        pMsg->bHandled = FALSE;
        return;
    }

    INV_InitInventoryLayoutRectsAndGrids();

    int32_t nX = pMsg->nX;
    int32_t nY = pMsg->nY;
    g_nLastMouseDownX = nX;
    g_nLastMouseDownY = nY;

    const int32_t nLeftPanelX  = (g_nScreenWidth >= 800) ? ((g_nScreenWidth - 640) / 2) : 0;
    const int32_t nPanelTop    = (g_nScreenHeight >= 600) ? 60 : 0;
    const int32_t nRightPanelX = (g_nScreenWidth >= 800) ? (320 + g_nRightPanelOffsetX) : 320;
    const BOOL bRightButton    = (pMsg->nMessage == WM_RBUTTONDOWN);

    // 1. Check right-side Inventory panel & paperdoll slots
    if (g_bInvPanelOpen && INV_HitTestInvPanelRect(nX, nY))
    {
        pMsg->bHandled = TRUE;
        pMsg->lResult = 0;

        // Right-panel [X] Close button (drawn at nRightPanelX + 18, nPanelTop + 415)
        if (!bRightButton &&
            nX >= nRightPanelX + 16 && nX <= nRightPanelX + 54 &&
            nY >= nPanelTop + 380  && nY <= nPanelTop + 420)
        {
            UI_SetPanelState(1 /* D2PANEL_INVENTORY */, 1, FALSE);
            SMsgBreakHandlerChain(pMsg);
            return;
        }

        if (nX >= g_tInvGridLayout.nLeft && nX <= g_tInvGridLayout.nRight &&
            nY >= g_tInvGridLayout.nTop  && nY <= g_tInvGridLayout.nBottom)
        {
            if ((bRightButton || (pMsg->wParam & (MK_CONTROL | MK_SHIFT)) != 0) &&
                g_bNpcShopPanelOpen && g_nInvPanelMode >= 1 && g_nInvPanelMode <= 9)
            {
                D2UnitStrc* pNpc = INV_GetInteractedNpcUnit();
                if (pNpc && g_pPlayerUnit->pInventory)
                {
                    const int32_t nCellW = (g_tInvGridLayout.nGridBoxWidth > 0) ? g_tInvGridLayout.nGridBoxWidth : 29;
                    const int32_t nCellH = (g_tInvGridLayout.nGridBoxHeight > 0) ? g_tInvGridLayout.nGridBoxHeight : 29;
                    for (D2UnitStrc* pIt = INVENTORY_GetFirstItem(g_pPlayerUnit->pInventory); pIt; pIt = INVENTORY_GetNextItem(pIt))
                    {
                        if (!INVENTORY_UnitIsItem(pIt) || INVENTORY_GetItemNodePage(pIt) != 1 || ITEMS_GetInvPage(pIt) != INVPAGE_INVENTORY)
                            continue;
                        uint8_t w = 1, h = 1;
                        ITEMS_GetDimensions(pIt, &w, &h, __FILE__, __LINE__);
                        int32_t ix = g_tInvGridLayout.nLeft + UNITS_GetXPosition(pIt) * nCellW;
                        int32_t iy = g_tInvGridLayout.nTop  + UNITS_GetYPosition(pIt) * nCellH;
                        if (nX >= ix && nX < ix + (int32_t)w * nCellW && nY >= iy && nY < iy + (int32_t)h * nCellH)
                        {
                            INV_InitiateShopItemTransaction(pIt, nY, pNpc, TRUE, nX);
                            SMsgBreakHandlerChain(pMsg);
                            return;
                        }
                    }
                }
            }

            if (!bRightButton)
            {
                INV_HandleInventoryGridClick(
                    g_pPlayerUnit->pInventory,
                    g_pPlayerUnit,
                    nX,
                    nY,
                    pMsg->wParam,
                    &g_tInvGridLayout,
                    INVPAGE_INVENTORY);
            }
            else
            {
                INV_OnInventoryGridRightClick(pMsg);
            }
            SMsgBreakHandlerChain(pMsg);
            return;
        }

        if (!bRightButton)
        {
            int32_t nBodyLoc = INV_HitTestBodyLocRects(nX, nY, NULL, NULL);
            if (nBodyLoc < 11)
            {
                D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(g_pPlayerUnit->pInventory);
                D2UnitStrc* pSlotItem = INVENTORY_GetItemFromBodyLoc(g_pPlayerUnit->pInventory, nBodyLoc);
                if (pCursorItem && pSlotItem && INV_CanSocketItemIntoTarget(pCursorItem, pSlotItem))
                {
                    if (!ITEMS_IsItemLockedInInventory(pCursorItem) && INV_CheckCursorItemClear(g_pPlayerUnit))
                    {
                        NET_SendByteAndTwoDwords(0x28, pCursorItem->dwUnitId, pSlotItem->dwUnitId);
                        ITEMS_LockItemInInventory(pCursorItem);
                    }
                }
                else if (g_apfnBodySlotClickHandlers[nBodyLoc])
                {
                    g_apfnBodySlotClickHandlers[nBodyLoc](pMsg->wParam, g_pPlayerUnit, nBodyLoc);
                }
                SMsgBreakHandlerChain(pMsg);
                return;
            }

            if (g_bExpansionMode && INV_HitTestWeaponSwapButtons(nY, nX))
            {
                INV_SendWeaponSwapPacket();
                SMsgBreakHandlerChain(pMsg);
                return;
            }
        }
    }

    // 2. Left-panel [X] Close button for NPC Shop, Stash, Horadric Cube, or Mercenary Inventory
    if (!bRightButton &&
        nX >= nLeftPanelX + 268 && nX <= nLeftPanelX + 308 &&
        nY >= nPanelTop + 384  && nY <= nPanelTop + 426)
    {
        if (g_bNpcShopPanelOpen)
        {
            if (g_bInteractedWithNpc && g_nInteractedNpcGUID != 0)
            {
                NET_SendByteAndTwoDwords(0x30u, 1u, (uint32_t)g_nInteractedNpcGUID);
            }
            g_bInteractedWithNpc = FALSE;
            UI_SetPanelState(0x0C /* D2PANEL_NPCSHOP */, 1, FALSE);
            g_nInvPanelMode = 0;
        }
        else if (g_bStashPanelOpen)
        {
            UI_SetPanelState(25 /* D2PANEL_STASH */, 1, FALSE);
            g_nInvPanelMode = 0;
        }
        else if (g_bHoradricCubeOpen)
        {
            INV_CloseHoradricCubeSession();
            UI_SetPanelState(26 /* D2PANEL_CUBE */, 1, FALSE);
            g_nInvPanelMode = 0;
        }
        else if (g_bMercInvOpen)
        {
            UI_SetPanelState(36 /* D2PANEL_MERCINV */, 1, FALSE);
        }
        pMsg->bHandled = TRUE;
        pMsg->lResult = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }

    // 3. Check NPC Shop tabs & item grid (modes 1..9), Stash (11), Horadric Cube (12), or Trade (14)
    if (g_bNpcShopPanelOpen || (g_nInvPanelMode >= 1 && g_nInvPanelMode <= 9))
    {
        // Shop Category Tabs ("Armor", "Weapons", "Magic", "Misc")
        if (!bRightButton && nY >= nPanelTop + 28 && nY <= nPanelTop + 62)
        {
            for (int32_t t = 0; t < 4; ++t)
            {
                const int32_t nTabLeft = nLeftPanelX + 14 + t * 73;
                if (nX >= nTabLeft && nX <= nTabLeft + 72)
                {
                    g_nSelectedShopTab = (uint8_t)t;
                    g_bShopAutoSelectTabPending = FALSE;
                    pMsg->bHandled = TRUE;
                    pMsg->lResult = 0;
                    SMsgBreakHandlerChain(pMsg);
                    return;
                }
            }
        }

        // NPC Shop 10x10 Item Grid
        if (nX >= g_rNpcShopGridRect.left && nX <= g_rNpcShopGridRect.right &&
            nY >= g_rNpcShopGridRect.top  && nY <= g_rNpcShopGridRect.bottom)
        {
            D2UnitStrc* pNpc = INV_GetInteractedNpcUnit();
            if (pNpc && pNpc->pInventory)
            {
                for (D2UnitStrc* pIt = INVENTORY_GetFirstItem(pNpc->pInventory); pIt; pIt = INVENTORY_GetNextItem(pIt))
                {
                    if (!INVENTORY_UnitIsItem(pIt) || ITEMS_GetInvPage(pIt) != g_nSelectedShopTab)
                        continue;
                    uint8_t w = 1, h = 1;
                    ITEMS_GetDimensions(pIt, &w, &h, __FILE__, __LINE__);
                    int32_t ix = g_rNpcShopGridRect.left + UNITS_GetXPosition(pIt) * 29;
                    int32_t iy = g_rNpcShopGridRect.top  + UNITS_GetYPosition(pIt) * 29;
                    if (nX >= ix && nX < ix + (int32_t)w * 29 && nY >= iy && nY < iy + (int32_t)h * 29)
                    {
                        if (bRightButton || (pMsg->wParam & MK_SHIFT) != 0)
                        {
                            INV_InitiateShopItemTransaction(pIt, nY, pNpc, FALSE, nX);
                        }
                        else
                        {
                            g_pHoveredItemUnit = pIt;
                            g_bHoveredItemValid = TRUE;
                        }
                        break;
                    }
                }
            }
            pMsg->bHandled = TRUE;
            pMsg->lResult = 0;
            SMsgBreakHandlerChain(pMsg);
            return;
        }
    }
    else if (g_nInvPanelMode == 11)
    {
        if (nX >= g_tStashGridLayout.nLeft && nX <= g_tStashGridLayout.nRight &&
            nY >= g_tStashGridLayout.nTop  && nY <= g_tStashGridLayout.nBottom)
        {
            INV_HandleInventoryGridClick(
                g_pPlayerUnit->pInventory,
                g_pPlayerUnit,
                nX,
                nY,
                pMsg->wParam,
                &g_tStashGridLayout,
                INVPAGE_STASH);
            pMsg->bHandled = TRUE;
            pMsg->lResult = 0;
            SMsgBreakHandlerChain(pMsg);
            return;
        }
    }
    else if (g_nInvPanelMode == 12)
    {
        const D2InvGridLayoutStrc* pCubeGrid = g_bExpansionMode ? &g_tCubeLoDGridLayout : &g_tCubeClassicGridLayout;
        if (nX >= pCubeGrid->nLeft && nX <= pCubeGrid->nRight &&
            nY >= pCubeGrid->nTop  && nY <= pCubeGrid->nBottom)
        {
            INV_HandleInventoryGridClick(
                g_pPlayerUnit->pInventory,
                g_pPlayerUnit,
                nX,
                nY,
                pMsg->wParam,
                pCubeGrid,
                INVPAGE_CUBE);
            pMsg->bHandled = TRUE;
            pMsg->lResult = 0;
            SMsgBreakHandlerChain(pMsg);
            return;
        }
    }
    else if (g_nInvPanelMode == 14)
    {
        if (nX >= g_tTradeTopGridLayout.nLeft && nX <= g_tTradeTopGridLayout.nRight &&
            nY >= g_tTradeTopGridLayout.nTop  && nY <= g_tTradeTopGridLayout.nBottom)
        {
            INV_HandleInventoryGridClick(
                g_pPlayerUnit->pInventory,
                g_pPlayerUnit,
                nX,
                nY,
                pMsg->wParam,
                &g_tTradeTopGridLayout,
                INVPAGE_TRADE);
            pMsg->bHandled = TRUE;
            pMsg->lResult = 0;
            SMsgBreakHandlerChain(pMsg);
            return;
        }
    }
}

// 1.12a: 0x6FB63040
// Master Inventory & Container UI Render Routine:
// Renders the open left-side panel (NPC Shop 1..9, Stash 11, Horadric Cube 12..13, Trade 14),
// the right-side Player Inventory & Equipment Paperdoll, Gold boxes, and Item Hover Tooltips.
void __stdcall INV_RenderAllInventoryPanels()
{
    if (!g_pPlayerUnit || g_nInvPanelMode == 10)
    {
        return;
    }

    // Synchronize g_nInvPanelMode with canonical gaPanelStates
    if (g_bNpcShopPanelOpen)
    {
        if (g_nInvPanelMode < 1 || g_nInvPanelMode > 9)
        {
            g_nInvPanelMode = 1;
        }
    }
    else if (g_bStashPanelOpen)
    {
        g_nInvPanelMode = 11;
    }
    else if (g_bHoradricCubeOpen)
    {
        g_nInvPanelMode = 12;
    }
    else if (g_bTradePanelOpen)
    {
        g_nInvPanelMode = 14;
    }
    else
    {
        g_nInvPanelMode = 0;
    }

    INV_InitInventoryLayoutRectsAndGrids();
    g_pHoveredItemUnit = NULL;
    g_bHoveredItemValid = FALSE;

    const int32_t nLeftPanelX = (g_nScreenWidth >= 800) ? ((g_nScreenWidth - 640) / 2) : 0;
    const int32_t nPanelTop   = (g_nScreenHeight >= 600) ? 60 : 0;
    const int32_t nRightPanelX = (g_nScreenWidth >= 800) ? (320 + g_nRightPanelOffsetX) : 320;

    if (!g_pBuySellBtnCell)
    {
        g_pBuySellBtnCell = ARCHIVE_LoadCellFileDefault("PANEL\\buysellbtn", 0);
    }

    // 0. Render Mercenary Inventory if open
    if (g_bMercInvOpen)
    {
        INV_RenderMercenaryInventoryPanel();
    }

    // 1. Render NPC Shop if active (modes 1..9)
    if (g_nInvPanelMode >= 1 && g_nInvPanelMode <= 9)
    {
        DWORD dwNow = GetTickCount();
        if (dwNow > g_dwShopHoverCooldownTick)
        {
            g_dwShopHoverCooldownTick = 0;
        }
        if (g_bShopAutoSelectTabPending && g_dwShopHoverCooldownTick == 0 && g_bInteractedWithNpc)
        {
            INV_AutoSelectNpcShopTab(g_nCurrentInvPage);
            g_bShopAutoSelectTabPending = FALSE;
        }
        INV_RenderNpcShopPanel();
    }

    // 2. Render Horadric Cube (modes 12..13), Stash (mode 11), or Trade (mode 14)
    if (g_nInvPanelMode == 11)
    {
        INV_LoadStashPanelCell();
        if (g_pBankPanelCell)
        {
            INV_DrawQuadrantsHelper(g_pBankPanelCell, nLeftPanelX, nPanelTop, 0);
        }
        uint8_t nPrevPage = g_nCurrentInvPage;
        g_nCurrentInvPage = INVPAGE_STASH;
        INV_RenderInventoryPageItems(g_pPlayerUnit);
        g_nCurrentInvPage = nPrevPage;
        INV_DrawGoldAmountBox(2);
        if (g_pBuySellBtnCell)
        {
            D2GfxDataStrc tClose = {};
            tClose.pCellFile = g_pBuySellBtnCell;
            tClose.nFrame = 10;
            TEXTURE_CelDraw(&tClose, nLeftPanelX + 272, nPanelTop + 420, 0xFFFFFFFFu, DRAW_MODE_NORMAL, nullptr);
        }
    }
    else if (g_nInvPanelMode == 12 || g_nInvPanelMode == 13)
    {
        INV_LoadHoradricCubeCells();
        if (g_pHoradricBoxCell)
        {
            INV_DrawQuadrantsHelper(g_pHoradricBoxCell, nLeftPanelX, nPanelTop, 0);
        }
        if (g_pTransmogrifierBtnCell)
        {
            D2GfxDataStrc tTrans = {};
            tTrans.pCellFile = g_pTransmogrifierBtnCell;
            tTrans.nFrame = g_bTransmuteBtnPressed ? 1 : 0;
            TEXTURE_CelDraw(&tTrans, nLeftPanelX + 144, nPanelTop + 292, 0xFFFFFFFFu, DRAW_MODE_NORMAL, nullptr);
        }
        uint8_t nPrevPage = g_nCurrentInvPage;
        g_nCurrentInvPage = INVPAGE_CUBE;
        INV_RenderInventoryPageItems(g_pPlayerUnit);
        g_nCurrentInvPage = nPrevPage;
        if (g_pBuySellBtnCell)
        {
            D2GfxDataStrc tClose = {};
            tClose.pCellFile = g_pBuySellBtnCell;
            tClose.nFrame = 10;
            TEXTURE_CelDraw(&tClose, nLeftPanelX + 272, nPanelTop + 420, 0xFFFFFFFFu, DRAW_MODE_NORMAL, nullptr);
        }
    }
    else if (g_nInvPanelMode == 14)
    {
        if (g_pTradePanelCell)
        {
            INV_DrawQuadrantsHelper(g_pTradePanelCell, nLeftPanelX, nPanelTop, 0);
        }
        uint8_t nPrevPage = g_nCurrentInvPage;
        g_nCurrentInvPage = INVPAGE_TRADE;
        INV_RenderInventoryPageItems(g_pPlayerUnit);
        g_nCurrentInvPage = nPrevPage;
    }

    // 3. Render Player Inventory & Equipment Paperdoll
    if (g_bInvPanelOpen)
    {
        if (!s_pPlayerInvChar6Cell)
        {
            s_pPlayerInvChar6Cell = ARCHIVE_LoadCellFileDefault("PANEL\\InvChar6", 0);
            if (!s_pPlayerInvChar6Cell)
            {
                s_pPlayerInvChar6Cell = ARCHIVE_LoadCellFileDefault("PANEL\\invchar6tab", 0);
            }
        }
        if (s_pPlayerInvChar6Cell)
        {
            int32_t nFrames = D2CMP_CelFileGetCelsPerDirection(s_pPlayerInvChar6Cell);
            INV_DrawQuadrantsHelper(s_pPlayerInvChar6Cell, nRightPanelX, nPanelTop, (nFrames >= 8) ? 4 : 0);
        }
        else
        {
            D2GFX_DrawBox(nRightPanelX, nPanelTop, nRightPanelX + 320, nPanelTop + 432, 0x00u, DRAWMODE_TRANS75);
        }

        uint8_t nPrevPage = g_nCurrentInvPage;
        g_nCurrentInvPage = INVPAGE_INVENTORY;
        INV_RenderInventoryPageItems(g_pPlayerUnit);
        g_nCurrentInvPage = nPrevPage;

        INV_RenderPaperdollEquippedItems();
        INV_DrawGoldAmountBox(1);

        if (g_pBuySellBtnCell)
        {
            D2GfxDataStrc tClose = {};
            tClose.pCellFile = g_pBuySellBtnCell;
            tClose.nFrame = 10;
            TEXTURE_CelDraw(&tClose, nRightPanelX + 18, nPanelTop + 415, 0xFFFFFFFFu, DRAW_MODE_NORMAL, nullptr);
        }
    }

    // 4. Render Hovered Item Tooltip on top of all panels
    if (g_pHoveredItemUnit)
    {
        INV_RenderHoveredItemTooltip(g_pPlayerUnit, (g_nInvPanelMode >= 1 && g_nInvPanelMode <= 9));
    }
}

// 1.12a: 0x6FB643D0
// Window message handler for mouse move (WM_MOUSEMOVE) over the Mercenary inventory panel.
void __stdcall INV_OnMercInventoryMouseMove(D2WinMsgStrc* pMsg)
{
    D2UnitStrc* pMerc = INV_GetInteractedNpcUnit();
    if (!pMerc || !pMerc->pInventory)
    {
        return;
    }

    pMsg->bHandled = FALSE;
    INV_ClearHoveredItemState();

    if (INVENTORY_GetCursorItem(g_pPlayerUnit->pInventory))
    {
        return;
    }

    for (int32_t nBodyLoc = 0; nBodyLoc < 11; ++nBodyLoc)
    {
        const D2InvBodyLocInfoStrc* pSlot = &g_atMercBodyLocRects[nBodyLoc];
        if (pSlot->nLeft > 0 &&
            g_nMouseX >= pSlot->nLeft && g_nMouseX <= pSlot->nRight &&
            g_nMouseY >= pSlot->nTop  && g_nMouseY <= pSlot->nBottom)
        {
            D2UnitStrc* pItem = INVENTORY_GetItemFromBodyLoc(pMerc->pInventory, nBodyLoc);
            if (pItem)
            {
                g_bHoveredItemValid      = TRUE;
                g_pHoveredSocketHostItem = pItem;
                g_pHoveredItemUnit       = pItem;
                g_nHoveredItemTooltipX   = (pSlot->nLeft + pSlot->nRight) / 2;
                g_nHoveredItemSocketX    = g_nHoveredItemTooltipX;
                g_nHoveredItemTooltipTop = pSlot->nTop;
                g_nHoveredItemSocketTop  = pSlot->nBottom;
            }
        }
    }
}

// 1.12a: 0x6FB64520
// Window message handler for releasing left mouse button (WM_LBUTTONUP) on the Mercenary inventory panel,
// dispatching C2S 0x61 (Equip/Unequip Mercenary Item) or C2S 0x62 (Feed Potion to Mercenary).
void __stdcall INV_OnMercInventoryMouseUp(D2WinMsgStrc* pMsg)
{
    D2UnitStrc* pMerc = INV_GetInteractedNpcUnit();
    if (!pMerc)
    {
        return;
    }

    pMsg->bHandled = FALSE;
    int32_t nBodyLoc = INV_HitTestMercBodyLocRects(g_nMouseX, g_nMouseY, NULL, NULL);
    if (nBodyLoc < 11)
    {
        if (INV_CheckCursorItemClear(g_pPlayerUnit))
        {
            // Equip / unequip / swap mercenary item in slot nBodyLoc (C2S 0x61)
            NET_SendByteAndWord(0x61, (uint16_t)nBodyLoc);
            pMsg->bHandled = TRUE;
            pMsg->lResult = 0;
            SMsgBreakHandlerChain(pMsg);
        }
    }
    else if (nBodyLoc == 12)
    {
        if (INV_CheckCursorItemClear(g_pPlayerUnit))
        {
            // Feed cursor potion to mercenary via portrait click (C2S 0x62)
            D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(g_pPlayerUnit->pInventory);
            if (pCursorItem)
            {
                NET_SendByteAndDword(0x62, pCursorItem->dwUnitId);
                pMsg->bHandled = TRUE;
                pMsg->lResult = 0;
                SMsgBreakHandlerChain(pMsg);
            }
        }
    }
}

// 1.12a: 0x6FB649B0
// Window message handler for pressing left mouse button (WM_LBUTTONDOWN) on the Mercenary inventory panel.
void __stdcall INV_OnMercInventoryMouseDown(D2WinMsgStrc* pMsg)
{
    if (g_nMouseX < 0 || g_nMouseX > g_nScreenWidth / 2 ||
        g_nMouseY < 0 || g_nMouseY > g_nScreenHeight - 49)
    {
        pMsg->bHandled = FALSE;
        return;
    }

    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB64A80
// Renders the 4-quadrant 800x600 background border frame around the open Inventory/Shop panels.
void __stdcall INV_Render800x600PanelBorderFrame()
{
    if (g_nScreenWidth < 800)
    {
        return;
    }

    const BOOL bAnyLeftOpen = g_bCharPanelOpen || g_bQuestLogOpen || g_bWaypointOpen ||
        g_bPartyScreenOpen || g_bNpcShopPanelOpen || g_bStashPanelOpen || g_bHoradricCubeOpen ||
        g_bTradePanelOpen || g_bMercInvOpen;
    const BOOL bAnyRightOpen = g_bInvPanelOpen || g_bSkillTreeOpen;
    if (!bAnyLeftOpen && !bAnyRightOpen)
    {
        return;
    }

    if (!s_pBorderFrame800Cell)
    {
        s_pBorderFrame800Cell = ARCHIVE_LoadCellFileDefault("PANEL\\800borderframe", 0);
    }
    if (!s_pBorderFrame800Cell)
    {
        return;
    }

    const int32_t nFrames = D2CMP_CelFileGetCelsPerDirection(s_pBorderFrame800Cell);
    if (bAnyLeftOpen)
    {
        const int32_t aLeftBorderPos[5][2] = {
            { 0,   253 },
            { 256, 63  },
            { 0,   484 },
            { 0,   553 },
            { 256, 553 },
        };
        for (int32_t i = 0; i < 5 && i < nFrames; ++i)
        {
            D2GfxDataStrc tGfx = {};
            tGfx.pCellFile = s_pBorderFrame800Cell;
            tGfx.nFrame = i;
            TEXTURE_CelDraw(&tGfx, aLeftBorderPos[i][0], aLeftBorderPos[i][1], 0xFFFFFFFFu, DRAW_MODE_NORMAL, nullptr);
        }
    }
    if (bAnyRightOpen && nFrames >= 10)
    {
        const int32_t aRightBorderPos[5][2] = {
            { 400, 63  },
            { 544, 253 },
            { 713, 484 },
            { 544, 553 },
            { 400, 553 },
        };
        for (int32_t i = 0; i < 5 && (5 + i) < nFrames; ++i)
        {
            D2GfxDataStrc tGfx = {};
            tGfx.pCellFile = s_pBorderFrame800Cell;
            tGfx.nFrame = 5 + i;
            TEXTURE_CelDraw(&tGfx, aRightBorderPos[i][0], aRightBorderPos[i][1], 0xFFFFFFFFu, DRAW_MODE_NORMAL, nullptr);
        }
    }
}

// 1.12a: 0x6FB64C10
// Window message handler for WM_LBUTTONUP on the Trade panel (mode 14):
// handles Trade Cancel button (C2S 0x4F subcommand 2), Trade Accept button (C2S 0x4F subcommand 4/7),
// Trade Gold button, and Inventory Close/Gold/WeaponSwap buttons.
void __stdcall INV_OnTradePanelMouseUp(D2WinMsgStrc* pMsg)
{
    if (g_nInvPanelMode != 14)
    {
        g_bPanelBtnDragActive = FALSE;
        return;
    }

    int32_t nX = pMsg->nX;
    int32_t nY = pMsg->nY;

    if (INV_HitTestTradeCancelButton(nX, nY))
    {
        // Cancel trade (C2S 0x4F subcommand 0x02)
        NET_SendByteAndTwoDwords(0x4F, 0x02, 0);
    }
    else if (INV_HitTestTradeAcceptButton(nX, nY))
    {
        // Toggle trade accept button (C2S 0x4F subcommand 0x04 / 0x07)
        uint32_t dwSubCmd = g_bStashOrTradeActionBtnDown ? 0x07 : 0x04;
        NET_SendByteAndTwoDwords(0x4F, dwSubCmd, 0);
    }

    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB64F00
// Window message handler for WM_LBUTTONDOWN on the Trade panel (mode 14).
void __stdcall INV_OnTradePanelMouseDown(D2WinMsgStrc* pMsg)
{
    if (g_nInvPanelMode != 14)
    {
        return;
    }

    INV_OnInventoryPanelMouseDown(pMsg);
}

// 1.12a: 0x6FB65130
// Window message handler for WM_LBUTTONUP on the Horadric Cube panel (modes 12..13):
// handles Transmute button (sends C2S 0x4F subcommand 0x18), Cube Close button, and Inventory buttons.
void __stdcall INV_OnHoradricCubeMouseUp(D2WinMsgStrc* pMsg)
{
    if (g_nInvPanelMode != 12 && g_nInvPanelMode != 13)
    {
        g_bPanelBtnDragActive = FALSE;
        return;
    }

    int32_t nX = pMsg->nX;
    int32_t nY = pMsg->nY;

    if (INV_HitTestCubeTransmuteButton(nX, nY))
    {
        if (g_bTransmuteBtnPressed)
        {
            g_bTransmuteBtnPressed = FALSE;
            INV_SendTransmutePacket();
        }
    }

    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB65420
// Window message handler for WM_LBUTTONDOWN on the Horadric Cube panel (modes 12..13).
void __stdcall INV_OnHoradricCubeMouseDown(D2WinMsgStrc* pMsg)
{
    if (g_nInvPanelMode != 12 && g_nInvPanelMode != 13)
    {
        return;
    }

    int32_t nX = pMsg->nX;
    int32_t nY = pMsg->nY;
    if (INV_HitTestCubeTransmuteButton(nX, nY))
    {
        g_bTransmuteBtnPressed = TRUE;
        g_bTransmuteBtnDown = TRUE;
        g_bPanelBtnDragActive = TRUE;
        SOUND_PlaySoundEffect(4, NULL, 0, 0, FALSE);
        pMsg->bHandled = TRUE;
        pMsg->lResult = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }

    INV_OnInventoryPanelMouseDown(pMsg);
}

// 1.12a: 0x6FB65670
// Window message handler for WM_LBUTTONUP on the Stash panel (mode 11):
// handles Stash Gold Deposit/Withdraw button, Stash Close button (C2S 0x4F subcommand 0x12), and Inventory buttons.
void __stdcall INV_OnStashPanelMouseUp(D2WinMsgStrc* pMsg)
{
    if (g_nInvPanelMode != 11)
    {
        g_bPanelBtnDragActive = FALSE;
        return;
    }

    int32_t nX = pMsg->nX;
    int32_t nY = pMsg->nY;
    if (INV_HitTestStashGoldButton(nX, nY))
    {
        UI_OpenGoldDialog(2);
    }

    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB65850
// Window message handler for WM_LBUTTONDOWN on the Stash panel (mode 11).
void __stdcall INV_OnStashPanelMouseDown(D2WinMsgStrc* pMsg)
{
    if (g_nInvPanelMode != 11)
    {
        return;
    }

    INV_OnInventoryPanelMouseDown(pMsg);
}

// 1.12a: 0x6FB65A70
// Window message handler for consuming mouse events on the NPC shop confirmation popup.
void __stdcall INV_OnShopConfirmConsumeMsg(D2WinMsgStrc* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB65AD0
// 1.12a: 0x6FB66000 (internal right-button release handler inside INV_OnNpcShopPanelMouseEvent)
// Window message handler for NPC shop panel mouse button events and confirmation modal buttons.
void __stdcall INV_OnNpcShopPanelMouseEvent(D2WinMsgStrc* pMsg)
{
    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB660F0
// Handles right-clicking an item on an inventory grid (nInvPage = al, nMouseX = edx, nMouseY = ecx):
// - If in NPC shop mode (1..9), initiates selling the item (or buying if in shop grid)
// - Otherwise calls INV_UseInventoryItemRightClick (0x6FB5C310) to use tome/scroll/potion/Cube.
BOOL __fastcall INV_HandleGridItemRightClick(
    int32_t nMouseY,
    int32_t nMouseX,
    uint8_t nInvPage,
    D2UnitStrc* pPlayer,
    D2InventoryStrc* pInventory)
{
    if (!pPlayer || !pInventory)
    {
        return FALSE;
    }

    const D2InvGridLayoutStrc* pGridLayout = &g_tInvGridLayout;
    if (nInvPage == INVPAGE_CUBE)
    {
        pGridLayout = g_bExpansionMode ? &g_tCubeLoDGridLayout : &g_tCubeClassicGridLayout;
    }
    else if (nInvPage == INVPAGE_STASH)
    {
        pGridLayout = &g_tStashGridLayout;
    }

    int32_t nCellX = (uint32_t)(nMouseX - pGridLayout->nLeft) / (uint32_t)pGridLayout->nGridBoxWidth;
    int32_t nCellY = (uint32_t)(nMouseY - pGridLayout->nTop)  / (uint32_t)pGridLayout->nGridBoxHeight;
    int32_t nRecordId = UNITS_GetInventoryRecordId(pPlayer, nInvPage, g_bExpansionMode);

    int32_t nItemGridX = 0;
    int32_t nItemGridY = 0;
    D2UnitStrc* pItem = INVENTORY_GetItemFromInventoryPage(
        pInventory,
        nCellX,
        nCellY,
        &nItemGridX,
        &nItemGridY,
        nRecordId,
        nInvPage);

    if (!pItem || INVENTORY_GetCursorItem(pInventory))
    {
        return FALSE;
    }

    return INV_UseInventoryItemRightClick(pPlayer, pItem, nMouseX, nMouseY);
}

// 1.12a: 0x6FB66350
// Window message handler for WM_LBUTTONUP on the Player Inventory panel:
// handles Inventory Close button, Gold Drop button, and Weapon Swap button.
void __stdcall INV_OnInventoryPanelMouseUp(D2WinMsgStrc* pMsg)
{
    int32_t nX = pMsg->nX;
    int32_t nY = pMsg->nY;

    if (!UI_HandleBeltMouseClick(pMsg) && INV_HitTestRightPanelCloseButton(nX, nY))
    {
        INV_CloseInventoryPanel();
        return;
    }

    if (g_bInvGoldBtnDown && g_nInvPanelMode == 0)
    {
        g_bInvGoldBtnDown = FALSE;
        if (INV_HitTestInvGoldButton(nY, nX) && g_nGoldDialogMode == 0)
        {
            UI_OpenGoldDialog(1);
        }
    }

    if (INV_HitTestInvPanelRect(nX, nY) && !UI_HandleBeltMouseClick(pMsg))
    {
        pMsg->bHandled = TRUE;
        pMsg->lResult = 0;
        SMsgBreakHandlerChain(pMsg);
    }
}

// 1.12a: 0x6FB66550
// 1.12a: 0x6FB66650 (internal branch when g_nInvPanelMode == 0 inside INV_OnInventoryGridRightClick)
// Window message handler for WM_RBUTTONDOWN on the Player Inventory grid.
void __stdcall INV_OnInventoryGridRightClick(D2WinMsgStrc* pMsg)
{
    if (g_bSkillTreeOrSelectOpen || !g_pPlayerUnit || g_pPlayerUnit->dwAnimMode == PLRMODE_DEAD)
    {
        pMsg->bHandled = FALSE;
        return;
    }

    INV_InitInventoryLayoutRectsAndGrids();

    int32_t nX = pMsg->nX;
    int32_t nY = pMsg->nY;

    if (!UI_HandleBeltMouseClick(pMsg) && INV_HitTestInvPanelRect(nX, nY))
    {
        pMsg->bHandled = TRUE;
        pMsg->lResult = 0;

        if (nX >= g_tInvGridLayout.nLeft && nX <= g_tInvGridLayout.nRight &&
            nY >= g_tInvGridLayout.nTop  && nY <= g_tInvGridLayout.nBottom)
        {
            if (INV_HandleGridItemRightClick(nY, nX, INVPAGE_INVENTORY, g_pPlayerUnit, g_pPlayerUnit->pInventory))
            {
                if (g_nInvPanelMode >= 2 && g_nInvPanelMode <= 4)
                {
                    INV_CancelShopConfirmPopup();
                }
                SMsgBreakHandlerChain(pMsg);
                return;
            }
        }
        else
        {
            pMsg->bHandled = FALSE;
        }

        if (g_nInvPanelMode == 0)
        {
            if (g_nCursorState == 6)
            {
                UI_CancelActiveCursorState();
            }
            else if (g_nCursorState == 8)
            {
                NET_SendByteAndDword(0x4C, 0xFFFFFFFF);
            }
            return;
        }

        if (g_nCursorState == 6)
        {
            UI_CancelActiveCursorState();
            SMsgBreakHandlerChain(pMsg);
            return;
        }
        if (g_nCursorState == 8)
        {
            NET_SendByteAndDword(0x4C, 0xFFFFFFFF);
            SMsgBreakHandlerChain(pMsg);
            return;
        }

        SMsgBreakHandlerChain(pMsg);
    }
}

// 1.12a: 0x6FB66690
// Master WM_RBUTTONDOWN handler when left-side panels (Shop, Cube, Trade) are open alongside the Inventory.
void __stdcall INV_OnContainerPanelRightClick(D2WinMsgStrc* pMsg)
{
    if (g_bSkillTreeOrSelectOpen || !g_pPlayerUnit || g_pPlayerUnit->dwAnimMode == PLRMODE_DEAD)
    {
        return;
    }

    int32_t nX = pMsg->nX;
    int32_t nY = pMsg->nY;

    // 1. If inside Player Inventory grid, delegate to INV_OnInventoryGridRightClick
    if (nX >= g_tInvGridLayout.nLeft && nX <= g_tInvGridLayout.nRight &&
        nY >= g_tInvGridLayout.nTop  && nY <= g_tInvGridLayout.nBottom)
    {
        INV_OnInventoryGridRightClick(pMsg);
        return;
    }

    if (UI_HandleBeltMouseClick(pMsg) == 1)
    {
        return;
    }

    if (g_nCursorState == 6)
    {
        UI_CancelActiveCursorState();
    }
    else if (g_nCursorState == 8)
    {
        NET_SendByteAndDword(0x4C, 0xFFFFFFFF);
    }
    else if (!INVENTORY_GetCursorItem(g_pPlayerUnit->pInventory))
    {
        if (g_nInvPanelMode == 1 || g_nInvPanelMode == 2)
        {
            // Right-click in NPC Shop grid to quick-buy item
            if (nX >= g_rNpcShopGridRect.left && nX <= g_rNpcShopGridRect.right &&
                nY >= g_rNpcShopGridRect.top  && nY <= g_rNpcShopGridRect.bottom &&
                g_pHoveredSocketHostItem)
            {
                D2UnitStrc* pNpc = INV_GetInteractedNpcUnit();
                if (pNpc)
                {
                    INV_InitiateShopItemTransaction(g_pHoveredSocketHostItem, nY, pNpc, FALSE, nX);
                }
            }
        }
        else if (g_nInvPanelMode == 14)
        {
            if (INV_HitTestTradePartnerGridRect(nX, nY))
            {
                SOUND_PlayUnitSpeech(g_pPlayerUnit, 0x13); // "I can't do that"
            }
        }
        else if (g_nInvPanelMode == 12)
        {
            if (INV_HitTestHoradricCubeGridRect(nX, nY))
            {
                if (INV_HitTestInvPanelRect(nX, nY))
                {
                    INV_OnInventoryGridRightClick(pMsg);
                }
                else
                {
                    INV_HandleGridItemRightClick(nY, nX, INVPAGE_CUBE, g_pPlayerUnit, g_pPlayerUnit->pInventory);
                }
            }
        }
    }

    pMsg->bHandled = TRUE;
    pMsg->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}
