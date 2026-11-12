// =============================================================================
// D2Client.dll 1.12a (ImageBase = 0x6FAB0000)
// Reconstructed Translation Unit #09 of 55: UI/useitem.cpp
//
// Source Assertion Path: "..\Source\D2Client\UI\useitem.cpp" (0x6FB85DEC)
// .text Range : 0x6FAE8B60..0x6FAE9D60 (0x01200 bytes, 28 functions)
// Anchor Range: 0x6FAE8D20..0x6FAE9D60
//
// Responsibilities:
//   - Quest Item Use / Upgrade Panel ("menu\\upgrade" & "menu\\horadricback"):
//     * Mode 0: Tal Rasha's Horadric Orifice (inserting Horadric Staff 'hst ', C2S 0x44)
//     * Mode 1: Town NPC Quest Item Upgrade:
//       - Charsi (154): Imbue Item (D2Common.#10757)
//       - Larzuk (511): Add Sockets (D2Common.#11119)
//       - Anya / Drehya (512): Personalize Item (D2Common.#11160)
//   - Item slot validation, sprite preview rendering, button hit-testing, and wait dialog
//   - S2C quest item action dispatch (0x6FAE9B10)
//   - Coupled with UI_inv.cpp right-click consumable/tome/scroll usage (C2S 0x20, 0x26, 0x27)
//     and identify / town portal cursor states (0x6FBB1674, 0x6FBB1664, 0x6FBB1668).
// =============================================================================

#include <stdint.h>
#include <string.h>
#include <windows.h>

#include <D2BasicTypes.h>
#include <Units/Units.h>
#include <D2Inventory.h>
#include <D2Items.h>
#include <D2DataTbls.h>
#include <D2Lang.h>
#include <D2Gfx.h>
#include <Texture.h>
#include <Window.h>
#include <D2CMP.h>
#include <D2WinFont.h>
#include <Fog.h>
#include <Storm.h>
#include <D2Unicode.h>

#pragma pack(push, 1)

// Storm window message dispatch parameter block
struct D2StormMsgStrc
{
    HWND     hWnd;          // +0x00
    uint32_t uMessage;      // +0x04
    WPARAM   wParam;        // +0x08
    union {
        LPARAM lParam;      // +0x0C
        struct {
            uint16_t wMouseX; // +0x0C
            uint16_t wMouseY; // +0x0E
        };
    };
    uint32_t dwUnk10;       // +0x10
    uint32_t dwUnk14;       // +0x14
    int32_t  bHandled;      // +0x18
    int32_t  dwReturn;      // +0x1C
};
static_assert(sizeof(D2StormMsgStrc) == 0x20, "D2StormMsgStrc must be 0x20 bytes");

// 0x28-byte (40-byte) UseItem button descriptor (0x6FBA3CA8..0x6FBA3CF7)
struct D2UseItemButtonStrc
{
    uint16_t wTooltipStrId; // +0x00 (0x0FB1 = Imbue, 0x102F = Cancel)
    int32_t  nTooltipX;     // +0x02
    int32_t  nTooltipY;     // +0x06
    int32_t  nDrawX;        // +0x0A
    int32_t  nDrawY;        // +0x0E
    int32_t  nHitLeft;      // +0x12
    int32_t  nHitRight;     // +0x16
    int32_t  nHitTop;       // +0x1A
    int32_t  nHitBottom;    // +0x1E
    int32_t  nBaseFrame;    // +0x22 (16 = Confirm/Upgrade, 10 = Cancel)
    uint8_t  bIsPressed;    // +0x26
    uint8_t  bArmed;        // +0x27
};
static_assert(sizeof(D2UseItemButtonStrc) == 0x28, "D2UseItemButtonStrc must be 0x28 bytes");

// 0x0C-byte Storm message handler table entry (0x6FBA3D00..0x6FBA3D6B)
struct D2MsgHandlerRegStrc
{
    uint32_t dwMsgCategory; // +0x00 (0 = Window message, 3 = Key down)
    uint32_t dwMsgCode;     // +0x04 (WM_LBUTTONUP, VK_ESCAPE, etc.)
    void*    pfnHandler;    // +0x08
};
static_assert(sizeof(D2MsgHandlerRegStrc) == 0x0C, "D2MsgHandlerRegStrc must be 0x0C bytes");

// 17-byte C2S 0x44 (C2S_STAFFINTOORIFICE) packet
struct D2C2SStaffIntoOrificePacketStrc
{
    uint8_t  nOpcode;       // +0x00 (0x44)
    uint32_t dwPlayerGUID;  // +0x01
    uint32_t dwObjectGUID;  // +0x05
    uint32_t dwItemGUID;    // +0x09
    uint32_t dwState;       // +0x0D (2 = Cancel/Close, 3 = Confirm/Insert)
};
static_assert(sizeof(D2C2SStaffIntoOrificePacketStrc) == 0x11, "D2C2SStaffIntoOrificePacketStrc must be 17 bytes");

#pragma pack(pop)

// =============================================================================
// External Imports & D2Client Internal Prototypes
// =============================================================================
// D2Client internal cross-unit helpers
void __fastcall NPCMENU_SendCancelUpgrade(void);                                    // 0x6FACA430
void __fastcall NPCMENU_SendQuestUpgradeRequest(uint32_t dwItemGUID);               // 0x6FAC8410
void __fastcall NPCMENU_CloseActiveSession(int32_t nNpcGUID);                       // 0x6FACA1F0
void __fastcall NPCMENU_ResetState(void);                                           // 0x6FACB4C0
int32_t __fastcall CUNIT_HasActiveInteract(void);                                   // 0x6FACEAC0
void __stdcall  UNITSND_PlayUiOrVoiceSound(int32_t nSoundType, int32_t a2, int32_t a3, int32_t a4, int32_t a5); // 0x6FAE1FA0
void __fastcall DIALOG_SetLineText(void* pDialog, int32_t nLineIdx, const Unicode* pText, int32_t nColor, int32_t nFont, int32_t nAlign, int32_t a7, int32_t a8); // 0x6FB12DF0
void __fastcall DIALOG_Draw(void* pDialog);                                         // 0x6FB12F40
void __fastcall DIALOG_Destroy(void* pDialog);                                      // 0x6FB13290
void* __stdcall DIALOG_Create(int32_t nX, int32_t nY, void* pfnCallback, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8, int32_t a9, int32_t a10, int32_t a11); // 0x6FB13600
void __fastcall SMSG_UnregisterWindowMsg(HWND hWnd, uint32_t dwCategory, uint32_t dwCode, void* pfnHandler); // 0x6FB1CE40
void __fastcall SMSG_RegisterWindowMsg(HWND hWnd, uint32_t dwCategory, uint32_t dwCode, void* pfnHandler);   // 0x6FB1CEC0
void __fastcall SMSG_RunModalLoop(void* pfnRenderCallback);                         // 0x6FB1CF40
int32_t __fastcall MSG_SendPacket(void* pPacket, size_t nSize);                     // 0x6FB291A0
void __fastcall GAME_UpdateCursorFromMsg(D2StormMsgStrc* pMsg);                     // 0x6FB308F0
void __stdcall  UI_CloseConflictingPanels(int32_t bKeepLeft, int32_t bKeepRight);   // 0x6FB3B480
void __stdcall  UI_ResetRightPanelLayout(void);                                     // 0x6FB3AA10
D2CellFileStrc* __fastcall UI_LoadPanelCelAsset(const char* szRelPath);             // 0x6FB3C170
void __fastcall UI_SetPanelVisible(int32_t nPanelId, int32_t bClose, int32_t nArg); // 0x6FB3EF00
void __stdcall  GFX_DrawItemSprite(int32_t nDrawX, int32_t nDrawY);                 // 0x6FB43380
int32_t __stdcall ITEM_PlayPickupDropSound(D2UnitStrc* pItem, int32_t a2, int32_t a3, int32_t a4, int32_t a5); // 0x6FB45920
void __fastcall CMDTBL_PickUpSlotItemToCursor(D2UnitStrc* pItem);                   // 0x6FB4EE90
void __stdcall  CMDTBL_DrawModalPrompt(void);                                       // 0x6FB4F5B0
int32_t __stdcall INV_HitTestCloseButton(void);                                     // 0x6FB567D0
void __fastcall INV_UpdateItemHoverTooltip(D2StormMsgStrc* pMsg);                   // 0x6FB5F070
int32_t __stdcall INV_HandleLButtonUp(void);                                        // 0x6FB61130
int32_t __stdcall INV_HandleLButtonDown(void);                                      // 0x6FB65A70
int32_t __stdcall GFXUTIL_BuildItemCelContext(D2UnitStrc* pItem, D2CellBufferStrc* pCelBuf, int32_t* pOutColor); // 0x6FB679A0
void __stdcall  GFXUTIL_GetItemSpriteSize(D2UnitStrc* pItem, int32_t a2, int32_t a3, int32_t* pWidth, int32_t* pHeight); // 0x6FB68010
void __fastcall VIEW_SetViewportRect(int32_t nLeft, int32_t nTop, int32_t nRight, int32_t nBottom, int32_t nOffsetX); // 0x6FB6A990
D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileDefault(const char* szPath, int32_t nType); // 0x6FB6EF70

// =============================================================================
// Module Globals (.data / .bss)
// =============================================================================
static int32_t  g_nUseItemPanelMode     = 0;        // 0x6FBC47A8 (0 = Horadric Orifice, 1 = NPC Upgrade)
static int32_t  g_nUseItemState         = 0;        // 0x6FBC47AC (0 = Closed, 1 = Empty, 2 = ItemInSlot, 3 = WaitingResponse)
static uint32_t g_dwUseItemUnkB0        = 0;        // 0x6FBC47B0
static uint32_t g_dwUseItemTargetGUID   = 0;        // 0x6FBC47B4 (Orifice object GUID)
static uint32_t g_dwUseItemSlotGUID     = 0;        // 0x6FBC47B8 (Item GUID placed in upgrade slot)
static void*    g_pUseItemWaitDialog    = nullptr;  // 0x6FBC47BC
static uint32_t g_dwUseItemLastClickTick= 0;        // 0x6FBC47C0

static D2CellFileStrc* g_pUpgradeCel      = nullptr;// 0x6FBCC350 ("menu\\upgrade")
static D2CellFileStrc* g_pHoradricBackCel = nullptr;// 0x6FBCC354 ("menu\\horadricback")
extern D2CellFileStrc* gpBuySellBtnCel;             // 0x6FBCBD4C ("%s\\ui\\panel\\buysellbtn")

extern D2UnitStrc* gpClientPlayer;                  // 0x6FBCC3D0
extern int32_t     gnScreenWidth;                   // 0x6FB8C6E0
extern int32_t     gnScreenHeight;                  // 0x6FB8C6E4
extern int32_t     gnMouseScreenX;                  // 0x6FBB1634
extern int32_t     gnMouseScreenY;                  // 0x6FBB1638
extern int32_t     gnActiveNpcClassId;              // 0x6FBCB62D
extern int32_t     gnActiveNpcGUID;                 // 0x6FBCB625
extern int32_t     gbNpcMenuOpen;                   // 0x6FBCB629
extern D2UnitStrc* gapServerSideUnitHashTables[6][128]; // 0x6FBCA960
#define gapClientItemHashTable (gapServerSideUnitHashTables[4])
extern D2UnitStrc* gpCursorLockedItem;              // 0x6FBB1644
extern int32_t     gnCursorTargetMode;              // 0x6FBB1664
extern int32_t     gnCursorTargetSubType;           // 0x6FBB1668
extern int32_t     gnCursorSpriteType;              // 0x6FBB1674
extern int32_t     gbUseItemPanelOpen;              // 0x6FBCC120
extern int32_t     gaPanelStates[38];               // 0x6FBB2B58
#define gnRightPanelOpenFlag (gaPanelStates[14])
extern int32_t     gnEscMenuOpenFlag;               // 0x6FBB5490
extern int32_t     gnViewportShiftMode;             // 0x6FBCC1D0
extern int32_t     gnViewportShiftOffsetX;          // 0x6FBCC1D4
extern int32_t     gnViewportWidth;                 // 0x6FBCD524
extern int32_t     gnViewportHeight;                // 0x6FBCD528
extern int32_t     gnViewportTopOffset;             // 0x6FBCC028

// 0x6FBA3CA8..0x6FBA3CF7: UseItem Confirm & Cancel button table
static int32_t g_nUseItemButtonCount = 2;           // 0x6FBA3CF8
static D2UseItemButtonStrc g_aUseItemButtons[2] = {
    // [0] 0x6FBA3CA8: Confirm / Upgrade / Imbue / Socket / Personalize button
    { 0x0FB1, 122, 256, 122, 256, 122, 154, 224, 256, 16, 0, 0 },
    // [1] 0x6FBA3CD0: Cancel / Close button
    { 0x102F, 177, 256, 177, 256, 177, 209, 224, 256, 10, 0, 0 },
};

// Forward declarations of message handlers
void __stdcall USEITEM_OnModalLButtonDownClose(D2StormMsgStrc* pMsg); // 0x6FAE8B60
void __stdcall USEITEM_OnCharMsg(D2StormMsgStrc* pMsg);               // 0x6FAE8CC0
void __stdcall USEITEM_OnMouseMove(D2StormMsgStrc* pMsg);             // 0x6FAE9510
void __stdcall USEITEM_OnEscapeOrSpaceKey(D2StormMsgStrc* pMsg);      // 0x6FAE9540
void __stdcall USEITEM_OnMouseButtonUp(D2StormMsgStrc* pMsg);         // 0x6FAE97D0
void __stdcall USEITEM_OnMouseButtonDown(D2StormMsgStrc* pMsg);       // 0x6FAE9BE0

// 0x6FBA3D00..0x6FBA3D6B: Storm message handler registration table
static const D2MsgHandlerRegStrc g_aUseItemMsgHandlers[8] = {
    { 0, WM_LBUTTONUP,   reinterpret_cast<void*>(USEITEM_OnMouseButtonUp)    }, // 0x6FBA3D00
    { 0, WM_LBUTTONDOWN, reinterpret_cast<void*>(USEITEM_OnMouseButtonDown)  }, // 0x6FBA3D0C
    { 0, WM_RBUTTONDOWN, reinterpret_cast<void*>(USEITEM_OnMouseButtonDown)  }, // 0x6FBA3D18
    { 0, WM_RBUTTONUP,   reinterpret_cast<void*>(USEITEM_OnMouseButtonUp)    }, // 0x6FBA3D24
    { 0, WM_MOUSEMOVE,   reinterpret_cast<void*>(USEITEM_OnMouseMove)        }, // 0x6FBA3D30
    { 3, VK_ESCAPE,      reinterpret_cast<void*>(USEITEM_OnEscapeOrSpaceKey) }, // 0x6FBA3D3C
    { 3, VK_SPACE,       reinterpret_cast<void*>(USEITEM_OnEscapeOrSpaceKey) }, // 0x6FBA3D48
    { 0, WM_CHAR,        reinterpret_cast<void*>(USEITEM_OnCharMsg)          }, // 0x6FBA3D54
};

static const D2MsgHandlerRegStrc g_sModalCloseMsgHandler = {
    0, WM_LBUTTONDOWN, reinterpret_cast<void*>(USEITEM_OnModalLButtonDownClose) // 0x6FBA3D60
};

// =============================================================================
// Reconstructed Functions (0x6FAE8B60..0x6FAE9D60)
// =============================================================================

// 1.12a: 0x6FAE8B60 (__stdcall)
void __stdcall USEITEM_OnModalLButtonDownClose(D2StormMsgStrc* pMsg)
{
    SendMessageA(pMsg->hWnd, WM_CLOSE, 0, 0);
    pMsg->bHandled = 0;
    pMsg->dwReturn = 0;
}

// 1.12a: 0x6FAE8B90 (__stdcall)
void __stdcall USEITEM_ClearMsgResultA(D2StormMsgStrc* pMsg)
{
    pMsg->bHandled = 0;
    pMsg->dwReturn = 0;
}

// 1.12a: 0x6FAE8BA0 (__stdcall)
void __stdcall USEITEM_ClearMsgResultB(D2StormMsgStrc* pMsg)
{
    pMsg->bHandled = 0;
    pMsg->dwReturn = 0;
}

// 1.12a: 0x6FAE8BB0 (__stdcall)
void __stdcall USEITEM_ClearMsgResultC(D2StormMsgStrc* pMsg)
{
    pMsg->bHandled = 0;
    pMsg->dwReturn = 0;
}

// 1.12a: 0x6FAE8BC0 (__stdcall)
int32_t __stdcall USEITEM_ModalRenderFrame(int32_t nUnused)
{
    (void)nUnused;
    if (D2GFX_StartDraw(1, 0, 0, 0))
    {
        D2GFX_ClearScreen(0);
        CMDTBL_DrawModalPrompt();
        D2GFX_EndScene();
    }
    return 0;
}

// 1.12a: 0x6FAE8BF0 (__stdcall)
int32_t __stdcall USEITEM_RunModalErrorLoop(void)
{
    SMsgPushRegisterState(WINDOW_GetWindow());

    HWND hWnd = WINDOW_GetWindow();
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\useitem.cpp", 377);
        exit(-1);
    }

    SMSG_RegisterWindowMsg(
        hWnd,
        g_sModalCloseMsgHandler.dwMsgCategory,
        g_sModalCloseMsgHandler.dwMsgCode,
        g_sModalCloseMsgHandler.pfnHandler);

    hWnd = WINDOW_GetWindow();
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\useitem.cpp", 310);
        exit(-1);
    }
    SMsgRegisterMessage(hWnd, WM_LBUTTONDOWN, reinterpret_cast<WNDPROC>(USEITEM_ClearMsgResultC));

    hWnd = WINDOW_GetWindow();
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\useitem.cpp", 310);
        exit(-1);
    }
    SMsgRegisterMessage(hWnd, WM_LBUTTONDOWN, reinterpret_cast<WNDPROC>(USEITEM_ClearMsgResultB));

    hWnd = WINDOW_GetWindow();
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\useitem.cpp", 310);
        exit(-1);
    }
    SMsgRegisterMessage(hWnd, WM_LBUTTONDOWN, reinterpret_cast<WNDPROC>(USEITEM_ClearMsgResultA));

    SMSG_RunModalLoop(reinterpret_cast<void*>(USEITEM_ModalRenderFrame));
    SMsgPopRegisterState(WINDOW_GetWindow());
    return 1;
}

// 1.12a: 0x6FAE8CB0 (__stdcall)
void __stdcall USEITEM_NoOpA(void)
{
}

// 1.12a: 0x6FAE8CC0 (__stdcall)
void __stdcall USEITEM_OnCharMsg(D2StormMsgStrc* pMsg)
{
    pMsg->bHandled = 1;
    pMsg->dwReturn = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FAE8CE0 (__stdcall)
void __stdcall USEITEM_NoOpB(void)
{
}

// 1.12a: 0x6FAE8CF0 (__stdcall)
int32_t __stdcall USEITEM_WaitDialogCallback(void)
{
    return 0;
}

// 1.12a: 0x6FAE8D00 (__stdcall)
D2UnitStrc* __stdcall USEITEM_GetPlayerCursorItem(void)
{
    if (gpClientPlayer != nullptr)
    {
        return INVENTORY_GetCursorItem(gpClientPlayer->pInventory);
    }
    return nullptr;
}

// 1.12a: 0x6FAE8D20 (__stdcall)
void __stdcall USEITEM_DrawPanel(void)
{
    const int32_t nPrevFont = D2Win_10127_SetFont(1);

    if (g_nUseItemPanelMode == 0 || g_nUseItemPanelMode == 1)
    {
        D2CellBufferStrc sCelBuf = {};
        sCelBuf.pCelFile = (g_nUseItemPanelMode == 0) ? g_pHoradricBackCel : g_pUpgradeCel;
        sCelBuf.nFrame   = 0;
        TEXTURE_CelDraw(&sCelBuf, 105, 270, -1, (DrawMode)5, nullptr);

        if (gpBuySellBtnCel == nullptr)
        {
            char szBtnPath[260];
            wsprintfA(szBtnPath, "%s\\ui\\panel\\buysellbtn", "DATA\\GLOBAL");
            gpBuySellBtnCel = ARCHIVE_LoadCellFileDefault(szBtnPath, 0);
        }
        sCelBuf.pCelFile = gpBuySellBtnCel;

        // Draw Confirm & Cancel buttons
        for (int32_t i = 0; i < g_nUseItemButtonCount; ++i)
        {
            const D2UseItemButtonStrc& rBtn = g_aUseItemButtons[i];
            sCelBuf.nFrame = rBtn.nBaseFrame + (rBtn.bIsPressed != 0 ? 1 : 0);
            TEXTURE_CelDraw(&sCelBuf, rBtn.nDrawX, rBtn.nDrawY, -1, (DrawMode)5, nullptr);
        }

        // Draw button hover tooltip if cursor is within the left panel viewport
        const int32_t nMouseX = gnMouseScreenX;
        const int32_t nMouseY = gnMouseScreenY;
        if (nMouseX < 320 && nMouseY < (gnScreenHeight - 48))
        {
            for (int32_t i = 0; i < g_nUseItemButtonCount; ++i)
            {
                const D2UseItemButtonStrc& rBtn = g_aUseItemButtons[i];
                if (nMouseX >= rBtn.nHitLeft && nMouseX <= rBtn.nHitRight &&
                    nMouseY >= rBtn.nHitTop  && nMouseY <= rBtn.nHitBottom)
                {
                    uint16_t wStrId = rBtn.wTooltipStrId;
                    if (wStrId == 0x0FB1)
                    {
                        if (g_nUseItemPanelMode == 0)
                        {
                            wStrId = 0x0D49; // Horadric Orifice accept
                        }
                        if (gnActiveNpcClassId == 511) // MONSTER_LARZUK
                        {
                            wStrId = 0x58DC; // Add Sockets
                        }
                        else if (gnActiveNpcClassId == 512) // MONSTER_DREHYA (Anya)
                        {
                            wStrId = 0x58DD; // Personalize
                        }
                    }

                    const Unicode* pwszTooltip = D2LANG_GetStringFromTblIndex(wStrId);
                    const int32_t nTextW = D2Win_10121_GetTextWidth(pwszTooltip);
                    const int32_t nFontH = D2Win_10125() + 6;
                    const int32_t nAscent = D2Win_10125();

                    const int32_t nHalfW   = nTextW / 2;
                    const int32_t nBoxLeft = (rBtn.nTooltipX - nHalfW) + 10;
                    const int32_t nBoxRight= nBoxLeft + nTextW + 12;
                    const int32_t nBoxTop  = (rBtn.nTooltipY - nAscent) - 42;
                    const int32_t nBoxBot  = nBoxTop + nFontH;

                    D2GFX_DrawBox(nBoxLeft, nBoxTop, nBoxRight, nBoxBot, 0, (DrawMode)2);
                    D2Win_10117_DrawText(
                        pwszTooltip,
                        (rBtn.nTooltipX - nHalfW) + 16,
                        rBtn.nTooltipY - 38,
                        0,
                        0);
                }
            }
        }

        // Draw wrapped NPC upgrade description header in Mode 1
        if (g_nUseItemPanelMode == 1)
        {
            uint16_t wDescStrId = 0x275C; // Charsi imbue description
            if (gnActiveNpcClassId == 511)
            {
                wDescStrId = 0x58DE;      // Larzuk socket description
            }
            else if (gnActiveNpcClassId == 512)
            {
                wDescStrId = 0x58DB;      // Anya personalize description
            }

            const Unicode* pwszDesc = D2LANG_GetStringFromTblIndex(wDescStrId);
            int nLineCount = 0;
            D2WrappedTextNodeStrc* pLineNode = D2Win_10199(pwszDesc, &nLineCount, 300);
            int32_t nDrawY = 30;
            while (pLineNode != nullptr)
            {
                const int32_t nLineW = D2Win_10121_GetTextWidth(pLineNode->pwszLine);
                D2Win_10117_DrawText(pLineNode->pwszLine, 160 - (nLineW / 2), nDrawY, 4, 0);

                D2WrappedTextNodeStrc* pNext = pLineNode->pNext;
                FOG_FreeClientMemory(pLineNode->pwszLine, "..\\Source\\D2Client\\UI\\useitem.cpp", 707, 0);
                FOG_FreeClientMemory(pLineNode, "..\\Source\\D2Client\\UI\\useitem.cpp", 708, 0);
                nDrawY += 20;
                pLineNode = pNext;
            }
        }
    }

    if (g_pUseItemWaitDialog != nullptr)
    {
        DIALOG_Draw(g_pUseItemWaitDialog);
    }

    D2Win_10127_SetFont(nPrevFont);
}

// 1.12a: 0x6FAE9010 (__stdcall)
void __stdcall USEITEM_UpdateButtonPressState(int32_t nMouseX, int32_t nMouseY)
{
    for (int32_t i = 0; i < g_nUseItemButtonCount; ++i)
    {
        g_aUseItemButtons[i].bIsPressed = 0;
        g_aUseItemButtons[i].bArmed     = 0;
    }

    for (int32_t i = 0; i < g_nUseItemButtonCount; ++i)
    {
        D2UseItemButtonStrc& rBtn = g_aUseItemButtons[i];
        if (nMouseX >= rBtn.nHitLeft && nMouseX <= rBtn.nHitRight &&
            nMouseY >= rBtn.nHitTop  && nMouseY <= rBtn.nHitBottom)
        {
            UNITSND_PlayUiOrVoiceSound(1, 0, 0, 0, 0);
            rBtn.bArmed     = 1;
            rBtn.bIsPressed = 1;
        }
    }
}

// 1.12a: 0x6FAE9090 (__usercall: eax = pCursorItem)
void __fastcall USEITEM_ToggleSlotItem(D2UnitStrc* pCursorItem)
{
    if (g_nUseItemState == 1)
    {
        g_nUseItemState        = 2;
        gpCursorLockedItem     = nullptr;
        gnCursorSpriteType     = 1;
        gnCursorTargetMode     = 5;
        gnCursorTargetSubType  = 0;

        if (pCursorItem != nullptr)
        {
            g_dwUseItemSlotGUID = pCursorItem->dwUnitId;
            const int32_t nSnd = ITEM_PlayPickupDropSound(pCursorItem, 0, 0, 0, 0);
            UNITSND_PlayUiOrVoiceSound(nSnd, 0, 0, 0, 0);
        }
    }
    else if (g_nUseItemState == 2)
    {
        g_nUseItemState     = 1;
        g_dwUseItemSlotGUID = 0;
        if (pCursorItem != nullptr)
        {
            CMDTBL_PickUpSlotItemToCursor(pCursorItem);
        }
        UNITSND_PlayUiOrVoiceSound(1, 0, 0, 0, 0);
    }
}

// 1.12a: 0x6FAE9120 (__stdcall)
void __stdcall USEITEM_LoadPanelAssets(void)
{
    g_nUseItemState         = 0;
    g_dwUseItemTargetGUID   = 0;
    g_dwUseItemUnkB0        = 0;
    g_dwUseItemLastClickTick= 0;

    g_pUpgradeCel      = UI_LoadPanelCelAsset("menu\\upgrade");
    g_pHoradricBackCel = UI_LoadPanelCelAsset("menu\\horadricback");

    for (int32_t i = 0; i < g_nUseItemButtonCount; ++i)
    {
        g_aUseItemButtons[i].bIsPressed = 0;
        g_aUseItemButtons[i].bArmed     = 0;
    }
}

// 1.12a: 0x6FAE9180 (__stdcall)
void __stdcall USEITEM_ClosePanel(void)
{
    g_nUseItemState = 0;
    USEITEM_ToggleSlotItem(USEITEM_GetPlayerCursorItem());

    HWND hWnd = WINDOW_GetWindow();
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\useitem.cpp", 396);
        exit(-1);
    }

    for (int32_t i = 0; i < 8; ++i)
    {
        SMSG_UnregisterWindowMsg(
            hWnd,
            g_aUseItemMsgHandlers[i].dwMsgCategory,
            g_aUseItemMsgHandlers[i].dwMsgCode,
            g_aUseItemMsgHandlers[i].pfnHandler);
    }

    if (g_pUseItemWaitDialog != nullptr)
    {
        DIALOG_Destroy(g_pUseItemWaitDialog);
        g_pUseItemWaitDialog = nullptr;
    }

    UI_SetPanelVisible(14, 1, 0);
    gbUseItemPanelOpen = 0;
}

// 1.12a: 0x6FAE9230 (__stdcall)
int32_t __stdcall USEITEM_OnConfirmButton(void)
{
    if (g_nUseItemState != 2)
    {
        return 0;
    }

    UNITSND_PlayUiOrVoiceSound(2, 0, 0, 0, 0);

    g_pUseItemWaitDialog = DIALOG_Create(
        150,
        130,
        reinterpret_cast<void*>(USEITEM_WaitDialogCallback),
        0, 1, 0, 1, 0, 0, 0, 1);

    if (g_pUseItemWaitDialog == nullptr)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\useitem.cpp", 1321);
        exit(-1);
    }

    *reinterpret_cast<int32_t*>( static_cast<uint8_t*>(g_pUseItemWaitDialog) + 0x5C ) = 0;

    const Unicode* pwszPleaseWait = D2LANG_GetStringFromTblIndex(0x0D19);
    DIALOG_SetLineText(g_pUseItemWaitDialog, 0, pwszPleaseWait, 15, 0, 1, 0, 0);

    g_nUseItemState = 3;

    if (g_nUseItemPanelMode == 0)
    {
        D2C2SStaffIntoOrificePacketStrc sPkt = {};
        sPkt.nOpcode      = 0x44; // C2S_STAFFINTOORIFICE
        sPkt.dwPlayerGUID = (gpClientPlayer != nullptr) ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
        sPkt.dwObjectGUID = g_dwUseItemTargetGUID;
        sPkt.dwItemGUID   = g_dwUseItemSlotGUID;
        sPkt.dwState      = 3;
        MSG_SendPacket(&sPkt, sizeof(sPkt));
        return 1;
    }
    else
    {
        NPCMENU_SendQuestUpgradeRequest(g_dwUseItemSlotGUID);
        return 1;
    }
}

// 1.12a: 0x6FAE9350 (__stdcall)
void __stdcall USEITEM_FreePanelAssets(void)
{
    if (g_nUseItemState != 0)
    {
        USEITEM_ClosePanel();
    }

    if (g_pUpgradeCel != nullptr)
    {
        D2CMP_CelFileFreeHardware(g_pUpgradeCel);
        FOG_FreeClientMemory(g_pUpgradeCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
        g_pUpgradeCel = nullptr;
    }

    if (g_pHoradricBackCel != nullptr)
    {
        D2CMP_CelFileFreeHardware(g_pHoradricBackCel);
        FOG_FreeClientMemory(g_pHoradricBackCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 122, 0);
        g_pHoradricBackCel = nullptr;
    }
}

// 1.12a: 0x6FAE93C0 (__stdcall)
void __stdcall USEITEM_UpdateAndRender(void)
{
    if (g_nUseItemState == 0)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\useitem.cpp", 797);
        exit(-1);
    }

    if (gpClientPlayer == nullptr || gpClientPlayer->dwAnimMode == PLRMODE_DEAD)
    {
        USEITEM_ClosePanel();
        return;
    }

    USEITEM_DrawPanel();

    if (g_nUseItemState != 2)
    {
        return;
    }

    // Find placed item unit in client item hash table
    D2UnitStrc* pSlotItem = gapClientItemHashTable[g_dwUseItemSlotGUID & 0x7F];
    while (pSlotItem != nullptr && pSlotItem->dwUnitId != g_dwUseItemSlotGUID)
    {
        pSlotItem = pSlotItem->pListNext;
    }
    if (pSlotItem == nullptr)
    {
        return;
    }

    if (pSlotItem->dwUnitType != UNIT_ITEM)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\useitem.cpp", 90);
        exit(-1);
    }

    D2CellBufferStrc sItemCelBuf = {};
    int32_t nItemColor = -1;
    if (!GFXUTIL_BuildItemCelContext(pSlotItem, &sItemCelBuf, &nItemColor))
    {
        return;
    }
    if (!D2CMP_SpriteValidate(&sItemCelBuf, 0, 1) || sItemCelBuf.pCelFile == nullptr)
    {
        return;
    }

    int32_t nSpriteW = 0;
    int32_t nSpriteH = 0;
    GFXUTIL_GetItemSpriteSize(pSlotItem, 0, 0, &nSpriteW, &nSpriteH);

    UNITS_ChangeAnimMode(pSlotItem, 0);

    int32_t nPadX = 0;
    if (static_cast<uint32_t>(nSpriteW) < 112u)
    {
        nPadX = (114u - static_cast<uint32_t>(nSpriteW)) >> 1;
    }
    const int32_t nDrawX = 106 + nPadX;
    const int32_t nDrawY = 167 - (static_cast<uint32_t>(nSpriteH) >> 1);

    GFX_DrawItemSprite(nDrawY, nDrawX);
    UNITS_ChangeAnimMode(pSlotItem, 4);
}

// 1.12a: 0x6FAE9510 (__stdcall)
void __stdcall USEITEM_OnMouseMove(D2StormMsgStrc* pMsg)
{
    D2Win_10130_DrawFramedTextEx(nullptr, -1, -1, 0, 0, 0);
    GAME_UpdateCursorFromMsg(pMsg);
    if (g_nUseItemState == 1)
    {
        INV_UpdateItemHoverTooltip(pMsg);
    }
}

// 1.12a: 0x6FAE9540 (__stdcall)
void __stdcall USEITEM_OnEscapeOrSpaceKey(D2StormMsgStrc* pMsg)
{
    pMsg->bHandled = 1;
    pMsg->dwReturn = 0;
    SMsgBreakHandlerChain(pMsg);

    if (g_nUseItemState != 1 && g_nUseItemState != 2)
    {
        return;
    }

    if (g_nUseItemState == 2)
    {
        D2UnitStrc* pCursorItem = USEITEM_GetPlayerCursorItem();
        gpCursorLockedItem    = pCursorItem;
        gnCursorTargetSubType = 0;
        gnCursorTargetMode    = 5;
        gnCursorSpriteType    = (pCursorItem == nullptr) ? 1 : 4;
    }

    if (g_nUseItemPanelMode == 0)
    {
        D2C2SStaffIntoOrificePacketStrc sPkt = {};
        sPkt.nOpcode      = 0x44;
        sPkt.dwPlayerGUID = (gpClientPlayer != nullptr) ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
        sPkt.dwObjectGUID = g_dwUseItemTargetGUID;
        sPkt.dwItemGUID   = 0;
        sPkt.dwState      = 2;
        MSG_SendPacket(&sPkt, sizeof(sPkt));
        USEITEM_ClosePanel();
    }
    else
    {
        NPCMENU_SendCancelUpgrade();
        USEITEM_ClosePanel();
    }
}

// 1.12a: 0x6FAE9630 (__stdcall)
int32_t __stdcall USEITEM_OnCancelButton(void)
{
    if (g_nUseItemState == 3)
    {
        return 0;
    }

    if (g_nUseItemState == 2)
    {
        D2UnitStrc* pCursorItem = USEITEM_GetPlayerCursorItem();
        gpCursorLockedItem    = pCursorItem;
        gnCursorTargetSubType = 0;
        gnCursorTargetMode    = 5;
        gnCursorSpriteType    = (pCursorItem != nullptr) ? 4 : 1;
    }

    UNITSND_PlayUiOrVoiceSound(1, 0, 0, 0, 0);

    if (g_nUseItemPanelMode == 0)
    {
        D2C2SStaffIntoOrificePacketStrc sPkt = {};
        sPkt.nOpcode      = 0x44;
        sPkt.dwPlayerGUID = (gpClientPlayer != nullptr) ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
        sPkt.dwObjectGUID = g_dwUseItemTargetGUID;
        sPkt.dwItemGUID   = 0;
        sPkt.dwState      = 2;
        MSG_SendPacket(&sPkt, sizeof(sPkt));
        USEITEM_ClosePanel();
        return 1;
    }
    else
    {
        NPCMENU_SendCancelUpgrade();
        USEITEM_ClosePanel();
        return 1;
    }
}

// 1.12a: 0x6FAE9710 (__userpurge: edi = nMouseY, [esp+4] = nMouseX)
int32_t __fastcall USEITEM_ReleaseButtonsAndDispatch(int32_t nMouseY, int32_t nMouseX)
{
    int32_t bDispatched = 0;

    for (int32_t i = 0; i < g_nUseItemButtonCount; ++i)
    {
        const D2UseItemButtonStrc& rBtn = g_aUseItemButtons[i];
        if (rBtn.bArmed != 0 &&
            nMouseX >= rBtn.nHitLeft && nMouseX <= rBtn.nHitRight &&
            nMouseY >= rBtn.nHitTop  && nMouseY <= rBtn.nHitBottom)
        {
            UNITSND_PlayUiOrVoiceSound(1, 0, 0, 0, 0);
            if (i == 0)
            {
                USEITEM_OnConfirmButton();
            }
            else
            {
                USEITEM_OnCancelButton();
            }
            bDispatched = 1;
        }
    }

    for (int32_t i = 0; i < g_nUseItemButtonCount; ++i)
    {
        g_aUseItemButtons[i].bIsPressed = 0;
        g_aUseItemButtons[i].bArmed     = 0;
    }

    return bDispatched;
}

// 1.12a: 0x6FAE97C0 (__stdcall)
int32_t __stdcall USEITEM_CancelThunk(void)
{
    return USEITEM_OnCancelButton();
}

// 1.12a: 0x6FAE97D0 (__stdcall)
void __stdcall USEITEM_OnMouseButtonUp(D2StormMsgStrc* pMsg)
{
    pMsg->bHandled = 1;
    pMsg->dwReturn = 0;
    SMsgBreakHandlerChain(pMsg);

    const int32_t nMouseX = pMsg->wMouseX;
    const int32_t nMouseY = pMsg->wMouseY;

    if (g_nUseItemState == 2)
    {
        if (!USEITEM_ReleaseButtonsAndDispatch(nMouseY, nMouseX))
        {
            if (INV_HitTestCloseButton())
            {
                USEITEM_OnCancelButton();
            }
        }
    }
    else
    {
        if (!INV_HandleLButtonUp())
        {
            USEITEM_ReleaseButtonsAndDispatch(nMouseY, nMouseX);
        }
    }
}

// 1.12a: 0x6FAE9830 (__stdcall)
void __stdcall USEITEM_OpenPanel(void)
{
    UI_CloseConflictingPanels(0, 0);
    g_nUseItemState          = 1;
    g_dwUseItemLastClickTick = GetTickCount();

    HWND hWnd = WINDOW_GetWindow();
    if (hWnd == nullptr)
    {
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\useitem.cpp", 377);
        exit(-1);
    }

    for (int32_t i = 0; i < 8; ++i)
    {
        SMSG_RegisterWindowMsg(
            hWnd,
            g_aUseItemMsgHandlers[i].dwMsgCategory,
            g_aUseItemMsgHandlers[i].dwMsgCode,
            g_aUseItemMsgHandlers[i].pfnHandler);
    }

    const int32_t bWasRightPanelOpen = gnRightPanelOpenFlag;
    FOG_IsExpansion();

    if (!gnEscMenuOpenFlag)
    {
        if (gpClientPlayer == nullptr ||
            (gpClientPlayer->dwUnitType == UNIT_PLAYER && !CUNIT_HasActiveInteract()))
        {
            gnRightPanelOpenFlag = 1;
        }

        if (!bWasRightPanelOpen)
        {
            if (gnRightPanelOpenFlag)
            {
                const int32_t nShiftX = -(gnScreenWidth / 4);
                gnViewportShiftOffsetX = nShiftX;
                gnViewportShiftMode    = 1;
                D2GFX_SetViewMatrixAdjust(nShiftX);

                const int32_t nQuarterH = gnViewportHeight / 4;
                VIEW_SetViewportRect(
                    0,
                    gnViewportTopOffset,
                    gnViewportWidth,
                    gnViewportHeight - nQuarterH,
                    -nQuarterH);
            }
            else
            {
                gnViewportShiftOffsetX = 0;
                gnViewportShiftMode    = 0;
                D2GFX_SetViewMatrixAdjust(0);
                VIEW_SetViewportRect(0, gnViewportTopOffset, gnViewportWidth, gnViewportHeight, 0);
            }
        }
        else if (gnRightPanelOpenFlag)
        {
            UI_ResetRightPanelLayout();
            NPCMENU_ResetState();
            gnViewportShiftOffsetX = 0;
            gnViewportShiftMode    = 0;
            D2GFX_SetViewMatrixAdjust(0);
            VIEW_SetViewportRect(0, gnViewportTopOffset, gnViewportWidth, gnViewportHeight, 0);
        }

        gbUseItemPanelOpen = 1;
        return;
    }

    // If ESC menu or conflicting modal was open, cancel immediately
    if (g_nUseItemPanelMode == 0)
    {
        D2C2SStaffIntoOrificePacketStrc sPkt = {};
        sPkt.nOpcode      = 0x44;
        sPkt.dwPlayerGUID = (gpClientPlayer != nullptr) ? gpClientPlayer->dwUnitId : 0xFFFFFFFFu;
        sPkt.dwObjectGUID = g_dwUseItemTargetGUID;
        sPkt.dwItemGUID   = 0;
        sPkt.dwState      = 2;
        MSG_SendPacket(&sPkt, sizeof(sPkt));
        USEITEM_ClosePanel();
    }
    else
    {
        NPCMENU_SendCancelUpgrade();
        USEITEM_ClosePanel();
    }
}

// 1.12a: 0x6FAE9B10 (__usercall: esi = pPacket)
void __fastcall USEITEM_HandleS2CQuestItemAction(const uint8_t* pPacket)
{
    const uint8_t nSubAction = pPacket[5];
    switch (nSubAction)
    {
    case 0: // Open Horadric Orifice panel for object GUID at +1
        g_nUseItemPanelMode   = 0;
        g_dwUseItemTargetGUID = *reinterpret_cast<const uint32_t*>(&pPacket[1]);
        USEITEM_OpenPanel();
        break;

    case 1: // Close panel
        USEITEM_ClosePanel();
        break;

    case 4: // Upgrade rejected / reset wait dialog back to slot state 2
        g_nUseItemState = 2;
        if (g_pUseItemWaitDialog != nullptr)
        {
            DIALOG_Destroy(g_pUseItemWaitDialog);
            g_pUseItemWaitDialog = nullptr;
        }
        break;

    case 5: // Consume cursor item and close panel
        if (gpClientPlayer != nullptr)
        {
            INVENTORY_SetCursorItem(gpClientPlayer->pInventory, nullptr);
        }
        UNITSND_PlayUiOrVoiceSound((pPacket[6] != 0) ? 5 : 3, 0, 0, 0, 0);
        USEITEM_ClosePanel();
        break;

    case 6:
    case 7: // Cancel NPC upgrade and close panel
        USEITEM_ClosePanel();
        NPCMENU_SendCancelUpgrade();
        break;

    case 2:
    case 3:
    default:
        FOG_DisplayHalt("..\\Source\\D2Client\\UI\\useitem.cpp", 852);
        exit(-1);
    }
}

// 1.12a: 0x6FAE9BE0 (__stdcall)
void __stdcall USEITEM_OnMouseButtonDown(D2StormMsgStrc* pMsg)
{
    const int32_t nMouseX = pMsg->wMouseX;
    const int32_t nMouseY = pMsg->wMouseY;

    pMsg->bHandled = 1;
    pMsg->dwReturn = 0;
    SMsgBreakHandlerChain(pMsg);

    const uint32_t dwNow = GetTickCount();
    if (dwNow - g_dwUseItemLastClickTick < 400u)
    {
        return;
    }
    g_dwUseItemLastClickTick = dwNow;

    if (g_nUseItemState == 1)
    {
        if (INV_HandleLButtonDown())
        {
            return;
        }
        USEITEM_UpdateButtonPressState(nMouseX, nMouseY);
    }
    else if (g_nUseItemState == 2)
    {
        if (INV_HitTestCloseButton())
        {
            return;
        }
        USEITEM_UpdateButtonPressState(nMouseX, nMouseY);
    }

    D2UnitStrc* pCursorItem = USEITEM_GetPlayerCursorItem();
    if (pCursorItem == nullptr)
    {
        return;
    }

    // Check if cursor is inside the upgrade slot box [123..211, 106..220]
    if (nMouseX < 123 || nMouseX > 211 || nMouseY < 106 || nMouseY > 220)
    {
        return;
    }

    D2ItemsTxt* pItemsTxt = DATATBLS_GetItemsTxtRecord(pCursorItem->dwClassId);
    if (pItemsTxt == nullptr)
    {
        return;
    }

    if (ITEMS_CheckItemFlag(pCursorItem, ITEMFLAG_IDENTIFIED, 528, "..\\Source\\D2Client\\UI\\useitem.cpp"))
    {
        if (g_nUseItemPanelMode == 1)
        {
            // NPC Quest Upgrade mode: Charsi (154), Larzuk (511), Anya (512)
            if (gnActiveNpcClassId == 154 && ITEMS_IsRepairable(pCursorItem))
            {
                USEITEM_ToggleSlotItem(pCursorItem);
                return;
            }
            if (gnActiveNpcClassId == 511 && ITEMS_IsImbueable(pCursorItem))
            {
                USEITEM_ToggleSlotItem(pCursorItem);
                return;
            }
            if (gnActiveNpcClassId == 512 && ITEMS_IsSocketable(pCursorItem))
            {
                USEITEM_ToggleSlotItem(pCursorItem);
                return;
            }
        }
        else if (g_nUseItemPanelMode == 0)
        {
            // Horadric Orifice mode: requires Horadric Staff ('hst ')
            if (pItemsTxt->dwCode == ' tsh')
            {
                USEITEM_ToggleSlotItem(pCursorItem);
                return;
            }
        }
    }

    // Play "I can't use this yet" voice line
    UNITSND_PlayUiOrVoiceSound(3, 0, 0, 0, 0);
}

// 1.12a: 0x6FAE9D40 (__fastcall: ecx = pUnit, eax = szName)
void __fastcall USEITEM_SetUnitPlayerName(D2UnitStrc* pUnit, const char* szName)
{
    char* pszDest = reinterpret_cast<char*>(pUnit->pPlayerData) + 0x28;
    SStrCopy(pszDest, szName, 16);
}

// 1.12a: 0x6FAE9D50 (__usercall: eax = pUnit)
char* __fastcall USEITEM_GetUnitPlayerName(D2UnitStrc* pUnit)
{
    return reinterpret_cast<char*>(pUnit->pPlayerData) + 0x28;
}
